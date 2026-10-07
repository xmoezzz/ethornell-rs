#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== __mbsstr_l @ 0x004AC066..0x004AC190 =====
unsigned __int8 *__cdecl _mbsstr_l(const unsigned __int8 *Str, const unsigned __int8 *Substr, _locale_t Locale)
{
  unsigned __int8 *result; // eax
  const unsigned __int8 *v4; // edi
  const unsigned __int8 *v5; // ebx
  const unsigned __int8 *v6; // eax
  int v7; // esi
  unsigned __int8 v8; // bl
  const unsigned __int8 *v9; // ecx
  _BYTE v10[4]; // [esp+0h] [ebp-10h] BYREF
  int v11; // [esp+4h] [ebp-Ch]
  int v12; // [esp+8h] [ebp-8h]
  char v13; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v10, (struct localeinfo_struct *)Locale);
  if ( *(_DWORD *)(v11 + 8) )
  {
    if ( Substr )
    {
      if ( *Substr )
      {
        if ( Str )
        {
          v4 = Str;
          v5 = &Str[-strlen((const char *)Substr)];
          v6 = &v5[strlen((const char *)Str)];
          if ( *Str )
          {
            v7 = Str - Substr;
            while ( v4 <= v6 )
            {
              v8 = *v4;
              v9 = Substr;
              if ( *v4 )
              {
                while ( *v9 )
                {
                  if ( v9[v7] == *v9 )
                  {
                    ++v9;
                    if ( v9[v7] )
                      continue;
                  }
                  goto LABEL_23;
                }
LABEL_32:
                if ( v13 )
                  *(_DWORD *)(v12 + 112) &= ~2u;
                return (unsigned __int8 *)v4;
              }
LABEL_23:
              if ( !*v9 )
                goto LABEL_32;
              ++v4;
              ++v7;
              if ( (*(_BYTE *)(v8 + v11 + 29) & 4) != 0 )
              {
                if ( !*v4 )
                  break;
                ++v4;
                ++v7;
              }
              if ( !*v4 )
                break;
            }
          }
          if ( v13 )
            *(_DWORD *)(v12 + 112) &= ~2u;
          return 0;
        }
        else
        {
          *_errno() = 22;
          _invalid_parameter_noinfo();
          if ( v13 )
            *(_DWORD *)(v12 + 112) &= ~2u;
          return 0;
        }
      }
      else
      {
        if ( v13 )
          *(_DWORD *)(v12 + 112) &= ~2u;
        return (unsigned __int8 *)Str;
      }
    }
    else
    {
      *_errno() = 22;
      _invalid_parameter_noinfo();
      if ( v13 )
        *(_DWORD *)(v12 + 112) &= ~2u;
      return 0;
    }
  }
  else
  {
    result = (unsigned __int8 *)strstr((const char *)Str, (const char *)Substr);
    if ( v13 )
      *(_DWORD *)(v12 + 112) &= ~2u;
  }
  return result;
}

// ===== __mbsstr @ 0x004AC190..0x004AC1A7 =====
unsigned __int8 *__cdecl _mbsstr(const unsigned __int8 *Str, const unsigned __int8 *Substr)
{
  return _mbsstr_l(Str, Substr, 0);
}

// ===== _strchr @ 0x004AC1C0..0x004AC27E =====
char *__cdecl strchr(const char *Str, int Val)
{
  const char *v2; // edx
  char v3; // cl
  int v4; // ecx
  int v5; // esi
  int v6; // eax
  unsigned int v7; // eax
  unsigned int v9; // eax
  unsigned int v10; // eax

  v2 = Str;
  if ( ((unsigned __int8)Str & 3) != 0 )
  {
    while ( 1 )
    {
      v3 = *v2++;
      if ( v3 == (_BYTE)Val )
        return (char *)(v2 - 1);
      if ( !v3 )
        return 0;
      if ( ((unsigned __int8)v2 & 3) == 0 )
        goto main_loop;
    }
  }
  else
  {
    while ( 1 )
    {
main_loop:
      while ( 1 )
      {
        v4 = (((unsigned __int8)Val << 8) | (unsigned __int8)Val | ((((unsigned __int8)Val << 8) | (unsigned __int8)Val) << 16)) ^ *(_DWORD *)v2;
        v5 = *(_DWORD *)v2 + 2130640639;
        v6 = v5 ^ ~*(_DWORD *)v2;
        v2 += 4;
        if ( (((v4 + 2130640639) ^ ~v4) & 0x81010100) != 0 )
          break;
        v7 = v6 & 0x81010100;
        if ( v7 && ((v7 & 0x1010100) != 0 || (v5 & 0x80000000) == 0) )
          return 0;
      }
      v9 = *((_DWORD *)v2 - 1);
      if ( (_BYTE)v9 == (_BYTE)Val )
        break;
      if ( !(_BYTE)v9 )
        return 0;
      if ( BYTE1(v9) == (_BYTE)Val )
        return (char *)(v2 - 3);
      if ( !BYTE1(v9) )
        return 0;
      v10 = HIWORD(v9);
      if ( (_BYTE)v10 == (_BYTE)Val )
        return (char *)(v2 - 2);
      if ( !(_BYTE)v10 )
        return 0;
      if ( BYTE1(v10) == (_BYTE)Val )
        return (char *)(v2 - 1);
      if ( !BYTE1(v10) )
        return 0;
    }
    return (char *)(v2 - 4);
  }
}

// ===== _atol @ 0x004AC27E..0x004AC294 =====
int __cdecl atol(const char *String)
{
  return strtol(String, 0, 10);
}

// ===== _atoi @ 0x004AC294..0x004AC29F =====
int __cdecl atoi(const char *String)
{
  return atol(String);
}

// ===== __purecall @ 0x004AC29F..0x004AC2C9 =====
void __noreturn _purecall()
{
  void (*v0)(void); // eax

  v0 = (void (*)(void))DecodePointer(dword_50A538);
  if ( v0 )
    v0();
  _NMSG_WRITE(25);
  _set_abort_behavior(0, 1u);
  abort();
}

// ===== ??0exception@std@@QAE@ABQBDH@Z @ 0x004AC2C9..0x004AC2E6 =====
const char **__thiscall std::exception::exception(const char **this, const char *const *a2, int a3)
{
  const char **result; // eax

  result = this;
  *this = (const char *)&std::exception::`vftable';
  this[1] = *a2;
  *((_BYTE *)this + 8) = 0;
  return result;
}

// ===== ?what@exception@@UBEPBDXZ @ 0x004AC2E6..0x004AC2F3 =====
const char *__thiscall exception::what(exception *this)
{
  const char *result; // eax

  result = (const char *)*((_DWORD *)this + 1);
  if ( !result )
    return "Unknown exception";
  return result;
}

// ===== ?_Copy_str@exception@std@@AAEXPBD@Z @ 0x004AC2F3..0x004AC333 =====
void __thiscall std::exception::_Copy_str(std::exception *this, char *Str)
{
  size_t v3; // esi
  char *v4; // eax

  if ( Str )
  {
    v3 = strlen(Str) + 1;
    v4 = (char *)malloc(v3);
    *((_DWORD *)this + 1) = v4;
    if ( v4 )
    {
      strcpy_s(v4, v3, Str);
      *((_BYTE *)this + 8) = 1;
    }
  }
}

// ===== ?_Tidy@exception@std@@AAEXXZ @ 0x004AC333..0x004AC351 =====
void __thiscall std::exception::_Tidy(void **this)
{
  if ( *((_BYTE *)this + 8) )
    free(this[1]);
  this[1] = 0;
  *((_BYTE *)this + 8) = 0;
}

// ===== ??0exception@std@@QAE@ABQBD@Z @ 0x004AC351..0x004AC378 =====
std::exception *__thiscall std::exception::exception(std::exception *this, char **a2)
{
  *((_DWORD *)this + 1) = 0;
  *(_DWORD *)this = &std::exception::`vftable';
  *((_BYTE *)this + 8) = 0;
  std::exception::_Copy_str(this, *a2);
  return this;
}

// ===== ??4exception@std@@QAEAAV01@ABV01@@Z @ 0x004AC378..0x004AC3AD =====
void **__thiscall std::exception::operator=(void **this, int a2)
{
  if ( this != (void **)a2 )
  {
    std::exception::_Tidy(this);
    if ( *(_BYTE *)(a2 + 8) )
      std::exception::_Copy_str((std::exception *)this, *(char **)(a2 + 4));
    else
      this[1] = *(void **)(a2 + 4);
  }
  return this;
}

// ===== sub_4AC3AD @ 0x004AC3AD..0x004AC3B8 =====
void __thiscall sub_4AC3AD(void **this)
{
  *this = &std::exception::`vftable';
  std::exception::_Tidy(this);
}

// ===== sub_4AC3B8 @ 0x004AC3B8..0x004AC3DF =====
void **__thiscall sub_4AC3B8(void **this, char a2)
{
  *this = &std::exception::`vftable';
  std::exception::_Tidy(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== ??0exception@std@@QAE@ABV01@@Z @ 0x004AC3DF..0x004AC404 =====
std::exception *__thiscall std::exception::exception(std::exception *this, const struct exception *a2)
{
  *((_DWORD *)this + 1) = 0;
  *(_DWORD *)this = &std::exception::`vftable';
  *((_BYTE *)this + 8) = 0;
  std::exception::operator=((void **)this, (int)a2);
  return this;
}

// ===== _memcpy @ 0x004AC410..0x004AC771 =====
void *__cdecl memcpy(void *a1, const void *Src, size_t Size)
{
  const __m128i *v3; // esi
  size_t v4; // ecx
  __m128i *v5; // edi
  size_t v6; // ecx
  void *result; // eax
  char *v8; // esi
  char *v9; // edi
  size_t v10; // ecx
  int v11; // eax
  size_t v12; // edx
  unsigned int v13; // ecx
  size_t j; // edx
  __m128i si128; // xmm1
  __m128i v16; // xmm2
  __m128i v17; // xmm3
  __m128i v18; // xmm5
  __m128i v19; // xmm6
  __m128i v20; // xmm7
  unsigned int k; // edx
  unsigned int v22; // ecx
  char v23; // al
  unsigned int m; // ecx
  int v25; // ecx
  unsigned int v26; // eax
  int v27; // ecx
  unsigned int i; // eax
  size_t v29; // [esp-8h] [ebp-10h]

  v3 = (const __m128i *)Src;
  v4 = Size;
  v5 = (__m128i *)a1;
  if ( a1 > Src && a1 < (char *)Src + Size )
  {
    v8 = (char *)Src + Size - 4;
    v9 = (char *)a1 + Size - 4;
    if ( ((unsigned __int8)v9 & 3) == 0 )
    {
      v10 = Size >> 2;
      if ( Size >> 2 >= 8 )
      {
        while ( v10 )
        {
          *(_DWORD *)v9 = *(_DWORD *)v8;
          v8 -= 4;
          v9 -= 4;
          --v10;
        }
        switch ( Size & 3 )
        {
          case 0u:
            goto TrailDown0;
          case 1u:
            goto TrailDown1;
          case 2u:
            goto TrailDown2;
          case 3u:
            goto TrailDown3;
        }
      }
      switch ( Size & 3 )
      {
        case 0u:
          goto TrailDown0;
        case 1u:
          goto TrailDown1;
        case 2u:
          goto TrailDown2;
        case 3u:
          goto TrailDown3;
      }
    }
    switch ( Size )
    {
      case 0u:
TrailDown0:
        result = a1;
        break;
      case 1u:
TrailDown1:
        v9[3] = v8[3];
        result = a1;
        break;
      case 2u:
TrailDown2:
        v9[3] = v8[3];
        v9[2] = v8[2];
        result = a1;
        break;
      case 3u:
TrailDown3:
        v9[3] = v8[3];
        v9[2] = v8[2];
        v9[1] = v8[1];
        result = a1;
        break;
      default:
        __asm { jmp     dword ptr ds:(ByteCopyDown+4)[eax*4] }
        return result;
    }
  }
  else if ( Size >= 0x80
         && dword_567C00
         && (v3 = (const __m128i *)Src, v5 = (__m128i *)a1, ((unsigned __int8)a1 & 0xF) == ((unsigned __int8)Src & 0xF)) )
  {
    v11 = (unsigned __int8)Src & 0xF;
    if ( ((unsigned __int8)Src & 0xF) != 0 )
    {
      v29 = Size - (16 - v11);
      v26 = 16 - v11;
      v27 = v26 & 3;
      if ( (v26 & 3) != 0 )
      {
        do
        {
          v5->m128i_i8[0] = v3->m128i_i8[0];
          v3 = (const __m128i *)((char *)v3 + 1);
          v5 = (__m128i *)((char *)v5 + 1);
          --v27;
        }
        while ( v27 );
      }
      for ( i = v26 >> 2; i; --i )
      {
        v5->m128i_i32[0] = v3->m128i_i32[0];
        v3 = (const __m128i *)((char *)v3 + 4);
        v5 = (__m128i *)((char *)v5 + 4);
      }
      v4 = v29;
    }
    v12 = v4;
    v13 = v4 & 0x7F;
    for ( j = v12 >> 7; j; --j )
    {
      si128 = _mm_load_si128(v3 + 1);
      v16 = _mm_load_si128(v3 + 2);
      v17 = _mm_load_si128(v3 + 3);
      *v5 = _mm_load_si128(v3);
      v5[1] = si128;
      v5[2] = v16;
      v5[3] = v17;
      v18 = _mm_load_si128(v3 + 5);
      v19 = _mm_load_si128(v3 + 6);
      v20 = _mm_load_si128(v3 + 7);
      v5[4] = _mm_load_si128(v3 + 4);
      v5[5] = v18;
      v5[6] = v19;
      v5[7] = v20;
      v3 += 8;
      v5 += 8;
    }
    if ( v13 )
    {
      for ( k = v13 >> 4; k; --k )
        *v5++ = _mm_load_si128(v3++);
      v22 = v13 & 0xF;
      if ( v22 )
      {
        v23 = v22;
        for ( m = v22 >> 2; m; --m )
        {
          v5->m128i_i32[0] = v3->m128i_i32[0];
          v3 = (const __m128i *)((char *)v3 + 4);
          v5 = (__m128i *)((char *)v5 + 4);
        }
        v25 = v23 & 3;
        if ( (v23 & 3) != 0 )
        {
          do
          {
            v5->m128i_i8[0] = v3->m128i_i8[0];
            v3 = (const __m128i *)((char *)v3 + 1);
            v5 = (__m128i *)((char *)v5 + 1);
            --v25;
          }
          while ( v25 );
        }
      }
    }
    return a1;
  }
  else
  {
    if ( ((unsigned __int8)v5 & 3) != 0 )
    {
      if ( Size >= 4 )
        __asm { jmp     dword ptr ds:(CopyUnwindUp+4)[eax*4] }
      __asm { jmp     dword ptr ds:TrailUp0[ecx*4]; jumptable 004AC469 case 0 }
    }
    v6 = Size >> 2;
    switch ( v6 )
    {
      case 0u:
        goto UnwindUp0;
      case 1u:
        goto UnwindUp1;
      case 2u:
        goto UnwindUp2;
      case 3u:
        goto UnwindUp3;
      case 4u:
        goto UnwindUp4;
      case 5u:
        goto UnwindUp5;
      case 6u:
        goto UnwindUp6;
      case 7u:
        *((_DWORD *)&v5[-1] + v6 - 3) = *((_DWORD *)&v3[-1] + v6 - 3);
UnwindUp6:
        *((_DWORD *)&v5[-1] + v6 - 2) = *((_DWORD *)&v3[-1] + v6 - 2);
UnwindUp5:
        *((_DWORD *)&v5[-1] + v6 - 1) = *((_DWORD *)&v3[-1] + v6 - 1);
UnwindUp4:
        v5[-1].m128i_i32[v6] = v3[-1].m128i_i32[v6];
UnwindUp3:
        v5->m128i_i32[v6 - 3] = v3->m128i_i32[v6 - 3];
UnwindUp2:
        v5->m128i_i32[v6 - 2] = v3->m128i_i32[v6 - 2];
UnwindUp1:
        v5->m128i_i32[v6 - 1] = v3->m128i_i32[v6 - 1];
        v3 = (const __m128i *)((char *)v3 + 4 * v6);
        v5 = (__m128i *)((char *)v5 + 4 * v6);
UnwindUp0:
        switch ( Size & 3 )
        {
          case 0u:
            goto TrailUp0;
          case 1u:
            goto TrailUp1;
          case 2u:
            goto TrailUp2;
          case 3u:
            goto TrailUp3;
        }
      default:
        qmemcpy(v5, v3, 4 * v6);
        v3 = (const __m128i *)((char *)v3 + 4 * v6);
        v5 = (__m128i *)((char *)v5 + 4 * v6);
        switch ( Size & 3 )
        {
          case 0u:
TrailUp0:
            result = a1;
            break;
          case 1u:
TrailUp1:
            v5->m128i_i8[0] = v3->m128i_i8[0];
            result = a1;
            break;
          case 2u:
TrailUp2:
            v5->m128i_i8[0] = v3->m128i_i8[0];
            v5->m128i_i8[1] = v3->m128i_i8[1];
            result = a1;
            break;
          case 3u:
TrailUp3:
            v5->m128i_i8[0] = v3->m128i_i8[0];
            v5->m128i_i8[1] = v3->m128i_i8[1];
            v5->m128i_i8[2] = v3->m128i_i8[2];
            result = a1;
            break;
        }
        break;
    }
  }
  return result;
}

// ===== _strrchr @ 0x004AC780..0x004AC7AD =====
char *__cdecl strrchr(const char *Str, int Ch)
{
  unsigned int v2; // ecx
  const char *v3; // edi
  bool v4; // zf
  char *v5; // edi

  v2 = strlen(Str) + 1;
  v3 = &Str[v2 - 1];
  do
  {
    if ( !v2 )
      break;
    v4 = *v3-- == (unsigned __int8)Ch;
    --v2;
  }
  while ( !v4 );
  v5 = (char *)(v3 + 1);
  if ( *v5 == (_BYTE)Ch )
    return v5;
  else
    return 0;
}

// ===== __endthreadex @ 0x004AC7AD..0x004AC7CB =====
void __cdecl __noreturn _endthreadex(unsigned int ReturnCode)
{
  void *v1; // eax

  v1 = (void *)_getptd_noexit();
  if ( v1 )
    _freeptd(v1);
  ExitThread(ReturnCode);
}

// ===== __callthreadstartex @ 0x004AC7CC..0x004AC80D =====
void __noreturn _callthreadstartex()
{
  int v0; // eax
  unsigned int v1; // eax
  int v2; // [esp+0h] [ebp-2Ch]

  v0 = _getptd();
  v1 = (*(int (__stdcall **)(_DWORD, int))(v0 + 84))(*(_DWORD *)(v0 + 88), v2);
  _endthreadex(v1);
}

// ===== __threadstartex@4 @ 0x004AC80D..0x004AC871 =====
void __stdcall __noreturn _threadstartex(DWORD *lpThreadParameter)
{
  int v1; // eax
  _DWORD *v2; // eax
  int v3; // eax
  DWORD LastError; // eax

  __set_flsgetvalue();
  v1 = sub_4AEBDB();
  v2 = (_DWORD *)__fls_getvalue(v1);
  if ( v2 )
  {
    v2[21] = lpThreadParameter[21];
    v2[22] = lpThreadParameter[22];
    v2[1] = lpThreadParameter[1];
    _freefls(lpThreadParameter);
  }
  else
  {
    v3 = sub_4AEBDB();
    if ( !__fls_setvalue(v3, lpThreadParameter) )
    {
      LastError = GetLastError();
      ExitThread(LastError);
    }
    *lpThreadParameter = GetCurrentThreadId();
  }
  _callthreadstartex();
}

// ===== __beginthreadex @ 0x004AC872..0x004AC912 =====
uintptr_t __cdecl _beginthreadex(
        void *Security,
        unsigned int StackSize,
        _beginthreadex_proc_type StartAddress,
        void *ArgList,
        unsigned int InitFlag,
        unsigned int *ThrdAddr)
{
  _beginthreadex_proc_type v6; // edi
  DWORD LastError; // ebx
  uintptr_t result; // eax
  _DWORD *v9; // esi
  int v10; // eax
  void *v11; // eax
  DWORD *p_StartAddress; // eax

  v6 = StartAddress;
  LastError = 0;
  if ( !StartAddress )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 0;
  }
  __set_flsgetvalue();
  v9 = (_DWORD *)_calloc_crt(1, 532);
  if ( !v9 )
    goto $error_return$27731;
  v10 = _getptd();
  _initptd(v9, *(_DWORD *)(v10 + 108));
  v11 = ArgList;
  v9[1] = -1;
  v9[22] = v11;
  p_StartAddress = ThrdAddr;
  v9[21] = v6;
  if ( !p_StartAddress )
    p_StartAddress = (DWORD *)&StartAddress;
  result = (uintptr_t)CreateThread(
                        (LPSECURITY_ATTRIBUTES)Security,
                        StackSize,
                        (LPTHREAD_START_ROUTINE)_threadstartex,
                        v9,
                        InitFlag,
                        p_StartAddress);
  if ( !result )
  {
    LastError = GetLastError();
$error_return$27731:
    free(v9);
    if ( LastError )
      _dosmaperr(LastError);
    return 0;
  }
  return result;
}

// ===== __mbslwr_s_l @ 0x004AC912..0x004ACA18 =====
errno_t __cdecl _mbslwr_s_l(unsigned __int8 *String, size_t SizeInBytes, _locale_t Locale)
{
  unsigned __int8 *v3; // ebx
  errno_t result; // eax
  size_t v5; // eax
  const CHAR *v6; // edi
  _BYTE *v7; // esi
  int v8; // eax
  char v9; // dl
  int v10; // eax
  char v11; // ch
  CHAR v12; // al
  bool v13; // zf
  _BYTE v14[4]; // [esp+8h] [ebp-10h] BYREF
  int v15; // [esp+Ch] [ebp-Ch]
  int v16; // [esp+10h] [ebp-8h]
  char v17; // [esp+14h] [ebp-4h]

  v3 = String;
  if ( !String )
  {
    if ( !SizeInBytes )
      goto LABEL_3;
LABEL_6:
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  if ( !SizeInBytes )
    goto LABEL_6;
LABEL_3:
  if ( !String )
    return 0;
  v5 = strnlen((const char *)String, SizeInBytes);
  if ( v5 >= SizeInBytes )
  {
    *v3 = 0;
    goto LABEL_6;
  }
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v14, (struct localeinfo_struct *)Locale);
  v6 = (const CHAR *)v3;
  v7 = v3;
  if ( !*v3 )
  {
LABEL_20:
    v13 = v17 == 0;
    *v7 = 0;
    if ( !v13 )
      *(_DWORD *)(v16 + 112) &= ~2u;
    return 0;
  }
  while ( 1 )
  {
    v8 = v15 + *(unsigned __int8 *)v6;
    v9 = *(_BYTE *)(v8 + 29);
    if ( (v9 & 4) == 0 )
    {
      if ( (v9 & 0x10) != 0 )
        v12 = *(_BYTE *)(v8 + 285);
      else
        v12 = *v6;
      *v7 = v12;
      goto LABEL_18;
    }
    v10 = __crtLCMapStringA(
            (struct localeinfo_struct *)v14,
            *(_DWORD *)(v15 + 12),
            0x100u,
            v6,
            2,
            (LPWSTR)&String,
            2,
            *(_DWORD *)(v15 + 4),
            1);
    if ( !v10 )
      break;
    v11 = BYTE1(String);
    *v7++ = (_BYTE)String;
    ++v6;
    if ( v10 > 1 )
    {
      *v7 = v11;
LABEL_18:
      ++v7;
    }
    if ( !*++v6 )
      goto LABEL_20;
  }
  *_errno() = 42;
  *v3 = 0;
  result = *_errno();
  if ( v17 )
    *(_DWORD *)(v16 + 112) &= ~2u;
  return result;
}

// ===== __mbslwr @ 0x004ACA18..0x004ACA3D =====
unsigned __int8 *__cdecl _mbslwr(unsigned __int8 *String)
{
  return _mbslwr_s_l(String, -(String != 0), 0) == 0 ? String : 0;
}

// ===== __mbschr_l @ 0x004ACA3D..0x004ACAED =====
unsigned __int8 *__cdecl _mbschr_l(const unsigned __int8 *Str, unsigned int C, _locale_t Locale)
{
  unsigned __int8 *result; // eax
  unsigned __int16 v4; // cx
  _BYTE v5[4]; // [esp+4h] [ebp-10h] BYREF
  int v6; // [esp+8h] [ebp-Ch]
  int v7; // [esp+Ch] [ebp-8h]
  char v8; // [esp+10h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v5, (struct localeinfo_struct *)Locale);
  result = (unsigned __int8 *)Str;
  if ( Str )
  {
    if ( *(_DWORD *)(v6 + 8) )
    {
      while ( 1 )
      {
        v4 = *result;
        if ( !*result )
          break;
        if ( (*(_BYTE *)((unsigned __int8)v4 + v6 + 29) & 4) != 0 )
        {
          if ( !*++result )
            goto LABEL_17;
          if ( C == (*result | (v4 << 8)) )
          {
            --result;
            goto LABEL_15;
          }
        }
        else if ( C == *result )
        {
          break;
        }
        ++result;
      }
      if ( C == *result )
        goto LABEL_15;
LABEL_17:
      if ( v8 )
        *(_DWORD *)(v7 + 112) &= ~2u;
      return 0;
    }
    else
    {
      result = (unsigned __int8 *)strchr((const char *)Str, C);
LABEL_15:
      if ( v8 )
        *(_DWORD *)(v7 + 112) &= ~2u;
    }
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    if ( v8 )
      *(_DWORD *)(v7 + 112) &= ~2u;
    return 0;
  }
  return result;
}

// ===== __mbschr @ 0x004ACAED..0x004ACB04 =====
unsigned __int8 *__cdecl _mbschr(const unsigned __int8 *Str, unsigned int C)
{
  return _mbschr_l(Str, C, 0);
}

// ===== _strncmp @ 0x004ACB04..0x004ACBC4 =====
int __cdecl strncmp(const char *Str1, const char *Str2, size_t MaxCount)
{
  const char *v4; // ecx
  const char *v5; // eax
  char v6; // dl
  char v7; // dl
  char v8; // dl
  char v9; // dl
  int v10; // eax
  int v11; // ecx
  size_t i; // esi
  size_t v13; // [esp+4h] [ebp-4h]

  v13 = 0;
  if ( !MaxCount )
    return 0;
  if ( MaxCount <= 4 )
  {
    v4 = Str2;
    v5 = Str1;
LABEL_20:
    for ( i = v13; ; ++i )
    {
      if ( i >= MaxCount )
        return 0;
      if ( !*v5 || *v5 != *v4 )
        break;
      ++v5;
      ++v4;
    }
    v10 = *(unsigned __int8 *)v5;
    v11 = *(unsigned __int8 *)v4;
  }
  else
  {
    v4 = Str2;
    v5 = Str1;
    while ( 1 )
    {
      v6 = *v5;
      v5 += 4;
      v4 += 4;
      if ( !v6 || v6 != *(v4 - 4) )
        break;
      v7 = *(v5 - 3);
      if ( !v7 || v7 != *(v4 - 3) )
      {
        v10 = *((unsigned __int8 *)v5 - 3);
        v11 = *((unsigned __int8 *)v4 - 3);
        return v10 - v11;
      }
      v8 = *(v5 - 2);
      if ( !v8 || v8 != *(v4 - 2) )
      {
        v10 = *((unsigned __int8 *)v5 - 2);
        v11 = *((unsigned __int8 *)v4 - 2);
        return v10 - v11;
      }
      v9 = *(v5 - 1);
      if ( !v9 || v9 != *(v4 - 1) )
      {
        v10 = *((unsigned __int8 *)v5 - 1);
        v11 = *((unsigned __int8 *)v4 - 1);
        return v10 - v11;
      }
      v13 += 4;
      if ( v13 >= MaxCount - 4 )
        goto LABEL_20;
    }
    v10 = *((unsigned __int8 *)v5 - 4);
    v11 = *((unsigned __int8 *)v4 - 4);
  }
  return v10 - v11;
}

// ===== __splitpath_helper @ 0x004ACBC4..0x004ACDBC =====
int __cdecl _splitpath_helper(
        unsigned __int8 *Src,
        unsigned __int8 *Dst,
        unsigned int a3,
        unsigned __int8 *a4,
        unsigned int a5,
        unsigned __int8 *a6,
        unsigned int a7,
        unsigned __int8 *a8,
        unsigned int a9)
{
  unsigned __int8 *v9; // edi
  unsigned __int8 *v10; // esi
  int v11; // eax
  unsigned __int8 *v12; // esi
  const unsigned __int8 *v13; // ebx
  unsigned __int8 v14; // al
  size_t v15; // esi
  size_t v16; // esi
  int *v18; // eax
  int v19; // [esp+Ch] [ebp-4h]

  v9 = 0;
  v19 = 0;
  if ( !Src )
    goto $error_einval$29424;
  if ( Dst )
  {
    if ( !a3 )
      goto $error_einval$29424;
  }
  else if ( a3 )
  {
    goto $error_einval$29424;
  }
  if ( a4 )
  {
    if ( !a5 )
      goto $error_einval$29424;
  }
  else if ( a5 )
  {
    goto $error_einval$29424;
  }
  if ( a6 )
  {
    if ( !a7 )
      goto $error_einval$29424;
  }
  else if ( a7 )
  {
    goto $error_einval$29424;
  }
  if ( a8 )
  {
    if ( a9 )
      goto LABEL_16;
$error_einval$29424:
    v19 = 1;
    goto $error_erange$29454;
  }
  if ( a9 )
    goto $error_einval$29424;
LABEL_16:
  v10 = Src;
  v11 = 1;
  do
  {
    if ( !*v10 )
      break;
    --v11;
    ++v10;
  }
  while ( v11 );
  if ( *v10 == 58 )
  {
    if ( Dst )
    {
      if ( a3 < 3 )
        goto $error_erange$29454;
      _mbsnbcpy_s(Dst, 0xFFFFFFFF, Src, 2u);
      v9 = 0;
    }
    Src = v10 + 1;
  }
  else if ( Dst )
  {
    *Dst = 0;
  }
  v12 = Src;
  v13 = 0;
  if ( !*Src )
    goto LABEL_42;
  do
  {
    if ( _ismbblead((char)*v12) )
    {
      ++v12;
    }
    else
    {
      v14 = *v12;
      if ( *v12 == 47 || v14 == 92 )
      {
        v9 = v12 + 1;
      }
      else if ( v14 == 46 )
      {
        v13 = v12;
      }
    }
    ++v12;
  }
  while ( *v12 );
  if ( v9 )
  {
    if ( a4 )
    {
      if ( a5 <= v9 - Src )
        goto $error_erange$29454;
      _mbsnbcpy_s(a4, 0xFFFFFFFF, Src, v9 - Src);
    }
    Src = v9;
  }
  else
  {
LABEL_42:
    if ( a4 )
      *a4 = 0;
  }
  if ( !v13 || v13 < Src )
  {
    if ( a6 )
    {
      v16 = v12 - Src;
      if ( a7 <= v16 )
        goto $error_erange$29454;
      _mbsnbcpy_s(a6, 0xFFFFFFFF, Src, v16);
    }
    if ( a8 )
      *a8 = 0;
    return 0;
  }
  if ( a6 )
  {
    if ( a7 <= v13 - Src )
      goto $error_erange$29454;
    _mbsnbcpy_s(a6, 0xFFFFFFFF, Src, v13 - Src);
  }
  if ( !a8 )
    return 0;
  v15 = v12 - v13;
  if ( a9 > v15 )
  {
    _mbsnbcpy_s(a8, 0xFFFFFFFF, v13, v15);
    return 0;
  }
$error_erange$29454:
  if ( Dst && a3 )
    *Dst = 0;
  if ( a4 && a5 )
    *a4 = 0;
  if ( a6 && a7 )
    *a6 = 0;
  if ( a8 && a9 )
    *a8 = 0;
  v18 = _errno();
  if ( !Src || v19 )
  {
    *v18 = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  else
  {
    *v18 = 34;
    return 34;
  }
}

// ===== __splitpath @ 0x004ACDBC..0x004ACE08 =====
void __cdecl _splitpath(const char *FullPath, char *Drive, char *Dir, char *Filename, char *Ext)
{
  _splitpath_helper(
    (unsigned __int8 *)FullPath,
    (unsigned __int8 *)Drive,
    Drive != 0 ? 3 : 0,
    (unsigned __int8 *)Dir,
    Dir != 0 ? 0x100 : 0,
    (unsigned __int8 *)Filename,
    Filename != 0 ? 0x100 : 0,
    (unsigned __int8 *)Ext,
    Ext != 0 ? 0x100 : 0);
}

// ===== _fast_error_exit @ 0x004ACE08..0x004ACE2D =====
void __cdecl __noreturn fast_error_exit(int a1)
{
  if ( dword_509B68 == 1 )
    _FF_MSGBANNER();
  _NMSG_WRITE(a1);
  __crtExitProcess(0xFFu);
}

// ===== ___tmainCRTStartup @ 0x004ACE31..0x004ACF9E =====
int __tmainCRTStartup()
{
  int v0; // eax
  CHAR *v1; // eax
  int wShowWindow; // ecx
  int v3; // eax
  _STARTUPINFOW StartupInfo; // [esp+10h] [ebp-68h] BYREF
  int v6; // [esp+58h] [ebp-20h]
  BOOL v7; // [esp+5Ch] [ebp-1Ch]
  CPPEH_RECORD ms_exc; // [esp+60h] [ebp-18h]

  GetStartupInfoW(&StartupInfo);
  if ( !dword_567C24 )
    HeapSetInformation(0, HeapEnableTerminationOnCorruption, 0, 0);
  v7 = LOWORD(MEMORY[0x400000].unused) == 23117
    && *(_DWORD *)(MEMORY[0x40003C] + 0x400000) == 17744
    && *(_WORD *)(MEMORY[0x40003C] + 4194328) == 267
    && *(_DWORD *)(MEMORY[0x40003C] + 4194420) > 0xEu
    && *(_DWORD *)(MEMORY[0x40003C] + 4194536) != 0;
  if ( !_heap_init() )
    fast_error_exit(28);
  if ( !_mtinit() )
    fast_error_exit(16);
  sub_4B1F7B();
  ms_exc.registration.TryLevel = 0;
  if ( _ioinit() < 0 )
    _amsg_exit(27);
  dword_567C20 = (int)GetCommandLineA();
  dword_509B60 = (char *)__crtGetEnvironmentStringsA();
  if ( _setargv() < 0 )
    _amsg_exit(8);
  if ( _setenvp() < 0 )
    _amsg_exit(9);
  v0 = _cinit(1);
  if ( v0 )
    _amsg_exit(v0);
  v1 = (CHAR *)_wincmdln();
  if ( (StartupInfo.dwFlags & 1) != 0 )
    wShowWindow = StartupInfo.wShowWindow;
  else
    wShowWindow = 10;
  v3 = WinMain((HINSTANCE)0x400000, 0, v1, wShowWindow);
  v6 = v3;
  if ( !v7 )
    exit(v3);
  _cexit();
  return v6;
}

// ===== start @ 0x004ACF9E..0x004ACFA8 =====
int start()
{
  __security_init_cookie();
  return __tmainCRTStartup();
}

// ===== ?_JumpToContinuation@@YGXPAXPAUEHRegistrationNode@@@Z @ 0x004ACFA8..0x004ACFD5 =====
void __stdcall _JumpToContinuation(
        void (__stdcall *a1)(void *, struct EHRegistrationNode *),
        struct EHRegistrationNode *a2)
{
  a1(a1, a2);
}

// ===== sub_4ACFDA @ 0x004ACFDA..0x004ACFE1 =====
// ?_CallMemberFunction0@@YGXPAX0@Z
// doubtful name
// positive sp value has been detected, the output may be wrong!
int __cdecl sub_4ACFDA(int a1, int a2)
{
  __int32 v3; // [esp-8h] [ebp-8h]
  _UNKNOWN *retaddr; // [esp+0h] [ebp+0h] BYREF

  return ((int (*)(void))_InterlockedExchange((volatile __int32 *)&retaddr, v3))();
}

// ===== ?_UnwindNestedFrames@@YGXPAUEHRegistrationNode@@PAUEHExceptionRecord@@@Z @ 0x004ACFE1..0x004AD035 =====
void __userpurge _UnwindNestedFrames(
        struct _EXCEPTION_REGISTRATION_RECORD **a1@<ebx>,
        PVOID TargetFrame,
        PEXCEPTION_RECORD ExceptionRecord)
{
  RtlUnwind(TargetFrame, &_ReturnPoint_27112, ExceptionRecord, 0);
  ExceptionRecord->ExceptionFlags &= ~2u;
  *a1 = NtCurrentTeb()->NtTib.ExceptionList;
}

// ===== ___CxxFrameHandler3 @ 0x004AD035..0x004AD06B =====
int __usercall __CxxFrameHandler3@<eax>(
        struct _s_FuncInfo *a1@<eax>,
        struct EHExceptionRecord *a2,
        struct EHRegistrationNode *a3,
        struct _CONTEXT *a4,
        void *a5)
{
  return __InternalCxxFrameHandler(a2, a3, a4, a5, a1, 0, 0, 0);
}

// ===== ?CatchGuardHandler@@YA?AW4_EXCEPTION_DISPOSITION@@PAUEHExceptionRecord@@PAUCatchGuardRN@@PAX2@Z @ 0x004AD06B..0x004AD09E =====
int __cdecl CatchGuardHandler(struct EHExceptionRecord *a1, struct EHRegistrationNode *a2, struct _CONTEXT *a3)
{
  sub_4AB245((void *)((unsigned int)a2 ^ a2->state));
  return __InternalCxxFrameHandler(
           a1,
           (struct EHRegistrationNode *)a2[1].frameHandler,
           a3,
           0,
           (struct _s_FuncInfo *)a2[1].pNext,
           a2[1].state,
           a2,
           0);
}

// ===== ?_CallSETranslator@@YAHPAUEHExceptionRecord@@PAUEHRegistrationNode@@PAX2PBU_s_FuncInfo@@H1@Z @ 0x004AD09E..0x004AD175 =====
int __cdecl _CallSETranslator(
        struct EHExceptionRecord *a1,
        struct EHRegistrationNode *a2,
        void *a3,
        void *a4,
        const struct _s_FuncInfo *a5,
        int a6,
        struct EHRegistrationNode *a7)
{
  int v8; // [esp+0h] [ebp-3Ch] BYREF
  int v9; // [esp+4h] [ebp-38h]
  _DWORD v10[2]; // [esp+8h] [ebp-34h] BYREF
  void (__cdecl *v11)(unsigned int, _DWORD *); // [esp+10h] [ebp-2Ch]
  _DWORD v12[9]; // [esp+14h] [ebp-28h] BYREF
  int v13; // [esp+38h] [ebp-4h]
  int savedregs; // [esp+3Ch] [ebp+0h] BYREF

  if ( a1 == (struct EHExceptionRecord *)291 )
  {
    a2->pNext = (EHRegistrationNode *)&_ExceptionContinuation_27230;
    return 1;
  }
  else
  {
    v12[1] = TranslatorGuardHandler;
    v12[2] = (unsigned int)v12 ^ dword_4FB734;
    v12[3] = a5;
    v12[4] = a2;
    v12[5] = a6;
    v12[6] = a7;
    v13 = 0;
    v12[7] = &v8;
    v12[8] = &savedregs;
    v12[0] = NtCurrentTeb()->NtTib.ExceptionList;
    v9 = 1;
    v10[0] = a1;
    v10[1] = a3;
    v11 = *(void (__cdecl **)(unsigned int, _DWORD *))(_getptd() + 128);
    v11(a1->ExceptionCode, v10);
    v9 = 0;
    if ( v13 )
      *(_DWORD *)v12[0] = NtCurrentTeb()->NtTib.ExceptionList->Next;
    return v9;
  }
}

// ===== ?TranslatorGuardHandler@@YA?AW4_EXCEPTION_DISPOSITION@@PAUEHExceptionRecord@@PAUTranslatorGuardRN@@PAX2@Z @ 0x004AD175..0x004AD214 =====
int __usercall TranslatorGuardHandler@<eax>(
        struct _EXCEPTION_REGISTRATION_RECORD **a1@<ebx>,
        struct EHExceptionRecord *ExceptionRecord,
        unsigned int TargetFrame,
        struct _CONTEXT *a4)
{
  int (*v5)(void); // [esp+4h] [ebp-4h] BYREF

  sub_4AB245((void *)(TargetFrame ^ *(_DWORD *)(TargetFrame + 8)));
  if ( (ExceptionRecord->ExceptionFlags & 0x66) != 0 )
  {
    *(_DWORD *)(TargetFrame + 36) = 1;
    return 1;
  }
  else
  {
    __InternalCxxFrameHandler(
      ExceptionRecord,
      *(struct EHRegistrationNode **)(TargetFrame + 16),
      a4,
      0,
      *(struct _s_FuncInfo **)(TargetFrame + 12),
      *(_DWORD *)(TargetFrame + 20),
      *(struct EHRegistrationNode **)(TargetFrame + 24),
      1u);
    if ( !*(_DWORD *)(TargetFrame + 36) )
      _UnwindNestedFrames(a1, (PVOID)TargetFrame, (PEXCEPTION_RECORD)ExceptionRecord);
    _CallSETranslator((struct EHExceptionRecord *)0x123, (struct EHRegistrationNode *)&v5, 0, 0, 0, 0, 0);
    return v5();
  }
}

// ===== ?_GetRangeOfTrysToCheck@@YAPBU_s_TryBlockMapEntry@@PBU_s_FuncInfo@@HHPAI1@Z @ 0x004AD214..0x004AD287 =====
TryBlockMapEntry *__cdecl _GetRangeOfTrysToCheck(
        const struct _s_FuncInfo *a1,
        int a2,
        int a3,
        unsigned int *a4,
        unsigned int *a5)
{
  unsigned int nTryBlocks; // esi
  unsigned int v7; // ebx
  TryBlockMapEntry *v8; // eax
  unsigned int v9; // esi
  TryBlockMapEntry *pTryBlockMap; // [esp+Ch] [ebp-4h]
  const struct _s_FuncInfo *v12; // [esp+18h] [ebp+8h]

  nTryBlocks = a1->nTryBlocks;
  pTryBlockMap = a1->pTryBlockMap;
  v7 = nTryBlocks;
LABEL_8:
  v12 = (const struct _s_FuncInfo *)nTryBlocks;
  while ( a2 >= 0 )
  {
    if ( nTryBlocks == -1 )
      _inconsistency();
    v8 = &pTryBlockMap[--nTryBlocks];
    if ( v8->tryHigh < a3 && a3 <= v8->catchHigh || nTryBlocks == -1 )
    {
      --a2;
      v7 = (unsigned int)v12;
      goto LABEL_8;
    }
  }
  v9 = nTryBlocks + 1;
  *a4 = v9;
  *a5 = v7;
  if ( v7 > a1->nTryBlocks || v9 > v7 )
    _inconsistency();
  return &pTryBlockMap[v9];
}

// ===== __CreateFrameInfo @ 0x004AD287..0x004AD2B3 =====
_DWORD *__cdecl _CreateFrameInfo(_DWORD *a1, int a2)
{
  *a1 = a2;
  a1[1] = *(_DWORD *)(_getptd() + 152);
  *(_DWORD *)(_getptd() + 152) = a1;
  return a1;
}

// ===== __IsExceptionObjectToBeDestroyed @ 0x004AD2B3..0x004AD2DA =====
int __cdecl _IsExceptionObjectToBeDestroyed(int a1)
{
  _DWORD *i; // eax

  for ( i = *(_DWORD **)(_getptd() + 152); ; i = (_DWORD *)i[1] )
  {
    if ( !i )
      return 1;
    if ( *i == a1 )
      break;
  }
  return 0;
}

// ===== __FindAndUnlinkFrame @ 0x004AD2DA..0x004AD32C =====
int __cdecl _FindAndUnlinkFrame(int a1)
{
  int result; // eax

  if ( a1 == *(_DWORD *)(_getptd() + 152) )
  {
    result = _getptd();
    *(_DWORD *)(result + 152) = *(_DWORD *)(a1 + 4);
  }
  else
  {
    for ( result = *(_DWORD *)(_getptd() + 152); ; result = *(_DWORD *)(result + 4) )
    {
      if ( !*(_DWORD *)(result + 4) )
        _inconsistency();
      if ( a1 == *(_DWORD *)(result + 4) )
        break;
    }
    *(_DWORD *)(result + 4) = *(_DWORD *)(a1 + 4);
  }
  return result;
}

// ===== ?_CallCatchBlock2@@YAPAXPAUEHRegistrationNode@@PBU_s_FuncInfo@@PAXHK@Z @ 0x004AD32C..0x004AD38C =====
void *__cdecl _CallCatchBlock2(
        struct EHRegistrationNode *a1,
        const struct _s_FuncInfo *a2,
        void *a3,
        int a4,
        unsigned int a5)
{
  _DWORD v6[6]; // [esp+0h] [ebp-18h] BYREF

  v6[2] = (unsigned int)v6 ^ dword_4FB734;
  v6[3] = a2;
  v6[1] = CatchGuardHandler;
  v6[4] = a1;
  v6[5] = a4 + 1;
  v6[0] = NtCurrentTeb()->NtTib.ExceptionList;
  return (void *)_CallSettingFrame(a3, a1, a5);
}

// ===== __CxxThrowException@8 @ 0x004AD38C..0x004AD3D8 =====
void __stdcall __noreturn _CxxThrowException(void *pExceptionObject, _ThrowInfo *pThrowInfo)
{
  DWORD dwExceptionCode[8]; // [esp+8h] [ebp-20h] BYREF

  qmemcpy(dwExceptionCode, &unk_4DC3CC, sizeof(dwExceptionCode));
  dwExceptionCode[6] = (DWORD)pExceptionObject;
  dwExceptionCode[7] = (DWORD)pThrowInfo;
  if ( pThrowInfo && (pThrowInfo->attributes & 8) != 0 )
    dwExceptionCode[5] = 26820608;
  RaiseException(dwExceptionCode[0], dwExceptionCode[1], dwExceptionCode[4], &dwExceptionCode[5]);
}

// ===== __cfltcvt_init @ 0x004AD3D8..0x004AD438 =====
int (__cdecl *_cfltcvt_init())(int, char *Str, int, int, size_t Size, int)
{
  int (__cdecl *result)(int, char *, int, int, size_t, int); // eax

  result = _cfltcvt;
  off_4FC0E0[0] = (void (__noreturn *)())_cfltcvt;
  off_4FC0E4[0] = (void (__noreturn *)())_cropzeros;
  off_4FC0E8[0] = (void (__noreturn *)())_fassign;
  off_4FC0EC[0] = (void (__noreturn *)())_forcdecpt;
  off_4FC0F0[0] = (void (__noreturn *)())_positive;
  off_4FC0F4[0] = (void (__noreturn *)())_cfltcvt;
  off_4FC0F8 = _cfltcvt_l;
  off_4FC0FC[0] = (void (__noreturn *)())_fassign_l;
  off_4FC100 = _cropzeros_l;
  off_4FC104 = _forcdecpt_l;
  return result;
}

// ===== __fpmath @ 0x004AD438..0x004AD451 =====
int (__cdecl *__cdecl _fpmath(int a1))(int, char *Str, int, int, size_t Size, int)
{
  int (__cdecl *result)(int, char *, int, int, size_t, int); // eax

  result = _cfltcvt_init();
  if ( a1 )
    result = (int (__cdecl *)(int, char *, int, int, size_t, int))_setdefaultprecision();
  __asm { fnclex }
  return result;
}

// ===== __ftol2_sse @ 0x004AD460..0x004AD47C =====
int __usercall _ftol2_sse@<eax>(double a1@<st0>)
{
  if ( dword_567C00 )
    return (int)a1;
  else
    return _ftol2(a1);
}

// ===== __ftol2 @ 0x004AD496..0x004AD50B =====
unsigned int __usercall _ftol2@<eax>(double a1@<st0>)
{
  int v1; // edx
  unsigned int result; // eax
  float v3; // [esp+0h] [ebp-20h]
  int v4; // [esp+18h] [ebp-8h]

  *(float *)&v4 = a1;
  v1 = v4;
  result = (__int64)a1;
  if ( result || (v1 = (unsigned __int64)(__int64)a1 >> 32, (v1 & 0x7FFFFFFF) != 0) )
  {
    if ( v1 >= 0 )
    {
      v3 = a1 - (double)(__int64)a1;
      result -= __CFADD__(LODWORD(v3), 0x7FFFFFFF);
    }
    else
    {
      return (__PAIR64__(result, -(float)(a1 - (double)(__int64)a1)) + 0x7FFFFFFF) >> 32;
    }
  }
  return result;
}

// ===== ??_L@YGXPAXIHP6EX0@Z1@Z @ 0x004AD50B..0x004AD570 =====
void __stdcall `eh vector constructor iterator'(
        char *a1,
        unsigned int a2,
        int a3,
        void (__thiscall *a4)(void *),
        void (__thiscall *a5)(void *))
{
  int i; // [esp+14h] [ebp-1Ch]

  for ( i = 0; i < a3; ++i )
  {
    a4(a1);
    a1 += a2;
  }
}

// ===== ?__ArrayUnwind@@YGXPAXIHP6EX0@Z@Z @ 0x004AD570..0x004AD5CE =====
void __stdcall __ArrayUnwind(char *a1, unsigned int a2, int a3, void (*a4)(void))
{
  while ( --a3 >= 0 )
  {
    a1 -= a2;
    a4();
  }
}

// ===== ??_M@YGXPAXIHP6EX0@Z@Z @ 0x004AD5CE..0x004AD631 =====
void __stdcall `eh vector destructor iterator'(char *a1, unsigned int a2, int a3, void (__thiscall *a4)(void *))
{
  char *i; // [esp+34h] [ebp+8h]

  for ( i = &a1[a3 * a2]; --a3 >= 0; a4(i) )
    i -= a2;
}

// ===== _memcpy_0 @ 0x004AD640..0x004AD9A1 =====
void *__cdecl memcpy_0(void *a1, const void *Src, size_t Size)
{
  _BYTE *v3; // esi
  _BYTE *v4; // edi
  size_t v5; // ecx
  void *result; // eax
  char *v7; // esi
  char *v8; // edi
  size_t v9; // ecx

  v3 = Src;
  v4 = a1;
  if ( a1 > Src && a1 < (char *)Src + Size )
  {
    v7 = (char *)Src + Size - 4;
    v8 = (char *)a1 + Size - 4;
    if ( ((unsigned __int8)v8 & 3) == 0 )
    {
      v9 = Size >> 2;
      if ( Size >> 2 >= 8 )
      {
        while ( v9 )
        {
          *(_DWORD *)v8 = *(_DWORD *)v7;
          v7 -= 4;
          v8 -= 4;
          --v9;
        }
        switch ( Size & 3 )
        {
          case 0u:
            goto TrailDown0_0;
          case 1u:
            goto TrailDown1_0;
          case 2u:
            goto TrailDown2_0;
          case 3u:
            goto TrailDown3_0;
        }
      }
      switch ( Size & 3 )
      {
        case 0u:
          goto TrailDown0_0;
        case 1u:
          goto TrailDown1_0;
        case 2u:
          goto TrailDown2_0;
        case 3u:
          goto TrailDown3_0;
      }
    }
    switch ( Size )
    {
      case 0u:
TrailDown0_0:
        result = a1;
        break;
      case 1u:
TrailDown1_0:
        v8[3] = v7[3];
        result = a1;
        break;
      case 2u:
TrailDown2_0:
        v8[3] = v7[3];
        v8[2] = v7[2];
        result = a1;
        break;
      case 3u:
TrailDown3_0:
        v8[3] = v7[3];
        v8[2] = v7[2];
        v8[1] = v7[1];
        result = a1;
        break;
      default:
        __asm { jmp     dword ptr ds:(ByteCopyDown_0+4)[eax*4] }
        return result;
    }
  }
  else
  {
    if ( Size >= 0x80 )
    {
      if ( dword_567C00 )
      {
        v3 = Src;
        v4 = a1;
        if ( ((unsigned __int8)a1 & 0xF) == ((unsigned __int8)Src & 0xF) )
        {
          if ( ((unsigned __int8)Src & 0xF) == 0 )
          {
            if ( Size >> 7 )
              JUMPOUT(0x4B14E1);
            JUMPOUT(0x4B153E);
          }
          JUMPOUT(0x4B1590);
        }
      }
    }
    if ( ((unsigned __int8)v4 & 3) != 0 )
    {
      if ( Size >= 4 )
        __asm { jmp     dword ptr ds:(CopyUnwindUp_0+4)[eax*4] }
      __asm { jmp     dword ptr ds:TrailUp0_0[ecx*4]; jumptable 004AD699 case 0 }
    }
    v5 = Size >> 2;
    switch ( v5 )
    {
      case 0u:
        goto UnwindUp0_0;
      case 1u:
        goto UnwindUp1_0;
      case 2u:
        goto UnwindUp2_0;
      case 3u:
        goto UnwindUp3_0;
      case 4u:
        goto UnwindUp4_0;
      case 5u:
        goto UnwindUp5_0;
      case 6u:
        goto UnwindUp6_0;
      case 7u:
        *(_DWORD *)&v4[4 * v5 - 28] = *(_DWORD *)&v3[4 * v5 - 28];
UnwindUp6_0:
        *(_DWORD *)&v4[4 * v5 - 24] = *(_DWORD *)&v3[4 * v5 - 24];
UnwindUp5_0:
        *(_DWORD *)&v4[4 * v5 - 20] = *(_DWORD *)&v3[4 * v5 - 20];
UnwindUp4_0:
        *(_DWORD *)&v4[4 * v5 - 16] = *(_DWORD *)&v3[4 * v5 - 16];
UnwindUp3_0:
        *(_DWORD *)&v4[4 * v5 - 12] = *(_DWORD *)&v3[4 * v5 - 12];
UnwindUp2_0:
        *(_DWORD *)&v4[4 * v5 - 8] = *(_DWORD *)&v3[4 * v5 - 8];
UnwindUp1_0:
        *(_DWORD *)&v4[4 * v5 - 4] = *(_DWORD *)&v3[4 * v5 - 4];
        v3 += 4 * v5;
        v4 += 4 * v5;
UnwindUp0_0:
        switch ( Size & 3 )
        {
          case 0u:
            goto TrailUp0_0;
          case 1u:
            goto TrailUp1_0;
          case 2u:
            goto TrailUp2_0;
          case 3u:
            goto TrailUp3_0;
        }
      default:
        qmemcpy(v4, v3, 4 * v5);
        v3 += 4 * v5;
        v4 += 4 * v5;
        switch ( Size & 3 )
        {
          case 0u:
TrailUp0_0:
            result = a1;
            break;
          case 1u:
TrailUp1_0:
            *v4 = *v3;
            result = a1;
            break;
          case 2u:
TrailUp2_0:
            *v4 = *v3;
            v4[1] = v3[1];
            result = a1;
            break;
          case 3u:
TrailUp3_0:
            *v4 = *v3;
            v4[1] = v3[1];
            v4[2] = v3[2];
            result = a1;
            break;
        }
        break;
    }
  }
  return result;
}

// ===== __freea @ 0x004AD9A1..0x004AD9C1 =====
void __cdecl _freea(void *Memory)
{
  if ( Memory )
  {
    if ( *((_DWORD *)Memory - 2) == 56797 )
      free((char *)Memory - 8);
  }
}

// ===== ?_strlwr_s_l_stat@@YAHPADIPAUlocaleinfo_struct@@@Z @ 0x004AD9C1..0x004ADB3F =====
errno_t __cdecl _strlwr_s_l_stat(char *Destination, rsize_t SizeInBytes, struct localeinfo_struct *a3)
{
  int *v3; // eax
  errno_t v4; // esi
  LCID v5; // ecx
  char *i; // ecx
  char v7; // al
  int v9; // eax
  int v10; // ecx
  int v11; // eax
  void *v12; // esp
  _DWORD *v13; // eax
  int v14; // [esp-4h] [ebp-1Ch]
  _DWORD v15[3]; // [esp+0h] [ebp-18h] BYREF
  int cchDest; // [esp+Ch] [ebp-Ch]
  void *Memory; // [esp+10h] [ebp-8h]

  if ( !Destination )
    goto LABEL_2;
  if ( strnlen(Destination, SizeInBytes) >= SizeInBytes )
  {
    *Destination = 0;
LABEL_2:
    v3 = _errno();
    v14 = 22;
LABEL_3:
    v4 = v14;
    *v3 = v14;
    _invalid_parameter_noinfo();
    return v4;
  }
  v5 = *(_DWORD *)(*(_DWORD *)a3 + 20);
  if ( v5 )
  {
    v9 = __crtLCMapStringA(a3, v5, 0x100u, Destination, -1, 0, 0, *(_DWORD *)(*(_DWORD *)a3 + 4), 1);
    v10 = v9;
    cchDest = v9;
    if ( !v9 )
    {
      *_errno() = 42;
      return *_errno();
    }
    if ( SizeInBytes < v9 )
    {
      *Destination = 0;
      v3 = _errno();
      v14 = 34;
      goto LABEL_3;
    }
    if ( v9 <= 0 || !(0xFFFFFFE0 / v9) )
    {
      Memory = 0;
      goto LABEL_28;
    }
    v11 = v9 + 8;
    if ( (unsigned int)(v10 + 8) > 0x400 )
    {
      v13 = malloc(v10 + 8);
      if ( v13 )
      {
        *v13 = 56797;
        goto LABEL_25;
      }
    }
    else
    {
      v12 = alloca(v11);
      v13 = v15;
      if ( v15 )
      {
        v15[0] = 52428;
LABEL_25:
        v13 += 2;
      }
    }
    v10 = cchDest;
    Memory = v13;
LABEL_28:
    if ( Memory )
    {
      if ( __crtLCMapStringA(
             a3,
             *(_DWORD *)(*(_DWORD *)a3 + 20),
             0x100u,
             Destination,
             -1,
             (LPWSTR)Memory,
             v10,
             *(_DWORD *)(*(_DWORD *)a3 + 4),
             1) )
      {
        v4 = strcpy_s(Destination, SizeInBytes, (const char *)Memory);
      }
      else
      {
        *_errno() = 42;
        v4 = 42;
      }
      _freea(Memory);
      return v4;
    }
    *_errno() = 12;
    return *_errno();
  }
  for ( i = Destination; *i; ++i )
  {
    v7 = *i;
    if ( *i >= 65 && v7 <= 90 )
      *i = v7 + 32;
  }
  return 0;
}

// ===== __strlwr_s_l @ 0x004ADB3F..0x004ADB73 =====
errno_t __cdecl _strlwr_s_l(char *String, size_t Size, _locale_t Locale)
{
  errno_t result; // eax
  _BYTE v4[8]; // [esp+0h] [ebp-10h] BYREF
  int v5; // [esp+8h] [ebp-8h]
  char v6; // [esp+Ch] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)v4, (struct localeinfo_struct *)Locale);
  result = _strlwr_s_l_stat(String, Size, (struct localeinfo_struct *)v4);
  if ( v6 )
    *(_DWORD *)(v5 + 112) &= ~2u;
  return result;
}

// ===== __strlwr @ 0x004ADB73..0x004ADBD0 =====
char *__cdecl _strlwr(char *String)
{
  char *result; // eax
  char *i; // edx
  char v3; // cl

  if ( dword_509F0C )
  {
    _strlwr_s_l(String, 0xFFFFFFFF, 0);
    return String;
  }
  else
  {
    result = String;
    if ( String )
    {
      for ( i = String; *i; ++i )
      {
        v3 = *i;
        if ( *i >= 65 && v3 <= 90 )
          *i = v3 + 32;
      }
    }
    else
    {
      *_errno() = 22;
      _invalid_parameter_noinfo();
      return 0;
    }
  }
  return result;
}

// ===== _memset @ 0x004ADBD0..0x004ADC4A =====
void *__cdecl memset(void *a1, int Val, size_t Size)
{
  size_t v3; // edx
  _BYTE *v4; // ecx
  int v5; // eax
  _BYTE *v6; // edi
  int v7; // ecx
  size_t v8; // ecx
  unsigned int v9; // ecx
  int v11; // eax
  size_t v12; // eax
  unsigned int v13; // edx
  size_t j; // eax
  unsigned int k; // eax
  unsigned int v16; // edx
  char v17; // al
  unsigned int m; // edx
  int n; // eax
  int v20; // edx
  unsigned int i; // ebx

  v3 = Size;
  v4 = a1;
  if ( !Size )
    return a1;
  LOBYTE(v5) = Val;
  if ( !(_BYTE)Val && Size >= 0x80 && dword_567C00 )
  {
    v11 = (unsigned __int8)a1 & 0xF;
    if ( ((unsigned __int8)a1 & 0xF) != 0 )
    {
      v20 = (16 - (_BYTE)v11) & 3;
      if ( ((16 - (_BYTE)v11) & 3) != 0 )
      {
        do
        {
          *v4++ = 0;
          --v20;
        }
        while ( v20 );
      }
      for ( i = (unsigned int)(16 - v11) >> 2; i; --i )
      {
        *(_DWORD *)v4 = 0;
        v4 += 4;
      }
      v3 = Size - (16 - v11);
    }
    v12 = v3;
    v13 = v3 & 0x7F;
    for ( j = v12 >> 7; j; --j )
    {
      *(_OWORD *)v4 = 0LL;
      *((_OWORD *)v4 + 1) = 0LL;
      *((_OWORD *)v4 + 2) = 0LL;
      *((_OWORD *)v4 + 3) = 0LL;
      *((_OWORD *)v4 + 4) = 0LL;
      *((_OWORD *)v4 + 5) = 0LL;
      *((_OWORD *)v4 + 6) = 0LL;
      *((_OWORD *)v4 + 7) = 0LL;
      v4 += 128;
    }
    if ( v13 )
    {
      for ( k = v13 >> 4; k; --k )
      {
        *(_OWORD *)v4 = 0LL;
        v4 += 16;
      }
      v16 = v13 & 0xF;
      if ( v16 )
      {
        v17 = v16;
        for ( m = v16 >> 2; m; --m )
        {
          *(_DWORD *)v4 = 0;
          v4 += 4;
        }
        for ( n = v17 & 3; n; --n )
          *v4++ = 0;
      }
    }
    return a1;
  }
  else
  {
    v6 = a1;
    if ( Size < 4 )
      goto LABEL_33;
    v7 = -(int)a1 & 3;
    if ( v7 )
    {
      v3 = Size - v7;
      do
      {
        *v6++ = Val;
        --v7;
      }
      while ( v7 );
    }
    v5 = 16843009 * (unsigned __int8)Val;
    v8 = v3;
    v3 &= 3u;
    v9 = v8 >> 2;
    if ( !v9 || (memset32(v6, v5, v9), v6 += 4 * v9, v3) )
    {
LABEL_33:
      do
      {
        *v6++ = v5;
        --v3;
      }
      while ( v3 );
    }
    return a1;
  }
}

// ===== _pow @ 0x004ADC50..0x004ADC8F =====
double __cdecl pow(double X, double Y)
{
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return _pow_pentium4(X, Y);
  }
  _fload_withFB();
  return start_0(X, Y);
}

// ===== __CIpow @ 0x004ADC90..0x004ADCE4 =====
double __usercall _CIpow@<st0>(double x@<st0>, double y@<st1>)
{
  int v2; // eax
  bool v3; // zf
  unsigned __int64 v5; // st6
  char v6; // [esp+Ch] [ebp-8h]

  if ( !dword_567C18 )
    goto __CIpow_default;
  v2 = _mm_getcsr() & 0x7F80;
  v3 = v2 == 8064;
  if ( v2 == 8064 )
    v3 = (v6 & 0x7F) == 127;
  if ( v3 )
  {
    _CIpow_pentium4();
  }
  else
  {
__CIpow_default:
    *(double *)&v5 = x;
    x = y;
    start_0(v5, HIDWORD(v5), LODWORD(y), HIDWORD(y));
  }
  return x;
}

// ===== start_0 @ 0x004ADCED..0x004ADEB2 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004ADCED

// ===== _test_whether_TOS_is_int @ 0x004ADEB2..0x004ADEDA =====
void __usercall test_whether_TOS_is_int(double a1@<st0>)
{
  _ST6 = a1;
  __asm { frndint }
  if ( _ST6 == a1 )
  {
    _ST6 = a1 * dbl_4FB750;
    __asm { frndint }
  }
}

// ===== __alloca_probe @ 0x004ADEE0..0x004ADF0B =====
void *__usercall _alloca_probe@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  unsigned int v2; // ecx
  unsigned int i; // eax
  int v5; // [esp-4h] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+0h] [ebp+0h] BYREF

  v5 = a2;
  v2 = ~((unsigned int)((unsigned int)&retaddr - (unsigned __int64)a1) >> 32) & ((unsigned int)&retaddr - a1);
  for ( i = (unsigned int)&v5 & 0xFFFFF000; v2 < i; i -= 4096 )
    ;
  return retaddr;
}

// ===== ___report_gsfailure @ 0x004ADF0B..0x004AE011 =====
void __cdecl __noreturn __report_gsfailure()
{
  int v0; // eax
  int v1; // edx
  int v2; // ecx
  int v3; // ebx
  int v4; // edi
  int v5; // esi
  unsigned int v6; // kr00_4
  HANDLE CurrentProcess; // eax
  int vars0; // [esp+328h] [ebp+0h]
  int retaddr; // [esp+32Ch] [ebp+4h]
  char v10; // [esp+330h] [ebp+8h] BYREF

  dword_509C78 = v0;
  dword_509C74 = v2;
  dword_509C70 = v1;
  dword_509C6C = v3;
  dword_509C68 = v5;
  dword_509C64 = v4;
  word_509C90 = __SS__;
  word_509C84 = __CS__;
  word_509C60 = __DS__;
  word_509C5C = __ES__;
  word_509C58 = __FS__;
  word_509C54 = __GS__;
  v6 = __readeflags();
  dword_509C88 = v6;
  dword_509C7C = vars0;
  dword_509C80 = retaddr;
  dword_509C8C = (int)&v10;
  dword_509BC8 = 65537;
  dword_509B7C = retaddr;
  dword_509B70 = -1073740791;
  dword_509B74 = 1;
  dword_509BC0 = IsDebuggerPresent();
  sub_4B4D01(1);
  SetUnhandledExceptionFilter(0);
  UnhandledExceptionFilter((struct _EXCEPTION_POINTERS *)&ExceptionInfo);
  if ( !dword_509BC0 )
    sub_4B4D01(1);
  CurrentProcess = GetCurrentProcess();
  TerminateProcess(CurrentProcess, 0xC0000409);
}

// ===== ___libm_error_support @ 0x004AE011..0x004AE2A8 =====
int (*__cdecl __libm_error_support(double *a1, double *a2, double *a3, int a4))()
{
  int (*result)(); // eax
  double *v5; // esi
  double v6; // st7
  double *v7; // ecx
  double v8; // st7
  int v9; // [esp+0h] [ebp-28h] BYREF
  const char *v10; // [esp+4h] [ebp-24h]
  double v11; // [esp+8h] [ebp-20h]
  double v12; // [esp+10h] [ebp-18h]
  double v13; // [esp+18h] [ebp-10h]
  double v14; // [esp+20h] [ebp-8h]

  v14 = 0.0;
  if ( dword_509E94 )
    result = (int (*)())DecodePointer(dword_567C1C);
  else
    result = sub_4B4D09;
  if ( a4 > 166 )
  {
    switch ( a4 )
    {
      case 1000:
        v10 = "log";
        goto LABEL_38;
      case 1001:
        v10 = "log10";
        goto LABEL_38;
      case 1002:
        v10 = "exp";
        goto LABEL_38;
      case 1003:
        v10 = "atan";
        goto LABEL_38;
      case 1004:
        v10 = "ceil";
        goto LABEL_38;
      case 1005:
        v10 = "floor";
        goto LABEL_38;
      case 1006:
        goto LABEL_39;
      case 1007:
        v10 = "modf";
        goto LABEL_38;
      case 1008:
        goto LABEL_36;
      case 1009:
        goto LABEL_35;
      case 1010:
        v10 = (const char *)&unk_4DC468;
        goto LABEL_54;
      case 1011:
        v10 = (const char *)&unk_4DC464;
        goto LABEL_54;
      case 1012:
        v10 = (const char *)&unk_4DC460;
LABEL_54:
        v5 = a3;
        v8 = *a1 * v14;
        *a3 = v8;
        v11 = *a1;
        v12 = *a2;
        goto LABEL_55;
      default:
        return result;
    }
  }
  if ( a4 == 166 )
  {
    v9 = 3;
    v10 = "exp10";
LABEL_17:
    v5 = a3;
    v11 = *a1;
    v12 = *a2;
    v13 = *a3;
    result = (int (*)())((int (__cdecl *)(int *))result)(&v9);
    if ( !result )
    {
      result = (int (*)())_errno();
      *(_DWORD *)result = 34;
    }
    goto LABEL_57;
  }
  if ( a4 > 25 )
  {
    switch ( a4 )
    {
      case 26:
        result = (int (*)())a3;
        *a3 = 1.0;
        return result;
      case 27:
        v9 = 2;
LABEL_16:
        v10 = "pow";
        goto LABEL_17;
      case 28:
LABEL_39:
        v10 = "pow";
        break;
      case 29:
        v10 = "pow";
LABEL_38:
        v7 = a1;
        v5 = a3;
        *a3 = *a1;
LABEL_24:
        v11 = *v7;
        v12 = *a2;
        v8 = *v5;
LABEL_55:
        v13 = v8;
        v9 = 1;
        result = (int (*)())((int (__cdecl *)(int *))result)(&v9);
        if ( !result )
        {
          result = (int (*)())_errno();
          *(_DWORD *)result = 33;
        }
        goto LABEL_57;
      case 58:
LABEL_36:
        v10 = "acos";
        break;
      case 61:
LABEL_35:
        v10 = "asin";
        break;
      default:
        return result;
    }
LABEL_23:
    v7 = a1;
    v5 = a3;
    goto LABEL_24;
  }
  switch ( a4 )
  {
    case 25:
      v10 = "pow";
      goto LABEL_20;
    case 2:
      v9 = 2;
      v10 = "log";
      goto LABEL_17;
    case 3:
      v10 = "log";
      goto LABEL_23;
    case 8:
      v9 = 2;
      v10 = "log10";
      goto LABEL_17;
    case 9:
      v10 = "log10";
      goto LABEL_23;
    case 14:
      v9 = 3;
      v10 = "exp";
      goto LABEL_17;
  }
  if ( a4 != 15 )
  {
    if ( a4 != 24 )
      return result;
    v9 = 3;
    goto LABEL_16;
  }
  v10 = "exp";
LABEL_20:
  v5 = a3;
  v11 = *a1;
  v12 = *a2;
  v6 = *a3;
  v9 = 4;
  v13 = v6;
  result = (int (*)())((int (__cdecl *)(int *))result)(&v9);
LABEL_57:
  *v5 = v13;
  return result;
}

// ===== sub_4AE2DD @ 0x004AE2DD..0x004AE2ED =====
int sub_4AE2DD()
{
  dword_567C18 = IsProcessorFeaturePresent(0xAu);
  return 0;
}

// ===== __ceil_default @ 0x004AE2ED..0x004AE3C5 =====
double __cdecl _ceil_default(double a1)
{
  double result; // st7
  int v2; // ebx
  int v3; // eax

  result = a1;
  v2 = _ctrlfp(dword_4FB760, 0xFFFF);
  if ( (HIWORD(a1) & 0x7FF0) == 0x7FF0 )
  {
    v3 = _sptype(LODWORD(a1), HIDWORD(a1));
    if ( v3 > 0 )
    {
      if ( v3 <= 2 )
      {
        _ctrlfp(v2, 0xFFFF);
        return a1;
      }
      if ( v3 == 3 )
      {
        result = a1;
        _handle_qnan1(12, a1, v2);
        return result;
      }
    }
    return _except1(
             8,
             12,
             LODWORD(a1),
             HIDWORD(a1),
             COERCE_UNSIGNED_INT64(a1 + 1.0),
             HIDWORD(COERCE_UNSIGNED_INT64(a1 + 1.0)),
             v2);
  }
  else
  {
    _frnd(a1);
    _ctrlfp(v2, 0xFFFF);
  }
  return result;
}

// ===== _free @ 0x004AE3C5..0x004AE3FF =====
void __cdecl free(void *Block)
{
  int *v1; // esi
  DWORD LastError; // eax

  if ( Block )
  {
    if ( !HeapFree(hHeap, 0, Block) )
    {
      v1 = _errno();
      LastError = GetLastError();
      *v1 = _get_errno_from_oserr(LastError);
    }
  }
}

// ===== ?_Type_info_dtor@type_info@@CAXPAV1@@Z @ 0x004AE3FF..0x004AE46F =====
void __cdecl type_info::_Type_info_dtor(struct type_info *a1)
{
  int v1; // ecx
  void *v2; // eax
  _DWORD *v3; // edx

  _lock(14);
  v1 = *((_DWORD *)a1 + 1);
  if ( v1 )
  {
    v2 = Block;
    v3 = &unk_509E98;
    while ( Block )
    {
      if ( *(_DWORD *)Block == v1 )
      {
        v3[1] = *((_DWORD *)Block + 1);
        free(v2);
        break;
      }
      v3 = Block;
    }
    free(*((void **)a1 + 1));
    *((_DWORD *)a1 + 1) = 0;
  }
  _unlock(14);
}

// ===== _strcmp @ 0x004AE470..0x004AE4F8 =====
int __cdecl strcmp(const char *Str1, const char *Str2)
{
  const char *v2; // edx
  const char *v3; // ecx
  unsigned int v4; // eax
  bool v5; // cf
  unsigned int v6; // eax
  __int16 v8; // ax

  v2 = Str1;
  v3 = Str2;
  if ( ((unsigned __int8)Str1 & 3) == 0 )
  {
dodwords:
    while ( 1 )
    {
      v4 = *(_DWORD *)v2;
      v5 = (unsigned __int8)*(_DWORD *)v2 < (unsigned int)*v3;
      if ( (unsigned __int8)*(_DWORD *)v2 != *v3 )
        break;
      if ( !(_BYTE)v4 )
        return 0;
      v5 = BYTE1(v4) < (unsigned int)v3[1];
      if ( BYTE1(v4) != v3[1] )
        break;
      if ( !BYTE1(v4) )
        return 0;
      v6 = HIWORD(v4);
      v5 = (unsigned __int8)v6 < (unsigned int)v3[2];
      if ( (_BYTE)v6 != v3[2] )
        break;
      if ( !(_BYTE)v6 )
        return 0;
      v5 = BYTE1(v6) < (unsigned int)v3[3];
      if ( BYTE1(v6) != v3[3] )
        break;
      v3 += 4;
      v2 += 4;
      if ( !BYTE1(v6) )
        return 0;
    }
    return -2 * v5 + 1;
  }
  if ( ((unsigned __int8)Str1 & 1) != 0 )
  {
    v2 = Str1 + 1;
    v5 = *Str1 < (unsigned int)*Str2;
    if ( *Str1 != *Str2 )
      return -2 * v5 + 1;
    v3 = Str2 + 1;
    if ( !*Str1 )
      return 0;
    if ( ((unsigned __int8)v2 & 2) == 0 )
      goto dodwords;
  }
  v8 = *(_WORD *)v2;
  v2 += 2;
  v5 = (unsigned __int8)v8 < (unsigned int)*v3;
  if ( (_BYTE)v8 != *v3 )
    return -2 * v5 + 1;
  if ( !(_BYTE)v8 )
    return 0;
  v5 = HIBYTE(v8) < (unsigned int)v3[1];
  if ( HIBYTE(v8) == v3[1] )
  {
    if ( HIBYTE(v8) )
    {
      v3 += 2;
      goto dodwords;
    }
    return 0;
  }
  return -2 * v5 + 1;
}

// ===== _malloc @ 0x004AE4F8..0x004AE58C =====
void *__cdecl malloc(size_t Size)
{
  SIZE_T v1; // eax
  void *v2; // edi

  if ( Size > 0xFFFFFFE0 )
  {
    _callnewh(Size);
    *_errno() = 12;
    return 0;
  }
  else
  {
    while ( 1 )
    {
      if ( !hHeap )
      {
        _FF_MSGBANNER();
        _NMSG_WRITE(30);
        __crtExitProcess(0xFFu);
      }
      v1 = Size ? Size : 1;
      v2 = HeapAlloc(hHeap, 0, v1);
      if ( v2 )
        return v2;
      if ( !dword_50A7DC )
      {
        *_errno() = 12;
LABEL_12:
        *_errno() = 12;
        return v2;
      }
      if ( !_callnewh(Size) )
        goto LABEL_12;
    }
  }
}

// ===== sub_4AE58C @ 0x004AE58C..0x004AE59B =====
void *__cdecl sub_4AE58C(void *a1)
{
  void *result; // eax

  result = a1;
  dword_509EA0 = a1;
  return result;
}

// ===== __callnewh @ 0x004AE59B..0x004AE5C3 =====
int __cdecl _callnewh(size_t Size)
{
  int (__cdecl *v1)(size_t); // eax

  v1 = (int (__cdecl *)(size_t))DecodePointer(dword_509EA0);
  return v1 && v1(Size);
}

// ===== __malloc_crt @ 0x004AE5C3..0x004AE608 =====
void *__cdecl _malloc_crt(size_t Size)
{
  DWORD v1; // esi
  void *v2; // edi
  int v3; // eax

  v1 = 0;
  do
  {
    v2 = malloc(Size);
    if ( v2 || !dword_509EA4 )
      break;
    Sleep(v1);
    v3 = v1 + 1000;
    if ( v1 + 1000 > dword_509EA4 )
      v3 = -1;
    v1 = v3;
  }
  while ( v3 != -1 );
  return v2;
}

// ===== __calloc_crt @ 0x004AE608..0x004AE654 =====
int __cdecl _calloc_crt(int a1, int a2)
{
  DWORD v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = 0;
  do
  {
    v3 = _calloc_impl(a1, a2, 0);
    if ( v3 || !dword_509EA4 )
      break;
    Sleep(v2);
    v4 = v2 + 1000;
    if ( v2 + 1000 > dword_509EA4 )
      v4 = -1;
    v2 = v4;
  }
  while ( v4 != -1 );
  return v3;
}

// ===== __realloc_crt @ 0x004AE654..0x004AE6A2 =====
void *__cdecl _realloc_crt(void *Block, size_t Size)
{
  DWORD v2; // esi
  void *v3; // edi
  int v4; // eax

  v2 = 0;
  do
  {
    v3 = realloc(Block, Size);
    if ( v3 || !Size || !dword_509EA4 )
      break;
    Sleep(v2);
    v4 = v2 + 1000;
    if ( v2 + 1000 > dword_509EA4 )
      v4 = -1;
    v2 = v4;
  }
  while ( v4 != -1 );
  return v3;
}

// ===== __msize @ 0x004AE6A2..0x004AE6D5 =====
size_t __cdecl _msize(void *Block)
{
  if ( Block )
    return HeapSize(hHeap, 0, Block);
  *_errno() = 22;
  _invalid_parameter_noinfo();
  return -1;
}

// ===== ___crtCorExitProcess @ 0x004AE6D5..0x004AE700 =====
HMODULE __cdecl __crtCorExitProcess(int a1)
{
  HMODULE result; // eax

  result = GetModuleHandleW(L"mscoree.dll");
  if ( result )
  {
    result = (HMODULE)GetProcAddress(result, "CorExitProcess");
    if ( result )
      return (HMODULE)((int (__stdcall *)(int))result)(a1);
  }
  return result;
}

// ===== ___crtExitProcess @ 0x004AE700..0x004AE717 =====
void __cdecl __noreturn __crtExitProcess(UINT uExitCode)
{
  __crtCorExitProcess(uExitCode);
  ExitProcess(uExitCode);
}

// ===== __lockexit @ 0x004AE718..0x004AE721 =====
int _lockexit()
{
  return _lock(8);
}

// ===== __unlockexit @ 0x004AE721..0x004AE72A =====
int _unlockexit()
{
  return _unlock(8);
}

// ===== __init_pointers @ 0x004AE72A..0x004AE75D =====
int _init_pointers()
{
  void *v0; // esi

  v0 = (void *)_encoded_null();
  sub_4AE58C(v0);
  sub_4AF2A3(v0);
  sub_4B1453(v0);
  sub_4B5E06(v0);
  _initp_misc_winsig(v0);
  return _initp_eh_hooks(v0);
}

// ===== __initterm_e @ 0x004AE75D..0x004AE781 =====
int __cdecl _initterm_e(_PIFV *First, _PIFV *Last)
{
  int result; // eax

  result = 0;
  while ( First < Last && !result )
  {
    if ( *First )
      result = (*First)();
    ++First;
  }
  return result;
}

// ===== __cinit @ 0x004AE781..0x004AE818 =====
int __cdecl _cinit(int a1)
{
  int result; // eax
  void (**v2)(void); // edi

  if ( _fpmath && _IsNonwritableInCurrentImage(&off_4DC3EC) )
    _fpmath(a1);
  _initp_misc_cfltcvt_tab();
  result = _initterm_e((_PIFV *)&First, (_PIFV *)&Last);
  if ( !result )
  {
    atexit(sub_4B1FA1);
    v2 = (void (**)(void))&unk_4DB564;
    if ( &unk_4DB564 < (_UNKNOWN *)&dword_4DB578 )
    {
      do
      {
        if ( *v2 )
          (*v2)();
        ++v2;
      }
      while ( v2 < &dword_4DB578 );
    }
    if ( dword_567C14 )
    {
      if ( _IsNonwritableInCurrentImage(&dword_567C14) )
        dword_567C14(0, 2, 0);
    }
    return 0;
  }
  return result;
}

// ===== _doexit @ 0x004AE818..0x004AE958 =====
char __cdecl doexit(UINT uExitCode, int a2, int a3)
{
  int (*v3)(void); // eax
  int (*v4)(void); // ebx
  PVOID *v5; // edi
  void (*v6)(void); // ebx
  int (*v7)(void); // ebx
  int (*v9)(void); // [esp+10h] [ebp-30h]
  int (*v10)(void); // [esp+18h] [ebp-28h]
  int (*v11)(void); // [esp+1Ch] [ebp-24h]
  int (**j)(void); // [esp+20h] [ebp-20h]
  int (**i)(void); // [esp+24h] [ebp-1Ch]

  _lock(8);
  LOBYTE(v3) = 1;
  if ( dword_509ED8 != 1 )
  {
    dword_509ED4 = 1;
    LOBYTE(v3) = a3;
    byte_509ED0 = a3;
    if ( !a2 )
    {
      v3 = (int (*)(void))DecodePointer(Ptr);
      v4 = v3;
      v9 = v3;
      if ( v3 )
      {
        v3 = (int (*)(void))DecodePointer(dword_567C08);
        v5 = (PVOID *)v3;
        v11 = v4;
        v10 = v3;
        while ( --v5 >= (PVOID *)v4 )
        {
          v3 = (int (*)(void))_encoded_null();
          if ( *v5 != v3 )
          {
            if ( v5 < (PVOID *)v4 )
              break;
            v6 = (void (*)(void))DecodePointer(*v5);
            *v5 = (PVOID)_encoded_null();
            v6();
            v7 = (int (*)(void))DecodePointer(Ptr);
            v3 = (int (*)(void))DecodePointer(dword_567C08);
            if ( v11 != v7 || v10 != v3 )
            {
              v11 = v7;
              v9 = v7;
              v10 = v3;
              v5 = (PVOID *)v3;
            }
            v4 = v9;
          }
        }
      }
      for ( i = (int (**)(void))&unk_4DB59C; i < &dword_4DB5A8; ++i )
      {
        v3 = *i;
        if ( *i )
          LOBYTE(v3) = v3();
      }
    }
    for ( j = (int (**)(void))&unk_4DB5AC; j < dword_4DB5B0; ++j )
    {
      v3 = *j;
      if ( *j )
        LOBYTE(v3) = v3();
    }
  }
  if ( a3 )
    LOBYTE(v3) = _unlock(8);
  if ( !a3 )
  {
    dword_509ED8 = 1;
    _unlock(8);
    __crtExitProcess(uExitCode);
  }
  return (char)v3;
}

// ===== _exit @ 0x004AE958..0x004AE96E =====
void __cdecl __noreturn exit(int Code)
{
  doexit(Code, 0, 0);
}

// ===== __exit @ 0x004AE96E..0x004AE984 =====
void __cdecl __noreturn _exit(int Code)
{
  doexit(Code, 1, 0);
}

// ===== __cexit @ 0x004AE984..0x004AE993 =====
void __cdecl _cexit()
{
  doexit(0, 0, 1);
}

// ===== __c_exit @ 0x004AE993..0x004AE9A2 =====
void __cdecl _c_exit()
{
  doexit(0, 1, 1);
}

// ===== __amsg_exit @ 0x004AE9A2..0x004AE9BF =====
void __cdecl __noreturn _amsg_exit(int a1)
{
  _FF_MSGBANNER();
  _NMSG_WRITE(a1);
  _exit(255);
}

// ===== __SEH_prolog4 @ 0x004AE9C0..0x004AEA05 =====
_DWORD *__usercall _SEH_prolog4@<eax>(int a1@<ebp>, int a2, int a3)
{
  int v3; // eax
  void *v4; // esp
  _DWORD v6[2]; // [esp-8h] [ebp-8h] BYREF
  unsigned int retaddr; // [esp+0h] [ebp+0h]
  int v8; // [esp+4h] [ebp+4h]

  v6[1] = SEH_4B5EB0;
  v6[0] = NtCurrentTeb()->NtTib.ExceptionList;
  v3 = a3;
  a3 = a1;
  v4 = alloca(v3);
  v8 = -2;
  retaddr = (unsigned int)&a3 ^ dword_4FB734;
  return v6;
}

// ===== __SEH_epilog4 @ 0x004AEA05..0x004AEA19 =====
// positive sp value has been detected, the output may be wrong!
void _SEH_epilog4()
{
  __asm { retn }
}

// ===== SEH_4B5EB0 @ 0x004AEA20..0x004AEBAF =====
int __cdecl SEH_4B5EB0(PEXCEPTION_RECORD ExceptionRecord, _DWORD *TargetFrame, int a3)
{
  _DWORD *v3; // ebx
  int *v4; // esi
  int v5; // eax
  char *v6; // edi
  int v7; // ecx
  _DWORD *v8; // eax
  int v9; // eax
  _DWORD *v11; // eax
  _DWORD v12[2]; // [esp+Ch] [ebp-18h] BYREF
  int *v13; // [esp+14h] [ebp-10h]
  int v14; // [esp+18h] [ebp-Ch]
  _DWORD *v15; // [esp+1Ch] [ebp-8h]
  char v16; // [esp+23h] [ebp-1h]

  v3 = TargetFrame;
  v4 = (int *)(dword_4FB734 ^ TargetFrame[2]);
  v5 = *v4;
  v16 = 0;
  v14 = 1;
  v6 = (char *)(TargetFrame + 4);
  if ( v5 != -2 )
    sub_4AB245((void *)(*(_DWORD *)&v6[v5] ^ (unsigned int)&v6[v4[1]]));
  sub_4AB245((void *)(*(_DWORD *)&v6[v4[2]] ^ (unsigned int)&v6[v4[3]]));
  if ( (ExceptionRecord->ExceptionFlags & 0x66) != 0 )
  {
LABEL_25:
    if ( v3[3] == -2 )
      return v14;
    _EH4_LocalUnwind(v6, &dword_4FB734);
  }
  else
  {
    *(TargetFrame - 1) = v12;
    v3 = (_DWORD *)TargetFrame[3];
    v12[0] = ExceptionRecord;
    v12[1] = a3;
    if ( v3 == (_DWORD *)-2 )
      return v14;
    do
    {
      v7 = v4[3 * (_DWORD)v3 + 5];
      v13 = &v4[3 * (_DWORD)v3 + 4];
      v8 = (_DWORD *)*v13;
      v15 = (_DWORD *)*v13;
      if ( v7 )
      {
        v9 = _EH4_CallFilterFunc(v7, v6);
        v16 = 1;
        if ( v9 < 0 )
        {
          v14 = 0;
          goto LABEL_11;
        }
        if ( v9 > 0 )
        {
          if ( ExceptionRecord->ExceptionCode == -529697949
            && __DestructExceptionObject
            && _IsNonwritableInCurrentImage(&off_4DD408) )
          {
            __DestructExceptionObject(ExceptionRecord, 1);
          }
          _EH4_GlobalUnwind2(TargetFrame, ExceptionRecord);
          v11 = TargetFrame;
          if ( (_DWORD *)TargetFrame[3] != v3 )
          {
            _EH4_LocalUnwind(v6, &dword_4FB734);
            v11 = TargetFrame;
          }
          v11[3] = v15;
          if ( *v4 != -2 )
            sub_4AB245((void *)(*(_DWORD *)&v6[*v4] ^ (unsigned int)&v6[v4[1]]));
          sub_4AB245((void *)(*(_DWORD *)&v6[v4[2]] ^ (unsigned int)&v6[v4[3]]));
          _EH4_TransferToHandler(v13[2], v6);
          goto LABEL_25;
        }
        v8 = v15;
      }
      v3 = v8;
    }
    while ( v8 != (_DWORD *)-2 );
    if ( !v16 )
      return v14;
  }
LABEL_11:
  if ( *v4 != -2 )
    sub_4AB245((void *)(*(_DWORD *)&v6[*v4] ^ (unsigned int)&v6[v4[1]]));
  sub_4AB245((void *)(*(_DWORD *)&v6[v4[2]] ^ (unsigned int)&v6[v4[3]]));
  return v14;
}

// ===== __encoded_null @ 0x004AEBAF..0x004AEBB8 =====
PVOID _encoded_null()
{
  return EncodePointer(0);
}

// ===== ___crtTlsAlloc@4 @ 0x004AEBB8..0x004AEBC1 =====
// attributes: thunk
DWORD __stdcall __crtTlsAlloc(int a1)
{
  return TlsAlloc();
}

// ===== ___fls_getvalue@4 @ 0x004AEBC1..0x004AEBDB =====
int __stdcall __fls_getvalue(int a1)
{
  int (__stdcall *Value)(int); // eax

  Value = (int (__stdcall *)(int))TlsGetValue(dwTlsIndex);
  return Value(a1);
}

// ===== sub_4AEBDB @ 0x004AEBDB..0x004AEBE1 =====
int sub_4AEBDB()
{
  return dword_4FB770;
}

// ===== ___set_flsgetvalue @ 0x004AEBE1..0x004AEC15 =====
void *__set_flsgetvalue()
{
  void *Value; // esi

  Value = TlsGetValue(dwTlsIndex);
  if ( !Value )
  {
    Value = DecodePointer(lpTlsValue);
    TlsSetValue(dwTlsIndex, Value);
  }
  return Value;
}

// ===== ___fls_setvalue@8 @ 0x004AEC15..0x004AEC32 =====
int __stdcall __fls_setvalue(int a1, int a2)
{
  int (__stdcall *v2)(int, int); // eax

  v2 = (int (__stdcall *)(int, int))DecodePointer(dword_509EE4);
  return v2(a1, a2);
}

// ===== __mtterm @ 0x004AEC32..0x004AEC6F =====
int _mtterm()
{
  void (__stdcall *v0)(int); // eax
  int v2; // [esp-4h] [ebp-4h]

  if ( dword_4FB770 != -1 )
  {
    v2 = dword_4FB770;
    v0 = (void (__stdcall *)(int))DecodePointer(dword_509EE8);
    v0(v2);
    dword_4FB770 = -1;
  }
  if ( dwTlsIndex != -1 )
  {
    TlsFree(dwTlsIndex);
    dwTlsIndex = -1;
  }
  return _mtdeletelocks();
}

// ===== __initptd @ 0x004AEC6F..0x004AED23 =====
int __cdecl _initptd(int a1, int a2)
{
  int savedregs; // [esp+28h] [ebp+0h]

  GetModuleHandleW(L"KERNEL32.DLL");
  *(_DWORD *)(a1 + 92) = &unk_4DC528;
  *(_DWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 20) = 1;
  *(_DWORD *)(a1 + 112) = 1;
  *(_BYTE *)(a1 + 200) = 67;
  *(_BYTE *)(a1 + 331) = 67;
  *(_DWORD *)(a1 + 104) = &unk_4FB8F0;
  _lock(13);
  InterlockedIncrement(*(volatile LONG **)(a1 + 104));
  _unlock(13);
  _lock(12);
  *(_DWORD *)(a1 + 108) = a2;
  if ( !a2 )
    *(_DWORD *)(a1 + 108) = off_4FC058;
  __addlocaleref(*(volatile LONG **)(a1 + 108));
  savedregs = 4910341;
  return _unlock(12);
}

// ===== __getptd_noexit @ 0x004AED23..0x004AED9C =====
DWORD *_getptd_noexit()
{
  DWORD LastError; // eax
  DWORD v1; // edi
  int (__stdcall *v2)(int); // eax
  DWORD *v3; // esi
  int v4; // eax
  int (__stdcall *v5)(int, int); // eax
  DWORD CurrentThreadId; // eax
  int v8; // [esp-8h] [ebp-10h]
  int v9; // [esp-4h] [ebp-Ch]
  int v10; // [esp-4h] [ebp-Ch]

  LastError = GetLastError();
  v9 = dword_4FB770;
  v1 = LastError;
  v2 = (int (__stdcall *)(int))__set_flsgetvalue();
  v3 = (DWORD *)v2(v9);
  if ( !v3 )
  {
    v4 = _calloc_crt(1, 532);
    v3 = (DWORD *)v4;
    if ( v4 )
    {
      v10 = v4;
      v8 = dword_4FB770;
      v5 = (int (__stdcall *)(int, int))DecodePointer(dword_509EE4);
      if ( v5(v8, v10) )
      {
        _initptd((int)v3, 0);
        CurrentThreadId = GetCurrentThreadId();
        v3[1] = -1;
        *v3 = CurrentThreadId;
      }
      else
      {
        free(v3);
        v3 = 0;
      }
    }
  }
  SetLastError(v1);
  return v3;
}

// ===== __getptd @ 0x004AED9C..0x004AEDB6 =====
DWORD *_getptd()
{
  DWORD *result; // eax

  result = _getptd_noexit();
  if ( !result )
    _amsg_exit(16);
  return result;
}

// ===== __freefls@4 @ 0x004AEDB6..0x004AEEE5 =====
void __stdcall _freefls(void *Block)
{
  void *v1; // edi
  volatile LONG *v2; // edi
  int savedregs; // [esp+28h] [ebp+0h]

  if ( Block )
  {
    if ( *((_DWORD *)Block + 9) )
      free(*((void **)Block + 9));
    if ( *((_DWORD *)Block + 11) )
      free(*((void **)Block + 11));
    if ( *((_DWORD *)Block + 13) )
      free(*((void **)Block + 13));
    if ( *((_DWORD *)Block + 15) )
      free(*((void **)Block + 15));
    if ( *((_DWORD *)Block + 16) )
      free(*((void **)Block + 16));
    if ( *((_DWORD *)Block + 17) )
      free(*((void **)Block + 17));
    if ( *((_DWORD *)Block + 18) )
      free(*((void **)Block + 18));
    if ( *((_UNKNOWN **)Block + 23) != &unk_4DC528 )
      free(*((void **)Block + 23));
    _lock(13);
    v1 = (void *)*((_DWORD *)Block + 26);
    if ( v1 && !InterlockedDecrement(*((volatile LONG **)Block + 26)) && v1 != &unk_4FB8F0 )
      free(v1);
    _unlock(13);
    _lock(12);
    v2 = (volatile LONG *)*((_DWORD *)Block + 27);
    if ( v2 )
    {
      __removelocaleref(*((volatile LONG **)Block + 27));
      if ( v2 != off_4FC058 && v2 != (volatile LONG *)&unk_4FBF80 && !*v2 )
        __freetlocinfo((void *)v2);
    }
    savedregs = 4910782;
    _unlock(12);
    free(Block);
  }
}

// ===== __freeptd @ 0x004AEEE5..0x004AEF53 =====
DWORD __cdecl _freeptd(void *Block)
{
  int (__stdcall *Value)(int); // eax
  void (__stdcall *v2)(int, _DWORD); // eax
  DWORD result; // eax
  int v4; // [esp-8h] [ebp-8h]
  int v5; // [esp-8h] [ebp-8h]

  if ( dword_4FB770 != -1 )
  {
    if ( !Block && TlsGetValue(dwTlsIndex) )
    {
      v4 = dword_4FB770;
      Value = (int (__stdcall *)(int))TlsGetValue(dwTlsIndex);
      Block = (void *)Value(v4);
    }
    v5 = dword_4FB770;
    v2 = (void (__stdcall *)(int, _DWORD))DecodePointer(dword_509EE4);
    v2(v5, 0);
    _freefls(Block);
  }
  result = dwTlsIndex;
  if ( dwTlsIndex != -1 )
    return TlsSetValue(dwTlsIndex, 0);
  return result;
}

// ===== __mtinit @ 0x004AEF53..0x004AF0CE =====
int _mtinit()
{
  HMODULE ModuleHandleW; // eax
  HMODULE v1; // edi
  BOOL (__stdcall *FlsFree)(DWORD); // eax
  DWORD v4; // eax
  int (__stdcall *v5)(_DWORD); // eax
  int v6; // eax
  DWORD *v7; // esi
  int (__stdcall *v8)(int, int); // eax
  DWORD CurrentThreadId; // eax
  int v10; // [esp-Ch] [ebp-10h]
  int v11; // [esp-8h] [ebp-Ch]

  ModuleHandleW = GetModuleHandleW(L"KERNEL32.DLL");
  v1 = ModuleHandleW;
  if ( !ModuleHandleW )
  {
    _mtterm();
    return 0;
  }
  FlsAlloc = (DWORD (__stdcall *)(PFLS_CALLBACK_FUNCTION))GetProcAddress(ModuleHandleW, "FlsAlloc");
  FlsGetValue = (PVOID (__stdcall *)(DWORD))GetProcAddress(v1, "FlsGetValue");
  FlsSetValue = (BOOL (__stdcall *)(DWORD, PVOID))GetProcAddress(v1, "FlsSetValue");
  FlsFree = (BOOL (__stdcall *)(DWORD))GetProcAddress(v1, "FlsFree");
  dword_509EE8 = FlsFree;
  if ( !FlsAlloc || !FlsGetValue || !FlsSetValue || !FlsFree )
  {
    FlsGetValue = TlsGetValue;
    FlsAlloc = (DWORD (__stdcall *)(PFLS_CALLBACK_FUNCTION))__crtTlsAlloc;
    FlsSetValue = TlsSetValue;
    dword_509EE8 = TlsFree;
  }
  v4 = TlsAlloc();
  dwTlsIndex = v4;
  if ( v4 != -1 && TlsSetValue(v4, FlsGetValue) )
  {
    _init_pointers();
    FlsAlloc = (DWORD (__stdcall *)(PFLS_CALLBACK_FUNCTION))EncodePointer(FlsAlloc);
    FlsGetValue = (PVOID (__stdcall *)(DWORD))EncodePointer(FlsGetValue);
    FlsSetValue = (BOOL (__stdcall *)(DWORD, PVOID))EncodePointer(FlsSetValue);
    dword_509EE8 = EncodePointer(dword_509EE8);
    if ( _mtinitlocks() )
    {
      v5 = (int (__stdcall *)(_DWORD))DecodePointer(FlsAlloc);
      dword_4FB770 = v5(_freefls);
      if ( dword_4FB770 != -1 )
      {
        v6 = _calloc_crt(1, 532);
        v7 = (DWORD *)v6;
        if ( v6 )
        {
          v11 = v6;
          v10 = dword_4FB770;
          v8 = (int (__stdcall *)(int, int))DecodePointer(FlsSetValue);
          if ( v8(v10, v11) )
          {
            _initptd((int)v7, 0);
            CurrentThreadId = GetCurrentThreadId();
            v7[1] = -1;
            *v7 = CurrentThreadId;
            return 1;
          }
        }
      }
    }
    _mtterm();
  }
  return 0;
}

// ===== __XcptFilter @ 0x004AF0CE..0x004AF218 =====
int __cdecl _XcptFilter(int a1, int a2)
{
  int result; // eax
  _DWORD *v3; // esi
  int *v4; // ecx
  int *v5; // eax
  void (__cdecl *v6)(int); // edx
  int v7; // ebx
  int v8; // ecx
  int i; // ecx
  int v10; // eax
  int v11; // edi

  result = (int)_getptd_noexit();
  v3 = (_DWORD *)result;
  if ( result )
  {
    v4 = *(int **)(result + 92);
    v5 = v4;
    do
    {
      if ( *v5 == a1 )
        break;
      v5 += 3;
    }
    while ( v5 < v4 + 36 );
    if ( v5 >= v4 + 36 || *v5 != a1 )
      v5 = 0;
    if ( v5 && (v6 = (void (__cdecl *)(int))v5[2]) != 0 )
    {
      if ( v6 == (void (__cdecl *)(int))5 )
      {
        v5[2] = 0;
        return 1;
      }
      else
      {
        if ( v6 != (void (__cdecl *)(int))1 )
        {
          v7 = v3[24];
          v3[24] = a2;
          v8 = v5[1];
          if ( v8 == 8 )
          {
            for ( i = 36; i < 144; i += 12 )
              *(_DWORD *)(i + v3[23] + 8) = 0;
            v10 = *v5;
            v11 = v3[25];
            switch ( v10 )
            {
              case -1073741682:
                v3[25] = 131;
                break;
              case -1073741680:
                v3[25] = 129;
                break;
              case -1073741679:
                v3[25] = 132;
                break;
              case -1073741677:
                v3[25] = 133;
                break;
              case -1073741683:
                v3[25] = 130;
                break;
              case -1073741681:
                v3[25] = 134;
                break;
              case -1073741678:
                v3[25] = 138;
                break;
              case -1073741131:
                v3[25] = 141;
                break;
              case -1073741132:
                v3[25] = 142;
                break;
            }
            v6(8);
            v3[25] = v11;
          }
          else
          {
            v5[2] = 0;
            v6(v8);
          }
          v3[24] = v7;
        }
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  return result;
}

// ===== __get_errno_from_oserr @ 0x004AF218..0x004AF25A =====
int __cdecl _get_errno_from_oserr(int a1)
{
  unsigned int i; // ecx

  for ( i = 0; i < 0x2D; ++i )
  {
    if ( a1 == dword_4FB778[2 * i] )
      return dword_4FB77C[2 * i];
  }
  if ( (unsigned int)(a1 - 19) > 0x11 )
    return (unsigned int)(a1 - 188) > 0xE ? 22 : 8;
  else
    return 13;
}

// ===== __errno @ 0x004AF25A..0x004AF26D =====
int *__cdecl _errno()
{
  DWORD *v0; // eax

  v0 = _getptd_noexit();
  if ( v0 )
    return (int *)(v0 + 2);
  else
    return (int *)&unk_4FB8E0;
}

// ===== ___doserrno @ 0x004AF26D..0x004AF280 =====
unsigned int *__cdecl __doserrno()
{
  DWORD *v0; // eax

  v0 = _getptd_noexit();
  if ( v0 )
    return v0 + 3;
  else
    return (unsigned int *)&unk_4FB8E4;
}

// ===== __dosmaperr @ 0x004AF280..0x004AF2A3 =====
int *__cdecl _dosmaperr(unsigned int a1)
{
  int errno_from_oserr; // esi
  int *result; // eax

  *__doserrno() = a1;
  errno_from_oserr = _get_errno_from_oserr(a1);
  result = _errno();
  *result = errno_from_oserr;
  return result;
}

// ===== sub_4AF2A3 @ 0x004AF2A3..0x004AF2B2 =====
void *__cdecl sub_4AF2A3(void *a1)
{
  void *result; // eax

  result = a1;
  dword_509EEC = a1;
  return result;
}

// ===== __call_reportfault @ 0x004AF2B2..0x004AF3DB =====
LONG __usercall _call_reportfault@<eax>(int a1@<edi>, int a2@<esi>, int a3, int a4, int a5)
{
  int v5; // ecx
  int v6; // edx
  unsigned int v7; // kr00_4
  BOOL v8; // edi
  LONG result; // eax
  struct _EXCEPTION_POINTERS ExceptionInfo; // [esp+8h] [ebp-328h] BYREF
  _DWORD v11[20]; // [esp+10h] [ebp-320h] BYREF
  _DWORD v12[35]; // [esp+60h] [ebp-2D0h] BYREF
  __int16 v13; // [esp+ECh] [ebp-244h]
  __int16 v14; // [esp+F0h] [ebp-240h]
  __int16 v15; // [esp+F4h] [ebp-23Ch]
  __int16 v16; // [esp+F8h] [ebp-238h]
  int v17; // [esp+FCh] [ebp-234h]
  int v18; // [esp+100h] [ebp-230h]
  int v19; // [esp+104h] [ebp-22Ch]
  int v20; // [esp+108h] [ebp-228h]
  int v21; // [esp+10Ch] [ebp-224h]
  _DWORD *v22; // [esp+110h] [ebp-220h]
  int v23; // [esp+114h] [ebp-21Ch]
  void *v24; // [esp+118h] [ebp-218h]
  __int16 v25; // [esp+11Ch] [ebp-214h]
  unsigned int v26; // [esp+120h] [ebp-210h]
  void **v27; // [esp+124h] [ebp-20Ch]
  __int16 v28; // [esp+128h] [ebp-208h]
  int savedregs; // [esp+330h] [ebp+0h]
  void *retaddr; // [esp+334h] [ebp+4h] BYREF

  if ( a3 != -1 )
    sub_4B4D01(a3);
  memset(&v11[1], 0, 0x4Cu);
  ExceptionInfo.ExceptionRecord = (PEXCEPTION_RECORD)v11;
  ExceptionInfo.ContextRecord = (PCONTEXT)v12;
  v22 = v12;
  v21 = v5;
  v20 = v6;
  v19 = a3;
  v18 = a2;
  v17 = a1;
  v28 = __SS__;
  v25 = __CS__;
  v16 = __DS__;
  v15 = __ES__;
  v14 = __FS__;
  v13 = __GS__;
  v7 = __readeflags();
  v26 = v7;
  v27 = &retaddr;
  v12[0] = 65537;
  v24 = retaddr;
  v23 = savedregs;
  v11[0] = a4;
  v11[1] = a5;
  v11[3] = retaddr;
  v8 = IsDebuggerPresent();
  SetUnhandledExceptionFilter(0);
  result = UnhandledExceptionFilter(&ExceptionInfo);
  if ( !result && !v8 && a3 != -1 )
    return sub_4B4D01(a3);
  return result;
}

// ===== __invoke_watson @ 0x004AF3DB..0x004AF400 =====
void __cdecl __noreturn _invoke_watson(
        const wchar_t *Expression,
        const wchar_t *FunctionName,
        const wchar_t *FileName,
        unsigned int LineNo,
        uintptr_t Reserved)
{
  int v5; // edi
  HANDLE CurrentProcess; // eax

  _call_reportfault(v5, -1073740777, 2, -1073740777, 1);
  CurrentProcess = GetCurrentProcess();
  TerminateProcess(CurrentProcess, 0xC0000417);
}

// ===== __invalid_parameter @ 0x004AF400..0x004AF42C =====
int __cdecl _invalid_parameter(
        wchar_t *Expression,
        wchar_t *FunctionName,
        wchar_t *FileName,
        unsigned int LineNo,
        uintptr_t Reserved)
{
  int (*v5)(void); // eax

  v5 = (int (*)(void))DecodePointer(dword_509EEC);
  if ( !v5 )
    _invoke_watson(Expression, FunctionName, FileName, LineNo, Reserved);
  return v5();
}

// ===== __invalid_parameter_noinfo @ 0x004AF42D..0x004AF43D =====
void __cdecl _invalid_parameter_noinfo()
{
  _invalid_parameter(0, 0, 0, 0, 0);
}

// ===== __flsbuf @ 0x004AF43D..0x004AF5A1 =====
int __cdecl _flsbuf(int Ch, FILE *File)
{
  FILE *v2; // esi
  int flag; // eax
  unsigned int v5; // eax
  char *base; // eax
  char *ptr; // edi
  signed int v8; // edi
  _BYTE *v9; // eax
  __int64 v10; // rax
  int v11; // [esp+4h] [ebp-4h]

  v2 = File;
  File = (FILE *)_fileno(File);
  flag = v2->_flag;
  if ( (flag & 0x82) == 0 )
  {
    *_errno() = 9;
LABEL_3:
    v2->_flag |= 0x20u;
    return -1;
  }
  if ( (flag & 0x40) != 0 )
  {
    *_errno() = 34;
    goto LABEL_3;
  }
  if ( (flag & 1) != 0 )
  {
    v2->_cnt = 0;
    if ( (flag & 0x10) == 0 )
    {
      v2->_flag = flag | 0x20;
      return -1;
    }
    v2->_ptr = v2->_base;
    v2->_flag = flag & 0xFFFFFFFE;
  }
  v5 = v2->_flag & 0xFFFFFFED | 2;
  v2->_flag = v5;
  v2->_cnt = 0;
  v11 = 0;
  if ( (v5 & 0x10C) == 0
    && (v2 != (FILE *)(sub_4B694B() + 32) && v2 != (FILE *)(sub_4B694B() + 64) || !_isatty((int)File)) )
  {
    _getbuf(v2);
  }
  if ( (v2->_flag & 0x108) != 0 )
  {
    base = v2->_base;
    ptr = v2->_ptr;
    v2->_ptr = base + 1;
    v8 = ptr - base;
    v2->_cnt = v2->_bufsiz - 1;
    if ( v8 <= 0 )
    {
      if ( File == (FILE *)-1 || File == (FILE *)-2 )
        v9 = &unk_4FC078;
      else
        v9 = (_BYTE *)(dword_567B00[(int)File >> 5] + (((unsigned __int8)File & 0x1F) << 6));
      if ( (v9[4] & 0x20) != 0 )
      {
        v10 = _lseeki64((int)File, 0LL, 2);
        if ( (HIDWORD(v10) & (unsigned int)v10) == 0xFFFFFFFF )
          goto LABEL_27;
      }
    }
    else
    {
      v11 = _write((int)File, base, v8);
    }
    *v2->_base = Ch;
  }
  else
  {
    v8 = 1;
    v11 = _write((int)File, &Ch, 1u);
  }
  if ( v11 != v8 )
  {
LABEL_27:
    v2->_flag |= 0x20u;
    return -1;
  }
  return (unsigned __int8)Ch;
}

// ===== _write_char @ 0x004AF5A1..0x004AF5D4 =====
int __usercall write_char@<eax>(FILE *File@<ecx>, int result@<eax>, _DWORD *a3@<esi>)
{
  bool v3; // sf

  if ( ((File->_flag & 0x40) == 0 || File->_base)
    && ((v3 = File->_cnt - 1 < 0, --File->_cnt, v3)
      ? (result = _flsbuf((char)result, File))
      : (*File->_ptr = result, ++File->_ptr, result = (unsigned __int8)result),
        result == -1) )
  {
    *a3 = -1;
  }
  else
  {
    ++*a3;
  }
  return result;
}

// ===== _write_string @ 0x004AF5D4..0x004AF636 =====
int __usercall write_string@<eax>(_DWORD *a1@<eax>, int a2@<ebx>, int *a3@<edi>, _BYTE *a4, int a5)
{
  int result; // eax
  int v7; // eax
  int v8; // [esp+4h] [ebp-4h]

  v8 = *a3;
  if ( (*(_BYTE *)(a2 + 12) & 0x40) == 0 || *(_DWORD *)(a2 + 8) )
  {
    *a3 = 0;
    if ( a5 <= 0 )
      goto LABEL_10;
    do
    {
      v7 = (int)a4;
      LOBYTE(v7) = *a4;
      --a5;
      result = write_char((FILE *)a2, v7, a1);
      ++a4;
      if ( *a1 == -1 )
      {
        if ( *a3 != 42 )
          break;
        LOBYTE(result) = 63;
        result = write_char((FILE *)a2, result, a1);
      }
    }
    while ( a5 > 0 );
    if ( !*a3 )
    {
LABEL_10:
      result = v8;
      *a3 = v8;
    }
  }
  else
  {
    result = a5;
    *a1 += a5;
  }
  return result;
}

// ===== __output_l @ 0x004AF636..0x004B021D =====
int __cdecl _output_l(FILE *Stream, _BYTE *a2, struct localeinfo_struct *a3, int *a4)
{
  _BYTE *v4; // ebx
  int *v5; // edi
  int v7; // eax
  _BYTE *v8; // ecx
  char *v9; // eax
  int v10; // ecx
  unsigned __int8 v11; // dl
  _BYTE *v12; // ebx
  int v13; // eax
  char v14; // al
  bool v15; // zf
  int v16; // eax
  int v17; // ecx
  char *v18; // edi
  char *v19; // eax
  _DWORD *v20; // edi
  __int16 *v21; // eax
  char *v22; // ecx
  int v23; // eax
  _WORD *v24; // esi
  __int64 v25; // rax
  _DWORD *v26; // edi
  char *v27; // ebx
  int v28; // esi
  void *v29; // eax
  int v30; // eax
  _DWORD *v31; // edi
  void (__cdecl *v32)(_DWORD *, char *, int, int, int, int, __crt_locale_pointers *); // eax
  int v33; // edi
  void (__cdecl *v34)(char *, __crt_locale_pointers *); // eax
  void (__cdecl *v35)(char *, __crt_locale_pointers *); // eax
  unsigned int v36; // edi
  unsigned int v37; // ebx
  char *j; // esi
  int v39; // eax
  unsigned __int64 v40; // rcx
  int v41; // ecx
  char *v42; // eax
  char *v43; // esi
  char *i; // eax
  int v45; // eax
  int v46; // edi
  FILE *v47; // ebx
  int v48; // eax
  int v49; // edi
  wchar_t *v50; // esi
  wchar_t v51; // ax
  errno_t v52; // eax
  int v53; // edi
  unsigned __int8 v54; // al
  int v55; // [esp-14h] [ebp-2A0h]
  int v56; // [esp-10h] [ebp-29Ch]
  unsigned __int64 v57; // [esp-10h] [ebp-29Ch]
  int v58; // [esp-Ch] [ebp-298h]
  int v59; // [esp-8h] [ebp-294h]
  _DWORD v60[2]; // [esp+Ch] [ebp-280h] BYREF
  int v61; // [esp+14h] [ebp-278h]
  int v62; // [esp+18h] [ebp-274h]
  int v63; // [esp+1Ch] [ebp-270h] BYREF
  int *v64; // [esp+20h] [ebp-26Ch]
  int v65; // [esp+24h] [ebp-268h]
  int v66; // [esp+2Ch] [ebp-260h]
  __crt_locale_pointers Locale; // [esp+30h] [ebp-25Ch] BYREF
  int v68; // [esp+38h] [ebp-254h]
  char v69; // [esp+3Ch] [ebp-250h]
  int v70; // [esp+40h] [ebp-24Ch]
  void *Block; // [esp+44h] [ebp-248h]
  int v72; // [esp+48h] [ebp-244h]
  _BYTE *v73; // [esp+4Ch] [ebp-240h]
  int v74; // [esp+50h] [ebp-23Ch]
  int v75; // [esp+54h] [ebp-238h]
  int v76; // [esp+58h] [ebp-234h]
  FILE *File; // [esp+5Ch] [ebp-230h]
  _BYTE v78[4]; // [esp+60h] [ebp-22Ch] BYREF
  int SizeConverted; // [esp+64h] [ebp-228h] BYREF
  int v80; // [esp+68h] [ebp-224h] BYREF
  char *v81; // [esp+6Ch] [ebp-220h]
  int v82; // [esp+70h] [ebp-21Ch]
  int v83; // [esp+74h] [ebp-218h]
  unsigned __int8 v84; // [esp+7Bh] [ebp-211h]
  int v85; // [esp+7Ch] [ebp-210h]
  char MbCh[511]; // [esp+80h] [ebp-20Ch] BYREF
  char v87; // [esp+27Fh] [ebp-Dh] BYREF
  char v88[8]; // [esp+280h] [ebp-Ch] BYREF

  v4 = a2;
  v5 = a4;
  File = Stream;
  v82 = (int)a4;
  v70 = 0;
  v85 = 0;
  v75 = 0;
  v83 = 0;
  v76 = 0;
  v72 = 0;
  v74 = 0;
  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&Locale, a3);
  v64 = _errno();
  if ( Stream
    && ((Stream->_flag & 0x40) != 0
     || ((v7 = _fileno(Stream), v7 == -1) || v7 == -2
       ? (v8 = &unk_4FC078)
       : (v8 = (_BYTE *)(dword_567B00[v7 >> 5] + ((v7 & 0x1F) << 6))),
         (v8[36] & 0x7F) == 0
      && (v7 == -1 || v7 == -2
        ? (v9 = (char *)&unk_4FC078)
        : (v9 = (char *)(dword_567B00[v7 >> 5] + ((v7 & 0x1F) << 6))),
          v9[36] >= 0)))
    && (v10 = 0, a2) )
  {
    v11 = *a2;
    v80 = 0;
    SizeConverted = 0;
    Block = 0;
    v84 = v11;
    if ( v11 )
    {
      while ( 1 )
      {
        v12 = v4 + 1;
        v73 = v12;
        if ( v80 < 0 )
          break;
        if ( (unsigned __int8)(v11 - 32) > 0x58u )
          v13 = 0;
        else
          v13 = byte_4DC5C0[(char)v11] & 0xF;
        v62 = byte_4DC5E0[8 * v13 + v10] >> 4;
        switch ( v62 )
        {
          case 0:
            goto $NORMAL_STATE$29165;
          case 1:
            v83 = -1;
            v61 = 0;
            v72 = 0;
            v75 = 0;
            v76 = 0;
            v85 = 0;
            v74 = 0;
            goto LABEL_227;
          case 2:
            switch ( v11 )
            {
              case ' ':
                v85 |= 2u;
                break;
              case '#':
                v85 |= 0x80u;
                break;
              case '+':
                v85 |= 1u;
                break;
              case '-':
                v85 |= 4u;
                break;
              case '0':
                v85 |= 8u;
                break;
            }
            goto LABEL_227;
          case 3:
            if ( v11 == 42 )
            {
              v82 = (int)(v5 + 1);
              v75 = *v5;
              if ( v75 < 0 )
              {
                v85 |= 4u;
                v75 = -v75;
              }
            }
            else
            {
              v75 = 10 * v75 + (char)v11 - 48;
            }
            goto LABEL_227;
          case 4:
            v83 = 0;
            goto LABEL_227;
          case 5:
            if ( v11 == 42 )
            {
              v82 = (int)(v5 + 1);
              v83 = *v5;
              if ( v83 < 0 )
                v83 = -1;
            }
            else
            {
              v83 = 10 * v83 + (char)v11 - 48;
            }
            goto LABEL_227;
          case 6:
            switch ( v11 )
            {
              case 'I':
                v14 = *v12;
                if ( *v12 == 54 && v12[1] == 52 )
                {
                  v85 |= 0x8000u;
                  v73 = v12 + 2;
                }
                else if ( v14 == 51 && v12[1] == 50 )
                {
                  v85 &= ~0x8000u;
                  v73 = v12 + 2;
                }
                else if ( v14 != 100 && v14 != 105 && v14 != 111 && v14 != 117 && v14 != 120 && v14 != 88 )
                {
                  v62 = 0;
$NORMAL_STATE$29165:
                  v74 = 0;
                  v16 = _isleadbyte_l(v11, &Locale);
                  v15 = v16 == 0;
                  LOBYTE(v16) = v84;
                  if ( !v15 )
                  {
                    v16 = write_char(File, v16, &v80);
                    LOBYTE(v16) = *v12;
                    v73 = v12 + 1;
                    if ( !(_BYTE)v16 )
                      goto LABEL_2;
                  }
                  write_char(File, v16, &v80);
                }
                break;
              case 'h':
                v85 |= 0x20u;
                break;
              case 'l':
                if ( *v12 == 108 )
                {
                  v85 |= 0x1000u;
                  v73 = v12 + 1;
                }
                else
                {
                  v85 |= 0x10u;
                }
                break;
              case 'w':
                v85 |= 0x800u;
                break;
            }
            goto LABEL_227;
          case 7:
            if ( (char)v11 <= 100 )
            {
              if ( v11 == 100 )
                goto LABEL_118;
              if ( (char)v11 > 83 )
              {
                switch ( v11 )
                {
                  case 'X':
                    goto LABEL_143;
                  case 'Z':
                    v21 = (__int16 *)*v5;
                    v82 = (int)(v5 + 1);
                    if ( v21 && (v22 = (char *)*((_DWORD *)v21 + 1)) != 0 )
                    {
                      v23 = *v21;
                      v81 = v22;
                      if ( (v85 & 0x800) != 0 )
                      {
                        v23 /= 2;
                        v74 = 1;
                      }
                      else
                      {
                        v74 = 0;
                      }
                    }
                    else
                    {
                      v81 = off_4FB8E8;
                      v23 = strlen(off_4FB8E8);
                    }
                    goto LABEL_192;
                  case 'a':
                    goto LABEL_123;
                }
                if ( v11 != 99 )
                  goto LABEL_193;
                v5 = (int *)v82;
              }
              else
              {
                if ( v11 == 83 )
                {
                  if ( (v85 & 0x830) == 0 )
                    v85 |= 0x800u;
                  goto LABEL_81;
                }
                if ( v11 == 65 )
                {
LABEL_76:
                  v11 += 32;
                  v61 = 1;
                  v84 = v11;
                  goto LABEL_123;
                }
                if ( v11 != 67 )
                {
                  if ( v11 != 69 && v11 != 71 )
                    goto LABEL_193;
                  goto LABEL_76;
                }
                if ( (v85 & 0x830) == 0 )
                  v85 |= 0x800u;
              }
              v20 = v5 + 1;
              v82 = (int)v20;
              if ( (v85 & 0x810) != 0 )
              {
                if ( wctomb_s(&SizeConverted, MbCh, 0x200u, *((_WORD *)v20 - 2)) )
                  v72 = 1;
              }
              else
              {
                MbCh[0] = *((_BYTE *)v20 - 4);
                SizeConverted = 1;
              }
              v81 = MbCh;
              goto LABEL_193;
            }
            if ( (char)v11 > 112 )
            {
              if ( v11 != 115 )
              {
                if ( v11 == 117 )
                  goto LABEL_119;
                if ( v11 != 120 )
                  goto LABEL_193;
                v5 = (int *)v82;
                v70 = 39;
                goto $COMMON_HEX$29326;
              }
LABEL_81:
              v17 = v83;
              if ( v83 == -1 )
                v17 = 0x7FFFFFFF;
              v82 = (int)(v5 + 1);
              v18 = (char *)*v5;
              v81 = v18;
              if ( (v85 & 0x810) != 0 )
              {
                if ( !v18 )
                  v81 = (char *)off_4FB8EC;
                v19 = v81;
                v74 = 1;
                while ( v17 )
                {
                  --v17;
                  if ( !*(_WORD *)v19 )
                    break;
                  v19 += 2;
                }
                v23 = (v19 - v81) >> 1;
              }
              else
              {
                if ( !v18 )
                  v81 = off_4FB8E8;
                for ( i = v81; v17; ++i )
                {
                  --v17;
                  if ( !*i )
                    break;
                }
                v23 = i - v81;
              }
              goto LABEL_192;
            }
            if ( v11 == 112 )
            {
              v83 = 8;
LABEL_143:
              v70 = 7;
$COMMON_HEX$29326:
              SizeConverted = 16;
              if ( (v85 & 0x80u) != 0 )
              {
                v78[0] = 48;
                v78[1] = v70 + 81;
                v76 = 2;
              }
              goto $COMMON_INT$29319;
            }
            if ( (char)v11 < 101 )
              goto LABEL_193;
            if ( (char)v11 <= 103 )
            {
              v5 = (int *)v82;
LABEL_123:
              v85 |= 0x40u;
              v27 = MbCh;
              v81 = MbCh;
              v66 = 512;
              if ( v83 >= 0 )
              {
                if ( v83 )
                {
                  if ( v83 > 512 )
                    v83 = 512;
                  if ( v83 > 163 )
                  {
                    v28 = v83 + 349;
                    v29 = _malloc_crt(v83 + 349);
                    v11 = v84;
                    Block = v29;
                    if ( v29 )
                    {
                      v81 = (char *)v29;
                      v66 = v28;
                      v27 = (char *)v29;
                    }
                    else
                    {
                      v83 = 163;
                    }
                  }
                }
                else
                {
                  v83 = v11 == 103;
                }
              }
              else
              {
                v83 = 6;
              }
              v30 = *v5;
              v31 = v5 + 2;
              v60[0] = v30;
              v60[1] = *(v31 - 1);
              v59 = v61;
              v58 = v83;
              v82 = (int)v31;
              v56 = (char)v11;
              v55 = v66;
              v32 = (void (__cdecl *)(_DWORD *, char *, int, int, int, int, __crt_locale_pointers *))DecodePointer(off_4FC0F8);
              v32(v60, v27, v55, v56, v58, v59, &Locale);
              v33 = v85 & 0x80;
              if ( (v85 & 0x80) != 0 && !v83 )
              {
                v34 = (void (__cdecl *)(char *, __crt_locale_pointers *))DecodePointer(off_4FC104);
                v34(v27, &Locale);
              }
              if ( v84 == 103 && !v33 )
              {
                v35 = (void (__cdecl *)(char *, __crt_locale_pointers *))DecodePointer(off_4FC100);
                v35(v27, &Locale);
              }
              if ( *v27 == 45 )
              {
                v85 |= 0x100u;
                v81 = ++v27;
              }
              v23 = strlen(v27);
LABEL_192:
              SizeConverted = v23;
LABEL_193:
              if ( v72 )
                goto LABEL_225;
              if ( (v85 & 0x40) != 0 )
              {
                if ( (v85 & 0x100) != 0 )
                {
                  v78[0] = 45;
                  goto LABEL_201;
                }
                if ( (v85 & 1) != 0 )
                {
                  v78[0] = 43;
                  goto LABEL_201;
                }
                if ( (v85 & 2) != 0 )
                {
                  v78[0] = 32;
LABEL_201:
                  v76 = 1;
                }
              }
              v45 = v75 - SizeConverted - v76;
              v66 = v45;
              if ( (v85 & 0xC) == 0 )
              {
                v46 = v75 - SizeConverted - v76;
                do
                {
                  if ( v46 <= 0 )
                    break;
                  LOBYTE(v45) = 32;
                  --v46;
                  v45 = write_char(File, v45, &v80);
                }
                while ( v80 != -1 );
              }
              v47 = File;
              v48 = write_string(&v80, (int)File, v64, v78, v76);
              if ( (v85 & 8) != 0 && (v85 & 4) == 0 )
              {
                v49 = v66;
                do
                {
                  if ( v49 <= 0 )
                    break;
                  LOBYTE(v48) = 48;
                  --v49;
                  v48 = write_char(v47, v48, &v80);
                }
                while ( v80 != -1 );
              }
              if ( v74 && SizeConverted > 0 )
              {
                v50 = (wchar_t *)v81;
                v65 = SizeConverted;
                while ( 1 )
                {
                  v51 = *v50;
                  --v65;
                  ++v50;
                  v52 = wctomb_s(&v63, v88, 6u, v51);
                  if ( v52 || !v63 )
                    break;
                  v52 = write_string(&v80, (int)File, v64, v88, v63);
                  if ( !v65 )
                    goto LABEL_220;
                }
                v80 = -1;
              }
              else
              {
                v52 = write_string(&v80, (int)v47, v64, v81, SizeConverted);
              }
LABEL_220:
              if ( v80 >= 0 && (v85 & 4) != 0 )
              {
                v53 = v66;
                do
                {
                  if ( v53 <= 0 )
                    break;
                  LOBYTE(v52) = 32;
                  --v53;
                  v52 = write_char(File, v52, &v80);
                }
                while ( v80 != -1 );
              }
              goto LABEL_225;
            }
            if ( v11 == 105 )
            {
              v5 = (int *)v82;
LABEL_118:
              v85 |= 0x40u;
LABEL_119:
              SizeConverted = 10;
$COMMON_INT$29319:
              if ( (v85 & 0x8000) != 0 || (v85 & 0x1000) != 0 )
              {
                v25 = *(_QWORD *)v5;
                v26 = v5 + 2;
              }
              else
              {
                v26 = v5 + 1;
                if ( (v85 & 0x20) != 0 )
                {
                  v82 = (int)v26;
                  if ( (v85 & 0x40) != 0 )
                    LODWORD(v25) = *((__int16 *)v26 - 2);
                  else
                    LODWORD(v25) = *((unsigned __int16 *)v26 - 2);
                  v25 = (int)v25;
LABEL_160:
                  if ( (v85 & 0x40) != 0 && v25 < 0 )
                  {
                    v25 = -v25;
                    v85 |= 0x100u;
                  }
                  v36 = HIDWORD(v25);
                  v37 = v25;
                  if ( (v85 & 0x9000) == 0 )
                    v36 = 0;
                  if ( v83 >= 0 )
                  {
                    v85 &= ~8u;
                    if ( v83 > 512 )
                      v83 = 512;
                  }
                  else
                  {
                    v83 = 1;
                  }
                  if ( !(v36 | (unsigned int)v25) )
                    v76 = 0;
                  for ( j = &v87; ; --j )
                  {
                    v39 = v83--;
                    if ( v39 <= 0 && !(v36 | v37) )
                      break;
                    v57 = __PAIR64__(v36, v37);
                    v40 = __PAIR64__(v36, v37) % SizeConverted;
                    v41 = v40 + 48;
                    v66 = HIDWORD(v40);
                    v36 = (v57 / SizeConverted) >> 32;
                    v37 = v57 / SizeConverted;
                    if ( v41 > 57 )
                      LOBYTE(v41) = v70 + v41;
                    *j = v41;
                  }
                  v42 = (char *)(&v87 - j);
                  v43 = j + 1;
                  SizeConverted = (int)v42;
                  v81 = v43;
                  if ( (v85 & 0x200) != 0 && (!v42 || *v43 != 48) )
                  {
                    *--v81 = 48;
                    v23 = (int)(v42 + 1);
                    goto LABEL_192;
                  }
                  goto LABEL_193;
                }
                LODWORD(v25) = *(v26 - 1);
                if ( (v85 & 0x40) != 0 )
                  v25 = (int)v25;
                else
                  HIDWORD(v25) = 0;
              }
              v82 = (int)v26;
              goto LABEL_160;
            }
            if ( v11 != 110 )
            {
              if ( v11 != 111 )
                goto LABEL_193;
              v5 = (int *)v82;
              SizeConverted = 8;
              if ( (v85 & 0x80u) != 0 )
                v85 |= 0x200u;
              goto $COMMON_INT$29319;
            }
            v82 += 4;
            v24 = *(_WORD **)(v82 - 4);
            if ( !_get_printf_count_output() )
              goto LABEL_2;
            if ( (v85 & 0x20) != 0 )
              *v24 = v80;
            else
              *(_DWORD *)v24 = v80;
            v72 = 1;
LABEL_225:
            if ( Block )
            {
              free(Block);
              Block = 0;
            }
LABEL_227:
            v4 = v73;
            v54 = *v73;
            v84 = v54;
            if ( !v54 )
              goto LABEL_229;
            v10 = v62;
            v5 = (int *)v82;
            v11 = v54;
            break;
          default:
            goto LABEL_227;
        }
      }
    }
LABEL_229:
    if ( v69 )
      *(_DWORD *)(v68 + 112) &= ~2u;
    return v80;
  }
  else
  {
LABEL_2:
    *_errno() = 22;
    _invalid_parameter_noinfo();
    if ( v69 )
      *(_DWORD *)(v68 + 112) &= ~2u;
    return -1;
  }
}
