#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4A40D0 @ 0x004A40D0..0x004A4208 =====
int __cdecl sub_4A40D0(unsigned int a1, const CHAR *lpFileName, int a3, int a4, int a5, double a6, char a7)
{
  void (__thiscall ***v8)(_DWORD, int); // esi
  void *v9[4]; // [esp+10h] [ebp-30h] BYREF
  void *v10; // [esp+24h] [ebp-1Ch]
  _DWORD pExceptionObject[3]; // [esp+28h] [ebp-18h] BYREF
  int v12; // [esp+3Ch] [ebp-4h]

  pExceptionObject[2] = v9;
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v12 = 0;
  v10 = operator new(0x954u);
  LOBYTE(v12) = 1;
  if ( v10 )
    v8 = (void (__thiscall ***)(_DWORD, int))sub_4A4ED0(v9[0], v9[1]);
  else
    v8 = 0;
  LOBYTE(v12) = 0;
  if ( !sub_4A4DB0(lpFileName, a3) )
  {
    if ( v8 )
      (**v8)(v8, 1);
    pExceptionObject[0] = 12;
    _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  return sub_4A3570(a1, (int)v8, a4, a5, a6, a7);
}

// ===== sub_4A4210 @ 0x004A4210..0x004A4475 =====
int __cdecl sub_4A4210(unsigned int a1, LPCSTR lpFileName, LPCSTR a3, int a4, __int64 a5, double a6)
{
  void (__thiscall ***v7)(_DWORD, int); // esi
  char v8; // al
  void (__thiscall ***v9)(_DWORD, int); // esi
  void (__thiscall ***v10)(void *, int); // edi
  int v11; // [esp+14h] [ebp-40h] BYREF
  int v12; // [esp+18h] [ebp-3Ch]
  void (__thiscall ***v13)(_DWORD, int); // [esp+28h] [ebp-2Ch]
  void *v14; // [esp+2Ch] [ebp-28h]
  int v15; // [esp+30h] [ebp-24h] BYREF
  int v16; // [esp+34h] [ebp-20h] BYREF
  int v17; // [esp+38h] [ebp-1Ch]
  _DWORD pExceptionObject[3]; // [esp+3Ch] [ebp-18h] BYREF
  int v19; // [esp+50h] [ebp-4h]

  pExceptionObject[2] = &v11;
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v19 = 0;
  if ( !strcmp(lpFileName, a3) )
  {
    v14 = operator new(0x1Cu);
    LOBYTE(v19) = 1;
    if ( v14 )
      v7 = (void (__thiscall ***)(_DWORD, int))sub_4A4C60(v11, v12);
    else
      v7 = 0;
    LOBYTE(v19) = 0;
    v13 = v7;
    if ( !sub_4A4B00(lpFileName) )
    {
      if ( v7 )
        (**v7)(v7, 1);
      pExceptionObject[0] = 12;
      _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
    }
    v8 = 1;
    v17 = 1;
    if ( a4 )
    {
      v8 = 3;
      v17 = 3;
    }
    return sub_4A3570(a1, (int)v7, a5, SHIDWORD(a5), a6, v8);
  }
  else
  {
    v14 = operator new(0x1Cu);
    LOBYTE(v19) = 2;
    if ( v14 )
      v9 = (void (__thiscall ***)(_DWORD, int))sub_4A4C60(v11, v12);
    else
      v9 = 0;
    LOBYTE(v19) = 0;
    v13 = v9;
    if ( !sub_4A4B00(lpFileName) )
    {
      if ( v9 )
        (**v9)(v9, 1);
      v16 = 12;
      _CxxThrowException(&v16, (_ThrowInfo *)&_TI1K);
    }
    v14 = operator new(0x1Cu);
    LOBYTE(v19) = 3;
    if ( v14 )
      v10 = (void (__thiscall ***)(void *, int))sub_4A4C60(v11, v12);
    else
      v10 = 0;
    LOBYTE(v19) = 0;
    v14 = v10;
    if ( !sub_4A4B00(a3) )
    {
      if ( v9 )
        (**v9)(v9, 1);
      if ( v10 )
        (**v10)(v10, 1);
      v15 = 12;
      _CxxThrowException(&v15, (_ThrowInfo *)&_TI1K);
    }
    return sub_4A3810(a1, (int)v9, (int)v10, a4, a5, SHIDWORD(a5), a6);
  }
}

// ===== sub_4A4480 @ 0x004A4480..0x004A46ED =====
int __cdecl sub_4A4480(
        unsigned int a1,
        const CHAR *lpFileName,
        const char *a3,
        const char *a4,
        int a5,
        __int64 a6,
        double a7)
{
  void (__thiscall ***v8)(_DWORD, int); // esi
  char v9; // al
  void (__thiscall ***v10)(void *, int); // edi
  int v11; // [esp+14h] [ebp-40h] BYREF
  int v12; // [esp+18h] [ebp-3Ch]
  void (__thiscall ***v13)(_DWORD, int); // [esp+28h] [ebp-2Ch]
  void *v14; // [esp+2Ch] [ebp-28h]
  int v15; // [esp+30h] [ebp-24h] BYREF
  int v16; // [esp+34h] [ebp-20h] BYREF
  int v17; // [esp+38h] [ebp-1Ch]
  _DWORD pExceptionObject[3]; // [esp+3Ch] [ebp-18h] BYREF
  int v19; // [esp+50h] [ebp-4h]

  pExceptionObject[2] = &v11;
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v8 = 0;
  v19 = 0;
  if ( !strcmp(a3, a4) )
  {
    v14 = operator new(0x954u);
    LOBYTE(v19) = 1;
    if ( v14 )
      v8 = (void (__thiscall ***)(_DWORD, int))sub_4A4ED0(v11, v12);
    LOBYTE(v19) = 0;
    v13 = v8;
    if ( !sub_4A4DB0(lpFileName, (int)a3) )
    {
      if ( v8 )
        (**v8)(v8, 1);
      pExceptionObject[0] = 12;
      _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
    }
    v9 = 1;
    v17 = 1;
    if ( a5 )
    {
      v9 = 3;
      v17 = 3;
    }
    return sub_4A3570(a1, (int)v8, a6, SHIDWORD(a6), a7, v9);
  }
  else
  {
    v14 = operator new(0x954u);
    LOBYTE(v19) = 2;
    if ( v14 )
      v8 = (void (__thiscall ***)(_DWORD, int))sub_4A4ED0(v11, v12);
    LOBYTE(v19) = 0;
    v13 = v8;
    if ( !sub_4A4DB0(lpFileName, (int)a3) )
    {
      if ( v8 )
        (**v8)(v8, 1);
      v16 = 12;
      _CxxThrowException(&v16, (_ThrowInfo *)&_TI1K);
    }
    v14 = operator new(0x954u);
    LOBYTE(v19) = 3;
    if ( v14 )
      v10 = (void (__thiscall ***)(void *, int))sub_4A4ED0(v11, v12);
    else
      v10 = 0;
    LOBYTE(v19) = 0;
    v14 = v10;
    if ( !sub_4A4DB0(lpFileName, (int)a4) )
    {
      if ( v8 )
        (**v8)(v8, 1);
      if ( v10 )
        (**v10)(v10, 1);
      v15 = 12;
      _CxxThrowException(&v15, (_ThrowInfo *)&_TI1K);
    }
    return sub_4A3810(a1, (int)v8, (int)v10, a5, a6, SHIDWORD(a6), a7);
  }
}

// ===== sub_4A46F0 @ 0x004A46F0..0x004A478F =====
int __cdecl sub_4A46F0(unsigned int a1, const void *a2, int a3, double a4, double a5)
{
  int v5; // esi
  _DWORD v7[9]; // [esp+10h] [ebp-24h] BYREF

  v7[5] = v7;
  sub_4A4A80(&unk_5089E8);
  v7[8] = 0;
  v5 = sub_4A3E40(a1, a2, a3, a4, a5);
  v7[4] = v5;
  sub_4A4A90(&unk_5089E8);
  return v5;
}

// ===== sub_4A4790 @ 0x004A4790..0x004A482E =====
int __cdecl sub_4A4790(unsigned int a1, LPCSTR lpFileName, int a3, int a4, double a5)
{
  int v5; // esi
  _DWORD v7[9]; // [esp+Ch] [ebp-24h] BYREF

  v7[5] = v7;
  sub_4A4A80(&unk_5089E8);
  v7[8] = 0;
  v5 = sub_4A3F90(a1, lpFileName, a3, a4, a5, 0);
  v7[4] = v5;
  sub_4A4A90(&unk_5089E8);
  return v5;
}

// ===== sub_4A4830 @ 0x004A4830..0x004A48D2 =====
int __cdecl sub_4A4830(unsigned int a1, const CHAR *a2, int a3, int a4, int a5, double a6)
{
  int v6; // esi
  _DWORD v8[9]; // [esp+Ch] [ebp-24h] BYREF

  v8[5] = v8;
  sub_4A4A80(&unk_5089E8);
  v8[8] = 0;
  v6 = sub_4A40D0(a1, a2, a3, a4, a5, a6, 0);
  v8[4] = v6;
  sub_4A4A90(&unk_5089E8);
  return v6;
}

// ===== sub_4A48E0 @ 0x004A48E0..0x004A4984 =====
int __cdecl sub_4A48E0(unsigned int a1, LPCSTR lpFileName, LPCSTR a3, int a4, __int64 a5, double a6)
{
  int v6; // esi
  _DWORD v8[9]; // [esp+8h] [ebp-24h] BYREF

  v8[5] = v8;
  sub_4A4A80(&unk_5089E8);
  v8[8] = 0;
  v6 = sub_4A4210(a1, lpFileName, a3, a4, a5, a6);
  v8[4] = v6;
  sub_4A4A90(&unk_5089E8);
  return v6;
}

// ===== sub_4A4990 @ 0x004A4990..0x004A4A38 =====
int __cdecl sub_4A4990(unsigned int a1, const CHAR *a2, const char *a3, const char *a4, int a5, __int64 a6, double a7)
{
  int v7; // esi
  _DWORD v9[9]; // [esp+8h] [ebp-24h] BYREF

  v9[5] = v9;
  sub_4A4A80(&unk_5089E8);
  v9[8] = 0;
  v7 = sub_4A4480(a1, a2, a3, a4, a5, a6, a7);
  v9[4] = v7;
  sub_4A4A90(&unk_5089E8);
  return v7;
}

// ===== sub_4A4A40 @ 0x004A4A40..0x004A4A57 =====
char *__thiscall sub_4A4A40(char *this)
{
  *(_DWORD *)this = &CExclusionCtrlLight::`vftable';
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 4));
  return this;
}

// ===== sub_4A4A60 @ 0x004A4A60..0x004A4A71 =====
void __thiscall sub_4A4A60(char *this)
{
  *(_DWORD *)this = &CExclusionCtrlLight::`vftable';
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 4));
}

// ===== sub_4A4A80 @ 0x004A4A80..0x004A4A8D =====
int __thiscall sub_4A4A80(int this)
{
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 4));
  return 0;
}

// ===== sub_4A4A90 @ 0x004A4A90..0x004A4A9D =====
int __thiscall sub_4A4A90(int this)
{
  LeaveCriticalSection((LPCRITICAL_SECTION)(this + 4));
  return 0;
}

// ===== sub_4A4AA0 @ 0x004A4AA0..0x004A4AC1 =====
char *__thiscall sub_4A4AA0(char *this, char a2)
{
  sub_4A4A60(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A4AD0 @ 0x004A4AD0..0x004A4AF1 =====
void *__thiscall sub_4A4AD0(void *this, char a2)
{
  sub_4A5600();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A4B00 @ 0x004A4B00..0x004A4B50 =====
BOOL __thiscall sub_4A4B00(_DWORD *this, LPCSTR lpFileName)
{
  if ( this[6] != -1 )
    return 0;
  this[6] = CreateFileA(lpFileName, 0x80000000, 1u, 0, 3u, 0x8000020u, 0);
  this[5] = 0;
  unknown_libname_2(1);
  return this[6] != -1;
}

// ===== sub_4A4B50 @ 0x004A4B50..0x004A4B6D =====
int __thiscall sub_4A4B50(HANDLE *this)
{
  if ( this[6] != (HANDLE)-1 )
  {
    CloseHandle(this[6]);
    this[6] = (HANDLE)-1;
  }
  return 0;
}

// ===== sub_4A4B70 @ 0x004A4B70..0x004A4BA5 =====
int __thiscall sub_4A4B70(int this, LPVOID lpBuffer, int nNumberOfBytesToRead)
{
  int result; // eax

  if ( !ReadFile(*(HANDLE *)(this + 24), lpBuffer, nNumberOfBytesToRead, (LPDWORD)&nNumberOfBytesToRead, 0) )
    return -1;
  result = nNumberOfBytesToRead;
  *(_DWORD *)(this + 20) += nNumberOfBytesToRead;
  return result;
}

// ===== sub_4A4BB0 @ 0x004A4BB0..0x004A4BE5 =====
int __thiscall sub_4A4BB0(int this, LPCVOID lpBuffer, int nNumberOfBytesToWrite)
{
  int result; // eax

  if ( !WriteFile(*(HANDLE *)(this + 24), lpBuffer, nNumberOfBytesToWrite, (LPDWORD)&nNumberOfBytesToWrite, 0) )
    return -1;
  result = nNumberOfBytesToWrite;
  *(_DWORD *)(this + 20) += nNumberOfBytesToWrite;
  return result;
}

// ===== sub_4A4BF0 @ 0x004A4BF0..0x004A4C11 =====
DWORD __thiscall sub_4A4BF0(_DWORD *this)
{
  void *v1; // eax
  DWORD FileSizeHigh; // [esp+0h] [ebp-4h] BYREF

  FileSizeHigh = (DWORD)this;
  v1 = (void *)this[6];
  if ( v1 )
    return GetFileSize(v1, &FileSizeHigh);
  else
    return -1;
}

// ===== sub_4A4C20 @ 0x004A4C20..0x004A4C30 =====
DWORD __thiscall sub_4A4C20(_DWORD *this)
{
  return sub_4A4BF0(this - 2) - this[3];
}

// ===== sub_4A4C30 @ 0x004A4C30..0x004A4C3D =====
DWORD __thiscall sub_4A4C30(_DWORD *this)
{
  return sub_4A4BF0(this) - this[5];
}

// ===== sub_4A4C40 @ 0x004A4C40..0x004A4C45 =====
// attributes: thunk
DWORD __thiscall sub_4A4C40(_DWORD *this)
{
  return sub_4A4BF0(this);
}

// ===== sub_4A4C50 @ 0x004A4C50..0x004A4C58 =====
void *__thiscall sub_4A4C50(char *this, char a2)
{
  return sub_4A4AD0(this - 8, a2);
}

// ===== sub_4A4C60 @ 0x004A4C60..0x004A4CDF =====
_DWORD *__thiscall sub_4A4C60(_DWORD *this)
{
  sub_4A5450();
  sub_4A5340(this + 2);
  *this = &CStorageModel::`vftable';
  this[2] = &CStorageModel::`vftable';
  *this = &CFileStorage::`vftable';
  this[2] = &CFileStorage::`vftable';
  this[6] = -1;
  return this;
}

// ===== sub_4A4CE0 @ 0x004A4CE0..0x004A4CE8 =====
int __thiscall sub_4A4CE0(char *this, char a2)
{
  return sub_4A4D80(this - 8, a2);
}

// ===== sub_4A4CF0 @ 0x004A4CF0..0x004A4D50 =====
int __thiscall sub_4A4CF0(int this)
{
  *(_DWORD *)this = &CFileStorage::`vftable';
  *(_DWORD *)(this + 8) = &CFileStorage::`vftable';
  sub_4A4B50((HANDLE *)this);
  return sub_4A5600();
}

// ===== sub_4A4D50 @ 0x004A4D50..0x004A4D7B =====
DWORD __thiscall sub_4A4D50(int this, unsigned int lDistanceToMove)
{
  DWORD v3; // eax
  LONG v4; // ecx
  DWORD result; // eax

  v3 = sub_4A4BF0((_DWORD *)this);
  v4 = lDistanceToMove;
  if ( lDistanceToMove > v3 )
    v4 = v3;
  result = SetFilePointer(*(HANDLE *)(this + 24), v4, 0, 0);
  *(_DWORD *)(this + 20) = result;
  return result;
}

// ===== sub_4A4D80 @ 0x004A4D80..0x004A4DA1 =====
void *__thiscall sub_4A4D80(void *this, char a2)
{
  sub_4A4CF0((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A4DB0 @ 0x004A4DB0..0x004A4E37 =====
int __thiscall sub_4A4DB0(int this, LPCSTR lpFileName, const char *a3)
{
  int result; // eax

  if ( GetFileAttributesA(lpFileName) == -1 )
    return 0;
  result = sub_4A5E70(lpFileName, (int)a3);
  if ( result )
  {
    strcpy((char *)(this + 28), lpFileName);
    strcpy((char *)(this + 808), a3);
    *(_DWORD *)(this + 20) = 0;
    *(_DWORD *)(this + 24) = sub_4A5EE0((LPCSTR)(this + 28), this + 808);
    return 1;
  }
  return result;
}

// ===== sub_4A4E40 @ 0x004A4E40..0x004A4E8A =====
int __thiscall sub_4A4E40(int this, int a2, DWORD a3)
{
  int v4; // edx
  unsigned int v5; // ecx
  int result; // eax
  DWORD v7; // ecx

  v4 = *(_DWORD *)(this + 20);
  v5 = *(_DWORD *)(this + 24);
  result = 0;
  if ( v4 + a3 <= v5 )
    v7 = a3;
  else
    v7 = v5 - v4;
  if ( v7 )
  {
    result = sub_4A5EA0(a2, (LPCSTR)(this + 28), this + 808, v4, v7);
    *(_DWORD *)(this + 20) += result;
  }
  return result;
}

// ===== sub_4A4E90 @ 0x004A4E90..0x004A4E95 =====
int __stdcall sub_4A4E90(int a1, int a2)
{
  return 0;
}

// ===== sub_4A4EA0 @ 0x004A4EA0..0x004A4EA4 =====
int __thiscall sub_4A4EA0(_DWORD *this)
{
  return this[5];
}

// ===== sub_4A4EB0 @ 0x004A4EB0..0x004A4EB5 =====
// attributes: thunk
int sub_4A4EB0()
{
  return sub_4C6E40();
}

// ===== sub_4A4EC0 @ 0x004A4EC0..0x004A4EC9 =====
int sub_4A4EC0()
{
  int v0; // eax
  int v1; // ecx

  v0 = sub_4C6E40();
  return v0 - *(_DWORD *)(v1 + 20);
}

// ===== sub_4A4ED0 @ 0x004A4ED0..0x004A4F6C =====
_DWORD *__thiscall sub_4A4ED0(_DWORD *this)
{
  sub_4A5450();
  sub_4A5340(this + 2);
  *this = &CStorageModel::`vftable';
  this[2] = &CStorageModel::`vftable';
  *this = &CArchiveFileStorage::`vftable';
  this[2] = &CArchiveFileStorage::`vftable';
  sub_4A5680(this + 397);
  unknown_libname_2(1);
  this[6] = 0;
  return this;
}

// ===== sub_4A4F70 @ 0x004A4F70..0x004A4F76 =====
int __thiscall sub_4A4F70(_DWORD *this, int a2)
{
  return this[4];
}

// ===== sub_4A4F80 @ 0x004A4F80..0x004A4F88 =====
int __thiscall sub_4A4F80(char *this, char a2)
{
  return sub_4A5020(this - 8, a2);
}

// ===== sub_4A4F90 @ 0x004A4F90..0x004A4FF6 =====
int __thiscall sub_4A4F90(_DWORD *this)
{
  *this = &CArchiveFileStorage::`vftable';
  this[2] = &CArchiveFileStorage::`vftable';
  sub_4A5A50(this + 397);
  return sub_4A5600();
}

// ===== sub_4A5000 @ 0x004A5000..0x004A501F =====
unsigned int __stdcall sub_4A5000(unsigned int a1)
{
  unsigned int result; // eax
  int v2; // ecx

  result = sub_4C6E40();
  if ( a1 <= result )
  {
    *(_DWORD *)(v2 + 20) = a1;
    return a1;
  }
  else
  {
    *(_DWORD *)(v2 + 20) = result;
  }
  return result;
}

// ===== sub_4A5020 @ 0x004A5020..0x004A5041 =====
_DWORD *__thiscall sub_4A5020(_DWORD *this, char a2)
{
  sub_4A4F90(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A5050 @ 0x004A5050..0x004A5099 =====
int __thiscall sub_4A5050(void **this, size_t Size)
{
  void *v3; // eax
  void (__thiscall **v4)(void **, _DWORD); // edx

  operator delete(this[5]);
  v3 = operator new(Size);
  v4 = (void (__thiscall **)(void **, _DWORD))*this;
  this[5] = v3;
  this[6] = (void *)Size;
  if ( v3 )
  {
    v4[5](this, 0);
    return 1;
  }
  else
  {
    ((void (__thiscall *)(void **))v4[8])(this);
    return 0;
  }
}

// ===== sub_4A50A0 @ 0x004A50A0..0x004A50F6 =====
int __thiscall sub_4A50A0(int this, size_t Size)
{
  void *v4; // ebx
  size_t v5; // eax

  if ( !*(_DWORD *)(this + 20) )
    return 0;
  v4 = operator new(Size);
  v5 = Size;
  if ( Size >= *(_DWORD *)(this + 24) )
    v5 = Size;
  memcpy_0(v4, *(const void **)(this + 20), v5);
  operator delete(*(void **)(this + 20));
  *(_DWORD *)(this + 24) = Size;
  *(_DWORD *)(this + 20) = v4;
  return 1;
}

// ===== sub_4A5100 @ 0x004A5100..0x004A5146 =====
size_t __thiscall sub_4A5100(_DWORD *this, void *a2, size_t Size)
{
  int v4; // edx
  unsigned int v6; // eax
  unsigned int v7; // ecx
  size_t v8; // esi
  size_t v9; // eax

  v4 = this[5];
  if ( !v4 )
    return -1;
  v6 = this[6];
  v7 = this[7];
  if ( v6 <= v7 )
    return -1;
  v8 = Size;
  v9 = v6 - v7;
  if ( v9 <= Size )
    v8 = v9;
  memcpy_0(a2, (const void *)(v4 + v7), v8);
  this[7] += v8;
  return v8;
}

// ===== sub_4A5150 @ 0x004A5150..0x004A5196 =====
size_t __thiscall sub_4A5150(_DWORD *this, void *Src, size_t Size)
{
  int v4; // edx
  unsigned int v6; // eax
  unsigned int v7; // ecx
  size_t v8; // esi
  size_t v9; // eax

  v4 = this[5];
  if ( !v4 )
    return -1;
  v6 = this[6];
  v7 = this[7];
  if ( v6 <= v7 )
    return -1;
  v8 = Size;
  v9 = v6 - v7;
  if ( v9 <= Size )
    v8 = v9;
  memcpy_0((void *)(v4 + v7), Src, v8);
  this[7] += v8;
  return v8;
}

// ===== sub_4A51A0 @ 0x004A51A0..0x004A51A7 =====
int __thiscall sub_4A51A0(_DWORD *this)
{
  return this[6] - this[7];
}

// ===== sub_4A51B0 @ 0x004A51B0..0x004A51B7 =====
int __thiscall sub_4A51B0(_DWORD *this)
{
  return this[4] - this[5];
}

// ===== sub_4A51C0 @ 0x004A51C0..0x004A51D4 =====
int __thiscall sub_4A51C0(_DWORD *this, unsigned int a2)
{
  if ( a2 <= this[6] )
    this[7] = a2;
  return 0;
}

// ===== sub_4A51E0 @ 0x004A51E0..0x004A51F9 =====
int __thiscall sub_4A51E0(int this)
{
  int result; // eax

  operator delete(*(void **)(this + 20));
  result = 0;
  *(_DWORD *)(this + 20) = 0;
  *(_DWORD *)(this + 24) = 0;
  return result;
}

// ===== sub_4A5200 @ 0x004A5200..0x004A529A =====
_DWORD *__thiscall sub_4A5200(_DWORD *this, size_t Size)
{
  sub_4A5450();
  sub_4A5340(this + 2);
  *this = &CStorageModel::`vftable';
  this[2] = &CStorageModel::`vftable';
  *this = &CMemoryStorage::`vftable';
  this[2] = &CMemoryStorage::`vftable';
  this[5] = operator new(Size);
  this[6] = Size;
  sub_4A51C0(this, 0);
  return this;
}

// ===== sub_4A52A0 @ 0x004A52A0..0x004A52A8 =====
int __thiscall sub_4A52A0(char *this, char a2)
{
  return sub_4A5310(this - 8, a2);
}

// ===== sub_4A52B0 @ 0x004A52B0..0x004A5310 =====
int __thiscall sub_4A52B0(_DWORD *this)
{
  *this = &CMemoryStorage::`vftable';
  this[2] = &CMemoryStorage::`vftable';
  sub_4A51E0((int)this);
  return sub_4A5600();
}

// ===== sub_4A5310 @ 0x004A5310..0x004A5331 =====
_DWORD *__thiscall sub_4A5310(_DWORD *this, char a2)
{
  sub_4A52B0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A5340 @ 0x004A5340..0x004A535D =====
_DWORD *__thiscall sub_4A5340(_DWORD *this)
{
  *this = &COutputStreamModel::`vftable';
  this[1] = operator new(0x400u);
  return this;
}

// ===== sub_4A5360 @ 0x004A5360..0x004A537E =====
void __thiscall sub_4A5360(_DWORD *this)
{
  void *v2; // [esp-4h] [ebp-8h]

  v2 = (void *)this[1];
  *this = &COutputStreamModel::`vftable';
  operator delete(v2);
  this[1] = 0;
}

// ===== sub_4A5380 @ 0x004A5380..0x004A541F =====
unsigned int __thiscall sub_4A5380(_DWORD *this, _DWORD *a2, unsigned int a3)
{
  unsigned int v5; // esi
  int v6; // ecx
  unsigned int result; // eax
  int v8; // ecx
  unsigned int v9; // esi
  int v10; // eax
  _DWORD *v11; // ecx
  unsigned int v12; // eax
  unsigned int v13; // [esp+Ch] [ebp-8h]
  int v14; // [esp+10h] [ebp-4h]
  int v15; // [esp+1Ch] [ebp+8h]

  v5 = (*(int (__thiscall **)(_DWORD *))(*a2 + 12))(a2);
  v6 = -((*(int (__thiscall **)(_DWORD *))(*this + 12))(this) < v5);
  result = 0;
  v8 = -v6;
  v14 = v8;
  v15 = 0;
  if ( a3 )
  {
    while ( 1 )
    {
      v9 = a3 - result;
      if ( a3 - result > 0x400 )
        v9 = 1024;
      v13 = v9;
      if ( v8 )
      {
        v10 = *this;
        v11 = this;
      }
      else
      {
        v10 = *a2;
        v11 = a2;
      }
      v12 = (*(int (__thiscall **)(_DWORD *))(v10 + 12))(v11);
      if ( v12 < v9 )
        v9 = v12;
      (*(void (__thiscall **)(_DWORD *, _DWORD, unsigned int))(*a2 + 8))(a2, this[1], v9);
      (*(void (__thiscall **)(_DWORD *, _DWORD, unsigned int))(*this + 8))(this, this[1], v9);
      result = v9 + v15;
      v15 += v9;
      if ( v13 != v9 || result >= a3 )
        break;
      v8 = v14;
    }
  }
  return result;
}

// ===== sub_4A5420 @ 0x004A5420..0x004A5441 =====
_DWORD *__thiscall sub_4A5420(_DWORD *this, char a2)
{
  sub_4A5360(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A5450 @ 0x004A5450..0x004A546D =====
_DWORD *__thiscall sub_4A5450(_DWORD *this)
{
  *this = &CInputStreamModel::`vftable';
  this[1] = operator new(0x400u);
  return this;
}

// ===== sub_4A5470 @ 0x004A5470..0x004A548E =====
void __thiscall sub_4A5470(_DWORD *this)
{
  void *v2; // [esp-4h] [ebp-8h]

  v2 = (void *)this[1];
  *this = &CInputStreamModel::`vftable';
  operator delete(v2);
  this[1] = 0;
}

// ===== sub_4A5490 @ 0x004A5490..0x004A552F =====
unsigned int __thiscall sub_4A5490(_DWORD *this, _DWORD *a2, unsigned int a3)
{
  unsigned int v5; // esi
  int v6; // ecx
  unsigned int result; // eax
  int v8; // ecx
  unsigned int v9; // esi
  int v10; // eax
  _DWORD *v11; // ecx
  unsigned int v12; // eax
  unsigned int v13; // [esp+Ch] [ebp-8h]
  int v14; // [esp+10h] [ebp-4h]
  int v15; // [esp+1Ch] [ebp+8h]

  v5 = (*(int (__thiscall **)(_DWORD *))(*a2 + 12))(a2);
  v6 = -(v5 < (*(int (__thiscall **)(_DWORD *))(*this + 12))(this));
  result = 0;
  v8 = -v6;
  v14 = v8;
  v15 = 0;
  if ( a3 )
  {
    while ( 1 )
    {
      v9 = a3 - result;
      if ( a3 - result > 0x400 )
        v9 = 1024;
      v13 = v9;
      if ( v8 )
      {
        v10 = *this;
        v11 = this;
      }
      else
      {
        v10 = *a2;
        v11 = a2;
      }
      v12 = (*(int (__thiscall **)(_DWORD *))(v10 + 12))(v11);
      if ( v12 < v9 )
        v9 = v12;
      (*(void (__thiscall **)(_DWORD *, _DWORD, unsigned int))(*this + 8))(this, this[1], v9);
      (*(void (__thiscall **)(_DWORD *, _DWORD, unsigned int))(*a2 + 8))(a2, this[1], v9);
      result = v9 + v15;
      v15 += v9;
      if ( v13 != v9 || result >= a3 )
        break;
      v8 = v14;
    }
  }
  return result;
}

// ===== sub_4A5530 @ 0x004A5530..0x004A5551 =====
_DWORD *__thiscall sub_4A5530(_DWORD *this, char a2)
{
  sub_4A5470(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A5560 @ 0x004A5560..0x004A5578 =====
int __thiscall sub_4A5560(_BYTE *this, int a2, int a3)
{
  if ( (this[16] & 1) != 0 )
    return (*(int (__thiscall **)(_BYTE *, int, int))(*(_DWORD *)this + 36))(this, a2, a3);
  else
    return -1;
}

// ===== sub_4A5580 @ 0x004A5580..0x004A559C =====
int __thiscall sub_4A5580(_BYTE *this, int a2, int a3)
{
  if ( (this[8] & 2) != 0 )
    return (*(int (__thiscall **)(_BYTE *, int, int))(*((_DWORD *)this - 2) + 40))(this - 8, a2, a3);
  else
    return -1;
}

// ===== sub_4A55A0 @ 0x004A55A0..0x004A55B6 =====
unsigned int __thiscall sub_4A55A0(_DWORD *this, _DWORD *a2, unsigned int a3)
{
  if ( (this[4] & 1) != 0 )
    return sub_4A5490(this, a2, a3);
  else
    return -1;
}

// ===== sub_4A55C0 @ 0x004A55C0..0x004A55D6 =====
unsigned int __thiscall sub_4A55C0(_DWORD *this, _DWORD *a2, unsigned int a3)
{
  if ( (this[2] & 2) != 0 )
    return sub_4A5380(this, a2, a3);
  else
    return -1;
}

// ===== unknown_libname_2 @ 0x004A55E0..0x004A55ED =====
// Microsoft VisualC 2-14/net runtime
int __thiscall unknown_libname_2(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[4] = a2;
  return result;
}

// ===== sub_4A55F0 @ 0x004A55F0..0x004A55FC =====
int __thiscall sub_4A55F0(void *this)
{
  (*(void (__thiscall **)(void *, _DWORD))(*(_DWORD *)this + 16))(this, 0);
  return 0;
}

// ===== sub_4A5600 @ 0x004A5600..0x004A567D =====
void __thiscall sub_4A5600(_DWORD *this)
{
  *this = &CStorageModel::`vftable';
  this[2] = &CStorageModel::`vftable';
  sub_4A55F0(this);
  sub_4A5360(this + 2);
  sub_4A5470(this);
  sub_4A5360(this + 2);
  sub_4A5470(this);
}

// ===== sub_4A5680 @ 0x004A5680..0x004A5693 =====
_DWORD *__thiscall sub_4A5680(_DWORD *this)
{
  _DWORD *result; // eax

  result = this;
  *this = 0;
  this[198] = 0;
  this[199] = 0;
  return result;
}

// ===== sub_4A56A0 @ 0x004A56A0..0x004A5703 =====
int __thiscall sub_4A56A0(int *this)
{
  int v2; // ebx
  int v3; // ecx
  void *v4; // edi

  v2 = *this;
  if ( *this )
  {
    v3 = this[199];
    *this = 0;
    if ( v3 )
    {
      sub_4A56A0();
      v4 = (void *)this[199];
      if ( v4 )
      {
        sub_4A5A50(this[199]);
        operator delete(v4);
      }
      this[199] = 0;
    }
    operator delete((void *)this[198]);
    this[198] = 0;
  }
  return v2;
}

// ===== sub_4A5710 @ 0x004A5710..0x004A57B1 =====
BOOL __thiscall sub_4A5710(_DWORD *this, const char *a2)
{
  int v3; // ebx
  BOOL result; // eax
  int v5; // esi
  int v6; // edi
  int v7; // eax
  char String[96]; // [esp+10h] [ebp-64h] BYREF

  strcpy(String, a2);
  _strlwr(String);
  v3 = this[196];
  result = 0;
  v5 = 0;
  if ( v3 > 0 )
  {
    v6 = 0;
    do
    {
      if ( result )
        break;
      v7 = strcmp((const char *)(v6 + this[198]), String);
      ++v5;
      v6 += 128;
      result = v7 == 0;
    }
    while ( v5 < v3 );
  }
  return result;
}

// ===== sub_4A57C0 @ 0x004A57C0..0x004A594D =====
int __thiscall sub_4A57C0(int this, void *a2, const char *a3, int a4, DWORD nNumberOfBytesToRead)
{
  int v6; // esi
  int v7; // edi
  int result; // eax
  const char *i; // eax
  DWORD v10; // esi
  int v11; // edx
  int v12; // edi
  unsigned int v13; // ecx
  int v14; // esi
  _BYTE v16[4]; // [esp+18h] [ebp-74h] BYREF
  char String[96]; // [esp+1Ch] [ebp-70h] BYREF
  int v18; // [esp+88h] [ebp-4h]

  strcpy(String, a3);
  _strlwr(String);
  v6 = *(_DWORD *)(this + 784);
  v7 = 0;
  result = -2147483616;
  if ( v6 > 0 )
  {
    for ( i = *(const char **)(this + 792); strcmp(i, String); i += 128 )
    {
      if ( ++v7 >= v6 )
        return -2147483616;
    }
    sub_4A5F10(v16);
    v18 = 0;
    if ( sub_4A5F20((LPCSTR)(this + 4)) )
    {
      v10 = nNumberOfBytesToRead;
      if ( !nNumberOfBytesToRead )
        v10 = *(_DWORD *)((v7 << 7) + *(_DWORD *)(this + 792) + 100);
      v11 = *(_DWORD *)(this + 792);
      v12 = v7 << 7;
      v13 = *(_DWORD *)(v12 + v11 + 100);
      if ( v10 > v13 )
      {
        v14 = -2147483584;
        sub_4A5F60(v16);
      }
      else
      {
        if ( a4 + v10 > v13 )
        {
          v14 = -2147483600;
        }
        else
        {
          sub_4A5FB0(a4 + *(_DWORD *)(this + 788) + *(_DWORD *)(v12 + v11 + 96));
          v14 = sub_4A5F80(a2, v10);
        }
        sub_4A5F60(v16);
      }
    }
    else
    {
      v14 = -2147483632;
    }
    v18 = -1;
    sub_4A5FE0(v16);
    return v14;
  }
  return result;
}

// ===== sub_4A5950 @ 0x004A5950..0x004A5A20 =====
int __thiscall sub_4A5950(_DWORD *this, const char *a2)
{
  int v3; // eax
  int v4; // edi
  const char *v5; // esi
  const char *v7; // [esp+8h] [ebp-68h]
  char String[96]; // [esp+Ch] [ebp-64h] BYREF

  strcpy(String, a2);
  _strlwr(String);
  v3 = this[196];
  v4 = 0;
  if ( v3 <= 0 )
    return -2147483616;
  v5 = (const char *)this[198];
  v7 = v5;
  while ( strcmp(v5, String) )
  {
    ++v4;
    v5 += 128;
    if ( v4 >= v3 )
      return -2147483616;
  }
  return *(_DWORD *)&v7[128 * v4 + 100];
}

// ===== sub_4A5A20 @ 0x004A5A20..0x004A5A46 =====
BOOL __stdcall sub_4A5A20(unsigned __int8 a1)
{
  if ( a1 < 0x80u )
    return 0;
  if ( a1 >= 0xA0u )
    return a1 >= 0xE0u;
  return 1;
}

// ===== sub_4A5A50 @ 0x004A5A50..0x004A5A55 =====
// attributes: thunk
int __thiscall sub_4A5A50(int *this)
{
  return sub_4A56A0(this);
}

// ===== sub_4A5A60 @ 0x004A5A60..0x004A5A9C =====
void __stdcall sub_4A5A60(unsigned __int8 *a1)
{
  unsigned __int8 *v1; // esi
  char v2; // dl

  v1 = a1;
  while ( *v1 )
  {
    if ( sub_4A5A20(*v1) )
    {
      v1 += 2;
    }
    else
    {
      if ( v2 >= 65 && v2 <= 90 )
        *v1 = v2 + 32;
      ++v1;
    }
  }
}

// ===== sub_4A5AA0 @ 0x004A5AA0..0x004A5D66 =====
int __thiscall sub_4A5AA0(char *this, LPCSTR lpFileName)
{
  int result; // eax
  unsigned int v4; // eax
  int v5; // ecx
  int v6; // eax
  DWORD v7; // edi
  _DWORD *v8; // ebx
  int v9; // edi
  _DWORD *v10; // ebx
  char *v11; // edx
  _BYTE *v12; // eax
  char v13; // cl
  unsigned int v14; // eax
  int v15; // ecx
  int v16; // eax
  DWORD v17; // edi
  void *v18; // eax
  int v19; // edi
  int v20; // ebx
  _DWORD *v21; // [esp+10h] [ebp-30h]
  int v22; // [esp+14h] [ebp-2Ch]
  int v23; // [esp+18h] [ebp-28h]
  char v24[4]; // [esp+1Ch] [ebp-24h] BYREF
  _DWORD Buffer[3]; // [esp+20h] [ebp-20h] BYREF
  int v26; // [esp+2Ch] [ebp-14h]
  int v27; // [esp+3Ch] [ebp-4h]

  result = 0;
  v23 = 0;
  if ( !*(_DWORD *)this )
  {
    sub_4A5F10(v24);
    v27 = 0;
    if ( sub_4A5F20(lpFileName) )
    {
      if ( sub_4A5F80(Buffer, 0x10u) == 16 )
      {
        v4 = 12;
        v5 = 0;
        while ( Buffer[v5] == dword_4E4168[v5] )
        {
          v4 -= 4;
          ++v5;
          if ( v4 < 4 )
          {
            strcpy(this + 4, lpFileName);
            sub_4A5A60((unsigned __int8 *)this + 4);
            v6 = v26;
            v7 = 32 * v26;
            *((_DWORD *)this + 197) = 32 * v26 + 16;
            *((_DWORD *)this + 196) = v6;
            *((_DWORD *)this + 198) = operator new(v6 << 7);
            v8 = operator new(32 * *((_DWORD *)this + 196));
            v21 = v8;
            sub_4A5F80(v8, v7);
            v9 = 0;
            v22 = 0;
            if ( *((int *)this + 196) > 0 )
            {
              v10 = v8 + 5;
              do
              {
                memset((void *)(v9 + *((_DWORD *)this + 198)), 0, 0x80u);
                v11 = (char *)(v10 - 5);
                v12 = (_BYTE *)(v9 + *((_DWORD *)this + 198));
                do
                {
                  v13 = *v11;
                  *v12++ = *v11++;
                }
                while ( v13 );
                sub_4A5A60((unsigned __int8 *)(v9 + *((_DWORD *)this + 198)));
                *(_DWORD *)(*((_DWORD *)this + 198) + v9 + 96) = *(v10 - 1);
                *(_DWORD *)(*((_DWORD *)this + 198) + v9 + 100) = *v10;
                v10 += 8;
                v9 += 128;
                ++v22;
              }
              while ( v22 < *((_DWORD *)this + 196) );
              v8 = v21;
            }
            operator delete(v8);
            *(_DWORD *)this = 1;
            sub_4A5F60(v24);
            goto LABEL_23;
          }
        }
        v14 = 12;
        v15 = 0;
        while ( Buffer[v15] == dword_4E4178[v15] )
        {
          v14 -= 4;
          ++v15;
          if ( v14 < 4 )
          {
            strcpy(this + 4, lpFileName);
            sub_4A5A60((unsigned __int8 *)this + 4);
            v16 = v26;
            v17 = v26 << 7;
            *((_DWORD *)this + 197) = (v26 << 7) + 16;
            *((_DWORD *)this + 196) = v16;
            v18 = operator new(v16 << 7);
            *((_DWORD *)this + 198) = v18;
            sub_4A5F80(v18, v17);
            v19 = 0;
            if ( *((int *)this + 196) > 0 )
            {
              v20 = 0;
              do
              {
                sub_4A5A60((unsigned __int8 *)(v20 + *((_DWORD *)this + 198)));
                ++v19;
                v20 += 128;
              }
              while ( v19 < *((_DWORD *)this + 196) );
            }
            *(_DWORD *)this = 1;
            sub_4A5F60(v24);
            goto LABEL_23;
          }
        }
      }
      v23 = -2147483646;
      sub_4A5F60(v24);
    }
    else
    {
      v23 = -2147483647;
    }
LABEL_23:
    v27 = -1;
    sub_4A5FE0(v24);
    return v23;
  }
  return result;
}

// ===== sub_4A5D70 @ 0x004A5D70..0x004A5E62 =====
int __thiscall sub_4A5D70(int this, LPCSTR lpFileName)
{
  _DWORD *v3; // eax
  _DWORD *v4; // eax

  if ( !*(_DWORD *)this )
  {
    if ( sub_4A5AA0((char *)this, lpFileName) )
      return 0;
    return this;
  }
  if ( !strcmp(lpFileName, (const char *)(this + 4)) )
    return this;
  if ( !*(_DWORD *)(this + 796) )
  {
    v3 = operator new(0x320u);
    if ( v3 )
      v4 = sub_4A5680(v3);
    else
      v4 = 0;
    *(_DWORD *)(this + 796) = v4;
  }
  return sub_4A5D70(lpFileName);
}

// ===== sub_4A5E70 @ 0x004A5E70..0x004A5E95 =====
BOOL __thiscall sub_4A5E70(void *this, LPCSTR lpFileName, const char *a3)
{
  _DWORD *v3; // eax

  v3 = (_DWORD *)sub_4A5D70((int)this, lpFileName);
  return v3 && sub_4A5710(v3, a3);
}

// ===== sub_4A5EA0 @ 0x004A5EA0..0x004A5ED9 =====
int __thiscall sub_4A5EA0(void *this, void *a2, LPCSTR lpFileName, const char *a4, int a5, DWORD nNumberOfBytesToRead)
{
  int v6; // eax

  v6 = sub_4A5D70((int)this, lpFileName);
  if ( v6 )
    return sub_4A57C0(v6, a2, a4, a5, nNumberOfBytesToRead);
  else
    return -2147483632;
}

// ===== sub_4A5EE0 @ 0x004A5EE0..0x004A5F0D =====
int __thiscall sub_4A5EE0(void *this, LPCSTR lpFileName, const char *a3)
{
  _DWORD *v3; // eax

  v3 = (_DWORD *)sub_4A5D70((int)this, lpFileName);
  if ( v3 )
    return sub_4A5950(v3, a3);
  else
    return -2147483632;
}

// ===== sub_4A5F10 @ 0x004A5F10..0x004A5F19 =====
_DWORD *__thiscall sub_4A5F10(_DWORD *this)
{
  _DWORD *result; // eax

  result = this;
  *this = -1;
  return result;
}

// ===== sub_4A5F20 @ 0x004A5F20..0x004A5F58 =====
BOOL __thiscall sub_4A5F20(_DWORD *this, LPCSTR lpFileName)
{
  BOOL result; // eax
  char *FileA; // eax

  result = 0;
  if ( *this == -1 )
  {
    FileA = (char *)CreateFileA(lpFileName, 0x80000000, 1u, 0, 3u, 0x8000020u, 0);
    *this = FileA;
    return FileA + 1 != 0;
  }
  return result;
}

// ===== sub_4A5F60 @ 0x004A5F60..0x004A5F79 =====
HANDLE __thiscall sub_4A5F60(HANDLE *this)
{
  HANDLE result; // eax

  result = *this;
  if ( *this != (HANDLE)-1 )
  {
    result = (HANDLE)CloseHandle(*this);
    *this = (HANDLE)-1;
  }
  return result;
}

// ===== sub_4A5F80 @ 0x004A5F80..0x004A5FA9 =====
int __thiscall sub_4A5F80(HANDLE *this, LPVOID lpBuffer, DWORD nNumberOfBytesToRead)
{
  int result; // eax

  result = ReadFile(*this, lpBuffer, nNumberOfBytesToRead, &nNumberOfBytesToRead, 0);
  if ( result )
    return nNumberOfBytesToRead;
  return result;
}

// ===== sub_4A5FB0 @ 0x004A5FB0..0x004A5FD2 =====
BOOL __thiscall sub_4A5FB0(HANDLE *this, LONG lDistanceToMove)
{
  return SetFilePointer(*this, lDistanceToMove, 0, 0) != -1;
}

// ===== sub_4A5FE0 @ 0x004A5FE0..0x004A5FE5 =====
// attributes: thunk
HANDLE __thiscall sub_4A5FE0(HANDLE *this)
{
  return sub_4A5F60(this);
}

// ===== sub_4A5FF0 @ 0x004A5FF0..0x004A5FF6 =====
int sub_4A5FF0()
{
  return dword_509B40;
}

// ===== sub_4A6000 @ 0x004A6000..0x004A6013 =====
int __cdecl sub_4A6000(int a1)
{
  int result; // eax

  result = dword_509B40;
  dword_509B40 = a1;
  return result;
}

// ===== sub_4A6020 @ 0x004A6020..0x004A612E =====
int sub_4A6020()
{
  HRESULT v0; // eax
  _DWORD v2[4]; // [esp+0h] [ebp-14h] BYREF

  dword_509B14 = 0;
  dword_509B1C = 0;
  dword_509B20 = 0;
  dword_509B24 = 0;
  dword_509B28 = 0;
  dword_509B10 = 0;
  dword_509B18 = 0;
  dword_509B08 = 36;
  dword_509B0C = 1;
  v0 = ppDS8->lpVtbl->CreateSoundBuffer(ppDS8, (LPCDSBUFFERDESC)&dword_509B08, (LPDIRECTSOUNDBUFFER *)&dword_509B38, 0);
  if ( v0 )
  {
    (*(void (__cdecl **)(int, _DWORD, const char *, int, void *, HRESULT))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      0,
      "src\\DSSpeakerModel.cpp",
      168,
      &unk_4DBCF4,
      v0);
    return 0;
  }
  else
  {
    v2[0] = 131073;
    v2[1] = 44100;
    v2[2] = 176400;
    v2[3] = 1048580;
    (*(void (__stdcall **)(int, _DWORD *))(*(_DWORD *)dword_509B38 + 56))(dword_509B38, v2);
    (*(void (__stdcall **)(int, _DWORD, _DWORD, int))(*(_DWORD *)dword_509B38 + 48))(dword_509B38, 0, 0, 1);
    return 1;
  }
}

// ===== sub_4A6130 @ 0x004A6130..0x004A6299 =====
int sub_4A6130()
{
  HRESULT v0; // eax
  void *v2; // [esp+0h] [ebp-4Ch] BYREF
  int v3; // [esp+4h] [ebp-48h] BYREF
  size_t v4; // [esp+8h] [ebp-44h] BYREF
  void *v5; // [esp+Ch] [ebp-40h] BYREF
  size_t Size; // [esp+10h] [ebp-3Ch] BYREF
  _DWORD v7[2]; // [esp+14h] [ebp-38h] BYREF
  int v8; // [esp+1Ch] [ebp-30h]
  int v9; // [esp+20h] [ebp-2Ch]
  _DWORD *v10; // [esp+24h] [ebp-28h]
  int v11; // [esp+28h] [ebp-24h]
  int v12; // [esp+2Ch] [ebp-20h]
  int v13; // [esp+30h] [ebp-1Ch]
  int v14; // [esp+34h] [ebp-18h]
  _DWORD v15[4]; // [esp+38h] [ebp-14h] BYREF

  v10 = v15;
  v9 = 0;
  v11 = 0;
  v12 = 0;
  v13 = 0;
  v14 = 0;
  v15[0] = 131073;
  v15[1] = 44100;
  v15[2] = 176400;
  v15[3] = 1048580;
  v7[0] = 36;
  v7[1] = 32936;
  v8 = 88200;
  v0 = ppDS8->lpVtbl->CreateSoundBuffer(ppDS8, (LPCDSBUFFERDESC)v7, (LPDIRECTSOUNDBUFFER *)&v3, 0);
  if ( v0 )
  {
    (*(void (__cdecl **)(int, _DWORD, const char *, int, void *, HRESULT))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      0,
      "src\\DSSpeakerModel.cpp",
      214,
      &unk_4DBCF4,
      v0);
    return 0;
  }
  (**(void (__stdcall ***)(int, void *, int *))v3)(v3, &unk_4DC238, &dword_509B34);
  (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( (*(int (__stdcall **)(int, _DWORD, int, void **, size_t *, void **, size_t *, _DWORD))(*(_DWORD *)dword_509B34
                                                                                            + 44))(
         dword_509B34,
         0,
         v8,
         &v5,
         &Size,
         &v2,
         &v4,
         0) )
  {
    return 0;
  }
  memset(v5, 0, Size);
  memset(v2, 0, v4);
  (*(void (__stdcall **)(int, void *, size_t, void *, size_t))(*(_DWORD *)dword_509B34 + 76))(
    dword_509B34,
    v5,
    Size,
    v2,
    v4);
  (*(void (__stdcall **)(int, _DWORD, _DWORD, int))(*(_DWORD *)dword_509B34 + 48))(dword_509B34, 0, 0, 1);
  return 1;
}

// ===== sub_4A62A0 @ 0x004A62A0..0x004A62B0 =====
double __thiscall sub_4A62A0(double *this, double a2)
{
  double result; // st7

  result = this[15];
  this[15] = a2;
  return result;
}

// ===== sub_4A62B0 @ 0x004A62B0..0x004A6347 =====
int __thiscall sub_4A62B0(int this)
{
  unsigned int v2; // edi
  int v4; // [esp+8h] [ebp-10h] BYREF
  unsigned int v5; // [esp+Ch] [ebp-Ch] BYREF
  int v6; // [esp+10h] [ebp-8h] BYREF
  unsigned int v7; // [esp+14h] [ebp-4h] BYREF

  v2 = *(_DWORD *)(this + 24) * *(_DWORD *)(this + 36);
  if ( (*(int (__stdcall **)(_DWORD, _DWORD, unsigned int, int *, unsigned int *, int *, unsigned int *, _DWORD))(**(_DWORD **)(this + 92) + 44))(
         *(_DWORD *)(this + 92),
         0,
         v2 * *(_DWORD *)(this + 96),
         &v4,
         &v5,
         &v6,
         &v7,
         0) )
  {
    return 17;
  }
  (*(void (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(this + 108) + 8))(
    *(_DWORD *)(this + 108),
    v4,
    v5 / v2);
  (*(void (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(this + 108) + 8))(
    *(_DWORD *)(this + 108),
    v6,
    v7 / v2);
  (*(void (__stdcall **)(_DWORD, int, unsigned int, int, unsigned int))(**(_DWORD **)(this + 92) + 76))(
    *(_DWORD *)(this + 92),
    v4,
    v5,
    v6,
    v7);
  return 0;
}

// ===== sub_4A6350 @ 0x004A6350..0x004A6365 =====
int __stdcall sub_4A6350(int a1, int a2)
{
  int result; // eax

  result = dword_509B3C;
  dword_509B3C = a1;
  return result;
}

// ===== sub_4A6370 @ 0x004A6370..0x004A637A =====
int __thiscall sub_4A6370(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[27] + 12))(this[27]);
}

// ===== sub_4A6380 @ 0x004A6380..0x004A638D =====
int __thiscall sub_4A6380(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[26] = a2;
  return result;
}

// ===== sub_4A6390 @ 0x004A6390..0x004A63AD =====
int __fastcall sub_4A6390(int a1)
{
  int v2; // [esp+0h] [ebp-4h] BYREF

  v2 = a1;
  (*(void (__stdcall **)(_DWORD, int *))(**(_DWORD **)(a1 + 92) + 36))(*(_DWORD *)(a1 + 92), &v2);
  return v2 & 1;
}

// ===== sub_4A63B0 @ 0x004A63B0..0x004A655B =====
int __thiscall sub_4A63B0(int this, HWND a2)
{
  int v4; // eax
  int v6; // [esp+0h] [ebp-38h] BYREF
  int v7; // [esp+14h] [ebp-24h] BYREF
  int v8; // [esp+18h] [ebp-20h] BYREF
  int v9; // [esp+1Ch] [ebp-1Ch] BYREF
  _DWORD pExceptionObject[3]; // [esp+20h] [ebp-18h] BYREF
  int v11; // [esp+34h] [ebp-4h]
  void *v12; // [esp+40h] [ebp+8h]

  pExceptionObject[2] = &v6;
  pExceptionObject[1] = this;
  *(_DWORD *)this = &CDSSpeakerModel::`vftable';
  *(_DWORD *)(this + 92) = 0;
  if ( ++dword_509B2C == 1 )
  {
    v11 = 0;
    dword_509B44 = (int)a2;
    v12 = operator new(0x14u);
    LOBYTE(v11) = 1;
    if ( v12 )
      v4 = sub_4C6740(0);
    else
      v4 = 0;
    LOBYTE(v11) = 0;
    dword_509B48 = v4;
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v4 + 8))(v4, 1);
    if ( DirectSoundCreate8(0, &ppDS8, 0) )
    {
      (*(void (__cdecl **)(int, _DWORD, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
        dword_509B48,
        0,
        "src\\DSSpeakerModel.cpp",
        42,
        &unk_4DBD5C);
      pExceptionObject[0] = 0;
      _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1H);
    }
    if ( ppDS8->lpVtbl->SetCooperativeLevel(ppDS8, a2, 2) )
    {
      (*(void (__cdecl **)(int, _DWORD, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
        dword_509B48,
        0,
        "src\\DSSpeakerModel.cpp",
        49,
        &unk_4DBD2C);
      v9 = 1;
      _CxxThrowException(&v9, (_ThrowInfo *)&_TI1H);
    }
    if ( !sub_4A6020() )
    {
      v8 = 2;
      _CxxThrowException(&v8, (_ThrowInfo *)&_TI1H);
    }
    if ( !sub_4A6130() )
    {
      v7 = 3;
      _CxxThrowException(&v7, (_ThrowInfo *)&_TI1H);
    }
    sub_4A6000(1);
    v11 = -1;
  }
  *(_DWORD *)(this + 92) = 0;
  *(_DWORD *)(this + 108) = 0;
  *(double *)(this + 120) = 1.0;
  sub_4A6380((_DWORD *)this, 0);
  return this;
}

// ===== sub_4A6560 @ 0x004A6560..0x004A659B =====
int __thiscall sub_4A6560(_DWORD *this)
{
  int v2; // eax
  int v3; // edx

  v2 = this[23];
  if ( v2 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v2 + 72))(v2);
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)this[23] + 8))(this[23]);
    this[23] = 0;
  }
  this[27] = 0;
  sub_4A6380(this, 0);
  return v3;
}

// ===== sub_4A65A0 @ 0x004A65A0..0x004A6738 =====
int __userpurge sub_4A65A0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, _DWORD **a3)
{
  int result; // eax
  _DWORD **v5; // edi
  void (__thiscall ***v6)(_DWORD, int); // eax
  _DWORD *v7; // ecx
  _DWORD *v8; // eax
  void (__thiscall *v9)(_DWORD *, _DWORD *, _DWORD *); // edx
  int (__thiscall *v10)(_DWORD *); // edx

  if ( (sub_4A5FF0() & 1) != 0 )
  {
    v5 = a3;
    if ( (*(int (__thiscall **)(_DWORD *))(*a3[1] + 24))(a3[1]) )
    {
      v6 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(_DWORD *, int))(*a1 + 28))(a1, a2);
      if ( v6 )
        (**v6)(v6, 1);
      v7 = v5[1];
      a1[27] = v7;
      v8 = (_DWORD *)(*(int (__thiscall **)(_DWORD *))(*v7 + 16))(v7);
      a1[4] = *v8;
      a1[5] = v8[1];
      a1[6] = v8[2];
      a1[7] = v8[3];
      v9 = *(void (__thiscall **)(_DWORD *, _DWORD *, _DWORD *))(*a1 + 40);
      a1[8] = v8[4];
      a1[9] = a1[5] >> 3;
      v9(a1, a1 + 10, a1 + 19);
      if ( ppDS8->lpVtbl->CreateSoundBuffer(ppDS8, (LPCDSBUFFERDESC)(a1 + 10), (LPDIRECTSOUNDBUFFER *)&a3, 0) )
      {
        a1[23] = 0;
        (*(void (__cdecl **)(int, _DWORD, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
          dword_509B48,
          0,
          "src\\DSSpeakerModel.cpp",
          327,
          &unk_4DBDC0);
        sub_4A6350(2, 328);
        return 16;
      }
      else
      {
        ((void (__stdcall *)(_DWORD **, void *, _DWORD *))**a3)(a3, &unk_4DC238, a1 + 23);
        ((void (__stdcall *)(_DWORD **))(*a3)[2])(a3);
        result = (*(int (__thiscall **)(_DWORD *))(*a1 + 48))(a1);
        if ( !result )
        {
          v10 = *(int (__thiscall **)(_DWORD *))(*a1 + 52);
          a1[28] = 128;
          result = v10(a1);
          if ( !result )
          {
            sub_4A6380(a1, 1);
            return 0;
          }
        }
      }
    }
    else
    {
      (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
        dword_509B48,
        1,
        "src\\DSSpeakerModel.cpp",
        305,
        &unk_4DBDE8);
      sub_4A6350(2, 306);
      return 3;
    }
  }
  else
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      298,
      &unk_4DBE0C);
    sub_4A6350(2, 299);
    return 2;
  }
  return result;
}

// ===== sub_4A6740 @ 0x004A6740..0x004A67BA =====
int __thiscall sub_4A6740(_DWORD **this, int a2)
{
  int v4; // edi
  int v5; // eax

  if ( (sub_4A84C0() & 1) != 0 )
  {
    ((void (__thiscall *)(_DWORD **, int))(*this)[6])(this, a2);
    v4 = *this[23];
    v5 = ((int (__thiscall *)(_DWORD **))(*this)[14])(this);
    (*(void (__stdcall **)(_DWORD *, _DWORD, _DWORD, int))(v4 + 48))(this[23], 0, 0, v5);
    return 0;
  }
  else
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      433,
      &unk_4DBE0C);
    sub_4A6350(2, 434);
    return 2;
  }
}

// ===== sub_4A67C0 @ 0x004A67C0..0x004A682C =====
int __thiscall sub_4A67C0(_DWORD **this)
{
  if ( (sub_4A84C0() & 1) != 0 )
  {
    (*(void (__stdcall **)(_DWORD *))(*this[23] + 72))(this[23]);
    (*(void (__stdcall **)(_DWORD *, _DWORD))(*this[23] + 52))(this[23], 0);
    (*(void (__thiscall **)(_DWORD *, int, _DWORD))(*this[27] + 20))(this[27], 1, 0);
    return 0;
  }
  else
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      458,
      &unk_4DBE0C);
    sub_4A6350(2, 459);
    return 2;
  }
}

// ===== sub_4A6830 @ 0x004A6830..0x004A68D7 =====
int __thiscall sub_4A6830(_DWORD **this, int a2)
{
  int v4; // edi
  int v5; // eax
  _BYTE v6[4]; // [esp+4h] [ebp-4h] BYREF

  if ( (sub_4A84C0() & 1) == 0 )
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      486,
      &unk_4DBE0C);
    sub_4A6350(2, 487);
    return 2;
  }
  (*(void (__stdcall **)(_DWORD *, _BYTE *))(*this[23] + 36))(this[23], v6);
  if ( (v6[0] & 1) != 0 )
  {
    if ( a2 )
    {
      (*(void (__stdcall **)(_DWORD *))(*this[23] + 72))(this[23]);
      return 0;
    }
  }
  else if ( !a2 )
  {
    v4 = *this[23];
    v5 = ((int (__thiscall *)(_DWORD **))(*this)[14])(this);
    (*(void (__stdcall **)(_DWORD *, _DWORD, _DWORD, int))(v4 + 48))(this[23], 0, 0, v5);
  }
  return 0;
}

// ===== sub_4A68E0 @ 0x004A68E0..0x004A6955 =====
int __thiscall sub_4A68E0(int this, int a2)
{
  if ( (sub_4A84C0() & 1) != 0 )
  {
    *(_DWORD *)(this + 112) = -100 * a2;
    if ( -100 * a2 < -10000 )
      *(_DWORD *)(this + 112) = -10000;
    (*(void (__stdcall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 92) + 60))(
      *(_DWORD *)(this + 92),
      *(_DWORD *)(this + 112));
    return 0;
  }
  else
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      522,
      &unk_4DBE0C);
    sub_4A6350(2, 523);
    return 2;
  }
}

// ===== sub_4A6960 @ 0x004A6960..0x004A6A17 =====
int __thiscall sub_4A6960(int this, int a2)
{
  int v4; // eax

  if ( (sub_4A84C0() & 1) == 0 )
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSSpeakerModel.cpp",
      551,
      &unk_4DBE0C);
    sub_4A6350(2, 552);
    return 2;
  }
  v4 = a2;
  if ( a2 < -128 )
  {
    v4 = -128;
LABEL_7:
    a2 = v4;
    goto LABEL_8;
  }
  if ( a2 > 128 )
  {
    v4 = 128;
    goto LABEL_7;
  }
LABEL_8:
  *(_DWORD *)(this + 116) = v4;
  (*(void (__stdcall **)(_DWORD, int))(**(_DWORD **)(this + 92) + 64))(
    *(_DWORD *)(this + 92),
    (int)-(pow((double)a2, 3.0) / pow(128.0, 3.0) * -10000.0));
  return 0;
}

// ===== sub_4A6A20 @ 0x004A6A20..0x004A6AC8 =====
int (__thiscall ***__thiscall sub_4A6A20(_DWORD *this))(_DWORD, int)
{
  int (__thiscall ***result)(_DWORD, int); // eax

  *this = &CDSSpeakerModel::`vftable';
  result = (int (__thiscall ***)(_DWORD, int))sub_4A6560(this);
  if ( result )
    result = (int (__thiscall ***)(_DWORD, int))(**result)(result, 1);
  if ( dword_509B2C == 1 )
  {
    if ( dword_509B38 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_509B38 + 8))(dword_509B38);
      dword_509B38 = 0;
    }
    if ( dword_509B34 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_509B34 + 8))(dword_509B34);
      dword_509B34 = 0;
    }
    if ( ppDS8 )
    {
      ppDS8->lpVtbl->Release(ppDS8);
      ppDS8 = 0;
    }
    result = (int (__thiscall ***)(_DWORD, int))sub_4C6930(dword_509B48);
    if ( result )
      result = (int (__thiscall ***)(_DWORD, int))(**result)(result, 1);
    if ( dword_509B48 )
      result = (int (__thiscall ***)(_DWORD, int))(**(int (__thiscall ***)(int, int))dword_509B48)(dword_509B48, 1);
  }
  --dword_509B2C;
  return result;
}

// ===== sub_4A6AD0 @ 0x004A6AD0..0x004A6AF1 =====
_DWORD *__thiscall sub_4A6AD0(_DWORD *this, char a2)
{
  sub_4A6A20(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A6B00 @ 0x004A6B00..0x004A6B5A =====
_DWORD *__thiscall sub_4A6B00(_DWORD *this, HWND a2)
{
  sub_4A63B0((int)this, a2);
  *this = &CDSStaticSpeaker::`vftable';
  return this;
}

// ===== sub_4A6B60 @ 0x004A6B60..0x004A6BA7 =====
int (__thiscall ***__thiscall sub_4A6B60(_DWORD *this))(_DWORD, int)
{
  *this = &CDSStaticSpeaker::`vftable';
  return sub_4A6A20(this);
}

// ===== sub_4A6BB0 @ 0x004A6BB0..0x004A6CEC =====
int __thiscall sub_4A6BB0(int this, _DWORD *a2, int a3)
{
  unsigned __int16 v4; // ax
  int result; // eax
  int v6; // edx
  unsigned int v7; // [esp+18h] [ebp-4h]

  *(_DWORD *)a3 = 0;
  *(_DWORD *)(a3 + 4) = 0;
  *(_DWORD *)(a3 + 8) = 0;
  *(_DWORD *)(a3 + 12) = 0;
  *(_WORD *)a3 = 1;
  *(_WORD *)(a3 + 2) = *(_WORD *)(this + 24);
  *(_DWORD *)(a3 + 4) = *(_DWORD *)(this + 16);
  v4 = *(_WORD *)(this + 24) * *(_WORD *)(this + 36);
  *(_WORD *)(a3 + 12) = v4;
  *(_DWORD *)(a3 + 8) = *(_DWORD *)(this + 16) * v4;
  *(_WORD *)(a3 + 14) = *(_WORD *)(this + 20);
  *(_DWORD *)(this + 96) = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 108) + 12))(*(_DWORD *)(this + 108));
  a2[2] = 0;
  a2[3] = 0;
  a2[4] = 0;
  a2[5] = 0;
  a2[6] = 0;
  a2[7] = 0;
  a2[8] = 0;
  *a2 = 36;
  a2[1] = 98536;
  if ( 1.0 == *(double *)(this + 120) )
  {
    result = *(_DWORD *)(this + 96) * *(unsigned __int16 *)(a3 + 12);
    a2[4] = a3;
    a2[2] = result;
  }
  else
  {
    v7 = (__int64)((double)*(unsigned int *)(this + 16) * 0.03);
    result = v7
           * ((unsigned int)(__int64)((double)*(unsigned int *)(this + 96) / *(double *)(this + 120) + 0.9) / v7 + 1);
    *(_DWORD *)(this + 128) = result;
    v6 = result * *(unsigned __int16 *)(a3 + 12);
    a2[4] = a3;
    a2[2] = v6;
  }
  return result;
}

// ===== sub_4A6CF0 @ 0x004A6CF0..0x004A6EDF =====
int __usercall sub_4A6CF0@<eax>(int a1@<ecx>, int a2@<ebx>)
{
  unsigned int v3; // edi
  void *v4; // ebx
  void *v5; // eax
  int v6; // ecx
  void (__thiscall *v7)(int, void *, _DWORD, int); // edx
  double v8; // st7
  size_t v9; // ecx
  double v10; // st7
  unsigned int v11; // ebx
  unsigned int v12; // eax
  size_t v13; // eax
  char *v15; // edi
  size_t v16; // [esp-Ch] [ebp-44h]
  int v18; // [esp+Ch] [ebp-2Ch]
  int v19; // [esp+14h] [ebp-24h]
  void *v20; // [esp+18h] [ebp-20h] BYREF
  void *v21; // [esp+1Ch] [ebp-1Ch] BYREF
  size_t v22; // [esp+20h] [ebp-18h] BYREF
  size_t v23; // [esp+24h] [ebp-14h]
  size_t Size; // [esp+28h] [ebp-10h] BYREF
  void *v25; // [esp+2Ch] [ebp-Ch]
  void *Src; // [esp+30h] [ebp-8h]
  char *v27; // [esp+34h] [ebp-4h]

  if ( 1.0 == *(double *)(a1 + 120) )
    return sub_4A62B0(a1);
  v3 = *(_DWORD *)(a1 + 24) * *(_DWORD *)(a1 + 36);
  v4 = operator new(v3 * *(_DWORD *)(a1 + 96));
  v16 = v3 * *(_DWORD *)(a1 + 128);
  Src = v4;
  v5 = operator new(v16);
  v6 = *(_DWORD *)(a1 + 108);
  v7 = *(void (__thiscall **)(int, void *, _DWORD, int))(*(_DWORD *)v6 + 8);
  v25 = v5;
  v7(v6, v4, *(_DWORD *)(a1 + 96), a2);
  v8 = (double)*(unsigned int *)(a1 + 16);
  v9 = v3 * (__int64)(v8 * 0.03);
  v10 = (double)(unsigned int)(__int64)(v8 * 0.03);
  v23 = v9;
  v11 = 0;
  v12 = v3 * *(_DWORD *)(a1 + 96);
  v19 = v3 * (__int64)(v10 * *(double *)(a1 + 120) - v10);
  if ( v12 )
  {
    v18 = v9 + v3 * (__int64)(v10 * *(double *)(a1 + 120) - v10);
    v27 = (char *)v25;
    while ( 1 )
    {
      v13 = v12 - v11;
      if ( v13 > v9 )
        v13 = v9;
      if ( v11 )
        (*(void (__thiscall **)(int, char *, char *, char *, size_t))(*(_DWORD *)a1 + 60))(
          a1,
          (char *)Src + v11 - v19,
          (char *)Src + v11,
          v27,
          v13 / v3);
      else
        memcpy_0(v27, Src, v13);
      v27 += v23;
      v11 += v18;
      v12 = v3 * *(_DWORD *)(a1 + 96);
      if ( v11 >= v12 )
        break;
      v9 = v23;
    }
  }
  if ( (*(int (__stdcall **)(_DWORD, _DWORD, unsigned int, void **, size_t *, void **, size_t *))(**(_DWORD **)(a1 + 92)
                                                                                                + 44))(
         *(_DWORD *)(a1 + 92),
         0,
         v3 * *(_DWORD *)(a1 + 128),
         &v20,
         &Size,
         &v21,
         &v22) )
  {
    operator delete(Src);
    operator delete(v25);
    return 17;
  }
  else
  {
    v15 = (char *)v25;
    memcpy_0(v20, v25, Size);
    memcpy_0(v21, &v15[Size], v22);
    (*(void (__stdcall **)(_DWORD, void *, size_t, void *, size_t))(**(_DWORD **)(a1 + 92) + 76))(
      *(_DWORD *)(a1 + 92),
      v20,
      Size,
      v21,
      v22);
    operator delete(Src);
    operator delete(v15);
    return 0;
  }
}

// ===== sub_4A6EE0 @ 0x004A6EE0..0x004A6F06 =====
int __thiscall sub_4A6EE0(_DWORD *this, int a2, int a3, int a4, int a5)
{
  int v5; // edx
  int result; // eax

  v5 = this[5];
  result = 0;
  if ( v5 == 8 )
    return (*(int (__thiscall **)(_DWORD *, int, int, int, int))(*this + 64))(this, a2, a3, a4, a5);
  if ( v5 == 16 )
    return (*(int (__thiscall **)(_DWORD *, int, int, int, int))(*this + 68))(this, a2, a3, a4, a5);
  return result;
}

// ===== sub_4A6F10 @ 0x004A6F10..0x004A7031 =====
int __thiscall sub_4A6F10(_DWORD *this, int a2, int a3, int a4, unsigned int a5)
{
  unsigned int v6; // ecx
  double v7; // st6
  unsigned int v8; // edx
  int v9; // esi
  double v10; // st5
  int v11; // eax
  double v12; // rt0
  double v13; // rt1
  double v14; // st5
  double v15; // st6
  double v16; // st5
  double v17; // st4
  _BYTE *v18; // edi
  unsigned __int8 *v19; // esi
  int v20; // eax
  bool v21; // sf
  double v22; // st5
  unsigned int v23; // ebx
  int v24; // eax
  unsigned int v25; // ecx
  _BYTE *v26; // eax
  int v28; // [esp+14h] [ebp-Ch]
  _DWORD *v29; // [esp+18h] [ebp-8h]
  unsigned int v30; // [esp+1Ch] [ebp-4h]

  v6 = a5;
  v29 = this;
  v7 = 0.0;
  v8 = a5 / 0xA;
  v9 = 0;
  v28 = 0;
  if ( a5 / 0xA )
  {
    v10 = 1.0;
    v11 = this[6];
    while ( 1 )
    {
      v13 = v10;
      v14 = v7;
      v15 = v13;
      v30 = 0;
      v16 = v14 / ((double)a5 - 1.0);
      v17 = v13 - v16;
      if ( v11 )
      {
        v18 = (_BYTE *)(v9 + a4);
        v19 = (unsigned __int8 *)(v6 + a2 + v9 - v8);
        do
        {
          v20 = (int)((double)(unsigned __int8)v18[a3 - a4] * v16 + (double)*v19 * v17);
          if ( v20 > 255 )
            LOBYTE(v20) = -1;
          *v18 = v20;
          ++v19;
          ++v18;
          ++v30;
        }
        while ( v30 < v29[6] );
        v8 = a5 / 0xA;
        v9 = v28;
        this = v29;
        v6 = a5;
      }
      v11 = this[6];
      v21 = v11 + v9 < 0;
      v9 += v11;
      v22 = (double)v9;
      v28 = v9;
      if ( v21 )
        v22 = v22 + 4294967296.0;
      if ( v9 >= v8 )
        break;
      v12 = v22;
      v10 = v15;
      v7 = v12;
    }
  }
  v23 = v8;
  if ( v8 < v6 )
  {
    v24 = this[6];
    do
    {
      v25 = 0;
      if ( v24 )
      {
        v26 = (_BYTE *)(v23 + a4);
        do
        {
          *v26 = v26[a3 - a4];
          ++v25;
          ++v26;
        }
        while ( v25 < this[6] );
      }
      v24 = this[6];
      v23 += v24;
    }
    while ( v23 < a5 );
  }
  return 0;
}

// ===== sub_4A7040 @ 0x004A7040..0x004A7195 =====
int __thiscall sub_4A7040(unsigned int *this, int a2, int a3, int a4, int a5)
{
  unsigned int v6; // ecx
  unsigned int v7; // ebx
  unsigned int v8; // edx
  unsigned int v9; // esi
  double v10; // st7
  unsigned int v11; // eax
  unsigned int v12; // ebx
  __int16 *v13; // esi
  int i; // eax
  int v15; // eax
  unsigned int v16; // ecx
  unsigned int v17; // edx
  _WORD *v18; // eax
  unsigned int v20; // [esp+10h] [ebp-18h]
  unsigned int v21; // [esp+20h] [ebp-8h]
  unsigned int v22; // [esp+24h] [ebp-4h]

  v6 = this[6];
  v7 = a5 * v6;
  v21 = a5 * v6;
  v8 = 0;
  v9 = v6 * (__int64)((double)this[4] * 0.005);
  v20 = v9;
  v22 = 0;
  if ( v9 )
  {
    v10 = (double)(v9 - 2);
    v11 = this[6];
    do
    {
      v12 = 0;
      if ( v11 )
      {
        v13 = (__int16 *)(a3 + 2 * v8);
        for ( i = a2 - a3; ; i = a2 - a3 )
        {
          v15 = (int)((double)*(__int16 *)((char *)v13 + i) * (1.0 - (double)v8 / v10)
                    + (double)*v13 * ((double)v8 / v10));
          if ( v15 <= 0x7FFF )
          {
            if ( v15 < -32768 )
              LOWORD(v15) = 0x8000;
          }
          else
          {
            LOWORD(v15) = 0x7FFF;
          }
          ++v22;
          *(__int16 *)((char *)v13 + a4 - a3) = v15;
          v11 = this[6];
          ++v12;
          ++v13;
          if ( v12 >= v11 )
            break;
        }
        v9 = v20;
        v8 = v22;
      }
    }
    while ( v8 < v9 );
    v7 = a5 * v6;
  }
  if ( v9 < v7 )
  {
    v16 = this[6];
    do
    {
      v17 = 0;
      if ( v16 )
      {
        v18 = (_WORD *)(a4 + 2 * v9);
        do
        {
          *v18 = *(_WORD *)((char *)v18 + a3 - a4);
          v16 = this[6];
          ++v17;
          ++v9;
          ++v18;
        }
        while ( v17 < v16 );
      }
    }
    while ( v9 < v21 );
  }
  return a5;
}

// ===== sub_4A71A0 @ 0x004A71A0..0x004A71C1 =====
_DWORD *__thiscall sub_4A71A0(_DWORD *this, char a2)
{
  sub_4A6B60(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A71D0 @ 0x004A71D0..0x004A72DB =====
_DWORD *__thiscall sub_4A71D0(_DWORD *this, HWND a2, int a3, int a4)
{
  void *v5; // eax
  void *v6; // eax
  size_t v8; // [esp-18h] [ebp-34h]
  size_t v9; // [esp-8h] [ebp-24h]

  sub_4A63B0((int)this, a2);
  *this = &CDSStreamSpeaker::`vftable';
  sub_4C6970(this + 32);
  this[39] = a3;
  this[41] = a4;
  this[40] = a3 + 1;
  v5 = operator new(4 * (a3 + 1));
  v9 = 4 * this[40];
  this[38] = v5;
  memset(v5, 0, v9);
  v6 = operator new(8 * this[40]);
  v8 = 8 * this[40];
  this[37] = v6;
  memset(v6, 0, v8);
  this[28] = 128;
  unknown_libname_2(this + 32, -1);
  this[46] = 0;
  return this;
}

// ===== sub_4A72E0 @ 0x004A72E0..0x004A739D =====
_DWORD *__thiscall sub_4A72E0(int this, _DWORD *a2, int a3)
{
  unsigned int v3; // edx
  unsigned __int16 v4; // ax
  _DWORD *result; // eax

  v3 = *(_DWORD *)(this + 16) * *(_DWORD *)(this + 164) / 0x3E8u;
  *(_DWORD *)(this + 168) = v3 * *(_DWORD *)(this + 36) * *(_DWORD *)(this + 24);
  *(_DWORD *)(this + 172) = v3;
  *(_DWORD *)a3 = 0;
  *(_DWORD *)(a3 + 4) = 0;
  *(_DWORD *)(a3 + 8) = 0;
  *(_DWORD *)(a3 + 12) = 0;
  *(_WORD *)a3 = 1;
  *(_WORD *)(a3 + 2) = *(_WORD *)(this + 24);
  *(_DWORD *)(a3 + 4) = *(_DWORD *)(this + 16);
  v4 = *(_WORD *)(this + 36) * *(_WORD *)(this + 24);
  *(_WORD *)(a3 + 12) = v4;
  *(_DWORD *)(a3 + 8) = *(_DWORD *)(this + 16) * v4;
  *(_WORD *)(a3 + 14) = *(_WORD *)(this + 20);
  *(_DWORD *)(this + 96) = *(_DWORD *)(this + 172) * *(_DWORD *)(this + 156);
  result = a2;
  a2[2] = 0;
  a2[3] = 0;
  a2[4] = 0;
  a2[5] = 0;
  a2[6] = 0;
  a2[7] = 0;
  a2[8] = 0;
  *a2 = 36;
  a2[1] = 98792;
  a2[2] = *(_DWORD *)(this + 96) * *(unsigned __int16 *)(a3 + 12);
  a2[4] = a3;
  return result;
}

// ===== sub_4A73A0 @ 0x004A73A0..0x004A7442 =====
int __thiscall sub_4A73A0(int this)
{
  unsigned int v2; // edi
  int v4; // [esp+8h] [ebp-10h] BYREF
  unsigned int v5; // [esp+Ch] [ebp-Ch] BYREF
  int v6; // [esp+10h] [ebp-8h] BYREF
  unsigned int v7; // [esp+14h] [ebp-4h] BYREF

  v2 = *(_DWORD *)(this + 24) * *(_DWORD *)(this + 36);
  if ( (*(int (__stdcall **)(_DWORD, _DWORD, unsigned int, int *, unsigned int *, int *, unsigned int *, _DWORD))(**(_DWORD **)(this + 92) + 44))(
         *(_DWORD *)(this + 92),
         0,
         v2 * *(_DWORD *)(this + 172) * (*(_DWORD *)(this + 156) - 1),
         &v4,
         &v5,
         &v6,
         &v7,
         0) )
  {
    return 17;
  }
  (*(void (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(this + 108) + 8))(
    *(_DWORD *)(this + 108),
    v4,
    v5 / v2);
  (*(void (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(this + 108) + 8))(
    *(_DWORD *)(this + 108),
    v6,
    v7 / v2);
  (*(void (__stdcall **)(_DWORD, int, unsigned int, int, unsigned int))(**(_DWORD **)(this + 92) + 76))(
    *(_DWORD *)(this + 92),
    v4,
    v5,
    v6,
    v7);
  return 0;
}

// ===== sub_4A7450 @ 0x004A7450..0x004A746E =====
int __thiscall sub_4A7450(int this, int a2)
{
  int result; // eax

  result = sub_4A6740((_DWORD **)this, a2);
  if ( !result )
    *(_DWORD *)(this + 188) = 0;
  return result;
}

// ===== sub_4A7470 @ 0x004A7470..0x004A7539 =====
int __thiscall sub_4A7470(_DWORD **this, _DWORD *a2)
{
  int v4; // edi
  int v5; // eax
  char v6[4]; // [esp+4h] [ebp-4h] BYREF

  if ( (sub_4A84C0() & 1) != 0 )
  {
    (*(void (__stdcall **)(_DWORD *, char *))(*this[23] + 36))(this[23], v6);
    if ( (v6[0] & 1) != 0 )
    {
      this[47] = a2;
      return 0;
    }
    else
    {
      if ( !a2 )
      {
        (*(void (__stdcall **)(_DWORD *, unsigned int))(*this[23] + 52))(
          this[23],
          (_DWORD)this[39] * ((unsigned int)this[48] / (unsigned int)this[39]));
        v4 = *this[23];
        v5 = ((int (__thiscall *)(_DWORD **))(*this)[14])(this);
        (*(void (__stdcall **)(_DWORD *, _DWORD, _DWORD, int))(v4 + 48))(this[23], 0, 0, v5);
      }
      this[47] = a2;
      return 0;
    }
  }
  else
  {
    (*(void (__cdecl **)(int, int, const char *, int, void *))(*(_DWORD *)dword_509B48 + 4))(
      dword_509B48,
      1,
      "src\\DSStreamSpeaker.cpp",
      187,
      &unk_4DBE0C);
    sub_4A6350(2, 188);
    return 2;
  }
}

// ===== sub_4A7540 @ 0x004A7540..0x004A7544 =====
int __thiscall sub_4A7540(_DWORD *this)
{
  return this[25];
}

// ===== sub_4A7550 @ 0x004A7550..0x004A755D =====
int __thiscall sub_4A7550(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[25] = a2;
  return result;
}

// ===== sub_4A7560 @ 0x004A7560..0x004A764B =====
int __thiscall sub_4A7560(int this)
{
  unsigned int i; // edi
  _DWORD *v3; // edi

  for ( i = 0; i < *(_DWORD *)(this + 160); ++i )
  {
    if ( !*(_DWORD *)(*(_DWORD *)(this + 152) + 4 * i) )
    {
      *(_DWORD *)(*(_DWORD *)(this + 152) + 4 * i) = CreateEventA(0, 0, 0, 0);
      if ( !*(_DWORD *)(*(_DWORD *)(this + 152) + 4 * i) )
        return 257;
      if ( i < *(_DWORD *)(this + 156) )
        *(_DWORD *)(*(_DWORD *)(this + 148) + 8 * i) = i * *(_DWORD *)(this + 168);
      *(_DWORD *)(*(_DWORD *)(this + 148) + 8 * i + 4) = *(_DWORD *)(*(_DWORD *)(this + 152) + 4 * i);
    }
  }
  *(_DWORD *)(*(_DWORD *)(this + 148) + 8 * *(_DWORD *)(this + 156)) = -1;
  v3 = (_DWORD *)(this + 184);
  if ( !(***(int (__stdcall ****)(_DWORD, void *, int))(this + 92))(*(_DWORD *)(this + 92), &unk_4DC228, this + 184) )
    return (*(int (__stdcall **)(_DWORD, _DWORD, _DWORD))(*(_DWORD *)*v3 + 12))(
             *v3,
             *(_DWORD *)(this + 160),
             *(_DWORD *)(this + 148)) != 0
         ? 0x102
         : 0;
  *v3 = 0;
  return 256;
}

// ===== sub_4A7650 @ 0x004A7650..0x004A76B0 =====
int __thiscall sub_4A7650(_DWORD *this)
{
  int v2; // eax
  unsigned int v3; // edi
  int v4; // eax
  bool v5; // zf
  HANDLE *v6; // eax

  v2 = this[46];
  v3 = 0;
  if ( v2 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v2 + 8))(v2);
    this[46] = 0;
  }
  if ( this[40] )
  {
    do
    {
      v4 = this[38];
      v5 = *(_DWORD *)(v4 + 4 * v3) == 0;
      v6 = (HANDLE *)(v4 + 4 * v3);
      if ( !v5 )
      {
        CloseHandle(*v6);
        *(_DWORD *)(this[38] + 4 * v3) = 0;
      }
      ++v3;
    }
    while ( v3 < this[40] );
  }
  return 0;
}

// ===== sub_4A76B0 @ 0x004A76B0..0x004A76F2 =====
int __thiscall sub_4A76B0(_DWORD **this, int a2, unsigned int a3, int a4, unsigned int a5, unsigned int a6)
{
  int v7; // ebx

  v7 = (*(int (__thiscall **)(_DWORD *, int, unsigned int))(*this[27] + 8))(this[27], a2, a3 / a6);
  return v7 + (*(int (__thiscall **)(_DWORD *, int, unsigned int))(*this[27] + 8))(this[27], a4, a5 / a6);
}

// ===== sub_4A7700 @ 0x004A7700..0x004A7939 =====
int __cdecl sub_4A7700(int *a1, _DWORD *a2)
{
  int v2; // esi
  DWORD v3; // ecx
  unsigned int v4; // edi
  unsigned int v5; // ebx
  unsigned int v6; // eax
  unsigned int v7; // eax
  char *v8; // eax
  size_t v9; // edx
  int v11; // [esp+0h] [ebp-40h] BYREF
  char v12[4]; // [esp+10h] [ebp-30h] BYREF
  int v13; // [esp+14h] [ebp-2Ch]
  void *v14; // [esp+18h] [ebp-28h]
  unsigned int v15; // [esp+1Ch] [ebp-24h]
  int v16; // [esp+20h] [ebp-20h]
  void *v17; // [esp+24h] [ebp-1Ch] BYREF
  size_t Size; // [esp+28h] [ebp-18h] BYREF
  void *v19[2]; // [esp+2Ch] [ebp-14h] BYREF
  int v20; // [esp+3Ch] [ebp-4h]

  v19[1] = &v11;
  v14 = a1;
  v2 = *a1;
  v13 = 0;
  v15 = *(_DWORD *)(v2 + 24) * *(_DWORD *)(v2 + 36);
  v16 = -1;
  v20 = 0;
  while ( !*a2 )
  {
    v3 = WaitForMultipleObjects(*(_DWORD *)(v2 + 160), *(const HANDLE **)(v2 + 152), 0, 0x64u);
    if ( v3 != 258 )
    {
      v4 = *(_DWORD *)(v2 + 156);
      if ( v3 == (v16 + 1) % v4 || v16 == -1 )
      {
        v16 = v3;
        v5 = v3 - 1;
        if ( v3 - 1 >= v4 )
          v5 = v4 - 1;
        (*(void (__thiscall **)(_DWORD, _DWORD, unsigned int, _DWORD, void **, size_t *, void **, int **, _DWORD))(**(_DWORD **)(v2 + 92) + 44))(
          *(_DWORD *)(v2 + 92),
          *(_DWORD *)(v2 + 92),
          v5 * *(_DWORD *)(v2 + 168),
          *(_DWORD *)(v2 + 168),
          &v17,
          &Size,
          v19,
          &a1,
          0);
        if ( sub_4A7540((_DWORD *)v2) == 1 )
        {
          v6 = sub_4A76B0((_DWORD **)v2, (int)v17, Size, (int)v19[0], (unsigned int)a1, v15);
          goto LABEL_10;
        }
        memset(v17, 0, Size);
        if ( v19[0] )
          memset(v19[0], 0, (size_t)a1);
        v6 = *(_DWORD *)(v2 + 172);
        if ( *(_DWORD *)(v2 + 176) == v5 )
        {
          (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 16))(v2);
        }
        else
        {
LABEL_10:
          if ( v6 < *(_DWORD *)(v2 + 172) )
          {
            v7 = v15 * v6;
            if ( v7 > Size )
            {
              v9 = (size_t)a1 + Size - v7;
              v8 = (char *)v19[0] + v7 - Size;
LABEL_19:
              memset(v8, 0, v9);
            }
            else
            {
              memset((char *)v17 + v7, 0, Size - v7);
              v8 = (char *)v19[0];
              if ( v19[0] )
              {
                v9 = (size_t)a1;
                goto LABEL_19;
              }
            }
            sub_4A7550((_DWORD *)v2, 2);
            *(_DWORD *)(v2 + 176) = v5;
          }
          (*(void (__stdcall **)(_DWORD, void *, size_t, void *, int *))(**(_DWORD **)(v2 + 92) + 76))(
            *(_DWORD *)(v2 + 92),
            v17,
            Size,
            v19[0],
            a1);
          if ( *(_DWORD *)(v2 + 188) )
          {
            (*(void (__stdcall **)(_DWORD))(**(_DWORD **)(v2 + 92) + 72))(*(_DWORD *)(v2 + 92));
            (*(void (__stdcall **)(_DWORD, int, char *))(**(_DWORD **)(v2 + 92) + 16))(
              *(_DWORD *)(v2 + 92),
              v2 + 192,
              v12);
          }
        }
      }
    }
  }
  v20 = -1;
  operator delete(v14);
  return v13;
}

// ===== sub_4A7940 @ 0x004A7940..0x004A79B5 =====
int __userpurge sub_4A7940@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, _DWORD **a3)
{
  int result; // eax
  _DWORD *v5; // eax

  result = sub_4A65A0(a1, a2, a3);
  if ( !result )
  {
    result = sub_4A7560((int)a1);
    if ( !result )
    {
      v5 = operator new(4u);
      if ( v5 )
        *v5 = 0;
      else
        v5 = 0;
      *v5 = a1;
      sub_4C6A50((int)sub_4A7700, (int)v5, 15, 0, 0, 0);
      a1[48] = 0;
      a1[47] = 0;
      sub_4A7550(a1, 1);
      return 0;
    }
  }
  return result;
}

// ===== sub_4A79C0 @ 0x004A79C0..0x004A79E5 =====
int __thiscall sub_4A79C0(int this)
{
  int result; // eax
  int v3; // edx

  result = sub_4A67C0((_DWORD **)this);
  if ( !result )
  {
    *(_DWORD *)(this + 176) = -1;
    sub_4A7550((_DWORD *)this, 1);
    return v3;
  }
  return result;
}

// ===== sub_4A79F0 @ 0x004A79F0..0x004A7A16 =====
int __thiscall sub_4A79F0(_DWORD *this)
{
  int v2; // edi

  sub_4C69C0(0xFFFFFFFF);
  v2 = sub_4A6560(this);
  sub_4A7650(this);
  return v2;
}

// ===== sub_4A7A20 @ 0x004A7A20..0x004A7AC5 =====
int (__thiscall ***__thiscall sub_4A7A20(int this))(_DWORD, int)
{
  void (__thiscall ***v2)(_DWORD, int); // eax
  void *v4; // [esp-8h] [ebp-20h]

  *(_DWORD *)this = &CDSStreamSpeaker::`vftable';
  v2 = (void (__thiscall ***)(_DWORD, int))sub_4A79F0((_DWORD *)this);
  if ( v2 )
    (**v2)(v2, 1);
  operator delete(*(void **)(this + 152));
  v4 = *(void **)(this + 148);
  *(_DWORD *)(this + 152) = 0;
  operator delete(v4);
  *(_DWORD *)(this + 148) = 0;
  sub_4C6A40(this + 128);
  return sub_4A6A20((_DWORD *)this);
}

// ===== sub_4A7AD0 @ 0x004A7AD0..0x004A7AF1 =====
void *__thiscall sub_4A7AD0(void *this, char a2)
{
  sub_4A7A20((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A7B00 @ 0x004A7B00..0x004A7B58 =====
_DWORD *__thiscall sub_4A7B00(_DWORD *this)
{
  sub_4A82B0();
  *this = &CBurikoWaveBoxADPCM4Model::`vftable';
  this[46] = 0;
  return this;
}

// ===== sub_4A7B60 @ 0x004A7B60..0x004A7BB6 =====
int __thiscall sub_4A7B60(int this, int a2, double a3)
{
  int v4; // eax
  size_t v6; // [esp-8h] [ebp-8h]

  v4 = *(_DWORD *)(this + 36);
  *(double *)(this + 72) = a3;
  v6 = (unsigned int)(v4 + 2) >> 2;
  *(_DWORD *)(this + 188) = v6;
  *(_DWORD *)(this + 184) = operator new(v6);
  *(_DWORD *)(this + 196) = 0;
  *(_DWORD *)(this + 168) = 0;
  *(_DWORD *)(this + 176) = 0;
  *(_DWORD *)(this + 172) = 127;
  *(_DWORD *)(this + 180) = 127;
  return 0;
}

// ===== sub_4A7BC0 @ 0x004A7BC0..0x004A7BDE =====
void __thiscall sub_4A7BC0(void **this)
{
  operator delete(this[46]);
  this[46] = 0;
}

// ===== sub_4A7BE0 @ 0x004A7BE0..0x004A7BF2 =====
BOOL __stdcall sub_4A7BE0(int a1)
{
  return *(_DWORD *)(a1 + 48) == 0;
}

// ===== sub_4A7C00 @ 0x004A7C00..0x004A7C71 =====
int __thiscall sub_4A7C00(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax
  int v6; // edi
  unsigned int v7; // ebx
  int v8; // eax
  unsigned int i; // [esp+1Ch] [ebp+10h]

  result = a4 * this[25];
  v6 = 0;
  v7 = 0;
  for ( i = result; v7 < i; ++v7 )
  {
    if ( (v7 & 1) != 0 )
      v8 = *(unsigned __int8 *)((v7 >> 1) + a3) >> 4;
    else
      v8 = *(_BYTE *)((v7 >> 1) + a3) & 0xF;
    result = sub_4AAAB0(v8, &this[2 * v6 + 42]);
    *(_WORD *)(a2 + 2 * v7) = result;
    if ( this[25] == 2 )
      v6 ^= 1u;
  }
  return result;
}

// ===== sub_4A7C80 @ 0x004A7C80..0x004A7CD9 =====
int __thiscall sub_4A7C80(void **this)
{
  *this = &CBurikoWaveBoxADPCM4Model::`vftable';
  sub_4A7BC0(this);
  return sub_4A8540();
}

// ===== sub_4A7CE0 @ 0x004A7CE0..0x004A7E72 =====
unsigned int __userpurge sub_4A7CE0@<eax>(int a1@<ecx>, int a2@<ebx>, int a3@<edi>, int a4, unsigned int a5)
{
  unsigned int v6; // ecx
  unsigned int v7; // ebx
  bool v8; // al
  BOOL v9; // ecx
  unsigned int result; // eax
  unsigned int v11; // edi
  int v12; // eax
  unsigned int v15; // [esp+4h] [ebp-18h]
  unsigned int v16; // [esp+Ch] [ebp-10h]
  unsigned int v17; // [esp+10h] [ebp-Ch]
  unsigned int v18; // [esp+14h] [ebp-8h]
  unsigned int v19; // [esp+18h] [ebp-4h]
  int v20; // [esp+28h] [ebp+Ch]

  if ( *(_DWORD *)(a1 + 196) )
  {
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a4 + 8))(a4, a1 + 192, 2);
    v6 = a5 - 1;
  }
  else
  {
    v6 = a5;
  }
  v18 = *(_DWORD *)(a1 + 92) - *(_DWORD *)(a1 + 148);
  if ( v6 <= v18 )
    v18 = v6;
  v7 = (*(_DWORD *)(a1 + 36) - (*(_DWORD *)(a1 + 36) & 3u)) / *(_DWORD *)(a1 + 20);
  v16 = v7;
  v20 = *(_DWORD *)(a1 + 32);
  v8 = *(_DWORD *)(a1 + 100) == 1 && (v6 & 1) != 0;
  v9 = v8;
  result = 0;
  *(_DWORD *)(a1 + 196) = v9;
  v19 = 0;
  if ( v18 )
  {
    while ( 1 )
    {
      v17 = v18 - result;
      if ( v7 <= v18 - result )
        v17 = v7;
      else
        v7 = v18 - result + (((_BYTE)v18 - (_BYTE)result) & 1) * (*(_DWORD *)(a1 + 100) & 1);
      (*(void (__thiscall **)(_DWORD, _DWORD, unsigned int, int, int))(**(_DWORD **)(a1 + 144) + 8))(
        *(_DWORD *)(a1 + 144),
        *(_DWORD *)(a1 + 184),
        (v7 * *(_DWORD *)(a1 + 20)) >> 2,
        a3,
        a2);
      sub_4A7C00((_DWORD *)a1, v20, *(_DWORD *)(a1 + 184), v7);
      v11 = 0;
      v15 = v7 * *(_DWORD *)(a1 + 16);
      if ( v15 )
      {
        do
        {
          v12 = (int)((double)*(__int16 *)(v20 + 2 * v11) * *(double *)(a1 + 72));
          if ( v12 >= -32768 )
          {
            if ( v12 > 0x7FFF )
              LOWORD(v12) = 0x7FFF;
          }
          else
          {
            LOWORD(v12) = 0x8000;
          }
          *(_WORD *)(v20 + 2 * v11++) = v12;
        }
        while ( v11 < v15 );
      }
      a2 = v17 * *(_DWORD *)(a1 + 20);
      a3 = v20;
      (*(void (__thiscall **)(int))(*(_DWORD *)a4 + 8))(a4);
      v19 += v17;
      if ( *(_DWORD *)(a1 + 196) && v17 != v7 )
        *(_WORD *)(a1 + 192) = *(_WORD *)(v20 + 2 * v17);
      result = v19;
      if ( v19 >= v18 )
        break;
      v7 = v16;
    }
  }
  *(_DWORD *)(a1 + 148) += result;
  if ( *(_DWORD *)(a1 + 148) == *(_DWORD *)(a1 + 92) )
    *(_DWORD *)(a1 + 196) = 0;
  return result;
}

// ===== sub_4A7E80 @ 0x004A7E80..0x004A7EA1 =====
void **__thiscall sub_4A7E80(void **this, char a2)
{
  sub_4A7C80(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A7EB0 @ 0x004A7EB0..0x004A7ECD =====
_DWORD *__thiscall sub_4A7EB0(_DWORD *this)
{
  _DWORD *result; // eax

  result = this;
  *this = &CWaveDevice::`vftable';
  this[1] = 0;
  this[2] = 0;
  this[3] = 0;
  this[4] = 0;
  this[5] = 0;
  this[6] = 0;
  return result;
}

// ===== sub_4A7ED0 @ 0x004A7ED0..0x004A7EF1 =====
int __thiscall sub_4A7ED0(_DWORD *this)
{
  int (__thiscall ***v2)(_DWORD, int); // ecx
  int result; // eax

  v2 = (int (__thiscall ***)(_DWORD, int))this[1];
  *this = &CWaveDevice::`vftable';
  if ( v2 )
    result = (**v2)(v2, 1);
  this[1] = 0;
  return result;
}

// ===== sub_4A7F00 @ 0x004A7F00..0x004A7F3A =====
int __thiscall sub_4A7F00(_DWORD *this, int a2)
{
  _DWORD *v3; // eax

  this[1] = a2;
  v3 = (_DWORD *)(*(int (__thiscall **)(int))(*(_DWORD *)a2 + 16))(a2);
  this[2] = *v3;
  this[3] = v3[1];
  this[4] = v3[2];
  this[5] = v3[3];
  this[6] = v3[4];
  return 1;
}

// ===== sub_4A7F40 @ 0x004A7F40..0x004A7F44 =====
char *__thiscall sub_4A7F40(char *this)
{
  return this + 8;
}

// ===== sub_4A7F50 @ 0x004A7F50..0x004A7F62 =====
int __thiscall sub_4A7F50(_DWORD **this)
{
  int result; // eax

  result = 0;
  if ( this[1] )
    return (*(int (__thiscall **)(_DWORD *))(*this[1] + 12))(this[1]);
  return result;
}

// ===== sub_4A7F70 @ 0x004A7F70..0x004A7F89 =====
int __thiscall sub_4A7F70(_DWORD **this, int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( this[1] )
    return (*(int (__thiscall **)(_DWORD *, int, int))(*this[1] + 20))(this[1], a2, a3);
  return result;
}

// ===== sub_4A7F90 @ 0x004A7F90..0x004A7FA2 =====
int __thiscall sub_4A7F90(_DWORD **this)
{
  int result; // eax

  result = 0;
  if ( this[1] )
    return (*(int (__thiscall **)(_DWORD *))(*this[1] + 24))(this[1]);
  return result;
}

// ===== sub_4A7FB0 @ 0x004A7FB0..0x004A7FD4 =====
int __cdecl sub_4A7FB0(_DWORD *a1, unsigned int a2, int a3, int a4)
{
  int result; // eax

  a1[1] = a2;
  result = a3 * (a2 >> 3);
  *a1 = a4;
  a1[2] = a3;
  a1[3] = result;
  return result;
}

// ===== sub_4A7FE0 @ 0x004A7FE0..0x004A8001 =====
_DWORD *__thiscall sub_4A7FE0(_DWORD *this, char a2)
{
  sub_4A7ED0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}
