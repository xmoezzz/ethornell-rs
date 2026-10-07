#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_440000 @ 0x00440000..0x004400B1 =====
int __userpurge sub_440000@<eax>(
        int a1@<eax>,
        int a2,
        unsigned int a3,
        int a4,
        int a5,
        unsigned int a6,
        unsigned int *a7)
{
  _DWORD *v7; // edi
  int v8; // esi
  unsigned int v9; // eax

  v7 = dword_56674C;
  v8 = sub_43FD30(a1, (int)dword_56674C);
  if ( !v8 )
    return 255;
  v9 = sub_421930(v8, a2, a3, a4, a5, a6, a7);
  if ( v9 > 0x80000002 )
  {
    if ( v9 != -2147483645 )
      return 254;
    return 3;
  }
  else
  {
    if ( v9 == -2147483646 )
      return 2;
    if ( v9 )
    {
      if ( v9 == -2147483647 )
        return 1;
      return 254;
    }
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 8))(v8) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 12))(v8);
    sub_4307D0(v7[5], v8);
    return 0;
  }
}

// ===== sub_4400C0 @ 0x004400C0..0x0044016A =====
int __userpurge sub_4400C0@<eax>(int a1@<eax>, unsigned int a2, unsigned int a3, _DWORD *a4)
{
  _DWORD *v4; // ebx
  _DWORD *v5; // esi
  unsigned int v6; // eax

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_43FD30(a1, (int)dword_56674C);
  if ( !v5 )
    return 255;
  v6 = sub_421E60(a2, v5, a3, a4);
  if ( v6 > 0x80000005 )
  {
    if ( v6 != -2147483642 )
      return 254;
    return 6;
  }
  else
  {
    if ( v6 == -2147483643 )
      return 5;
    if ( v6 )
    {
      if ( v6 == -2147483644 )
        return 4;
      return 254;
    }
    if ( (*(int (__thiscall **)(_DWORD *))(*v5 + 8))(v5) )
      (*(void (__thiscall **)(_DWORD *))(*v5 + 12))(v5);
    sub_4307D0(v4[5], (int)v5);
    return 0;
  }
}

// ===== sub_440170 @ 0x00440170..0x004401CF =====
int __stdcall sub_440170(int a1, int a2, unsigned int a3, int a4)
{
  int v4; // eax
  int v5; // eax

  v4 = sub_43FD30(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_4224D0(v4, a2, a3, a4);
  if ( !v5 )
    return 0;
  if ( v5 == -2147483647 )
    return 1;
  return v5 != -2147483646 ? 254 : 2;
}

// ===== sub_4401D0 @ 0x004401D0..0x004402B4 =====
int __stdcall sub_4401D0(int a1, unsigned int a2, int a3, unsigned int a4, unsigned int a5, int a6)
{
  int v7; // edi
  _DWORD *v8; // esi
  int v9; // eax
  int result; // eax
  int v11; // [esp+1Ch] [ebp+10h]

  v7 = 0;
  v11 = 0;
  v8 = (_DWORD *)sub_43FD30(a1, (int)dword_56674C);
  if ( !v8 )
    return 255;
  if ( !a2 )
    return v11;
  while ( 1 )
  {
    v9 = sub_422680(*(_DWORD *)(a3 + 8 * v7), a5, v8, *(_DWORD *)(a3 + 8 * v7 + 4), a4, a6);
    if ( v9 )
      break;
    if ( ++v7 >= a2 )
      return 0;
  }
  switch ( v9 )
  {
    case -2147483637:
      result = 11;
      break;
    case -2147483636:
      result = 12;
      break;
    case -2147483635:
      result = 13;
      break;
    case -2147483632:
      result = 16;
      break;
    case -2147483631:
      result = 17;
      break;
    default:
      result = 254;
      break;
  }
  return result;
}

// ===== sub_4402D0 @ 0x004402D0..0x00440353 =====
int __stdcall sub_4402D0(int a1, unsigned int a2, unsigned int a3)
{
  _DWORD *v3; // eax
  unsigned int v4; // eax

  v3 = (_DWORD *)sub_43FD30(a1, (int)dword_56674C);
  if ( !v3 )
    return 255;
  v4 = sub_422050(a3, v3, a2);
  if ( v4 <= 0x80000004 )
  {
    switch ( v4 )
    {
      case 0x80000004:
        return 4;
      case 0u:
        return 0;
      case 0x80000001:
        return 1;
    }
    return 254;
  }
  if ( v4 == -2147483640 )
    return 8;
  if ( v4 != -2147483639 )
    return 254;
  return 9;
}

// ===== sub_440360 @ 0x00440360..0x004403FC =====
int __userpurge sub_440360@<eax>(int a1@<eax>, unsigned int a2, unsigned int a3, unsigned int a4)
{
  _DWORD *v4; // edi
  _DWORD *v5; // esi
  unsigned int v6; // eax

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_43FD30(a1, (int)dword_56674C);
  if ( !v5 )
    return 255;
  v6 = sub_4222A0(v5, a2, a3, a4);
  if ( v6 <= 0x8000000B )
  {
    switch ( v6 )
    {
      case 0x8000000B:
        return 11;
      case 0u:
        sub_4307D0(v4[5], (int)v5);
        return 0;
      case 0x8000000A:
        return 10;
    }
    return 254;
  }
  if ( v6 == -2147483636 )
    return 12;
  if ( v6 != -2147483635 )
    return 254;
  return 13;
}

// ===== sub_440400 @ 0x00440400..0x0044047F =====
unsigned int __stdcall sub_440400(_DWORD *a1, int a2, unsigned int a3, unsigned int a4)
{
  _DWORD *v4; // eax
  unsigned int result; // eax

  v4 = (_DWORD *)sub_43FD30(a2, (int)dword_56674C);
  if ( !v4 )
    return 255;
  result = sub_4228B0(v4, a4, a1, a3);
  if ( result > 0xE )
  {
    switch ( result )
    {
      case 0x8000000B:
        return 11;
      case 0x8000000C:
        return 12;
      case 0x8000000D:
        return 13;
    }
    return 254;
  }
  if ( result == 14 )
    return 14;
  if ( result )
    return 254;
  return result;
}

// ===== sub_440480 @ 0x00440480..0x00440506 =====
unsigned int __stdcall sub_440480(int a1, int a2, unsigned int a3, unsigned int a4)
{
  _DWORD *v4; // edx
  unsigned int result; // eax

  v4 = (_DWORD *)sub_43FD30(a2, (int)dword_56674C);
  if ( !v4 )
    return 255;
  result = sub_4227C0(a4, v4, a1, a3);
  if ( result > 0xE )
  {
    switch ( result )
    {
      case 0x8000000B:
        result = 11;
        break;
      case 0x8000000C:
        result = 12;
        break;
      case 0x8000000D:
        result = 13;
        break;
      case 0x8000000F:
        result = 15;
        break;
      default:
        return 254;
    }
  }
  else if ( result == 14 )
  {
    return 14;
  }
  else if ( result )
  {
    return 254;
  }
  return result;
}

// ===== sub_440520 @ 0x00440520..0x00440562 =====
_DWORD *__stdcall sub_440520(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 573;
  v2 = 16;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  result = a1;
  a1[589] = 0;
  a1[590] = 0;
  return result;
}

// ===== sub_440570 @ 0x00440570..0x004405AA =====
int __userpurge sub_440570@<eax>(int a1@<eax>, int a2)
{
  int result; // eax
  int v3; // ecx
  _DWORD *v4; // esi
  int v5; // edi

  sub_42B0D0(a1);
  result = sub_42B0E0(a2);
  v4 = (_DWORD *)(v3 + 2292);
  v5 = 16;
  do
  {
    if ( *v4 )
      result = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)*v4 + 12))(*v4);
    ++v4;
    --v5;
  }
  while ( v5 );
  return result;
}

// ===== sub_4405B0 @ 0x004405B0..0x004406BD =====
int __stdcall sub_4405B0(_DWORD *a1, unsigned int a2, unsigned int a3)
{
  _DWORD *v3; // esi
  _DWORD *v4; // edi
  _DWORD *v5; // eax
  int i; // ebx
  _DWORD *v7; // eax
  char *v8; // ecx
  int v9; // eax

  v3 = dword_56674C;
  if ( *((int *)dword_56674C + 589) >= 16 )
    return 9;
  v4 = 0;
  v5 = (char *)dword_56674C + 2292;
  for ( i = 0; *v5; ++i )
    ++v5;
  v7 = operator new(0x3CCu);
  if ( v7 )
  {
    v8 = (char *)v3[590];
    v3[590] = v8 + 1;
    v4 = sub_42AE20(v8, v7);
  }
  if ( sub_42B1A0(a2, v4, a3) )
  {
    v3[i + 573] = v4;
    v9 = v3[5];
    ++v3[589];
    sub_4306F0(v9, (int)v4);
    *a1 = i - 1342177280;
    return 0;
  }
  else
  {
    if ( v4 )
      (*(void (__thiscall **)(_DWORD *, int))*v4)(v4, 1);
    return 10;
  }
}

// ===== sub_4406C0 @ 0x004406C0..0x004406F3 =====
int __userpurge sub_4406C0@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xB0000000 && (v3 = sub_443260(0), v3 < 0x10) )
    return *(_DWORD *)(a2 + 4 * v3 + 2292);
  else
    return v2;
}

// ===== sub_440700 @ 0x00440700..0x00440778 =====
BOOL __stdcall sub_440700(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 573];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 573] = 0;
    --v1[589];
  }
  return v2 != 0;
}

// ===== sub_440780 @ 0x00440780..0x00440805 =====
int __stdcall sub_440780(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int result; // eax

  v2 = sub_4406C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return 255;
  switch ( sub_42B2A0(v2, a2) )
  {
    case 0:
      if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
        (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
      result = 0;
      break;
    case 1:
      result = 1;
      break;
    case 2:
      result = 2;
      break;
    case 3:
      result = 3;
      break;
    case 8:
      result = 16;
      break;
    default:
      result = a1;
      break;
  }
  return result;
}

// ===== sub_440830 @ 0x00440830..0x0044089F =====
BOOL __userpurge sub_440830@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *v6; // edi
  int v7; // esi

  v6 = dword_56674C;
  v7 = sub_4406C0(a1, (int)dword_56674C);
  if ( v7 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
    sub_42B360(a2, a3, v7, a4, a5, a6);
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
    sub_4307D0(v6[5], v7);
  }
  return v7 != 0;
}

// ===== sub_4408A0 @ 0x004408A0..0x004408CE =====
BOOL __stdcall sub_4408A0(int a1, int a2)
{
  int v2; // esi

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
    sub_42B3A0(a2, v2);
  return v2 != 0;
}

// ===== sub_4408D0 @ 0x004408D0..0x00440910 =====
int __stdcall sub_4408D0(int a1, unsigned int a2, unsigned int a3, int a4, int a5)
{
  _DWORD *v5; // eax

  v5 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
  if ( v5 )
    return sub_42B970(a3, a2, v5, a4, a5) != 0 ? 0 : 4;
  else
    return 255;
}

// ===== sub_440910 @ 0x00440910..0x0044093E =====
int __stdcall sub_440910(_DWORD *a1, int a2)
{
  _DWORD *v2; // ecx

  v2 = (_DWORD *)sub_4406C0(a2, (int)dword_56674C);
  if ( !v2 )
    return 255;
  sub_42C2A0(a1, v2);
  return 0;
}

// ===== sub_440940 @ 0x00440940..0x00440974 =====
int __stdcall sub_440940(int a1, unsigned int a2)
{
  int v2; // eax

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
    return sub_42C550(v2, a2) != 0 ? 0 : 18;
  else
    return 255;
}

// ===== sub_440980 @ 0x00440980..0x004409B5 =====
int __stdcall sub_440980(int a1, unsigned int a2)
{
  int v2; // eax
  int v3; // edx

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
    return sub_42C580(a2, v3, v2) != 0 ? 0 : 19;
  else
    return 255;
}

// ===== sub_4409C0 @ 0x004409C0..0x00440A4D =====
int __stdcall sub_4409C0(int a1, const CHAR *a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // eax
  _DWORD *v8; // edi
  int v9; // eax
  unsigned int v10; // eax

  v7 = sub_4406C0(a1, (int)dword_56674C);
  v8 = (_DWORD *)v7;
  if ( !v7 )
    return 255;
  v9 = sub_42C510(v7, a6);
  sub_42C530(v9, a7);
  v10 = sub_42C3B0(a3, a2, v8, a4, a5);
  if ( v10 > 0x80000003 )
  {
    if ( v10 == -2147483644 )
      return 7;
  }
  else
  {
    switch ( v10 )
    {
      case 0x80000003:
        return 6;
      case 0u:
        return 0;
      case 0x80000002:
        return 5;
    }
  }
  return a1;
}

// ===== sub_440A50 @ 0x00440A50..0x00440A86 =====
int __stdcall sub_440A50(int a1, unsigned int a2)
{
  int v2; // eax

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
    return sub_42C470(a2, v2) != 0 ? 0 : 8;
  else
    return 255;
}

// ===== sub_440A90 @ 0x00440A90..0x00440AF8 =====
BOOL __stdcall sub_440A90(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_4406C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( v4 )
  {
    if ( v5 )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( v5 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return v3 != 0;
}

// ===== sub_440B00 @ 0x00440B00..0x00440B44 =====
BOOL __stdcall sub_440B00(int a1, int a2)
{
  int v2; // esi

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( v2 )
  {
    sub_42B3B0(a2, v2);
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
  }
  return v2 != 0;
}

// ===== sub_440B50 @ 0x00440B50..0x00440B7E =====
int __stdcall sub_440B50(int a1, int a2)
{
  int v2; // ecx

  v2 = sub_4406C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  sub_42B3C0(a2, v2);
  return 0;
}

// ===== sub_440B80 @ 0x00440B80..0x00440C54 =====
int __userpurge sub_440B80@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  void *v6; // edi
  _DWORD *v7; // esi
  int result; // eax
  int v9; // [esp+Ch] [ebp-14h]
  int v10[4]; // [esp+10h] [ebp-10h] BYREF

  v6 = dword_56674C;
  v7 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  switch ( sub_42B400(a3, a4, v7, v10, a2, a5, a6) )
  {
    case 0:
      if ( (*(int (__thiscall **)(_DWORD *))(*v7 + 8))(v7) )
      {
        (*(void (__thiscall **)(_DWORD *))(*v7 + 28))(v7);
        sub_443240(v6);
      }
      result = 0;
      break;
    case 2:
      result = 2;
      break;
    case 4:
      result = 12;
      break;
    case 5:
      result = 13;
      break;
    case 6:
      result = 14;
      break;
    case 7:
      result = 15;
      break;
    default:
      result = v9;
      break;
  }
  return result;
}

// ===== sub_440C80 @ 0x00440C80..0x00440D51 =====
int __thiscall sub_440C80(void *this, int a2)
{
  int v3; // eax
  _DWORD *v4; // edi
  unsigned int v5; // eax
  size_t v7[6]; // [esp+Ch] [ebp-2Ch] BYREF
  _DWORD v8[4]; // [esp+24h] [ebp-14h] BYREF

  v3 = sub_4406C0(a2, (int)dword_56674C);
  v4 = (_DWORD *)v3;
  if ( !v3 )
    return 255;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v3 + 32))(v3, v8);
  v5 = sub_407B10();
  if ( v5 == 1 )
    v5 = 2;
  if ( !sub_407DA0((int)this, (_DWORD *)dword_565D5C, v8[2] - v8[0] + 1, v8[3] - v8[1] + 1, v5) )
    return 2;
  sub_407F20(dword_565D5C, (int)this, v7);
  sub_42C940(v7, v4);
  return 0;
}

// ===== sub_440D60 @ 0x00440D60..0x00440DBC =====
int __stdcall sub_440D60(int a1, int a2)
{
  _DWORD *v2; // eax
  int v3; // esi

  v2 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
  v3 = (int)v2;
  if ( !v2 )
    return 255;
  if ( !sub_42C870(v2, a2) )
    return 20;
  sub_42CA00(v3);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return 0;
}

// ===== sub_440DC0 @ 0x00440DC0..0x00440E07 =====
BOOL __stdcall sub_440DC0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi

  v2 = sub_4406C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( v2 )
  {
    sub_42B540(v2, a2);
    sub_42CA00(v3);
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  }
  return v3 != 0;
}

// ===== sub_440E10 @ 0x00440E10..0x00440EE5 =====
int __userpurge sub_440E10@<eax>(int a1@<eax>, int a2, int a3, void *a4, int a5, int a6)
{
  void *v6; // edi
  int v7; // esi
  int result; // eax
  int v9; // [esp+Ch] [ebp-14h]
  _BYTE v10[16]; // [esp+10h] [ebp-10h] BYREF

  v6 = dword_56674C;
  v7 = sub_4406C0(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  switch ( sub_42B560(a4, v7, (int)v10, a2, a3, a5, a6) )
  {
    case 0:
      if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
      {
        (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 28))(v7);
        sub_443240(v6);
      }
      result = 0;
      break;
    case 2:
      result = 2;
      break;
    case 4:
      result = 12;
      break;
    case 5:
      result = 13;
      break;
    case 6:
      result = 14;
      break;
    case 7:
      result = 15;
      break;
    default:
      result = v9;
      break;
  }
  return result;
}

// ===== sub_440F10 @ 0x00440F10..0x00440F79 =====
int __stdcall sub_440F10(int a1, const char *a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // esi

  v7 = sub_4406C0(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  if ( !sub_42B710(a2, v7, a3, a4, a5, a6, a7) )
    return 17;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  return 0;
}

// ===== sub_440F80 @ 0x00440F80..0x00440FDB =====
BOOL __stdcall sub_440F80(int a1)
{
  _DWORD *v1; // esi
  int v2; // edx

  v1 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
  if ( v1 )
  {
    sub_42C5B0(v1);
    sub_42B9B0((int)v1);
    sub_42BBB0((int)v1);
    sub_42BBD0(0, v2, v1);
    if ( (*(int (__thiscall **)(_DWORD *))(*v1 + 8))(v1) )
      (*(void (__thiscall **)(_DWORD *))(*v1 + 12))(v1);
  }
  return v1 != 0;
}

// ===== sub_440FE0 @ 0x00440FE0..0x0044100E =====
BOOL __stdcall sub_440FE0(int a1, int a2, int a3)
{
  int v3; // eax

  v3 = sub_4406C0(a1, (int)dword_56674C);
  if ( v3 )
    v3 = sub_42C700(v3, a2, a3);
  return v3 != 0;
}

// ===== sub_441010 @ 0x00441010..0x0044103E =====
BOOL __stdcall sub_441010(_DWORD *a1, int a2)
{
  int v2; // esi

  v2 = sub_4406C0(a2, (int)dword_56674C);
  if ( v2 )
    sub_42C720(a1, v2);
  return v2 != 0;
}

// ===== sub_441040 @ 0x00441040..0x00441072 =====
int __stdcall sub_441040(BOOL *a1, int a2)
{
  _DWORD *v2; // eax

  v2 = (_DWORD *)sub_4406C0(a2, (int)dword_56674C);
  if ( !v2 )
    return 255;
  *a1 = sub_42C740(v2);
  return 0;
}

// ===== sub_441080 @ 0x00441080..0x004410AE =====
BOOL __stdcall sub_441080(int a1, int a2)
{
  int v2; // eax
  int v3; // esi

  v2 = sub_4406C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( v2 )
    sub_42C7A0(v2, a2);
  return v3 != 0;
}

// ===== sub_4410B0 @ 0x004410B0..0x004410D7 =====
int sub_4410B0()
{
  int *v0; // esi
  int v1; // edi
  int result; // eax

  v0 = (int *)((char *)dword_56674C + 2364);
  v1 = 8;
  do
  {
    result = *v0;
    if ( *v0 )
      result = sub_424540(result);
    ++v0;
    --v1;
  }
  while ( v1 );
  return result;
}

// ===== sub_4410E0 @ 0x004410E0..0x00441122 =====
_DWORD *__stdcall sub_4410E0(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 591;
  v2 = 8;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  result = a1;
  a1[599] = 0;
  a1[600] = 0;
  return result;
}

// ===== sub_441130 @ 0x00441130..0x0044123D =====
int __stdcall sub_441130(_DWORD *a1, unsigned int a2, unsigned int a3)
{
  _DWORD *v3; // esi
  _DWORD *v4; // edi
  _DWORD *v5; // eax
  int i; // ebx
  _DWORD *v7; // eax
  char *v8; // ecx
  int v9; // eax

  v3 = dword_56674C;
  if ( *((int *)dword_56674C + 599) >= 8 )
    return 1;
  v4 = 0;
  v5 = (char *)dword_56674C + 2364;
  for ( i = 0; *v5; ++i )
    ++v5;
  v7 = operator new(0x100164u);
  if ( v7 )
  {
    v8 = (char *)v3[600];
    v3[600] = v8 + 1;
    v4 = sub_4243C0(v8, v7);
  }
  if ( sub_424850(a3, v4, a2) )
  {
    v3[i + 591] = v4;
    v9 = v3[5];
    ++v3[599];
    sub_4306F0(v9, (int)v4);
    *a1 = i - 0x40000000;
    return 0;
  }
  else
  {
    if ( v4 )
      (*(void (__thiscall **)(_DWORD *, int))*v4)(v4, 1);
    return 2;
  }
}

// ===== sub_441240 @ 0x00441240..0x00441273 =====
int __userpurge sub_441240@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xC0000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2364);
  else
    return v2;
}

// ===== sub_441280 @ 0x00441280..0x004412F8 =====
BOOL __stdcall sub_441280(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_441240(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 591];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 591] = 0;
    --v1[599];
  }
  return v2 != 0;
}

// ===== sub_441300 @ 0x00441300..0x00441368 =====
BOOL __stdcall sub_441300(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_441240(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( v4 )
  {
    if ( v5 )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( v5 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return v3 != 0;
}

// ===== sub_441370 @ 0x00441370..0x004413DF =====
BOOL __userpurge sub_441370@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *v6; // edi
  int v7; // esi

  v6 = dword_56674C;
  v7 = sub_441240(a1, (int)dword_56674C);
  if ( v7 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
    sub_42B360(a2, a3, v7, a4, a5, a6);
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
    sub_4307D0(v6[5], v7);
  }
  return v7 != 0;
}

// ===== sub_4413E0 @ 0x004413E0..0x00441472 =====
int __userpurge sub_4413E0@<eax>(int a1@<eax>, unsigned int a2, int *a3, int *a4)
{
  _DWORD *v4; // ebx
  _DWORD *v5; // eax
  int v6; // edi
  int v7; // eax

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_441240(a1, (int)dword_56674C);
  v6 = (int)v5;
  if ( !v5 )
    return 255;
  v7 = sub_4246E0(v5, a2, a3, a4);
  if ( v7 )
  {
    if ( v7 == -2147483647 )
    {
      return 7;
    }
    else if ( v7 == -2147483644 )
    {
      return 8;
    }
    else
    {
      return (int)a4;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v6 + 8))(v6) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 12))(v6);
    sub_4307D0(v4[5], v6);
    return 0;
  }
}

// ===== sub_441480 @ 0x00441480..0x004414A9 =====
BOOL __stdcall sub_441480(int a1)
{
  int *v1; // edi

  v1 = (int *)sub_441240(a1, (int)dword_56674C);
  if ( v1 )
    sub_424890(v1);
  return v1 != 0;
}

// ===== sub_4414B0 @ 0x004414B0..0x004414DA =====
BOOL __stdcall sub_4414B0(int a1, int a2)
{
  int v2; // eax

  v2 = sub_441240(a1, (int)dword_56674C);
  if ( v2 )
    v2 = sub_41D050(v2, a2);
  return v2 != 0;
}

// ===== sub_4414E0 @ 0x004414E0..0x00441536 =====
int __stdcall sub_4414E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  int v10; // edx

  v10 = sub_441240(a1, (int)dword_56674C);
  if ( v10 )
    return sub_424A10(a9, v10, a2, a3, a4, a5, a6, a7, a8, a10) != 0 ? 0 : 3;
  else
    return 255;
}

// ===== sub_441540 @ 0x00441540..0x00441576 =====
int __stdcall sub_441540(int a1, unsigned int a2)
{
  int v2; // ecx

  v2 = sub_441240(a1, (int)dword_56674C);
  if ( v2 )
    return sub_424A50(a2, v2) ? 0 : 4;
  else
    return 255;
}

// ===== sub_441580 @ 0x00441580..0x004415AE =====
BOOL __stdcall sub_441580(int a1, int a2)
{
  int v2; // esi

  v2 = sub_441240(a1, (int)dword_56674C);
  if ( v2 )
    sub_424550(a2, v2);
  return v2 != 0;
}

// ===== sub_4415B0 @ 0x004415B0..0x004415D9 =====
BOOL __stdcall sub_4415B0(int a1)
{
  int v1; // eax
  int v2; // esi

  v1 = sub_441240(a1, (int)dword_56674C);
  v2 = v1;
  if ( v1 )
    sub_424A60(v1);
  return v2 != 0;
}

// ===== sub_4415E0 @ 0x004415E0..0x00441635 =====
BOOL __stdcall sub_4415E0(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  int v12; // esi

  v12 = sub_441240(a1, (int)dword_56674C);
  if ( v12 )
    sub_424A70(a2, a9, v12, a2, a3, a4, a5, a6, a7, a8, a10, a11, a12);
  return v12 != 0;
}

// ===== sub_441640 @ 0x00441640..0x0044169F =====
int __stdcall sub_441640(int a1, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // eax

  v4 = sub_441240(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_424AB0(a2, v4, a4);
  switch ( v5 )
  {
    case 0:
      return 0;
    case -2147483646:
      return 10;
    case -2147483645:
      return 6;
  }
  return a1;
}

// ===== sub_4416A0 @ 0x004416A0..0x004416FF =====
int __stdcall sub_4416A0(int a1, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // eax

  v4 = sub_441240(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_424AD0(a2, v4, a4);
  switch ( v5 )
  {
    case 0:
      return 0;
    case -2147483646:
      return 10;
    case -2147483645:
      return 6;
  }
  return a1;
}

// ===== sub_441700 @ 0x00441700..0x0044177D =====
int __stdcall sub_441700(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15)
{
  int v15; // eax
  int v16; // eax

  v15 = sub_441240(a1, (int)dword_56674C);
  if ( !v15 )
    return 255;
  v16 = sub_424AF0(v15, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
  if ( !v16 )
    return 0;
  if ( v16 == -2147483646 )
    return 10;
  return a1;
}

// ===== sub_441780 @ 0x00441780..0x004417D8 =====
int __stdcall sub_441780(int a1, int a2)
{
  int v2; // ebx
  unsigned int v3; // esi
  int *v4; // edi
  int v5; // eax

  v2 = 0;
  v3 = 0;
  v4 = (int *)((char *)dword_56674C + 2364);
  do
  {
    if ( v2 )
      break;
    if ( *v4 )
    {
      v5 = sub_424560(a2, a1, *v4);
      if ( v5 == -2147483643 )
      {
        v2 = 9;
      }
      else if ( v5 == -2147483642 )
      {
        v2 = 10;
      }
    }
    ++v3;
    ++v4;
  }
  while ( v3 < 8 );
  return v2;
}

// ===== sub_4417E0 @ 0x004417E0..0x00441809 =====
int sub_4417E0()
{
  int *v0; // edi
  int v1; // ebx
  int result; // eax

  v0 = (int *)((char *)dword_56674C + 2404);
  v1 = 8;
  do
  {
    if ( *v0 )
      result = sub_4250C0(*v0);
    ++v0;
    --v1;
  }
  while ( v1 );
  return result;
}

// ===== sub_441810 @ 0x00441810..0x00441859 =====
int sub_441810()
{
  int v0; // edi
  _DWORD **v1; // ebx
  int v2; // eax
  int v4; // [esp+Ch] [ebp-4h]

  v0 = 0;
  v1 = (_DWORD **)((char *)dword_56674C + 2404);
  v4 = 8;
  do
  {
    if ( *v1 )
    {
      v2 = sub_4250F0(*v1);
      if ( v0 || !v2 )
        v0 = 1;
    }
    ++v1;
    --v4;
  }
  while ( v4 );
  return v0;
}

// ===== sub_441860 @ 0x00441860..0x004418A2 =====
_DWORD *__stdcall sub_441860(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 601;
  v2 = 8;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  result = a1;
  a1[609] = 0;
  a1[610] = 0;
  return result;
}

// ===== sub_4418B0 @ 0x004418B0..0x004419BE =====
int __stdcall sub_4418B0(_DWORD *a1, int a2, int a3)
{
  _DWORD *v3; // esi
  _DWORD *v4; // edi
  _DWORD *v5; // eax
  int i; // ebx
  _DWORD *v7; // eax
  char *v8; // ecx
  int v9; // eax

  v3 = dword_56674C;
  if ( *((int *)dword_56674C + 609) >= 8 )
    return 1;
  v4 = 0;
  v5 = (char *)dword_56674C + 2404;
  for ( i = 0; *v5; ++i )
    ++v5;
  v7 = operator new(0x1B4u);
  if ( v7 )
  {
    v8 = (char *)v3[610];
    v3[610] = v8 + 1;
    v4 = sub_424D60(v8, v7);
  }
  if ( sub_425050(v4, a2, a3) )
  {
    if ( v4 )
      (*(void (__thiscall **)(_DWORD *, int))*v4)(v4, 1);
    return 3;
  }
  else
  {
    v3[i + 601] = v4;
    v9 = v3[5];
    ++v3[609];
    sub_4306F0(v9, (int)v4);
    *a1 = i - 1056964608;
    return 0;
  }
}

// ===== sub_4419C0 @ 0x004419C0..0x004419F3 =====
int __userpurge sub_4419C0@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xC1000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2404);
  else
    return v2;
}

// ===== sub_441A00 @ 0x00441A00..0x00441A78 =====
BOOL __stdcall sub_441A00(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 601];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 601] = 0;
    --v1[609];
  }
  return v2 != 0;
}

// ===== sub_441A80 @ 0x00441A80..0x00441AAF =====
BOOL __stdcall sub_441A80(int a1, int a2)
{
  _DWORD *v2; // eax
  _DWORD *v3; // esi

  v2 = (_DWORD *)sub_4419C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( v2 )
    sub_424F80(v2, a2);
  return v3 != 0;
}

// ===== sub_441AB0 @ 0x00441AB0..0x00441B22 =====
int __userpurge sub_441AB0@<eax>(int a1@<edi>, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // eax

  v2 = sub_4419C0(a2, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return 255;
  v4 = sub_425620(a1, v2);
  if ( v4 )
  {
    if ( v4 == -2147483643 )
      return 4;
    if ( v4 == -2147483642 )
      return 5;
    return 255;
  }
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return 0;
}

// ===== sub_441B30 @ 0x00441B30..0x00441B98 =====
BOOL __stdcall sub_441B30(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( v4 )
  {
    if ( v5 )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( v5 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return v3 != 0;
}

// ===== sub_441BA0 @ 0x00441BA0..0x00441C15 =====
int __userpurge sub_441BA0@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *v6; // edi
  int v7; // esi

  v6 = dword_56674C;
  v7 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  sub_4255E0(a2, a3, v7, a4, a5, a6);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  sub_4307D0(v6[5], v7);
  return 0;
}

// ===== sub_441C20 @ 0x00441C20..0x00441CA8 =====
int __stdcall sub_441C20(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD *v7; // esi
  int v8; // eax

  v7 = (_DWORD *)sub_4419C0(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  v8 = sub_425160(a3, a2, v7, a4, a5, a6, a7);
  if ( v8 )
  {
    if ( v8 == -2147483646 )
    {
      return 2;
    }
    else if ( v8 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(_DWORD *))(*v7 + 8))(v7) )
      (*(void (__thiscall **)(_DWORD *))(*v7 + 12))(v7);
    return 0;
  }
}

// ===== sub_441CB0 @ 0x00441CB0..0x00441D27 =====
int __stdcall sub_441CB0(int a1, int a2)
{
  int v2; // esi
  int v3; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_4251D0(a2, v2);
  if ( v3 )
  {
    if ( v3 == -2147483646 )
    {
      return 2;
    }
    else if ( v3 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_441D30 @ 0x00441D30..0x00441DA7 =====
int __stdcall sub_441D30(int a1, int a2)
{
  int v2; // esi
  int v3; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_425220(a2, v2);
  if ( v3 )
  {
    if ( v3 == -2147483646 )
    {
      return 2;
    }
    else if ( v3 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_441DB0 @ 0x00441DB0..0x00441E25 =====
int __stdcall sub_441DB0(int a1, int a2)
{
  int v2; // esi
  int v3; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_425270(a2, v2);
  if ( v3 )
  {
    if ( v3 == -2147483646 )
    {
      return 2;
    }
    else if ( v3 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_441E30 @ 0x00441E30..0x00441EA5 =====
int __stdcall sub_441E30(int a1, int a2)
{
  int v2; // esi
  int v3; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_4252C0(a2, v2);
  if ( v3 )
  {
    if ( v3 == -2147483646 )
    {
      return 2;
    }
    else if ( v3 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_441EB0 @ 0x00441EB0..0x00441F25 =====
int __stdcall sub_441EB0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return 255;
  v4 = sub_425310(v2, a2);
  if ( v4 )
  {
    if ( v4 == -2147483646 )
    {
      return 2;
    }
    else if ( v4 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
    return 0;
  }
}

// ===== sub_441F30 @ 0x00441F30..0x00441FAC =====
int __stdcall sub_441F30(int a1, int a2, int a3, int a4)
{
  _DWORD *v4; // esi
  int v5; // eax

  v4 = (_DWORD *)sub_4419C0(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_425360(a3, a2, v4, a4);
  if ( v5 )
  {
    if ( v5 == -2147483646 )
    {
      return 2;
    }
    else if ( v5 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(_DWORD *))(*v4 + 8))(v4) )
      (*(void (__thiscall **)(_DWORD *))(*v4 + 12))(v4);
    return 0;
  }
}

// ===== sub_441FB0 @ 0x00441FB0..0x0044202C =====
int __stdcall sub_441FB0(int a1, int a2, int a3, int a4)
{
  _DWORD *v4; // esi
  int v5; // eax

  v4 = (_DWORD *)sub_4419C0(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_4253B0(a2, a3, v4, a4);
  if ( v5 )
  {
    if ( v5 == -2147483646 )
    {
      return 2;
    }
    else if ( v5 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(_DWORD *))(*v4 + 8))(v4) )
      (*(void (__thiscall **)(_DWORD *))(*v4 + 12))(v4);
    return 0;
  }
}

// ===== sub_442030 @ 0x00442030..0x004420A7 =====
int __stdcall sub_442030(int a1, int a2)
{
  int v2; // esi
  int v3; // eax

  v2 = sub_4419C0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_425400(a2, v2);
  if ( v3 )
  {
    if ( v3 == -2147483646 )
    {
      return 2;
    }
    else if ( v3 == -2147483644 )
    {
      return 3;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_4420B0 @ 0x004420B0..0x004420F2 =====
_DWORD *__stdcall sub_4420B0(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 611;
  v2 = 32;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  result = a1;
  a1[643] = 0;
  a1[644] = 0;
  return result;
}

// ===== sub_442100 @ 0x00442100..0x00442216 =====
int __stdcall sub_442100(_DWORD *a1)
{
  _DWORD *v1; // edi
  int result; // eax
  int v3; // eax
  int v4; // ebx
  int v5; // esi
  _DWORD *i; // eax
  _DWORD *v7; // eax
  int v8; // ecx
  _DWORD *v9; // eax

  v1 = dword_56674C;
  result = 1;
  if ( *((int *)dword_56674C + 643) < 32 )
  {
    v3 = sub_443270();
    v4 = v3;
    if ( v3 )
    {
      if ( (unsigned int)sub_41B0B0(v3) >= 8 )
      {
        return 3;
      }
      else
      {
        v5 = 0;
        for ( i = v1 + 611; *i; ++v5 )
          ++i;
        v7 = operator new(0x174u);
        if ( v7 )
        {
          v8 = v1[644];
          v1[644] = v8 + 1;
          v9 = sub_420EC0(v8, v4, v7);
        }
        else
        {
          v9 = 0;
        }
        v1[v5 + 611] = v9;
        ++v1[643];
        *a1 = v5 - 0x10000000;
        return 0;
      }
    }
    else
    {
      return 2;
    }
  }
  return result;
}

// ===== sub_442220 @ 0x00442220..0x00442253 =====
int __userpurge sub_442220@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xF0000000 && (v3 = sub_443260(0), v3 < 0x20) )
    return *(_DWORD *)(a2 + 4 * v3 + 2444);
  else
    return v2;
}

// ===== sub_442260 @ 0x00442260..0x004422AF =====
BOOL __usercall sub_442260@<eax>(int a1@<eax>)
{
  _DWORD *v1; // esi
  int v2; // ecx
  int v3; // ebx
  int v4; // edi
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v3 = sub_442220(a1, (int)dword_56674C);
  if ( v3 )
  {
    v4 = sub_443260(v2);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 611];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 611] = 0;
    --v1[643];
  }
  return v3 != 0;
}

// ===== sub_4422B0 @ 0x004422B0..0x00442318 =====
BOOL __stdcall sub_4422B0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_442220(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( v4 )
  {
    if ( v5 )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( v5 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return v3 != 0;
}

// ===== sub_442320 @ 0x00442320..0x0044237C =====
BOOL __userpurge sub_442320@<eax>(int a1@<edi>, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // esi
  int v6; // edi

  v4 = sub_442220(a2, (int)dword_56674C);
  v5 = v4;
  if ( v4 )
  {
    v6 = (*(int (__thiscall **)(int, int))(*(_DWORD *)v4 + 8))(v4, a1);
    if ( v6 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v5 + 44))(v5, a3);
    if ( v6 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  }
  return v5 != 0;
}

// ===== sub_442380 @ 0x00442380..0x004423BE =====
int __stdcall sub_442380(int a1, int a2, int a3)
{
  int v3; // eax

  v3 = sub_442220(a1, (int)dword_56674C);
  if ( v3 )
    return sub_4211E0(v3, a2, a3) != 0 ? 0 : 4;
  else
    return 255;
}

// ===== sub_4423C0 @ 0x004423C0..0x004423FF =====
int __stdcall sub_4423C0(int a1, int a2, int a3)
{
  _DWORD *v3; // eax

  v3 = (_DWORD *)sub_442220(a1, (int)dword_56674C);
  if ( v3 )
    return sub_421200(a2, v3, a3) != 0 ? 0 : 5;
  else
    return 255;
}

// ===== sub_442400 @ 0x00442400..0x0044245A =====
BOOL __stdcall sub_442400(int a1, int a2, int a3)
{
  int v3; // eax
  int v4; // esi
  int v5; // edi

  v3 = sub_442220(a1, (int)dword_56674C);
  v4 = v3;
  if ( v3 )
  {
    v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
    if ( v5 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v4 + 12))(v4);
    sub_421430(v4, a2, a3);
    if ( v5 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v4 + 12))(v4);
  }
  return v4 != 0;
}

// ===== sub_442460 @ 0x00442460..0x0044248E =====
BOOL __stdcall sub_442460(int a1, _DWORD *a2)
{
  int v2; // esi

  v2 = sub_442220(a1, (int)dword_56674C);
  if ( v2 )
    sub_421500(a2, v2);
  return v2 != 0;
}

// ===== sub_442490 @ 0x00442490..0x004424D3 =====
BOOL __stdcall sub_442490(int a1, _DWORD *a2)
{
  _DWORD *v2; // ecx
  _DWORD v4[2]; // [esp+0h] [ebp-8h] BYREF

  v2 = (_DWORD *)sub_442220(a1, (int)dword_56674C);
  if ( v2 )
  {
    sub_421570(v4, v2, &a1);
    *a2 = a1 != 0 ? v4[1] : 0;
  }
  return v2 != 0;
}

// ===== sub_4424E0 @ 0x004424E0..0x0044250A =====
BOOL __stdcall sub_4424E0(int a1, int a2)
{
  int v2; // eax

  v2 = sub_442220(a1, (int)dword_56674C);
  if ( v2 )
    v2 = sub_421290(v2, a2);
  return v2 != 0;
}

// ===== sub_442510 @ 0x00442510..0x00442552 =====
_DWORD *__stdcall sub_442510(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 645;
  v2 = 8;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  result = a1;
  a1[653] = 0;
  a1[654] = 0;
  return result;
}

// ===== sub_442560 @ 0x00442560..0x0044260E =====
int sub_442560()
{
  _DWORD *v0; // esi
  int result; // eax
  int v2; // edi
  _DWORD *i; // eax
  _DWORD *v4; // eax
  char *v5; // ecx
  _DWORD *v6; // eax

  v0 = dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 653) < 8 )
  {
    v2 = 0;
    for ( i = (char *)dword_56674C + 2580; *i; ++v2 )
      ++i;
    v4 = operator new(0x134u);
    if ( v4 )
    {
      v5 = (char *)v0[654];
      v0[654] = v5 + 1;
      v6 = sub_420DE0(v5, v4);
    }
    else
    {
      v6 = 0;
    }
    v0[v2 + 645] = v6;
    ++v0[653];
    return v2 - 251658240;
  }
  return result;
}

// ===== sub_442610 @ 0x00442610..0x00442643 =====
int __userpurge sub_442610@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xF1000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2580);
  else
    return v2;
}

// ===== sub_442650 @ 0x00442650..0x0044269F =====
BOOL __usercall sub_442650@<eax>(int a1@<eax>)
{
  _DWORD *v1; // esi
  int v2; // ecx
  int v3; // ebx
  int v4; // edi
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v3 = sub_442610(a1, (int)dword_56674C);
  if ( v3 )
  {
    v4 = sub_443260(v2);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 645];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 645] = 0;
    --v1[653];
  }
  return v3 != 0;
}

// ===== sub_4426A0 @ 0x004426A0..0x00442708 =====
BOOL __stdcall sub_4426A0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_442610(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
  if ( v4 )
  {
    if ( v5 )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( v5 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return v3 != 0;
}

// ===== sub_442710 @ 0x00442710..0x0044277E =====
BOOL __stdcall sub_442710(int a1, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // esi

  v4 = sub_442610(a1, (int)dword_56674C);
  v5 = v4;
  if ( v4 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v4 + 8))(v4) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v5 + 44))(v5, a2, a3);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v5 + 72))(v5, a4);
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  }
  return v5 != 0;
}

// ===== sub_442780 @ 0x00442780..0x004427F0 =====
int __userpurge sub_442780@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  _DWORD *v4; // ebx
  int v5; // eax

  v4 = (_DWORD *)sub_442610(a1, (int)dword_56674C);
  if ( !v4 )
    return 255;
  v5 = sub_443270();
  if ( !v5 )
    return 1;
  if ( v4 == (_DWORD *)v5 )
    return 3;
  return sub_41AB40(v5, v4, a3, a4) != 0 ? 0 : 4;
}

// ===== sub_4427F0 @ 0x004427F0..0x00442848 =====
int __userpurge sub_4427F0@<eax>(int a1@<eax>, int a2)
{
  int v2; // ebx
  int v3; // eax

  v2 = sub_442610(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_443270();
  if ( v3 )
    return sub_41AC40(v2, v3) != 0 ? 0 : 2;
  else
    return 1;
}

// ===== sub_442850 @ 0x00442850..0x00442C23 =====
int *__thiscall sub_442850(int *this, void *a2)
{
  int v3; // edi
  _DWORD *v4; // eax
  int v5; // eax
  _DWORD *v6; // eax
  _DWORD *v7; // eax
  int v8; // eax

  v3 = dword_5666F8;
  *this = (int)hWndParent;
  this[4] = v3;
  v4 = operator new(0x70u);
  if ( v4 )
    v5 = sub_430570(this + 6, v4, 1024, (int)a2);
  else
    v5 = 0;
  this[5] = v5;
  this[6] = 0;
  this[8] = 0;
  this[9] = 0;
  this[18] = 0;
  this[19] = 0;
  sub_442E70(-1);
  v6 = operator new(0x144u);
  if ( v6 )
    v7 = sub_41E890(v6);
  else
    v7 = 0;
  this[20] = (int)v7;
  v8 = sub_4207A0((int)v7);
  sub_431140(this[5], v8);
  sub_4306F0(this[5], this[20]);
  sub_43E490(1, 0, this);
  this[535] = 0;
  this[536] = 0;
  memset(this + 23, 0, 0x800u);
  this[545] = 0;
  this[546] = 0;
  this[537] = 0;
  this[538] = 0;
  this[539] = 0;
  this[540] = 0;
  this[541] = 0;
  this[542] = 0;
  this[543] = 0;
  this[544] = 0;
  this[555] = 0;
  this[556] = 0;
  this[547] = 0;
  this[548] = 0;
  this[549] = 0;
  this[550] = 0;
  this[551] = 0;
  this[552] = 0;
  this[553] = 0;
  this[554] = 0;
  this[565] = 0;
  this[566] = 0;
  this[557] = 0;
  this[558] = 0;
  this[559] = 0;
  this[560] = 0;
  this[561] = 0;
  this[562] = 0;
  this[563] = 0;
  this[564] = 0;
  this[571] = 0;
  this[572] = 0;
  this[567] = 0;
  this[568] = 0;
  this[569] = 0;
  this[570] = 0;
  this[589] = 0;
  this[590] = 0;
  this[573] = 0;
  this[574] = 0;
  this[575] = 0;
  this[576] = 0;
  this[577] = 0;
  this[578] = 0;
  this[579] = 0;
  this[580] = 0;
  this[581] = 0;
  this[582] = 0;
  this[583] = 0;
  this[584] = 0;
  this[585] = 0;
  this[586] = 0;
  this[587] = 0;
  this[588] = 0;
  this[599] = 0;
  this[600] = 0;
  this[591] = 0;
  this[592] = 0;
  this[593] = 0;
  this[594] = 0;
  this[595] = 0;
  this[596] = 0;
  this[597] = 0;
  this[598] = 0;
  this[609] = 0;
  this[610] = 0;
  this[601] = 0;
  this[602] = 0;
  this[603] = 0;
  this[604] = 0;
  this[605] = 0;
  this[606] = 0;
  this[607] = 0;
  this[608] = 0;
  this[643] = 0;
  this[644] = 0;
  this[611] = 0;
  this[612] = 0;
  this[613] = 0;
  this[614] = 0;
  this[615] = 0;
  this[616] = 0;
  this[617] = 0;
  this[618] = 0;
  this[619] = 0;
  this[620] = 0;
  this[621] = 0;
  this[622] = 0;
  this[623] = 0;
  this[624] = 0;
  this[625] = 0;
  this[626] = 0;
  this[627] = 0;
  this[628] = 0;
  this[629] = 0;
  this[630] = 0;
  this[631] = 0;
  this[632] = 0;
  this[633] = 0;
  this[634] = 0;
  this[635] = 0;
  this[636] = 0;
  this[637] = 0;
  this[638] = 0;
  this[639] = 0;
  this[640] = 0;
  this[641] = 0;
  this[642] = 0;
  this[653] = 0;
  this[654] = 0;
  this[645] = 0;
  this[646] = 0;
  this[647] = 0;
  this[648] = 0;
  this[649] = 0;
  this[650] = 0;
  this[651] = 0;
  this[652] = 0;
  this[1] = 0;
  this[2] = 0;
  this[3] = 0;
  return this;
}

// ===== sub_442C30 @ 0x00442C30..0x00442C8B =====
_DWORD *__usercall sub_442C30@<eax>(_DWORD *a1@<esi>)
{
  void (__thiscall ***v1)(_DWORD, int); // ecx
  void (__thiscall ***v2)(_DWORD, int); // ecx

  v1 = (void (__thiscall ***)(_DWORD, int))a1[5];
  if ( v1 )
    (**v1)(v1, 1);
  v2 = (void (__thiscall ***)(_DWORD, int))a1[20];
  if ( v2 )
    (**v2)(v2, 1);
  sub_442510(a1);
  sub_4420B0(a1);
  sub_43E4D0(a1);
  sub_43EF40(a1);
  sub_43F240(a1);
  sub_43F7A0(a1);
  sub_43FC00(a1);
  sub_440520(a1);
  sub_4410E0(a1);
  return sub_441860(a1);
}

// ===== sub_442C90 @ 0x00442C90..0x00442C9B =====
void *sub_442C90()
{
  void *result; // eax

  result = dword_566750;
  dword_565D5C = (int)dword_566750;
  return result;
}

// ===== sub_442CA0 @ 0x00442CA0..0x00442D38 =====
int sub_442CA0()
{
  int *v0; // esi
  int v1; // edx

  v0 = (int *)dword_56674C;
  sub_430CD0(*((_DWORD **)dword_56674C + 5));
  sub_4306B0(v0[5]);
  sub_4306F0(v0[5], v0[20]);
  sub_43E490(1, 0, v0);
  sub_43E4D0(v0);
  sub_43EF40(v0);
  sub_43F240(v0);
  sub_43F7A0(v0);
  sub_43FC00(v0);
  sub_440520(v0);
  sub_4410E0(v0);
  sub_4420B0(v0);
  sub_442510(v0);
  sub_440570(0, 0);
  sub_443130(v0);
  sub_442E70(-1);
  sub_442EC0(v0);
  sub_430D20(0, v0[5]);
  return sub_430D10(*(_DWORD *)(v1 + 20));
}

// ===== sub_442D40 @ 0x00442D40..0x00442E0F =====
int __fastcall sub_442D40(int a1, int a2, int a3, int a4)
{
  char *v4; // edi
  int v5; // ecx
  int v6; // edx
  int v7; // ecx
  _DWORD **v8; // ebx
  _DWORD **v9; // esi
  int v10; // ebx
  _DWORD **v11; // ebx
  int v12; // ecx
  int v14; // [esp+18h] [ebp+8h]
  int v15; // [esp+18h] [ebp+8h]

  v4 = (char *)dword_56674C;
  sub_407B00(a2);
  sub_431120(*((_DWORD *)v4 + 5), v5);
  *((_DWORD *)v4 + 8) = a3;
  *((_DWORD *)v4 + 6) = 0;
  *((_DWORD *)v4 + 7) = 0;
  *((_DWORD *)v4 + 9) = a4;
  *((_DWORD *)v4 + 10) = v6;
  *((_DWORD *)v4 + 11) = sub_407B20();
  sub_409190((_DWORD *)v4 + 12, v7);
  (*(void (__thiscall **)(_DWORD))(**((_DWORD **)v4 + 20) + 124))(*((_DWORD *)v4 + 20));
  v8 = (_DWORD **)(v4 + 2148);
  v14 = 8;
  do
  {
    if ( *v8 )
      sub_420D70(*v8);
    ++v8;
    --v14;
  }
  while ( v14 );
  v9 = (_DWORD **)(v4 + 2188);
  v10 = 8;
  do
  {
    if ( *v9 )
      sub_420730(*v9);
    ++v9;
    --v10;
  }
  while ( v10 );
  v11 = (_DWORD **)(v4 + 2292);
  v15 = 16;
  do
  {
    if ( *v11 )
      sub_42BDE0(*v11);
    ++v11;
    --v15;
  }
  while ( v15 );
  sub_41ADF0((int)v4, 0);
  sub_430D10(*((_DWORD *)v4 + 5));
  return v12 + 1;
}

// ===== sub_442E10 @ 0x00442E10..0x00442E4C =====
_DWORD *__usercall sub_442E10@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  if ( a2 )
  {
    *result = a2[6];
    result[1] = a2[7];
    result[2] = a2[8];
    result[3] = a2[9];
    result[4] = a2[10];
    result[5] = a2[11];
  }
  else
  {
    *result = 0;
    result[1] = 0;
    result[2] = 0;
    result[3] = 0;
    result[4] = 0;
    result[5] = 0;
  }
  return result;
}

// ===== sub_442E50 @ 0x00442E50..0x00442E6D =====
int __thiscall sub_442E50(_DWORD *this)
{
  _DWORD *v1; // eax
  int v2; // edx
  int result; // eax

  v1 = dword_56674C;
  *this = *((_DWORD *)dword_56674C + 12);
  this[1] = v1[13];
  v2 = v1[14];
  result = v1[15];
  this[2] = v2;
  this[3] = result;
  return result;
}

// ===== sub_442E70 @ 0x00442E70..0x00442E88 =====
int __userpurge sub_442E70@<eax>(int *a1@<eax>, int a2@<ecx>, int a3)
{
  a1[16] = a2;
  a1[17] = a3;
  return sub_430D10(a1[5]);
}

// ===== sub_442E90 @ 0x00442E90..0x00442EC0 =====
int __usercall sub_442E90@<eax>(_DWORD *a1@<esi>)
{
  int v1; // edx
  int result; // eax
  int v3; // edx

  v1 = *(_DWORD *)(dword_565B2C + 64);
  result = 0;
  if ( v1 >= 0 && v1 < *(_DWORD *)(dword_565B2C + 32) )
  {
    v3 = *(_DWORD *)(dword_565B2C + 68);
    if ( v3 >= 0 && v3 < *(_DWORD *)(dword_565B2C + 36) )
    {
      *a1 = *(_DWORD *)(dword_565B2C + 64);
      a1[1] = v3;
      return 1;
    }
  }
  return result;
}

// ===== sub_442EC0 @ 0x00442EC0..0x00442ED7 =====
int __userpurge sub_442EC0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  sub_41A680(a1, a2);
  return sub_430D10(*(_DWORD *)(a3 + 20));
}

// ===== sub_442EE0 @ 0x00442EE0..0x00442F75 =====
int __usercall sub_442EE0@<eax>(int a1@<eax>)
{
  _DWORD *v1; // edi
  int result; // eax
  int v4; // ebx
  char **v5; // eax
  _DWORD v6[6]; // [esp+10h] [ebp-30h] BYREF
  _DWORD v7[6]; // [esp+28h] [ebp-18h] BYREF

  v1 = dword_56674C;
  sub_442E10(v6, dword_56674C);
  result = sub_407DA0(a1, (_DWORD *)dword_565D5C, v6[2], v6[3], v6[4]);
  v4 = result;
  if ( result )
  {
    sub_407F20(dword_565D5C, a1, v7);
    if ( sub_442FF0() )
    {
      v5 = (char **)sub_442E10(v6, v1);
      sub_40ADF0((int)v7, v5);
      sub_443080();
    }
    else
    {
      sub_40A620((int)v7, 0);
    }
    return v4;
  }
  return result;
}

// ===== sub_442F80 @ 0x00442F80..0x00442FEC =====
int __userpurge sub_442F80@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // ebx
  int result; // eax
  int v5; // edi
  _DWORD v6[6]; // [esp+10h] [ebp-18h] BYREF

  v2 = dword_56674C;
  sub_442E10(v6, dword_56674C);
  result = sub_407DA0(a1, (_DWORD *)dword_565D5C, v6[2], v6[3], v6[4]);
  v5 = result;
  if ( result )
  {
    sub_407F20(dword_565D5C, a1, v6);
    sub_430D30(v6, v2[5], a2);
    return v5;
  }
  return result;
}

// ===== sub_442FF0 @ 0x00442FF0..0x00443074 =====
int __usercall sub_442FF0@<eax>(_DWORD *a1@<esi>)
{
  int v1; // edx
  int result; // eax
  _DWORD v3[2]; // [esp+4h] [ebp-1Ch] BYREF
  struct tagRECT rc; // [esp+Ch] [ebp-14h] BYREF

  if ( !a1[18] )
    return 0;
  if ( a1[19] )
    return 0;
  SetRect(&rc, 0, 0, 1280, 720);
  if ( (*(int (__stdcall **)(_DWORD, _DWORD, _DWORD *, struct tagRECT *, int))(*(_DWORD *)a1[18] + 76))(
         a1[18],
         0,
         v3,
         &rc,
         0x8000) < 0 )
    return 0;
  v1 = v3[0];
  result = 1;
  a1[6] = v3[1];
  a1[7] = v1;
  a1[19] = 1;
  return result;
}

// ===== sub_443080 @ 0x00443080..0x004430B3 =====
int __usercall sub_443080@<eax>(_DWORD *a1@<esi>)
{
  int v1; // ecx
  int result; // eax

  v1 = a1[18];
  result = 0;
  if ( v1 )
  {
    if ( a1[19] )
    {
      (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)v1 + 80))(v1, 0);
      a1[6] = 0;
      a1[7] = 0;
      a1[19] = 0;
      return 1;
    }
  }
  return result;
}

// ===== sub_4430C0 @ 0x004430C0..0x004430EC =====
int sub_4430C0()
{
  _DWORD **v0; // esi

  v0 = (_DWORD **)dword_56674C;
  if ( !sub_442FF0(dword_56674C) )
    return 0;
  sub_430DD0(v0[5]);
  sub_443080(v0);
  return 1;
}

// ===== sub_4430F0 @ 0x004430F0..0x0044312E =====
int __stdcall sub_4430F0(int *a1, _DWORD *a2)
{
  _DWORD **v2; // esi

  v2 = (_DWORD **)dword_56674C;
  if ( !sub_442FF0(dword_56674C) )
    return 0;
  *a1 = sub_430EF0(v2[5], a2);
  sub_443080(v2);
  return 1;
}

// ===== sub_443130 @ 0x00443130..0x00443147 =====
int __userpurge sub_443130@<eax>(int a1@<eax>, int a2)
{
  sub_41A670(a1);
  return sub_430D10(*(_DWORD *)(a2 + 20));
}

// ===== sub_443160 @ 0x00443160..0x00443168 =====
int __usercall sub_443160@<eax>(int a1@<eax>)
{
  return sub_42E9A0(*(_DWORD *)(a1 + 20));
}

// ===== sub_443170 @ 0x00443170..0x0044317D =====
int sub_443170()
{
  return sub_431130(*((_DWORD *)dword_56674C + 5));
}

// ===== sub_443180 @ 0x00443180..0x004431E2 =====
int __fastcall sub_443180(int a1, int a2)
{
  int result; // eax

  result = -1;
  switch ( a2 )
  {
    case 0:
      result = *((_DWORD *)dword_56674C + 535);
      break;
    case 1:
      result = *((_DWORD *)dword_56674C + 545);
      break;
    case 2:
      result = *((_DWORD *)dword_56674C + 555);
      break;
    case 3:
      result = *((_DWORD *)dword_56674C + 565);
      break;
    case 4:
      result = *((_DWORD *)dword_56674C + 571);
      break;
    case 5:
      result = *((_DWORD *)dword_56674C + 589);
      break;
    case 6:
      result = *((_DWORD *)dword_56674C + 599);
      break;
    case 7:
      result = *((_DWORD *)dword_56674C + 609);
      break;
    case 16:
      result = *((_DWORD *)dword_56674C + 643);
      break;
    case 17:
      result = *((_DWORD *)dword_56674C + 653);
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_443240 @ 0x00443240..0x00443254 =====
void __userpurge sub_443240(int *a1@<eax>, unsigned int a2@<ecx>, int a3)
{
  sub_430830(*(int **)(a3 + 20), a2, a1);
}

// ===== sub_443260 @ 0x00443260..0x00443266 =====
int __usercall sub_443260@<eax>(int a1@<eax>)
{
  return a1 & 0xFFFFFF;
}

// ===== sub_443270 @ 0x00443270..0x004432EC =====
int __usercall sub_443270@<eax>(int a1@<edi>, int a2@<esi>)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 80);
  if ( a2 )
  {
    result = sub_43E5D0(a2, a1);
    if ( !result )
    {
      result = sub_43F050(a2, a1);
      if ( !result )
      {
        result = sub_43F350(a2, a1);
        if ( !result )
        {
          result = sub_43F8B0(a2, a1);
          if ( !result )
          {
            result = sub_43FD30(a2, a1);
            if ( !result )
            {
              result = sub_4406C0(a2, a1);
              if ( !result )
              {
                result = sub_441240(a2, a1);
                if ( !result )
                {
                  result = sub_4419C0(a2, a1);
                  if ( !result )
                  {
                    result = sub_442220(a2, a1);
                    if ( !result )
                      return sub_442610(a2, a1);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_4432F0 @ 0x004432F0..0x004432FD =====
int __fastcall sub_4432F0(int a1, int a2)
{
  int v2; // ecx
  int result; // eax

  v2 = *(_DWORD *)(*((_DWORD *)dword_56674C + 5) + 12);
  for ( result = 0; v2; ++result )
  {
    if ( a2 )
      *(_DWORD *)(a2 + 4 * result) = *(_DWORD *)(v2 + 4);
    v2 = *(_DWORD *)(v2 + 8);
  }
  return result;
}

// ===== sub_443300 @ 0x00443300..0x0044330B =====
int __usercall sub_443300@<eax>(int a1@<eax>, int a2@<esi>)
{
  return sub_4307D0(*(_DWORD *)(a1 + 20), a2);
}

// ===== sub_443310 @ 0x00443310..0x00443378 =====
BOOL __userpurge sub_443310@<eax>(int a1@<eax>, int a2)
{
  int v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = sub_443270((int)dword_56674C, a1);
  if ( !v2 )
    return v2 != 0;
  v3 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v2 + 4))(v2, a2);
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  if ( v3 )
  {
    if ( v4 )
      return v2 != 0;
    goto LABEL_6;
  }
  if ( v4 )
LABEL_6:
    (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
  return v2 != 0;
}

// ===== sub_443380 @ 0x00443380..0x004433E6 =====
BOOL __userpurge sub_443380@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( !v2 )
    return v2 != 0;
  v3 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
  sub_41AD60(v2, a2);
  v4 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
  if ( v3 )
  {
    if ( v4 )
      return v2 != 0;
    goto LABEL_6;
  }
  if ( v4 )
LABEL_6:
    (*(void (__thiscall **)(_DWORD *))(*v2 + 12))(v2);
  return v2 != 0;
}

// ===== sub_4433F0 @ 0x004433F0..0x00443456 =====
BOOL __userpurge sub_4433F0@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( !v2 )
    return v2 != 0;
  v3 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
  sub_41ADA0(v2, a2);
  v4 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
  if ( v3 )
  {
    if ( v4 )
      return v2 != 0;
    goto LABEL_6;
  }
  if ( v4 )
LABEL_6:
    (*(void (__thiscall **)(_DWORD *))(*v2 + 12))(v2);
  return v2 != 0;
}

// ===== sub_443460 @ 0x00443460..0x004434B7 =====
BOOL __userpurge sub_443460@<eax>(int a1@<eax>, int a2)
{
  int v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = sub_443270((int)dword_56674C, a1);
  if ( v2 )
  {
    v3 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v2 + 72))(v2, a2);
    v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
    if ( v3 || v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
  }
  return v2 != 0;
}

// ===== sub_4434C0 @ 0x004434C0..0x0044351B =====
BOOL __userpurge sub_4434C0@<eax>(int a1@<eax>, int a2, int a3)
{
  int v3; // esi
  int v4; // edi

  v3 = sub_443270((int)dword_56674C, a1);
  if ( v3 )
  {
    v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
    if ( v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v3 + 44))(v3, a2, a3);
    if ( v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  }
  return v3 != 0;
}

// ===== sub_443520 @ 0x00443520..0x004435A6 =====
BOOL __userpurge sub_443520@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  void *v4; // edi
  int v5; // esi
  int v6; // ebx
  int v8; // [esp+8h] [ebp-4h]

  v4 = dword_56674C;
  v5 = sub_443270((int)dword_56674C, a1);
  if ( v5 )
  {
    v8 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5);
    if ( v8 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
    v6 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5);
    (*(void (__thiscall **)(int, int, int, int))(*(_DWORD *)v5 + 60))(v5, a2, a3, a4);
    if ( v6 != (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5) )
      sub_443300((int)v4, v5);
    if ( v8 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  }
  return v5 != 0;
}

// ===== sub_4435B0 @ 0x004435B0..0x00443605 =====
BOOL __userpurge sub_4435B0@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( v2 )
  {
    v3 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
    sub_41B660(v2, a2);
    v4 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
    if ( v3 || v4 )
      (*(void (__thiscall **)(_DWORD *))(*v2 + 12))(v2);
  }
  return v2 != 0;
}

// ===== sub_443610 @ 0x00443610..0x00443665 =====
BOOL __userpurge sub_443610@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // esi
  int v3; // edi
  int v4; // eax

  v2 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( v2 )
  {
    v3 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
    sub_41B6A0(v2, a2);
    v4 = (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2);
    if ( v3 || v4 )
      (*(void (__thiscall **)(_DWORD *))(*v2 + 12))(v2);
  }
  return v2 != 0;
}

// ===== sub_443670 @ 0x00443670..0x004436C4 =====
BOOL __userpurge sub_443670@<eax>(int a1@<eax>, int a2)
{
  int v2; // esi
  int v3; // edi

  v2 = sub_443270((int)dword_56674C, a1);
  if ( v2 )
  {
    v3 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
    if ( v3 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_41B6E0(v2, a2);
    if ( v3 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
  }
  return v2 != 0;
}

// ===== sub_4436D0 @ 0x004436D0..0x0044372B =====
BOOL __userpurge sub_4436D0@<eax>(int a1@<eax>, int a2, int a3)
{
  int v3; // esi
  int v4; // edi

  v3 = sub_443270((int)dword_56674C, a1);
  if ( v3 )
  {
    v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
    if ( v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v3 + 56))(v3, a2, a3);
    if ( v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  }
  return v3 != 0;
}

// ===== sub_443730 @ 0x00443730..0x00443789 =====
BOOL __userpurge sub_443730@<eax>(int a1@<eax>, int a2, int a3)
{
  _DWORD *v3; // esi
  int v4; // edi

  v3 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( v3 )
  {
    v4 = (*(int (__thiscall **)(_DWORD *))(*v3 + 8))(v3);
    if ( v4 )
      (*(void (__thiscall **)(_DWORD *))(*v3 + 12))(v3);
    sub_41B320(v3, a2, a3);
    if ( v4 )
      (*(void (__thiscall **)(_DWORD *))(*v3 + 12))(v3);
  }
  return v3 != 0;
}

// ===== sub_443790 @ 0x00443790..0x00443816 =====
BOOL __userpurge sub_443790@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  void *v4; // edi
  int v5; // esi
  int v6; // ebx
  int v8; // [esp+8h] [ebp-4h]

  v4 = dword_56674C;
  v5 = sub_443270((int)dword_56674C, a1);
  if ( v5 )
  {
    v8 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5);
    if ( v8 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
    v6 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5);
    (*(void (__thiscall **)(int, int, int, int))(*(_DWORD *)v5 + 68))(v5, a2, a3, a4);
    if ( v6 != (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5) )
      sub_443300((int)v4, v5);
    if ( v8 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  }
  return v5 != 0;
}

// ===== sub_443820 @ 0x00443820..0x004438A4 =====
BOOL __userpurge sub_443820@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  void *v4; // edi
  _DWORD *v5; // esi
  int v6; // ebx
  int v8; // [esp+8h] [ebp-4h]

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( v5 )
  {
    v8 = (*(int (__thiscall **)(_DWORD *))(*v5 + 8))(v5);
    if ( v8 )
      (*(void (__thiscall **)(_DWORD *))(*v5 + 12))(v5);
    v6 = (*(int (__thiscall **)(_DWORD *))(*v5 + 28))(v5);
    sub_41B580(v5, a2, a3, a4);
    if ( v6 != (*(int (__thiscall **)(_DWORD *))(*v5 + 28))(v5) )
      sub_443300((int)v4, (int)v5);
    if ( v8 )
      (*(void (__thiscall **)(_DWORD *))(*v5 + 12))(v5);
  }
  return v5 != 0;
}

// ===== sub_4438B0 @ 0x004438B0..0x0044395D =====
int __userpurge sub_4438B0@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  void *v4; // edi
  int v5; // esi
  int v6; // ebx
  int v7; // eax

  v4 = dword_56674C;
  v5 = sub_443270((int)dword_56674C, a1);
  if ( !v5 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  v6 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5);
  v7 = (*(int (__thiscall **)(int, int, int, int))(*(_DWORD *)v5 + 92))(v5, a2, a3, a4);
  if ( v7 )
    return v7 != -65535 ? 254 : 5;
  if ( v6 != (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 28))(v5) )
    sub_443300((int)v4, v5);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 12))(v5);
  return 0;
}

// ===== sub_443960 @ 0x00443960..0x004439AD =====
int __userpurge sub_443960@<eax>(int a1@<esi>, int a2, int a3)
{
  int v3; // eax
  int v4; // eax

  v3 = sub_443270((int)dword_56674C, a1);
  if ( !v3 )
    return 255;
  v4 = (*(int (__thiscall **)(int, int, int))(*(_DWORD *)v3 + 96))(v3, a2, a3);
  if ( v4 )
    return v4 != -65535 ? 254 : 5;
  else
    return 0;
}

// ===== sub_4439B0 @ 0x004439B0..0x00443A35 =====
int __userpurge sub_4439B0@<eax>(int a1@<eax>, int a2)
{
  void *v2; // edi
  int v3; // esi
  int v4; // ebx

  v2 = dword_56674C;
  v3 = sub_443270((int)dword_56674C, a1);
  if ( !v3 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 84))(v3, a2);
  if ( v4 != (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3) )
    sub_443300((int)v2, v3);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  return 0;
}

// ===== sub_443A40 @ 0x00443A40..0x00443B84 =====
int __thiscall sub_443A40(void *this, int a2)
{
  int v2; // eax
  int v3; // edi
  void *v5[6]; // [esp+Ch] [ebp-2Ch] BYREF
  _BYTE v6[8]; // [esp+24h] [ebp-14h] BYREF

  v2 = sub_443270((int)dword_56674C, (int)this);
  v3 = v2;
  if ( !v2 )
    return 255;
  if ( (unsigned int)sub_41B0B0(v2) >= 8 )
    return 1;
  if ( a2 == -2 )
  {
    (*(void (__thiscall **)(int, _BYTE *))(*(_DWORD *)v3 + 32))(v3, v6);
    sub_409080(v5, 3);
    sub_40A620((int)v5, 0);
    sub_41BC70(v3, (char *)v5);
    operator delete[](v5[0]);
    return 0;
  }
  else if ( a2 == -1 )
  {
    sub_41BC70(v3, 0);
    return 0;
  }
  else if ( sub_407F20(dword_565D5C, a2, v5) )
  {
    sub_41BC70(v3, (char *)v5);
    return 0;
  }
  else
  {
    return 2;
  }
}

// ===== sub_443B90 @ 0x00443B90..0x00443BCD =====
BOOL __userpurge sub_443B90@<eax>(int a1@<eax>, _DWORD *a2, int a3, int a4)
{
  int v4; // eax
  int v5; // esi

  v4 = sub_443270((int)dword_56674C, a1);
  v5 = v4;
  if ( v4 )
    *a2 = (*(int (__thiscall **)(int, int, int, int))(*(_DWORD *)v4 + 100))(v4, a3, a4, 1);
  return v5 != 0;
}

// ===== sub_443BD0 @ 0x00443BD0..0x00443BF6 =====
BOOL __usercall sub_443BD0@<eax>(int a1@<esi>)
{
  int v1; // eax

  v1 = sub_443270((int)dword_56674C, a1);
  return v1 && sub_41ACE0(v1) != 0;
}

// ===== sub_443C00 @ 0x00443C00..0x00443C4B =====
int __usercall sub_443C00@<eax>(int a1@<esi>)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_443270((int)dword_56674C, a1);
  if ( v1 )
  {
    v2 = (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 108))(v1);
    switch ( v2 )
    {
      case 0:
        return 0;
      case -2147483647:
        return 3;
      case -2147483646:
        return 4;
    }
  }
  return 255;
}

// ===== sub_443C50 @ 0x00443C50..0x00443C83 =====
int __userpurge sub_443C50@<eax>(int a1@<esi>, int a2)
{
  if ( !sub_443270((int)dword_56674C, a1) )
    return 255;
  sub_41B1A0(a2);
  return 0;
}

// ===== sub_443C90 @ 0x00443C90..0x00443D33 =====
int __userpurge sub_443C90@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  void *v4; // edi
  _DWORD *v5; // ebx
  int v6; // eax
  int v7; // edi

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_443270((int)dword_56674C, a1);
  if ( !v5 )
    return 255;
  v6 = sub_443270((int)v4, a2);
  v7 = v6;
  if ( !v6 )
    return 6;
  if ( v5 == (_DWORD *)v6 )
    return 7;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v6 + 8))(v6) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  if ( !sub_41AB40(v7, v5, a3, a4) )
    return 8;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  return 0;
}

// ===== sub_443D40 @ 0x00443D40..0x00443D97 =====
int __userpurge sub_443D40@<eax>(int a1@<eax>, int a2)
{
  void *v2; // edi
  int v3; // ebx
  int v4; // eax

  v2 = dword_56674C;
  v3 = sub_443270((int)dword_56674C, a1);
  if ( !v3 )
    return 255;
  v4 = sub_443270((int)v2, a2);
  if ( v4 )
    return sub_41AC40(v3, v4) != 0 ? 0 : 9;
  else
    return 6;
}

// ===== sub_443DA0 @ 0x00443DA0..0x00443DE7 =====
int __thiscall sub_443DA0(_DWORD *this)
{
  *this = &std::bad_alloc::`vftable';
  return sub_4AC3AD();
}

// ===== sub_443DF0 @ 0x00443DF0..0x00443E4E =====
_DWORD *__thiscall sub_443DF0(_DWORD *this, char a2)
{
  *this = &std::bad_alloc::`vftable';
  sub_4AC3AD();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_443E50 @ 0x00443E50..0x00443FF1 =====
int *__fastcall sub_443E50(int *a1, _DWORD *a2)
{
  int v2; // eax
  int v3; // esi
  int *v4; // esi
  double v5; // st7
  int v6; // edx
  int *v7; // eax
  double v8; // st4
  int v9; // eax
  int *v10; // edx
  double v11; // st3
  int v12; // edx
  unsigned int v13; // edi
  double *v14; // esi
  double *v15; // eax
  double v16; // st5
  double v17; // st5
  double v18; // st5
  double v19; // st5
  double v20; // st5
  int v21; // eax
  double v22; // st5
  double v23; // st5
  int *result; // eax
  int v25; // edx
  double v26; // st6
  double v27; // st6
  double v28[2]; // [esp+0h] [ebp-340h] BYREF
  double v29[101]; // [esp+10h] [ebp-330h]

  v2 = 0;
  v3 = ((a2[1] - *a2) >> 2) - 1;
  *a1 = v3;
  if ( v3 >= 0 )
  {
    v4 = a1 + 2;
    do
    {
      v5 = (double)*(int *)(*a2 + 4 * v2++);
      v4 += 2;
      *((double *)v4 - 1) = v5;
    }
    while ( v2 <= *a1 );
  }
  *(double *)&a1[2 * *a1 + 406] = 0.0;
  v6 = 1;
  *((double *)a1 + 203) = 0.0;
  if ( *a1 > 1 )
  {
    v7 = a1 + 6;
    do
    {
      ++v6;
      v8 = *((double *)v7 - 1) * 2.0;
      v7 += 2;
      *((double *)v7 + 200) = (*((double *)v7 - 3) - v8 + *((double *)v7 - 1)) * 3.0;
    }
    while ( v6 < *a1 );
  }
  v9 = 1;
  v29[0] = 0.0;
  if ( *a1 > 1 )
  {
    v10 = a1 + 408;
    do
    {
      v11 = 4.0 - v28[++v9];
      v10 += 2;
      *((double *)v10 - 1) = (*((double *)v10 - 1) - *((double *)v10 - 2)) / v11;
      v28[v9 + 1] = 1.0 / v11;
    }
    while ( v9 < *a1 );
  }
  v12 = *a1 - 1;
  if ( v12 >= 4 )
  {
    v13 = ((unsigned int)(*a1 - 5) >> 2) + 1;
    v14 = &v28[v12];
    v15 = (double *)&a1[2 * v12 + 406];
    v12 = *a1 - 1 - 4 * v13;
    do
    {
      v16 = v15[1];
      v15 -= 4;
      v17 = v16 * v14[2];
      v14 -= 4;
      --v13;
      v18 = v15[4] - v17;
      v15[4] = v18;
      v19 = v15[3] - v18 * v14[5];
      v15[3] = v19;
      v20 = v15[2] - v19 * v14[4];
      v15[2] = v20;
      v15[1] = v15[1] - v20 * v14[3];
    }
    while ( v13 );
  }
  if ( v12 > 0 )
  {
    v21 = (int)&a1[2 * v12 + 406];
    do
    {
      v22 = v29[v12--];
      v23 = v22 * *(double *)(v21 + 8);
      v21 -= 8;
      *(double *)(v21 + 8) = *(double *)(v21 + 8) - v23;
    }
    while ( v12 > 0 );
  }
  *(double *)&a1[2 * *a1 + 608] = 0.0;
  result = (int *)*a1;
  v25 = 0;
  *(double *)&a1[2 * *a1 + 204] = 0.0;
  if ( *a1 > 0 )
  {
    result = a1 + 608;
    do
    {
      ++v25;
      v26 = *((double *)result - 100) - *((double *)result - 101);
      result += 2;
      v27 = v26 / 3.0;
      *((double *)result - 1) = v27;
      *((double *)result - 203) = *((double *)result - 303)
                                - *((double *)result - 304)
                                - *((double *)result - 102)
                                - v27;
    }
    while ( v25 < *a1 );
  }
  return result;
}
