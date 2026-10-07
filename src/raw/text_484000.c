#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4840A0 @ 0x004840A0..0x004841C3 =====
int __cdecl sub_4840A0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  unsigned int v3; // eax
  unsigned int v4; // eax
  int v6; // [esp+Ch] [ebp-118h]
  int v7; // [esp+10h] [ebp-114h]
  int v8; // [esp+14h] [ebp-110h]
  int v9; // [esp+18h] [ebp-10Ch]
  int v10; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497AF0();
  v3 = sub_462BF0(v1, v7, v6, v2, v10, v9, v8);
  if ( v3 > 3 )
  {
    if ( v3 == -1 )
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  else
  {
    if ( v3 == 3 )
    {
      sprintf(Buffer, &byte_4E8264, v2);
LABEL_6:
      sub_4646F0(Buffer, (int)a1);
    }
    v4 = v3 - 1;
    if ( !v4 )
    {
      sprintf(Buffer, &byte_4E7970, v10);
      sub_4646F0(Buffer, (int)a1);
    }
    if ( v4 == 1 )
    {
      sprintf(Buffer, &byte_4E823C, v9);
      goto LABEL_6;
    }
  }
  return 0;
}

// ===== sub_4841D0 @ 0x004841D0..0x00484249 =====
int __cdecl sub_4841D0(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462C60(v1, v2);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EB204, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_484250 @ 0x00484250..0x004842C9 =====
int __cdecl sub_484250(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462B90(v1, v2);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EB22C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_4842D0 @ 0x004842D0..0x00484349 =====
int __cdecl sub_4842D0(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462BC0(v1, v2);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EB264, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_484350 @ 0x00484350..0x00484393 =====
int __cdecl sub_484350(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_463100(v3, v1, v2) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_4843A0 @ 0x004843A0..0x004843EA =====
int __cdecl sub_4843A0(_DWORD *a1)
{
  int v1; // eax
  BOOL v2; // eax
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  int v6[2]; // [esp+8h] [ebp-8h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_463110(v1, v6);
  if ( !v2 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  v3 = sub_4450D0(a1, v2);
  v4 = sub_4450D0(v3, v6[0]);
  sub_4450D0(v4, v6[1]);
  return 0;
}

// ===== sub_4843F0 @ 0x004843F0..0x0048442A =====
int __cdecl sub_4843F0(_DWORD *a1)
{
  int v1; // eax
  BOOL v3; // [esp+8h] [ebp-4h] BYREF

  v1 = sub_4450B0(a1);
  if ( sub_463120(v1, &v3) == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_484430 @ 0x00484430..0x004844E8 =====
int __cdecl sub_484430(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+14h] [ebp-Ch]
  int v8; // [esp+18h] [ebp-8h]
  int v9; // [esp+1Ch] [ebp-4h]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4911E0(a1, v3, v4, 0, v7, v8, v9, v2, v1, 0);
  if ( v5 == -2147483647 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v5 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 2;
}

// ===== sub_4844F0 @ 0x004844F0..0x00484578 =====
int __cdecl sub_4844F0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  _DWORD v5[5]; // [esp+Ch] [ebp-20h] BYREF
  int v6; // [esp+20h] [ebp-Ch]
  const char *v7; // [esp+24h] [ebp-8h]
  int v8; // [esp+28h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = (const char *)sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  sub_433570(v5);
  v3 = sub_4630A0((int)v5, v2, v6, v7, v8, v1, v2);
  if ( v3 == 1 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_484580 @ 0x00484580..0x00484644 =====
int __cdecl sub_484580(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+14h] [ebp-10h]
  int v8; // [esp+18h] [ebp-Ch]
  int v9; // [esp+1Ch] [ebp-8h]
  int v10; // [esp+20h] [ebp-4h]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4911E0(a1, v3, v4, v7, v8, v9, v10, v2, v1, 0);
  if ( v5 == -2147483647 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v5 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 2;
}

// ===== sub_484650 @ 0x00484650..0x00484706 =====
int __cdecl sub_484650(_DWORD *a1)
{
  int v1; // edi
  int v2; // esi
  int v3; // eax
  unsigned int v5[5]; // [esp+Ch] [ebp-28h] BYREF
  int v6; // [esp+20h] [ebp-14h]
  const char *v7; // [esp+24h] [ebp-10h]
  int v8; // [esp+28h] [ebp-Ch]
  int v9; // [esp+2Ch] [ebp-8h]
  int v10; // [esp+30h] [ebp-4h]

  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v10 = v2;
  v7 = (const char *)sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  if ( v1 )
  {
    sub_434E30(0, v5, 0, 0, 0, 0);
    v2 = v10;
  }
  else
  {
    sub_433570(v5);
  }
  v3 = sub_4630A0((int)v5, v2, v6, v7, v8, v9, v2);
  if ( v3 == 1 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_484710 @ 0x00484710..0x00484737 =====
int __cdecl sub_484710(int a1)
{
  int v1; // edi
  const char *v2; // eax

  v1 = sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  sub_463270(v1, v2);
  return 0;
}

// ===== sub_484740 @ 0x00484740..0x00484770 =====
int __cdecl sub_484740(_DWORD *a1)
{
  const char *v1; // esi
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  sub_48DF50(a1);
  v2 = sub_4632A0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_484770 @ 0x00484770..0x00484795 =====
int __cdecl sub_484770(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  v1 = (void *)sub_48DF50(a1);
  v2 = sub_4632B0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4847A0 @ 0x004847A0..0x004847F5 =====
int __cdecl sub_4847A0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  _DWORD *v4; // edx
  int v5; // ebx
  _DWORD *v6; // edx
  _DWORD *v7; // edx
  int v8; // eax
  int v10; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v5 = sub_4450B0(v4);
  v10 = sub_4450B0(v6);
  v8 = sub_4450B0(v7);
  sub_4632C0(-1, -1, v8, v10, v5, v3, v1);
  return 0;
}

// ===== sub_484800 @ 0x00484800..0x004848D2 =====
int __cdecl sub_484800(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-10Ch]
  int v8; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4632F0(v4, v8, v3, v7, v2, v1);
  if ( v5 == -2147483647 )
  {
    sprintf(Buffer, &byte_4EB2A4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == -2147483646 )
  {
    sprintf(Buffer, &byte_4EB2DC, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4848E0 @ 0x004848E0..0x00484905 =====
int __cdecl sub_4848E0(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_463390(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_484910 @ 0x00484910..0x004849A3 =====
int __cdecl sub_484910(_DWORD *a1)
{
  int v1; // edi
  unsigned int v2; // eax
  int v3; // eax
  int v4; // edx
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_463310(v1, v2);
  if ( v3 == -2147483641 )
  {
    sprintf(Buffer, &byte_4EB314, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483640 )
  {
    sprintf(Buffer, &byte_4EB348, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4849B0 @ 0x004849B0..0x00484A2A =====
int __cdecl sub_4849B0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  int v3; // eax
  int v5; // [esp+10h] [ebp-Ch]
  int v6; // [esp+14h] [ebp-8h]
  int v7; // [esp+18h] [ebp-4h]

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_403BA0(v2, v5, v6, v7, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_484A30 @ 0x00484A30..0x00484C29 =====
int __cdecl sub_484A30(_DWORD *a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  int v3; // esi
  _DWORD v5[5]; // [esp+Ch] [ebp-14Ch] BYREF
  int v6; // [esp+20h] [ebp-138h]
  int v7; // [esp+24h] [ebp-134h]
  int v8; // [esp+28h] [ebp-130h]
  int v9; // [esp+2Ch] [ebp-12Ch]
  int v10; // [esp+30h] [ebp-128h]
  int v11; // [esp+34h] [ebp-124h]
  int v12; // [esp+38h] [ebp-120h]
  int v13; // [esp+3Ch] [ebp-11Ch]
  int v14; // [esp+40h] [ebp-118h]
  int v15; // [esp+44h] [ebp-114h]
  int v16; // [esp+48h] [ebp-110h]
  int v17; // [esp+4Ch] [ebp-10Ch]
  int v18; // [esp+50h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+54h] [ebp-104h] BYREF

  v8 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v10 = sub_48DF50(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_48DF50(a1);
  v11 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  sub_497B60(v17);
  sub_497AF0();
  v2 = sub_433570(v5);
  v3 = v15;
  switch ( sub_403B10((int)&v18, v18, v11, v13, v14, v10, v1, v16, v15, v12, v6, v7, v9, v8, v8, (int)v2) )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7970, v16);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E823C, v3);
      break;
    case -2147483645:
      sprintf(Buffer, &byte_4E8264, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v17);
      break;
    default:
      sub_4450D0(a1, v18);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_484C40 @ 0x00484C40..0x00484E7E =====
int __cdecl sub_484C40(_DWORD *a1)
{
  int v1; // edi
  int v2; // esi
  unsigned int v4[5]; // [esp+Ch] [ebp-154h] BYREF
  int v5; // [esp+20h] [ebp-140h]
  int v6; // [esp+24h] [ebp-13Ch]
  int v7; // [esp+28h] [ebp-138h]
  int v8; // [esp+2Ch] [ebp-134h]
  int v9; // [esp+30h] [ebp-130h]
  int v10; // [esp+34h] [ebp-12Ch]
  int v11; // [esp+38h] [ebp-128h]
  int v12; // [esp+3Ch] [ebp-124h]
  int v13; // [esp+40h] [ebp-120h]
  int v14; // [esp+44h] [ebp-11Ch]
  int v15; // [esp+48h] [ebp-118h]
  int v16; // [esp+4Ch] [ebp-114h]
  int v17; // [esp+50h] [ebp-110h]
  int v18; // [esp+54h] [ebp-10Ch]
  int v19; // [esp+58h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+5Ch] [ebp-104h] BYREF

  v11 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v8 = v1;
  v15 = sub_48DF50(a1);
  v14 = sub_4450B0(a1);
  v12 = sub_48DF50(a1);
  v10 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  sub_497B60(v18);
  sub_497AF0();
  if ( v11 )
  {
    sub_434E30(0, v4, 0, 0, 0, 0);
    v1 = v8;
  }
  else
  {
    sub_433570(v4);
  }
  v2 = v7;
  switch ( sub_403B10((int)&v19, v19, v10, v12, v14, v15, v1, v17, v7, v16, v5, v13, v9, v6, v6, (int)v4) )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7970, v17);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E823C, v2);
      break;
    case -2147483645:
      sprintf(Buffer, &byte_4E8264, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v18);
      break;
    default:
      sub_4450D0(a1, v19);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_484E90 @ 0x00484E90..0x00484EC0 =====
int __cdecl sub_484E90(_DWORD *a1)
{
  char *v1; // esi
  char *v2; // eax
  int v3; // eax

  v1 = (char *)sub_48DF50(a1);
  v2 = (char *)sub_48DF50(a1);
  v3 = sub_463340(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_484EC0 @ 0x00484EC0..0x00484EE5 =====
int __cdecl sub_484EC0(int a1)
{
  const char *v1; // edi
  _BYTE *v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (_BYTE *)sub_48DF50(a1);
  sub_463380(v1, v2);
  return 0;
}

// ===== sub_484EF0 @ 0x00484EF0..0x00484F19 =====
int __cdecl sub_484EF0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46C630(v1, (void *)1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_484F20 @ 0x00484F20..0x00484F53 =====
int __cdecl sub_484F20(_DWORD *a1)
{
  int *v1; // esi
  int v2; // eax
  int v3; // ecx
  int v4; // eax

  v1 = (int *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_46CB60(v3, v2, v1, (int)a1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_484F60 @ 0x00484F60..0x00484FA8 =====
int __cdecl sub_484F60(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // ecx
  int v6; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v6 = sub_46CCA0(v5, v3, v4, v2, v1);
  sub_4450D0(a1, v6);
  return 0;
}

// ===== sub_484FB0 @ 0x00484FB0..0x0048501A =====
int __cdecl sub_484FB0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_447BB0(v1) )
  {
    sprintf(Buffer, &byte_4EB380, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_485020 @ 0x00485020..0x00485038 =====
int __cdecl sub_485020(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4639A0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_485040 @ 0x00485040..0x00485100 =====
int __cdecl sub_485040(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_48F7B0(v3, v2, v1);
  if ( v4 > 0x80000002 )
  {
    if ( v4 == -2147483645 )
    {
      v4 = 3;
    }
    else if ( v4 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000002:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_485100 @ 0x00485100..0x0048516C =====
int __cdecl sub_485100(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_48F8B0(v1);
  if ( v2 )
  {
    if ( v2 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v2 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v2 = 0;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_485170 @ 0x00485170..0x004851CF =====
int __cdecl sub_485170(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_48F8C0();
  if ( v1 )
  {
    if ( v1 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v1 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v1 = 0;
  }
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4851D0 @ 0x004851D0..0x0048523E =====
int __cdecl sub_4851D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_48F8D0(v1);
  if ( v2 )
  {
    if ( v2 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v2 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v2 = 0;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_485240 @ 0x00485240..0x004852FC =====
int __cdecl sub_485240(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // eax
  int xRight; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  xRight = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_48F910(v2, v3, xRight, v1);
  if ( v4 > 0x80000002 )
  {
    if ( v4 == -2147483644 )
      v4 = 4;
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000002:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_485300 @ 0x00485300..0x0048537E =====
int __cdecl sub_485300(_DWORD *a1)
{
  int v1; // eax
  unsigned int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48F930(v1);
  if ( v2 > 0x80000002 )
  {
    if ( v2 == -2147483644 )
      v2 = 4;
  }
  else
  {
    switch ( v2 )
    {
      case 0x80000002:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_485380 @ 0x00485380..0x004853E3 =====
int __cdecl sub_485380(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48F940(v1);
  if ( v2 )
  {
    if ( v2 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v2 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v2 = 0;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4853F0 @ 0x004853F0..0x00485422 =====
int __cdecl sub_4853F0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  BOOL v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_48F960(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_485430 @ 0x00485430..0x00485492 =====
int __cdecl sub_485430(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(_DWORD *); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = (int (__cdecl *)(_DWORD *))dword_504F00[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB3B4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2(a1);
}

// ===== sub_4854A0 @ 0x004854A0..0x00485542 =====
int __cdecl sub_4854A0(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // edi
  int v3; // edx
  unsigned int v5; // [esp+Ch] [ebp-10Ch]
  int v6; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( sub_461ED0(v3, v1, v2, v6, v5) == 16 )
  {
    sprintf(Buffer, &byte_4EB3E8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_485550 @ 0x00485550..0x0048561A =====
int __cdecl sub_485550(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // edi
  int v3; // edx
  int v5; // [esp+Ch] [ebp-114h]
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+14h] [ebp-10Ch]
  unsigned int v8; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( sub_461EF0(v3, v1, v2, v7, v8, v5, v6) == 16 )
  {
    sprintf(Buffer, &byte_4EB3E8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_485620 @ 0x00485620..0x00485713 =====
int __cdecl sub_485620(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-10Ch]
  int v7; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_404E20(v1, v7, v6, v3);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EB410, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483639:
      sprintf(Buffer, &byte_4EB49C, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_485720 @ 0x00485720..0x004857E1 =====
int __cdecl sub_485720(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_404FA0(v1);
  switch ( v3 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EB410, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483638:
      sprintf(Buffer, &byte_4EB4C8, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4857F0 @ 0x004857F0..0x0048582C =====
int __cdecl sub_4857F0(_DWORD *a1)
{
  int v1; // esi
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_402440(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_485830 @ 0x00485830..0x0048586C =====
int __cdecl sub_485830(_DWORD *a1)
{
  int v1; // esi
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_4024A0(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_485870 @ 0x00485870..0x004858F7 =====
int __cdecl sub_485870(int a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  _DWORD *v3; // eax

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = operator new(0x678u);
  if ( v2 )
    v3 = sub_452290(v2, v1);
  else
    v3 = 0;
  sub_4451C0(a1, (int)v3);
  return 2;
}

// ===== sub_485900 @ 0x00485900..0x00485908 =====
int sub_485900()
{
  sub_401DF0();
  return 0;
}

// ===== sub_485910 @ 0x00485910..0x00485942 =====
int __cdecl sub_485910(_DWORD *a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_402470(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_485950 @ 0x00485950..0x0048599E =====
int __cdecl sub_485950(_DWORD *a1)
{
  int v1; // esi
  int v2; // edi
  _DWORD *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_4025E0(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4859A0 @ 0x004859A0..0x00485A2F =====
int __cdecl sub_4859A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_403C00(v2) - 9;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EB4F4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EB528, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_485A30 @ 0x00485A30..0x00485A5C =====
int __cdecl sub_485A30(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_403C10();
  sub_4450D0(a1, v1 == 0);
  return 0;
}

// ===== sub_485A60 @ 0x00485A60..0x00485B89 =====
int __cdecl sub_485A60(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v5; // [esp+Ch] [ebp-110h]
  int v6; // [esp+10h] [ebp-10Ch]
  int v7; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  switch ( sub_403C20(v6, v7, v5, v1, v3) )
  {
    case -2147483639:
      sprintf(Buffer, &byte_4EA034, v6);
      sub_4646F0(Buffer, (int)a1);
    case -2147483638:
      sprintf(Buffer, &byte_4E9FA4, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483637:
      sprintf(Buffer, &byte_4E9FA4, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483636:
      sprintf(Buffer, &byte_4EB560, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_485BA0 @ 0x00485BA0..0x00485D3C =====
int __cdecl sub_485BA0(_DWORD *a1)
{
  int v1; // ebx
  int v3; // [esp+Ch] [ebp-128h]
  int v4; // [esp+10h] [ebp-124h]
  int v5; // [esp+14h] [ebp-120h]
  int v6; // [esp+18h] [ebp-11Ch]
  int v7; // [esp+1Ch] [ebp-118h]
  int v8; // [esp+20h] [ebp-114h]
  int v9; // [esp+24h] [ebp-110h]
  int v10; // [esp+28h] [ebp-10Ch]
  int v11; // [esp+2Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v3 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  sub_497B60(v10);
  sub_497AF0();
  switch ( sub_403600(v11, v7, v4, v1, v9, v6, v8, v5, v3, (int)&v11) )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7970, v9);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E823C, v6);
      break;
    case -2147483645:
      sprintf(Buffer, &byte_4E8264, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v10);
      break;
    default:
      sub_4450D0(a1, v11);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_485D50 @ 0x00485D50..0x00485F00 =====
int __cdecl sub_485D50(_DWORD *a1)
{
  int v1; // ebx
  int v3; // [esp+Ch] [ebp-12Ch]
  int v4; // [esp+10h] [ebp-128h]
  int v5; // [esp+14h] [ebp-124h]
  int v6; // [esp+18h] [ebp-120h]
  int v7; // [esp+1Ch] [ebp-11Ch]
  int v8; // [esp+20h] [ebp-118h]
  int v9; // [esp+24h] [ebp-114h]
  int v10; // [esp+28h] [ebp-110h]
  int v11; // [esp+2Ch] [ebp-10Ch]
  int v12; // [esp+30h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+34h] [ebp-104h] BYREF

  v4 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v8 = sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  sub_497B60(v11);
  sub_497AF0();
  switch ( sub_403680(v12, v7, v8, v1, v10, v6, v9, v3, v5, v4, (int)&v12) )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7970, v10);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E823C, v6);
      break;
    case -2147483645:
      sprintf(Buffer, &byte_4E8264, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v11);
      break;
    default:
      sub_4450D0(a1, v12);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_485F10 @ 0x00485F10..0x0048608E =====
int __cdecl sub_485F10(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-124h]
  _BYTE *v5; // [esp+10h] [ebp-120h]
  int v6; // [esp+14h] [ebp-11Ch]
  unsigned int v7; // [esp+18h] [ebp-118h]
  int v8; // [esp+1Ch] [ebp-114h]
  int v9; // [esp+20h] [ebp-110h]
  int v10; // [esp+24h] [ebp-10Ch]
  int v11; // [esp+28h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+2Ch] [ebp-104h] BYREF

  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v5 = (_BYTE *)sub_48DF50(a1);
  v8 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  sub_497B60(v10);
  sub_497AF0();
  v2 = sub_4039E0(v11, v8, v5, v1, v9, v4, v6, v7, &v11);
  switch ( v2 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7970, v9);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E8264, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v10);
      sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v11);
  return 0;
}

// ===== sub_486090 @ 0x00486090..0x004860C2 =====
int __cdecl sub_486090(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4020D0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4860D0 @ 0x004860D0..0x00486104 =====
int __cdecl sub_4860D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462CB0(v2, v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_486110 @ 0x00486110..0x0048621C =====
int __cdecl sub_486110(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-114h]
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+14h] [ebp-10Ch]
  int v8; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497C40(v8);
  sub_497DB0(v1);
  v3 = sub_462CF0(v1, v8, v6, v7, v5, v2);
  switch ( v3 )
  {
    case 1:
      sprintf(Buffer, &byte_4E799C, v2);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4EB5B8, v2);
      sub_4646F0(Buffer, (int)a1);
    case -1:
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  return 0;
}

// ===== sub_486220 @ 0x00486220..0x00486255 =====
int __cdecl sub_486220(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( sub_462CC0(v2, v1) == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_486260 @ 0x00486260..0x00486294 =====
int __cdecl sub_486260(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462F00(v2, v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_4862A0 @ 0x004862A0..0x004863AC =====
int __cdecl sub_4862A0(_DWORD *a1)
{
  int v1; // ebx
  void *v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-114h]
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+14h] [ebp-10Ch]
  int v8; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497C40(v8);
  sub_497DB0(v1);
  v3 = sub_462F10(v1, v8, v6, v7, v5, v2);
  switch ( v3 )
  {
    case 1:
      sprintf(Buffer, &byte_4E799C, v2);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4EB5B8, v2);
      sub_4646F0(Buffer, (int)a1);
    case -1:
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  return 0;
}

// ===== sub_4863B0 @ 0x004863B0..0x004863D7 =====
int __cdecl sub_4863B0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_4630F0(v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_4863E0 @ 0x004863E0..0x004864FA =====
int __cdecl sub_4863E0(_DWORD *a1)
{
  unsigned int v1; // esi
  unsigned int v2; // edi
  int v3; // eax
  unsigned int v5[5]; // [esp+Ch] [ebp-48h] BYREF
  int v6; // [esp+20h] [ebp-34h]
  int v7; // [esp+24h] [ebp-30h]
  int v8; // [esp+28h] [ebp-2Ch]
  int v9; // [esp+2Ch] [ebp-28h]
  int v10; // [esp+30h] [ebp-24h]
  int v11; // [esp+34h] [ebp-20h]
  int v12; // [esp+38h] [ebp-1Ch]
  int v13; // [esp+3Ch] [ebp-18h]
  int v14; // [esp+40h] [ebp-14h]
  int v15; // [esp+44h] [ebp-10h]
  unsigned int v16; // [esp+48h] [ebp-Ch]
  unsigned int v17; // [esp+4Ch] [ebp-8h]
  unsigned int v18; // [esp+50h] [ebp-4h]

  v15 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  sub_434E30(v16, v5, v17, v18, v2, v1);
  v3 = sub_491220(a1, v6, v7, v8, v9, v10, v11, 0, v5, v12, v13, v14, v15, 1);
  if ( v3 == -2147483647 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 2;
}

// ===== sub_486500 @ 0x00486500..0x004865D6 =====
int __cdecl sub_486500(_DWORD *a1)
{
  unsigned int v1; // esi
  unsigned int v2; // edi
  int v3; // eax
  unsigned int v5[5]; // [esp+Ch] [ebp-38h] BYREF
  int v6; // [esp+20h] [ebp-24h]
  int v7; // [esp+24h] [ebp-20h]
  const char *v8; // [esp+28h] [ebp-1Ch]
  int v9; // [esp+2Ch] [ebp-18h]
  int v10; // [esp+30h] [ebp-14h]
  int v11; // [esp+34h] [ebp-10h]
  unsigned int v12; // [esp+38h] [ebp-Ch]
  unsigned int v13; // [esp+3Ch] [ebp-8h]
  unsigned int v14; // [esp+40h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v8 = (const char *)sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  sub_434E30(v12, v5, v13, v14, v2, v1);
  v3 = sub_4630A0((int)v5, v6, v7, v8, v9, v10, v11);
  if ( v3 == 1 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_4865E0 @ 0x004865E0..0x0048664F =====
int __cdecl sub_4865E0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  _DWORD *v4; // edx
  int v5; // ebx
  _DWORD *v6; // edx
  _DWORD *v7; // edx
  _DWORD *v8; // edx
  _DWORD *v9; // edx
  int v10; // eax
  int v12; // [esp+Ch] [ebp-Ch]
  int v13; // [esp+10h] [ebp-8h]
  int v14; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v5 = sub_4450B0(v4);
  v14 = sub_4450B0(v6);
  v13 = sub_4450B0(v7);
  v12 = sub_4450B0(v8);
  v10 = sub_4450B0(v9);
  sub_4632C0(v1, v3, v10, v12, v13, v14, v5);
  return 0;
}

// ===== sub_486650 @ 0x00486650..0x0048675F =====
int __cdecl sub_486650(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_463190(v1, v2, v6, v8, v7, v3);
  switch ( v4 )
  {
    case -2147483646:
      sprintf(Buffer, &byte_4E9444, v8);
      sub_4646F0(Buffer, (int)a1);
    case -2147483642:
      sprintf(Buffer, &byte_4EB604, v6);
      sub_4646F0(Buffer, (int)a1);
    case -2147483641:
      sprintf(Buffer, &byte_4EB630, v2, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_486760 @ 0x00486760..0x004867CB =====
int __cdecl sub_486760(_DWORD *a1)
{
  int v1; // edi
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  if ( sub_463320(v1) == -2147483641 )
  {
    sprintf(Buffer, &byte_4EB664, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4867D0 @ 0x004867D0..0x00486A36 =====
int __cdecl sub_4867D0(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // esi
  int v3; // edi
  int v4; // ecx
  const char *v6; // [esp-8h] [ebp-178h]
  int v7; // [esp-4h] [ebp-174h]
  unsigned int v8[5]; // [esp+Ch] [ebp-164h] BYREF
  int v9; // [esp+20h] [ebp-150h]
  unsigned int v10; // [esp+24h] [ebp-14Ch]
  int v11; // [esp+28h] [ebp-148h]
  unsigned int v12; // [esp+2Ch] [ebp-144h]
  int v13; // [esp+30h] [ebp-140h]
  int v14; // [esp+34h] [ebp-13Ch]
  unsigned int v15; // [esp+38h] [ebp-138h]
  int v16; // [esp+3Ch] [ebp-134h]
  unsigned int v17; // [esp+40h] [ebp-130h]
  int v18; // [esp+44h] [ebp-12Ch]
  int v19; // [esp+48h] [ebp-128h]
  int v20; // [esp+4Ch] [ebp-124h]
  int v21; // [esp+50h] [ebp-120h]
  int v22; // [esp+54h] [ebp-11Ch]
  int v23; // [esp+58h] [ebp-118h]
  int v24; // [esp+5Ch] [ebp-114h]
  int v25; // [esp+60h] [ebp-110h]
  int v26; // [esp+64h] [ebp-10Ch]
  int v27; // [esp+68h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+6Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v24 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v23 = sub_4450B0(a1);
  v22 = sub_4450B0(a1);
  v26 = sub_4450B0(a1);
  v21 = sub_4450B0(a1);
  v20 = sub_48DF50(a1);
  v18 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v16 = sub_48DF50(a1);
  v14 = sub_4450B0(a1);
  v27 = sub_4450B0(a1);
  v25 = sub_4450B0(a1);
  sub_497B60(v25);
  sub_497AF0();
  sub_434E30(v15, v8, v10, v12, v17, v1);
  v2 = v23;
  v3 = v22;
  switch ( sub_403B10((int)&v27, v27, v14, v16, v18, v20, v26, v22, v23, v9, v24, v13, v19, v11, v21, v4) )
  {
    case -2147483647:
      v7 = v3;
      v6 = &byte_4E7970;
      goto LABEL_3;
    case -2147483646:
      sprintf(Buffer, &byte_4E823C, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      v7 = v26;
      v6 = &byte_4E8264;
LABEL_3:
      sprintf(Buffer, v6, v7);
      break;
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v25);
      break;
    default:
      sub_4450D0(a1, v27);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_486A50 @ 0x00486A50..0x00486B24 =====
int __cdecl sub_486A50(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // eax
  DWORD v4; // edx
  unsigned int v5; // eax
  int v7; // [esp+Ch] [ebp-8h]
  int v8; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v8 = -1;
  v5 = sub_463350(v4, v1, v3, v7, v2);
  if ( v5 > 0x80000005 )
  {
    if ( v5 == -2147483642 )
      v8 = 3;
  }
  else
  {
    switch ( v5 )
    {
      case 0x80000005:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000004:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v8);
  return 0;
}

// ===== sub_486B30 @ 0x00486B30..0x00486B55 =====
int __cdecl sub_486B30(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  v1 = (void *)sub_48DF50(a1);
  v2 = sub_463330(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_486B60 @ 0x00486B60..0x00486B74 =====
int __cdecl sub_486B60(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463370(v1);
  return 0;
}

// ===== sub_486B80 @ 0x00486B80..0x00486C31 =====
int __cdecl sub_486B80(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  if ( v2 <= 0 || v1 <= 0 )
  {
    sprintf(Buffer, &byte_4EA528, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = sub_48F270(&v6, v3);
  sub_4450D0(a1, v4 != 0 ? v6 : 0);
  return 0;
}

// ===== sub_486C40 @ 0x00486C40..0x00486D26 =====
int __cdecl sub_486C40(_DWORD *a1)
{
  const char *v1; // edi
  const char *v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  void *v7; // [esp+10h] [ebp-18h]
  int v8; // [esp+14h] [ebp-14h]
  int v9; // [esp+18h] [ebp-10h]

  v7 = (void *)sub_4450B0(a1);
  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v9 = sub_48DF50(a1);
  v8 = sub_48DF50(a1);
  if ( !v7 )
  {
    v5 = operator new(0x65Cu);
    if ( v5 )
    {
      v4 = sub_452100(v5, v8, v9, (int)v2);
      goto LABEL_7;
    }
LABEL_6:
    v4 = 0;
    goto LABEL_7;
  }
  v3 = operator new(0x80u);
  if ( !v3 )
    goto LABEL_6;
  v4 = sub_451DE0(a1, v2, v3, v8, v1);
LABEL_7:
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_486D30 @ 0x00486D30..0x00486E0C =====
int __cdecl sub_486D30(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_48F7B0(v3, v2, v1);
  if ( v4 > 0x80000002 )
  {
    if ( v4 == -2147483645 )
    {
      v4 = 3;
    }
    else if ( v4 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000002:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_486E10 @ 0x00486E10..0x00486E99 =====
int __cdecl sub_486E10(_DWORD *a1)
{
  int v1; // eax
  unsigned int v2; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_48F8E0(v1);
  if ( v2 > 0x80000004 )
  {
    if ( v2 == -2147483643 )
      v2 = 5;
  }
  else
  {
    switch ( v2 )
    {
      case 0x80000004:
        sub_4450D0(a1, 4);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_486EA0 @ 0x00486EA0..0x00486F10 =====
int __cdecl sub_486EA0(_DWORD *a1)
{
  int v1; // esi
  unsigned int v2; // eax

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_48F8F0(v1);
  if ( v2 )
  {
    if ( v2 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v2 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v2 = 0;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_486F10 @ 0x00486F10..0x00486F99 =====
int __cdecl sub_486F10(_DWORD *a1)
{
  int v1; // eax
  unsigned int v2; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_48F900(v1);
  if ( v2 > 0x80000003 )
  {
    if ( v2 == -2147483644 )
      v2 = 4;
  }
  else
  {
    switch ( v2 )
    {
      case 0x80000003:
        sub_4450D0(a1, 3);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_486FA0 @ 0x00486FA0..0x00487002 =====
int __cdecl sub_486FA0(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(int); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = funcs_486FEE[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB694, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2((int)a1);
}

// ===== sub_487010 @ 0x00487010..0x00487026 =====
int __cdecl sub_487010(_DWORD *a1)
{
  sub_4450D0(a1, 20);
  return 0;
}

// ===== sub_487030 @ 0x00487030..0x0048706A =====
int __cdecl sub_487030(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497A40(v2);
  sub_493B60(v2, v1);
  return 0;
}

// ===== sub_487070 @ 0x00487070..0x004870AA =====
int __cdecl sub_487070(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497950(v2);
  sub_493B90(v2, v1);
  return 0;
}

// ===== sub_4870B0 @ 0x004870B0..0x00487178 =====
int __cdecl sub_4870B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-10Ch]
  int v5; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  v4 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497A40(v4);
  v2 = sub_493C00(v1) - 12;
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB6C8, v5);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 == 2 )
  {
    sprintf(Buffer, &byte_4EB6F4, v5);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_487180 @ 0x00487180..0x0048727B =====
int __cdecl sub_487180(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-114h]
  int v5; // [esp+10h] [ebp-110h]
  int v6; // [esp+14h] [ebp-10Ch]
  int v7; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  sub_4979A0(v1);
  sub_4979F0(v6);
  sub_497A40(v7);
  v2 = sub_493DB0(v7, v5, v6, v1) - 12;
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB72C, v5, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 == 2 )
  {
    sprintf(Buffer, &byte_4E57B4, v5, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_487280 @ 0x00487280..0x004873B1 =====
int __cdecl sub_487280(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-11Ch]
  int v5; // [esp+10h] [ebp-118h]
  int v6; // [esp+14h] [ebp-114h]
  int v7; // [esp+18h] [ebp-110h]
  int v8; // [esp+1Ch] [ebp-10Ch]
  int v9; // [esp+20h] [ebp-108h]
  char Buffer[256]; // [esp+24h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v9 = sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  sub_4979A0(v1);
  sub_4979F0(v8);
  sub_497A40(v7);
  v2 = sub_4940D0(v7, v9, v4, v6, v8, v1) - 12;
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB75C, v9, v5, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 == 2 )
  {
    sprintf(Buffer, &byte_4EB798, v9, v5, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4873C0 @ 0x004873C0..0x004873F9 =====
int __cdecl sub_4873C0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497A40(v2);
  sub_4A3150(v2, v1 == 0);
  return 0;
}

// ===== sub_487400 @ 0x00487400..0x00487442 =====
int __cdecl sub_487400(_DWORD *a1)
{
  int v1; // eax
  int v3; // [esp+Ch] [ebp-4h]

  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  sub_497A40(v3);
  v1 = sub_4942B0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487450 @ 0x00487450..0x00487499 =====
int __cdecl sub_487450(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-4h]

  v4 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497A40(v2);
  sub_4A31E0(v2, v1, v4);
  return 0;
}

// ===== sub_4874A0 @ 0x004874A0..0x004874D8 =====
int __cdecl sub_4874A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979A0(v1);
  sub_497A40(v2);
  sub_4A3390(v2, v1);
  return 0;
}

// ===== sub_4874E0 @ 0x004874E0..0x00487512 =====
int __cdecl sub_4874E0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497A40(v2);
  sub_4A3030(v2, v1);
  return 0;
}

// ===== sub_487520 @ 0x00487520..0x00487552 =====
int __cdecl sub_487520(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497A40(v2);
  sub_4A2FA0(v2, v1);
  return 0;
}

// ===== sub_487560 @ 0x00487560..0x00487598 =====
int __cdecl sub_487560(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497A40(v2);
  sub_4A3300(v2, v1);
  return 0;
}

// ===== sub_4875A0 @ 0x004875A0..0x0048764D =====
int __cdecl sub_4875A0(_DWORD *a1)
{
  int v1; // edi
  void *v2; // eax
  int v3; // eax
  int v5; // [esp+24h] [ebp-10h]

  sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  sub_497950(v1);
  v2 = operator new(0x670u);
  if ( v2 )
    v3 = sub_439EF0((int)v2, v1, v5, 0, 1.0, 1.0);
  else
    v3 = 0;
  sub_4451C0((int)a1, v3);
  return 2;
}

// ===== sub_487650 @ 0x00487650..0x00487736 =====
int __cdecl sub_487650(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  void *v3; // eax
  int v4; // eax
  int v6; // [esp+40h] [ebp-54h]
  double v7; // [esp+4Ch] [ebp-48h]

  v7 = (double)sub_4450B0(a1) * 0.0000152587890625;
  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_497950(v2);
  v3 = operator new(0x670u);
  if ( v3 )
    v4 = sub_439EF0((int)v3, v2, v6, v1, v7, 1.0);
  else
    v4 = 0;
  sub_4451C0((int)a1, v4);
  return 2;
}

// ===== sub_487740 @ 0x00487740..0x00487767 =====
int __cdecl sub_487740(_DWORD *a1)
{
  int v1; // edi

  v1 = sub_4450B0(a1);
  sub_497950(v1);
  sub_494380();
  return 0;
}

// ===== sub_487770 @ 0x00487770..0x0048785A =====
int __cdecl sub_487770(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  void *v3; // eax
  int v4; // eax
  int v6; // [esp+40h] [ebp-54h]
  double v7; // [esp+4Ch] [ebp-48h]

  v7 = (double)sub_4450B0(a1) * 0.0000152587890625;
  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_497950(v2);
  v3 = operator new(0x670u);
  if ( v3 )
    v4 = sub_439EF0((int)v3, v2, v6, v1, v7, 2.0);
  else
    v4 = 0;
  sub_4451C0((int)a1, v4);
  return 2;
}

// ===== sub_487860 @ 0x00487860..0x004878E7 =====
int __cdecl sub_487860(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edx
  int v3; // eax
  int v5; // eax
  int v6; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4979A0(v2);
  sub_4979F0(v6);
  sub_497950(v1);
  v3 = sub_4943A0(v1);
  if ( !v3 || v3 == 20 )
  {
    v5 = sub_4943E0(v1);
    sub_4450D0(a1, v5);
    return 0;
  }
  else
  {
    sub_4450D0(a1, 0);
    return 0;
  }
}

// ===== sub_4878F0 @ 0x004878F0..0x00487917 =====
int __cdecl sub_4878F0(_DWORD *a1)
{
  int v1; // edi

  v1 = sub_4450B0(a1);
  sub_497950(v1);
  sub_4943C0();
  return 0;
}

// ===== sub_487920 @ 0x00487920..0x00487954 =====
int __cdecl sub_487920(_DWORD *a1)
{
  int v1; // edi

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_497950(v1);
  sub_4943D0(v1);
  return 0;
}

// ===== sub_487960 @ 0x00487960..0x00487A61 =====
int __cdecl sub_487960(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  void *v3; // eax
  int v4; // eax
  int v6; // [esp+38h] [ebp-5Ch]
  double v7; // [esp+44h] [ebp-50h]
  double v8; // [esp+4Ch] [ebp-48h]

  v7 = (double)sub_4450B0(a1) * 0.0000152587890625;
  v8 = 0.0000152587890625 * (double)sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_497950(v2);
  v3 = operator new(0x670u);
  if ( v3 )
    v4 = sub_439EF0((int)v3, v2, v6, v1, v8, v7);
  else
    v4 = 0;
  sub_4451C0((int)a1, v4);
  return 2;
}

// ===== sub_487A70 @ 0x00487A70..0x00487B5F =====
int __cdecl sub_487A70(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  _DWORD *v3; // eax
  int v4; // eax
  int v6; // [esp+3Ch] [ebp-58h]
  double v7; // [esp+44h] [ebp-50h]
  double v8; // [esp+4Ch] [ebp-48h]

  v7 = (double)sub_4450B0(a1) * 0.0000152587890625;
  v8 = 0.0000152587890625 * (double)sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_497950(v2);
  v3 = operator new(0x48u);
  if ( v3 )
    v4 = sub_452530(a1, v3, v2, v6, v1, v8, v7);
  else
    v4 = 0;
  sub_4451C0((int)a1, v4);
  return 2;
}

// ===== sub_487B60 @ 0x00487B60..0x00487B98 =====
int __cdecl sub_487B60(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4979F0(v1);
  sub_497950(v2);
  sub_4A2E80(v2, v1);
  return 0;
}

// ===== sub_487BA0 @ 0x00487BA0..0x00487BD4 =====
int __cdecl sub_487BA0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_497950(v1);
  v2 = sub_4943E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_487BE0 @ 0x00487BE0..0x00487BE8 =====
int sub_487BE0()
{
  sub_48D640();
  return 0;
}

// ===== sub_487BF0 @ 0x00487BF0..0x00487BF8 =====
int sub_487BF0()
{
  sub_48D7C0();
  return 0;
}

// ===== sub_487C00 @ 0x00487C00..0x00487C31 =====
int __cdecl sub_487C00(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_48D850(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_487C40 @ 0x00487C40..0x00487C48 =====
int sub_487C40()
{
  sub_48D8F0();
  return 0;
}

// ===== sub_487C50 @ 0x00487C50..0x00487C75 =====
int __cdecl sub_487C50(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  v1 = sub_48D910();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487C80 @ 0x00487C80..0x00487CA7 =====
int __cdecl sub_487C80(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_4944D0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_487CB0 @ 0x00487CB0..0x00487D12 =====
int __cdecl sub_487CB0(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(_DWORD *); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = (int (__cdecl *)(_DWORD *))*(&funcs_487CFE + v1);
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EB7DC, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2(a1);
}

// ===== sub_487D20 @ 0x00487D20..0x00487D38 =====
int __cdecl sub_487D20(_DWORD *a1)
{
  unsigned int v1; // eax

  v1 = sub_4450B0(a1);
  srand(v1);
  return 0;
}

// ===== sub_487D40 @ 0x00487D40..0x00487D58 =====
int __cdecl sub_487D40(_DWORD *a1)
{
  int v1; // eax

  v1 = rand();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487D60 @ 0x00487D60..0x00487DA7 =====
int __cdecl sub_487D60(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edx
  int v3; // edi
  int v4; // esi

  v1 = sub_4450B0(a1);
  v2 = 0;
  if ( v1 > 0 )
  {
    v3 = rand() << 8;
    v4 = (v3 ^ rand()) << 8;
    v2 = (v4 ^ rand()) % v1;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_487DB0 @ 0x00487DB0..0x00487DC8 =====
int __cdecl sub_487DB0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_498720();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487DD0 @ 0x00487DD0..0x00487DF7 =====
int __cdecl sub_487DD0(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  v1 = sub_4988C0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487E00 @ 0x00487E00..0x00487E14 =====
int __cdecl sub_487E00(_DWORD *a1)
{
  __int64 v1; // rax

  LODWORD(v1) = sub_4450B0(a1);
  sub_401600(v1);
  return 0;
}

// ===== sub_487E20 @ 0x00487E20..0x00487E47 =====
int __cdecl sub_487E20(_DWORD *a1)
{
  int v1; // edi
  int v2; // edx
  int *v3; // eax

  v1 = sub_4450B0(a1);
  v3 = (int *)sub_48DF50(v2);
  sub_401670(v1, v3);
  return 0;
}

// ===== sub_487E50 @ 0x00487E50..0x00487E83 =====
int __cdecl sub_487E50(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v3[2]; // [esp+8h] [ebp-8h] BYREF

  sub_48E680(v3);
  v1 = sub_4450D0(a1, v3[0]);
  sub_4450D0(v1, v3[1]);
  return 0;
}

// ===== sub_487E90 @ 0x00487E90..0x00487EA8 =====
int __cdecl sub_487E90(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_45FEF0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487EB0 @ 0x00487EB0..0x00487ED1 =====
int __cdecl sub_487EB0(int a1)
{
  qmemcpy((void *)sub_48DF50(a1), dword_518920, 0x40u);
  return 0;
}

// ===== sub_487EE0 @ 0x00487EE0..0x00487EF8 =====
int __cdecl sub_487EE0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_461EB0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487F00 @ 0x00487F00..0x00487F16 =====
int __cdecl sub_487F00(int a1)
{
  struct _SYSTEMTIME *v1; // eax

  v1 = (struct _SYSTEMTIME *)sub_48DF50(a1);
  GetLocalTime(v1);
  return 0;
}

// ===== sub_487F20 @ 0x00487F20..0x00487FAF =====
int __cdecl sub_487F20(_DWORD *a1)
{
  SIZE_T dwTotalPhys; // esi
  _DWORD *v2; // eax
  SIZE_T dwAvailPhys; // esi
  _MEMORYSTATUS Buffer; // [esp+Ch] [ebp-24h] BYREF

  memset(&Buffer.dwMemoryLoad, 0, 28);
  Buffer.dwLength = 32;
  GlobalMemoryStatus(&Buffer);
  dwTotalPhys = Buffer.dwTotalPhys;
  if ( Buffer.dwTotalPhys >= 0x80000000 )
    dwTotalPhys = 0x7FFFFFFF;
  v2 = sub_4450D0(a1, dwTotalPhys);
  dwAvailPhys = Buffer.dwAvailPhys;
  if ( Buffer.dwAvailPhys >= 0x80000000 )
    dwAvailPhys = 0x7FFFFFFF;
  sub_4450D0(v2, dwAvailPhys);
  return 0;
}

// ===== sub_487FB0 @ 0x00487FB0..0x00487FC8 =====
int __cdecl sub_487FB0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_49A240();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487FD0 @ 0x00487FD0..0x00487FE8 =====
int __cdecl sub_487FD0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_49A230();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_487FF0 @ 0x00487FF0..0x00488009 =====
int __cdecl sub_487FF0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46D970(v1);
  sub_46DA20();
  return 0;
}
