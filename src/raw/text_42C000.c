#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_42C060 @ 0x0042C060..0x0042C090 =====
int __fastcall sub_42C060(unsigned int a1, int a2, int a3)
{
  int result; // eax
  int v4; // edx
  bool v5; // zf
  _DWORD **v6; // ecx

  result = 9;
  if ( a1 < *(_DWORD *)(a2 + 784) )
  {
    v4 = *(_DWORD *)(a2 + 788);
    v5 = *(_DWORD *)(v4 + 4 * a1) == 0;
    v6 = (_DWORD **)(v4 + 4 * a1);
    if ( !v5 )
    {
      sub_41AD60(*v6, a3);
      return 0;
    }
  }
  return result;
}

// ===== sub_42C090 @ 0x0042C090..0x0042C0C4 =====
int __fastcall sub_42C090(unsigned int a1, int a2, int a3)
{
  int result; // eax
  int v4; // edx
  bool v5; // zf
  int *v6; // edx

  result = 9;
  if ( a1 < *(_DWORD *)(a2 + 784) )
  {
    v4 = *(_DWORD *)(a2 + 788);
    v5 = *(_DWORD *)(v4 + 4 * a1) == 0;
    v6 = (int *)(v4 + 4 * a1);
    if ( !v5 )
      return sub_4272F0(*v6, a3) != 0 ? 2 : 0;
  }
  return result;
}

// ===== sub_42C0D0 @ 0x0042C0D0..0x0042C137 =====
int __userpurge sub_42C0D0@<eax>(unsigned int a1@<eax>, int a2@<edx>, _DWORD *a3, int a4, int a5)
{
  int result; // eax
  int v7; // ecx

  result = 9;
  if ( a1 < a3[196] )
  {
    v7 = a3[197];
    if ( *(_DWORD *)(v7 + 4 * a1) )
    {
      (*(void (__thiscall **)(_DWORD, int, int, int))(**(_DWORD **)(v7 + 4 * a1) + 60))(
        *(_DWORD *)(v7 + 4 * a1),
        (a4 + a3[209]) << 16,
        (a5 + a3[210]) << 16,
        a2 << 16);
      sub_4307D0();
      return 0;
    }
  }
  return result;
}

// ===== sub_42C140 @ 0x0042C140..0x0042C16F =====
int __fastcall sub_42C140(unsigned int a1, int a2, int a3)
{
  int result; // eax
  int v4; // edx
  bool v5; // zf
  _DWORD **v6; // ecx

  result = 9;
  if ( a1 < *(_DWORD *)(a2 + 784) )
  {
    v4 = *(_DWORD *)(a2 + 788);
    v5 = *(_DWORD *)(v4 + 4 * a1) == 0;
    v6 = (_DWORD **)(v4 + 4 * a1);
    if ( !v5 )
    {
      sub_428060(*v6, a3);
      return 0;
    }
  }
  return result;
}

// ===== sub_42C170 @ 0x0042C170..0x0042C1BC =====
int __userpurge sub_42C170@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3)
{
  int result; // eax
  int v5; // ecx

  result = 9;
  if ( a1 < *(_DWORD *)(a2 + 784) )
  {
    v5 = *(_DWORD *)(a2 + 788);
    if ( *(_DWORD *)(v5 + 4 * a1) )
    {
      (*(void (__thiscall **)(_DWORD, int))(**(_DWORD **)(v5 + 4 * a1) + 84))(*(_DWORD *)(v5 + 4 * a1), a3);
      sub_4307D0();
      return 0;
    }
  }
  return result;
}

// ===== sub_42C1C0 @ 0x0042C1C0..0x0042C212 =====
int __usercall sub_42C1C0@<eax>(unsigned int a1@<ecx>, _DWORD *a2@<edi>, _DWORD *a3@<esi>)
{
  int result; // eax
  int v4; // edx
  int v5[2]; // [esp+0h] [ebp-8h] BYREF

  result = 0;
  if ( a1 < a3[196] )
  {
    v4 = a3[197];
    if ( *(_DWORD *)(v4 + 4 * a1) )
    {
      (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(v4 + 4 * a1) + 36))(*(_DWORD *)(v4 + 4 * a1), a2);
      sub_42CA30(a3, a2);
      (*(void (__thiscall **)(_DWORD *, int *))(*a3 + 52))(a3, v5);
      sub_409170(v5[1], v5[0], a2);
      return 1;
    }
  }
  return result;
}

// ===== sub_42C220 @ 0x0042C220..0x0042C271 =====
int __userpurge sub_42C220@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<edi>, unsigned int a3)
{
  int result; // eax
  int v5; // ecx
  int v6; // ecx

  result = 0;
  if ( a3 < a1[196] )
  {
    v5 = a1[197];
    if ( *(_DWORD *)(v5 + 4 * a3) )
    {
      (*(void (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(v5 + 4 * a3) + 4))(*(_DWORD *)(v5 + 4 * a3), 0);
      sub_42C1C0(a3, a2, a1);
      v6 = *(_DWORD *)(a1[197] + 4 * a3);
      (*(void (__thiscall **)(int, int))(*(_DWORD *)v6 + 4))(v6, 1);
      return 1;
    }
  }
  return result;
}

// ===== sub_42C280 @ 0x0042C280..0x0042C295 =====
int sub_42C280()
{
  if ( sub_42C540() )
    return sub_42C460();
  else
    return 0;
}

// ===== sub_42C2A0 @ 0x0042C2A0..0x0042C2C4 =====
_DWORD *__usercall sub_42C2A0@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  *result = a2[104];
  result[1] = a2[105];
  result[2] = a2[106];
  result[3] = a2[107];
  return result;
}

// ===== sub_42C2D0 @ 0x0042C2D0..0x0042C2E4 =====
int __usercall sub_42C2D0@<eax>(_DWORD *a1@<edi>, _DWORD *a2@<esi>)
{
  int result; // eax

  sub_42C2A0(a2, a1);
  result = sub_42C280();
  a2[2] += result;
  return result;
}

// ===== sub_42C2F0 @ 0x0042C2F0..0x0042C323 =====
_DWORD *__usercall sub_42C2F0@<eax>(_DWORD *a1@<edi>, int a2@<esi>)
{
  int v3[2]; // [esp+0h] [ebp-8h] BYREF

  (*(void (__thiscall **)(int, int *))(*(_DWORD *)a2 + 52))(a2, v3);
  sub_409190(a1, a2 + 388);
  return sub_409170(v3[1], v3[0], a1);
}

// ===== sub_42C330 @ 0x0042C330..0x0042C3A1 =====
int __userpurge sub_42C330@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3)
{
  _DWORD *v3; // esi
  _DWORD *v4; // edi
  _DWORD v6[3]; // [esp+10h] [ebp-Ch] BYREF

  v3 = (_DWORD *)(a2 + 44 * a1);
  if ( !v3[108] )
    return 0;
  v4 = (_DWORD *)(a2 + 44 * (a1 + 10));
  if ( !*v4 )
    return 0;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)a2 + 52))(a2, v6);
  sub_409190(a3, (int)v4);
  sub_409170(v6[1] + v3[117], v6[0] + v3[116], a3);
  return 1;
}

// ===== sub_42C3B0 @ 0x0042C3B0..0x0042C449 =====
int __userpurge sub_42C3B0@<eax>(int a1@<ecx>, const CHAR *a2@<eax>, _DWORD *a3@<edi>, int a4, int a5)
{
  int result; // eax
  int v7; // esi
  unsigned int v8[2]; // [esp+8h] [ebp-20h] BYREF
  int v9; // [esp+10h] [ebp-18h]
  int v10; // [esp+18h] [ebp-10h] BYREF
  int v11; // [esp+20h] [ebp-8h]

  result = sub_409290(a4, a5, dword_565B30, (int)(a3 + 212), a2, a1);
  if ( !result )
  {
    a3[214] = a1;
    v7 = a4 * a1 / 100;
    a3[215] = v7;
    if ( sub_42C540() )
    {
      sub_42C2A0(v8, a3);
      sub_409190(&v10, (int)(a3 + 97));
      if ( v9 > v11 - v7 )
      {
        v9 = v11 - v7;
        sub_42B900(a3, v8);
      }
    }
    return 0;
  }
  return result;
}

// ===== sub_42C450 @ 0x0042C450..0x0042C457 =====
int __usercall sub_42C450@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 848);
}

// ===== sub_42C460 @ 0x0042C460..0x0042C467 =====
int __usercall sub_42C460@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 856);
}

// ===== sub_42C470 @ 0x0042C470..0x0042C487 =====
int __fastcall sub_42C470(unsigned int a1, int a2)
{
  int result; // eax

  if ( a1 > 0x320 )
    return 0;
  result = 1;
  *(_DWORD *)(a2 + 864) = a1;
  return result;
}

// ===== sub_42C490 @ 0x0042C490..0x0042C497 =====
int __usercall sub_42C490@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 864);
}

// ===== sub_42C4A0 @ 0x0042C4A0..0x0042C4B5 =====
int __thiscall sub_42C4A0(void *this)
{
  int v1; // ecx
  int v2; // eax
  int v4; // [esp-4h] [ebp-4h]

  v4 = sub_42C490((int)this);
  v2 = sub_42C460(v1);
  return sub_4097B0(v2, v4);
}

// ===== sub_42C4C0 @ 0x0042C4C0..0x0042C4D3 =====
int __usercall sub_42C4C0@<eax>(void *a1@<esi>)
{
  int v1; // eax
  int v2; // ecx

  sub_42C4A0(a1);
  v1 = sub_42C460((int)a1);
  return v2 + v1;
}

// ===== sub_42C4E0 @ 0x0042C4E0..0x0042C509 =====
int __usercall sub_42C4E0@<eax>(_DWORD *a1@<eax>)
{
  int v2; // eax
  _DWORD v4[4]; // [esp+8h] [ebp-10h] BYREF

  sub_42C2A0(v4, a1);
  v2 = sub_42C4C0(a1);
  return (v4[3] - v4[1] + 1) / v2;
}

// ===== sub_42C510 @ 0x0042C510..0x0042C517 =====
int __usercall sub_42C510@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 852) = a2;
  return result;
}

// ===== sub_42C520 @ 0x0042C520..0x0042C527 =====
int __usercall sub_42C520@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 852);
}

// ===== sub_42C530 @ 0x0042C530..0x0042C537 =====
int __usercall sub_42C530@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 868) = a2;
  return result;
}

// ===== sub_42C540 @ 0x0042C540..0x0042C547 =====
int __usercall sub_42C540@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 868);
}

// ===== sub_42C550 @ 0x0042C550..0x0042C56C =====
int __usercall sub_42C550@<eax>(int a1@<eax>, unsigned int a2@<ecx>)
{
  int result; // eax

  result = 0;
  if ( a2 <= 1 )
  {
    *(_DWORD *)(a1 + 880) = a2;
    sub_42C5B0();
    return 1;
  }
  return result;
}

// ===== sub_42C570 @ 0x0042C570..0x0042C577 =====
int __usercall sub_42C570@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 880);
}

// ===== sub_42C580 @ 0x0042C580..0x0042C59C =====
int __fastcall sub_42C580(unsigned int a1, int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( a1 <= 2 )
  {
    *(_DWORD *)(a3 + 884) = a1;
    return 1;
  }
  return result;
}

// ===== sub_42C5A0 @ 0x0042C5A0..0x0042C5A7 =====
int __usercall sub_42C5A0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 884);
}

// ===== sub_42C5B0 @ 0x0042C5B0..0x0042C5FB =====
int __usercall sub_42C5B0@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax
  int result; // eax
  int v3; // ecx
  int v4; // [esp+0h] [ebp-10h] BYREF
  int v5; // [esp+4h] [ebp-Ch]
  int v6; // [esp+8h] [ebp-8h]

  sub_42C2A0(&v4, a1);
  v1 = sub_42C570((int)a1);
  if ( v1 )
  {
    result = v1 - 1;
    if ( !result )
    {
      result = v6;
      v3 = v5;
      a1[218] = v6;
      a1[219] = v3;
    }
  }
  else
  {
    result = v5;
    a1[218] = v4;
    a1[219] = result;
  }
  return result;
}

// ===== sub_42C600 @ 0x0042C600..0x0042C67C =====
int __usercall sub_42C600@<eax>(_DWORD *a1@<eax>)
{
  int v2; // eax
  int v3; // ecx
  int v4; // eax
  int v6; // eax
  int v7; // ebx
  _DWORD v8[4]; // [esp+10h] [ebp-10h] BYREF

  sub_42C2A0(v8, a1);
  sub_42C4C0(a1);
  v2 = sub_42C570((int)a1);
  if ( v2 )
  {
    if ( v2 == 1 )
    {
      v4 = v8[1];
      a1[218] -= v3;
      a1[219] = v4;
      return 1;
    }
  }
  else
  {
    v6 = a1[219];
    v7 = v8[3];
    a1[218] = v8[0];
    if ( v6 + 2 * v3 <= v7 + 1 )
    {
      a1[219] = v3 + v6;
      return 1;
    }
  }
  return 0;
}

// ===== sub_42C680 @ 0x0042C680..0x0042C6D8 =====
int __usercall sub_42C680@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // eax
  _DWORD v4[5]; // [esp+4h] [ebp-14h] BYREF

  sub_42C2A0(v4, a2);
  v2 = sub_42C570((int)a2);
  if ( !v2 )
    return a1 + a2[218] <= v4[2] + 1;
  if ( v2 == 1 )
    return a1 + a2[219] <= v4[3] + 1;
  return v4[4];
}

// ===== sub_42C6E0 @ 0x0042C6E0..0x0042C6FD =====
int __thiscall sub_42C6E0(void *this)
{
  int result; // eax
  int v2; // edx
  int v3; // ecx

  result = sub_42C570((int)this);
  if ( result )
  {
    if ( !--result )
      *(_DWORD *)(v3 + 876) += v2;
  }
  else
  {
    *(_DWORD *)(v3 + 872) += v2;
  }
  return result;
}

// ===== sub_42C700 @ 0x0042C700..0x0042C716 =====
int __userpurge sub_42C700@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 872) = a2;
  *(_DWORD *)(result + 876) = a3;
  return result;
}

// ===== sub_42C720 @ 0x0042C720..0x0042C732 =====
_DWORD *__usercall sub_42C720@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  *result = *(_DWORD *)(a2 + 872);
  result[1] = *(_DWORD *)(a2 + 876);
  return result;
}

// ===== sub_42C740 @ 0x0042C740..0x0042C794 =====
BOOL __usercall sub_42C740@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax
  _DWORD v5[4]; // [esp+0h] [ebp-18h] BYREF
  _DWORD v6[2]; // [esp+10h] [ebp-8h] BYREF

  sub_42C720(v6, (int)a1);
  sub_42C2A0(v5, a1);
  v1 = sub_42C570((int)a1);
  v3 = v1 - v2;
  if ( !v3 )
    return v6[0] == v5[0] + sub_4344D0();
  if ( v3 == 1 )
    LOBYTE(v2) = v6[1] == v5[1] + sub_4344D0();
  return v2;
}

// ===== sub_42C7A0 @ 0x0042C7A0..0x0042C7D0 =====
int __fastcall sub_42C7A0(int a1, int a2)
{
  int result; // eax
  _DWORD *v3; // ecx

  result = a2 != 0;
  *(_DWORD *)(a1 + 888) = result;
  if ( a2 )
  {
    result = 0;
    v3 = (_DWORD *)(a1 + 892);
    do
      *v3++ = *(_DWORD *)(a2 + 4 * result++);
    while ( result < 16 );
  }
  return result;
}

// ===== sub_42C7D0 @ 0x0042C7D0..0x0042C870 =====
int __fastcall sub_42C7D0(_DWORD *a1, _DWORD *a2)
{
  int result; // eax
  int v3; // esi
  int v4; // ecx

  result = a1[222];
  if ( result )
  {
    *a2 = a1[223];
    a2[1] = a1[224];
    a2[2] = a1[225];
    a2[3] = a1[226];
    a2[4] = a1[227];
    a2[5] = a1[228];
    a2[6] = a1[229];
    a2[7] = a1[230];
    a2[8] = a1[231];
    a2[9] = a1[232];
    a2[10] = a1[233];
    a2[11] = a1[234];
    a2[12] = a1[235];
    a2[13] = a1[236];
    v3 = a1[237];
    v4 = a1[238];
    a2[14] = v3;
    a2[15] = v4;
  }
  return result;
}

// ===== sub_42C870 @ 0x0042C870..0x0042C927 =====
int __fastcall sub_42C870(_DWORD *a1, int a2)
{
  int result; // eax

  result = 1;
  switch ( a2 )
  {
    case 0:
      a1[240] = 0;
      a1[242] = 2;
      a1[241] = 1;
      break;
    case 1:
      a1[240] = 0;
      a1[241] = 2;
      a1[242] = 1;
      break;
    case 2:
      a1[240] = 1;
      a1[241] = 0;
      a1[242] = 2;
      break;
    case 3:
      a1[240] = 1;
      a1[241] = 2;
      a1[242] = 0;
      break;
    case 4:
      a1[240] = 2;
      a1[241] = 0;
      a1[242] = 1;
      break;
    case 5:
      a1[240] = 2;
      a1[242] = 0;
      a1[241] = 1;
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_42C940 @ 0x0042C940..0x0042CA00 =====
int __usercall sub_42C940@<eax>(size_t *a1@<eax>, _DWORD *a2@<edi>)
{
  size_t v2; // edx
  size_t v3; // ecx
  size_t v4; // edx
  size_t v5; // ecx
  size_t v6; // edx
  int v7; // eax
  int v8; // ecx
  int v9; // edx
  int v10; // ecx
  int v11; // eax
  int v12; // edx
  int *v13; // eax
  size_t v15[6]; // [esp+8h] [ebp-50h] BYREF
  _DWORD v16[6]; // [esp+20h] [ebp-38h] BYREF
  int v17[4]; // [esp+38h] [ebp-20h] BYREF
  int v18[4]; // [esp+48h] [ebp-10h] BYREF

  if ( a2[79] )
  {
    v2 = a1[1];
    v15[0] = *a1;
    v3 = a1[2];
    v15[1] = v2;
    v4 = a1[3];
    v15[2] = v3;
    v5 = a1[4];
    v15[3] = v4;
    v6 = a1[5];
    v7 = a2[81];
    v15[4] = v5;
    v8 = a2[82];
    v15[5] = v6;
    v9 = a2[83];
    v16[1] = v8;
    v10 = a2[85];
    v16[0] = v7;
    v11 = a2[84];
    v16[2] = v9;
    v12 = a2[86];
    v16[4] = v10;
    v16[3] = v11;
    v16[5] = v12;
    sub_409190(v17, (int)v15);
    v13 = sub_409190(v18, (int)v16);
    sub_409110(v17, v13);
    sub_4091B0(v15, v18);
    sub_4091B0(v16, v18);
    sub_40AF50((size_t)v16, v15);
  }
  return a2[79];
}

// ===== sub_42CA00 @ 0x0042CA00..0x0042CA22 =====
int __usercall sub_42CA00@<eax>(int a1@<esi>)
{
  _DWORD *v1; // eax
  _DWORD v3[4]; // [esp+0h] [ebp-10h] BYREF

  v1 = sub_409190(v3, a1 + 324);
  return sub_42CA30(a1, v1);
}

// ===== sub_42CA30 @ 0x0042CA30..0x0042CD23 =====
int __stdcall sub_42CA30(_DWORD *a1, int *a2)
{
  int v3; // edx
  int v4; // ecx
  int v5; // edx
  int *v6; // eax
  int v7; // ecx
  int v8; // edx
  int v9; // ecx
  int v10; // edx
  void *v11; // esi
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  int v15; // ecx
  int *v16; // esi
  int *v17; // esi
  int v18; // ecx
  int v19; // edx
  int v20; // ecx
  int v21; // edx
  void *v23[6]; // [esp+10h] [ebp-78h] BYREF
  _DWORD v24[6]; // [esp+28h] [ebp-60h] BYREF
  _DWORD v25[6]; // [esp+40h] [ebp-48h] BYREF
  _DWORD v26[2]; // [esp+58h] [ebp-30h] BYREF
  _DWORD v27[4]; // [esp+60h] [ebp-28h] BYREF
  int v28; // [esp+70h] [ebp-18h] BYREF
  int v29; // [esp+74h] [ebp-14h]
  int v30; // [esp+78h] [ebp-10h]
  int v31; // [esp+7Ch] [ebp-Ch]
  int v32; // [esp+80h] [ebp-8h]
  int v33; // [esp+84h] [ebp-4h]
  _DWORD *v34; // [esp+90h] [ebp+8h]

  if ( a1[79] )
  {
    v3 = a2[1];
    v28 = *a2;
    v4 = a2[2];
    v29 = v3;
    v5 = a2[3];
    v30 = v4;
    v31 = v5;
    v6 = sub_409190(v27, (int)(a1 + 81));
    if ( sub_409110(&v28, v6) )
    {
      v7 = a1[82];
      v8 = a1[83];
      v26[0] = a1[81];
      v27[1] = a1[84];
      v26[1] = v7;
      v9 = a1[85];
      v27[0] = v8;
      v10 = a1[86];
      v27[2] = v9;
      v27[3] = v10;
      sub_4091B0(v26, &v28);
      v34 = a1 + 240;
      v32 = 3;
      do
      {
        if ( *v34 )
        {
          if ( *v34 == 1 )
          {
            if ( a1[95] )
            {
              v12 = a1[98];
              v13 = a1[99];
              v25[0] = a1[97];
              v25[3] = a1[100];
              v25[1] = v12;
              v14 = a1[101];
              v25[2] = v13;
              v15 = a1[102];
              v25[4] = v14;
              v25[5] = v15;
              sub_4091B0(v25, &v28);
              if ( a1[239] )
              {
                sub_409080(v23, 1);
                sub_40A9E0((int)v23, (int)v25, 0x80u, 0, 1);
                v16 = a1 + 110;
                v33 = 8;
                do
                {
                  if ( *(v16 - 2) && *v16 )
                    sub_40A530(v16, v23, v16[7] - v29, v16[6] - v28, 64, 1);
                  v16 += 11;
                  --v33;
                }
                while ( v33 );
                sub_40A9E0((int)v26, (int)v23, 1u, a1[103], 1);
                operator delete[](v23[0]);
              }
              else
              {
                sub_40A9E0((int)v26, (int)v25, 1u, a1[103], 1);
              }
            }
            v17 = a1 + 110;
            v33 = 8;
            do
            {
              if ( *(v17 - 2) && *v17 )
                sub_40A530(v17, v26, v17[7] - v29, v17[6] - v28, 1, v17[8]);
              v17 += 11;
              --v33;
            }
            while ( v33 );
          }
          else if ( *v34 == 2 && a1[208] )
          {
            v11 = operator new[](16 * a1[196]);
            sub_430830(0, a2);
            sub_430EF0(a1[208], v11);
            operator delete[](v11);
          }
        }
        else if ( a1[87] )
        {
          v18 = a1[90];
          v19 = a1[91];
          v24[0] = a1[89];
          v24[3] = a1[92];
          v24[1] = v18;
          v20 = a1[93];
          v24[2] = v19;
          v21 = a1[94];
          v24[4] = v20;
          v24[5] = v21;
          sub_4091B0(v24, &v28);
          sub_40A9E0((int)v26, (int)v24, 0x80u, 0, 1);
        }
        else
        {
          sub_40A620((int)v26, 0);
        }
        ++v34;
        --v32;
      }
      while ( v32 );
    }
  }
  return a1[79];
}

// ===== sub_42CD30 @ 0x0042CD30..0x0042CD63 =====
int __thiscall sub_42CD30(void *this, _DWORD *a2, int a3, int a4)
{
  _DWORD *v4; // eax
  int v6[4]; // [esp+0h] [ebp-10h] BYREF

  v4 = sub_409190(v6, (int)this);
  sub_409170(a4, a3, v4);
  return sub_42CA30(a2, v6);
}

// ===== sub_42CD70 @ 0x0042CD70..0x0042CDCF =====
BOOL __userpurge sub_42CD70@<eax>(_DWORD *a1@<eax>, int a2@<ecx>, int a3)
{
  BOOL v3; // edi
  int v4; // ebx
  _DWORD *v5; // esi
  _DWORD *v6; // ecx

  v3 = a2 < 8;
  if ( a2 < 8 )
  {
    v4 = a1[11 * a2 + 108];
    v5 = &a1[11 * a2];
    if ( v4 )
    {
      v6 = &a1[11 * a2 + 110];
      if ( *v6 )
      {
        v5[108] = a3;
        sub_42CD30(v6, a1, v5[116], v5[117]);
        v5[108] = v4;
      }
    }
  }
  return v3;
}

// ===== sub_42CDD0 @ 0x0042CDD0..0x0042CE34 =====
int __userpurge sub_42CDD0@<eax>(int result@<eax>, char *a2@<ecx>, int a3)
{
  int v3; // esi
  char v4; // dl

  *(_DWORD *)result = &CSection::`vftable';
  if ( a2 )
  {
    v3 = result + 4 - (_DWORD)a2;
    do
    {
      v4 = *a2;
      a2[v3] = *a2;
      ++a2;
    }
    while ( v4 );
  }
  else
  {
    *(_BYTE *)(result + 4) = 0;
  }
  *(_DWORD *)(result + 264) = 0;
  *(_DWORD *)(result + 268) = 0;
  *(_DWORD *)(result + 272) = 0;
  *(_DWORD *)(result + 276) = 0;
  *(_DWORD *)(result + 280) = 0;
  *(_DWORD *)(result + 284) = 0;
  *(_DWORD *)(result + 288) = 0;
  *(_DWORD *)(result + 292) = 0;
  *(_DWORD *)(result + 260) = a3;
  return result;
}

// ===== sub_42CE40 @ 0x0042CE40..0x0042CE61 =====
void *__thiscall sub_42CE40(void *this, char a2)
{
  sub_42CE70();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42CE70 @ 0x0042CE70..0x0042CEB3 =====
void __thiscall sub_42CE70(_DWORD *this)
{
  _DWORD *v2; // esi
  _DWORD *v3; // esi
  void *v4; // [esp-4h] [ebp-Ch]
  void *v5; // [esp-4h] [ebp-Ch]

  v2 = (_DWORD *)this[69];
  *this = &CSection::`vftable';
  while ( v2 )
  {
    v4 = v2;
    v2 = (_DWORD *)v2[1];
    operator delete(v4);
  }
  v3 = (_DWORD *)this[72];
  while ( v3 )
  {
    v5 = v3;
    v3 = (_DWORD *)v3[2];
    operator delete(v5);
  }
}

// ===== sub_42CEC0 @ 0x0042CEC0..0x0042CF0D =====
_DWORD *__userpurge sub_42CEC0@<eax>(int a1@<edi>, int a2, unsigned int a3)
{
  int v3; // esi
  int i; // ebx
  _DWORD *result; // eax

  v3 = *(_DWORD *)(a1 + 288);
  for ( i = a1 + 280; v3; v3 = *(_DWORD *)(v3 + 8) )
  {
    if ( a3 > *(_DWORD *)(v3 + 4) )
      break;
    i = v3;
  }
  result = operator new(0xCu);
  result[2] = v3;
  *result = a2;
  result[1] = a3;
  *(_DWORD *)(i + 8) = result;
  ++*(_DWORD *)(a1 + 268);
  return result;
}

// ===== sub_42CF10 @ 0x0042CF10..0x0042CF6A =====
int __usercall sub_42CF10@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  _DWORD *v2; // ecx
  int result; // eax
  _DWORD *v4; // eax
  void *v5; // [esp-4h] [ebp-4h]

  v2 = (_DWORD *)a2[72];
  result = 0;
  if ( v2 && a1 == *v2 && a2[66] < a2[65] )
  {
    v5 = (void *)a2[72];
    a2[72] = v2[2];
    operator delete(v5);
    --a2[67];
    v4 = operator new(8u);
    *v4 = a1;
    v4[1] = a2[69];
    a2[69] = v4;
    result = 1;
    ++a2[66];
  }
  return result;
}

// ===== sub_42CF70 @ 0x0042CF70..0x0042CFAB =====
int __usercall sub_42CF70@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  _DWORD *v2; // ecx
  int result; // eax
  _DWORD *v4; // edx

  v2 = (_DWORD *)a2[69];
  result = 0;
  v4 = a2 + 68;
  if ( v2 )
  {
    while ( a1 != *v2 )
    {
      v4 = v2;
      v2 = (_DWORD *)v2[1];
      if ( !v2 )
        return result;
    }
    v4[1] = v2[1];
    operator delete(v2);
    --a2[66];
    return 1;
  }
  return result;
}

// ===== sub_42CFB0 @ 0x0042CFB0..0x0042CFE3 =====
BOOL __usercall sub_42CFB0@<eax>(_DWORD *a1@<edx>, unsigned int a2@<esi>)
{
  int v2; // eax
  int i; // ecx

  v2 = a1[72];
  for ( i = 0; v2; ++i )
  {
    if ( a2 > *(_DWORD *)(v2 + 4) )
      break;
    v2 = *(_DWORD *)(v2 + 8);
  }
  return a1[65] >= i + a1[66];
}

// ===== ?CheckStaticConstruction@SchedulerBase@details@Concurrency@@CAXXZ @ 0x0042CFF0..0x0042D05C =====
void __usercall Concurrency::details::SchedulerBase::CheckStaticConstruction(_DWORD *a1@<esi>)
{
  void *v1; // eax
  int v2; // eax

  *a1 = &CExclusion::`vftable';
  v1 = operator new(0x128u);
  if ( v1 )
    v2 = sub_42CDD0((int)v1, 0, 0);
  else
    v2 = 0;
  a1[1] = v2;
}

// ===== sub_42D060 @ 0x0042D060..0x0042D081 =====
void *__thiscall sub_42D060(void *this, char a2)
{
  sub_42D090();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42D090 @ 0x0042D090..0x0042D0AF =====
int __thiscall sub_42D090(_DWORD *this)
{
  int result; // eax
  int (__thiscall ***v3)(_DWORD, int); // ecx

  *this = &CExclusion::`vftable';
  result = sub_42D280();
  v3 = (int (__thiscall ***)(_DWORD, int))this[1];
  if ( v3 )
    return (**v3)(v3, 1);
  return result;
}

// ===== sub_42D0B0 @ 0x0042D0B0..0x0042D0FA =====
int __usercall sub_42D0B0@<eax>(int a1@<eax>, const char *a2@<esi>)
{
  int result; // eax

  for ( result = *(_DWORD *)(*(_DWORD *)(a1 + 4) + 292); result; result = *(_DWORD *)(result + 292) )
  {
    if ( !strcmp(a2, (const char *)(result + 4)) )
      break;
  }
  return result;
}

// ===== sub_42D100 @ 0x0042D100..0x0042D1D2 =====
int __stdcall sub_42D100(char *a1, int a2)
{
  int v2; // edi
  int v3; // esi
  int result; // eax
  void *v5; // eax
  int v6; // eax

  v2 = *(_DWORD *)(dword_566770 + 4);
  v3 = *(_DWORD *)(v2 + 292);
  result = 0;
  if ( v3 )
  {
    while ( strcmp(a1, (const char *)(v3 + 4)) )
    {
      v2 = v3;
      v3 = *(_DWORD *)(v3 + 292);
      if ( !v3 )
        goto LABEL_6;
    }
  }
  else
  {
LABEL_6:
    v5 = operator new(0x128u);
    if ( v5 )
      v6 = sub_42CDD0((int)v5, a1, a2);
    else
      v6 = 0;
    *(_DWORD *)(v2 + 292) = v6;
    return 1;
  }
  return result;
}

// ===== sub_42D1E0 @ 0x0042D1E0..0x0042D27C =====
int __usercall sub_42D1E0@<eax>(const char *a1@<eax>)
{
  int v1; // edi
  int v2; // ecx

  v1 = *(_DWORD *)(dword_566770 + 4);
  v2 = *(_DWORD *)(v1 + 292);
  if ( !v2 )
    return -2147483647;
  while ( strcmp(a1, (const char *)(v2 + 4)) )
  {
    v1 = v2;
    v2 = *(_DWORD *)(v2 + 292);
    if ( !v2 )
      return -2147483647;
  }
  if ( *(_DWORD *)(v2 + 264) || *(_DWORD *)(v2 + 268) )
    return -2147483646;
  *(_DWORD *)(v1 + 292) = *(_DWORD *)(v2 + 292);
  (**(void (__thiscall ***)(int, int))v2)(v2, 1);
  return 0;
}

// ===== sub_42D280 @ 0x0042D280..0x0042D2BD =====
int __usercall sub_42D280@<eax>(int a1@<edi>)
{
  int result; // eax
  int v2; // esi
  int v3; // ecx
  int (__thiscall *v4)(int, int); // eax

  result = *(_DWORD *)(a1 + 4);
  v2 = *(_DWORD *)(result + 292);
  if ( v2 )
  {
    do
    {
      v3 = v2;
      v4 = **(int (__thiscall ***)(int, int))v2;
      v2 = *(_DWORD *)(v2 + 292);
      result = v4(v3, 1);
    }
    while ( v2 );
    *(_DWORD *)(*(_DWORD *)(a1 + 4) + 292) = 0;
  }
  else
  {
    *(_DWORD *)(result + 292) = 0;
  }
  return result;
}

// ===== sub_42D2C0 @ 0x0042D2C0..0x0042D2F0 =====
BOOL __userpurge sub_42D2C0@<eax>(int a1@<eax>, const char *a2@<ecx>, int a3, unsigned int a4)
{
  int v4; // eax
  BOOL v5; // esi

  v4 = sub_42D0B0(a1, a2);
  v5 = v4 != 0;
  if ( v4 )
    sub_42CEC0(v4, a3, a4);
  return v5;
}

// ===== sub_42D2F0 @ 0x0042D2F0..0x0042D32B =====
int __userpurge sub_42D2F0@<eax>(int a1@<eax>, const char *a2@<ecx>, int a3)
{
  _DWORD *v3; // eax

  v3 = (_DWORD *)sub_42D0B0(a1, a2);
  if ( v3 )
    return sub_42CF10(a3, v3) != 0 ? 0 : -2147483644;
  else
    return -2147483647;
}

// ===== sub_42D330 @ 0x0042D330..0x0042D370 =====
int __userpurge sub_42D330@<eax>(const char *a1@<eax>, int a2)
{
  _DWORD *v2; // eax

  v2 = (_DWORD *)sub_42D0B0(dword_566770, a1);
  if ( v2 )
    return sub_42CF70(a2, v2) != 0 ? 0 : -2147483645;
  else
    return -2147483647;
}

// ===== sub_42D370 @ 0x0042D370..0x0042D3B0 =====
int __userpurge sub_42D370@<eax>(const char *a1@<eax>, unsigned int a2)
{
  _DWORD *v2; // eax

  v2 = (_DWORD *)sub_42D0B0(dword_566770, a1);
  if ( v2 )
    return sub_42CFB0(v2, a2) ? 0 : -2147483644;
  else
    return -2147483647;
}

// ===== sub_42D3B0 @ 0x0042D3B0..0x0042D3C5 =====
_DWORD *__usercall sub_42D3B0@<eax>(_DWORD *result@<eax>)
{
  *result = &CFileDX::`vftable';
  result[1] = -1;
  result[2] = 0;
  return result;
}

// ===== sub_42D3D0 @ 0x0042D3D0..0x0042D3F1 =====
void *__thiscall sub_42D3D0(void *this, char a2)
{
  sub_42D400(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42D400 @ 0x0042D400..0x0042D410 =====
int __thiscall sub_42D400(_DWORD *this)
{
  *this = &CFileDX::`vftable';
  return sub_42D5B0();
}

// ===== sub_42D410 @ 0x0042D410..0x0042D513 =====
int __usercall sub_42D410@<eax>(CHAR *a1@<ecx>, CHAR *a2@<edi>, CHAR *a3@<esi>, CHAR *a4, const CHAR *lpMultiByteStr)
{
  int result; // eax
  WCHAR Filename[780]; // [esp+Ch] [ebp-1E7Ch] BYREF
  WCHAR Ext[780]; // [esp+624h] [ebp-1864h] BYREF
  WCHAR Dir[780]; // [esp+C3Ch] [ebp-124Ch] BYREF
  WCHAR WideCharStr[780]; // [esp+1254h] [ebp-C34h] BYREF
  WCHAR Drive[780]; // [esp+186Ch] [ebp-61Ch] BYREF

  MultiByteToWideChar(0, 0, lpMultiByteStr, -1, WideCharStr, 780);
  _wsplitpath(WideCharStr, Drive, Dir, Filename, Ext);
  if ( a3 )
    WideCharToMultiByte(0, 0, Drive, -1, a3, 780, 0, 0);
  if ( a2 )
    WideCharToMultiByte(0, 0, Dir, -1, a2, 780, 0, 0);
  if ( a4 )
    WideCharToMultiByte(0, 0, Filename, -1, a4, 780, 0, 0);
  result = (int)a1;
  if ( a1 )
    return WideCharToMultiByte(0, 0, Ext, -1, a1, 780, 0, 0);
  return result;
}

// ===== sub_42D520 @ 0x0042D520..0x0042D55A =====
int __usercall sub_42D520@<eax>(const CHAR *a1@<eax>, int a2@<esi>)
{
  HANDLE FileA; // eax

  if ( *(_DWORD *)(a2 + 4) == -1 )
  {
    FileA = CreateFileA(a1, 0x80000000, 1u, 0, 3u, 0x8000020u, 0);
    *(_DWORD *)(a2 + 4) = FileA;
    if ( FileA != (HANDLE)-1 )
      return 1;
    *(_DWORD *)(a2 + 8) = GetLastError();
  }
  return 0;
}

// ===== sub_42D560 @ 0x0042D560..0x0042D564 =====
int __usercall sub_42D560@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 8);
}

// ===== sub_42D570 @ 0x0042D570..0x0042D5AE =====
BOOL __userpurge sub_42D570@<eax>(LPCSTR lpFileName@<ecx>, int a2@<esi>, int a3)
{
  BOOL result; // eax
  char *FileA; // eax

  result = 0;
  if ( *(_DWORD *)(a2 + 4) == -1 )
  {
    LOBYTE(result) = a3 == 1;
    FileA = (char *)CreateFileA(lpFileName, 0x40000000u, 0, 0, 2 * result + 2, 0x8000020u, 0);
    *(_DWORD *)(a2 + 4) = FileA;
    return FileA + 1 != 0;
  }
  return result;
}

// ===== sub_42D5B0 @ 0x0042D5B0..0x0042D5C7 =====
int __usercall sub_42D5B0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 4);
  if ( result != -1 )
  {
    result = CloseHandle(*(HANDLE *)(a1 + 4));
    *(_DWORD *)(a1 + 4) = -1;
  }
  return result;
}

// ===== sub_42D5D0 @ 0x0042D5D0..0x0042D5F7 =====
int __fastcall sub_42D5D0(DWORD nNumberOfBytesToRead, LPVOID lpBuffer, DWORD NumberOfBytesRead)
{
  int result; // eax

  result = ReadFile(*(HANDLE *)(NumberOfBytesRead + 4), lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, 0);
  if ( result )
    return NumberOfBytesRead;
  return result;
}

// ===== sub_42D600 @ 0x0042D600..0x0042D627 =====
int __fastcall sub_42D600(DWORD nNumberOfBytesToWrite, LPCVOID lpBuffer, DWORD NumberOfBytesWritten)
{
  int result; // eax

  result = WriteFile(*(HANDLE *)(NumberOfBytesWritten + 4), lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, 0);
  if ( result )
    return NumberOfBytesWritten;
  return result;
}

// ===== sub_42D630 @ 0x0042D630..0x0042D64A =====
BOOL __usercall sub_42D630@<eax>(LONG a1@<eax>, int a2@<ecx>)
{
  return SetFilePointer(*(HANDLE *)(a2 + 4), a1, 0, 0) != -1;
}

// ===== sub_42D650 @ 0x0042D650..0x0042D65D =====
DWORD __usercall sub_42D650@<eax>(int a1@<eax>)
{
  return GetFileSize(*(HANDLE *)(a1 + 4), 0);
}

// ===== sub_42D660 @ 0x0042D660..0x0042D67A =====
BOOL __userpurge sub_42D660@<eax>(
        LPFILETIME lpLastAccessTime@<ecx>,
        struct _FILETIME *a2@<eax>,
        int a3,
        LPFILETIME lpCreationTime)
{
  return GetFileTime(*(HANDLE *)(a3 + 4), lpCreationTime, lpLastAccessTime, a2);
}

// ===== sub_42D680 @ 0x0042D680..0x0042D69A =====
BOOL __userpurge sub_42D680@<eax>(
        FILETIME *lpLastAccessTime@<ecx>,
        const FILETIME *a2@<eax>,
        int a3,
        FILETIME *lpCreationTime)
{
  return SetFileTime(*(HANDLE *)(a3 + 4), lpCreationTime, lpLastAccessTime, a2);
}

// ===== sub_42D6A0 @ 0x0042D6A0..0x0042D6C0 =====
int __usercall sub_42D6A0@<eax>(_DWORD *a1@<eax>)
{
  int result; // eax
  int v2; // ecx

  *a1 = &CFontDx::`vftable';
  a1[1] = 0;
  result = sub_42E9E0(0);
  *(_DWORD *)(result + 76) = v2;
  *(_DWORD *)(result + 136) = v2;
  return result;
}

// ===== sub_42D6C0 @ 0x0042D6C0..0x0042D6E1 =====
void *__thiscall sub_42D6C0(void *this, char a2)
{
  sub_42D6F0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42D6F0 @ 0x0042D6F0..0x0042D718 =====
void __thiscall sub_42D6F0(void **this)
{
  *this = &CFontDx::`vftable';
  sub_42DD70();
  operator delete(this[19]);
  operator delete(this[34]);
}

// ===== sub_42D780 @ 0x0042D780..0x0042D816 =====
const char *__usercall sub_42D780@<eax>(const char *a1@<edi>, const char *a2)
{
  const char **v2; // esi
  const char *result; // eax
  const char *v4; // eax
  const char *v5; // ecx
  char *v6; // edx
  char v7; // al

  v2 = (const char **)dword_565B5C;
  if ( dword_565B5C )
  {
    while ( 1 )
    {
      result = (const char *)strcmp(a1, *v2);
      if ( !result )
        break;
      v2 = (const char **)v2[3];
      if ( !v2 )
        goto LABEL_4;
    }
  }
  else
  {
LABEL_4:
    v2 = (const char **)operator new(0x10u);
    v4 = (const char *)operator new(strlen(a1) + 1);
    *v2 = v4;
    v5 = a1;
    v6 = (char *)v4;
    do
    {
      v7 = *v5;
      *v6++ = *v5++;
    }
    while ( v7 );
    result = (const char *)dword_565B5C;
    v2[1] = 0;
    v2[3] = result;
    dword_565B5C = v2;
  }
  v2[2] = a2;
  return result;
}

// ===== sub_42D820 @ 0x0042D820..0x0042D97A =====
void __usercall sub_42D820(const char *a1@<edi>, const char *a2)
{
  void *v2; // esi
  _BYTE *v3; // eax
  const char *v4; // ecx
  _BYTE *v5; // edx
  char v6; // al
  void *v7; // ecx
  _BYTE *v8; // eax
  const char *v9; // ecx
  _BYTE *v10; // edx
  char v11; // al
  const char *v12; // ecx
  int v13; // edx
  char v14; // al

  if ( a1 )
  {
    v2 = dword_565B5C;
    if ( dword_565B5C )
    {
      while ( strcmp(a1, *(const char **)v2) )
      {
        v2 = (void *)*((_DWORD *)v2 + 3);
        if ( !v2 )
          goto LABEL_5;
      }
      operator delete(*((void **)v2 + 1));
      *((_DWORD *)v2 + 1) = 0;
    }
    else
    {
LABEL_5:
      v2 = operator new(0x10u);
      v3 = operator new(strlen(a1) + 1);
      *(_DWORD *)v2 = v3;
      v4 = a1;
      v5 = v3;
      do
      {
        v6 = *v4;
        *v5++ = *v4++;
      }
      while ( v6 );
      v7 = dword_565B5C;
      *((_DWORD *)v2 + 2) = -1;
      *((_DWORD *)v2 + 3) = v7;
      dword_565B5C = v2;
    }
    if ( a2 )
    {
      v8 = operator new(strlen(a2) + 1);
      *((_DWORD *)v2 + 1) = v8;
      v9 = a2;
      v10 = v8;
      do
      {
        v11 = *v9;
        *v10++ = *v9++;
      }
      while ( v11 );
    }
  }
  else
  {
    operator delete(dword_565B4C);
    if ( a2 )
    {
      dword_565B4C = operator new(strlen(a2) + 1);
      v12 = a2;
      v13 = (_BYTE *)dword_565B4C - a2;
      do
      {
        v14 = *v12;
        v12[v13] = *v12;
        ++v12;
      }
      while ( v14 );
    }
    else
    {
      dword_565B4C = 0;
    }
  }
}

// ===== sub_42D980 @ 0x0042D980..0x0042D9F7 =====
int __usercall sub_42D980@<eax>(const char *a1@<edi>)
{
  const char **v1; // esi
  int result; // eax

  v1 = (const char **)dword_565B5C;
  if ( !dword_565B5C )
    goto LABEL_6;
  while ( strcmp(a1, *v1) )
  {
    v1 = (const char **)v1[3];
    if ( !v1 )
      goto LABEL_6;
  }
  result = (int)v1[2];
  if ( result == -1 )
  {
LABEL_6:
    if ( sub_42DA70((LPARAM)a1) )
      return 128;
    else
      return sub_42DA00((LPARAM)a1) == 0;
  }
  return result;
}

// ===== sub_42DA00 @ 0x0042DA00..0x0042DA6A =====
int __thiscall sub_42DA00(LPARAM lParam)
{
  char *v2; // eax
  CHAR *v3; // edx
  char v4; // cl
  HDC DC; // edi
  int v6; // esi
  tagLOGFONTA Logfont; // [esp+8h] [ebp-40h] BYREF

  Logfont.lfCharSet = 0;
  v2 = (char *)lParam;
  v3 = &Logfont.lfFaceName[-lParam];
  do
  {
    v4 = *v2;
    v2[(_DWORD)v3] = *v2;
    ++v2;
  }
  while ( v4 );
  Logfont.lfPitchAndFamily = 0;
  DC = GetDC(0);
  v6 = -(EnumFontFamiliesExA(DC, &Logfont, Proc, lParam, 0) != 0);
  ReleaseDC(0, DC);
  return v6 + 1;
}

// ===== sub_42DA70 @ 0x0042DA70..0x0042DADA =====
int __thiscall sub_42DA70(LPARAM lParam)
{
  char *v2; // eax
  CHAR *v3; // edx
  char v4; // cl
  HDC DC; // edi
  int v6; // esi
  struct tagLOGFONTA Logfont; // [esp+8h] [ebp-40h] BYREF

  Logfont.lfCharSet = 0x80;
  v2 = (char *)lParam;
  v3 = &Logfont.lfFaceName[-lParam];
  do
  {
    v4 = *v2;
    v2[(_DWORD)v3] = *v2;
    ++v2;
  }
  while ( v4 );
  Logfont.lfPitchAndFamily = 0;
  DC = GetDC(0);
  v6 = -(EnumFontFamiliesExA(DC, &Logfont, Proc, lParam, 0) != 0);
  ReleaseDC(0, DC);
  return v6 + 1;
}

// ===== sub_42DAE0 @ 0x0042DAE0..0x0042DB54 =====
LPARAM __usercall sub_42DAE0@<eax>(LPARAM a1@<esi>)
{
  HDC DC; // edi
  LPARAM result; // eax
  LPARAM lParam[2]; // [esp+8h] [ebp-4Ch] BYREF
  int v4; // [esp+10h] [ebp-44h]
  struct tagLOGFONTA Logfont; // [esp+14h] [ebp-40h] BYREF

  memset(&Logfont, 0, sizeof(Logfont));
  Logfont.lfCharSet = 0x80;
  *(_WORD *)&Logfont.lfPitchAndFamily = 0;
  lParam[0] = 0;
  lParam[1] = a1;
  v4 = 0;
  DC = GetDC(0);
  EnumFontFamiliesExA(DC, &Logfont, sub_42EC50, (LPARAM)lParam, 0);
  ReleaseDC(0, DC);
  result = lParam[0];
  if ( !a1 )
    return v4;
  return result;
}

// ===== sub_42DB60 @ 0x0042DB60..0x0042DB94 =====
int __usercall sub_42DB60@<eax>(int a1@<edi>)
{
  if ( sub_42DBA0(a1, 128) )
    return 1;
  else
    return sub_42DBA0(a1, 0);
}

// ===== sub_42DBA0 @ 0x0042DBA0..0x0042DC26 =====
int __cdecl sub_42DBA0(int *a1, BYTE a2)
{
  LPARAM v2; // ecx
  LPARAM v3; // esi
  char *v4; // eax
  CHAR *v5; // edx
  char v6; // cl
  HDC DC; // edi
  int v8; // esi
  int v9; // esi
  LPARAM lParam[7]; // [esp+10h] [ebp-80h] BYREF
  char v12; // [esp+2Fh] [ebp-61h]
  struct tagLOGFONTA Logfont; // [esp+50h] [ebp-40h] BYREF

  v3 = v2;
  Logfont.lfCharSet = a2;
  v4 = (char *)v2;
  v5 = &Logfont.lfFaceName[-v2];
  do
  {
    v6 = *v4;
    v4[(_DWORD)v5] = *v4;
    ++v4;
  }
  while ( v6 );
  Logfont.lfPitchAndFamily = 0;
  lParam[0] = v3;
  DC = GetDC(0);
  v8 = -(EnumFontFamiliesExA(DC, &Logfont, sub_42ECA0, (LPARAM)lParam, 0) != 0);
  ReleaseDC(0, DC);
  v9 = v8 + 1;
  if ( v9 )
    *a1 = v12 & 3;
  return v9;
}

// ===== sub_42DC30 @ 0x0042DC30..0x0042DC36 =====
int __usercall sub_42DC30@<eax>(int result@<eax>)
{
  dword_565B60 = result;
  return result;
}

// ===== sub_42DC40 @ 0x0042DC40..0x0042DC53 =====
int __fastcall sub_42DC40(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    dword_565B64 = a1;
    return 1;
  }
  return result;
}

// ===== sub_42DC60 @ 0x0042DC60..0x0042DC75 =====
int sub_42DC60()
{
  dword_507694 = 0;
  return sub_42DC80(dword_507698);
}

// ===== sub_42DC80 @ 0x0042DC80..0x0042DD59 =====
BOOL __fastcall sub_42DC80(int a1)
{
  BOOL result; // eax

  result = a1 <= 3;
  if ( a1 <= 3 )
  {
    if ( dword_507694 )
    {
      switch ( a1 )
      {
        case 0:
          dword_5076A0 = 2;
          dword_5076A4 = 1;
          dword_50769C = 2;
          dword_507698 = a1;
          break;
        case 1:
          dword_5076A0 = 4;
          dword_5076A4 = 2;
          dword_50769C = 4;
          dword_507698 = a1;
          break;
        case 2:
          dword_50769C = 6;
          dword_5076A0 = 8;
          dword_5076A4 = 3;
          dword_507698 = a1;
          break;
        case 3:
          dword_50769C = 8;
          dword_5076A0 = 16;
          dword_5076A4 = 4;
          dword_507698 = a1;
          break;
        default:
          goto LABEL_8;
      }
    }
    else
    {
LABEL_8:
      dword_5076A0 = 1;
      dword_5076A4 = 0;
      dword_50769C = 0;
      dword_507698 = a1;
    }
  }
  return result;
}

// ===== sub_42DD70 @ 0x0042DD70..0x0042DDCB =====
BOOL __usercall sub_42DD70@<eax>(int a1@<esi>)
{
  BOOL result; // eax

  if ( *(_DWORD *)(a1 + 4) )
  {
    SelectObject(*(HDC *)(a1 + 156), *(HGDIOBJ *)(a1 + 164));
    SelectObject(*(HDC *)(a1 + 156), *(HGDIOBJ *)(a1 + 168));
    DeleteObject(*(HGDIOBJ *)(a1 + 148));
    DeleteObject(*(HGDIOBJ *)(a1 + 152));
    result = DeleteDC(*(HDC *)(a1 + 156));
    *(_DWORD *)(a1 + 4) = 0;
  }
  return result;
}

// ===== sub_42DDD0 @ 0x0042DDD0..0x0042DDE5 =====
int __usercall sub_42DDD0@<eax>(const CHAR *a1@<eax>)
{
  return sub_42DDF0(a1, 16, 100, 0, 0, 0, 2, 1);
}

// ===== sub_42DDF0 @ 0x0042DDF0..0x0042DF36 =====
int __thiscall sub_42DDF0(
        int this,
        LPCSTR pszFaceName,
        int a3,
        int a4,
        int a5,
        DWORD bItalic,
        int a7,
        int cHeight,
        int a9)
{
  int result; // eax
  size_t OutlineTextMetricsA; // edi
  const CHAR *v12; // eax
  int v13; // edi
  HDC v14; // [esp-Ch] [ebp-24h]
  struct _OUTLINETEXTMETRICA *v15; // [esp+10h] [ebp-8h]

  sub_42DD70(this);
  if ( cHeight < 2 )
    return -2147483647;
  result = sub_42EAB0(a4);
  if ( !result )
  {
    result = sub_42DF40(this, pszFaceName, a3, a4, a5, bItalic, cHeight);
    if ( !result )
    {
      v14 = *(HDC *)(this + 156);
      *(_DWORD *)(this + 4) = 1;
      OutlineTextMetricsA = GetOutlineTextMetricsA(v14, 0, 0);
      v15 = (struct _OUTLINETEXTMETRICA *)operator new(OutlineTextMetricsA);
      if ( GetOutlineTextMetricsA(*(HDC *)(this + 156), OutlineTextMetricsA, v15)
        && !strcmp(pszFaceName, (const char *)v15 + (unsigned int)v15->otmpFamilyName)
        || !a9 )
      {
        operator delete(v15);
        return 0;
      }
      else
      {
        v12 = (const CHAR *)sub_42EBD0();
        if ( !v12 )
          v12 = pszFaceName;
        v13 = sub_42DDF0(v12, a3, a4, a5, bItalic, 0, cHeight, 0);
        operator delete(v15);
        return v13;
      }
    }
  }
  return result;
}

// ===== sub_42DF40 @ 0x0042DF40..0x0042E36D =====
int __userpurge sub_42DF40@<eax>(
        int *a1@<eax>,
        int a2,
        LPCSTR pszFaceName,
        int a4,
        int a5,
        int a6,
        DWORD bItalic,
        int cHeight)
{
  int result; // eax
  int v11; // eax
  int v12; // edx
  int v13; // eax
  int v14; // ecx
  int v15; // eax
  int v16; // edx
  int v17; // ecx
  bool v18; // zf
  int v19; // eax
  int v20; // edi
  void *v21; // eax
  void *v22; // eax
  int v23; // ecx
  int v24; // eax
  int v25; // edi
  int v26; // esi
  int v27; // eax
  DWORD v28; // eax
  HFONT FontA; // eax
  char *v30; // edi
  int v31; // ecx
  _BYTE *v32; // eax
  int v33; // edx
  HDC CompatibleDC; // eax
  void *v35; // edx
  int v36; // ecx
  HGDIOBJ v37; // eax
  size_t OutlineTextMetricsA; // edi
  UINT v39; // eax
  int v40; // edi
  int v41; // ecx
  int v42; // eax
  int v43; // eax
  int v44; // eax
  void *v45; // [esp-Ch] [ebp-58h]
  DWORD v46; // [esp-8h] [ebp-54h]
  HDC v47; // [esp-8h] [ebp-54h]
  HDC v48; // [esp-8h] [ebp-54h]
  void *v49; // [esp-4h] [ebp-50h]
  char pv[12]; // [esp+10h] [ebp-3Ch] BYREF
  int v51; // [esp+1Ch] [ebp-30h]
  int v52; // [esp+28h] [ebp-24h]
  int v53; // [esp+2Ch] [ebp-20h]
  int v54; // [esp+30h] [ebp-1Ch]
  int v55; // [esp+34h] [ebp-18h]
  DWORD iPitchAndFamily; // [esp+38h] [ebp-14h] BYREF
  int v57; // [esp+3Ch] [ebp-10h]
  int v58; // [esp+40h] [ebp-Ch]
  int v59; // [esp+44h] [ebp-8h]
  int v60; // [esp+54h] [ebp+8h]
  struct _OUTLINETEXTMETRICA *pszFaceNamea; // [esp+58h] [ebp+Ch]
  int cHeighta; // [esp+6Ch] [ebp+20h]

  v53 = 0;
  v59 = 0;
  iPitchAndFamily = 1;
  v60 = 0x10000;
  v58 = 0x10000;
  v55 = 0;
  v54 = 0;
  if ( a1 )
  {
    result = sub_42EB30();
    v53 = result;
    if ( result )
      return result;
    v11 = *a1;
    if ( *a1 || a1[1] || a1[2] || a1[3] )
    {
      iPitchAndFamily = v11 != 0;
      if ( !v11 )
        v11 = a1[1];
      v12 = a1[2];
      v60 = v11;
      v13 = a1[3];
      v58 = a1[1];
      v55 = v12;
      v54 = v13;
    }
    else
    {
      v59 = 1;
      iPitchAndFamily = 0;
    }
    *(_DWORD *)(a2 + 40) = *a1;
    *(_DWORD *)(a2 + 44) = a1[1];
    *(_DWORD *)(a2 + 48) = a1[2];
    *(_DWORD *)(a2 + 52) = a1[3];
  }
  else
  {
    *(_DWORD *)(a2 + 40) = 0x10000;
    *(_DWORD *)(a2 + 44) = 0x10000;
    *(_DWORD *)(a2 + 48) = 0;
    *(_DWORD *)(a2 + 52) = 0;
  }
  v14 = 2 * a5 * a4 / 100;
  v15 = 3 * a4 / 2;
  *(_DWORD *)(a2 + 68) = v14;
  *(_DWORD *)(a2 + 80) = v14;
  *(_DWORD *)(a2 + 84) = v14 * v15;
  v16 = dword_5076A0;
  v17 = dword_5076A0 * ((v60 * v14) >> 16);
  v18 = v59 == 0;
  *(_DWORD *)(a2 + 72) = v15;
  *(_DWORD *)(a2 + 88) = v17;
  if ( v18 )
    v19 = (v58 * v15) >> 16;
  else
    v19 = 2 * v15;
  *(_DWORD *)(a2 + 92) = v16 * v19;
  operator delete(*(void **)(a2 + 76));
  v20 = cHeight;
  v21 = operator new(cHeight * *(_DWORD *)(a2 + 84));
  v45 = *(void **)(a2 + 136);
  *(_DWORD *)(a2 + 76) = v21;
  operator delete(v45);
  v22 = operator new(32 * cHeight);
  v23 = 0;
  *(_DWORD *)(a2 + 136) = v22;
  v24 = *(_DWORD *)(a2 + 76);
  *(_DWORD *)(a2 + 144) = cHeight;
  *(_DWORD *)(a2 + 140) = 0;
  if ( cHeight > 0 )
  {
    do
    {
      *(_DWORD *)(v23 + *(_DWORD *)(a2 + 136) + 8) = v24;
      v24 += *(_DWORD *)(a2 + 84);
      v23 += 32;
      --v20;
    }
    while ( v20 );
  }
  v25 = dword_5076A0 * ((a4 * v58) >> 16);
  v52 = (a4 * v58) >> 16;
  v57 = v25;
  if ( iPitchAndFamily )
  {
    v26 = (dword_5076A0 * ((v60 * (a5 * a4 / 100)) >> 16)) >> 1;
    cHeighta = v26;
  }
  else
  {
    cHeighta = 0;
    v26 = 0;
  }
  iPitchAndFamily = 0;
  if ( dword_565B60 )
  {
    v27 = sub_42DB60((int)&iPitchAndFamily);
    v25 = v57;
    if ( v27 && iPitchAndFamily == 2 )
      cHeighta = 0;
    v26 = cHeighta;
  }
  if ( v59 )
    v25 = -v25;
  v46 = iPitchAndFamily;
  v28 = sub_42D980(pszFaceName);
  FontA = CreateFontA(v25, v26, 0, 0, a6 != 0 ? 700 : 100, bItalic, 0, 0, v28, 4u, 0, 2u, v46, pszFaceName);
  *(_DWORD *)(a2 + 152) = FontA;
  if ( FontA )
  {
    v30 = (char *)operator new(0x428u);
    *(_DWORD *)v30 = 40;
    *((_DWORD *)v30 + 1) = *(_DWORD *)(a2 + 88);
    v31 = 255;
    *((_DWORD *)v30 + 2) = -*(_DWORD *)(a2 + 92);
    *((_DWORD *)v30 + 3) = 524289;
    *((_DWORD *)v30 + 4) = 0;
    *((_DWORD *)v30 + 5) = 0;
    *((_DWORD *)v30 + 6) = 0;
    *((_DWORD *)v30 + 7) = 0;
    *((_DWORD *)v30 + 8) = 256;
    *((_DWORD *)v30 + 9) = 0;
    v32 = v30 + 41;
    v33 = 256;
    do
    {
      *(v32 - 1) = v31;
      *v32 = v31;
      v32[1] = v31;
      v32[2] = 0;
      v32 += 4;
      --v31;
      --v33;
    }
    while ( v33 );
    CompatibleDC = CreateCompatibleDC(0);
    *(_DWORD *)(a2 + 156) = CompatibleDC;
    *(_DWORD *)(a2 + 148) = CreateDIBSection(CompatibleDC, (const BITMAPINFO *)v30, 0, (void **)(a2 + 160), 0, 0);
    operator delete(v30);
    GetObjectA(*(HANDLE *)(a2 + 148), 24, pv);
    v35 = *(void **)(a2 + 148);
    v36 = v51 * *(_DWORD *)(a2 + 92);
    *(_DWORD *)(a2 + 96) = v51;
    v47 = *(HDC *)(a2 + 156);
    *(_DWORD *)(a2 + 100) = v36;
    v37 = SelectObject(v47, v35);
    v49 = *(void **)(a2 + 152);
    v48 = *(HDC *)(a2 + 156);
    *(_DWORD *)(a2 + 164) = v37;
    *(_DWORD *)(a2 + 168) = SelectObject(v48, v49);
    *(_DWORD *)(a2 + 8) = a4;
    *(_DWORD *)(a2 + 12) = a5;
    *(_DWORD *)(a2 + 16) = a6;
    *(_DWORD *)(a2 + 20) = bItalic;
    if ( v59 )
    {
      OutlineTextMetricsA = GetOutlineTextMetricsA(*(HDC *)(a2 + 156), 0, 0);
      pszFaceNamea = (struct _OUTLINETEXTMETRICA *)operator new(OutlineTextMetricsA);
      v39 = GetOutlineTextMetricsA(*(HDC *)(a2 + 156), OutlineTextMetricsA, pszFaceNamea);
      v40 = v60;
      if ( v39 )
      {
        v41 = v57;
        *(_DWORD *)(a2 + 28) = v58;
        *(_DWORD *)(a2 + 24) = v60;
        *(_DWORD *)(a2 + 32) = 0;
        v42 = (pszFaceNamea->otmTextMetrics.tmAscent - 7 * v41 / 8) >> dword_5076A4;
        *(_DWORD *)(a2 + 36) = v42 < 0 ? 0 : v42;
      }
      else
      {
        v59 = 0;
      }
      operator delete(pszFaceNamea);
      if ( v59 )
        goto LABEL_39;
    }
    else
    {
      v40 = v60;
    }
    v43 = v58;
    *(_DWORD *)(a2 + 24) = v40;
    *(_DWORD *)(a2 + 28) = v43;
    v44 = (v54 * v52) >> 16;
    *(_DWORD *)(a2 + 32) = (v55 * ((a4 * v40) >> 16)) >> 16;
    *(_DWORD *)(a2 + 36) = v44;
LABEL_39:
    result = v53;
    *(_DWORD *)(a2 + 64) = v59;
    return result;
  }
  return -2147483644;
}

// ===== sub_42E370 @ 0x0042E370..0x0042E8A9 =====
_DWORD *__userpurge sub_42E370@<eax>(char *a1@<ecx>, int a2@<esi>, _DWORD *a3, UINT uChar)
{
  int v5; // eax
  int v6; // eax
  int v7; // edi
  _BYTE *v8; // ebx
  _BYTE *v9; // edx
  _BYTE *v10; // eax
  int v11; // ecx
  _BYTE *v13; // edi
  _BYTE *v14; // eax
  int v15; // ebx
  int v16; // edx
  int v17; // ecx
  int v18; // ecx
  UINT v19; // edi
  size_t GlyphOutlineA; // ebx
  signed int gmBlackBoxX; // edi
  int v22; // edi
  int v23; // eax
  char *v24; // edi
  UINT v25; // eax
  char *v26; // edx
  int v27; // eax
  int v28; // ebx
  bool v29; // zf
  int v30; // edi
  int v31; // edx
  char *v32; // eax
  int v33; // ecx
  int v34; // edi
  int v35; // edx
  char *v36; // eax
  int v37; // ecx
  int v38; // ecx
  int v39; // eax
  int v41; // [esp+Ch] [ebp-84h]
  signed int v42; // [esp+Ch] [ebp-84h]
  int v43; // [esp+10h] [ebp-80h]
  signed int v44; // [esp+10h] [ebp-80h]
  int v45; // [esp+14h] [ebp-7Ch]
  UINT v46; // [esp+14h] [ebp-7Ch]
  char *v48; // [esp+1Ch] [ebp-74h]
  char v49; // [esp+1Ch] [ebp-74h]
  int v50; // [esp+20h] [ebp-70h]
  signed int v51; // [esp+20h] [ebp-70h]
  void *v52; // [esp+24h] [ebp-6Ch]
  int v53; // [esp+28h] [ebp-68h]
  signed int gmBlackBoxY; // [esp+28h] [ebp-68h]
  int v55; // [esp+28h] [ebp-68h]
  CHAR String[4]; // [esp+2Ch] [ebp-64h] BYREF
  tagTEXTMETRICA tm; // [esp+30h] [ebp-60h] BYREF
  _GLYPHMETRICS gm; // [esp+68h] [ebp-28h] BYREF
  MAT2 mat2; // [esp+7Ch] [ebp-14h] BYREF

  if ( !dword_507694 )
  {
    mat2.eM12 = 0;
    mat2.eM11 = (FIXED)0x10000;
    mat2.eM21 = 0;
    mat2.eM22 = (FIXED)0x10000;
    if ( dword_507698 )
    {
      if ( dword_507698 == 1 )
      {
        v19 = 5;
        v49 = 4;
      }
      else
      {
        v19 = 6;
        v49 = 6;
      }
    }
    else
    {
      v19 = 4;
      v49 = 2;
    }
    GlyphOutlineA = GetGlyphOutlineA(*(HDC *)(a2 + 156), uChar, v19, &gm, 0, 0, &mat2);
    v52 = operator new(GlyphOutlineA);
    GetGlyphOutlineA(*(HDC *)(a2 + 156), uChar, v19, &gm, GlyphOutlineA, v52, &mat2);
    GetTextMetricsA(*(HDC *)(a2 + 156), &tm);
    gmBlackBoxX = gm.gmBlackBoxX;
    v51 = gm.gmBlackBoxX;
    gmBlackBoxY = gm.gmBlackBoxY;
    memset(a1, 0, *(_DWORD *)(a2 + 84));
    if ( (gmBlackBoxX >= 2 || (int)gm.gmBlackBoxY >= 2) && gmBlackBoxX > 0 && (int)gm.gmBlackBoxY > 0 )
    {
      v22 = *(_DWORD *)(a2 + 80);
      v23 = tm.tmAscent - *(_DWORD *)(a2 + 36) - gm.gmptGlyphOrigin.y;
      *(_DWORD *)String = v52;
      v24 = &a1[(v23 < 0 ? 0 : v23) * v22 + *(_DWORD *)(a2 + 32)];
      v25 = (GlyphOutlineA / gm.gmBlackBoxY) & 0xFFFFFFFC;
      v46 = v25;
      if ( (int)gm.gmBlackBoxX >= *(_DWORD *)(a2 + 68) )
        v51 = *(_DWORD *)(a2 + 68);
      if ( (int)gm.gmBlackBoxY >= *(_DWORD *)(a2 + 72) )
        gmBlackBoxY = *(_DWORD *)(a2 + 72);
      if ( gmBlackBoxY > 0 )
      {
        v44 = gmBlackBoxY;
        do
        {
          if ( v51 > 0 )
          {
            v26 = v24;
            v27 = *(_DWORD *)String - (_DWORD)v24;
            v42 = v51;
            do
            {
              v28 = 255 * (unsigned __int8)(v26++)[v27];
              v29 = v42-- == 1;
              *(v26 - 1) = v28 >> v49;
            }
            while ( !v29 );
            v25 = v46;
          }
          v24 += *(_DWORD *)(a2 + 80);
          *(_DWORD *)String += v25;
          --v44;
        }
        while ( v44 );
      }
    }
    operator delete(v52);
    goto LABEL_64;
  }
  memset(*(void **)(a2 + 160), 0, *(_DWORD *)(a2 + 100));
  if ( uChar >= 0xEF40 )
  {
    if ( uChar != 61248 )
    {
      uChar = 32;
LABEL_10:
      memset(a1, 0, *(_DWORD *)(a2 + 84));
      goto LABEL_64;
    }
    v6 = 2;
    *(_DWORD *)String = 538189844;
    goto LABEL_13;
  }
  if ( uChar == 127 )
  {
    v6 = 1;
    *(_WORD *)String = 8212;
LABEL_13:
    TextOutW(*(HDC *)(a2 + 156), 0, 0, (LPCWSTR)String, v6);
    goto LABEL_14;
  }
  if ( uChar > 0xFF )
  {
    v5 = 2;
    String[0] = BYTE1(uChar);
    String[1] = uChar;
  }
  else
  {
    v5 = 1;
    String[0] = uChar;
    String[1] = 0;
  }
  TextOutA(*(HDC *)(a2 + 156), 0, 0, String, v5);
LABEL_14:
  if ( uChar == 32 || uChar == 33088 )
    goto LABEL_10;
  v7 = dword_5076A0;
  if ( dword_5076A0 <= 1 )
  {
    v13 = a1;
    v14 = (_BYTE *)(*(_DWORD *)(a2 + 160) + *(_DWORD *)(a2 + 32) + *(_DWORD *)(a2 + 96) * *(_DWORD *)(a2 + 36));
    v15 = 0;
    if ( *(int *)(a2 + 72) > 0 )
    {
      v16 = *(_DWORD *)(a2 + 68);
      do
      {
        v17 = 0;
        if ( v16 > 0 )
        {
          do
          {
            *v13 = *v14;
            ++v17;
            ++v13;
            ++v14;
          }
          while ( v17 < *(_DWORD *)(a2 + 68) );
        }
        v16 = *(_DWORD *)(a2 + 68);
        v18 = *(_DWORD *)(a2 + 96);
        if ( v16 < v18 )
          v14 += v18 - v16;
        ++v15;
      }
      while ( v15 < *(_DWORD *)(a2 + 72) );
    }
  }
  else
  {
    v8 = a1;
    v48 = a1;
    v41 = 0;
    v53 = *(_DWORD *)(a2 + 160)
        + (*(_DWORD *)(a2 + 32) << dword_5076A4)
        + *(_DWORD *)(a2 + 36) * (*(_DWORD *)(a2 + 96) << dword_5076A4);
    if ( *(int *)(a2 + 72) > 0 )
    {
      while ( 1 )
      {
        v50 = v53;
        v45 = 0;
        if ( *(int *)(a2 + 68) > 0 )
          break;
LABEL_33:
        v53 += *(_DWORD *)(a2 + 96) << dword_5076A4;
        if ( ++v41 >= *(_DWORD *)(a2 + 72) )
          goto LABEL_64;
      }
      while ( 1 )
      {
        v9 = (_BYTE *)v50;
        *(_DWORD *)String = 0;
        if ( v7 > 0 )
        {
          v43 = v7;
          do
          {
            v10 = v9;
            v11 = v7;
            do
            {
              if ( *v10++ )
                ++*(_DWORD *)String;
              --v11;
            }
            while ( v11 );
            v9 += *(_DWORD *)(a2 + 96);
            --v43;
          }
          while ( v43 );
          v8 = v48;
        }
        if ( !dword_565B64 )
          break;
        if ( dword_565B64 == 1 )
        {
          *v8 = (int)(sin((double)*(unsigned int *)String * 1.570796326794897 / (double)(v7 * v7)) * 255.0);
LABEL_31:
          v48 = ++v8;
        }
        v50 += v7;
        if ( ++v45 >= *(_DWORD *)(a2 + 68) )
          goto LABEL_33;
      }
      *v8 = (unsigned int)(255 * *(_DWORD *)String) >> dword_50769C;
      goto LABEL_31;
    }
  }
LABEL_64:
  v30 = 0;
  a3[1] = 0;
  a3[3] = *(_DWORD *)(a2 + 72) - 1;
  if ( uChar == 32 || uChar == 33088 )
  {
    *a3 = 0;
    goto LABEL_91;
  }
  *(_DWORD *)String = 0;
  if ( *(int *)(a2 + 68) <= 0 )
  {
LABEL_89:
    *a3 = 0;
LABEL_91:
    a3[2] = *(_DWORD *)(a2 + 8) / 2 - 1;
    return a3;
  }
  while ( !*(_DWORD *)String )
  {
    v31 = *(_DWORD *)(a2 + 72);
    v32 = &a1[v30];
    v33 = 0;
    if ( v31 > 0 )
    {
      while ( !*v32 )
      {
        v32 += *(_DWORD *)(a2 + 80);
        if ( ++v33 >= v31 )
          goto LABEL_73;
      }
      *a3 = v30;
      *(_DWORD *)String = 1;
    }
LABEL_73:
    if ( ++v30 >= *(_DWORD *)(a2 + 68) )
    {
      if ( !*(_DWORD *)String )
        goto LABEL_89;
      break;
    }
  }
  v34 = *(_DWORD *)(a2 + 68) - 1;
  *(_DWORD *)String = 0;
  if ( v34 >= 0 )
  {
    v55 = 2 * v34;
    while ( !*(_DWORD *)String )
    {
      v35 = *(_DWORD *)(a2 + 72);
      v36 = &a1[v34];
      v37 = 0;
      if ( v35 > 0 )
      {
        while ( !*v36 )
        {
          v36 += *(_DWORD *)(a2 + 80);
          if ( ++v37 >= v35 )
            goto LABEL_87;
        }
        a3[2] = v34;
        *(_DWORD *)String = 1;
        if ( uChar == 33089 || uChar == 33090 )
        {
          v38 = *(_DWORD *)(a2 + 68);
          v39 = v55 - *a3 + 1;
          if ( v39 >= v38 )
            v39 = v38 - 1;
          a3[2] = v39;
        }
      }
LABEL_87:
      v55 -= 2;
      if ( --v34 < 0 )
        return a3;
    }
  }
  return a3;
}

// ===== sub_42E8B0 @ 0x0042E8B0..0x0042E961 =====
int __userpurge sub_42E8B0@<eax>(_DWORD *a1@<eax>, void *a2, UINT uChar)
{
  int v4; // edx
  int v5; // eax
  _DWORD *v6; // edi
  _DWORD *v7; // ecx
  char *v8; // ecx
  _DWORD *v9; // eax
  int result; // eax
  int v11[4]; // [esp+10h] [ebp-10h] BYREF

  v4 = a1[35];
  v5 = 0;
  v6 = (_DWORD *)a1[33];
  v7 = a1 + 26;
  if ( v4 <= 0 )
  {
LABEL_4:
    if ( v4 >= a1[36] )
    {
      v6 = v7;
    }
    else
    {
      v6 = (_DWORD *)(a1[34] + 32 * v4);
      a1[35] = v4 + 1;
    }
    v8 = (char *)v6[2];
    *v6 = uChar;
    v6[1] = uChar >= 0x100;
    v9 = sub_42E370(v8, (int)a1, v11, uChar);
    v6[3] = *v9;
    v6[4] = v9[1];
    v6[5] = v9[2];
    v6[6] = v9[3];
  }
  else
  {
    while ( *v6 != uChar )
    {
      ++v5;
      v7 = v6;
      v6 = (_DWORD *)v6[7];
      if ( v5 >= v4 )
        goto LABEL_4;
    }
    v7[7] = v6[7];
  }
  result = a1[33];
  v6[7] = result;
  a1[33] = v6;
  qmemcpy(a2, v6, 0x1Cu);
  return result;
}

// ===== sub_42E970 @ 0x0042E970..0x0042E981 =====
int __userpurge sub_42E970@<eax>(void *a1@<ecx>, UINT a2@<eax>, _DWORD *a3)
{
  return sub_42E8B0(a3, a1, a2);
}

// ===== sub_42E990 @ 0x0042E990..0x0042E994 =====
int __usercall sub_42E990@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 68);
}

// ===== sub_42E9A0 @ 0x0042E9A0..0x0042E9A4 =====
int __usercall sub_42E9A0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 72);
}

// ===== sub_42E9B0 @ 0x0042E9B0..0x0042E9B4 =====
int __usercall sub_42E9B0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 80);
}

// ===== sub_42E9C0 @ 0x0042E9C0..0x0042E9D8 =====
_DWORD *__usercall sub_42E9C0@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  *result = a2[10];
  result[1] = a2[11];
  v2 = a2[12];
  v3 = a2[13];
  result[2] = v2;
  result[3] = v3;
  return result;
}

// ===== sub_42E9E0 @ 0x0042E9E0..0x0042E9F0 =====
int __userpurge sub_42E9E0@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 56) = a2;
  *(_DWORD *)(result + 60) = a3;
  return result;
}

// ===== sub_42E9F0 @ 0x0042E9F0..0x0042EA0C =====
int __fastcall sub_42E9F0(unsigned int a1, _DWORD *a2, int a3)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    *a2 = *(_DWORD *)(a3 + 4 * a1 + 56);
    return 1;
  }
  return result;
}

// ===== sub_42EA10 @ 0x0042EA10..0x0042EA2C =====
int __usercall sub_42EA10@<eax>(int *a1@<esi>)
{
  int result; // eax
  int v2; // edx
  int v3; // ecx

  result = sub_42FA80();
  v3 = (unsigned __int8)v3;
  if ( result )
    v3 = *(unsigned __int8 *)(v2 + 1) + ((unsigned __int8)v3 << 8);
  *a1 = v3;
  return result;
}

// ===== sub_42EA30 @ 0x0042EA30..0x0042EA73 =====
int __cdecl sub_42EA30(int a1)
{
  _BYTE *v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-4h] BYREF

  v1 = &unk_4E50A4;
  while ( 1 )
  {
    v2 = sub_42EA10(&v4);
    if ( v4 == a1 )
      break;
    v1 += (v2 != 0) + 1;
    if ( !*v1 )
      return 0;
  }
  return 1;
}

// ===== sub_42EA80 @ 0x0042EA80..0x0042EAAD =====
void __fastcall sub_42EA80(int a1, _BYTE *a2)
{
  _BYTE *v2; // edx
  char v3; // cl
  _BYTE *v4; // edx

  if ( *a2 )
  {
    do
    {
      if ( sub_42FA80() )
      {
        v4 = v2 + 2;
      }
      else
      {
        if ( v3 >= 65 && v3 <= 90 )
          *v2 = v3 + 32;
        v4 = v2 + 1;
      }
    }
    while ( *v4 );
  }
}

// ===== sub_42EAB0 @ 0x0042EAB0..0x0042EB29 =====
int __usercall sub_42EAB0@<eax>(const char *a1@<eax>, int a2@<edi>, int a3@<esi>, int a4)
{
  unsigned int v4; // eax

  if ( (!a1 || (v4 = strlen(a1)) == 0 || v4 >= 0x20) && !a2 )
    return -2147483644;
  if ( (a3 < 4 || a3 > 200) && (!a2 || a3) )
    return -2147483646;
  if ( (a4 < 25 || a4 > 200) && (!a2 || a4) )
    return -2147483645;
  return 0;
}

// ===== sub_42EB30 @ 0x0042EB30..0x0042EBCB =====
int __usercall sub_42EB30@<eax>(int *a1@<esi>)
{
  int v1; // eax
  int v2; // ecx
  int v4; // [esp+4h] [ebp-4h]

  v1 = *a1;
  if ( !*a1 && !a1[1] && !a1[2] && !a1[3] )
    return 0;
  if ( (v1 < 0x10000 || v1 > 0x20000) && v1 )
    return -2147483643;
  v2 = a1[1];
  if ( v2 < 0x10000 || v2 > 0x20000 )
    return -2147483643;
  v4 = *a1;
  if ( !v1 )
    v4 = a1[1];
  if ( a1[2] <= (int)(65536.0 - 4294967296.0 / (double)v4) && a1[3] <= (int)(65536.0 - 4294967296.0 / (double)v2) )
    return 0;
  return -2147483642;
}

// ===== sub_42EBD0 @ 0x0042EBD0..0x0042EC29 =====
const char *__usercall sub_42EBD0@<eax>(const char *a1@<edi>)
{
  const char **v1; // esi
  const char *result; // eax

  v1 = (const char **)dword_565B5C;
  if ( !dword_565B5C )
    return (const char *)dword_565B4C;
  while ( strcmp(a1, *v1) )
  {
    v1 = (const char **)v1[3];
    if ( !v1 )
      return (const char *)dword_565B4C;
  }
  result = v1[1];
  if ( !result )
    return (const char *)dword_565B4C;
  return result;
}

// ===== Proc @ 0x0042EC30..0x0042EC4A =====
int __stdcall Proc(const LOGFONTA *a1, const TEXTMETRICA *a2, DWORD a3, const char *a4)
{
  return _stricmp(a1->lfFaceName, a4);
}

// ===== sub_42EC50 @ 0x0042EC50..0x0042EC93 =====
int __stdcall sub_42EC50(const LOGFONTA *a1, const TEXTMETRICA *a2, DWORD a3, _DWORD *a4)
{
  CHAR *lfFaceName; // edx
  _BYTE *v5; // esi
  unsigned int v6; // edi
  CHAR v7; // al

  lfFaceName = a1->lfFaceName;
  ++*a4;
  v5 = (_BYTE *)a4[1];
  v6 = strlen(a1->lfFaceName) + 1;
  if ( v5 )
  {
    do
    {
      v7 = *lfFaceName;
      *v5++ = *lfFaceName++;
    }
    while ( v7 );
    a4[1] += v6;
  }
  a4[2] += v6;
  return 1;
}

// ===== sub_42ECA0 @ 0x0042ECA0..0x0042ECCC =====
int __stdcall sub_42ECA0(const LOGFONTA *a1, const TEXTMETRICA *a2, DWORD a3, const char **a4)
{
  const char *v5; // [esp-4h] [ebp-Ch]

  v5 = *a4;
  qmemcpy(a4 + 1, a1, 0x3Cu);
  return _stricmp(a1->lfFaceName, v5);
}

// ===== sub_42ECD0 @ 0x0042ECD0..0x0042ED11 =====
_DWORD *__usercall sub_42ECD0@<eax>(_DWORD *a1@<esi>)
{
  *a1 = &CFontManager::`vftable';
  memset(a1 + 1, 0, 0x3Cu);
  a1[9] = 0;
  a1[10] = 0;
  a1[11] = 0;
  a1[13] = 0;
  a1[15] = 0;
  a1[16] = 0;
  a1[29] = 0;
  a1[30] = 0;
  a1[43] = 0;
  a1[14] = 64;
  return a1;
}

// ===== sub_42ED20 @ 0x0042ED20..0x0042ED41 =====
void *__thiscall sub_42ED20(void *this, char a2)
{
  sub_42ED50();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_42ED50 @ 0x0042ED50..0x0042EDC3 =====
void __thiscall sub_42ED50(_DWORD *this)
{
  _DWORD *v2; // esi
  _DWORD *v3; // esi
  void *v4; // edi
  void (__thiscall ***v5)(_DWORD, int); // ecx
  _DWORD *v6; // esi
  void *v7; // [esp-4h] [ebp-Ch]
  void *v8; // [esp-4h] [ebp-Ch]

  v2 = (_DWORD *)this[15];
  *this = &CFontManager::`vftable';
  while ( v2 )
  {
    v7 = v2;
    v2 = (_DWORD *)v2[14];
    operator delete(v7);
  }
  v3 = (_DWORD *)this[30];
  while ( v3 )
  {
    v4 = v3;
    v5 = (void (__thiscall ***)(_DWORD, int))v3[13];
    v3 = (_DWORD *)v3[14];
    if ( v5 )
      (**v5)(v5, 1);
    operator delete(v4);
  }
  v6 = (_DWORD *)this[43];
  while ( v6 )
  {
    v8 = v6;
    v6 = (_DWORD *)v6[12];
    operator delete(v8);
  }
}

// ===== sub_42EDD0 @ 0x0042EDD0..0x0042EEBC =====
int __userpurge sub_42EDD0@<eax>(int a1@<eax>, int a2@<ecx>, const char *a3@<edi>, int a4, int a5, int a6)
{
  int v6; // eax
  char *v7; // eax
  int v8; // edx
  int v9; // ecx
  int v10; // edx
  int v12; // [esp+4h] [ebp-10h] BYREF
  int v13; // [esp+8h] [ebp-Ch]
  int v14; // [esp+Ch] [ebp-8h]
  int v15; // [esp+10h] [ebp-4h]

  v12 = a1;
  v13 = a2;
  v14 = a5;
  v15 = a6;
  v6 = sub_42EB30(&v12);
  if ( v6 == -2147483643 )
    return -2147483643;
  if ( v6 == -2147483642 )
    return -2147483642;
  v7 = *(char **)(a4 + 172);
  if ( v7 )
  {
    while ( strcmp(a3, v7) )
    {
      v7 = (char *)*((_DWORD *)v7 + 12);
      if ( !v7 )
        goto LABEL_8;
    }
  }
  else
  {
LABEL_8:
    v7 = (char *)operator new(0x34u);
    strcpy(v7, a3);
    *((_DWORD *)v7 + 12) = *(_DWORD *)(a4 + 172);
    *(_DWORD *)(a4 + 172) = v7;
  }
  v8 = v13;
  *((_DWORD *)v7 + 8) = v12;
  v9 = v14;
  *((_DWORD *)v7 + 9) = v8;
  v10 = v15;
  *((_DWORD *)v7 + 10) = v9;
  *((_DWORD *)v7 + 11) = v10;
  return 0;
}

// ===== sub_42EEC0 @ 0x0042EEC0..0x0042EF0D =====
int __usercall sub_42EEC0@<eax>(int a1@<ecx>, const char *a2@<edi>)
{
  int v2; // esi
  int result; // eax

  v2 = *(_DWORD *)(a1 + 172);
  result = 0;
  if ( v2 )
  {
    while ( strcmp(a2, (const char *)v2) )
    {
      v2 = *(_DWORD *)(v2 + 48);
      if ( !v2 )
        return result;
    }
    return v2 + 32;
  }
  return result;
}

// ===== sub_42EF10 @ 0x0042EF10..0x0042EF56 =====
int __userpurge sub_42EF10@<eax>(int a1@<ecx>, int a2@<eax>, int a3@<edi>, LPCSTR pszFaceName, int a5, int a6, int a7)
{
  int result; // eax
  _BYTE v8[48]; // [esp+4h] [ebp-34h] BYREF
  int v9; // [esp+34h] [ebp-4h]

  result = sub_42F0F0(pszFaceName, a3, (int)&a5, a5, a1, a2);
  if ( !result )
  {
    sub_42F2D0(v8);
    sub_42E9E0(v9, a6, a7);
    return 0;
  }
  return result;
}

// ===== sub_42EF60 @ 0x0042EF60..0x0042F06D =====
int __userpurge sub_42EF60@<eax>(const char *a1@<edi>, int a2, int a3, int a4, int a5, int a6)
{
  char *v7; // eax

  if ( a1 )
  {
    if ( (unsigned int)(a3 - 4) > 0xC4 )
      return -2147483646;
    if ( (unsigned int)(a4 - 25) > 0xAF )
      return -2147483645;
    if ( a6 < 2 )
      return -2147483647;
  }
  v7 = (char *)(a2 + 4);
  if ( a1 )
  {
    if ( a2 != -4 )
    {
      do
      {
        if ( !strcmp(a1, v7) && a3 == *((_DWORD *)v7 + 8) && a4 == *((_DWORD *)v7 + 9) )
        {
          if ( a5 )
          {
            if ( *((_DWORD *)v7 + 10) )
              goto LABEL_15;
          }
          else if ( !*((_DWORD *)v7 + 10) )
          {
            goto LABEL_15;
          }
        }
        v7 = (char *)*((_DWORD *)v7 + 14);
      }
      while ( v7 );
    }
    goto LABEL_16;
  }
LABEL_15:
  if ( !v7 )
  {
LABEL_16:
    v7 = (char *)operator new(0x3Cu);
    strcpy(v7, a1);
    *((_DWORD *)v7 + 8) = a3;
    *((_DWORD *)v7 + 9) = a4;
    *((_DWORD *)v7 + 10) = a5;
    *((_DWORD *)v7 + 12) = 0;
    *((_DWORD *)v7 + 14) = *(_DWORD *)(a2 + 60);
    *(_DWORD *)(a2 + 60) = v7;
  }
  *((_DWORD *)v7 + 13) = a6;
  return 0;
}

// ===== sub_42F070 @ 0x0042F070..0x0042F0E5 =====
int __userpurge sub_42F070@<eax>(int a1@<ecx>, const char *a2@<edi>, int a3, int a4, int a5)
{
  int result; // eax
  int i; // esi

  result = *(_DWORD *)(a1 + 56);
  for ( i = *(_DWORD *)(a1 + 60); i; i = *(_DWORD *)(i + 56) )
  {
    if ( !strcmp(a2, (const char *)i) && a3 == *(_DWORD *)(i + 32) && a4 == *(_DWORD *)(i + 36) )
    {
      if ( a5 )
      {
        if ( *(_DWORD *)(i + 40) )
          return *(_DWORD *)(i + 52);
      }
      else if ( !*(_DWORD *)(i + 40) )
      {
        return *(_DWORD *)(i + 52);
      }
    }
  }
  return result;
}

// ===== sub_42F0F0 @ 0x0042F0F0..0x0042F2C8 =====
int __thiscall sub_42F0F0(LPCSTR pszFaceName, int a2, _DWORD *a3, int a4, int a5, int a6)
{
  int v7; // esi
  _DWORD *v8; // ebx
  _DWORD *v9; // eax
  int v11; // eax
  unsigned int v12; // eax
  int v13; // esi
  char *v14; // eax
  int v15; // ecx
  int v16; // [esp-8h] [ebp-2Ch]
  void (__thiscall ***v17)(_DWORD, int); // [esp+14h] [ebp-10h]

  *a3 = 0;
  v7 = *(_DWORD *)(a2 + 120);
  v8 = (_DWORD *)(a2 + 64);
  if ( !v7 )
  {
LABEL_7:
    v9 = operator new(0xACu);
    if ( v9 )
      v17 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v9);
    else
      v17 = 0;
    v16 = sub_42F070(a2, pszFaceName, a4, a5, a6);
    v11 = sub_42EEC0(a2, pszFaceName);
    v12 = sub_42DDF0((int)v17, pszFaceName, a4, a5, a6, 0, v11, v16, 1);
    if ( v12 > 0x80000002 )
    {
      if ( v12 == -2147483645 )
      {
        v13 = -2147483645;
        goto LABEL_23;
      }
      if ( v12 == -2147483644 )
      {
        v13 = -2147483644;
        goto LABEL_23;
      }
    }
    else
    {
      switch ( v12 )
      {
        case 0x80000002:
          v13 = -2147483646;
          goto LABEL_23;
        case 0u:
          v14 = (char *)operator new(0x3Cu);
          v15 = *v8 + 1;
          v8[14] = v14;
          *(_DWORD *)v14 = v15;
          strcpy(v14 + 4, pszFaceName);
          *((_DWORD *)v14 + 9) = a4;
          *((_DWORD *)v14 + 11) = a6;
          *((_DWORD *)v14 + 10) = a5;
          *((_DWORD *)v14 + 13) = v17;
          *((_DWORD *)v14 + 12) = 0;
          *((_DWORD *)v14 + 14) = 0;
          *a3 = *(_DWORD *)v14;
          return 0;
        case 0x80000001:
          v13 = -2147483647;
          goto LABEL_23;
      }
    }
    v13 = a4;
    if ( !a4 )
      return v13;
LABEL_23:
    if ( v17 )
      (**v17)(v17, 1);
    return v13;
  }
  while ( strcmp(pszFaceName, (const char *)(v7 + 4))
       || a4 != *(_DWORD *)(v7 + 36)
       || a5 != *(_DWORD *)(v7 + 40)
       || a6 != *(_DWORD *)(v7 + 44) )
  {
    v8 = (_DWORD *)v7;
    v7 = *(_DWORD *)(v7 + 56);
    if ( !v7 )
      goto LABEL_7;
  }
  *a3 = *(_DWORD *)v7;
  return 0;
}

// ===== sub_42F2D0 @ 0x0042F2D0..0x0042F309 =====
int __fastcall sub_42F2D0(int a1, int a2, void *a3)
{
  _DWORD *v3; // ecx
  int result; // eax

  v3 = *(_DWORD **)(a1 + 120);
  result = 0;
  if ( v3 )
  {
    while ( a2 != *v3 )
    {
      v3 = (_DWORD *)v3[14];
      if ( !v3 )
        return result;
    }
    qmemcpy(a3, v3 + 1, 0x34u);
    return 1;
  }
  return result;
}

// ===== sub_42F310 @ 0x0042F310..0x0042F3E4 =====
int __stdcall sub_42F310(int a1)
{
  int result; // eax
  int i; // esi
  void (__thiscall ***v3)(_DWORD, int); // ecx
  _DWORD *v4; // eax
  int v5; // ebx
  int v6; // eax
  int v7; // eax
  int v8; // [esp-8h] [ebp-2Ch]
  int v9; // [esp+14h] [ebp-10h]

  result = a1;
  for ( i = *(_DWORD *)(a1 + 120); i; i = *(_DWORD *)(i + 56) )
  {
    v3 = *(void (__thiscall ****)(_DWORD, int))(i + 52);
    if ( v3 )
      (**v3)(v3, 1);
    v4 = operator new(0xACu);
    if ( v4 )
      v9 = sub_42D6A0(v4);
    else
      v9 = 0;
    v5 = *(_DWORD *)(i + 44);
    v6 = *(_DWORD *)(i + 40);
    *(_DWORD *)(i + 52) = v9;
    v8 = sub_42F070(a1, (const char *)(i + 4), *(_DWORD *)(i + 36), v6, v5);
    v7 = sub_42EEC0(a1, (const char *)(i + 4));
    result = sub_42DDF0(v9, (LPCSTR)(i + 4), *(_DWORD *)(i + 36), *(_DWORD *)(i + 40), v5, 0, v7, v8, 1);
  }
  return result;
}

// ===== sub_42F3F0 @ 0x0042F3F0..0x0042F424 =====
_DWORD *__usercall sub_42F3F0@<eax>(_DWORD *a1@<esi>)
{
  *a1 = 0;
  a1[2] = operator new(1u);
  a1[3] = operator new(1u);
  a1[14] = operator new(0x20u);
  a1[17] = operator new(0x1Cu);
  return a1;
}

// ===== sub_42F430 @ 0x0042F430..0x0042F461 =====
void __usercall sub_42F430(void **a1@<eax>)
{
  sub_42F470();
  operator delete(a1[2]);
  operator delete(a1[3]);
  operator delete(a1[14]);
  operator delete(a1[17]);
}

// ===== sub_42F470 @ 0x0042F470..0x0042F4E6 =====
BOOL __usercall sub_42F470@<eax>(int a1@<esi>)
{
  BOOL result; // eax

  if ( *(_DWORD *)a1 )
  {
    SetBkMode(*(HDC *)(a1 + 84), *(_DWORD *)(a1 + 116));
    SetTextColor(*(HDC *)(a1 + 84), *(_DWORD *)(a1 + 112));
    SelectPalette(*(HDC *)(a1 + 84), *(HPALETTE *)(a1 + 108), 0);
    SelectObject(*(HDC *)(a1 + 84), *(HGDIOBJ *)(a1 + 104));
    SelectObject(*(HDC *)(a1 + 84), *(HGDIOBJ *)(a1 + 100));
    DeleteObject(*(HGDIOBJ *)(a1 + 76));
    DeleteObject(*(HGDIOBJ *)(a1 + 80));
    DeleteObject(*(HGDIOBJ *)(a1 + 88));
    result = DeleteDC(*(HDC *)(a1 + 84));
    *(_DWORD *)a1 = 0;
  }
  return result;
}

// ===== sub_42F4F0 @ 0x0042F4F0..0x0042F544 =====
int __userpurge sub_42F4F0@<eax>(_DWORD *a1@<eax>, int a2@<edi>, const CHAR *pszFaceName, int a4)
{
  int result; // eax

  sub_42F470((int)a1);
  if ( (unsigned int)(a2 - 8) > 0xC0 )
    return -2147483646;
  sub_42F550(a2);
  if ( !sub_42F5F0(pszFaceName, a2) )
    return -2147483644;
  result = 0;
  a1[18] = 0;
  *a1 = 1;
  return result;
}

// ===== sub_42F550 @ 0x0042F550..0x0042F5EA =====
int __usercall sub_42F550@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int v2; // eax
  void *v3; // eax
  void *v4; // eax
  int v5; // ecx
  int result; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  void *v10; // [esp-Ch] [ebp-Ch]

  v2 = (a1 + 7) / 8;
  *(_DWORD *)(a2 + 16) = v2;
  *(_DWORD *)(a2 + 20) = a1 * v2;
  operator delete(*(void **)(a2 + 8));
  v3 = operator new(16 * *(_DWORD *)(a2 + 20));
  v10 = *(void **)(a2 + 56);
  *(_DWORD *)(a2 + 8) = v3;
  operator delete(v10);
  v4 = operator new(0x200u);
  v5 = 0;
  *(_DWORD *)(a2 + 56) = v4;
  result = *(_DWORD *)(a2 + 8);
  *(_DWORD *)(a2 + 64) = 16;
  *(_DWORD *)(a2 + 60) = 0;
  do
  {
    *(_DWORD *)(v5 + *(_DWORD *)(a2 + 56) + 8) = result;
    v7 = *(_DWORD *)(a2 + 20) + result;
    *(_DWORD *)(v5 + *(_DWORD *)(a2 + 56) + 40) = v7;
    v8 = *(_DWORD *)(a2 + 20) + v7;
    *(_DWORD *)(v5 + *(_DWORD *)(a2 + 56) + 72) = v8;
    v9 = *(_DWORD *)(a2 + 20) + v8;
    *(_DWORD *)(v5 + *(_DWORD *)(a2 + 56) + 104) = v9;
    result = *(_DWORD *)(a2 + 20) + v9;
    v5 += 128;
  }
  while ( v5 < 512 );
  return result;
}

// ===== sub_42F5F0 @ 0x0042F5F0..0x0042F789 =====
BOOL __userpurge sub_42F5F0@<eax>(LPCSTR pszFaceName@<ecx>, int a2@<eax>, int a3@<esi>, int cHeight)
{
  int v5; // eax
  HFONT FontA; // eax
  LOGPALETTE *v7; // edi
  BITMAPINFO *v8; // edi
  HDC CompatibleDC; // eax
  HGDIOBJ v10; // eax
  void *v11; // edx
  HGDIOBJ v12; // eax
  COLORREF v13; // eax
  HDC v15; // [esp-Ch] [ebp-30h]
  HDC v16; // [esp-8h] [ebp-2Ch]
  HPALETTE v17; // [esp-8h] [ebp-2Ch]
  HDC v18; // [esp-8h] [ebp-2Ch]
  void *v20; // [esp-4h] [ebp-28h]
  _BYTE pv[12]; // [esp+8h] [ebp-1Ch] BYREF
  int v22; // [esp+14h] [ebp-10h]

  v5 = sub_42DA70((LPARAM)pszFaceName);
  FontA = CreateFontA(
            cHeight,
            cHeight / 2,
            0,
            0,
            a2 != 0 ? 700 : 400,
            0,
            0,
            0,
            v5 != 0 ? 0x80 : 0,
            0,
            0,
            1u,
            1u,
            pszFaceName);
  *(_DWORD *)(a3 + 76) = FontA;
  if ( FontA )
  {
    v7 = (LOGPALETTE *)operator new(0xCu);
    v7->palVersion = 768;
    v7->palNumEntries = 2;
    v7->palPalEntry[0] = 0;
    *(_DWORD *)&v7[1].palVersion = 1;
    v7->palPalEntry[0].peFlags = 2;
    HIBYTE(v7[1].palNumEntries) = 2;
    *(_DWORD *)(a3 + 80) = CreatePalette(v7);
    operator delete(v7);
    v8 = (BITMAPINFO *)operator new(0x30u);
    memset(v8, 0, 0x30u);
    v8->bmiHeader.biWidth = ((((cHeight + 31) >> 31) & 0x1F) + cHeight + 31) & 0xFFFFFFE0;
    v8->bmiHeader.biSize = 40;
    v8->bmiHeader.biHeight = -cHeight;
    *(_DWORD *)&v8->bmiHeader.biPlanes = 65537;
    v8->bmiHeader.biCompression = 0;
    v8->bmiHeader.biSizeImage = 0;
    v8->bmiHeader.biXPelsPerMeter = 0;
    v8->bmiHeader.biYPelsPerMeter = 0;
    v8->bmiHeader.biClrUsed = 0;
    v8->bmiHeader.biClrImportant = 0;
    v8->bmiColors[0] = 0;
    v8[1].bmiHeader.biSize = 1;
    CompatibleDC = CreateCompatibleDC(0);
    *(_DWORD *)(a3 + 84) = CompatibleDC;
    *(_DWORD *)(a3 + 88) = CreateDIBSection(CompatibleDC, v8, 1u, (void **)(a3 + 92), 0, 0);
    operator delete(v8);
    GetObjectA(*(HANDLE *)(a3 + 88), 24, pv);
    v20 = *(void **)(a3 + 88);
    v16 = *(HDC *)(a3 + 84);
    *(_DWORD *)(a3 + 96) = v22;
    v10 = SelectObject(v16, v20);
    v11 = *(void **)(a3 + 76);
    *(_DWORD *)(a3 + 100) = v10;
    v12 = SelectObject(*(HDC *)(a3 + 84), v11);
    v17 = *(HPALETTE *)(a3 + 80);
    v15 = *(HDC *)(a3 + 84);
    *(_DWORD *)(a3 + 104) = v12;
    *(_DWORD *)(a3 + 108) = SelectPalette(v15, v17, 0);
    v13 = SetTextColor(*(HDC *)(a3 + 84), 0x1000001u);
    v18 = *(HDC *)(a3 + 84);
    *(_DWORD *)(a3 + 112) = v13;
    *(_DWORD *)(a3 + 116) = SetBkMode(v18, 1);
    *(_DWORD *)(a3 + 4) = cHeight;
  }
  return *(_DWORD *)(a3 + 76) != 0;
}

// ===== sub_42F790 @ 0x0042F790..0x0042F94E =====
_DWORD *__userpurge sub_42F790@<eax>(_BYTE *a1@<eax>, _DWORD *a2@<edi>, int a3, int String)
{
  unsigned int v4; // ebx
  _DWORD *v6; // ebx
  int v7; // eax
  int v8; // edx
  int v9; // ecx
  int i; // eax
  int v11; // eax
  int v12; // edx
  int v13; // ecx
  int v14; // esi
  int v15; // edx
  int v16; // eax
  unsigned __int8 v17; // bl
  unsigned __int8 v18; // cl
  int v19; // esi
  int v20; // ebx
  int v21; // eax
  int v22; // ecx
  int v23; // eax
  HDC v25; // [esp-14h] [ebp-28h]
  int v26; // [esp+8h] [ebp-Ch]
  int v27; // [esp+Ch] [ebp-8h]
  int v28; // [esp+10h] [ebp-4h]

  v4 = String;
  memset(*(void **)(a3 + 92), 0, *(_DWORD *)(a3 + 96) * *(_DWORD *)(a3 + 4));
  if ( v4 >= 0xEF40 )
  {
    if ( v4 == 61248 )
    {
      v7 = 2;
      String = 538189844;
    }
    else
    {
      v7 = a3;
    }
    goto LABEL_10;
  }
  if ( v4 == 127 )
  {
    v7 = 1;
    LOWORD(String) = 8212;
LABEL_10:
    TextOutW(*(HDC *)(a3 + 84), 0, 0, (LPCWSTR)&String, v7);
    goto LABEL_11;
  }
  if ( v4 < 0x100 )
  {
    LOBYTE(String) = v4;
    v6 = (_DWORD *)a3;
    TextOutA(*(HDC *)(a3 + 84), 0, 0, (LPCSTR)&String, 1);
    goto LABEL_12;
  }
  LOBYTE(String) = BYTE1(v4);
  v25 = *(HDC *)(a3 + 84);
  BYTE1(String) = v4;
  TextOutA(v25, 0, 0, (LPCSTR)&String, 2);
LABEL_11:
  v6 = (_DWORD *)a3;
LABEL_12:
  v8 = v6[23];
  String = v6[1];
  if ( String )
  {
    v9 = v6[4];
    do
    {
      --String;
      for ( i = 0; i < v9; ++a1 )
      {
        *a1 = *(_BYTE *)(i + v8);
        v9 = v6[4];
        ++i;
      }
      v8 += v6[24];
    }
    while ( String );
  }
  v11 = v6[1];
  v12 = v6[23];
  a2[3] = v11 - 1;
  *a2 = v11 - 1;
  v13 = v11;
  a2[1] = 0;
  a2[2] = 0;
  v28 = v12;
  if ( v11 )
  {
    v14 = v6[4];
    v27 = v14;
    v26 = v6[24];
    do
    {
      --v13;
      v15 = 0;
      v16 = 0;
      String = v13;
      if ( v14 > 0 )
      {
        do
        {
          v17 = *(_BYTE *)(v15 + v28);
          v18 = 0x80;
          v19 = 8;
          do
          {
            --v19;
            if ( (v17 & v18) != 0 )
            {
              if ( v16 < *a2 )
                *a2 = v16;
              if ( v16 > a2[2] )
                a2[2] = v16;
            }
            v18 >>= 1;
            ++v16;
          }
          while ( v19 );
          v14 = v27;
          ++v15;
        }
        while ( v15 < v27 );
        v6 = (_DWORD *)a3;
        v13 = String;
      }
      v28 += v26;
    }
    while ( v13 );
  }
  v20 = v6[1];
  v21 = v20 / 8;
  if ( v20 / 8 <= 0 )
    v21 = 1;
  v22 = *a2 - v21;
  v23 = a2[2] + v21;
  *a2 = v22 <= 0 ? 0 : v22;
  if ( v23 >= v20 - 1 )
    a2[2] = v20 - 1;
  else
    a2[2] = v23;
  return a2;
}

// ===== sub_42F950 @ 0x0042F950..0x0042F9A5 =====
BOOL __userpurge sub_42F950@<eax>(int a1@<eax>, int a2@<ecx>, void *a3)
{
  int v3; // edx
  int v5; // eax
  _DWORD *v6; // esi
  _DWORD *i; // ecx

  v3 = *(_DWORD *)(a2 + 72);
  v5 = 0;
  if ( v3 > 0 )
  {
    v6 = *(_DWORD **)(a2 + 68);
    for ( i = v6; a1 != *i; i += 7 )
    {
      if ( ++v5 >= v3 )
        return 0;
    }
    qmemcpy(a3, &v6[7 * v5], 0x1Cu);
  }
  return v5 < v3;
}

// ===== sub_42F9B0 @ 0x0042F9B0..0x0042FA49 =====
unsigned int *__userpurge sub_42F9B0@<eax>(unsigned int a1@<eax>, _DWORD *a2, unsigned int *a3)
{
  int v3; // edx
  unsigned int *v4; // esi
  int v6; // eax
  unsigned int *v7; // ecx
  unsigned int *result; // eax
  int v9[4]; // [esp+Ch] [ebp-10h] BYREF

  v3 = a2[15];
  v4 = (unsigned int *)a2[13];
  v6 = 0;
  v7 = a2 + 6;
  if ( v3 <= 0 )
  {
LABEL_4:
    if ( v3 >= a2[16] )
    {
      v4 = v7;
    }
    else
    {
      v4 = (unsigned int *)(a2[14] + 32 * v3);
      a2[15] = v3 + 1;
    }
    *v4 = a1;
    v4[1] = a1 >= 0x100;
    result = sub_42F790((_BYTE *)v4[2], v9, (int)a2, a1);
    v4[3] = *result;
    v4[4] = result[1];
    v4[5] = result[2];
    v4[6] = result[3];
  }
  else
  {
    while ( *v4 != a1 )
    {
      ++v6;
      v7 = v4;
      v4 = (unsigned int *)v4[7];
      if ( v6 >= v3 )
        goto LABEL_4;
    }
    result = (unsigned int *)v4[7];
    v7[7] = (unsigned int)result;
  }
  v4[7] = a2[13];
  a2[13] = v4;
  qmemcpy(a3, v4, 0x1Cu);
  return result;
}

// ===== sub_42FA50 @ 0x0042FA50..0x0042FA73 =====
unsigned int *__userpurge sub_42FA50@<eax>(void *a1@<edi>, unsigned int a2@<esi>, _DWORD *a3)
{
  unsigned int *result; // eax

  result = (unsigned int *)sub_42F950(a2, (int)a3, a1);
  if ( !result )
    return sub_42F9B0(a2, a3, (unsigned int *)a1);
  return result;
}

// ===== sub_42FA80 @ 0x0042FA80..0x0042FA97 =====
BOOL __usercall sub_42FA80@<eax>(unsigned __int8 a1@<al>)
{
  if ( a1 < 0x80u )
    return 0;
  if ( a1 >= 0xA0u )
    return a1 >= 0xE0u;
  return 1;
}

// ===== sub_42FAA0 @ 0x0042FAA0..0x0042FABF =====
void *sub_42FAA0()
{
  void *result; // eax

  result = memset(xmmword_50C900, 0, sizeof(xmmword_50C900));
  dword_565B68 = 1;
  return result;
}

// ===== sub_42FAC0 @ 0x0042FAC0..0x0042FAFB =====
int sub_42FAC0()
{
  int v0; // edi
  __m128i *v1; // esi
  int result; // eax

  if ( dword_565B68 )
  {
    v0 = 0;
    v1 = xmmword_50C900;
    do
    {
      if ( v1->m128i_i32[0] )
        result = sub_42FC40(v0 - 0x1000000);
      v1 = (__m128i *)((char *)v1 + 40);
      ++v0;
    }
    while ( (int)v1 < (int)&dword_50CA40 );
    dword_565B68 = 0;
  }
  return result;
}

// ===== sub_42FB00 @ 0x0042FB00..0x0042FBD8 =====
int __cdecl sub_42FB00(_DWORD *a1, LPCSTR lpWindowName, int X, int Y, int a5, int a6)
{
  int v6; // ecx
  __m128i *v7; // eax
  HWND *v9; // edi
  HWND Window; // eax

  if ( (unsigned int)(a5 - 32) > 0x7E0 || (unsigned int)(a6 - 32) > 0x7E0 )
    return -2147483647;
  v6 = 0;
  v7 = xmmword_50C900;
  while ( v7->m128i_i32[0] )
  {
    v7 = (__m128i *)((char *)v7 + 40);
    ++v6;
    if ( (int)v7 >= (int)&dword_50CA40 )
      return -2147483646;
  }
  v9 = (HWND *)xmmword_50C900 + 10 * v6;
  *a1 = v6 - 0x1000000;
  if ( !v9 )
    return -2147483646;
  sub_409080(v9 + 3, 0);
  sub_40A710(v9 + 3, 0, 0);
  Window = CreateWindowExA(
             0,
             ClassName,
             lpWindowName,
             0x80CA0000,
             X,
             Y,
             a5 + dword_517F14,
             a6 + dword_517B04,
             hWndParent,
             0,
             hInst,
             0);
  v9[1] = 0;
  v9[2] = 0;
  v9[9] = 0;
  *v9 = Window;
  return 0;
}

// ===== sub_42FBE0 @ 0x0042FBE0..0x0042FC08 =====
char *__fastcall sub_42FBE0(int a1)
{
  char *result; // eax
  unsigned int v2; // ecx

  result = 0;
  if ( (a1 & 0xFF000000) == 0xFF000000 )
  {
    v2 = a1 & 0xFFFFFF;
    if ( v2 < 8 )
      return (char *)xmmword_50C900 + 40 * v2;
  }
  return result;
}

// ===== sub_42FC10 @ 0x0042FC10..0x0042FC3C =====
char *__usercall sub_42FC10@<eax>(int a1@<esi>)
{
  char *result; // eax
  int v2; // edx
  __m128i *v3; // ecx

  result = 0;
  v2 = 0;
  v3 = xmmword_50C900;
  while ( a1 != v3->m128i_i32[0] )
  {
    v3 = (__m128i *)((char *)v3 + 40);
    ++v2;
    if ( (int)v3 >= (int)&dword_50CA40 )
      return result;
  }
  return (char *)xmmword_50C900 + 40 * v2;
}

// ===== sub_42FC40 @ 0x0042FC40..0x0042FC6B =====
BOOL __thiscall sub_42FC40(void *this)
{
  char *v1; // eax
  char *v2; // esi
  HWND v4; // [esp-10h] [ebp-14h]

  v1 = sub_42FBE0((int)this);
  v2 = v1;
  if ( v1 )
  {
    v4 = *(HWND *)v1;
    *((_DWORD *)v1 + 2) = 1;
    SendMessageA(v4, 0x10u, 0, 0);
  }
  return v2 != 0;
}

// ===== sub_42FC70 @ 0x0042FC70..0x0042FCC0 =====
BOOL __usercall sub_42FC70@<eax>(int a1@<eax>, int a2@<ecx>)
{
  char *v3; // eax
  HWND *v4; // esi

  v3 = sub_42FBE0(a2);
  v4 = (HWND *)v3;
  if ( !v3 )
    return v4 != 0;
  if ( a1 )
  {
    if ( *((_DWORD *)v3 + 1) )
      return v3 != 0;
    goto LABEL_6;
  }
  if ( *((_DWORD *)v3 + 1) )
  {
LABEL_6:
    *((_DWORD *)v3 + 1) = a1;
    sub_45FFB0(a1);
    ShowWindow(*v4, a1 != 0 ? 8 : 0);
  }
  return v4 != 0;
}

// ===== sub_42FCC0 @ 0x0042FCC0..0x0042FD00 =====
BOOL __cdecl sub_42FCC0(int X, int Y)
{
  int v2; // ecx
  char *v3; // eax
  char *v4; // esi

  v3 = sub_42FBE0(v2);
  v4 = v3;
  if ( v3 )
    MoveWindow(*(HWND *)v3, X, Y, dword_517F14 + *((_DWORD *)v3 + 5), dword_517B04 + *((_DWORD *)v3 + 6), 1);
  return v4 != 0;
}

// ===== sub_42FD00 @ 0x0042FD00..0x0042FD4A =====
BOOL __usercall sub_42FD00@<eax>(int a1@<ecx>, LONG *a2@<edi>)
{
  char *v2; // eax
  char *v3; // esi
  LONG top; // eax
  tagRECT Rect; // [esp+4h] [ebp-14h] BYREF

  v2 = sub_42FBE0(a1);
  v3 = v2;
  if ( v2 )
  {
    GetWindowRect(*(HWND *)v2, &Rect);
    top = Rect.top;
    *a2 = Rect.left;
    a2[1] = top;
  }
  return v3 != 0;
}

// ===== sub_42FD50 @ 0x0042FD50..0x0042FD76 =====
BOOL __cdecl sub_42FD50(LPCSTR lpString)
{
  int v1; // ecx
  char *v2; // esi

  v2 = sub_42FBE0(v1);
  if ( v2 )
    SetWindowTextA(*(HWND *)v2, lpString);
  return v2 != 0;
}

// ===== sub_42FD80 @ 0x0042FD80..0x0042FDDB =====
BOOL __cdecl sub_42FD80(int Val)
{
  int v1; // ecx
  char *v2; // eax
  HWND *v3; // esi
  HDC DC; // ebx

  v2 = sub_42FBE0(v1);
  v3 = (HWND *)v2;
  if ( v2 )
  {
    sub_40A710((_DWORD *)v2 + 3, 0, Val);
    DC = GetDC(*v3);
    sub_4617E0(DC, 0, 0, v3 + 3, 0);
    ReleaseDC(*v3, DC);
  }
  return v3 != 0;
}

// ===== sub_42FDE0 @ 0x0042FDE0..0x0042FEC1 =====
int __cdecl sub_42FDE0(int a1, int a2, int a3, int a4, int a5)
{
  int v5; // ecx
  char *v6; // ebx
  HDC DC; // edi
  int result; // eax
  int v9[6]; // [esp+10h] [ebp-18h] BYREF

  v6 = sub_42FBE0(v5);
  if ( !v6 )
    return -1;
  if ( !sub_407F20((int)dword_566750, a3, v9) )
    return -2147483645;
  switch ( sub_40A530(v9, (_DWORD *)v6 + 3, a2, a1, a4, a5) )
  {
    case 0:
      DC = GetDC(*(HWND *)v6);
      sub_4617E0(DC, 0, 0, v6 + 12, 0);
      ReleaseDC(*(HWND *)v6, DC);
      result = 0;
      break;
    case 1:
      result = -2147483644;
      break;
    case 2:
      result = -2147483643;
      break;
    case 3:
      result = -2147483642;
      break;
    case 4:
      result = -2147483641;
      break;
    default:
      result = -2147483640;
      break;
  }
  return result;
}

// ===== sub_42FEE0 @ 0x0042FEE0..0x0042FFC4 =====
int __cdecl sub_42FEE0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  int v10; // ecx
  char *v11; // esi
  const CHAR *v12; // eax
  unsigned int v13; // eax
  HDC DC; // ebx
  int v16; // [esp+4h] [ebp-4h] BYREF

  v11 = sub_42FBE0(v10);
  if ( !v11 )
    return -1;
  v12 = (const CHAR *)sub_468BB0();
  v13 = sub_409290(a6, a7, (int)dword_566750, (int)&v16, v12, a5);
  if ( v13 > 0x80000003 )
  {
    if ( v13 == -2147483644 )
      return -2147483637;
  }
  else
  {
    switch ( v13 )
    {
      case 0x80000003:
        return -2147483638;
      case 0u:
        sub_4097D0(a8, 0, (int)dword_566750, (int)(v11 + 12), a10, a1, a2, a3, v16, a9);
        DC = GetDC(*(HWND *)v11);
        sub_4617E0(DC, 0, 0, v11 + 12, 0);
        ReleaseDC(*(HWND *)v11, DC);
        return 0;
      case 0x80000002:
        return -2147483639;
    }
  }
  return v16;
}

// ===== sub_42FFD0 @ 0x0042FFD0..0x00430023 =====
int __usercall sub_42FFD0@<eax>(int a1@<ecx>, const char *a2@<edi>)
{
  char *v2; // esi
  _BYTE *v3; // eax
  const char *v4; // ecx
  _BYTE *v5; // edx
  char v6; // al

  v2 = sub_42FBE0(a1);
  if ( !v2 )
    return -1;
  operator delete(*((void **)v2 + 9));
  v3 = operator new[](strlen(a2) + 1);
  *((_DWORD *)v2 + 9) = v3;
  v4 = a2;
  v5 = v3;
  do
  {
    v6 = *v4;
    *v5++ = *v4++;
  }
  while ( v6 );
  return 0;
}
