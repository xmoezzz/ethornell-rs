#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4708F0 @ 0x004708F0..0x00470AD3 =====
void *__usercall sub_4708F0@<eax>(int a1@<eax>, int a2)
{
  void *result; // eax
  void *v5; // esi
  int v6; // eax
  char *v7; // ecx
  _BYTE *v8; // edx
  char v9; // al
  char *v10; // ecx
  _BYTE *v11; // edx
  char v12; // al
  HANDLE Thread; // eax
  char *v14; // ebx
  _BYTE *v15; // ecx
  char v16; // al
  char *v17; // ecx
  _BYTE *v18; // edi
  char v19; // al
  int v20; // [esp+10h] [ebp+8h]
  _DWORD *v21; // [esp+10h] [ebp+8h]

  result = 0;
  if ( a2 < *(_DWORD *)(a1 + 8) )
  {
    v5 = operator new(0x24u);
    *(_DWORD *)v5 = operator new[](0x30Cu);
    *((_DWORD *)v5 + 1) = operator new[](0x30Cu);
    *((_DWORD *)v5 + 2) = operator new[](0x30Cu);
    *((_DWORD *)v5 + 3) = operator new[](0x30Cu);
    *((_DWORD *)v5 + 4) = operator new[](0x30Cu);
    *((_DWORD *)v5 + 5) = *(_DWORD *)(a1 + 16);
    *((_DWORD *)v5 + 6) = a2;
    v6 = sub_46FE40((void *)a2);
    v7 = *(char **)(*(_DWORD *)(a1 + 12) + 4 * a2);
    v8 = *(_BYTE **)v5;
    v20 = v6;
    do
    {
      v9 = *v7;
      *v8++ = *v7++;
    }
    while ( v9 );
    sprintf(*((char *const *)v5 + 1), "%s\\%s", *(const char **)a1, *(const char **)(*(_DWORD *)(a1 + 12) + 4 * a2));
    sprintf(
      *((char *const *)v5 + 2),
      "%s\\%s",
      *(const char **)(a1 + 4),
      *(const char **)(*(_DWORD *)(a1 + 12) + 4 * a2));
    sprintf(*((char *const *)v5 + 3), "%s\\%s", *(const char **)(a1 + 4), *(const char **)(dword_566850 + 4 * v20));
    v10 = *(char **)(dword_566854 + 4 * v20);
    v11 = (_BYTE *)*((_DWORD *)v5 + 4);
    do
    {
      v12 = *v10;
      *v11++ = *v10++;
    }
    while ( v12 );
    Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, v5, 0, (LPDWORD)v5 + 8);
    *((_DWORD *)v5 + 7) = Thread;
    if ( Thread )
    {
      v21 = operator new(0xCu);
      *v21 = operator new[](strlen(*(const char **)(*(_DWORD *)(a1 + 12) + 4 * a2)) + 1);
      v21[1] = operator new[](strlen(*((const char **)v5 + 1)) + 1);
      v21[2] = dword_51891C;
      v14 = *(char **)(*(_DWORD *)(a1 + 12) + 4 * a2);
      v15 = (_BYTE *)*v21;
      do
      {
        v16 = *v14;
        *v15++ = *v14++;
      }
      while ( v16 );
      v17 = (char *)*((_DWORD *)v5 + 1);
      v18 = (_BYTE *)v21[1];
      do
      {
        v19 = *v17;
        *v18++ = *v17++;
      }
      while ( v19 );
      result = v5;
      dword_51891C = (int)v21;
    }
    else
    {
      operator delete[](*(void **)v5);
      operator delete[](*((void **)v5 + 1));
      operator delete[](*((void **)v5 + 2));
      operator delete[](*((void **)v5 + 3));
      operator delete[](*((void **)v5 + 4));
      operator delete(v5);
      return 0;
    }
  }
  return result;
}

// ===== sub_470AE0 @ 0x00470AE0..0x00470B4A =====
BOOL __usercall sub_470AE0@<eax>(void **a1@<esi>)
{
  BOOL result; // eax

  result = a1 != 0;
  if ( a1 )
  {
    if ( *a1 )
      operator delete[](*a1);
    if ( a1[1] )
      operator delete[](a1[1]);
    if ( a1[2] )
      operator delete[](a1[2]);
    if ( a1[3] )
      operator delete[](a1[3]);
    if ( a1[4] )
      operator delete[](a1[4]);
    operator delete(a1);
    return a1 != 0;
  }
  return result;
}

// ===== sub_470B50 @ 0x00470B50..0x00470B70 =====
int __usercall sub_470B50@<eax>(int result@<eax>)
{
  int i; // esi

  for ( i = *(_DWORD *)(result + 8); i; i = *(_DWORD *)(i + 8) )
    result = DeleteFileA(*(LPCSTR *)(i + 4));
  return result;
}

// ===== sub_470B70 @ 0x00470B70..0x00470BAF =====
void __cdecl sub_470B70(int a1)
{
  int v1; // esi
  void **v2; // edi
  void *v3; // eax

  v1 = *(_DWORD *)(a1 + 8);
  while ( v1 )
  {
    v2 = (void **)v1;
    v3 = *(void **)v1;
    v1 = *(_DWORD *)(v1 + 8);
    operator delete[](v3);
    operator delete[](v2[1]);
    operator delete(v2);
  }
  *(_DWORD *)(a1 + 8) = 0;
}

// ===== sub_470BB0 @ 0x00470BB0..0x00470F63 =====
int __fastcall sub_470BB0(CHAR *a1, const CHAR *a2, int a3, BYTE *a4, const void *a5)
{
  const char *v5; // ecx
  int v6; // edi
  char *v7; // esi
  DWORD FileAttributesA; // eax
  bool v9; // zf
  const CHAR *v10; // eax
  const CHAR *v11; // edi
  CHAR *v12; // esi
  const CHAR *v13; // eax
  DWORD NumberOfBytesWritten[3]; // [esp+10h] [ebp-68A0h] BYREF
  DWORD NumberOfBytesRead[3]; // [esp+1Ch] [ebp-6894h] BYREF
  BYTE *lpData; // [esp+28h] [ebp-6888h]
  int v18; // [esp+2Ch] [ebp-6884h]
  int v19; // [esp+30h] [ebp-6880h]
  LPSTR lpClass; // [esp+34h] [ebp-687Ch] BYREF
  const CHAR *v21; // [esp+38h] [ebp-6878h]
  LPCVOID lpBuffer; // [esp+3Ch] [ebp-6874h]
  HKEY phkResult; // [esp+40h] [ebp-6870h] BYREF
  CHAR SubKey[4096]; // [esp+44h] [ebp-686Ch] BYREF
  char Buffer[21060]; // [esp+1044h] [ebp-586Ch] BYREF
  CHAR FileName[780]; // [esp+6288h] [ebp-628h] BYREF
  BYTE Data[780]; // [esp+6594h] [ebp-31Ch] BYREF
  int v28; // [esp+68ACh] [ebp-4h]

  lpClass = a1;
  lpData = a4;
  lpBuffer = a5;
  v21 = a2;
  v19 = 0;
  sub_42D3B0(NumberOfBytesRead);
  v28 = 0;
  sub_42D3B0(NumberOfBytesWritten);
  LOBYTE(v28) = 1;
  sprintf((char *const)Data, "%s\\%s", *(const char **)a3, v5);
  phkResult = (HKEY)1;
  while ( 1 )
  {
    v6 = 0;
    v18 = sub_46FE60(Buffer);
    if ( v18 > 0 )
    {
      v7 = Buffer;
      while ( 1 )
      {
        sprintf(FileName, "%s\\%s\\%s", v7, *(const char **)(a3 + 4), (const char *)lpBuffer);
        if ( sub_464B80(FileName) )
        {
          FileAttributesA = GetFileAttributesA(FileName);
          if ( FileAttributesA != -1 )
            break;
        }
        ++v6;
        v7 += 780;
        if ( v6 >= v18 )
          goto LABEL_9;
      }
      if ( (FileAttributesA & 0x10) == 0 )
        break;
    }
LABEL_9:
    if ( sub_46BC80(0, *(HWND *)(a3 + 16), v21, 0x41u) != 1 )
    {
      v9 = !sub_46F710();
      v10 = (const CHAR *)&unk_4E6DC4;
      if ( v9 )
        v10 = "Are you sure you want to quit?";
      if ( sub_46BC80(0, *(HWND *)(a3 + 16), v10, 0x124u) == 6 )
        phkResult = 0;
    }
    Sleep(0xC8u);
    if ( !phkResult )
      goto LABEL_26;
  }
  sub_42D520(FileName, (int)NumberOfBytesRead);
  v11 = (const CHAR *)sub_42D650((int)NumberOfBytesRead);
  lpBuffer = operator new[]((unsigned int)v11);
  sub_42D5D0((DWORD)v11, (LPVOID)lpBuffer, (DWORD)NumberOfBytesRead);
  sub_42D5B0((int)NumberOfBytesRead);
  if ( !sub_42D570((LPCSTR)Data, (int)NumberOfBytesWritten, 0) )
  {
    v9 = !sub_46F710();
    v13 = (const CHAR *)&unk_4E73DC;
    if ( v9 )
      v13 = "Unistaller could not be installed.";
    goto LABEL_24;
  }
  v21 = (const CHAR *)sub_42D600((DWORD)v11, lpBuffer, (DWORD)NumberOfBytesWritten);
  sub_42D5B0((int)NumberOfBytesWritten);
  if ( v21 != v11 )
  {
    DeleteFileA((LPCSTR)Data);
    v9 = !sub_46F710();
    v13 = (const CHAR *)&unk_4E73DC;
    if ( v9 )
      v13 = "Unistaller could not be installed.";
LABEL_24:
    sub_46BC80(0, *(HWND *)(a3 + 16), v13, 0x10u);
    goto LABEL_25;
  }
  v12 = lpClass;
  sprintf(SubKey, "%s\\%s", "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", lpClass);
  RegCreateKeyExA(HKEY_LOCAL_MACHINE, SubKey, 0, v12, 0, 0x20006u, 0, &phkResult, (LPDWORD)&lpClass);
  RegSetValueExA(phkResult, "DisplayName", 0, 1u, (const BYTE *)v12, strlen(v12) + 1);
  RegSetValueExA(phkResult, "Publisher", 0, 1u, lpData, strlen((const char *)lpData) + 1);
  RegSetValueExA(phkResult, "UninstallString", 0, 1u, Data, strlen((const char *)Data) + 1);
  RegSetValueExA(phkResult, "DisplayIcon", 0, 1u, Data, strlen((const char *)Data) + 1);
  v19 = 1;
LABEL_25:
  operator delete[]((void *)lpBuffer);
LABEL_26:
  LOBYTE(v28) = 0;
  sub_42D400(NumberOfBytesWritten);
  v28 = -1;
  sub_42D400(NumberOfBytesRead);
  return v19;
}

// ===== sub_470F70 @ 0x00470F70..0x004712BD =====
BOOL __fastcall sub_470F70(const char *a1, const char **a2, int a3, const char *a4, int a5, int a6)
{
  DWORD v6; // eax
  char *v7; // ebx
  DWORD v8; // edi
  char *v9; // esi
  char *v10; // esi
  int i; // edi
  const char **j; // edi
  const char **v13; // edi
  BOOL v14; // edi
  unsigned int v15; // kr10_4
  bool v16; // zf
  const CHAR *v17; // eax
  char **v18; // esi
  char *v19; // eax
  int v20; // ebx
  char *v21; // eax
  char *v22; // edx
  char *v23; // ecx
  char v24; // al
  DWORD NumberOfBytesRead[3]; // [esp+1Ch] [ebp-948h] BYREF
  int v29; // [esp+28h] [ebp-93Ch]
  char *Str; // [esp+2Ch] [ebp-938h]
  char v31[780]; // [esp+30h] [ebp-934h] BYREF
  char SubStr[780]; // [esp+33Ch] [ebp-628h] BYREF
  char Buffer[780]; // [esp+648h] [ebp-31Ch] BYREF
  int v34; // [esp+960h] [ebp-4h]

  v29 = a3;
  sprintf(Buffer, "%s\\%s", *(const char **)a6, "uninst.lst");
  Str = (char *)operator new[](0x100000u);
  sub_42D3B0(NumberOfBytesRead);
  v34 = 0;
  if ( sub_42D520(Buffer, (int)NumberOfBytesRead) )
  {
    v6 = sub_42D650((int)NumberOfBytesRead);
    v7 = Str;
    v8 = v6;
    sub_42D5D0(v6, Str, (DWORD)NumberOfBytesRead);
    sub_42D5B0((int)NumberOfBytesRead);
    v9 = &v7[v8 - 1];
  }
  else
  {
    v10 = Str;
    sprintf(Str, "%s\n%s\n@%s\n@%s\n", a4, "uninst.lst", "BGIError.txt", "BGI.gdb");
    v9 = &v10[strlen(v10)];
    for ( i = 0; i < a5; v9 += strlen(v9) )
    {
      if ( a1 )
        sprintf(v31, a1, i);
      else
        sub_465F70(v31, i);
      sprintf(v9, "@%s\n", v31);
      ++i;
    }
    v7 = Str;
  }
  for ( j = *(const char ***)(v29 + 8); j; j = (const char **)j[2] )
  {
    if ( !strstr(v7, *j) )
    {
      sprintf(v9, "%s\n", *j);
      v9 += strlen(v9);
    }
  }
  v13 = a2;
  if ( a2 && *a2 )
  {
    do
    {
      sprintf(SubStr, "$%s", *v13);
      if ( !strstr(v7, SubStr) )
      {
        sprintf(v9, "%s\n", SubStr);
        v9 += strlen(v9);
      }
      ++v13;
    }
    while ( *v13 );
  }
  v14 = 0;
  if ( sub_42D570(Buffer, (int)NumberOfBytesRead, 0) )
  {
    v15 = strlen(v7);
    v14 = sub_42D600(v15 + 1, v7, (DWORD)NumberOfBytesRead) == v15 + 1;
    sub_42D5B0((int)NumberOfBytesRead);
  }
  operator delete[](v7);
  if ( !v14 )
  {
    v16 = !sub_46F710();
    v17 = (const CHAR *)&unk_4E7454;
    if ( v16 )
      v17 = "Unable to output install info.";
    sub_46BC80(0, *(HWND *)(a6 + 16), v17, 0x10u);
  }
  v18 = (char **)operator new(0xCu);
  *v18 = (char *)operator new[](0xBu);
  v19 = (char *)operator new[](strlen(Buffer) + 1);
  v20 = v29;
  v18[1] = v19;
  v21 = *v18;
  v18[2] = *(char **)(v20 + 8);
  strcpy(v21, "uninst.lst");
  v22 = v18[1];
  v23 = Buffer;
  do
  {
    v24 = *v23;
    *v22++ = *v23++;
  }
  while ( v24 );
  *(_DWORD *)(v20 + 8) = v18;
  v34 = -1;
  sub_42D400(NumberOfBytesRead);
  return v14;
}

// ===== sub_4712C0 @ 0x004712C0..0x004713E8 =====
BOOL __cdecl sub_4712C0(const CHAR *a1, int a2)
{
  const CHAR *v2; // ecx
  const CHAR *v3; // edi
  BOOL v4; // esi
  int v6; // [esp+10h] [ebp-214h] BYREF
  LPVOID ppv; // [esp+14h] [ebp-210h] BYREF
  WCHAR WideCharStr[260]; // [esp+18h] [ebp-20Ch] BYREF

  v3 = v2;
  v4 = 0;
  ppv = 0;
  if ( !CoCreateInstance(&stru_4DC308, 0, 1u, &stru_4DC2D8, &ppv) )
  {
    if ( !(*(int (__stdcall **)(LPVOID, int))(*(_DWORD *)ppv + 80))(ppv, a2) )
    {
      if ( !v3 )
        v3 = MultiByteStr;
      if ( !(*(int (__stdcall **)(LPVOID, const CHAR *))(*(_DWORD *)ppv + 44))(ppv, v3)
        && !(*(int (__stdcall **)(LPVOID, const CHAR *))(*(_DWORD *)ppv + 36))(ppv, MultiByteStr) )
      {
        v6 = 0;
        if ( !(**(int (__stdcall ***)(LPVOID, void *, int *))ppv)(ppv, &unk_4DC328, &v6) )
        {
          MultiByteToWideChar(0, 1u, a1, -1, WideCharStr, 260);
          v4 = (*(int (__stdcall **)(int, WCHAR *, int))(*(_DWORD *)v6 + 24))(v6, WideCharStr, 1) == 0;
          (*(void (__stdcall **)(int))(*(_DWORD *)v6 + 8))(v6);
        }
      }
    }
    (*(void (__stdcall **)(LPVOID))(*(_DWORD *)ppv + 8))(ppv);
  }
  return v4;
}

// ===== sub_4713F0 @ 0x004713F0..0x00471530 =====
BOOL __usercall sub_4713F0@<eax>(const char *a1@<esi>, const char *a2, int a3)
{
  BOOL v3; // edi
  void **v4; // ebx
  char Buffer[780]; // [esp+14h] [ebp-928h] BYREF
  char v7[780]; // [esp+320h] [ebp-61Ch] BYREF
  CHAR pszPath[780]; // [esp+62Ch] [ebp-310h] BYREF

  v3 = 0;
  if ( a1 )
  {
    v4 = (void **)operator new(0xCu);
    *v4 = 0;
    v4[1] = 0;
    v4[2] = 0;
    sub_467570((BYTE *)pszPath);
    sprintf(Buffer, "%s\\%s", pszPath, a1);
    if ( sub_46FB90(v4) )
    {
      sprintf(v7, "%s\\%s", Buffer, a2);
      v3 = sub_4712C0(v7, a3);
      if ( !v3 )
        sub_46FD80((int)v4);
    }
    sub_46FDF0(v4);
    return v3;
  }
  else
  {
    sub_467570((BYTE *)pszPath);
    sprintf(v7, "%s\\%s", pszPath, a2);
    return sub_4712C0(v7, a3);
  }
}

// ===== sub_471530 @ 0x00471530..0x004718CB =====
int __fastcall sub_471530(
        const char *a1,
        const char *a2,
        const char *a3,
        const char *a4,
        const char *a5,
        const char *a6,
        int a7,
        int a8)
{
  _DWORD *v8; // esi
  void *v9; // eax
  _BYTE *v10; // edx
  const char *v11; // ecx
  char v12; // al
  _BYTE *v13; // edx
  char *v14; // ecx
  char v15; // al
  void **v16; // esi
  _DWORD *v17; // esi
  _BYTE *v18; // edx
  const char *v19; // ecx
  char v20; // al
  _BYTE *v21; // edx
  char *v22; // ecx
  char v23; // al
  _DWORD *v24; // esi
  void *v25; // eax
  _BYTE *v26; // edx
  const char *v27; // ecx
  char v28; // al
  _BYTE *v29; // edx
  char *v30; // ecx
  char v31; // al
  void **v35; // [esp+1Ch] [ebp-F48h]
  int v36; // [esp+20h] [ebp-F44h]
  CHAR v37[780]; // [esp+24h] [ebp-F40h] BYREF
  CHAR pszPath[780]; // [esp+330h] [ebp-C34h] BYREF
  char v39[780]; // [esp+63Ch] [ebp-928h] BYREF
  char v40[780]; // [esp+948h] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+C54h] [ebp-310h] BYREF

  v36 = 1;
  sub_467570((BYTE *)pszPath);
  sub_467570((BYTE *)v37);
  dword_518914 = 0;
  dword_518918 = 0;
  dword_51891C = 0;
  if ( a8 )
  {
    sprintf(Buffer, "%s\\%s", pszPath, a5);
    sprintf(v40, "%s\\%s", a3, a4);
    if ( sub_4712C0(Buffer, (int)v40) )
    {
      v8 = operator new(0xCu);
      *v8 = operator new[](strlen(a5) + 1);
      v9 = operator new[](strlen(Buffer) + 1);
      v10 = (_BYTE *)*v8;
      v8[1] = v9;
      v8[2] = dword_51891C;
      v11 = a5;
      do
      {
        v12 = *v11;
        *v10++ = *v11++;
      }
      while ( v12 );
      v13 = (_BYTE *)v8[1];
      v14 = Buffer;
      do
      {
        v15 = *v14;
        *v13++ = *v14++;
      }
      while ( v15 );
      dword_51891C = (int)v8;
    }
  }
  v16 = (void **)operator new(0xCu);
  v35 = v16;
  *v16 = 0;
  v16[1] = 0;
  v16[2] = 0;
  if ( a7 )
  {
    v36 = 0;
    sprintf(v39, "%s\\%s", v37, a2);
    if ( sub_46FB90(v16) )
    {
      sprintf(Buffer, "%s\\%s", v39, a5);
      sprintf(v40, "%s\\%s", a3, a4);
      if ( sub_4712C0(Buffer, (int)v40) )
      {
        v17 = operator new(0xCu);
        *v17 = operator new[](strlen(a5) + 1);
        v17[1] = operator new[](strlen(Buffer) + 1);
        v17[2] = dword_51891C;
        v18 = (_BYTE *)*v17;
        v19 = a5;
        do
        {
          v20 = *v19;
          *v18++ = *v19++;
        }
        while ( v20 );
        v21 = (_BYTE *)v17[1];
        v22 = Buffer;
        do
        {
          v23 = *v22;
          *v21++ = *v22++;
        }
        while ( v23 );
        dword_51891C = (int)v17;
        sprintf(Buffer, "%s\\%s", v39, a1);
        sprintf(v40, "%s\\%s", a3, a6);
        if ( sub_4712C0(Buffer, (int)v40) )
        {
          v24 = operator new(0xCu);
          *v24 = operator new[](strlen(a1) + 1);
          v25 = operator new[](strlen(Buffer) + 1);
          v26 = (_BYTE *)*v24;
          v24[1] = v25;
          v24[2] = dword_51891C;
          v27 = a1;
          do
          {
            v28 = *v27;
            *v26++ = *v27++;
          }
          while ( v28 );
          v29 = (_BYTE *)v24[1];
          v30 = Buffer;
          do
          {
            v31 = *v30;
            *v29++ = *v30++;
          }
          while ( v31 );
          dword_51891C = (int)v24;
          v16 = v35;
          v36 = 1;
          goto LABEL_23;
        }
        v16 = v35;
      }
    }
    sub_470B50((int)&dword_518914);
    sub_46FD80((int)v16);
  }
LABEL_23:
  sub_470B70((int)&dword_518914);
  sub_46FDF0(v16);
  return v36;
}

// ===== sub_4718D0 @ 0x004718D0..0x004719CA =====
BOOL __usercall sub_4718D0@<eax>(const char *a1@<edi>, const char *a2@<esi>, const char *a3, int a4)
{
  BOOL result; // eax
  CHAR pszPath[780]; // [esp+8h] [ebp-928h] BYREF
  CHAR v6[780]; // [esp+314h] [ebp-61Ch] BYREF
  CHAR FileName[780]; // [esp+620h] [ebp-310h] BYREF

  sub_467570((BYTE *)pszPath);
  sub_467570((BYTE *)v6);
  sprintf(FileName, "%s\\%s", pszPath, a1);
  DeleteFileA(FileName);
  sprintf(FileName, "%s\\%s\\%s", v6, a2, a1);
  DeleteFileA(FileName);
  sprintf(FileName, "%s\\%s\\%s", v6, a2, a3);
  result = DeleteFileA(FileName);
  if ( a4 )
  {
    sprintf(FileName, "%s\\%s", v6, a2);
    return RemoveDirectoryA(FileName);
  }
  return result;
}

// ===== sub_4719D0 @ 0x004719D0..0x00471A98 =====
int __usercall sub_4719D0@<eax>(const char *a1@<ecx>, BYTE *a2@<edi>, const char *a3)
{
  int v3; // esi
  DWORD Type; // [esp+4h] [ebp-1010h] BYREF
  DWORD cbData; // [esp+8h] [ebp-100Ch] BYREF
  HKEY phkResult; // [esp+Ch] [ebp-1008h] BYREF
  CHAR SubKey[4096]; // [esp+10h] [ebp-1004h] BYREF

  v3 = 0;
  cbData = 780;
  if ( a3 )
  {
    if ( a1 )
    {
      sprintf(SubKey, "%s\\%s\\%s", "Software", a3, a1);
      if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, SubKey, 0, 0x20019u, &phkResult) )
      {
        if ( RegQueryValueExA(phkResult, "InstalledFolder", 0, &Type, a2, &cbData) || (v3 = 1, Type != 1) )
          v3 = 0;
        RegCloseKey(phkResult);
      }
    }
  }
  return v3;
}

// ===== sub_471AA0 @ 0x00471AA0..0x00471B9C =====
int __usercall sub_471AA0@<eax>(const char *a1@<ecx>, char *a2@<edi>)
{
  int result; // eax
  DWORD v4; // ebx
  DWORD NumberOfBytesRead[3]; // [esp+Ch] [ebp-638h] BYREF
  int v6; // [esp+18h] [ebp-62Ch]
  char Buffer[780]; // [esp+1Ch] [ebp-628h] BYREF
  CHAR pszPath[780]; // [esp+328h] [ebp-31Ch] BYREF
  int v9; // [esp+640h] [ebp-4h]

  result = 0;
  v6 = 0;
  if ( a1 )
  {
    sub_467570((BYTE *)pszPath);
    sprintf(Buffer, "%s\\%s", pszPath, a1);
    sub_42D3B0(NumberOfBytesRead);
    v9 = 0;
    if ( sub_42D520(Buffer, (int)NumberOfBytesRead) )
    {
      v4 = sub_42D650((int)NumberOfBytesRead);
      sub_42D5D0(v4, a2, (DWORD)NumberOfBytesRead);
      sub_42D5B0((int)NumberOfBytesRead);
      if ( a2[v4 - 2] == 92 && !a2[v4 - 1] )
      {
        a2[v4 - 2] = 0;
        v6 = 1;
      }
    }
    v9 = -1;
    sub_42D400(NumberOfBytesRead);
    return v6;
  }
  return result;
}

// ===== sub_471BA0 @ 0x00471BA0..0x00471C01 =====
BOOL __fastcall sub_471BA0(const char *a1, const char *a2)
{
  BOOL result; // eax
  CHAR SubKey[4096]; // [esp+0h] [ebp-1004h] BYREF

  result = 0;
  if ( a1 )
  {
    if ( a2 )
    {
      sprintf(SubKey, "%s\\%s\\%s", "Software", a1, a2);
      return RegDeleteKeyA(HKEY_LOCAL_MACHINE, SubKey) == 0;
    }
  }
  return result;
}

// ===== sub_471C10 @ 0x00471C10..0x00471E0B =====
int __cdecl sub_471C10(const char *a1, const char **a2)
{
  int v2; // esi
  DWORD v3; // edi
  char *v4; // eax
  char *v5; // edi
  char v6; // al
  char *i; // ecx
  const char *v8; // ecx
  int j; // esi
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-640h] BYREF
  const char *v12; // [esp+1Ch] [ebp-634h]
  int v13; // [esp+20h] [ebp-630h]
  void *v14; // [esp+24h] [ebp-62Ch]
  CHAR FileName[780]; // [esp+28h] [ebp-628h] BYREF
  char v16[780]; // [esp+334h] [ebp-31Ch] BYREF
  int v17; // [esp+64Ch] [ebp-4h]

  v12 = a1;
  sprintf(FileName, "%s\\%s", a1, "uninst.lst");
  sub_42D3B0(NumberOfBytesRead);
  v17 = 0;
  v2 = sub_42D520(FileName, (int)NumberOfBytesRead);
  v13 = v2;
  if ( v2 )
  {
    v3 = sub_42D650((int)NumberOfBytesRead);
    v14 = operator new[](v3 + 1);
    sub_42D5D0(v3, v14, (DWORD)NumberOfBytesRead);
    sub_42D5B0((int)NumberOfBytesRead);
    v4 = (char *)v14;
    *((_BYTE *)v14 + v3) = 0;
    v5 = v4;
    if ( *v4 )
    {
      do
      {
        v6 = *v5;
        for ( i = v16; *v5 != 10; ++i )
        {
          ++v5;
          *i = v6;
          v6 = *v5;
        }
        *i = 0;
        ++v5;
        if ( v16[0] != 64 && strcmp(v16, "uninst.lst") )
        {
          v8 = *a2;
          for ( j = 0; v8; v8 = a2[++j] )
          {
            if ( !strcmp(v16, v8) )
              break;
          }
          if ( !a2[j] )
          {
            sprintf(FileName, "%s\\%s", v12, v16);
            DeleteFileA(FileName);
          }
        }
      }
      while ( *v5 );
      v4 = (char *)v14;
    }
    operator delete[](v4);
    v2 = v13;
  }
  v17 = -1;
  sub_42D400(NumberOfBytesRead);
  return v2;
}

// ===== sub_471E10 @ 0x00471E10..0x00471FA8 =====
BOOL __fastcall sub_471E10(int a1, const char **a2)
{
  int v3; // edx
  const char *v4; // ecx
  char *v5; // esi
  DWORD v6; // ebx
  const char *v7; // eax
  char *i; // esi
  BOOL v9; // eax
  char *v10; // edi
  unsigned int v11; // kr04_4
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-330h] BYREF
  BOOL v14; // [esp+1Ch] [ebp-324h]
  char *Str; // [esp+20h] [ebp-320h]
  char Buffer[780]; // [esp+24h] [ebp-31Ch] BYREF
  int v17; // [esp+33Ch] [ebp-4h]

  v14 = 0;
  sub_42D3B0(NumberOfBytesRead);
  v17 = v3;
  sprintf(Buffer, "%s\\%s", v4, "uninst.lst");
  if ( sub_42D520(Buffer, (int)NumberOfBytesRead) )
  {
    v5 = (char *)operator new[](0x100000u);
    Str = v5;
    v6 = sub_42D650((int)NumberOfBytesRead);
    sub_42D5D0(v6, v5, (DWORD)NumberOfBytesRead);
    sub_42D5B0((int)NumberOfBytesRead);
    v7 = *a2;
    for ( i = &Str[v6 - 1]; v7; ++a2 )
    {
      if ( !strstr(Str, v7) )
      {
        sprintf(i, "%s\n", *a2);
        i += strlen(i);
      }
      v7 = a2[1];
    }
    v9 = sub_42D570(Buffer, (int)NumberOfBytesRead, 0);
    v10 = Str;
    if ( v9 )
    {
      v11 = strlen(Str);
      v14 = sub_42D600(v11 + 1, Str, (DWORD)NumberOfBytesRead) == v11 + 1;
      sub_42D5B0((int)NumberOfBytesRead);
    }
    operator delete[](v10);
  }
  v17 = -1;
  sub_42D400(NumberOfBytesRead);
  return v14;
}

// ===== sub_471FB0 @ 0x00471FB0..0x00472049 =====
INT_PTR __usercall sub_471FB0@<eax>(
        char *a1@<eax>,
        char *a2,
        _DWORD *a3,
        _DWORD *a4,
        WPARAM a5,
        WPARAM a6,
        int a7,
        const CHAR *a8)
{
  CHAR *v8; // ecx
  char v9; // dl
  BOOL v10; // eax
  INT_PTR result; // eax

  v8 = (CHAR *)(byte_51A630 - a1);
  do
  {
    v9 = *a1;
    a1[(_DWORD)v8] = *a1;
    ++a1;
  }
  while ( v9 );
  dword_566868 = a5;
  dword_56686C = a6;
  dword_566864 = a7;
  dword_566870 = a8;
  v10 = sub_46F710();
  result = DialogBoxParamA(hInst, (LPCSTR)(v10 ? 108 : 122), hWndParent, sub_472CC0, 0);
  if ( result )
  {
    strcpy(a2, byte_51A630);
    *a3 = dword_566868;
    *a4 = dword_56686C;
  }
  return result;
}

// ===== sub_472050 @ 0x00472050..0x004720AA =====
INT_PTR __usercall sub_472050@<eax>(
        const CHAR *a1@<eax>,
        const CHAR *a2@<ecx>,
        const CHAR *a3,
        const CHAR *a4,
        int a5,
        int a6)
{
  dword_56687C = a3;
  dword_506700 = a5;
  dword_566874 = a2;
  dword_566880 = a4;
  dword_566884 = a6;
  dword_566878 = a1;
  return DialogBoxParamA(hInst, (LPCSTR)((a1 != 0) + 109), hWndParent, sub_473060, 0);
}

// ===== sub_4720B0 @ 0x004720B0..0x0047214C =====
int __usercall sub_4720B0@<eax>(const char *a1@<ecx>, const BYTE *a2@<esi>, const char *a3)
{
  DWORD dwDisposition; // [esp+0h] [ebp-100Ch] BYREF
  HKEY phkResult; // [esp+4h] [ebp-1008h] BYREF
  CHAR SubKey[4096]; // [esp+8h] [ebp-1004h] BYREF

  sprintf(SubKey, "%s\\%s\\%s", "Software", a3, a1);
  RegCreateKeyExA(HKEY_LOCAL_MACHINE, SubKey, 0, 0, 0, 0x20006u, 0, &phkResult, &dwDisposition);
  RegSetValueExA(phkResult, "InstalledFolder", 0, 1u, a2, strlen((const char *)a2) + 1);
  return 1;
}

// ===== sub_472150 @ 0x00472150..0x0047248B =====
int __fastcall sub_472150(
        const char *a1,
        char *a2,
        const char *a3,
        const char **a4,
        _DWORD *a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        char *a11,
        const char *a12,
        const CHAR *a13,
        int a14)
{
  int v14; // ecx
  void **v15; // edi
  int v16; // eax
  unsigned __int8 v17; // cl
  int v18; // eax
  unsigned __int8 v19; // cl
  unsigned __int8 *v20; // eax
  unsigned __int8 *v21; // edx
  unsigned __int8 v22; // cl
  LPARAM v23; // eax
  BOOL v24; // eax
  HWND v26; // [esp-10h] [ebp-650h]
  int v29; // [esp+20h] [ebp-620h]
  char Buffer[780]; // [esp+24h] [ebp-61Ch] BYREF
  unsigned __int8 String; // [esp+330h] [ebp-310h] BYREF
  char v32; // [esp+331h] [ebp-30Fh]
  char v33; // [esp+332h] [ebp-30Eh]

  v29 = 0;
  if ( __uncaught_exception() )
    return v14;
  strcpy(byte_51A630, a3);
  dword_566848 = a6;
  dword_56684C = a7;
  dword_566850 = a8;
  dword_566854 = a9;
  dword_566888 = a14;
  v15 = (void **)operator new(0xCu);
  *v15 = 0;
  v15[1] = 0;
  v15[2] = 0;
  if ( sub_46FB90(v15) )
  {
    if ( !sub_46FD10(a4, v15, a3) )
      goto LABEL_30;
    v16 = 0;
    dword_518914 = 0;
    dword_518918 = 0;
    dword_51891C = 0;
    do
    {
      v17 = *(&::Buffer + v16);
      *(&String + v16++) = v17;
    }
    while ( v17 );
    _mbslwr(&String);
    if ( (unsigned __int8)(String - 97) <= 0x19u && v32 == 58 && v33 == 92 )
    {
      v18 = 0;
      do
      {
        v19 = byte_517F1B[v18];
        *(&String + v18++) = v19;
      }
      while ( v19 );
    }
    else if ( String == 92 && v32 == 92 && v33 != 63 )
    {
      v20 = _mbschr(&byte_517F1A, 0x5Cu) + 1;
      v21 = (unsigned __int8 *)(&String - v20);
      do
      {
        v22 = *v20;
        v20[(_DWORD)v21] = *v20;
        ++v20;
      }
      while ( v22 );
    }
    else
    {
      String = 0;
    }
    v23 = 0;
    if ( *a5 )
    {
      do
        ++v23;
      while ( a5[v23] );
    }
    dword_518C8C = v23;
    dword_518C84 = (int)a3;
    dword_518C88 = (int)&String;
    dword_518C90 = (int)a5;
    dword_518C94 = 0;
    dword_56685C = 0;
    dword_566860 = 0;
    sprintf(Buffer, "%s%s", &::Buffer, "BGI.hvl");
    sub_401A50();
    v26 = hWndParent;
    v24 = sub_46F710();
    if ( DialogBoxParamA(hInst, (LPCSTR)(v24 ? 111 : 123), v26, sub_473210, 0) )
    {
      if ( a12 )
      {
        dword_518C94 = (int)hWndParent;
        if ( sub_470F70(a1, a4, (int)&dword_518914, a12, a10, (int)&dword_518C84)
          && sub_470BB0(a11, a13, (int)&dword_518C84, (BYTE *)a2, a12) )
        {
          v29 = sub_4720B0(a11, (const BYTE *)dword_518C84, a2);
        }
      }
      else
      {
        v29 = 1;
      }
    }
    if ( dword_566860 )
      operator delete[](dword_566860);
    if ( !v29 )
    {
LABEL_30:
      sub_470B50((int)&dword_518914);
      sub_46FD80((int)v15);
    }
    sub_470B70((int)&dword_518914);
  }
  else
  {
    sub_46FD80((int)v15);
  }
  sub_46FDF0(v15);
  return v29;
}

// ===== sub_472490 @ 0x00472490..0x004724A4 =====
int __usercall sub_472490@<eax>(const char *a1@<eax>)
{
  return sprintf(byte_51A530, "Uninstaller for %s is executing.", a1);
}

// ===== sub_4724B0 @ 0x004724B0..0x0047287F =====
int __fastcall sub_4724B0(
        const CHAR *a1,
        const CHAR *a2,
        DWORD *a3,
        const char *a4,
        const char *a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  HMODULE ModuleHandleA; // eax
  int v11; // eax
  char v12; // cl
  const CHAR *v13; // eax
  int v14; // edi
  const CHAR *v15; // eax
  bool v16; // zf
  const CHAR *v17; // eax
  int v18; // eax
  DWORD v19; // esi
  HANDLE v20; // eax
  HWND hObject; // [esp+24h] [ebp-13A4h]
  BOOL (__stdcall *CreateProcessWithTokenW)(HANDLE, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION); // [esp+28h] [ebp-13A0h]
  int v25; // [esp+2Ch] [ebp-139Ch]
  _PROCESS_INFORMATION ProcessInformation; // [esp+34h] [ebp-1394h] BYREF
  LPDWORD lpExitCode; // [esp+44h] [ebp-1384h]
  _DWORD v29[18]; // [esp+48h] [ebp-1380h] BYREF
  _STARTUPINFOA StartupInfo; // [esp+90h] [ebp-1338h] BYREF
  _DWORD v31[38]; // [esp+D8h] [ebp-12F0h] BYREF
  char v32[784]; // [esp+170h] [ebp-1258h] BYREF
  CHAR MultiByteStr[784]; // [esp+480h] [ebp-F48h] BYREF
  WCHAR v34[780]; // [esp+790h] [ebp-C38h] BYREF
  WCHAR WideCharStr[782]; // [esp+DA8h] [ebp-620h] BYREF

  lpExitCode = a3;
  hObject = 0;
  if ( sub_467080() )
    hObject = sub_4670E0();
  CreateProcessWithTokenW = 0;
  if ( hObject )
  {
    sub_46F800((struct _OSVERSIONINFOA *)v31);
    if ( v31[1] >= 6u )
    {
      ModuleHandleA = GetModuleHandleA("advapi32.dll");
      CreateProcessWithTokenW = (BOOL (__stdcall *)(HANDLE, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION))GetProcAddress(ModuleHandleA, "CreateProcessWithTokenW");
    }
  }
  if ( a4 )
  {
    sprintf(MultiByteStr, "%s\\%s", a4, a5);
    strcpy(v32, a4);
  }
  else
  {
    sprintf(MultiByteStr, "%s%s", &Buffer, a5);
    v11 = 0;
    do
    {
      v12 = *(&Buffer + v11);
      v32[v11++] = v12;
    }
    while ( v12 );
    *((_BYTE *)&v31[37] + strlen(&Buffer) + 3) = 0;
  }
  if ( CreateProcessWithTokenW )
  {
    MultiByteToWideChar(0, 0, MultiByteStr, -1, WideCharStr, 780);
    v13 = a1;
    if ( !a1 )
      v13 = v32;
    MultiByteToWideChar(0, 0, v13, -1, v34, 780);
  }
  memset(&StartupInfo, 0, sizeof(StartupInfo));
  StartupInfo.cb = 68;
  StartupInfo.dwFlags = 1;
  StartupInfo.wShowWindow = a6 != 0 ? 5 : 0;
  memset(v29, 0, 0x44u);
  v29[0] = 68;
  LOWORD(v29[12]) = StartupInfo.wShowWindow;
  v29[11] = 1;
  memset(&ProcessInformation, 0, sizeof(ProcessInformation));
  v14 = 0;
  while ( 1 )
  {
    if ( sub_464B80(MultiByteStr) )
    {
      if ( CreateProcessWithTokenW
        && CreateProcessWithTokenW(hObject, 0, 0, WideCharStr, 0, 0, v34, (LPSTARTUPINFOW)v29, &ProcessInformation) )
      {
        break;
      }
      v15 = a1;
      if ( !a1 )
        v15 = v32;
      if ( CreateProcessA(0, MultiByteStr, 0, 0, 0, 0, 0, v15, &StartupInfo, &ProcessInformation) )
        break;
    }
    if ( !a2 )
      goto LABEL_45;
    if ( !a9 )
    {
      sub_46BC80(0, hWndParent, a2, 0x40u);
      goto LABEL_45;
    }
    if ( sub_46BC80(0, hWndParent, a2, 0x41u) != 1 )
    {
      v16 = !sub_46F710();
      v17 = (const CHAR *)&unk_4E6DC4;
      if ( v16 )
        v17 = "Are you sure you want to quit?";
      if ( sub_46BC80(0, hWndParent, v17, 0x124u) == 6 )
        goto LABEL_45;
    }
  }
  if ( a8 )
  {
    v18 = sub_49A220();
    v25 = v18;
    if ( a7 && v18 )
      sub_49A170();
    WaitForInputIdle(ProcessInformation.hProcess, 0xFFFFFFFF);
    do
    {
      v19 = WaitForSingleObject(ProcessInformation.hProcess, 8u);
      sub_49A060();
    }
    while ( v19 == 258 );
    if ( lpExitCode )
      GetExitCodeProcess(ProcessInformation.hProcess, lpExitCode);
    CloseHandle(ProcessInformation.hThread);
    CloseHandle(ProcessInformation.hProcess);
    if ( a10 )
    {
      while ( 1 )
      {
        v20 = OpenMutexA(0x1F0001u, 0, byte_51A530);
        if ( !v20 )
          break;
        CloseHandle(v20);
        Sleep(0x64u);
      }
    }
    if ( a7 && v25 )
      sub_49A170();
  }
  v14 = 1;
LABEL_45:
  if ( hObject )
    CloseHandle(hObject);
  return v14;
}

// ===== sub_472880 @ 0x00472880..0x0047289B =====
BOOL __usercall sub_472880@<eax>(const CHAR *a1@<eax>)
{
  return (unsigned int)ShellExecuteA(0, "open", a1, 0, 0, 1) >= 0x20;
}

// ===== sub_4728A0 @ 0x004728A0..0x004728A6 =====
int sub_4728A0()
{
  return dword_5666DC;
}

// ===== sub_4728B0 @ 0x004728B0..0x00472B52 =====
int __fastcall sub_4728B0(const BYTE *a1, const BYTE *a2, const char *a3, BYTE *lpData, BYTE *a5)
{
  DWORD dwDisposition; // [esp+14h] [ebp-100Ch] BYREF
  HKEY phkResult; // [esp+18h] [ebp-1008h] BYREF
  CHAR SubKey[4096]; // [esp+1Ch] [ebp-1004h] BYREF

  sprintf(SubKey, ".%s", a3);
  if ( RegCreateKeyExA(HKEY_CLASSES_ROOT, SubKey, 0, SubKey, 0, 0x20006u, 0, &phkResult, &dwDisposition) )
    return 0;
  if ( RegSetValueExA(phkResult, 0, 0, 1u, lpData, strlen((const char *)lpData) + 1) )
    return 0;
  if ( RegCreateKeyExA(HKEY_CLASSES_ROOT, (LPCSTR)lpData, 0, (LPSTR)lpData, 0, 0x20006u, 0, &phkResult, &dwDisposition) )
    return 0;
  if ( RegSetValueExA(phkResult, 0, 0, 1u, a5, strlen((const char *)a5) + 1) )
    return 0;
  sprintf(SubKey, "%s\\DefaultIcon", (const char *)lpData);
  if ( RegCreateKeyExA(HKEY_CLASSES_ROOT, SubKey, 0, "DefaultIcon", 0, 0x20006u, 0, &phkResult, &dwDisposition) )
    return 0;
  if ( RegSetValueExA(phkResult, 0, 0, 1u, a1, strlen((const char *)a1) + 1) )
    return 0;
  sprintf(SubKey, "%s\\Shell", (const char *)lpData);
  if ( RegCreateKeyExA(HKEY_CLASSES_ROOT, SubKey, 0, "Shell", 0, 0x20006u, 0, &phkResult, &dwDisposition)
    || RegCreateKeyExA(phkResult, "Open", 0, "Open", 0, 0x20006u, 0, &phkResult, &dwDisposition)
    || RegCreateKeyExA(phkResult, "Command", 0, "Command", 0, 0x20006u, 0, &phkResult, &dwDisposition)
    || RegSetValueExA(phkResult, 0, 0, 1u, a2, strlen((const char *)a2) + 1) )
  {
    return 0;
  }
  PostMessageA(HWND_BROADCAST, 0x1Au, 0x2Eu, 0);
  SHChangeNotify(0x8000000, 0x3000u, 0, 0);
  return 1;
}

// ===== sub_472B60 @ 0x00472B60..0x00472B81 =====
int sub_472B60()
{
  int v0; // esi

  EnterCriticalSection(&stru_51A940);
  v0 = dword_506704;
  LeaveCriticalSection(&stru_51A940);
  return v0;
}

// ===== sub_472B90 @ 0x00472B90..0x00472BF8 =====
BOOL __usercall sub_472B90@<eax>(HWND a1@<esi>)
{
  int SystemMetrics; // eax
  int v3; // [esp-10h] [ebp-28h]
  struct tagRECT Rect; // [esp+4h] [ebp-14h] BYREF

  GetWindowRect(a1, &Rect);
  v3 = (GetSystemMetrics(1) + Rect.top - Rect.bottom - 1) / 2;
  SystemMetrics = GetSystemMetrics(0);
  return SetWindowPos(a1, 0, (SystemMetrics + Rect.left - Rect.right - 1) / 2, v3, 0, 0, 0x21u);
}

// ===== sub_472C00 @ 0x00472C00..0x00472C38 =====
int __stdcall sub_472C00(HWND hWnd, int a2, int a3, LPARAM lParam)
{
  if ( a2 != 1 )
    return 0;
  sub_472B90(hWnd);
  if ( lParam )
    SendMessageA(hWnd, 0x466u, 1u, lParam);
  return 1;
}

// ===== sub_472C40 @ 0x00472C40..0x00472CB8 =====
BOOL __thiscall sub_472C40(char *this)
{
  char v1; // bl
  BOOL result; // eax
  CHAR RootPathName[4]; // [esp+4h] [ebp-310h] BYREF
  _BYTE v4[776]; // [esp+8h] [ebp-30Ch] BYREF

  v1 = *this;
  result = 0;
  if ( *this >= 65 && v1 <= 90 || v1 >= 97 && v1 <= 122 )
  {
    strcpy(RootPathName, "A:\\");
    memset(v4, 0, sizeof(v4));
    RootPathName[0] = v1;
    return GetDriveTypeA(RootPathName) == 3;
  }
  return result;
}

// ===== sub_472CC0 @ 0x00472CC0..0x00473058 =====
INT_PTR __stdcall sub_472CC0(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  int v5; // eax
  CHAR v6; // cl
  void **v7; // edi
  int v8; // ebx
  bool v9; // zf
  const CHAR *v10; // eax
  int v11; // eax
  CHAR v12; // cl
  CHAR *v13; // eax
  CHAR String[1560]; // [esp+4h] [ebp-C34h] BYREF
  CHAR pszPath; // [esp+61Ch] [ebp-61Ch] BYREF
  _BYTE v16[3]; // [esp+61Dh] [ebp-61Bh] BYREF

  if ( a2 != 16 )
  {
    if ( a2 == 272 )
    {
      sub_472B90(hDlg);
      SetDlgItemTextA(hDlg, 1000, byte_51A630);
      SendDlgItemMessageA(hDlg, 1002, 0xF1u, dword_566868, 0);
      SendDlgItemMessageA(hDlg, 1003, 0xF1u, dword_56686C, 0);
      if ( dword_566870 )
        SetDlgItemTextA(hDlg, 1004, dword_566870);
      return 1;
    }
    if ( a2 != 273 )
      return 0;
    if ( (unsigned __int16)a3 == 1001 )
    {
      if ( !sub_46F9C0(0, hDlg, &pszPath, 0) )
        return 1;
      if ( strlen(&pszPath) > 0x2BC || !sub_46FA50(&pszPath) )
      {
        v9 = !sub_46F710();
        v10 = (const CHAR *)&unk_4E75A8;
LABEL_31:
        if ( v9 )
          v10 = "An illegal path was selected.";
        goto LABEL_33;
      }
      if ( sub_472C40(&pszPath) )
      {
        v11 = &v16[strlen(&pszPath)] - v16;
        v9 = String[v11 + 1559] == 92;
        v13 = &String[v11 + 1559];
        if ( v9 )
          *v13 = v12;
        sprintf(String, "%s\\%s", &pszPath, (const char *)dword_566864);
        SetDlgItemTextA(hDlg, 1000, String);
        return 1;
      }
      v9 = !sub_46F710();
      v10 = (const CHAR *)&unk_4E7554;
      if ( v9 )
      {
        sub_46BC80(0, hDlg, "Non-stationary device was selected.", 0x30u);
        return 1;
      }
    }
    else
    {
      if ( (unsigned __int16)a3 != 1004 )
      {
        if ( (unsigned __int16)a3 == 1005 )
          EndDialog(hDlg, 0);
        return 1;
      }
      GetDlgItemTextA(hDlg, 1000, byte_51A630, 780);
      byte_51A93C = 0;
      if ( !sub_46FA50(byte_51A630) )
      {
LABEL_18:
        v9 = !sub_46F710();
        v10 = (const CHAR *)&unk_4E75E8;
        goto LABEL_31;
      }
      if ( sub_472C40(byte_51A630) )
      {
        if ( strlen(byte_51A630) <= 0x2BC )
        {
          sprintf(String, "%c:\\", byte_51A630[0]);
          if ( !strcmp(String, byte_51A630) )
          {
            sprintf(String, "%s%s", byte_51A630, (const char *)dword_566864);
            v5 = 0;
            do
            {
              v6 = String[v5];
              byte_51A630[v5++] = v6;
            }
            while ( v6 );
          }
          v7 = (void **)operator new(0xCu);
          *v7 = 0;
          v7[1] = 0;
          v7[2] = 0;
          v8 = sub_46FB90(v7);
          sub_46FD80((int)v7);
          sub_46FDF0(v7);
          if ( v8 )
          {
            dword_566868 = IsDlgButtonChecked(hDlg, 1002);
            dword_56686C = IsDlgButtonChecked(hDlg, 1003);
            EndDialog(hDlg, 1);
            return 1;
          }
        }
        goto LABEL_18;
      }
      v9 = !sub_46F710();
      v10 = (const CHAR *)&unk_4E7608;
      if ( v9 )
      {
        sub_46BC80(0, hDlg, "Please choose a stationary harddrive.", 0x30u);
        return 1;
      }
    }
LABEL_33:
    sub_46BC80(0, hDlg, v10, 0x30u);
    return 1;
  }
  EndDialog(hDlg, 0);
  return 1;
}

// ===== sub_473060 @ 0x00473060..0x0047320E =====
INT_PTR __stdcall sub_473060(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  INT_PTR i; // edi
  int v6; // eax
  HWND DlgItem; // eax
  HWND v8; // eax
  int v9; // [esp-8h] [ebp-20h]
  int nIDButton; // [esp+8h] [ebp-10h]
  int v11; // [esp+Ch] [ebp-Ch]
  int v12; // [esp+10h] [ebp-8h]

  nIDButton = 1007;
  v11 = 1008;
  v12 = 1009;
  if ( a2 == 16 )
  {
    EndDialog(hDlg, -1);
    return 1;
  }
  else
  {
    if ( a2 == 272 )
    {
      sub_472B90(hDlg);
      SetDlgItemTextA(hDlg, 1006, dword_566874);
      if ( dword_566878 )
        SetDlgItemTextA(hDlg, 1007, dword_566878);
      SetDlgItemTextA(hDlg, 1008, dword_56687C);
      SetDlgItemTextA(hDlg, 1009, dword_566880);
      v6 = dword_506700;
      nIDButton = 1007;
      v11 = 1008;
      v12 = 1009;
      if ( (unsigned int)dword_506700 > 2 )
        goto LABEL_22;
      if ( dword_566878 || dword_506700 > 0 )
      {
        DlgItem = GetDlgItem(hDlg, *(&nIDButton + dword_506700));
        EnableWindow(DlgItem, 0);
        v6 = dword_506700;
      }
      if ( v6 == 2 )
        SendDlgItemMessageA(hDlg, 1008, 0xF1u, 1u, 0);
      else
LABEL_22:
        SendDlgItemMessageA(hDlg, 1009, 0xF1u, 1u, 0);
      v9 = dword_566884 != 0 ? 5 : 0;
      v8 = GetDlgItem(hDlg, 1010);
      ShowWindow(v8, v9);
    }
    else
    {
      if ( a2 != 273 )
        return 0;
      switch ( (unsigned __int16)a3 )
      {
        case 0x3F2u:
          EndDialog(hDlg, 3);
          break;
        case 0x3F3u:
          for ( i = dword_566878 == 0; i < 3; ++i )
          {
            if ( IsDlgButtonChecked(hDlg, *(&nIDButton + i)) )
              break;
          }
          EndDialog(hDlg, i);
          break;
        case 0x3F4u:
          EndDialog(hDlg, -1);
          break;
      }
    }
    return 1;
  }
}

// ===== sub_473210 @ 0x00473210..0x004734FF =====
INT_PTR __stdcall sub_473210(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  bool v4; // zf
  const CHAR *v5; // eax
  INT_PTR result; // eax
  const char *v7; // eax
  HWND DlgItem; // eax
  int v9; // [esp-4h] [ebp-41Ch]
  CHAR String[1036]; // [esp+8h] [ebp-410h] BYREF

  if ( a2 == 2 )
  {
    sub_473520();
    EnterCriticalSection(&stru_51A940);
    LeaveCriticalSection(&stru_51A940);
    DeleteCriticalSection(&stru_51A940);
    return 0;
  }
  if ( a2 == 272 )
  {
    sub_472B90(hDlg);
    InitializeCriticalSection(&stru_51A940);
    v9 = dword_566888 != 0 ? 5 : 0;
    dword_518C94 = (int)hDlg;
    dword_506704 = 1;
    DlgItem = GetDlgItem(hDlg, 1016);
    ShowWindow(DlgItem, v9);
    SendDlgItemMessageA(hDlg, 1015, 0x406u, 0, dword_518C8C);
    PostMessageA(hDlg, 0x111u, 0x8000u, 0);
    return 1;
  }
  if ( a2 != 273 )
    return 0;
  if ( (unsigned __int16)a3 > 0x8000u )
  {
    switch ( (unsigned __int16)a3 )
    {
      case 0x8001u:
        sub_473520();
        SendDlgItemMessageA(hDlg, 1015, 0x402u, a4 + 1, 0);
        PostMessageA(hDlg, 0x111u, 0x8000u, a4 + 1);
        result = 1;
        break;
      case 0x8002u:
        EndDialog(hDlg, 0);
        result = 1;
        break;
      case 0x8003u:
        SendDlgItemMessageA(hDlg, 1014, 0x406u, 0, a4);
        return 1;
      case 0x8004u:
        SendDlgItemMessageA(hDlg, 1014, 0x402u, a4, 0);
        return 1;
      default:
        return 1;
    }
  }
  else if ( (unsigned __int16)a3 == 0x8000 )
  {
    if ( sub_4708F0((int)&dword_518C84, a4) )
    {
      sub_473510();
      v4 = !sub_46F710();
      v7 = aS_6;
      if ( v4 )
        v7 = "Installing %s...";
      sprintf(String, v7, *(_DWORD *)(dword_518C90 + 4 * a4));
      SetDlgItemTextA(hDlg, 1013, String);
      SendDlgItemMessageA(hDlg, 1014, 0x406u, 0, 10);
      SendDlgItemMessageA(hDlg, 1014, 0x402u, 0, 0);
      return 1;
    }
    else
    {
      EndDialog(hDlg, 1);
      return 1;
    }
  }
  else if ( (unsigned __int16)a3 == 1016 )
  {
    EnterCriticalSection(&stru_51A940);
    if ( !dword_566858 )
    {
      v4 = !sub_46F710();
      v5 = (const CHAR *)&unk_4E6DC4;
      if ( v4 )
        v5 = "Are you sure you want to quit?";
      if ( sub_46BC80(0, hDlg, v5, 0x124u) == 6 )
      {
        dword_506704 = 0;
        EndDialog(hDlg, 0);
      }
    }
    LeaveCriticalSection(&stru_51A940);
    return 1;
  }
  else
  {
    return 1;
  }
  return result;
}

// ===== sub_473510 @ 0x00473510..0x00473516 =====
void *__usercall sub_473510@<eax>(void *result@<eax>)
{
  hObject = result;
  return result;
}

// ===== sub_473520 @ 0x00473520..0x00473570 =====
BOOL sub_473520()
{
  BOOL result; // eax
  DWORD ExitCode; // [esp+4h] [ebp-4h] BYREF

  result = GetExitCodeThread(hObject, &ExitCode);
  if ( result )
  {
    while ( ExitCode == 259 )
    {
      result = GetExitCodeThread(hObject, &ExitCode);
      if ( !result )
        return result;
    }
    return CloseHandle(hObject);
  }
  return result;
}

// ===== sub_473570 @ 0x00473570..0x00473576 =====
int __usercall sub_473570@<eax>(int result@<eax>)
{
  dword_56688C = result;
  return result;
}

// ===== sub_473580 @ 0x00473580..0x00473586 =====
int sub_473580()
{
  return dword_56688C;
}

// ===== sub_473590 @ 0x00473590..0x004735AB =====
int __cdecl sub_473590(_DWORD *a1)
{
  char v1; // al
  _DWORD *v2; // ecx

  v1 = sub_445030(a1);
  sub_4450D0(v2, v1);
  return 0;
}

// ===== sub_4735B0 @ 0x004735B0..0x004735CB =====
int __cdecl sub_4735B0(_DWORD *a1)
{
  __int16 v1; // ax
  _DWORD *v2; // ecx

  v1 = sub_445040(a1);
  sub_4450D0(v2, v1);
  return 0;
}

// ===== sub_4735D0 @ 0x004735D0..0x004735EA =====
int __cdecl sub_4735D0(_DWORD *a1)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_445060(a1);
  sub_4450D0(v2, v1);
  return 0;
}

// ===== sub_4735F0 @ 0x004735F0..0x0047361B =====
int __cdecl sub_4735F0(_DWORD *a1)
{
  int v1; // ecx
  int v2; // eax
  int v3; // edx
  _DWORD *v4; // ecx

  sub_445040(a1);
  v2 = sub_444FF0(v1);
  sub_4450D0(v4, (v2 - v3) | 0x8000000);
  return 0;
}

// ===== sub_473620 @ 0x00473620..0x0047364A =====
int __cdecl sub_473620(_DWORD *a1)
{
  int v1; // esi
  int v2; // ecx
  int v3; // eax
  _DWORD *v4; // ecx

  v1 = (__int16)sub_445040(a1);
  v3 = sub_444FD0(v2);
  sub_4450D0(v4, (v3 + v1) | 0x4000000);
  return 0;
}

// ===== sub_473650 @ 0x00473650..0x00473674 =====
int __cdecl sub_473650(_DWORD *a1)
{
  int v1; // esi
  int v2; // ecx
  int v3; // eax
  _DWORD *v4; // ecx

  v1 = (__int16)sub_445040(a1);
  v3 = sub_444FD0(v2);
  sub_4450D0(v4, v3 + v1);
  return 0;
}

// ===== sub_473680 @ 0x00473680..0x004736EE =====
int __cdecl sub_473680(_DWORD *a1)
{
  int *v1; // esi
  unsigned __int8 v2; // al
  int v3; // eax
  int v5; // [esp+8h] [ebp-4h]

  v1 = (int *)sub_48DF50(a1);
  v2 = sub_445030(a1);
  if ( v2 )
  {
    v3 = v2 - 1;
    if ( v3 )
    {
      if ( v3 == 1 )
        sub_4450D0(a1, *v1);
      else
        sub_4450D0(a1, v5);
      return 0;
    }
    else
    {
      sub_4450D0(a1, *(__int16 *)v1);
      return 0;
    }
  }
  else
  {
    sub_4450D0(a1, *(char *)v1);
    return 0;
  }
}

// ===== sub_4736F0 @ 0x004736F0..0x0047370E =====
int __fastcall sub_4736F0(int a1, _WORD *a2, int a3)
{
  int result; // eax

  result = a3;
  if ( a3 )
  {
    result = a3 - 1;
    if ( a3 == 1 )
    {
      *a2 = a1;
    }
    else
    {
      result = a3 - 2;
      if ( a3 == 2 )
        *(_DWORD *)a2 = a1;
    }
  }
  else
  {
    *(_BYTE *)a2 = a1;
  }
  return result;
}

// ===== sub_473710 @ 0x00473710..0x00473750 =====
int __cdecl sub_473710(_DWORD *a1)
{
  int v1; // esi
  _WORD *v2; // ebx
  unsigned __int8 v3; // al

  v1 = sub_4450B0(a1);
  v2 = (_WORD *)sub_48DF50(a1);
  v3 = sub_445030(a1);
  sub_4736F0(v1, v2, v3);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_473750 @ 0x00473750..0x00473789 =====
int __cdecl sub_473750(_DWORD *a1)
{
  _WORD *v1; // edi
  int v2; // ebx
  unsigned __int8 v3; // al

  v1 = (_WORD *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_445030(a1);
  sub_4736F0(v2, v1, v3);
  return 0;
}

// ===== sub_473790 @ 0x00473790..0x004737B9 =====
int __cdecl sub_473790(_DWORD *a1)
{
  void *v1; // ebx
  unsigned __int8 v2; // al

  v1 = (void *)sub_48DF50(a1);
  v2 = sub_445030(a1);
  sub_445070(v2, a1, v1);
  return 0;
}

// ===== sub_4737C0 @ 0x004737C0..0x00473873 =====
int __cdecl sub_4737C0(_DWORD *a1)
{
  _DWORD *v1; // ecx
  unsigned __int8 v2; // al
  int v3; // ebx
  int *v4; // esi
  int v5; // edx
  _WORD *v6; // edx
  int i; // edi
  int v8; // edx
  int v10; // [esp+Ch] [ebp-418h]
  int v11; // [esp+10h] [ebp-414h]
  char v12; // [esp+14h] [ebp-410h] BYREF
  _DWORD v13[3]; // [esp+414h] [ebp-10h]

  v11 = (unsigned __int8)sub_445030(a1);
  v2 = sub_445030(v1);
  v3 = v2;
  v4 = (int *)&v12;
  if ( v2 )
  {
    do
      *v4++ = sub_4450B0(a1);
    while ( v5 != 1 );
  }
  v6 = (_WORD *)sub_48DF50(a1);
  v13[0] = 1;
  v13[1] = 2;
  v13[2] = 4;
  v10 = v13[v11];
  for ( i = v3; i; --i )
  {
    sub_4736F0(*--v4, v6, v11);
    v6 = (_WORD *)(v10 + v8);
  }
  return 0;
}

// ===== sub_473880 @ 0x00473880..0x0047389C =====
int __cdecl sub_473880(int a1)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_444FF0(a1);
  sub_4450D0(v2, v1);
  return 0;
}

// ===== sub_4738A0 @ 0x004738A0..0x00473906 =====
int __cdecl sub_4738A0(_DWORD *a1)
{
  unsigned int v1; // eax
  unsigned int v2; // edx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_444C40(a1);
  if ( v2 >= v1 )
  {
    sprintf(Buffer, aSp, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_445000((int)a1, v2);
  return 0;
}

// ===== sub_473910 @ 0x00473910..0x00473983 =====
int __cdecl sub_473910(_DWORD *a1)
{
  unsigned int v1; // eax
  unsigned int v2; // edx
  int v3; // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  if ( !sub_4450B0(a1) )
    sub_4646F0(byte_4E76CC, (int)a1);
  v1 = sub_444C30(a1);
  if ( v2 >= v1 )
  {
    sprintf(Buffer, aIp, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_444FE0(v2, v3);
  return 0;
}

// ===== sub_473990 @ 0x00473990..0x00473A55 =====
int __cdecl sub_473990(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // esi
  _DWORD *v3; // ecx
  BOOL v4; // eax
  int v5; // ecx
  BOOL v7; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  switch ( (unsigned __int8)sub_445030(a1) )
  {
    case 0u:
      v4 = v2 != 0;
      break;
    case 1u:
      v4 = v2 == 0;
      break;
    case 2u:
      v4 = v2 > 0;
      break;
    case 3u:
      v4 = v2 >= 0;
      break;
    case 4u:
      v4 = v2 <= 0;
      break;
    case 5u:
      v4 = v2 < 0;
      break;
    default:
      v4 = v7;
      break;
  }
  if ( v4 )
  {
    if ( v1 >= sub_444C30(v3) )
    {
      sprintf(Buffer, aIp, v1);
      sub_4646F0(Buffer, (int)a1);
    }
    sub_444FE0(v1, v5);
  }
  return 0;
}

// ===== sub_473A70 @ 0x00473A70..0x00473ABD =====
int __cdecl sub_473A70(_DWORD *a1)
{
  unsigned int v1; // eax
  unsigned int v2; // edx
  int v3; // eax

  sub_444FF0((int)a1);
  v1 = sub_444C40(a1);
  if ( v2 >= v1 )
    sub_4646F0(byte_4E7738, (int)a1);
  sub_445130((int)a1);
  v3 = sub_444FD0((int)a1);
  sub_445110((int)a1, v3 + 1);
  return sub_473910(a1);
}

// ===== sub_473AC0 @ 0x00473AC0..0x00473AF9 =====
int __cdecl sub_473AC0(int a1)
{
  int v1; // edx
  unsigned int v2; // eax
  int v3; // edx
  unsigned int v4; // ecx
  int v5; // eax
  int v6; // edx
  int v7; // edx

  sub_444FF0(a1);
  v2 = sub_444C50(v1);
  if ( v4 <= v2 )
    return 4;
  v5 = sub_4450F0(v3);
  sub_444FE0(v5, v6);
  sub_445150(v7);
  return 0;
}

// ===== sub_473B00 @ 0x00473B00..0x00473B25 =====
int __cdecl sub_473B00(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 + v1);
  return 0;
}

// ===== sub_473B30 @ 0x00473B30..0x00473B59 =====
int __cdecl sub_473B30(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 - v1);
  return 0;
}

// ===== sub_473B60 @ 0x00473B60..0x00473B8A =====
int __cdecl sub_473B60(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v1 * v3);
  return 0;
}

// ===== sub_473B90 @ 0x00473B90..0x00473BD0 =====
int __cdecl sub_473B90(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( v1 )
    sub_4450D0(a1, v2 / v1);
  else
    sub_4450D0(a1, -1);
  return 0;
}

// ===== sub_473BD0 @ 0x00473BD0..0x00473C10 =====
int __cdecl sub_473BD0(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( v1 )
    sub_4450D0(a1, v2 % v1);
  else
    sub_4450D0(a1, -1);
  return 0;
}

// ===== sub_473C10 @ 0x00473C10..0x00473C39 =====
int __cdecl sub_473C10(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v1 & v3);
  return 0;
}

// ===== sub_473C40 @ 0x00473C40..0x00473C69 =====
int __cdecl sub_473C40(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v1 | v3);
  return 0;
}

// ===== sub_473C70 @ 0x00473C70..0x00473C99 =====
int __cdecl sub_473C70(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v1 ^ v3);
  return 0;
}

// ===== sub_473CA0 @ 0x00473CA0..0x00473CBE =====
int __cdecl sub_473CA0(_DWORD *a1)
{
  int v1; // eax
  _DWORD *v2; // edx

  v1 = sub_4450B0(a1);
  sub_4450D0(v2, ~v1);
  return 0;
}

// ===== sub_473CC0 @ 0x00473CC0..0x00473CEB =====
int __cdecl sub_473CC0(_DWORD *a1)
{
  char v1; // di
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 << v1);
  return 0;
}

// ===== sub_473CF0 @ 0x00473CF0..0x00473D1B =====
int __cdecl sub_473CF0(_DWORD *a1)
{
  char v1; // di
  _DWORD *v2; // edx
  unsigned int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 >> v1);
  return 0;
}

// ===== sub_473D20 @ 0x00473D20..0x00473D4B =====
int __cdecl sub_473D20(_DWORD *a1)
{
  char v1; // di
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 >> v1);
  return 0;
}

// ===== sub_473D50 @ 0x00473D50..0x00473D7C =====
int __cdecl sub_473D50(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 == v1);
  return 0;
}

// ===== sub_473D80 @ 0x00473D80..0x00473DAC =====
int __cdecl sub_473D80(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 != v1);
  return 0;
}

// ===== sub_473DB0 @ 0x00473DB0..0x00473DDC =====
int __cdecl sub_473DB0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 <= v1);
  return 0;
}

// ===== sub_473DE0 @ 0x00473DE0..0x00473E0C =====
int __cdecl sub_473DE0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 >= v1);
  return 0;
}

// ===== sub_473E10 @ 0x00473E10..0x00473E3C =====
int __cdecl sub_473E10(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 < v1);
  return 0;
}

// ===== sub_473E40 @ 0x00473E40..0x00473E6C =====
int __cdecl sub_473E40(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450D0(v4, v3 > v1);
  return 0;
}

// ===== sub_473E70 @ 0x00473E70..0x00473EAE =====
int __cdecl sub_473E70(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  _DWORD *v3; // edx

  v1 = sub_4450B0(a1);
  if ( sub_4450B0(v2) && v1 )
  {
    sub_4450D0(v3, 1);
    return 0;
  }
  else
  {
    sub_4450D0(v3, 0);
    return 0;
  }
}

// ===== sub_473EB0 @ 0x00473EB0..0x00473EF1 =====
int __cdecl sub_473EB0(_DWORD *a1)
{
  int v1; // edx

  sub_4450B0(a1);
  if ( sub_4450B0(a1) || v1 )
  {
    sub_4450D0(a1, 1);
    return 0;
  }
  else
  {
    sub_4450D0(a1, 0);
    return 0;
  }
}

// ===== sub_473F00 @ 0x00473F00..0x00473F1F =====
int __cdecl sub_473F00(_DWORD *a1)
{
  int v1; // eax
  _DWORD *v2; // edx

  v1 = sub_4450B0(a1);
  sub_4450D0(v2, v1 == 0);
  return 0;
}

// ===== sub_473F20 @ 0x00473F20..0x00473F54 =====
int __cdecl sub_473F20(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  _DWORD *v4; // edx
  _DWORD *v5; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  if ( sub_4450B0(v4) )
    v1 = v3;
  sub_4450D0(v5, v1);
  return 0;
}

// ===== sub_473F60 @ 0x00473F60..0x00473FBF =====
int __cdecl sub_473F60(_DWORD *a1)
{
  __int64 v1; // kr00_8
  __int64 v2; // rax
  __int64 v4; // [esp+10h] [ebp-8h]

  v1 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v2 = sub_4450B0(a1) * v4 / v1;
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_473FC0 @ 0x00473FC0..0x00473FEF =====
int __cdecl sub_473FC0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax
  int v4; // [esp-4h] [ebp-Ch]

  v4 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_401000(v1, v4);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_473FF0 @ 0x00473FF0..0x0047404D =====
int __cdecl sub_473FF0(_DWORD *a1)
{
  int v2; // [esp+8h] [ebp-Ch]
  int v3; // [esp+Ch] [ebp-8h]
  int v4; // [esp+10h] [ebp-4h]

  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_4450D0(a1, (int)sqrt((double)v4 * (double)v4 + (double)v3 * (double)v3 + (double)v2 * (double)v2));
  return 0;
}
