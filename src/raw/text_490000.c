#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4900A0 @ 0x004900A0..0x00490109 =====
int __stdcall sub_4900A0(int a1)
{
  int v1; // ecx

  sub_490360();
  v1 = *(_DWORD *)(a1 + 20);
  if ( v1 )
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v1 + 20))(v1, 1);
  *(_DWORD *)(a1 + 20) = 0;
  return sub_490800();
}

// ===== sub_490110 @ 0x00490110..0x0049017D =====
void *__fastcall sub_490110(int a1, _DWORD *a2, void *a3, const void *a4)
{
  int v4; // esi
  int v5; // edi
  int v6; // ebx
  int v7; // ecx
  int v8; // eax
  unsigned int v9; // ecx
  void *result; // eax

  qmemcpy(a3, a2 + 7, 0x40u);
  qmemcpy(a2 + 7, a4, 0x40u);
  v4 = a2[13];
  v5 = a2[14];
  v6 = a2[15];
  v7 = a2[16] >> 8;
  a2[23] = v7 * v4;
  a2[24] = v7 * v5;
  v8 = v7 * v6;
  v9 = a2[17];
  a2[25] = v8;
  v9 >>= 8;
  a2[26] = v4 * v9;
  a2[27] = v5 * v9;
  result = a3;
  a2[28] = v6 * v9;
  return result;
}

// ===== sub_490180 @ 0x00490180..0x00490287 =====
int *__stdcall sub_490180(int *a1, int *a2, const void *a3)
{
  double v3; // st7
  double v4; // st7
  long double v6; // [esp+10h] [ebp-8h]
  long double v7; // [esp+10h] [ebp-8h]
  long double v8; // [esp+10h] [ebp-8h]

  qmemcpy(a2, a1 + 29, 0x1Cu);
  qmemcpy(a1 + 29, a3, 0x1Cu);
  v6 = (double)a1[32] * 0.01745329251944444 / 10.0;
  a1[36] = (int)(sin(v6) * 256.0);
  v3 = (double)a1[33];
  a1[37] = (int)(cos(v6) * 256.0);
  v7 = v3 * 0.01745329251944444 / 10.0;
  a1[38] = (int)(sin(v7) * 256.0);
  v4 = (double)a1[34];
  a1[39] = (int)(cos(v7) * 256.0);
  v8 = v4 * 0.01745329251944444 / 10.0;
  a1[40] = (int)(sin(v8) * 256.0);
  a1[41] = (int)(cos(v8) * 256.0);
  return a2;
}

// ===== sub_490290 @ 0x00490290..0x004902AB =====
int __userpurge sub_490290@<eax>(int a1@<esi>, int a2)
{
  *(_DWORD *)(a1 + 24) = sub_490340() - a2;
  return sub_490360();
}

// ===== sub_4902B0 @ 0x004902B0..0x004902FA =====
int __usercall sub_4902B0@<eax>(int a1@<eax>)
{
  int v2; // ebx
  unsigned int v3; // edi

  v2 = sub_490340();
  v3 = v2 - *(_DWORD *)(a1 + 24);
  if ( v3 >= 0x1F4 )
  {
    *(_DWORD *)(a1 + 24) = sub_490340();
    return -1;
  }
  if ( v3 < *(_DWORD *)(a1 + 84) )
    return -1;
  do
  {
    sub_490390();
    v3 -= *(_DWORD *)(a1 + 84);
  }
  while ( v3 >= *(_DWORD *)(a1 + 84) );
  *(_DWORD *)(a1 + 24) = v2 - v3;
  return -1;
}

// ===== sub_490300 @ 0x00490300..0x0049033F =====
int __userpurge sub_490300@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5)
{
  int result; // eax
  int v6; // eax

  sub_490520(a4, a2, a1);
  for ( result = sub_490980(a3); result; result = sub_490990() )
  {
    v6 = sub_42D560(result);
    sub_4905C0(v6);
  }
  return result;
}

// ===== sub_490340 @ 0x00490340..0x00490352 =====
DWORD __usercall sub_490340@<eax>(int a1@<eax>)
{
  if ( *(_DWORD *)(a1 + 88) )
    return timeGetTime();
  else
    return GetTickCount();
}

// ===== sub_490360 @ 0x00490360..0x0049038E =====
int __usercall sub_490360@<eax>(int a1@<eax>)
{
  int i; // eax
  void *v2; // eax

  for ( i = sub_490980(a1); i; i = sub_490990() )
  {
    v2 = (void *)sub_42D560(i);
    operator delete(v2);
  }
  return sub_490920();
}

// ===== sub_490390 @ 0x00490390..0x0049039E =====
int __usercall sub_490390@<eax>(int a1@<esi>)
{
  sub_4903A0();
  return sub_490430(a1);
}

// ===== sub_4903A0 @ 0x004903A0..0x004903E7 =====
int __usercall sub_4903A0@<eax>(int a1@<eax>)
{
  int result; // eax
  void *v3; // edi

  result = sub_490980(a1);
  while ( result )
  {
    v3 = (void *)sub_42D560(result);
    if ( sub_4903F0(a1) )
    {
      operator delete(v3);
      sub_4908B0();
      result = sub_42D560(a1);
    }
    else
    {
      result = sub_490990();
    }
  }
  return result;
}

// ===== sub_4903F0 @ 0x004903F0..0x00490422 =====
BOOL __usercall sub_4903F0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>)
{
  a1[1] += a2[23];
  a1[2] += a2[24];
  a1[3] += a2[25];
  a1[4] += a2[23];
  a1[5] += a2[24];
  a1[6] += a2[25];
  return a1[2] <= a2[11];
}

// ===== sub_490430 @ 0x00490430..0x00490517 =====
int __thiscall sub_490430(_DWORD *this)
{
  int v2; // ebx
  int result; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // edi
  int v6; // edx
  int v7; // eax
  unsigned int i; // [esp+14h] [ebp-14h]
  unsigned int v9; // [esp+18h] [ebp-10h]

  v2 = 0;
  v9 = this[10] - this[7];
  result = this[12] - this[9];
  for ( i = result; v2 < this[20]; ++v2 )
  {
    v4 = operator new(0x1Cu);
    v5 = 0;
    if ( v4 )
    {
      *v4 = 1;
      v5 = v4;
    }
    v5[4] = this[7] + rand() % v9;
    v5[5] = this[8] - rand() % 100;
    v5[6] = this[9] + rand() % i;
    v6 = v5[5];
    v7 = v5[6];
    v5[1] = v5[4];
    v5[2] = v6;
    v5[3] = v7;
    v5[1] -= this[26];
    v5[2] -= this[27];
    v5[3] -= this[28];
    result = sub_490810(v5);
  }
  return result;
}

// ===== sub_490520 @ 0x00490520..0x004905B9 =====
int __userpurge sub_490520@<eax>(unsigned int a1@<edi>, int *a2@<esi>, int a3, unsigned int a4, int a5)
{
  _DWORD *v5; // eax
  _DWORD *v6; // eax

  if ( !a2[5] )
  {
    v5 = operator new(0x2Cu);
    if ( v5 )
      v6 = sub_48FA90(v5);
    else
      v6 = 0;
    a2[5] = (int)v6;
  }
  sub_48FAF0(a3, a2[5], a1, a4, a5);
  sub_48FB60(&a5, a2[5], a2[18]);
  sub_48FB80(&a5, a2[5], a2[19]);
  return a2[5];
}

// ===== sub_4905C0 @ 0x004905C0..0x004906F1 =====
int __userpurge sub_4905C0@<eax>(int a1@<ecx>, int a2@<edi>, int a3)
{
  int v5; // ecx
  int v6; // ebx
  int v7; // esi
  int v8; // eax
  unsigned int v9; // [esp+8h] [ebp-2Ch]
  unsigned int v10; // [esp+Ch] [ebp-28h]
  unsigned int v11; // [esp+10h] [ebp-24h]
  int v12; // [esp+14h] [ebp-20h] BYREF
  int v13; // [esp+18h] [ebp-1Ch]
  int v14; // [esp+1Ch] [ebp-18h]
  int v15; // [esp+20h] [ebp-14h]
  int v16; // [esp+24h] [ebp-10h]
  int v17; // [esp+28h] [ebp-Ch]
  int v18; // [esp+2Ch] [ebp-8h]

  v9 = (unsigned int)(*(int (__thiscall **)(int))(*(_DWORD *)a2 + 12))(a2) >> 1;
  v10 = (unsigned int)(*(int (__thiscall **)(int))(*(_DWORD *)a2 + 16))(a2) >> 1;
  sub_490700(a1);
  if ( !sub_4907D0() )
    return 0;
  sub_490700(a1);
  if ( !sub_4907D0() )
    return 0;
  v5 = *(_DWORD *)(a1 + 140);
  v16 = v16 * v5 / v18;
  v17 = v17 * v5 / v18;
  v13 = v13 * v5 / v15;
  v6 = v14 * v5 / v15;
  v11 = *(_DWORD *)(a1 + 72);
  v7 = HIBYTE(v11) * *(_DWORD *)(a1 + 140);
  v12 = v18 / 8;
  v8 = v7 / (v18 / 8);
  if ( v8 > (int)HIBYTE(v11) )
    LOBYTE(v8) = HIBYTE(v11);
  HIBYTE(v11) = v8;
  sub_48FB60(&v12, a2, v11);
  (*(void (__thiscall **)(int, unsigned int, unsigned int, unsigned int, unsigned int))(*(_DWORD *)a2 + 8))(
    a2,
    v9 + v16,
    v10 - v17,
    v9 + v13,
    v10 - v6);
  return 1;
}

// ===== sub_490700 @ 0x00490700..0x004907CC =====
int __userpurge sub_490700@<eax>(int *a1@<eax>, int *a2@<ecx>, _DWORD *a3)
{
  int v4; // edi
  int v5; // esi
  int v6; // ebx
  int v7; // ecx
  int v8; // edi
  int v9; // ebx
  int v10; // esi
  int v11; // ecx
  int v12; // edi
  int v13; // edx
  int v15; // [esp+14h] [ebp+8h]

  *a1 = *a2;
  a1[1] = a2[1];
  a1[2] = a2[2];
  *a1 -= a3[29];
  a1[1] -= a3[30];
  a1[2] -= a3[31];
  v4 = a1[1];
  v5 = (*a1 * a3[41] - v4 * a3[40]) >> 8;
  *a1 = v5;
  v6 = a1[2];
  v7 = (v4 * a3[41] + v5 * a3[40]) >> 8;
  a1[1] = v7;
  v8 = a3[38];
  v9 = v8 * v6;
  v10 = v7;
  v11 = a3[39] * a1[2] + v8 * v7;
  v12 = *a1;
  v11 >>= 8;
  a1[1] = (a3[39] * v10 - v9) >> 8;
  a1[2] = v11;
  v13 = a3[36];
  v15 = a3[37];
  a1[2] = (v15 * v11 - v13 * v12) >> 8;
  *a1 = (v15 * v12 + v13 * v11) >> 8;
  return 0;
}

// ===== sub_4907D0 @ 0x004907D0..0x004907DF =====
BOOL __usercall sub_4907D0@<eax>(int a1@<eax>, int a2@<edx>)
{
  return *(_DWORD *)(a1 + 8) > *(_DWORD *)(a2 + 140);
}

// ===== sub_4907E0 @ 0x004907E0..0x004907F5 =====
_DWORD *__usercall sub_4907E0@<eax>(_DWORD *result@<eax>)
{
  *result = 0;
  result[1] = 0;
  result[2] = 0;
  result[3] = -1;
  result[4] = 0;
  return result;
}

// ===== sub_490800 @ 0x00490800..0x00490805 =====
// attributes: thunk
int sub_490800(void)
{
  return sub_490920();
}

// ===== sub_490810 @ 0x00490810..0x004908A1 =====
int __userpurge sub_490810@<eax>(int *a1@<esi>, int a2)
{
  int v2; // edi
  int v3; // eax
  int result; // eax

  if ( operator new(0xCu) )
    v2 = sub_490A70();
  else
    v2 = 0;
  sub_490A80(v2, a2);
  if ( a1[4] )
    sub_490A90(a1[1], v2);
  else
    *a1 = v2;
  v3 = a1[4];
  a1[3] = v3;
  a1[4] = v3 + 1;
  result = a1[3];
  a1[1] = v2;
  a1[2] = v2;
  return result;
}

// ===== sub_4908B0 @ 0x004908B0..0x00490915 =====
int __usercall sub_4908B0@<eax>(_DWORD *a1@<eax>, int a2@<ecx>)
{
  int v3; // edi
  int result; // eax
  int v5; // ebx
  void *v6; // edi

  v3 = a1[2];
  if ( !v3 )
    return -1;
  if ( v3 == a1[1] )
  {
    sub_490A10();
    return a1[4];
  }
  else if ( v3 == *a1 )
  {
    sub_4909B0();
    return a1[4];
  }
  else
  {
    v5 = sub_490AE0(a2);
    sub_490AB0();
    v6 = (void *)a1[2];
    if ( v6 )
    {
      sub_490A70();
      operator delete(v6);
    }
    result = --a1[4];
    a1[2] = v5;
  }
  return result;
}

// ===== sub_490920 @ 0x00490920..0x0049097E =====
int __usercall sub_490920@<eax>(int a1@<ecx>, _DWORD *a2@<esi>)
{
  int result; // eax
  void *v3; // edi
  int v4; // ebx
  int v5; // [esp+0h] [ebp-4h]

  a2[2] = *a2;
  result = 0;
  v5 = 0;
  if ( (int)a2[4] > 0 )
  {
    do
    {
      v3 = (void *)a2[2];
      v4 = sub_490AE0(a1);
      if ( v3 )
      {
        sub_490A70();
        operator delete(v3);
      }
      a2[2] = v4;
      ++v5;
    }
    while ( v5 < a2[4] );
    result = 0;
  }
  a2[2] = 0;
  *a2 = 0;
  a2[1] = 0;
  a2[4] = 0;
  a2[3] = -1;
  return result;
}

// ===== sub_490980 @ 0x00490980..0x0049098D =====
int __thiscall sub_490980(int *this)
{
  int result; // eax

  result = *this;
  this[2] = *this;
  this[3] = 0;
  return result;
}

// ===== sub_490990 @ 0x00490990..0x004909AA =====
int __usercall sub_490990@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int result; // eax

  if ( !*(_DWORD *)(a2 + 8) )
    return 0;
  result = sub_490AE0(a1);
  if ( !result )
    return 0;
  ++*(_DWORD *)(a2 + 12);
  *(_DWORD *)(a2 + 8) = result;
  return result;
}

// ===== sub_4909B0 @ 0x004909B0..0x00490A06 =====
int __usercall sub_4909B0@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int result; // eax
  int v3; // ebx
  void *v4; // edi

  if ( !*(_DWORD *)(a2 + 16) )
    return 0;
  v3 = sub_490AE0(a1);
  sub_490AB0();
  v4 = *(void **)a2;
  if ( *(_DWORD *)a2 )
  {
    sub_490A70();
    operator delete(v4);
  }
  if ( *(_DWORD *)a2 == *(_DWORD *)(a2 + 4) )
    *(_DWORD *)(a2 + 4) = 0;
  result = --*(_DWORD *)(a2 + 16);
  *(_DWORD *)a2 = v3;
  *(_DWORD *)(a2 + 8) = v3;
  *(_DWORD *)(a2 + 12) = 0;
  return result;
}

// ===== sub_490A10 @ 0x00490A10..0x00490A66 =====
int __usercall sub_490A10@<eax>(_DWORD *a1@<esi>)
{
  int result; // eax
  int v2; // ebx
  void *v3; // edi

  result = a1[4];
  if ( result )
  {
    a1[4] = result - 1;
    v2 = sub_407760(a1[1]);
    sub_490AB0();
    v3 = (void *)a1[1];
    if ( v3 )
    {
      sub_490A70();
      operator delete(v3);
    }
    if ( *a1 == a1[1] )
      *a1 = 0;
    result = a1[4];
    a1[1] = v2;
    a1[2] = v2;
    a1[3] = result - 1;
  }
  return result;
}

// ===== sub_490A70 @ 0x00490A70..0x00490A7E =====
_DWORD *__usercall sub_490A70@<eax>(_DWORD *result@<eax>)
{
  *result = 0;
  result[1] = 0;
  return result;
}

// ===== sub_490A80 @ 0x00490A80..0x00490A87 =====
int __fastcall sub_490A80(int a1, int a2)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 8);
  *(_DWORD *)(a1 + 8) = a2;
  return result;
}

// ===== sub_490A90 @ 0x00490A90..0x00490AAA =====
int __fastcall sub_490A90(int a1, _DWORD *a2)
{
  int result; // eax
  _DWORD *v3; // esi

  result = *(_DWORD *)(a1 + 4);
  *a2 = a1;
  a2[1] = *(_DWORD *)(a1 + 4);
  v3 = *(_DWORD **)(a1 + 4);
  if ( v3 )
    *v3 = a2;
  *(_DWORD *)(a1 + 4) = a2;
  return result;
}

// ===== sub_490AB0 @ 0x00490AB0..0x00490AD1 =====
_DWORD *__usercall sub_490AB0@<eax>(_DWORD *result@<eax>)
{
  _DWORD *v1; // ecx

  v1 = (_DWORD *)result[1];
  if ( v1 )
    *v1 = *result;
  if ( *result )
    *(_DWORD *)(*result + 4) = result[1];
  *result = 0;
  result[1] = 0;
  return result;
}

// ===== sub_490AE0 @ 0x00490AE0..0x00490AE4 =====
int __usercall sub_490AE0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 4);
}

// ===== sub_490AF0 @ 0x00490AF0..0x00490AF5 =====
// attributes: thunk
int sub_490AF0()
{
  return sub_44DDB0();
}

// ===== sub_490B00 @ 0x00490B00..0x00490B05 =====
// attributes: thunk
void *sub_490B00()
{
  return sub_44DDD0();
}

// ===== sub_490B10 @ 0x00490B10..0x00490B15 =====
// attributes: thunk
int sub_490B10()
{
  return sub_4410B0();
}

// ===== sub_490B20 @ 0x00490B20..0x00490B47 =====
_DWORD *sub_490B20()
{
  _DWORD *result; // eax

  for ( result = dword_566984; dword_566984; result = dword_566984 )
    sub_490C30(*result, 0);
  return result;
}

// ===== sub_490B50 @ 0x00490B50..0x00490C22 =====
void sub_490B50()
{
  int v0; // eax
  int *v1; // esi
  int *v2; // ebx
  int v3; // eax
  int v4; // edi
  unsigned int v5; // [esp+8h] [ebp-Ch]
  int v6; // [esp+Ch] [ebp-8h]
  unsigned int v7; // [esp+10h] [ebp-4h]

  v7 = sub_498720();
  v0 = sub_443160((int)dword_56674C);
  v1 = (int *)dword_566984;
  v5 = v0;
  v6 = 0;
  v2 = (int *)&unk_566978;
  if ( dword_566984 )
  {
    do
    {
      if ( v1[2] <= v7 )
      {
        v3 = sub_441240(*v1, (int)dword_56674C);
        v4 = v3;
        if ( v3 )
        {
          if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3)
            && v5 <= (*(int (__thiscall **)(int))(*(_DWORD *)v4 + 28))(v4) )
          {
            sub_490D40();
            v6 = 1;
          }
          if ( !v1[2] )
            v1[2] = v7;
          v1[2] += v1[1] * ((v7 - v1[2]) / v1[1] + 1);
        }
        else
        {
          v2[3] = v1[3];
          operator delete(v1);
          v1 = v2;
        }
      }
      v2 = v1;
      v1 = (int *)v1[3];
    }
    while ( v1 );
    if ( v6 )
      sub_461D80();
  }
}

// ===== sub_490C30 @ 0x00490C30..0x00490CCE =====
BOOL __cdecl sub_490C30(int a1, int a2)
{
  int *v2; // esi
  int *v3; // edi
  int v5; // [esp+4h] [ebp-4h]

  v5 = sub_441240(a1, (int)dword_56674C);
  if ( !v5 )
    return v5 != 0;
  v2 = (int *)dword_566984;
  v3 = (int *)&unk_566978;
  if ( dword_566984 )
  {
    do
    {
      if ( a1 == *v2 )
        break;
      v3 = v2;
      v2 = (int *)v2[3];
    }
    while ( v2 );
  }
  if ( !a2 )
  {
    if ( v2 )
    {
      v3[3] = v2[3];
      operator delete(v2);
    }
    return v5 != 0;
  }
  if ( !v2 )
  {
    v2 = (int *)operator new(0x10u);
    *v2 = a1;
    v2[2] = sub_498720();
    v2[3] = 0;
    v3[3] = (int)v2;
  }
  v2[1] = a2;
  return v5 != 0;
}

// ===== sub_490CD0 @ 0x00490CD0..0x00490CE0 =====
int __usercall sub_490CD0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, _DWORD *a3)
{
  return sub_441130(a3, a2, a1);
}

// ===== sub_490CE0 @ 0x00490CE0..0x00490CE7 =====
BOOL __usercall sub_490CE0@<eax>(int a1@<eax>)
{
  return sub_441280(a1);
}

// ===== sub_490CF0 @ 0x00490CF0..0x00490CF8 =====
BOOL __usercall sub_490CF0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441300(a2, a1);
}

// ===== sub_490D00 @ 0x00490D00..0x00490D1B =====
BOOL __usercall sub_490D00@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_441370(a3, a4, a5, a6, a2, a1);
}

// ===== sub_490D20 @ 0x00490D20..0x00490D33 =====
int __usercall sub_490D20@<eax>(int *a1@<eax>, int *a2@<ecx>, int a3, unsigned int a4)
{
  return sub_4413E0(a3, a4, a2, a1);
}

// ===== sub_490D40 @ 0x00490D40..0x00490D47 =====
BOOL __usercall sub_490D40@<eax>(int a1@<eax>)
{
  return sub_441480(a1);
}

// ===== sub_490D50 @ 0x00490D50..0x00490D58 =====
BOOL __usercall sub_490D50@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4414B0(a2, a1);
}

// ===== sub_490D60 @ 0x00490D60..0x00490D8C =====
int __usercall sub_490D60@<eax>(
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
  return sub_4414E0(a3, a4, a5, a6, a7, a8, a9, a10, a2, a1);
}

// ===== sub_490D90 @ 0x00490D90..0x00490D98 =====
int __usercall sub_490D90@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  return sub_441540(a2, a1);
}

// ===== sub_490DA0 @ 0x00490DA0..0x00490DA8 =====
BOOL __usercall sub_490DA0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441580(a2, a1);
}

// ===== sub_490DB0 @ 0x00490DB0..0x00490DB7 =====
BOOL __usercall sub_490DB0@<eax>(int a1@<eax>)
{
  return sub_4415B0(a1);
}

// ===== sub_490DC0 @ 0x00490DC0..0x00490DF7 =====
BOOL __usercall sub_490DC0@<eax>(
        int a1@<edx>,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  return sub_4415E0(a2, a3, a4, a5, a6, a7, a8, a9, a10, a1, a11, a12);
}

// ===== sub_490E00 @ 0x00490E00..0x00490F51 =====
int __cdecl sub_490E00(unsigned int a1, int a2, unsigned int a3, int a4, unsigned int a5, int a6, int a7)
{
  int v7; // edi
  int v8; // ebx
  unsigned int v9; // edi
  _DWORD *v10; // esi
  int v11; // eax
  int v12; // esi
  int v13; // esi
  _DWORD *v15; // [esp+Ch] [ebp-4h]

  v7 = a3;
  v15 = 0;
  v8 = 1;
  if ( !a3 || a4 == -1 )
  {
LABEL_8:
    if ( dword_566988 )
    {
      if ( dword_566988 == 1 )
      {
        if ( a5 )
          v12 = (a7 << 16) / a5;
        else
          v12 = 0;
      }
      else if ( dword_566988 == 2 )
      {
        if ( a5 )
          v12 = (a7 << 16) / a5;
        else
          v12 = 0;
      }
      else
      {
        v12 = a3;
      }
    }
    else if ( a5 )
    {
      v12 = (a7 << 16) / a5;
    }
    else
    {
      v12 = 0;
    }
    v13 = sub_44DDE0(v12, v7, (int)v15, a1);
  }
  else
  {
    v9 = 0;
    v15 = operator new[](24 * a3);
    v10 = v15;
    while ( v8 )
    {
      v11 = sub_407F20((int)dword_566750, v9 + a4, v10);
      ++v9;
      v10 += 6;
      v8 = v11;
      if ( v9 >= a3 )
      {
        if ( v11 )
        {
          v7 = a3;
          goto LABEL_8;
        }
        break;
      }
    }
    v13 = -2147483644;
  }
  operator delete[](v15);
  return v13;
}

// ===== sub_490F60 @ 0x00490F60..0x004910B8 =====
int __cdecl sub_490F60(int a1, int a2, unsigned int a3, int a4, int a5, int a6, int a7, int a8)
{
  int v8; // ebx
  unsigned int v9; // edi
  int v10; // esi
  _DWORD *v11; // ebx
  int v12; // eax
  int v13; // edi
  _DWORD *v15; // [esp+Ch] [ebp-4h]

  v8 = a3;
  v9 = 0;
  v15 = 0;
  v10 = 1;
  if ( !a3 || a4 == -1 )
  {
LABEL_8:
    v13 = sub_44DE60(a8, (int)v15, a1, v8);
    if ( !v13 )
      sub_441780(a1, a2);
  }
  else
  {
    v15 = operator new[](24 * a3);
    v11 = v15;
    while ( v10 )
    {
      v12 = sub_407F20((int)dword_566750, v9 + a4, v11);
      ++v9;
      v11 += 6;
      v10 = v12;
      if ( v9 >= a3 )
      {
        if ( v12 )
        {
          v8 = a3;
          goto LABEL_8;
        }
        break;
      }
    }
    v13 = -2147483644;
  }
  operator delete[](v15);
  return v13;
}

// ===== sub_4910C0 @ 0x004910C0..0x004910D1 =====
int __usercall sub_4910C0@<eax>(int a1@<eax>, int a2)
{
  return sub_44DEC0(a2, a1);
}

// ===== sub_4910E0 @ 0x004910E0..0x004910F3 =====
int __fastcall sub_4910E0(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 2 )
  {
    dword_566988 = a1;
    return 1;
  }
  return result;
}

// ===== sub_491100 @ 0x00491100..0x00491114 =====
int __usercall sub_491100@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_441640(a3, a4, a2, a1);
}

// ===== sub_491120 @ 0x00491120..0x00491151 =====
int __fastcall sub_491120(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
  return sub_44DF10(a10, a9, a3, a4, a5, a6, a7, a8, a2, a1, a11);
}

// ===== sub_491160 @ 0x00491160..0x00491174 =====
int __usercall sub_491160@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_4416A0(a3, a4, a2, a1);
}

// ===== sub_491180 @ 0x00491180..0x004911C6 =====
int __cdecl sub_491180(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15)
{
  return sub_441700(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

// ===== sub_4911D0 @ 0x004911D0..0x004911D9 =====
int __cdecl sub_4911D0(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15,
        int a16,
        int a17,
        int a18)
{
  return sub_44DF50(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
}

// ===== sub_4911E0 @ 0x004911E0..0x0049121A =====
int __usercall sub_4911E0@<eax>(
        int a1@<eax>,
        int a2@<edx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  return sub_491220(a3, a4, a5, a1, a10, a1, a11, a6, 0, a7, a8, a9, a2, a12);
}

// ===== sub_491220 @ 0x00491220..0x00491426 =====
int __cdecl sub_491220(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14)
{
  int v14; // eax
  int v15; // esi
  _DWORD *v16; // eax
  _DWORD *v17; // edi
  void *v18; // eax
  _DWORD *v19; // eax
  _DWORD *v20; // eax
  void *v21; // eax
  int v22; // esi
  int v23; // eax
  int v24; // eax
  int v25; // eax
  _DWORD *v27; // [esp+10h] [ebp-10h]

  v14 = sub_4406C0(a2, (int)dword_56674C);
  v15 = v14;
  if ( !v14 )
    return -1;
  if ( sub_42C450(v14) )
  {
    if ( !a14 )
    {
      v21 = operator new(0xF8u);
      if ( v21 )
        v22 = sub_4341D0(a1, v15, (int)v21);
      else
        v22 = 0;
      v17 = (_DWORD *)v22;
      (*(void (__thiscall **)(int, int))(*(_DWORD *)v22 + 32))(v22, a4);
      sub_434DA0(v22, a3, a5, a7, a8);
      goto LABEL_22;
    }
    if ( a14 != 1 )
    {
      if ( a14 == -1 )
      {
        v16 = operator new(0x7Cu);
        if ( v16 )
          v17 = sub_432BC0(a1, v15, v16);
        else
          v17 = 0;
        (*(void (__thiscall **)(_DWORD *, int))(*v17 + 32))(v17, a4);
        (*(void (__thiscall **)(_DWORD *, int))(*v17 + 28))(v17, a3);
      }
      else
      {
        v17 = v27;
      }
      goto LABEL_22;
    }
    if ( sub_42C570(v15) == 1 )
    {
      v20 = operator new(0xF8u);
      if ( v20 )
      {
        v19 = sub_438190(a1, v15, v20);
        goto LABEL_16;
      }
    }
    else
    {
      v18 = operator new(0xF8u);
      if ( v18 )
      {
        v19 = (_DWORD *)sub_4341D0(a1, v15, (int)v18);
LABEL_16:
        v17 = v19;
        sub_434DE0(a4, v19, a6);
        sub_434E00(a9, (int)v17);
        sub_434DA0((int)v17, a3, a5, a7, 0);
LABEL_22:
        v23 = sub_4335D0((int)v17, a10);
        v24 = sub_44DBA0(v23, a11);
        v25 = sub_4335E0(v24, a12);
        sub_4335F0(v25, a13);
        sub_4451C0(a1, (int)v17);
        return 0;
      }
    }
    v19 = 0;
    goto LABEL_16;
  }
  return -2147483647;
}

// ===== sub_491430 @ 0x00491430..0x00491462 =====
unsigned int __usercall sub_491430@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3)
{
  if ( (unsigned int)(a1 - 1) > 0xF )
    return -2147483647;
  if ( a2 < 0 || a2 >= a1 )
    return -2147483646;
  return a3 > 0x10 ? 0x80000003 : 0;
}

// ===== sub_491470 @ 0x00491470..0x00491587 =====
unsigned int __cdecl sub_491470(int a1, int a2, char *a3, unsigned int a4, int a5, int a6, int a7, int a8)
{
  void *v8; // ecx
  void *v9; // esi
  unsigned int result; // eax
  int v11; // eax
  int v12; // edi
  _DWORD *v13; // eax
  _DWORD *v14; // edi
  int v15; // eax
  _BYTE v16[64]; // [esp+10h] [ebp-50h] BYREF
  void *v17; // [esp+50h] [ebp-10h]
  int v18; // [esp+5Ch] [ebp-4h]

  v9 = v8;
  result = sub_491430(a2, a7, a4);
  if ( !result )
  {
    sub_48DF70(a3, (int)v16, (int)v9, a2);
    v11 = sub_4406C0(a1, (int)dword_56674C);
    v12 = v11;
    if ( v11 )
    {
      if ( sub_42C450(v11) )
      {
        v13 = operator new(0x194u);
        v17 = v13;
        v18 = 0;
        if ( v13 )
          v14 = sub_43B380(v9, v13, v12);
        else
          v14 = 0;
        v18 = -1;
        sub_43B5E0(a4, v14, a2, (int)v16, a5, a6);
        v15 = sub_43B8C0((int)v14, a7);
        sub_43B8D0(v15, a8);
        sub_4451C0((int)v9, (int)v14);
        return 0;
      }
      else
      {
        return -2147483644;
      }
    }
    else
    {
      return -1;
    }
  }
  return result;
}

// ===== sub_491590 @ 0x00491590..0x00491665 =====
unsigned int __usercall sub_491590@<eax>(int a1@<ecx>, int a2@<edi>, char *a3, int a4, unsigned int a5, int a6, int a7)
{
  unsigned int result; // eax
  _DWORD *v9; // esi
  int v10; // eax
  const char *v11[16]; // [esp+10h] [ebp-148h] BYREF
  char v12[260]; // [esp+50h] [ebp-108h] BYREF

  result = sub_491430(a2, 0, a5);
  if ( !result )
  {
    sub_48DF70(a3, (int)v11, a4, a2);
    v9 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
    if ( v9 )
    {
      sub_43B670(v9, v11, v12, a2, a5, a6, a7);
      v10 = sub_42B550((int)v9, 0);
      sub_42B540(v10, 1);
      sub_42CA00((int)v9);
      sub_42BBB0((int)v9);
      return 0;
    }
    else
    {
      return -1;
    }
  }
  return result;
}

// ===== sub_491670 @ 0x00491670..0x00491859 =====
unsigned int __cdecl sub_491670(
        void *a1,
        int a2,
        int a3,
        char *a4,
        unsigned int a5,
        int a6,
        int a7,
        int a8,
        void *a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15)
{
  int v15; // ecx
  int v16; // esi
  unsigned int result; // eax
  int v18; // eax
  int v19; // edi
  _DWORD *v20; // eax
  int *v21; // eax
  _DWORD *v22; // eax
  int *v23; // edi
  int v24; // eax
  int v25; // ebx
  _BYTE v26[64]; // [esp+10h] [ebp-50h] BYREF
  void *v27; // [esp+50h] [ebp-10h]
  int v28; // [esp+5Ch] [ebp-4h]

  v16 = v15;
  result = sub_491430(a3, v15, a5);
  if ( result )
    return result;
  sub_48DF70(a4, (int)v26, (int)a1, a3);
  v18 = sub_4406C0(a2, (int)dword_56674C);
  v19 = v18;
  if ( !v18 )
    return -1;
  if ( !sub_42C450(v18) )
    return -2147483644;
  if ( a15 )
  {
    v20 = operator new(0x1ACu);
    v27 = v20;
    v28 = 0;
    if ( v20 )
    {
      v21 = sub_43C270(a1, v20, v19);
      goto LABEL_10;
    }
  }
  else
  {
    v22 = operator new(0x1B0u);
    v27 = v22;
    v28 = 1;
    if ( v22 )
    {
      v21 = sub_43C650(a1, v22, v19);
      goto LABEL_10;
    }
  }
  v21 = 0;
LABEL_10:
  v28 = -1;
  v23 = v21;
  sub_43B5E0(a5, v21, a3, (int)v26, a6, a7);
  v24 = sub_43B8C0((int)v23, v16);
  sub_43B8D0(v24, a8);
  v25 = sub_43C3C0(a9, v23, a10, a11, a12, a13, a14);
  if ( v25 )
  {
    if ( v23 )
      (*(void (__thiscall **)(int *, int))*v23)(v23, 1);
    switch ( v25 )
    {
      case 32769:
        result = -2147483643;
        break;
      case 32770:
        result = -2147483642;
        break;
      case 32771:
        result = -2147483641;
        break;
      case 32772:
        result = -2147483640;
        break;
      default:
        goto LABEL_18;
    }
  }
  else
  {
LABEL_18:
    sub_4451C0((int)a1, (int)v23);
    return 0;
  }
  return result;
}

// ===== sub_491870 @ 0x00491870..0x00491998 =====
int __cdecl sub_491870(void *a1, int a2, int a3, _DWORD *a4, int a5, int a6, int a7)
{
  unsigned int v7; // ecx
  int v8; // esi
  int v10; // edi
  _DWORD *v11; // eax
  int *v12; // edi

  v8 = v7;
  if ( (unsigned int)(a3 - 1) > 0x3F )
    return -2147483647;
  if ( v7 > 3 )
    return -2147483646;
  v10 = sub_4406C0(a2, (int)dword_56674C);
  if ( !v10 )
    return -1;
  v11 = operator new(0x658u);
  if ( v11 )
    v12 = sub_43A150(a1, v11, v10, v8);
  else
    v12 = 0;
  sub_43A820((int)v12, a6);
  if ( sub_43A400(v12, a3, a4, a7) )
    return -2147483645;
  if ( a5 )
    sub_43A7B0((int)v12, a5);
  sub_4451C0((int)a1, (int)v12);
  return 0;
}

// ===== sub_4919A0 @ 0x004919A0..0x00491ACA =====
int __cdecl sub_4919A0(void *a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  unsigned int v7; // ecx
  int v8; // esi
  void *v10; // edi
  _DWORD *v11; // eax
  int *v12; // edi

  v8 = v7;
  if ( (unsigned int)(a3 - 1) > 0x3F )
    return -2147483647;
  if ( v7 > 3 )
    return -2147483646;
  v10 = (void *)sub_4406C0(a2, (int)dword_56674C);
  if ( !v10 )
    return -1;
  v11 = operator new(0x1658u);
  if ( v11 )
    v12 = sub_43ADF0(v10, v11, a1, v8);
  else
    v12 = 0;
  sub_43A820((int)v12, a6);
  if ( sub_43AED0(v12, a3, a4, a7) )
    return -2147483645;
  if ( a5 )
    sub_43A7B0((int)v12, a5);
  sub_4451C0((int)a1, (int)v12);
  return 0;
}

// ===== sub_491AD0 @ 0x00491AD0..0x00491B3C =====
int __usercall sub_491AD0@<eax>(int a1@<edi>, int a2, int a3)
{
  _DWORD *v3; // eax
  int v4; // esi
  int v5; // eax

  if ( (unsigned int)(a1 - 1) > 0x3F )
    return -2147483647;
  v3 = (_DWORD *)sub_4406C0(a2, (int)dword_56674C);
  v4 = (int)v3;
  if ( !v3 )
    return -1;
  sub_43A710(a1, v3, a3);
  v5 = sub_42B550(v4, 0);
  sub_42B540(v5, 1);
  sub_42CA00(v4);
  sub_42BBB0(v4);
  return 0;
}

// ===== sub_491B40 @ 0x00491B40..0x00491C3F =====
int __cdecl sub_491B40(int a1, int a2, int a3, int a4, int a5, int a6, int a7, void *a8, int a9, int a10, int a11)
{
  int v11; // ecx
  int v12; // esi
  _DWORD *v14; // eax
  _DWORD *v15; // esi

  v12 = v11;
  if ( !a8 )
    return -2147483647;
  if ( !sub_443270((int)dword_56674C, v11) )
    return -1;
  v14 = operator new(0xA8u);
  if ( v14 )
    v15 = sub_431BD0(a1, v12, v14);
  else
    v15 = 0;
  if ( a5 )
    sub_431D50(a9, (int)a8, (int)v15, a2, a3, a4, a6, a7);
  else
    sub_431D10((int)v15, a6, a7, (int)a8, a9);
  sub_431E80(a10, v15, a11);
  sub_4451C0(a1, (int)v15);
  return 0;
}

// ===== sub_491C40 @ 0x00491C40..0x00491D5C =====
int __cdecl sub_491C40(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9,
        int a10,
        int a11,
        int a12)
{
  int v12; // ecx
  int v13; // esi
  _DWORD *v14; // eax
  _DWORD *v15; // esi
  int v16; // eax
  int v17; // edi
  _DWORD *v19; // [esp+10h] [ebp-10h]

  v13 = v12;
  if ( sub_443270((int)dword_56674C, v12) )
  {
    v14 = operator new(0xB8u);
    v19 = v14;
    if ( v14 )
      v15 = sub_432870(a1, v13, v14);
    else
      v15 = 0;
    v16 = sub_432970(a8, a9, (int)v15, a2, a3, a4, a5, a6, a7, a10);
    switch ( v16 )
    {
      case 0:
        sub_431E80(a11, v15, a12);
        sub_4451C0(a1, (int)v15);
        return 0;
      case -2147483647:
        v17 = -2147483647;
        break;
      case -2147483646:
        v17 = -2147483646;
        break;
      default:
        v17 = (int)v19;
        if ( !v19 )
          return v17;
        break;
    }
    if ( v15 )
      (*(void (__thiscall **)(_DWORD *, int))*v15)(v15, 1);
    return v17;
  }
  return -1;
}

// ===== sub_491D60 @ 0x00491D60..0x00491E51 =====
int __cdecl sub_491D60(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        void *a9,
        int a10,
        int a11,
        int a12)
{
  int v12; // ecx
  int v13; // esi
  _DWORD *v15; // eax
  _DWORD *v16; // esi

  v13 = v12;
  if ( !a9 )
    return -2147483647;
  if ( !sub_443270((int)dword_56674C, v12) )
    return -1;
  v15 = operator new(0xA8u);
  if ( v15 )
    v16 = sub_431BD0(a1, v13, v15);
  else
    v16 = 0;
  sub_431D90(a8, v16, a2, a3, a4, a5, a6, a7, (int)a9, a10);
  sub_431E80(a11, v16, a12);
  sub_4451C0(a1, (int)v16);
  return 0;
}

// ===== sub_491E60 @ 0x00491E60..0x00491F8F =====
int __cdecl sub_491E60(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        unsigned int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  int v12; // ecx
  int v14; // esi
  _DWORD *v16; // edi
  _DWORD *v17; // eax
  int v18; // eax
  _DWORD *v19; // [esp+44h] [ebp+28h]

  v14 = v12;
  if ( !a9 )
    return -2147483647;
  v16 = 0;
  if ( !sub_443270((int)dword_56674C, v12) )
    return -1;
  v17 = operator new(0xD8u);
  v19 = v17;
  if ( v17 )
    v16 = sub_432360(a1, v14, v17);
  v18 = sub_432490(a4, a10, (int)v16, a2, a3, a4, a5, a6, a7, a8, a9);
  if ( v18 )
  {
    if ( v18 == -2147483647 )
    {
      if ( v16 )
        (*(void (__thiscall **)(_DWORD *, int))*v16)(v16, 1);
      return -2147483645;
    }
    else
    {
      return (int)v19;
    }
  }
  else
  {
    sub_431E80(a11, v16, a12);
    sub_4451C0(a1, (int)v16);
    return 0;
  }
}

// ===== sub_491F90 @ 0x00491F90..0x004920E7 =====
unsigned int __cdecl sub_491F90(
        int a1,
        unsigned int a2,
        int a3,
        unsigned int a4,
        int a5,
        int a6,
        unsigned int a7,
        int a8,
        int a9)
{
  int v9; // ecx
  int v10; // esi
  _DWORD *v11; // eax
  _DWORD *v12; // esi
  unsigned int result; // eax
  _DWORD *v14; // [esp+10h] [ebp-10h]

  v10 = v9;
  if ( !sub_443270((int)dword_56674C, v9) )
    return -1;
  v11 = operator new(0xC8u);
  v14 = v11;
  if ( v11 )
    v12 = sub_43C7A0(a1, v10, v11);
  else
    v12 = 0;
  result = sub_43C8A0(a2, a7, (int)v12, a3, a4, a5, a6);
  if ( result <= 0x80000002 )
  {
    switch ( result )
    {
      case 0x80000002:
        return -2147483646;
      case 0u:
        sub_431E80(a8, v12, a9);
        sub_4451C0(a1, (int)v12);
        return 0;
      case 0x80000001:
        return result;
    }
    return (unsigned int)v14;
  }
  if ( result == -2147483645 )
    return -2147483645;
  if ( result != -2147483644 )
    return (unsigned int)v14;
  return result;
}

// ===== sub_4920F0 @ 0x004920F0..0x004920F5 =====
// attributes: thunk
int sub_4920F0()
{
  return sub_4417E0();
}

// ===== sub_492100 @ 0x00492100..0x00492149 =====
unsigned int sub_492100()
{
  unsigned int result; // eax
  unsigned int v1; // esi

  result = sub_424F70();
  if ( result )
  {
    result = sub_498720();
    v1 = result;
    if ( dword_56698C <= result )
    {
      if ( sub_441810() )
        sub_461D80();
      result = dword_503E2C * ((v1 - dword_56698C) / dword_503E2C + 1);
      dword_56698C += result;
    }
  }
  return result;
}

// ===== sub_492150 @ 0x00492150..0x00492160 =====
int __usercall sub_492150@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3)
{
  return sub_4418B0(a3, a2, a1);
}

// ===== sub_492160 @ 0x00492160..0x00492167 =====
BOOL __usercall sub_492160@<eax>(int a1@<eax>)
{
  return sub_441A00(a1);
}

// ===== sub_492170 @ 0x00492170..0x00492178 =====
BOOL __usercall sub_492170@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441A80(a2, a1);
}

// ===== sub_492180 @ 0x00492180..0x00492187 =====
int __usercall sub_492180@<eax>(int a1@<eax>, int a2@<edi>)
{
  return sub_441AB0(a2, a1);
}

// ===== sub_492190 @ 0x00492190..0x00492198 =====
BOOL __usercall sub_492190@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441B30(a2, a1);
}

// ===== sub_4921A0 @ 0x004921A0..0x004921BB =====
int __usercall sub_4921A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_441BA0(a3, a4, a5, a6, a2, a1);
}

// ===== sub_4921C0 @ 0x004921C0..0x004921E0 =====
int __usercall sub_4921C0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  return sub_441C20(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_4921E0 @ 0x004921E0..0x004921E8 =====
int __usercall sub_4921E0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441CB0(a2, a1);
}

// ===== sub_4921F0 @ 0x004921F0..0x004921F8 =====
int __usercall sub_4921F0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441D30(a2, a1);
}

// ===== sub_492200 @ 0x00492200..0x00492208 =====
int __usercall sub_492200@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441DB0(a2, a1);
}

// ===== sub_492210 @ 0x00492210..0x00492218 =====
int __usercall sub_492210@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441E30(a2, a1);
}

// ===== sub_492220 @ 0x00492220..0x00492228 =====
int __usercall sub_492220@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441EB0(a2, a1);
}

// ===== sub_492230 @ 0x00492230..0x00492244 =====
int __usercall sub_492230@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_441F30(a3, a4, a2, a1);
}

// ===== sub_492250 @ 0x00492250..0x00492264 =====
int __usercall sub_492250@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_441FB0(a3, a4, a2, a1);
}

// ===== sub_492270 @ 0x00492270..0x00492278 =====
int __usercall sub_492270@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_442030(a2, a1);
}

// ===== sub_492280 @ 0x00492280..0x004922BC =====
int __cdecl sub_492280(int a1)
{
  unsigned int v1; // ecx
  int result; // eax

  result = 0;
  if ( v1 - 1 <= 0x3E7 )
  {
    dword_56698C = 0;
    dword_503E2C = 0x3E8 / v1;
    sub_424F60(a1);
    sub_461D70();
    return 1;
  }
  return result;
}

// ===== sub_4922C0 @ 0x004922C0..0x004922C6 =====
int __usercall sub_4922C0@<eax>(int result@<eax>)
{
  dword_503E28 = result;
  return result;
}

// ===== sub_4922D0 @ 0x004922D0..0x00492318 =====
int sub_4922D0()
{
  unsigned __int16 v0; // ax

  v0 = (unsigned int)(22695477 * dword_503E28) >> 16;
  dword_503E28 = (v0 << 16) + (unsigned __int16)(20021 * dword_503E28) + 1;
  return v0 & 0x7FFF;
}

// ===== sub_492320 @ 0x00492320..0x00492359 =====
void sub_492320()
{
  if ( dword_566758 )
  {
    operator delete[](dword_566758);
    dword_566758 = 0;
  }
  if ( dword_566760 )
  {
    operator delete[](dword_566760);
    dword_566760 = 0;
  }
}

// ===== sub_492360 @ 0x00492360..0x004923E7 =====
void sub_492360()
{
  int *i; // eax
  void *v1; // esi
  void *v2; // esi

  v1 = dword_56674C;
  if ( dword_56674C )
  {
    sub_442C30(dword_56674C);
    operator delete(v1);
  }
  if ( dword_566750 )
  {
    v2 = dword_566750;
    sub_4076D0((int)dword_566750);
    operator delete(v2);
  }
  sub_408790();
  sub_408740(0, 0);
  sub_407A50();
  if ( dword_566754 )
    (**(void (__thiscall ***)(int, int))dword_566754)(dword_566754, 1);
  sub_432DD0();
  sub_433470();
  sub_434B90();
  sub_439970();
  if ( dword_5666F8 )
    (**(void (__thiscall ***)(int, int))dword_5666F8)(dword_5666F8, 1);
  if ( dword_565D64 )
  {
    for ( i = (int *)dword_565D68; dword_565D68; i = (int *)dword_565D68 )
      sub_446960(*i, 0);
    DeleteCriticalSection(&stru_50EA40);
    dword_565D64 = 0;
  }
}

// ===== sub_4923F0 @ 0x004923F0..0x004923FA =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004923F0

// ===== sub_492400 @ 0x00492400..0x00492413 =====
int sub_492400()
{
  int result; // eax

  if ( dword_56676C )
    return (**(int (__thiscall ***)(size_t, int))dword_56676C)(dword_56676C, 1);
  return result;
}

// ===== sub_492420 @ 0x00492420..0x00492445 =====
BOOL sub_492420()
{
  BOOL result; // eax

  if ( hWndParent )
  {
    sub_464190();
    result = DestroyWindow(hWndParent);
    hWndParent = 0;
  }
  return result;
}

// ===== sub_492450 @ 0x00492450..0x00492560 =====
void sub_492450()
{
  void *v0; // ecx
  void *v1; // ecx
  void *v2; // ecx
  void *v3; // ecx
  void *v4; // ecx

  dword_5666B8 = 0;
  sub_498400();
  sub_49A2D0(0);
  sub_46ABE0();
  sub_46A240();
  sub_48D7C0();
  sub_4923F0();
  sub_490B20();
  sub_405AD0();
  sub_401DF0();
  sub_46C540(v0);
  sub_495E90();
  sub_4965E0();
  sub_492320();
  sub_492360();
  sub_492400();
  sub_48DBB0();
  sub_498970();
  sub_46BD60(0);
  sub_46C1A0(v1);
  sub_494A30();
  sub_4954A0();
  sub_496070();
  sub_46A880(v2);
  sub_42FAC0();
  sub_45E2E0();
  sub_490B00();
  sub_468B30();
  sub_468E00();
  sub_48FA60(v3);
  sub_463650();
  sub_463850();
  sub_46D620();
  sub_49A030();
  sub_496270();
  sub_48DFB0();
  sub_46BB40();
  sub_465250();
  sub_492420();
  sub_496500();
  sub_496490();
  sub_48F0E0();
  sub_4602C0(1);
  sub_460620(v4);
  sub_498700();
  sub_493AC0();
  sub_464460();
  sub_46F860();
  sub_46BC30(0);
  DeleteCriticalSection(&stru_560F88);
}

// ===== sub_492560 @ 0x00492560..0x00492755 =====
_DWORD *__thiscall sub_492560(_DWORD *this, int xRight, int yBottom)
{
  int v3; // ebx
  HMODULE LibraryA; // eax
  FARPROC AtlAxAttachControl; // eax
  int v7; // ecx
  int v8; // eax
  int v9; // ecx
  HANDLE EventA; // eax
  HANDLE WaitableTimerA; // eax
  BOOL v12; // eax
  uintptr_t v13; // eax
  LARGE_INTEGER DueTime; // [esp+Ch] [ebp-1Ch] BYREF
  struct tagRECT rc; // [esp+14h] [ebp-14h] BYREF

  v3 = dword_565B28;
  *this = &SCFlashControl::`vftable';
  sub_493120(0);
  LibraryA = LoadLibraryA("ATL100.dll");
  this[92] = LibraryA;
  if ( !LibraryA )
  {
    v7 = -2147483638;
    goto LABEL_5;
  }
  AtlAxAttachControl = GetProcAddress(LibraryA, "AtlAxAttachControl");
  this[93] = AtlAxAttachControl;
  if ( !AtlAxAttachControl )
  {
    FreeLibrary((HMODULE)this[92]);
    this[92] = 0;
    v7 = -2147483637;
LABEL_5:
    sub_493120(v7);
  }
  sub_493130(v3);
  SetRect(&rc, 0, 0, xRight, yBottom);
  sub_493140((int)this, rc.left, rc.top, rc.right, rc.bottom);
  v8 = sub_49EEE0(0);
  this[8] = 0;
  sub_41BF10(v8, v9);
  this[10] = -1;
  this[11] = 0;
  sub_493180(this);
  sub_493280(0);
  sub_4932A0();
  sub_4931B0(0);
  sub_4931D0();
  this[85] = sub_492EF0();
  this[90] = 0;
  this[91] = 0;
  this[88] = CreateEventA(0, 0, 0, 0);
  EventA = CreateEventA(0, 0, 0, 0);
  this[87] = EventA;
  if ( !this[88] || !EventA )
    sub_493120(-2147483647);
  if ( this[85] )
  {
    DueTime.QuadPart = 0LL;
    WaitableTimerA = CreateWaitableTimerA(0, 0, 0);
    this[89] = WaitableTimerA;
    v12 = SetWaitableTimer(WaitableTimerA, &DueTime, 1, 0, &DueTime, 0);
    if ( !this[89] || !v12 )
      sub_493120(-2147483646);
  }
  this[83] = 0;
  this[84] = 1;
  v13 = _beginthreadex(0, 0, sub_492810, this, 0, 0);
  if ( v13 == -1 )
    return (_DWORD *)sub_493120(-2147483645);
  this[83] = v13;
  sub_492F70();
  return this;
}

// ===== sub_492760 @ 0x00492760..0x00492781 =====
void *__thiscall sub_492760(void *this, char a2)
{
  sub_492790();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_492790 @ 0x00492790..0x0049280A =====
int __thiscall sub_492790(void *this)
{
  int result; // eax
  HMODULE v3; // [esp-4h] [ebp-10h]

  *(_DWORD *)this = &SCFlashControl::`vftable';
  sub_4930B0();
  if ( *((_DWORD *)this + 85) && *((_DWORD *)this + 89) )
  {
    CloseHandle(*((HANDLE *)this + 89));
    *((_DWORD *)this + 89) = 0;
  }
  if ( *((_DWORD *)this + 88) )
  {
    CloseHandle(*((HANDLE *)this + 88));
    *((_DWORD *)this + 88) = 0;
  }
  if ( *((_DWORD *)this + 87) )
  {
    CloseHandle(*((HANDLE *)this + 87));
    *((_DWORD *)this + 87) = 0;
  }
  result = *((_DWORD *)this + 92);
  if ( result )
  {
    v3 = (HMODULE)*((_DWORD *)this + 92);
    *((_DWORD *)this + 93) = 0;
    result = FreeLibrary(v3);
    *((_DWORD *)this + 92) = 0;
  }
  return result;
}

// ===== sub_492810 @ 0x00492810..0x00492C54 =====
void __stdcall __noreturn sub_492810(void *a1)
{
  const RECT *v1; // edi
  int right; // ecx
  int bottom; // edi
  int v4; // ebx
  int SystemMetrics; // eax
  HWND Window; // eax
  HDC CompatibleDC; // ebx
  int (__stdcall *v8)(LPVOID, HWND, _DWORD); // eax
  HBITMAP v9; // eax
  HBITMAP v10; // edi
  int v11; // edx
  int v12; // eax
  HWND v13; // edi
  DWORD v14; // eax
  DWORD v15; // eax
  int v16; // edi
  int v17; // eax
  int v18; // eax
  HDC v19; // esi
  tagMSG Msg; // [esp+Ch] [ebp-A0h] BYREF
  WNDCLASSEXA v21; // [esp+28h] [ebp-84h] BYREF
  HANDLE pHandles; // [esp+58h] [ebp-54h] BYREF
  int v23; // [esp+5Ch] [ebp-50h]
  HGDIOBJ h; // [esp+60h] [ebp-4Ch]
  LPCRECT lprcBounds; // [esp+64h] [ebp-48h]
  HDC hDC; // [esp+68h] [ebp-44h]
  HWND hWndParent; // [esp+6Ch] [ebp-40h] BYREF
  int nWidth; // [esp+70h] [ebp-3Ch]
  HWND hWnd; // [esp+74h] [ebp-38h]
  LPVOID ppv; // [esp+78h] [ebp-34h] BYREF
  BITMAPINFO pbmi; // [esp+7Ch] [ebp-30h] BYREF

  v1 = (const RECT *)sub_493170();
  lprcBounds = v1;
  hWndParent = (HWND)sub_42D560((int)a1);
  v21.cbSize = 48;
  v21.style = 3;
  v21.lpfnWndProc = sub_492C70;
  v21.cbClsExtra = 0;
  v21.cbWndExtra = 0;
  v21.hInstance = (HINSTANCE)GetWindowLongA(hWndParent, -6);
  v21.hIcon = 0;
  v21.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
  v21.hbrBackground = (HBRUSH)GetStockObject(5);
  v21.lpszClassName = "InvisibleFlashWindow";
  v21.lpszMenuName = 0;
  v21.hIconSm = 0;
  RegisterClassExA(&v21);
  right = v1->right;
  bottom = v1->bottom;
  nWidth = right;
  v4 = (GetSystemMetrics(0) - right) / 2;
  SystemMetrics = GetSystemMetrics(1);
  Window = CreateWindowExA(
             0,
             v21.lpszClassName,
             "Flash Window",
             0x90000000,
             v4,
             (SystemMetrics - bottom) / 2,
             nWidth,
             bottom,
             hWndParent,
             0,
             v21.hInstance,
             0);
  CompatibleDC = 0;
  hWnd = Window;
  ppv = 0;
  hDC = 0;
  h = 0;
  if ( Window )
  {
    ShowWindow(Window, 0);
    UpdateWindow(hWnd);
    SetFocus(hWndParent);
    if ( CoCreateInstance(&stru_4E411C, 0, 0x17u, &stru_4E410C, &ppv)
      || (v8 = (int (__stdcall *)(LPVOID, HWND, _DWORD))*((_DWORD *)a1 + 93)) == 0
      || v8(ppv, hWnd, 0) )
    {
      if ( !sub_490AE0((int)a1) )
        sub_493120(-2147483643);
    }
    else
    {
      hDC = GetDC(hWnd);
      hWndParent = 0;
      CompatibleDC = CreateCompatibleDC(hDC);
      ReleaseDC(hWnd, hDC);
      pbmi.bmiHeader.biHeight = -bottom;
      pbmi.bmiHeader.biWidth = nWidth;
      pbmi.bmiHeader.biSize = 40;
      *(_DWORD *)&pbmi.bmiHeader.biPlanes = 2097153;
      pbmi.bmiHeader.biCompression = 0;
      pbmi.bmiHeader.biSizeImage = 4 * nWidth * bottom;
      memset(&pbmi.bmiHeader.biXPelsPerMeter, 0, 16);
      v9 = CreateDIBSection(CompatibleDC, &pbmi, 0, (void **)&hWndParent, 0, 0);
      v10 = v9;
      hDC = (HDC)v9;
      if ( v9 )
      {
        h = SelectObject(CompatibleDC, v9);
        sub_4931B0(v10);
        sub_4931D0(hWndParent);
      }
      else if ( !sub_490AE0((int)a1) )
      {
        sub_493120(-2147483642);
      }
    }
  }
  else if ( !sub_490AE0((int)a1) )
  {
    sub_493120(-2147483644);
  }
  if ( sub_4931F0() )
  {
    v11 = *((_DWORD *)a1 + 89);
    pHandles = (HANDLE)*((_DWORD *)a1 + 88);
    v23 = v11;
    nWidth = 2;
  }
  else
  {
    pHandles = (HANDLE)*((_DWORD *)a1 + 88);
    v23 = 0;
    nWidth = 1;
  }
  sub_493020(hDC);
  SetEvent(*((HANDLE *)a1 + 87));
  while ( 1 )
  {
    while ( 1 )
    {
      v12 = sub_493090();
      v13 = hWnd;
      if ( !v12 )
      {
LABEL_36:
        if ( CompatibleDC )
        {
          v19 = hDC;
          if ( hDC )
          {
            SelectObject(CompatibleDC, h);
            DeleteObject(v19);
          }
          DeleteDC(CompatibleDC);
        }
        if ( ppv )
        {
          (*(void (__stdcall **)(LPVOID))(*(_DWORD *)ppv + 8))(ppv);
          ppv = 0;
        }
        if ( v13 )
        {
          DestroyWindow(v13);
          while ( GetMessageA(&Msg, 0, 0, 0) )
          {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
          }
        }
        _endthreadex(0);
      }
      if ( !PeekMessageA(&Msg, hWnd, 0, 0, 1u) )
        break;
      if ( Msg.message == 18 )
        goto LABEL_36;
      TranslateMessage(&Msg);
      DispatchMessageA(&Msg);
    }
    v14 = MsgWaitForMultipleObjects(nWidth, &pHandles, 0, 0x64u, 0xFFu);
    if ( v14 )
    {
      v15 = v14 - 1;
      if ( !v15 || v15 == 257 )
        goto LABEL_35;
    }
    else
    {
      v16 = sub_490AE0((int)a1);
      if ( v16 )
      {
        sub_493120(v16);
        SetEvent(*((HANDLE *)a1 + 87));
      }
      else
      {
        v17 = sub_493210() - 32;
        if ( v17 )
        {
          v18 = v17 - 1;
          if ( v18 )
          {
            if ( v18 == 15 )
              v16 = sub_492DB0(a1);
          }
          else
          {
            v16 = sub_492D90();
          }
        }
        else
        {
          v16 = sub_492CB0(a1, ppv);
          if ( !v16 )
          {
            while ( OleDraw((LPUNKNOWN)ppv, 1u, CompatibleDC, lprcBounds) )
              ;
          }
        }
        sub_493120(v16);
        SetEvent(*((HANDLE *)a1 + 87));
LABEL_35:
        sub_492DF0(a1, a1, CompatibleDC, lprcBounds);
      }
    }
  }
}

// ===== sub_492C70 @ 0x00492C70..0x00492CA2 =====
LRESULT __stdcall sub_492C70(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if ( Msg != 2 )
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  PostQuitMessage(0);
  return 0;
}

// ===== sub_492CB0 @ 0x00492CB0..0x00492D87 =====
int __stdcall sub_492CB0(int a1, int a2)
{
  const CHAR *v2; // eax
  const CHAR *v3; // ebx
  int v4; // edi
  void *v5; // esp
  WCHAR *v6; // eax
  int (__stdcall *v8)(int, int *); // eax
  WCHAR v9[6]; // [esp+0h] [ebp-14h] BYREF
  int v10; // [esp+Ch] [ebp-8h] BYREF

  v2 = (const CHAR *)sub_4931A0();
  v3 = v2;
  if ( v2 && (v4 = lstrlenA(v2) + 1, v4 <= 0x3FFFFFFF) && (v5 = alloca(2 * v4), v9) )
  {
    v9[0] = 0;
    v6 = MultiByteToWideChar(3u, 0, v3, -1, v9, v4) != 0 ? v9 : 0;
  }
  else
  {
    v6 = 0;
  }
  if ( (*(int (__stdcall **)(int, WCHAR *))(*(_DWORD *)a2 + 88))(a2, v6) )
    return -2147483640;
  if ( (*(int (__stdcall **)(int, _DWORD))(*(_DWORD *)a2 + 80))(a2, 0) )
    return -2147483641;
  if ( (*(int (__stdcall **)(int))(*(_DWORD *)a2 + 112))(a2) )
    return -2147483639;
  v8 = *(int (__stdcall **)(int, int *))(*(_DWORD *)a2 + 32);
  v10 = 0;
  if ( v8(a2, &v10) )
    return -2147483641;
  sub_41BF10(a1, v10);
  return 0;
}

// ===== sub_492D90 @ 0x00492D90..0x00492DAC =====
int __usercall sub_492D90@<eax>(int a1@<eax>)
{
  bool v1; // zf
  int result; // eax

  v1 = (*(int (__stdcall **)(int, _DWORD))(*(_DWORD *)a1 + 136))(a1, 0) == 0;
  result = -2147483641;
  if ( v1 )
    return 0;
  return result;
}

// ===== sub_492DB0 @ 0x00492DB0..0x00492DEE =====
int __userpurge sub_492DB0@<eax>(int a1@<eax>, int a2)
{
  int result; // eax
  __int16 v3; // [esp+4h] [ebp-4h] BYREF

  if ( (*(int (__stdcall **)(int, __int16 *))(*(_DWORD *)a1 + 144))(a1, &v3) )
    return -2147483641;
  result = 0;
  *(_DWORD *)(a2 + 32) = v3 != 0;
  return result;
}

// ===== sub_492DF0 @ 0x00492DF0..0x00492E6A =====
HRESULT __userpurge sub_492DF0@<eax>(IUnknown *a1@<eax>, int a2@<ecx>, int a3, HDC hdcDraw, LPCRECT lprcBounds)
{
  HRESULT result; // eax
  HWND v8; // eax
  LPCRECT v9; // [esp-4h] [ebp-Ch]

  result = sub_492E70(a2);
  if ( result )
  {
    result = OleDraw(a1, 1u, hdcDraw, lprcBounds);
    if ( !result )
    {
      if ( sub_493040(&lprcBounds) )
      {
        return ((int (__cdecl *)(LPCRECT))hdcDraw)(lprcBounds);
      }
      else
      {
        sub_493070(&lprcBounds);
        v9 = lprcBounds;
        v8 = (HWND)sub_42D560(a2);
        return PostMessageA(v8, 0x403u, (WPARAM)hdcDraw, (LPARAM)v9);
      }
    }
  }
  return result;
}

// ===== sub_492E70 @ 0x00492E70..0x00492EEE =====
int __userpurge sub_492E70@<eax>(int a1@<edi>, int a2@<esi>, int a3)
{
  int (__stdcall *v3)(int, int *); // edx
  int v4; // eax
  int v5; // ecx
  int v7; // edx
  int v8; // [esp+4h] [ebp-4h] BYREF

  v3 = *(int (__stdcall **)(int, int *))(*(_DWORD *)a2 + 92);
  v8 = 0;
  if ( v3(a2, &v8) )
    return 0;
  v4 = v8;
  if ( *(_DWORD *)(a1 + 40) == v8 )
  {
    if ( sub_4467B0(a3) && v5 == *(_DWORD *)(a1 + 36) - 1 )
    {
      (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)a2 + 136))(a2, 0);
      (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 112))(a2);
      return 1;
    }
    return 0;
  }
  v7 = *(_DWORD *)(a1 + 36) - 1;
  *(_DWORD *)(a1 + 40) = v8;
  if ( v4 == v7 )
    sub_4930A0();
  return 1;
}

// ===== sub_492EF0 @ 0x00492EF0..0x00492F61 =====
BOOL sub_492EF0()
{
  struct _OSVERSIONINFOA VersionInformation; // [esp+0h] [ebp-98h] BYREF

  memset(&VersionInformation, 0, sizeof(VersionInformation));
  VersionInformation.dwOSVersionInfoSize = 148;
  GetVersionExA(&VersionInformation);
  return VersionInformation.dwPlatformId == 2 && VersionInformation.dwMajorVersion >= 4;
}

// ===== sub_492F70 @ 0x00492F70..0x00493014 =====
DWORD __usercall sub_492F70@<eax>(int a1@<esi>)
{
  DWORD result; // eax
  HWND v2; // eax
  HWND v3; // eax
  MSG Msg; // [esp+0h] [ebp-1Ch] BYREF

  for ( result = WaitForSingleObject(*(HANDLE *)(a1 + 348), 1u);
        result == 258;
        result = WaitForSingleObject(*(HANDLE *)(a1 + 348), 1u) )
  {
    v2 = (HWND)sub_42D560(a1);
    if ( PeekMessageA(&Msg, v2, 0, 0, 1u) )
    {
      while ( Msg.message != 18 )
      {
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
        v3 = (HWND)sub_42D560(a1);
        if ( !PeekMessageA(&Msg, v3, 0, 0, 1u) )
          goto LABEL_7;
      }
      PostQuitMessage(0);
      Sleep(0xAu);
    }
LABEL_7:
    ;
  }
  return result;
}

// ===== sub_493020 @ 0x00493020..0x00493036 =====
int __userpurge sub_493020@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 360) = a2;
  *(_DWORD *)(result + 364) = a3;
  return result;
}

// ===== sub_493040 @ 0x00493040..0x00493067 =====
BOOL __fastcall sub_493040(int a1, _DWORD *a2, _DWORD *a3)
{
  int v3; // eax
  int v4; // ecx

  v3 = *(_DWORD *)(a1 + 316);
  if ( v3 )
  {
    v4 = *(_DWORD *)(a1 + 320);
    *a2 = v3;
    *a3 = v4;
  }
  return v3 != 0;
}

// ===== sub_493070 @ 0x00493070..0x0049308A =====
int __userpurge sub_493070@<eax>(int a1@<eax>, _DWORD *a2@<edx>, _DWORD *a3)
{
  int v3; // ecx
  int result; // eax

  v3 = *(_DWORD *)(a1 + 308);
  result = *(_DWORD *)(a1 + 312);
  *a2 = v3;
  *a3 = result;
  return result;
}

// ===== sub_493090 @ 0x00493090..0x00493097 =====
int __usercall sub_493090@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 336);
}

// ===== sub_4930A0 @ 0x004930A0..0x004930A4 =====
int __usercall sub_4930A0@<eax>(int result@<eax>)
{
  ++*(_DWORD *)(result + 44);
  return result;
}

// ===== sub_4930B0 @ 0x004930B0..0x00493119 =====
DWORD __usercall sub_4930B0@<eax>(int a1@<esi>)
{
  DWORD result; // eax
  void *v2; // [esp-8h] [ebp-8h]

  v2 = *(void **)(a1 + 332);
  *(_DWORD *)(a1 + 336) = 0;
  result = WaitForSingleObject(v2, 0x2710u);
  if ( result == 258 )
  {
    if ( *(_DWORD *)(a1 + 364) )
    {
      DeleteObject(*(HGDIOBJ *)(a1 + 364));
      *(_DWORD *)(a1 + 364) = 0;
    }
    if ( *(_DWORD *)(a1 + 360) )
    {
      DeleteDC(*(HDC *)(a1 + 360));
      *(_DWORD *)(a1 + 360) = 0;
    }
    return TerminateThread(*(HANDLE *)(a1 + 332), 0);
  }
  return result;
}

// ===== sub_493120 @ 0x00493120..0x00493124 =====
int __usercall sub_493120@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 4) = a2;
  return result;
}

// ===== sub_493130 @ 0x00493130..0x00493134 =====
int __usercall sub_493130@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 8) = a2;
  return result;
}

// ===== sub_493140 @ 0x00493140..0x00493164 =====
BOOL __stdcall sub_493140(int a1, int xLeft, int yTop, int xRight, int yBottom)
{
  return SetRect((LPRECT)(a1 + 12), xLeft, yTop, xRight, yBottom);
}

// ===== sub_493170 @ 0x00493170..0x00493174 =====
int __usercall sub_493170@<eax>(int a1@<eax>)
{
  return a1 + 12;
}

// ===== sub_493180 @ 0x00493180..0x00493195 =====
const CHAR *__usercall sub_493180@<eax>(const CHAR *result@<eax>, int a2@<ecx>)
{
  if ( result )
    return lstrcpyA((LPSTR)(a2 + 48), result);
  *(_BYTE *)(a2 + 48) = 0;
  return result;
}

// ===== sub_4931A0 @ 0x004931A0..0x004931A4 =====
int __usercall sub_4931A0@<eax>(int a1@<eax>)
{
  return a1 + 48;
}

// ===== sub_4931B0 @ 0x004931B0..0x004931B7 =====
int __usercall sub_4931B0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 324) = a2;
  return result;
}

// ===== sub_4931C0 @ 0x004931C0..0x004931C7 =====
int __usercall sub_4931C0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 324);
}

// ===== sub_4931D0 @ 0x004931D0..0x004931D7 =====
int __usercall sub_4931D0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 328) = a2;
  return result;
}

// ===== sub_4931E0 @ 0x004931E0..0x004931E7 =====
int __usercall sub_4931E0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 328);
}

// ===== sub_4931F0 @ 0x004931F0..0x004931F7 =====
int __usercall sub_4931F0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 340);
}

// ===== sub_493200 @ 0x00493200..0x00493207 =====
int __usercall sub_493200@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 344) = a2;
  return result;
}

// ===== sub_493210 @ 0x00493210..0x00493217 =====
int __usercall sub_493210@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 344);
}

// ===== sub_493220 @ 0x00493220..0x0049327D =====
const CHAR *__usercall sub_493220@<eax>(const char *a1@<edi>, int a2@<esi>)
{
  CHAR v3[260]; // [esp+0h] [ebp-20Ch] BYREF
  CHAR Buffer[260]; // [esp+104h] [ebp-108h] BYREF

  GetCurrentDirectoryA(0x104u, Buffer);
  wsprintfA(v3, "%s\\%s", Buffer, a1);
  return sub_493180(v3, a2);
}

// ===== sub_493280 @ 0x00493280..0x00493296 =====
int __userpurge sub_493280@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 316) = a2;
  *(_DWORD *)(result + 320) = a3;
  return result;
}

// ===== sub_4932A0 @ 0x004932A0..0x004932AF =====
int __usercall sub_4932A0@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 308) = 0;
  *(_DWORD *)(result + 312) = 0;
  return result;
}

// ===== sub_4932B0 @ 0x004932B0..0x004932F7 =====
int __usercall sub_4932B0@<eax>(int a1@<eax>)
{
  if ( !sub_490AE0(a1) )
  {
    ResetEvent(*(HANDLE *)(a1 + 348));
    sub_493200(a1, 32);
    SetEvent(*(HANDLE *)(a1 + 352));
    sub_492F70(a1);
  }
  return sub_490AE0(a1);
}

// ===== sub_493300 @ 0x00493300..0x00493347 =====
int __usercall sub_493300@<eax>(int a1@<eax>)
{
  if ( !sub_490AE0(a1) )
  {
    ResetEvent(*(HANDLE *)(a1 + 348));
    sub_493200(a1, 33);
    SetEvent(*(HANDLE *)(a1 + 352));
    sub_492F70(a1);
  }
  return sub_490AE0(a1);
}

// ===== sub_493350 @ 0x00493350..0x00493393 =====
BOOL __userpurge sub_493350@<eax>(int a1@<edi>, int *a2, _DWORD *a3)
{
  void *v3; // esi
  _BYTE pv[12]; // [esp+8h] [ebp-18h] BYREF
  int v6; // [esp+14h] [ebp-Ch]

  v3 = (void *)sub_4931C0(a1);
  if ( v3 )
  {
    GetObjectA(v3, 24, pv);
    *a3 = v6;
    *a2 = sub_4931E0(a1);
  }
  return v3 != 0;
}

// ===== sub_4933A0 @ 0x004933A0..0x0049374E =====
int __cdecl sub_4933A0(_BYTE *a1, unsigned int a2)
{
  unsigned __int8 *v2; // ecx
  unsigned int v3; // esi
  _BYTE *v4; // eax
  int v5; // edx
  unsigned int v6; // edi
  unsigned int v7; // edi
  unsigned int v8; // ebx
  unsigned int v9; // eax
  int v10; // eax
  int v11; // edx
  unsigned int v12; // eax
  int v13; // edx
  int v14; // ebx
  unsigned int *v15; // edi
  int v16; // eax
  unsigned int v17; // edx
  unsigned int v18; // eax
  _BYTE *v19; // eax
  int v20; // eax
  int v21; // edx
  unsigned int v22; // eax
  int v23; // edx
  int v24; // ebx
  unsigned int *v25; // edi
  int v26; // eax
  bool v27; // zf
  _BYTE *v28; // eax
  unsigned int v29; // esi
  _BYTE *v31; // [esp+Ch] [ebp-6C38h]
  __int16 v32; // [esp+10h] [ebp-6C34h]
  _DWORD *v33; // [esp+18h] [ebp-6C2Ch]
  unsigned __int8 *v34; // [esp+1Ch] [ebp-6C28h]
  unsigned int v35; // [esp+20h] [ebp-6C24h]
  unsigned int v36; // [esp+20h] [ebp-6C24h]
  unsigned int v37; // [esp+24h] [ebp-6C20h]
  int v38; // [esp+28h] [ebp-6C1Ch]
  unsigned int v39; // [esp+2Ch] [ebp-6C18h]
  _BYTE *v41; // [esp+30h] [ebp-6C14h]
  _BYTE v42[24588]; // [esp+34h] [ebp-6C10h] BYREF
  _BYTE v43[3072]; // [esp+6040h] [ebp-C04h] BYREF

  v3 = 0;
  v34 = v2;
  v39 = 0;
  v38 = 0;
  v4 = v43;
  v5 = 256;
  do
  {
    *((_DWORD *)v4 + 1) = v4;
    *((_DWORD *)v4 + 2) = v4;
    v4 += 12;
    --v5;
  }
  while ( v5 );
  v6 = a2;
  if ( !a2 )
    return 0;
  do
  {
    v7 = v6 - v3;
    v35 = v7;
    v37 = 0;
    v33 = *(_DWORD **)&v43[12 * *v2 + 4];
    v31 = &v43[12 * *v2];
    if ( v33 == (_DWORD *)v31 )
      goto LABEL_18;
    do
    {
      if ( v3 - *v33 == 1 )
        goto LABEL_16;
      v8 = v7;
      if ( v7 >= 0x11 )
        v8 = 17;
      v9 = 1;
      if ( v8 <= 1 )
        goto LABEL_16;
      do
      {
        if ( v2[*v33 - v3 + v9] != v2[v9] )
          break;
        ++v9;
      }
      while ( v9 < v8 );
      if ( v9 > 1 )
      {
        if ( v37 < v9 )
        {
          v37 = v9;
          v32 = v3 - *(_WORD *)v33;
        }
        if ( v9 == v8 )
          break;
      }
      v7 = v35;
LABEL_16:
      v33 = (_DWORD *)v33[1];
    }
    while ( v33 != (_DWORD *)v31 );
    if ( !v37 )
    {
LABEL_18:
      if ( v3 >= 0x801 )
      {
        v10 = 12 * *(v2 - 2049);
        v11 = *(_DWORD *)&v43[v10 + 8];
        *(_DWORD *)&v43[v10 + 8] = *(_DWORD *)(v11 + 8);
        *(_DWORD *)(*(_DWORD *)(v11 + 8) + 4) = &v43[v10];
      }
      v12 = 12 * (v3 % 0x801);
      v13 = 12 * *v2;
      v14 = *(_DWORD *)&v43[v13 + 4];
      v15 = (unsigned int *)&v42[v12];
      *(_DWORD *)&v42[v12 + 4] = v14;
      *(_DWORD *)&v42[v12 + 8] = &v43[v13];
      *v15 = v3;
      *(_DWORD *)(v14 + 8) = &v42[v12];
      v16 = 3 * *v2++;
      ++v3;
      ++v39;
      *(_DWORD *)&v43[4 * v16 + 4] = v15;
    }
    v17 = v39;
    if ( v39 >= 0x80 )
    {
LABEL_24:
      *a1 = v39 - 1;
      v41 = a1 + 1;
      v19 = v41;
      do
      {
        *v19 = v19[v34 - v41];
        ++v19;
        --v17;
      }
      while ( v17 );
      a1 = &v41[v39];
      v38 += v39 + 1;
      v18 = v37;
      v39 = 0;
      v34 = v2;
      goto LABEL_27;
    }
    v18 = v37;
    if ( v37 > 1 )
    {
      if ( v39 )
        goto LABEL_24;
LABEL_27:
      if ( v18 > 1 )
      {
        v36 = v18;
        do
        {
          if ( v3 >= 0x801 )
          {
            v20 = 12 * *(v2 - 2049);
            v21 = *(_DWORD *)&v43[v20 + 8];
            *(_DWORD *)&v43[v20 + 8] = *(_DWORD *)(v21 + 8);
            *(_DWORD *)(*(_DWORD *)(v21 + 8) + 4) = &v43[v20];
          }
          v22 = 12 * (v3 % 0x801);
          v23 = 12 * *v2;
          v24 = *(_DWORD *)&v43[v23 + 4];
          v25 = (unsigned int *)&v42[v22];
          *(_DWORD *)&v42[v22 + 4] = v24;
          *(_DWORD *)&v42[v22 + 8] = &v43[v23];
          *v25 = v3;
          *(_DWORD *)(v24 + 8) = &v42[v22];
          v26 = 3 * *v2++;
          ++v3;
          v27 = v36-- == 1;
          *(_DWORD *)&v43[4 * v26 + 4] = v25;
        }
        while ( !v27 );
        *a1 = (unsigned __int16)((((_WORD)v37 - 2) << 11) + v32 + 32766) >> 8;
        a1[1] = v32 - 2;
        v38 += 2;
        a1 += 2;
        v34 = v2;
      }
      v17 = v39;
    }
    v6 = a2;
  }
  while ( v3 < a2 );
  if ( !v17 )
    return v38;
  *a1 = v17 - 1;
  v28 = a1 + 1;
  v29 = v17;
  do
  {
    *v28 = v28[v34 - (a1 + 1)];
    ++v28;
    --v29;
  }
  while ( v29 );
  return v38 + v17 + 1;
}

// ===== sub_493750 @ 0x00493750..0x004937D5 =====
int __usercall sub_493750@<eax>(int a1@<eax>, unsigned __int8 *a2@<ecx>, _BYTE *a3)
{
  unsigned __int8 v5; // cl
  int v6; // ecx
  _BYTE *v7; // edx
  int v8; // ecx
  int v10; // [esp+8h] [ebp-4h]

  v10 = 0;
  if ( !a1 )
    return 0;
  do
  {
    v5 = *a2;
    if ( (*a2 & 0x80u) == 0 )
    {
      v8 = v5 + 1;
      v10 += v8;
      ++a2;
      for ( a1 += -1 - v8; v8; --v8 )
        *a3++ = *a2++;
    }
    else
    {
      v6 = ((v5 >> 3) & 0xF) + 2;
      v10 += v6;
      a1 -= 2;
      if ( v6 )
      {
        v7 = &a3[-a2[1] - 2 + -256 * (*a2 & 7)];
        do
        {
          *a3++ = *v7++;
          --v6;
        }
        while ( v6 );
      }
      a2 += 2;
    }
  }
  while ( a1 );
  return v10;
}

// ===== sub_4937E0 @ 0x004937E0..0x0049385F =====
void __thiscall sub_4937E0(int this)
{
  _BYTE *v2; // esi
  int v3; // edi
  __int16 v4; // bx
  __int16 v5; // ax
  __int16 i; // [esp+10h] [ebp-18h]
  struct _SYSTEMTIME SystemTime; // [esp+14h] [ebp-14h] BYREF

  sub_46BB70();
  GetSystemTime(&SystemTime);
  sub_4922C0(SystemTime.wMilliseconds);
  v2 = (_BYTE *)(this + 32);
  v3 = *(_DWORD *)(this + 20);
  v4 = 0;
  for ( i = 0; v3; --v3 )
  {
    *v2 += sub_4922D0();
    v5 = (unsigned __int8)*v2;
    i ^= v5;
    v4 += v5;
    ++v2;
  }
  *(_DWORD *)(this + 16) = SystemTime.wMilliseconds;
  *(_WORD *)(this + 28) = v4;
  *(_WORD *)(this + 30) = i;
  sub_46BB80();
}

// ===== sub_493860 @ 0x00493860..0x004938F0 =====
_BYTE *__cdecl sub_493860(int a1)
{
  _BYTE *v1; // eax
  _BYTE *v2; // esi
  int v3; // edi
  __int16 v4; // bx
  __int16 v5; // ax
  int v7; // [esp+Ch] [ebp-Ch]
  _BYTE *v8; // [esp+10h] [ebp-8h]
  __int16 v9; // [esp+14h] [ebp-4h]

  sub_46BB70();
  sub_4922C0(*(_DWORD *)(a1 + 16));
  v1 = operator new[](*(_DWORD *)(a1 + 20));
  v2 = (_BYTE *)(a1 + 32);
  v3 = *(_DWORD *)(a1 + 20);
  v4 = 0;
  v8 = v1;
  v9 = 0;
  if ( v3 )
  {
    v7 = v1 - v2;
    do
    {
      v2[v7] = *v2 - sub_4922D0();
      v5 = (unsigned __int8)*v2;
      v9 ^= v5;
      v4 += v5;
      ++v2;
      --v3;
    }
    while ( v3 );
    v1 = v8;
  }
  if ( *(_WORD *)(a1 + 28) != v4 || *(_WORD *)(a1 + 30) != v9 )
  {
    operator delete[](v1);
    v8 = 0;
  }
  sub_46BB80();
  return v8;
}

// ===== sub_4938F0 @ 0x004938F0..0x0049393F =====
int __usercall sub_4938F0@<eax>(int a1@<eax>, _BYTE *a2)
{
  unsigned __int8 *v3; // esi
  int v4; // edi

  if ( !sub_493940() )
    return 0;
  v3 = sub_493860(a1);
  if ( !v3 )
    return 0;
  v4 = sub_493750(*(_DWORD *)(a1 + 20), v3, a2);
  operator delete[](v3);
  return v4;
}

// ===== sub_493940 @ 0x00493940..0x0049397C =====
BOOL __usercall sub_493940@<eax>(const char *a1@<eax>)
{
  return strcmp(a1, "SDC FORMAT 1.00") == 0;
}

// ===== sub_493980 @ 0x00493980..0x00493A45 =====
int __usercall sub_493980@<eax>(int a1@<eax>, size_t a2@<edi>, void *Src)
{
  void *v4; // ebx
  int v5; // eax
  size_t v6; // esi
  _BYTE *v7; // eax
  unsigned int v9; // [esp+0h] [ebp-18h]
  int v10; // [esp+Ch] [ebp-Ch]
  int v11; // [esp+10h] [ebp-8h]
  _BYTE *v12; // [esp+14h] [ebp-4h]

  v4 = operator new[](v9);
  memcpy_0(v4, Src, a2);
  v5 = sub_4933A0((_BYTE *)(a1 + 32), a2);
  strcpy((char *)a1, "SDC FORMAT 1.00");
  v10 = v5;
  *(_DWORD *)(a1 + 20) = v5;
  *(_DWORD *)(a1 + 24) = a2;
  sub_4937E0(a1);
  v12 = operator new[](2 * a2);
  v11 = 0;
  if ( sub_4938F0(a1, v12) == a2 )
  {
    v6 = 0;
    if ( a2 )
    {
      v7 = v4;
      do
      {
        if ( v7[v12 - (_BYTE *)v4] != *v7 )
          break;
        ++v6;
        ++v7;
      }
      while ( v6 < a2 );
    }
    if ( v6 == a2 )
      v11 = v10 + 32;
  }
  operator delete[](v12);
  operator delete[](v4);
  return v11;
}

// ===== sub_493A50 @ 0x00493A50..0x00493AB3 =====
_DWORD *sub_493A50()
{
  _DWORD *v0; // eax
  _DWORD *result; // eax

  v0 = operator new(8u);
  if ( v0 )
    result = sub_4526E0(v0);
  else
    result = 0;
  dword_566990 = (int)result;
  return result;
}

// ===== sub_493AC0 @ 0x00493AC0..0x00493ADD =====
int sub_493AC0()
{
  int result; // eax

  if ( dword_566990 )
    result = (**(int (__thiscall ***)(int, int))dword_566990)(dword_566990, 1);
  dword_566990 = 0;
  return result;
}

// ===== sub_493AE0 @ 0x00493AE0..0x00493AEE =====
DWORD __usercall sub_493AE0@<eax>(DWORD a1@<eax>)
{
  return sub_4527C0(a1, dword_566990);
}

// ===== sub_493AF0 @ 0x00493AF0..0x00493B58 =====
int sub_493AF0()
{
  memset32(&dword_55EE18, 128, 0x40u);
  dword_55FF18 = 128;
  dword_55FF1C = 128;
  dword_55FF20 = 128;
  dword_55FF24 = 128;
  dword_55FF28 = 128;
  dword_55FF2C = 128;
  dword_55FF30 = 128;
  dword_55FF34 = 128;
  dword_55FF38 = 128;
  dword_55FF3C = 128;
  dword_55FF40 = 128;
  dword_55FF44 = 128;
  dword_55FF48 = 128;
  dword_55FF4C = 128;
  dword_55FF50 = 128;
  dword_55FF54 = 128;
  return sub_493BC0();
}

// ===== sub_493B60 @ 0x00493B60..0x00493B8E =====
int __fastcall sub_493B60(unsigned int a1, unsigned int a2)
{
  int result; // eax

  result = 0;
  if ( a1 < 0x10 && a2 <= 0x80 )
  {
    dword_55FF18[a1] = a2;
    if ( !dword_566994 )
      sub_4A3270(a1, a2);
    return 1;
  }
  return result;
}

// ===== sub_493B90 @ 0x00493B90..0x00493BBE =====
int __fastcall sub_493B90(unsigned int a1, unsigned int a2)
{
  int result; // eax

  result = 0;
  if ( a1 < 0x40 && a2 <= 0x80 )
  {
    dword_55EE18[a1] = a2;
    if ( !dword_566994 )
      sub_4A2DF0(a1, a2);
    return 1;
  }
  return result;
}

// ===== sub_493BC0 @ 0x00493BC0..0x00493BF9 =====
int sub_493BC0()
{
  int i; // esi
  int j; // esi
  int result; // eax

  for ( i = 0; i < 16; ++i )
    sub_4A3270(i, dword_55FF18[i]);
  for ( j = 0; j < 64; ++j )
    result = sub_4A2DF0(j, dword_55EE18[j]);
  return result;
}

// ===== sub_493C00 @ 0x00493C00..0x00493CAF =====
int __usercall sub_493C00@<eax>(const char *a1@<edi>, int a2@<esi>, int a3)
{
  int result; // eax
  char Buffer[780]; // [esp+Ch] [ebp-310h] BYREF

  sub_4A3420();
  sub_4649F0(a1, &::Buffer, Buffer);
  result = sub_4A4790(a2, Buffer, a3, 64, 1.0);
  if ( result == 12 )
  {
    if ( sub_464B80(byte_517C08) )
    {
      sub_4649F0(a1, byte_517C08, Buffer);
      return sub_4A4790(a2, Buffer, a3, 64, 1.0);
    }
    else
    {
      return 12;
    }
  }
  return result;
}

// ===== sub_493CB0 @ 0x00493CB0..0x00493DAF =====
int __usercall sub_493CB0@<eax>(const char *a1@<edi>, int a2, const char *a3, int a4, int a5)
{
  int result; // eax
  const char **v6; // esi
  int v7; // ebx
  const char *v8; // [esp+4h] [ebp-328h]
  char Buffer[788]; // [esp+14h] [ebp-318h] BYREF

  if ( !sub_464B80(v8) )
    return 12;
  sprintf(Buffer, "%s%s", a1, a3);
  result = sub_4A4790(a2, Buffer, a4, a5, 1.0);
  v6 = dword_506BE0 != 0 ? (const char **)dword_566630 : 0;
  v7 = result;
  if ( v6 )
  {
    do
    {
      if ( v7 != 12 )
        break;
      if ( sub_464B80(a1) )
      {
        sprintf(Buffer, "%s%s\\%s", a1, *v6, a3);
        v7 = sub_4A4790(a2, Buffer, a4, a5, 1.0);
      }
      v6 = (const char **)v6[1];
    }
    while ( v6 );
    return v7;
  }
  return result;
}

// ===== sub_493DB0 @ 0x00493DB0..0x00493F6B =====
int __usercall sub_493DB0@<eax>(const char *a1@<esi>, int a2, const char *a3, int a4, int a5)
{
  int result; // eax
  int v6; // edi
  char Buffer[784]; // [esp+20h] [ebp-628h] BYREF
  int v8[197]; // [esp+330h] [ebp-318h] BYREF

  sub_4A3420();
  result = sub_493CB0(&::Buffer, a2, a1, a4, a5);
  if ( result != 12 )
    return result;
  if ( !a3 )
  {
    while ( 1 )
    {
      result = sub_493CB0(byte_517C08, a2, a1, a4, a5);
      if ( result != 12 )
        break;
      sprintf(Buffer, &byte_4E6E44, a1);
      sub_465A20(Buffer);
    }
    return result;
  }
  sub_4649F0(a3, &::Buffer, Buffer);
  if ( !sub_4069D0((void *)dword_566754, (int)v8, (int)Buffer, (int)a1) )
  {
    v6 = 12;
    while ( 1 )
    {
LABEL_7:
      if ( sub_464B80(byte_517C08) )
      {
        sub_4649F0(a3, byte_517C08, Buffer);
        if ( !sub_4069D0((void *)dword_566754, (int)v8, (int)Buffer, (int)a1) )
        {
          v6 = 12;
          goto LABEL_11;
        }
        v6 = sub_4A4830(a2, (int)v8, (int)a1, a4, a5, 1.0);
      }
      if ( v6 != 12 )
        return v6;
LABEL_11:
      sprintf(Buffer, &byte_4E6E70, a3, a1);
      sub_465A20(Buffer);
    }
  }
  result = sub_4A4830(a2, (int)v8, (int)a1, a4, a5, 1.0);
  v6 = result;
  if ( result == 12 )
    goto LABEL_7;
  return result;
}

// ===== sub_493F70 @ 0x00493F70..0x004940D0 =====
int __usercall sub_493F70@<eax>(const char *a1@<edi>, int a2, const char *a3, const char *a4, int a5, int a6, int a7)
{
  int result; // eax
  const char **v8; // esi
  const char *v9; // [esp+4h] [ebp-63Ch]
  int i; // [esp+14h] [ebp-62Ch]
  char Buffer[784]; // [esp+18h] [ebp-628h] BYREF
  CHAR v12[788]; // [esp+328h] [ebp-318h] BYREF

  if ( !sub_464B80(v9) )
    return 12;
  sprintf(Buffer, "%s%s", a1, a3);
  sprintf(v12, "%s%s", a1, a4);
  result = sub_4A48E0(a2, Buffer, v12, a5, a6, a7, 1.0);
  v8 = dword_506BE0 != 0 ? (const char **)dword_566630 : 0;
  for ( i = result; v8; result = i )
  {
    if ( result != 12 )
      break;
    if ( sub_464B80(a1) )
    {
      sprintf(Buffer, "%s%s\\%s", a1, *v8, a3);
      sprintf(v12, "%s%s\\%s", a1, *v8, a4);
      i = sub_4A48E0(a2, Buffer, v12, a5, a6, a7, 1.0);
    }
    v8 = (const char **)v8[1];
  }
  return result;
}
