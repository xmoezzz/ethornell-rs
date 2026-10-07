#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_45C0B0 @ 0x0045C0B0..0x0045C14B =====
int __thiscall sub_45C0B0(_DWORD *this, int a2)
{
  _DWORD *v2; // esi

  v2 = *(_DWORD **)(a2 + 68);
  this[89] = v2[13];
  this[90] = abs32(v2[14]);
  switch ( this[88] )
  {
    case 0:
      this[91] = 4 * this[89];
      break;
    case 1:
      this[91] = (3 * this[89] + 3) & 0xFFFFFFFC;
      break;
    case 2:
    case 3:
      this[91] = 2 * this[89];
      break;
    default:
      this[91] = 0;
      break;
  }
  this[92] = v2[14] >= 0;
  this[94] = v2[10];
  this[95] = v2[11];
  return 0;
}

// ===== sub_45C160 @ 0x0045C160..0x0045C16D =====
__int64 __thiscall sub_45C160(_QWORD *this)
{
  return this[47];
}

// ===== sub_45C170 @ 0x0045C170..0x0045C3CA =====
int __fastcall sub_45C170(int a1, int a2, _DWORD *a3, int **a4)
{
  int result; // eax
  unsigned int v5; // edi
  int *v6; // esi
  int *v7; // ebx
  int v8; // eax
  _DWORD *v9; // ecx
  int *v10; // eax
  unsigned int v11; // edx
  bool v12; // zf
  unsigned int v13; // edx
  _DWORD *v14; // eax
  int *v15; // ecx
  unsigned __int16 *v16; // eax
  int v17; // edx
  _WORD *v18; // edx
  BOOL *v19; // eax
  BOOL v20; // ecx
  _WORD *v21; // edx
  BOOL *v22; // eax
  BOOL v23; // ecx
  int *v24; // [esp+8h] [ebp-Ch]
  int *v25; // [esp+Ch] [ebp-8h]
  int *v26; // [esp+Ch] [ebp-8h]
  _DWORD *v27; // [esp+10h] [ebp-4h] BYREF

  result = -2147483647;
  if ( a4[4] != (int *)1 )
    return result;
  if ( a3[89] > (unsigned int)a4[2] )
  {
    v25 = a4[2];
    v5 = (unsigned int)v25;
  }
  else
  {
    v5 = a3[89];
    v25 = (int *)v5;
  }
  v6 = a4[3];
  if ( a3[90] <= (unsigned int)v6 )
    v6 = (int *)a3[90];
  v7 = *a4;
  (*(void (__stdcall **)(int, _DWORD **))(*(_DWORD *)a2 + 12))(a2, &v27);
  v8 = a3[88];
  switch ( v8 )
  {
    case 0:
      if ( v6 )
      {
        v9 = v27;
        v26 = v6;
        do
        {
          v10 = v7;
          if ( v5 )
          {
            v11 = v5;
            do
            {
              *v10++ = *v9++ & 0xFFFFFF;
              --v11;
            }
            while ( v11 );
            v9 = v27;
          }
          v9 = (_DWORD *)((char *)v9 + a3[91]);
          v7 = (int *)((char *)v7 + (_DWORD)a4[1]);
          v12 = v26 == (int *)1;
          v26 = (int *)((char *)v26 - 1);
          v27 = v9;
        }
        while ( !v12 );
        return 0;
      }
      return 0;
    case 1:
      if ( !v6 )
        return 0;
      v13 = v5 >> 2;
      v24 = v6;
      do
      {
        v14 = v27;
        v15 = v7;
        if ( v13 )
        {
          do
          {
            *v15 = *v14 & 0xFFFFFF;
            v15[1] = *((unsigned __int8 *)v14 + 3) | (*((unsigned __int16 *)v14 + 2) << 8);
            v15[2] = *((unsigned __int16 *)v14 + 3) | (*((unsigned __int8 *)v14 + 8) << 16);
            v15[3] = v14[2] >> 8;
            v15 += 4;
            v14 += 3;
            --v13;
          }
          while ( v13 );
          v13 = v5 >> 2;
        }
        if ( (v5 & 3) != 0 )
        {
          v16 = (unsigned __int16 *)((char *)v14 + 1);
          v17 = v5 & 3;
          do
          {
            *v15++ = *((unsigned __int8 *)v16 - 1) | (*v16 << 8);
            v16 = (unsigned __int16 *)((char *)v16 + 3);
            --v17;
          }
          while ( v17 );
          v13 = v5 >> 2;
        }
        v7 = (int *)((char *)v7 + (_DWORD)a4[1]);
        v27 = (_DWORD *)((char *)v27 + a3[91]);
        v24 = (int *)((char *)v24 - 1);
      }
      while ( v24 );
      return 0;
    case 2:
      if ( !v6 )
        return 0;
      v18 = v27;
      do
      {
        v19 = v7;
        if ( v5 )
        {
          do
          {
            v20 = (*v18 & 0x7FE0) != 0 || (*v18 & 0x1F) != 0;
            *v19++ = v20;
            ++v18;
            --v5;
          }
          while ( v5 );
          v18 = v27;
          v5 = (unsigned int)v25;
        }
        v18 = (_WORD *)((char *)v18 + a3[91]);
        v7 = (int *)((char *)v7 + (_DWORD)a4[1]);
        v6 = (int *)((char *)v6 - 1);
        v27 = v18;
      }
      while ( v6 );
      return 0;
    default:
      if ( v8 == 3 && v6 )
      {
        v21 = v27;
        do
        {
          v22 = v7;
          if ( v5 )
          {
            do
            {
              v23 = (*v21 & 0xFFE0) != 0 || (*v21 & 0x1F) != 0;
              *v22++ = v23;
              ++v21;
              --v5;
            }
            while ( v5 );
            v21 = v27;
            v5 = (unsigned int)v25;
          }
          v21 = (_WORD *)((char *)v21 + a3[91]);
          v7 = (int *)((char *)v7 + (_DWORD)a4[1]);
          v6 = (int *)((char *)v6 - 1);
          v27 = v21;
        }
        while ( v6 );
      }
      return 0;
  }
}

// ===== sub_45C3D0 @ 0x0045C3D0..0x0045C502 =====
int __thiscall sub_45C3D0(void *this)
{
  _DWORD *v1; // ebx
  int v2; // ecx
  void *v3; // eax
  unsigned int v4; // esi
  int v5; // edi
  int v6; // eax
  int v7; // eax
  char *v8; // edi
  int v10; // [esp-8h] [ebp-90h]
  int v11; // [esp-4h] [ebp-8Ch]
  _DWORD v12[11]; // [esp+Ch] [ebp-7Ch]
  unsigned int v13; // [esp+38h] [ebp-50h]
  char *Buffer; // [esp+3Ch] [ebp-4Ch]
  void *v15; // [esp+40h] [ebp-48h]
  _BYTE v16[64]; // [esp+44h] [ebp-44h] BYREF

  v13 = sub_4432F0((int)this, 0);
  v1 = operator new[](4 * v13);
  sub_4432F0(v2, (int)v1);
  v12[0] = "BG";
  v12[1] = "Map";
  v12[2] = "Sprite";
  v12[3] = "Window";
  v12[4] = "PrtclScrn";
  v12[5] = "RainScrn";
  v12[6] = "Effector";
  v12[7] = "Filter";
  v12[8] = "Virtual";
  v12[9] = "Group";
  v12[10] = "Knob";
  v3 = operator new[](v13 << 6);
  v4 = 0;
  v15 = v3;
  Buffer = (char *)v3;
  if ( v13 )
  {
    do
    {
      v5 = v1[v4];
      v6 = (*(int (**)(void))(*(_DWORD *)v5 + 88))();
      v10 = (*(int (__thiscall **)(int, int))(*(_DWORD *)v5 + 88))(v5, v6);
      v7 = sub_41B0B0(v1[v4]);
      v8 = Buffer;
      sprintf(Buffer, "%s - $%08X( %d )\n", (const char *)v12[v7], v10, v11);
      ++v4;
      Buffer = &v8[strlen(v8)];
    }
    while ( v4 < v13 );
    v3 = v15;
  }
  sub_45DD50(v16, "Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )", &unk_4E65C4, v3);
  operator delete[](v15);
  operator delete[](v1);
  return 0;
}

// ===== sub_45C510 @ 0x0045C510..0x0045C706 =====
int sub_45C510()
{
  _DWORD *v0; // ecx
  int v1; // eax
  unsigned int v2; // ebx
  _DWORD *v3; // edi
  char *v4; // esi
  int v5; // ebx
  int v6; // ecx
  int v7; // eax
  int v9; // [esp-10h] [ebp-88h]
  int v10; // [esp-Ch] [ebp-84h]
  int v11; // [esp-8h] [ebp-80h]
  int v12; // [esp-4h] [ebp-7Ch]
  _DWORD *v13; // [esp+Ch] [ebp-6Ch]
  int v14; // [esp+10h] [ebp-68h]
  unsigned int v15; // [esp+14h] [ebp-64h]
  char *v16; // [esp+18h] [ebp-60h]
  _DWORD v17[11]; // [esp+1Ch] [ebp-5Ch]
  _DWORD v18[7]; // [esp+48h] [ebp-30h] BYREF
  _DWORD v19[4]; // [esp+64h] [ebp-14h] BYREF

  v0 = dword_566808;
  v1 = dword_566824;
  v2 = 0;
  v3 = dword_566808;
  v13 = (_DWORD *)dword_566824;
  v15 = 0;
  if ( dword_566808 )
  {
    do
    {
      v0 = (_DWORD *)v0[6];
      ++v2;
    }
    while ( v0 );
    v15 = v2;
  }
  if ( dword_566824 )
  {
    do
    {
      v1 = *(_DWORD *)(v1 + 24);
      ++v2;
    }
    while ( v1 );
    v15 = v2;
  }
  v18[0] = -1;
  memset(&v18[1], 0, 24);
  v17[0] = "BG";
  v17[1] = "Map";
  v17[2] = "Sprite";
  v17[3] = "Window";
  v17[4] = "PrtclScrn";
  v17[5] = "RainScrn";
  v17[6] = "Effector";
  v17[7] = "Filter";
  v17[8] = "Virtual";
  v17[9] = "Group";
  v17[10] = "Knob";
  v4 = (char *)operator new[](v2 << 6);
  v16 = v4;
  v14 = 0;
  if ( v2 )
  {
    while ( 1 )
    {
      v5 = 0;
      if ( *v3 < *v13 )
      {
        sprintf(v4, "$%08X\n", *v13);
      }
      else
      {
        v6 = v3[5];
        if ( v6 )
        {
          (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v6 + 36))(v6, v19);
          v12 = v19[3];
          v11 = v19[2];
          v10 = v19[1];
          v9 = v19[0];
          v7 = sub_41B0B0(v3[5]);
          sprintf(v4, "$%08X - %s [ %d , %d - %d , %d ] \n", *v3, (const char *)v17[v7], v9, v10, v11, v12);
        }
        else
        {
          sprintf(v4, "$%08X\n", *v3);
        }
        if ( *v3 == *v13 )
        {
          v5 = 1;
          ++v14;
        }
        v3 = (_DWORD *)v3[6];
        if ( !v3 )
          v3 = v18;
        if ( !v5 )
          goto LABEL_19;
      }
      v13 = (_DWORD *)v13[6];
      if ( !v13 )
        v13 = v18;
LABEL_19:
      v4 += strlen(v4);
      if ( ++v14 >= v15 )
      {
        v4 = v16;
        break;
      }
    }
  }
  sub_45DD50(v4, "Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )", &unk_4E6660, v4);
  operator delete[](v4);
  return 0;
}

// ===== sub_45C710 @ 0x0045C710..0x0045C7F4 =====
int __cdecl sub_45C710(int a1)
{
  int v1; // edi
  char *v2; // esi
  int v3; // ebx
  int v4; // edi
  int v5; // edx
  const char *v6; // eax
  int v7; // eax
  char *v9; // [esp+Ch] [ebp-110h]
  void *v10; // [esp+14h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = (*(int (__stdcall **)(void **, int))(*(_DWORD *)a1 + 20))(&v10, 1);
  v9 = (char *)operator new[](v1 << 6);
  v2 = v9;
  v3 = v1;
  if ( v1 )
  {
    v4 = 16 * v1;
    do
    {
      v5 = *(_DWORD *)((char *)v10 + v4 - 8);
      v6 = *(const char **)((char *)v10 + v4 - 16);
      v4 -= 16;
      --v3;
      sprintf(v2, "%s - $%06X\n", v6, v5);
      v2 += strlen(v2);
    }
    while ( v3 );
  }
  v7 = sub_42D560(a1);
  sprintf(Buffer, "Thread [ %d ]", v7);
  sub_45DD50(v9, "Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )", Buffer, v9);
  operator delete[](v9);
  operator delete(v10);
  return 0;
}

// ===== sub_45C800 @ 0x0045C800..0x0045C862 =====
int __cdecl sub_45C800(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__thiscall *v2)(void *); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = (int (__thiscall *)(void *))funcs_45C84E[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4E6690, v1);
    sub_4646F0(a1);
  }
  return v2(v2);
}

// ===== sub_45C870 @ 0x0045C870..0x0045C880 =====
HWND sub_45C870()
{
  return sub_49A220() != 0 ? hWndParent : 0;
}

// ===== sub_45C880 @ 0x0045C880..0x0045C8A4 =====
int __usercall sub_45C880@<eax>(int a1@<esi>)
{
  int v1; // ecx
  int result; // eax
  int v3; // edx

  v1 = dword_565DC8;
  result = -1;
  v3 = 0;
  if ( dword_565DC8 )
  {
    while ( a1 != v1 )
    {
      v1 = dword_565DCC[v3++];
      if ( !v1 )
        return result;
    }
    return v3;
  }
  return result;
}

// ===== sub_45C8B0 @ 0x0045C8B0..0x0045C93C =====
LRESULT __stdcall sub_45C8B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  int v4; // eax
  int v5; // edi
  LRESULT (__stdcall *v6)(HWND, UINT, WPARAM, LPARAM); // edi

  v4 = sub_45C880((int)hWnd);
  v5 = v4;
  if ( v4 >= 0 )
  {
    if ( Msg == 258 && dword_565DA4[v4] )
    {
      if ( wParam == 45 )
      {
        if ( !GetWindowTextLengthA(hWnd) )
          goto LABEL_6;
        return 0;
      }
      if ( wParam != 8 && (wParam < 0x30 || wParam > 0x39) )
        return 0;
    }
LABEL_6:
    v6 = *(&dwNewLong + v5);
    if ( v6 )
      return CallWindowProcA(v6, hWnd, Msg, wParam, lParam);
  }
  return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

// ===== DialogFunc @ 0x0045C940..0x0045CB0F =====
INT_PTR __stdcall DialogFunc(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  HWND DlgItem; // eax
  HWND (__stdcall *v6)(HWND, int); // edi
  HWND v7; // eax
  const CHAR *v8; // eax
  int v9; // edx
  unsigned int v10; // eax
  signed int v11; // edi
  signed int v12; // ebx
  unsigned int v13; // kr00_4
  HWND v14; // eax
  HWND v15; // eax
  WNDPROC v16; // [esp-4h] [ebp-40Ch]
  CHAR String[1024]; // [esp+4h] [ebp-404h] BYREF

  if ( a2 == 16 )
    goto LABEL_7;
  if ( a2 != 272 )
  {
    if ( a2 != 273 )
      return 0;
    if ( (unsigned __int16)a3 == 1018 )
    {
      GetDlgItemTextA(hDlg, 1017, lpString, 256);
      EndDialog(hDlg, 1);
      return 1;
    }
    if ( (unsigned __int16)a3 != 1019 )
      return 1;
LABEL_7:
    v16 = dwNewLong;
    DlgItem = GetDlgItem(hDlg, 1017);
    SetWindowLongA(DlgItem, -4, (LONG)v16);
    EndDialog(hDlg, 0);
    return 1;
  }
  sub_472B90();
  v6 = GetDlgItem;
  dword_565DC8 = (int)GetDlgItem(hDlg, 1017);
  v7 = GetDlgItem(hDlg, 1017);
  dwNewLong = (WNDPROC)SetWindowLongA(v7, -4, (LONG)sub_45C8B0);
  v8 = dword_565DBC;
  if ( !dword_565DBC )
    v8 = (const CHAR *)&unk_4E66B8;
  SetWindowTextA(hDlg, v8);
  v9 = wParam;
  if ( (int)wParam > 0 )
  {
    SendDlgItemMessageA(hDlg, 1017, 0xC5u, wParam, 0);
    v9 = wParam;
  }
  if ( Src )
  {
    v10 = strlen((const char *)Src);
    v11 = v10;
    v12 = v9;
    if ( v9 <= 0 )
      v12 = v10;
    memset(String, 0, sizeof(String));
    if ( v11 > v12 )
      v11 = v12;
    memcpy_0(String, Src, v11);
    SetDlgItemTextA(hDlg, 1017, String);
    v13 = strlen(String);
    v14 = GetDlgItem(hDlg, 1017);
    SendMessageA(v14, 0xB1u, v13, v13);
    v6 = GetDlgItem;
  }
  v15 = v6(hDlg, 1017);
  SetFocus(v15);
  return 0;
}

// ===== sub_45CB10 @ 0x0045CB10..0x0045CBBC =====
INT_PTR __usercall sub_45CB10@<eax>(int a1@<eax>, CHAR *a2, const CHAR *a3, void *a4)
{
  int v5; // edi
  int v6; // eax
  INT_PTR v7; // esi
  HWND v9; // [esp-Ch] [ebp-14h]

  sub_45FFB0(1);
  sub_498770(1);
  v5 = sub_48ED40(1);
  lpString = a2;
  dword_565DBC = a3;
  Src = a4;
  if ( a1 < 0 )
  {
    a1 = -a1;
    dword_565DA4[0] = 1;
  }
  else
  {
    dword_565DA4[0] = 0;
  }
  wParam = a1;
  v9 = sub_45C870();
  v6 = sub_46F710();
  v7 = DialogBoxParamA(hInst, (LPCSTR)(v6 != 0 ? 113 : 124), v9, DialogFunc, 0);
  sub_48ED40(v5);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v7;
}

// ===== sub_45CBC0 @ 0x0045CBC0..0x0045CEBB =====
INT_PTR __stdcall sub_45CBC0(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  HWND v4; // eax
  HWND v5; // eax
  HWND DlgItem; // eax
  HWND v8; // eax
  const CHAR *v9; // eax
  LPCSTR v10; // eax
  HWND v11; // eax
  LPCSTR v12; // eax
  HWND v13; // eax
  int v14; // ecx
  unsigned int v15; // eax
  signed int v16; // ebx
  HWND v17; // eax
  signed int v18; // ebx
  unsigned int v19; // eax
  signed int v20; // edi
  int v21; // eax
  HWND v22; // eax
  WNDPROC v23; // [esp-4h] [ebp-818h]
  LONG v24; // [esp-4h] [ebp-818h]
  const CHAR *v25; // [esp-4h] [ebp-818h]
  const CHAR *v26; // [esp-4h] [ebp-818h]
  signed int v27; // [esp+Ch] [ebp-808h]
  int v28; // [esp+Ch] [ebp-808h]
  CHAR v29[1024]; // [esp+10h] [ebp-804h] BYREF
  CHAR String[1024]; // [esp+410h] [ebp-404h] BYREF

  switch ( a2 )
  {
    case 0x10u:
      goto LABEL_6;
    case 0x110u:
      sub_472B90();
      dword_565DC8 = (int)GetDlgItem(hDlg, 1020);
      dword_565DCC[0] = (int)GetDlgItem(hDlg, 1021);
      DlgItem = GetDlgItem(hDlg, 1020);
      dwNewLong = (WNDPROC)SetWindowLongA(DlgItem, -4, (LONG)sub_45C8B0);
      v8 = GetDlgItem(hDlg, 1021);
      dword_565DE0 = SetWindowLongA(v8, -4, (LONG)sub_45C8B0);
      v9 = dword_565DF4;
      if ( !dword_565DF4 )
        v9 = (const CHAR *)&unk_4E66C8;
      SetWindowTextA(hDlg, v9);
      v10 = dword_565DF8;
      if ( !dword_565DF8 )
        v10 = (LPCSTR)&unk_4E66D4;
      v25 = v10;
      v11 = GetDlgItem(hDlg, 1024);
      SetWindowTextA(v11, v25);
      v12 = dword_565E04;
      if ( !dword_565E04 )
        v12 = (LPCSTR)&unk_4E66E0;
      v26 = v12;
      v13 = GetDlgItem(hDlg, 1025);
      SetWindowTextA(v13, v26);
      v14 = dword_565E00;
      if ( (int)dword_565E00 > 0 )
      {
        SendDlgItemMessageA(hDlg, 1020, 0xC5u, dword_565E00, 0);
        v14 = dword_565E00;
      }
      if ( (int)dword_565E0C > 0 )
      {
        SendDlgItemMessageA(hDlg, 1021, 0xC5u, dword_565E0C, 0);
        v14 = dword_565E00;
      }
      if ( dword_565DFC )
      {
        v15 = strlen((const char *)dword_565DFC);
        v16 = v15;
        v27 = v14;
        if ( v14 <= 0 )
          v27 = v15;
        memset(String, 0, sizeof(String));
        if ( v16 > v27 )
          v16 = v27;
        memcpy_0(String, dword_565DFC, v16);
        SetDlgItemTextA(hDlg, 1020, String);
        v28 = 1;
      }
      else
      {
        v17 = GetDlgItem(hDlg, 1020);
        SetFocus(v17);
        v28 = 0;
      }
      if ( dword_565E08 )
      {
        v18 = dword_565E0C;
        v19 = strlen((const char *)dword_565E08);
        v20 = v19;
        if ( (int)dword_565E0C <= 0 )
          v18 = v19;
        memset(v29, 0, sizeof(v29));
        if ( v20 > v18 )
          v20 = v18;
        memcpy_0(v29, dword_565E08, v20);
        SetDlgItemTextA(hDlg, 1021, v29);
        v21 = 1;
      }
      else
      {
        if ( v28 )
        {
          v22 = GetDlgItem(hDlg, 1021);
          SetFocus(v22);
        }
        v21 = 0;
      }
      if ( v28 && v21 )
        return 1;
      break;
    case 0x111u:
      if ( (unsigned __int16)a3 == 1022 )
      {
        GetDlgItemTextA(hDlg, 1020, dword_565DEC, 256);
        GetDlgItemTextA(hDlg, 1021, dword_565DF0, 256);
        EndDialog(hDlg, 1);
        return 1;
      }
      if ( (unsigned __int16)a3 != 1023 )
        return 1;
LABEL_6:
      v23 = dwNewLong;
      v4 = GetDlgItem(hDlg, 1020);
      SetWindowLongA(v4, -4, (LONG)v23);
      v24 = dword_565DE0;
      v5 = GetDlgItem(hDlg, 1020);
      SetWindowLongA(v5, -4, v24);
      EndDialog(hDlg, 0);
      return 1;
  }
  return 0;
}

// ===== sub_45CEC0 @ 0x0045CEC0..0x0045CF8B =====
INT_PTR __usercall sub_45CEC0@<eax>(
        int a1@<eax>,
        CHAR *a2,
        CHAR *a3,
        const CHAR *a4,
        const CHAR *a5,
        void *a6,
        WPARAM a7,
        int a8,
        const CHAR *a9,
        void *a10,
        WPARAM a11,
        int a12)
{
  int v13; // edi
  HWND v14; // eax
  INT_PTR v15; // esi

  sub_45FFB0(1);
  sub_498770(1);
  v13 = sub_48ED40(1);
  dword_565DEC = a2;
  dword_565DF0 = a3;
  dword_565DF8 = a5;
  dword_565DF4 = a4;
  dword_565DFC = a6;
  dword_565DA4[0] = a8;
  dword_565E00 = a7;
  dword_565E04 = a9;
  dword_565E08 = a10;
  dword_565E0C = a11;
  dword_565DA8 = a12;
  v14 = sub_45C870();
  v15 = DialogBoxParamA(hInst, (LPCSTR)(a1 != 0 ? 126 : 114), v14, sub_45CBC0, 0);
  sub_48ED40(v13);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v15;
}

// ===== sub_45CF90 @ 0x0045CF90..0x0045CFB4 =====
int __usercall sub_45CF90@<eax>(HWND a1@<esi>)
{
  HWND v1; // ecx
  int result; // eax
  int v3; // edx

  v1 = dword_565E28;
  result = -1;
  v3 = 0;
  if ( dword_565E28 )
  {
    while ( a1 != v1 )
    {
      v1 = *(&hWnd + v3++);
      if ( !v1 )
        return result;
    }
    return v3;
  }
  return result;
}

// ===== sub_45CFC0 @ 0x0045CFC0..0x0045D104 =====
LRESULT __stdcall sub_45CFC0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  int v4; // edi
  bool v6; // cc
  HWND DlgItem; // eax
  LRESULT (__stdcall *v8)(HWND, UINT, WPARAM, LPARAM); // eax
  CHAR String[36]; // [esp+Ch] [ebp-28h] BYREF

  v4 = sub_45CF90(hWnd);
  if ( v4 < 0 )
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  if ( Msg == 258 )
  {
    if ( dword_565DB4 )
    {
      dword_565DB4 = 0;
      return 0;
    }
    if ( sub_42FA80(wParam) )
    {
      dword_565DB4 = 1;
      return 0;
    }
    if ( wParam != 8 )
    {
      if ( dword_565DA4[0] )
      {
        v6 = wParam - 48 <= 9;
      }
      else
      {
        if ( wParam >= 0x30 && wParam <= 0x39 || wParam >= 0x41 && wParam <= 0x5A )
        {
LABEL_17:
          if ( (int)dword_565E1C >= 1 && GetWindowTextA(hWnd, String, 33) >= (int)(dword_565E1C - 1) )
          {
            DlgItem = *(&::hWnd + v4);
            if ( !DlgItem )
              DlgItem = GetDlgItem(hDlg, 1053);
            SetFocus(DlgItem);
          }
          goto LABEL_22;
        }
        v6 = wParam - 97 <= 0x19;
      }
      if ( !v6 )
        return 0;
      goto LABEL_17;
    }
  }
LABEL_22:
  v8 = *(&lpPrevWndFunc + v4);
  if ( v8 )
    return CallWindowProcA(v8, hWnd, Msg, wParam, lParam);
  return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

// ===== sub_45D110 @ 0x0045D110..0x0045D39E =====
INT_PTR __stdcall sub_45D110(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  HWND *v4; // edi
  const CHAR *v6; // eax
  LPCSTR v7; // eax
  HWND DlgItem; // eax
  const CHAR *v9; // [esp-8h] [ebp-38h]
  CHAR String[36]; // [esp+8h] [ebp-28h] BYREF

  if ( a2 == 16 )
  {
    SetWindowLongA(dword_565E28, -4, (LONG)lpPrevWndFunc);
    SetWindowLongA(hWnd, -4, dword_565E40);
    SetWindowLongA(dword_565E30, -4, dword_565E44);
    SetWindowLongA(dword_565E34, -4, dword_565E48);
    EndDialog(hDlg, 0);
    return 1;
  }
  if ( a2 == 272 )
  {
    ::hDlg = hDlg;
    sub_472B90();
    dword_565E28 = GetDlgItem(hDlg, 1049);
    hWnd = GetDlgItem(hDlg, 1050);
    dword_565E30 = GetDlgItem(hDlg, 1051);
    dword_565E34 = GetDlgItem(hDlg, 1052);
    lpPrevWndFunc = (WNDPROC)SetWindowLongA(dword_565E28, -4, (LONG)sub_45CFC0);
    dword_565E40 = SetWindowLongA(hWnd, -4, (LONG)sub_45CFC0);
    dword_565E44 = SetWindowLongA(dword_565E30, -4, (LONG)sub_45CFC0);
    dword_565E48 = SetWindowLongA(dword_565E34, -4, (LONG)sub_45CFC0);
    v6 = dword_565E14;
    if ( !dword_565E14 )
      v6 = (const CHAR *)&unk_4E66F0;
    SetWindowTextA(hDlg, v6);
    v7 = dword_565E18;
    if ( !dword_565E18 )
      v7 = (LPCSTR)&unk_4E6708;
    v9 = v7;
    DlgItem = GetDlgItem(hDlg, 1055);
    SetWindowTextA(DlgItem, v9);
    if ( (int)dword_565E1C > 0 )
    {
      SendDlgItemMessageA(hDlg, 1049, 0xC5u, dword_565E1C, 0);
      SendDlgItemMessageA(hDlg, 1050, 0xC5u, dword_565E1C, 0);
      SendDlgItemMessageA(hDlg, 1051, 0xC5u, dword_565E1C, 0);
      SendDlgItemMessageA(hDlg, 1052, 0xC5u, dword_565E1C, 0);
    }
    SetFocus(dword_565E28);
    return 0;
  }
  if ( a2 != 273 )
    return 0;
  if ( (unsigned __int16)a3 == 1053 )
  {
    *(_BYTE *)dword_565E10 = 0;
    v4 = &dword_565E28;
    do
    {
      GetWindowTextA(*v4, String, 33);
      sprintf((char *const)(dword_565E10 + strlen((const char *)dword_565E10)), "%s-", String);
      ++v4;
    }
    while ( (int)v4 < (int)&dword_565E38 );
    *(_BYTE *)(strlen((const char *)dword_565E10) + dword_565E10 - 1) = 0;
    EndDialog(hDlg, 1);
  }
  else if ( (unsigned __int16)a3 == 1054 )
  {
    EndDialog(hDlg, 0);
  }
  return 1;
}

// ===== sub_45D3A0 @ 0x0045D3A0..0x0045D46B =====
INT_PTR __usercall sub_45D3A0@<eax>(int a1@<eax>, int a2, const CHAR *a3, const CHAR *a4, int a5)
{
  int v6; // edi
  HWND v7; // eax
  INT_PTR v8; // esi

  sub_45FFB0(1);
  sub_498770(1);
  v6 = sub_48ED40(1);
  dword_565E10 = a2;
  dword_565E14 = a3;
  dword_565E18 = a4;
  if ( a1 < 0 )
  {
    dword_565E1C = -a1;
    if ( -a1 > 32 )
      dword_565E1C = 32;
    dword_565DA4[0] = 1;
  }
  else
  {
    dword_565E1C = a1;
    if ( a1 > 32 )
      dword_565E1C = 32;
    dword_565DA4[0] = 0;
  }
  dword_565E20 = a5;
  v7 = sub_45C870();
  v8 = DialogBoxParamA(hInst, (LPCSTR)0x7D, v7, sub_45D110, 0);
  sub_48ED40(v6);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v8;
}

// ===== sub_45D470 @ 0x0045D470..0x0045D4FC =====
int __usercall sub_45D470@<eax>(int a1@<edx>, char *a2@<esi>, HWND hDlg)
{
  CHAR *v3; // ecx
  int v4; // ecx
  char v5; // al
  CHAR String[1024]; // [esp+4h] [ebp-404h] BYREF

  GetDlgItemTextA(hDlg, a1, String, 11);
  v3 = String;
  if ( String[0] )
  {
    while ( sub_495970(v3) )
    {
      v5 = *(_BYTE *)(v4 + 2);
      v3 = (CHAR *)(v4 + 2);
      if ( !v5 )
        goto LABEL_4;
    }
    return 0;
  }
  else
  {
LABEL_4:
    strcpy(a2, String);
    return 1;
  }
}

// ===== sub_45D500 @ 0x0045D500..0x0045DA9B =====
INT_PTR __stdcall sub_45D500(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  int v4; // esi
  int v5; // eax
  int v6; // esi
  int v8; // ebx
  int v9; // eax
  int v10; // ebx
  int v11; // eax
  int v12; // ebx
  int v13; // eax
  size_t v14; // ebx
  int v15; // eax
  size_t v16; // ebx
  int v17; // eax
  size_t v18; // ebx
  int v19; // eax
  size_t v20; // ebx
  HWND DlgItem; // eax
  _DWORD v22[4]; // [esp+Ch] [ebp-46Ch]
  _DWORD v23[4]; // [esp+1Ch] [ebp-45Ch]
  LRESULT v24; // [esp+2Ch] [ebp-44Ch]
  int v25; // [esp+30h] [ebp-448h]
  _DWORD v26[12]; // [esp+34h] [ebp-444h]
  _DWORD v27[4]; // [esp+64h] [ebp-414h]
  CHAR lParam[1024]; // [esp+74h] [ebp-404h] BYREF

  v23[0] = dword_565E4C;
  v23[3] = dword_565E58;
  v23[1] = dword_565E50;
  v23[2] = dword_565E54;
  v27[0] = 1031;
  v27[1] = 1040;
  v27[2] = 1043;
  v27[3] = 1042;
  v22[0] = &unk_4E672C;
  v22[1] = &unk_4E6730;
  v22[2] = &unk_4E6734;
  v22[3] = &unk_4E6744;
  if ( a2 == 272 )
  {
    sub_472B90();
    v9 = 0;
    do
    {
      v10 = v9 + 1;
      sprintf(lParam, "%d", v9 + 1);
      SendDlgItemMessageA(hDlg, 1033, 0x143u, 0, (LPARAM)lParam);
      v9 = v10;
    }
    while ( v10 < 12 );
    v11 = 0;
    do
    {
      v12 = v11 + 1;
      sprintf(lParam, "%d", v11 + 1);
      SendDlgItemMessageA(hDlg, 1034, 0x143u, 0, (LPARAM)lParam);
      v11 = v12;
    }
    while ( v12 < 31 );
    SendDlgItemMessageA(hDlg, 1031, 0xC5u, 0xAu, 0);
    v13 = strlen((const char *)dword_565E4C);
    if ( v13 > 0 )
    {
      v14 = v13;
      if ( v13 > 10 )
        v14 = 10;
      memset(lParam, 0, sizeof(lParam));
      memcpy_0(lParam, dword_565E4C, v14);
      SetDlgItemTextA(hDlg, 1031, lParam);
      SendDlgItemMessageA(hDlg, 1031, 0xB1u, 0, -1);
    }
    SendDlgItemMessageA(hDlg, 1040, 0xC5u, 0xAu, 0);
    v15 = strlen((const char *)dword_565E50);
    if ( v15 > 0 )
    {
      v16 = v15;
      if ( v15 > 10 )
        v16 = 10;
      memset(lParam, 0, sizeof(lParam));
      memcpy_0(lParam, dword_565E50, v16);
      SetDlgItemTextA(hDlg, 1040, lParam);
      SendDlgItemMessageA(hDlg, 1040, 0xB1u, 0, -1);
    }
    SendDlgItemMessageA(hDlg, 1043, 0xC5u, 0xAu, 0);
    v17 = strlen((const char *)dword_565E54);
    if ( v17 > 0 )
    {
      v18 = v17;
      if ( v17 > 10 )
        v18 = 10;
      memset(lParam, 0, sizeof(lParam));
      memcpy_0(lParam, dword_565E54, v18);
      SetDlgItemTextA(hDlg, 1043, lParam);
      SendDlgItemMessageA(hDlg, 1043, 0xB1u, 0, -1);
    }
    SendDlgItemMessageA(hDlg, 1042, 0xC5u, 0xAu, 0);
    v19 = strlen((const char *)dword_565E58);
    if ( v19 > 0 )
    {
      v20 = v19;
      if ( v19 > 10 )
        v20 = 10;
      memset(lParam, 0, sizeof(lParam));
      memcpy_0(lParam, dword_565E58, v20);
      SetDlgItemTextA(hDlg, 1042, lParam);
      SendDlgItemMessageA(hDlg, 1042, 0xB1u, 0, -1);
    }
    SendDlgItemMessageA(hDlg, 1033, 0x14Eu, *(_DWORD *)dword_565E5C, 0);
    SendDlgItemMessageA(hDlg, 1034, 0x14Eu, *(_DWORD *)dword_565E60, 0);
    DlgItem = GetDlgItem(hDlg, 1);
    SetFocus(DlgItem);
    return 0;
  }
  if ( a2 != 273 )
    return 0;
  if ( (unsigned __int16)a3 == 1 )
  {
    v8 = 0;
    while ( sub_45D470(v27[v8], (char *)v23[v8], hDlg) )
    {
      if ( ++v8 >= 4 )
      {
        *(_DWORD *)dword_565E5C = SendDlgItemMessageA(hDlg, 1033, 0x147u, 0, 0);
        *(_DWORD *)dword_565E60 = SendDlgItemMessageA(hDlg, 1034, 0x147u, 0, 0);
        EndDialog(hDlg, 1);
        return 1;
      }
    }
    sprintf(lParam, &byte_4E674C, v22[v8]);
    sub_46BC80(hDlg, lParam, 16);
    return 1;
  }
  if ( (unsigned __int16)a3 != 1033 || HIWORD(a3) != 9 )
    return 1;
  v26[0] = 31;
  v26[1] = 29;
  v26[2] = 31;
  v26[3] = 30;
  v26[4] = 31;
  v26[5] = 30;
  v26[6] = 31;
  v26[7] = 31;
  v26[8] = 30;
  v26[9] = 31;
  v26[10] = 30;
  v26[11] = 31;
  v4 = v26[SendDlgItemMessageA(hDlg, 1033, 0x147u, 0, 0)];
  v25 = v4;
  v24 = SendDlgItemMessageA(hDlg, 1034, 0x147u, 0, 0);
  SendDlgItemMessageA(hDlg, 1034, 0x14Bu, 0, 0);
  v5 = 0;
  if ( v4 > 0 )
  {
    do
    {
      v6 = v5 + 1;
      sprintf(lParam, "%d", v5 + 1);
      SendDlgItemMessageA(hDlg, 1034, 0x143u, 0, (LPARAM)lParam);
      v5 = v6;
    }
    while ( v6 < v25 );
    v4 = v25;
  }
  SendDlgItemMessageA(hDlg, 1034, 0x14Eu, v24 >= v4 ? 0 : v24, 0);
  return 1;
}

// ===== sub_45DAA0 @ 0x0045DAA0..0x0045DB34 =====
INT_PTR __cdecl sub_45DAA0(void *a1, void *a2, void *a3, void *a4, int a5, int a6)
{
  int v6; // esi
  HWND v7; // eax
  INT_PTR v8; // edi

  sub_45FFB0(1);
  sub_498770(1);
  v6 = sub_48ED40(1);
  dword_565E4C = a1;
  dword_565E50 = a2;
  dword_565E54 = a3;
  dword_565E58 = a4;
  dword_565E5C = a5;
  dword_565E60 = a6;
  v7 = sub_45C870();
  v8 = DialogBoxParamA(hInst, (LPCSTR)0x77, v7, sub_45D500, 0);
  sub_48ED40(v6);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v8;
}

// ===== sub_45DB40 @ 0x0045DB40..0x0045DD4A =====
INT_PTR __stdcall sub_45DB40(HWND hDlg, UINT a2, WPARAM a3, LPARAM a4)
{
  unsigned __int16 v5; // ax
  int v6; // eax
  LRESULT v7; // eax
  const CHAR *v8; // eax
  const CHAR *v9; // eax
  _BYTE *v10; // ebx
  char i; // al
  signed int v12; // edi
  int v13; // edx
  HWND DlgItem; // eax
  _BYTE lParam[780]; // [esp+4h] [ebp-310h] BYREF

  switch ( a2 )
  {
    case 0x10u:
      EndDialog(hDlg, 0);
      return 1;
    case 0x110u:
      sub_472B90();
      v8 = dword_565E68;
      if ( !dword_565E68 )
        v8 = (const CHAR *)&unk_4E6780;
      SetWindowTextA(hDlg, v8);
      v9 = dword_565E6C;
      if ( !dword_565E6C )
        v9 = (const CHAR *)&unk_4E6790;
      SetDlgItemTextA(hDlg, 1045, v9);
      v10 = dword_565E70;
      for ( i = *(_BYTE *)dword_565E70; i; v10 += v13 )
      {
        v12 = 0;
        if ( i )
        {
          do
          {
            if ( i == 10 )
              break;
            i = v10[++v12];
          }
          while ( i );
          if ( v12 > 0 )
          {
            memcpy_0(lParam, v10, v12);
            lParam[v12] = 0;
            SendDlgItemMessageA(hDlg, 1044, 0x181u, 0xFFFFFFFF, (LPARAM)lParam);
          }
        }
        v13 = v12 + (v10[v12] == 10);
        i = v10[v13];
      }
      SendDlgItemMessageA(hDlg, 1044, 0x186u, 0, 0);
      DlgItem = GetDlgItem(hDlg, 1044);
      SetFocus(DlgItem);
      return 0;
    case 0x111u:
      v5 = a3;
      if ( HIWORD(a3) == 2 && (_WORD)a3 == 1044 )
        v5 = 1046;
      v6 = v5 - 1046;
      if ( v6 )
      {
        if ( v6 == 1 )
        {
          EndDialog(hDlg, 0);
          return 1;
        }
      }
      else
      {
        v7 = SendDlgItemMessageA(hDlg, 1044, 0x188u, 0, 0);
        if ( v7 != -1 )
        {
          SendDlgItemMessageA(hDlg, 1044, 0x189u, v7, ::lParam);
          EndDialog(hDlg, 1);
          return 1;
        }
        *(_BYTE *)::lParam = 0;
        EndDialog(hDlg, 1);
      }
      return 1;
    default:
      return 0;
  }
}

// ===== sub_45DD50 @ 0x0045DD50..0x0045DDD3 =====
INT_PTR __cdecl sub_45DD50(LPARAM a1, const CHAR *a2, const CHAR *a3, void *a4)
{
  int v4; // esi
  HWND v5; // eax
  INT_PTR v6; // edi

  sub_45FFB0(1);
  sub_498770(1);
  v4 = sub_48ED40(1);
  lParam = a1;
  dword_565E68 = a2;
  dword_565E6C = a3;
  dword_565E70 = a4;
  v5 = sub_45C870();
  v6 = DialogBoxParamA(hInst, (LPCSTR)0x79, v5, sub_45DB40, 0);
  sub_48ED40(v4);
  sub_46DA20();
  sub_4987C0();
  sub_45FFB0(0);
  return v6;
}

// ===== sub_45DDE0 @ 0x0045DDE0..0x0045DDFC =====
void **__thiscall sub_45DDE0(void *this)
{
  void **result; // eax

  result = (void **)dword_565E8C;
  if ( dword_565E8C )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[6];
    }
    while ( result );
  }
  return result;
}

// ===== sub_45DE00 @ 0x0045DE00..0x0045DE1D =====
_DWORD *__thiscall sub_45DE00(void *this)
{
  _DWORD *result; // eax

  result = dword_565E8C;
  if ( dword_565E8C )
  {
    do
    {
      if ( this == (void *)result[2] )
        break;
      result = (_DWORD *)result[6];
    }
    while ( result );
  }
  return result;
}

// ===== sub_45DE20 @ 0x0045DE20..0x0045DE6F =====
int __cdecl sub_45DE20(int a1, int a2)
{
  void *v2; // ecx
  int v3; // edx
  _DWORD *v4; // esi
  _DWORD *v5; // eax
  int v6; // ecx

  v4 = sub_45DE00(v2);
  if ( !v4 )
    return v3;
  v5 = operator new(0xCu);
  *v5 = a1;
  v5[1] = a2;
  v5[2] = 0;
  v6 = v4[5];
  if ( v6 )
  {
    *(_DWORD *)(v6 + 8) = v5;
  }
  else
  {
    v4[4] = v5;
    v4[5] = v5;
  }
  return 0;
}

// ===== sub_45DE70 @ 0x0045DE70..0x0045DECC =====
int __thiscall sub_45DE70(void *this)
{
  void **v1; // eax
  _DWORD *v2; // edx
  void **v3; // esi
  _DWORD *v4; // eax

  v1 = sub_45DDE0(this);
  v3 = v1;
  if ( !v1 )
    return 0x80000000;
  v4 = v1[4];
  if ( !v4 )
    return 1;
  if ( v2 )
  {
    *v2 = *v4;
    v2[1] = v4[1];
    v2[2] = 0;
  }
  v3[4] = (void *)v4[2];
  operator delete(v4);
  if ( !v3[4] )
    v3[5] = 0;
  return 0;
}

// ===== sub_45DED0 @ 0x0045DED0..0x0045E28E =====
INT_PTR __stdcall sub_45DED0(HWND hDlg, UINT a2, WPARAM a3, HWND hWnd)
{
  INT_PTR result; // eax
  _DWORD *v5; // eax
  int DlgCtrlID; // eax
  int v7; // esi
  LRESULT v8; // eax

  if ( a2 > 0x111 )
  {
    if ( a2 == 276 )
    {
      if ( (_WORD)a3 == 5 || (_WORD)a3 == 8 )
      {
        DlgCtrlID = GetDlgCtrlID(hWnd);
        if ( DlgCtrlID )
        {
          switch ( DlgCtrlID )
          {
            case 1062:
              v7 = 0;
              goto LABEL_34;
            case 1063:
              v7 = 1;
              goto LABEL_34;
            case 1064:
              v7 = 2;
              goto LABEL_34;
            case 1065:
              v7 = 3;
              goto LABEL_34;
            case 1066:
              v7 = 4;
LABEL_34:
              v8 = SendDlgItemMessageA(hDlg, DlgCtrlID, 0x400u, 0, 0);
              sub_45DE20(v7, v8);
              break;
            default:
              return 1;
          }
        }
      }
    }
    else
    {
      if ( a2 != 33024 )
        return 0;
      if ( !a3 )
      {
        SendDlgItemMessageA(hDlg, 1062, 0x405u, 1u, *(_DWORD *)hWnd);
        SendDlgItemMessageA(hDlg, 1063, 0x405u, 1u, *((_DWORD *)hWnd + 1));
        SendDlgItemMessageA(hDlg, 1064, 0x405u, 1u, *((_DWORD *)hWnd + 2));
        SendDlgItemMessageA(hDlg, 1065, 0x405u, 1u, *((_DWORD *)hWnd + 3));
        SendDlgItemMessageA(hDlg, 1066, 0x405u, 1u, *((_DWORD *)hWnd + 4));
        SendDlgItemMessageA(hDlg, 1067, 0xF1u, *((_DWORD *)hWnd + 5) == 0, 0);
        SendDlgItemMessageA(hDlg, 1068, 0xF1u, *((_DWORD *)hWnd + 5) == 1, 0);
        SendDlgItemMessageA(hDlg, 1073, 0xF1u, *((_DWORD *)hWnd + 6) == 0, 0);
        SendDlgItemMessageA(hDlg, 1074, 0xF1u, *((_DWORD *)hWnd + 6) == 1, 0);
        SendDlgItemMessageA(hDlg, 1075, 0xF1u, *((_DWORD *)hWnd + 7) == 1, 0);
        SendDlgItemMessageA(hDlg, 1076, 0xF1u, *((_DWORD *)hWnd + 7) == 0, 0);
        SendDlgItemMessageA(hDlg, 1077, 0xF1u, *((_DWORD *)hWnd + 8) == 1, 0);
        SendDlgItemMessageA(hDlg, 1078, 0xF1u, *((_DWORD *)hWnd + 8) == 0, 0);
        return 1;
      }
    }
    return 1;
  }
  else if ( a2 == 273 )
  {
    switch ( (__int16)a3 )
    {
      case 1067:
        sub_45DE20(5, 0);
        result = 1;
        break;
      case 1068:
        sub_45DE20(5, 1);
        result = 1;
        break;
      case 1069:
        SendMessageA(hDlg, 0x10u, 0, 0);
        result = 1;
        break;
      case 1073:
        sub_45DE20(6, 0);
        result = 1;
        break;
      case 1074:
        sub_45DE20(6, 1);
        result = 1;
        break;
      case 1075:
        sub_45DE20(7, 1);
        result = 1;
        break;
      case 1076:
        sub_45DE20(7, 0);
        result = 1;
        break;
      case 1077:
        sub_45DE20(8, 1);
        result = 1;
        break;
      case 1078:
        sub_45DE20(8, 0);
        result = 1;
        break;
      default:
        return 1;
    }
  }
  else
  {
    if ( a2 != 16 )
    {
      if ( a2 == 272 )
      {
        sub_472B90();
        SendDlgItemMessageA(hDlg, 1062, 0x406u, 0, 0x800000);
        SendDlgItemMessageA(hDlg, 1063, 0x406u, 0, 0x800000);
        SendDlgItemMessageA(hDlg, 1064, 0x406u, 0, 0x800000);
        SendDlgItemMessageA(hDlg, 1065, 0x406u, 0, 0x800000);
        SendDlgItemMessageA(hDlg, 1066, 0x406u, 0, 0x800000);
      }
      return 0;
    }
    sub_45DE20(-1, 0);
    v5 = sub_45DE00(hDlg);
    if ( v5 )
    {
      v5[2] = 0;
      if ( v5[3] )
      {
        v5[3] = 0;
        sub_45FFB0(0);
      }
    }
    EndDialog(hDlg, 0);
    return 1;
  }
  return result;
}

// ===== sub_45E2E0 @ 0x0045E2E0..0x0045E301 =====
_DWORD *sub_45E2E0()
{
  _DWORD *result; // eax

  for ( result = dword_565E8C; dword_565E8C; result = dword_565E8C )
    sub_45E3B0(*result);
  return result;
}

// ===== sub_45E310 @ 0x0045E310..0x0045E3A5 =====
int __cdecl sub_45E310(_DWORD *a1, int a2, LPARAM lParam)
{
  HWND v4; // eax
  HWND DialogParamA; // eax
  HWND v6; // esi
  _DWORD *v7; // eax

  if ( a2 )
    return -2147483647;
  v4 = sub_45C870();
  DialogParamA = CreateDialogParamA(hInst, (LPCSTR)0x7F, v4, (DLGPROC)sub_45DED0, 0);
  v6 = DialogParamA;
  if ( !DialogParamA )
    return -1;
  SendMessageA(DialogParamA, 0x8100u, 0, lParam);
  v7 = operator new(0x1Cu);
  *v7 = ++dword_565E74;
  v7[1] = 0;
  v7[2] = v6;
  v7[3] = 0;
  v7[4] = 0;
  v7[5] = 0;
  v7[6] = dword_565E8C;
  dword_565E8C = v7;
  *a1 = *v7;
  return 0;
}

// ===== sub_45E3B0 @ 0x0045E3B0..0x0045E411 =====
unsigned int __thiscall sub_45E3B0(void *this)
{
  void *v1; // esi
  unsigned int result; // eax
  int *v3; // edi
  HWND v4; // eax

  v1 = dword_565E8C;
  result = 0x80000000;
  v3 = &dword_565E74;
  if ( dword_565E8C )
  {
    while ( this != *(void **)v1 )
    {
      v3 = (int *)v1;
      v1 = (void *)*((_DWORD *)v1 + 6);
      if ( !v1 )
        return result;
    }
    v4 = (HWND)*((_DWORD *)v1 + 2);
    if ( v4 )
      SendMessageA(v4, 0x10u, 0, 0);
    while ( !sub_45DE70(*(void **)v1) )
      ;
    v3[6] = *((_DWORD *)v1 + 6);
    operator delete(v1);
    return 0;
  }
  return result;
}

// ===== sub_45E420 @ 0x0045E420..0x0045E46C =====
int __usercall sub_45E420@<eax>(void *a1@<eax>, void *a2@<ecx>)
{
  void **v3; // eax
  int v4; // edx
  void **v5; // edi

  v3 = sub_45DDE0(a2);
  v5 = v3;
  if ( !v3 )
    return v4;
  if ( a1 )
  {
    if ( v3[3] )
      return v4;
  }
  else if ( !v3[3] )
  {
    return v4;
  }
  v3[3] = a1;
  sub_45FFB0(a1);
  ShowWindow((HWND)v5[2], a1 != 0 ? 8 : 0);
  return 0;
}

// ===== sub_45E470 @ 0x0045E470..0x0045E476 =====
int sub_45E470()
{
  return dword_565E90;
}

// ===== sub_45E480 @ 0x0045E480..0x0045E48F =====
int sub_45E480()
{
  int result; // eax

  result = dword_5177B8;
  if ( !dword_5177B8 )
    return 60;
  return result;
}

// ===== sub_45E490 @ 0x0045E490..0x0045E4C6 =====
int __usercall sub_45E490@<eax>(int a1@<eax>, _DWORD *a2@<esi>)
{
  int result; // eax

  sub_495B30(a1);
  *a2 = (unsigned __int16)word_517706;
  a2[1] = (unsigned __int16)word_517704;
  result = (unsigned __int16)word_517702;
  a2[2] = (unsigned __int16)word_517702;
  a2[3] = (unsigned __int16)word_517700;
  return result;
}

// ===== sub_45E4D0 @ 0x0045E4D0..0x0045E4EA =====
HMONITOR sub_45E4D0()
{
  int v0; // eax

  v0 = sub_4610E0();
  return MonitorFromWindow(hWndParent, (v0 != 0) + 1);
}

// ===== sub_45E4F0 @ 0x0045E4F0..0x0045E54E =====
int sub_45E4F0()
{
  HMONITOR v0; // ebx
  unsigned int v1; // edi
  int v2; // ecx
  int v3; // esi

  v0 = sub_45E4D0();
  v1 = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565E9C + 16))(dword_565E9C);
  if ( sub_45E850(0) != 1 )
    return v2;
  v3 = 0;
  if ( !v1 )
    return v2;
  while ( (HMONITOR)(*(int (__stdcall **)(int, int))(*(_DWORD *)dword_565E9C + 60))(dword_565E9C, v3) != v0 )
  {
    if ( ++v3 >= v1 )
      return 0;
  }
  return v3;
}

// ===== sub_45E550 @ 0x0045E550..0x0045E580 =====
BOOL __usercall sub_45E550@<eax>(int *a1@<eax>)
{
  int v1; // esi
  int v2; // eax
  int *v4; // [esp-8h] [ebp-8h]

  if ( !a1 )
    a1 = &dword_5177B0;
  v1 = *(_DWORD *)dword_565E9C;
  v4 = a1;
  v2 = sub_45E4F0();
  return (*(int (__stdcall **)(int, int, int *))(v1 + 32))(dword_565E9C, v2, v4) >= 0;
}

// ===== sub_45E580 @ 0x0045E580..0x0045E5E7 =====
unsigned int __usercall sub_45E580@<eax>(unsigned int *a1@<edi>, int a2)
{
  unsigned int v2; // ecx
  unsigned int v3; // ecx
  unsigned int result; // eax

  if ( a2 )
  {
    v2 = dword_5177B0;
    if ( 4 * dword_5177B0 / (unsigned int)dword_5177B4 >= 0xA )
      v2 = (unsigned int)dword_5177B0 >> 1;
    *a1 = v2;
    v3 = dword_5177B4;
    result = dword_5177B4 / (unsigned int)dword_5177B0;
    if ( dword_5177B4 / (unsigned int)dword_5177B0 )
    {
      result = 100 * dword_5177B4 / (unsigned int)dword_5177B0;
      if ( result < 0x7D )
        v3 = (unsigned int)dword_5177B4 >> 1;
    }
    a1[1] = v3;
  }
  else
  {
    result = dword_5177B0;
    *a1 = dword_5177B0;
    a1[1] = dword_5177B4;
  }
  return result;
}

// ===== sub_45E5F0 @ 0x0045E5F0..0x0045E5F6 =====
int sub_45E5F0()
{
  return dword_5177B4;
}

// ===== sub_45E600 @ 0x0045E600..0x0045E610 =====
int sub_45E600()
{
  int v0; // eax
  int *v1; // ecx
  int result; // eax
  int v3; // ecx

  v0 = sub_4610A0();
  *v1 = v0;
  result = sub_4610C0();
  *(_DWORD *)(v3 + 4) = result;
  return result;
}

// ===== sub_45E610 @ 0x0045E610..0x0045E737 =====
int __usercall sub_45E610@<eax>(_DWORD *a1@<edi>, unsigned int *a2@<esi>)
{
  long double v2; // st7
  int v3; // ecx
  BOOL v4; // eax
  double v5; // st7
  bool v6; // sf
  int v7; // ecx
  unsigned int v8; // eax
  double v9; // st5
  double v10; // st7
  int v11; // edx
  long double v12; // st7
  long double v13; // st7
  int v14; // ecx
  BOOL v15; // eax
  double v16; // st7
  int v17; // ecx
  unsigned int v18; // eax
  double v19; // st5
  double v20; // st7
  int result; // eax
  double v22; // [esp+0h] [ebp-18h]

  v22 = log(2.0);
  v2 = log((double)*a2) / v22;
  v3 = (int)v2;
  v4 = v2 - (double)(int)v2 > 0.0;
  v5 = 2.0;
  v6 = v4 + v3 < 0;
  v7 = v4 + v3;
  v8 = v7;
  if ( v6 )
    v8 = -v7;
  v9 = 1.0;
  while ( 1 )
  {
    if ( (v8 & 1) != 0 )
      v9 = v9 * v5;
    v8 >>= 1;
    if ( !v8 )
      break;
    v5 = v5 * v5;
  }
  v10 = v9;
  if ( v7 < 0 )
    v10 = 1.0 / v9;
  v11 = a2[1];
  *a1 = (__int64)v10;
  v12 = (double)(int)a2[1];
  if ( v11 < 0 )
    v12 = v12 + 4294967296.0;
  v13 = log(v12) / v22;
  v14 = (int)v13;
  v15 = v13 - (double)(int)v13 > 0.0;
  v16 = 2.0;
  v6 = v15 + v14 < 0;
  v17 = v15 + v14;
  v18 = v17;
  if ( v6 )
    v18 = -v17;
  v19 = 1.0;
  while ( 1 )
  {
    if ( (v18 & 1) != 0 )
      v19 = v19 * v16;
    v18 >>= 1;
    if ( !v18 )
      break;
    v16 = v16 * v16;
  }
  v20 = v19;
  if ( v17 < 0 )
    v20 = 1.0 / v19;
  result = (__int64)v20;
  a1[1] = result;
  return result;
}

// ===== sub_45E740 @ 0x0045E740..0x0045E74C =====
int __usercall sub_45E740@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_51772C = result;
  dword_517730 = a2;
  return result;
}

// ===== sub_45E750 @ 0x0045E750..0x0045E762 =====
_DWORD *__usercall sub_45E750@<eax>(_DWORD *result@<eax>)
{
  int v1; // edx

  v1 = dword_517730;
  *result = dword_51772C;
  result[1] = v1;
  return result;
}

// ===== sub_45E770 @ 0x0045E770..0x0045E7BC =====
int __usercall sub_45E770@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax
  int v3; // eax
  int v4; // ecx
  int result; // eax

  if ( a1 && a2 )
  {
    sub_45E740(a1, a2);
  }
  else
  {
    v2 = sub_4610C0();
    v3 = sub_4610A0(v2);
    sub_45E740(v3, v4);
  }
  sub_461080();
  result = sub_45F640();
  if ( !result )
    return sub_461110(dword_506B88, dword_518604, 0);
  return result;
}

// ===== sub_45E7C0 @ 0x0045E7C0..0x0045E822 =====
BOOL sub_45E7C0()
{
  int v1[4]; // [esp+4h] [ebp-14h] BYREF

  return dword_565E9C && sub_45E550(v1) && (v1[0] != dword_5177B0 || v1[1] != dword_5177B4);
}

// ===== sub_45E830 @ 0x0045E830..0x0045E843 =====
int __fastcall sub_45E830(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    dword_50721C = a1;
    return 1;
  }
  return result;
}

// ===== sub_45E850 @ 0x0045E850..0x0045E856 =====
int sub_45E850()
{
  return dword_50721C;
}

// ===== sub_45E860 @ 0x0045E860..0x0045E873 =====
int __fastcall sub_45E860(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 2 )
  {
    dword_565E98 = a1;
    return 1;
  }
  return result;
}

// ===== sub_45E880 @ 0x0045E880..0x0045E8C8 =====
int sub_45E880()
{
  int v0; // esi
  unsigned int v2; // [esp+8h] [ebp-10h]
  unsigned int v3; // [esp+Ch] [ebp-Ch]
  unsigned int v4[2]; // [esp+10h] [ebp-8h] BYREF

  v0 = dword_565E98;
  if ( dword_565E98 == 2 && ((sub_45E600(), sub_45E580(v4, 1), v4[0] < v2) || v4[1] < v3) )
    return 0;
  else
    return v0;
}

// ===== sub_45E8D0 @ 0x0045E8D0..0x0045EB57 =====
int *__cdecl sub_45E8D0(int *a1, int a2, int a3, int a4)
{
  int v4; // ebx
  double v5; // st7
  bool v6; // sf
  double v7; // st6
  int v8; // eax
  int v9; // eax
  int *result; // eax
  unsigned int v11; // edx
  double v12; // st7
  double v13; // st7
  double v14; // st6
  double v15; // st5
  double v16; // st4
  unsigned int v17; // [esp+Ch] [ebp-14h]
  int v18; // [esp+10h] [ebp-10h]
  unsigned int v19; // [esp+14h] [ebp-Ch] BYREF
  int v20; // [esp+18h] [ebp-8h]
  int v21; // [esp+1Ch] [ebp-4h]
  unsigned int v22; // [esp+30h] [ebp+10h]
  int v23; // [esp+30h] [ebp+10h]

  sub_45E600();
  if ( !sub_45F640() )
  {
    sub_45E750(&v19);
LABEL_3:
    if ( !a4 )
    {
      v4 = (int)((double)v19 * (double)a2 / (double)v17);
      v5 = (double)(unsigned int)v20 * (double)a3;
      v6 = v18 < 0;
      v7 = (double)v18;
      goto LABEL_14;
    }
    if ( a4 == 1 )
    {
      v4 = (int)((double)v17 * (double)a2 / (double)v19);
      v5 = (double)(unsigned int)v18 * (double)a3;
      v6 = v20 < 0;
      v7 = (double)v20;
LABEL_14:
      if ( v6 )
        v7 = v7 + 4294967296.0;
      v12 = v5 / v7;
      goto LABEL_17;
    }
    goto LABEL_24;
  }
  sub_45E580(&v19, 1);
  v8 = sub_45E880();
  if ( v8 )
  {
    v9 = v8 - 1;
    if ( !v9 )
      goto LABEL_3;
    if ( v9 == 1 )
    {
      if ( !a4 )
      {
        result = a1;
        v11 = (unsigned int)(v20 - v18) >> 1;
        *a1 = ((v19 - v17) >> 1) + a2;
        a1[1] = v11 + a3;
        return result;
      }
      if ( a4 == 1 )
      {
        v22 = a3 - ((unsigned int)(v20 - v18) >> 1);
        result = a1;
        *a1 = a2 - ((v19 - v17) >> 1);
        a1[1] = v22;
        return result;
      }
    }
  }
  else
  {
    v13 = (double)v17;
    v14 = (double)v19 / v13;
    v15 = (double)(unsigned int)v18;
    v21 = v20;
    v16 = (double)(unsigned int)v20 / v15;
    if ( v16 < v14 )
      v14 = v16;
    if ( !a4 )
    {
      v21 = (int)(v19 - (int)(v13 * v14)) >> 1;
      v4 = (int)((double)v21 + (double)a2 * v14);
      v12 = v14 * (double)a3 + (double)((v20 - (int)(v15 * v14)) >> 1);
LABEL_17:
      result = a1;
      *a1 = v4;
      a1[1] = (int)v12;
      return result;
    }
    if ( a4 == 1 )
    {
      v23 = (int)((double)(a3 - ((v20 - (int)(v15 * v14)) >> 1)) / v14);
      result = a1;
      *a1 = (int)((double)(a2 - ((int)(v19 - (int)(v13 * v14)) >> 1)) / v14);
      a1[1] = v23;
      return result;
    }
  }
LABEL_24:
  result = a1;
  *a1 = a2;
  a1[1] = a3;
  return result;
}

// ===== sub_45EB60 @ 0x0045EB60..0x0045EBA9 =====
int __usercall sub_45EB60@<eax>(_DWORD *a1@<edi>, int *a2@<esi>)
{
  int v2; // ecx
  int v3; // edx
  int v4; // eax
  int result; // eax
  int v6; // [esp+0h] [ebp-8h] BYREF
  int v7; // [esp+4h] [ebp-4h]

  sub_45E8D0(&v6, *a2, a2[1], 0);
  v2 = v7;
  v3 = a2[3];
  *a1 = v6;
  v4 = a2[2];
  a1[1] = v2;
  sub_45E8D0(&v6, v4, v3, 0);
  result = v7;
  a1[2] = v6;
  a1[3] = result;
  return result;
}

// ===== sub_45EBB0 @ 0x0045EBB0..0x0045ECEF =====
unsigned int __usercall sub_45EBB0@<eax>(_DWORD *a1@<esi>)
{
  unsigned int result; // eax
  unsigned int v2; // ecx
  int v3; // eax
  int v4; // eax
  unsigned int v5; // eax
  unsigned int v6; // ecx
  int v7; // edx
  double v8; // st7
  double v9; // st6
  double v10; // st5
  unsigned int v11; // ebx
  double v12; // st4
  int v13; // ecx
  unsigned int v14; // edi
  int v15; // ecx
  unsigned int v16; // [esp+Ch] [ebp-14h]
  unsigned int v17; // [esp+10h] [ebp-10h]
  unsigned int v18; // [esp+14h] [ebp-Ch] BYREF
  unsigned int v19; // [esp+18h] [ebp-8h]
  unsigned int v20; // [esp+1Ch] [ebp-4h]

  sub_45E600();
  if ( sub_45F640() )
  {
    sub_45E580(&v18, 1);
    v3 = sub_45E880();
    if ( v3 )
    {
      v4 = v3 - 1;
      if ( v4 )
      {
        result = v4 - 1;
        if ( !result )
        {
          v5 = (v18 - v16) >> 1;
          a1[2] = v18 - v5 - 1;
          v6 = v19;
          *a1 = v5;
          result = (v6 - v17) >> 1;
          a1[1] = result;
          a1[3] = v6 - result - 1;
        }
      }
      else
      {
        v7 = v18 - 1;
        result = v19 - 1;
        *a1 = 0;
        a1[2] = v7;
        a1[1] = 0;
        a1[3] = result;
      }
    }
    else
    {
      v8 = (double)v16;
      v9 = (double)v18 / v8;
      v10 = (double)v17;
      v11 = v19;
      v20 = v19;
      v12 = (double)v19 / v10;
      if ( v12 < v9 )
        v9 = v12;
      v13 = (int)(v18 - (int)(v8 * v9)) >> 1;
      v14 = v18 - v13 - 1;
      *a1 = v13;
      a1[2] = v14;
      result = (int)(v10 * v9);
      v15 = (int)(v11 - result) >> 1;
      a1[1] = v15;
      a1[3] = v11 - v15 - 1;
    }
  }
  else
  {
    sub_45E750(&v18);
    result = v18 - 1;
    v2 = v19 - 1;
    *a1 = 0;
    a1[2] = result;
    a1[1] = 0;
    a1[3] = v2;
  }
  return result;
}

// ===== sub_45ECF0 @ 0x0045ECF0..0x0045ED35 =====
unsigned int __usercall sub_45ECF0@<eax>(_DWORD *a1@<edi>, unsigned int *a2)
{
  unsigned int result; // eax
  _DWORD v3[7]; // [esp+8h] [ebp-1Ch] BYREF

  sub_45EBB0(v3);
  sub_45E600();
  *a2 = (unsigned int)(*a1 * (v3[2] - v3[0] + 1)) / v3[4];
  result = (unsigned int)(a1[1] * (v3[3] - v3[1] + 1)) / v3[5];
  a2[1] = result;
  return result;
}

// ===== sub_45ED40 @ 0x0045ED40..0x0045EE6C =====
int sub_45ED40()
{
  unsigned int v0; // eax
  int result; // eax
  int v2; // eax
  int v3; // eax
  double v4; // st7
  double v5; // st6
  unsigned int v6; // [esp+8h] [ebp-18h]
  unsigned int v7; // [esp+Ch] [ebp-14h]
  unsigned int v8; // [esp+10h] [ebp-10h] BYREF
  unsigned int v9; // [esp+14h] [ebp-Ch]

  sub_45E600();
  if ( !sub_45F640() )
  {
    sub_45E750(&v8);
LABEL_3:
    if ( v8 > v6 )
      v0 = (v8 << 16) / v6;
    else
      v0 = (v6 << 16) / v8;
    dword_5177A8 = v0;
    if ( v9 > v7 )
      result = (v9 << 16) / v7;
    else
      result = (v7 << 16) / v9;
    dword_5177AC = result;
    return result;
  }
  sub_45E580(&v8, 1);
  v2 = sub_45E880();
  if ( v2 )
  {
    v3 = v2 - 1;
    if ( !v3 )
      goto LABEL_3;
    result = v3 - 1;
    if ( !result )
    {
      result = 0x10000;
      dword_5177A8 = 0x10000;
      dword_5177AC = 0x10000;
    }
  }
  else
  {
    v4 = (double)v8 / (double)v6;
    v5 = (double)v9 / (double)v7;
    if ( v5 < v4 )
      v4 = v5;
    result = (__int64)(v4 * 65536.0);
    dword_5177A8 = result;
    dword_5177AC = result;
  }
  return result;
}

// ===== sub_45EE70 @ 0x0045EE70..0x0045F010 =====
int __cdecl sub_45EE70(int a1)
{
  int v1; // ebx
  unsigned int *v2; // ecx
  int v3; // esi
  HDC v4; // edi
  HBRUSH StockObject; // eax
  int v7; // [esp+68h] [ebp-28h] BYREF
  HDC hDC; // [esp+6Ch] [ebp-24h] BYREF
  int v9; // [esp+70h] [ebp-20h]
  LONG v10; // [esp+74h] [ebp-1Ch]
  LONG v11; // [esp+78h] [ebp-18h]
  RECT rc; // [esp+7Ch] [ebp-14h] BYREF

  if ( !dword_565EA0 )
    return 4;
  v1 = 22 - (sub_45E470() != 0);
  sub_45E600();
  sub_45E610(&hDC, v2);
  v3 = v9;
  v4 = hDC;
  if ( !a1 )
  {
    if ( (*(int (__stdcall **)(int, HDC, int, int, _DWORD, int, int, int *, _DWORD))(*(_DWORD *)dword_565EA0 + 92))(
           dword_565EA0,
           hDC,
           v9,
           1,
           0,
           v1,
           2,
           &dword_565EA8,
           0) < 0 )
      return 5;
    if ( (*(int (__stdcall **)(int, _DWORD, int *))(*(_DWORD *)dword_565EA8 + 72))(dword_565EA8, 0, &v7) >= 0 )
    {
      if ( (*(int (__stdcall **)(int, HDC *))(*(_DWORD *)v7 + 60))(v7, &hDC) >= 0 )
      {
        rc.left = 0;
        rc.top = 0;
        rc.right = v10;
        rc.bottom = v11;
        StockObject = (HBRUSH)GetStockObject(4);
        FillRect(hDC, &rc, StockObject);
        (*(void (__stdcall **)(int, HDC))(*(_DWORD *)v7 + 64))(v7, hDC);
      }
      (*(void (__stdcall **)(int))(*(_DWORD *)v7 + 8))(v7);
    }
  }
  if ( (*(int (__stdcall **)(int, HDC, int, int, _DWORD, int, _DWORD, int *, _DWORD))(*(_DWORD *)dword_565EA0 + 92))(
         dword_565EA0,
         v4,
         v3,
         1,
         0,
         v1,
         0,
         &dword_565EAC,
         0) < 0 )
    return 7;
  else
    return (*(int (__stdcall **)(int, HDC, int, int, int, int, _DWORD, int *, _DWORD))(*(_DWORD *)dword_565EA0 + 92))(
             dword_565EA0,
             v4,
             v3,
             1,
             512,
             22,
             0,
             &dword_565EB0,
             0) >= 0
         ? 0
         : 9;
}

// ===== sub_45F010 @ 0x0045F010..0x0045F016 =====
int sub_45F010()
{
  return dword_565EA8;
}

// ===== sub_45F020 @ 0x0045F020..0x0045F026 =====
int sub_45F020()
{
  return dword_565EB0;
}

// ===== sub_45F030 @ 0x0045F030..0x0045F2AD =====
int sub_45F030()
{
  unsigned int *v0; // ecx
  double v1; // st6
  double v2; // st5
  double v3; // st2
  double v4; // st7
  double v5; // st1
  double v6; // st5
  int v8; // [esp+Ch] [ebp-2Ch] BYREF
  int v9; // [esp+10h] [ebp-28h]
  int v10; // [esp+14h] [ebp-24h]
  int v11; // [esp+18h] [ebp-20h]
  unsigned int v12; // [esp+1Ch] [ebp-1Ch]
  unsigned int v13; // [esp+20h] [ebp-18h]
  float v14; // [esp+24h] [ebp-14h] BYREF
  float v15; // [esp+28h] [ebp-10h]
  float v16; // [esp+2Ch] [ebp-Ch]
  unsigned int v17; // [esp+30h] [ebp-8h] BYREF
  float v18; // [esp+34h] [ebp-4h]

  if ( !dword_565EA0 )
    return 4;
  if ( (*(int (__stdcall **)(int, int, int, int, int, int *, _DWORD))(*(_DWORD *)dword_565EA0 + 104))(
         dword_565EA0,
         112,
         8,
         324,
         1,
         &dword_565EB8,
         0) < 0 )
    return 13;
  sub_45E600();
  sub_45E610(&v17, v0);
  sub_45EBB0(&v8);
  LODWORD(v16) = v10 - v8 + 1;
  v15 = (double)v12 / (double)SLODWORD(v16) - 0.5;
  LODWORD(v16) = v11 - v9 + 1;
  dword_517748 = 0xFFFFFF;
  v14 = (double)v13 / (double)SLODWORD(v16) - 0.5;
  v16 = (float)v8;
  v1 = v16;
  flt_517738 = v16;
  v16 = (float)v9;
  v2 = v16;
  flt_51773C = v16;
  flt_517740 = 0.0;
  flt_517744 = 1.0;
  v16 = (float)v17;
  v3 = v16;
  v16 = 0.5 / v16;
  flt_51774C = v16;
  v18 = (float)LODWORD(v18);
  dword_517764 = 0xFFFFFF;
  v4 = v16;
  v16 = 0.5 / v18;
  v5 = v16;
  flt_517750 = v16;
  v16 = (float)(v10 + 1);
  flt_517754 = v16;
  flt_517758 = v2;
  flt_51775C = 0.0;
  flt_517760 = 1.0;
  dword_517780 = 0xFFFFFF;
  v15 = ((double)v12 + v15) / v3;
  flt_517768 = v15;
  v6 = v15;
  flt_51776C = v5;
  flt_517770 = v1;
  v15 = (float)(v11 + 1);
  flt_517774 = v15;
  flt_517778 = 0.0;
  flt_51777C = 1.0;
  flt_517784 = v4;
  dword_51779C = 0xFFFFFF;
  v14 = ((double)v13 + v14) / v18;
  flt_517788 = v14;
  flt_51778C = v16;
  flt_517790 = v15;
  flt_517794 = 0.0;
  flt_517798 = 1.0;
  flt_5177A0 = v6;
  flt_5177A4 = v14;
  (*(void (__stdcall **)(int, _DWORD, _DWORD, float *, _DWORD))(*(_DWORD *)dword_565EB8 + 44))(
    dword_565EB8,
    0,
    0,
    &v14,
    0);
  qmemcpy((void *)LODWORD(v14), &flt_517738, 0x70u);
  (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB8 + 48))(dword_565EB8);
  dword_565EBC = 0;
  return 0;
}

// ===== sub_45F2B0 @ 0x0045F2B0..0x0045F33F =====
int sub_45F2B0()
{
  int result; // eax

  if ( dword_565EA8 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EA8 + 8))(dword_565EA8);
    dword_565EA8 = 0;
  }
  if ( dword_565EAC )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EAC + 8))(dword_565EAC);
    dword_565EAC = 0;
  }
  if ( dword_565EB0 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB0 + 8))(dword_565EB0);
    dword_565EB0 = 0;
  }
  if ( dword_565EB4 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB4 + 8))(dword_565EB4);
    dword_565EB4 = 0;
  }
  if ( dword_565EB8 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB8 + 8))(dword_565EB8);
    dword_565EB8 = 0;
  }
  result = dword_565EA0;
  if ( dword_565EA0 )
  {
    result = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 8))(dword_565EA0);
    dword_565EA0 = 0;
  }
  return result;
}

// ===== sub_45F340 @ 0x0045F340..0x0045F583 =====
int __cdecl sub_45F340(int a1)
{
  BOOL v1; // esi
  int v2; // esi
  int v3; // eax
  int v4; // esi
  int v5; // eax
  int result; // eax
  int v7; // esi
  HWND v8; // [esp-10h] [ebp-1Ch]
  HWND v9; // [esp-10h] [ebp-1Ch]

  sub_4602C0(0);
  dword_565E9C = (int)Direct3DCreate9(0x20u);
  if ( !dword_565E9C )
    return 1;
  dword_565E94 = a1;
  if ( !sub_45E550(0) )
    return 2;
  if ( sub_45E850() )
    v1 = 1;
  else
    v1 = a1 == 0;
  dword_5177C0 = !v1 ? dword_5177B0 : 0;
  dword_5177C4 = !v1 ? dword_5177B4 : 0;
  dword_5177E0 = v1;
  dword_5177C8 = !v1 ? dword_5177BC : 0;
  dword_5177D8 = 2 * v1 + 1;
  v8 = hWndParent;
  dword_5177CC = 1;
  dword_5177D0 = 0;
  dword_5177D4 = 0;
  dword_5177DC = (int)hWndParent;
  dword_5177E4 = 0;
  dword_5177E8 = 80;
  dword_5177EC = !v1;
  dword_5177F0 = !v1 ? dword_5177B8 : 0;
  dword_5177F4 = 0;
  v2 = *(_DWORD *)dword_565E9C;
  v3 = sub_45E4F0();
  if ( (*(int (__stdcall **)(int, int, int, HWND, int, int *, int *))(v2 + 64))(
         dword_565E9C,
         v3,
         1,
         v8,
         68,
         &dword_5177C0,
         &dword_565EA0) < 0 )
  {
    v4 = *(_DWORD *)dword_565E9C;
    v9 = hWndParent;
    v5 = sub_45E4F0();
    if ( (*(int (__stdcall **)(int, int, int, HWND, int, int *, int *))(v4 + 64))(
           dword_565E9C,
           v5,
           1,
           v9,
           36,
           &dword_5177C0,
           &dword_565EA0) < 0 )
      return 3;
  }
  result = sub_45EE70(0);
  if ( !result )
  {
    v7 = sub_45F030();
    if ( !v7 )
    {
      (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 22, 1);
      (*(void (__stdcall **)(int, int, _DWORD))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 7, 0);
      (*(void (__stdcall **)(int, int, _DWORD))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 137, 0);
      (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 27, 1);
      sub_45ED40();
      dword_517734 = sub_46F3C0();
      if ( sub_460030() > 0 && a1 == 1 && !sub_45E850() )
        (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_565EA0 + 80))(dword_565EA0, 1);
      sub_460550();
    }
    return v7;
  }
  return result;
}

// ===== sub_45F590 @ 0x0045F590..0x0045F63C =====
int sub_45F590()
{
  int v0; // eax

  v0 = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 12))(dword_565EA0);
  if ( v0 == -2005530520 )
    return 0x80000000;
  if ( v0 != -2005530519 )
    return -(v0 != 0);
  if ( dword_565EAC )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EAC + 8))(dword_565EAC);
    dword_565EAC = 0;
  }
  if ( dword_565EB0 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB0 + 8))(dword_565EB0);
    dword_565EB0 = 0;
  }
  if ( dword_565EB4 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB4 + 8))(dword_565EB4);
    dword_565EB4 = 0;
  }
  sub_460400();
  (*(void (__stdcall **)(int, int *))(*(_DWORD *)dword_565EA0 + 64))(dword_565EA0, &dword_5177C0);
  sub_45EE70(1);
  sub_460550();
  return -2147483647;
}

// ===== sub_45F640 @ 0x0045F640..0x0045F646 =====
int sub_45F640()
{
  return dword_565E94;
}

// ===== sub_45F650 @ 0x0045F650..0x0045F662 =====
_DWORD *__usercall sub_45F650@<eax>(_DWORD *result@<eax>)
{
  int v1; // edx

  v1 = dword_5177AC;
  *result = dword_5177A8;
  result[1] = v1;
  return result;
}

// ===== sub_45F670 @ 0x0045F670..0x0045F676 =====
int sub_45F670()
{
  return dword_517734;
}

// ===== sub_45F680 @ 0x0045F680..0x0045F686 =====
int sub_45F680()
{
  return dword_5172DC;
}

// ===== sub_45F690 @ 0x0045F690..0x0045F6E1 =====
int __cdecl sub_45F690(unsigned int *a1)
{
  unsigned int v2; // eax
  int v3; // [esp+8h] [ebp-Ch] BYREF
  unsigned int v4; // [esp+Ch] [ebp-8h]

  if ( (*(int (__stdcall **)(int, _DWORD, int *))(*(_DWORD *)dword_565EA0 + 76))(dword_565EA0, 0, &v3) )
    return -1;
  if ( v3 )
    return -2130706432;
  v2 = sub_45F680();
  if ( v4 <= v2 )
    v2 = v4;
  *a1 = v2;
  return 0;
}

// ===== sub_45F6F0 @ 0x0045F6F0..0x0045F6F6 =====
int __usercall sub_45F6F0@<eax>(int result@<eax>)
{
  dword_507220 = result;
  return result;
}

// ===== sub_45F700 @ 0x0045F700..0x0045F706 =====
int sub_45F700()
{
  return dword_507220;
}

// ===== sub_45F710 @ 0x0045F710..0x0045F72E =====
BOOL sub_45F710()
{
  return !dword_565EA0 || (*(int (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 12))(dword_565EA0) != 0;
}

// ===== sub_45F730 @ 0x0045F730..0x0045FC30 =====
int __fastcall sub_45F730(int a1, int a2, int a3, int a4)
{
  int result; // eax
  _DWORD *v7; // esi
  unsigned int *v8; // ecx
  double v9; // st7
  void (__stdcall *v10)(int, _DWORD, _DWORD, float *, _DWORD); // eax
  double v11; // st7
  double v12; // st6
  double v13; // st6
  float v14; // [esp+104h] [ebp-3Ch] BYREF
  float v15; // [esp+108h] [ebp-38h]
  float v16; // [esp+10Ch] [ebp-34h]
  int xRight; // [esp+110h] [ebp-30h] BYREF
  int yBottom; // [esp+114h] [ebp-2Ch]
  int v19; // [esp+118h] [ebp-28h]
  int v20; // [esp+11Ch] [ebp-24h]
  unsigned int v21; // [esp+120h] [ebp-20h] BYREF
  unsigned int v22; // [esp+124h] [ebp-1Ch]
  float v23; // [esp+128h] [ebp-18h] BYREF
  struct tagRECT rc; // [esp+12Ch] [ebp-14h] BYREF

  result = (*(int (__stdcall **)(int, _DWORD, _DWORD, int, int, _DWORD, _DWORD))(*(_DWORD *)dword_565EA0 + 172))(
             dword_565EA0,
             0,
             0,
             1,
             -16777216,
             1.0,
             0);
  if ( result >= 0 )
  {
    if ( a2 )
    {
      if ( a1 )
      {
        v7 = (_DWORD *)(a2 + 8);
        do
        {
          SetRect(&rc, *(v7 - 2), *(v7 - 1), *v7 + 1, v7[1] + 1);
          (*(void (__stdcall **)(int, struct tagRECT *))(*(_DWORD *)dword_565EA8 + 84))(dword_565EA8, &rc);
          v7 += 4;
          --a1;
        }
        while ( a1 );
      }
    }
    else
    {
      sub_45E600();
      SetRect(&rc, 0, 0, xRight, yBottom);
      (*(void (__stdcall **)(int, struct tagRECT *))(*(_DWORD *)dword_565EA8 + 84))(dword_565EA8, &rc);
    }
    (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 124))(dword_565EA0, dword_565EA8, dword_565EAC);
    result = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 164))(dword_565EA0);
    if ( result >= 0 )
    {
      if ( dword_565EC8 )
      {
        if ( dword_565EC8 == 1 )
        {
          (*(void (__stdcall **)(int, const char *))(*(_DWORD *)dword_565ECC + 232))(dword_565ECC, "DEScalerByBicubic");
          (*(void (__stdcall **)(int, const char *, int))(*(_DWORD *)dword_565ECC + 208))(
            dword_565ECC,
            "SourceTexture01",
            dword_565EAC);
          sub_45E600();
          sub_45E610(&v21, v8);
          v14 = (float)v21;
          v15 = (float)v22;
          (*(void (__stdcall **)(int, const char *, float *, int))(*(_DWORD *)dword_565ECC + 128))(
            dword_565ECC,
            "g_f2SizeOfSourceTexture01",
            &v14,
            2);
          v14 = (double)(unsigned int)xRight + 0.5;
          v15 = (double)(unsigned int)yBottom + 0.5;
          (*(void (__stdcall **)(int, const char *, float *, int))(*(_DWORD *)dword_565ECC + 128))(
            dword_565ECC,
            "g_f2AvailableAreaOfSourceTexture01",
            &v14,
            2);
          (*(void (__stdcall **)(int, float *, _DWORD))(*(_DWORD *)dword_565ECC + 252))(dword_565ECC, &v23, 0);
          (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)dword_565ECC + 256))(dword_565ECC, 0);
        }
      }
      else
      {
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 276))(dword_565EA0, 0, 6, 2);
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 276))(dword_565EA0, 0, 5, 2);
        (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 171, 1);
        (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 20, 6);
        (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 19, 5);
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 1, 4);
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 2, 2);
        (*(void (__stdcall **)(int, _DWORD, int, _DWORD))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 3, 0);
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 4, 2);
        (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 5, 2);
        (*(void (__stdcall **)(int, _DWORD, int, _DWORD))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 6, 0);
        (*(void (__stdcall **)(int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 260))(dword_565EA0, 0, dword_565EAC);
      }
      (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_565EA0 + 356))(dword_565EA0, 324);
      if ( dword_565EBC || a3 || a4 )
      {
        sub_45EBB0(&xRight);
        sub_45E600();
        LODWORD(v14) = v19 - xRight + 1;
        v9 = (double)SLODWORD(v14) * (double)a3;
        LODWORD(v14) = v20 - yBottom + 1;
        v23 = v9 / (double)v21;
        v10 = *(void (__stdcall **)(int, _DWORD, _DWORD, float *, _DWORD))(*(_DWORD *)dword_565EB8 + 44);
        v16 = (double)SLODWORD(v14) * (double)a4 / (double)v22;
        v10(dword_565EB8, 0, 0, &v14, 0);
        v11 = v23;
        v12 = v23;
        qmemcpy((void *)LODWORD(v14), &flt_517738, 0x70u);
        *(float *)LODWORD(v14) = v12 + *(float *)LODWORD(v14);
        v13 = v16;
        *(float *)(LODWORD(v14) + 4) = v16 + *(float *)(LODWORD(v14) + 4);
        *(float *)(LODWORD(v14) + 28) = *(float *)(LODWORD(v14) + 28) + v11;
        *(float *)(LODWORD(v14) + 32) = *(float *)(LODWORD(v14) + 32) + v13;
        *(float *)(LODWORD(v14) + 56) = *(float *)(LODWORD(v14) + 56) + v11;
        *(float *)(LODWORD(v14) + 60) = *(float *)(LODWORD(v14) + 60) + v13;
        *(float *)(LODWORD(v14) + 84) = v11 + *(float *)(LODWORD(v14) + 84);
        *(float *)(LODWORD(v14) + 88) = v13 + *(float *)(LODWORD(v14) + 88);
        (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB8 + 48))(dword_565EB8);
        if ( a3 || (dword_565EBC = 0, a4) )
          dword_565EBC = 1;
      }
      (*(void (__stdcall **)(int, _DWORD, int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 400))(
        dword_565EA0,
        0,
        dword_565EB8,
        0,
        28);
      (*(void (__stdcall **)(int, int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 324))(dword_565EA0, 5, 0, 2);
      if ( dword_565EC8 )
      {
        (*(void (__stdcall **)(int))(*(_DWORD *)dword_565ECC + 264))(dword_565ECC);
        (*(void (__stdcall **)(int))(*(_DWORD *)dword_565ECC + 268))(dword_565ECC);
      }
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 168))(dword_565EA0);
      result = dword_565EA4;
      if ( dword_565EA4 )
        return (*(int (__stdcall **)(int, int))(*(_DWORD *)dword_565EA4 + 24))(dword_565EA4, 1);
    }
  }
  return result;
}

// ===== sub_45FC30 @ 0x0045FC30..0x0045FEEA =====
int __cdecl sub_45FC30(int a1, unsigned int a2, unsigned int a3)
{
  int result; // eax
  unsigned int *v4; // ecx
  double v5; // st7
  double v6; // st7
  int v7; // eax
  double v8; // st5
  double v9; // st4
  double v10; // st4
  double v11; // st3
  double v12; // rt1
  double v13; // st3
  double v14; // st6
  void (__stdcall *v15)(int, _DWORD, int, _DWORD, int); // eax
  float *v16; // [esp+D0h] [ebp-14h] BYREF
  float v17; // [esp+D4h] [ebp-10h]
  unsigned int v18; // [esp+DCh] [ebp-8h] BYREF
  int v19; // [esp+E0h] [ebp-4h]

  result = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 164))(dword_565EA0);
  if ( result >= 0 )
  {
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 276))(dword_565EA0, 0, 6, 2);
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 276))(dword_565EA0, 0, 5, 2);
    (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 171, 1);
    (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 20, 6);
    (*(void (__stdcall **)(int, int, int))(*(_DWORD *)dword_565EA0 + 228))(dword_565EA0, 19, 5);
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 1, 4);
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 2, 2);
    (*(void (__stdcall **)(int, _DWORD, int, _DWORD))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 3, 0);
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 4, 2);
    (*(void (__stdcall **)(int, _DWORD, int, int))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 5, 2);
    (*(void (__stdcall **)(int, _DWORD, int, _DWORD))(*(_DWORD *)dword_565EA0 + 268))(dword_565EA0, 0, 6, 0);
    (*(void (__stdcall **)(int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 260))(dword_565EA0, 0, a1);
    (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_565EA0 + 356))(dword_565EA0, 324);
    sub_45E600();
    sub_45E610(&v18, v4);
    (*(void (__stdcall **)(int, _DWORD, _DWORD, float **, _DWORD))(*(_DWORD *)dword_565EB8 + 44))(
      dword_565EB8,
      0,
      0,
      &v16,
      0);
    v5 = (double)v18;
    qmemcpy(v16, &flt_517738, 0x70u);
    v17 = v5;
    v6 = v17;
    v7 = v19;
    v17 = 0.5 / v17;
    v8 = v17;
    v16[5] = v17;
    v9 = (double)v19;
    if ( v7 < 0 )
      v9 = v9 + 4294967300.0;
    v17 = v9;
    v10 = v17;
    v17 = 0.5 / v17;
    v11 = v17;
    v16[6] = v17;
    v12 = v11;
    v17 = ((double)a2 + 0.5) / v6;
    v13 = v17;
    v16[12] = v17;
    v16[13] = v12;
    v16[19] = v8;
    v17 = ((double)a3 + 0.5) / v10;
    v14 = v17;
    v16[20] = v17;
    v16[26] = v13;
    v16[27] = v14;
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EB8 + 48))(dword_565EB8);
    v15 = *(void (__stdcall **)(int, _DWORD, int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 400);
    dword_565EBC = 1;
    v15(dword_565EA0, 0, dword_565EB8, 0, 28);
    (*(void (__stdcall **)(int, int, _DWORD, int))(*(_DWORD *)dword_565EA0 + 324))(dword_565EA0, 5, 0, 2);
    (*(void (__stdcall **)(int))(*(_DWORD *)dword_565EA0 + 168))(dword_565EA0);
    result = dword_565EA4;
    if ( dword_565EA4 )
      return (*(int (__stdcall **)(int, int))(*(_DWORD *)dword_565EA4 + 24))(dword_565EA4, 1);
  }
  return result;
}

// ===== sub_45FEF0 @ 0x0045FEF0..0x0045FEF6 =====
int sub_45FEF0()
{
  return dword_565EC4;
}

// ===== sub_45FF00 @ 0x0045FF00..0x0045FFA9 =====
unsigned int __cdecl sub_45FF00(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // eax
  unsigned int i; // edi
  unsigned int v4; // esi
  unsigned int v6; // [esp+10h] [ebp-8h] BYREF
  unsigned int v7; // [esp+14h] [ebp-4h]

  v1 = 0;
  if ( sub_460320() )
  {
    v2 = sub_45F680();
    v7 = v2 - (v2 >> 1);
    for ( i = 0; !sub_45F690(&v6); ++v1 )
    {
      v4 = v6;
      if ( i > v6 )
        break;
      if ( v6 >= v7 )
        break;
      sub_493AE0();
      i = v4;
    }
  }
  if ( (*(int (__stdcall **)(int, _DWORD, _DWORD, _DWORD, _DWORD))(*(_DWORD *)dword_565EA0 + 68))(
         dword_565EA0,
         0,
         0,
         0,
         0) < 0 )
    return 0x80000000;
  dword_565EC4 = sub_498720();
  *a1 = v1;
  return 0;
}

// ===== sub_45FFB0 @ 0x0045FFB0..0x00460021 =====
int __cdecl sub_45FFB0(int a1)
{
  int result; // eax
  int v2; // ecx
  int v3; // ecx

  result = 0;
  if ( dword_565EA0 )
  {
    if ( a1 )
    {
      if ( !dword_565EC0 && sub_45F640() == 1 && !sub_45E850() )
        (*(void (__stdcall **)(int, int))(*(_DWORD *)v2 + 80))(v2, 1);
      ++dword_565EC0;
      return 1;
    }
    else
    {
      if ( !--dword_565EC0 && sub_45F640() == 1 && !sub_45E850() )
        (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)v3 + 80))(v3, 0);
      return 1;
    }
  }
  return result;
}
