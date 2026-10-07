#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_47C090 @ 0x0047C090..0x0047C0B1 =====
int __cdecl sub_47C090(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_462330(v3, v1);
  return 0;
}

// ===== sub_47C0C0 @ 0x0047C0C0..0x0047C0D8 =====
int __cdecl sub_47C0C0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_462340();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47C0E0 @ 0x0047C0E0..0x0047C10A =====
int __cdecl sub_47C0E0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_462350();
  if ( !v1 )
    sub_4646F0(byte_4E9374, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47C110 @ 0x0047C110..0x0047C16C =====
int __cdecl sub_47C110(_DWORD *a1)
{
  int v1; // esi

  v1 = sub_4450B0(a1);
  sub_496300(v1);
  if ( sub_46C5E0(v1) )
    sub_4646F0(byte_4E93B0, (int)a1);
  if ( sub_462060(v1) )
    sub_4646F0(byte_4E93F0, (int)a1);
  if ( !sub_462360(v1) )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C170 @ 0x0047C170..0x0047C1E3 =====
int __cdecl sub_47C170(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // edx
  int v4; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_462550(v3, v1, v2);
  if ( v4 == 10 )
    sub_4646F0(byte_4E9420, (int)a1);
  if ( v4 == 255 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C1F0 @ 0x0047C1F0..0x0047C224 =====
int __cdecl sub_47C1F0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462540(v2, v1) )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C230 @ 0x0047C230..0x0047C2CD =====
int __cdecl sub_47C230(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462520(v1, v2) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E9444, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E9470, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 253 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C2D0 @ 0x0047C2D0..0x0047C3D6 =====
int __cdecl sub_47C2D0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-118h]
  int v6; // [esp+10h] [ebp-114h]
  int v7; // [esp+14h] [ebp-110h]
  int v8; // [esp+18h] [ebp-10Ch]
  int v9; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v9);
  sub_497C40(v8);
  sub_497B60(v2);
  v3 = sub_462370(v1, v9, v6, v5, v7, v2, v8) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8DB4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 254 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C3E0 @ 0x0047C3E0..0x0047C466 =====
int __cdecl sub_47C3E0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v1);
  v3 = sub_462510(v1, v2) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8DB4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 254 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C470 @ 0x0047C470..0x0047C5CE =====
int __cdecl sub_47C470(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-120h]
  int v7; // [esp+10h] [ebp-11Ch]
  int v8; // [esp+14h] [ebp-118h]
  int v9; // [esp+18h] [ebp-114h]
  int v10; // [esp+1Ch] [ebp-110h]
  int v11; // [esp+20h] [ebp-10Ch]
  int v12; // [esp+24h] [ebp-108h]
  char Buffer[256]; // [esp+28h] [ebp-104h] BYREF

  v8 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497B60(v1);
  sub_497E00(v12);
  sub_497E50(v10);
  sub_497BB0(v11);
  v3 = sub_462390(v8, v11, v7, v6, v9, v2, v1, v12, v10) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E94B8, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E94F0, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 253 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C5D0 @ 0x0047C5D0..0x0047C76F =====
int __cdecl sub_47C5D0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-130h]
  int v7; // [esp+10h] [ebp-12Ch]
  int v8; // [esp+14h] [ebp-128h]
  int v9; // [esp+18h] [ebp-124h]
  int v10; // [esp+1Ch] [ebp-120h]
  int v11; // [esp+20h] [ebp-11Ch]
  int v12; // [esp+24h] [ebp-118h]
  int v13; // [esp+28h] [ebp-114h]
  int v14; // [esp+2Ch] [ebp-110h]
  int v15; // [esp+30h] [ebp-10Ch]
  int v16; // [esp+34h] [ebp-108h]
  char Buffer[256]; // [esp+38h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  sub_497C40(v15);
  v3 = sub_4623C0(v8, v10, v12, v16, v13, v7, v14, v9, v6, v11, v15, v2, v1) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E9444, v16);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 7;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E9520, v9, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 247 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C770 @ 0x0047C770..0x0047C8EB =====
int __cdecl sub_47C770(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-124h]
  int v7; // [esp+10h] [ebp-120h]
  int v8; // [esp+14h] [ebp-11Ch]
  int v9; // [esp+18h] [ebp-118h]
  int v10; // [esp+1Ch] [ebp-114h]
  int v11; // [esp+20h] [ebp-110h]
  int v12; // [esp+24h] [ebp-10Ch]
  int v13; // [esp+28h] [ebp-108h]
  char Buffer[256]; // [esp+2Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  sub_497C40(v10);
  sub_497DB0(v12);
  v3 = sub_462400(v1, v2, v8, v7, v6, v11, v13, v9, v12, v10) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E94B8, v11, v13);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E954C, v13);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 253 )
    sub_4646F0(byte_4E7FAC, (int)a1);
  return 0;
}

// ===== sub_47C8F0 @ 0x0047C8F0..0x0047CAE1 =====
int __cdecl sub_47C8F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-124h]
  int v5; // [esp+10h] [ebp-120h]
  int v6; // [esp+14h] [ebp-11Ch]
  int v7; // [esp+18h] [ebp-118h]
  int v8; // [esp+1Ch] [ebp-114h]
  int v9; // [esp+20h] [ebp-110h]
  int v10; // [esp+24h] [ebp-10Ch]
  int v11; // [esp+28h] [ebp-108h]
  char Buffer[256]; // [esp+2Ch] [ebp-104h] BYREF

  v8 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497B60(v1);
  sub_497E50(v10);
  sub_497BB0(v8);
  switch ( sub_462430(v8, v10, v6, v5, v4, v2, v1, v9, v11, v7) )
  {
    case 1:
      sprintf(Buffer, &byte_4E9444, v2);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E9588, v2);
      break;
    case 3:
      sprintf(Buffer, &byte_4E95C0, v1);
      goto LABEL_7;
    case 4:
      sprintf(Buffer, &byte_4E9610, v1, v2);
      sub_4646F0(Buffer, (int)a1);
    case 5:
      sprintf(Buffer, &byte_4E9690, v9);
LABEL_7:
      sub_4646F0(Buffer, (int)a1);
    case 6:
      sprintf(Buffer, byte_4E91E4, v11);
      break;
    case 7:
      sprintf(Buffer, &byte_4E9210, v11, v1);
      break;
    case 255:
      sub_4646F0(byte_4E7FAC, (int)a1);
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47CC10 @ 0x0047CC10..0x0047CE44 =====
int __cdecl sub_47CC10(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v4; // [esp+Ch] [ebp-140h]
  int v5; // [esp+10h] [ebp-13Ch]
  int v6; // [esp+14h] [ebp-138h]
  int v7; // [esp+18h] [ebp-134h]
  int v8; // [esp+1Ch] [ebp-130h]
  int v9; // [esp+20h] [ebp-12Ch]
  int v10; // [esp+24h] [ebp-128h]
  int v11; // [esp+28h] [ebp-124h]
  int v12; // [esp+2Ch] [ebp-120h]
  int v13; // [esp+30h] [ebp-11Ch]
  int v14; // [esp+34h] [ebp-118h]
  int v15; // [esp+38h] [ebp-114h]
  int v16; // [esp+3Ch] [ebp-110h]
  int v17; // [esp+40h] [ebp-10Ch]
  int v18; // [esp+44h] [ebp-108h]
  char Buffer[256]; // [esp+48h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497E00(v18);
  sub_497C40(v16);
  sub_497DB0(v15);
  sub_497BB0(v1);
  switch ( sub_462460(v6, v8, v10, v12, v2, v13, v18, v9, v7, v14, v5, v17, v4, v11, v16, v15, v1) )
  {
    case 1:
      sprintf(Buffer, &byte_4E94B8, v2, v13);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E96C8, v2, v13);
      sub_4646F0(Buffer, (int)a1);
    case 8:
      sprintf(Buffer, &byte_4E9714, v17);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4E7FAC, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_47CF60 @ 0x0047CF60..0x0047D1D0 =====
int __cdecl sub_47CF60(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v4; // [esp+Ch] [ebp-14Ch]
  int v5; // [esp+10h] [ebp-148h]
  int v6; // [esp+14h] [ebp-144h]
  int v7; // [esp+18h] [ebp-140h]
  int v8; // [esp+1Ch] [ebp-13Ch]
  int v9; // [esp+20h] [ebp-138h]
  int v10; // [esp+24h] [ebp-134h]
  int v11; // [esp+28h] [ebp-130h]
  int v12; // [esp+2Ch] [ebp-12Ch]
  int v13; // [esp+30h] [ebp-128h]
  int v14; // [esp+34h] [ebp-124h]
  int v15; // [esp+38h] [ebp-120h]
  int v16; // [esp+3Ch] [ebp-11Ch]
  int v17; // [esp+40h] [ebp-118h]
  int v18; // [esp+44h] [ebp-114h]
  int v19; // [esp+48h] [ebp-110h]
  int v20; // [esp+4Ch] [ebp-10Ch]
  int v21; // [esp+50h] [ebp-108h]
  char Buffer[256]; // [esp+54h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v20 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v21 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497E00(v21);
  sub_497C40(v19);
  sub_497DB0(v18);
  sub_497BB0(v1);
  switch ( sub_4624B0(v6, v8, v10, v12, v2, v14, v21, v16, v7, v11, v17, v13, v9, v15, v20, v4, v5, v19, v18, v1) )
  {
    case 1:
      sprintf(Buffer, &byte_4E94B8, v2, v14);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E96C8, v2, v14);
      sub_4646F0(Buffer, (int)a1);
    case 8:
      sprintf(Buffer, &byte_4E9714, v20);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4E7FAC, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_47D2F0 @ 0x0047D2F0..0x0047D31A =====
int __cdecl sub_47D2F0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_462560();
  if ( !v1 )
    sub_4646F0(byte_4E9740, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47D320 @ 0x0047D320..0x0047D347 =====
int __cdecl sub_47D320(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_462570(v1) )
    sub_4646F0(byte_4E977C, (int)a1);
  return 0;
}

// ===== sub_47D350 @ 0x0047D350..0x0047D384 =====
int __cdecl sub_47D350(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_4625A0(v2, v1) )
    sub_4646F0(byte_4E977C, (int)a1);
  return 0;
}

// ===== sub_47D390 @ 0x0047D390..0x0047D400 =====
int __cdecl sub_47D390(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-8h]
  int v5; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  if ( sub_462580(v1, v2, v4, 0, v5, -1, 0) )
    sub_4646F0(byte_4E977C, (int)a1);
  return 0;
}

// ===== sub_47D400 @ 0x0047D400..0x0047D554 =====
int __cdecl sub_47D400(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-118h]
  int v5; // [esp+10h] [ebp-114h]
  int v6; // [esp+14h] [ebp-110h]
  int v7; // [esp+18h] [ebp-10Ch]
  int v8; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  switch ( sub_462580(v1, v2, v6, v5, v7, v8, v4) )
  {
    case 1:
      sprintf(Buffer, &byte_4E97A8, v8);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4E8648, v8);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sprintf(Buffer, &byte_4E97D8, v8);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4E977C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_47D670 @ 0x0047D670..0x0047D69A =====
int __cdecl sub_47D670(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_462680();
  if ( !v1 )
    sub_4646F0(byte_4E9818, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47D6A0 @ 0x0047D6A0..0x0047D6C7 =====
int __cdecl sub_47D6A0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_462690(v1) )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47D6D0 @ 0x0047D6D0..0x0047D704 =====
int __cdecl sub_47D6D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_4626A0(v2, v1) )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47D710 @ 0x0047D710..0x0047D816 =====
int __cdecl sub_47D710(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-118h]
  int v6; // [esp+10h] [ebp-114h]
  int v7; // [esp+14h] [ebp-110h]
  int v8; // [esp+18h] [ebp-10Ch]
  int v9; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v9);
  sub_497C40(v8);
  sub_497B60(v2);
  v3 = sub_4626B0(v1, v9, v6, v5, v7, v2, v8) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E9878, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 254 )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47D820 @ 0x0047D820..0x0047D8FE =====
int __cdecl sub_47D820(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  unsigned int v4; // edx
  int v5; // eax
  int v6; // eax
  int v8; // [esp+Ch] [ebp-10Ch]
  unsigned int v9; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v8 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4626D0(v4, v9, v3, v2, v1) - 2;
  if ( !v5 )
  {
    sprintf(Buffer, &byte_4E98BC, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v6 = v5 - 1;
  if ( !v6 )
  {
    sprintf(Buffer, &byte_4E98EC, v9, v8);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v6 == 252 )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47D900 @ 0x0047D900..0x0047D995 =====
int __cdecl sub_47D900(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  void *v4; // edx
  int v5; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4626F0(v1, v4, v3, v2);
  if ( v5 == 4 )
  {
    sprintf(Buffer, &byte_4E991C, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 255 )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47D9A0 @ 0x0047D9A0..0x0047DA6B =====
int __cdecl sub_47D9A0(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  unsigned int v7; // [esp+Ch] [ebp-10Ch]
  unsigned int v8; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_462710(v4, v1, v3, v8, v7, v2);
  if ( v5 == 5 )
  {
    sprintf(Buffer, &byte_4E9950, v8, v7, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 255 )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47DA70 @ 0x0047DA70..0x0047DAB7 =====
int __cdecl sub_47DA70(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462730(v2, v1);
  if ( v3 == 6 )
    sub_4646F0(byte_4E99A8, (int)a1);
  if ( v3 == 255 )
    sub_4646F0(byte_4E9850, (int)a1);
  return 0;
}

// ===== sub_47DAC0 @ 0x0047DAC0..0x0047DB51 =====
int __cdecl sub_47DAC0(_DWORD *a1)
{
  unsigned int v1; // esi
  unsigned int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462940(v1, v2, &v5) - 1;
  if ( !v3 )
    sub_4646F0(byte_4E99DC, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E9A18, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_47DB60 @ 0x0047DB60..0x0047DBB5 =====
int __cdecl sub_47DB60(_DWORD *a1)
{
  int v1; // esi

  v1 = sub_4450B0(a1);
  if ( sub_46C5E0(v1) )
    sub_4646F0(byte_4E9A48, (int)a1);
  if ( sub_462060(v1) )
    sub_4646F0(byte_4E9A88, (int)a1);
  if ( !sub_462980(v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DBC0 @ 0x0047DBC0..0x0047DC04 =====
int __cdecl sub_47DBC0(_DWORD *a1)
{
  int v1; // ebx
  void *v2; // edi

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  sub_497B60(v2);
  if ( sub_462E80(v1, v2) == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DC10 @ 0x0047DC10..0x0047DC89 =====
int __cdecl sub_47DC10(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_462EC0(v1, v2);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E9AE4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DC90 @ 0x0047DC90..0x0047DCC4 =====
int __cdecl sub_47DC90(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462CA0(v2, v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DCD0 @ 0x0047DCD0..0x0047DD72 =====
int __cdecl sub_47DCD0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-14h]
  int v5; // [esp+10h] [ebp-10h]
  int v6; // [esp+14h] [ebp-Ch]
  int v7; // [esp+18h] [ebp-8h]
  int v8; // [esp+1Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v7);
  sub_497DB0(v2);
  sub_497C40(v8);
  if ( !sub_462AF0(v1, v2, v4, v5, v6, v8) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DD80 @ 0x0047DD80..0x0047DE4D =====
int __cdecl sub_47DD80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // eax
  unsigned int v5; // eax
  int v7; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_462990(v1, v3);
  if ( v4 > 3 )
  {
    if ( v4 == -1 )
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  else
  {
    if ( v4 == 3 )
    {
      sprintf(Buffer, &byte_4E9BB4, v2);
      sub_4646F0(Buffer, (int)a1);
    }
    v5 = v4 - 1;
    if ( !v5 )
      sub_4646F0(byte_4E9B0C, (int)a1);
    if ( v5 == 1 )
    {
      sprintf(Buffer, &byte_4E9B48, v7, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    }
  }
  return 0;
}

// ===== sub_47DE50 @ 0x0047DE50..0x0047DE84 =====
int __cdecl sub_47DE50(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462B10(v2, v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DE90 @ 0x0047DE90..0x0047DF4B =====
int __cdecl sub_47DE90(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // edx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-10Ch]
  unsigned int v8; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_462B20(v1, v2, v3, v8, v4);
  if ( v5 == 1 )
  {
    sprintf(Buffer, &byte_4E9BF8, v8, v7, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47DF50 @ 0x0047DF50..0x0047DF97 =====
int __cdecl sub_47DF50(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_462B60(v1, v2);
  if ( v3 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  sub_4450D0(a1, v3 == 0);
  return 0;
}

// ===== sub_47DFA0 @ 0x0047DFA0..0x0047E031 =====
int __cdecl sub_47DFA0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4911E0(a1, v3, v4, 0, v7, v2, v1, 0, 0, -1);
  if ( v5 == -2147483647 )
    sub_4646F0(byte_4E9C5C, (int)a1);
  if ( v5 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 2;
}

// ===== sub_47E040 @ 0x0047E040..0x0047E0D1 =====
int __cdecl sub_47E040(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // eax
  int v3; // eax
  int v4; // edx
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_463250(v2, v1);
  if ( v3 == -2147483644 )
  {
    sprintf(Buffer, &byte_4E9C94, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483643 )
  {
    sprintf(Buffer, &byte_4E9CBC, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47E0E0 @ 0x0047E0E0..0x0047E0F4 =====
int __cdecl sub_47E0E0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463260(v1);
  return 0;
}

// ===== sub_47E100 @ 0x0047E100..0x0047E114 =====
int __cdecl sub_47E100(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463150(v1);
  return 0;
}

// ===== sub_47E120 @ 0x0047E120..0x0047E154 =====
int __cdecl sub_47E120(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497A90(v2);
  sub_463160(v2, v1);
  return 0;
}

// ===== sub_47E160 @ 0x0047E160..0x0047E194 =====
int __cdecl sub_47E160(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497A90(v2);
  sub_463170(v2, v1);
  return 0;
}

// ===== sub_47E1A0 @ 0x0047E1A0..0x0047E1C3 =====
int __cdecl sub_47E1A0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_463180(v3, v1);
  return 0;
}

// ===== sub_47E1D0 @ 0x0047E1D0..0x0047E244 =====
int __cdecl sub_47E1D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v4; // [esp+4h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_4631C0(&v4, v2, v1) )
  {
    sprintf(Buffer, &byte_4E9CF0, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47E250 @ 0x0047E250..0x0047E264 =====
int __cdecl sub_47E250(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_4631E0(v1);
  return 0;
}

// ===== sub_47E270 @ 0x0047E270..0x0047E2A4 =====
int __cdecl sub_47E270(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  _DWORD *v4; // edx
  int v5; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v5 = sub_4450B0(v4);
  sub_4631F0(v1, v3, v5);
  return 0;
}

// ===== sub_47E2B0 @ 0x0047E2B0..0x0047E2D3 =====
int __cdecl sub_47E2B0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_463210(v3, v1);
  return 0;
}

// ===== sub_47E2E0 @ 0x0047E2E0..0x0047E2F4 =====
int __cdecl sub_47E2E0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463220(v1);
  return 0;
}

// ===== sub_47E300 @ 0x0047E300..0x0047E39D =====
int __cdecl sub_47E300(_DWORD *a1)
{
  unsigned int v1; // esi
  signed int v2; // eax
  signed int v3; // edx
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( v2 < 0 || v3 < 0 )
  {
    sprintf(Buffer, &byte_4E9D40, v2, v3);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v1 > 0x100 )
  {
    sprintf(Buffer, &byte_4E9D68, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_463230(v3, v2, v1);
  return 0;
}

// ===== sub_47E3A0 @ 0x0047E3A0..0x0047E45F =====
int __cdecl sub_47E3A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4631B0(v1, v2);
  switch ( v3 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E9D8C, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E9444, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E9DB4, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47E460 @ 0x0047E460..0x0047E474 =====
int __cdecl sub_47E460(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463240(v1);
  return 0;
}

// ===== sub_47E480 @ 0x0047E480..0x0047E5D2 =====
int __cdecl sub_47E480(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-114h]
  int v8; // [esp+10h] [ebp-110h]
  int v9; // [esp+14h] [ebp-10Ch]
  int v10; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_48DF50(a1);
  v10 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_491470(v3, v10, v4, v8, v9, v7, v2, v1);
  if ( v5 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  switch ( v5 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E9DE4, v10);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E9E0C, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E9E3C, v8);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sub_4646F0(byte_4E9C5C, (int)a1);
    default:
      return 2;
  }
}

// ===== sub_47E5F0 @ 0x0047E5F0..0x0047E6D3 =====
int __cdecl sub_47E5F0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-10Ch]
  int v7; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v4 = sub_491590(v3, a1, v1, v7, v6);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E9DE4, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E9E3C, v1);
      sub_4646F0(Buffer, (int)a1);
    case -1:
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  return 0;
}

// ===== sub_47E6E0 @ 0x0047E6E0..0x0047E8EA =====
int __usercall sub_47E6E0@<eax>(_DWORD *a1@<esi>, int a2)
{
  int v2; // edi
  int v3; // ebx
  int v4; // eax
  int v5; // edx
  int v6; // eax
  const char *v8; // [esp-8h] [ebp-13Ch]
  int v9; // [esp-4h] [ebp-138h]
  int v10; // [esp+8h] [ebp-12Ch]
  int v11; // [esp+Ch] [ebp-128h]
  int v12; // [esp+10h] [ebp-124h]
  int v13; // [esp+14h] [ebp-120h]
  int v14; // [esp+18h] [ebp-11Ch]
  int v15; // [esp+1Ch] [ebp-118h]
  int v16; // [esp+20h] [ebp-114h]
  int v17; // [esp+24h] [ebp-110h]
  int v18; // [esp+28h] [ebp-10Ch]
  int v19; // [esp+2Ch] [ebp-108h]
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  sub_48DF50(a1);
  v17 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v6 = sub_491670(a1, v4, v17, v5, v18, v12, v14, v15, v3, v13, v16, v2, v10, v11, a2);
  if ( v6 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  switch ( v6 )
  {
    case -2147483647:
      v9 = v17;
      v8 = &byte_4E9DE4;
      goto LABEL_4;
    case -2147483646:
      sprintf(Buffer, &byte_4E9E0C, v19);
      goto LABEL_7;
    case -2147483645:
      sprintf(Buffer, &byte_4E9E3C, v18);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sub_4646F0(byte_4E9C5C, (int)a1);
    case -2147483643:
      sprintf(Buffer, &byte_4E8DB4, v3);
LABEL_7:
      sub_4646F0(Buffer, (int)a1);
    case -2147483642:
      v9 = v3;
      v8 = byte_4E9E64;
LABEL_4:
      sprintf(Buffer, v8, v9);
      break;
    case -2147483641:
      sprintf(Buffer, &byte_4E8DB4, v2);
      break;
    case -2147483640:
      sprintf(Buffer, byte_4E9E64, v2);
      break;
    default:
      return 2;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_47E910 @ 0x0047E910..0x0047E924 =====
int __cdecl sub_47E910(_DWORD *a1)
{
  return sub_47E6E0(a1, 1);
}

// ===== sub_47E930 @ 0x0047E930..0x0047E944 =====
int __cdecl sub_47E930(_DWORD *a1)
{
  return sub_47E6E0(a1, 0);
}

// ===== sub_47E950 @ 0x0047E950..0x0047E973 =====
int __cdecl sub_47E950(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4633A0(v3, v1);
  return 0;
}

// ===== sub_47E980 @ 0x0047E980..0x0047E994 =====
int __cdecl sub_47E980(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_4633B0(v1);
  return 0;
}

// ===== sub_47E9A0 @ 0x0047E9A0..0x0047E9DC =====
int __cdecl sub_47E9A0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edi
  _DWORD *v4; // edx
  _DWORD *v5; // edx
  int v6; // eax
  int v8; // [esp-4h] [ebp-Ch]

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v8 = sub_4450B0(v4);
  v6 = sub_4450B0(v5);
  sub_4633C0(v1, v3, v6, v8);
  return 0;
}

// ===== sub_47E9E0 @ 0x0047E9E0..0x0047EA14 =====
int __cdecl sub_47E9E0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_4633E0(v2, v1) )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47EA20 @ 0x0047EA20..0x0047EA34 =====
int __cdecl sub_47EA20(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_4633F0(v1);
  return 0;
}

// ===== sub_47EA40 @ 0x0047EA40..0x0047EB4F =====
int __cdecl sub_47EA40(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v4; // edx
  unsigned int v5; // eax
  int v7; // [esp+Ch] [ebp-110h]
  int v8; // [esp+10h] [ebp-10Ch]
  int v9; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v9 = sub_48DF50(a1);
  sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_491870(a1, v3, v2, v4, v9, v7, v1);
  if ( v5 > 0x80000003 )
  {
    if ( v5 == -1 )
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  else
  {
    switch ( v5 )
    {
      case 0x80000003:
        sub_4646F0(byte_4E9F00, (int)a1);
      case 0x80000001:
        sprintf(Buffer, &byte_4E9EA4, v2);
        sub_4646F0(Buffer, (int)a1);
      case 0x80000002:
        sprintf(Buffer, &byte_4E9ED0, v8);
        sub_4646F0(Buffer, (int)a1);
    }
  }
  return 2;
}

// ===== sub_47EB50 @ 0x0047EB50..0x0047EC5F =====
int __cdecl sub_47EB50(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v4; // edx
  unsigned int v5; // eax
  int v7; // [esp+Ch] [ebp-110h]
  int v8; // [esp+10h] [ebp-10Ch]
  int v9; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v9 = sub_48DF50(a1);
  sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_4919A0(a1, v3, v2, v4, v9, v7, v1);
  if ( v5 > 0x80000003 )
  {
    if ( v5 == -1 )
      sub_4646F0(byte_4E9AB8, (int)a1);
  }
  else
  {
    switch ( v5 )
    {
      case 0x80000003:
        sub_4646F0(byte_4E9F00, (int)a1);
      case 0x80000001:
        sprintf(Buffer, &byte_4E9EA4, v2);
        sub_4646F0(Buffer, (int)a1);
      case 0x80000002:
        sprintf(Buffer, &byte_4E9ED0, v8);
        sub_4646F0(Buffer, (int)a1);
    }
  }
  return 2;
}

// ===== sub_47EC60 @ 0x0047EC60..0x0047ECE5 =====
int __cdecl sub_47EC60(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx
  int v4; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_491AD0(v2, v3);
  if ( v4 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E9EA4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47ECF0 @ 0x0047ECF0..0x0047EE14 =====
int __cdecl sub_47ECF0(_DWORD *a1)
{
  int v1; // esi
  int v2; // edi
  _DWORD *v3; // eax
  void *v4; // ebx
  _DWORD *v5; // eax
  _DWORD *v6; // ecx
  int v7; // edx
  int v8; // esi
  int v10; // [esp+Ch] [ebp-10Ch]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  if ( (unsigned int)(v2 - 1) > 0x3F )
  {
    sprintf(Buffer, &byte_4E9EA4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v3 = operator new[](16 * v2);
  v4 = v3;
  if ( v2 > 0 )
  {
    v5 = v3 + 2;
    v6 = (_DWORD *)(v1 + 8);
    v7 = v2;
    do
    {
      *(v5 - 2) = *(v6 - 2);
      *(v5 - 1) = *(v6 - 1);
      *v5 = *v6;
      v5[1] = -1;
      v6 += 16;
      v5 += 4;
      --v7;
    }
    while ( v7 );
  }
  v8 = sub_491AD0(v10, v4);
  operator delete[](v4);
  if ( v8 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E9EA4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v8 == -1 )
    sub_4646F0(byte_4E9AB8, (int)a1);
  return 0;
}

// ===== sub_47EE20 @ 0x0047EE20..0x0047EE52 =====
int __cdecl sub_47EE20(_DWORD *a1)
{
  int v1; // eax
  int *v2; // edx
  int v3; // eax

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_46CD10(v1, v2, (int)a1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_47EE60 @ 0x0047EE60..0x0047EE92 =====
int __cdecl sub_47EE60(_DWORD *a1)
{
  int v1; // eax
  int *v2; // edx
  int v3; // eax

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_46CDB0(v1, v2, (int)a1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_47EEA0 @ 0x0047EEA0..0x0047EEC9 =====
int __cdecl sub_47EEA0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46C630(v1, 0);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_47EED0 @ 0x0047EED0..0x0047EEF5 =====
int __cdecl sub_47EED0(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_46C6F0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_47EF00 @ 0x0047EF00..0x0047EF33 =====
int __cdecl sub_47EF00(_DWORD *a1)
{
  int *v1; // esi
  int v2; // eax
  int v3; // ecx
  int v4; // eax

  v1 = (int *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_46C8E0(v3, v2, v1, (int)a1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47EF40 @ 0x0047EF40..0x0047EF72 =====
int __cdecl sub_47EF40(_DWORD *a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  int v3; // ecx
  BOOL v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_46CC00(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47EF80 @ 0x0047EF80..0x0047EFB2 =====
int __cdecl sub_47EF80(_DWORD *a1)
{
  int v1; // esi
  int *v2; // eax
  int v3; // ecx
  BOOL v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (int *)sub_48DF50(a1);
  v4 = sub_46CC20(v3, v1, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47EFC0 @ 0x0047EFC0..0x0047EFF2 =====
int __cdecl sub_47EFC0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v3; // ecx
  BOOL v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v4 = sub_46CC50(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47F000 @ 0x0047F000..0x0047F032 =====
int __cdecl sub_47F000(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  int v3; // ecx
  BOOL v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_46CC70(v3, v1, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47F040 @ 0x0047F040..0x0047F0FC =====
int __cdecl sub_47F040(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  v3 = sub_401EF0(v2, v1);
  if ( v3 == -2147483635 )
  {
    sprintf(Buffer, &byte_4E9F38, v5, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -1 )
  {
    sprintf(Buffer, &byte_4E6E70, v5, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F100 @ 0x0047F100..0x0047F1E7 =====
int __cdecl sub_47F100(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497B60(v1);
  v3 = sub_405590(v1, v2, v5);
  switch ( v3 )
  {
    case -2147483639:
      sprintf(Buffer, &byte_4E9F70, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483638:
      sprintf(Buffer, &byte_4E9FA4, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483626:
      sprintf(Buffer, &byte_4E9FD8, v5);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F1F0 @ 0x0047F1F0..0x0047F2BC =====
int __cdecl sub_47F1F0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  sub_497B60(v1);
  v3 = sub_405090(v1, v2);
  switch ( v3 )
  {
    case -2147483642:
      sprintf(Buffer, &byte_4E9FFC, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483639:
      sprintf(Buffer, &byte_4E9F70, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483638:
      sprintf(Buffer, &byte_4E9FA4, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F2C0 @ 0x0047F2C0..0x0047F352 =====
int __cdecl sub_47F2C0(_DWORD *a1)
{
  void *v1; // ebx
  int v2; // esi
  int v3; // eax
  int v5; // [esp+10h] [ebp-4h]

  v5 = sub_4450B0(a1);
  sub_48DF50(a1);
  v1 = (void *)sub_4450B0(a1);
  sub_497B60(v1);
  v2 = -1;
  v3 = sub_46A300(v1, v5);
  if ( v3 )
  {
    if ( v3 == -2147483647 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v3 == -2147483646 )
    {
      sub_4450D0(a1, 2);
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

// ===== sub_47F360 @ 0x0047F360..0x0047F420 =====
int __cdecl sub_47F360(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  unsigned int v3; // eax
  const CHAR *v5; // [esp+Ch] [ebp-Ch]
  int v6; // [esp+10h] [ebp-8h]
  int v7; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = (const CHAR *)sub_48DF50(a1);
  sub_497B60(v1);
  v2 = -1;
  v3 = sub_46A600(v5, v6, v7);
  if ( v3 > 0x80000004 )
  {
    if ( v3 == -2147483643 )
      v2 = 5;
  }
  else
  {
    switch ( v3 )
    {
      case 0x80000004:
        sub_4450D0(a1, 4);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000003:
        sub_4450D0(a1, 3);
        return 0;
    }
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_47F420 @ 0x0047F420..0x0047F476 =====
int __cdecl sub_47F420(_DWORD *a1)
{
  size_t v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_401CB0(v6, v2, v3, v6, v1);
  sub_4450D0(a1, v4 == 0);
  return 0;
}

// ===== sub_47F480 @ 0x0047F480..0x0047F4E8 =====
int __cdecl sub_47F480(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]
  int v6; // [esp+14h] [ebp-4h]

  v6 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  sub_497B60(v1);
  v2 = sub_401CE0(v4, v1, v5, v6);
  sub_4450D0(a1, v2 == 0);
  return 0;
}

// ===== sub_47F4F0 @ 0x0047F4F0..0x0047F6BF =====
int __cdecl sub_47F4F0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-140h]
  int v7; // [esp+10h] [ebp-13Ch]
  int v8; // [esp+14h] [ebp-138h]
  int v9; // [esp+18h] [ebp-134h]
  int v10; // [esp+1Ch] [ebp-130h]
  int v11; // [esp+20h] [ebp-12Ch]
  int v12; // [esp+24h] [ebp-128h]
  int v13; // [esp+28h] [ebp-124h]
  int v14; // [esp+2Ch] [ebp-120h]
  int v15; // [esp+30h] [ebp-11Ch]
  int v16; // [esp+34h] [ebp-118h]
  int v17; // [esp+38h] [ebp-114h]
  int v18; // [esp+3Ch] [ebp-110h]
  int v19; // [esp+40h] [ebp-10Ch]
  int v20; // [esp+44h] [ebp-108h]
  char Buffer[256]; // [esp+48h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v20 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_404FE0(v8, v10, v1, v12, v14, v16, v18, v19, v20, v7, v15, v11, v17, v9, v6, v13, v3);
  if ( v4 == -2147483639 )
  {
    sprintf(Buffer, &byte_4EA034, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -2147483638 )
  {
    sprintf(Buffer, &byte_4E9FA4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F6C0 @ 0x0047F6C0..0x0047F759 =====
int __cdecl sub_47F6C0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_405120(v1);
  if ( v3 == -2147483639 )
  {
    sprintf(Buffer, &byte_4E9F70, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483638 )
  {
    sprintf(Buffer, &byte_4E9FA4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F760 @ 0x0047F760..0x0047F7F3 =====
int __cdecl sub_47F760(_DWORD *a1)
{
  int v1; // edi
  int v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_405420(v1, v2);
  if ( v3 == -2147483633 )
  {
    sprintf(Buffer, &byte_4EA068, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483632 )
  {
    sprintf(Buffer, &byte_4EA0A8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_47F800 @ 0x0047F800..0x0047F95F =====
int __cdecl sub_47F800(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  const char *v5; // [esp-8h] [ebp-130h]
  int v6; // [esp-4h] [ebp-12Ch]
  int v7; // [esp+Ch] [ebp-11Ch]
  int v8; // [esp+10h] [ebp-118h]
  int v9; // [esp+14h] [ebp-114h]
  int v10; // [esp+18h] [ebp-110h]
  int v11; // [esp+1Ch] [ebp-10Ch]
  int v12; // [esp+20h] [ebp-108h]
  char Buffer[256]; // [esp+24h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  switch ( sub_405450(v8, v10, v12, v11, v7, v3, v1) )
  {
    case -2147483639:
      v6 = v8;
      v5 = &byte_4E9F70;
      goto LABEL_3;
    case -2147483638:
      sprintf(Buffer, &byte_4E9FA4, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483633:
      v6 = v11;
      v5 = (const char *)&unk_4EA104;
      goto LABEL_3;
    case -2147483631:
      sprintf(Buffer, &byte_4EA140, v2);
      goto LABEL_4;
    case -2147483629:
      v6 = v9;
      v5 = (const char *)&unk_4EA188;
      goto LABEL_3;
    case -2147483628:
      sprintf(Buffer, byte_4EA1C4, v1);
      goto LABEL_4;
    case -2147483627:
      v6 = v12;
      v5 = (const char *)&unk_4EA200;
LABEL_3:
      sprintf(Buffer, v5, v6);
LABEL_4:
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_47F9A0 @ 0x0047F9A0..0x0047FAF4 =====
int __cdecl sub_47F9A0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  void *v3; // eax
  unsigned int v4; // eax
  const char *v6; // [esp-8h] [ebp-120h]
  int v7; // [esp-4h] [ebp-11Ch]
  unsigned int v8; // [esp+Ch] [ebp-10Ch]
  size_t *v9; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v9 = (size_t *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_4056F0(v3, v9, v2, v1, v8);
  if ( v4 <= 0x80000017 )
  {
    if ( v4 != -2147483625 )
    {
      switch ( v4 )
      {
        case 0x80000006:
          sprintf(Buffer, &byte_4EA23C, v2);
          sub_4646F0(Buffer, (int)a1);
        case 0x80000008:
          sub_4646F0(byte_4EA2C4, (int)a1);
        case 0x8000000A:
          v7 = v2;
          v6 = &byte_4E8DB4;
          goto LABEL_5;
        case 0x80000011:
          sprintf(Buffer, &byte_4EA140, v2);
          sub_4646F0(Buffer, (int)a1);
        default:
          return 0;
      }
    }
    v7 = v1;
    v6 = (const char *)&unk_4EA274;
LABEL_5:
    sprintf(Buffer, v6, v7);
LABEL_6:
    sub_4646F0(Buffer, (int)a1);
  }
  switch ( v4 )
  {
    case 0x80000018:
      sprintf(Buffer, byte_4EA298, v1);
      goto LABEL_6;
    case 0xFFFFFFFE:
      sub_4646F0(byte_4EA2E8, (int)a1);
    case 0xFFFFFFFF:
      sub_4646F0(byte_4EA304, (int)a1);
  }
  return 0;
}

// ===== sub_47FB20 @ 0x0047FB20..0x0047FB95 =====
int __cdecl sub_47FB20(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax
  int v4; // esi
  int v6; // [esp+8h] [ebp-4h] BYREF

  v1 = sub_4450B0(a1);
  if ( sub_462060(v1) )
    sub_4646F0(byte_4EA324, (int)a1);
  v2 = sub_463400(&v6, v1) - 1;
  if ( !v2 )
    sub_4646F0(byte_4EA354, (int)a1);
  v3 = v2 - 1;
  if ( !v3 )
    sub_4646F0(byte_4EA388, (int)a1);
  if ( v3 == 1 )
    sub_4646F0(byte_4EA3B4, (int)a1);
  v4 = v6;
  sub_4636A0(v6);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_47FBA0 @ 0x0047FBA0..0x0047FBDD =====
int __cdecl sub_47FBA0(_DWORD *a1)
{
  void *v1; // esi

  v1 = (void *)sub_4450B0(a1);
  sub_463890((int)v1);
  sub_463720(v1);
  if ( !sub_463450((int)v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FBE0 @ 0x0047FBE0..0x0047FC14 =====
int __cdecl sub_47FBE0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_463460(v2, v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FC20 @ 0x0047FC20..0x0047FC63 =====
int __cdecl sub_47FC20(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_463470(v3, v1, v1, v2) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FC70 @ 0x0047FC70..0x0047FCB3 =====
int __cdecl sub_47FC70(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_463480(v3, v1, v2) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FCC0 @ 0x0047FCC0..0x0047FD05 =====
int __cdecl sub_47FCC0(_DWORD *a1)
{
  int v1; // eax
  _DWORD *v2; // eax
  int v4[2]; // [esp+8h] [ebp-8h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_463490(v4, v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  v2 = sub_4450D0(a1, v4[0]);
  sub_4450D0(v2, v4[1]);
  return 0;
}

// ===== sub_47FD10 @ 0x0047FD10..0x0047FD99 =====
int __cdecl sub_47FD10(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_4634B0(v1, v2, v3);
  if ( v4 == 4 )
  {
    sprintf(Buffer, &byte_4EA408, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -1 )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FDA0 @ 0x0047FDA0..0x0047FE29 =====
int __cdecl sub_47FDA0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_4634F0(v1, v2, v3);
  if ( v4 == 5 )
  {
    sprintf(Buffer, &byte_4EA434, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -1 )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FE30 @ 0x0047FE30..0x0047FE6B =====
int __cdecl sub_47FE30(_DWORD *a1)
{
  int v1; // eax
  int v3; // [esp+8h] [ebp-4h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_4634A0(&v3, v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_47FE70 @ 0x0047FE70..0x0047FE88 =====
int __cdecl sub_47FE70(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_463970();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47FE90 @ 0x0047FE90..0x0047FEC4 =====
int __cdecl sub_47FE90(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_463530(v2, v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FED0 @ 0x0047FED0..0x0047FEFA =====
int __cdecl sub_47FED0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ecx
  _DWORD *v3; // edx

  sub_4450B0(a1);
  v1 = sub_463840();
  sub_463830(v2);
  sub_4450D0(v3, v1);
  return 0;
}

// ===== sub_47FF00 @ 0x0047FF00..0x0047FF2B =====
int __cdecl sub_47FF00(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_4638E0(v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FF30 @ 0x0047FF30..0x0047FF5B =====
int __cdecl sub_47FF30(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_463890(v1) )
    sub_4646F0(byte_4EA3E4, (int)a1);
  return 0;
}

// ===== sub_47FF60 @ 0x0047FF60..0x0047FF8A =====
int __cdecl sub_47FF60(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_463540();
  if ( !v1 )
    sub_4646F0(byte_4EA460, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_47FF90 @ 0x0047FF90..0x0047FFB7 =====
int __cdecl sub_47FF90(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_463550(v1) )
    sub_4646F0(byte_4EA498, (int)a1);
  return 0;
}

// ===== sub_47FFC0 @ 0x0047FFC0..0x0047FFF4 =====
int __cdecl sub_47FFC0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_463560(v2, v1) )
    sub_4646F0(byte_4EA498, (int)a1);
  return 0;
}
