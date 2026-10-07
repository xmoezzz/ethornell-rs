#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4C0050 @ 0x004C0050..0x004C0141 =====
int __thiscall sub_4C0050(int this, int a2, int a3, int a4, int a5, void *Src, int a7)
{
  int v8; // edi
  void *v9; // eax

  sub_4BCF60((_DWORD *)this, a2, 0);
  *(_DWORD *)(this + 28) = a7;
  *(_DWORD *)(this + 20) = 0;
  *(_DWORD *)(this + 24) = 0;
  *(_DWORD *)(this + 32) = a4;
  *(_WORD *)(this + 36) = 0;
  *(_BYTE *)(this + 38) = 0;
  *(_DWORD *)(this + 40) = a3;
  *(_DWORD *)(this + 44) = 0;
  *(_DWORD *)(this + 48) = 1;
  sub_4C0AB0(this + 52);
  *(_DWORD *)(this + 128) = 0;
  *(_DWORD *)(this + 132) = 0;
  *(_DWORD *)(this + 136) = -1;
  *(_DWORD *)(this + 140) = 0x7FFFFFFF;
  *(double *)(this + 144) = 1.0;
  if ( Src )
  {
    v8 = sub_4C34C0(Src);
    v9 = operator new(2 * (v8 + 1));
    *(_DWORD *)(this + 20) = v9;
    if ( v9 )
      memcpy_0(v9, Src, 2 * v8 + 2);
  }
  return this;
}

// ===== sub_4C0150 @ 0x004C0150..0x004C0259 =====
int __stdcall sub_4C0150(int a1, int a2, int a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // ebx
  int v5; // edi

  if ( !a2 )
    return -2147467261;
  v4 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 20);
  EnterCriticalSection(v4);
  if ( *(_DWORD *)(a1 + 12) )
  {
    LeaveCriticalSection(v4);
    return -2147220988;
  }
  else if ( !*(_DWORD *)(*(_DWORD *)(a1 + 28) + 20) || *(_BYTE *)(a1 + 25) )
  {
    v5 = sub_4BE0B0(a1 - 12, a2, a3);
    if ( v5 >= 0 )
    {
      LeaveCriticalSection(v4);
      return 0;
    }
    else
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 44))(a1 - 12);
      LeaveCriticalSection(v4);
      return v5;
    }
  }
  else
  {
    LeaveCriticalSection(v4);
    return -2147220956;
  }
}

// ===== sub_4C0260 @ 0x004C0260..0x004C02EA =====
int __stdcall sub_4C0260(int a1)
{
  struct _RTL_CRITICAL_SECTION *v1; // esi
  int v3; // edi

  v1 = *(struct _RTL_CRITICAL_SECTION **)(a1 + 20);
  EnterCriticalSection(v1);
  if ( *(_DWORD *)(*(_DWORD *)(a1 + 28) + 20) )
  {
    LeaveCriticalSection(v1);
    return -2147220956;
  }
  else
  {
    v3 = sub_4BE390((_DWORD *)(a1 - 12));
    LeaveCriticalSection(v1);
    return v3;
  }
}

// ===== sub_4C02F0 @ 0x004C02F0..0x004C037F =====
int __stdcall sub_4C02F0(int a1, _DWORD *a2)
{
  _DWORD *v3; // eax
  _DWORD *v4; // eax

  if ( !a2 )
    return -2147467261;
  v3 = operator new(0x14u);
  if ( v3 )
    v4 = sub_4BFF10(v3, a1 - 12, 0);
  else
    v4 = 0;
  *a2 = v4;
  return v4 != 0 ? 0 : -2147024882;
}

// ===== sub_4C0380 @ 0x004C0380..0x004C03D0 =====
int __thiscall sub_4C0380(int this, int a2, int a3, int a4, int a5, void *Src)
{
  sub_4C0050(this, a2, a3, a4, a5, Src, 0);
  *(_DWORD *)(this + 156) = 0;
  *(_WORD *)(this + 160) = 0;
  memset((void *)(this + 168), 0, 0x30u);
  return this;
}

// ===== sub_4C03D0 @ 0x004C03D0..0x004C03F1 =====
_DWORD *__thiscall sub_4C03D0(_DWORD *this, char a2)
{
  sub_4BEC10(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C0400 @ 0x004C0400..0x004C04EB =====
int __stdcall sub_4C0400(_DWORD *a1, int a2)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v4; // ebx
  int v5; // eax

  v2 = 0;
  if ( !a2 )
    return -2147467261;
  if ( a1 != (_DWORD *)12 )
    v2 = (struct _RTL_CRITICAL_SECTION *)(a1 + 1);
  EnterCriticalSection(v2);
  *(_DWORD *)(a2 + 28) = a1[7];
  a1[7] = a2;
  v4 = 1;
  ++a1[8];
  if ( a1[10] )
    sub_4BF540((int)(a1 - 3));
  if ( a1[18] && a1[8] == a1[12] )
  {
    (*(void (__thiscall **)(_DWORD *))(*(a1 - 3) + 16))(a1 - 3);
    a1[18] = 0;
  }
  else
  {
    v4 = 0;
  }
  LeaveCriticalSection(v2);
  v5 = a1[19];
  if ( v5 )
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v5 + 12))(a1[19]);
  if ( v4 )
    (*(void (__stdcall **)(_DWORD *))(*a1 + 8))(a1);
  return 0;
}

// ===== sub_4C04F0 @ 0x004C04F0..0x004C0511 =====
_DWORD *__thiscall sub_4C04F0(_DWORD *this, char a2)
{
  sub_4BF880(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C0520 @ 0x004C0520..0x004C057A =====
_DWORD *__thiscall sub_4C0520(_DWORD *this, int a2, _DWORD *a3, int a4, _DWORD *a5)
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

// ===== sub_4C0580 @ 0x004C0580..0x004C060F =====
int __stdcall sub_4C0580(int a1, _DWORD *a2)
{
  _DWORD *v3; // eax
  _DWORD *v4; // eax

  if ( !a2 )
    return -2147467261;
  v3 = operator new(0x30u);
  if ( v3 )
    v4 = sub_4BFC50(v3, a1 - 12, 0);
  else
    v4 = 0;
  *a2 = v4;
  return v4 != 0 ? 0 : -2147024882;
}

// ===== sub_4C0610 @ 0x004C0610..0x004C0645 =====
_DWORD *__thiscall sub_4C0610(_DWORD *this, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *result; // eax

  result = this;
  this[3] = a5;
  this[4] = a6;
  this[5] = a6;
  *this = &CMediaSample::`vftable';
  this[1] = 0;
  this[2] = 0;
  this[6] = a3;
  this[15] = 0;
  this[16] = 0;
  this[17] = 0;
  return result;
}

// ===== sub_4C0650 @ 0x004C0650..0x004C06D2 =====
int __thiscall sub_4C0650(int this, int a2, _DWORD *a3, _DWORD *a4, int a5, int a6)
{
  HANDLE SemaphoreA; // eax
  bool v8; // zf
  int result; // eax

  sub_4BCF60((_DWORD *)this, a2, a3);
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 16));
  *(_DWORD *)(this + 40) = 0;
  *(_DWORD *)(this + 44) = 0;
  *(_DWORD *)(this + 48) = 0;
  *(_DWORD *)(this + 52) = 0;
  *(_DWORD *)(this + 56) = 0;
  *(_DWORD *)(this + 60) = 0;
  *(_DWORD *)(this + 64) = 0;
  *(_DWORD *)(this + 68) = 0;
  *(_DWORD *)(this + 72) = 0;
  *(_DWORD *)(this + 76) = 0;
  *(_DWORD *)(this + 80) = 0;
  *(_DWORD *)(this + 84) = 0;
  *(_DWORD *)(this + 88) = 0;
  *(_DWORD *)(this + 92) = a6;
  if ( !a5 )
    return this;
  SemaphoreA = CreateSemaphoreA(0, 0, 0x7FFFFFFF, 0);
  *(_DWORD *)(this + 48) = SemaphoreA;
  v8 = SemaphoreA == 0;
  result = this;
  if ( v8 )
    *a4 = -2147024882;
  return result;
}

// ===== sub_4C06E0 @ 0x004C06E0..0x004C0718 =====
_DWORD *__thiscall sub_4C06E0(_DWORD *this, int a2, _DWORD *a3, _DWORD *a4)
{
  sub_4C0650((int)this, a2, a3, a4, 1, 1);
  *this = &CMemAllocator::`vftable';
  this[3] = &CMemAllocator::`vftable';
  this[24] = 0;
  return this;
}

// ===== sub_4C0720 @ 0x004C0720..0x004C089E =====
int __thiscall sub_4C0720(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // ebx
  int v3; // eax
  int v4; // edi
  int v6; // ecx
  int v7; // edi
  char *v8; // eax
  char *v9; // ebx
  _DWORD *v10; // eax
  _DWORD *v11; // eax
  struct _RTL_CRITICAL_SECTION *v12; // ecx
  struct _RTL_CRITICAL_SECTION *v13; // [esp+10h] [ebp-14h] BYREF
  LPCRITICAL_SECTION lpCriticalSection; // [esp+14h] [ebp-10h]
  int v15; // [esp+20h] [ebp-4h]

  if ( this )
  {
    v2 = (struct _RTL_CRITICAL_SECTION *)(this + 16);
    lpCriticalSection = (LPCRITICAL_SECTION)(this + 16);
  }
  else
  {
    lpCriticalSection = 0;
    v2 = 0;
  }
  v13 = v2;
  EnterCriticalSection(v2);
  v15 = 0;
  v3 = sub_4BF6E0((int *)this);
  v4 = v3;
  if ( v3 < 0 )
  {
    LeaveCriticalSection(v2);
    return v4;
  }
  if ( v3 == 1 )
  {
    LeaveCriticalSection(v2);
    return 0;
  }
  if ( *(_DWORD *)(this + 96) )
    sub_4BF830((_DWORD *)this);
  v6 = *(_DWORD *)(this + 68);
  v7 = *(_DWORD *)(this + 72) + *(_DWORD *)(this + 64);
  if ( v6 > 1 && v7 % v6 )
    v7 += v6 - v7 % v6;
  v8 = (char *)VirtualAlloc(0, v7 * *(_DWORD *)(this + 56), 0x1000u, 4u);
  *(_DWORD *)(this + 96) = v8;
  if ( !v8 )
  {
    LeaveCriticalSection(v2);
    return -2147024882;
  }
  v9 = v8;
  if ( *(_DWORD *)(this + 60) >= *(_DWORD *)(this + 56) )
  {
LABEL_19:
    v12 = lpCriticalSection;
    *(_DWORD *)(this + 76) = 0;
    LeaveCriticalSection(v12);
    return 0;
  }
  while ( 1 )
  {
    v10 = operator new(0x48u);
    if ( !v10 )
      break;
    v11 = sub_4C0610(v10, 0, this, (int)&v13, (int)&v9[*(_DWORD *)(this + 72)], *(_DWORD *)(this + 64));
    if ( !v11 )
      break;
    v11[7] = *(_DWORD *)(this + 40);
    *(_DWORD *)(this + 40) = v11;
    ++*(_DWORD *)(this + 44);
    ++*(_DWORD *)(this + 60);
    v9 += v7;
    if ( *(_DWORD *)(this + 60) >= *(_DWORD *)(this + 56) )
      goto LABEL_19;
  }
  LeaveCriticalSection(lpCriticalSection);
  return -2147024882;
}

// ===== sub_4C08A0 @ 0x004C08A0..0x004C08BD =====
int __thiscall sub_4C08A0(void *this)
{
  int result; // eax

  memset(this, 0, 0x48u);
  result = 1;
  *((_DWORD *)this + 10) = 1;
  *((_DWORD *)this + 8) = 1;
  return result;
}

// ===== sub_4C08C0 @ 0x004C08C0..0x004C08F3 =====
int __thiscall sub_4C08C0(_DWORD *this)
{
  int result; // eax

  if ( sub_445510(this, &dword_4DC2F8) )
    return 1;
  result = sub_445510(this + 11, &dword_4DC2F8);
  if ( result )
    return 1;
  return result;
}

// ===== sub_4C0900 @ 0x004C0900..0x004C09E1 =====
int __thiscall sub_4C0900(_DWORD *this, _DWORD *a2)
{
  int result; // eax
  unsigned int v4; // eax
  int v5; // ecx
  int v6; // esi

  if ( sub_445510(a2, &dword_4DC2F8) || (result = sub_445510(this, a2)) != 0 )
  {
    if ( !sub_445510(a2 + 4, &dword_4DC2F8) && !sub_445510(this + 4, a2 + 4) )
      return 0;
    if ( !sub_445510(a2 + 11, &dword_4DC2F8) )
    {
      if ( !sub_445510(this + 11, a2 + 11) )
        return 0;
      v4 = this[16];
      if ( v4 != a2[16] )
        return 0;
      if ( v4 )
      {
        v5 = a2[17];
        v6 = this[17];
        if ( v4 >= 4 )
        {
          while ( *(_DWORD *)v6 == *(_DWORD *)v5 )
          {
            v4 -= 4;
            v5 += 4;
            v6 += 4;
            if ( v4 < 4 )
              goto LABEL_12;
          }
          return 0;
        }
LABEL_12:
        if ( v4
          && (*(_BYTE *)v5 != *(_BYTE *)v6
           || v4 > 1 && (*(_BYTE *)(v5 + 1) != *(_BYTE *)(v6 + 1) || v4 > 2 && *(_BYTE *)(v5 + 2) != *(_BYTE *)(v6 + 2))) )
        {
          return 0;
        }
      }
    }
    return 1;
  }
  return result;
}

// ===== sub_4C09F0 @ 0x004C09F0..0x004C0A57 =====
int __stdcall sub_4C09F0(_DWORD *a1, int a2)
{
  SIZE_T v2; // eax
  void *v3; // eax
  int v5; // ebx

  qmemcpy(a1, (const void *)a2, 0x48u);
  v2 = *(_DWORD *)(a2 + 64);
  if ( v2 )
  {
    v3 = CoTaskMemAlloc(v2);
    a1[17] = v3;
    if ( !v3 )
    {
      a1[16] = 0;
      return -2147024882;
    }
    memcpy_0(v3, *(const void **)(a2 + 68), a1[16]);
  }
  v5 = a1[15];
  if ( v5 )
    (*(void (__stdcall **)(int))(*(_DWORD *)v5 + 4))(v5);
  return 0;
}

// ===== sub_4C0A60 @ 0x004C0A60..0x004C0AA0 =====
int __stdcall sub_4C0A60(int a1)
{
  int result; // eax

  if ( *(_DWORD *)(a1 + 64) )
  {
    CoTaskMemFree(*(LPVOID *)(a1 + 68));
    *(_DWORD *)(a1 + 64) = 0;
    *(_DWORD *)(a1 + 68) = 0;
  }
  result = *(_DWORD *)(a1 + 60);
  if ( result )
  {
    result = (*(int (__stdcall **)(_DWORD))(*(_DWORD *)result + 8))(*(_DWORD *)(a1 + 60));
    *(_DWORD *)(a1 + 60) = 0;
  }
  return result;
}

// ===== sub_4C0AA0 @ 0x004C0AA0..0x004C0AA7 =====
int __thiscall sub_4C0AA0(void *this)
{
  return sub_4C0A60((int)this);
}

// ===== sub_4C0AB0 @ 0x004C0AB0..0x004C0ABC =====
void *__thiscall sub_4C0AB0(void *this)
{
  sub_4C08A0(this);
  return this;
}

// ===== sub_4C0AC0 @ 0x004C0AC0..0x004C0AF2 =====
int __thiscall sub_4C0AC0(_DWORD *this, int a2)
{
  if ( (_DWORD *)a2 == this )
    return 0;
  sub_4C0A60((int)this);
  if ( sub_4C09F0(this, a2) >= 0 )
    return 0;
  else
    return -2147024882;
}

// ===== sub_4C0B00 @ 0x004C0B00..0x004C0B1D =====
void __stdcall sub_4C0B00(LPVOID pv)
{
  if ( pv )
  {
    sub_4C0A60((int)pv);
    CoTaskMemFree(pv);
  }
}

// ===== sub_4C0B20 @ 0x004C0B20..0x004C0B55 =====
_DWORD *__stdcall sub_4C0B20(int a1)
{
  _DWORD *v1; // esi

  v1 = CoTaskMemAlloc(0x48u);
  if ( !v1 )
    return 0;
  if ( sub_4C09F0(v1, a1) < 0 )
  {
    CoTaskMemFree(v1);
    return 0;
  }
  return v1;
}

// ===== sub_4C0B60 @ 0x004C0B60..0x004C0B76 =====
_DWORD *__thiscall sub_4C0B60(_DWORD *this, int a2)
{
  sub_4C0AC0(this, a2);
  return this;
}

// ===== sub_4C0B80 @ 0x004C0B80..0x004C0B96 =====
_DWORD *__thiscall sub_4C0B80(_DWORD *this, int a2)
{
  sub_4C0B60(this, a2);
  return this;
}

// ===== sub_4C0BA0 @ 0x004C0BA0..0x004C0BA9 =====
int __thiscall sub_4C0BA0(_DWORD *this, int a2)
{
  return sub_4C0AC0(this, a2);
}

// ===== sub_4C0BB0 @ 0x004C0BB0..0x004C0CAB =====
int __thiscall sub_4C0BB0(int this, _DWORD *a2, int a3, _DWORD *a4, int a5)
{
  struct _RTL_CRITICAL_SECTION *v6; // ebx

  v6 = (struct _RTL_CRITICAL_SECTION *)(this + 124);
  sub_4C0520((_DWORD *)this, a3, a4, this + 124, a2);
  *(_DWORD *)this = &CBaseRenderer::`vftable';
  *(_DWORD *)(this + 12) = &CBaseRenderer::`vftable';
  *(_DWORD *)(this + 16) = &CBaseRenderer::`vftable';
  *(_DWORD *)(this + 80) = 0;
  sub_4C33D0(0);
  sub_4C33D0(1);
  sub_4C33D0(1);
  *(_DWORD *)(this + 96) = 0;
  *(_DWORD *)(this + 100) = 0;
  *(_DWORD *)(this + 104) = 0;
  *(_DWORD *)(this + 108) = 0;
  *(_DWORD *)(this + 112) = 0;
  *(_DWORD *)(this + 116) = 0;
  *(_DWORD *)(this + 120) = 0;
  InitializeCriticalSection(v6);
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 148));
  *(_DWORD *)(this + 172) = 0;
  *(_DWORD *)(this + 176) = 1;
  *(_DWORD *)(this + 180) = 0;
  *(_DWORD *)(this + 184) = 0;
  *(_DWORD *)(this + 188) = 0;
  *(_DWORD *)(this + 192) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 196));
  SetEvent(*(HANDLE *)(this + 92));
  return this;
}

// ===== sub_4C0CB0 @ 0x004C0CB0..0x004C0DE9 =====
int __thiscall sub_4C0CB0(char *this, int a2, int a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // edi
  int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // ecx
  int v9; // eax
  int v11; // esi
  int v12; // [esp+18h] [ebp-10h] BYREF
  int v13; // [esp+24h] [ebp-4h]

  v4 = (struct _RTL_CRITICAL_SECTION *)(this + 196);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 196));
  v5 = *((_DWORD *)this + 20);
  v13 = 0;
  if ( v5 )
  {
    v6 = (**(int (__stdcall ***)(int, int, int))(v5 + 8))(v5 + 8, a2, a3);
  }
  else
  {
    v12 = 0;
    LOBYTE(v13) = 1;
    if ( operator new(0x50u) )
    {
      v7 = (*(int (__thiscall **)(char *, _DWORD))(*(_DWORD *)this + 28))(this, 0);
      if ( v7 )
        v8 = v7 + 12;
      else
        v8 = 0;
      v9 = sub_4C4C20(0, *((_DWORD *)this + 1), &v12, v8);
    }
    else
    {
      v9 = 0;
    }
    LOBYTE(v13) = 0;
    *((_DWORD *)this + 20) = v9;
    if ( !v9 )
    {
      LeaveCriticalSection(v4);
      return -2147024882;
    }
    if ( v12 < 0 )
    {
      (*(void (__thiscall **)(int, int))(*(_DWORD *)(v9 + 8) + 12))(v9 + 8, 1);
      *((_DWORD *)this + 20) = 0;
      LeaveCriticalSection(v4);
      return -2147467262;
    }
    v6 = (*(int (__thiscall **)(char *, int, int))(*(_DWORD *)this + 36))(this, a2, a3);
  }
  v11 = v6;
  LeaveCriticalSection(v4);
  return v11;
}

// ===== sub_4C0DF0 @ 0x004C0DF0..0x004C0E42 =====
int __stdcall sub_4C0DF0(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB8E4) || sub_445510(a2, dword_4DB9B4) )
    return (*(int (__thiscall **)(int, _DWORD *, _DWORD *))(*(_DWORD *)a1 + 36))(a1, a2, a3);
  else
    return sub_4BD080(a1, a2, a3);
}

// ===== sub_4C0E50 @ 0x004C0E50..0x004C0E79 =====
int __thiscall sub_4C0E50(HANDLE *this, int a2)
{
  if ( a2 == 1 )
    ResetEvent(this[22]);
  else
    SetEvent(this[22]);
  return 0;
}

// ===== sub_4C0E80 @ 0x004C0E80..0x004C0EE6 =====
DWORD __thiscall sub_4C0E80(_DWORD *this)
{
  DWORD result; // eax
  DWORD CurrentThreadId; // eax
  struct tagMSG Msg; // [esp+4h] [ebp-1Ch] BYREF

  while ( this[45] )
  {
    PeekMessageA(&Msg, 0, 0, 0, 0);
    Sleep(1u);
  }
  result = GetQueueStatus(8u) >> 16;
  if ( (result & 8) != 0 )
  {
    CurrentThreadId = GetCurrentThreadId();
    return PostThreadMessageA(CurrentThreadId, 0, 0, 0);
  }
  return result;
}

// ===== sub_4C0EF0 @ 0x004C0EF0..0x004C0F40 =====
int __stdcall sub_4C0EF0(int a1, DWORD dwMilliseconds, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  if ( sub_4C37E0(*(_DWORD *)(a1 + 80), dwMilliseconds, 0, 0, 0) == 258 )
  {
    *a3 = *(_DWORD *)(a1 + 8);
    return 262711;
  }
  else
  {
    *a3 = *(_DWORD *)(a1 + 8);
    return 0;
  }
}

// ===== sub_4C0F40 @ 0x004C0F40..0x004C0FB1 =====
int __thiscall sub_4C0F40(void *this, int a2)
{
  if ( !*(_DWORD *)(*((_DWORD *)this + 30) + 24) )
    goto LABEL_2;
  if ( *((_DWORD *)this + 28) == 1 )
  {
    SetEvent(*((HANDLE *)this + 23));
    return 0;
  }
  else
  {
    if ( (*(int (__thiscall **)(void *))(*(_DWORD *)this + 160))(this) == 1 && a2 )
    {
LABEL_2:
      SetEvent(*((HANDLE *)this + 23));
      return 0;
    }
    ResetEvent(*((HANDLE *)this + 23));
    return 1;
  }
}

// ===== sub_4C0FC0 @ 0x004C0FC0..0x004C1031 =====
int __stdcall sub_4C0FC0(int a1, int a2, int *a3)
{
  int v4; // eax
  int v5; // eax

  if ( !a3 )
    return -2147467261;
  if ( sub_4C3470(a2, L"In") )
  {
    *a3 = 0;
    return -2147220970;
  }
  else
  {
    v4 = (*(int (__thiscall **)(int, _DWORD))(*(_DWORD *)(a1 - 12) + 28))(a1 - 12, 0);
    if ( v4 )
    {
      v5 = v4 + 12;
      *a3 = v5;
      (*(void (__stdcall **)(int))(*(_DWORD *)v5 + 4))(v5);
    }
    else
    {
      *a3 = 0;
      (*(void (__stdcall **)(_DWORD))(MEMORY[0] + 4))(0);
    }
    return 0;
  }
}

// ===== sub_4C1040 @ 0x004C1040..0x004C1073 =====
int __thiscall sub_4C1040(void *this)
{
  bool v2; // zf

  if ( *((_DWORD *)this + 5) )
  {
    v2 = *((_DWORD *)this + 27) == 0;
    *((_DWORD *)this + 28) = 1;
    if ( v2 )
    {
      SetEvent(*((HANDLE *)this + 23));
      if ( *((_DWORD *)this + 25) )
        (*(void (__thiscall **)(void *))(*(_DWORD *)this + 96))(this);
    }
  }
  return 0;
}

// ===== sub_4C1080 @ 0x004C1080..0x004C10BB =====
int __thiscall sub_4C1080(_DWORD *this)
{
  if ( this[5] == 1 )
    ResetEvent((HANDLE)this[23]);
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*this + 40))(this, 0);
  (*(void (__thiscall **)(_DWORD *))(*this + 108))(this);
  (*(void (__thiscall **)(_DWORD *))(*this + 112))(this);
  sub_4C0E80(this);
  return 0;
}

// ===== sub_4C10C0 @ 0x004C10C0..0x004C10DE =====
int __thiscall sub_4C10C0(_DWORD *this)
{
  if ( this[20] )
    sub_4C4970();
  (*(void (__thiscall **)(_DWORD *, int))(*this + 40))(this, 1);
  return 0;
}

// ===== sub_4C10E0 @ 0x004C10E0..0x004C10FC =====
int __thiscall sub_4C10E0(_DWORD *this)
{
  if ( this[20] )
    sub_4C4970();
  (*(void (__thiscall **)(_DWORD *))(*this + 112))(this);
  return 0;
}

// ===== sub_4C1100 @ 0x004C1100..0x004C1160 =====
int __thiscall sub_4C1100(_DWORD *this, int a2, _QWORD *a3, _QWORD *a4)
{
  if ( (*(int (__stdcall **)(int, _QWORD *, _QWORD *))(*(_DWORD *)a2 + 20))(a2, a3, a4) < 0 )
    return 0;
  if ( *a4 < *a3 )
    return -2147220952;
  if ( this[6] )
    return (*(int (__thiscall **)(_DWORD *, int, _QWORD *, _QWORD *))(*this + 92))(this, a2, a3, a4);
  else
    return 0;
}

// ===== _DllMain@12 @ 0x004C1160..0x004C1168 =====
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  return 1;
}

// ===== sub_4C1170 @ 0x004C1170..0x004C1178 =====
void __thiscall sub_4C1170(_DWORD *this)
{
  this[26] = 0;
}

// ===== sub_4C1180 @ 0x004C1180..0x004C11B2 =====
BOOL __thiscall sub_4C1180(int this)
{
  int v2; // edi

  v2 = *(_DWORD *)(this + 104);
  if ( v2 )
  {
    (*(void (__stdcall **)(_DWORD, int))(**(_DWORD **)(this + 24) + 24))(*(_DWORD *)(this + 24), v2);
    sub_4C1170((_DWORD *)this);
  }
  ResetEvent(*(HANDLE *)(this + 84));
  return v2 == 0;
}

// ===== sub_4C11C0 @ 0x004C11C0..0x004C123E =====
BOOL __thiscall sub_4C11C0(void *this, int a2)
{
  int v4; // eax
  _BYTE v5[8]; // [esp+4h] [ebp-10h] BYREF
  _DWORD v6[2]; // [esp+Ch] [ebp-8h] BYREF

  if ( !a2 )
    return 0;
  v4 = (*(int (__thiscall **)(void *, int, _DWORD *, _BYTE *))(*(_DWORD *)this + 88))(this, a2, v6, v5);
  if ( v4 < 0 )
    return 0;
  if ( v4 )
    return (*(int (__stdcall **)(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, int))(**((_DWORD **)this + 6) + 16))(
             *((_DWORD *)this + 6),
             *((_DWORD *)this + 8),
             *((_DWORD *)this + 9),
             v6[0],
             v6[1],
             *((_DWORD *)this + 21),
             (int)this + 104) >= 0;
  SetEvent(*((HANDLE *)this + 21));
  return 1;
}

// ===== sub_4C1240 @ 0x004C1240..0x004C1286 =====
int __thiscall sub_4C1240(_DWORD *this, int a2)
{
  if ( !a2 || !this[25] )
    return 1;
  (*(void (__thiscall **)(_DWORD *, int))(*this + 56))(this, a2);
  (*(void (__thiscall **)(_DWORD *, int))(*this + 172))(this, a2);
  (*(void (__thiscall **)(_DWORD *, int))(*this + 60))(this, a2);
  return 0;
}

// ===== sub_4C1290 @ 0x004C1290..0x004C12B7 =====
BOOL __thiscall sub_4C1290(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  BOOL v3; // esi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  v3 = *(_DWORD *)(this + 108) != 0;
  LeaveCriticalSection(v2);
  return v3;
}

// ===== sub_4C12C0 @ 0x004C12C0..0x004C132C =====
int __thiscall sub_4C12C0(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // esi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  if ( *(_DWORD *)(this + 108) )
    (*(void (__stdcall **)(_DWORD))(**(_DWORD **)(this + 108) + 4))(*(_DWORD *)(this + 108));
  v3 = *(_DWORD *)(this + 108);
  LeaveCriticalSection(v2);
  return v3;
}

// ===== sub_4C1330 @ 0x004C1330..0x004C14B5 =====
int __thiscall sub_4C1330(char *this, int a2)
{
  int v3; // eax

  v3 = (*(int (__thiscall **)(char *, int))(*(_DWORD *)this + 152))(this, a2);
  if ( v3 < 0 )
    return v3 != -2147220949 ? v3 : 0;
  if ( *((_DWORD *)this + 5) == 1 )
  {
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 80))(this);
    *((_DWORD *)this + 45) = 0;
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 124));
    if ( !*((_DWORD *)this + 5) )
      goto LABEL_5;
    *((_DWORD *)this + 45) = 1;
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
    (*(void (__thiscall **)(char *, int))(*(_DWORD *)this + 52))(this, a2);
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 148));
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 124));
    SetEvent(*((HANDLE *)this + 23));
  }
  if ( (*(int (__thiscall **)(char *))(*(_DWORD *)this + 44))(this) < 0 )
  {
    *((_DWORD *)this + 45) = 0;
    return 0;
  }
  (*(void (__thiscall **)(char *))(*(_DWORD *)this + 80))(this);
  *((_DWORD *)this + 45) = 0;
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 124));
  if ( *((_DWORD *)this + 5) )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
    (*(void (__thiscall **)(char *, _DWORD))(*(_DWORD *)this + 168))(this, *((_DWORD *)this + 27));
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 112))(this);
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 96))(this);
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 108))(this);
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 148));
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 124));
    return 0;
  }
LABEL_5:
  LeaveCriticalSection((LPCRITICAL_SECTION)(this + 124));
  return 0;
}

// ===== sub_4C14C0 @ 0x004C14C0..0x004C152E =====
int __thiscall sub_4C14C0(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // eax

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  v3 = *(_DWORD *)(this + 108);
  if ( v3 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    *(_DWORD *)(this + 108) = 0;
  }
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C1530 @ 0x004C1530..0x004C159B =====
void __thiscall sub_4C1530(char *this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  if ( *((_DWORD *)this + 48) )
  {
    *((_DWORD *)this + 48) = 0;
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 96))(this);
  }
  LeaveCriticalSection(v2);
}

// ===== sub_4C15A0 @ 0x004C15A0..0x004C1646 =====
int __thiscall sub_4C15A0(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v4; // ecx
  int v5; // esi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  if ( *(_DWORD *)(this + 100) )
  {
    v4 = *(_DWORD *)(this + 80);
    *(_DWORD *)(this + 192) = 0;
    if ( v4 )
      sub_4C49A0();
    *(_DWORD *)(this + 116) = 1;
    v5 = sub_4BD880((_DWORD *)this, 1, 0, (_DWORD *)(this + 12));
    LeaveCriticalSection(v2);
    return v5;
  }
  else
  {
    LeaveCriticalSection(v2);
    return 0;
  }
}

// ===== sub_4C1650 @ 0x004C1650..0x004C1670 =====
MMRESULT __thiscall sub_4C1650(UINT *this)
{
  MMRESULT result; // eax

  result = this[48];
  if ( result )
  {
    result = timeKillEvent(this[48]);
    this[48] = 0;
  }
  return result;
}

// ===== sub_4C1670 @ 0x004C1670..0x004C1727 =====
int __thiscall sub_4C1670(char *this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // esi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  if ( *((_DWORD *)this + 25) != 1 )
  {
    *((_DWORD *)this + 25) = 1;
    timeBeginPeriod(1u);
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 64))(this);
    if ( !*((_DWORD *)this + 27) )
    {
      v3 = (*(int (__thiscall **)(char *))(*(_DWORD *)this + 96))(this);
      LeaveCriticalSection(v2);
      return v3;
    }
    if ( !(*(int (__thiscall **)(char *, _DWORD))(*(_DWORD *)this + 84))(this, *((_DWORD *)this + 27)) )
      SetEvent(*((HANDLE *)this + 21));
  }
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C1730 @ 0x004C1730..0x004C17A3 =====
int __thiscall sub_4C1730(char *this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  bool v3; // zf

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  v3 = *((_DWORD *)this + 25) == 1;
  *((_DWORD *)this + 29) = 0;
  if ( v3 )
  {
    *((_DWORD *)this + 25) = 0;
    (*(void (__thiscall **)(char *))(*(_DWORD *)this + 68))(this);
    timeEndPeriod(1u);
  }
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C17B0 @ 0x004C17B0..0x004C17DA =====
void __thiscall sub_4C17B0(int this, int a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi

  v3 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  *(_DWORD *)(this + 176) = a2;
  LeaveCriticalSection(v3);
}

// ===== nullsub_4 @ 0x004C17E0..0x004C17E3 =====
void __stdcall nullsub_4(int a1)
{
  ;
}

// ===== sub_4C17F0 @ 0x004C17F0..0x004C183C =====
_DWORD *__thiscall sub_4C17F0(_DWORD *this, int a2, int a3, void *Src)
{
  sub_4C0380((int)this, 0, a2, a2 + 124, a3, Src);
  this[54] = a2;
  *this = &CRendererInputPin::`vftable';
  this[3] = &CRendererInputPin::`vftable';
  this[4] = &CRendererInputPin::`vftable';
  this[38] = &CRendererInputPin::`vftable';
  return this;
}

// ===== sub_4C1840 @ 0x004C1840..0x004C185C =====
int __stdcall sub_4C1840(int a1, int a2, int a3)
{
  return (***(int (__stdcall ****)(_DWORD, int, int))(a1 - 8))(*(_DWORD *)(a1 - 8), a2, a3);
}

// ===== sub_4C1860 @ 0x004C1860..0x004C1911 =====
int __stdcall sub_4C1860(int a1)
{
  struct _RTL_CRITICAL_SECTION *v2; // esi
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int v5; // [esp+2Ch] [ebp+8h]

  v2 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 124);
  EnterCriticalSection(v2);
  v3 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 148);
  EnterCriticalSection(v3);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 56))(a1 - 12);
  if ( !v5 )
  {
    v5 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 204) + 104))(*(_DWORD *)(a1 + 204));
    if ( v5 >= 0 )
      v5 = sub_44DAE0(a1);
  }
  LeaveCriticalSection(v3);
  LeaveCriticalSection(v2);
  return v5;
}

// ===== sub_4C1920 @ 0x004C1920..0x004C19C8 =====
int __stdcall sub_4C1920(int a1)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  int v3; // esi
  struct _RTL_CRITICAL_SECTION *v5; // [esp+10h] [ebp-10h]

  v2 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 124);
  EnterCriticalSection(v2);
  v5 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 148);
  EnterCriticalSection(v5);
  sub_4BEB60(a1);
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 204) + 132))(*(_DWORD *)(a1 + 204));
  LeaveCriticalSection(v5);
  v3 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 204) + 100))(*(_DWORD *)(a1 + 204));
  LeaveCriticalSection(v2);
  return v3;
}

// ===== sub_4C19D0 @ 0x004C19D0..0x004C1A72 =====
int __stdcall sub_4C19D0(int a1)
{
  struct _RTL_CRITICAL_SECTION *v2; // esi
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int v5; // [esp+2Ch] [ebp+8h]

  v2 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 124);
  EnterCriticalSection(v2);
  v3 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 204) + 148);
  EnterCriticalSection(v3);
  v5 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 204) + 136))(*(_DWORD *)(a1 + 204));
  if ( v5 >= 0 )
    v5 = sub_4BEB90(a1);
  LeaveCriticalSection(v3);
  LeaveCriticalSection(v2);
  return v5;
}

// ===== sub_4C1A80 @ 0x004C1A80..0x004C1B5C =====
int __stdcall sub_4C1A80(int a1, int a2)
{
  int result; // eax
  int v4; // edi
  struct _RTL_CRITICAL_SECTION *v5; // ebx
  _DWORD *v6; // ecx
  struct _RTL_CRITICAL_SECTION *v7; // edi
  int v8; // ecx
  int v9; // [esp+28h] [ebp+8h]

  result = (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(a1 + 64) + 156))(*(_DWORD *)(a1 + 64), a2);
  v4 = result;
  v9 = result;
  if ( result < 0 )
  {
    v5 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 64) + 124);
    EnterCriticalSection(v5);
    if ( *(_DWORD *)(*(_DWORD *)(a1 - 112) + 20) )
    {
      if ( !*(_BYTE *)(a1 + 9) )
      {
        v6 = *(_DWORD **)(a1 + 64);
        if ( !v6[24] && !*(_BYTE *)(a1 - 116) )
        {
          sub_4BD880(v6, 3, v4, 0);
          v7 = (struct _RTL_CRITICAL_SECTION *)(*(_DWORD *)(a1 + 64) + 148);
          EnterCriticalSection(v7);
          v8 = *(_DWORD *)(a1 + 64);
          if ( *(_DWORD *)(v8 + 100) )
          {
            if ( !*(_DWORD *)(v8 + 116) )
              sub_4C15A0(v8);
          }
          LeaveCriticalSection(v7);
          v4 = v9;
          *(_BYTE *)(a1 - 116) = 1;
        }
      }
    }
    LeaveCriticalSection(v5);
    return v4;
  }
  return result;
}

// ===== sub_4C1B60 @ 0x004C1B60..0x004C1B81 =====
int __thiscall sub_4C1B60(int this)
{
  int v1; // eax
  int result; // eax

  result = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 216) + 140))(*(_DWORD *)(this + 216));
  if ( result >= 0 )
  {
    v1 = *(_DWORD *)(this + 156);
    if ( v1 )
    {
      result = (*(int (__cdecl **)(int))(*(_DWORD *)v1 + 24))(v1);
      if ( result < 0 )
        return result;
      (*(void (__cdecl **)(_DWORD))(**(_DWORD **)(this + 156) + 8))(*(_DWORD *)(this + 156));
      *(_DWORD *)(this + 156) = 0;
    }
    return 0;
  }
  return result;
}

// ===== sub_4C1B90 @ 0x004C1B90..0x004C1BBD =====
int __thiscall sub_4C1B90(_DWORD **this, int a2)
{
  int result; // eax

  result = (*(int (__thiscall **)(_DWORD *, int))(*this[54] + 148))(this[54], a2);
  if ( result >= 0 )
    return sub_44DAE0(a2);
  return result;
}

// ===== sub_4C1BC0 @ 0x004C1BC0..0x004C1BFF =====
int __stdcall sub_4C1BC0(int a1, _DWORD *a2)
{
  LPVOID v3; // eax

  if ( !a2 )
    return -2147467261;
  v3 = CoTaskMemAlloc(8u);
  *a2 = v3;
  if ( !v3 )
    return -2147024882;
  sub_4C3400(v3, L"In");
  return 0;
}

// ===== sub_4C1C00 @ 0x004C1C00..0x004C1C14 =====
int __thiscall sub_4C1C00(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[54] + 176))(this[54]);
}

// ===== sub_4C1C20 @ 0x004C1C20..0x004C1C2D =====
int __thiscall sub_4C1C20(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[54] + 116))(this[54]);
}

// ===== sub_4C1C30 @ 0x004C1C30..0x004C1C41 =====
int __thiscall sub_4C1C30(int this)
{
  *(_BYTE *)(this + 36) = 0;
  return (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 216) + 120))(*(_DWORD *)(this + 216));
}

// ===== sub_4C1C50 @ 0x004C1C50..0x004C1C7B =====
int __thiscall sub_4C1C50(_DWORD **this, int a2)
{
  int result; // eax

  result = sub_4BE190(a2);
  if ( result >= 0 )
    return (*(int (__thiscall **)(_DWORD *, int))(*this[54] + 144))(this[54], a2);
  return result;
}

// ===== sub_4C1C80 @ 0x004C1C80..0x004C1D3F =====
int __thiscall sub_4C1C80(_DWORD *this)
{
  int result; // eax

  this[78] = -1000;
  this[79] = -1;
  this[86] = timeGetTime();
  result = 0;
  this[61] = 0;
  this[67] = -1;
  this[68] = 0;
  this[62] = 0;
  this[66] = 0;
  this[63] = 0;
  this[73] = 0;
  this[72] = 0;
  this[74] = 0;
  this[75] = 0;
  this[76] = 0;
  this[77] = 0;
  this[80] = 0;
  this[81] = 0;
  this[85] = 0;
  this[84] = 0;
  this[82] = 0;
  this[83] = 0;
  this[58] = 0;
  this[64] = 0;
  this[65] = -300000;
  this[60] = 0;
  this[70] = 0;
  this[71] = 0;
  return result;
}

// ===== sub_4C1D40 @ 0x004C1D40..0x004C1D4D =====
int __thiscall sub_4C1D40(void *this)
{
  (*(void (__thiscall **)(void *))(*(_DWORD *)this + 188))(this);
  return 0;
}

// ===== sub_4C1D50 @ 0x004C1D50..0x004C1D69 =====
int __thiscall sub_4C1D50(_DWORD *this)
{
  this[86] = timeGetTime() - this[86];
  return 0;
}

// ===== sub_4C1D70 @ 0x004C1D70..0x004C1D89 =====
int __thiscall sub_4C1D70(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[84] = a2;
  this[85] = a3;
  return result;
}

// ===== sub_4C1D90 @ 0x004C1D90..0x004C1E5C =====
int __thiscall sub_4C1D90(int this, int a2, int a3)
{
  __int64 v3; // rax
  int v4; // esi
  int v5; // edi
  bool v6; // cf
  int v7; // esi

  LODWORD(v3) = 1759218605 * a2;
  v4 = a2 / 10000;
  if ( a2 / 10000 > 1000 || v4 < -1000 )
  {
    if ( *(int *)(this + 292) > 1 )
    {
      LODWORD(v3) = v4 <= 0;
      v4 = (((_WORD)v3 - 1) & 0x7D0) - 1000;
    }
    else
    {
      v4 = 0;
    }
  }
  v5 = *(_DWORD *)(this + 292);
  if ( v5 > 1 )
  {
    v6 = __CFADD__(v4, *(_DWORD *)(this + 296));
    *(_DWORD *)(this + 296) += v4;
    *(_DWORD *)(this + 300) += (v4 >> 31) + v6;
    v3 = v4 * v4;
    *(_QWORD *)(this + 304) += v3;
  }
  if ( v5 > 2 )
  {
    v7 = a3 / 10000;
    if ( a3 / 10000 > 1000 || v7 < 0 )
      v7 = 1000;
    v3 = v7 * v7;
    v6 = __CFADD__((_DWORD)v3, *(_DWORD *)(this + 320));
    *(_DWORD *)(this + 320) += v3;
    *(_DWORD *)(this + 324) += HIDWORD(v3) + v6;
    LODWORD(v3) = v7;
    *(_QWORD *)(this + 328) += v7;
  }
  *(_DWORD *)(this + 292) = v5 + 1;
  return v3;
}

// ===== sub_4C1E60 @ 0x004C1E60..0x004C1E8C =====
void __thiscall sub_4C1E60(_DWORD *this)
{
  int v1; // ecx

  v1 = this[60];
  if ( v1 <= 0 )
    Sleep(0);
  else
    Sleep(v1 / 10000);
}

// ===== sub_4C1E90 @ 0x004C1E90..0x004C1ECC =====
void __thiscall sub_4C1E90(_DWORD *this, int a2)
{
  void (__thiscall *v3)(_DWORD *, int, int); // eax
  int v4; // [esp-8h] [ebp-Ch]
  int v5; // [esp-4h] [ebp-8h]

  v3 = *(void (__thiscall **)(_DWORD *, int, int))(*this + 180);
  v5 = this[85];
  v4 = this[84];
  this[61] = 0;
  this[62] = &loc_4C4B40;
  v3(this, v4, v5);
  sub_4C1E60(this);
}

// ===== sub_4C1ED0 @ 0x004C1ED0..0x004C1EFD =====
DWORD __thiscall sub_4C1ED0(_DWORD *this, int a2)
{
  DWORD result; // eax

  (*(void (__thiscall **)(_DWORD *, _DWORD, _DWORD))(*this + 180))(this, this[84], this[85]);
  result = timeGetTime();
  this[63] = result;
  return result;
}

// ===== sub_4C1F00 @ 0x004C1F00..0x004C1F55 =====
void __thiscall sub_4C1F00(_DWORD *this, int a2)
{
  DWORD Time; // eax
  int v4; // edx
  int v5; // ecx

  Time = timeGetTime();
  v4 = this[61];
  v5 = 10000 * (Time - this[63]);
  if ( v5 < 2 * v4 || v5 < 2 * this[62] )
    this[61] = (v4 + v5 + 2 * v4) / 4;
  this[62] = v5;
  sub_4C1E60(this);
}

// ===== sub_4C1F60 @ 0x004C1F60..0x004C1F72 =====
int __stdcall sub_4C1F60(int a1, int a2)
{
  *(_DWORD *)(a1 - 56) = a2;
  return 0;
}

// ===== sub_4C1F80 @ 0x004C1F80..0x004C1FBC =====
int __stdcall sub_4C1F80(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  if ( a4 < 1000 )
    *(_DWORD *)(a1 + 12) = 388880000 / (a4 + 167) - 330000;
  else
    *(_DWORD *)(a1 + 12) = 0;
  return 0;
}

// ===== sub_4C1FC0 @ 0x004C1FC0..0x004C2117 =====
int __thiscall sub_4C1FC0(_DWORD *this, __int64 a2, int a3, int a4)
{
  int v5; // eax
  int v6; // esi
  int v7; // ecx
  int v8; // ebx
  int v9; // eax
  int v10; // ecx
  BOOL v12; // [esp+Ch] [ebp-18h]
  int v13; // [esp+18h] [ebp-Ch]

  v5 = this[67];
  v12 = v5 >= 0 && v5 <= 2 * this[61];
  v6 = 1000;
  if ( v5 >= 0 )
  {
    if ( a2 <= 0 )
    {
      v7 = this[66];
      if ( v7 > 20000 && a2 < -20000 )
      {
        if ( v7 >= v5 || v5 + 20000 <= v7 || (v6 = 1000 * (v5 / (v5 - v7 + 20000)), v6 > 2000) )
          v6 = 2000;
      }
    }
    else
    {
      v6 = 1000 - a2 / 10000;
      if ( v6 < 500 )
        v6 = 500;
    }
  }
  v8 = a2 + this[61] / 2;
  v13 = (unsigned __int64)(a2 + this[61] / 2) >> 32;
  if ( !this[43] )
  {
    v9 = this[30];
    HIDWORD(a2) = 0;
    if ( (***(int (__stdcall ****)(_DWORD, _DWORD *, char *))(v9 + 24))(
           *(_DWORD *)(v9 + 24),
           dword_4DB9D4,
           (char *)&a2 + 4) >= 0 )
      this[43] = HIDWORD(a2);
  }
  v10 = this[43];
  if ( !v10 )
    return 1;
  HIDWORD(a2) = this + 3;
  return (*(int (__stdcall **)(int, _DWORD *, BOOL, int, int, int, int, int))(*(_DWORD *)v10 + 12))(
           v10,
           this + 3,
           v12,
           v6,
           v8,
           v13,
           a3,
           a4);
}

// ===== sub_4C2120 @ 0x004C2120..0x004C252B =====
int __thiscall sub_4C2120(int this, int a2, int *a3, _QWORD *a4)
{
  int v6; // eax
  __int64 v7; // kr18_8
  unsigned int v8; // eax
  int v9; // ecx
  unsigned __int64 v10; // kr30_8
  BOOL v11; // ecx
  int v12; // eax
  int v13; // ebx
  bool v14; // zf
  int v15; // ebx
  int v16; // ecx
  int v17; // ebx
  bool v18; // cf
  unsigned int v19; // ecx
  int v20; // eax
  int v21; // ecx
  BOOL v22; // edx
  int v24; // edx
  int v25; // edx
  int v26; // edx
  int v27; // eax
  int v28; // edx
  int v29; // ecx
  int v30; // ebx
  __int64 v31; // kr20_8
  int v32; // eax
  int v33; // eax
  __int64 v34; // kr28_8
  unsigned int v35; // [esp+Ch] [ebp-24h]
  int v36; // [esp+14h] [ebp-1Ch]
  __int64 v37; // [esp+1Ch] [ebp-14h] BYREF
  int v38; // [esp+24h] [ebp-Ch]
  int v39; // [esp+28h] [ebp-8h]
  int v40; // [esp+2Ch] [ebp-4h]
  int v41; // [esp+38h] [ebp+8h]
  int v42; // [esp+3Ch] [ebp+Ch]
  int v43; // [esp+40h] [ebp+10h]
  BOOL v44; // [esp+40h] [ebp+10h]

  if ( *(__int64 *)a3 >= 80000 )
  {
    *(_QWORD *)a3 -= 80000LL;
    *a4 -= 80000LL;
  }
  *(_DWORD *)(this + 280) = *a3;
  v6 = *(_DWORD *)(this + 24);
  *(_DWORD *)(this + 284) = a3[1];
  (*(void (__stdcall **)(int, __int64 *))(*(_DWORD *)v6 + 12))(v6, &v37);
  v7 = v37 - *(_QWORD *)(this + 32);
  v10 = v7 - *(_QWORD *)a3;
  v8 = HIDWORD(v10);
  v9 = v10;
  v37 = v7;
  if ( __SPAIR64__(v8, v9) >= -500000000 )
  {
    if ( __SPAIR64__(v8, v9) <= 500000000 )
      v42 = v9;
    else
      v42 = 500000000;
  }
  else
  {
    v42 = -500000000;
  }
  v11 = (*(int (__thiscall **)(int, int, int, _DWORD, _DWORD))(*(_DWORD *)this + 192))(
          this,
          v42,
          v42 >> 31,
          v7,
          HIDWORD(v37)) == 0;
  *(_DWORD *)(this + 236) = v11;
  v12 = *(_DWORD *)a4 - *a3;
  v13 = *(_DWORD *)(this + 272);
  v43 = v12;
  if ( v12 > v13 + v13 / 32 || v12 < v13 - v13 / 32 )
  {
    *(_DWORD *)(this + 268) = v12;
    *(_DWORD *)(this + 272) = v12;
  }
  if ( v11 && !(*(int (__stdcall **)(int))(*(_DWORD *)a2 + 60))(a2)
    || (v14 = *(_DWORD *)(this + 232) == -1, v40 = 0, v14) )
  {
    v40 = 1;
  }
  if ( v42 <= 0 )
  {
    v15 = *(_DWORD *)(this + 256);
    if ( v42 >= v15 || v40 )
      *(_DWORD *)(this + 256) = v42;
    else
      *(_DWORD *)(this + 256) = v15 - v15 / 8;
  }
  else
  {
    *(_DWORD *)(this + 256) = 0;
  }
  if ( v42 >= 0 )
    v16 = 0;
  else
    v16 = -v42;
  v38 = 3 * *(_DWORD *)(this + 264);
  v17 = (v16 + v38) / 4;
  v18 = (unsigned int)v37 < *(_DWORD *)(this + 312);
  v19 = v37 - *(_DWORD *)(this + 312);
  v39 = v17;
  v20 = HIDWORD(v37) - (v18 + *(_DWORD *)(this + 316));
  v35 = v19;
  v36 = v19;
  if ( v20 >= 0 && (v20 > 0 || v19 > 0x989680) )
  {
    v19 = 10000000;
    v36 = 10000000;
  }
  v41 = v19;
  v21 = v42;
  if ( 3 * *(_DWORD *)(this + 244) > *(_DWORD *)(this + 268) )
  {
    if ( *(_DWORD *)(this + 236) )
    {
      v22 = v42 <= 4 * v43;
    }
    else
    {
      v22 = 2 * v42 < v43;
      v21 = v42;
    }
    if ( !v22 && *(int *)(this + 264) <= 80000 && v20 <= 0 && (v20 < 0 || v35 <= 0x989680) )
    {
      *(_DWORD *)(this + 264) = v39;
      *(_DWORD *)(this + 232) = -1;
      return -2147467259;
    }
    v17 = v39;
  }
  v39 = 0;
  if ( v40 )
  {
    v24 = v43;
LABEL_43:
    v39 = 1;
    goto LABEL_44;
  }
  v24 = v43;
  if ( *(_DWORD *)(this + 268) > v43 + v43 / 16 && v21 > -10 * v43 )
    goto LABEL_43;
LABEL_44:
  if ( v21 >= -9000000 && v39 )
  {
    v25 = *(_DWORD *)(this + 268);
    *(_DWORD *)(this + 264) = v38 / 4;
    *(_DWORD *)(this + 232) = 0;
    *(_DWORD *)(this + 268) = (v25 + v36 + 2 * v25) / 4;
    sub_4C1D70((_DWORD *)this, v21, v36);
    v26 = HIDWORD(v37);
    *(_DWORD *)(this + 312) = v37;
    *(_DWORD *)(this + 316) = v26;
    if ( *(_DWORD *)(this + 256) > v42 )
      *(_DWORD *)(this + 256) = v42;
    return 0;
  }
  else
  {
    v27 = *(_DWORD *)(this + 256);
    ++*(_DWORD *)(this + 232);
    *(_DWORD *)(this + 268) = v24;
    v28 = -v24;
    if ( v27 < v28 )
      v27 = v28;
    *(_QWORD *)a3 += v27;
    v29 = -v21;
    *(_DWORD *)(this + 264) = v17;
    v44 = v29 > 0;
    if ( v44 )
    {
      v30 = *a3;
      v31 = *(_QWORD *)a3 - *(_QWORD *)(this + 312);
      if ( v31 >= -500000000 )
      {
        v32 = a3[1];
        if ( v31 <= 500000000 )
          v41 = v31;
        else
          v41 = 500000000;
        *(_DWORD *)(this + 312) = v30;
      }
      else
      {
        v32 = a3[1];
        v41 = -500000000;
        *(_DWORD *)(this + 312) = v30;
      }
    }
    else
    {
      v32 = HIDWORD(v37);
      *(_DWORD *)(this + 312) = v37;
    }
    *(_DWORD *)(this + 316) = v32;
    if ( v29 <= 0 )
    {
      v33 = v42;
    }
    else
    {
      v34 = *(_QWORD *)a3 - *(_QWORD *)(this + 280);
      v33 = v34;
      if ( v34 >= -500000000 )
      {
        if ( v34 > 500000000 )
          v33 = 500000000;
      }
      else
      {
        v33 = -500000000;
      }
    }
    sub_4C1D70((_DWORD *)this, v33, v41);
    return v44;
  }
}

// ===== sub_4C2530 @ 0x004C2530..0x004C2558 =====
BOOL __thiscall sub_4C2530(_DWORD *this, int a2)
{
  BOOL result; // eax

  result = sub_4C11C0(this, a2);
  if ( result )
    return 1;
  ++this[72];
  return result;
}

// ===== sub_4C2560 @ 0x004C2560..0x004C2599 =====
int __stdcall sub_4C2560(int a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  *a2 = *(_DWORD *)(a1 + 64);
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  return 0;
}

// ===== sub_4C25A0 @ 0x004C25A0..0x004C25D9 =====
int __stdcall sub_4C25A0(int a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  *a2 = *(_DWORD *)(a1 + 68);
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  return 0;
}

// ===== sub_4C25E0 @ 0x004C25E0..0x004C2654 =====
int __stdcall sub_4C25E0(int a1, int *a2)
{
  int v3; // eax

  if ( !a2 )
    return -2147467261;
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  if ( *(_DWORD *)(a1 - 124) )
    v3 = timeGetTime() - *(_DWORD *)(a1 + 120);
  else
    v3 = *(_DWORD *)(a1 + 120);
  if ( v3 > 0 )
    *a2 = MulDiv(100000, *(_DWORD *)(a1 + 68), v3);
  else
    *a2 = 0;
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  return 0;
}

// ===== sub_4C2660 @ 0x004C2660..0x004C26CE =====
int __stdcall sub_4C2660(int a1, _DWORD *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // ebx
  int v4; // eax

  if ( !a2 )
    return -2147467261;
  v3 = (struct _RTL_CRITICAL_SECTION *)(a1 - 100);
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 - 100));
  if ( *(_DWORD *)(a1 - 200) && (v4 = *(_DWORD *)(a1 + 68), v4 > 1) )
  {
    *a2 = *(_QWORD *)(a1 + 72) / (v4 - 1);
    LeaveCriticalSection(v3);
    return 0;
  }
  else
  {
    *a2 = 0;
    LeaveCriticalSection(v3);
    return 0;
  }
}

// ===== sub_4C26D0 @ 0x004C26D0..0x004C2734 =====
int __cdecl sub_4C26D0(int a1)
{
  int v1; // ecx
  int result; // eax

  v1 = 1;
  if ( a1 > 0x40000000 )
    return 0x8000;
  if ( a1 > 1 )
  {
    do
      v1 *= 2;
    while ( v1 * v1 < a1 );
  }
  if ( !a1 )
    return 0;
  result = (a1 + v1 * v1) / (2 * v1);
  if ( result >= 0 )
  {
    result = (a1 + result * result) / (2 * result);
    if ( result >= 0 )
      return (a1 + result * result) / (2 * result);
  }
  return result;
}

// ===== sub_4C2740 @ 0x004C2740..0x004C280E =====
int __thiscall sub_4C2740(int this, int a2, int *a3, __int64 a4, int a5, int a6)
{
  struct _RTL_CRITICAL_SECTION *v8; // ebx
  __int64 v9; // kr00_8

  if ( !a3 )
    return -2147467261;
  v8 = (struct _RTL_CRITICAL_SECTION *)(this + 124);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 124));
  if ( *(_DWORD *)(this + 24) && a2 > 1 )
  {
    v9 = a4 - sub_4C34E0(a5, a6, a5, a6, a2, a2 >> 31, 0, 0);
    *a3 = sub_4C26D0(v9 / (a2 - 1));
  }
  else
  {
    *a3 = 0;
  }
  LeaveCriticalSection(v8);
  return 0;
}

// ===== sub_4C2810 @ 0x004C2810..0x004C283E =====
int __stdcall sub_4C2810(int a1, int *a2)
{
  return sub_4C2740(
           a1 - 224,
           *(_DWORD *)(a1 + 68) - 1,
           a2,
           *(_QWORD *)(a1 + 80),
           *(_DWORD *)(a1 + 72),
           *(_DWORD *)(a1 + 76));
}

// ===== sub_4C2840 @ 0x004C2840..0x004C2870 =====
int __stdcall sub_4C2840(int a1, int *a2)
{
  return sub_4C2740(
           a1 - 224,
           *(_DWORD *)(a1 + 68) - 2,
           a2,
           *(_QWORD *)(a1 + 96),
           *(_DWORD *)(a1 + 104),
           *(_DWORD *)(a1 + 108));
}

// ===== sub_4C2870 @ 0x004C2870..0x004C2906 =====
int __stdcall sub_4C2870(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB894) )
  {
    if ( a1 )
      return sub_4BCF30(a1 + 224, a3);
    return sub_4BCF30(0, a3);
  }
  if ( !sub_445510(a2, dword_4DB9D4) )
    return sub_4C0DF0(a1, a2, a3);
  if ( !a1 )
    return sub_4BCF30(0, a3);
  return sub_4BCF30(a1 + 228, a3);
}

// ===== sub_4C2910 @ 0x004C2910..0x004C2961 =====
int __stdcall sub_4C2910(
        LPCRITICAL_SECTION lpCriticalSection,
        int (__stdcall ***a2)(_DWORD, void *, LONG *),
        void *Src)
{
  int (__stdcall ***v3)(_DWORD, void *, LONG *); // edi

  v3 = a2;
  if ( !a2 && lpCriticalSection[2].LockCount )
  {
    ((void (__stdcall *)(LPCRITICAL_SECTION, _DWORD *, int (__stdcall ****)(_DWORD, void *, LONG *)))lpCriticalSection->DebugInfo->Type)(
      lpCriticalSection,
      dword_4DB944,
      &a2);
    sub_4BD880(&lpCriticalSection[-1].OwningThread, 21, (int)a2, 0);
    ((void (__stdcall *)(int (__stdcall ***)(_DWORD, void *, LONG *)))(*a2)[2])(a2);
  }
  return sub_4BD790(lpCriticalSection, v3, Src);
}

// ===== sub_4C2970 @ 0x004C2970..0x004C297D =====
int __stdcall sub_4C2970(int a1, int a2, int a3)
{
  return sub_4C1840(a1 - 140, a2, a3);
}

// ===== sub_4C2980 @ 0x004C2980..0x004C298D =====
int __stdcall sub_4C2980(int a1)
{
  return sub_4BF8F0(a1 - 140);
}

// ===== sub_4C2990 @ 0x004C2990..0x004C299D =====
int __stdcall sub_4C2990(int a1)
{
  return sub_45B670(a1 - 140);
}

// ===== sub_4C29A0 @ 0x004C29A0..0x004C2A83 =====
HMODULE __thiscall sub_4C29A0(int this)
{
  int v2; // eax
  int v3; // ecx

  *(_DWORD *)this = &CBaseRenderer::`vftable';
  *(_DWORD *)(this + 12) = &CBaseRenderer::`vftable';
  *(_DWORD *)(this + 16) = &CBaseRenderer::`vftable';
  sub_4C1730((char *)this);
  sub_4C14C0(this);
  v2 = *(_DWORD *)(this + 80);
  if ( v2 )
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)(v2 + 8) + 12))(v2 + 8, 1);
    *(_DWORD *)(this + 80) = 0;
  }
  v3 = *(_DWORD *)(this + 120);
  if ( v3 )
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 12))(v3, 1);
    *(_DWORD *)(this + 120) = 0;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 196));
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 148));
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 124));
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam *)(this + 92));
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam *)(this + 88));
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam *)(this + 84));
  return sub_4BD150(this);
}

// ===== sub_4C2A90 @ 0x004C2A90..0x004C2AF7 =====
int __thiscall sub_4C2A90(_DWORD *this)
{
  void *v2; // eax
  void *v3; // ecx
  int v4; // edx
  void (__thiscall *v5)(_DWORD *); // eax
  DWORD v6; // esi
  HANDLE Handles[2]; // [esp+8h] [ebp-8h] BYREF

  v2 = (void *)this[22];
  v3 = (void *)this[21];
  v4 = *this;
  Handles[0] = v2;
  v5 = *(void (__thiscall **)(_DWORD *))(v4 + 72);
  Handles[1] = v3;
  v5(this);
  do
    v6 = WaitForMultipleObjects(2u, Handles, 0, 0x2710u);
  while ( v6 == 258 );
  (*(void (__thiscall **)(_DWORD *))(*this + 76))(this);
  if ( !v6 )
    return -2147220957;
  sub_4C1170(this);
  return 0;
}

// ===== sub_4C2B00 @ 0x004C2B00..0x004C2BD4 =====
int __stdcall sub_4C2B00(int a1)
{
  int v1; // ecx
  int v2; // eax

  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 112));
  if ( *(_DWORD *)(a1 + 8) )
  {
    if ( *(_DWORD *)(*(_DWORD *)(a1 + 108) + 24) )
    {
      sub_4BD350(a1);
      v1 = *(_DWORD *)(a1 + 108);
      v2 = *(_DWORD *)(v1 + 156);
      if ( v2 )
        (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 24))(*(_DWORD *)(v1 + 156));
      sub_4C17B0(a1 - 12, 1);
      (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 128))(a1 - 12);
      (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)(a1 - 12) + 40))(a1 - 12, 0);
      (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 100))(a1 - 12);
      (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 108))(a1 - 12);
      SetEvent(*(HANDLE *)(a1 + 80));
      sub_4C0E80((_DWORD *)(a1 - 12));
      *(_DWORD *)(a1 + 84) = 0;
    }
    else
    {
      *(_DWORD *)(a1 + 8) = 0;
    }
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 112));
  return 0;
}

// ===== sub_4C2BE0 @ 0x004C2BE0..0x004C2CF9 =====
int __stdcall sub_4C2BE0(int a1)
{
  struct _RTL_CRITICAL_SECTION *v2; // ebx
  int v3; // eax
  int (__thiscall *v4)(int, int); // eax
  int v5; // esi
  int v6; // ecx
  int v7; // eax
  void (__thiscall *v8)(int); // eax
  int v10; // [esp+28h] [ebp+8h]

  v2 = (struct _RTL_CRITICAL_SECTION *)(a1 + 112);
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 112));
  v10 = *(_DWORD *)(a1 + 8);
  if ( v10 == 1 )
  {
    v3 = (*(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 48))(a1 - 12, 1);
LABEL_11:
    v5 = v3;
    goto LABEL_12;
  }
  if ( !*(_DWORD *)(*(_DWORD *)(a1 + 108) + 24) )
  {
    v4 = *(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 48);
    *(_DWORD *)(a1 + 8) = 1;
    v3 = v4(a1 - 12, 1);
    goto LABEL_11;
  }
  v5 = sub_4BD400(a1);
  if ( v5 >= 0 )
  {
    sub_4C17B0(a1 - 12, 1);
    (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 128))(a1 - 12);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 40))(a1 - 12, 1);
    (*(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 108))(a1 - 12);
    sub_4C1650((UINT *)(a1 - 12));
    v6 = *(_DWORD *)(a1 + 108);
    v7 = *(_DWORD *)(v6 + 156);
    if ( v7 )
      (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v7 + 20))(*(_DWORD *)(v6 + 156));
    if ( !v10 )
    {
      v8 = *(void (__thiscall **)(int))(*(_DWORD *)(a1 - 12) + 112);
      *(_DWORD *)(a1 + 84) = 0;
      v8(a1 - 12);
    }
    v3 = (*(int (__thiscall **)(int, int))(*(_DWORD *)(a1 - 12) + 48))(a1 - 12, v10);
    goto LABEL_11;
  }
LABEL_12:
  LeaveCriticalSection(v2);
  return v5;
}

// ===== sub_4C2D00 @ 0x004C2D00..0x004C2E34 =====
int __stdcall sub_4C2D00(LPCRITICAL_SECTION lpCriticalSection, ULONG_PTR a2, struct _RTL_CRITICAL_SECTION_DEBUG *a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // ebx
  int v6; // edi
  _DWORD *OwningThread; // ecx
  int v8; // eax
  void (__thiscall *v9)(HANDLE *); // eax
  int v10; // esi
  LPCRITICAL_SECTION lpCriticalSectiona; // [esp+28h] [ebp+8h]

  v4 = (LPCRITICAL_SECTION)((char *)lpCriticalSection + 112);
  EnterCriticalSection((LPCRITICAL_SECTION)((char *)lpCriticalSection + 112));
  lpCriticalSectiona = (LPCRITICAL_SECTION)lpCriticalSection->RecursionCount;
  if ( lpCriticalSectiona == (LPCRITICAL_SECTION)2 )
    goto LABEL_4;
  if ( !*((_DWORD *)lpCriticalSection[4].OwningThread + 6) )
  {
    sub_4BD880(
      &lpCriticalSection[-1].OwningThread,
      1,
      0,
      lpCriticalSection != (LPCRITICAL_SECTION)12 ? lpCriticalSection : 0);
    lpCriticalSection->RecursionCount = 2;
LABEL_4:
    LeaveCriticalSection(v4);
    return 0;
  }
  SetEvent((HANDLE)lpCriticalSection[3].RecursionCount);
  v6 = sub_4BD4D0(lpCriticalSection, a2, a3);
  if ( v6 >= 0 )
  {
    (*((void (__thiscall **)(HANDLE *, int))lpCriticalSection[-1].OwningThread + 10))(
      &lpCriticalSection[-1].OwningThread,
      1);
    sub_4C17B0((int)&lpCriticalSection[-1].OwningThread, 0);
    OwningThread = lpCriticalSection[4].OwningThread;
    v8 = OwningThread[39];
    if ( v8 )
      (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v8 + 20))(OwningThread[39]);
    if ( !lpCriticalSectiona )
    {
      v9 = (void (__thiscall *)(HANDLE *))*((_DWORD *)lpCriticalSection[-1].OwningThread + 28);
      lpCriticalSection[3].OwningThread = 0;
      v9(&lpCriticalSection[-1].OwningThread);
    }
    v10 = (*((int (__thiscall **)(HANDLE *))lpCriticalSection[-1].OwningThread + 31))(&lpCriticalSection[-1].OwningThread);
    LeaveCriticalSection(v4);
    return v10;
  }
  else
  {
    LeaveCriticalSection(v4);
    return v6;
  }
}

// ===== sub_4C2E40 @ 0x004C2E40..0x004C2F13 =====
int __thiscall sub_4C2E40(int this, int a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  int v7; // esi

  v3 = (struct _RTL_CRITICAL_SECTION *)(this + 196);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 196));
  if ( a2 )
    goto LABEL_9;
  if ( !*(_DWORD *)(this + 120) )
  {
    a2 = 0;
    v4 = operator new(0xE0u);
    if ( v4 )
      v5 = sub_4C17F0(v4, this, (int)&a2, L"In");
    else
      v5 = 0;
    *(_DWORD *)(this + 120) = v5;
    if ( !v5 )
      goto LABEL_9;
    if ( a2 < 0 )
    {
      (*(void (__thiscall **)(_DWORD *, int))(*v5 + 12))(v5, 1);
      *(_DWORD *)(this + 120) = 0;
LABEL_9:
      LeaveCriticalSection(v3);
      return 0;
    }
  }
  v7 = *(_DWORD *)(this + 120);
  LeaveCriticalSection(v3);
  return v7;
}

// ===== sub_4C2F20 @ 0x004C2F20..0x004C2F5B =====
int __thiscall sub_4C2F20(_DWORD *this, int a2)
{
  int v3; // ecx
  int result; // eax

  this[24] = 0;
  if ( sub_4A4EA0(this) == 2 )
  {
    result = (*(int (**)(void))(*this + 124))();
    if ( result >= 0 )
    {
      sub_4C17B0((int)this, 0);
      return 0;
    }
  }
  else
  {
    sub_4C17B0(v3, 1);
    return 0;
  }
  return result;
}

// ===== sub_4C2F60 @ 0x004C2F60..0x004C2FDA =====
int __thiscall sub_4C2F60(_DWORD *this)
{
  int v2; // eax
  int v3; // eax
  bool v5; // zf

  v2 = this[43];
  if ( v2 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v2 + 8))(v2);
    this[43] = 0;
  }
  v3 = this[30];
  if ( !*(_DWORD *)(v3 + 24) )
    return 1;
  if ( this[5] && !*(_BYTE *)(v3 + 37) )
    return -2147220956;
  sub_4C17B0((int)this, 0);
  (*(void (__thiscall **)(_DWORD *))(*this + 100))(this);
  (*(void (__thiscall **)(_DWORD *))(*this + 112))(this);
  v5 = this[5] == 2;
  this[24] = 0;
  if ( v5 )
    (*(void (__thiscall **)(_DWORD *))(*this + 128))(this);
  return 0;
}

// ===== sub_4C2FE0 @ 0x004C2FE0..0x004C31C0 =====
int __thiscall sub_4C2FE0(char *this, int a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  _DWORD *v5; // ecx
  int v6; // ebx
  struct _RTL_CRITICAL_SECTION *v7; // ebx
  int v8; // eax
  _DWORD *v9; // [esp-8h] [ebp-2Ch]

  v3 = (struct _RTL_CRITICAL_SECTION *)(this + 124);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 124));
  v9 = (_DWORD *)(*((_DWORD *)this + 30) + 152);
  *((_DWORD *)this + 45) = 1;
  if ( sub_4BE8E0(0, (int)this, v9, a2) )
  {
    *((_DWORD *)this + 45) = 0;
    LeaveCriticalSection(v3);
    return -2147467259;
  }
  else
  {
    v5 = (_DWORD *)*((_DWORD *)this + 30);
    if ( v5[51] && (v6 = (*(int (__thiscall **)(_DWORD *, _DWORD))(*v5 + 36))(v5, v5[51]), v6 < 0) )
    {
      *((_DWORD *)this + 45) = 0;
      LeaveCriticalSection(v3);
      return v6;
    }
    else
    {
      v7 = (struct _RTL_CRITICAL_SECTION *)(this + 148);
      EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
      if ( *((_DWORD *)this + 27) || *((_DWORD *)this + 28) || *((_DWORD *)this + 24) )
      {
        SetEvent(*((HANDLE *)this + 23));
        *((_DWORD *)this + 45) = 0;
        LeaveCriticalSection(v7);
        LeaveCriticalSection(v3);
        return -2147418113;
      }
      else
      {
        if ( *((_DWORD *)this + 20) )
          sub_4C47F0(a2);
        if ( *((_DWORD *)this + 25) != 1 || (*(int (__thiscall **)(char *, int))(*(_DWORD *)this + 84))(this, a2) )
        {
          v8 = *((_DWORD *)this + 30);
          *((_DWORD *)this + 46) = *(_DWORD *)(v8 + 192);
          *((_DWORD *)this + 47) = *(_DWORD *)(v8 + 196);
          *((_DWORD *)this + 27) = a2;
          (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 4))(a2);
          if ( !*((_DWORD *)this + 25) )
            sub_4C17B0((int)this, 1);
          LeaveCriticalSection(v7);
          LeaveCriticalSection(v3);
          return 0;
        }
        else
        {
          *((_DWORD *)this + 45) = 0;
          LeaveCriticalSection(v7);
          LeaveCriticalSection(v3);
          return -2147220949;
        }
      }
    }
  }
}

// ===== fptc @ 0x004C31C0..0x004C31CF =====
void __stdcall fptc(UINT uTimerID, UINT uMsg, char *dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
  sub_4C1530(dwUser);
}

// ===== sub_4C31D0 @ 0x004C31D0..0x004C3260 =====
int __thiscall sub_4C31D0(int dwUser)
{
  DWORD_PTR v1; // esi
  int v2; // eax
  __int64 v3; // kr00_8
  __int64 v4; // rax
  int v6; // eax
  __int64 v7; // [esp+4h] [ebp-8h] BYREF

  v1 = dwUser;
  if ( !*(_DWORD *)(dwUser + 112) || *(_DWORD *)(dwUser + 116) || *(_DWORD *)(dwUser + 192) )
    return 0;
  v2 = *(_DWORD *)(dwUser + 24);
  if ( !v2 )
    return sub_4C15A0(dwUser);
  v3 = *(_QWORD *)(dwUser + 32) + *(_QWORD *)(dwUser + 184);
  (*(void (__stdcall **)(int, __int64 *))(*(_DWORD *)v2 + 12))(v2, &v7);
  v4 = (v3 - v7) / 10000;
  if ( (int)v4 < 50 || (v6 = sub_4C3A40(v4, 0xAu, (LPTIMECALLBACK)fptc, v1, 0), (*(_DWORD *)(v1 + 192) = v6) == 0) )
  {
    dwUser = v1;
    return sub_4C15A0(dwUser);
  }
  return 0;
}

// ===== sub_4C3260 @ 0x004C3260..0x004C3296 =====
int __thiscall sub_4C3260(int this)
{
  sub_4C1650((UINT *)this);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 148));
  *(_DWORD *)(this + 112) = 0;
  *(_DWORD *)(this + 116) = 0;
  *(_DWORD *)(this + 184) = 0;
  *(_DWORD *)(this + 188) = 0;
  LeaveCriticalSection((LPCRITICAL_SECTION)(this + 148));
  return 0;
}

// ===== sub_4C32A0 @ 0x004C32A0..0x004C32C1 =====
void *__thiscall sub_4C32A0(void *this, char a2)
{
  sub_4BE670((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C32D0 @ 0x004C32D0..0x004C3337 =====
_DWORD *__thiscall sub_4C32D0(_DWORD *this, _DWORD *a2, int a3, _DWORD *a4, int a5)
{
  sub_4C0BB0((int)this, a2, a3, a4, a5);
  *this = &CBaseVideoRenderer::`vftable';
  this[3] = &CBaseVideoRenderer::`vftable';
  this[4] = &CBaseVideoRenderer::`vftable';
  this[56] = &CBaseVideoRenderer::`vftable';
  this[57] = &CBaseVideoRenderer::`vftable';
  this[59] = 0;
  this[72] = 0;
  this[73] = 0;
  sub_4C1C80(this);
  return this;
}

// ===== sub_4C3340 @ 0x004C3340..0x004C336D =====
HMODULE __thiscall sub_4C3340(int this)
{
  *(_DWORD *)this = &CBaseVideoRenderer::`vftable';
  *(_DWORD *)(this + 12) = &CBaseVideoRenderer::`vftable';
  *(_DWORD *)(this + 16) = &CBaseVideoRenderer::`vftable';
  *(_DWORD *)(this + 224) = &CBaseVideoRenderer::`vftable';
  *(_DWORD *)(this + 228) = &CBaseVideoRenderer::`vftable';
  return sub_4C29A0(this);
}

// ===== sub_4C3370 @ 0x004C3370..0x004C3391 =====
void *__thiscall sub_4C3370(void *this, char a2)
{
  sub_4C29A0((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C33A0 @ 0x004C33A0..0x004C33C1 =====
void *__thiscall sub_4C33A0(void *this, char a2)
{
  sub_4C3340((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C33D0 @ 0x004C33D0..0x004C33EF =====
HANDLE *__thiscall sub_4C33D0(HANDLE *this, BOOL bManualReset)
{
  *this = CreateEventA(0, bManualReset, 0, 0);
  return this;
}

// ===== ??1InitialThreadParam@UMSFreeVirtualProcessorRoot@details@Concurrency@@QAE@XZ @ 0x004C33F0..0x004C33FE =====
void __thiscall Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam(
        HANDLE *this)
{
  if ( *this )
    CloseHandle(*this);
}

// ===== sub_4C3400 @ 0x004C3400..0x004C3424 =====
unsigned __int16 *__stdcall sub_4C3400(unsigned __int16 *a1, const unsigned __int16 *a2)
{
  return wcscpy(a1, a2);
}

// ===== sub_4C3430 @ 0x004C3430..0x004C3469 =====
_WORD *__stdcall sub_4C3430(_WORD *a1, int a2, int a3)
{
  _WORD *v3; // ecx
  int v4; // edi
  _WORD *result; // eax
  __int16 v6; // dx

  v3 = a1;
  v4 = a3;
  result = a1;
  if ( a3 )
  {
    while ( --v4 )
    {
      v6 = *(_WORD *)((char *)v3 + a2 - (_DWORD)a1);
      *v3++ = v6;
      if ( !v6 )
        return result;
    }
    *v3 = 0;
  }
  return result;
}

// ===== sub_4C3470 @ 0x004C3470..0x004C34B5 =====
int __stdcall sub_4C3470(unsigned __int16 *a1, unsigned __int16 *a2)
{
  __int16 v4; // ax
  __int16 v5; // cx
  int v6; // edx
  int v7; // esi

  while ( 1 )
  {
    v4 = *a1;
    v5 = *a2;
    v6 = *a1;
    v7 = *a2;
    if ( (_WORD)v6 != (_WORD)v7 )
      break;
    ++a1;
    if ( v4 )
    {
      ++a2;
      if ( v5 )
        continue;
    }
    return 0;
  }
  return v6 - v7;
}

// ===== sub_4C34C0 @ 0x004C34C0..0x004C34DC =====
int __stdcall sub_4C34C0(int a1)
{
  int result; // eax

  result = -1;
  do
    ++result;
  while ( *(_WORD *)(a1 + 2 * result) );
  return result;
}

// ===== sub_4C34E0 @ 0x004C34E0..0x004C3774 =====
int __stdcall sub_4C34E0(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  unsigned __int64 v4; // kr10_8
  unsigned __int64 v5; // kr18_8
  signed __int64 v6; // rdi
  unsigned __int64 v7; // rax
  int v8; // esi
  __int64 v9; // kr30_8
  unsigned int v10; // ebx
  unsigned __int64 v11; // kr40_8
  unsigned int v12; // ecx
  unsigned __int64 v13; // rax
  int v14; // ebx
  int v15; // ebx
  int v16; // ebx
  unsigned int v17; // ecx
  unsigned __int64 v19; // [esp+Ch] [ebp-30h]
  __int64 v20; // [esp+24h] [ebp-18h]
  unsigned __int64 v21; // [esp+34h] [ebp-8h]
  unsigned int v22; // [esp+44h] [ebp+8h]
  int v23; // [esp+50h] [ebp+14h]
  int v24; // [esp+58h] [ebp+1Ch]

  v4 = abs64(a1);
  v5 = abs64(a2);
  v21 = abs64(a3);
  v23 = (a1 < 0) ^ (a2 < 0);
  v19 = (unsigned int)v4 * (unsigned __int64)(unsigned int)v5;
  HIDWORD(v6) = (v5 * v4) >> 32;
  v22 = HIDWORD(v6);
  v7 = ((HIDWORD(v4) * (unsigned __int64)(unsigned int)v5
       + HIDWORD(v5) * (unsigned __int64)(unsigned int)v4
       + HIDWORD(v19)) >> 32)
     + HIDWORD(v5) * (unsigned __int64)HIDWORD(v4);
  if ( !a4 )
    goto LABEL_9;
  v8 = 0;
  if ( v23 )
  {
    v9 = -a4;
    if ( a4 <= 0 )
      goto LABEL_7;
  }
  else
  {
    v9 = a4;
    if ( a4 >= 0 )
      goto LABEL_7;
  }
  v8 = -1;
LABEL_7:
  HIDWORD(v20) = v8;
  LODWORD(v20) = v8;
  v10 = ((unsigned int)v9 + (unsigned __int64)(unsigned int)v19) >> 32;
  LODWORD(v19) = v9 + v19;
  v11 = v10 + v22 + (unsigned __int64)HIDWORD(v9);
  HIDWORD(v6) = v10 + v22 + HIDWORD(v9);
  v7 += v20 + HIDWORD(v11);
  if ( (v7 & 0x8000000000000000uLL) != 0LL )
  {
    v23 = v23 == 0;
    HIDWORD(v6) = (__PAIR64__(~(_DWORD)v11, ~(_DWORD)v19) + 1) >> 32;
    LODWORD(v19) = -(int)v19;
    v7 = (__PAIR64__(HIDWORD(v6), v19) == 0) + ~v7;
  }
LABEL_9:
  if ( a3 < 0 )
    v23 = v23 == 0;
  if ( HIDWORD(v21) > HIDWORD(v7) )
  {
    v12 = v21;
  }
  else if ( HIDWORD(v21) < HIDWORD(v7) || (v12 = v21, (unsigned int)v21 <= (unsigned int)v7) )
  {
    if ( v23 )
      LODWORD(v13) = 0;
    else
      LODWORD(v13) = -1;
    return v13;
  }
  v14 = HIDWORD(v7) | v7;
  if ( v7 )
  {
    if ( HIDWORD(v21) )
    {
      v16 = 0;
      v17 = 0;
      v24 = 64;
      do
      {
        v17 = __PAIR64__(v17, v16) >> 31;
        v16 *= 2;
        v7 *= 2LL;
        if ( v6 < 0 )
          LODWORD(v7) = v7 + 1;
        v6 = 2 * __PAIR64__(HIDWORD(v6), v19);
        LODWORD(v19) = 2 * v19;
        if ( v21 <= v7 )
        {
          v7 -= v21;
          v17 = (__PAIR64__(v17, v16++) + 1) >> 32;
        }
        --v24;
      }
      while ( v24 );
      if ( v23 )
        v16 = -v16;
      LODWORD(v13) = v16;
    }
    else
    {
      v15 = __PAIR64__(__PAIR64__(v7, HIDWORD(v6)) % v12, v19) / (unsigned int)v21;
      if ( v23 )
        v15 = -(int)(__PAIR64__(__PAIR64__(v7, HIDWORD(v6)) % v12, v19) / (unsigned int)v21);
      LODWORD(v13) = v15;
    }
  }
  else
  {
    v13 = __PAIR64__(HIDWORD(v6), v19) / __PAIR64__(HIDWORD(v21), v12);
    if ( v23 != v14 )
      LODWORD(v13) = -(int)v13;
  }
  return v13;
}

// ===== sub_4C3780 @ 0x004C3780..0x004C37D1 =====
int __stdcall sub_4C3780(void *Src, _DWORD *a2)
{
  SIZE_T v3; // esi
  void *v4; // eax

  if ( !a2 )
    return -2147467261;
  v3 = 2 * sub_4C34C0((int)Src) + 2;
  v4 = CoTaskMemAlloc(v3);
  *a2 = v4;
  if ( !v4 )
    return -2147024882;
  memcpy_0(v4, Src, v3);
  return 0;
}

// ===== sub_4C37E0 @ 0x004C37E0..0x004C39C7 =====
DWORD __stdcall sub_4C37E0(void *TickCount, DWORD dwMilliseconds, HWND hWnd, UINT wMsgFilterMin, DWORD dwWakeMask)
{
  DWORD v5; // ebx
  unsigned int v6; // esi
  DWORD result; // eax
  DWORD v8; // eax
  DWORD v9; // edi
  _BYTE *v10; // eax
  HANDLE CurrentThread; // eax
  HANDLE v12; // eax
  HANDLE v13; // eax
  UINT v14; // eax
  bool v15; // zf
  DWORD CurrentThreadId; // eax
  UINT v17; // [esp-10h] [ebp-48h]
  int v18; // [esp-8h] [ebp-40h]
  struct tagMSG Msg; // [esp+8h] [ebp-30h] BYREF
  HANDLE Handles[2]; // [esp+24h] [ebp-14h] BYREF
  int nPriority; // [esp+2Ch] [ebp-Ch]
  DWORD nCount; // [esp+30h] [ebp-8h]
  int v23; // [esp+34h] [ebp-4h]

  v5 = dwMilliseconds;
  v23 = 0;
  Handles[0] = TickCount;
  Handles[1] = (HANDLE)dwWakeMask;
  if ( dwMilliseconds != -1 && dwMilliseconds )
    TickCount = (void *)GetTickCount();
  v6 = (dwWakeMask != 0) + 1;
  nCount = v6;
  result = WaitForMultipleObjects(v6, Handles, 0, 0);
  if ( result >= v6 )
  {
    while ( 1 )
    {
      if ( v5 > 0xA )
        v5 = 10;
      v8 = MsgWaitForMultipleObjects(v6, Handles, 0, v5, 8 * (hWnd != 0) + 64);
      v9 = v8;
      if ( v8 != v6 && (v8 != 258 || v5 == dwMilliseconds) )
        break;
      if ( hWnd && PeekMessageA(&Msg, hWnd, wMsgFilterMin, wMsgFilterMin, 1u) )
      {
        do
          DispatchMessageA(&Msg);
        while ( PeekMessageA(&Msg, hWnd, wMsgFilterMin, wMsgFilterMin, 1u) );
      }
      PeekMessageA(&Msg, 0, 0, 0, 0);
      if ( dwMilliseconds != -1 && dwMilliseconds )
      {
        v10 = (_BYTE *)GetTickCount();
        if ( v10 - (_BYTE *)TickCount <= dwMilliseconds )
          dwMilliseconds -= v10 - (_BYTE *)TickCount;
        else
          dwMilliseconds = 0;
        TickCount = v10;
      }
      if ( !v23 )
      {
        CurrentThread = GetCurrentThread();
        nPriority = GetThreadPriority(CurrentThread);
        if ( (unsigned int)nPriority < 2 )
        {
          v12 = GetCurrentThread();
          SetThreadPriority(v12, 2);
        }
        v23 = 1;
      }
      v6 = nCount;
      v9 = WaitForMultipleObjects(nCount, Handles, 0, 0);
      if ( v9 < v6 )
        break;
      v5 = dwMilliseconds;
    }
    if ( v23 )
    {
      v18 = nPriority;
      v13 = GetCurrentThread();
      SetThreadPriority(v13, v18);
      if ( (GetQueueStatus(8u) & 0x80000) != 0 )
      {
        v14 = wMsgFilterMax;
        if ( wMsgFilterMax || (v14 = RegisterWindowMessageA("AMUnblock"), (wMsgFilterMax = v14) != 0) )
        {
          do
          {
            v15 = !PeekMessageA(&Msg, HWND_MESSAGE|0x2, v14, v14, 1u);
            v14 = wMsgFilterMax;
          }
          while ( !v15 );
        }
        v17 = v14;
        CurrentThreadId = GetCurrentThreadId();
        PostThreadMessageA(CurrentThreadId, v17, 0, 0);
      }
    }
    return v9;
  }
  return result;
}

// ===== sub_4C39D0 @ 0x004C39D0..0x004C3A34 =====
bool sub_4C39D0()
{
  struct _OSVERSIONINFOA VersionInformation; // [esp+0h] [ebp-98h] BYREF

  VersionInformation.dwOSVersionInfoSize = 148;
  return GetVersionExA(&VersionInformation)
      && (VersionInformation.dwMajorVersion > 5
       || VersionInformation.dwMajorVersion == 5 && VersionInformation.dwMinorVersion);
}

// ===== sub_4C3A40 @ 0x004C3A40..0x004C3A89 =====
MMRESULT __cdecl sub_4C3A40(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD_PTR dwUser, UINT fuEvent)
{
  if ( !byte_50A835 )
  {
    byte_50A834 = sub_4C39D0();
    byte_50A835 = 1;
  }
  if ( byte_50A834 )
    fuEvent |= 0x100u;
  return timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
}

// ===== sub_4C3AC0 @ 0x004C3AC0..0x004C3ADC =====
_DWORD *__thiscall sub_4C3AC0(_DWORD *this, int a2)
{
  _DWORD *result; // eax

  result = this;
  *this = 0;
  this[1] = 0;
  this[2] = 0;
  this[3] = 10;
  this[4] = 0;
  this[5] = 0;
  return result;
}

// ===== sub_4C3AE0 @ 0x004C3AE0..0x004C3B17 =====
void __thiscall sub_4C3AE0(_DWORD *this)
{
  _DWORD *v2; // esi
  void *v3; // [esp-4h] [ebp-Ch]

  v2 = (_DWORD *)*this;
  if ( *this )
  {
    do
    {
      v3 = v2;
      v2 = (_DWORD *)v2[1];
      operator delete(v3);
    }
    while ( v2 );
  }
  this[2] = 0;
  this[1] = 0;
  *this = 0;
}

// ===== sub_4C3B20 @ 0x004C3B20..0x004C3B23 =====
int __thiscall sub_4C3B20(void *this)
{
  return *(_DWORD *)this;
}

// ===== sub_4C3B30 @ 0x004C3B30..0x004C3B4C =====
int __stdcall sub_4C3B30(int *a1)
{
  int result; // eax

  result = *a1;
  if ( *a1 )
  {
    *a1 = *(_DWORD *)(result + 4);
    return *(_DWORD *)(result + 8);
  }
  return result;
}

// ===== sub_4C3B50 @ 0x004C3B50..0x004C3B65 =====
int __stdcall sub_4C3B50(int a1)
{
  int result; // eax

  result = a1;
  if ( a1 )
    return *(_DWORD *)(a1 + 8);
  return result;
}

// ===== sub_4C3B70 @ 0x004C3B70..0x004C3BA9 =====
int __thiscall sub_4C3B70(void *this, int a2)
{
  int v2; // edx
  int v3; // edx
  int *v4; // ecx

  v2 = sub_4C3B20(this);
  if ( !v2 )
    return 0;
  while ( sub_4C3B50(v2) != a2 )
  {
    if ( v3 )
      v2 = *(_DWORD *)(v3 + 4);
    else
      v2 = *v4;
    if ( !v2 )
      return 0;
  }
  return v3;
}

// ===== sub_4C3BB0 @ 0x004C3BB0..0x004C3C1E =====
_DWORD *__thiscall sub_4C3BB0(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax
  _DWORD *v4; // ecx
  int v5; // edi

  result = a2;
  if ( a2 )
  {
    if ( *a2 )
      *(_DWORD *)(*a2 + 4) = a2[1];
    else
      *this = a2[1];
    v4 = (_DWORD *)a2[1];
    if ( v4 )
      *v4 = *a2;
    else
      this[1] = *a2;
    v5 = a2[2];
    if ( this[4] >= this[3] )
    {
      operator delete(a2);
    }
    else
    {
      a2[1] = this[5];
      ++this[4];
      this[5] = a2;
    }
    --this[2];
    return (_DWORD *)v5;
  }
  return result;
}

// ===== sub_4C3C20 @ 0x004C3C20..0x004C3C81 =====
_DWORD *__thiscall sub_4C3C20(_DWORD *this, int a2)
{
  _DWORD *result; // eax
  int v4; // ecx
  int v5; // ecx

  result = (_DWORD *)this[5];
  if ( result )
  {
    v4 = result[1];
    --this[4];
    this[5] = v4;
  }
  else
  {
    result = operator new(0xCu);
    if ( !result )
      return result;
  }
  result[2] = a2;
  result[1] = 0;
  *result = this[1];
  v5 = this[1];
  if ( v5 )
  {
    *(_DWORD *)(v5 + 4) = result;
    ++this[2];
  }
  else
  {
    ++this[2];
    *this = result;
  }
  this[1] = result;
  return result;
}

// ===== sub_4C3C90 @ 0x004C3C90..0x004C3CD8 =====
int __thiscall sub_4C3C90(_DWORD *this, void *a2)
{
  int v3; // eax

  a2 = (void *)sub_4C3B20(a2);
  if ( !a2 )
    return 1;
  while ( 1 )
  {
    v3 = sub_4C3B30((int *)&a2);
    if ( !sub_4C3C20(this, v3) )
      break;
    if ( !a2 )
      return 1;
  }
  return 0;
}

// ===== sub_4C3CE0 @ 0x004C3CE0..0x004C3D43 =====
void __thiscall sub_4C3CE0(_DWORD *this)
{
  _DWORD *v2; // esi
  void *v3; // [esp-4h] [ebp-1Ch]

  sub_4C3AE0(this);
  v2 = (_DWORD *)this[5];
  while ( v2 )
  {
    v3 = v2;
    v2 = (_DWORD *)v2[1];
    operator delete(v3);
  }
}

// ===== sub_4C3D50 @ 0x004C3D50..0x004C3D59 =====
_DWORD *__thiscall sub_4C3D50(_DWORD *this)
{
  return sub_4C3BB0(this, (_DWORD *)*this);
}

// ===== sub_4C3D60 @ 0x004C3D60..0x004C3D6F =====
int __thiscall sub_4C3D60(int *this)
{
  int result; // eax

  result = *this;
  if ( *this )
    return (*(int (__stdcall **)(int))(*(_DWORD *)result + 8))(result);
  return result;
}

// ===== sub_4C3D70 @ 0x004C3D70..0x004C3D8F =====
int __stdcall sub_4C3D70(int a1, _DWORD *a2)
{
  if ( !a2 )
    return -2147467261;
  *a2 = 1;
  return 0;
}

// ===== sub_4C3D90 @ 0x004C3D90..0x004C3E6C =====
int __stdcall sub_4C3D90(_DWORD *a1, int a2, int a3, LCID a4, _DWORD *a5)
{
  _DWORD *v5; // eax
  int result; // eax
  bool v7; // zf
  HMODULE v8; // eax
  HMODULE v9; // esi
  HRESULT (__stdcall *LoadRegTypeLib)(const GUID *const, WORD, WORD, LCID, ITypeLib **); // eax
  HRESULT (__stdcall *LoadTypeLib)(LPCOLESTR, ITypeLib **); // eax
  int v12; // esi

  v5 = a5;
  if ( !a5 )
    return -2147467261;
  v7 = a3 == 0;
  *a5 = 0;
  if ( !v7 )
    return -2147319765;
  if ( *a1 )
  {
LABEL_15:
    *v5 = *a1;
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)*a1 + 4))(*a1);
    return 0;
  }
  v8 = sub_4BCF10();
  v9 = v8;
  if ( !v8 )
    return GetLastError() | 0x80070000;
  LoadRegTypeLib = (HRESULT (__stdcall *)(const GUID *const, WORD, WORD, LCID, ITypeLib **))GetProcAddress(
                                                                                              v8,
                                                                                              "LoadRegTypeLib");
  if ( !LoadRegTypeLib )
    return GetLastError() | 0x80070000;
  if ( LoadRegTypeLib(&CLSID_LIBID_QuartzTypeLib, 1, 0, a4, (ITypeLib **)&a3) >= 0 )
    goto LABEL_12;
  LoadTypeLib = (HRESULT (__stdcall *)(LPCOLESTR, ITypeLib **))GetProcAddress(v9, "LoadTypeLib");
  if ( !LoadTypeLib )
    return GetLastError() | 0x80070000;
  result = LoadTypeLib(L"control.tlb", (ITypeLib **)&a3);
  if ( result >= 0 )
  {
LABEL_12:
    v12 = (*(int (__stdcall **)(int, int, _DWORD *))(*(_DWORD *)a3 + 24))(a3, a2, a1);
    (*(void (__stdcall **)(int))(*(_DWORD *)a3 + 8))(a3);
    if ( v12 < 0 )
      return v12;
    v5 = a5;
    goto LABEL_15;
  }
  return result;
}

// ===== sub_4C3E70 @ 0x004C3E70..0x004C3EB9 =====
int __stdcall sub_4C3E70(_DWORD *a1, int a2, int a3, int a4, LCID a5, int a6)
{
  int result; // eax
  LCID v7; // esi
  int v8; // edi

  result = sub_4C3D90(a1, a2, 0, a5, &a5);
  if ( result >= 0 )
  {
    v7 = a5;
    v8 = (*(int (__stdcall **)(LCID, int, int, int))(*(_DWORD *)a5 + 40))(a5, a3, a4, a6);
    (*(void (__stdcall **)(LCID))(*(_DWORD *)v7 + 8))(v7);
    return v8;
  }
  return result;
}

// ===== sub_4C3EC0 @ 0x004C3EC0..0x004C3F01 =====
int __stdcall sub_4C3EC0(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( sub_445510(a2, dword_4DB8E4) )
    return sub_4BCF30(a1 - 4, a3);
  else
    return sub_4BCF90(a1, a2, a3);
}

// ===== sub_4C3F10 @ 0x004C3F10..0x004C3F27 =====
int __stdcall sub_4C3F10(int a1, _DWORD *a2)
{
  return sub_4C3D70(a1 + 16, a2);
}

// ===== sub_4C3F30 @ 0x004C3F30..0x004C3F54 =====
int __stdcall sub_4C3F30(int a1, int a2, LCID a3, _DWORD *a4)
{
  return sub_4C3D90((_DWORD *)(a1 + 16), (int)dword_4DB8E4, a2, a3, a4);
}

// ===== sub_4C3F60 @ 0x004C3F60..0x004C3F88 =====
int __stdcall sub_4C3F60(int a1, int a2, int a3, int a4, LCID a5, int a6)
{
  return sub_4C3E70((_DWORD *)(a1 + 16), (int)dword_4DB8E4, a3, a4, a5, a6);
}

// ===== sub_4C3F90 @ 0x004C3F90..0x004C4003 =====
int __stdcall sub_4C3F90(int a1, int a2, _DWORD *a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  int result; // eax
  int v10; // esi

  if ( !sub_445510(&dword_4DC2F8, a3) )
    return -2147352575;
  result = (*(int (__stdcall **)(int, _DWORD, int, _DWORD **))(*(_DWORD *)a1 + 16))(a1, 0, a4, &a3);
  if ( result >= 0 )
  {
    v10 = (*(int (__stdcall **)(_DWORD *, int, int, int, int, int, int, int))(*a3 + 44))(a3, a1, a2, a5, a6, a7, a8, a9);
    (*(void (__stdcall **)(_DWORD *))(*a3 + 8))(a3);
    return v10;
  }
  return result;
}
