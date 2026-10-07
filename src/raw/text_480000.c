#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_480000 @ 0x00480000..0x00480064 =====
int __cdecl sub_480000(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-8h]
  int v5; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497DB0(v1);
  if ( !sub_463570(v1, v2, v4, v5) )
    sub_4646F0(byte_4EA498, (int)a1);
  return 0;
}

// ===== sub_480070 @ 0x00480070..0x004800F0 =====
int __cdecl sub_480070(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  unsigned int v5; // eax
  unsigned int v6; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_463590(v4, v1, v3, v2);
  if ( v5 > 4 )
  {
    if ( v5 == -1 )
      sub_4646F0(byte_4EA498, (int)a1);
  }
  else
  {
    if ( v5 == 4 )
      sub_4646F0(byte_4EA324, (int)a1);
    v6 = v5 - 1;
    if ( !v6 )
      sub_4646F0(byte_4E8BC0, (int)a1);
    if ( v6 == 2 )
      sub_4646F0(byte_4EA4C0, (int)a1);
  }
  return 0;
}

// ===== sub_480100 @ 0x00480100..0x00480157 =====
int __cdecl sub_480100(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // ecx
  int v4; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v4 = sub_463600(v2, v3, v1);
  switch ( v4 )
  {
    case 1:
      sub_4646F0(byte_4E8BC0, (int)a1);
    case 2:
      sub_4646F0(byte_4EA4F0, (int)a1);
    case -1:
      sub_4646F0(byte_4EA498, (int)a1);
  }
  return 0;
}

// ===== sub_480160 @ 0x00480160..0x00480250 =====
int __cdecl sub_480160(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-208h] BYREF
  char v6[256]; // [esp+10h] [ebp-204h] BYREF
  char Buffer[256]; // [esp+110h] [ebp-104h] BYREF

  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  if ( v1 <= 0 || v5 <= 0 )
  {
    sprintf(v6, &byte_4EA528, v1, v5);
    sub_4646F0(v6, (int)a1);
  }
  while ( !sub_4665C0(0) )
  {
    sprintf(Buffer, &byte_4EA560, v2);
    sub_465A20(Buffer);
  }
  v3 = sub_48F270(&v5, 0);
  sub_4450D0(a1, v3 != 0 ? v5 : 0);
  return 0;
}

// ===== sub_480260 @ 0x00480260..0x00480278 =====
int sub_480260()
{
  sub_48F0E0();
  sub_461A70(0, 0, 1, 0);
  return 0;
}

// ===== sub_480280 @ 0x00480280..0x00480298 =====
int __cdecl sub_480280(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_48F690();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4802A0 @ 0x004802A0..0x004802B4 =====
int __cdecl sub_4802A0(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_48F6F0();
  return 0;
}

// ===== sub_4802C0 @ 0x004802C0..0x00480367 =====
int __cdecl sub_4802C0(int a1)
{
  int v1; // ebx
  _DWORD *v2; // eax
  _DWORD *v3; // eax
  int v5; // [esp+14h] [ebp-14h]
  int v6; // [esp+18h] [ebp-10h]

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v2 = operator new(0x65Cu);
  if ( v2 )
    v3 = sub_452100(v2, v5, v6, v1);
  else
    v3 = 0;
  sub_4451C0(a1, (int)v3);
  return 2;
}

// ===== sub_480370 @ 0x00480370..0x004803B8 =====
int __cdecl sub_480370(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_405CD0(v1);
  if ( v2 )
  {
    if ( v2 == -2147483645 )
    {
      sub_4450D0(a1, 3);
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

// ===== sub_4803C0 @ 0x004803C0..0x004804CE =====
int __cdecl sub_4803C0(_DWORD *a1)
{
  void *v1; // esi
  int v2; // eax
  unsigned int v3; // edx
  int v4; // ecx
  int v5; // edi
  unsigned int v6; // esi
  int v8; // edi
  int v9; // [esp+Ch] [ebp-4h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  sub_4450B0(a1);
  v1 = (void *)sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_48D1A0(v2);
  if ( v5 )
  {
    v6 = sub_405E00(v4, v1, v3);
    sub_48D190();
  }
  else
  {
    v6 = sub_405F10(v1, (int)&savedregs, v4, v3);
  }
  v9 = 0;
  if ( v6 > 0x80000004 )
  {
    if ( v6 == -2147483643 )
    {
      sub_4450D0(a1, 5);
      return 0;
    }
    else
    {
      if ( v6 != -2147483642 )
        goto LABEL_10;
      sub_4450D0(a1, 6);
      return 0;
    }
  }
  else if ( v6 == -2147483644 )
  {
    sub_4450D0(a1, 4);
    return 0;
  }
  else
  {
    if ( v6 )
    {
      if ( v6 == -2147483645 )
      {
        sub_4450D0(a1, 3);
        return 0;
      }
      goto LABEL_10;
    }
    v8 = v5 != 0 ? 2 : 0;
    v6 = 0;
    v9 = v8;
    if ( !v8 )
    {
LABEL_10:
      sub_4450D0(a1, v6);
      return v9;
    }
    return v8;
  }
}

// ===== sub_4804D0 @ 0x004804D0..0x0048053D =====
int __cdecl sub_4804D0(_DWORD *a1)
{
  void *v1; // esi
  _DWORD *v2; // eax
  int v3; // eax

  v1 = (void *)sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_406120(v2, v1);
  if ( v3 )
  {
    if ( v3 == -2147483645 )
    {
      sub_4450D0(a1, 3);
      return 0;
    }
    if ( v3 == -2147483641 )
    {
      sub_4450D0(a1, 7);
      return 0;
    }
  }
  else
  {
    v3 = 0;
  }
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_480540 @ 0x00480540..0x00480548 =====
int sub_480540()
{
  sub_496270();
  return 0;
}

// ===== sub_480550 @ 0x00480550..0x0048057B =====
int __cdecl sub_480550(_DWORD *a1)
{
  sub_4450B0(a1);
  if ( !sub_4962A0() )
    sub_4646F0(byte_4EA590, (int)a1);
  return 0;
}

// ===== sub_480580 @ 0x00480580..0x004805A9 =====
int __cdecl sub_480580(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_496300(v1) )
    sub_4646F0(byte_4EA5D4, (int)a1);
  return 0;
}

// ===== sub_4805B0 @ 0x004805B0..0x004805C8 =====
int __cdecl sub_4805B0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_496350();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4805D0 @ 0x004805D0..0x0048060F =====
int __cdecl sub_4805D0(_DWORD *a1)
{
  int v2; // [esp+8h] [ebp-4h] BYREF

  sub_4450B0(a1);
  if ( !sub_496400(&v2) )
    sub_4646F0(byte_4EA610, (int)a1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_480610 @ 0x00480610..0x00480672 =====
int __cdecl sub_480610(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(int); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = funcs_48065E[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EA638, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2((int)a1);
}

// ===== sub_480680 @ 0x00480680..0x004806D0 =====
int __cdecl sub_480680(_DWORD *a1)
{
  size_t v1; // esi
  void *v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_401ED0(v3, v1, v6, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4806D0 @ 0x004806D0..0x004806F7 =====
int __cdecl sub_4806D0(_DWORD *a1)
{
  _DWORD *v1; // edx
  int v2; // eax

  sub_4450B0(a1);
  v2 = sub_4450B0(v1);
  sub_461E40(v2);
  return 0;
}

// ===== sub_480700 @ 0x00480700..0x00480714 =====
int __cdecl sub_480700(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_48D210();
  return 0;
}

// ===== sub_480720 @ 0x00480720..0x00480743 =====
int __cdecl sub_480720(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax
  _DWORD *v3; // edx

  v1 = sub_4450B0(a1);
  v2 = sub_468F90(v1);
  sub_4450D0(v3, v2);
  return 0;
}

// ===== sub_480750 @ 0x00480750..0x00480764 =====
int __cdecl sub_480750(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_468F80(v1);
  return 0;
}

// ===== sub_480770 @ 0x00480770..0x00480855 =====
int __cdecl sub_480770(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-110h]
  int v5; // [esp+10h] [ebp-10Ch]
  int v6; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v4 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_461D90(v1, v6, v5, v4);
  if ( v2 == -2147483643 )
  {
    sprintf(Buffer, &byte_4EA66C, v6, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 == -2147483642 )
  {
    sprintf(Buffer, &byte_4EA69C, v5, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480860 @ 0x00480860..0x00480967 =====
int __cdecl sub_480860(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_461DB0(v2, v1, v8, v6, v3);
  switch ( v4 )
  {
    case -2147483646:
      sprintf(Buffer, &byte_4E82F0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4E83DC, v1);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, &byte_4E82C4, v7);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480970 @ 0x00480970..0x00480A70 =====
int __cdecl sub_480970(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  __int16 v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  __int16 v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_403DF0(v8, v6, v3, v1);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EA6D0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483643:
      sprintf(Buffer, &byte_4EA740, v7, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480A80 @ 0x00480A80..0x00480B17 =====
int __cdecl sub_480A80(_DWORD *a1)
{
  int v1; // edi
  int v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_403F00(v2);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E8548, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483645 )
  {
    sprintf(Buffer, &byte_4EA6D0, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480B20 @ 0x00480B20..0x00480C27 =====
int __cdecl sub_480B20(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  unsigned int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_404290(v7, v8, v1, v6, v3);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EA6D0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483642:
      sprintf(Buffer, &byte_4EA76C, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480C30 @ 0x00480C30..0x00480CFB =====
int __cdecl sub_480C30(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-10Ch]
  int v7; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4044E0(v7, v6, v1, v3);
  if ( v4 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E8548, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -2147483645 )
  {
    sprintf(Buffer, &byte_4EA6D0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480D00 @ 0x00480D00..0x00480E00 =====
int __cdecl sub_480D00(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4046F0(v8, v6, v3, v1);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EA6D0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483640:
      sprintf(Buffer, &byte_4EA798, v7, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480E10 @ 0x00480E10..0x00480F10 =====
int __cdecl sub_480E10(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4048C0(v8, v6, v3, v1);
  switch ( v4 )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4E8548, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EA6D0, v2);
      sub_4646F0(Buffer, (int)a1);
    case -2147483640:
      sprintf(Buffer, &byte_4EA7D4, v7, v1);
      sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_480F20 @ 0x00480F20..0x00481013 =====
int __cdecl sub_480F20(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-114h]
  unsigned int v7; // [esp+10h] [ebp-110h]
  unsigned int v8; // [esp+14h] [ebp-10Ch]
  int v9; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_404AB0(v8, v9, v6, v7, v1, v3);
  if ( v4 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E8548, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -2147483645 )
  {
    sprintf(Buffer, &byte_4EA6D0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481020 @ 0x00481020..0x004810FF =====
int __cdecl sub_481020(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_404C50(v7, v8, v6, v1, v3);
  if ( v4 == -2147483647 )
  {
    sprintf(Buffer, &byte_4E8548, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == -2147483645 )
  {
    sprintf(Buffer, &byte_4EA6D0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481100 @ 0x00481100..0x0048127C =====
int __cdecl sub_481100(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-128h]
  int v7; // [esp+10h] [ebp-124h]
  int v8; // [esp+14h] [ebp-120h]
  int v9; // [esp+18h] [ebp-11Ch]
  int v10; // [esp+1Ch] [ebp-118h]
  int v11; // [esp+20h] [ebp-114h]
  int v12; // [esp+24h] [ebp-110h]
  int v13; // [esp+28h] [ebp-10Ch]
  int v14; // [esp+2Ch] [ebp-108h]
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  v3 = sub_402F30(v8, v10, v14, v11, v9, v12, v13, v6, v1, v7) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EA810, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E86FC, v14);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E9520, v13, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481280 @ 0x00481280..0x004813FC =====
int __cdecl sub_481280(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-128h]
  int v7; // [esp+10h] [ebp-124h]
  int v8; // [esp+14h] [ebp-120h]
  int v9; // [esp+18h] [ebp-11Ch]
  int v10; // [esp+1Ch] [ebp-118h]
  int v11; // [esp+20h] [ebp-114h]
  int v12; // [esp+24h] [ebp-110h]
  int v13; // [esp+28h] [ebp-10Ch]
  int v14; // [esp+2Ch] [ebp-108h]
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  v3 = sub_402FD0(v8, v10, v14, v11, v9, v12, v13, v6, v1, v7) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EA810, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E86FC, v14);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E9520, v13, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481400 @ 0x00481400..0x0048149B =====
int __cdecl sub_481400(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // edx
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4034F0(v1, v3) - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4E8B34, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4EA848, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4814A0 @ 0x004814A0..0x004815A9 =====
int __cdecl sub_4814A0(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  unsigned int v3; // edx
  int v5; // [esp+Ch] [ebp-110h]
  int v6; // [esp+10h] [ebp-10Ch]
  int v7; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  switch ( sub_403070(v6, v1, v7, v2, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4E8B34, v6);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4EA848, v7);
      break;
    case 4:
      sprintf(Buffer, &byte_4EA878, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    case 5:
      sprintf(Buffer, byte_4EA8A4, v5);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_4815C0 @ 0x004815C0..0x004816D6 =====
int __cdecl sub_4815C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // edx
  int v5; // [esp+Ch] [ebp-10Ch]
  int v6; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  switch ( sub_402B90(v2, v1, v5, v6, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4EA8CC, v5);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sprintf(Buffer, &byte_4EA848, v2);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sprintf(Buffer, &byte_4EA8FC, v2);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, &byte_4EA93C, v6, v1);
      sub_4646F0(Buffer, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_4816F0 @ 0x004816F0..0x004817EF =====
int __cdecl sub_4816F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-110h]
  int v7; // [esp+10h] [ebp-10Ch]
  int v8; // [esp+14h] [ebp-108h]
  char Buffer[256]; // [esp+18h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497DB0(v1);
  v3 = sub_403110(v6, v8, v7, v1) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8B34, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4EA848, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 4 )
  {
    sprintf(Buffer, &byte_4EA96C, v8);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4817F0 @ 0x004817F0..0x004818C4 =====
int __cdecl sub_4817F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // eax
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_403190(v1) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EA994, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4EA848, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4EA9C8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4818D0 @ 0x004818D0..0x0048195F =====
int __cdecl sub_4818D0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_403450(v2, v1) - 1;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8B34, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E8B64, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481960 @ 0x00481960..0x00481996 =====
int __cdecl sub_481960(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_461F30(v2, v1) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_4819A0 @ 0x004819A0..0x004819EF =====
int __cdecl sub_4819A0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  if ( !sub_461F60(v4, v1, v3, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_4819F0 @ 0x004819F0..0x00481A3F =====
int __cdecl sub_4819F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  if ( !sub_461FF0(v4, v1, v3, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_481A40 @ 0x00481A40..0x00481A8F =====
int __cdecl sub_481A40(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  if ( !sub_461FD0(v4, v1, v3, v2) )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_481A90 @ 0x00481A90..0x00481B4A =====
int __cdecl sub_481A90(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // eax
  int v4; // eax
  _DWORD *v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_462030(v1, (int)v6, v2) - 5;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8CE0, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = v3 - 249;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4EAA10, v1, *v6, *v6);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_481B50 @ 0x00481B50..0x00481B85 =====
int __cdecl sub_481B50(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  if ( sub_4620F0(v2, v1) == 255 )
    sub_4646F0(byte_4E8BC0, (int)a1);
  return 0;
}

// ===== sub_481B90 @ 0x00481B90..0x00481C14 =====
int __cdecl sub_481B90(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_462100(v4, v1, v3, v2) )
  {
    case 6:
      sub_4646F0(byte_4EAA94, (int)a1);
    case 7:
      sub_4646F0(byte_4EAAC8, (int)a1);
    case 8:
      sub_4646F0(byte_4EAB08, (int)a1);
    case 255:
      sub_4646F0(byte_4EAA60, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_481D30 @ 0x00481D30..0x00481D89 =====
int __cdecl sub_481D30(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462120(v2, v1);
  switch ( v3 )
  {
    case 6:
      sub_4646F0(byte_4EAA94, (int)a1);
    case 9:
      sub_4646F0(byte_4EAB40, (int)a1);
    case 255:
      sub_4646F0(byte_4EAA60, (int)a1);
  }
  return 0;
}

// ===== sub_481D90 @ 0x00481D90..0x00481EBB =====
int __cdecl sub_481D90(_DWORD *a1)
{
  int v1; // ebx
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-120h]
  int v6; // [esp+10h] [ebp-11Ch]
  int v7; // [esp+14h] [ebp-118h]
  int v8; // [esp+18h] [ebp-114h]
  int v9; // [esp+1Ch] [ebp-110h]
  int v10; // [esp+20h] [ebp-10Ch]
  int v11; // [esp+24h] [ebp-108h]
  char Buffer[256]; // [esp+28h] [ebp-104h] BYREF

  v7 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  sub_497B60(v2);
  v3 = sub_462250(v7, v1, v9, v6, v2, v10, v5, v8, v11) - 3;
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4E8DB4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4E9520, v11, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481EC0 @ 0x00481EC0..0x00481F28 =====
int __cdecl sub_481EC0(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax
  int v3; // edx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_462280(v1) - 1;
  if ( !v2 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v2 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v3);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481F30 @ 0x00481F30..0x00481FA7 =====
int __cdecl sub_481F30(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462290(v2, v1) - 1;
  if ( !v3 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_481FB0 @ 0x00481FB0..0x00482032 =====
int __cdecl sub_481FB0(_DWORD *a1)
{
  unsigned int v1; // ebx
  unsigned int v2; // edi
  unsigned int v3; // edx
  int v4; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4622A0(v3, v1, v2) - 1;
  if ( !v4 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_482040 @ 0x00482040..0x004820CC =====
int __cdecl sub_482040(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  unsigned int v4; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497C40(v1);
  v2 = sub_4622B0(v1, v4) - 1;
  if ( !v2 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v2 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4820D0 @ 0x004820D0..0x0048215C =====
int __cdecl sub_4820D0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  unsigned int v4; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497DB0(v1);
  v2 = sub_4622C0(v1, v4) - 1;
  if ( !v2 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v2 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v4);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_482160 @ 0x00482160..0x00482229 =====
int __cdecl sub_482160(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // edi
  int v3; // eax
  int v4; // eax
  int v6; // [esp+Ch] [ebp-10Ch]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v6 = sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_497B60(v1);
  v3 = sub_4622D0(v6, v2) - 1;
  if ( !v3 )
    sub_4646F0(byte_4EAB74, (int)a1);
  v4 = v3 - 1;
  if ( !v4 )
  {
    sprintf(Buffer, &byte_4EABA8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4E8DB4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_482230 @ 0x00482230..0x00482304 =====
int __cdecl sub_482230(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // edx
  int v4; // eax
  int v5; // eax
  unsigned int v7; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_4622E0(v3, v7, v2) - 1;
  if ( !v4 )
    sub_4646F0(byte_4EAB74, (int)a1);
  v5 = v4 - 1;
  if ( !v5 )
  {
    sprintf(Buffer, &byte_4EABA8, v7);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v5 == 2 )
  {
    sprintf(Buffer, &byte_4E9520, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_482310 @ 0x00482310..0x00482396 =====
int __cdecl sub_482310(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462300(v2, v1) - 1;
  if ( !v3 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4823A0 @ 0x004823A0..0x00482426 =====
int __cdecl sub_4823A0(_DWORD *a1)
{
  unsigned int v1; // edi
  int v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462310(v2, v1) - 1;
  if ( !v3 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v3 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_482430 @ 0x00482430..0x004824C9 =====
int __cdecl sub_482430(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // esi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_462320(v3, v1, v2, v6) - 1;
  if ( !v4 )
    sub_4646F0(byte_4EAB74, (int)a1);
  if ( v4 == 1 )
  {
    sprintf(Buffer, &byte_4EABA8, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_4824D0 @ 0x004824D0..0x00482502 =====
int __cdecl sub_4824D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_462530(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_482510 @ 0x00482510..0x0048253A =====
int __cdecl sub_482510(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4625B0();
  if ( !v1 )
    sub_4646F0(byte_4EABD0, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_482540 @ 0x00482540..0x00482567 =====
int __cdecl sub_482540(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_4625C0(v1) )
    sub_4646F0(byte_4EAC0C, (int)a1);
  return 0;
}

// ===== sub_482570 @ 0x00482570..0x004825A6 =====
int __cdecl sub_482570(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_462670(v2, v1) )
    sub_4646F0(byte_4EAC0C, (int)a1);
  return 0;
}

// ===== sub_4825B0 @ 0x004825B0..0x004826DA =====
int __cdecl sub_4825B0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  const char *v4; // [esp-8h] [ebp-128h]
  int v5; // [esp-4h] [ebp-124h]
  int v6; // [esp+Ch] [ebp-114h]
  int v7; // [esp+10h] [ebp-110h]
  int v8; // [esp+14h] [ebp-10Ch]
  int v9; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  switch ( sub_4625D0(v1, v7, v8, v9, v6, v2) )
  {
    case 1:
      v5 = v9;
      v4 = &byte_4E8798;
      goto LABEL_4;
    case 2:
      v5 = v6;
      v4 = byte_4E8870;
LABEL_4:
      sprintf(Buffer, v4, v5);
      break;
    case 3:
      sprintf(Buffer, &byte_4EAC38, v9);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sprintf(Buffer, byte_4EACC8, v6);
      break;
    case 255:
      sub_4646F0(byte_4EAC0C, (int)a1);
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_482800 @ 0x00482800..0x004828BC =====
int __cdecl sub_482800(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-10Ch]
  unsigned int v6; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_4625F0(v1, v2, v5, v6);
  if ( v3 == 5 )
  {
    sprintf(Buffer, &byte_4EAD54, v6);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4EAC0C, (int)a1);
  return 0;
}

// ===== sub_4828C0 @ 0x004828C0..0x00482A35 =====
int __cdecl sub_4828C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-114h]
  int v5; // [esp+10h] [ebp-110h]
  unsigned int v6; // [esp+14h] [ebp-10Ch]
  int v7; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  switch ( sub_462610(v1, v2, v4, v7, v5, v6) )
  {
    case 1:
      sprintf(Buffer, &byte_4E95C0, v7);
      break;
    case 3:
      sprintf(Buffer, &byte_4EAD88, v7);
      sub_4646F0(Buffer, (int)a1);
    case 6:
      sprintf(Buffer, &byte_4E9690, v5);
      sub_4646F0(Buffer, (int)a1);
    case 7:
      sprintf(Buffer, byte_4E91E4, v6);
      break;
    case 8:
      sprintf(Buffer, &byte_4E9210, v6, v7);
      break;
    case 255:
      sub_4646F0(byte_4EAC0C, (int)a1);
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_482B60 @ 0x00482B60..0x00482C87 =====
int __cdecl sub_482B60(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v5; // [esp+Ch] [ebp-120h]
  int v6; // [esp+10h] [ebp-11Ch]
  int v7; // [esp+14h] [ebp-118h]
  int v8; // [esp+18h] [ebp-114h]
  int v9; // [esp+1Ch] [ebp-110h]
  int v10; // [esp+20h] [ebp-10Ch]
  int v11; // [esp+24h] [ebp-108h]
  char Buffer[256]; // [esp+28h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  v3 = sub_462630(v1, v2, v7, v8, v6, v9, v11, v10, v5);
  if ( v3 == 9 )
  {
    sprintf(Buffer, &byte_4E9520, v11, v10);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == 255 )
    sub_4646F0(byte_4EAC0C, (int)a1);
  return 0;
}

// ===== sub_482C90 @ 0x00482C90..0x00482CED =====
int __cdecl sub_482C90(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  int v4; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_4450B0(a1);
  sub_497BB0(v1);
  sub_497DB0(v2);
  if ( sub_462660(v1, v2, v4) == 255 )
    sub_4646F0(byte_4EAC0C, (int)a1);
  return 0;
}

// ===== sub_482CF0 @ 0x00482CF0..0x00482D69 =====
int __cdecl sub_482CF0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  unsigned int v3; // eax
  int v4; // edx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-8h]
  int v8; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  v5 = sub_462740(v4, v1, v3, v7, v8, v2);
  if ( !v5 )
    sub_4646F0(byte_4EADFC, (int)a1);
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_482D70 @ 0x00482D70..0x00482D97 =====
int __cdecl sub_482D70(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  if ( !sub_462760(v1) )
    sub_4646F0(byte_4EAE3C, (int)a1);
  return 0;
}

// ===== sub_482DA0 @ 0x00482DA0..0x00482E05 =====
int __cdecl sub_482DA0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = (int *)sub_48DF50(a1);
  v4 = sub_462770(v2, v3, v1);
  if ( v4 == 13 )
    sub_4646F0(byte_4EAE70, (int)a1);
  if ( v4 == 255 )
    sub_4646F0(byte_4EAE3C, (int)a1);
  sub_4450D0(a1, v4 == 0);
  return 0;
}

// ===== sub_482E10 @ 0x00482E10..0x00482E44 =====
int __cdecl sub_482E10(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_4627E0(v2, v1) )
    sub_4646F0(byte_4EAE3C, (int)a1);
  return 0;
}

// ===== sub_482E50 @ 0x00482E50..0x00482EE2 =====
int __cdecl sub_482E50(_DWORD *a1)
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
  if ( sub_4627F0(v1, v2, v4, v5, v6, v7) == 255 )
    sub_4646F0(byte_4EAE3C, (int)a1);
  return 0;
}

// ===== sub_482EF0 @ 0x00482EF0..0x00483008 =====
int __cdecl sub_482EF0(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  int v4; // edx
  const char *v6; // [esp-8h] [ebp-120h]
  unsigned int v7; // [esp-4h] [ebp-11Ch]
  unsigned int v8; // [esp+Ch] [ebp-10Ch]
  unsigned int v9; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_462810(v4, v1, v3, v8, v9, v2) )
  {
    case 11:
      v7 = v8;
      v6 = &byte_4EAEB4;
      goto LABEL_4;
    case 12:
      sprintf(Buffer, &byte_4EAEE0, v9);
      sub_4646F0(Buffer, (int)a1);
    case 13:
      sub_4646F0(byte_4EAE70, (int)a1);
    case 18:
      v7 = v2;
      v6 = (const char *)&unk_4EAF0C;
LABEL_4:
      sprintf(Buffer, v6, v7);
      break;
    case 19:
      sprintf(Buffer, byte_4EAF3C, v1);
      break;
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_483120 @ 0x00483120..0x00483276 =====
int __cdecl sub_483120(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // edx
  int v5; // [esp+Ch] [ebp-118h]
  unsigned int *v6; // [esp+10h] [ebp-114h]
  unsigned int v7; // [esp+14h] [ebp-110h]
  unsigned int v8; // [esp+18h] [ebp-10Ch]
  int v9; // [esp+1Ch] [ebp-108h] BYREF
  char Buffer[256]; // [esp+20h] [ebp-104h] BYREF

  v6 = (unsigned int *)sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  sub_48DF50(a1);
  v8 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v9 = sub_462830(v6, v7, v2, v1, v8, v3, v5);
  switch ( v9 )
  {
    case 1:
      sprintf(Buffer, &byte_4E799C, v1);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sub_462030(-1, (int)&v9, v2);
      sprintf(Buffer, &byte_4EAF6C, v9);
      sub_4646F0(Buffer, (int)a1);
    case 3:
      sub_462030(-1, (int)&v9, v2);
      sprintf(Buffer, &byte_4EAF9C, v9);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_483390 @ 0x00483390..0x00483447 =====
int __cdecl sub_483390(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  _DWORD *v4; // edx
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_462850(v4, v1, v3, v2) )
  {
    case 4:
      sub_4646F0(byte_4EAFCC, (int)a1);
    case 5:
      sprintf(Buffer, &byte_4EB00C, v2, v1);
      sub_4646F0(Buffer, (int)a1);
    case 6:
      sub_4646F0(byte_4EB03C, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_483560 @ 0x00483560..0x0048363A =====
int __cdecl sub_483560(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // edx
  unsigned int v5; // [esp+Ch] [ebp-10Ch]
  int v6; // [esp+10h] [ebp-108h] BYREF
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_462870(v3, v5, v2, v1);
  switch ( v6 )
  {
    case 1:
      sprintf(Buffer, &byte_4E799C, v1);
      sub_4646F0(Buffer, (int)a1);
    case 2:
      sub_462030(-1, (int)&v6, v2);
      sprintf(Buffer, &byte_4EAF6C, v6);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
  }
  return 0;
}

// ===== sub_483640 @ 0x00483640..0x00483770 =====
int __cdecl sub_483640(_DWORD *a1)
{
  int v1; // edi
  unsigned int v2; // ebx
  int v4; // [esp+Ch] [ebp-114h]
  int v5; // [esp+10h] [ebp-110h]
  unsigned int v6; // [esp+14h] [ebp-10Ch]
  unsigned int v7; // [esp+18h] [ebp-108h]
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  sub_497DB0(v1);
  switch ( sub_462890(v1, v2, v5, v6, v4, v7) )
  {
    case 11:
      sub_4646F0(byte_4EB060, (int)a1);
    case 12:
      sub_4646F0(byte_4EB0B0, (int)a1);
    case 13:
      sub_4646F0(byte_4EAE70, (int)a1);
    case 16:
      sprintf(Buffer, &byte_4EB100, v7);
      sub_4646F0(Buffer, (int)a1);
    case 17:
      sprintf(Buffer, &byte_4EB12C, v2);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_483890 @ 0x00483890..0x00483976 =====
int __cdecl sub_483890(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_4628B0(v1, v2, v3) )
  {
    case 1:
      sprintf(Buffer, &byte_4E799C, v1);
      sub_4646F0(Buffer, (int)a1);
    case 4:
      sub_4646F0(byte_4EAFCC, (int)a1);
    case 8:
      sprintf(Buffer, &byte_4EB158, v2);
      sub_4646F0(Buffer, (int)a1);
    case 9:
      sprintf(Buffer, &byte_4EB180, v1, v2);
      sub_4646F0(Buffer, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_483A90 @ 0x00483A90..0x00483B8D =====
int __cdecl sub_483A90(_DWORD *a1)
{
  unsigned int v1; // edi
  unsigned int v2; // ebx
  int v3; // eax
  unsigned int v4; // edx
  int v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_4628C0(v1, v2, v3, v4) )
  {
    case 10:
      sprintf(Buffer, &byte_4EB1DC, v1);
      sub_4646F0(Buffer, (int)a1);
    case 11:
      sprintf(Buffer, &byte_4EAEB4, v6);
      sub_4646F0(Buffer, (int)a1);
    case 12:
      sprintf(Buffer, &byte_4EAEE0, v2);
      sub_4646F0(Buffer, (int)a1);
    case 13:
      sub_4646F0(byte_4EAE70, (int)a1);
    case 255:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      return 0;
  }
}

// ===== sub_483CA0 @ 0x00483CA0..0x00483D87 =====
int __cdecl sub_483CA0(_DWORD *a1)
{
  unsigned int v1; // esi
  unsigned int v2; // ebx
  _DWORD *v3; // eax
  unsigned int v4; // eax
  int v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_4628E0(v1, v2, v3, v6);
  switch ( v4 )
  {
    case 0xBu:
      sprintf(Buffer, &byte_4EAEB4, v2);
      sub_4646F0(Buffer, (int)a1);
    case 0xCu:
      sprintf(Buffer, &byte_4EAEE0, v1);
      sub_4646F0(Buffer, (int)a1);
    case 0xDu:
      sub_4646F0(byte_4EAE70, (int)a1);
    case 0xFFu:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      sub_4450D0(a1, v4 == 0);
      return 0;
  }
}

// ===== sub_483EA0 @ 0x00483EA0..0x00483F8E =====
int __cdecl sub_483EA0(_DWORD *a1)
{
  unsigned int v1; // esi
  unsigned int v2; // ebx
  int v3; // edx
  unsigned int v4; // eax
  int v6; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v4 = sub_462900(v1, v2, v6, v3);
  switch ( v4 )
  {
    case 0xBu:
      sprintf(Buffer, &byte_4EAEB4, v2);
      break;
    case 0xCu:
      sprintf(Buffer, &byte_4EAEE0, v1);
      sub_4646F0(Buffer, (int)a1);
    case 0xDu:
      sub_4646F0(byte_4EAE70, (int)a1);
    case 0xFu:
      sprintf(Buffer, &byte_4E8548, v6);
      break;
    case 0xFFu:
      sub_4646F0(byte_4EAE3C, (int)a1);
    default:
      sub_4450D0(a1, v4 == 0);
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}
