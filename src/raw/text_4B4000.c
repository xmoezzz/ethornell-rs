#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== __fFEXP @ 0x004B45C1..0x004B45F6 =====
double __usercall _fFEXP@<st0>(int a1@<ebp>, double a2@<st0>)
{
  long double v2; // st7
  char v3; // dl
  char v4; // ch
  long double v5; // st6

  *(_BYTE *)(a1 - 144) = -2;
  v2 = a2 * 1.442695040888963407;
  _ffexpm1();
  v5 = 1.442695040888963407 + 1.0;
  if ( (*(_BYTE *)(a1 - 159) & 1) != 0 )
    v5 = 1.0 / v5;
  if ( (v3 & 0x40) == 0 )
    v5 = __FSCALE__(v5, v2);
  if ( v4 )
    return -v5;
  return v5;
}

// ===== zerotoxdone @ 0x004B4687..0x004B4688 =====
void zerotoxdone()
{
  ;
}

// ===== _expbigret @ 0x004B46D2..0x004B46D3 =====
void expbigret()
{
  ;
}

// ===== _rtforexpinf @ 0x004B46F1..0x004B46FE =====
// positive sp value has been detected, the output may be wrong!
double __fastcall rtforexpinf(char a1)
{
  if ( a1 )
    return 0.0;
  else
    return -0.0;
}

// ===== __ffexpm1 @ 0x004B46FE..0x004B4741 =====
// positive sp value has been detected, the output may be wrong!
void __usercall _ffexpm1(char a1@<ch>, int _EBP@<ebp>)
{
  __asm
  {
    fld     st
    fabs
    fld     ds:tbyte_4E0D8E
    fcompp
    fstsw   word ptr [ebp-0A0h]
  }
  if ( (*(_BYTE *)(_EBP - 159) & 0x41) != 0 )
  {
    __asm
    {
      ftst
      fstsw   word ptr [ebp-0A0h]
    }
    if ( (*(_BYTE *)(_EBP - 159) & 1) != 0 )
    {
      *(_BYTE *)(_EBP - 144) = 4;
      __asm
      {
        fstp    st
        fldz
      }
    }
    else
    {
      __asm
      {
        fstp    st
        fld     ds:tbyte_4E0D70
      }
      if ( a1 )
        __asm { fchs }
      expbigret();
    }
  }
  else
  {
    __asm
    {
      fld     st
      frndint
      ftst
      fstsw   word ptr [ebp-0A0h]
      fxch    st(1)
      fsub    st, st(1)
      ftst
      fstsw   word ptr [ebp-0A0h]
      fabs
      f2xm1
    }
  }
}

// ===== _isintTOS @ 0x004B4741..0x004B4766 =====
int __usercall isintTOS@<eax>(double a1@<st0>)
{
  _ST6 = a1;
  __asm { frndint }
  if ( _ST6 != a1 )
    return notanint();
  _ST5 = a1 * 0.5;
  __asm { frndint }
  if ( _ST5 == a1 * 0.5 )
    return evenint();
  else
    return isintTOSret();
}

// ===== _isintTOSret @ 0x004B4766..0x004B4767 =====
void isintTOSret()
{
  ;
}

// ===== notanint @ 0x004B4767..0x004B476E =====
void notanint()
{
  isintTOSret();
}

// ===== evenint @ 0x004B476E..0x004B4775 =====
void evenint()
{
  isintTOSret();
}

// ===== _usepowhlp @ 0x004B4775..0x004B47A7 =====
double __usercall usepowhlp@<st0>(double a1@<st1>, double a2@<st0>)
{
  int v3; // eax
  double result; // st7
  double *v5; // [esp+10h] [ebp-7Ch]
  double v6[15]; // [esp+14h] [ebp-78h] BYREF

  _ESI = v6;
  v5 = v6;
  __asm { fsave   byte ptr [esi+8] }
  v3 = _powhlp(a2, a1, (int)v6);
  __asm { frstor  byte ptr [esi+8] }
  result = v6[0];
  if ( v3 )
    _rttosnpopde();
  return result;
}

// ===== __trandisp1 @ 0x004B47B0..0x004B4817 =====
int __usercall _trandisp1@<eax>(int a1@<edx>, __int16 a2@<cx>, int a3@<ebp>, __int16 a4@<fpstat>, double _ST7@<st0>)
{
  __int16 v5; // bx

  if ( *(_BYTE *)(a1 + 14) == 5 )
  {
    HIBYTE(v5) = HIBYTE(*(_WORD *)(a3 - 164)) & 0xFC | 2;
    LOBYTE(v5) = 63;
  }
  else
  {
    v5 = 4927;
  }
  *(_WORD *)(a3 - 162) = v5;
  _EBX = &unk_4E0DEC;
  __asm { fxam }
  *(_DWORD *)(a3 - 148) = a1;
  *(_WORD *)(a3 - 160) = a4;
  *(_BYTE *)(a3 - 144) = 0;
  LOBYTE(a2) = __ROL1__((char)(2 * *(_BYTE *)(a3 - 159)) >> 1, 1);
  _AL = a2 & 0xF;
  __asm { xlat }
  return (*(int (__thiscall **)(int))(_AL + a1 + 16))(a2 & 0x404);
}

// ===== __trandisp2 @ 0x004B4817..0x004B48A3 =====
int __usercall _trandisp2@<eax>(int a1@<edx>, int a2@<ebp>, __int16 a3@<fpstat>, double _ST6@<st1>, double a5@<st0>)
{
  __int16 v5; // bx
  __int16 v7; // fps
  char v9; // cl
  __int16 v10; // cx
  char v13; // ah

  if ( *(_BYTE *)(a1 + 14) == 5 )
  {
    HIBYTE(v5) = HIBYTE(*(_WORD *)(a2 - 164)) & 0xFC | 2;
    LOBYTE(v5) = 63;
  }
  else
  {
    v5 = 4927;
  }
  *(_WORD *)(a2 - 162) = v5;
  _EBX = &unk_4E0DEC;
  __asm { fxam }
  *(_DWORD *)(a2 - 148) = a1;
  *(_WORD *)(a2 - 160) = a3;
  *(_BYTE *)(a2 - 144) = 0;
  _ST6 = a5;
  v9 = *(_BYTE *)(a2 - 159);
  __asm { fxam }
  *(_WORD *)(a2 - 160) = v7;
  HIBYTE(v10) = __ROL1__((char)(2 * *(_BYTE *)(a2 - 159)) >> 1, 1);
  _AL = HIBYTE(v10) & 0xF;
  __asm { xlat }
  v13 = _AL;
  LOBYTE(v10) = __ROL1__((char)(2 * v9) >> 1, 1);
  _AL = v10 & 0xF;
  __asm { xlat }
  return (*(int (__thiscall **)(int))((char)((4 * v13) | _AL) + a1 + 16))(v10 & 0x404);
}

// ===== __rtonenpop @ 0x004B48BD..0x004B48C2 =====
double _rtonenpop()
{
  return 1.0;
}

// ===== __tosnan1 @ 0x004B48C2..0x004B48ED =====
double __usercall _tosnan1@<st0>(int a1@<ebp>, double a2@<st0>)
{
  double result; // st7

  *(double *)(a1 - 158) = a2;
  result = *(double *)(a1 - 158);
  if ( (*(_BYTE *)(a1 - 151) & 0x40) != 0 )
  {
    *(_BYTE *)(a1 - 144) = 7;
  }
  else
  {
    *(_BYTE *)(a1 - 144) = 1;
    return result + 1.0;
  }
  return result;
}

// ===== __nosnan2 @ 0x004B48ED..0x004B48EF =====
int _nosnan2()
{
  return _tosnan2();
}

// ===== __tosnan2 @ 0x004B48EF..0x004B4917 =====
double __usercall _tosnan2@<st0>(int a1@<ebp>, double a2@<st1>, double a3@<st0>)
{
  double v3; // st6

  *(double *)(a1 - 158) = a2;
  v3 = *(double *)(a1 - 158);
  if ( (*(_BYTE *)(a1 - 151) & 0x40) != 0 )
    *(_BYTE *)(a1 - 144) = 7;
  else
    *(_BYTE *)(a1 - 144) = 1;
  return a3 + v3;
}

// ===== __nan2 @ 0x004B4917..0x004B4956 =====
double __usercall _nan2@<st0>(int a1@<ebp>, double a2@<st1>, double a3@<st0>)
{
  double v3; // st6
  double v4; // rt0
  double v5; // st6

  *(double *)(a1 - 158) = a2;
  v3 = *(double *)(a1 - 158);
  if ( (*(_BYTE *)(a1 - 151) & 0x40) != 0
    && (v4 = v3,
        v5 = a3,
        a3 = v4,
        *(double *)(a1 - 158) = v5,
        v3 = *(double *)(a1 - 158),
        (*(_BYTE *)(a1 - 151) & 0x40) != 0) )
  {
    *(_BYTE *)(a1 - 144) = 7;
  }
  else
  {
    *(_BYTE *)(a1 - 144) = 1;
  }
  return a3 + v3;
}

// ===== __rtindfpop @ 0x004B4956..0x004B4969 =====
int __usercall _rtindfpop@<eax>(int a1@<ebp>)
{
  if ( *(char *)(a1 - 144) > 0 )
    JUMPOUT(0x4B4970);
  return _rttosnpopde();
}

// ===== __rttosnpopde @ 0x004B4969..0x004B4973 =====
void __usercall _rttosnpopde(int a1@<ebp>)
{
  *(_BYTE *)(a1 - 144) = 1;
}

// ===== chsifnegret @ 0x004B4979..0x004B497A =====
void chsifnegret()
{
  ;
}

// ===== __startTwoArgErrorHandling @ 0x004B4980..0x004B4997 =====
void _startTwoArgErrorHandling()
{
  JUMPOUT(0x4B49A0);
}

// ===== __startOneArgErrorHandling @ 0x004B4997..0x004B49D3 =====
double __usercall _startOneArgErrorHandling@<st0>(
        int a1@<eax>,
        int a2@<edx>,
        int a3@<ecx>,
        double a4@<st0>,
        __int16 a5,
        int a6,
        int a7,
        int a8)
{
  _DWORD v9[6]; // [esp+0h] [ebp-20h] BYREF
  double v10; // [esp+18h] [ebp-8h]

  v9[0] = a1;
  v10 = a4;
  v9[1] = a3;
  v9[2] = a7;
  v9[3] = a8;
  _87except(a2, v9, &a5);
  return v10;
}

// ===== __twoToTOS @ 0x004B49E0..0x004B49F5 =====
double __usercall _twoToTOS@<st0>(double a1@<st0>)
{
  _ST6 = a1;
  __asm { frndint }
  return __FSCALE__(__F2XM1__(-(_ST6 - a1)) + 1.0, _ST6);
}

// ===== __load_CW @ 0x004B49F5..0x004B4A0C =====
void _load_CW()
{
  ;
}

// ===== __convertTOStoQNaN @ 0x004B4A0C..0x004B4A25 =====
double __usercall _convertTOStoQNaN@<st0>(int a1@<eax>, double result@<st0>)
{
  if ( (a1 & 0x80000) == 0 )
    return result + 1.0;
  return result;
}

// ===== __fload_withFB @ 0x004B4A25..0x004B4A68 =====
double __fastcall _fload_withFB(int a1, _DWORD *a2)
{
  double result; // st7

  if ( (a2[1] & 0x7FF00000) != 0x7FF00000 )
    return *(double *)a2;
  *(_QWORD *)&result = *(_QWORD *)a2 << 11;
  return result;
}

// ===== __checkTOS_withFB @ 0x004B4A68..0x004B4A7E =====
int __cdecl _checkTOS_withFB(int a1, int a2)
{
  int result; // eax

  result = a2 & 0x7FF00000;
  if ( (a2 & 0x7FF00000) == 0x7FF00000 )
    return a2;
  return result;
}

// ===== __check_overflow_exit @ 0x004B4AB5..0x004B4AC9 =====
void _check_overflow_exit()
{
  JUMPOUT(0x4B4ADD);
}

// ===== __check_range_exit @ 0x004B4AC9..0x004B4B6C =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004B4AC9

// ===== __d_inttype @ 0x004B4B6C..0x004B4BD6 =====
int __cdecl _d_inttype(double X)
{
  if ( (_fpclass(X) & 0x90) != 0 )
    return 0;
  _frnd(X);
  _frnd(X * 0.5);
  return 2;
}

// ===== __powhlp @ 0x004B4BD6..0x004B4D01 =====
int __cdecl _powhlp(long double a1, double a2, double *a3)
{
  long double v3; // st7
  int v4; // esi
  double v5; // st7
  bool v6; // c3
  double *v7; // eax
  int v8; // eax

  v3 = fabs(a1);
  v4 = 0;
  if ( HIDWORD(a2) == 2146435072 )
  {
    if ( !LODWORD(a2) )
    {
      if ( v3 <= 1.0 )
      {
        v6 = 1.0 == v3;
        v5 = 1.0;
        v7 = a3;
        if ( !v6 )
          v5 = 0.0;
        goto LABEL_27;
      }
      goto LABEL_4;
    }
  }
  else if ( a2 == -INFINITY )
  {
    if ( v3 <= 1.0 )
    {
      v7 = a3;
      if ( v3 >= 1.0 )
      {
        v5 = dbl_4FC208;
        v4 = 1;
      }
      else
      {
        v5 = dbl_4FC200;
      }
      goto LABEL_27;
    }
    v5 = 0.0;
LABEL_26:
    v7 = a3;
LABEL_27:
    *v7 = v5;
    return v4;
  }
  if ( HIDWORD(a1) == 2146435072 )
  {
    if ( !LODWORD(a1) )
    {
      v5 = 0.0;
      if ( a2 <= 0.0 )
      {
        v7 = a3;
        if ( a2 >= 0.0 )
          v5 = 1.0;
        goto LABEL_27;
      }
LABEL_4:
      v5 = dbl_4FC200;
      goto LABEL_26;
    }
  }
  else if ( a1 == -INFINITY )
  {
    v8 = _d_inttype(a2);
    v5 = 0.0;
    if ( a2 <= 0.0 )
    {
      if ( a2 >= 0.0 )
      {
        v5 = 1.0;
      }
      else if ( v8 == 1 )
      {
        v5 = dbl_4FC220;
      }
    }
    else
    {
      v5 = dbl_4FC200;
      if ( v8 == 1 )
        v5 = -dbl_4FC200;
    }
    goto LABEL_26;
  }
  return v4;
}

// ===== sub_4B4D01 @ 0x004B4D01..0x004B4D09 =====
void sub_4B4D01()
{
  dword_567AE4 = 0;
}

// ===== sub_4B4D09 @ 0x004B4D09..0x004B4D0C =====
int sub_4B4D09()
{
  return 0;
}

// ===== __raise_exc_ex @ 0x004B4D0C..0x004B4FE8 =====
unsigned int __cdecl _raise_exc_ex(
        ULONG_PTR Arguments,
        unsigned int *a2,
        DWORD dwExceptionCode,
        int a4,
        float *a5,
        float *a6,
        int a7)
{
  char v7; // cl
  unsigned int *v8; // esi
  char v9; // al
  int v10; // eax
  unsigned int *v11; // eax
  unsigned int v12; // ecx
  int v13; // eax
  unsigned int *v14; // eax
  unsigned int v15; // ecx
  float *v16; // edi
  ULONG_PTR v17; // ecx
  int v18; // eax
  int v19; // eax
  int v20; // eax
  unsigned int v21; // eax
  int v22; // eax
  int v23; // eax
  unsigned int result; // eax

  v7 = dwExceptionCode;
  *(_DWORD *)(Arguments + 4) = 0;
  *(_DWORD *)(Arguments + 8) = 0;
  *(_DWORD *)(Arguments + 12) = 0;
  if ( (v7 & 0x10) != 0 )
  {
    *(_DWORD *)(Arguments + 4) |= 1u;
    dwExceptionCode = -1073741681;
  }
  if ( (v7 & 2) != 0 )
  {
    *(_DWORD *)(Arguments + 4) |= 2u;
    dwExceptionCode = -1073741677;
  }
  if ( (v7 & 1) != 0 )
  {
    *(_DWORD *)(Arguments + 4) |= 4u;
    dwExceptionCode = -1073741679;
  }
  if ( (v7 & 4) != 0 )
  {
    *(_DWORD *)(Arguments + 4) |= 8u;
    dwExceptionCode = -1073741682;
  }
  if ( (v7 & 8) != 0 )
  {
    *(_DWORD *)(Arguments + 4) |= 0x10u;
    dwExceptionCode = -1073741680;
  }
  v8 = a2;
  *(_DWORD *)(Arguments + 8) ^= (*(_DWORD *)(Arguments + 8) ^ ~(16 * *a2)) & 0x10;
  *(_DWORD *)(Arguments + 8) ^= (*(_DWORD *)(Arguments + 8) ^ ~(2 * *v8)) & 8;
  *(_DWORD *)(Arguments + 8) ^= (*(_DWORD *)(Arguments + 8) ^ ~(*v8 >> 1)) & 4;
  *(_DWORD *)(Arguments + 8) ^= (*(_DWORD *)(Arguments + 8) ^ ~(*v8 >> 3)) & 2;
  *(_DWORD *)(Arguments + 8) ^= (*(_DWORD *)(Arguments + 8) ^ ~(*v8 >> 5)) & 1;
  v9 = _statfp();
  if ( (v9 & 1) != 0 )
    *(_DWORD *)(Arguments + 12) |= 0x10u;
  if ( (v9 & 4) != 0 )
    *(_DWORD *)(Arguments + 12) |= 8u;
  if ( (v9 & 8) != 0 )
    *(_DWORD *)(Arguments + 12) |= 4u;
  if ( (v9 & 0x10) != 0 )
    *(_DWORD *)(Arguments + 12) |= 2u;
  if ( (v9 & 0x20) != 0 )
    *(_DWORD *)(Arguments + 12) |= 1u;
  v10 = *v8 & 0xC00;
  switch ( v10 )
  {
    case 0:
      *(_DWORD *)Arguments &= 0xFFFFFFFC;
      break;
    case 1024:
      v11 = (unsigned int *)Arguments;
      v12 = *(_DWORD *)Arguments & 0xFFFFFFFC | 1;
      goto LABEL_27;
    case 2048:
      v11 = (unsigned int *)Arguments;
      v12 = *(_DWORD *)Arguments & 0xFFFFFFFC | 2;
LABEL_27:
      *v11 = v12;
      break;
    case 3072:
      *(_DWORD *)Arguments |= 3u;
      break;
  }
  v13 = *v8 & 0x300;
  switch ( v13 )
  {
    case 0:
      v14 = (unsigned int *)Arguments;
      v15 = *(_DWORD *)Arguments & 0xFFFFFFE3 | 8;
      goto LABEL_36;
    case 512:
      v14 = (unsigned int *)Arguments;
      v15 = *(_DWORD *)Arguments & 0xFFFFFFE3 | 4;
LABEL_36:
      *v14 = v15;
      break;
    case 768:
      *(_DWORD *)Arguments &= 0xFFFFFFE3;
      break;
  }
  *(_DWORD *)Arguments ^= (*(_DWORD *)Arguments ^ (32 * a4)) & 0x1FFE0;
  *(_DWORD *)(Arguments + 32) |= 1u;
  v16 = a6;
  if ( a7 )
  {
    *(_DWORD *)(Arguments + 32) &= 0xFFFFFFE1;
    *(float *)(Arguments + 16) = *a5;
    *(_DWORD *)(Arguments + 96) |= 1u;
    *(_DWORD *)(Arguments + 96) &= 0xFFFFFFE1;
    *(float *)(Arguments + 80) = *v16;
  }
  else
  {
    *(_DWORD *)(Arguments + 32) = *(_DWORD *)(Arguments + 32) & 0xFFFFFFE1 | 2;
    *(double *)(Arguments + 16) = *(double *)a5;
    *(_DWORD *)(Arguments + 96) |= 1u;
    *(_DWORD *)(Arguments + 96) = *(_DWORD *)(Arguments + 96) & 0xFFFFFFE1 | 2;
    *(double *)(Arguments + 80) = *(double *)v16;
  }
  _clrfp();
  RaiseException(dwExceptionCode, 0, 1u, &Arguments);
  v17 = Arguments;
  if ( (*(_BYTE *)(Arguments + 8) & 0x10) != 0 )
    *v8 &= ~1u;
  if ( (*(_BYTE *)(v17 + 8) & 8) != 0 )
    *v8 &= ~4u;
  if ( (*(_BYTE *)(v17 + 8) & 4) != 0 )
    *v8 &= ~8u;
  if ( (*(_BYTE *)(v17 + 8) & 2) != 0 )
    *v8 &= ~0x10u;
  if ( (*(_BYTE *)(v17 + 8) & 1) != 0 )
    *v8 &= ~0x20u;
  v18 = *(_DWORD *)v17 & 3;
  if ( !v18 )
  {
    *v8 &= 0xFFFFF3FF;
    goto LABEL_59;
  }
  v19 = v18 - 1;
  if ( !v19 )
  {
    v21 = *v8 & 0xFFFFF3FF | 0x400;
    goto LABEL_56;
  }
  v20 = v19 - 1;
  if ( !v20 )
  {
    v21 = *v8 & 0xFFFFF3FF | 0x800;
LABEL_56:
    *v8 = v21;
    goto LABEL_59;
  }
  if ( v20 == 1 )
    *v8 |= 0xC00u;
LABEL_59:
  v22 = (*(_DWORD *)v17 >> 2) & 7;
  if ( !v22 )
  {
    result = *v8 & 0xFFFFF0FF | 0x300;
    goto LABEL_65;
  }
  v23 = v22 - 1;
  if ( !v23 )
  {
    result = *v8 & 0xFFFFF1FF | 0x200;
LABEL_65:
    *v8 = result;
    goto LABEL_66;
  }
  result = v23 - 1;
  if ( !result )
    *v8 &= 0xFFFFF3FF;
LABEL_66:
  if ( a7 )
    *v16 = *(float *)(v17 + 80);
  else
    *(double *)v16 = *(double *)(v17 + 80);
  return result;
}

// ===== __raise_exc @ 0x004B4FE8..0x004B500B =====
unsigned int __cdecl _raise_exc(
        ULONG_PTR Arguments,
        unsigned int *a2,
        DWORD dwExceptionCode,
        int a4,
        float *a5,
        float *a6)
{
  return _raise_exc_ex(Arguments, a2, dwExceptionCode, a4, a5, a6, 0);
}

// ===== __handle_exc @ 0x004B500B..0x004B51EF =====
BOOL __cdecl _handle_exc(char a1, double *a2, __int16 a3)
{
  int v3; // esi
  int v4; // eax
  double *v5; // ecx
  double v6; // st7
  BOOL v7; // esi
  double v8; // st7
  int v9; // ecx
  double v10; // st7
  int v11; // eax
  double v13; // [esp+18h] [ebp-14h]
  int v14; // [esp+24h] [ebp-8h] BYREF
  int v15; // [esp+28h] [ebp-4h]

  v3 = a1 & 0x1F;
  v15 = v3;
  if ( (a1 & 8) != 0 && (a3 & 1) != 0 )
  {
    _set_statfp(1);
    v3 = a1 & 0x17;
    goto LABEL_46;
  }
  if ( (a1 & 4) != 0 && (a3 & 4) != 0 )
  {
    _set_statfp(4);
    v3 = a1 & 0x1B;
    goto LABEL_46;
  }
  if ( (a1 & 1) != 0 && (a3 & 8) != 0 )
  {
    _set_statfp(8);
    v4 = a3 & 0xC00;
    if ( (a3 & 0xC00) != 0 )
    {
      if ( v4 != 1024 )
      {
        if ( v4 != 2048 )
        {
          if ( v4 != 3072 )
          {
LABEL_24:
            v3 = a1 & 0x1E;
            goto LABEL_46;
          }
          v5 = a2;
          v6 = dbl_4FC210;
          if ( *a2 <= 0.0 )
            goto LABEL_22;
LABEL_23:
          *v5 = v6;
          goto LABEL_24;
        }
        v5 = a2;
        if ( *a2 <= 0.0 )
        {
          v6 = dbl_4FC210;
LABEL_22:
          v6 = -v6;
          goto LABEL_23;
        }
LABEL_20:
        v6 = dbl_4FC200;
        goto LABEL_23;
      }
      v5 = a2;
      if ( *a2 > 0.0 )
      {
        v6 = dbl_4FC210;
        goto LABEL_23;
      }
    }
    else
    {
      v5 = a2;
      if ( *a2 > 0.0 )
        goto LABEL_20;
    }
    v6 = dbl_4FC200;
    goto LABEL_22;
  }
  if ( (a1 & 2) != 0 && (a3 & 0x10) != 0 )
  {
    v7 = (a1 & 0x10) != 0;
    if ( 0.0 == *a2 )
    {
      v7 = 1;
      goto LABEL_43;
    }
    v8 = *a2;
    _decomp(v8, (int)&v14);
    v13 = v8;
    v9 = v14 - 1536;
    if ( v14 - 1536 >= -1074 )
    {
      HIWORD(v13) = BYTE6(v13) & 0xF | 0x10;
      if ( v9 < -1021 )
      {
        v11 = -1021 - v9;
        do
        {
          if ( (LOBYTE(v13) & 1) != 0 && !v7 )
            v7 = 1;
          LODWORD(v13) >>= 1;
          if ( (BYTE4(v13) & 1) != 0 )
            LODWORD(v13) |= 0x80000000;
          HIDWORD(v13) >>= 1;
          --v11;
        }
        while ( v11 );
      }
      if ( v8 >= 0.0 )
        goto LABEL_41;
      v10 = -v13;
    }
    else
    {
      v7 = 1;
      v10 = v8 * 0.0;
    }
    v13 = v10;
LABEL_41:
    *a2 = v13;
LABEL_43:
    if ( v7 )
      _set_statfp(16);
    v15 &= ~2u;
    v3 = v15;
  }
LABEL_46:
  if ( (a1 & 0x10) != 0 && (a3 & 0x20) != 0 )
  {
    _set_statfp(32);
    v3 &= ~0x10u;
  }
  return v3 == 0;
}

// ===== __set_errno_from_matherr @ 0x004B51EF..0x004B521C =====
void __cdecl _set_errno_from_matherr(int a1)
{
  if ( a1 == 1 )
  {
    *_errno() = 33;
  }
  else if ( a1 > 1 && a1 <= 3 )
  {
    *_errno() = 34;
  }
}

// ===== __errcode @ 0x004B521C..0x004B5250 =====
int __cdecl _errcode(char a1)
{
  if ( (a1 & 0x20) != 0 )
    return 5;
  if ( (a1 & 8) != 0 )
    return 1;
  if ( (a1 & 4) != 0 )
    return 2;
  if ( (a1 & 1) != 0 )
    return 3;
  return 2 * (a1 & 2);
}

// ===== __umatherr @ 0x004B5250..0x004B52F0 =====
double __cdecl _umatherr(int a1, int a2, int a3, int a4, int a5, int a6, double a7, int a8)
{
  int v8; // eax
  char *v9; // eax
  _DWORD v11[6]; // [esp+0h] [ebp-20h] BYREF
  double v12; // [esp+18h] [ebp-8h]

  v8 = 0;
  while ( dword_4FC118[2 * v8] != a2 )
  {
    if ( ++v8 >= 29 )
    {
      v9 = 0;
      goto LABEL_5;
    }
  }
  v9 = (&off_4FC11C)[2 * v8];
LABEL_5:
  v11[1] = v9;
  if ( v9 )
  {
    v11[2] = a3;
    v11[3] = a4;
    v11[4] = a5;
    v11[5] = a6;
    v12 = a7;
    v11[0] = a1;
    _ctrlfp(a8, 0xFFFF);
    if ( !sub_4B4D09(v11) )
      _set_errno_from_matherr(a1);
    return v12;
  }
  else
  {
    _ctrlfp(a8, 0xFFFF);
    _set_errno_from_matherr(a1);
    return a7;
  }
}

// ===== __handle_qnan1 @ 0x004B52F0..0x004B5345 =====
double __cdecl _handle_qnan1(int a1, double a2, int a3)
{
  if ( !dword_4FC110 )
    return _umatherr(
             1,
             a1,
             SLODWORD(a2),
             SHIDWORD(a2),
             COERCE_UNSIGNED_INT64(0.0),
             HIDWORD(COERCE_UNSIGNED_INT64(0.0)),
             a2,
             a3);
  *_errno() = 33;
  _ctrlfp(a3, 0xFFFF);
  return a2;
}

// ===== __handle_qnan2 @ 0x004B5345..0x004B53A6 =====
double __cdecl _handle_qnan2(int a1, double a2, double a3, int a4)
{
  double v5; // [esp+1Ch] [ebp-8h]

  v5 = a2 + a3;
  if ( !dword_4FC110 )
    return _umatherr(1, a1, SLODWORD(a2), SHIDWORD(a2), SLODWORD(a3), SHIDWORD(a3), v5, a4);
  *_errno() = 33;
  _ctrlfp(a4, 0xFFFF);
  return v5;
}

// ===== __except1 @ 0x004B53A6..0x004B5470 =====
void __usercall _except1(int a1@<ebp>, DWORD a2, int a3, int a4, int a5, double a6, int a7)
{
  int v7; // eax
  int v8; // [esp+1Ch] [ebp-8Ch] BYREF
  int v9; // [esp+5Ch] [ebp-4Ch]
  unsigned int v10; // [esp+98h] [ebp-10h]
  _DWORD v11[3]; // [esp+9Ch] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+A8h] [ebp+0h]

  v11[0] = a1;
  v11[1] = retaddr;
  v10 = (unsigned int)v11 ^ dword_4FB734;
  if ( !_handle_exc(a2, &a6, a7) )
  {
    v9 &= ~1u;
    _raise_exc_ex((ULONG_PTR)&v8, (unsigned int *)&a7, a2, a3, (float *)&a4, (float *)&a6, 0);
  }
  v7 = _errcode(a2);
  if ( dword_4FC110 || !v7 )
  {
    _set_errno_from_matherr(v7);
    _ctrlfp(a7, 0xFFFF);
  }
  else
  {
    _umatherr(v7, a3, a4, a5, COERCE_UNSIGNED_INT64(0.0), HIDWORD(COERCE_UNSIGNED_INT64(0.0)), a6, a7);
  }
  sub_4AB245((void *)((unsigned int)v11 ^ v10));
}

// ===== __except2 @ 0x004B5470..0x004B554A =====
void __usercall _except2(int a1@<ebp>, DWORD a2, int a3, int a4, int a5, double a6, double a7, int a8)
{
  int v8; // eax
  double v9[8]; // [esp+1Ch] [ebp-8Ch] BYREF
  unsigned int v10; // [esp+5Ch] [ebp-4Ch]
  unsigned int v11; // [esp+98h] [ebp-10h]
  _DWORD v12[3]; // [esp+9Ch] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+A8h] [ebp+0h]

  v12[0] = a1;
  v12[1] = retaddr;
  v11 = (unsigned int)v12 ^ dword_4FB734;
  if ( !_handle_exc(a2, &a7, a8) )
  {
    v9[6] = a6;
    v10 = v10 & 0xFFFFFFE0 | 3;
    _raise_exc_ex((ULONG_PTR)v9, (unsigned int *)&a8, a2, a3, (float *)&a4, (float *)&a7, 0);
  }
  v8 = _errcode(a2);
  if ( dword_4FC110 || !v8 )
  {
    _set_errno_from_matherr(v8);
    _ctrlfp(a8, 0xFFFF);
  }
  else
  {
    _umatherr(v8, a3, a4, a5, SLODWORD(a6), SHIDWORD(a6), a7, a8);
  }
  sub_4AB245((void *)((unsigned int)v12 ^ v11));
}

// ===== __frnd @ 0x004B554A..0x004B555E =====
double __cdecl _frnd(double a1)
{
  double result; // st7

  _ST7 = a1;
  __asm { frndint }
  return result;
}

// ===== __set_exp @ 0x004B555E..0x004B558B =====
double __cdecl _set_exp(double a1, __int16 a2)
{
  double v3; // [esp+0h] [ebp-8h]

  v3 = a1;
  HIWORD(v3) = HIWORD(a1) & 0x800F | (16 * (a2 + 1022));
  return v3;
}

// ===== __sptype @ 0x004B558B..0x004B55F1 =====
int __cdecl _sptype(int a1, int a2)
{
  if ( a2 == 2146435072 )
  {
    if ( !a1 )
      return 1;
  }
  else if ( a2 == -1048576 && !a1 )
  {
    return 2;
  }
  if ( (HIWORD(a2) & 0x7FF8) == 0x7FF8 )
    return 3;
  if ( (HIWORD(a2) & 0x7FF8) == 0x7FF0 && ((a2 & 0x7FFFF) != 0 || a1) )
    return 4;
  return 0;
}

// ===== __decomp @ 0x004B55F1..0x004B56B4 =====
double __cdecl _decomp(double a1, int *a2)
{
  double result; // st7
  int v3; // edx
  int v4; // edx
  BOOL v5; // eax

  result = 0.0;
  if ( 0.0 == a1 )
  {
    v3 = 0;
  }
  else if ( (HIWORD(a1) & 0x7FF0) == 0 && ((HIDWORD(a1) & 0xFFFFF) != 0 || LODWORD(a1)) )
  {
    v4 = -1021;
    v5 = a1 < 0.0;
    while ( (BYTE6(a1) & 0x10) == 0 )
    {
      HIDWORD(a1) *= 2;
      if ( SLODWORD(a1) < 0 )
        HIDWORD(a1) |= 1u;
      LODWORD(a1) *= 2;
      --v4;
    }
    HIWORD(a1) &= ~0x10u;
    if ( v5 )
      HIWORD(a1) |= 0x8000u;
    result = _set_exp(a1, 0);
  }
  else
  {
    result = _set_exp(a1, 0);
    v3 = ((HIWORD(a1) >> 4) & 0x7FF) - 1022;
  }
  *a2 = v3;
  return result;
}

// ===== __statfp @ 0x004B56B4..0x004B56C4 =====
int __usercall _statfp@<eax>(__int16 a1@<fpstat>)
{
  return a1;
}

// ===== __clrfp @ 0x004B56C4..0x004B56D5 =====
int __usercall _clrfp@<eax>(__int16 a1@<fpstat>)
{
  __asm { fnclex }
  return a1;
}

// ===== __ctrlfp @ 0x004B56D5..0x004B5700 =====
int __fastcall _ctrlfp(__int16 a1)
{
  return a1;
}

// ===== __set_statfp @ 0x004B5700..0x004B5758 =====
void _set_statfp()
{
  ;
}

// ===== ___set_fpsr_sse2 @ 0x004B5758..0x004B57CA =====
int __cdecl __set_fpsr_sse2(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( dword_567C00 )
  {
    if ( (a1 & 0x40) != 0 && dword_4FC240 )
      _mm_setcsr(a1);
    else
      _mm_setcsr(a1 & 0xFFFFFFBF);
  }
  return result;
}

// ===== __mtinitlocks @ 0x004B57CA..0x004B5814 =====
int _mtinitlocks()
{
  int v0; // esi
  struct _RTL_CRITICAL_SECTION *v1; // edi
  LPCRITICAL_SECTION *v2; // eax

  v0 = 0;
  v1 = (struct _RTL_CRITICAL_SECTION *)&unk_50A650;
  while ( 1 )
  {
    if ( dword_4FC24C[2 * v0] == 1 )
    {
      v2 = &lpCriticalSection + 2 * v0;
      *v2 = v1++;
      if ( !InitializeCriticalSectionAndSpinCount(*v2, 0xFA0u) )
        break;
    }
    if ( ++v0 >= 36 )
      return 1;
  }
  *(&lpCriticalSection + 2 * v0) = 0;
  return 0;
}

// ===== __mtdeletelocks @ 0x004B5814..0x004B586B =====
void _mtdeletelocks()
{
  LPCRITICAL_SECTION *v0; // esi
  LPCRITICAL_SECTION v1; // edi
  LPCRITICAL_SECTION *v2; // esi

  v0 = &lpCriticalSection;
  do
  {
    v1 = *v0;
    if ( *v0 && v0[1] != (LPCRITICAL_SECTION)1 )
    {
      DeleteCriticalSection(*v0);
      free(v1);
      *v0 = 0;
    }
    v0 += 2;
  }
  while ( (int)v0 < (int)dword_4FC368 );
  v2 = &lpCriticalSection;
  do
  {
    if ( *v2 )
    {
      if ( v2[1] == (LPCRITICAL_SECTION)1 )
        DeleteCriticalSection(*v2);
    }
    v2 += 2;
  }
  while ( (int)v2 < (int)dword_4FC368 );
}

// ===== __unlock @ 0x004B586B..0x004B5882 =====
void __cdecl _unlock(int a1)
{
  LeaveCriticalSection(*(&lpCriticalSection + 2 * a1));
}

// ===== __mtinitlocknum @ 0x004B5882..0x004B5944 =====
int __cdecl _mtinitlocknum(int a1)
{
  LPCRITICAL_SECTION *v1; // esi
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int v4; // [esp+10h] [ebp-1Ch]

  v4 = 1;
  if ( !hHeap )
  {
    _FF_MSGBANNER();
    _NMSG_WRITE(30);
    __crtExitProcess(0xFFu);
  }
  v1 = &lpCriticalSection + 2 * a1;
  if ( *v1 )
    return 1;
  v3 = (struct _RTL_CRITICAL_SECTION *)_malloc_crt(0x18u);
  if ( v3 )
  {
    _lock(10);
    if ( *v1 )
    {
      free(v3);
    }
    else if ( InitializeCriticalSectionAndSpinCount(v3, 0xFA0u) )
    {
      *v1 = v3;
    }
    else
    {
      free(v3);
      *_errno() = 12;
      v4 = 0;
    }
    _unlock(10);
    return v4;
  }
  else
  {
    *_errno() = 12;
    return 0;
  }
}

// ===== __lock @ 0x004B5944..0x004B5977 =====
void __cdecl _lock(int a1)
{
  LPCRITICAL_SECTION *v1; // esi

  v1 = &lpCriticalSection + 2 * a1;
  if ( !*v1 && !_mtinitlocknum(a1) )
    _amsg_exit(17);
  EnterCriticalSection(*v1);
}

// ===== __local_unwind4 @ 0x004B5980..0x004B5A56 =====
int __cdecl _local_unwind4(_DWORD *a1, int a2, unsigned int a3)
{
  int result; // eax
  unsigned int v4; // esi
  int v5; // esi
  int v6; // ebx
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp-8h] [ebp-28h] BYREF
  void *v8; // [esp-4h] [ebp-24h]
  unsigned int v9; // [esp+0h] [ebp-20h]
  unsigned int v10; // [esp+4h] [ebp-1Ch]
  int v11; // [esp+8h] [ebp-18h]
  _DWORD *v12; // [esp+Ch] [ebp-14h]

  v12 = a1;
  v11 = a2;
  v10 = a3;
  v8 = &_unwind_handler4;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v9 = (unsigned int)&ExceptionList ^ dword_4FB734;
  while ( 1 )
  {
    result = a2;
    v4 = *(_DWORD *)(a2 + 12);
    if ( v4 == -2 || a3 != -2 && v4 <= a3 )
      break;
    v5 = 3 * v4;
    v6 = (*a1 ^ *(_DWORD *)(a2 + 8)) + 4 * v5 + 16;
    *(_DWORD *)(a2 + 12) = *(_DWORD *)((*a1 ^ *(_DWORD *)(a2 + 8)) + 4 * v5 + 0x10);
    if ( !*(_DWORD *)(v6 + 4) )
    {
      _NLG_Notify(257);
      _NLG_Call(1, ExceptionList, v8, v9, v10, v11, v12);
    }
  }
  return result;
}

// ===== __seh_longjmp_unwind4@4 @ 0x004B5A56..0x004B5A72 =====
int __stdcall _seh_longjmp_unwind4(int a1)
{
  return _local_unwind4(*(_DWORD **)(a1 + 40), *(_DWORD *)(a1 + 24), *(_DWORD *)(a1 + 28));
}

// ===== @_EH4_CallFilterFunc@8 @ 0x004B5A72..0x004B5A89 =====
int __thiscall _EH4_CallFilterFunc(int (*this)(void))
{
  return this();
}

// ===== @_EH4_TransferToHandler@8 @ 0x004B5A89..0x004B5AA2 =====
int __thiscall _EH4_TransferToHandler(int (__fastcall *this)(_DWORD, _DWORD))
{
  _NLG_Notify(1);
  return this(0, 0);
}

// ===== @_EH4_GlobalUnwind2@8 @ 0x004B5AA2..0x004B5ABB =====
void __fastcall _EH4_GlobalUnwind2(PVOID TargetFrame, PEXCEPTION_RECORD ExceptionRecord)
{
  RtlUnwind(TargetFrame, &ReturnPoint, ExceptionRecord, 0);
}

// ===== @_EH4_LocalUnwind@16 @ 0x004B5ABB..0x004B5AD2 =====
int __fastcall _EH4_LocalUnwind(int a1, unsigned int a2, int a3, _DWORD *a4)
{
  return _local_unwind4(a4, a1, a2);
}

// ===== __calloc_impl @ 0x004B5AD2..0x004B5B54 =====
LPVOID __cdecl _calloc_impl(unsigned int a1, unsigned int a2, _DWORD *a3)
{
  LPVOID result; // eax
  SIZE_T v4; // esi

  if ( a1 && 0xFFFFFFE0 / a1 < a2 )
  {
    *_errno() = 12;
    return 0;
  }
  else
  {
    v4 = a2 * a1;
    if ( !(a2 * a1) )
      v4 = 1;
    while ( 1 )
    {
      result = 0;
      if ( v4 <= 0xFFFFFFE0 )
      {
        result = HeapAlloc(hHeap, 8u, v4);
        if ( result )
          break;
      }
      if ( !dword_50A7DC )
      {
        if ( a3 )
          *a3 = 12;
        return result;
      }
      if ( !_callnewh(v4) )
      {
        if ( a3 )
          *a3 = 12;
        return 0;
      }
    }
  }
  return result;
}

// ===== _realloc @ 0x004B5B54..0x004B5C01 =====
void *__cdecl realloc(void *Block, size_t Size)
{
  size_t v3; // esi
  void *v4; // edi
  int *v5; // esi
  DWORD LastError; // eax
  int *v7; // esi
  DWORD v8; // eax

  if ( !Block )
    return malloc(Size);
  v3 = Size;
  if ( !Size )
  {
    free(Block);
    return 0;
  }
  while ( 1 )
  {
    if ( v3 > 0xFFFFFFE0 )
    {
      _callnewh(v3);
      *_errno() = 12;
      return 0;
    }
    if ( !v3 )
      v3 = 1;
    v4 = HeapReAlloc(hHeap, 0, Block, v3);
    if ( v4 )
      return v4;
    if ( !dword_50A7DC )
      break;
    if ( !_callnewh(v3) )
    {
      v5 = _errno();
      LastError = GetLastError();
      *v5 = _get_errno_from_oserr(LastError);
      return 0;
    }
  }
  v7 = _errno();
  v8 = GetLastError();
  *v7 = _get_errno_from_oserr(v8);
  return v4;
}

// ===== __initp_misc_winsig @ 0x004B5C01..0x004B5C1F =====
void *__cdecl _initp_misc_winsig(void *a1)
{
  void *result; // eax

  result = a1;
  dword_50A7E0 = a1;
  dword_50A7E4 = (int)a1;
  dword_50A7E8 = a1;
  dword_50A7EC = (int)a1;
  return result;
}

// ===== _siglookup @ 0x004B5C1F..0x004B5C56 =====
unsigned int __usercall siglookup@<eax>(int a1@<edx>, unsigned int a2)
{
  unsigned int result; // eax

  result = a2;
  do
  {
    if ( *(_DWORD *)(result + 4) == a1 )
      break;
    result += 12;
  }
  while ( result < a2 + 144 );
  if ( result >= a2 + 144 || *(_DWORD *)(result + 4) != a1 )
    return 0;
  return result;
}

// ===== sub_4B5C56 @ 0x004B5C56..0x004B5C63 =====
PVOID sub_4B5C56()
{
  return DecodePointer(dword_50A7E8);
}

// ===== _raise @ 0x004B5C63..0x004B5E06 =====
int __cdecl raise(int Signal)
{
  DWORD *v1; // edi
  DWORD *v2; // eax
  int result; // eax
  int *v4; // esi
  PVOID v5; // eax
  void (__cdecl *v6)(int, DWORD); // eax
  DWORD v7; // [esp+10h] [ebp-30h]
  DWORD v8; // [esp+14h] [ebp-2Ch]
  int i; // [esp+1Ch] [ebp-24h]
  void (__cdecl *v10)(int, DWORD); // [esp+20h] [ebp-20h]
  int v11; // [esp+24h] [ebp-1Ch]

  v1 = 0;
  v11 = 0;
  if ( Signal > 11 )
  {
    if ( Signal == 15 )
    {
      v4 = &dword_50A7EC;
      v5 = (PVOID)dword_50A7EC;
      goto LABEL_18;
    }
    if ( Signal == 21 )
    {
      v4 = &dword_50A7E4;
      v5 = (PVOID)dword_50A7E4;
      goto LABEL_18;
    }
    if ( Signal != 22 )
      goto LABEL_14;
    goto LABEL_15;
  }
  if ( Signal != 11 )
  {
    if ( Signal == 2 )
    {
      v4 = (int *)&dword_50A7E0;
      v5 = dword_50A7E0;
LABEL_18:
      v11 = 1;
      v6 = (void (__cdecl *)(int, DWORD))DecodePointer(v5);
      goto LABEL_19;
    }
    if ( Signal != 4 )
    {
      if ( Signal != 6 )
      {
        if ( Signal == 8 )
          goto LABEL_7;
LABEL_14:
        *_errno() = 22;
        _invalid_parameter_noinfo();
        return -1;
      }
LABEL_15:
      v4 = (int *)&dword_50A7E8;
      v5 = dword_50A7E8;
      goto LABEL_18;
    }
  }
LABEL_7:
  v2 = _getptd_noexit();
  v1 = v2;
  if ( !v2 )
    return -1;
  v4 = (int *)(siglookup(Signal, v2[23]) + 8);
  v6 = (void (__cdecl *)(int, DWORD))*v4;
LABEL_19:
  v10 = v6;
  result = 0;
  if ( v10 == (void (__cdecl *)(int, DWORD))1 )
    return result;
  if ( !v10 )
    _exit(3);
  if ( v11 )
    _lock(0);
  if ( Signal == 8 || Signal == 11 || Signal == 4 )
  {
    v8 = v1[24];
    v1[24] = 0;
    if ( Signal != 8 )
      goto LABEL_33;
    v7 = v1[25];
    v1[25] = 140;
  }
  if ( Signal == 8 )
  {
    for ( i = 3; i < 12; ++i )
      *(_DWORD *)(12 * i + v1[23] + 8) = 0;
    goto LABEL_37;
  }
LABEL_33:
  *v4 = (int)_encoded_null();
LABEL_37:
  if ( v11 )
    _unlock(0);
  if ( Signal == 8 )
    v10(8, v1[25]);
  else
    ((void (__cdecl *)(int))v10)(Signal);
  if ( Signal == 8 || Signal == 11 || Signal == 4 )
  {
    v1[24] = v8;
    if ( Signal == 8 )
      v1[25] = v7;
  }
  return 0;
}

// ===== sub_4B5E06 @ 0x004B5E06..0x004B5E15 =====
int __cdecl sub_4B5E06(int a1)
{
  int result; // eax

  result = a1;
  dword_50A7F4 = a1;
  return result;
}

// ===== __ValidateImageBase @ 0x004B5E20..0x004B5E55 =====
BOOL __cdecl _ValidateImageBase(int a1)
{
  int v2; // eax

  if ( *(_WORD *)a1 == 23117 && (v2 = a1 + *(_DWORD *)(a1 + 60), *(_DWORD *)v2 == 17744) )
    return *(_WORD *)(v2 + 24) == 267;
  else
    return 0;
}

// ===== __FindPESection @ 0x004B5E60..0x004B5EA4 =====
int __cdecl _FindPESection(int a1, unsigned int a2)
{
  int v2; // ecx
  unsigned int v3; // esi
  unsigned int v4; // edx
  int result; // eax
  unsigned int v6; // ecx

  v2 = a1 + *(_DWORD *)(a1 + 60);
  v3 = *(unsigned __int16 *)(v2 + 6);
  v4 = 0;
  result = *(unsigned __int16 *)(v2 + 20) + v2 + 24;
  if ( !*(_WORD *)(v2 + 6) )
    return 0;
  while ( 1 )
  {
    v6 = *(_DWORD *)(result + 12);
    if ( a2 >= v6 && a2 < v6 + *(_DWORD *)(result + 8) )
      break;
    ++v4;
    result += 40;
    if ( v4 >= v3 )
      return 0;
  }
  return result;
}

// ===== __IsNonwritableInCurrentImage @ 0x004B5EB0..0x004B5F6C =====
BOOL __cdecl _IsNonwritableInCurrentImage(int a1)
{
  int PESection; // eax

  return _ValidateImageBase(0x400000)
      && (PESection = _FindPESection(0x400000, a1 - 0x400000)) != 0
      && *(int *)(PESection + 36) >= 0;
}

// ===== __lseeki64_nolock @ 0x004B5F6C..0x004B5FF1 =====
__int64 __cdecl _lseeki64_nolock(int FileHandle, LONG a2, int a3, DWORD dwMoveMethod)
{
  void *osfhandle; // eax
  DWORD LastError; // eax
  _BYTE *v7; // eax
  __int64 lDistanceToMove; // [esp+8h] [ebp-8h] BYREF

  HIDWORD(lDistanceToMove) = a3;
  osfhandle = (void *)_get_osfhandle(FileHandle);
  if ( osfhandle == (void *)-1 )
  {
    *_errno() = 9;
    return -1LL;
  }
  LODWORD(lDistanceToMove) = SetFilePointer(osfhandle, a2, (PLONG)&lDistanceToMove + 1, dwMoveMethod);
  if ( (_DWORD)lDistanceToMove == -1 )
  {
    LastError = GetLastError();
    if ( LastError )
    {
      _dosmaperr(LastError);
      return -1LL;
    }
  }
  v7 = (_BYTE *)(dword_567B00[FileHandle >> 5] + ((FileHandle & 0x1F) << 6) + 4);
  *v7 &= ~2u;
  return lDistanceToMove;
}

// ===== __lseeki64 @ 0x004B5FF1..0x004B60DB =====
__int64 __cdecl _lseeki64(int FileHandle, __int64 Offset, int Origin)
{
  int *v4; // edi
  int v5; // esi
  __int64 v6; // [esp+10h] [ebp-24h]

  if ( FileHandle == -2 )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    return -1LL;
  }
  if ( FileHandle < 0
    || FileHandle >= uNumber
    || (v4 = &dword_567B00[FileHandle >> 5], v5 = (FileHandle & 0x1F) << 6, (*(_BYTE *)(*v4 + v5 + 4) & 1) == 0) )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return -1LL;
  }
  __lock_fhandle(FileHandle);
  if ( (*(_BYTE *)(*v4 + v5 + 4) & 1) != 0 )
  {
    v6 = _lseeki64_nolock(FileHandle, Offset, SHIDWORD(Offset), Origin);
  }
  else
  {
    *_errno() = 9;
    *__doserrno() = 0;
    v6 = -1LL;
  }
  _unlock_fhandle(FileHandle);
  return v6;
}

// ===== __write_nolock @ 0x004B60DB..0x004B67D8 =====
int __cdecl _write_nolock(int FileHandle, const void *a2, DWORD nNumberOfBytesToWrite)
{
  int *v4; // ebx
  int v5; // eax
  int v6; // edi
  char v7; // cl
  BOOL v8; // esi
  UINT ConsoleCP; // eax
  const char *v10; // ebx
  CHAR v11; // cl
  int *v12; // esi
  int v13; // eax
  int v14; // eax
  DWORD v15; // eax
  signed int v16; // esi
  int v17; // eax
  int v18; // esi
  int v19; // ecx
  char v20; // dl
  CHAR *v21; // ebx
  unsigned int v22; // esi
  DWORD v23; // ecx
  CHAR *v24; // eax
  CHAR v25; // dl
  signed int v26; // esi
  char *v27; // ebx
  DWORD v28; // ecx
  CHAR *v29; // eax
  int v30; // edx
  signed int v31; // esi
  DWORD v32; // ecx
  WCHAR *v33; // eax
  int v34; // edx
  int v35; // esi
  int v36; // ebx
  BOOL v37; // [esp+8h] [ebp-1AE4h]
  DWORD Mode; // [esp+Ch] [ebp-1AE0h] BYREF
  int *v39; // [esp+10h] [ebp-1ADCh]
  DWORD v40; // [esp+14h] [ebp-1AD8h] BYREF
  DWORD NumberOfBytesWritten; // [esp+18h] [ebp-1AD4h] BYREF
  int v42; // [esp+1Ch] [ebp-1AD0h]
  LPCVOID lpBuffer; // [esp+20h] [ebp-1ACCh]
  DWORD v44; // [esp+24h] [ebp-1AC8h]
  char v45; // [esp+2Bh] [ebp-1AC1h]
  DWORD v46; // [esp+2Ch] [ebp-1AC0h]
  WCHAR WideCharStr[2]; // [esp+30h] [ebp-1ABCh] BYREF
  CHAR Buffer[1704]; // [esp+34h] [ebp-1AB8h] BYREF
  CHAR v49[3416]; // [esp+6DCh] [ebp-1410h] BYREF
  WCHAR v50[854]; // [esp+1434h] [ebp-6B8h] BYREF
  CHAR MultiByteStr[8]; // [esp+1AE0h] [ebp-Ch] BYREF

  lpBuffer = a2;
  v44 = 0;
  v42 = 0;
  if ( !nNumberOfBytesToWrite )
    return 0;
  if ( !a2 )
  {
    *__doserrno() = 0;
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
  v4 = &dword_567B00[FileHandle >> 5];
  v5 = *v4;
  v6 = (FileHandle & 0x1F) << 6;
  v7 = (char)(2 * *(_BYTE *)(*v4 + v6 + 36)) >> 1;
  v39 = v4;
  v45 = v7;
  if ( (v7 == 2 || v7 == 1) && (nNumberOfBytesToWrite & 1) != 0 )
  {
    *__doserrno() = 0;
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
  if ( (*(_BYTE *)(v5 + v6 + 4) & 0x20) != 0 )
    _lseeki64_nolock(FileHandle, 0, 0, 2u);
  if ( _isatty(FileHandle) )
  {
    if ( *(char *)(v6 + *v4 + 4) < 0 )
    {
      v8 = *(_DWORD *)(_getptd()[27] + 20) == 0;
      if ( GetConsoleMode(*(HANDLE *)(v6 + *v4), &Mode) )
      {
        if ( !v8 || v45 )
        {
          ConsoleCP = GetConsoleCP();
          v10 = (const char *)lpBuffer;
          Mode = ConsoleCP;
          NumberOfBytesWritten = 0;
          v46 = 0;
          while ( 1 )
          {
            if ( v45 )
            {
              if ( v45 == 1 || v45 == 2 )
              {
                v18 = *(unsigned __int16 *)v10;
                v10 += 2;
                v46 += 2;
                *(_DWORD *)WideCharStr = v18;
                v37 = v18 == 10;
              }
              if ( v45 == 1 || v45 == 2 )
              {
                if ( _putwch_nolock(WideCharStr[0]) != WideCharStr[0] )
                  goto LABEL_85;
                v44 += 2;
                if ( v37 )
                {
                  wcscpy(WideCharStr, L"\r");
                  if ( _putwch_nolock(0xDu) != WideCharStr[0] )
                    goto LABEL_85;
                  ++v44;
                  ++v42;
                }
              }
              goto LABEL_43;
            }
            v11 = *v10;
            v12 = v39;
            v37 = *v10 == 10;
            v13 = v6 + *v39;
            if ( *(_DWORD *)(v13 + 56) )
            {
              MultiByteStr[0] = *(_BYTE *)(v13 + 52);
              MultiByteStr[1] = v11;
              *(_DWORD *)(v13 + 56) = 0;
              v14 = mbtowc(WideCharStr, MultiByteStr, 2u);
            }
            else
            {
              if ( isleadbyte(v11) )
              {
                if ( nNumberOfBytesToWrite + (_BYTE *)lpBuffer - v10 <= 1 )
                {
                  v19 = *v12;
                  v20 = *v10;
                  ++v44;
                  *(_BYTE *)(v6 + v19 + 52) = v20;
                  *(_DWORD *)(v6 + *v12 + 56) = 1;
                  goto LABEL_86;
                }
                if ( mbtowc(WideCharStr, v10, 2u) == -1 )
                  goto LABEL_86;
                ++v10;
                ++v46;
                goto LABEL_27;
              }
              v14 = mbtowc(WideCharStr, v10, 1u);
            }
            if ( v14 == -1 )
              goto LABEL_86;
LABEL_27:
            ++v10;
            ++v46;
            v15 = WideCharToMultiByte(Mode, 0, WideCharStr, 1, MultiByteStr, 5, 0, 0);
            v16 = v15;
            if ( !v15 )
              goto LABEL_86;
            if ( !WriteFile(*(HANDLE *)(v6 + *v39), MultiByteStr, v15, &NumberOfBytesWritten, 0) )
              goto LABEL_85;
            v44 = v42 + v46;
            if ( (int)NumberOfBytesWritten < v16 )
              goto LABEL_86;
            if ( v37 )
            {
              v17 = *v39;
              MultiByteStr[0] = 13;
              if ( !WriteFile(*(HANDLE *)(v6 + v17), MultiByteStr, 1u, &NumberOfBytesWritten, 0) )
                goto LABEL_85;
              if ( (int)NumberOfBytesWritten < 1 )
                goto LABEL_86;
              ++v42;
              ++v44;
            }
LABEL_43:
            if ( v46 >= nNumberOfBytesToWrite )
              goto LABEL_86;
          }
        }
      }
    }
  }
  if ( *(char *)(*v4 + v6 + 4) >= 0 )
  {
    if ( WriteFile(*(HANDLE *)(*v4 + ((FileHandle & 0x1F) << 6)), lpBuffer, nNumberOfBytesToWrite, &v40, 0) )
    {
      *(_DWORD *)WideCharStr = 0;
      v44 = v40;
      goto LABEL_86;
    }
  }
  else
  {
    *(_DWORD *)WideCharStr = 0;
    if ( v45 )
    {
      if ( v45 == 2 )
      {
        v27 = (char *)lpBuffer;
        while ( 1 )
        {
          v46 = 0;
          v28 = v27 - (_BYTE *)lpBuffer;
          v29 = Buffer;
          do
          {
            if ( v28 >= nNumberOfBytesToWrite )
              break;
            v30 = *(unsigned __int16 *)v27;
            v27 += 2;
            v28 += 2;
            Mode = (DWORD)v27;
            if ( v30 == 10 )
            {
              v42 += 2;
              *(_WORD *)v29 = 13;
              v27 = (char *)Mode;
              v29 += 2;
              v46 += 2;
            }
            v46 += 2;
            *(_WORD *)v29 = v30;
            v29 += 2;
          }
          while ( v46 < 0x13FE );
          v31 = v29 - Buffer;
          if ( !WriteFile(*(HANDLE *)(v6 + *v39), Buffer, v29 - Buffer, &v40, 0) )
            break;
          v44 += v40;
          if ( (int)v40 < v31 || v27 - (_BYTE *)lpBuffer >= nNumberOfBytesToWrite )
            goto LABEL_86;
        }
      }
      else
      {
        NumberOfBytesWritten = (DWORD)lpBuffer;
        while ( 1 )
        {
          v46 = 0;
          v32 = NumberOfBytesWritten - (_DWORD)lpBuffer;
          v33 = v50;
          do
          {
            if ( v32 >= nNumberOfBytesToWrite )
              break;
            v34 = *(unsigned __int16 *)NumberOfBytesWritten;
            NumberOfBytesWritten += 2;
            v32 += 2;
            if ( v34 == 10 )
            {
              *v33++ = 13;
              v46 += 2;
            }
            v46 += 2;
            *v33++ = v34;
          }
          while ( v46 < 0x6A8 );
          v35 = 0;
          v36 = WideCharToMultiByte(0xFDE9u, 0, v50, v33 - v50, v49, 3413, 0, 0);
          if ( !v36 )
            break;
          while ( WriteFile(*(HANDLE *)(v6 + *v39), &v49[v35], v36 - v35, &v40, 0) )
          {
            v35 += v40;
            if ( v36 <= v35 )
              goto LABEL_80;
          }
          *(_DWORD *)WideCharStr = GetLastError();
LABEL_80:
          if ( v36 <= v35 )
          {
            v44 = NumberOfBytesWritten - (_DWORD)lpBuffer;
            if ( NumberOfBytesWritten - (unsigned int)lpBuffer < nNumberOfBytesToWrite )
              continue;
          }
          goto LABEL_86;
        }
      }
    }
    else
    {
      v21 = (CHAR *)lpBuffer;
      while ( 1 )
      {
        v22 = 0;
        v23 = v21 - (_BYTE *)lpBuffer;
        v24 = Buffer;
        do
        {
          if ( v23 >= nNumberOfBytesToWrite )
            break;
          v25 = *v21++;
          ++v23;
          Mode = (DWORD)v21;
          if ( v25 == 10 )
          {
            ++v42;
            *v24++ = 13;
            ++v22;
          }
          *v24++ = v25;
          ++v22;
        }
        while ( v22 < 0x13FF );
        v26 = v24 - Buffer;
        if ( !WriteFile(*(HANDLE *)(v6 + *v39), Buffer, v24 - Buffer, &v40, 0) )
          break;
        v44 += v40;
        if ( (int)v40 < v26 || v21 - (_BYTE *)lpBuffer >= nNumberOfBytesToWrite )
          goto LABEL_86;
      }
    }
  }
LABEL_85:
  *(_DWORD *)WideCharStr = GetLastError();
LABEL_86:
  if ( !v44 )
  {
    if ( *(_DWORD *)WideCharStr )
    {
      if ( *(_DWORD *)WideCharStr == 5 )
      {
        *_errno() = 9;
        *__doserrno() = 5;
      }
      else
      {
        _dosmaperr(*(unsigned int *)WideCharStr);
      }
    }
    else
    {
      if ( (*(_BYTE *)(v6 + *v39 + 4) & 0x40) != 0 && *(_BYTE *)lpBuffer == 26 )
        return 0;
      *_errno() = 28;
      *__doserrno() = 0;
    }
    return -1;
  }
  return v44 - v42;
}

// ===== __write @ 0x004B67D8..0x004B68AC =====
int __cdecl _write(int FileHandle, const void *Buf, unsigned int MaxCharCount)
{
  int *v4; // edi
  int v5; // esi
  int v6; // [esp+14h] [ebp-1Ch]

  if ( FileHandle == -2 )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    return -1;
  }
  if ( FileHandle < 0
    || FileHandle >= uNumber
    || (v4 = &dword_567B00[FileHandle >> 5], v5 = (FileHandle & 0x1F) << 6, (*(_BYTE *)(*v4 + v5 + 4) & 1) == 0) )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return -1;
  }
  __lock_fhandle(FileHandle);
  if ( (*(_BYTE *)(*v4 + v5 + 4) & 1) != 0 )
  {
    v6 = _write_nolock(FileHandle, Buf, MaxCharCount);
  }
  else
  {
    *_errno() = 9;
    *__doserrno() = 0;
    v6 = -1;
  }
  _unlock_fhandle(FileHandle);
  return v6;
}

// ===== __getbuf @ 0x004B68AC..0x004B68F5 =====
int __cdecl _getbuf(_DWORD *a1)
{
  void *v1; // eax
  int result; // eax

  ++dword_50A7F8;
  v1 = _malloc_crt(0x1000u);
  a1[2] = v1;
  if ( v1 )
  {
    a1[3] |= 8u;
    a1[6] = 4096;
  }
  else
  {
    a1[3] |= 4u;
    a1[2] = a1 + 5;
    a1[6] = 2;
  }
  result = a1[2];
  a1[1] = 0;
  *a1 = result;
  return result;
}

// ===== __isatty @ 0x004B68F5..0x004B694B =====
int __cdecl _isatty(int FileHandle)
{
  if ( FileHandle == -2 )
  {
    *_errno() = 9;
    return 0;
  }
  if ( FileHandle < 0 || FileHandle >= uNumber )
  {
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return 0;
  }
  return *(_BYTE *)(dword_567B00[FileHandle >> 5] + ((FileHandle & 0x1F) << 6) + 4) & 0x40;
}

// ===== sub_4B694B @ 0x004B694B..0x004B6951 =====
_UNKNOWN **sub_4B694B()
{
  return &off_4FC370;
}

// ===== ___initstdio @ 0x004B6951..0x004B6A02 =====
int __initstdio()
{
  int v0; // eax
  char *v1; // eax
  int v3; // edx
  _UNKNOWN **v4; // ecx
  int v5; // edx
  _DWORD *v6; // ecx
  int v7; // eax

  v0 = dword_567AE0;
  if ( !dword_567AE0 )
  {
    v0 = 512;
LABEL_5:
    dword_567AE0 = v0;
    goto LABEL_6;
  }
  if ( dword_567AE0 < 20 )
  {
    v0 = 20;
    goto LABEL_5;
  }
LABEL_6:
  v1 = (char *)_calloc_crt(v0, 4);
  dword_566AD0 = v1;
  if ( !v1 )
  {
    dword_567AE0 = 20;
    v1 = (char *)_calloc_crt(20, 4);
    dword_566AD0 = v1;
    if ( !v1 )
      return 26;
  }
  v3 = 0;
  v4 = &off_4FC370;
  while ( 1 )
  {
    *(_DWORD *)&v1[v3] = v4;
    v4 += 8;
    v3 += 4;
    if ( (int)v4 >= (int)&dword_4FC5F0 )
      break;
    v1 = (char *)dword_566AD0;
  }
  v5 = 0;
  v6 = &unk_4FC380;
  do
  {
    v7 = *(_DWORD *)(((v5 & 0x1F) << 6) + dword_567B00[v5 >> 5]);
    if ( v7 == -1 || v7 == -2 || !v7 )
      *v6 = -2;
    v6 += 8;
    ++v5;
  }
  while ( (int)v6 < (int)dword_4FC3E0 );
  return 0;
}

// ===== ___endstdio @ 0x004B6A02..0x004B6A22 =====
void __endstdio()
{
  sub_4B8868();
  if ( byte_509ED0 )
    _fcloseall();
  free(dword_566AD0);
}

// ===== __lock_file @ 0x004B6A22..0x004B6A63 =====
void __cdecl _lock_file(FILE *Stream)
{
  if ( Stream < (FILE *)&off_4FC370 || Stream > &stru_4FC5D0 )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)&Stream[1]);
  }
  else
  {
    _lock((((char *)Stream - (char *)&off_4FC370) >> 5) + 16);
    Stream->_flag |= 0x8000u;
  }
}

// ===== __lock_file2 @ 0x004B6A63..0x004B6A95 =====
void __cdecl _lock_file2(int a1, int a2)
{
  if ( a1 >= 20 )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(a2 + 32));
  }
  else
  {
    _lock(a1 + 16);
    *(_DWORD *)(a2 + 12) |= 0x8000u;
  }
}

// ===== __unlock_file @ 0x004B6A95..0x004B6AD1 =====
void __cdecl _unlock_file(FILE *Stream)
{
  if ( Stream < (FILE *)&off_4FC370 || Stream > &stru_4FC5D0 )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)&Stream[1]);
  }
  else
  {
    Stream->_flag &= ~0x8000u;
    _unlock((((char *)Stream - (char *)&off_4FC370) >> 5) + 16);
  }
}

// ===== __unlock_file2 @ 0x004B6AD1..0x004B6B00 =====
void __cdecl _unlock_file2(int a1, int a2)
{
  if ( a1 >= 20 )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(a2 + 32));
  }
  else
  {
    *(_DWORD *)(a2 + 12) &= ~0x8000u;
    _unlock(a1 + 16);
  }
}

// ===== __fileno @ 0x004B6B00..0x004B6B26 =====
int __cdecl _fileno(FILE *Stream)
{
  if ( Stream )
    return Stream->_file;
  *_errno() = 22;
  _invalid_parameter_noinfo();
  return -1;
}

// ===== __get_printf_count_output @ 0x004B6B26..0x004B6B3C =====
int __cdecl _get_printf_count_output()
{
  return dword_50A7FC == (dword_4FB734 | 1);
}

// ===== __wctomb_s_l @ 0x004B6B3C..0x004B6C91 =====
errno_t __cdecl _wctomb_s_l(int *SizeConverted, char *MbCh, size_t SizeInBytes, wchar_t WCh, _locale_t Locale)
{
  char *v5; // esi
  size_t v6; // edi
  errno_t result; // eax
  errno_t v8; // esi
  int v9; // eax
  int v10; // [esp+Ch] [ebp-10h] BYREF
  int v11; // [esp+14h] [ebp-8h]
  char v12; // [esp+18h] [ebp-4h]

  v5 = MbCh;
  v6 = SizeInBytes;
  if ( !MbCh && SizeInBytes )
  {
    if ( SizeConverted )
      *SizeConverted = 0;
    return 0;
  }
  if ( SizeConverted )
    *SizeConverted = -1;
  if ( v6 > 0x7FFFFFFF )
  {
    v8 = 22;
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return v8;
  }
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v10, (struct localeinfo_struct *)Locale);
  if ( !*(_DWORD *)(v10 + 20) )
  {
    if ( WCh > 0xFFu )
    {
      if ( v5 && v6 )
        memset(v5, 0, v6);
      goto LABEL_16;
    }
    if ( v5 )
    {
      if ( !v6 )
      {
LABEL_21:
        v8 = 34;
        *_errno() = 34;
        _invalid_parameter_noinfo();
        if ( v12 )
          *(_DWORD *)(v11 + 112) &= ~2u;
        return v8;
      }
      *v5 = WCh;
    }
    if ( SizeConverted )
      *SizeConverted = 1;
LABEL_26:
    if ( v12 )
      *(_DWORD *)(v11 + 112) &= ~2u;
    return 0;
  }
  MbCh = 0;
  v9 = WideCharToMultiByte(*(_DWORD *)(v10 + 4), 0, &WCh, 1, v5, v6, 0, (LPBOOL)&MbCh);
  if ( v9 )
  {
    if ( !MbCh )
    {
      if ( SizeConverted )
        *SizeConverted = v9;
      goto LABEL_26;
    }
  }
  else if ( GetLastError() == 122 )
  {
    if ( v5 && v6 )
      memset(v5, 0, v6);
    goto LABEL_21;
  }
LABEL_16:
  *_errno() = 42;
  result = *_errno();
  if ( v12 )
    *(_DWORD *)(v11 + 112) &= ~2u;
  return result;
}

// ===== _wctomb_s @ 0x004B6C91..0x004B6CAE =====
errno_t __cdecl wctomb_s(int *SizeConverted, char *MbCh, rsize_t SizeInBytes, wchar_t WCh)
{
  return _wctomb_s_l(SizeConverted, MbCh, SizeInBytes, WCh, 0);
}

// ===== __isleadbyte_l @ 0x004B6CAE..0x004B6CE6 =====
int __cdecl _isleadbyte_l(int C, _locale_t Locale)
{
  int result; // eax
  int v3; // [esp+0h] [ebp-10h] BYREF
  int v4; // [esp+8h] [ebp-8h]
  char v5; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v3, (struct localeinfo_struct *)Locale);
  result = *(_WORD *)(*(_DWORD *)(v3 + 200) + 2 * (unsigned __int8)C) & 0x8000;
  if ( v5 )
    *(_DWORD *)(v4 + 112) &= ~2u;
  return result;
}

// ===== _isleadbyte @ 0x004B6CE6..0x004B6CF9 =====
int __cdecl isleadbyte(int C)
{
  return _isleadbyte_l(C, 0);
}

// ===== __aulldvrm @ 0x004B6D00..0x004B6D95 =====
unsigned int __stdcall _aulldvrm(unsigned __int64 a1, __int64 a2)
{
  unsigned __int64 v2; // rtt
  unsigned int v3; // esi
  unsigned int v4; // ecx
  unsigned int v5; // ebx
  unsigned __int64 v6; // rax
  char v7; // cf
  unsigned __int64 v8; // rax

  if ( HIDWORD(a2) )
  {
    v4 = HIDWORD(a2);
    v5 = a2;
    v6 = a1;
    do
    {
      v7 = v4 & 1;
      v4 >>= 1;
      v5 = (v5 >> 1) | (v7 << 31);
      v6 >>= 1;
    }
    while ( v4 );
    v3 = v6 / v5;
    v8 = v3 * (unsigned __int64)(unsigned int)a2;
    if ( __CFADD__(HIDWORD(a2) * v3, HIDWORD(v8)) || (HIDWORD(v8) = (a2 * (unsigned __int64)v3) >> 32, v8 > a1) )
      --v3;
  }
  else
  {
    LODWORD(v2) = a1;
    HIDWORD(v2) = HIDWORD(a1) % (unsigned int)a2;
    return v2 / (unsigned int)a2;
  }
  return v3;
}

// ===== ?__crtGetStringTypeA_stat@@YAHPAUlocaleinfo_struct@@KPBDHPAGHHH@Z @ 0x004B6D95..0x004B6E7C =====
BOOL __cdecl __crtGetStringTypeA_stat(
        struct localeinfo_struct *a1,
        DWORD dwInfoType,
        LPCCH lpMultiByteStr,
        int cbMultiByte,
        LPWORD lpCharType,
        UINT CodePage,
        int a7)
{
  void *v7; // ebx
  int v8; // eax
  int v9; // edi
  unsigned int v11; // eax
  void *v12; // esp
  _DWORD *v13; // eax
  int v14; // eax
  _DWORD v15[3]; // [esp+0h] [ebp-14h] BYREF
  BOOL StringTypeW; // [esp+Ch] [ebp-8h]

  v7 = 0;
  StringTypeW = 0;
  if ( !CodePage )
    CodePage = *(_DWORD *)(*(_DWORD *)a1 + 4);
  v8 = MultiByteToWideChar(CodePage, 8 * (a7 != 0) + 1, lpMultiByteStr, cbMultiByte, 0, 0);
  v9 = v8;
  if ( !v8 )
    return 0;
  if ( v8 <= 0 || (unsigned int)v8 > 0x7FFFFFF0 )
    goto LABEL_14;
  v11 = 2 * v8 + 8;
  if ( v11 > 0x400 )
  {
    v13 = malloc(2 * v9 + 8);
    if ( v13 )
    {
      *v13 = 56797;
      goto LABEL_12;
    }
  }
  else
  {
    v12 = alloca(v11);
    v13 = v15;
    if ( v15 )
    {
      v15[0] = 52428;
LABEL_12:
      v13 += 2;
    }
  }
  v7 = v13;
LABEL_14:
  if ( !v7 )
    return 0;
  memset(v7, 0, 2 * v9);
  v14 = MultiByteToWideChar(CodePage, 1u, lpMultiByteStr, cbMultiByte, (LPWSTR)v7, v9);
  if ( v14 )
    StringTypeW = GetStringTypeW(dwInfoType, (LPCWCH)v7, v14, lpCharType);
  _freea(v7);
  return StringTypeW;
}

// ===== ___crtGetStringTypeA @ 0x004B6E7C..0x004B6EBC =====
BOOL __cdecl __crtGetStringTypeA(
        struct localeinfo_struct *a1,
        DWORD dwInfoType,
        LPCCH lpMultiByteStr,
        int cbMultiByte,
        LPWORD lpCharType,
        UINT CodePage,
        int a7,
        int a8)
{
  BOOL result; // eax
  _BYTE v9[8]; // [esp+0h] [ebp-10h] BYREF
  int v10; // [esp+8h] [ebp-8h]
  char v11; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v9, a1);
  result = __crtGetStringTypeA_stat(
             (struct localeinfo_struct *)v9,
             dwInfoType,
             lpMultiByteStr,
             cbMultiByte,
             lpCharType,
             CodePage,
             a8);
  if ( v11 )
    *(_DWORD *)(v10 + 112) &= ~2u;
  return result;
}

// ===== ___free_lc_time @ 0x004B6EBC..0x004B7233 =====
void __cdecl __free_lc_time(void **a1)
{
  if ( a1 )
  {
    free(a1[1]);
    free(a1[2]);
    free(a1[3]);
    free(a1[4]);
    free(a1[5]);
    free(a1[6]);
    free(*a1);
    free(a1[8]);
    free(a1[9]);
    free(a1[10]);
    free(a1[11]);
    free(a1[12]);
    free(a1[13]);
    free(a1[7]);
    free(a1[14]);
    free(a1[15]);
    free(a1[16]);
    free(a1[17]);
    free(a1[18]);
    free(a1[19]);
    free(a1[20]);
    free(a1[21]);
    free(a1[22]);
    free(a1[23]);
    free(a1[24]);
    free(a1[25]);
    free(a1[26]);
    free(a1[27]);
    free(a1[28]);
    free(a1[29]);
    free(a1[30]);
    free(a1[31]);
    free(a1[32]);
    free(a1[33]);
    free(a1[34]);
    free(a1[35]);
    free(a1[36]);
    free(a1[37]);
    free(a1[38]);
    free(a1[39]);
    free(a1[40]);
    free(a1[41]);
    free(a1[42]);
    free(a1[47]);
    free(a1[48]);
    free(a1[49]);
    free(a1[50]);
    free(a1[51]);
    free(a1[52]);
    free(a1[46]);
    free(a1[54]);
    free(a1[55]);
    free(a1[56]);
    free(a1[57]);
    free(a1[58]);
    free(a1[59]);
    free(a1[53]);
    free(a1[60]);
    free(a1[61]);
    free(a1[62]);
    free(a1[63]);
    free(a1[64]);
    free(a1[65]);
    free(a1[66]);
    free(a1[67]);
    free(a1[68]);
    free(a1[69]);
    free(a1[70]);
    free(a1[71]);
    free(a1[72]);
    free(a1[73]);
    free(a1[74]);
    free(a1[75]);
    free(a1[76]);
    free(a1[77]);
    free(a1[78]);
    free(a1[79]);
    free(a1[80]);
    free(a1[81]);
    free(a1[82]);
    free(a1[83]);
    free(a1[84]);
    free(a1[85]);
    free(a1[86]);
    free(a1[87]);
    free(a1[88]);
  }
}

// ===== ___free_lconv_num @ 0x004B7233..0x004B729C =====
void __cdecl __free_lconv_num(int a1)
{
  void *v1; // esi

  if ( a1 )
  {
    if ( *(_UNKNOWN **)a1 != off_4FC5F8 )
      free(*(void **)a1);
    if ( *(_UNKNOWN **)(a1 + 4) != off_4FC5FC )
      free(*(void **)(a1 + 4));
    if ( *(_UNKNOWN **)(a1 + 8) != off_4FC600 )
      free(*(void **)(a1 + 8));
    if ( *(_UNKNOWN **)(a1 + 48) != off_4FC628 )
      free(*(void **)(a1 + 48));
    v1 = *(void **)(a1 + 52);
    if ( v1 != off_4FC62C )
      free(v1);
  }
}

// ===== ___free_lconv_mon @ 0x004B729C..0x004B739A =====
void __cdecl __free_lconv_mon(int a1)
{
  void *v1; // esi

  if ( a1 )
  {
    if ( *(_UNKNOWN **)(a1 + 12) != off_4FC604 )
      free(*(void **)(a1 + 12));
    if ( *(_UNKNOWN **)(a1 + 16) != off_4FC608 )
      free(*(void **)(a1 + 16));
    if ( *(_UNKNOWN **)(a1 + 20) != off_4FC60C )
      free(*(void **)(a1 + 20));
    if ( *(_UNKNOWN **)(a1 + 24) != off_4FC610 )
      free(*(void **)(a1 + 24));
    if ( *(_UNKNOWN **)(a1 + 28) != off_4FC614 )
      free(*(void **)(a1 + 28));
    if ( *(_UNKNOWN **)(a1 + 32) != off_4FC618 )
      free(*(void **)(a1 + 32));
    if ( *(_UNKNOWN **)(a1 + 36) != off_4FC61C )
      free(*(void **)(a1 + 36));
    if ( *(_UNKNOWN **)(a1 + 56) != off_4FC630 )
      free(*(void **)(a1 + 56));
    if ( *(_UNKNOWN **)(a1 + 60) != off_4FC634 )
      free(*(void **)(a1 + 60));
    if ( *(_UNKNOWN **)(a1 + 64) != off_4FC638 )
      free(*(void **)(a1 + 64));
    if ( *(_UNKNOWN **)(a1 + 68) != off_4FC63C )
      free(*(void **)(a1 + 68));
    if ( *(_UNKNOWN **)(a1 + 72) != off_4FC640 )
      free(*(void **)(a1 + 72));
    v1 = *(void **)(a1 + 76);
    if ( v1 != off_4FC644 )
      free(v1);
  }
}

// ===== __isctype_l @ 0x004B739A..0x004B7452 =====
int __cdecl _isctype_l(int C, int Type, _locale_t Locale)
{
  __int16 v3; // bx
  int v4; // eax
  int v5; // ecx
  int result; // eax
  __crt_locale_pointers Localea; // [esp+4h] [ebp-18h] BYREF
  int v8; // [esp+Ch] [ebp-10h]
  char v9; // [esp+10h] [ebp-Ch]
  CHAR MultiByteStr; // [esp+14h] [ebp-8h] BYREF
  char v11; // [esp+15h] [ebp-7h]
  char v12; // [esp+16h] [ebp-6h]
  WORD CharType; // [esp+18h] [ebp-4h] BYREF
  int Ca; // [esp+24h] [ebp+8h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Localea, (struct localeinfo_struct *)Locale);
  v3 = C;
  if ( (unsigned int)(C + 1) <= 0x100 )
  {
    v4 = *(unsigned __int16 *)(*((_DWORD *)Localea.locinfo + 50) + 2 * C);
    goto LABEL_11;
  }
  Ca = C >> 8;
  if ( _isleadbyte_l(HIBYTE(v3), &Localea) )
  {
    MultiByteStr = Ca;
    v11 = v3;
    v12 = 0;
    v5 = 2;
  }
  else
  {
    MultiByteStr = v3;
    v11 = 0;
    v5 = 1;
  }
  if ( __crtGetStringTypeA(
         (struct localeinfo_struct *)&Localea,
         1u,
         &MultiByteStr,
         v5,
         &CharType,
         *((_DWORD *)Localea.locinfo + 1),
         *((_DWORD *)Localea.locinfo + 5),
         1) )
  {
    v4 = CharType;
LABEL_11:
    result = Type & v4;
    if ( v9 )
      *(_DWORD *)(v8 + 112) &= ~2u;
    return result;
  }
  if ( v9 )
    *(_DWORD *)(v8 + 112) &= ~2u;
  return 0;
}

// ===== _strcspn @ 0x004B7460..0x004B74A6 =====
size_t __cdecl strcspn(const char *Str, const char *Control)
{
  unsigned int v2; // eax
  size_t v5; // ecx
  signed __int32 v7[9]; // [esp+0h] [ebp-24h] BYREF

  v2 = 0;
  memset(v7, 0, 32);
  while ( 1 )
  {
    LOBYTE(v2) = *Control;
    if ( !*Control )
      break;
    ++Control;
    _bittestandset(v7, v2);
  }
  v5 = -1;
  do
  {
    ++v5;
    LOBYTE(v2) = *Str;
    if ( !*Str )
      break;
    ++Str;
  }
  while ( !_bittest(v7, v2) );
  return v5;
}

// ===== _strncpy_s @ 0x004B74A6..0x004B755B =====
errno_t __cdecl strncpy_s(char *Destination, rsize_t SizeInBytes, const char *Source, rsize_t MaxCount)
{
  rsize_t v5; // edi
  errno_t v6; // esi
  const char *v7; // edx
  char *v8; // ecx
  char v9; // cl
  char v10; // dl

  if ( MaxCount )
  {
    if ( !Destination )
    {
LABEL_7:
      v6 = 22;
      *_errno() = 22;
LABEL_8:
      _invalid_parameter_noinfo();
      return v6;
    }
  }
  else if ( !Destination )
  {
    if ( !SizeInBytes )
      return 0;
    goto LABEL_7;
  }
  v5 = SizeInBytes;
  if ( !SizeInBytes )
    goto LABEL_7;
  if ( !MaxCount )
  {
    *Destination = 0;
    return 0;
  }
  v7 = Source;
  if ( !Source )
  {
    *Destination = 0;
    goto LABEL_7;
  }
  v8 = Destination;
  if ( MaxCount == -1 )
  {
    do
    {
      v9 = *v7;
      v7[Destination - Source] = *v7;
      ++v7;
      if ( !v9 )
        break;
      --v5;
    }
    while ( v5 );
  }
  else
  {
    do
    {
      v10 = v8[Source - Destination];
      *v8++ = v10;
      if ( !v10 )
        break;
      if ( !--v5 )
        break;
      --MaxCount;
    }
    while ( MaxCount );
    if ( !MaxCount )
      *v8 = 0;
  }
  if ( v5 )
    return 0;
  if ( MaxCount != -1 )
  {
    *Destination = 0;
    *_errno() = 34;
    v6 = 34;
    goto LABEL_8;
  }
  Destination[SizeInBytes - 1] = 0;
  return 80;
}

// ===== _strpbrk @ 0x004B7560..0x004B75A0 =====
char *__cdecl strpbrk(const char *Str, const char *Control)
{
  char *result; // eax
  signed __int32 v5[9]; // [esp+0h] [ebp-24h] BYREF

  result = 0;
  memset(v5, 0, 32);
  while ( 1 )
  {
    LOBYTE(result) = *Control;
    if ( !*Control )
      break;
    ++Control;
    _bittestandset(v5, (unsigned int)result);
  }
  while ( 1 )
  {
    LOBYTE(result) = *Str;
    if ( !*Str )
      break;
    ++Str;
    if ( _bittest(v5, (unsigned int)result) )
      return (char *)(Str - 1);
  }
  return result;
}

// ===== __allmul @ 0x004B75A0..0x004B75D4 =====
unsigned __int64 __stdcall _allmul(__int64 a1, __int64 a2)
{
  if ( HIDWORD(a1) | HIDWORD(a2) )
    return a1 * a2;
  else
    return (unsigned int)a2 * (unsigned __int64)(unsigned int)a1;
}

// ===== sub_4B75D4 @ 0x004B75D4..0x004B7740 =====
int __cdecl sub_4B75D4(int a1, int a2, int a3)
{
  HMODULE LibraryW; // eax
  HMODULE v4; // ebx
  int (__stdcall *MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT); // eax
  HWND (__stdcall *GetActiveWindow)(); // eax
  HWND (__stdcall *GetLastActivePopup)(HWND); // eax
  BOOL (__stdcall *GetUserObjectInformationW)(HANDLE, int, PVOID, DWORD, LPDWORD); // eax
  HWINSTA (__stdcall *GetProcessWindowStation)(); // eax
  int (*v10)(void); // edi
  PVOID v11; // eax
  int (__stdcall *v12)(int, int, _BYTE *, int, _BYTE *); // ebx
  int v13; // eax
  int (*v14)(void); // eax
  int (__stdcall *v15)(int); // eax
  int (__stdcall *v16)(int, int, int, int); // eax
  _BYTE v18[4]; // [esp+Ch] [ebp-24h] BYREF
  int v19; // [esp+10h] [ebp-20h]
  int v20; // [esp+14h] [ebp-1Ch]
  PVOID v21; // [esp+18h] [ebp-18h]
  int v22; // [esp+1Ch] [ebp-14h]
  _BYTE v23[12]; // [esp+20h] [ebp-10h] BYREF

  v19 = a1;
  v20 = a2;
  v22 = 0;
  v21 = _encoded_null();
  if ( !dword_50A808 )
  {
    LibraryW = LoadLibraryW(L"USER32.DLL");
    v4 = LibraryW;
    if ( !LibraryW )
      return 0;
    MessageBoxW = (int (__stdcall *)(HWND, LPCWSTR, LPCWSTR, UINT))GetProcAddress(LibraryW, "MessageBoxW");
    if ( !MessageBoxW )
      return 0;
    dword_50A808 = EncodePointer(MessageBoxW);
    GetActiveWindow = (HWND (__stdcall *)())GetProcAddress(v4, "GetActiveWindow");
    dword_50A80C = EncodePointer(GetActiveWindow);
    GetLastActivePopup = (HWND (__stdcall *)(HWND))GetProcAddress(v4, "GetLastActivePopup");
    dword_50A810 = EncodePointer(GetLastActivePopup);
    GetUserObjectInformationW = (BOOL (__stdcall *)(HANDLE, int, PVOID, DWORD, LPDWORD))GetProcAddress(
                                                                                          v4,
                                                                                          "GetUserObjectInformationW");
    dword_50A818 = EncodePointer(GetUserObjectInformationW);
    if ( dword_50A818 )
    {
      GetProcessWindowStation = (HWINSTA (__stdcall *)())GetProcAddress(v4, "GetProcessWindowStation");
      dword_50A814 = EncodePointer(GetProcessWindowStation);
    }
  }
  if ( dword_50A814 == v21
    || dword_50A818 == v21
    || (v10 = (int (*)(void))DecodePointer(dword_50A814),
        v11 = DecodePointer(dword_50A818),
        v12 = (int (__stdcall *)(int, int, _BYTE *, int, _BYTE *))v11,
        !v10)
    || !v11
    || (v13 = v10()) != 0 && v12(v13, 1, v23, 12, v18) && (v23[8] & 1) != 0 )
  {
    if ( dword_50A80C != v21 )
    {
      v14 = (int (*)(void))DecodePointer(dword_50A80C);
      if ( v14 )
      {
        v22 = v14();
        if ( v22 )
        {
          if ( dword_50A810 != v21 )
          {
            v15 = (int (__stdcall *)(int))DecodePointer(dword_50A810);
            if ( v15 )
              v22 = v15(v22);
          }
        }
      }
    }
  }
  else
  {
    a3 |= 0x200000u;
  }
  v16 = (int (__stdcall *)(int, int, int, int))DecodePointer(dword_50A808);
  if ( v16 )
    return v16(v22, v19, v20, a3);
  return 0;
}

// ===== _wcscat_s @ 0x004B7740..0x004B77B5 =====
errno_t __cdecl wcscat_s(wchar_t *Destination, rsize_t SizeInWords, const wchar_t *Source)
{
  rsize_t v3; // edi
  errno_t v4; // esi
  errno_t result; // eax
  const wchar_t *v6; // ecx
  wchar_t *v7; // edx
  int v8; // edx
  wchar_t v9; // ax

  if ( !Destination )
    goto LABEL_3;
  v3 = SizeInWords;
  if ( !SizeInWords )
    goto LABEL_3;
  v6 = Source;
  if ( !Source )
    goto LABEL_7;
  v7 = Destination;
  do
  {
    if ( !*v7 )
      break;
    ++v7;
    --v3;
  }
  while ( v3 );
  if ( !v3 )
  {
LABEL_7:
    *Destination = 0;
LABEL_3:
    v4 = 22;
    *_errno() = 22;
LABEL_4:
    _invalid_parameter_noinfo();
    return v4;
  }
  v8 = (char *)v7 - (char *)Source;
  do
  {
    v9 = *v6;
    *(const wchar_t *)((char *)v6 + v8) = *v6;
    ++v6;
    if ( !v9 )
      break;
    --v3;
  }
  while ( v3 );
  result = 0;
  if ( !v3 )
  {
    *Destination = 0;
    *_errno() = 34;
    v4 = 34;
    goto LABEL_4;
  }
  return result;
}

// ===== _wcslen @ 0x004B77B5..0x004B77D0 =====
size_t __cdecl wcslen(const wchar_t *String)
{
  const wchar_t *v1; // eax

  v1 = String;
  while ( *v1++ )
    ;
  return v1 - String - 1;
}

// ===== _wcscpy_s @ 0x004B77D0..0x004B7833 =====
errno_t __cdecl wcscpy_s(wchar_t *Destination, rsize_t SizeInWords, const wchar_t *Source)
{
  rsize_t v3; // edi
  errno_t v4; // esi
  errno_t result; // eax
  const wchar_t *v6; // eax
  wchar_t v7; // cx

  if ( !Destination )
    goto LABEL_3;
  v3 = SizeInWords;
  if ( !SizeInWords )
    goto LABEL_3;
  v6 = Source;
  if ( !Source )
  {
    *Destination = 0;
LABEL_3:
    v4 = 22;
    *_errno() = 22;
LABEL_4:
    _invalid_parameter_noinfo();
    return v4;
  }
  do
  {
    v7 = *v6;
    *(const wchar_t *)((char *)v6 + (char *)Destination - (char *)Source) = *v6;
    ++v6;
    if ( !v7 )
      break;
    --v3;
  }
  while ( v3 );
  result = 0;
  if ( !v3 )
  {
    *Destination = 0;
    *_errno() = 34;
    v4 = 34;
    goto LABEL_4;
  }
  return result;
}

// ===== __set_error_mode @ 0x004B7833..0x004B7872 =====
int __cdecl _set_error_mode(int Mode)
{
  int result; // eax

  if ( Mode >= 0 )
  {
    if ( Mode <= 2 )
    {
      result = dword_509B68;
      dword_509B68 = Mode;
      return result;
    }
    if ( Mode == 3 )
      return dword_509B68;
  }
  *_errno() = 22;
  _invalid_parameter_noinfo();
  return -1;
}

// ===== __mbsnbcpy_s_l @ 0x004B7872..0x004B7A5A =====
errno_t __cdecl _mbsnbcpy_s_l(
        unsigned __int8 *Dst,
        size_t DstSizeInBytes,
        const unsigned __int8 *Src,
        size_t MaxCount,
        _locale_t Locale)
{
  errno_t result; // eax
  size_t v6; // ebx
  const unsigned __int8 *v7; // edi
  errno_t v8; // esi
  unsigned __int8 *v9; // eax
  size_t v10; // edx
  unsigned __int8 v11; // cl
  unsigned __int8 v12; // cl
  unsigned __int8 *v13; // edi
  unsigned __int8 *j; // ebx
  unsigned __int8 *v15; // edi
  unsigned __int8 *i; // ebx
  unsigned __int8 *k; // ebx
  int *v18; // ecx
  __crt_locale_pointers Localea; // [esp+8h] [ebp-10h] BYREF
  int v20; // [esp+10h] [ebp-8h]
  char v21; // [esp+14h] [ebp-4h]

  if ( MaxCount )
  {
    if ( !Dst )
    {
LABEL_7:
      *_errno() = 22;
      _invalid_parameter_noinfo();
      return 22;
    }
  }
  else if ( !Dst )
  {
    if ( !DstSizeInBytes )
      return 0;
    goto LABEL_7;
  }
  v6 = DstSizeInBytes;
  if ( !DstSizeInBytes )
    goto LABEL_7;
  if ( !MaxCount )
  {
    *Dst = 0;
    return 0;
  }
  v7 = Src;
  if ( !Src )
  {
    *Dst = 0;
    v8 = 22;
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return v8;
  }
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Localea, (struct localeinfo_struct *)Locale);
  if ( !*((_DWORD *)Localea.mbcinfo + 2) )
  {
    result = strncpy_s((char *)Dst, DstSizeInBytes, (const char *)Src, MaxCount);
    goto LABEL_52;
  }
  v9 = Dst;
  v10 = DstSizeInBytes;
  if ( MaxCount == -1 )
  {
    do
    {
      v11 = *v7;
      *v9++ = *v7++;
      if ( !v11 )
        break;
      --v10;
    }
    while ( v10 );
  }
  else
  {
    do
    {
      v12 = *v7;
      *v9++ = *v7++;
      if ( !v12 )
        break;
      if ( !--v10 )
        break;
      --MaxCount;
    }
    while ( MaxCount );
    if ( !MaxCount )
      *v9++ = 0;
  }
  if ( !v10 )
  {
    if ( *v7 && MaxCount != 1 )
    {
LABEL_31:
      if ( MaxCount != -1 )
      {
        *Dst = 0;
        v8 = 34;
        *_errno() = 34;
        _invalid_parameter_noinfo();
        if ( v21 )
          *(_DWORD *)(v20 + 112) &= ~2u;
        return v8;
      }
      if ( v6 > 1 )
      {
        v15 = &Dst[v6 - 2];
        for ( i = v15; i >= Dst; --i )
        {
          if ( !_ismbblead_l(*i, &Localea) )
            break;
        }
        if ( (((_BYTE)v15 - (_BYTE)i) & 1) != 0 )
        {
          *v15 = 0;
LABEL_40:
          if ( v21 )
            *(_DWORD *)(v20 + 112) &= ~2u;
          return 80;
        }
        v6 = DstSizeInBytes;
      }
      Dst[v6 - 1] = 0;
      goto LABEL_40;
    }
    v13 = v9 - 1;
    for ( j = v9 - 1; j >= Dst; --j )
    {
      if ( !_ismbblead_l(*j, &Localea) )
        break;
    }
    if ( (((_BYTE)v13 - (_BYTE)j) & 1) == 0 )
    {
      v6 = DstSizeInBytes;
      goto LABEL_31;
    }
LABEL_51:
    *v13 = 0;
    v18 = _errno();
    result = 42;
    *v18 = 42;
LABEL_52:
    if ( v21 )
      *(_DWORD *)(v20 + 112) &= ~2u;
    return result;
  }
  if ( v9 - Dst >= 2 )
  {
    v13 = v9 - 2;
    for ( k = v9 - 2; k >= Dst; --k )
    {
      if ( !_ismbblead_l(*k, &Localea) )
        break;
    }
    if ( (((_BYTE)v13 - (_BYTE)k) & 1) != 0 )
      goto LABEL_51;
  }
  if ( v21 )
    *(_DWORD *)(v20 + 112) &= ~2u;
  return 0;
}

// ===== __EH_prolog3_catch @ 0x004B7A5A..0x004B7A90 =====
_DWORD *__usercall _EH_prolog3_catch@<eax>(int a1@<eax>)
{
  _DWORD v3[2]; // [esp-8h] [ebp-8h] BYREF
  int retaddr; // [esp+0h] [ebp+0h]

  v3[1] = a1;
  v3[0] = NtCurrentTeb()->NtTib.ExceptionList;
  retaddr = -1;
  return v3;
}

// ===== unknown_libname_3 @ 0x004B7A90..0x004B7AA2 =====
// Microsoft VisualC 2-14/net runtime
BOOL __cdecl unknown_libname_3(int a1)
{
  return a1 != 0;
}

// ===== __global_unwind2 @ 0x004B7AB0..0x004B7AD0 =====
void __cdecl _global_unwind2(PVOID TargetFrame)
{
  RtlUnwind(TargetFrame, &gu_return, 0, 0);
}

// ===== __unwind_handler @ 0x004B7AD0..0x004B7B14 =====
int __cdecl _unwind_handler(int a1, int a2, int a3, _DWORD *a4, int a5)
{
  int v5; // eax

  if ( (*(_DWORD *)(a1 + 4) & 6) != 0 )
  {
    sub_4AB245((void *)(a5 ^ *(_DWORD *)(a5 - 4)));
    _local_unwind2(*(_DWORD *)(v5 + 36), *(_DWORD *)(v5 + 40));
    *a4 = a2;
  }
  return uh_return();
}

// ===== _uh_return @ 0x004B7B14..0x004B7B15 =====
void uh_return()
{
  ;
}

// ===== __local_unwind2 @ 0x004B7B15..0x004B7B99 =====
int __cdecl _local_unwind2(int a1, unsigned int a2)
{
  int result; // eax
  int v3; // ebx
  unsigned int v4; // esi
  int v5; // esi
  int v6; // ecx
  unsigned int v7; // [esp-4h] [ebp-24h]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+0h] [ebp-20h] BYREF
  int (__cdecl *v9)(int, int, int, _DWORD *, int); // [esp+4h] [ebp-1Ch]
  int v10; // [esp+8h] [ebp-18h]
  int v11; // [esp+Ch] [ebp-14h]
  int v12; // [esp+10h] [ebp-10h]

  v11 = a1;
  v10 = -2;
  v9 = _unwind_handler;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v7 = (unsigned int)&ExceptionList ^ dword_4FB734;
  while ( 1 )
  {
    result = a1;
    v3 = *(_DWORD *)(a1 + 8);
    v4 = *(_DWORD *)(a1 + 12);
    if ( v4 == -1 || a2 != -1 && v4 <= a2 )
      break;
    v5 = 3 * v4;
    v10 = *(_DWORD *)(v3 + 4 * v5);
    *(_DWORD *)(a1 + 12) = v10;
    if ( !*(_DWORD *)(v3 + 4 * v5 + 4) )
    {
      _NLG_Notify(257);
      _NLG_Call(v6, v7, ExceptionList, v9, v10, v11, v12);
    }
  }
  return result;
}

// ===== __abnormal_termination @ 0x004B7B99..0x004B7BBC =====
int __cdecl _abnormal_termination()
{
  int result; // eax
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // ecx

  result = 0;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  if ( (int (__cdecl *)(int, int, int, _DWORD *, int))ExceptionList->Handler == _unwind_handler )
    return ExceptionList[1].Next == (struct _EXCEPTION_REGISTRATION_RECORD *)*((_DWORD *)ExceptionList[1].Handler + 3);
  return result;
}

// ===== __NLG_Notify1 @ 0x004B7BBC..0x004B7BC5 =====
void __stdcall _NLG_Notify1(int a1)
{
  JUMPOUT(0x4B7BD0);
}

// ===== __NLG_Notify @ 0x004B7BC5..0x004B7BE4 =====
int __userpurge _NLG_Notify@<eax>(int result@<eax>, int a2@<ebp>, int a3)
{
  dword_4FC660[2] = a3;
  dword_4FC660[1] = result;
  dword_4FC660[3] = a2;
  return result;
}

// ===== __NLG_Call @ 0x004B7BE4..0x004B7BE7 =====
int __usercall _NLG_Call@<eax>(int (*a1)(void)@<eax>)
{
  return a1();
}

// ===== __isdigit_l @ 0x004B7BE7..0x004B7C38 =====
int __cdecl _isdigit_l(int C, _locale_t Locale)
{
  int result; // eax
  __crt_locale_pointers Localea; // [esp+0h] [ebp-10h] BYREF
  int v4; // [esp+8h] [ebp-8h]
  char v5; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Localea, (struct localeinfo_struct *)Locale);
  if ( *((int *)Localea.locinfo + 43) <= 1 )
    result = *(_WORD *)(*((_DWORD *)Localea.locinfo + 50) + 2 * C) & 4;
  else
    result = _isctype_l(C, 4, &Localea);
  if ( v5 )
    *(_DWORD *)(v4 + 112) &= ~2u;
  return result;
}

// ===== _isdigit @ 0x004B7C38..0x004B7C66 =====
int __cdecl isdigit(int C)
{
  if ( dword_509F0C )
    return _isdigit_l(C, 0);
  else
    return off_4FC048[C] & 4;
}

// ===== sub_4B7C66 @ 0x004B7C66..0x004B7D0E =====
int __cdecl sub_4B7C66(int a1, int a2, struct localeinfo_struct *a3)
{
  int v3; // eax
  _BYTE v6[4]; // [esp+Ch] [ebp-28h] BYREF
  _BYTE v7[8]; // [esp+10h] [ebp-24h] BYREF
  int v8; // [esp+18h] [ebp-1Ch]
  char v9; // [esp+1Ch] [ebp-18h]
  int v10; // [esp+20h] [ebp-14h]
  _BYTE v11[12]; // [esp+24h] [ebp-10h] BYREF

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v7, a3);
  v10 = __strgtold12_l(v11, v6, a2, 0, 0, 0, 0, v7);
  v3 = sub_4B8871(v11, a1);
  if ( (v10 & 3) != 0 )
  {
    if ( (v10 & 1) != 0 )
      goto LABEL_8;
    if ( (v10 & 2) != 0 )
      goto LABEL_3;
  }
  else
  {
    if ( v3 == 1 )
    {
LABEL_3:
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
      return 3;
    }
    if ( v3 == 2 )
    {
LABEL_8:
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
      return 4;
    }
  }
  if ( v9 )
    *(_DWORD *)(v8 + 112) &= ~2u;
  return 0;
}

// ===== sub_4B7D0E @ 0x004B7D0E..0x004B7DB6 =====
int __cdecl sub_4B7D0E(int a1, int a2, struct localeinfo_struct *a3)
{
  int v3; // eax
  _BYTE v6[4]; // [esp+Ch] [ebp-28h] BYREF
  _BYTE v7[8]; // [esp+10h] [ebp-24h] BYREF
  int v8; // [esp+18h] [ebp-1Ch]
  char v9; // [esp+1Ch] [ebp-18h]
  int v10; // [esp+20h] [ebp-14h]
  _BYTE v11[12]; // [esp+24h] [ebp-10h] BYREF

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v7, a3);
  v10 = __strgtold12_l(v11, v6, a2, 0, 0, 0, 0, v7);
  v3 = sub_4B8DC2(v11, a1);
  if ( (v10 & 3) != 0 )
  {
    if ( (v10 & 1) != 0 )
      goto LABEL_8;
    if ( (v10 & 2) != 0 )
      goto LABEL_3;
  }
  else
  {
    if ( v3 == 1 )
    {
LABEL_3:
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
      return 3;
    }
    if ( v3 == 2 )
    {
LABEL_8:
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
      return 4;
    }
  }
  if ( v9 )
    *(_DWORD *)(v8 + 112) &= ~2u;
  return 0;
}

// ===== __fptostr @ 0x004B7DB6..0x004B7E69 =====
int __cdecl _fptostr(char *a1, unsigned int a2, int a3, int a4)
{
  int v4; // ecx
  char *v5; // ebx
  int v6; // eax
  int v7; // esi
  int v9; // edx
  char *v10; // eax
  char v11; // cl
  size_t v12; // eax

  v4 = a4;
  v5 = *(char **)(a4 + 12);
  v6 = 0;
  if ( !a1 || !a2 )
  {
    v7 = 22;
    *_errno() = 22;
LABEL_3:
    _invalid_parameter_noinfo();
    return v7;
  }
  v9 = a3;
  *a1 = 0;
  if ( a3 > 0 )
    v6 = a3;
  if ( a2 <= v6 + 1 )
  {
    *_errno() = 34;
    v7 = 34;
    goto LABEL_3;
  }
  *a1 = 48;
  v10 = a1 + 1;
  if ( a3 > 0 )
  {
    do
    {
      v11 = *v5;
      if ( *v5 )
        ++v5;
      else
        v11 = 48;
      *v10++ = v11;
      --v9;
    }
    while ( v9 > 0 );
    v4 = a4;
  }
  *v10 = 0;
  if ( v9 >= 0 && *v5 >= 53 )
  {
    while ( *--v10 == 57 )
      *v10 = 48;
    ++*v10;
  }
  if ( *a1 == 49 )
  {
    ++*(_DWORD *)(v4 + 4);
  }
  else
  {
    v12 = strlen(a1 + 1);
    memcpy(a1, a1 + 1, v12 + 1);
  }
  return 0;
}

// ===== ___dtold @ 0x004B7E69..0x004B7F1C =====
int *__cdecl __dtold(int *a1, int *a2)
{
  int v3; // ebx
  int v4; // eax
  unsigned int v5; // ecx
  int v6; // eax
  __int16 v7; // bx
  __int16 v8; // di
  int *result; // eax
  __int16 v10; // cx
  int v11; // edx
  int v12; // ecx
  unsigned int v13; // [esp+Ch] [ebp-4h]
  __int16 v14; // [esp+1Ch] [ebp+Ch]

  v3 = (*((unsigned __int16 *)a2 + 3) >> 4) & 0x7FF;
  v14 = *((_WORD *)a2 + 3) & 0x8000;
  v4 = a2[1];
  v5 = *a2;
  v6 = v4 & 0xFFFFF;
  v13 = 0x80000000;
  if ( (_WORD)v3 )
  {
    if ( (unsigned __int16)v3 == 2047 )
    {
      v8 = 0x7FFF;
      goto LABEL_10;
    }
    v7 = v3 + 15360;
  }
  else
  {
    if ( !v6 && !v5 )
    {
      result = a1;
      v10 = v14;
      a1[1] = 0;
      *a1 = 0;
      goto LABEL_14;
    }
    v7 = 15361;
    v13 = 0;
  }
  v8 = v7;
LABEL_10:
  v11 = v13 | (v6 << 11) | (v5 >> 21);
  result = a1;
  v12 = v5 << 11;
  while ( 1 )
  {
    *a1 = v12;
    a1[1] = v11;
    if ( v11 < 0 )
      break;
    v11 = ((unsigned int)*a1 >> 31) | (2 * v11);
    v12 = 2 * *a1;
    --v8;
  }
  v10 = v8 | v14;
LABEL_14:
  *((_WORD *)result + 4) = v10;
  return result;
}

// ===== __fltout2 @ 0x004B7F1C..0x004B7FA9 =====
_DWORD *__cdecl _fltout2(int a1, int a2, _DWORD *a3, char *a4, rsize_t SizeInBytes)
{
  _DWORD *v5; // ebx
  int v6; // eax
  char *v7; // esi
  int v9[2]; // [esp+Ch] [ebp-30h] BYREF
  __int16 v10; // [esp+14h] [ebp-28h]
  char *Destination; // [esp+18h] [ebp-24h]
  __int16 v12; // [esp+1Ch] [ebp-20h] BYREF
  char v13; // [esp+1Eh] [ebp-1Eh]
  char Source[24]; // [esp+20h] [ebp-1Ch] BYREF

  v5 = a3;
  Destination = a4;
  __dtold(v9, &a1);
  v6 = _I10_OUTPUT(v9[0], v9[1], v10, 17, 0, &v12);
  v7 = Destination;
  v5[2] = v6;
  *v5 = v13;
  v5[1] = v12;
  if ( strcpy_s(v7, SizeInBytes, Source) )
    _invoke_watson(0, 0, 0, 0, 0);
  v5[3] = v7;
  return v5;
}

// ===== __alldvrm @ 0x004B7FB0..0x004B808F =====
int __stdcall _alldvrm(unsigned __int64 a1, __int64 a2)
{
  int v2; // edi
  int v3; // eax
  unsigned __int64 v4; // rtt
  int v5; // esi
  unsigned __int64 v6; // rcx
  unsigned __int64 v7; // rax
  unsigned __int64 v8; // rax
  int result; // eax

  v2 = 0;
  if ( (a1 & 0x8000000000000000uLL) != 0LL )
  {
    v2 = 1;
    HIDWORD(a1) = -HIDWORD(a1) - ((_DWORD)a1 != 0);
    LODWORD(a1) = -(int)a1;
  }
  v3 = HIDWORD(a2);
  if ( a2 < 0 )
  {
    ++v2;
    v3 = -HIDWORD(a2) - ((_DWORD)a2 != 0);
    HIDWORD(a2) = v3;
    LODWORD(a2) = -(int)a2;
  }
  if ( v3 )
  {
    v6 = __PAIR64__(v3, a2);
    v7 = a1;
    do
    {
      v6 >>= 1;
      v7 >>= 1;
    }
    while ( HIDWORD(v6) );
    v5 = v7 / (unsigned int)v6;
    v8 = (unsigned int)v5 * (unsigned __int64)(unsigned int)a2;
    if ( __CFADD__(HIDWORD(a2) * v5, HIDWORD(v8))
      || (HIDWORD(v8) = (a2 * (unsigned __int64)(unsigned int)v5) >> 32, v8 > a1) )
    {
      --v5;
    }
  }
  else
  {
    LODWORD(v4) = a1;
    HIDWORD(v4) = HIDWORD(a1) % (unsigned int)a2;
    v5 = v4 / (unsigned int)a2;
  }
  result = v5;
  if ( v2 == 1 )
    return -v5;
  return result;
}
