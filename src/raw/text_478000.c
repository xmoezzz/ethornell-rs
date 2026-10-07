#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_478030 @ 0x00478030..0x004780B9 =====
int __cdecl sub_478030(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497B60(v1);
  if ( sub_4619C0(v4, v2) == 2 )
  {
    sprintf(Buffer, &byte_4E799C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4780C0 @ 0x004780C0..0x004780D8 =====
int __cdecl sub_4780C0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_461690();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4780E0 @ 0x004780E0..0x0047816E =====
int __cdecl sub_4780E0(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // esi
  int v4; // ebx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v4 = 0;
  if ( !sub_45F640() )
  {
    v4 = sub_46F930(v3, v1);
    if ( v4 )
    {
      if ( sub_49A220() )
      {
        SetWindowPos(hWndParent, 0, v3, v1, 0, 0, 0x25u);
        sub_4610D0(v1);
        sub_4450D0(a1, v4);
        return 0;
      }
      sub_49A150(v1);
    }
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_478170 @ 0x00478170..0x004781B5 =====
int __cdecl sub_478170(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  if ( sub_48EBD0(v1, v2) == -1 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_4781C0 @ 0x004781C0..0x004781D8 =====
int __cdecl sub_4781C0(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_48E850();
  return 0;
}

// ===== sub_4781E0 @ 0x004781E0..0x004781F8 =====
int __cdecl sub_4781E0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_48EE20();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_478200 @ 0x00478200..0x004783BF =====
int __cdecl sub_478200(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  const char *v5; // [esp-8h] [ebp-144h]
  unsigned int v6; // [esp-4h] [ebp-140h]
  int v7; // [esp+14h] [ebp-128h]
  int v8; // [esp+18h] [ebp-124h]
  unsigned int v9; // [esp+1Ch] [ebp-120h]
  int v10; // [esp+20h] [ebp-11Ch]
  int v11; // [esp+24h] [ebp-118h]
  _DWORD *v12; // [esp+28h] [ebp-114h]
  char Buffer[256]; // [esp+2Ch] [ebp-110h] BYREF
  int v14; // [esp+138h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v3 = operator new(0x70u);
  v14 = 0;
  if ( v3 )
    v12 = (_DWORD *)sub_43CBD0(a1, v3);
  else
    v12 = 0;
  v14 = -1;
  switch ( sub_43CCE0(v12, v9, v8, v10, v11, v7, v2, v1) )
  {
    case -2147483647:
      v6 = v9;
      v5 = &byte_4E7FD8;
      goto LABEL_6;
    case -2147483646:
      sprintf(Buffer, &byte_4E8004, v10);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E802C, v11);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      v6 = v2;
      if ( v2 >= 1 )
      {
        v5 = (const char *)&unk_4E8058;
LABEL_6:
        sprintf(Buffer, v5, v6);
      }
      else
      {
        sprintf(Buffer, &byte_4E7F2C, v2);
      }
      sub_4646F0(Buffer, (int)a1);
    default:
      sub_4451C0((int)a1, (int)v12);
      return 2;
  }
}

// ===== sub_4783D0 @ 0x004783D0..0x00478497 =====
int __cdecl sub_4783D0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  const CHAR *v3; // eax
  int v4; // eax
  int Y; // [esp+Ch] [ebp-10Ch]
  int X; // [esp+10h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  Y = sub_4450B0(a1);
  X = sub_4450B0(a1);
  v3 = (const CHAR *)sub_48DF50(a1);
  v4 = sub_42FB00(&X, v3, X, Y, v2, v1);
  if ( v4 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E80F4, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -2147483646 )
    sub_4646F0(byte_4E80B8, (int)a1);
  sub_4450D0(a1, X);
  return 0;
}

// ===== sub_4784A0 @ 0x004784A0..0x004784C9 =====
int __cdecl sub_4784A0(_DWORD *a1)
{
  void *v1; // eax

  v1 = (void *)sub_4450B0(a1);
  if ( !sub_42FC40(v1) )
    sub_4646F0(byte_4E8128, (int)a1);
  return 0;
}

// ===== sub_4784D0 @ 0x004784D0..0x00478504 =====
int __cdecl sub_4784D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_42FC70(v2, v1) )
    sub_4646F0(byte_4E8168, (int)a1);
  return 0;
}

// ===== sub_478510 @ 0x00478510..0x00478546 =====
int __cdecl sub_478510(_DWORD *a1)
{
  LPCSTR v1; // edx

  sub_48DF50(a1);
  sub_4450B0(a1);
  if ( !sub_42FD50(v1) )
    sub_4646F0(byte_4E8168, (int)a1);
  return 0;
}

// ===== sub_478550 @ 0x00478550..0x00478592 =====
int __cdecl sub_478550(_DWORD *a1)
{
  int v1; // edi
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  if ( !sub_42FCC0(v1, v2) )
    sub_4646F0(byte_4E8168, (int)a1);
  return 0;
}

// ===== sub_4785A0 @ 0x004785A0..0x004785E7 =====
int __cdecl sub_4785A0(_DWORD *a1)
{
  int v1; // eax
  _DWORD *v2; // eax
  LONG v4[2]; // [esp+Ch] [ebp-8h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_42FD00(v1, v4) )
    sub_4646F0(byte_4E8168, (int)a1);
  v2 = sub_4450D0(a1, v4[0]);
  sub_4450D0(v2, v4[1]);
  return 0;
}

// ===== sub_4785F0 @ 0x004785F0..0x00478626 =====
int __cdecl sub_4785F0(_DWORD *a1)
{
  int v1; // edx

  sub_4450B0(a1);
  sub_4450B0(a1);
  if ( !sub_42FD80(v1) )
    sub_4646F0(byte_4E8168, (int)a1);
  return 0;
}

// ===== sub_478630 @ 0x00478630..0x00478770 =====
int __cdecl sub_478630(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  unsigned int v3; // eax
  int v5; // [esp+10h] [ebp-110h]
  int v6; // [esp+14h] [ebp-10Ch]
  int v7; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497B60(v2);
  sub_497C40(v6);
  sub_497DB0(v1);
  v3 = sub_42FDE0(v7, v5, v2, v6, v1);
  if ( v3 > 0x80000007 )
  {
    if ( v3 == -1 )
      sub_4646F0(byte_4E8168, (int)a1);
  }
  else
  {
    switch ( v3 )
    {
      case 0x80000007:
        sprintf(Buffer, &byte_4E81E8, v7, v5);
        sub_4646F0(Buffer, (int)a1);
      case 0x80000003:
        sprintf(Buffer, &byte_4E799C, v2);
        sub_4646F0(Buffer, (int)a1);
      case 0x80000004:
        sprintf(Buffer, &byte_4E81A0, v2);
        sub_4646F0(Buffer, (int)a1);
    }
  }
  return 0;
}

// ===== sub_478780 @ 0x00478780..0x0047891E =====
int __cdecl sub_478780(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // eax
  int v4; // [esp+Ch] [ebp-128h]
  int v5; // [esp+10h] [ebp-124h]
  int v6; // [esp+14h] [ebp-120h]
  int v7; // [esp+18h] [ebp-11Ch]
  int v8; // [esp+1Ch] [ebp-118h]
  int v9; // [esp+20h] [ebp-114h]
  int v10; // [esp+24h] [ebp-110h]
  int v11; // [esp+28h] [ebp-10Ch]
  int v12; // [esp+2Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v7 = sub_48DF50(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  sub_497AF0();
  v2 = sub_42FEE0(v8, v9, v7, v1, v10, v11, v4, v6, v5, (int)&v12);
  if ( v2 > 0x8000000B )
  {
    if ( v2 == -1 )
      sub_4646F0(byte_4E8168, (int)a1);
  }
  else
  {
    switch ( v2 )
    {
      case 0x8000000B:
        sprintf(Buffer, &byte_4E8264, v1);
        sub_4646F0(Buffer, (int)a1);
      case 0x80000009:
        sprintf(Buffer, &byte_4E7970, v10);
        sub_4646F0(Buffer, (int)a1);
      case 0x8000000A:
        sprintf(Buffer, &byte_4E823C, v11);
        sub_4646F0(Buffer, (int)a1);
    }
  }
  sub_4450D0(a1, v12);
  return 0;
}

// ===== sub_478920 @ 0x00478920..0x00478955 =====
int __cdecl sub_478920(_DWORD *a1)
{
  const char *v1; // edi
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  if ( sub_42FFD0(v2, v1) == -1 )
    sub_4646F0(byte_4E8168, (int)a1);
  return 0;
}

// ===== sub_478960 @ 0x00478960..0x00478AB2 =====
int __cdecl sub_478960(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v6; // [esp+Ch] [ebp-114h]
  int v7; // [esp+10h] [ebp-110h]
  unsigned int v8; // [esp+14h] [ebp-10Ch]
  int wParam; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  wParam = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_463F20(v1, v3, v6, v2, v8, wParam, v4) )
  {
    case 1:
      sprintf(Buffer, &byte_4E8290, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E82C4, v7);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sprintf(Buffer, &byte_4E82F0, v8);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, &byte_4E8320, wParam);
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_478AD0 @ 0x00478AD0..0x00478AE8 =====
int __cdecl sub_478AD0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_464190();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_478AF0 @ 0x00478AF0..0x00478B15 =====
int __cdecl sub_478AF0(_DWORD *a1)
{
  char *v1; // eax
  int v2; // eax

  v1 = (char *)sub_4450B0(a1);
  v2 = sub_4641F0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_478B20 @ 0x00478B20..0x00478B38 =====
int __cdecl sub_478B20(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_464280();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_478B40 @ 0x00478B40..0x00478B58 =====
int __cdecl sub_478B40(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_464210(v1);
  return 0;
}

// ===== sub_478B60 @ 0x00478B60..0x00478B74 =====
int __cdecl sub_478B60(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_464290(v1);
  return 0;
}

// ===== sub_478B80 @ 0x00478B80..0x00478B94 =====
int __cdecl sub_478B80(int a1)
{
  char *v1; // eax

  v1 = (char *)sub_48DF50(a1);
  sub_464300(v1);
  return 0;
}

// ===== sub_478BA0 @ 0x00478BA0..0x00478BC7 =====
int __cdecl sub_478BA0(_DWORD *a1)
{
  CHAR *v1; // eax
  int v2; // eax

  v1 = (CHAR *)sub_48DF50(a1);
  v2 = sub_464320(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_478BD0 @ 0x00478BD0..0x00478BE4 =====
int __cdecl sub_478BD0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_464350(v1);
  return 0;
}

// ===== sub_478BF0 @ 0x00478BF0..0x00478C04 =====
int __cdecl sub_478BF0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_464360(v1);
  return 0;
}

// ===== sub_478C10 @ 0x00478C10..0x00478C32 =====
int __cdecl sub_478C10(int a1)
{
  const CHAR *v1; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  sub_46BC80(0, hWndParent, v1, 0x40u);
  return 0;
}

// ===== sub_478C40 @ 0x00478C40..0x00478C94 =====
int __cdecl sub_478C40(_DWORD *a1)
{
  int v1; // esi
  const CHAR *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v3 = sub_46BC80(0, hWndParent, v2, (v1 != 0 ? 0 : 256) | 0x24);
  sub_4450D0(a1, v3 == 6);
  return 0;
}

// ===== sub_478CA0 @ 0x00478CA0..0x00478D42 =====
int __cdecl sub_478CA0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  const CHAR *v3; // eax
  int v4; // eax
  int v6; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = (const CHAR *)sub_48DF50(a1);
  if ( v2 == 1 )
  {
    v6 = sub_46BC80(0, hWndParent, v3, (v1 != 0 ? 0 : 256) | 0x41);
    sub_4450D0(a1, v6 == 1);
  }
  else
  {
    v4 = sub_46BC80(0, hWndParent, v3, (v1 != 0 ? 0 : 256) | 0x24);
    sub_4450D0(a1, v4 == 6);
  }
  return 0;
}

// ===== sub_478D50 @ 0x00478D50..0x00478D68 =====
int __cdecl sub_478D50(int a1)
{
  const char *v1; // eax

  v1 = (const char *)sub_48DF50(a1);
  sub_46BC30(v1);
  return 0;
}

// ===== sub_478D70 @ 0x00478D70..0x00478DBF =====
int __cdecl sub_478D70(_DWORD *a1)
{
  int v1; // esi
  void *v2; // ebx
  CHAR *v3; // eax
  INT_PTR v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v3 = (CHAR *)sub_48DF50(a1);
  v4 = sub_45CB10(v1, v3, v6, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_478DC0 @ 0x00478DC0..0x00478E5C =====
int __cdecl sub_478DC0(_DWORD *a1)
{
  WPARAM v1; // esi
  void *v2; // ebx
  CHAR *v3; // eax
  INT_PTR v4; // eax
  CHAR *v6; // [esp+Ch] [ebp-18h]
  const CHAR *v7; // [esp+10h] [ebp-14h]
  const CHAR *v8; // [esp+14h] [ebp-10h]
  void *v9; // [esp+18h] [ebp-Ch]
  WPARAM v10; // [esp+1Ch] [ebp-8h]
  const CHAR *v11; // [esp+20h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v11 = (const CHAR *)sub_48DF50(a1);
  v10 = sub_4450B0(a1);
  v9 = (void *)sub_48DF50(a1);
  v8 = (const CHAR *)sub_48DF50(a1);
  v7 = (const CHAR *)sub_48DF50(a1);
  v6 = (CHAR *)sub_48DF50(a1);
  v3 = (CHAR *)sub_48DF50(a1);
  v4 = sub_45CEC0(0, v3, v6, v7, v8, v9, v10, 0, v11, v2, v1, 0);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_478E60 @ 0x00478E60..0x00478EBF =====
int __cdecl sub_478E60(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  INT_PTR v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-8h]
  const CHAR *v7; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = (const CHAR *)sub_48DF50(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_45D3A0(v2, v3, v6, v7, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_478EC0 @ 0x00478EC0..0x00478F7B =====
int __cdecl sub_478EC0(_DWORD *a1)
{
  int v1; // esi
  WPARAM v2; // ebx
  int v3; // eax
  CHAR *v4; // edx
  INT_PTR v5; // eax
  CHAR *v7; // [esp+Ch] [ebp-20h]
  const CHAR *v8; // [esp+10h] [ebp-1Ch]
  const CHAR *v9; // [esp+14h] [ebp-18h]
  void *v10; // [esp+18h] [ebp-14h]
  WPARAM v11; // [esp+1Ch] [ebp-10h]
  int v12; // [esp+20h] [ebp-Ch]
  const CHAR *v13; // [esp+24h] [ebp-8h]
  void *v14; // [esp+28h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v14 = (void *)sub_48DF50(a1);
  v13 = (const CHAR *)sub_48DF50(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = (void *)sub_48DF50(a1);
  v9 = (const CHAR *)sub_48DF50(a1);
  v8 = (const CHAR *)sub_48DF50(a1);
  v7 = (CHAR *)sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_45CEC0(v3, v4, v7, v8, v9, v10, v11, v12, v13, v14, v2, v1);
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_478F80 @ 0x00478F80..0x00478FCE =====
int __cdecl sub_478F80(_DWORD *a1)
{
  void *v1; // esi
  const CHAR *v2; // ebx
  LPARAM v3; // eax
  INT_PTR v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-4h]

  v1 = (void *)sub_48DF50(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_45DD50(v3, v6, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_478FD0 @ 0x00478FD0..0x0047903C =====
int __cdecl sub_478FD0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  void *v3; // eax
  INT_PTR v4; // eax
  void *v6; // [esp+Ch] [ebp-Ch]
  void *v7; // [esp+10h] [ebp-8h]
  void *v8; // [esp+14h] [ebp-4h]

  v1 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v8 = (void *)sub_48DF50(a1);
  v7 = (void *)sub_48DF50(a1);
  v6 = (void *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_45DAA0(v3, v6, v7, v8, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_479040 @ 0x00479040..0x00479084 =====
int __cdecl sub_479040(_DWORD *a1)
{
  LPARAM v1; // esi
  int v2; // ebx
  _DWORD *v3; // eax
  int v4; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_45E310(v3, v2, v1);
  sub_4450D0(a1, v4 == 0);
  return 0;
}

// ===== sub_479090 @ 0x00479090..0x004790BC =====
int __cdecl sub_479090(_DWORD *a1)
{
  void *v1; // eax
  unsigned int v2; // eax

  v1 = (void *)sub_4450B0(a1);
  v2 = sub_45E3B0(v1);
  sub_4450D0(a1, v2 == 0);
  return 0;
}

// ===== sub_4790C0 @ 0x004790C0..0x004790F7 =====
int __cdecl sub_4790C0(_DWORD *a1)
{
  void *v1; // eax
  void *v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = (void *)sub_4450B0(a1);
  v3 = sub_45E420(v2, v1);
  sub_4450D0(a1, v3 == 0);
  return 0;
}

// ===== sub_479100 @ 0x00479100..0x00479185 =====
int __cdecl sub_479100(_DWORD *a1)
{
  void *v1; // edi
  int v2; // edx
  _DWORD *v3; // ebx
  int v4; // esi
  int v5; // eax
  int v7; // [esp+Ch] [ebp-Ch]
  int v8; // [esp+10h] [ebp-8h]

  v1 = (void *)sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(v2);
  v4 = -2;
  v5 = sub_45DE70(v1);
  if ( v5 )
  {
    if ( v5 == 1 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v5 == 0x80000000 )
    {
      sub_4450D0(a1, -1);
      return 0;
    }
  }
  else
  {
    *v3 = v7;
    v3[1] = v8;
    v4 = 0;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_479190 @ 0x00479190..0x004791B8 =====
int __cdecl sub_479190(_DWORD *a1)
{
  const char *v1; // eax
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_468A70(-1, v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4791C0 @ 0x004791C0..0x004791F0 =====
int __cdecl sub_4791C0(_DWORD *a1)
{
  int v1; // esi
  const char *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_468A70(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4791F0 @ 0x004791F0..0x0047921A =====
int __cdecl sub_4791F0(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  v1 = sub_468BD0(0);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_479220 @ 0x00479220..0x00479252 =====
int __cdecl sub_479220(_DWORD *a1)
{
  DWORD v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_468BD0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_479260 @ 0x00479260..0x00479285 =====
int __cdecl sub_479260(_DWORD *a1)
{
  LPARAM v1; // eax
  LPARAM v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_468F50(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_479290 @ 0x00479290..0x004792C0 =====
int __cdecl sub_479290(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_468F70(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4792C0 @ 0x004792C0..0x004792E9 =====
int __cdecl sub_4792C0(int a1)
{
  const char *v1; // ebx
  const char *v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  sub_468F60(v1, v2);
  return 0;
}

// ===== sub_4792F0 @ 0x004792F0..0x00479322 =====
int __cdecl sub_4792F0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  int v4; // edx
  void *v5; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v5 = (void *)sub_48DF50(v4);
  sub_498D30(v5, v3, v1);
  return 0;
}

// ===== sub_479330 @ 0x00479330..0x00479392 =====
int __cdecl sub_479330(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(_DWORD *); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = (int (__cdecl *)(_DWORD *))*(&funcs_47937E + v1);
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4E834C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2(a1);
}

// ===== sub_4793A0 @ 0x004793A0..0x004793C1 =====
int __cdecl sub_4793A0(_DWORD *a1)
{
  if ( sub_4450B0(a1) )
    sub_461D70();
  else
    sub_461D80();
  return 0;
}

// ===== sub_4793D0 @ 0x004793D0..0x004793E4 =====
int __cdecl sub_4793D0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_461D00(v1);
  return 0;
}

// ===== sub_4793F0 @ 0x004793F0..0x00479452 =====
int __cdecl sub_4793F0(_DWORD *a1)
{
  unsigned int v1; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( v1 - 1 > 0x3E7 )
  {
    sprintf(Buffer, &byte_4E7F2C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_461D20(v1);
  return 0;
}

// ===== sub_479460 @ 0x00479460..0x004794BE =====
int __cdecl sub_479460(_DWORD *a1)
{
  unsigned int v1; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( v1 > 0x20000000 )
  {
    sprintf(Buffer, &byte_4E8374, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4398C0(v1);
  return 0;
}

// ===== sub_4794C0 @ 0x004794C0..0x004794E7 =====
int __cdecl sub_4794C0(_DWORD *a1)
{
  int v1; // edi

  v1 = sub_4450B0(a1);
  sub_497B60(v1);
  sub_461E60(v1);
  return 0;
}

// ===== sub_4794F0 @ 0x004794F0..0x00479529 =====
int __cdecl sub_4794F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497B60(v2);
  sub_461E70(v1, v2);
  return 0;
}

// ===== sub_479530 @ 0x00479530..0x00479555 =====
int __cdecl sub_479530(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_461E30(v1, v3);
  return 0;
}

// ===== sub_479560 @ 0x00479560..0x00479574 =====
int __cdecl sub_479560(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_402070(v1);
  return 0;
}

// ===== sub_479580 @ 0x00479580..0x00479598 =====
int __cdecl sub_479580(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_461E80(v1);
  return 0;
}

// ===== sub_4795A0 @ 0x004795A0..0x004795C7 =====
int __cdecl sub_4795A0(_DWORD *a1)
{
  int v1; // edi

  v1 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_461EA0(v1);
  return 0;
}

// ===== sub_4795D0 @ 0x004795D0..0x004795F3 =====
int __cdecl sub_4795D0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4319E0(v3, v1);
  return 0;
}

// ===== sub_479600 @ 0x00479600..0x00479616 =====
int __cdecl sub_479600(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_401B70();
  return 0;
}

// ===== sub_479620 @ 0x00479620..0x00479653 =====
int __cdecl sub_479620(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  sub_462920(v1, v2);
  return 0;
}

// ===== sub_479660 @ 0x00479660..0x004796C0 =====
int __cdecl sub_479660(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_42DC80(v1) )
  {
    sprintf(Buffer, &byte_4E83A4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_461E10();
  return 0;
}

// ===== sub_4796C0 @ 0x004796C0..0x004797AB =====
int __cdecl sub_4796C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  const char *v5; // [esp-8h] [ebp-120h]
  int v6; // [esp-4h] [ebp-11Ch]
  int v7; // [esp+Ch] [ebp-10Ch]
  int v8; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  switch ( sub_461DE0(v8, v2, v3, v1) )
  {
    case -2147483647:
      sprintf(Buffer, byte_4E8408, v1);
      goto LABEL_4;
    case -2147483646:
      sprintf(Buffer, &byte_4E82F0, v8);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      v6 = v2;
      v5 = &byte_4E83DC;
      goto LABEL_3;
    case -2147483644:
      v6 = v7;
      v5 = &byte_4E82C4;
LABEL_3:
      sprintf(Buffer, v5, v6);
LABEL_4:
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_4797C0 @ 0x004797C0..0x004797D4 =====
int __cdecl sub_4797C0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_461E20(v1);
  return 0;
}

// ===== sub_4797E0 @ 0x004797E0..0x0047998D =====
int __cdecl sub_4797E0(_DWORD *a1)
{
  const unsigned __int8 *v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  const char *v6; // [esp-Ch] [ebp-134h]
  int v7; // [esp-8h] [ebp-130h]
  const unsigned __int8 *v8; // [esp-4h] [ebp-12Ch]
  int v9; // [esp+14h] [ebp-114h]
  char Buffer[256]; // [esp+18h] [ebp-110h] BYREF
  int v11; // [esp+124h] [ebp-4h]

  v1 = (const unsigned __int8 *)sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v9 = sub_4450B0(a1);
  sub_497B60(v9);
  if ( !sub_401CE0((int)v1, v9, v2, 1) )
    return 0;
  if ( !_mbschr(v1, 0x2Fu) && sub_402080() )
  {
    switch ( sub_401E00((int)v1, v9, v2) )
    {
      case -2147483646:
        v8 = v1;
        v7 = v2;
        v6 = &byte_4E5628;
        goto LABEL_6;
      case -2147483645:
        sprintf(Buffer, &byte_4E5670, v2, v1);
        sub_4646F0(Buffer, (int)a1);
      case -2147483644:
        sprintf(Buffer, &byte_4E8438, v2, v1);
        sub_4646F0(Buffer, (int)a1);
      case -2147483643:
        v8 = v1;
        v7 = v2;
        v6 = &byte_4E5708;
LABEL_6:
        sprintf(Buffer, v6, v7, v8);
        goto LABEL_7;
      case -2147483642:
        sprintf(Buffer, &byte_4E574C, v2, v1);
        goto LABEL_7;
      case -2147483640:
        sprintf(Buffer, byte_4E8488, v2, v1);
LABEL_7:
        sub_4646F0(Buffer, (int)a1);
      default:
        return 0;
    }
    return 0;
  }
  v3 = operator new(0x674u);
  v11 = 0;
  if ( v3 )
    v4 = sub_439C70(v3, v9, v2);
  else
    v4 = 0;
  v11 = -1;
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_4799B0 @ 0x004799B0..0x00479A4B =====
int __cdecl sub_4799B0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v4; // [esp+Ch] [ebp-10Ch]
  int v5; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  if ( !sub_4026C0(v1, v5, v2, v4) )
  {
    sprintf(Buffer, &byte_4E84D0, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_479A50 @ 0x00479A50..0x00479A77 =====
int __cdecl sub_479A50(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4026F0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_479A80 @ 0x00479A80..0x00479ABE =====
int __cdecl sub_479A80(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  if ( !sub_4026E0(v1) )
    sub_497C00(v2);
  return 0;
}

// ===== sub_479AC0 @ 0x00479AC0..0x00479B24 =====
int __cdecl sub_479AC0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]
  int v6; // [esp+14h] [ebp-4h]

  v1 = sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_402420(0, v1, v2, v4, v5, v6);
  return 0;
}

// ===== sub_479B30 @ 0x00479B30..0x00479BF0 =====
int __cdecl sub_479B30(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  sub_48DF50(a1);
  sub_497B60(v1);
  v3 = sub_4026A0(v2, v5) - 9;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E799C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E8500, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_479C00 @ 0x00479C00..0x00479C38 =====
int __cdecl sub_479C00(_DWORD *a1)
{
  void *v1; // ebx
  _DWORD *v2; // esi
  int v3; // eax

  v1 = (void *)sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_402710(v1);
  *v2 = 0;
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_479C40 @ 0x00479C40..0x00479C70 =====
int __cdecl sub_479C40(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = (void *)sub_4450B0(a1);
  v2 = sub_402660(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_479C70 @ 0x00479C70..0x00479D9D =====
int __cdecl sub_479C70(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+14h] [ebp-10Ch]
  int v8; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497B60(v8);
  sub_497B60(v2);
  sub_497C40(v7);
  sub_497DB0(v1);
  v3 = sub_402720(v6, v2, v7, v1) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8548, v8);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E857C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E85B0, v8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_479DA0 @ 0x00479DA0..0x00479F07 =====
int __cdecl sub_479DA0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  const char *v4; // [esp-8h] [ebp-12Ch]
  int v5; // [esp-4h] [ebp-128h]
  int v6; // [esp+Ch] [ebp-118h]
  int v7; // [esp+10h] [ebp-114h]
  int v8; // [esp+14h] [ebp-110h]
  int v9; // [esp+18h] [ebp-10Ch]
  int v10; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  sub_497B60(v9);
  sub_497B60(v2);
  sub_497DB0(v1);
  switch ( sub_4027E0(v9, v7, v6, v2, v10, v8, v1) )
  {
    case 1:
      v5 = v9;
      v4 = &byte_4E8548;
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E857C, v2);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      v5 = v10;
      v4 = (const char *)&unk_4E8610;
      goto LABEL_3;
    case 4:
      sprintf(Buffer, &byte_4E85B0, v9, v2);
      goto LABEL_4;
    case 5:
      sprintf(Buffer, &byte_4E8648, v10);
      goto LABEL_4;
    case 6:
      v5 = v10;
      v4 = (const char *)&unk_4E8688;
LABEL_3:
      sprintf(Buffer, v4, v5);
LABEL_4:
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_479F20 @ 0x00479F20..0x0047A097 =====
int __cdecl sub_479F20(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v5; // [esp+Ch] [ebp-110h]
  int v6; // [esp+10h] [ebp-10Ch]
  int v7; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  switch ( sub_402970(v1, v2, v6, v7, v5, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4E86C4, v2);
      break;
    case 2:
      sprintf(Buffer, &byte_4E86FC, v6);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sprintf(Buffer, &byte_4E8738, v2, v6);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, &byte_4E8798, v7);
      break;
    case 5:
      sprintf(Buffer, &byte_4E87E0, v7, v2);
      break;
    case 6:
      sprintf(Buffer, byte_4E8870, v1);
      break;
    case 7:
      sprintf(Buffer, &byte_4E88B8, v1, v2);
      break;
    case 8:
      sprintf(Buffer, byte_4E8948, v5);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47A0C0 @ 0x0047A0C0..0x0047A1BE =====
int __cdecl sub_47A0C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-10Ch]
  int v5; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v4 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  switch ( sub_402AC0(v1, v5) )
  {
    case 1:
      sprintf(Buffer, &byte_4E86C4, v2);
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E86FC, v1);
      goto LABEL_5;
    case 3:
      sprintf(Buffer, &byte_4E8978, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, &byte_4E89DC, v5);
LABEL_5:
      sub_4646F0(Buffer, (int)a1);
    case 5:
      sprintf(Buffer, byte_4E8948, v4);
LABEL_3:
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_47A1E0 @ 0x0047A1E0..0x0047A36A =====
int __cdecl sub_47A1E0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  unsigned int v3; // edx
  const char *v5; // [esp-Ch] [ebp-13Ch]
  unsigned int v6; // [esp-8h] [ebp-138h]
  unsigned int v7; // [esp-4h] [ebp-134h]
  int v8; // [esp+Ch] [ebp-124h]
  int v9; // [esp+10h] [ebp-120h]
  int v10; // [esp+14h] [ebp-11Ch]
  int v11; // [esp+18h] [ebp-118h]
  int v12; // [esp+1Ch] [ebp-114h]
  unsigned int v13; // [esp+20h] [ebp-110h]
  unsigned int v14; // [esp+24h] [ebp-10Ch]
  unsigned int v15; // [esp+28h] [ebp-108h]
  char Buffer[256]; // [esp+2Ch] [ebp-104h] BYREF

  v10 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  switch ( sub_402C90(v11, v2, __SPAIR64__(v12, v9), v15, v13, v1, v8, v14, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4E86C4, v2);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E86FC, v1);
      sub_4646F0(Buffer, (int)a1);
    case 5:
      v7 = v13;
      v6 = v15;
      v5 = &byte_4E8A10;
      goto LABEL_5;
    case 6:
      v7 = v10;
      v6 = v14;
      v5 = (const char *)&unk_4E8A3C;
LABEL_5:
      sprintf(Buffer, v5, v6, v7);
      break;
    case 8:
      sprintf(Buffer, &byte_4E8738, v2, v1);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47A390 @ 0x0047A390..0x0047A476 =====
int __cdecl sub_47A390(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v5; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  switch ( sub_402EA0(v1, v5, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4E86C4, v2);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E86FC, v1);
      break;
    case 3:
      sprintf(Buffer, &byte_4E8A68, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, &byte_4E8AAC, v5);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47A490 @ 0x0047A490..0x0047A58E =====
int __cdecl sub_47A490(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-118h]
  int v7; // [esp+10h] [ebp-114h]
  int v8; // [esp+14h] [ebp-110h]
  int v9; // [esp+18h] [ebp-10Ch]
  int v10; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4032C0(v8, v2, v9, v10, v1, v6, v7, v3) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E8AD4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E8B04, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47A590 @ 0x0047A590..0x0047A69D =====
int __cdecl sub_47A590(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v5; // eax
  int v7; // [esp+Ch] [ebp-114h]
  int v8; // [esp+10h] [ebp-110h]
  int v9; // [esp+14h] [ebp-10Ch]
  int v10; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4033A0(v2, v10, v9, v7, v3, v1) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E8B34, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v5 = v4 - 1;
  if ( !v5 )
  {
    sprintf(Buffer, &byte_4E8B64, v10);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 1 )
  {
    sprintf(Buffer, &byte_4E8B94, v8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47A6A0 @ 0x0047A6A0..0x0047A789 =====
int __cdecl sub_47A6A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  void *v5; // [esp+Ch] [ebp-114h]
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = (void *)sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_491B40((int)a1, 0, 0, 0, 0, v2, v7, v5, 0, v6, v1);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7F2C, v5);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 2;
}

// ===== sub_47A790 @ 0x0047A790..0x0047A88B =====
int __cdecl sub_47A790(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-118h]
  int v6; // [esp+10h] [ebp-114h]
  int v7; // [esp+18h] [ebp-10Ch]
  void *v8; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v8 = (void *)sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_491B40((int)a1, 0, 0, 0, 0, v2, v7, v8, v5, v6, v1);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7F2C, v8);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 2;
}

// ===== sub_47A890 @ 0x0047A890..0x0047A9AF =====
int __cdecl sub_47A890(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  void *v5; // [esp+Ch] [ebp-120h]
  int v6; // [esp+10h] [ebp-11Ch]
  int v7; // [esp+18h] [ebp-114h]
  int v8; // [esp+1Ch] [ebp-110h]
  int v9; // [esp+20h] [ebp-10Ch]
  int v10; // [esp+24h] [ebp-108h]
  char Buffer[256]; // [esp+28h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = (void *)sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_491B40((int)a1, v8, v9, v10, 1, v2, v6, v5, 0, v7, v1);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7F2C, v5);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 2;
}

// ===== sub_47A9B0 @ 0x0047A9B0..0x0047AAE1 =====
int __cdecl sub_47A9B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-124h]
  int v6; // [esp+10h] [ebp-120h]
  int v7; // [esp+18h] [ebp-118h]
  int v8; // [esp+1Ch] [ebp-114h]
  void *v9; // [esp+20h] [ebp-110h]
  int v10; // [esp+24h] [ebp-10Ch]
  int v11; // [esp+28h] [ebp-108h]
  char Buffer[256]; // [esp+2Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v9 = (void *)sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_491B40((int)a1, v8, v10, v11, 1, v2, v6, v9, v5, v7, v1);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7F2C, v9);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 2;
}

// ===== sub_47AAF0 @ 0x0047AAF0..0x0047AC76 =====
int __cdecl sub_47AAF0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-12Ch]
  int v6; // [esp+10h] [ebp-128h]
  int v7; // [esp+18h] [ebp-120h]
  int v8; // [esp+1Ch] [ebp-11Ch]
  int v9; // [esp+20h] [ebp-118h]
  int v10; // [esp+24h] [ebp-114h]
  int v11; // [esp+28h] [ebp-110h]
  int v12; // [esp+2Ch] [ebp-10Ch]
  int v13; // [esp+30h] [ebp-108h]
  char Buffer[256]; // [esp+34h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_491C40(a1, v8, v10, v11, v12, v9, v2, v7, v13, v5, v6, v1);
  switch ( v3 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7F2C, v13);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E8BF0, v8, v11);
      sub_4646F0(Buffer, (int)a1);
    case -1:
      sub_4646F0(byte_4E8BC0, (int)a1);
  }
  return 2;
}

// ===== sub_47AC80 @ 0x0047AC80..0x0047ADE3 =====
int __cdecl sub_47AC80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-12Ch]
  int v6; // [esp+10h] [ebp-128h]
  void *v7; // [esp+18h] [ebp-120h]
  int v8; // [esp+1Ch] [ebp-11Ch]
  int v9; // [esp+20h] [ebp-118h]
  int v10; // [esp+24h] [ebp-114h]
  int v11; // [esp+28h] [ebp-110h]
  int v12; // [esp+2Ch] [ebp-10Ch]
  int v13; // [esp+30h] [ebp-108h]
  char Buffer[256]; // [esp+34h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = (void *)sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497D50(v2);
  sub_497DB0(v13);
  v3 = sub_491D60((int)a1, v8, v10, v11, v13, v12, v2, v9, v7, v5, v6, v1);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7F2C, v7);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 2;
}

// ===== sub_47ADF0 @ 0x0047ADF0..0x0047AF7B =====
int __cdecl sub_47ADF0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-12Ch]
  int v6; // [esp+10h] [ebp-128h]
  int v7; // [esp+18h] [ebp-120h]
  int v8; // [esp+1Ch] [ebp-11Ch]
  int v9; // [esp+20h] [ebp-118h]
  int v10; // [esp+24h] [ebp-114h]
  int v11; // [esp+28h] [ebp-110h]
  int v12; // [esp+2Ch] [ebp-10Ch]
  int v13; // [esp+30h] [ebp-108h]
  char Buffer[256]; // [esp+34h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_48DF50(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497D50(v2);
  sub_497DB0(v13);
  v3 = sub_491E60(a1, v8, v10, v11, v13, v12, v2, v9, v7, v5, v6, v1);
  switch ( v3 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7F2C, v7);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E8C4C, v8);
      sub_4646F0(Buffer, (int)a1);
    case -1:
      sub_4646F0(byte_4E8BC0, (int)a1);
  }
  return 2;
}

// ===== sub_47AF80 @ 0x0047AF80..0x0047B0CC =====
int __cdecl sub_47AF80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  const char *v5; // [esp-8h] [ebp-12Ch]
  int v6; // [esp-4h] [ebp-128h]
  int v7; // [esp+Ch] [ebp-118h]
  int v8; // [esp+10h] [ebp-114h]
  int v9; // [esp+14h] [ebp-110h]
  int v10; // [esp+18h] [ebp-10Ch]
  int v11; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  sub_4450B0(a1);
  switch ( sub_491F90(a1, v10, v8, v11, v2, v7, v1, v9, v3) )
  {
    case 0x80000001:
      v6 = v10;
      v5 = &byte_4E7FD8;
      break;
    case 0x80000002:
      sprintf(Buffer, &byte_4E8004, v11);
      sub_4646F0(Buffer, (int)a1);
    case 0x80000003:
      v6 = v2;
      v5 = &byte_4E802C;
      break;
    case 0x80000004:
      v6 = v1;
      if ( v1 )
        v5 = (const char *)&unk_4E8C78;
      else
        v5 = &byte_4E7F2C;
      break;
    default:
      return 2;
  }
  sprintf(Buffer, v5, v6);
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47B0E0 @ 0x0047B0E0..0x0047B116 =====
int __cdecl sub_47B0E0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_461F10(v2, v1) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B120 @ 0x0047B120..0x0047B156 =====
int __cdecl sub_47B120(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_461F20(v2, v1) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B160 @ 0x0047B160..0x0047B1A2 =====
int __cdecl sub_47B160(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_461F40(v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B1B0 @ 0x0047B1B0..0x0047B1F3 =====
int __cdecl sub_47B1B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_461F50(v3, v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B200 @ 0x0047B200..0x0047B242 =====
int __cdecl sub_47B200(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_461F80(v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B250 @ 0x0047B250..0x0047B292 =====
int __cdecl sub_47B250(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_461FA0(v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B2A0 @ 0x0047B2A0..0x0047B2E3 =====
int __cdecl sub_47B2A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_461FC0(v3, v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B2F0 @ 0x0047B2F0..0x0047B333 =====
int __cdecl sub_47B2F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_461FB0(v3, v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B340 @ 0x0047B340..0x0047B40E =====
int __cdecl sub_47B340(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v6; // eax
  int v8; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_462010(v1, v2, v3, v4) - 5;
  if ( !v5 )
  {
    sprintf(Buffer, &byte_4E8CE0, v8);
    sub_4646F0(Buffer, (int)a1);
  }
  v6 = v5 - 249;
  if ( !v6 )
  {
    sprintf(Buffer, &byte_4E8D20, v8, v2, v2, v1, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v6 == 1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B410 @ 0x0047B410..0x0047B452 =====
int __cdecl sub_47B410(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_461F90(v1, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B460 @ 0x0047B460..0x0047B4A5 =====
int __cdecl sub_47B460(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497BB0(v1);
  if ( sub_462040(v1, v2) == 255 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B4B0 @ 0x0047B4B0..0x0047B537 =====
int __cdecl sub_47B4B0(_DWORD *a1)
{
  int v1; // edi
  void *v2; // eax
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v3 = sub_462050(v1, v2) - 1;
  if ( !v3 )
    sub_4646F0(byte_4E8D78, (int)a1);
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E8DB4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 253 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B540 @ 0x0047B540..0x0047B57D =====
int __cdecl sub_47B540(_DWORD *a1)
{
  int v1; // eax
  int v3; // [esp+8h] [ebp-4h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_462070(v1, &v3) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_47B580 @ 0x0047B580..0x0047B5CC =====
int __cdecl sub_47B580(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4620E0(v1) - 3;
  if ( !v2 )
    sub_4646F0(byte_4E8DE0, (int)a1);
  v3 = v2 - 1;
  if ( !v3 )
    sub_4646F0(byte_4E8E30, (int)a1);
  if ( v3 == 251 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_47B5D0 @ 0x0047B5D0..0x0047B62D =====
int __cdecl sub_47B5D0(_DWORD *a1)
{
  int v1; // edi
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_462130(v1) )
  {
    sprintf(Buffer, &byte_4E8E80, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47B630 @ 0x0047B630..0x0047B6C0 =====
int __cdecl sub_47B630(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_462140(v1, v2, v4) )
  {
    sprintf(Buffer, &byte_4E8EC8, v4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47B6C0 @ 0x0047B6C0..0x0047B7B2 =====
int __cdecl sub_47B6C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-114h]
  int v7; // [esp+10h] [ebp-110h]
  int v8; // [esp+14h] [ebp-10Ch]
  int v9; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_462150(v3, v9, v7, v8, v2, v1) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E8F20, v9, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E8F68, v7, v8, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47B7C0 @ 0x0047B7C0..0x0047B916 =====
int __cdecl sub_47B7C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp-4h] [ebp-130h]
  int v5; // [esp+Ch] [ebp-120h]
  int v6; // [esp+10h] [ebp-11Ch]
  int v7; // [esp+14h] [ebp-118h]
  int v8; // [esp+18h] [ebp-114h]
  int v9; // [esp+1Ch] [ebp-110h]
  int v10; // [esp+20h] [ebp-10Ch]
  int v11; // [esp+24h] [ebp-108h]
  char Buffer[256]; // [esp+28h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497DB0(v1);
  switch ( sub_462170(v1, v7, v8, v6, v10, v9, v5, v11, v2) )
  {
    case 1:
      v4 = v10;
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E799C, v11);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      v4 = v2;
LABEL_3:
      sprintf(Buffer, &byte_4E799C, v4);
      break;
    case 4:
      sprintf(Buffer, &byte_4E8648, v2);
      break;
    case 5:
      sprintf(Buffer, byte_4E8FC8, v2);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47B930 @ 0x0047B930..0x0047BA75 =====
int __cdecl sub_47B930(_DWORD *a1)
{
  int v1; // esi
  int v2; // edx
  int v3; // edi
  int i; // eax
  int v5; // eax
  int j; // esi
  int v8; // eax
  char v9; // cl
  int v10; // [esp+Ch] [ebp-888h]
  _DWORD v11[32]; // [esp+10h] [ebp-884h] BYREF
  char v12[1024]; // [esp+90h] [ebp-804h] BYREF
  char Buffer[1024]; // [esp+490h] [ebp-404h] BYREF

  v1 = sub_4450B0(a1);
  v10 = sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  for ( i = 0; i < v3; ++i )
  {
    if ( i >= 32 )
      break;
    v11[i] = *(_DWORD *)((char *)&v11[i] + v2 - (_DWORD)v11);
  }
  v5 = sub_4621A0(v1, (int)v11, v3) - 1;
  if ( !v5 )
  {
    sprintf(Buffer, &byte_4E7BE8, v3);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 1 )
  {
    sprintf(Buffer, &byte_4E900C);
    for ( j = 0; j < v3; ++j )
    {
      if ( j >= v3 - 1 )
        sprintf(v12, aSD_0, Buffer, *(_DWORD *)(v10 + 4 * j));
      else
        sprintf(v12, "%s%d , ", Buffer, *(_DWORD *)(v10 + 4 * j));
      v8 = 0;
      do
      {
        v9 = v12[v8];
        Buffer[v8++] = v9;
      }
      while ( v9 );
    }
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47BA80 @ 0x0047BA80..0x0047BBBF =====
int __cdecl sub_47BA80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  const char *v4; // [esp-8h] [ebp-124h]
  int v5; // [esp-4h] [ebp-120h]
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v6 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_497DB0(v1);
  switch ( sub_4621B0(v6, v1, v7, v8, v2) )
  {
    case 1:
      v5 = v7;
      v4 = &byte_4E799C;
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E9078, v7);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sprintf(Buffer, &byte_4E799C, v8);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      v5 = v8;
      v4 = byte_4E90C0;
LABEL_3:
      sprintf(Buffer, v4, v5);
      break;
    case 5:
      sprintf(Buffer, &byte_4E799C, v2);
      break;
    case 6:
      sprintf(Buffer, byte_4E90C0, v2);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47BBE0 @ 0x0047BBE0..0x0047BCB0 =====
int __cdecl sub_47BBE0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  unsigned int v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  v3 = sub_4621D0(v1, v6, v2) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E799C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E9078, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E89DC, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47BCC0 @ 0x0047BCC0..0x0047BDF7 =====
int __cdecl sub_47BCC0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  const char *v4; // [esp-8h] [ebp-124h]
  int v5; // [esp-4h] [ebp-120h]
  unsigned int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497DB0(v1);
  switch ( sub_4621E0(v1, v6, v8, v2, v7) )
  {
    case 1:
      v5 = v8;
      v4 = &byte_4E799C;
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E9078, v8);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      v5 = v2;
      v4 = (const char *)&unk_4E9124;
      goto LABEL_3;
    case 4:
      sprintf(Buffer, byte_4E9168, v2);
      break;
    case 5:
      v5 = v7;
      v4 = &byte_4E89DC;
LABEL_3:
      sprintf(Buffer, v4, v5);
      break;
    case 6:
      sprintf(Buffer, byte_4E91E4, v6);
      break;
    case 7:
      sprintf(Buffer, &byte_4E9210, v6, v2);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47BE20 @ 0x0047BE20..0x0047BEE5 =====
int __cdecl sub_47BE20(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-10Ch]
  int v7; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_462200(v1, v2, v7, v6, v3) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E799C, v7);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E925C, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47BEF0 @ 0x0047BEF0..0x0047BF8C =====
int __cdecl sub_47BEF0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_462220(v3, v1, v2) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E799C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E8AAC, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47BF90 @ 0x0047BF90..0x0047C078 =====
int __cdecl sub_47BF90(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // ebx
  int v3; // edx
  const char *v5; // [esp-8h] [ebp-120h]
  int v6; // [esp-4h] [ebp-11Ch]
  int v7; // [esp+Ch] [ebp-10Ch]
  int v8; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  switch ( sub_462230(v1, v3, v7, v8, v2) )
  {
    case 1:
      v6 = v7;
      v5 = &byte_4E9288;
      goto LABEL_3;
    case 2:
      sprintf(Buffer, &byte_4E92D8, v8);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      v6 = v2;
      v5 = (const char *)&unk_4E9324;
LABEL_3:
      sprintf(Buffer, v5, v6);
      break;
    case 4:
      sprintf(Buffer, byte_4E934C, v1);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}
