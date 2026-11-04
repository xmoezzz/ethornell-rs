//! The MSVC CRT `qsort` linked into BGI 1.622 (0x4AC3F0, `_shortsort`
//! 0x4AC360). It is not stable, so the order of equal keys is part of the
//! observable behaviour; this follows the disassembly step for step.

const CUTOFF: usize = 8;

/// Sorts `items` with a C comparator (`> 0` means `a` sorts after `b`).
pub(crate) fn qsort<T>(items: &mut [T], compare: impl Fn(&T, &T) -> i32) {
    let num = items.len();
    if num < 2 {
        return;
    }
    // Signed slots stand in for the target's element pointers: `higuy` can
    // step one slot below `lo`.
    let cmp = |items: &[T], a: isize, b: isize| compare(&items[a as usize], &items[b as usize]);
    let swap = |items: &mut [T], a: isize, b: isize| items.swap(a as usize, b as usize);
    let mut stack: Vec<(isize, isize)> = Vec::new();
    let (mut lo, mut hi) = (0isize, num as isize - 1);
    loop {
        let size = hi - lo + 1;
        if size <= CUTOFF as isize {
            short_sort(items, lo as usize, hi as usize, &compare);
        } else {
            let mut mid = lo + size / 2;
            if cmp(items, lo, mid) > 0 {
                swap(items, lo, mid);
            }
            if cmp(items, lo, hi) > 0 {
                swap(items, lo, hi);
            }
            if cmp(items, mid, hi) > 0 {
                swap(items, mid, hi);
            }
            let mut loguy = lo;
            let mut higuy = hi;
            loop {
                if mid > loguy {
                    loop {
                        loguy += 1;
                        if !(loguy < mid && cmp(items, loguy, mid) <= 0) {
                            break;
                        }
                    }
                }
                if mid <= loguy {
                    loop {
                        loguy += 1;
                        if !(loguy <= hi && cmp(items, loguy, mid) <= 0) {
                            break;
                        }
                    }
                }
                loop {
                    higuy -= 1;
                    if !(higuy > mid && cmp(items, higuy, mid) > 0) {
                        break;
                    }
                }
                if higuy < loguy {
                    break;
                }
                swap(items, loguy, higuy);
                if mid == higuy {
                    mid = loguy;
                }
            }
            higuy += 1;
            if mid < higuy {
                loop {
                    higuy -= 1;
                    if !(higuy > mid && cmp(items, higuy, mid) == 0) {
                        break;
                    }
                }
            }
            if mid >= higuy {
                loop {
                    higuy -= 1;
                    if !(higuy > lo && cmp(items, higuy, mid) == 0) {
                        break;
                    }
                }
            }
            if higuy - lo >= hi - loguy {
                if lo < higuy {
                    stack.push((lo, higuy));
                }
                if loguy < hi {
                    lo = loguy;
                    continue;
                }
            } else {
                if loguy < hi {
                    stack.push((loguy, hi));
                }
                if lo < higuy {
                    hi = higuy;
                    continue;
                }
            }
        }
        match stack.pop() {
            Some((l, h)) => {
                lo = l;
                hi = h;
            }
            None => return,
        }
    }
}

/// `_shortsort`: repeatedly move the largest of [lo, hi] to hi; the first
/// maximum wins ties.
fn short_sort<T>(items: &mut [T], lo: usize, mut hi: usize, compare: &impl Fn(&T, &T) -> i32) {
    while hi > lo {
        let mut max = lo;
        for p in lo + 1..=hi {
            if compare(&items[p], &items[max]) > 0 {
                max = p;
            }
        }
        items.swap(max, hi);
        hi -= 1;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sorts_and_keeps_the_crt_order_of_equal_keys() {
        let mut values: Vec<(i32, usize)> = [5, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3]
            .iter()
            .copied()
            .zip(0..)
            .collect();
        qsort(&mut values, |a, b| a.0.wrapping_sub(b.0));
        let keys: Vec<i32> = values.iter().map(|v| v.0).collect();
        assert_eq!(keys, [1, 1, 2, 3, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9, 9, 9]);
        // Small runs: shortsort moves the first maximum to the end, so equal
        // keys come out reversed.
        let mut small = vec![(1, 0), (1, 1), (1, 2)];
        qsort(&mut small, |a, b| a.0 - b.0);
        assert_eq!(small, [(1, 1), (1, 2), (1, 0)]);
    }

    #[test]
    fn large_random_input_is_sorted() {
        let mut seed = 12345u32;
        let mut values: Vec<i32> = (0..500)
            .map(|_| {
                seed = seed.wrapping_mul(1_103_515_245).wrapping_add(12345);
                (seed >> 16) as i32 % 50
            })
            .collect();
        qsort(&mut values, |a, b| a - b);
        assert!(values.windows(2).all(|w| w[0] <= w[1]));
    }
}
