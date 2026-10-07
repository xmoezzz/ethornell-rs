#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_428020 @ 0x00428020..0x00428051 =====
_DWORD *__userpurge sub_428020@<eax>(_DWORD *result@<eax>, int a2@<edx>, int a3)
{
  int v3; // ecx

  v3 = result[77];
  if ( v3 == 2 || (unsigned int)(v3 - 5) <= 1 )
  {
    result[146] = a3 << 16;
    result[147] = a2 << 16;
  }
  return result;
}

// ===== sub_428060 @ 0x00428060..0x004280C7 =====
int __usercall sub_428060@<eax>(_DWORD *a1@<eax>, int a2@<edx>)
{
  int v3; // ecx
  int result; // eax
  int v5; // ecx
  int v6; // ecx

  v3 = a1[77];
  result = 0;
  v5 = v3 - 2;
  if ( v5 )
  {
    v6 = v5 - 3;
    if ( v6 )
    {
      if ( v6 == 1 )
      {
        a1[151] = a2;
        sub_42A090();
        return 1;
      }
    }
    else
    {
      a1[148] = a2;
      sub_4299A0();
      sub_428E70(0);
      return 1;
    }
  }
  else
  {
    a1[148] = a2;
    sub_429880();
    return 1;
  }
  return result;
}

// ===== sub_4280D0 @ 0x004280D0..0x004280FF =====
int __usercall sub_4280D0@<eax>(_DWORD *a1@<edx>, _DWORD *a2@<esi>)
{
  int result; // eax

  result = 0;
  if ( a1[77] == 2 || a1[77] == 5 )
  {
    *a2 = a1[148];
    return 1;
  }
  else if ( a1[77] == 6 )
  {
    *a2 = a1[151];
    return 1;
  }
  return result;
}

// ===== sub_428100 @ 0x00428100..0x00428126 =====
int __fastcall sub_428100(_DWORD *a1, int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( a1[77] == 6 )
  {
    a1[149] = a3;
    a1[150] = a2;
    return 1;
  }
  return result;
}

// ===== sub_428130 @ 0x00428130..0x004281D5 =====
int __fastcall sub_428130(int a1, int a2, _DWORD *a3)
{
  int result; // eax

  result = 0;
  if ( !a1 )
    a1 = 1;
  if ( a2 )
  {
    a3[157] = 1;
  }
  else
  {
    a2 = a1;
    a3[157] = 0;
  }
  switch ( a3[77] )
  {
    case 2:
      a3[155] = a1;
      a3[156] = a2;
      sub_429880();
      return 1;
    case 5:
      a3[155] = a1;
      a3[156] = a2;
      sub_4299A0();
      sub_428E70(0);
      sub_429000();
      return 1;
    case 6:
      a3[155] = a1;
      a3[156] = a2;
      sub_42A090();
      return 1;
  }
  return result;
}

// ===== sub_4281E0 @ 0x004281E0..0x0042827C =====
int __thiscall sub_4281E0(_DWORD *this, int *a2)
{
  int result; // eax
  _DWORD v4[2]; // [esp+8h] [ebp-8h] BYREF

  if ( this[77] == 6 )
  {
    if ( this[185] )
    {
      *a2 = this[188];
      a2[1] = this[189];
      result = sub_41C0E0(v4, (int)this);
      if ( result )
      {
        result = v4[1];
        *a2 += v4[0];
        a2[1] += result;
      }
    }
    else
    {
      return sub_41B260(this, a2);
    }
  }
  else
  {
    sub_41B260(this, a2);
    if ( this[77] == 2 || (result = this[77] - 5, this[77] == 5) )
    {
      *a2 -= this[183];
      result = this[184];
      a2[1] -= result;
    }
  }
  return result;
}

// ===== sub_428280 @ 0x00428280..0x004282AD =====
int __usercall sub_428280@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  int result; // eax
  int v3; // ecx
  _DWORD v4[2]; // [esp+4h] [ebp-8h] BYREF

  (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 52))(a1);
  result = sub_41C0E0(v4, a1);
  if ( result )
  {
    result = v4[0];
    v3 = v4[1];
    *a2 -= v4[0];
    a2[1] -= v3;
  }
  return result;
}

// ===== sub_4282B0 @ 0x004282B0..0x0042830D =====
int __thiscall sub_4282B0(_DWORD *this, signed int a2, signed int a3, int a4)
{
  int result; // eax

  sub_41B370(this, a2, a3, a4);
  if ( this[77] == 5 )
  {
    sub_4299A0();
    sub_428E70(0);
    return sub_429000();
  }
  else
  {
    result = this[77] - 6;
    if ( this[77] == 6 )
    {
      sub_428E70(1);
      sub_429000();
      return sub_42A090();
    }
  }
  return result;
}

// ===== sub_428310 @ 0x00428310..0x0042836D =====
int __thiscall sub_428310(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax

  sub_41B520(this, a2, a3, a4);
  if ( this[77] == 5 )
  {
    sub_4299A0();
    sub_428E70(0);
    return sub_429000();
  }
  else
  {
    result = this[77] - 6;
    if ( this[77] == 6 )
    {
      sub_428E70(1);
      sub_429000();
      return sub_42A090();
    }
  }
  return result;
}

// ===== sub_428370 @ 0x00428370..0x00428377 =====
int __usercall sub_428370@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 632) = a2;
  return result;
}

// ===== sub_428380 @ 0x00428380..0x00428422 =====
unsigned int __thiscall sub_428380(_DWORD *this, int a2)
{
  unsigned int result; // eax

  result = this[145];
  if ( result > 2 )
  {
    if ( result != 3 )
    {
      if ( result != -1 )
        return result;
      if ( this[77] == 4 )
        sub_409F40(this[203], this[204], this[205], this[206], a2);
    }
    return sub_41B620(this, a2);
  }
  if ( result == 2 )
  {
    sub_41B620(this, a2);
    this[144] = a2;
    sub_428E70(0);
    return sub_429000();
  }
  if ( !result )
    return sub_41B620(this, a2);
  if ( !--result )
  {
    this[144] = a2;
    sub_428E70(0);
    return sub_429000();
  }
  return result;
}

// ===== sub_428430 @ 0x00428430..0x0042846B =====
int __thiscall sub_428430(_DWORD *this)
{
  unsigned int v1; // eax

  v1 = this[145];
  if ( v1 <= 3 )
  {
    if ( v1 == 1 )
      return this[144];
    return sub_41B650(this);
  }
  if ( v1 == -1 )
    return sub_41B650(this);
  return (int)this;
}

// ===== sub_428470 @ 0x00428470..0x00428527 =====
int __thiscall sub_428470(_DWORD *this, int a2, int a3)
{
  int result; // eax

  sub_41B6F0(this, a2, a3);
  result = this[77] - 1;
  switch ( this[77] )
  {
    case 1:
      if ( this[145] == 3 )
      {
        result = sub_41B740();
        this[144] = result;
      }
      break;
    case 2:
      result = sub_429880();
      break;
    case 5:
      if ( this[145] == 3 )
        this[144] = sub_41B740();
      sub_4299A0();
      sub_428E70(0);
      result = sub_429000();
      break;
    case 6:
      if ( this[145] == 3 )
        this[144] = sub_41B740();
      sub_428E70(1);
      sub_429000();
      result = sub_42A090();
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_428540 @ 0x00428540..0x00428592 =====
int __userpurge sub_428540@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3, int a4)
{
  int v4; // eax
  int result; // eax

  a3[192] = a1;
  v4 = a3[77] - 5;
  a3[193] = a2;
  a3[194] = a4;
  if ( v4 )
  {
    result = v4 - 1;
    if ( !result )
    {
      sub_429000();
      return sub_42A090();
    }
  }
  else
  {
    sub_429000();
    return sub_4299A0();
  }
  return result;
}

// ===== sub_4285A0 @ 0x004285A0..0x004287B6 =====
int __thiscall sub_4285A0(_DWORD *this, unsigned int a2, unsigned int a3, int a4)
{
  int result; // eax
  int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax

  switch ( a2 )
  {
    case 0x10u:
      return sub_4272F0((int)this, a3) != 0 ? 0xFFFF0002 : 0;
    case 0x11u:
      return sub_42AB50(BYTE1(a3), a4, HIWORD(a3)) != 0 ? 0xFFFF0002 : 0;
    case 0x40u:
      v5 = this[77];
      if ( v5 == 2 || (unsigned int)(v5 - 5) <= 1 )
        sub_428020(this, a4, a3);
      return 0;
    case 0x41u:
      v6 = this[77];
      if ( v6 == 2 || (unsigned int)(v6 - 5) <= 1 )
        sub_428060(this, a3);
      return 0;
    case 0x42u:
      v7 = this[77];
      if ( v7 == 2 || (unsigned int)(v7 - 5) <= 1 )
        sub_428130(a3, a4, this);
      return 0;
    case 0x43u:
      if ( this[77] == 6 )
        sub_428100(this, a4, a3);
      return 0;
    case 0x60u:
      sub_428540(HIWORD(a3), (unsigned __int16)a3, this, a4);
      return 0;
    case 0x80u:
      v8 = this[77];
      if ( v8 == 2 || (unsigned int)(v8 - 5) <= 1 )
      {
        this[171] = a3;
        this[172] = a4;
      }
      return 0;
    case 0x81u:
      if ( this[77] == 2 || this[77] == 5 )
      {
        this[173] = a3;
        this[179] = a4;
      }
      else
      {
        result = this[77] - 6;
        if ( this[77] == 6 )
        {
          this[176] = a3;
          this[179] = a4;
          return result;
        }
      }
      return 0;
    case 0x82u:
      v9 = this[77];
      if ( v9 == 2 || (unsigned int)(v9 - 5) <= 1 )
      {
        this[177] = a3;
        this[178] = a4;
      }
      return 0;
    case 0x83u:
      if ( this[77] == 6 )
      {
        this[174] = a3;
        this[175] = a4;
      }
      return 0;
    case 0x8Fu:
      this[180] = a3;
      return 0;
    case 0x100u:
      return sub_428DE0(a4) != 0 ? 0xFFFF0002 : 0;
    default:
      return sub_41B8E0(this, a2, a3, a4);
  }
}

// ===== sub_4288F0 @ 0x004288F0..0x00428AB1 =====
int __thiscall sub_4288F0(_DWORD *this, int *a2, unsigned int a3)
{
  int v5; // edx
  unsigned int v6; // ecx
  int v7; // edx
  int v8; // ecx
  int v9; // edx
  int v10; // ecx
  int v11; // edx
  int v12; // [esp+Ch] [ebp-34h] BYREF
  int v13; // [esp+10h] [ebp-30h] BYREF
  int v14; // [esp+14h] [ebp-2Ch] BYREF
  char v15[4]; // [esp+18h] [ebp-28h] BYREF
  char v16[4]; // [esp+1Ch] [ebp-24h] BYREF
  _DWORD v17[2]; // [esp+20h] [ebp-20h] BYREF
  _DWORD v18[6]; // [esp+28h] [ebp-18h] BYREF

  switch ( a3 )
  {
    case 0x41u:
      return (unsigned __int16)-(sub_4280D0(this, a2) != 0) - 0xFFFF;
    case 0x10000000u:
      if ( this[77] == 5 )
      {
        sub_4296C0(v17, &v14, &v12, &v13, this[148], this[153], this[154], this[155], this[156], this[146], this[147]);
      }
      else
      {
        if ( this[77] != 6 )
          return -65535;
        sub_429BF0(
          v17,
          v16,
          v15,
          &v14,
          &v12,
          &v13,
          this + 146,
          this[149],
          this[150],
          this[151],
          this[153],
          this[154],
          this[155],
          this[156]);
      }
      v8 = v17[1];
      *a2 = v17[0];
      v9 = v14;
      a2[1] = v8;
      v10 = v12;
      a2[2] = v9;
      v11 = v13;
      a2[3] = v10;
      a2[4] = v11;
      return 0;
    case 0x10000100u:
      if ( sub_407F20(dword_565B30, this[84], v18) )
      {
        v5 = v18[3];
        *a2 = v18[2];
        a2[1] = v5;
      }
      v6 = this[77];
      if ( v6 == 2 || v6 > 4 && v6 <= 6 )
      {
        a2[2] = this[181];
        a2[3] = this[182];
        return 0;
      }
      else
      {
        v7 = a2[1];
        a2[2] = *a2;
        a2[3] = v7;
        return 0;
      }
    default:
      return sub_41BAE0(this, a2, a3);
  }
}

// ===== sub_428AC0 @ 0x00428AC0..0x00428BD5 =====
int __thiscall sub_428AC0(int this, int a2, int a3, int a4)
{
  int result; // eax
  int v6; // ecx
  int v7; // ecx
  long double v8; // st7
  double v9; // st6
  double v10; // st5
  long double v11; // [esp+8h] [ebp-10h]
  long double v12; // [esp+10h] [ebp-8h]

  result = 0;
  v6 = *(_DWORD *)(this + 308) - 2;
  if ( v6 )
  {
    v7 = v6 - 3;
    if ( v7 )
    {
      if ( v7 != 1 )
        return sub_41BDF0((_DWORD *)this, a2, a3, a4);
    }
    else
    {
      v11 = -((double)*(int *)(this + 652) * 0.0000152587890625 * 3.141592653589793 / 180.0);
      v12 = cos(v11);
      v8 = sin(v11);
      v10 = (double)(*(_DWORD *)(this + 736) - a3);
      v9 = (double)(a2 - *(_DWORD *)(this + 732));
      return sub_41BDF0(
               (_DWORD *)this,
               *(__int16 *)(this + 586)
             - (int)((v9 * v12 - v8 * v10) * -65536.0 / (double)*(unsigned int *)(this + 668)),
               *(__int16 *)(this + 590) - (int)((v10 * v12 + v9 * v8) * 65536.0 / (double)*(unsigned int *)(this + 672)),
               0);
    }
  }
  return result;
}

// ===== sub_428BE0 @ 0x00428BE0..0x00428BF6 =====
BOOL __thiscall sub_428BE0(_DWORD *this)
{
  return (unsigned int)(this[77] - 5) <= 1;
}

// ===== sub_428C00 @ 0x00428C00..0x00428CC3 =====
int __stdcall sub_428C00(_DWORD *a1)
{
  int v2; // [esp+Ch] [ebp-18h]
  int v3; // [esp+10h] [ebp-14h]
  int v4; // [esp+14h] [ebp-10h]
  int v5; // [esp+18h] [ebp-Ch]

  if ( sub_42A2C0(a1) )
  {
    if ( a1[77] == 5 )
      sub_42A770(a1);
    if ( v2 > v4 || v3 > v5 )
    {
      return 0;
    }
    else
    {
      (*(void (__thiscall **)(_DWORD *))(*a1 + 28))(a1);
      sub_443240(dword_565B2C);
      return 1;
    }
  }
  else
  {
    if ( (unsigned int)(a1[77] - 5) <= 1 )
      sub_42A650(a1);
    (*(void (__thiscall **)(_DWORD *))(*a1 + 12))(a1);
    return 1;
  }
}

// ===== sub_428CD0 @ 0x00428CD0..0x00428D4D =====
int __thiscall sub_428CD0(_DWORD *this, int a2)
{
  int result; // eax

  if ( a2 )
  {
    sub_427F80(this, -1);
    result = -2147483637;
    if ( !this[78] && !this[81] )
    {
      sub_41C100((int)this);
      return sub_428D90(this);
    }
  }
  else
  {
    result = -2147483636;
    if ( this[78] && this[81] )
    {
      sub_41C100((int)this);
      return sub_428D90(0);
    }
  }
  return result;
}

// ===== sub_428D50 @ 0x00428D50..0x00428D84 =====
int __thiscall sub_428D50(void *this, int a2)
{
  int v2; // edx
  int v3; // ecx

  if ( a2 != sub_41C100((int)this) )
    return 0;
  *(_DWORD *)(v3 + 324) = v2;
  *(_DWORD *)(v3 + 312) = v2 != 0;
  return 1;
}

// ===== sub_428D90 @ 0x00428D90..0x00428DDB =====
int __usercall sub_428D90@<eax>(void *a1@<ecx>, int a2@<edi>, int a3@<esi>)
{
  int v3; // ecx
  void *v5; // ecx
  int v6; // edx

  if ( a1 )
  {
    if ( *(_DWORD *)(a3 + 328) )
    {
      return -2147483635;
    }
    else
    {
      sub_428D50(a1, a2);
      *(_DWORD *)(a3 + 328) = v3;
      *(_DWORD *)(a3 + 332) = a2;
      return 0;
    }
  }
  else
  {
    v5 = *(void **)(a3 + 328);
    if ( v5 )
    {
      sub_428D50(v5, a2);
      *(_DWORD *)(a3 + 328) = v6;
      return 0;
    }
    else
    {
      return -2147483634;
    }
  }
}

// ===== sub_428DE0 @ 0x00428DE0..0x00428E67 =====
int __userpurge sub_428DE0@<eax>(int a1@<edi>, _DWORD *a2@<esi>, unsigned int a3)
{
  int result; // eax
  unsigned int v4; // ebx
  int v5; // eax

  result = 0;
  v4 = a3;
  if ( a2[77] == 4 )
  {
    if ( sub_409FF0(a2[205], dword_565B30, &a3, a1, a2[203]) )
    {
      return -2147483640;
    }
    else if ( a3 )
    {
      a2[206] = a1;
      if ( v4 > 0x100 )
      {
        v5 = (*(int (__thiscall **)(_DWORD *))(*a2 + 76))(a2);
        (*(void (__thiscall **)(_DWORD *, int))(*a2 + 72))(a2, v5);
      }
      else
      {
        (*(void (__thiscall **)(_DWORD *, unsigned int))(*a2 + 72))(a2, v4);
      }
      return 0;
    }
    else
    {
      return -2147483639;
    }
  }
  return result;
}

// ===== sub_428E70 @ 0x00428E70..0x00428FAA =====
int __userpurge sub_428E70@<eax>(_DWORD *a1@<edi>, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // ecx
  _BYTE v5[24]; // [esp+8h] [ebp-34h] BYREF
  _BYTE v6[8]; // [esp+20h] [ebp-1Ch] BYREF
  int v7; // [esp+28h] [ebp-14h]
  int v8; // [esp+2Ch] [ebp-10h]
  int v9; // [esp+30h] [ebp-Ch]

  if ( a1[77] != 5 )
  {
    result = a1[77] - 6;
    if ( a1[77] != 6 )
      return result;
    goto LABEL_13;
  }
  if ( a2
    || (a1[163]
     || (result = 0x10000, a1[167] != 0x10000)
     || a1[168] != 0x10000
     || a1[169]
     || a1[170]
     || a1[192] && a1[194])
    && (result = a1[142], result != a1[144]) )
  {
LABEL_13:
    result = sub_428FB0();
    if ( a1[85] != -1 )
    {
      result = sub_42AAA0(&a2, 0);
      if ( result )
      {
        result = sub_42AAA0(0, 1);
        if ( result )
        {
          v3 = dword_565B30;
          result = sub_408300(a1[84], dword_565B30);
          if ( a1[86] == result )
          {
            result = sub_408300(a1[85], v3);
            if ( a1[87] == result )
            {
              sub_409030(v9, v8, a1 + 136, v7);
              sub_40C0F0((int)(a1 + 136), (int)v6, (int)v5, a1[144], 1);
              result = a1[144];
              v4 = a2;
              a1[142] = result;
              a1[143] = v4;
            }
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_428FB0 @ 0x00428FB0..0x00428FF7 =====
BOOL __usercall sub_428FB0@<eax>(int a1@<esi>)
{
  int v1; // eax
  BOOL v2; // edi

  v1 = *(_DWORD *)(a1 + 544);
  v2 = v1 != 0;
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 544));
    *(_DWORD *)(a1 + 544) = 0;
    *(_DWORD *)(a1 + 548) = 0;
    *(_DWORD *)(a1 + 552) = 0;
    *(_DWORD *)(a1 + 556) = 0;
    *(_DWORD *)(a1 + 560) = 0;
    *(_DWORD *)(a1 + 564) = 0;
  }
  return v2;
}

// ===== sub_429000 @ 0x00429000..0x004290E9 =====
int __usercall sub_429000@<eax>(_DWORD *a1@<edi>)
{
  int v1; // eax
  int result; // eax
  bool v3; // cf
  char *v4; // ebx
  char v5; // [esp+Ch] [ebp-1Ch] BYREF
  int v6; // [esp+24h] [ebp-4h] BYREF

  v1 = a1[77];
  v3 = v1 == 5;
  result = v1 - 5;
  if ( v3 || result == 1 )
  {
    result = sub_4290F0();
    if ( a1[192] )
    {
      if ( a1[194] )
      {
        result = sub_42AAA0(&v6, 0);
        if ( result )
        {
          if ( a1[85] == -1 )
          {
            result = sub_408300(a1[84], dword_565B30);
            if ( a1[86] == result )
            {
              v4 = &v5;
LABEL_8:
              sub_409030(
                *((_DWORD *)v4 + 4),
                *((_DWORD *)v4 + 3),
                a1 + 195,
                *((_DWORD *)v4 + 2)
              + ((((unsigned int)(*((_DWORD *)v4 + 2) * (a1[194] + 0x10000)) >> 16) - *((_DWORD *)v4 + 2) + 1) & 0xFFFFFFFE));
              return sub_418470((int)v4, (int)(a1 + 195), a1[192] >> v6, a1[193] >> v6, a1[194]);
            }
          }
          else
          {
            v4 = (char *)(a1 + 136);
            if ( a1[136] && a1 != (_DWORD *)-544 )
              goto LABEL_8;
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_4290F0 @ 0x004290F0..0x00429137 =====
BOOL __usercall sub_4290F0@<eax>(int a1@<esi>)
{
  int v1; // eax
  BOOL v2; // edi

  v1 = *(_DWORD *)(a1 + 780);
  v2 = v1 != 0;
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 780));
    *(_DWORD *)(a1 + 780) = 0;
    *(_DWORD *)(a1 + 784) = 0;
    *(_DWORD *)(a1 + 788) = 0;
    *(_DWORD *)(a1 + 792) = 0;
    *(_DWORD *)(a1 + 796) = 0;
    *(_DWORD *)(a1 + 800) = 0;
  }
  return v2;
}

// ===== sub_429140 @ 0x00429140..0x0042915A =====
void __usercall sub_429140(int a1@<esi>)
{
  operator delete[](*(void **)(a1 + 816));
  *(_DWORD *)(a1 + 816) = 0;
}

// ===== sub_429160 @ 0x00429160..0x004291A7 =====
BOOL __usercall sub_429160@<eax>(int a1@<esi>)
{
  int v1; // eax
  BOOL v2; // edi

  v1 = *(_DWORD *)(a1 + 832);
  v2 = v1 != 0;
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 832));
    *(_DWORD *)(a1 + 832) = 0;
    *(_DWORD *)(a1 + 836) = 0;
    *(_DWORD *)(a1 + 840) = 0;
    *(_DWORD *)(a1 + 844) = 0;
    *(_DWORD *)(a1 + 848) = 0;
    *(_DWORD *)(a1 + 852) = 0;
  }
  return v2;
}

// ===== sub_4291B0 @ 0x004291B0..0x004291D5 =====
int __usercall sub_4291B0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = 0;
  if ( *(_DWORD *)(a1 + 740) )
  {
    operator delete[](*(void **)(a1 + 740));
    *(_DWORD *)(a1 + 740) = 0;
    return 1;
  }
  return result;
}

// ===== sub_4291E0 @ 0x004291E0..0x00429217 =====
int __fastcall sub_4291E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
  return sub_429220(a5, a6, a7, 0, a8, a9, a2, 0, 0, a10, a11);
}

// ===== sub_429220 @ 0x00429220..0x004296BF =====
_DWORD *__fastcall sub_429220(
        _DWORD *a1,
        _DWORD *a2,
        _DWORD *a3,
        unsigned int a4,
        unsigned int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9,
        int a10,
        int a11,
        int a12,
        int a13)
{
  long double v13; // st7
  double v14; // st6
  double v15; // st5
  long double v16; // st3
  double v17; // st6
  double v18; // st5
  double v19; // st6
  double v20; // st7
  double v21; // st5
  double v22; // st4
  double v23; // st3
  double v24; // st1
  double v25; // st6
  double v26; // rtt
  double v27; // st3
  double v28; // st5
  double v29; // rt0
  double v30; // st3
  double v31; // st4
  double v32; // st7
  double v33; // st5
  double v34; // st2
  double v35; // st4
  int v36; // edi
  int v37; // eax
  double v38; // st2
  double v39; // st3
  double v40; // st7
  int v41; // esi
  int v42; // eax
  double v43; // st3
  double v44; // rt1
  double v45; // st4
  double v46; // st6
  int v47; // ebx
  int v48; // eax
  int v49; // ecx
  _DWORD *result; // eax
  double v51; // [esp+10h] [ebp-CCh]
  double v52; // [esp+20h] [ebp-BCh]
  double v53; // [esp+28h] [ebp-B4h]
  long double v54; // [esp+38h] [ebp-A4h]
  long double v55; // [esp+48h] [ebp-94h]
  double v56; // [esp+58h] [ebp-84h]
  double v57; // [esp+60h] [ebp-7Ch]
  double v58; // [esp+70h] [ebp-6Ch]
  double v59; // [esp+80h] [ebp-5Ch]
  double v60; // [esp+90h] [ebp-4Ch]
  double v61; // [esp+98h] [ebp-44h]
  double v62; // [esp+A8h] [ebp-34h]
  double v63; // [esp+B0h] [ebp-2Ch]
  double v64; // [esp+B8h] [ebp-24h]
  double v65; // [esp+C0h] [ebp-1Ch]
  double v66; // [esp+C8h] [ebp-14h]

  v52 = (double)(a4 * (a6 + 0x10000)) * 0.0000152587890625;
  v51 = (v52 - (double)a4) * 0.5 + (double)a12 * 0.0000152587890625;
  v53 = 0.0000152587890625 * (double)a13;
  v55 = (double)a7 * 3.141592653589793 / 11796480.0;
  v54 = cos(v55);
  v13 = sin(v55);
  v14 = (double)a8 * 0.0000152587890625;
  v15 = 0.0000152587890625 * (double)a9;
  v56 = v52 - v51 - 1.0;
  v57 = -v51;
  v59 = v53 - (double)a5 + 1.0;
  v58 = v53 * v15;
  v60 = v57 * v14 * v54 - v58 * v13;
  v63 = v57 * v14 * v13 + v58 * v54;
  v61 = v56 * v14 * v54 - v58 * v13;
  v64 = v58 * v54 + v56 * v14 * v13;
  v16 = v57 * v14 * v54 - v59 * v15 * v13;
  v65 = v57 * v14 * v13 + v59 * v15 * v54;
  v17 = v14 * v56;
  v18 = v15 * v59;
  v62 = v17 * v54 - v18 * v13;
  v19 = v13 * v17 + v18 * v54;
  v20 = v16;
  v66 = v19;
  v21 = 1000000000.0;
  v22 = -1000000000.0;
  if ( v60 < 1000000000.0 )
    v21 = v60;
  if ( v60 > -1000000000.0 )
    v22 = v60;
  v23 = v63;
  if ( v63 <= -1000000000.0 )
    v24 = -1000000000.0;
  else
    v24 = v63;
  v25 = v24;
  if ( v63 >= 1000000000.0 )
    v23 = 1000000000.0;
  if ( v61 < v21 )
    v21 = v61;
  if ( v61 > v22 )
    v22 = v61;
  if ( v64 > v24 )
    v25 = v64;
  if ( v64 < v23 )
    v23 = v64;
  v26 = v23;
  v27 = v21;
  v28 = v26;
  if ( v27 > v20 )
    v27 = v20;
  v29 = v27;
  v30 = v22;
  v31 = v29;
  if ( v30 >= v20 )
    v20 = v30;
  if ( v65 > v25 )
    v25 = v65;
  if ( v65 < v28 )
    v28 = v65;
  if ( v62 < v31 )
    v31 = v62;
  if ( v62 > v20 )
    v20 = v62;
  if ( v66 > v25 )
    v25 = v66;
  if ( v66 < v28 )
    v28 = v66;
  v32 = v20 + (double)((a8 * a10) >> 16) * 0.0000152587890625;
  v33 = v28 - 0.0000152587890625 * (double)((a9 * a11) >> 16);
  if ( v31 < 0.0 )
  {
    v37 = (int)(v31 * 65536.0);
    v38 = v31;
    v35 = 65536.0;
    v36 = (int)v38 - (((unsigned __int16)v37 + 0xFFFF) >> 16);
  }
  else
  {
    v34 = v31;
    v35 = 65536.0;
    v36 = (int)v34;
  }
  if ( v32 < 0.0 )
  {
    v42 = (int)(v32 * v35);
    v43 = v32;
    v40 = 0.0;
    v41 = (int)v43 - (((unsigned __int16)v42 + 0xFFFF) >> 16);
  }
  else
  {
    v39 = v32;
    v40 = 0.0;
    v41 = (int)v39;
  }
  v44 = v35;
  v45 = v25;
  v46 = v44;
  if ( v45 >= v40 )
    v47 = (int)v45 + (((unsigned __int16)(int)(v45 * v46) + 0xFFFF) >> 16);
  else
    v47 = (int)v45;
  if ( v33 >= v40 )
    v48 = (int)v33 + (((unsigned __int16)(int)(v46 * v33) + 0xFFFF) >> 16);
  else
    v48 = (int)v33;
  *a1 = v41 - v36 + 1;
  v49 = v47 - v48;
  result = a3;
  *a2 = v49 + 1;
  *a3 = -v36;
  a3[1] = v47;
  return result;
}

// ===== sub_4296C0 @ 0x004296C0..0x0042987F =====
int __userpurge sub_4296C0@<eax>(
        _DWORD *a1@<eax>,
        _DWORD *a2,
        _DWORD *a3,
        _DWORD *a4,
        _DWORD *a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        unsigned int a9,
        unsigned int a10,
        __int64 a11)
{
  int v12; // edi
  __int64 v13; // rax
  __int64 v14; // rax
  __int64 v16; // [esp+40h] [ebp+2Ch]

  v12 = sub_41B750(1, (int)a1);
  *a2 = a11 + (((int)a1[171] * (unsigned __int64)(unsigned int)v12) >> 24);
  a2[1] = HIDWORD(a11) + (((int)a1[172] * (unsigned __int64)(unsigned int)v12) >> 24);
  *a3 = a6 + ((unsigned __int64)((int)a1[173] * (__int64)sub_41A690(v12)) >> 16);
  v13 = (int)a1[179];
  if ( a1[157] )
  {
    *a4 = ((a9 + (((v13 + (int)a1[177]) * (unsigned int)v12) >> 24)) * (unsigned __int64)a7) >> 16;
    v14 = ((a10 + (((v13 + (int)a1[178]) * (unsigned int)v12) >> 24)) * a8) >> 16;
  }
  else
  {
    v16 = (__int64)((int)v13 * (unsigned __int64)(unsigned int)v12) >> 24;
    *a4 = ((a7 + ((__int64)((int)a1[177] * (unsigned __int64)(unsigned int)v12) >> 24)) * (v16 + (unsigned __int64)a9)) >> 16;
    v14 = ((a8 + ((__int64)((int)a1[178] * (unsigned __int64)(unsigned int)v12) >> 24)) * (v16 + a10)) >> 16;
  }
  *a5 = v14;
  return v14;
}

// ===== sub_429880 @ 0x00429880..0x0042999B =====
int __usercall sub_429880@<eax>(int *a1@<edi>)
{
  int result; // eax
  unsigned int v2; // esi
  int v3; // ebx
  unsigned int v4; // ecx
  int v5; // edx
  int (__thiscall *v6)(int *, int, unsigned int); // eax
  int v7; // [esp-8h] [ebp-48h]
  _DWORD v8[6]; // [esp+8h] [ebp-38h] BYREF
  _DWORD v9[2]; // [esp+20h] [ebp-20h] BYREF
  int v10; // [esp+28h] [ebp-18h] BYREF
  unsigned int v11; // [esp+2Ch] [ebp-14h] BYREF
  unsigned int v12; // [esp+30h] [ebp-10h] BYREF
  int v13; // [esp+34h] [ebp-Ch] BYREF
  int v14; // [esp+38h] [ebp-8h] BYREF
  int v15; // [esp+3Ch] [ebp-4h] BYREF

  result = sub_407F20(dword_565B30, a1[84], v8);
  if ( result )
  {
    result = sub_408300(a1[84], dword_565B30);
    if ( a1[86] == result )
    {
      sub_4296C0(a1, &v10, &v15, &v14, &v13, a1[148], a1[153], a1[154], a1[155], a1[156], *((_QWORD *)a1 + 73));
      v2 = v11;
      v3 = v10;
      sub_4291E0((int)&v11, v13, (int)&v12, (int)&v11, (int)v9, v8[2], v8[3], v15, v14, v10, v11);
      result = v12;
      if ( v12 >= 2 )
      {
        v4 = v11;
        if ( v11 >= 2 )
        {
          a1[163] = v15;
          a1[167] = v14;
          a1[168] = v13;
          a1[183] = v9[0];
          a1[184] = v9[1];
          v5 = *a1;
          a1[181] = result;
          a1[182] = v4;
          v7 = result;
          v6 = *(int (__thiscall **)(int *, int, unsigned int))(v5 + 116);
          a1[161] = v3;
          a1[162] = v2;
          return v6(a1, v7, v4);
        }
      }
    }
  }
  return result;
}

// ===== sub_4299A0 @ 0x004299A0..0x00429AEC =====
_DWORD *__usercall sub_4299A0@<eax>(int a1@<eax>)
{
  _DWORD *result; // eax
  __int64 v3; // rax
  unsigned int v4; // esi
  int v5; // eax
  unsigned int v6; // esi
  unsigned int v7; // ebx
  int v8; // eax
  int v9; // ecx
  unsigned int v10; // edx
  unsigned int v11; // eax
  int v12; // ecx
  int v13; // edx
  _DWORD v14[6]; // [esp+Ch] [ebp-44h] BYREF
  int v15[2]; // [esp+24h] [ebp-2Ch] BYREF
  _DWORD v16[2]; // [esp+2Ch] [ebp-24h] BYREF
  int v17; // [esp+34h] [ebp-1Ch] BYREF
  int v18; // [esp+38h] [ebp-18h]
  unsigned int v19; // [esp+3Ch] [ebp-14h] BYREF
  unsigned int v20; // [esp+40h] [ebp-10h] BYREF
  unsigned int v21; // [esp+44h] [ebp-Ch] BYREF
  unsigned int v22; // [esp+48h] [ebp-8h] BYREF
  int v23; // [esp+4Ch] [ebp-4h] BYREF

  result = (_DWORD *)sub_407F20(dword_565B30, *(_DWORD *)(a1 + 336), v14);
  if ( result )
  {
    sub_41B4C0((_DWORD *)a1, v15);
    if ( sub_41BF00(a1) )
      v4 = 0;
    else
      v4 = *(_DWORD *)(a1 + 632);
    LODWORD(v3) = v16[0];
    sub_41AAA0(v3, v4, &v21);
    sub_4296C0(
      (_DWORD *)a1,
      &v17,
      &v23,
      &v22,
      &v21,
      *(_DWORD *)(a1 + 592),
      v21,
      v21,
      *(_DWORD *)(a1 + 620),
      *(_DWORD *)(a1 + 624),
      *(_QWORD *)(a1 + 584));
    if ( *(_DWORD *)(a1 + 768) )
      v5 = *(_DWORD *)(a1 + 776);
    else
      v5 = 0;
    result = sub_429220(&v20, &v19, v16, v14[2], v14[3], v5, v23, v22, v21, v15[0], v15[1], v17, v18);
    v6 = v20;
    if ( v20 >= 2 )
    {
      v7 = v19;
      if ( v19 >= 2 )
      {
        v8 = v18;
        v9 = v23;
        *(_DWORD *)(a1 + 644) = v17;
        v10 = v22;
        *(_DWORD *)(a1 + 648) = v8;
        v11 = v21;
        *(_DWORD *)(a1 + 652) = v9;
        v12 = v16[0];
        *(_DWORD *)(a1 + 668) = v10;
        v13 = v16[1];
        *(_DWORD *)(a1 + 672) = v11;
        *(_DWORD *)(a1 + 724) = v6;
        *(_DWORD *)(a1 + 728) = v7;
        *(_DWORD *)(a1 + 732) = v12;
        *(_DWORD *)(a1 + 736) = v13;
        sub_429AF0(a1);
        return (_DWORD *)(*(int (__thiscall **)(int, unsigned int, unsigned int))(*(_DWORD *)a1 + 116))(a1, v6, v7);
      }
    }
  }
  return result;
}

// ===== sub_429AF0 @ 0x00429AF0..0x00429BE5 =====
unsigned int *__stdcall sub_429AF0(unsigned int *a1)
{
  unsigned int v1; // ebx
  unsigned int v2; // edi
  __int64 v3; // rax
  unsigned int *v4; // ecx
  int v5; // esi
  int v6; // ebx
  unsigned int *result; // eax
  unsigned int v8; // [esp+18h] [ebp-2Ch]
  unsigned int v9; // [esp+1Ch] [ebp-28h]
  int v10; // [esp+28h] [ebp-1Ch] BYREF
  int v11; // [esp+2Ch] [ebp-18h]
  int v12; // [esp+30h] [ebp-14h]
  unsigned int v13; // [esp+38h] [ebp-Ch]
  unsigned int v14; // [esp+3Ch] [ebp-8h] BYREF

  sub_41B4C0(a1, &v10);
  sub_41C0A0();
  v1 = v8 >> 1;
  v2 = v9 >> 1;
  if ( sub_41C0C0((int)a1) )
  {
    LODWORD(v3) = sub_41C0D0();
    v4 = a1;
    if ( (_DWORD)v3 )
    {
      v1 = v13;
      v2 = v14;
    }
  }
  if ( v4[159] )
  {
    LODWORD(v3) = v12;
    sub_41AAA0(v3, v4[158], &v14);
    v5 = (v1 << 16) + ((v10 * (unsigned __int64)v14) >> 16);
    v4 = a1;
    v6 = (v2 << 16) + ((__int64)(v11 * (unsigned __int64)v14) >> 16);
  }
  else
  {
    v5 = v10 + (v1 << 16);
    v6 = v11 + (v2 << 16);
  }
  (*(void (__thiscall **)(unsigned int *, int, int, _DWORD, _DWORD))(*v4 + 40))(v4, v5 >> 16, v6 >> 16, 0, 0);
  result = a1;
  a1[169] = (unsigned __int16)v5;
  a1[170] = (unsigned __int16)v6;
  return result;
}

// ===== sub_429BF0 @ 0x00429BF0..0x00429E14 =====
int __userpurge sub_429BF0@<eax>(
        _DWORD *a1@<eax>,
        _DWORD *a2,
        _DWORD *a3,
        _DWORD *a4,
        _DWORD *a5,
        _DWORD *a6,
        _DWORD *a7,
        _DWORD *a8,
        int a9,
        int a10,
        int a11,
        unsigned int a12,
        unsigned int a13,
        unsigned int a14,
        unsigned int a15)
{
  int v16; // edi
  __int64 v17; // rax
  __int64 v18; // rax
  __int64 v20; // [esp+14h] [ebp-8h]
  __int64 v21; // [esp+14h] [ebp-8h]

  v16 = sub_41B750(1, (int)a1);
  *a2 = *a8 + (((int)a1[171] * (unsigned __int64)(unsigned int)v16) >> 24);
  a2[1] = a8[1] + (((int)a1[172] * (unsigned __int64)(unsigned int)v16) >> 24);
  v20 = sub_41A690(v16);
  *a3 = a9 + ((unsigned __int64)((int)a1[174] * v20) >> 16);
  *a4 = a10 + ((unsigned __int64)((int)a1[175] * v20) >> 16);
  *a5 = a11 + ((unsigned __int64)((int)a1[176] * v20) >> 16);
  v17 = (int)a1[179];
  if ( a1[157] )
  {
    *a6 = ((a14 + (((v17 + (int)a1[177]) * (unsigned int)v16) >> 24)) * (unsigned __int64)a12) >> 16;
    v18 = ((a15 + (((v17 + (int)a1[178]) * (unsigned int)v16) >> 24)) * a13) >> 16;
  }
  else
  {
    v21 = (__int64)((int)v17 * (unsigned __int64)(unsigned int)v16) >> 24;
    *a6 = ((a12 + ((__int64)((int)a1[177] * (unsigned __int64)(unsigned int)v16) >> 24)) * (v21 + (unsigned __int64)a14)) >> 16;
    v18 = ((a13 + ((__int64)((int)a1[178] * (unsigned __int64)(unsigned int)v16) >> 24)) * (v21 + a15)) >> 16;
  }
  *a7 = v18;
  return v18;
}

// ===== sub_429E20 @ 0x00429E20..0x00429EC1 =====
_DWORD *__stdcall sub_429E20(int a1, int *a2, _DWORD *a3, _DWORD *a4, int a5)
{
  unsigned int v5; // edi
  unsigned int v6; // ebx
  int v7; // edi
  int v8; // ebx
  _DWORD *result; // eax
  int v10; // ebx
  int v11; // edx
  unsigned int v12; // [esp+18h] [ebp-1Ch]
  unsigned int v13; // [esp+1Ch] [ebp-18h]
  unsigned int v14; // [esp+28h] [ebp-Ch]
  unsigned int v15; // [esp+2Ch] [ebp-8h]

  sub_41C0A0();
  v5 = v12 >> 1;
  v6 = v13 >> 1;
  if ( sub_41C0C0(a1) && sub_41C0D0() )
  {
    v5 = v14;
    v6 = v15;
  }
  v7 = v5 << 16;
  v8 = v6 << 16;
  if ( a5 )
  {
    *a2 = v7;
    a2[1] = v8;
    *a3 = *a4;
    a3[1] = a4[1];
    result = (_DWORD *)a4[3];
    a3[2] = a4[2];
    a3[3] = result;
  }
  else
  {
    result = a4;
    v10 = a4[1] + v8;
    v11 = a4[2];
    *a2 = *a4 + v7;
    a2[1] = v10;
    *a3 = 0;
    a3[1] = 0;
    a3[2] = v11;
  }
  return result;
}

// ===== sub_429ED0 @ 0x00429ED0..0x0042A087 =====
int __userpurge sub_429ED0@<eax>(
        int *a1@<eax>,
        int *a2@<ecx>,
        _DWORD *a3@<edi>,
        int *a4,
        int *a5,
        int *a6,
        int a7,
        int a8)
{
  int v10; // edx
  int v11; // eax
  int v12; // eax
  int v13; // ebx
  int v14; // ecx
  int v15; // ecx
  int v17; // [esp+Ch] [ebp-28h]
  int v18; // [esp+10h] [ebp-24h]
  int v19; // [esp+14h] [ebp-20h]
  int v20; // [esp+18h] [ebp-1Ch]
  int v21; // [esp+1Ch] [ebp-18h]
  int v22; // [esp+20h] [ebp-14h]
  int v23; // [esp+24h] [ebp-10h]
  int v24; // [esp+24h] [ebp-10h]
  int v25; // [esp+28h] [ebp-Ch]
  int v26; // [esp+28h] [ebp-Ch]
  char v27[8]; // [esp+2Ch] [ebp-8h] BYREF
  int v28; // [esp+4Ch] [ebp+18h]

  v28 = 0;
  *(_DWORD *)v27 = 0;
  if ( a1 )
  {
    v10 = *a1;
    v11 = a1[1];
    v23 = v10;
  }
  else
  {
    v11 = 0;
    v23 = 0;
  }
  v25 = v11;
  if ( !a2[192] || !a2[194] )
  {
    if ( a2[85] == -1 )
    {
      if ( sub_42AAA0(v27, 0) && a2[86] == sub_408300(a2[84], dword_565B30) )
      {
        *a3 = v17;
        a3[1] = v18;
        a3[2] = v19;
        a3[3] = v20;
        a3[4] = v21;
        a3[5] = v22;
        v28 = 1;
      }
      LOBYTE(v15) = v27[0];
    }
    else
    {
      *a3 = a2[136];
      a3[1] = a2[137];
      a3[2] = a2[138];
      a3[3] = a2[139];
      a3[4] = a2[140];
      a3[5] = a2[141];
      v15 = a2[143];
      v28 = 1;
    }
    v24 = v23 >> v15;
    v26 = v25 >> v15;
    v12 = a7 << v15;
    v13 = a8 << v15;
    if ( v28 )
    {
      v14 = v28;
      goto LABEL_15;
    }
    return v28;
  }
  if ( !sub_42AAA0(v27, 0) )
    return v28;
  *a3 = a2[195];
  a3[1] = a2[196];
  a3[2] = a2[197];
  a3[3] = a2[198];
  a3[4] = a2[199];
  a3[5] = a2[200];
  v26 = v25 >> v27[0];
  v12 = a7 << v27[0];
  v13 = a8 << v27[0];
  v24 = (((a2[197] - v19) << 15) & 0xFFFF0000) + (v23 >> v27[0]);
  v14 = 1;
LABEL_15:
  if ( a4 )
  {
    *a4 = v24;
    a4[1] = v26;
  }
  if ( a5 )
    *a5 = v12;
  if ( a6 )
    *a6 = v13;
  return v14;
}

// ===== sub_42A090 @ 0x0042A090..0x0042A100 =====
int __usercall sub_42A090@<eax>(int *a1@<edi>)
{
  int result; // eax
  _DWORD v2[6]; // [esp+8h] [ebp-18h] BYREF

  result = sub_407F20(dword_565B30, a1[84], v2);
  if ( result )
    return sub_42A100(a1, a1 + 146, a1[149], a1[150], a1[151], a1[152], a1[158], a1[153], a1[154], a1[155], a1[156]);
  return result;
}

// ===== sub_42A100 @ 0x0042A100..0x0042A2BB =====
int __stdcall sub_42A100(
        int a1,
        _DWORD *a2,
        int a3,
        int a4,
        int a5,
        unsigned int a6,
        int a7,
        unsigned int a8,
        unsigned int a9,
        unsigned int a10,
        unsigned int a11)
{
  int v12; // edx
  int v13; // eax
  int v15; // [esp-4h] [ebp-B8h]
  _DWORD v16[2]; // [esp+Ch] [ebp-A8h] BYREF
  int v17[4]; // [esp+14h] [ebp-A0h] BYREF
  int v18[4]; // [esp+24h] [ebp-90h] BYREF
  int v19[2]; // [esp+34h] [ebp-80h] BYREF
  int v20; // [esp+3Ch] [ebp-78h] BYREF
  int v21; // [esp+40h] [ebp-74h] BYREF
  int v22; // [esp+44h] [ebp-70h] BYREF
  int v23[2]; // [esp+48h] [ebp-6Ch] BYREF
  unsigned int v24; // [esp+50h] [ebp-64h] BYREF
  _DWORD *v25; // [esp+54h] [ebp-60h] BYREF
  float v26[21]; // [esp+58h] [ebp-5Ch] BYREF

  v25 = a2;
  sub_4291B0(a1);
  sub_41B4C0((_DWORD *)a1, v17);
  sub_429BF0((_DWORD *)a1, v23, &v22, &v21, &v20, &v25, &v24, v25, a3, a4, a5, a8, a9, a10, a11);
  sub_429E20(a1, v19, v18, v17, *(_DWORD *)(a1 + 636));
  sub_429ED0(v23, (int *)a1, v16, v23, (int *)&v25, (int *)&v24, (int)v25, v24);
  sub_40F630(
    v26,
    (int)v16,
    v19[0],
    v19[1],
    v23[0],
    v23[1],
    (unsigned int)v25,
    v24,
    v18[0],
    v18[1],
    v18[2],
    v22,
    v21,
    v20,
    a6,
    a7);
  sub_41C0A0();
  sub_40EA40(v26, (float **)(a1 + 740), (int *)(a1 + 744), (int *)(a1 + 748), (int *)(a1 + 752), v17[1]);
  if ( !*(_DWORD *)(a1 + 740) )
    return (*(int (__thiscall **)(int, int, int))(*(_DWORD *)a1 + 116))(a1, 1, 1);
  v12 = *(_DWORD *)a1;
  v13 = *(_DWORD *)(a1 + 760) - *(_DWORD *)(a1 + 752) + 1;
  v15 = *(_DWORD *)(a1 + 764) - *(_DWORD *)(a1 + 756) + 1;
  *(_DWORD *)(a1 + 724) = v13;
  *(_DWORD *)(a1 + 728) = v15;
  (*(void (__thiscall **)(int, int, int))(v12 + 116))(a1, v13, v15);
  return (*(int (__thiscall **)(int, _DWORD, _DWORD, _DWORD, int))(*(_DWORD *)a1 + 40))(
           a1,
           *(_DWORD *)(a1 + 752),
           *(_DWORD *)(a1 + 756),
           0,
           1);
}

// ===== sub_42A2C0 @ 0x0042A2C0..0x0042A312 =====
int __usercall sub_42A2C0@<eax>(_DWORD *a1@<ecx>, _DWORD *a2@<edi>, int *a3@<esi>)
{
  int result; // eax
  int v4; // eax
  int v5; // edx
  int v6; // ecx
  int v7; // edx

  result = 0;
  switch ( a1[77] )
  {
    case 0:
    case 1:
    case 3:
      (*(void (__thiscall **)(_DWORD *, int *))(*a1 + 36))(a1, a3);
      v4 = *a3;
      v5 = a2[3];
      a3[2] = *a3 + a2[2];
      v6 = a3[1];
      a3[3] = v6 + v5;
      v7 = v4 + *a2;
      a3[1] = v6 + a2[1];
      *a3 = v7;
      result = 1;
      break;
    case 5:
      result = sub_42A330(a3, a2);
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_42A330 @ 0x0042A330..0x0042A498 =====
int __userpurge sub_42A330@<eax>(int a1@<eax>, unsigned int a2, _DWORD *a3)
{
  _DWORD *v3; // ebx
  __int64 v5; // rax
  unsigned int v6; // esi
  _DWORD *v7; // eax
  int v8; // ecx
  unsigned int v9; // esi
  unsigned int v10; // ecx
  unsigned int v11; // edi
  unsigned int v12; // edx
  int v13; // ecx
  int v15; // [esp+10h] [ebp-30h] BYREF
  int v16[4]; // [esp+18h] [ebp-28h] BYREF
  int v17; // [esp+28h] [ebp-18h] BYREF
  int v18; // [esp+2Ch] [ebp-14h]
  int v19; // [esp+30h] [ebp-10h] BYREF
  int v20; // [esp+34h] [ebp-Ch] BYREF
  unsigned int v21; // [esp+38h] [ebp-8h] BYREF
  unsigned int v22; // [esp+3Ch] [ebp-4h] BYREF

  v3 = (_DWORD *)a2;
  if ( !sub_407F20(dword_565B30, *(_DWORD *)(a1 + 336), &v15) )
    return 0;
  sub_41B4C0((_DWORD *)a1, v16);
  if ( sub_41BF00(a1) )
    v6 = 0;
  else
    v6 = *(_DWORD *)(a1 + 632);
  LODWORD(v5) = v16[2];
  sub_41AAA0(v5, v6, &a2);
  sub_4296C0(
    (_DWORD *)a1,
    &v17,
    &v20,
    &v22,
    &v21,
    *(_DWORD *)(a1 + 592),
    a2,
    a2,
    *(_DWORD *)(a1 + 620),
    *(_DWORD *)(a1 + 624),
    *(_QWORD *)(a1 + 584));
  v7 = a3;
  a3 = (_DWORD *)*a3;
  v17 -= (_DWORD)a3 << 16;
  v8 = v7[1];
  if ( *(_DWORD *)(a1 + 768) )
    a2 = *(_DWORD *)(a1 + 776);
  else
    a2 = 0;
  v9 = v21;
  sub_429220(
    &a2,
    &a3,
    &v19,
    v7[2] - (_DWORD)a3 + 1,
    v7[3] - v8 + 1,
    a2,
    v20,
    v22,
    v21,
    v16[0],
    v16[1],
    v17,
    v18 - (v8 << 16));
  sub_41B260((_DWORD *)a1, &v17);
  v10 = v22;
  v11 = v17 - v19 - HIWORD(v22);
  v3[2] = v17 - v19 + a2 - 1;
  v12 = v18 - v20 - ((v10 - 1) >> 17);
  v13 = (int)a3 + v18 - v20 + (v9 >> 17) - 1;
  *v3 = v11;
  v3[1] = v12;
  v3[3] = v13;
  return 1;
}

// ===== sub_42A4A0 @ 0x0042A4A0..0x0042A641 =====
int __fastcall sub_42A4A0(_DWORD *a1, _DWORD *a2, int a3, int *a4)
{
  int v5; // eax
  int v6; // esi
  int v7; // edx
  int v8; // eax
  int v9; // ecx
  int v10; // edx
  int v11; // eax
  int v12; // ecx
  int v13; // edx
  int v14; // eax
  int v16[4]; // [esp+Ch] [ebp-54h] BYREF
  int v17; // [esp+1Ch] [ebp-44h]
  int v18; // [esp+20h] [ebp-40h]
  int v19[4]; // [esp+28h] [ebp-38h] BYREF
  int v20; // [esp+38h] [ebp-28h] BYREF
  int v21; // [esp+3Ch] [ebp-24h]
  _DWORD *v22; // [esp+40h] [ebp-20h]
  int v23; // [esp+44h] [ebp-1Ch] BYREF
  int v24; // [esp+48h] [ebp-18h]
  int v25[4]; // [esp+4Ch] [ebp-14h] BYREF

  v24 = a3;
  v22 = a1;
  v5 = sub_493090();
  if ( !sub_407F20(dword_565B30, v5, v16) )
    return 0;
  v6 = v24;
  (*(void (__thiscall **)(int, int *))(*(_DWORD *)v24 + 52))(v24, &v20);
  (*(void (__thiscall **)(_DWORD, int *))(**(_DWORD **)(v6 + 324) + 52))(*(_DWORD *)(v6 + 324), &v23);
  v7 = a4[1];
  v8 = a4[2];
  v19[0] = *a4;
  v9 = a4[3];
  v19[1] = v7;
  v19[3] = v9;
  v19[2] = v8;
  sub_409170(v21, v20, v19);
  (*(void (__thiscall **)(_DWORD, int *))(**(_DWORD **)(v6 + 324) + 36))(*(_DWORD *)(v6 + 324), v25);
  if ( sub_4090B0(v25, v19) )
  {
    v10 = v16[1];
    v11 = v16[2];
    *a2 = v16[0];
    v12 = v16[3];
    a2[1] = v10;
    v13 = v17;
    a2[2] = v11;
    v14 = v18;
    a2[3] = v12;
    a2[4] = v13;
    a2[5] = v14;
    sub_409110(v19, v25);
    sub_409170(-v24, -v23, v19);
    sub_4091B0(a2, v19);
    *v22 = 0;
  }
  else
  {
    sub_409030(v17, a4[3] - a4[1] + 1, a2, a4[2] - *a4 + 1);
    sub_40A620((int)a2, 0);
    sub_40A530(v16, a2, v24 - a4[1] - v21, v23 - *a4 - v20, 128, 0);
    *v22 = 1;
  }
  return 1;
}

// ===== sub_42A650 @ 0x0042A650..0x0042A763 =====
unsigned int __stdcall sub_42A650(int *a1)
{
  unsigned int result; // eax
  _DWORD *v3; // esi
  _DWORD *v4; // edi
  _DWORD *v5; // ebx
  unsigned int v6; // ecx
  unsigned int v7; // ecx
  _DWORD v8[6]; // [esp+Ch] [ebp-38h] BYREF
  _DWORD v9[6]; // [esp+24h] [ebp-20h] BYREF
  int v10; // [esp+3Ch] [ebp-8h]
  _DWORD *v11; // [esp+4Ch] [ebp+8h]

  sub_42A9C0(a1);
  if ( !sub_407F20(dword_565B30, a1[84], v9) )
    memset(v9, 0, sizeof(v9));
  result = sub_407F20(dword_565B30, a1[85], v8);
  if ( !result )
    memset(v8, 0, sizeof(v8));
  v3 = v8;
  v4 = v9;
  v11 = v8;
  v5 = a1 + 112;
  v10 = 4;
  while ( 1 )
  {
    if ( *v4 )
    {
      v6 = v4[2];
      if ( v6 >= 2 )
      {
        result = v4[3];
        if ( result >= 2 )
        {
          result = sub_409030(v4[4], (result + 1) >> 1, v5 - 24, (v6 + 1) >> 1);
          if ( result )
            result = sub_418FA0((int)v4, v5 - 24);
          v3 = v11;
        }
      }
    }
    if ( *v3 )
    {
      v7 = v3[2];
      if ( v7 >= 2 )
      {
        result = v3[3];
        if ( result >= 2 )
        {
          result = sub_409030(v3[4], (result + 1) >> 1, v5, (v7 + 1) >> 1);
          if ( result )
            result = sub_418FA0((int)v11, v5);
        }
      }
    }
    v4 = v5 - 24;
    v11 = v5;
    v5 += 6;
    if ( !--v10 )
      break;
    v3 = v11;
  }
  return result;
}

// ===== sub_42A770 @ 0x0042A770..0x0042A9B4 =====
signed int __userpurge sub_42A770@<eax>(signed int *a1@<eax>, int *a2)
{
  signed int result; // eax
  int v5; // edx
  signed int v6; // esi
  int v7; // ecx
  int *v8; // edi
  int v9; // ebx
  bool v10; // zf
  int v11; // ecx
  unsigned int v12; // edx
  unsigned int v13; // ecx
  int v14; // edx
  int v15; // eax
  int v16; // eax
  int v17; // ecx
  int v18; // eax
  int v19; // ecx
  int v20; // edx
  unsigned int v21; // ecx
  unsigned int v22; // edx
  int v23; // ecx
  int v24; // edx
  int v25; // edx
  int v26; // eax
  int v27; // edx
  int v28; // eax
  int *v29; // edx
  _DWORD v30[6]; // [esp+10h] [ebp-8Ch] BYREF
  _DWORD v31[6]; // [esp+28h] [ebp-74h] BYREF
  int v32[4]; // [esp+40h] [ebp-5Ch] BYREF
  int v33; // [esp+50h] [ebp-4Ch] BYREF
  int v34; // [esp+54h] [ebp-48h]
  int v35; // [esp+58h] [ebp-44h]
  int v36; // [esp+5Ch] [ebp-40h]
  int v37; // [esp+60h] [ebp-3Ch] BYREF
  int v38; // [esp+64h] [ebp-38h]
  int v39; // [esp+68h] [ebp-34h]
  int v40; // [esp+6Ch] [ebp-30h]
  int v41; // [esp+70h] [ebp-2Ch]
  int v42; // [esp+74h] [ebp-28h]
  int *v43; // [esp+78h] [ebp-24h]
  int v44; // [esp+7Ch] [ebp-20h]
  int v45; // [esp+80h] [ebp-1Ch] BYREF
  int v46; // [esp+84h] [ebp-18h]
  unsigned int v47; // [esp+88h] [ebp-14h]
  unsigned int v48; // [esp+8Ch] [ebp-10h]
  int v49; // [esp+90h] [ebp-Ch]
  int v50; // [esp+94h] [ebp-8h]
  int *v51; // [esp+A4h] [ebp+8h]

  if ( !a1 )
    return sub_42A650(a2);
  if ( !sub_407F20(dword_565B30, a2[84], v31) )
    memset(v31, 0, sizeof(v31));
  if ( !sub_407F20(dword_565B30, a2[85], v30) )
    memset(v30, 0, sizeof(v30));
  v5 = a1[2];
  v6 = a1[3];
  v51 = v31;
  result = *a1;
  v43 = v30;
  v7 = a1[1];
  v8 = a2 + 112;
  v44 = 4;
  do
  {
    if ( v5 - result <= 0 )
    {
      if ( result < 1 )
        ++v5;
      else
        --result;
    }
    if ( v6 - v7 <= 0 )
    {
      if ( v7 < 1 )
        ++v6;
      else
        --v7;
    }
    v32[0] = result & 0xFFFFFFFE;
    v9 = (int)(result & 0xFFFFFFFE) >> 1;
    v32[1] = v7 & 0xFFFFFFFE;
    v32[2] = v5 | 1;
    v32[3] = v6 | 1;
    v10 = *v51 == 0;
    v33 = v9;
    v34 = (int)(v7 & 0xFFFFFFFE) >> 1;
    v35 = (v5 | 1) >> 1;
    v36 = (v6 | 1) >> 1;
    if ( !v10 )
    {
      v11 = v51[1];
      v45 = *v51;
      v12 = v51[2];
      v46 = v11;
      v13 = v51[3];
      v47 = v12;
      v14 = v51[4];
      v15 = v51[5];
      v48 = v13;
      v49 = v14;
      v50 = v15;
      if ( sub_4091B0(&v45, v32) )
      {
        if ( v47 >= 2 && v48 >= 2 )
        {
          v16 = *(v8 - 23);
          v17 = *(v8 - 22);
          v37 = *(v8 - 24);
          v40 = *(v8 - 21);
          v38 = v16;
          v18 = *(v8 - 20);
          v39 = v17;
          v19 = *(v8 - 19);
          v41 = v18;
          v42 = v19;
          sub_4091B0(&v37, &v33);
          sub_418FA0((int)&v45, &v37);
        }
      }
    }
    if ( *v43 )
    {
      v20 = v43[1];
      v45 = *v43;
      v21 = v43[2];
      v46 = v20;
      v22 = v43[3];
      v47 = v21;
      v23 = v43[4];
      v48 = v22;
      v24 = v43[5];
      v49 = v23;
      v50 = v24;
      if ( sub_4091B0(&v45, v32) )
      {
        if ( v47 >= 2 && v48 >= 2 )
        {
          v25 = v8[1];
          v26 = v8[2];
          v37 = *v8;
          v40 = v8[3];
          v38 = v25;
          v27 = v8[4];
          v39 = v26;
          v28 = v8[5];
          v41 = v27;
          v42 = v28;
          sub_4091B0(&v37, &v33);
          sub_418FA0((int)&v45, &v37);
        }
      }
    }
    v7 = v34;
    v6 = v36;
    v29 = v8 - 24;
    v43 = v8;
    v8 += 6;
    v10 = v44-- == 1;
    v51 = v29;
    v5 = v35;
    result = v9;
  }
  while ( !v10 );
  return result;
}

// ===== sub_42A9C0 @ 0x0042A9C0..0x0042AA15 =====
void *__stdcall sub_42A9C0(int a1)
{
  void **v1; // esi
  int v2; // edi

  v1 = (void **)(a1 + 448);
  v2 = 4;
  do
  {
    operator delete[](*(v1 - 24));
    operator delete[](*v1);
    v1 += 6;
    --v2;
  }
  while ( v2 );
  memset((void *)(a1 + 352), 0, 0x60u);
  return memset((void *)(a1 + 448), 0, 0x60u);
}

// ===== sub_42AA20 @ 0x0042AA20..0x0042AA92 =====
int __usercall sub_42AA20@<eax>(_DWORD *a1@<eax>)
{
  int v2; // ecx
  int result; // eax
  int v4; // ecx
  unsigned int v5; // ebx
  __int64 v6; // rax
  unsigned int v7; // ecx
  int v8[4]; // [esp+10h] [ebp-18h] BYREF
  unsigned int v9; // [esp+20h] [ebp-8h] BYREF
  int v10; // [esp+24h] [ebp-4h]

  v2 = a1[77];
  result = 0;
  v4 = v2 - 5;
  v10 = 0;
  v5 = 0x8000;
  if ( v4 )
  {
    if ( v4 == 1 )
    {
      sub_41B4C0(a1, v8);
      LODWORD(v6) = v8[2];
      sub_41AAA0(v6, a1[158], &v9);
      if ( v9 <= 0x8000 )
      {
        do
        {
          ++v10;
          v5 >>= 1;
        }
        while ( v5 >= v9 );
      }
      return v10;
    }
  }
  else
  {
    v7 = a1[167];
    if ( v7 <= 0x8000 )
    {
      do
      {
        v5 >>= 1;
        ++result;
      }
      while ( v5 >= v7 );
    }
  }
  return result;
}

// ===== sub_42AAA0 @ 0x0042AAA0..0x0042AB47 =====
int __userpurge sub_42AAA0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>, unsigned int *a3, int a4)
{
  int v7; // ecx
  unsigned int v8; // eax
  _DWORD *v9; // ecx
  unsigned int v10; // edx
  int v11; // edi
  int v13; // [esp+1Ch] [ebp+Ch]

  if ( a4 )
    v7 = a2[85];
  else
    v7 = a2[84];
  v13 = sub_407F20(dword_565B30, v7, a1);
  v8 = sub_42AA20(a2);
  if ( v13 && v8 )
  {
    v9 = a2 + 88;
    if ( a4 )
      v9 = a2 + 112;
    if ( v8 > 4 )
      v8 = 4;
    v10 = 0;
    while ( *v9 )
    {
      *a1 = *v9;
      a1[1] = v9[1];
      a1[2] = v9[2];
      a1[3] = v9[3];
      a1[4] = v9[4];
      v11 = v9[5];
      ++v10;
      v9 += 6;
      a1[5] = v11;
      if ( v10 >= v8 )
        goto LABEL_15;
    }
    v8 = v10;
  }
LABEL_15:
  if ( a3 )
    *a3 = v8;
  return v13;
}

// ===== sub_42AB50 @ 0x0042AB50..0x0042AB99 =====
int __fastcall sub_42AB50(unsigned int a1, int a2, unsigned int a3, int a4, int a5)
{
  int result; // eax
  int v6; // eax

  result = -2147483633;
  if ( a1 < 0x10 )
  {
    if ( a3 > 5 )
    {
      return -2147483632;
    }
    else
    {
      v6 = a2 + 12 * a1;
      *(_DWORD *)(v6 + 856) = a3;
      *(_DWORD *)(v6 + 860) = a4;
      *(_DWORD *)(a2 + 4 * (3 * a1 + 216)) = a5;
      return 0;
    }
  }
  return result;
}

// ===== sub_42ABA0 @ 0x0042ABA0..0x0042ABD3 =====
int __thiscall sub_42ABA0(_DWORD *this)
{
  int result; // eax
  _DWORD *v2; // ecx
  int v3; // edx

  result = 0;
  v2 = this + 217;
  v3 = 4;
  do
  {
    if ( result || *(v2 - 3) || *v2 || v2[3] || v2[6] )
      result = 1;
    v2 += 12;
    --v3;
  }
  while ( v3 );
  return result;
}

// ===== sub_42ABE0 @ 0x0042ABE0..0x0042AC42 =====
int __userpurge sub_42ABE0@<eax>(size_t a1@<eax>, _DWORD *a2@<ecx>, size_t *a3)
{
  int result; // eax
  int v7; // ecx
  unsigned int *v8; // esi
  int v9; // [esp+Ch] [ebp-4h]
  int v10; // [esp+18h] [ebp+8h]

  result = sub_42ABA0(a2);
  v7 = result;
  v9 = result;
  if ( result )
  {
    if ( !a1 )
      a1 = (size_t)a3;
    v8 = a2 + 214;
    v10 = 16;
    do
    {
      if ( *v8 )
      {
        sub_4199D0(a3, a1, *v8, v8[1], v8[2], 1);
        v7 = v9;
        a1 = (size_t)a3;
      }
      v8 += 3;
      --v10;
    }
    while ( v10 );
    return v7;
  }
  return result;
}

// ===== sub_42AC50 @ 0x0042AC50..0x0042ACB0 =====
int __thiscall sub_42AC50(void *this, _DWORD *a2, int a3)
{
  sub_41A400((int)this, a2, 8, 1);
  *a2 = &CDspObjVirtual::`vftable';
  return sub_41ACD0((int)a2, a3);
}

// ===== sub_42ACB0 @ 0x0042ACB0..0x0042ACD2 =====
void *__thiscall sub_42ACB0(void *this, char a2)
{
  sub_42ACE0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42ACE0 @ 0x0042ACE0..0x0042AD28 =====
int __stdcall sub_42ACE0(_DWORD *a1)
{
  *a1 = &CDspObjVirtual::`vftable';
  return sub_41A5E0(a1);
}

// ===== sub_42AD30 @ 0x0042AD30..0x0042AD49 =====
int __thiscall sub_42AD30(void *this)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_41ACE0((int)this);
  if ( v1 )
    return (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 8))(v1);
  else
    return sub_41AE30(v2);
}

// ===== sub_42AD50 @ 0x0042AD50..0x0042AD69 =====
int __thiscall sub_42AD50(void *this)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_41ACE0((int)this);
  if ( v1 )
    return (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 88))(v1);
  else
    return sub_41B8D0(v2);
}

// ===== sub_42AD70 @ 0x0042AD70..0x0042AD8F =====
int __thiscall sub_42AD70(void *this)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_41ACE0((int)this);
  if ( v1 )
    return (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 28))(v1) + 0x8000;
  else
    return sub_41B0C0(v2);
}

// ===== sub_42AD90 @ 0x0042AD90..0x0042AE1B =====
int __thiscall sub_42AD90(_DWORD *this, int a2, int a3, int a4)
{
  int v5; // esi
  _DWORD v7[2]; // [esp+Ch] [ebp-10h] BYREF
  _DWORD v8[2]; // [esp+14h] [ebp-8h] BYREF

  if ( !sub_41BDF0(this, a2, a3, a4) )
    return 0;
  v5 = sub_41ACE0((int)this);
  if ( !v5 )
    return 1;
  (*(void (__thiscall **)(_DWORD *, _DWORD *))(*this + 48))(this, v8);
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v5 + 48))(v5, v7);
  return (*(int (__thiscall **)(int, int, int, int))(*(_DWORD *)v5 + 100))(
           v5,
           a2 + v8[0] - v7[0],
           a3 + v8[1] - v7[1],
           a4);
}

// ===== sub_42AE20 @ 0x0042AE20..0x0042AFD4 =====
_DWORD *__thiscall sub_42AE20(void *this, _DWORD *a2)
{
  int v2; // esi
  int v3; // eax
  int v4; // eax
  int v5; // eax
  _DWORD *v6; // ebx
  int v7; // eax

  sub_41A400((int)this, a2, 3, 1);
  v2 = 0;
  *a2 = &CDspObjWindow::`vftable';
  if ( operator new(0x10u) )
    v3 = sub_430260();
  else
    v3 = 0;
  a2[80] = v3;
  if ( operator new(0x10u) )
    v4 = sub_430260();
  else
    v4 = 0;
  a2[88] = v4;
  if ( operator new(0x10u) )
    v5 = sub_430260();
  else
    v5 = 0;
  a2[96] = v5;
  v6 = a2 + 110;
  do
  {
    if ( operator new(0x10u) )
      v7 = sub_430260();
    else
      v7 = 0;
    *(v6 - 1) = v7;
    *v6 = 0;
    sub_42BAE0(0, 0, 0);
    sub_42BAB0(0);
    ++v2;
    v6 += 11;
  }
  while ( v2 < 8 );
  a2[196] = 0;
  a2[197] = 0;
  a2[208] = 0;
  a2[79] = 0;
  sub_42B3A0(a2);
  sub_42B3B0(a2);
  sub_42B550(0);
  sub_42B540();
  sub_42CA00();
  sub_41B600((int)a2, 1);
  sub_41B620(a2, 0);
  a2[212] = 0;
  a2[214] = 0;
  sub_42C470(0, a2);
  sub_42C510();
  sub_42C530();
  sub_42C550();
  sub_42C580(a2);
  sub_42C7A0(a2, 0);
  sub_42C870(a2);
  return a2;
}

// ===== sub_42AFE0 @ 0x0042AFE0..0x0042B002 =====
void *__thiscall sub_42AFE0(void *this, char a2)
{
  sub_42B010(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42B010 @ 0x0042B010..0x0042B0C4 =====
int __stdcall sub_42B010(_DWORD *a1)
{
  void (__thiscall ***v1)(_DWORD, int); // ecx
  void (__thiscall ***v2)(_DWORD, int); // ecx
  void (__thiscall ***v3)(_DWORD, int); // ecx
  _DWORD *v4; // esi
  int v5; // ebx

  *a1 = &CDspObjWindow::`vftable';
  v1 = (void (__thiscall ***)(_DWORD, int))a1[80];
  if ( v1 )
    (**v1)(v1, 1);
  v2 = (void (__thiscall ***)(_DWORD, int))a1[88];
  if ( v2 )
    (**v2)(v2, 1);
  v3 = (void (__thiscall ***)(_DWORD, int))a1[96];
  if ( v3 )
    (**v3)(v3, 1);
  v4 = a1 + 109;
  v5 = 8;
  do
  {
    if ( *v4 )
      (**(void (__thiscall ***)(_DWORD, int))*v4)(*v4, 1);
    v4 += 11;
    --v5;
  }
  while ( v5 );
  sub_42BBD0(a1);
  return sub_41A5E0(a1);
}

// ===== sub_42B0D0 @ 0x0042B0D0..0x0042B0D6 =====
int __usercall sub_42B0D0@<eax>(int result@<eax>)
{
  dword_565B44 = result;
  return result;
}

// ===== sub_42B0E0 @ 0x0042B0E0..0x0042B0E6 =====
int __usercall sub_42B0E0@<eax>(int result@<eax>)
{
  dword_565B48 = result;
  return result;
}

// ===== sub_42B0F0 @ 0x0042B0F0..0x0042B19E =====
void __thiscall sub_42B0F0(_DWORD *this, int a2, int *a3, int a4)
{
  int v5; // ecx
  int v6; // edx
  int v7; // ecx
  int v8; // edx
  unsigned int v9; // eax
  unsigned int v10; // [esp-8h] [ebp-28h]
  _DWORD v11[6]; // [esp+8h] [ebp-18h] BYREF

  if ( dword_565B44 )
  {
    v5 = this[82];
    v6 = this[83];
    v11[0] = this[81];
    v11[3] = this[84];
    v11[1] = v5;
    v7 = this[85];
    v11[2] = v6;
    v8 = this[86];
    v11[4] = v7;
    v11[5] = v8;
    sub_4091B0(v11, a3);
    v10 = 256 - (((256 - dword_565B48) * (256 - sub_41B770(this))) >> 8);
    v9 = sub_41B610((int)this);
    sub_40A9E0(a2, (int)v11, v9, v10, 1);
  }
}

// ===== sub_42B1A0 @ 0x0042B1A0..0x0042B292 =====
int __userpurge sub_42B1A0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<ecx>, unsigned int a3)
{
  int v3; // ebx
  int v4; // edi
  int v7; // [esp-Ch] [ebp-1Ch]

  v3 = a3;
  v4 = a1;
  a2[79] = 0;
  if ( a1 < 0x20 )
    v4 = 32 * a1;
  if ( a3 < 0x14 )
    v3 = 32 * a3;
  if ( (unsigned int)(v4 - 1) <= 0x77F && (unsigned int)(v3 - 1) <= 0x7FFF )
  {
    v7 = a2[80];
    a2[77] = v4;
    a2[78] = v3;
    sub_41BF90(0, a2 + 81, v7, v4, v3);
    sub_42B3B0(a2);
    sub_41BF90(0, a2 + 89, a2[88], v4, v3);
    sub_40A620((int)(a2 + 89), 0);
    sub_42B540(0);
    sub_41BF90(0, a2 + 97, a2[96], v4, v3);
    sub_409190(a2 + 104, (int)(a2 + 97));
    sub_42C5B0();
    sub_42B9B0();
    a2[79] = (*(int (__thiscall **)(_DWORD *, int, int))(*a2 + 116))(a2, v4, v3);
  }
  return a2[79];
}

// ===== sub_42B2A0 @ 0x0042B2A0..0x0042B352 =====
int __stdcall sub_42B2A0(int a1, int a2)
{
  int v3[6]; // [esp+10h] [ebp-18h] BYREF

  if ( !*(_DWORD *)(a1 + 316) )
    return 1;
  if ( a2 == -1 )
  {
    sub_40A620(a1 + 356, 0);
  }
  else
  {
    if ( !sub_407F20(dword_565B30, a2, v3) )
      return 2;
    sub_40A620(a1 + 356, 0);
    sub_40A530(v3, (_DWORD *)(a1 + 356), 0, 0, 128, 0);
  }
  sub_42B3B0(a1);
  sub_42CA00();
  return 0;
}

// ===== sub_42B360 @ 0x0042B360..0x0042B396 =====
int __userpurge sub_42B360@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>, int a4, int a5, int a6)
{
  (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a3 + 44))(a3, a1, a2);
  sub_41B600(a3, a4);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)a3 + 72))(a3, a5);
  return (*(int (__thiscall **)(int, int))(*(_DWORD *)a3 + 84))(a3, a6);
}

// ===== sub_42B3A0 @ 0x0042B3A0..0x0042B3B0 =====
int __usercall sub_42B3A0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(a2 + 956) = a1;
  return sub_42CA00();
}

// ===== sub_42B3B0 @ 0x0042B3B0..0x0042B3C0 =====
int __usercall sub_42B3B0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(a2 + 348) = a1;
  return sub_42CA00();
}

// ===== sub_42B3C0 @ 0x0042B3C0..0x0042B3FC =====
int __usercall sub_42B3C0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v3; // [esp+4h] [ebp-4h]

  if ( !*(_DWORD *)(a2 + 316) )
    return 1;
  sub_40A710((_DWORD *)(a2 + 356), 0, a1);
  sub_42CA00();
  return v3;
}

// ===== sub_42B400 @ 0x0042B400..0x0042B525 =====
int __userpurge sub_42B400@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3, int *a4, int a5, int a6, int a7)
{
  _DWORD *v8; // eax
  int result; // eax
  int v10[6]; // [esp+10h] [ebp-34h] BYREF
  int v11[4]; // [esp+28h] [ebp-1Ch] BYREF
  int v12[3]; // [esp+38h] [ebp-Ch] BYREF

  if ( !a3[79] )
    return 1;
  if ( !sub_407F20(dword_565B30, a2, v10) )
    return 2;
  switch ( sub_40A530(v10, a3 + 89, a1, a5, a6, a7) )
  {
    case 0:
      sub_409190(a4, (int)(a3 + 89));
      v8 = sub_409190(v11, (int)v10);
      sub_409170(a1, a5, v8);
      sub_409110(a4, v11);
      (*(void (__thiscall **)(_DWORD *, int *))(*a3 + 52))(a3, v12);
      sub_409170(v12[1], v12[0], a4);
      sub_42CD30(a3, a5, a1);
      result = 0;
      break;
    case 1:
      result = 4;
      break;
    case 2:
      result = 5;
      break;
    case 3:
      result = 6;
      break;
    case 4:
      result = 7;
      break;
    default:
      result = a5;
      break;
  }
  return result;
}

// ===== sub_42B540 @ 0x0042B540..0x0042B547 =====
int __usercall sub_42B540@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 380) = a2;
  return result;
}

// ===== sub_42B550 @ 0x0042B550..0x0042B557 =====
int __usercall sub_42B550@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 412) = a2;
  return result;
}

// ===== sub_42B560 @ 0x0042B560..0x0042B5AC =====
int __thiscall sub_42B560(void *this, int a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD v8[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(dword_565B30, (int)this, v8) )
    return sub_42B5B0(a2, a3, a4, a5, v8, a6, a7);
  else
    return 2;
}

// ===== sub_42B5B0 @ 0x0042B5B0..0x0042B6F9 =====
int __stdcall sub_42B5B0(_DWORD *a1, int *a2, int a3, int a4, int *a5, int a6, int a7)
{
  int v7; // ecx
  int v8; // edx
  int v9; // eax
  int v10; // ecx
  int v11; // edx
  int v12; // ecx
  int v13; // edx
  int v14; // eax
  _DWORD *v15; // eax
  int result; // eax
  _DWORD v17[6]; // [esp+10h] [ebp-34h] BYREF
  int v18; // [esp+28h] [ebp-1Ch] BYREF
  int v19; // [esp+2Ch] [ebp-18h]
  int v20; // [esp+30h] [ebp-14h]
  int v21; // [esp+34h] [ebp-10h]
  int v22[3]; // [esp+38h] [ebp-Ch] BYREF

  v7 = a1[98];
  v8 = a1[99];
  v17[0] = a1[97];
  v9 = a1[100];
  v17[1] = v7;
  v10 = a1[101];
  v17[2] = v8;
  v11 = a1[102];
  v17[3] = v9;
  v17[4] = v10;
  v17[5] = v11;
  sub_42C2D0();
  sub_4091B0(v17, &v18);
  switch ( sub_40A530(a5, v17, a4 - v19, a3 - v18, a6, a7) )
  {
    case 0:
      v12 = v19;
      v13 = v20;
      *a2 = v18;
      v14 = v21;
      a2[1] = v12;
      a2[2] = v13;
      a2[3] = v14;
      v15 = sub_409190(&v18, (int)a5);
      sub_409170(a4, a3, v15);
      sub_409110(a2, &v18);
      (*(void (__thiscall **)(_DWORD *, int *))(*a1 + 52))(a1, v22);
      sub_409170(v22[1], v22[0], a2);
      sub_42CD30(a1, a3, a4);
      result = 0;
      break;
    case 1:
      result = 4;
      break;
    case 2:
      result = 5;
      break;
    case 3:
      result = 6;
      break;
    case 4:
      result = 7;
      break;
    default:
      result = a4;
      break;
  }
  return result;
}

// ===== sub_42B710 @ 0x0042B710..0x0042B8F8 =====
int __userpurge sub_42B710@<eax>(const char *a1@<ecx>, int a2@<esi>, int a3, int a4, int a5, int a6, int a7)
{
  int v8; // eax
  char *v9; // ebx
  int v10; // eax
  int v11; // eax
  char *v12; // edi
  int v14; // [esp+Ch] [ebp-434h] BYREF
  int v15; // [esp+10h] [ebp-430h]
  int v16; // [esp+14h] [ebp-42Ch] BYREF
  int v17; // [esp+18h] [ebp-428h]
  int v18; // [esp+1Ch] [ebp-424h]
  int v19[2]; // [esp+20h] [ebp-420h] BYREF
  int v20[4]; // [esp+28h] [ebp-418h] BYREF
  _BYTE Src[1028]; // [esp+38h] [ebp-408h] BYREF

  v15 = a7;
  v8 = sub_42B550(a2, 0);
  sub_42B540(v8, 1);
  sub_42BBB0();
  sub_42C720(a2);
  sub_42C2A0(a2);
  memset(Src, 0, 1024);
  if ( a3 )
    sub_434920(a1);
  v14 = sub_42C450();
  v16 = sub_42C520();
  v17 = sub_42C5A0();
  v18 = sub_42C490();
  v9 = (char *)operator new[](strlen(a1) << 6);
  v10 = sub_42C570();
  if ( v10 )
  {
    if ( v10 != 1 )
      goto LABEL_8;
    v11 = sub_438270(
            a2 + 388,
            (int)&v14,
            (int)v9,
            (int)&v16,
            (int)v19,
            (int)v20,
            (int)a1,
            a3,
            Src,
            a4,
            v17,
            v18,
            a5,
            a6,
            v15);
  }
  else
  {
    v11 = sub_434C50(
            a2 + 388,
            (int)&v14,
            (int)v9,
            (int)&v16,
            (int)v19,
            (int)v20,
            (int)a1,
            a3,
            Src,
            v16,
            a4,
            v17,
            v18,
            a5,
            a6,
            v15);
  }
  v15 = v11;
LABEL_8:
  if ( v15 )
  {
    if ( v14 )
    {
      v12 = v9;
      do
      {
        sub_42CA30(a2, v12);
        v12 += 16;
        --v14;
      }
      while ( v14 );
    }
    sub_42C700(v19[1]);
  }
  operator delete[](v9);
  return v15;
}

// ===== sub_42B900 @ 0x0042B900..0x0042B96F =====
int __usercall sub_42B900@<eax>(_DWORD *a1@<eax>, unsigned int *a2@<ecx>)
{
  int result; // eax
  unsigned int v4; // edi
  int v5; // edx
  int v6; // edx
  unsigned int v7; // edi
  int v8; // edx
  unsigned int v9; // edx
  unsigned int v10; // eax
  unsigned int v11; // ecx

  result = 0;
  if ( (*a2 & 0x80000000) == 0 )
  {
    v4 = a1[99];
    if ( *a2 < v4 )
    {
      v5 = a2[2];
      if ( v5 >= 0 && v5 < v4 )
      {
        v6 = a2[1];
        if ( v6 >= 0 )
        {
          v7 = a1[100];
          if ( v6 < v7 )
          {
            v8 = a2[3];
            if ( v8 >= 0 && v8 < v7 )
            {
              v9 = a2[1];
              a1[104] = *a2;
              v10 = a2[2];
              v11 = a2[3];
              a1[105] = v9;
              a1[106] = v10;
              a1[107] = v11;
              sub_42C5B0();
              return 1;
            }
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_42B970 @ 0x0042B970..0x0042B9A1 =====
int __userpurge sub_42B970@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, _DWORD *a3, int a4, int a5)
{
  unsigned int v6[4]; // [esp+0h] [ebp-10h] BYREF

  v6[0] = a2;
  v6[1] = a1;
  v6[2] = a2 + a4 - 1;
  v6[3] = a1 + a5 - 1;
  return sub_42B900(a3, v6);
}

// ===== sub_42B9B0 @ 0x0042B9B0..0x0042B9C7 =====
int __usercall sub_42B9B0@<eax>(int a1@<eax>)
{
  sub_40A620(a1 + 388, 0);
  return sub_42CA00();
}

// ===== sub_42B9D0 @ 0x0042B9D0..0x0042BAAD =====
BOOL __stdcall sub_42B9D0(_DWORD *a1, int a2)
{
  BOOL result; // eax
  int v3; // edi
  int v4; // ecx
  int v5; // eax
  int v6; // edx
  int v7; // ecx
  int v8; // eax
  int v9[6]; // [esp+Ch] [ebp-3Ch] BYREF
  int v10; // [esp+24h] [ebp-24h]
  int v11; // [esp+28h] [ebp-20h]
  int v12; // [esp+2Ch] [ebp-1Ch]
  int v13; // [esp+30h] [ebp-18h]
  int v14; // [esp+34h] [ebp-14h] BYREF
  int v15; // [esp+38h] [ebp-10h]
  int v16; // [esp+3Ch] [ebp-Ch]
  int v17; // [esp+40h] [ebp-8h]
  BOOL v18; // [esp+44h] [ebp-4h]

  result = a2 > 0;
  v18 = result;
  if ( a2 > 0 )
  {
    sub_42C2D0();
    v3 = v11;
    v14 = v10;
    v17 = v13;
    v15 = v11;
    v16 = v12;
    if ( a2 < v13 - v11 + 1 )
    {
      v4 = a1[99];
      v5 = a1[98];
      v9[0] = a1[97];
      v6 = a1[100];
      v9[2] = v4;
      v7 = a1[102];
      v9[1] = v5;
      v8 = a1[101];
      v9[3] = v6;
      v9[5] = v7;
      v9[4] = v8;
      v15 = v11 + a2;
      sub_4091B0(v9, &v14);
      sub_40A530(v9, a1 + 97, v3, v10, 128, 0);
      v15 = v17 - a2 + 1;
    }
    sub_40A620((int)(a1 + 97), &v14);
    sub_42CA00();
    return v18;
  }
  return result;
}

// ===== sub_42BAB0 @ 0x0042BAB0..0x0042BADE =====
int __userpurge sub_42BAB0@<eax>(int a1@<edi>, int a2@<esi>, int a3)
{
  sub_42CD70(0);
  *(_DWORD *)(44 * a2 + a1 + 432) = a3;
  return sub_42CD70(1);
}

// ===== sub_42BAE0 @ 0x0042BAE0..0x0042BB22 =====
int __userpurge sub_42BAE0@<eax>(int a1@<edi>, int a2@<esi>, int a3, int a4, int a5)
{
  int v5; // eax

  sub_42CD70(0);
  v5 = 44 * a2;
  *(_DWORD *)(v5 + a1 + 464) = a3;
  *(_DWORD *)(v5 + a1 + 472) = a5;
  *(_DWORD *)(v5 + a1 + 468) = a4;
  return sub_42CD70(1);
}

// ===== sub_42BB30 @ 0x0042BB30..0x0042BBA6 =====
int __userpurge sub_42BB30@<eax>(int a1@<edi>, int a2@<esi>, int a3)
{
  _DWORD *v5; // [esp+Ch] [ebp+8h]

  if ( !sub_40A9D0() )
    return 4;
  sub_42CD70(0);
  v5 = (_DWORD *)(a3 + 44 * (a1 + 10));
  sub_41BF90(a2, v5, *(_DWORD *)(44 * a1 + a3 + 436), *(_DWORD *)(a2 + 8), *(_DWORD *)(a2 + 12));
  sub_40A9E0((int)v5, a2, 0x80u, 0, 1);
  sub_42CD70(1);
  return 0;
}

// ===== sub_42BBB0 @ 0x0042BBB0..0x0042BBC8 =====
int __usercall sub_42BBB0@<eax>(int a1@<eax>)
{
  int i; // esi
  int result; // eax

  for ( i = 0; i < 8; ++i )
    result = sub_42BAB0(a1, i, 0);
  return result;
}

// ===== sub_42BBD0 @ 0x0042BBD0..0x0042BDDF =====
unsigned int __fastcall sub_42BBD0(int a1, int a2, void *a3)
{
  unsigned int i; // esi
  void (__thiscall ***v5)(_DWORD, int); // ecx
  void (__thiscall ***v6)(_DWORD, int); // ecx
  unsigned int result; // eax
  void *v8; // eax
  int v9; // eax
  int v10; // ecx
  int v11; // edx
  int v12; // eax
  int v13; // ecx
  void *v14; // eax
  int v15; // eax
  unsigned int v16; // [esp+1Ch] [ebp-20h]
  unsigned int v17; // [esp+20h] [ebp-1Ch]

  if ( *((_DWORD *)a3 + 197) )
  {
    for ( i = 0; i < *((_DWORD *)a3 + 196); ++i )
    {
      sub_430770(*((_DWORD *)a3 + 208));
      v5 = *(void (__thiscall ****)(_DWORD, int))(*((_DWORD *)a3 + 197) + 4 * i);
      if ( v5 )
        (**v5)(v5, 1);
    }
    operator delete[](*((void **)a3 + 197));
  }
  v6 = (void (__thiscall ***)(_DWORD, int))*((_DWORD *)a3 + 208);
  if ( v6 )
    (**v6)(v6, 1);
  result = 0;
  if ( a1 )
  {
    *((_DWORD *)a3 + 196) = a1;
    v8 = operator new[](4 * a1);
    *((_DWORD *)a3 + 197) = v8;
    memset(v8, 0, 4 * a1);
    *((_DWORD *)a3 + 198) = 0;
    *((_DWORD *)a3 + 199) = 0;
    *((_DWORD *)a3 + 200) = 0;
    *((_DWORD *)a3 + 201) = 0;
    *((_DWORD *)a3 + 202) = 0;
    *((_DWORD *)a3 + 203) = 0;
    *((_DWORD *)a3 + 204) = 0;
    *((_DWORD *)a3 + 205) = 0;
    *((_DWORD *)a3 + 206) = 0;
    *((_DWORD *)a3 + 207) = 0;
    v9 = *((_DWORD *)a3 + 82);
    v10 = *((_DWORD *)a3 + 83);
    *((_DWORD *)a3 + 198) = *((_DWORD *)a3 + 81);
    v11 = *((_DWORD *)a3 + 84);
    *((_DWORD *)a3 + 199) = v9;
    v12 = *((_DWORD *)a3 + 85);
    *((_DWORD *)a3 + 200) = v10;
    v13 = *((_DWORD *)a3 + 86);
    *((_DWORD *)a3 + 201) = v11;
    *((_DWORD *)a3 + 202) = v12;
    *((_DWORD *)a3 + 203) = v13;
    sub_409190((_DWORD *)a3 + 204, (int)a3 + 792);
    v14 = operator new(0x70u);
    if ( v14 )
      v15 = sub_44C760(v14, a1);
    else
      v15 = 0;
    *((_DWORD *)a3 + 208) = v15;
    sub_430CD0();
    sub_442E10(dword_565B2C);
    result = v16 >> 1;
    *((_DWORD *)a3 + 209) = -(v16 >> 1);
    *((_DWORD *)a3 + 210) = -(v17 >> 1);
    *((_DWORD *)a3 + 211) = v16 >> 1;
  }
  else
  {
    *((_DWORD *)a3 + 196) = 0;
    *((_DWORD *)a3 + 197) = 0;
    *((_DWORD *)a3 + 208) = 0;
  }
  return result;
}

// ===== sub_42BDE0 @ 0x0042BDE0..0x0042BEC5 =====
void __usercall sub_42BDE0(_DWORD *a1@<esi>)
{
  unsigned int v1; // eax
  int v2; // ecx
  unsigned int v3; // edi
  _DWORD *v4; // eax
  int v5; // ecx
  int v6; // [esp+Ch] [ebp-24h]
  int v7; // [esp+10h] [ebp-20h]
  unsigned int v8; // [esp+1Ch] [ebp-14h] BYREF
  unsigned int v9; // [esp+20h] [ebp-10h]
  int v10; // [esp+24h] [ebp-Ch]

  if ( a1[196] )
  {
    v7 = a1[210];
    v6 = a1[209];
    sub_442E10(dword_565B2C);
    v1 = v8 >> 1;
    v2 = -(v9 >> 1);
    v3 = 0;
    a1[209] = -(v8 >> 1);
    a1[210] = v2;
    a1[211] = v1;
    do
    {
      v4 = (_DWORD *)(a1[197] + 4 * v3);
      if ( *v4 )
      {
        (*(void (__thiscall **)(_DWORD, unsigned int *))(*(_DWORD *)*v4 + 64))(*v4, &v8);
        v5 = *(_DWORD *)(a1[197] + 4 * v3);
        (*(void (__thiscall **)(int, unsigned int, unsigned int, int))(*(_DWORD *)v5 + 60))(
          v5,
          v8 + ((a1[209] - v6) << 16),
          v9 + ((a1[210] - v7) << 16),
          v10);
        sub_428370(*(_DWORD *)(a1[197] + 4 * v3), a1[211]);
      }
      ++v3;
    }
    while ( v3 < a1[196] );
  }
}

// ===== sub_42BED0 @ 0x0042BED0..0x0042C055 =====
int __userpurge sub_42BED0@<eax>(_DWORD *a1@<edi>, unsigned int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  void (__thiscall ***v8)(_DWORD, int); // ecx
  _DWORD *v9; // eax
  _DWORD *v10; // eax
  int v11; // ecx
  int v12; // eax
  int v13; // ecx

  if ( a2 >= a1[196] )
    return 9;
  if ( *(_DWORD *)(a1[197] + 4 * a2) )
  {
    sub_430770(a1[208]);
    v8 = *(void (__thiscall ****)(_DWORD, int))(a1[197] + 4 * a2);
    if ( v8 )
      (**v8)(v8, 1);
  }
  v9 = operator new(0x418u);
  if ( v9 )
    v10 = sub_4256C0((void *)a2, v9, 0);
  else
    v10 = 0;
  *(_DWORD *)(a1[197] + 4 * a2) = v10;
  sub_41ADF0(*(_DWORD *)(a1[197] + 4 * a2), 0);
  v12 = sub_427170(
          *(_DWORD **)(a1[197] + 4 * a2),
          (a4 + a1[209]) << 16,
          0,
          a3,
          -1,
          0,
          0,
          a6,
          a7,
          0,
          a1[211],
          v11,
          1,
          32,
          v11,
          a8);
  v13 = *(_DWORD *)(a1[197] + 4 * a2);
  if ( v12 )
  {
    if ( v13 )
      (**(void (__thiscall ***)(int, int))v13)(v13, 1);
    *(_DWORD *)(a1[197] + 4 * a2) = 0;
    return 2;
  }
  else
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v13 + 4))(v13, 1);
    sub_4306F0(*(_DWORD *)(a1[197] + 4 * a2));
    return 0;
  }
}
