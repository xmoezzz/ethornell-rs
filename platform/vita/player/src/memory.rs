use std::alloc::{GlobalAlloc, Layout, System};
use std::ptr;
use vitasdk_sys::*;

struct VitaAllocator;
#[global_allocator]
static ALLOCATOR: VitaAllocator = VitaAllocator;

#[repr(C)]
struct Header {
    base: *mut u8,
    size: usize,
    align: usize,
    uid: i32,
}

unsafe impl GlobalAlloc for VitaAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let align = layout.align().max(std::mem::align_of::<Header>());
        let prefix = match std::mem::size_of::<Header>().checked_add(align - 1) {
            Some(n) => n & !(align - 1),
            None => return ptr::null_mut(),
        };
        let Some(size) = prefix.checked_add(layout.size()) else {
            return ptr::null_mut();
        };
        let Ok(storage) = Layout::from_size_align(size, align) else {
            return ptr::null_mut();
        };
        let mut base = unsafe { System.alloc(storage) };
        let mut uid = -1;
        if base.is_null() {
            if let Some((address, pool)) = cdram_alloc(storage) {
                base = address;
                uid = pool;
            }
        }
        if base.is_null() {
            return base;
        }
        let result = unsafe { base.add(prefix) };
        unsafe {
            result
                .sub(std::mem::size_of::<Header>())
                .cast::<Header>()
                .write(Header {
                    base,
                    size,
                    align,
                    uid,
                });
        }
        result
    }

    unsafe fn realloc(&self, address: *mut u8, layout: Layout, new_size: usize) -> *mut u8 {
        let header_ptr = unsafe { address.sub(std::mem::size_of::<Header>()).cast::<Header>() };
        let header = unsafe { &*header_ptr };
        let prefix = address as usize - header.base as usize;
        if header.uid < 0 {
            let Some(size) = prefix.checked_add(new_size) else {
                return ptr::null_mut();
            };
            if Layout::from_size_align(size, header.align).is_err() {
                return ptr::null_mut();
            }
            let storage = unsafe { Layout::from_size_align_unchecked(header.size, header.align) };
            let align = header.align;
            let base = unsafe { System.realloc(header.base, storage, size) };
            if !base.is_null() {
                let result = unsafe { base.add(prefix) };
                unsafe {
                    result
                        .sub(std::mem::size_of::<Header>())
                        .cast::<Header>()
                        .write(Header {
                            base,
                            size,
                            align,
                            uid: -1,
                        });
                }
                return result;
            }
        } else if new_size <= layout.size() {
            return address;
        }
        let Ok(new_layout) = Layout::from_size_align(new_size, layout.align()) else {
            return ptr::null_mut();
        };
        let result = unsafe { self.alloc(new_layout) };
        if !result.is_null() {
            unsafe {
                ptr::copy_nonoverlapping(address, result, layout.size().min(new_size));
                self.dealloc(address, layout);
            }
        }
        result
    }

    unsafe fn dealloc(&self, address: *mut u8, _: Layout) {
        let header = unsafe {
            address
                .sub(std::mem::size_of::<Header>())
                .cast::<Header>()
                .read()
        };
        if header.uid >= 0 {
            let storage = unsafe { Layout::from_size_align_unchecked(header.size, header.align) };
            let mut pools = POOLS.lock();
            unsafe {
                pools[header.uid as usize]
                    .deallocate(std::ptr::NonNull::new_unchecked(header.base), storage);
            }
        } else {
            let layout = unsafe { Layout::from_size_align_unchecked(header.size, header.align) };
            unsafe {
                System.dealloc(header.base, layout);
            }
        }
    }
}

// Grow lazily in 16 MiB blocks, leaving CDRAM available to GXM when the
// primary heap is sufficient. Free holes are coalesced and reused. No pool
// bookkeeping allocates through the global allocator while holding this lock.
struct PoolLock {
    held: std::sync::atomic::AtomicBool,
    heaps: std::cell::UnsafeCell<[linked_list_allocator::Heap; 8]>,
}
// Access to the heaps is serialized by `held`, including deallocation.
unsafe impl Sync for PoolLock {}
struct PoolGuard<'a>(&'a PoolLock);
impl PoolLock {
    fn lock(&self) -> PoolGuard<'_> {
        use std::sync::atomic::Ordering::{Acquire, Relaxed};
        while self
            .held
            .compare_exchange_weak(false, true, Acquire, Relaxed)
            .is_err()
        {
            std::hint::spin_loop();
        }
        PoolGuard(self)
    }
}
impl std::ops::Deref for PoolGuard<'_> {
    type Target = [linked_list_allocator::Heap; 8];
    fn deref(&self) -> &Self::Target {
        unsafe { &*self.0.heaps.get() }
    }
}
impl std::ops::DerefMut for PoolGuard<'_> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        unsafe { &mut *self.0.heaps.get() }
    }
}
impl Drop for PoolGuard<'_> {
    fn drop(&mut self) {
        self.0
            .held
            .store(false, std::sync::atomic::Ordering::Release);
    }
}
// std::sync::Mutex lazily allocates its native handle on some targets, which
// would recursively enter this allocator when the primary heap is exhausted.
static POOLS: PoolLock = PoolLock {
    held: std::sync::atomic::AtomicBool::new(false),
    heaps: std::cell::UnsafeCell::new([const { linked_list_allocator::Heap::empty() }; 8]),
};

fn cdram_alloc(layout: Layout) -> Option<(*mut u8, i32)> {
    const POOL_SIZE: usize = 16 * 1024 * 1024;
    if layout.size().checked_add(layout.align())? > POOL_SIZE {
        return None;
    }
    let mut pools = POOLS.lock();
    for (index, pool) in pools.iter_mut().enumerate() {
        if pool.size() == 0 {
            let mut uid = unsafe {
                sceKernelAllocMemBlock(
                    c"ethornell-heap-pool".as_ptr(),
                    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
                    POOL_SIZE as u32,
                    ptr::null_mut(),
                )
            };
            let mut pool_size = POOL_SIZE;
            if uid < 0 {
                // A remaining 10–15 MiB CDRAM span can still satisfy a
                // large scene buffer even when a full 16 MiB pool cannot.
                pool_size = (layout.size() + layout.align()).max(4 * 1024 * 1024)
                    .checked_add(0x3ffff)? & !0x3ffff;
                uid = unsafe {
                    sceKernelAllocMemBlock(c"ethornell-heap-tail".as_ptr(),
                        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, pool_size as u32, ptr::null_mut())
                };
            }
            if uid < 0 {
                continue;
            }
            let mut address = ptr::null_mut();
            if unsafe { sceKernelGetMemBlockBase(uid, &mut address) } < 0 || address.is_null() {
                unsafe {
                    sceKernelFreeMemBlock(uid);
                }
                continue;
            }
            // Pools live until process exit, when the kernel reclaims them.
            unsafe {
                pool.init(address.cast(), pool_size);
            }
        }
        if let Ok(address) = pool.allocate_first_fit(layout) {
            return Some((address.as_ptr(), index as i32));
        }
    }
    None
}
