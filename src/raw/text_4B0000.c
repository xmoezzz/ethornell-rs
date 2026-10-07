#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== __vsprintf_l @ 0x004B023E..0x004B02C2 =====
int __cdecl _vsprintf_l(char *const Buffer, const char *const Format, const _locale_t Locale, va_list ArgList)
{
  int v5; // eax
  bool v6; // sf
  int v7; // esi
  FILE File; // [esp+4h] [ebp-20h] BYREF

  memset(&File, 0, sizeof(File));
  if ( Format && Buffer )
  {
    File._base = Buffer;
    File._ptr = Buffer;
    File._cnt = 0x7FFFFFFF;
    File._flag = 66;
    v5 = _output_l(&File, Format, (struct localeinfo_struct *)Locale, (int *)ArgList);
    v6 = --File._cnt < 0;
    v7 = v5;
    if ( v6 )
      _flsbuf(0, &File);
    else
      *File._ptr = 0;
    return v7;
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
}

// ===== _vsprintf @ 0x004B02C2..0x004B02DC =====
int __cdecl vsprintf(char *const Buffer, const char *const Format, va_list ArgList)
{
  return _vsprintf_l(Buffer, Format, 0, ArgList);
}

// ===== _wcsncpy_s @ 0x004B02DC..0x004B03A9 =====
errno_t __cdecl wcsncpy_s(wchar_t *Destination, rsize_t SizeInWords, const wchar_t *Source, rsize_t MaxCount)
{
  rsize_t v4; // ebx
  rsize_t v6; // edi
  errno_t v7; // esi
  const wchar_t *v8; // ecx
  wchar_t *v9; // eax
  wchar_t v10; // ax
  wchar_t v11; // cx

  v4 = MaxCount;
  if ( MaxCount )
  {
    if ( !Destination )
    {
LABEL_7:
      v7 = 22;
      *_errno() = 22;
LABEL_8:
      _invalid_parameter_noinfo();
      return v7;
    }
  }
  else if ( !Destination )
  {
    if ( !SizeInWords )
      return 0;
    goto LABEL_7;
  }
  v6 = SizeInWords;
  if ( !SizeInWords )
    goto LABEL_7;
  if ( !MaxCount )
  {
    *Destination = 0;
    return 0;
  }
  v8 = Source;
  if ( !Source )
  {
    *Destination = 0;
    goto LABEL_7;
  }
  v9 = Destination;
  if ( MaxCount == -1 )
  {
    do
    {
      v10 = *v8;
      *(const wchar_t *)((char *)v8 + (char *)Destination - (char *)Source) = *v8;
      ++v8;
      if ( !v10 )
        break;
      --v6;
    }
    while ( v6 );
  }
  else
  {
    do
    {
      v11 = *(wchar_t *)((char *)v9 + (char *)Source - (char *)Destination);
      *v9++ = v11;
      if ( !v11 )
        break;
      if ( !--v6 )
        break;
      --v4;
    }
    while ( v4 );
    if ( !v4 )
      *v9 = 0;
  }
  if ( v6 )
    return 0;
  if ( v4 != -1 )
  {
    *Destination = 0;
    *_errno() = 34;
    v7 = 34;
    goto LABEL_8;
  }
  Destination[SizeInWords - 1] = 0;
  return 80;
}

// ===== _CPtoLCID @ 0x004B03A9..0x004B03D8 =====
int __usercall CPtoLCID@<eax>(int a1@<eax>)
{
  int v1; // eax
  int v2; // eax
  int v3; // eax

  v1 = a1 - 932;
  if ( !v1 )
    return 1041;
  v2 = v1 - 4;
  if ( !v2 )
    return 2052;
  v3 = v2 - 13;
  if ( !v3 )
    return 1042;
  if ( v3 == 1 )
    return 1028;
  return 0;
}

// ===== ?setSBCS@@YAXPAUthreadmbcinfostruct@@@Z @ 0x004B03D8..0x004B043C =====
void __usercall setSBCS(_DWORD *a1@<eax>)
{
  _BYTE *v2; // eax
  int v3; // ecx
  int v4; // edi
  _BYTE *v5; // eax
  int v6; // esi

  memset(a1 + 7, 0, 0x101u);
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  a1[5] = 0;
  a1[6] = 0;
  v2 = a1 + 7;
  v3 = &unk_4FB8F0 - (_UNKNOWN *)a1;
  v4 = 257;
  do
  {
    *v2 = v2[v3];
    ++v2;
    --v4;
  }
  while ( v4 );
  v5 = (char *)a1 + 285;
  v6 = 256;
  do
  {
    *v5 = v5[v3];
    ++v5;
    --v6;
  }
  while ( v6 );
}

// ===== ?setSBUpLow@@YAXPAUthreadmbcinfostruct@@@Z @ 0x004B043C..0x004B05CC =====
void __usercall setSBUpLow(int a1@<esi>)
{
  unsigned int i; // eax
  BYTE v2; // al
  BYTE *v3; // ebx
  unsigned int v4; // ecx
  unsigned int v5; // eax
  int v6; // eax
  WORD v7; // cx
  char v8; // cl
  unsigned int v9; // ecx
  _BYTE *v10; // eax
  char v11; // dl
  int v12; // [esp+8h] [ebp-51Ch]
  _cpinfo CPInfo; // [esp+Ch] [ebp-518h] BYREF
  WORD CharType[256]; // [esp+20h] [ebp-504h] BYREF
  WCHAR v15[128]; // [esp+220h] [ebp-304h] BYREF
  WCHAR DestStr[128]; // [esp+320h] [ebp-204h] BYREF
  CHAR MultiByteStr[256]; // [esp+420h] [ebp-104h] BYREF

  if ( GetCPInfo(*(_DWORD *)(a1 + 4), &CPInfo) )
  {
    for ( i = 0; i < 0x100; ++i )
      MultiByteStr[i] = i;
    v2 = CPInfo.LeadByte[0];
    MultiByteStr[0] = 32;
    if ( CPInfo.LeadByte[0] )
    {
      v3 = &CPInfo.LeadByte[1];
      do
      {
        v4 = v2;
        v5 = *v3;
        if ( v4 <= v5 )
          memset(&MultiByteStr[v4], 32, v5 - v4 + 1);
        v2 = v3[1];
        v3 += 2;
      }
      while ( v2 );
    }
    __crtGetStringTypeA(0, 1u, MultiByteStr, 256, CharType, *(_DWORD *)(a1 + 4), *(_DWORD *)(a1 + 12), 0);
    __crtLCMapStringA(0, *(_DWORD *)(a1 + 12), 0x100u, MultiByteStr, 256, DestStr, 256, *(_DWORD *)(a1 + 4), 0);
    __crtLCMapStringA(0, *(_DWORD *)(a1 + 12), 0x200u, MultiByteStr, 256, v15, 256, *(_DWORD *)(a1 + 4), 0);
    v6 = 0;
    while ( 1 )
    {
      v7 = CharType[v6];
      if ( (v7 & 1) != 0 )
      {
        *(_BYTE *)(a1 + v6 + 29) |= 0x10u;
        v8 = *((_BYTE *)DestStr + v6);
      }
      else
      {
        if ( (v7 & 2) == 0 )
        {
          *(_BYTE *)(a1 + v6 + 285) = 0;
          goto LABEL_16;
        }
        *(_BYTE *)(a1 + v6 + 29) |= 0x20u;
        v8 = *((_BYTE *)v15 + v6);
      }
      *(_BYTE *)(a1 + v6 + 285) = v8;
LABEL_16:
      if ( (unsigned int)++v6 >= 0x100 )
        return;
    }
  }
  v9 = 0;
  v12 = -97 - (a1 + 285);
  do
  {
    v10 = (_BYTE *)(a1 + v9 + 285);
    if ( (unsigned int)&v10[v12 + 32] <= 0x19 )
    {
      *(_BYTE *)(a1 + v9 + 29) |= 0x10u;
      v11 = v9 + 32;
LABEL_23:
      *v10 = v11;
      goto LABEL_25;
    }
    if ( (unsigned int)&v10[v12] <= 0x19 )
    {
      *(_BYTE *)(a1 + v9 + 29) |= 0x20u;
      v11 = v9 - 32;
      goto LABEL_23;
    }
    *v10 = 0;
LABEL_25:
    ++v9;
  }
  while ( v9 < 0x100 );
}

// ===== ___updatetmbcinfo @ 0x004B05CC..0x004B0670 =====
volatile LONG *__updatetmbcinfo()
{
  DWORD *v0; // edi
  volatile LONG *v1; // esi

  v0 = _getptd();
  if ( (dword_4FBE10 & v0[28]) != 0 && v0[27] )
  {
    v1 = (volatile LONG *)v0[26];
  }
  else
  {
    _lock(13);
    v1 = (volatile LONG *)v0[26];
    if ( v1 != lpAddend )
    {
      if ( v1 && !InterlockedDecrement(v1) && v1 != (volatile LONG *)&unk_4FB8F0 )
        free((void *)v1);
      v0[26] = (DWORD)lpAddend;
      v1 = lpAddend;
      InterlockedIncrement(lpAddend);
    }
    _unlock(13);
  }
  if ( !v1 )
    _amsg_exit(32);
  return v1;
}

// ===== ?getSystemCP@@YAHH@Z @ 0x004B0670..0x004B06EC =====
UINT __usercall getSystemCP@<eax>(int a1@<esi>)
{
  UINT result; // eax
  int v2; // [esp+4h] [ebp-10h] BYREF
  int v3; // [esp+Ch] [ebp-8h]
  char v4; // [esp+10h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v2, 0);
  dword_509EF0 = 0;
  switch ( a1 )
  {
    case -2:
      dword_509EF0 = 1;
      result = GetOEMCP();
      goto LABEL_3;
    case -3:
      dword_509EF0 = 1;
      result = GetACP();
      goto LABEL_3;
    case -4:
      result = *(_DWORD *)(v2 + 4);
      dword_509EF0 = 1;
LABEL_3:
      if ( v4 )
        *(_DWORD *)(v3 + 112) &= ~2u;
      return result;
  }
  if ( v4 )
    *(_DWORD *)(v3 + 112) &= ~2u;
  return a1;
}

// ===== __setmbcp_nolock @ 0x004B06EC..0x004B08D5 =====
int __cdecl _setmbcp_nolock(int a1, _DWORD *a2)
{
  int SystemCP; // edi
  unsigned int i; // eax
  BYTE *v5; // esi
  BYTE v6; // cl
  unsigned int j; // eax
  _BYTE *v8; // esi
  unsigned int v9; // eax
  unsigned int v10; // edi
  _WORD *v11; // eax
  int v12; // ecx
  _WORD *v13; // ecx
  int v14; // edx
  _BYTE *v15; // eax
  int v16; // ecx
  int v17; // edx
  unsigned int v18; // [esp+Ch] [ebp-20h]
  int v19; // [esp+10h] [ebp-1Ch]
  _BYTE *v20; // [esp+10h] [ebp-1Ch]
  struct _cpinfo CPInfo; // [esp+14h] [ebp-18h] BYREF
  int v22; // [esp+34h] [ebp+8h]

  SystemCP = getSystemCP(a1);
  v22 = SystemCP;
  if ( SystemCP )
  {
    v19 = 0;
    for ( i = 0; i < 60; i += 12 )
    {
      if ( dword_4FBD20[i] == SystemCP )
      {
        memset(a2 + 7, 0, 0x101u);
        v18 = 0;
        v8 = (char *)&unk_4FBD30 + 48 * v19;
        v20 = v8;
        do
        {
          while ( *v8 )
          {
            LOBYTE(v9) = v8[1];
            if ( !(_BYTE)v9 )
              break;
            v10 = (unsigned __int8)*v8;
            v9 = (unsigned __int8)v9;
            while ( v10 <= v9 )
            {
              *((_BYTE *)a2 + v10 + 29) |= byte_4FBD1C[v18];
              v9 = (unsigned __int8)v8[1];
              ++v10;
            }
            SystemCP = v22;
            v8 += 2;
          }
          ++v18;
          v8 = v20 + 8;
          v20 += 8;
        }
        while ( v18 < 4 );
        a2[1] = SystemCP;
        a2[2] = 1;
        a2[3] = CPtoLCID(SystemCP);
        v11 = a2 + 4;
        v13 = (_WORD *)((char *)&unk_4FBD24 + v12);
        v14 = 6;
        do
        {
          *v11++ = *v13++;
          --v14;
        }
        while ( v14 );
LABEL_26:
        setSBUpLow((int)a2);
        return 0;
      }
      ++v19;
    }
    if ( SystemCP == 65000 || SystemCP == 65001 || !IsValidCodePage((unsigned __int16)SystemCP) )
      return -1;
    if ( GetCPInfo(SystemCP, &CPInfo) )
    {
      memset(a2 + 7, 0, 0x101u);
      a2[1] = SystemCP;
      a2[3] = 0;
      if ( CPInfo.MaxCharSize <= 1 )
      {
        a2[2] = 0;
      }
      else
      {
        if ( CPInfo.LeadByte[0] )
        {
          v5 = &CPInfo.LeadByte[1];
          do
          {
            v6 = *v5;
            if ( !*v5 )
              break;
            for ( j = *(v5 - 1); j <= v6; ++j )
              *((_BYTE *)a2 + j + 29) |= 4u;
            v5 += 2;
          }
          while ( *(v5 - 1) );
        }
        v15 = (char *)a2 + 30;
        v16 = 254;
        do
        {
          *v15++ |= 8u;
          --v16;
        }
        while ( v16 );
        a2[3] = CPtoLCID(a2[1]);
        a2[2] = v17;
      }
      a2[4] = 0;
      a2[5] = 0;
      a2[6] = 0;
      goto LABEL_26;
    }
    if ( !dword_509EF0 )
      return -1;
  }
  setSBCS(a2);
  return 0;
}

// ===== __setmbcp @ 0x004B08D5..0x004B0A3F =====
int __cdecl _setmbcp(int CodePage)
{
  DWORD *v1; // edi
  DWORD v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // ebx
  int v5; // eax
  int i; // eax
  int j; // eax
  int k; // eax
  int v10; // [esp+14h] [ebp-20h]
  UINT CodePagea; // [esp+3Ch] [ebp+8h]

  v10 = -1;
  v1 = _getptd();
  __updatetmbcinfo();
  v2 = v1[26];
  CodePagea = getSystemCP(CodePage);
  if ( CodePagea == *(_DWORD *)(v2 + 4) )
    return 0;
  v3 = _malloc_crt(0x220u);
  v4 = v3;
  if ( v3 )
  {
    qmemcpy(v3, (const void *)v1[26], 0x220u);
    *v3 = 0;
    v5 = _setmbcp_nolock(CodePagea, v3);
    v10 = v5;
    if ( v5 )
    {
      if ( v5 == -1 )
      {
        if ( v4 != (_DWORD *)&unk_4FB8F0 )
          free(v4);
        *_errno() = 22;
      }
    }
    else
    {
      if ( !InterlockedDecrement((volatile LONG *)v1[26]) && (_UNKNOWN *)v1[26] != &unk_4FB8F0 )
        free((void *)v1[26]);
      v1[26] = (DWORD)v4;
      InterlockedIncrement(v4);
      if ( (v1[28] & 2) == 0 && (dword_4FBE10 & 1) == 0 )
      {
        _lock(13);
        dword_509F00 = v4[1];
        dword_509F04 = v4[2];
        dword_509F08 = v4[3];
        for ( i = 0; i < 5; ++i )
          word_509EF4[i] = *((_WORD *)v4 + i + 8);
        for ( j = 0; j < 257; ++j )
          byte_4FBB10[j] = *((_BYTE *)v4 + j + 28);
        for ( k = 0; k < 256; ++k )
          byte_4FBC18[k] = *((_BYTE *)v4 + k + 285);
        if ( !InterlockedDecrement(lpAddend) && lpAddend != (volatile LONG *)&unk_4FB8F0 )
          free((void *)lpAddend);
        lpAddend = v4;
        InterlockedIncrement(v4);
        _unlock(13);
      }
    }
  }
  return v10;
}

// ===== ___initmbctable @ 0x004B0A6F..0x004B0A8D =====
int __initmbctable()
{
  if ( !dword_567C10 )
  {
    _setmbcp(-3);
    dword_567C10 = 1;
  }
  return 0;
}

// ===== ___addlocaleref @ 0x004B0A8D..0x004B0B1C =====
LONG __cdecl __addlocaleref(volatile LONG *lpAddend)
{
  volatile LONG **v2; // ebx
  int lpAddenda; // [esp+14h] [ebp+8h]

  InterlockedIncrement(lpAddend);
  if ( *((_DWORD *)lpAddend + 44) )
    InterlockedIncrement(*((volatile LONG **)lpAddend + 44));
  if ( *((_DWORD *)lpAddend + 46) )
    InterlockedIncrement(*((volatile LONG **)lpAddend + 46));
  if ( *((_DWORD *)lpAddend + 45) )
    InterlockedIncrement(*((volatile LONG **)lpAddend + 45));
  if ( *((_DWORD *)lpAddend + 48) )
    InterlockedIncrement(*((volatile LONG **)lpAddend + 48));
  v2 = (volatile LONG **)(lpAddend + 20);
  lpAddenda = 6;
  do
  {
    if ( *(v2 - 2) != (volatile LONG *)&unk_4FBE14 && *v2 )
      InterlockedIncrement(*v2);
    if ( *(v2 - 1) && v2[1] )
      InterlockedIncrement(v2[1]);
    v2 += 4;
    --lpAddenda;
  }
  while ( lpAddenda );
  return InterlockedIncrement((volatile LONG *)(*((_DWORD *)lpAddend + 53) + 180));
}

// ===== ___removelocaleref @ 0x004B0B1C..0x004B0BB5 =====
volatile LONG *__cdecl __removelocaleref(volatile LONG *lpAddend)
{
  volatile LONG **v2; // ebx
  int lpAddenda; // [esp+Ch] [ebp+8h]

  if ( lpAddend )
  {
    InterlockedDecrement(lpAddend);
    if ( *((_DWORD *)lpAddend + 44) )
      InterlockedDecrement(*((volatile LONG **)lpAddend + 44));
    if ( *((_DWORD *)lpAddend + 46) )
      InterlockedDecrement(*((volatile LONG **)lpAddend + 46));
    if ( *((_DWORD *)lpAddend + 45) )
      InterlockedDecrement(*((volatile LONG **)lpAddend + 45));
    if ( *((_DWORD *)lpAddend + 48) )
      InterlockedDecrement(*((volatile LONG **)lpAddend + 48));
    v2 = (volatile LONG **)(lpAddend + 20);
    lpAddenda = 6;
    do
    {
      if ( *(v2 - 2) != (volatile LONG *)&unk_4FBE14 && *v2 )
        InterlockedDecrement(*v2);
      if ( *(v2 - 1) && v2[1] )
        InterlockedDecrement(v2[1]);
      v2 += 4;
      --lpAddenda;
    }
    while ( lpAddenda );
    InterlockedDecrement((volatile LONG *)(*((_DWORD *)lpAddend + 53) + 180));
  }
  return lpAddend;
}

// ===== ___freetlocinfo @ 0x004B0BB5..0x004B0D00 =====
void __cdecl __freetlocinfo(char *Block)
{
  _UNKNOWN **v2; // eax
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  _DWORD *v6; // eax
  int v7; // eax
  void **v8; // edi
  _DWORD *v9; // eax
  int Blocka; // [esp+14h] [ebp+8h]

  v2 = (_UNKNOWN **)*((_DWORD *)Block + 47);
  if ( v2 )
  {
    if ( v2 != &off_4FC5F8 )
    {
      v3 = (_DWORD *)*((_DWORD *)Block + 44);
      if ( v3 )
      {
        if ( !*v3 )
        {
          v4 = (_DWORD *)*((_DWORD *)Block + 46);
          if ( v4 && !*v4 )
          {
            free(*((void **)Block + 46));
            __free_lconv_mon(*((_DWORD *)Block + 47));
          }
          v5 = (_DWORD *)*((_DWORD *)Block + 45);
          if ( v5 && !*v5 )
          {
            free(*((void **)Block + 45));
            __free_lconv_num(*((_DWORD *)Block + 47));
          }
          free(*((void **)Block + 44));
          free(*((void **)Block + 47));
        }
      }
    }
  }
  v6 = (_DWORD *)*((_DWORD *)Block + 48);
  if ( v6 && !*v6 )
  {
    free((void *)(*((_DWORD *)Block + 49) - 254));
    free((void *)(*((_DWORD *)Block + 51) - 128));
    free((void *)(*((_DWORD *)Block + 52) - 128));
    free(*((void **)Block + 48));
  }
  v7 = *((_DWORD *)Block + 53);
  if ( (_UNKNOWN **)v7 != &off_4FBE18 && !*(_DWORD *)(v7 + 180) )
  {
    __free_lc_time(*((_DWORD *)Block + 53));
    free(*((void **)Block + 53));
  }
  v8 = (void **)(Block + 80);
  Blocka = 6;
  do
  {
    if ( *(v8 - 2) != &unk_4FBE14 && *v8 && !*(_DWORD *)*v8 )
      free(*v8);
    if ( *(v8 - 1) )
    {
      v9 = v8[1];
      if ( v9 )
      {
        if ( !*v9 )
          free(v8[1]);
      }
    }
    v8 += 4;
    --Blocka;
  }
  while ( Blocka );
  free(Block);
}

// ===== __updatetlocinfoEx_nolock @ 0x004B0D00..0x004B0D4D =====
volatile LONG *__cdecl _updatetlocinfoEx_nolock(volatile LONG **a1, volatile LONG *lpAddend)
{
  volatile LONG *v2; // esi

  if ( !lpAddend || !a1 )
    return 0;
  v2 = *a1;
  if ( *a1 != lpAddend )
  {
    *a1 = lpAddend;
    __addlocaleref(lpAddend);
    if ( v2 )
    {
      __removelocaleref(v2);
      if ( !*v2 && v2 != (volatile LONG *)&unk_4FBF80 )
        __freetlocinfo((char *)v2);
    }
  }
  return lpAddend;
}

// ===== ___updatetlocinfo @ 0x004B0D4D..0x004B0DC6 =====
DWORD __updatetlocinfo()
{
  DWORD *v0; // esi
  DWORD v1; // esi
  volatile LONG *v3; // [esp+10h] [ebp-1Ch]

  v0 = _getptd();
  if ( (dword_4FBE10 & v0[28]) != 0 && v0[27] )
  {
    v1 = _getptd()[27];
  }
  else
  {
    _lock(12);
    v3 = _updatetlocinfoEx_nolock((volatile LONG **)v0 + 27, off_4FC058);
    _unlock(12);
    v1 = (DWORD)v3;
  }
  if ( !v1 )
    _amsg_exit(32);
  return v1;
}

// ===== __tolower_l @ 0x004B0DC6..0x004B0EDB =====
int __cdecl _tolower_l(int C, _locale_t Locale)
{
  struct __crt_locale_data *locinfo; // ecx
  int v4; // eax
  int result; // eax
  int v6; // ecx
  int v7; // eax
  bool v8; // zf
  __crt_locale_pointers v9; // [esp+8h] [ebp-18h] BYREF
  int v10; // [esp+10h] [ebp-10h]
  char v11; // [esp+14h] [ebp-Ch]
  WCHAR DestStr; // [esp+18h] [ebp-8h] BYREF
  CHAR MultiByteStr; // [esp+1Ch] [ebp-4h] BYREF
  char v14; // [esp+1Dh] [ebp-3h]
  char v15; // [esp+1Eh] [ebp-2h]
  int Ca; // [esp+28h] [ebp+8h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v9, (struct localeinfo_struct *)Locale);
  if ( (unsigned int)C >= 0x100 )
  {
    if ( *((int *)v9.locinfo + 43) > 1 && (Ca = C >> 8, _isleadbyte_l(BYTE1(C), &v9)) )
    {
      MultiByteStr = Ca;
      v14 = C;
      v15 = 0;
      v6 = 2;
    }
    else
    {
      *_errno() = 42;
      MultiByteStr = C;
      v14 = 0;
      v6 = 1;
    }
    v7 = __crtLCMapStringA(
           (struct localeinfo_struct *)&v9,
           *((_DWORD *)v9.locinfo + 5),
           0x100u,
           &MultiByteStr,
           v6,
           &DestStr,
           3,
           *((_DWORD *)v9.locinfo + 1),
           1);
    if ( v7 )
    {
      v8 = v7 == 1;
      result = (unsigned __int8)DestStr;
      if ( !v8 )
        result = HIBYTE(DestStr) | ((unsigned __int8)DestStr << 8);
      goto LABEL_17;
    }
  }
  else
  {
    locinfo = v9.locinfo;
    if ( *((int *)v9.locinfo + 43) <= 1 )
    {
      v4 = *(_WORD *)(*((_DWORD *)v9.locinfo + 50) + 2 * C) & 1;
    }
    else
    {
      v4 = _isctype_l(C, 1, &v9);
      locinfo = v9.locinfo;
    }
    if ( v4 )
    {
      result = *(unsigned __int8 *)(*((_DWORD *)locinfo + 51) + C);
LABEL_17:
      if ( v11 )
        *(_DWORD *)(v10 + 112) &= ~2u;
      return result;
    }
  }
  if ( v11 )
    *(_DWORD *)(v10 + 112) &= ~2u;
  return C;
}

// ===== _tolower @ 0x004B0EDB..0x004B0F07 =====
int __cdecl tolower(int C)
{
  int result; // eax

  if ( dword_509F0C )
    return _tolower_l(C, 0);
  result = C;
  if ( (unsigned int)(C - 65) <= 0x19 )
    return C + 32;
  return result;
}

// ===== _strlen @ 0x004B0F10..0x004B0F9B =====
size_t __cdecl strlen(const char *Str)
{
  const char *v1; // ecx
  int v3; // eax
  int v4; // eax

  v1 = Str;
  if ( ((unsigned __int8)Str & 3) == 0 )
    goto main_loop_0;
  do
  {
    if ( !*v1++ )
      return v1 - 1 - Str;
  }
  while ( ((unsigned __int8)v1 & 3) != 0 );
  while ( 1 )
  {
    do
    {
main_loop_0:
      v3 = (*(_DWORD *)v1 + 2130640639) ^ ~*(_DWORD *)v1;
      v1 += 4;
    }
    while ( (v3 & 0x81010100) == 0 );
    v4 = *((_DWORD *)v1 - 1);
    if ( !(_BYTE)v4 )
      break;
    if ( !BYTE1(v4) )
      return v1 - 3 - Str;
    if ( (v4 & 0xFF0000) == 0 )
      return v1 - 2 - Str;
    if ( (v4 & 0xFF000000) == 0 )
      return v1 - 1 - Str;
  }
  return v1 - 4 - Str;
}

// ===== ?strtoxl@@YAKPAUlocaleinfo_struct@@PBDPAPBDHH@Z @ 0x004B0F9B..0x004B11C6 =====
unsigned int __cdecl strtoxl(struct localeinfo_struct *a1, const char *a2, const char **a3, unsigned int a4, int a5)
{
  struct __crt_locale_data *locinfo; // ecx
  unsigned __int8 v7; // bl
  const char *i; // edi
  int v9; // eax
  unsigned int v10; // eax
  int v11; // esi
  unsigned int v12; // ecx
  int v13; // ecx
  const char *v14; // edi
  __crt_locale_pointers Locale; // [esp+4h] [ebp-1Ch] BYREF
  int v16; // [esp+Ch] [ebp-14h]
  char v17; // [esp+10h] [ebp-10h]
  unsigned int v18; // [esp+18h] [ebp-8h]
  unsigned int v19; // [esp+1Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Locale, a1);
  if ( a3 )
    *a3 = a2;
  if ( !a2 || a4 && ((int)a4 < 2 || (int)a4 > 36) )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    if ( v17 )
      *(_DWORD *)(v16 + 112) &= ~2u;
    return 0;
  }
  v19 = 0;
  locinfo = Locale.locinfo;
  v7 = *a2;
  for ( i = a2 + 1; ; ++i )
  {
    if ( *((int *)locinfo + 43) <= 1 )
    {
      v9 = *(_WORD *)(*((_DWORD *)locinfo + 50) + 2 * v7) & 8;
    }
    else
    {
      v9 = _isctype_l(v7, 8, &Locale);
      locinfo = Locale.locinfo;
    }
    if ( !v9 )
      break;
    v7 = *i;
  }
  if ( v7 == 45 )
  {
    a5 |= 2u;
  }
  else if ( v7 != 43 )
  {
    goto LABEL_20;
  }
  v7 = *i++;
LABEL_20:
  if ( a4 )
  {
    if ( a4 != 16 || v7 != 48 )
      goto LABEL_32;
  }
  else
  {
    if ( v7 != 48 )
    {
      a4 = 10;
      goto LABEL_32;
    }
    if ( *i != 120 && *i != 88 )
    {
      a4 = 8;
      goto LABEL_32;
    }
    a4 = 16;
  }
  if ( *i == 120 || *i == 88 )
  {
    v7 = i[1];
    i += 2;
  }
LABEL_32:
  v10 = 0xFFFFFFFF / a4;
  v11 = *((_DWORD *)locinfo + 50);
  v18 = 0xFFFFFFFF % a4;
  while ( 1 )
  {
    if ( (*(_WORD *)(v11 + 2 * v7) & 4) != 0 )
    {
      v12 = (char)v7 - 48;
    }
    else
    {
      if ( (*(_WORD *)(v11 + 2 * v7) & 0x103) == 0 )
        break;
      v13 = (char)v7;
      if ( (unsigned __int8)(v7 - 97) <= 0x19u )
        v13 = (char)v7 - 32;
      v12 = v13 - 55;
    }
    if ( v12 >= a4 )
      break;
    a5 |= 8u;
    if ( v19 < v10 || v19 == v10 && v12 <= v18 )
    {
      v19 = v12 + a4 * v19;
    }
    else
    {
      a5 |= 4u;
      if ( !a3 )
        break;
    }
    v7 = *i++;
  }
  v14 = i - 1;
  if ( (a5 & 8) != 0 )
  {
    if ( (a5 & 4) != 0 || (a5 & 1) == 0 && ((a5 & 2) != 0 && v19 > 0x80000000 || (a5 & 2) == 0 && v19 > 0x7FFFFFFF) )
    {
      *_errno() = 34;
      if ( (a5 & 1) != 0 )
        v19 = -1;
      else
        v19 = ((a5 & 2) != 0) + 0x7FFFFFFF;
    }
  }
  else
  {
    if ( a3 )
      v14 = a2;
    v19 = 0;
  }
  if ( a3 )
    *a3 = v14;
  if ( (a5 & 2) != 0 )
    v19 = -v19;
  if ( v17 )
    *(_DWORD *)(v16 + 112) &= ~2u;
  return v19;
}

// ===== _strtol @ 0x004B11C6..0x004B11F1 =====
int __cdecl strtol(const char *String, char **EndPtr, int Radix)
{
  if ( dword_509F0C )
    return strtoxl(0, String, (const char **)EndPtr, Radix, 0);
  else
    return strtoxl((struct localeinfo_struct *)&off_4FC05C, String, (const char **)EndPtr, Radix, 0);
}

// ===== _abort @ 0x004B11F1..0x004B1223 =====
void __cdecl __noreturn abort()
{
  int v0; // edi
  int v1; // esi

  if ( sub_4B5C56() )
    raise(22);
  if ( (dword_4FC070 & 2) != 0 )
    _call_reportfault(v0, v1, 3, 1073741845, 1);
  _exit(3);
}

// ===== __set_abort_behavior @ 0x004B1224..0x004B1245 =====
unsigned int __cdecl _set_abort_behavior(unsigned int Flags, unsigned int Mask)
{
  unsigned int result; // eax

  result = dword_4FC070;
  dword_4FC070 = Mask & Flags | dword_4FC070 & ~Mask;
  return result;
}

// ===== __GET_RTERRMSG @ 0x004B1245..0x004B126B =====
wchar_t *__cdecl _GET_RTERRMSG(int a1)
{
  int v1; // eax

  v1 = 0;
  while ( a1 != dword_4DD298[2 * v1] )
  {
    if ( (unsigned int)++v1 >= 0x16 )
      return 0;
  }
  return (&off_4DD29C)[2 * v1];
}

// ===== __NMSG_WRITE @ 0x004B126B..0x004B141A =====
_BYTE *__cdecl _NMSG_WRITE(int a1)
{
  _BYTE *result; // eax
  _BYTE *v2; // edi
  size_t v3; // eax
  void *v4; // esi
  unsigned int i; // eax
  size_t v6; // eax
  DWORD NumberOfBytesWritten; // [esp+Ch] [ebp-1FCh] BYREF
  char Buffer[500]; // [esp+10h] [ebp-1F8h] BYREF

  result = _GET_RTERRMSG(a1);
  v2 = result;
  NumberOfBytesWritten = (DWORD)result;
  if ( result )
  {
    if ( _set_error_mode(3) == 1 || (result = (_BYTE *)_set_error_mode(3)) == 0 && dword_4FB730 == 1 )
    {
      result = GetStdHandle(0xFFFFFFF4);
      v4 = result;
      if ( result && result != (_BYTE *)-1 )
      {
        for ( i = 0; i < 0x1F4; ++i )
        {
          Buffer[i] = v2[2 * i];
          if ( !*(_WORD *)&v2[2 * i] )
            break;
        }
        Buffer[499] = 0;
        v6 = strlen(Buffer);
        return (_BYTE *)WriteFile(v4, Buffer, v6, &NumberOfBytesWritten, 0);
      }
    }
    else if ( a1 != 252 )
    {
      if ( wcscpy_s(&word_509F10, 0x314u, L"Runtime Error!\n\nProgram: ") )
        _invoke_watson(0, 0, 0, 0, 0);
      word_50A14A = 0;
      if ( !GetModuleFileNameW(0, Filename, 0x104u) && wcscpy_s(Filename, 0x2FBu, L"<program name unknown>")
        || wcslen(Filename) + 1 > 0x3C
        && (v3 = wcslen(Filename), wcsncpy_s(&Destination + v3, 763 - (&Destination + v3 - Filename), L"...", 3u))
        || wcscat_s(&word_509F10, 0x314u, L"\n\n")
        || wcscat_s(&word_509F10, 0x314u, (const wchar_t *)NumberOfBytesWritten) )
      {
        _invoke_watson(0, 0, 0, 0, 0);
      }
      return (_BYTE *)sub_4B75D4(&word_509F10, L"Microsoft Visual C++ Runtime Library", 73744);
    }
  }
  return result;
}

// ===== __FF_MSGBANNER @ 0x004B141A..0x004B1453 =====
_BYTE *_FF_MSGBANNER()
{
  _BYTE *result; // eax

  if ( _set_error_mode(3) == 1 || (result = (_BYTE *)_set_error_mode(3)) == 0 && dword_4FB730 == 1 )
  {
    _NMSG_WRITE(252);
    return _NMSG_WRITE(255);
  }
  return result;
}

// ===== sub_4B1453 @ 0x004B1453..0x004B1462 =====
void *__cdecl sub_4B1453(void *a1)
{
  void *result; // eax

  result = a1;
  dword_50A538 = a1;
  return result;
}

// ===== _strcpy_s @ 0x004B1462..0x004B14C1 =====
errno_t __cdecl strcpy_s(char *Destination, rsize_t SizeInBytes, const char *Source)
{
  rsize_t v3; // edi
  errno_t v4; // esi
  const char *v6; // eax
  char v7; // cl

  if ( !Destination )
    goto LABEL_3;
  v3 = SizeInBytes;
  if ( !SizeInBytes )
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
    v6[Destination - Source] = *v6;
    ++v6;
    if ( !v7 )
      break;
    --v3;
  }
  while ( v3 );
  if ( !v3 )
  {
    *Destination = 0;
    *_errno() = 34;
    v4 = 34;
    goto LABEL_4;
  }
  return 0;
}

// ===== sub_4B15C4 @ 0x004B15C4..0x004B15D4 =====
int sub_4B15C4()
{
  dword_567C00 = IsProcessorFeaturePresent(0xAu);
  return 0;
}

// ===== ?__crtLCMapStringA_stat@@YAHPAUlocaleinfo_struct@@KKPBDHPADHHH@Z @ 0x004B15D4..0x004B17BB =====
int __cdecl __crtLCMapStringA_stat(
        struct localeinfo_struct *a1,
        LCID Locale,
        DWORD dwMapFlags,
        LPCCH lpMultiByteStr,
        int cbMultiByte,
        LPWSTR lpDestStr,
        int cchDest,
        UINT CodePage,
        int a9)
{
  LPCCH v9; // eax
  int v10; // ecx
  int v11; // eax
  int v12; // eax
  int v13; // edi
  unsigned int v15; // eax
  void *v16; // esp
  WCHAR *v17; // eax
  unsigned int v18; // eax
  void *v19; // esp
  WCHAR *v20; // edi
  WCHAR *v21; // eax
  int v22; // eax
  _DWORD v23[2]; // [esp+0h] [ebp-1Ch] BYREF
  int v24; // [esp+8h] [ebp-14h] BYREF
  int cchSrc; // [esp+Ch] [ebp-10h]
  LPWSTR lpWideCharStr; // [esp+10h] [ebp-Ch]
  int cchWideChar; // [esp+14h] [ebp-8h]

  if ( cbMultiByte > 0 )
  {
    v9 = lpMultiByteStr;
    v10 = cbMultiByte;
    while ( 1 )
    {
      --v10;
      if ( !*v9 )
        break;
      ++v9;
      if ( !v10 )
      {
        v10 = -1;
        break;
      }
    }
    v11 = cbMultiByte - v10 - 1;
    if ( v11 < cbMultiByte )
      v11 = cbMultiByte - v10;
    cbMultiByte = v11;
  }
  cchWideChar = 0;
  if ( !CodePage )
    CodePage = *(_DWORD *)(*(_DWORD *)a1 + 4);
  v12 = MultiByteToWideChar(CodePage, 8 * (a9 != 0) + 1, lpMultiByteStr, cbMultiByte, 0, 0);
  v13 = v12;
  cchSrc = v12;
  if ( !v12 )
    return 0;
  if ( v12 > 0 && 0xFFFFFFE0 / v12 >= 2 )
  {
    v15 = 2 * v12 + 8;
    if ( v15 > 0x400 )
    {
      v17 = (WCHAR *)malloc(2 * v13 + 8);
      if ( v17 )
      {
        *(_DWORD *)v17 = 56797;
        goto LABEL_20;
      }
    }
    else
    {
      v16 = alloca(v15);
      v17 = (WCHAR *)v23;
      if ( v23 )
      {
        v23[0] = 52428;
LABEL_20:
        v17 += 4;
      }
    }
    lpWideCharStr = v17;
    goto LABEL_23;
  }
  lpWideCharStr = 0;
LABEL_23:
  if ( !lpWideCharStr )
    return 0;
  if ( MultiByteToWideChar(CodePage, 1u, lpMultiByteStr, cbMultiByte, lpWideCharStr, v13) )
  {
    cchWideChar = LCMapStringW(Locale, dwMapFlags, lpWideCharStr, v13, 0, 0);
    if ( cchWideChar )
    {
      if ( (dwMapFlags & 0x400) != 0 )
      {
        if ( cchDest )
        {
          if ( cchWideChar <= cchDest )
            LCMapStringW(Locale, dwMapFlags, lpWideCharStr, v13, lpDestStr, cchDest);
        }
        goto LABEL_46;
      }
      if ( cchWideChar <= 0 || 0xFFFFFFE0 / cchWideChar < 2 )
      {
        v20 = 0;
LABEL_39:
        if ( v20 )
        {
          if ( LCMapStringW(Locale, dwMapFlags, lpWideCharStr, cchSrc, v20, cchWideChar) )
          {
            if ( cchDest )
              v22 = WideCharToMultiByte(CodePage, 0, v20, cchWideChar, (LPSTR)lpDestStr, cchDest, 0, 0);
            else
              v22 = WideCharToMultiByte(CodePage, 0, v20, cchWideChar, 0, 0, 0, 0);
            cchWideChar = v22;
          }
          _freea(v20);
        }
        goto LABEL_46;
      }
      v18 = 2 * cchWideChar + 8;
      if ( v18 > 0x400 )
      {
        v21 = (WCHAR *)malloc(2 * cchWideChar + 8);
        if ( v21 )
        {
          *(_DWORD *)v21 = 56797;
          v21 += 4;
        }
        v20 = v21;
        goto LABEL_39;
      }
      v19 = alloca(v18);
      if ( v23 )
      {
        v23[0] = 52428;
        v20 = (WCHAR *)&v24;
        goto LABEL_39;
      }
    }
  }
LABEL_46:
  _freea(lpWideCharStr);
  return cchWideChar;
}

// ===== ___crtLCMapStringA @ 0x004B17BB..0x004B1801 =====
int __cdecl __crtLCMapStringA(
        struct localeinfo_struct *a1,
        LCID Locale,
        DWORD dwMapFlags,
        LPCCH lpMultiByteStr,
        int cbMultiByte,
        LPWSTR lpDestStr,
        int cchDest,
        UINT CodePage,
        int a9)
{
  int result; // eax
  _BYTE v10[8]; // [esp+0h] [ebp-10h] BYREF
  int v11; // [esp+8h] [ebp-8h]
  char v12; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v10, a1);
  result = __crtLCMapStringA_stat(
             (struct localeinfo_struct *)v10,
             Locale,
             dwMapFlags,
             lpMultiByteStr,
             cbMultiByte,
             lpDestStr,
             cchDest,
             CodePage,
             a9);
  if ( v12 )
    *(_DWORD *)(v11 + 112) &= ~2u;
  return result;
}

// ===== _strnlen @ 0x004B1801..0x004B181E =====
size_t __cdecl strnlen(const char *String, size_t MaxCount)
{
  size_t result; // eax

  for ( result = 0; result < MaxCount; ++String )
  {
    if ( !*String )
      break;
    ++result;
  }
  return result;
}

// ===== ?x_ismbbtype_l@@YAHPAUlocaleinfo_struct@@IHH@Z @ 0x004B181E..0x004B1871 =====
int __cdecl x_ismbbtype_l(struct localeinfo_struct *a1, unsigned __int8 a2, int a3, unsigned __int8 a4)
{
  int result; // eax
  _DWORD v5[2]; // [esp+0h] [ebp-10h] BYREF
  int v6; // [esp+8h] [ebp-8h]
  char v7; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v5, a1);
  if ( (a4 & *(_BYTE *)(v5[1] + a2 + 29)) != 0
    || (!a3 ? (result = 0) : (result = (unsigned __int16)(a3 & *(_WORD *)(*(_DWORD *)(v5[0] + 200) + 2 * a2))), result) )
  {
    result = 1;
  }
  if ( v7 )
    *(_DWORD *)(v6 + 112) &= ~2u;
  return result;
}

// ===== __ismbblead_l @ 0x004B1871..0x004B188A =====
int __cdecl _ismbblead_l(unsigned int Ch, _locale_t Locale)
{
  return x_ismbbtype_l((struct localeinfo_struct *)Locale, Ch, 0, 4u);
}

// ===== __ismbblead @ 0x004B188A..0x004B18A2 =====
int __cdecl _ismbblead(unsigned int Ch)
{
  return x_ismbbtype_l(0, Ch, 0, 4u);
}

// ===== __mbsnbcpy_s @ 0x004B18A2..0x004B18BF =====
errno_t __cdecl _mbsnbcpy_s(unsigned __int8 *Dst, size_t SizeInBytes, const unsigned __int8 *Src, size_t MaxCount)
{
  return _mbsnbcpy_s_l(Dst, SizeInBytes, Src, MaxCount, 0);
}

// ===== ?__CxxUnhandledExceptionFilter@@YGJPAU_EXCEPTION_POINTERS@@@Z @ 0x004B18BF..0x004B1901 =====
LONG __stdcall __CxxUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
  PEXCEPTION_RECORD ExceptionRecord; // eax
  ULONG_PTR v2; // eax

  ExceptionRecord = ExceptionInfo->ExceptionRecord;
  if ( ExceptionInfo->ExceptionRecord->ExceptionCode == -529697949 && ExceptionRecord->NumberParameters == 3 )
  {
    v2 = ExceptionRecord->ExceptionInformation[0];
    if ( v2 == 429065504 || v2 == 429065505 || v2 == 429065506 || v2 == 26820608 )
      terminate();
  }
  return 0;
}

// ===== sub_4B1901 @ 0x004B1901..0x004B190F =====
int sub_4B1901()
{
  SetUnhandledExceptionFilter(__CxxUnhandledExceptionFilter);
  return 0;
}

// ===== __wincmdln @ 0x004B190F..0x004B196E =====
const CHAR *_wincmdln()
{
  BOOL v0; // edi
  const CHAR *v1; // esi
  unsigned __int8 v2; // al

  v0 = 0;
  if ( !dword_567C10 )
    __initmbctable();
  v1 = (const CHAR *)dword_567C20;
  if ( !dword_567C20 )
    v1 = MultiByteStr;
  while ( 1 )
  {
    v2 = *v1;
    if ( *v1 <= 0x20u )
    {
      if ( !v2 )
        return v1;
      if ( !v0 )
        break;
    }
    if ( v2 == 34 )
      v0 = !v0;
    if ( _ismbblead(v2) )
      ++v1;
    ++v1;
  }
  while ( *v1 && *v1 <= 0x20u )
    ++v1;
  return v1;
}

// ===== __setenvp @ 0x004B196E..0x004B1A49 =====
int _setenvp()
{
  char *v0; // esi
  int v1; // edi
  char **v3; // edi
  char *i; // esi
  size_t v5; // eax
  rsize_t v6; // ebx
  char *v7; // eax

  if ( !dword_567C10 )
    __initmbctable();
  v0 = dword_509B60;
  v1 = 0;
  if ( !dword_509B60 )
    return -1;
  while ( *v0 )
  {
    if ( *v0 != 61 )
      ++v1;
    v0 += strlen(v0) + 1;
  }
  v3 = (char **)_calloc_crt(v1 + 1, 4);
  dword_509EB8 = v3;
  if ( !v3 )
    return -1;
  for ( i = dword_509B60; ; i += v6 )
  {
    if ( !*i )
    {
      free(dword_509B60);
      dword_509B60 = 0;
      *v3 = 0;
      dword_567C04 = 1;
      return 0;
    }
    v5 = strlen(i);
    v6 = v5 + 1;
    if ( *i != 61 )
      break;
LABEL_15:
    ;
  }
  v7 = (char *)_calloc_crt(v5 + 1, 1);
  *v3 = v7;
  if ( v7 )
  {
    if ( strcpy_s(v7, v6, i) )
      _invoke_watson(0, 0, 0, 0, 0);
    ++v3;
    goto LABEL_15;
  }
  free(dword_509EB8);
  dword_509EB8 = 0;
  return -1;
}

// ===== _parse_cmdline @ 0x004B1A4A..0x004B1BE4 =====
char **__usercall parse_cmdline@<eax>(char *a1@<edx>, _DWORD *a2@<edi>, char **a3, char *a4, _DWORD *a5)
{
  _DWORD *v5; // ecx
  char *v7; // edx
  char **v8; // ebx
  char v9; // bl
  char *v10; // ecx
  char **v11; // eax
  int v12; // ebx
  unsigned int v13; // ecx
  char v14; // al
  char *v15; // ecx
  char *v16; // ecx
  char **result; // eax
  unsigned int v18; // [esp-4h] [ebp-10h]
  BOOL v19; // [esp+8h] [ebp-4h]
  BOOL v20; // [esp+8h] [ebp-4h]

  v5 = a5;
  *a2 = 0;
  v7 = a4;
  *a5 = 1;
  if ( a3 )
  {
    v8 = a3++;
    *v8 = a4;
  }
  v19 = 0;
  do
  {
    if ( *a1 == 34 )
    {
      v9 = 34;
      ++a1;
      v19 = !v19;
    }
    else
    {
      ++*a2;
      if ( v7 )
      {
        *v7 = *a1;
        a4 = v7 + 1;
      }
      v9 = *a1;
      v18 = (unsigned __int8)*a1++;
      if ( _ismbblead(v18) )
      {
        ++*a2;
        if ( a4 )
        {
          v10 = a4++;
          *v10 = *a1;
        }
        ++a1;
      }
      v7 = a4;
      v5 = a5;
      if ( !v9 )
      {
        --a1;
        goto LABEL_18;
      }
    }
  }
  while ( v19 || v9 != 32 && v9 != 9 );
  if ( v7 )
    *(v7 - 1) = 0;
LABEL_18:
  v20 = 0;
  while ( *a1 )
  {
    while ( *a1 == 32 || *a1 == 9 )
      ++a1;
    if ( !*a1 )
      break;
    if ( a3 )
    {
      v11 = a3++;
      *v11 = v7;
    }
    ++*v5;
    while ( 1 )
    {
      v12 = 1;
      v13 = 0;
      while ( *a1 == 92 )
      {
        ++a1;
        ++v13;
      }
      if ( *a1 == 34 )
      {
        if ( (v13 & 1) == 0 )
        {
          if ( v20 && a1[1] == 34 )
          {
            ++a1;
          }
          else
          {
            v12 = 0;
            v20 = !v20;
          }
        }
        v13 >>= 1;
      }
      if ( v13 )
      {
        do
        {
          --v13;
          if ( v7 )
            *v7++ = 92;
          ++*a2;
        }
        while ( v13 );
        a4 = v7;
      }
      v14 = *a1;
      if ( !*a1 || !v20 && (v14 == 32 || v14 == 9) )
        break;
      if ( v12 )
      {
        if ( v7 )
        {
          if ( _ismbblead(v14) )
          {
            v15 = a4++;
            *v15 = *a1++;
            ++*a2;
          }
          v16 = a4++;
          *v16 = *a1;
        }
        else if ( _ismbblead(v14) )
        {
          ++a1;
          ++*a2;
        }
        ++*a2;
        v7 = a4;
      }
      ++a1;
    }
    if ( v7 )
    {
      *v7++ = 0;
      a4 = v7;
    }
    ++*a2;
    v5 = a5;
  }
  result = a3;
  if ( a3 )
    *a3 = 0;
  ++*v5;
  return result;
}

// ===== __setargv @ 0x004B1BE4..0x004B1C9F =====
int _setargv()
{
  int v0; // edi
  size_t v1; // eax
  char **v2; // esi
  size_t v4; // [esp+Ch] [ebp-Ch] BYREF
  unsigned int v5; // [esp+10h] [ebp-8h] BYREF
  char *v6; // [esp+14h] [ebp-4h]

  if ( !dword_567C10 )
    __initmbctable();
  byte_50A644 = 0;
  GetModuleFileNameA(0, byte_50A540, 0x104u);
  dword_509EC8 = (int)byte_50A540;
  if ( !dword_567C20 || (v6 = (char *)dword_567C20, !*(_BYTE *)dword_567C20) )
    v6 = byte_50A540;
  parse_cmdline(v6, &v4, 0, 0, &v5);
  if ( v5 >= 0x3FFFFFFF )
    return -1;
  if ( v4 == -1 )
    return -1;
  v0 = v5;
  v1 = 4 * v5 + v4;
  if ( v1 < v4 )
    return -1;
  v2 = (char **)_malloc_crt(v1);
  if ( !v2 )
    return -1;
  parse_cmdline(v6, &v4, v2, (char *)&v2[v0], &v5);
  dword_509EAC = v5 - 1;
  dword_509EB0 = (int)v2;
  return 0;
}

// ===== ___crtGetEnvironmentStringsA @ 0x004B1C9F..0x004B1D36 =====
CHAR *__crtGetEnvironmentStringsA()
{
  LPWCH EnvironmentStringsW; // eax
  WCHAR *v1; // ebx
  size_t v3; // eax
  CHAR *v4; // eax
  int cchWideChar; // [esp+8h] [ebp-Ch]
  int cbMultiByte; // [esp+Ch] [ebp-8h]
  CHAR *Block; // [esp+10h] [ebp-4h]

  EnvironmentStringsW = GetEnvironmentStringsW();
  v1 = EnvironmentStringsW;
  if ( !EnvironmentStringsW )
    return 0;
  for ( ; *EnvironmentStringsW; ++EnvironmentStringsW )
  {
    do
      ++EnvironmentStringsW;
    while ( *EnvironmentStringsW );
  }
  cchWideChar = EnvironmentStringsW - v1 + 1;
  v3 = WideCharToMultiByte(0, 0, v1, cchWideChar, 0, 0, 0, 0);
  cbMultiByte = v3;
  if ( v3 && (v4 = (CHAR *)_malloc_crt(v3), (Block = v4) != 0) )
  {
    if ( !WideCharToMultiByte(0, 0, v1, cchWideChar, v4, cbMultiByte, 0, 0) )
    {
      free(Block);
      Block = 0;
    }
    FreeEnvironmentStringsW(v1);
    return Block;
  }
  else
  {
    FreeEnvironmentStringsW(v1);
    return 0;
  }
}

// ===== __ioinit @ 0x004B1D36..0x004B1F7B =====
int _ioinit()
{
  unsigned int v0; // eax
  unsigned int v2; // eax
  int v3; // ebx
  unsigned int *v4; // edi
  unsigned int v5; // eax
  unsigned int v6; // eax
  int i; // edi
  int v8; // esi
  int j; // ebx
  int v10; // esi
  DWORD v11; // eax
  HANDLE StdHandle; // eax
  HANDLE v13; // edi
  DWORD FileType; // eax
  struct _STARTUPINFOW StartupInfo; // [esp+4h] [ebp-4Ch] BYREF
  HANDLE *v16; // [esp+48h] [ebp-8h]
  LPBYTE v17; // [esp+4Ch] [ebp-4h]

  GetStartupInfoW(&StartupInfo);
  v0 = _calloc_crt(32, 64);
  if ( !v0 )
    return -1;
  dword_567B00[0] = v0;
  uNumber = 32;
  if ( v0 < v0 + 2048 )
  {
    v2 = v0 + 5;
    do
    {
      *(_DWORD *)(v2 - 5) = -1;
      *(_WORD *)(v2 - 1) = 2560;
      *(_DWORD *)(v2 + 3) = 0;
      *(_WORD *)(v2 + 31) = 2560;
      *(_BYTE *)(v2 + 33) = 10;
      *(_DWORD *)(v2 + 51) = 0;
      *(_BYTE *)(v2 + 47) = 0;
      v2 += 64;
    }
    while ( v2 - 5 < dword_567B00[0] + 2048 );
  }
  if ( StartupInfo.cbReserved2 && StartupInfo.lpReserved2 )
  {
    v3 = *(_DWORD *)StartupInfo.lpReserved2;
    v17 = StartupInfo.lpReserved2 + 4;
    v16 = (HANDLE *)&StartupInfo.lpReserved2[v3 + 4];
    if ( v3 >= 2048 )
      v3 = 2048;
    if ( (int)uNumber < v3 )
    {
      v4 = (unsigned int *)&unk_567B04;
      while ( 1 )
      {
        v5 = _calloc_crt(32, 64);
        if ( !v5 )
          break;
        uNumber += 32;
        *v4 = v5;
        if ( v5 < v5 + 2048 )
        {
          v6 = v5 + 5;
          do
          {
            *(_DWORD *)(v6 - 5) = -1;
            *(_DWORD *)(v6 + 3) = 0;
            *(_BYTE *)(v6 + 31) &= 0x80u;
            *(_DWORD *)(v6 + 51) = 0;
            *(_WORD *)(v6 - 1) = 2560;
            *(_WORD *)(v6 + 32) = 2570;
            *(_BYTE *)(v6 + 47) = 0;
            v6 += 64;
          }
          while ( v6 - 5 < *v4 + 2048 );
        }
        ++v4;
        if ( (int)uNumber >= v3 )
          goto LABEL_19;
      }
      v3 = uNumber;
    }
LABEL_19:
    for ( i = 0; i < v3; ++v17 )
    {
      if ( *v16 != (HANDLE)-1 && *v16 != (HANDLE)-2 && (*v17 & 1) != 0 && ((*v17 & 8) != 0 || GetFileType(*v16)) )
      {
        v8 = dword_567B00[i >> 5] + ((i & 0x1F) << 6);
        *(_DWORD *)v8 = *v16;
        *(_BYTE *)(v8 + 4) = *v17;
        if ( !InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)(v8 + 12), 0xFA0u) )
          return -1;
        ++*(_DWORD *)(v8 + 8);
      }
      ++v16;
      ++i;
    }
  }
  for ( j = 0; j < 3; ++j )
  {
    v10 = dword_567B00[0] + (j << 6);
    if ( *(_DWORD *)v10 == -1 || *(_DWORD *)v10 == -2 )
    {
      *(_BYTE *)(v10 + 4) = -127;
      if ( j )
        v11 = -(j != 1) - 11;
      else
        v11 = -10;
      StdHandle = GetStdHandle(v11);
      v13 = StdHandle;
      if ( StdHandle != (HANDLE)-1 && StdHandle && (FileType = GetFileType(StdHandle)) != 0 )
      {
        *(_DWORD *)v10 = v13;
        if ( (unsigned __int8)FileType == 2 )
        {
          *(_BYTE *)(v10 + 4) |= 0x40u;
        }
        else if ( (unsigned __int8)FileType == 3 )
        {
          *(_BYTE *)(v10 + 4) |= 8u;
        }
        if ( !InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)(v10 + 12), 0xFA0u) )
          return -1;
        ++*(_DWORD *)(v10 + 8);
      }
      else
      {
        *(_BYTE *)(v10 + 4) |= 0x40u;
        *(_DWORD *)v10 = -2;
      }
    }
    else
    {
      *(_BYTE *)(v10 + 4) |= 0x80u;
    }
  }
  SetHandleCount(uNumber);
  return 0;
}

// ===== sub_4B1F7B @ 0x004B1F7B..0x004B1FA1 =====
void *sub_4B1F7B()
{
  return &unk_4F3D84;
}

// ===== sub_4B1FA1 @ 0x004B1FA1..0x004B1FC7 =====
void __cdecl sub_4B1FA1()
{
  ;
}

// ===== __heap_init @ 0x004B1FC7..0x004B1FE5 =====
BOOL _heap_init()
{
  hHeap = HeapCreate(0, 0x1000u, 0);
  return hHeap != 0;
}

// ===== ___security_init_cookie @ 0x004B1FE5..0x004B2080 =====
void __cdecl __security_init_cookie()
{
  DWORD v0; // esi
  DWORD v1; // esi
  DWORD v2; // esi
  DWORD v3; // esi
  DWORD v4; // esi
  LARGE_INTEGER PerformanceCount; // [esp+8h] [ebp-10h] BYREF
  struct _FILETIME SystemTimeAsFileTime; // [esp+10h] [ebp-8h] BYREF

  SystemTimeAsFileTime.dwLowDateTime = 0;
  SystemTimeAsFileTime.dwHighDateTime = 0;
  if ( dword_4FB734 == -1153374642 || (dword_4FB734 & 0xFFFF0000) == 0 )
  {
    GetSystemTimeAsFileTime(&SystemTimeAsFileTime);
    v0 = SystemTimeAsFileTime.dwLowDateTime ^ SystemTimeAsFileTime.dwHighDateTime;
    v1 = GetCurrentProcessId() ^ v0;
    v2 = GetCurrentThreadId() ^ v1;
    v3 = GetTickCount() ^ v2;
    QueryPerformanceCounter(&PerformanceCount);
    v4 = PerformanceCount.LowPart ^ PerformanceCount.HighPart ^ v3;
    if ( v4 == -1153374642 )
    {
      v4 = -1153374641;
    }
    else if ( (v4 & 0xFFFF0000) == 0 )
    {
      v4 |= (v4 | 0x4711) << 16;
    }
    dword_4FB734 = v4;
    dword_4FB738 = ~v4;
  }
  else
  {
    dword_4FB738 = ~dword_4FB734;
  }
}

// ===== sub_4B2080 @ 0x004B2080..0x004B208B =====
void __thiscall sub_4B2080(void **this)
{
  *this = &std::bad_exception::`vftable';
  sub_4AC3AD(this);
}

// ===== sub_4B208B @ 0x004B208B..0x004B20B2 =====
void **__thiscall sub_4B208B(void **this, char a2)
{
  *this = &std::bad_exception::`vftable';
  sub_4AC3AD(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== ___TypeMatch @ 0x004B20B2..0x004B2111 =====
BOOL __cdecl __TypeMatch(int a1, int a2, _DWORD *a3)
{
  int v3; // eax
  int v4; // ecx
  BOOL result; // eax

  v3 = *(_DWORD *)(a1 + 4);
  result = 1;
  if ( v3 && *(_BYTE *)(v3 + 8) )
  {
    v4 = *(_DWORD *)(a2 + 4);
    if ( v3 != v4 )
    {
      if ( strcmp((const char *)(v3 + 8), (const char *)(v4 + 8)) )
        return 0;
    }
    if ( (*(_BYTE *)a2 & 2) != 0 && (*(_BYTE *)a1 & 8) == 0
      || (*a3 & 1) != 0 && (*(_BYTE *)a1 & 1) == 0
      || (*a3 & 2) != 0 && (*(_BYTE *)a1 & 2) == 0 )
    {
      return 0;
    }
  }
  return result;
}

// ===== ___FrameUnwindFilter @ 0x004B2111..0x004B2160 =====
int __cdecl __FrameUnwindFilter(int **a1)
{
  int v1; // eax
  DWORD *v2; // eax

  v1 = **a1;
  if ( v1 == -532462766 || v1 == -532459699 )
  {
    if ( (int)_getptd()[36] > 0 )
    {
      v2 = _getptd();
      --v2[36];
    }
  }
  else if ( v1 == -529697949 )
  {
    _getptd()[36] = 0;
    terminate();
  }
  return 0;
}

// ===== ___FrameUnwindToState @ 0x004B2160..0x004B223C =====
DWORD *__cdecl __FrameUnwindToState(int a1, int a2, int a3, int a4)
{
  int v4; // esi
  DWORD *v5; // eax
  int v6; // eax
  int v7; // ecx
  DWORD *result; // eax

  if ( *(int *)(a3 + 4) > 128 )
    v4 = *(_DWORD *)(a1 + 8);
  else
    v4 = *(char *)(a1 + 8);
  v5 = _getptd();
  ++v5[36];
  while ( v4 != a4 )
  {
    if ( v4 <= -1 || v4 >= *(_DWORD *)(a3 + 4) )
      _inconsistency();
    v6 = v4;
    v7 = *(_DWORD *)(a3 + 8);
    v4 = *(_DWORD *)(v7 + 8 * v4);
    if ( *(_DWORD *)(v7 + 8 * v6 + 4) )
    {
      *(_DWORD *)(a1 + 8) = v4;
      _CallSettingFrame(*(_DWORD *)(*(_DWORD *)(a3 + 8) + 8 * v6 + 4), a1, 259);
    }
  }
  result = _getptd();
  if ( (int)result[36] > 0 )
  {
    result = _getptd();
    --result[36];
  }
  if ( v4 != a4 )
    _inconsistency();
  *(_DWORD *)(a1 + 8) = v4;
  return result;
}

// ===== ?ExFilterRethrow@@YAHPAU_EXCEPTION_POINTERS@@@Z @ 0x004B223C..0x004B2281 =====
int __usercall ExFilterRethrow@<eax>(_DWORD **a1@<eax>)
{
  _DWORD *v1; // eax
  int v2; // ecx

  v1 = *a1;
  if ( *v1 != -529697949 )
    return 0;
  if ( v1[4] != 3 )
    return 0;
  v2 = v1[5];
  if ( v2 != 429065504 && v2 != 429065505 && v2 != 429065506 )
    return 0;
  if ( v1[7] )
    return 0;
  _getptd()[131] = 1;
  return 1;
}

// ===== ___DestructExceptionObject @ 0x004B2281..0x004B22D6 =====
void __cdecl __DestructExceptionObject(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  if ( a1 )
  {
    if ( *a1 == -529697949 )
    {
      v1 = a1[7];
      if ( v1 )
      {
        v2 = *(_DWORD *)(v1 + 4);
        if ( v2 )
          sub_4ACFDA(a1[6], v2);
      }
    }
  }
}

// ===== ___AdjustPointer @ 0x004B22D6..0x004B22FF =====
int __cdecl __AdjustPointer(int a1, _DWORD *a2)
{
  int result; // eax

  result = a1 + *a2;
  if ( (int)a2[1] >= 0 )
    result += a2[1] + *(_DWORD *)(*(_DWORD *)(a2[1] + a1) + a2[2]);
  return result;
}

// ===== ?IsInExceptionSpec@@YAEPAUEHExceptionRecord@@PBU_s_ESTypeList@@@Z @ 0x004B22FF..0x004B2375 =====
char __usercall IsInExceptionSpec@<al>(int *a1@<edi>, struct EHExceptionRecord *a2)
{
  CatchableTypeArray *pCatchableTypeArray; // eax
  int nCatchableTypes; // ebx
  int *arrayOfCatchableTypes; // esi
  int v6; // [esp+0h] [ebp-Ch]
  int v7; // [esp+4h] [ebp-8h]
  char v8; // [esp+Bh] [ebp-1h]

  if ( !a1 )
    _inconsistency();
  v8 = 0;
  v6 = 0;
  if ( *a1 > 0 )
  {
    v7 = 0;
    do
    {
      pCatchableTypeArray = a2->params.pThrowInfo->pCatchableTypeArray;
      nCatchableTypes = pCatchableTypeArray->nCatchableTypes;
      arrayOfCatchableTypes = (int *)pCatchableTypeArray->arrayOfCatchableTypes;
      while ( nCatchableTypes > 0 )
      {
        if ( __TypeMatch(v7 + a1[1], *arrayOfCatchableTypes, &a2->params.pThrowInfo->attributes) )
        {
          v8 = 1;
          break;
        }
        --nCatchableTypes;
        ++arrayOfCatchableTypes;
      }
      ++v6;
      v7 += 16;
    }
    while ( v6 < *a1 );
  }
  return v8;
}

// ===== ?CallUnexpected@@YAXPBU_s_ESTypeList@@@Z @ 0x004B2375..0x004B239D =====
void __cdecl __noreturn CallUnexpected()
{
  if ( _getptd()[37] )
    _inconsistency();
  unexpected();
}

// ===== ?CallCatchBlock@@YAPAXPAUEHExceptionRecord@@PAUEHRegistrationNode@@PAU_CONTEXT@@PBU_s_FuncInfo@@PAXHK@Z @ 0x004B23BE..0x004B255A =====
void *__cdecl CallCatchBlock(
        struct EHExceptionRecord *a1,
        struct EHRegistrationNode *a2,
        struct _CONTEXT *a3,
        const struct _s_FuncInfo *a4,
        void *a5,
        unsigned int a6)
{
  void *v6; // ecx
  void *v7; // ebx
  unsigned int magicNumber; // eax
  int v10; // [esp+10h] [ebp-3Ch] BYREF
  int v11; // [esp+18h] [ebp-34h]
  DWORD v12; // [esp+1Ch] [ebp-30h]
  DWORD v13; // [esp+20h] [ebp-2Ch]
  _DWORD *v14; // [esp+24h] [ebp-28h]
  __ehstate_t state; // [esp+28h] [ebp-24h]
  void *v16; // [esp+30h] [ebp-1Ch]
  CPPEH_RECORD ms_exc; // [esp+34h] [ebp-18h]

  v7 = v6;
  v16 = v6;
  v11 = 0;
  state = a2[-1].state;
  v14 = _CreateFrameInfo(&v10, (int)a1->params.pExceptionObject);
  v13 = _getptd()[34];
  v12 = _getptd()[35];
  _getptd()[34] = (DWORD)a1;
  _getptd()[35] = (DWORD)a3;
  ms_exc.registration.TryLevel = 1;
  v16 = _CallCatchBlock2(a2, a4, v7, (int)a5, a6);
  ms_exc.registration.TryLevel = -2;
  a2[-1].state = state;
  _FindAndUnlinkFrame((int)v14);
  _getptd()[34] = v13;
  _getptd()[35] = v12;
  if ( a1->ExceptionCode == -529697949 && a1->NumberParameters == 3 )
  {
    magicNumber = a1->params.magicNumber;
    if ( (magicNumber == 429065504 || magicNumber == 429065505 || magicNumber == 429065506)
      && !v11
      && v16
      && _IsExceptionObjectToBeDestroyed((int)a1->params.pExceptionObject) )
    {
      __DestructExceptionObject(a1);
    }
  }
  return v16;
}

// ===== ___BuildCatchObjectHelper @ 0x004B255A..0x004B26D9 =====
int __cdecl __BuildCatchObjectHelper(int a1, int *a2, int *a3, int a4)
{
  int v4; // ecx
  int v5; // ecx
  int *v6; // esi
  int v7; // eax
  int v8; // eax
  const void *v9; // eax
  int v11; // [esp-8h] [ebp-34h]
  size_t v12; // [esp-4h] [ebp-30h]
  int v13; // [esp+10h] [ebp-1Ch]

  v13 = 0;
  v4 = a3[1];
  if ( v4 )
  {
    if ( *(_BYTE *)(v4 + 8) )
    {
      v5 = a3[2];
      if ( v5 || *a3 < 0 )
      {
        v6 = a2;
        if ( *a3 >= 0 )
          v6 = (int *)((char *)a2 + v5 + 12);
        if ( (*a3 & 8) != 0 )
        {
          if ( unknown_libname_3(*(_DWORD *)(a1 + 24)) && unknown_libname_3(v6) )
          {
            v7 = *(_DWORD *)(a1 + 24);
            *v6 = v7;
            v8 = __AdjustPointer(v7, (_DWORD *)(a4 + 8));
LABEL_11:
            *v6 = v8;
            return v13;
          }
        }
        else
        {
          v11 = *(_DWORD *)(a1 + 24);
          if ( (*(_BYTE *)a4 & 1) != 0 )
          {
            if ( unknown_libname_3(v11) && unknown_libname_3(v6) )
            {
              memcpy(v6, *(const void **)(a1 + 24), *(_DWORD *)(a4 + 20));
              if ( *(_DWORD *)(a4 + 20) != 4 || !*v6 )
                return v13;
              v8 = __AdjustPointer(*v6, (_DWORD *)(a4 + 8));
              goto LABEL_11;
            }
          }
          else if ( *(_DWORD *)(a4 + 24) )
          {
            if ( unknown_libname_3(v11) && unknown_libname_3(v6) && unknown_libname_3(*(_DWORD *)(a4 + 24)) )
              return ((*(_BYTE *)a4 & 4) != 0) + 1;
          }
          else if ( unknown_libname_3(v11) && unknown_libname_3(v6) )
          {
            v12 = *(_DWORD *)(a4 + 20);
            v9 = (const void *)__AdjustPointer(*(_DWORD *)(a1 + 24), (_DWORD *)(a4 + 8));
            memcpy(v6, v9, v12);
            return v13;
          }
        }
        _inconsistency();
      }
    }
  }
  return 0;
}

// ===== ___BuildCatchObject @ 0x004B26D9..0x004B276B =====
int __cdecl __BuildCatchObject(int a1, int *a2, int *a3, int a4)
{
  int *v4; // ebx
  int v5; // eax
  int result; // eax

  if ( *a3 >= 0 )
    v4 = (int *)((char *)a2 + a3[2] + 12);
  else
    v4 = a2;
  v5 = __BuildCatchObjectHelper(a1, a2, a3, a4) - 1;
  if ( v5 )
  {
    result = v5 - 1;
    if ( !result )
    {
      __AdjustPointer(*(_DWORD *)(a1 + 24), (_DWORD *)(a4 + 8));
      return sub_4ACFDA((int)v4, *(_DWORD *)(a4 + 24));
    }
  }
  else
  {
    __AdjustPointer(*(_DWORD *)(a1 + 24), (_DWORD *)(a4 + 8));
    return sub_4ACFDA((int)v4, *(_DWORD *)(a4 + 24));
  }
  return result;
}

// ===== ?CatchIt@@YAXPAUEHExceptionRecord@@PAUEHRegistrationNode@@PAU_CONTEXT@@PAXPBU_s_FuncInfo@@PBU_s_HandlerType@@PBU_s_CatchableType@@PBU_s_TryBlockMapEntry@@H1E@Z @ 0x004B276B..0x004B27D9 =====
void __usercall CatchIt(
        int *a1@<ebx>,
        int *a2@<edi>,
        struct EHRegistrationNode *a3@<esi>,
        EXCEPTION_RECORD *ExceptionRecord,
        struct _CONTEXT *a5,
        struct _CONTEXT *a6,
        struct _s_FuncInfo *a7,
        const struct _s_FuncInfo *a8,
        struct _s_HandlerType *a9,
        struct _s_CatchableType *TargetFrame)
{
  void (__stdcall *v10)(void *, struct EHRegistrationNode *); // eax

  if ( a8 )
    __BuildCatchObject((int)ExceptionRecord, (int *)a3, a1, (int)a8);
  if ( TargetFrame )
    _UnwindNestedFrames((struct _EXCEPTION_REGISTRATION_RECORD **)a1, TargetFrame, ExceptionRecord);
  else
    _UnwindNestedFrames((struct _EXCEPTION_REGISTRATION_RECORD **)a1, a3, ExceptionRecord);
  __FrameUnwindToState((int)a3, (int)a6, (int)a7, *a2);
  a3->state = a2[1] + 1;
  v10 = (void (__stdcall *)(void *, struct EHRegistrationNode *))CallCatchBlock(
                                                                   (struct EHExceptionRecord *)ExceptionRecord,
                                                                   a3,
                                                                   a5,
                                                                   a7,
                                                                   a9,
                                                                   0x100u);
  if ( v10 )
    _JumpToContinuation(v10, a3);
}

// ===== ?FindHandlerForForeignException@@YAXPAUEHExceptionRecord@@PAUEHRegistrationNode@@PAU_CONTEXT@@PAXPBU_s_FuncInfo@@HH1@Z @ 0x004B27D9..0x004B28E0 =====
void __cdecl FindHandlerForForeignException(
        struct EHExceptionRecord *a1,
        struct EHRegistrationNode *a2,
        struct _CONTEXT *a3,
        struct _CONTEXT *a4,
        struct _s_FuncInfo *a5,
        int a6,
        struct _s_HandlerType *a7,
        struct _s_CatchableType *a8)
{
  PVOID *v8; // edi
  int v9; // esi
  TryBlockMapEntry *v10; // eax
  int *p_nCatches; // eax
  int v12; // ecx
  int v13; // edx
  unsigned int v14; // [esp+4h] [ebp-Ch] BYREF
  int *v15; // [esp+8h] [ebp-8h]
  unsigned int v16; // [esp+Ch] [ebp-4h] BYREF

  if ( a1->ExceptionCode != -2147483645 )
  {
    if ( !_getptd()[32]
      || (v8 = (PVOID *)(_getptd() + 32), *v8 == _encoded_null())
      || a1->ExceptionCode == -532459699
      || a1->ExceptionCode == -532462766
      || !_CallSETranslator(a1, a2, a3, a4, a5, (int)a7, (struct EHRegistrationNode *)a8) )
    {
      if ( !a5->nTryBlocks )
        _inconsistency();
      v9 = a6;
      v10 = _GetRangeOfTrysToCheck(a5, (int)a7, a6, &v16, &v14);
      if ( v16 < v14 )
      {
        p_nCatches = &v10->nCatches;
        v15 = p_nCatches;
        do
        {
          if ( v9 >= *(p_nCatches - 3) && v9 <= *(p_nCatches - 2) )
          {
            v12 = p_nCatches[1] + 16 * *p_nCatches;
            v13 = *(_DWORD *)(v12 - 12);
            if ( (!v13 || !*(_BYTE *)(v13 + 8)) && (*(_BYTE *)(v12 - 16) & 0x40) == 0 )
            {
              CatchIt((int *)(v12 - 16), p_nCatches - 3, a2, (EXCEPTION_RECORD *)a1, a3, a4, a5, 0, a7, a8);
              v9 = a6;
              p_nCatches = v15;
            }
          }
          ++v16;
          p_nCatches += 5;
          v15 = p_nCatches;
        }
        while ( v16 < v14 );
      }
    }
  }
}

// ===== ?FindHandler@@YAXPAUEHExceptionRecord@@PAUEHRegistrationNode@@PAU_CONTEXT@@PAXPBU_s_FuncInfo@@EH1@Z @ 0x004B28E0..0x004B2C55 =====
void __cdecl FindHandler(
        struct EHExceptionRecord *ExceptionRecord,
        struct EHRegistrationNode *a2,
        struct _CONTEXT *a3,
        struct _CONTEXT *a4,
        struct _s_FuncInfo *a5,
        unsigned __int8 a6,
        struct _s_HandlerType *a7,
        struct _s_CatchableType *TargetFrame)
{
  struct _s_FuncInfo *v8; // ebx
  __ehstate_t maxState; // eax
  __ehstate_t state; // ecx
  struct EHExceptionRecord *v11; // esi
  int arrayOfCatchableTypes; // ebx
  unsigned int magicNumber; // eax
  unsigned int v14; // eax
  int *v15; // edi
  DWORD *v16; // eax
  int v17; // esi
  int v18; // ebx
  unsigned int v19; // eax
  struct _s_FuncInfo *v20; // edi
  TryBlockMapEntry *v21; // eax
  int **p_pHandlerArray; // edi
  CatchableTypeArray *pCatchableTypeArray; // eax
  int *p_nCount; // edi
  DWORD *v25; // eax
  bool v26; // zf
  struct EHExceptionRecord *v27; // [esp-4h] [ebp-44h]
  ThrowInfo *pThrowInfo; // [esp-4h] [ebp-44h]
  _DWORD pExceptionObject[3]; // [esp+Ch] [ebp-34h] BYREF
  int *v30; // [esp+18h] [ebp-28h]
  struct _s_FuncInfo *v31; // [esp+1Ch] [ebp-24h]
  unsigned int v32; // [esp+20h] [ebp-20h] BYREF
  int **v33; // [esp+24h] [ebp-1Ch]
  int v34; // [esp+28h] [ebp-18h]
  int nCatchableTypes; // [esp+2Ch] [ebp-14h]
  unsigned int v36; // [esp+30h] [ebp-10h] BYREF
  int *v37; // [esp+34h] [ebp-Ch]
  int v38; // [esp+38h] [ebp-8h]
  char v39; // [esp+3Fh] [ebp-1h]

  v8 = a5;
  maxState = a5->maxState;
  v39 = 0;
  if ( maxState > 128 )
    state = a2->state;
  else
    state = SLOBYTE(a2->state);
  v38 = state;
  if ( state < -1 || state >= maxState )
    _inconsistency();
  v11 = ExceptionRecord;
  if ( ExceptionRecord->ExceptionCode != -529697949 )
  {
LABEL_61:
    if ( v8->nTryBlocks )
    {
      if ( a6 )
        goto LABEL_28;
      FindHandlerForForeignException(v11, a2, a3, a4, v8, v38, a7, TargetFrame);
    }
    goto LABEL_64;
  }
  arrayOfCatchableTypes = 429065504;
  if ( ExceptionRecord->NumberParameters == 3 )
  {
    magicNumber = ExceptionRecord->params.magicNumber;
    if ( (magicNumber == 429065504 || magicNumber == 429065505 || magicNumber == 429065506)
      && !ExceptionRecord->params.pThrowInfo )
    {
      if ( !_getptd()[34] )
        return;
      v11 = (struct EHExceptionRecord *)_getptd()[34];
      ExceptionRecord = v11;
      a3 = (struct _CONTEXT *)_getptd()[35];
      if ( !unknown_libname_3(v11) )
        _inconsistency();
      if ( v11->ExceptionCode == -529697949 && v11->NumberParameters == 3 )
      {
        v14 = v11->params.magicNumber;
        if ( (v14 == 429065504 || v14 == 429065505 || v14 == 429065506) && !v11->params.pThrowInfo )
          _inconsistency();
      }
      if ( _getptd()[37] )
      {
        v15 = (int *)_getptd()[37];
        v16 = _getptd();
        v27 = ExceptionRecord;
        v17 = 0;
        v16[37] = 0;
        if ( !IsInExceptionSpec(v15, v27) )
        {
          v18 = 0;
          if ( *v15 > 0 )
          {
            do
            {
              if ( type_info::operator==(
                     *(const char **)(v18 + v15[1] + 4),
                     (int)&std::bad_exception `RTTI Type Descriptor') )
              {
                __DestructExceptionObject(ExceptionRecord);
                ExceptionRecord = (struct EHExceptionRecord *)"bad exception";
                std::exception::exception((std::exception *)pExceptionObject, (char **)&ExceptionRecord);
                pExceptionObject[0] = &std::bad_exception::`vftable';
                _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI2_AVbad_exception_std__);
              }
              ++v17;
              v18 += 16;
            }
            while ( v17 < *v15 );
          }
LABEL_28:
          terminate();
        }
        v11 = ExceptionRecord;
      }
    }
  }
  if ( v11->ExceptionCode != -529697949
    || v11->NumberParameters != 3
    || (v19 = v11->params.magicNumber, v19 != 429065504) && v19 != 429065505 && v19 != 429065506 )
  {
    v8 = a5;
    goto LABEL_61;
  }
  v20 = a5;
  if ( a5->nTryBlocks )
  {
    v21 = _GetRangeOfTrysToCheck(a5, (int)a7, v38, &v36, &v32);
    if ( v36 < v32 )
    {
      p_pHandlerArray = (int **)&v21->pHandlerArray;
      v33 = (int **)&v21->pHandlerArray;
      do
      {
        v30 = (int *)(p_pHandlerArray - 4);
        if ( (int)*(p_pHandlerArray - 4) <= v38 && v38 <= (int)*(p_pHandlerArray - 3) )
        {
          v37 = *p_pHandlerArray;
          v34 = (int)*(p_pHandlerArray - 1);
          if ( v34 > 0 )
          {
            while ( 1 )
            {
              pCatchableTypeArray = v11->params.pThrowInfo->pCatchableTypeArray;
              arrayOfCatchableTypes = (int)pCatchableTypeArray->arrayOfCatchableTypes;
              nCatchableTypes = pCatchableTypeArray->nCatchableTypes;
              if ( nCatchableTypes > 0 )
                break;
LABEL_45:
              --v34;
              v37 += 4;
              if ( v34 <= 0 )
                goto $NextTryBlock$28284;
            }
            while ( 1 )
            {
              pThrowInfo = v11->params.pThrowInfo;
              v31 = *(struct _s_FuncInfo **)arrayOfCatchableTypes;
              if ( __TypeMatch((int)v37, (int)v31, pThrowInfo) )
                break;
              --nCatchableTypes;
              arrayOfCatchableTypes += 4;
              if ( nCatchableTypes <= 0 )
                goto LABEL_45;
            }
            arrayOfCatchableTypes = (int)v37;
            v39 = 1;
            CatchIt(v37, v30, a2, (EXCEPTION_RECORD *)v11, a3, a4, a5, v31, a7, TargetFrame);
            v11 = ExceptionRecord;
            p_pHandlerArray = v33;
          }
        }
$NextTryBlock$28284:
        ++v36;
        p_pHandlerArray += 5;
        v33 = p_pHandlerArray;
      }
      while ( v36 < v32 );
      v20 = a5;
    }
  }
  if ( a6 )
    __DestructExceptionObject(v11);
  if ( !v39 && (*(_DWORD *)v20 & 0x1FFFFFFFu) >= 0x19930521 )
  {
    p_nCount = &v20->pESTypeList->nCount;
    if ( p_nCount )
    {
      if ( !IsInExceptionSpec(p_nCount, v11) )
      {
        _getptd();
        _getptd();
        _getptd()[34] = (DWORD)v11;
        v25 = _getptd();
        v26 = TargetFrame == 0;
        v25[35] = (DWORD)a3;
        if ( v26 )
          _UnwindNestedFrames(
            (struct _EXCEPTION_REGISTRATION_RECORD **)arrayOfCatchableTypes,
            a2,
            (PEXCEPTION_RECORD)v11);
        else
          _UnwindNestedFrames(
            (struct _EXCEPTION_REGISTRATION_RECORD **)arrayOfCatchableTypes,
            TargetFrame,
            (PEXCEPTION_RECORD)v11);
        __FrameUnwindToState((int)a2, (int)a4, (int)a5, -1);
        CallUnexpected();
      }
    }
  }
LABEL_64:
  if ( _getptd()[37] )
    _inconsistency();
}

// ===== sub_4B2C55 @ 0x004B2C55..0x004B2C72 =====
std::exception *__thiscall sub_4B2C55(std::exception *this, struct exception *a2)
{
  std::exception::exception(this, a2);
  *(_DWORD *)this = &std::bad_exception::`vftable';
  return this;
}

// ===== ___InternalCxxFrameHandler @ 0x004B2C72..0x004B2D58 =====
int __cdecl __InternalCxxFrameHandler(
        struct EHExceptionRecord *a1,
        struct EHRegistrationNode *a2,
        struct _CONTEXT *a3,
        struct _CONTEXT *a4,
        struct _s_FuncInfo *a5,
        struct _s_HandlerType *a6,
        struct _s_CatchableType *a7,
        unsigned __int8 a8)
{
  int (__cdecl *pForwardCompat)(); // edx

  if ( _getptd()[131]
    || a1->ExceptionCode == -529697949
    || a1->ExceptionCode == -2147483610
    || (*(_DWORD *)a5 & 0x1FFFFFFFu) < 0x19930522
    || (a5->EHFlags & 1) == 0 )
  {
    if ( (a1->ExceptionFlags & 0x66) != 0 )
    {
      if ( a5->maxState )
      {
        if ( !a6 )
          __FrameUnwindToState((int)a2, (int)a4, (int)a5, -1);
      }
    }
    else if ( a5->nTryBlocks || (*(_DWORD *)a5 & 0x1FFFFFFFu) >= 0x19930521 && a5->pESTypeList )
    {
      if ( a1->ExceptionCode == -529697949 && a1->NumberParameters >= 3 && a1->params.magicNumber > 0x19930522 )
      {
        pForwardCompat = a1->params.pThrowInfo->pForwardCompat;
        if ( pForwardCompat )
          return ((int (__cdecl *)(struct EHExceptionRecord *, struct EHRegistrationNode *, struct _CONTEXT *, struct _CONTEXT *, struct _s_FuncInfo *, struct _s_HandlerType *, struct _s_CatchableType *, _DWORD))pForwardCompat)(
                   a1,
                   a2,
                   a3,
                   a4,
                   a5,
                   a6,
                   a7,
                   a8);
      }
      FindHandler(a1, a2, a3, a4, a5, a8, a6, a7);
    }
  }
  return 1;
}

// ===== ?terminate@@YAXXZ @ 0x004B2D58..0x004B2D91 =====
void __cdecl __noreturn terminate()
{
  void (*v0)(void); // eax

  v0 = (void (*)(void))_getptd()[30];
  if ( v0 )
    v0();
  abort();
}

// ===== ?unexpected@@YAXXZ @ 0x004B2D91..0x004B2DA4 =====
void __cdecl __noreturn unexpected()
{
  void (*v0)(void); // eax

  v0 = (void (*)(void))_getptd()[31];
  if ( v0 )
    v0();
  terminate();
}

// ===== ?_inconsistency@@YAXXZ @ 0x004B2DA4..0x004B2DDC =====
void __cdecl __noreturn _inconsistency()
{
  void (*v0)(void); // eax

  v0 = (void (*)(void))DecodePointer(dword_50A64C);
  if ( v0 )
    v0();
  terminate();
}

// ===== __initp_eh_hooks @ 0x004B2DDC..0x004B2DED =====
PVOID _initp_eh_hooks()
{
  PVOID result; // eax

  result = EncodePointer(terminate);
  dword_50A64C = result;
  return result;
}

// ===== __CallSettingFrame@12 @ 0x004B2DF0..0x004B2E3C =====
int __stdcall _CallSettingFrame(int a1, int a2, int a3)
{
  void (*v3)(void); // eax
  int v4; // ecx

  v3 = (void (*)(void))_NLG_Notify1(a3);
  v3();
  v4 = a3;
  if ( a3 == 256 )
    v4 = 2;
  return _NLG_Notify1(v4);
}

// ===== __forcdecpt_l @ 0x004B2E3C..0x004B2EB0 =====
char __cdecl _forcdecpt_l(char *a1, struct localeinfo_struct *a2)
{
  char *v2; // esi
  bool i; // zf
  char result; // al
  char *v5; // esi
  char v6; // cl
  int v8; // [esp+4h] [ebp-10h] BYREF
  int v9; // [esp+Ch] [ebp-8h]
  char v10; // [esp+10h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v8, a2);
  v2 = a1;
  for ( i = tolower(*a1) == 101; !i; i = isdigit((unsigned __int8)*v2) == 0 )
    ++v2;
  if ( tolower(*v2) == 120 )
    v2 += 2;
  result = *v2;
  *v2 = ***(_BYTE ***)(v8 + 188);
  v5 = v2 + 1;
  do
  {
    v6 = *v5;
    *v5 = result;
    result = v6;
  }
  while ( *v5++ );
  if ( v10 )
  {
    result = v9;
    *(_DWORD *)(v9 + 112) &= ~2u;
  }
  return result;
}

// ===== __cropzeros_l @ 0x004B2EB0..0x004B2F32 =====
char *__cdecl _cropzeros_l(char *a1, struct localeinfo_struct *a2)
{
  char *v2; // eax
  char i; // cl
  char v4; // cl
  char *result; // eax
  char v6; // cl
  char *v7; // edx
  char v8; // cl
  int v9; // [esp+4h] [ebp-10h] BYREF
  char *v10; // [esp+Ch] [ebp-8h]
  char v11; // [esp+10h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v9, a2);
  v2 = a1;
  for ( i = *a1; *v2; i = *++v2 )
  {
    if ( i == ***(_BYTE ***)(v9 + 188) )
      break;
  }
  v4 = *v2;
  result = v2 + 1;
  if ( v4 )
  {
    while ( 1 )
    {
      v6 = *result;
      if ( !*result || v6 == 101 || v6 == 69 )
        break;
      ++result;
    }
    v7 = result;
    do
      --result;
    while ( *result == 48 );
    if ( *result == ***(_BYTE ***)(v9 + 188) )
      --result;
    do
    {
      v8 = *v7;
      ++result;
      ++v7;
      *result = v8;
    }
    while ( v8 );
  }
  if ( v11 )
  {
    result = v10;
    *((_DWORD *)v10 + 28) &= ~2u;
  }
  return result;
}

// ===== __positive @ 0x004B2F32..0x004B2F4E =====
BOOL __cdecl _positive(double *a1)
{
  return *a1 >= 0.0;
}

// ===== __fassign_l @ 0x004B2F4E..0x004B2F90 =====
int *__cdecl _fassign_l(int a1, int *a2, int a3, struct localeinfo_struct *a4)
{
  int *result; // eax
  int v5[2]; // [esp+0h] [ebp-8h] BYREF

  if ( a1 )
  {
    sub_4B7C66((int)v5, a3, a4);
    result = a2;
    *a2 = v5[0];
    result[1] = v5[1];
  }
  else
  {
    sub_4B7D0E((int)&a1, a3, a4);
    result = a2;
    *a2 = a1;
  }
  return result;
}

// ===== __fassign @ 0x004B2F90..0x004B2FAA =====
int *__cdecl _fassign(int a1, int *a2, int a3)
{
  return _fassign_l(a1, a2, a3, 0);
}

// ===== __shift @ 0x004B2FAA..0x004B2FC9 =====
const char *__usercall _shift@<eax>(const char *result@<eax>, int a2@<edi>)
{
  const char *v2; // esi
  size_t v3; // eax

  v2 = result;
  if ( a2 )
  {
    v3 = strlen(result);
    return (const char *)memcpy((void *)&v2[a2], v2, v3 + 1);
  }
  return result;
}

// ===== __forcdecpt @ 0x004B2FC9..0x004B2FDC =====
char __cdecl _forcdecpt(char *a1)
{
  return _forcdecpt_l(a1, 0);
}

// ===== __cropzeros @ 0x004B2FDC..0x004B2FEF =====
char *__cdecl _cropzeros(char *a1)
{
  return _cropzeros_l(a1, 0);
}

// ===== __cftoe2_l @ 0x004B2FEF..0x004B314F =====
int __usercall _cftoe2_l@<eax>(
        _BYTE *a1@<eax>,
        unsigned int a2,
        int a3,
        int a4,
        int a5,
        char a6,
        struct localeinfo_struct *a7)
{
  int *v8; // eax
  int v10; // eax
  _BYTE *v11; // esi
  int v12; // eax
  char *v13; // esi
  rsize_t v14; // ebx
  int v15; // eax
  int v16; // [esp-4h] [ebp-1Ch]
  int v17; // [esp+8h] [ebp-10h] BYREF
  int v18; // [esp+10h] [ebp-8h]
  char v19; // [esp+14h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v17, a7);
  if ( !a1 || !a2 )
  {
    v8 = _errno();
    v16 = 22;
LABEL_3:
    *v8 = v16;
    _invalid_parameter_noinfo();
    if ( v19 )
      *(_DWORD *)(v18 + 112) &= ~2u;
    return v16;
  }
  if ( a3 <= 0 )
    v10 = 0;
  else
    v10 = a3;
  if ( a2 <= v10 + 9 )
  {
    v8 = _errno();
    v16 = 34;
    goto LABEL_3;
  }
  if ( a6 )
    _shift(&a1[*(_DWORD *)a5 == 45], a3 > 0);
  v11 = a1;
  if ( *(_DWORD *)a5 == 45 )
  {
    *a1 = 45;
    v11 = a1 + 1;
  }
  if ( a3 > 0 )
  {
    v12 = v17;
    *v11 = v11[1];
    *++v11 = ***(_BYTE ***)(v12 + 188);
  }
  v13 = &v11[a3 + (a6 == 0)];
  if ( a2 == -1 )
    v14 = -1;
  else
    v14 = a2 + a1 - v13;
  if ( strcpy_s(v13, v14, "e+000") )
    _invoke_watson(0, 0, 0, 0, 0);
  if ( a4 )
    *v13 = 69;
  if ( **(_BYTE **)(a5 + 12) != 48 )
  {
    v15 = *(_DWORD *)(a5 + 4) - 1;
    if ( v15 < 0 )
    {
      v15 = 1 - *(_DWORD *)(a5 + 4);
      v13[1] = 45;
    }
    if ( v15 >= 100 )
    {
      v13[2] += v15 / 100;
      v15 %= 100;
    }
    if ( v15 >= 10 )
    {
      v13[3] += v15 / 10;
      LOBYTE(v15) = v15 % 10;
    }
    v13[4] += v15;
  }
  if ( (byte_50A81C & 1) != 0 && v13[2] == 48 )
    memcpy(v13 + 2, v13 + 3, 3u);
  if ( v19 )
    *(_DWORD *)(v18 + 112) &= ~2u;
  return 0;
}

// ===== __cftoe_l @ 0x004B3150..0x004B3217 =====
int __cdecl _cftoe_l(_DWORD *a1, _BYTE *a2, unsigned int a3, int a4, int a5, struct localeinfo_struct *a6)
{
  int result; // eax
  int v7; // eax
  int v8[4]; // [esp+Ch] [ebp-2Ch] BYREF
  int v9[6]; // [esp+1Ch] [ebp-1Ch] BYREF

  _fltout2(*a1, a1[1], (int)v8, (int)v9, 0x16u);
  if ( a2 && (v7 = a3) != 0 )
  {
    if ( a3 != -1 )
      v7 = a3 - (v8[0] == 45) - (a4 > 0);
    result = _fptostr(&a2[(v8[0] == 45) + (a4 > 0)], v7, a4 + 1, (int)v8);
    if ( result )
      *a2 = 0;
    else
      return _cftoe2_l(a2, a3, a4, a5, (int)v8, 0, a6);
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  return result;
}

// ===== __cftoe @ 0x004B3217..0x004B3237 =====
int __cdecl _cftoe(_DWORD *a1, _BYTE *a2, unsigned int a3, int a4, int a5)
{
  return _cftoe_l(a1, a2, a3, a4, a5, 0);
}

// ===== __cftoa_l @ 0x004B3237..0x004B35AD =====
int __cdecl _cftoa_l(_DWORD *a1, _BYTE *a2, unsigned int a3, int Size, int a5, struct localeinfo_struct *a6)
{
  _BYTE *v6; // esi
  int *v7; // eax
  int result; // eax
  unsigned int v9; // eax
  bool v10; // zf
  char *v11; // eax
  _BYTE *v12; // esi
  _BYTE *v13; // eax
  _BYTE *v14; // esi
  int v15; // eax
  unsigned int v16; // eax
  unsigned int v17; // ecx
  _BYTE *i; // eax
  __int64 v19; // rax
  __int64 v20; // rcx
  _BYTE *v21; // esi
  _BYTE *v22; // edi
  __int64 v23; // rax
  __int64 v24; // rcx
  __int64 v25; // rax
  __int64 v26; // rcx
  __int64 v27; // rcx
  __int64 v28; // [esp-Ch] [ebp-38h]
  int v29; // [esp-4h] [ebp-30h]
  int v30; // [esp+8h] [ebp-24h] BYREF
  int v31; // [esp+10h] [ebp-1Ch]
  char v32; // [esp+14h] [ebp-18h]
  int v33; // [esp+18h] [ebp-14h]
  int v34; // [esp+1Ch] [ebp-10h]
  unsigned __int64 v35; // [esp+20h] [ebp-Ch]
  int v36; // [esp+28h] [ebp-4h]
  _BYTE *v37; // [esp+38h] [ebp+Ch]

  v33 = 1023;
  v36 = 48;
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v30, a6);
  if ( Size < 0 )
    Size = 0;
  v6 = a2;
  if ( !a2 || !a3 )
  {
    v7 = _errno();
    v29 = 22;
LABEL_5:
    *v7 = v29;
    _invalid_parameter_noinfo();
    if ( v32 )
      *(_DWORD *)(v31 + 112) &= ~2u;
    return v29;
  }
  *a2 = 0;
  if ( a3 <= Size + 11 )
  {
    v7 = _errno();
    v29 = 34;
    goto LABEL_5;
  }
  LODWORD(v35) = *a1;
  if ( ((a1[1] >> 20) & 0x7FF) == 0x7FF )
  {
    v9 = a3;
    if ( a3 != -1 )
      v9 = a3 - 2;
    result = _cftoe(a1, a2 + 2, v9, Size, 0);
    if ( result )
    {
      v10 = v32 == 0;
      *a2 = 0;
      if ( !v10 )
        *(_DWORD *)(v31 + 112) &= ~2u;
      return result;
    }
    if ( a2[2] == 45 )
    {
      *a2 = 45;
      v6 = a2 + 1;
    }
    *v6 = 48;
    v6[1] = a5 == 0 ? 120 : 88;
    v11 = strrchr(v6 + 2, 101);
    if ( v11 )
    {
      *v11 = a5 == 0 ? 112 : 80;
      v11[3] = 0;
    }
  }
  else
  {
    if ( (a1[1] & 0x80000000) != 0 )
    {
      *a2 = 45;
      v6 = a2 + 1;
    }
    *v6 = 48;
    v6[1] = a5 == 0 ? 120 : 88;
    if ( (a1[1] & 0x7FF00000) != 0 )
    {
      v6[2] = 49;
      v12 = v6 + 3;
    }
    else
    {
      v6[2] = 48;
      v12 = v6 + 3;
      if ( a1[1] & 0xFFFFF | *a1 )
        v33 = 1022;
      else
        v33 = 0;
    }
    v13 = v12;
    v14 = v12 + 1;
    v37 = v13;
    if ( Size )
      *v13 = ***(_BYTE ***)(v30 + 188);
    else
      *v13 = 0;
    v15 = *a1;
    HIDWORD(v35) = a1[1] & 0xFFFFF;
    if ( HIDWORD(v35) || v15 )
    {
      v35 = 0xF000000000000LL;
      do
      {
        if ( Size <= 0 )
          break;
        v16 = (unsigned __int16)(((v35 & *(_QWORD *)a1 & 0xFFFFFFFFFFFFFLL) >> v36) + 48);
        if ( v16 > 0x39 )
          LOBYTE(v16) = (a5 != 0 ? 7 : 39) + ((v35 & *(_QWORD *)a1 & 0xFFFFFFFFFFFFFLL) >> v36) + 48;
        v17 = HIDWORD(v35);
        v36 -= 4;
        *v14++ = v16;
        --Size;
        v35 = __PAIR64__(v17, v35) >> 4;
      }
      while ( (v36 & 0x8000u) == 0 );
      if ( (v36 & 0x8000u) == 0 && (unsigned __int16)((v35 & *(_QWORD *)a1 & 0xFFFFFFFFFFFFFLL) >> v36) > 8u )
      {
        for ( i = v14 - 1; *i == 102 || *i == 70; --i )
          *i = 48;
        if ( i == v37 )
        {
          ++*(i - 1);
        }
        else if ( *i == 57 )
        {
          *i = a5 != 0 ? 65 : 97;
        }
        else
        {
          ++*i;
        }
      }
    }
    if ( Size > 0 )
    {
      memset(v14, 48, Size);
      v14 += Size;
    }
    if ( !*v37 )
      v14 = v37;
    *v14 = a5 == 0 ? 112 : 80;
    HIDWORD(v20) = 0;
    v19 = ((*(_QWORD *)a1 >> 52) & 0x7FFLL) - (unsigned int)v33;
    if ( v19 < 0 )
    {
      v14[1] = 45;
      v21 = v14 + 2;
      v19 = -v19;
    }
    else
    {
      v14[1] = 43;
      v21 = v14 + 2;
    }
    v22 = v21;
    *v21 = 48;
    if ( v19 >= 0 )
    {
      LODWORD(v20) = 1000;
      if ( v19 >= 1000 )
      {
        v28 = v20;
        v24 = v19 % v20;
        v23 = v19 / v28;
        *v21++ = v23 + 48;
        v34 = HIDWORD(v23);
        v19 = v24;
        if ( v21 != v22 )
          goto LABEL_60;
      }
    }
    if ( v19 >= 100 )
    {
LABEL_60:
      v26 = v19 % 100;
      v25 = v19 / 100;
      *v21 = v25 + 48;
      v34 = HIDWORD(v25);
      ++v21;
      v19 = v26;
    }
    if ( v21 != v22 || v19 >= 10 )
    {
      v27 = v19 % 10;
      *v21++ = v19 / 10 + 48;
      LOBYTE(v19) = v19 % 10;
      v34 = HIDWORD(v27);
    }
    *v21 = v19 + 48;
    v21[1] = 0;
  }
  if ( v32 )
    *(_DWORD *)(v31 + 112) &= ~2u;
  return 0;
}

// ===== __cftof2_l @ 0x004B35AD..0x004B36B0 =====
int __usercall _cftof2_l@<eax>(
        char *Str@<ecx>,
        _DWORD *a2@<eax>,
        int a3,
        int Size,
        char a5,
        struct localeinfo_struct *a6)
{
  int v8; // esi
  char *v10; // esi
  int v11; // eax
  size_t v12; // eax
  char *v13; // esi
  size_t v14; // eax
  int v15; // edi
  int v16; // edi
  int v17; // [esp+Ch] [ebp-10h] BYREF
  int v18; // [esp+14h] [ebp-8h]
  char v19; // [esp+18h] [ebp-4h]

  v8 = a2[1] - 1;
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v17, a6);
  if ( Str && a3 )
  {
    if ( a5 && v8 == Size )
      *(_WORD *)&Str[v8 + (*a2 == 45)] = 48;
    v10 = Str;
    if ( *a2 == 45 )
    {
      *Str = 45;
      v10 = Str + 1;
    }
    v11 = a2[1];
    if ( v11 > 0 )
    {
      v13 = &v10[v11];
    }
    else
    {
      v12 = strlen(v10);
      memcpy(v10 + 1, v10, v12 + 1);
      *v10 = 48;
      v13 = v10 + 1;
    }
    if ( Size > 0 )
    {
      v14 = strlen(v13);
      memcpy(v13 + 1, v13, v14 + 1);
      *v13 = ***(_BYTE ***)(v17 + 188);
      v15 = a2[1];
      if ( v15 < 0 )
      {
        v16 = -v15;
        if ( a5 || Size >= v16 )
          Size = v16;
        _shift(v13 + 1, Size);
        memset(v13 + 1, 48, Size);
      }
    }
    if ( v19 )
      *(_DWORD *)(v18 + 112) &= ~2u;
    return 0;
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    if ( v19 )
      *(_DWORD *)(v18 + 112) &= ~2u;
    return 22;
  }
}

// ===== __cftof_l @ 0x004B36B0..0x004B3771 =====
int __cdecl _cftof_l(_DWORD *a1, char *Str, int a3, size_t Size, struct localeinfo_struct *a5)
{
  int result; // eax
  int v6; // eax
  int v7[4]; // [esp+8h] [ebp-2Ch] BYREF
  int v8[6]; // [esp+18h] [ebp-1Ch] BYREF

  _fltout2(*a1, a1[1], (int)v7, (int)v8, 0x16u);
  if ( Str && a3 )
  {
    v6 = -1;
    if ( a3 != -1 )
      v6 = a3 - (v7[0] == 45);
    result = _fptostr(&Str[v7[0] == 45], v6, Size + v7[1], (int)v7);
    if ( result )
      *Str = 0;
    else
      return _cftof2_l(Str, v7, a3, Size, 0, a5);
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  return result;
}

// ===== __cftog_l @ 0x004B3771..0x004B385E =====
int __cdecl _cftog_l(_DWORD *a1, char *Str, int a3, signed int Size, int a5, struct localeinfo_struct *a6)
{
  int result; // eax
  int v7; // ecx
  int v8; // ebx
  BOOL v9; // eax
  char *v10; // edi
  int v11; // [esp+8h] [ebp-2Ch] BYREF
  int v12; // [esp+Ch] [ebp-28h]
  int v13[6]; // [esp+18h] [ebp-1Ch] BYREF

  _fltout2(*a1, a1[1], (int)&v11, (int)v13, 0x16u);
  if ( Str && (v7 = a3) != 0 )
  {
    v8 = v12 - 1;
    v9 = v11 == 45;
    v10 = &Str[v9];
    if ( a3 != -1 )
      v7 = a3 - v9;
    result = _fptostr(v10, v7, Size, (int)&v11);
    if ( result )
    {
      *Str = 0;
    }
    else if ( v12 - 1 < -4 || v12 - 1 >= Size )
    {
      return _cftoe2_l(Str, a3, Size, a5, (int)&v11, 1, a6);
    }
    else
    {
      if ( v8 < v12 - 1 )
        v10[strlen(v10) - 1] = 0;
      return _cftof2_l(Str, &v11, a3, Size, 1, a6);
    }
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  return result;
}

// ===== __cfltcvt_l @ 0x004B385E..0x004B38E6 =====
int __cdecl _cfltcvt_l(_DWORD *a1, char *Str, int a3, int a4, size_t Size, int a6, struct localeinfo_struct *a7)
{
  switch ( a4 )
  {
    case 'e':
    case 'E':
      return _cftoe_l(a1, Str, a3, Size, a6, a7);
    case 'f':
      return _cftof_l(a1, Str, a3, Size, a7);
    case 'a':
    case 'A':
      return _cftoa_l(a1, Str, a3, Size, a6, a7);
  }
  return _cftog_l(a1, Str, a3, Size, a6, a7);
}

// ===== __cfltcvt @ 0x004B38E6..0x004B3909 =====
int __cdecl _cfltcvt(_DWORD *a1, char *Str, int a3, int a4, size_t Size, int a6)
{
  return _cfltcvt_l(a1, Str, a3, a4, Size, a6, 0);
}

// ===== __initp_misc_cfltcvt_tab @ 0x004B3909..0x004B392C =====
void (__noreturn *_initp_misc_cfltcvt_tab())()
{
  unsigned int i; // edi
  void (__noreturn *result)(); // eax

  for ( i = 0; i < 10; ++i )
  {
    result = (void (__noreturn *)())EncodePointer(off_4FC0E0[i]);
    off_4FC0E0[i] = result;
  }
  return result;
}

// ===== __setdefaultprecision @ 0x004B392C..0x004B3954 =====
errno_t _setdefaultprecision()
{
  errno_t result; // eax

  result = _controlfp_s(0, 0x10000u, 0x30000u);
  if ( result )
    _invoke_watson(0, 0, 0, 0, 0);
  return result;
}

// ===== __alloca_probe_16 @ 0x004B3960..0x004B3976 =====
void *__usercall _alloca_probe_16@<eax>(int a1@<eax>, int a2@<ecx>)
{
  char v2; // sp
  int v3; // ecx

  v3 = (v2 + 8 - (_BYTE)a1) & 0xF;
  return _alloca_probe(__CFADD__(v3, a1) ? -1 : v3 + a1, a2);
}

// ===== __alloca_probe_8 @ 0x004B3976..0x004B398C =====
void *__usercall _alloca_probe_8@<eax>(int a1@<eax>, int a2@<ecx>)
{
  char v2; // sp
  int v3; // ecx

  v3 = (v2 + 8 - (_BYTE)a1) & 7;
  return _alloca_probe(__CFADD__(v3, a1) ? -1 : v3 + a1, a2);
}

// ===== __CIpow_pentium4 @ 0x004B3A50..0x004B3A69 =====
void __usercall _CIpow_pentium4(double a1@<st1>, double a2@<st0>)
{
  _pow_pentium4(a2, a1);
}

// ===== __pow_pentium4 @ 0x004B3A69..0x004B45A1 =====
double __usercall _pow_pentium4@<st0>(
        __m128d a1@<xmm0>,
        __m128d a2@<xmm1>,
        __m128d a3@<xmm2>,
        __m128d a4@<xmm3>,
        __m128d a5@<xmm4>,
        __m128d a6@<xmm5>,
        __m128d a7@<xmm7>,
        double a8,
        double a9)
{
  __m128d v9; // xmm7
  __m128d inserted; // xmm0
  int epi16; // ecx
  int v12; // eax
  __m128d v13; // xmm7
  __m128d v14; // xmm6
  int v15; // edx
  unsigned int v16; // ecx
  unsigned int v17; // edx
  __m128d v18; // xmm1
  __m128d v19; // xmm0
  __m128d v20; // xmm7
  int v21; // eax
  __m128d v22; // xmm6
  double v23; // xmm2_8
  int v24; // eax
  double v25; // xmm2_8
  double v26; // xmm5_8
  __m128d v27; // xmm3
  double v28; // xmm2_8
  double v29; // xmm2_8
  __m128i v30; // xmm1
  int v31; // edx
  double v32; // xmm2_8
  __m128d v33; // xmm0
  __m128d v34; // xmm6
  __m128d v35; // xmm7
  unsigned int v36; // eax
  double v37; // xmm2_8
  double v38; // xmm4_8
  double v39; // xmm3_8
  double v40; // xmm1_8
  double v41; // xmm3_8
  double v42; // xmm5_8
  double v43; // xmm5_8
  double v44; // xmm4_8
  double v45; // xmm5_8
  int v46; // eax
  __m128d v47; // xmm2
  __m128d v48; // xmm7
  __m128i v49; // xmm2
  __m128i v50; // xmm3
  __m128d v51; // xmm0
  __m128d v52; // xmm6
  double v53; // xmm4_8
  __m128d v54; // xmm0
  __m128i v55; // xmm7
  double result; // st7
  int v57; // eax
  __m128i v58; // xmm1
  unsigned int v59; // ecx
  __m128i v60; // xmm2
  __m128d v61; // xmm1
  __m128i v62; // xmm2
  unsigned __int8 v63; // al
  unsigned int v64; // edx
  __m128i v65; // xmm1
  int v66; // edx
  int v67; // eax
  __m128d v68; // xmm7
  int v69; // eax
  __m128d v70; // xmm0
  int v71; // edx
  __m128d v72; // xmm1
  int v73; // eax
  int v74; // eax
  int v75; // eax
  __m128i v76; // xmm4
  int v77; // ecx
  unsigned int v78; // eax
  int v79; // eax
  unsigned int v80; // ecx
  unsigned int v81; // edx
  unsigned int v82; // edi
  double v83; // xmm4_8
  __m128d v84; // xmm2
  __m128d v85; // xmm7
  __m128i v86; // xmm2
  __m128i v87; // xmm3
  __m128d v88; // xmm0
  __m128d v89; // xmm6
  double v90; // xmm4_8
  __m128i v91; // xmm7
  double v92; // xmm7_8
  double v93; // xmm1_8
  __int64 v94; // xmm4_8
  double v95; // xmm1_8
  double v96; // xmm6_8
  int v97; // eax
  int v98; // eax
  int v99; // eax
  int v100; // edx
  unsigned int v101; // eax
  int v102; // [esp+0h] [ebp-1Ch] BYREF
  double v103; // [esp+10h] [ebp-Ch] BYREF

  a1.m128d_f64[0] = a8;
  a7.m128d_f64[0] = COERCE_DOUBLE(0xFFFFFFFFFFFFFLL);
  a3.m128d_f64[0] = 1.0;
  v9 = _mm_and_pd(a7, a1);
  a5.m128d_f64[0] = a8;
  inserted = (__m128d)_mm_srli_epi64((__m128i)a1, 0x2Cu);
  v13 = _mm_or_pd(v9, a3);
  epi16 = _mm_extract_epi16((__m128i)a5, 3);
  v12 = ((unsigned __int8)_mm_extract_epi16((__m128i)inserted, 0) + 1) & 0x1FE;
  v13.m128d_f64[0] = v13.m128d_f64[0] * *(double *)((char *)&qword_4DD430 + 4 * v12);
  a6.m128d_f64[0] = *(double *)((char *)&qword_4DD430 + 4 * v12);
  v14 = *(__m128d *)((char *)&xmmword_4DD840 + 8 * v12);
  v15 = 32751 - epi16;
  if ( ((32751 - epi16) | (unsigned int)(epi16 - 16)) < 0x80000000 )
  {
    v16 = 0;
    v17 = 261759;
    goto BACK_MAIN;
  }
  a2.m128d_f64[0] = a9;
  a4.m128d_f64[0] = NAN;
  v57 = _mm_cvtsi128_si32((__m128i)a2);
  a3.m128d_f64[0] = a9;
  v58 = _mm_srli_epi64((__m128i)_mm_and_pd(a2, a4), 0x20u);
  v59 = _mm_cvtsi128_si32(v58);
  if ( v59 >= 0x7FF00000 )
  {
    v13.m128d_f64[0] = a8;
    a5.m128d_f64[0] = a8;
    v31 = _mm_cvtsi128_si32((__m128i)v13);
    v16 = _mm_cvtsi128_si32(_mm_srli_epi64((__m128i)v13, 0x20u));
    if ( (v16 & 0x7FFFFFFF) < 0x7FF00000 || (v16 & 0x7FFFFFFF) <= 0x7FF00000 && !v31 )
      goto Y_INF_NAN;
    goto X_NAN;
  }
  if ( !(v59 | v57) )
  {
    v75 = _mm_cvtsi128_si32((__m128i)a5);
    v76 = _mm_srli_epi64((__m128i)a5, 0x20u);
    v77 = v75;
    v70.m128d_f64[0] = 1.0;
    v71 = 26;
    if ( _mm_cvtsi128_si32(v76) & 0x7FFFFFFF | v75 )
    {
      v71 = 29;
      v78 = _mm_cvtsi128_si32(v76) & 0x7FFFFFFF;
      if ( v78 <= 0x7FF00000 && (v78 < 0x7FF00000 || !v77) )
      {
        v103 = 1.0;
        return 1.0;
      }
    }
    goto CALL_LIBM_ERROR;
  }
  if ( v15 >= 0 )
  {
    v16 = 0;
    goto DENORMAL_X;
  }
  v60 = (__m128i)_mm_or_pd(a3, (__m128d)_mm_slli_epi64((__m128i)a4, 0x34u));
  a4 = 0LL;
  v61 = (__m128d)_mm_max_epi16(_mm_sub_epi32(_mm_srli_epi64(v58, 0x14u), _mm_cvtsi32_si128(0x3F3u)), (__m128i)0LL);
  v62 = _mm_cmpeq_epi32(_mm_sll_epi64(v60, (__m128i)v61), (__m128i)0LL);
  v63 = _mm_movemask_epi8(v62);
  v64 = (32751 - v15) & 0x7FFF;
  if ( v64 >= 0x7FF0 )
  {
    a4.m128d_f64[0] = COERCE_DOUBLE(0xFFFFFFFFFFFFFLL);
    v72 = (__m128d)_mm_cmpeq_epi32((__m128i)0LL, (__m128i)_mm_and_pd(a4, a5));
    if ( (unsigned __int8)_mm_movemask_epi8((__m128i)v72) == 255 )
    {
      if ( (_mm_extract_epi16((__m128i)a5, 3) & 0x8000) != 0 )
      {
        if ( v63 == 255 )
        {
          v72.m128d_f64[0] = a9;
          *(double *)v62.m128i_i64 = a9;
          v72 = (__m128d)_mm_sub_epi32(
                           _mm_srli_epi64((__m128i)_mm_and_pd(v72, (__m128d)xmmword_4E0CE0), 0x34u),
                           _mm_cvtsi32_si128(0x3F4u));
          if ( (unsigned __int8)_mm_movemask_epi8(_mm_cmpeq_epi32(_mm_sll_epi64(v62, (__m128i)v72), (__m128i)0LL)) != 255 )
          {
            v72.m128d_f64[0] = a9;
            if ( (_mm_extract_epi16((__m128i)v72, 3) & 0x8000) == 0 )
              return -INFINITY;
            return -0.0;
          }
        }
        v72.m128d_f64[0] = a9;
        if ( (_mm_extract_epi16((__m128i)v72, 3) & 0x8000) != 0 )
          return 0.0;
      }
      else
      {
        v72.m128d_f64[0] = a9;
        if ( (_mm_extract_epi16((__m128i)v72, 3) & 0x8000) != 0 )
          return 0.0;
      }
      return INFINITY;
    }
X_NAN:
    v70.m128d_f64[0] = a5.m128d_f64[0] + a5.m128d_f64[0];
    v71 = 1006;
    goto CALL_LIBM_ERROR;
  }
  if ( v63 != 255 )
  {
    *(double *)v62.m128i_i64 = a8;
    v73 = _mm_cvtsi128_si32(v62);
    a3 = (__m128d)_mm_srli_epi64(v62, 0x20u);
    v67 = _mm_cvtsi128_si32((__m128i)a3) & 0x7FFFFFFF | v73;
    v16 = 0;
    if ( v67 )
    {
      v70.m128d_f64[0] = NAN;
      v71 = 28;
      goto CALL_LIBM_ERROR;
    }
    goto ZERO_X;
  }
  v61.m128d_f64[0] = a9;
  *(double *)v62.m128i_i64 = a9;
  a4 = (__m128d)_mm_cvtsi32_si128(0x3F4u);
  v65 = _mm_sub_epi32(_mm_srli_epi64((__m128i)_mm_and_pd(v61, (__m128d)xmmword_4E0CE0), 0x34u), (__m128i)a4);
  a4.m128d_f64[0] = -0.0;
  a3 = (__m128d)_mm_cmpeq_epi32(_mm_sll_epi64(v62, v65), (__m128i)a4);
  v16 = ((unsigned __int8)_mm_movemask_epi8((__m128i)a3) + 261889) & 0x40000;
  if ( v64 >= 0x10 )
  {
    v17 = 786047;
    goto BACK_MAIN;
  }
DENORMAL_X:
  inserted = (__m128d)_mm_insert_epi16((__m128i)0LL, 0x43F0u, 3);
  v13.m128d_f64[0] = COERCE_DOUBLE(0xFFFFFFFFFFFFFLL);
  a3.m128d_f64[0] = 1.0;
  inserted.m128d_f64[0] = inserted.m128d_f64[0] * a8;
  v66 = _mm_cvtsi128_si32((__m128i)a5);
  a5 = (__m128d)_mm_srli_epi64((__m128i)a5, 0x20u);
  v67 = _mm_cvtsi128_si32((__m128i)a5);
  if ( !v66 )
  {
ZERO_X:
    if ( (v67 & 0x7FFFFFFF) != 0 )
      goto BACK_DEN;
    if ( a9 < 0.0 )
    {
      *(_QWORD *)&v70.m128d_f64[0] = _mm_cvtsi32_si128((v16 << 13) & v67 | 0x7FF00000).m128i_u64[0] << 32;
      v71 = 27;
      goto CALL_LIBM_ERROR;
    }
    if ( ((v16 << 13) & v67) == 0 )
      return 0.0;
    return -0.0;
  }
BACK_DEN:
  v68 = _mm_and_pd(v13, inserted);
  a5.m128d_f64[0] = inserted.m128d_f64[0];
  inserted = (__m128d)_mm_srli_epi64((__m128i)_mm_and_pd(inserted, (__m128d)xmmword_4E0CE0), 0x2Cu);
  v13 = _mm_or_pd(v68, a3);
  v69 = ((unsigned __int8)_mm_extract_epi16((__m128i)inserted, 0) + 1) & 0x1FE;
  v13.m128d_f64[0] = v13.m128d_f64[0] * *(double *)((char *)&qword_4DD430 + 4 * v69);
  a6.m128d_f64[0] = *(double *)((char *)&qword_4DD430 + 4 * v69);
  v14 = *(__m128d *)((char *)&xmmword_4DD840 + 8 * v69);
  v17 = 278143;
BACK_MAIN:
  v18 = (__m128d)_mm_cvtsi32_si128(v17);
  v19 = _mm_cvtepi32_pd(_mm_srli_epi64(_mm_sub_epi64((__m128i)inserted, (__m128i)v18), 8u));
  v18.m128d_f64[0] = NAN;
  a4.m128d_f64[0] = v13.m128d_f64[0];
  v20 = (__m128d)_mm_srli_epi64((__m128i)v13, 0x26u);
  v21 = ((unsigned __int8)_mm_extract_epi16((__m128i)v20, 0) + 1) & 0x1FE;
  a4.m128d_f64[0] = a4.m128d_f64[0] * *(double *)((char *)&qword_4DE050 + 4 * v21);
  a6.m128d_f64[0] = a6.m128d_f64[0] * *(double *)((char *)&qword_4DE050 + 4 * v21);
  v22 = _mm_add_pd(v14, *(__m128d *)((char *)&xmmword_4DE460 + 8 * v21));
  a5 = _mm_or_pd(_mm_and_pd(a5, (__m128d)xmmword_4E0CC0), (__m128d)xmmword_4E0CD0);
  v22.m128d_f64[0] = v22.m128d_f64[0] + v19.m128d_f64[0];
  v30 = (__m128i)_mm_and_pd(v18, a5);
  v23 = a4.m128d_f64[0];
  v27 = (__m128d)_mm_srli_epi64((__m128i)a4, 0x1Fu);
  v19.m128d_f64[0] = NAN;
  a5.m128d_f64[0] = a5.m128d_f64[0] - *(double *)v30.m128i_i64;
  v24 = ((_mm_extract_epi16((__m128i)v27, 0) & 0x1FF) + 1) & 0x3FE;
  a6.m128d_f64[0] = a6.m128d_f64[0] * *(double *)((char *)&qword_4DEC70 + 4 * v24);
  v25 = v23 * *(double *)((char *)&qword_4DEC70 + 4 * v24);
  v34 = _mm_add_pd(v22, *(__m128d *)((char *)&xmmword_4DF480 + 8 * v24));
  v33 = _mm_and_pd(v19, a6);
  v26 = a6.m128d_f64[0] - v33.m128d_f64[0];
  v20.m128d_f64[0] = v25 + -1.442694902420044;
  v27.m128d_f64[0] = v33.m128d_f64[0];
  v28 = v25 - v33.m128d_f64[0] * *(double *)v30.m128i_i64;
  v33.m128d_f64[0] = v34.m128d_f64[0];
  v29 = v28 - *(double *)v30.m128i_i64 * v26;
  v34.m128d_f64[0] = v34.m128d_f64[0] + v20.m128d_f64[0];
  *(double *)v30.m128i_i64 = a9;
  v31 = _mm_extract_epi16((__m128i)v34, 3);
  v32 = v29 - v27.m128d_f64[0] * a5.m128d_f64[0] - a5.m128d_f64[0] * v26;
  a5.m128d_f64[0] = v34.m128d_f64[0];
  v33.m128d_f64[0] = v33.m128d_f64[0] - v34.m128d_f64[0] + v20.m128d_f64[0];
  v20.m128d_f64[0] = v20.m128d_f64[0] - v32;
  v34.m128d_f64[0] = v34.m128d_f64[0] - v32;
  v35 = _mm_unpacklo_pd(v20, v20);
  v36 = _mm_extract_epi16(v30, 3) & 0x7FF0;
  if ( v36 >= 0x7FF0 )
  {
    a5.m128d_f64[0] = a8;
    a3.m128d_f64[0] = a9;
    v27.m128d_f64[0] = COERCE_DOUBLE(0xFFFFFFFFFFFFFLL);
    a4 = _mm_and_pd(v27, a3);
    if ( (unsigned __int8)_mm_movemask_epi8(_mm_cmpeq_epi32((__m128i)0LL, (__m128i)a4)) != 255 )
      goto RET_Y_NAN;
    if ( !_mm_cvtsi128_si32((__m128i)a5) )
    {
      a5 = (__m128d)_mm_srli_epi64((__m128i)a5, 0x20u);
      v31 = _mm_cvtsi128_si32((__m128i)a5);
      if ( v31 == 1072693248 )
        goto RET_ONE;
      if ( v31 == -1074790400 )
        return 1.0;
    }
Y_INF_NAN:
    a4.m128d_f64[0] = COERCE_DOUBLE(0xFFFFFFFFFFFFFLL);
    if ( (unsigned __int8)_mm_movemask_epi8(_mm_cmpeq_epi32((__m128i)0LL, (__m128i)_mm_and_pd(a4, a3))) == 255 )
    {
      a5.m128d_f64[0] = a8;
      v74 = _mm_extract_epi16((__m128i)a3, 3) & 0x8000;
      if ( v16 ^ 0xBFF00000 | v31 )
      {
        if ( v74 )
        {
          if ( (_mm_extract_epi16((__m128i)a5, 3) & 0x7FF0u) >= 0x3FF0 )
            return 0.0;
        }
        else if ( (_mm_extract_epi16((__m128i)a5, 3) & 0x7FF0u) < 0x3FF0 )
        {
          return 0.0;
        }
        return INFINITY;
      }
RET_ONE:
      v70.m128d_f64[0] = 1.0;
      v71 = 28;
      goto CALL_LIBM_ERROR;
    }
RET_Y_NAN:
    v70.m128d_f64[0] = a3.m128d_f64[0] + a3.m128d_f64[0];
    v71 = 1006;
    goto CALL_LIBM_ERROR;
  }
  if ( (((v31 & 0x7FF0) + v36 - 16368 - 15472) | (16544 - ((v31 & 0x7FF0) + v36 - 16368))) < 0x80000000
    || (a5 = _mm_mul_pd((__m128d)_mm_shuffle_epi32(v30, 68), v34),
        v99 = _mm_extract_epi16((__m128i)a5, 3) & 0x7FF0,
        v100 = 16544 - v99,
        v101 = v99 - 15472,
        (v101 | v100) < 0x80000000) )
  {
    v37 = v32 - (a5.m128d_f64[0] - v34.m128d_f64[0]);
    *(_QWORD *)&v38 = COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&v34.m128d_f64[0];
    v33.m128d_f64[0] = v33.m128d_f64[0] - v37;
    v39 = COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&a9)
        * COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&v34.m128d_f64[0]);
    v34.m128d_f64[0] = v34.m128d_f64[0] - COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&v34.m128d_f64[0]);
    v40 = a9 - COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&a9);
    v41 = v39 * *(double *)_mm_insert_epi16((__m128i)0LL, 0x4060u, 3).m128i_i64;
    v42 = COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & *(_QWORD *)&a9) * v34.m128d_f64[0];
    v34.m128d_f64[0] = v34.m128d_f64[0] * v40;
    v43 = v42 + v38 * v40;
    *(_QWORD *)&v44 = _mm_shuffle_epi32((__m128i)v34, 238).m128i_u64[0];
    v45 = v43 + v34.m128d_f64[0];
    v46 = (int)v41;
    if ( (((int)v41 + 123391) | (130943 - (int)v41)) > 0 )
    {
      v47 = _mm_mul_pd((__m128d)xmmword_4E0490, v35);
      v48 = _mm_mul_pd(v35, v35);
      v49 = (__m128i)_mm_add_pd(v47, _mm_mul_pd((__m128d)xmmword_4E04A0, v48));
      *(double *)v49.m128i_i64 = *(double *)v49.m128i_i64 * v48.m128d_f64[0]
                               + *(double *)_mm_shuffle_epi32(v49, 238).m128i_i64
                               + v44
                               + v33.m128d_f64[0];
      v33.m128d_f64[0] = (v41 - (v41 + 6.755399441055744e15 - 6.755399441055744e15))
                       * *(double *)_mm_insert_epi16((__m128i)0LL, 0x3F80u, 3).m128i_i64;
      v50 = (__m128i)_mm_mul_pd(
                       (__m128d)xmmword_4E04B0[(int)v41 & 0x7F],
                       (__m128d)_mm_shuffle_epi32(
                                  _mm_slli_epi64(_mm_cvtsi32_si128((((int)v41 + v16) & 0xFFFFFF80) + 130944), 0x2Du),
                                  68));
      v33.m128d_f64[0] = v33.m128d_f64[0] + v45 + *(double *)v49.m128i_i64 * a9;
      v51 = _mm_unpacklo_pd(v33, v33);
      v52 = _mm_mul_pd((__m128d)xmmword_4E0CF0, v51);
      v53 = 0.6931471805599453 * v51.m128d_f64[0];
      v54 = _mm_mul_pd(v51, v51);
      v55 = (__m128i)_mm_mul_pd(_mm_add_pd((__m128d)xmmword_4E0D00, v52), v54);
      return v54.m128d_f64[0] * *(double *)v50.m128i_i64 * *(double *)v55.m128i_i64
           + *(double *)_mm_shuffle_epi32(v50, 238).m128i_i64
           + *(double *)_mm_shuffle_epi32(v55, 238).m128i_i64 * *(double *)v50.m128i_i64
           + v53 * *(double *)v50.m128i_i64
           + *(double *)v50.m128i_i64;
    }
    if ( v46 <= 0 )
    {
      if ( v46 <= -261632 )
      {
RET_ZERO_UF:
        *(_QWORD *)&v70.m128d_f64[0] = COERCE_UNSIGNED_INT64(2.225073858507201e-308 * 2.225073858507201e-308) | (_mm_cvtsi32_si128(v16).m128i_u64[0] << 45);
        v71 = 25;
        goto CALL_LIBM_ERROR;
      }
      v79 = (int)v41 & 0x7F;
      v80 = v16 + 128;
      v81 = ((int)v41 & 0xFFFFFF80) + 261760;
      v82 = 0;
    }
    else
    {
      if ( (unsigned int)v46 >= 0x40000 )
        goto RET_INF_OF;
      v79 = (int)v41 & 0x7F;
      v80 = v16 + 261888;
      v81 = ((int)v41 - 128) & 0xFFFFFF80;
      v82 = 16368;
    }
    v83 = v44 + v33.m128d_f64[0];
    v84 = _mm_mul_pd((__m128d)xmmword_4E0490, v35);
    v85 = _mm_mul_pd(v35, v35);
    v33.m128d_f64[0] = v41 - (v41 + 6.755399441055744e15 - 6.755399441055744e15);
    v86 = (__m128i)_mm_add_pd(v84, _mm_mul_pd((__m128d)xmmword_4E04A0, v85));
    v87 = (__m128i)_mm_mul_pd(
                     (__m128d)xmmword_4E04B0[v79],
                     (__m128d)_mm_shuffle_epi32(_mm_slli_epi64(_mm_cvtsi32_si128(v81), 0x2Du), 68));
    v33.m128d_f64[0] = v33.m128d_f64[0] * *(double *)_mm_insert_epi16((__m128i)0LL, 0x3F80u, 3).m128i_i64
                     + v45
                     + (*(double *)v86.m128i_i64 * v85.m128d_f64[0]
                      + *(double *)_mm_shuffle_epi32(v86, 238).m128i_i64
                      + v83)
                     * a9;
    v88 = _mm_unpacklo_pd(v33, v33);
    v89 = _mm_mul_pd((__m128d)xmmword_4E0CF0, v88);
    v90 = 0.6931471805599453 * v88.m128d_f64[0];
    v70 = _mm_mul_pd(v88, v88);
    v91 = (__m128i)_mm_mul_pd(_mm_add_pd((__m128d)xmmword_4E0D00, v89), v70);
    v89.m128d_f64[0] = *(double *)_mm_shuffle_epi32(v91, 238).m128i_i64;
    v70.m128d_f64[0] = v70.m128d_f64[0] * *(double *)v87.m128i_i64 * *(double *)v91.m128i_i64;
    *(_QWORD *)&v92 = _mm_cvtsi32_si128(v80).m128i_u64[0] << 45;
    v86.m128i_i8[0] = _mm_cvtsi32_si128((((unsigned __int8)((int)(130944 - v81) >> 7) + 2) & 0x20u) + ((int)(130944 - v81) >> 7) + 2).m128i_u8[0];
    *(_QWORD *)&v93 = (-1LL << v86.m128i_i8[0]) & v87.m128i_i64[0];
    v70.m128d_f64[0] = v70.m128d_f64[0]
                     + *(double *)_mm_shuffle_epi32(v87, 238).m128i_i64
                     + v89.m128d_f64[0] * *(double *)v87.m128i_i64
                     + v90 * *(double *)v87.m128i_i64;
    v94 = -1LL << v86.m128i_i8[0];
    *(double *)v87.m128i_i64 = *(double *)v87.m128i_i64 - v93;
    *(double *)v86.m128i_i64 = v93;
    *(_QWORD *)&v95 = COERCE_UNSIGNED_INT64(v93 + v70.m128d_f64[0]) & v94;
    *(_QWORD *)&v96 = _mm_insert_epi16((__m128i)0LL, v82, 3).m128i_u64[0];
    v70.m128d_f64[0] = v70.m128d_f64[0] + *(double *)v86.m128i_i64 - v95 + *(double *)v87.m128i_i64;
    if ( (int)(v81 - 130944) > 0 )
    {
      v70.m128d_f64[0] = (v70.m128d_f64[0] + v95) * v92 + v96 * ((v70.m128d_f64[0] + v95) * v92);
      v98 = _mm_extract_epi16((__m128i)v70, 3) & 0x7FF0;
      v71 = 24;
      if ( v98 != 32752 )
      {
        v71 = 25;
        if ( v98 )
        {
          v103 = v70.m128d_f64[0];
          return v70.m128d_f64[0];
        }
      }
    }
    else
    {
      v70.m128d_f64[0] = v70.m128d_f64[0] * v92 + v95 * v92 + v96 * (v70.m128d_f64[0] * v92 + v95 * v92);
      v97 = _mm_extract_epi16((__m128i)v70, 3) & 0x7FF0;
      v71 = 24;
      if ( v97 != 32752 )
      {
        v71 = 25;
        if ( v97 )
        {
          v103 = v70.m128d_f64[0];
          return v70.m128d_f64[0];
        }
      }
    }
CALL_LIBM_ERROR:
    v103 = v70.m128d_f64[0];
    __libm_error_support((double *)&v102 + 4, &a9, &v103, v71);
    return v103;
  }
  if ( v101 < 0x80000000 )
  {
    a5.m128d_f64[0] = a8;
    if ( (((unsigned __int16)((_mm_extract_epi16((__m128i)a5, 3) & 0x7FF0) - 16368) ^ (unsigned __int16)_mm_extract_epi16(v30, 3)) & 0x8000) == 0 )
    {
RET_INF_OF:
      v71 = 24;
      if ( v16 )
        v70.m128d_f64[0] = -8.98846567431158e307 * 8.98846567431158e307;
      else
        v70.m128d_f64[0] = 8.98846567431158e307 * 8.98846567431158e307;
      goto CALL_LIBM_ERROR;
    }
    goto RET_ZERO_UF;
  }
  *(_QWORD *)&result = _mm_cvtsi32_si128(v16 | 0x1FF80).m128i_u64[0] << 45;
  return result;
}
