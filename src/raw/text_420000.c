#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_420110 @ 0x00420110..0x00420243 =====
int __userpurge sub_420110@<eax>(_DWORD *a1@<edi>, int a2, int a3, int a4, int a5, int a6)
{
  int v7; // esi
  int v8; // edx
  int v9; // eax
  unsigned int v10; // [esp+10h] [ebp-28h]
  unsigned int v11; // [esp+14h] [ebp-24h]
  _DWORD v12[6]; // [esp+20h] [ebp-18h] BYREF

  sub_41C0A0();
  if ( !sub_407F20(dword_565B30, a2, v12) )
    return -2147483647;
  if ( v12[4] != 4 || v12[2] < v10 || v12[3] < v11 )
    return -2147483645;
  if ( (a3 != -1 ? (unsigned int)v12 : 0) != 0 )
  {
    if ( !sub_407F20(dword_565B30, a3, a3 != -1 ? v12 : 0) )
      return -2147483646;
    if ( *(a3 != -1 ? &v12[4] : (_DWORD *)16) != 4
      || *(a3 != -1 ? &v12[2] : (_DWORD *)8) < v10
      || *(a3 != -1 ? &v12[3] : (_DWORD *)12) < v11 )
    {
      return -2147483644;
    }
  }
  sub_4208C0();
  v7 = dword_565B30;
  v8 = dword_565B30;
  a1[77] = 0;
  a1[78] = a2;
  a1[80] = sub_408300(a2, v8);
  a1[79] = a3;
  a1[81] = sub_408300(a3, v7);
  v9 = *a1;
  a1[82] = a5;
  (*(void (__thiscall **)(_DWORD *, int))(v9 + 72))(a1, a4);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a6);
  return 0;
}

// ===== sub_420250 @ 0x00420250..0x0042029B =====
int __userpurge sub_420250@<eax>(_DWORD *a1@<eax>, unsigned int a2@<edi>, int a3, int a4)
{
  void (__thiscall *v5)(_DWORD *, int); // edx

  if ( a2 > 1 )
    return -2147483643;
  sub_4208C0();
  v5 = *(void (__thiscall **)(_DWORD *, int))(*a1 + 72);
  a1[77] = 1;
  a1[89] = a2;
  v5(a1, a3);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a4);
  return 0;
}

// ===== sub_4202A0 @ 0x004202A0..0x0042040A =====
int __userpurge sub_4202A0@<eax>(_DWORD *a1@<edi>, int a2, int a3, unsigned int a4, int a5, int a6)
{
  void *v6; // esi
  int v7; // edx
  void *v8; // eax
  void (__thiscall *v9)(_DWORD *, int); // edx
  int v11; // [esp+8h] [ebp-38h] BYREF
  void *v12; // [esp+Ch] [ebp-34h]
  _DWORD v13[12]; // [esp+10h] [ebp-30h] BYREF

  if ( !sub_407F20(dword_565B30, a2, v13) )
    return -2147483647;
  sub_41C0A0();
  if ( v13[4] != 6 || v13[2] != v13[8] || v13[3] != v13[9] )
    return -2147483645;
  if ( !a3 )
    return -2147483642;
  v6 = operator new[](16 * a3);
  v12 = v6;
  if ( sub_409FF0(a4, dword_565B30, &v11, 0, a3) )
  {
    operator delete[](v6);
    return -2147483641;
  }
  else if ( v11 )
  {
    sub_4208C0();
    v7 = dword_565B30;
    a1[77] = 2;
    a1[90] = a2;
    a1[91] = sub_408300(a2, v7);
    v8 = v12;
    a1[92] = a3;
    v9 = *(void (__thiscall **)(_DWORD *, int))(*a1 + 72);
    a1[93] = v8;
    a1[94] = a4;
    a1[95] = 0;
    v9(a1, a5);
    (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a6);
    return 0;
  }
  else
  {
    return -2147483640;
  }
}

// ===== sub_420410 @ 0x00420410..0x004204CA =====
int __userpurge sub_420410@<eax>(_DWORD *a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  if ( !a5 || !a6 )
    return -2147483639;
  sub_4208C0();
  a1[97] = a3;
  a1[98] = a4;
  a1[96] = a2;
  a1[101] = a7;
  a1[77] = 3;
  a1[99] = a5;
  a1[100] = a6;
  a1[102] = 0;
  a1[103] = 0;
  a1[104] = 0;
  a1[105] = 0;
  a1[106] = 0;
  a1[112] = 0;
  sub_41B620(a1, a8);
  sub_41B6E0((int)a1, 0);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a9);
  return 0;
}

// ===== sub_4204D0 @ 0x004204D0..0x00420513 =====
int __userpurge sub_4204D0@<eax>(_DWORD *a1@<eax>, int a2, int a3)
{
  void (__thiscall *v4)(_DWORD *, int); // edx

  sub_4208C0();
  v4 = *(void (__thiscall **)(_DWORD *, int))(*a1 + 72);
  a1[77] = 4;
  v4(a1, a2);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a3);
  sub_40A620((int)(a1 + 83), 0);
  return 0;
}

// ===== sub_420520 @ 0x00420520..0x00420571 =====
int __thiscall sub_420520(_DWORD *this, int a2)
{
  int v3; // eax
  int result; // eax

  sub_41B620(this, a2);
  v3 = sub_4207A0() - 2;
  if ( !v3 )
    return sub_409F40(this[92], this[93], this[94], this[95], a2);
  result = v3 - 1;
  if ( !result )
    return sub_4208E0();
  return result;
}

// ===== sub_420580 @ 0x00420580..0x004205AB =====
int __thiscall sub_420580(_DWORD *this, int a2, int a3)
{
  int result; // eax

  sub_41B6F0(this, a2, a3);
  result = sub_4207A0();
  if ( result == 3 )
    return sub_4208E0();
  return result;
}

// ===== sub_4205B0 @ 0x004205B0..0x0042068F =====
int __thiscall sub_4205B0(_DWORD *this, unsigned int a2, int a3, int a4)
{
  int v4; // ecx
  int result; // eax
  int v6; // ecx
  int v7; // ecx

  switch ( a2 )
  {
    case 0x80u:
      if ( sub_4207A0(this, a2) == 3 )
      {
        *(_DWORD *)(v4 + 408) = a3;
        *(_DWORD *)(v4 + 412) = a4;
      }
      goto LABEL_4;
    case 0x81u:
      if ( sub_4207A0(this, a2) == 3 )
        *(_DWORD *)(v6 + 416) = a3;
      result = 0;
      break;
    case 0x82u:
      if ( sub_4207A0(this, a2) == 3 )
      {
        *(_DWORD *)(v7 + 420) = a3;
        *(_DWORD *)(v7 + 424) = a4;
      }
      result = 0;
      break;
    case 0x8Fu:
      this[112] = a3;
      result = 0;
      break;
    case 0xFFu:
      sub_4207B0(this, a2);
      result = 0;
      break;
    case 0x100u:
      if ( sub_4207A0(this, a2) == 2 )
        result = sub_4207D0(a4) != 0 ? 0xFFFF0002 : 0;
      else
LABEL_4:
        result = 0;
      break;
    default:
      result = sub_41B8E0(this, a2, a3, a4);
      break;
  }
  return result;
}

// ===== sub_420730 @ 0x00420730..0x0042079E =====
int __usercall sub_420730@<eax>(_DWORD *a1@<eax>)
{
  _DWORD v3[6]; // [esp+10h] [ebp-30h] BYREF
  int v4; // [esp+30h] [ebp-10h]
  int v5; // [esp+34h] [ebp-Ch]
  int v6; // [esp+38h] [ebp-8h]

  sub_41C0A0();
  sub_41C060(v3, a1);
  if ( v4 == v3[2] && v5 == v3[3] && v6 == v3[4] )
    return 0;
  (*(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116))(a1, v4, v5);
  sub_420870();
  sub_409080(a1 + 83, 0);
  return 1;
}

// ===== sub_4207A0 @ 0x004207A0..0x004207A7 =====
int __usercall sub_4207A0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 308);
}

// ===== sub_4207B0 @ 0x004207B0..0x004207C3 =====
int __usercall sub_4207B0@<eax>(int result@<eax>, _DWORD *a2@<ecx>)
{
  a2[113] = result;
  a2[82] = result;
  a2[101] = result;
  return result;
}

// ===== sub_4207D0 @ 0x004207D0..0x00420861 =====
int __userpurge sub_4207D0@<eax>(int a1@<edi>, _DWORD *a2@<esi>, unsigned int a3)
{
  unsigned int v3; // ebx
  int v4; // ecx
  int v6; // eax

  v3 = a3;
  if ( sub_4207A0((int)a2) != 2 )
    return v4;
  if ( sub_409FF0(a2[94], dword_565B30, &a3, a1, a2[92]) )
    return -2147483641;
  if ( !a3 )
    return -2147483640;
  a2[95] = a1;
  if ( v3 > 0x100 )
  {
    v6 = (*(int (__thiscall **)(_DWORD *))(*a2 + 76))(a2);
    (*(void (__thiscall **)(_DWORD *, int))(*a2 + 72))(a2, v6);
  }
  else
  {
    (*(void (__thiscall **)(_DWORD *, unsigned int))(*a2 + 72))(a2, v3);
  }
  return 0;
}

// ===== sub_420870 @ 0x00420870..0x004208B7 =====
BOOL __usercall sub_420870@<eax>(int a1@<esi>)
{
  int v1; // eax
  BOOL v2; // edi

  v1 = *(_DWORD *)(a1 + 332);
  v2 = v1 != 0;
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 332));
    *(_DWORD *)(a1 + 332) = 0;
    *(_DWORD *)(a1 + 336) = 0;
    *(_DWORD *)(a1 + 340) = 0;
    *(_DWORD *)(a1 + 344) = 0;
    *(_DWORD *)(a1 + 348) = 0;
    *(_DWORD *)(a1 + 352) = 0;
  }
  return v2;
}

// ===== sub_4208C0 @ 0x004208C0..0x004208DA =====
void __usercall sub_4208C0(int a1@<esi>)
{
  operator delete[](*(void **)(a1 + 372));
  *(_DWORD *)(a1 + 372) = 0;
}

// ===== sub_4208E0 @ 0x004208E0..0x00420A2D =====
int __usercall sub_4208E0@<eax>(_DWORD *a1@<esi>)
{
  int v1; // edi
  int result; // eax
  int v3; // [esp+8h] [ebp-8h]

  v3 = (*(int (__thiscall **)(_DWORD *))(*a1 + 76))(a1);
  v1 = sub_41B750(1, (int)a1);
  a1[107] = a1[96] + (((int)a1[102] * (unsigned __int64)(unsigned int)v1) >> 24);
  a1[108] = a1[97] + (((int)a1[103] * (unsigned __int64)(unsigned int)v1) >> 24);
  a1[109] = (((int)a1[98] + (((int)a1[104] * (__int64)sub_41A690(v1)) >> 16)) * (unsigned __int64)(unsigned int)v3) >> 8;
  a1[110] = ((((unsigned int)a1[99] + ((__int64)((int)a1[105] * (unsigned __int64)(unsigned int)v1) >> 24) - 0x10000)
            * (unsigned __int64)(unsigned int)v3) >> 8)
          + 0x10000;
  result = ((((unsigned int)a1[100] + ((__int64)((int)a1[106] * (unsigned __int64)(unsigned int)v1) >> 24) - 0x10000)
           * (unsigned int)v3) >> 8)
         + 0x10000;
  a1[111] = result;
  return result;
}

// ===== sub_420A30 @ 0x00420A30..0x00420AA5 =====
_DWORD *__thiscall sub_420A30(void *this, _DWORD *a2)
{
  sub_41A400((int)this, a2, 7, 1);
  *a2 = &CDspObjFilter::`vftable';
  sub_41B600((int)a2, 192);
  sub_420C60(0, 0, 0);
  sub_420D70();
  return a2;
}

// ===== sub_420AB0 @ 0x00420AB0..0x00420AD2 =====
void *__thiscall sub_420AB0(void *this, char a2)
{
  sub_420AE0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_420AE0 @ 0x00420AE0..0x00420B28 =====
int __stdcall sub_420AE0(_DWORD *a1)
{
  *a1 = &CDspObjFilter::`vftable';
  return sub_41A5E0(a1);
}

// ===== sub_420B30 @ 0x00420B30..0x00420C42 =====
int *__thiscall sub_420B30(_DWORD *this, int *a2, int *a3, int a4)
{
  unsigned int v5; // edi
  int v6; // eax
  int *result; // eax
  _DWORD v8[6]; // [esp+10h] [ebp-18h] BYREF

  v5 = sub_41B770(this);
  v6 = this[77];
  if ( v6 )
  {
    result = (int *)(v6 - 1);
    if ( !result )
    {
      result = (int *)sub_407F20(dword_565B30, this[80], v8);
      if ( result )
      {
        result = (int *)sub_408300(this[80], dword_565B30);
        if ( (int *)this[82] == result )
        {
          sub_4091B0(v8, a3);
          return (int *)sub_40E4F0((int)a2, this[81], this[79], (int)v8, v5);
        }
      }
    }
  }
  else
  {
    result = (int *)this[78];
    switch ( (unsigned int)result )
    {
      case 0u:
        result = (int *)sub_40E190(v5, this[79], (int)a2, a2);
        break;
      case 1u:
        result = sub_410C80(a2, this[79], v5, a2);
        break;
      case 2u:
        result = (int *)sub_411060(v5, (int)a2, this[79], (int)a2);
        break;
      case 3u:
        result = (int *)sub_410CB0(v5, (int)a2, this[79], (int)a2);
        break;
      default:
        return result;
    }
  }
  return result;
}

// ===== sub_420C60 @ 0x00420C60..0x00420C9A =====
int __userpurge sub_420C60@<eax>(_DWORD *a1@<esi>, int a2, int a3, int a4)
{
  int result; // eax

  sub_420DC0();
  sub_420DD0(a2);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 72))(a1, a3);
  result = (*(int (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a4);
  a1[77] = 0;
  return result;
}

// ===== sub_420CA0 @ 0x00420CA0..0x00420D63 =====
int __userpurge sub_420CA0@<eax>(_DWORD *a1@<edi>, int a2, int a3)
{
  int v3; // edx
  int result; // eax
  _DWORD v5[12]; // [esp+8h] [ebp-30h] BYREF

  if ( a2 == -1 )
  {
    result = 0;
    a1[77] = 0;
  }
  else if ( sub_407F20(dword_565B30, a2, v5) )
  {
    if ( v5[4] == 3 )
    {
      sub_442E10(dword_565B2C);
      if ( v5[8] == v5[2] && v5[9] == v5[3] )
      {
        a1[81] = a3;
        v3 = dword_565B30;
        a1[80] = a2;
        a1[82] = sub_408300(a2, v3);
        a1[77] = 1;
        return 0;
      }
      else
      {
        return -2147483645;
      }
    }
    else
    {
      return -2147483646;
    }
  }
  else
  {
    return -2147483647;
  }
  return result;
}

// ===== sub_420D70 @ 0x00420D70..0x00420DBE =====
int __usercall sub_420D70@<eax>(_DWORD *a1@<esi>)
{
  int v1; // ecx
  int result; // eax
  int v3; // [esp-4h] [ebp-3Ch]
  _DWORD v4[6]; // [esp+8h] [ebp-30h] BYREF
  int v5; // [esp+28h] [ebp-10h]
  int v6; // [esp+2Ch] [ebp-Ch]
  int v7; // [esp+30h] [ebp-8h]

  sub_41C0A0();
  sub_41C060(v4, a1);
  v1 = v5;
  result = 0;
  if ( v5 != v4[2] || v6 != v4[3] || v7 != v4[4] )
  {
    v3 = v6;
    a1[77] = 0;
    (*(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116))(a1, v1, v3);
    return 1;
  }
  return result;
}

// ===== sub_420DC0 @ 0x00420DC0..0x00420DC7 =====
int __usercall sub_420DC0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 312) = a2;
  return result;
}

// ===== sub_420DD0 @ 0x00420DD0..0x00420DD7 =====
int __usercall sub_420DD0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 316) = a2;
  return result;
}

// ===== sub_420DE0 @ 0x00420DE0..0x00420E37 =====
_DWORD *__thiscall sub_420DE0(void *this, _DWORD *a2)
{
  sub_41A400((int)this, a2, 9, 1);
  *a2 = &CDspObjGroup::`vftable';
  return a2;
}

// ===== sub_420E40 @ 0x00420E40..0x00420E62 =====
void *__thiscall sub_420E40(void *this, char a2)
{
  sub_420E70(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_420E70 @ 0x00420E70..0x00420EB8 =====
int __stdcall sub_420E70(_DWORD *a1)
{
  *a1 = &CDspObjGroup::`vftable';
  return sub_41A5E0(a1);
}

// ===== sub_420EC0 @ 0x00420EC0..0x00420FDF =====
_DWORD *__fastcall sub_420EC0(int a1, int a2, _DWORD *a3)
{
  int v4; // eax
  int v5; // eax
  unsigned int v6; // eax
  int v8[2]; // [esp+14h] [ebp-28h] BYREF
  _DWORD v9[8]; // [esp+1Ch] [ebp-20h] BYREF

  sub_41A400(a1, a3, 10, 1);
  v9[7] = 0;
  *a3 = &CDspObjKnob::`vftable';
  sub_41AC80();
  a3[77] = a2;
  v4 = sub_41B610(a2);
  sub_41B600((int)a3, v4);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 76))(a2);
  sub_41B620(a3, v5);
  v6 = (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 88))(a2);
  sub_41B8B0(a3, v6);
  sub_4211E0(0, a3);
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)a2 + 32))(a2, v9);
  sub_421200(v9[3] - v9[1] + 1);
  sub_421290(1);
  (*(void (__thiscall **)(int, int *))(*(_DWORD *)a2 + 48))(a2, v8);
  sub_41B1B0(a3, v8[0], v8[1]);
  sub_421430(0, 0);
  sub_421570(a3, 0);
  return a3;
}

// ===== sub_420FE0 @ 0x00420FE0..0x00421002 =====
void *__thiscall sub_420FE0(void *this, char a2)
{
  sub_421010(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_421010 @ 0x00421010..0x00421071 =====
int __stdcall sub_421010(_DWORD *a1)
{
  *a1 = &CDspObjKnob::`vftable';
  sub_41ACA0((int)a1);
  return sub_41A5E0(a1);
}

// ===== sub_421080 @ 0x00421080..0x0042108D =====
int __thiscall sub_421080(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[77] + 12))(this[77]);
}

// ===== sub_421090 @ 0x00421090..0x0042109D =====
int __thiscall sub_421090(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[77] + 28))(this[77]);
}

// ===== sub_4210A0 @ 0x004210A0..0x004210B1 =====
int __thiscall sub_4210A0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[77] + 32))(this[77]);
}

// ===== sub_4210C0 @ 0x004210C0..0x004210D1 =====
int __thiscall sub_4210C0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[77] + 36))(this[77]);
}

// ===== sub_4210E0 @ 0x004210E0..0x00421104 =====
int __thiscall sub_4210E0(int this, int a2)
{
  sub_41AE00((_DWORD *)this, a2);
  return (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(this + 308) + 4))(*(_DWORD *)(this + 308), a2);
}

// ===== sub_421110 @ 0x00421110..0x00421119 =====
int __thiscall sub_421110(void *this, int a2, int a3)
{
  return sub_41B1B0(this, a2, a3);
}

// ===== sub_421120 @ 0x00421120..0x004211A9 =====
int __thiscall sub_421120(int this, int a2, int a3, int a4, int a5)
{
  int v6; // edi
  int v7; // eax
  int v9; // [esp+Ch] [ebp-8h]
  int v10; // [esp+10h] [ebp-4h]

  sub_41B1D0((_DWORD *)this, a2, a3, a4, a5);
  sub_421500(this);
  if ( *(int *)(this + 332) <= 0 )
    v6 = v9;
  else
    v6 = (v9 * (sub_4215E0(this) + 1)) >> 16;
  if ( *(int *)(this + 336) <= 0 )
    v7 = v10;
  else
    v7 = (v10 * (sub_421640(this) + 1)) >> 16;
  return (*(int (__thiscall **)(_DWORD, int, int, int, int))(**(_DWORD **)(this + 308) + 40))(
           *(_DWORD *)(this + 308),
           a2 + v6,
           a3 + v7,
           a4,
           1);
}

// ===== sub_4211B0 @ 0x004211B0..0x004211D4 =====
int __thiscall sub_4211B0(int this, int a2)
{
  sub_41B620((_DWORD *)this, a2);
  return (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(this + 308) + 72))(*(_DWORD *)(this + 308), a2);
}

// ===== sub_4211E0 @ 0x004211E0..0x004211FC =====
int __usercall sub_4211E0@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int result; // eax

  result = 0;
  if ( a2 >= 0 && a3 >= 0 )
  {
    *(_DWORD *)(a1 + 332) = a2;
    *(_DWORD *)(a1 + 336) = a3;
    return 1;
  }
  return result;
}

// ===== sub_421200 @ 0x00421200..0x00421281 =====
int __userpurge sub_421200@<eax>(int a1@<ecx>, _DWORD *a2@<esi>, int a3)
{
  int result; // eax
  _DWORD v5[4]; // [esp+8h] [ebp-14h] BYREF

  (*(void (__thiscall **)(_DWORD *, _DWORD *))(*a2 + 32))(a2, v5);
  if ( a1 < v5[2] - v5[0] + 1 )
    return 0;
  result = 0;
  if ( a3 >= v5[3] - v5[1] + 1 )
  {
    a2[87] = a1 - 1;
    a2[85] = 0;
    a2[86] = 0;
    a2[88] = a3 - 1;
    return 1;
  }
  return result;
}

// ===== sub_421290 @ 0x00421290..0x00421297 =====
int __usercall sub_421290@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 320) = a2;
  return result;
}

// ===== sub_4212A0 @ 0x004212A0..0x004212F6 =====
int __userpurge sub_4212A0@<eax>(int a1@<eax>, int a2@<esi>, int a3)
{
  int result; // eax
  int v5; // edi
  _DWORD v6[2]; // [esp+8h] [ebp-8h] BYREF

  result = 0;
  if ( *(_DWORD *)(a2 + 320) )
  {
    result = (*(int (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(a2 + 308) + 48))(*(_DWORD *)(a2 + 308), v6);
    v5 = a1 - v6[1];
    *(_DWORD *)(a2 + 324) = a3 - v6[0];
    *(_DWORD *)(a2 + 328) = v5;
  }
  else
  {
    *(_DWORD *)(a2 + 324) = 0;
    *(_DWORD *)(a2 + 328) = 0;
  }
  return result;
}

// ===== sub_421300 @ 0x00421300..0x0042142A =====
int __userpurge sub_421300@<eax>(int a1@<ecx>, _DWORD *a2@<esi>, int a3)
{
  int v4; // ebx
  int v5; // eax
  int v6; // ecx
  int v7; // edi
  int v8; // edi
  int v9; // eax
  int v10; // ecx
  int v11; // eax
  _DWORD v13[3]; // [esp+Ch] [ebp-28h] BYREF
  int v14; // [esp+18h] [ebp-1Ch]
  _BYTE v15[8]; // [esp+1Ch] [ebp-18h] BYREF
  int v16; // [esp+24h] [ebp-10h]
  int v17; // [esp+28h] [ebp-Ch]

  (*(void (__thiscall **)(_DWORD *, _DWORD *))(*a2 + 48))(a2, v13);
  (*(void (__thiscall **)(_DWORD *, _BYTE *))(*a2 + 32))(a2, v15);
  v4 = a1 - a2[81] - v13[0];
  if ( v4 < a2[85] )
    v4 = a2[85];
  v5 = a2[87] - v16;
  if ( v4 > v5 )
    v4 = a2[87] - v16;
  v6 = a2[83];
  if ( v6 > 0 )
  {
    if ( v6 <= 1 )
      v7 = 0;
    else
      v7 = v5 / (2 * v6 - 2);
    v4 = ((v7 + v4) << 16) / sub_4215E0(a2);
  }
  v8 = a3 - a2[82] - v13[1];
  if ( v8 < a2[86] )
    v8 = a2[86];
  v9 = a2[88] - v17;
  if ( v8 > v9 )
    v8 = a2[88] - v17;
  v10 = a2[84];
  if ( v10 > 0 )
  {
    if ( v10 <= 1 )
      v14 = 0;
    else
      v14 = v9 / (2 * v10 - 2);
    v11 = sub_421640(a2);
    v8 = ((v8 + v14) << 16) / v11;
  }
  sub_421500(a2);
  if ( v13[2] == v4 && v14 == v8 )
    return 0;
  sub_421430(v4, v8);
  return 1;
}

// ===== sub_421430 @ 0x00421430..0x004214FB =====
int __userpurge sub_421430@<eax>(int a1@<eax>, int a2, int a3)
{
  int v3; // ebx
  bool v5; // cc
  int v6; // edi
  int result; // eax
  int v8; // [esp+Ch] [ebp-Ch]
  int v9; // [esp+10h] [ebp-8h] BYREF
  int v10; // [esp+14h] [ebp-4h]

  v3 = a2;
  v8 = 1;
  sub_4216A0();
  if ( a2 < 0 || a2 > v9 )
  {
    v8 = 0;
  }
  else
  {
    v5 = *(_DWORD *)(a1 + 332) <= 0;
    *(_DWORD *)(a1 + 312) = a2;
    if ( !v5 )
      v3 = ((sub_4215E0(a1) + 1) * a2) >> 16;
  }
  v6 = a3;
  if ( a3 < 0 || a3 > v10 )
    return 0;
  v5 = *(_DWORD *)(a1 + 336) <= 0;
  *(_DWORD *)(a1 + 316) = a3;
  if ( !v5 )
    v6 = (a3 * (sub_421640(a1) + 1)) >> 16;
  result = v8;
  if ( v8 )
  {
    (*(void (__thiscall **)(int, int *))(*(_DWORD *)a1 + 48))(a1, &v9);
    (*(void (__thiscall **)(_DWORD, int, int))(**(_DWORD **)(a1 + 308) + 44))(*(_DWORD *)(a1 + 308), v3 + v9, v6 + v10);
    return v8;
  }
  return result;
}

// ===== sub_421500 @ 0x00421500..0x00421512 =====
_DWORD *__usercall sub_421500@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  v2 = *(_DWORD *)(a2 + 312);
  v3 = *(_DWORD *)(a2 + 316);
  *result = v2;
  result[1] = v3;
  return result;
}

// ===== sub_421520 @ 0x00421520..0x0042156C =====
__int64 __usercall sub_421520@<edx:eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  __int64 result; // rax
  int v3[2]; // [esp+0h] [ebp-8h] BYREF

  sub_421500(v3, (int)a2);
  LODWORD(result) = sub_421430((int)a2, v3[0], a1 + v3[1]);
  HIDWORD(result) = result == 0;
  a2[89] = 1;
  a2[90] = 0;
  a2[91] = a1;
  a2[92] = HIDWORD(result);
  return result;
}

// ===== sub_421570 @ 0x00421570..0x004215B6 =====
int __usercall sub_421570@<eax>(_DWORD *a1@<edx>, _DWORD *a2@<ecx>, _DWORD *a3@<edi>)
{
  int result; // eax

  result = a2[89];
  if ( a1 )
  {
    *a1 = a2[90];
    a1[1] = a2[91];
  }
  if ( a3 )
    *a3 = a2[92];
  a2[89] = 0;
  a2[90] = 0;
  a2[91] = 0;
  a2[92] = 0;
  return result;
}

// ===== sub_4215C0 @ 0x004215C0..0x004215D1 =====
int __thiscall sub_4215C0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[77] + 100))(this[77]);
}

// ===== sub_4215E0 @ 0x004215E0..0x00421639 =====
int __thiscall sub_4215E0(_DWORD *this)
{
  int v2; // eax
  int v3; // esi
  int result; // eax
  _BYTE v5[8]; // [esp+4h] [ebp-14h] BYREF
  int v6; // [esp+Ch] [ebp-Ch]

  (*(void (__thiscall **)(_DWORD *, _BYTE *))(*this + 32))(this, v5);
  v2 = this[87] - this[85];
  v3 = this[83];
  result = (v2 - v6) << 16;
  if ( v3 > 2 )
    result /= v3 - 1;
  if ( result <= 0 )
    return 1;
  return result;
}

// ===== sub_421640 @ 0x00421640..0x00421699 =====
int __thiscall sub_421640(_DWORD *this)
{
  int v2; // eax
  int v3; // esi
  int result; // eax
  _BYTE v5[12]; // [esp+4h] [ebp-14h] BYREF
  int v6; // [esp+10h] [ebp-8h]

  (*(void (__thiscall **)(_DWORD *, _BYTE *))(*this + 32))(this, v5);
  v2 = this[88] - this[86];
  v3 = this[84];
  result = (v2 - v6) << 16;
  if ( v3 > 2 )
    result /= v3 - 1;
  if ( result <= 0 )
    return 1;
  return result;
}

// ===== sub_4216A0 @ 0x004216A0..0x00421776 =====
int __usercall sub_4216A0@<eax>(int *a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // eax
  int v3; // eax
  int result; // eax
  int v5; // eax
  _BYTE v6[8]; // [esp+4h] [ebp-14h] BYREF
  int v7; // [esp+Ch] [ebp-Ch]
  int v8; // [esp+10h] [ebp-8h]

  (*(void (__thiscall **)(_DWORD *, _BYTE *))(*a2 + 32))(a2, v6);
  v2 = a2[83];
  if ( v2 <= 0 )
  {
    *a1 = a2[87] - a2[85] - v7;
  }
  else if ( v2 <= 1 )
  {
    *a1 = 0;
  }
  else
  {
    v3 = sub_4215E0(a2);
    *a1 = ((a2[87] - a2[85] - v7) << 16) / v3;
  }
  result = a2[84];
  if ( result <= 0 )
  {
    a1[1] = a2[88] - a2[86] - v8;
  }
  else if ( result <= 1 )
  {
    result = 0;
    a1[1] = 0;
  }
  else
  {
    v5 = sub_421640(a2);
    result = ((a2[88] - a2[86] - v8) << 16) / v5;
    a1[1] = result;
  }
  return result;
}

// ===== sub_421780 @ 0x00421780..0x00421883 =====
_DWORD *__userpurge sub_421780@<eax>(
        int a1@<ecx>,
        int a2@<edi>,
        _DWORD *a3,
        unsigned int a4,
        int a5,
        int a6,
        int a7,
        int a8)
{
  unsigned int v8; // ecx
  int v9; // edx
  int v10; // ebx
  int v11; // edx

  sub_41A400(a1, a3, 1, 1);
  *a3 = &CDspObjLandscape::`vftable';
  if ( !a4 || (v8 = a4, (a4 & 1) != 0) )
    v8 = 64;
  a3[77] = v8;
  v9 = a2;
  if ( !a2 )
    v9 = 16;
  v10 = a5;
  a3[78] = v9;
  if ( !a5 )
    v10 = 16;
  a3[80] = v8 >> 1;
  a3[79] = v10;
  if ( a6 )
    v11 = a6;
  else
    v11 = 35 * v9;
  a3[81] = v11;
  a3[82] = a7;
  a3[83] = a8;
  a3[84] = 0;
  a3[85] = 0;
  a3[86] = 0;
  a3[87] = 0;
  a3[88] = 0;
  a3[89] = 0;
  a3[90] = 0;
  a3[91] = 0;
  a3[92] = 0;
  a3[93] = 0;
  a3[94] = 0;
  return a3;
}

// ===== sub_421890 @ 0x00421890..0x004218B2 =====
void *__thiscall sub_421890(void *this, char a2)
{
  sub_4218C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4218C0 @ 0x004218C0..0x00421922 =====
int __stdcall sub_4218C0(_DWORD *a1)
{
  *a1 = &CDspObjLandscape::`vftable';
  sub_4232F0();
  sub_4233E0();
  sub_423450();
  return sub_41A5E0(a1);
}

// ===== sub_421930 @ 0x00421930..0x00421E5C =====
int __stdcall sub_421930(int a1, int a2, unsigned int a3, int a4, int a5, unsigned int a6, unsigned int *a7)
{
  int result; // eax
  int *v8; // esi
  int *v9; // ebx
  int v10; // edi
  int v11; // ecx
  int v12; // edx
  _DWORD *v13; // edi
  unsigned int *v14; // ecx
  int *v15; // ebx
  unsigned int v16; // esi
  unsigned int v17; // eax
  unsigned int v18; // edx
  bool v19; // zf
  unsigned int v20; // eax
  int v21; // ecx
  unsigned int v22; // esi
  _DWORD *v23; // eax
  int v24; // edx
  int v25; // eax
  int v26; // eax
  int v27; // eax
  void **v28; // edi
  unsigned int v29; // ebx
  unsigned int v30; // esi
  void **v31; // edi
  int v32; // [esp-4h] [ebp-74h]
  int v33; // [esp+Ch] [ebp-64h]
  unsigned int v34; // [esp+Ch] [ebp-64h]
  unsigned int *v35; // [esp+10h] [ebp-60h]
  int v36; // [esp+14h] [ebp-5Ch]
  unsigned int v37; // [esp+18h] [ebp-58h]
  int v38; // [esp+1Ch] [ebp-54h]
  unsigned int v39; // [esp+20h] [ebp-50h]
  int v40; // [esp+24h] [ebp-4Ch]
  unsigned int *v41; // [esp+28h] [ebp-48h]
  _DWORD *v42; // [esp+28h] [ebp-48h]
  int *v43; // [esp+2Ch] [ebp-44h]
  int *v44; // [esp+30h] [ebp-40h]
  int v45; // [esp+34h] [ebp-3Ch]
  _DWORD v46[4]; // [esp+38h] [ebp-38h] BYREF
  _DWORD v47[4]; // [esp+48h] [ebp-28h] BYREF
  int v48[4]; // [esp+58h] [ebp-18h] BYREF
  int v49; // [esp+68h] [ebp-8h]

  if ( !a3 )
  {
    sub_41C110(a1, -1);
    return -2147483646;
  }
  if ( !a6 )
  {
    sub_41C110(a1, -1);
    return -2147483645;
  }
  v36 = -1;
  if ( !sub_407F20(dword_565B30, a2, v48) )
    return -2147483647;
  sub_409190(v47, (int)v48);
  v8 = (int *)operator new[](36 * a3);
  v44 = v8;
  memset(v8, 0, 36 * a3);
  v33 = 0;
  v9 = (int *)(a4 + 8);
  while ( *v9 )
  {
    v10 = v9[1];
    if ( !v10 )
      break;
    v11 = *(v9 - 1);
    v12 = *(v9 - 2) + *v9 - 1;
    v46[0] = *(v9 - 2);
    v46[1] = v11;
    v46[3] = v11 + v10 - 1;
    v46[2] = v12;
    if ( !sub_4090B0(v47, v46) )
    {
      v13 = (_DWORD *)a1;
      sub_41C110(a1, v33);
      v36 = -2147483646;
      goto LABEL_13;
    }
    sub_409030(v49, v10, v8, *v9);
    sub_40A530(v48, v8, -*(v9 - 1), -*(v9 - 2), 128, 0);
    v8[6] = 1;
    v9 += 5;
    v8 += 9;
    if ( ++v33 >= a3 )
    {
      v13 = (_DWORD *)a1;
      goto LABEL_13;
    }
  }
  v36 = -2147483646;
  v13 = (_DWORD *)sub_41C110(a1, v33);
LABEL_13:
  if ( v33 == a3 )
  {
    v43 = (int *)operator new[](176 * a6);
    memset(v43, 0, 176 * a6);
    v34 = 0;
    v14 = a7;
    v35 = a7;
    v15 = v43 + 36;
    while ( 1 )
    {
      v16 = *v14;
      if ( !*v14 )
        break;
      if ( v16 > 0x20 )
        break;
      v17 = v14[33];
      if ( !v17 || v17 > 0x20 )
        break;
      v18 = 0;
      v37 = 0;
      v39 = 0;
      v19 = v16 == 0;
      if ( v16 )
      {
        v41 = v14 + 1;
        do
        {
          v20 = *v41;
          if ( *v41 >= a3 )
          {
            if ( v20 != -1 )
              break;
          }
          else
          {
            v21 = a4 + 20 * v20;
            if ( v37 < *(_DWORD *)(v21 + 8) )
              v37 = *(_DWORD *)(v21 + 8);
            if ( v39 < *(_DWORD *)(v21 + 12) + v18 * v13[79] )
              v39 = *(_DWORD *)(v21 + 12) + v18 * v13[79];
          }
          ++v41;
          ++v18;
        }
        while ( v18 < v16 );
        v19 = v18 == v16;
      }
      if ( !v19 )
        break;
      sub_409030(v49, v39, v15, v37);
      sub_40A620((int)v15, 0);
      v15[6] = 0;
      v22 = 1;
      v38 = 0;
      v40 = 1;
      if ( *v35 )
      {
        v45 = 1;
        v42 = v35 + 1;
        do
        {
          if ( *v42 == -1 )
          {
            v40 = 0;
          }
          else
          {
            sub_40A530(
              &v44[9 * *v42],
              v15,
              v39 - v13[79] * (v22 - 1) - v44[9 * *v42 + 3],
              (v37 - v44[9 * *v42 + 2]) >> 1,
              1,
              0);
            if ( v40 )
            {
              if ( *(_DWORD *)(a4 + 20 * *v42 + 16) )
              {
                v40 = 0;
              }
              else if ( v22 == v35[33] )
              {
                v38 += a5;
                v40 = 0;
              }
              else
              {
                ++v38;
              }
            }
            if ( v22 == v35[33] && v22 < *v35 )
            {
              v23 = operator new(0x18u);
              v24 = v15[3];
              v32 = v15[2];
              v15[6] = (int)v23;
              sub_409030(3, v24, v23, v32);
              sub_4190C0((_DWORD *)v15[6], v15);
              v25 = (unsigned int)v15[3] / *(_DWORD *)(a1 + 316) - v35[33] - 2;
              if ( v25 < 1 )
                v25 = 1;
              v22 = v45;
              v15[7] = v25;
              v13 = (_DWORD *)a1;
            }
            else
            {
              v13 = (_DWORD *)a1;
            }
          }
          ++v42;
          v45 = ++v22;
        }
        while ( v22 - 1 < *v35 );
      }
      qmemcpy(v15 - 36, v35, 0x88u);
      v26 = (int)(*(_DWORD *)(a1 + 308) - v37) >> 1;
      *(v15 - 2) = v26;
      if ( v26 > 0 )
        v27 = 0;
      else
        v27 = v38 * *(_DWORD *)(a1 + 316);
      v35 += 34;
      *(v15 - 1) = v27;
      v15 += 44;
      ++v34;
      v13 = (_DWORD *)a1;
      if ( v34 >= a6 )
        goto LABEL_56;
      v14 = v35;
    }
    sub_41C110((int)v13, v34);
    v36 = -2147483645;
LABEL_56:
    if ( v34 == a6 )
    {
      sub_4232F0();
      sub_4233E0();
      v13[84] = a3;
      v13[85] = v44;
      result = 0;
      v13[86] = a6;
      v13[87] = v43;
      return result;
    }
    v28 = (void **)(v43 + 36);
    v29 = a6;
    do
    {
      operator delete[](*v28);
      v28 += 44;
      --v29;
    }
    while ( v29 );
    operator delete[](v43);
  }
  if ( !v44 )
    return v36;
  v30 = a3;
  v31 = (void **)v44;
  do
  {
    operator delete[](*v31);
    v31 += 9;
    --v30;
  }
  while ( v30 );
  operator delete[](v44);
  return v36;
}

// ===== sub_421E60 @ 0x00421E60..0x0042204B =====
int __userpurge sub_421E60@<eax>(unsigned int a1@<edi>, _DWORD *a2, unsigned int a3, _DWORD *a4)
{
  int v6; // eax
  unsigned int v7; // ecx
  _DWORD *v8; // eax
  void *v9; // eax
  unsigned int i; // esi
  _DWORD *v11; // [esp+8h] [ebp-8h]
  _DWORD *v12; // [esp+Ch] [ebp-4h]
  _DWORD *v13; // [esp+Ch] [ebp-4h]
  int v14; // [esp+18h] [ebp+8h]
  unsigned int v15; // [esp+18h] [ebp+8h]

  if ( !a2[86] )
    return -2147483644;
  if ( !a1 )
    return -2147483643;
  if ( a1 > 0x100 )
    return -2147483643;
  if ( !a3 )
    return -2147483643;
  if ( a3 > 0x100 )
    return -2147483643;
  v6 = (*(int (__thiscall **)(_DWORD *))(*a2 + 88))(a2);
  if ( !sub_423090(v6) )
    return -2147483643;
  v14 = 0;
  v12 = a4;
  while ( 2 )
  {
    v7 = 0;
    v8 = v12;
    do
    {
      if ( *v8 >= a2[86] && *v8 != -1 )
        return -2147483642;
      ++v7;
      ++v8;
    }
    while ( v7 < a1 );
    v12 += a1;
    if ( ++v14 < a3 )
      continue;
    break;
  }
  v11 = operator new[](12 * a3);
  memset(v11, 0, 12 * a3);
  v15 = 0;
  v13 = v11 + 2;
  do
  {
    v9 = operator new[](68 * a1);
    *v13 = v9;
    memset(v9, 0, 68 * a1);
    for ( i = 0; i < a1; ++i )
      sub_4234A0(a2, v11, v15, i, a1, a3, a4);
    v13 += 3;
    ++v15;
  }
  while ( v15 < a3 );
  sub_4233E0();
  a2[88] = a1;
  a2[89] = a3;
  a2[90] = v11;
  sub_4231A0();
  (*(void (__thiscall **)(_DWORD *, unsigned int))(*a2 + 116))(a2, a2[80] + a1 * a2[77]);
  a2[92] = 1;
  return 0;
}

// ===== sub_422050 @ 0x00422050..0x0042229A =====
int __userpurge sub_422050@<eax>(unsigned int a1@<edx>, _DWORD *a2@<edi>, unsigned int a3)
{
  unsigned int v3; // eax
  int v4; // eax
  _DWORD *v5; // esi
  int v6; // edx
  int v7; // ebx
  unsigned int *v8; // edx
  unsigned int v9; // eax
  unsigned int v10; // ecx
  _DWORD *v11; // edx
  void *v12; // eax
  unsigned int v13; // ebx
  int v14; // ebx
  unsigned int v15; // ecx
  _DWORD *v16; // edx
  int v18; // [esp+8h] [ebp-10h]
  unsigned int v19; // [esp+Ch] [ebp-Ch]
  int v20; // [esp+Ch] [ebp-Ch]
  int v21; // [esp+10h] [ebp-8h]
  unsigned int v22; // [esp+10h] [ebp-8h]
  _DWORD *Src; // [esp+14h] [ebp-4h]
  char *Srca; // [esp+14h] [ebp-4h]

  if ( !a2[86] )
    return -2147483644;
  v3 = a2[84];
  if ( a3 >= v3 )
    return -2147483640;
  if ( a1 >= v3 )
    return -2147483639;
  v4 = a2[85];
  v5 = (_DWORD *)(v4 + 36 * a3);
  v6 = v4 + 36 * a1;
  if ( *(_DWORD *)(v6 + 8) != v5[2] || *(_DWORD *)(v6 + 12) != v5[3] || *(_DWORD *)(v6 + 16) != v5[4] )
    return -2147483639;
  sub_40ADF0((int)v5, (char **)v6);
  if ( v5[6] )
  {
    v7 = 0;
    v18 = 0;
    Src = operator new[](4 * a2[86]);
    v19 = 0;
    if ( a2[86] )
    {
      v21 = 0;
      do
      {
        v8 = (unsigned int *)(a2[87] + v21);
        v9 = 0;
        v10 = *v8;
        if ( *v8 )
        {
          v11 = v8 + 1;
          while ( a3 != *v11 )
          {
            ++v9;
            ++v11;
            if ( v9 >= v10 )
            {
              v7 = v18;
              goto LABEL_16;
            }
          }
          Src[v18] = v19;
          v7 = ++v18;
        }
LABEL_16:
        v21 += 176;
        ++v19;
      }
      while ( v19 < a2[86] );
      if ( v7 )
      {
        v5[7] = v7;
        v12 = operator new[](4 * v7);
        v5[8] = v12;
        memcpy_0(v12, Src, 4 * v7);
      }
    }
    operator delete[](Src);
    v5[6] = 0;
  }
  if ( v5[7] )
  {
    v13 = 0;
    do
    {
      sub_423780(a2);
      ++v13;
    }
    while ( v13 < v5[7] );
  }
  if ( a2[92] )
  {
    if ( v5[7] )
    {
      v22 = 0;
      if ( a2[89] )
      {
        v20 = 0;
        do
        {
          v14 = 0;
          for ( Srca = 0; (unsigned int)Srca < a2[88]; ++Srca )
          {
            v15 = 0;
            if ( v5[7] )
            {
              v16 = (_DWORD *)v5[8];
              while ( *v16 != *(_DWORD *)(*(_DWORD *)(a2[90] + v20 + 8) + v14) )
              {
                ++v15;
                ++v16;
                if ( v15 >= v5[7] )
                  goto LABEL_34;
              }
              sub_423850(a2);
            }
LABEL_34:
            v14 += 68;
          }
          v20 += 12;
          ++v22;
        }
        while ( v22 < a2[89] );
      }
    }
  }
  return 0;
}

// ===== sub_4222A0 @ 0x004222A0..0x004224C7 =====
int __userpurge sub_4222A0@<eax>(_DWORD *a1@<esi>, unsigned int a2, unsigned int a3, unsigned int a4)
{
  unsigned int v4; // ebx
  unsigned int v5; // edi
  unsigned int v7; // ecx
  unsigned int v8; // edx
  unsigned int v9; // eax
  unsigned int v10; // eax
  int v11; // ecx
  unsigned int v12; // edi
  unsigned int v13; // [esp+8h] [ebp-8h]
  unsigned int v14; // [esp+8h] [ebp-8h]
  int v15; // [esp+Ch] [ebp-4h]

  v4 = a3;
  v5 = a4;
  if ( !a1[90] )
    return -2147483635;
  v7 = a1[89];
  v8 = a2;
  if ( a2 >= v7 )
    return -2147483637;
  v9 = a1[88];
  if ( a3 >= v9 )
    return -2147483636;
  if ( a4 >= a1[86] && a4 != -1 )
    return -2147483638;
  if ( !a1[91] )
  {
    a1[91] = operator new[](4 * v7 * v9);
    v13 = 0;
    if ( a1[89] )
    {
      v15 = 0;
      do
      {
        v10 = 0;
        if ( a1[88] )
        {
          v11 = 0;
          do
          {
            v12 = v10 + v13 * a1[88];
            ++v10;
            *(_DWORD *)(a1[91] + 4 * v12) = *(_DWORD *)(v11 + *(_DWORD *)(v15 + a1[90] + 8));
            v11 += 68;
          }
          while ( v10 < a1[88] );
          v4 = a3;
        }
        v15 += 12;
        ++v13;
      }
      while ( v13 < a1[89] );
      v5 = a4;
    }
    v8 = a2;
  }
  *(_DWORD *)(a1[91] + 4 * (v4 + v8 * a1[88])) = v5;
  if ( (v8 & 1) != 0 )
    v14 = v4;
  else
    v14 = v4 - 1;
  sub_423850(a1);
  sub_4234A0(a1, a1[90], a2, v4, a1[88], a1[89], a1[91]);
  sub_423850(a1);
  sub_4234A0(a1, a1[90], a2 - 1, v14, a1[88], a1[89], a1[91]);
  sub_423850(a1);
  sub_4234A0(a1, a1[90], a2 - 1, v14 + 1, a1[88], a1[89], a1[91]);
  sub_423850(a1);
  sub_4234A0(a1, a1[90], a2 - 2, v4, a1[88], a1[89], a1[91]);
  sub_423850(a1);
  sub_4231A0();
  return 0;
}

// ===== sub_4224D0 @ 0x004224D0..0x00422676 =====
int __stdcall sub_4224D0(int a1, int a2, unsigned int a3, int a4)
{
  unsigned int v4; // edi
  int result; // eax
  _DWORD *v6; // esi
  int *v7; // ebx
  int v8; // edi
  int v9; // ecx
  int v10; // edx
  int v11; // [esp+Ch] [ebp-44h]
  int v12; // [esp+10h] [ebp-40h]
  _DWORD *v13; // [esp+14h] [ebp-3Ch]
  _DWORD v14[4]; // [esp+18h] [ebp-38h] BYREF
  _DWORD v15[4]; // [esp+28h] [ebp-28h] BYREF
  int v16[6]; // [esp+38h] [ebp-18h] BYREF

  v4 = a3;
  if ( !a3 )
  {
    sub_41C110(a1, -1);
    return -2147483646;
  }
  v12 = -1;
  if ( !sub_407F20(dword_565B30, a2, v16) )
    return -2147483647;
  sub_409190(v15, (int)v16);
  v6 = operator new[](24 * a3);
  v13 = v6;
  memset(v6, 0, 24 * a3);
  v11 = 0;
  v7 = (int *)(a4 + 8);
  while ( *v7 )
  {
    v8 = v7[1];
    if ( !v8 )
    {
      v4 = a3;
      break;
    }
    v9 = *(v7 - 1);
    v10 = *(v7 - 2) + *v7 - 1;
    v14[0] = *(v7 - 2);
    v14[1] = v9;
    v14[3] = v9 + v8 - 1;
    v14[2] = v10;
    if ( !sub_4090B0(v15, v14) )
    {
      sub_41C110(a1, v11);
      v4 = a3;
      goto LABEL_14;
    }
    sub_409030(v16[4], v8, v6, *v7);
    sub_40A530(v16, v6, -*(v7 - 1), -*(v7 - 2), 128, 0);
    v4 = a3;
    v7 += 5;
    v6 += 6;
    if ( ++v11 >= a3 )
      goto LABEL_15;
  }
  sub_41C110(a1, v11);
LABEL_14:
  v12 = -2147483646;
LABEL_15:
  if ( v11 == v4 )
  {
    sub_423450();
    result = 0;
    *(_DWORD *)(a1 + 372) = v4;
    *(_DWORD *)(a1 + 376) = v13;
    return result;
  }
  return v12;
}

// ===== sub_422680 @ 0x00422680..0x00422729 =====
int __userpurge sub_422680@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<edx>,
        _DWORD *a3@<esi>,
        unsigned int a4,
        unsigned int a5,
        int a6)
{
  unsigned int v7; // edi

  if ( !a3[90] )
    return -2147483635;
  if ( a4 >= a3[89] )
    return -2147483637;
  if ( a1 >= a3[88] )
    return -2147483636;
  if ( a5 >= 4 )
    return -2147483632;
  if ( a2 >= a3[93] && a2 != -1 )
    return -2147483631;
  v7 = a1 + a5 + 16 * a1;
  *(_DWORD *)(*(_DWORD *)(12 * a4 + a3[90] + 8) + 4 * v7 + 24) = a2;
  *(_DWORD *)(*(_DWORD *)(12 * a4 + a3[90] + 8) + 4 * v7 + 40) = a6;
  sub_423850(a3);
  return 0;
}

// ===== sub_422730 @ 0x00422730..0x004227BC =====
int __userpurge sub_422730@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<edx>,
        _DWORD *a3@<esi>,
        unsigned int a4,
        unsigned int a5,
        int a6)
{
  int v6; // ecx
  _DWORD *v8; // ecx

  v6 = a3[90];
  if ( !v6 )
    return -2147483635;
  if ( a2 >= a3[89] )
    return -2147483637;
  if ( a1 >= a3[88] )
    return -2147483636;
  if ( a4 > 2 )
    return -2147483630;
  if ( a5 > 0x100 )
    return -2147483629;
  v8 = (_DWORD *)(*(_DWORD *)(v6 + 12 * a2 + 8) + 68 * a1);
  v8[14] = a4;
  v8[15] = a5;
  v8[16] = a6;
  sub_423850(a3);
  return 0;
}

// ===== sub_4227C0 @ 0x004227C0..0x004228A7 =====
int __userpurge sub_4227C0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<edx>, int a3, unsigned int a4)
{
  int v4; // ecx
  int result; // eax
  int v6; // ecx
  int v7; // edi
  _DWORD v8[6]; // [esp+10h] [ebp-18h] BYREF

  v4 = a2[90];
  if ( !v4 )
    return -2147483635;
  if ( a4 >= a2[89] )
    return -2147483637;
  if ( a1 >= a2[88] )
    return -2147483636;
  v6 = *(_DWORD *)(*(_DWORD *)(v4 + 12 * a4 + 8) + 68 * a1);
  result = 14;
  if ( v6 != -1 )
  {
    v7 = a2[85] + 36 * *(_DWORD *)(a2[87] + 176 * v6 + 4 * *(_DWORD *)(a2[87] + 176 * v6 + 132));
    if ( sub_407DA0(a3, (_DWORD *)dword_565B30, *(_DWORD *)(v7 + 8), *(_DWORD *)(v7 + 12), *(_DWORD *)(v7 + 16)) )
    {
      sub_407F20(dword_565B30, a3, v8);
      sub_40ADF0((int)v8, (char **)v7);
      return 0;
    }
    else
    {
      return -2147483633;
    }
  }
  return result;
}

// ===== sub_4228B0 @ 0x004228B0..0x0042291D =====
int __userpurge sub_4228B0@<eax>(_DWORD *a1@<eax>, unsigned int a2@<edx>, _DWORD *a3, unsigned int a4)
{
  int v4; // ecx
  int result; // eax
  int v6; // ecx
  bool v7; // zf
  int v8; // ecx

  v4 = a1[90];
  if ( !v4 )
    return -2147483635;
  if ( a4 >= a1[89] )
    return -2147483637;
  if ( a2 >= a1[88] )
    return -2147483636;
  v6 = *(_DWORD *)(v4 + 12 * a4 + 8);
  v7 = *(_DWORD *)(v6 + 68 * a2) == -1;
  v8 = v6 + 68 * a2;
  result = 14;
  if ( !v7 )
  {
    *a3 = *(unsigned __int16 *)(v8 + 22);
    return 0;
  }
  return result;
}

// ===== sub_422920 @ 0x00422920..0x00422AE5 =====
int __userpurge sub_422920@<eax>(_DWORD *a1@<esi>, int *a2, int a3, int a4, int a5)
{
  signed int v5; // ecx
  unsigned int v6; // ebx
  int v7; // ecx
  signed int v8; // eax
  int v9; // edx
  signed int v10; // ecx
  signed int v11; // ecx
  int v12; // edx
  int v13; // eax
  int v14; // edx
  int v15; // edi
  int v16; // eax
  _DWORD *v17; // ecx
  int v18; // edi
  int v19; // edx
  _DWORD *v20; // eax
  _DWORD *v21; // edx
  int v22; // ecx
  bool v23; // zf
  int v25; // [esp+0h] [ebp-1Ch]
  int v26; // [esp+4h] [ebp-18h]
  int v27; // [esp+8h] [ebp-14h]
  int i; // [esp+Ch] [ebp-10h]
  int v29; // [esp+10h] [ebp-Ch]
  signed int v30; // [esp+14h] [ebp-8h]
  int v31; // [esp+18h] [ebp-4h]

  if ( a1[92] )
  {
    v5 = a1[77];
    v6 = a1[78];
    v27 = a3 / v5;
    v26 = (__PAIR64__(a3 / v5, a3 % (unsigned int)v5) - (unsigned int)a1[80]) >> 32;
    v7 = (a4 >= a1[81]) + (a4 - a1[81]) / (int)v6;
    v8 = v7 + 32 * a1[79] / v6;
    v9 = v7 < 0 ? 0 : v7;
    v10 = a1[89];
    v25 = v9;
    if ( v8 >= v10 )
      v8 = v10 - 1;
    LOBYTE(v11) = v8;
    v30 = v8;
    if ( v9 > v8 )
      return 14;
    v12 = 12 * v8;
    for ( i = 12 * v8; ; i -= 12 )
    {
      if ( (v11 & 1) != 0 )
      {
        v31 = v26;
        v13 = v26;
      }
      else
      {
        v13 = v27;
        v31 = v27;
      }
      if ( v13 < 0 )
        goto LABEL_24;
      if ( v13 >= a1[88] )
        goto LABEL_24;
      v14 = *(_DWORD *)(a1[90] + v12 + 8);
      v15 = 17 * v13;
      v16 = *(_DWORD *)(v14 + 68 * v13);
      v17 = (_DWORD *)(v14 + 4 * v15);
      if ( v16 == -1 )
        goto LABEL_24;
      v29 = v17[1];
      if ( v29 > a3 )
        goto LABEL_24;
      if ( a3 > v17[3] )
        goto LABEL_24;
      v18 = v17[2];
      if ( v18 > a4 || a4 > v17[4] )
        goto LABEL_24;
      v19 = 176 * v16 + a1[87];
      v20 = (_DWORD *)(v19 + 144);
      if ( a5 == 1 )
      {
        v21 = *(_DWORD **)(v19 + 168);
        if ( v21 )
          v20 = v21;
      }
      if ( v20[4] == 2 )
      {
        v22 = 0;
        v23 = *(_BYTE *)(v20[5] * (a3 - v29) + v20[1] * (a4 - v18) + *v20 + 3) == 0;
      }
      else
      {
        v22 = v20[4] - 3;
        if ( v20[4] != 3 )
          goto LABEL_24;
        v23 = *(_BYTE *)(v20[5] * (a3 - v29) + v20[1] * (a4 - v18) + *v20) == 0;
      }
      LOBYTE(v22) = !v23;
      if ( v22 )
      {
        *a2 = v31;
        a2[1] = v30;
        return 0;
      }
LABEL_24:
      v11 = v30 - 1;
      v12 = i - 12;
      v30 = v11;
      if ( v25 > v11 )
        return 14;
    }
  }
  return -2147483635;
}

// ===== sub_422AF0 @ 0x00422AF0..0x00422B38 =====
int __userpurge sub_422AF0@<eax>(int a1@<ecx>, int a2@<esi>, int a3, int a4, int a5, int a6)
{
  if ( !(*(int (__thiscall **)(int, int))(*(_DWORD *)a2 + 84))(a2, a1) )
    return -2147483641;
  (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a2 + 44))(a2, a3, a4);
  sub_41B600(a2, a5);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)a2 + 72))(a2, a6);
  return 0;
}

// ===== sub_422B40 @ 0x00422B40..0x00423030 =====
void __thiscall sub_422B40(_DWORD *this, int *a2, int *a3, unsigned int a4)
{
  _DWORD *v4; // ecx
  _DWORD *v5; // ecx
  int v6; // eax
  unsigned int *v7; // eax
  unsigned int v8; // edx
  _DWORD *v9; // edi
  int v10; // esi
  int v11; // eax
  int v12; // edx
  int v13; // ecx
  _DWORD *v14; // esi
  int v15; // edx
  int v16; // ecx
  int v17; // edx
  int v18; // ecx
  int v19; // edx
  int v20; // ecx
  int v21; // edx
  int v22; // ecx
  int v23; // eax
  _DWORD *v24; // esi
  int v25; // edi
  int v26; // eax
  int v27; // eax
  int v28; // ecx
  int v29; // edx
  int v30; // eax
  int v31; // ecx
  int v32; // ecx
  bool v33; // zf
  unsigned int v34; // [esp+Ch] [ebp-BCh]
  _DWORD *v35; // [esp+Ch] [ebp-BCh]
  int v37; // [esp+14h] [ebp-B4h] BYREF
  int v38; // [esp+18h] [ebp-B0h]
  int v39; // [esp+1Ch] [ebp-ACh]
  int v40; // [esp+20h] [ebp-A8h]
  int v41; // [esp+24h] [ebp-A4h]
  int v42; // [esp+28h] [ebp-A0h]
  unsigned int v43; // [esp+2Ch] [ebp-9Ch]
  unsigned int v44; // [esp+30h] [ebp-98h]
  char *v45; // [esp+34h] [ebp-94h] BYREF
  int v46; // [esp+38h] [ebp-90h]
  int v47; // [esp+3Ch] [ebp-8Ch]
  int v48; // [esp+40h] [ebp-88h]
  int v49; // [esp+44h] [ebp-84h]
  int v50; // [esp+48h] [ebp-80h]
  unsigned int v51; // [esp+4Ch] [ebp-7Ch]
  int v52; // [esp+50h] [ebp-78h]
  int v53; // [esp+54h] [ebp-74h] BYREF
  int v54; // [esp+58h] [ebp-70h]
  int v55; // [esp+5Ch] [ebp-6Ch]
  int v56; // [esp+60h] [ebp-68h]
  int v57; // [esp+64h] [ebp-64h]
  int v58; // [esp+68h] [ebp-60h] BYREF
  int v59; // [esp+6Ch] [ebp-5Ch]
  int v60; // [esp+70h] [ebp-58h]
  int v61; // [esp+74h] [ebp-54h]
  int v62; // [esp+78h] [ebp-50h]
  int v63; // [esp+7Ch] [ebp-4Ch]
  _DWORD *v64; // [esp+80h] [ebp-48h]
  _DWORD *v65; // [esp+84h] [ebp-44h]
  int v66; // [esp+88h] [ebp-40h]
  int v67; // [esp+8Ch] [ebp-3Ch]
  int v68; // [esp+90h] [ebp-38h]
  int v69; // [esp+94h] [ebp-34h]
  void *v70[6]; // [esp+98h] [ebp-30h] BYREF
  void *v71[6]; // [esp+B0h] [ebp-18h] BYREF

  if ( this[92] )
  {
    v43 = sub_41B610((int)this);
    v44 = sub_41B770(v4);
    v51 = 0;
    if ( v5[89] )
    {
      v6 = 0;
      v42 = 0;
      do
      {
        v7 = (unsigned int *)(v5[90] + v6);
        v8 = a4;
        if ( a4 >= *v7 && a4 <= v7[1] )
        {
          v52 = 0;
          if ( v5[88] )
          {
            v41 = 0;
            while ( 1 )
            {
              v9 = (_DWORD *)(v41 + *(_DWORD *)(v42 + v5[90] + 8));
              v64 = v9;
              if ( v8 == v9[5] )
              {
                v10 = *v9;
                if ( *v9 != -1 )
                {
                  v11 = v9[3];
                  v12 = v9[2];
                  v37 = v9[1];
                  v13 = v9[4];
                  v39 = v11;
                  v40 = v13;
                  v38 = v12;
                  if ( sub_409110(&v37, a3) )
                  {
                    v14 = (_DWORD *)(this[87] + 176 * v10);
                    v15 = a2[1];
                    v58 = *a2;
                    v16 = a2[2];
                    v59 = v15;
                    v17 = a2[3];
                    v60 = v16;
                    v18 = a2[4];
                    v61 = v17;
                    v19 = a2[5];
                    v62 = v18;
                    v63 = v19;
                    v45 = (char *)v14[36];
                    v46 = v14[37];
                    v47 = v14[38];
                    v48 = v14[39];
                    v20 = v14[40];
                    v65 = v14;
                    v49 = v20;
                    v21 = v14[41];
                    v53 = v37;
                    v50 = v21;
                    v56 = v40;
                    v54 = v38;
                    v22 = a3[1];
                    v55 = v39;
                    sub_409170(-v22, -*a3, &v53);
                    sub_4091B0(&v58, &v53);
                    sub_409170(-v9[2], -v9[1], &v37);
                    sub_4091B0(&v45, &v37);
                    v23 = v9[14];
                    if ( v23 )
                    {
                      v34 = 32;
                      if ( v23 == 2 )
                        v34 = 33;
                      sub_409030(v49, v48, v71, v47);
                      sub_4188D0((int *)&v45, (int **)v71, v9[16]);
                      sub_409030(v49, v48, v70, v47);
                      sub_40ADF0((int)v70, &v45);
                      sub_40A9E0((int)v70, (int)v71, v34, 256 - v9[15], 1);
                      sub_40A9E0((int)&v58, (int)v70, v43, v44, 1);
                      operator delete(v71[0]);
                      operator delete(v70[0]);
                    }
                    else
                    {
                      sub_40A9E0((int)&v58, (int)&v45, v43, v44, 1);
                    }
                    v24 = v9 + 6;
                    v35 = v9 + 6;
                    v57 = 4;
                    do
                    {
                      if ( *v24 != -1 )
                      {
                        v25 = this[94] + 24 * *v24;
                        sub_409190(&v37, v25);
                        v26 = this[94] + 24 * *v24;
                        sub_409170(
                          v64[2] + v65[39] - v65[33] * this[79] - *(_DWORD *)(v26 + 12),
                          v64[1] + ((v65[38] - *(_DWORD *)(v26 + 8)) >> 1),
                          &v37);
                        v66 = v37;
                        v68 = v39;
                        v69 = v40;
                        v67 = v38;
                        if ( sub_409110(&v37, a3) )
                        {
                          v27 = a2[1];
                          v28 = a2[2];
                          v58 = *a2;
                          v29 = a2[3];
                          v59 = v27;
                          v30 = a2[4];
                          v60 = v28;
                          v31 = a2[5];
                          v61 = v29;
                          v62 = v30;
                          v53 = v37;
                          v63 = v31;
                          v54 = v38;
                          v55 = v39;
                          v32 = a3[1];
                          v56 = v40;
                          sub_409170(-v32, -*a3, &v53);
                          sub_4091B0(&v58, &v53);
                          v45 = *(char **)v25;
                          v46 = *(_DWORD *)(v25 + 4);
                          v47 = *(_DWORD *)(v25 + 8);
                          v48 = *(_DWORD *)(v25 + 12);
                          v49 = *(_DWORD *)(v25 + 16);
                          v50 = *(_DWORD *)(v25 + 20);
                          sub_409170(-v67, -v66, &v37);
                          sub_4091B0(&v45, &v37);
                          sub_40A9E0((int)&v58, (int)&v45, v43, 256 - (((256 - v44) * (256 - v35[4])) >> 8), 1);
                        }
                      }
                      v24 = v35 + 1;
                      v33 = v57-- == 1;
                      ++v35;
                    }
                    while ( !v33 );
                  }
                }
                v5 = this;
              }
              v41 += 68;
              if ( (unsigned int)++v52 >= v5[88] )
                break;
              v8 = a4;
            }
          }
        }
        v6 = v42 + 12;
        ++v51;
        v42 += 12;
      }
      while ( v51 < v5[89] );
    }
  }
}

// ===== sub_423030 @ 0x00423030..0x0042304A =====
int __thiscall sub_423030(_DWORD *this, int a2, int a3)
{
  return sub_41B470(this, a2 << 16, a3 << 16);
}

// ===== sub_423050 @ 0x00423050..0x0042308F =====
int __stdcall sub_423050(int a1)
{
  unsigned int v1; // edx
  _DWORD *v2; // ecx

  if ( !sub_423090(a1) )
    return 0;
  sub_41B8B0(v2, v1);
  sub_4230C0();
  sub_4231A0();
  return 1;
}

// ===== sub_423090 @ 0x00423090..0x004230B4 =====
BOOL __userpurge sub_423090@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  if ( !a1 )
    a1 = 1;
  return (unsigned int)(a3 + *(_DWORD *)(a2 + 328) * (a1 - 1)) < 0x10000;
}

// ===== sub_4230C0 @ 0x004230C0..0x00423192 =====
void __usercall sub_4230C0(_DWORD *a1@<esi>)
{
  unsigned int v1; // edi
  int v2; // ebx
  int v3; // ecx
  int v4; // edx
  unsigned int v5; // [esp+4h] [ebp-Ch]
  unsigned int i; // [esp+8h] [ebp-8h]
  int v7; // [esp+Ch] [ebp-4h]

  v1 = 0;
  if ( a1[90] )
  {
    v5 = (*(int (__thiscall **)(_DWORD *))(*a1 + 28))(a1) & 0xFFFFE01F;
    if ( a1[89] )
    {
      v7 = 0;
      do
      {
        v2 = 0;
        for ( i = 0; i < a1[88]; ++i )
        {
          v3 = *(_DWORD *)(v7 + a1[90] + 8);
          v4 = *(_DWORD *)(v3 + v2);
          if ( v4 != -1 )
          {
            if ( *(_DWORD *)(v3 + v2 + 20) )
              *(_DWORD *)(v3 + v2 + 20) = v5
                                        + 32
                                        * (v1
                                         + ((v1 * a1[82]
                                           + a1[83]
                                           * (*(_DWORD *)(a1[87] + 176 * v4 + 172)
                                            + *(_DWORD *)(a1[87] + 176 * v4 + 132)
                                            - 1)) << 11));
          }
          v2 += 68;
        }
        v7 += 12;
        ++v1;
      }
      while ( v1 < a1[89] );
    }
  }
}

// ===== sub_4231A0 @ 0x004231A0..0x004232E5 =====
void __usercall sub_4231A0(_DWORD *a1@<eax>)
{
  size_t v2; // esi
  char *v3; // ebx
  int v4; // eax
  unsigned int v5; // edx
  char *v6; // ebx
  _DWORD *v7; // eax
  int v8; // esi
  unsigned int i; // ecx
  unsigned int v10; // eax
  bool v11; // cf
  char *v12; // [esp+8h] [ebp-20h]
  unsigned int v13; // [esp+Ch] [ebp-1Ch]
  int v14; // [esp+10h] [ebp-18h]
  int v15; // [esp+14h] [ebp-14h]
  unsigned int v16; // [esp+18h] [ebp-10h]
  unsigned int v17; // [esp+1Ch] [ebp-Ch]
  int v18; // [esp+20h] [ebp-8h]
  size_t v19; // [esp+24h] [ebp-4h]

  v2 = 0;
  sub_41AF00((int)a1, 0, 0);
  if ( a1[90] )
  {
    v3 = (char *)operator new[](4 * a1[89] * a1[88]);
    v4 = 0;
    v12 = v3;
    v19 = 0;
    v13 = 0;
    if ( a1[89] )
    {
      v15 = 0;
      do
      {
        v5 = 0;
        v6 = &v3[4 * v2];
        v17 = -1;
        v16 = 0;
        v14 = 0;
        if ( a1[88] )
        {
          v18 = 0;
          do
          {
            v7 = (_DWORD *)(v18 + *(_DWORD *)(v4 + a1[90] + 8));
            if ( *v7 != -1 )
            {
              v8 = v7[5];
              if ( v8 )
              {
                for ( i = 0; i < v5; ++i )
                {
                  if ( v8 == *(_DWORD *)&v6[4 * i] )
                    break;
                }
                if ( i == v5 )
                {
                  ++v19;
                  *(_DWORD *)&v6[4 * v5++] = v8;
                  if ( v7[5] < v17 )
                    v17 = v7[5];
                  v10 = v7[5];
                  if ( v16 < v10 )
                    v16 = v10;
                }
              }
            }
            v18 += 68;
            v11 = (unsigned int)++v14 < a1[88];
            v4 = v15;
          }
          while ( v11 );
        }
        v3 = v12;
        v2 = v19;
        *(_DWORD *)(v4 + a1[90]) = v17;
        *(_DWORD *)(v4 + a1[90] + 4) = v16;
        v4 += 12;
        ++v13;
        v15 = v4;
      }
      while ( v13 < a1[89] );
    }
    qsort(v3, v2, 4u, CompareFunction);
    sub_41AF00((int)a1, v2, v3);
    operator delete[](v3);
  }
}

// ===== sub_4232F0 @ 0x004232F0..0x004233DE =====
int __usercall sub_4232F0@<eax>(_DWORD *a1@<esi>)
{
  unsigned int v1; // ebx
  int v2; // edi
  int v3; // edi
  unsigned int v4; // ebx
  void **v5; // eax
  int result; // eax
  void *v7; // [esp-4h] [ebp-Ch]
  void *v8; // [esp-4h] [ebp-Ch]

  v1 = 0;
  if ( a1[84] )
  {
    v2 = 0;
    do
    {
      operator delete[](*(void **)(v2 + a1[85]));
      operator delete[](*(void **)(v2 + a1[85] + 32));
      ++v1;
      v2 += 36;
    }
    while ( v1 < a1[84] );
  }
  v7 = (void *)a1[85];
  a1[84] = 0;
  operator delete[](v7);
  v3 = 0;
  v4 = 0;
  for ( a1[85] = 0; v4 < a1[86]; v3 += 176 )
  {
    operator delete[](*(void **)(a1[87] + v3 + 144));
    v5 = *(void ***)(a1[87] + v3 + 168);
    if ( v5 )
    {
      operator delete[](*v5);
      operator delete(*(void **)(a1[87] + v3 + 168));
    }
    ++v4;
  }
  v8 = (void *)a1[87];
  a1[86] = 0;
  operator delete[](v8);
  result = 0;
  a1[87] = 0;
  a1[92] = 0;
  return result;
}

// ===== sub_4233E0 @ 0x004233E0..0x0042344D =====
int __usercall sub_4233E0@<eax>(int a1@<esi>)
{
  unsigned int v1; // edi
  int v2; // ebx
  int result; // eax

  v1 = 0;
  if ( *(_DWORD *)(a1 + 356) )
  {
    v2 = 0;
    do
    {
      operator delete[](*(void **)(v2 + *(_DWORD *)(a1 + 360) + 8));
      ++v1;
      v2 += 12;
    }
    while ( v1 < *(_DWORD *)(a1 + 356) );
  }
  operator delete[](*(void **)(a1 + 360));
  operator delete[](*(void **)(a1 + 364));
  result = 0;
  *(_DWORD *)(a1 + 352) = 0;
  *(_DWORD *)(a1 + 356) = 0;
  *(_DWORD *)(a1 + 360) = 0;
  *(_DWORD *)(a1 + 364) = 0;
  *(_DWORD *)(a1 + 368) = 0;
  return result;
}

// ===== sub_423450 @ 0x00423450..0x0042349E =====
int __usercall sub_423450@<eax>(int a1@<esi>)
{
  unsigned int v1; // edi
  int v2; // ebx
  int result; // eax

  v1 = 0;
  if ( *(_DWORD *)(a1 + 372) )
  {
    v2 = 0;
    do
    {
      operator delete[](*(void **)(v2 + *(_DWORD *)(a1 + 376)));
      ++v1;
      v2 += 24;
    }
    while ( v1 < *(_DWORD *)(a1 + 372) );
  }
  operator delete[](*(void **)(a1 + 376));
  result = 0;
  *(_DWORD *)(a1 + 372) = 0;
  *(_DWORD *)(a1 + 376) = 0;
  return result;
}

// ===== sub_4234A0 @ 0x004234A0..0x0042373C =====
int __stdcall sub_4234A0(
        _DWORD *a1,
        int a2,
        unsigned int a3,
        unsigned int a4,
        unsigned int a5,
        unsigned int a6,
        int a7)
{
  int result; // eax
  int v9; // eax
  int v10; // ecx
  _DWORD *v11; // edi
  int v12; // eax
  int v13; // eax
  int v14; // eax
  int v15; // edx
  int v16; // eax
  int v17; // esi
  _DWORD *v18; // eax
  int v19; // [esp+8h] [ebp-10h]
  int v20; // [esp+Ch] [ebp-Ch]
  int v21; // [esp+10h] [ebp-8h]
  int v22; // [esp+14h] [ebp-4h]
  int *v23; // [esp+20h] [ebp+8h]
  int v24; // [esp+20h] [ebp+8h]
  int v25; // [esp+34h] [ebp+1Ch]
  int v26; // [esp+38h] [ebp+20h]

  if ( !a1[86] )
    return -2147483644;
  if ( !a5 )
    return -2147483643;
  if ( a5 > 0x100 )
    return -2147483643;
  if ( !a6 )
    return -2147483643;
  if ( a6 > 0x100 )
    return -2147483643;
  v9 = (*(int (__thiscall **)(_DWORD *))(*a1 + 88))(a1);
  if ( !sub_423090(a6, (int)a1, v9) )
    return -2147483643;
  if ( a3 >= a6 )
    return -2147483637;
  if ( a4 >= a5 )
    return -2147483636;
  v23 = (int *)(a7 + 4 * (a4 + a5 * a3));
  v10 = *v23;
  if ( (unsigned int)*v23 >= a1[86] && v10 != -1 )
    return -2147483642;
  v11 = (_DWORD *)(*(_DWORD *)(a2 + 12 * a3 + 8) + 68 * a4);
  *v11 = v10;
  v20 = *v23;
  if ( *v23 == -1 )
    return 0;
  v22 = a1[78];
  v24 = 0;
  v12 = sub_423740(a1, a6, a7) - v22;
  if ( v12 > 0 )
  {
    v21 = v12;
    v13 = sub_423740(a1, a6, a7) - v22;
    if ( v13 > 0 )
    {
      v24 = v21;
      if ( v21 > v13 )
        v24 = v13;
    }
  }
  v14 = sub_423740(a1, a6, a7) - 2 * v22;
  if ( v14 > 0 && v24 > v14 )
    v24 = v14;
  v15 = a1[87] + 176 * v20;
  v25 = v15;
  v26 = *(_DWORD *)(v15 + 156) - v24;
  if ( v26 <= 0 )
  {
    v11[5] = 0;
    return 0;
  }
  if ( (a3 & 1) != 0 )
    v16 = a1[80];
  else
    v16 = 0;
  v17 = v16 + *(_DWORD *)(v15 + 136) + a4 * a1[77];
  v19 = a1[81] + a3 * v22 - *(_DWORD *)(v15 + 156);
  v18 = sub_409190(v11 + 1, v15 + 144);
  v11[4] = v26 - 1;
  sub_409170(v19, v17, v18);
  v11[5] = 32 * (a3 + ((a3 * a1[82] + a1[83] * (*(_DWORD *)(v25 + 172) + *(_DWORD *)(v25 + 132) - 1)) << 11))
         + ((*(int (__thiscall **)(_DWORD *))(*a1 + 28))(a1) & 0xFFFFE01F);
  result = 0;
  v11[6] = -1;
  v11[10] = 0;
  v11[7] = -1;
  v11[11] = 0;
  v11[8] = -1;
  v11[12] = 0;
  v11[9] = -1;
  v11[13] = 0;
  v11[14] = 0;
  v11[15] = 0;
  v11[16] = 0;
  return result;
}

// ===== sub_423740 @ 0x00423740..0x0042377A =====
int __userpurge sub_423740@<eax>(
        unsigned int a1@<edx>,
        unsigned int a2@<ecx>,
        unsigned int a3@<esi>,
        int a4,
        unsigned int a5,
        int a6)
{
  int result; // eax
  int v7; // ecx

  result = 0;
  if ( a1 < a2 && a3 < a5 )
  {
    v7 = *(_DWORD *)(a6 + 4 * (a1 + a3 * a2));
    if ( v7 != -1 )
      return *(_DWORD *)(176 * v7 + *(_DWORD *)(a4 + 348) + 140);
  }
  return result;
}

// ===== sub_423780 @ 0x00423780..0x00423848 =====
int __userpurge sub_423780@<eax>(unsigned int a1@<eax>, int a2)
{
  int v2; // edi
  unsigned int v3; // ecx
  _DWORD *v4; // esi
  int v5; // ebx
  _DWORD *v7; // [esp+10h] [ebp-4h]

  v2 = a2;
  v3 = *(_DWORD *)(a2 + 344);
  if ( !v3 )
    return -2147483644;
  if ( a1 >= v3 )
    return -2147483638;
  v4 = (_DWORD *)(176 * a1 + *(_DWORD *)(a2 + 348));
  sub_40A620((int)(v4 + 36), 0);
  v5 = 0;
  if ( *v4 )
  {
    v7 = v4 + 1;
    while ( 1 )
    {
      sub_40A530(
        (int *)(*(_DWORD *)(v2 + 340) + 36 * *v7),
        v4 + 36,
        v4[39] - v5 * *(_DWORD *)(v2 + 316) - *(_DWORD *)(*(_DWORD *)(v2 + 340) + 36 * *v7 + 12),
        (unsigned int)(v4[38] - *(_DWORD *)(*(_DWORD *)(v2 + 340) + 36 * *v7 + 8)) >> 1,
        1,
        0);
      ++v7;
      if ( (unsigned int)++v5 >= *v4 )
        break;
      v2 = a2;
    }
  }
  return 0;
}

// ===== sub_423850 @ 0x00423850..0x0042390B =====
int __userpurge sub_423850@<eax>(unsigned int a1@<eax>, unsigned int a2@<edx>, _DWORD *a3)
{
  int v3; // esi
  int v5; // edi
  int v6; // eax
  int v7; // edx
  _DWORD *v8; // esi
  int v9; // eax
  int v10; // edx
  int v11; // eax
  void (__stdcall *v12)(int *); // edx
  int v13[2]; // [esp+8h] [ebp-18h] BYREF
  _DWORD v14[4]; // [esp+10h] [ebp-10h] BYREF

  v3 = a3[90];
  if ( !v3 )
    return -2147483635;
  if ( a2 >= a3[89] )
    return -2147483637;
  if ( a1 >= a3[88] )
    return -2147483636;
  v5 = 17 * a1;
  v6 = *(_DWORD *)(v3 + 12 * a2 + 8);
  v7 = *(_DWORD *)(v6 + 4 * v5 + 4);
  v8 = (_DWORD *)(v6 + 4 * v5);
  v9 = v8[2];
  v14[0] = v7;
  v10 = v8[3];
  v14[1] = v9;
  v11 = v8[4];
  v14[2] = v10;
  v12 = *(void (__stdcall **)(int *))(*a3 + 52);
  v14[3] = v11;
  v12(v13);
  sub_409170(v13[1], v13[0], v14);
  sub_443240(dword_565B2C);
  return 0;
}

// ===== CompareFunction @ 0x00423910..0x0042391F =====
int __cdecl CompareFunction(_DWORD *a1, _DWORD *a2)
{
  return *a1 - *a2;
}

// ===== sub_423920 @ 0x00423920..0x0042398D =====
_DWORD *__thiscall sub_423920(void *this, _DWORD *a2)
{
  sub_41A400((int)this, a2, 1, 1);
  *a2 = &CDspObjMap::`vftable';
  a2[77] = 0;
  a2[92] = 0;
  a2[96] = 0;
  a2[106] = 0;
  return a2;
}

// ===== sub_423990 @ 0x00423990..0x004239B2 =====
void *__thiscall sub_423990(void *this, char a2)
{
  sub_4239C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4239C0 @ 0x004239C0..0x00423A22 =====
int __stdcall sub_4239C0(_DWORD *a1)
{
  *a1 = &CDspObjMap::`vftable';
  sub_4242B0();
  sub_4242F0();
  sub_424350();
  return sub_41A5E0(a1);
}

// ===== sub_423A30 @ 0x00423A30..0x00423AEA =====
void __thiscall sub_423A30(int *this, int a2, int *a3, int a4)
{
  int v5; // ecx
  int v6; // edx
  int v7; // ecx
  int v8; // edx
  unsigned int v9; // eax
  unsigned int v10; // [esp-8h] [ebp-28h]
  _DWORD v11[6]; // [esp+8h] [ebp-18h] BYREF

  if ( this[96] && this[106] )
  {
    if ( this[105] )
      sub_423FF0(this);
    v5 = this[85];
    v6 = this[86];
    v11[0] = this[84];
    v11[3] = this[87];
    v11[1] = v5;
    v7 = this[88];
    v11[2] = v6;
    v8 = this[89];
    v11[4] = v7;
    v11[5] = v8;
    sub_4091B0(v11, this + 101);
    sub_4091B0(v11, a3);
    v10 = sub_41B770(this);
    v9 = sub_41B610((int)this);
    sub_40A9E0(a2, (int)v11, v9, v10, 1);
  }
}

// ===== sub_423AF0 @ 0x00423AF0..0x00423B40 =====
int __userpurge sub_423AF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  int result; // eax
  int v9; // esi

  result = sub_424140(a1);
  v9 = result;
  if ( result )
  {
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a2 + 44))(a2, a3, a4);
    sub_41B600(a2, a5);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2 + 72))(a2, a6);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2 + 84))(a2, a7);
    return v9;
  }
  return result;
}

// ===== sub_423B40 @ 0x00423B40..0x00423CD2 =====
int __userpurge sub_423B40@<eax>(int a1@<edi>, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
  void *v5; // eax
  void *v6; // ecx

  if ( a4 - 1 > 0xFF || a5 - 1 > 0xFF )
    return -2147483645;
  if ( !a2
    || a2 > 0x400 / a4
    || !a3
    || a3 > 0x300 / a5
    || !(*(int (__thiscall **)(int, unsigned int, unsigned int))(*(_DWORD *)a1 + 116))(a1, a4 * a2, a5 * a3) )
  {
    return -2147483646;
  }
  sub_4242F0();
  sub_4242B0();
  sub_424390();
  *(_DWORD *)(a1 + 312) = a2;
  *(_DWORD *)(a1 + 316) = a3;
  *(_DWORD *)(a1 + 320) = a2 + 1;
  *(_DWORD *)(a1 + 324) = a3 + 1;
  *(_DWORD *)(a1 + 328) = a4;
  *(_DWORD *)(a1 + 332) = a5;
  sub_409080((_DWORD *)(a1 + 336), 1);
  sub_40A620(a1 + 336, 0);
  *(_DWORD *)(a1 + 360) = operator new[](2 * *(_DWORD *)(a1 + 324) * *(_DWORD *)(a1 + 320));
  v5 = operator new[](2 * *(_DWORD *)(a1 + 324) * *(_DWORD *)(a1 + 320));
  v6 = *(void **)(a1 + 360);
  *(_DWORD *)(a1 + 364) = v5;
  memset(v6, 255, 2 * *(_DWORD *)(a1 + 324) * *(_DWORD *)(a1 + 320));
  memset(*(void **)(a1 + 364), 0, 2 * *(_DWORD *)(a1 + 324) * *(_DWORD *)(a1 + 320));
  *(_DWORD *)(a1 + 308) = 1;
  return 0;
}

// ===== sub_423CE0 @ 0x00423CE0..0x00423D62 =====
int __userpurge sub_423CE0@<eax>(_DWORD *a1@<eax>, int a2, int a3, void *Src)
{
  int result; // eax
  void *v6; // eax

  result = 0;
  if ( a2 )
  {
    if ( a3 )
    {
      sub_424350();
      sub_424390();
      sub_423F90();
      a1[94] = a3;
      a1[92] = 1;
      a1[93] = a2;
      v6 = operator new[](2 * a3 * a2);
      a1[95] = v6;
      memcpy_0(v6, Src, 2 * a3 * a2);
      return 1;
    }
  }
  return result;
}

// ===== sub_423D70 @ 0x00423D70..0x00423F07 =====
int __userpurge sub_423D70@<eax>(
        unsigned int a1@<eax>,
        int a2@<esi>,
        unsigned int a3,
        unsigned int a4,
        unsigned int a5,
        int a6)
{
  int result; // eax
  unsigned int v8; // edx
  unsigned int v9; // ecx
  int v10; // ebx
  unsigned int v11; // ecx
  unsigned int j; // edx
  unsigned int v13; // eax
  int v14; // eax
  int v15; // edx
  int v16; // eax
  int v17; // ecx
  unsigned int i; // [esp+4h] [ebp-8h]
  int v19; // [esp+8h] [ebp-4h]

  result = 0;
  if ( *(_DWORD *)(a2 + 308) )
  {
    if ( *(_DWORD *)(a2 + 368) )
    {
      if ( a3 < *(_DWORD *)(a2 + 372) && a1 < *(_DWORD *)(a2 + 376) )
      {
        v8 = a4;
        if ( a4 < *(_DWORD *)(a2 + 328) )
        {
          v9 = a5;
          if ( a5 < *(_DWORD *)(a2 + 332) )
          {
            if ( a3 != *(_DWORD *)(a2 + 388) || a1 != *(_DWORD *)(a2 + 392) )
            {
              *(_DWORD *)(a2 + 388) = a3;
              *(_DWORD *)(a2 + 392) = a1;
              if ( !a6 )
                memset(*(void **)(a2 + 364), 255, 2 * *(_DWORD *)(a2 + 320) * *(_DWORD *)(a2 + 324));
              v10 = *(_DWORD *)(a2 + 364);
              v19 = v10;
              for ( i = 0; i < *(_DWORD *)(a2 + 324); ++i )
              {
                if ( a1 >= *(_DWORD *)(a2 + 376) )
                {
                  if ( !a6 )
                    break;
                  a1 = 0;
                }
                v11 = a3;
                for ( j = 0; j < *(_DWORD *)(a2 + 320); ++v11 )
                {
                  v13 = *(_DWORD *)(a2 + 372);
                  if ( v11 >= v13 )
                  {
                    if ( !a6 )
                      break;
                    v11 = 0;
                  }
                  v10 = v19;
                  *(_WORD *)(v19 + 2 * j++) = *(_WORD *)(*(_DWORD *)(a2 + 380) + 2 * (v11 + a1 * v13));
                }
                v10 += 2 * *(_DWORD *)(a2 + 320);
                ++a1;
                v19 = v10;
              }
              v9 = a5;
              v8 = a4;
              *(_DWORD *)(a2 + 420) = 1;
            }
            v14 = *(_DWORD *)(a2 + 328) * *(_DWORD *)(a2 + 312);
            *(_DWORD *)(a2 + 396) = v8;
            *(_DWORD *)(a2 + 404) = v8;
            v15 = v14 + v8 - 1;
            v16 = *(_DWORD *)(a2 + 332) * *(_DWORD *)(a2 + 316);
            *(_DWORD *)(a2 + 400) = v9;
            *(_DWORD *)(a2 + 408) = v9;
            v17 = v16 + v9 - 1;
            result = 1;
            *(_DWORD *)(a2 + 412) = v15;
            *(_DWORD *)(a2 + 416) = v17;
            *(_DWORD *)(a2 + 384) = 1;
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_423F10 @ 0x00423F10..0x00423F82 =====
int __thiscall sub_423F10(_DWORD *this, int a2)
{
  int result; // eax
  _WORD *v3; // edx
  _WORD *v4; // esi
  unsigned int i; // edi
  unsigned int j; // eax

  result = 0;
  if ( this[77] )
  {
    v3 = (_WORD *)this[90];
    v4 = (_WORD *)this[91];
    for ( i = 0; i < this[81]; ++i )
    {
      for ( j = 0; j < this[80]; ++v4 )
      {
        if ( (unsigned __int16)*v3 == a2 )
          *v3 = ~*v4++;
        ++j;
        ++v3;
      }
    }
    this[105] = 1;
    return 1;
  }
  return result;
}

// ===== sub_423F90 @ 0x00423F90..0x00423FEB =====
int __usercall sub_423F90@<eax>(_DWORD *a1@<edi>)
{
  int result; // eax
  _WORD *v2; // edx
  unsigned int v3; // ebx
  _WORD *v4; // esi
  unsigned int v5; // ecx
  unsigned int i; // eax

  result = 0;
  if ( a1[77] )
  {
    v2 = (_WORD *)a1[90];
    v3 = 0;
    v4 = (_WORD *)a1[91];
    if ( a1[81] )
    {
      v5 = a1[80];
      do
      {
        for ( i = 0; i < v5; ++v4 )
        {
          *v2 = ~*v4;
          v5 = a1[80];
          ++i;
          ++v2;
        }
        ++v3;
      }
      while ( v3 < a1[81] );
    }
    return 1;
  }
  return result;
}

// ===== sub_423FF0 @ 0x00423FF0..0x00424139 =====
int __stdcall sub_423FF0(int a1)
{
  int v2; // edi
  int v3; // esi
  unsigned int v4; // eax
  _DWORD v6[6]; // [esp+Ch] [ebp-28h] BYREF
  unsigned int i; // [esp+24h] [ebp-10h]
  unsigned int j; // [esp+28h] [ebp-Ch]
  _WORD *v9; // [esp+2Ch] [ebp-8h]
  _WORD *v10; // [esp+3Ch] [ebp+8h]

  v2 = 0;
  if ( *(_DWORD *)(a1 + 384) && *(_DWORD *)(a1 + 424) )
  {
    if ( sub_407F20(dword_565B30, *(_DWORD *)(a1 + 428), v6)
      && *(_DWORD *)(a1 + 432) == sub_408300(*(_DWORD *)(a1 + 428), dword_565B30) )
    {
      v10 = *(_WORD **)(a1 + 360);
      v9 = *(_WORD **)(a1 + 364);
      for ( i = 0; i < *(_DWORD *)(a1 + 324); ++i )
      {
        v3 = 0;
        for ( j = 0; j < *(_DWORD *)(a1 + 320); ++j )
        {
          v4 = (unsigned __int16)*v9;
          if ( *v10 != (_WORD)v4 )
          {
            if ( (_WORD)v4 == 0xFFFF || v4 >= *(_DWORD *)(a1 + 436) )
              sub_40A530(*(int **)(a1 + 440), (_DWORD *)(a1 + 336), v2, v3, 65, 0);
            else
              sub_40A530((int *)(*(_DWORD *)(a1 + 440) + 24 * v4), (_DWORD *)(a1 + 336), v2, v3, 128, 0);
            *v10 = *v9;
          }
          v3 += *(_DWORD *)(a1 + 328);
          ++v10;
          ++v9;
        }
        v2 += *(_DWORD *)(a1 + 332);
      }
      v2 = 1;
    }
    else
    {
      sub_40A620(a1 + 336, 0);
    }
    *(_DWORD *)(a1 + 420) = 0;
  }
  return v2;
}
