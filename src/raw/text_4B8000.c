#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== __aullshr @ 0x004B8090..0x004B80AF =====
unsigned __int64 __usercall _aullshr@<edx:eax>(unsigned __int64 a1@<edx:eax>, unsigned __int8 a2@<cl>)
{
  if ( a2 >= 0x40u )
    return 0LL;
  else
    return a1 >> a2;
}

// ===== __fptrap @ 0x004B80AF..0x004B80B6 =====
void __noreturn _fptrap()
{
  _amsg_exit(2);
}

// ===== __controlfp_s @ 0x004B80B8..0x004B8117 =====
errno_t __cdecl _controlfp_s(unsigned int *CurrentState, unsigned int NewValue, unsigned int Mask)
{
  unsigned int v4; // [esp-4h] [ebp-8h]

  if ( (Mask & 0xFFF7FFFF & NewValue & 0xFCF0FCE0) != 0 )
  {
    if ( CurrentState )
      *CurrentState = _control87(0, 0);
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  else
  {
    v4 = Mask & 0xFFF7FFFF;
    if ( CurrentState )
      *CurrentState = _control87(NewValue, v4);
    else
      _control87(NewValue, v4);
    return 0;
  }
}

// ===== __87except @ 0x004B8117..0x004B8256 =====
void __usercall _87except(int a1@<ebp>, int a2, int a3, __int16 *a4)
{
  __int16 v4; // cx
  bool v5; // zf
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  unsigned int v12; // [esp-88h] [ebp-94h] BYREF
  DWORD v13; // [esp-84h] [ebp-90h]
  double v14[8]; // [esp-80h] [ebp-8Ch] BYREF
  unsigned int v15; // [esp-40h] [ebp-4Ch]
  unsigned int v16; // [esp-4h] [ebp-10h]
  _DWORD v17[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v17[0] = a1;
  v17[1] = retaddr;
  v16 = (unsigned int)v17 ^ dword_4FB734;
  v4 = *a4;
  v6 = *(_DWORD *)a3 - 1;
  v5 = *(_DWORD *)a3 == 1;
  v12 = (unsigned __int16)*a4;
  if ( v5 )
    goto LABEL_13;
  v7 = v6 - 1;
  if ( !v7 )
  {
    v13 = 4;
    goto LABEL_14;
  }
  v8 = v7 - 1;
  if ( !v8 )
  {
    v13 = 17;
    goto LABEL_14;
  }
  v9 = v8 - 1;
  if ( !v9 )
  {
    v13 = 18;
    goto LABEL_14;
  }
  v10 = v9 - 1;
  if ( !v10 )
  {
LABEL_13:
    v13 = 8;
LABEL_14:
    if ( !_handle_exc(v13, (double *)(a3 + 24), v4) )
    {
      if ( a2 == 16 || a2 == 22 || a2 == 29 )
      {
        v14[6] = *(double *)(a3 + 16);
        v15 = v15 & 0xFFFFFFE0 | 3;
      }
      else
      {
        v15 &= ~1u;
      }
      _raise_exc((ULONG_PTR)v14, &v12, v13, a2, (float *)(a3 + 8), (float *)(a3 + 24));
    }
    goto LABEL_21;
  }
  v11 = v10 - 2;
  if ( !v11 )
  {
    *(_DWORD *)a3 = 1;
    goto LABEL_21;
  }
  if ( v11 == 1 )
  {
    v13 = 16;
    goto LABEL_14;
  }
LABEL_21:
  _ctrlfp(v4);
  if ( *(_DWORD *)a3 == 8 || dword_4FC110 || !sub_4B4D09(a3) )
    _set_errno_from_matherr(*(_DWORD *)a3);
  sub_4AB245((void *)((unsigned int)v17 ^ v16));
}

// ===== __copysign @ 0x004B8256..0x004B827E =====
double __cdecl _copysign(double Number, double Sign)
{
  double v3; // [esp+0h] [ebp-8h]

  LODWORD(v3) = LODWORD(Number);
  HIDWORD(v3) = HIDWORD(Sign) ^ (HIDWORD(Number) ^ HIDWORD(Sign)) & 0x7FFFFFFF;
  return v3;
}

// ===== __fpclass @ 0x004B827E..0x004B831B =====
int __cdecl _fpclass(double X)
{
  int v1; // eax
  int v2; // eax
  int v4; // ecx

  if ( (HIWORD(X) & 0x7FF0) == 0x7FF0 )
  {
    v1 = _sptype(SLODWORD(X), SHIDWORD(X)) - 1;
    if ( v1 )
    {
      v2 = v1 - 1;
      if ( !v2 )
        return 4;
      if ( v2 != 1 )
        return 1;
      return 2;
    }
    else
    {
      return 512;
    }
  }
  else
  {
    v4 = HIWORD(X) & 0x8000;
    if ( (HIWORD(X) & 0x7FF0) == 0 && ((HIDWORD(X) & 0xFFFFF) != 0 || LODWORD(X)) )
    {
      return v4 != 0 ? 16 : 128;
    }
    else if ( 0.0 == X )
    {
      return v4 != 0 ? 32 : 64;
    }
    else
    {
      return v4 != 0 ? 8 : 256;
    }
  }
}

// ===== __free_osfhnd @ 0x004B831B..0x004B83A1 =====
int __cdecl _free_osfhnd(int a1)
{
  int *v1; // edi
  int v2; // esi

  if ( a1 < 0
    || a1 >= uNumber
    || (v1 = &dword_567B00[a1 >> 5], v2 = (a1 & 0x1F) << 6, (*(_BYTE *)(*v1 + v2 + 4) & 1) == 0)
    || *(_DWORD *)(*v1 + ((a1 & 0x1F) << 6)) == -1 )
  {
    *_errno() = 9;
    *__doserrno() = 0;
    return -1;
  }
  else
  {
    if ( dword_4FB730 == 1 )
    {
      if ( a1 )
      {
        if ( a1 == 1 )
        {
          SetStdHandle(0xFFFFFFF5, 0);
        }
        else if ( a1 == 2 )
        {
          SetStdHandle(0xFFFFFFF4, 0);
        }
      }
      else
      {
        SetStdHandle(0xFFFFFFF6, 0);
      }
    }
    *(_DWORD *)(v2 + *v1) = -1;
    return 0;
  }
}

// ===== __get_osfhandle @ 0x004B83A1..0x004B840A =====
intptr_t __cdecl _get_osfhandle(int FileHandle)
{
  int v2; // ecx
  int v3; // eax

  if ( FileHandle == -2 )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    return -1;
  }
  if ( FileHandle < 0
    || FileHandle >= uNumber
    || (v2 = dword_567B00[FileHandle >> 5], v3 = (FileHandle & 0x1F) << 6, (*(_BYTE *)(v3 + v2 + 4) & 1) == 0) )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return -1;
  }
  return *(_DWORD *)(v3 + v2);
}

// ===== ___lock_fhandle @ 0x004B840A..0x004B84A9 =====
BOOL __cdecl __lock_fhandle(int a1)
{
  int v1; // esi
  BOOL v3; // [esp+10h] [ebp-1Ch]

  v1 = dword_567B00[a1 >> 5] + ((a1 & 0x1F) << 6);
  v3 = 1;
  if ( !*(_DWORD *)(v1 + 8) )
  {
    _lock(10);
    if ( !*(_DWORD *)(v1 + 8) )
    {
      v3 = InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)(v1 + 12), 0xFA0u);
      ++*(_DWORD *)(v1 + 8);
    }
    _unlock(10);
  }
  if ( v3 )
    EnterCriticalSection((LPCRITICAL_SECTION)(dword_567B00[a1 >> 5] + ((a1 & 0x1F) << 6) + 12));
  return v3;
}

// ===== __unlock_fhandle @ 0x004B84A9..0x004B84D0 =====
void __cdecl _unlock_fhandle(int a1)
{
  LeaveCriticalSection((LPCRITICAL_SECTION)(dword_567B00[a1 >> 5] + ((a1 & 0x1F) << 6) + 12));
}

// ===== __putwch_nolock @ 0x004B84D0..0x004B8512 =====
wint_t __cdecl _putwch_nolock(wchar_t Character)
{
  DWORD v1; // ecx
  DWORD NumberOfCharsWritten; // [esp+0h] [ebp-4h] BYREF

  NumberOfCharsWritten = v1;
  if ( hConsoleOutput == (HANDLE)-2 )
    __initconout(NumberOfCharsWritten);
  if ( hConsoleOutput == (HANDLE)-1 || !WriteConsoleW(hConsoleOutput, &Character, 1u, &NumberOfCharsWritten, 0) )
    return -1;
  else
    return Character;
}

// ===== __mbtowc_l @ 0x004B8512..0x004B8628 =====
int __cdecl _mbtowc_l(wchar_t *DstCh, const char *SrcCh, size_t SrcSizeInBytes, _locale_t Locale)
{
  int result; // eax
  struct __crt_locale_data *locinfo; // eax
  int v6; // ecx
  bool v7; // zf
  __crt_locale_pointers Localea; // [esp+8h] [ebp-10h] BYREF
  int v9; // [esp+10h] [ebp-8h]
  char v10; // [esp+14h] [ebp-4h]

  if ( !SrcCh || !SrcSizeInBytes )
    return 0;
  if ( !*SrcCh )
  {
    if ( DstCh )
      *DstCh = 0;
    return 0;
  }
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Localea, (struct localeinfo_struct *)Locale);
  if ( !*((_DWORD *)Localea.locinfo + 5) )
  {
    if ( DstCh )
      *DstCh = *(unsigned __int8 *)SrcCh;
    goto LABEL_11;
  }
  if ( _isleadbyte_l(*(unsigned __int8 *)SrcCh, &Localea) )
  {
    locinfo = Localea.locinfo;
    v6 = *((_DWORD *)Localea.locinfo + 43);
    if ( v6 > 1
      && (int)SrcSizeInBytes >= v6
      && (v7 = MultiByteToWideChar(*((_DWORD *)Localea.locinfo + 1), 9u, SrcCh, v6, DstCh, DstCh != 0) == 0,
          locinfo = Localea.locinfo,
          !v7)
      || SrcSizeInBytes >= *((_DWORD *)locinfo + 43) && SrcCh[1] )
    {
      result = *((_DWORD *)locinfo + 43);
      if ( v10 )
        *(_DWORD *)(v9 + 112) &= ~2u;
      return result;
    }
  }
  else if ( MultiByteToWideChar(*((_DWORD *)Localea.locinfo + 1), 9u, SrcCh, 1, DstCh, DstCh != 0) )
  {
LABEL_11:
    if ( v10 )
      *(_DWORD *)(v9 + 112) &= ~2u;
    return 1;
  }
  *_errno() = 42;
  if ( v10 )
    *(_DWORD *)(v9 + 112) &= ~2u;
  return -1;
}

// ===== _mbtowc @ 0x004B8628..0x004B8642 =====
int __cdecl mbtowc(wchar_t *DstCh, const char *SrcCh, size_t SrcSizeInBytes)
{
  return _mbtowc_l(DstCh, SrcCh, SrcSizeInBytes, 0);
}

// ===== __fcloseall @ 0x004B8642..0x004B86DE =====
int __cdecl _fcloseall()
{
  int i; // edi
  int v1; // eax
  int v3; // [esp+14h] [ebp-1Ch]

  v3 = 0;
  _lock(1);
  for ( i = 3; i < dword_567AE0; ++i )
  {
    if ( *((_DWORD *)dword_566AD0 + i) )
    {
      v1 = *((_DWORD *)dword_566AD0 + i);
      if ( (*(_BYTE *)(v1 + 12) & 0x83) != 0 && fclose((FILE *)v1) != -1 )
        ++v3;
      if ( i >= 20 )
      {
        DeleteCriticalSection((LPCRITICAL_SECTION)(*((_DWORD *)dword_566AD0 + i) + 32));
        free(*((void **)dword_566AD0 + i));
        *((_DWORD *)dword_566AD0 + i) = 0;
      }
    }
  }
  _unlock(1);
  return v3;
}

// ===== __flush @ 0x004B86DE..0x004B8746 =====
int __cdecl _flush(FILE *Stream)
{
  int flag; // eax
  int v2; // ebx
  char *base; // eax
  char *v4; // edi
  int v5; // eax
  int v6; // eax
  char *v7; // eax
  char *v9; // [esp-Ch] [ebp-14h]
  char *v10; // [esp-8h] [ebp-10h]

  flag = Stream->_flag;
  v2 = 0;
  if ( (flag & 3) == 2 && (flag & 0x108) != 0 )
  {
    base = Stream->_base;
    v4 = (char *)(Stream->_ptr - base);
    if ( (int)v4 > 0 )
    {
      v10 = (char *)(Stream->_ptr - base);
      v9 = Stream->_base;
      v5 = _fileno(Stream);
      if ( (char *)_write(v5, v9, (unsigned int)v10) == v4 )
      {
        v6 = Stream->_flag;
        if ( (v6 & 0x80u) != 0 )
          Stream->_flag = v6 & 0xFFFFFFFD;
      }
      else
      {
        Stream->_flag |= 0x20u;
        v2 = -1;
      }
    }
  }
  v7 = Stream->_base;
  Stream->_cnt = 0;
  Stream->_ptr = v7;
  return v2;
}

// ===== __fflush_nolock @ 0x004B8746..0x004B878E =====
int __cdecl _fflush_nolock(FILE *Stream)
{
  int v2; // eax

  if ( !Stream )
    return flsall(0);
  if ( _flush(Stream) )
    return -1;
  if ( (Stream->_flag & 0x4000) == 0 )
    return 0;
  v2 = _fileno(Stream);
  return -(_commit(v2) != 0);
}

// ===== _flsall @ 0x004B878E..0x004B8868 =====
int __cdecl flsall(int a1)
{
  int i; // esi
  int *v2; // eax
  int v3; // eax
  int v4; // ecx
  int result; // eax
  int v6; // [esp+10h] [ebp-24h]
  int v7; // [esp+18h] [ebp-1Ch]

  v7 = 0;
  v6 = 0;
  _lock(1);
  for ( i = 0; i < dword_567AE0; ++i )
  {
    v2 = (int *)((char *)dword_566AD0 + 4 * i);
    if ( *v2 )
    {
      v3 = *v2;
      if ( (*(_BYTE *)(v3 + 12) & 0x83) != 0 )
      {
        _lock_file2(i, v3);
        v4 = *(_DWORD *)(*((_DWORD *)dword_566AD0 + i) + 12);
        if ( (v4 & 0x83) != 0 )
        {
          if ( a1 == 1 )
          {
            if ( _fflush_nolock(*((FILE **)dword_566AD0 + i)) != -1 )
              ++v7;
          }
          else if ( !a1 && (v4 & 2) != 0 && _fflush_nolock(*((FILE **)dword_566AD0 + i)) == -1 )
          {
            v6 = -1;
          }
        }
        _unlock_file2(i, *((_DWORD *)dword_566AD0 + i));
      }
    }
  }
  _unlock(1);
  result = v7;
  if ( a1 != 1 )
    return v6;
  return result;
}

// ===== sub_4B8868 @ 0x004B8868..0x004B8871 =====
int sub_4B8868()
{
  return flsall(1);
}

// ===== sub_4B8871 @ 0x004B8871..0x004B8DC2 =====
int __cdecl sub_4B8871(unsigned __int16 *a1, unsigned int *a2)
{
  unsigned __int16 v2; // bx
  int v3; // ebx
  int v4; // eax
  int v5; // ebx
  int v6; // eax
  int result; // eax
  int v8; // edi
  unsigned int *v9; // esi
  int v10; // eax
  bool i; // zf
  int v12; // eax
  unsigned int v13; // edx
  unsigned int *v14; // ecx
  bool v15; // cf
  unsigned int v16; // edi
  int v17; // eax
  int v18; // edx
  int *v19; // ebx
  int v20; // edx
  unsigned int *v21; // ecx
  int v22; // esi
  int v23; // eax
  unsigned int *v24; // ebx
  bool n; // zf
  int v26; // eax
  unsigned int v27; // edx
  unsigned int *v28; // ecx
  unsigned int v29; // edi
  int ii; // ecx
  unsigned int *v31; // ecx
  unsigned int v32; // esi
  int v33; // edi
  int v34; // eax
  int v35; // edx
  int *v36; // ebx
  int v37; // edx
  unsigned int *v38; // ecx
  int v39; // eax
  int v40; // edx
  int *v41; // ebx
  int v42; // edx
  unsigned int *v43; // ecx
  int v44; // eax
  int v45; // edx
  int v46; // edx
  unsigned int *v47; // ecx
  unsigned int v48; // ebx
  unsigned int v49; // edx
  int v50; // [esp+8h] [ebp-38h]
  char v51; // [esp+10h] [ebp-30h]
  int v52; // [esp+10h] [ebp-30h]
  int v53; // [esp+14h] [ebp-2Ch]
  int v54; // [esp+14h] [ebp-2Ch]
  char v55; // [esp+14h] [ebp-2Ch]
  int v56; // [esp+14h] [ebp-2Ch]
  int v57; // [esp+14h] [ebp-2Ch]
  int v58; // [esp+14h] [ebp-2Ch]
  int v59; // [esp+18h] [ebp-28h]
  int v60; // [esp+18h] [ebp-28h]
  int v61; // [esp+18h] [ebp-28h]
  int v62; // [esp+18h] [ebp-28h]
  int v63; // [esp+18h] [ebp-28h]
  int v64; // [esp+18h] [ebp-28h]
  int v65; // [esp+1Ch] [ebp-24h]
  unsigned int v66; // [esp+20h] [ebp-20h]
  int m; // [esp+20h] [ebp-20h]
  int jj; // [esp+20h] [ebp-20h]
  int k; // [esp+20h] [ebp-20h]
  int j; // [esp+20h] [ebp-20h]
  unsigned int v71; // [esp+24h] [ebp-1Ch]
  unsigned int v72; // [esp+28h] [ebp-18h]
  int v73; // [esp+2Ch] [ebp-14h]
  unsigned int v74; // [esp+30h] [ebp-10h] BYREF
  unsigned int v75; // [esp+34h] [ebp-Ch]
  int v76; // [esp+38h] [ebp-8h] BYREF

  v2 = a1[5];
  v50 = v2 & 0x8000;
  v74 = *(_DWORD *)(a1 + 3);
  v3 = (v2 & 0x7FFF) - 0x3FFF;
  v4 = *a1 << 16;
  v75 = *(_DWORD *)(a1 + 1);
  v76 = v4;
  if ( v3 != -16383 )
  {
    v65 = 0;
    v71 = v74;
    v72 = v75;
    v73 = v76;
    v8 = dword_4FC678 - 1;
    v53 = v3;
    v59 = dword_4FC678 / 32;
    v9 = &v74 + dword_4FC678 / 32;
    v51 = 31 - dword_4FC678 % 32;
    if ( ((1 << v51) & *v9) != 0 )
    {
      v10 = dword_4FC678 / 32;
      for ( i = (~(-1 << (31 - dword_4FC678 % 32)) & *(&v74 + v59)) == 0; i; i = *(&v74 + v10) == 0 )
      {
        if ( ++v10 >= 3 )
          goto LABEL_21;
      }
      v12 = v8 / 32;
      v65 = 0;
      v13 = 1 << (31 - v8 % 32);
      v14 = &v74 + v8 / 32;
      v66 = v13 + *v14;
      if ( v66 >= *v14 )
      {
        v15 = v66 < v13;
        goto LABEL_18;
      }
LABEL_19:
      v65 = 1;
      while ( 1 )
      {
        --v12;
        *v14 = v66;
        if ( v12 < 0 || !v65 )
          break;
        v65 = 0;
        v14 = &v74 + v12;
        v16 = *v14 + 1;
        v66 = v16;
        if ( v16 >= *v14 )
        {
          v15 = v16 == 0;
LABEL_18:
          if ( !v15 )
            continue;
        }
        goto LABEL_19;
      }
    }
LABEL_21:
    *v9 &= -1 << v51;
    if ( v59 + 1 < 3 )
      memset(&v74 + v59 + 1, 0, 4 * (3 - (v59 + 1)));
    if ( v65 )
      ++v3;
    if ( v3 >= dword_4FC674 - dword_4FC678 )
    {
      if ( v3 > dword_4FC674 )
      {
        if ( v3 < dword_4FC670 )
        {
          v5 = dword_4FC684 + v3;
          v74 &= ~0x80000000;
          v44 = dword_4FC67C / 32;
          v45 = dword_4FC67C % 32;
          v64 = 0;
          for ( j = 0; j < 3; ++j )
          {
            v58 = ~(-1 << v45) & *(&v74 + j);
            *(&v74 + j) = v64 | (*(&v74 + j) >> v45);
            v64 = v58 << (32 - v45);
          }
          v46 = 2;
          v47 = (unsigned int *)(&v76 - v44);
          do
          {
            if ( v46 < v44 )
              *(&v74 + v46) = 0;
            else
              *(&v74 + v46) = *v47;
            --v47;
            --v46;
          }
          while ( v46 >= 0 );
          result = 0;
        }
        else
        {
          v75 = 0;
          v76 = 0;
          v74 = 0x80000000;
          v39 = dword_4FC67C / 32;
          v40 = dword_4FC67C % 32;
          v63 = 0;
          for ( k = 0; k < 3; ++k )
          {
            v41 = (int *)(&v74 + k);
            v57 = ~(-1 << v40) & *v41;
            *v41 = v63 | ((unsigned int)*v41 >> v40);
            v63 = v57 << (32 - v40);
          }
          v42 = 2;
          v43 = (unsigned int *)(&v76 - v39);
          do
          {
            if ( v42 < v39 )
              *(&v74 + v42) = 0;
            else
              *(&v74 + v42) = *v43;
            --v43;
            --v42;
          }
          while ( v42 >= 0 );
          v5 = dword_4FC670 + dword_4FC684;
          result = 1;
        }
        goto LABEL_78;
      }
      v74 = v71;
      v75 = v72;
      v17 = (dword_4FC674 - v53) / 32;
      v76 = v73;
      v18 = (dword_4FC674 - v53) % 32;
      v60 = 0;
      for ( m = 0; m < 3; ++m )
      {
        v19 = (int *)(&v74 + m);
        v54 = ~(-1 << v18) & *v19;
        *v19 = v60 | ((unsigned int)*v19 >> v18);
        v60 = v54 << (32 - v18);
      }
      v20 = 2;
      v21 = (unsigned int *)(&v76 - v17);
      do
      {
        if ( v20 < v17 )
          *(&v74 + v20) = 0;
        else
          *(&v74 + v20) = *v21;
        --v21;
        --v20;
      }
      while ( v20 >= 0 );
      v22 = dword_4FC678 - 1;
      v23 = dword_4FC678 / 32;
      v52 = dword_4FC678 / 32;
      v24 = &v74 + dword_4FC678 / 32;
      v55 = 31 - dword_4FC678 % 32;
      if ( ((1 << v55) & *v24) != 0 )
      {
        for ( n = (~(-1 << (31 - dword_4FC678 % 32)) & *(&v74 + v23)) == 0; n; n = *(&v74 + v23) == 0 )
        {
          if ( ++v23 >= 3 )
            goto LABEL_51;
        }
        v26 = v22 / 32;
        v61 = 0;
        v27 = 1 << (31 - v22 % 32);
        v28 = &v74 + v22 / 32;
        v29 = *v28 + v27;
        if ( v29 < *v28 || v29 < v27 )
          v61 = 1;
        *v28 = v29;
        for ( ii = v61; --v26 >= 0 && ii; ii = v33 )
        {
          v31 = &v74 + v26;
          v32 = *v31 + 1;
          v33 = 0;
          if ( v32 < *v31 || *v31 == -1 )
            v33 = 1;
          *v31 = v32;
        }
      }
LABEL_51:
      *v24 &= -1 << v55;
      if ( v52 + 1 < 3 )
        memset(&v74 + v52 + 1, 0, 4 * (3 - (v52 + 1)));
      v34 = (dword_4FC67C + 1) / 32;
      v35 = (dword_4FC67C + 1) % 32;
      v62 = 0;
      for ( jj = 0; jj < 3; ++jj )
      {
        v36 = (int *)(&v74 + jj);
        v56 = ~(-1 << v35) & *v36;
        *v36 = v62 | ((unsigned int)*v36 >> v35);
        v62 = v56 << (32 - v35);
      }
      v37 = 2;
      v38 = (unsigned int *)(&v76 - v34);
      do
      {
        if ( v37 < v34 )
          *(&v74 + v37) = 0;
        else
          *(&v74 + v37) = *v38;
        --v38;
        --v37;
      }
      while ( v37 >= 0 );
    }
    else
    {
      v74 = 0;
      v75 = 0;
      v76 = 0;
    }
    v5 = 0;
    result = 2;
    goto LABEL_78;
  }
  v5 = 0;
  v6 = 0;
  while ( !*(&v74 + v6) )
  {
    if ( ++v6 >= 3 )
    {
      result = 0;
      goto LABEL_78;
    }
  }
  v74 = 0;
  v75 = 0;
  v76 = 0;
  result = 2;
LABEL_78:
  v48 = v74 | (v50 != 0 ? 0x80000000 : 0) | (v5 << (31 - dword_4FC67C));
  if ( dword_4FC680 == 64 )
  {
    v49 = v75;
    a2[1] = v48;
    *a2 = v49;
  }
  else if ( dword_4FC680 == 32 )
  {
    *a2 = v48;
  }
  return result;
}

// ===== sub_4B8DC2 @ 0x004B8DC2..0x004B9313 =====
int __cdecl sub_4B8DC2(unsigned __int16 *a1, unsigned int *a2)
{
  unsigned __int16 v2; // bx
  int v3; // ebx
  int v4; // eax
  int v5; // ebx
  int v6; // eax
  int result; // eax
  int v8; // edi
  unsigned int *v9; // esi
  int v10; // eax
  bool i; // zf
  int v12; // eax
  unsigned int v13; // edx
  unsigned int *v14; // ecx
  bool v15; // cf
  unsigned int v16; // edi
  int v17; // eax
  int v18; // edx
  int *v19; // ebx
  int v20; // edx
  unsigned int *v21; // ecx
  int v22; // esi
  int v23; // eax
  unsigned int *v24; // ebx
  bool n; // zf
  int v26; // eax
  unsigned int v27; // edx
  unsigned int *v28; // ecx
  unsigned int v29; // edi
  int ii; // ecx
  unsigned int *v31; // ecx
  unsigned int v32; // esi
  int v33; // edi
  int v34; // eax
  int v35; // edx
  int *v36; // ebx
  int v37; // edx
  unsigned int *v38; // ecx
  int v39; // eax
  int v40; // edx
  int *v41; // ebx
  int v42; // edx
  unsigned int *v43; // ecx
  int v44; // eax
  int v45; // edx
  int v46; // edx
  unsigned int *v47; // ecx
  unsigned int v48; // ebx
  unsigned int v49; // edx
  int v50; // [esp+8h] [ebp-38h]
  char v51; // [esp+10h] [ebp-30h]
  int v52; // [esp+10h] [ebp-30h]
  int v53; // [esp+14h] [ebp-2Ch]
  int v54; // [esp+14h] [ebp-2Ch]
  char v55; // [esp+14h] [ebp-2Ch]
  int v56; // [esp+14h] [ebp-2Ch]
  int v57; // [esp+14h] [ebp-2Ch]
  int v58; // [esp+14h] [ebp-2Ch]
  int v59; // [esp+18h] [ebp-28h]
  int v60; // [esp+18h] [ebp-28h]
  int v61; // [esp+18h] [ebp-28h]
  int v62; // [esp+18h] [ebp-28h]
  int v63; // [esp+18h] [ebp-28h]
  int v64; // [esp+18h] [ebp-28h]
  int v65; // [esp+1Ch] [ebp-24h]
  unsigned int v66; // [esp+20h] [ebp-20h]
  int m; // [esp+20h] [ebp-20h]
  int jj; // [esp+20h] [ebp-20h]
  int k; // [esp+20h] [ebp-20h]
  int j; // [esp+20h] [ebp-20h]
  unsigned int v71; // [esp+24h] [ebp-1Ch]
  unsigned int v72; // [esp+28h] [ebp-18h]
  int v73; // [esp+2Ch] [ebp-14h]
  unsigned int v74; // [esp+30h] [ebp-10h] BYREF
  unsigned int v75; // [esp+34h] [ebp-Ch]
  int v76; // [esp+38h] [ebp-8h] BYREF

  v2 = a1[5];
  v50 = v2 & 0x8000;
  v74 = *(_DWORD *)(a1 + 3);
  v3 = (v2 & 0x7FFF) - 0x3FFF;
  v4 = *a1 << 16;
  v75 = *(_DWORD *)(a1 + 1);
  v76 = v4;
  if ( v3 != -16383 )
  {
    v65 = 0;
    v71 = v74;
    v72 = v75;
    v73 = v76;
    v8 = dword_4FC690 - 1;
    v53 = v3;
    v59 = dword_4FC690 / 32;
    v9 = &v74 + dword_4FC690 / 32;
    v51 = 31 - dword_4FC690 % 32;
    if ( ((1 << v51) & *v9) != 0 )
    {
      v10 = dword_4FC690 / 32;
      for ( i = (~(-1 << (31 - dword_4FC690 % 32)) & *(&v74 + v59)) == 0; i; i = *(&v74 + v10) == 0 )
      {
        if ( ++v10 >= 3 )
          goto LABEL_21;
      }
      v12 = v8 / 32;
      v65 = 0;
      v13 = 1 << (31 - v8 % 32);
      v14 = &v74 + v8 / 32;
      v66 = v13 + *v14;
      if ( v66 >= *v14 )
      {
        v15 = v66 < v13;
        goto LABEL_18;
      }
LABEL_19:
      v65 = 1;
      while ( 1 )
      {
        --v12;
        *v14 = v66;
        if ( v12 < 0 || !v65 )
          break;
        v65 = 0;
        v14 = &v74 + v12;
        v16 = *v14 + 1;
        v66 = v16;
        if ( v16 >= *v14 )
        {
          v15 = v16 == 0;
LABEL_18:
          if ( !v15 )
            continue;
        }
        goto LABEL_19;
      }
    }
LABEL_21:
    *v9 &= -1 << v51;
    if ( v59 + 1 < 3 )
      memset(&v74 + v59 + 1, 0, 4 * (3 - (v59 + 1)));
    if ( v65 )
      ++v3;
    if ( v3 >= dword_4FC68C - dword_4FC690 )
    {
      if ( v3 > dword_4FC68C )
      {
        if ( v3 < dword_4FC688 )
        {
          v5 = dword_4FC69C + v3;
          v74 &= ~0x80000000;
          v44 = dword_4FC694 / 32;
          v45 = dword_4FC694 % 32;
          v64 = 0;
          for ( j = 0; j < 3; ++j )
          {
            v58 = ~(-1 << v45) & *(&v74 + j);
            *(&v74 + j) = v64 | (*(&v74 + j) >> v45);
            v64 = v58 << (32 - v45);
          }
          v46 = 2;
          v47 = (unsigned int *)(&v76 - v44);
          do
          {
            if ( v46 < v44 )
              *(&v74 + v46) = 0;
            else
              *(&v74 + v46) = *v47;
            --v47;
            --v46;
          }
          while ( v46 >= 0 );
          result = 0;
        }
        else
        {
          v75 = 0;
          v76 = 0;
          v74 = 0x80000000;
          v39 = dword_4FC694 / 32;
          v40 = dword_4FC694 % 32;
          v63 = 0;
          for ( k = 0; k < 3; ++k )
          {
            v41 = (int *)(&v74 + k);
            v57 = ~(-1 << v40) & *v41;
            *v41 = v63 | ((unsigned int)*v41 >> v40);
            v63 = v57 << (32 - v40);
          }
          v42 = 2;
          v43 = (unsigned int *)(&v76 - v39);
          do
          {
            if ( v42 < v39 )
              *(&v74 + v42) = 0;
            else
              *(&v74 + v42) = *v43;
            --v43;
            --v42;
          }
          while ( v42 >= 0 );
          v5 = dword_4FC688 + dword_4FC69C;
          result = 1;
        }
        goto LABEL_78;
      }
      v74 = v71;
      v75 = v72;
      v17 = (dword_4FC68C - v53) / 32;
      v76 = v73;
      v18 = (dword_4FC68C - v53) % 32;
      v60 = 0;
      for ( m = 0; m < 3; ++m )
      {
        v19 = (int *)(&v74 + m);
        v54 = ~(-1 << v18) & *v19;
        *v19 = v60 | ((unsigned int)*v19 >> v18);
        v60 = v54 << (32 - v18);
      }
      v20 = 2;
      v21 = (unsigned int *)(&v76 - v17);
      do
      {
        if ( v20 < v17 )
          *(&v74 + v20) = 0;
        else
          *(&v74 + v20) = *v21;
        --v21;
        --v20;
      }
      while ( v20 >= 0 );
      v22 = dword_4FC690 - 1;
      v23 = dword_4FC690 / 32;
      v52 = dword_4FC690 / 32;
      v24 = &v74 + dword_4FC690 / 32;
      v55 = 31 - dword_4FC690 % 32;
      if ( ((1 << v55) & *v24) != 0 )
      {
        for ( n = (~(-1 << (31 - dword_4FC690 % 32)) & *(&v74 + v23)) == 0; n; n = *(&v74 + v23) == 0 )
        {
          if ( ++v23 >= 3 )
            goto LABEL_51;
        }
        v26 = v22 / 32;
        v61 = 0;
        v27 = 1 << (31 - v22 % 32);
        v28 = &v74 + v22 / 32;
        v29 = *v28 + v27;
        if ( v29 < *v28 || v29 < v27 )
          v61 = 1;
        *v28 = v29;
        for ( ii = v61; --v26 >= 0 && ii; ii = v33 )
        {
          v31 = &v74 + v26;
          v32 = *v31 + 1;
          v33 = 0;
          if ( v32 < *v31 || *v31 == -1 )
            v33 = 1;
          *v31 = v32;
        }
      }
LABEL_51:
      *v24 &= -1 << v55;
      if ( v52 + 1 < 3 )
        memset(&v74 + v52 + 1, 0, 4 * (3 - (v52 + 1)));
      v34 = (dword_4FC694 + 1) / 32;
      v35 = (dword_4FC694 + 1) % 32;
      v62 = 0;
      for ( jj = 0; jj < 3; ++jj )
      {
        v36 = (int *)(&v74 + jj);
        v56 = ~(-1 << v35) & *v36;
        *v36 = v62 | ((unsigned int)*v36 >> v35);
        v62 = v56 << (32 - v35);
      }
      v37 = 2;
      v38 = (unsigned int *)(&v76 - v34);
      do
      {
        if ( v37 < v34 )
          *(&v74 + v37) = 0;
        else
          *(&v74 + v37) = *v38;
        --v38;
        --v37;
      }
      while ( v37 >= 0 );
    }
    else
    {
      v74 = 0;
      v75 = 0;
      v76 = 0;
    }
    v5 = 0;
    result = 2;
    goto LABEL_78;
  }
  v5 = 0;
  v6 = 0;
  while ( !*(&v74 + v6) )
  {
    if ( ++v6 >= 3 )
    {
      result = 0;
      goto LABEL_78;
    }
  }
  v74 = 0;
  v75 = 0;
  v76 = 0;
  result = 2;
LABEL_78:
  v48 = v74 | (v50 != 0 ? 0x80000000 : 0) | (v5 << (31 - dword_4FC694));
  if ( dword_4FC698 == 64 )
  {
    v49 = v75;
    a2[1] = v48;
    *a2 = v49;
  }
  else if ( dword_4FC698 == 32 )
  {
    *a2 = v48;
  }
  return result;
}

// ===== ___strgtold12_l @ 0x004B9313..0x004B99BC =====
int __cdecl __strgtold12_l(int a1, char **a2, char *a3, int a4, int a5, int a6, int a7, int a8)
{
  int v8; // ecx
  _BYTE *v9; // edi
  int result; // eax
  char *v11; // edx
  char v12; // al
  char v13; // al
  int v14; // eax
  int v15; // eax
  int v16; // eax
  int v17; // eax
  bool v18; // zf
  int v19; // eax
  int v20; // ecx
  int v21; // eax
  int v22; // eax
  char *v23; // ecx
  char v24; // al
  char *v25; // ecx
  int v26; // eax
  char *v27; // ebx
  __int16 v28; // ax
  __int16 v29; // si
  unsigned __int16 v30; // cx
  int v31; // eax
  _WORD *v32; // edi
  unsigned int v33; // eax
  unsigned int v34; // edx
  unsigned int v35; // esi
  __int16 v36; // cx
  unsigned int v37; // eax
  int v38; // esi
  int v39; // eax
  int v40; // esi
  int v41; // eax
  int v42; // edi
  int v43; // eax
  __int16 v44; // cx
  int v45; // esi
  unsigned int v46; // edx
  __int16 v47; // ax
  int v48; // [esp-8h] [ebp-8Ch]
  int v49; // [esp-8h] [ebp-8Ch]
  char *v50; // [esp+8h] [ebp-7Ch]
  __int16 v51; // [esp+10h] [ebp-74h]
  __int16 v52; // [esp+14h] [ebp-70h]
  int v53; // [esp+18h] [ebp-6Ch]
  int v54; // [esp+1Ch] [ebp-68h]
  int v55; // [esp+1Ch] [ebp-68h]
  int v56; // [esp+20h] [ebp-64h]
  int v57; // [esp+20h] [ebp-64h]
  int v58; // [esp+24h] [ebp-60h]
  unsigned __int16 *v59; // [esp+24h] [ebp-60h]
  int v60; // [esp+28h] [ebp-5Ch]
  unsigned __int16 *v61; // [esp+28h] [ebp-5Ch]
  int v62; // [esp+2Ch] [ebp-58h]
  int i; // [esp+2Ch] [ebp-58h]
  char *v64; // [esp+30h] [ebp-54h]
  int v65; // [esp+30h] [ebp-54h]
  int v66; // [esp+34h] [ebp-50h]
  int v67; // [esp+34h] [ebp-50h]
  unsigned int v68; // [esp+38h] [ebp-4Ch]
  int v69; // [esp+38h] [ebp-4Ch]
  __int64 v70; // [esp+3Ch] [ebp-48h] BYREF
  int v71; // [esp+44h] [ebp-40h]
  _DWORD v72[7]; // [esp+48h] [ebp-3Ch] BYREF
  _BYTE v73[23]; // [esp+64h] [ebp-20h] BYREF
  char v74; // [esp+7Bh] [ebp-9h]

  v8 = 0;
  v9 = v73;
  v51 = 0;
  v54 = 1;
  v68 = 0;
  v62 = 0;
  v60 = 0;
  v58 = 0;
  v56 = 0;
  v66 = 0;
  v53 = 0;
  if ( !a8 )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 0;
  }
  v11 = a3;
  v64 = a3;
  while ( 1 )
  {
    v12 = *v11;
    if ( *v11 != 32 && v12 != 9 && v12 != 10 && v12 != 13 )
      break;
    ++v11;
  }
  while ( 2 )
  {
    v13 = *v11++;
    switch ( v8 )
    {
      case 0:
        if ( (unsigned __int8)(v13 - 49) <= 8u )
          goto LABEL_11;
        if ( v13 == ***(_BYTE ***)(*(_DWORD *)a8 + 188) )
          goto LABEL_14;
        v14 = v13 - 43;
        if ( !v14 )
        {
          v51 = 0;
          v8 = 2;
          continue;
        }
        v15 = v14 - 2;
        if ( !v15 )
        {
          v8 = 2;
          v51 = 0x8000;
          continue;
        }
        if ( v15 != 3 )
          goto LABEL_74;
        goto LABEL_19;
      case 1:
        v62 = 1;
        if ( (unsigned __int8)(v13 - 49) <= 8u )
          goto LABEL_11;
        if ( v13 == ***(_BYTE ***)(*(_DWORD *)a8 + 188) )
          goto LABEL_24;
        if ( v13 == 43 || v13 == 45 )
          goto LABEL_32;
        if ( v13 == 48 )
          goto LABEL_19;
LABEL_28:
        if ( v13 <= 67 || v13 > 69 && (unsigned __int8)(v13 - 100) > 1u )
          goto LABEL_74;
        v49 = 6;
        goto LABEL_15;
      case 2:
        if ( (unsigned __int8)(v13 - 49) <= 8u )
        {
LABEL_11:
          v48 = 3;
LABEL_12:
          v8 = v48;
          --v11;
        }
        else
        {
          if ( v13 == ***(_BYTE ***)(*(_DWORD *)a8 + 188) )
          {
LABEL_14:
            v49 = 5;
            goto LABEL_15;
          }
          if ( v13 != 48 )
          {
LABEL_36:
            v11 = v64;
            goto LABEL_81;
          }
LABEL_19:
          v8 = 1;
        }
        continue;
      case 3:
        v62 = 1;
        while ( v13 >= 48 && v13 <= 57 )
        {
          if ( v68 >= 0x19 )
          {
            ++v66;
          }
          else
          {
            ++v68;
            *v9++ = v13 - 48;
          }
          v13 = *v11++;
        }
        if ( v13 != ***(_BYTE ***)(*(_DWORD *)a8 + 188) )
          goto LABEL_45;
LABEL_24:
        v49 = 4;
        goto LABEL_15;
      case 4:
        v62 = 1;
        v60 = 1;
        if ( !v68 )
        {
          while ( v13 == 48 )
          {
            --v66;
            v13 = *v11++;
          }
        }
        while ( v13 >= 48 && v13 <= 57 )
        {
          if ( v68 < 0x19 )
          {
            ++v68;
            *v9++ = v13 - 48;
            --v66;
          }
          v13 = *v11++;
        }
LABEL_45:
        if ( v13 != 43 && v13 != 45 )
          goto LABEL_28;
LABEL_32:
        --v11;
        v49 = 11;
        goto LABEL_15;
      case 5:
        v60 = 1;
        if ( (unsigned __int8)(v13 - 48) > 9u )
          goto LABEL_36;
        v48 = 4;
        goto LABEL_12;
      case 6:
        v64 = v11 - 2;
        if ( (unsigned __int8)(v13 - 49) <= 8u )
          goto LABEL_62;
        v16 = v13 - 43;
        if ( !v16 )
          goto LABEL_69;
        v17 = v16 - 2;
        if ( !v17 )
          goto LABEL_68;
        v18 = v17 == 3;
LABEL_66:
        if ( !v18 )
          goto LABEL_36;
        v49 = 8;
        goto LABEL_15;
      case 7:
        if ( (unsigned __int8)(v13 - 49) <= 8u )
          goto LABEL_62;
        v18 = v13 == 48;
        goto LABEL_66;
      case 8:
        v58 = 1;
        while ( v13 == 48 )
          v13 = *v11++;
        if ( (unsigned __int8)(v13 - 49) > 8u )
          goto LABEL_74;
LABEL_62:
        v48 = 9;
        goto LABEL_12;
      case 9:
        v58 = 1;
        v20 = 0;
        while ( 2 )
        {
          if ( v13 >= 48 && v13 <= 57 )
          {
            v20 = 10 * v20 + v13 - 48;
            if ( v20 <= 5200 )
            {
              v13 = *v11++;
              continue;
            }
            v20 = 5201;
          }
          break;
        }
        v56 = v20;
        while ( v13 >= 48 && v13 <= 57 )
          v13 = *v11++;
LABEL_74:
        --v11;
        goto LABEL_81;
      case 11:
        if ( !a7 )
        {
          v8 = 10;
          --v11;
LABEL_89:
          if ( v8 == 10 )
            goto LABEL_81;
          continue;
        }
        v19 = v13 - 43;
        v64 = v11 - 1;
        if ( !v19 )
        {
LABEL_69:
          v49 = 7;
LABEL_15:
          v8 = v49;
          continue;
        }
        if ( v19 == 2 )
        {
LABEL_68:
          v54 = -1;
          v8 = 7;
          continue;
        }
        --v11;
LABEL_81:
        *a2 = v11;
        if ( !v62 )
        {
          v53 = 4;
LABEL_178:
          v44 = 0;
          v47 = 0;
          v46 = 0;
          v45 = 0;
          goto LABEL_179;
        }
        if ( v68 > 0x18 )
        {
          if ( v74 >= 5 )
            ++v74;
          --v9;
          ++v66;
          v68 = 24;
        }
        if ( !v68 )
          goto LABEL_178;
        while ( !*--v9 )
        {
          --v68;
          ++v66;
        }
        __mtold12(v73, v68, v72);
        v21 = v56;
        if ( v54 < 0 )
          v21 = -v56;
        v22 = v66 + v21;
        if ( !v58 )
          v22 += a5;
        if ( !v60 )
          v22 -= a6;
        if ( v22 > 5200 )
        {
          v45 = 0;
          v47 = 0x7FFF;
          v46 = 0x80000000;
          v44 = 0;
          v53 = 2;
          goto LABEL_179;
        }
        if ( v22 < -5200 )
        {
          v53 = 1;
          goto LABEL_178;
        }
        v23 = (char *)&unk_4FC6B0 - 96;
        v65 = v22;
        if ( v22 )
        {
          if ( v22 < 0 )
          {
            v65 = -v22;
            v23 = (char *)&unk_4FC810 - 96;
          }
          if ( !a4 )
            LOWORD(v72[0]) = 0;
          if ( v65 )
          {
            while ( 1 )
            {
              v24 = v65;
              v65 >>= 3;
              v25 = v23 + 84;
              v26 = v24 & 7;
              v50 = v25;
              if ( !v26 )
                goto LABEL_173;
              v27 = &v25[12 * v26];
              if ( *(_WORD *)v27 >= 0x8000u )
              {
                v70 = *(_QWORD *)v27;
                v71 = *((_DWORD *)v27 + 2);
                --*(_DWORD *)((char *)&v70 + 2);
                v27 = (char *)&v70;
              }
              v67 = 0;
              memset(&v72[4], 0, 12);
              v28 = *((_WORD *)v27 + 5) & 0x7FFF;
              v29 = (HIWORD(v72[2]) ^ *((_WORD *)v27 + 5)) & 0x8000;
              v52 = v29;
              v30 = v28 + (HIWORD(v72[2]) & 0x7FFF);
              if ( (HIWORD(v72[2]) & 0x7FFF) == 0x7FFF || (*((_WORD *)v27 + 5) & 0x7FFF) == 0x7FFF || v30 > 0xBFFDu )
                break;
              if ( v30 <= 0x3FBFu )
              {
                v31 = 0;
                v72[1] = 0;
                v72[0] = 0;
LABEL_172:
                v72[2] = v31;
                goto LABEL_173;
              }
              if ( (v72[2] & 0x7FFF0000) != 0 || (++v30, (v72[2] & 0x7FFFFFFF) != 0) || v72[1] || v72[0] )
              {
                if ( v28 || (++v30, (*((_DWORD *)v27 + 2) & 0x7FFFFFFF) != 0) || *((_DWORD *)v27 + 1) || *(_DWORD *)v27 )
                {
                  v55 = 0;
                  v32 = &v72[5];
                  for ( i = 5; i > 0; --i )
                  {
                    v57 = i;
                    v61 = (unsigned __int16 *)v72 + v55;
                    v59 = (unsigned __int16 *)(v27 + 8);
                    do
                    {
                      v69 = 0;
                      v33 = *v61 * *v59;
                      v34 = *((_DWORD *)v32 - 1);
                      v35 = v34 + v33;
                      if ( v34 + v33 < v34 || v35 < v33 )
                        v69 = 1;
                      *((_DWORD *)v32 - 1) = v35;
                      if ( v69 )
                        ++*v32;
                      ++v61;
                      --v59;
                      --v57;
                    }
                    while ( v57 > 0 );
                    ++v32;
                    ++v55;
                  }
                  v36 = v30 - 16382;
                  if ( v36 <= 0 )
                    goto LABEL_182;
                  do
                  {
                    if ( v72[6] < 0 )
                      break;
                    v37 = v72[4];
                    v72[4] *= 2;
                    v38 = (v37 >> 31) | (2 * v72[5]);
                    v39 = *(__int64 *)&v72[5] >> 31;
                    --v36;
                    v72[5] = v38;
                    v72[6] = v39;
                  }
                  while ( v36 > 0 );
                  if ( v36 <= 0 )
                  {
LABEL_182:
                    if ( --v36 < 0 )
                    {
                      v40 = (unsigned __int16)-v36;
                      v36 = 0;
                      do
                      {
                        if ( (v72[4] & 1) != 0 )
                          ++v67;
                        v41 = v72[6];
                        v72[6] >>= 1;
                        v42 = (v41 << 31) | (v72[5] >> 1);
                        v43 = *(__int64 *)&v72[4] >> 1;
                        --v40;
                        v72[5] = v42;
                        v72[4] = v43;
                      }
                      while ( v40 );
                      if ( v67 )
                        LOWORD(v72[4]) |= 1u;
                    }
                  }
                  if ( LOWORD(v72[4]) > 0x8000u || (v72[4] & 0x1FFFF) == 0x18000 )
                  {
                    if ( *(_DWORD *)((char *)&v72[4] + 2) == -1 )
                    {
                      *(_DWORD *)((char *)&v72[4] + 2) = 0;
                      if ( *(_DWORD *)((char *)&v72[5] + 2) == -1 )
                      {
                        *(_DWORD *)((char *)&v72[5] + 2) = 0;
                        if ( HIWORD(v72[6]) == 0xFFFF )
                        {
                          HIWORD(v72[6]) = 0x8000;
                          ++v36;
                        }
                        else
                        {
                          ++HIWORD(v72[6]);
                        }
                      }
                      else
                      {
                        ++*(_DWORD *)((char *)&v72[5] + 2);
                      }
                    }
                    else
                    {
                      ++*(_DWORD *)((char *)&v72[4] + 2);
                    }
                  }
                  if ( (unsigned __int16)v36 < 0x7FFFu )
                  {
                    LOWORD(v72[0]) = HIWORD(v72[4]);
                    *(_QWORD *)((char *)v72 + 2) = *(_QWORD *)&v72[5];
                    HIWORD(v72[2]) = v52 | v36;
                  }
                  else
                  {
                    v72[1] = 0;
                    v72[0] = 0;
                    v72[2] = v52 == 0 ? 2147450880 : -32768;
                  }
                }
                else
                {
                  memset(v72, 0, 12);
                }
              }
              else
              {
                HIWORD(v72[2]) = 0;
              }
LABEL_173:
              if ( !v65 )
                goto LABEL_174;
              v23 = v50;
            }
            v72[1] = 0;
            v31 = v29 == 0 ? 2147450880 : -32768;
            v72[0] = 0;
            goto LABEL_172;
          }
        }
LABEL_174:
        v44 = v72[0];
        v45 = *(_DWORD *)((char *)v72 + 2);
        v46 = *(_DWORD *)((char *)&v72[1] + 2);
        v47 = HIWORD(v72[2]);
LABEL_179:
        *(_WORD *)a1 = v44;
        *(_WORD *)(a1 + 10) = v51 | v47;
        result = v53;
        *(_DWORD *)(a1 + 2) = v45;
        *(_DWORD *)(a1 + 6) = v46;
        return result;
      default:
        goto LABEL_89;
    }
  }
}

// ===== _$I10_OUTPUT @ 0x004B99EF..0x004BA2E7 =====
int __cdecl _I10_OUTPUT(__int64 a1, __int16 a2, int a3, char a4, int a5)
{
  unsigned __int16 v5; // dx
  errno_t v7; // eax
  errno_t v8; // eax
  __int16 v9; // ax
  int v10; // ebx
  bool v11; // zf
  char v12; // cl
  int v13; // ecx
  char *v14; // eax
  int *v15; // esi
  __int16 v16; // cx
  unsigned __int16 v17; // di
  _WORD *v18; // esi
  unsigned int v19; // edx
  unsigned int v20; // ecx
  unsigned int v21; // eax
  __int16 v22; // di
  unsigned int v23; // ecx
  unsigned int v24; // edx
  int v25; // eax
  unsigned int v26; // ecx
  int v27; // esi
  int v28; // ecx
  unsigned __int16 v29; // si
  int v30; // eax
  _WORD *v31; // edi
  unsigned __int16 *v32; // eax
  unsigned int v33; // ecx
  unsigned int v34; // edx
  unsigned int v35; // ebx
  __int16 v36; // si
  unsigned int v37; // ecx
  unsigned int v38; // edx
  int v39; // eax
  unsigned int v40; // ecx
  int v41; // edi
  int v42; // ecx
  int v43; // edi
  int v44; // esi
  unsigned int v45; // eax
  int v46; // ebx
  int v47; // eax
  int v48; // esi
  int v49; // eax
  int v50; // ebx
  int v51; // eax
  _BYTE *v52; // ebx
  unsigned int v53; // edx
  unsigned int v54; // edi
  unsigned int v55; // ecx
  int v56; // esi
  int v57; // ecx
  unsigned int v58; // esi
  unsigned int v59; // edi
  int v60; // edx
  unsigned int v61; // edx
  char v62; // al
  _BYTE *v63; // ebx
  int v64; // eax
  char v65; // bl
  unsigned __int16 *v66; // [esp+10h] [ebp-70h]
  unsigned __int16 *v67; // [esp+14h] [ebp-6Ch]
  char *v68; // [esp+18h] [ebp-68h]
  __int16 v69; // [esp+20h] [ebp-60h]
  int v70; // [esp+24h] [ebp-5Ch]
  __int16 v71; // [esp+24h] [ebp-5Ch]
  __int16 v72; // [esp+28h] [ebp-58h]
  int *v73; // [esp+28h] [ebp-58h]
  int v74; // [esp+2Ch] [ebp-54h]
  int v75; // [esp+2Ch] [ebp-54h]
  int v76; // [esp+30h] [ebp-50h]
  int v77; // [esp+30h] [ebp-50h]
  __int16 v78; // [esp+34h] [ebp-4Ch]
  int k; // [esp+34h] [ebp-4Ch]
  int v80; // [esp+38h] [ebp-48h]
  int v81; // [esp+38h] [ebp-48h]
  char *v82; // [esp+3Ch] [ebp-44h]
  int v83; // [esp+3Ch] [ebp-44h]
  int v84; // [esp+3Ch] [ebp-44h]
  unsigned int v85; // [esp+3Ch] [ebp-44h]
  int i; // [esp+40h] [ebp-40h]
  int j; // [esp+40h] [ebp-40h]
  _BYTE *v88; // [esp+40h] [ebp-40h]
  __int64 v89; // [esp+44h] [ebp-3Ch] BYREF
  int v90; // [esp+4Ch] [ebp-34h]
  int v91; // [esp+50h] [ebp-30h]
  int v92; // [esp+54h] [ebp-2Ch]
  int v93; // [esp+58h] [ebp-28h] BYREF
  _BYTE v94[12]; // [esp+60h] [ebp-20h] BYREF
  _BYTE v95[12]; // [esp+70h] [ebp-10h] BYREF

  v69 = a2 & 0x8000;
  v5 = a2 & 0x7FFF;
  v91 = -858993460;
  v92 = -858993460;
  v93 = 1073466572;
  if ( a2 >= 0 )
    *(_BYTE *)(a5 + 2) = 32;
  else
    *(_BYTE *)(a5 + 2) = 45;
  if ( v5 )
  {
    if ( v5 != 0x7FFF )
      goto LABEL_25;
    *(_WORD *)a5 = 1;
    if ( a1 != 0x8000000000000000uLL && (a1 & 0x4000000000000000LL) == 0 )
    {
      v7 = strcpy_s((char *)(a5 + 4), 0x16u, "1#SNAN");
      goto LABEL_22;
    }
    if ( v69 && HIDWORD(a1) == -1073741824 )
    {
      if ( !(_DWORD)a1 )
      {
        v8 = strcpy_s((char *)(a5 + 4), 0x16u, "1#IND");
        goto LABEL_19;
      }
    }
    else if ( a1 == 0x8000000000000000uLL )
    {
      v8 = strcpy_s((char *)(a5 + 4), 0x16u, "1#INF");
LABEL_19:
      if ( !v8 )
      {
        *(_BYTE *)(a5 + 3) = 5;
        return 0;
      }
LABEL_12:
      _invoke_watson(0, 0, 0, 0, 0);
    }
    v7 = strcpy_s((char *)(a5 + 4), 0x16u, "1#QNAN");
LABEL_22:
    if ( !v7 )
    {
      *(_BYTE *)(a5 + 3) = 6;
      return 0;
    }
    goto LABEL_12;
  }
  if ( !a1 )
  {
    *(_WORD *)a5 = 0;
    *(_BYTE *)(a5 + 2) = v69 != -32768 ? 32 : 45;
    *(_WORD *)(a5 + 3) = 12289;
    *(_BYTE *)(a5 + 5) = 0;
    return 1;
  }
LABEL_25:
  v9 = (77 * (HIBYTE(v5) + 2 * HIBYTE(HIDWORD(a1))) + 19728 * (unsigned int)v5 - 323162868) >> 16;
  *(_WORD *)v94 = 0;
  v10 = -v9;
  v78 = v9;
  *(_WORD *)&v94[10] = a2 & 0x7FFF;
  *(_QWORD *)&v94[2] = a1;
  v68 = (char *)&unk_4FC6B0 - 96;
  if ( v9 )
  {
    v11 = v9 == 0;
    if ( v9 > 0 )
    {
      v10 = v9;
      v68 = (char *)&unk_4FC810 - 96;
      v11 = v9 == 0;
    }
    if ( !v11 )
    {
      do
      {
        v68 += 84;
        v12 = v10;
        v10 >>= 3;
        v13 = v12 & 7;
        if ( v13 )
        {
          v14 = &v68[12 * v13];
          v82 = v14;
          if ( *(_WORD *)v14 >= 0x8000u )
          {
            v89 = *(_QWORD *)v14;
            v15 = (int *)(v14 + 8);
            v14 = (char *)&v89;
            v90 = *v15;
            --*(_DWORD *)((char *)&v89 + 2);
            v82 = (char *)&v89;
          }
          v80 = 0;
          memset(v95, 0, sizeof(v95));
          v72 = (*(_WORD *)&v94[10] ^ *((_WORD *)v14 + 5)) & 0x8000;
          v16 = *((_WORD *)v14 + 5) & 0x7FFF;
          v17 = v16 + (*(_WORD *)&v94[10] & 0x7FFF);
          if ( (*(_WORD *)&v94[10] & 0x7FFF) == 0x7FFF || (*((_WORD *)v14 + 5) & 0x7FFF) == 0x7FFF || v17 > 0xBFFDu )
          {
            *(_DWORD *)&v94[8] = ((*(_WORD *)&v94[10] ^ *((_WORD *)v14 + 5)) & 0x8000u) == 0 ? 2147450880 : -32768;
          }
          else
          {
            if ( v17 > 0x3FBFu )
            {
              if ( (*(_WORD *)&v94[10] & 0x7FFF) == 0 )
              {
                ++v17;
                if ( (*(_DWORD *)&v94[8] & 0x7FFFFFFF) == 0 && !*(_DWORD *)&v94[4] && !*(_DWORD *)v94 )
                {
                  *(_WORD *)&v94[10] = 0;
                  continue;
                }
              }
              if ( v16 || (++v17, (*((_DWORD *)v14 + 2) & 0x7FFFFFFF) != 0) || *((_DWORD *)v14 + 1) || *(_DWORD *)v14 )
              {
                v74 = 0;
                v18 = &v95[4];
                for ( i = 5; i > 0; --i )
                {
                  v76 = i;
                  v66 = (unsigned __int16 *)&v94[2 * v74];
                  v67 = (unsigned __int16 *)(v14 + 8);
                  do
                  {
                    v19 = *((_DWORD *)v18 - 1);
                    v20 = *v67 * *v66;
                    v70 = 0;
                    v21 = v19 + v20;
                    if ( v19 + v20 < v19 || v21 < v20 )
                      v70 = 1;
                    *((_DWORD *)v18 - 1) = v21;
                    if ( v70 )
                      ++*v18;
                    ++v66;
                    --v67;
                    --v76;
                  }
                  while ( v76 > 0 );
                  v14 = v82;
                  ++v18;
                  ++v74;
                }
                v22 = v17 - 16382;
                if ( v22 <= 0 )
                  goto LABEL_169;
                do
                {
                  if ( *(int *)&v95[8] < 0 )
                    break;
                  v23 = *(_DWORD *)v95;
                  *(_DWORD *)v95 *= 2;
                  v24 = *(_DWORD *)&v95[4];
                  *(_DWORD *)&v95[4] = (v23 >> 31) | (2 * *(_DWORD *)&v95[4]);
                  --v22;
                  *(_DWORD *)&v95[8] = (v24 >> 31) | (2 * *(_DWORD *)&v95[8]);
                }
                while ( v22 > 0 );
                if ( v22 <= 0 )
                {
LABEL_169:
                  if ( --v22 < 0 )
                  {
                    v25 = (unsigned __int16)-v22;
                    v22 = 0;
                    do
                    {
                      if ( (v95[0] & 1) != 0 )
                        ++v80;
                      v26 = *(_DWORD *)&v95[8];
                      *(_DWORD *)&v95[8] >>= 1;
                      v27 = __SPAIR64__(v26, *(unsigned int *)&v95[4]) >> 1;
                      v28 = *(__int64 *)v95 >> 1;
                      --v25;
                      *(_DWORD *)&v95[4] = v27;
                      *(_DWORD *)v95 = v28;
                    }
                    while ( v25 );
                    if ( v80 )
                      *(_WORD *)v95 |= 1u;
                  }
                }
                if ( *(_WORD *)v95 > 0x8000u || (*(_DWORD *)v95 & 0x1FFFF) == 0x18000 )
                {
                  if ( *(_DWORD *)&v95[2] == -1 )
                  {
                    *(_DWORD *)&v95[2] = 0;
                    if ( *(_DWORD *)&v95[6] == -1 )
                    {
                      *(_DWORD *)&v95[6] = 0;
                      if ( *(_WORD *)&v95[10] == 0xFFFF )
                      {
                        *(_WORD *)&v95[10] = 0x8000;
                        ++v22;
                      }
                      else
                      {
                        ++*(_WORD *)&v95[10];
                      }
                    }
                    else
                    {
                      ++*(_DWORD *)&v95[6];
                    }
                  }
                  else
                  {
                    ++*(_DWORD *)&v95[2];
                  }
                }
                if ( (unsigned __int16)v22 < 0x7FFFu )
                {
                  *(_WORD *)v94 = *(_WORD *)&v95[2];
                  *(_DWORD *)&v94[2] = *(_DWORD *)&v95[4];
                  *(_DWORD *)&v94[6] = *(_DWORD *)&v95[8];
                  *(_WORD *)&v94[10] = v72 | v22;
                }
                else
                {
                  *(_DWORD *)&v94[4] = 0;
                  *(_DWORD *)v94 = 0;
                  *(_DWORD *)&v94[8] = v72 == 0 ? 2147450880 : -32768;
                }
                continue;
              }
            }
            *(_DWORD *)&v94[8] = 0;
          }
          *(_QWORD *)v94 = 0LL;
        }
      }
      while ( v10 );
    }
  }
  if ( *(_WORD *)&v94[10] < 0x3FFFu )
    goto LABEL_131;
  ++v78;
  v77 = 0;
  memset(v95, 0, sizeof(v95));
  v71 = (*(_WORD *)&v94[10] ^ HIWORD(v93)) & 0x8000;
  v29 = (HIWORD(v93) & 0x7FFF) + (*(_WORD *)&v94[10] & 0x7FFF);
  if ( (*(_WORD *)&v94[10] & 0x7FFF) == 0x7FFF || (HIWORD(v93) & 0x7FFF) == 0x7FFF || v29 > 0xBFFDu )
  {
    *(_DWORD *)&v94[4] = 0;
    v30 = ((*(_WORD *)&v94[10] ^ HIWORD(v93)) & 0x8000u) == 0 ? 2147450880 : -32768;
    *(_DWORD *)v94 = 0;
  }
  else
  {
    if ( v29 > 0x3FBFu )
    {
      v30 = 0;
      if ( (*(_WORD *)&v94[10] & 0x7FFF) == 0 )
      {
        ++v29;
        if ( (*(_DWORD *)&v94[8] & 0x7FFFFFFF) == 0 && !*(_DWORD *)&v94[4] && !*(_DWORD *)v94 )
        {
          *(_WORD *)&v94[10] = 0;
          goto LABEL_131;
        }
      }
      if ( (v93 & 0x7FFF0000) != 0 || (++v29, (v93 & 0x7FFFFFFF) != 0) || v92 || v91 )
      {
        v75 = 0;
        v31 = &v95[4];
        for ( j = 5; j > 0; --j )
        {
          v81 = j;
          v73 = &v93;
          v32 = (unsigned __int16 *)&v94[2 * v75];
          do
          {
            v83 = 0;
            v33 = *v32 * *(unsigned __int16 *)v73;
            v34 = *((_DWORD *)v31 - 1);
            v35 = v34 + v33;
            if ( v34 + v33 < v34 || v35 < v33 )
              v83 = 1;
            *((_DWORD *)v31 - 1) = v35;
            if ( v83 )
              ++*v31;
            v73 = (int *)((char *)v73 - 2);
            ++v32;
            --v81;
          }
          while ( v81 > 0 );
          ++v31;
          ++v75;
        }
        v36 = v29 - 16382;
        if ( v36 <= 0 )
          goto LABEL_170;
        do
        {
          if ( *(int *)&v95[8] < 0 )
            break;
          v37 = *(_DWORD *)v95;
          *(_DWORD *)v95 *= 2;
          v38 = *(_DWORD *)&v95[4];
          *(_DWORD *)&v95[4] = (v37 >> 31) | (2 * *(_DWORD *)&v95[4]);
          --v36;
          *(_DWORD *)&v95[8] = (v38 >> 31) | (2 * *(_DWORD *)&v95[8]);
        }
        while ( v36 > 0 );
        if ( v36 <= 0 )
        {
LABEL_170:
          if ( --v36 < 0 )
          {
            v39 = (unsigned __int16)-v36;
            v36 = 0;
            do
            {
              if ( (v95[0] & 1) != 0 )
                ++v77;
              v40 = *(_DWORD *)&v95[8];
              *(_DWORD *)&v95[8] >>= 1;
              v41 = __SPAIR64__(v40, *(unsigned int *)&v95[4]) >> 1;
              v42 = *(__int64 *)v95 >> 1;
              --v39;
              *(_DWORD *)&v95[4] = v41;
              *(_DWORD *)v95 = v42;
            }
            while ( v39 );
            if ( v77 )
              *(_WORD *)v95 |= 1u;
          }
        }
        if ( *(_WORD *)v95 > 0x8000u || (*(_DWORD *)v95 & 0x1FFFF) == 0x18000 )
        {
          if ( *(_DWORD *)&v95[2] == -1 )
          {
            *(_DWORD *)&v95[2] = 0;
            if ( *(_DWORD *)&v95[6] == -1 )
            {
              *(_DWORD *)&v95[6] = 0;
              if ( *(_WORD *)&v95[10] == 0xFFFF )
              {
                *(_WORD *)&v95[10] = 0x8000;
                ++v36;
              }
              else
              {
                ++*(_WORD *)&v95[10];
              }
            }
            else
            {
              ++*(_DWORD *)&v95[6];
            }
          }
          else
          {
            ++*(_DWORD *)&v95[2];
          }
        }
        if ( (unsigned __int16)v36 < 0x7FFFu )
        {
          *(_WORD *)v94 = *(_WORD *)&v95[2];
          *(_DWORD *)&v94[2] = *(_DWORD *)&v95[4];
          *(_DWORD *)&v94[6] = *(_DWORD *)&v95[8];
          *(_WORD *)&v94[10] = v71 | v36;
        }
        else
        {
          *(_DWORD *)&v94[4] = 0;
          *(_DWORD *)v94 = 0;
          *(_DWORD *)&v94[8] = v71 == 0 ? 2147450880 : -32768;
        }
        goto LABEL_131;
      }
    }
    else
    {
      v30 = 0;
    }
    *(_QWORD *)v94 = 0LL;
  }
  *(_DWORD *)&v94[8] = v30;
LABEL_131:
  v43 = a3;
  *(_WORD *)a5 = v78;
  if ( (a4 & 1) != 0 )
  {
    v43 = v78 + a3;
    if ( v43 <= 0 )
    {
      *(_WORD *)a5 = 0;
      *(_WORD *)(a5 + 3) = 12289;
      *(_BYTE *)(a5 + 2) = v69 != -32768 ? 32 : 45;
      *(_BYTE *)(a5 + 5) = 0;
      return 1;
    }
  }
  if ( v43 > 21 )
    v43 = 21;
  v44 = *(unsigned __int16 *)&v94[10] - 16382;
  *(_WORD *)&v94[10] = 0;
  v84 = 8;
  do
  {
    v45 = *(_DWORD *)v94;
    *(_DWORD *)v94 *= 2;
    v46 = (v45 >> 31) | (2 * *(_DWORD *)&v94[4]);
    v47 = *(__int64 *)&v94[4] >> 31;
    v11 = v84-- == 1;
    *(_DWORD *)&v94[4] = v46;
    *(_DWORD *)&v94[8] = v47;
  }
  while ( !v11 );
  if ( v44 < 0 )
  {
    v48 = (unsigned __int8)-(char)v44;
    if ( v48 )
    {
      do
      {
        v49 = *(_DWORD *)&v94[8];
        *(_DWORD *)&v94[8] >>= 1;
        v50 = (v49 << 31) | (*(_DWORD *)&v94[4] >> 1);
        v51 = *(__int64 *)v94 >> 1;
        --v48;
        *(_DWORD *)&v94[4] = v50;
        *(_DWORD *)v94 = v51;
      }
      while ( v48 > 0 );
    }
  }
  v52 = (_BYTE *)(a5 + 4);
  v88 = (_BYTE *)(a5 + 4);
  for ( k = v43 + 1; k > 0; v94[11] = 0 )
  {
    v53 = *(_DWORD *)v94;
    v89 = *(_QWORD *)v94;
    v90 = *(_DWORD *)&v94[8];
    *(_DWORD *)v94 *= 2;
    v54 = *(_DWORD *)v94;
    *(_DWORD *)v94 *= 2;
    v55 = __SPAIR64__(*(unsigned int *)&v94[4], v53) >> 31;
    v56 = 2 * v55;
    v57 = (v55 >> 31) | (2 * (*(__int64 *)&v94[4] >> 31));
    v58 = (v54 >> 31) | v56;
    v59 = v89 + *(_DWORD *)v94;
    if ( (unsigned int)(v89 + *(_DWORD *)v94) < *(_DWORD *)v94 || v59 < (unsigned int)v89 )
    {
      v60 = 0;
      if ( v58 + 1 < v58 || v58 == -1 )
        v60 = 1;
      ++v58;
      if ( v60 )
        ++v57;
    }
    v61 = HIDWORD(v89) + v58;
    v85 = HIDWORD(v89) + v58;
    if ( HIDWORD(v89) + v58 < v58 || v61 < HIDWORD(v89) )
      ++v57;
    *(_DWORD *)v94 = 2 * v59;
    *(_DWORD *)&v94[8] = (v61 >> 31) | (2 * (v90 + v57));
    *v52++ = v94[11] + 48;
    --k;
    *(_DWORD *)&v94[4] = (v59 >> 31) | (2 * v85);
  }
  v62 = *(v52 - 1);
  v63 = v52 - 2;
  if ( v62 >= 53 )
  {
    while ( v63 >= v88 && *v63 == 57 )
      *v63-- = 48;
    v64 = a5;
    if ( v63 < v88 )
    {
      ++v63;
      ++*(_WORD *)a5;
    }
    ++*v63;
  }
  else
  {
    while ( v63 >= v88 && *v63 == 48 )
      --v63;
    v64 = a5;
    if ( v63 < v88 )
    {
      *(_WORD *)a5 = 0;
      *(_BYTE *)(a5 + 3) = 1;
      *(_BYTE *)(a5 + 2) = v69 != -32768 ? 32 : 45;
      *v88 = 48;
      *(_BYTE *)(a5 + 5) = 0;
      return 1;
    }
  }
  v65 = (_BYTE)v63 - v64 - 3;
  *(_BYTE *)(v64 + 3) = v65;
  *(_BYTE *)(v65 + v64 + 4) = 0;
  return 1;
}

// ===== __hw_cw @ 0x004BA2E7..0x004BA375 =====
int __usercall _hw_cw@<eax>(int a1@<ebx>)
{
  int result; // eax
  int v2; // ecx

  result = (a1 & 0x10) != 0;
  if ( (a1 & 8) != 0 )
    result |= 4u;
  if ( (a1 & 4) != 0 )
    result |= 8u;
  if ( (a1 & 2) != 0 )
    result |= 0x10u;
  if ( (a1 & 1) != 0 )
    result |= 0x20u;
  if ( (a1 & 0x80000) != 0 )
    result |= 2u;
  v2 = a1 & 0x300;
  if ( (a1 & 0x300) != 0 )
  {
    switch ( v2 )
    {
      case 256:
        result |= 0x400u;
        break;
      case 512:
        result |= 0x800u;
        break;
      case 768:
        result |= 0xC00u;
        break;
    }
  }
  if ( (a1 & 0x30000) != 0 )
  {
    if ( (a1 & 0x30000) == 0x10000 )
      result |= 0x200u;
  }
  else
  {
    result |= 0x300u;
  }
  if ( (a1 & 0x40000) != 0 )
    return result | 0x1000;
  return result;
}

// ===== ___hw_cw_sse2 @ 0x004BA375..0x004BA415 =====
int __fastcall __hw_cw_sse2(int a1, int a2)
{
  int result; // eax
  int v3; // ecx
  int v4; // edx

  result = 0;
  if ( (a2 & 0x10) != 0 )
    result = 128;
  if ( (a2 & 8) != 0 )
    result |= 0x200u;
  if ( (a2 & 4) != 0 )
    result |= 0x400u;
  if ( (a2 & 2) != 0 )
    result |= 0x800u;
  if ( (a2 & 1) != 0 )
    result |= 0x1000u;
  if ( (a2 & 0x80000) != 0 )
    result |= 0x100u;
  v3 = a2 & 0x300;
  if ( (a2 & 0x300) != 0 )
  {
    switch ( v3 )
    {
      case 256:
        result |= 0x2000u;
        break;
      case 512:
        result |= 0x4000u;
        break;
      case 768:
        result |= 0x6000u;
        break;
    }
  }
  v4 = a2 & 0x3000000;
  switch ( v4 )
  {
    case 16777216:
      return result | 0x8040;
    case 33554432:
      return result | 0x40;
    case 50331648:
      return result | 0x8000;
  }
  return result;
}

// ===== __control87 @ 0x004BA415..0x004BA727 =====
unsigned int __cdecl _control87(unsigned int NewValue, unsigned int Mask)
{
  int v2; // edx
  int v3; // eax
  unsigned int result; // eax
  __int16 v6; // ax
  __int16 v7; // bx
  unsigned int v8; // edx
  int v9; // eax
  int v10; // esi
  __int16 v11; // ax
  int v12; // ecx
  int v13; // eax
  int v14; // eax
  unsigned int v15; // edx
  int v16; // eax
  __int16 v17; // cx
  int v18; // edx
  int v19; // eax
  int v20; // ecx
  int v21; // ecx
  int v22; // ecx
  unsigned int v23; // [esp+14h] [ebp-Ch]
  __int16 v24; // [esp+1Ch] [ebp-4h]
  unsigned int Maska; // [esp+2Ch] [ebp+Ch]

  v2 = 0;
  if ( (v24 & 1) != 0 )
    v2 = 16;
  if ( (v24 & 4) != 0 )
    v2 |= 8u;
  if ( (v24 & 8) != 0 )
    v2 |= 4u;
  if ( (v24 & 0x10) != 0 )
    v2 |= 2u;
  if ( (v24 & 0x20) != 0 )
    v2 |= 1u;
  if ( (v24 & 2) != 0 )
    v2 |= 0x80000u;
  v3 = v24 & 0xC00;
  if ( (v24 & 0xC00) != 0 )
  {
    switch ( v3 )
    {
      case 1024:
        v2 |= 0x100u;
        break;
      case 2048:
        v2 |= 0x200u;
        break;
      case 3072:
        v2 |= 0x300u;
        break;
    }
  }
  if ( (v24 & 0x300) != 0 )
  {
    if ( (v24 & 0x300) == 0x200 )
      v2 |= 0x10000u;
  }
  else
  {
    v2 |= 0x20000u;
  }
  if ( (v24 & 0x1000) != 0 )
    v2 |= 0x40000u;
  result = Mask & NewValue | v2 & ~Mask;
  Maska = result;
  if ( result != v2 )
  {
    v6 = _hw_cw(result);
    v7 = v6;
    v8 = 0;
    if ( (v6 & 1) != 0 )
      v8 = 16;
    if ( (v6 & 4) != 0 )
      v8 |= 8u;
    if ( (v6 & 8) != 0 )
      v8 |= 4u;
    if ( (v6 & 0x10) != 0 )
      v8 |= 2u;
    if ( (v6 & 0x20) != 0 )
      v8 |= 1u;
    if ( (v6 & 2) != 0 )
      v8 |= 0x80000u;
    v9 = v6 & 0xC00;
    if ( (v7 & 0xC00) != 0 )
    {
      switch ( v9 )
      {
        case 1024:
          v8 |= 0x100u;
          break;
        case 2048:
          v8 |= 0x200u;
          break;
        case 3072:
          v8 |= 0x300u;
          break;
      }
    }
    if ( (v7 & 0x300) != 0 )
    {
      if ( (v7 & 0x300) == 0x200 )
        v8 |= 0x10000u;
    }
    else
    {
      v8 |= 0x20000u;
    }
    if ( (v7 & 0x1000) != 0 )
      v8 |= 0x40000u;
    Maska = v8;
    result = v8;
  }
  v10 = 0;
  if ( dword_567C00 )
  {
    v11 = _mm_getcsr();
    if ( (v11 & 0x80u) != 0 )
      v10 = 16;
    if ( (v11 & 0x200) != 0 )
      v10 |= 8u;
    if ( (v11 & 0x400) != 0 )
      v10 |= 4u;
    if ( (v11 & 0x800) != 0 )
      v10 |= 2u;
    if ( (v11 & 0x1000) != 0 )
      v10 |= 1u;
    if ( (v11 & 0x100) != 0 )
      v10 |= 0x80000u;
    v12 = v11 & 0x6000;
    if ( (v11 & 0x6000) != 0 )
    {
      switch ( v12 )
      {
        case 8192:
          v10 |= 0x100u;
          break;
        case 16384:
          v10 |= 0x200u;
          break;
        case 24576:
          v10 |= 0x300u;
          break;
      }
    }
    v13 = (v11 & 0x8040) - 64;
    if ( v13 )
    {
      v14 = v13 - 32704;
      if ( v14 )
      {
        if ( v14 == 64 )
          v10 |= 0x1000000u;
      }
      else
      {
        v10 |= 0x3000000u;
      }
    }
    else
    {
      v10 |= 0x2000000u;
    }
    v15 = NewValue & Mask & 0x308031F | v10 & ~(Mask & 0x308031F);
    if ( v15 == v10 )
    {
      v16 = v10;
    }
    else
    {
      v23 = __hw_cw_sse2(v12, v15);
      __set_fpsr_sse2(v23);
      v17 = _mm_getcsr();
      v18 = 0;
      if ( (v17 & 0x80u) != 0 )
        v18 = 16;
      if ( (v17 & 0x200) != 0 )
        v18 |= 8u;
      if ( (v17 & 0x400) != 0 )
        v18 |= 4u;
      if ( (v17 & 0x800) != 0 )
        v18 |= 2u;
      if ( (v17 & 0x1000) != 0 )
        v18 |= 1u;
      if ( (v17 & 0x100) != 0 )
        v18 |= 0x80000u;
      v19 = v17 & 0x6000;
      if ( (v17 & 0x6000) != 0 )
      {
        switch ( v19 )
        {
          case 8192:
            v18 |= 0x100u;
            break;
          case 16384:
            v18 |= 0x200u;
            break;
          case 24576:
            v18 |= 0x300u;
            break;
        }
      }
      v20 = (v17 & 0x8040) - 64;
      if ( v20 )
      {
        v21 = v20 - 32704;
        if ( v21 )
        {
          if ( v21 == 64 )
            v18 |= 0x1000000u;
        }
        else
        {
          v18 |= 0x3000000u;
        }
      }
      else
      {
        v18 |= 0x2000000u;
      }
      v16 = v18;
    }
    v22 = Maska ^ v16;
    result = Maska | v16;
    if ( (v22 & 0x8031F) != 0 )
      result |= 0x80000000;
  }
  return result;
}

// ===== _ldexp @ 0x004BA727..0x004BA8D9 =====
double __cdecl ldexp(double X, int Y)
{
  __int16 v2; // cx
  int v3; // edi
  __int16 v4; // cx
  int v5; // eax
  double result; // st7
  double v7; // st7
  int v8; // eax
  double v9; // st7
  double v10; // st7
  __int16 v11; // cx
  double Number; // [esp+8h] [ebp-20h]
  double v13; // [esp+10h] [ebp-18h]
  double v14; // [esp+10h] [ebp-18h]
  __int16 v15; // [esp+18h] [ebp-10h]
  int v16; // [esp+18h] [ebp-10h]
  int v17; // [esp+24h] [ebp-4h] BYREF
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  v3 = _ctrlfp(v2);
  v4 = HIWORD(X) & 0x7FF0;
  if ( (HIWORD(X) & 0x7FF0) != 0x7FF0 )
  {
    if ( 0.0 == X )
    {
LABEL_6:
      _ctrlfp(v4);
      return X;
    }
    v7 = _decomp(X, &v17);
    if ( Y >= 0 )
    {
      if ( v17 > 0x7FFFFFFF - Y )
      {
LABEL_15:
        v16 = v3;
        v13 = _copysign(dbl_4FC200, v7);
        Number = (double)Y;
LABEL_16:
        result = X;
        _except2((int)&savedregs, 0x11u, 25, SLODWORD(X), SHIDWORD(X), Number, v13, v16);
        return result;
      }
    }
    else if ( v17 < (int)(0x80000000 - Y) )
    {
      goto LABEL_20;
    }
    v8 = v17 + Y;
    if ( v17 + Y > 2560 )
      goto LABEL_15;
    if ( v8 > 1024 )
    {
      v9 = _set_exp(v7, (unsigned __int16)v8 - 1536);
      v16 = v3;
      v13 = v9;
      Number = (double)Y;
      goto LABEL_16;
    }
    if ( v8 >= -2557 )
    {
      if ( v8 >= -1021 )
      {
        X = _set_exp(v7, v8);
        _ctrlfp(v11);
        return X;
      }
      v10 = _set_exp(v7, (unsigned __int16)v8 + 1536);
LABEL_21:
      v14 = v10;
      result = X;
      _except2((int)&savedregs, 0x12u, 25, SLODWORD(X), SHIDWORD(X), (double)Y, v14, v3);
      return result;
    }
LABEL_20:
    v10 = v7 * 0.0;
    goto LABEL_21;
  }
  v5 = _sptype(SLODWORD(X), SHIDWORD(X));
  v4 = v15;
  if ( v5 > 0 )
  {
    if ( v5 > 2 )
    {
      if ( v5 == 3 )
        return _handle_qnan2(25, X, (double)Y, v3);
      goto LABEL_7;
    }
    goto LABEL_6;
  }
LABEL_7:
  result = X;
  _except2((int)&savedregs, 8u, 25, SLODWORD(X), SHIDWORD(X), (double)Y, X + 1.0, v3);
  return result;
}

// ===== ___initconout @ 0x004BA8D9..0x004BA8F8 =====
HANDLE __initconout()
{
  HANDLE result; // eax

  result = CreateFileW(&FileName, 0x40000000u, 3u, 0, 3u, 0, 0);
  hConsoleOutput = result;
  return result;
}

// ===== sub_4BA8F8 @ 0x004BA8F8..0x004BA90F =====
HANDLE sub_4BA8F8()
{
  HANDLE result; // eax

  result = hConsoleOutput;
  if ( hConsoleOutput != (HANDLE)-1 && hConsoleOutput != (HANDLE)-2 )
    return (HANDLE)CloseHandle(hConsoleOutput);
  return result;
}

// ===== __fclose_nolock @ 0x004BA90F..0x004BA97C =====
int __cdecl _fclose_nolock(FILE *Stream)
{
  int v1; // edi
  int v3; // eax

  v1 = -1;
  if ( Stream )
  {
    if ( (Stream->_flag & 0x83) != 0 )
    {
      v1 = _flush(Stream);
      _freebuf(Stream);
      v3 = _fileno(Stream);
      if ( _close(v3) >= 0 )
      {
        if ( Stream->_tmpfname )
        {
          free(Stream->_tmpfname);
          Stream->_tmpfname = 0;
        }
      }
      else
      {
        v1 = -1;
      }
    }
    Stream->_flag = 0;
    return v1;
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
}

// ===== _fclose @ 0x004BA97C..0x004BA9F0 =====
int __cdecl fclose(FILE *Stream)
{
  int v2; // [esp+10h] [ebp-1Ch]

  v2 = -1;
  if ( Stream )
  {
    if ( (Stream->_flag & 0x40) != 0 )
    {
      Stream->_flag = 0;
    }
    else
    {
      _lock_file(Stream);
      v2 = _fclose_nolock(Stream);
      _unlock_file(Stream);
    }
    return v2;
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
}

// ===== __commit @ 0x004BA9F0..0x004BAAC9 =====
int __cdecl _commit(int FileHandle)
{
  int *v2; // edi
  int v3; // esi
  void *osfhandle; // eax
  DWORD LastError; // [esp+14h] [ebp-1Ch]

  if ( FileHandle == -2 )
  {
    *_errno() = 9;
    return -1;
  }
  if ( FileHandle < 0
    || FileHandle >= uNumber
    || (v2 = &dword_567B00[FileHandle >> 5], v3 = (FileHandle & 0x1F) << 6, (*(_BYTE *)(v3 + *v2 + 4) & 1) == 0) )
  {
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return -1;
  }
  __lock_fhandle(FileHandle);
  if ( (*(_BYTE *)(v3 + *v2 + 4) & 1) != 0 )
  {
    osfhandle = (void *)_get_osfhandle(FileHandle);
    if ( FlushFileBuffers(osfhandle) )
      LastError = 0;
    else
      LastError = GetLastError();
    if ( !LastError )
      goto $good$28895;
    *__doserrno() = LastError;
  }
  *_errno() = 9;
  LastError = -1;
$good$28895:
  _unlock_fhandle(FileHandle);
  return LastError;
}

// ===== ___ascii_strnicmp @ 0x004BAAD0..0x004BAB31 =====
int __cdecl __ascii_strnicmp(unsigned __int8 *a1, unsigned __int8 *a2, int a3)
{
  int v3; // ecx
  unsigned __int8 v6; // ah
  unsigned __int8 v7; // al
  bool v8; // cf

  v3 = a3;
  if ( a3 )
  {
    do
    {
      v6 = *a1;
      v7 = *a2;
      if ( !*a1 || !v7 )
        break;
      ++a1;
      ++a2;
      if ( v6 >= 0x41u && v6 <= 0x5Au )
        v6 += 32;
      if ( v7 >= 0x41u && v7 <= 0x5Au )
        v7 += 32;
      v8 = v6 < v7;
      if ( v6 != v7 )
        goto differ;
      --v3;
    }
    while ( v3 );
    v3 = 0;
    v8 = v6 < v7;
    if ( v6 == v7 )
      return v3;
differ:
    v3 = -1;
    if ( !v8 )
      return 1;
  }
  return v3;
}

// ===== ___mtold12 @ 0x004BAB31..0x004BAD15 =====
unsigned int *__cdecl __mtold12(char *a1, int a2, unsigned int *a3)
{
  unsigned int *result; // eax
  __int64 v4; // rcx
  unsigned int v5; // edx
  unsigned int v6; // esi
  int v7; // edx
  unsigned int v8; // edi
  unsigned int v9; // esi
  unsigned int v11; // edx
  unsigned int v12; // edx
  int v13; // esi
  unsigned __int64 v14; // kr00_8
  unsigned int v15; // esi
  int v16; // edx
  int v17; // esi
  unsigned int v18; // edx
  unsigned int v19; // edi
  unsigned int v20; // ecx
  int v21; // edi
  unsigned int v22; // ecx
  __int64 v23; // kr08_8
  unsigned int v24; // [esp+Ch] [ebp-18h]
  unsigned int v25; // [esp+10h] [ebp-14h]
  unsigned int v26; // [esp+14h] [ebp-10h]
  int v27; // [esp+18h] [ebp-Ch]
  unsigned int v28; // [esp+1Ch] [ebp-8h]
  int v29; // [esp+1Ch] [ebp-8h]
  __int16 v30; // [esp+20h] [ebp-4h]
  unsigned int v31; // [esp+34h] [ebp+10h]
  int v32; // [esp+34h] [ebp+10h]
  int v33; // [esp+34h] [ebp+10h]
  int v34; // [esp+34h] [ebp+10h]

  result = a3;
  HIDWORD(v4) = 0;
  v30 = 16462;
  *a3 = 0;
  a3[1] = 0;
  a3[2] = 0;
  if ( a2 )
  {
    LODWORD(v4) = 0;
    v31 = 0;
    do
    {
      v24 = *result;
      v25 = result[1];
      v26 = result[2];
      v5 = v31;
      v32 = 0;
      v6 = HIDWORD(v4);
      HIDWORD(v4) = v4 >> 31;
      v7 = (v6 >> 31) | (2 * v5);
      v8 = __SPAIR64__(HIDWORD(v4), 2 * (int)v4) >> 31;
      v28 = v8;
      v9 = 4 * v4;
      LODWORD(v4) = (HIDWORD(v4) >> 31) | (2 * v7);
      v11 = v9 + *result;
      *result = v9;
      result[1] = v8;
      result[2] = v4;
      if ( v11 < v9 || v11 < v24 )
        v32 = 1;
      *result = v11;
      if ( v32 )
      {
        v33 = 0;
        ++v8;
        if ( v28 + 1 < v28 || v28 == -1 )
          v33 = 1;
        result[1] = v8;
        if ( v33 )
        {
          LODWORD(v4) = v4 + 1;
          result[2] = v4;
        }
      }
      v34 = 0;
      HIDWORD(v4) = v8 + v25;
      if ( v8 + v25 < v8 || HIDWORD(v4) < v25 )
        v34 = 1;
      result[1] = HIDWORD(v4);
      if ( v34 )
      {
        LODWORD(v4) = v4 + 1;
        result[2] = v4;
      }
      v29 = 0;
      LODWORD(v4) = (HIDWORD(v4) >> 31) | (2 * (v26 + v4));
      v14 = 2LL * v11;
      v13 = HIDWORD(v14);
      v12 = v14;
      HIDWORD(v4) = v13 | (2 * HIDWORD(v4));
      result[2] = v4;
      v27 = v4;
      v31 = v4;
      *result = v12;
      result[1] = HIDWORD(v4);
      v15 = *a1;
      LODWORD(v4) = v12 + v15;
      if ( v12 + v15 < v12 || (unsigned int)v4 < v15 )
        v29 = 1;
      *result = v4;
      if ( v29 )
      {
        v16 = HIDWORD(v4) + 1;
        v17 = 0;
        if ( (unsigned int)(HIDWORD(v4) + 1) < HIDWORD(v4) || HIDWORD(v4) == -1 )
          v17 = 1;
        ++HIDWORD(v4);
        result[1] = v16;
        if ( v17 )
        {
          v31 = v27 + 1;
          result[2] = v27 + 1;
        }
      }
      --a2;
      ++a1;
      result[1] = HIDWORD(v4);
      result[2] = v31;
    }
    while ( a2 );
  }
  if ( !result[2] )
  {
    v18 = result[1];
    do
    {
      v30 -= 16;
      v19 = HIWORD(v18);
      v18 = HIWORD(*result) | (v18 << 16);
      v20 = *result << 16;
      result[1] = v18;
      *result = v20;
    }
    while ( !v19 );
    result[2] = v19;
  }
  v21 = result[2];
  if ( (v21 & 0x8000) == 0 )
  {
    v22 = result[1];
    do
    {
      --v30;
      v21 = (v22 >> 31) | (2 * v21);
      v23 = 2LL * *result;
      v22 = HIDWORD(v23) | (2 * v22);
      *result = v23;
      result[1] = v22;
      result[2] = v21;
    }
    while ( (v21 & 0x8000) == 0 );
  }
  *((_WORD *)result + 5) = v30;
  return result;
}

// ===== __close_nolock @ 0x004BAD15..0x004BADB1 =====
int __cdecl _close_nolock(int FileHandle)
{
  intptr_t osfhandle; // edi
  void *v2; // eax
  DWORD LastError; // edi

  if ( _get_osfhandle(FileHandle) == -1
    || (FileHandle == 1 && (*(_BYTE *)(dword_567B00[0] + 132) & 1) != 0
     || FileHandle == 2 && (*(_BYTE *)(dword_567B00[0] + 68) & 1) != 0)
    && (osfhandle = _get_osfhandle(2), _get_osfhandle(1) == osfhandle)
    || (v2 = (void *)_get_osfhandle(FileHandle), CloseHandle(v2)) )
  {
    LastError = 0;
  }
  else
  {
    LastError = GetLastError();
  }
  _free_osfhnd(FileHandle);
  *(_BYTE *)(dword_567B00[FileHandle >> 5] + ((FileHandle & 0x1F) << 6) + 4) = 0;
  if ( !LastError )
    return 0;
  _dosmaperr(LastError);
  return -1;
}

// ===== __close @ 0x004BADB1..0x004BAE75 =====
int __cdecl _close(int FileHandle)
{
  int *v2; // edi
  int v3; // esi
  int v4; // [esp+14h] [ebp-1Ch]

  if ( FileHandle == -2 )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    return -1;
  }
  if ( FileHandle < 0
    || FileHandle >= uNumber
    || (v2 = &dword_567B00[FileHandle >> 5], v3 = (FileHandle & 0x1F) << 6, (*(_BYTE *)(*v2 + v3 + 4) & 1) == 0) )
  {
    *__doserrno() = 0;
    *_errno() = 9;
    _invalid_parameter_noinfo();
    return -1;
  }
  __lock_fhandle(FileHandle);
  if ( (*(_BYTE *)(*v2 + v3 + 4) & 1) != 0 )
  {
    v4 = _close_nolock(FileHandle);
  }
  else
  {
    *_errno() = 9;
    v4 = -1;
  }
  _unlock_fhandle(FileHandle);
  return v4;
}

// ===== __freebuf @ 0x004BAE75..0x004BAEA6 =====
int __cdecl _freebuf(int a1)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 12);
  if ( (result & 0x83) != 0 && (result & 8) != 0 )
  {
    free(*(void **)(a1 + 8));
    *(_DWORD *)(a1 + 12) &= 0xFFFFFBF7;
    result = 0;
    *(_DWORD *)a1 = 0;
    *(_DWORD *)(a1 + 8) = 0;
    *(_DWORD *)(a1 + 4) = 0;
  }
  return result;
}

// ===== sub_4BAEB0 @ 0x004BAEB0..0x004BAFA6 =====
int __cdecl sub_4BAEB0(char *a1)
{
  int v1; // eax
  int v2; // ebp
  int v3; // ebx
  int v4; // esi

  if ( a1 )
  {
    sub_4C7CC0(a1 + 592);
    sub_4C80A0(a1 + 480);
    sub_4D41B0(a1 + 120);
    if ( *((_DWORD *)a1 + 18) )
    {
      v1 = *((_DWORD *)a1 + 13);
      if ( v1 )
      {
        v2 = 0;
        if ( v1 > 0 )
        {
          v3 = 0;
          v4 = 0;
          do
          {
            sub_4C74B0(*((_DWORD *)a1 + 18) + v4);
            sub_4C73F0(v3 + *((_DWORD *)a1 + 19));
            ++v2;
            v4 += 32;
            v3 += 16;
          }
          while ( v2 < *((_DWORD *)a1 + 13) );
        }
        free(*((void **)a1 + 18));
        free(*((void **)a1 + 19));
      }
    }
    if ( *((_DWORD *)a1 + 15) )
      free(*((void **)a1 + 15));
    if ( *((_DWORD *)a1 + 17) )
      free(*((void **)a1 + 17));
    if ( *((_DWORD *)a1 + 16) )
      free(*((void **)a1 + 16));
    if ( *((_DWORD *)a1 + 14) )
      free(*((void **)a1 + 14));
    sub_4D4350(a1 + 24);
    if ( *(_DWORD *)a1 )
      (*((void (__cdecl **)(_DWORD))a1 + 178))(*(_DWORD *)a1);
    memset(a1, 0, 0x2D0u);
  }
  return 0;
}

// ===== sub_4BAFB0 @ 0x004BAFB0..0x004BAFFC =====
int __cdecl sub_4BAFB0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  int result; // eax

  result = sub_4BB000(a1, a2, a3, a4, a5, a6, a7, a8);
  if ( !result )
    return sub_4BB3A0(a2);
  return result;
}

// ===== sub_4BB000 @ 0x004BB000..0x004BB111 =====
int __cdecl sub_4BB000(
        int a1,
        _DWORD *a2,
        const void *a3,
        unsigned int a4,
        int a5,
        int (__cdecl *a6)(int, _DWORD, _DWORD, int),
        int a7,
        int a8)
{
  void *v9; // eax
  int result; // eax
  int v11; // edi
  size_t v12; // [esp-10h] [ebp-20h]
  int v13; // [esp+14h] [ebp+4h]

  if ( a1 )
    v13 = a6(a1, 0, 0, 1);
  else
    v13 = -1;
  memset(a2, 0, 0x2D0u);
  *a2 = a1;
  a2[176] = a5;
  a2[177] = a6;
  a2[178] = a7;
  a2[179] = a8;
  sub_4D4330(a2 + 6);
  if ( a3 )
  {
    qmemcpy((void *)sub_4D4380(a2 + 6, a4), a3, a4);
    sub_4D4400(a2 + 6, a4);
  }
  if ( v13 != -1 )
    a2[1] = 1;
  a2[13] = 1;
  v9 = calloc(1u, 0x20u);
  v12 = a2[13];
  a2[18] = v9;
  a2[19] = calloc(v12, 0x10u);
  sub_4D4140(a2 + 30, -1);
  result = sub_4BB120(a2, a2[18], a2[19], a2 + 23, 0);
  v11 = result;
  if ( result >= 0 )
  {
    if ( (int)a2[22] < 1 )
      a2[22] = 1;
  }
  else
  {
    *a2 = 0;
    sub_4BAEB0((char *)a2);
    return v11;
  }
  return result;
}

// ===== sub_4BB120 @ 0x004BB120..0x004BB261 =====
int __cdecl sub_4BB120(int a1, int a2, int a3, _DWORD *a4, _BYTE *a5)
{
  _BYTE *v5; // ebx
  int result; // eax
  int v7; // edx
  int v8; // esi
  int v9; // eax
  int v10; // edi
  int v11; // eax
  int v12; // esi
  int v13; // edx
  _BYTE v14[16]; // [esp+10h] [ebp-30h] BYREF
  _BYTE v15[32]; // [esp+20h] [ebp-20h] BYREF

  v5 = a5;
  if ( !a5 )
  {
    result = sub_4BB270(a1, v14, 8500, 0);
    if ( result == -128 && v7 == -1 )
      return result;
    if ( v7 < 0 )
      return -132;
    v5 = v14;
  }
  v8 = a1 + 120;
  v9 = sub_4D40E0(v5);
  sub_4D48D0(a1 + 120, v9);
  if ( a4 )
    *a4 = *(_DWORD *)(a1 + 456);
  *(_DWORD *)(a1 + 88) = 3;
  sub_4C7480(a2);
  sub_4C73D0(a3);
  v10 = 0;
LABEL_10:
  sub_4D4570(v8, v5);
  do
  {
    v8 = a1 + 120;
    v11 = sub_4D48F0(a1 + 120, v15);
    if ( !v11 )
    {
      sub_4BB270(a1, v5, 8500, 0);
      if ( v13 >= 0 )
        goto LABEL_10;
LABEL_17:
      v12 = -133;
LABEL_18:
      sub_4C74B0(a2);
      sub_4C73F0(a3);
      result = v12;
      *(_DWORD *)(a1 + 88) = 2;
      return result;
    }
    if ( v11 == -1 )
      goto LABEL_17;
    v12 = sub_4C7620(a2, a3, v15);
    if ( v12 )
      goto LABEL_18;
    ++v10;
  }
  while ( v10 < 3 );
  return 0;
}

// ===== sub_4BB270 @ 0x004BB270..0x004BB32E =====
int __cdecl sub_4BB270(int a1, int a2, __int64 a3)
{
  __int64 v3; // kr00_8
  int v4; // eax
  int v5; // eax
  int v7; // ecx

  v3 = a3;
  if ( a3 > 0 )
    v3 = *(_QWORD *)(a1 + 8) + a3;
  while ( 1 )
  {
    while ( 1 )
    {
      if ( v3 > 0 && *(_QWORD *)(a1 + 8) >= v3 )
        return -1;
      v4 = sub_4D4420(a1 + 24, a2);
      if ( v4 >= 0 )
        break;
      *(_QWORD *)(a1 + 8) -= v4;
    }
    if ( v4 )
      break;
    if ( !v3 )
      return -1;
    v5 = sub_4BB330(a1);
    if ( !v5 )
      return -2;
    if ( v5 < 0 )
      return -128;
  }
  v7 = *(_DWORD *)(a1 + 8);
  *(_QWORD *)(a1 + 8) += v4;
  return v7;
}

// ===== sub_4BB330 @ 0x004BB330..0x004BB394 =====
int __cdecl sub_4BB330(int a1)
{
  int v1; // eax
  int v2; // eax
  int v3; // esi

  *_errno() = 0;
  if ( !*(_DWORD *)a1 )
    return 0;
  v1 = sub_4D4380(a1 + 24, 8500);
  v2 = (*(int (__cdecl **)(int, int, int, _DWORD))(a1 + 704))(v1, 1, 8500, *(_DWORD *)a1);
  v3 = v2;
  if ( v2 > 0 )
    sub_4D4400(a1 + 24, v2);
  if ( v3 || !*_errno() )
    return v3;
  else
    return -1;
}

// ===== sub_4BB3A0 @ 0x004BB3A0..0x004BB3E1 =====
int __cdecl sub_4BB3A0(int a1)
{
  int v1; // edi

  if ( *(int *)(a1 + 88) < 2 )
    *(_DWORD *)(a1 + 88) = 2;
  if ( !*(_DWORD *)(a1 + 4) )
    return 0;
  v1 = sub_4BB3F0(a1);
  if ( v1 )
  {
    *(_DWORD *)a1 = 0;
    sub_4BAEB0((char *)a1);
  }
  return v1;
}

// ===== sub_4BB3F0 @ 0x004BB3F0..0x004BB4E9 =====
int __cdecl sub_4BB3F0(int a1)
{
  int v1; // ebp
  int v2; // eax
  __int64 v3; // rax
  unsigned int v4; // edi
  unsigned int v5; // ebx
  int v7; // [esp+10h] [ebp-18h]
  int v8; // [esp+14h] [ebp-14h]
  _BYTE v9[16]; // [esp+18h] [ebp-10h] BYREF

  v1 = *(_DWORD *)(a1 + 92);
  v7 = *(_DWORD *)(a1 + 8);
  v8 = *(_DWORD *)(a1 + 12);
  (*(void (__cdecl **)(_DWORD, _DWORD, _DWORD, int))(a1 + 708))(*(_DWORD *)a1, 0, 0, 2);
  v2 = (*(int (__cdecl **)(_DWORD))(a1 + 716))(*(_DWORD *)a1);
  *(_QWORD *)(a1 + 16) = v2;
  *(_DWORD *)(a1 + 8) = *(_DWORD *)(a1 + 16);
  *(_DWORD *)(a1 + 12) = v2 >> 31;
  v3 = sub_4BB4F0(a1, v9);
  v4 = HIDWORD(v3);
  v5 = v3;
  if ( v3 >= 0 )
  {
    if ( sub_4D40E0(v9) == v1 )
    {
      if ( sub_4BB650(a1, 0, 0, v5, v4, v5 + 1, (__PAIR64__(v4, v5) + 1) >> 32, v1, 0) )
      {
        LODWORD(v3) = -128;
        return v3;
      }
    }
    else if ( (int)sub_4BB650(a1, 0, 0, 0, 0, v5 + 1, (__PAIR64__(v4, v5) + 1) >> 32, v1, 0) < 0 )
    {
      LODWORD(v3) = -128;
      return v3;
    }
    sub_4BB870(a1, v7, v8);
    LODWORD(v3) = sub_4BBBC0(a1, 0, 0);
  }
  return v3;
}

// ===== sub_4BB4F0 @ 0x004BB4F0..0x004BB602 =====
int __cdecl sub_4BB4F0(int a1, int a2)
{
  int v2; // ebp
  unsigned int v3; // ecx
  unsigned int v4; // eax
  unsigned int v5; // ebx
  unsigned int v6; // edi
  unsigned __int64 i; // kr00_8
  unsigned __int64 v8; // kr08_8
  unsigned int v9; // eax
  unsigned int v10; // ecx
  int v11; // eax
  int v12; // edx
  int v13; // edx
  unsigned __int64 v15; // [esp+10h] [ebp-10h]
  int v16; // [esp+1Ch] [ebp-4h]

  v2 = -1;
  v3 = *(_DWORD *)(a1 + 8);
  v4 = *(_DWORD *)(a1 + 12);
  v5 = v3;
  v6 = v4;
  v16 = -1;
  for ( i = __PAIR64__(v4, v3); ; i = v15 )
  {
    v8 = i - 8500;
    v15 = i - 8500;
    if ( (((i - 8500) >> 32) & 0x80000000) != 0LL )
    {
      v15 = 0LL;
      v8 = 0LL;
    }
    sub_4BB610(a1, v8, HIDWORD(v8));
    v9 = *(_DWORD *)(a1 + 12);
    v10 = *(_DWORD *)(a1 + 8);
    if ( __SPAIR64__(v9, v10) < __SPAIR64__(v6, v5) )
      break;
LABEL_10:
    if ( (v16 & v2) != 0xFFFFFFFF )
    {
      sub_4BB610(a1, v2, v16);
      sub_4BB270(a1, a2, 8500LL);
      if ( v13 < 0 )
        return -129;
      else
        return v2;
    }
  }
  while ( 1 )
  {
    v11 = sub_4BB270(a1, a2, __PAIR64__(v6, v5) - __PAIR64__(v9, v10));
    if ( v11 == -128 && v12 == -1 )
      return -128;
    if ( v12 >= 0 )
    {
      v10 = *(_DWORD *)(a1 + 8);
      v2 = v11;
      v9 = *(_DWORD *)(a1 + 12);
      v16 = v12;
      if ( __SPAIR64__(v9, v10) < __SPAIR64__(v6, v5) )
        continue;
    }
    goto LABEL_10;
  }
}

// ===== sub_4BB610 @ 0x004BB610..0x004BB646 =====
int __cdecl sub_4BB610(int *a1, int a2, int a3)
{
  int result; // eax

  result = *a1;
  if ( *a1 )
  {
    ((void (__cdecl *)(int, int, int, _DWORD))a1[177])(result, a2, a3, 0);
    a1[2] = a2;
    a1[3] = a3;
    return sub_4D4860(a1 + 6);
  }
  return result;
}

// ===== sub_4BB650 @ 0x004BB650..0x004BB86C =====
int __cdecl sub_4BB650(int *a1, int a2, int a3, signed __int64 a4, signed __int64 a5, int a6, int a7)
{
  unsigned int v7; // ebp
  signed __int64 v8; // kr00_8
  signed __int64 v9; // kr10_8
  __int64 v10; // kr08_8
  int v11; // eax
  int v12; // edx
  int v13; // edi
  int result; // eax
  int v15; // edx
  int v16; // esi
  int v17; // eax
  void *v18; // eax
  int v19; // edx
  int v20; // ecx
  signed __int64 v21; // [esp+10h] [ebp-28h]
  unsigned __int64 v22; // [esp+18h] [ebp-20h]
  int v23; // [esp+20h] [ebp-18h]
  char v24[4]; // [esp+28h] [ebp-10h] BYREF
  int v25; // [esp+2Ch] [ebp-Ch]
  int v26; // [esp+34h] [ebp-4h]

  v7 = HIDWORD(a5);
  v21 = a5;
  v22 = a5;
  v8 = a5;
  v9 = a4;
  if ( a4 < a5 )
  {
    while ( 1 )
    {
      if ( v8 - v9 >= 8500 )
        v10 = (v9 + v8) / 2;
      else
        v10 = v9;
      sub_4BB610(a1, v10, SHIDWORD(v10));
      v11 = sub_4BB270((int)a1, (int)v24, -1LL);
      v23 = v11;
      v13 = v12;
      if ( v11 == -128 && v12 == -1 )
        return -128;
      if ( v12 >= 0 )
      {
        if ( sub_4D40E0(v24) == a6 )
        {
          v9 = __PAIR64__(v13, v23) + v25 + (__int64)v26;
          goto LABEL_15;
        }
        v11 = v23;
      }
      v21 = v10;
      if ( v13 >= 0 )
        v22 = __PAIR64__(v13, v11);
LABEL_15:
      if ( v9 >= v21 )
      {
        v7 = HIDWORD(a5);
        break;
      }
      v8 = v21;
    }
  }
  sub_4BB610(a1, v22, SHIDWORD(v22));
  result = sub_4BB270((int)a1, (int)v24, -1LL);
  if ( result != -128 || v15 != -1 )
  {
    if ( v9 >= __SPAIR64__(v7, a5) || v15 < 0 )
    {
      a1[13] = a7 + 1;
      v18 = malloc(8 * (a7 + 1) + 8);
      v19 = a1[13];
      a1[14] = (int)v18;
      a1[16] = (int)malloc(4 * v19);
      *(_QWORD *)(a1[14] + 8 * (a7 + 1)) = v9;
      v16 = a7;
    }
    else
    {
      v16 = a7;
      v17 = sub_4D40E0(v24);
      if ( sub_4BB650(a1, v22, HIDWORD(v22), a1[2], a1[3], a5, v7, v17, a7 + 1) == -128 )
        return -128;
    }
    v20 = a1[14];
    *(_DWORD *)(v20 + 8 * v16) = a2;
    *(_DWORD *)(v20 + 8 * v16 + 4) = a3;
    result = 0;
    *(_DWORD *)(a1[16] + 4 * v16) = a6;
  }
  return result;
}

// ===== sub_4BB870 @ 0x004BB870..0x004BBB4D =====
int __cdecl sub_4BB870(int a1, int a2, int a3)
{
  void *v3; // eax
  int v4; // edx
  void *v5; // eax
  int v6; // ecx
  void *v7; // eax
  int v8; // edx
  int result; // eax
  int i; // ebx
  int *v11; // edx
  bool v12; // sf
  _DWORD *v13; // edi
  int v14; // edi
  int v15; // edx
  int v16; // eax
  bool v17; // cc
  int v18; // eax
  int v19; // eax
  __int64 v20; // rax
  int v21; // eax
  int v22; // edx
  int v23; // edi
  int v24; // ebp
  __int64 v25; // rax
  _QWORD *v26; // edi
  __int64 v27; // [esp+Ch] [ebp-38h]
  _BYTE v28[16]; // [esp+14h] [ebp-30h] BYREF
  _BYTE v29[32]; // [esp+24h] [ebp-20h] BYREF

  v3 = realloc(*(void **)(a1 + 72), 32 * *(_DWORD *)(a1 + 52));
  v4 = *(_DWORD *)(a1 + 52);
  *(_DWORD *)(a1 + 72) = v3;
  v5 = realloc(*(void **)(a1 + 76), 16 * v4);
  v6 = *(_DWORD *)(a1 + 52);
  *(_DWORD *)(a1 + 76) = v5;
  v7 = malloc(8 * v6);
  v8 = *(_DWORD *)(a1 + 52);
  *(_DWORD *)(a1 + 60) = v7;
  *(_DWORD *)(a1 + 68) = malloc(16 * v8);
  result = *(_DWORD *)(a1 + 52);
  for ( i = 0; i < result; ++i )
  {
    if ( i )
    {
      sub_4BB610((int *)a1, *(_DWORD *)(*(_DWORD *)(a1 + 56) + 8 * i), *(_DWORD *)(*(_DWORD *)(a1 + 56) + 8 * i + 4));
      v12 = sub_4BB120(a1, *(_DWORD *)(a1 + 72) + 32 * i, *(_DWORD *)(a1 + 76) + 16 * i, 0, 0) < 0;
      v13 = (_DWORD *)(*(_DWORD *)(a1 + 60) + 8 * i);
      if ( v12 )
      {
        *v13 = -1;
        v13[1] = -1;
      }
      else
      {
        *v13 = *(_DWORD *)(a1 + 8);
        v13[1] = *(_DWORD *)(a1 + 12);
      }
    }
    else
    {
      v11 = *(int **)(a1 + 60);
      *v11 = a2;
      v11[1] = a3;
      sub_4BB610((int *)a1, a2, a3);
    }
    if ( (*(_DWORD *)(*(_DWORD *)(a1 + 60) + 8 * i + 4) & *(_DWORD *)(*(_DWORD *)(a1 + 60) + 8 * i)) != -1 )
    {
      v27 = 0LL;
      v14 = -1;
      sub_4D48D0(a1 + 120, *(_DWORD *)(*(_DWORD *)(a1 + 64) + 4 * i));
      while ( 1 )
      {
        sub_4BB270(a1, (int)v28, -1LL);
        if ( v15 < 0 || sub_4D40E0(v28) != *(_DWORD *)(*(_DWORD *)(a1 + 64) + 4 * i) )
          break;
        sub_4D4570(a1 + 120, v28);
        v16 = sub_4D48F0(a1 + 120, v29);
        v17 = v16 <= 0;
        if ( v16 )
        {
          do
          {
            if ( !v17 )
            {
              v18 = sub_4C8A00(*(_DWORD *)(a1 + 72) + 32 * i, v29);
              if ( v14 != -1 )
                v27 += (v18 + v14) >> 2;
              v14 = v18;
            }
            v19 = sub_4D48F0(a1 + 120, v29);
            v17 = v19 <= 0;
          }
          while ( v19 );
        }
        v20 = sub_4D4000(v28);
        if ( (HIDWORD(v20) & (unsigned int)v20) != 0xFFFFFFFF )
        {
          v27 = sub_4D4000(v28) - v27;
          break;
        }
      }
      if ( v27 < 0 )
        v27 = 0LL;
      *(_QWORD *)(*(_DWORD *)(a1 + 68) + 16 * i) = v27;
    }
    sub_4BB610((int *)a1, *(_DWORD *)(*(_DWORD *)(a1 + 56) + 8 * i + 8), *(_DWORD *)(*(_DWORD *)(a1 + 56) + 8 * i + 12));
    while ( 1 )
    {
      v21 = sub_4BB4F0(a1, (int)v28);
      v23 = v22;
      v24 = v21;
      if ( v22 < 0 )
      {
        sub_4C74B0(*(_DWORD *)(a1 + 72) + 32 * i);
        sub_4C73F0(*(_DWORD *)(a1 + 76) + 16 * i);
        goto LABEL_28;
      }
      v25 = sub_4D4000(v28);
      if ( (HIDWORD(v25) & (unsigned int)v25) != 0xFFFFFFFF )
        break;
      *(_DWORD *)(a1 + 8) = v24;
      *(_DWORD *)(a1 + 12) = v23;
    }
    v26 = (_QWORD *)(16 * i + *(_DWORD *)(a1 + 68));
    v26[1] = sub_4D4000(v28) - *v26;
LABEL_28:
    result = *(_DWORD *)(a1 + 52);
  }
  return result;
}

// ===== sub_4BBB50 @ 0x004BBB50..0x004BBBB7 =====
int __cdecl sub_4BBB50(_DWORD *a1, int a2)
{
  int v2; // eax
  int v3; // esi
  __int64 v4; // kr00_8
  __int64 v5; // rax

  if ( (int)a1[22] >= 2 && a1[1] && (v2 = a1[13], a2 < v2) )
  {
    if ( a2 >= 0 )
    {
      return *(_QWORD *)(a1[17] + 16 * a2 + 8);
    }
    else
    {
      v3 = 0;
      v4 = 0LL;
      if ( v2 > 0 )
      {
        do
          v4 += sub_4BBB50(a1, v3++);
        while ( v3 < a1[13] );
      }
      LODWORD(v5) = v4;
    }
  }
  else
  {
    LODWORD(v5) = -131;
  }
  return v5;
}

// ===== sub_4BBBC0 @ 0x004BBBC0..0x004BBE86 =====
int __cdecl sub_4BBBC0(int a1, __int64 a2)
{
  int v3; // ebp
  int v4; // ebx
  int v5; // eax
  int v6; // edx
  bool v7; // cc
  int v8; // eax
  int v9; // edx
  int v10; // ecx
  _DWORD *v11; // edi
  int v12; // edx
  int v13; // eax
  __int64 v14; // kr00_8
  _QWORD *v15; // eax
  __int64 v16; // kr08_8
  int v17; // edx
  int v18; // [esp+10h] [ebp-1A0h]
  int v19; // [esp+14h] [ebp-19Ch]
  _BYTE v20[16]; // [esp+18h] [ebp-198h] BYREF
  _BYTE v21[16]; // [esp+28h] [ebp-188h] BYREF
  __int64 v22; // [esp+38h] [ebp-178h]
  _BYTE v23[360]; // [esp+48h] [ebp-168h] BYREF

  if ( *(int *)(a1 + 88) < 2 )
    return -131;
  if ( !*(_DWORD *)(a1 + 4) )
    return -138;
  if ( a2 < 0 || a2 > *(_QWORD *)(a1 + 16) )
    return -131;
  *(_DWORD *)(a1 + 80) = -1;
  *(_DWORD *)(a1 + 84) = -1;
  sub_4BBE90(a1);
  sub_4BB610((int *)a1, a2, SHIDWORD(a2));
  v3 = 0;
  v18 = 0;
  sub_4D4140(v23, -1);
  v4 = v19;
  while ( 1 )
  {
    while ( *(_DWORD *)(a1 + 88) != 3 || (int)sub_4D48F0(v23, v21) <= 0 )
    {
      if ( v3 )
      {
        *(_DWORD *)(a1 + 80) = -1;
        *(_DWORD *)(a1 + 84) = -1;
        goto LABEL_40;
      }
      sub_4BB270(a1, (int)v20, -1LL);
      if ( v6 < 0 )
      {
        *(_DWORD *)(a1 + 80) = sub_4BBB50((_DWORD *)a1, -1);
        *(_DWORD *)(a1 + 84) = v17;
        goto LABEL_40;
      }
      v7 = *(_DWORD *)(a1 + 88) < 3;
      if ( *(_DWORD *)(a1 + 88) == 3 )
      {
        if ( *(_DWORD *)(a1 + 92) != sub_4D40E0(v20) )
        {
          sub_4BBE90(a1);
          sub_4D41B0(v23);
        }
        v7 = *(_DWORD *)(a1 + 88) < 3;
      }
      if ( v7 )
      {
        v8 = sub_4D40E0(v20);
        v9 = *(_DWORD *)(a1 + 52);
        v10 = 0;
        *(_DWORD *)(a1 + 92) = v8;
        if ( v9 > 0 )
        {
          v11 = *(_DWORD **)(a1 + 64);
          do
          {
            if ( *v11 == v8 )
              break;
            ++v10;
            ++v11;
          }
          while ( v10 < v9 );
        }
        if ( v10 == v9 )
        {
          *(_DWORD *)(a1 + 80) = -1;
          *(_DWORD *)(a1 + 84) = -1;
          sub_4D41B0(v23);
          sub_4BBE90(a1);
          return -137;
        }
        *(_DWORD *)(a1 + 96) = v10;
        sub_4D48D0(a1 + 120, v8);
        sub_4D48D0(v23, *(_DWORD *)(a1 + 92));
        *(_DWORD *)(a1 + 88) = 3;
      }
      sub_4D4570(a1 + 120, v20);
      sub_4D4570(v23, v20);
      v4 = sub_4D3FF0(v20);
    }
    v5 = *(_DWORD *)(a1 + 72) + 32 * *(_DWORD *)(a1 + 96);
    if ( *(_DWORD *)(v5 + 28) )
      v19 = sub_4C8A00(v5, v21);
    if ( v4 )
    {
      sub_4D48F0(a1 + 120, 0);
    }
    else if ( v3 )
    {
      v18 += (v3 + v19) >> 2;
    }
    if ( (HIDWORD(v22) & (unsigned int)v22) != 0xFFFFFFFF )
      break;
    v3 = v19;
  }
  v12 = *(_DWORD *)(a1 + 96);
  v13 = *(_DWORD *)(a1 + 68);
  v14 = v22 - *(_QWORD *)(16 * v12 + v13);
  if ( v14 < 0 )
    v14 = 0LL;
  if ( v12 > 0 )
  {
    v15 = (_QWORD *)(v13 + 8);
    do
    {
      v16 = *v15 + v14;
      v15 += 2;
      --v12;
      v14 = v16;
    }
    while ( v12 );
  }
  *(_QWORD *)(a1 + 80) = v14 - v18;
LABEL_40:
  sub_4D41B0(v23);
  return 0;
}

// ===== sub_4BBE90 @ 0x004BBE90..0x004BBEC7 =====
int __cdecl sub_4BBE90(_DWORD *a1)
{
  int result; // eax

  sub_4C80A0(a1 + 120);
  sub_4C7CC0(a1 + 148);
  result = 0;
  a1[26] = 0;
  a1[28] = 0;
  a1[22] = 2;
  a1[27] = 0;
  a1[29] = 0;
  return result;
}

// ===== sub_4BBED0 @ 0x004BBED0..0x004BC48F =====
int __cdecl sub_4BBED0(int a1, __int64 a2)
{
  __int64 v2; // rax
  int v4; // ecx
  _QWORD *v5; // esi
  int v6; // esi
  unsigned int v7; // edx
  __int64 v8; // rcx
  signed __int64 v9; // rdi
  int v10; // edx
  __int64 v11; // rax
  int v12; // edx
  int v13; // eax
  int v14; // eax
  int v15; // ecx
  __int64 v16; // rax
  __int64 v17; // rdi
  unsigned int *v18; // edx
  bool v19; // cf
  unsigned int v20; // esi
  int v21; // ecx
  int v22; // esi
  int v23; // edx
  __int64 v24; // rax
  __int64 v25; // [esp+10h] [ebp-84h]
  unsigned __int64 v26; // [esp+18h] [ebp-7Ch]
  unsigned int v27; // [esp+20h] [ebp-74h]
  __int64 v28; // [esp+28h] [ebp-6Ch]
  int v29; // [esp+30h] [ebp-64h]
  __int64 v30; // [esp+34h] [ebp-60h]
  __int64 v31; // [esp+3Ch] [ebp-58h]
  unsigned __int64 v32; // [esp+44h] [ebp-50h]
  __int64 v33; // [esp+4Ch] [ebp-48h]
  _QWORD v34[2]; // [esp+54h] [ebp-40h] BYREF
  _BYTE v35[16]; // [esp+64h] [ebp-30h] BYREF
  char v36[16]; // [esp+74h] [ebp-20h] BYREF
  unsigned int v37; // [esp+84h] [ebp-10h]
  int v38; // [esp+88h] [ebp-Ch]

  LODWORD(v2) = sub_4BBB50((_DWORD *)a1, -1);
  v28 = v2;
  if ( *(int *)(a1 + 88) < 2 )
    return -131;
  if ( !*(_DWORD *)(a1 + 4) )
    return -138;
  if ( a2 < 0 || a2 > v2 )
    return -131;
  v4 = *(_DWORD *)(a1 + 52) - 1;
  v29 = v4;
  if ( v4 >= 0 )
  {
    v5 = (_QWORD *)(*(_DWORD *)(a1 + 68) + 16 * v4 + 8);
    do
    {
      v2 -= *v5;
      if ( a2 >= v2 )
        break;
      --v4;
      v5 -= 2;
    }
    while ( v4 >= 0 );
    v29 = v4;
    v28 = v2;
  }
  v6 = *(_DWORD *)(a1 + 56);
  HIDWORD(v8) = *(_DWORD *)(v6 + 8 * v4 + 4);
  v25 = *(_QWORD *)(v6 + 8 * v4 + 8);
  v7 = *(_DWORD *)(v6 + 8 * v4);
  v27 = v7;
  LODWORD(v8) = *(_DWORD *)(a1 + 68) + 16 * v4;
  v32 = __PAIR64__(HIDWORD(v8), v7);
  v31 = *(_QWORD *)v8;
  v33 = *(_QWORD *)v8 + *(_QWORD *)(v8 + 8);
  v30 = a2 + *(_QWORD *)v8 - v28;
  if ( __SPAIR64__(HIDWORD(v8), v7) < v25 )
  {
LABEL_12:
    if ( (__int64)(v25 - __PAIR64__(HIDWORD(v8), v27)) >= 8500 )
    {
      LODWORD(v8) = v27;
      v9 = v8 + (__int64)((v30 - v31) * (v25 - __PAIR64__(HIDWORD(v8), v27))) / (v33 - v31) - 8500;
      if ( v9 <= __SPAIR64__(HIDWORD(v8), v27) )
        v9 = __PAIR64__(HIDWORD(v8), v27) + 1;
    }
    else
    {
      v9 = __PAIR64__(HIDWORD(v8), v27);
    }
    sub_4BB610((int *)a1, v9, SHIDWORD(v9));
    while ( 1 )
    {
      LODWORD(v26) = sub_4BB270(a1, (int)v35, v25 - *(_QWORD *)(a1 + 8));
      HIDWORD(v26) = v10;
      if ( (_DWORD)v26 == -128 && v10 == -1 )
        goto LABEL_57;
      if ( v10 < 0 )
        break;
      v11 = sub_4D4000(v35);
      if ( (HIDWORD(v11) & (unsigned int)v11) != 0xFFFFFFFF )
      {
        if ( v11 >= v30 )
        {
          LODWORD(v34[0]) = v27 + 1;
          HIDWORD(v34[0]) = __CFADD__(v27, 1) + HIDWORD(v8);
          if ( v9 <= (__int64)(__PAIR64__(HIDWORD(v8), v27) + 1) )
            goto LABEL_37;
          if ( v25 != *(_QWORD *)(a1 + 8) )
          {
            v33 = v11;
            v25 = v26;
LABEL_36:
            if ( __SPAIR64__(HIDWORD(v8), v27) < v25 )
              goto LABEL_12;
            goto LABEL_37;
          }
          v25 = v26;
          v9 -= 8500LL;
          if ( v9 <= __SPAIR64__(HIDWORD(v8), v27) )
            v9 = v34[0];
          goto LABEL_32;
        }
        LODWORD(v9) = *(_DWORD *)(a1 + 8);
        v32 = v26;
        HIDWORD(v8) = *(_DWORD *)(a1 + 12);
        v31 = v11;
        v27 = v9;
        if ( v30 - v11 > 44100 )
          goto LABEL_36;
        HIDWORD(v9) = *(_DWORD *)(a1 + 12);
      }
LABEL_33:
      if ( __SPAIR64__(HIDWORD(v8), v27) >= v25 )
        goto LABEL_37;
    }
    LODWORD(v8) = v27;
    if ( v9 <= v8 + 1 )
      goto LABEL_37;
    if ( !v9 )
      goto LABEL_57;
    v9 -= 8500LL;
    if ( v9 <= __SPAIR64__(HIDWORD(v8), v27) )
      v9 = v8 + 1;
LABEL_32:
    sub_4BB610((int *)a1, v9, SHIDWORD(v9));
    goto LABEL_33;
  }
LABEL_37:
  sub_4BBE90((_DWORD *)a1);
  sub_4BB610((int *)a1, v32, SHIDWORD(v32));
  sub_4BB270(a1, (int)v34, -1LL);
  if ( v12 < 0 )
    return -2;
  v13 = sub_4D40E0(v34);
  *(_DWORD *)(a1 + 92) = v13;
  *(_DWORD *)(a1 + 96) = v29;
  sub_4D48D0(a1 + 120, v13);
  *(_DWORD *)(a1 + 88) = 3;
  sub_4D4570(a1 + 120, v34);
  while ( 1 )
  {
    v14 = sub_4D4A40(a1 + 120, v36);
    if ( !v14 )
    {
      sub_4BBE90((_DWORD *)a1);
      sub_4BB610((int *)a1, v32, SHIDWORD(v32));
      while ( 1 )
      {
        LODWORD(v16) = sub_4BB4F0(a1, (int)v34);
        v17 = v16;
        LODWORD(v26) = v16;
        if ( v16 < 0 )
          break;
        if ( (int)((unsigned __int64)sub_4D4000(v34) >> 32) > -1 || !sub_4D3FD0(v34) )
          return sub_4BBBC0(a1, v17);
        *(_QWORD *)(a1 + 8) = v17;
      }
      goto LABEL_57;
    }
    if ( v14 >> 31 < 0 )
    {
      LODWORD(v26) = -136;
      goto LABEL_57;
    }
    v15 = v38;
    if ( (v38 & v37) != 0xFFFFFFFF )
      break;
    sub_4D48F0(a1 + 120, 0);
  }
  v18 = (unsigned int *)(*(_DWORD *)(a1 + 68) + 16 * *(_DWORD *)(a1 + 96));
  v19 = v37 < *v18;
  v20 = v18[1];
  *(_DWORD *)(a1 + 80) = v37 - *v18;
  v21 = v15 - (v19 + v20);
  *(_DWORD *)(a1 + 84) = v21;
  if ( v21 < 0 )
  {
    *(_DWORD *)(a1 + 80) = 0;
    *(_DWORD *)(a1 + 84) = 0;
  }
  v22 = *(_DWORD *)(a1 + 80);
  *(_DWORD *)(a1 + 80) = v28 + v22;
  v23 = HIDWORD(v28) + __CFADD__((_DWORD)v28, v22) + *(_DWORD *)(a1 + 84);
  *(_DWORD *)(a1 + 84) = v23;
  if ( __SPAIR64__(v23, *(_DWORD *)(a1 + 80)) <= a2 )
  {
    LODWORD(v24) = sub_4BBB50((_DWORD *)a1, -1);
    if ( a2 <= v24 )
      return 0;
  }
  LODWORD(v26) = -129;
LABEL_57:
  *(_DWORD *)(a1 + 80) = -1;
  *(_DWORD *)(a1 + 84) = -1;
  sub_4BBE90((_DWORD *)a1);
  return v26;
}
