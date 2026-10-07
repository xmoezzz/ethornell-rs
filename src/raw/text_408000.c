#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_408020 @ 0x00408020..0x0040803D =====
int __fastcall sub_408020(int a1, int a2)
{
  int result; // eax
  int v3; // edx
  int v4; // ecx
  bool v5; // zf
  int v6; // ecx

  result = 0;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
  {
    v3 = *(_DWORD *)(a2 + 8);
    v4 = 9 * a1;
    v5 = *(_DWORD *)(v3 + 8 * v4) == 0;
    v6 = v3 + 8 * v4;
    if ( !v5 )
      return v6 + 4;
  }
  return result;
}

// ===== sub_408040 @ 0x00408040..0x004080A1 =====
int __thiscall sub_408040(void *this, int Val)
{
  _DWORD v3[6]; // [esp+10h] [ebp-18h] BYREF

  if ( !sub_407F20((int)dword_566750, (int)this, v3) )
    return 0;
  if ( Val )
    sub_40A710(Val);
  else
    sub_40A620(0);
  return 1;
}

// ===== sub_4080B0 @ 0x004080B0..0x004081AD =====
int __userpurge sub_4080B0@<eax>(int a1@<eax>, int a2, int a3, unsigned int a4, unsigned __int8 *a5, _DWORD *a6)
{
  int *v7; // edx
  int i; // ebx
  int j; // ecx
  int v11; // eax
  void *v13; // [esp+10h] [ebp-38h]
  _DWORD v14[5]; // [esp+30h] [ebp-18h] BYREF

  v13 = dword_566750;
  if ( !sub_407DA0(a2, dword_566750, a3, a1, a4) )
    return 0;
  sub_407F20((int)v13, a2, v14);
  if ( a4 == 1 )
  {
    v7 = (int *)v14[0];
    for ( i = a1; i; --i )
    {
      for ( j = a3; j; --j )
      {
        *v7++ = *a5 | (*(unsigned __int16 *)(a5 + 1) << 8);
        a5 += 3;
      }
    }
  }
  else
  {
    sub_40ADF0();
  }
  sub_408E90(v14);
  if ( a6 )
  {
    v11 = sub_407F00(a2, (int)v13);
    *(_DWORD *)(v11 + 40) = *a6;
    *(_DWORD *)(v11 + 44) = a6[1];
  }
  return 1;
}

// ===== sub_4081B0 @ 0x004081B0..0x004081F9 =====
int __usercall sub_4081B0@<eax>(int a1@<ecx>, int a2@<edi>)
{
  int v2; // eax
  int v3; // ecx
  int v4; // eax
  int v5; // eax
  int v6; // ecx

  v2 = sub_408020(a1, (int)dword_566750);
  v3 = v2;
  if ( !v2 )
    return 11;
  v4 = *(_DWORD *)(v2 + 16);
  if ( a2 == v4 )
    return 0;
  if ( v4 == 2 && a2 == 1 )
  {
    *(_DWORD *)(v3 + 16) = 1;
    v5 = sub_407B30(1);
    *(_DWORD *)(v6 + 20) = v5;
    return 0;
  }
  return 21;
}

// ===== sub_408200 @ 0x00408200..0x004082FF =====
int __fastcall sub_408200(int a1, void *a2, unsigned int *a3, unsigned int a4)
{
  int v5; // esi
  int v6; // eax
  size_t v7; // ebx
  unsigned int v8; // ecx
  char *v9; // edi
  void *Src[2]; // [esp+10h] [ebp-40h] BYREF
  int v12; // [esp+18h] [ebp-38h]
  int v13; // [esp+1Ch] [ebp-34h]
  int v14; // [esp+20h] [ebp-30h]
  int v15; // [esp+24h] [ebp-2Ch]
  unsigned int *v16; // [esp+28h] [ebp-28h]
  int v17; // [esp+2Ch] [ebp-24h]
  unsigned int v18; // [esp+30h] [ebp-20h]
  char *v19; // [esp+34h] [ebp-1Ch]
  void *v20; // [esp+38h] [ebp-18h]
  _DWORD v21[4]; // [esp+3Ch] [ebp-14h]

  v16 = a3;
  if ( !sub_407F20((int)dword_566750, a1, Src) )
    return 9;
  v5 = v12;
  v6 = v13;
  v21[0] = 16;
  v21[1] = 24;
  v21[2] = 32;
  v21[3] = 8;
  v7 = v21[v14] >> 3;
  v8 = v7 * v13 * v12;
  v18 = v8;
  if ( a4 < v8 )
    return 10;
  v20 = a2;
  v9 = (char *)Src[0];
  v19 = (char *)Src[0];
  if ( v13 )
  {
    do
    {
      v17 = --v6;
      if ( v5 )
      {
        do
        {
          memcpy_0(v20, v9, v7);
          v20 = (char *)v20 + v7;
          v9 += v15;
          --v5;
        }
        while ( v5 );
        v5 = v12;
        v9 = v19;
        v6 = v17;
        v8 = v18;
      }
      v9 += (unsigned int)Src[1];
      v19 = v9;
    }
    while ( v6 );
  }
  *v16 = v8;
  return 0;
}

// ===== sub_408300 @ 0x00408300..0x0040831F =====
int __fastcall sub_408300(int a1, int a2)
{
  int result; // eax
  int v3; // edx
  int v4; // ecx
  bool v5; // zf
  int v6; // ecx

  result = -1;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
  {
    v3 = *(_DWORD *)(a2 + 8);
    v4 = 9 * a1;
    v5 = *(_DWORD *)(v3 + 8 * v4) == 0;
    v6 = v3 + 8 * v4;
    if ( !v5 )
      return *(_DWORD *)(v6 + 28);
  }
  return result;
}

// ===== sub_408320 @ 0x00408320..0x0040839C =====
int __thiscall sub_408320(void *this, int a2)
{
  _DWORD *v2; // edi
  _DWORD v4[6]; // [esp+10h] [ebp-30h] BYREF
  _DWORD v5[6]; // [esp+28h] [ebp-18h] BYREF

  v2 = dword_566750;
  if ( !sub_407F20((int)dword_566750, (int)this, v4) )
    return 9;
  if ( !sub_407DA0(a2, v2, v4[2], v4[3], 3u) )
    return 10;
  sub_407F20((int)v2, a2, v5);
  sub_40E680(v4, v5);
  return 0;
}

// ===== sub_4083A0 @ 0x004083A0..0x004083E1 =====
int __thiscall sub_4083A0(void *this)
{
  _DWORD v2[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20((int)dword_566750, (int)this, v2) )
    return sub_40E800(v2) != 0 ? 0 : 7;
  else
    return 11;
}

// ===== sub_4083F0 @ 0x004083F0..0x0040842C =====
int __userpurge sub_4083F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<edi>, int a4, LONG a5, int a6)
{
  _DWORD *v6; // esi
  int result; // eax

  v6 = dword_566750;
  result = sub_408890(&a6, dword_566750, a3, a4, a5, a6, a2, a1);
  if ( !result )
    *(_DWORD *)(v6[2] + 72 * a3 + 32) = a6;
  return result;
}

// ===== sub_408430 @ 0x00408430..0x004084A1 =====
int __stdcall sub_408430(int a1, int a2)
{
  void *v3; // edi
  int result; // eax
  int v5; // edx
  int v6; // ecx
  void *v7; // [esp+14h] [ebp+Ch]

  v3 = dword_566750;
  v7 = dword_566750;
  result = -2147483644;
  if ( a2 >= 0 && a2 < *((_DWORD *)dword_566750 + 3) )
  {
    v5 = *((_DWORD *)dword_566750 + 2);
    if ( *(_DWORD *)(v5 + 72 * a2) )
    {
      v6 = *(_DWORD *)(v5 + 72 * a2 + 32);
      result = -2147483647;
      if ( v6 != -1 )
      {
        if ( sub_408A90(v6, *(_DWORD *)(v5 + 72 * a2 + 32)) )
        {
          result = sub_44D110();
          if ( !result )
            return result;
          v3 = v7;
        }
        sub_4084B0(v3, a2);
        return -2147483647;
      }
    }
  }
  return result;
}

// ===== sub_4084B0 @ 0x004084B0..0x00408548 =====
int __stdcall sub_4084B0(int a1, int a2)
{
  int result; // eax
  _DWORD *v3; // edi
  int v4; // edx
  int v5; // esi

  result = -2147483644;
  if ( a2 >= 0 && a2 < *(_DWORD *)(a1 + 12) )
  {
    v3 = (_DWORD *)(72 * a2 + *(_DWORD *)(a1 + 8));
    if ( *v3 )
    {
      v4 = v3[8];
      result = -2147483647;
      if ( v4 != -1 )
      {
        if ( sub_408A90(a1, v4) )
        {
          sub_44D560(v3 + 9);
          if ( sub_44D4D0() )
            sub_44D210();
          v5 = 0;
        }
        else
        {
          v5 = -2147483647;
        }
        sub_408A10();
        *(_DWORD *)(*(_DWORD *)(a1 + 8) + 72 * a2 + 32) = -1;
        return v5;
      }
    }
  }
  return result;
}

// ===== sub_408550 @ 0x00408550..0x0040859D =====
int __fastcall sub_408550(int a1)
{
  int result; // eax
  int v2; // edx
  int v3; // ecx
  bool v4; // zf
  int v5; // ecx
  int v6; // ecx

  result = -2147483644;
  if ( a1 >= 0 && a1 < *((_DWORD *)dword_566750 + 3) )
  {
    v2 = *((_DWORD *)dword_566750 + 2);
    v3 = 9 * a1;
    v4 = *(_DWORD *)(v2 + 8 * v3) == 0;
    v5 = v2 + 8 * v3;
    if ( !v4 )
    {
      v6 = *(_DWORD *)(v5 + 32);
      if ( v6 == -1 )
        return -2147483647;
      if ( !sub_408A90(v6, v6) )
        return -2147483647;
      result = sub_44D240();
      if ( result )
        return -2147483647;
    }
  }
  return result;
}

// ===== sub_4085A0 @ 0x004085A0..0x004085FD =====
unsigned int __fastcall sub_4085A0(int a1, int a2, int a3)
{
  unsigned int result; // eax
  int v4; // edx
  int v5; // ecx
  bool v6; // zf
  int v7; // ecx
  int v8; // ecx

  result = -2147483644;
  if ( a1 >= 0 && a1 < *((_DWORD *)dword_566750 + 3) )
  {
    v4 = *((_DWORD *)dword_566750 + 2);
    v5 = 9 * a1;
    v6 = *(_DWORD *)(v4 + 8 * v5) == 0;
    v7 = v4 + 8 * v5;
    if ( !v6 )
    {
      v8 = *(_DWORD *)(v7 + 32);
      if ( v8 == -1 || !sub_408A90(v8, v8) )
        return -2147483647;
      else
        return sub_44D2C0(a3) != 0 ? 0x80000005 : 0;
    }
  }
  return result;
}

// ===== sub_408600 @ 0x00408600..0x00408650 =====
unsigned int __fastcall sub_408600(int a1)
{
  unsigned int result; // eax
  int v2; // edx
  int v3; // ecx
  bool v4; // zf
  int v5; // ecx
  int v6; // ecx
  int v7; // eax

  result = -2147483644;
  if ( a1 >= 0 && a1 < *((_DWORD *)dword_566750 + 3) )
  {
    v2 = *((_DWORD *)dword_566750 + 2);
    v3 = 9 * a1;
    v4 = *(_DWORD *)(v2 + 8 * v3) == 0;
    v5 = v2 + 8 * v3;
    if ( !v4 )
    {
      v6 = *(_DWORD *)(v5 + 32);
      if ( v6 == -1 )
        return -2147483647;
      v7 = sub_408A90(v6, v6);
      if ( !v7 )
        return -2147483647;
      else
        return sub_44D390(v7) != 0 ? 0x80000004 : 0;
    }
  }
  return result;
}

// ===== sub_408650 @ 0x00408650..0x004086AC =====
unsigned int __fastcall sub_408650(int a1, int a2, int a3)
{
  unsigned int result; // eax
  int v4; // edx
  int v5; // ecx
  bool v6; // zf
  int v7; // ecx
  int v8; // ecx

  result = -2147483644;
  if ( a1 >= 0 && a1 < *((_DWORD *)dword_566750 + 3) )
  {
    v4 = *((_DWORD *)dword_566750 + 2);
    v5 = 9 * a1;
    v6 = *(_DWORD *)(v4 + 8 * v5) == 0;
    v7 = v4 + 8 * v5;
    if ( !v6 )
    {
      v8 = *(_DWORD *)(v7 + 32);
      if ( v8 == -1 || !sub_408A90(v8, v8) )
        return -2147483647;
      else
        return sub_44D450(a3) != 0 ? 0x80000003 : 0;
    }
  }
  return result;
}

// ===== sub_4086B0 @ 0x004086B0..0x004086D6 =====
int sub_4086B0()
{
  _DWORD *v0; // esi
  int result; // eax

  v0 = dword_5076C8;
  if ( dword_5076C8 )
  {
    do
    {
      result = sub_44D690();
      v0 = (_DWORD *)v0[5];
    }
    while ( v0 );
  }
  return result;
}

// ===== sub_4086E0 @ 0x004086E0..0x00408734 =====
void sub_4086E0()
{
  void *v0; // ebx
  int *v1; // edi

  v0 = dword_566750;
  v1 = (int *)dword_5076C8;
  if ( dword_5076C8 )
  {
    do
    {
      if ( sub_42C570() )
      {
        if ( !sub_44D4D0() )
        {
          sub_4084B0((int)v0, v1[4]);
          v1 = &dword_5076B4;
        }
      }
      v1 = (int *)v1[5];
    }
    while ( v1 );
  }
}

// ===== sub_408740 @ 0x00408740..0x00408781 =====
void __usercall sub_408740(int a1@<edi>, int a2@<esi>)
{
  if ( dword_565B20 )
  {
    if ( !a2 )
      DeleteCriticalSection(&stru_50A8B8);
  }
  else if ( a2 )
  {
    InitializeCriticalSection(&stru_50A8B8);
    dword_565B24 = a1;
    dword_565B20 = a2;
    return;
  }
  dword_565B24 = a1;
  dword_565B20 = a2;
}

// ===== sub_408790 @ 0x00408790..0x004087B2 =====
void *sub_408790()
{
  void *result; // eax

  for ( result = dword_5076C8; dword_5076C8; result = dword_5076C8 )
    sub_408A10();
  return result;
}

// ===== sub_4087F0 @ 0x004087F0..0x00408817 =====
int sub_4087F0()
{
  _DWORD *v0; // edi

  sub_408870();
  v0 = dword_5076C8;
  if ( dword_5076C8 )
  {
    do
    {
      sub_44D040();
      v0 = (_DWORD *)v0[5];
    }
    while ( v0 );
  }
  return sub_408880();
}

// ===== sub_408820 @ 0x00408820..0x00408861 =====
int __usercall sub_408820@<eax>(int a1@<edi>)
{
  int v1; // esi
  int v2; // ecx

  v1 = 0;
  sub_408870();
  if ( dword_5076C8 )
  {
    while ( a1 != sub_44D680() )
    {
      if ( !*(_DWORD *)(v2 + 20) )
      {
        sub_408880();
        return 0;
      }
    }
    v1 = sub_44D710();
  }
  sub_408880();
  return v1;
}

// ===== sub_408870 @ 0x00408870..0x0040887C =====
void sub_408870()
{
  EnterCriticalSection(&stru_50A8B8);
}

// ===== sub_408880 @ 0x00408880..0x0040888C =====
void sub_408880()
{
  LeaveCriticalSection(&stru_50A8B8);
}

// ===== sub_408890 @ 0x00408890..0x00408A0D =====
int __cdecl sub_408890(_DWORD *a1, int a2, int a3, int a4, LONG lDistanceToMove, int a6, int a7, int a8)
{
  int v8; // ebx
  _DWORD *v9; // esi
  int v10; // eax
  int v11; // ebx
  int v13; // [esp+0h] [ebp-28h] BYREF
  void *v14; // [esp+10h] [ebp-18h]
  int v15; // [esp+14h] [ebp-14h]
  int *v16; // [esp+18h] [ebp-10h]
  int v17; // [esp+24h] [ebp-4h]

  v16 = &v13;
  v14 = operator new(0x388u);
  v8 = 0;
  v17 = 0;
  if ( v14 )
    v8 = sub_44C940();
  v17 = -1;
  v9 = operator new(0x18u);
  v14 = v9;
  *v9 = dword_5076B4;
  v9[1] = v8;
  v9[2] = sub_408AB0;
  v9[3] = v9;
  v9[4] = a3;
  sub_408870();
  v9[5] = dword_5076C8;
  ++dword_5076B4;
  dword_5076C8 = v9;
  sub_408880();
  v17 = 1;
  v10 = sub_44C9D0(a2, a3, (int)(v9 + 2), a4, lDistanceToMove, a6, a7, a8);
  v15 = v10;
  v17 = -1;
  if ( v10 )
  {
    if ( v10 == -2147483646 )
      v11 = -2147483646;
    else
      v11 = 2 * (v10 == -2147483645) - 0x7FFFFFFF;
    v17 = 3;
    sub_408A10();
    v17 = -1;
    return v11;
  }
  else
  {
    *a1 = *v9;
    return 0;
  }
}

// ===== sub_408A10 @ 0x00408A10..0x00408A8C =====
int __usercall sub_408A10@<eax>(int a1@<edi>)
{
  int *v1; // esi
  int *v2; // eax
  void (__thiscall ***v4)(_DWORD, int); // ecx

  sub_408870();
  v1 = (int *)dword_5076C8;
  v2 = &dword_5076B4;
  if ( !dword_5076C8 )
  {
LABEL_4:
    sub_408880();
    return 0;
  }
  while ( a1 != *v1 )
  {
    v2 = v1;
    v1 = (int *)v1[5];
    if ( !v1 )
      goto LABEL_4;
  }
  v2[5] = v1[5];
  sub_408880();
  v4 = (void (__thiscall ***)(_DWORD, int))v1[1];
  if ( v4 )
  {
    if ( *(v4 - 1) )
    {
      (**v4)(v4, 3);
      operator delete(v1);
      return 1;
    }
    operator delete[](v4 - 1);
  }
  operator delete(v1);
  return 1;
}

// ===== sub_408A90 @ 0x00408A90..0x00408AB0 =====
int __fastcall sub_408A90(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_5076C8;
  result = 0;
  if ( dword_5076C8 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[5];
      if ( !v2 )
        return result;
    }
    return v2[1];
  }
  return result;
}

// ===== sub_408AB0 @ 0x00408AB0..0x00408AD0 =====
int __cdecl sub_408AB0(int a1)
{
  sub_496540(0x10000, *(_DWORD *)(a1 + 16), 0);
  return 1;
}

// ===== sub_408AD0 @ 0x00408AD0..0x00408B7F =====
int __userpurge sub_408AD0@<eax>(int a1@<ecx>, int a2@<eax>, int xRight, int yBottom, int a5)
{
  int result; // eax
  _DWORD *v7; // edi
  _BYTE v8[4]; // [esp+Ch] [ebp-Ch] BYREF
  _BYTE v9[4]; // [esp+10h] [ebp-8h] BYREF
  _DWORD *v10; // [esp+14h] [ebp-4h]

  v10 = dword_566750;
  result = sub_408D00((int)&a5, a2, xRight, yBottom, a1);
  if ( !result )
  {
    sub_408E50(a5);
    if ( sub_493350(v8, v9) )
    {
      v7 = v10;
      if ( sub_407DA0(a2, v10, xRight, yBottom, 1u) )
      {
        *(_DWORD *)(v7[2] + 72 * a2 + 32) = a5;
        return 0;
      }
      else
      {
        sub_408DC0();
        return -2147483644;
      }
    }
    else
    {
      sub_408DC0();
      return -2147483647;
    }
  }
  return result;
}

// ===== sub_408B80 @ 0x00408B80..0x00408BDE =====
int __usercall sub_408B80@<eax>(int a1@<esi>)
{
  void *v1; // edi
  int result; // eax
  int v3; // edx
  int v4; // ecx
  int v5; // ebx
  int v6; // eax

  v1 = dword_566750;
  result = -2147483644;
  if ( a1 >= 0 && a1 < *((_DWORD *)dword_566750 + 3) )
  {
    v3 = *((_DWORD *)dword_566750 + 2);
    if ( *(_DWORD *)(v3 + 72 * a1) )
    {
      v4 = *(_DWORD *)(v3 + 72 * a1 + 32);
      v5 = -2147483647;
      if ( v4 != -1 )
      {
        if ( sub_408E50(v4) )
        {
          v6 = sub_4932B0();
          if ( !v6 )
            return 0;
          if ( v6 == -2147483640 )
            v5 = -2147483646;
        }
        sub_408BE0(v1);
      }
      return v5;
    }
  }
  return result;
}

// ===== sub_408BE0 @ 0x00408BE0..0x00408CFE =====
int __userpurge sub_408BE0@<eax>(int a1@<eax>, int a2)
{
  int result; // eax
  int v4; // ecx
  int v5; // esi
  int v6; // ecx
  int v7; // edx
  int v8; // ecx
  int *v9; // eax
  int v10; // edx
  int v11; // ecx
  int v12; // edx
  int v13; // ecx
  int v14; // edx
  int v15; // ecx
  int v16; // edx
  int v17; // ecx
  int v18; // edx
  int v19; // ecx
  int v20; // edx
  int v21; // [esp+Ch] [ebp-34h]
  int v22; // [esp+10h] [ebp-30h] BYREF
  _DWORD v23[11]; // [esp+14h] [ebp-2Ch] BYREF

  result = -2147483644;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
  {
    v4 = *(_DWORD *)(a2 + 8);
    v5 = 72 * a1;
    if ( *(_DWORD *)(v4 + v5) )
    {
      v6 = *(_DWORD *)(v4 + v5 + 32);
      result = -2147483647;
      v21 = -2147483647;
      if ( v6 != -1 )
      {
        if ( sub_408E50(v6) )
        {
          sub_493300();
          v7 = *(_DWORD *)(a2 + 8);
          v8 = *(_DWORD *)(v7 + v5 + 4);
          v9 = (int *)(v7 + v5 + 4);
          v10 = *(_DWORD *)(v7 + v5 + 8);
          v23[5] = v8;
          v11 = v9[2];
          v23[6] = v10;
          v12 = v9[3];
          v23[7] = v11;
          v13 = v9[4];
          v23[8] = v12;
          v14 = v9[5];
          v23[9] = v13;
          v15 = *v9;
          v23[10] = v14;
          v16 = v9[1];
          v22 = v15;
          v17 = v9[2];
          v23[0] = v16;
          v18 = v9[3];
          v23[1] = v17;
          v19 = v9[4];
          v23[2] = v18;
          v20 = v9[5];
          v23[3] = v19;
          v23[4] = v20;
          if ( sub_493350(&v22, v23) )
          {
            sub_40ADF0();
            v21 = 0;
          }
          else
          {
            v21 = -2147483647;
          }
        }
        sub_408DC0();
        *(_DWORD *)(*(_DWORD *)(a2 + 8) + v5 + 32) = -1;
        *(_DWORD *)(*(_DWORD *)(a2 + 8) + v5 + 36) = -1;
        return v21;
      }
    }
  }
  return result;
}

// ===== sub_408D00 @ 0x00408D00..0x00408DB9 =====
int __cdecl sub_408D00(_DWORD *a1, int a2, int xRight, int yBottom, int a5)
{
  void *v5; // eax
  int v6; // esi
  _DWORD *v7; // eax

  v5 = operator new(0x178u);
  v6 = 0;
  if ( v5 )
    v6 = sub_492560((int)v5, xRight, yBottom);
  v7 = operator new(0x10u);
  *v7 = dword_5076CC;
  v7[2] = a2;
  v7[1] = v6;
  v7[3] = dword_5076D8;
  ++dword_5076CC;
  dword_5076D8 = v7;
  *a1 = *v7;
  sub_493280(v7);
  sub_493220();
  sub_49EEE0(a5);
  return 0;
}

// ===== sub_408DC0 @ 0x00408DC0..0x00408E28 =====
int __fastcall sub_408DC0(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = (int *)dword_5076D8;
  result = 0;
  v4 = &dword_5076CC;
  if ( dword_5076D8 )
  {
    while ( a2 != *v2 )
    {
      v4 = v2;
      v2 = (int *)v2[3];
      if ( !v2 )
        return result;
    }
    v4[3] = v2[3];
    v5 = (void (__thiscall ***)(_DWORD, int))v2[1];
    if ( v5 )
    {
      if ( *(v5 - 1) )
      {
        (**v5)(v5, 3);
        operator delete(v2);
        return 1;
      }
      operator delete[](v5 - 1);
    }
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_408E30 @ 0x00408E30..0x00408E4C =====
void **__thiscall sub_408E30(void *this)
{
  void **result; // eax

  result = (void **)dword_5076D8;
  if ( dword_5076D8 )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[3];
    }
    while ( result );
  }
  return result;
}

// ===== sub_408E50 @ 0x00408E50..0x00408E60 =====
void *__thiscall sub_408E50(void *this)
{
  void **v1; // eax

  v1 = sub_408E30(this);
  if ( v1 )
    return v1[1];
  else
    return 0;
}

// ===== sub_408E60 @ 0x00408E60..0x00408E81 =====
int __cdecl sub_408E60(_DWORD *a1)
{
  sub_496540(65792, a1[2], *a1);
  return 1;
}

// ===== sub_408E90 @ 0x00408E90..0x00409029 =====
int __stdcall sub_408E90(unsigned __int8 **a1)
{
  int v1; // eax
  int v2; // ecx
  unsigned __int8 **v3; // edi
  unsigned __int8 *v4; // edx
  unsigned __int8 *v5; // ecx
  unsigned __int8 *v6; // esi
  unsigned __int8 v7; // dl
  int v8; // esi
  int v9; // eax
  int v10; // eax
  unsigned __int8 v11; // dl
  unsigned __int8 *v13; // [esp+0h] [ebp-20h]
  unsigned __int8 *v14; // [esp+4h] [ebp-1Ch]
  unsigned __int8 *v15; // [esp+8h] [ebp-18h]
  int v16; // [esp+Ch] [ebp-14h]
  char v17; // [esp+10h] [ebp-10h]
  unsigned __int8 *v18; // [esp+14h] [ebp-Ch]
  unsigned __int8 *v19; // [esp+18h] [ebp-8h]
  unsigned __int8 v20; // [esp+1Eh] [ebp-2h]
  unsigned __int8 v21; // [esp+1Fh] [ebp-1h]

  v1 = sub_407B60();
  v17 = v1;
  if ( !v1 )
    return v2;
  v3 = a1;
  if ( a1[4] != (unsigned __int8 *)2 )
    return v2;
  v20 = BYTE2(v1);
  v4 = a1[3];
  v21 = BYTE1(v1);
  v5 = *a1;
  v19 = *a1;
  if ( v4 )
  {
    v18 = a1[2];
    v13 = a1[1];
    do
    {
      v6 = v18;
      v14 = --v4;
      if ( v18 )
      {
        v15 = v3[5];
        do
        {
          v7 = v5[3];
          v16 = (int)--v6;
          if ( v7 && v7 != 0xFF )
          {
            v8 = 255 - v7;
            v9 = 255 * (*v5 - v8 * (unsigned __int8)v1 / 255) / v7;
            if ( v9 > 255 )
              v9 = 255;
            *v5 = v9 < 0 ? 0 : v9;
            v10 = 255 * (v5[1] - v8 * v21 / 255) / v7;
            if ( v10 > 255 )
              v10 = 255;
            v5[1] = v10 < 0 ? 0 : v10;
            v1 = 255 * (v5[2] - v8 * v20 / 255) / v7;
            if ( v1 > 255 )
              v1 = 255;
            v3 = a1;
            v6 = (unsigned __int8 *)v16;
            v11 = v1 < 0 ? 0 : v1;
            LOBYTE(v1) = v17;
            v5[2] = v11;
          }
          v5 = &v5[(_DWORD)v15];
        }
        while ( v6 );
        v4 = v14;
      }
      v5 = &v19[(_DWORD)v13];
      v19 = &v19[(_DWORD)v13];
    }
    while ( v4 );
  }
  return 1;
}

// ===== sub_409030 @ 0x00409030..0x0040907B =====
BOOL __usercall sub_409030@<eax>(int a1@<eax>, int a2@<edx>, _DWORD *a3@<esi>, int a4)
{
  int v4; // eax
  int v5; // edx
  int v6; // ecx
  int v7; // eax

  a3[2] = a4;
  a3[3] = a2;
  a3[4] = a1;
  v4 = sub_407B30(a1);
  a3[5] = v4;
  v7 = v6 * v4;
  a3[1] = v7;
  if ( v6 && v5 )
  {
    *a3 = operator new[](v5 * v7);
    return *a3 != 0;
  }
  else
  {
    *a3 = 0;
    return *a3 != 0;
  }
}

// ===== sub_409080 @ 0x00409080..0x004090A3 =====
BOOL __usercall sub_409080@<eax>(_DWORD *a1@<esi>, int a2)
{
  int v2; // eax
  int v3; // edx
  int v4; // ecx

  v2 = sub_407B10();
  if ( a2 && v2 == 1 )
    v2 = 2;
  return sub_409030(v2, v3, a1, v4);
}

// ===== sub_4090B0 @ 0x004090B0..0x004090D7 =====
BOOL __usercall sub_4090B0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>)
{
  return *a1 <= *a2 && a2[2] <= a1[2] && a1[1] <= a2[1] && a2[3] <= a1[3];
}

// ===== sub_4090E0 @ 0x004090E0..0x00409107 =====
BOOL __usercall sub_4090E0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>)
{
  return *a2 <= a1[2] && *a1 <= a2[2] && a2[1] <= a1[3] && a1[1] <= a2[3];
}

// ===== sub_409110 @ 0x00409110..0x00409167 =====
BOOL __usercall sub_409110@<eax>(int *a1@<eax>, int *a2@<ecx>)
{
  int v2; // ebx
  int v3; // edx
  int v4; // edi
  int v5; // edx
  int v6; // esi
  int v7; // edx
  int v8; // ecx

  v2 = *a2;
  if ( *a1 >= *a2 )
    v2 = *a1;
  v3 = a1[1];
  *a1 = v2;
  v4 = a2[1];
  if ( v3 >= v4 )
    v4 = v3;
  v5 = a1[2];
  a1[1] = v4;
  v6 = a2[2];
  if ( v5 < v6 )
    v6 = v5;
  v7 = a1[3];
  a1[2] = v6;
  v8 = a2[3];
  if ( v7 < v8 )
    v8 = v7;
  a1[3] = v8;
  return v2 <= v6 && v4 <= v8;
}

// ===== sub_409170 @ 0x00409170..0x00409183 =====
_DWORD *__fastcall sub_409170(int a1, int a2, _DWORD *a3)
{
  _DWORD *result; // eax

  result = a3;
  *a3 += a2;
  a3[2] += a2;
  a3[1] += a1;
  a3[3] += a1;
  return result;
}

// ===== sub_409190 @ 0x00409190..0x004091AC =====
_DWORD *__usercall sub_409190@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  *result = 0;
  result[1] = 0;
  result[2] = *(_DWORD *)(a2 + 8) - 1;
  result[3] = *(_DWORD *)(a2 + 12) - 1;
  return result;
}

// ===== sub_4091B0 @ 0x004091B0..0x00409206 =====
int __usercall sub_4091B0@<eax>(_DWORD *a1@<esi>, int *a2)
{
  int *v2; // eax
  int v3; // eax
  int v4; // edx
  int v5; // ecx
  int v7; // [esp+8h] [ebp-14h] BYREF
  int v8; // [esp+Ch] [ebp-10h]
  int v9; // [esp+10h] [ebp-Ch]
  int v10; // [esp+14h] [ebp-8h]

  v2 = sub_409190(&v7, (int)a1);
  if ( !sub_409110(v2, a2) )
    return 0;
  v3 = v7;
  v4 = v10;
  a1[2] = v9 - v7 + 1;
  v5 = v8;
  a1[3] = v4 - v8 + 1;
  *a1 += v3 * a1[5] + v5 * a1[1];
  return 1;
}

// ===== sub_409210 @ 0x00409210..0x00409268 =====
BOOL __fastcall sub_409210(int a1, int a2)
{
  BOOL result; // eax
  int v3; // ecx

  result = 0;
  switch ( *(_DWORD *)(a2 + 16) )
  {
    case 0:
      result = *(_DWORD *)(a1 + 16) == 0;
      break;
    case 1:
    case 2:
      v3 = *(_DWORD *)(a1 + 16);
      result = v3 == 1 || v3 == 2;
      break;
    case 3:
      result = *(_DWORD *)(a1 + 16) == 3;
      break;
    case 4:
      result = *(_DWORD *)(a1 + 16) == 4;
      break;
    case 5:
      result = *(_DWORD *)(a1 + 16) == 5;
      break;
    case 6:
      result = *(_DWORD *)(a1 + 16) == 6;
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_409290 @ 0x00409290..0x004092B0 =====
int __userpurge sub_409290@<eax>(int a1@<ecx>, int a2@<eax>, int a3, int a4, LPCSTR pszFaceName, int a6)
{
  return sub_42F0F0(pszFaceName, *(_DWORD *)(a3 + 4), a4, a6, a1, a2);
}

// ===== sub_4092B0 @ 0x004092B0..0x004092C3 =====
int __userpurge sub_4092B0@<eax>(int a1@<eax>, int a2)
{
  return sub_42F2D0(a1);
}

// ===== sub_4092D0 @ 0x004092D0..0x004092DF =====
int sub_4092D0()
{
  return sub_42F310(*((_DWORD *)dword_566750 + 1));
}

// ===== sub_4092E0 @ 0x004092E0..0x004094B6 =====
unsigned int __thiscall sub_4092E0(void *this, BOOL **a2, int a3, int a4)
{
  int v4; // eax
  BOOL *v5; // edx
  unsigned int v6; // ecx
  unsigned int result; // eax
  BOOL *v8; // ebx
  BOOL *v9; // esi
  unsigned int i; // esi
  unsigned __int8 *v11; // eax
  BOOL *v12; // edi
  BOOL *v13; // ecx
  unsigned __int8 *v14; // ecx
  BOOL *v15; // esi
  BOOL *v16; // edi
  int v17; // eax
  BOOL v18; // eax
  BOOL *v19; // esi
  int v20; // ecx
  BOOL *v21; // [esp+18h] [ebp-1Ch]
  int v22; // [esp+18h] [ebp-1Ch]
  int v23; // [esp+1Ch] [ebp-18h]
  BOOL *v24; // [esp+20h] [ebp-14h]
  unsigned __int8 *v25; // [esp+24h] [ebp-10h]
  BOOL *v26; // [esp+2Ch] [ebp-8h]
  unsigned __int8 *v27; // [esp+30h] [ebp-4h]
  int v28; // [esp+44h] [ebp+10h]

  sub_42E970(a3, (int)this);
  sub_42E990();
  v4 = sub_42E9A0();
  v23 = sub_42E9B0(v4);
  result = (unsigned int)a2[3];
  if ( v6 < result )
    result = v6;
  if ( v5 >= a2[2] )
    v5 = a2[2];
  v26 = v5;
  v8 = *a2;
  v9 = a2[4];
  v24 = a2[1];
  v27 = *(unsigned __int8 **)(a3 + 8);
  v21 = *a2;
  if ( v9 == (BOOL *)2 )
  {
    v28 = a4 & 0xFFFFFF;
    for ( i = result; i; v27 += v23 )
    {
      v11 = v27;
      --i;
      v12 = v8;
      v13 = v5;
      if ( v5 )
      {
        do
        {
          *v12++ = v28 | (*v11++ << 24);
          v13 = (BOOL *)((char *)v13 - 1);
        }
        while ( v13 );
        v5 = v26;
      }
      result = v23;
      v8 = (BOOL *)((char *)v8 + (_DWORD)v24);
    }
  }
  else if ( v9 == (BOOL *)1 )
  {
    for ( ; result; v27 += v23 )
    {
      v14 = v27;
      v22 = --result;
      v15 = v8;
      v16 = v5;
      if ( v5 )
      {
        do
        {
          v17 = *v14;
          v16 = (BOOL *)((char *)v16 - 1);
          v18 = ((v17 * BYTE2(a4)) & 0xFFFF00) != 0
             || ((v17 * BYTE1(a4)) & 0xFFFFFF00) != 0
             || ((v17 * (unsigned __int8)a4) & 0xFFFFFF00) != 0;
          *v15++ = v18;
          ++v14;
        }
        while ( v16 );
        v5 = v26;
        result = v22;
      }
      v8 = (BOOL *)((char *)v8 + (_DWORD)v24);
    }
  }
  else if ( !v9 && result )
  {
    do
    {
      --result;
      v19 = v8;
      v25 = v27;
      if ( v5 )
      {
        do
        {
          v20 = *v25++;
          *(_WORD *)v19 = ((v20 * (unsigned __int8)a4) >> 11) | ((BYTE1(a4) * v20) >> 6) & 0xFFE0 | ((BYTE2(a4) * v20) >> 1) & 0xFC00;
          v19 = (BOOL *)((char *)v19 + 2);
          v5 = (BOOL *)((char *)v5 - 1);
        }
        while ( v5 );
        v5 = v26;
        v8 = v21;
      }
      v8 = (BOOL *)((char *)v8 + (_DWORD)v24);
      v27 += v23;
      v21 = v8;
    }
    while ( result );
  }
  return result;
}

// ===== sub_4094C0 @ 0x004094C0..0x00409702 =====
int __thiscall sub_4094C0(void *this, int **a2, int a3, int a4, int a5)
{
  signed int v5; // ebx
  int v6; // ecx
  __int64 v7; // rax
  int *v8; // ecx
  bool v9; // zf
  bool v10; // cc
  int v11; // ecx
  signed int v12; // eax
  int v13; // esi
  int v14; // edi
  int v15; // esi
  int v16; // edx
  int v17; // ecx
  long double v18; // st7
  int *v19; // ecx
  int v21; // [esp+Ch] [ebp-64h] BYREF
  int v22; // [esp+14h] [ebp-5Ch]
  __int64 v23; // [esp+28h] [ebp-48h]
  int v24; // [esp+34h] [ebp-3Ch]
  int v25; // [esp+38h] [ebp-38h]
  int v26; // [esp+3Ch] [ebp-34h]
  int v27; // [esp+40h] [ebp-30h]
  int i; // [esp+44h] [ebp-2Ch]
  int *v29; // [esp+48h] [ebp-28h]
  int *v30; // [esp+4Ch] [ebp-24h]
  int v31; // [esp+50h] [ebp-20h]
  int v32; // [esp+54h] [ebp-1Ch]
  int v33; // [esp+58h] [ebp-18h]
  int v34; // [esp+5Ch] [ebp-14h]
  int v35; // [esp+60h] [ebp-10h]
  int v36; // [esp+64h] [ebp-Ch]
  unsigned int v37; // [esp+68h] [ebp-8h]
  int v38; // [esp+6Ch] [ebp-4h]

  v5 = a4;
  sub_42E970((int)&v21, (int)this);
  v29 = *a2;
  v27 = sub_42E990();
  v26 = sub_42E9A0();
  v7 = sub_42E9B0(v6);
  v9 = *(_DWORD *)(HIDWORD(v7) + 16) == 2;
  v35 = v7;
  if ( v9 )
  {
    v10 = *(_DWORD *)(HIDWORD(v7) + 12) <= 0;
    v31 = 0;
    if ( !v10 )
    {
      LODWORD(v7) = *(_DWORD *)(HIDWORD(v7) + 8);
      do
      {
        v30 = v8;
        v32 = 0;
        if ( (int)v7 > 0 )
        {
          v11 = -v5;
          for ( i = -v5; ; v11 = i )
          {
            LODWORD(v7) = 0;
            v37 = 0;
            v34 = v11;
            if ( v11 <= v5 )
            {
              v12 = a3;
              v13 = -a3;
              v14 = v31 + v11 - v5;
              v24 = -a3;
              v33 = v11 + v5;
              do
              {
                v36 = v13;
                if ( v13 <= v12 )
                {
                  v15 = v32 + v13 - v12;
                  do
                  {
                    if ( v15 >= 0 && v15 < v27 && v14 >= 0 && v14 < v26 )
                    {
                      if ( v5 == v12 && (unsigned int)v5 <= 5 && (unsigned int)v12 <= 5 )
                      {
                        v16 = dword_50B0D4[v5];
                        v17 = v12 + v36 + v33 * (2 * v5 + 1);
                        v5 = a4;
                        v37 += (*(_DWORD *)(v16 + 4 * v17) * (unsigned int)*(unsigned __int8 *)(v15 + v35 * v14 + v22)) >> 16;
                      }
                      else
                      {
                        *(double *)&v23 = (double)(unsigned int)v5;
                        v25 = v12;
                        v38 = v11 * v11;
                        v18 = sqrt(
                                (double)v36
                              * *(double *)&v23
                              / (double)(unsigned int)v12
                              * ((double)v36
                               * *(double *)&v23
                               / (double)(unsigned int)v12)
                              + (double)(v11 * v11))
                            - *(double *)&v23;
                        if ( v18 > 0.0 )
                        {
                          if ( v18 < 1.0 )
                          {
                            v38 = *(unsigned __int8 *)(v15 + v35 * v14 + v22);
                            v25 = HIWORD(v38) | 0xC00;
                            v23 = (__int64)(v18 * (double)v38);
                            v37 += v23;
                          }
                        }
                        else
                        {
                          v37 += *(unsigned __int8 *)(v15 + v35 * v14 + v22);
                        }
                      }
                      v12 = a3;
                    }
                    ++v15;
                    ++v36;
                    v11 = v34;
                  }
                  while ( v36 <= v12 );
                  v13 = v24;
                }
                ++v33;
                ++v11;
                ++v14;
                v34 = v11;
              }
              while ( v11 <= v5 );
              v7 = __PAIR64__((unsigned int)a2, v37);
              if ( v37 >= 0x100 )
                LODWORD(v7) = 255;
            }
            v19 = v30 + 1;
            *v30 = a5 | ((_DWORD)v7 << 24);
            LODWORD(v7) = *(_DWORD *)(HIDWORD(v7) + 8);
            v30 = v19;
            if ( ++v32 >= (int)v7 )
              break;
          }
          v8 = v29;
        }
        v8 = (int *)((char *)v8 + *(_DWORD *)(HIDWORD(v7) + 4));
        v29 = v8;
        ++v31;
      }
      while ( v31 < *(_DWORD *)(HIDWORD(v7) + 12) );
    }
  }
  return v7;
}

// ===== sub_409710 @ 0x00409710..0x00409782 =====
int __thiscall sub_409710(_DWORD *this, int a2)
{
  int v2; // eax
  int v3; // eax
  _DWORD v5[8]; // [esp+0h] [ebp-24h]

  v5[0] = 4;
  v5[1] = 4;
  v5[2] = 4;
  v5[3] = 3;
  v5[4] = 3;
  v5[5] = 3;
  v5[6] = 3;
  v2 = this[5] - this[3];
  v5[7] = 2;
  v3 = 16 * (v2 + 1) / a2;
  if ( v3 >= 8 )
    return 1;
  else
    return v5[v3];
}

// ===== sub_409790 @ 0x00409790..0x004097A3 =====
int __stdcall sub_409790(int a1)
{
  return (a1 * dword_5076B0) >> 16;
}

// ===== sub_4097B0 @ 0x004097B0..0x004097CE =====
int __userpurge sub_4097B0@<eax>(int a1@<eax>, int a2)
{
  return a2 * a1 / 100;
}

// ===== sub_4097D0 @ 0x004097D0..0x00409800 =====
int __userpurge sub_4097D0@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  return sub_409800(a4, a5, a6, a7, a8, a10, a2, a1, 0, 0);
}

// ===== sub_409800 @ 0x00409800..0x00409A7A =====
int __userpurge sub_409800@<eax>(
        int a1@<eax>,
        int a2,
        int *a3,
        int a4,
        int a5,
        _BYTE *a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11)
{
  int v11; // ebx
  int v12; // edi
  int v13; // esi
  int v15; // [esp+10h] [ebp-A8h]
  int v16; // [esp+14h] [ebp-A4h]
  _BYTE *v17; // [esp+18h] [ebp-A0h]
  unsigned int v18; // [esp+1Ch] [ebp-9Ch]
  int v19; // [esp+20h] [ebp-98h]
  int v20; // [esp+28h] [ebp-90h]
  int v21; // [esp+2Ch] [ebp-8Ch]
  int v22; // [esp+30h] [ebp-88h]
  unsigned int v23; // [esp+34h] [ebp-84h]
  int v24; // [esp+40h] [ebp-78h]
  void *v25[3]; // [esp+50h] [ebp-68h] BYREF
  int v26[7]; // [esp+68h] [ebp-50h] BYREF
  char v27[32]; // [esp+84h] [ebp-34h] BYREF
  int v28; // [esp+A4h] [ebp-14h]
  int v29; // [esp+A8h] [ebp-10h]
  int v30; // [esp+B4h] [ebp-4h]

  if ( !sub_4092B0((int)v27, a1) )
    return 0;
  v11 = v28;
  sub_409080(v25, 1);
  v12 = v11 * v29 / 100;
  v23 = *(_DWORD *)(a2 + 8) - v12;
  v20 = v12;
  v19 = sub_4097B0(v11, a11);
  v21 = a4;
  v15 = 1;
  v16 = 0;
  v18 = 0;
  v17 = a6;
  for ( *a3 = 0; *v17; v17 += (v22 != 0) + 1 )
  {
    v22 = sub_42EA10();
    if ( *v17 == 10 )
    {
      ++v15;
      a5 += v11 + v19;
      a4 = v21;
      v16 = 0;
      if ( !a10 )
      {
        if ( v18 < *a3 )
          v18 = *a3;
        *a3 = 0;
      }
    }
    else
    {
      sub_4092E0((void *)v30, (BOOL **)v25, (int)v26, a7);
      v24 = (int)v25[2];
      v13 = 0;
      if ( a9 )
      {
        sub_409A80();
        v13 = sub_409710(v26, v11);
      }
      else if ( v22 )
      {
        v24 = v12;
      }
      else
      {
        v24 = v12 / 2;
      }
      if ( a10 && a4 + v24 + 2 * v13 > v23 )
      {
        if ( v16 || !sub_42EA30() )
        {
          a5 += v11 + v19;
          ++v15;
          a4 = v21;
          v16 = 0;
        }
        else
        {
          v16 = 1;
        }
      }
      if ( sub_40A530(v13 + a4, a8, 0) )
        break;
      v12 = v20;
      a4 += v24 + 2 * v13;
      if ( a10 )
        *a3 = v15;
      else
        *a3 += v24 + 2 * v13;
    }
  }
  operator delete[](v25[0]);
  if ( !a10 && v18 > *a3 )
    *a3 = v18;
  return 1;
}

// ===== sub_409A80 @ 0x00409A80..0x00409AA3 =====
int __usercall sub_409A80@<eax>(_DWORD *a1@<edx>, int a2@<edi>)
{
  int v2; // esi
  unsigned int v3; // eax
  int result; // eax

  v2 = *(_DWORD *)(a2 + 12);
  v3 = *(_DWORD *)(a2 + 20) - v2 + 1;
  if ( v3 > a1[2] - v2 )
    v3 = a1[2] - v2;
  a1[2] = v3;
  result = *(_DWORD *)(a2 + 12) * a1[5];
  *a1 += result;
  return result;
}

// ===== sub_409AB0 @ 0x00409AB0..0x00409AED =====
int __usercall sub_409AB0@<eax>(int result@<eax>)
{
  int v1; // esi
  int v2; // edi

  v1 = result + 24;
  v2 = 8;
  do
  {
    if ( *(_DWORD *)v1 )
    {
      operator delete[](*(void **)(v1 + 20));
      result = 0;
      *(_DWORD *)v1 = 0;
      *(_DWORD *)(v1 + 4) = 0;
      *(_DWORD *)(v1 + 8) = 0;
      *(_DWORD *)(v1 + 12) = 0;
      *(_DWORD *)(v1 + 16) = 0;
      *(_DWORD *)(v1 + 20) = 0;
    }
    v1 += 24;
    --v2;
  }
  while ( v2 );
  return result;
}

// ===== sub_409AF0 @ 0x00409AF0..0x00409C52 =====
int __userpurge sub_409AF0@<eax>(unsigned int a1@<eax>, int a2, unsigned int a3, int a4, int a5)
{
  int v5; // ebx
  void **v6; // edi
  unsigned int v7; // ebx
  int v8; // esi
  _WORD *v9; // eax
  _WORD *v10; // esi
  unsigned int i; // edi
  int v12; // eax
  int v13; // edx
  int v15; // [esp+Ch] [ebp-1Ch]

  v5 = a2;
  if ( a1 >= 8 )
    return 16;
  v6 = (void **)((char *)dword_566750 + 24 * a1 + 24);
  if ( *v6 )
    operator delete[](v6[5]);
  if ( !a2 )
    v5 = 1;
  v7 = 4 * v5;
  if ( !a4 )
    a4 = 1;
  if ( !a5 )
    a5 = 1;
  v8 = a5 * a4 * v7;
  v6[3] = (void *)(a4 * v7);
  v6[1] = 0;
  v6[2] = 0;
  *v6 = (void *)1;
  v6[4] = (void *)v8;
  v9 = operator new[](4 * v8);
  v6[5] = v9;
  v10 = v9;
  v15 = a5;
  do
  {
    for ( i = 0; i < v7; v10 += 2 )
    {
      v12 = (int)(sin((double)i * 6.283185307179586 / (double)v7) * (double)a3);
      *v10 = v12;
      v10[1] = v12;
      ++i;
    }
    v13 = a4 - 1;
    if ( a4 != 1 )
    {
      do
      {
        if ( v7 )
        {
          memset(v10, 0, 4 * v7);
          v10 += 2 * v7;
        }
        --v13;
      }
      while ( v13 );
    }
    --v15;
  }
  while ( v15 );
  return 0;
}

// ===== sub_409C60 @ 0x00409C60..0x00409F31 =====
int __userpurge sub_409C60@<eax>(unsigned int a1@<eax>, int a2, unsigned int a3, int a4, int a5, int a6, int a7)
{
  int v7; // ebx
  void **v8; // esi
  unsigned int v9; // ebx
  int v10; // edi
  _WORD *v11; // eax
  _WORD *v12; // esi
  int v13; // ecx
  char v14; // cl
  unsigned int i; // edi
  int v16; // eax
  bool v17; // zf
  unsigned int j; // edi
  int v19; // eax
  int v20; // eax
  unsigned int k; // edi
  int v22; // eax
  int v24; // [esp+10h] [ebp-30h]
  int v25; // [esp+10h] [ebp-30h]
  int v26; // [esp+10h] [ebp-30h]
  int v27; // [esp+14h] [ebp-2Ch]
  int v28; // [esp+14h] [ebp-2Ch]
  int v29; // [esp+18h] [ebp-28h]
  int v30; // [esp+20h] [ebp-20h]

  v7 = a2;
  if ( a1 >= 8 )
    return 16;
  v8 = (void **)((char *)dword_566750 + 24 * a1 + 24);
  if ( *v8 )
    operator delete[](v8[5]);
  if ( !a2 )
    v7 = 1;
  v9 = 4 * v7;
  if ( !a6 )
    a6 = 1;
  if ( !a7 )
    a7 = 1;
  v30 = v9 * (a4 + a5 + 1);
  v10 = a7 * a6 * v30;
  v8[3] = (void *)(a6 * v30);
  v8[1] = 0;
  v8[2] = 0;
  *v8 = (void *)1;
  v8[4] = (void *)v10;
  v11 = operator new[](4 * v10);
  v8[5] = v11;
  v12 = v11;
  v13 = a6 - 1;
  v29 = a7;
  while ( 1 )
  {
    if ( v13 )
    {
      v24 = v13;
      do
      {
        if ( v30 )
        {
          memset(v12, 0, 4 * v30);
          v12 += 2 * v30;
        }
        --v24;
      }
      while ( v24 );
    }
    if ( a4 )
    {
      v14 = a4;
      v25 = a4;
      v27 = a4;
      do
      {
        for ( i = 0; i < v9; v12 += 2 )
        {
          v16 = (int)(sin((double)i * 6.283185307179586 / (double)v9) * (double)a3 / (double)(1 << v14));
          *v12 = v16;
          v12[1] = v16;
          ++i;
        }
        v14 = v25 - 1;
        v17 = v27-- == 1;
        --v25;
      }
      while ( !v17 );
    }
    for ( j = 0; j < v9; v12 += 2 )
    {
      v19 = (int)(sin((double)j * 6.283185307179586 / (double)v9) * (double)a3);
      *v12 = v19;
      v12[1] = v19;
      ++j;
    }
    v20 = 1;
    if ( a5 )
    {
      v26 = a5;
      do
      {
        v28 = __ROL4__(v20, 1);
        for ( k = 0; k < v9; v12 += 2 )
        {
          v22 = (int)(sin((double)k * 6.283185307179586 / (double)v9) * (double)a3 / (double)v28);
          *v12 = v22;
          v12[1] = v22;
          ++k;
        }
        v17 = v26-- == 1;
        v20 = v28;
      }
      while ( !v17 );
    }
    if ( !--v29 )
      break;
    v13 = a6 - 1;
  }
  return 0;
}

// ===== sub_409F40 @ 0x00409F40..0x00409FF0 =====
int __userpurge sub_409F40@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5)
{
  int v5; // esi
  int result; // eax
  _DWORD *v8; // esi
  signed int v9; // edx
  unsigned int v10; // ecx
  unsigned int v11; // edi
  int v12; // edx
  unsigned int v13; // ebx
  int v14; // [esp+Ch] [ebp-4h] BYREF

  v5 = dword_565B30;
  result = sub_409FF0(&v14, a4, a1);
  if ( !result )
  {
    if ( v14 )
    {
      v8 = (_DWORD *)(v5 + 8 * (3 * a3 + 3));
      v9 = v8[1] - 4 * a4;
      if ( v9 < v8[2] )
        v9 = v8[2] + v8[3] - (unsigned int)(v8[2] - v9) % v8[3];
      v10 = 0;
      v11 = 4 * a1;
      if ( v11 )
      {
        v12 = 4 * v9;
        do
        {
          v13 = (unsigned int)(a5 * *(__int16 *)(v8[5] + v12)) >> 8;
          *(_DWORD *)(a2 + 4 * v10++) = (unsigned __int16)v13 | (v13 << 16);
          v12 += 4;
        }
        while ( v10 < v11 );
      }
      return 0;
    }
    else
    {
      return 18;
    }
  }
  return result;
}

// ===== sub_409FF0 @ 0x00409FF0..0x0040A061 =====
int __userpurge sub_409FF0@<eax>(unsigned int a1@<eax>, int a2@<ecx>, _DWORD *a3, int a4, int a5)
{
  int v5; // eax
  bool v6; // zf
  _DWORD *v7; // ecx
  signed int v8; // edx

  if ( a1 >= 8 )
    return 16;
  v5 = 3 * a1 + 3;
  v6 = *(_DWORD *)(a2 + 8 * v5) == 0;
  v7 = (_DWORD *)(a2 + 8 * v5);
  if ( v6 )
    return 17;
  v8 = v7[1] - 4 * a4;
  if ( v8 < v7[2] )
    v8 = v7[2] + v7[3] - (unsigned int)(v7[2] - v8) % v7[3];
  *a3 = v7[4] - v8 >= (unsigned int)(4 * a5);
  return 0;
}

// ===== sub_40A070 @ 0x0040A070..0x0040A251 =====
int __thiscall sub_40A070(_DWORD *this, int a2)
{
  _DWORD *v2; // edi
  unsigned int i; // eax
  int v5; // ecx
  _DWORD *v6; // eax
  unsigned int v7; // edi
  char *v8; // ebx
  unsigned int v9; // esi
  _DWORD *v10; // eax
  int v11; // edx
  int v13; // edx
  int v14; // edx
  int v15; // edx
  _DWORD *v16; // [esp+Ch] [ebp-81Ch]
  _DWORD *v17; // [esp+10h] [ebp-818h]
  _DWORD v18[3]; // [esp+14h] [ebp-814h] BYREF
  _BYTE v19[2052]; // [esp+20h] [ebp-808h] BYREF

  v2 = dword_566750;
  for ( i = 0; i < 3; ++i )
  {
    v5 = this[2 * i];
    if ( v5 <= 0 || v5 >= 255 || this[2 * i + 1] >= 0x100u )
      return 22;
  }
  v6 = (_DWORD *)*((_DWORD *)dword_566750 + 54);
  if ( v6 )
  {
    while ( a2 != *v6 )
    {
      v6 = (_DWORD *)v6[769];
      if ( !v6 )
        goto LABEL_9;
    }
  }
  else
  {
LABEL_9:
    v6 = operator new(0xC08u);
    *v6 = a2;
    v6[769] = v2[54];
    v2[54] = v6;
  }
  v18[0] = v6 + 513;
  v7 = 0;
  v8 = (char *)(this + 4);
  v18[1] = v6 + 257;
  v18[2] = v6 + 1;
  v16 = v18;
  v17 = this + 5;
  do
  {
    sub_494730(v19, 0, 0, *v17, 255, 255, 256);
    v9 = 0;
    v10 = (_DWORD *)(*v16 + 8);
    do
    {
      v11 = *(_DWORD *)&v19[8 * v9 + 4];
      if ( v11 <= 0 )
      {
        v11 = 0;
      }
      else if ( v11 >= 256 )
      {
        v11 = 255;
      }
      *(v10 - 2) = v11 << v7;
      v13 = *(_DWORD *)&v19[8 * v9 + 12];
      if ( v13 <= 0 )
      {
        v13 = 0;
      }
      else if ( v13 >= 256 )
      {
        v13 = 255;
      }
      *(v10 - 1) = v13 << v7;
      v14 = *(_DWORD *)&v19[8 * v9 + 20];
      if ( v14 <= 0 )
      {
        v14 = 0;
      }
      else if ( v14 >= 256 )
      {
        v14 = 255;
      }
      *v10 = v14 << v7;
      v15 = *(_DWORD *)&v19[8 * v9 + 28];
      if ( v15 <= 0 )
      {
        v15 = 0;
      }
      else if ( v15 >= 256 )
      {
        v15 = 255;
      }
      v9 += 4;
      v10 += 4;
      *(v10 - 3) = v15 << v7;
    }
    while ( v9 < 0x100 );
    v17 -= 2;
    ++v16;
    v7 += 8;
    v8 -= 8;
  }
  while ( v7 < 0x18 );
  return 0;
}

// ===== sub_40A260 @ 0x0040A260..0x0040A29A =====
int __usercall sub_40A260@<eax>(int a1@<edx>, int a2@<esi>)
{
  _DWORD *v2; // ecx
  _DWORD *v3; // edx
  int result; // eax

  v2 = *(_DWORD **)(a1 + 216);
  v3 = (_DWORD *)(a1 + 216);
  result = 16;
  if ( v2 )
  {
    while ( a2 != *v2 )
    {
      v3 = v2 + 769;
      v2 = (_DWORD *)v2[769];
      if ( !v2 )
        return result;
    }
    *v3 = v2[769];
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_40A2A0 @ 0x0040A2A0..0x0040A2EB =====
int __stdcall sub_40A2A0(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // eax

  v7 = sub_40A2F0(dword_566750);
  if ( v7 )
    return sub_419150(a1, a2, a3, a4, v7, a5, a6, a7, 1);
  else
    return 16;
}

// ===== sub_40A2F0 @ 0x0040A2F0..0x0040A312 =====
_DWORD *__fastcall sub_40A2F0(int a1, int a2)
{
  _DWORD *v2; // ecx
  _DWORD *result; // eax

  v2 = *(_DWORD **)(a1 + 216);
  result = 0;
  if ( v2 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[769];
      if ( !v2 )
        return result;
    }
    return v2;
  }
  return result;
}

// ===== sub_40A320 @ 0x0040A320..0x0040A530 =====
int __thiscall sub_40A320(int *this, int a2, int a3, unsigned __int8 *a4, int a5, int a6, int a7)
{
  int v8; // edi
  int v9; // esi
  int v10; // ebx
  int v11; // eax
  unsigned __int8 *v12; // edi
  int v13; // eax
  char v14; // cl
  _BYTE v16[32]; // [esp+10h] [ebp-268h] BYREF
  int v17; // [esp+30h] [ebp-248h]
  _BYTE v18[4]; // [esp+44h] [ebp-234h] BYREF
  void *v19[4]; // [esp+48h] [ebp-230h] BYREF
  int v20; // [esp+58h] [ebp-220h]
  void *v21; // [esp+60h] [ebp-218h]
  int *v22; // [esp+64h] [ebp-214h]
  unsigned __int8 *v23; // [esp+68h] [ebp-210h]
  int v24; // [esp+6Ch] [ebp-20Ch]
  char v25[256]; // [esp+70h] [ebp-208h] BYREF
  char Buffer[260]; // [esp+170h] [ebp-108h] BYREF

  v23 = a4;
  v21 = dword_566750;
  v22 = this;
  if ( sub_4092B0((int)v16, (int)dword_566750) )
  {
    sub_409030(this[4], v17, v19, 54 * v17 / 2);
    v8 = a5;
    v9 = (a5 + 15) / 16;
    v10 = 0;
    if ( v9 )
    {
      while ( 1 )
      {
        --v9;
        if ( v20 )
        {
          if ( v20 == 1 )
          {
            sub_40A840(0);
          }
          else if ( v20 == 2 )
          {
            sub_40A840(-1610612736);
          }
        }
        else
        {
          sub_40A7C0(v19);
        }
        memset(v25, 0, sizeof(v25));
        if ( v8 >= 16 )
        {
          v11 = 16;
        }
        else
        {
          v11 = v8;
          if ( v8 <= 0 )
            goto LABEL_17;
        }
        v24 = v11;
        do
        {
          v12 = v23;
          sprintf(Buffer, "%s%.2X ", v25, *v23);
          v23 = v12 + 1;
          v13 = 0;
          do
          {
            v14 = Buffer[v13];
            v25[v13++] = v14;
          }
          while ( v14 );
          --v24;
        }
        while ( v24 );
LABEL_17:
        sprintf(Buffer, "%.4X : %s", v10, v25);
        sub_4097D0(0, 0, (int)v21, (int)v19, (int)v18, 0, 0, (int)Buffer, a6, a7);
        sub_40A530(a2, 128, 0);
        a3 += v17;
        a5 -= 16;
        v10 += 16;
        if ( !v9 )
          break;
        v8 = a5;
      }
    }
    operator delete[](v19[0]);
  }
  return 0;
}

// ===== sub_40A530 @ 0x0040A530..0x0040A612 =====
int __usercall sub_40A530@<eax>(int *a1@<eax>, _DWORD *a2@<ecx>, int a3@<edi>, int a4, int a5, int a6)
{
  int v6; // edx
  int v7; // ecx
  int v8; // edx
  int v9; // ecx
  int v10; // edx
  int v11; // ecx
  int v12; // edx
  int v13; // eax
  _DWORD v15[6]; // [esp+8h] [ebp-54h] BYREF
  _DWORD v16[6]; // [esp+20h] [ebp-3Ch] BYREF
  int v17[4]; // [esp+38h] [ebp-24h] BYREF
  int v18[5]; // [esp+48h] [ebp-14h] BYREF

  v15[0] = *a2;
  v15[1] = a2[1];
  v15[2] = a2[2];
  v15[3] = a2[3];
  v6 = a2[4];
  v7 = a2[5];
  v15[4] = v6;
  v8 = *a1;
  v15[5] = v7;
  v9 = a1[1];
  v16[0] = v8;
  v10 = a1[2];
  v16[1] = v9;
  v11 = a1[3];
  v16[2] = v10;
  v12 = a1[4];
  v13 = a1[5];
  v16[3] = v11;
  v16[5] = v13;
  v16[4] = v12;
  sub_409190(v17, (int)v15);
  sub_409190(v18, (int)v16);
  sub_409170(-a3, -a4, v17);
  if ( !sub_409110(v18, v17) )
    return 4;
  sub_4091B0(v16, v18);
  sub_409170(a3, a4, v18);
  sub_4091B0(v15, v18);
  return sub_40A9E0(v15, v16, a5, a6, 1);
}

// ===== sub_40A620 @ 0x0040A620..0x0040A709 =====
int __usercall sub_40A620@<eax>(int a1@<eax>, int *a2@<ecx>)
{
  int v2; // edx
  int v3; // ebx
  __m64 *v4; // esi
  int v5; // edi
  int v6; // eax
  int result; // eax
  size_t v8; // ebx
  size_t j; // ebx
  float *v10; // ecx
  size_t i; // ebx
  __m64 *v12; // ecx
  __m64 *v13; // [esp+10h] [ebp-18h] BYREF
  int v14; // [esp+14h] [ebp-14h]
  int v15; // [esp+18h] [ebp-10h]
  int v16; // [esp+1Ch] [ebp-Ch]
  int v17; // [esp+20h] [ebp-8h]
  int v18; // [esp+24h] [ebp-4h]

  v2 = *(_DWORD *)(a1 + 4);
  v3 = *(_DWORD *)(a1 + 16);
  v4 = *(__m64 **)a1;
  v15 = *(_DWORD *)(a1 + 8);
  v5 = *(_DWORD *)(a1 + 12);
  v6 = *(_DWORD *)(a1 + 20);
  v13 = v4;
  v14 = v2;
  v16 = v5;
  v17 = v3;
  v18 = v6;
  if ( a2 )
  {
    sub_4091B0(&v13, a2);
    v6 = v18;
    v5 = v16;
    v4 = v13;
    v2 = v14;
  }
  result = v15 * v6;
  v8 = result;
  if ( (result & 7) != 0 )
  {
    for ( ; v5; --v5 )
    {
      result = (int)memset(v4, 0, v8);
      v4 = (__m64 *)((char *)v4 + v14);
    }
  }
  else
  {
    result |= (unsigned int)v4 | v2;
    if ( (result & 0xF) != 0 )
    {
      for ( i = v8 >> 3; v5; v4 = (__m64 *)((char *)v4 + v2) )
      {
        --v5;
        v12 = v4;
        result = i;
        if ( i )
        {
          do
          {
            _mm_stream_pi(v12++, 0LL);
            --result;
          }
          while ( result );
        }
      }
      _m_empty();
    }
    else
    {
      for ( j = v8 >> 4; v5; v4 = (__m64 *)((char *)v4 + v2) )
      {
        --v5;
        v10 = (float *)v4;
        result = j;
        if ( j )
        {
          do
          {
            _mm_stream_ps(v10, (__m128)0LL);
            v10 += 4;
            --result;
          }
          while ( result );
        }
      }
    }
  }
  return result;
}

// ===== sub_40A710 @ 0x0040A710..0x0040A7A2 =====
int __usercall sub_40A710@<eax>(_DWORD *a1@<eax>, int *a2@<edx>, int Val)
{
  int v3; // ecx
  int result; // eax
  int v5; // edi
  _DWORD v6[4]; // [esp+8h] [ebp-18h] BYREF
  int v7; // [esp+18h] [ebp-8h]
  int v8; // [esp+1Ch] [ebp-4h]

  v6[0] = *a1;
  v6[1] = a1[1];
  v6[2] = a1[2];
  v6[3] = a1[3];
  v3 = a1[4];
  result = a1[5];
  v5 = Val;
  v7 = v3;
  v8 = result;
  if ( a2 )
  {
    result = sub_4091B0(v6, a2);
    v3 = v7;
  }
  switch ( v3 )
  {
    case 0:
      result = sub_40A7C0(v6);
      break;
    case 1:
      v5 = Val & 0xFFFFFF;
      goto LABEL_6;
    case 2:
LABEL_6:
      result = sub_40A840(v5);
      break;
    case 3:
      result = sub_40A910(Val);
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_40A7C0 @ 0x0040A7C0..0x0040A834 =====
unsigned int __usercall sub_40A7C0@<eax>(unsigned int a1@<eax>, char **a2@<ecx>)
{
  char *v2; // esi
  unsigned int result; // eax
  char *v4; // edx
  char *v5; // edi
  int i; // ecx
  char *v7; // [esp+8h] [ebp-Ch]
  unsigned int v8; // [esp+Ch] [ebp-8h]
  unsigned __int16 v9; // [esp+10h] [ebp-4h]

  v2 = *a2;
  result = (unsigned __int16)(((a1 >> 3) & 0x1F) + ((a1 >> 6) & 0x3E0) + ((a1 >> 9) & 0x7C00));
  v4 = a2[3];
  v9 = result;
  if ( v4 )
  {
    result = (unsigned int)a2[2];
    v8 = result;
    v7 = a2[1];
    do
    {
      --v4;
      if ( result )
      {
        memset32(v2, (v9 << 16) | v9, result >> 1);
        v5 = &v2[4 * (result >> 1)];
        for ( i = result & 1; i; --i )
        {
          *(_WORD *)v5 = v9;
          v5 += 2;
        }
        result = v8;
      }
      v2 = &v2[(_DWORD)v7];
    }
    while ( v4 );
  }
  return result;
}

// ===== sub_40A840 @ 0x0040A840..0x0040A901 =====
int __cdecl sub_40A840(unsigned int a1)
{
  int v1; // ecx
  int result; // eax
  __m64 *v3; // esi
  unsigned int v4; // edx
  int v5; // edi
  unsigned int v6; // edx
  __m128 j; // xmm0
  float *v8; // ecx
  unsigned int v9; // edx
  __m64 v10; // mm0
  __m64 i; // mm0
  __m64 *v12; // ecx
  int v13; // [esp+Ch] [ebp-4h]

  result = a1;
  v3 = *(__m64 **)v1;
  v13 = *(_DWORD *)(v1 + 4);
  v4 = *(_DWORD *)(v1 + 8);
  v5 = *(_DWORD *)(v1 + 12);
  if ( (v4 & 1) != 0 )
  {
    for ( ; v5; v3 = (__m64 *)((char *)v3 + v13) )
    {
      --v5;
      if ( v4 )
      {
        memset32(v3, result, v4);
        result = a1;
      }
    }
  }
  else if ( (v4 & 3) != 0 || (((unsigned __int8)v3 | (unsigned __int8)v13) & 0xF) != 0 )
  {
    v9 = v4 >> 1;
    v10 = _mm_cvtsi32_si64(a1);
    for ( i = _m_punpckldq(v10, v10); v5; v3 = (__m64 *)((char *)v3 + v13) )
    {
      --v5;
      v12 = v3;
      result = v9;
      if ( v9 )
      {
        do
        {
          _mm_stream_pi(v12++, i);
          --result;
        }
        while ( result );
      }
    }
    _m_empty();
  }
  else
  {
    v6 = v4 >> 2;
    for ( j = (__m128)_mm_shuffle_epi32(_mm_cvtsi32_si128(a1), 0); v5; v3 = (__m64 *)((char *)v3 + v13) )
    {
      --v5;
      v8 = (float *)v3;
      result = v6;
      if ( v6 )
      {
        do
        {
          _mm_stream_ps(v8, j);
          v8 += 4;
          --result;
        }
        while ( result );
      }
    }
  }
  return result;
}

// ===== sub_40A910 @ 0x0040A910..0x0040A997 =====
int __usercall sub_40A910@<eax>(int Val@<ecx>, char **a2@<eax>)
{
  size_t v2; // ebx
  char *v4; // edi
  char *v5; // ecx
  int result; // eax
  int v7; // ebx
  int v8; // esi
  char *i; // edx
  int *v10; // ecx
  int v11; // [esp+Ch] [ebp-8h]
  char *v12; // [esp+10h] [ebp-4h]

  v2 = (size_t)a2[2];
  v4 = *a2;
  v12 = a2[1];
  v5 = a2[3];
  if ( (v2 & 3) != 0 )
  {
    result = (int)a2[3];
    if ( v5 )
    {
      do
      {
        v11 = --result;
        if ( v2 )
        {
          memset(v4, Val, v2);
          result = v11;
        }
        v4 = &v4[(_DWORD)v12];
      }
      while ( result );
    }
  }
  else
  {
    result = (unsigned __int8)Val;
    v7 = v2 >> 2;
    v8 = result | ((result | ((result | (Val << 8)) << 8)) << 8);
    for ( i = v5; i; v4 = &v4[(_DWORD)v12] )
    {
      --i;
      v10 = (int *)v4;
      for ( result = v7; result; --result )
        _mm_stream_si32(v10++, v8);
    }
  }
  return result;
}

// ===== sub_40A9A0 @ 0x0040A9A0..0x0040A9C8 =====
BOOL __fastcall sub_40A9A0(int a1, int a2)
{
  BOOL result; // eax

  result = a1 == a2;
  if ( a1 != a2 )
  {
    if ( a1 == 1 )
      return a2 == 2;
    else
      return a1 == 2 && a2 == 1;
  }
  return result;
}

// ===== sub_40A9D0 @ 0x0040A9D0..0x0040A9DF =====
BOOL sub_40A9D0()
{
  int v0; // eax
  int v1; // ecx

  v0 = sub_407B10();
  return sub_40A9A0(v0, *(_DWORD *)(v1 + 16));
}

// ===== sub_40A9E0 @ 0x0040A9E0..0x0040AC9C =====
int __cdecl sub_40A9E0(int a1, int a2, unsigned int a3, unsigned int a4, int a5)
{
  int result; // eax
  int v6; // [esp+Ch] [ebp-14h]
  _DWORD v7[2]; // [esp+10h] [ebp-10h] BYREF
  _DWORD v8[2]; // [esp+18h] [ebp-8h] BYREF

  v6 = 0;
  if ( !sub_40A9A0(*(_DWORD *)(a1 + 16), *(_DWORD *)(a2 + 16)) )
    return 1;
  if ( a4 > 0x100 )
    return 3;
  if ( a5 )
  {
    v8[0] = a1;
    v8[1] = a2;
    v7[0] = a3;
    v7[1] = a4;
    if ( sub_419DE0(sub_41A040, v8, 2, 0, 0, v7) )
      return v6;
  }
  if ( a3 > 0xFF )
    return 2;
  if ( a3 == 255 )
  {
    sub_413900(a1);
    return 0;
  }
  else
  {
    switch ( a3 )
    {
      case 0u:
        sub_40B080(a1);
        result = 0;
        break;
      case 1u:
      case 0x20u:
        sub_40B320(a1);
        result = 0;
        break;
      case 2u:
        sub_40CA70(a1, a4);
        result = 0;
        break;
      case 3u:
        sub_40D200(a1, a4);
        result = 0;
        break;
      case 4u:
      case 0x23u:
        sub_40D440(a1);
        result = 0;
        break;
      case 5u:
      case 0xC0u:
        sub_40DAD0(a1);
        result = 0;
        break;
      case 6u:
      case 0x24u:
        sub_414C20(a1);
        result = 0;
        break;
      case 7u:
      case 0x25u:
        sub_4158C0(a1);
        result = 0;
        break;
      case 8u:
      case 0x26u:
        sub_414F00(a1);
        result = 0;
        break;
      case 9u:
      case 0x27u:
        sub_415250(a1);
        result = 0;
        break;
      case 0x21u:
        sub_40CA70(a1, 256 - a4);
        result = 0;
        break;
      case 0x22u:
        sub_40D200(a1, 256 - a4);
        result = 0;
        break;
      case 0x40u:
        sub_40DF80(a2);
        result = 0;
        break;
      case 0x41u:
        sub_40E150(a2);
        result = 0;
        break;
      case 0x80u:
        sub_40AF50(a1);
        result = 0;
        break;
      case 0xC1u:
        sub_40E190(a1);
        result = 0;
        break;
      case 0xF0u:
        sub_40BC10(a1);
        result = 0;
        break;
      default:
        return 2;
    }
  }
  return result;
}

// ===== sub_40ADF0 @ 0x0040ADF0..0x0040AF48 =====
size_t __usercall sub_40ADF0@<eax>(int a1@<eax>, char **a2@<edx>)
{
  char *v2; // esi
  _BYTE *v3; // edi
  size_t result; // eax
  size_t v5; // ecx
  size_t v6; // ecx
  __m128 *v7; // eax
  size_t v8; // edx
  __m64 *v9; // ecx
  __m64 v10; // mm0
  size_t v11; // edx
  __m64 *v12; // eax
  size_t v13; // ecx
  size_t v14; // [esp+8h] [ebp-14h]
  size_t v15; // [esp+8h] [ebp-14h]
  size_t v16; // [esp+8h] [ebp-14h]
  _BYTE *v17; // [esp+Ch] [ebp-10h]
  size_t v18; // [esp+Ch] [ebp-10h]
  char *v19; // [esp+Ch] [ebp-10h]
  char *v20; // [esp+10h] [ebp-Ch]
  int v21; // [esp+14h] [ebp-8h]
  size_t v22; // [esp+18h] [ebp-4h]
  size_t v23; // [esp+18h] [ebp-4h]

  v2 = *a2;
  v3 = *(_BYTE **)a1;
  v21 = *(_DWORD *)(a1 + 4);
  result = (_DWORD)a2[2] * (_DWORD)a2[5];
  v20 = a2[1];
  v5 = (size_t)a2[3];
  v17 = v3;
  v14 = result;
  if ( (result & 0xF) == 0 )
  {
    result >>= 4;
    v22 = result;
    if ( (((unsigned __int8)v3 | (unsigned __int8)((unsigned __int8)v2 | v21 | (unsigned __int8)v20)) & 0xF) == 0 )
    {
      result = (size_t)a2[3];
      if ( v5 )
      {
        v6 = v22;
        do
        {
          v18 = result - 1;
          v7 = (__m128 *)v2;
          if ( v6 )
          {
            do
            {
              _mm_prefetch((const char *)&v7[32], 0);
              _mm_stream_ps((float *)((char *)v7->m128_f32 + v3 - v2), *v7);
              ++v7;
              --v6;
            }
            while ( v6 );
            v6 = v22;
          }
          result = v18;
          v3 += v21;
          v2 = &v2[(_DWORD)v20];
        }
        while ( v18 );
      }
      return result;
    }
    if ( v5 )
    {
      v8 = result;
      do
      {
        v15 = --v5;
        if ( v8 )
        {
          result = (size_t)v3;
          v9 = (__m64 *)(v2 + 8);
          do
          {
            v10 = *(__m64 *)(v2 - v17 + result);
            _mm_prefetch((const char *)&v9[63], 0);
            _mm_stream_pi((__m64 *)result, v10);
            _mm_stream_pi((__m64 *)(result + 8), (__m64)v9->m64_u64);
            result += 16;
            v9 += 2;
            --v8;
          }
          while ( v8 );
          v3 = v17;
          v5 = v15;
          v8 = v22;
        }
        v3 += v21;
        v2 = &v2[(_DWORD)v20];
        v17 = v3;
      }
      while ( v5 );
    }
LABEL_17:
    _m_empty();
    return result;
  }
  if ( (result & 7) == 0 )
  {
    v11 = result >> 3;
    v23 = result >> 3;
    result = v5;
    if ( v5 )
    {
      do
      {
        v16 = result - 1;
        v12 = (__m64 *)v2;
        v13 = v11;
        if ( v11 )
        {
          do
          {
            _mm_prefetch((const char *)&v12[64], 0);
            _mm_stream_pi((__m64 *)((char *)v12 + v3 - v2), (__m64)v12->m64_u64);
            ++v12;
            --v13;
          }
          while ( v13 );
          v11 = v23;
        }
        result = v16;
        v3 += v21;
        v2 = &v2[(_DWORD)v20];
      }
      while ( v16 );
    }
    goto LABEL_17;
  }
  v19 = a2[3];
  if ( v5 )
  {
    while ( 1 )
    {
      --v19;
      result = (size_t)memcpy_0(v3, v2, result);
      v3 += v21;
      v2 = &v2[(_DWORD)v20];
      if ( !v19 )
        break;
      result = v14;
    }
  }
  return result;
}

// ===== sub_40AF50 @ 0x0040AF50..0x0040B07B =====
size_t __usercall sub_40AF50@<eax>(size_t result@<eax>, size_t *a2@<ecx>)
{
  int v2; // esi
  int v3; // esi
  char *v4; // edx
  size_t v5; // esi
  int v6; // edi
  int v7; // ecx
  char *v8; // edx
  char *v9; // edi
  int v10; // edx
  unsigned int *v11; // ecx
  int v12; // [esp-14h] [ebp-18h]
  int v13; // [esp-10h] [ebp-14h]
  size_t v14; // [esp-10h] [ebp-14h]
  size_t v15; // [esp-Ch] [ebp-10h]
  int j; // [esp-8h] [ebp-Ch]
  int i; // [esp-8h] [ebp-Ch]
  char *v18; // [esp-4h] [ebp-8h]
  size_t v19; // [esp-4h] [ebp-8h]

  v2 = *(_DWORD *)(result + 16);
  if ( a2[4] == v2 )
    return sub_40ADF0((int)a2, (char **)result);
  v3 = v2 - 1;
  if ( v3 )
  {
    if ( v3 == 1 )
    {
      v19 = *a2;
      v9 = *(char **)result;
      v12 = *(_DWORD *)(result + 4);
      v10 = *(_DWORD *)(result + 8);
      result = *(_DWORD *)(result + 12);
      v14 = a2[1];
      for ( i = v10; result; v9 += v12 )
      {
        --result;
        v11 = (unsigned int *)v9;
        if ( v10 )
        {
          do
          {
            *(unsigned int *)((char *)v11 + v19 - (_DWORD)v9) = _mm_cvtsi128_si32(
                                                                  _mm_packus_epi16(
                                                                    _mm_srli_epi16(
                                                                      _mm_mullo_epi16(
                                                                        _mm_unpacklo_epi8(
                                                                          _mm_cvtsi32_si128(*v11),
                                                                          (__m128i)0LL),
                                                                        _mm_load_si128(&xmmword_50B0F0[*v11 >> 25])),
                                                                      7u),
                                                                    (__m128i)0LL));
            ++v11;
            --v10;
          }
          while ( v10 );
          v10 = i;
        }
        v19 += v14;
      }
    }
  }
  else
  {
    v4 = *(char **)result;
    v5 = *a2;
    v15 = a2[1];
    v6 = *(_DWORD *)(result + 12);
    v13 = *(_DWORD *)(result + 4);
    v7 = *(_DWORD *)(result + 8);
    v18 = *(char **)result;
    for ( j = v7; v6; v18 += v13 )
    {
      --v6;
      result = v5;
      if ( v7 )
      {
        v8 = &v4[-v5];
        do
        {
          *(_DWORD *)result = *(_DWORD *)&v8[result] | 0xFF000000;
          result += 4;
          --v7;
        }
        while ( v7 );
        v7 = j;
      }
      v4 = &v18[v13];
      v5 += v15;
    }
  }
  return result;
}

// ===== sub_40B080 @ 0x0040B080..0x0040B0CB =====
size_t __usercall sub_40B080@<eax>(size_t result@<eax>, int a2@<ecx>)
{
  int v2; // esi
  int v3; // esi

  v2 = *(_DWORD *)(result + 16);
  if ( !v2 )
    return sub_40B0D0(a2, result);
  v3 = v2 - 1;
  if ( v3 )
  {
    if ( v3 == 1 )
    {
      if ( *(_DWORD *)(a2 + 16) == 1 )
      {
        return sub_40B130(a2, a2);
      }
      else if ( *(_DWORD *)(a2 + 16) == 2 )
      {
        return sub_40B200();
      }
    }
  }
  else if ( *(_DWORD *)(a2 + 16) == 1 )
  {
    return sub_40ADF0(a2, (char **)result);
  }
  else if ( *(_DWORD *)(a2 + 16) == 2 )
  {
    JUMPOUT(0x40AF80);
  }
  return result;
}

// ===== sub_40B0D0 @ 0x0040B0D0..0x0040B12F =====
int __cdecl sub_40B0D0(int *a1, _DWORD *a2)
{
  _DWORD *v2; // ecx
  int v3; // ebx
  int result; // eax
  int i; // esi
  int v6; // edx
  int v7; // eax
  __int16 v8; // cx
  int v9; // [esp+8h] [ebp-4h]

  v2 = a2;
  v3 = *a1;
  result = a2[3];
  for ( i = *a2; result; i += v2[1] )
  {
    v6 = v2[2];
    v9 = --result;
    if ( v6 )
    {
      v7 = i + 2 * v6;
      do
      {
        v8 = *(_WORD *)(v7 - 2);
        v7 -= 2;
        --v6;
        if ( v8 )
          *(_WORD *)(v3 - i + v7) = v8;
      }
      while ( v6 );
      result = v9;
      v2 = a2;
    }
    v3 += a1[1];
  }
  return result;
}

// ===== sub_40B130 @ 0x0040B130..0x0040B200 =====
int __usercall sub_40B130@<eax>(int *a1@<eax>, int *a2@<edx>)
{
  int v2; // ecx
  int v3; // esi
  int v4; // edi
  int v5; // edx
  int result; // eax
  int v7; // esi
  unsigned int v8; // edx
  int v9; // eax
  __m128i v10; // xmm0
  int v11; // [esp+8h] [ebp-1Ch]
  int v12; // [esp+Ch] [ebp-18h]
  int v13; // [esp+10h] [ebp-14h]
  int i; // [esp+14h] [ebp-10h]
  int v15; // [esp+1Ch] [ebp-8h]
  int v16; // [esp+20h] [ebp-4h]

  v2 = *a1;
  v3 = *a2;
  v4 = a1[2];
  v12 = a2[1];
  v5 = a1[1];
  result = a1[3];
  v16 = v3;
  v15 = v2;
  v11 = v5;
  for ( i = v4; result; v15 = v2 )
  {
    v13 = --result;
    if ( v4 )
    {
      v7 = v3 - v2;
      do
      {
        v8 = *(_DWORD *)v2;
        --v4;
        if ( (*(_DWORD *)v2 & 0xFE000000) != 0 )
        {
          v9 = *(unsigned __int8 *)(v2 + 3) >> 1;
          if ( v9 == 127 )
          {
            *(_DWORD *)(v7 + v2) = v8 & 0xFFFFFF;
          }
          else
          {
            v10 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(v7 + v2)), (__m128i)0LL);
            *(_DWORD *)(v7 + v2) = _mm_cvtsi128_si32(
                                     _mm_packus_epi16(
                                       _mm_add_epi16(
                                         v10,
                                         _mm_srai_epi16(
                                           _mm_mullo_epi16(
                                             _mm_sub_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(v8), (__m128i)0LL), v10),
                                             _mm_load_si128(&xmmword_50B0F0[v9])),
                                           7u)),
                                       (__m128i)0LL));
          }
        }
        v2 += 4;
      }
      while ( v4 );
      v4 = i;
      v2 = v15;
      result = v13;
    }
    v3 = v12 + v16;
    v2 += v11;
    v16 += v12;
  }
  return result;
}

// ===== sub_40B200 @ 0x0040B200..0x0040B313 =====
int __usercall sub_40B200@<eax>(int *a1@<eax>, int *a2@<ecx>)
{
  int v2; // esi
  int v3; // edx
  int v4; // ecx
  int result; // eax
  int v6; // edx
  unsigned int v7; // eax
  unsigned int v8; // edi
  int v9; // eax
  int v10; // [esp+8h] [ebp-24h]
  int v11; // [esp+Ch] [ebp-20h]
  int v12; // [esp+10h] [ebp-1Ch]
  int i; // [esp+14h] [ebp-18h]
  int v14; // [esp+18h] [ebp-14h]
  unsigned int v15; // [esp+20h] [ebp-Ch]
  int v16; // [esp+24h] [ebp-8h]
  int v17; // [esp+28h] [ebp-4h]

  v2 = *a2;
  v17 = *a1;
  v3 = a1[1];
  v11 = a2[1];
  v4 = a1[2];
  result = a1[3];
  v16 = v2;
  v10 = v3;
  for ( i = v4; result; v16 = v2 )
  {
    v12 = --result;
    if ( v4 )
    {
      v6 = v17 - v2;
      v14 = v17 - v2;
      do
      {
        v7 = *(_DWORD *)(v6 + v2);
        --v4;
        v15 = v7;
        if ( (v7 & 0xFF000000) != 0 )
        {
          v8 = HIBYTE(v7);
          if ( HIBYTE(v7) != 255 )
          {
            v9 = (256 - v8) * *(unsigned __int8 *)(v2 + 3);
            v6 = v14;
            v7 = (((v9 + (v8 << 8)) & 0xFFFFFF00) << 16) | _mm_cvtsi128_si32(
                                                             _mm_packus_epi16(
                                                               _mm_srli_epi16(
                                                                 _mm_add_epi16(
                                                                   _mm_mullo_epi16(
                                                                     _mm_unpacklo_epi8(
                                                                       _mm_cvtsi32_si128(*(_DWORD *)v2),
                                                                       (__m128i)0LL),
                                                                     _mm_load_si128(&xmmword_50B8F0[(v9 << 8) / (v9 + (v8 << 8))])),
                                                                   _mm_mullo_epi16(
                                                                     _mm_unpacklo_epi8(
                                                                       _mm_cvtsi32_si128(v15),
                                                                       (__m128i)0LL),
                                                                     _mm_load_si128(&xmmword_50B8F0[(v8 << 16) / (v9 + (v8 << 8))]))),
                                                                 8u),
                                                               (__m128i)0LL));
          }
          *(_DWORD *)v2 = v7;
        }
        v2 += 4;
      }
      while ( v4 );
      v2 = v16;
      v4 = i;
      result = v12;
    }
    v2 += v11;
    v17 += v10;
  }
  return result;
}

// ===== sub_40B320 @ 0x0040B320..0x0040B3AC =====
int __usercall sub_40B320@<eax>(int result@<eax>, unsigned int a2@<ecx>, int a3)
{
  int v3; // esi
  int v4; // esi

  if ( !a2 )
    return sub_40B080(result, a3);
  if ( a2 < 0x100 )
  {
    v3 = *(_DWORD *)(result + 16);
    if ( v3 )
    {
      v4 = v3 - 1;
      if ( v4 )
      {
        if ( v4 == 1 )
        {
          if ( *(_DWORD *)(a3 + 16) == 1 )
          {
            return sub_40B6F0(result, a2);
          }
          else if ( *(_DWORD *)(a3 + 16) == 2 )
          {
            return sub_40B9B0(result, a2);
          }
        }
      }
      else if ( *(_DWORD *)(a3 + 16) == 1 )
      {
        return sub_40B4B0(a2);
      }
      else if ( *(_DWORD *)(a3 + 16) == 2 )
      {
        return sub_40B5D0(a2);
      }
    }
    else if ( !*(_DWORD *)(a3 + 16) )
    {
      return sub_40B3B0(a3, result, a2);
    }
  }
  return result;
}

// ===== sub_40B3B0 @ 0x0040B3B0..0x0040B4AE =====
int __cdecl sub_40B3B0(int *a1, _DWORD *a2, int a3)
{
  int v3; // edx
  _DWORD *v4; // ebx
  int result; // eax
  int v6; // edi
  int v7; // esi
  int v8; // ecx
  int v9; // eax
  _WORD *v10; // edx
  __int16 v11; // ax
  unsigned int v12; // ebx
  int v13; // [esp+Ch] [ebp-18h]
  int v14; // [esp+14h] [ebp-10h]
  int i; // [esp+1Ch] [ebp-8h]
  int v16; // [esp+20h] [ebp-4h]

  v3 = *a1;
  v4 = a2;
  result = *a2;
  v6 = a3;
  v7 = 256 - a3;
  v16 = *a1;
  v14 = *a2;
  for ( i = a2[3]; i; v14 = result )
  {
    v8 = v4[2];
    --i;
    if ( v8 )
    {
      v9 = result - v16;
      v10 = (_WORD *)(v3 + 2 * v8);
      v13 = v9;
      do
      {
        --v8;
        if ( *(_WORD *)((char *)--v10 + v9) )
        {
          v11 = *(_WORD *)((char *)v10 + v9);
          v12 = v6 * (*v10 & 0x3E0) + v7 * (v11 & 0x3E0);
          v6 = a3;
          v7 = 256 - a3;
          LOWORD(v12) = ((a3 * (*v10 & 0x1F) + (256 - a3) * (v11 & 0x1Fu)) >> 8)
                      + (((a3 * (*v10 & 0x7C00) + (256 - a3) * (v11 & 0x7C00u)) >> 8) & 0x7C00)
                      + ((v12 >> 8) & 0x3E0);
          v9 = v13;
          *v10 = v12;
          v4 = a2;
        }
      }
      while ( v8 );
      result = v14;
      v3 = v16;
    }
    v3 += a1[1];
    result += v4[1];
    v16 = v3;
  }
  return result;
}

// ===== sub_40B4B0 @ 0x0040B4B0..0x0040B5CB =====
unsigned int __usercall sub_40B4B0@<eax>(int *a1@<eax>, __m128i **a2@<ecx>, unsigned int a3)
{
  __m128i *v3; // edi
  int v4; // ecx
  unsigned int v5; // edx
  unsigned int result; // eax
  __m128i v7; // xmm0
  __m128i v8; // xmm2
  unsigned int *v9; // eax
  unsigned int v10; // esi
  int i; // ecx
  __m128i v12; // xmm0
  unsigned int j; // edx
  __m128i *v14; // eax
  unsigned int k; // esi
  __m128i v16; // xmm0
  int v17; // [esp+8h] [ebp-14h]
  unsigned int v18; // [esp+10h] [ebp-Ch]
  __m128i *v19; // [esp+14h] [ebp-8h]
  int v20; // [esp+18h] [ebp-4h]

  v3 = *a2;
  v20 = *a1;
  v19 = a2[1];
  v4 = a1[3];
  v18 = a1[1];
  v5 = a1[2];
  result = (__int16)(a3 >> 1);
  v7 = _mm_cvtsi32_si128(result);
  v8 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v7, v7), 0);
  if ( (v5 & 1) != 0 )
  {
    for ( ; v4; v20 += v18 )
    {
      v17 = --v4;
      v9 = (unsigned int *)v3;
      v10 = v5;
      if ( v5 )
      {
        for ( i = v20 - (_DWORD)v3; ; i = v20 - (_DWORD)v3 )
        {
          v12 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(unsigned int *)((char *)v9 + i)), (__m128i)0LL);
          *v9 = _mm_cvtsi128_si32(
                  _mm_packus_epi16(
                    _mm_add_epi16(
                      v12,
                      _mm_srai_epi16(
                        _mm_mullo_epi16(_mm_sub_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(*v9), (__m128i)0LL), v12), v8),
                        7u)),
                    (__m128i)0LL));
          ++v9;
          if ( !--v10 )
            break;
        }
        v4 = v17;
      }
      result = v18;
      v3 = (__m128i *)((char *)v3 + (_DWORD)v19);
    }
  }
  else
  {
    for ( j = v5 >> 1; v4; v20 += v18 )
    {
      --v4;
      v14 = v3;
      for ( k = j; k; --k )
      {
        v16 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i *)((char *)v14 + v20 - (_DWORD)v3)), (__m128i)0LL);
        v14->m128i_i64[0] = _mm_packus_epi16(
                              _mm_add_epi16(
                                v16,
                                _mm_srai_epi16(
                                  _mm_mullo_epi16(
                                    _mm_sub_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64(v14), (__m128i)0LL), v16),
                                    v8),
                                  7u)),
                              (__m128i)0LL).m128i_u64[0];
        v14 = (__m128i *)((char *)v14 + 8);
      }
      result = v18;
      v3 = (__m128i *)((char *)v3 + (_DWORD)v19);
    }
  }
  return result;
}

// ===== sub_40B5D0 @ 0x0040B5D0..0x0040B6EF =====
int __usercall sub_40B5D0@<eax>(int *a1@<eax>, int *a2@<ecx>, int a3)
{
  int v4; // ecx
  int v5; // edx
  int v6; // edi
  int v7; // esi
  int result; // eax
  int v9; // eax
  unsigned __int8 *v10; // esi
  int v11; // edi
  unsigned int v12; // ecx
  unsigned int v13; // ebx
  unsigned int v14; // eax
  int v15; // edx
  int v16; // [esp+8h] [ebp-28h]
  int v17; // [esp+Ch] [ebp-24h]
  int v18; // [esp+10h] [ebp-20h]
  int i; // [esp+14h] [ebp-1Ch]
  int v20; // [esp+18h] [ebp-18h]
  int v21; // [esp+1Ch] [ebp-14h]
  int v22; // [esp+20h] [ebp-10h]
  int v23; // [esp+24h] [ebp-Ch]
  int v24; // [esp+28h] [ebp-8h]
  int v25; // [esp+2Ch] [ebp-4h]
  __int16 v26; // [esp+38h] [ebp+8h]

  v4 = *a1;
  v5 = *a2;
  v17 = a1[1];
  v16 = a2[1];
  v6 = a2[2];
  v7 = a2[3];
  result = 256 - a3;
  v24 = v4;
  v23 = v5;
  v18 = v6;
  for ( i = 256 - a3; v7; v23 = v5 )
  {
    v20 = --v7;
    v25 = v6;
    if ( v6 )
    {
      v9 = 255 * result;
      v10 = (unsigned __int8 *)(v4 + 3);
      v22 = v9;
      v11 = v5 + 2;
      v21 = v4 - v5;
      while ( 1 )
      {
        v12 = ((unsigned int)*v10 * (0x10000 - v9)) >> 8;
        v26 = v12 + v9;
        --v25;
        v11 += 4;
        v10 += 4;
        v13 = (v9 << 16) / (v12 + v9);
        v14 = (v12 << 16) / (v12 + v9);
        v15 = v14 * *(v10 - 6);
        *(v10 - 7) = (v14 * *(v10 - 7) + v13 * *(unsigned __int8 *)(v11 - 6)) >> 16;
        *(v10 - 6) = (v15 + v13 * *(unsigned __int8 *)(v11 - 5)) >> 16;
        *(_BYTE *)(v21 + v11 - 4) = (v13 * *(unsigned __int8 *)(v11 - 4) + v14 * *(unsigned __int8 *)(v21 + v11 - 4)) >> 16;
        *(v10 - 4) = HIBYTE(v26);
        if ( !v25 )
          break;
        v9 = v22;
      }
      v7 = v20;
      result = i;
      v6 = v18;
      v4 = v24;
      v5 = v23;
    }
    v4 += v17;
    v5 += v16;
    v24 = v4;
  }
  return result;
}

// ===== sub_40B6F0 @ 0x0040B6F0..0x0040B9AE =====
int __usercall sub_40B6F0@<eax>(int *a1@<ecx>, int a2@<ebp>, unsigned int **a3, int a4)
{
  unsigned int v4; // eax
  __m128i v5; // xmm5
  bool v6; // zf
  unsigned int *v7; // esi
  unsigned int *v8; // edx
  int v9; // ecx
  unsigned int v10; // eax
  unsigned int v11; // edi
  __m128i v12; // xmm1
  unsigned int *v13; // edx
  unsigned int *v14; // ecx
  int v15; // esi
  __m128i v16; // xmm1
  __m128i *v18; // [esp-838h] [ebp-844h]
  int v19; // [esp-834h] [ebp-840h]
  int v20; // [esp-830h] [ebp-83Ch]
  unsigned int *v21; // [esp-82Ch] [ebp-838h]
  int v22; // [esp-828h] [ebp-834h]
  unsigned int *v23; // [esp-824h] [ebp-830h]
  unsigned int *v24; // [esp-820h] [ebp-82Ch]
  int v25; // [esp-81Ch] [ebp-828h]
  int v26; // [esp-81Ch] [ebp-828h]
  int v27; // [esp-818h] [ebp-824h]
  unsigned int *v28; // [esp-814h] [ebp-820h]
  _OWORD v29[128]; // [esp-810h] [ebp-81Ch] BYREF
  unsigned int v30; // [esp-4h] [ebp-10h]
  _DWORD v31[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v31[0] = a2;
  v31[1] = retaddr;
  v30 = (unsigned int)v31 ^ dword_4FB734;
  v27 = *a1;
  v28 = *a3;
  v20 = a1[1];
  v23 = a3[1];
  v24 = a3[3];
  v25 = 256 - a4;
  v4 = 0;
  v21 = a3[2];
  v22 = 0;
  v18 = (__m128i *)v29;
  v19 = 128;
  do
  {
    v5 = _mm_cvtsi32_si128((unsigned __int16)(v4 >> 8));
    v4 = v25 + v22;
    v6 = v19-- == 1;
    *v18 = _mm_unpacklo_epi16(
             _mm_unpacklo_epi16(
               _mm_unpacklo_epi16(v5, _mm_cvtsi32_si128(0)),
               _mm_unpacklo_epi16(v5, _mm_cvtsi32_si128(0))),
             _mm_unpacklo_epi16(
               _mm_unpacklo_epi16(v5, _mm_cvtsi32_si128(0)),
               _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    v22 += v25;
    ++v18;
  }
  while ( !v6 );
  v7 = v24;
  if ( sub_407AE0() )
  {
    if ( v24 )
    {
      do
      {
        v13 = v21;
        v14 = v28;
        v7 = (unsigned int *)((char *)v7 - 1);
        v26 = (int)v7;
        if ( v21 )
        {
          v15 = v27 - (_DWORD)v28;
          do
          {
            v13 = (unsigned int *)((char *)v13 - 1);
            if ( (*v14 & 0xFE000000) != 0 )
            {
              v16 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(unsigned int *)((char *)v14 + v15)), (__m128i)0LL);
              *(unsigned int *)((char *)v14 + v15) = _mm_cvtsi128_si32(
                                                       _mm_packus_epi16(
                                                         _mm_add_epi16(
                                                           v16,
                                                           _mm_srai_epi16(
                                                             _mm_mullo_epi16(
                                                               _mm_sub_epi16(
                                                                 _mm_unpacklo_epi8(
                                                                   _mm_cvtsi32_si128(*v14),
                                                                   (__m128i)0LL),
                                                                 v16),
                                                               _mm_load_si128((const __m128i *)&v29[*v14 >> 25])),
                                                             7u)),
                                                         (__m128i)0LL));
            }
            ++v14;
          }
          while ( v13 );
          v7 = (unsigned int *)v26;
          v14 = v28;
        }
        v27 += v20;
        v28 = (unsigned int *)((char *)v14 + (_DWORD)v23);
      }
      while ( v7 );
    }
  }
  else if ( v24 )
  {
    do
    {
      v8 = v21;
      v9 = v27;
      v7 = (unsigned int *)((char *)v7 - 1);
      if ( v21 )
      {
        do
        {
          v10 = *(unsigned int *)((char *)v28 + v9 - v27);
          v8 = (unsigned int *)((char *)v8 - 1);
          if ( (v10 & 0xFE000000) != 0 )
          {
            v11 = *(_DWORD *)v9;
            _mm_prefetch((const char *)(v9 + 128), 0);
            v12 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(v11), (__m128i)0LL);
            *(_DWORD *)v9 = _mm_cvtsi128_si32(
                              _mm_packus_epi16(
                                _mm_add_epi16(
                                  v12,
                                  _mm_srai_epi16(
                                    _mm_mullo_epi16(
                                      _mm_sub_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(v10), (__m128i)0LL), v12),
                                      _mm_load_si128((const __m128i *)&v29[v10 >> 25])),
                                    7u)),
                                (__m128i)0LL));
          }
          v9 += 4;
        }
        while ( v8 );
        v9 = v27;
      }
      v27 = v20 + v9;
      v28 = (unsigned int *)((char *)v28 + (_DWORD)v23);
    }
    while ( v7 );
  }
  return sub_4AB245((unsigned int)v31 ^ v30);
}

// ===== sub_40B9B0 @ 0x0040B9B0..0x0040BC0A =====
int __usercall sub_40B9B0@<eax>(int *a1@<ecx>, int a2@<ebp>, int *a3, int a4)
{
  void *v4; // esp
  int v5; // edx
  int v6; // ecx
  int v7; // edx
  int v8; // ecx
  int v9; // edx
  int v10; // eax
  __m128i v11; // xmm7
  __m128i *v12; // ecx
  int v13; // edx
  __m128i v14; // xmm0
  int v15; // ecx
  int v16; // edi
  __m128i *v17; // eax
  bool v18; // zf
  int v19; // esi
  unsigned int v20; // eax
  unsigned int v21; // ecx
  __m128i v22; // xmm1
  __m128i si128; // xmm3
  unsigned int v24; // edx
  __m128i v26; // [esp-1060h] [ebp-106Ch] BYREF
  int v27; // [esp-1048h] [ebp-1054h]
  int v28; // [esp-1044h] [ebp-1050h]
  int v29; // [esp-1040h] [ebp-104Ch]
  int v30; // [esp-103Ch] [ebp-1048h]
  int v31; // [esp-1038h] [ebp-1044h]
  int v32; // [esp-1034h] [ebp-1040h]
  int v33; // [esp-1030h] [ebp-103Ch]
  int v34; // [esp-102Ch] [ebp-1038h]
  int v35; // [esp-1028h] [ebp-1034h]
  __m128i *v36; // [esp-1024h] [ebp-1030h]
  _OWORD v37[257]; // [esp-1020h] [ebp-102Ch] BYREF
  unsigned int v38; // [esp-4h] [ebp-10h]
  _DWORD v39[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v39[0] = a2;
  v39[1] = retaddr;
  v4 = alloca(4200);
  v38 = (unsigned int)v39 ^ dword_4FB734;
  v5 = *a1;
  v6 = a1[1];
  v33 = v5;
  v32 = *a3;
  v7 = a3[1];
  v27 = v6;
  v8 = a3[3];
  v29 = v7;
  v9 = a3[2];
  v35 = v8;
  v30 = 256 - a4;
  LOWORD(v10) = 0;
  v28 = v9;
  v26 = 0LL;
  v31 = 0;
  v36 = (__m128i *)v37;
  do
  {
    v34 = (unsigned __int16)v10;
    v11 = _mm_cvtsi32_si128((unsigned __int16)v10);
    v12 = v36 + 1;
    v10 = ++v31;
    *v36 = _mm_unpacklo_epi16(
             _mm_unpacklo_epi16(
               _mm_unpacklo_epi16(v11, _mm_cvtsi32_si128(0)),
               _mm_unpacklo_epi16(v11, _mm_cvtsi32_si128(0))),
             _mm_unpacklo_epi16(
               _mm_unpacklo_epi16(v11, _mm_cvtsi32_si128(0)),
               _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    v36 = v12;
  }
  while ( v10 < 257 );
  v13 = v35;
  if ( v35 )
  {
    v14 = _mm_load_si128(&v26);
    do
    {
      v15 = v28;
      v16 = v33;
      v34 = --v13;
      if ( v28 )
      {
        v17 = (__m128i *)(v32 - v33);
        v36 = (__m128i *)(v32 - v33);
        do
        {
          --v15;
          v18 = (*(__int32 *)((_BYTE *)v17->m128i_i32 + v16) & 0xFF000000) == 0;
          v35 = v15;
          if ( !v18 )
          {
            v19 = v30 * v17->m128i_u8[v16 + 3];
            v20 = ((unsigned int)*(unsigned __int8 *)(v16 + 3) * (0x10000 - v19)) >> 8;
            v21 = v20 + v19;
            v22 = _mm_mullo_epi16(
                    _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)v16), v14),
                    _mm_load_si128((const __m128i *)&v37[(v20 << 8) / (v20 + v19)]));
            si128 = _mm_load_si128((const __m128i *)&v37[(v19 << 8) / (v20 + v19)]);
            v17 = v36;
            v24 = ((v21 & 0xFFFFFF00) << 16) | _mm_cvtsi128_si32(
                                                 _mm_packus_epi16(
                                                   _mm_srli_epi16(
                                                     _mm_add_epi16(
                                                       v22,
                                                       _mm_mullo_epi16(
                                                         _mm_unpacklo_epi8(
                                                           _mm_cvtsi32_si128(*(unsigned __int32 *)((char *)v36->m128i_u32
                                                                                                 + v16)),
                                                           v14),
                                                         si128)),
                                                     8u),
                                                   v14));
            v15 = v35;
            *(_DWORD *)v16 = v24;
          }
          v16 += 4;
        }
        while ( v15 );
        v16 = v33;
        v13 = v34;
      }
      v32 += v29;
      v33 = v27 + v16;
    }
    while ( v13 );
  }
  return sub_4AB245((unsigned int)v39 ^ v38);
}

// ===== sub_40BC10 @ 0x0040BC10..0x0040BC51 =====
int *__usercall sub_40BC10@<eax>(int *result@<eax>, unsigned int a2@<edx>, size_t *a3)
{
  int v3; // esi

  if ( !a2 )
    return (int *)sub_40AF50((size_t)result, a3);
  if ( a2 < 0x100 )
  {
    v3 = result[4];
    if ( v3 )
    {
      if ( (unsigned int)(v3 - 1) <= 1 )
        return (int *)sub_40B4B0(result, (__m128i **)a3, a2);
    }
    else
    {
      return (int *)sub_40BC60(a3, result, a2);
    }
  }
  return result;
}

// ===== sub_40BC60 @ 0x0040BC60..0x0040BD53 =====
_DWORD *__cdecl sub_40BC60(_DWORD *a1, _DWORD *a2, int a3)
{
  _DWORD *v3; // ecx
  _DWORD *result; // eax
  int v5; // ebx
  int v6; // edi
  int v7; // edx
  int v8; // esi
  int v9; // eax
  int v10; // ecx
  _WORD *v11; // edx
  __int16 v12; // ax
  __int16 v13; // cx
  unsigned int v14; // ebx
  unsigned int v15; // ebx
  int v16; // [esp+Ch] [ebp-18h]
  int v17; // [esp+10h] [ebp-14h]
  int j; // [esp+14h] [ebp-10h]
  int i; // [esp+18h] [ebp-Ch]
  int v20; // [esp+20h] [ebp-4h]

  v3 = a2;
  result = a1;
  v5 = *a1;
  v6 = a3;
  v20 = *a2;
  v7 = a2[3];
  v8 = 256 - a3;
  for ( i = *a1; v7; i = v5 )
  {
    v9 = v3[2];
    v16 = --v7;
    if ( v9 )
    {
      v10 = v20 - v5;
      v11 = (_WORD *)(v5 + 2 * v9);
      for ( j = v20 - v5; ; v10 = j )
      {
        v17 = v9 - 1;
        v12 = *(_WORD *)((char *)v11 + v10 - 2);
        v13 = *--v11;
        v14 = v6 * (v13 & 0x3E0) + v8 * (v12 & 0x3E0);
        v6 = a3;
        v8 = 256 - a3;
        v15 = ((a3 * (v13 & 0x1F) + (256 - a3) * (v12 & 0x1Fu)) >> 8)
            + (((a3 * (v13 & 0x7C00) + (256 - a3) * (v12 & 0x7C00u)) >> 8) & 0x7C00)
            + ((v14 >> 8) & 0x3E0);
        v9 = v17;
        *v11 = v15;
        if ( !v17 )
          break;
      }
      v7 = v16;
      v5 = i;
      v3 = a2;
    }
    v5 += a1[1];
    result = (_DWORD *)v3[1];
    v20 += (int)result;
  }
  return result;
}

// ===== sub_40BD60 @ 0x0040BD60..0x0040BDA7 =====
int __usercall sub_40BD60@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, unsigned int a5)
{
  if ( *(_DWORD *)(a3 + 16) != 1 )
    return 10;
  if ( *(_DWORD *)(a2 + 16) != 2 || *(_DWORD *)(a1 + 16) != 2 )
    return 9;
  if ( a5 < 0x100 )
    sub_40BDB0(a1, a4, a5);
  return 0;
}

// ===== sub_40BDB0 @ 0x0040BDB0..0x0040C0EB =====
int __usercall sub_40BDB0@<eax>(unsigned int **a1@<edx>, int *a2@<ecx>, int a3@<ebp>, int *a4, __int16 a5, int a6)
{
  void *v6; // esp
  unsigned int v7; // eax
  __m128i v8; // xmm6
  __m128i v9; // xmm7
  __m128i si128; // xmm3
  unsigned int v11; // eax
  __m128i v12; // xmm0
  __m128i v13; // xmm2
  unsigned int v14; // edx
  unsigned int *v15; // ecx
  int v16; // edi
  __m128i v17; // xmm0
  __m128i v18; // xmm0
  int epi16; // eax
  unsigned int *v21; // [esp-2064h] [ebp-2070h]
  int v22; // [esp-2060h] [ebp-206Ch]
  int v23; // [esp-205Ch] [ebp-2068h]
  int v24; // [esp-2058h] [ebp-2064h]
  unsigned int v25; // [esp-2054h] [ebp-2060h]
  unsigned int v26; // [esp-2050h] [ebp-205Ch]
  int v27; // [esp-204Ch] [ebp-2058h]
  int v28; // [esp-2048h] [ebp-2054h]
  int v29; // [esp-2044h] [ebp-2050h]
  int v30; // [esp-2040h] [ebp-204Ch]
  unsigned int *v31; // [esp-203Ch] [ebp-2048h]
  int v32; // [esp-2038h] [ebp-2044h]
  int v33; // [esp-2034h] [ebp-2040h]
  _BYTE v34[8236]; // [esp-2030h] [ebp-203Ch] BYREF
  unsigned int v35; // [esp-4h] [ebp-10h]
  _DWORD v36[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v36[0] = a3;
  v36[1] = retaddr;
  v6 = alloca(8296);
  v35 = (unsigned int)v36 ^ dword_4FB734;
  v31 = *a1;
  v21 = a1[1];
  v33 = *a2;
  v22 = a2[1];
  v29 = *a4;
  v23 = a4[1];
  v25 = a2[3];
  if ( v25 > a4[3] )
    v25 = a4[3];
  v26 = a2[2];
  if ( v26 > a4[2] )
    v26 = a4[2];
  v7 = 0;
  v24 = 0;
  v28 = 256 - a6;
  v32 = 256;
  v27 = 0;
  do
  {
    v8 = _mm_cvtsi32_si128((unsigned __int16)(16 * (v7 >> 8)));
    *(__m128i *)&v34[v27 + 4112] = _mm_unpacklo_epi16(
                                     _mm_unpacklo_epi16(
                                       _mm_unpacklo_epi16(v8, _mm_cvtsi32_si128(0)),
                                       _mm_unpacklo_epi16(v8, _mm_cvtsi32_si128(0))),
                                     _mm_unpacklo_epi16(
                                       _mm_unpacklo_epi16(v8, _mm_cvtsi32_si128(0)),
                                       _mm_unpacklo_epi16(
                                         _mm_cvtsi32_si128((unsigned __int16)(16 * (256 - a6))),
                                         _mm_cvtsi32_si128(0))));
    v9 = _mm_cvtsi32_si128((unsigned __int16)(16 * v32));
    v7 = v28 + v24;
    *(__m128i *)&v34[v27] = _mm_unpacklo_epi16(
                              _mm_unpacklo_epi16(
                                _mm_unpacklo_epi16(v9, _mm_cvtsi32_si128(0)),
                                _mm_unpacklo_epi16(v9, _mm_cvtsi32_si128(0))),
                              _mm_unpacklo_epi16(
                                _mm_unpacklo_epi16(v9, _mm_cvtsi32_si128(0)),
                                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    v24 += v28;
    --v32;
    v27 += 16;
  }
  while ( v32 >= 0 );
  si128 = _mm_load_si128((const __m128i *)&xmmword_4E42C0);
  v11 = v25;
  v12 = _mm_cvtsi32_si128((__int16)(16 * a5));
  v13 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v12, v12), 0);
  if ( v25 )
  {
    v14 = v26;
    do
    {
      v30 = --v11;
      if ( v14 )
      {
        v15 = v31;
        v16 = v33 - (_DWORD)v31;
        do
        {
          v17 = _mm_mulhi_epi16(
                  _mm_slli_epi16(
                    _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(unsigned int *)((char *)v15 + v16)), (__m128i)0LL),
                    4u),
                  _mm_load_si128((const __m128i *)&v34[16 * HIBYTE(*(unsigned int *)((char *)v15 + v16)) + 4112]));
          v18 = _mm_add_epi16(
                  v17,
                  _mm_mulhi_epi16(
                    _mm_slli_epi16(
                      _mm_sub_epi16(
                        _mm_mulhi_epi16(
                          _mm_slli_epi16(
                            _mm_unpacklo_epi8(
                              _mm_cvtsi32_si128(*(unsigned int *)((char *)v15 + v29 - (_DWORD)v31)),
                              (__m128i)0LL),
                            4u),
                          _mm_load_si128((const __m128i *)&v34[16
                                                             * HIBYTE(*(unsigned int *)((char *)v15 + v29 - (_DWORD)v31))
                                                             + 4112])),
                        v17),
                      4u),
                    v13));
          --v14;
          epi16 = _mm_extract_epi16(v18, 3);
          if ( epi16 )
          {
            v16 = v33 - (_DWORD)v31;
            *v15 = _mm_cvtsi128_si32(
                     _mm_packus_epi16(
                       _mm_add_epi16(
                         _mm_mulhi_epi16(
                           _mm_slli_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(*v15), (__m128i)0LL), 4u),
                           _mm_load_si128((const __m128i *)&v34[16 * epi16])),
                         _mm_and_si128(v18, si128)),
                       (__m128i)0LL));
          }
          ++v15;
        }
        while ( v14 );
        v14 = v26;
        v11 = v30;
      }
      v31 = (unsigned int *)((char *)v31 + (_DWORD)v21);
      v33 += v22;
      v29 += v23;
    }
    while ( v11 );
  }
  return sub_4AB245((unsigned int)v36 ^ v35);
}
