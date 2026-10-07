#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_41C020 @ 0x0041C020..0x0041C04D =====
int __userpurge sub_41C020@<eax>(int a1@<eax>, _DWORD *a2@<esi>, _DWORD *a3)
{
  int v3; // ecx
  int v4; // edx
  int result; // eax

  v3 = *(_DWORD *)(a1 + 152);
  if ( !v3 )
    return 0;
  v4 = *(_DWORD *)(a1 + 156);
  if ( !v4 )
    return 0;
  *a2 = v3;
  result = 1;
  *a3 = v4;
  return result;
}

// ===== sub_41C050 @ 0x0041C050..0x0041C05C =====
BOOL __thiscall sub_41C050(_DWORD *this)
{
  return this[36] != 0;
}

// ===== sub_41C060 @ 0x0041C060..0x0041C096 =====
_DWORD *__usercall sub_41C060@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  *result = a2[36];
  result[1] = a2[37];
  result[2] = a2[38];
  result[3] = a2[39];
  v2 = a2[40];
  v3 = a2[41];
  result[4] = v2;
  result[5] = v3;
  return result;
}

// ===== sub_41C0A0 @ 0x0041C0A0..0x0041C0AB =====
int sub_41C0A0()
{
  return sub_442E10(dword_565B2C);
}

// ===== sub_41C0B0 @ 0x0041C0B0..0x0041C0B7 =====
int __usercall sub_41C0B0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 256) = a2;
  return result;
}

// ===== sub_41C0C0 @ 0x0041C0C0..0x0041C0C7 =====
int __usercall sub_41C0C0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 256);
}

// ===== sub_41C0D0 @ 0x0041C0D0..0x0041C0D5 =====
// attributes: thunk
int sub_41C0D0()
{
  return sub_442E90();
}

// ===== sub_41C0E0 @ 0x0041C0E0..0x0041C0FD =====
int __fastcall sub_41C0E0(_DWORD *a1, int a2)
{
  int result; // eax
  int v3; // edx

  result = 0;
  if ( *(_DWORD *)(a2 + 72) )
  {
    v3 = dword_565B38;
    *a1 = dword_565B34;
    a1[1] = v3;
    return 1;
  }
  return result;
}

// ===== sub_41C100 @ 0x0041C100..0x0041C104 =====
int __usercall sub_41C100@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 32);
}

// ===== sub_41C110 @ 0x0041C110..0x0041C117 =====
int __usercall sub_41C110@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 260) = a2;
  return result;
}

// ===== sub_41C120 @ 0x0041C120..0x0041C127 =====
int __usercall sub_41C120@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 260);
}

// ===== sub_41C130 @ 0x0041C130..0x0041C18E =====
BOOL __usercall sub_41C130@<eax>(_DWORD *a1@<ecx>, int a2@<edi>)
{
  _DWORD *v2; // esi
  _DWORD v4[2]; // [esp+4h] [ebp-10h] BYREF
  _DWORD v5[2]; // [esp+Ch] [ebp-8h] BYREF

  v2 = (_DWORD *)a1[75];
  if ( v2 )
  {
    while ( *v2 != a2 )
    {
      v2 = (_DWORD *)v2[3];
      if ( !v2 )
        return 0;
    }
    (*(void (__thiscall **)(_DWORD *, _DWORD *))(*a1 + 48))(a1, v4);
    (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)a2 + 48))(a2, v5);
    v2[1] = v5[0] - v4[0];
    v2[2] = v5[1] - v4[1];
  }
  return v2 != 0;
}

// ===== sub_41C190 @ 0x0041C190..0x0041C1BD =====
_DWORD *__thiscall sub_41C190(void *this, _DWORD *a2)
{
  _DWORD v3[3]; // [esp+0h] [ebp-Ch] BYREF

  (*(void (__thiscall **)(void *, _DWORD *))(*(_DWORD *)this + 52))(this, v3);
  return sub_409170(-v3[1], -v3[0], a2);
}

// ===== sub_41C1C0 @ 0x0041C1C0..0x0041C1C7 =====
int __usercall sub_41C1C0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 188) = a2;
  return result;
}

// ===== sub_41C1D0 @ 0x0041C1D0..0x0041C1D7 =====
int __usercall sub_41C1D0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 188);
}

// ===== sub_41C1E0 @ 0x0041C1E0..0x0041C1F1 =====
BOOL __usercall sub_41C1E0@<eax>(int a1@<edx>, unsigned int a2@<ecx>, int a3@<esi>)
{
  BOOL result; // eax

  result = a2 < 0x10;
  if ( a2 < 0x10 )
    *(_DWORD *)(a1 + 4 * a2 + 192) = a3;
  return result;
}

// ===== sub_41C200 @ 0x0041C200..0x0041C21C =====
BOOL __fastcall sub_41C200(unsigned int a1, int a2, _DWORD *a3)
{
  BOOL result; // eax

  result = a1 < 0x10;
  if ( a1 < 0x10 )
    *a3 = *(_DWORD *)(a2 + 4 * a1 + 192);
  return result;
}

// ===== sub_41C220 @ 0x0041C220..0x0041C29D =====
_DWORD *__stdcall sub_41C220(_DWORD *a1, int a2)
{
  sub_41A400(0, a1, 0, 1);
  *a1 = &CDspObjBack::`vftable';
  a1[77] = a2;
  a1[78] = 0;
  sub_41C390(a1);
  sub_41AE00(a1, 1);
  return a1;
}

// ===== sub_41C2A0 @ 0x0041C2A0..0x0041C2C2 =====
void *__thiscall sub_41C2A0(void *this, char a2)
{
  sub_41C2D0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41C2D0 @ 0x0041C2D0..0x0041C318 =====
int __stdcall sub_41C2D0(_DWORD *a1)
{
  *a1 = &CDspObjBack::`vftable';
  return sub_41A5E0(a1);
}

// ===== sub_41C320 @ 0x0041C320..0x0041C330 =====
int __thiscall sub_41C320(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[78] = a2;
  return result;
}

// ===== sub_41C330 @ 0x0041C330..0x0041C33F =====
int __thiscall sub_41C330(_DWORD *this)
{
  int result; // eax

  if ( this[78] )
    return sub_430D10();
  return result;
}

// ===== sub_41C340 @ 0x0041C340..0x0041C372 =====
int __thiscall sub_41C340(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax

  if ( !this[78] )
    return sub_40A620(a2, 0);
  result = (*(int (__thiscall **)(_DWORD *, int, int))(*this + 132))(this, a2, a3);
  if ( !result )
    return sub_40A620(a2, 0);
  return result;
}

// ===== sub_41C380 @ 0x0041C380..0x0041C38B =====
int __thiscall sub_41C380(void *this)
{
  return (*(int (__thiscall **)(void *))(*(_DWORD *)this + 32))(this);
}

// ===== sub_41C390 @ 0x0041C390..0x0041C3DC =====
int __thiscall sub_41C390(_DWORD *this)
{
  int result; // eax
  _DWORD v3[6]; // [esp+8h] [ebp-30h] BYREF
  int v4; // [esp+28h] [ebp-10h]
  int v5; // [esp+2Ch] [ebp-Ch]
  int v6; // [esp+30h] [ebp-8h]

  sub_41C0A0();
  sub_41C060(v3, this);
  result = 0;
  if ( v4 != v3[2] || v5 != v3[3] || v6 != v3[4] )
  {
    (*(void (__thiscall **)(_DWORD *, int, int))(*this + 116))(this, v4, v5);
    return 1;
  }
  return result;
}

// ===== sub_41C3E0 @ 0x0041C3E0..0x0041C41C =====
BOOL __stdcall sub_41C3E0(_DWORD *a1)
{
  int v2; // [esp+8h] [ebp-10h]
  int v3; // [esp+Ch] [ebp-Ch]
  int v4; // [esp+10h] [ebp-8h]

  sub_41C0A0();
  return a1[2] == v2 && a1[3] == v3 && a1[4] == v4;
}

// ===== sub_41C420 @ 0x0041C420..0x0041C45D =====
int __usercall sub_41C420@<eax>(int a1@<ecx>, int a2@<edi>)
{
  _DWORD v3[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(dword_565B30, a1, v3) )
    return (*(int (__thiscall **)(int, _DWORD *))(*(_DWORD *)a2 + 128))(a2, v3);
  else
    return 0;
}

// ===== sub_41C460 @ 0x0041C460..0x0041C4C9 =====
int __stdcall sub_41C460(_DWORD *a1)
{
  sub_41C220(a1, 2);
  *a1 = &CDspObjBackB::`vftable';
  a1[79] = -1;
  a1[80] = -1;
  return sub_41B600((int)a1, 1);
}

// ===== sub_41C4D0 @ 0x0041C4D0..0x0041C4F2 =====
void *__thiscall sub_41C4D0(void *this, char a2)
{
  sub_41C500(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41C500 @ 0x0041C500..0x0041C549 =====
int __stdcall sub_41C500(_DWORD *a1)
{
  *a1 = &CDspObjBackB::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41C550 @ 0x0041C550..0x0041C601 =====
int __userpurge sub_41C550@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3)
{
  int v5; // ecx
  int v6; // esi
  int v7; // edx
  int v9; // edx

  if ( sub_41C420(a1, (int)a2) )
  {
    if ( a3 == 28672 || a3 == 28673 )
    {
      v9 = dword_565B30;
      a2[79] = a1;
      a2[80] = a3;
      a2[81] = sub_408300(a1, v9);
      return 1;
    }
    if ( sub_41C420(a3, (int)a2) )
    {
      a2[79] = a1;
      v5 = a1;
      v6 = dword_565B30;
      v7 = dword_565B30;
      a2[80] = a3;
      a2[81] = sub_408300(v5, v7);
      a2[82] = sub_408300(a3, v6);
      return 1;
    }
  }
  return 0;
}

// ===== sub_41C610 @ 0x0041C610..0x0041C773 =====
int __thiscall sub_41C610(_DWORD *this, int a2, int *a3)
{
  unsigned int v4; // eax
  int v5; // ecx
  unsigned int v6; // ebx
  int v8; // ecx
  unsigned int v9; // edi
  unsigned int v10; // [esp+Ch] [ebp-34h]
  _DWORD v11[6]; // [esp+10h] [ebp-30h] BYREF
  _DWORD v12[6]; // [esp+28h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v11) || this[81] != sub_408300(this[79], dword_565B30) )
    return 0;
  v4 = sub_41B770(this);
  v5 = this[80];
  v6 = v4;
  v10 = v4;
  if ( v5 == 28672 || v5 == 28673 )
  {
    v8 = v5 - 28672;
    v9 = 128;
    if ( v8 )
    {
      if ( v8 == 1 )
        v9 = 193;
    }
    else
    {
      v9 = 192;
    }
    sub_4091B0(v11, a3);
    sub_40A9E0(a2, (int)v11, v9, v6, 1);
    return 1;
  }
  else if ( sub_407F20(dword_565B30, v5, v12) && this[82] == sub_408300(this[80], dword_565B30) )
  {
    sub_4091B0(v12, a3);
    sub_40A9E0(a2, (int)v12, 0x80u, 0, 1);
    sub_4091B0(v11, a3);
    sub_40A9E0(a2, (int)v11, 0xF0u, v10, 1);
    return 1;
  }
  else
  {
    return 0;
  }
}

// ===== sub_41C780 @ 0x0041C780..0x0041C807 =====
_DWORD *__stdcall sub_41C780(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v2; // edx
  _DWORD *v3; // esi
  void *v4; // edi

  sub_41C220(a1, 5);
  *a1 = &CDspObjBackD::`vftable';
  v1 = a1 + 82;
  v2 = 32;
  do
  {
    v3 = v1 + 1;
    *(v1 - 1) = -1;
    *v1 = 0;
    v1[2] = 0;
    v4 = v1 + 3;
    v1 += 66;
    --v2;
    *v3 = 0;
    qmemcpy(v4, v3, 0xF8u);
  }
  while ( v2 );
  return a1;
}

// ===== sub_41C810 @ 0x0041C810..0x0041C832 =====
void *__thiscall sub_41C810(void *this, char a2)
{
  sub_41C840(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41C840 @ 0x0041C840..0x0041C898 =====
int __stdcall sub_41C840(_DWORD *a1)
{
  *a1 = &CDspObjBackD::`vftable';
  sub_41CD10(a1);
  return sub_41C2D0(a1);
}

// ===== sub_41C8A0 @ 0x0041C8A0..0x0041C921 =====
int __thiscall sub_41C8A0(_DWORD *this)
{
  int result; // eax
  unsigned int v3; // ecx
  int v4; // edx
  int *v5; // edi
  int v6; // [esp+4h] [ebp-8h]
  int v7; // [esp+8h] [ebp-4h]

  result = (*(int (__thiscall **)(_DWORD *))(*this + 76))(this);
  if ( (unsigned int)result < 0x20 )
  {
    v3 = this[79];
    if ( v3 >= this[80] )
    {
      return sub_41C330(this);
    }
    else
    {
      v4 = result + v3 + 32 * result;
      result = 0;
      v5 = &this[2 * v4 + 83];
      v6 = 0;
      if ( *v5 > 0 )
      {
        v7 = 0;
        do
        {
          (*(void (__thiscall **)(_DWORD *))(*this + 28))(this);
          sub_443240(dword_565B2C);
          v7 += 16;
          result = v6 + 1;
          v6 = result;
        }
        while ( result < *v5 );
      }
    }
  }
  return result;
}

// ===== sub_41C930 @ 0x0041C930..0x0041C943 =====
int __thiscall sub_41C930(_DWORD *this, int a2)
{
  this[79] = -1;
  return sub_41AE00(this, a2);
}

// ===== sub_41C950 @ 0x0041C950..0x0041C963 =====
int __thiscall sub_41C950(_DWORD *this, int a2)
{
  this[79] = -1;
  return sub_41C320(this, a2);
}

// ===== sub_41C970 @ 0x0041C970..0x0041CA6C =====
int __stdcall sub_41C970(int a1, int a2, int a3)
{
  int v3; // edi
  int v4; // eax
  int v5; // ebx
  int *v6; // esi
  int v7; // ecx
  int v8; // edx
  _DWORD *v9; // ebx
  _DWORD *v10; // edi
  _DWORD *v11; // esi
  int v13; // [esp+10h] [ebp-8h]
  int v14; // [esp+14h] [ebp-4h]

  v3 = a1;
  sub_41CD10(a1);
  v4 = a2;
  if ( (unsigned int)(a2 - 2) > 0x1E )
    return -2147483647;
  v5 = 0;
  if ( a2 > 0 )
  {
    v6 = (int *)(a1 + 328);
    do
    {
      if ( !sub_41C420(*(_DWORD *)(a3 + 4 * v5), a1) )
        break;
      v7 = *(_DWORD *)(a3 + 4 * v5);
      v8 = dword_565B30;
      *(v6 - 1) = v7;
      *v6 = sub_408300(v7, v8);
      ++v5;
      v6 += 66;
    }
    while ( v5 < a2 );
    v4 = a2;
  }
  if ( v5 == v4 )
  {
    *(_DWORD *)(a1 + 320) = v4;
    if ( v4 > 0 )
    {
      v9 = (_DWORD *)(a1 + 336);
      v14 = v4;
      while ( 1 )
      {
        v10 = (_DWORD *)(v3 + 324);
        v11 = v9;
        v13 = a2;
        do
        {
          *v11 = operator new[](0x1800u);
          sub_41CAF0(v11 - 1, *v10, *(v9 - 3));
          v11 += 2;
          v10 += 66;
          --v13;
        }
        while ( v13 );
        v9 += 66;
        if ( !--v14 )
          break;
        v3 = a1;
      }
    }
    return 0;
  }
  else
  {
    sub_41CD10(a1);
    return -2147483646;
  }
}

// ===== sub_41CA70 @ 0x0041CA70..0x0041CA8C =====
int __thiscall sub_41CA70(_DWORD *this)
{
  sub_41CD10(this);
  return sub_41C390(this);
}

// ===== sub_41CA90 @ 0x0041CA90..0x0041CAB1 =====
void __thiscall sub_41CA90(_DWORD *this, int a2, int a3, int a4)
{
  if ( a2 == -268435456 )
    this[79] = (*(int (__thiscall **)(_DWORD *))(*this + 76))(this);
}

// ===== sub_41CAC0 @ 0x0041CAC0..0x0041CAEF =====
unsigned int *__stdcall sub_41CAC0(unsigned int *a1, unsigned int *a2)
{
  unsigned int *result; // eax
  unsigned int v3; // [esp+8h] [ebp-10h]
  unsigned int v4; // [esp+Ch] [ebp-Ch]

  sub_41C0A0();
  *a1 = v3 >> 5;
  result = a2;
  *a2 = v4 / 0x18;
  return result;
}

// ===== sub_41CAF0 @ 0x0041CAF0..0x0041CD10 =====
int __userpurge sub_41CAF0@<eax>(int a1@<eax>, _DWORD *a2, int a3, int a4)
{
  int v5; // edi
  int v6; // edx
  _DWORD *v7; // esi
  int v8; // ecx
  BOOL v9; // eax
  int v10; // edx
  _DWORD *v11; // ebx
  __int16 v12; // ax
  bool v13; // cc
  int v14; // ecx
  int v15; // edx
  int v16; // edx
  int v18; // [esp+20h] [ebp-64h]
  int v19; // [esp+28h] [ebp-5Ch] BYREF
  int v20; // [esp+2Ch] [ebp-58h]
  int v21; // [esp+3Ch] [ebp-48h]
  int v22; // [esp+40h] [ebp-44h] BYREF
  int v23; // [esp+44h] [ebp-40h]
  int v24; // [esp+54h] [ebp-30h]
  int v25; // [esp+58h] [ebp-2Ch]
  int v26; // [esp+5Ch] [ebp-28h]
  int v27; // [esp+60h] [ebp-24h]
  int v28; // [esp+64h] [ebp-20h]
  int v29; // [esp+68h] [ebp-1Ch]
  int v30; // [esp+6Ch] [ebp-18h]
  int v31; // [esp+70h] [ebp-14h]
  _DWORD *v32; // [esp+74h] [ebp-10h]
  int v33; // [esp+78h] [ebp-Ch]
  _DWORD *v34; // [esp+7Ch] [ebp-8h]

  sub_41C0A0();
  if ( !sub_407F20(dword_565B30, a3, &v22) )
    return -2147483645;
  if ( !(*(int (__thiscall **)(int, int *))(*(_DWORD *)a1 + 128))(a1, &v22) )
    return -2147483644;
  if ( !sub_407F20(dword_565B30, a4, &v19) )
    return -2147483643;
  if ( !(*(int (__thiscall **)(int, int *))(*(_DWORD *)a1 + 128))(a1, &v19) )
    return -2147483642;
  sub_41CAC0((unsigned int *)&a3, (unsigned int *)&a4);
  v5 = a3;
  v6 = 0;
  *a2 = 0;
  v33 = 0;
  v29 = 0;
  while ( 1 )
  {
    v7 = 0;
    v31 = 1;
    v26 = 0;
    v30 = 0;
    do
    {
      v8 = v22 + v5 * v6 * v24 + a4 * v33 * v23;
      v9 = 0;
      v10 = v19 + v5 * v6 * v21 + a4 * v33 * v20;
      v27 = 0;
      if ( a4 <= 0 )
      {
LABEL_24:
        v31 = 1;
      }
      else
      {
        while ( !v9 )
        {
          v11 = (_DWORD *)v10;
          v34 = (_DWORD *)v8;
          v32 = (_DWORD *)v10;
          v28 = 0;
          if ( v5 > 0 )
          {
            while ( !v9 )
            {
              if ( v18 )
              {
                if ( v18 == 1 )
                  v9 = ((*v34 ^ *v11) & 0xFFFFFF) != 0;
              }
              else
              {
                v12 = *(_WORD *)v34;
                v25 = v8;
                v9 = v12 != *(_WORD *)v11;
              }
              v34 = (_DWORD *)((char *)v34 + v24);
              v32 = (_DWORD *)((char *)v32 + v21);
              if ( ++v28 >= a3 )
                break;
              v11 = v32;
            }
          }
          v8 += v23;
          v10 += v20;
          v13 = ++v27 < a4;
          v5 = a3;
          if ( !v13 )
          {
            if ( !v9 )
              goto LABEL_24;
            break;
          }
        }
        if ( v31 )
        {
          v14 = v29;
          v15 = a4;
          v7 = (_DWORD *)(a2[1] + 16 * *a2);
          v31 = 0;
          v7[1] = v29;
          v7[3] = v14 + v15 - 1;
          v16 = v30 + v5 - 1;
          *v7 = v30;
          v7[2] = v16;
          ++*a2;
        }
        else
        {
          v7[2] += v5;
        }
      }
      v30 += v5;
      v6 = v26 + 1;
      v26 = v6;
    }
    while ( v6 < 32 );
    v29 += a4;
    if ( ++v33 >= 24 )
      return 0;
    v6 = 0;
  }
}

// ===== sub_41CD10 @ 0x0041CD10..0x0041CD84 =====
_DWORD *__stdcall sub_41CD10(_DWORD *a1)
{
  _DWORD *v1; // edi
  void **v2; // esi
  int v3; // ebx
  _DWORD *result; // eax
  int v5; // [esp+Ch] [ebp-4h]

  v1 = a1 + 82;
  v5 = 32;
  do
  {
    *(v1 - 1) = -1;
    *v1 = 0;
    v2 = (void **)(v1 + 2);
    v3 = 32;
    do
    {
      if ( *v2 )
      {
        operator delete[](*v2);
        *(v2 - 1) = 0;
        *v2 = 0;
      }
      v2 += 2;
      --v3;
    }
    while ( v3 );
    v1 += 66;
    --v5;
  }
  while ( v5 );
  result = a1;
  a1[80] = 0;
  a1[79] = -1;
  return result;
}

// ===== sub_41CD90 @ 0x0041CD90..0x0041CE22 =====
int __thiscall sub_41CD90(_DWORD *this, int a2, int *a3)
{
  unsigned int v4; // eax
  int *v5; // edi
  _DWORD v7[6]; // [esp+10h] [ebp-18h] BYREF

  v4 = (*(int (__thiscall **)(_DWORD *))(*this + 76))(this);
  if ( v4 >= this[80] )
    return 0;
  v5 = &this[66 * v4 + 81];
  if ( !sub_407F20(dword_565B30, *v5, v7) || v5[1] != sub_408300(*v5, dword_565B30) )
    return 0;
  sub_4091B0(v7, a3);
  sub_40A9E0(a2, (int)v7, 0x80u, 0, 1);
  return 1;
}

// ===== sub_41CE30 @ 0x0041CE30..0x0041CE97 =====
_DWORD *__stdcall sub_41CE30(_DWORD *a1)
{
  sub_41C220(a1, 6);
  *a1 = &CDspObjBackDST::`vftable';
  a1[79] = -1;
  a1[80] = -1;
  a1[81] = -1;
  return a1;
}

// ===== sub_41CEA0 @ 0x0041CEA0..0x0041CEC2 =====
void *__thiscall sub_41CEA0(void *this, char a2)
{
  sub_41CED0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41CED0 @ 0x0041CED0..0x0041CF19 =====
int __stdcall sub_41CED0(_DWORD *a1)
{
  *a1 = &CDspObjBackDST::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41CF20 @ 0x0041CF20..0x0041D04E =====
int __userpurge sub_41CF20@<eax>(_DWORD *a1@<edi>, int a2, int a3, int a4)
{
  int v4; // ecx
  int v6; // esi
  _DWORD v7[6]; // [esp+8h] [ebp-30h] BYREF
  _DWORD v8[6]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, v7) )
    return -2147483647;
  if ( !(*(int (__thiscall **)(_DWORD *, _DWORD *))(*a1 + 128))(a1, v7) )
    return -2147483646;
  if ( !sub_407F20(dword_565B30, a3, v8) )
    return -2147483645;
  if ( !sub_41D060() )
    return -2147483644;
  v4 = a4;
  if ( a4 != -1 )
  {
    if ( !sub_407F20(dword_565B30, a4, v8) )
      return -2147483643;
    if ( !sub_41D060() )
      return -2147483642;
    v4 = a4;
  }
  v6 = dword_565B30;
  a1[81] = v4;
  a1[79] = a2;
  a1[80] = a3;
  a1[82] = sub_408300(a2, v6);
  a1[83] = sub_408300(a3, v6);
  a1[84] = sub_408300(a4, v6);
  return 0;
}

// ===== sub_41D050 @ 0x0041D050..0x0041D057 =====
int __usercall sub_41D050@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 340) = a2;
  return result;
}

// ===== sub_41D060 @ 0x0041D060..0x0041D095 =====
BOOL __usercall sub_41D060@<eax>(_DWORD *a1@<esi>)
{
  BOOL result; // eax
  int v2; // [esp+8h] [ebp-10h]
  int v3; // [esp+Ch] [ebp-Ch]

  result = 0;
  if ( a1[4] == 4 )
  {
    sub_41C0A0();
    return a1[2] == v2 && a1[3] == v3;
  }
  return result;
}

// ===== sub_41D0A0 @ 0x0041D0A0..0x0041D207 =====
BOOL __thiscall sub_41D0A0(_DWORD *this, _DWORD *a2, int *a3)
{
  int v4; // edi
  int v5; // edi
  BOOL result; // eax
  unsigned int v7; // eax
  int v8; // [esp-4h] [ebp-5Ch]
  int v9; // [esp+Ch] [ebp-4Ch]
  _DWORD v10[6]; // [esp+10h] [ebp-48h] BYREF
  _DWORD v11[6]; // [esp+28h] [ebp-30h] BYREF
  _DWORD v12[6]; // [esp+40h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v12) )
    return 0;
  v4 = dword_565B30;
  if ( this[82] != sub_408300(this[79], dword_565B30)
    || !sub_407F20(v4, this[80], v11)
    || this[83] != sub_408300(this[80], dword_565B30) )
  {
    return 0;
  }
  v5 = this[81];
  if ( v5 != -1 )
  {
    if ( sub_407F20(dword_565B30, this[81], v10) )
    {
      v5 = this[81];
      result = this[84] == sub_408300(v5, dword_565B30);
      v9 = result;
      if ( !result )
        return result;
      goto LABEL_8;
    }
    return 0;
  }
  v9 = 1;
LABEL_8:
  sub_4091B0(v12, a3);
  sub_4091B0(v11, a3);
  if ( v5 != -1 )
    sub_4091B0(v10, a3);
  v8 = this[85];
  v7 = sub_41B770(this);
  sub_412220(v11, (int)v12, a2, v5 != -1 ? v10 : 0, v7, v8);
  return v9;
}

// ===== sub_41D210 @ 0x0041D210..0x0041D28F =====
_DWORD *__stdcall sub_41D210(_DWORD *a1)
{
  sub_41C220(a1, 4);
  *a1 = &CDspObjBackF::`vftable';
  a1[81] = -1;
  a1[85] = -1;
  a1[87] = -1;
  sub_41B600((int)a1, 1);
  sub_41D510(0);
  return a1;
}

// ===== sub_41D290 @ 0x0041D290..0x0041D2B2 =====
void *__thiscall sub_41D290(void *this, char a2)
{
  sub_41D2C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41D2C0 @ 0x0041D2C0..0x0041D309 =====
int __stdcall sub_41D2C0(_DWORD *a1)
{
  *a1 = &CDspObjBackF::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41D310 @ 0x0041D310..0x0041D329 =====
int __thiscall sub_41D310(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[79] = a2;
  this[80] = a3;
  return result;
}

// ===== sub_41D330 @ 0x0041D330..0x0041D34B =====
_DWORD *__thiscall sub_41D330(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = this[79];
  a2[1] = this[80];
  return result;
}

// ===== sub_41D350 @ 0x0041D350..0x0041D438 =====
int __userpurge sub_41D350@<eax>(_DWORD *a1@<edi>, int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // esi
  int v8; // esi
  int v9; // eax
  int result; // eax
  _DWORD v11[6]; // [esp+8h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a4, v11) )
    return -2147483647;
  v7 = a7;
  if ( a7 == 28672 || a7 == 28673 || a7 == 0x7FFF || a7 == -1 )
  {
    a1[82] = sub_408300(a4, dword_565B30);
  }
  else
  {
    if ( !sub_407F20(dword_565B30, a7, v11) )
      return -2147483646;
    v8 = dword_565B30;
    a1[82] = sub_408300(a4, dword_565B30);
    v9 = sub_408300(a7, v8);
    v7 = a7;
    a1[86] = v9;
  }
  a1[79] = a2;
  a1[80] = a3;
  result = 0;
  a1[81] = a4;
  a1[83] = a5;
  a1[84] = a6;
  a1[85] = v7;
  return result;
}

// ===== sub_41D440 @ 0x0041D440..0x0041D4C4 =====
int __userpurge sub_41D440@<eax>(_DWORD *a1@<edi>, int a2, int a3)
{
  int v3; // edx
  _DWORD v5[6]; // [esp+8h] [ebp-18h] BYREF

  if ( a2 == -1 )
  {
    a1[87] = -1;
    return 0;
  }
  else if ( sub_407F20(dword_565B30, a2, v5) )
  {
    if ( v5[4] == 3 )
    {
      v3 = dword_565B30;
      a1[87] = a2;
      a1[88] = a3;
      a1[89] = sub_408300(a2, v3);
      return 0;
    }
    else
    {
      return -2147483644;
    }
  }
  else
  {
    return -2147483645;
  }
}

// ===== sub_41D4D0 @ 0x0041D4D0..0x0041D50A =====
int __thiscall sub_41D4D0(void *this, unsigned int a2, int a3, int a4)
{
  if ( a2 == 0x40000000 )
    return sub_41D510(a3) != 0 ? 0 : -65534;
  else
    return sub_41B8E0(this, a2, a3, a4);
}

// ===== sub_41D510 @ 0x0041D510..0x0041D53A =====
int __fastcall sub_41D510(int a1, unsigned int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( !*(_DWORD *)(a1 + 360) || a2 < 4 )
  {
    *(_DWORD *)(a1 + 360) = a3;
    *(_DWORD *)(a1 + 364) = a2;
    return 1;
  }
  return result;
}

// ===== sub_41D540 @ 0x0041D540..0x0041D581 =====
unsigned int __thiscall sub_41D540(_DWORD *this)
{
  unsigned int result; // eax

  result = 0;
  if ( this[90] )
  {
    switch ( this[91] )
    {
      case 1:
        return 256 - sub_41B770(this);
      case 2:
        return sub_41B740();
      case 3:
        return 256 - sub_41B740();
      default:
        return sub_41B770(this);
    }
  }
  return result;
}

// ===== sub_41D590 @ 0x0041D590..0x0041D90C =====
int __thiscall sub_41D590(_DWORD *this, _DWORD *a2, int *a3)
{
  BOOL v4; // edi
  unsigned int v5; // eax
  int v6; // ecx
  int v7; // edi
  int v8; // ecx
  unsigned int v9; // eax
  int v11; // ecx
  int v12; // esi
  int v13; // ecx
  int v14; // eax
  int v15; // ecx
  unsigned int v16; // eax
  unsigned int v17; // [esp+Ch] [ebp-54h]
  int v18; // [esp+10h] [ebp-50h]
  int Val; // [esp+14h] [ebp-4Ch]
  int v20[6]; // [esp+18h] [ebp-48h] BYREF
  _DWORD v21[6]; // [esp+30h] [ebp-30h] BYREF
  int v22[6]; // [esp+48h] [ebp-18h] BYREF

  v4 = 0;
  v18 = 0;
  if ( !sub_407F20(dword_565B30, this[81], v20) || this[82] != sub_408300(this[81], dword_565B30) )
    return v18;
  v5 = sub_41B770(this);
  v6 = this[85];
  v17 = v5;
  if ( v6 != 28672 && v6 != 28673 && v6 != 0x7FFF && v6 != -1 )
  {
    if ( !sub_407F20(dword_565B30, v6, v22) )
      return v18;
    v7 = dword_565B30;
    if ( this[86] != sub_408300(this[85], dword_565B30) )
      return v18;
    v8 = this[87];
    if ( v8 != -1 )
    {
      if ( sub_407F20(v7, v8, v21) && this[89] == sub_408300(this[87], dword_565B30) )
      {
        sub_40A530(v22, a2, -(a3[1] + this[84]), -(*a3 + this[83]), 128, 0);
        sub_4091B0(v21, a3);
        v9 = sub_41D540(this);
        sub_411990(v21, v20, a2, -(*a3 + this[79]), -(a3[1] + this[80]), this[88], v17, v9, 1);
        return 1;
      }
      return v18;
    }
    sub_40A530(v22, a2, -(a3[1] + this[84]), -(*a3 + this[83]), 128, 0);
    sub_40A530(v20, a2, -(a3[1] + this[80]), -(*a3 + this[79]), 1, v17);
    return 1;
  }
  v11 = v6 - 28672;
  Val = 0;
  v12 = 128;
  if ( v11 )
  {
    if ( v11 != 1 )
      goto LABEL_25;
    Val = 0xFFFFFF;
    v12 = 193;
  }
  else
  {
    Val = 0;
    v12 = 192;
  }
  sub_41C0A0();
  v13 = this[79];
  v4 = 1;
  if ( v13 >= 0 )
  {
    v14 = this[80];
    if ( v14 >= 0 && (unsigned int)(v20[2] - v13) >= v22[2] && (unsigned int)(v20[3] - v14) >= v22[3] && this[87] == -1 )
      v4 = 0;
  }
LABEL_25:
  v15 = this[87];
  if ( v15 == -1 )
  {
    if ( v4 )
    {
      sub_40A710(a2, 0, Val);
    }
    else if ( v17 )
    {
      v12 = 192;
    }
    sub_40A530(v20, a2, -(a3[1] + this[80]), -(*a3 + this[79]), v12, v17);
    return 1;
  }
  if ( !sub_407F20(dword_565B30, v15, v21) || this[89] != sub_408300(this[87], dword_565B30) )
    return v18;
  if ( v4 )
    sub_40A710(a2, 0, Val);
  sub_4091B0(v21, a3);
  v16 = sub_41D540(this);
  sub_411990(v21, v20, a2, -(*a3 + this[79]), -(a3[1] + this[80]), this[88], v17, v16, 1);
  return 1;
}

// ===== sub_41D910 @ 0x0041D910..0x0041D96B =====
_DWORD *__stdcall sub_41D910(_DWORD *a1)
{
  sub_41C220(a1, 7);
  *a1 = &CDspObjBackGRD::`vftable';
  a1[79] = -1;
  return a1;
}

// ===== sub_41D970 @ 0x0041D970..0x0041D992 =====
void *__thiscall sub_41D970(void *this, char a2)
{
  sub_41D9A0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41D9A0 @ 0x0041D9A0..0x0041D9E9 =====
int __stdcall sub_41D9A0(_DWORD *a1)
{
  *a1 = &CDspObjBackGRD::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41D9F0 @ 0x0041D9F0..0x0041DA63 =====
int __userpurge sub_41D9F0@<eax>(_DWORD *a1@<edi>, int a2)
{
  int v2; // edx
  _DWORD v4[6]; // [esp+8h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a2, v4) )
    return -2147483647;
  if ( !(*(int (__thiscall **)(_DWORD *, _DWORD *))(*a1 + 128))(a1, v4) )
    return -2147483646;
  v2 = dword_565B30;
  a1[79] = a2;
  a1[80] = sub_408300(a2, v2);
  return 0;
}

// ===== sub_41DA70 @ 0x0041DA70..0x0041DA84 =====
int __usercall sub_41DA70@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  if ( a1 > 1 )
    return -2147483645;
  *(_DWORD *)(a2 + 324) = a1;
  return 0;
}

// ===== sub_41DA90 @ 0x0041DA90..0x0041DB12 =====
int __thiscall sub_41DA90(_DWORD *this, int a2, int *a3)
{
  unsigned int v4; // eax
  _DWORD v6[6]; // [esp+10h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v6) || this[80] != sub_408300(this[79], dword_565B30) )
    return 0;
  sub_4091B0(v6, a3);
  v4 = sub_41B770(this);
  sub_413500((int)v6, v4, a2, this[81]);
  return 1;
}

// ===== sub_41DB20 @ 0x0041DB20..0x0041DB98 =====
_DWORD *__stdcall sub_41DB20(_DWORD *a1)
{
  int v1; // edx

  sub_41C220(a1, 12);
  *a1 = &CDspObjBackML::`vftable';
  sub_41BEA0((int)a1, 0);
  a1[79] = v1;
  memset(a1 + 80, v1, 0x2A0u);
  return a1;
}

// ===== sub_41DBA0 @ 0x0041DBA0..0x0041DBC2 =====
void *__thiscall sub_41DBA0(void *this, char a2)
{
  sub_41DBD0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41DBD0 @ 0x0041DBD0..0x0041DC19 =====
int __stdcall sub_41DBD0(_DWORD *a1)
{
  *a1 = &CDspObjBackML::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41DC20 @ 0x0041DC20..0x0041DC44 =====
int __userpurge sub_41DC20@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *(_DWORD *)(84 * a1 + a2 + 324) = a3;
  return 0;
}

// ===== sub_41DC50 @ 0x0041DC50..0x0041DC76 =====
int __userpurge sub_41DC50@<eax>(unsigned int a1@<eax>, int a2@<ecx>, _DWORD *a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *a3 = *(_DWORD *)(84 * a1 + a2 + 324);
  return 0;
}

// ===== sub_41DC80 @ 0x0041DC80..0x0041DCE1 =====
int __userpurge sub_41DC80@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, unsigned int a4)
{
  unsigned int v4; // ebx
  int v7; // esi

  v4 = a4;
  if ( a2 >= 8 )
    return -2147483647;
  if ( sub_41BED0(a3, 0) )
  {
    a1 = (a1 + 0x8000) & 0xFFFF0000;
    v4 = (a4 + 0x8000) & 0xFFFF0000;
  }
  v7 = 84 * a2;
  *(_DWORD *)(a3 + v7 + 328) = a1;
  *(_DWORD *)(a3 + v7 + 332) = v4;
  return 0;
}

// ===== sub_41DCF0 @ 0x0041DCF0..0x0041DD22 =====
int __userpurge sub_41DCF0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<ecx>, int a3)
{
  int v3; // eax

  if ( a1 >= 8 )
    return -2147483647;
  v3 = 84 * a1;
  *a2 = *(_DWORD *)(v3 + a3 + 328);
  a2[1] = *(_DWORD *)(v3 + a3 + 332);
  return 0;
}

// ===== sub_41DD30 @ 0x0041DD30..0x0041DD53 =====
int __userpurge sub_41DD30@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *(_DWORD *)(84 * (a1 + 4) + a2) = a3;
  return 0;
}

// ===== sub_41DD60 @ 0x0041DD60..0x0041DD84 =====
int __userpurge sub_41DD60@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *(_DWORD *)(84 * a1 + a2 + 340) = a3;
  return 0;
}

// ===== sub_41DD90 @ 0x0041DD90..0x0041DDB6 =====
int __userpurge sub_41DD90@<eax>(unsigned int a1@<eax>, int a2@<ecx>, _DWORD *a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *a3 = *(_DWORD *)(84 * a1 + a2 + 340);
  return 0;
}

// ===== sub_41DDC0 @ 0x0041DDC0..0x0041DE77 =====
int __userpurge sub_41DDC0@<eax>(unsigned int a1@<eax>, int a2, int a3, int a4, int a5)
{
  int v6; // edx
  _DWORD *v7; // esi
  int result; // eax
  int v9; // edi
  _DWORD v10[6]; // [esp+8h] [ebp-18h] BYREF

  if ( a1 >= 8 )
    return -2147483647;
  if ( a3 == -1 )
  {
    v9 = 84 * a1;
    result = 0;
    *(_DWORD *)(v9 + a2 + 320) = 0;
  }
  else if ( sub_407F20(dword_565B30, a3, v10) )
  {
    v6 = dword_565B30;
    v7 = (_DWORD *)(84 * a1 + a2);
    v7[80] = 1;
    v7[86] = a3;
    v7[87] = sub_408300(a3, v6);
    result = 0;
    v7[88] = a4;
    v7[89] = a5;
    v7[95] = 0;
    v7[96] = 0;
  }
  else
  {
    return -2147483646;
  }
  return result;
}

// ===== sub_41DE80 @ 0x0041DE80..0x0041DEE2 =====
int __userpurge sub_41DE80@<eax>(unsigned int a1@<eax>, int a2@<edx>, int a3@<esi>, int a4, int a5, int a6)
{
  _DWORD *v6; // eax

  if ( a1 >= 8 )
    return -2147483647;
  if ( !a3 || !a2 )
    return -2147483645;
  v6 = (_DWORD *)(a4 + 84 * a1);
  v6[92] = a2;
  v6[90] = a5;
  v6[91] = a3;
  v6[97] = 0;
  v6[98] = 0;
  v6[99] = 0;
  v6[100] = a6;
  return 0;
}

// ===== sub_41DEF0 @ 0x0041DEF0..0x0041DF1E =====
int __userpurge sub_41DEF0@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int v4; // eax

  if ( a1 >= 8 )
    return -2147483647;
  v4 = a2 + 84 * a1;
  *(_DWORD *)(v4 + 372) = a3;
  *(_DWORD *)(v4 + 376) = a4;
  return 0;
}

// ===== sub_41DF20 @ 0x0041DF20..0x0041DF4E =====
int __userpurge sub_41DF20@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int v4; // eax

  if ( a1 >= 8 )
    return -2147483647;
  v4 = a2 + 84 * a1;
  *(_DWORD *)(v4 + 380) = a3;
  *(_DWORD *)(v4 + 384) = a4;
  return 0;
}

// ===== sub_41DF50 @ 0x0041DF50..0x0041DF74 =====
int __userpurge sub_41DF50@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3)
{
  if ( a1 >= 8 )
    return -2147483647;
  *(_DWORD *)(84 * a1 + a2 + 388) = a3;
  return 0;
}

// ===== sub_41DF80 @ 0x0041DF80..0x0041DFAE =====
int __userpurge sub_41DF80@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int v4; // eax

  if ( a1 >= 8 )
    return -2147483647;
  v4 = a2 + 84 * a1;
  *(_DWORD *)(v4 + 392) = a3;
  *(_DWORD *)(v4 + 396) = a4;
  return 0;
}

// ===== sub_41DFB0 @ 0x0041DFB0..0x0041DFC4 =====
int __usercall sub_41DFB0@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  if ( a1 >= 8 )
    return -2147483647;
  *(_DWORD *)(a2 + 316) = a1;
  return 0;
}

// ===== sub_41DFD0 @ 0x0041DFD0..0x0041DFD9 =====
int __thiscall sub_41DFD0(_DWORD *this, int a2, int a3)
{
  return sub_41B470(this, a2, a3);
}

// ===== sub_41DFE0 @ 0x0041DFE0..0x0041DFF6 =====
int __thiscall sub_41DFE0(unsigned int *this, _DWORD *a2)
{
  return sub_41DCF0(this[79], a2, (int)this);
}

// ===== sub_41E000 @ 0x0041E000..0x0041E02F =====
int __thiscall sub_41E000(_DWORD *this, unsigned int a2, unsigned int a3, int a4)
{
  sub_41B370(this, a2, a3, a4);
  return sub_41DC80(a2, this[79], (int)this, a3);
}

// ===== sub_41E030 @ 0x0041E030..0x0041E03F =====
int __thiscall sub_41E030(unsigned int *this, int a2)
{
  return sub_41DD60(this[79], (int)this, a2);
}

// ===== sub_41E040 @ 0x0041E040..0x0041E05A =====
unsigned int *__thiscall sub_41E040(unsigned int *this)
{
  unsigned int *v2; // [esp+0h] [ebp-4h] BYREF

  v2 = this;
  sub_41DD90(this[79], (int)this, &v2);
  return v2;
}

// ===== sub_41E060 @ 0x0041E060..0x0041E125 =====
int __thiscall sub_41E060(unsigned int *this, unsigned int a2, int a3, int a4)
{
  int result; // eax

  switch ( a2 )
  {
    case 0x41u:
      sub_41DE80(this[79], this[21 * this[79] + 92], this[21 * this[79] + 91], (int)this, a3, this[21 * this[79] + 100]);
      result = 0;
      break;
    case 0x80u:
      sub_41DF20(this[79], (int)this, a3, a4);
      result = 0;
      break;
    case 0x81u:
      sub_41DF50(this[79], (int)this, a3);
      result = 0;
      break;
    case 0x82u:
      sub_41DF80(this[79], (int)this, a3, a4);
      result = 0;
      break;
    case 0x8Fu:
      sub_41DEF0(this[79], (int)this, a3, a4);
      result = 0;
      break;
    default:
      result = sub_41B8E0(this, a2, a3, a4);
      break;
  }
  return result;
}

// ===== sub_41E190 @ 0x0041E190..0x0041E558 =====
int __thiscall sub_41E190(void *this, _DWORD *a2, _DWORD *a3)
{
  int v3; // ebx
  unsigned int v4; // eax
  int v5; // edi
  int v6; // eax
  int v7; // esi
  int v8; // edi
  int v9; // ebx
  int v10; // eax
  double v11; // st5
  double v12; // st7
  int v13; // esi
  int v14; // ecx
  int v15; // esi
  int v17; // [esp+Ch] [ebp-BCh]
  int v18; // [esp+Ch] [ebp-BCh]
  int v19; // [esp+10h] [ebp-B8h]
  int v20; // [esp+14h] [ebp-B4h]
  unsigned int v21; // [esp+18h] [ebp-B0h]
  int v22; // [esp+1Ch] [ebp-ACh]
  int v23; // [esp+20h] [ebp-A8h]
  _DWORD v25[21]; // [esp+28h] [ebp-A0h] BYREF
  int v26[2]; // [esp+80h] [ebp-48h] BYREF
  unsigned int v27; // [esp+88h] [ebp-40h]
  unsigned int v28; // [esp+8Ch] [ebp-3Ch]
  int v29; // [esp+90h] [ebp-38h]
  unsigned int v30; // [esp+A0h] [ebp-28h]
  unsigned int v31; // [esp+A4h] [ebp-24h]
  void *v32[6]; // [esp+B0h] [ebp-18h] BYREF

  v3 = (int)this;
  v17 = 0;
  sub_41C0A0();
  v4 = 0;
  v21 = 0;
  do
  {
    qmemcpy(v25, (const void *)(84 * v4 + v3 + 320), sizeof(v25));
    if ( v25[0] )
    {
      if ( v25[1] )
      {
        v5 = v25[6];
        if ( sub_407F20(dword_565B30, v25[6], v26) )
        {
          v6 = sub_408300(v5, dword_565B30);
          if ( v25[7] == v6 )
          {
            v7 = sub_41B750(1, v3);
            v22 = v25[2] - (*a3 << 16);
            v23 = v25[3] - (a3[1] << 16);
            v8 = v25[8] + ((v25[15] * (unsigned __int64)(unsigned int)v7) >> 24);
            v9 = v25[9] + ((v25[16] * (unsigned __int64)(unsigned int)v7) >> 24);
            v10 = sub_41A690(v7);
            v18 = v25[10] + ((unsigned __int64)(v25[17] * (__int64)v10) >> 16);
            v11 = (double)sub_41A690(v7);
            v19 = (int)(65536.0
                      / (65536.0 / (double)v25[11]
                       + (65536.0 / (double)(v25[11] + v25[18]) - 65536.0 / (double)v25[11]) * v11 * 0.0000152587890625));
            v12 = 65536.0
                / (0.0000152587890625 * (v11 * (65536.0 / (double)(v25[12] + v25[19]) - 65536.0 / (double)v25[12]))
                 + 65536.0 / (double)v25[12]);
            if ( v21 )
            {
              if ( v25[4] < 2u || v25[4] == 32 )
              {
                sub_4168E0(
                  (int)a2,
                  v22,
                  v23,
                  v26,
                  v8,
                  v9,
                  v18,
                  (int)(65536.0
                      / (65536.0 / (double)v25[11]
                       + (65536.0 / (double)(v25[11] + v25[18]) - 65536.0 / (double)v25[11]) * v11 * 0.0000152587890625)),
                  (int)v12,
                  v25[5],
                  v25[20],
                  1);
              }
              else
              {
                sub_409030(v29, a3[3] - a3[1] + 1, v32, a3[2] - *a3 + 1);
                sub_417730((int)v32, v22, v23, v26, v8, v9, v18, v19, (int)v12, 0, v25[20], 1);
                sub_40A9E0((int)a2, (int)v32, v25[4], v25[5], 1);
                operator delete[](v32[0]);
              }
            }
            else if ( v27 >= v30
                   && v28 >= v31
                   && (v13 = v25[2] - v8, LOWORD(v25[2]) == (_WORD)v8)
                   && (v14 = v25[3] - v9, LOWORD(v25[3]) == (_WORD)v9)
                   && v13 <= 0
                   && v14 <= 0
                   && (v15 = v13 >> 16, v15 + v27 >= v30)
                   && (v20 = v14 >> 16, v28 + (v14 >> 16) >= v31)
                   && !v18
                   && v19 == 0x10000
                   && (int)v12 == 0x10000 )
            {
              sub_40A620((int)a2, 0);
              sub_40A530(v26, a2, v20 - a3[1], v15 - *a3, 5, v25[5]);
            }
            else
            {
              sub_417730((int)a2, v22, v23, v26, v8, v9, v18, v19, (int)v12, v25[5], v25[20], 1);
            }
            v3 = (int)this;
            v17 = 1;
          }
        }
      }
    }
    if ( !v21 && !v17 )
    {
      sub_40A620((int)a2, 0);
      v17 = 1;
    }
    v4 = v21 + 1;
    v21 = v4;
  }
  while ( v4 < 8 );
  return v17;
}

// ===== sub_41E560 @ 0x0041E560..0x0041E5D2 =====
_DWORD *__stdcall sub_41E560(_DWORD *a1)
{
  sub_41C220(a1, 11);
  *a1 = &CDspObjBackMSC::`vftable';
  a1[79] = -1;
  a1[81] = -1;
  sub_41E720(0);
  sub_41E740(a1);
  return a1;
}

// ===== sub_41E5E0 @ 0x0041E5E0..0x0041E602 =====
void *__thiscall sub_41E5E0(void *this, char a2)
{
  sub_41E610(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41E610 @ 0x0041E610..0x0041E659 =====
int __stdcall sub_41E610(_DWORD *a1)
{
  *a1 = &CDspObjBackMSC::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41E660 @ 0x0041E660..0x0041E716 =====
int __userpurge sub_41E660@<eax>(_DWORD *a1@<eax>, int a2@<esi>, int a3)
{
  int v4; // eax
  int v5; // edx
  int result; // eax

  if ( !sub_41C420(a2, (int)a1) )
    return -2147483647;
  if ( a3 == 28672 || a3 == 28673 )
  {
    a1[80] = sub_408300(a2, dword_565B30);
    result = 0;
    a1[79] = a2;
    a1[81] = a3;
  }
  else if ( sub_41C420(a3, (int)a1) )
  {
    v4 = sub_408300(a2, dword_565B30);
    v5 = dword_565B30;
    a1[80] = v4;
    a1[82] = sub_408300(a3, v5);
    result = 0;
    a1[79] = a2;
    a1[81] = a3;
  }
  else
  {
    return -2147483646;
  }
  return result;
}

// ===== sub_41E720 @ 0x0041E720..0x0041E739 =====
int __thiscall sub_41E720(_DWORD *this, int a2)
{
  int result; // eax

  result = 0;
  if ( !a2 )
  {
    this[83] = 0;
    return 1;
  }
  return result;
}

// ===== sub_41E740 @ 0x0041E740..0x0041E75C =====
int __fastcall sub_41E740(unsigned int a1, int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    *(_DWORD *)(a3 + 336) = a1;
    return 1;
  }
  return result;
}

// ===== sub_41E760 @ 0x0041E760..0x0041E88D =====
int __thiscall sub_41E760(_DWORD *this, _DWORD *a2, int a3)
{
  unsigned int v4; // eax
  int v5; // ecx
  int v6; // edi
  int v7; // ecx
  unsigned int v9; // [esp+Ch] [ebp-34h]
  _DWORD v10[6]; // [esp+10h] [ebp-30h] BYREF
  _DWORD v11[6]; // [esp+28h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v10) || this[80] != sub_408300(this[79], dword_565B30) )
    return 0;
  v4 = sub_41B770(this);
  v5 = this[81];
  v6 = v4;
  v9 = v4;
  if ( v5 == 28672 || v5 == 28673 )
  {
    v7 = v5 - 28672;
    if ( v7 )
    {
      if ( v7 == 1 )
        v4 = 0xFFFFFF;
    }
    else
    {
      v4 = 0;
    }
    if ( this[84] == 1 )
      sub_40A710(a2, 0, v4);
  }
  else if ( this[84] == 1 && sub_407F20(dword_565B30, v5, v11) && this[82] == sub_408300(this[81], dword_565B30) )
  {
    sub_4149B0(0, v11, 256 - v9, (int)a2);
    v6 = v9;
  }
  sub_4149B0(this[84] == 1 ? v6 : 0, v10, v6, (int)a2);
  return 1;
}

// ===== sub_41E890 @ 0x0041E890..0x0041E8EB =====
_DWORD *__stdcall sub_41E890(_DWORD *a1)
{
  sub_41C220(a1, 1);
  *a1 = &CDspObjBackN::`vftable';
  a1[79] = -1;
  return a1;
}

// ===== sub_41E8F0 @ 0x0041E8F0..0x0041E912 =====
void *__thiscall sub_41E8F0(void *this, char a2)
{
  sub_41E920(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41E920 @ 0x0041E920..0x0041E969 =====
int __stdcall sub_41E920(_DWORD *a1)
{
  *a1 = &CDspObjBackN::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41E970 @ 0x0041E970..0x0041E9A5 =====
int __usercall sub_41E970@<eax>(int a1@<eax>, int a2@<esi>)
{
  int v3; // edx

  if ( !sub_41C420(a2, a1) )
    return 0;
  v3 = dword_565B30;
  *(_DWORD *)(a1 + 316) = a2;
  *(_DWORD *)(a1 + 320) = sub_408300(a2, v3);
  return 1;
}

// ===== sub_41E9B0 @ 0x0041E9B0..0x0041EA2C =====
int __thiscall sub_41E9B0(int *this, int a2, int *a3)
{
  _DWORD v5[6]; // [esp+10h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v5) || this[80] != sub_408300(this[79], dword_565B30) )
    return 0;
  sub_4091B0(v5, a3);
  sub_40A9E0(a2, (int)v5, 0x80u, 0, 1);
  return 1;
}

// ===== sub_41EA30 @ 0x0041EA30..0x0041EAAE =====
int __stdcall sub_41EA30(_DWORD *a1)
{
  sub_41C220(a1, 8);
  *a1 = &CDspObjBackRPL::`vftable';
  a1[79] = -1;
  a1[80] = -1;
  a1[86] = 0;
  sub_41EB40(0, 0);
  return sub_41EE00(0);
}

// ===== sub_41EAB0 @ 0x0041EAB0..0x0041EAD2 =====
void *__thiscall sub_41EAB0(void *this, char a2)
{
  sub_41EAE0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41EAE0 @ 0x0041EAE0..0x0041EB37 =====
int __stdcall sub_41EAE0(_DWORD *a1)
{
  *a1 = &CDspObjBackRPL::`vftable';
  sub_41EE90();
  return sub_41C2D0(a1);
}

// ===== sub_41EB40 @ 0x0041EB40..0x0041EB59 =====
int __thiscall sub_41EB40(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[83] = a2;
  this[84] = a3;
  return result;
}

// ===== sub_41EB60 @ 0x0041EB60..0x0041EB7B =====
_DWORD *__thiscall sub_41EB60(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = this[83];
  a2[1] = this[84];
  return result;
}

// ===== nullsub_3 @ 0x0041EB80..0x0041EB83 =====
void __stdcall nullsub_3(int a1, int a2)
{
  ;
}

// ===== sub_41EB90 @ 0x0041EB90..0x0041EBFF =====
int __userpurge sub_41EB90@<eax>(_DWORD *a1@<edi>, int a2)
{
  int v2; // edx
  _DWORD v4[7]; // [esp+8h] [ebp-1Ch] BYREF

  if ( !sub_407F20(dword_565B30, a2, v4) )
    return -2147483647;
  if ( !(*(int (__thiscall **)(_DWORD *, _DWORD *))(*a1 + 128))(a1, v4) )
    return -2147483646;
  v2 = dword_565B30;
  a1[79] = a2;
  a1[81] = sub_408300(a2, v2);
  return 0;
}

// ===== sub_41EC00 @ 0x0041EC00..0x0041ED22 =====
int __userpurge sub_41EC00@<eax>(int *a1@<edi>, int a2, int a3, unsigned int a4, int a5)
{
  int v5; // ebx
  void *v6; // esi
  int v7; // edx
  int v8; // eax
  int v9; // ecx
  int v10; // eax
  void (__thiscall *v11)(int *, int); // edx
  void *v13; // [esp-4h] [ebp-24h]
  _DWORD v14[6]; // [esp+8h] [ebp-18h] BYREF

  v5 = a3;
  if ( !sub_407F20(dword_565B30, a2, v14) )
    return -2147483645;
  if ( !sub_41EEB0() )
    return -2147483644;
  if ( !v5 )
    return -2147483643;
  v6 = operator new[](16 * v5);
  if ( sub_409FF0(a4, dword_565B30, &a3, 0, v5) )
  {
    operator delete[](v6);
    return -2147483642;
  }
  else if ( a3 )
  {
    v7 = dword_565B30;
    a1[80] = a2;
    v8 = sub_408300(a2, v7);
    v13 = (void *)a1[86];
    a1[82] = v8;
    a1[85] = v5;
    operator delete[](v13);
    v9 = a5;
    v10 = *a1;
    a1[87] = a4;
    v11 = *(void (__thiscall **)(int *, int))(v10 + 72);
    a1[86] = (int)v6;
    a1[88] = 0;
    v11(a1, v9);
    return 0;
  }
  else
  {
    return -2147483641;
  }
}

// ===== sub_41ED30 @ 0x0041ED30..0x0041ED67 =====
int __thiscall sub_41ED30(_DWORD *this, int a2)
{
  sub_41B620(this, a2);
  return sub_409F40(this[85], this[86], this[87], this[88], a2);
}

// ===== sub_41ED70 @ 0x0041ED70..0x0041EDC6 =====
int __thiscall sub_41ED70(void *this, unsigned int a2, int a3, int a4)
{
  if ( a2 == 255 )
  {
    sub_41EE00(a3);
    return 0;
  }
  else if ( a2 == 256 )
  {
    return sub_41EE10(a4) != 0 ? 0xFFFF0002 : 0;
  }
  else
  {
    return sub_41B8E0(this, a2, a3, a4);
  }
}

// ===== sub_41EDD0 @ 0x0041EDD0..0x0041EDF2 =====
BOOL __stdcall sub_41EDD0(int a1)
{
  int v2; // [esp+10h] [ebp-8h]

  sub_41C0A0();
  return *(_DWORD *)(a1 + 16) == v2;
}

// ===== sub_41EE00 @ 0x0041EE00..0x0041EE07 =====
int __usercall sub_41EE00@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 356) = a2;
  return result;
}

// ===== sub_41EE10 @ 0x0041EE10..0x0041EE8C =====
int __userpurge sub_41EE10@<eax>(int a1@<edi>, _DWORD *a2@<esi>, unsigned int a3)
{
  unsigned int v3; // ebx
  int v5; // eax

  v3 = a3;
  if ( sub_409FF0(a2[87], dword_565B30, &a3, a1, a2[85]) )
    return -2147483642;
  if ( !a3 )
    return -2147483641;
  a2[88] = a1;
  if ( v3 > 0x100 )
  {
    v5 = (*(int (__thiscall **)(_DWORD *))(*a2 + 76))(a2);
    (*(void (__thiscall **)(_DWORD *, int))(*a2 + 72))(a2, v5);
  }
  else
  {
    (*(void (__thiscall **)(_DWORD *, unsigned int))(*a2 + 72))(a2, v3);
  }
  return 0;
}

// ===== sub_41EE90 @ 0x0041EE90..0x0041EEAA =====
void __usercall sub_41EE90(int a1@<esi>)
{
  operator delete[](*(void **)(a1 + 344));
  *(_DWORD *)(a1 + 344) = 0;
}

// ===== sub_41EEB0 @ 0x0041EEB0..0x0041EEE5 =====
BOOL __usercall sub_41EEB0@<eax>(_DWORD *a1@<esi>)
{
  BOOL result; // eax
  unsigned int v2; // [esp+8h] [ebp-10h]
  unsigned int v3; // [esp+Ch] [ebp-Ch]

  result = 0;
  if ( a1[4] == 6 )
  {
    sub_41C0A0();
    return a1[2] >= v2 && a1[3] >= v3;
  }
  return result;
}

// ===== sub_41EEF0 @ 0x0041EEF0..0x0041F037 =====
int __thiscall sub_41EEF0(int *this, _DWORD *a2, int *a3)
{
  int v4; // ebx
  _DWORD *v5; // eax
  int v7[4]; // [esp+10h] [ebp-58h] BYREF
  _DWORD v8[6]; // [esp+20h] [ebp-48h] BYREF
  _DWORD v9[6]; // [esp+38h] [ebp-30h] BYREF
  _DWORD v10[6]; // [esp+50h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v8) )
    return 0;
  v4 = dword_565B30;
  if ( this[81] != sub_408300(this[79], dword_565B30)
    || !sub_407F20(v4, this[80], v10)
    || this[82] != sub_408300(this[80], dword_565B30) )
  {
    return 0;
  }
  v9[1] = v8[1];
  v9[0] = v8[0];
  v9[2] = v8[2];
  v9[4] = v8[4];
  v9[3] = v8[3];
  v9[5] = v8[5];
  v5 = sub_409190(v7, (int)v8);
  sub_409170(this[84], this[83], v5);
  sub_4091B0(v9, v7);
  sub_4091B0(v9, a3);
  sub_4091B0(v10, a3);
  sub_413A10(v10, (int)v9, a2, (int)v8, this[86], this[89]);
  return 1;
}

// ===== sub_41F040 @ 0x0041F040..0x0041F09B =====
_DWORD *__stdcall sub_41F040(_DWORD *a1)
{
  sub_41C220(a1, 10);
  *a1 = &CDspObjBackRTT::`vftable';
  a1[79] = -1;
  return a1;
}

// ===== sub_41F0A0 @ 0x0041F0A0..0x0041F0C2 =====
void *__thiscall sub_41F0A0(void *this, char a2)
{
  sub_41F0D0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41F0D0 @ 0x0041F0D0..0x0041F119 =====
int __stdcall sub_41F0D0(_DWORD *a1)
{
  *a1 = &CDspObjBackRTT::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41F120 @ 0x0041F120..0x0041F153 =====
int __userpurge sub_41F120@<eax>(_DWORD *a1@<eax>, int a2@<ecx>, int a3)
{
  if ( !a2 )
    return -2147483646;
  a1[81] = a2;
  a1[82] = a3;
  a1[83] = 0;
  a1[84] = 0;
  return 0;
}

// ===== sub_41F160 @ 0x0041F160..0x0041F1BE =====
int __thiscall sub_41F160(_DWORD *this, unsigned int a2, int a3, int a4)
{
  if ( a2 == 128 )
    return sub_41F1C0(a4) != 0 ? 0xFFFF0002 : 0;
  if ( a2 == 257 )
    return sub_41F120(this, a3, a4) != 0 ? 0xFFFF0002 : 0;
  return sub_41B8E0(this, a2, a3, a4);
}

// ===== sub_41F1C0 @ 0x0041F1C0..0x0041F1EB =====
int __userpurge sub_41F1C0@<eax>(_DWORD *a1@<eax>, int a2@<ecx>, int a3)
{
  if ( !(a2 + a1[81]) )
    return -2147483646;
  a1[83] = a2;
  a1[84] = a3;
  return 0;
}

// ===== sub_41F1F0 @ 0x0041F1F0..0x0041F371 =====
int __thiscall sub_41F1F0(int *this, int a2, int a3)
{
  unsigned int v4; // eax
  int v5; // ebx
  unsigned int v6; // esi
  long double v7; // st7
  int v8; // eax
  double v9; // st3
  int v11; // [esp+10h] [ebp-28h]
  _DWORD v12[6]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v12) || this[80] != sub_408300(this[79], dword_565B30) )
    return 0;
  v4 = (*(int (__thiscall **)(int *))(*this + 76))(this);
  v5 = this[83];
  v6 = v4;
  v11 = this[81];
  if ( v5 && v4 )
  {
    v7 = sin((double)v4 * 3.141592653589793 * 0.001953125) * 256.0;
    v8 = abs32(v5);
    if ( v5 < 0 )
      v9 = 1.0;
    else
      v9 = -1.0;
    v11 = (__int64)((0.00390625 * (v9 * (256.0 - v7))
                   + (double)v5 / ((double)v8 - (double)(v8 - 0x10000) * v7 * 0.00390625))
                  * 65536.0
                  + (double)(unsigned int)v11);
  }
  sub_414260(a2, v11, this[82] + ((this[84] * (unsigned __int64)v6) >> 8));
  return 1;
}

// ===== sub_41F380 @ 0x0041F380..0x0041F3F4 =====
_DWORD *__stdcall sub_41F380(_DWORD *a1)
{
  sub_41C220(a1, 3);
  *a1 = &CDspObjBackS::`vftable';
  sub_41F590(a1);
  a1[79] = -1;
  a1[80] = -1;
  a1[81] = -1;
  a1[82] = -1;
  return a1;
}

// ===== sub_41F400 @ 0x0041F400..0x0041F422 =====
void *__thiscall sub_41F400(void *this, char a2)
{
  sub_41F430(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41F430 @ 0x0041F430..0x0041F479 =====
int __stdcall sub_41F430(_DWORD *a1)
{
  *a1 = &CDspObjBackS::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41F480 @ 0x0041F480..0x0041F496 =====
int __thiscall sub_41F480(void *this, int a2, int a3)
{
  return sub_41F540(this, a3);
}

// ===== sub_41F4A0 @ 0x0041F4A0..0x0041F539 =====
int __userpurge sub_41F4A0@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3, int a4, int a5)
{
  int v7; // ebx
  int *v8; // esi
  int v9; // edi

  if ( !sub_41C420(a1, (int)a2) || !sub_41C420(a3, (int)a2) || !sub_41C420(a4, (int)a2) || !sub_41C420(a5, (int)a2) )
    return 0;
  a2[79] = a1;
  a2[82] = a5;
  v7 = dword_565B30;
  a2[80] = a3;
  a2[81] = a4;
  v8 = a2 + 83;
  v9 = 4;
  do
  {
    *v8 = sub_408300(*(v8 - 4), v7);
    ++v8;
    --v9;
  }
  while ( v9 );
  return 1;
}

// ===== sub_41F540 @ 0x0041F540..0x0041F58B =====
int __userpurge sub_41F540@<eax>(int a1@<esi>, int a2, int a3)
{
  int v4; // [esp+10h] [ebp-10h]
  int v5; // [esp+14h] [ebp-Ch]

  sub_41C0A0();
  if ( a1 < 0 || a1 > v4 || a3 < 0 || a3 > v5 )
    return 0;
  *(_DWORD *)(a2 + 352) = a3;
  *(_DWORD *)(a2 + 348) = a1;
  return 1;
}

// ===== sub_41F590 @ 0x0041F590..0x0041F637 =====
int __thiscall sub_41F590(_DWORD *this)
{
  int v2; // ebx
  int v3; // edx
  int result; // eax
  int v5; // [esp+Ch] [ebp-1Ch]
  int v6; // [esp+18h] [ebp-10h]
  int v7; // [esp+1Ch] [ebp-Ch]

  v5 = sub_41C390(this);
  sub_41C0A0();
  this[89] = 0;
  this[90] = 0;
  this[94] = 0;
  v2 = 2 * v6 - 1;
  this[91] = v6 - 1;
  this[99] = v6 - 1;
  this[92] = v7 - 1;
  this[96] = v7 - 1;
  v3 = 2 * v7 - 1;
  this[93] = v6;
  this[95] = v2;
  this[101] = v6;
  result = v5;
  this[103] = v2;
  this[97] = 0;
  this[98] = v7;
  this[100] = v3;
  this[102] = v7;
  this[104] = v3;
  return result;
}

// ===== sub_41F640 @ 0x0041F640..0x0041F830 =====
int __thiscall sub_41F640(_DWORD *this, _DWORD *a2, int *a3)
{
  int result; // eax
  _DWORD *v4; // esi
  _DWORD *v5; // esi
  int *v6; // eax
  int v7; // ecx
  int v8; // edx
  int v9; // eax
  int v10; // ecx
  int v11; // edx
  int v12; // edx
  int v13; // eax
  int v14; // edx
  int v15; // eax
  int *v16; // [esp+10h] [ebp-A8h]
  int *v17; // [esp+10h] [ebp-A8h]
  int v18; // [esp+14h] [ebp-A4h]
  _DWORD *v19; // [esp+14h] [ebp-A4h]
  BOOL v20; // [esp+18h] [ebp-A0h]
  int v22; // [esp+20h] [ebp-98h]
  int v23[4]; // [esp+24h] [ebp-94h] BYREF
  int v24; // [esp+34h] [ebp-84h]
  _DWORD v25[6]; // [esp+38h] [ebp-80h] BYREF
  _BYTE v26[100]; // [esp+50h] [ebp-68h] BYREF

  result = 1;
  v18 = 0;
  v4 = v26;
  v16 = this + 79;
  while ( result )
  {
    if ( sub_407F20(dword_565B30, *v16, v4) )
      v20 = v16[4] == sub_408300(*v16, dword_565B30);
    else
      v20 = 0;
    ++v16;
    v4 += 6;
    ++v18;
    result = v20;
    if ( v18 >= 4 )
    {
      if ( v20 )
      {
        v5 = this;
        v6 = this + 89;
        v19 = v26;
        v17 = this + 89;
        v22 = 4;
        while ( 1 )
        {
          v7 = *a3;
          v23[1] = a3[1];
          v23[3] = a3[3];
          v8 = v6[1];
          v9 = *v6;
          v23[0] = v7;
          v10 = a3[2];
          v24 = v8;
          v11 = v5[87];
          v23[2] = v10;
          sub_409170(v5[88] - v24, v11 - v9, v23);
          if ( sub_409110(v23, v5 + 89) )
          {
            v12 = a2[1];
            v13 = a2[2];
            v25[0] = *a2;
            v25[3] = a2[3];
            v25[1] = v12;
            v14 = a2[4];
            v25[2] = v13;
            v15 = a2[5];
            v25[4] = v14;
            v25[5] = v15;
            sub_4091B0(v19, v23);
            sub_409170(v17[1] - a3[1] - this[88], *v17 - *a3 - this[87], v23);
            sub_4091B0(v25, v23);
            sub_40A9E0((int)v25, (int)v19, 0x80u, 0, 1);
            v5 = this;
          }
          v17 += 4;
          v19 += 6;
          if ( !--v22 )
            break;
          v6 = v17;
        }
        return v20;
      }
      return result;
    }
  }
  return result;
}

// ===== sub_41F830 @ 0x0041F830..0x0041F88B =====
_DWORD *__stdcall sub_41F830(_DWORD *a1)
{
  sub_41C220(a1, 9);
  *a1 = &CDspObjBackSTR::`vftable';
  a1[79] = -1;
  return a1;
}

// ===== sub_41F890 @ 0x0041F890..0x0041F8B2 =====
void *__thiscall sub_41F890(void *this, char a2)
{
  sub_41F8C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41F8C0 @ 0x0041F8C0..0x0041F909 =====
int __stdcall sub_41F8C0(_DWORD *a1)
{
  *a1 = &CDspObjBackSTR::`vftable';
  return sub_41C2D0(a1);
}

// ===== sub_41F910 @ 0x0041F910..0x0041F95F =====
int __userpurge sub_41F910@<eax>(int a1@<edi>, int a2)
{
  int v2; // edx
  _DWORD v4[6]; // [esp+8h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, a1, v4) )
    return -2147483647;
  v2 = dword_565B30;
  *(_DWORD *)(a2 + 316) = a1;
  *(_DWORD *)(a2 + 320) = sub_408300(a1, v2);
  return 0;
}

// ===== sub_41F960 @ 0x0041F960..0x0041F979 =====
int __thiscall sub_41F960(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[82] = a2;
  this[83] = a3;
  return result;
}

// ===== sub_41F980 @ 0x0041F980..0x0041F99B =====
_DWORD *__thiscall sub_41F980(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = this[82];
  a2[1] = this[83];
  return result;
}

// ===== sub_41F9A0 @ 0x0041F9A0..0x0041F9CB =====
int __userpurge sub_41F9A0@<eax>(unsigned int a1@<eax>, unsigned int a2@<edx>, int a3)
{
  if ( a1 < 2 || a2 < 2 )
    return -2147483646;
  *(_DWORD *)(a3 + 336) = a1;
  *(_DWORD *)(a3 + 340) = a2;
  return 0;
}

// ===== sub_41F9D0 @ 0x0041F9D0..0x0041FA26 =====
int __thiscall sub_41F9D0(void *this, unsigned int a2, int a3, int a4)
{
  if ( a2 == 258 )
    return sub_41FA30(this, (__int16)a3, SHIWORD(a3)) != 0 ? 0xFFFF0002 : 0;
  else
    return sub_41B8E0(this, a2, a3, a4);
}

// ===== sub_41FA30 @ 0x0041FA30..0x0041FA6F =====
int __fastcall sub_41FA30(unsigned int a1, unsigned int a2, _DWORD *a3, int a4, int a5)
{
  if ( a1 < 2 || a2 < 2 )
    return -2147483646;
  a3[88] = a4;
  a3[89] = a5;
  a3[90] = a1;
  a3[91] = a2;
  return 0;
}

// ===== sub_41FA70 @ 0x0041FA70..0x0041FC92 =====
int __thiscall sub_41FA70(int *this, int a2, int a3)
{
  double v4; // st7
  _DWORD v6[6]; // [esp+28h] [ebp-18h] BYREF

  if ( !sub_407F20(dword_565B30, this[79], v6) || this[80] != sub_408300(this[79], dword_565B30) )
    return 0;
  v4 = (double)(unsigned int)(*(int (__thiscall **)(int *))(*this + 76))(this);
  sub_414560(
    (__int64)(((double)(this[91] - this[85]) * v4 * 0.00390625 + (double)(unsigned int)this[85]) * 65536.0),
    (__int64)(((double)(this[90] - this[84]) * v4 * 0.00390625 + (double)(unsigned int)this[84]) * 65536.0),
    a2,
    (int)v6,
    (int)(v4 * (double)((this[88] << 16) - this[82]) * 0.00390625 + (double)this[82]),
    (int)((double)((this[89] << 16) - this[83]) * v4 * 0.00390625 + (double)this[83]));
  return 1;
}

// ===== sub_41FCA0 @ 0x0041FCA0..0x0041FD57 =====
_DWORD *__thiscall sub_41FCA0(void *this, _DWORD *a2)
{
  _DWORD *v2; // eax

  sub_41A400((int)this, a2, 6, 1);
  *a2 = &CDspObjEffector::`vftable';
  a2[83] = 0;
  a2[84] = 0;
  a2[85] = 0;
  a2[86] = 0;
  a2[87] = 0;
  a2[88] = 0;
  a2[77] = -1;
  a2[93] = 0;
  v2 = operator new(8u);
  *v2 = a2;
  v2[1] = dword_565B40;
  dword_565B40 = v2;
  sub_420730();
  sub_420250(0, 0);
  return a2;
}

// ===== sub_41FD60 @ 0x0041FD60..0x0041FD82 =====
void *__thiscall sub_41FD60(void *this, char a2)
{
  sub_41FD90(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41FD90 @ 0x0041FD90..0x0041FE1E =====
int __stdcall sub_41FD90(_DWORD *a1)
{
  _DWORD *v1; // eax
  _DWORD *v2; // ecx

  *a1 = &CDspObjEffector::`vftable';
  sub_420870();
  sub_4208C0();
  v1 = dword_565B40;
  v2 = &unk_565B3C;
  if ( dword_565B40 )
  {
    while ( (_DWORD *)*v1 != a1 )
    {
      v2 = v1;
      v1 = (_DWORD *)v1[1];
      if ( !v1 )
        return sub_41A5E0(a1);
    }
    v2[1] = v1[1];
    operator delete(v1);
  }
  return sub_41A5E0(a1);
}

// ===== sub_41FE20 @ 0x0041FE20..0x0041FE4D =====
BOOL sub_41FE20()
{
  _DWORD *v0; // esi

  v0 = dword_565B40;
  if ( dword_565B40 )
  {
    do
    {
      if ( (*(int (__thiscall **)(_DWORD))(*(_DWORD *)*v0 + 8))(*v0) )
        break;
      v0 = (_DWORD *)v0[1];
    }
    while ( v0 );
  }
  return v0 != 0;
}

// ===== sub_41FE50 @ 0x0041FE50..0x0041FE8E =====
BOOL sub_41FE50()
{
  _DWORD *v0; // esi
  int v1; // eax

  v0 = dword_565B40;
  if ( dword_565B40 )
  {
    do
    {
      if ( (*(int (__thiscall **)(_DWORD))(*(_DWORD *)*v0 + 8))(*v0) )
      {
        v1 = sub_4207A0();
        if ( v1 != 4 && v1 != -1 )
          break;
      }
      v0 = (_DWORD *)v0[1];
    }
    while ( v0 );
  }
  return v0 == 0;
}

// ===== sub_41FE90 @ 0x0041FE90..0x004200F7 =====
int __thiscall sub_41FE90(_DWORD *this, char **a2, int *a3, int a4)
{
  unsigned int v5; // esi
  int result; // eax
  int v7; // ecx
  _DWORD *v8; // esi
  int v9; // eax
  int v10; // ecx
  int v11; // eax
  int v12; // ecx
  unsigned int v13; // [esp+Ch] [ebp-64h]
  _DWORD v14[6]; // [esp+10h] [ebp-60h] BYREF
  _DWORD v15[6]; // [esp+28h] [ebp-48h] BYREF
  _DWORD v16[6]; // [esp+40h] [ebp-30h] BYREF
  char v17; // [esp+58h] [ebp-18h] BYREF

  v5 = sub_41B770(this);
  v13 = v5;
  if ( sub_4207A0() != 4 )
    sub_40A9E0((int)(this + 83), (int)a2, 0x80u, 0, 1);
  result = sub_4207A0();
  switch ( result )
  {
    case 0:
      result = sub_407F20(dword_565B30, this[78], v15);
      if ( result )
      {
        result = sub_408300(this[78], dword_565B30);
        if ( this[80] == result )
        {
          v7 = this[79];
          v8 = v7 != -1 ? (_DWORD *)&v17 : 0;
          if ( !v8
            || (result = sub_407F20(dword_565B30, v7, v8)) != 0
            && (result = this[81] == sub_408300(this[79], dword_565B30)) != 0 )
          {
            result = sub_412220(v15, (int)(this + 83), a2, v8, v13, this[82]);
          }
        }
      }
      break;
    case 1:
      result = sub_413500((int)(this + 83), v5, (int)a2, this[89]);
      break;
    case 2:
      result = sub_407F20(dword_565B30, this[90], v16);
      if ( result )
      {
        result = sub_408300(this[90], dword_565B30);
        if ( this[91] == result )
          result = sub_413A10(v16, (int)(this + 83), a2, (int)(this + 83), this[93], this[113]);
      }
      break;
    case 3:
      result = sub_417730(
                 (int)a2,
                 (this[85] & 0xFFFFFFFE) << 15,
                 (this[86] & 0xFFFFFFFE) << 15,
                 this + 83,
                 this[107],
                 this[108],
                 this[109],
                 this[110],
                 this[111],
                 0,
                 this[101],
                 1);
      break;
    case 4:
      v9 = this[84];
      v10 = this[85];
      v14[0] = this[83];
      v14[3] = this[86];
      v14[1] = v9;
      v11 = this[87];
      v14[2] = v10;
      v12 = this[88];
      v14[4] = v11;
      v14[5] = v12;
      sub_4091B0(v14, a3);
      sub_40A9E0((int)a2, (int)v14, 1u, 256 - v5, 1);
      result = sub_40ADF0((int)v14, a2);
      break;
    default:
      return result;
  }
  return result;
}
