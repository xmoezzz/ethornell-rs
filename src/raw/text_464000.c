#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_464190 @ 0x00464190..0x004641ED =====
int sub_464190()
{
  if ( __uncaught_exception() )
  {
    sub_464210();
    SetWindowLongA(dword_565FD8, -4, dword_565FDC);
    DeleteObject(ho);
    DestroyWindow(dword_565FD8);
    dword_565FDC = 0;
    ho = 0;
    dword_565FD8 = 0;
    sub_45FFB0(0);
  }
  return 0;
}

// ===== sub_4641F0 @ 0x004641F0..0x00464209 =====
int __thiscall sub_4641F0(char *this)
{
  int result; // eax

  result = 0;
  if ( (unsigned int)(this - 25) <= 0xAF )
  {
    dword_506EF4 = (int)this;
    return 1;
  }
  return result;
}

// ===== sub_464210 @ 0x00464210..0x0046427A =====
int __usercall sub_464210@<eax>(int a1@<esi>)
{
  int result; // eax
  HWND DefaultIMEWnd; // eax

  result = __uncaught_exception();
  if ( result )
  {
    ShowWindow(dword_565FD8, a1 != 0 ? 5 : 0);
    SendMessageA(dword_565FD8, 0x30u, a1 != 0 ? (unsigned int)ho : 0, 0);
    DefaultIMEWnd = ImmGetDefaultIMEWnd(hWndParent);
    result = SendMessageA(DefaultIMEWnd, 0x283u, (a1 != 0) + 33, 0);
    dword_565FE4 = a1;
  }
  return result;
}

// ===== sub_464280 @ 0x00464280..0x00464286 =====
int sub_464280()
{
  return dword_565FE4;
}

// ===== sub_464290 @ 0x00464290..0x004642B2 =====
int __usercall sub_464290@<eax>(int a1@<eax>)
{
  int v1; // edx
  int v2; // ecx
  int result; // eax

  v1 = BYTE1(a1);
  v2 = (unsigned __int8)a1 << 8;
  result = BYTE2(a1);
  dword_506BE4 = result | ((v2 | v1) << 8);
  return result;
}

// ===== sub_4642C0 @ 0x004642C0..0x004642C6 =====
int sub_4642C0()
{
  return dword_506BE4;
}

// ===== sub_4642D0 @ 0x004642D0..0x004642F4 =====
_DWORD *__usercall sub_4642D0@<eax>(_DWORD *result@<eax>)
{
  int v1; // edx
  int v2; // ecx
  int v3; // edx

  v1 = dword_565FEC;
  *result = dword_565FE8;
  v2 = dword_565FF0;
  result[1] = v1;
  v3 = dword_565FF4;
  result[2] = v2;
  result[3] = v3;
  return result;
}

// ===== sub_464300 @ 0x00464300..0x00464312 =====
char *__usercall sub_464300@<eax>(char *result@<eax>)
{
  CHAR *v1; // edx
  char v2; // cl

  v1 = (CHAR *)(String - result);
  do
  {
    v2 = *result;
    result[(_DWORD)v1] = *result;
    ++result;
  }
  while ( v2 );
  return result;
}

// ===== sub_464320 @ 0x00464320..0x0046434B =====
int __cdecl sub_464320(LPSTR lpString)
{
  int v1; // ecx

  if ( __uncaught_exception() )
    return GetWindowTextA(dword_565FD8, lpString, 256);
  else
    return v1;
}

// ===== sub_464350 @ 0x00464350..0x00464356 =====
int __usercall sub_464350@<eax>(int result@<eax>)
{
  dword_565FF8 = result;
  return result;
}

// ===== sub_464360 @ 0x00464360..0x00464366 =====
int __usercall sub_464360@<eax>(int result@<eax>)
{
  dword_565FFC = result;
  return result;
}

// ===== sub_464370 @ 0x00464370..0x0046443A =====
LRESULT __stdcall sub_464370(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if ( Msg > 0x102 )
  {
    if ( Msg != 513 && Msg != 642 )
      return CallWindowProcA((WNDPROC)dword_565FDC, hWnd, Msg, wParam, lParam);
    goto LABEL_17;
  }
  switch ( Msg )
  {
    case 0x102u:
      if ( wParam != 9 && (!dword_565FFC || wParam - 32 > 0x5F) )
      {
        if ( wParam != 13 )
          return CallWindowProcA((WNDPROC)dword_565FDC, hWnd, Msg, wParam, lParam);
        SendMessageA(hWnd, 0xB1u, 0, -1);
        SetFocus(hWndParent);
        if ( dword_565FF8 )
          sub_464210(0);
      }
      return 0;
    case 0xFu:
      sub_461CB0();
      break;
    case 0x100u:
      if ( wParam == 9 )
        sub_464210(0);
LABEL_17:
      InvalidateRect(hWnd, 0, 0);
      break;
  }
  return CallWindowProcA((WNDPROC)dword_565FDC, hWnd, Msg, wParam, lParam);
}

// ===== sub_464440 @ 0x00464440..0x00464446 =====
int __usercall sub_464440@<eax>(int result@<eax>)
{
  dword_566000 = result;
  return result;
}

// ===== sub_464450 @ 0x00464450..0x00464456 =====
int sub_464450()
{
  return dword_566000;
}

// ===== sub_464460 @ 0x00464460..0x00464479 =====
void sub_464460()
{
  operator delete[](dword_566004);
  dword_566004 = 0;
}

// ===== sub_464480 @ 0x00464480..0x004644B2 =====
unsigned int __usercall sub_464480@<eax>(int a1@<eax>)
{
  char *v1; // ecx
  unsigned int result; // eax
  int v4; // esi
  char v5; // dl

  v1 = (char *)dword_566004;
  result = 0;
  if ( dword_566004 )
  {
    result = strlen((const char *)dword_566004) + 1;
    if ( a1 )
    {
      v4 = a1 - (_DWORD)dword_566004;
      do
      {
        v5 = *v1;
        v1[v4] = *v1;
        ++v1;
      }
      while ( v5 );
    }
  }
  return result;
}

// ===== sub_4644C0 @ 0x004644C0..0x00464500 =====
int __usercall sub_4644C0@<eax>(const char *a1@<esi>)
{
  int result; // eax
  const char *v2; // ecx
  int v3; // edx
  char v4; // al

  result = 0;
  if ( !dword_566004 )
  {
    dword_566004 = operator new[](strlen(a1) + 1);
    v2 = a1;
    v3 = (_BYTE *)dword_566004 - a1;
    do
    {
      v4 = *v2;
      v2[v3] = *v2;
      ++v2;
    }
    while ( v4 );
    return 1;
  }
  return result;
}

// ===== sub_464500 @ 0x00464500..0x0046451B =====
int __usercall sub_464500@<eax>(int a1@<eax>)
{
  return sub_46BC80(hWndParent, a1, 4112);
}

// ===== sub_464520 @ 0x00464520..0x0046456A =====
void __usercall __noreturn sub_464520(char *a1@<eax>, int pExceptionObject)
{
  if ( !sub_464480(0) )
    sub_464500((int)a1);
  if ( sub_464450() )
    sub_4644C0(a1);
  if ( pExceptionObject )
    operator delete[](a1);
  pExceptionObject = 0x7FFFFFFF;
  _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1H);
}

// ===== sub_464570 @ 0x00464570..0x004646F0 =====
void __usercall sub_464570(int a1@<eax>, char *Buffer, const char *a3)
{
  int v4; // eax
  int v5; // ecx
  unsigned int v6; // edi
  int v7; // eax
  unsigned int v8; // ecx
  int v9; // edx
  const char **v10; // edx
  int v11; // eax
  char *v12; // edi
  int v13; // eax
  _DWORD *v14; // ebx
  int i; // esi
  const char **v16; // ecx
  unsigned int v17; // eax
  const char *v18; // eax
  int v19; // edi
  char v20; // cl
  int v21; // eax
  const char *v22; // [esp-10h] [ebp-24h]
  int v23; // [esp-Ch] [ebp-20h]
  int v24; // [esp-Ch] [ebp-20h]
  int v25; // [esp-4h] [ebp-18h]
  int v26; // [esp+Ch] [ebp-8h]
  void *v27; // [esp+10h] [ebp-4h] BYREF

  sub_444C60(a1);
  v4 = sub_444FD0(a1);
  v6 = *(unsigned __int8 *)(v5 + v4);
  if ( v6 >= 0x80 )
    v6 = *(unsigned __int8 *)(v5 + v4 + 1) + (v6 << 8);
  (*(void (__thiscall **)(int, void **, int))(*(_DWORD *)a1 + 20))(a1, &v27, 1);
  sub_444FD0(a1);
  v7 = sub_444FF0(a1);
  if ( v9 )
  {
    v10 = (const char **)v27;
    if ( *((_DWORD *)v27 + 2) > v8 )
    {
      do
        v10 += 4;
      while ( (unsigned int)v10[2] > v8 );
    }
    v25 = v7;
    v23 = v8 - (_DWORD)v10[2];
    v22 = *v10;
    v11 = sub_42D560(a1);
    sprintf(
      Buffer,
      "Thread [ %d ] , Program [ %s ] , IP [ $%.8X ] , Instruction [ $%X ] , SP [ $%.8X ]\n\n",
      v11,
      v22,
      v23,
      v6,
      v25);
    v12 = &Buffer[strlen(Buffer)];
    v13 = sub_445190(a1, 0);
    v14 = 0;
    v26 = v13;
    if ( v13 )
    {
      v14 = operator new[](4 * v13);
      sub_445190(a1, (int)v14);
      v13 = v26;
    }
    for ( i = v13; i; v12 += strlen(v12) )
    {
      v16 = (const char **)v27;
      v17 = v14[--i];
      if ( *((_DWORD *)v27 + 2) > v17 )
      {
        do
          v16 += 4;
        while ( (unsigned int)v16[2] > v17 );
      }
      sprintf(v12, "( %s - $%.8X )\n", *v16, v17 - (_DWORD)v16[2]);
    }
    operator delete[](v14);
    if ( v26 )
      *v12++ = 10;
    v18 = a3;
    v19 = v12 - a3;
    do
    {
      v20 = *v18;
      v18[v19] = *v18;
      ++v18;
    }
    while ( v20 );
    operator delete(v27);
  }
  else
  {
    v24 = v8;
    v21 = sub_42D560(a1);
    sprintf(Buffer, "Thread [ %d ] , IP [ $%.8X ] , Instruction [ $%X ]\n\n%s", v21, v24, v6, a3);
  }
}

// ===== sub_4646F0 @ 0x004646F0..0x00464757 =====
void __usercall __noreturn sub_4646F0(const char *a1@<edi>, int a2)
{
  char *v2; // esi
  char Buffer[780]; // [esp+8h] [ebp-310h] BYREF

  v2 = (char *)operator new[](0x10000u);
  sub_464570(a2, v2, a1);
  sub_46B400(Buffer);
  sub_465E30(v2, strlen(v2));
  sub_464520(v2, 1);
}

// ===== sub_464760 @ 0x00464760..0x004648ED =====
char *__usercall sub_464760@<eax>(unsigned int a1@<eax>, char *a2)
{
  unsigned int v2; // esi
  int v3; // ebx
  const char *v4; // ecx
  unsigned int v5; // edi
  unsigned int v6; // esi
  const char *v7; // esi
  char *result; // eax
  char *v9; // edi
  char *v10; // esi
  void *v11[11]; // [esp+0h] [ebp-78h]
  _DWORD v12[10]; // [esp+2Ch] [ebp-4Ch]
  _DWORD v13[4]; // [esp+54h] [ebp-24h]
  _DWORD v14[3]; // [esp+64h] [ebp-14h]
  char *Buffer; // [esp+70h] [ebp-8h]
  int v16; // [esp+74h] [ebp-4h]

  v2 = a1;
  if ( a1 )
  {
    v12[0] = MultiByteStr;
    v12[1] = &unk_4E6D68;
    v12[2] = &unk_4E6D6C;
    v12[3] = &unk_4E6D70;
    v12[4] = &unk_4E6D74;
    v12[5] = &unk_4E6D78;
    v12[6] = &unk_4E6D7C;
    v12[7] = &unk_4E6D80;
    v12[8] = &unk_4E6D84;
    v12[9] = &unk_4E6D88;
    v13[0] = MultiByteStr;
    v13[1] = &unk_4E6D8C;
    v13[2] = &unk_4E6D90;
    v13[3] = &unk_4E6D94;
    v14[0] = MultiByteStr;
    v14[1] = &unk_4E6D98;
    v14[2] = &unk_4E6D9C;
    v3 = 0;
    do
    {
      Buffer = (char *)operator new[](0x10u);
      v11[v3 + 1] = Buffer;
      v16 = v3 % 4;
      if ( v3 % 4 )
      {
        if ( v2 % 0xA )
          v4 = (const char *)v13[v3 % 4];
        else
          v4 = MultiByteStr;
      }
      else
      {
        v4 = (const char *)v14[v3 / 4];
      }
      v5 = v2 / 0xA;
      v6 = v2 % 0xA;
      if ( v6 == 1 && v16 )
        v7 = MultiByteStr;
      else
        v7 = (const char *)v12[v6];
      sprintf(Buffer, "%s%s", v7, v4);
      v2 = v5;
      ++v3;
    }
    while ( v5 );
    result = a2;
    v9 = a2;
    if ( v3 )
    {
      do
      {
        v10 = (char *)v11[v3--];
        strcpy(v9, v10);
        v9 += strlen(v9);
        operator delete[](v10);
      }
      while ( v3 );
      return a2;
    }
  }
  else
  {
    result = a2;
    *(_WORD *)a2 = -5225;
    a2[2] = 0;
  }
  return result;
}

// ===== sub_4648F0 @ 0x004648F0..0x00464979 =====
int __cdecl sub_4648F0(BOOL *a1, LPCSTR lpFileName)
{
  HANDLE FileA; // eax
  void *v3; // esi
  _BYTE v5[16]; // [esp+8h] [ebp-98h] BYREF
  int v6; // [esp+18h] [ebp-88h]
  BOOL pfOn; // [esp+9Ch] [ebp-4h] BYREF

  sub_46F800(v5);
  if ( v6 != 2 )
    return 1;
  FileA = CreateFileA(lpFileName, 0x80000000, 1u, 0, 3u, 0x80u, 0);
  v3 = FileA;
  if ( FileA == (HANDLE)-1 )
    return 0;
  if ( GetDevicePowerState(FileA, &pfOn) )
  {
    if ( a1 )
      *a1 = pfOn;
  }
  CloseHandle(v3);
  return 1;
}

// ===== sub_464980 @ 0x00464980..0x00464994 =====
BOOL __usercall sub_464980@<eax>(_BYTE *a1@<eax>)
{
  return *a1 == 92 || a1[1] == 58;
}

// ===== sub_4649A0 @ 0x004649A0..0x004649E3 =====
int __usercall sub_4649A0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int result; // eax
  char *v4; // ecx
  int v5; // esi
  char v6; // dl
  CHAR *v7; // ecx
  int v8; // esi
  CHAR v9; // dl

  result = 1;
  if ( a2 )
  {
    if ( a2 == 1 && byte_517C08 )
    {
      v4 = &byte_517C08;
      v5 = a1 - (_DWORD)&byte_517C08;
      do
      {
        v6 = *v4;
        v4[v5] = *v4;
        ++v4;
      }
      while ( v6 );
    }
    else
    {
      return 0;
    }
  }
  else
  {
    v7 = &Buffer;
    v8 = a1 - (_DWORD)&Buffer;
    do
    {
      v9 = *v7;
      v7[v8] = *v7;
      ++v7;
    }
    while ( v9 );
  }
  return result;
}

// ===== sub_4649F0 @ 0x004649F0..0x00464A08 =====
int __usercall sub_4649F0@<eax>(const char *a1@<eax>, const char *a2@<ecx>, char *Buffer)
{
  return sprintf(Buffer, "%s%s", a2, a1);
}

// ===== sub_464A10 @ 0x00464A10..0x00464A7D =====
int sub_464A10()
{
  CHAR v1[780]; // [esp+8h] [ebp-928h] BYREF
  CHAR Filename[780]; // [esp+314h] [ebp-61Ch] BYREF
  CHAR v3[780]; // [esp+620h] [ebp-310h] BYREF

  GetModuleFileNameA(0, Filename, 0x30Cu);
  sub_42D410(0, v3, v1, 0, Filename);
  return sprintf(&Buffer, "%s%s", v1, v3);
}

// ===== sub_464A80 @ 0x00464A80..0x00464AFE =====
UINT sub_464A80()
{
  int i; // ebx
  UINT result; // eax
  CHAR RootPathName[4]; // [esp+8h] [ebp-310h] BYREF
  _BYTE v3[776]; // [esp+Ch] [ebp-30Ch] BYREF

  strcpy(RootPathName, "A:\\");
  memset(v3, 0, sizeof(v3));
  for ( i = 0; i < 26; dword_51828C[i] = result != 3 )
  {
    RootPathName[0] = i + 65;
    result = GetDriveTypeA(RootPathName);
    dword_518228[i++] = result;
  }
  return result;
}

// ===== sub_464B00 @ 0x00464B00..0x00464B6A =====
int __usercall sub_464B00@<eax>(_DWORD *a1@<eax>)
{
  int result; // eax
  _DWORD *v3; // ecx
  char *v4; // edi
  int v5; // esi

  sub_464A80();
  result = 26;
  v3 = a1;
  v4 = (char *)((char *)dword_518228 - (char *)a1);
  v5 = 26;
  do
  {
    switch ( *(_DWORD *)((char *)v3 + (_DWORD)v4) )
    {
      case 2:
        *v3 = 2;
        break;
      case 3:
        *v3 = 1;
        break;
      case 4:
        *v3 = 3;
        break;
      case 5:
        *v3 = 4;
        break;
      case 6:
        *v3 = 5;
        break;
      default:
        *v3 = 0;
        --result;
        break;
    }
    ++v3;
    --v5;
  }
  while ( v5 );
  return result;
}

// ===== sub_464B80 @ 0x00464B80..0x00464D15 =====
BOOL __cdecl sub_464B80(const char *a1)
{
  char v1; // cl
  int v2; // edi
  HANDLE v3; // eax
  void *v4; // esi
  BOOL VolumeInformationA; // ebx
  UINT v6; // edi
  _BYTE v8[16]; // [esp+4h] [ebp-6C0h] BYREF
  int v9; // [esp+14h] [ebp-6B0h]
  DWORD BytesReturned; // [esp+98h] [ebp-62Ch] BYREF
  DWORD FileSystemFlags; // [esp+9Ch] [ebp-628h] BYREF
  DWORD MaximumComponentLength; // [esp+A0h] [ebp-624h] BYREF
  CHAR RootPathName[4]; // [esp+A4h] [ebp-620h] BYREF
  CHAR VolumeNameBuffer[780]; // [esp+A8h] [ebp-61Ch] BYREF
  CHAR FileName; // [esp+3B4h] [ebp-310h] BYREF
  char v16; // [esp+3B5h] [ebp-30Fh]

  strcpy(&FileName, a1);
  sub_42EA80(v1, &FileName);
  if ( (unsigned __int8)(FileName - 97) > 0x19u || v16 != 58 )
    return FileName == 92 && v16 == 92;
  v2 = FileName;
  if ( dword_51810C[FileName] && (sub_46F800(v8), v9 == 2) )
  {
    wsprintfA(&FileName, "\\\\.\\%c%c", v2, 58);
    v3 = CreateFileA(&FileName, 0x80000000, 1u, 0, 3u, 0x80u, 0);
    v4 = v3;
    if ( v3 == (HANDLE)-1
      || (VolumeInformationA = DeviceIoControl(v3, 0x2D4800u, 0, 0, 0, 0, &BytesReturned, 0),
          CloseHandle(v4),
          !VolumeInformationA) )
    {
      sprintf(RootPathName, "%c:\\", v2);
      v6 = SetErrorMode(1u);
      VolumeInformationA = GetVolumeInformationA(
                             RootPathName,
                             VolumeNameBuffer,
                             0x30Cu,
                             0,
                             &MaximumComponentLength,
                             &FileSystemFlags,
                             0,
                             0);
      SetErrorMode(v6);
    }
    return VolumeInformationA;
  }
  else
  {
    return 1;
  }
}

// ===== sub_464D20 @ 0x00464D20..0x00464D66 =====
int sub_464D20()
{
  int i; // esi

  for ( i = 0; sub_46FB10(i); ++i )
    ;
  return sub_46FB10(i - 2);
}

// ===== sub_464D70 @ 0x00464D70..0x00464F40 =====
int __fastcall sub_464D70(int a1, int a2, const char *a3, int a4)
{
  CHAR *v4; // esi
  int v5; // ebx
  UINT DriveTypeA; // eax
  int v7; // ebx
  int v8; // esi
  const char *v9; // edi
  bool v10; // zf
  const char *v11; // eax
  _DWORD v13[27]; // [esp+Ch] [ebp-A9Ch]
  int v14; // [esp+78h] [ebp-A30h]
  int v15; // [esp+7Ch] [ebp-A2Ch]
  const char *v16; // [esp+80h] [ebp-A28h]
  int v17; // [esp+84h] [ebp-A24h]
  int v18; // [esp+88h] [ebp-A20h]
  char v19[780]; // [esp+8Ch] [ebp-A1Ch] BYREF
  CHAR FileName[780]; // [esp+398h] [ebp-710h] BYREF
  CHAR Buffer[1024]; // [esp+6A4h] [ebp-404h] BYREF

  v16 = a3;
  v4 = Buffer;
  v13[26] = a1;
  v14 = a2;
  GetLogicalDriveStringsA(0x400u, Buffer);
  v5 = 0;
  v18 = 0;
  if ( Buffer[0] )
  {
    do
    {
      DriveTypeA = GetDriveTypeA(v4);
      if ( DriveTypeA == 5 || DriveTypeA == 2 )
        v13[v5++] = v4;
      v4 += strlen(v4) + 1;
    }
    while ( *v4 );
    v18 = v5;
  }
  sub_464D20();
  v7 = 0;
  v17 = 1;
  while ( 2 )
  {
    v15 = 0;
    while ( !v7 )
    {
      v8 = 0;
      if ( v18 <= 0 )
      {
LABEL_14:
        Sleep(0x32u);
      }
      else
      {
        while ( 1 )
        {
          v9 = (const char *)v13[v8];
          if ( sub_464B80(v9) )
          {
            sub_4649F0(v16, v9, FileName);
            if ( GetFileAttributesA(FileName) != -1 )
              break;
          }
          if ( ++v8 >= v18 )
            goto LABEL_14;
        }
        sprintf(&byte_517C08, "%s%s", v9, v19);
        v7 = 1;
        v17 = 0;
      }
      if ( ++v15 >= 20 )
      {
        if ( !v7 )
        {
          if ( !a4 )
            return v7;
          if ( sub_46BC80(hWndParent, v14, 65) != 1 )
          {
            v10 = sub_46F710() == 0;
            v11 = (const char *)&unk_4E6DC4;
            if ( v10 )
              v11 = "Are you sure you want to quit?";
            if ( sub_46BC80(hWndParent, v11, 292) == 6 )
              return v7;
          }
        }
        break;
      }
    }
    if ( v17 )
      continue;
    return v7;
  }
}

// ===== sub_464F40 @ 0x00464F40..0x00464FFA =====
int __fastcall sub_464F40(const char *a1, const char *a2, const char *a3, int a4)
{
  const char *v5; // eax
  int v6; // esi
  char v7; // cl
  int result; // eax
  int v9; // ebx
  CHAR v11[780]; // [esp+10h] [ebp-310h] BYREF

  v5 = a1;
  v6 = &unk_5182F8 - (_UNKNOWN *)a1;
  do
  {
    v7 = *v5;
    v5[v6] = *v5;
    ++v5;
  }
  while ( v7 );
  strcpy(byte_566008, a3);
  strcpy(byte_566320, a2);
  result = sub_464D70((int)a3, (int)a2, a1, a4);
  v9 = result;
  if ( result )
  {
    sub_42D410(0, 0, v11, 0, byte_517C08);
    sprintf(&byte_518608, "%s\\%s", v11, a1);
    return v9;
  }
  return result;
}

// ===== sub_465000 @ 0x00465000..0x00465082 =====
int __usercall sub_465000@<eax>(const char *a1@<eax>)
{
  CHAR *v1; // edx
  char v2; // cl
  int result; // eax
  char v4; // cl

  if ( a1 )
  {
    if ( a1[strlen(a1) - 1] == 92 )
    {
      v1 = (CHAR *)(&Buffer - a1);
      do
      {
        v2 = *a1;
        a1[(_DWORD)v1] = *a1;
        ++a1;
      }
      while ( v2 );
    }
    else
    {
      sprintf(&Buffer, "%s\\", a1);
    }
  }
  else
  {
    sub_464A10();
  }
  byte_517C08[0] = 0;
  byte_518608 = 0;
  SetCurrentDirectoryA(&Buffer);
  result = 0;
  do
  {
    v4 = *(&Buffer + result);
    byte_518978[result++] = v4;
  }
  while ( v4 );
  return result;
}

// ===== sub_465090 @ 0x00465090..0x004650BB =====
char *__usercall sub_465090@<eax>(char *result@<eax>, char *a2@<ecx>)
{
  char *v2; // esi
  char v3; // dl
  char *v4; // edx
  char v5; // cl

  v2 = (char *)(aSystemArc_0 - a2);
  do
  {
    v3 = *a2;
    a2[(_DWORD)v2] = *a2;
    ++a2;
  }
  while ( v3 );
  v4 = (char *)(aIplBp_0 - result);
  do
  {
    v5 = *result;
    result[(_DWORD)v4] = *result;
    ++result;
  }
  while ( v5 );
  return result;
}

// ===== sub_4650C0 @ 0x004650C0..0x004650F0 =====
int __usercall sub_4650C0@<eax>(int a1@<edx>, char *a2)
{
  char *v2; // eax
  int v3; // edx
  char v4; // cl
  int result; // eax

  v2 = aSystemArc_0;
  v3 = a1 - (_DWORD)aSystemArc_0;
  do
  {
    v4 = *v2;
    v2[v3] = *v2;
    ++v2;
  }
  while ( v4 );
  strcpy(a2, aIplBp_0);
  return result;
}

// ===== sub_4650F0 @ 0x004650F0..0x0046523F =====
char *__cdecl sub_4650F0(char *a1)
{
  const char *v1; // ecx
  const char *v2; // edi
  char *result; // eax
  DWORD FileAttributesA; // eax
  bool v5; // zf
  char *v6; // eax
  char v7; // cl
  char *v8; // esi
  char *v9; // ecx
  char v10[780]; // [esp+8h] [ebp-124Ch] BYREF
  CHAR v11[780]; // [esp+314h] [ebp-F40h] BYREF
  char Buffer[780]; // [esp+620h] [ebp-C34h] BYREF
  CHAR v13[780]; // [esp+92Ch] [ebp-928h] BYREF
  CHAR v14[780]; // [esp+C38h] [ebp-61Ch] BYREF
  char Str[780]; // [esp+F44h] [ebp-310h] BYREF

  v2 = v1;
  sub_465000(0);
  sub_465090(a1, "system.arc");
  result = (char *)strcmp(v2, ".");
  if ( result )
  {
    FileAttributesA = GetFileAttributesA(v2);
    if ( FileAttributesA == -1 )
    {
      sub_42D410(v14, Str, v11, v13, v2);
      sprintf(Buffer, "%s%s", v11, Str);
      sprintf(v10, "%s%s", v13, v14);
      sub_465000(Buffer);
      v9 = v10;
    }
    else
    {
      v5 = (FileAttributesA & 0x10) == 0;
      v6 = (char *)v2;
      if ( !v5 )
        return (char *)sub_465000(v2);
      do
      {
        v7 = *v6;
        v6[Str - v2] = *v6;
        ++v6;
      }
      while ( v7 );
      v8 = strrchr(Str, 92);
      if ( v8 )
      {
        *v8 = 0;
        sub_465000(Str);
        v9 = v8 + 1;
      }
      else
      {
        v9 = (char *)v2;
      }
    }
    return sub_465090(a1, v9);
  }
  return result;
}

// ===== sub_465240 @ 0x00465240..0x00465246 =====
int __usercall sub_465240@<eax>(int result@<eax>)
{
  dword_506BE0 = result;
  return result;
}

// ===== sub_465250 @ 0x00465250..0x00465287 =====
void sub_465250()
{
  void **v0; // esi
  void **v1; // edi
  void *v2; // eax

  v0 = (void **)dword_566630;
  if ( dword_566630 )
  {
    do
    {
      v1 = v0;
      v2 = *v0;
      v0 = (void **)v0[1];
      operator delete[](v2);
      operator delete(v1);
    }
    while ( v0 );
  }
  dword_566630 = 0;
}

// ===== sub_465290 @ 0x00465290..0x004652D6 =====
_BYTE *__usercall sub_465290@<eax>(const char *a1@<edi>)
{
  _DWORD *v1; // esi
  _BYTE *result; // eax
  const char *v3; // ecx
  _BYTE *v4; // edx

  v1 = operator new(8u);
  result = operator new[](strlen(a1) + 1);
  v1[1] = dword_566630;
  *v1 = result;
  v3 = a1;
  v4 = result;
  do
  {
    LOBYTE(result) = *v3;
    *v4++ = *v3++;
  }
  while ( (_BYTE)result );
  dword_566630 = v1;
  return result;
}

// ===== sub_4652E0 @ 0x004652E0..0x0046531C =====
BOOL __usercall sub_4652E0@<eax>(const char *a1@<eax>)
{
  return strcmp(a1, "DSC FORMAT 1.00") == 0;
}

// ===== sub_465320 @ 0x00465320..0x0046557A =====
int __cdecl sub_465320(void *a1, size_t *a2, int a3, __int64 a4)
{
  const char *v4; // ecx
  int v5; // esi
  unsigned int *v6; // ebx
  _BYTE *v7; // edi
  unsigned int v8; // ebx
  int v9; // edi
  int v10; // ecx
  int v11; // ecx
  int v12; // ebx
  unsigned int v13; // edi
  int v14; // eax
  unsigned int v15; // eax
  int v16; // ecx
  int v17; // ebx
  size_t v18; // edi
  int v19; // ecx
  int v20; // ecx
  int v21; // ebx
  unsigned int v22; // edi
  void *v24; // [esp+24h] [ebp-20h]
  int v25; // [esp+2Ch] [ebp-18h]
  void (__thiscall ***v26)(_DWORD, int); // [esp+30h] [ebp-14h]
  void *v27; // [esp+34h] [ebp-10h]

  v5 = (int)v4;
  v27 = 0;
  v25 = 0;
  if ( sub_4652E0(v4) )
  {
    v6 = (unsigned int *)(v5 + 20);
    if ( *(_DWORD *)(v5 + 20) > 0x4000000u )
    {
      v9 = 6;
      goto LABEL_44;
    }
    v7 = operator new[](*(_DWORD *)(v5 + 20));
    v27 = v7;
    if ( sub_463EB0(v7, v5) == *v6 )
    {
      v8 = *v6;
      v5 = (int)v7;
LABEL_23:
      v18 = HIDWORD(a4);
      if ( !a4 )
        v18 = v8;
      if ( v18 && v18 <= v8 )
      {
        if ( (unsigned int)a4 + v18 > v8 )
        {
          v9 = 2;
        }
        else
        {
          if ( !v25 )
            memcpy_0(a1, (const void *)(a4 + v5), v18);
          if ( a2 )
            *a2 = v18;
          v9 = 0;
        }
      }
      else
      {
        v9 = 3;
      }
      goto LABEL_44;
    }
LABEL_5:
    v9 = 5;
    goto LABEL_44;
  }
  if ( sub_4A0D60() )
  {
    if ( sub_402030((const char *)v5) )
    {
      v19 = *(_DWORD *)(v5 + 20);
      if ( (_WORD)v19 == 24 )
        v20 = 4;
      else
        v20 = (unsigned __int16)v19 >> 3;
      v21 = (int)a1;
      v22 = v20 * (unsigned __int16)*(_DWORD *)(v5 + 16) * HIWORD(*(_DWORD *)(v5 + 16)) + 16;
      if ( a4 )
      {
        v27 = operator new[](v22);
        v21 = (int)v27;
      }
      else
      {
        v25 = 1;
      }
      if ( sub_469EE0(v5) )
        goto LABEL_5;
      v5 = v21;
      a3 = v22;
    }
LABEL_22:
    v8 = a3;
    goto LABEL_23;
  }
  v10 = *(_DWORD *)(v5 + 20);
  if ( (_WORD)v10 == 24 )
    v11 = 4;
  else
    v11 = (unsigned __int16)v10 >> 3;
  v12 = (int)a1;
  v13 = v11 * (unsigned __int16)*(_DWORD *)(v5 + 16) * HIWORD(*(_DWORD *)(v5 + 16)) + 16;
  if ( a4 )
  {
    v27 = operator new[](v13);
    v12 = (int)v27;
  }
  else
  {
    v25 = 1;
  }
  v24 = operator new(0x40u);
  v14 = 0;
  if ( v24 )
  {
    v15 = sub_490AE0();
    v14 = sub_4461E0(v16, v15);
  }
  v26 = (void (__thiscall ***)(_DWORD, int))v14;
  if ( sub_4A0F40(v12, v14) )
  {
    v17 = 0;
    v9 = 5;
  }
  else
  {
    v5 = v12;
    v17 = 1;
    a3 = v13;
    v9 = (int)v24;
  }
  if ( v26 )
    (**v26)(v26, 1);
  if ( v17 )
    goto LABEL_22;
LABEL_44:
  operator delete[](v27);
  return v9;
}

// ===== sub_465580 @ 0x00465580..0x004656BE =====
int __cdecl sub_465580(void *a1, size_t *a2, __int64 a3)
{
  const char *v3; // ecx
  const CHAR *v4; // esi
  DWORD v5; // eax
  DWORD v6; // esi
  void *v7; // edi
  int v9; // [esp+0h] [ebp-30h] BYREF
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-20h] BYREF
  int v11; // [esp+1Ch] [ebp-14h]
  int *v12; // [esp+20h] [ebp-10h]
  int v13; // [esp+2Ch] [ebp-4h]

  v12 = &v9;
  v4 = v3;
  if ( !sub_464B80(v3) )
    return 1;
  v13 = 0;
  sub_42D3B0(NumberOfBytesRead);
  LOBYTE(v13) = 1;
  if ( sub_42D520(v4, (int)NumberOfBytesRead) )
  {
    v5 = sub_42D650((int)NumberOfBytesRead);
    v6 = v5;
    if ( v5 > 0x4000000 )
    {
      v11 = 6;
      sub_42D5B0((int)NumberOfBytesRead);
    }
    else
    {
      v7 = operator new[](v5);
      if ( sub_42D5D0(v6, v7, (DWORD)NumberOfBytesRead) == v6 )
        v11 = sub_465320(a1, a2, v6, a3);
      else
        v11 = 5;
      operator delete[](v7);
      sub_42D5B0((int)NumberOfBytesRead);
    }
  }
  else
  {
    v11 = 1;
  }
  LOBYTE(v13) = 0;
  sub_42D400(NumberOfBytesRead);
  v13 = -1;
  return v11;
}

// ===== sub_4656C0 @ 0x004656C0..0x00465893 =====
int __cdecl sub_4656C0(void *a1, int a2, __int64 a3)
{
  const char *v3; // ecx
  int v4; // esi
  unsigned int v5; // eax
  int v6; // edi
  int result; // eax
  void *v8; // esi
  void *v9; // [esp+10h] [ebp-110h]
  void *v10; // [esp+14h] [ebp-10Ch] BYREF
  char Buffer[260]; // [esp+18h] [ebp-108h] BYREF

  v4 = (int)v3;
  v10 = a1;
  if ( strlen(v3) >= 0x60 )
  {
    sprintf(Buffer, &byte_4E6E08, v4, 95);
    sub_464520(Buffer, 0);
  }
  v5 = sub_406AF0((void *)dword_566754, a2, v4);
  v6 = v5;
  if ( v5 >= 0x80000000 )
    return -2147483616;
  if ( v5 > 0x4000000 )
    return -2147483552;
  v9 = operator new[](v5);
  if ( sub_406A50(v4, a2, (int)v9) == v6 )
  {
    switch ( sub_465320(v10, (size_t *)&v10, v6, a3) )
    {
      case 2:
        operator delete[](v9);
        result = -2147483600;
        break;
      case 3:
        operator delete[](v9);
        result = -2147483584;
        break;
      case 5:
        goto LABEL_9;
      case 6:
        operator delete[](v9);
        result = -2147483552;
        break;
      default:
        v8 = v10;
        operator delete[](v9);
        result = (int)v8;
        break;
    }
  }
  else
  {
LABEL_9:
    operator delete[](v9);
    return -2147483568;
  }
  return result;
}

// ===== sub_4658B0 @ 0x004658B0..0x004658C8 =====
int __usercall sub_4658B0@<eax>(void *a1@<ecx>, int a2@<eax>)
{
  return sub_4656C0(a1, a2, 0LL);
}

// ===== sub_4658D0 @ 0x004658D0..0x004659BB =====
int __usercall sub_4658D0@<eax>(size_t *a1@<edx>, const char *a2@<ecx>, _BYTE *a3@<edi>, void *a4, __int64 a5)
{
  const char *v6; // ecx
  int result; // eax
  const char **i; // esi
  char Buffer[788]; // [esp+14h] [ebp-318h] BYREF

  if ( sub_464980(a3) )
    return sub_465580(a4, a1, a5);
  sprintf(Buffer, "%s%s", v6, a3);
  result = sub_465580(a4, a1, a5);
  for ( i = dword_506BE0 != 0 ? (const char **)dword_566630 : 0; i; i = (const char **)i[1] )
  {
    if ( result != 1 )
      break;
    sprintf(Buffer, "%s%s\\%s", a2, *i, a3);
    result = sub_465580(a4, a1, a5);
  }
  return result;
}

// ===== sub_4659C0 @ 0x004659C0..0x004659E3 =====
size_t __usercall sub_4659C0@<eax>(void *a1@<eax>, const char *a2@<ecx>, _BYTE *a3@<edi>)
{
  int v3; // eax
  size_t v5; // [esp+4h] [ebp-4h] BYREF

  v3 = sub_4658D0(&v5, a2, a3, a1, 0LL);
  return v3 == 0 ? v5 : 0;
}

// ===== sub_4659F0 @ 0x004659F0..0x00465A11 =====
size_t __usercall sub_4659F0@<eax>(_BYTE *a1@<eax>, void *a2@<esi>)
{
  size_t result; // eax

  result = sub_4659C0(a2, &Buffer, a1);
  if ( !result )
    return sub_4659C0(a2, byte_517C08, a1);
  return result;
}

// ===== sub_465A20 @ 0x00465A20..0x00465AAC =====
void __cdecl sub_465A20(char *a1)
{
  int v1; // ecx
  bool v2; // zf
  const char *v3; // eax
  int pExceptionObject; // [esp+0h] [ebp-4h] BYREF

  pExceptionObject = v1;
  if ( !byte_517C08[0] )
    sub_464520(a1, 0);
  if ( sub_46BC80(hWndParent, byte_566320, 65) == 2 )
  {
    v2 = sub_46F710(pExceptionObject) == 0;
    v3 = (const char *)&unk_4E6DC4;
    if ( v2 )
      v3 = "Are you sure you want to quit?";
    if ( sub_46BC80(hWndParent, v3, 292) == 6 )
    {
      pExceptionObject = 0x7FFFFFFF;
      _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1H);
    }
  }
  Sleep(0xC8u);
}

// ===== sub_465AB0 @ 0x00465AB0..0x00465C27 =====
size_t __usercall sub_465AB0@<eax>(_BYTE *a1@<ecx>, _DWORD *a2@<esi>, const char *a3)
{
  size_t result; // eax
  unsigned int v5; // ebx
  char Buffer[1044]; // [esp+Ch] [ebp-418h] BYREF

  *a2 = 0;
  a2[1] = 0;
  a2[2] = 0;
  a2[3] = 0;
  result = sub_4659C0(a2, &::Buffer, a1);
  if ( a3 )
  {
    if ( !result )
    {
      sub_4649F0(a3, &::Buffer, Buffer);
      v5 = sub_4658B0(a2, (int)Buffer);
      while ( v5 == -2147483632 || v5 == -2147483616 )
      {
        if ( sub_464B80(byte_517C08) )
        {
          sub_4649F0(a3, byte_517C08, Buffer);
          v5 = sub_4658B0(a2, (int)Buffer);
        }
        if ( v5 == -2147483632 || v5 == -2147483616 )
        {
          sprintf(Buffer, &byte_4E6E70, a3, a1);
          sub_465A20(Buffer);
        }
      }
      if ( v5 < 0x80000000 )
        return v5;
      else
        return 0;
    }
  }
  else if ( !result )
  {
    while ( 1 )
    {
      result = sub_4659C0(a2, byte_517C08, a1);
      if ( result )
        break;
      sprintf(Buffer, &byte_4E6E44, a1);
      sub_465A20(Buffer);
    }
  }
  return result;
}

// ===== sub_465C30 @ 0x00465C30..0x00465DB6 =====
int __usercall sub_465C30@<eax>(_BYTE *a1@<ecx>, void *a2@<esi>, const char *a3, __int64 a4)
{
  int result; // eax
  int v6; // eax
  int v7; // [esp+Ch] [ebp-314h]
  char Buffer[780]; // [esp+10h] [ebp-310h] BYREF

  result = sub_4658D0(0, &::Buffer, a1, a2, a4);
  if ( result == 1 )
  {
    if ( a3 )
    {
      sub_4649F0(a3, &::Buffer, Buffer);
      v6 = sub_4656C0(a2, (int)Buffer, a4);
      v7 = v6;
      if ( v6 == -2147483632 || v6 == -2147483616 )
      {
        if ( sub_464B80(byte_517C08) )
        {
          sub_4649F0(a3, byte_517C08, Buffer);
          v6 = sub_4656C0(a2, (int)Buffer, a4);
        }
        else
        {
          v6 = v7;
        }
      }
      switch ( v6 )
      {
        case -2147483632:
        case -2147483616:
          result = 1;
          break;
        case -2147483600:
          result = 2;
          break;
        case -2147483584:
          result = 3;
          break;
        case -2147483568:
          result = 5;
          break;
        case -2147483552:
          result = 6;
          break;
        default:
          result = 0;
          break;
      }
    }
    else
    {
      return sub_4658D0(0, byte_517C08, a1, a2, a4);
    }
  }
  return result;
}

// ===== sub_465E30 @ 0x00465E30..0x00465F02 =====
int __cdecl sub_465E30(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
  int v2; // edi
  _BYTE *v3; // ecx
  char *v4; // ecx
  char *v5; // eax
  CHAR *v6; // edx
  char v7; // cl
  DWORD NumberOfBytesWritten[3]; // [esp+10h] [ebp-328h] BYREF
  CHAR FileName[780]; // [esp+1Ch] [ebp-31Ch] BYREF
  int v11; // [esp+334h] [ebp-4h]

  sub_42D3B0(NumberOfBytesWritten);
  v2 = 0;
  v11 = 0;
  if ( sub_464980(v3) )
  {
    v5 = v4;
    v6 = (CHAR *)(FileName - v4);
    do
    {
      v7 = *v5;
      v5[(_DWORD)v6] = *v5;
      ++v5;
    }
    while ( v7 );
  }
  else
  {
    sub_4649F0(v4, &Buffer, FileName);
  }
  if ( sub_42D570(FileName, (int)NumberOfBytesWritten, 0) )
  {
    v2 = sub_42D600(nNumberOfBytesToWrite, lpBuffer, (DWORD)NumberOfBytesWritten);
    sub_42D5B0((int)NumberOfBytesWritten);
  }
  v11 = -1;
  sub_42D400(NumberOfBytesWritten);
  return v2;
}

// ===== sub_465F10 @ 0x00465F10..0x00465F6A =====
int __usercall sub_465F10@<eax>(int a1@<esi>)
{
  signed int v1; // ebx
  signed int v2; // edi

  sub_46BB70();
  sub_4922C0();
  v1 = Size;
  v2 = 0;
  for ( *(_WORD *)(a1 + 56) = 0; v2 < v1; ++v2 )
  {
    *(_BYTE *)(v2 + a1 + 64) += sub_4922D0();
    *(_BYTE *)(a1 + 56) += *(_BYTE *)(v2 + a1 + 64);
    *(_BYTE *)(a1 + 57) ^= *(_BYTE *)(v2 + a1 + 64);
  }
  *(_BYTE *)(a1 + 58) = sub_4922D0();
  *(_BYTE *)(a1 + 59) = sub_4922D0();
  *(_BYTE *)(a1 + 60) = 1;
  return sub_46BB80();
}

// ===== sub_465F70 @ 0x00465F70..0x00465F8A =====
int __usercall sub_465F70@<eax>(char *Buffer@<ecx>, int a2@<eax>)
{
  return sprintf(Buffer, "%s%.4d%s", "BGI", a2, ".cad");
}

// ===== sub_465F90 @ 0x00465F90..0x00466075 =====
BOOL __cdecl sub_465F90(int a1)
{
  const char *v1; // ecx
  const char *v2; // edi
  DWORD v3; // ebx
  char *v4; // esi
  BOOL v5; // edi
  char v7[780]; // [esp+Ch] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+318h] [ebp-310h] BYREF

  v2 = v1;
  sub_465F70(Buffer, a1);
  v3 = Size + 64;
  v4 = (char *)operator new[](Size + 64);
  memset(v4, 0, 0x40u);
  GetLocalTime((LPSYSTEMTIME)v4);
  *((_DWORD *)v4 + 4) = 0;
  *((_DWORD *)v4 + 5) = 0;
  *((_DWORD *)v4 + 6) = 0;
  *((_DWORD *)v4 + 7) = 0;
  *((_DWORD *)v4 + 8) = 0;
  *((_DWORD *)v4 + 9) = 0;
  *((_DWORD *)v4 + 10) = 0;
  *((_DWORD *)v4 + 11) = 0;
  *((_DWORD *)v4 + 12) = 0;
  *((_DWORD *)v4 + 13) = 0;
  strcpy(v4 + 16, v2);
  memcpy_0(v4 + 64, dword_566758, Size);
  if ( dword_506BDC )
    sub_465F10((int)v4);
  sub_46B400(v7);
  v5 = sub_465E30(v4, v3) == v3;
  operator delete[](v4);
  return v5;
}

// ===== sub_466080 @ 0x00466080..0x004660E1 =====
BOOL __usercall sub_466080@<eax>(int a1@<edi>)
{
  char v1; // bl
  signed int v2; // esi
  char v3; // al
  char i; // [esp+Bh] [ebp-1h]

  sub_46BB70();
  sub_4922C0();
  v1 = 0;
  v2 = 0;
  for ( i = 0; v2 < (int)Size; ++v2 )
  {
    v3 = *(_BYTE *)(v2 + a1 + 64);
    i ^= v3;
    v1 += v3;
    *(_BYTE *)(v2 + a1 + 64) = v3 - sub_4922D0();
  }
  sub_46BB80();
  return *(_BYTE *)(a1 + 56) == v1 && *(_BYTE *)(a1 + 57) == i;
}

// ===== sub_4660F0 @ 0x004660F0..0x00466194 =====
int __cdecl sub_4660F0(int a1)
{
  void *v1; // ecx
  void *v2; // esi
  size_t v3; // eax
  char v5[780]; // [esp+8h] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+314h] [ebp-310h] BYREF

  v2 = v1;
  sub_465F70(Buffer, a1);
  sub_46B400(v5);
  v3 = sub_4659F0(v5, v2);
  if ( v3 != Size + 64 )
    return (v3 != 0) + 1;
  if ( !dword_506BDC || sub_466080((int)v2) )
    return 0;
  return 3;
}

// ===== sub_4661A0 @ 0x004661A0..0x004661F1 =====
int __cdecl sub_4661A0(int a1)
{
  char *v1; // esi
  int v2; // edi

  v1 = (char *)operator new[](Size + 64);
  v2 = sub_4660F0(a1);
  if ( !v2 )
    memcpy_0(dword_566758, v1 + 64, Size);
  operator delete[](v1);
  return v2;
}

// ===== sub_466200 @ 0x00466200..0x00466231 =====
int __cdecl sub_466200(int a1)
{
  void *v1; // esi
  int v2; // edi

  v1 = operator new[](Size + 64);
  v2 = sub_4660F0(a1);
  operator delete[](v1);
  return v2;
}

// ===== sub_466240 @ 0x00466240..0x004662DA =====
int __cdecl sub_466240(void *a1, int a2)
{
  size_t v2; // edi
  void *v3; // ebx
  size_t v4; // eax
  int v5; // esi
  char v7[780]; // [esp+10h] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+31Ch] [ebp-310h] BYREF

  sub_465F70(Buffer, a2);
  sub_46B400(v7);
  v2 = Size + 64;
  v3 = operator new[](Size + 64);
  v4 = sub_4659F0(v7, v3);
  if ( v4 == v2 )
  {
    qmemcpy(a1, v3, 0x40u);
    v5 = 0;
  }
  else
  {
    v5 = (v4 != 0) + 1;
  }
  operator delete[](v3);
  return v5;
}

// ===== sub_4662E0 @ 0x004662E0..0x004663F2 =====
size_t __cdecl sub_4662E0(_BYTE *a1)
{
  const char *v1; // ecx
  const char *v2; // edi
  _DWORD *v3; // eax
  void *v4; // esi
  size_t v5; // edi
  const char *v7; // [esp+10h] [ebp-31Ch]
  char Buffer[788]; // [esp+14h] [ebp-318h] BYREF

  v2 = v1;
  v7 = v1;
  v3 = operator new[](0x4000000u);
  v4 = v3;
  *v3 = 0;
  v3[1] = 0;
  v3[2] = 0;
  v3[3] = 0;
  if ( v2 )
  {
    v5 = sub_4659C0(v3, &::Buffer, a1);
    if ( v5 )
      goto LABEL_10;
    sub_4649F0(v7, &::Buffer, Buffer);
    v5 = sub_4658B0(v4, (int)Buffer);
    if ( v5 == -2147483632 )
    {
      if ( !sub_464B80(byte_517C08) )
        goto LABEL_9;
      sub_4649F0(v7, byte_517C08, Buffer);
      v5 = sub_4658B0(v4, (int)Buffer);
    }
    if ( v5 < 0x80000000 )
      goto LABEL_10;
LABEL_9:
    v5 = 0;
    goto LABEL_10;
  }
  v5 = sub_4659C0(v3, &::Buffer, a1);
  if ( !v5 )
    v5 = sub_4659C0(v4, byte_517C08, a1);
LABEL_10:
  operator delete[](v4);
  return v5;
}

// ===== sub_466400 @ 0x00466400..0x00466425 =====
BOOL __usercall sub_466400@<eax>(const CHAR *a1@<esi>)
{
  const char *v2; // [esp+0h] [ebp-8h]

  return sub_464B80(v2) && (GetFileAttributesA(a1) & 0x10) == 0;
}

// ===== sub_466430 @ 0x00466430..0x0046643C =====
int __usercall sub_466430@<eax>(int a1@<eax>)
{
  return sub_466440(0, a1);
}

// ===== sub_466440 @ 0x00466440..0x00466543 =====
BOOL __cdecl sub_466440(char *a1, const char *a2)
{
  const char *v2; // ecx
  const char *v3; // edi
  BOOL v4; // esi
  const char **v5; // edi
  const char *v7; // [esp+Ch] [ebp-318h]
  CHAR FileName[780]; // [esp+14h] [ebp-310h] BYREF

  v3 = v2;
  v7 = v2;
  v4 = 0;
  if ( sub_464B80(a2) )
  {
    sprintf(FileName, "%s%s", a2, v3);
    v4 = (GetFileAttributesA(FileName) & 0x10) == 0;
    v5 = dword_506BE0 != 0 ? (const char **)dword_566630 : 0;
    if ( v5 )
    {
      while ( !v4 )
      {
        if ( sub_464B80(a2) )
        {
          sprintf(FileName, "%s%s\\%s", a2, *v5, v7);
          v4 = (GetFileAttributesA(FileName) & 0x10) == 0;
        }
        v5 = (const char **)v5[1];
        if ( !v5 )
          goto LABEL_7;
      }
    }
    else
    {
LABEL_7:
      if ( !v4 )
        return v4;
    }
    if ( a1 )
      strcpy(a1, FileName);
  }
  return v4;
}

// ===== sub_466550 @ 0x00466550..0x004665BE =====
int __usercall sub_466550@<eax>(const char *a1@<edx>, int a2@<edi>)
{
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  if ( strlen(a1) >= 0x60 )
  {
    sprintf(Buffer, &byte_4E6E08, a1, 95);
    sub_464520(Buffer, 0);
  }
  return sub_406970((void *)dword_566754, a2, (int)a1);
}

// ===== sub_4665C0 @ 0x004665C0..0x004666BC =====
int __cdecl sub_4665C0(const char *a1)
{
  char *v1; // ecx
  const char *v2; // esi
  int result; // eax
  char Buffer[780]; // [esp+8h] [ebp-310h] BYREF

  v2 = v1;
  if ( a1 )
  {
    result = sub_466430((int)&::Buffer);
    if ( !result )
    {
      sub_4649F0(a1, &::Buffer, Buffer);
      result = sub_466550(v2, (int)Buffer);
      if ( !result )
      {
        if ( sub_464B80(byte_517C08) )
        {
          sub_4649F0(a1, byte_517C08, Buffer);
          return sub_466550(v2, (int)Buffer);
        }
        else
        {
          return 0;
        }
      }
    }
  }
  else if ( sub_464980(v1) )
  {
    return sub_466400(v2);
  }
  else
  {
    result = sub_466430((int)&::Buffer);
    if ( !result )
      return sub_466430((int)byte_517C08);
  }
  return result;
}

// ===== sub_4666C0 @ 0x004666C0..0x0046679F =====
int __usercall sub_4666C0@<eax>(char *a1@<edi>, const char *a2@<esi>)
{
  CHAR FileName[780]; // [esp+4h] [ebp-310h] BYREF

  sub_4649F0(a2, &Buffer, FileName);
  if ( GetFileAttributesA(FileName) == -1 )
  {
    if ( !sub_464B80(byte_517C08) )
      return 0;
    sub_4649F0(a2, byte_517C08, FileName);
    if ( GetFileAttributesA(FileName) == -1 )
    {
      return 0;
    }
    else
    {
      strcpy(a1, FileName);
      return 1;
    }
  }
  else
  {
    strcpy(a1, FileName);
    return 1;
  }
}

// ===== sub_4667A0 @ 0x004667A0..0x00466874 =====
BOOL __cdecl sub_4667A0(const char *a1, int a2)
{
  BOOL v2; // edi
  DWORD FileAttributesA; // eax
  bool v4; // zf
  const char *v5; // eax
  int v7; // [esp+0h] [ebp-31Ch]
  CHAR FileName[780]; // [esp+Ch] [ebp-310h] BYREF

  v2 = 0;
  while ( !v2 )
  {
    if ( sub_464B80(&Buffer) )
    {
      sub_4649F0(a1, &Buffer, FileName);
      FileAttributesA = GetFileAttributesA(FileName);
      if ( FileAttributesA != -1 )
      {
        v2 = (FileAttributesA & 0x10) == 0;
        if ( (FileAttributesA & 0x10) == 0 )
          continue;
      }
    }
    if ( sub_46BC80(hWndParent, a2, 65) == 2 )
    {
      v4 = sub_46F710(v7) == 0;
      v5 = (const char *)&unk_4E6DC4;
      if ( v4 )
        v5 = "Are you sure you want to quit?";
      if ( sub_46BC80(hWndParent, v5, 292) == 6 )
        break;
    }
  }
  return v2;
}

// ===== sub_466880 @ 0x00466880..0x00466919 =====
int __usercall sub_466880@<eax>(const char *a1@<edi>, const char **a2)
{
  _DWORD *v2; // esi
  int v3; // ebx
  _DWORD *v4; // eax

  v2 = 0;
  v3 = 0;
  v4 = operator new(0x348u);
  if ( v4 )
    v2 = sub_4455A0(v4);
  if ( !sub_445830(a1, (int)v2) || !sub_445880((int)v2, a2) || (v3 = sub_406BC0((_DWORD *)dword_566754, (int)v2)) == 0 )
  {
    if ( v2 )
      (*(void (__thiscall **)(_DWORD *, int))*v2)(v2, 1);
  }
  return v3;
}

// ===== sub_466920 @ 0x00466920..0x00466B0A =====
int __fastcall sub_466920(int a1, const CHAR *a2, void *a3, int a4, const char **a5, const CHAR *a6, int a7)
{
  char *v7; // edi
  const char **v8; // esi
  int v9; // eax
  unsigned int v10; // ecx
  unsigned int v11; // kr00_4
  bool v12; // zf
  int v13; // esi
  BOOL SaveFileNameA; // eax
  int v17; // [esp+Ch] [ebp-474h]
  int v18; // [esp+10h] [ebp-470h]
  tagOFNA v20; // [esp+20h] [ebp-460h] BYREF
  char Buffer[1028]; // [esp+78h] [ebp-408h] BYREF

  if ( a4 )
  {
    sub_45FFB0(1);
    sub_498770(1);
    memset(a3, 0, 0x30Cu);
    v7 = Buffer;
    Buffer[0] = 0;
    v8 = a5;
    v9 = a1 - (_DWORD)a5;
    v17 = a1 - (_DWORD)a5;
    v18 = a4;
    while ( 1 )
    {
      sprintf(v7, "%s %s", *v8, *(const char **)((char *)v8 + v9));
      v7[strlen(*v8)] = 0;
      v10 = strlen(*v8);
      v11 = strlen(*(const char **)((char *)v8++ + v17));
      v12 = v18-- == 1;
      v7 += v10 + v11 + 2;
      if ( v12 )
        break;
      v9 = v17;
    }
    *v7 = 0;
    memset(&v20, 0, sizeof(v20));
    v20.lpstrFile = (LPSTR)a3;
    v20.hwndOwner = hWndParent;
    v20.lpstrInitialDir = a6;
    v20.lpstrFilter = Buffer;
    v20.lStructSize = 88;
    v20.nFilterIndex = 1;
    v20.nMaxFile = 780;
    v20.lpstrTitle = a2;
    v20.Flags = (a7 != 1 ? 0 : 2) | (a7 != 0 ? 0 : 4096) | 0x804;
    v20.lpstrDefExt = MultiByteStr;
    if ( a7 )
    {
      if ( a7 != 1 )
      {
        v13 = 4;
LABEL_11:
        sub_4987C0();
        sub_45FFB0(0);
        return v13;
      }
      SaveFileNameA = GetSaveFileNameA(&v20);
    }
    else
    {
      SaveFileNameA = GetOpenFileNameA(&v20);
    }
    v13 = SaveFileNameA - 1;
    goto LABEL_11;
  }
  return 7;
}

// ===== sub_466B10 @ 0x00466B10..0x00466BA7 =====
int __usercall sub_466B10@<eax>(
        const CHAR *a1@<ecx>,
        const char *a2@<edi>,
        void *a3@<esi>,
        const char *a4,
        const CHAR *a5,
        int a6)
{
  int v8; // [esp+8h] [ebp-80Ch] BYREF
  char *v9; // [esp+Ch] [ebp-808h] BYREF
  char v10[1024]; // [esp+10h] [ebp-804h] BYREF
  char Buffer[1024]; // [esp+410h] [ebp-404h] BYREF

  sprintf(Buffer, "*.%s", a4);
  sprintf(v10, "%s(%s)", a2, Buffer);
  v8 = (int)v10;
  v9 = Buffer;
  return sub_466920((int)&v9, a5, a3, 1, (const char **)&v8, a1, a6);
}

// ===== sub_466BB0 @ 0x00466BB0..0x00466EFD =====
unsigned int __cdecl sub_466BB0(
        char *a1,
        _DWORD *a2,
        int a3,
        char *lpFileName,
        int a5,
        const char *a6,
        unsigned int a7)
{
  char *v7; // esi
  HANDLE FirstFileA; // edi
  int v9; // eax
  char v10; // cl
  size_t v11; // esi
  char *v12; // eax
  int v13; // edx
  char v14; // cl
  char *v15; // eax
  CHAR *v16; // edi
  HANDLE v17; // ebx
  char *v18; // eax
  int v19; // eax
  int v20; // eax
  void *v22; // [esp+10h] [ebp-A98h] BYREF
  unsigned int v23; // [esp+14h] [ebp-A94h]
  int v24; // [esp+18h] [ebp-A90h]
  int v25; // [esp+1Ch] [ebp-A8Ch]
  int v26; // [esp+20h] [ebp-A88h]
  char *Str; // [esp+24h] [ebp-A84h]
  const char *v28; // [esp+28h] [ebp-A80h]
  _DWORD *v29; // [esp+2Ch] [ebp-A7Ch]
  _WIN32_FIND_DATAA FindFileData; // [esp+30h] [ebp-A78h] BYREF
  CHAR FileName[784]; // [esp+170h] [ebp-938h] BYREF
  char Buffer[784]; // [esp+480h] [ebp-628h] BYREF
  char v33[788]; // [esp+790h] [ebp-318h] BYREF

  v7 = a1;
  v29 = a2;
  v28 = a6;
  Str = lpFileName;
  v26 = 0;
  v23 = 0;
  v25 = 0;
  FirstFileA = FindFirstFileA(lpFileName, &FindFileData);
  if ( FirstFileA == (HANDLE)-1 )
    goto LABEL_42;
  v22 = a1;
  v24 = a3;
  while ( (FindFileData.dwFileAttributes & 0x10) != 0 )
  {
LABEL_12:
    if ( a7 && v23 >= a7 || !FindNextFileA(FirstFileA, &FindFileData) )
      goto LABEL_17;
  }
  if ( v28 )
  {
    sprintf(Buffer, "%s%s", v28, FindFileData.cFileName);
  }
  else
  {
    v9 = 0;
    do
    {
      v10 = FindFileData.cFileName[v9];
      Buffer[v9++] = v10;
    }
    while ( v10 );
  }
  v11 = strlen(Buffer) + 1;
  if ( !v22 )
  {
LABEL_11:
    ++v23;
    v25 += v11;
    v7 = (char *)v22;
    goto LABEL_12;
  }
  if ( v11 <= v24 )
  {
    memcpy_0(v22, Buffer, v11);
    v22 = (char *)v22 + v11;
    v24 -= v11;
    goto LABEL_11;
  }
  v7 = (char *)v22;
  v26 = 1;
LABEL_17:
  FindClose(FirstFileA);
  if ( !a5 )
    goto LABEL_42;
  if ( v26 )
    goto LABEL_42;
  v12 = Str;
  v13 = FileName - Str;
  do
  {
    v14 = *v12;
    v12[v13] = *v12;
    ++v12;
  }
  while ( v14 );
  v15 = strrchr(FileName, 92);
  v16 = v15 ? v15 + 1 : FileName;
  *(_WORD *)v16 = 42;
  v17 = FindFirstFileA(FileName, &FindFileData);
  if ( v17 == (HANDLE)-1 )
    goto LABEL_42;
  while ( 2 )
  {
    if ( (FindFileData.dwFileAttributes & 0x10) == 0
      || !strcmp(FindFileData.cFileName, ".")
      || !strcmp(FindFileData.cFileName, "..") )
    {
LABEL_38:
      if ( !FindNextFileA(v17, &FindFileData) )
        goto LABEL_41;
      continue;
    }
    break;
  }
  v18 = strrchr(Str, 92);
  sprintf(v16, "%s%s", FindFileData.cFileName, v18);
  if ( v28 )
    sprintf(v33, "%s%s\\", v28, FindFileData.cFileName);
  else
    sprintf(v33, "%s\\", FindFileData.cFileName);
  if ( a7 )
    v19 = a7 - v23;
  else
    v19 = 0;
  v20 = sub_466BB0((int)v7, (int)&v22, v24, FileName, 1, (int)v33, v19);
  if ( v20 >= 0 )
  {
    if ( v7 )
    {
      v7 = &v7[(_DWORD)v22];
      v24 -= (int)v22;
    }
    v23 += v20;
    v25 += (int)v22;
    goto LABEL_38;
  }
  v26 = 1;
LABEL_41:
  FindClose(v17);
LABEL_42:
  if ( v29 )
  {
    if ( !v26 )
    {
      *v29 = v25;
      return v23;
    }
    return -1;
  }
  else
  {
    if ( v26 )
      return -1;
    return v23;
  }
}

// ===== sub_466F00 @ 0x00466F00..0x0046707F =====
unsigned int __cdecl sub_466F00(unsigned int a1, LPCSTR lpFileName, unsigned int a3)
{
  char *v3; // ecx
  char *v4; // edi
  HANDLE FirstFileA; // ebx
  unsigned int v6; // kr00_4
  unsigned int v7; // esi
  unsigned int result; // eax
  unsigned int v9; // [esp+Ch] [ebp-15Ch]
  int v10; // [esp+10h] [ebp-158h]
  int v12; // [esp+18h] [ebp-150h]
  char *v13; // [esp+1Ch] [ebp-14Ch]
  struct _WIN32_FIND_DATAA FindFileData; // [esp+20h] [ebp-148h] BYREF

  v4 = v3;
  v13 = v3;
  v12 = 0;
  v9 = 0;
  v10 = 0;
  FirstFileA = FindFirstFileA(lpFileName, &FindFileData);
  if ( FirstFileA == (HANDLE)-1 )
    goto LABEL_17;
  while ( (FindFileData.dwFileAttributes & 0x10) == 0
       || !strcmp(FindFileData.cFileName, ".")
       || !strcmp(FindFileData.cFileName, "..") )
  {
LABEL_9:
    if ( a3 && v9 >= a3 || !FindNextFileA(FirstFileA, &FindFileData) )
      goto LABEL_14;
  }
  v6 = strlen(FindFileData.cFileName);
  v7 = v6 + 1;
  if ( !v4 )
  {
LABEL_8:
    ++v9;
    v10 += v7;
    goto LABEL_9;
  }
  if ( v7 <= a1 )
  {
    memcpy_0(v4, FindFileData.cFileName, v6 + 1);
    v4 += v7;
    a1 -= v7;
    goto LABEL_8;
  }
  v12 = 1;
LABEL_14:
  FindClose(FirstFileA);
  if ( v12 )
    return -1;
  v4 = v13;
LABEL_17:
  result = v9;
  if ( !v4 )
    return v10;
  return result;
}

// ===== sub_467080 @ 0x00467080..0x004670D6 =====
int sub_467080()
{
  HMODULE ModuleHandleA; // eax
  BOOL (__stdcall *IsUserAnAdmin)(); // eax
  _BYTE v3[4]; // [esp+4h] [ebp-94h] BYREF
  unsigned int v4; // [esp+8h] [ebp-90h]
  int v5; // [esp+14h] [ebp-84h]

  sub_46F800(v3);
  if ( v5 == 2
    && v4 >= 5
    && (ModuleHandleA = GetModuleHandleA("shell32.dll"),
        (IsUserAnAdmin = GetProcAddress(ModuleHandleA, "IsUserAnAdmin")) != 0) )
  {
    return IsUserAnAdmin();
  }
  else
  {
    return 0;
  }
}

// ===== sub_4670E0 @ 0x004670E0..0x0046717B =====
HWND sub_4670E0()
{
  HWND result; // eax
  HWND v1; // esi
  BOOL v2; // edi
  BOOL v3; // edi
  HANDLE phNewToken; // [esp+0h] [ebp-Ch] BYREF
  HANDLE TokenHandle; // [esp+4h] [ebp-8h] BYREF
  DWORD dwProcessId; // [esp+8h] [ebp-4h] BYREF

  result = GetShellWindow();
  if ( result )
  {
    dwProcessId = 0;
    GetWindowThreadProcessId(result, &dwProcessId);
    result = (HWND)OpenProcess(0x2000000u, 0, dwProcessId);
    v1 = result;
    if ( result )
    {
      v2 = OpenProcessToken(result, 0x2000000u, &TokenHandle);
      CloseHandle(v1);
      if ( v2 )
      {
        v3 = DuplicateTokenEx(TokenHandle, 0x2000000u, 0, SecurityDelegation, TokenPrimary, &phNewToken);
        CloseHandle(TokenHandle);
        return v3 ? (HWND)phNewToken : 0;
      }
      else
      {
        return 0;
      }
    }
  }
  return result;
}

// ===== sub_467180 @ 0x00467180..0x004671A6 =====
HWND sub_467180()
{
  HWND result; // eax
  HWND v1; // esi

  result = sub_4670E0();
  v1 = result;
  if ( result )
  {
    if ( ImpersonateLoggedOnUser(result) )
    {
      return v1;
    }
    else
    {
      CloseHandle(v1);
      return 0;
    }
  }
  return result;
}

// ===== sub_4671B0 @ 0x004671B0..0x00467251 =====
int __thiscall sub_4671B0(void *this)
{
  int v2; // edi
  HANDLE CurrentProcess; // eax
  _LUID Luid; // [esp+8h] [ebp-20h] BYREF
  HANDLE TokenHandle; // [esp+10h] [ebp-18h] BYREF
  _TOKEN_PRIVILEGES NewState; // [esp+14h] [ebp-14h] BYREF

  v2 = 0;
  CurrentProcess = GetCurrentProcess();
  if ( OpenProcessToken(CurrentProcess, 0x20u, &TokenHandle) )
  {
    if ( LookupPrivilegeValueA(0, "SeDebugPrivilege", &Luid) )
    {
      NewState.Privileges[0].Luid = Luid;
      v2 = 1;
      NewState.PrivilegeCount = 1;
      NewState.Privileges[0].Attributes = this != 0 ? 2 : 0;
      if ( !AdjustTokenPrivileges(TokenHandle, 0, &NewState, 0x10u, 0, 0) || GetLastError() )
        v2 = 0;
    }
    CloseHandle(TokenHandle);
  }
  return v2;
}

// ===== sub_467260 @ 0x00467260..0x004674F9 =====
int __cdecl sub_467260(_BYTE *a1)
{
  HWND ShellWindow; // eax
  HANDLE v2; // ebx
  void *v3; // esi
  void *v4; // edi
  char *v5; // esi
  char v7; // al
  int v9; // [esp+0h] [ebp-78h] BYREF
  BOOL bOwnerDefaulted; // [esp+10h] [ebp-68h] BYREF
  int v11; // [esp+14h] [ebp-64h] BYREF
  int v12; // [esp+18h] [ebp-60h] BYREF
  _SID_NAME_USE peUse; // [esp+1Ch] [ebp-5Ch] BYREF
  int v14; // [esp+20h] [ebp-58h] BYREF
  int v15; // [esp+24h] [ebp-54h] BYREF
  int v16; // [esp+28h] [ebp-50h] BYREF
  int v17; // [esp+2Ch] [ebp-4Ch] BYREF
  int v18; // [esp+30h] [ebp-48h] BYREF
  int v19; // [esp+34h] [ebp-44h] BYREF
  int pExceptionObject; // [esp+38h] [ebp-40h] BYREF
  int v21; // [esp+3Ch] [ebp-3Ch]
  HANDLE hObject; // [esp+40h] [ebp-38h]
  void *v23; // [esp+44h] [ebp-34h]
  void *v24; // [esp+48h] [ebp-30h]
  void *v25; // [esp+4Ch] [ebp-2Ch]
  DWORD pSIRequested; // [esp+50h] [ebp-28h] BYREF
  DWORD dwProcessId; // [esp+54h] [ebp-24h] BYREF
  PSID pOwner; // [esp+58h] [ebp-20h] BYREF
  DWORD cchReferencedDomainName; // [esp+5Ch] [ebp-1Ch] BYREF
  DWORD cchName; // [esp+60h] [ebp-18h] BYREF
  DWORD nLengthNeeded[2]; // [esp+64h] [ebp-14h] BYREF
  int v32; // [esp+74h] [ebp-4h]

  nLengthNeeded[1] = (DWORD)&v9;
  v21 = 0;
  hObject = 0;
  v24 = 0;
  v23 = 0;
  v25 = 0;
  v32 = 0;
  ShellWindow = GetShellWindow();
  if ( !ShellWindow )
  {
    pExceptionObject = 1;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1H);
  }
  dwProcessId = 0;
  if ( !GetWindowThreadProcessId(ShellWindow, &dwProcessId) )
  {
    v19 = 1;
    _CxxThrowException(&v19, (_ThrowInfo *)&_TI1H);
  }
  v2 = OpenProcess(0x2000000u, 0, dwProcessId);
  hObject = v2;
  if ( !v2 )
  {
    v18 = 1;
    _CxxThrowException(&v18, (_ThrowInfo *)&_TI1H);
  }
  pSIRequested = 1;
  nLengthNeeded[0] = 0;
  GetUserObjectSecurity(v2, &pSIRequested, 0, 0, nLengthNeeded);
  if ( !nLengthNeeded[0] )
  {
    v17 = 1;
    _CxxThrowException(&v17, (_ThrowInfo *)&_TI1H);
  }
  v3 = operator new[](nLengthNeeded[0]);
  v25 = v3;
  memset(v3, 0, nLengthNeeded[0]);
  if ( !GetUserObjectSecurity(v2, &pSIRequested, v3, nLengthNeeded[0], nLengthNeeded) )
  {
    v16 = 1;
    _CxxThrowException(&v16, (_ThrowInfo *)&_TI1H);
  }
  if ( !GetSecurityDescriptorOwner(v3, &pOwner, &bOwnerDefaulted) )
  {
    v15 = 1;
    _CxxThrowException(&v15, (_ThrowInfo *)&_TI1H);
  }
  if ( !IsValidSid(pOwner) )
  {
    v14 = 1;
    _CxxThrowException(&v14, (_ThrowInfo *)&_TI1H);
  }
  cchReferencedDomainName = 0;
  cchName = 0;
  LookupAccountSidA(0, pOwner, 0, &cchName, 0, &cchReferencedDomainName, &peUse);
  if ( !cchReferencedDomainName || !cchName )
  {
    v11 = 1;
    _CxxThrowException(&v11, (_ThrowInfo *)&_TI1H);
  }
  v4 = operator new[](cchReferencedDomainName);
  v24 = v4;
  v5 = (char *)operator new[](cchName);
  v23 = v5;
  if ( !LookupAccountSidA(0, pOwner, v5, &cchName, (LPSTR)v4, &cchReferencedDomainName, &peUse) )
  {
    v12 = 1;
    _CxxThrowException(&v12, (_ThrowInfo *)&_TI1H);
  }
  do
  {
    v7 = *v5;
    *a1++ = *v5++;
  }
  while ( v7 );
  v21 = 1;
  v32 = -1;
  operator delete[](v25);
  operator delete[](v24);
  operator delete[](v23);
  if ( hObject )
    CloseHandle(hObject);
  return v21;
}

// ===== sub_467500 @ 0x00467500..0x0046756B =====
int __cdecl sub_467500(LPBYTE lpData)
{
  int v1; // esi
  DWORD Type; // [esp+4h] [ebp-Ch] BYREF
  DWORD cbData; // [esp+8h] [ebp-8h] BYREF
  HKEY phkResult; // [esp+Ch] [ebp-4h] BYREF

  v1 = 0;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, 0x20119u, &phkResult) )
  {
    cbData = 780;
    if ( RegQueryValueExA(phkResult, "ProgramFilesDir", 0, &Type, lpData, &cbData) || (v1 = 1, Type != 1) )
      v1 = 0;
    RegCloseKey(phkResult);
  }
  return v1;
}

// ===== sub_467570 @ 0x00467570..0x004677D1 =====
int __cdecl sub_467570(BYTE *pszPath)
{
  int v1; // ecx
  int v2; // edi
  int v3; // esi
  int v4; // ecx
  size_t v5; // kr00_4
  HANDLE v6; // esi
  LPITEMIDLIST v8; // [esp+Ch] [ebp-53Ch] BYREF
  int v9; // [esp+10h] [ebp-538h]
  LPITEMIDLIST ppidl; // [esp+14h] [ebp-534h] BYREF
  IMalloc *ppMalloc; // [esp+18h] [ebp-530h] BYREF
  HANDLE hObject; // [esp+1Ch] [ebp-52Ch]
  CHAR Str1[264]; // [esp+20h] [ebp-528h] BYREF
  char v14[264]; // [esp+128h] [ebp-420h] BYREF
  char Dir[264]; // [esp+230h] [ebp-318h] BYREF
  char v16[256]; // [esp+338h] [ebp-210h] BYREF
  char Drive[268]; // [esp+438h] [ebp-110h] BYREF

  v2 = v1;
  v3 = 0;
  if ( SHGetMalloc(&ppMalloc) >= 0 )
  {
    if ( v2 )
    {
      if ( v2 != 4 )
      {
        if ( v2 == 5 )
        {
          v3 = sub_467500(pszPath);
        }
        else
        {
          hObject = 0;
          v8 = 0;
          if ( sub_467080() )
          {
            hObject = sub_467180();
            v8 = (LPITEMIDLIST)(hObject == 0);
          }
          v4 = -1;
          switch ( v2 )
          {
            case 1:
              v4 = 16;
              break;
            case 2:
              v4 = 2;
              break;
            case 3:
              v4 = 5;
              break;
          }
          v9 = 0;
          if ( v4 >= 0 )
          {
            ppidl = 0;
            if ( !SHGetSpecialFolderLocation(hWndParent, v4, &ppidl) )
            {
              SHGetPathFromIDListA(ppidl, (LPSTR)pszPath);
              ppMalloc->lpVtbl->Free(ppMalloc, ppidl);
              v9 = 1;
            }
          }
          if ( v8 )
          {
            if ( v9 )
            {
              v8 = 0;
              if ( !SHGetSpecialFolderLocation(hWndParent, 40, &v8) )
              {
                SHGetPathFromIDListA(v8, Str1);
                ppMalloc->lpVtbl->Free(ppMalloc, v8);
                v5 = strlen(Str1);
                if ( !strncmp(Str1, (const char *)pszPath, v5) )
                {
                  if ( sub_4671B0((void *)1) )
                  {
                    if ( sub_467260(v16) )
                    {
                      strcpy(v14, (const char *)pszPath);
                      _splitpath(Str1, Drive, Dir, 0, 0);
                      sprintf((char *const)pszPath, "%s%s%s%s", Drive, Dir, v16, &v14[v5]);
                    }
                    sub_4671B0(0);
                  }
                }
              }
            }
          }
          v6 = hObject;
          if ( hObject )
          {
            RevertToSelf();
            CloseHandle(v6);
          }
          v3 = v9;
        }
        goto LABEL_31;
      }
      SHGetFolderPathA(hWndParent, 38, 0, 0, (LPSTR)pszPath);
    }
    else
    {
      GetWindowsDirectoryA((LPSTR)pszPath, 0x30Cu);
    }
    v3 = 1;
LABEL_31:
    ppMalloc->lpVtbl->Release(ppMalloc);
  }
  return v3;
}

// ===== sub_4677E0 @ 0x004677E0..0x00467917 =====
int __thiscall sub_4677E0(LPCSTR lpPathName)
{
  HWND v1; // ebx
  _DWORD *v3; // edi
  _DWORD *v4; // eax
  int v5; // esi
  int v6; // esi
  _DWORD v8[3]; // [esp+10h] [ebp-32Ch] BYREF
  int v9; // [esp+1Ch] [ebp-320h]
  CHAR TempFileName[780]; // [esp+20h] [ebp-31Ch] BYREF
  int v11; // [esp+338h] [ebp-4h]

  v1 = 0;
  v9 = 0;
  if ( sub_467080() )
    v1 = sub_467180();
  v3 = operator new(0xCu);
  *v3 = 0;
  v3[1] = 0;
  v3[2] = 0;
  if ( sub_46FB90(v3) && GetTempFileNameA(lpPathName, "BGI", 0, TempFileName) )
  {
    v4 = sub_42D3B0(v8);
    v11 = 0;
    v5 = (int)v4;
    if ( sub_42D570(TempFileName, (int)v4, 0) && (sub_42D5B0(v5), sub_42D520(TempFileName, v5)) )
    {
      sub_42D5B0(v5);
      v6 = 1;
    }
    else
    {
      v6 = v9;
    }
    DeleteFileA(TempFileName);
    v11 = -1;
    sub_42D400(v8);
  }
  else
  {
    v6 = v9;
  }
  sub_46FD80(v3);
  sub_46FDF0(v3);
  if ( v1 )
  {
    RevertToSelf();
    CloseHandle(v1);
  }
  return v6;
}

// ===== sub_467920 @ 0x00467920..0x00467A57 =====
int __cdecl sub_467920(LPARAM a1, const CHAR *a2, const CHAR *a3)
{
  _BYTE *v3; // ecx
  char *v4; // ecx
  char *v5; // eax
  char *v6; // edx
  char v7; // cl
  void *v8; // eax
  unsigned int v9; // edi
  char *v10; // esi
  int v11; // ebx
  BOOL v12; // esi
  void *v14; // [esp+0h] [ebp-31Ch] BYREF
  void *v15; // [esp+4h] [ebp-318h]
  unsigned int v16; // [esp+8h] [ebp-314h] BYREF
  char Buffer[780]; // [esp+Ch] [ebp-310h] BYREF

  if ( sub_464980(v3) )
  {
    v5 = v4;
    v6 = (char *)(Buffer - v4);
    do
    {
      v7 = *v5;
      v5[(_DWORD)v6] = *v5;
      ++v5;
    }
    while ( v7 );
  }
  else
  {
    sub_4649F0(v4, &::Buffer, Buffer);
  }
  if ( sub_406810((void *)dword_566754, &v14, &v16, (int)Buffer) )
    return 1;
  v8 = operator new[](96 * v16);
  v9 = 0;
  v15 = v8;
  v10 = (char *)v8;
  if ( v16 )
  {
    v11 = 0;
    do
    {
      sprintf(v10, "%s\n", (const char *)v14 + v11);
      ++v9;
      v10 += strlen(v10);
      v11 += 128;
    }
    while ( v9 < v16 );
    v8 = v15;
  }
  v12 = sub_45DD50(a1, a2, a3, v8) != 0;
  operator delete[](v15);
  operator delete[](v14);
  return v12 - 1;
}

// ===== sub_467A60 @ 0x00467A60..0x00467BA5 =====
int __fastcall sub_467A60(_BYTE *a1, const char *a2, unsigned int *a3)
{
  bool v4; // zf
  char *v5; // ecx
  char *v6; // eax
  char *v7; // edx
  char v8; // cl
  unsigned int v9; // eax
  unsigned int v10; // edi
  int v11; // ebx
  char *v12; // ecx
  char *v13; // edx
  char v14; // al
  unsigned int v15; // ebx
  unsigned int v16; // ecx
  const char *v17; // esi
  unsigned int v18; // kr00_4
  unsigned int v20; // [esp+4h] [ebp-31Ch] BYREF
  unsigned int *v21; // [esp+8h] [ebp-318h]
  void *v22; // [esp+Ch] [ebp-314h] BYREF
  char Buffer[780]; // [esp+10h] [ebp-310h] BYREF

  v21 = a3;
  v4 = !sub_464980(a1);
  v6 = v5;
  if ( v4 )
  {
    sub_4649F0(v5, &::Buffer, Buffer);
  }
  else
  {
    v7 = (char *)(Buffer - v5);
    do
    {
      v8 = *v6;
      v6[(_DWORD)v7] = *v6;
      ++v6;
    }
    while ( v8 );
  }
  if ( sub_406810((void *)dword_566754, &v22, &v20, (int)Buffer) )
    return 1;
  if ( a2 )
  {
    v9 = v20;
    v10 = 0;
    if ( v20 )
    {
      v11 = 0;
      do
      {
        v12 = (char *)v22 + v11;
        v13 = (char *)a2;
        do
        {
          v14 = *v12;
          *v13++ = *v12++;
        }
        while ( v14 );
        a2 += strlen(a2) + 1;
        v9 = v20;
        ++v10;
        v11 += 128;
      }
      while ( v10 < v20 );
    }
    *v21 = v9;
  }
  else
  {
    v15 = v20;
    v16 = 0;
    if ( v20 )
    {
      v17 = (const char *)v22;
      do
      {
        v18 = strlen(v17);
        v17 += 128;
        --v15;
        v16 += v18 + 1;
      }
      while ( v15 );
    }
    *v21 = v16;
  }
  operator delete[](v22);
  return (int)v21;
}

// ===== sub_467BB0 @ 0x00467BB0..0x00467CBB =====
int __usercall sub_467BB0@<eax>(int a1@<edi>, int a2@<esi>, char *a3, const char *a4)
{
  int v4; // ebx
  int result; // eax
  const char *v6; // [esp+8h] [ebp-620h] BYREF
  char v7[780]; // [esp+Ch] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+318h] [ebp-310h] BYREF

  v6 = a4;
  sub_4649F0(a4, &::Buffer, Buffer);
  v4 = sub_406910((void *)dword_566754, a2, (int)Buffer, a1);
  if ( v4 )
  {
LABEL_4:
    sub_4069D0((void *)dword_566754, (int)v7, (int)Buffer, a1);
    sub_4068B0((void *)dword_566754, &v6, (int)v7);
    *(_DWORD *)(a2 + 96) += v6;
    if ( a3 )
      strcpy(a3, v7);
    return v4;
  }
  if ( sub_464B80(byte_517C08) )
  {
    sub_4649F0(v6, byte_517C08, Buffer);
    result = sub_406910((void *)dword_566754, a2, (int)Buffer, a1);
    v4 = result;
    if ( !result )
      return result;
    goto LABEL_4;
  }
  return v4;
}

// ===== sub_467CC0 @ 0x00467CC0..0x00467DC6 =====
int __fastcall sub_467CC0(const CHAR *a1, void *a2, _DWORD *a3, unsigned int a4, DWORD nNumberOfBytesToRead)
{
  DWORD v7; // eax
  int v8; // eax
  int v9; // edi
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-18h] BYREF
  int v12; // [esp+24h] [ebp-4h]

  if ( sub_464B80(a1) )
  {
    sub_42D3B0(NumberOfBytesRead);
    v12 = 0;
    if ( !sub_42D520(a1, (int)NumberOfBytesRead) )
    {
      v9 = 1;
      goto LABEL_14;
    }
    v7 = sub_42D650((int)NumberOfBytesRead);
    if ( a2 )
    {
      if ( a4 >= v7 || !sub_42D630(a4, (int)NumberOfBytesRead) )
      {
        v9 = 2;
        sub_42D5B0((int)NumberOfBytesRead);
        goto LABEL_14;
      }
      v8 = sub_42D5D0(nNumberOfBytesToRead, a2, (DWORD)NumberOfBytesRead);
      v9 = nNumberOfBytesToRead != v8 ? 3 : 0;
      if ( a3 )
      {
        *a3 = v8;
        sub_42D5B0((int)NumberOfBytesRead);
LABEL_14:
        v12 = -1;
        sub_42D400(NumberOfBytesRead);
        return v9;
      }
    }
    else
    {
      if ( a3 )
        *a3 = v7;
      v9 = 0;
    }
    sub_42D5B0((int)NumberOfBytesRead);
    goto LABEL_14;
  }
  return 1;
}

// ===== sub_467DD0 @ 0x00467DD0..0x00467EBB =====
int __usercall sub_467DD0@<eax>(
        _DWORD *a1@<edx>,
        const char *a2@<ecx>,
        CHAR *a3@<edi>,
        void *a4,
        unsigned int a5,
        DWORD a6)
{
  const char *v7; // ecx
  int result; // eax
  const char **i; // esi
  char Buffer[780]; // [esp+10h] [ebp-310h] BYREF

  if ( sub_464980(a3) )
    return sub_467CC0(a3, a4, a1, a5, a6);
  sprintf(Buffer, "%s%s", v7, a3);
  result = sub_467CC0(Buffer, a4, a1, a5, a6);
  for ( i = dword_506BE0 != 0 ? (const char **)dword_566630 : 0; i; i = (const char **)i[1] )
  {
    if ( result != 1 )
      break;
    sprintf(Buffer, "%s%s\\%s", a2, *i, a3);
    result = sub_467CC0(Buffer, a4, a1, a5, a6);
  }
  return result;
}

// ===== sub_467EC0 @ 0x00467EC0..0x00467F4F =====
unsigned int __usercall sub_467EC0@<eax>(int a1@<edi>, const char *a2@<esi>, int a3, int a4, int a5)
{
  unsigned int result; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  if ( strlen(a2) >= 0x60 )
  {
    sprintf(Buffer, &byte_4E6E08, a2, 95);
    sub_464520(Buffer, 0);
  }
  result = sub_406AF0((void *)dword_566754, a1, (int)a2);
  if ( result < 0x80000000 )
  {
    if ( a3 )
      return sub_406A70((void *)dword_566754, a3, a1, (int)a2, a4, a5);
  }
  return result;
}

// ===== sub_467F50 @ 0x00467F50..0x004680F1 =====
int __fastcall sub_467F50(const char *a1, unsigned int *a2, void *a3, CHAR *a4, unsigned int a5, DWORD a6)
{
  int result; // eax
  unsigned int v7; // eax
  unsigned int v8; // edi
  char Buffer[780]; // [esp+14h] [ebp-310h] BYREF

  if ( a6 > 0x4000000 )
    return 3;
  result = sub_467DD0(a2, &::Buffer, a4, a3, a5, a6);
  if ( result == 1 )
  {
    if ( a1 )
    {
      sub_4649F0(a1, &::Buffer, Buffer);
      v7 = sub_467EC0((int)Buffer, a4, (int)a3, a5, a6);
      v8 = v7;
      if ( (v7 == -2147483632 || v7 == -2147483616) && sub_464B80(byte_517C08) )
      {
        sub_4649F0(a1, byte_517C08, Buffer);
        v8 = sub_467EC0((int)Buffer, a4, (int)a3, a5, a6);
      }
      switch ( v8 )
      {
        case 0x80000010:
        case 0x80000020:
          result = 1;
          break;
        case 0x80000030:
          result = 2;
          break;
        case 0x80000040:
          result = 3;
          break;
        default:
          if ( a2 )
            *a2 = v8;
          result = 0;
          break;
      }
    }
    else
    {
      return sub_467DD0(a2, byte_517C08, a4, a3, a5, a6);
    }
  }
  return result;
}
