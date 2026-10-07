#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4D9F80 @ 0x004D9F80..0x004D9F91 =====
int sub_4D9F80()
{
  sub_450310();
  return atexit(sub_4DA030);
}

// ===== sub_4D9FA0 @ 0x004D9FA0..0x004D9FB6 =====
int sub_4D9FA0()
{
  sub_4A4A40(byte_5089E8);
  return atexit(sub_4DA040);
}

// ===== sub_4D9FC0 @ 0x004D9FC0..0x004D9FE4 =====
int sub_4D9FC0()
{
  `eh vector constructor iterator'(
    (char *)dword_5085A8,
    0x44u,
    16,
    (void (__thiscall *)(void *))sub_4A25D0,
    (void (__thiscall *)(void *))sub_4A1E30);
  return atexit(sub_4DA050);
}

// ===== sub_4DA000 @ 0x004DA000..0x004DA024 =====
int sub_4DA000()
{
  `eh vector constructor iterator'(
    (char *)dword_508A08,
    0x44u,
    64,
    (void (__thiscall *)(void *))sub_4A25D0,
    (void (__thiscall *)(void *))sub_4A1E30);
  return atexit(sub_4DA070);
}

// ===== sub_4DA030 @ 0x004DA030..0x004DA03A =====
void __cdecl sub_4DA030()
{
  sub_450370(&dword_566AA4);
}

// ===== sub_4DA040 @ 0x004DA040..0x004DA04A =====
void __cdecl sub_4DA040()
{
  sub_4A4A60(byte_5089E8);
}

// ===== sub_4DA050 @ 0x004DA050..0x004DA064 =====
void __cdecl sub_4DA050()
{
  `eh vector destructor iterator'((char *)dword_5085A8, 0x44u, 16, (void (__thiscall *)(void *))sub_4A1E30);
}

// ===== sub_4DA070 @ 0x004DA070..0x004DA084 =====
void __cdecl sub_4DA070()
{
  `eh vector destructor iterator'((char *)dword_508A08, 0x44u, 64, (void (__thiscall *)(void *))sub_4A1E30);
}

// ===== sub_4DA084 @ 0x004DA084..0x004DA098 =====
void __cdecl sub_4DA084()
{
  dword_509B50[0] = &std::bad_alloc::`vftable';
  sub_4AC3AD((void **)dword_509B50);
}
