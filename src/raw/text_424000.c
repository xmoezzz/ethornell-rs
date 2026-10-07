#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_424140 @ 0x00424140..0x004242B0 =====
int __userpurge sub_424140@<eax>(_DWORD *a1@<edi>, int a2)
{
  int v2; // ebx
  int v3; // edx
  _DWORD *v4; // eax
  int v5; // esi
  int v6; // ecx
  int v7; // edx
  int v8; // esi
  bool v9; // zf
  int v11; // [esp+8h] [ebp-28h]
  int v12; // [esp+Ch] [ebp-24h]
  int v13; // [esp+10h] [ebp-20h]
  int v14; // [esp+14h] [ebp-1Ch]
  int v15; // [esp+18h] [ebp-18h] BYREF
  int v16; // [esp+1Ch] [ebp-14h]
  unsigned int v17; // [esp+20h] [ebp-10h]
  unsigned int v18; // [esp+24h] [ebp-Ch]
  int v19; // [esp+28h] [ebp-8h]
  int v20; // [esp+2Ch] [ebp-4h]

  if ( !a1[77] || !sub_407F20(dword_565B30, a2, &v15) )
    return 0;
  v11 = v17 / a1[82];
  v2 = v18 / a1[83];
  if ( !v11 || !(v18 / a1[83]) || !sub_409210((int)(a1 + 84), (int)&v15) )
    return 0;
  sub_4242B0();
  v3 = dword_565B30;
  a1[106] = 1;
  a1[107] = a2;
  a1[108] = sub_408300(a2, v3);
  a1[109] = v11 * v2;
  v4 = operator new[](24 * v11 * v2);
  v5 = v16 * a1[83];
  v6 = v15;
  v7 = v20 * a1[82];
  a1[110] = v4;
  v12 = v6;
  v14 = v5;
  if ( v2 )
  {
    v13 = v2;
    do
    {
      v8 = v11;
      do
      {
        v4[1] = v16;
        *v4 = v6;
        v4[2] = a1[82];
        v4[3] = a1[83];
        v4[4] = v19;
        v4[5] = v20;
        v4 += 6;
        v6 += v7;
        --v8;
      }
      while ( v8 );
      v6 = v14 + v12;
      v9 = v13-- == 1;
      v12 += v14;
    }
    while ( !v9 );
  }
  return 1;
}

// ===== sub_4242B0 @ 0x004242B0..0x004242E2 =====
int __usercall sub_4242B0@<eax>(int a1@<esi>)
{
  int v1; // edi

  v1 = *(_DWORD *)(a1 + 424);
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 440));
    *(_DWORD *)(a1 + 440) = 0;
    *(_DWORD *)(a1 + 424) = 0;
  }
  return v1;
}

// ===== sub_4242F0 @ 0x004242F0..0x00424342 =====
int __usercall sub_4242F0@<eax>(int a1@<esi>)
{
  int v1; // edi
  void *v3; // [esp-Ch] [ebp-14h]
  void *v4; // [esp-8h] [ebp-10h]

  v1 = *(_DWORD *)(a1 + 308);
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 336));
    v4 = *(void **)(a1 + 360);
    *(_DWORD *)(a1 + 336) = 0;
    operator delete[](v4);
    v3 = *(void **)(a1 + 364);
    *(_DWORD *)(a1 + 360) = 0;
    operator delete[](v3);
    *(_DWORD *)(a1 + 364) = 0;
    *(_DWORD *)(a1 + 308) = 0;
  }
  return v1;
}

// ===== sub_424350 @ 0x00424350..0x00424382 =====
int __usercall sub_424350@<eax>(int a1@<esi>)
{
  int v1; // edi

  v1 = *(_DWORD *)(a1 + 368);
  if ( v1 )
  {
    operator delete[](*(void **)(a1 + 380));
    *(_DWORD *)(a1 + 380) = 0;
    *(_DWORD *)(a1 + 368) = 0;
  }
  return v1;
}

// ===== sub_424390 @ 0x00424390..0x004243B6 =====
_DWORD *__usercall sub_424390@<eax>(_DWORD *result@<eax>)
{
  result[96] = 0;
  result[97] = -1;
  result[98] = -1;
  result[99] = -1;
  result[100] = -1;
  return result;
}

// ===== sub_4243C0 @ 0x004243C0..0x0042449B =====
_DWORD *__thiscall sub_4243C0(void *this, _DWORD *a2)
{
  void *v2; // eax
  int v3; // eax

  sub_41A400((int)this, a2, 4, 1);
  *a2 = &CDspObjPrtclScrn::`vftable';
  v2 = operator new(0x216A8u);
  if ( v2 )
    v3 = sub_44DC30(v2);
  else
    v3 = 0;
  a2[77] = v3;
  a2[78] = 0;
  a2[79] = 0;
  a2[80] = 0;
  a2[81] = 0;
  a2[82] = 0;
  sub_4246E0(0, 0);
  a2[83] = 0;
  a2[84] = 0;
  a2[85] = 4096;
  a2[86] = 0;
  a2[87] = 0;
  a2[88] = 0;
  return a2;
}

// ===== sub_4244A0 @ 0x004244A0..0x004244C2 =====
void *__thiscall sub_4244A0(void *this, char a2)
{
  sub_4244D0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4244D0 @ 0x004244D0..0x0042453C =====
int __stdcall sub_4244D0(_DWORD *a1)
{
  void (__thiscall ***v1)(_DWORD, int); // ecx

  *a1 = &CDspObjPrtclScrn::`vftable';
  v1 = (void (__thiscall ***)(_DWORD, int))a1[77];
  if ( v1 )
    (**v1)(v1, 1);
  sub_424B40();
  return sub_41A5E0(a1);
}

// ===== sub_424540 @ 0x00424540..0x0042454D =====
int __usercall sub_424540@<eax>(int a1@<eax>)
{
  return sub_44E2C0(*(_DWORD *)(a1 + 308));
}

// ===== sub_424550 @ 0x00424550..0x0042455E =====
int __usercall sub_424550@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_44E2E0(*(_DWORD *)(a2 + 308), a1);
}

// ===== sub_424560 @ 0x00424560..0x004245AB =====
int __userpurge sub_424560@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  int v3; // eax

  v3 = sub_44E380(a2, a1);
  switch ( v3 )
  {
    case 0:
      return 0;
    case -2147483647:
      return -2147483643;
    case -2147483646:
      return -2147483642;
  }
  return -1;
}

// ===== sub_4245B0 @ 0x004245B0..0x004245D6 =====
int __thiscall sub_4245B0(_DWORD *this, int a2)
{
  int v3; // ecx
  int result; // eax

  v3 = sub_41AF90(this, a2);
  result = -1;
  if ( v3 >= 0 )
    return this[78] - v3 - 1;
  return result;
}

// ===== sub_4245E0 @ 0x004245E0..0x0042467C =====
int __thiscall sub_4245E0(_DWORD *this, int a2, int *a3, int a4)
{
  int result; // eax
  int v6; // ecx
  int v7; // eax
  int v8; // edx
  _DWORD *v9; // eax
  unsigned int v10; // eax
  unsigned int v11; // [esp-8h] [ebp-28h]
  _DWORD v12[6]; // [esp+8h] [ebp-18h] BYREF

  result = this[79];
  if ( result && *(_DWORD *)result )
  {
    result = (*(int (__thiscall **)(_DWORD *, int))(*this + 20))(this, a4);
    if ( result >= 0 )
    {
      v6 = this[79];
      v7 = 3 * result;
      v8 = *(_DWORD *)(v6 + 8 * v7);
      v9 = (_DWORD *)(v6 + 8 * v7);
      v12[0] = v8;
      v12[1] = v9[1];
      v12[2] = v9[2];
      v12[3] = v9[3];
      v12[4] = v9[4];
      v12[5] = v9[5];
      sub_4091B0(v12, a3);
      v11 = sub_41B770(this);
      v10 = sub_41B610((int)this);
      return sub_40A9E0(a2, (int)v12, v10, v11, 1);
    }
  }
  return result;
}

// ===== sub_424680 @ 0x00424680..0x004246D5 =====
int __thiscall sub_424680(_DWORD *this, unsigned int a2)
{
  unsigned int v3; // ebx

  v3 = (*(int (__thiscall **)(_DWORD *))(*this + 88))(this);
  if ( sub_41B8B0(this, a2) )
  {
    if ( !sub_424C90(this, this[82], 0) )
      return 1;
    sub_41B8B0(this, v3);
  }
  return 0;
}

// ===== sub_4246E0 @ 0x004246E0..0x0042484C =====
int __userpurge sub_4246E0@<eax>(_DWORD *a1@<eax>, unsigned int a2@<esi>, int *a3, int *a4)
{
  int v5; // ebx
  void *v6; // eax
  unsigned int v7; // edx
  _DWORD *v8; // eax
  int v9; // ebx
  int v10; // ecx
  int v11; // ebx
  int v13; // [esp+10h] [ebp-10h] BYREF
  int v14; // [esp+14h] [ebp-Ch] BYREF
  int v15; // [esp+18h] [ebp-8h]
  int v16; // [esp+1Ch] [ebp-4h]

  v14 = 0x7FFF;
  v13 = 0;
  if ( a2 == 1 )
  {
    if ( !a3 )
      a3 = &v14;
    a4 = &v13;
  }
  v5 = sub_424C90(a1, a4, 1);
  if ( !v5 )
  {
    sub_424B40();
    a1[78] = a2;
    a1[79] = operator new[](24 * a2);
    a1[80] = operator new[](4 * a2);
    a1[81] = operator new[](4 * a2);
    v6 = operator new[](4 * a2);
    v7 = 0;
    a1[82] = v6;
    v15 = 0;
    if ( a2 )
    {
      v16 = 0;
      do
      {
        v8 = (_DWORD *)(v16 + a1[79]);
        *v8 = 0;
        v8[1] = 0;
        v8[2] = 0;
        v8[3] = 0;
        v8[4] = 0;
        v8[5] = 0;
        v9 = a3[v7];
        v16 += 24;
        v10 = (v9 << 8) + v15;
        *(_DWORD *)(a1[80] + 4 * v7) = v10;
        v11 = a3[v7];
        v15 = v10;
        *(_DWORD *)(a1[81] + 4 * v7) = v11;
        *(_DWORD *)(a1[82] + 4 * v7) = a4[v7];
        ++v7;
      }
      while ( v7 < a2 );
      v5 = 0;
    }
    sub_424C90(a1, a4, 0);
    sub_424BA0();
  }
  return v5;
}

// ===== sub_424850 @ 0x00424850..0x0042488C =====
int __userpurge sub_424850@<eax>(unsigned int a1@<eax>, _DWORD *a2@<ecx>, unsigned int a3)
{
  int result; // eax
  int v7; // [esp+14h] [ebp+8h]

  result = (*(int (__thiscall **)(_DWORD *, unsigned int, unsigned int))(*a2 + 116))(a2, a3, a1);
  v7 = result;
  if ( result )
  {
    sub_424BA0();
    result = v7;
    a2[83] = a3 >> 1;
    a2[84] = a1 >> 1;
  }
  return result;
}

// ===== sub_424890 @ 0x00424890..0x00424A07 =====
_DWORD *__usercall sub_424890@<eax>(int *a1@<edi>)
{
  _DWORD *result; // eax
  unsigned int v2; // esi
  int v3; // ebx
  int v4; // edx
  int *v5; // esi
  int v6; // ebx
  _DWORD *v7; // esi
  int v8; // edx
  int v9; // eax
  int v10; // ecx
  bool v11; // zf
  _DWORD *v12; // [esp+8h] [ebp-28h]
  _DWORD *v13; // [esp+Ch] [ebp-24h]
  int v14; // [esp+10h] [ebp-20h]
  int v15[2]; // [esp+18h] [ebp-18h] BYREF
  _DWORD v16[4]; // [esp+20h] [ebp-10h] BYREF

  result = (_DWORD *)a1[79];
  if ( result && *result )
  {
    v2 = 0;
    if ( a1[78] )
    {
      v3 = 0;
      do
      {
        sub_40A620(v3 + a1[79], 0);
        ++v2;
        v3 += 24;
      }
      while ( v2 < a1[78] );
    }
    sub_44E230(a1[78], a1[79], a1[80], a1[83], a1[84]);
    v4 = *a1;
    v5 = a1 + 87;
    if ( a1[87] + a1[88] >= a1[85] )
    {
      result = (_DWORD *)(*(int (__thiscall **)(int *))(v4 + 12))(a1);
      a1[86] ^= 1u;
    }
    else
    {
      (*(void (__thiscall **)(int *))(v4 + 28))(a1);
      (*(void (__thiscall **)(int *, int *))(*a1 + 52))(a1, v15);
      result = a1 + 89;
      v13 = a1 + 89;
      v12 = a1 + 87;
      v14 = 2;
      do
      {
        v6 = 0;
        if ( *v5 > 0 )
        {
          v7 = v13;
          do
          {
            v8 = v7[1];
            v9 = v7[2];
            v16[0] = *v7;
            v10 = v7[3];
            v16[1] = v8;
            v16[3] = v10;
            v16[2] = v9;
            sub_409170(v15[1], v15[0], v16);
            result = (_DWORD *)sub_443240(dword_565B2C);
            ++v6;
            v7 += 4;
          }
          while ( v6 < *v12 );
        }
        v13 += 0x20000;
        v5 = v12 + 1;
        v11 = v14-- == 1;
        ++v12;
      }
      while ( !v11 );
      a1[86] ^= 1u;
    }
  }
  return result;
}

// ===== sub_424A10 @ 0x00424A10..0x00424A4D =====
int __userpurge sub_424A10@<eax>(
        int a1@<eax>,
        int a2@<edx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  *(_DWORD *)(a2 + 332) = a1;
  *(_DWORD *)(a2 + 336) = a10;
  return sub_44E150(a3, a6, a7, a8, a9);
}

// ===== sub_424A50 @ 0x00424A50..0x00424A5B =====
BOOL __usercall sub_424A50@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  int v2; // ecx
  BOOL v3; // esi

  v2 = *(_DWORD *)(a2 + 308);
  v3 = a1 <= 0x64;
  if ( a1 <= 0x64 )
  {
    *(_DWORD *)(v2 + 8) = a1;
    sub_44F0E0(*(_DWORD *)(v2 + 136788));
  }
  return v3;
}

// ===== sub_424A60 @ 0x00424A60..0x00424A6D =====
int __usercall sub_424A60@<eax>(int a1@<eax>)
{
  return sub_44E260(*(_DWORD *)(a1 + 308));
}

// ===== sub_424A70 @ 0x00424A70..0x00424AAE =====
int __fastcall sub_424A70(
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
        int a13)
{
  return sub_44E110(*(_DWORD *)(a3 + 308), a4, a5, a6, a7, a8, a9, a10, a2, a12, a13);
}

// ===== sub_424AB0 @ 0x00424AB0..0x00424ACB =====
int __userpurge sub_424AB0@<eax>(int a1@<eax>, int a2, int a3)
{
  return sub_44DFB0(a1);
}

// ===== sub_424AD0 @ 0x00424AD0..0x00424AEB =====
int __userpurge sub_424AD0@<eax>(int a1@<eax>, int a2, int a3)
{
  return sub_44DFD0(a1);
}

// ===== sub_424AF0 @ 0x00424AF0..0x00424B3D =====
int __stdcall sub_424AF0(
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
  return sub_44DFF0(*(_DWORD *)(a1 + 308), a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

// ===== sub_424B40 @ 0x00424B40..0x00424B9F =====
void __usercall sub_424B40(_DWORD *a1@<eax>)
{
  void *v2; // [esp-10h] [ebp-18h]
  void *v3; // [esp-Ch] [ebp-14h]
  void *v4; // [esp-8h] [ebp-10h]
  void *v5; // [esp-4h] [ebp-Ch]

  sub_424C30();
  v5 = (void *)a1[79];
  a1[78] = 0;
  operator delete[](v5);
  v4 = (void *)a1[80];
  a1[79] = 0;
  operator delete[](v4);
  v3 = (void *)a1[81];
  a1[80] = 0;
  operator delete[](v3);
  v2 = (void *)a1[82];
  a1[81] = 0;
  operator delete[](v2);
  a1[82] = 0;
}

// ===== sub_424BA0 @ 0x00424BA0..0x00424C30 =====
int __usercall sub_424BA0@<eax>(int a1@<edi>)
{
  int v1; // ebx
  unsigned int i; // [esp+Ch] [ebp-Ch]
  int v4; // [esp+10h] [ebp-8h] BYREF
  int v5; // [esp+14h] [ebp-4h] BYREF

  v1 = 0;
  if ( !sub_41C020(a1, &v5, &v4) )
    return 0;
  sub_424C30();
  for ( i = 0; i < *(_DWORD *)(a1 + 312); ++i )
  {
    sub_409080((_DWORD *)(v1 + *(_DWORD *)(a1 + 316)), 1);
    sub_40A620(v1 + *(_DWORD *)(a1 + 316), 0);
    v1 += 24;
  }
  return 1;
}

// ===== sub_424C30 @ 0x00424C30..0x00424C8C =====
_DWORD *__usercall sub_424C30@<eax>(int a1@<esi>)
{
  unsigned int v1; // ebx
  int v2; // edi
  _DWORD *result; // eax

  if ( *(_DWORD *)(a1 + 316) )
  {
    v1 = 0;
    if ( *(_DWORD *)(a1 + 312) )
    {
      v2 = 0;
      do
      {
        operator delete[](*(void **)(v2 + *(_DWORD *)(a1 + 316)));
        result = (_DWORD *)(v2 + *(_DWORD *)(a1 + 316));
        *result = 0;
        result[1] = 0;
        result[2] = 0;
        result[3] = 0;
        result[4] = 0;
        ++v1;
        result[5] = 0;
        v2 += 24;
      }
      while ( v1 < *(_DWORD *)(a1 + 312) );
    }
  }
  return result;
}

// ===== sub_424C90 @ 0x00424C90..0x00424D59 =====
int __userpurge sub_424C90@<eax>(unsigned int a1@<eax>, int a2, int a3, int a4)
{
  char *v6; // eax
  unsigned int v7; // ecx
  char *v8; // edx
  int v9; // eax
  int v11; // [esp+8h] [ebp-Ch]
  int v12; // [esp+Ch] [ebp-8h]
  void *Src; // [esp+10h] [ebp-4h]
  int v14; // [esp+1Ch] [ebp+8h]

  v14 = 0;
  if ( a1 - 1 > 0xF )
    return -2147483647;
  v12 = (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 88))(a2);
  v11 = (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 28))(a2);
  v6 = (char *)operator new[](4 * a1);
  v7 = 0;
  Src = v6;
  if ( a1 )
  {
    v8 = &v6[4 * a1 - 4];
    while ( 1 )
    {
      v9 = *(_DWORD *)(a3 + 4 * v7);
      if ( (unsigned int)(v9 + v12) > 0xFFFF )
        break;
      ++v7;
      *(_DWORD *)v8 = v11 + (v9 << 16);
      v8 -= 4;
      if ( v7 >= a1 )
        goto LABEL_8;
    }
    v14 = -2147483644;
  }
LABEL_8:
  if ( !a4 && !v14 )
    sub_41AF00(a2, a1, Src);
  operator delete[](Src);
  return v14;
}

// ===== sub_424D60 @ 0x00424D60..0x00424E9E =====
_DWORD *__thiscall sub_424D60(void *this, _DWORD *a2)
{
  sub_41A400((int)this, a2, 5, 1);
  *a2 = &CDspObjRainScrn::`vftable';
  a2[100] = 0;
  a2[102] = 0;
  a2[103] = 0;
  a2[104] = 0;
  a2[105] = 0;
  a2[106] = 0;
  a2[101] = 0;
  a2[107] = -1;
  a2[77] = -6000;
  a2[78] = -4000;
  a2[79] = -2000;
  a2[80] = 6000;
  a2[81] = 4000;
  a2[82] = 2000;
  a2[83] = 0;
  a2[84] = -1;
  a2[85] = 0;
  a2[86] = 15360;
  a2[87] = 89600;
  a2[88] = -1;
  a2[89] = 0;
  a2[90] = 20;
  a2[91] = 50;
  a2[92] = 1;
  a2[93] = 0;
  a2[94] = 0;
  a2[95] = 0;
  a2[96] = 0;
  a2[97] = 0;
  a2[98] = 0;
  a2[99] = 100;
  return a2;
}

// ===== sub_424EA0 @ 0x00424EA0..0x00424EC2 =====
void *__thiscall sub_424EA0(void *this, char a2)
{
  sub_424ED0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_424ED0 @ 0x00424ED0..0x00424F51 =====
int __stdcall sub_424ED0(int a1)
{
  void *v1; // edi

  *(_DWORD *)a1 = &CDspObjRainScrn::`vftable';
  v1 = *(void **)(a1 + 400);
  if ( v1 )
  {
    sub_4900A0(*(_DWORD *)(a1 + 400));
    operator delete(v1);
  }
  if ( *(_DWORD *)(a1 + 404) )
    operator delete[](*(void **)(a1 + 404));
  return sub_41A5E0((_DWORD *)a1);
}

// ===== sub_424F60 @ 0x00424F60..0x00424F66 =====
int __usercall sub_424F60@<eax>(int result@<eax>)
{
  dword_5076A8 = result;
  return result;
}

// ===== sub_424F70 @ 0x00424F70..0x00424F76 =====
int sub_424F70()
{
  return dword_5076A8;
}

// ===== sub_424F80 @ 0x00424F80..0x00425049 =====
int __thiscall sub_424F80(_DWORD *this, int a2)
{
  void *v3; // edi
  void *v4; // eax
  int v5; // eax
  _BYTE v7[28]; // [esp+10h] [ebp-6Ch] BYREF
  _BYTE v8[68]; // [esp+2Ch] [ebp-50h] BYREF
  int v9; // [esp+78h] [ebp-4h]

  v3 = (void *)this[100];
  if ( v3 )
  {
    sub_4900A0(this[100]);
    operator delete(v3);
  }
  v4 = operator new(0xA8u);
  v9 = 0;
  if ( v4 )
    v5 = sub_48FF50(v4);
  else
    v5 = 0;
  v9 = -1;
  this[100] = v5;
  sub_490110(v8, this + 77);
  sub_490180(this[100], v7, this + 93);
  return sub_490290(a2);
}

// ===== sub_425050 @ 0x00425050..0x004250B5 =====
int __userpurge sub_425050@<eax>(_DWORD *a1@<eax>, int a2, int a3)
{
  int v4; // edi
  void *v5; // eax
  _DWORD *v6; // esi

  v4 = (*(int (__thiscall **)(_DWORD *, int, int))(*a1 + 116))(a1, a2, a3);
  if ( v4 )
  {
    v5 = (void *)a1[101];
    v6 = a1 + 101;
    if ( v5 )
      operator delete[](v5);
    sub_409080(v6, 1);
    sub_40A620((int)v6, 0);
  }
  return v4 != 0 ? 0 : -2147483647;
}

// ===== sub_4250C0 @ 0x004250C0..0x004250E4 =====
int __usercall sub_4250C0@<eax>(int a1@<esi>)
{
  if ( *(_DWORD *)(a1 + 400) )
    sub_4902B0();
  return *(_DWORD *)(a1 + 400) != 0 ? 0 : -2147483646;
}

// ===== sub_4250F0 @ 0x004250F0..0x00425151 =====
int __usercall sub_4250F0@<eax>(_DWORD *a1@<esi>)
{
  if ( !a1[101] )
    return -2147483645;
  if ( !a1[100] )
    return -2147483646;
  sub_40A620((int)(a1 + 101), 0);
  sub_490300(a1[100], a1[101], a1[102]);
  (*(void (__thiscall **)(_DWORD *))(*a1 + 12))(a1);
  return 0;
}

// ===== sub_425160 @ 0x00425160..0x004251CA =====
int __userpurge sub_425160@<eax>(int a1@<edx>, int a2@<ecx>, _DWORD *a3@<esi>, int a4, int a5, int a6, int a7)
{
  int v7; // edx
  _BYTE v9[64]; // [esp+0h] [ebp-40h] BYREF

  a3[78] = a1;
  a3[80] = a5;
  a3[77] = a2;
  a3[82] = a7;
  v7 = a3[100];
  a3[79] = a4;
  a3[81] = a6;
  if ( v7 )
    sub_490110(v9, a3 + 77);
  return a3[100] != 0 ? 0 : -2147483646;
}

// ===== sub_4251D0 @ 0x004251D0..0x00425219 =====
int __usercall sub_4251D0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // edx
  _BYTE v4[64]; // [esp+0h] [ebp-40h] BYREF

  if ( !a1 )
    return -2147483644;
  v2 = *(_DWORD *)(a2 + 400);
  *(_DWORD *)(a2 + 344) = a1 << 8;
  if ( !v2 )
    return -2147483646;
  sub_490110(v4, a2 + 308);
  return 0;
}

// ===== sub_425220 @ 0x00425220..0x00425269 =====
int __usercall sub_425220@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // edx
  _BYTE v4[64]; // [esp+0h] [ebp-40h] BYREF

  if ( !a1 )
    return -2147483644;
  v2 = *(_DWORD *)(a2 + 400);
  *(_DWORD *)(a2 + 348) = a1 << 8;
  if ( !v2 )
    return -2147483646;
  sub_490110(v4, a2 + 308);
  return 0;
}

// ===== sub_425270 @ 0x00425270..0x004252B2 =====
int __usercall sub_425270@<eax>(int a1@<eax>, int a2@<esi>)
{
  int v2; // edx
  _BYTE v4[64]; // [esp+0h] [ebp-40h] BYREF

  v2 = *(_DWORD *)(a2 + 400);
  *(_DWORD *)(a2 + 352) = a1;
  if ( v2 )
    sub_490110(v4, a2 + 308);
  return *(_DWORD *)(a2 + 400) != 0 ? 0 : -2147483646;
}

// ===== sub_4252C0 @ 0x004252C0..0x00425302 =====
int __usercall sub_4252C0@<eax>(int a1@<eax>, int a2@<esi>)
{
  int v2; // edx
  _BYTE v4[64]; // [esp+0h] [ebp-40h] BYREF

  v2 = *(_DWORD *)(a2 + 400);
  *(_DWORD *)(a2 + 360) = a1;
  if ( v2 )
    sub_490110(v4, a2 + 308);
  return *(_DWORD *)(a2 + 400) != 0 ? 0 : -2147483646;
}

// ===== sub_425310 @ 0x00425310..0x00425355 =====
int __usercall sub_425310@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // edx
  _BYTE v4[64]; // [esp+0h] [ebp-40h] BYREF

  if ( !a2 )
    return -2147483644;
  v2 = *(_DWORD *)(a1 + 400);
  *(_DWORD *)(a1 + 364) = a2;
  if ( !v2 )
    return -2147483646;
  sub_490110(v4, a1 + 308);
  return 0;
}

// ===== sub_425360 @ 0x00425360..0x004253AC =====
int __userpurge sub_425360@<eax>(int a1@<edx>, int a2@<ecx>, _DWORD *a3@<esi>, int a4)
{
  int v4; // ecx
  _BYTE v6[28]; // [esp+0h] [ebp-1Ch] BYREF

  a3[93] = a2;
  a3[95] = a4;
  v4 = a3[100];
  a3[94] = a1;
  if ( v4 )
    sub_490180(v4, v6, a3 + 93);
  return a3[100] != 0 ? 0 : -2147483646;
}

// ===== sub_4253B0 @ 0x004253B0..0x00425400 =====
int __userpurge sub_4253B0@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3@<esi>, int a4)
{
  int v4; // eax
  _BYTE v6[28]; // [esp+0h] [ebp-1Ch] BYREF

  a3[96] = a1;
  v4 = a3[100];
  a3[97] = a2;
  a3[98] = a4;
  if ( v4 )
    sub_490180(v4, v6, a3 + 93);
  return a3[100] != 0 ? 0 : -2147483646;
}

// ===== sub_425400 @ 0x00425400..0x00425443 =====
int __usercall sub_425400@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax
  _BYTE v4[28]; // [esp+0h] [ebp-1Ch] BYREF

  if ( !a1 )
    return -2147483644;
  *(_DWORD *)(a2 + 396) = a1;
  v2 = *(_DWORD *)(a2 + 400);
  if ( !v2 )
    return -2147483646;
  sub_490180(v2, v4, a2 + 372);
  return 0;
}

// ===== sub_425450 @ 0x00425450..0x0042546B =====
BOOL __thiscall sub_425450(_DWORD *this)
{
  return sub_41AE30(this) && sub_424F70();
}

// ===== sub_425470 @ 0x00425470..0x004255DC =====
void __thiscall sub_425470(_DWORD *this, int a2, int *a3, int a4)
{
  int v5; // ecx
  int v6; // edx
  int v7; // eax
  int v8; // ecx
  int v9; // edx
  int v10; // ecx
  unsigned int v11; // eax
  int v12; // eax
  unsigned int v13; // ebx
  unsigned int v14; // eax
  unsigned int v15; // eax
  unsigned int v16; // [esp-8h] [ebp-60h]
  _DWORD v17[2]; // [esp+10h] [ebp-48h] BYREF
  int v18; // [esp+18h] [ebp-40h]
  int v19; // [esp+1Ch] [ebp-3Ch]
  int v20; // [esp+20h] [ebp-38h]
  int v21; // [esp+24h] [ebp-34h]
  void *v22[6]; // [esp+28h] [ebp-30h] BYREF
  _DWORD v23[6]; // [esp+40h] [ebp-18h] BYREF

  if ( this[101] )
  {
    v5 = this[102];
    v6 = this[103];
    v17[0] = this[101];
    v7 = this[104];
    v17[1] = v5;
    v8 = this[105];
    v18 = v6;
    v9 = this[106];
    v19 = v7;
    v20 = v8;
    v21 = v9;
    sub_4091B0(v17, a3);
    v10 = this[107];
    if ( v10 == -1 )
    {
      v16 = sub_41B770(this);
      v11 = sub_41B610((int)this);
      sub_40A9E0(a2, (int)v17, v11, v16, 1);
      return;
    }
    if ( sub_407F20(dword_565B30, v10, v23) && this[108] == sub_408300(this[107], dword_565B30) )
    {
      sub_4091B0(v23, a3);
      v12 = sub_41B610((int)this);
      v13 = v12;
      if ( v12 )
      {
        if ( v12 != 1 && v12 != 32 )
        {
          sub_409030(v20, v19, v22, v18);
          sub_416510((int)v17, (int)v23, (int)v22);
          v14 = sub_41B770(this);
          sub_40A9E0(a2, (int)v22, v13, v14, 1);
          operator delete[](v22[0]);
          return;
        }
        v15 = sub_41B770(this);
      }
      else
      {
        v15 = 0;
      }
      sub_4163A0((int)v23, a2, v15, (int)v17);
    }
  }
}

// ===== sub_4255E0 @ 0x004255E0..0x00425618 =====
int __userpurge sub_4255E0@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>, int a4, int a5, int a6)
{
  (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a3 + 44))(a3, a1, a2);
  sub_41B600(a3, a4);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)a3 + 72))(a3, a5);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)a3 + 84))(a3, a6);
  return 0;
}

// ===== sub_425620 @ 0x00425620..0x004256B8 =====
int __userpurge sub_425620@<eax>(int a1@<edi>, int a2)
{
  int v2; // ebx
  int v3; // edx
  _DWORD v5[6]; // [esp+4h] [ebp-1Ch] BYREF
  int v6; // [esp+1Ch] [ebp-4h] BYREF

  v2 = a2;
  if ( a1 == -1 )
  {
    *(_DWORD *)(a2 + 428) = -1;
    return 0;
  }
  else if ( sub_407F20(dword_565B30, a1, v5) )
  {
    if ( v5[4] == 3 && (sub_41C020(v2, &a2, &v6), a2 == v5[2]) && v6 == v5[3] )
    {
      v3 = dword_565B30;
      *(_DWORD *)(v2 + 428) = a1;
      *(_DWORD *)(v2 + 432) = sub_408300(a1, v3);
      return 0;
    }
    else
    {
      return -2147483642;
    }
  }
  else
  {
    return -2147483643;
  }
}

// ===== sub_4256C0 @ 0x004256C0..0x00425807 =====
_DWORD *__thiscall sub_4256C0(void *this, _DWORD *a2, int a3)
{
  unsigned int i; // ebx

  sub_41A400((int)this, a2, 2, a3);
  *a2 = &CDspObjSprite::`vftable';
  a2[77] = 0;
  a2[78] = 0;
  a2[81] = 0;
  a2[82] = 0;
  a2[84] = -1;
  a2[85] = -1;
  memset(a2 + 88, 0, 0x60u);
  memset(a2 + 112, 0, 0x60u);
  a2[145] = -1;
  a2[136] = 0;
  a2[137] = 0;
  a2[138] = 0;
  a2[139] = 0;
  a2[140] = 0;
  a2[141] = 0;
  a2[185] = 0;
  a2[192] = 0;
  a2[194] = 0;
  a2[195] = 0;
  a2[196] = 0;
  a2[197] = 0;
  a2[198] = 0;
  a2[199] = 0;
  a2[200] = 0;
  a2[204] = 0;
  a2[208] = 0;
  a2[209] = 0;
  a2[210] = 0;
  a2[211] = 0;
  a2[212] = 0;
  a2[213] = 0;
  for ( i = 0; i < 0x10; ++i )
    sub_42AB50(0, 0, 0);
  return a2;
}

// ===== sub_425810 @ 0x00425810..0x00425832 =====
void *__thiscall sub_425810(void *this, char a2)
{
  sub_425840(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_425840 @ 0x00425840..0x004258CA =====
int __stdcall sub_425840(_DWORD *a1)
{
  *a1 = &CDspObjSprite::`vftable';
  sub_428CD0(0);
  sub_428D90(0);
  sub_42A9C0(a1);
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  return sub_41A5E0(a1);
}

// ===== sub_4258D0 @ 0x004258D0..0x00426F2D =====
void __thiscall sub_4258D0(_DWORD *this, _DWORD *a2, int *a3, int a4)
{
  unsigned int v5; // edi
  _DWORD *v6; // ecx
  void *v7; // eax
  int v8; // ecx
  int v9; // eax
  unsigned int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // ecx
  int v14; // edx
  void **v15; // ecx
  int v16; // eax
  int v17; // esi
  unsigned int v18; // eax
  unsigned int v19; // eax
  int v20; // ecx
  int v21; // edx
  int v22; // eax
  void *v23; // esi
  int v24; // eax
  bool v25; // zf
  int v26; // eax
  int v27; // eax
  int v28; // ecx
  int v29; // edx
  int v30; // eax
  int v31; // edx
  int v32; // eax
  int v33; // edx
  int v34; // eax
  int v35; // eax
  int v36; // edi
  int v37; // eax
  int v38; // ecx
  signed int v39; // esi
  int v40; // edx
  int v41; // ecx
  void *v42; // eax
  int v43; // eax
  int v44; // eax
  int v45; // ecx
  void *v46; // edx
  char v47; // cl
  unsigned int v48; // eax
  _DWORD *v49; // ecx
  int v50; // eax
  int v51; // eax
  unsigned int v52; // eax
  int v53; // eax
  int v54; // ecx
  int v55; // edx
  int v56; // eax
  int v57; // esi
  int v58; // eax
  unsigned int v59; // eax
  int v60; // eax
  int v61; // ecx
  int v62; // edx
  unsigned int v63; // [esp+Ch] [ebp-BCh]
  _DWORD *v64; // [esp+10h] [ebp-B8h]
  _DWORD *v65; // [esp+10h] [ebp-B8h]
  int v66; // [esp+18h] [ebp-B0h]
  int v67; // [esp+18h] [ebp-B0h]
  int v68; // [esp+18h] [ebp-B0h]
  unsigned int v69; // [esp+1Ch] [ebp-ACh]
  void *v70[6]; // [esp+20h] [ebp-A8h] BYREF
  void *v71; // [esp+38h] [ebp-90h] BYREF
  int v72; // [esp+3Ch] [ebp-8Ch]
  int v73; // [esp+40h] [ebp-88h]
  int v74; // [esp+44h] [ebp-84h]
  int v75; // [esp+48h] [ebp-80h]
  int v76; // [esp+4Ch] [ebp-7Ch]
  void *v77; // [esp+50h] [ebp-78h] BYREF
  int v78; // [esp+54h] [ebp-74h]
  int v79; // [esp+58h] [ebp-70h]
  int v80; // [esp+5Ch] [ebp-6Ch]
  int v81; // [esp+60h] [ebp-68h]
  int v82; // [esp+64h] [ebp-64h]
  _DWORD v83[2]; // [esp+68h] [ebp-60h] BYREF
  void *v84; // [esp+70h] [ebp-58h]
  void *v85; // [esp+74h] [ebp-54h]
  void *v86; // [esp+78h] [ebp-50h]
  void *v87; // [esp+7Ch] [ebp-4Ch]
  void *v88[6]; // [esp+80h] [ebp-48h] BYREF
  void *v89; // [esp+98h] [ebp-30h] BYREF
  int v90; // [esp+9Ch] [ebp-2Ch]
  int v91; // [esp+A0h] [ebp-28h]
  int v92; // [esp+A4h] [ebp-24h]
  int v93; // [esp+A8h] [ebp-20h]
  int v94; // [esp+ACh] [ebp-1Ch]
  _DWORD v95[6]; // [esp+B0h] [ebp-18h] BYREF

  if ( !this[82] )
  {
    v5 = sub_41B610((int)this);
    v63 = sub_41B770(v6);
    v7 = (void *)sub_41B740();
    v8 = this[77];
    v89 = v7;
    if ( v8 == 2 )
    {
      if ( !this[163] && this[167] == 0x10000 && this[168] == 0x10000 )
        v8 = 0;
    }
    else if ( v8 == 3 )
    {
      if ( (unsigned int)v7 >= 0x100 )
        v8 = 0;
    }
    else if ( v8 == 5
           && !this[163]
           && this[167] == 0x10000
           && this[168] == 0x10000
           && !this[169]
           && !this[170]
           && (!this[192] || !this[194]) )
    {
      v8 = this[85] != -1;
    }
    switch ( v8 )
    {
      case 0:
        if ( sub_407F20(dword_565B30, this[84], v83) && this[86] == sub_408300(this[84], dword_565B30) )
        {
          if ( !this[78] || this[77] == 3 )
          {
            if ( sub_42ABA0(this) )
            {
              v16 = (int)v86;
              if ( v86 == (void *)1 )
                v16 = 2;
              sub_409030(v16, a3[3] - a3[1] + 1, v88, a3[2] - *a3 + 1);
              sub_4091B0(v83, a3);
              sub_42ABE0(v88);
              sub_40A9E0((int)a2, (int)v88, v5, v63, 1);
              operator delete[](v88[0]);
            }
            else
            {
              sub_4091B0(v83, a3);
              sub_40A9E0((int)a2, (int)v83, v5, v63, 1);
            }
          }
          else if ( this[81] )
          {
            if ( sub_42A4A0(this, a3) )
            {
              v9 = (int)v86;
              if ( v86 == (void *)1 )
                v9 = 2;
              sub_409030(v9, a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
              sub_4091B0(v83, a3);
              sub_40AF50((size_t)v83, (size_t *)v70);
              v10 = sub_41B770((_DWORD *)this[81]);
              sub_415B30((int)v70, (int)v88, (int)v70, v10);
              sub_42ABE0(v70);
              sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
              operator delete[](v70[0]);
              if ( v89 )
                goto LABEL_157;
            }
          }
          else if ( sub_407F20(dword_565B30, this[79], v88) && this[80] == sub_408300(this[79], dword_565B30) )
          {
            v11 = (int)v86;
            if ( v86 == (void *)1 )
              v11 = 2;
            sub_409030(v11, a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
            sub_4091B0(v83, a3);
            sub_40AF50((size_t)v83, (size_t *)v70);
            v12 = a3[1];
            v13 = a3[2];
            v77 = (void *)*a3;
            v14 = a3[3];
            v78 = v12;
            v79 = v13;
            v80 = v14;
            sub_428280();
            sub_409170(v72, (int)v71, &v77);
            v15 = &v77;
            goto LABEL_161;
          }
        }
        return;
      case 1:
        if ( sub_407F20(dword_565B30, this[84], v83) )
        {
          if ( sub_407F20(dword_565B30, this[85], v95) )
          {
            v17 = dword_565B30;
            if ( this[86] == sub_408300(this[84], dword_565B30) && this[87] == sub_408300(this[85], v17) )
            {
              if ( this[78] )
              {
                if ( this[81] )
                {
                  if ( sub_42A4A0(this, a3) )
                  {
                    sub_409030((int)v86, a3[3] - a3[1] + 1, &v71, a3[2] - *a3 + 1);
                    sub_4091B0(v83, a3);
                    sub_4091B0(v95, a3);
                    sub_40C0F0((int)&v71, (int)v83, (int)v95, this[144], 1);
                    if ( v75 == 1 )
                    {
                      sub_409080(&v77, 2);
                      v18 = sub_41B770((_DWORD *)this[81]);
                      sub_415B30((int)&v71, (int)v70, (int)&v77, v18);
                      operator delete[](v71);
                      v71 = v77;
                      v72 = v78;
                      v73 = v79;
                      v74 = v80;
                      v75 = v81;
                      v76 = v82;
                    }
                    else
                    {
                      v19 = sub_41B770((_DWORD *)this[81]);
                      sub_415B30((int)&v71, (int)v70, (int)&v71, v19);
                    }
                    sub_42ABE0(&v71);
                    sub_40A9E0((int)a2, (int)&v71, v5, v63, 1);
                    operator delete[](v71);
                    if ( v89 )
                      goto LABEL_163;
                  }
                }
                else if ( sub_407F20(dword_565B30, this[79], v70) && this[80] == sub_408300(this[79], dword_565B30) )
                {
                  sub_409030((int)v86, a3[3] - a3[1] + 1, &v89, a3[2] - *a3 + 1);
                  sub_4091B0(v83, a3);
                  sub_4091B0(v95, a3);
                  sub_40C0F0((int)&v89, (int)v83, (int)v95, this[144], 1);
                  v20 = a3[1];
                  v21 = a3[2];
                  v77 = (void *)*a3;
                  v22 = a3[3];
                  v78 = v20;
                  v79 = v21;
                  v80 = v22;
                  sub_428280();
                  sub_409170(v72, (int)v71, &v77);
                  if ( sub_4091B0(v70, (int *)&v77) )
                  {
                    if ( v93 == 1 )
                    {
                      sub_409080(&v77, 2);
                      sub_4155A0((int)v70, (int)&v89, (int)&v77);
                      operator delete[](v89);
                      v89 = v77;
                      v90 = v78;
                      v91 = v79;
                      v92 = v80;
                      v93 = v81;
                      v94 = v82;
                    }
                    else
                    {
                      sub_4155A0((int)v70, (int)&v89, (int)&v89);
                    }
                    sub_42ABE0(&v89);
                    sub_40A9E0((int)a2, (int)&v89, v5, v63, 1);
                  }
                  operator delete[](v89);
                }
              }
              else
              {
                sub_4091B0(v83, a3);
                sub_4091B0(v95, a3);
                if ( v5 >= 2 && v5 != 32 || sub_42ABA0(this) )
                {
                  sub_409030((int)v86, (int)v85, v88, (int)v84);
                  sub_40C0F0((int)v88, (int)v83, (int)v95, this[144], 1);
                  sub_42ABE0(v88);
                  sub_40A9E0((int)a2, (int)v88, v5, v63, 1);
                  operator delete[](v88[0]);
                }
                else
                {
                  sub_40BD60((int)v95, (int)v83, (int)a2, this[144], v5 != 0 ? v63 : 0);
                }
              }
            }
          }
        }
        return;
      case 2:
        if ( !sub_407F20(dword_565B30, this[84], v83) || this[86] != sub_408300(this[84], dword_565B30) )
          return;
        v66 = *a3;
        v23 = (void *)((this[183] - *a3) << 16);
        v24 = (this[184] - a3[1]) << 16;
        v25 = this[78] == 0;
        v71 = v23;
        v72 = v24;
        if ( v25 )
        {
          v26 = (int)v86;
          if ( (v5 < 2 || v5 == 32) && a2[4] == 1 && v86 == (void *)2 )
          {
            if ( !sub_42ABA0(this) )
            {
              sub_4168E0(
                (int)a2,
                (int)v23,
                v72,
                v83,
                this[161],
                this[162],
                this[163],
                this[167],
                this[168],
                v63,
                this[160],
                1);
              return;
            }
            v26 = (int)v86;
          }
          else if ( v86 == (void *)1 )
          {
            v26 = 2;
          }
          sub_409030(v26, a3[3] - a3[1] + 1, v88, a3[2] - v66 + 1);
          sub_417730(
            (int)v88,
            (int)v71,
            v72,
            v83,
            this[161],
            this[162],
            this[163],
            this[167],
            this[168],
            0,
            this[160],
            1);
          goto LABEL_78;
        }
        if ( sub_407F20(dword_565B30, this[79], v88) && this[80] == sub_408300(this[79], dword_565B30) )
        {
          v27 = (int)v86;
          if ( v86 == (void *)1 )
            v27 = 2;
          sub_409030(v27, a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
          sub_417730(
            (int)v70,
            (int)v71,
            v72,
            v83,
            this[161],
            this[162],
            this[163],
            this[167],
            this[168],
            0,
            this[160],
            1);
          v28 = a3[1];
          v29 = a3[2];
          v71 = (void *)*a3;
          v30 = a3[3];
          v72 = v28;
          v73 = v29;
          v74 = v30;
          sub_428280();
          sub_409170(v78, (int)v77, &v71);
          if ( sub_4091B0(v88, (int *)&v71) )
          {
            sub_4155A0((int)v88, (int)v70, (int)v70);
            sub_42ABE0(v70);
            sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
          }
LABEL_163:
          operator delete[](v70[0]);
        }
        break;
      case 3:
        if ( !sub_407F20(dword_565B30, this[84], v83) || this[86] != sub_408300(this[84], dword_565B30) )
          return;
        v31 = this[209];
        v32 = this[210];
        v95[0] = this[208];
        v95[3] = this[211];
        v95[1] = v31;
        v33 = this[212];
        v95[2] = v32;
        v34 = this[213];
        v95[4] = v33;
        v95[5] = v34;
        sub_4091B0(v83, a3);
        sub_4091B0(v95, a3);
        if ( (v5 < 2 || v5 == 32) && !sub_42ABA0(this) )
        {
          sub_415D90((int)v83, (int)v95, (int)a2, this[207], (unsigned int)v89, v63);
          return;
        }
        v35 = (int)v86;
        if ( v86 == (void *)1 )
          v35 = 2;
        sub_409030(v35, (int)v85, v88, (int)v84);
        sub_4160D0((int)v83, (int)v95, (unsigned int)v89, (size_t *)v88, this[207]);
        goto LABEL_95;
      case 4:
        if ( sub_407F20(dword_565B30, this[84], v83) )
        {
          v36 = dword_565B30;
          if ( this[86] == sub_408300(this[84], dword_565B30)
            && sub_407F20(v36, this[201], v88)
            && this[202] == sub_408300(this[201], dword_565B30) )
          {
            v70[0] = (void *)v83[0];
            v70[1] = (void *)v83[1];
            v70[2] = v84;
            v70[3] = v85;
            v70[4] = v86;
            v70[5] = v87;
            sub_4091B0(v70, a3);
            sub_4091B0(v88, a3);
            sub_41B690((int)this);
            v37 = sub_41B6D0((int)this);
            sub_413F10(this[204], v88, (int)v70, a2, (int)v83, 256 - ((unsigned int)(v37 * v38) >> 8));
          }
        }
        return;
      case 5:
        v39 = this[161];
        v40 = this[162];
        v41 = this[168];
        v42 = (void *)(this[169] + ((this[183] - *a3) << 16));
        v77 = (void *)v39;
        v71 = v42;
        v43 = this[184] - a3[1];
        v78 = v40;
        v67 = v41;
        v72 = this[170] + (v43 << 16);
        v69 = this[167];
        v89 = 0;
        v64 = 0;
        if ( this[192] && this[194] )
        {
          if ( !sub_42AAA0(&v89, 0) )
            goto LABEL_113;
          v78 >>= (char)v89;
          v69 <<= (char)v89;
          v67 <<= (char)v89;
          v64 = this + 195;
          v39 = (((this[197] - (_DWORD)v84) << 15) & 0xFFFF0000) + (v39 >> (char)v89);
        }
        else
        {
          if ( this[85] == -1 )
          {
            if ( sub_42AAA0(&v89, 0) )
            {
              v44 = sub_408300(this[84], dword_565B30);
              LOBYTE(v45) = (_BYTE)v89;
              if ( this[86] == v44 )
                v64 = v83;
            }
            else
            {
              LOBYTE(v45) = (_BYTE)v89;
            }
          }
          else
          {
            v64 = this + 136;
            v45 = this[143];
          }
          v78 >>= v45;
          v69 <<= v45;
          v39 >>= v45;
          v67 <<= v45;
        }
        v77 = (void *)v39;
LABEL_113:
        v46 = (void *)v69;
        v89 = (void *)v69;
        v47 = 1;
        if ( v69 < 0x10000 )
        {
          do
          {
            if ( (unsigned int)v46 >= 0x8000 )
              v48 = (unsigned int)(0x10000 - (_DWORD)v46) >> v47;
            else
              v48 = 0x8000 >> v47;
            v78 += v48;
            v46 = (void *)(2 * (_DWORD)v89);
            v39 += v48;
            ++v47;
            v89 = v46;
          }
          while ( (unsigned int)v46 < 0x10000 );
          v77 = (void *)v39;
        }
        v49 = v64;
        if ( !v64 )
          return;
        if ( !this[78] )
        {
          if ( (v5 < 2 || v5 == 32) && a2[4] == 1 && v64[4] == 2 )
          {
            if ( !sub_42ABA0(this) )
            {
              sub_4168E0((int)a2, (int)v71, v72, v64, v39, v78, this[163], v69, v67, v63, this[160], 1);
              return;
            }
            v49 = v64;
          }
          v50 = v49[4];
          if ( v50 == 1 )
            v50 = 2;
          sub_409030(v50, a3[3] - a3[1] + 1, v88, a3[2] - *a3 + 1);
          sub_417730((int)v88, (int)v71, v72, v64, (int)v77, v78, this[163], v69, v67, 0, this[160], 1);
LABEL_78:
          sub_42ABE0(v88);
          sub_40A9E0((int)a2, (int)v88, v5, v63, 1);
          operator delete[](v88[0]);
          return;
        }
        if ( !this[81] )
        {
          if ( !sub_407F20(dword_565B30, this[79], v88) || this[80] != sub_408300(this[79], dword_565B30) )
            return;
          v53 = v64[4];
          if ( v53 == 1 )
            v53 = 2;
          sub_409030(v53, a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
          sub_417730((int)v70, (int)v71, v72, v64, (int)v77, v78, this[163], v69, v67, 0, this[160], 1);
          v54 = a3[1];
          v55 = a3[2];
          v71 = (void *)*a3;
          v56 = a3[3];
          v72 = v54;
          v73 = v55;
          v74 = v56;
          sub_428280();
          sub_409170(v90, (int)v89, &v71);
          if ( sub_4091B0(v88, (int *)&v71) )
          {
            sub_4155A0((int)v88, (int)v70, (int)v70);
            sub_42ABE0(v70);
            sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
            operator delete[](v70[0]);
            return;
          }
          goto LABEL_163;
        }
        if ( !sub_42A4A0(this, a3) )
          return;
        v51 = v64[4];
        if ( v51 == 1 )
          v51 = 2;
        sub_409030(v51, a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
        sub_417730((int)v70, (int)v71, v72, v64, (int)v77, v78, this[163], v69, v67, 0, this[160], 1);
        v52 = sub_41B770((_DWORD *)this[81]);
        sub_415B30((int)v70, (int)v88, (int)v70, v52);
        sub_42ABE0(v70);
        sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
        operator delete[](v70[0]);
        if ( !v89 )
          return;
        goto LABEL_157;
      case 6:
        if ( !this[185] )
          return;
        v65 = sub_429ED0(0, 0, 0, 0, 0) != 0 ? v83 : 0;
        if ( !v65 )
          return;
        (*(void (__thiscall **)(_DWORD *, void **))(*this + 48))(this, &v71);
        v68 = *a3;
        v57 = this[187] - a3[1] - v72;
        v25 = this[78] == 0;
        v89 = (void *)-((int)v71 + *a3);
        if ( v25 )
        {
          if ( (v5 < 2 || v5 == 32) && a2[4] == 1 && ((v58 = v65[4], v58 == 1) || v58 == 2) && !sub_42ABA0(this) )
          {
            sub_40FA50((int)a2, (int)v65, this[185], this[186], v57, (int)v89, 1, v63, 1);
          }
          else
          {
            sub_409030(v65[4], a3[3] - a3[1] + 1, v88, a3[2] - v68 + 1);
            sub_40FA50((int)v88, (int)v65, this[185], this[186], v57, (int)v89, 0, 0, 1);
LABEL_95:
            sub_42ABE0(v88);
            sub_40A9E0((int)a2, (int)v88, v5, v63, 1);
            operator delete[](v88[0]);
          }
          return;
        }
        if ( this[81] )
        {
          if ( sub_42A4A0(this, a3) )
          {
            sub_409030(v65[4], a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
            sub_40FA50((int)v70, (int)v65, this[185], this[186], v57, (int)v89, 0, 0, 1);
            v59 = sub_41B770((_DWORD *)this[81]);
            sub_415B30((int)v70, (int)v88, (int)v70, v59);
            sub_42ABE0(v70);
            sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
            operator delete[](v70[0]);
            if ( v68 )
LABEL_157:
              operator delete[](v88[0]);
          }
          return;
        }
        if ( !sub_407F20(dword_565B30, this[79], v88) || this[80] != sub_408300(this[79], dword_565B30) )
          return;
        sub_409030(v65[4], a3[3] - a3[1] + 1, v70, a3[2] - *a3 + 1);
        sub_40FA50((int)v70, (int)v65, this[185], this[186], v57, (int)v89, 0, 0, 1);
        v60 = a3[1];
        v61 = a3[2];
        v89 = (void *)*a3;
        v62 = a3[3];
        v90 = v60;
        v91 = v61;
        v92 = v62;
        sub_428280();
        sub_409170(v78, (int)v77, &v89);
        v15 = &v89;
LABEL_161:
        if ( sub_4091B0(v88, (int *)v15) )
        {
          sub_4155A0((int)v88, (int)v70, (int)v70);
          sub_42ABE0(v70);
          sub_40A9E0((int)a2, (int)v70, v5, v63, 1);
        }
        goto LABEL_163;
      default:
        return;
    }
  }
}

// ===== sub_426F50 @ 0x00426F50..0x00426FA0 =====
int __userpurge sub_426F50@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  int result; // eax

  result = sub_427410(a1);
  if ( !result )
  {
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a2 + 44))(a2, a3, a4);
    sub_41B600(a2, a5);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2 + 72))(a2, a6);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2 + 84))(a2, a7);
    return 0;
  }
  return result;
}

// ===== sub_426FA0 @ 0x00426FA0..0x00426FF8 =====
int __userpurge sub_426FA0@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9)
{
  int result; // eax

  result = sub_4274A0(a6, a7, a2, a1);
  if ( !result )
  {
    (*(void (__thiscall **)(_DWORD *, int, int))(*a3 + 44))(a3, a4, a5);
    sub_41B600((int)a3, 1);
    sub_41B620(a3, a8);
    (*(void (__thiscall **)(_DWORD *, int))(*a3 + 84))(a3, a9);
    return 0;
  }
  return result;
}

// ===== sub_427000 @ 0x00427000..0x00427092 =====
int __fastcall sub_427000(
        int a1,
        int a2,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13)
{
  void (__thiscall *v14)(_DWORD *, int, int); // eax

  a3[171] = 0;
  a3[172] = 0;
  a3[173] = 0;
  a3[177] = 0;
  a3[178] = 0;
  a3[179] = 0;
  a3[180] = 0;
  v14 = *(void (__thiscall **)(_DWORD *, int, int))(*a3 + 44);
  a3[145] = -1;
  v14(a3, a2, a1);
  sub_41B600((int)a3, a11);
  (*(void (__thiscall **)(_DWORD *, int))(*a3 + 72))(a3, a12);
  (*(void (__thiscall **)(_DWORD *, int))(*a3 + 84))(a3, a13);
  return sub_4275B0(a4, a5, a6, a7, a8, a9, a10);
}

// ===== sub_4270A0 @ 0x004270A0..0x00427101 =====
int __userpurge sub_4270A0@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        _DWORD *a3@<esi>,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  int result; // eax

  result = sub_427790(a3, a2, a1);
  if ( !result )
  {
    (*(void (__thiscall **)(_DWORD *, int, int))(*a3 + 44))(a3, a4, a5);
    a3[207] = a6;
    sub_41B6E0((int)a3, a7);
    sub_41B600((int)a3, a8);
    (*(void (__thiscall **)(_DWORD *, int))(*a3 + 72))(a3, a9);
    (*(void (__thiscall **)(_DWORD *, int))(*a3 + 84))(a3, a10);
    return 0;
  }
  return result;
}

// ===== sub_427110 @ 0x00427110..0x0042716C =====
int __userpurge sub_427110@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  int result; // eax

  result = sub_4278B0(a6, a7, a8, a2, a1);
  if ( !result )
  {
    (*(void (__thiscall **)(_DWORD *, int, int))(*a3 + 44))(a3, a4, a5);
    sub_41B600((int)a3, 1);
    sub_41B660(a3, a9);
    (*(void (__thiscall **)(_DWORD *, int))(*a3 + 84))(a3, a10);
    return 0;
  }
  return result;
}

// ===== sub_427170 @ 0x00427170..0x0042721B =====
int __userpurge sub_427170@<eax>(
        _DWORD *a1@<esi>,
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
  int v16; // edx

  a1[171] = 0;
  a1[172] = 0;
  a1[173] = 0;
  a1[177] = 0;
  a1[178] = 0;
  a1[179] = 0;
  a1[180] = 0;
  a1[145] = -1;
  sub_41BEA0((int)a1, 0);
  (*(void (__thiscall **)(_DWORD *, int, int, int))(*a1 + 60))(a1, a2, v16, a3);
  sub_41B600((int)a1, a14);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 72))(a1, a15);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a16);
  return sub_427AA0(a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

// ===== sub_427220 @ 0x00427220..0x004272E9 =====
int __userpurge sub_427220@<eax>(
        _DWORD *a1@<esi>,
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
        int a16,
        int a17,
        int a18,
        int a19)
{
  int v19; // edx

  a1[171] = 0;
  a1[172] = 0;
  a1[173] = 0;
  a1[174] = 0;
  a1[175] = 0;
  a1[176] = 0;
  a1[177] = 0;
  a1[178] = 0;
  a1[179] = 0;
  a1[180] = 0;
  a1[145] = -1;
  sub_41BEA0((int)a1, 0);
  (*(void (__thiscall **)(_DWORD *, int, int, int))(*a1 + 60))(a1, a2, v19, a3);
  sub_41B600((int)a1, a17);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 72))(a1, a18);
  (*(void (__thiscall **)(_DWORD *, int))(*a1 + 84))(a1, a19);
  return sub_427D90(a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
}

// ===== sub_4272F0 @ 0x004272F0..0x004273EA =====
int __usercall sub_4272F0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int result; // eax

  switch ( *(_DWORD *)(a1 + 308) )
  {
    case 0:
      result = sub_427410(a2);
      break;
    case 2:
      result = sub_4275B0(
                 a2,
                 *(__int16 *)(a1 + 586),
                 *(__int16 *)(a1 + 590),
                 *(_DWORD *)(a1 + 592),
                 *(_DWORD *)(a1 + 612),
                 *(_DWORD *)(a1 + 616),
                 *(_DWORD *)(a1 + 640));
      break;
    case 5:
      result = sub_427AA0(
                 a2,
                 -1,
                 0,
                 -1,
                 *(__int16 *)(a1 + 586),
                 *(__int16 *)(a1 + 590),
                 *(_DWORD *)(a1 + 592),
                 *(_DWORD *)(a1 + 632),
                 *(_DWORD *)(a1 + 636),
                 *(_DWORD *)(a1 + 640));
      break;
    case 6:
      result = sub_427D90(
                 a2,
                 -1,
                 0,
                 -1,
                 *(__int16 *)(a1 + 586),
                 *(__int16 *)(a1 + 590),
                 *(_DWORD *)(a1 + 596),
                 *(_DWORD *)(a1 + 600),
                 *(_DWORD *)(a1 + 604),
                 *(_DWORD *)(a1 + 608),
                 *(_DWORD *)(a1 + 632),
                 *(_DWORD *)(a1 + 636),
                 *(_DWORD *)(a1 + 640));
      break;
    default:
      result = 0;
      break;
  }
  return result;
}

// ===== sub_427410 @ 0x00427410..0x0042749F =====
int __userpurge sub_427410@<eax>(_DWORD *a1@<edi>, int a2)
{
  int v2; // edx
  int v3; // eax
  int v4; // ecx
  int v5; // edx
  void (__thiscall *v6)(_DWORD *, int, int); // eax
  _DWORD v8[7]; // [esp+8h] [ebp-1Ch] BYREF

  if ( !sub_407F20(dword_565B30, a2, v8) )
    return -2147483647;
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  v2 = dword_565B30;
  a1[77] = 0;
  a1[84] = a2;
  v3 = sub_408300(a2, v2);
  v4 = v8[3];
  v5 = v8[2];
  a1[86] = v3;
  v6 = *(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116);
  a1[145] = -1;
  v6(a1, v5, v4);
  return 0;
}

// ===== sub_4274A0 @ 0x004274A0..0x004275AB =====
int __userpurge sub_4274A0@<eax>(_DWORD *a1@<edi>, int a2, int a3, int a4, int a5)
{
  int v5; // ebx
  int v6; // edx
  int v7; // eax
  int v8; // ecx
  int v9; // eax
  int v11; // [esp+8h] [ebp-30h] BYREF
  int v12; // [esp+10h] [ebp-28h]
  int v13; // [esp+14h] [ebp-24h]
  _DWORD v14[6]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, &v11) )
    return -2147483647;
  if ( !sub_407F20(dword_565B30, a3, v14) )
    return -2147483646;
  if ( v12 != v14[2] || v13 != v14[3] )
    return -2147483645;
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  a1[84] = a2;
  v5 = dword_565B30;
  v6 = dword_565B30;
  a1[77] = 1;
  a1[85] = a3;
  a1[86] = sub_408300(a2, v6);
  v7 = sub_408300(a3, v5);
  v8 = v12;
  a1[87] = v7;
  a1[145] = a5;
  v9 = v13;
  a1[144] = a4;
  (*(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116))(a1, v8, v9);
  return 0;
}

// ===== sub_4275B0 @ 0x004275B0..0x0042778B =====
int __userpurge sub_4275B0@<eax>(int *a1@<edi>, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  unsigned int v8; // ebx
  int v9; // edx
  int v10; // eax
  int v11; // edx
  int v12; // ecx
  int v13; // edx
  int v14; // ecx
  int v15; // edx
  int v16; // eax
  int v17; // ecx
  int v18; // edx
  unsigned int v19; // eax
  int v20; // ecx
  int v21; // edx
  void (__thiscall *v22)(int *, unsigned int, unsigned int); // eax
  unsigned int v24; // [esp-4h] [ebp-54h]
  int v25; // [esp+Ch] [ebp-44h] BYREF
  int v26; // [esp+10h] [ebp-40h] BYREF
  int v27; // [esp+14h] [ebp-3Ch] BYREF
  unsigned int v28; // [esp+18h] [ebp-38h] BYREF
  unsigned int v29; // [esp+1Ch] [ebp-34h] BYREF
  int v30; // [esp+20h] [ebp-30h] BYREF
  int v31; // [esp+24h] [ebp-2Ch]
  int v32; // [esp+28h] [ebp-28h]
  int v33; // [esp+2Ch] [ebp-24h]
  _DWORD v34[2]; // [esp+30h] [ebp-20h] BYREF
  _DWORD v35[6]; // [esp+38h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, v35) )
    return -2147483647;
  v32 = a3 << 16;
  v33 = a4 << 16;
  sub_4296C0(&v30, &v25, &v26, &v27, a5, a6, a7, 0x10000, 0x10000, a3 << 16, a4 << 16);
  sub_4291E0(&v29, &v28, v34, v35[2], v35[3], v25, v26, v30, v31);
  v8 = v29;
  if ( v29 < 2 || v28 < 2 )
    return -2147483644;
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  v9 = dword_565B30;
  a1[77] = 2;
  a1[84] = a2;
  v10 = sub_408300(a2, v9);
  v11 = v33;
  v12 = v32;
  a1[86] = v10;
  a1[147] = v11;
  a1[148] = a5;
  a1[146] = v12;
  a1[154] = a7;
  v13 = v31;
  a1[155] = 0x10000;
  a1[156] = 0x10000;
  a1[153] = a6;
  v14 = v30;
  a1[162] = v13;
  v15 = v27;
  a1[160] = a8;
  v16 = v25;
  a1[161] = v14;
  v17 = v26;
  a1[168] = v15;
  v18 = v34[1];
  a1[163] = v16;
  v19 = v28;
  a1[167] = v17;
  v20 = v34[0];
  a1[184] = v18;
  v21 = *a1;
  v24 = v19;
  a1[182] = v19;
  v22 = *(void (__thiscall **)(int *, unsigned int, unsigned int))(v21 + 116);
  a1[183] = v20;
  a1[145] = -1;
  a1[157] = 0;
  a1[181] = v8;
  v22(a1, v8, v24);
  return 0;
}

// ===== sub_427790 @ 0x00427790..0x004278AD =====
int __stdcall sub_427790(_DWORD *a1, int a2, int a3)
{
  int v3; // edx
  int v4; // eax
  int v5; // ecx
  void (__thiscall *v6)(_DWORD *, int, int); // edx
  int v8; // [esp-4h] [ebp-44h]
  int v9; // [esp+10h] [ebp-30h] BYREF
  int v10; // [esp+18h] [ebp-28h]
  int v11; // [esp+1Ch] [ebp-24h]
  int v12[6]; // [esp+28h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, &v9) )
    return -2147483647;
  if ( !sub_407F20(dword_565B30, a3, v12) )
    return -2147483646;
  if ( v12[4] != 3 )
    return -2147483638;
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  sub_409030(3, v11, a1 + 208, v10);
  sub_40A620((int)(a1 + 208), 0);
  sub_40A530(v12, a1 + 208, 0, 0, 128, 0);
  v3 = dword_565B30;
  a1[77] = 3;
  a1[84] = a2;
  v4 = sub_408300(a2, v3);
  v5 = v10;
  v6 = *(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116);
  a1[86] = v4;
  v8 = v11;
  a1[145] = -1;
  v6(a1, v5, v8);
  return 0;
}

// ===== sub_4278B0 @ 0x004278B0..0x00427A9E =====
int __userpurge sub_4278B0@<eax>(_DWORD *a1@<edi>, int a2, int a3, int a4, unsigned int a5, int a6)
{
  void *v6; // esi
  int v7; // esi
  int v8; // eax
  void *v9; // ecx
  int v10; // eax
  void (__thiscall *v11)(_DWORD *, int); // edx
  int v13; // [esp+8h] [ebp-38h] BYREF
  void *v14; // [esp+Ch] [ebp-34h]
  int v15; // [esp+10h] [ebp-30h] BYREF
  int v16; // [esp+18h] [ebp-28h]
  int v17; // [esp+1Ch] [ebp-24h]
  _DWORD v18[6]; // [esp+28h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, &v15) )
    return -2147483647;
  if ( !v16 || !v17 )
    return -2147483638;
  if ( !sub_407F20(dword_565B30, a3, v18) )
    return -2147483643;
  if ( v18[4] != 6 || v18[2] != v16 || v18[3] != v17 )
    return -2147483642;
  if ( !a4 )
    return -2147483641;
  v6 = operator new[](16 * a4);
  v14 = v6;
  if ( sub_409FF0(a5, dword_565B30, &v13, 0, a4) )
  {
    operator delete[](v6);
    return -2147483640;
  }
  else if ( v13 )
  {
    sub_428FB0();
    sub_4290F0();
    sub_429140();
    sub_429160();
    sub_4291B0();
    v7 = dword_565B30;
    a1[201] = a3;
    a1[77] = 4;
    a1[145] = -1;
    a1[84] = a2;
    a1[86] = sub_408300(a2, v7);
    v8 = sub_408300(a3, v7);
    v9 = v14;
    a1[202] = v8;
    a1[204] = v9;
    a1[203] = a4;
    v10 = *a1;
    a1[205] = a5;
    v11 = *(void (__thiscall **)(_DWORD *, int))(v10 + 72);
    a1[206] = 0;
    v11(a1, a6);
    (*(void (__thiscall **)(_DWORD *, int, int))(*a1 + 116))(a1, v16, v17);
    return 0;
  }
  else
  {
    return -2147483639;
  }
}

// ===== sub_427AA0 @ 0x00427AA0..0x00427D8C =====
int __userpurge sub_427AA0@<eax>(
        _DWORD *a1@<eax>,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        unsigned int a9,
        int a10,
        int a11)
{
  __int64 v13; // rax
  unsigned int v14; // ebx
  int v15; // eax
  int v16; // edx
  int v17; // eax
  BOOL v18; // esi
  int v19; // eax
  int v20; // eax
  int v22; // ecx
  unsigned int v23; // esi
  int v24; // eax
  int v25; // ecx
  int v26; // edx
  int v27; // eax
  unsigned int v28; // ebx
  int v29; // ecx
  int v30; // edx
  int v31; // eax
  int v32; // [esp+10h] [ebp-58h] BYREF
  int v33; // [esp+14h] [ebp-54h] BYREF
  int v34; // [esp+18h] [ebp-50h] BYREF
  unsigned int v35; // [esp+1Ch] [ebp-4Ch]
  unsigned int v36; // [esp+20h] [ebp-48h] BYREF
  BOOL v37; // [esp+24h] [ebp-44h]
  int v38; // [esp+28h] [ebp-40h]
  int v39; // [esp+2Ch] [ebp-3Ch]
  int v40; // [esp+30h] [ebp-38h] BYREF
  int v41; // [esp+34h] [ebp-34h]
  int v42; // [esp+38h] [ebp-30h] BYREF
  int v43; // [esp+3Ch] [ebp-2Ch]
  int v44; // [esp+40h] [ebp-28h]
  int v45; // [esp+44h] [ebp-24h]
  int v46; // [esp+48h] [ebp-20h]
  int v47; // [esp+50h] [ebp-18h] BYREF
  int v48; // [esp+58h] [ebp-10h]
  int v49; // [esp+5Ch] [ebp-Ch]
  int v50; // [esp+60h] [ebp-8h]

  if ( !sub_407F20(dword_565B30, a2, &v47) )
    return -2147483647;
  v37 = a3 != -1;
  if ( a3 != -1 )
  {
    if ( !sub_407F20(dword_565B30, a3, &v42) )
      return -2147483646;
    if ( v48 != v44 || v49 != v45 || v50 != v46 )
      return -2147483645;
  }
  v38 = a6 << 16;
  v39 = a7 << 16;
  sub_41B4C0(a1, &v42);
  LODWORD(v13) = v44;
  sub_41AAA0(v13, a9, &v36);
  v14 = v36;
  sub_4296C0(&v40, &v32, &v33, &v34, a8, v36, v36, 0x10000, 0x10000, v38, a7 << 16);
  if ( a1[192] )
    v15 = a1[194];
  else
    v15 = 0;
  sub_429220(&v42, v48, v49, v15, v32, v33, v34, v42, v43, v40, v41);
  if ( v35 < 2 || v36 < 2 )
    return -2147483644;
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  sub_4291B0();
  a1[85] = a3;
  v16 = dword_565B30;
  a1[77] = 5;
  a1[84] = a2;
  v17 = sub_408300(a2, v16);
  v18 = v37;
  a1[86] = v17;
  if ( v18 )
    v19 = sub_408300(a3, dword_565B30);
  else
    v19 = 0;
  a1[87] = v19;
  v20 = v18 ? a4 : 0;
  a1[144] = v20;
  a1[142] = v20;
  if ( !v18 )
    a5 = -1;
  v22 = v39;
  v23 = v36;
  a1[145] = a5;
  a1[146] = v38;
  a1[155] = 0x10000;
  a1[156] = 0x10000;
  a1[147] = v22;
  a1[148] = a8;
  a1[158] = a9;
  v24 = v40;
  a1[159] = a10;
  v25 = v41;
  a1[160] = a11;
  v26 = v32;
  a1[161] = v24;
  v27 = v33;
  a1[153] = v14;
  a1[154] = v14;
  v28 = v35;
  a1[162] = v25;
  v29 = v34;
  a1[163] = v26;
  v30 = v42;
  a1[167] = v27;
  v31 = v43;
  a1[157] = 0;
  a1[168] = v29;
  a1[181] = v28;
  a1[182] = v23;
  a1[183] = v30;
  a1[184] = v31;
  sub_42A650(a1);
  sub_429AF0(a1);
  sub_428E70(1);
  sub_429000();
  (*(void (__thiscall **)(_DWORD *, unsigned int, unsigned int))(*a1 + 116))(a1, v28, v23);
  return 0;
}

// ===== sub_427D90 @ 0x00427D90..0x00427F78 =====
int __userpurge sub_427D90@<eax>(
        _DWORD *a1@<eax>,
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
        int a14)
{
  int v16; // edx
  int v17; // eax
  int v18; // eax
  int v20; // ecx
  int v21; // [esp+10h] [ebp-38h] BYREF
  int v22; // [esp+14h] [ebp-34h]
  _DWORD v23[6]; // [esp+18h] [ebp-30h] BYREF
  _DWORD v24[6]; // [esp+30h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, v23) )
    return -2147483647;
  if ( a3 != -1 )
  {
    if ( !sub_407F20(dword_565B30, a3, v24) )
      return -2147483646;
    if ( v23[2] != v24[2] || v23[3] != v24[3] || v23[4] != v24[4] )
      return -2147483645;
  }
  sub_428FB0();
  sub_4290F0();
  sub_429140();
  sub_429160();
  v16 = dword_565B30;
  v22 = a7 << 16;
  v21 = a6 << 16;
  a1[77] = 6;
  a1[84] = a2;
  a1[85] = a3;
  a1[86] = sub_408300(a2, v16);
  if ( a3 == -1 )
    v17 = 0;
  else
    v17 = sub_408300(a3, dword_565B30);
  a1[87] = v17;
  v18 = a3 != -1 ? a4 : 0;
  a1[144] = v18;
  a1[142] = v18;
  if ( a3 == -1 )
    a5 = -1;
  v20 = v22;
  a1[145] = a5;
  a1[147] = v20;
  a1[146] = a6 << 16;
  a1[149] = a8;
  a1[150] = a9;
  a1[151] = a10;
  a1[152] = a11;
  a1[153] = 0x10000;
  a1[154] = 0x10000;
  a1[155] = 0x10000;
  a1[156] = 0x10000;
  a1[157] = 0;
  a1[158] = a12;
  a1[159] = a13;
  a1[160] = a14;
  sub_42A650(a1);
  sub_428E70(1);
  sub_429000();
  sub_42A100(a1, &v21, a8, a9, a10, a11, a12, 0x10000, 0x10000, 0x10000, 0x10000);
  return 0;
}

// ===== sub_427F80 @ 0x00427F80..0x0042801B =====
int __userpurge sub_427F80@<eax>(_DWORD *a1@<edi>, int a2)
{
  int result; // eax
  int v3; // edx
  _DWORD v4[4]; // [esp+8h] [ebp-1Ch] BYREF
  int v5; // [esp+18h] [ebp-Ch]

  if ( a2 == -1 )
  {
    result = 0;
    a1[78] = 0;
    a1[79] = -1;
  }
  else
  {
    sub_428CD0(0);
    if ( sub_407F20(dword_565B30, a2, v4) )
    {
      if ( v5 == 3 || v5 == 2 )
      {
        v3 = dword_565B30;
        a1[78] = 1;
        a1[79] = a2;
        a1[80] = sub_408300(a2, v3);
        return 0;
      }
      else
      {
        return -2147483638;
      }
    }
    else
    {
      return -2147483647;
    }
  }
  return result;
}
