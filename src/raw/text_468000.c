#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_468140 @ 0x00468140..0x0046830D =====
int __fastcall sub_468140(size_t *a1, CHAR *a2, void *a3, LPDWORD lpFileSizeHigh)
{
  char *v5; // eax
  CHAR *v6; // ecx
  char v7; // dl
  HANDLE FileA; // ebx
  DWORD FileSize; // eax
  SIZE_T i; // esi
  void *v11; // eax
  void *v12; // edi
  size_t v13; // eax
  DWORD NumberOfFreeClusters; // [esp+8h] [ebp-330h] BYREF
  DWORD SectorsPerCluster; // [esp+Ch] [ebp-32Ch] BYREF
  DWORD TotalNumberOfClusters; // [esp+10h] [ebp-328h] BYREF
  size_t *v18; // [esp+14h] [ebp-324h]
  DWORD NumberOfBytesRead; // [esp+18h] [ebp-320h] BYREF
  DWORD BytesPerSector; // [esp+1Ch] [ebp-31Ch] BYREF
  void *v21; // [esp+20h] [ebp-318h]
  int v22; // [esp+24h] [ebp-314h]
  CHAR RootPathName[780]; // [esp+28h] [ebp-310h] BYREF

  v18 = a1;
  v21 = a3;
  v22 = -1;
  v5 = a2;
  v6 = (CHAR *)(RootPathName - a2);
  do
  {
    v7 = *v5;
    v5[(_DWORD)v6] = *v5;
    ++v5;
  }
  while ( v7 );
  sub_42EA80((int)v6, RootPathName);
  if ( (unsigned __int8)(RootPathName[0] - 97) > 0x19u || RootPathName[1] != 58 )
    return 8;
  sprintf(RootPathName, "%c:\\", RootPathName[0]);
  if ( !GetDiskFreeSpaceA(
          RootPathName,
          &SectorsPerCluster,
          &BytesPerSector,
          &NumberOfFreeClusters,
          &TotalNumberOfClusters) )
    return -1;
  FileA = CreateFileA(a2, 0x80000000, 1u, 0, 3u, 0x28000020u, 0);
  if ( FileA == (HANDLE)-1 )
    return 1;
  FileSize = (DWORD)lpFileSizeHigh;
  if ( !lpFileSizeHigh )
  {
    FileSize = GetFileSize(FileA, 0);
    lpFileSizeHigh = (LPDWORD)FileSize;
  }
  for ( i = BytesPerSector; i < FileSize; i *= 2 )
    ;
  v11 = VirtualAlloc(0, i, 0x3000u, 4u);
  v12 = v11;
  if ( v11 )
  {
    if ( ReadFile(FileA, v11, i, &NumberOfBytesRead, 0) )
    {
      v13 = NumberOfBytesRead;
      if ( NumberOfBytesRead >= (unsigned int)lpFileSizeHigh )
        v13 = (size_t)lpFileSizeHigh;
      *v18 = v13;
      memcpy_0(v21, v12, v13);
      v22 = 0;
    }
    VirtualFree(v12, 0, 0x8000u);
  }
  CloseHandle(FileA);
  return v22;
}

// ===== sub_468310 @ 0x00468310..0x00468411 =====
unsigned int __cdecl sub_468310(const char *a1)
{
  CHAR *v1; // ecx
  const char *v2; // esi
  CHAR *v3; // edi
  unsigned int result; // eax
  unsigned int v5; // [esp+Ch] [ebp-314h] BYREF
  char Buffer[780]; // [esp+10h] [ebp-310h] BYREF

  v2 = v1;
  v5 = 0;
  v3 = v1;
  if ( a1 )
  {
    sub_467DD0(&v5, &::Buffer, v1, 0, 0, 0);
    result = v5;
    if ( v5 )
      return result;
    sub_4649F0(a1, &::Buffer, Buffer);
    result = sub_467EC0((int)Buffer, v2, 0, 0, 0);
    if ( result == -2147483632 )
    {
      if ( !sub_464B80(byte_517C08) )
        return 0;
      sub_4649F0(a1, byte_517C08, Buffer);
      result = sub_467EC0((int)Buffer, v2, 0, 0, 0);
    }
    if ( result < 0x80000000 )
      return result;
    return 0;
  }
  if ( sub_467DD0(&v5, &::Buffer, v1, 0, 0, 0) )
    sub_467DD0(&v5, byte_517C08, v3, 0, 0, 0);
  return v5;
}

// ===== sub_468420 @ 0x00468420..0x004685C5 =====
int __usercall sub_468420@<eax>(LONG a1@<eax>, DWORD *lpBuffer, const CHAR *lpszUrl, DWORD dwNumberOfBytesToRead)
{
  void *v5; // eax
  void *v6; // edi
  int v7; // esi
  void *v9; // esi
  BOOL v10; // ebx
  void *hInternet; // [esp+8h] [ebp-10h]
  DWORD v12; // [esp+Ch] [ebp-Ch] BYREF
  DWORD dwNumberOfBytesRead; // [esp+10h] [ebp-8h] BYREF
  DWORD dwNumberOfBytesAvailable; // [esp+14h] [ebp-4h] BYREF

  if ( InternetAttemptConnect(0) )
    return -1;
  v5 = InternetOpenA("Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )", 0, 0, 0, 0);
  hInternet = v5;
  if ( !v5 )
    return -1;
  v6 = InternetOpenUrlA(v5, lpszUrl, 0, 0, 0x80000000, 0);
  if ( v6 )
  {
    if ( dwNumberOfBytesToRead )
    {
      if ( a1 )
        InternetSetFilePointer(v6, a1, 0, 0, 0);
      if ( InternetReadFile(v6, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead) && dwNumberOfBytesRead )
      {
        v7 = dwNumberOfBytesToRead != dwNumberOfBytesRead ? 3 : 0;
        InternetCloseHandle(v6);
        InternetCloseHandle(hInternet);
        return v7;
      }
      else
      {
        InternetCloseHandle(v6);
        InternetCloseHandle(hInternet);
        return 2;
      }
    }
    else if ( a1 )
    {
      InternetCloseHandle(v6);
      InternetCloseHandle(hInternet);
      return 3;
    }
    else
    {
      dwNumberOfBytesRead = 0;
      for ( dwNumberOfBytesAvailable = 0;
            InternetQueryDataAvailable(v6, &dwNumberOfBytesAvailable, 0, 0);
            dwNumberOfBytesAvailable = 0 )
      {
        if ( !dwNumberOfBytesAvailable )
          break;
        v9 = operator new[](dwNumberOfBytesAvailable);
        v10 = InternetReadFile(v6, v9, dwNumberOfBytesAvailable, &v12);
        operator delete[](v9);
        if ( !v10 )
          break;
        dwNumberOfBytesRead += v12;
      }
      *lpBuffer = dwNumberOfBytesRead;
      InternetCloseHandle(v6);
      InternetCloseHandle(hInternet);
      return 0;
    }
  }
  else
  {
    InternetCloseHandle(hInternet);
    return 1;
  }
}

// ===== sub_4685D0 @ 0x004685D0..0x0046866B =====
BOOL __usercall sub_4685D0@<eax>(const char *a1@<edi>)
{
  int v1; // esi
  char Buffer[780]; // [esp+4h] [ebp-310h] BYREF

  sub_4649F0(a1, &::Buffer, Buffer);
  v1 = (*(int (__thiscall **)(int, char *))(*(_DWORD *)dword_566754 + 4))(dword_566754, Buffer);
  if ( v1 == -2147483632 && sub_464B80(byte_517C08) )
  {
    sub_4649F0(a1, byte_517C08, Buffer);
    v1 = (*(int (__thiscall **)(int, char *))(*(_DWORD *)dword_566754 + 4))(dword_566754, Buffer);
  }
  return v1 != 0;
}

// ===== sub_468670 @ 0x00468670..0x004686C3 =====
BOOL __usercall sub_468670@<eax>(char *a1@<eax>, LPSTR lpVolumeNameBuffer)
{
  UINT v2; // edi
  BOOL VolumeInformationA; // ebx
  CHAR RootPathName[4]; // [esp+Ch] [ebp-4h] BYREF

  sprintf(RootPathName, "%c:\\", *a1);
  v2 = SetErrorMode(1u);
  VolumeInformationA = GetVolumeInformationA(RootPathName, lpVolumeNameBuffer, 0x30Cu, 0, 0, 0, 0, 0);
  SetErrorMode(v2);
  return VolumeInformationA;
}

// ===== sub_4686D0 @ 0x004686D0..0x00468792 =====
int __usercall sub_4686D0@<eax>(_DWORD *a1@<edi>, const char *a2)
{
  unsigned int v2; // kr00_4
  int result; // eax
  ULARGE_INTEGER FreeBytesAvailableToCaller; // [esp+0h] [ebp-318h] BYREF
  CHAR DirectoryName[780]; // [esp+8h] [ebp-310h] BYREF

  strcpy(DirectoryName, a2);
  v2 = strlen(DirectoryName);
  if ( v2 && *((_BYTE *)&FreeBytesAvailableToCaller.QuadPart + v2 + 7) != 92 )
    *(_WORD *)&DirectoryName[v2] = 92;
  if ( !sub_464B80(DirectoryName) || !GetDiskFreeSpaceExA(DirectoryName, &FreeBytesAvailableToCaller, 0, 0) )
    return 0;
  result = 1;
  *a1 = FreeBytesAvailableToCaller.QuadPart >> 20;
  return result;
}

// ===== sub_4687A0 @ 0x004687A0..0x0046884C =====
int __cdecl sub_4687A0(LPSYSTEMTIME lpSystemTime, LPSYSTEMTIME a2, LPSYSTEMTIME a3, const CHAR *a4)
{
  int v4; // edi
  int v6[3]; // [esp+Ch] [ebp-30h] BYREF
  FILETIME v7; // [esp+18h] [ebp-24h] BYREF
  FILETIME LastAccessTime; // [esp+20h] [ebp-1Ch] BYREF
  FILETIME FileTime; // [esp+28h] [ebp-14h] BYREF
  int v10; // [esp+38h] [ebp-4h]

  v4 = 0;
  sub_42D3B0(v6);
  v10 = 0;
  if ( sub_42D520(a4, (int)v6) )
  {
    if ( sub_42D660(&LastAccessTime, &v7, (int)v6, &FileTime) )
    {
      FileTimeToSystemTime(&FileTime, lpSystemTime);
      FileTimeToSystemTime(&LastAccessTime, a2);
      FileTimeToSystemTime(&v7, a3);
      v4 = 1;
    }
    sub_42D5B0((int)v6);
  }
  v10 = -1;
  sub_42D400(v6);
  return v4;
}

// ===== sub_468850 @ 0x00468850..0x004688F4 =====
BOOL __cdecl sub_468850(SYSTEMTIME *lpSystemTime, SYSTEMTIME *a2, SYSTEMTIME *a3)
{
  BOOL v3; // edi
  _DWORD *v4; // eax
  LPCSTR v5; // ecx
  int v7[3]; // [esp+Ch] [ebp-30h] BYREF
  FILETIME LastAccessTime; // [esp+18h] [ebp-24h] BYREF
  struct _FILETIME v9; // [esp+20h] [ebp-1Ch] BYREF
  struct _FILETIME FileTime; // [esp+28h] [ebp-14h] BYREF
  int v11; // [esp+38h] [ebp-4h]

  v3 = 0;
  v4 = sub_42D3B0(v7);
  v11 = 0;
  if ( sub_42D570(v5, (int)v4, 1) )
  {
    SystemTimeToFileTime(lpSystemTime, &FileTime);
    SystemTimeToFileTime(a2, &LastAccessTime);
    SystemTimeToFileTime(a3, &v9);
    v3 = sub_42D680(&LastAccessTime, &v9, (int)v7, &FileTime);
    sub_42D5B0((int)v7);
  }
  v11 = -1;
  sub_42D400(v7);
  return v3;
}

// ===== sub_468900 @ 0x00468900..0x00468A6C =====
int sub_468900()
{
  int v0; // edi
  bool v1; // zf
  const CHAR *v2; // eax
  int v3; // esi
  int v4; // ecx
  int i; // edx
  int v7[7]; // [esp+10h] [ebp-128h] BYREF
  int v8; // [esp+2Ch] [ebp-10Ch]
  int v9; // [esp+30h] [ebp-108h]
  int v10; // [esp+34h] [ebp-104h]
  int v11[43]; // [esp+38h] [ebp-100h] BYREF
  _DWORD v12[9]; // [esp+E4h] [ebp-54h]
  _DWORD v13[8]; // [esp+108h] [ebp-30h] BYREF
  int v14; // [esp+134h] [ebp-4h]

  v0 = 0;
  v9 = 0;
  sub_42D6A0(v11);
  v14 = 0;
  v1 = sub_46F710() == 0;
  v2 = (const CHAR *)&unk_4E6F4C;
  if ( v1 )
    v2 = "MS Gothic";
  if ( !sub_42DDD0(v2) )
  {
    v12[3] = 5;
    v12[7] = 5;
    v0 = 1;
    v12[2] = 4;
    v12[6] = 4;
    v12[0] = 2;
    v12[1] = 3;
    v12[4] = 2;
    v12[5] = 3;
    v12[8] = -1;
    memset(v13, 0, 16);
    v13[4] = 1;
    v13[5] = 1;
    v13[6] = 1;
    v13[7] = 1;
    v10 = 0;
    sub_42E970(v7, 0x61u, v11);
    v8 = sub_42E9B0((int)v11);
    v3 = 0;
    v4 = 0;
    for ( i = 2; i >= 0; i = v12[v3] )
    {
      if ( *(_BYTE *)(i + v8 * v13[v4] + v7[2]) )
        ++v10;
      v4 = ++v3;
    }
    if ( v10 == v3 )
    {
      sub_42DC60();
      v0 = v9;
    }
  }
  v14 = -1;
  sub_42D6F0((void **)v11);
  return v0;
}

// ===== sub_468A70 @ 0x00468A70..0x00468B2B =====
int __usercall sub_468A70@<eax>(int a1@<eax>, const char *a2@<ecx>)
{
  int v3; // eax
  void *v4; // esi
  _BYTE *v5; // eax
  const char *v6; // ecx
  _BYTE *v7; // edx
  char v8; // al

  if ( a1 )
  {
    v3 = a1 - 1;
    if ( v3 )
      goto LABEL_6;
  }
  else
  {
    v3 = 128;
  }
  sub_42D780(a2, (const char *)v3);
LABEL_6:
  v4 = dword_56631C;
  if ( dword_56631C )
  {
    while ( strcmp(*((const char **)v4 + 1), a2) )
    {
      v4 = (void *)*((_DWORD *)v4 + 2);
      if ( !v4 )
        goto LABEL_9;
    }
  }
  else
  {
LABEL_9:
    v4 = operator new(0xCu);
    *(_DWORD *)v4 = dword_566314;
    v5 = operator new[](strlen(a2) + 1);
    *((_DWORD *)v4 + 2) = dword_56631C;
    *((_DWORD *)v4 + 1) = v5;
    v6 = a2;
    v7 = v5;
    do
    {
      v8 = *v6;
      *v7++ = *v6++;
    }
    while ( v8 );
    ++dword_566314;
    dword_56631C = v4;
  }
  return *(_DWORD *)v4;
}

// ===== sub_468B30 @ 0x00468B30..0x00468B6B =====
void sub_468B30()
{
  void *v0; // esi
  void *v1; // edi
  void *v2; // eax

  v0 = dword_56631C;
  while ( v0 )
  {
    v1 = v0;
    v2 = (void *)*((_DWORD *)v0 + 1);
    v0 = (void *)*((_DWORD *)v0 + 2);
    operator delete[](v2);
    operator delete(v1);
  }
  dword_56631C = 0;
  dword_566314 = 0;
}

// ===== sub_468B70 @ 0x00468B70..0x00468BAB =====
int sub_468B70()
{
  int v0; // eax
  const char *v1; // ecx
  int v2; // eax
  const char *v3; // ecx

  sub_468B30();
  v0 = sub_46F710();
  v1 = (const char *)&unk_4E6F4C;
  if ( !v0 )
    v1 = "MS Gothic";
  sub_468A70(-1, v1);
  v2 = sub_46F710();
  v3 = (const char *)&unk_4E6F68;
  if ( !v2 )
    v3 = "MS Mincho";
  return sub_468A70(-1, v3);
}

// ===== sub_468BB0 @ 0x00468BB0..0x00468BD0 =====
int __fastcall sub_468BB0(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_56631C;
  result = 0;
  if ( dword_56631C )
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

// ===== sub_468BD0 @ 0x00468BD0..0x00468DFA =====
int __cdecl sub_468BD0(DWORD a1)
{
  char *v1; // ecx
  const char **v2; // esi
  char *v3; // edi
  size_t v4; // eax
  DWORD v5; // ebx
  _DWORD *v6; // esi
  _DWORD *v7; // edi
  HANDLE v8; // eax
  _BYTE *v9; // eax
  const char *v10; // ecx
  _BYTE *v11; // edx
  char v12; // al
  _DWORD *v14; // esi
  void *v15; // eax
  _BYTE *v16; // edx
  char *v17; // ecx
  char v18; // al
  _BYTE *v19; // edx
  CHAR *v20; // ecx
  CHAR v21; // al
  const char *v22; // [esp+Ch] [ebp-31Ch]
  DWORD pNumFonts; // [esp+10h] [ebp-318h] BYREF
  int v24; // [esp+14h] [ebp-314h]
  CHAR Buffer[780]; // [esp+18h] [ebp-310h] BYREF

  v2 = (const char **)dword_566640;
  v3 = v1;
  pNumFonts = a1;
  v22 = v1;
  v24 = 0;
  if ( dword_566640 )
  {
    while ( strcmp(*v2, v3) )
    {
      v2 = (const char **)v2[3];
      if ( !v2 )
        goto LABEL_4;
    }
    return 1;
  }
LABEL_4:
  if ( a1 )
  {
    v4 = sub_4662E0(v3);
    v5 = v4;
    if ( v4 )
    {
      v6 = operator new[](v4);
      if ( sub_465AB0(v3, v6, (const char *)pNumFonts) == v5 )
      {
        v7 = operator new(0x10u);
        v8 = AddFontMemResourceEx(v6, v5, 0, &pNumFonts);
        v7[2] = v8;
        if ( v8 )
        {
          v9 = operator new[](strlen(v22) + 1);
          v7[3] = dword_566640;
          *v7 = v9;
          v7[1] = 0;
          v10 = v22;
          v11 = v9;
          do
          {
            v12 = *v10;
            *v11++ = *v10++;
          }
          while ( v12 );
          dword_566640 = v7;
          v24 = 1;
        }
        else
        {
          operator delete(v7);
        }
      }
      operator delete[](v6);
    }
    return v24;
  }
  sprintf(Buffer, "%s%s", &::Buffer, v3);
  if ( AddFontResourceA(Buffer) <= 0 )
    return v24;
  v14 = operator new(0x10u);
  *v14 = operator new[](strlen(v3) + 1);
  v15 = operator new[](strlen(Buffer) + 1);
  v14[3] = dword_566640;
  v16 = (_BYTE *)*v14;
  v14[1] = v15;
  v14[2] = 0;
  v17 = v3;
  do
  {
    v18 = *v17;
    *v16++ = *v17++;
  }
  while ( v18 );
  v19 = (_BYTE *)v14[1];
  v20 = Buffer;
  do
  {
    v21 = *v20;
    *v19++ = *v20++;
  }
  while ( v21 );
  dword_566640 = v14;
  SendMessageA(HWND_BROADCAST, 0x1Du, 0, 0);
  return 1;
}

// ===== sub_468E00 @ 0x00468E00..0x00468E70 =====
void sub_468E00()
{
  void **v0; // esi
  void **v1; // edi
  void *v2; // ecx
  void **v3; // edi
  int v4; // ebx
  void **v5; // esi
  void *v6; // eax

  v3 = (void **)dword_566640;
  v4 = 0;
  if ( dword_566640 )
  {
    do
    {
      v5 = v3;
      v6 = v3[2];
      v3 = (void **)v3[3];
      if ( v6 )
      {
        RemoveFontMemResourceEx(v6);
      }
      else
      {
        RemoveFontResourceA((LPCSTR)v5[1]);
        operator delete[](v5[1]);
        v4 = 1;
      }
      operator delete[](*v5);
      operator delete(v5);
    }
    while ( v3 );
    if ( v4 )
      SendMessageA(HWND_BROADCAST, 0x1Du, 0, 0);
  }
  operator delete(dword_565B4C);
  v0 = (void **)dword_565B5C;
  dword_565B4C = 0;
  if ( dword_565B5C )
  {
    do
    {
      v1 = v0;
      v2 = *v0;
      v0 = (void **)v0[3];
      operator delete(v2);
      operator delete(v1[1]);
      operator delete(v1);
    }
    while ( v0 );
  }
  dword_565B5C = 0;
}

// ===== sub_468E70 @ 0x00468E70..0x00468F42 =====
INT_PTR __cdecl sub_468E70(LPARAM a1, const CHAR *a2)
{
  char *v2; // edi
  LPARAM v3; // ebx
  char *v4; // esi
  INT_PTR v5; // esi
  char *v7; // [esp+Ch] [ebp-8h]
  char *v8; // [esp+10h] [ebp-4h]

  v2 = (char *)operator new[](0x100000u);
  v7 = v2;
  v3 = sub_42DAE0((LPARAM)v2);
  if ( v3 )
  {
    v8 = (char *)operator new[](0x100000u);
    v4 = v8;
    do
    {
      sprintf(v4, "%s\n", v2);
      v4 += strlen(v4);
      --v3;
      v2 += strlen(v2) + 1;
    }
    while ( v3 );
    v5 = sub_45DD50(a1, a2, byte_4E6F80, v8);
    operator delete[](v8);
    operator delete[](v7);
    return v5;
  }
  else
  {
    operator delete[](v2);
    return 0;
  }
}

// ===== sub_468F50 @ 0x00468F50..0x00468F55 =====
// attributes: thunk
LPARAM __usercall sub_468F50@<eax>(LPARAM a1@<esi>)
{
  return sub_42DAE0(a1);
}

// ===== sub_468F60 @ 0x00468F60..0x00468F68 =====
void __usercall sub_468F60(const char *a1@<eax>, const char *a2@<edi>)
{
  sub_42D820(a2, a1);
}

// ===== sub_468F70 @ 0x00468F70..0x00468F75 =====
// attributes: thunk
int __usercall sub_468F70@<eax>(int a1@<edi>)
{
  return sub_42DB60(a1);
}

// ===== sub_468F80 @ 0x00468F80..0x00468F85 =====
// attributes: thunk
int __usercall sub_468F80@<eax>(int result@<eax>)
{
  return sub_42DC30(result);
}

// ===== sub_468F90 @ 0x00468F90..0x00468F95 =====
// attributes: thunk
int __fastcall sub_468F90(unsigned int a1)
{
  return sub_42DC40(a1);
}

// ===== sub_468FA0 @ 0x00468FA0..0x00468FDE =====
int __usercall sub_468FA0@<eax>(_DWORD *a1@<esi>)
{
  unsigned __int16 v1; // ax

  v1 = (unsigned int)(22695477 * *a1) >> 16;
  *a1 = (v1 << 16) + (unsigned __int16)(20021 * *a1) + 1;
  return v1 & 0x7FFF;
}

// ===== sub_468FE0 @ 0x00468FE0..0x00469041 =====
int __fastcall sub_468FE0(int a1, _WORD *a2)
{
  int result; // eax
  int v3; // ecx

  result = -2147483647;
  if ( *a2 && a2[1] )
  {
    v3 = (unsigned __int16)a2[2];
    switch ( a2[2] )
    {
      case 8:
      case 0x10:
      case 0x18:
      case 0x20:
      case 0x30:
        if ( a2[4] < 7u )
        {
          if ( v3 == 8 || v3 == 24 || v3 == 32 )
            result = 0;
          else
            result = -2147483646;
        }
        break;
      default:
        return result;
    }
  }
  return result;
}

// ===== sub_469080 @ 0x00469080..0x00469197 =====
unsigned int __cdecl sub_469080(int a1, unsigned __int16 *a2)
{
  unsigned __int16 *v2; // ecx
  unsigned int result; // eax
  unsigned int v4; // edi
  int v5; // edx
  int v6; // ebx
  unsigned int v7; // eax
  int v8; // edx
  int v9; // esi
  int v10; // ecx
  int v11; // [esp+Ch] [ebp-20h]
  int v12; // [esp+10h] [ebp-1Ch]
  int v13; // [esp+14h] [ebp-18h]
  int v14; // [esp+18h] [ebp-14h]
  unsigned int v15; // [esp+1Ch] [ebp-10h]
  unsigned int v16; // [esp+20h] [ebp-Ch]
  int v17; // [esp+24h] [ebp-8h]
  int v18; // [esp+28h] [ebp-4h]

  v2 = a2;
  result = *a2;
  v4 = a2[2] >> 3;
  v5 = v4 * result;
  v11 = v4 * result;
  v15 = 0;
  if ( a2[1] )
  {
    v17 = 0;
    v14 = -v5;
    do
    {
      v16 = 0;
      if ( result )
      {
        v18 = 0;
        v6 = -v4;
        v13 = -v4;
        do
        {
          v7 = 0;
          if ( v4 )
          {
            v12 = (int)v2 + v18 + v17 + 16;
            do
            {
              LOBYTE(v8) = 0;
              if ( v15 )
                v9 = *((unsigned __int8 *)a2 + v7 + v18 + v14 + 16);
              else
                v9 = -1;
              if ( v16 )
              {
                v10 = *((unsigned __int8 *)a2 + v7 + v6 + v17 + 16);
                v6 = v13;
              }
              else
              {
                v10 = -1;
              }
              if ( v9 < 0 )
              {
                if ( v10 >= 0 )
                  LOBYTE(v8) = v10;
              }
              else if ( v10 < 0 )
              {
                LOBYTE(v8) = v9;
              }
              else
              {
                v8 = (v10 + v9) >> 1;
              }
              *(_BYTE *)(v7 + a1) = *(_BYTE *)(v12 + v7) - v8;
              ++v7;
            }
            while ( v7 < v4 );
            v5 = v11;
            v2 = a2;
          }
          result = *v2;
          a1 += v4;
          v18 += v4;
          v6 += v4;
          ++v16;
          v13 = v6;
        }
        while ( v16 < result );
      }
      v17 += v5;
      v14 += v5;
      ++v15;
    }
    while ( v15 < v2[1] );
  }
  return result;
}

// ===== sub_4691A0 @ 0x004691A0..0x004692C2 =====
_DWORD *__fastcall sub_4691A0(unsigned __int16 *a1, _DWORD *a2, _DWORD *a3)
{
  _DWORD *result; // eax
  int v4; // ecx
  int v5; // esi
  _DWORD *v6; // edi
  __m128i v7; // xmm0
  int v8; // eax
  int v9; // ecx
  __m128i v10; // xmm0
  _DWORD *v11; // ecx
  __m128i v12; // xmm0
  char *v13; // eax
  int v14; // edi
  int v15; // esi
  __m128i v16; // xmm2
  unsigned int v17; // esi
  int v18; // [esp+8h] [ebp-18h]
  int v19; // [esp+Ch] [ebp-14h]
  int v20; // [esp+10h] [ebp-10h]
  int v21; // [esp+14h] [ebp-Ch]
  int v22; // [esp+18h] [ebp-8h]
  int v23; // [esp+18h] [ebp-8h]
  int *v24; // [esp+1Ch] [ebp-4h]
  unsigned int *v25; // [esp+1Ch] [ebp-4h]
  int v26; // [esp+1Ch] [ebp-4h]
  _DWORD *v27; // [esp+28h] [ebp+8h]

  result = a3;
  v22 = a1[1] - 1;
  v4 = *a1;
  v5 = v4 - 1;
  v21 = 4 * v4;
  v20 = 3 * v4;
  v19 = v4 - 1;
  v6 = a2;
  v24 = a3;
  v7 = 0LL;
  if ( v4 )
  {
    do
    {
      v8 = *v24;
      v24 = (int *)((char *)v24 + 3);
      v7 = _mm_add_epi8(v7, _mm_cvtsi32_si128(v8 & 0xFFFFFF));
      *v6++ = _mm_cvtsi128_si32(v7);
      --v4;
    }
    while ( v4 );
    result = a3;
  }
  v9 = v22;
  if ( v22 )
  {
    do
    {
      result = (_DWORD *)((char *)result + v20);
      v25 = a2;
      a2 = (_DWORD *)((char *)a2 + v21);
      v10 = _mm_add_epi8(_mm_cvtsi32_si128(*result & 0xFFFFFF), _mm_cvtsi32_si128(*v25));
      v18 = v9 - 1;
      v27 = result;
      v11 = a2;
      *a2 = _mm_cvtsi128_si32(v10);
      v12 = _mm_unpacklo_epi8(v10, (__m128i)0LL);
      if ( v5 )
      {
        v13 = (char *)result + 1;
        v23 = (char *)v25 - (char *)a2;
        do
        {
          v14 = (unsigned __int8)v13[3];
          v26 = v5 - 1;
          v15 = (unsigned __int8)v13[4];
          v13 += 3;
          v16 = _mm_cvtsi32_si128((unsigned __int8)*(v13 - 1) | ((v14 | (unsigned int)(v15 << 8)) << 8));
          v17 = *(_DWORD *)((char *)v11++ + v23 + 4);
          v12 = _mm_add_epi8(
                  _mm_srli_epi16(_mm_add_epi16(v12, _mm_unpacklo_epi8(_mm_cvtsi32_si128(v17), (__m128i)0LL)), 1u),
                  _mm_unpacklo_epi8(v16, (__m128i)0LL));
          *v11 = _mm_cvtsi128_si32(_mm_packus_epi16(v12, (__m128i)0LL));
          v5 = v26;
        }
        while ( v26 );
        result = v27;
        v5 = v19;
      }
      v9 = v18;
    }
    while ( v18 );
  }
  return result;
}

// ===== sub_4692D0 @ 0x004692D0..0x00469453 =====
int __fastcall sub_4692D0(unsigned __int16 *a1, _DWORD *a2, int a3)
{
  int v3; // eax
  int v4; // ecx
  unsigned int v6; // eax
  int v7; // ecx
  unsigned int v8; // eax
  _DWORD *v9; // esi
  __m128i v10; // xmm0
  int i; // ecx
  int result; // eax
  int v13; // esi
  unsigned int *v14; // eax
  __m128i v15; // xmm2
  unsigned int *v16; // edi
  unsigned int *v17; // eax
  char *v18; // ecx
  unsigned int v19; // esi
  int v20; // edi
  __m128i v21; // xmm2
  __m128i v22; // xmm2
  int v23; // [esp+8h] [ebp-14h]
  unsigned int v24; // [esp+Ch] [ebp-10h]
  unsigned int v25; // [esp+10h] [ebp-Ch]
  int j; // [esp+10h] [ebp-Ch]
  unsigned int *v27; // [esp+14h] [ebp-8h]
  int k; // [esp+14h] [ebp-8h]
  int v29; // [esp+18h] [ebp-4h]
  int v30; // [esp+24h] [ebp+8h]
  int v31; // [esp+24h] [ebp+8h]

  v3 = a1[1];
  v4 = *a1;
  v30 = v3 - 1;
  v6 = v4 - 1;
  v7 = 4 * v4;
  v25 = v6;
  v8 = v6 + 1;
  v29 = v7;
  v9 = a2;
  v10 = 0LL;
  if ( v8 )
  {
    for ( i = a3 - (_DWORD)a2; ; i = a3 - (_DWORD)a2 )
    {
      v10 = _mm_add_epi8(v10, _mm_cvtsi32_si128(*(_DWORD *)((char *)v9 + i)));
      *v9++ = _mm_cvtsi128_si32(v10);
      if ( !--v8 )
        break;
    }
    v7 = v29;
  }
  v24 = v25 >> 1;
  result = v30;
  if ( v30 )
  {
    v13 = v25 & 1;
    for ( j = v13; ; v13 = j )
    {
      v23 = result - 1;
      v14 = a2;
      a2 = (_DWORD *)((char *)a2 + v7);
      v31 = v7 + a3;
      v15 = _mm_add_epi8(_mm_cvtsi32_si128(*(_DWORD *)(v7 + a3)), _mm_cvtsi32_si128(*v14));
      v16 = (unsigned int *)(v7 + a3 + 4);
      v17 = v14 + 1;
      *a2 = _mm_cvtsi128_si32(v15);
      v18 = (char *)(a2 + 1);
      v27 = v16;
      if ( v13 )
      {
        v15 = _mm_packus_epi16(
                _mm_add_epi8(
                  _mm_srli_epi16(
                    _mm_add_epi16(
                      _mm_unpacklo_epi8(v15, (__m128i)0LL),
                      _mm_unpacklo_epi8(_mm_cvtsi32_si128(*v17), (__m128i)0LL)),
                    1u),
                  _mm_unpacklo_epi8(_mm_cvtsi32_si128(*v16), (__m128i)0LL)),
                (__m128i)0LL);
        *(_DWORD *)v18 = _mm_cvtsi128_si32(v15);
        v18 = (char *)(a2 + 2);
        v27 = v16 + 1;
        ++v17;
      }
      v19 = v24;
      if ( v24 )
      {
        v20 = (char *)v27 - v18;
        for ( k = (char *)v27 - v18; ; v20 = k )
        {
          v21 = _mm_add_epi8(
                  _mm_loadl_epi64((const __m128i *)&v18[v20]),
                  _mm_packus_epi16(
                    _mm_srli_epi16(
                      _mm_add_epi16(
                        _mm_unpacklo_epi8(v15, (__m128i)0LL),
                        _mm_unpacklo_epi8(_mm_cvtsi32_si128(*v17), (__m128i)0LL)),
                      1u),
                    (__m128i)0LL));
          v22 = _mm_add_epi8(
                  v21,
                  _mm_slli_epi64(
                    _mm_packus_epi16(
                      _mm_srli_epi16(
                        _mm_add_epi16(
                          _mm_unpacklo_epi8(v21, (__m128i)0LL),
                          _mm_unpacklo_epi8(_mm_cvtsi32_si128(v17[1]), (__m128i)0LL)),
                        1u),
                      (__m128i)0LL),
                    0x20u));
          *(_QWORD *)v18 = v22.m128i_i64[0];
          v18 += 8;
          v17 += 2;
          --v19;
          v15 = _mm_srli_epi64(v22, 0x20u);
          if ( !v19 )
            break;
        }
      }
      result = v23;
      if ( !v23 )
        break;
      a3 = v31;
      v7 = v29;
    }
  }
  return result;
}

// ===== sub_469460 @ 0x00469460..0x00469641 =====
unsigned int __cdecl sub_469460(_DWORD *a1, unsigned __int16 *a2, _DWORD *a3)
{
  unsigned __int16 *v3; // ecx
  _DWORD *v4; // ebx
  unsigned int v5; // esi
  int v6; // eax
  unsigned int result; // eax
  unsigned int v8; // eax
  unsigned int v9; // edx
  unsigned int v10; // edi
  int v11; // esi
  unsigned int v12; // eax
  int v13; // edx
  int v14; // esi
  int v15; // ecx
  int v16; // [esp+Ch] [ebp-34h]
  unsigned int v17; // [esp+10h] [ebp-30h]
  unsigned int v18; // [esp+14h] [ebp-2Ch]
  unsigned int v19; // [esp+18h] [ebp-28h]
  int v20; // [esp+1Ch] [ebp-24h]
  int v21; // [esp+20h] [ebp-20h]
  int v22; // [esp+24h] [ebp-1Ch]
  int v23; // [esp+28h] [ebp-18h]
  int v24; // [esp+2Ch] [ebp-14h]
  int v25; // [esp+30h] [ebp-10h]
  char *v26; // [esp+38h] [ebp-8h]
  int v27; // [esp+3Ch] [ebp-4h]

  v3 = a2;
  v4 = a1;
  v5 = *a2;
  if ( v5 >= 8 && a2[1] >= 2u )
  {
    v6 = a2[2];
    if ( v6 == 24 )
      return (unsigned int)sub_4691A0(a2, a1, a3);
    if ( v6 == 32 )
      return sub_4692D0(a2, a1, (int)a3);
  }
  v8 = a2[2];
  if ( v8 == 24 )
    v9 = 4;
  else
    v9 = v8 >> 3;
  result = *a2;
  v25 = v9 * v5;
  v10 = a2[2] >> 3;
  v27 = v10 * v5;
  v17 = v9;
  v18 = 0;
  if ( a2[1] )
  {
    v16 = 0;
    v23 = 0;
    v24 = -v25;
    do
    {
      v19 = 0;
      if ( result )
      {
        v11 = 0;
        v22 = 0;
        v20 = 0;
        v21 = -v9;
        do
        {
          v12 = 0;
          if ( v10 )
          {
            v26 = (char *)v4 + v11 + v16;
            do
            {
              LOBYTE(v13) = 0;
              if ( v18 )
                v14 = *((unsigned __int8 *)v4 + v12 + v11 + v24);
              else
                v14 = -1;
              if ( v19 )
              {
                v4 = a1;
                v15 = *((unsigned __int8 *)a1 + v12 + v21 + v16);
              }
              else
              {
                v15 = -1;
              }
              if ( v14 < 0 )
              {
                if ( v15 >= 0 )
                  LOBYTE(v13) = v15;
              }
              else if ( v15 < 0 )
              {
                LOBYTE(v13) = v14;
              }
              else
              {
                v13 = (v15 + v14) >> 1;
              }
              v11 = v22;
              v26[v12] = v13 + *((_BYTE *)a3 + v20 + v23 + v12);
              ++v12;
            }
            while ( v12 < v10 );
            v9 = v17;
          }
          if ( v10 < v9 )
          {
            memset((char *)v4 + v10 + v11 + v16, 0, v9 - v10);
            v9 = v17;
          }
          result = *a2;
          v20 += v10;
          v21 += v9;
          v11 += v9;
          ++v19;
          v22 = v11;
        }
        while ( v19 < result );
        v3 = a2;
      }
      v23 += v27;
      v24 += v25;
      v16 += v25;
      ++v18;
    }
    while ( v18 < v3[1] );
  }
  return result;
}

// ===== sub_469650 @ 0x00469650..0x00469704 =====
int __usercall sub_469650@<eax>(_BYTE *a1@<eax>, int a2, unsigned int a3)
{
  unsigned int v3; // edx
  unsigned int v4; // ebx
  unsigned int v6; // edi
  int result; // eax
  bool v8; // cf
  unsigned int v9; // edx
  char v10; // cl
  unsigned int v11; // ecx
  unsigned int v12; // [esp+Ch] [ebp-Ch]
  int v13; // [esp+14h] [ebp-4h]

  v3 = a3;
  v4 = 0;
  v6 = 0;
  result = 0;
  v13 = 1;
  v12 = 0;
  if ( a3 )
  {
    do
    {
      if ( v13 )
      {
        if ( !*(_BYTE *)(v4 + a2) && !*(_BYTE *)(v4 + a2 + 1) )
          goto LABEL_8;
      }
      else if ( *(_BYTE *)(v4 + a2) )
      {
        v12 = v4;
LABEL_8:
        v9 = v6;
        do
        {
          v10 = v9;
          if ( v9 >= 0x80 )
            v10 = v9 & 0x7F | 0x80;
          *a1 = v10;
          v9 >>= 7;
          ++a1;
          ++result;
        }
        while ( v9 );
        if ( v13 )
        {
          v11 = 0;
          if ( v6 )
          {
            do
              *a1++ = *(_BYTE *)(a2 + v12 + v11++);
            while ( v11 < v6 );
            result += v6;
          }
          v13 = 0;
        }
        else
        {
          v13 = 1;
        }
        v3 = a3;
        v6 = 0;
        v8 = v4 < a3;
        continue;
      }
      ++v4;
      ++v6;
      v8 = v4 < v3;
      if ( v4 == v3 )
        goto LABEL_8;
    }
    while ( v8 );
  }
  return result;
}

// ===== sub_469710 @ 0x00469710..0x0046979F =====
int __usercall sub_469710@<eax>(char *a1@<eax>, int a2, unsigned int a3)
{
  int result; // eax
  unsigned int v4; // edi
  size_t v5; // esi
  int v6; // ecx
  char v7; // al
  int v8; // edx
  char *v9; // ebx
  int v10; // [esp+4h] [ebp-Ch]
  int v12; // [esp+Ch] [ebp-4h]

  result = 0;
  v4 = 0;
  v10 = 1;
  v12 = 0;
  if ( a3 )
  {
    do
    {
      v5 = 0;
      v6 = 0;
      do
      {
        v7 = *(_BYTE *)(v4 + a2);
        v8 = (v7 & 0x7F) << v6;
        v6 += 7;
        ++v4;
        v5 |= v8;
      }
      while ( v7 < 0 );
      if ( v10 )
      {
        memcpy_0(a1, (const void *)(v4 + a2), v5);
        v12 += v5;
        v9 = &a1[v5];
        v4 += v5;
        v10 = 0;
      }
      else
      {
        memset(a1, 0, v5);
        v9 = &a1[v5];
        v12 += v5;
        v10 = 1;
      }
      a1 = v9;
    }
    while ( v4 < a3 );
    return v12;
  }
  return result;
}

// ===== sub_4697A0 @ 0x004697A0..0x004697D2 =====
unsigned int __usercall sub_4697A0@<eax>(void *a1@<edi>, unsigned int a2@<esi>, int a3)
{
  unsigned int result; // eax

  memset(a1, 0, 0x400u);
  for ( result = 0; result < a2; ++result )
    ++*((_DWORD *)a1 + *(unsigned __int8 *)(result + a3));
  return result;
}

// ===== sub_4697E0 @ 0x004697E0..0x00469989 =====
int __cdecl sub_4697E0(unsigned int *a1, int a2)
{
  int v2; // eax
  unsigned int v3; // edx
  _DWORD *v4; // ecx
  int v5; // esi
  int v6; // esi
  unsigned int *v8; // esi
  _DWORD *i; // eax
  unsigned int v10; // edx
  unsigned int *v11; // ebx
  unsigned int v12; // edi
  unsigned int v13; // ecx
  unsigned int *v14; // esi
  unsigned int v15; // edx
  unsigned int *v16; // ecx
  int v17; // esi
  unsigned int v18; // edi
  int v19; // edx
  unsigned int v20; // ecx
  int v21; // [esp+24h] [ebp-14h] BYREF
  int v22; // [esp+28h] [ebp-10h]
  int v23; // [esp+2Ch] [ebp-Ch]
  unsigned int v24; // [esp+30h] [ebp-8h]
  int v25; // [esp+34h] [ebp-4h]

  v2 = 0;
  v3 = 0;
  v25 = 0;
  v4 = a1 + 2;
  do
  {
    v5 = *(_DWORD *)(a2 + 4 * v3);
    v25 += v5;
    *(v4 - 2) = v5 != 0;
    *(v4 - 1) = v5;
    v6 = *(_DWORD *)(a2 + 4 * v3 + 4);
    v4[4] = v6 != 0;
    v4[2] = v3;
    v4[3] = v3;
    *v4 = 0;
    v4[1] = -1;
    v4[5] = v6;
    v4[6] = 0;
    v4[7] = -1;
    v4[8] = v3 + 1;
    v4[9] = v3 + 1;
    v3 += 2;
    v2 += v6;
    v4 += 12;
  }
  while ( v3 < 0x100 );
  v25 += v2;
  if ( !v25 )
    return -1;
  if ( v3 < 0x1FF )
  {
    v8 = &a1[6 * v3];
    *v8 = 0;
    v8[1] = 0;
    v8[2] = 1;
    v8[3] = -1;
    v8[4] = -1;
    v8[5] = -1;
    qmemcpy(v8 + 6, v8, 4 * ((24 * (510 - v3)) >> 2));
  }
  v24 = 256;
  for ( i = a1 + 1536; ; i += 6 )
  {
    v10 = v24;
    v11 = (unsigned int *)&v21;
    v23 = 2;
    do
    {
      v12 = -1;
      v13 = 0;
      *v11 = -1;
      if ( v10 )
      {
        v14 = a1 + 1;
        do
        {
          if ( *(v14 - 1) )
          {
            v15 = *v14;
            if ( *v14 < v12 )
            {
              *v11 = v13;
              v12 = v15;
            }
          }
          v10 = v24;
          ++v13;
          v14 += 6;
        }
        while ( v13 < v24 );
        if ( *v11 != -1 )
        {
          v16 = &a1[6 * *v11];
          *v16 = 0;
          v16[3] = v10;
        }
      }
      ++v11;
      --v23;
    }
    while ( v23 );
    v17 = v22;
    v18 = v22 == -1 ? 0 : a1[6 * v22 + 1];
    v19 = v21;
    v20 = v18 + a1[6 * v21 + 1];
    *i = 1;
    i[1] = v20;
    i[2] = 1;
    i[3] = -1;
    i[4] = v19;
    i[5] = v17;
    if ( v20 == v25 )
      break;
    ++v24;
  }
  return v24;
}

// ===== sub_469990 @ 0x00469990..0x00469A4B =====
_DWORD *__usercall sub_469990@<eax>(int a1@<edi>, _DWORD *a2)
{
  _DWORD *result; // eax
  unsigned int v3; // ebx
  unsigned int v4; // ecx
  int v5; // eax
  unsigned int v6; // edx
  int v7; // ecx
  _BYTE *i; // ecx
  char v9; // dl
  _DWORD *v10; // [esp+8h] [ebp-10Ch]
  _DWORD v11[65]; // [esp+Ch] [ebp-108h]

  result = a2;
  v3 = 0;
  v11[0] = a2;
  v10 = (_DWORD *)(a1 + 12);
  do
  {
    if ( *v10 == -1 )
    {
      *result = 0;
    }
    else
    {
      v4 = v3;
      v5 = 0;
      do
      {
        v6 = *(_DWORD *)(a1 + 24 * v4 + 12);
        *((_BYTE *)&v11[1] + v5++) = *(_DWORD *)(a1 + 24 * v6 + 20) == v4;
        v4 = v6;
      }
      while ( *(_DWORD *)(a1 + 24 * v6 + 12) != -1 );
      v7 = v11[0];
      *(_DWORD *)v11[0] = v5;
      for ( i = (_BYTE *)(v7 + 4); v5; ++i )
      {
        v9 = *((_BYTE *)v11 + v5-- + 3);
        *i = v9;
      }
    }
    v10 += 6;
    ++v3;
    result = (_DWORD *)(v11[0] + 260);
    v11[0] += 260;
  }
  while ( v3 < 0x100 );
  return result;
}

// ===== sub_469A50 @ 0x00469A50..0x00469AA5 =====
int __usercall sub_469A50@<eax>(_BYTE *a1@<eax>, int a2)
{
  int v2; // edi
  unsigned int i; // eax
  unsigned int v5; // ecx
  int j; // edx

  v2 = 0;
  for ( i = 0; i < 0x100; ++i )
  {
    v5 = *(_DWORD *)(a2 + 4 * i);
    for ( j = 1; j || v5; j = 0 )
    {
      *a1++ = v5 & 0x7F | ((v5 & 0xFFFFFF80) != 0 ? 0x80 : 0);
      v5 >>= 7;
      ++v2;
    }
  }
  return v2;
}

// ===== sub_469AB0 @ 0x00469AB0..0x00469AE8 =====
int __usercall sub_469AB0@<eax>(char *a1@<edx>, int a2)
{
  unsigned int i; // edi
  int v3; // esi
  int v4; // ecx
  char v5; // al
  int v6; // ebx
  int result; // eax

  for ( i = 0; i < 0x100; ++i )
  {
    v3 = 0;
    v4 = 0;
    do
    {
      v5 = *a1;
      v6 = (*a1 & 0x7F) << v4;
      v4 += 7;
      ++a1;
      v3 |= v6;
    }
    while ( v5 < 0 );
    result = a2;
    *(_DWORD *)(a2 + 4 * i) = v3;
  }
  return result;
}

// ===== sub_469AF0 @ 0x00469AF0..0x00469B50 =====
_BYTE *__usercall sub_469AF0@<eax>(int a1@<esi>, _BYTE *a2, _BYTE *a3, int a4)
{
  char v4; // al
  unsigned int v5; // edi
  char v6; // bl
  unsigned int v7; // ecx
  char v8; // al
  char v9; // dl
  char v10; // bl
  _BYTE *result; // eax
  char v12; // [esp+Ah] [ebp-2h]
  char v13; // [esp+Bh] [ebp-1h]

  v4 = 0;
  v5 = a4;
  v6 = 0;
  v7 = 0;
  v12 = 0;
  v13 = 0;
  if ( a4 >= 2 )
  {
    do
    {
      v8 = *(_BYTE *)(v7 + a1 + 1);
      v9 = *(_BYTE *)(v7 + a1);
      v12 += v9;
      v6 += v8;
      v13 ^= v9 ^ v8;
      v7 += 2;
    }
    while ( v7 < a4 - 1 );
    v4 = 0;
    v5 = a4;
  }
  if ( v7 < v5 )
  {
    v4 = *(_BYTE *)(v7 + a1);
    v13 ^= v4;
  }
  v10 = v4 + v12 + v6;
  result = a2;
  *a2 = v10;
  *a3 = v13;
  return result;
}

// ===== sub_469B50 @ 0x00469B50..0x00469B76 =====
void sub_469B50(int a1, unsigned int a2, ...)
{
  unsigned int i; // edi
  va_list va; // [esp+18h] [ebp+10h] BYREF

  va_start(va, a2);
  for ( i = 0; i < a2; ++i )
    *(_BYTE *)(i + a1) += sub_468FA0((int *)va);
}

// ===== sub_469B80 @ 0x00469B80..0x00469BA6 =====
void sub_469B80(int a1, unsigned int a2, ...)
{
  unsigned int i; // edi
  va_list va; // [esp+18h] [ebp+10h] BYREF

  va_start(va, a2);
  for ( i = 0; i < a2; ++i )
    *(_BYTE *)(i + a1) -= sub_468FA0((int *)va);
}

// ===== sub_469BB0 @ 0x00469BB0..0x00469C47 =====
int __usercall sub_469BB0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, int a3, int a4)
{
  int v4; // eax
  unsigned int v6; // [esp-18h] [ebp-1Ch]
  int v7; // [esp-14h] [ebp-18h]

  *(_DWORD *)a2 = 0;
  *(_DWORD *)(a2 + 4) = 0;
  *(_DWORD *)(a2 + 8) = 0;
  *(_DWORD *)(a2 + 12) = 0;
  strcpy((char *)a2, "CompressedBG___");
  *(_DWORD *)(a2 + 16) = *a1;
  *(_DWORD *)(a2 + 20) = a1[1];
  *(_DWORD *)(a2 + 24) = a1[2];
  *(_DWORD *)(a2 + 28) = a1[3];
  *(_DWORD *)(a2 + 32) = a4;
  *(_DWORD *)(a2 + 36) = GetTickCount();
  v4 = sub_469A50((_BYTE *)(a2 + 48), a3);
  *(_DWORD *)(a2 + 40) = v4;
  sub_469AF0(a2 + 48, (_BYTE *)(a2 + 44), (_BYTE *)(a2 + 45), v4);
  v7 = *(_DWORD *)(a2 + 36);
  v6 = *(_DWORD *)(a2 + 40);
  *(_WORD *)(a2 + 46) = 1;
  sub_469B50(a2 + 48, v6, v7);
  return *(_DWORD *)(a2 + 40) + 48;
}

// ===== sub_469C50 @ 0x00469C50..0x00469D4F =====
int __cdecl sub_469C50(_BYTE *a1, int a2, unsigned int a3, int a4)
{
  _BYTE *v4; // eax
  char v5; // bl
  int v6; // edx
  unsigned int v7; // edi
  unsigned int i; // esi
  _BYTE *v10; // [esp+4h] [ebp-114h]
  unsigned int v11; // [esp+8h] [ebp-110h]
  int v12; // [esp+Ch] [ebp-10Ch]
  _DWORD v13[65]; // [esp+10h] [ebp-108h] BYREF

  v4 = a1;
  v5 = 0;
  v6 = 0;
  v12 = 0;
  v10 = a1;
  v11 = 0;
  if ( !a3 )
    return 0;
  do
  {
    qmemcpy(v13, (const void *)(a4 + 260 * *(unsigned __int8 *)(v11 + a2)), sizeof(v13));
    v7 = v13[0];
    for ( i = 0; i < v7; ++i )
    {
      if ( *((_BYTE *)&v13[1] + i) )
      {
        v5 |= 128 >> v6;
        v4 = v10;
      }
      if ( ++v6 == 8 )
      {
        *v4++ = v5;
        v5 = 0;
        v6 = 0;
        ++v12;
        v10 = v4;
      }
    }
    ++v11;
  }
  while ( v11 < a3 );
  if ( !v6 )
    return v12;
  *v4 = v5;
  return v12 + 1;
}

// ===== sub_469D50 @ 0x00469D50..0x00469DB7 =====
int __usercall sub_469D50@<eax>(_BYTE *a1@<ecx>, int a2@<edi>, int a3, int a4, unsigned int a5)
{
  int result; // eax
  int v6; // edx
  unsigned __int8 v8; // cl
  BOOL v9; // ebx
  int v10; // [esp+4h] [ebp-4h]

  result = a4;
  v6 = 0;
  v8 = 0x80;
  if ( a5 )
  {
    v10 = *(_DWORD *)(a2 + 24 * a4 + 8);
    while ( 1 )
    {
      if ( v10 == 1 )
      {
        do
        {
          v9 = (v8 & *a1) != 0;
          v8 >>= 1;
          result = *(_DWORD *)(a2 + 4 * (v9 + 6 * result) + 16);
          if ( !v8 )
          {
            ++a1;
            v8 = 0x80;
          }
        }
        while ( *(_DWORD *)(a2 + 24 * result + 8) == 1 );
      }
      *(_BYTE *)(v6 + a3) = result;
      if ( ++v6 >= a5 )
        break;
      result = a4;
    }
  }
  return result;
}

// ===== sub_469DC0 @ 0x00469DC0..0x00469ED2 =====
int __cdecl sub_469DC0(int a1, unsigned __int16 *a2)
{
  int v2; // ecx
  int result; // eax
  unsigned int v4; // esi
  int v5; // esi
  int v6; // eax
  _DWORD *v7; // [esp+4h] [ebp-137FCh]
  void *v8; // [esp+Ch] [ebp-137F4h]
  _BYTE *v9; // [esp+10h] [ebp-137F0h]
  unsigned int v10[3066]; // [esp+14h] [ebp-137ECh] BYREF
  _DWORD v11[16640]; // [esp+2FFCh] [ebp-10804h] BYREF
  _BYTE v12[1024]; // [esp+133FCh] [ebp-404h] BYREF

  v7 = (_DWORD *)v2;
  result = sub_468FE0(v2, a2);
  if ( !result )
  {
    v4 = *a2 * a2[1] * (a2[2] >> 3);
    v8 = operator new[](v4);
    sub_469080((int)v8, a2);
    v9 = operator new[](2 * v4);
    v5 = sub_469650(v9, (int)v8, v4);
    sub_4697A0(v12, v5, (int)v9);
    sub_4697E0(v10, (int)v12);
    sub_469990((int)v10, v11);
    v6 = sub_469BB0(a2, a1, (int)v12, v5);
    *v7 = v6 + sub_469C50((_BYTE *)(v6 + a1), (int)v9, v5, (int)v11);
    operator delete[](v9);
    operator delete[](v8);
    return 0;
  }
  return result;
}

// ===== sub_469EE0 @ 0x00469EE0..0x0046A0AC =====
int __cdecl sub_469EE0(int a1)
{
  _DWORD *v1; // ecx
  _DWORD *v2; // edi
  void *v3; // esi
  int v4; // edi
  void *v5; // esi
  unsigned __int16 *v6; // edi
  void *v8; // [esp+10h] [ebp-33FCh]
  unsigned __int16 *v9; // [esp+14h] [ebp-33F8h]
  char *v10; // [esp+14h] [ebp-33F8h]
  unsigned int v11[3066]; // [esp+18h] [ebp-33F4h] BYREF
  _BYTE v12[1028]; // [esp+3000h] [ebp-40Ch] BYREF

  v2 = v1;
  v9 = (unsigned __int16 *)v1;
  if ( strcmp((const char *)a1, "CompressedBG___") )
    return -2147483645;
  v3 = operator new[](*(_DWORD *)(a1 + 40));
  v8 = v3;
  memcpy_0(v3, (const void *)(a1 + 48), *(_DWORD *)(a1 + 40));
  sub_469B80((int)v3, *(_DWORD *)(a1 + 40), *(_DWORD *)(a1 + 36));
  if ( sub_4A13D0(*(_DWORD *)(a1 + 40), *(_BYTE *)(a1 + 44), *(_BYTE *)(a1 + 45)) )
  {
    *v2 = *(_DWORD *)(a1 + 16);
    v2[1] = *(_DWORD *)(a1 + 20);
    v2[2] = *(_DWORD *)(a1 + 24);
    v2[3] = *(_DWORD *)(a1 + 28);
    sub_469AB0((char *)v3, (int)v12);
    v4 = sub_4697E0(v11, (int)v12);
    v5 = operator new[](*(_DWORD *)(a1 + 32));
    sub_469D50((_BYTE *)(*(_DWORD *)(a1 + 40) + a1 + 48), (int)v11, (int)v5, v4, *(_DWORD *)(a1 + 32));
    v6 = v9;
    v10 = (char *)operator new[](*v9 * v9[1] * (v9[2] >> 3));
    sub_469710(v10, (int)v5, *(_DWORD *)(a1 + 32));
    operator delete[](v5);
    sub_469460((_DWORD *)v6 + 4, v6, v10);
    if ( v6[2] == 24 )
    {
      v6[2] = 32;
      v6[4] = 7;
    }
    operator delete[](v10);
    operator delete[](v8);
    return 0;
  }
  else
  {
    operator delete[](v3);
    return -2147483644;
  }
}

// ===== sub_46A0C0 @ 0x0046A0C0..0x0046A0EA =====
_DWORD *__thiscall sub_46A0C0(_DWORD *this, char a2)
{
  int v4; // [esp-4h] [ebp-8h]

  v4 = this[1];
  *this = &Gdiplus::Image::`vftable';
  GdipDisposeImage(v4);
  if ( (a2 & 1) != 0 )
    GdipFree(this);
  return this;
}

// ===== sub_46A110 @ 0x0046A110..0x0046A197 =====
_DWORD *__thiscall sub_46A110(_DWORD *this)
{
  int v2; // eax
  _DWORD *result; // eax
  int v4; // esi
  int v5; // ecx
  int v6; // [esp-8h] [ebp-24h]
  _DWORD v7[4]; // [esp+Ch] [ebp-10h] BYREF

  v6 = this[1];
  v7[0] = 0;
  v2 = GdipCloneImage(v6, v7);
  if ( v2 )
    this[2] = v2;
  result = (_DWORD *)GdipAlloc(16);
  v7[3] = 0;
  if ( !result )
    return 0;
  v4 = this[2];
  v5 = v7[0];
  *result = &Gdiplus::Image::`vftable';
  result[1] = v5;
  result[2] = v4;
  return result;
}

// ===== sub_46A1A0 @ 0x0046A1A0..0x0046A201 =====
_DWORD *__thiscall sub_46A1A0(_DWORD *this, char a2)
{
  int v4; // [esp-4h] [ebp-1Ch]

  v4 = this[1];
  *this = &Gdiplus::Image::`vftable';
  GdipDisposeImage(v4);
  if ( (a2 & 1) != 0 )
    GdipFree(this);
  return this;
}

// ===== sub_46A210 @ 0x0046A210..0x0046A23C =====
int sub_46A210()
{
  _DWORD v1[4]; // [esp+0h] [ebp-10h] BYREF

  memset(&v1[1], 0, 12);
  v1[0] = 1;
  return GdiplusStartup(&dword_56666C, v1, 0);
}

// ===== sub_46A240 @ 0x0046A240..0x0046A257 =====
int sub_46A240()
{
  int result; // eax

  result = GdiplusShutdown(dword_56666C);
  dword_56666C = 0;
  return result;
}

// ===== sub_46A260 @ 0x0046A260..0x0046A2F8 =====
int __usercall sub_46A260@<eax>(int result@<eax>, int a2@<esi>, _DWORD *a3)
{
  int ImagePixelFormat; // eax
  int v4; // eax
  int v5; // [esp+8h] [ebp-4h] BYREF

  switch ( result )
  {
    case -1:
      ImagePixelFormat = GdipGetImagePixelFormat(*(_DWORD *)(a2 + 4), &v5);
      if ( ImagePixelFormat )
        *(_DWORD *)(a2 + 8) = ImagePixelFormat;
      v4 = v5;
      *a3 = v5;
      switch ( v4 )
      {
        case 137224:
          return 1;
        case 198659:
          return 3;
        case 2498570:
          return 2;
        default:
          return -1;
      }
    case 1:
      *a3 = 137224;
      break;
    case 2:
      *a3 = 2498570;
      break;
    case 3:
      *a3 = 198659;
      break;
  }
  return result;
}

// ===== sub_46A300 @ 0x0046A300..0x0046A50B =====
int __cdecl sub_46A300(void *a1, int a2)
{
  LPCSTR lpFileName; // ecx
  const CHAR *v3; // edi
  _DWORD *v4; // eax
  int v5; // esi
  int v6; // eax
  int v7; // ebx
  int ImageHeight; // eax
  int v9; // edi
  int ImageWidth; // eax
  int v11; // ecx
  int v12; // eax
  int v13; // eax
  int v14; // edi
  int v16; // [esp-8h] [ebp-684h]
  int v17; // [esp-8h] [ebp-684h]
  int v18; // [esp+10h] [ebp-66Ch] BYREF
  int v19; // [esp+14h] [ebp-668h]
  int v20; // [esp+18h] [ebp-664h]
  int v21; // [esp+1Ch] [ebp-660h] BYREF
  int v22; // [esp+20h] [ebp-65Ch] BYREF
  _DWORD v23[4]; // [esp+24h] [ebp-658h] BYREF
  int v24[6]; // [esp+34h] [ebp-648h] BYREF
  WCHAR WideCharStr[782]; // [esp+4Ch] [ebp-630h] BYREF
  int v26; // [esp+678h] [ebp-4h]

  v3 = lpFileName;
  v20 = -1;
  MultiByteToWideChar(0, 0, lpFileName, -1, WideCharStr, 780);
  v4 = (_DWORD *)GdipAlloc(16);
  v5 = (int)v4;
  v19 = (int)v4;
  v26 = 0;
  if ( v4 )
  {
    *v4 = &Gdiplus::Image::`vftable';
    LOBYTE(v26) = 1;
    *v4 = &Gdiplus::Bitmap::`vftable';
    v18 = 0;
    v4[2] = GdipCreateBitmapFromFile(WideCharStr, &v18);
    *(_DWORD *)(v5 + 4) = v18;
    LOBYTE(v26) = 0;
  }
  else
  {
    v5 = 0;
  }
  v26 = -1;
  v6 = sub_46A260(a2, v5, &v21);
  v7 = v6;
  if ( v6 == -1 )
  {
    v20 = (GetFileAttributesA(v3) != -1) - 0x7FFFFFFF;
    goto LABEL_19;
  }
  v19 = v6;
  if ( v6 == 1 )
  {
    v21 = 2498570;
    v19 = 2;
  }
  v16 = *(_DWORD *)(v5 + 4);
  v18 = 0;
  ImageHeight = GdipGetImageHeight(v16, &v18);
  if ( ImageHeight )
    *(_DWORD *)(v5 + 8) = ImageHeight;
  v9 = v18;
  v17 = *(_DWORD *)(v5 + 4);
  v22 = 0;
  ImageWidth = GdipGetImageWidth(v17, &v22);
  if ( ImageWidth )
    *(_DWORD *)(v5 + 8) = ImageWidth;
  v23[0] = 0;
  v23[1] = 0;
  v11 = *(_DWORD *)(v5 + 4);
  v23[2] = v22;
  v23[3] = v9;
  v12 = GdipBitmapLockBits(v11, v23, 1, v21, v24);
  if ( v12 )
  {
    *(_DWORD *)(v5 + 8) = v12;
LABEL_19:
    v14 = v20;
    goto LABEL_20;
  }
  sub_402420(0, v24[4], (int)a1, v24[0], v24[1], v19);
  if ( v7 == 1 )
    sub_402660(a1);
  v13 = GdipBitmapUnlockBits(*(_DWORD *)(v5 + 4), v24);
  if ( v13 )
    *(_DWORD *)(v5 + 8) = v13;
  v14 = 0;
LABEL_20:
  if ( v5 )
    (**(void (__thiscall ***)(int, int))v5)(v5, 1);
  return v14;
}

// ===== sub_46A510 @ 0x0046A510..0x0046A5FA =====
int __cdecl sub_46A510(const unsigned __int16 **a1, const unsigned __int16 *a2)
{
  const unsigned __int16 **v2; // edi
  unsigned int v3; // esi
  const unsigned __int16 **v4; // edi
  const unsigned __int16 **v5; // eax
  int v7; // [esp+4h] [ebp-10h]
  const unsigned __int16 **v8; // [esp+8h] [ebp-Ch]
  unsigned int v9; // [esp+Ch] [ebp-8h] BYREF
  unsigned int v10; // [esp+10h] [ebp-4h] BYREF

  v7 = -1;
  v10 = 0;
  v9 = 0;
  if ( GdipGetImageEncodersSize(&v10, &v9) || !v9 )
    return -1;
  v2 = (const unsigned __int16 **)operator new[](v9);
  v8 = v2;
  GdipGetImageEncoders(v10, v9, v2);
  v3 = 0;
  if ( v10 )
  {
    v4 = v2 + 12;
    while ( wcscmp(*v4, a2) )
    {
      ++v3;
      v4 += 19;
      if ( v3 >= v10 )
        goto LABEL_9;
    }
    v5 = &v8[19 * v3];
    v7 = v3;
    *a1 = *v5;
    a1[1] = v5[1];
    a1[2] = v5[2];
    a1[3] = v5[3];
LABEL_9:
    v2 = v8;
  }
  operator delete(v2);
  return v7;
}

// ===== sub_46A600 @ 0x0046A600..0x0046A863 =====
int __cdecl sub_46A600(const CHAR *a1, int a2, int a3)
{
  int v3; // ecx
  int v4; // edx
  int v5; // ecx
  _DWORD *v6; // eax
  _DWORD *v7; // esi
  int v8; // eax
  int v9; // ecx
  _DWORD *v10; // edi
  int v11; // eax
  int v12; // edi
  int v14; // [esp-8h] [ebp-6CCh]
  const unsigned __int16 *v15; // [esp-4h] [ebp-6C8h]
  int v16; // [esp+10h] [ebp-6B4h] BYREF
  LPCCH lpMultiByteStr; // [esp+14h] [ebp-6B0h]
  _DWORD v18[2]; // [esp+18h] [ebp-6ACh] BYREF
  _DWORD v19[6]; // [esp+20h] [ebp-6A4h] BYREF
  _DWORD v20[8]; // [esp+38h] [ebp-68Ch] BYREF
  const unsigned __int16 *v21[4]; // [esp+58h] [ebp-66Ch] BYREF
  _DWORD v22[3]; // [esp+68h] [ebp-65Ch] BYREF
  __int16 v23; // [esp+74h] [ebp-650h]
  __int16 v24; // [esp+76h] [ebp-64Eh]
  int v25; // [esp+78h] [ebp-64Ch]
  int v26; // [esp+7Ch] [ebp-648h]
  int v27; // [esp+80h] [ebp-644h]
  int v28; // [esp+84h] [ebp-640h]
  int v29; // [esp+88h] [ebp-63Ch]
  int v30; // [esp+8Ch] [ebp-638h]
  WCHAR WideCharStr[782]; // [esp+94h] [ebp-630h] BYREF
  int v32; // [esp+6C0h] [ebp-4h]

  lpMultiByteStr = a1;
  if ( !sub_407F20((int)dword_566750, v3, v19) )
    return -2147483645;
  v22[2] = -v19[3];
  v23 = 1;
  v22[0] = 40;
  v22[1] = v19[2];
  v24 = sub_407B40(v19[4]);
  v25 = 0;
  v26 = v5 * v4 * v19[5];
  v27 = 0;
  v28 = 0;
  v29 = 0;
  v30 = 0;
  v6 = (_DWORD *)GdipAlloc(16);
  v7 = v6;
  v18[1] = v6;
  v32 = 0;
  if ( v6 )
  {
    *v6 = &Gdiplus::Image::`vftable';
    LOBYTE(v32) = 1;
    v14 = v19[0];
    *v6 = &Gdiplus::Bitmap::`vftable';
    v16 = 0;
    v8 = GdipCreateBitmapFromGdiDib(v22, v14, &v16);
    v9 = v16;
    v7[2] = v8;
    v7[1] = v9;
    LOBYTE(v32) = 0;
  }
  else
  {
    v7 = 0;
  }
  v32 = -1;
  v20[1] = 492561589;
  v20[4] = -337181359;
  v18[0] = a3;
  v20[0] = 1;
  v20[2] = 1160641098;
  v20[5] = 1;
  v10 = 0;
  v20[3] = -1285694052;
  v20[6] = 4;
  v20[7] = v18;
  switch ( a2 )
  {
    case 0:
      v15 = L"image/bmp";
      goto LABEL_7;
    case 1:
      sub_46A510(v21, L"image/jpeg");
      v10 = v20;
      goto LABEL_8;
    case 2:
      sub_46A510(v21, L"image/gif");
      goto LABEL_8;
    case 3:
      v15 = L"image/tiff";
LABEL_7:
      sub_46A510(v21, v15);
      goto LABEL_8;
    case 4:
      sub_46A510(v21, L"image/png");
LABEL_8:
      MultiByteToWideChar(0, 0, lpMultiByteStr, -1, WideCharStr, 780);
      v11 = GdipSaveImageToFile(v7[1], WideCharStr, v21, v10);
      if ( v11 )
      {
        v7[2] = v11;
        v12 = -2147483643;
      }
      else
      {
        v12 = 0;
      }
      break;
    default:
      v12 = -2147483644;
      break;
  }
  if ( v7 )
    (*(void (__thiscall **)(_DWORD *, int))*v7)(v7, 1);
  return v12;
}

// ===== sub_46A880 @ 0x0046A880..0x0046A8AE =====
_DWORD *__thiscall sub_46A880(void *this)
{
  _DWORD *result; // eax

  for ( result = dword_566678; dword_566678; result = dword_566678 )
    sub_46A960(this, *result);
  dword_566678 = 0;
  dword_566670 = 0;
  return result;
}

// ===== sub_46A8B0 @ 0x0046A8B0..0x0046A95C =====
int __cdecl sub_46A8B0(_DWORD *a1, unsigned int a2)
{
  _DWORD *v2; // edi
  int *v3; // esi
  int *v4; // eax

  if ( a2 <= 1 )
    return -2147483647;
  v2 = operator new(0xCu);
  *v2 = ++dword_566670;
  v3 = (int *)operator new(8u);
  v4 = 0;
  if ( v3 )
    v4 = sub_447450(a2, v3);
  v2[1] = v4;
  v2[2] = dword_566678;
  dword_566678 = v2;
  *a1 = *v2;
  return 0;
}

// ===== sub_46A960 @ 0x0046A960..0x0046A9B0 =====
int __fastcall sub_46A960(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void *v5; // edi

  v2 = (int *)dword_566678;
  result = -2147483646;
  v4 = &dword_566670;
  if ( dword_566678 )
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
      sub_447490(v2[1]);
      operator delete(v5);
    }
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_46A9B0 @ 0x0046A9B0..0x0046A9CC =====
void **__thiscall sub_46A9B0(void *this)
{
  void **result; // eax

  result = (void **)dword_566678;
  if ( dword_566678 )
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

// ===== sub_46A9D0 @ 0x0046A9D0..0x0046AA24 =====
unsigned int __cdecl sub_46A9D0(int *a1, unsigned int a2, void *Src)
{
  void *v3; // ecx
  void **v4; // eax
  size_t v5; // edx
  int *v6; // eax

  v4 = sub_46A9B0(v3);
  if ( !v4 )
    return -2147483646;
  v6 = (int *)v4[1];
  if ( a1 )
    return sub_4474C0(v6, a1, Src, v5) != 0 ? 0x80000003 : 0;
  else
    return sub_447510((unsigned int *)v6, a2, Src, v5) != 0 ? 0x80000003 : 0;
}

// ===== sub_46AA30 @ 0x0046AA30..0x0046AA5D =====
unsigned int __cdecl sub_46AA30(unsigned int a1)
{
  void *v1; // ecx
  void **v2; // eax
  int v3; // edx

  v2 = sub_46A9B0(v1);
  if ( v2 )
    return sub_4475D0(a1, (unsigned int *)v2[1]) != 0 ? 0x80000004 : 0;
  else
    return v3;
}

// ===== sub_46AA60 @ 0x0046AA60..0x0046AA91 =====
unsigned int __usercall sub_46AA60@<eax>(void *a1@<ecx>, unsigned int a2@<esi>, void *a3, _DWORD *a4)
{
  void **v4; // eax
  int v5; // edx

  v4 = sub_46A9B0(a1);
  if ( v4 )
    return sub_447610(a3, (unsigned int *)v4[1], a2, a4) != 0 ? 0x80000004 : 0;
  else
    return v5;
}

// ===== sub_46AAA0 @ 0x0046AAA0..0x0046AACC =====
int __cdecl sub_46AAA0(void *a1, int *a2)
{
  void *v2; // ecx
  void **v3; // eax
  int v4; // edx

  v3 = sub_46A9B0(v2);
  if ( !v3 )
    return v4;
  *a2 = sub_447650((unsigned int *)v3[1], a1);
  return 0;
}

// ===== sub_46AAD0 @ 0x0046AAD0..0x0046ABE0 =====
void sub_46AAD0()
{
  HMODULE ModuleHandleA; // eax
  HMODULE v1; // eax
  HMODULE v2; // eax
  HMODULE v3; // eax
  HMODULE v4; // eax
  HMODULE v5; // eax
  BOOL (__stdcall *CloseTouchInputHandle)(HTOUCHINPUT); // eax
  _BYTE v7[4]; // [esp+0h] [ebp-94h] BYREF
  unsigned int v8; // [esp+4h] [ebp-90h]
  int v9; // [esp+8h] [ebp-8Ch]

  if ( !dword_56667C )
  {
    sub_46F800(v7);
    if ( v8 >= 6 )
    {
      if ( v9 )
      {
        ModuleHandleA = GetModuleHandleA("user32.dll");
        GetGestureInfo = (BOOL (__stdcall *)(HGESTUREINFO, PGESTUREINFO))GetProcAddress(ModuleHandleA, "GetGestureInfo");
        v1 = GetModuleHandleA("user32.dll");
        CloseGestureInfoHandle = (BOOL (__stdcall *)(HGESTUREINFO))GetProcAddress(v1, "CloseGestureInfoHandle");
        v2 = GetModuleHandleA("user32.dll");
        RegisterTouchWindow = (BOOL (__stdcall *)(HWND, ULONG))GetProcAddress(v2, "RegisterTouchWindow");
        v3 = GetModuleHandleA("user32.dll");
        UnregisterTouchWindow = (BOOL (__stdcall *)(HWND))GetProcAddress(v3, "UnregisterTouchWindow");
        v4 = GetModuleHandleA("user32.dll");
        GetTouchInputInfo = (BOOL (__stdcall *)(HTOUCHINPUT, UINT, PTOUCHINPUT, int))GetProcAddress(
                                                                                       v4,
                                                                                       "GetTouchInputInfo");
        v5 = GetModuleHandleA("user32.dll");
        CloseTouchInputHandle = (BOOL (__stdcall *)(HTOUCHINPUT))GetProcAddress(v5, "CloseTouchInputHandle");
        dword_566694 = (int)CloseTouchInputHandle;
        if ( !GetGestureInfo
          || !CloseGestureInfoHandle
          || !RegisterTouchWindow
          || !UnregisterTouchWindow
          || !GetTouchInputInfo
          || (dword_56667C = 1, !CloseTouchInputHandle) )
        {
          dword_56667C = 0;
        }
      }
    }
  }
}

// ===== sub_46ABE0 @ 0x0046ABE0..0x0046AC1D =====
int sub_46ABE0()
{
  dword_56667C = 0;
  GetGestureInfo = 0;
  CloseGestureInfoHandle = 0;
  RegisterTouchWindow = 0;
  UnregisterTouchWindow = 0;
  GetTouchInputInfo = 0;
  dword_566694 = 0;
  sub_46AF40();
  return sub_46B1C0(0);
}

// ===== sub_46AC20 @ 0x0046AC20..0x0046AD84 =====
int __thiscall sub_46AC20(HGESTUREINFO this)
{
  int v1; // edi
  int x; // ebx
  int y; // esi
  int v5; // ebx
  int v6; // esi
  int v7; // esi
  struct tagGESTUREINFO v9; // [esp+10h] [ebp-50h] BYREF
  HGESTUREINFO v10; // [esp+40h] [ebp-20h]
  int v11; // [esp+44h] [ebp-1Ch] BYREF
  int v12; // [esp+48h] [ebp-18h]
  struct tagRECT Rect; // [esp+4Ch] [ebp-14h] BYREF

  v1 = 0;
  v10 = this;
  if ( dword_56667C )
  {
    memset(&v9, 0, sizeof(v9));
    v9.cbSize = 48;
    if ( GetGestureInfo(this, &v9) )
    {
      GetWindowRect(hWndParent, &Rect);
      x = v9.ptsLocation.x;
      y = v9.ptsLocation.y;
      if ( sub_45F640() )
      {
        v5 = x - Rect.left;
        v7 = y - Rect.top;
      }
      else
      {
        v5 = x - (Rect.left + GetSystemMetrics(7));
        v6 = y - Rect.top - GetSystemMetrics(4);
        v7 = v6 - GetSystemMetrics(8);
      }
      sub_45E8D0(&v11, v5, v7, 1);
      if ( (v9.dwFlags & 1) != 0 )
      {
        switch ( v9.dwID )
        {
          case 3u:
            goto LABEL_8;
          case 4u:
            goto LABEL_10;
          case 5u:
          case 6u:
          case 7u:
            goto LABEL_11;
          default:
            return v1;
        }
      }
      else
      {
        switch ( v9.dwID )
        {
          case 3u:
LABEL_8:
            sub_496540(512, (unsigned __int16)v11 | (v12 << 16));
            goto LABEL_11;
          case 4u:
LABEL_10:
            sub_496540(513, (unsigned __int16)v11 | (v12 << 16));
            goto LABEL_11;
          case 5u:
          case 6u:
          case 7u:
LABEL_11:
            v1 = 1;
            CloseGestureInfoHandle(v10);
            break;
          default:
            return v1;
        }
      }
    }
  }
  return v1;
}

// ===== sub_46ADB0 @ 0x0046ADB0..0x0046AF40 =====
void __cdecl sub_46ADB0(int a1)
{
  void **v1; // esi
  void **v2; // ebx
  _DWORD *v3; // eax
  int v4; // ecx
  int v5; // ebx
  int v6; // edi
  int v7; // ebx
  int v8; // edi
  int v9; // edi
  int v10; // eax
  int v11[2]; // [esp+Ch] [ebp-20h] BYREF
  struct tagRECT Rect; // [esp+14h] [ebp-18h] BYREF

  v1 = (void **)dword_56669C;
  v2 = &dword_56669C;
  if ( dword_56669C )
  {
    do
    {
      if ( *v1 == *(void **)(a1 + 12) )
        break;
      v2 = v1 + 7;
      v1 = (void **)v1[7];
    }
    while ( v1 );
  }
  if ( (*(_BYTE *)(a1 + 16) & 4) != 0 )
  {
    if ( v1 )
    {
      *v2 = v1[7];
      operator delete(v1);
    }
  }
  else
  {
    if ( !v1 )
    {
      v3 = operator new(0x20u);
      *v3 = 0;
      v3[1] = 0;
      v3[2] = 0;
      v3[3] = 0;
      v3[4] = 0;
      v3[5] = 0;
      v3[6] = 0;
      v3[7] = 0;
      *v3 = *(_DWORD *)(a1 + 12);
      v4 = dword_566698;
      v3[1] = dword_566698;
      v3[7] = 0;
      dword_566698 = v4 + 1;
      *v2 = v3;
      v1 = (void **)v3;
    }
    GetWindowRect(hWndParent, &Rect);
    v5 = *(_DWORD *)a1 / 100;
    v6 = *(_DWORD *)(a1 + 4) / 100;
    if ( sub_45F640() )
    {
      v7 = v5 - Rect.left;
      v9 = v6 - Rect.top;
    }
    else
    {
      v7 = v5 - (Rect.left + GetSystemMetrics(7));
      v8 = v6 - Rect.top - GetSystemMetrics(4);
      v9 = v8 - GetSystemMetrics(8);
    }
    sub_45E8D0(v11, v7, v9, 1);
    v10 = v11[1];
    v1[2] = (void *)v11[0];
    v1[3] = (void *)v10;
    if ( (*(_BYTE *)(a1 + 20) & 4) != 0 )
    {
      v1[4] = (void *)(*(_DWORD *)(a1 + 32) / 0x64u);
      v1[5] = (void *)(*(_DWORD *)(a1 + 36) / 0x64u);
    }
    else
    {
      v1[4] = 0;
      v1[5] = 0;
    }
    if ( (*(_BYTE *)(a1 + 20) & 1) != 0 )
      v1[6] = *(void **)(a1 + 24);
    else
      v1[6] = (void *)sub_498720();
  }
}

// ===== sub_46AF40 @ 0x0046AF40..0x0046AF89 =====
int *sub_46AF40()
{
  int *result; // eax
  _BYTE v1[12]; // [esp+0h] [ebp-28h] BYREF
  int v2; // [esp+Ch] [ebp-1Ch]
  int i; // [esp+10h] [ebp-18h]

  result = (int *)dword_56669C;
  for ( i = 4; dword_56669C; result = (int *)dword_56669C )
  {
    v2 = *result;
    sub_46ADB0((int)v1);
  }
  dword_566698 = 0;
  return result;
}

// ===== sub_46AF90 @ 0x0046AF90..0x0046B0F4 =====
int __usercall sub_46AF90@<eax>(UINT a1@<eax>, HTOUCHINPUT a2)
{
  UINT v2; // edi
  int v3; // esi
  int v4; // esi
  UINT v5; // ebx
  int v6; // esi
  double v7; // st5
  struct tagTOUCHINPUT *v9; // [esp+18h] [ebp-Ch]
  int v10; // [esp+1Ch] [ebp-8h]

  v2 = a1;
  v3 = 0;
  if ( a1 )
  {
    v9 = (struct tagTOUCHINPUT *)operator new[](40 * a1);
    if ( !GetTouchInputInfo(a2, v2, v9, 40) )
    {
LABEL_19:
      operator delete[](v9);
      return v3;
    }
    dword_566694(a2);
    if ( v2 )
    {
      v4 = (int)v9;
      v5 = v2;
      do
      {
        if ( (*(_BYTE *)(v4 + 16) & 0x12) != 0 )
          sub_46ADB0(v4);
        v4 += 40;
        --v5;
      }
      while ( v5 );
      v6 = (int)v9;
      do
      {
        sub_46ADB0(v6);
        v6 += 40;
        --v2;
      }
      while ( v2 );
    }
    if ( dword_56669C )
    {
      if ( dword_5666B0 )
      {
        if ( *((_DWORD *)dword_56669C + 1) == *(_DWORD *)dword_5666B0 )
        {
          v10 = *((_DWORD *)dword_5666B0 + 2) - *((_DWORD *)dword_56669C + 3);
          v7 = (double)(*((_DWORD *)dword_5666B0 + 1) - *((_DWORD *)dword_56669C + 2));
          if ( sqrt(v7 * v7 + (double)v10 * (double)v10) < dbl_5666A8 )
            goto LABEL_18;
        }
        else
        {
          sub_46B1C0((__int64)dbl_5666A8);
        }
      }
      sub_46B240();
    }
    else
    {
      sub_46AF40();
      sub_46B1C0((__int64)dbl_5666A8);
    }
LABEL_18:
    v3 = 1;
    goto LABEL_19;
  }
  return v3;
}

// ===== sub_46B100 @ 0x0046B100..0x0046B131 =====
BOOL __cdecl sub_46B100(int a1)
{
  BOOL result; // eax

  result = 0;
  if ( dword_56667C )
  {
    if ( a1 )
      return RegisterTouchWindow(hWndParent, 1u);
    else
      return UnregisterTouchWindow(hWndParent);
  }
  return result;
}

// ===== sub_46B140 @ 0x0046B140..0x0046B184 =====
int __usercall sub_46B140@<eax>(_DWORD *a1@<esi>)
{
  _DWORD *v1; // ecx
  int result; // eax
  _DWORD *v3; // edx

  v1 = dword_56669C;
  result = 0;
  if ( dword_56669C )
  {
    v3 = a1;
    do
    {
      if ( a1 )
      {
        *v3 = v1[1];
        v3[1] = v1[2];
        v3[2] = v1[3];
        v3[3] = v1[4];
        v3[4] = v1[5];
        v3[5] = v1[6];
      }
      v1 = (_DWORD *)v1[7];
      ++result;
      v3 += 6;
    }
    while ( v1 );
  }
  return result;
}

// ===== sub_46B190 @ 0x0046B190..0x0046B1B3 =====
int __fastcall sub_46B190(int a1, _DWORD *a2)
{
  int result; // eax

  result = 0;
  if ( dword_56669C )
  {
    *a2 = *((_DWORD *)dword_56669C + 2);
    a2[1] = *((_DWORD *)dword_56669C + 3);
    return 1;
  }
  return result;
}

// ===== sub_46B1C0 @ 0x0046B1C0..0x0046B234 =====
int __usercall sub_46B1C0@<eax>(unsigned int a1@<esi>, unsigned int a2)
{
  int result; // eax
  _DWORD *i; // eax

  result = 0;
  if ( a1 <= 0x200 )
  {
    for ( i = dword_5666B0; dword_5666B0; i = dword_5666B0 )
    {
      dword_5666B0 = (void *)i[7];
      operator delete(i);
    }
    dword_5666A0 = a1;
    dword_5666A4 = 0;
    dbl_5666A8 = (double)a2;
    dword_5666B4 = 0;
    return 1;
  }
  return result;
}

// ===== sub_46B240 @ 0x0046B240..0x0046B2F5 =====
int __usercall sub_46B240@<eax>(_DWORD *a1@<esi>)
{
  int result; // eax
  void *v2; // ecx
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // edx

  result = 0;
  if ( dword_5666A4 >= (unsigned int)dword_5666A0 )
  {
    v2 = dword_5666B4;
    if ( !dword_5666B4 )
      return result;
    v3 = (_DWORD *)*((_DWORD *)dword_5666B4 + 6);
    dword_5666B4 = v3;
    if ( v3 )
      v3[7] = 0;
    else
      dword_5666B0 = 0;
    operator delete(v2);
  }
  else
  {
    ++dword_5666A4;
  }
  v4 = operator new(0x20u);
  *v4 = *a1;
  v4[1] = a1[1];
  v4[2] = a1[2];
  v4[3] = a1[3];
  v4[4] = a1[4];
  v5 = dword_5666B0;
  v4[5] = a1[5];
  v4[6] = 0;
  v4[7] = v5;
  if ( v5 )
    v5[6] = v4;
  dword_5666B0 = v4;
  if ( !dword_5666B4 )
    dword_5666B4 = v4;
  return 1;
}

// ===== sub_46B300 @ 0x0046B300..0x0046B381 =====
unsigned int __cdecl sub_46B300(int a1, int a2, unsigned int a3)
{
  unsigned int v3; // ecx
  unsigned int result; // eax
  _DWORD *i; // esi
  unsigned int j; // edi

  result = 0;
  if ( v3 < dword_5666A4 )
  {
    for ( i = (_DWORD *)*((_DWORD *)dword_5666B0 + 7); v3; i = (_DWORD *)i[7] )
      --v3;
    for ( j = 0; j < a3; ++j )
    {
      if ( !i )
        break;
      *(_DWORD *)(a1 + 8 * j) = i[1];
      *(_DWORD *)(a1 + 8 * j + 4) = i[2];
      *(_DWORD *)(a2 + 4 * j) = i[7] ? sub_401000(i[1] - *(_DWORD *)(i[7] + 4), i[2] - *(_DWORD *)(i[7] + 8)) : -1;
      i = (_DWORD *)i[7];
    }
    return j;
  }
  return result;
}

// ===== sub_46B390 @ 0x0046B390..0x0046B3F5 =====
int __usercall sub_46B390@<eax>(const char *a1@<esi>)
{
  DWORD FileAttributesA; // eax
  const CHAR *v3; // [esp+0h] [ebp-8h]

  FileAttributesA = GetFileAttributesA(v3);
  if ( FileAttributesA == -1 || (FileAttributesA & 0x10) == 0 )
    return 0;
  if ( a1[strlen(a1) - 1] == 92 )
    strcpy(byte_518978, a1);
  else
    sprintf(byte_518978, "%s\\", a1);
  return 1;
}

// ===== sub_46B400 @ 0x0046B400..0x0046B415 =====
int __usercall sub_46B400@<eax>(char *Buffer@<ecx>, const char *a2@<eax>)
{
  return sprintf(Buffer, "%s%s", byte_518978, a2);
}

// ===== sub_46B420 @ 0x0046B420..0x0046B426 =====
int __usercall sub_46B420@<eax>(int result@<eax>)
{
  dword_506BDC = result;
  return result;
}

// ===== sub_46B430 @ 0x0046B430..0x0046B444 =====
int __usercall sub_46B430@<eax>(void *a1@<eax>)
{
  sub_4956E0(a1);
  return 1;
}

// ===== sub_46B450 @ 0x0046B450..0x0046B45F =====
BOOL __usercall sub_46B450@<eax>(int a1@<eax>)
{
  return sub_4958C0(a1) == 0;
}

// ===== sub_46B460 @ 0x0046B460..0x0046B474 =====
BOOL __usercall sub_46B460@<eax>(const char *a1@<ecx>, int a2@<eax>)
{
  return sub_446B80(dword_56676C, a1, a2) == 0;
}

// ===== sub_46B480 @ 0x0046B480..0x0046B48E =====
int __usercall sub_46B480@<eax>(int a1@<eax>, unsigned int a2@<esi>, int a3)
{
  return sub_446CD0(a3, a2, a1);
}

// ===== sub_46B490 @ 0x0046B490..0x0046B4A2 =====
int __usercall sub_46B490@<eax>(int a1@<eax>, int a2@<ecx>, int a3, unsigned int a4)
{
  return sub_446D40(a3, a4, a2, a1);
}

// ===== sub_46B4B0 @ 0x0046B4B0..0x0046B4BE =====
int __usercall sub_46B4B0@<eax>(_DWORD *a1@<eax>, unsigned int a2@<esi>, int a3)
{
  return sub_446DE0(a3, a2, a1);
}

// ===== sub_46B4C0 @ 0x0046B4C0..0x0046B67F =====
BOOL sub_46B4C0()
{
  char *v0; // eax
  _DWORD *v1; // ebx
  const void *v2; // esi
  const void *v3; // eax
  void *v4; // esi
  DWORD v5; // edi
  int v6; // ebx
  HWND v8; // [esp-8h] [ebp-650h]
  int v9; // [esp+Ch] [ebp-63Ch]
  DWORD nNumberOfBytesToWrite; // [esp+10h] [ebp-638h]
  int v11; // [esp+14h] [ebp-634h] BYREF
  LPCVOID lpBuffer; // [esp+18h] [ebp-630h]
  struct tagRECT Rect; // [esp+1Ch] [ebp-62Ch] BYREF
  char Buffer[780]; // [esp+2Ch] [ebp-61Ch] BYREF
  char v15[780]; // [esp+338h] [ebp-310h] BYREF

  v9 = sub_495640(0) + 4;
  sub_446E40(0, &v11);
  nNumberOfBytesToWrite = v11 + v9 + 1049636;
  v0 = (char *)operator new[](nNumberOfBytesToWrite);
  strcpy(v0, "BURIKO GDB 3.00");
  v1 = v0 + 16;
  lpBuffer = v0;
  v8 = hWndParent;
  *((_DWORD *)v0 + 4) = nNumberOfBytesToWrite;
  GetWindowRect(v8, &Rect);
  v2 = dword_566758;
  v3 = dword_566760;
  v1[1] = Rect.left;
  v1[2] = Rect.top;
  v1[3] = 1024;
  qmemcpy(v1 + 4, v2, 0x400u);
  v1[260] = 0x100000;
  memcpy_0(v1 + 261, v3, 0x100000u);
  v1 += 262405;
  *v1 = sub_4956C0();
  sub_495640(v1 + 1);
  sub_446E40((_DWORD *)((char *)v1 + v9), &v11);
  v4 = operator new[](2 * nNumberOfBytesToWrite);
  v5 = sub_493980(lpBuffer);
  if ( !v5 )
  {
    sub_46B400(Buffer, "BGIError.txt");
    sub_465E30(lpBuffer, nNumberOfBytesToWrite);
    sub_464500((int)&unk_4E70BC);
  }
  sub_46B400(v15, "BGI.gdb");
  v6 = sub_465E30(v4, v5);
  operator delete[](v4);
  operator delete[]((void *)lpBuffer);
  return v6 == v5;
}

// ===== sub_46B680 @ 0x0046B680..0x0046BA1C =====
int __cdecl sub_46B680(_DWORD *a1)
{
  size_t v1; // eax
  _DWORD *v2; // esi
  char *v3; // edi
  unsigned int v4; // eax
  _DWORD *v5; // ebx
  int v6; // esi
  int v7; // edi
  size_t v8; // esi
  size_t *v9; // edi
  size_t v10; // esi
  char *v11; // ebx
  int v12; // esi
  _DWORD *v13; // esi
  BOOL v14; // ebx
  int v15; // esi
  char *v16; // edi
  _BYTE *v17; // edi
  _BYTE *v18; // eax
  int v19; // ebx
  int *v20; // esi
  unsigned int v22; // [esp+14h] [ebp-330h]
  int v23; // [esp+18h] [ebp-32Ch]
  _DWORD *v24; // [esp+1Ch] [ebp-328h]
  int v25; // [esp+20h] [ebp-324h]
  char *v26; // [esp+24h] [ebp-320h]
  int v27; // [esp+28h] [ebp-31Ch]
  int v28; // [esp+28h] [ebp-31Ch]
  char Buffer[788]; // [esp+2Ch] [ebp-318h] BYREF

  sub_46B400(Buffer, "BGI.gdb");
  v1 = sub_4662E0(Buffer);
  if ( v1 )
  {
    v25 = -2147483646;
    v2 = operator new[](v1);
    v24 = v2;
    sub_465AB0(Buffer, v2, 0);
    v3 = (char *)operator new[](v2[6]);
    v26 = v3;
    v4 = sub_4938F0(v3);
    v22 = v4;
    if ( v4 == v2[6] )
    {
      if ( !strcmp(v3, (const char *)&dword_4E70AC) )
      {
        v5 = v3 + 16;
        if ( v4 == *((_DWORD *)v3 + 4) )
        {
          v6 = *((_DWORD *)v3 + 5);
          v7 = *((_DWORD *)v3 + 6);
          if ( a1 )
          {
            if ( sub_46F930(v5[2]) )
            {
              *a1 = v6;
              a1[1] = v7;
            }
            else
            {
              sub_4615E0(a1);
            }
          }
          v8 = v5[3];
          memcpy_0(dword_566758, v5 + 4, v8);
          v9 = (_DWORD *)((char *)v5 + v8 + 16);
          v10 = *v9;
          v11 = (char *)dword_566760;
          memcpy_0(dword_566760, v9 + 1, *v9);
          if ( v10 < 0x100000 )
            memset(&v11[v10], 0, 0x100000 - v10);
          v12 = (int)v9 + v10 + 4;
          sub_495550(0x80000000, (void *)(v12 + 4));
          v13 = (_DWORD *)(v12 + sub_495640(0) + 4);
          sub_446B40(dword_56676C);
          sub_446F00(v13);
LABEL_35:
          v25 = 0;
LABEL_36:
          v3 = v26;
          v2 = v24;
        }
      }
      else if ( v4 >= 0x70408 )
      {
        v14 = !v3[394248] || strlen(v3 + 394248) >= 0x18 || !*((_DWORD *)v3 + 98568) || *((_DWORD *)v3 + 98569);
        if ( a1 )
        {
          v15 = *(_DWORD *)v3;
          if ( sub_46F930(*((_DWORD *)v3 + 1)) )
          {
            *a1 = v15;
            a1[1] = *((_DWORD *)v3 + 1);
          }
          else
          {
            sub_4615E0(a1);
          }
        }
        qmemcpy(dword_566758, v3 + 8, 0x400u);
        v16 = (char *)dword_566760;
        memcpy_0(dword_566760, v26 + 1032, 0x40000u);
        memset(v16 + 0x40000, 0, 0xC0000u);
        sub_495550(0x80000000, 0);
        if ( (v14 ? 0x4000 : 4096) > 0 )
        {
          v17 = v26 + 263176;
          v27 = v14 ? 0x4000 : 4096;
          do
          {
            if ( *v17 )
              sub_4956E0(v17);
            v17 += 32;
            --v27;
          }
          while ( v27 );
        }
        sub_446B40(dword_56676C);
        v18 = v26 + 787464;
        if ( !v14 )
          v18 = v26 + 394248;
        v23 = v14 ? 853000 : 459784;
        v19 = 0;
        v20 = (int *)(v18 + 24);
        v28 = 2048;
        do
        {
          if ( *v20 )
          {
            sub_446C80(*v20, dword_56676C, (const char *)v20 - 24, &v26[v23 + v20[1]]);
            v19 += sub_446B10(*v20);
          }
          v20 += 8;
          --v28;
        }
        while ( v28 );
        if ( v22 != v19 + v23 )
          goto LABEL_36;
        goto LABEL_35;
      }
    }
    operator delete[](v2);
    operator delete[](v3);
    return v25;
  }
  return -2147483647;
}

// ===== sub_46BA20 @ 0x0046BA20..0x0046BB22 =====
int __cdecl sub_46BA20(_DWORD *a1)
{
  size_t v1; // eax
  _DWORD *v2; // esi
  int *v3; // edi
  unsigned int v4; // eax
  int v5; // esi
  int v6; // edx
  int v8; // [esp+Ch] [ebp-8h]
  _DWORD *v9; // [esp+10h] [ebp-4h]

  v8 = 0;
  v1 = sub_4662E0("BGI.gdb");
  if ( v1 )
  {
    v2 = operator new[](v1);
    v9 = v2;
    sub_465AB0("BGI.gdb", v2, 0);
    v3 = (int *)operator new[](v2[6]);
    v4 = sub_4938F0(v3);
    if ( v4 != v2[6] )
    {
LABEL_12:
      operator delete[](v2);
      operator delete[](v3);
      return v8;
    }
    if ( !strcmp((const char *)v3, (const char *)&dword_4E70AC) )
    {
      if ( v4 != v3[4] )
        goto LABEL_11;
      v5 = v3[5];
      if ( !sub_46F930(v3[6]) )
        goto LABEL_11;
      v6 = v3[6];
    }
    else
    {
      if ( v4 < 0xD0408 )
        goto LABEL_11;
      v5 = *v3;
      if ( !sub_46F930(v3[1]) )
        goto LABEL_11;
      v6 = v3[1];
    }
    v8 = 1;
    a1[1] = v6;
    *a1 = v5;
LABEL_11:
    v2 = v9;
    goto LABEL_12;
  }
  return 0;
}

// ===== sub_46BB30 @ 0x0046BB30..0x0046BB3C =====
void sub_46BB30()
{
  InitializeCriticalSection(&stru_518960);
}

// ===== sub_46BB40 @ 0x0046BB40..0x0046BB62 =====
void sub_46BB40()
{
  EnterCriticalSection(&stru_518960);
  LeaveCriticalSection(&stru_518960);
  DeleteCriticalSection(&stru_518960);
}

// ===== sub_46BB70 @ 0x0046BB70..0x0046BB7C =====
void sub_46BB70()
{
  EnterCriticalSection(&stru_518960);
}

// ===== sub_46BB80 @ 0x0046BB80..0x0046BB8C =====
void sub_46BB80()
{
  LeaveCriticalSection(&stru_518960);
}

// ===== sub_46BB90 @ 0x0046BB90..0x0046BBDE =====
int __fastcall sub_46BB90(unsigned int a1)
{
  int result; // eax
  unsigned int v2; // eax

  result = 0;
  if ( a1 <= 0xC )
  {
    v2 = 4096 << a1;
    Size = 4096 << a1;
    if ( dword_566758 )
    {
      operator delete[](dword_566758);
      v2 = Size;
    }
    dword_566758 = operator new[](v2);
    memset(dword_566758, 0, Size);
    return 1;
  }
  return result;
}

// ===== sub_46BBE0 @ 0x0046BBE0..0x0046BC28 =====
char *__usercall sub_46BBE0@<eax>(char *result@<eax>)
{
  char *v1; // edx
  char v2; // cl

  if ( strlen(result) >= 0x100 )
  {
    qmemcpy(aEthornellBurik_0, result, 0xFFu);
    byte_506B87 = 0;
  }
  else
  {
    v1 = (char *)(aEthornellBurik_0 - result);
    do
    {
      v2 = *result;
      result[(_DWORD)v1] = *result;
      ++result;
    }
    while ( v2 );
  }
  return result;
}

// ===== sub_46BC30 @ 0x0046BC30..0x0046BC7B =====
void __usercall sub_46BC30(const char *a1@<esi>)
{
  const char *v1; // ecx
  int v2; // edx
  char v3; // al

  operator delete[](dword_5666E4);
  if ( a1 )
  {
    dword_5666E4 = operator new[](strlen(a1) + 1);
    v1 = a1;
    v2 = (_BYTE *)dword_5666E4 - a1;
    do
    {
      v3 = *v1;
      v1[v2] = *v1;
      ++v1;
    }
    while ( v3 );
  }
  else
  {
    dword_5666E4 = 0;
  }
}

// ===== sub_46BC80 @ 0x0046BC80..0x0046BCF0 =====
int __usercall sub_46BC80@<eax>(const CHAR *a1@<eax>, HWND hWnd, const CHAR *lpText, UINT uType)
{
  int v5; // edi
  const CHAR *v6; // eax
  int v7; // esi

  sub_45FFB0(1);
  sub_498770(1);
  v5 = sub_48ED40(1);
  if ( a1 )
  {
    v6 = a1;
  }
  else
  {
    v6 = (const CHAR *)dword_5666E4;
    if ( !dword_5666E4 )
      v6 = aEthornellBurik_0;
  }
  v7 = MessageBoxA(hWnd, lpText, v6, uType);
  sub_48ED40(v5);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v7;
}

// ===== sub_46BCF0 @ 0x0046BCF0..0x0046BD57 =====
BOOL __usercall sub_46BCF0@<eax>(void **a1@<esi>)
{
  BOOL result; // eax

  result = a1 != 0;
  if ( a1 )
  {
    if ( a1[9] )
      operator delete[](a1[9]);
    if ( a1[10] )
      operator delete[](a1[10]);
    if ( a1[11] )
      operator delete[](a1[11]);
    operator delete[](a1[12]);
    if ( a1[13] )
      operator delete[](a1[13]);
    operator delete(a1);
    return a1 != 0;
  }
  return result;
}

// ===== sub_46BD60 @ 0x0046BD60..0x0046BDA8 =====
int __cdecl sub_46BD60(int a1)
{
  int v1; // edi
  void **v2; // esi
  int result; // eax

  v1 = dword_5667BC;
  while ( v1 )
  {
    v2 = (void **)v1;
    v1 = *(_DWORD *)(v1 + 60);
    sub_46BCF0(v2);
  }
  result = a1;
  dword_5667B8 = 0;
  dword_5667BC = 0;
  dword_5667C0 = a1;
  dword_5667C4 = 0;
  return result;
}

// ===== sub_46BDB0 @ 0x0046BDB0..0x0046BFCF =====
int __usercall sub_46BDB0@<eax>(
        const char *a1@<eax>,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        const char *a11,
        const char *a12,
        const char *a13,
        const char *a14)
{
  _DWORD *v15; // esi
  int v16; // eax
  _BYTE *v17; // eax
  const char *v18; // ecx
  _BYTE *v19; // edx
  char v20; // al
  int v22; // eax
  _BYTE *v23; // eax
  const char *v24; // ecx
  _BYTE *v25; // edx
  char v26; // al
  int v27; // eax
  _BYTE *v28; // eax
  const char *v29; // ecx
  _BYTE *v30; // edx
  char v31; // al
  int v32; // eax
  _BYTE *v33; // eax
  const char *v34; // ecx
  _BYTE *v35; // edx
  char v36; // al
  int v37; // eax
  _BYTE *v38; // eax
  const char *v39; // ecx
  _BYTE *v40; // edx
  char v41; // al
  int v42; // eax
  void **v43; // esi

  v15 = operator new(0x40u);
  *v15 = a2;
  v15[1] = a3;
  v15[2] = a4;
  v15[3] = a5;
  v15[4] = a6;
  v15[5] = a7;
  v15[6] = a8;
  v15[7] = a9;
  v15[8] = a10;
  if ( a1 )
  {
    v16 = strlen(a1);
    if ( v16 >= 32 )
      return -2147483647;
    v17 = operator new[](v16 + 1);
    v15[9] = v17;
    v18 = a1;
    v19 = v17;
    do
    {
      v20 = *v18;
      *v19++ = *v18++;
    }
    while ( v20 );
  }
  else
  {
    v15[9] = 0;
  }
  if ( a11 )
  {
    v22 = strlen(a11);
    if ( v22 >= 32 )
      return -2147483646;
    v23 = operator new[](v22 + 1);
    v15[10] = v23;
    v24 = a11;
    v25 = v23;
    do
    {
      v26 = *v24;
      *v25++ = *v24++;
    }
    while ( v26 );
  }
  else
  {
    v15[10] = 0;
  }
  if ( a12 )
  {
    v27 = strlen(a12);
    if ( v27 >= 32 )
      return -2147483645;
    v28 = operator new[](v27 + 1);
    v15[11] = v28;
    v29 = a12;
    v30 = v28;
    do
    {
      v31 = *v29;
      *v30++ = *v29++;
    }
    while ( v31 );
  }
  else
  {
    v15[11] = 0;
  }
  v32 = strlen(a13);
  if ( v32 >= 256 )
    return -2147483644;
  v33 = operator new[](v32 + 1);
  v15[12] = v33;
  v34 = a13;
  v35 = v33;
  do
  {
    v36 = *v34;
    *v35++ = *v34++;
  }
  while ( v36 );
  if ( a14 )
  {
    v37 = strlen(a14);
    if ( v37 >= 512 )
      return -2147483643;
    v38 = operator new[](v37 + 1);
    v15[13] = v38;
    v39 = a14;
    v40 = v38;
    do
    {
      v41 = *v39;
      *v40++ = *v39++;
    }
    while ( v41 );
  }
  else
  {
    v15[13] = 0;
  }
  v15[14] = &unk_566780;
  v42 = dword_5667BC;
  v15[15] = dword_5667BC;
  if ( v42 )
    *(_DWORD *)(v42 + 56) = v15;
  else
    dword_5667B8 = (int)v15;
  dword_5667BC = (int)v15;
  if ( dword_5667C4 >= (unsigned int)dword_5667C0 )
  {
    v43 = (void **)dword_5667B8;
    dword_5667B8 = *(_DWORD *)(dword_5667B8 + 56);
    *(_DWORD *)(dword_5667B8 + 60) = 0;
    sub_46BCF0(v43);
  }
  else
  {
    ++dword_5667C4;
  }
  return 0;
}

// ===== sub_46BFD0 @ 0x0046BFD0..0x0046C087 =====
int __thiscall sub_46BFD0(int this)
{
  return sub_46BDB0(
           *(_BYTE *)(this + 160) != 0 ? (const char *)(this + 160) : 0,
           *(_DWORD *)this,
           *(_DWORD *)(this + 64),
           *(_DWORD *)(this + 68),
           *(_DWORD *)(this + 72),
           *(_DWORD *)(this + 76),
           *(_DWORD *)(this + 80),
           *(_DWORD *)(this + 84),
           *(_DWORD *)(this + 88),
           *(_DWORD *)(this + 92),
           *(_BYTE *)(this + 192) != 0 ? (const char *)(this + 192) : 0,
           *(_BYTE *)(this + 224) != 0 ? (const char *)(this + 224) : 0,
           (const char *)(this + 256),
           *(_BYTE *)(this + 512) != 0 ? (const char *)(this + 512) : 0);
}
