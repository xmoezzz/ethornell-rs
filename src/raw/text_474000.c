#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_474050 @ 0x00474050..0x00474093 =====
int __cdecl sub_474050(_DWORD *a1)
{
  long double v1; // st7

  v1 = sin((double)sub_4450B0(a1) * 3.141592653589793 / 11796480.0) * 65536.0;
  sub_4450D0(a1, (int)v1);
  return 0;
}

// ===== sub_4740A0 @ 0x004740A0..0x004740E3 =====
int __cdecl sub_4740A0(_DWORD *a1)
{
  long double v1; // st7

  v1 = cos((double)sub_4450B0(a1) * 3.141592653589793 / 11796480.0) * 65536.0;
  sub_4450D0(a1, (int)v1);
  return 0;
}

// ===== sub_4740F0 @ 0x004740F0..0x00474128 =====
int __cdecl sub_4740F0(int a1)
{
  _QWORD *v1; // edi
  _QWORD *v2; // ebx

  v1 = (_QWORD *)sub_48DF50(a1);
  v2 = (_QWORD *)sub_48DF50(a1);
  *(_QWORD *)sub_48DF50(a1) = *v1 + *v2;
  return 0;
}

// ===== sub_474130 @ 0x00474130..0x00474168 =====
int __cdecl sub_474130(int a1)
{
  _QWORD *v1; // edi
  _QWORD *v2; // ebx

  v1 = (_QWORD *)sub_48DF50(a1);
  v2 = (_QWORD *)sub_48DF50(a1);
  *(_QWORD *)sub_48DF50(a1) = *v2 - *v1;
  return 0;
}

// ===== sub_474170 @ 0x00474170..0x004741B3 =====
int __cdecl sub_474170(int a1)
{
  _QWORD *v1; // edi
  _QWORD *v2; // ebx

  v1 = (_QWORD *)sub_48DF50(a1);
  v2 = (_QWORD *)sub_48DF50(a1);
  *(_QWORD *)sub_48DF50(a1) = *v2 * *v1;
  return 0;
}

// ===== sub_4741C0 @ 0x004741C0..0x00474203 =====
int __cdecl sub_4741C0(int a1)
{
  __int64 *v1; // edi
  _QWORD *v2; // ebx

  v1 = (__int64 *)sub_48DF50(a1);
  v2 = (_QWORD *)sub_48DF50(a1);
  *(_QWORD *)sub_48DF50(a1) = *v2 / *v1;
  return 0;
}

// ===== sub_474210 @ 0x00474210..0x00474253 =====
int __cdecl sub_474210(int a1)
{
  _QWORD *v1; // edi
  __int64 *v2; // ebx

  v1 = (_QWORD *)sub_48DF50(a1);
  v2 = (__int64 *)sub_48DF50(a1);
  *(_QWORD *)sub_48DF50(a1) = *v2 % *v1;
  return 0;
}

// ===== sub_474260 @ 0x00474260..0x00474294 =====
int __cdecl sub_474260(_DWORD *a1)
{
  size_t v1; // edi
  const void *v2; // ebx
  void *v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const void *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  memcpy_0(v3, v2, v1);
  return 0;
}

// ===== sub_4742A0 @ 0x004742A0..0x004742C4 =====
int __cdecl sub_4742A0(_DWORD *a1)
{
  int v1; // edx
  void *v2; // eax
  size_t v4; // [esp-4h] [ebp-4h]

  v4 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(v1);
  memset(v2, 0, v4);
  return 0;
}

// ===== sub_4742D0 @ 0x004742D0..0x004742FE =====
int __cdecl sub_4742D0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // edx
  void *v4; // eax
  size_t v6; // [esp-4h] [ebp-8h]

  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(v2);
  v4 = (void *)sub_48DF50(v3);
  memset(v4, v1, v6);
  return 0;
}

// ===== sub_474300 @ 0x00474300..0x0047439B =====
int __cdecl sub_474300(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned __int8 *v2; // esi
  unsigned __int8 *v3; // eax
  int v4; // ecx
  int v5; // ecx

  v1 = sub_4450B0(a1);
  v2 = (unsigned __int8 *)sub_48DF50(a1);
  v3 = (unsigned __int8 *)sub_48DF50(a1);
  if ( v1 < 4 )
  {
LABEL_4:
    if ( !v1 )
      goto LABEL_13;
  }
  else
  {
    while ( *(_DWORD *)v3 == *(_DWORD *)v2 )
    {
      v1 -= 4;
      v2 += 4;
      v3 += 4;
      if ( v1 < 4 )
        goto LABEL_4;
    }
  }
  v4 = *v3 - *v2;
  if ( !v4 )
  {
    if ( v1 <= 1 )
      goto LABEL_13;
    v4 = v3[1] - v2[1];
    if ( !v4 )
    {
      if ( v1 <= 2 )
        goto LABEL_13;
      v4 = v3[2] - v2[2];
      if ( !v4 )
      {
        if ( v1 > 3 )
        {
          v4 = v3[3] - v2[3];
          goto LABEL_12;
        }
LABEL_13:
        v5 = 0;
        goto LABEL_14;
      }
    }
  }
LABEL_12:
  v5 = (v4 >> 31) | 1;
LABEL_14:
  sub_4450D0(a1, v5 == 0);
  return 0;
}

// ===== sub_4743A0 @ 0x004743A0..0x004743FB =====
int __cdecl sub_4743A0(_DWORD *a1)
{
  const void *v1; // ebx
  size_t v2; // edi
  char *i; // esi
  int v5; // [esp+Ch] [ebp-4h]

  v1 = (const void *)sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  for ( i = (char *)sub_48DF50(a1); v5; --v5 )
  {
    memcpy_0(i, v1, v2);
    i += v2;
  }
  return 0;
}

// ===== sub_474400 @ 0x00474400..0x004744C2 =====
int __cdecl sub_474400(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // eax
  int v3; // ecx
  int v4; // edx
  unsigned int v5; // ecx
  int v6; // edi
  unsigned int v8; // [esp+10h] [ebp-Ch]
  int v9; // [esp+14h] [ebp-8h]
  int v10; // [esp+18h] [ebp-4h]

  v9 = sub_48DF50(a1);
  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = -1;
  v10 = 0;
  if ( !v8 )
    goto LABEL_14;
  while ( 1 )
  {
    v4 = v9;
    v5 = v1;
    v6 = v2;
    if ( v1 >= 4 )
    {
      while ( *(_DWORD *)v6 == *(_DWORD *)v4 )
      {
        v5 -= 4;
        v4 += 4;
        v6 += 4;
        if ( v5 < 4 )
          goto LABEL_5;
      }
      goto LABEL_11;
    }
LABEL_5:
    if ( !v5
      || *(_BYTE *)v4 == *(_BYTE *)v6
      && (v5 <= 1 || *(_BYTE *)(v4 + 1) == *(_BYTE *)(v6 + 1) && (v5 <= 2 || *(_BYTE *)(v4 + 2) == *(_BYTE *)(v6 + 2))) )
    {
      break;
    }
LABEL_11:
    v2 += v1;
    if ( ++v10 >= v8 )
    {
      sub_4450D0(a1, -1);
      return 0;
    }
  }
  v3 = v10;
LABEL_14:
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4744D0 @ 0x004744D0..0x0047451E =====
int __cdecl sub_4744D0(_DWORD *a1)
{
  const char *v1; // ebx
  const char *v2; // esi
  char *v3; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = strstr(v2, v1);
  if ( v3 )
    sub_4450D0(a1, v3 - v2);
  else
    sub_4450D0(a1, -1);
  return 0;
}

// ===== sub_474520 @ 0x00474520..0x0047456D =====
int __cdecl sub_474520(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_495A80(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_474570 @ 0x00474570..0x0047459A =====
int __cdecl sub_474570(_DWORD *a1)
{
  int v1; // kr00_4

  v1 = strlen((const char *)sub_48DF50(a1));
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4745A0 @ 0x004745A0..0x004745FB =====
int __cdecl sub_4745A0(_DWORD *a1)
{
  const char *v1; // esi
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = strcmp((const char *)sub_48DF50(a1), v1);
  sub_4450D0(a1, v2 == 0);
  return 0;
}

// ===== sub_474600 @ 0x00474600..0x00474630 =====
int __cdecl sub_474600(int a1)
{
  char *v1; // esi
  int v2; // eax
  char v3; // cl

  v1 = (char *)sub_48DF50(a1);
  v2 = sub_48DF50(a1) - (_DWORD)v1;
  do
  {
    v3 = *v1;
    v1[v2] = *v1;
    ++v1;
  }
  while ( v3 );
  return 0;
}

// ===== sub_474630 @ 0x00474630..0x00474669 =====
int __cdecl sub_474630(int a1)
{
  const char *v1; // edi
  const char *v2; // ebx
  char *v3; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (char *)sub_48DF50(a1);
  sprintf(v3, "%s%s", v2, v1);
  return 0;
}

// ===== sub_474670 @ 0x00474670..0x004746BC =====
int __cdecl sub_474670(_DWORD *a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  int v3; // eax
  int v5; // [esp+Ch] [ebp-4h] BYREF

  sub_48DF50(a1);
  v1 = sub_42EA10(&v5);
  v2 = sub_4450D0(a1, v5);
  sub_4450D0(v2, v1);
  v3 = sub_42EA30(v5);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4746C0 @ 0x004746C0..0x004746D6 =====
int __cdecl sub_4746C0(int a1)
{
  _BYTE *v1; // eax
  int v2; // ecx

  v1 = (_BYTE *)sub_48DF50(a1);
  sub_42EA80(v2, v1);
  return 0;
}

// ===== sub_4746E0 @ 0x004746E0..0x0047471D =====
int __cdecl sub_4746E0(_DWORD *a1)
{
  char v1; // bl
  const char *v2; // edi
  char *v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (char *)sub_48DF50(a1);
  sprintf(v3, "%c%s%c", v1, v2, v1);
  return 0;
}

// ===== sub_474720 @ 0x00474720..0x00474D63 =====
int __fastcall sub_474720(_DWORD *a1, const char *a2, char *a3)
{
  int result; // eax
  int v5; // ecx
  int v6; // edx
  int v7; // esi
  const char *v8; // ebx
  int *v9; // esi
  char v10; // al
  char v11; // al
  int i; // ecx
  int v13; // eax
  int v15; // [esp+14h] [ebp-14Ch]
  int v16; // [esp+1Ch] [ebp-144h] BYREF
  int v17; // [esp+20h] [ebp-140h]
  int v18; // [esp+24h] [ebp-13Ch]
  int v19; // [esp+28h] [ebp-138h]
  int v20; // [esp+2Ch] [ebp-134h]
  int v21; // [esp+30h] [ebp-130h]
  int v22; // [esp+34h] [ebp-12Ch]
  int v23; // [esp+38h] [ebp-128h]
  int v24; // [esp+3Ch] [ebp-124h]
  int v25; // [esp+40h] [ebp-120h]
  int v26; // [esp+44h] [ebp-11Ch]
  int v27; // [esp+48h] [ebp-118h]
  int v28; // [esp+4Ch] [ebp-114h]
  int v29; // [esp+50h] [ebp-110h]
  int v30; // [esp+54h] [ebp-10Ch]
  int v31; // [esp+58h] [ebp-108h]
  char Buffer[256]; // [esp+5Ch] [ebp-104h] BYREF

  result = 0;
  v5 = 0;
  v6 = 0;
  v7 = 0;
  v16 = 0;
  v17 = 0;
  v18 = 0;
  v19 = 0;
  v20 = 0;
  v21 = 0;
  v22 = 0;
  v23 = 0;
  v24 = 0;
  v25 = 0;
  v26 = 0;
  v27 = 0;
  v28 = 0;
  v29 = 0;
  v30 = 0;
  v31 = 0;
  v15 = 0;
  v8 = a2;
  if ( *a2 )
  {
    v9 = &v16;
    do
    {
      if ( *v8 == 37 )
      {
        v10 = v8[1];
        if ( v10 == 32 || v10 == 48 || v10 == 45 || v10 == 46 )
        {
          v11 = v8[2];
          for ( i = 0; v11 >= 48; v11 = v8[i++ + 3] )
          {
            if ( v11 > 57 )
              break;
          }
          v8 += i + 1;
        }
        switch ( v8[1] )
        {
          case 0:
            sub_4646F0(byte_4E7760, (int)a1);
          case 0x25:
            ++v8;
            break;
          case 0x58:
          case 0x63:
          case 0x64:
          case 0x78:
            v13 = sub_4450B0(a1);
            goto LABEL_16;
          case 0x73:
            v13 = sub_48DF50(a1);
LABEL_16:
            *v9++ = v13;
            ++v8;
            if ( v15 >= 16 )
              sub_4646F0(byte_4E77D0, (int)a1);
            ++v15;
            break;
          default:
            sprintf(Buffer, &byte_4E7790, v8[1]);
            sub_4646F0(Buffer, (int)a1);
        }
      }
      ++v8;
    }
    while ( *v8 );
    v7 = v19;
    v6 = v18;
    v5 = v17;
    result = v16;
  }
  switch ( v15 )
  {
    case 0:
      result = sprintf(a3, a2);
      break;
    case 1:
      result = sprintf(a3, a2, result);
      break;
    case 2:
      result = sprintf(a3, a2, result, v5);
      break;
    case 3:
      result = sprintf(a3, a2, result, v5, v6);
      break;
    case 4:
      result = sprintf(a3, a2, result, v5, v6, v7);
      break;
    case 5:
      result = sprintf(a3, a2, result, v5, v6, v7, v20);
      break;
    case 6:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21);
      break;
    case 7:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22);
      break;
    case 8:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23);
      break;
    case 9:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24);
      break;
    case 10:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25);
      break;
    case 11:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26);
      break;
    case 12:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26, v27);
      break;
    case 13:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26, v27, v28);
      break;
    case 14:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29);
      break;
    case 15:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30);
      break;
    case 16:
      result = sprintf(a3, a2, result, v5, v6, v7, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31);
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_474E40 @ 0x00474E40..0x00474E6B =====
int __cdecl sub_474E40(_DWORD *a1)
{
  const char *v1; // edi
  char *v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (char *)sub_48DF50(a1);
  sub_474720(a1, v1, v2);
  return 0;
}

// ===== sub_474E70 @ 0x00474E70..0x00474EC1 =====
int __cdecl sub_474E70(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4978F0(v1);
  v2 = (*(int (__thiscall **)(_DWORD *, int))(*a1 + 4))(a1, v1);
  if ( (unsigned int)(v2 + v1) > 0x4000000 )
    sub_4646F0(byte_4E7810, (int)a1);
  sub_4450D0(a1, v2 + 201326592);
  return 0;
}

// ===== sub_474ED0 @ 0x00474ED0..0x00474F59 =====
int __cdecl sub_474ED0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( (v1 & 0xFC000000) != 0xC000000 )
  {
    sprintf(Buffer, &byte_4E7838, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v2 = (*(int (__thiscall **)(_DWORD *, int))(*a1 + 8))(a1, v1 & 0x3FFFFFF);
  if ( !v2 )
    sub_4646F0(byte_4E787C, (int)a1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_474F60 @ 0x00474F60..0x00474F78 =====
int __cdecl sub_474F60(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_48E4B0(v1);
  return 0;
}

// ===== sub_474F80 @ 0x00474F80..0x00474FBD =====
int __cdecl sub_474F80(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_48DF50(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_48DFE0(a1, v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_474FC0 @ 0x00474FC0..0x00474FE5 =====
int __cdecl sub_474FC0(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_461EC0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_474FF0 @ 0x00474FF0..0x0047504A =====
int __cdecl sub_474FF0(_DWORD *a1)
{
  int v1; // esi
  const CHAR *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v3 = sub_46BC80(byte_4E78B0, hWndParent, v2, (v1 != 0 ? 0 : 256) | 0x1024);
  sub_4450D0(a1, v3 == 6);
  return 0;
}

// ===== sub_475050 @ 0x00475050..0x00475099 =====
int __cdecl sub_475050(int a1)
{
  char *v1; // esi
  const char *v2; // eax

  v1 = (char *)operator new[](0x10000u);
  v2 = (const char *)sub_48DF50(a1);
  sub_464570(a1, v1, v2);
  sub_46BC80(byte_4E78B8, hWndParent, v1, 0x1040u);
  operator delete[](v1);
  return 0;
}

// ===== sub_4750A0 @ 0x004750A0..0x00475122 =====
int __cdecl sub_4750A0(_DWORD *a1)
{
  char *v1; // esi
  int v2; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = (char *)operator new[](0x10000u);
  v2 = sub_4450B0(a1);
  sprintf(Buffer, "Number : %d ( $%.8x )", v2, v2);
  sub_464570((int)a1, v1, Buffer);
  sub_46BC80(byte_4E78E0, hWndParent, v1, 0x1040u);
  operator delete[](v1);
  return 0;
}

// ===== sub_475130 @ 0x00475130..0x00475363 =====
int __cdecl sub_475130(_DWORD *a1)
{
  int v1; // ebx
  unsigned __int8 *v2; // edi
  int v3; // eax
  signed int v4; // esi
  signed int v5; // ebx
  int v6; // eax
  char v7; // cl
  signed int v8; // eax
  int v9; // eax
  char v10; // cl
  unsigned __int8 *Src; // [esp+Ch] [ebp-8218h]
  const char *v13; // [esp+10h] [ebp-8214h]
  int v14; // [esp+14h] [ebp-8210h]
  int v15; // [esp+18h] [ebp-820Ch]
  int v16; // [esp+1Ch] [ebp-8208h]
  char v17[16384]; // [esp+20h] [ebp-8204h] BYREF
  CHAR v18[16384]; // [esp+4020h] [ebp-4204h] BYREF
  char v19[256]; // [esp+8020h] [ebp-204h] BYREF
  char Buffer[256]; // [esp+8120h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v15 = v1;
  v2 = (unsigned __int8 *)sub_48DF50(a1);
  v13 = (const char *)sub_48DF50(a1);
  if ( (unsigned int)(v1 - 1) > 0x3FF )
  {
    sprintf(v19, &byte_4E78EC, v1);
    sub_4646F0(v19, (int)a1);
  }
  memset(v17, 0, sizeof(v17));
  v3 = (v1 + 15) / 16;
  v16 = 0;
  if ( v3 )
  {
    while ( 1 )
    {
      v14 = v3 - 1;
      v4 = v1;
      if ( v1 >= 16 )
        v4 = 16;
      Src = v2;
      memset(v19, 0, sizeof(v19));
      if ( v4 > 0 )
      {
        v5 = v4;
        do
        {
          sprintf(Buffer, "%s%.2X ", v19, *v2++);
          v6 = 0;
          do
          {
            v7 = Buffer[v6];
            v19[v6++] = v7;
          }
          while ( v7 );
          --v5;
        }
        while ( v5 );
        v1 = v15;
      }
      memcpy_0(Buffer, Src, v4);
      v8 = 0;
      for ( Buffer[v4] = 0; v8 < v4; ++v8 )
      {
        if ( (unsigned __int8)Buffer[v8] < 0x20u )
          Buffer[v8] = 32;
      }
      sprintf(v18, "%s\n0x%.4X : %s %s", v17, v16, v19, Buffer);
      v9 = 0;
      do
      {
        v10 = v18[v9];
        v17[v9++] = v10;
      }
      while ( v10 );
      v16 += 16;
      v1 -= 16;
      v15 = v1;
      if ( !v14 )
        break;
      v3 = v14;
    }
  }
  sprintf(v18, "%s\n\n%s", v13, v17);
  sub_46BC80(byte_4E7934, hWndParent, v18, 0x1000u);
  return 0;
}

// ===== sub_475370 @ 0x00475370..0x004753A1 =====
int __cdecl sub_475370(_DWORD *a1)
{
  const CHAR *v1; // esi
  LPARAM v2; // eax
  INT_PTR v3; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_468E70(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4753B0 @ 0x004753B0..0x004754D2 =====
int __cdecl sub_4753B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-118h]
  int v6; // [esp+10h] [ebp-114h]
  int v7; // [esp+14h] [ebp-110h]
  int v8; // [esp+18h] [ebp-10Ch]
  int v9; // [esp+1Ch] [ebp-108h]
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v5 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v8 = sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v2);
  if ( (unsigned int)(v1 - 1) > 0x3FF )
  {
    sprintf(Buffer, byte_4E7944, v1);
LABEL_6:
    sub_4646F0(Buffer, (int)a1);
  }
  v3 = sub_401B80(v7, v6, v8, v1, v9, v5);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E7970, v9);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483646 )
  {
    sprintf(Buffer, &byte_4E799C, v2);
    goto LABEL_6;
  }
  return 0;
}

// ===== sub_4754E0 @ 0x004754E0..0x00475503 =====
int __cdecl sub_4754E0(_DWORD *a1)
{
  const char *v1; // eax
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_4301E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_475510 @ 0x00475510..0x004755AF =====
int __cdecl sub_475510(_DWORD *a1)
{
  unsigned int v1; // edi
  _DWORD *v2; // edx
  int v3; // ebx
  _DWORD *v4; // edx
  _DWORD *v5; // edx
  _DWORD *v6; // edx
  int v7; // eax
  int v9; // [esp+10h] [ebp-50h]
  int v10; // [esp+14h] [ebp-4Ch]
  _DWORD v11[6]; // [esp+18h] [ebp-48h] BYREF
  _DWORD v12[6]; // [esp+30h] [ebp-30h] BYREF
  _DWORD v13[6]; // [esp+48h] [ebp-18h] BYREF

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v10 = sub_4450B0(v4);
  v9 = sub_4450B0(v5);
  v7 = sub_4450B0(v6);
  if ( sub_407F20((int)dword_566750, v7, v11)
    && sub_407F20((int)dword_566750, v9, v13)
    && sub_407F20((int)dword_566750, v10, v12) )
  {
    sub_40BD60((int)v12, (int)v13, (int)v11, v3, v1);
  }
  return 0;
}

// ===== sub_4755B0 @ 0x004755B0..0x00475641 =====
int __cdecl sub_4755B0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490CD0(&v5) - 1;
  if ( !v3 )
    sub_4646F0(byte_4E79D0, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E7A18, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_475650 @ 0x00475650..0x00475677 =====
int __cdecl sub_475650(_DWORD *a1)
{
  sub_4450B0(a1);
  if ( !sub_490CE0() )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475680 @ 0x00475680..0x004756B4 =====
int __cdecl sub_475680(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_490CF0(v1) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_4756C0 @ 0x004756C0..0x0047574F =====
int __cdecl sub_4756C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-10h]
  int v5; // [esp+10h] [ebp-Ch]
  int v6; // [esp+14h] [ebp-8h]
  int v7; // [esp+18h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  sub_497C40(v7);
  if ( !sub_490D00(v4, v5, v6, v7) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475750 @ 0x00475750..0x004757F2 =====
int __cdecl sub_475750(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_48DF50(a1);
  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490D20(v2, v1) - 7;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E7A84, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
    sub_4646F0(byte_4E7AB0, (int)a1);
  if ( v4 == 247 )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475800 @ 0x00475800..0x00475827 =====
int __cdecl sub_475800(_DWORD *a1)
{
  sub_4450B0(a1);
  if ( !sub_490D40() )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475830 @ 0x00475830..0x00475865 =====
int __cdecl sub_475830(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_490C30(v1, v2) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475870 @ 0x00475870..0x004758A4 =====
int __cdecl sub_475870(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_490D50(v1) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_4758B0 @ 0x004758B0..0x004759BC =====
int __cdecl sub_4758B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  int v5; // [esp+Ch] [ebp-11Ch]
  int v6; // [esp+10h] [ebp-118h]
  int v7; // [esp+14h] [ebp-114h]
  int v8; // [esp+18h] [ebp-110h]
  int v9; // [esp+1Ch] [ebp-10Ch]
  int v10; // [esp+20h] [ebp-108h]
  char Buffer[256]; // [esp+24h] [ebp-104h] BYREF

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490D60(v2, v7, v9, v10, v8, v5, v6, v1);
  if ( v3 == 3 )
  {
    sprintf(Buffer, &byte_4E7AEC, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_4759C0 @ 0x004759C0..0x00475A3B =====
int __cdecl sub_4759C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490D90(v2);
  if ( v3 == 4 )
  {
    sprintf(Buffer, &byte_4E7B10, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475A40 @ 0x00475A40..0x00475A74 =====
int __cdecl sub_475A40(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_490DA0(v1) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475A80 @ 0x00475A80..0x00475AA7 =====
int __cdecl sub_475A80(_DWORD *a1)
{
  sub_4450B0(a1);
  if ( !sub_490DB0() )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475AB0 @ 0x00475AB0..0x00475B71 =====
int __cdecl sub_475AB0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-20h]
  int v6; // [esp+10h] [ebp-1Ch]
  int v7; // [esp+14h] [ebp-18h]
  int v8; // [esp+18h] [ebp-14h]
  int v9; // [esp+1Ch] [ebp-10h]
  int v10; // [esp+20h] [ebp-Ch]
  int v11; // [esp+24h] [ebp-8h]
  int v12; // [esp+28h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  if ( !sub_490DC0(v3, v5, v6, v7, v8, v9, v10, v11, v12, v2, v1) )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_475B80 @ 0x00475B80..0x00475CAB =====
int __cdecl sub_475B80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v5; // [esp+Ch] [ebp-114h]
  int v6; // [esp+10h] [ebp-110h]
  int v7; // [esp+14h] [ebp-10Ch]
  int v8; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  switch ( sub_490E00(v6, v8, v2, v1, v7, v5, v3) )
  {
    case 0x80000001:
      sprintf(Buffer, &byte_4E7B38, v6);
      break;
    case 0x80000002:
      sprintf(Buffer, &byte_4E7B6C, v8);
      sub_4646F0(Buffer, (int)a1);
    case 0x80000004:
      sprintf(Buffer, &byte_4E799C, v1);
      break;
    case 0x80000005:
      sprintf(Buffer, &byte_4E7BA0, v1, v2 + v1 - 1);
      break;
    case 0x80000006:
      sprintf(Buffer, &byte_4E7BE8, v2);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_475CD0 @ 0x00475CD0..0x00475E25 =====
int __cdecl sub_475CD0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-11Ch]
  int v5; // [esp+10h] [ebp-118h]
  int v6; // [esp+14h] [ebp-114h]
  int v7; // [esp+18h] [ebp-110h]
  int v8; // [esp+1Ch] [ebp-10Ch]
  int v9; // [esp+20h] [ebp-108h]
  char Buffer[256]; // [esp+24h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497E00(v1);
  switch ( sub_490F60(v8, v9, v6, v2, v5, v7, v4, v1) )
  {
    case 0x80000001:
      sprintf(Buffer, &byte_4E7B38, v8);
      break;
    case 0x80000002:
      sprintf(Buffer, &byte_4E7B6C, v9);
      sub_4646F0(Buffer, (int)a1);
    case 0x80000004:
      sprintf(Buffer, &byte_4E799C, v2);
      break;
    case 0x80000005:
      sprintf(Buffer, &byte_4E7BA0, v2, v6 + v2 - 1);
      break;
    case 0x80000006:
      sprintf(Buffer, &byte_4E7BE8, v6);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_475E40 @ 0x00475E40..0x00475F1D =====
int __cdecl sub_475E40(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  sub_497E00(v1);
  v3 = sub_4910C0(v1);
  switch ( v3 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E7B38, v5);
      sub_4646F0(Buffer, (int)a1);
    case -2147483646:
      sprintf(Buffer, &byte_4E7B6C, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483641:
      sprintf(Buffer, &byte_4E7C18, v5, v2);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_475F20 @ 0x00475F20..0x00475F7B =====
int __cdecl sub_475F20(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( !sub_4910E0(v1) )
  {
    sprintf(Buffer, &byte_4E7C74, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_475F80 @ 0x00475F80..0x0047603A =====
int __cdecl sub_475F80(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_491100(v3, v2);
  switch ( v4 )
  {
    case 6:
      sprintf(Buffer, &byte_4E7CCC, v1);
      sub_4646F0(Buffer, (int)a1);
    case 10:
      sprintf(Buffer, &byte_4E7CA0, v2);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4E7A4C, (int)a1);
  }
  return 0;
}

// ===== sub_476040 @ 0x00476040..0x0047610A =====
int __cdecl sub_476040(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490E00(0, v2, 1, v1, 0, 0, 0);
  switch ( v3 )
  {
    case -2147483646:
      sprintf(Buffer, &byte_4E7CA0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483643:
      sprintf(Buffer, &byte_4E7D00, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_476110 @ 0x00476110..0x00476233 =====
int __cdecl sub_476110(_DWORD *a1)
{
  _DWORD *v1; // ebx
  int v2; // edi
  int v3; // edx
  int v5; // [esp+Ch] [ebp-128h]
  _DWORD v6[2]; // [esp+10h] [ebp-124h] BYREF
  int v7; // [esp+18h] [ebp-11Ch]
  int v8; // [esp+1Ch] [ebp-118h]
  int v9; // [esp+20h] [ebp-114h]
  int v10; // [esp+24h] [ebp-110h]
  int v11; // [esp+28h] [ebp-10Ch]
  int v12; // [esp+2Ch] [ebp-108h]
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v1 = (_DWORD *)sub_48DF50(a1);
  sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6[1] = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6[0] = *v1;
  if ( !sub_491120(v2, v10, v11, v12, v9, v7, v5, v3, v6) )
  {
    sprintf(Buffer, &byte_4E7CA0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_476240 @ 0x00476240..0x004762FA =====
int __cdecl sub_476240(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v4 = sub_491160(v3, v2);
  switch ( v4 )
  {
    case 6:
      sprintf(Buffer, &byte_4E7CCC, v1);
      sub_4646F0(Buffer, (int)a1);
    case 10:
      sprintf(Buffer, &byte_4E7CA0, v2);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4E7A4C, (int)a1);
  }
  return 0;
}

// ===== sub_476300 @ 0x00476300..0x00476481 =====
int __cdecl sub_476300(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-134h]
  int v8; // [esp+10h] [ebp-130h]
  int v9; // [esp+14h] [ebp-12Ch]
  int v10; // [esp+18h] [ebp-128h]
  int v11; // [esp+1Ch] [ebp-124h]
  int v12; // [esp+20h] [ebp-120h]
  int v13; // [esp+24h] [ebp-11Ch]
  int v14; // [esp+28h] [ebp-118h]
  int v15; // [esp+2Ch] [ebp-114h]
  int v16; // [esp+30h] [ebp-110h]
  int v17; // [esp+34h] [ebp-10Ch]
  int v18; // [esp+38h] [ebp-108h]
  char Buffer[256]; // [esp+3Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_491180(v3, v9, v11, v13, v15, v17, v18, v10, v14, v8, v16, v7, v12, v1, v4);
  if ( v5 == 10 )
  {
    sprintf(Buffer, &byte_4E7CA0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 255 )
    sub_4646F0(byte_4E7A4C, (int)a1);
  return 0;
}

// ===== sub_476490 @ 0x00476490..0x0047655A =====
int __cdecl sub_476490(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_490E00(1, v2, 1, v1, 0, 0, 0);
  switch ( v3 )
  {
    case -2147483646:
      sprintf(Buffer, &byte_4E7CA0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E799C, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483643:
      sprintf(Buffer, &byte_4E7D00, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_476560 @ 0x00476560..0x00476711 =====
int __cdecl sub_476560(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int *v3; // edx
  int v5; // [esp+Ch] [ebp-144h]
  int v6; // [esp+10h] [ebp-140h]
  int v7; // [esp+14h] [ebp-13Ch]
  int v8; // [esp+18h] [ebp-138h]
  int v9; // [esp+1Ch] [ebp-134h]
  int v10; // [esp+20h] [ebp-130h]
  int v11; // [esp+24h] [ebp-12Ch]
  int v12; // [esp+28h] [ebp-128h]
  int v13; // [esp+2Ch] [ebp-124h]
  int v14; // [esp+30h] [ebp-120h]
  int v15; // [esp+34h] [ebp-11Ch]
  int v16; // [esp+38h] [ebp-118h]
  int v17; // [esp+3Ch] [ebp-114h]
  int v18; // [esp+40h] [ebp-110h] BYREF
  int v19; // [esp+44h] [ebp-10Ch]
  int v20; // [esp+48h] [ebp-108h]
  char Buffer[256]; // [esp+4Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  sub_48DF50(a1);
  v8 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v20 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v18 = *v3;
  if ( !sub_4911D0(v2, v7, v9, v11, v13, v15, v17, v19, v20, v6, v14, v10, v16, v8, &v18, v5, v12, v1) )
  {
    sprintf(Buffer, &byte_4E7CA0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_476720 @ 0x00476720..0x004767B1 =====
int __cdecl sub_476720(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_492150(&v5) - 1;
  if ( !v3 )
    sub_4646F0(byte_4E7D40, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E7A18, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_4767C0 @ 0x004767C0..0x004767E7 =====
int __cdecl sub_4767C0(_DWORD *a1)
{
  sub_4450B0(a1);
  if ( !sub_492160() )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_4767F0 @ 0x004767F0..0x00476824 =====
int __cdecl sub_4767F0(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_492170(v1) )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476830 @ 0x00476830..0x004768CB =====
int __cdecl sub_476830(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_492180() - 4;
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4E7DB4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v3 = v2 - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E7DF0, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 250 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_4768D0 @ 0x004768D0..0x00476904 =====
int __cdecl sub_4768D0(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_492190(v1) )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476910 @ 0x00476910..0x004769A2 =====
int __cdecl sub_476910(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-10h]
  int v5; // [esp+10h] [ebp-Ch]
  int v6; // [esp+14h] [ebp-8h]
  int v7; // [esp+18h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  sub_497C40(v7);
  if ( sub_4921A0(v4, v5, v6, v7) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_4769B0 @ 0x004769B0..0x00476A31 =====
int __cdecl sub_4769B0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]
  int v6; // [esp+14h] [ebp-4h]

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( sub_4921C0(v2, v4, v5, v6, v1) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476A40 @ 0x00476A40..0x00476ABB =====
int __cdecl sub_476A40(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4921E0(v2);
  if ( v3 == 3 )
  {
    sprintf(Buffer, &byte_4E7E64, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476AC0 @ 0x00476AC0..0x00476B3B =====
int __cdecl sub_476AC0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4921F0(v2);
  if ( v3 == 3 )
  {
    sprintf(Buffer, &byte_4E7E9C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476B40 @ 0x00476B40..0x00476B77 =====
int __cdecl sub_476B40(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( sub_492200(v1) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476B80 @ 0x00476B80..0x00476BB7 =====
int __cdecl sub_476B80(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( sub_492210(v1) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476BC0 @ 0x00476BC0..0x00476C3B =====
int __cdecl sub_476BC0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_492220(v2);
  if ( v3 == 3 )
  {
    sprintf(Buffer, &byte_4E7EC4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476C40 @ 0x00476C40..0x00476C92 =====
int __cdecl sub_476C40(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( sub_492230(v2, v1) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476CA0 @ 0x00476CA0..0x00476CF2 =====
int __cdecl sub_476CA0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( sub_492250(v2, v1) == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476D00 @ 0x00476D00..0x00476D7B =====
int __cdecl sub_476D00(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_492270(v2);
  if ( v3 == 3 )
  {
    sprintf(Buffer, &byte_4E7EF8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4E7D84, (int)a1);
  return 0;
}

// ===== sub_476D80 @ 0x00476D80..0x00476DEA =====
int __cdecl sub_476D80(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  if ( !sub_492280(v2) )
  {
    sprintf(Buffer, &byte_4E7F2C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_476DF0 @ 0x00476DF0..0x00476E08 =====
int __cdecl sub_476DF0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_494930();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_476E10 @ 0x00476E10..0x00476E35 =====
int __cdecl sub_476E10(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4949E0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_476E40 @ 0x00476E40..0x00476E89 =====
int __cdecl sub_476E40(_DWORD *a1)
{
  int v1; // esi
  int v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_494A60(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_476E90 @ 0x00476E90..0x00476ECD =====
int __cdecl sub_476E90(_DWORD *a1)
{
  int v1; // esi
  int v2; // edi
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_494AE0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_476ED0 @ 0x00476ED0..0x00476F2F =====
int __cdecl sub_476ED0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  _DWORD *v6; // [esp+Ch] [ebp-8h]
  int v7; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v7 = sub_48DF50(a1);
  v6 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_4066C0(v2, v3, v6, v7, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_476F30 @ 0x00476F30..0x00476F92 =====
int __cdecl sub_476F30(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(int); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = funcs_476F7E[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4E7F5C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2((int)a1);
}

// ===== sub_476FA0 @ 0x00476FA0..0x00476FDD =====
int __cdecl sub_476FA0(_DWORD *a1)
{
  int v1; // esi
  void *v2; // ebx
  int v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_496740(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_476FE0 @ 0x00476FE0..0x00477005 =====
int __cdecl sub_476FE0(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_496810();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_477010 @ 0x00477010..0x00477058 =====
int __cdecl sub_477010(_DWORD *a1)
{
  void *v1; // esi
  int v2; // ebx
  int v3; // edx
  int v4; // eax

  v1 = (void *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v4 = sub_496890(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477060 @ 0x00477060..0x0047709E =====
int __cdecl sub_477060(_DWORD *a1)
{
  void *v1; // esi
  int v2; // eax
  int v3; // eax

  v1 = (void *)sub_48DF50(a1);
  sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_4968C0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4770A0 @ 0x004770A0..0x004770D2 =====
int __cdecl sub_4770A0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_4968F0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4770E0 @ 0x004770E0..0x00477112 =====
int __cdecl sub_4770E0(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_496920(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477120 @ 0x00477120..0x00477152 =====
int __cdecl sub_477120(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_496950(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477160 @ 0x00477160..0x004771A6 =====
int __cdecl sub_477160(_DWORD *a1)
{
  int v1; // edx
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_496980(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4771B0 @ 0x004771B0..0x004771F7 =====
int __cdecl sub_4771B0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_4969E0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477200 @ 0x00477200..0x0047723E =====
int __cdecl sub_477200(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_496A10(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477240 @ 0x00477240..0x0047727E =====
int __cdecl sub_477240(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_496A40(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477280 @ 0x00477280..0x004772BD =====
int __cdecl sub_477280(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4969B0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4772C0 @ 0x004772C0..0x00477329 =====
int __cdecl sub_4772C0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-8h]
  int v7; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_4450B0(a1);
  v4 = sub_496A70(v6, v7, v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477330 @ 0x00477330..0x0047739B =====
int __cdecl sub_477330(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-8h]
  int v7; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_496AB0(v3, v6, v7, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4773A0 @ 0x004773A0..0x004773DE =====
int __cdecl sub_4773A0(_DWORD *a1)
{
  int v1; // esi
  void *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v3 = sub_496AF0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4773E0 @ 0x004773E0..0x0047742F =====
int __cdecl sub_4773E0(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax
  int v5; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_496B20(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477430 @ 0x00477430..0x004774E1 =====
int __cdecl sub_477430(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-1Ch]
  int v7; // [esp+14h] [ebp-18h]
  int v8; // [esp+18h] [ebp-14h]
  int v9; // [esp+1Ch] [ebp-10h]
  int v10; // [esp+20h] [ebp-Ch]
  int v11; // [esp+24h] [ebp-8h]
  int v12; // [esp+28h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_496B50(v3, v6, v7, v8, v9, v10, v11, v12, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4774F0 @ 0x004774F0..0x0047754F =====
int __cdecl sub_4774F0(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax
  int v5; // [esp+Ch] [ebp-8h]
  int v6; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_496BA0(v2, v5, v6, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477550 @ 0x00477550..0x004775BB =====
int __cdecl sub_477550(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-8h]
  int v7; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_496BE0(v3, v6, v7, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4775C0 @ 0x004775C0..0x004775E7 =====
int __cdecl sub_4775C0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_496C20(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4775F0 @ 0x004775F0..0x00477615 =====
int __cdecl sub_4775F0(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_496CC0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_477620 @ 0x00477620..0x00477708 =====
int __cdecl sub_477620(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // edx
  int v4; // eax
  float v6; // [esp+0h] [ebp-5Ch]
  float v7; // [esp+4h] [ebp-58h]
  float v8; // [esp+8h] [ebp-54h]
  float v9; // [esp+Ch] [ebp-50h]
  float v10; // [esp+10h] [ebp-4Ch]
  float v11; // [esp+14h] [ebp-48h]
  float v12; // [esp+18h] [ebp-44h]
  float v13; // [esp+1Ch] [ebp-40h]
  float v14; // [esp+20h] [ebp-3Ch]
  int v15; // [esp+38h] [ebp-24h]
  int v16; // [esp+3Ch] [ebp-20h]
  int v17; // [esp+40h] [ebp-1Ch]
  int v18; // [esp+44h] [ebp-18h]
  int v19; // [esp+48h] [ebp-14h]
  int v20; // [esp+4Ch] [ebp-10h]
  int v21; // [esp+50h] [ebp-Ch]
  int v22; // [esp+54h] [ebp-8h]
  int v23; // [esp+58h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v23 = sub_4450B0(a1);
  v22 = sub_4450B0(a1);
  v21 = sub_4450B0(a1);
  v20 = sub_4450B0(a1);
  v19 = sub_4450B0(a1);
  v18 = sub_4450B0(a1);
  v17 = sub_4450B0(a1);
  v16 = sub_4450B0(a1);
  v15 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v14 = (float)v23;
  v13 = (float)v22;
  v12 = (float)v21;
  v11 = (float)v20;
  v10 = (float)v19;
  v9 = (float)v18;
  v8 = (float)v17;
  v7 = (float)v16;
  v6 = (float)v15;
  v4 = sub_496D60(v3, v6, v7, v8, v9, v10, v11, v12, v13, v14, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477710 @ 0x00477710..0x00477742 =====
int __cdecl sub_477710(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_496DE0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477750 @ 0x00477750..0x004777BC =====
int __cdecl sub_477750(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  float v4; // [esp+0h] [ebp-20h]
  float v5; // [esp+4h] [ebp-1Ch]
  float v6; // [esp+8h] [ebp-18h]
  int v7; // [esp+14h] [ebp-Ch]
  int v8; // [esp+18h] [ebp-8h]
  int v9; // [esp+1Ch] [ebp-4h]

  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = (float)v9;
  v5 = (float)v8;
  v4 = (float)v7;
  v2 = sub_496E10(v1, v4, v5, v6);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4777C0 @ 0x004777C0..0x00477844 =====
int __cdecl sub_4777C0(_DWORD *a1)
{
  int v1; // esi
  int v2; // esi
  double v3; // st7
  int v4; // eax
  double v5; // st7
  _DWORD *v7; // [esp+Ch] [ebp-18h]
  float v8[4]; // [esp+10h] [ebp-14h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_496E50(v8, v1);
  if ( !v2 )
  {
    v3 = v8[1];
    *v7 = (int)v8[0];
    v4 = (int)v3;
    v5 = v8[2];
    v7[1] = v4;
    v7[2] = (int)v5;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477850 @ 0x00477850..0x004778BC =====
int __cdecl sub_477850(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  float v4; // [esp+0h] [ebp-20h]
  float v5; // [esp+4h] [ebp-1Ch]
  float v6; // [esp+8h] [ebp-18h]
  int v7; // [esp+14h] [ebp-Ch]
  int v8; // [esp+18h] [ebp-8h]
  int v9; // [esp+1Ch] [ebp-4h]

  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = (float)v9;
  v5 = (float)v8;
  v4 = (float)v7;
  v2 = sub_496E80(v1, v4, v5, v6);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4778C0 @ 0x004778C0..0x00477944 =====
int __cdecl sub_4778C0(_DWORD *a1)
{
  int v1; // esi
  int v2; // esi
  double v3; // st7
  int v4; // eax
  double v5; // st7
  _DWORD *v7; // [esp+Ch] [ebp-18h]
  float v8[4]; // [esp+10h] [ebp-14h] BYREF

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_496EC0(v8, v1);
  if ( !v2 )
  {
    v3 = v8[1];
    *v7 = (int)v8[0];
    v4 = (int)v3;
    v5 = v8[2];
    v7[1] = v4;
    v7[2] = (int)v5;
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477950 @ 0x00477950..0x00477998 =====
int __cdecl sub_477950(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // edx
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v4 = sub_496EF0(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4779A0 @ 0x004779A0..0x004779ED =====
int __cdecl sub_4779A0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_496F20(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4779F0 @ 0x004779F0..0x00477A4C =====
int __cdecl sub_4779F0(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax
  int v5; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_496F50(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477A50 @ 0x00477A50..0x00477AD2 =====
int __cdecl sub_477A50(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // edx
  int v4; // eax
  float v6; // [esp+0h] [ebp-2Ch]
  float v7; // [esp+4h] [ebp-28h]
  float v8; // [esp+8h] [ebp-24h]
  int v9; // [esp+20h] [ebp-Ch]
  int v10; // [esp+24h] [ebp-8h]
  int v11; // [esp+28h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v8 = (float)v11;
  v7 = (float)v10;
  v6 = (float)v9;
  v4 = sub_496F80(v3, v6, v7, v8, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477AE0 @ 0x00477AE0..0x00477B99 =====
int __cdecl sub_477AE0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  float v6; // [esp+0h] [ebp-44h]
  float v7; // [esp+4h] [ebp-40h]
  int v8; // [esp+28h] [ebp-1Ch]
  int v9; // [esp+2Ch] [ebp-18h]
  int v10; // [esp+30h] [ebp-14h]
  int v11; // [esp+34h] [ebp-10h]
  int v12; // [esp+38h] [ebp-Ch]
  int v13; // [esp+3Ch] [ebp-8h]
  int v14; // [esp+40h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  sub_4450B0(a1);
  v8 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v7 = (float)v12;
  v6 = (float)v11;
  v4 = sub_496FD0(v3, v8, v9, v10, v6, v7, v13, v14, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477BA0 @ 0x00477BA0..0x00477BFD =====
int __cdecl sub_477BA0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_497030(v3, v6, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477C00 @ 0x00477C00..0x00477CA3 =====
int __cdecl sub_477C00(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  float v6; // [esp+0h] [ebp-3Ch]
  float v7; // [esp+4h] [ebp-38h]
  float v8; // [esp+8h] [ebp-34h]
  float v9; // [esp+Ch] [ebp-30h]
  int v10; // [esp+28h] [ebp-14h]
  int v11; // [esp+2Ch] [ebp-10h]
  int v12; // [esp+30h] [ebp-Ch]
  int v13; // [esp+34h] [ebp-8h]
  int v14; // [esp+38h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  sub_4450B0(a1);
  v10 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v9 = (float)v14;
  v8 = (float)v13;
  v7 = (float)v12;
  v6 = (float)v11;
  v4 = sub_497070(v3, v10, v6, v7, v8, v9, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477CB0 @ 0x00477CB0..0x00477D0D =====
int __cdecl sub_477CB0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_4970D0(v3, v6, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477D10 @ 0x00477D10..0x00477D5D =====
int __cdecl sub_477D10(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_497110(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477D60 @ 0x00477D60..0x00477D78 =====
int __cdecl sub_477D60(int a1)
{
  int v1; // eax

  v1 = sub_48DF50(a1);
  sub_497250(v1);
  return 0;
}

// ===== sub_477D80 @ 0x00477D80..0x00477DA5 =====
int __cdecl sub_477D80(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_497370();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_477DB0 @ 0x00477DB0..0x00477DE2 =====
int __cdecl sub_477DB0(_DWORD *a1)
{
  int v1; // edx
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_497540(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477DF0 @ 0x00477DF0..0x00477E22 =====
int __cdecl sub_477DF0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_4974E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477E30 @ 0x00477E30..0x00477E89 =====
int __cdecl sub_477E30(_DWORD *a1)
{
  int v1; // esi
  void *v2; // edi
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_4975A0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477E90 @ 0x00477E90..0x00477EED =====
int __cdecl sub_477E90(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_497840(v3, v6, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_477EF0 @ 0x00477EF0..0x00477F1E =====
int __cdecl sub_477EF0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4976A0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_477F20 @ 0x00477F20..0x00477F7A =====
int __cdecl sub_477F20(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  sub_4450B0(a1);
  v3 = sub_497710(v5, v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477F80 @ 0x00477F80..0x00477FBE =====
int __cdecl sub_477F80(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_4977E0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_477FC0 @ 0x00477FC0..0x00478022 =====
int __cdecl sub_477FC0(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(int); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = funcs_47800E[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4E7F84, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2((int)a1);
}
