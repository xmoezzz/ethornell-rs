#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_401000 @ 0x00401000..0x004010A2 =====
int __cdecl sub_401000(int a1, int a2)
{
  int result; // eax
  long double v3; // st7

  result = a2;
  if ( a1 )
  {
    if ( a1 <= 0 )
    {
      return 11796480 - (int)(atan((double)a2 / (double)a1) * -11796480.0 / 3.141592653589793);
    }
    else
    {
      v3 = (double)a2 / (double)a1;
      if ( a2 < 0 )
        return 23592960 - (int)(atan(v3) * -11796480.0 / 3.141592653589793);
      else
        return (int)(atan(v3) * 11796480.0 / 3.141592653589793);
    }
  }
  else if ( a2 )
  {
    if ( a2 <= 0 )
      return 17694720;
    else
      return 5898240;
  }
  return result;
}

// ===== sub_4010B0 @ 0x004010B0..0x00401104 =====
double __cdecl sub_4010B0(double X, double a2)
{
  double result; // st7

  result = floor(X);
  if ( 1.0 - a2 <= X - result )
    return ceil(X);
  if ( a2 < X - result )
    return X;
  return result;
}

// ===== sub_401110 @ 0x00401110..0x00401126 =====
void sub_401110()
{
  InitializeCriticalSection(&CriticalSection);
  dword_565A90 = 1;
}

// ===== sub_401130 @ 0x00401130..0x0040113C =====
void sub_401130()
{
  EnterCriticalSection(&CriticalSection);
}

// ===== sub_401140 @ 0x00401140..0x0040114C =====
void sub_401140()
{
  LeaveCriticalSection(&CriticalSection);
}

// ===== sub_401150 @ 0x00401150..0x004011AA =====
void sub_401150()
{
  _DWORD *v0; // esi

  sub_401130();
  v0 = dword_565AA8;
  if ( dword_565AA8 )
  {
    do
    {
      sub_401490();
      v0 = (_DWORD *)v0[5];
    }
    while ( v0 );
  }
  sub_401140();
  while ( dword_565AA8 )
    Sleep(1u);
  dword_565A90 = 0;
  DeleteCriticalSection(&CriticalSection);
}

// ===== sub_4011B0 @ 0x004011B0..0x00401387 =====
int __cdecl sub_4011B0(_DWORD *a1, unsigned int a2)
{
  LPCSTR lpFileName; // ecx
  const char *v3; // edi
  int v5; // ecx
  void *v6; // esi
  void (__thiscall ***v7)(_DWORD, int); // esi
  int v8; // eax
  _DWORD *v9; // edi
  _BYTE *v10; // eax
  char *v11; // ecx
  _BYTE *v12; // edx
  char v13; // al
  int v14; // esi
  void *v15; // [esp+Ch] [ebp-324h]
  char v16[780]; // [esp+14h] [ebp-31Ch] BYREF
  int v17; // [esp+32Ch] [ebp-4h]

  v3 = lpFileName;
  if ( a2 > 2 )
    return -2147483647;
  sub_401130();
  strcpy(v16, v3);
  sub_42EA80(v5, v16);
  v6 = dword_565AA8;
  if ( dword_565AA8 )
  {
    while ( strcmp(v16, *((const char **)v6 + 1)) )
    {
      v6 = (void *)*((_DWORD *)v6 + 5);
      if ( !v6 )
        goto LABEL_6;
    }
    v14 = -2147483646;
  }
  else
  {
LABEL_6:
    v15 = operator new(0xCu);
    v17 = 0;
    if ( v15 )
      v7 = (void (__thiscall ***)(_DWORD, int))sub_42D3B0();
    else
      v7 = 0;
    v17 = -1;
    if ( a2 )
    {
      if ( a2 == 1 )
        v8 = sub_42D570(v3, 0);
      else
        v8 = sub_42D570(v3, 1);
    }
    else
    {
      v8 = sub_42D520();
    }
    if ( v8 )
    {
      v9 = operator new(0x18u);
      *v9 = ++dword_565A94;
      v10 = operator new[](strlen(v16) + 1);
      v9[1] = v10;
      v11 = v16;
      v12 = v10;
      do
      {
        v13 = *v11;
        *v12++ = *v11++;
      }
      while ( v13 );
      v9[2] = v7;
      v9[3] = a2;
      v9[4] = 0;
      v9[5] = dword_565AA8;
      dword_565AA8 = v9;
      *a1 = *v9;
      v14 = 0;
    }
    else
    {
      if ( v7 )
        (**v7)(v7, 1);
      v14 = -2147483645;
    }
  }
  sub_401140();
  return v14;
}

// ===== sub_401390 @ 0x00401390..0x004013AC =====
void **__thiscall sub_401390(void *this)
{
  void **result; // eax

  result = (void **)dword_565AA8;
  if ( dword_565AA8 )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[5];
    }
    while ( result );
  }
  return result;
}

// ===== sub_4013B0 @ 0x004013B0..0x004013FF =====
int __fastcall sub_4013B0(int a1, int a2)
{
  void *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = dword_565AA8;
  result = -2147483644;
  v4 = &dword_565A94;
  if ( dword_565AA8 )
  {
    while ( a2 != *(_DWORD *)v2 )
    {
      v4 = (int *)v2;
      v2 = (void *)*((_DWORD *)v2 + 5);
      if ( !v2 )
        return result;
    }
    v4[5] = *((_DWORD *)v2 + 5);
    v5 = (void (__thiscall ***)(_DWORD, int))*((_DWORD *)v2 + 2);
    if ( v5 )
      (**v5)(v5, 1);
    operator delete[](*((void **)v2 + 1));
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_401400 @ 0x00401400..0x00401481 =====
int __usercall sub_401400@<eax>(_DWORD *a1@<edi>, void *a2, int a3, int a4, int a5)
{
  _DWORD *v5; // eax
  _DWORD *v6; // esi
  _DWORD *v7; // eax

  sub_401130();
  if ( sub_401390(a2) )
  {
    v5 = dword_565AC0;
    v6 = &unk_565AAC;
    if ( dword_565AC0 )
    {
      do
      {
        v6 = v5;
        v5 = (_DWORD *)v5[5];
      }
      while ( v5 );
    }
    v7 = operator new(0x18u);
    v7[2] = a3;
    *v7 = a1;
    v7[1] = a2;
    v7[3] = a4;
    v7[4] = a5;
    v7[5] = 0;
    v6[5] = v7;
    if ( a1 )
      *a1 = 0;
    sub_401140();
    return 0;
  }
  else
  {
    sub_401140();
    return -2147483644;
  }
}

// ===== sub_401490 @ 0x00401490..0x004014A0 =====
int __usercall sub_401490@<eax>(void *a1@<eax>, _DWORD *a2@<edi>)
{
  return sub_401400(a2, a1, 0, 0, 1);
}

// ===== sub_4014A0 @ 0x004014A0..0x004014B5 =====
int __usercall sub_4014A0@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3@<edi>, void *a4)
{
  return sub_401400(a3, a4, a2, a1, 0);
}

// ===== sub_4014C0 @ 0x004014C0..0x004014CF =====
int __usercall sub_4014C0@<eax>(int a1@<eax>, void *a2@<ecx>, _DWORD *a3@<edi>)
{
  return sub_401400(a3, a2, 0, a1, 0);
}

// ===== sub_4014D0 @ 0x004014D0..0x004015F3 =====
int sub_4014D0()
{
  int result; // eax
  _DWORD *v1; // edi
  void **v2; // eax
  DWORD *v3; // esi
  int v4; // ecx
  void *v5; // edx
  int v6; // ebx
  DWORD v7; // eax
  unsigned int v8; // eax
  unsigned int v9; // ecx

  result = -1;
  if ( dword_565A90 )
  {
    sub_401130();
    v1 = dword_565AC0;
    if ( dword_565AC0 )
    {
      v2 = sub_401390(*((void **)dword_565AC0 + 1));
      v3 = (DWORD *)v2;
      if ( !v2 )
      {
        if ( *v1 )
          *(_DWORD *)*v1 = -1;
        goto LABEL_23;
      }
      if ( v1[4] )
      {
        sub_42D5B0();
        sub_4013B0(v4, v1[1]);
        if ( *v1 )
          *(_DWORD *)*v1 = 1;
LABEL_23:
        dword_565AC0 = (void *)v1[5];
        operator delete(v1);
        sub_401140();
        return 0;
      }
      v2[4] = (void *)1;
      sub_401140();
      v5 = (void *)v1[2];
      v6 = 0;
      if ( v5 )
      {
        v7 = v3[3];
        if ( v7 )
        {
          if ( v7 - 1 <= 1 )
            v6 = sub_42D600(v1[3], v5, v3[2]);
        }
        else
        {
          v6 = sub_42D5D0(v1[3], v5, v3[2]);
        }
        goto LABEL_18;
      }
      v8 = sub_42D650();
      v9 = v1[3];
      if ( v9 != -1 )
      {
        if ( v9 > v8 )
        {
LABEL_18:
          sub_401130();
          v3[4] = 0;
          if ( !v6 )
            v6 = -1;
          *(_DWORD *)*v1 = v6;
          goto LABEL_23;
        }
        v8 = v1[3];
      }
      if ( v8 != -1 && sub_42D630(v3[2]) )
        v6 = 1;
      goto LABEL_18;
    }
    sub_401140();
    return 1;
  }
  return result;
}

// ===== sub_401600 @ 0x00401600..0x00401669 =====
int __usercall sub_401600@<eax>(__int64 a1@<edx:eax>)
{
  dword_565AC4 = a1;
  if ( (_DWORD)a1 )
  {
    a1 = 96 * sub_46F380(0) / 100;
    qword_565AC8 = a1;
    qword_565AD0 = 0LL;
    dword_565AD8 = 0;
    dword_565ADC = 0;
    dword_50A858 = 0;
    dword_50A85C = 0;
  }
  return a1;
}

// ===== sub_401670 @ 0x00401670..0x004017DC =====
int __usercall sub_401670@<eax>(int result@<eax>, int *a2@<esi>)
{
  double v2; // st7
  double v3; // [esp+0h] [ebp-14h]
  unsigned __int16 v4; // [esp+12h] [ebp-2h]

  switch ( result )
  {
    case 0:
      result = dword_565AD8;
      *a2 = dword_565AD8;
      break;
    case 1:
      if ( !dword_565AD8 )
        goto LABEL_9;
      v3 = (double)qword_565AD0;
      v2 = (double)sub_498860();
      result = v4 | 0xC00;
      *a2 = (__int64)(v3 / (v2 * (double)(unsigned int)dword_565AD8) * 1000000.0);
      break;
    case 2:
      if ( qword_565AD0 <= 0 )
        goto LABEL_9;
      result = (__int64)((double)(qword_565AC8 >> 1)
                       * (double)(unsigned int)dword_565AD8
                       / (double)qword_565AD0
                       * 10000.0);
      *a2 = result;
      break;
    case 3:
      result = dword_565AD8;
      if ( !dword_565AD8 )
        goto LABEL_9;
      result = v4 | 0xC00;
      *a2 = (__int64)((double)(unsigned int)dword_565ADC
                    * (double)(unsigned int)dword_565ADC
                    / ((double)(unsigned int)dword_565AD8
                     * (double)(unsigned int)dword_565AD8)
                    * 10000.0);
      break;
    default:
LABEL_9:
      *a2 = 0;
      break;
  }
  return result;
}

// ===== sub_4017F0 @ 0x004017F0..0x00401828 =====
void sub_4017F0()
{
  if ( dword_565AC4 )
  {
    qword_50A860 = sub_4988A0();
    if ( sub_45F690(&dword_50A868) == -2130706432 )
      dword_50A868 = -1;
  }
}

// ===== sub_401830 @ 0x00401830..0x004018FE =====
void __cdecl sub_401830(int a1)
{
  __int64 v1; // rdi
  unsigned int v2; // eax
  unsigned int v3; // [esp+Ch] [ebp-4h] BYREF

  if ( dword_565AC4 )
  {
    v1 = qword_50A858 - qword_50A860 + sub_4988A0();
    if ( a1 )
    {
      qword_565AD0 += v1;
      if ( sub_45F690(&v3) == -2130706432 )
        v2 = -1;
      else
        v2 = v3;
      if ( (dword_50A868 == -1 || v2 == -1 || dword_50A868 <= v2) && v1 < qword_565AC8 )
        ++dword_565ADC;
      ++dword_565AD8;
      qword_50A858 = 0LL;
    }
    else
    {
      qword_50A858 = v1;
    }
  }
}

// ===== sub_401900 @ 0x00401900..0x00401974 =====
int __usercall sub_401900@<eax>(int result@<eax>, int a2@<edi>, unsigned int a3)
{
  unsigned int v3; // esi
  char v4; // cl
  char v5; // dl
  char v6; // bl
  char v7; // dl
  char v8; // [esp+5h] [ebp-3h]
  char v9; // [esp+6h] [ebp-2h]
  char v10; // [esp+7h] [ebp-1h]

  v3 = 0;
  if ( a3 )
  {
    v9 = *(_BYTE *)(result + 5);
    v10 = *(_BYTE *)(result + 4);
    v4 = *(_BYTE *)(result + 6);
    v8 = *(_BYTE *)(result + 7);
    do
    {
      *(_DWORD *)result = *(unsigned __int8 *)(v3 + a2) + 233 * *(_DWORD *)result;
      v5 = *(_BYTE *)result + v10;
      v6 = *(_BYTE *)result ^ v9;
      *(_BYTE *)(result + 4) = v5;
      *(_BYTE *)(result + 5) = v6;
      v4 += *(_BYTE *)(v3 + a2);
      v10 = v5;
      *(_BYTE *)(result + 6) = v4;
      v7 = *(_BYTE *)(v3 + a2) ^ v8;
      ++v3;
      v9 = v6;
      v8 = v7;
      *(_BYTE *)(result + 7) = v7;
    }
    while ( v3 < a3 );
  }
  return result;
}

// ===== sub_401980 @ 0x00401980..0x00401A4F =====
int __cdecl sub_401980(_DWORD *a1)
{
  int v1; // edi
  unsigned int v2; // ebx
  void *v3; // edi
  DWORD v4; // esi
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-18h] BYREF
  int v7; // [esp+24h] [ebp-4h]

  v1 = 0;
  sub_42D3B0();
  v7 = 0;
  if ( sub_42D520() )
  {
    v2 = sub_42D650();
    v3 = operator new(0x10000u);
    *a1 = 0;
    for ( a1[1] = 0; v2; v2 -= v4 )
    {
      v4 = v2;
      if ( v2 >= 0x10000 )
        v4 = 0x10000;
      sub_42D5D0(v4, v3, (DWORD)NumberOfBytesRead);
      sub_401900((int)a1, (int)v3, v4);
    }
    operator delete(v3);
    sub_42D5B0();
    v1 = 1;
  }
  v7 = -1;
  sub_42D400(NumberOfBytesRead);
  return v1;
}

// ===== sub_401A50 @ 0x00401A50..0x00401B67 =====
int sub_401A50()
{
  unsigned int v0; // eax
  int v1; // ecx
  void *v2; // edi
  int v3; // esi
  int v4; // esi
  DWORD NumberOfBytesRead[3]; // [esp+Ch] [ebp-2Ch] BYREF
  _DWORD Buffer[3]; // [esp+18h] [ebp-20h] BYREF
  int v8; // [esp+24h] [ebp-14h]
  int v9; // [esp+34h] [ebp-4h]

  sub_42D3B0();
  v9 = 0;
  if ( sub_42D520() )
  {
    if ( sub_42D5D0(0x10u, Buffer, (DWORD)NumberOfBytesRead) == 16 )
    {
      v0 = 8;
      v1 = 0;
      while ( Buffer[v1] == dword_4E412C[v1] )
      {
        v0 -= 4;
        ++v1;
        if ( v0 < 4 )
        {
          v2 = operator new(v8 << 6);
          v3 = v8 << 6;
          if ( sub_42D5D0(v8 << 6, v2, (DWORD)NumberOfBytesRead) == v3 )
          {
            dword_56685C = v8;
            dword_566860 = v2;
            v4 = 0;
          }
          else
          {
            operator delete(v2);
            v4 = -2147483645;
          }
          goto LABEL_11;
        }
      }
    }
    v4 = -2147483646;
  }
  else
  {
    v4 = -2147483647;
  }
LABEL_11:
  v9 = -1;
  sub_42D400(NumberOfBytesRead);
  return v4;
}

// ===== sub_401B70 @ 0x00401B70..0x00401B75 =====
// attributes: thunk
int sub_401B70()
{
  return sub_407BB0();
}

// ===== sub_401B80 @ 0x00401B80..0x00401C10 =====
int __cdecl sub_401B80(int a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // ecx
  const CHAR *v7; // eax
  int v8; // eax
  int v10; // [esp+4h] [ebp-1Ch] BYREF

  if ( !sub_407F20(v6, dword_566750) )
    return -2147483646;
  v7 = (const CHAR *)sub_468BB0();
  v8 = sub_409290(100, (int)dword_566750, (int)&v10, v7, a5);
  if ( v8 )
  {
    if ( v8 == -2147483646 )
      return -2147483647;
    else
      return v10;
  }
  else
  {
    sub_40A320(a1, a2, a3, a4, v10, a6);
    return 0;
  }
}

// ===== sub_401C10 @ 0x00401C10..0x00401C6B =====
int __fastcall sub_401C10(int a1, int a2)
{
  int result; // eax

  result = -1;
  switch ( *(_WORD *)(a2 + 4) )
  {
    case 8:
      result = 3;
      break;
    case 0x10:
      result = 0;
      break;
    case 0x18:
      result = 1;
      break;
    case 0x20:
      switch ( *(_WORD *)(a2 + 8) )
      {
        case 4:
          result = 4;
          break;
        case 5:
          result = 5;
          break;
        case 7:
          result = 7;
          break;
        default:
          result = 2;
          break;
      }
      break;
    case 0x30:
      result = 6;
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_401CB0 @ 0x00401CB0..0x00401CD9 =====
int __fastcall sub_401CB0(int a1, int a2, int a3, int a4, size_t Size)
{
  void *v5; // edx

  if ( sub_401C10(a1, a2) == -1 )
    return 1;
  sub_450380(a4, v5, Size);
  return 0;
}

// ===== sub_401CE0 @ 0x00401CE0..0x00401DE2 =====
int __usercall sub_401CE0@<eax>(int a1@<edi>, int a2, int a3, int a4)
{
  void *v4; // esi
  int v5; // eax
  int v6; // eax
  int v8; // [esp+8h] [ebp-Ch]
  int v9; // [esp+Ch] [ebp-8h]
  size_t Size; // [esp+10h] [ebp-4h] BYREF

  v9 = -1;
  v8 = 0;
  if ( sub_450480(&Size, a3, a1, 0) )
  {
    v4 = operator new[](Size);
    sub_450480(&Size, a3, a1, a4);
    v8 = a4;
  }
  else
  {
    if ( !sub_439950(a3, (int)&Size, 0, a1) )
      return v9;
    v4 = operator new[](Size);
    sub_439950(a3, (int)&Size, v4, a1);
  }
  if ( v4 )
  {
    v5 = sub_402330(a2);
    if ( v5 )
    {
      v6 = v5 - 1;
      if ( !v6 )
      {
        operator delete[](v4);
        return -2147483644;
      }
      if ( v6 == 1 )
      {
        operator delete[](v4);
        return -2147483640;
      }
    }
    else
    {
      if ( v8 )
        sub_439930(v4, a1, a3, Size);
      v9 = 0;
    }
    operator delete[](v4);
  }
  return v9;
}

// ===== sub_401DF0 @ 0x00401DF0..0x00401DFD =====
int sub_401DF0()
{
  return sub_4504F0();
}

// ===== sub_401E00 @ 0x00401E00..0x00401ECE =====
int __usercall sub_401E00@<eax>(int a1@<eax>, int a2, int a3)
{
  void *v4; // esi
  int v5; // edi
  int v6; // eax
  int v7; // eax
  int v9; // [esp+Ch] [ebp-4h] BYREF

  v4 = operator new[](0x4000000u);
  if ( !sub_439950(a3, (int)&v9, v4, a1) )
  {
    v9 = sub_465AB0(a1, a3);
    sub_439930(v4, a1, a3, v9);
  }
  if ( v9 )
  {
    v5 = sub_402110(a2, v4);
    if ( v5 == -2147483647 )
    {
      v6 = sub_402330(a2);
      if ( !v6 )
      {
        operator delete[](v4);
        return 0;
      }
      v7 = v6 - 1;
      if ( !v7 )
      {
        operator delete[](v4);
        return -2147483644;
      }
      if ( v7 == 1 )
      {
        operator delete[](v4);
        return -2147483640;
      }
    }
  }
  else
  {
    v5 = 0;
  }
  operator delete[](v4);
  return v5;
}

// ===== sub_401ED0 @ 0x00401ED0..0x00401EEC =====
int __usercall sub_401ED0@<eax>(int a1@<ecx>, size_t a2@<eax>, int a3, void *Src)
{
  return (sub_439930(Src, a3, a1, a2) != 0) - 1;
}

// ===== sub_401EF0 @ 0x00401EF0..0x0040201E =====
int __usercall sub_401EF0@<eax>(int a1@<edi>, int a2)
{
  int v2; // esi
  int v3; // ecx
  char *v4; // esi
  int v5; // eax
  int result; // eax
  _WORD v7[2]; // [esp+4h] [ebp-34h] BYREF
  __int16 v8; // [esp+8h] [ebp-30h]
  unsigned __int16 v9; // [esp+Ah] [ebp-2Eh]
  char v10; // [esp+14h] [ebp-24h] BYREF

  v2 = -1;
  switch ( sub_467F50(v7, a2, 0, 48) )
  {
    case 0:
      if ( sub_402030() )
      {
        v4 = &v10;
      }
      else
      {
        v4 = (char *)v7;
        if ( !v7[0] || !v7[1] || v8 != 8 && v8 != 24 && v8 != 32 || v9 > 1u )
          goto LABEL_17;
      }
      sub_401C10(v3, (int)v4);
      if ( sub_4026C0(a1, *(unsigned __int16 *)v4) )
      {
        if ( *((_WORD *)v4 + 5) == 1 )
        {
          v5 = sub_407F00(a1, dword_566750);
          if ( v5 )
          {
            *(_DWORD *)(v5 + 40) = *((unsigned __int16 *)v4 + 6);
            *(_DWORD *)(v5 + 44) = *((unsigned __int16 *)v4 + 7);
          }
        }
        sub_4026E0(a1);
        result = 0;
      }
      else
      {
        result = -2147483639;
      }
      break;
    case 1:
      result = -1;
      break;
    case 2:
    case 3:
LABEL_17:
      v2 = -2147483635;
      goto LABEL_18;
    default:
LABEL_18:
      result = v2;
      break;
  }
  return result;
}

// ===== sub_402030 @ 0x00402030..0x0040206C =====
BOOL __usercall sub_402030@<eax>(const char *a1@<eax>)
{
  return strcmp(a1, "CompressedBG___") == 0;
}

// ===== sub_402070 @ 0x00402070..0x00402080 =====
int __usercall sub_402070@<eax>(int result@<eax>)
{
  dword_565AE0 = result;
  dword_565AE4 = 0;
  return result;
}

// ===== sub_402080 @ 0x00402080..0x004020D0 =====
int sub_402080()
{
  int result; // eax

  result = sub_46DE30();
  if ( dword_565AE0 )
  {
    if ( !dword_565AE4 )
    {
      dword_565AE4 = dword_565AE0 + sub_498720();
      return 1;
    }
    if ( result || dword_565AE4 > (unsigned int)sub_498720() )
      return 1;
    result = 0;
    dword_565AE4 = 0;
  }
  else if ( !result )
  {
    dword_565AE4 = 0;
  }
  return result;
}

// ===== sub_4020D0 @ 0x004020D0..0x00402110 =====
int __cdecl sub_4020D0(int a1)
{
  void *v1; // esi
  int v2; // ebx

  v1 = operator new[](0x4000000u);
  v2 = -1;
  if ( sub_4659C0(0) )
    v2 = sub_402110(a1, v1);
  operator delete[](v1);
  return v2;
}

// ===== sub_402110 @ 0x00402110..0x004022FD =====
int __cdecl sub_402110(int a1, int a2)
{
  int v2; // edi
  int result; // eax
  unsigned int v4; // edx
  int v5; // ecx
  int v6; // eax
  unsigned int v7; // ebx
  size_t v8; // esi
  char *v9; // eax
  int v10; // edx
  void *v11; // ecx
  char *v12; // eax
  char *v13; // edi
  int v14; // esi
  int v15; // [esp+4h] [ebp-20h]
  int v16; // [esp+8h] [ebp-1Ch]
  void *v17; // [esp+Ch] [ebp-18h]
  char *v18; // [esp+10h] [ebp-14h]
  int v19; // [esp+14h] [ebp-10h]
  int v20; // [esp+18h] [ebp-Ch]
  int v21; // [esp+1Ch] [ebp-8h]
  unsigned int v22; // [esp+20h] [ebp-4h] BYREF

  v2 = a2;
  LOWORD(v22) = 0;
  BYTE2(v22) = 0;
  LOWORD(v22) = *(_WORD *)a2;
  if ( strcmp((const char *)&v22, "BM") )
    return -2147483647;
  if ( *(_DWORD *)(a2 + 14) != 40 )
    return -2147483646;
  if ( *(_WORD *)(a2 + 26) != 1 )
    return -2147483645;
  v4 = *(unsigned __int16 *)(a2 + 28);
  switch ( *(_WORD *)(a2 + 28) )
  {
    case 8:
      v21 = 3;
      goto LABEL_12;
    case 0x10:
      v21 = 0;
      goto LABEL_12;
    case 0x18:
      v21 = 7;
      goto LABEL_12;
    case 0x20:
      v21 = 2;
LABEL_12:
      if ( *(_DWORD *)(a2 + 30) )
      {
        result = -2147483643;
      }
      else
      {
        v5 = *(_DWORD *)(a2 + 18);
        if ( v5 && (v6 = *(_DWORD *)(a2 + 22)) != 0 )
        {
          if ( v4 == 24 )
            v7 = 4;
          else
            v7 = v4 >> 3;
          v8 = v4 >> 3;
          v19 = v7 * v5;
          v9 = (char *)operator new[](v7 * v5 * v6);
          v10 = *(_DWORD *)(a2 + 22);
          v11 = v9;
          v12 = &v9[v19 * (v10 - 1)];
          v17 = v11;
          v18 = v12;
          v16 = a2 + *(_DWORD *)(a2 + 10);
          v22 = 0;
          if ( v10 )
          {
            do
            {
              v15 = --v10;
              v13 = v12;
              v20 = *(_DWORD *)(a2 + 18);
              if ( v20 )
              {
                do
                {
                  --v20;
                  memcpy_0(v13, (const void *)(v16 + v22), v8);
                  if ( v8 < v7 )
                    memset(&v13[v8], 0, v7 - v8);
                  v22 += v8;
                  v13 += v7;
                }
                while ( v20 );
                v10 = v15;
                v12 = v18;
              }
              v12 -= v19;
              v18 = v12;
              v22 = (v22 + 3) & 0xFFFFFFFC;
            }
            while ( v10 );
            v2 = a2;
          }
          v14 = sub_402420(a1, *(_DWORD *)(v2 + 18), *(_DWORD *)(v2 + 22), v21);
          operator delete[](v17);
          result = v14 != 0 ? 0 : -2147483641;
        }
        else
        {
          result = -2147483642;
        }
      }
      break;
    default:
      result = -2147483644;
      break;
  }
  return result;
}

// ===== sub_402330 @ 0x00402330..0x00402415 =====
int __usercall sub_402330@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  unsigned __int16 *v3; // esi
  unsigned __int16 *v4; // edi
  int v5; // ebx
  int v7; // [esp+Ch] [ebp-Ch]

  v3 = (unsigned __int16 *)a1;
  v4 = 0;
  v7 = 0;
  v5 = sub_401C10(a2, a1);
  if ( v5 != -1 )
  {
    if ( v3[3] )
    {
      if ( v3[3] != 1 )
      {
        operator delete[](0);
        return 1;
      }
      v4 = (unsigned __int16 *)operator new[](*v3 * v3[1] * (v3[2] >> 3) + 16);
      if ( !sub_4058D0() )
        goto LABEL_9;
      v3 = v4;
    }
    if ( !sub_402420(a3, *v3, v3[1], v5) )
      v7 = 2;
LABEL_9:
    operator delete[](v4);
    return v7;
  }
  return 1;
}

// ===== sub_402420 @ 0x00402420..0x0040243B =====
int __usercall sub_402420@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_4080B0(a3, a4, a6, a2, a1);
}

// ===== sub_402440 @ 0x00402440..0x0040246C =====
int __cdecl sub_402440(int a1, int a2)
{
  int v2; // ecx
  int v3; // eax

  v3 = sub_407F00(v2, dword_566750);
  if ( !v3 )
    return 0;
  *(_DWORD *)(v3 + 40) = a1;
  *(_DWORD *)(v3 + 44) = a2;
  return 1;
}

// ===== sub_402470 @ 0x00402470..0x00402496 =====
int __usercall sub_402470@<eax>(int a1@<ecx>, _DWORD *a2@<esi>)
{
  int v2; // eax
  int v3; // edx
  int result; // eax

  v2 = sub_407F00(a1, dword_566750);
  if ( !v2 )
    return 0;
  *a2 = *(_DWORD *)(v2 + 40);
  v3 = *(_DWORD *)(v2 + 44);
  result = 1;
  a2[1] = v3;
  return result;
}

// ===== sub_4024A0 @ 0x004024A0..0x004025DF =====
int __cdecl sub_4024A0(int a1, int a2)
{
  int v2; // ecx
  unsigned int j; // edx
  int v5; // esi
  unsigned int k; // eax
  int v7; // esi
  unsigned int i; // eax
  int v9; // [esp+Ch] [ebp-1Ch]
  int v10; // [esp+10h] [ebp-18h]
  unsigned int v11; // [esp+14h] [ebp-14h]
  int v12; // [esp+18h] [ebp-10h]
  int v13; // [esp+1Ch] [ebp-Ch]
  int v14; // [esp+20h] [ebp-8h]
  int v15; // [esp+24h] [ebp-4h]

  if ( !sub_402710() )
    return 1;
  if ( v14 != 4 )
    return 2;
  v2 = v9;
  if ( v13 == 1 )
  {
    if ( v12 )
    {
      v7 = v12;
      do
      {
        for ( i = 0; i < v11; ++i )
        {
          if ( (*(_DWORD *)(v2 + 4 * i) & 0xFFFFFF) == (a1 & 0xFFFFFF) )
            *(_DWORD *)(v2 + 4 * i) = a2 & 0xFFFFFF;
        }
        v2 += v10;
        --v7;
      }
      while ( v7 );
    }
    return 0;
  }
  if ( v13 != 2 )
    return 0;
  if ( (a1 & 0xFF000000) == 0 )
  {
    if ( v12 )
    {
      v15 = v12;
      do
      {
        for ( j = 0; j < v11; ++j )
        {
          if ( (*(_DWORD *)(v2 + 4 * j) & 0xFFFFFF) == (a1 & 0xFFFFFF) )
            *(_DWORD *)(v2 + 4 * j) = a2 & 0xFFFFFF | *(_DWORD *)(v2 + 4 * j) & 0xFF000000;
        }
        v2 += v10;
        --v15;
      }
      while ( v15 );
      return 0;
    }
    return 0;
  }
  if ( !v12 )
    return 0;
  v5 = v12;
  do
  {
    for ( k = 0; k < v11; ++k )
    {
      if ( *(_DWORD *)(v2 + 4 * k) == a1 )
        *(_DWORD *)(v2 + 4 * k) = a2;
    }
    v2 += v10;
    --v5;
  }
  while ( v5 );
  return 0;
}

// ===== sub_4025E0 @ 0x004025E0..0x0040265C =====
int __usercall sub_4025E0@<eax>(_DWORD *a1@<edi>, int a2, int a3)
{
  int v4; // [esp+8h] [ebp-18h]
  int v5; // [esp+Ch] [ebp-14h]
  int v6; // [esp+10h] [ebp-10h]
  int v7; // [esp+14h] [ebp-Ch]
  size_t Size; // [esp+1Ch] [ebp-4h]

  if ( !sub_402710() )
    return 1;
  if ( Size > 4 )
    return 2;
  if ( a2 < 0 || a2 >= v6 || a3 < 0 || a3 >= v7 )
    return 3;
  *a1 = 0;
  memcpy_0(a1, (const void *)(v4 + a3 * v5 + a2 * Size), Size);
  return 0;
}

// ===== sub_402660 @ 0x00402660..0x00402696 =====
int __thiscall sub_402660(void *this)
{
  int v1; // eax

  v1 = sub_4081B0();
  switch ( v1 )
  {
    case 0:
      return 0;
    case 11:
      return 1;
    case 21:
      return 2;
  }
  return (int)this;
}

// ===== sub_4026A0 @ 0x004026A0..0x004026B2 =====
int __usercall sub_4026A0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_408200(a2, a1);
}

// ===== sub_4026C0 @ 0x004026C0..0x004026D9 =====
int __usercall sub_4026C0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_407DA0(a4, a2, a1);
}

// ===== sub_4026E0 @ 0x004026E0..0x004026E7 =====
int __usercall sub_4026E0@<eax>(int a1@<eax>)
{
  return sub_408040(a1);
}

// ===== sub_4026F0 @ 0x004026F0..0x00402703 =====
int sub_4026F0()
{
  return sub_407CF0(dword_566750);
}

// ===== sub_402710 @ 0x00402710..0x0040271B =====
int __thiscall sub_402710(void *this)
{
  return sub_407F20(this, dword_566750);
}

// ===== sub_402720 @ 0x00402720..0x004027C6 =====
int __cdecl sub_402720(int a1, int a2, int a3, int a4)
{
  int v4; // ecx
  int result; // eax

  if ( !sub_407F20(v4, dword_566750) )
    return 1;
  if ( !sub_407F20(a2, dword_566750) )
    return 2;
  switch ( sub_40A530(a1, a3, a4) )
  {
    case 0:
      result = 0;
      break;
    case 1:
      result = 3;
      break;
    case 2:
      result = 4;
      break;
    case 3:
      result = 5;
      break;
    case 4:
      result = 6;
      break;
    default:
      result = 7;
      break;
  }
  return result;
}

// ===== sub_4027E0 @ 0x004027E0..0x0040294A =====
int __usercall sub_4027E0@<eax>(int a1@<ecx>, int a2@<edi>, int a3, int a4, int a5, int a6, int a7)
{
  int result; // eax
  int v8; // eax
  _BYTE v9[16]; // [esp+8h] [ebp-58h] BYREF
  _BYTE v10[24]; // [esp+18h] [ebp-48h] BYREF
  _BYTE v11[24]; // [esp+30h] [ebp-30h] BYREF
  _BYTE v12[24]; // [esp+48h] [ebp-18h] BYREF

  if ( !sub_407F20(a1, dword_566750) )
    return 1;
  if ( !sub_407F20(a4, dword_566750) )
    return 2;
  if ( sub_407F20(a5, dword_566750) )
  {
    switch ( sub_411990(v11, a3, a2, a6, a7, 0, 0) )
    {
      case 0:
        return 0;
      case 1:
        result = 4;
        break;
      case 3:
        result = 8;
        break;
      case 4:
        return 7;
      case 7:
        result = 5;
        break;
      case 8:
        result = 6;
        break;
      default:
        result = 9;
        break;
    }
  }
  else
  {
    sub_409190(v11);
    v8 = sub_409190(v12);
    sub_409170(v8);
    if ( sub_409110(v10) )
    {
      sub_4091B0(v9);
      sub_409170(v9);
      sub_4091B0(v9);
      sub_40C0F0(v11, v11, v12, a7, 1);
      return 0;
    }
    else
    {
      return 7;
    }
  }
  return result;
}

// ===== sub_402970 @ 0x00402970..0x00402A97 =====
int __usercall sub_402970@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  int v7; // ebx
  int result; // eax
  _BYTE v9[24]; // [esp+10h] [ebp-60h] BYREF
  _BYTE v10[24]; // [esp+58h] [ebp-18h] BYREF

  v7 = 0;
  if ( sub_407F20(a2, dword_566750) )
  {
    if ( sub_407F20(a3, dword_566750) )
    {
      if ( sub_407F20(a4, dword_566750) )
      {
        if ( (a1 != -1 ? (unsigned int)v10 : 0) == 0 || sub_407F20(a1, dword_566750) )
        {
          switch ( sub_412220(v9, a1 != -1 ? v10 : 0, a5, a6) )
          {
            case 0:
              result = 0;
              break;
            case 1:
              result = 3;
              break;
            case 3:
              result = 8;
              break;
            case 0xC:
              result = 5;
              break;
            case 0xD:
              result = 7;
              break;
            default:
              return v7;
          }
        }
        else
        {
          return 6;
        }
      }
      else
      {
        return 4;
      }
    }
    else
    {
      return 2;
    }
  }
  else
  {
    return 1;
  }
  return result;
}

// ===== sub_402AC0 @ 0x00402AC0..0x00402B5D =====
int __cdecl sub_402AC0(int a1, int a2)
{
  int v2; // ecx
  int result; // eax
  int v4; // [esp+4h] [ebp-34h]

  if ( !sub_407F20(v2, dword_566750) )
    return 1;
  if ( !sub_407F20(a1, dword_566750) )
    return 2;
  switch ( sub_413500(a2) )
  {
    case 0:
      result = 0;
      break;
    case 3:
      result = 5;
      break;
    case 0xE:
      result = 4;
      break;
    case 0xF:
      result = 3;
      break;
    default:
      result = v4;
      break;
  }
  return result;
}

// ===== sub_402B90 @ 0x00402B90..0x00402C52 =====
int __usercall sub_402B90@<eax>(int a1@<ecx>, int a2@<edi>, int a3, int a4, int a5)
{
  int v6; // [esp+10h] [ebp-28h]
  int v7; // [esp+14h] [ebp-24h]
  int v8; // [esp+18h] [ebp-20h]
  _BYTE v9[24]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(a1, dword_566750) )
    return 2;
  if ( v8 != 1 && v8 != 2 )
    return 3;
  if ( !((unsigned int)(a4 * v6) >> 16) || !((unsigned int)(a2 * v7) >> 16) )
    return 4;
  if ( !sub_407DA0((unsigned int)(a4 * v6) >> 16, (unsigned int)(a2 * v7) >> 16, v8) )
    return 1;
  sub_407F20(a3, dword_566750);
  sub_402C60(v9, a5);
  return 0;
}

// ===== sub_402C60 @ 0x00402C60..0x00402C84 =====
int __usercall sub_402C60@<eax>(int a1@<eax>, int a2@<edx>, int a3@<esi>, int a4, int a5)
{
  if ( a5 )
    return sub_494D90(a3, a2, a1);
  else
    return sub_494B90(a4, a3, a2, a1);
}

// ===== sub_402C90 @ 0x00402C90..0x00402E6B =====
int __usercall sub_402C90@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        __int64 a3,
        unsigned int a4,
        unsigned int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9)
{
  bool v10; // zf
  int result; // eax
  __int64 v12; // [esp+10h] [ebp-50h] BYREF
  int v13; // [esp+18h] [ebp-48h]
  int v14; // [esp+1Ch] [ebp-44h]
  _BYTE v15[16]; // [esp+20h] [ebp-40h] BYREF
  _BYTE v16[24]; // [esp+30h] [ebp-30h] BYREF
  _BYTE v17[24]; // [esp+48h] [ebp-18h] BYREF

  if ( !sub_407F20(a2, dword_566750) )
    return 1;
  v10 = sub_407F20(a6, dword_566750) == 0;
  result = 2;
  if ( !v10 )
  {
    if ( a4 < 2 || a5 < 2 )
    {
      return 5;
    }
    else if ( a8 < 2 || a9 < 2 )
    {
      return 6;
    }
    else
    {
      v12 = a3;
      v13 = a3 + a4 - 1;
      v14 = HIDWORD(a3) + a5 - 1;
      sub_409190(v16);
      sub_409110(&v12);
      sub_4091B0(v15);
      v12 = (__int64)(65540.0 * (double)a4 / (double)a8);
      switch ( sub_414560(v16, v17, a7 << 16, a1 << 16, v12, (__int64)((double)a5 * 65540.0 / (double)a9)) )
      {
        case 0:
          result = 0;
          break;
        case 1:
          result = 8;
          break;
        case 0x13:
          result = 7;
          break;
        case 0x14:
          return 6;
        default:
          result = v12;
          break;
      }
    }
  }
  return result;
}

// ===== sub_402EA0 @ 0x00402EA0..0x00402F2F =====
int __cdecl sub_402EA0(int a1, int a2, int a3)
{
  int v3; // ecx
  int v4; // eax
  int v5; // eax
  int v7; // [esp+4h] [ebp-34h]
  _BYTE v8[24]; // [esp+8h] [ebp-30h] BYREF

  if ( !sub_407F20(v3, dword_566750) )
    return 1;
  if ( !sub_407F20(a1, dword_566750) )
    return 2;
  v4 = sub_414260(v8, a2, a3);
  if ( !v4 )
    return 0;
  v5 = v4 - 1;
  if ( !v5 )
    return 3;
  if ( v5 == 18 )
    return 4;
  return v7;
}

// ===== sub_402F30 @ 0x00402F30..0x00402FD0 =====
int __cdecl sub_402F30(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  int v10; // ecx
  int v11; // eax
  int v13; // [esp+4h] [ebp-34h]
  _BYTE v14[24]; // [esp+8h] [ebp-30h] BYREF
  _BYTE v15[24]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(v10, dword_566750) )
    return 1;
  if ( !sub_407F20(a3, dword_566750) )
    return 2;
  v11 = sub_4168E0(v15, a1, a2, v14, a4, a5, a6, a7, a8, a9, a10, 1);
  if ( !v11 )
    return 0;
  if ( v11 == 19 )
    return 3;
  return v13;
}

// ===== sub_402FD0 @ 0x00402FD0..0x00403070 =====
int __cdecl sub_402FD0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  int v10; // ecx
  int v11; // eax
  int v13; // [esp+4h] [ebp-34h]
  _BYTE v14[24]; // [esp+8h] [ebp-30h] BYREF
  _BYTE v15[24]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(v10, dword_566750) )
    return 1;
  if ( !sub_407F20(a3, dword_566750) )
    return 2;
  v11 = sub_417730(v15, a1, a2, v14, a4, a5, a6, a7, a8, a9, a10, 1);
  if ( !v11 )
    return 0;
  if ( v11 == 19 )
    return 3;
  return v13;
}

// ===== sub_403070 @ 0x00403070..0x00403106 =====
int __usercall sub_403070@<eax>(int a1@<ecx>, unsigned int a2@<edi>, int a3, unsigned int a4, unsigned int a5)
{
  if ( !sub_407F20(a1, dword_566750) )
    return 1;
  if ( !sub_407F20(a3, dword_566750) )
    return 2;
  if ( a4 > 0x10000 || a2 > 0x10000 )
    return 4;
  if ( a5 > 0x100 )
    return 5;
  sub_418CB0(a4, a2, a5);
  return 0;
}

// ===== sub_403110 @ 0x00403110..0x00403181 =====
int __cdecl sub_403110(int a1, int a2, int a3, int a4)
{
  int v4; // ecx
  _BYTE v6[24]; // [esp+8h] [ebp-30h] BYREF
  _BYTE v7[24]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(v4, dword_566750) )
    return 1;
  if ( sub_407F20(a1, dword_566750) )
    return sub_4199D0(v7, v6, a2, a3, a4, 1) != 0 ? 6 : 0;
  return 2;
}

// ===== sub_403190 @ 0x00403190..0x004032B9 =====
int __cdecl sub_403190(int a1)
{
  int v1; // ecx
  int v2; // eax
  _BYTE v4[16]; // [esp+10h] [ebp-50h] BYREF
  _BYTE v5[16]; // [esp+20h] [ebp-40h] BYREF
  _BYTE v6[24]; // [esp+30h] [ebp-30h] BYREF
  _BYTE v7[16]; // [esp+48h] [ebp-18h] BYREF
  int v8; // [esp+58h] [ebp-8h]

  if ( !sub_407F20(v1, dword_566750) )
    return 1;
  if ( !sub_407F20(a1, dword_566750) )
    return 2;
  sub_409190(v6);
  v2 = sub_409190(v7);
  sub_409170(v2);
  if ( !sub_409110(v5) )
    return 0;
  sub_4091B0(v4);
  sub_409170(v4);
  sub_4091B0(v4);
  if ( v8 == 2 )
  {
    sub_415B30(0);
    return 0;
  }
  else if ( v8 == 3 )
  {
    sub_4155A0(v6);
    return 0;
  }
  else
  {
    return 3;
  }
}

// ===== sub_4032C0 @ 0x004032C0..0x00403392 =====
int __usercall sub_4032C0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8)
{
  _DWORD v10[4]; // [esp+10h] [ebp-50h] BYREF
  _BYTE v11[16]; // [esp+20h] [ebp-40h] BYREF
  _BYTE v12[24]; // [esp+30h] [ebp-30h] BYREF

  if ( !sub_407F20(a2, dword_566750) )
    return 1;
  if ( !sub_407F20(a5, dword_566750) )
    return 2;
  if ( !a7 || !a8 )
    return 3;
  v10[2] = a6 + a7 - 1;
  v10[0] = a6;
  v10[1] = a1;
  v10[3] = a1 + a8 - 1;
  sub_409190(v12);
  if ( sub_409110(v10) )
  {
    sub_4091B0(v11);
    sub_40A530(a3, 128, 0);
  }
  return 0;
}

// ===== sub_4033A0 @ 0x004033A0..0x00403445 =====
int __usercall sub_4033A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  int v8; // [esp+20h] [ebp-20h]

  if ( !sub_407F20(a2, dword_566750) )
    return 2;
  if ( !a5 || !a6 )
    return 3;
  if ( !sub_407DA0(a5, a6, v8) )
    return 1;
  sub_407F20(a1, dword_566750);
  sub_40A530(-a3, 128, 0);
  return 0;
}

// ===== sub_403450 @ 0x00403450..0x004034EF =====
int __usercall sub_403450@<eax>(int a1@<edi>, int a2)
{
  int v2; // esi
  int v3; // edx
  int v4; // eax
  int v6; // [esp+10h] [ebp-28h]
  int v7; // [esp+14h] [ebp-24h]
  int v8; // [esp+18h] [ebp-20h]

  if ( !sub_407F20(a2, dword_566750) )
    return 2;
  if ( !sub_407DA0(v6, v7, v8) )
    return 1;
  sub_407F20(a1, dword_566750);
  sub_40ADF0();
  v2 = sub_407F00(a2, dword_566750);
  v4 = sub_407F00(a1, v3);
  *(_DWORD *)(v4 + 40) = *(_DWORD *)(v2 + 40);
  *(_DWORD *)(v4 + 44) = *(_DWORD *)(v2 + 44);
  return 0;
}

// ===== sub_4034F0 @ 0x004034F0..0x00403595 =====
int __cdecl sub_4034F0(int a1, int a2)
{
  int v2; // ecx
  int v3; // eax
  _BYTE v5[16]; // [esp+8h] [ebp-50h] BYREF
  _BYTE v6[24]; // [esp+28h] [ebp-30h] BYREF
  _BYTE v7[24]; // [esp+40h] [ebp-18h] BYREF

  if ( !sub_407F20(v2, dword_566750) )
    return 1;
  if ( !sub_407F20(a1, dword_566750) )
    return 2;
  sub_409190(v7);
  v3 = sub_409190(v6);
  sub_409110(v3);
  sub_4091B0(v5);
  sub_4091B0(v5);
  sub_4188D0(a2);
  return 0;
}

// ===== sub_4035A0 @ 0x004035A0..0x00403600 =====
int __usercall sub_4035A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  const CHAR *v4; // eax
  unsigned int v5; // eax
  int result; // eax
  bool v7; // zf

  v4 = (const CHAR *)sub_468BB0();
  v5 = sub_409290(a4, (int)dword_566750, a3, v4, a1);
  if ( v5 > 0x80000003 )
  {
    v7 = v5 == -2147483644;
    result = -2147483645;
    if ( v7 )
      return result;
  }
  else
  {
    switch ( v5 )
    {
      case 0x80000003:
        return -2147483646;
      case 0u:
        return 0;
      case 0x80000002:
        return -2147483647;
    }
  }
  return a2;
}

// ===== sub_403600 @ 0x00403600..0x0040367E =====
int __cdecl sub_403600(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  int v10; // ecx
  int result; // eax
  int v12; // [esp+4h] [ebp-1Ch] BYREF
  _BYTE v13[24]; // [esp+8h] [ebp-18h] BYREF

  if ( !sub_407F20(v10, dword_566750) )
    return -2147483644;
  result = sub_4035A0(a5, a6, (int)&v12, a6);
  if ( !result )
  {
    sub_4097D0(dword_566750, v13, a10, a1, a2, a3, v12, a9);
    return 0;
  }
  return result;
}

// ===== sub_403680 @ 0x00403680..0x00403701 =====
int __cdecl sub_403680(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
  int v11; // ecx
  int result; // eax
  int v13; // [esp+4h] [ebp-1Ch] BYREF
  _BYTE v14[24]; // [esp+8h] [ebp-18h] BYREF

  if ( !sub_407F20(v11, dword_566750) )
    return -2147483644;
  result = sub_4035A0(a5, a6, (int)&v13, a6);
  if ( !result )
  {
    sub_409800(v14, a11, a1, a2, a3, a9, 0, a8, 1, a10);
    return 0;
  }
  return result;
}

// ===== sub_403710 @ 0x00403710..0x00403838 =====
unsigned int __usercall sub_403710@<eax>(int a1@<eax>, unsigned int **a2, int a3, unsigned int a4)
{
  unsigned int result; // eax
  unsigned int v7; // ecx
  unsigned int i; // edx
  unsigned int *v9; // ecx
  unsigned int v10; // esi
  unsigned int v11; // edx
  int v12; // edi
  unsigned int *v13; // eax
  int v14; // eax
  unsigned int v15; // eax
  unsigned int v16; // [esp+Ch] [ebp-14h]
  unsigned int *v17; // [esp+14h] [ebp-Ch]
  unsigned int v18; // [esp+18h] [ebp-8h]
  _BYTE *v19; // [esp+1Ch] [ebp-4h]
  int v20; // [esp+28h] [ebp+8h]

  sub_42FA50(a3);
  v17 = *a2;
  v19 = *(_BYTE **)(a1 + 8);
  result = sub_490AE0();
  v7 = (unsigned int)a2[3];
  if ( result < v7 )
    v7 = result;
  if ( result >= (unsigned int)a2[2] )
  {
    v18 = (unsigned int)a2[2];
    result = v18;
  }
  else
  {
    v18 = result;
  }
  for ( i = v7; i; v17 = (unsigned int *)((char *)v17 + (_DWORD)a2[1]) )
  {
    v9 = v17;
    v16 = --i;
    v10 = result;
    if ( result )
    {
      do
      {
        v11 = 128;
        if ( v10 >= 8 )
        {
          v20 = 8;
          v12 = 8;
        }
        else
        {
          v12 = v10;
          v20 = v10;
        }
        if ( v12 )
        {
          do
          {
            --v12;
            if ( ((unsigned __int8)v11 & *v19) != 0 )
            {
              v13 = a2[4];
              if ( v13 )
              {
                v14 = (int)v13 - 1;
                if ( v14 )
                {
                  if ( v14 == 1 )
                    *v9 = a4 | 0xFF000000;
                }
                else
                {
                  *v9 = a4;
                }
              }
              else
              {
                *(_WORD *)v9 = ((unsigned __int8)a4 >> 3) + ((a4 >> 6) & 0x3E0) + ((a4 >> 9) & 0x7C00);
              }
            }
            else
            {
              v15 = 0;
              if ( a2[5] )
              {
                do
                  *((_BYTE *)v9 + v15++) = 0;
                while ( v15 < (unsigned int)a2[5] );
              }
            }
            v9 = (unsigned int *)((char *)v9 + (_DWORD)a2[5]);
            v11 >>= 1;
          }
          while ( v12 );
          v12 = v20;
        }
        ++v19;
        v10 -= v12;
      }
      while ( v10 );
      result = v18;
      i = v16;
    }
  }
  return result;
}

// ===== sub_403840 @ 0x00403840..0x004039DF =====
void __usercall sub_403840(_BYTE *a1@<eax>, int a2, int a3, int a4, int a5, int a6, unsigned int a7, _DWORD *a8)
{
  int v9; // ebx
  bool v10; // zf
  _BYTE *v11; // esi
  int v12; // edi
  unsigned int v13; // ecx
  unsigned int v14; // ecx
  unsigned int v15; // ecx
  unsigned int v16; // esi
  char v17[28]; // [esp+10h] [ebp-64h] BYREF
  void *v18; // [esp+2Ch] [ebp-48h]
  void *v19; // [esp+30h] [ebp-44h]
  unsigned int v20; // [esp+34h] [ebp-40h]
  int v21; // [esp+38h] [ebp-3Ch]
  int v22; // [esp+3Ch] [ebp-38h]
  int v23; // [esp+40h] [ebp-34h]
  void *v24[2]; // [esp+44h] [ebp-30h] BYREF
  unsigned int v25; // [esp+4Ch] [ebp-28h]
  int v26; // [esp+50h] [ebp-24h]
  int v27; // [esp+54h] [ebp-20h]
  int v28; // [esp+58h] [ebp-1Ch]
  int v29; // [esp+5Ch] [ebp-18h]
  unsigned int v30; // [esp+60h] [ebp-14h]
  int v31; // [esp+64h] [ebp-10h]
  _BYTE *i; // [esp+68h] [ebp-Ch]
  int v33; // [esp+6Ch] [ebp-8h]

  v9 = sub_490AE0();
  sub_409080(1);
  v10 = *a1 == 0;
  *a8 = 0;
  v33 = a3;
  v31 = 100;
  v30 = 0;
  v11 = a1;
  for ( i = a1; !v10; i = v11 )
  {
    v12 = sub_42FA80();
    v29 = v12;
    v13 = (unsigned __int8)v13;
    if ( v12 )
      v13 = (unsigned __int8)v11[1] + ((unsigned __int8)v13 << 8);
    if ( v13 >= 0x20 )
    {
      sub_403710((int)v17, (unsigned int **)v24, a5, a7);
      v16 = v25;
      v18 = v24[0];
      v19 = v24[1];
      v20 = v25;
      v21 = v26;
      v22 = v27;
      v23 = v28;
      if ( !v12 )
      {
        v16 = v25 >> 1;
        v20 = v25 >> 1;
      }
      if ( v30 && v16 + v33 > v30 )
      {
        v33 = a3;
        a4 += v9 * v31 / 100;
      }
      if ( sub_40A530(v33, 0, 0) )
        break;
      v12 = v29;
      v33 += v16 + a6;
      *a8 += v16 + a6;
    }
    else
    {
      v14 = v13 - 3;
      if ( v14 )
      {
        v15 = v14 - 1;
        if ( v15 )
        {
          if ( v15 == 6 )
          {
            v33 = a3;
            a4 += v9 * v31 / 100;
          }
        }
        else
        {
          v30 = *(_DWORD *)(a2 + 8);
        }
        goto LABEL_19;
      }
      v31 = (unsigned __int8)v11[1];
      i = v11 + 1;
    }
    v11 = i;
LABEL_19:
    v11 += (v12 != 0) + 1;
    v10 = *v11 == 0;
  }
  operator delete(v24[0]);
}

// ===== sub_4039E0 @ 0x004039E0..0x00403B05 =====
int __cdecl sub_4039E0(int a1, int a2, _BYTE *a3, int a4, int a5, int a6, int a7, unsigned int a8, _DWORD *a9)
{
  int v9; // ecx
  int v10; // edi
  void *v11; // esi
  int v12; // eax
  int v13; // edi
  void *v15; // [esp+10h] [ebp-2Ch]
  _BYTE v16[28]; // [esp+14h] [ebp-28h] BYREF
  int v17; // [esp+38h] [ebp-4h]

  if ( !sub_407F20(v9, dword_566750) )
    return -2147483644;
  v10 = sub_468BB0();
  if ( !v10 )
    return -2147483645;
  v15 = operator new(0x78u);
  v17 = 0;
  if ( v15 )
    v11 = (void *)sub_42F3F0();
  else
    v11 = 0;
  v17 = -1;
  v12 = sub_42F4F0(v10, a6);
  if ( v12 )
  {
    if ( v12 == -2147483646 )
      v13 = -2147483647;
    else
      v13 = (int)v15;
  }
  else
  {
    sub_403840(a3, (int)v16, a1, a2, (int)v11, a7, a8, a9);
    v13 = 0;
  }
  if ( v11 )
  {
    sub_42F430();
    operator delete(v11);
  }
  return v13;
}

// ===== sub_403B10 @ 0x00403B10..0x00403BA0 =====
int __cdecl sub_403B10(
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
        int a15,
        int a16)
{
  int v16; // ecx
  int result; // eax
  int v18; // [esp+4h] [ebp-1Ch] BYREF

  if ( !sub_407F20(v16, dword_566750) )
    return -2147483644;
  result = sub_4035A0(a8, a9, (int)&v18, a9);
  if ( !result )
  {
    sub_434BA0(a1, a2, a3, a4, a5, a6, v18, a11, a12, a13, a14, a15, a16);
    return 0;
  }
  return result;
}

// ===== sub_403BA0 @ 0x00403BA0..0x00403BF9 =====
int __cdecl sub_403BA0(_DWORD *a1, int a2, int a3, int a4, int a5)
{
  int v5; // ecx
  int result; // eax
  _BYTE v7[52]; // [esp+4h] [ebp-44h] BYREF
  _BYTE v8[8]; // [esp+38h] [ebp-10h] BYREF
  int v9; // [esp+40h] [ebp-8h]
  int v10; // [esp+44h] [ebp-4h] BYREF

  result = sub_4035A0(a4, v5, (int)&v10, v5);
  if ( !result )
  {
    sub_4092B0(dword_566750);
    sub_434F00(v8, a2, v7, a5);
    *a1 = v9;
    return 0;
  }
  return result;
}

// ===== sub_403C00 @ 0x00403C00..0x00403C07 =====
int __usercall sub_403C00@<eax>(int a1@<eax>)
{
  return sub_408320(a1);
}

// ===== sub_403C10 @ 0x00403C10..0x00403C15 =====
// attributes: thunk
int sub_403C10()
{
  return sub_4083A0();
}

// ===== sub_403C20 @ 0x00403C20..0x00403D0D =====
int __cdecl sub_403C20(int a1, int a2, int a3, int a4, int a5)
{
  int v5; // ecx
  char *v6; // edi
  int v8; // eax
  int v9; // [esp+Ch] [ebp-4Ch]
  int v10; // [esp+18h] [ebp-40h]
  int v11; // [esp+1Ch] [ebp-3Ch]
  char v12; // [esp+28h] [ebp-30h] BYREF

  if ( !sub_407F20(v5, dword_566750) )
    return -2147483638;
  v6 = 0;
  if ( a4 != -1 )
  {
    v6 = sub_407F20(a4, dword_566750) != 0 ? &v12 : 0;
    if ( !v6 )
      return -2147483637;
  }
  sub_442E10(dword_56674C);
  if ( !sub_407DA0(v10, v11, 3) )
    return -2147483639;
  sub_407F20(a1, dword_566750);
  v8 = sub_418960(a2, a3, v6, a5);
  if ( !v8 )
    return 0;
  if ( v8 == 1 )
    return -2147483636;
  return v9;
}

// ===== sub_403D10 @ 0x00403D10..0x00403DED =====
int __usercall sub_403D10@<eax>(int *a1@<eax>, __int16 a2, __int16 a3, int a4, int a5)
{
  int v5; // edi
  int v6; // ebx
  int v7; // ecx
  int v8; // esi
  __int16 v9; // dx
  int v10; // ecx
  int v11; // ebx
  unsigned int v12; // eax
  __int16 v13; // dx
  int v15; // [esp+8h] [ebp-14h]
  __int16 v16; // [esp+Ch] [ebp-10h]
  int v17; // [esp+10h] [ebp-Ch]
  int v18; // [esp+14h] [ebp-8h]
  int v19; // [esp+18h] [ebp-4h]

  if ( a1[4] != 4 )
    return -2147483645;
  v5 = a1[2];
  v6 = 0;
  if ( !v5 )
    return -2147483645;
  v7 = a1[3];
  v17 = v7;
  if ( !v7 )
    return -2147483645;
  if ( !a4 || !a5 )
    return -2147483643;
  v8 = *a1;
  v18 = 0;
  if ( v7 > 0 )
  {
    v15 = a1[1];
    v19 = 0;
    while ( 1 )
    {
      v9 = 16 * (a3 - v6) + 16 * v19 / (unsigned int)v7;
      v10 = 0;
      v16 = v9;
      if ( v5 > 0 )
      {
        v11 = 0;
        do
        {
          v12 = 16 * v11 / (unsigned int)v5;
          v11 += a4;
          v13 = 16 * (a2 - v10++);
          *(_WORD *)(v8 + 4 * v10 - 4) = v13 + v12;
          *(_WORD *)(v8 + 4 * v10 - 2) = v16;
        }
        while ( v10 < v5 );
        v6 = v18;
      }
      v8 += v15;
      v19 += a5;
      v18 = ++v6;
      if ( v6 >= v17 )
        break;
      v7 = v17;
    }
  }
  return 0;
}

// ===== sub_403DF0 @ 0x00403DF0..0x00403E36 =====
int __cdecl sub_403DF0(__int16 a1, __int16 a2, int a3, int a4)
{
  int v4; // ecx
  int v6[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v4, dword_566750) )
    return sub_403D10(v6, a1, a2, a3, a4);
  else
    return -2147483647;
}

// ===== sub_403E40 @ 0x00403E40..0x00403EF9 =====
int __cdecl sub_403E40(_DWORD *a1, int a2)
{
  _DWORD *v2; // ecx
  int v3; // eax
  int v4; // edx
  int v5; // ebx
  int v6; // esi
  int v7; // edi
  int v8; // edi
  __int16 v9; // cx
  int v11; // [esp+0h] [ebp-4h]

  v2 = a1;
  if ( a1[4] != 4 )
    return -2147483645;
  v3 = a1[2];
  if ( !v3 )
    return -2147483645;
  v4 = a1[3];
  if ( !v4 )
    return -2147483645;
  v5 = *a1;
  v11 = 0;
  if ( v4 > 0 )
  {
    do
    {
      v6 = 0;
      if ( v3 > 0 )
      {
        do
        {
          v7 = rand() << 15;
          *(_WORD *)(v5 + 4 * v6) = a2
                                  - ((unsigned int)v7 | (unsigned __int64)(unsigned int)rand())
                                  % (unsigned int)(2 * a2 + 1);
          v8 = rand() << 15;
          ++v6;
          v9 = a2 - ((unsigned int)v8 | (unsigned __int64)(unsigned int)rand()) % (unsigned int)(2 * a2 + 1);
          v3 = a1[2];
          *(_WORD *)(v5 + 4 * v6 - 2) = v9;
        }
        while ( v6 < v3 );
        v2 = a1;
      }
      v5 += v2[1];
      ++v11;
    }
    while ( v11 < v2[3] );
  }
  return 0;
}

// ===== sub_403F00 @ 0x00403F00..0x00403F3B =====
int __cdecl sub_403F00(int a1)
{
  int v1; // ecx
  _DWORD v3[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v1, dword_566750) )
    return sub_403E40(v3, a1);
  else
    return -2147483647;
}

// ===== sub_403F40 @ 0x00403F40..0x00404128 =====
int __usercall sub_403F40@<eax>(int *a1@<eax>, int a2@<ecx>, int a3)
{
  int v4; // edx
  int v5; // eax
  _WORD *v7; // edi
  int v8; // ebx
  int v9; // eax
  int v10; // ebx
  unsigned int v11; // edx
  unsigned int v12; // eax
  unsigned int *v13; // ebx
  unsigned int v14; // edi
  __int64 v15; // rax
  unsigned int v16; // edx
  unsigned int v17; // eax
  __int64 v18; // rax
  bool v19; // zf
  int v20; // [esp+Ch] [ebp-38h]
  int v21; // [esp+10h] [ebp-34h]
  int v22; // [esp+14h] [ebp-30h]
  int v23; // [esp+24h] [ebp-20h]
  int v24; // [esp+28h] [ebp-1Ch]
  int v25; // [esp+2Ch] [ebp-18h]
  _WORD *v26; // [esp+30h] [ebp-14h]
  int v27; // [esp+34h] [ebp-10h]
  _WORD *v28; // [esp+38h] [ebp-Ch]
  int v29; // [esp+40h] [ebp-4h]
  int v30; // [esp+40h] [ebp-4h]

  if ( *(_DWORD *)(a2 + 16) != 4 )
    return -2147483645;
  v4 = *(_DWORD *)(a2 + 8);
  v21 = v4;
  if ( !v4 )
    return -2147483645;
  v5 = *(_DWORD *)(a2 + 12);
  if ( !v5 )
    return -2147483645;
  if ( a1[4] != 5 || a1[2] != v4 + 1 || a1[3] != v5 + 1 )
    return -2147483644;
  v7 = *(_WORD **)a2;
  v8 = *a1;
  v26 = *(_WORD **)a2;
  if ( !a3 )
    a3 = 1;
  v20 = *(_DWORD *)(a2 + 4);
  v24 = a1[1];
  v25 = *(_DWORD *)(a2 + 12);
  do
  {
    v9 = v8 + v24;
    v22 = v8 + v24;
    if ( v4 )
    {
      v10 = v8 - (_DWORD)v7;
      v28 = v7;
      v23 = v10;
      v27 = v4;
      while ( 1 )
      {
        v11 = *(_DWORD *)((char *)v7 + v10);
        v12 = *(_DWORD *)((char *)v7 + v10 + 4);
        v13 = (unsigned int *)((char *)v7 + v10);
        if ( v11 == v12 )
        {
          *v7 = 0;
        }
        else
        {
          if ( v11 <= v12 )
          {
            v14 = v11;
            v11 = v12;
            v29 = 0;
            v12 = v14;
          }
          else
          {
            v29 = 1;
          }
          v15 = (__int64)((v11 - (unsigned __int64)v12) * (v11 + (unsigned __int64)v12)) / (unsigned int)(2 * a3);
          if ( v15 >= 0x8000 )
            v15 = 0x7FFFLL;
          if ( v29 )
            v15 = -v15;
          v7 = v28;
          *v28 = v15;
        }
        v16 = *v13;
        v17 = *(unsigned int *)((char *)v13 + v24);
        if ( *v13 == v17 )
        {
          v7[1] = 0;
        }
        else
        {
          if ( v16 <= v17 )
          {
            v16 = *(unsigned int *)((char *)v13 + v24);
            v30 = 0;
            v17 = *v13;
          }
          else
          {
            v30 = 1;
          }
          v18 = (__int64)((v16 - (unsigned __int64)v17) * (v16 + (unsigned __int64)v17)) / (unsigned int)(2 * a3);
          if ( v18 >= 0x8000 )
            v18 = 0x7FFFLL;
          if ( v30 )
            v18 = -v18;
          v7 = v28;
          v28[1] = v18;
        }
        v7 += 2;
        v19 = v27-- == 1;
        v28 = v7;
        if ( v19 )
          break;
        v10 = v23;
      }
      v9 = v22;
      v7 = v26;
      v4 = v21;
    }
    v7 = (_WORD *)((char *)v7 + v20);
    v19 = v25-- == 1;
    v26 = v7;
    v8 = v9;
  }
  while ( !v19 );
  return 0;
}
