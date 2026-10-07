#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4940D0 @ 0x004940D0..0x004942A7 =====
int __usercall sub_4940D0@<eax>(const char *a1@<esi>, int a2, const char *a3, const char *a4, int a5, int a6, int a7)
{
  int result; // eax
  int v8; // edi
  char Buffer[784]; // [esp+20h] [ebp-628h] BYREF
  int v10[197]; // [esp+330h] [ebp-318h] BYREF

  sub_4A3420();
  result = sub_493F70(&::Buffer, a2, a1, a4, a5, a6, a7);
  if ( result != 12 )
    return result;
  if ( !a3 )
  {
    while ( 1 )
    {
      result = sub_493F70(byte_517C08, a2, a1, a4, a5, a6, a7);
      if ( result != 12 )
        break;
      sprintf(Buffer, &byte_4EC22C, a1, a4);
      sub_465A20(Buffer);
    }
    return result;
  }
  sub_4649F0(a3, &::Buffer, Buffer);
  if ( !sub_4069D0((void *)dword_566754, (int)v10, (int)Buffer, (int)a1) )
  {
    v8 = 12;
    while ( 1 )
    {
LABEL_7:
      if ( sub_464B80(byte_517C08) )
      {
        sub_4649F0(a3, byte_517C08, Buffer);
        if ( !sub_4069D0((void *)dword_566754, (int)v10, (int)Buffer, (int)a1) )
        {
          v8 = 12;
          goto LABEL_11;
        }
        v8 = sub_4A4990(a2, (int)v10, (int)a1, (int)a4, a5, a6, a7, 1.0);
      }
      if ( v8 != 12 )
        return v8;
LABEL_11:
      sprintf(Buffer, &byte_4EC25C, a3, a1, a4);
      sub_465A20(Buffer);
    }
  }
  result = sub_4A4990(a2, (int)v10, (int)a1, (int)a4, a5, a6, a7, 1.0);
  v8 = result;
  if ( result == 12 )
    goto LABEL_7;
  return result;
}

// ===== sub_4942B0 @ 0x004942B0..0x004942E0 =====
int __usercall sub_4942B0@<eax>(int a1@<edi>, unsigned int a2@<esi>)
{
  int result; // eax
  int v3; // [esp+0h] [ebp-4h] BYREF

  result = 0;
  v3 = 0;
  if ( a2 < 0x10 )
  {
    sub_4A2560(a2, &v3);
    if ( a1 )
      sub_4A25A0(a2, a1);
    return v3;
  }
  return result;
}

// ===== sub_4942E0 @ 0x004942E0..0x004942F5 =====
void *sub_4942E0()
{
  return memset(&unk_55EF18, 0, 0x1000u);
}

// ===== sub_494300 @ 0x00494300..0x00494373 =====
int __cdecl sub_494300(int a1, const void *a2, int a3, double a4, double a5)
{
  qmemcpy((char *)&unk_55EF18 + 64 * a1, a2, 0x40u);
  dword_55EF54[16 * a1] = (__int64)(65536.0 / a5);
  return sub_4A46F0(a1, (int)a2, a3, a4, a5);
}

// ===== sub_494380 @ 0x00494380..0x0049439E =====
int __usercall sub_494380@<eax>(int a1@<esi>)
{
  int v2; // [esp-Ch] [ebp-Ch]

  memset((char *)&unk_55EF18 + 64 * a1, 0, 0x40u);
  return sub_4A2710(a1, v2);
}

// ===== sub_4943A0 @ 0x004943A0..0x004943B3 =====
int __usercall sub_4943A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_4A3DB0(a3, a2, a1);
}

// ===== sub_4943C0 @ 0x004943C0..0x004943CA =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004943C0

// ===== sub_4943D0 @ 0x004943D0..0x004943DB =====
int __usercall sub_4943D0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4A30C0(a2, a1);
}

// ===== sub_4943E0 @ 0x004943E0..0x0049445D =====
unsigned int __fastcall sub_4943E0(int a1)
{
  int v1; // ecx
  unsigned int v2; // edx
  unsigned int result; // eax

  v1 = a1 << 6;
  v2 = *(int *)((char *)&dword_55EF28 + v1);
  result = 0;
  if ( v2 )
    return (unsigned int)(__int64)((double)*(unsigned int *)((char *)&dword_55EF24 + v1)
                                 * 1000.0
                                 / (double)v2
                                 * (double)*(unsigned int *)((char *)dword_55EF54 + v1)) >> 16;
  return result;
}

// ===== sub_494460 @ 0x00494460..0x004944A3 =====
int sub_494460()
{
  int i; // esi
  int j; // esi
  int result; // eax

  if ( !dword_566994 )
  {
    dword_566994 = 1;
    for ( i = 0; i < 16; ++i )
      sub_4A3270(i, 0);
    for ( j = 0; j < 64; ++j )
      result = sub_4A2DF0(j, 0);
  }
  return result;
}

// ===== sub_4944B0 @ 0x004944B0..0x004944C9 =====
int sub_4944B0()
{
  int result; // eax

  if ( dword_566994 )
  {
    result = sub_493BC0();
    dword_566994 = 0;
  }
  return result;
}

// ===== sub_4944D0 @ 0x004944D0..0x00494527 =====
BOOL __cdecl sub_4944D0(const char *a1)
{
  CHAR pszSound[780]; // [esp+0h] [ebp-310h] BYREF

  sprintf(pszSound, "%s%s", &Buffer, a1);
  return PlaySoundA(pszSound, hInst, 0x22003u);
}

// ===== sub_494530 @ 0x00494530..0x0049455F =====
int __cdecl sub_494530(double a1)
{
  if ( a1 < 0.0 )
    return (int)(a1 + -0.5);
  else
    return (int)(a1 + 0.5);
}

// ===== sub_494560 @ 0x00494560..0x00494727 =====
double __cdecl sub_494560(double *a1, double *a2, double *a3, int a4, int a5, int a6, int a7, double *a8, double a9)
{
  int v9; // esi
  int v10; // ecx
  double v11; // rt0

  v9 = -1;
  v10 = 1;
  while ( v9 < 0 )
  {
    if ( a9 < a1[v10] )
      v9 = v10 - 1;
    if ( ++v10 >= 2 )
    {
      if ( v9 < 0 )
        v9 = 1;
      break;
    }
  }
  *a3 = a1[1] - *a1;
  a3[1] = a1[2] - a1[1];
  *(double *)(a4 + 8) = (a3[1] + *a3) * 2.0;
  *(double *)(a5 + 8) = ((a2[2] - a2[1]) / a3[1] - (a2[1] - *a2) / *a3) * 3.0;
  *(double *)(a6 + 8) = a3[1] / *(double *)(a4 + 8);
  *(double *)(a7 + 8) = *(double *)(a5 + 8) / *(double *)(a4 + 8);
  *a8 = 0.0;
  a8[2] = 0.0;
  a8[1] = *(double *)(a7 + 8);
  v11 = a9 - a1[v9];
  return v11
       * ((a2[v9 + 1] - a2[v9]) / a3[v9]
        - (2.0 * a8[v9] + a8[v9 + 1]) * a3[v9] / 3.0
        + ((a8[v9 + 1] - a8[v9]) / (3.0 * a3[v9]) * v11 + a8[v9]) * v11)
       + a2[v9];
}

// ===== sub_494730 @ 0x00494730..0x00494926 =====
int __usercall sub_494730@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, unsigned int a8)
{
  int v8; // esi
  bool v9; // cc
  double *v11; // edi
  double *v12; // eax
  int v13; // ecx
  double *v14; // ebx
  int v15; // eax
  double v16; // st7
  unsigned int v17; // esi
  int v18; // eax
  double v19; // st7
  int v20; // eax
  double v21; // [esp+18h] [ebp-34h]
  double i; // [esp+20h] [ebp-2Ch]
  double *v23; // [esp+2Ch] [ebp-20h]
  void *v24; // [esp+30h] [ebp-1Ch]
  void *v25; // [esp+34h] [ebp-18h]
  void *v26; // [esp+38h] [ebp-14h]
  void *v27; // [esp+3Ch] [ebp-10h]
  double *v28; // [esp+40h] [ebp-Ch]
  int v29; // [esp+44h] [ebp-8h]

  v8 = a1;
  v9 = a3 <= a1;
  if ( a3 < a1 )
  {
    if ( a1 > a6 )
      return 0;
    v9 = a3 <= a1;
  }
  if ( !v9 && a1 < a6 )
    return 0;
  v28 = (double *)operator new[](0x10u);
  v27 = operator new[](0x10u);
  v26 = operator new[](0x10u);
  v25 = operator new[](0x10u);
  v24 = operator new[](0x10u);
  v23 = (double *)operator new[](0x18u);
  v11 = (double *)operator new[](0x18u);
  v12 = (double *)operator new[](0x18u);
  v13 = a3;
  v14 = v12;
  v15 = a6;
  if ( a3 >= v8 || v8 >= a6 )
  {
    v13 = -a3;
    v8 = -v8;
    v15 = -a6;
    a3 = -a3;
    v29 = 0;
  }
  else
  {
    v29 = 1;
  }
  *v11 = 0.0;
  v11[1] = (double)(v8 - v13);
  v11[2] = (double)(v15 - v13);
  *v14 = 0.0;
  v14[1] = (double)(a5 - a4);
  v14[2] = (double)(a7 - a4);
  v16 = *v11;
  v21 = v16;
  v17 = 0;
  for ( i = (v11[2] - v16) / (double)(a8 - 1); v17 < a8; v21 = i + v21 )
  {
    v18 = a3 + sub_494530(v16);
    if ( v29 != 1 )
      v18 = -v18;
    *(_DWORD *)(a2 + 8 * v17) = v18;
    v19 = sub_494560(v11, v14, v28, (int)v27, (int)v26, (int)v25, (int)v24, v23, v21);
    v20 = sub_494530(v19);
    v16 = i + v21;
    *(_DWORD *)(a2 + 8 * v17++ + 4) = a4 + v20;
  }
  operator delete[](v28);
  operator delete[](v27);
  operator delete[](v26);
  operator delete[](v25);
  operator delete[](v24);
  operator delete[](v23);
  operator delete[](v11);
  operator delete[](v14);
  return 1;
}

// ===== sub_494930 @ 0x00494930..0x004949BA =====
int sub_494930()
{
  _DWORD *v0; // esi
  _DWORD *v1; // eax
  _DWORD *v2; // eax

  v0 = operator new(0x10u);
  *v0 = dword_503E18++;
  v1 = operator new(0x2640u);
  if ( v1 )
    v2 = sub_444070(v1);
  else
    v2 = 0;
  v0[1] = v2;
  v0[3] = dword_503E24;
  dword_503E24 = v0;
  return *v0;
}

// ===== sub_4949C0 @ 0x004949C0..0x004949DC =====
void **__thiscall sub_4949C0(void *this)
{
  void **result; // eax

  result = (void **)dword_503E24;
  if ( dword_503E24 )
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

// ===== sub_4949E0 @ 0x004949E0..0x00494A26 =====
int __fastcall sub_4949E0(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = (int *)dword_503E24;
  result = 1;
  v4 = &dword_503E18;
  if ( dword_503E24 )
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
      (**v5)(v5, 1);
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_494A30 @ 0x00494A30..0x00494A51 =====
int *__thiscall sub_494A30(void *this)
{
  int *result; // eax

  for ( result = (int *)dword_503E24; dword_503E24; result = (int *)dword_503E24 )
    sub_4949E0((int)this, *result);
  return result;
}

// ===== sub_494A60 @ 0x00494A60..0x00494ADE =====
int __usercall sub_494A60@<eax>(unsigned int a1@<eax>, void *a2@<ecx>, int a3, unsigned int a4)
{
  int result; // eax
  void **v6; // eax
  void **v7; // ebx
  _DWORD *v8; // esi
  void *v9; // ecx
  unsigned int v10; // [esp+4h] [ebp-4h]

  result = 2;
  if ( a1 >= 2 )
  {
    if ( a4 >= 2 )
    {
      v6 = sub_4949C0(a2);
      v7 = v6;
      if ( v6 )
      {
        sub_4441D0((int)v6[1]);
        v8 = (_DWORD *)(a3 + 4);
        v10 = a1;
        do
        {
          sub_444270((int)v7[1], *(v8 - 1), *v8, v8[1]);
          v8 += 4;
          --v10;
        }
        while ( v10 );
        sub_4442C0((int)v7[1], a4);
        v7[2] = v9;
        return 0;
      }
      else
      {
        return 1;
      }
    }
    else
    {
      return 3;
    }
  }
  return result;
}

// ===== sub_494AE0 @ 0x00494AE0..0x00494B47 =====
int __usercall sub_494AE0@<eax>(void *a1@<ecx>, _DWORD *a2@<edi>)
{
  void **v2; // eax
  unsigned int v3; // edx
  int v4; // eax
  int v5; // ecx
  int v6; // edx
  int v8; // [esp+4h] [ebp-Ch] BYREF
  int v9; // [esp+8h] [ebp-8h]
  int v10; // [esp+Ch] [ebp-4h]

  v2 = sub_4949C0(a1);
  if ( !v2 )
    return 1;
  if ( v3 >= (unsigned int)v2[2] )
    return 4;
  v4 = (int)v2[1];
  v10 = 0;
  v9 = 0;
  v8 = 0;
  if ( !sub_4442E0(v4, v3, &v8) )
    return -1;
  v5 = v9;
  v6 = v10;
  *a2 = v8;
  a2[1] = v5;
  a2[2] = v6;
  return 0;
}

// ===== sub_494B50 @ 0x00494B50..0x00494B83 =====
unsigned int sub_494B50()
{
  unsigned int result; // eax
  __m128i *v1; // ecx
  __m128i v2; // xmm0

  result = 0;
  v1 = (__m128i *)&unk_55FF70;
  do
  {
    v2 = _mm_cvtsi32_si128((__int16)result);
    *v1 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v2, v2), 0);
    ++result;
    ++v1;
  }
  while ( result <= 0x100 );
  return result;
}

// ===== sub_494B90 @ 0x00494B90..0x00494D81 =====
int __cdecl sub_494B90(_DWORD *a1, _DWORD *a2, int a3, int a4)
{
  int result; // eax
  unsigned int v5; // edi
  unsigned int v6; // ebx
  unsigned int v7; // edx
  signed int v8; // ecx
  signed int v9; // esi
  unsigned int v10; // ebx
  int v11; // edx
  unsigned int v12; // eax
  unsigned int v13; // esi
  int v14; // ecx
  unsigned int v15; // edx
  unsigned int v16; // edi
  unsigned int v17; // ebx
  unsigned int v18; // edx
  bool v19; // zf
  unsigned int v20; // [esp+8h] [ebp-34h]
  unsigned int v21; // [esp+10h] [ebp-2Ch]
  unsigned int v22; // [esp+14h] [ebp-28h]
  int v23; // [esp+18h] [ebp-24h]
  int v24; // [esp+1Ch] [ebp-20h]
  unsigned int v25; // [esp+2Ch] [ebp-10h]
  unsigned int v26; // [esp+2Ch] [ebp-10h]
  unsigned int v27; // [esp+30h] [ebp-Ch]
  unsigned int v28; // [esp+34h] [ebp-8h]
  int v29; // [esp+34h] [ebp-8h]
  unsigned int v30; // [esp+38h] [ebp-4h]
  unsigned int v31; // [esp+44h] [ebp+8h]

  v27 = a2[2] << 16;
  result = a2[3];
  v28 = result << 16;
  if ( (int)(dword_503E04[a1[4]] & 0xFFFFFFF8) >= 24 )
  {
    result *= a4;
    v5 = (unsigned int)(a3 * a2[2] + 0x8000) >> 16;
    v6 = (unsigned int)(result + 0x8000) >> 16;
    v30 = v6;
    if ( v5 )
    {
      if ( v6 )
      {
        v7 = a1[2];
        v8 = (v7 >> 1) - ((unsigned int)(a3 * a2[2] + 0x8000) >> 17);
        v9 = (a1[3] >> 1) - ((unsigned int)(result + 0x8000) >> 17);
        v25 = v9 + v6;
        v21 = v8 + v5;
        v20 = v8 <= 0 ? 0 : v8;
        v10 = v9 <= 0 ? 0 : v9;
        if ( v8 + v5 > v7 )
          v21 = a1[2];
        v22 = v25;
        if ( v25 > a1[3] )
          v22 = a1[3];
        v24 = v27 / v5;
        v23 = v28 / v30;
        v13 = (__int64)((double)(int)(v10 - v9) / (double)v30 * (double)v28);
        v11 = a1[1];
        v12 = HIWORD(v13);
        v13 = (unsigned __int16)v13;
        v26 = (__int64)((double)(int)(v20 - v8) / (double)v5 * (double)v27);
        result = *a2 + a2[1] * v12;
        v14 = *a1 + v10 * v11;
        if ( v10 < v22 )
        {
          v29 = 4 * (v11 >> 2);
          v31 = v22 - v10;
          do
          {
            v15 = v20;
            if ( v20 < v21 )
            {
              v16 = v26;
              do
              {
                v17 = v16;
                v16 += v24;
                *(_DWORD *)(v14 + 4 * v15++) = *(_DWORD *)(result + 4 * HIWORD(v17));
              }
              while ( v15 < v21 );
            }
            v13 += v23;
            v14 += v29;
            v18 = (unsigned int)(a2[1] * HIWORD(v13)) >> 2;
            v13 = (unsigned __int16)v13;
            v19 = v31-- == 1;
            result += 4 * v18;
          }
          while ( !v19 );
        }
      }
    }
  }
  return result;
}

// ===== sub_494D90 @ 0x00494D90..0x00495069 =====
int __cdecl sub_494D90(int a1, unsigned int a2, unsigned int a3)
{
  _DWORD *v3; // ecx
  int result; // eax
  unsigned int v5; // edi
  unsigned int v6; // esi
  unsigned int v7; // edx
  int v8; // ebx
  int v9; // ecx
  unsigned int v10; // esi
  char v11; // cl
  char v12; // al
  unsigned int v13; // esi
  unsigned int v14; // ecx
  unsigned int v15; // edx
  int v16; // ecx
  double v17; // st6
  int v18; // esi
  int v19; // edi
  int v20; // ebx
  int v21; // esi
  unsigned int v22; // esi
  bool v23; // zf
  int v24; // [esp+10h] [ebp-64h]
  int v25; // [esp+1Ch] [ebp-58h]
  int v26; // [esp+20h] [ebp-54h]
  int v27; // [esp+24h] [ebp-50h]
  unsigned int v28; // [esp+28h] [ebp-4Ch]
  unsigned int v29; // [esp+2Ch] [ebp-48h]
  unsigned int v30; // [esp+30h] [ebp-44h]
  int (__cdecl *v31)(int, int, int, int, int, int, int); // [esp+38h] [ebp-3Ch]
  _DWORD *v32; // [esp+3Ch] [ebp-38h]
  int v33; // [esp+3Ch] [ebp-38h]
  LONG v34; // [esp+44h] [ebp-30h]
  int v35; // [esp+44h] [ebp-30h]
  unsigned int v36; // [esp+4Ch] [ebp-28h]
  int v37; // [esp+4Ch] [ebp-28h]
  unsigned int v38; // [esp+58h] [ebp-1Ch]
  LONG v39; // [esp+58h] [ebp-1Ch]
  unsigned int v40; // [esp+58h] [ebp-1Ch]
  struct tagRECT rc; // [esp+5Ch] [ebp-18h] BYREF

  result = a1;
  v5 = (a2 * *(_DWORD *)(a1 + 8) + 0x8000) >> 16;
  v6 = (a3 * *(_DWORD *)(a1 + 12) + 0x8000) >> 16;
  v32 = v3;
  v31 = sub_495320;
  v36 = v6;
  if ( v5 && v6 )
  {
    rc.left = 0;
    rc.top = 0;
    rc.right = 0x10000 / (a2 >> 8);
    v7 = v3[2];
    v38 = v3[3];
    rc.bottom = 0x10000 / (a3 >> 8);
    v8 = (v7 >> 1) - (v5 >> 1);
    v9 = (v38 >> 1) - (v6 >> 1);
    v10 = v9 + v6;
    v24 = v9;
    v27 = v8 <= 0 ? 0 : v8;
    v28 = v9 <= 0 ? 0 : v9;
    v29 = v8 + v5;
    if ( v8 + v5 > v7 )
      v29 = v7;
    v30 = v10;
    if ( v10 > v38 )
      v30 = v38;
    v39 = rc.right << 7;
    v34 = rc.bottom << 7;
    v11 = 8;
    v12 = 8;
    if ( (a2 >= 0x10000 || a2 <= 0x7FFF) && a3 - 0x8000 > 0x7FFF )
    {
      if ( a2 < 0x8000 || a3 < 0x8000 )
      {
        v31 = sub_495070;
      }
      else
      {
        SetRect(&rc, 0, 0, 512, 512);
        v11 = 9;
        v12 = 9;
        v39 = 0;
        v34 = 0;
      }
    }
    else
    {
      v31 = sub_4951B0;
    }
    v13 = (*(_DWORD *)(a1 + 8) - (rc.right >> v11)) << 16;
    v14 = (*(_DWORD *)(a1 + 12) - (rc.bottom >> v12)) << 16;
    v25 = v13 / v5;
    v26 = v14 / v36;
    v15 = v8 <= 0 ? 0 : v8;
    v16 = v34 + (__int64)((double)(int)(v28 - v24) / (double)v36 * (double)v14);
    v37 = v16;
    v17 = (double)v13;
    v18 = v32[1];
    v19 = v39 + (__int64)((double)(v27 - v8) / (double)v5 * v17);
    v20 = *v32 + v28 * v18;
    result = v30;
    v21 = v18 >> 2;
    v35 = v19;
    if ( v28 < v30 )
    {
      result = v30 - v28;
      v33 = 4 * v21;
      v40 = v30 - v28;
      while ( 1 )
      {
        v22 = v15;
        if ( v15 < v29 )
        {
          do
          {
            result = v31(a1, rc.left, rc.top, rc.right, rc.bottom, v19, v16);
            v19 += v25;
            v16 = v37;
            *(_DWORD *)(v20 + 4 * v22++) = result;
          }
          while ( v22 < v29 );
          v15 = v27;
        }
        v16 += v26;
        v20 += v33;
        v23 = v40-- == 1;
        v37 = v16;
        if ( v23 )
          break;
        v19 = v35;
      }
    }
  }
  return result;
}

// ===== sub_495070 @ 0x00495070..0x004951A7 =====
unsigned int __cdecl sub_495070(_DWORD *a1, int a2, int a3, int a4, int a5, unsigned int a6, unsigned int a7)
{
  unsigned int v7; // edi
  unsigned int top; // esi
  _DWORD *v9; // eax
  unsigned int bottom; // edx
  int v11; // ebx
  int v12; // ecx
  unsigned int left; // edx
  unsigned __int8 v14; // bl
  unsigned int v16; // [esp+14h] [ebp-30h]
  unsigned int v17; // [esp+18h] [ebp-2Ch]
  unsigned int v18; // [esp+1Ch] [ebp-28h]
  unsigned int v19; // [esp+20h] [ebp-24h]
  unsigned int v20; // [esp+24h] [ebp-20h]
  unsigned int v21; // [esp+28h] [ebp-1Ch]
  struct tagRECT rc; // [esp+2Ch] [ebp-18h] BYREF

  v7 = 0;
  v18 = 0;
  SetRect(&rc, HIWORD(a6), HIWORD(a7), HIWORD(a6) + ((a4 + 255) >> 8), HIWORD(a7) + ((a5 + 255) >> 8));
  if ( rc.left < 0 )
    rc.left = 0;
  top = rc.top;
  if ( rc.top < 0 )
  {
    top = 0;
    rc.top = 0;
  }
  v9 = a1;
  if ( rc.right > a1[2] )
    rc.right = a1[2];
  bottom = rc.bottom;
  if ( rc.bottom > a1[3] )
  {
    bottom = a1[3];
    rc.bottom = bottom;
  }
  v11 = a1[1];
  v12 = *a1 + top * v11;
  v19 = 0;
  v21 = 0;
  v20 = 0;
  v17 = 0;
  if ( top >= bottom )
    goto LABEL_20;
  v16 = bottom - top;
  do
  {
    left = rc.left;
    if ( rc.left < (unsigned int)rc.right )
    {
      v18 += rc.right - rc.left;
      do
      {
        v14 = *(_BYTE *)(v12 + 4 * left + 3);
        if ( v14 || v9[4] == 1 )
        {
          v20 += *(unsigned __int8 *)(v12 + 4 * left);
          v21 += *(unsigned __int8 *)(v12 + 4 * left + 1);
          v19 += *(unsigned __int8 *)(v12 + 4 * left + 2);
          v17 += v14;
          v9 = a1;
          ++v7;
        }
        ++left;
      }
      while ( left < rc.right );
      v11 = a1[1];
    }
    v12 += v11;
    --v16;
  }
  while ( v16 );
  if ( (int)v7 <= 0 )
LABEL_20:
    v7 = 1;
  return (((v21 / v7) | (((v19 / v7) | ((v17 / v18) << 8)) << 8)) << 8) | (v20 / v7);
}

// ===== sub_4951B0 @ 0x004951B0..0x0049531E =====
int __cdecl sub_4951B0(_DWORD *a1, int a2, int a3, int a4, int a5, unsigned int a6, unsigned int a7)
{
  LONG top; // edx
  unsigned int right; // edi
  unsigned int bottom; // ecx
  unsigned int v10; // esi
  int v11; // esi
  __m128i v12; // xmm0
  unsigned int v13; // eax
  unsigned int v14; // ebx
  unsigned int left; // edx
  unsigned int v16; // ecx
  unsigned int v17; // eax
  __m128i v18; // xmm2
  unsigned int v20; // [esp+10h] [ebp-20h]
  unsigned int v21; // [esp+14h] [ebp-1Ch]
  struct tagRECT rc; // [esp+1Ch] [ebp-14h] BYREF

  SetRect(&rc, a6 >> 8, a7 >> 8, (a6 >> 8) + a4, (a7 >> 8) + a5);
  if ( rc.left < 0 )
    rc.left = 0;
  top = rc.top;
  if ( rc.top < 0 )
  {
    top = 0;
    rc.top = 0;
  }
  right = rc.right;
  if ( rc.right > a1[2] << 8 )
  {
    right = a1[2] << 8;
    rc.right = right;
  }
  bottom = rc.bottom;
  v10 = a1[3] << 8;
  if ( rc.bottom > v10 )
  {
    rc.bottom = a1[3] << 8;
    bottom = v10;
  }
  v11 = *a1 + a1[1] * (top >> 8);
  v21 = (int)((right - rc.left) * (bottom - top)) >> 8;
  v12 = 0LL;
  if ( top < bottom )
  {
    do
    {
      v13 = (top + 256) & 0xFFFFFF00;
      v20 = v13;
      if ( v13 > bottom )
      {
        v20 = bottom;
        v13 = bottom;
      }
      v14 = v13 - top;
      left = rc.left;
      if ( rc.left < right )
      {
        do
        {
          v16 = (left + 256) & 0xFFFFFF00;
          if ( v16 > right )
            v16 = right;
          v17 = ((v14 * (v16 - left)) << 8) / v21;
          v18 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(((left >> 6) & 0x3FFFFFC) + v11)), (__m128i)0LL);
          left = v16;
          v12 = _mm_adds_epu16(v12, _mm_mullo_epi16(v18, _mm_load_si128(&xmmword_55FF70[v17 >> 8])));
        }
        while ( v16 < right );
        bottom = rc.bottom;
        v13 = v20;
      }
      v11 += a1[1];
      top = v13;
    }
    while ( v13 < bottom );
  }
  return _mm_cvtsi128_si32(_mm_packus_epi16(_mm_srli_epi16(v12, 8u), (__m128i)0LL));
}

// ===== sub_495320 @ 0x00495320..0x0049549C =====
unsigned int __cdecl sub_495320(_DWORD *a1, int a2, int a3, int a4, int a5, unsigned int a6, unsigned int a7)
{
  int v7; // edx
  unsigned int v8; // edi
  unsigned int v9; // eax
  unsigned __int8 *v10; // ecx
  int v11; // esi
  int v13; // [esp+Ch] [ebp-10h]
  int v14; // [esp+10h] [ebp-Ch]
  int v15; // [esp+14h] [ebp-8h]
  unsigned __int8 *v16; // [esp+18h] [ebp-4h]
  unsigned __int8 *v17; // [esp+38h] [ebp+1Ch]
  unsigned __int8 *v18; // [esp+3Ch] [ebp+20h]

  v7 = BYTE1(a6);
  v14 = BYTE1(a7);
  v15 = 256 - BYTE1(a7);
  v8 = HIWORD(a7);
  v9 = HIWORD(a6);
  v10 = (unsigned __int8 *)(*a1 + HIWORD(a7) * a1[1] + 4 * HIWORD(a6));
  v18 = v10 + 4;
  v16 = &v10[a1[1]];
  v17 = v16 + 4;
  if ( v9 >= a1[2] - 1 )
  {
    v18 = (unsigned __int8 *)&unk_566ACC;
    v17 = (unsigned __int8 *)&unk_566ACC;
  }
  if ( v8 >= a1[3] - 1 )
  {
    v16 = (unsigned __int8 *)&unk_566ACC;
    v17 = (unsigned __int8 *)&unk_566ACC;
  }
  v11 = 256 - v7;
  v13 = 256 - v7;
  return ((((v14 * (v13 * *v16 + v7 * *v17) + v15 * (v13 * *v10 + v7 * (unsigned int)*v18)) >> 8) | (v15 * (v7 * v18[1] + v13 * v10[1]) + v14 * (v7 * v17[1] + v13 * v16[1])) & 0xFFFF00FF) >> 8) | (v15 * (v7 * v18[2] + v11 * v10[2]) + v14 * (v7 * v17[2] + v11 * v16[2])) & 0xFFFF0000 | ((v15 * (v7 * v18[3] + v11 * v10[3]) + v14 * (v7 * v17[3] + v11 * v16[3])) << 8) & 0xFF000000;
}

// ===== sub_4954A0 @ 0x004954A0..0x004954D5 =====
_DWORD *__usercall sub_4954A0@<eax>(int a1@<ecx>, int a2@<edi>)
{
  _DWORD *v2; // esi
  _DWORD *result; // eax

  v2 = &unk_503DF0;
  if ( dword_503E00 )
  {
    do
    {
      if ( a2 && (result = (_DWORD *)v2[4], *result == 0x80000000) )
        v2 = (_DWORD *)v2[4];
      else
        result = (_DWORD *)sub_4954E0(a1, *(_DWORD *)v2[4]);
    }
    while ( v2[4] );
  }
  return result;
}

// ===== sub_4954E0 @ 0x004954E0..0x00495546 =====
int __fastcall sub_4954E0(int a1, void *a2)
{
  void **v2; // esi
  int result; // eax
  void **v4; // ecx
  int v5; // edi
  int v6; // ebx

  v2 = (void **)dword_503E00;
  result = 0;
  v4 = (void **)&unk_503DF0;
  if ( dword_503E00 )
  {
    while ( a2 != *v2 )
    {
      v4 = v2;
      v2 = (void **)v2[4];
      if ( !v2 )
        return result;
    }
    v5 = 0;
    v4[4] = v2[4];
    if ( (int)v2[2] > 0 )
    {
      v6 = 0;
      do
      {
        operator delete[](*(void **)((char *)v2[3] + v6 + 8));
        ++v5;
        v6 += 12;
      }
      while ( v5 < (int)v2[2] );
    }
    operator delete[](v2[3]);
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_495550 @ 0x00495550..0x00495640 =====
int __usercall sub_495550@<eax>(int a1@<eax>, int a2@<ecx>, void *a3, char *Src)
{
  int result; // eax
  int i; // edi
  _DWORD *v7; // ebx
  void *v8; // eax
  int v9; // ecx
  char *v10; // edi
  int *v11; // esi
  unsigned int v12; // kr00_4
  void *v13; // eax
  size_t v14; // [esp-10h] [ebp-1Ch]
  int v15; // [esp+4h] [ebp-8h]
  int v16; // [esp+8h] [ebp-4h]

  result = 0;
  if ( a1 )
  {
    if ( a1 >= 1 && Src )
    {
      sub_4954E0(a2, a3);
      for ( i = 32; i < a1; i *= 2 )
        ;
      v7 = operator new(0x14u);
      *v7 = a3;
      v7[1] = i;
      v7[2] = a1;
      v8 = operator new[](12 * i);
      v10 = Src;
      v7[3] = v8;
      v7[4] = dword_503E00;
      dword_503E00 = v7;
      v16 = 0;
      v15 = a1;
      do
      {
        v11 = (int *)(v16 + v7[3]);
        *v11 = sub_452AE0(v9, v10);
        v12 = strlen(v10);
        v11[1] = v12 + 1;
        v13 = operator new[](v12 + 1);
        v14 = v11[1];
        v11[2] = (int)v13;
        memcpy_0(v13, v10, v14);
        v10 += v11[1];
        v16 += 12;
        --v15;
      }
      while ( v15 );
      return 1;
    }
  }
  else
  {
    sub_4954E0(a2, a3);
    return 1;
  }
  return result;
}

// ===== sub_495640 @ 0x00495640..0x004956B5 =====
int __usercall sub_495640@<eax>(int a1@<eax>, char *a2)
{
  _DWORD *v2; // edi
  int v3; // ebx
  int result; // eax
  void *v5; // ecx
  int v6; // esi
  int v7; // [esp+8h] [ebp-Ch]
  int v8; // [esp+Ch] [ebp-8h]
  char *v9; // [esp+10h] [ebp-4h]

  v2 = dword_503E00;
  v3 = 0;
  if ( dword_503E00 )
  {
    do
    {
      if ( a1 == *v2 )
        break;
      v2 = (_DWORD *)v2[4];
    }
    while ( v2 );
  }
  result = 0;
  v7 = 0;
  if ( v2 )
  {
    v5 = a2;
    v9 = a2;
    if ( (int)v2[2] > 0 )
    {
      v8 = 0;
      do
      {
        v6 = v8 + v2[3];
        if ( v5 )
        {
          memcpy_0(v5, *(const void **)(v6 + 8), *(_DWORD *)(v6 + 4));
          v9 += *(_DWORD *)(v6 + 4);
          v5 = v9;
          result = v7;
        }
        result += *(_DWORD *)(v6 + 4);
        v8 += 12;
        ++v3;
        v7 = result;
      }
      while ( v3 < v2[2] );
    }
  }
  return result;
}

// ===== sub_4956C0 @ 0x004956C0..0x004956E0 =====
int __fastcall sub_4956C0(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_503E00;
  result = 0;
  if ( dword_503E00 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[4];
      if ( !v2 )
        return result;
    }
    return v2[2];
  }
  return result;
}

// ===== sub_4956E0 @ 0x004956E0..0x00495841 =====
int __usercall sub_4956E0@<eax>(int a1@<eax>, int a2@<ecx>, char *Src)
{
  _DWORD *v3; // esi
  int v5; // edi
  int v6; // eax
  int v7; // eax
  int v8; // eax
  void *v9; // ebx
  int *v10; // ebx
  unsigned int v11; // kr00_4
  void *v12; // eax
  int result; // eax
  size_t v14; // [esp-8h] [ebp-18h]
  int v15; // [esp+Ch] [ebp-4h]

  v3 = dword_503E00;
  if ( dword_503E00 )
  {
    while ( a1 != *v3 )
    {
      v3 = (_DWORD *)v3[4];
      if ( !v3 )
        goto LABEL_4;
    }
  }
  else
  {
LABEL_4:
    v3 = operator new(0x14u);
    v3[1] = 32;
    *v3 = a1;
    v3[2] = 0;
    v3[3] = operator new[](0x180u);
    v3[4] = dword_503E00;
    dword_503E00 = v3;
  }
  v5 = 0;
  v15 = sub_452AE0(a2, Src);
  if ( (int)v3[2] > 0 )
  {
    v6 = v3[3];
    while ( v15 != *(_DWORD *)v6 || strcmp(Src, *(const char **)(v6 + 8)) )
    {
      ++v5;
      v6 += 12;
      if ( v5 >= v3[2] )
        goto LABEL_12;
    }
    if ( v5 != -1 )
      return v5;
  }
LABEL_12:
  v7 = v3[1];
  if ( v3[2] == v7 )
  {
    v8 = 2 * v7;
    v3[1] = v8;
    v9 = operator new[](12 * v8);
    memcpy_0(v9, (const void *)v3[3], 12 * v3[2]);
    operator delete[]((void *)v3[3]);
    v3[3] = v9;
  }
  v10 = (int *)(v3[3] + 12 * v3[2]);
  *v10 = v15;
  v11 = strlen(Src);
  v10[1] = v11 + 1;
  v12 = operator new[](v11 + 1);
  v14 = v10[1];
  v10[2] = (int)v12;
  memcpy_0(v12, Src, v14);
  result = v3[2];
  v3[2] = result + 1;
  return result;
}

// ===== sub_495850 @ 0x00495850..0x004958BD =====
int __usercall sub_495850@<eax>(void *a1@<ecx>, int a2@<eax>, int a3@<edi>, _DWORD *a4)
{
  _DWORD *v4; // esi
  int result; // eax

  v4 = dword_503E00;
  if ( dword_503E00 )
  {
    do
    {
      if ( a2 == *v4 )
        break;
      v4 = (_DWORD *)v4[4];
    }
    while ( v4 );
  }
  result = -2147483647;
  if ( v4 )
  {
    if ( a3 >= v4[2] || a3 < 0 )
    {
      return -2147483646;
    }
    else
    {
      if ( a1 )
        memcpy_0(a1, *(const void **)(v4[3] + 12 * a3 + 8), *(_DWORD *)(v4[3] + 12 * a3 + 4));
      if ( a4 )
        *a4 = *(_DWORD *)(v4[3] + 12 * a3 + 4) - 1;
      return 0;
    }
  }
  return result;
}

// ===== sub_4958C0 @ 0x004958C0..0x00495961 =====
int __cdecl sub_4958C0(char *a1)
{
  int v1; // ecx
  _DWORD *v2; // esi
  int result; // eax
  int v4; // eax
  int v5; // edi
  int v6; // eax
  int i; // esi
  int v8; // [esp+4h] [ebp-4h]

  v2 = dword_503E00;
  result = -2147483647;
  if ( dword_503E00 )
  {
    while ( *v2 != 0x80000000 )
    {
      v2 = (_DWORD *)v2[4];
      if ( !v2 )
        return result;
    }
    v4 = sub_452AE0(v1, a1);
    v5 = v2[2];
    v8 = v4;
    v6 = 0;
    if ( v5 <= 0 )
    {
      return 1;
    }
    else
    {
      for ( i = v2[3]; v8 != *(_DWORD *)i || strcmp(a1, *(const char **)(i + 8)); i += 12 )
      {
        if ( ++v6 >= v5 )
          return 1;
      }
      return 0;
    }
  }
  return result;
}

// ===== sub_495970 @ 0x00495970..0x00495985 =====
BOOL __usercall sub_495970@<eax>(char a1@<al>)
{
  return a1 < -96 || (unsigned __int8)a1 >= 0xE0u;
}

// ===== sub_495990 @ 0x00495990..0x004959AC =====
BOOL __usercall sub_495990@<eax>(char *a1@<edx>, int *a2@<esi>)
{
  BOOL result; // eax
  int v3; // edx
  int v4; // ecx

  result = sub_495970(*a1);
  v4 = (unsigned __int8)v4;
  if ( result )
    v4 = *(unsigned __int8 *)(v3 + 1) | ((unsigned __int8)v4 << 8);
  *a2 = v4;
  return result;
}

// ===== sub_4959B0 @ 0x004959B0..0x00495A7C =====
int __usercall sub_4959B0@<eax>(const char *a1@<eax>, char *a2)
{
  char *v3; // edi
  void *v4; // eax
  bool v5; // zf
  int *v6; // esi
  int v7; // edi
  int v8; // eax
  int v10; // [esp+Ch] [ebp-10h] BYREF
  int v11; // [esp+10h] [ebp-Ch]
  void *v12; // [esp+14h] [ebp-8h]
  int v13; // [esp+18h] [ebp-4h]
  int v14; // [esp+24h] [ebp+8h]

  v3 = (char *)a1;
  v4 = operator new(4 * strlen(a1));
  v5 = *v3 == 0;
  v12 = v4;
  v14 = 0;
  if ( !v5 )
  {
    v6 = (int *)v4;
    do
    {
      ++v14;
      v3 += sub_495990(v3, v6++) + 1;
    }
    while ( *v3 );
  }
  v7 = 0;
  v5 = *a2 == 0;
  v13 = 0;
  v11 = -1;
  if ( !v5 )
  {
    do
    {
      v8 = sub_495990(a2, &v10) + 1;
      if ( v10 == *((_DWORD *)v12 + v7) )
      {
        if ( !v7 )
          v11 = v13;
        if ( ++v7 >= v14 )
          break;
      }
      else
      {
        v7 = 0;
      }
      v13 += v8;
      a2 += v8;
    }
    while ( *a2 );
  }
  operator delete(v12);
  if ( *a2 )
    return v11;
  else
    return -1;
}

// ===== sub_495A80 @ 0x00495A80..0x00495B23 =====
char *__usercall sub_495A80@<eax>(char *a1@<eax>, char *a2, const char *a3, const char *a4)
{
  signed int v6; // esi
  bool i; // cc
  char *v8; // eax
  int v9; // ebx
  char v10; // cl
  unsigned int v12; // [esp+8h] [ebp-8h]
  unsigned int v13; // [esp+Ch] [ebp-4h]
  char *v14; // [esp+18h] [ebp+8h]

  v12 = strlen(a3);
  v13 = strlen(a4);
  v14 = 0;
  v6 = sub_4959B0(a3, a1);
  for ( i = v6 <= 0; v6 >= 0; i = v6 <= 0 )
  {
    if ( !i )
    {
      memcpy_0(a2, a1, v6);
      a2 += v6;
      a1 += v6;
    }
    strcpy(a2, a4);
    a1 += v12;
    a2 += v13;
    ++v14;
    v6 = sub_4959B0(a3, a1);
  }
  v8 = a1;
  v9 = a2 - a1;
  do
  {
    v10 = *v8;
    v8[v9] = *v8;
    ++v8;
  }
  while ( v10 );
  return v14;
}

// ===== sub_495B30 @ 0x00495B30..0x00495BDE =====
void __usercall sub_495B30(const char *a1@<eax>, char *a2)
{
  char *v3; // eax
  const char *i; // ebx
  const char *j; // esi
  int v6; // ecx
  const char *v7; // ecx
  int v8; // edi
  char v9; // dl

  v3 = (char *)operator new(strlen(a1) + 1);
  strcpy(v3, a1);
  for ( i = v3; *i == 32; ++i )
    ;
  for ( j = i; *j; ++j )
  {
    if ( *j == 32 )
    {
      v6 = 0;
      if ( j[1] == 32 )
      {
        do
          ++v6;
        while ( j[v6 + 1] == 32 );
        if ( v6 > 0 )
        {
          v7 = &j[v6];
          v8 = j - v7;
          do
          {
            v9 = *v7;
            v7[v8] = *v7;
            ++v7;
          }
          while ( v9 );
        }
      }
    }
  }
  if ( *(j - 1) == 32 )
    *((_BYTE *)j - 1) = 0;
  strcpy(a2, i);
  operator delete(v3);
}

// ===== sub_495BE0 @ 0x00495BE0..0x00495C48 =====
int __usercall sub_495BE0@<eax>(unsigned __int16 *a1@<eax>, _WORD *a2@<edx>)
{
  unsigned __int16 *v2; // esi
  int v3; // edi
  unsigned __int8 v4; // cl

  v2 = a1;
  v3 = 0;
  if ( !a1 )
    return -1;
  if ( *(_BYTE *)a1 )
  {
    do
    {
      v4 = *(_BYTE *)v2;
      if ( (*(_BYTE *)v2 < 0x81u || v4 > 0x9Fu) && (unsigned __int8)(v4 + 32) > 0x1Cu )
      {
        if ( a2 )
          *a2++ = v4;
        v2 = (unsigned __int16 *)((char *)v2 + 1);
      }
      else
      {
        if ( a2 )
          *a2++ = _byteswap_ushort(*v2);
        ++v2;
      }
      ++v3;
    }
    while ( *(_BYTE *)v2 );
  }
  if ( a2 )
    *a2 = 0;
  return v3;
}

// ===== sub_495C50 @ 0x00495C50..0x00495E58 =====
int __usercall sub_495C50@<eax>(_WORD *a1@<eax>, _WORD *a2@<ecx>)
{
  int v3; // ecx
  int v4; // ebx
  _WORD *v5; // esi
  _WORD *v6; // eax
  _WORD *v7; // eax
  _DWORD *v8; // ecx
  int v9; // eax
  BOOL v10; // ebx
  int v11; // ebx
  int v12; // esi
  char *v13; // edi
  int v14; // edx
  int v15; // ecx
  int v16; // ebx
  int v17; // eax
  int v18; // edx
  int v19; // ecx
  int v20; // esi
  int v21; // esi
  int v23; // [esp+10h] [ebp-24h]
  _DWORD *v24; // [esp+14h] [ebp-20h]
  char *v25; // [esp+18h] [ebp-1Ch]
  int v26; // [esp+1Ch] [ebp-18h]
  int v27; // [esp+1Ch] [ebp-18h]
  int v28; // [esp+20h] [ebp-14h]
  int v29; // [esp+24h] [ebp-10h]
  int v30; // [esp+28h] [ebp-Ch]
  int v31; // [esp+2Ch] [ebp-8h]
  int v32; // [esp+30h] [ebp-4h]

  v3 = 0;
  v4 = 0;
  v5 = a1;
  v32 = 0;
  v31 = 0;
  if ( !a2 || !a1 )
    return -1;
  v6 = a2;
  if ( *a2 )
  {
    do
    {
      ++v6;
      ++v3;
    }
    while ( *v6 );
    v32 = v3;
  }
  v7 = v5;
  if ( *v5 )
  {
    do
    {
      ++v7;
      ++v4;
    }
    while ( *v7 );
    v31 = v4;
  }
  if ( !v3 )
    return 0;
  v24 = operator new(4 * v3 * v4);
  v8 = v24;
  if ( v4 > 0 )
  {
    v26 = v4;
    do
    {
      v9 = 0;
      if ( v32 > 0 )
      {
        do
        {
          v10 = a2[v9++] == *v5;
          *v8++ = v10;
        }
        while ( v9 < v32 );
        v4 = v31;
      }
      ++v5;
      --v26;
    }
    while ( v26 );
  }
  v11 = v4 + 1;
  v12 = v32 + 1;
  v13 = (char *)operator new(4 * (v32 + 1) * v11);
  v25 = v13;
  memset(v13, 0, 4 * (v32 + 1) * (v31 + 1));
  v14 = v32;
  v15 = 0;
  v29 = 0;
  if ( v11 > 0 )
  {
    v28 = -v32;
    v16 = 0;
    v30 = -4 * v12;
    v27 = 0;
    v23 = 4 * v12;
    do
    {
      v17 = 0;
      if ( v12 > 0 )
      {
        do
        {
          if ( v17 || v15 )
          {
            v18 = -1;
            v19 = -1;
            v20 = -1;
            if ( v17 > 0 )
              v18 = *(_DWORD *)&v13[4 * v17 - 4 + 4 * v16];
            if ( v29 > 0 )
              v19 = *(_DWORD *)&v13[4 * v17 + v30];
            if ( v17 > 0 && v29 > 0 )
            {
              v13 = v25;
              if ( v24[v17 - 1 + v28] )
                v20 = *(_DWORD *)&v25[4 * v17 - 4 + v30];
            }
            if ( v18 >= 0 && (v19 < 0 || v18 < v19) )
              v19 = v18;
            if ( v20 >= 0 && v20 < v19 )
              v19 = v20;
            v16 = v27;
            *(_DWORD *)&v13[4 * v27 + 4 * v17] = v19 + 1;
            v14 = v32;
            v15 = v29;
          }
          ++v17;
          v12 = v14 + 1;
        }
        while ( v17 < v14 + 1 );
      }
      v30 += v23;
      v28 += v14;
      ++v15;
      v16 = v12 + v27;
      v29 = v15;
      v27 += v12;
    }
    while ( v15 < v31 + 1 );
  }
  v21 = v14 + v31 - *(_DWORD *)&v13[4 * (v32 + 1) * (v31 + 1) - 4];
  operator delete(v13);
  operator delete(v24);
  return v21;
}

// ===== sub_495E60 @ 0x00495E60..0x00495E8E =====
void sub_495E60()
{
  if ( !lpAddress )
  {
    InitializeCriticalSection(&stru_55FF58);
    lpAddress = VirtualAlloc(0, 0x6000000u, 0x102000u, 4u);
  }
}

// ===== sub_495E90 @ 0x00495E90..0x00495ED8 =====
void sub_495E90()
{
  if ( lpAddress )
  {
    EnterCriticalSection(&stru_55FF58);
    VirtualFree(lpAddress, 0, 0x8000u);
    lpAddress = 0;
    LeaveCriticalSection(&stru_55FF58);
    DeleteCriticalSection(&stru_55FF58);
  }
}

// ===== sub_495EE0 @ 0x00495EE0..0x00495FE2 =====
void __cdecl __noreturn sub_495EE0(void *a1)
{
  SIZE_T v1; // ebx
  _DWORD *v2; // edi
  int v3; // [esp+0h] [ebp-24h] BYREF
  LPVOID lpAddress; // [esp+10h] [ebp-14h]
  int *v5; // [esp+14h] [ebp-10h]
  int v6; // [esp+20h] [ebp-4h]

  v5 = &v3;
  EnterCriticalSection(&stru_55FF58);
  v6 = 0;
  v1 = ((unsigned int)(3 * *((_DWORD *)a1 + 5) * *((_DWORD *)a1 + 6)) >> 1) + 24;
  v2 = VirtualAlloc(::lpAddress, v1, 0x1000u, 4u);
  lpAddress = v2;
  if ( v2 )
  {
    sub_447140((int)v2, v2, (_DWORD *)a1 + 3, *((char **)a1 + 4), *((_DWORD *)a1 + 5), *((_DWORD *)a1 + 6));
    *((_DWORD *)a1 + 3) = sub_493980(*((_DWORD *)a1 + 2), *((_DWORD *)a1 + 3), v2);
    VirtualFree(lpAddress, v1, 0x4000u);
  }
  else
  {
    sub_464500((int)&unk_4EC290);
    *((_DWORD *)a1 + 3) = 0;
  }
  v6 = -1;
  *((_DWORD *)a1 + 1) = 1;
  LeaveCriticalSection(&stru_55FF58);
  _endthread();
}

// ===== sub_496000 @ 0x00496000..0x00496066 =====
_DWORD *__cdecl sub_496000(int a1, int a2, int a3, int a4)
{
  _DWORD *v4; // esi
  void *v5; // eax

  v4 = operator new(0x1Cu);
  v4[2] = a1;
  v4[1] = 0;
  v4[3] = 0;
  v4[4] = a2;
  v4[5] = a3;
  v4[6] = a4;
  v5 = (void *)_beginthread((_beginthread_proc_type)sub_495EE0, 0, v4);
  if ( v5 == (void *)-1 )
  {
    operator delete(v4);
    return 0;
  }
  else
  {
    *v4 = v5;
    SetThreadPriority(v5, 2);
    return v4;
  }
}

// ===== sub_496070 @ 0x00496070..0x0049609E =====
_DWORD *__thiscall sub_496070(void *this)
{
  _DWORD *result; // eax

  for ( result = dword_5669A4; dword_5669A4; result = dword_5669A4 )
    sub_496150(this, *result);
  dword_5669A4 = 0;
  dword_56699C = 0;
  return result;
}

// ===== sub_4960A0 @ 0x004960A0..0x00496148 =====
int __usercall sub_4960A0@<eax>(unsigned int a1@<edi>, _DWORD *a2)
{
  _DWORD *v2; // esi
  int *v3; // eax
  int *v4; // eax

  if ( a1 <= 1 )
    return -2147483647;
  v2 = operator new(0xCu);
  *v2 = ++dword_56699C;
  v3 = (int *)operator new(0x14u);
  if ( v3 )
    v4 = sub_452830(v3, a1);
  else
    v4 = 0;
  v2[1] = v4;
  v2[2] = dword_5669A4;
  dword_5669A4 = v2;
  *a2 = *v2;
  return 0;
}

// ===== sub_496150 @ 0x00496150..0x0049619E =====
int __fastcall sub_496150(int a1, int a2)
{
  int *v2; // edi
  int result; // eax
  int *v4; // ecx
  void *v5; // esi

  v2 = (int *)dword_5669A4;
  result = -2147483646;
  v4 = &dword_56699C;
  if ( dword_5669A4 )
  {
    while ( a2 != *v2 )
    {
      v4 = v2;
      v2 = (int *)v2[2];
      if ( !v2 )
        return result;
    }
    v4[2] = v2[2];
    v5 = (void *)v2[1];
    if ( v5 )
    {
      sub_452850((int)v5);
      operator delete(v5);
    }
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_4961A0 @ 0x004961A0..0x004961BC =====
void **__thiscall sub_4961A0(void *this)
{
  void **result; // eax

  result = (void **)dword_5669A4;
  if ( dword_5669A4 )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[2];
    }
    while ( result );
  }
  return result;
}

// ===== sub_4961C0 @ 0x004961C0..0x004961E5 =====
int __cdecl sub_4961C0(void *Src)
{
  void *v1; // ecx
  void **v2; // eax
  const char *v3; // edx

  v2 = sub_4961A0(v1);
  if ( !v2 )
    return -2147483646;
  sub_452880(Src, (size_t *)v2[1], v3, Src);
  return 0;
}

// ===== sub_4961F0 @ 0x004961F0..0x00496211 =====
unsigned int __usercall sub_4961F0@<eax>(void *a1@<ecx>, const char *a2@<edi>)
{
  void **v2; // eax
  int v3; // ecx

  v2 = sub_4961A0(a1);
  if ( v2 )
    return sub_452960((int)v2[1], v3, a2) != 0 ? 0x80000003 : 0;
  else
    return -2147483646;
}

// ===== sub_496220 @ 0x00496220..0x0049626C =====
unsigned int __cdecl sub_496220(const char *a1, int a2)
{
  void *v2; // ecx
  void **v3; // eax
  void *v4; // edx
  int v5; // ecx

  v3 = sub_4961A0(v2);
  if ( !v3 )
    return -2147483646;
  if ( a1 )
    return sub_452A00(v5, a1, (size_t *)v3[1], v4) != 0 ? 0x80000003 : 0;
  return sub_452AA0(a2, (size_t *)v3[1], v4) != 0 ? 0x80000003 : 0;
}

// ===== sub_496270 @ 0x00496270..0x0049629C =====
_DWORD *sub_496270()
{
  _DWORD *result; // eax

  for ( result = dword_5669B8; dword_5669B8; result = dword_5669B8 )
    sub_496300(result[1]);
  dword_5669A8 = 0;
  return result;
}

// ===== sub_4962A0 @ 0x004962A0..0x004962F7 =====
BOOL __usercall sub_4962A0@<eax>(int a1@<edi>)
{
  void *v1; // eax
  void *v2; // esi
  _DWORD *v3; // eax

  v1 = (void *)sub_43E5D0(a1, (int)dword_56674C);
  v2 = v1;
  if ( v1 )
  {
    sub_46D6E0(v1);
    v3 = operator new(0x14u);
    *v3 = dword_5669A8;
    v3[1] = a1;
    v3[2] = v2;
    v3[3] = 0;
    v3[4] = dword_5669B8;
    ++dword_5669A8;
    dword_5669B8 = v3;
  }
  return v2 != 0;
}

// ===== sub_496300 @ 0x00496300..0x00496345 =====
int __thiscall sub_496300(void *this)
{
  int *v1; // edi
  int result; // eax
  int *v3; // ebx

  v1 = (int *)dword_5669B8;
  result = 0;
  v3 = &dword_5669A8;
  if ( dword_5669B8 )
  {
    while ( this != (void *)v1[1] )
    {
      v3 = v1;
      v1 = (int *)v1[4];
      if ( !v1 )
        return result;
    }
    sub_46D800(v1[2]);
    v3[4] = v1[4];
    operator delete(v1);
    return 1;
  }
  return result;
}

// ===== sub_496350 @ 0x00496350..0x004963FB =====
int sub_496350()
{
  _DWORD **v0; // esi
  int v1; // ebx
  int v2; // edi
  int v4; // [esp+Ch] [ebp-14h] BYREF
  int v5; // [esp+10h] [ebp-10h]
  int v6; // [esp+14h] [ebp-Ch]
  int v7; // [esp+18h] [ebp-8h]

  sub_48E680(&v4);
  v0 = (_DWORD **)dword_5669B8;
  if ( !dword_5669B8 )
    return -1;
  v1 = v5;
  v2 = v4;
  while ( 1 )
  {
    (*(void (__thiscall **)(_DWORD *, int *))(*v0[2] + 36))(v0[2], &v4);
    if ( v4 <= v2 && v2 <= v6 && v5 <= v1 && v1 <= v7 )
      break;
    v0 = (_DWORD **)v0[4];
    if ( !v0 )
      return -1;
  }
  return (int)*v0;
}

// ===== sub_496400 @ 0x00496400..0x0049642C =====
int __usercall sub_496400@<eax>(int a1@<edx>, _DWORD *a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_5669B8;
  result = 0;
  if ( dword_5669B8 )
  {
    while ( a1 != *v2 )
    {
      v2 = (_DWORD *)v2[4];
      if ( !v2 )
        return result;
    }
    *a2 = v2[3];
    return 1;
  }
  return result;
}

// ===== sub_496430 @ 0x00496430..0x00496464 =====
void sub_496430()
{
  _DWORD *v0; // esi
  _DWORD *v1; // eax

  v0 = dword_5669B8;
  if ( dword_5669B8 )
  {
    do
    {
      v1 = (_DWORD *)(*(int (__thiscall **)(_DWORD))(*(_DWORD *)v0[2] + 28))(v0[2]);
      v0[3] = sub_46DF00(0, v1) & 1;
      v0 = (_DWORD *)v0[4];
    }
    while ( v0 );
  }
}

// ===== sub_496470 @ 0x00496470..0x00496486 =====
void sub_496470()
{
  InitializeCriticalSection(&stru_560F70);
  dword_5669CC = 1;
}

// ===== sub_496490 @ 0x00496490..0x004964BC =====
void sub_496490()
{
  EnterCriticalSection(&stru_560F70);
  dword_5669CC = 0;
  LeaveCriticalSection(&stru_560F70);
  DeleteCriticalSection(&stru_560F70);
}

// ===== sub_4964C0 @ 0x004964C0..0x004964D5 =====
void sub_4964C0()
{
  if ( dword_5669CC )
    EnterCriticalSection(&stru_560F70);
}

// ===== sub_4964E0 @ 0x004964E0..0x004964F5 =====
void sub_4964E0()
{
  if ( dword_5669CC )
    LeaveCriticalSection(&stru_560F70);
}

// ===== sub_496500 @ 0x00496500..0x0049653A =====
void sub_496500()
{
  void *v0; // esi
  void *v1; // [esp-4h] [ebp-8h]

  sub_4964C0();
  v0 = dword_5669C8;
  while ( v0 )
  {
    v1 = v0;
    v0 = (void *)*((_DWORD *)v0 + 3);
    operator delete(v1);
  }
  dword_5669C8 = 0;
  off_503DEC = &unk_5669BC;
  sub_4964E0();
}

// ===== sub_496540 @ 0x00496540..0x0049657E =====
void __cdecl sub_496540(int a1, int a2, int a3)
{
  _DWORD *v3; // eax
  _DWORD *v4; // edx

  sub_4964C0();
  v3 = operator new(0x10u);
  *v3 = a1;
  v3[1] = a2;
  v4 = off_503DEC;
  v3[2] = a3;
  v3[3] = 0;
  v4[3] = v3;
  off_503DEC = v3;
  sub_4964E0();
}

// ===== sub_496580 @ 0x00496580..0x004965CF =====
int __usercall sub_496580@<eax>(_DWORD *a1@<edi>)
{
  _DWORD *v1; // eax
  int v2; // esi

  sub_4964C0();
  v1 = dword_5669C8;
  v2 = 0;
  if ( dword_5669C8 )
  {
    dword_5669C8 = (void *)*((_DWORD *)dword_5669C8 + 3);
    if ( !dword_5669C8 )
      off_503DEC = &unk_5669BC;
    *a1 = *v1;
    a1[1] = v1[1];
    a1[2] = v1[2];
    operator delete(v1);
    v2 = 1;
  }
  sub_4964E0();
  return v2;
}

// ===== sub_4965D0 @ 0x004965D0..0x004965D5 =====
// attributes: thunk
void sub_4965D0()
{
  sub_4546E0();
}

// ===== sub_4965E0 @ 0x004965E0..0x004965F4 =====
void sub_4965E0()
{
  sub_497490();
  sub_496D10();
  sub_496860();
  operator delete[](dword_565DA0);
  dword_565DA0 = 0;
}

// ===== sub_496600 @ 0x00496600..0x0049670E =====
int __usercall sub_496600@<eax>(unsigned int a1@<eax>)
{
  int result; // eax

  if ( a1 > 0x8000000A )
  {
    if ( a1 <= 0xA0000000 )
    {
      switch ( a1 )
      {
        case 0xA0000000:
          return 1;
        case 0x8000000B:
          return 12;
        case 0x90000002:
          return 16;
        case 0x90000003:
          return 17;
      }
      return -1;
    }
    if ( a1 == -1 )
      return -1;
    if ( a1 == -2 )
    {
      return -2;
    }
    else
    {
      switch ( a1 )
      {
        case 0xA0000001:
          result = 18;
          break;
        case 0xA0000002:
          result = 19;
          break;
        case 0xA0000003:
          result = 20;
          break;
        case 0xA0000004:
LABEL_19:
          result = 8;
          break;
        case 0xA0000005:
          result = 21;
          break;
        case 0xA0000006:
          result = 22;
          break;
        default:
          return -1;
      }
    }
  }
  else
  {
    if ( a1 == -2147483638 )
      return 11;
    if ( a1 > 0x80000004 )
    {
      switch ( a1 )
      {
        case 0x80000005:
          result = 6;
          break;
        case 0x80000006:
          result = 7;
          break;
        case 0x80000007:
          goto LABEL_19;
        case 0x80000008:
          result = 9;
          break;
        case 0x80000009:
          result = 10;
          break;
        default:
          return -1;
      }
    }
    else
    {
      if ( a1 == -2147483644 )
        return 5;
      if ( a1 <= 0x80000001 )
      {
        switch ( a1 )
        {
          case 0x80000001:
            return 2;
          case 0u:
            return 0;
          case 0x80000000:
            return 1;
        }
        return -1;
      }
      if ( a1 == -2147483646 )
        return 3;
      else
        return 4;
    }
  }
  return result;
}

// ===== sub_496740 @ 0x00496740..0x004967E3 =====
int __cdecl sub_496740(_DWORD *a1, void *a2, int a3)
{
  _DWORD *v3; // esi
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  int result; // eax
  _DWORD *v7; // eax

  v3 = 0;
  if ( !a2 )
  {
    v4 = operator new(0x498u);
    if ( v4 )
      v5 = sub_454510(v4, a3);
    else
      v5 = 0;
    v3 = v5;
  }
  result = 0;
  if ( v3 )
  {
    v7 = operator new(0xCu);
    *v7 = dword_5669D0++;
    v7[1] = v3;
    v7[2] = dword_5669D8;
    dword_5669D8 = v7;
    *a1 = *v7;
    return 1;
  }
  return result;
}

// ===== sub_4967F0 @ 0x004967F0..0x00496810 =====
int __fastcall sub_4967F0(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_5669D8;
  result = 0;
  if ( dword_5669D8 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return result;
    }
    return v2[1];
  }
  return result;
}

// ===== sub_496810 @ 0x00496810..0x00496856 =====
int __fastcall sub_496810(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = (int *)dword_5669D8;
  result = 0;
  v4 = &dword_5669D0;
  if ( dword_5669D8 )
  {
    while ( a2 != *v2 )
    {
      v4 = v2;
      v2 = (int *)v2[2];
      if ( !v2 )
        return result;
    }
    v4[2] = v2[2];
    v5 = (void (__thiscall ***)(_DWORD, int))v2[1];
    if ( v5 )
      (**v5)(v5, 1);
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_496860 @ 0x00496860..0x00496881 =====
int *__thiscall sub_496860(void *this)
{
  int *result; // eax

  for ( result = (int *)dword_5669D8; dword_5669D8; result = (int *)dword_5669D8 )
    sub_496810((int)this, *result);
  return result;
}

// ===== sub_496890 @ 0x00496890..0x004968BF =====
int __fastcall sub_496890(int a1, int a2, int a3, int a4, void *Src)
{
  _DWORD *v5; // ecx
  unsigned int v6; // eax

  v5 = (_DWORD *)sub_4967F0(a1, a2);
  if ( !v5 )
    return sub_496600(0x80000000);
  v6 = sub_453060(a3, v5, a4, Src);
  return sub_496600(v6);
}

// ===== sub_4968C0 @ 0x004968C0..0x004968EE =====
int __fastcall sub_4968C0(int a1, int a2, _DWORD *a3, void *Src)
{
  _DWORD *v4; // eax
  unsigned int v5; // eax

  v4 = (_DWORD *)sub_4967F0(a1, a2);
  if ( !v4 )
    return sub_496600(0x80000000);
  v5 = sub_453110(v4, a3, Src);
  return sub_496600(v5);
}

// ===== sub_4968F0 @ 0x004968F0..0x0049691A =====
int __fastcall sub_4968F0(int a1, int a2, _DWORD *a3)
{
  _DWORD *v3; // eax
  unsigned int v4; // eax

  v3 = (_DWORD *)sub_4967F0(a1, a2);
  if ( !v3 )
    return sub_496600(0x80000000);
  v4 = sub_453190(v3, a3);
  return sub_496600(v4);
}

// ===== sub_496920 @ 0x00496920..0x00496947 =====
int __fastcall sub_496920(int a1, int a2, int a3)
{
  int v3; // eax
  unsigned int v4; // eax

  v3 = sub_4967F0(a1, a2);
  if ( !v3 )
    return sub_496600(0x80000000);
  v4 = sub_453210(a3, v3);
  return sub_496600(v4);
}

// ===== sub_496950 @ 0x00496950..0x00496975 =====
int __fastcall sub_496950(int a1, int a2)
{
  unsigned int v2; // eax

  if ( !sub_4967F0(a1, a2) )
    return sub_496600(0x80000000);
  v2 = sub_453260();
  return sub_496600(v2);
}

// ===== sub_496980 @ 0x00496980..0x004969A7 =====
int __usercall sub_496980@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<edi>, int a4@<esi>)
{
  unsigned int v4; // eax

  if ( !sub_4967F0(a2, a1) )
    return sub_496600(0x80000000);
  v4 = sub_453290(a3, a4);
  return sub_496600(v4);
}

// ===== sub_4969B0 @ 0x004969B0..0x004969D7 =====
int __usercall sub_4969B0@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<edi>)
{
  unsigned int v3; // eax

  if ( !sub_4967F0(a2, a1) )
    return sub_496600(0x80000000);
  v3 = sub_453300(a3);
  return sub_496600(v3);
}

// ===== sub_4969E0 @ 0x004969E0..0x00496A09 =====
int __usercall sub_4969E0@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int v3; // eax
  unsigned int v4; // eax

  v3 = sub_4967F0(a2, a1);
  if ( !v3 )
    return sub_496600(0x80000000);
  v4 = sub_453370(a3, v3);
  return sub_496600(v4);
}

// ===== sub_496A10 @ 0x00496A10..0x00496A39 =====
int __fastcall sub_496A10(int a1, int a2)
{
  int v2; // eax
  unsigned int v3; // eax

  v2 = sub_4967F0(a1, a2);
  if ( !v2 )
    return sub_496600(0x80000000);
  v3 = sub_4533B0(v2);
  return sub_496600(v3);
}

// ===== sub_496A40 @ 0x00496A40..0x00496A6C =====
int __fastcall sub_496A40(int a1, int a2)
{
  int v2; // eax
  unsigned int v3; // eax

  v2 = sub_4967F0(a1, a2);
  if ( !v2 )
    return sub_496600(0x80000000);
  v3 = sub_4533E0(v2);
  return sub_496600(v3);
}

// ===== sub_496A70 @ 0x00496A70..0x00496AA7 =====
int __fastcall sub_496A70(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // eax
  unsigned int v8; // eax

  v7 = sub_4967F0(a1, a2);
  if ( !v7 )
    return sub_496600(0x80000000);
  v8 = sub_453410(v7, a4, a5, a6, a7, 1);
  return sub_496600(v8);
}

// ===== sub_496AB0 @ 0x00496AB0..0x00496AE5 =====
int __fastcall sub_496AB0(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  unsigned int v7; // eax

  if ( !sub_4967F0(a1, a2) )
    return sub_496600(0x80000000);
  v7 = sub_453510(a3, a4, a6, a7);
  return sub_496600(v7);
}

// ===== sub_496AF0 @ 0x00496AF0..0x00496B19 =====
int __fastcall sub_496AF0(int a1, int a2, void *a3, void *a4)
{
  unsigned int v4; // eax

  if ( !sub_4967F0(a1, a2) )
    return sub_496600(0x80000000);
  v4 = sub_453710(a4, a3);
  return sub_496600(v4);
}

// ===== sub_496B20 @ 0x00496B20..0x00496B4F =====
int __fastcall sub_496B20(int a1, int a2, int a3, int a4, void *a5)
{
  unsigned int v5; // eax

  if ( !sub_4967F0(a1, a2) )
    return sub_496600(0x80000000);
  v5 = sub_453760(a5, a3, a4, 0);
  return sub_496600(v5);
}

// ===== sub_496B50 @ 0x00496B50..0x00496BA0 =====
int __fastcall sub_496B50(
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
        int a12)
{
  int v12; // eax
  unsigned int v13; // eax

  v12 = sub_4967F0(a1, a2);
  if ( !v12 )
    return sub_496600(0x80000000);
  v13 = (*(int (__thiscall **)(int, int, int, int, int, int, int, int, int, int, int, int))(*(_DWORD *)v12 + 4))(
          v12,
          a3,
          a4,
          a5,
          a6,
          a7,
          a8,
          a9,
          a10,
          a11,
          a12,
          1);
  return sub_496600(v13);
}

// ===== sub_496BA0 @ 0x00496BA0..0x00496BD3 =====
int __fastcall sub_496BA0(int a1, int a2, int a3, int a4, int a5, void *a6)
{
  unsigned int v6; // eax

  if ( !sub_4967F0(a1, a2) )
    return sub_496600(0x80000000);
  v6 = sub_4537C0(a6, a3, a4, a5);
  return sub_496600(v6);
}

// ===== sub_496BE0 @ 0x00496BE0..0x00496C1D =====
int __fastcall sub_496BE0(int a1, int a2, _DWORD *a3, int a4, int a5, int a6, int a7)
{
  int v7; // eax

  v7 = sub_4967F0(a1, a2);
  if ( !v7 )
    return sub_496600(0x80000000);
  *a3 = (*(int (__thiscall **)(int, int, int, int, int))(*(_DWORD *)v7 + 12))(v7, a4, a5, a6, a7);
  return sub_496600(0);
}

// ===== sub_496C20 @ 0x00496C20..0x00496CB3 =====
int __cdecl sub_496C20(_DWORD *a1)
{
  _DWORD *v1; // edi
  _DWORD *v2; // eax
  _DWORD *v3; // eax

  v1 = operator new(0xCu);
  *v1 = ++dword_5669DC;
  v2 = operator new(0x14Cu);
  if ( v2 )
    v3 = sub_455500(v2);
  else
    v3 = 0;
  v1[1] = v3;
  v1[2] = dword_5669E4;
  dword_5669E4 = v1;
  *a1 = *v1;
  return 0;
}

// ===== sub_496CC0 @ 0x00496CC0..0x00496D06 =====
int __fastcall sub_496CC0(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = (int *)dword_5669E4;
  result = 1;
  v4 = &dword_5669DC;
  if ( dword_5669E4 )
  {
    while ( a2 != *v2 )
    {
      v4 = v2;
      v2 = (int *)v2[2];
      if ( !v2 )
        return result;
    }
    v4[2] = v2[2];
    v5 = (void (__thiscall ***)(_DWORD, int))v2[1];
    if ( v5 )
      (**v5)(v5, 1);
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_496D10 @ 0x00496D10..0x00496D31 =====
int *__thiscall sub_496D10(void *this)
{
  int *result; // eax

  for ( result = (int *)dword_5669E4; dword_5669E4; result = (int *)dword_5669E4 )
    sub_496CC0((int)this, *result);
  return result;
}

// ===== sub_496D40 @ 0x00496D40..0x00496D60 =====
int __fastcall sub_496D40(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_5669E4;
  result = 0;
  if ( dword_5669E4 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return result;
    }
    return v2[1];
  }
  return result;
}

// ===== sub_496D60 @ 0x00496D60..0x00496DD3 =====
int __fastcall sub_496D60(
        int a1,
        int a2,
        unsigned int a3,
        float a4,
        float a5,
        float a6,
        float a7,
        float a8,
        float a9,
        float a10,
        float a11,
        float a12,
        int a13,
        int a14)
{
  unsigned int v14; // esi
  int v15; // eax

  v14 = -1610612736;
  v15 = sub_496D40(a1, a2);
  if ( v15 )
    v14 = sub_455590(v15, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
  return sub_496600(v14);
}

// ===== sub_496DE0 @ 0x00496DE0..0x00496E07 =====
int __fastcall sub_496DE0(int a1, int a2, unsigned int a3)
{
  unsigned int v3; // esi
  int v4; // eax

  v3 = -1610612736;
  v4 = sub_496D40(a1, a2);
  if ( v4 )
    v3 = sub_455780(v4, a3);
  return sub_496600(v3);
}

// ===== sub_496E10 @ 0x00496E10..0x00496E4E =====
int __fastcall sub_496E10(int a1, int a2, unsigned int a3, float a4, float a5, float a6)
{
  unsigned int v6; // esi
  int v7; // eax

  v6 = -1610612736;
  v7 = sub_496D40(a1, a2);
  if ( v7 )
    v6 = sub_455820(a3, v7, a4, a5, a6);
  return sub_496600(v6);
}

// ===== sub_496E50 @ 0x00496E50..0x00496E7B =====
int __fastcall sub_496E50(int a1, int a2, _OWORD *a3, unsigned int a4)
{
  unsigned int v4; // esi
  int v5; // eax

  v4 = -1610612736;
  v5 = sub_496D40(a1, a2);
  if ( v5 )
    v4 = sub_455910(a4, v5, a3);
  return sub_496600(v4);
}

// ===== sub_496E80 @ 0x00496E80..0x00496EBE =====
int __fastcall sub_496E80(int a1, int a2, unsigned int a3, float a4, float a5, float a6)
{
  unsigned int v6; // esi
  int v7; // eax

  v6 = -1610612736;
  v7 = sub_496D40(a1, a2);
  if ( v7 )
    v6 = sub_455940(a3, v7, a4, a5, a6);
  return sub_496600(v6);
}

// ===== sub_496EC0 @ 0x00496EC0..0x00496EEB =====
int __fastcall sub_496EC0(int a1, int a2, _OWORD *a3, unsigned int a4)
{
  unsigned int v4; // esi
  int v5; // eax

  v4 = -1610612736;
  v5 = sub_496D40(a1, a2);
  if ( v5 )
    v4 = sub_455A90(a4, v5, a3);
  return sub_496600(v4);
}

// ===== sub_496EF0 @ 0x00496EF0..0x00496F20 =====
int __fastcall sub_496EF0(int a1, int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
  unsigned int v5; // esi
  _DWORD *v6; // eax

  v5 = -1610612736;
  v6 = (_DWORD *)sub_496D40(a1, a2);
  if ( v6 )
    v5 = sub_455AC0(v6, a3, a4, a5);
  return sub_496600(v5);
}

// ===== sub_496F20 @ 0x00496F20..0x00496F4E =====
int __fastcall sub_496F20(int a1, int a2, _DWORD *a3, unsigned int a4, unsigned int a5)
{
  unsigned int v5; // esi
  int v6; // eax

  v5 = -1610612736;
  v6 = sub_496D40(a1, a2);
  if ( v6 )
    v5 = sub_455C90(v6, a4, a5, a3);
  return sub_496600(v5);
}

// ===== sub_496F50 @ 0x00496F50..0x00496F7F =====
int __usercall sub_496F50@<eax>(int a1@<edx>, int a2@<ecx>, unsigned int a3@<edi>, int a4, _DWORD *a5, unsigned int a6)
{
  unsigned int v6; // esi
  int v7; // eax

  v6 = -1610612736;
  v7 = sub_496D40(a2, a1);
  if ( v7 )
    v6 = sub_455CD0(v7, a6, a3, a4, a5);
  return sub_496600(v6);
}

// ===== sub_496F80 @ 0x00496F80..0x00496FC6 =====
int __fastcall sub_496F80(int a1, int a2, unsigned int a3, int a4, int a5, int a6, int a7, int a8)
{
  unsigned int v8; // esi
  int v9; // eax

  v8 = -1610612736;
  v9 = sub_496D40(a1, a2);
  if ( v9 )
    v8 = sub_455D30(a3, v9, a4, a5, a6, a7, a8);
  return sub_496600(v8);
}

// ===== sub_496FD0 @ 0x00496FD0..0x0049702F =====
int __fastcall sub_496FD0(
        int a1,
        int a2,
        __int32 *a3,
        _DWORD *a4,
        unsigned int a5,
        unsigned int a6,
        float a7,
        unsigned int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  unsigned int v12; // esi
  int v13; // eax

  v12 = -1610612736;
  v13 = sub_496D40(a1, a2);
  if ( v13 )
    v12 = sub_455DA0(a4, v13, a3, a5, a6, a7, a8, a9, a10, a11, a12 != 0 ? dword_5666F8 : 0);
  return sub_496600(v12);
}

// ===== sub_497030 @ 0x00497030..0x00497061 =====
int __fastcall sub_497030(int a1, int a2, int a3, int a4, unsigned int a5, char a6)
{
  unsigned int v6; // esi
  int v7; // eax

  v6 = -1610612736;
  v7 = sub_496D40(a1, a2);
  if ( v7 )
    v6 = sub_4565C0(v7, a5, a3, a4, a6);
  return sub_496600(v6);
}

// ===== sub_497070 @ 0x00497070..0x004970C2 =====
int __fastcall sub_497070(int a1, int a2, int a3, int a4, int a5, int a6, int a7, float a8, int a9, char a10)
{
  unsigned int v10; // esi

  v10 = -1610612736;
  if ( sub_496D40(a1, a2) )
    v10 = sub_456620(a10, a4, a3, a5, a6, a7, a8, a9);
  return sub_496600(v10);
}

// ===== sub_4970D0 @ 0x004970D0..0x00497103 =====
int __fastcall sub_4970D0(int a1, int a2, int a3, _DWORD *a4, unsigned int a5, char a6)
{
  unsigned int v6; // esi
  int v7; // eax

  v6 = -1610612736;
  v7 = sub_496D40(a1, a2);
  if ( v7 )
    v6 = sub_456670(v7, a3, a4, a5, a6);
  return sub_496600(v6);
}

// ===== sub_497110 @ 0x00497110..0x0049713D =====
int __fastcall sub_497110(int a1, int a2, int a3, unsigned int a4, unsigned int a5)
{
  unsigned int v5; // esi
  int v6; // eax

  v5 = -1610612736;
  v6 = sub_496D40(a1, a2);
  if ( v6 )
    v5 = sub_4568C0(a5, a4, v6, a3);
  return sub_496600(v5);
}

// ===== sub_497140 @ 0x00497140..0x0049723B =====
void __stdcall __noreturn sub_497140(char *a1)
{
  int v1; // eax
  int v2; // eax
  unsigned int v3; // eax
  unsigned int v4; // eax
  int v5; // eax
  unsigned int v6; // eax
  int v7; // eax
  void *v8; // [esp-4h] [ebp-14h]
  void *v9; // [esp-4h] [ebp-14h]

  while ( !*((_DWORD *)a1 + 8) )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 8));
    if ( *((_DWORD *)a1 + 9) == 1 )
    {
      *((_DWORD *)a1 + 9) = 2;
      LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 8));
      v1 = *((_DWORD *)a1 + 10);
      if ( v1 )
      {
        v2 = v1 - 1;
        if ( v2 )
        {
          if ( v2 == 1 )
          {
            v3 = sub_4584F0(
                   *((size_t **)a1 + 12),
                   *((_DWORD **)a1 + 28),
                   *((char **)a1 + 11),
                   *((_DWORD *)a1 + 13),
                   *((_DWORD *)a1 + 14));
            *((_DWORD *)a1 + 26) = sub_496600(v3);
          }
          else
          {
            *((_DWORD *)a1 + 26) = 15;
          }
        }
        else
        {
          v4 = sub_458160(
                 *((_DWORD *)a1 + 11),
                 *((_DWORD *)a1 + 12),
                 *((int **)a1 + 28),
                 *((void ***)a1 + 12),
                 *((_DWORD *)a1 + 13));
          v5 = sub_496600(v4);
          v8 = (void *)*((_DWORD *)a1 + 12);
          *((_DWORD *)a1 + 26) = v5;
          operator delete(v8);
        }
      }
      else
      {
        v6 = sub_457FD0(*((_DWORD **)a1 + 28), *((_DWORD *)a1 + 11), *((_DWORD *)a1 + 12), *((_DWORD *)a1 + 13));
        v7 = sub_496600(v6);
        v9 = (void *)*((_DWORD *)a1 + 13);
        *((_DWORD *)a1 + 26) = v7;
        operator delete[](v9);
      }
      EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 8));
      *((_DWORD *)a1 + 9) = 0;
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 8));
    ResetEvent(*((HANDLE *)a1 + 1));
    WaitForSingleObject(*((HANDLE *)a1 + 1), 0xFFFFFFFF);
  }
  _endthreadex(0);
}

// ===== sub_497250 @ 0x00497250..0x0049736C =====
int __cdecl sub_497250(_DWORD *a1)
{
  _DWORD *v1; // ebx
  uintptr_t v2; // eax
  _DWORD *v3; // esi
  unsigned int v4; // eax
  int v5; // eax
  unsigned int ThrdAddr; // [esp+14h] [ebp-10h] BYREF
  int v8; // [esp+20h] [ebp-4h]

  v1 = operator new(0x78u);
  memset(v1, 0, 0x78u);
  *v1 = ++dword_5669E8;
  v1[1] = CreateEventA(0, 1, 1, 0);
  InitializeCriticalSection((LPCRITICAL_SECTION)(v1 + 2));
  v1[8] = 0;
  v1[9] = 0;
  v2 = _beginthreadex(0, 0, (_beginthreadex_proc_type)sub_497140, v1, 0, &ThrdAddr);
  if ( v2 )
  {
    v1[27] = v2;
    v3 = operator new(0x160u);
    v8 = 0;
    if ( v3 )
    {
      v4 = sub_490AE0(dword_5666F8);
      v5 = sub_457E60(v4, v3);
    }
    else
    {
      v5 = 0;
    }
    v8 = -1;
    v1[28] = v5;
    v1[29] = dword_5669EC;
    dword_5669EC = v1;
    *a1 = *v1;
    return 0;
  }
  else
  {
    DeleteCriticalSection((LPCRITICAL_SECTION)(v1 + 2));
    CloseHandle((HANDLE)v1[1]);
    operator delete(v1);
    return 13;
  }
}

// ===== sub_497370 @ 0x00497370..0x00497484 =====
int __thiscall sub_497370(void *this)
{
  char *v1; // edi
  int result; // eax
  void **v3; // ebx
  void (__thiscall ***v4)(_DWORD, int); // ecx
  _DWORD v5[3]; // [esp+14h] [ebp-18h] BYREF
  int v6; // [esp+28h] [ebp-4h]

  v1 = (char *)dword_5669EC;
  result = 1;
  v3 = &dword_5669EC;
  if ( dword_5669EC )
  {
    while ( this != *(void **)v1 )
    {
      v3 = (void **)(v1 + 116);
      v1 = (char *)*((_DWORD *)v1 + 29);
      if ( !v1 )
        return result;
    }
    *((_DWORD *)v1 + 8) = 1;
    sub_4526E0(v5);
    v6 = 0;
    while ( *((_DWORD *)v1 + 9) )
      sub_4527C0(1u, (int)v5);
    SetEvent(*((HANDLE *)v1 + 1));
    while ( WaitForSingleObject(*((HANDLE *)v1 + 27), 1u) == 258 )
      ;
    v4 = (void (__thiscall ***)(_DWORD, int))*((_DWORD *)v1 + 28);
    if ( v4 )
      (**v4)(v4, 1);
    CloseHandle(*((HANDLE *)v1 + 27));
    DeleteCriticalSection((LPCRITICAL_SECTION)(v1 + 8));
    CloseHandle(*((HANDLE *)v1 + 1));
    *v3 = (void *)*((_DWORD *)v1 + 29);
    operator delete(v1);
    v6 = -1;
    sub_4527A0(v5);
    return 0;
  }
  return result;
}

// ===== sub_497490 @ 0x00497490..0x004974B4 =====
void **sub_497490()
{
  void **result; // eax

  for ( result = (void **)dword_5669EC; dword_5669EC; result = (void **)dword_5669EC )
    sub_497370(*result);
  return result;
}

// ===== sub_4974C0 @ 0x004974C0..0x004974DC =====
void **__thiscall sub_4974C0(void *this)
{
  void **result; // eax

  result = (void **)dword_5669EC;
  if ( dword_5669EC )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[29];
    }
    while ( result );
  }
  return result;
}

// ===== sub_4974E0 @ 0x004974E0..0x0049753A =====
int __cdecl sub_4974E0(_DWORD *a1)
{
  void *v1; // ecx
  void **v2; // eax
  int v3; // edx
  void **v4; // esi

  v2 = sub_4974C0(v1);
  v4 = v2;
  if ( !v2 )
    return v3;
  EnterCriticalSection((LPCRITICAL_SECTION)(v2 + 2));
  if ( v4[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v4 + 2));
    return 14;
  }
  else
  {
    *a1 = v4[26];
    v4[26] = (void *)-65536;
    LeaveCriticalSection((LPCRITICAL_SECTION)(v4 + 2));
    return 0;
  }
}

// ===== sub_497540 @ 0x00497540..0x0049759C =====
int __cdecl sub_497540(int a1)
{
  void *v1; // ecx
  void **v2; // eax
  int v3; // edx
  void **v4; // esi
  int v5; // edx
  unsigned int v6; // eax
  int v7; // esi
  struct _RTL_CRITICAL_SECTION *v9; // [esp-8h] [ebp-Ch]

  v2 = sub_4974C0(v1);
  v4 = v2;
  if ( !v2 )
    return v3;
  EnterCriticalSection((LPCRITICAL_SECTION)(v2 + 2));
  if ( v4[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v4 + 2));
    return 14;
  }
  else
  {
    v6 = sub_457FB0(a1, v5, (int)v4[28]);
    v9 = (struct _RTL_CRITICAL_SECTION *)(v4 + 2);
    v7 = sub_496600(v6);
    LeaveCriticalSection(v9);
    return v7;
  }
}

// ===== sub_4975A0 @ 0x004975A0..0x00497694 =====
int __usercall sub_4975A0@<eax>(void *a1@<ecx>, unsigned int a2@<edi>, void *Src, int a4)
{
  int v4; // edx
  int v5; // ecx
  void **v6; // esi
  void *v7; // ebx
  unsigned int v9; // eax
  int v10; // esi
  void *v11; // [esp-4h] [ebp-14h]
  struct _RTL_CRITICAL_SECTION *v12; // [esp-4h] [ebp-14h]
  int v13; // [esp+Ch] [ebp-4h]

  v6 = sub_4974C0(a1);
  if ( !v6 )
    return 1;
  v13 = sub_4967F0(v5, v4);
  if ( !v13 )
    return 1;
  EnterCriticalSection((LPCRITICAL_SECTION)(v6 + 2));
  if ( v6[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v6 + 2));
    return 14;
  }
  else if ( a4 )
  {
    v7 = operator new[](2100 * a2);
    memcpy_0(v7, Src, 2100 * a2);
    v11 = v6[1];
    v6[9] = (void *)1;
    v6[10] = 0;
    v6[11] = (void *)v13;
    v6[12] = (void *)a2;
    v6[13] = v7;
    SetEvent(v11);
    LeaveCriticalSection((LPCRITICAL_SECTION)(v6 + 2));
    return 0;
  }
  else
  {
    v9 = sub_457FD0(v6[28], v13, a2, (int)Src);
    v12 = (struct _RTL_CRITICAL_SECTION *)(v6 + 2);
    v10 = sub_496600(v9);
    LeaveCriticalSection(v12);
    return v10;
  }
}

// ===== sub_4976A0 @ 0x004976A0..0x00497709 =====
int __thiscall sub_4976A0(void *this)
{
  int v1; // edx
  int v2; // ecx
  void **v3; // esi
  _DWORD *v4; // edi
  unsigned int v5; // eax
  int v6; // esi
  struct _RTL_CRITICAL_SECTION *v8; // [esp-8h] [ebp-10h]

  v3 = sub_4974C0(this);
  if ( !v3 )
    return 1;
  v4 = (_DWORD *)sub_4967F0(v2, v1);
  if ( !v4 )
    return 1;
  EnterCriticalSection((LPCRITICAL_SECTION)(v3 + 2));
  if ( v3[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v3 + 2));
    return 14;
  }
  else
  {
    v5 = sub_458130(v4, (int)v3[28]);
    v8 = (struct _RTL_CRITICAL_SECTION *)(v3 + 2);
    v6 = sub_496600(v5);
    LeaveCriticalSection(v8);
    return v6;
  }
}

// ===== sub_497710 @ 0x00497710..0x004977DA =====
int __usercall sub_497710@<eax>(void *a1@<eax>, void *a2@<ecx>, unsigned int a3, int a4, int a5)
{
  void **v6; // eax
  int v7; // edx
  void **v8; // ebx
  void *v9; // eax
  void *v10; // eax
  unsigned int v12; // eax
  int v13; // esi
  struct _RTL_CRITICAL_SECTION *lpCriticalSection; // [esp+8h] [ebp-4h]

  v6 = sub_4974C0(a2);
  v8 = v6;
  if ( !v6 )
    return v7;
  lpCriticalSection = (struct _RTL_CRITICAL_SECTION *)(v6 + 2);
  EnterCriticalSection((LPCRITICAL_SECTION)(v6 + 2));
  if ( v8[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v8 + 2));
    return 14;
  }
  else if ( a5 )
  {
    v9 = operator new(0x834u);
    qmemcpy(v9, a1, 0x834u);
    v8[12] = v9;
    v10 = v8[1];
    v8[9] = (void *)1;
    v8[10] = (void *)1;
    v8[11] = (void *)a3;
    v8[13] = (void *)a4;
    SetEvent(v10);
    LeaveCriticalSection(lpCriticalSection);
    return 0;
  }
  else
  {
    v12 = sub_458160(a3, (int)v8[28], (int *)v8[28], (void **)a1, a4);
    v13 = sub_496600(v12);
    LeaveCriticalSection((LPCRITICAL_SECTION)(v8 + 2));
    return v13;
  }
}

// ===== sub_4977E0 @ 0x004977E0..0x0049783F =====
int __cdecl sub_4977E0(void *a1, unsigned int a2)
{
  void *v2; // ecx
  void **v3; // eax
  int v4; // edx
  void **v5; // esi
  unsigned int v6; // eax
  int v7; // esi
  struct _RTL_CRITICAL_SECTION *v9; // [esp-8h] [ebp-Ch]

  v3 = sub_4974C0(v2);
  v5 = v3;
  if ( !v3 )
    return v4;
  EnterCriticalSection((LPCRITICAL_SECTION)(v3 + 2));
  if ( v5[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v5 + 2));
    return 14;
  }
  else
  {
    v6 = sub_4584C0(a2, (int)v5[28], a1);
    v9 = (struct _RTL_CRITICAL_SECTION *)(v5 + 2);
    v7 = sub_496600(v6);
    LeaveCriticalSection(v9);
    return v7;
  }
}

// ===== sub_497840 @ 0x00497840..0x004978BB =====
int __cdecl sub_497840(void *a1, void *a2, void *a3, void *a4)
{
  void *v4; // ecx
  void **v5; // eax
  int v6; // edx
  void **v7; // esi
  void *v8; // ecx

  v5 = sub_4974C0(v4);
  v7 = v5;
  if ( !v5 )
    return v6;
  EnterCriticalSection((LPCRITICAL_SECTION)(v5 + 2));
  if ( v7[9] )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(v7 + 2));
    return 14;
  }
  else
  {
    v7[12] = a2;
    v8 = v7[1];
    v7[11] = a1;
    v7[9] = (void *)1;
    v7[10] = (void *)2;
    v7[13] = a3;
    v7[14] = a4;
    SetEvent(v8);
    LeaveCriticalSection((LPCRITICAL_SECTION)(v7 + 2));
    return 0;
  }
}

// ===== sub_4978C0 @ 0x004978C0..0x004978D6 =====
void sub_4978C0()
{
  InitializeCriticalSection(&stru_560F88);
  dword_5669F0 = 0;
}

// ===== sub_4978F0 @ 0x004978F0..0x00497945 =====
unsigned int __usercall sub_4978F0@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( !a2 || a2 > 0x4000000 )
  {
    sprintf(Buffer, &byte_4EC2F0, a2, 0x4000000);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497950 @ 0x00497950..0x0049799A =====
unsigned int __usercall sub_497950@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 >= 0x40 )
  {
    sprintf(Buffer, &byte_4EC364, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_4979A0 @ 0x004979A0..0x004979F0 =====
unsigned int __usercall sub_4979A0@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x80 )
  {
    sprintf(Buffer, &byte_4EC390, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_4979F0 @ 0x004979F0..0x00497A40 =====
unsigned int __usercall sub_4979F0@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x80 )
  {
    sprintf(Buffer, &byte_4EC3C4, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497A40 @ 0x00497A40..0x00497A8A =====
unsigned int __usercall sub_497A40@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 >= 0x10 )
  {
    sprintf(Buffer, &byte_4EC3F8, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497A90 @ 0x00497A90..0x00497AE1 =====
int __usercall sub_497A90@<eax>(int a1@<esi>, int a2)
{
  int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 < 1 || a2 > 256 )
  {
    sprintf(Buffer, &byte_4EC42C, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497AF0 @ 0x00497AF0..0x00497B3B =====
int __usercall sub_497AF0@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int result; // eax
  int v4; // edx
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = sub_468BB0(a2, a1);
  if ( !result )
  {
    sprintf(Buffer, &byte_4E82C4, v4);
    sub_4646F0(Buffer, a3);
  }
  return result;
}

// ===== sub_497B40 @ 0x00497B40..0x00497B58 =====
unsigned int __usercall sub_497B40@<eax>(unsigned int result@<eax>)
{
  if ( result >= 0x100 )
    sub_464520(byte_4EC454, 0);
  return result;
}

// ===== sub_497B60 @ 0x00497B60..0x00497BB0 =====
unsigned int __usercall sub_497B60@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 >= 0x4000 )
  {
    sprintf(Buffer, &byte_4E97A8, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497BB0 @ 0x00497BB0..0x00497BFC =====
unsigned int __usercall sub_497BB0@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 >= 0x10000 )
  {
    sprintf(Buffer, &byte_4E9CBC, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497C00 @ 0x00497C00..0x00497C38 =====
void __usercall __noreturn sub_497C00(int a1@<esi>, int a2)
{
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  sprintf(Buffer, &byte_4EC480, a2);
  sub_4646F0(Buffer, a1);
}

// ===== sub_497C40 @ 0x00497C40..0x00497CBA =====
unsigned int __usercall sub_497C40@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x80 )
  {
    switch ( a2 )
    {
      case 0xC0u:
      case 0xC1u:
      case 0xF0u:
      case 0xFFu:
        return result;
      default:
LABEL_5:
        sprintf(Buffer, &byte_4EC4CC, a2);
        sub_4646F0(Buffer, a1);
    }
  }
  else if ( a2 != 128 )
  {
    switch ( a2 )
    {
      case 0u:
      case 1u:
      case 2u:
      case 3u:
      case 4u:
      case 5u:
      case 6u:
      case 7u:
      case 8u:
      case 9u:
      case 0x20u:
      case 0x21u:
      case 0x22u:
      case 0x23u:
      case 0x24u:
      case 0x25u:
      case 0x26u:
      case 0x27u:
      case 0x40u:
      case 0x41u:
        return result;
      default:
        goto LABEL_5;
    }
  }
  return result;
}

// ===== sub_497D50 @ 0x00497D50..0x00497DA1 =====
int __usercall sub_497D50@<eax>(int a1@<esi>, int a2)
{
  int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 < -1 || a2 > 256 )
  {
    sprintf(Buffer, &byte_4EC4FC, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497DB0 @ 0x00497DB0..0x00497DFC =====
unsigned int __usercall sub_497DB0@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x100 )
  {
    sprintf(Buffer, &byte_4EC530, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497E00 @ 0x00497E00..0x00497E4C =====
unsigned int __usercall sub_497E00@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x100 )
  {
    sprintf(Buffer, &byte_4EC598, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497E50 @ 0x00497E50..0x00497E9C =====
unsigned int __usercall sub_497E50@<eax>(int a1@<esi>, unsigned int a2)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+0h] [ebp-104h] BYREF

  result = a2;
  if ( a2 > 0x100 )
  {
    sprintf(Buffer, &byte_4EC5C0, a2);
    sub_4646F0(Buffer, a1);
  }
  return result;
}

// ===== sub_497EA0 @ 0x00497EA0..0x00497F72 =====
void __usercall sub_497EA0(_DWORD *a1@<eax>, int a2, const char *a3, const char *a4, int a5, int a6)
{
  _DWORD *v7; // esi
  void *v8; // eax
  _BYTE *v9; // edx
  const char *v10; // ecx
  char v11; // al
  _BYTE *v12; // edx
  const char *v13; // ecx
  char v14; // al
  _DWORD *v15; // eax
  _DWORD *v16; // ecx

  *a1 = 0;
  v7 = operator new(0x1Cu);
  *v7 = a1;
  v7[1] = a2;
  if ( a3 )
    v8 = operator new[](strlen(a3) + 1);
  else
    v8 = 0;
  v7[2] = v8;
  v7[3] = operator new[](strlen(a4) + 1);
  v7[4] = a5;
  v7[5] = a6;
  v7[6] = 0;
  if ( a3 )
  {
    v9 = (_BYTE *)v7[2];
    v10 = a3;
    do
    {
      v11 = *v10;
      *v9++ = *v10++;
    }
    while ( v11 );
  }
  v12 = (_BYTE *)v7[3];
  v13 = a4;
  do
  {
    v14 = *v13;
    *v12++ = *v13++;
  }
  while ( v14 );
  EnterCriticalSection(&stru_560FA0);
  v15 = dword_566A1C;
  v16 = &unk_566A04;
  if ( dword_566A1C )
  {
    do
    {
      v16 = v15;
      v15 = (_DWORD *)v15[6];
    }
    while ( v15 );
  }
  v16[6] = v7;
  LeaveCriticalSection(&stru_560FA0);
}

// ===== sub_497F80 @ 0x00497F80..0x00497FC5 =====
BOOL sub_497F80()
{
  void **v0; // esi
  BOOL result; // eax
  BOOL v2; // edi

  v0 = (void **)dword_566A1C;
  result = dword_566A1C != 0;
  v2 = result;
  if ( dword_566A1C )
  {
    dword_566A1C = (void *)*((_DWORD *)dword_566A1C + 6);
    if ( v0[2] )
      operator delete[](v0[2]);
    operator delete[](v0[3]);
    operator delete(v0);
    return v2;
  }
  return result;
}

// ===== sub_497FD0 @ 0x00497FD0..0x004980E4 =====
BOOL sub_497FD0()
{
  void *v0; // edi
  size_t v1; // eax
  _DWORD v3[7]; // [esp+0h] [ebp-28h] BYREF
  int v4; // [esp+24h] [ebp-4h]

  v3[6] = v3;
  EnterCriticalSection(&stru_560FA0);
  v0 = dword_566A1C;
  v3[5] = dword_566A1C;
  LeaveCriticalSection(&stru_560FA0);
  if ( v0 )
  {
    v4 = 0;
    if ( *((_DWORD *)v0 + 5) )
    {
      if ( sub_467F50(
             *((const char **)v0 + 2),
             0,
             *((void **)v0 + 1),
             *((CHAR **)v0 + 3),
             *((_DWORD *)v0 + 4),
             *((_DWORD *)v0 + 5)) )
      {
        **(_DWORD **)v0 = -1;
      }
      else
      {
        **(_DWORD **)v0 = *((_DWORD *)v0 + 5);
      }
      v4 = -1;
    }
    else
    {
      v1 = sub_465AB0(*((_BYTE **)v0 + 3), *((_DWORD **)v0 + 1), *((const char **)v0 + 2));
      if ( !v1 )
        v1 = -1;
      **(_DWORD **)v0 = v1;
      v4 = -1;
    }
    EnterCriticalSection(&stru_560FA0);
    sub_497F80();
    LeaveCriticalSection(&stru_560FA0);
  }
  return v0 != 0;
}
