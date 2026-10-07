#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_43C070 @ 0x0043C070..0x0043C1C7 =====
int __stdcall sub_43C070(int *a1)
{
  int v1; // esi
  int v2; // eax
  BOOL v3; // eax
  int v4; // ecx
  int v5; // eax
  int v6; // edx
  int v8; // [esp-4h] [ebp-44h]
  BOOL v9; // [esp+10h] [ebp-30h]
  char v10[4]; // [esp+14h] [ebp-2Ch] BYREF
  _DWORD v11[4]; // [esp+18h] [ebp-28h] BYREF
  void *v12[6]; // [esp+28h] [ebp-18h] BYREF

  v1 = *a1;
  v2 = (*(int (__thiscall **)(int *))(*a1 + 36))(a1);
  (*(void (__thiscall **)(int *, int))(v1 + 8))(a1, v2);
  if ( sub_42C330(0, a1[8], v11) )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
  }
  a1[99] ^= 1u;
  v3 = dword_507630[a1[99]] != 0;
  v9 = v3;
  if ( dword_507630[a1[99]] )
  {
    sub_42C460(a1[8]);
    sub_409080(v12, 1);
    sub_40A620((int)v12, 0);
    sub_42C520(a1[8]);
    v8 = dword_507630[a1[99]];
    v5 = sub_42C450(v4);
    sub_4097D0(v6, 128, dword_565B70, (int)v12, (int)v10, 0, 0, a1[a1[11] + 15], v5, v8);
    sub_42BB30(0, (int)v12, a1[8]);
    operator delete[](v12[0]);
    v3 = v9;
  }
  sub_42BAB0(a1[8], 0, v3);
  if ( sub_42C330(0, a1[8], v11) )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
  }
  return (*(int (__thiscall **)(int *))(*a1 + 16))(a1);
}

// ===== sub_43C1D0 @ 0x0043C1D0..0x0043C261 =====
int __thiscall sub_43C1D0(int *this)
{
  int v2; // edi
  int v3; // eax
  int v5; // eax
  int v6; // [esp-4h] [ebp-Ch]

  v2 = 0;
  if ( !this[98] )
  {
    (*(void (__thiscall **)(int *))(*this + 40))(this);
    ++this[98];
  }
  if ( this[9] )
  {
    v3 = sub_43BB40(this) - 1;
    if ( v3 )
    {
      if ( v3 != 1 )
      {
LABEL_6:
        if ( sub_431A60((unsigned int *)this) )
          sub_43C070(this);
        return v2;
      }
    }
    else
    {
      v2 = 1;
      this[98] = 0;
    }
  }
  else if ( !this[10] || !(*(int (__thiscall **)(int *))(*this + 32))(this) )
  {
    goto LABEL_6;
  }
  (*(void (__thiscall **)(int *))(*this + 40))(this);
  if ( v2 )
    return v2;
  v6 = this[11];
  v5 = sub_42D560(this[1]);
  sub_496540(268435457, v5, v6);
  return 0;
}

// ===== sub_43C270 @ 0x0043C270..0x0043C2D2 =====
_DWORD *__thiscall sub_43C270(void *this, _DWORD *a2, int a3)
{
  sub_43B380(this, a2, a3);
  *a2 = &CProcSelectItemEx::`vftable';
  a2[101] = 0;
  a2[102] = 0;
  return a2;
}

// ===== sub_43C2E0 @ 0x0043C2E0..0x0043C302 =====
void *__thiscall sub_43C2E0(void *this, char a2)
{
  sub_43C310(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43C310 @ 0x0043C310..0x0043C359 =====
int __stdcall sub_43C310(_DWORD *a1)
{
  *a1 = &CProcSelectItemEx::`vftable';
  return sub_43B4B0((int)a1);
}

// ===== sub_43C360 @ 0x0043C360..0x0043C36F =====
int __stdcall sub_43C360(int a1)
{
  return a1 & 0x3FF0203;
}

// ===== sub_43C370 @ 0x0043C370..0x0043C3BF =====
int __thiscall sub_43C370(int *this)
{
  int v2; // edi
  int v3; // eax

  if ( this[100] )
    v2 = sub_43BA10((int)this);
  else
    v2 = -1;
  if ( v2 == this[11] )
    return 0;
  v3 = sub_42D560(this[1]);
  sub_496540(268435458, v3, v2);
  sub_43B8C0((int)this, v2);
  return 1;
}

// ===== sub_43C3C0 @ 0x0043C3C0..0x0043C4B1 =====
int __thiscall sub_43C3C0(void *this, int *a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD v8[6]; // [esp+10h] [ebp-18h] BYREF

  a2[101] = 0;
  if ( this != (void *)-1 )
  {
    if ( !sub_407F20(dword_565B70, (int)this, v8) )
      return 32769;
    if ( sub_42BB30(1, (int)v8, a2[8]) )
      return 32770;
    a2[101] = 1;
  }
  a2[102] = 0;
  if ( a5 == -1 )
    goto LABEL_12;
  if ( sub_407F20(dword_565B70, a5, v8) )
  {
    if ( sub_42BB30(2, (int)v8, a2[8]) )
      return 32772;
    a2[102] = 1;
LABEL_12:
    a2[103] = a3;
    a2[104] = a4;
    a2[105] = a6;
    a2[106] = a7;
    return 0;
  }
  return 32771;
}

// ===== sub_43C4C0 @ 0x0043C4C0..0x0043C5E0 =====
int __thiscall sub_43C4C0(int *this)
{
  int *v2; // edi
  int v3; // ecx
  int v4; // esi
  int result; // eax
  int v6; // [esp+Ch] [ebp-2Ch]
  _DWORD *v7; // [esp+10h] [ebp-28h]
  int *v8; // [esp+14h] [ebp-24h]
  _DWORD v9[2]; // [esp+18h] [ebp-20h]
  _DWORD v10[2]; // [esp+20h] [ebp-18h]
  _DWORD v11[4]; // [esp+28h] [ebp-10h] BYREF

  sub_43BEE0(this);
  v2 = &this[4 * this[11] + 31];
  v3 = this[4 * this[11] + 33] - *v2 + 1;
  v8 = v2;
  v9[0] = 1;
  v9[1] = 2;
  v10[0] = 0;
  v10[1] = v3;
  v7 = this + 103;
  v6 = 0;
  do
  {
    v4 = *(_DWORD *)((char *)v9 + v6);
    if ( sub_42C330(v4, this[8], v11) )
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
      sub_443240(dword_565B6C);
    }
    if ( this[11] == -1 )
    {
      sub_42BAB0(this[8], v4, 0);
    }
    else
    {
      if ( !this[101] )
        goto LABEL_9;
      sub_42BAE0(this[8], v4, *v7 + *v2 + *(_DWORD *)((char *)v10 + v6), v2[1] + v7[1], 0);
      sub_42BAB0(this[8], v4, 1);
      sub_42C330(v4, this[8], v11);
      (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
      sub_443240(dword_565B6C);
    }
    v2 = v8;
LABEL_9:
    v7 += 2;
    result = v6 + 4;
    v6 = result;
  }
  while ( result < 8 );
  return result;
}

// ===== sub_43C5E0 @ 0x0043C5E0..0x0043C64E =====
int __thiscall sub_43C5E0(_DWORD *this)
{
  int v2; // eax
  int v4; // eax
  int v5; // [esp-4h] [ebp-Ch]

  if ( !this[98] )
  {
    (*(void (__thiscall **)(_DWORD *))(*this + 40))(this);
    ++this[98];
  }
  if ( this[9] )
  {
    v2 = sub_43BB40(this) - 1;
    if ( !v2 )
      return 1;
    if ( v2 != 1 )
      return 0;
    goto LABEL_9;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*this + 32))(this) )
  {
LABEL_9:
    (*(void (__thiscall **)(_DWORD *))(*this + 40))(this);
    v5 = this[11];
    v4 = sub_42D560(this[1]);
    sub_496540(268435457, v4, v5);
  }
  return 0;
}

// ===== sub_43C650 @ 0x0043C650..0x0043C6AC =====
_DWORD *__thiscall sub_43C650(void *this, _DWORD *a2, int a3)
{
  sub_43C270(this, a2, a3);
  *a2 = &CProcSelectItemExBlink::`vftable';
  a2[107] = 0;
  return a2;
}

// ===== sub_43C6B0 @ 0x0043C6B0..0x0043C6D2 =====
void *__thiscall sub_43C6B0(void *this, char a2)
{
  sub_43C6E0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43C6E0 @ 0x0043C6E0..0x0043C729 =====
int __stdcall sub_43C6E0(_DWORD *a1)
{
  *a1 = &CProcSelectItemExBlink::`vftable';
  return sub_43C310(a1);
}

// ===== sub_43C730 @ 0x0043C730..0x0043C736 =====
int sub_43C730()
{
  return 50;
}

// ===== sub_43C740 @ 0x0043C740..0x0043C795 =====
int __thiscall sub_43C740(int *this)
{
  if ( this[107] >= 20 || this[11] == -1 )
    return 1;
  if ( sub_431A60((unsigned int *)this) )
  {
    if ( !this[107] )
    {
      (*(void (__thiscall **)(int *))(*this + 40))(this);
      ++this[107];
      return 0;
    }
    sub_43C070(this);
    ++this[107];
  }
  return 0;
}

// ===== sub_43C7A0 @ 0x0043C7A0..0x0043C7FE =====
_DWORD *__fastcall sub_43C7A0(int a1, int a2, _DWORD *a3)
{
  sub_431BD0(a1, a2, a3);
  *a3 = &CProcShakeDspObj::`vftable';
  a3[48] = 0;
  a3[49] = 0;
  return a3;
}

// ===== sub_43C800 @ 0x0043C800..0x0043C822 =====
void *__thiscall sub_43C800(void *this, char a2)
{
  sub_43C830(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43C830 @ 0x0043C830..0x0043C89E =====
int __stdcall sub_43C830(int a1)
{
  *(_DWORD *)a1 = &CProcShakeDspObj::`vftable';
  operator delete[](*(void **)(a1 + 192));
  operator delete[](*(void **)(a1 + 196));
  return sub_431C90((_DWORD *)a1);
}

// ===== sub_43C8A0 @ 0x0043C8A0..0x0043CAEC =====
int __userpurge sub_43C8A0@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<ecx>,
        int a3@<esi>,
        int a4,
        unsigned int a5,
        int a6,
        int a7)
{
  unsigned int v8; // ecx
  unsigned int v9; // eax
  int v10; // ecx
  int v11; // ecx
  void *v12; // eax
  unsigned int v13; // ebx
  unsigned int v14; // ecx
  unsigned __int64 i; // kr00_8
  void *v16; // eax
  unsigned int v17; // ebx
  bool v18; // cf
  unsigned int v19; // eax
  unsigned int v20; // edi
  unsigned int v21; // edx
  int v22; // ecx
  int v23; // eax
  unsigned int v24; // eax
  unsigned int j; // [esp+Ch] [ebp-8h]

  if ( a1 >= 6 )
    return -2147483647;
  if ( !a5 )
    return -2147483646;
  if ( !a6 )
    return -2147483645;
  if ( !a2 || a2 < a5 )
    return -2147483644;
  *(_DWORD *)(a3 + 168) = a1;
  *(_DWORD *)(a3 + 172) = a4;
  *(_DWORD *)(a3 + 60) = 0;
  *(_DWORD *)(a3 + 64) = 0;
  *(_DWORD *)(a3 + 44) = 0;
  *(_DWORD *)(a3 + 184) = a7;
  *(_DWORD *)(a3 + 56) = a2;
  *(_DWORD *)(a3 + 176) = a5;
  *(_DWORD *)(a3 + 180) = a6;
  v8 = 0x3E8 / a2;
  *(_DWORD *)(a3 + 52) = v8;
  v9 = 0x3E8 / (a5 * v8);
  v10 = *(_DWORD *)(a3 + 36);
  *(_DWORD *)(a3 + 188) = v9;
  *(_DWORD *)(a3 + 48) = a6 * v9;
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v10 + 48))(v10, a3 + 68);
  v11 = *(_DWORD *)(a3 + 36);
  *(_DWORD *)(a3 + 108) = 0x80000000;
  *(_DWORD *)(a3 + 112) = 0x80000000;
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v11 + 4))(v11, 1);
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a3 + 36) + 12))(*(_DWORD *)(a3 + 36));
  (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)a3 + 8))(a3, *(_DWORD *)(a3 + 52));
  operator delete[](*(void **)(a3 + 192));
  v12 = operator new[](4 * a6);
  v13 = 0;
  v14 = *(_DWORD *)(a3 + 172) << 16;
  *(_DWORD *)(a3 + 192) = v12;
  for ( i = v14; v13 < *(_DWORD *)(a3 + 180); i = (__int64)((100 - a7) * i) / 100 )
    *(_DWORD *)(*(_DWORD *)(a3 + 192) + 4 * v13++) = i >> 16;
  operator delete[](*(void **)(a3 + 196));
  v16 = operator new[](4 * *(_DWORD *)(a3 + 188));
  v17 = *(_DWORD *)(a3 + 188);
  v18 = *(_DWORD *)(a3 + 168) < 4u;
  *(_DWORD *)(a3 + 196) = v16;
  v19 = 0x20000 / v17;
  v20 = v18 ? 0 : 0x8000;
  v21 = 0;
  v22 = v20;
  for ( j = 0x20000 / v17; ; v19 = j )
  {
    v22 += v19;
    if ( v22 >= 0x10000 )
    {
      v19 = -v19;
      j = v19;
      v22 = 0x20000 - v22;
    }
    if ( v22 <= 0 )
    {
      j = -v19;
      v22 = -v22;
    }
    v23 = *(_DWORD *)(a3 + 168);
    if ( !v23 || v23 == 2 )
      v24 = v20 - v22;
    else
      v24 = v22 - v20;
    *(_DWORD *)(*(_DWORD *)(a3 + 196) + 4 * v21++) = v24;
    if ( v21 >= *(_DWORD *)(a3 + 188) )
      break;
  }
  return 0;
}

// ===== sub_43CAF0 @ 0x0043CAF0..0x0043CBB5 =====
BOOL __thiscall sub_43CAF0(unsigned int *this)
{
  unsigned int v2; // ebx
  unsigned int v3; // edi
  __int64 v4; // rax
  unsigned int v5; // ecx
  BOOL v7; // [esp+Ch] [ebp-4h]

  if ( this[10] || this[38] )
  {
    this[11] = this[12];
    v7 = 1;
  }
  else
  {
    v7 = sub_4320E0(this);
  }
  v2 = this[17];
  v3 = this[18];
  if ( !v7 )
  {
    v4 = (*(unsigned int *)(this[48] + 4 * (this[11] / this[47]))
        * (__int64)*(int *)(this[49] + 4 * (this[11] % this[47]))) >> 16;
    switch ( this[42] )
    {
      case 0u:
      case 1u:
      case 4u:
        v3 += v4;
        break;
      case 2u:
      case 3u:
      case 5u:
        v2 += v4;
        break;
      default:
        break;
    }
  }
  if ( v2 != this[27] || v3 != this[28] )
  {
    v5 = this[9];
    this[27] = v2;
    this[28] = v3;
    (*(void (__thiscall **)(unsigned int))(*(_DWORD *)v5 + 12))(v5);
    (*(void (__thiscall **)(unsigned int, unsigned int, unsigned int))(*(_DWORD *)this[9] + 44))(this[9], v2, v3);
    (*(void (__thiscall **)(unsigned int))(*(_DWORD *)this[9] + 12))(this[9]);
    (*(void (__thiscall **)(unsigned int *))(*this + 16))(this);
  }
  return v7;
}

// ===== sub_43CBD0 @ 0x0043CBD0..0x0043CC2E =====
int __thiscall sub_43CBD0(void *this, _DWORD *a2)
{
  _DWORD *v2; // edx
  int v3; // edx
  int v4; // ecx

  sub_4318F0(a2, (int)this);
  *v2 = &CProcShakeScreen::`vftable';
  sub_461D10();
  *(_DWORD *)(v3 + 32) = v4;
  *(_DWORD *)(v3 + 60) = v4;
  return v3;
}

// ===== sub_43CC30 @ 0x0043CC30..0x0043CC52 =====
void *__thiscall sub_43CC30(void *this, char a2)
{
  sub_43CC60(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43CC60 @ 0x0043CC60..0x0043CCD9 =====
int __stdcall sub_43CC60(_DWORD *a1)
{
  *a1 = &CProcShakeScreen::`vftable';
  if ( a1[15] )
  {
    sub_46DF00(-1);
    sub_46D7A0();
    sub_46D7B0();
  }
  sub_461D10();
  return sub_431950(a1);
}

// ===== sub_43CCE0 @ 0x0043CCE0..0x0043CDCE =====
int __userpurge sub_43CCE0@<eax>(_DWORD *a1@<edi>, unsigned int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  int v9; // ecx
  int v10; // eax

  if ( a2 >= 4 )
    return -2147483647;
  v9 = a4;
  if ( a4 < 1 )
    return -2147483646;
  if ( a5 < 1 )
    return -2147483645;
  v10 = a7;
  if ( a7 < 1 || a7 < a4 )
    return -2147483644;
  if ( a1[15] )
  {
    sub_46D7A0();
    sub_46D7B0();
    v10 = a7;
    v9 = a4;
  }
  a1[9] = a2;
  a1[12] = a5;
  a1[13] = a6;
  a1[14] = v10;
  a1[10] = a3 << 11;
  a1[11] = v9;
  a1[15] = a8;
  a1[19] = 0;
  a1[20] = 0;
  a1[23] = 0;
  a1[24] = 0;
  a1[16] = v10 / v9;
  if ( a8 )
  {
    sub_46D6C0();
    sub_46D720(-1);
    sub_46DF00(-1);
    a1[27] = sub_46E5C0();
  }
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*a1 + 8))(a1, 0);
  a1[8] = 1;
  return 0;
}

// ===== sub_43CDD0 @ 0x0043CDD0..0x0043CFED =====
BOOL __thiscall sub_43CDD0(unsigned int *this)
{
  int v2; // eax
  int v3; // eax
  unsigned int v4; // edx
  int v5; // eax
  unsigned int v6; // ecx
  int v7; // edx
  int v8; // eax
  int v9; // ecx
  unsigned int v10; // eax
  bool v11; // sf
  bool v12; // of
  int v13; // esi
  int v15; // [esp-8h] [ebp-48h]
  int v16; // [esp+8h] [ebp-38h]
  _DWORD v17[8]; // [esp+1Ch] [ebp-24h] BYREF

  if ( !this[8] )
    goto LABEL_20;
  if ( this[15] )
  {
    v2 = sub_46E5C0();
    v16 = this[27] == v2;
    if ( this[27] != v2 )
      goto LABEL_19;
  }
  else
  {
    v16 = 1;
  }
  if ( sub_431A60(this) )
  {
    sub_431A30((int)this, 1000 / (int)this[14]);
    if ( !this[20] )
    {
      v3 = (int)(4 * this[10]) / (int)this[16];
      v4 = this[23];
      v15 = this[19] & 3;
      this[17] = 0;
      this[21] = v4;
      this[18] = v3;
      this[22] = this[24];
      sub_43D030(this, v15);
      v5 = this[24] - this[22];
      this[25] = this[23] - this[21];
      this[26] = v5;
    }
    v6 = this[18];
    this[17] += v6;
    v7 = this[17];
    v8 = this[10];
    if ( v8 < v7 || v7 < -v8 )
      this[18] = -v6;
    if ( v8 < v7 )
      this[17] = 2 * v8 - v7;
    v9 = this[17];
    if ( v9 < -v8 )
      this[17] = -v9 - 2 * v8;
    if ( !sub_431A60(this) )
    {
      v17[4] = 1;
      v17[5] = 0;
      v17[6] = 1;
      v17[7] = 0;
      memset(v17, 0, 12);
      v17[3] = 1;
      sub_461A70(1, 0);
    }
    if ( (int)++this[20] >= (int)this[16] )
    {
      v10 = ++this[19];
      v12 = __OFSUB__(v10, this[12]);
      v11 = (int)(v10 - this[12]) < 0;
      this[10] = (int)(this[10] * (100 - this[13])) / 100;
      this[20] = 0;
      v16 = v11 ^ v12;
    }
  }
LABEL_19:
  this[8] = v16;
  if ( !v16 )
  {
LABEL_22:
    v13 = 0;
    sub_461A70(1, 0);
    return v13 == 0;
  }
LABEL_20:
  if ( !sub_4319C0((int)this) )
    goto LABEL_22;
  v13 = 1;
  return v13 == 0;
}

// ===== sub_43CFF0 @ 0x0043CFF0..0x0043D026 =====
int __usercall sub_43CFF0@<eax>(int a1@<eax>)
{
  int v1; // edi
  int v2; // ebx
  int v3; // esi
  int v5; // [esp+Ch] [ebp-4h]

  v1 = 0;
  v5 = a1 + 1;
  v2 = 8;
  do
  {
    v3 = rand() << 15;
    v1 += (v3 | rand()) % v5;
    --v2;
  }
  while ( v2 );
  return v1 >> 3;
}

// ===== sub_43D030 @ 0x0043D030..0x0043D0B3 =====
BOOL __userpurge sub_43D030@<eax>(int *a1@<edi>, int a2, int a3)
{
  int v4; // eax
  int v5; // esi
  int v6; // eax
  BOOL result; // eax
  BOOL v8; // [esp+10h] [ebp+8h]

  v8 = *(_DWORD *)(a2 + 36) == 3;
  if ( !v8 )
    return v8;
  v4 = *(_DWORD *)(a2 + 40);
  if ( v4 <= 0 )
  {
    *a1 = 0;
    a1[1] = 0;
    return v8;
  }
  v5 = sub_43CFF0(v4);
  v6 = sub_43CFF0(*(_DWORD *)(a2 + 40));
  switch ( a3 )
  {
    case 0:
      goto LABEL_7;
    case 1:
      *a1 = v5;
      a1[1] = v6;
      return v8;
    case 2:
      *a1 = -v5;
      a1[1] = v6;
      return v8;
    case 3:
      v5 = -v5;
LABEL_7:
      *a1 = v5;
      a1[1] = -v6;
      result = v8;
      break;
    default:
      return v8;
  }
  return result;
}

// ===== sub_43D0D0 @ 0x0043D0D0..0x0043D11F =====
_DWORD *__thiscall sub_43D0D0(void *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = sub_4318F0(a2, (int)this);
  *result = &CProcUsingThread::`vftable';
  return result;
}

// ===== sub_43D120 @ 0x0043D120..0x0043D141 =====
void *__thiscall sub_43D120(void *this, char a2)
{
  sub_43D150();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43D150 @ 0x0043D150..0x0043D197 =====
int __thiscall sub_43D150(_DWORD *this)
{
  *this = &CProcUsingThread::`vftable';
  return sub_431950(this);
}

// ===== sub_43D1A0 @ 0x0043D1A0..0x0043D1A7 =====
void sub_43D1A0()
{
  ++dword_565D58;
}

// ===== sub_43D1B0 @ 0x0043D1B0..0x0043D1B7 =====
void sub_43D1B0()
{
  --dword_565D58;
}

// ===== sub_43D1C0 @ 0x0043D1C0..0x0043D1C6 =====
int sub_43D1C0()
{
  return dword_565D58;
}

// ===== sub_43D1D0 @ 0x0043D1D0..0x0043D230 =====
_DWORD *__thiscall sub_43D1D0(void *this, _DWORD *a2, int a3)
{
  sub_4318F0(a2, (int)this);
  *a2 = &CProcWaitTiming::`vftable';
  sub_431A10(a2, a3);
  return a2;
}

// ===== sub_43D230 @ 0x0043D230..0x0043D252 =====
void *__thiscall sub_43D230(void *this, char a2)
{
  sub_43D260(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43D260 @ 0x0043D260..0x0043D2A8 =====
int __stdcall sub_43D260(_DWORD *a1)
{
  *a1 = &CProcWaitTiming::`vftable';
  return sub_431950(a1);
}

// ===== sub_43D2B0 @ 0x0043D2B0..0x0043D2D9 =====
BOOL __thiscall sub_43D2B0(unsigned int *this)
{
  sub_431AF0((int)this, (int)this);
  return sub_431A60(this) || !sub_4319C0((int)this);
}

// ===== sub_43D2E0 @ 0x0043D2E0..0x0043D37B =====
_DWORD *__fastcall sub_43D2E0(int a1, int a2, _DWORD *a3, int a4, int a5)
{
  int v6; // edi

  sub_4318F0(a3, a1);
  *a3 = &CProcWaitTimingEx::`vftable';
  sub_431A10(a3, a4);
  a3[8] = a5;
  if ( a5 )
  {
    a3[9] = a2;
    v6 = (a2 << 16) | 0xFFFF;
    sub_46D6C0();
    sub_46D720(v6);
    sub_46DF00(v6);
  }
  a3[10] = 0;
  return a3;
}

// ===== sub_43D380 @ 0x0043D380..0x0043D3A2 =====
void *__thiscall sub_43D380(void *this, char a2)
{
  sub_43D3B0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43D3B0 @ 0x0043D3B0..0x0043D421 =====
int __stdcall sub_43D3B0(_DWORD *a1)
{
  *a1 = &CProcWaitTimingEx::`vftable';
  if ( a1[8] )
  {
    sub_46D7A0();
    sub_46D7B0();
  }
  return sub_431950(a1);
}

// ===== sub_43D430 @ 0x0043D430..0x0043D4A3 =====
int __thiscall sub_43D430(unsigned int *this)
{
  int v2; // esi

  sub_431AF0((int)this, (int)this);
  v2 = 0;
  if ( sub_431A60(this) || !sub_4319C0((int)this) || this[10] )
  {
    sub_4450D0();
    return 1;
  }
  else
  {
    if ( this[8] )
    {
      if ( (sub_46DF00((this[9] << 16) | 0xFFFF) & (dword_507690 | 0x80000181)) != 0 )
      {
        v2 = 1;
        sub_4450D0();
      }
    }
    return v2;
  }
}

// ===== sub_43D4B0 @ 0x0043D4B0..0x0043D4C6 =====
int __thiscall sub_43D4B0(_DWORD *this, _DWORD *a2)
{
  int result; // eax

  result = *a2 - 1;
  if ( *a2 == 1 )
    this[10] = 1;
  return result;
}

// ===== sub_43D4D0 @ 0x0043D4D0..0x0043D535 =====
_DWORD *__thiscall sub_43D4D0(void *this, _DWORD *a2)
{
  int v2; // edx
  int v4; // [esp-8h] [ebp-1Ch]

  sub_4318F0(a2, (int)this);
  v4 = a2[1];
  *a2 = &CProcWaitWndMsg::`vftable';
  a2[8] = v2;
  sub_499F40(v4, v2);
  return a2;
}

// ===== sub_43D540 @ 0x0043D540..0x0043D562 =====
void *__thiscall sub_43D540(void *this, char a2)
{
  sub_43D570(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43D570 @ 0x0043D570..0x0043D5D0 =====
int __thiscall sub_43D570(void *this, _DWORD *a2)
{
  *a2 = &CProcWaitWndMsg::`vftable';
  sub_499FA0(this, a2[8]);
  return sub_431950(a2);
}

// ===== sub_43D5D0 @ 0x0043D5D0..0x0043D678 =====
int __thiscall sub_43D5D0(void *this)
{
  int v3; // [esp+20h] [ebp-8h]

  sub_431AF0((int)this, (int)this);
  if ( sub_499FE0() )
  {
    if ( sub_4319C0((int)this) && !v3 )
    {
      return 0;
    }
    else
    {
      sub_4450D0();
      sub_4450D0();
      return 1;
    }
  }
  else
  {
    MessageBoxA(0, &byte_4E5A50, &byte_4E5A40, 0);
    return 1;
  }
}

// ===== sub_43D680 @ 0x0043D680..0x0043D6AA =====
int __stdcall sub_43D680(int a1)
{
  _DWORD *v1; // ebx

  v1 = dword_56674C;
  sub_43E190();
  return sub_41E970(v1[20], a1);
}

// ===== sub_43D6B0 @ 0x0043D6B0..0x0043D6E8 =====
int __stdcall sub_43D6B0(int a1, int a2, int a3)
{
  _DWORD **v3; // esi

  v3 = (_DWORD **)dword_56674C;
  sub_43E190();
  (*(void (__thiscall **)(_DWORD *, int))(*v3[20] + 72))(v3[20], a3);
  return sub_41C550(a1, v3[20], a2);
}

// ===== sub_43D6F0 @ 0x0043D6F0..0x0043D750 =====
int __stdcall sub_43D6F0(int a1, int a2, int a3, int a4, int a5, int a6)
{
  void *v6; // ebx

  v6 = dword_56674C;
  sub_43E190();
  if ( sub_41F540(a5, *((_DWORD *)v6 + 20), a6) )
    return sub_41F4A0(a1, *((_DWORD **)v6 + 20), a2, a3, a4) != 0 ? 0 : 2;
  else
    return 1;
}

// ===== sub_43D750 @ 0x0043D750..0x0043D83B =====
int __stdcall sub_43D750(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  _DWORD **v9; // esi
  int v10; // ebx
  int v11; // eax
  unsigned int v13; // eax

  v9 = (_DWORD **)dword_56674C;
  sub_43E190();
  v10 = -1;
  v11 = sub_41D350(v9[20], a1, a2, a3, a4, a5, a6);
  if ( v11 )
  {
    if ( v11 == -2147483647 )
      return 1;
    if ( v11 == -2147483646 )
      return 2;
  }
  else
  {
    v13 = sub_41D440(v9[20], a7, a8);
    if ( v13 > 0x80000004 )
    {
      if ( v13 == -2147483643 )
        return 5;
    }
    else
    {
      switch ( v13 )
      {
        case 0x80000004:
          return 4;
        case 0u:
          (*(void (__thiscall **)(_DWORD *, int))(*v9[20] + 72))(v9[20], a9);
          return 0;
        case 0x80000003:
          return 3;
      }
    }
  }
  return v10;
}

// ===== sub_43D840 @ 0x0043D840..0x0043D8A7 =====
int __stdcall sub_43D840(int a1, int a2, int a3)
{
  void *v3; // esi
  int v4; // eax

  v3 = dword_56674C;
  sub_43E190();
  (*(void (__thiscall **)(_DWORD, int))(**((_DWORD **)v3 + 20) + 72))(*((_DWORD *)v3 + 20), a3);
  v4 = sub_41C970(*((_DWORD *)v3 + 20), a1, a2);
  switch ( v4 )
  {
    case 0:
      return 0;
    case -2147483647:
      return 1;
    case -2147483646:
      return 2;
  }
  return a3;
}

// ===== sub_43D8B0 @ 0x0043D8B0..0x0043D975 =====
int __stdcall sub_43D8B0(int a1, int a2, int a3, int a4, int a5)
{
  void *v5; // esi
  unsigned int v6; // eax

  v5 = dword_56674C;
  sub_43E190();
  v6 = sub_41CF20(*((_DWORD **)v5 + 20), a1, a2, a3);
  if ( v6 > 0x80000003 )
  {
    switch ( v6 )
    {
      case 0x80000004:
        return 4;
      case 0x80000005:
        return 5;
      case 0x80000006:
        return 6;
    }
  }
  else
  {
    switch ( v6 )
    {
      case 0x80000003:
        return 3;
      case 0u:
        (*(void (__thiscall **)(_DWORD, int))(**((_DWORD **)v5 + 20) + 72))(*((_DWORD *)v5 + 20), a4);
        sub_41D050(*((_DWORD *)v5 + 20), a5);
        return 0;
      case 0x80000001:
        return 1;
      case 0x80000002:
        return 2;
    }
  }
  return a3;
}

// ===== sub_43D980 @ 0x0043D980..0x0043DA09 =====
int __stdcall sub_43D980(int a1, unsigned int a2, int a3)
{
  void *v3; // esi
  int v4; // eax
  int v6; // eax

  v3 = dword_56674C;
  sub_43E190();
  v4 = sub_41D9F0(*((_DWORD **)v3 + 20), a1);
  if ( v4 )
  {
    if ( v4 == -2147483647 )
      return 1;
    if ( v4 == -2147483646 )
      return 2;
  }
  else
  {
    v6 = sub_41DA70(a2, *((_DWORD *)v3 + 20));
    if ( !v6 )
    {
      (*(void (__thiscall **)(_DWORD, int))(**((_DWORD **)v3 + 20) + 72))(*((_DWORD *)v3 + 20), a3);
      return 0;
    }
    if ( v6 == -2147483645 )
      return 3;
  }
  return a1;
}

// ===== sub_43DA10 @ 0x0043DA10..0x0043DAFF =====
int __stdcall sub_43DA10(int a1, int a2, int a3, unsigned int a4, int a5)
{
  _DWORD **v5; // esi
  int v6; // eax
  unsigned int v8; // eax
  int v9; // [esp+Ch] [ebp-4h]

  v5 = (_DWORD **)dword_56674C;
  sub_43E190();
  v6 = sub_41EB90(v5[20], a1);
  if ( v6 )
  {
    if ( v6 == -2147483647 )
      return 1;
    if ( v6 == -2147483646 )
      return 2;
  }
  else
  {
    v8 = sub_41EC00(v5[20], a2, a3, a4, a5);
    if ( v8 > 0x80000005 )
    {
      if ( v8 == -2147483642 )
        return 6;
      if ( v8 == -2147483641 )
        return 7;
    }
    else
    {
      switch ( v8 )
      {
        case 0x80000005:
          return 5;
        case 0u:
          return 0;
        case 0x80000003:
          return 3;
        case 0x80000004:
          return 4;
      }
    }
  }
  return v9;
}

// ===== sub_43DB00 @ 0x0043DB00..0x0043DB8A =====
int __stdcall sub_43DB00(int a1, int a2, int a3, unsigned int a4, unsigned int a5)
{
  _DWORD *v5; // esi
  int v6; // eax
  int v8; // eax

  v5 = dword_56674C;
  sub_43E190();
  v6 = sub_41F910(a1, v5[20]);
  if ( v6 )
  {
    if ( v6 == -2147483647 )
      return 1;
  }
  else
  {
    v8 = sub_41F9A0(a4, a5, v5[20]);
    if ( !v8 )
    {
      (*(void (__thiscall **)(_DWORD, int, int))(*(_DWORD *)v5[20] + 44))(v5[20], a2 << 16, a3 << 16);
      return 0;
    }
    if ( v8 == -2147483646 )
      return 2;
  }
  return a3;
}

// ===== sub_43DB90 @ 0x0043DB90..0x0043DBFD =====
int __stdcall sub_43DB90(int a1, int a2, int a3)
{
  _DWORD *v3; // esi
  int v4; // eax
  int v6; // eax

  v3 = dword_56674C;
  sub_43E190();
  v4 = sub_41F910(a1, v3[20]);
  if ( v4 )
  {
    if ( v4 == -2147483647 )
      return 1;
  }
  else
  {
    v6 = sub_41F120((_DWORD *)v3[20], a2, a3);
    if ( !v6 )
      return 0;
    if ( v6 == -2147483646 )
      return 2;
  }
  return a1;
}

// ===== sub_43DC00 @ 0x0043DC00..0x0043DCA8 =====
int __stdcall sub_43DC00(int a1, int a2, int a3, int a4, unsigned int a5)
{
  void *v5; // ebx
  int v6; // eax

  v5 = dword_56674C;
  sub_43E190();
  v6 = sub_41E660(*((_DWORD **)v5 + 20), a1, a2);
  if ( v6 )
  {
    if ( v6 == -2147483647 )
    {
      return 1;
    }
    else if ( v6 == -2147483646 )
    {
      return 2;
    }
    else
    {
      return a2;
    }
  }
  else if ( sub_41E720(*((_DWORD **)v5 + 20), a3) )
  {
    if ( sub_41E740(a5, *((_DWORD *)v5 + 20), *((_DWORD *)v5 + 20)) )
    {
      (*(void (__thiscall **)(_DWORD, int))(**((_DWORD **)v5 + 20) + 72))(*((_DWORD *)v5 + 20), a4);
      return 0;
    }
    else
    {
      return 4;
    }
  }
  else
  {
    return 3;
  }
}

// ===== sub_43DCB0 @ 0x0043DCB0..0x0043DD52 =====
int __stdcall sub_43DCB0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  _DWORD *v9; // esi
  int v10; // edi
  int v11; // eax
  int v13; // eax
  int v14; // ecx

  v9 = dword_56674C;
  sub_43E190();
  v10 = v9[20];
  v11 = sub_41DDC0(0, v10, a3, a4, a5);
  if ( v11 )
  {
    if ( v11 == -2147483646 )
      return 3;
  }
  else
  {
    v13 = sub_41DE80(0, a8, a7, v10, a6, a9);
    if ( !v13 )
    {
      sub_41DC20(0, v10, 1);
      sub_41DFB0(0, v14);
      (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v10 + 44))(v10, a1, a2);
      return 0;
    }
    if ( v13 == -2147483645 )
      return 4;
  }
  return a5;
}

// ===== sub_43DD60 @ 0x0043DD60..0x0043DDA9 =====
int __stdcall sub_43DD60(unsigned int a1)
{
  int v1; // ecx
  int v2; // eax

  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v2 = sub_41DFB0(a1, v1);
  if ( !v2 )
    return 0;
  if ( v2 == -2147483647 )
    return 2;
  return a1;
}

// ===== sub_43DDB0 @ 0x0043DDB0..0x0043DE0B =====
int __stdcall sub_43DDB0(unsigned int a1, int a2)
{
  _DWORD *v2; // esi
  int v3; // ecx
  int v4; // eax

  v2 = dword_56674C;
  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v4 = sub_41DC20(a1, v3, a2);
  if ( v4 )
  {
    if ( v4 == -2147483647 )
      return 2;
    else
      return a2;
  }
  else
  {
    sub_430D10(v2[5]);
    return 0;
  }
}

// ===== sub_43DE10 @ 0x0043DE10..0x0043DE86 =====
int __userpurge sub_43DE10@<eax>(unsigned int a1@<edi>, unsigned int a2, unsigned int a3)
{
  _DWORD *v3; // ebx
  int v4; // esi
  int v5; // eax

  v3 = dword_56674C;
  v4 = *((_DWORD *)dword_56674C + 20);
  if ( sub_4207A0(v4) != 12 )
    return 1;
  v5 = sub_41DC80(a2, a1, v4, a3);
  if ( v5 )
  {
    if ( v5 == -2147483647 )
      return 2;
    else
      return a3;
  }
  else
  {
    sub_41DC50(a1, v4, &a3);
    if ( a3 )
      sub_430D10(v3[5]);
    return 0;
  }
}

// ===== sub_43DE90 @ 0x0043DE90..0x0043DEFB =====
int __userpurge sub_43DE90@<eax>(unsigned int a1@<esi>, int a2)
{
  _DWORD *v2; // edi
  int v3; // ecx
  int v4; // eax
  int v5; // ecx

  v2 = dword_56674C;
  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v4 = sub_41DD30(a1, v3, a2);
  if ( v4 )
  {
    if ( v4 == -2147483647 )
      return 2;
    else
      return a2;
  }
  else
  {
    sub_41DC50(a1, v5, &a2);
    if ( a2 )
      sub_430D10(v2[5]);
    return 0;
  }
}

// ===== sub_43DF00 @ 0x0043DF00..0x0043DF6B =====
int __userpurge sub_43DF00@<eax>(unsigned int a1@<esi>, int a2)
{
  _DWORD *v2; // edi
  int v3; // ecx
  int v4; // eax
  int v5; // ecx

  v2 = dword_56674C;
  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v4 = sub_41DD60(a1, v3, a2);
  if ( v4 )
  {
    if ( v4 == -2147483647 )
      return 2;
    else
      return a2;
  }
  else
  {
    sub_41DC50(a1, v5, &a2);
    if ( a2 )
      sub_430D10(v2[5]);
    return 0;
  }
}

// ===== sub_43DF70 @ 0x0043DF70..0x0043DFF7 =====
int __userpurge sub_43DF70@<eax>(unsigned int a1@<edi>, int a2)
{
  _DWORD *v2; // ebx
  int v3; // esi
  int v4; // edx
  int v5; // ecx
  int v6; // eax

  v2 = dword_56674C;
  v3 = *((_DWORD *)dword_56674C + 20);
  if ( sub_4207A0(v3) != 12 )
    return 1;
  v6 = sub_41DDC0(a1, v3, v4, v5, a2);
  if ( v6 )
  {
    if ( v6 == -2147483647 )
    {
      return 2;
    }
    else if ( v6 == -2147483646 )
    {
      return 3;
    }
    else
    {
      return a2;
    }
  }
  else
  {
    sub_41DC50(a1, v3, &a2);
    if ( a2 )
      sub_430D10(v2[5]);
    return 0;
  }
}

// ===== sub_43E000 @ 0x0043E000..0x0043E08D =====
int __userpurge sub_43E000@<eax>(int a1@<esi>, unsigned int a2, int a3)
{
  unsigned int v3; // ebx
  int v4; // edi
  int v5; // edx
  int v6; // ecx
  int v7; // eax

  v3 = a2;
  v4 = *((_DWORD *)dword_56674C + 20);
  if ( sub_4207A0(v4) != 12 )
    return 1;
  v7 = sub_41DE80(v3, v5, a1, v4, v6, a3);
  if ( v7 )
  {
    if ( v7 == -2147483647 )
    {
      return 2;
    }
    else if ( v7 == -2147483645 )
    {
      return 4;
    }
    else
    {
      return a2;
    }
  }
  else
  {
    sub_41DC50(v3, v4, &a2);
    if ( a2 )
      sub_430D10(*((_DWORD *)dword_56674C + 5));
    return 0;
  }
}

// ===== sub_43E090 @ 0x0043E090..0x0043E0DE =====
int __stdcall sub_43E090(unsigned int a1, int a2)
{
  int v2; // edx
  int v3; // ecx
  int v4; // eax

  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v4 = sub_41DEF0(a1, v3, v2, a2);
  if ( !v4 )
    return 0;
  if ( v4 == -2147483647 )
    return 2;
  return a2;
}

// ===== sub_43E0E0 @ 0x0043E0E0..0x0043E12E =====
int __stdcall sub_43E0E0(unsigned int a1, int a2)
{
  int v2; // edx
  int v3; // ecx
  int v4; // eax

  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v4 = sub_41DF20(a1, v3, v2, a2);
  if ( !v4 )
    return 0;
  if ( v4 == -2147483647 )
    return 2;
  return a2;
}

// ===== sub_43E130 @ 0x0043E130..0x0043E18B =====
int __userpurge sub_43E130@<eax>(unsigned int a1@<esi>, int a2, int a3, int a4)
{
  int v4; // ecx
  int v5; // eax
  int v6; // ecx

  if ( sub_4207A0(*((_DWORD *)dword_56674C + 20)) != 12 )
    return 1;
  v5 = sub_41DF50(a1, v4, a2);
  if ( v5 )
  {
    if ( v5 == -2147483647 )
      return 2;
    else
      return a2;
  }
  else
  {
    sub_41DF80(a1, v6, a3, a4);
    return 0;
  }
}

// ===== sub_43E190 @ 0x0043E190..0x0043E460 =====
int __usercall sub_43E190@<eax>(int a1@<edi>, int *a2@<esi>)
{
  int v2; // ecx
  void (__thiscall ***v3)(_DWORD, int); // ecx
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  _DWORD *v6; // eax
  _DWORD *v7; // eax
  _DWORD *v8; // eax
  _DWORD *v9; // eax
  _DWORD *v10; // eax
  _DWORD *v11; // eax
  _DWORD *v12; // eax
  _DWORD *v13; // eax
  _DWORD *v14; // eax
  _DWORD *v15; // eax
  _DWORD *v16; // eax
  int v17; // eax

  if ( a1 != sub_4207A0(a2[20]) )
  {
    sub_430770(v2, a2[5]);
    v3 = (void (__thiscall ***)(_DWORD, int))a2[20];
    if ( v3 )
      (**v3)(v3, 1);
    a2[20] = 0;
  }
  if ( !a2[20] )
  {
    switch ( a1 )
    {
      case 1:
        v4 = operator new(0x144u);
        if ( !v4 )
          goto LABEL_32;
        goto LABEL_8;
      case 2:
        v6 = operator new(0x14Cu);
        if ( !v6 )
          goto LABEL_32;
        v5 = (_DWORD *)sub_41C460(v6);
        break;
      case 3:
        v7 = operator new(0x1A4u);
        if ( !v7 )
          goto LABEL_32;
        v5 = sub_41F380(v7);
        break;
      case 4:
        v8 = operator new(0x170u);
        if ( !v8 )
          goto LABEL_32;
        v5 = sub_41D210(v8);
        break;
      case 5:
        v9 = operator new(0x2244u);
        if ( !v9 )
          goto LABEL_32;
        v5 = sub_41C780(v9);
        break;
      case 6:
        v10 = operator new(0x158u);
        if ( !v10 )
          goto LABEL_32;
        v5 = sub_41CE30(v10);
        break;
      case 7:
        v11 = operator new(0x148u);
        if ( !v11 )
          goto LABEL_32;
        v5 = sub_41D910(v11);
        break;
      case 8:
        v12 = operator new(0x168u);
        if ( !v12 )
          goto LABEL_32;
        v5 = (_DWORD *)sub_41EA30(v12);
        break;
      case 9:
        v13 = operator new(0x170u);
        if ( !v13 )
          goto LABEL_32;
        v5 = sub_41F830(v13);
        break;
      case 10:
        v14 = operator new(0x154u);
        if ( !v14 )
          goto LABEL_32;
        v5 = sub_41F040(v14);
        break;
      case 11:
        v15 = operator new(0x154u);
        if ( !v15 )
          goto LABEL_32;
        v5 = sub_41E560(v15);
        break;
      case 12:
        v16 = operator new(0x3E0u);
        if ( !v16 )
          goto LABEL_32;
        v5 = sub_41DB20(v16);
        break;
      default:
        v4 = operator new(0x144u);
        if ( v4 )
LABEL_8:
          v5 = sub_41E890(v4);
        else
LABEL_32:
          v5 = 0;
        break;
    }
    a2[20] = (int)v5;
    sub_4306F0(a2[5], (int)v5);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2[20] + 4))(a2[20], a2[21]);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a2[20] + 120))(a2[20], a2[22]);
  }
  v17 = sub_4207A0(a2[20]);
  sub_431140(a2[5], v17);
  return sub_430D10(a2[5]);
}

// ===== sub_43E490 @ 0x0043E490..0x0043E4B4 =====
int __usercall sub_43E490@<eax>(int a1@<eax>, int a2@<edi>, int *a3@<esi>)
{
  int v3; // ecx

  v3 = a3[20];
  a3[21] = a1;
  a3[22] = a2;
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a1);
  (*(void (__thiscall **)(int, int))(*(_DWORD *)a3[20] + 120))(a3[20], a2);
  return sub_430D10(a3[5]);
}

// ===== sub_43E4C0 @ 0x0043E4C0..0x0043E4CD =====
int sub_43E4C0()
{
  return sub_4207A0(*((_DWORD *)dword_56674C + 20));
}

// ===== sub_43E4D0 @ 0x0043E4D0..0x0043E50F =====
_DWORD *__stdcall sub_43E4D0(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 23;
  v2 = 512;
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
  a1[535] = 0;
  a1[536] = 0;
  return result;
}

// ===== sub_43E510 @ 0x0043E510..0x0043E5C3 =====
int sub_43E510()
{
  int *v0; // esi
  int result; // eax
  int v2; // edi
  _DWORD *i; // eax
  _DWORD *v4; // eax
  void *v5; // ecx
  _DWORD *v6; // eax

  v0 = (int *)dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 535) < 512 )
  {
    v2 = 0;
    for ( i = (char *)dword_56674C + 92; *i; ++v2 )
      ++i;
    v4 = operator new(0x418u);
    if ( v4 )
    {
      v5 = (void *)v0[536];
      v0[536] = (int)v5 + 1;
      v6 = sub_4256C0(v5, v4, 1);
    }
    else
    {
      v6 = 0;
    }
    v0[v2 + 23] = (int)v6;
    sub_4306F0(v0[5], (int)v6);
    ++v0[535];
    return v2 + 0x80000000;
  }
  return result;
}

// ===== sub_43E5D0 @ 0x0043E5D0..0x0043E602 =====
int __userpurge sub_43E5D0@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0x80000000 && (v3 = sub_443260(0), v3 < 0x200) )
    return *(_DWORD *)(a2 + 4 * v3 + 92);
  else
    return v2;
}

// ===== sub_43E610 @ 0x0043E610..0x0043E682 =====
BOOL __stdcall sub_43E610(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_43E5D0(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 23];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 23] = 0;
    --v1[535];
  }
  return v2 != 0;
}

// ===== sub_43E690 @ 0x0043E690..0x0043E72B =====
int __userpurge sub_43E690@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD *v7; // edi
  int v8; // esi
  int v9; // eax

  v7 = dword_56674C;
  v8 = sub_43E5D0(a1, (int)dword_56674C);
  if ( !v8 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 8))(v8) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 12))(v8);
  v9 = sub_426F50(a4, v8, a2, a3, a5, a6, a7);
  if ( v9 )
  {
    if ( v9 == -2147483647 )
      return 1;
    else
      return a7;
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 8))(v8) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 12))(v8);
    sub_4307D0(v7[5], v8);
    return 0;
  }
}

// ===== sub_43E730 @ 0x0043E730..0x0043E7EE =====
int __userpurge sub_43E730@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  _DWORD *v9; // edi
  _DWORD *v10; // esi
  unsigned int v11; // eax

  v9 = dword_56674C;
  v10 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v10 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v10 + 8))(v10) )
    (*(void (__thiscall **)(_DWORD *))(*v10 + 12))(v10);
  v11 = sub_426FA0(a9, a6, v10, a2, a3, a4, a5, a7, a8);
  if ( v11 > 0x80000002 )
  {
    if ( v11 == -2147483645 )
      return 9;
    return a8;
  }
  if ( v11 >= 0x80000001 )
    return 1;
  if ( v11 )
    return a8;
  if ( (*(int (__thiscall **)(_DWORD *))(*v10 + 8))(v10) )
    (*(void (__thiscall **)(_DWORD *))(*v10 + 12))(v10);
  sub_4307D0(v9[5], (int)v10);
  return 0;
}

// ===== sub_43E7F0 @ 0x0043E7F0..0x0043E8B7 =====
int __userpurge sub_43E7F0@<eax>(
        int a1@<eax>,
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
  _DWORD *v13; // edi
  _DWORD *v14; // esi
  int v15; // eax

  v13 = dword_56674C;
  v14 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v14 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v14 + 8))(v14) )
    (*(void (__thiscall **)(_DWORD *))(*v14 + 12))(v14);
  v15 = sub_427000(a3, a2, v14, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
  if ( v15 )
  {
    if ( v15 == -2147483647 )
    {
      return 1;
    }
    else if ( v15 == -2147483644 )
    {
      return 8;
    }
    else
    {
      return a13;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(_DWORD *))(*v14 + 8))(v14) )
      (*(void (__thiscall **)(_DWORD *))(*v14 + 12))(v14);
    sub_4307D0(v13[5], (int)v14);
    return 0;
  }
}

// ===== sub_43E8C0 @ 0x0043E8C0..0x0043E981 =====
int __userpurge sub_43E8C0@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  _DWORD *v10; // edi
  _DWORD *v11; // esi
  unsigned int v12; // eax

  v10 = dword_56674C;
  v11 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v11 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v11 + 8))(v11) )
    (*(void (__thiscall **)(_DWORD *))(*v11 + 12))(v11);
  v12 = sub_4270A0(a5, a4, v11, a2, a3, a6, a7, a8, a9, a10);
  if ( v12 > 0x80000002 )
  {
    if ( v12 == -2147483638 )
      return 2;
    return a10;
  }
  if ( v12 >= 0x80000001 )
    return 1;
  if ( v12 )
    return a10;
  if ( (*(int (__thiscall **)(_DWORD *))(*v11 + 8))(v11) )
    (*(void (__thiscall **)(_DWORD *))(*v11 + 12))(v11);
  sub_4307D0(v10[5], (int)v11);
  return 0;
}

// ===== sub_43E990 @ 0x0043E990..0x0043EAAD =====
int __userpurge sub_43E990@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  _DWORD *v10; // edi
  _DWORD *v11; // esi
  unsigned int v12; // eax

  v10 = dword_56674C;
  v11 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v11 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v11 + 8))(v11) )
    (*(void (__thiscall **)(_DWORD *))(*v11 + 12))(v11);
  v12 = sub_427110(a8, a7, v11, a2, a3, a4, a5, a6, a9, a10);
  if ( v12 > 0x80000007 )
  {
    switch ( v12 )
    {
      case 0x80000008:
        return 6;
      case 0x80000009:
        return 7;
      case 0x8000000A:
        return 2;
    }
    return a10;
  }
  if ( v12 == -2147483641 )
    return 5;
  if ( v12 > 0x80000005 )
    return 4;
  if ( v12 == -2147483643 )
    return 3;
  if ( v12 )
  {
    if ( v12 == -2147483647 )
      return 1;
    return a10;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v11 + 8))(v11) )
    (*(void (__thiscall **)(_DWORD *))(*v11 + 12))(v11);
  sub_4307D0(v10[5], (int)v11);
  return 0;
}

// ===== sub_43EAB0 @ 0x0043EAB0..0x0043EB9E =====
int __userpurge sub_43EAB0@<eax>(
        int a1@<eax>,
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
        int a17)
{
  _DWORD *v17; // edi
  _DWORD *v18; // esi
  unsigned int v19; // eax

  v17 = dword_56674C;
  v18 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v18 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v18 + 8))(v18) )
    (*(void (__thiscall **)(_DWORD *))(*v18 + 12))(v18);
  v19 = sub_427170(v18, a2, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
  if ( v19 > 0x80000003 )
  {
    if ( v19 == -2147483644 )
      return 8;
    return a17;
  }
  if ( v19 == -2147483645 )
    return 9;
  if ( v19 )
  {
    if ( v19 == -2147483647 || v19 == -2147483646 )
      return 1;
    return a17;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v18 + 8))(v18) )
    (*(void (__thiscall **)(_DWORD *))(*v18 + 12))(v18);
  sub_4307D0(v17[5], (int)v18);
  return 0;
}

// ===== sub_43EBA0 @ 0x0043EBA0..0x0043EC9A =====
int __userpurge sub_43EBA0@<eax>(
        int a1@<eax>,
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
        int a19,
        int a20)
{
  _DWORD *v20; // edi
  _DWORD *v21; // esi
  unsigned int v22; // eax

  v20 = dword_56674C;
  v21 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v21 )
    return 255;
  if ( (*(int (__thiscall **)(_DWORD *))(*v21 + 8))(v21) )
    (*(void (__thiscall **)(_DWORD *))(*v21 + 12))(v21);
  v22 = sub_427220(v21, a2, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
  if ( v22 > 0x80000003 )
  {
    if ( v22 == -2147483644 )
      return 8;
    return a20;
  }
  if ( v22 == -2147483645 )
    return 9;
  if ( v22 )
  {
    if ( v22 == -2147483647 || v22 == -2147483646 )
      return 1;
    return a20;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v21 + 8))(v21) )
    (*(void (__thiscall **)(_DWORD *))(*v21 + 12))(v21);
  sub_4307D0(v20[5], (int)v21);
  return 0;
}

// ===== sub_43ECA0 @ 0x0043ECA0..0x0043ED19 =====
int __stdcall sub_43ECA0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_43E5D0(a1, (int)dword_56674C);
  v3 = v2;
  if ( !v2 )
    return 255;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2);
  if ( v4 )
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
  v5 = sub_4272F0(v3, a2);
  if ( v5 )
  {
    if ( v5 == -2147483647 )
      return 1;
    else
      return a1;
  }
  else
  {
    if ( v4 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 12))(v3);
    return 0;
  }
}

// ===== sub_43ED20 @ 0x0043ED20..0x0043ED96 =====
int __stdcall sub_43ED20(int a1, int a2)
{
  _DWORD *v2; // edi
  int v3; // eax

  v2 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v2 )
    return 255;
  v3 = sub_427F80(v2, a2);
  if ( v3 )
  {
    if ( v3 == -2147483647 )
    {
      return 1;
    }
    else if ( v3 == -2147483638 )
    {
      return 2;
    }
    else
    {
      return a1;
    }
  }
  else
  {
    if ( (*(int (__thiscall **)(_DWORD *))(*v2 + 8))(v2) )
      (*(void (__thiscall **)(_DWORD *))(*v2 + 12))(v2);
    return 0;
  }
}

// ===== sub_43EDA0 @ 0x0043EDA0..0x0043EE62 =====
int __userpurge sub_43EDA0@<eax>(int a1@<eax>, int a2)
{
  void *v2; // edi
  _DWORD *v3; // esi
  int v4; // eax
  unsigned int v5; // eax

  v2 = dword_56674C;
  v3 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( !v3 )
    return 255;
  v4 = sub_43E5D0(a2, (int)v2);
  if ( v3 == (_DWORD *)v4 || !v4 && a2 )
    return 11;
  v5 = sub_428CD0(v3, v4);
  if ( v5 > 0x8000000C )
  {
    if ( v5 == -2147483635 )
      return 14;
    if ( v5 == -2147483634 )
      return 15;
    return a2;
  }
  if ( v5 == -2147483636 )
    return 13;
  if ( v5 )
  {
    if ( v5 == -2147483637 )
      return 13;
    return a2;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v3 + 8))(v3) )
    (*(void (__thiscall **)(_DWORD *))(*v3 + 12))(v3);
  return 0;
}

// ===== sub_43EE70 @ 0x0043EE70..0x0043EED8 =====
BOOL __stdcall sub_43EE70(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_43E5D0(a1, (int)dword_56674C);
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

// ===== sub_43EEE0 @ 0x0043EEE0..0x0043EF3E =====
int __stdcall sub_43EEE0(int a1, int a2, int a3)
{
  _DWORD *v3; // eax

  v3 = (_DWORD *)sub_43E5D0(a1, (int)dword_56674C);
  if ( v3 )
    return sub_428C00(v3) != 0 ? 0 : 10;
  else
    return 255;
}

// ===== sub_43EF40 @ 0x0043EF40..0x0043EF82 =====
_DWORD *__stdcall sub_43EF40(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 537;
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
  a1[545] = 0;
  a1[546] = 0;
  return result;
}

// ===== sub_43EF90 @ 0x0043EF90..0x0043F047 =====
int sub_43EF90()
{
  int *v0; // esi
  int result; // eax
  int v2; // edi
  _DWORD *i; // eax
  _DWORD *v4; // eax
  void *v5; // ecx
  _DWORD *v6; // eax

  v0 = (int *)dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 545) < 8 )
  {
    v2 = 0;
    for ( i = (char *)dword_56674C + 2148; *i; ++v2 )
      ++i;
    v4 = operator new(0x14Cu);
    if ( v4 )
    {
      v5 = (void *)v0[546];
      v0[546] = (int)v5 + 1;
      v6 = sub_420A30(v5, v4);
    }
    else
    {
      v6 = 0;
    }
    v0[v2 + 537] = (int)v6;
    sub_4306F0(v0[5], (int)v6);
    ++v0[545];
    return v2 - 1879048192;
  }
  return result;
}

// ===== sub_43F050 @ 0x0043F050..0x0043F083 =====
int __userpurge sub_43F050@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0x90000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2148);
  else
    return v2;
}

// ===== sub_43F090 @ 0x0043F090..0x0043F108 =====
BOOL __stdcall sub_43F090(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_43F050(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 537];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 537] = 0;
    --v1[545];
  }
  return v2 != 0;
}

// ===== sub_43F110 @ 0x0043F110..0x0043F1CA =====
int __userpurge sub_43F110@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD *v7; // ebx
  _DWORD *v8; // esi
  unsigned int v9; // eax

  v7 = dword_56674C;
  v8 = (_DWORD *)sub_43F050(a1, (int)dword_56674C);
  if ( !v8 )
    return 255;
  sub_420C60(v8, a3, a6, a7);
  v9 = sub_420CA0(v8, a4, a5);
  if ( v9 > 0x80000002 )
  {
    if ( v9 == -2147483645 )
      return 3;
    return a7;
  }
  if ( v9 == -2147483646 )
    return 2;
  if ( v9 )
  {
    if ( v9 == -2147483647 )
      return 1;
    return a7;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v8 + 8))(v8) )
    (*(void (__thiscall **)(_DWORD *))(*v8 + 12))(v8);
  sub_4307D0(v7[5], (int)v8);
  return 0;
}

// ===== sub_43F1D0 @ 0x0043F1D0..0x0043F238 =====
BOOL __stdcall sub_43F1D0(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_43F050(a1, (int)dword_56674C);
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

// ===== sub_43F240 @ 0x0043F240..0x0043F282 =====
_DWORD *__stdcall sub_43F240(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 547;
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
  a1[555] = 0;
  a1[556] = 0;
  return result;
}

// ===== sub_43F290 @ 0x0043F290..0x0043F347 =====
int sub_43F290()
{
  int *v0; // esi
  int result; // eax
  int v2; // edi
  _DWORD *i; // eax
  _DWORD *v4; // eax
  void *v5; // ecx
  _DWORD *v6; // eax

  v0 = (int *)dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 555) < 8 )
  {
    v2 = 0;
    for ( i = (char *)dword_56674C + 2188; *i; ++v2 )
      ++i;
    v4 = operator new(0x1C8u);
    if ( v4 )
    {
      v5 = (void *)v0[556];
      v0[556] = (int)v5 + 1;
      v6 = sub_41FCA0(v5, v4);
    }
    else
    {
      v6 = 0;
    }
    v0[v2 + 547] = (int)v6;
    sub_4306F0(v0[5], (int)v6);
    ++v0[555];
    return v2 - 1862270976;
  }
  return result;
}

// ===== sub_43F350 @ 0x0043F350..0x0043F383 =====
int __userpurge sub_43F350@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0x91000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2188);
  else
    return v2;
}

// ===== sub_43F390 @ 0x0043F390..0x0043F407 =====
BOOL __stdcall sub_43F390(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_43F350(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      sub_430D10(v1[5]);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 547];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 547] = 0;
    --v1[555];
  }
  return v2 != 0;
}

// ===== sub_43F410 @ 0x0043F410..0x0043F4D4 =====
int __userpurge sub_43F410@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *v6; // ebx
  _DWORD *v7; // esi
  unsigned int v8; // eax

  v6 = dword_56674C;
  v7 = (_DWORD *)sub_43F350(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  v8 = sub_420110(v7, a2, a3, a4, a5, a6);
  if ( v8 > 0x80000002 )
  {
    if ( v8 == -2147483645 )
      return 3;
    if ( v8 == -2147483644 )
      return 4;
    return a6;
  }
  if ( v8 == -2147483646 )
    return 2;
  if ( v8 )
  {
    if ( v8 == -2147483647 )
      return 1;
    return a6;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v7 + 8))(v7) )
    sub_430D10(v6[5]);
  sub_4307D0(v6[5], (int)v7);
  return 0;
}

// ===== sub_43F4E0 @ 0x0043F4E0..0x0043F55C =====
int __userpurge sub_43F4E0@<eax>(int a1@<eax>, unsigned int a2, int a3, int a4)
{
  _DWORD *v4; // ebx
  _DWORD *v5; // eax
  int v6; // esi
  int v7; // eax

  v4 = dword_56674C;
  v5 = (_DWORD *)sub_43F350(a1, (int)dword_56674C);
  v6 = (int)v5;
  if ( !v5 )
    return 255;
  v7 = sub_420250(v5, a2, a3, a4);
  if ( v7 )
  {
    if ( v7 == -2147483643 )
      return 5;
    else
      return a4;
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v6 + 8))(v6) )
      sub_430D10(v4[5]);
    sub_4307D0(v4[5], v6);
    return 0;
  }
}

// ===== sub_43F560 @ 0x0043F560..0x0043F637 =====
int __userpurge sub_43F560@<eax>(int a1@<eax>, int a2, int a3, unsigned int a4, int a5, int a6)
{
  _DWORD *v6; // ebx
  _DWORD *v7; // esi
  unsigned int v8; // eax

  v6 = dword_56674C;
  v7 = (_DWORD *)sub_43F350(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  v8 = sub_4202A0(v7, a2, a3, a4, a5, a6);
  if ( v8 > 0x80000006 )
  {
    if ( v8 == -2147483641 )
      return 7;
    if ( v8 == -2147483640 )
      return 8;
    return a6;
  }
  if ( v8 == -2147483642 )
    return 6;
  if ( v8 )
  {
    if ( v8 == -2147483647 )
      return 1;
    if ( v8 == -2147483645 )
      return 3;
    return a6;
  }
  if ( (*(int (__thiscall **)(_DWORD *))(*v7 + 8))(v7) )
    sub_430D10(v6[5]);
  sub_4307D0(v6[5], (int)v7);
  return 0;
}

// ===== sub_43F640 @ 0x0043F640..0x0043F6CD =====
int __userpurge sub_43F640@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  _DWORD *v9; // edi
  _DWORD *v10; // eax
  int v11; // esi
  int v12; // eax

  v9 = dword_56674C;
  v10 = (_DWORD *)sub_43F350(a1, (int)dword_56674C);
  v11 = (int)v10;
  if ( !v10 )
    return 255;
  v12 = sub_420410(v10, a2, a3, a4, a5, a6, a7, a8, a9);
  if ( v12 )
  {
    if ( v12 == -2147483639 )
      return 9;
    else
      return a9;
  }
  else
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v11 + 8))(v11) )
      sub_430D10(v9[5]);
    sub_4307D0(v9[5], v11);
    return 0;
  }
}

// ===== sub_43F6D0 @ 0x0043F6D0..0x0043F726 =====
int __userpurge sub_43F6D0@<eax>(int a1@<eax>, int a2, int a3)
{
  _DWORD *v3; // edi
  _DWORD *v4; // eax
  int v5; // esi

  v3 = dword_56674C;
  v4 = (_DWORD *)sub_43F350(a1, (int)dword_56674C);
  v5 = (int)v4;
  if ( !v4 )
    return 255;
  sub_4204D0(v4, a2, a3);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5) )
    sub_430D10(v3[5]);
  sub_4307D0(v3[5], v5);
  return 0;
}

// ===== sub_43F730 @ 0x0043F730..0x0043F798 =====
BOOL __userpurge sub_43F730@<eax>(int a1@<eax>, int a2)
{
  _DWORD *v2; // ebx
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = dword_56674C;
  v3 = sub_43F350(a1, (int)dword_56674C);
  if ( !v3 )
    return v3 != 0;
  v4 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 8))(v3);
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
    sub_430D10(v2[5]);
  return v3 != 0;
}

// ===== sub_43F7A0 @ 0x0043F7A0..0x0043F7E2 =====
_DWORD *__stdcall sub_43F7A0(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 557;
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
  a1[565] = 0;
  a1[566] = 0;
  return result;
}

// ===== sub_43F7F0 @ 0x0043F7F0..0x0043F8A7 =====
int sub_43F7F0()
{
  int *v0; // esi
  int result; // eax
  int v2; // edi
  _DWORD *i; // eax
  _DWORD *v4; // eax
  void *v5; // ecx
  _DWORD *v6; // eax

  v0 = (int *)dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 565) < 8 )
  {
    v2 = 0;
    for ( i = (char *)dword_56674C + 2228; *i; ++v2 )
      ++i;
    v4 = operator new(0x1BCu);
    if ( v4 )
    {
      v5 = (void *)v0[566];
      v0[566] = (int)v5 + 1;
      v6 = sub_423920(v5, v4);
    }
    else
    {
      v6 = 0;
    }
    v0[v2 + 557] = (int)v6;
    sub_4306F0(v0[5], (int)v6);
    ++v0[565];
    return v2 - 1610612736;
  }
  return result;
}

// ===== sub_43F8B0 @ 0x0043F8B0..0x0043F8E3 =====
int __userpurge sub_43F8B0@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xA0000000 && (v3 = sub_443260(0), v3 < 8) )
    return *(_DWORD *)(a2 + 4 * v3 + 2228);
  else
    return v2;
}

// ===== sub_43F8F0 @ 0x0043F8F0..0x0043F968 =====
BOOL __stdcall sub_43F8F0(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_43F8B0(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 557];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 557] = 0;
    --v1[565];
  }
  return v2 != 0;
}

// ===== sub_43F970 @ 0x0043F970..0x0043F9D8 =====
BOOL __stdcall sub_43F970(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_43F8B0(a1, (int)dword_56674C);
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

// ===== sub_43F9E0 @ 0x0043F9E0..0x0043FA6B =====
int __userpurge sub_43F9E0@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7)
{
  _DWORD *v7; // edi
  int v8; // esi

  v7 = dword_56674C;
  v8 = sub_43F8B0(a1, (int)dword_56674C);
  if ( !v8 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 8))(v8) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 12))(v8);
  if ( !sub_423AF0(a4, v8, a2, a3, a5, a6, a7) )
    return 1;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 8))(v8) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v8 + 12))(v8);
  sub_4307D0(v7[5], v8);
  return 0;
}

// ===== sub_43FA70 @ 0x0043FA70..0x0043FB06 =====
int __stdcall sub_43FA70(int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
  int v5; // eax
  int v6; // edi
  int v7; // esi
  int v8; // eax

  v5 = sub_43F8B0(a1, (int)dword_56674C);
  v6 = v5;
  if ( !v5 )
    return 255;
  v7 = (*(int (__thiscall **)(int))(*(_DWORD *)v5 + 8))(v5);
  if ( v7 )
    (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 12))(v6);
  v8 = sub_423B40(v6, a2, a3, a4, a5);
  if ( v8 )
  {
    if ( v8 == -2147483646 )
    {
      return 2;
    }
    else if ( v8 == -2147483645 )
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
    if ( v7 )
      (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 12))(v6);
    return 0;
  }
}

// ===== sub_43FB10 @ 0x0043FB10..0x0043FB4D =====
int __stdcall sub_43FB10(int a1, int a2, int a3, void *Src)
{
  _DWORD *v4; // eax

  v4 = (_DWORD *)sub_43F8B0(a1, (int)dword_56674C);
  if ( v4 )
    return sub_423CE0(v4, a2, a3, Src) != 0 ? 0 : 4;
  else
    return 255;
}

// ===== sub_43FB50 @ 0x0043FB50..0x0043FBB5 =====
int __stdcall sub_43FB50(int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, int a6)
{
  int v6; // esi

  v6 = sub_43F8B0(a1, (int)dword_56674C);
  if ( !v6 )
    return 255;
  if ( !sub_423D70(a3, v6, a2, a4, a5, a6) )
    return 5;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v6 + 8))(v6) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 12))(v6);
  return 0;
}

// ===== sub_43FBC0 @ 0x0043FBC0..0x0043FBF7 =====
int __stdcall sub_43FBC0(int a1, int a2)
{
  _DWORD *v2; // eax

  v2 = (_DWORD *)sub_43F8B0(a1, (int)dword_56674C);
  if ( v2 )
    return sub_423F10(v2, a2) != 0 ? 0 : 6;
  else
    return 255;
}

// ===== sub_43FC00 @ 0x0043FC00..0x0043FC42 =====
_DWORD *__stdcall sub_43FC00(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax

  v1 = a1 + 567;
  v2 = 4;
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
  a1[571] = 0;
  a1[572] = 0;
  return result;
}

// ===== sub_43FC50 @ 0x0043FC50..0x0043FD2B =====
int __stdcall sub_43FC50(unsigned int a1, int a2, int a3, int a4, int a5, int a6)
{
  int *v6; // esi
  int result; // eax
  int v8; // ebx
  _DWORD *i; // eax
  _DWORD *v10; // eax
  int v11; // ecx
  _DWORD *v12; // eax

  v6 = (int *)dword_56674C;
  result = 0;
  if ( *((int *)dword_56674C + 571) < 4 )
  {
    v8 = 0;
    for ( i = (char *)dword_56674C + 2268; *i; ++v8 )
      ++i;
    v10 = operator new(0x17Cu);
    if ( v10 )
    {
      v11 = v6[572];
      v6[572] = v11 + 1;
      v12 = sub_421780(v11, a2, v10, a1, a3, a4, a5, a6);
    }
    else
    {
      v12 = 0;
    }
    v6[v8 + 567] = (int)v12;
    sub_4306F0(v6[5], (int)v12);
    ++v6[571];
    return v8 - 1593835520;
  }
  return result;
}

// ===== sub_43FD30 @ 0x0043FD30..0x0043FD63 =====
int __userpurge sub_43FD30@<eax>(int a1@<eax>, int a2)
{
  int v2; // ecx
  unsigned int v3; // eax

  v2 = 0;
  if ( (a1 & 0xFF000000) == 0xA1000000 && (v3 = sub_443260(0), v3 < 4) )
    return *(_DWORD *)(a2 + 4 * v3 + 2268);
  else
    return v2;
}

// ===== sub_43FD70 @ 0x0043FD70..0x0043FDE8 =====
BOOL __stdcall sub_43FD70(int a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  int v3; // ecx
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v1 = dword_56674C;
  v2 = sub_43FD30(a1, (int)dword_56674C);
  if ( v2 )
  {
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 8))(v2) )
      (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 12))(v2);
    sub_430770(v2, v1[5]);
    v4 = sub_443260(v3);
    v5 = (void (__thiscall ***)(_DWORD, int))v1[v4 + 567];
    if ( v5 )
      (**v5)(v5, 1);
    v1[v4 + 567] = 0;
    --v1[571];
  }
  return v2 != 0;
}

// ===== sub_43FDF0 @ 0x0043FDF0..0x0043FE54 =====
int __stdcall sub_43FDF0(int *a1, int a2, int a3, int a4, int a5)
{
  _DWORD *v5; // eax
  int v6; // eax

  v5 = (_DWORD *)sub_43FD30(a2, (int)dword_56674C);
  if ( !v5 )
    return 255;
  v6 = sub_422920(v5, a1, a3, a4, a5);
  if ( !v6 )
    return 0;
  if ( v6 == 14 )
    return 14;
  return v6 != -2147483635 ? 254 : 13;
}

// ===== sub_43FE60 @ 0x0043FE60..0x0043FEC8 =====
BOOL __stdcall sub_43FE60(int a1, int a2)
{
  int v2; // eax
  int v3; // esi
  int v4; // edi
  int v5; // eax

  v2 = sub_43FD30(a1, (int)dword_56674C);
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

// ===== sub_43FED0 @ 0x0043FED0..0x0043FF46 =====
int __userpurge sub_43FED0@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  _DWORD *v6; // edi
  int v7; // esi

  v6 = dword_56674C;
  v7 = sub_43FD30(a1, (int)dword_56674C);
  if ( !v7 )
    return 255;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  sub_422AF0(a6, v7, a2, a3, a4, a5);
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)v7 + 8))(v7) )
    (*(void (__thiscall **)(int))(*(_DWORD *)v7 + 12))(v7);
  sub_4307D0(v6[5], v7);
  return 0;
}

// ===== sub_43FF50 @ 0x0043FF50..0x0043FFF9 =====
int __stdcall sub_43FF50(int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, int a6)
{
  _DWORD *v6; // esi
  unsigned int v7; // eax

  v6 = (_DWORD *)sub_43FD30(a1, (int)dword_56674C);
  if ( !v6 )
    return 255;
  v7 = sub_422730(a3, a2, v6, a4, a5, a6);
  if ( v7 <= 0x8000000D )
  {
    switch ( v7 )
    {
      case 0x8000000D:
        return 13;
      case 0u:
        return 0;
      case 0x8000000B:
        return 11;
      case 0x8000000C:
        return 12;
    }
    return 254;
  }
  if ( v7 == -2147483630 )
    return 18;
  if ( v7 != -2147483629 )
    return 254;
  return 19;
}
