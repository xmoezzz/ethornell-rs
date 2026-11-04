"""Fail closed on ABI symbol/activity glue/16KB LOAD and RELRO regressions."""
import re, subprocess, sys
library, readelf = sys.argv[1:]
headers = subprocess.check_output([readelf, '-lW', library], text=True)
elf = subprocess.check_output([readelf, '-h', library], text=True)
assert 'AArch64' in elf and 'ELF64' in elf and 'DYN' in elf
loads = 0
relro = 0
for line in headers.splitlines():
    columns = line.split()
    if not columns:
        continue
    if columns[0] == 'LOAD':
        loads += 1
        assert int(columns[-1], 16) >= 16384, line
        assert int(columns[-1], 16) & (int(columns[-1], 16) - 1) == 0, line
        assert (int(columns[1], 16) - int(columns[2], 16)) % 16384 == 0, line
    if columns[0] == 'GNU_RELRO':
        relro += 1
        assert (int(columns[2], 16) + int(columns[5], 16)) % 16384 == 0, line
assert loads and relro
symbols = subprocess.check_output([readelf, '--dyn-syms', '-W', library], text=True)
assert 'android_main' not in symbols
assert 'ANativeActivity_onCreate' not in symbols
expected = {'ethornell_android_' + name for name in ['abi_version','build_info','last_error','init_context','create','set_surface','detach_surface','step','input','set_paused','request_close','poll_event','complete_request','request_payload','destroy']}
defined = set()
for line in symbols.splitlines():
    columns = line.split()
    if len(columns) >= 8 and columns[0].endswith(':') and columns[6] != 'UND':
        defined.add(columns[7])
assert defined & expected == expected, expected - defined
assert {name for name in defined if name.startswith('ethornell_android_')} == expected
dynamic = subprocess.check_output([readelf, '-dW', library], text=True)
needed = set(re.findall(r'Shared library: \[([^\]]+)\]', dynamic))
allowed = {'libandroid.so','libdl.so','liblog.so','libc.so','libm.so','libvulkan.so','libEGL.so','libGLESv2.so','libGLESv3.so','libOpenSLES.so','libaaudio.so'}
assert needed and needed <= allowed, needed - allowed
assert 'BIND_NOW' in dynamic or re.search(r'FLAGS_1.*NOW', dynamic)
assert 'TEXTREL' not in dynamic
print('ELF AArch64, 15 defined host API symbols, no activity glue, NEEDED allowlist, NOW/RELRO and 16KB LOAD/RELRO: OK')
