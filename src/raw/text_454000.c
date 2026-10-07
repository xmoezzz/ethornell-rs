#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_454070 @ 0x00454070..0x004540C4 =====
_DWORD *__stdcall sub_454070(int a1, int a2)
{
  _DWORD *v2; // ecx

  sub_4574B0(a1);
  *v2 = &DCTELgclGrdFld::`vftable';
  return v2;
}

// ===== sub_4540D0 @ 0x004540D0..0x004540F1 =====
void *__thiscall sub_4540D0(void *this, char a2)
{
  sub_454100();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_454100 @ 0x00454100..0x00454147 =====
int __thiscall sub_454100(_DWORD *this)
{
  *this = &DCTELgclGrdFld::`vftable';
  return sub_457520();
}

// ===== sub_454150 @ 0x00454150..0x00454182 =====
double __usercall sub_454150@<st0>(int a1@<eax>, int a2@<edx>, int a3, int a4, int a5, int a6, int a7)
{
  return sqrt(
           (double)(a2 - a6) / (double)a7 * ((double)(a2 - a6) / (double)a7)
         + (double)((a1 - a4) * (a1 - a4) + (a3 - a5) * (a3 - a5)));
}

// ===== sub_454190 @ 0x00454190..0x004541C8 =====
int __usercall sub_454190@<eax>(int a1@<ecx>, _DWORD *a2@<edi>, int a3, int a4, int a5)
{
  int v5; // ecx

  if ( a5 )
    a1 += 2;
  if ( !sub_454470(a1, &a4) )
    return -2147483640;
  v5 = a4;
  *a2 = a3;
  a2[1] = v5;
  return 0;
}

// ===== sub_4541D0 @ 0x004541D0..0x0045433E =====
int __thiscall sub_4541D0(_DWORD *this, int a2, int *a3, int a4, int a5, signed int a6)
{
  int *v6; // edx
  unsigned int v7; // esi
  char *v8; // eax
  int v9; // esi
  int v10; // edi
  int v11; // eax
  int v12; // ecx
  int v13; // eax
  bool v14; // zf
  int v15; // ebx
  __int64 v16; // rax
  _DWORD *v18; // [esp+Ch] [ebp-3Ch]
  unsigned int v19; // [esp+10h] [ebp-38h]
  int *v20; // [esp+14h] [ebp-34h]
  int v21; // [esp+1Ch] [ebp-2Ch]
  int v22; // [esp+24h] [ebp-24h]
  _DWORD v23[7]; // [esp+28h] [ebp-20h] BYREF

  v6 = a3;
  v18 = this;
  if ( !this[6] )
    return -2147483645;
  v23[0] = -1;
  v23[3] = -1;
  v23[2] = 1;
  v23[5] = 1;
  v7 = 0;
  v8 = (char *)((char *)v23 - (char *)a3);
  v22 = 0;
  v23[1] = 0;
  v23[4] = 0;
  v23[6] = 0;
  v19 = 0;
  v20 = a3;
  while ( 1 )
  {
    *(_DWORD *)(a2 + 4 * v7) = -1;
    if ( v7 < 4 )
    {
      v9 = a4 + *(int *)((char *)v6 + (_DWORD)v8 - 4);
      v10 = a5 + *(int *)((char *)v6 + (_DWORD)v8);
      if ( v9 >= 0 )
      {
        v11 = this[3];
        if ( v9 < v11 && v10 >= 0 && v10 < this[4] )
        {
          v12 = this[6];
          v13 = v9 + v10 * v11;
          v14 = *(_DWORD *)(v12 + 24 * v13 + 4) == 0;
          v21 = v12 + 24 * v13;
          this = v18;
          if ( !v14 )
          {
            v15 = v18[5];
            if ( (*(_BYTE *)(v15 + 16 * v13 + 8) & 0xD) == 0 )
            {
              v16 = *(_DWORD *)(v15 + 16 * (v9 + v10 * v18[3])) - *(_DWORD *)(v15 + 16 * (a4 + a5 * v18[3]));
              if ( (int)((HIDWORD(v16) ^ v16) - HIDWORD(v16)) > a6 )
              {
                v6 = v20;
              }
              else
              {
                *(_DWORD *)(a2 + 4 * v19) = *(_DWORD *)(v21 + 8);
                v6 = v20;
                this = v18;
                *v20 = v9;
                v20[1] = v10;
              }
            }
          }
        }
      }
    }
    v7 = v19 + 1;
    v6 += 2;
    v19 = v7;
    v20 = v6;
    if ( v7 >= 6 )
      break;
    v8 = (char *)((char *)v23 - (char *)a3);
  }
  return 0;
}

// ===== sub_454340 @ 0x00454340..0x00454449 =====
int __thiscall sub_454340(void *this, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  int result; // eax
  int v10; // [esp+10h] [ebp-14h]

  sub_457B30(this);
  result = v10;
  if ( v10 != 2 )
  {
    sub_457BA0(a3 - 1, 3, a4, a5 + 1, a6, a7, a8 + (v10 != 3), a9);
    result = v10;
  }
  if ( result != 3 )
  {
    sub_457BA0(a3 + 1, 2, a4, a5 + 1, a6, a7, a8 + (result != 2), a9);
    result = v10;
  }
  if ( result != 4 )
  {
    sub_457BA0(a3, 5, a4, a5 + 1, a6, a7, a8 + (result != 5), a9);
    result = v10;
  }
  if ( result != 5 )
    return sub_457BA0(a3, 4, a4, a5 + 1, a6, a7, a8 + (result != 4), a9);
  return result;
}

// ===== sub_454450 @ 0x00454450..0x00454467 =====
int __stdcall sub_454450(int a1, int a2, int a3)
{
  return sub_454470(a3, a2);
}

// ===== sub_454470 @ 0x00454470..0x00454493 =====
int __usercall sub_454470@<eax>(_DWORD *a1@<edx>, int a2@<ecx>, _DWORD *a3@<esi>)
{
  int result; // eax

  result = 1;
  switch ( a2 )
  {
    case 2:
      --*a1;
      break;
    case 3:
      ++*a1;
      break;
    case 4:
      --*a3;
      break;
    case 5:
      ++*a3;
      break;
    default:
      result = 0;
      break;
  }
  return result;
}

// ===== sub_4544B0 @ 0x004544B0..0x004544F4 =====
int __stdcall sub_4544B0(int a1, int a2)
{
  int result; // eax

  result = a1;
  if ( a2 )
    result = a1 + 2;
  switch ( result )
  {
    case 2:
      result = 3;
      break;
    case 3:
      result = 2;
      break;
    case 4:
      result = 5;
      break;
    case 5:
      result = 4;
      break;
    default:
      break;
  }
  if ( a2 )
    result -= 2;
  return result;
}

// ===== sub_454510 @ 0x00454510..0x00454645 =====
_DWORD *__stdcall sub_454510(_DWORD *a1, int a2)
{
  int v2; // eax
  int v3; // ecx
  int *v4; // esi
  int v5; // ebx
  int v6; // ebx
  int v7; // edi
  int v8; // eax
  _DWORD *result; // eax
  _DWORD v10[16]; // [esp+10h] [ebp-58h] BYREF
  int v11; // [esp+50h] [ebp-18h]
  int v12; // [esp+54h] [ebp-14h]
  int v13; // [esp+58h] [ebp-10h]
  int v14; // [esp+64h] [ebp-4h]
  int v15; // [esp+74h] [ebp+Ch]

  sub_452DC0(a2, a1);
  v2 = 0;
  v14 = 0;
  v10[2] = 0x2000;
  v10[3] = 0x2000;
  v10[6] = 0x10000;
  v10[7] = 0x2000;
  v10[10] = 0x2000;
  v10[11] = 0x10000;
  v10[14] = 0x10000;
  v10[15] = 0x10000;
  v10[0] = 0;
  v10[1] = 0;
  v10[4] = 57344;
  v10[5] = 0;
  v10[8] = 0;
  v10[9] = 57344;
  v10[12] = 57344;
  v10[13] = 57344;
  qmemcpy(a1 + 21, v10, 0x40u);
  v3 = 0;
  *a1 = &DCTELgclGrdFldMngr::`vftable';
  v13 = 0;
  v12 = 8;
  v4 = a1 + 37;
  while ( 1 )
  {
    v5 = v12;
    if ( v3 >= 8 )
      v5 = v3 - 7;
    v15 = 0;
    v6 = v5 * v5;
    v7 = 8;
    while ( 1 )
    {
      v8 = v2 >= 8 ? v2 - 7 : v7;
      v11 = v6 + v8 * v8;
      ++v15;
      *v4 = ((int)sqrt((double)v11) << 8) / 8;
      --v7;
      ++v4;
      if ( v7 <= -8 )
        break;
      v2 = v15;
    }
    ++v13;
    if ( --v12 <= -8 )
      break;
    v3 = v13;
    v2 = 0;
  }
  result = a1;
  a1[293] = 0;
  return result;
}

// ===== sub_454650 @ 0x00454650..0x00454671 =====
void *__thiscall sub_454650(void *this, char a2)
{
  sub_454680();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_454680 @ 0x00454680..0x004546D9 =====
int __thiscall sub_454680(_DWORD *this)
{
  *this = &DCTELgclGrdFldMngr::`vftable';
  sub_454F10();
  return sub_452E30(this);
}

// ===== sub_4546E0 @ 0x004546E0..0x00454733 =====
void sub_4546E0()
{
  int v0; // ebx
  int *v1; // edi
  int v2; // esi

  if ( !dword_565DA0 )
  {
    dword_565DA0 = operator new[](0x6E4u);
    v0 = -10;
    v1 = (int *)dword_565DA0;
    do
    {
      v2 = -10;
      do
        *v1++ = sub_401000(v2++, v0);
      while ( v2 <= 10 );
      ++v0;
    }
    while ( v0 <= 10 );
  }
}

// ===== sub_454760 @ 0x00454760..0x0045478E =====
int __usercall sub_454760@<eax>(int a1@<eax>, int a2@<ecx>)
{
  if ( (unsigned int)(a2 + 10) > 0x14 || (unsigned int)(a1 + 10) > 0x14 )
    return sub_401000(a2, a1);
  else
    return *((_DWORD *)dword_565DA0 + 21 * a1 + a2 + 220);
}

// ===== sub_454790 @ 0x00454790..0x004549A4 =====
int __thiscall sub_454790(
        _DWORD *this,
        int a2,
        int a3,
        _DWORD *a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  int result; // eax
  _DWORD *v14; // eax
  int v15; // edx
  int v16; // ecx
  int i; // esi
  int v18; // ebx
  int v19; // ecx
  int k; // ebx
  _DWORD *v21; // esi
  int v22; // esi
  void (__thiscall *v23)(_DWORD *, int *, int, int, int, int, int, int); // edx
  int v24; // eax
  int v25; // [esp+10h] [ebp-B0h]
  int v26; // [esp+1Ch] [ebp-A4h] BYREF
  int v27; // [esp+20h] [ebp-A0h] BYREF
  int v28; // [esp+24h] [ebp-9Ch] BYREF
  _DWORD *v29; // [esp+28h] [ebp-98h]
  int v30; // [esp+2Ch] [ebp-94h]
  int v31; // [esp+30h] [ebp-90h]
  int v32; // [esp+34h] [ebp-8Ch]
  int j; // [esp+38h] [ebp-88h]
  _DWORD v34[32]; // [esp+3Ch] [ebp-84h]

  v30 = a5;
  v32 = a6;
  result = sub_4530E0(this, &v27, &v28);
  if ( !result )
  {
    v14 = (_DWORD *)(this[4] + 16 * (a5 + a6 * this[2]));
    v15 = (a8 * a7) << 8;
    v16 = (a7 << 16) + v15 * *v14;
    v29 = v14;
    for ( i = 0; i < 32; ++i )
    {
      if ( v16 < 0 )
        v18 = 0;
      else
        v18 = v16 + 0x4000;
      v34[i] = v18;
      v16 -= v15;
    }
    v19 = 0;
    v31 = 0;
    for ( j = 0; v19 < v28; j = v19 )
    {
      for ( k = 0; k < v27; ++k )
      {
        v21 = (_DWORD *)(this[4] + 16 * (k + v19 * this[2]));
        v25 = (int)(((double (__thiscall *)(_DWORD *, int, int, _DWORD, int, int, _DWORD))*(_DWORD *)(*this + 40))(
                      this,
                      k,
                      v19,
                      *v21,
                      v30,
                      v32,
                      *v14)
                  * 65536.0);
        if ( v25 < v34[*v21] )
        {
          v22 = j;
          if ( !a10 || sub_453E40(j, k, (int)this) )
          {
            v23 = *(void (__thiscall **)(_DWORD *, int *, int, int, int, int, int, int))(*this + 52);
            v26 = 0;
            v23(this, &v26, k, v22, v30, v32, a9, a11);
            if ( v26 || !a12 && k == v30 && v22 == v32 )
            {
              v24 = v31;
              *(_DWORD *)(a2 + 8 * v31) = k;
              *(_DWORD *)(a2 + 8 * v24 + 4) = v22;
              if ( a3 )
                *(_DWORD *)(a3 + 4 * v24) = v25;
              v31 = v24 + 1;
            }
          }
        }
        v14 = v29;
        v19 = j;
      }
      ++v19;
    }
    *a4 = v31;
    return 0;
  }
  return result;
}

// ===== sub_4549B0 @ 0x004549B0..0x00454A03 =====
int __stdcall sub_4549B0(int a1, int a2, int a3, int a4)
{
  int v4; // eax

  v4 = sub_454760(a4 - a2, a1 - a3) >> 16;
  if ( (unsigned int)(v4 - 45) <= 0x59 )
    return 2;
  if ( (unsigned int)(v4 - 225) > 0x59 )
    return ((unsigned int)(v4 - 135) > 0x59) + 4;
  return 3;
}

// ===== sub_454A10 @ 0x00454A10..0x00454ABD =====
int __stdcall sub_454A10(_DWORD *a1, int a2, int a3)
{
  int result; // eax
  _DWORD *v4; // ecx

  result = 0;
  if ( a3 )
    a2 += 2;
  switch ( a2 )
  {
    case 2:
      v4 = a1;
      *a1 = 3;
      a1[1] = 4;
      a1[2] = 5;
      a1[3] = 2;
      goto LABEL_5;
    case 3:
      v4 = a1;
      *a1 = 2;
      a1[1] = 5;
      a1[2] = 4;
      a1[3] = 3;
      goto LABEL_5;
    case 4:
      v4 = a1;
      *a1 = 5;
      a1[1] = 3;
      a1[2] = 2;
      a1[3] = 4;
      goto LABEL_5;
    case 5:
      v4 = a1;
      *a1 = 4;
      a1[1] = 2;
      a1[2] = 3;
      a1[3] = 5;
LABEL_5:
      if ( a3 )
      {
        *v4 -= 2;
        v4[1] -= 2;
        v4[2] -= 2;
        v4[3] -= 2;
      }
      break;
    default:
      result = -2147483640;
      break;
  }
  return result;
}

// ===== sub_454AD0 @ 0x00454AD0..0x00454B15 =====
unsigned int __thiscall sub_454AD0(void *this, _DWORD *a2, int a3, int a4, int a5)
{
  _DWORD *v5; // eax
  int v6; // edx

  v5 = sub_453E70((int)this, a3);
  if ( v5 )
    return sub_454190(a4, a2, v5[1], v5[2], a5) != 0 ? 0x8000000B : 0;
  else
    return v6;
}

// ===== sub_454B20 @ 0x00454B20..0x00454B43 =====
BOOL __stdcall sub_454B20(unsigned int a1)
{
  return a1 >= 2 && a1 <= 5 || sub_453E30(a1);
}

// ===== sub_454B50 @ 0x00454B50..0x00454CD4 =====
int __thiscall sub_454B50(_DWORD *this, int a2, int a3)
{
  int v3; // edx
  int v4; // edi
  int v5; // eax
  int *v6; // esi
  _DWORD *v7; // edi
  int v8; // edx
  int v9; // eax
  int v10; // esi
  int v11; // ebx
  int v12; // esi
  int v13; // eax
  int v14; // esi
  int v15; // edx
  int result; // eax
  int v17; // esi
  int v18; // esi
  int v19; // eax
  int v20; // esi
  int v21; // esi
  int v22; // edx
  _DWORD *v23; // esi
  int v24; // esi
  int v25; // edx
  int v26; // esi
  __int64 v27; // rax
  int v28; // [esp+10h] [ebp-10h]
  int v29; // [esp+14h] [ebp-Ch]

  v3 = *(_DWORD *)(a3 + 8);
  v4 = this[2];
  v5 = *(_DWORD *)(a3 + 4);
  v6 = (int *)(16 * (v5 + v3 * v4) + a2);
  v29 = *v6;
  v28 = *(unsigned __int8 *)(a3 + 28);
  if ( v5 < 0 || v5 >= v4 || v3 < 0 )
  {
    v7 = this;
  }
  else
  {
    v7 = this;
    if ( v3 < this[3] && (signed int)abs32(0) <= *(_DWORD *)(a3 + 36) )
      v6[3] |= *(unsigned __int8 *)(a3 + 28);
  }
  v8 = *(_DWORD *)(a3 + 4);
  v9 = *(_DWORD *)(a3 + 8) - 1;
  if ( v8 < 0 || (v10 = v7[2], v8 >= v10) || v9 < 0 || v9 >= v7[3] )
  {
    v11 = a2;
  }
  else
  {
    v11 = a2;
    v12 = 16 * (v8 + v9 * v10);
    v13 = v29 - *(_DWORD *)(v12 + a2);
    v14 = a2 + v12;
    if ( (signed int)abs32(v13) <= *(_DWORD *)(a3 + 36) )
      *(_DWORD *)(v14 + 12) |= v28;
  }
  v15 = *(_DWORD *)(a3 + 4);
  result = *(_DWORD *)(a3 + 8) + 1;
  if ( v15 >= 0 )
  {
    v17 = v7[2];
    if ( v15 < v17 && result >= 0 && result < v7[3] )
    {
      v18 = 16 * (v15 + result * v17);
      v19 = v29 - *(_DWORD *)(v18 + v11);
      v20 = v11 + v18;
      result = abs32(v19);
      if ( result <= *(_DWORD *)(a3 + 36) )
        *(_DWORD *)(v20 + 12) |= v28;
    }
  }
  v21 = *(_DWORD *)(a3 + 4);
  v22 = *(_DWORD *)(a3 + 8);
  if ( v21 - 1 >= 0 )
  {
    result = this[2];
    if ( v21 - 1 < result && v22 >= 0 && v22 < this[3] )
    {
      v23 = (_DWORD *)(v11 + 16 * (v21 + v22 * result) - 16);
      result = abs32(v29 - *v23);
      if ( result <= *(_DWORD *)(a3 + 36) )
        v23[3] |= v28;
    }
  }
  v24 = *(_DWORD *)(a3 + 4);
  v25 = *(_DWORD *)(a3 + 8);
  if ( v24 + 1 >= 0 )
  {
    result = this[2];
    if ( v24 + 1 < result && v25 >= 0 && v25 < this[3] )
    {
      v26 = 2 * (v25 * result + v24 + 1);
      v27 = v29 - *(_DWORD *)(v11 + 8 * v26);
      result = (HIDWORD(v27) ^ v27) - HIDWORD(v27);
      if ( result <= *(_DWORD *)(a3 + 36) )
        *(_DWORD *)(v11 + 8 * v26 + 12) |= v28;
    }
  }
  return result;
}

// ===== sub_454CE0 @ 0x00454CE0..0x00454D46 =====
_DWORD *sub_454CE0()
{
  _DWORD *result; // eax
  int v1; // eax
  _DWORD *v2; // ecx
  void *v3; // [esp+8h] [ebp-10h]

  v3 = operator new(0x498u);
  result = 0;
  if ( v3 )
  {
    v1 = sub_490AE0();
    return sub_454510(v2, v1);
  }
  return result;
}

// ===== sub_454D50 @ 0x00454D50..0x00454DB3 =====
_DWORD *sub_454D50()
{
  _DWORD *result; // eax
  int v1; // eax
  int v2; // ecx
  void *v3; // [esp+8h] [ebp-10h]

  v3 = operator new(0x48u);
  result = 0;
  if ( v3 )
  {
    v1 = sub_490AE0();
    return sub_454070(v2, v1);
  }
  return result;
}

// ===== sub_454DC0 @ 0x00454DC0..0x00454DED =====
void __stdcall sub_454DC0(int a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // eax

  v6 = sub_490AE0();
  sub_454150(a1, a3, a2, a4, a5, a6, v6);
}

// ===== sub_454DF0 @ 0x00454DF0..0x00454F05 =====
int __thiscall sub_454DF0(_DWORD *this)
{
  int result; // eax
  char *v3; // edi
  int v4; // esi
  int v5; // eax
  int i; // esi
  int v7; // edx
  int v8; // edi
  int v9; // eax
  int *v10; // edi
  int v11; // ecx
  int v12; // edx
  int v13; // edi
  char *v14; // edx
  int v15; // eax
  char *v16; // [esp+4h] [ebp-10h] BYREF
  char *v17; // [esp+8h] [ebp-Ch]
  int v18; // [esp+Ch] [ebp-8h] BYREF
  int v19; // [esp+10h] [ebp-4h]

  result = sub_4530E0(this, &v16, &v18);
  if ( !result )
  {
    v3 = v16;
    v4 = v18 * (_DWORD)v16;
    v16 = (char *)operator new[](16 * v18 * (_DWORD)v16);
    memset(v16, 0, 16 * v4);
    v5 = 1;
    v19 = 1;
    if ( v18 > 1 )
    {
      v17 = v3 - 1;
      do
      {
        for ( i = 0; i < (int)v17; ++i )
        {
          v7 = this[4];
          v8 = v5 * this[2];
          v9 = 2 * (i + this[2] * (v5 - 1));
          v10 = (int *)(v7 + 16 * (i + v8));
          v11 = *(_DWORD *)(v7 + 8 * v9) + (*(_DWORD *)(v7 + 8 * v9 + 12) >> 28);
          v12 = v10[4] + ((unsigned int)v10[7] >> 28);
          v13 = *v10;
          if ( v13 < v11 && v13 < v12 )
          {
            if ( v11 >= v12 )
              v11 = v12;
            v14 = v16;
            *(_DWORD *)&v16[8 * v9 + 12] = v11;
            v15 = v19;
            *(_DWORD *)&v14[16 * i + 24 + 16 * (v19 - 1) * this[2]] = v11;
            *(_DWORD *)&v14[16 * i + 4 + 16 * v15 * this[2]] = v11;
            *(_DWORD *)&v14[16 * v15 * this[2] + 16 + 16 * i] = v11;
          }
          v5 = v19;
        }
        v19 = ++v5;
      }
      while ( v5 < v18 );
    }
    this[293] = v16;
    return 0;
  }
  return result;
}

// ===== sub_454F10 @ 0x00454F10..0x00454F2E =====
void __thiscall sub_454F10(void **this)
{
  operator delete[](this[293]);
  this[293] = 0;
}

// ===== sub_454F30 @ 0x00454F30..0x0045537B =====
int __thiscall sub_454F30(_DWORD *this, int *a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  int v10; // ebx
  _DWORD *v11; // eax
  _DWORD *v12; // eax
  int v13; // eax
  _DWORD *v14; // eax
  int v15; // eax
  int v16; // ebx
  unsigned int v17; // edx
  _DWORD *v18; // edi
  int v19; // esi
  int v20; // edx
  int v21; // ebx
  int v22; // eax
  int v23; // esi
  unsigned int v24; // ecx
  _DWORD *v25; // eax
  _DWORD *v26; // eax
  int v27; // edx
  int v28; // eax
  int v29; // [esp+18h] [ebp-50h]
  int v30; // [esp+18h] [ebp-50h]
  _DWORD *v31; // [esp+1Ch] [ebp-4Ch]
  __int64 v32; // [esp+20h] [ebp-48h]
  int v33; // [esp+24h] [ebp-44h]
  unsigned int v34; // [esp+28h] [ebp-40h]
  int v36; // [esp+30h] [ebp-38h]
  int v37; // [esp+34h] [ebp-34h]
  double v38; // [esp+38h] [ebp-30h]
  int v39; // [esp+38h] [ebp-30h]
  int v40; // [esp+4Ch] [ebp-1Ch] BYREF
  int v41; // [esp+50h] [ebp-18h]
  int v42; // [esp+54h] [ebp-14h]
  int v43; // [esp+64h] [ebp-4h]

  if ( !this[4] )
    return -2147483646;
  if ( a3 < 0 )
    return -2147483644;
  if ( a3 >= this[2] )
    return -2147483644;
  if ( a4 < 0 )
    return -2147483644;
  v10 = this[3];
  if ( a4 >= v10 || a5 == a3 && a6 == a4 )
    return -2147483644;
  if ( a5 < 0 || a5 >= this[2] || a6 < 0 || a6 >= v10 )
    return -2147483640;
  v11 = operator new(0x2640u);
  v43 = 0;
  if ( v11 )
    v31 = sub_444070(v11);
  else
    v31 = 0;
  v43 = -1;
  v38 = ((double (__thiscall *)(_DWORD *, int, int, _DWORD, int, int, _DWORD))*(_DWORD *)(*this + 40))(
          this,
          a3,
          a4,
          *(_DWORD *)(16 * (a3 + a4 * this[2]) + this[4]),
          a5,
          a6,
          *(_DWORD *)(this[4] + 16 * (a5 + a6 * this[2])));
  v34 = (__int64)(v38 * 8.0);
  v12 = sub_453E40(a6, a5, (int)this);
  if ( v12 )
    v13 = v12[4];
  else
    v13 = 3;
  v29 = v13 << 15;
  v14 = sub_453E40(a4, a3, (int)this);
  if ( v14 )
    v15 = v14[4];
  else
    v15 = 3;
  v33 = v15 << 15;
  sub_4441D0((int)v31);
  sub_444270((int)v31, 0, 0, v29);
  if ( a7 >= 1 )
    sub_444270((int)v31, 0, 0, ((v29 + v33) >> 1) - (int)((double)a7 * v38 * -0.00390625 * 65536.0));
  sub_444270((int)v31, 0, 0, v33);
  sub_4442C0((int)v31, v34);
  v16 = 1;
  v17 = 0;
  v32 = 0x100000000LL;
  while ( 1 )
  {
    if ( v17 > v34 )
    {
      v18 = this;
      v28 = 0;
      goto LABEL_55;
    }
    v42 = 0;
    v41 = 0;
    v40 = 0;
    if ( sub_4442E0((int)v31, v17, &v40) )
    {
      v18 = this;
      v19 = this[4];
      v20 = v40 >> 16;
      v21 = v42 >> 16;
      v22 = 16 * ((v40 >> 16) + (v41 >> 16) * this[2]);
      v30 = v41 >> 16;
      v39 = v42 >> 16;
      if ( (*(_BYTE *)(v19 + v22 + 8) & 2) != 0
        || (v37 = *(_DWORD *)(v19 + v22), v37 + (*(_DWORD *)(v19 + v22 + 12) >> 28) > v21) )
      {
        v16 = 0;
        goto LABEL_58;
      }
      v23 = v22 + this[293];
      v36 = v23;
      v24 = 0;
      v25 = this + 22;
      while ( 1 )
      {
        if ( *(_DWORD *)(v23 + 4 * v24) > v21
          && *(v25 - 1) <= (int)(unsigned __int16)v40
          && (unsigned __int16)v40 < (int)v25[1]
          && *v25 <= (int)(unsigned __int16)v41
          && (unsigned __int16)v41 < (int)v25[2] )
        {
          v16 = 0;
          HIDWORD(v32) = 0;
          goto LABEL_46;
        }
        ++v24;
        v25 += 4;
        if ( v24 >= 4 )
          break;
        v23 = v36;
      }
      v16 = HIDWORD(v32);
      if ( HIDWORD(v32) && !a8 && (v20 != a5 || v30 != a6) )
      {
        v26 = sub_453E40(v30, v20, (int)this);
        if ( v26 )
        {
          if ( v37 + v26[4] > v39
            && v26[5] >= this[16 * ((int)(unsigned __int16)v41 >> 12) + 37 + ((int)(unsigned __int16)v40 >> 12)] )
          {
            break;
          }
        }
      }
    }
LABEL_46:
    v17 = v32 + 1;
    LODWORD(v32) = v32 + 1;
    if ( !v16 )
      goto LABEL_58;
  }
  if ( v27 == a3 && v30 == a4 )
  {
    v16 = 1;
    v28 = 1;
  }
  else
  {
    v16 = 0;
    v28 = 1;
  }
LABEL_55:
  if ( v16 && !v28 )
    v16 = (*(_DWORD *)(v18[4] + 16 * (a3 + a4 * v18[2]) + 8) & 4) == 0;
LABEL_58:
  *a2 = v16;
  if ( v31 )
    (*(void (__thiscall **)(_DWORD *, int))*v31)(v31, 1);
  return 0;
}

// ===== sub_455380 @ 0x00455380..0x004553FA =====
int __stdcall sub_455380(int *a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // edi
  int v7; // ecx
  int v8; // esi
  int v9; // eax

  v6 = 0;
  v7 = 0;
  if ( !a6 )
    goto LABEL_12;
  v8 = 0;
  switch ( a6 )
  {
    case 2:
      v8 = 5898240;
      break;
    case 3:
      v8 = 17694720;
      break;
    case 4:
      v8 = 11796480;
      break;
    case 5:
      v8 = 0;
      break;
    default:
      v6 = -2147483637;
      break;
  }
  v9 = abs32(sub_454760(a5 - a3, a2 - a4) - v8);
  v7 = v9 > 11796480 ? 23592960 - v9 : v9;
  if ( !v6 )
LABEL_12:
    *a1 = v7;
  return v6;
}

// ===== sub_455410 @ 0x00455410..0x0045542E =====
double __cdecl sub_455410(float a1)
{
  return (float)ceil(a1);
}

// ===== sub_455430 @ 0x00455430..0x00455483 =====
float __usercall sub_455430@<xmm0>(__m128 *a1@<eax>)
{
  __m128 v1; // xmm0
  float v2; // xmm1_4

  v1 = _mm_mul_ps(*a1, *a1);
  v1.m128_f32[0] = v1.m128_f32[0]
                 + (float)(_mm_shuffle_ps(v1, v1, 253).m128_f32[0] + _mm_shuffle_ps(v1, v1, 254).m128_f32[0]);
  v2 = 1.0 / fsqrt(v1.m128_f32[0]);
  return v1.m128_f32[0]
       * (float)((float)(v2 * 1.5) + (float)((float)((float)((float)(v1.m128_f32[0] * v2) * v2) * v2) * -0.5));
}

// ===== sub_455490 @ 0x00455490..0x004554F7 =====
__m128i __usercall sub_455490@<xmm0>(__m128 *a1@<eax>)
{
  __m128i v1; // xmm3
  __m128i v2; // xmm1
  __m128i v3; // xmm0
  __m128i v4; // xmm4
  __m128i v5; // xmm2
  __m128i v6; // xmm3

  v1 = _mm_cvtps_epi32(_mm_mul_ps((__m128)xmmword_4E41D0, *a1));
  v2 = _mm_and_si128(v1, _mm_load_si128((const __m128i *)&xmmword_4E42E0));
  v3 = _mm_sub_epi32(v1, v2);
  v4 = _mm_cmpgt_epi32(_mm_load_si128((const __m128i *)&xmmword_4E42D0), v3);
  v5 = _mm_andnot_si128(v4, v1);
  v6 = _mm_cmpgt_epi32(v3, _mm_load_si128((const __m128i *)&xmmword_4E42F0));
  return _mm_or_si128(
           _mm_andnot_si128(v6, _mm_or_si128(v5, _mm_and_si128(v4, v2))),
           _mm_and_si128(v6, _mm_add_epi32(v2, (__m128i)xmmword_4E4300)));
}

// ===== sub_455500 @ 0x00455500..0x00455539 =====
_DWORD *__usercall sub_455500@<eax>(_DWORD *a1@<esi>)
{
  void *v1; // eax

  *a1 = &DCTELgclSpcMngr::`vftable';
  a1[1] = 64;
  v1 = _aligned_malloc(0x1800u, 0x10u);
  a1[2] = v1;
  memset(v1, 0, 0x1800u);
  a1[80] = 0;
  return a1;
}

// ===== sub_455540 @ 0x00455540..0x00455561 =====
void *__thiscall sub_455540(void *this, char a2)
{
  sub_455570();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_455570 @ 0x00455570..0x0045558C =====
void __thiscall sub_455570(void **this)
{
  *this = &DCTELgclSpcMngr::`vftable';
  sub_457060();
  _aligned_free(this[2]);
}

// ===== sub_455590 @ 0x00455590..0x00455777 =====
int __userpurge sub_455590@<eax>(
        int a1@<edi>,
        unsigned int a2,
        float a3,
        float a4,
        float a5,
        float a6,
        float a7,
        float a8,
        float a9,
        float a10,
        float a11,
        int a12,
        int a13)
{
  int result; // eax
  unsigned int i; // esi
  __m128 *v15; // esi
  char *v16; // [esp+1Ch] [ebp-34h]
  unsigned int v17; // [esp+1Ch] [ebp-34h]
  unsigned int v18; // [esp+1Ch] [ebp-34h]
  unsigned int v19; // [esp+1Ch] [ebp-34h]
  __m128 v20; // [esp+20h] [ebp-30h]
  __m128 v21; // [esp+40h] [ebp-10h]

  result = -1610612735;
  if ( a2 < 0x1000 )
  {
    for ( i = *(_DWORD *)(a1 + 4); a2 >= i; i *= 2 )
      ;
    if ( i > *(_DWORD *)(a1 + 4) )
    {
      v16 = (char *)_aligned_malloc(96 * i, 0x10u);
      memcpy_0(v16, *(const void **)(a1 + 8), 96 * *(_DWORD *)(a1 + 4));
      memset(&v16[96 * *(_DWORD *)(a1 + 4)], 0, 96 * (i - *(_DWORD *)(a1 + 4)));
      _aligned_free(*(void **)(a1 + 8));
      *(_DWORD *)(a1 + 8) = v16;
      *(_DWORD *)(a1 + 4) = i;
    }
    if ( a9 < 1.0 )
    {
      return -1610612734;
    }
    else if ( (_BYTE)a12 )
    {
      v15 = (__m128 *)(*(_DWORD *)(a1 + 8) + 96 * a2);
      v15->m128_f32[1] = a9;
      v15[1].m128_i32[1] = a13;
      v15[1].m128_i32[0] = a12;
      v15->m128_f32[2] = a10;
      v15->m128_i32[0] = 1;
      v15->m128_f32[3] = a11;
      v15[1].m128_i32[2] = -1;
      v15[2].m128_i32[0] = 0;
      v15[2].m128_i32[1] = 0;
      v15[1].m128_i32[3] = -1;
      v15[2].m128_i32[2] = 0;
      v15[2].m128_i32[3] = 0;
      *(float *)&v17 = sub_4010B0(a5, 0.0009765625);
      v20 = (__m128)v17;
      *(float *)&v18 = sub_4010B0(a4, 0.0009765625);
      v21 = (__m128)v18;
      *(float *)&v19 = sub_4010B0(a3, 0.0009765625);
      v15[3] = _mm_unpacklo_ps(
                 _mm_unpacklo_ps((__m128)v19, v20),
                 _mm_unpacklo_ps(v21, (__m128)COERCE_UNSIGNED_INT(0.0)));
      sub_455940(a6, a7, a8);
      return 0;
    }
    else
    {
      return -1610612733;
    }
  }
  return result;
}

// ===== sub_455780 @ 0x00455780..0x00455811 =====
int __stdcall sub_455780(int a1, unsigned int a2)
{
  unsigned int v2; // edx
  int result; // eax
  unsigned int *v4; // esi
  unsigned int v5; // ebx
  unsigned int *v6; // esi
  unsigned int i; // edi
  int v8; // eax
  unsigned int *v9; // [esp+0h] [ebp-4h]

  v2 = a2;
  result = -1610612735;
  if ( a2 < *(_DWORD *)(a1 + 4) )
  {
    v4 = (unsigned int *)(*(_DWORD *)(a1 + 8) + 96 * a2);
    v9 = v4;
    if ( *v4 )
    {
      v5 = 0;
      v6 = v4 + 8;
      while ( 1 )
      {
        sub_455AC0(v2, v5, -1);
        for ( i = 0; i < *v6; ++i )
        {
          v8 = *(_DWORD *)(v6[1] + 4 * i);
          if ( v8 != -1 )
            sub_455AC0(v8, v5, -1);
        }
        operator delete((void *)v6[1]);
        ++v5;
        v6 += 2;
        if ( v5 >= 2 )
          break;
        v2 = a2;
      }
      memset(v9, 0, 0x60u);
      return 0;
    }
  }
  return result;
}

// ===== sub_455820 @ 0x00455820..0x00455905 =====
int __fastcall sub_455820(unsigned int a1, int a2, float a3, float a4, float a5)
{
  int result; // eax
  __m128 *v6; // esi
  unsigned int v7; // [esp+1Ch] [ebp-34h]
  unsigned int v8; // [esp+1Ch] [ebp-34h]
  unsigned int v9; // [esp+1Ch] [ebp-34h]
  __m128 v10; // [esp+20h] [ebp-30h]
  __m128 v11; // [esp+40h] [ebp-10h]

  result = -1610612735;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    v6 = (__m128 *)(*(_DWORD *)(a2 + 8) + 96 * a1);
    if ( v6->m128_i32[0] )
    {
      *(float *)&v7 = sub_4010B0(a5, 0.0009765625);
      v10 = (__m128)v7;
      *(float *)&v8 = sub_4010B0(a4, 0.0009765625);
      v11 = (__m128)v8;
      *(float *)&v9 = sub_4010B0(a3, 0.0009765625);
      result = 0;
      v6[3] = _mm_unpacklo_ps(_mm_unpacklo_ps((__m128)v9, v10), _mm_unpacklo_ps(v11, (__m128)COERCE_UNSIGNED_INT(0.0)));
    }
  }
  return result;
}

// ===== sub_455910 @ 0x00455910..0x0045593B =====
int __fastcall sub_455910(unsigned int a1, int a2, _OWORD *a3)
{
  int result; // eax
  int v4; // ecx

  result = -1610612735;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    v4 = *(_DWORD *)(a2 + 8) + 96 * a1;
    if ( *(_DWORD *)v4 )
    {
      *a3 = *(_OWORD *)(v4 + 48);
      return 0;
    }
  }
  return result;
}

// ===== sub_455940 @ 0x00455940..0x00455A8D =====
int __fastcall sub_455940(unsigned int a1, int a2, float a3, float a4, float a5)
{
  int result; // eax
  int v6; // esi
  __m128 v7; // xmm1
  __m128 v8; // xmm0
  __m128 v9; // xmm2
  __m128 v10; // xmm0
  float v11; // [esp+18h] [ebp-10h]
  float v12; // [esp+18h] [ebp-10h]
  unsigned int v13; // [esp+18h] [ebp-10h]
  float v14; // [esp+1Ch] [ebp-Ch]
  float v15; // [esp+24h] [ebp-4h]

  result = -1610612735;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    v6 = *(_DWORD *)(a2 + 8) + 96 * a1;
    if ( *(_DWORD *)v6 )
    {
      v11 = sub_4010B0(a5, 0.0009765625);
      v14 = v11;
      v12 = sub_4010B0(a4, 0.0009765625);
      v15 = v12;
      *(float *)&v13 = sub_4010B0(a3, 0.0009765625);
      result = 0;
      v7 = _mm_unpacklo_ps(
             _mm_unpacklo_ps((__m128)v13, (__m128)LODWORD(v14)),
             _mm_unpacklo_ps((__m128)LODWORD(v15), (__m128)COERCE_UNSIGNED_INT(0.0)));
      v8 = _mm_mul_ps(v7, v7);
      v8.m128_f32[0] = v8.m128_f32[0]
                     + (float)(_mm_shuffle_ps(v8, v8, 253).m128_f32[0] + _mm_shuffle_ps(v8, v8, 254).m128_f32[0]);
      v9 = v8;
      v9.m128_f32[0] = fsqrt(v8.m128_f32[0]);
      v9.m128_f32[0] = (float)((float)(1.0 / v9.m128_f32[0]) * 1.5)
                     + (float)((float)((float)((float)(v8.m128_f32[0] * (float)(1.0 / v9.m128_f32[0]))
                                             * (float)(1.0 / v9.m128_f32[0]))
                                     * (float)(1.0 / v9.m128_f32[0]))
                             * -0.5);
      v10 = _mm_shuffle_ps((__m128)*(unsigned int *)(v6 + 8), (__m128)*(unsigned int *)(v6 + 8), 0);
      *(__m128 *)(v6 + 64) = v7;
      *(__m128 *)(v6 + 80) = _mm_mul_ps(_mm_mul_ps(_mm_shuffle_ps(v9, v9, 192), v7), v10);
    }
  }
  return result;
}

// ===== sub_455A90 @ 0x00455A90..0x00455ABB =====
int __fastcall sub_455A90(unsigned int a1, int a2, _OWORD *a3)
{
  int result; // eax
  int v4; // ecx

  result = -1610612735;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    v4 = *(_DWORD *)(a2 + 8) + 96 * a1;
    if ( *(_DWORD *)v4 )
    {
      *a3 = *(_OWORD *)(v4 + 64);
      return 0;
    }
  }
  return result;
}

// ===== sub_455AC0 @ 0x00455AC0..0x00455C8B =====
int __thiscall sub_455AC0(_DWORD *this, unsigned int a2, unsigned int a3, unsigned int a4)
{
  unsigned int v4; // esi
  unsigned int v5; // edi
  int v6; // edx
  _DWORD *v7; // eax
  _DWORD *v9; // esi
  unsigned int v10; // edx
  unsigned int v11; // eax
  _DWORD *v12; // ecx
  const void **v13; // edi
  char *v14; // ecx
  unsigned int v15; // eax
  unsigned int v17; // eax
  unsigned int *v18; // edx
  unsigned int v19; // ecx
  int v20; // eax
  _DWORD *v21; // edx
  _DWORD *i; // esi
  _DWORD *v23; // [esp+Ch] [ebp-Ch]
  char *v24; // [esp+10h] [ebp-8h]
  const void **v25; // [esp+14h] [ebp-4h]
  unsigned int v26; // [esp+24h] [ebp+Ch]

  v4 = this[1];
  v5 = a2;
  if ( a2 >= v4 )
    return -1610612735;
  v6 = this[2];
  v7 = (_DWORD *)(v6 + 96 * a2);
  v23 = v7;
  if ( !*v7 )
    return -1610612735;
  if ( a3 >= 2 )
    return -1610612731;
  if ( a4 < v4 && a4 != a2 )
  {
    v9 = (_DWORD *)(v6 + 96 * a4);
    if ( *v9 )
    {
      sub_455AC0(a2, a3, -1);
      v10 = v9[2 * a3 + 8];
      v11 = 0;
      if ( v10 )
      {
        v25 = (const void **)&v9[2 * a3 + 9];
        v12 = *v25;
        while ( *v12 != -1 )
        {
          ++v11;
          ++v12;
          if ( v11 >= v10 )
            goto LABEL_13;
        }
        if ( v11 != -1 )
          goto LABEL_20;
LABEL_13:
        v26 = 2 * v10;
      }
      else
      {
        v26 = 64;
      }
      v13 = (const void **)&v9[2 * a3 + 9];
      v14 = (char *)operator new(4 * v26);
      v24 = v14;
      v25 = v13;
      if ( *v13 )
      {
        memcpy_0(v14, *v13, 4 * v9[2 * a3 + 8]);
        operator delete((void *)*v13);
        v14 = v24;
      }
      v15 = v9[2 * a3 + 8] + 1;
      if ( v15 < v26 )
      {
        memset(&v14[4 * v15], 0xFFu, 4 * (v26 - v15));
        v14 = v24;
        v13 = (const void **)&v9[2 * a3 + 9];
      }
      v11 = v9[2 * a3 + 8];
      *v13 = v14;
      v5 = a2;
      v9[2 * a3 + 8] = v26;
LABEL_20:
      *((_DWORD *)*v25 + v11) = v5;
      v23[a3 + 6] = a4;
      return 0;
    }
    return -1610612734;
  }
  if ( a4 != -1 )
    return -1610612734;
  v17 = v7[a3 + 6];
  if ( v17 < v4 )
  {
    v18 = (unsigned int *)(v6 + 8 * (a3 + 12 * v17) + 32);
    v19 = *v18;
    v20 = 0;
    if ( *v18 )
    {
      v21 = (_DWORD *)v18[1];
      for ( i = v21; *i != a2; ++i )
      {
        if ( ++v20 >= v19 )
        {
          v23[a3 + 6] = -1;
          return 0;
        }
      }
      v21[v20] = -1;
    }
    v23[a3 + 6] = -1;
  }
  return 0;
}

// ===== sub_455C90 @ 0x00455C90..0x00455CC8 =====
int __userpurge sub_455C90@<eax>(int a1@<edx>, unsigned int a2@<ecx>, unsigned int a3@<esi>, _DWORD *a4)
{
  int result; // eax
  _DWORD *v5; // ecx

  result = -1610612735;
  if ( a2 < *(_DWORD *)(a1 + 4) )
  {
    v5 = (_DWORD *)(*(_DWORD *)(a1 + 8) + 96 * a2);
    if ( *v5 )
    {
      if ( a3 >= 2 )
      {
        return -1610612731;
      }
      else
      {
        *a4 = v5[a3 + 6];
        return 0;
      }
    }
  }
  return result;
}

// ===== sub_455CD0 @ 0x00455CD0..0x00455D2E =====
int __userpurge sub_455CD0@<eax>(int a1@<edx>, unsigned int a2@<ecx>, unsigned int a3@<edi>, int a4, _DWORD *a5)
{
  int result; // eax
  _DWORD *v6; // ecx
  unsigned int v7; // edx
  int i; // esi
  int v9; // eax

  result = -1610612735;
  if ( a2 < *(_DWORD *)(a1 + 4) )
  {
    v6 = (_DWORD *)(*(_DWORD *)(a1 + 8) + 96 * a2);
    if ( *v6 )
    {
      if ( a3 >= 2 )
      {
        return -1610612731;
      }
      else
      {
        v7 = 0;
        for ( i = 0; v7 < v6[2 * a3 + 8]; ++v7 )
        {
          v9 = *(_DWORD *)(v6[2 * a3 + 9] + 4 * v7);
          if ( v9 != -1 )
            *(_DWORD *)(a4 + 4 * i++) = v9;
        }
        *a5 = i;
        return 0;
      }
    }
  }
  return result;
}

// ===== sub_455D30 @ 0x00455D30..0x00455D95 =====
int __userpurge sub_455D30@<eax>(unsigned int a1@<edx>, int a2@<esi>, int a3, int a4, int a5, int a6, int a7)
{
  int result; // eax
  int v8; // ecx

  result = -1610612735;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    v8 = *(_DWORD *)(a2 + 8) + 96 * a1;
    if ( *(_DWORD *)v8 )
      return sub_4569F0(a2, *(float *)(v8 + 4), *(_DWORD *)(v8 + 20), a1, a6, a7);
  }
  return result;
}

// ===== sub_455DA0 @ 0x00455DA0..0x004565B9 =====
int __userpurge sub_455DA0@<eax>(
        _DWORD *a1@<ecx>,
        int a2@<esi>,
        __int32 *a3,
        unsigned int a4,
        unsigned int a5,
        float a6,
        unsigned int a7,
        int a8,
        int a9,
        int a10,
        int a11)
{
  unsigned int v11; // ecx
  int result; // eax
  int v13; // edx
  __m128 *v14; // ebx
  __m128 *v15; // edi
  __m128 v16; // xmm0
  __m128 v17; // xmm1
  int v18; // eax
  __m128i v19; // xmm0
  int v20; // eax
  int v21; // ebx
  size_t v22; // edi
  void *v23; // eax
  unsigned int v24; // eax
  unsigned int v25; // ecx
  int *v26; // eax
  int v27; // edx
  unsigned int v28; // edi
  __int32 v29; // edx
  int v30; // eax
  void *v31; // eax
  double v32; // st7
  int v33; // edi
  int v34; // ebx
  double v35; // st5
  int v36; // eax
  int v37; // ecx
  int v38; // edx
  int v39; // ecx
  bool v40; // zf
  int v41; // ebx
  int v42; // edi
  _DWORD *v43; // edx
  int v44; // ecx
  int v45; // ecx
  int v46; // ecx
  int v47; // ecx
  int v48; // ebx
  unsigned int v49; // edx
  unsigned int v50; // ecx
  __int32 v51; // eax
  __m128 *v52; // edi
  int v53; // eax
  __int32 *v54; // ebx
  __m128i *v55; // eax
  __m128i v56; // xmm3
  __m128i v57; // xmm0
  __m128i v58; // xmm1
  __m128i v59; // xmm4
  __m128i v60; // xmm1
  __int32 v61; // ecx
  int v62; // edi
  int v64; // edi
  __int32 *X_4; // [esp+4h] [ebp-B4h]
  __m128 *v66; // [esp+10h] [ebp-A8h]
  int v67; // [esp+10h] [ebp-A8h]
  unsigned int v68; // [esp+10h] [ebp-A8h]
  unsigned int v69; // [esp+14h] [ebp-A4h]
  float v70; // [esp+14h] [ebp-A4h]
  unsigned int v71; // [esp+14h] [ebp-A4h]
  float v72; // [esp+14h] [ebp-A4h]
  int v73; // [esp+14h] [ebp-A4h]
  __m128 *v74; // [esp+18h] [ebp-A0h]
  int v75; // [esp+18h] [ebp-A0h]
  int v76; // [esp+18h] [ebp-A0h]
  unsigned int v77; // [esp+18h] [ebp-A0h]
  float v78; // [esp+1Ch] [ebp-9Ch]
  __m128 *v79; // [esp+1Ch] [ebp-9Ch]
  int v80; // [esp+1Ch] [ebp-9Ch]
  int v81; // [esp+1Ch] [ebp-9Ch]
  int epi16; // [esp+20h] [ebp-98h]
  int v83; // [esp+20h] [ebp-98h]
  int v84; // [esp+28h] [ebp-90h]
  __m128 v86; // [esp+38h] [ebp-80h]
  char v87[80]; // [esp+48h] [ebp-70h] BYREF
  __m128 v88; // [esp+98h] [ebp-20h]

  v11 = *(_DWORD *)(a2 + 4);
  result = -1610612735;
  if ( a4 < v11 )
  {
    v13 = *(_DWORD *)(a2 + 8);
    v14 = (__m128 *)(v13 + 96 * a4);
    v66 = v14;
    if ( v14->m128_i32[0] )
    {
      result = -1610612734;
      if ( a5 < v11 )
      {
        v15 = (__m128 *)(v13 + 96 * a5);
        v74 = v15;
        if ( v15->m128_i32[0] )
        {
          _mm_setcsr(_mm_getcsr() & 0xF3FF);
          if ( 0.0 == *(float *)&a7 )
            *(float *)&a7 = 1.0;
          v16 = _mm_shuffle_ps((__m128)a7, (__m128)a7, 0);
          v17 = _mm_rcp_ps(v16);
          v86 = _mm_sub_ps(_mm_add_ps(v17, v17), _mm_mul_ps(_mm_mul_ps(v17, v17), v16));
          v78 = ceil((float)(a6 * v86.m128_f32[0]));
          v18 = abs32((int)v78);
          if ( v18 < 1 )
            v18 = 1;
          v84 = 2 * v18;
          v19 = _mm_add_epi32(
                  _mm_shuffle_epi32(_mm_cvtsi32_si128(2 * v18), 0),
                  _mm_cvtps_epi32(_mm_mul_ps(_mm_mul_ps(_mm_sub_ps(v15[3], v14[3]), (__m128)xmmword_4E4260), v86)));
          v79 = v14 + 3;
          v20 = 4 * v18 + 1;
          epi16 = _mm_extract_epi16(v19, 0);
          v21 = _mm_extract_epi16(v19, 2);
          if ( epi16 >= v20 || v21 >= v20 )
          {
            return -1610612730;
          }
          else
          {
            v22 = 32 * v20 * v20;
            *(_DWORD *)(a2 + 12) = v20;
            *(_DWORD *)(a2 + 16) = v20;
            v23 = _aligned_malloc(v22, 0x10u);
            *(_DWORD *)(a2 + 20) = v23;
            memset(v23, 0, v22);
            memset((void *)(a2 + 24), 0, 0x100u);
            v24 = abs32((int)sub_455410((float)((float)(v66->m128_f32[1] + v74->m128_f32[1]) * 2.0) * v86.m128_f32[0]));
            v69 = v24 + v21;
            v25 = v21 - v24;
            v26 = (int *)(a2 + 28);
            v27 = epi16 - v21;
            v75 = 2;
            do
            {
              *(v26 - 1) = v27 + v69;
              v26[7] = epi16;
              v28 = v69++;
              v26[8] = v28;
              v26[15] = v27 + v25;
              v26[24] = v25;
              *v26 = v21;
              v26[16] = v21;
              v26[23] = epi16;
              --v25;
              v26 += 16;
              --v75;
            }
            while ( v75 );
            *(float *)(a2 + 280) = v66->m128_f32[1];
            v29 = v66[1].m128_i32[1];
            *(_DWORD *)(a2 + 288) = a4;
            *(float *)(a2 + 292) = a6;
            *(_DWORD *)(a2 + 284) = v29;
            *(_DWORD *)(a2 + 304) = a9;
            v30 = a10;
            *(_DWORD *)(a2 + 300) = a8;
            *(_DWORD *)(a2 + 296) = v79;
            if ( 4 << a10 > 16 )
              v30 = 2;
            *(_DWORD *)(a2 + 308) = v30;
            v31 = _aligned_malloc(0x100u, 0x10u);
            *(_DWORD *)(a2 + 312) = v31;
            memset(v31, 0, 0x100u);
            v32 = *(float *)&a7;
            v33 = -2;
            v34 = 2;
            v70 = -*(float *)&a7;
            *(_DWORD *)(a2 + 316) = _aligned_malloc(0x100u, 0x10u);
            v35 = v70;
            v76 = -2;
            v67 = 2;
            v36 = 96;
            v83 = 4;
            do
            {
              v37 = *(_DWORD *)(a2 + 312);
              v38 = 2 * (v33 & 0xF);
              *(_DWORD *)(v37 + 8 * v38) = 2;
              *(_DWORD *)(v37 + 8 * v38 + 4) = v33;
              *(_DWORD *)(v36 + v37 - 64) = v34;
              *(_DWORD *)(v36 + v37 - 60) = 2;
              *(_DWORD *)(v36 + v37) = -2;
              *(_DWORD *)(v36 + v37 + 4) = v34;
              *(float *)&v71 = (double)v76 * v32 * 0.5;
              *(_DWORD *)(v36 + v37 + 64) = v33;
              *(_DWORD *)(v36 + v37 + 68) = -2;
              v39 = *(_DWORD *)(a2 + 316);
              v36 += 16;
              *(__m128 *)(v39 + 8 * v38) = _mm_unpacklo_ps(
                                             _mm_unpacklo_ps((__m128)a7, (__m128)COERCE_UNSIGNED_INT(0.0)),
                                             _mm_unpacklo_ps((__m128)v71, (__m128)COERCE_UNSIGNED_INT(0.0)));
              *(float *)&v77 = (double)v67 * v32 * 0.5;
              *(__m128 *)(v36 + v39 - 80) = _mm_unpacklo_ps(
                                              _mm_unpacklo_ps((__m128)v77, (__m128)COERCE_UNSIGNED_INT(0.0)),
                                              _mm_unpacklo_ps((__m128)a7, (__m128)COERCE_UNSIGNED_INT(0.0)));
              *(float *)&v68 = v35;
              *(__m128 *)(v36 + v39 - 16) = _mm_unpacklo_ps(
                                              _mm_unpacklo_ps((__m128)v68, (__m128)COERCE_UNSIGNED_INT(0.0)),
                                              _mm_unpacklo_ps((__m128)v77, (__m128)COERCE_UNSIGNED_INT(0.0)));
              *(__m128 *)(v36 + v39 + 48) = _mm_unpacklo_ps(
                                              _mm_unpacklo_ps((__m128)v71, (__m128)COERCE_UNSIGNED_INT(0.0)),
                                              _mm_unpacklo_ps((__m128)v68, (__m128)COERCE_UNSIGNED_INT(0.0)));
              --v34;
              ++v33;
              v40 = v83-- == 1;
              v67 = v34;
              v76 = v33;
            }
            while ( !v40 );
            sub_457080(v84, v84, -1, 0.0);
            v41 = -1;
            if ( a11 && (unsigned int)sub_490AE0() >= 2 )
            {
              *(_DWORD *)(a2 + 324) = a11;
              *(_DWORD *)(a2 + 328) = 1;
              sub_4464D0(a11, (int)sub_457490, a2);
              sub_4464E0(1, a11);
              sub_4464D0(a11, 0, 0);
              v72 = 3.4028235e38;
              v42 = 2;
              v43 = (_DWORD *)(a2 + 24);
              v80 = 4;
              do
              {
                v44 = *(_DWORD *)(a2 + 20) + 32 * (*v43 + *(_DWORD *)(a2 + 12) * v43[1]);
                if ( 0.0 != *(float *)(v44 + 4) && v72 > (double)*(float *)(v44 + 4) )
                {
                  v41 = v42 - 2;
                  v72 = *(float *)(v44 + 4);
                }
                v45 = *(_DWORD *)(a2 + 20) + 32 * (v43[4] + *(_DWORD *)(a2 + 12) * v43[5]);
                if ( 0.0 != *(float *)(v45 + 4) && v72 > (double)*(float *)(v45 + 4) )
                {
                  v41 = v42 - 1;
                  v72 = *(float *)(v45 + 4);
                }
                v46 = *(_DWORD *)(a2 + 20) + 32 * (v43[8] + *(_DWORD *)(a2 + 12) * v43[9]);
                if ( 0.0 != *(float *)(v46 + 4) && v72 > (double)*(float *)(v46 + 4) )
                {
                  v41 = v42;
                  v72 = *(float *)(v46 + 4);
                }
                v47 = *(_DWORD *)(a2 + 20) + 32 * (v43[12] + *(_DWORD *)(a2 + 12) * v43[13]);
                if ( 0.0 != *(float *)(v47 + 4) && v72 > (double)*(float *)(v47 + 4) )
                {
                  v41 = v42 + 1;
                  v72 = *(float *)(v47 + 4);
                }
                v42 += 4;
                v43 += 16;
                --v80;
              }
              while ( v80 );
            }
            else if ( sub_457170(a2) )
            {
              do
                v41 = sub_4571C0(a2, v87, 0);
              while ( v41 < 0 && sub_457170(a2) );
            }
            while ( sub_457170(a2) )
              ;
            if ( v41 < 0 )
            {
              v64 = -1610612730;
            }
            else
            {
              v48 = 2 * v41;
              v49 = *(_DWORD *)(a2 + 8 * v48 + 28);
              v50 = *(_DWORD *)(a2 + 8 * v48 + 24);
              v51 = *(_DWORD *)(a2 + 8 * v48 + 36);
              v88.m128_i32[2] = *(_DWORD *)(a2 + 8 * v48 + 32);
              v52 = (__m128 *)(*(_DWORD *)(a2 + 20) + 32 * (v50 + v49 * *(_DWORD *)(a2 + 12)));
              v88.m128_i32[3] = v51;
              v53 = 0;
              v54 = 0;
              v40 = v52->m128_i32[0] == -1;
              v88.m128_u64[0] = __PAIR64__(v49, v50);
              v73 = 0;
              if ( !v40 )
              {
                do
                {
                  v55 = (__m128i *)operator new(0x14u);
                  v56 = _mm_cvtps_epi32(_mm_mul_ps((__m128)xmmword_4E41D0, v52[1]));
                  v57 = _mm_and_si128(v56, _mm_load_si128((const __m128i *)&xmmword_4E42E0));
                  v58 = _mm_sub_epi32(v56, v57);
                  v59 = _mm_cmpgt_epi32(_mm_load_si128((const __m128i *)&xmmword_4E42D0), v58);
                  v60 = _mm_cmpgt_epi32(v58, _mm_load_si128((const __m128i *)&xmmword_4E42F0));
                  *v55 = _mm_or_si128(
                           _mm_andnot_si128(v60, _mm_or_si128(_mm_andnot_si128(v59, v56), _mm_and_si128(v59, v57))),
                           _mm_and_si128(v60, _mm_add_epi32(v57, (__m128i)xmmword_4E4300)));
                  v55[1].m128i_i32[0] = (__int32)v54;
                  v61 = v52->m128_i32[0];
                  v62 = *(_DWORD *)(a2 + 12);
                  ++v73;
                  v54 = (__int32 *)v55;
                  v88 = _mm_add_ps(*(__m128 *)(*(_DWORD *)(a2 + 312) + 16 * (((_BYTE)v61 - 8) & 0xF)), v88);
                  v52 = (__m128 *)(*(_DWORD *)(a2 + 20) + 32 * (v88.m128_i32[0] + v88.m128_i32[1] * v62));
                }
                while ( v52->m128_i32[0] != -1 );
                v53 = v73;
              }
              if ( v53 )
              {
                v81 = v53;
                do
                {
                  *a3 = *v54;
                  a3[1] = v54[1];
                  a3[2] = v54[2];
                  a3[3] = v54[3];
                  X_4 = v54;
                  v54 = (__int32 *)v54[4];
                  operator delete(X_4);
                  a3 += 4;
                  --v81;
                }
                while ( v81 );
                v53 = v73;
              }
              *a1 = v53;
              v64 = 0;
            }
            _aligned_free(*(void **)(a2 + 316));
            _aligned_free(*(void **)(a2 + 312));
            _aligned_free(*(void **)(a2 + 20));
            return v64;
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_4565C0 @ 0x004565C0..0x00456616 =====
int __userpurge sub_4565C0@<eax>(int a1@<eax>, unsigned int a2@<edx>, int a3, int a4, char a5)
{
  int result; // eax
  int v7; // ecx
  char v8; // al

  result = -1610612735;
  if ( a2 < *(_DWORD *)(a1 + 4) )
  {
    v7 = *(_DWORD *)(a1 + 8) + 96 * a2;
    if ( *(_DWORD *)v7 )
    {
      v8 = a5;
      if ( !a5 )
        v8 = -1;
      return sub_456FC0(a3, a4, *(float *)(v7 + 12), a2, v8);
    }
  }
  return result;
}

// ===== sub_456620 @ 0x00456620..0x0045666B =====
int __userpurge sub_456620@<eax>(char a1@<al>, int a2@<ecx>, int a3, int a4, int a5, int a6, float a7, int a8)
{
  if ( !a1 )
    a1 = -1;
  return sub_456FC0(a3, a2, LODWORD(a7), a8, a1);
}

// ===== sub_456670 @ 0x00456670..0x004568B5 =====
int __stdcall sub_456670(int a1, int a2, _DWORD *a3, unsigned int a4, char a5)
{
  unsigned int v5; // esi
  int result; // eax
  __m128 *v7; // ebx
  unsigned __int32 v8; // edi
  __m128 *v9; // eax
  __m128 *v10; // edx
  int v11; // esi
  __m128 *v12; // ecx
  __m128 *v13; // esi
  __m128 v14; // xmm2
  float v15; // xmm1_4
  __m128 v16; // xmm3
  __m128 v17; // xmm1
  __m128 v18; // xmm0
  __m128 *v19; // eax
  __m128 **i; // esi
  __m128 *v21; // ecx
  int v22; // esi
  __m128i *v23; // eax
  __m128i v24; // xmm3
  __m128i v25; // xmm0
  __m128i v26; // xmm1
  __m128i v27; // xmm4
  __m128i v28; // xmm1
  void *v29; // [esp-4h] [ebp-34h]
  int v30; // [esp+10h] [ebp-20h]
  __m128 *v31; // [esp+14h] [ebp-1Ch] BYREF
  unsigned int v32; // [esp+18h] [ebp-18h]
  void *Block; // [esp+1Ch] [ebp-14h]

  v5 = *(_DWORD *)(a1 + 4);
  result = -1610612735;
  if ( a4 < v5 )
  {
    v7 = (__m128 *)(*(_DWORD *)(a1 + 8) + 96 * a4);
    v8 = 0;
    if ( v7->m128_i32[0] )
    {
      if ( !a5 )
        a5 = -1;
      v9 = (__m128 *)_aligned_malloc(32 * v5, 0x10u);
      v10 = 0;
      Block = v9;
      v31 = 0;
      v30 = 0;
      if ( v5 )
      {
        v11 = 0;
        v32 = 0;
        v12 = v9;
        do
        {
          v13 = (__m128 *)(*(_DWORD *)(a1 + 8) + v11);
          if ( v13->m128_i32[0] && v8 != a4 && ((unsigned __int8)a5 & v13[1].m128_i8[0]) != 0 )
          {
            v14 = _mm_sub_ps(v13[3], v7[3]);
            ++v30;
            v18 = _mm_mul_ps(v14, v14);
            v18.m128_f32[0] = v18.m128_f32[0]
                            + (float)(_mm_shuffle_ps(v18, v18, 253).m128_f32[0]
                                    + _mm_shuffle_ps(v18, v18, 254).m128_f32[0]);
            v15 = fsqrt(v18.m128_f32[0]);
            v16 = v18;
            v16.m128_f32[0] = (float)((float)((float)((float)(v18.m128_f32[0] * (float)(1.0 / v15)) * (float)(1.0 / v15))
                                            * (float)(1.0 / v15))
                                    * -0.5)
                            + (float)((float)(1.0 / v15) * 1.5);
            v17 = _mm_mul_ps(_mm_shuffle_ps(v16, v16, 192), v14);
            v18.m128_f32[0] = v18.m128_f32[0] * v16.m128_f32[0];
            v19 = v12;
            v12[1] = _mm_shuffle_ps(v17, _mm_shuffle_ps(v17, v18, 10), 132);
            v12->m128_i32[0] = v8;
            v12 += 2;
            for ( i = &v31; v10; v10 = (__m128 *)v10->m128_i32[1] )
            {
              if ( v18.m128_f32[0] < v10[1].m128_f32[3] )
                break;
              i = (__m128 **)&v10->m128_i32[1];
            }
            v19->m128_i32[1] = (__int32)v10;
            *i = v19;
            v10 = v31;
          }
          ++v8;
          v11 = v32 + 96;
          v32 += 96;
        }
        while ( v8 < *(_DWORD *)(a1 + 4) );
      }
      v32 = _mm_getcsr() & 0xF3FF;
      _mm_setcsr(v32);
      v21 = v31;
      if ( v30 )
      {
        v22 = v30;
        v23 = (__m128i *)(a2 + 4);
        do
        {
          v23[-1].m128i_i32[3] = v21->m128_i32[0];
          v24 = _mm_cvtps_epi32(_mm_mul_ps((__m128)xmmword_4E41D0, v21[1]));
          v25 = _mm_and_si128(v24, _mm_load_si128((const __m128i *)&xmmword_4E42E0));
          v26 = _mm_sub_epi32(v24, v25);
          v27 = _mm_cmpgt_epi32(_mm_load_si128((const __m128i *)&xmmword_4E42D0), v26);
          v28 = _mm_cmpgt_epi32(v26, _mm_load_si128((const __m128i *)&xmmword_4E42F0));
          *v23 = _mm_or_si128(
                   _mm_andnot_si128(v28, _mm_or_si128(_mm_andnot_si128(v27, v24), _mm_and_si128(v27, v25))),
                   _mm_and_si128(v28, _mm_add_epi32(v25, (__m128i)xmmword_4E4300)));
          v21 = (__m128 *)v21->m128_i32[1];
          v23 = (__m128i *)((char *)v23 + 20);
          --v22;
        }
        while ( v22 );
      }
      v29 = Block;
      *a3 = v30;
      _aligned_free(v29);
      return 0;
    }
  }
  return result;
}

// ===== sub_4568C0 @ 0x004568C0..0x004569E9 =====
int __userpurge sub_4568C0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4)
{
  unsigned int v4; // edx
  int v5; // esi
  int v6; // ecx
  int v7; // eax
  __m128 v8; // xmm2
  __m128 v9; // xmm0
  float v10; // xmm1_4
  __m128 v11; // xmm3
  __m128 v12; // xmm1
  __m128 v14; // [esp+10h] [ebp-10h] BYREF

  v4 = *(_DWORD *)(a3 + 4);
  if ( a2 >= v4 )
    return -1610612735;
  v5 = *(_DWORD *)(a3 + 8);
  v6 = 96 * a2;
  if ( !*(_DWORD *)(v6 + v5) )
    return -1610612735;
  if ( a1 >= v4 )
    return -1610612734;
  v7 = 96 * a1;
  if ( !*(_DWORD *)(v7 + v5) )
    return -1610612734;
  v8 = _mm_sub_ps(*(__m128 *)(v7 + v5 + 48), *(__m128 *)(v6 + v5 + 48));
  v9 = _mm_mul_ps(v8, v8);
  v9.m128_f32[0] = v9.m128_f32[0]
                 + (float)(_mm_shuffle_ps(v9, v9, 253).m128_f32[0] + _mm_shuffle_ps(v9, v9, 254).m128_f32[0]);
  v10 = fsqrt(v9.m128_f32[0]);
  v11 = v9;
  v11.m128_f32[0] = (float)((float)((float)((float)(v9.m128_f32[0] * (float)(1.0 / v10)) * (float)(1.0 / v10))
                                  * (float)(1.0 / v10))
                          * -0.5)
                  + (float)((float)(1.0 / v10) * 1.5);
  v12 = _mm_mul_ps(_mm_shuffle_ps(v11, v11, 192), v8);
  v9.m128_f32[0] = v9.m128_f32[0] * v11.m128_f32[0];
  v14 = _mm_shuffle_ps(v12, _mm_shuffle_ps(v12, v9, 10), 132);
  _mm_setcsr(_mm_getcsr() & 0xF3FF);
  *(__m128i *)(a4 + 4) = sub_455490(&v14);
  return 0;
}

// ===== sub_4569F0 @ 0x004569F0..0x00456FB2 =====
int __userpurge sub_4569F0@<eax>(
        int a1@<ebp>,
        __m128 a2@<xmm0>,
        __m128 a3@<xmm1>,
        int a4,
        unsigned int a5,
        __int32 a6,
        int a7,
        int a8,
        int a9)
{
  double *v9; // edi
  unsigned int v10; // esi
  long double v11; // st7
  __m128 v12; // xmm0
  __m128 v13; // xmm1
  __m128 v14; // xmm0
  __m128 v15; // xmm0
  __m128 v16; // xmm0
  __int128 v17; // xmm1
  __m128 v18; // xmm0
  __m128 v19; // xmm1
  __m128 v20; // xmm0
  __m128 v21; // xmm0
  __m128 v22; // xmm0
  __int128 v23; // xmm1
  unsigned int v24; // esi
  int v25; // eax
  __m128 *i; // edi
  __m128 v27; // xmm4
  __m128 v28; // xmm1
  __m128 v29; // xmm1
  __m128 v30; // xmm0
  __m128 v31; // xmm0
  float v32; // xmm1_4
  float v33; // xmm3_4
  float v34; // xmm2_4
  BOOL v36; // edx
  BOOL v37; // esi
  unsigned int v38; // ecx
  __m128 v39; // xmm0
  __m128 v40; // xmm1
  float v41; // xmm1_4
  __m128 v42; // xmm0
  __m128 v43; // xmm0
  __m128 v44; // xmm1
  float v45; // xmm1_4
  __m128 v46; // xmm0
  float v47; // xmm0_4
  __m128 v48; // [esp-200h] [ebp-20Ch]
  double v49[2]; // [esp-1F0h] [ebp-1FCh] BYREF
  double v50; // [esp-1E0h] [ebp-1ECh]
  double v51; // [esp-1D8h] [ebp-1E4h]
  __m128 v52; // [esp-1D0h] [ebp-1DCh]
  __m128 v53; // [esp-1C0h] [ebp-1CCh]
  long double v54; // [esp-1A8h] [ebp-1B4h]
  unsigned int v55; // [esp-1A0h] [ebp-1ACh]
  unsigned int v56; // [esp-19Ch] [ebp-1A8h]
  int v57; // [esp-198h] [ebp-1A4h]
  unsigned int v58; // [esp-194h] [ebp-1A0h]
  _BYTE v59[256]; // [esp-190h] [ebp-19Ch]
  float v60[8]; // [esp-90h] [ebp-9Ch]
  float v61[27]; // [esp-70h] [ebp-7Ch]
  unsigned int v62; // [esp-4h] [ebp-10h]
  _DWORD v63[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v63[0] = a1;
  v63[1] = retaddr;
  v62 = (unsigned int)v63 ^ dword_4FB734;
  v52 = a3;
  v53 = a2;
  if ( !_mm_movemask_epi8((__m128i)_mm_cmpneq_ps(a2, a3)) )
    return sub_4AB245((unsigned int)v63 ^ v62);
  if ( !a8 )
    LOBYTE(a8) = -1;
  v48 = _mm_shuffle_ps((__m128)a5, (__m128)a5, 0);
  *(float *)&v57 = a3.m128_f32[0] - a2.m128_f32[0];
  *(float *)&v58 = v52.m128_f32[1] - v53.m128_f32[1];
  sub_4D58DA(*(float *)&v58);
  v50 = *(float *)&v58 + 1.570796326794897;
  *(float *)&v58 = v52.m128_f32[2] - v53.m128_f32[2];
  sub_4D58DA(*(float *)&v58);
  v9 = v49;
  v10 = 0;
  v51 = *(float *)&v58 + 1.570796326794897;
  v49[0] = 0.0;
  v49[1] = 3.141592653589793;
  do
  {
    v54 = *v9 + v50;
    *(float *)&v55 = 0.0;
    *(float *)&v57 = 0.0;
    *(float *)&v58 = sin(v54);
    v56 = v58;
    *(float *)&v58 = cos(v54);
    v11 = *v9 + v51;
    v54 = v11;
    v12 = _mm_mul_ps(
            _mm_unpacklo_ps(
              _mm_unpacklo_ps((__m128)v58, (__m128)COERCE_UNSIGNED_INT(0.0)),
              _mm_unpacklo_ps((__m128)v56, (__m128)COERCE_UNSIGNED_INT(0.0))),
            v48);
    v13 = _mm_add_ps(v12, v53);
    v14 = _mm_add_ps(v12, v52);
    *(__m128 *)&v59[v10 * 4] = v14;
    v15 = _mm_sub_ps(v14, v13);
    *(__m128 *)&v59[v10 * 4 + 224] = v13;
    *(__m128 *)&v59[v10 * 4 + 128] = v15;
    v16 = _mm_mul_ps(v15, v15);
    v16.m128_f32[0] = v16.m128_f32[0] + _mm_shuffle_ps(v16, v16, 253).m128_f32[0];
    *(__m128 *)&v61[v10 + 16] = v16;
    v17 = *(_OWORD *)&v61[v10 + 16];
    *(float *)&v17 = (float)((float)(1.0 / *(float *)&v17) + (float)(1.0 / *(float *)&v17))
                   - (float)(v61[v10 + 16] * (float)((float)(1.0 / *(float *)&v17) * (float)(1.0 / *(float *)&v17)));
    *(_OWORD *)&v59[v10 * 4 + 192] = v17;
    *(float *)&v58 = 0.0;
    *(float *)&v56 = sin(v11);
    v55 = v56;
    *(float *)&v57 = 0.0;
    *(float *)&v56 = cos(v54);
    v18 = _mm_mul_ps(
            _mm_unpacklo_ps(
              _mm_unpacklo_ps((__m128)v56, (__m128)v55),
              _mm_unpacklo_ps((__m128)COERCE_UNSIGNED_INT(0.0), (__m128)COERCE_UNSIGNED_INT(0.0))),
            v48);
    v19 = _mm_add_ps(v18, v53);
    v20 = _mm_add_ps(v18, v52);
    *(__m128 *)&v59[v10 * 4 + 32] = v20;
    v21 = _mm_sub_ps(v20, v19);
    *(__m128 *)&v59[v10 * 4 + 160] = v19;
    *(__m128 *)&v59[v10 * 4 + 64] = v21;
    v22 = _mm_mul_ps(v21, v21);
    v22.m128_f32[0] = v22.m128_f32[0] + _mm_shuffle_ps(v22, v22, 254).m128_f32[0];
    *(__m128 *)&v61[v10 + 8] = v22;
    v23 = *(_OWORD *)&v61[v10 + 8];
    *(float *)&v23 = (float)((float)(1.0 / *(float *)&v23) + (float)(1.0 / *(float *)&v23))
                   - (float)(v61[v10 + 8] * (float)((float)(1.0 / *(float *)&v23) * (float)(1.0 / *(float *)&v23)));
    *(_OWORD *)&v59[v10 * 4 + 96] = v23;
    v10 += 4;
    ++v9;
  }
  while ( v10 < 8 );
  v24 = *(_DWORD *)(a4 + 4);
  v25 = 0;
  *(float *)&v57 = 0.0;
  v56 = v24;
  if ( !v24 )
    return sub_4AB245((unsigned int)v63 ^ v62);
  for ( i = *(__m128 **)(a4 + 8); ; i += 6 )
  {
    if ( !i->m128_i32[0] || v25 == a7 || ((unsigned __int8)a8 & i[1].m128_i8[0]) == 0 )
      goto LABEL_47;
    v27 = i[3];
    v28 = _mm_sub_ps(v53, v27);
    v29 = _mm_mul_ps(v28, v28);
    v30 = _mm_sub_ps(v52, v27);
    v31 = _mm_mul_ps(v30, v30);
    v32 = v29.m128_f32[0]
        + (float)(_mm_shuffle_ps(v29, v29, 253).m128_f32[0] + _mm_shuffle_ps(v29, v29, 254).m128_f32[0]);
    v33 = i->m128_f32[1];
    v34 = (float)(v48.m128_f32[0] + v33) * (float)(v48.m128_f32[0] + v33);
    if ( (float)(v31.m128_f32[0]
               + (float)(_mm_shuffle_ps(v31, v31, 253).m128_f32[0] + _mm_shuffle_ps(v31, v31, 254).m128_f32[0])) >= v34 )
      break;
    if ( v32 >= v34 || a6 <= i[1].m128_i32[1] )
      goto LABEL_14;
LABEL_47:
    if ( ++v25 >= v24 )
      return sub_4AB245((unsigned int)v63 ^ v62);
  }
  if ( v32 < v34 || !a9 )
  {
LABEL_46:
    v24 = v56;
    goto LABEL_47;
  }
  v36 = 0;
  v37 = 0;
  v38 = 0;
  while ( 1 )
  {
    if ( !v36 )
    {
      v39 = _mm_sub_ps(v27, *(__m128 *)&v59[v38 * 4 + 224]);
      v40 = _mm_mul_ps(*(__m128 *)&v59[v38 * 4 + 128], v39);
      v41 = v40.m128_f32[0] + _mm_shuffle_ps(v40, v40, 253).m128_f32[0];
      if ( v41 > 0.0 && v61[v38 + 16] > v41 )
      {
        v42 = _mm_mul_ps(v39, v39);
        v42.m128_f32[0] = (float)(v42.m128_f32[0] + _mm_shuffle_ps(v42, v42, 253).m128_f32[0])
                        - (float)((float)(v41 * *(float *)&v59[v38 * 4 + 192]) * v41);
        if ( v42.m128_f32[0] >= (float)(v33 * v33) )
          *(__m128 *)&v60[v38] = v42;
        else
          v36 = 1;
      }
    }
    if ( !v37 )
    {
      v43 = _mm_sub_ps(v27, *(__m128 *)&v59[v38 * 4 + 160]);
      v44 = _mm_mul_ps(*(__m128 *)&v59[v38 * 4 + 64], v43);
      v45 = v44.m128_f32[0] + _mm_shuffle_ps(v44, v44, 254).m128_f32[0];
      if ( v45 > 0.0 && v61[v38 + 8] > v45 )
      {
        v46 = _mm_mul_ps(v43, v43);
        v46.m128_f32[0] = (float)(v46.m128_f32[0] + _mm_shuffle_ps(v46, v46, 254).m128_f32[0])
                        - (float)((float)(v45 * *(float *)&v59[v38 * 4 + 96]) * v45);
        if ( v46.m128_f32[0] >= (float)(v33 * v33) )
          *(__m128 *)&v61[v38] = v46;
        else
          v37 = 1;
      }
    }
    if ( v36 && v37 )
      break;
    v38 += 4;
    if ( v38 >= 8 )
    {
      v47 = (float)(v48.m128_f32[0] + v48.m128_f32[0]) * (float)(v48.m128_f32[0] + v48.m128_f32[0]);
      if ( !v36 )
        v36 = v60[0] < v47 && v60[4] < v47;
      if ( !v37 )
        v37 = v61[0] < v47 && v61[4] < v47;
      if ( v36 && v37 )
        break;
      goto LABEL_46;
    }
  }
LABEL_14:
  v57 = 1;
  return sub_4AB245((unsigned int)v63 ^ v62);
}

// ===== sub_456FC0 @ 0x00456FC0..0x00457059 =====
int __userpurge sub_456FC0@<eax>(
        int a1@<edi>,
        __m128 a2@<xmm0>,
        int a3,
        _DWORD *a4,
        unsigned int a5,
        int a6,
        unsigned __int8 a7)
{
  unsigned int v7; // eax
  int v8; // ecx
  float v9; // xmm3_4
  __m128 *v10; // esi
  __m128 v11; // xmm1
  __m128 v12; // xmm1
  int v14; // [esp+10h] [ebp+10h]

  v7 = 0;
  v8 = 0;
  LODWORD(v9) = _mm_shuffle_ps((__m128)a5, (__m128)a5, 0).m128_u32[0];
  if ( *(_DWORD *)(a1 + 4) )
  {
    v14 = 0;
    do
    {
      v10 = (__m128 *)(*(_DWORD *)(a1 + 8) + v14);
      if ( v10->m128_i32[0] && v7 != a6 && (a7 & v10[1].m128_i8[0]) != 0 )
      {
        v11 = _mm_sub_ps(a2, v10[3]);
        v12 = _mm_mul_ps(v11, v11);
        if ( (float)(v12.m128_f32[0]
                   + (float)(_mm_shuffle_ps(v12, v12, 253).m128_f32[0] + _mm_shuffle_ps(v12, v12, 254).m128_f32[0])) < (float)((float)(v9 + v10->m128_f32[1]) * (float)(v9 + v10->m128_f32[1])) )
          *(_DWORD *)(a3 + 4 * v8++) = v7;
      }
      v14 += 96;
      ++v7;
    }
    while ( v7 < *(_DWORD *)(a1 + 4) );
    *a4 = v8;
    return 0;
  }
  else
  {
    *a4 = 0;
    return 0;
  }
}

// ===== sub_457060 @ 0x00457060..0x00457077 =====
int __usercall sub_457060@<eax>(int a1@<edi>)
{
  unsigned int i; // esi
  int result; // eax

  for ( i = 0; i < *(_DWORD *)(a1 + 4); ++i )
    result = sub_455780(a1, i);
  return result;
}

// ===== sub_457080 @ 0x00457080..0x0045716F =====
int __userpurge sub_457080@<eax>(
        int a1@<eax>,
        int a2@<ebp>,
        __m128 a3@<xmm0>,
        __m128 a4@<xmm1>,
        int a5,
        int a6,
        __int32 a7,
        float a8)
{
  int v9; // eax
  __m128 **i; // edi
  __m128 *v11; // esi
  __m128 v14; // [esp-10h] [ebp-1Ch] BYREF
  int v15; // [esp+0h] [ebp-Ch]
  void *v16; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v15 = a2;
  v16 = retaddr;
  if ( a5 < 0 )
    return 0;
  if ( a5 >= *(_DWORD *)(a1 + 12) )
    return 0;
  if ( a6 < 0 )
    return 0;
  if ( a6 >= *(_DWORD *)(a1 + 16) )
    return 0;
  v14 = _mm_sub_ps(a4, *(__m128 *)*(_DWORD *)(a1 + 296));
  if ( sub_455430(&v14) > *(float *)(a1 + 292) )
    return 0;
  v9 = *(_DWORD *)(a1 + 320);
  for ( i = (__m128 **)(a1 + 320); v9; v9 = *(_DWORD *)(v9 + 48) )
    i = (__m128 **)(v9 + 48);
  v11 = (__m128 *)_aligned_malloc(0x50u, 0x10u);
  memset(v11, 0, 0x50u);
  v11[1].m128_f32[1] = a8;
  v11[2] = a4;
  v11[1].m128_i32[0] = a7;
  v11->m128_i32[0] = a5;
  v11->m128_i32[1] = a6;
  v11[3].m128_i32[0] = 0;
  v11[4] = a3;
  *i = v11;
  return 1;
}

// ===== sub_457170 @ 0x00457170..0x004571B8 =====
int __userpurge sub_457170@<eax>(void *a1@<eax>, int a2)
{
  void *v2; // edx
  int result; // eax

  v2 = *(void **)(a2 + 320);
  result = 0;
  if ( v2 )
  {
    if ( a1 )
      qmemcpy(a1, v2, 0x50u);
    *(_DWORD *)(a2 + 320) = *(_DWORD *)(*(_DWORD *)(a2 + 320) + 48);
    _aligned_free(v2);
    return 1;
  }
  return result;
}

// ===== sub_4571C0 @ 0x004571C0..0x004573F4 =====
unsigned int __userpurge sub_4571C0@<eax>(int a1@<ebp>, int a2, __m128 *a3, int a4)
{
  float *v4; // edx
  unsigned int result; // eax
  _DWORD *v6; // ecx
  int v7; // edx
  char v8; // cl
  unsigned int v9; // edi
  int v10; // edx
  int v11; // eax
  int v12; // ecx
  __m128 v13; // xmm0
  float v14; // xmm1_4
  int v15; // [esp+4Ch] [ebp-24h]
  unsigned int v16; // [esp+50h] [ebp-20h]
  __int32 v17; // [esp+58h] [ebp-18h]
  unsigned int v18; // [esp+58h] [ebp-18h]
  __int32 v19; // [esp+5Ch] [ebp-14h]
  char v20; // [esp+5Ch] [ebp-14h]
  _DWORD v21[3]; // [esp+64h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+70h] [ebp+0h]

  v21[0] = a1;
  v21[1] = retaddr;
  if ( sub_4569F0(
         (int)v21,
         a3[4],
         a3[2],
         a2,
         COERCE_UNSIGNED_INT(*(float *)(a2 + 280)),
         *(_DWORD *)(a2 + 284),
         *(_DWORD *)(a2 + 288),
         *(_DWORD *)(a2 + 300),
         *(_DWORD *)(a2 + 304)) )
  {
    return -1;
  }
  if ( a4 )
    v15 = sub_4465D0(a4);
  v4 = (float *)(*(_DWORD *)(a2 + 20) + 32 * (a3->m128_i32[0] + a3->m128_i32[1] * *(_DWORD *)(a2 + 12)));
  v19 = a3->m128_i32[1];
  v17 = a3->m128_i32[0];
  if ( 0.0 != v4[1] && v4[1] <= (double)a3[1].m128_f32[1] )
    goto LABEL_21;
  qmemcpy(v4, &a3[1], 0x20u);
  if ( a4 )
  {
LABEL_11:
    v7 = *(_DWORD *)v4;
    v16 = v7 != -1 ? 9 : 16;
    if ( v7 == -1 )
    {
      v20 = 0;
      v8 = 0;
    }
    else
    {
      v8 = (v7 - 4) & 0xF;
      v20 = v8;
    }
    v9 = 0;
    v18 = 0;
    if ( (v7 != -1 ? 0xFFFFFFF9 : 0) != 0xFFFFFFF0 )
    {
      while ( 1 )
      {
        v10 = *(_DWORD *)(a2 + 316);
        v11 = ((_BYTE)v9 + v8) & 0xF;
        v12 = 16 * v11;
        v13 = _mm_mul_ps(*(__m128 *)(v10 + v12), *(__m128 *)(v10 + v12));
        v13.m128_f32[0] = v13.m128_f32[0]
                        + (float)(_mm_shuffle_ps(v13, v13, 253).m128_f32[0] + _mm_shuffle_ps(v13, v13, 254).m128_f32[0]);
        v14 = 1.0 / fsqrt(v13.m128_f32[0]);
        v18 += sub_457080(
                 a2,
                 (int)v21,
                 a3[2],
                 _mm_add_ps(a3[2], *(__m128 *)(v12 + v10)),
                 *(_DWORD *)(*(_DWORD *)(a2 + 312) + v12) + a3->m128_i32[0],
                 a3->m128_i32[1] + *(_DWORD *)(*(_DWORD *)(a2 + 312) + v12 + 4),
                 v11,
                 v13.m128_f32[0]
               * (float)((float)(v14 * 1.5)
                       + (float)((float)((float)((float)(v13.m128_f32[0] * v14) * v14) * v14) * -0.5))) != 0;
        v9 += 16 >> (*(_BYTE *)(a2 + 308) + 2);
        if ( v9 >= v16 )
          break;
        v8 = v20;
      }
      if ( v18 )
      {
        if ( !a4 )
          return -1;
        sub_4466D0(v18, a4);
      }
    }
LABEL_21:
    if ( a4 )
      sub_4465F0(a4, v15);
    return -1;
  }
  result = 0;
  v6 = (_DWORD *)(a2 + 28);
  while ( v17 != *(v6 - 1) || v19 != *v6 )
  {
    ++result;
    v6 += 4;
    if ( result >= 0x10 )
      goto LABEL_11;
  }
  return result;
}

// ===== sub_457400 @ 0x00457400..0x0045748A =====
int __userpurge sub_457400@<eax>(int a1@<edi>, unsigned int a2)
{
  int v2; // esi
  int v3; // ebx
  __m128 v5[5]; // [esp+10h] [ebp-50h] BYREF
  int savedregs; // [esp+60h] [ebp+0h] BYREF

  while ( *(_DWORD *)(a1 + 328) )
  {
    v2 = sub_4465D0(*(_DWORD *)(a1 + 324));
    v3 = sub_457170(v5, a1);
    sub_4465F0(*(_DWORD *)(a1 + 324), v2);
    if ( v3 )
    {
      sub_4571C0((int)&savedregs, a1, v5, *(_DWORD *)(a1 + 324));
    }
    else if ( !sub_446620(a2, *(_DWORD *)(a1 + 324)) )
    {
      *(_DWORD *)(a1 + 328) = 0;
    }
  }
  sub_4466D0(0, *(_DWORD *)(a1 + 324));
  return 0;
}

// ===== sub_457490 @ 0x00457490..0x004574A3 =====
int __cdecl sub_457490(int a1, unsigned int a2)
{
  return sub_457400(a1, a2);
}

// ===== sub_4574B0 @ 0x004574B0..0x004574E9 =====
_DWORD *__usercall sub_4574B0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<ecx>)
{
  unsigned int v2; // esi

  v2 = a1;
  *a2 = &DCTELogicalField::`vftable';
  a2[1] = 0;
  if ( !a1 )
    v2 = 1;
  a2[5] = 0;
  a2[6] = 0;
  a2[16] = 0;
  a2[2] = 0x10000 / v2;
  a2[17] = a2 + 7;
  return a2;
}

// ===== sub_4574F0 @ 0x004574F0..0x00457511 =====
void *__thiscall sub_4574F0(void *this, char a2)
{
  sub_457520();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_457520 @ 0x00457520..0x00457539 =====
int __thiscall sub_457520(_DWORD *this)
{
  *this = &DCTELogicalField::`vftable';
  sub_457AA0();
  return sub_457C60();
}

// ===== sub_457540 @ 0x00457540..0x004575B6 =====
int __userpurge sub_457540@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3, void *Src)
{
  int v7; // edi
  void *v8; // eax

  if ( a2[1] )
    return -3;
  if ( !a1 || !a3 )
    return -2147483647;
  sub_457AA0();
  a2[3] = a1;
  v7 = a3 * a1;
  a2[4] = a3;
  v8 = operator new(16 * v7);
  a2[5] = v8;
  memcpy_0(v8, Src, 16 * v7);
  return 0;
}

// ===== sub_4575C0 @ 0x004575C0..0x00457690 =====
int __userpurge sub_4575C0@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  int v10; // eax
  _DWORD *i; // esi
  int v13; // [esp+Ch] [ebp-8h]
  int v14; // [esp+10h] [ebp-4h]
  int v15; // [esp+1Ch] [ebp+8h]

  if ( !sub_457AD0() )
    return -2147483646;
  v13 = *(_DWORD *)a1 & 1;
  v14 = *(_DWORD *)a1 & 4;
  if ( (*(_DWORD *)a1 & 2) != 0 )
    v15 = 0;
  else
    v15 = (unsigned __int8)~*(_BYTE *)(a1 + 4);
  sub_457C60();
  v10 = a6;
  if ( a6 < 1 )
    v10 = 0x7FFFFFFF;
  sub_457BA0(a4, 1, 0, 0, 0, 0, 0, v10);
  for ( i = *(_DWORD **)(a2 + 64); i; i = (_DWORD *)i[9] )
  {
    if ( sub_457C90(a2, a5, v13, v14, v15) && a7 == *i && a8 == i[1] )
      break;
  }
  sub_457C60();
  return 0;
}

// ===== sub_457690 @ 0x00457690..0x00457781 =====
int __userpurge sub_457690@<eax>(int a1@<eax>, int a2@<edx>, _DWORD *a3@<esi>, int a4, int *a5)
{
  int v5; // ecx
  int v7; // eax
  int v8; // eax
  bool v9; // zf
  int v10; // eax
  int v11; // ebx
  int result; // eax
  int v13; // edi
  int v14; // [esp+4h] [ebp-4h] BYREF

  v5 = a3[6];
  if ( !v5 )
    return -2147483645;
  if ( a2 < 0 )
    return -2147483643;
  v7 = a3[3];
  if ( a2 >= v7 || a1 < 0 || a1 >= a3[4] )
    return -2147483643;
  v8 = 3 * (a2 + a1 * v7);
  v9 = *(_DWORD *)(v5 + 8 * v8 + 4) == 0;
  v10 = v5 + 8 * v8;
  if ( v9 )
    return -2147483644;
  v11 = *(_DWORD *)(v10 + 8);
  result = 0;
  *a5 = v11;
  if ( v11 >= 1 && a4 )
  {
    v14 = a2;
    a5 = (int *)a1;
    while ( 1 )
    {
      v13 = a3[6] + 24 * (v14 + (_DWORD)a5 * a3[3]);
      --v11;
      *(_DWORD *)(a4 + 4 * v11) = (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD))(*a3 + 16))(
                                    a3,
                                    *(_DWORD *)(v13 + 4),
                                    0);
      if ( !(*(int (__thiscall **)(_DWORD *, int *, int **, _DWORD))(*a3 + 12))(a3, &v14, &a5, *(_DWORD *)(v13 + 4)) )
        break;
      if ( !v11 )
        return 0;
    }
    return -2;
  }
  return result;
}

// ===== sub_457790 @ 0x00457790..0x004577ED =====
int __userpurge sub_457790@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<edi>, int a4, int a5, int a6)
{
  int v6; // ebx
  int result; // eax
  int v8; // esi
  int v9; // edx
  int v10; // [esp+8h] [ebp-4h] BYREF

  v6 = a5;
  result = 0;
  v8 = 0;
  a5 = a2;
  v10 = a1;
  if ( a3 > 0 )
  {
    while ( (*(int (__thiscall **)(int, int *, int *, _DWORD))(*(_DWORD *)a4 + 12))(
              a4,
              &a5,
              &v10,
              *(_DWORD *)(a6 + 4 * v8)) )
    {
      v9 = v10;
      *(_DWORD *)(v6 + 8 * v8) = a5;
      *(_DWORD *)(v6 + 8 * v8++ + 4) = v9;
      if ( v8 >= a3 )
        return 0;
    }
    return -2147483641;
  }
  return result;
}

// ===== sub_4577F0 @ 0x004577F0..0x00457853 =====
int __userpurge sub_4577F0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, int a3@<esi>, _DWORD *a4)
{
  int v4; // edx
  int v5; // eax
  int v6; // eax
  bool v7; // zf
  int v8; // eax

  v4 = a1[6];
  if ( !v4 )
    return -2147483645;
  if ( a3 < 0 )
    return -2147483643;
  v5 = a1[3];
  if ( a3 >= v5 || a2 < 0 || a2 >= a1[4] )
    return -2147483643;
  v6 = 3 * (a3 + a2 * v5);
  v7 = *(_DWORD *)(v4 + 8 * v6 + 4) == 0;
  v8 = v4 + 8 * v6;
  if ( v7 )
    return -2147483644;
  *a4 = (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD))(*a1 + 16))(a1, *(_DWORD *)(v8 + 4), 0);
  return 0;
}

// ===== sub_457860 @ 0x00457860..0x004578B9 =====
int __userpurge sub_457860@<eax>(int a1@<edx>, int a2@<edi>, _DWORD *a3@<esi>, _DWORD *a4)
{
  int v4; // ecx
  int v5; // eax
  int v6; // eax
  bool v7; // zf
  int v8; // eax

  v4 = a3[6];
  if ( !v4 )
    return -2147483645;
  if ( a1 < 0 )
    return -2147483643;
  v5 = a3[3];
  if ( a1 >= v5 || a2 < 0 || a2 >= a3[4] )
    return -2147483643;
  v6 = 3 * (a1 + a2 * v5);
  v7 = *(_DWORD *)(v4 + 8 * v6 + 4) == 0;
  v8 = v4 + 8 * v6;
  if ( v7 )
    return -2147483644;
  *a4 = *(_DWORD *)(v8 + 12);
  return 0;
}

// ===== sub_4578C0 @ 0x004578C0..0x004578F7 =====
int __thiscall sub_4578C0(_DWORD *this, void *a2)
{
  const void *v2; // edx

  v2 = (const void *)this[6];
  if ( !v2 )
    return -2147483645;
  memcpy_0(a2, v2, 24 * this[3] * this[4]);
  return 0;
}

// ===== sub_457900 @ 0x00457900..0x00457A91 =====
int __userpurge sub_457900@<eax>(_DWORD *a1@<eax>, _DWORD *a2, size_t *a3, int a4)
{
  int result; // eax
  int v6; // ebx
  size_t v7; // edi
  int v8; // ecx
  int v9; // edx
  int *v10; // eax
  int v11; // ecx
  int *v12; // ecx
  size_t v13; // edi
  size_t v14; // eax
  _DWORD *v15; // ecx
  _DWORD *v16; // edx
  char *v17; // [esp+4h] [ebp-10h]
  _DWORD *Base; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]
  size_t v20; // [esp+10h] [ebp-4h]

  result = -1;
  if ( a1[6] )
  {
    v17 = (char *)operator new(32 * a1[4] * a1[3]);
    v6 = 0;
    v7 = 0;
    Base = operator new(4 * a1[4] * a1[3]);
    v20 = 0;
    for ( i = 0; v6 < a1[4]; i = v6 )
    {
      v8 = a1[3];
      v9 = 0;
      if ( v8 > 0 )
      {
        v10 = (int *)&v17[32 * v7];
        do
        {
          v11 = v9 + v6 * v8;
          if ( *(int *)(a1[6] + 24 * v11 + 12) <= 0 || (*(_BYTE *)(a1[5] + 16 * v11 + 8) & 0xD) != 0 )
          {
            v6 = i;
            v7 = v20;
          }
          else
          {
            v6 = i;
            if ( a4 )
            {
              *v10 = v9;
              v10[1] = i;
              v12 = (int *)(a1[6] + 24 * (v9 + i * a1[3]));
              v10[2] = *v12;
              v10[3] = v12[1];
              v10[4] = v12[2];
              v10[5] = v12[3];
              v10[6] = v12[4];
              v13 = v20;
              v10[7] = v12[5];
              Base[v20] = v10;
            }
            else
            {
              v13 = v20;
              a2[2 * v20] = v9;
              a2[2 * v20 + 1] = i;
            }
            v7 = v13 + 1;
            v20 = v7;
            v10 += 8;
          }
          v8 = a1[3];
          ++v9;
        }
        while ( v9 < v8 );
      }
      ++v6;
    }
    if ( a4 && v7 )
    {
      qsort(Base, v7, 4u, sub_457E40);
      v14 = 0;
      v15 = a2;
      do
      {
        v16 = (_DWORD *)Base[v14];
        *v15 = *v16;
        v15[1] = v16[1];
        ++v14;
        v15 += 2;
      }
      while ( v14 < v7 );
    }
    *a3 = v7;
    operator delete(v17);
    operator delete(Base);
    return 0;
  }
  return result;
}

// ===== sub_457AA0 @ 0x00457AA0..0x00457ACD =====
void __usercall sub_457AA0(int a1@<esi>)
{
  if ( !*(_DWORD *)(a1 + 4) )
  {
    operator delete(*(void **)(a1 + 20));
    *(_DWORD *)(a1 + 20) = 0;
  }
  operator delete(*(void **)(a1 + 24));
  *(_DWORD *)(a1 + 24) = 0;
}

// ===== sub_457AD0 @ 0x00457AD0..0x00457B24 =====
int __usercall sub_457AD0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = 0;
  if ( *(_DWORD *)(a1 + 20) )
  {
    if ( !*(_DWORD *)(a1 + 24) )
      *(_DWORD *)(a1 + 24) = operator new(24 * *(_DWORD *)(a1 + 12) * *(_DWORD *)(a1 + 16));
    memset(*(void **)(a1 + 24), 0, 24 * *(_DWORD *)(a1 + 12) * *(_DWORD *)(a1 + 16));
    return 1;
  }
  return result;
}

// ===== sub_457B30 @ 0x00457B30..0x00457B9A =====
int __userpurge sub_457B30@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, int a3@<esi>, _DWORD *a4)
{
  int v4; // edx
  int v5; // eax
  _DWORD *v6; // eax
  int v7; // edx
  int v8; // eax

  v4 = a4[6];
  if ( !v4 )
    return -2147483645;
  if ( a3 < 0 )
    return -2147483643;
  v5 = a4[3];
  if ( a3 >= v5 || a2 < 0 || a2 >= a4[4] )
    return -2147483643;
  v6 = (_DWORD *)(v4 + 24 * (a3 + a2 * v5));
  *a1 = *v6;
  a1[1] = v6[1];
  a1[2] = v6[2];
  a1[3] = v6[3];
  v7 = v6[4];
  v8 = v6[5];
  a1[4] = v7;
  a1[5] = v8;
  return 0;
}

// ===== sub_457BA0 @ 0x00457BA0..0x00457C5D =====
int __userpurge sub_457BA0@<eax>(
        int a1@<edi>,
        _DWORD *a2@<esi>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  int v10; // ecx
  int v11; // eax
  int v12; // ecx
  bool v13; // zf
  int v14; // ecx
  int v15; // edx
  int *v16; // eax

  if ( a1 < 0 )
    return -2147483643;
  v10 = a2[3];
  if ( v10 <= a1 || a3 < 0 || a2[4] <= a3 )
    return -2147483643;
  v11 = 3 * (a1 + a3 * v10);
  v12 = a2[6];
  v13 = *(_DWORD *)(v12 + 8 * v11) == 0;
  v14 = v12 + 8 * v11;
  if ( !v13 )
  {
    v15 = *(_DWORD *)(v14 + 12);
    if ( a7 + 1 >= v15 && (a7 + 1 != v15 || a8 >= *(_DWORD *)(v14 + 16)) )
      return -2147483642;
  }
  v16 = (int *)operator new(0x28u);
  v16[2] = a4;
  v16[3] = a5;
  v16[4] = a6;
  v16[5] = a7;
  v16[6] = a8;
  v16[1] = a3;
  *v16 = a1;
  v16[7] = a9;
  v16[8] = a10;
  v16[9] = 0;
  *(_DWORD *)(a2[17] + 36) = v16;
  a2[17] = v16;
  return 0;
}

// ===== sub_457C60 @ 0x00457C60..0x00457C8F =====
int __usercall sub_457C60@<eax>(int a1@<edi>)
{
  _DWORD *v1; // esi
  int result; // eax
  void *v3; // [esp-4h] [ebp-8h]

  v1 = *(_DWORD **)(a1 + 64);
  while ( v1 )
  {
    v3 = v1;
    v1 = (_DWORD *)v1[9];
    operator delete(v3);
  }
  result = a1 + 28;
  *(_DWORD *)(a1 + 64) = 0;
  *(_DWORD *)(a1 + 68) = a1 + 28;
  return result;
}

// ===== sub_457C90 @ 0x00457C90..0x00457E3C =====
int __userpurge sub_457C90@<eax>(_DWORD *a1@<esi>, _DWORD *a2, int a3, int a4, int a5, int a6)
{
  int v6; // eax
  int v7; // edi
  bool v8; // zf
  _DWORD *v9; // edi
  int v10; // ebx
  int v11; // ecx
  int v12; // ecx
  int v13; // ebx
  _DWORD *v14; // ecx
  int v15; // eax
  int v16; // eax
  int v17; // edx
  int v18; // eax
  int v19; // ebx
  int v20; // edx
  int v21; // ebx
  int v22; // eax
  int v23; // eax
  int v24; // eax
  BOOL v26; // [esp+10h] [ebp-8h]
  int v27; // [esp+14h] [ebp-4h]
  int v28; // [esp+24h] [ebp+Ch]

  v6 = *a1 + a1[1] * a2[3];
  v7 = a2[6];
  v8 = *(_DWORD *)(v7 + 24 * v6) == 0;
  v9 = (_DWORD *)(v7 + 24 * v6);
  if ( !v8 )
  {
    v10 = v9[3];
    v11 = a1[5] + 1;
    if ( v11 >= v10 )
    {
      if ( v11 != v10 )
        return 0;
      v12 = a1[6];
      v13 = v9[4];
      if ( v12 >= v13 && (v12 != v13 || a1[7] >= v9[5]) )
        return 0;
    }
  }
  v14 = (_DWORD *)(a2[5] + 16 * v6);
  v15 = v14[2];
  v26 = a1[2] == 1;
  if ( (v15 & 2) != 0 || !a4 && ((v15 & 0xC) != 0 || a1[2] != 1 && (v15 & 1) != 0) )
  {
    *v9 = 1;
    return 0;
  }
  v16 = *v14 + (v14[3] >> 28);
  v17 = v16;
  if ( a1[2] != 1 )
    v17 = a1[3];
  if ( a4 && v17 >= v16 )
  {
    v16 = v17;
    v27 = v17;
  }
  else
  {
    v27 = *v14 + (v14[3] >> 28);
  }
  v18 = abs32(v16 - v17);
  if ( a3 < v18 && a1[2] != 1 )
    return 0;
  v19 = 0;
  v28 = 0;
  if ( a1[2] != 1 )
  {
    v19 = v14[1] + 1;
    v28 = v19;
    if ( !a5 )
    {
      if ( a4 )
      {
        if ( v18 < 4 )
          goto LABEL_28;
        v19 = v19 + v18 - 3;
      }
      else
      {
        if ( v18 < 2 )
          goto LABEL_28;
        v19 = v14[1] + v18;
      }
      v28 = v19;
    }
  }
LABEL_28:
  v20 = v19 + a1[5];
  v21 = v18 + a1[6];
  if ( *v9 )
  {
    v22 = v9[3];
    if ( v20 >= v22 )
    {
      if ( v20 != v22 )
        return 0;
      v23 = v9[4];
      if ( v21 >= v23 && (v21 != v23 || a1[7] >= v9[5]) )
        return 0;
    }
  }
  v24 = a1[8] - v28;
  if ( v24 < 0 )
    return 0;
  *v9 = 1;
  v9[1] = a1[2];
  v9[2] = a1[4];
  v9[3] = v20;
  v9[4] = v21;
  v9[5] = a1[7];
  if ( v24 >= 1 && (a6 & v14[3]) == 0 || v26 )
    (*(void (__thiscall **)(_DWORD *, _DWORD, _DWORD, int, _DWORD, int, int, _DWORD, int))(*a2 + 8))(
      a2,
      *a1,
      a1[1],
      v27,
      a1[4],
      v20,
      v21,
      a1[7],
      v24);
  return 1;
}

// ===== sub_457E40 @ 0x00457E40..0x00457E57 =====
int __cdecl sub_457E40(const void *a1, const void *a2)
{
  return *(_DWORD *)(*(_DWORD *)a1 + 20) - *(_DWORD *)(*(_DWORD *)a2 + 20);
}

// ===== sub_457E60 @ 0x00457E60..0x00457F40 =====
int __usercall sub_457E60@<eax>(unsigned int a1@<edi>, _DWORD *a2@<esi>)
{
  void *v2; // eax
  int v3; // eax
  int result; // eax

  *a2 = &DCTESLGEvaluator::`vftable';
  a2[2] = a1;
  v2 = operator new(0x40u);
  if ( v2 )
    v3 = sub_4461E0((int)v2, a1);
  else
    v3 = 0;
  a2[3] = v3;
  sub_4464D0(v3, (int)sub_45B5A0, (int)a2);
  a2[5] = operator new(4 * a2[2]);
  a2[4] = 0;
  a2[9] = 0;
  a2[10] = 0;
  sub_457FD0(0, 0, 0);
  sub_459220(a2);
  result = sub_4592A0();
  a2[85] = 50;
  a2[86] = 67;
  return result;
}

// ===== sub_457F40 @ 0x00457F40..0x00457F61 =====
void *__thiscall sub_457F40(void *this, char a2)
{
  sub_457F70();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_457F70 @ 0x00457F70..0x00457FAD =====
void __thiscall sub_457F70(void *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx

  *(_DWORD *)this = &DCTESLGEvaluator::`vftable';
  sub_457FD0(0, 0, 0);
  sub_4464D0(*((_DWORD *)this + 3), 0, 0);
  v2 = (void (__thiscall ***)(_DWORD, int))*((_DWORD *)this + 3);
  if ( v2 )
    (**v2)(v2, 1);
  operator delete(*((void **)this + 5));
}

// ===== sub_457FB0 @ 0x00457FB0..0x00457FC9 =====
int __fastcall sub_457FB0(int a1, int a2, int a3)
{
  int result; // eax

  result = -1879048190;
  if ( a1 >= 1 )
  {
    *(_DWORD *)(a3 + 32) = a1;
    return 0;
  }
  return result;
}

// ===== sub_457FD0 @ 0x00457FD0..0x00458125 =====
int __thiscall sub_457FD0(_DWORD *this, int a2, unsigned int a3, int a4)
{
  void *v5; // eax
  unsigned int i; // esi
  int result; // eax
  void (__thiscall ***v8)(_DWORD, int); // ecx
  int v9; // ebx
  unsigned int k; // esi
  void *v11; // [esp-4h] [ebp-10h]
  unsigned int j; // [esp+14h] [ebp+8h]

  if ( a2 )
  {
    if ( a3 - 1 > 0x3F )
    {
      return -1879048190;
    }
    else
    {
      sub_457FD0(0, 0, 0);
      sub_458130();
      this[9] = a3;
      v5 = operator new(2268 * a3);
      this[10] = v5;
      memset(v5, 0, 2268 * a3);
      for ( i = 0; i < a3; ++i )
      {
        sub_458160(this, a4, 1);
        a4 += 2100;
      }
      return 0;
    }
  }
  else
  {
    v8 = (void (__thiscall ***)(_DWORD, int))this[4];
    if ( v8 )
      (**v8)(v8, 1);
    v9 = 0;
    this[4] = 0;
    for ( j = 0; j < this[9]; ++j )
    {
      operator delete(*(void **)(v9 + this[10] + 2132));
      operator delete(*(void **)(v9 + this[10] + 2136));
      for ( k = 0; k < 0x80; k += 8 )
      {
        operator delete(*(void **)(v9 + k + this[10] + 2140));
        operator delete(*(void **)(v9 + k + this[10] + 2144));
      }
      v9 += 2268;
    }
    v11 = (void *)this[10];
    this[9] = 0;
    operator delete(v11);
    result = 0;
    this[10] = 0;
  }
  return result;
}
