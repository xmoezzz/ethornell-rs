#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4BC490 @ 0x004BC490..0x004BC763 =====
int __cdecl sub_4BC490(int a1, __int64 a2)
{
  _DWORD *v2; // esi
  int v3; // ebx
  int result; // eax
  _DWORD *v5; // edi
  int v6; // eax
  int v7; // ebp
  int v8; // ecx
  __int64 v9; // rax
  __int64 v10; // rax
  int v11; // ebx
  int v12; // edx
  _DWORD *v13; // eax
  int v14; // edi
  int v15; // ecx
  bool v16; // cf
  int v17; // edx
  int v18; // eax
  int v19; // edx
  int v20; // ecx
  _DWORD *v21; // ebx
  unsigned int v22; // eax
  unsigned int i; // ecx
  signed __int64 v24; // kr00_8
  int v25; // ebx
  int v26; // edx
  _BYTE v27[16]; // [esp+10h] [ebp-30h] BYREF
  _BYTE v28[16]; // [esp+20h] [ebp-20h] BYREF
  __int64 v29; // [esp+30h] [ebp-10h]

  v2 = (_DWORD *)a1;
  v3 = 0;
  result = sub_4BBED0(a1, a2);
  if ( result >= 0 )
  {
    sub_4BC770(v2);
    while ( 1 )
    {
      while ( 1 )
      {
        v5 = v2 + 30;
        v6 = sub_4D4A40(v2 + 30, v28);
        if ( v6 > 0 )
          break;
        if ( v6 < 0 && v6 != -3 )
          goto LABEL_30;
        sub_4BB270((int)v2, (int)v27, -1LL);
        if ( v17 < 0 )
          goto LABEL_30;
        if ( v2[23] != sub_4D40E0(v27) )
          sub_4BBE90(v2);
        if ( (int)v2[22] < 3 )
        {
          v18 = sub_4D40E0(v27);
          v19 = v2[13];
          v20 = 0;
          v2[23] = v18;
          if ( v19 > 0 )
          {
            v21 = (_DWORD *)v2[16];
            do
            {
              if ( *v21 == v18 )
                break;
              ++v20;
              ++v21;
            }
            while ( v20 < v19 );
          }
          if ( v20 == v19 )
            return -137;
          v5 = v2 + 30;
          v2[24] = v20;
          sub_4D48D0(v2 + 30, v18);
          v2[22] = 3;
          sub_4BC770(v2);
          v3 = 0;
        }
        sub_4D4570(v5, v27);
      }
      v7 = sub_4C8A00(v2[18] + 32 * v2[24], v28);
      if ( v7 < 0 )
        v7 = 0;
      if ( v3 )
      {
        v8 = v2[20];
        v9 = (v3 + v7) >> 2;
        v2[20] = v9 + v8;
        v2[21] += HIDWORD(v9) + __CFADD__((_DWORD)v9, v8);
      }
      v10 = *((_QWORD *)v2 + 10) + ((v7 + sub_4C7460(v2[18], 1)) >> 2);
      if ( v10 >= a2 )
        break;
      sub_4D48F0(v2 + 30, 0);
      sub_4C8920(v2 + 148, v28);
      sub_4C8320(v2 + 120, v2 + 148);
      if ( v29 >= 0 )
      {
        v11 = v2[24];
        v12 = v2[17];
        *((_QWORD *)v2 + 10) = v29 - *(_QWORD *)(16 * v11 + v12);
        if ( (int)v2[21] < 0 )
        {
          v2[20] = 0;
          v2[21] = 0;
        }
        if ( v11 > 0 )
        {
          v13 = (_DWORD *)(v12 + 8);
          do
          {
            v14 = v2[20];
            v15 = v13[1];
            v16 = __CFADD__(*v13, v14);
            v2[20] = *v13 + v14;
            v13 += 4;
            --v11;
            v2[21] += v15 + v16;
          }
          while ( v11 );
        }
      }
      v3 = v7;
    }
LABEL_30:
    v22 = v2[21];
    for ( i = v2[20]; __SPAIR64__(v22, i) < a2; i = v2[20] )
    {
      v24 = a2 - __PAIR64__(v22, i);
      v25 = sub_4C8730(v2 + 120, &a1);
      if ( v25 > v24 )
        v25 = v24;
      sub_4C8790(v2 + 120, v25);
      *((_QWORD *)v2 + 10) += v25;
      if ( v25 < v24 && (int)sub_4BC7C0(v2, 0, 1) <= 0 )
      {
        v2[20] = sub_4BBB50(v2, -1);
        v2[21] = v26;
      }
      v22 = v2[21];
    }
    return 0;
  }
  return result;
}

// ===== sub_4BC770 @ 0x004BC770..0x004BC7BE =====
int __cdecl sub_4BC770(_DWORD *a1)
{
  int result; // eax

  if ( a1[22] == 3 )
  {
    if ( a1[1] )
      sub_4C82F0(a1 + 120, a1[18] + 32 * a1[24]);
    else
      sub_4C82F0(a1 + 120, a1[18]);
    result = sub_4C7B90(a1 + 120, a1 + 148);
    a1[22] = 4;
  }
  return result;
}

// ===== sub_4BC7C0 @ 0x004BC7C0..0x004BCA7A =====
int __cdecl sub_4BC7C0(int a1, char *a2, int a3)
{
  char *v3; // edi
  int v4; // eax
  unsigned int v5; // ebx
  int v6; // ebp
  int v7; // edx
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // edx
  int v12; // ecx
  _DWORD *v13; // edi
  int result; // eax
  int v15; // eax
  int v16; // edi
  unsigned __int64 v17; // kr00_8
  unsigned __int64 v18; // kr08_8
  _QWORD *v19; // eax
  unsigned __int64 v20; // kr10_8
  _BYTE v21[4]; // [esp+10h] [ebp-30h] BYREF
  int v22; // [esp+14h] [ebp-2Ch]
  char v23; // [esp+20h] [ebp-20h] BYREF

  while ( *(_DWORD *)(a1 + 88) != 4 )
  {
LABEL_8:
    if ( *(int *)(a1 + 88) >= 2 )
    {
      if ( !a3 )
        return 0;
      sub_4BB270(a1, (int)v21, -1LL);
      if ( v7 < 0 )
        return -2;
      v8 = *(_DWORD *)(a1 + 88);
      *(double *)(a1 + 104) = (double)(8 * v22) + *(double *)(a1 + 104);
      if ( v8 == 4 && *(_DWORD *)(a1 + 92) != sub_4D40E0(v21) )
      {
        sub_4BBE90((_DWORD *)a1);
        if ( !*(_DWORD *)(a1 + 4) )
        {
          sub_4C74B0(*(_DWORD *)(a1 + 72));
          sub_4C73F0(*(_DWORD *)(a1 + 76));
        }
      }
    }
    v9 = *(_DWORD *)(a1 + 88);
    if ( v9 != 4 )
    {
      if ( v9 < 3 )
      {
        if ( *(_DWORD *)(a1 + 4) )
        {
          v10 = sub_4D40E0(v21);
          v11 = *(_DWORD *)(a1 + 52);
          v12 = 0;
          *(_DWORD *)(a1 + 92) = v10;
          if ( v11 > 0 )
          {
            v13 = *(_DWORD **)(a1 + 64);
            do
            {
              if ( *v13 == v10 )
                break;
              ++v12;
              ++v13;
            }
            while ( v12 < v11 );
          }
          if ( v12 == v11 )
            return -137;
          *(_DWORD *)(a1 + 96) = v12;
          sub_4D48D0(a1 + 120, v10);
          *(_DWORD *)(a1 + 88) = 3;
        }
        else
        {
          result = sub_4BB120(a1, *(_DWORD *)(a1 + 72), *(_DWORD *)(a1 + 76), (_DWORD *)(a1 + 92), v21);
          if ( result )
            return result;
          ++*(_DWORD *)(a1 + 96);
        }
      }
      sub_4BC770((_DWORD *)a1);
    }
    sub_4D4570(a1 + 120, v21);
  }
  do
  {
    v3 = a2;
    if ( !a2 )
      v3 = &v23;
    v4 = sub_4D48F0(a1 + 120, v3);
    a2 = 0;
    if ( v4 == -1 )
      return -3;
    if ( v4 <= 0 )
      goto LABEL_8;
    v5 = *((_DWORD *)v3 + 4);
    v6 = *((_DWORD *)v3 + 5);
  }
  while ( sub_4C87C0(a1 + 592, v3) );
  if ( sub_4C8730(a1 + 480, 0) )
    return -129;
  sub_4C8320(a1 + 480, a1 + 592);
  *(double *)(a1 + 112) = (double)(int)sub_4C8730(a1 + 480, 0) + *(double *)(a1 + 112);
  *(double *)(a1 + 104) = (double)(8 * *((_DWORD *)v3 + 1)) + *(double *)(a1 + 104);
  if ( (v6 & v5) != 0xFFFFFFFF && !*((_DWORD *)v3 + 3) )
  {
    v15 = *(_DWORD *)(a1 + 4);
    if ( v15 )
      v16 = *(_DWORD *)(a1 + 96);
    else
      v16 = 0;
    if ( v15 && v16 > 0 )
    {
      v17 = __PAIR64__(v6, v5) - *(_QWORD *)(*(_DWORD *)(a1 + 68) + 16 * v16);
      v6 = HIDWORD(v17);
      v5 = v17;
    }
    if ( v6 < 0 )
    {
      v5 = 0;
      v6 = 0;
    }
    v18 = __PAIR64__(v6, v5) - (int)sub_4C8730(a1 + 480, 0);
    if ( v16 > 0 )
    {
      v19 = (_QWORD *)(*(_DWORD *)(a1 + 68) + 8);
      do
      {
        v20 = *v19 + v18;
        v19 += 2;
        --v16;
        v18 = v20;
      }
      while ( v16 );
    }
    *(_QWORD *)(a1 + 80) = v18;
  }
  return 1;
}

// ===== sub_4BCA80 @ 0x004BCA80..0x004BCABA =====
int __cdecl sub_4BCA80(_DWORD *a1, int a2)
{
  if ( !a1[1] )
    return a1[18];
  if ( a2 < 0 )
  {
    if ( (int)a1[22] >= 3 )
      return a1[18] + 32 * a1[24];
    return a1[18];
  }
  if ( a2 < a1[13] )
    return 32 * a2 + a1[18];
  else
    return 0;
}

// ===== sub_4BCAC0 @ 0x004BCAC0..0x004BCE1E =====
int __cdecl sub_4BCAC0(int a1, int a2, int a3, int a4, int a5, int a6, _DWORD *a7)
{
  int result; // eax
  int v8; // esi
  int v9; // ebx
  int v10; // ecx
  int v11; // edi
  _BYTE *v12; // esi
  int v13; // eax
  int v14; // ecx
  int v16; // edx
  int v17; // edi
  _WORD *v18; // edx
  float *v19; // ecx
  int v20; // esi
  int v21; // ebx
  int v22; // eax
  int v23; // eax
  _WORD *v24; // ecx
  float *v25; // edx
  int v26; // esi
  int v27; // ebx
  int v28; // eax
  int v29; // edi
  _BYTE *v30; // esi
  int i; // ecx
  int v32; // eax
  __int16 v33; // ax
  _BYTE *v34; // esi
  _BYTE *v35; // edx
  int j; // ecx
  int v37; // eax
  __int16 v38; // ax
  _BYTE *v39; // edx
  int v40; // [esp+Ch] [ebp-18h]
  int v41; // [esp+14h] [ebp-10h]
  int v42; // [esp+18h] [ebp-Ch]
  int v43; // [esp+1Ch] [ebp-8h]
  int v44; // [esp+20h] [ebp-4h] BYREF
  int v45; // [esp+3Ch] [ebp+18h]
  int v46; // [esp+3Ch] [ebp+18h]
  __int16 v47; // [esp+40h] [ebp+1Ch]

  v41 = sub_4BCE20();
  if ( *(int *)(a1 + 88) < 2 )
    return -131;
  while ( 1 )
  {
    if ( *(int *)(a1 + 88) >= 3 )
    {
      result = sub_4C8730(a1 + 480, &v44);
      v8 = result;
      v43 = result;
      if ( result )
        break;
    }
    result = sub_4BC7C0(a1, 0, 1);
    if ( result == -2 )
      return 0;
    if ( result <= 0 )
      return result;
  }
  if ( result > 0 )
  {
    v9 = *(_DWORD *)(sub_4BCA80((_DWORD *)a1, -1) + 4);
    v10 = a5 * v9;
    v42 = v9;
    v40 = a5 * v9;
    if ( v8 > a3 / (a5 * v9) )
    {
      v43 = a3 / v10;
      v8 = a3 / v10;
    }
    if ( v8 > 0 )
    {
      if ( a5 == 1 )
      {
        v11 = 0;
        v12 = (_BYTE *)a2;
        do
        {
          v13 = 0;
          if ( v9 > 0 )
          {
            do
            {
              v14 = (int)(*(float *)(*(_DWORD *)(v44 + 4 * v13) + 4 * v11) * 128.0);
              if ( v14 <= 127 )
              {
                if ( v14 < -128 )
                  LOBYTE(v14) = 0x80;
              }
              else
              {
                LOBYTE(v14) = 127;
              }
              *v12++ = v14 + (a6 != 0 ? 0 : 0x80);
              ++v13;
            }
            while ( v13 < v42 );
            v9 = v42;
          }
          ++v11;
        }
        while ( v11 < v43 );
        v8 = v43;
      }
      else
      {
        v16 = a6 != 0 ? 0 : 0x8000;
        v47 = v16;
        if ( v41 == a4 )
        {
          if ( a6 )
          {
            v17 = 0;
            if ( v9 > 0 )
            {
              v18 = (_WORD *)a2;
              do
              {
                v19 = *(float **)(v44 + 4 * v17);
                if ( v8 > 0 )
                {
                  v20 = v43;
                  v21 = 2 * v9;
                  do
                  {
                    v22 = (int)(*v19 * 32768.0);
                    if ( v22 <= 0x7FFF )
                    {
                      if ( v22 < -32768 )
                        LOWORD(v22) = 0x8000;
                    }
                    else
                    {
                      LOWORD(v22) = 0x7FFF;
                    }
                    *v18 = v22;
                    v18 = (_WORD *)((char *)v18 + v21);
                    ++v19;
                    --v20;
                  }
                  while ( v20 );
                  v9 = v42;
                  v8 = v43;
                }
                ++v17;
                v18 = (_WORD *)(a2 + 2);
                a2 += 2;
              }
              while ( v17 < v9 );
            }
          }
          else
          {
            v23 = 0;
            v45 = 0;
            if ( v9 > 0 )
            {
              v24 = (_WORD *)a2;
              do
              {
                v25 = *(float **)(v44 + 4 * v23);
                if ( v8 > 0 )
                {
                  v26 = v43;
                  v27 = 2 * v9;
                  do
                  {
                    v28 = (int)(*v25 * 32768.0);
                    if ( v28 <= 0x7FFF )
                    {
                      if ( v28 < -32768 )
                        LOWORD(v28) = 0x8000;
                    }
                    else
                    {
                      LOWORD(v28) = 0x7FFF;
                    }
                    ++v25;
                    *v24 = v28 + v47;
                    v24 = (_WORD *)((char *)v24 + v27);
                    --v26;
                  }
                  while ( v26 );
                  v23 = v45;
                  v9 = v42;
                  v8 = v43;
                }
                ++v23;
                v24 = (_WORD *)(a2 + 2);
                v45 = v23;
                a2 += 2;
              }
              while ( v23 < v9 );
            }
          }
        }
        else
        {
          v29 = 0;
          if ( a4 )
          {
            v30 = (_BYTE *)a2;
            do
            {
              for ( i = 0; i < v9; ++i )
              {
                v32 = (int)(*(float *)(*(_DWORD *)(v44 + 4 * i) + 4 * v29) * 32768.0);
                if ( v32 <= 0x7FFF )
                {
                  if ( v32 < -32768 )
                    LOWORD(v32) = 0x8000;
                }
                else
                {
                  LOWORD(v32) = 0x7FFF;
                }
                v33 = v16 + v32;
                *v30 = HIBYTE(v33);
                v34 = v30 + 1;
                LOWORD(v16) = v47;
                *v34 = v33;
                v30 = v34 + 1;
              }
              ++v29;
            }
            while ( v29 < v43 );
            v8 = v43;
          }
          else
          {
            v46 = 0;
            v35 = (_BYTE *)a2;
            do
            {
              for ( j = 0; j < v9; ++j )
              {
                v37 = (int)(*(float *)(*(_DWORD *)(v44 + 4 * j) + 4 * v46) * 32768.0);
                if ( v37 <= 0x7FFF )
                {
                  if ( v37 < -32768 )
                    LOWORD(v37) = 0x8000;
                }
                else
                {
                  LOWORD(v37) = 0x7FFF;
                }
                v38 = v47 + v37;
                *v35 = v38;
                v39 = v35 + 1;
                *v39 = HIBYTE(v38);
                v35 = v39 + 1;
              }
              ++v46;
            }
            while ( v46 < v8 );
          }
        }
      }
      sub_4C8790(a1 + 480, v8);
      *(_QWORD *)(a1 + 80) += v8;
      if ( a7 )
        *a7 = *(_DWORD *)(a1 + 96);
      return v8 * v40;
    }
    else
    {
      return -131;
    }
  }
  return result;
}

// ===== sub_4BCE20 @ 0x004BCE20..0x004BCE23 =====
int sub_4BCE20()
{
  return 0;
}

// ===== sub_4BCE30 @ 0x004BCE30..0x004BCE35 =====
// attributes: thunk
void __thiscall sub_4BCE30(void **this)
{
  sub_4AC3AD(this);
}

// ===== sub_4BCE35 @ 0x004BCE35..0x004BCE52 =====
std::exception *__thiscall sub_4BCE35(std::exception *this, struct exception *a2)
{
  std::exception::exception(this, a2);
  *(_DWORD *)this = &std::logic_error::`vftable';
  return this;
}

// ===== ?_Xlength_error@std@@YAXPBD@Z @ 0x004BCE52..0x004BCE81 =====
void __cdecl __noreturn std::_Xlength_error(char *a1)
{
  _DWORD pExceptionObject[3]; // [esp+0h] [ebp-Ch] BYREF

  std::exception::exception((std::exception *)pExceptionObject, &a1);
  pExceptionObject[0] = &std::length_error::`vftable';
  _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI3_AVlength_error_std__);
}

// ===== sub_4BCE82 @ 0x004BCE82..0x004BCE9F =====
std::exception *__thiscall sub_4BCE82(std::exception *this, struct exception *a2)
{
  std::exception::exception(this, a2);
  *(_DWORD *)this = &std::length_error::`vftable';
  return this;
}

// ===== sub_4BCE9F @ 0x004BCE9F..0x004BCEC0 =====
void **__thiscall sub_4BCE9F(void **this, char a2)
{
  sub_4AC3AD(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4BCEC0 @ 0x004BCEC0..0x004BCED4 =====
void *__thiscall sub_4BCEC0(void *this, int a2)
{
  InterlockedIncrement(&Addend);
  return this;
}

// ===== sub_4BCEE0 @ 0x004BCEE0..0x004BCF0A =====
HMODULE sub_4BCEE0()
{
  HMODULE result; // eax

  result = (HMODULE)InterlockedDecrement(&Addend);
  if ( !result )
  {
    result = dword_50A828;
    if ( dword_50A828 )
    {
      result = (HMODULE)FreeLibrary(dword_50A828);
      dword_50A828 = 0;
    }
  }
  return result;
}

// ===== sub_4BCF10 @ 0x004BCF10..0x004BCF2A =====
HMODULE sub_4BCF10()
{
  HMODULE result; // eax

  result = dword_50A828;
  if ( !dword_50A828 )
  {
    result = LoadLibraryA("OleAut32.dll");
    dword_50A828 = result;
  }
  return result;
}

// ===== sub_4BCF30 @ 0x004BCF30..0x004BCF56 =====
int __stdcall sub_4BCF30(int a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  *a2 = a1;
  (*(void (__stdcall **)(int))(*(_DWORD *)a1 + 4))(a1);
  return 0;
}

// ===== sub_4BCF60 @ 0x004BCF60..0x004BCF8E =====
_DWORD *__thiscall sub_4BCF60(_DWORD *this, int a2, _DWORD *a3)
{
  _DWORD *v4; // eax

  sub_4BCEC0(this + 1, a2);
  v4 = a3;
  if ( !a3 )
    v4 = this;
  this[1] = v4;
  this[2] = 0;
  return this;
}

// ===== sub_4BCF90 @ 0x004BCF90..0x004BCFDB =====
int __stdcall sub_4BCF90(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  if ( sub_445510(a2, dword_4DC2E8) )
  {
    sub_4BCF30(a1, a3);
    return 0;
  }
  else
  {
    *a3 = 0;
    return -2147467262;
  }
}

// ===== sub_4BCFE0 @ 0x004BCFE0..0x004BD002 =====
unsigned int __stdcall sub_4BCFE0(int a1)
{
  unsigned int result; // eax

  InterlockedIncrement((volatile LONG *)(a1 + 8));
  result = *(_DWORD *)(a1 + 8);
  if ( result <= 1 )
    return 1;
  return result;
}

// ===== sub_4BD010 @ 0x004BD010..0x004BD04F =====
int __stdcall sub_4BD010(volatile LONG *a1)
{
  int *v1; // esi
  int result; // eax

  v1 = (int *)(a1 + 2);
  if ( InterlockedDecrement(a1 + 2) )
  {
    result = *v1;
    if ( (unsigned int)*v1 <= 1 )
      return 1;
  }
  else
  {
    ++*v1;
    (*(void (__thiscall **)(volatile LONG *, int))(*a1 + 12))(a1, 1);
    return 0;
  }
  return result;
}

// ===== sub_4BD060 @ 0x004BD060..0x004BD07F =====
HRESULT __stdcall sub_4BD060(LPVOID *ppv)
{
  return CoCreateInstance(&stru_4DB874, 0, 1u, &stru_4DB974, ppv);
}

// ===== sub_4BD080 @ 0x004BD080..0x004BD14F =====
int __stdcall sub_4BD080(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB944) )
  {
LABEL_2:
    if ( a1 )
      return sub_4BCF30(a1 + 12, a3);
    return sub_4BCF30(0, a3);
  }
  if ( sub_445510(a2, dword_4DB934) )
  {
    if ( a1 )
      return sub_4BCF30(a1 + 12, a3);
    return sub_4BCF30(0, a3);
  }
  if ( sub_445510(a2, dword_4DC318) )
    goto LABEL_2;
  if ( !sub_445510(a2, dword_4DB9A4) )
    return sub_4BCF90(a1, a2, a3);
  if ( !a1 )
    return sub_4BCF30(0, a3);
  return sub_4BCF30(a1 + 16, a3);
}

// ===== sub_4BD150 @ 0x004BD150..0x004BD1C1 =====
HMODULE __thiscall sub_4BD150(int this)
{
  int v2; // eax

  operator delete(*(void **)(this + 60));
  v2 = *(_DWORD *)(this + 24);
  if ( v2 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 8))(*(_DWORD *)(this + 24));
    *(_DWORD *)(this + 24) = 0;
  }
  return sub_4BCEE0();
}

// ===== sub_4BD1D0 @ 0x004BD1D0..0x004BD203 =====
int __stdcall sub_4BD1D0(_DWORD *a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  *a2 = a1[7];
  a2[1] = a1[8];
  a2[2] = a1[9];
  a2[3] = a1[10];
  return 0;
}

// ===== sub_4BD210 @ 0x004BD210..0x004BD231 =====
int __stdcall sub_4BD210(int a1, int a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  *a3 = *(_DWORD *)(a1 + 8);
  return 0;
}

// ===== sub_4BD240 @ 0x004BD240..0x004BD2BA =====
int __stdcall sub_4BD240(int a1, int a2)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // eax

  v2 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 44);
  EnterCriticalSection(v2);
  if ( a2 )
    (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 4))(a2);
  v3 = *(_DWORD *)(a1 + 12);
  if ( v3 )
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v3 + 8))(*(_DWORD *)(a1 + 12));
  *(_DWORD *)(a1 + 12) = a2;
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4BD2C0 @ 0x004BD2C0..0x004BD34F =====
int __stdcall sub_4BD2C0(int a1, _DWORD *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi

  if ( !a2 )
    return -2147467261;
  v3 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 44);
  EnterCriticalSection(v3);
  if ( *(_DWORD *)(a1 + 12) )
    (*(void (__stdcall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 4))(*(_DWORD *)(a1 + 12));
  *a2 = *(_DWORD *)(a1 + 12);
  LeaveCriticalSection(v3);
  return 0;
}

// ===== sub_4BD350 @ 0x004BD350..0x004BD3FE =====
int __stdcall sub_4BD350(int a1)
{
  int v1; // esi
  int v2; // edi
  int v3; // ebx
  _DWORD *v4; // eax
  int v5; // eax
  struct _RTL_CRITICAL_SECTION *lpCriticalSection; // [esp+10h] [ebp-14h]
  int v8; // [esp+14h] [ebp-10h]

  v1 = a1;
  lpCriticalSection = *(struct _RTL_CRITICAL_SECTION **)(a1 + 44);
  EnterCriticalSection(lpCriticalSection);
  v2 = 0;
  v8 = 0;
  if ( *(_DWORD *)(a1 + 8) )
  {
    v3 = (*(int (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 24))(a1 - 12);
    if ( v3 > 0 )
    {
      do
      {
        v4 = (_DWORD *)(*(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 28))(a1 - 12, v2);
        if ( v4[6] )
        {
          v5 = (*(int (__thiscall **)(_DWORD *))(*v4 + 24))(v4);
          if ( v5 < 0 && v8 >= 0 )
            v8 = v5;
        }
        ++v2;
      }
      while ( v2 < v3 );
    }
    v1 = a1;
  }
  *(_DWORD *)(v1 + 8) = 0;
  LeaveCriticalSection(lpCriticalSection);
  return v8;
}

// ===== sub_4BD400 @ 0x004BD400..0x004BD4C9 =====
int __stdcall sub_4BD400(int a1)
{
  int v1; // esi
  int v2; // edi
  int v3; // ebx
  _DWORD *v4; // eax
  int v5; // ebx
  int v7; // [esp+10h] [ebp-14h]
  struct _RTL_CRITICAL_SECTION *lpCriticalSection; // [esp+14h] [ebp-10h]

  v1 = a1;
  lpCriticalSection = *(struct _RTL_CRITICAL_SECTION **)(a1 + 44);
  EnterCriticalSection(lpCriticalSection);
  v2 = 0;
  if ( *(_DWORD *)(a1 + 8) )
    goto LABEL_8;
  v3 = (*(int (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 24))(a1 - 12);
  v7 = v3;
  if ( v3 <= 0 )
  {
LABEL_7:
    v1 = a1;
LABEL_8:
    *(_DWORD *)(v1 + 8) = 1;
    LeaveCriticalSection(lpCriticalSection);
    return 0;
  }
  while ( 1 )
  {
    v4 = (_DWORD *)(*(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 28))(a1 - 12, v2);
    if ( v4[6] )
      break;
LABEL_6:
    if ( ++v2 >= v3 )
      goto LABEL_7;
  }
  v5 = (*(int (__thiscall **)(_DWORD *))(*v4 + 20))(v4);
  if ( v5 >= 0 )
  {
    v3 = v7;
    goto LABEL_6;
  }
  LeaveCriticalSection(lpCriticalSection);
  return v5;
}

// ===== sub_4BD4D0 @ 0x004BD4D0..0x004BD5EC =====
int __stdcall sub_4BD4D0(LPCRITICAL_SECTION lpCriticalSection, ULONG_PTR a2, struct _RTL_CRITICAL_SECTION_DEBUG *a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // edi
  int v5; // ebx
  int v6; // ebx
  _DWORD *v7; // eax
  int v9; // [esp+10h] [ebp-14h]
  int v10; // [esp+14h] [ebp-10h]
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+2Ch] [ebp+8h]

  lpCriticalSectiona = (struct _RTL_CRITICAL_SECTION *)lpCriticalSection[1].SpinCount;
  v4 = lpCriticalSectiona;
  EnterCriticalSection(lpCriticalSectiona);
  lpCriticalSection->SpinCount = a2;
  lpCriticalSection[1].DebugInfo = a3;
  if ( lpCriticalSection->RecursionCount
    || (v5 = ((int (__stdcall *)(LPCRITICAL_SECTION))lpCriticalSection->DebugInfo->ContentionCount)(lpCriticalSection),
        v5 >= 0) )
  {
    if ( lpCriticalSection->RecursionCount != 2 )
    {
      v6 = 0;
      v9 = (*((int (__thiscall **)(HANDLE *))lpCriticalSection[-1].OwningThread + 6))(&lpCriticalSection[-1].OwningThread);
      v10 = 0;
      if ( v9 > 0 )
      {
        do
        {
          v7 = (_DWORD *)(*((int (__thiscall **)(HANDLE *, int))lpCriticalSection[-1].OwningThread + 7))(
                           &lpCriticalSection[-1].OwningThread,
                           v6);
          if ( v7[6] )
          {
            v5 = (*(int (__thiscall **)(_DWORD *, ULONG_PTR, struct _RTL_CRITICAL_SECTION_DEBUG *))(*v7 + 28))(
                   v7,
                   a2,
                   a3);
            if ( v5 < 0 )
              goto LABEL_11;
            v6 = v10;
          }
          v10 = ++v6;
        }
        while ( v6 < v9 );
      }
      v4 = lpCriticalSectiona;
    }
    lpCriticalSection->RecursionCount = 2;
    LeaveCriticalSection(v4);
    return 0;
  }
  else
  {
LABEL_11:
    LeaveCriticalSection(lpCriticalSectiona);
    return v5;
  }
}

// ===== sub_4BD5F0 @ 0x004BD5F0..0x004BD62D =====
int __thiscall sub_4BD5F0(_DWORD *this, unsigned int *a2)
{
  int result; // eax
  unsigned int v4; // eax
  bool v5; // cf

  if ( !this[6] )
    return -2147220973;
  result = (*(int (__stdcall **)(_DWORD, unsigned int *))(*(_DWORD *)this[6] + 12))(this[6], a2);
  if ( result >= 0 )
  {
    v4 = this[8];
    v5 = *a2 < v4;
    *a2 -= v4;
    a2[1] -= v5 + this[9];
    return 0;
  }
  return result;
}

// ===== sub_4BD630 @ 0x004BD630..0x004BD721 =====
int __stdcall sub_4BD630(LPCRITICAL_SECTION lpCriticalSection, int a2, _DWORD *a3)
{
  int v3; // edi
  struct _RTL_CRITICAL_SECTION *v6; // ebx
  int v7; // ebx
  int v8; // [esp+10h] [ebp-10h]
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+28h] [ebp+8h]

  v3 = 0;
  if ( !a3 )
    return -2147467261;
  lpCriticalSectiona = (struct _RTL_CRITICAL_SECTION *)lpCriticalSection[1].SpinCount;
  v6 = lpCriticalSectiona;
  EnterCriticalSection(lpCriticalSectiona);
  v8 = (*((int (__thiscall **)(HANDLE *))lpCriticalSection[-1].OwningThread + 6))(&lpCriticalSection[-1].OwningThread);
  if ( v8 <= 0 )
  {
LABEL_7:
    *a3 = 0;
    LeaveCriticalSection(v6);
    return -2147220970;
  }
  else
  {
    while ( 1 )
    {
      v7 = (*((int (__thiscall **)(HANDLE *, int))lpCriticalSection[-1].OwningThread + 7))(
             &lpCriticalSection[-1].OwningThread,
             v3);
      if ( !sub_4C3470(*(_DWORD *)(v7 + 20), a2) )
        break;
      if ( ++v3 >= v8 )
      {
        v6 = lpCriticalSectiona;
        goto LABEL_7;
      }
    }
    *a3 = v7 + 12;
    (*(void (__stdcall **)(int))(*(_DWORD *)(v7 + 12) + 4))(v7 + 12);
    LeaveCriticalSection(lpCriticalSectiona);
    return 0;
  }
}

// ===== sub_4BD730 @ 0x004BD730..0x004BD785 =====
int __stdcall sub_4BD730(int a1, int a2)
{
  int v3; // eax

  if ( !a2 )
    return -2147467261;
  v3 = *(_DWORD *)(a1 + 48);
  if ( v3 )
    sub_4C3430(a2, v3, 128);
  else
    *(_WORD *)a2 = 0;
  *(_DWORD *)(a2 + 256) = *(_DWORD *)(a1 + 52);
  if ( *(_DWORD *)(a1 + 52) )
    (*(void (__stdcall **)(_DWORD))(**(_DWORD **)(a1 + 52) + 4))(*(_DWORD *)(a1 + 52));
  return 0;
}

// ===== sub_4BD790 @ 0x004BD790..0x004BD86B =====
int __stdcall sub_4BD790(
        LPCRITICAL_SECTION lpCriticalSection,
        int (__stdcall ***a2)(_DWORD, void *, LONG *),
        void *Src)
{
  int v4; // esi
  struct _RTL_CRITICAL_SECTION_DEBUG *v5; // eax
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+24h] [ebp+8h]

  lpCriticalSectiona = (struct _RTL_CRITICAL_SECTION *)lpCriticalSection[1].SpinCount;
  EnterCriticalSection(lpCriticalSectiona);
  lpCriticalSection[2].LockCount = (LONG)a2;
  if ( a2 )
  {
    if ( (**a2)(a2, &unk_4DB9E4, &lpCriticalSection[2].RecursionCount) >= 0 )
      (*(void (__stdcall **)(LONG))(*(_DWORD *)lpCriticalSection[2].RecursionCount + 8))(lpCriticalSection[2].RecursionCount);
  }
  else
  {
    lpCriticalSection[2].RecursionCount = 0;
  }
  if ( lpCriticalSection[2].DebugInfo )
  {
    operator delete(lpCriticalSection[2].DebugInfo);
    lpCriticalSection[2].DebugInfo = 0;
  }
  if ( Src )
  {
    v4 = sub_4C34C0(Src);
    v5 = (struct _RTL_CRITICAL_SECTION_DEBUG *)operator new(2 * (v4 + 1));
    lpCriticalSection[2].DebugInfo = v5;
    if ( v5 )
      memcpy_0(v5, Src, 2 * v4 + 2);
  }
  LeaveCriticalSection(lpCriticalSectiona);
  return 0;
}

// ===== sub_4BD870 @ 0x004BD870..0x004BD878 =====
int __stdcall sub_4BD870(int a1, int a2)
{
  return -2147467263;
}

// ===== sub_4BD880 @ 0x004BD880..0x004BD8B7 =====
int __thiscall sub_4BD880(_DWORD *this, int a2, int a3, _DWORD *a4)
{
  int v4; // eax
  _DWORD *v5; // ecx

  v4 = this[17];
  if ( !v4 )
    return -2147467263;
  if ( a2 == 1 )
    v5 = this + 3;
  else
    v5 = a4;
  return (*(int (__stdcall **)(int, int, int, _DWORD *))(*(_DWORD *)v4 + 12))(v4, a2, a3, v5);
}

// ===== sub_4BD8C0 @ 0x004BD8C0..0x004BD8C4 =====
int __thiscall sub_4BD8C0(_DWORD *this)
{
  return this[18];
}

// ===== sub_4BD8D0 @ 0x004BD8D0..0x004BD934 =====
int __thiscall sub_4BD8D0(_DWORD *this)
{
  *this = &CEnumPins::`vftable';
  (*(void (__stdcall **)(int))(*(_DWORD *)(this[3] + 12) + 8))(this[3] + 12);
  return sub_4C3CE0();
}

// ===== sub_4BD940 @ 0x004BD940..0x004BD99A =====
int __stdcall sub_4BD940(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  if ( sub_445510(a2, dword_4DB914) || sub_445510(a2, dword_4DC2E8) )
    return sub_4BCF30(a1, a3);
  *a3 = 0;
  return -2147467262;
}

// ===== sub_4BD9A0 @ 0x004BD9A0..0x004BD9B4 =====
LONG __stdcall sub_4BD9A0(int a1)
{
  return InterlockedIncrement((volatile LONG *)(a1 + 20));
}

// ===== sub_4BD9C0 @ 0x004BD9C0..0x004BD9EF =====
LONG __stdcall sub_4BD9C0(volatile LONG *a1)
{
  LONG result; // eax

  result = InterlockedDecrement(a1 + 5);
  if ( !result )
  {
    if ( a1 )
    {
      (*(void (__thiscall **)(volatile LONG *, int))(*a1 + 28))(a1, 1);
      return 0;
    }
  }
  return result;
}

// ===== sub_4BD9F0 @ 0x004BD9F0..0x004BDA35 =====
int __stdcall sub_4BD9F0(int a1, unsigned int a2)
{
  int v3; // eax

  if ( (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 20))(*(_DWORD *)(a1 + 12)) != *(_DWORD *)(a1 + 16) )
    return -2147220989;
  v3 = *(_DWORD *)(a1 + 4);
  if ( a2 > *(_DWORD *)(a1 + 8) - v3 )
    return 1;
  *(_DWORD *)(a1 + 4) = a2 + v3;
  return 0;
}

// ===== sub_4BDA40 @ 0x004BDA40..0x004BDA77 =====
int __stdcall sub_4BDA40(int a1)
{
  int v1; // eax
  int v2; // ecx

  v1 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 20))(*(_DWORD *)(a1 + 12));
  v2 = *(_DWORD *)(a1 + 12);
  *(_DWORD *)(a1 + 16) = v1;
  *(_DWORD *)(a1 + 8) = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 24))(v2);
  *(_DWORD *)(a1 + 4) = 0;
  sub_4C3AE0(a1 + 24);
  return 0;
}

// ===== sub_4BDA80 @ 0x004BDA80..0x004BDAAF =====
int __stdcall sub_4BDA80(int a1)
{
  int v1; // eax
  int v2; // ecx

  v1 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 20))(*(_DWORD *)(a1 + 12));
  v2 = *(_DWORD *)(a1 + 12);
  *(_DWORD *)(a1 + 16) = v1;
  *(_DWORD *)(a1 + 8) = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 24))(v2);
  *(_DWORD *)(a1 + 4) = 0;
  return 0;
}

// ===== sub_4BDAB0 @ 0x004BDAB0..0x004BDAC5 =====
int __thiscall sub_4BDAB0(_DWORD *this)
{
  int v1; // eax

  v1 = this[2] + 12;
  *this = &CEnumMediaTypes::`vftable';
  return (*(int (__stdcall **)(int))(*(_DWORD *)v1 + 8))(v1);
}

// ===== sub_4BDAD0 @ 0x004BDAD0..0x004BDB2A =====
int __stdcall sub_4BDAD0(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  if ( sub_445510(a2, dword_4DB924) || sub_445510(a2, dword_4DC2E8) )
    return sub_4BCF30(a1, a3);
  *a3 = 0;
  return -2147467262;
}

// ===== sub_4BDB30 @ 0x004BDB30..0x004BDB44 =====
LONG __stdcall sub_4BDB30(int a1)
{
  return InterlockedIncrement((volatile LONG *)(a1 + 16));
}

// ===== sub_4BDB50 @ 0x004BDB50..0x004BDB7F =====
LONG __stdcall sub_4BDB50(volatile LONG *a1)
{
  LONG result; // eax

  result = InterlockedDecrement(a1 + 4);
  if ( !result )
  {
    if ( a1 )
    {
      (*(void (__thiscall **)(volatile LONG *, int))(*a1 + 28))(a1, 1);
      return 0;
    }
  }
  return result;
}

// ===== sub_4BDB80 @ 0x004BDB80..0x004BDCC4 =====
int __stdcall sub_4BDB80(int a1, unsigned int a2, _DWORD *a3, _DWORD *a4)
{
  _DWORD *v4; // edi
  int v5; // esi
  unsigned int v7; // ebx
  int v8; // eax
  int v9; // ecx
  int (__thiscall *v10)(int, int, _DWORD *); // eax
  void *v11; // eax
  int v12; // [esp-8h] [ebp-80h]
  _DWORD *v13; // [esp+18h] [ebp-60h]
  int v14; // [esp+1Ch] [ebp-5Ch]
  _DWORD v15[18]; // [esp+20h] [ebp-58h] BYREF
  int v16; // [esp+74h] [ebp-4h]

  v4 = a3;
  v5 = a1;
  v13 = a3;
  if ( !a3 )
    return -2147467261;
  if ( (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 8) + 16))(*(_DWORD *)(a1 + 8)) != *(_DWORD *)(a1 + 12) )
    return -2147220989;
  v7 = a2;
  if ( a4 )
  {
    *a4 = 0;
  }
  else if ( a2 > 1 )
  {
    return -2147024809;
  }
  v14 = 0;
  if ( a2 )
  {
    while ( 1 )
    {
      sub_4C0AB0(v15);
      v8 = *(_DWORD *)(v5 + 4);
      *(_DWORD *)(v5 + 4) = v8 + 1;
      v9 = *(_DWORD *)(v5 + 8);
      v12 = v8;
      v10 = *(int (__thiscall **)(int, int, _DWORD *))(*(_DWORD *)v9 + 52);
      v16 = 0;
      if ( v10(v9, v12, v15) )
        break;
      v11 = CoTaskMemAlloc(0x48u);
      *v4 = v11;
      v16 = -1;
      if ( !v11 )
        goto LABEL_17;
      ++v13;
      ++v14;
      qmemcpy(v11, v15, 0x48u);
      memset(&v15[15], 0, 12);
      --v7;
      sub_4C0AA0(v15);
      if ( !v7 )
        goto LABEL_18;
      v5 = a1;
      v4 = v13;
    }
    v16 = -1;
LABEL_17:
    sub_4C0AA0(v15);
  }
LABEL_18:
  if ( a4 )
    *a4 = v14;
  return v7 != 0;
}

// ===== sub_4BDCD0 @ 0x004BDCD0..0x004BDD77 =====
int __stdcall sub_4BDCD0(int a1, int a2)
{
  int v3; // ecx
  int (__thiscall *v4)(int, int, _BYTE *); // eax
  BOOL v5; // esi
  int v6; // [esp-8h] [ebp-6Ch]
  _BYTE v7[72]; // [esp+Ch] [ebp-58h] BYREF
  int v8; // [esp+60h] [ebp-4h]

  if ( !a2 )
    return 0;
  if ( (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 8) + 16))(*(_DWORD *)(a1 + 8)) != *(_DWORD *)(a1 + 12) )
    return -2147220989;
  *(_DWORD *)(a1 + 4) += a2;
  sub_4C0AB0(v7);
  v3 = *(_DWORD *)(a1 + 8);
  v4 = *(int (__thiscall **)(int, int, _BYTE *))(*(_DWORD *)v3 + 52);
  v6 = *(_DWORD *)(a1 + 4) - 1;
  v8 = 0;
  v5 = v4(v3, v6, v7) != 0;
  v8 = -1;
  sub_4C0AA0(v7);
  return v5;
}

// ===== sub_4BDD80 @ 0x004BDD80..0x004BDDA2 =====
int __stdcall sub_4BDD80(_DWORD *a1)
{
  int v1; // ecx

  v1 = a1[2];
  a1[1] = 0;
  a1[3] = (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 16))(v1);
  return 0;
}

// ===== sub_4BDDB0 @ 0x004BDDB0..0x004BDE17 =====
HMODULE __thiscall sub_4BDDB0(void **this)
{
  operator delete(this[5]);
  sub_4C0AA0(this + 13);
  return sub_4BCEE0();
}

// ===== sub_4BDE20 @ 0x004BDE20..0x004BDEB2 =====
int __stdcall sub_4BDE20(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB904) )
  {
    if ( a1 )
      return sub_4BCF30(a1 + 12, a3);
    return sub_4BCF30(0, a3);
  }
  if ( !sub_445510(a2, dword_4DB9D4) )
    return sub_4BCF90(a1, a2, a3);
  if ( !a1 )
    return sub_4BCF30(0, a3);
  return sub_4BCF30(a1 + 16, a3);
}

// ===== sub_4BDEC0 @ 0x004BDEC0..0x004BDED9 =====
int __stdcall sub_4BDEC0(int a1)
{
  return (*(int (__stdcall **)(int))(*(_DWORD *)(*(_DWORD *)(a1 + 40) + 12) + 4))(*(_DWORD *)(a1 + 40) + 12);
}

// ===== sub_4BDEE0 @ 0x004BDEE0..0x004BDEF9 =====
int __stdcall sub_4BDEE0(int a1)
{
  return (*(int (__stdcall **)(int))(*(_DWORD *)(*(_DWORD *)(a1 + 40) + 12) + 8))(*(_DWORD *)(a1 + 40) + 12);
}

// ===== sub_4BDF00 @ 0x004BDF00..0x004BDFC9 =====
int __thiscall sub_4BDF00(_DWORD *this, int a2, int a3)
{
  int v4; // edi
  int v5; // eax
  int result; // eax
  int v7; // eax
  int v8; // edi
  int v9; // eax

  v4 = (*(int (__thiscall **)(_DWORD *, int))(*this + 40))(this, a2);
  v5 = *this;
  if ( v4 < 0 )
  {
    (*(void (__thiscall **)(_DWORD *))(v5 + 44))(this);
    return v4;
  }
  v7 = (*(int (__thiscall **)(_DWORD *, int))(v5 + 32))(this, a3);
  v8 = v7;
  if ( v7 )
  {
    if ( v7 >= 0 || v7 == -2147467259 || v7 == -2147024809 )
      v8 = -2147220950;
  }
  else
  {
    this[6] = a2;
    (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 4))(a2);
    v8 = (*(int (__thiscall **)(_DWORD *, int))(*this + 36))(this, a3);
    if ( v8 >= 0 )
    {
      v8 = (*(int (__stdcall **)(int, _DWORD *, int))(*(_DWORD *)a2 + 16))(a2, this + 3, a3);
      if ( v8 >= 0 )
      {
        result = (*(int (__thiscall **)(_DWORD *, int))(*this + 48))(this, a2);
        v8 = result;
        if ( result >= 0 )
          return result;
        (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 20))(a2);
      }
    }
  }
  (*(void (__thiscall **)(_DWORD *))(*this + 44))(this);
  v9 = this[6];
  if ( v9 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v9 + 8))(this[6]);
    this[6] = 0;
  }
  return v8;
}

// ===== sub_4BDFD0 @ 0x004BDFD0..0x004BE0A1 =====
int __thiscall sub_4BDFD0(_DWORD *this, int a2, int a3, LPVOID pv)
{
  LPVOID v4; // edi
  int result; // eax
  int v6; // ebx
  int (__stdcall *v7)(LPVOID, int, LPVOID *, int *); // edx
  int v8; // esi
  int v9; // eax
  int v11; // [esp+Ch] [ebp-4h] BYREF

  v4 = pv;
  result = (*(int (__stdcall **)(LPVOID))(*(_DWORD *)pv + 20))(pv);
  v6 = 0;
  if ( result >= 0 )
  {
    v7 = *(int (__stdcall **)(LPVOID, int, LPVOID *, int *))(*(_DWORD *)v4 + 12);
    pv = 0;
    v11 = 0;
    if ( !v7(v4, 1, &pv, &v11) )
    {
      do
      {
        if ( !a3 || sub_4C0900(a3) )
        {
          v9 = sub_4BDF00(this, a2, (int)pv);
          v8 = v9;
          if ( v9 < 0 && v6 >= 0 && v9 != -2147467259 && v9 != -2147024809 && v9 != -2147220950 )
            v6 = v9;
        }
        else
        {
          v8 = -2147220985;
        }
        sub_4C0B00(pv);
        if ( !v8 )
          return 0;
      }
      while ( !(*(int (__stdcall **)(LPVOID, int, LPVOID *, int *))(*(_DWORD *)v4 + 12))(v4, 1, &pv, &v11) );
      if ( v6 )
        return v6;
    }
    return -2147220985;
  }
  return result;
}

// ===== sub_4BE0B0 @ 0x004BE0B0..0x004BE189 =====
int __thiscall sub_4BE0B0(int this, int a2, int a3)
{
  int v3; // esi
  int v6; // eax
  int v7; // eax
  int v8; // esi
  int v9; // [esp+8h] [ebp-Ch]
  int v10; // [esp+Ch] [ebp-8h]
  LPVOID pv; // [esp+10h] [ebp-4h] BYREF

  v3 = a3;
  pv = 0;
  if ( a3 && !sub_4C08C0(a3) )
    return sub_4BDF00((_DWORD *)this, a2, a3);
  v6 = 0;
  v9 = -2147220985;
  v10 = 0;
  while ( 1 )
  {
    v7 = v6 == *(unsigned __int8 *)(this + 38)
       ? (*(int (__stdcall **)(int, LPVOID *))(*(_DWORD *)a2 + 48))(a2, &pv)
       : (*(int (__stdcall **)(int, LPVOID *))(*(_DWORD *)(this + 12) + 48))(this + 12, &pv);
    if ( v7 >= 0 )
      break;
LABEL_15:
    v6 = v10 + 1;
    v10 = v6;
    if ( v6 >= 2 )
      return v9;
    v3 = a3;
  }
  v8 = sub_4BDFD0((_DWORD *)this, a2, v3, pv);
  (*(void (__stdcall **)(LPVOID))(*(_DWORD *)pv + 8))(pv);
  if ( v8 < 0 )
  {
    if ( v8 != -2147467259 && v8 != -2147024809 && v8 != -2147220950 )
      v9 = v8;
    goto LABEL_15;
  }
  return 0;
}

// ===== sub_4BE190 @ 0x004BE190..0x004BE1AD =====
int __stdcall sub_4BE190(int a1)
{
  int v1; // eax

  v1 = sub_4C0BA0(a1);
  return v1 >= 0 ? 0 : v1;
}

// ===== sub_4BE1B0 @ 0x004BE1B0..0x004BE1DB =====
unsigned int __thiscall sub_4BE1B0(_DWORD *this, int a2)
{
  (*(void (__stdcall **)(int, int *))(*(_DWORD *)a2 + 36))(a2, &a2);
  return a2 != this[7] ? 0 : 0x80040208;
}

// ===== sub_4BE1E0 @ 0x004BE1E0..0x004BE387 =====
int __stdcall sub_4BE1E0(LPCRITICAL_SECTION lpCriticalSection, void *a2, int a3)
{
  int v5; // eax
  int (__thiscall **OwningThread)(char *, int); // edx
  int v7; // ebx
  char *p_OwningThread; // ecx
  void (__thiscall *v9)(HANDLE *); // edx
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+24h] [ebp+8h]

  if ( !a2 || !a3 )
    return -2147467261;
  lpCriticalSectiona = (struct _RTL_CRITICAL_SECTION *)lpCriticalSection->SpinCount;
  EnterCriticalSection(lpCriticalSectiona);
  if ( lpCriticalSection->OwningThread )
  {
    LeaveCriticalSection(lpCriticalSectiona);
    return -2147220988;
  }
  if ( *(_DWORD *)(lpCriticalSection[1].LockCount + 20) && !BYTE1(lpCriticalSection[1].DebugInfo) )
  {
    LeaveCriticalSection(lpCriticalSectiona);
    return -2147220956;
  }
  v5 = (*((int (__thiscall **)(HANDLE *, void *))lpCriticalSection[-1].OwningThread + 10))(
         &lpCriticalSection[-1].OwningThread,
         a2);
  OwningThread = (int (__thiscall **)(char *, int))lpCriticalSection[-1].OwningThread;
  v7 = v5;
  p_OwningThread = (char *)&lpCriticalSection[-1].OwningThread;
  if ( v5 >= 0 )
  {
    v7 = OwningThread[8](p_OwningThread, a3);
    if ( v7 )
    {
      (*((void (__thiscall **)(HANDLE *))lpCriticalSection[-1].OwningThread + 11))(&lpCriticalSection[-1].OwningThread);
      if ( v7 >= 0 || v7 == -2147467259 || v7 == -2147024809 )
        v7 = -2147220950;
    }
    else
    {
      lpCriticalSection->OwningThread = a2;
      (*(void (__stdcall **)(void *))(*(_DWORD *)a2 + 4))(a2);
      v7 = (*((int (__thiscall **)(HANDLE *, int))lpCriticalSection[-1].OwningThread + 9))(
             &lpCriticalSection[-1].OwningThread,
             a3);
      if ( v7 >= 0 )
      {
        v7 = (*((int (__thiscall **)(HANDLE *, void *))lpCriticalSection[-1].OwningThread + 12))(
               &lpCriticalSection[-1].OwningThread,
               a2);
        if ( v7 >= 0 )
        {
          LeaveCriticalSection(lpCriticalSectiona);
          return 0;
        }
      }
      (*(void (__stdcall **)(HANDLE))(*(_DWORD *)lpCriticalSection->OwningThread + 8))(lpCriticalSection->OwningThread);
      v9 = (void (__thiscall *)(HANDLE *))*((_DWORD *)lpCriticalSection[-1].OwningThread + 11);
      lpCriticalSection->OwningThread = 0;
      v9(&lpCriticalSection[-1].OwningThread);
    }
    LeaveCriticalSection(lpCriticalSectiona);
  }
  else
  {
    ((void (__thiscall *)(char *))OwningThread[11])(p_OwningThread);
    LeaveCriticalSection(lpCriticalSectiona);
  }
  return v7;
}

// ===== sub_4BE390 @ 0x004BE390..0x004BE3CD =====
int __stdcall sub_4BE390(_DWORD *a1)
{
  int result; // eax

  if ( !a1[6] )
    return 1;
  result = (*(int (__thiscall **)(_DWORD *))(*a1 + 44))(a1);
  if ( result >= 0 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)a1[6] + 8))(a1[6]);
    a1[6] = 0;
    return 0;
  }
  return result;
}

// ===== sub_4BE3D0 @ 0x004BE3D0..0x004BE406 =====
int __stdcall sub_4BE3D0(int a1, _DWORD *a2)
{
  int v3; // eax

  if ( !a2 )
    return -2147467261;
  v3 = *(_DWORD *)(a1 + 12);
  *a2 = v3;
  if ( !v3 )
    return -2147220983;
  (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 4))(v3);
  return 0;
}

// ===== sub_4BE410 @ 0x004BE410..0x004BE4C0 =====
int __stdcall sub_4BE410(int a1, void *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // esi

  if ( !a2 )
    return -2147467261;
  v3 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 20);
  EnterCriticalSection(v3);
  if ( *(_DWORD *)(a1 + 12) )
  {
    sub_4C09F0(a2, a1 + 40);
    LeaveCriticalSection(v3);
    return 0;
  }
  else
  {
    sub_4C08A0(a2);
    LeaveCriticalSection(v3);
    return -2147220983;
  }
}

// ===== sub_4BE4C0 @ 0x004BE4C0..0x004BE536 =====
int __stdcall sub_4BE4C0(_DWORD *a1, int a2)
{
  int v3; // eax
  int v4; // eax
  int v5; // eax

  if ( !a2 )
    return -2147467261;
  v3 = a1[7];
  if ( v3 )
    v4 = v3 + 12;
  else
    v4 = 0;
  *(_DWORD *)a2 = v4;
  if ( a1[7] )
    (*(void (__stdcall **)(int))(*(_DWORD *)(a1[7] + 12) + 4))(a1[7] + 12);
  v5 = a1[2];
  if ( v5 )
    sub_4C3430(a2 + 8, v5, 128);
  else
    *(_WORD *)(a2 + 8) = 0;
  *(_DWORD *)(a2 + 4) = a1[4];
  return 0;
}

// ===== sub_4BE540 @ 0x004BE540..0x004BE561 =====
int __stdcall sub_4BE540(int a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  *a2 = *(_DWORD *)(a1 + 16);
  return 0;
}

// ===== sub_4BE570 @ 0x004BE570..0x004BE587 =====
int __stdcall sub_4BE570(int a1, int a2)
{
  return sub_4C3780(*(void **)(a1 + 8), a2);
}

// ===== sub_4BE590 @ 0x004BE590..0x004BE5BF =====
int __stdcall sub_4BE590(int a1, int a2)
{
  int result; // eax

  if ( !a2 )
    return -2147467261;
  result = (*(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 32))(a1 - 12, a2);
  if ( result < 0 )
    return 1;
  return result;
}

// ===== sub_4BE5C0 @ 0x004BE5C0..0x004BE5C8 =====
int __stdcall sub_4BE5C0(int a1, int a2)
{
  return -2147418113;
}

// ===== sub_4BE5D0 @ 0x004BE5D0..0x004BE5D4 =====
int __thiscall sub_4BE5D0(_DWORD *this)
{
  return this[12];
}

// ===== sub_4BE5E0 @ 0x004BE5E0..0x004BE5E3 =====
int sub_4BE5E0()
{
  return 0;
}

// ===== sub_4BE5F0 @ 0x004BE5F0..0x004BE5F7 =====
int __thiscall sub_4BE5F0(_BYTE *this)
{
  this[36] = 0;
  return 0;
}

// ===== sub_4BE600 @ 0x004BE600..0x004BE627 =====
int __stdcall sub_4BE600(int a1, int a2)
{
  struct _RTL_CRITICAL_SECTION *v2; // esi

  v2 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 16);
  EnterCriticalSection(v2);
  *(_DWORD *)(a1 + 28) = a2;
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4BE630 @ 0x004BE630..0x004BE638 =====
int __stdcall sub_4BE630(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  return -2147467263;
}

// ===== sub_4BE640 @ 0x004BE640..0x004BE670 =====
int __stdcall sub_4BE640(int a1, int a2, int a3, int a4, int a5, double a6)
{
  *(_DWORD *)(a1 + 116) = a2;
  *(_DWORD *)(a1 + 120) = a3;
  *(_DWORD *)(a1 + 124) = a4;
  *(_DWORD *)(a1 + 128) = a5;
  *(double *)(a1 + 132) = a6;
  return 0;
}

// ===== sub_4BE670 @ 0x004BE670..0x004BE6DA =====
HMODULE __thiscall sub_4BE670(int this)
{
  int v2; // eax

  v2 = *(_DWORD *)(this + 156);
  if ( v2 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v2 + 8))(v2);
    *(_DWORD *)(this + 156) = 0;
  }
  return sub_4BDDB0((void **)this);
}

// ===== sub_4BE6E0 @ 0x004BE6E0..0x004BE735 =====
int __stdcall sub_4BE6E0(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !sub_445510(a2, dword_4DB994) )
    return sub_4BDE20(a1, a2, a3);
  if ( a1 )
    return sub_4BCF30(a1 + 152, a3);
  return sub_4BCF30(0, a3);
}

// ===== sub_4BE740 @ 0x004BE740..0x004BE7FA =====
int __stdcall sub_4BE740(int a1, LPVOID *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  LPVOID *v4; // esi
  HRESULT v5; // ebx

  if ( !a2 )
    return -2147467261;
  v3 = *(struct _RTL_CRITICAL_SECTION **)(a1 - 120);
  EnterCriticalSection(v3);
  v4 = (LPVOID *)(a1 + 4);
  if ( *(_DWORD *)(a1 + 4) || (v5 = sub_4BD060(v4), v5 >= 0) )
  {
    *a2 = *v4;
    (*(void (__stdcall **)(LPVOID))(*(_DWORD *)*v4 + 4))(*v4);
    LeaveCriticalSection(v3);
    return 0;
  }
  else
  {
    LeaveCriticalSection(v3);
    return v5;
  }
}

// ===== sub_4BE800 @ 0x004BE800..0x004BE89C =====
int __stdcall sub_4BE800(int a1, LPCRITICAL_SECTION lpCriticalSection, char a3)
{
  int v5; // ebx
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+28h] [ebp+Ch]

  if ( !lpCriticalSection )
    return -2147467261;
  lpCriticalSectiona = *(struct _RTL_CRITICAL_SECTION **)(a1 - 120);
  EnterCriticalSection(lpCriticalSectiona);
  v5 = *(_DWORD *)(a1 + 4);
  ((void (__stdcall *)(LPCRITICAL_SECTION))lpCriticalSection->DebugInfo->CriticalSection)(lpCriticalSection);
  *(_DWORD *)(a1 + 4) = lpCriticalSection;
  if ( v5 )
    (*(void (__stdcall **)(int))(*(_DWORD *)v5 + 8))(v5);
  *(_BYTE *)(a1 + 8) = a3;
  LeaveCriticalSection(lpCriticalSectiona);
  return 0;
}

// ===== sub_4BE8E0 @ 0x004BE8E0..0x004BEA44 =====
int __userpurge sub_4BE8E0@<eax>(int a1@<ebx>, int a2@<esi>, _DWORD *a3, int a4)
{
  int v4; // edi
  int result; // eax
  int v6; // edi
  void (__stdcall *v7)(_DWORD *); // edx

  v4 = a4;
  if ( !a4 )
    return -2147467261;
  result = (*(int (__thiscall **)(_DWORD *))(*(a3 - 38) + 56))(a3 - 38);
  if ( !result )
  {
    if ( (**(int (__stdcall ***)(int, void *, int *, int, int))v4)(v4, &unk_4DB964, &a4, a2, a1) < 0 )
    {
      a3[4] = 48;
      a3[5] = 0;
      a3[12] = 0;
      a3[6] = 0;
      if ( !(*(int (__stdcall **)(int))(*(_DWORD *)v4 + 60))(v4) )
        a3[6] |= 4u;
      if ( !(*(int (__stdcall **)(int))(*(_DWORD *)v4 + 36))(v4) )
        a3[6] |= 2u;
      if ( !(*(int (__stdcall **)(int))(*(_DWORD *)v4 + 28))(v4) )
        a3[6] |= 1u;
      if ( (*(int (__stdcall **)(int, _DWORD *, _DWORD *))(*(_DWORD *)v4 + 20))(v4, a3 + 8, a3 + 10) >= 0 )
        a3[6] |= 0x110u;
      if ( !(*(int (__stdcall **)(int, _DWORD *))(*(_DWORD *)v4 + 52))(v4, a3 + 13) )
        a3[6] |= 8u;
      (*(void (__cdecl **)(int, _DWORD *))(*(_DWORD *)v4 + 12))(v4, a3 + 14);
      a3[7] = (*(int (__stdcall **)(int))(*(_DWORD *)v4 + 44))(v4);
      a3[15] = (*(int (__stdcall **)(int))(*(_DWORD *)v4 + 16))(v4);
    }
    else
    {
      v6 = (*(int (__stdcall **)(int))(*(_DWORD *)a4 + 76))(a4);
      (*(void (__stdcall **)(int))(*(_DWORD *)a4 + 8))(a4);
      if ( v6 < 0 )
        return v6;
    }
    if ( (a3[6] & 8) != 0 && (*(int (__thiscall **)(_DWORD *, _DWORD))(*(a3 - 38) + 32))(a3 - 38, a3[13]) )
    {
      v7 = *(void (__stdcall **)(_DWORD *))(*(a3 - 35) + 56);
      *((_BYTE *)a3 - 116) = 1;
      v7(a3 - 35);
      sub_4BD880((_DWORD *)*(a3 - 28), 3, -2147220950, 0);
      return -2147220992;
    }
    else
    {
      return 0;
    }
  }
  return result;
}

// ===== sub_4BEA50 @ 0x004BEA50..0x004BEA99 =====
int __stdcall sub_4BEA50(int a1, int a2, int a3, _DWORD *a4)
{
  int result; // eax
  int v5; // edi

  if ( !a2 )
    return -2147467261;
  v5 = a3;
  result = 0;
  for ( *a4 = 0; v5 > 0; ++*a4 )
  {
    --v5;
    result = (*(int (__stdcall **)(int, _DWORD))(*(_DWORD *)a1 + 24))(a1, *(_DWORD *)(a2 + 4 * *a4));
    if ( result )
      break;
  }
  return result;
}

// ===== sub_4BEAA0 @ 0x004BEAA0..0x004BEB60 =====
int __stdcall sub_4BEAA0(int a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // esi
  int result; // eax
  int v5; // esi
  int v6; // esi
  int v7; // [esp+Ch] [ebp-10h]
  int v8; // [esp+10h] [ebp-Ch] BYREF
  int v9; // [esp+14h] [ebp-8h] BYREF
  int v10; // [esp+18h] [ebp-4h] BYREF

  v1 = 0;
  v2 = 0;
  v7 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 - 112) + 24))(*(_DWORD *)(a1 - 112));
  if ( v7 <= 0 )
    return v1 != 0;
  while ( 1 )
  {
    v3 = (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(a1 - 112) + 28))(*(_DWORD *)(a1 - 112), v2) + 12;
    result = (*(int (__stdcall **)(int, int *))(*(_DWORD *)v3 + 36))(v3, &v8);
    if ( result < 0 )
      break;
    if ( v8 == 1 && (*(int (__stdcall **)(int, int *))(*(_DWORD *)v3 + 24))(v3, &v10) >= 0 )
    {
      ++v1;
      v5 = (**(int (__stdcall ***)(int, _DWORD *, int *))v10)(v10, dword_4DB994, &v9);
      (*(void (__stdcall **)(int))(*(_DWORD *)v10 + 8))(v10);
      if ( v5 < 0 )
        return 0;
      v6 = (*(int (__stdcall **)(int))(*(_DWORD *)v9 + 32))(v9);
      (*(void (__stdcall **)(int))(*(_DWORD *)v9 + 8))(v9);
      if ( v6 != 1 )
        return 0;
    }
    if ( ++v2 >= v7 )
      return v1 != 0;
  }
  return result;
}

// ===== sub_4BEB60 @ 0x004BEB60..0x004BEB88 =====
int __stdcall sub_4BEB60(int a1)
{
  struct _RTL_CRITICAL_SECTION *v1; // esi

  v1 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 20);
  EnterCriticalSection(v1);
  *(_BYTE *)(a1 + 149) = 1;
  LeaveCriticalSection(v1);
  return 0;
}

// ===== sub_4BEB90 @ 0x004BEB90..0x004BEBBC =====
int __stdcall sub_4BEB90(int a1)
{
  struct _RTL_CRITICAL_SECTION *v1; // edi

  v1 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 20);
  EnterCriticalSection(v1);
  *(_BYTE *)(a1 + 149) = 0;
  *(_BYTE *)(a1 + 24) = 0;
  LeaveCriticalSection(v1);
  return 0;
}

// ===== sub_4BEBC0 @ 0x004BEBC0..0x004BEBD8 =====
int __stdcall sub_4BEBC0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  return a2 != 0 ? 0 : -2147467261;
}

// ===== sub_4BEBE0 @ 0x004BEBE0..0x004BEC0C =====
unsigned int __thiscall sub_4BEBE0(int this)
{
  if ( !*(_DWORD *)(*(_DWORD *)(this + 40) + 20) )
    return -2147220953;
  if ( *(_BYTE *)(this + 161) )
    return 1;
  return *(_BYTE *)(this + 36) != 0 ? 0x8004020B : 0;
}

// ===== sub_4BEC10 @ 0x004BEC10..0x004BEC24 =====
int __thiscall sub_4BEC10(_DWORD *this)
{
  void *v1; // ecx
  int result; // eax

  *this = &CMediaSample::`vftable';
  v1 = (void *)this[15];
  if ( v1 )
    return sub_4C0B00(v1);
  return result;
}

// ===== sub_4BEC30 @ 0x004BEC30..0x004BEC89 =====
int __stdcall sub_4BEC30(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB954) || sub_445510(a2, dword_4DB964) || sub_445510(a2, dword_4DC2E8) )
    return sub_4BCF30(a1, a3);
  else
    return -2147467262;
}

// ===== sub_4BEC90 @ 0x004BEC90..0x004BECA4 =====
LONG __stdcall sub_4BEC90(int a1)
{
  return InterlockedIncrement((volatile LONG *)(a1 + 68));
}

// ===== sub_4BECB0 @ 0x004BECB0..0x004BED10 =====
LONG __stdcall sub_4BECB0(int a1)
{
  LONG v1; // edi
  LONG result; // eax
  int v3; // eax

  if ( *(_DWORD *)(a1 + 68) == 1 )
  {
    v1 = 0;
    *(_DWORD *)(a1 + 68) = 0;
  }
  else
  {
    result = InterlockedDecrement((volatile LONG *)(a1 + 68));
    v1 = result;
    if ( result )
      return result;
  }
  if ( (*(_BYTE *)(a1 + 4) & 8) != 0 )
    (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)a1 + 56))(a1, 0);
  v3 = *(_DWORD *)(a1 + 24) + 12;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 64) = 0;
  (*(void (__stdcall **)(int, int))(*(_DWORD *)v3 + 32))(v3, a1);
  return v1;
}

// ===== sub_4BED10 @ 0x004BED10..0x004BED24 =====
int __stdcall sub_4BED10(int a1, _DWORD *a2)
{
  *a2 = *(_DWORD *)(a1 + 12);
  return 0;
}

// ===== sub_4BED30 @ 0x004BED30..0x004BED3D =====
int __stdcall sub_4BED30(int a1)
{
  return *(_DWORD *)(a1 + 20);
}

// ===== sub_4BED40 @ 0x004BED40..0x004BEDAC =====
int __stdcall sub_4BED40(_DWORD *a1, _DWORD *a2, _DWORD *a3)
{
  int v3; // ecx

  v3 = a1[1];
  if ( (v3 & 0x100) != 0 )
  {
    *a2 = a1[8];
    a2[1] = a1[9];
    *a3 = a1[10];
    a3[1] = a1[11];
    return 0;
  }
  else if ( (v3 & 0x10) != 0 )
  {
    *a2 = a1[8];
    a2[1] = a1[9];
    *(_QWORD *)a3 = *((_QWORD *)a1 + 4) + 1LL;
    return 262768;
  }
  else
  {
    return -2147220919;
  }
}

// ===== sub_4BEDB0 @ 0x004BEDB0..0x004BEE16 =====
int __stdcall sub_4BEDB0(_DWORD *a1, _DWORD *a2, _DWORD *a3)
{
  unsigned int v4; // edx
  int v5; // edx

  if ( a2 )
  {
    a1[8] = *a2;
    if ( a3 )
    {
      a1[9] = a2[1];
      a1[10] = *a3;
      v5 = a3[1];
      a1[1] |= 0x110u;
      a1[11] = v5;
    }
    else
    {
      v4 = a1[1] & 0xFFFFFEEF | 0x10;
      a1[9] = a2[1];
      a1[1] = v4;
    }
    return 0;
  }
  else
  {
    a1[1] &= 0xFFFFFEEF;
    return 0;
  }
}

// ===== sub_4BEE20 @ 0x004BEE20..0x004BEE5F =====
int __stdcall sub_4BEE20(int a1, _DWORD *a2, _QWORD *a3)
{
  if ( (*(_BYTE *)(a1 + 4) & 0x20) == 0 )
    return -2147220911;
  *a2 = *(_DWORD *)(a1 + 48);
  a2[1] = *(_DWORD *)(a1 + 52);
  *a3 = *(_QWORD *)(a1 + 48) + *(int *)(a1 + 56);
  return 0;
}

// ===== sub_4BEE60 @ 0x004BEE60..0x004BEE96 =====
int __stdcall sub_4BEE60(_DWORD *a1, _DWORD *a2, _DWORD *a3)
{
  int v4; // edx

  if ( a2 )
  {
    a1[12] = *a2;
    a1[13] = a2[1];
    v4 = *a3 - *a2;
    a1[1] |= 0x20u;
    a1[14] = v4;
  }
  else
  {
    a1[1] &= ~0x20u;
  }
  return 0;
}

// ===== sub_4BEEA0 @ 0x004BEEA0..0x004BEEB5 =====
BOOL __stdcall sub_4BEEA0(int a1)
{
  return (*(_BYTE *)(a1 + 4) & 1) == 0;
}

// ===== sub_4BEEC0 @ 0x004BEEC0..0x004BEEE0 =====
int __stdcall sub_4BEEC0(int a1, int a2)
{
  if ( a2 )
    *(_DWORD *)(a1 + 4) |= 1u;
  else
    *(_DWORD *)(a1 + 4) &= ~1u;
  return 0;
}

// ===== sub_4BEEE0 @ 0x004BEEE0..0x004BEEF8 =====
int __stdcall sub_4BEEE0(int a1)
{
  return ((unsigned __int8)~*(_BYTE *)(a1 + 4) >> 2) & 1;
}

// ===== sub_4BEF00 @ 0x004BEF00..0x004BEF20 =====
int __stdcall sub_4BEF00(int a1, int a2)
{
  if ( a2 )
    *(_DWORD *)(a1 + 4) |= 4u;
  else
    *(_DWORD *)(a1 + 4) &= ~4u;
  return 0;
}

// ===== sub_4BEF20 @ 0x004BEF20..0x004BEF37 =====
int __stdcall sub_4BEF20(int a1)
{
  return ((unsigned __int8)~*(_BYTE *)(a1 + 4) >> 1) & 1;
}

// ===== sub_4BEF40 @ 0x004BEF40..0x004BEF60 =====
int __stdcall sub_4BEF40(int a1, int a2)
{
  if ( a2 )
    *(_DWORD *)(a1 + 4) |= 2u;
  else
    *(_DWORD *)(a1 + 4) &= ~2u;
  return 0;
}

// ===== sub_4BEF60 @ 0x004BEF60..0x004BEF6D =====
int __stdcall sub_4BEF60(int a1)
{
  return *(_DWORD *)(a1 + 16);
}

// ===== sub_4BEF70 @ 0x004BEF70..0x004BEF90 =====
int __stdcall sub_4BEF70(int a1, int a2)
{
  if ( a2 > *(_DWORD *)(a1 + 20) )
    return -2147220979;
  *(_DWORD *)(a1 + 16) = a2;
  return 0;
}

// ===== sub_4BEF90 @ 0x004BEF90..0x004BEFCE =====
int __stdcall sub_4BEF90(int a1, _DWORD *a2)
{
  int v3; // eax

  if ( (*(_BYTE *)(a1 + 4) & 8) != 0 )
  {
    v3 = sub_4C0B20(*(_DWORD *)(a1 + 60));
    *a2 = v3;
    return v3 != 0 ? 0 : -2147024882;
  }
  else
  {
    *a2 = 0;
    return 1;
  }
}

// ===== sub_4BEFD0 @ 0x004BEFD0..0x004BF021 =====
int __stdcall sub_4BEFD0(int a1, int a2)
{
  int result; // eax
  int v3; // eax

  if ( *(_DWORD *)(a1 + 60) )
  {
    sub_4C0B00(*(LPVOID *)(a1 + 60));
    *(_DWORD *)(a1 + 60) = 0;
  }
  result = a2;
  if ( a2 )
  {
    v3 = sub_4C0B20(a2);
    *(_DWORD *)(a1 + 60) = v3;
    if ( v3 )
    {
      *(_DWORD *)(a1 + 4) |= 8u;
      return 0;
    }
    else
    {
      *(_DWORD *)(a1 + 4) &= ~8u;
      return -2147024882;
    }
  }
  else
  {
    *(_DWORD *)(a1 + 4) &= ~8u;
  }
  return result;
}

// ===== sub_4BF030 @ 0x004BF030..0x004BF0D5 =====
int __stdcall sub_4BF030(_DWORD *a1, size_t Size, void *a3)
{
  size_t v4; // edx
  int v5; // ecx
  _DWORD Src[9]; // [esp+4h] [ebp-30h] BYREF
  int v7; // [esp+28h] [ebp-Ch]
  int v8; // [esp+2Ch] [ebp-8h]
  int v9; // [esp+30h] [ebp-4h]

  if ( Size )
  {
    if ( !a3 )
      return -2147467261;
    v4 = Size;
    if ( Size >= 0x30 )
      v4 = 48;
    v5 = a1[1];
    Src[2] = v5 & 0xFFFFFFDF;
    Src[1] = a1[2];
    v8 = a1[3];
    v9 = a1[5];
    Src[3] = a1[4];
    Src[4] = a1[8];
    Src[5] = a1[9];
    Src[6] = a1[10];
    Src[7] = a1[11];
    Src[8] = a1[16];
    Src[0] = v4;
    if ( (v5 & 8) != 0 )
      v7 = a1[15];
    else
      v7 = 0;
    memcpy_0(a3, Src, v4);
  }
  return 0;
}

// ===== sub_4BF0E0 @ 0x004BF0E0..0x004BF267 =====
int __stdcall sub_4BF0E0(int a1, unsigned int a2, unsigned int *a3)
{
  unsigned int v3; // ebx
  unsigned int v5; // eax
  unsigned int v6; // eax
  signed int v7; // eax
  int v8; // [esp+Ch] [ebp-4h]

  v3 = a2;
  v8 = 0;
  if ( a2 < 4 )
    return 0;
  if ( !a3 )
    return -2147467261;
  if ( *a3 < a2 )
    v3 = *a3;
  if ( v3 > 0x30 )
    return -2147024809;
  if ( *a3 > 0x30 )
    return -2147024809;
  if ( v3 >= 0xC )
  {
    v5 = a3[2];
    if ( (v5 & 0xFFFFFE20) != 0 || (v5 & 0x10) != 0 && (*(_BYTE *)(a1 + 4) & 0x10) == 0 && v3 < 0x20 )
      return -2147024809;
  }
  if ( v3 >= 0x2C )
  {
    v6 = a3[10];
    if ( v6 )
    {
      if ( v6 != *(_DWORD *)(a1 + 12) )
        return -2147024809;
    }
  }
  if ( v3 >= 0x30 )
  {
    v7 = a3[11];
    if ( v7 )
    {
      if ( v7 != *(_DWORD *)(a1 + 20) )
        return -2147024809;
    }
    if ( v7 < (int)a3[3] )
      return -2147024809;
  }
  if ( v3 >= 0x28 && (a3[2] & 8) != 0 )
  {
    if ( !a3[9] )
      return -2147467261;
    v8 = sub_4C0B20(a3[9]);
    if ( !v8 )
      return -2147024882;
  }
  if ( v3 >= 0x24 )
    *(_DWORD *)(a1 + 64) = a3[8];
  if ( v3 < 0xC )
  {
    if ( v3 >= 8 )
      *(_DWORD *)(a1 + 8) = a3[1];
  }
  else
  {
    *(_DWORD *)(a1 + 4) = a3[2] | *(_DWORD *)(a1 + 4) & 0x20;
    *(_DWORD *)(a1 + 8) = a3[1];
  }
  if ( v3 >= 0x10 )
    *(_DWORD *)(a1 + 16) = a3[3];
  if ( v3 >= 0x20 )
  {
    *(_DWORD *)(a1 + 40) = a3[6];
    *(_DWORD *)(a1 + 44) = a3[7];
  }
  if ( v3 >= 0x18 )
  {
    *(_DWORD *)(a1 + 32) = a3[4];
    *(_DWORD *)(a1 + 36) = a3[5];
  }
  if ( v3 >= 0x28 && (a3[2] & 8) != 0 )
  {
    if ( *(_DWORD *)(a1 + 60) )
      sub_4C0B00(*(LPVOID *)(a1 + 60));
    *(_DWORD *)(a1 + 60) = v8;
  }
  if ( *(_DWORD *)(a1 + 60) )
  {
    *(_DWORD *)(a1 + 4) |= 8u;
    return 0;
  }
  *(_DWORD *)(a1 + 4) &= ~8u;
  return 0;
}

// ===== sub_4BF270 @ 0x004BF270..0x004BF278 =====
int __stdcall sub_4BF270(int a1, int a2, int a3)
{
  return -2147467263;
}

// ===== sub_4BF280 @ 0x004BF280..0x004BF2F8 =====
HMODULE __thiscall sub_4BF280(int this)
{
  void *v2; // eax

  v2 = *(void **)(this + 48);
  if ( v2 )
    CloseHandle(v2);
  if ( *(_DWORD *)(this + 88) )
    (*(void (__stdcall **)(_DWORD))(**(_DWORD **)(this + 88) + 8))(*(_DWORD *)(this + 88));
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 16));
  return sub_4BCEE0();
}

// ===== sub_4BF300 @ 0x004BF300..0x004BF36F =====
int __stdcall sub_4BF300(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !sub_445510(a2, &stru_4DB974) && (!sub_445510(a2, dword_4DB984) || !*(_DWORD *)(a1 + 92)) )
    return sub_4BCF90(a1, a2, a3);
  if ( a1 )
    return sub_4BCF30(a1 + 12, a3);
  return sub_4BCF30(0, a3);
}

// ===== sub_4BF370 @ 0x004BF370..0x004BF3C6 =====
int __stdcall sub_4BF370(_DWORD *a1, _DWORD *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // ebx

  if ( !a2 )
    return -2147467261;
  if ( a1 == (_DWORD *)12 )
    v3 = 0;
  else
    v3 = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(v3);
  a2[1] = a1[13];
  *a2 = a1[11];
  a2[2] = a1[14];
  a2[3] = a1[15];
  LeaveCriticalSection(v3);
  return 0;
}

// ===== sub_4BF3D0 @ 0x004BF3D0..0x004BF465 =====
int __stdcall sub_4BF3D0(int a1, _DWORD *a2, int a3, int a4, char a5)
{
  struct _RTL_CRITICAL_SECTION *v5; // ebx
  int v6; // edi
  int v7; // ecx

  *a2 = 0;
  while ( 1 )
  {
    if ( a1 == 12 )
      v5 = 0;
    else
      v5 = (struct _RTL_CRITICAL_SECTION *)(a1 + 4);
    EnterCriticalSection(v5);
    if ( !*(_DWORD *)(a1 + 68) )
    {
      LeaveCriticalSection(v5);
      return -2147220975;
    }
    v6 = *(_DWORD *)(a1 + 28);
    if ( v6 )
    {
      v7 = *(_DWORD *)(v6 + 28);
      --*(_DWORD *)(a1 + 32);
      *(_DWORD *)(a1 + 28) = v7;
    }
    else
    {
      ++*(_DWORD *)(a1 + 40);
    }
    LeaveCriticalSection(v5);
    if ( v6 )
    {
      *(_DWORD *)(v6 + 68) = 1;
      *a2 = v6;
      return 0;
    }
    if ( (a5 & 4) != 0 )
      break;
    WaitForSingleObject(*(HANDLE *)(a1 + 36), 0xFFFFFFFF);
  }
  return -2147220946;
}

// ===== sub_4BF470 @ 0x004BF470..0x004BF4F5 =====
int __stdcall sub_4BF470(int a1, int a2)
{
  struct _RTL_CRITICAL_SECTION *v2; // esi
  int v3; // eax

  if ( a1 == 12 )
    v2 = 0;
  else
    v2 = (struct _RTL_CRITICAL_SECTION *)(a1 + 4);
  EnterCriticalSection(v2);
  if ( a2 )
    (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 4))(a2);
  v3 = *(_DWORD *)(a1 + 76);
  if ( v3 )
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v3 + 8))(*(_DWORD *)(a1 + 76));
  *(_DWORD *)(a1 + 76) = a2;
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4BF500 @ 0x004BF500..0x004BF53A =====
int __stdcall sub_4BF500(_DWORD *a1, _DWORD *a2)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi

  if ( a1 == (_DWORD *)12 )
    v2 = 0;
  else
    v2 = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(v2);
  *a2 = a1[8] + a1[11] - a1[12];
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4BF540 @ 0x004BF540..0x004BF560 =====
LONG __thiscall sub_4BF540(int this)
{
  LONG result; // eax

  result = *(_DWORD *)(this + 52);
  if ( result )
  {
    result = ReleaseSemaphore(*(HANDLE *)(this + 48), result, 0);
    *(_DWORD *)(this + 52) = 0;
  }
  return result;
}

// ===== sub_4BF560 @ 0x004BF560..0x004BF615 =====
int __stdcall sub_4BF560(_DWORD *a1)
{
  struct _RTL_CRITICAL_SECTION *v1; // edi
  int v2; // ebx

  if ( a1 == (_DWORD *)12 )
    v1 = 0;
  else
    v1 = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(v1);
  if ( !a1[17] )
  {
    a1[17] = 1;
    if ( a1[18] )
    {
      a1[18] = 0;
    }
    else
    {
      v2 = (*(int (__thiscall **)(_DWORD *))(*(a1 - 3) + 20))(a1 - 3);
      if ( v2 < 0 )
      {
        a1[17] = 0;
        LeaveCriticalSection(v1);
        return v2;
      }
      (*(void (__stdcall **)(_DWORD *))(*a1 + 4))(a1);
    }
  }
  LeaveCriticalSection(v1);
  return 0;
}

// ===== sub_4BF620 @ 0x004BF620..0x004BF6D9 =====
int __stdcall sub_4BF620(_DWORD *a1)
{
  int v1; // ebx
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // eax
  void (__thiscall *v4)(_DWORD *); // eax

  v1 = 0;
  if ( a1 == (_DWORD *)12 )
    v2 = 0;
  else
    v2 = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(v2);
  if ( a1[17] || a1[18] )
  {
    v3 = a1[8];
    a1[17] = 0;
    if ( v3 >= a1[12] )
    {
      v4 = *(void (__thiscall **)(_DWORD *))(*(a1 - 3) + 16);
      a1[18] = 0;
      v4(a1 - 3);
      v1 = 1;
    }
    else
    {
      a1[18] = 1;
    }
    sub_4BF540((int)(a1 - 3));
    LeaveCriticalSection(v2);
    if ( v1 )
      (*(void (__stdcall **)(_DWORD *))(*a1 + 8))(a1);
  }
  else
  {
    LeaveCriticalSection(v2);
  }
  return 0;
}

// ===== sub_4BF6E0 @ 0x004BF6E0..0x004BF701 =====
int __thiscall sub_4BF6E0(int *this)
{
  if ( this[14] <= 0 || this[16] <= 0 || this[17] <= 0 )
    return -2147220974;
  else
    return this[19] == 0;
}

// ===== sub_4BF710 @ 0x004BF710..0x004BF82C =====
int __stdcall sub_4BF710(_DWORD *a1, int a2, LPCRITICAL_SECTION lpCriticalSection)
{
  int v6; // ebx
  LONG v7; // eax
  struct _RTL_CRITICAL_SECTION_DEBUG *v8; // eax
  LONG v9; // eax
  void *v10; // ecx
  struct _SYSTEM_INFO SystemInfo; // [esp+4h] [ebp-24h] BYREF
  int v12; // [esp+30h] [ebp+8h]
  struct _RTL_CRITICAL_SECTION *lpCriticalSectiona; // [esp+38h] [ebp+10h]

  if ( !lpCriticalSection )
    return -2147467261;
  if ( a1 == (_DWORD *)12 )
    lpCriticalSectiona = 0;
  else
    lpCriticalSectiona = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(lpCriticalSectiona);
  lpCriticalSection->DebugInfo = 0;
  lpCriticalSection->LockCount = 0;
  lpCriticalSection->RecursionCount = 0;
  lpCriticalSection->OwningThread = 0;
  GetSystemInfo(&SystemInfo);
  v6 = *(_DWORD *)(a2 + 8);
  if ( !v6 || ((v6 - 1) & SystemInfo.dwAllocationGranularity) != 0 )
  {
    LeaveCriticalSection(lpCriticalSectiona);
    return -2147220978;
  }
  else if ( a1[17] == 1 )
  {
    LeaveCriticalSection(lpCriticalSectiona);
    return -2147220977;
  }
  else if ( a1[8] >= a1[12] )
  {
    v12 = *(_DWORD *)(a2 + 12) + *(_DWORD *)(a2 + 4);
    if ( v12 % v6 )
      v12 += v6 - v12 % v6;
    v7 = v12 - *(_DWORD *)(a2 + 12);
    a1[13] = v7;
    lpCriticalSection->LockCount = v7;
    v8 = *(struct _RTL_CRITICAL_SECTION_DEBUG **)a2;
    a1[11] = *(_DWORD *)a2;
    lpCriticalSection->DebugInfo = v8;
    v9 = *(_DWORD *)(a2 + 8);
    a1[14] = v9;
    lpCriticalSection->RecursionCount = v9;
    v10 = *(void **)(a2 + 12);
    a1[15] = v10;
    lpCriticalSection->OwningThread = v10;
    a1[16] = 1;
    LeaveCriticalSection(lpCriticalSectiona);
    return 0;
  }
  else
  {
    LeaveCriticalSection(lpCriticalSectiona);
    return -2147220976;
  }
}

// ===== sub_4BF830 @ 0x004BF830..0x004BF873 =====
void *__thiscall sub_4BF830(_DWORD *this)
{
  _DWORD *v2; // ecx
  int v3; // eax
  void *result; // eax

  while ( 1 )
  {
    v2 = (_DWORD *)this[10];
    if ( !v2 )
      break;
    v3 = v2[7];
    --this[11];
    this[10] = v3;
    (*(void (__thiscall **)(_DWORD *, int))(*v2 + 84))(v2, 1);
  }
  result = (void *)this[24];
  this[15] = 0;
  if ( result )
  {
    result = (void *)VirtualFree(result, 0, 0x8000u);
    this[24] = 0;
  }
  return result;
}

// ===== sub_4BF880 @ 0x004BF880..0x004BF8EA =====
HMODULE __thiscall sub_4BF880(_DWORD *this)
{
  *this = &CMemAllocator::`vftable';
  this[3] = &CMemAllocator::`vftable';
  sub_4BF620(this + 3);
  sub_4BF830(this);
  return sub_4BF280((int)this);
}

// ===== sub_4BF8F0 @ 0x004BF8F0..0x004BF905 =====
int __stdcall sub_4BF8F0(int a1)
{
  return (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(a1 - 8) + 8))(*(_DWORD *)(a1 - 8));
}

// ===== sub_4BF910 @ 0x004BF910..0x004BFAFC =====
int __stdcall sub_4BF910(_DWORD **a1, int *a2, int a3)
{
  int *v5; // edi
  int v6; // ecx
  int v7; // ebx
  _DWORD *v8; // eax
  int v9; // ebx
  unsigned int v10; // eax
  _DWORD *v11; // edx
  int v12; // ebx
  _DWORD *v13; // eax
  int v14; // [esp+Ch] [ebp-8h]
  unsigned int v15; // [esp+10h] [ebp-4h]
  _DWORD *v16; // [esp+1Ch] [ebp+8h]
  unsigned int v17; // [esp+1Ch] [ebp+8h]
  int v18; // [esp+24h] [ebp+10h]

  if ( !a1 )
    return 1;
  v5 = a2;
  v6 = (*(int (__stdcall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD))(*a2 + 28))(a2, **a1, (*a1)[1], (*a1)[2], (*a1)[3]);
  if ( a3 )
  {
    v6 = (*(int (__stdcall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD *, _DWORD *))(*a2 + 12))(
           a2,
           **a1,
           (*a1)[1],
           (*a1)[2],
           (*a1)[3],
           a1[1],
           a1[2]);
    v7 = 0;
    if ( v6 >= 0 )
    {
      v15 = 0;
      if ( a1[3] )
      {
        v18 = 0;
        do
        {
          v8 = (_DWORD *)((char *)a1[4] + v7);
          v16 = (_DWORD *)v8[5];
          v6 = (*(int (__stdcall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD))(*v5 + 20))(
                 v5,
                 **a1,
                 (*a1)[1],
                 (*a1)[2],
                 (*a1)[3],
                 *v8,
                 v8[1],
                 v8[2],
                 v8[3],
                 v8[4],
                 *v16,
                 v16[1],
                 v16[2],
                 v16[3],
                 v8[6]);
          if ( v6 < 0 )
            break;
          v9 = v18;
          v10 = 0;
          v11 = (_DWORD *)((char *)a1[4] + v18);
          v17 = 0;
          if ( v11[7] )
          {
            do
            {
              v12 = *v5;
              v14 = v11[8] + 8 * v10;
              v13 = *(_DWORD **)(v14 + 4);
              v5 = a2;
              v6 = (*(int (__stdcall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD))(v12 + 24))(
                     a2,
                     **a1,
                     (*a1)[1],
                     (*a1)[2],
                     (*a1)[3],
                     *v11,
                     **(_DWORD **)v14,
                     *(_DWORD *)(*(_DWORD *)v14 + 4),
                     *(_DWORD *)(*(_DWORD *)v14 + 8),
                     *(_DWORD *)(*(_DWORD *)v14 + 12),
                     *v13,
                     v13[1],
                     v13[2],
                     v13[3]);
              if ( v6 < 0 )
                return v6 != -2147024894 ? v6 : 0;
              v9 = v18;
              v10 = v17 + 1;
              v11 = (_DWORD *)((char *)a1[4] + v18);
              v17 = v10;
            }
            while ( v10 < v11[7] );
          }
          v7 = v9 + 36;
          ++v15;
          v18 = v7;
        }
        while ( v15 < (unsigned int)a1[3] );
      }
    }
  }
  return v6 != -2147024894 ? v6 : 0;
}

// ===== sub_4BFB00 @ 0x004BFB00..0x004BFB5A =====
_DWORD *__thiscall sub_4BFB00(_DWORD *this, int a2, _DWORD *a3, int a4, _DWORD *a5, int a6)
{
  sub_4BCF60(this, a2, a3);
  this[5] = 0;
  this[6] = 0;
  this[8] = 0;
  this[9] = 0;
  this[10] = *a5;
  this[11] = a5[1];
  this[12] = a5[2];
  this[13] = a5[3];
  this[14] = a4;
  this[15] = 0;
  this[16] = 0;
  this[17] = 0;
  this[18] = 1;
  return this;
}

// ===== sub_4BFB60 @ 0x004BFB60..0x004BFBCE =====
int __stdcall sub_4BFB60(char *ppv)
{
  _DWORD **v1; // esi

  v1 = (_DWORD **)(*(int (__thiscall **)(char *))(*((_DWORD *)ppv - 4) + 32))(ppv - 16);
  if ( !v1 )
    return 1;
  CoInitialize(0);
  if ( CoCreateInstance(&stru_4DB844, 0, 1u, &stru_4DB9C4, (LPVOID *)&ppv) >= 0 )
  {
    sub_4BF910(v1, (int *)ppv, 1);
    (*(void (__stdcall **)(char *))(*(_DWORD *)ppv + 8))(ppv);
  }
  CoFreeUnusedLibraries();
  CoUninitialize();
  return 0;
}

// ===== sub_4BFBD0 @ 0x004BFBD0..0x004BFC4F =====
HRESULT __stdcall sub_4BFBD0(char *ppv)
{
  _DWORD **v1; // esi
  HRESULT Instance; // edi

  v1 = (_DWORD **)(*(int (__thiscall **)(char *))(*((_DWORD *)ppv - 4) + 32))(ppv - 16);
  if ( !v1 )
    return 1;
  CoInitialize(0);
  Instance = CoCreateInstance(&stru_4DB844, 0, 1u, &stru_4DB9C4, (LPVOID *)&ppv);
  if ( Instance >= 0 )
  {
    Instance = sub_4BF910(v1, (int *)ppv, 0);
    (*(void (__stdcall **)(char *))(*(_DWORD *)ppv + 8))(ppv);
  }
  CoFreeUnusedLibraries();
  CoUninitialize();
  return Instance != -2147024894 ? Instance : 0;
}

// ===== sub_4BFC50 @ 0x004BFC50..0x004BFD11 =====
_DWORD *__thiscall sub_4BFC50(_DWORD *this, int a2, _DWORD *a3)
{
  int v4; // eax
  int v5; // ecx

  *this = &CEnumPins::`vftable';
  this[1] = 0;
  this[2] = 0;
  this[3] = a2;
  this[5] = 1;
  sub_4C3AC0(0);
  (*(void (__stdcall **)(int))(*(_DWORD *)(this[3] + 12) + 4))(this[3] + 12);
  if ( a3 )
  {
    this[1] = a3[1];
    this[2] = a3[2];
    this[4] = a3[4];
    sub_4C3C90(a3 + 6);
  }
  else
  {
    v4 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)this[3] + 20))(this[3]);
    v5 = this[3];
    this[4] = v4;
    this[2] = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
  }
  return this;
}

// ===== sub_4BFD20 @ 0x004BFD20..0x004BFD41 =====
_DWORD *__thiscall sub_4BFD20(_DWORD *this, char a2)
{
  sub_4BD8D0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4BFD50 @ 0x004BFD50..0x004BFDFB =====
int __stdcall sub_4BFD50(_DWORD *a1, _DWORD *a2)
{
  int v3; // edi
  _DWORD *v4; // eax
  _DWORD *v5; // eax

  if ( !a2 )
    return -2147467261;
  v3 = 0;
  if ( (*(int (__thiscall **)(_DWORD))(*(_DWORD *)a1[3] + 20))(a1[3]) == a1[4] )
  {
    v4 = operator new(0x30u);
    if ( v4 )
      v5 = sub_4BFC50(v4, a1[3], a1);
    else
      v5 = 0;
    *a2 = v5;
    if ( !v5 )
      return -2147024882;
  }
  else
  {
    *a2 = 0;
    return -2147220989;
  }
  return v3;
}

// ===== sub_4BFE00 @ 0x004BFE00..0x004BFF05 =====
int __userpurge sub_4BFE00@<eax>(int a1@<edi>, int a2, unsigned int a3, _DWORD *a4, _DWORD *a5)
{
  int v7; // eax
  int v8; // ecx
  int v9; // eax
  int v10; // esi
  int v12; // [esp+0h] [ebp-4h]
  int v13; // [esp+Ch] [ebp+8h]

  if ( !a4 )
    return -2147467261;
  if ( a5 )
  {
    *a5 = 0;
  }
  else if ( a3 > 1 )
  {
    return -2147024809;
  }
  v12 = 0;
  if ( (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(a2 + 12) + 20))(*(_DWORD *)(a2 + 12), a1) != *(_DWORD *)(a2 + 16) )
    sub_4BDA80(a2);
  v13 = *(_DWORD *)(a2 + 8) - *(_DWORD *)(a2 + 4);
  if ( v13 >= (int)a3 )
    v13 = a3;
  if ( !v13 )
    return 1;
  do
  {
    v7 = *(_DWORD *)(a2 + 4);
    if ( *(_DWORD *)(a2 + 8) == v7 )
      break;
    v8 = *(_DWORD *)(a2 + 12);
    *(_DWORD *)(a2 + 4) = v7 + 1;
    v9 = (*(int (__thiscall **)(int, int))(*(_DWORD *)v8 + 28))(v8, v7);
    v10 = v9;
    if ( !v9 )
      return -2147220989;
    if ( !sub_4C3B70(v9) )
    {
      *a4 = v10 + 12;
      (*(void (__stdcall **)(int))(*(_DWORD *)(v10 + 12) + 4))(v10 + 12);
      ++v12;
      ++a4;
      sub_4C3C20(v10);
      --v13;
    }
  }
  while ( v13 );
  if ( a5 )
    *a5 = v12;
  return a3 != v12;
}

// ===== sub_4BFF10 @ 0x004BFF10..0x004BFF69 =====
_DWORD *__thiscall sub_4BFF10(_DWORD *this, int a2, int a3)
{
  this[2] = a2;
  *this = &CEnumMediaTypes::`vftable';
  this[1] = 0;
  this[4] = 1;
  (*(void (__stdcall **)(int))(*(_DWORD *)(a2 + 12) + 4))(a2 + 12);
  if ( a3 )
  {
    this[1] = *(_DWORD *)(a3 + 4);
    this[3] = *(_DWORD *)(a3 + 12);
  }
  else
  {
    this[3] = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)this[2] + 16))(this[2]);
  }
  return this;
}

// ===== sub_4BFF70 @ 0x004BFF70..0x004BFF91 =====
_DWORD *__thiscall sub_4BFF70(_DWORD *this, char a2)
{
  sub_4BDAB0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4BFFA0 @ 0x004BFFA0..0x004C004B =====
int __stdcall sub_4BFFA0(int a1, _DWORD *a2)
{
  int v3; // edi
  _DWORD *v4; // eax
  _DWORD *v5; // eax

  if ( !a2 )
    return -2147467261;
  v3 = 0;
  if ( (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 8) + 16))(*(_DWORD *)(a1 + 8)) == *(_DWORD *)(a1 + 12) )
  {
    v4 = operator new(0x14u);
    if ( v4 )
      v5 = sub_4BFF10(v4, *(_DWORD *)(a1 + 8), a1);
    else
      v5 = 0;
    *a2 = v5;
    if ( !v5 )
      return -2147024882;
  }
  else
  {
    *a2 = 0;
    return -2147220989;
  }
  return v3;
}
