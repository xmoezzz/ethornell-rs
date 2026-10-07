#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4A8010 @ 0x004A8010..0x004A8094 =====
int __thiscall sub_4A8010(int this, size_t Size)
{
  sub_4A7EB0((_DWORD *)this);
  *(_DWORD *)this = &CWaveBoxModel::`vftable';
  sub_4A4A40((char *)(this + 44));
  *(_DWORD *)(this + 32) = operator new(Size);
  *(_DWORD *)(this + 36) = Size;
  *(_DWORD *)(this + 40) = 0;
  *(_DWORD *)(this + 28) = 0;
  return this;
}

// ===== sub_4A80A0 @ 0x004A80A0..0x004A81A7 =====
unsigned int __thiscall sub_4A80A0(_DWORD *this, int a2, unsigned int a3)
{
  unsigned int v4; // ebx
  unsigned int v5; // edi
  _DWORD v7[5]; // [esp+0h] [ebp-30h] BYREF
  unsigned int v8; // [esp+14h] [ebp-1Ch]
  unsigned int v9; // [esp+18h] [ebp-18h]
  unsigned int v10; // [esp+1Ch] [ebp-14h]
  _DWORD *v11; // [esp+20h] [ebp-10h]
  int v12; // [esp+2Ch] [ebp-4h]

  v11 = v7;
  v7[4] = this;
  v9 = (*(int (__thiscall **)(_DWORD *))(*this + 12))(this);
  sub_4A4A80((int)(this + 11));
  v12 = 0;
  v4 = 0;
  v8 = 0;
  while ( v4 < a3 && this[7] < v9 )
  {
    v10 = (*(int (__thiscall **)(_DWORD *))(*this + 12))(this) - this[7];
    if ( a3 - v4 <= v10 )
      v10 = a3 - v4;
    v5 = (unsigned int)(*(int (__thiscall **)(_DWORD, unsigned int, unsigned int))(*(_DWORD *)this[10] + 8))(
                         this[10],
                         a2 + v4 * this[5],
                         v10 * this[5])
       / this[5];
    v4 += v5;
    v8 = v4;
    this[7] += v5;
    if ( this[7] == (*(int (__thiscall **)(_DWORD *))(*this + 12))(this) )
    {
      if ( !(*(int (__thiscall **)(_DWORD *))(*this + 36))(this) )
        break;
      (*(void (__thiscall **)(_DWORD *))(*this + 40))(this);
    }
    else if ( v5 < v10 )
    {
      break;
    }
  }
  sub_4A4A90((int)(this + 11));
  return v4;
}

// ===== sub_4A81B0 @ 0x004A81B0..0x004A81ED =====
int __thiscall sub_4A81B0(_DWORD **this, int a2, int a3)
{
  if ( a2 == 1 )
    ((void (__thiscall *)(_DWORD **))(*this)[11])(this);
  if ( this[1] )
    return (*(int (__thiscall **)(_DWORD *, int, int))(*this[1] + 20))(this[1], a2, a3);
  else
    return 0;
}

// ===== sub_4A81F0 @ 0x004A81F0..0x004A81F8 =====
void __thiscall sub_4A81F0(_DWORD *this)
{
  this[7] = 0;
}

// ===== sub_4A8200 @ 0x004A8200..0x004A8277 =====
int __thiscall sub_4A8200(int this)
{
  *(_DWORD *)this = &CWaveBoxModel::`vftable';
  operator delete(*(void **)(this + 32));
  *(_DWORD *)(this + 32) = 0;
  sub_4A81F0((_DWORD *)this);
  sub_4A4A60((char *)(this + 44));
  return sub_4A7ED0((_DWORD *)this);
}

// ===== sub_4A8280 @ 0x004A8280..0x004A82A1 =====
void *__thiscall sub_4A8280(void *this, char a2)
{
  sub_4A8200((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A82B0 @ 0x004A82B0..0x004A8330 =====
_DWORD *__thiscall sub_4A82B0(_DWORD *this)
{
  sub_4A8010((int)this, 0x100000u);
  *this = &CBurikoWaveBoxModel::`vftable';
  memset(this + 20, 0, 0x40u);
  this[36] = 0;
  this[38] = 0;
  this[40] = 0;
  this[39] = 0;
  return this;
}

// ===== sub_4A8330 @ 0x004A8330..0x004A84A9 =====
int __thiscall sub_4A8330(_DWORD *this, int a2, double a3)
{
  int v4; // eax
  int v6; // [esp+14h] [ebp-40h] BYREF
  int v7; // [esp+24h] [ebp-30h] BYREF
  char v8; // [esp+28h] [ebp-2Ch]
  int v9; // [esp+30h] [ebp-24h] BYREF
  int v10; // [esp+34h] [ebp-20h] BYREF
  int v11; // [esp+38h] [ebp-1Ch] BYREF
  _DWORD pExceptionObject[6]; // [esp+3Ch] [ebp-18h] BYREF

  pExceptionObject[2] = &v6;
  pExceptionObject[5] = 0;
  if ( (unsigned int)(*(int (__thiscall **)(int))(*(_DWORD *)a2 + 12))(a2) < 0x40 )
  {
    pExceptionObject[0] = 268435459;
    _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  (*(void (__thiscall **)(int, _DWORD *, int))(*(_DWORD *)a2 + 8))(a2, this + 20, 64);
  v7 = 0;
  v8 = 0;
  v7 = this[21];
  if ( strcmp((const char *)&v7, "bw  ") )
  {
    v11 = 285212673;
    _CxxThrowException(&v11, (_ThrowInfo *)&_TI1K);
  }
  if ( !(*(int (__thiscall **)(_DWORD *, _DWORD *))(*this + 56))(this, this + 20) )
  {
    v10 = 285212676;
    _CxxThrowException(&v10, (_ThrowInfo *)&_TI1K);
  }
  (*(void (__thiscall **)(_DWORD *))(*this + 28))(this);
  this[37] = 0;
  sub_4A7FB0(this + 2, 0x10u, this[25], this[24]);
  v4 = (*(int (__thiscall **)(_DWORD *, int, _DWORD, _DWORD))(*this + 60))(this, a2, LODWORD(a3), HIDWORD(a3));
  if ( v4 )
  {
    v9 = v4;
    _CxxThrowException(&v9, (_ThrowInfo *)&_TI1K);
  }
  this[38] = 1;
  this[39] = 0;
  this[40] = 0;
  return 0;
}

// ===== sub_4A84B0 @ 0x004A84B0..0x004A84B4 =====
int __thiscall sub_4A84B0(_DWORD *this)
{
  return this[23];
}

// ===== sub_4A84C0 @ 0x004A84C0..0x004A84C4 =====
int __thiscall sub_4A84C0(_DWORD *this)
{
  return this[26];
}

// ===== sub_4A84D0 @ 0x004A84D0..0x004A84D7 =====
int __thiscall sub_4A84D0(_DWORD *this)
{
  return this[38];
}

// ===== sub_4A84E0 @ 0x004A84E0..0x004A8500 =====
int __thiscall sub_4A84E0(_DWORD *this)
{
  int v1; // edi
  bool v2; // cc
  int result; // eax

  v1 = this[39];
  v2 = this[40] <= GetTickCount();
  result = v1 - 1;
  if ( v2 )
    return v1;
  return result;
}

// ===== sub_4A8500 @ 0x004A8500..0x004A8513 =====
int __thiscall sub_4A8500(_DWORD *this, int a2)
{
  return this[38] != 0 ? -30 : -3;
}

// ===== sub_4A8520 @ 0x004A8520..0x004A8533 =====
int __thiscall sub_4A8520(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[26] = a2;
  this[27] = a3;
  return result;
}

// ===== sub_4A8540 @ 0x004A8540..0x004A8599 =====
int __thiscall sub_4A8540(_DWORD *this)
{
  *this = &CBurikoWaveBoxModel::`vftable';
  sub_4A8D30();
  return sub_4A8200((int)this);
}

// ===== sub_4A85A0 @ 0x004A85A0..0x004A85C1 =====
_DWORD *__thiscall sub_4A85A0(_DWORD *this, char a2)
{
  sub_4A8540(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A85D0 @ 0x004A85D0..0x004A8691 =====
int __thiscall sub_4A85D0(_DWORD *this, void (__thiscall ***a2)(void *, int), double a3)
{
  _DWORD *v4; // eax
  _DWORD *v5; // esi
  void (__thiscall *v6)(_DWORD *, _DWORD *, int); // edx
  int v8; // [esp+4h] [ebp-20h]

  sub_4A7B60((int)this, (int)a2, a3);
  v4 = operator new(0x20u);
  v5 = 0;
  if ( v4 )
    v5 = sub_4A5200(v4, 4 * this[22]);
  (*(void (__thiscall **)(_DWORD *, int))(*v5 + 16))(v5, 3);
  v6 = *(void (__thiscall **)(_DWORD *, _DWORD *, int))(*this + 48);
  v8 = this[23];
  this[36] = a2;
  v6(this, v5 + 2, v8);
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*v5 + 20))(v5, 0);
  this[36] = v5;
  this[10] = v5;
  if ( a2 )
    (**a2)(a2, 1);
  return 0;
}

// ===== sub_4A86A0 @ 0x004A86A0..0x004A86C5 =====
int __thiscall sub_4A86A0(_DWORD *this)
{
  int v2; // edx
  int v3; // ecx

  v2 = this[27];
  v3 = this[36];
  this[7] = v2;
  this[37] = v2;
  return (*(int (__thiscall **)(int, int))(*(_DWORD *)v3 + 20))(v3, v2 * this[5]);
}

// ===== sub_4A86D0 @ 0x004A86D0..0x004A870E =====
int __thiscall sub_4A86D0(_DWORD *this, int a2)
{
  if ( !this[38] )
    return -3;
  sub_4AAC00(this + 20, this[36]);
  return sub_4AAC20(a2);
}

// ===== sub_4A8710 @ 0x004A8710..0x004A8769 =====
int __thiscall sub_4A8710(void **this)
{
  *this = &CStaticBurikoWaveBoxADPCM4::`vftable';
  sub_4A8D30();
  return sub_4A7C80(this);
}

// ===== sub_4A8770 @ 0x004A8770..0x004A87E7 =====
_DWORD *__thiscall sub_4A8770(_DWORD *this)
{
  sub_4A82B0(this);
  *this = &CBurikoWaveBoxHFADPCM8Model::`vftable';
  sub_445330(this + 50);
  this[46] = 0;
  this[48] = 0;
  return this;
}

// ===== sub_4A87F0 @ 0x004A87F0..0x004A88B3 =====
int __thiscall sub_4A87F0(int this, int a2, double a3)
{
  unsigned int v4; // eax

  v4 = *(_DWORD *)(this + 36);
  *(double *)(this + 72) = a3;
  *(_DWORD *)(this + 188) = v4 >> 1;
  *(_DWORD *)(this + 184) = operator new(v4 >> 1);
  *(_DWORD *)(this + 196) = 1024;
  *(_DWORD *)(this + 192) = operator new(0x400u);
  (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a2 + 8))(a2, this + 1232, 1032);
  *(_DWORD *)(this + 2264) = (*(int (__thiscall **)(int, _DWORD, _DWORD))(*(_DWORD *)a2 + 8))(
                               a2,
                               *(_DWORD *)(this + 192),
                               *(_DWORD *)(this + 196));
  sub_4AACF0(this + 1240);
  sub_4AAD10(*(_DWORD *)(this + 192), 0);
  *(_DWORD *)(this + 168) = 0;
  *(_DWORD *)(this + 176) = 0;
  *(_DWORD *)(this + 172) = 127;
  *(_DWORD *)(this + 180) = 127;
  return 0;
}

// ===== sub_4A88C0 @ 0x004A88C0..0x004A88F4 =====
void __thiscall sub_4A88C0(int this)
{
  void *v2; // [esp-8h] [ebp-Ch]

  operator delete(*(void **)(this + 184));
  v2 = *(void **)(this + 192);
  *(_DWORD *)(this + 184) = 0;
  operator delete(v2);
  *(_DWORD *)(this + 192) = 0;
}

// ===== sub_4A8900 @ 0x004A8900..0x004A8913 =====
BOOL __stdcall sub_4A8900(int a1)
{
  return *(_DWORD *)(a1 + 48) == 2;
}

// ===== sub_4A8920 @ 0x004A8920..0x004A897C =====
int __thiscall sub_4A8920(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax
  int v6; // ebx
  unsigned int v7; // edi
  unsigned int i; // [esp+1Ch] [ebp+10h]

  result = a4 * this[25];
  v6 = 0;
  v7 = 0;
  for ( i = result; v7 < i; ++v7 )
  {
    result = sub_4AAB30(*(unsigned __int8 *)(v7 + a3), &this[2 * v6 + 42], this[35]);
    *(_WORD *)(a2 + 2 * v7) = result;
    if ( this[25] == 2 )
      v6 ^= 1u;
  }
  return result;
}

// ===== sub_4A8980 @ 0x004A8980..0x004A8A7B =====
void __thiscall sub_4A8980(int this, int a2, unsigned int a3)
{
  unsigned int v3; // ebx
  int v5; // edi
  unsigned int v6; // eax
  char *v7; // ecx
  char v8; // di
  unsigned int v9; // eax
  int v10; // ecx
  int v11; // ecx
  int v12; // eax
  int v13; // [esp-8h] [ebp-18h]
  int v14; // [esp+8h] [ebp-8h]
  BOOL v15; // [esp+Ch] [ebp-4h]

  v3 = 0;
  v15 = 0;
  v14 = 0;
  if ( a3 )
  {
    v5 = this + 200;
    do
    {
      if ( (unsigned int)(8 * *(_DWORD *)(this + 2264) - sub_4AAD30(v5)) < 0x100 && !v15 )
      {
        v6 = sub_4AAD30(v5);
        v7 = *(char **)(this + 192);
        v8 = v6;
        v6 >>= 3;
        *(_DWORD *)(this + 2264) -= v6;
        memcpy_0(v7, &v7[v6], *(_DWORD *)(this + 2264));
        v9 = (*(int (__thiscall **)(_DWORD, int, int))(**(_DWORD **)(this + 144) + 8))(
               *(_DWORD *)(this + 144),
               *(_DWORD *)(this + 2264) + *(_DWORD *)(this + 192),
               1024 - *(_DWORD *)(this + 2264));
        v10 = *(_DWORD *)(this + 2264);
        v15 = v9 < 1024 - v10;
        v11 = v9 + v10;
        v12 = *(_DWORD *)(this + 192);
        v13 = v8 & 7;
        *(_DWORD *)(this + 2264) = v11;
        v5 = this + 200;
        sub_4AAD10(v12, v13);
        v3 = v14;
      }
      *(_BYTE *)(v3 + a2) = sub_4AADB0(v5);
      v14 = ++v3;
    }
    while ( v3 < a3 );
  }
}

// ===== sub_4A8A80 @ 0x004A8A80..0x004A8AE8 =====
int __thiscall sub_4A8A80(_DWORD *this)
{
  *this = &CBurikoWaveBoxHFADPCM8Model::`vftable';
  sub_4A88C0((int)this);
  nullsub_1();
  return sub_4A8540(this);
}

// ===== sub_4A8AF0 @ 0x004A8AF0..0x004A8BFC =====
unsigned int __userpurge sub_4A8AF0@<eax>(double *a1@<ecx>, int a2@<edi>, int a3, unsigned int a4)
{
  unsigned int v5; // eax
  unsigned int v7; // eax
  int v8; // ebx
  unsigned int v9; // ecx
  int v10; // edi
  int v11; // eax
  unsigned int v12; // edi
  int v13; // eax
  unsigned int result; // eax
  unsigned int i; // [esp+8h] [ebp-14h]
  unsigned int v17; // [esp+10h] [ebp-Ch]
  unsigned int v18; // [esp+14h] [ebp-8h]
  unsigned int v19; // [esp+18h] [ebp-4h]
  unsigned int v20; // [esp+28h] [ebp+Ch]

  v5 = *((_DWORD *)a1 + 23) - *((_DWORD *)a1 + 37);
  v20 = v5;
  if ( a4 <= v5 )
    v20 = a4;
  v7 = *((_DWORD *)a1 + 47) / *((_DWORD *)a1 + 25);
  v8 = *((_DWORD *)a1 + 8);
  v9 = 0;
  v19 = 0;
  v17 = v7;
  if ( v20 )
  {
    while ( 1 )
    {
      v10 = v20 - v9;
      if ( v7 <= v20 - v9 )
      {
        v18 = v7;
        v10 = v7;
      }
      else
      {
        v18 = v20 - v9;
      }
      (*(void (__thiscall **)(double *, _DWORD, int, int))(*(_DWORD *)a1 + 64))(
        a1,
        *((_DWORD *)a1 + 46),
        v10 * *((_DWORD *)a1 + 25),
        a2);
      sub_4A8920(a1, v8, *((_DWORD *)a1 + 46), v10);
      v11 = v10 * *((_DWORD *)a1 + 4);
      v12 = 0;
      for ( i = v11; v12 < i; ++v12 )
      {
        v13 = (int)((double)*(__int16 *)(v8 + 2 * v12) * a1[9]);
        if ( v13 >= -32768 )
        {
          if ( v13 > 0x7FFF )
            LOWORD(v13) = 0x7FFF;
        }
        else
        {
          LOWORD(v13) = 0x8000;
        }
        *(_WORD *)(v8 + 2 * v12) = v13;
      }
      a2 = v18 * *((_DWORD *)a1 + 5);
      (*(void (__thiscall **)(int, int))(*(_DWORD *)a3 + 8))(a3, v8);
      result = v18 + v19;
      v19 = result;
      if ( result >= v20 )
        break;
      v9 = result;
      v7 = v17;
    }
    *((_DWORD *)a1 + 37) += result;
  }
  else
  {
    *((_DWORD *)a1 + 37) = *((_DWORD *)a1 + 37);
    return 0;
  }
  return result;
}

// ===== sub_4A8C00 @ 0x004A8C00..0x004A8C21 =====
_DWORD *__thiscall sub_4A8C00(_DWORD *this, char a2)
{
  sub_4A8A80(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A8C30 @ 0x004A8C30..0x004A8CF3 =====
int __thiscall sub_4A8C30(_DWORD *this, void (__thiscall ***a2)(_DWORD, int), double a3)
{
  _DWORD *v4; // eax
  _DWORD *v5; // edi
  void (__thiscall *v6)(_DWORD *, _DWORD *, int); // edx
  int v8; // [esp+4h] [ebp-20h]

  sub_4A87F0((int)this, (int)a2, a3);
  v4 = operator new(0x20u);
  v5 = 0;
  if ( v4 )
    v5 = sub_4A5200(v4, 2 * this[23] * this[25]);
  (*(void (__thiscall **)(_DWORD *, int))(*v5 + 16))(v5, 3);
  v6 = *(void (__thiscall **)(_DWORD *, _DWORD *, int))(*this + 48);
  v8 = this[23];
  this[36] = a2;
  v6(this, v5 + 2, v8);
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*v5 + 20))(v5, 0);
  this[36] = v5;
  this[10] = v5;
  if ( a2 )
    (**a2)(a2, 1);
  return 0;
}

// ===== sub_4A8D00 @ 0x004A8D00..0x004A8D25 =====
int __thiscall sub_4A8D00(_DWORD *this)
{
  int v2; // edx
  int v3; // ecx

  v2 = this[27];
  v3 = this[36];
  this[7] = v2;
  this[37] = v2;
  return (*(int (__thiscall **)(int, int))(*(_DWORD *)v3 + 20))(v3, v2 * this[23]);
}

// ===== sub_4A8D30 @ 0x004A8D30..0x004A8D51 =====
int __thiscall sub_4A8D30(_DWORD *this)
{
  int (__thiscall ***v2)(_DWORD, int); // ecx
  int result; // eax

  v2 = (int (__thiscall ***)(_DWORD, int))this[36];
  if ( v2 )
    result = (**v2)(v2, 1);
  this[36] = 0;
  return result;
}

// ===== sub_4A8D60 @ 0x004A8D60..0x004A8D9E =====
int __thiscall sub_4A8D60(_DWORD *this, int a2)
{
  if ( !this[38] )
    return -3;
  sub_4AAC00(this + 20, this[36]);
  return sub_4AAC20(a2);
}

// ===== sub_4A8DA0 @ 0x004A8DA0..0x004A8DF9 =====
int __thiscall sub_4A8DA0(_DWORD *this)
{
  *this = &CStaticBurikoWaveBoxHFADPCM8::`vftable';
  sub_4A8D30(this);
  return sub_4A8A80(this);
}

// ===== sub_4A8E00 @ 0x004A8E00..0x004A8E58 =====
_DWORD *__thiscall sub_4A8E00(_DWORD *this)
{
  sub_4A82B0(this);
  *this = &CBurikoWaveBoxOGGModel::`vftable';
  this[222] = 0;
  return this;
}

// ===== sub_4A8E60 @ 0x004A8E60..0x004A8F08 =====
int __thiscall sub_4A8E60(int this, int a2, double a3)
{
  _BYTE v5[720]; // [esp+0h] [ebp-2D4h] BYREF

  *(double *)(this + 72) = a3;
  if ( (int)sub_4BAFB0(this, v5, 0, 0, off_4FB5AC[0], off_4FB5B0, off_4FB5B4, off_4FB5B8[0]) < 0 )
    return 0x10000000;
  qmemcpy((void *)(this + 168), v5, 0x2D0u);
  *(_DWORD *)(this + 888) = 1;
  return 0;
}

// ===== sub_4A8F10 @ 0x004A8F10..0x004A8F37 =====
int __thiscall sub_4A8F10(_DWORD *this)
{
  int result; // eax

  if ( this[222] )
  {
    result = sub_4BAEB0(this + 42);
    this[222] = 0;
  }
  return result;
}

// ===== sub_4A8F40 @ 0x004A8F40..0x004A9067 =====
int __thiscall sub_4A8F40(int this, int a2, unsigned int a3)
{
  unsigned int v4; // eax
  unsigned int v6; // edi
  unsigned int v7; // esi
  unsigned int v8; // ecx
  unsigned int i; // esi
  int v10; // eax
  unsigned int j; // edi
  int v12; // eax
  int result; // eax
  unsigned int v14; // [esp+14h] [ebp-10h]
  unsigned int v15; // [esp+18h] [ebp-Ch]
  int v16; // [esp+1Ch] [ebp-8h]
  unsigned int v17; // [esp+20h] [ebp-4h]
  int v18; // [esp+30h] [ebp+Ch]

  v4 = *(_DWORD *)(this + 92) - *(_DWORD *)(this + 148);
  v6 = *(_DWORD *)(this + 20);
  if ( a3 <= v4 )
  {
    v18 = a3 * v6;
    v7 = a3 * v6;
  }
  else
  {
    v7 = v4 * v6;
    v18 = v4 * v6;
  }
  v16 = *(_DWORD *)(this + 32);
  v15 = 0;
  v8 = *(_DWORD *)(this + 36) - *(_DWORD *)(this + 36) % v6;
  v14 = v8;
  if ( v7 )
  {
    while ( 1 )
    {
      v17 = v8;
      if ( v7 <= v8 )
        v17 = v7;
      for ( i = 0; i < v17; i += v10 )
      {
        v10 = sub_4BCAC0(this + 168, i + *(_DWORD *)(this + 32), v17 - i, 0, 2, 1, 0);
        if ( v10 <= 0 )
          break;
      }
      for ( j = 0; j < i >> 1; ++j )
      {
        v12 = (int)((double)*(__int16 *)(v16 + 2 * j) * *(double *)(this + 72));
        if ( v12 >= -32768 )
        {
          if ( v12 > 0x7FFF )
            LOWORD(v12) = 0x7FFF;
        }
        else
        {
          LOWORD(v12) = 0x8000;
        }
        *(_WORD *)(v16 + 2 * j) = v12;
      }
      (*(void (__thiscall **)(int, int, unsigned int))(*(_DWORD *)a2 + 8))(a2, v16, i);
      v18 -= i;
      v15 += i;
      if ( v17 != i || !v18 )
        break;
      v8 = v14;
      v7 = v18;
    }
  }
  result = v15 / *(_DWORD *)(this + 20);
  *(_DWORD *)(this + 148) += result;
  return result;
}

// ===== sub_4A9070 @ 0x004A9070..0x004A9083 =====
BOOL __stdcall sub_4A9070(int a1)
{
  return *(_DWORD *)(a1 + 48) == 3;
}

// ===== sub_4A9090 @ 0x004A9090..0x004A90B1 =====
int __cdecl sub_4A9090(int a1, int a2, int a3, int a4)
{
  return (*(int (__thiscall **)(_DWORD, int, int))(**(_DWORD **)(a4 + 144) + 8))(*(_DWORD *)(a4 + 144), a1, a3 * a2);
}

// ===== sub_4A90C0 @ 0x004A90C0..0x004A9137 =====
int __cdecl sub_4A90C0(int a1, int a2, int a3, int a4)
{
  int v5; // edi
  int (*v6)(void); // edx
  int v7; // esi

  switch ( a4 )
  {
    case 0:
      return -(a2 + 64 != (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(a1 + 144) + 20))(
                            *(_DWORD *)(a1 + 144),
                            a2 + 64));
    case 1:
      v5 = a1;
      v6 = *(int (**)(void))(**(_DWORD **)(a1 + 144) + 24);
      break;
    case 2:
      v5 = a1;
      v6 = *(int (**)(void))(**(_DWORD **)(a1 + 144) + 28);
      break;
    default:
      return -1;
  }
  v7 = a2 + v6();
  return -(v7 != (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(v5 + 144) + 20))(*(_DWORD *)(v5 + 144), v7));
}

// ===== sub_4A9140 @ 0x004A9140..0x004A9158 =====
int __cdecl sub_4A9140(int a1)
{
  return (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 144) + 24))(*(_DWORD *)(a1 + 144)) - 64;
}

// ===== sub_4A9160 @ 0x004A9160..0x004A91B9 =====
int __thiscall sub_4A9160(_DWORD *this)
{
  *this = &CBurikoWaveBoxOGGModel::`vftable';
  sub_4A8F10(this);
  return sub_4A8540(this);
}

// ===== sub_4A91C0 @ 0x004A91C0..0x004A91E1 =====
_DWORD *__thiscall sub_4A91C0(_DWORD *this, char a2)
{
  sub_4A9160(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A91F0 @ 0x004A91F0..0x004A92B6 =====
int __thiscall sub_4A91F0(_DWORD *this, void (__thiscall ***a2)(_DWORD, int), double a3)
{
  _DWORD *v4; // eax
  _DWORD *v5; // edi

  v4 = operator new(0x20u);
  v5 = 0;
  if ( v4 )
    v5 = sub_4A5200(v4, 2 * this[23] * this[25]);
  (*(void (__thiscall **)(_DWORD *, int))(*v5 + 16))(v5, 3);
  this[36] = a2;
  sub_4A8E60((int)this, (int)a2, a3);
  (*(void (__thiscall **)(_DWORD *, _DWORD *, _DWORD))(*this + 48))(this, v5 + 2, this[23]);
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*v5 + 20))(v5, 0);
  this[36] = v5;
  this[10] = v5;
  if ( a2 )
    (**a2)(a2, 1);
  return 0;
}

// ===== sub_4A92C0 @ 0x004A92C0..0x004A92E7 =====
int __thiscall sub_4A92C0(_DWORD *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx

  v2 = (void (__thiscall ***)(_DWORD, int))this[36];
  if ( v2 )
    (**v2)(v2, 1);
  this[36] = 0;
  return sub_4A8F10(this);
}

// ===== sub_4A92F0 @ 0x004A92F0..0x004A932E =====
int __thiscall sub_4A92F0(_DWORD *this, int a2)
{
  if ( !this[38] )
    return -3;
  sub_4AAC00(this + 20, this[36]);
  return sub_4AAC20(a2);
}

// ===== sub_4A9330 @ 0x004A9330..0x004A9389 =====
int __thiscall sub_4A9330(_DWORD *this)
{
  *this = &CStaticBurikoWaveBoxOGG::`vftable';
  sub_4A92C0(this);
  return sub_4A9160(this);
}

// ===== sub_4A9390 @ 0x004A9390..0x004A93E9 =====
int __thiscall sub_4A9390(_DWORD *this)
{
  *this = &CBurikoWaveBoxPCM16Model::`vftable';
  sub_4A8D30(this);
  return sub_4A8540(this);
}

// ===== sub_4A93F0 @ 0x004A93F0..0x004A94F7 =====
int __thiscall sub_4A93F0(int this, int a2, unsigned int a3)
{
  unsigned int v4; // eax
  unsigned int v6; // ebx
  unsigned int v7; // esi
  unsigned int v8; // ecx
  unsigned int v9; // ebx
  unsigned int i; // esi
  int v11; // eax
  int result; // eax
  unsigned int v13; // [esp+14h] [ebp-10h]
  unsigned int v14; // [esp+18h] [ebp-Ch]
  unsigned int v15; // [esp+1Ch] [ebp-8h]
  int v16; // [esp+20h] [ebp-4h]
  int v17; // [esp+30h] [ebp+Ch]

  v4 = *(_DWORD *)(this + 92) - *(_DWORD *)(this + 148);
  v6 = *(_DWORD *)(this + 20);
  if ( a3 <= v4 )
  {
    v17 = a3 * v6;
    v7 = a3 * v6;
  }
  else
  {
    v7 = v4 * v6;
    v17 = v4 * v6;
  }
  v16 = *(_DWORD *)(this + 32);
  v14 = 0;
  v8 = *(_DWORD *)(this + 36) - *(_DWORD *)(this + 36) % v6;
  v13 = v8;
  if ( v7 )
  {
    while ( 1 )
    {
      v15 = v8;
      if ( v7 <= v8 )
        v15 = v7;
      v9 = (*(int (__thiscall **)(_DWORD, _DWORD, unsigned int))(**(_DWORD **)(this + 144) + 8))(
             *(_DWORD *)(this + 144),
             *(_DWORD *)(this + 32),
             v15);
      for ( i = 0; i < v9 >> 1; ++i )
      {
        v11 = (int)((double)*(__int16 *)(v16 + 2 * i) * *(double *)(this + 72));
        if ( v11 >= -32768 )
        {
          if ( v11 > 0x7FFF )
            LOWORD(v11) = 0x7FFF;
        }
        else
        {
          LOWORD(v11) = 0x8000;
        }
        *(_WORD *)(v16 + 2 * i) = v11;
      }
      (*(void (__thiscall **)(int, int, unsigned int))(*(_DWORD *)a2 + 8))(a2, v16, v9);
      v17 -= v9;
      v14 += v9;
      if ( v15 != v9 || !v17 )
        break;
      v8 = v13;
      v7 = v17;
    }
  }
  result = v14 / *(_DWORD *)(this + 20);
  *(_DWORD *)(this + 148) += result;
  return result;
}

// ===== sub_4A9500 @ 0x004A9500..0x004A9513 =====
BOOL __stdcall sub_4A9500(int a1)
{
  return *(_DWORD *)(a1 + 48) == 1;
}

// ===== sub_4A9520 @ 0x004A9520..0x004A95D4 =====
int __thiscall sub_4A9520(void *this, void (__thiscall ***a2)(_DWORD, int), double a3)
{
  _DWORD *v4; // eax
  _DWORD *v5; // esi
  void (__thiscall *v6)(void *, _DWORD *, int); // edx
  int v8; // [esp-4h] [ebp-20h]

  *((double *)this + 9) = a3;
  v4 = operator new(0x20u);
  v5 = 0;
  if ( v4 )
    v5 = sub_4A5200(v4, *((_DWORD *)this + 22));
  (*(void (__thiscall **)(_DWORD *, int))(*v5 + 16))(v5, 3);
  v6 = *(void (__thiscall **)(void *, _DWORD *, int))(*(_DWORD *)this + 48);
  v8 = *((_DWORD *)this + 23);
  *((_DWORD *)this + 36) = a2;
  v6(this, v5 + 2, v8);
  (*(void (__thiscall **)(_DWORD *, _DWORD))(*v5 + 20))(v5, 0);
  *((_DWORD *)this + 36) = v5;
  *((_DWORD *)this + 10) = v5;
  if ( a2 )
    (**a2)(a2, 1);
  return 0;
}

// ===== sub_4A95E0 @ 0x004A95E0..0x004A9608 =====
int __thiscall sub_4A95E0(_DWORD *this)
{
  int v2; // edx
  int v3; // ecx

  v2 = this[27];
  v3 = this[36];
  this[7] = v2;
  this[37] = v2;
  return (*(int (__thiscall **)(int, unsigned int))(*(_DWORD *)v3 + 20))(v3, (unsigned int)(v2 * this[23]) >> 2);
}

// ===== sub_4A9610 @ 0x004A9610..0x004A9699 =====
int __thiscall sub_4A9610(int this)
{
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  v3[4] = this;
  sub_4A4A80(this + 44);
  v3[8] = 0;
  *(_DWORD *)(this + 28) = 0;
  *(_DWORD *)(this + 148) = 0;
  (*(void (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 144) + 20))(*(_DWORD *)(this + 144), 0);
  return sub_4A4A90(this + 44);
}

// ===== sub_4A96A0 @ 0x004A96A0..0x004A96DE =====
int __thiscall sub_4A96A0(_DWORD *this, int a2)
{
  if ( !this[38] )
    return -3;
  sub_4AAC00(this + 20, this[36]);
  return sub_4AAC20(a2);
}

// ===== sub_4A96E0 @ 0x004A96E0..0x004A9739 =====
int __thiscall sub_4A96E0(_DWORD *this)
{
  *this = &CStaticBurikoWaveBoxPCM16::`vftable';
  sub_4A8D30(this);
  return sub_4A9390(this);
}

// ===== sub_4A9740 @ 0x004A9740..0x004A97DE =====
int __thiscall sub_4A9740(_DWORD *this, void *a2, double a3)
{
  int (__thiscall ***v4)(_DWORD); // eax

  this[36] = a2;
  sub_4A8E60((int)this, (int)a2, a3);
  if ( operator new(0x4Cu) )
    v4 = (int (__thiscall ***)(_DWORD))sub_4AADD0(this, this + 224, this[24], this[5], 4);
  else
    v4 = 0;
  this[225] = v4;
  this[10] = (**v4)(v4);
  return 0;
}

// ===== sub_4A97E0 @ 0x004A97E0..0x004A9877 =====
int __thiscall sub_4A97E0(_DWORD *this)
{
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  v3[4] = this;
  sub_4A4A80((int)(this + 11));
  v3[8] = 0;
  this[7] = 0;
  this[37] = 0;
  sub_4BC490(this + 42, 0, 0);
  sub_4AAE70(v3[0], v3[1]);
  return sub_4A4A90((int)(this + 11));
}

// ===== sub_4A9880 @ 0x004A9880..0x004A98BA =====
int __thiscall sub_4A9880(_DWORD *this)
{
  DWORD TickCount; // eax
  int v3; // eax

  TickCount = GetTickCount();
  ++*(this - 185);
  *(this - 184) = TickCount + 4000;
  v3 = *(this - 197);
  *(this - 187) = v3;
  return sub_4BC490(this - 182, v3, 0);
}

// ===== sub_4A98C0 @ 0x004A98C0..0x004A992C =====
_DWORD *__thiscall sub_4A98C0(_DWORD *this)
{
  sub_4A8E00(this);
  this[224] = &CWaveStreamCtrlEventListener::`vftable';
  *this = &CStreamBurikoWaveBoxOGG::`vftable';
  this[224] = &CStreamBurikoWaveBoxOGG::`vftable';
  this[225] = 0;
  return this;
}

// ===== sub_4A9930 @ 0x004A9930..0x004A9970 =====
int __usercall sub_4A9930@<eax>(_DWORD *a1@<ecx>, int a2@<edi>)
{
  void *v3; // edi

  if ( a1[225] )
  {
    sub_4AAF60();
    v3 = (void *)a1[225];
    if ( v3 )
    {
      sub_4AB130(a2);
      operator delete(v3);
    }
    a1[225] = 0;
  }
  return sub_4A8F10(a1);
}

// ===== sub_4A9970 @ 0x004A9970..0x004A99D3 =====
int __usercall sub_4A9970@<eax>(_DWORD *a1@<ecx>, int a2@<edi>)
{
  *a1 = &CStreamBurikoWaveBoxOGG::`vftable';
  a1[224] = &CStreamBurikoWaveBoxOGG::`vftable';
  sub_4A9930(a1, a2);
  return sub_4A9160(a1);
}

// ===== sub_4A99E0 @ 0x004A99E0..0x004A9A01 =====
_DWORD *__userpurge sub_4A99E0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, char a3)
{
  sub_4A9970(a1, a2);
  if ( (a3 & 1) != 0 )
    operator delete(a1);
  return a1;
}

// ===== sub_4A9A10 @ 0x004A9A10..0x004A9AB2 =====
int __thiscall sub_4A9A10(_DWORD *this, void *a2, double a3)
{
  _DWORD *v5; // ecx
  int (__thiscall ***v6)(_DWORD); // eax
  void *v8; // [esp+28h] [ebp+8h]

  sub_4A87F0((int)this, (int)a2, a3);
  v8 = operator new(0x4Cu);
  v5 = 0;
  if ( v8 )
  {
    if ( this )
      v5 = this + 568;
    v6 = (int (__thiscall ***)(_DWORD))sub_4AADD0(this, v5, this[24], this[5], 4);
  }
  else
  {
    v6 = 0;
  }
  this[569] = v6;
  this[36] = a2;
  this[10] = (**v6)(v6);
  return 0;
}

// ===== sub_4A9AC0 @ 0x004A9AC0..0x004A9AC7 =====
int __thiscall sub_4A9AC0(_DWORD *this)
{
  int result; // eax

  result = this[27];
  this[7] = result;
  return result;
}

// ===== sub_4A9AD0 @ 0x004A9AD0..0x004A9BC8 =====
int __thiscall sub_4A9AD0(int this)
{
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  v3[4] = this;
  sub_4A4A80(this + 44);
  v3[8] = 0;
  *(_DWORD *)(this + 28) = 0;
  *(_DWORD *)(this + 148) = 0;
  *(_DWORD *)(this + 168) = 0;
  *(_DWORD *)(this + 176) = 0;
  *(_DWORD *)(this + 172) = 127;
  *(_DWORD *)(this + 180) = 127;
  (*(void (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 144) + 20))(
    *(_DWORD *)(this + 144),
    *(_DWORD *)(this + 80));
  (*(void (__thiscall **)(_DWORD, int, int))(**(_DWORD **)(this + 144) + 8))(*(_DWORD *)(this + 144), this + 1232, 1032);
  *(_DWORD *)(this + 2264) = (*(int (__thiscall **)(_DWORD, _DWORD, _DWORD))(**(_DWORD **)(this + 144) + 8))(
                               *(_DWORD *)(this + 144),
                               *(_DWORD *)(this + 192),
                               *(_DWORD *)(this + 196));
  sub_4AAD10(*(_DWORD *)(this + 192), 0);
  return sub_4A4A90(this + 44);
}

// ===== sub_4A9BD0 @ 0x004A9BD0..0x004A9C7E =====
int __thiscall sub_4A9BD0(_DWORD *this)
{
  DWORD TickCount; // eax
  int v3; // ecx
  int v4; // edx
  int v5; // eax
  int v6; // ecx
  unsigned int v7; // eax
  int v8; // eax
  int v10; // [esp-8h] [ebp-Ch]
  int v11; // [esp-4h] [ebp-8h]

  TickCount = GetTickCount();
  v3 = *(this - 540);
  v4 = *(this - 539);
  ++*(this - 529);
  *(this - 528) = TickCount + 4000;
  *(this - 531) = *(this - 541);
  v5 = *(this - 538);
  *(this - 526) = v3;
  v6 = *(this - 537);
  *(this - 525) = v4;
  *(this - 524) = v5;
  v7 = *(this - 534);
  *(this - 523) = v6;
  (*(void (__thiscall **)(_DWORD, unsigned int))(*(_DWORD *)*(this - 532) + 20))(*(this - 532), (v7 >> 3) + 1096);
  v8 = (*(int (__thiscall **)(_DWORD, _DWORD, _DWORD))(*(_DWORD *)*(this - 532) + 8))(
         *(this - 532),
         *(this - 520),
         *(this - 519));
  v11 = *(this - 534) & 7;
  v10 = *(this - 520);
  *(this - 2) = v8;
  return sub_4AAD10(v10, v11);
}

// ===== sub_4A9C80 @ 0x004A9C80..0x004A9CEC =====
_DWORD *__thiscall sub_4A9C80(_DWORD *this)
{
  sub_4A8770(this);
  this[568] = &CWaveStreamCtrlEventListener::`vftable';
  *this = &CStreamBurikoWaveBoxHFADPCM8::`vftable';
  this[568] = &CStreamBurikoWaveBoxHFADPCM8::`vftable';
  this[569] = 0;
  return this;
}

// ===== sub_4A9CF0 @ 0x004A9CF0..0x004A9D2A =====
void __usercall sub_4A9CF0(int a1@<ecx>, int a2@<edi>)
{
  void *v3; // edi

  if ( *(_DWORD *)(a1 + 2276) )
  {
    sub_4AAF60();
    v3 = *(void **)(a1 + 2276);
    if ( v3 )
    {
      sub_4AB130(a2);
      operator delete(v3);
    }
    *(_DWORD *)(a1 + 2276) = 0;
  }
}

// ===== sub_4A9D30 @ 0x004A9D30..0x004A9D93 =====
int __usercall sub_4A9D30@<eax>(_DWORD *a1@<ecx>, int a2@<edi>)
{
  *a1 = &CStreamBurikoWaveBoxHFADPCM8::`vftable';
  a1[568] = &CStreamBurikoWaveBoxHFADPCM8::`vftable';
  sub_4A9CF0((int)a1, a2);
  return sub_4A8A80(a1);
}

// ===== sub_4A9DA0 @ 0x004A9DA0..0x004A9DC1 =====
_DWORD *__userpurge sub_4A9DA0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, char a3)
{
  sub_4A9D30(a1, a2);
  if ( (a3 & 1) != 0 )
    operator delete(a1);
  return a1;
}

// ===== sub_4A9DD0 @ 0x004A9DD0..0x004A9E65 =====
int __thiscall sub_4A9DD0(int this, int a2, double a3)
{
  int (__thiscall ***v4)(_DWORD); // eax

  *(double *)(this + 72) = a3;
  if ( operator new(0x4Cu) )
    v4 = (int (__thiscall ***)(_DWORD))sub_4AADD0(this, this + 168, *(_DWORD *)(this + 96), *(_DWORD *)(this + 20), 4);
  else
    v4 = 0;
  *(_DWORD *)(this + 172) = v4;
  *(_DWORD *)(this + 144) = a2;
  *(_DWORD *)(this + 40) = (**v4)(v4);
  return 0;
}

// ===== sub_4A9E70 @ 0x004A9E70..0x004A9F07 =====
int __thiscall sub_4A9E70(int this)
{
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  v3[4] = this;
  sub_4A4A80(this + 44);
  v3[8] = 0;
  *(_DWORD *)(this + 28) = 0;
  *(_DWORD *)(this + 148) = 0;
  (*(void (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 144) + 20))(
    *(_DWORD *)(this + 144),
    *(_DWORD *)(this + 80));
  sub_4AAE70(v3[0], v3[1]);
  return sub_4A4A90(this + 44);
}

// ===== sub_4A9F10 @ 0x004A9F10..0x004A9F44 =====
int __thiscall sub_4A9F10(_DWORD *this)
{
  DWORD TickCount; // eax
  int v3; // edx
  int v4; // ecx
  int v5; // edx
  int result; // eax

  TickCount = GetTickCount();
  v3 = *(this - 15) * *(this - 37);
  v4 = *(this - 6);
  ++*(this - 3);
  v5 = *(this - 22) + v3;
  *(this - 2) = TickCount + 4000;
  result = (*(int (__thiscall **)(int, int))(*(_DWORD *)v4 + 20))(v4, v5);
  *(this - 5) = *(this - 15);
  return result;
}

// ===== sub_4A9F50 @ 0x004A9F50..0x004A9FC9 =====
_DWORD *__thiscall sub_4A9F50(_DWORD *this)
{
  sub_4A82B0(this);
  *this = &CBurikoWaveBoxPCM16Model::`vftable';
  this[42] = &CWaveStreamCtrlEventListener::`vftable';
  *this = &CStreamBurikoWaveBoxPCM16::`vftable';
  this[42] = &CStreamBurikoWaveBoxPCM16::`vftable';
  this[43] = 0;
  return this;
}

// ===== sub_4A9FD0 @ 0x004A9FD0..0x004AA00A =====
void __usercall sub_4A9FD0(int a1@<ecx>, int a2@<edi>)
{
  void *v3; // edi

  if ( *(_DWORD *)(a1 + 172) )
  {
    sub_4AAF60();
    v3 = *(void **)(a1 + 172);
    if ( v3 )
    {
      sub_4AB130(a2);
      operator delete(v3);
    }
    *(_DWORD *)(a1 + 172) = 0;
  }
}

// ===== sub_4AA010 @ 0x004AA010..0x004AA073 =====
int __usercall sub_4AA010@<eax>(_DWORD *a1@<ecx>, int a2@<edi>)
{
  *a1 = &CStreamBurikoWaveBoxPCM16::`vftable';
  a1[42] = &CStreamBurikoWaveBoxPCM16::`vftable';
  sub_4A9FD0((int)a1, a2);
  return sub_4A9390(a1);
}

// ===== sub_4AA080 @ 0x004AA080..0x004AA0A1 =====
_DWORD *__userpurge sub_4AA080@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, char a3)
{
  sub_4AA010(a1, a2);
  if ( (a3 & 1) != 0 )
    operator delete(a1);
  return a1;
}

// ===== sub_4AA0B0 @ 0x004AA0B0..0x004AA152 =====
int __thiscall sub_4AA0B0(_DWORD *this, int a2, double a3)
{
  _DWORD *v5; // ecx
  int (__thiscall ***v6)(_DWORD); // eax
  void *v8; // [esp+28h] [ebp+8h]

  sub_4A7B60((int)this, a2, a3);
  v8 = operator new(0x4Cu);
  v5 = 0;
  if ( v8 )
  {
    if ( this )
      v5 = this + 50;
    v6 = (int (__thiscall ***)(_DWORD))sub_4AADD0(this, v5, this[24], this[5], 4);
  }
  else
  {
    v6 = 0;
  }
  this[51] = v6;
  this[36] = a2;
  this[10] = (**v6)(v6);
  return 0;
}

// ===== sub_4AA160 @ 0x004AA160..0x004AA223 =====
int __thiscall sub_4AA160(int this)
{
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  v3[4] = this;
  sub_4A4A80(this + 44);
  v3[8] = 0;
  *(_DWORD *)(this + 28) = 0;
  *(_DWORD *)(this + 148) = 0;
  *(_DWORD *)(this + 168) = 0;
  *(_DWORD *)(this + 176) = 0;
  *(_DWORD *)(this + 172) = 127;
  *(_DWORD *)(this + 180) = 127;
  *(_WORD *)(this + 192) = 0;
  *(_DWORD *)(this + 196) = 0;
  (*(void (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 144) + 20))(
    *(_DWORD *)(this + 144),
    *(_DWORD *)(this + 80));
  sub_4AAE70(v3[0], v3[1]);
  return sub_4A4A90(this + 44);
}

// ===== sub_4AA230 @ 0x004AA230..0x004AA2C2 =====
int __thiscall sub_4AA230(_DWORD *this)
{
  DWORD TickCount; // eax
  int v3; // ebx
  int v4; // ecx
  int v5; // edx
  int v6; // eax
  int v7; // ecx
  _DWORD *v8; // edi
  int v9; // edx
  int v10; // ecx
  int result; // eax
  unsigned __int8 v12; // [esp+Fh] [ebp-1h] BYREF

  TickCount = GetTickCount();
  v3 = *(this - 45);
  v4 = *(this - 22);
  v5 = *(this - 21);
  ++*(this - 11);
  *(this - 10) = TickCount + 4000;
  v6 = *(this - 23);
  *(this - 8) = v4;
  v7 = *(this - 20);
  v8 = this - 8;
  v8[1] = v5;
  v9 = *(this - 19);
  v8[2] = v7;
  v10 = *(this - 14);
  *(this - 13) = v6;
  v8[3] = v9;
  result = (*(int (__thiscall **)(int, unsigned int))(*(_DWORD *)v10 + 20))(v10, ((unsigned int)(v6 * v3) >> 2) + 64);
  if ( *(this - 25) == 1 && (*(_BYTE *)(this - 13) & 1) != 0 )
  {
    (*(void (__thiscall **)(_DWORD, unsigned __int8 *, int))(*(_DWORD *)*(this - 14) + 8))(*(this - 14), &v12, 1);
    result = sub_4AAAB0(v12 >> 4, this - 8);
    *((_WORD *)this - 4) = result;
    *(this - 1) = 1;
  }
  return result;
}

// ===== sub_4AA2D0 @ 0x004AA2D0..0x004AA33C =====
_DWORD *__thiscall sub_4AA2D0(_DWORD *this)
{
  sub_4A7B00(this);
  this[50] = &CWaveStreamCtrlEventListener::`vftable';
  *this = &CStreamBurikoWaveBoxADPCM4::`vftable';
  this[50] = &CStreamBurikoWaveBoxADPCM4::`vftable';
  this[51] = 0;
  return this;
}

// ===== sub_4AA340 @ 0x004AA340..0x004AA37A =====
void __usercall sub_4AA340(int a1@<ecx>, int a2@<edi>)
{
  void *v3; // edi

  if ( *(_DWORD *)(a1 + 204) )
  {
    sub_4AAF60();
    v3 = *(void **)(a1 + 204);
    if ( v3 )
    {
      sub_4AB130(a2);
      operator delete(v3);
    }
    *(_DWORD *)(a1 + 204) = 0;
  }
}

// ===== sub_4AA380 @ 0x004AA380..0x004AA3E3 =====
int __usercall sub_4AA380@<eax>(int a1@<ecx>, int a2@<edi>)
{
  *(_DWORD *)a1 = &CStreamBurikoWaveBoxADPCM4::`vftable';
  *(_DWORD *)(a1 + 200) = &CStreamBurikoWaveBoxADPCM4::`vftable';
  sub_4AA340(a1, a2);
  return sub_4A7C80((void **)a1);
}

// ===== sub_4AA3F0 @ 0x004AA3F0..0x004AA411 =====
void *__userpurge sub_4AA3F0@<eax>(void *a1@<ecx>, int a2@<edi>, char a3)
{
  sub_4AA380((int)a1, a2);
  if ( (a3 & 1) != 0 )
    operator delete(a1);
  return a1;
}

// ===== sub_4AA420 @ 0x004AA420..0x004AA5E5 =====
int __thiscall sub_4AA420(_DWORD *this, int a2, int a3, double a4)
{
  int v5; // esi
  int v6; // edi
  int v7; // esi
  int v8; // eax
  int v10; // [esp+8h] [ebp-50h] BYREF
  _DWORD v11[2]; // [esp+18h] [ebp-40h]
  int v12; // [esp+20h] [ebp-38h] BYREF
  char v13; // [esp+24h] [ebp-34h]
  int v14; // [esp+2Ch] [ebp-2Ch] BYREF
  int v15; // [esp+30h] [ebp-28h] BYREF
  int v16; // [esp+34h] [ebp-24h]
  int v17; // [esp+38h] [ebp-20h] BYREF
  int pExceptionObject; // [esp+3Ch] [ebp-1Ch] BYREF
  int v19; // [esp+44h] [ebp-14h]
  int *v20; // [esp+48h] [ebp-10h]
  int v21; // [esp+54h] [ebp-4h]

  v20 = &v10;
  v5 = 0;
  v21 = 0;
  v11[0] = a2;
  v11[1] = a3;
  v19 = 0;
  while ( v5 < 2 )
  {
    v6 = v11[v5];
    if ( (unsigned int)(*(int (__thiscall **)(int))(*(_DWORD *)v6 + 12))(v6) < 0x40 )
    {
      pExceptionObject = 268435459;
      _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
    }
    v7 = v5 << 6;
    v16 = (int)this + v7 + 2352;
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v6 + 8))(v6, v16, 64);
    v12 = 0;
    v13 = 0;
    v12 = *(_DWORD *)((char *)this + v7 + 2356);
    if ( strcmp((const char *)&v12, "bw  ") )
    {
      v17 = 285212673;
      _CxxThrowException(&v17, (_ThrowInfo *)&_TI1K);
    }
    if ( !(*(int (__thiscall **)(_DWORD *, int))(*this + 56))(this, v16) )
    {
      v15 = 285212676;
      _CxxThrowException(&v15, (_ThrowInfo *)&_TI1K);
    }
    v5 = ++v19;
  }
  (*(void (__thiscall **)(_DWORD *))(*this + 28))(this);
  qmemcpy(this + 20, this + 588, 0x40u);
  this[37] = 0;
  sub_4A7FB0(this + 2, 0x10u, this[25], this[24]);
  this[39] = 0;
  this[40] = 0;
  v8 = (*(int (__thiscall **)(_DWORD *, int, int, _DWORD, _DWORD))(*this + 68))(this, a2, a3, LODWORD(a4), HIDWORD(a4));
  if ( v8 )
  {
    v14 = v8;
    _CxxThrowException(&v14, (_ThrowInfo *)&_TI1K);
  }
  this[38] = 1;
  return 0;
}

// ===== sub_4AA5F0 @ 0x004AA5F0..0x004AA5F8 =====
int __stdcall sub_4AA5F0(int a1, int a2, int a3)
{
  return 0x10000000;
}

// ===== sub_4AA600 @ 0x004AA600..0x004AA60D =====
int __thiscall sub_4AA600(_DWORD *this)
{
  return this[591] + this[607];
}

// ===== sub_4AA610 @ 0x004AA610..0x004AA625 =====
int __thiscall sub_4AA610(_DWORD *this)
{
  int result; // eax

  result = 1;
  if ( this[620] )
    return this[621];
  return result;
}

// ===== sub_4AA630 @ 0x004AA630..0x004AA728 =====
int __thiscall sub_4AA630(_DWORD *this, int a2, void *a3, double a4)
{
  int (__thiscall ***v5)(_DWORD); // eax

  this[36] = a3;
  sub_4A8E60((int)this, (int)a3, a4);
  this[227] = a3;
  qmemcpy(this + 408, this + 42, 0x2D0u);
  this[36] = a2;
  sub_4A8E60((int)this, a2, a4);
  qmemcpy(this + 228, this + 42, 0x2D0u);
  this[226] = a2;
  this[620] = 0;
  if ( operator new(0x4Cu) )
    v5 = (int (__thiscall ***)(_DWORD))sub_4AADD0(this, this + 224, this[24], this[5], 4);
  else
    v5 = 0;
  this[225] = v5;
  this[10] = (**v5)(v5);
  return 0;
}

// ===== sub_4AA730 @ 0x004AA730..0x004AA73D =====
int __thiscall sub_4AA730(_DWORD *this)
{
  int result; // eax

  result = this[620] - 1;
  if ( this[620] == 1 )
    this[7] = 0;
  return result;
}

// ===== sub_4AA740 @ 0x004AA740..0x004AA7B9 =====
_DWORD *__thiscall sub_4AA740(_DWORD *this, int a2)
{
  _DWORD *result; // eax
  int v3; // ecx

  result = this;
  v3 = this[620];
  if ( v3 != a2 )
  {
    qmemcpy(&result[180 * v3 + 228], result + 42, 0x2D0u);
    result[620] = a2;
    result[36] = result[a2 + 226];
    qmemcpy(result + 20, &result[16 * a2 + 588], 0x40u);
    qmemcpy(result + 42, &result[180 * a2 + 228], 0x2D0u);
  }
  return result;
}

// ===== sub_4AA7C0 @ 0x004AA7C0..0x004AA7D0 =====
int __thiscall sub_4AA7C0(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[621] = a2;
  return result;
}

// ===== sub_4AA7D0 @ 0x004AA7D0..0x004AA84E =====
_DWORD *__thiscall sub_4AA7D0(_DWORD *this)
{
  sub_4A8E00(this);
  this[224] = &CWaveStreamCtrlEventListener::`vftable';
  *this = &CStreamBurikoWaveBoxOGGFileExchange::`vftable';
  this[224] = &CStreamBurikoWaveBoxOGGFileExchange::`vftable';
  this[225] = 0;
  this[226] = 0;
  this[227] = 0;
  this[621] = 0;
  return this;
}

// ===== sub_4AA850 @ 0x004AA850..0x004AA912 =====
int __thiscall sub_4AA850(_DWORD *this)
{
  _DWORD v3[5]; // [esp+0h] [ebp-28h] BYREF
  _DWORD *v4; // [esp+14h] [ebp-14h]
  _DWORD *v5; // [esp+18h] [ebp-10h]
  int v6; // [esp+24h] [ebp-4h]

  v5 = v3;
  v3[4] = this;
  v4 = this + 11;
  sub_4A4A80((int)(this + 11));
  v6 = 0;
  this[7] = 0;
  this[37] = 0;
  this[39] = 0;
  this[40] = 0;
  sub_4AA740(this, 1);
  sub_4BC490(this + 42, 0, 0);
  sub_4AA740(this, 0);
  sub_4BC490(this + 42, 0, 0);
  sub_4AAE70(v3[0], v3[1]);
  return sub_4A4A90((int)v4);
}

// ===== sub_4AA920 @ 0x004AA920..0x004AA980 =====
_DWORD *__thiscall sub_4AA920(_DWORD *this)
{
  int v2; // eax
  _DWORD *result; // eax
  DWORD TickCount; // eax

  v2 = this[396];
  if ( v2 )
  {
    result = (_DWORD *)(v2 - 1);
    if ( !result )
    {
      TickCount = GetTickCount();
      ++*(this - 185);
      *(this - 184) = TickCount + 4000;
      *(this - 187) = 0;
      return (_DWORD *)sub_4BC490(this - 182, 0, 0);
    }
  }
  else
  {
    *(this - 187) = 0;
    return sub_4AA740(this - 224, 1);
  }
  return result;
}

// ===== sub_4AA980 @ 0x004AA980..0x004AAA0B =====
int __thiscall sub_4AA980(_DWORD *this)
{
  int v2; // ebx
  void *v3; // esi
  _DWORD *v4; // esi
  int result; // eax
  int v6; // [esp+0h] [ebp-Ch]

  v2 = 0;
  if ( this[225] )
  {
    sub_4AAF60();
    v3 = (void *)this[225];
    if ( v3 )
    {
      sub_4AB130(v6);
      operator delete(v3);
    }
    this[225] = 0;
  }
  v4 = this + 226;
  do
  {
    if ( *v4 )
    {
      sub_4AA740(this, v2);
      sub_4BAEB0(this + 42);
      if ( *v4 )
        (**(void (__thiscall ***)(_DWORD, int))*v4)(*v4, 1);
      *v4 = 0;
    }
    ++v2;
    ++v4;
  }
  while ( v2 < 2 );
  result = 0;
  this[222] = 0;
  this[36] = 0;
  return result;
}

// ===== sub_4AAA10 @ 0x004AAA10..0x004AAA73 =====
int __thiscall sub_4AAA10(_DWORD *this)
{
  *this = &CStreamBurikoWaveBoxOGGFileExchange::`vftable';
  this[224] = &CStreamBurikoWaveBoxOGGFileExchange::`vftable';
  sub_4AA980(this);
  return sub_4A9160(this);
}

// ===== sub_4AAA80 @ 0x004AAA80..0x004AAAA1 =====
_DWORD *__thiscall sub_4AAA80(_DWORD *this, char a2)
{
  sub_4AAA10(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4AAAB0 @ 0x004AAAB0..0x004AAB28 =====
int __cdecl sub_4AAAB0(unsigned int a1, int *a2)
{
  int v2; // edx
  int v3; // esi
  int result; // eax
  unsigned int v5; // ecx

  v2 = a1 & 7;
  v3 = a2[1];
  result = *a2 + ((v3 * (2 * v2 + 1)) >> 3) * (1 - ((a1 >> 2) & 2));
  v5 = (unsigned int)(v3 * dword_4FB6C4[v2]) >> 6;
  if ( v5 >= 0x7F )
  {
    if ( (unsigned int)(v3 * dword_4FB6C4[v2]) >> 6 > 0x6000 )
      v5 = 24576;
  }
  else
  {
    v5 = 127;
  }
  *a2 = result;
  a2[1] = v5;
  if ( result > 0x7FFF )
    return 0x7FFF;
  if ( result < -32768 )
    return -32768;
  return result;
}

// ===== sub_4AAB30 @ 0x004AAB30..0x004AABFE =====
__int16 __cdecl sub_4AAB30(char a1, signed int *a2, int a3)
{
  signed int v3; // ebx
  unsigned int v4; // esi
  unsigned int v5; // ecx
  signed int v6; // edi
  int v7; // eax
  int v8; // eax

  v3 = a2[1];
  v4 = 1 << ((unsigned int)(a3 + 1) >> 1);
  v5 = a1 & 0x7F;
  v6 = *a2 + (2 * (a1 >= 0) - 1) * (v3 * (2 * v5 + 1) / (2 * v4));
  if ( v5 >= v4 )
    v7 = v3 * (77 - (int)((double)(v5 - v4) * -25.6));
  else
    v7 = 57 * v3;
  v8 = v7 / 64;
  if ( v8 >= 127 )
  {
    if ( v8 > 24576 )
      v8 = 24576;
  }
  else
  {
    v8 = 127;
  }
  *a2 = v6;
  a2[1] = v8;
  if ( v6 > 0x7FFF )
    return 0x7FFF;
  if ( v6 >= -32768 )
    return v6;
  return 0x8000;
}

// ===== sub_4AAC00 @ 0x004AAC00..0x004AAC1F =====
_DWORD *__thiscall sub_4AAC00(_DWORD *this, const void *a2, int a3)
{
  _DWORD *result; // eax

  result = this;
  qmemcpy(this, a2, 0x40u);
  this[16] = a3;
  return result;
}

// ===== sub_4AAC20 @ 0x004AAC20..0x004AACE7 =====
int __thiscall sub_4AAC20(int this, unsigned int a2)
{
  __int16 *v3; // esi
  unsigned int v4; // edx
  unsigned int v6; // ecx
  unsigned int v7; // eax
  unsigned int v8; // edi
  unsigned int v9; // [esp+Ch] [ebp-8h]
  unsigned int v10; // [esp+1Ch] [ebp+8h]

  v3 = (__int16 *)sub_4A4EA0(*(_DWORD **)(this + 64));
  v4 = a2 / 0x3E8 * *(_DWORD *)(this + 16) + *(_DWORD *)(this + 16) * (a2 % 0x3E8) / 0x3E8;
  v9 = v4;
  if ( v4 > *(_DWORD *)(this + 12) )
    return -1;
  v6 = 0;
  v10 = 0;
  if ( v4 )
  {
    v7 = *(_DWORD *)(this + 20);
    do
    {
      v8 = 0;
      if ( v7 )
      {
        do
        {
          *v3 = (int)((double)*v3 * (double)v6 / (double)v4);
          v7 = *(_DWORD *)(this + 20);
          ++v8;
          ++v3;
        }
        while ( v8 < v7 );
        v4 = v9;
        v6 = v10;
      }
      v10 = ++v6;
    }
    while ( v6 < v4 );
  }
  return 0;
}

// ===== sub_4AACF0 @ 0x004AACF0..0x004AAD07 =====
void __thiscall sub_4AACF0(void *this, const void *a2)
{
  qmemcpy(this, a2, 0x400u);
}

// ===== sub_4AAD10 @ 0x004AAD10..0x004AAD29 =====
int __thiscall sub_4AAD10(_DWORD *this, int a2, int a3)
{
  int result; // eax

  result = a2;
  this[256] = a2;
  this[257] = a3;
  return result;
}

// ===== sub_4AAD30 @ 0x004AAD30..0x004AAD37 =====
int __thiscall sub_4AAD30(_DWORD *this)
{
  return this[257];
}

// ===== sub_4AAD40 @ 0x004AAD40..0x004AADA6 =====
int __stdcall sub_4AAD40(int a1, _DWORD *a2, int *a3)
{
  _BYTE *v3; // esi
  unsigned __int8 v4; // cl
  __int64 v5; // rax
  BOOL v6; // ebx

  v3 = (_BYTE *)((*a2 >> 3) + a1);
  v4 = 128 >> (*(_BYTE *)a2 & 7);
  v5 = (unsigned int)*a3;
  if ( *a3 >= 256 )
  {
    do
    {
      v6 = (v4 & *v3) != 0;
      v4 >>= 1;
      LODWORD(v5) = *((__int16 *)&a3[v5 - 255] + v6);
      if ( !v4 )
      {
        v4 = 0x80;
        ++v3;
      }
      ++HIDWORD(v5);
    }
    while ( (int)v5 >= 256 );
  }
  *a2 += HIDWORD(v5);
  return v5;
}

// ===== sub_4AADB0 @ 0x004AADB0..0x004AADC5 =====
int __thiscall sub_4AADB0(int *this)
{
  return sub_4AAD40(this[256], this + 257, this);
}

// ===== sub_4AADD0 @ 0x004AADD0..0x004AAE66 =====
char *__thiscall sub_4AADD0(char *this, int a2, int a3, int a4, int a5, int a6)
{
  *(_DWORD *)this = &CWaveStreamCtrl::`vftable';
  sub_4C6970(this + 16);
  sub_4A4A40(this + 48);
  *((_DWORD *)this + 11) = a3;
  *((_DWORD *)this + 9) = 0;
  *((_DWORD *)this + 10) = a2;
  unknown_libname_2((_DWORD *)this + 4, -1);
  *((_DWORD *)this + 2) = a4;
  *((_DWORD *)this + 1) = a5;
  *((_DWORD *)this + 3) = a6;
  return this;
}

// ===== sub_4AAE70 @ 0x004AAE70..0x004AAF51 =====
void __thiscall sub_4AAE70(int this)
{
  unsigned int v2; // ebx
  unsigned int v3; // eax
  unsigned int v4; // esi
  int v5; // eax
  int v6; // eax
  int v7; // eax
  _DWORD v8[5]; // [esp+0h] [ebp-2Ch] BYREF
  unsigned int v9; // [esp+14h] [ebp-18h]
  unsigned int v10; // [esp+18h] [ebp-14h]
  _DWORD *v11; // [esp+1Ch] [ebp-10h]
  int v12; // [esp+28h] [ebp-4h]

  v11 = v8;
  v8[4] = this;
  v2 = 0;
  if ( *(_DWORD *)(this + 36) )
  {
    sub_4A4A80(this + 48);
    v12 = 0;
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 36) + 20))(*(_DWORD *)(this + 36));
    v3 = (unsigned int)(*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 36) + 16))(*(_DWORD *)(this + 36))
       / *(_DWORD *)(this + 4);
    v9 = v3;
    v10 = 0;
    while ( v2 < v3 )
    {
      v4 = v3 - v2;
      v5 = *(_DWORD *)(this + 36);
      if ( v5 )
        v6 = v5 + 8;
      else
        v6 = 0;
      v7 = (*(int (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(this + 40) + 48))(
             *(_DWORD *)(this + 40),
             v6,
             v4);
      v2 += v7;
      v10 = v2;
      if ( v7 != v4 )
      {
        if ( !(*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 36))(*(_DWORD *)(this + 40)) )
          break;
        (***(void (__thiscall ****)(_DWORD))(this + 44))(*(_DWORD *)(this + 44));
      }
      v3 = v9;
    }
    sub_4A4A90(this + 48);
  }
}

// ===== sub_4AAF60 @ 0x004AAF60..0x004AAF85 =====
int __thiscall sub_4AAF60(_DWORD *this)
{
  int result; // eax
  int (__thiscall ***v3)(_DWORD, int); // ecx

  result = sub_4C69C0(0xFFFFFFFF);
  v3 = (int (__thiscall ***)(_DWORD, int))this[9];
  if ( v3 )
    result = (**v3)(v3, 1);
  this[9] = 0;
  return result;
}

// ===== sub_4AAF90 @ 0x004AAF90..0x004AB129 =====
int __cdecl sub_4AAF90(int a1, _DWORD *a2)
{
  unsigned int v2; // esi
  unsigned int v3; // ebx
  unsigned int v4; // ebx
  unsigned int v5; // esi
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v10; // [esp+0h] [ebp-30h] BYREF
  unsigned int v11; // [esp+10h] [ebp-20h]
  unsigned int v12; // [esp+14h] [ebp-1Ch]
  unsigned int v13; // [esp+18h] [ebp-18h]
  int v14; // [esp+1Ch] [ebp-14h]
  int *v15; // [esp+20h] [ebp-10h]
  int v16; // [esp+2Ch] [ebp-4h]

  v15 = &v10;
  v16 = 0;
  v2 = *(_DWORD *)(a1 + 8) >> 1;
  v13 = v2;
  v3 = v2 * *(_DWORD *)(a1 + 4);
  v11 = v3;
  v14 = 0;
  while ( !*a2 && !v14 )
  {
    sub_4A4A80(*(_DWORD *)(a1 + 40) + 44);
    LOBYTE(v16) = 1;
    sub_4A4A80(a1 + 48);
    LOBYTE(v16) = 2;
    while ( (*(int (__thiscall **)(int))(*(_DWORD *)(*(_DWORD *)(a1 + 36) + 8) + 12))(*(_DWORD *)(a1 + 36) + 8) > v3
         && !v14
         && !*a2 )
    {
      v4 = 0;
      v12 = 0;
      while ( v4 < v2 && !*a2 )
      {
        v5 = v2 - v4;
        v6 = *(_DWORD *)(a1 + 36);
        if ( v6 )
          v7 = v6 + 8;
        else
          v7 = 0;
        v8 = (*(int (__thiscall **)(_DWORD, int, unsigned int))(**(_DWORD **)(a1 + 40) + 48))(
               *(_DWORD *)(a1 + 40),
               v7,
               v5);
        v4 += v8;
        v12 = v4;
        if ( v8 != v5 )
        {
          if ( !(*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 36))(*(_DWORD *)(a1 + 40)) )
          {
            v14 = 1;
            v2 = v13;
            break;
          }
          (***(void (__thiscall ****)(_DWORD))(a1 + 44))(*(_DWORD *)(a1 + 44));
        }
        v2 = v13;
      }
      v3 = v11;
    }
    sub_4A4A90(a1 + 48);
    v16 = 1;
    sub_4A4A90(*(_DWORD *)(a1 + 40) + 44);
    v16 = 0;
    Sleep(0x32u);
  }
  return 0;
}

// ===== sub_4AB130 @ 0x004AB130..0x004AB196 =====
int __thiscall sub_4AB130(int this)
{
  *(_DWORD *)this = &CWaveStreamCtrl::`vftable';
  sub_4AAF60((_DWORD *)this);
  sub_4A4A60((char *)(this + 48));
  return sub_4C6A40(this + 16);
}

// ===== sub_4AB1A0 @ 0x004AB1A0..0x004AB245 =====
int __thiscall sub_4AB1A0(_DWORD *this)
{
  int v2; // eax

  if ( operator new(0x40u) )
    v2 = sub_4C6AE0(this[1] * this[2] * this[3]);
  else
    v2 = 0;
  this[9] = v2;
  sub_4AAE70((int)this);
  if ( sub_4C6A50((int)sub_4AAF90, (int)this, 15, 0, 0, 0) >= 0 )
    return this[9];
  else
    return 0;
}

// ===== sub_4AB245 @ 0x004AB245..0x004AB254 =====
void __thiscall sub_4AB245(void *this)
{
  if ( this != (void *)dword_4FB734 )
    __report_gsfailure();
}

// ===== _ceil @ 0x004AB260..0x004AB37D =====
double __cdecl ceil(double X)
{
  int v1; // eax
  bool v2; // zf
  __m128i v3; // xmm7
  __m128d v4; // xmm0
  int v5; // eax
  __m128i v6; // xmm2
  __m128i v7; // xmm1
  double v8; // xmm1_8
  double result; // st7
  __m128d v10; // xmm1
  __m128d v11; // xmm3
  __int64 v12; // xmm0_8
  char v13; // [esp+8h] [ebp-8h]

  if ( !dword_567C18 )
    goto _ceil;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v13 & 0x7F) == 127;
  if ( v2 )
  {
    v3 = _mm_loadl_epi64((const __m128i *)&X);
    v4 = (__m128d)_mm_srli_epi64(v3, 0x34u);
    v5 = _mm_cvtsi128_si32((__m128i)v4);
    v6 = _mm_sub_epi32((__m128i)xmmword_4DC350, (__m128i)_mm_and_pd(v4, (__m128d)xmmword_4DC370));
    v7 = _mm_srl_epi64(v3, v6);
    if ( (v5 & 0x800) != 0 )
    {
      if ( v5 >= 3071 )
      {
        *(_QWORD *)&v8 = v7.m128i_i64[0] << v6.m128i_i8[0];
        if ( v5 <= 3122 )
        {
          X = v8;
          return v8;
        }
        return X;
      }
      return -0.0;
    }
    else
    {
      v10 = (__m128d)_mm_sll_epi64(v7, v6);
      v11 = (__m128d)_mm_loadl_epi64((const __m128i *)&X);
      v12 = *(_OWORD *)&_mm_cmpnle_pd(v11, v10);
      if ( v5 < 1023 )
      {
        *(_QWORD *)&X = *(_OWORD *)&_mm_cmpnle_pd(v11, (__m128d)xmmword_4DC360) & 0x3FF0000000000000LL;
        return X;
      }
      else
      {
        if ( v5 > 1074 )
          return X;
        return v10.m128d_f64[0] + COERCE_DOUBLE(v12 & 0x3FF0000000000000LL);
      }
    }
  }
  else
  {
_ceil:
    _ceil_default(X);
  }
  return result;
}

// ===== ??3@YAXPAX@Z @ 0x004AB37D..0x004AB388 =====
void __cdecl operator delete(void *Block)
{
  free(Block);
}

// ===== ??_U@YAPAXI@Z @ 0x004AB388..0x004AB393 =====
void *__cdecl operator new[](size_t a1)
{
  return operator new(a1);
}

// ===== ??_V@YAXPAX@Z @ 0x004AB393..0x004AB39E =====
void __cdecl operator delete[](void *a1)
{
  operator delete(a1);
}

// ===== sub_4AB39E @ 0x004AB39E..0x004AB3AE =====
void __thiscall sub_4AB39E(struct type_info *this)
{
  *(_DWORD *)this = &type_info::`vftable';
  type_info::_Type_info_dtor(this);
}

// ===== sub_4AB3AE @ 0x004AB3AE..0x004AB3CF =====
struct type_info *__thiscall sub_4AB3AE(struct type_info *this, char a2)
{
  sub_4AB39E(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== ??8type_info@@QBE_NABV0@@Z @ 0x004AB3CF..0x004AB3EF =====
BOOL __thiscall type_info::operator==(const char *this, int a2)
{
  return strcmp((const char *)(a2 + 9), this + 9) == 0;
}

// ===== ??2@YAPAXI@Z @ 0x004AB3EF..0x004AB46F =====
void *__cdecl operator new(size_t Size)
{
  void *result; // eax
  _DWORD pExceptionObject[3]; // [esp+0h] [ebp-10h] BYREF
  char *v3; // [esp+Ch] [ebp-4h] BYREF

  while ( 1 )
  {
    result = malloc(Size);
    if ( result )
      break;
    if ( !_callnewh(Size) )
    {
      if ( (dword_509B5C & 1) == 0 )
      {
        dword_509B5C |= 1u;
        v3 = "bad allocation";
        std::exception::exception((std::exception *)dword_509B50, (const char *const *)&v3, 1);
        dword_509B50[0] = &std::bad_alloc::`vftable';
        atexit(sub_4DA084);
      }
      std::exception::exception((std::exception *)pExceptionObject, (const struct exception *)dword_509B50);
      pExceptionObject[0] = &std::bad_alloc::`vftable';
      _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI2_AVbad_alloc_std__);
    }
  }
  return result;
}

// ===== __onexit_nolock @ 0x004AB46F..0x004AB525 =====
PVOID __cdecl _onexit_nolock(PVOID Ptr)
{
  PVOID *v1; // ebx
  PVOID *v2; // eax
  PVOID *v3; // esi
  int v4; // edi
  size_t v5; // ebx
  int v6; // eax
  size_t v7; // eax
  char *v8; // eax
  PVOID *Block; // [esp+Ch] [ebp-4h]

  v1 = (PVOID *)DecodePointer(::Ptr);
  Block = v1;
  v2 = (PVOID *)DecodePointer(dword_567C08);
  v3 = v2;
  if ( v2 >= v1 )
  {
    v4 = (char *)v2 - (char *)v1;
    if ( (unsigned int)((char *)v2 - (char *)v1) < 0xFFFFFFFC )
    {
      v5 = _msize(v1);
      if ( v5 >= v4 + 4 )
      {
LABEL_11:
        *v3 = EncodePointer(Ptr);
        dword_567C08 = EncodePointer(v3 + 1);
        return Ptr;
      }
      v6 = 2048;
      if ( v5 < 0x800 )
        v6 = v5;
      v7 = v5 + v6;
      if ( v7 >= v5 && (v8 = (char *)_realloc_crt(Block, v7)) != 0
        || v5 + 16 >= v5 && (v8 = (char *)_realloc_crt(Block, v5 + 16)) != 0 )
      {
        v3 = (PVOID *)&v8[4 * (v4 >> 2)];
        ::Ptr = EncodePointer(v8);
        goto LABEL_11;
      }
    }
  }
  return 0;
}

// ===== ___onexitinit @ 0x004AB525..0x004AB556 =====
int __onexitinit()
{
  _DWORD *v0; // esi

  v0 = (_DWORD *)_calloc_crt(32, 4);
  Ptr = EncodePointer(v0);
  dword_567C08 = Ptr;
  if ( !v0 )
    return 24;
  *v0 = 0;
  return 0;
}

// ===== __onexit @ 0x004AB556..0x004AB592 =====
_onexit_t __cdecl _onexit(_onexit_t Func)
{
  int (__cdecl *v2)(); // [esp+10h] [ebp-1Ch]

  _lockexit();
  v2 = (int (__cdecl *)())_onexit_nolock(Func);
  _unlockexit(4896131);
  return v2;
}

// ===== _atexit @ 0x004AB592..0x004AB5A9 =====
int __cdecl atexit(void (__cdecl *Func)())
{
  return (_onexit((_onexit_t)Func) != 0) - 1;
}

// ===== _srand @ 0x004AB5A9..0x004AB5BB =====
void __cdecl srand(unsigned int Seed)
{
  *(_DWORD *)(_getptd() + 20) = Seed;
}

// ===== _rand @ 0x004AB5BB..0x004AB5DC =====
int __cdecl rand()
{
  int v0; // ecx
  unsigned int v1; // eax

  v0 = _getptd();
  v1 = 214013 * *(_DWORD *)(v0 + 20) + 2531011;
  *(_DWORD *)(v0 + 20) = v1;
  return HIWORD(v1) & 0x7FFF;
}

// ===== __endthread @ 0x004AB5DC..0x004AB608 =====
void __cdecl __noreturn _endthread()
{
  int v0; // eax
  void *v1; // esi

  v0 = _getptd_noexit();
  v1 = (void *)v0;
  if ( v0 )
  {
    if ( *(_DWORD *)(v0 + 4) != -1 )
      CloseHandle(*(HANDLE *)(v0 + 4));
    _freeptd(v1);
  }
  ExitThread(0);
}

// ===== __callthreadstart @ 0x004AB609..0x004AB64A =====
void __noreturn _callthreadstart()
{
  int v0; // eax

  v0 = _getptd();
  (*(void (__cdecl **)(_DWORD))(v0 + 84))(*(_DWORD *)(v0 + 88));
  _endthread();
}

// ===== __threadstart@4 @ 0x004AB64A..0x004AB6A2 =====
void __stdcall __noreturn _threadstart(_DWORD *lpThreadParameter)
{
  int v1; // eax
  _DWORD *v2; // eax
  int v3; // eax
  DWORD LastError; // eax

  __set_flsgetvalue();
  v1 = sub_4AEBDB();
  v2 = (_DWORD *)__fls_getvalue(v1);
  if ( v2 )
  {
    v2[21] = lpThreadParameter[21];
    v2[22] = lpThreadParameter[22];
    v2[1] = lpThreadParameter[1];
    _freefls(lpThreadParameter);
  }
  else
  {
    v3 = sub_4AEBDB();
    if ( !__fls_setvalue(v3, lpThreadParameter) )
    {
      LastError = GetLastError();
      ExitThread(LastError);
    }
  }
  _callthreadstart();
}

// ===== __beginthread @ 0x004AB6A3..0x004AB74A =====
uintptr_t __cdecl _beginthread(_beginthread_proc_type StartAddress, unsigned int StackSize, void *ArgList)
{
  DWORD LastError; // ebx
  _DWORD *v5; // esi
  int v6; // eax
  HANDLE Thread; // eax
  uintptr_t v8; // edi

  LastError = 0;
  if ( !StartAddress )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
  __set_flsgetvalue();
  v5 = (_DWORD *)_calloc_crt(1, 532);
  if ( v5 )
  {
    v6 = _getptd();
    _initptd(v5, *(_DWORD *)(v6 + 108));
    v5[21] = StartAddress;
    v5[22] = ArgList;
    Thread = CreateThread(0, StackSize, (LPTHREAD_START_ROUTINE)_threadstart, v5, 4u, v5);
    v8 = (uintptr_t)Thread;
    v5[1] = Thread;
    if ( Thread && ResumeThread(Thread) != -1 )
      return v8;
    LastError = GetLastError();
  }
  free(v5);
  if ( LastError )
    _dosmaperr(LastError);
  return -1;
}

// ===== _sprintf @ 0x004AB74A..0x004AB7CE =====
int sprintf(char *const Buffer, const char *const Format, ...)
{
  int v3; // eax
  bool v4; // sf
  int v5; // esi
  FILE File; // [esp+8h] [ebp-20h] BYREF
  va_list va; // [esp+38h] [ebp+10h] BYREF

  va_start(va, Format);
  memset(&File, 0, sizeof(File));
  if ( Format && Buffer )
  {
    File._base = Buffer;
    File._ptr = Buffer;
    File._cnt = 0x7FFFFFFF;
    File._flag = 66;
    v3 = _output_l(&File, (int)Format, 0, (int)va);
    v4 = --File._cnt < 0;
    v5 = v3;
    if ( v4 )
      _flsbuf(0, &File);
    else
      *File._ptr = 0;
    return v5;
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return -1;
  }
}

// ===== _shortsort @ 0x004AB7D0..0x004AB851 =====
_BYTE *__usercall shortsort@<eax>(
        _BYTE *result@<eax>,
        unsigned int a2,
        int a3,
        int (__cdecl *a4)(unsigned int, unsigned int))
{
  unsigned int v4; // edx
  unsigned int v5; // edi
  int v6; // ecx
  unsigned int v7; // esi
  unsigned int v8; // ebx
  int v9; // esi
  unsigned int v10; // ecx
  char v11; // dl

  v4 = a2;
  v5 = (unsigned int)result;
  if ( (unsigned int)result > a2 )
  {
    v6 = a3;
    do
    {
      v7 = a2 + a3;
      v8 = v4;
      if ( a2 + a3 <= v5 )
      {
        do
        {
          if ( a4(v7, v8) > 0 )
            v8 = v7;
          v7 += a3;
        }
        while ( v7 <= v5 );
        v6 = a3;
        v4 = a2;
      }
      v9 = v6;
      result = (_BYTE *)v5;
      if ( v8 != v5 && v6 )
      {
        v10 = v8 - v5;
        do
        {
          v11 = result[v10];
          result[v10] = *result;
          *result++ = v11;
          --v9;
        }
        while ( v9 );
        v6 = a3;
        v4 = a2;
      }
      v5 -= v6;
    }
    while ( v5 > v4 );
  }
  return result;
}

// ===== _qsort @ 0x004AB860..0x004ABAEB =====
void __cdecl qsort(
        void *Base,
        size_t NumOfElements,
        size_t SizeOfElements,
        _CoreCrtNonSecureSearchSortCompareFunction CompareFunction)
{
  _BYTE *v4; // edi
  size_t v5; // ebx
  _BYTE *v6; // esi
  unsigned int v7; // eax
  _BYTE *v8; // ebx
  size_t v9; // edx
  _BYTE *v10; // eax
  int v11; // ecx
  size_t v12; // ecx
  _BYTE *v13; // eax
  int v14; // edi
  size_t v15; // ecx
  _BYTE *v16; // eax
  int v17; // edi
  size_t v18; // edx
  _BYTE *v19; // eax
  int v20; // ecx
  unsigned int v21; // eax
  unsigned int v22; // edx
  int v23; // ecx
  int v24; // ecx
  int v25; // eax
  _BYTE *v26; // edx
  _BYTE *v27; // eax
  _DWORD v28[60]; // [esp+8h] [ebp-100h]
  size_t v29; // [esp+F8h] [ebp-10h]
  int v30; // [esp+FCh] [ebp-Ch]
  _BYTE *v31; // [esp+100h] [ebp-8h]
  _BYTE *v32; // [esp+104h] [ebp-4h]
  char Base_3; // [esp+113h] [ebp+Bh]
  char Base_3a; // [esp+113h] [ebp+Bh]
  char Base_3b; // [esp+113h] [ebp+Bh]
  char Base_3c; // [esp+113h] [ebp+Bh]

  v4 = Base;
  if ( !Base && NumOfElements )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return;
  }
  v5 = SizeOfElements;
  if ( !SizeOfElements || !CompareFunction )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return;
  }
  if ( NumOfElements >= 2 )
  {
    v6 = (char *)Base + SizeOfElements * (NumOfElements - 1);
    v30 = 0;
    v32 = Base;
    v31 = v6;
    while ( 1 )
    {
      v7 = (v6 - v4) / v5 + 1;
      if ( v7 <= 8 )
      {
        shortsort(v6, (unsigned int)v4, v5, (int (__cdecl *)(unsigned int, unsigned int))CompareFunction);
        goto LABEL_57;
      }
      v8 = &v4[(v7 >> 1) * v5];
      if ( CompareFunction(v4, v8) > 0 )
      {
        v9 = SizeOfElements;
        v10 = v8;
        if ( v4 != v8 )
        {
          v11 = v4 - v8;
          do
          {
            v29 = v9 - 1;
            Base_3 = v10[v11];
            v10[v11] = *v10;
            *v10 = Base_3;
            v9 = v29;
            ++v10;
          }
          while ( v29 );
        }
      }
      if ( CompareFunction(v4, v6) > 0 )
      {
        v12 = SizeOfElements;
        v13 = v6;
        if ( v4 != v6 )
        {
          v14 = v4 - v6;
          do
          {
            Base_3a = v13[v14];
            v13[v14] = *v13;
            *v13++ = Base_3a;
            --v12;
          }
          while ( v12 );
          v4 = v32;
        }
      }
      if ( CompareFunction(v8, v6) > 0 )
      {
        v15 = SizeOfElements;
        v16 = v6;
        if ( v8 != v6 )
        {
          v17 = v8 - v6;
          do
          {
            Base_3b = v16[v17];
            v16[v17] = *v16;
            *v16++ = Base_3b;
            --v15;
          }
          while ( v15 );
          v4 = v32;
        }
      }
      while ( 1 )
      {
        if ( v8 > v4 )
        {
          while ( 1 )
          {
            v4 += SizeOfElements;
            if ( v4 >= v8 )
              break;
            if ( CompareFunction(v4, v8) > 0 )
            {
              if ( v8 > v4 )
                goto LABEL_32;
              goto LABEL_30;
            }
          }
        }
        do
LABEL_30:
          v4 += SizeOfElements;
        while ( v4 <= v31 && CompareFunction(v4, v8) <= 0 );
        do
LABEL_32:
          v6 -= SizeOfElements;
        while ( v6 > v8 && CompareFunction(v6, v8) > 0 );
        if ( v4 > v6 )
          break;
        v18 = SizeOfElements;
        v19 = v6;
        if ( v4 != v6 )
        {
          v20 = v4 - v6;
          do
          {
            v29 = v18 - 1;
            Base_3c = v19[v20];
            v19[v20] = *v19;
            *v19 = Base_3c;
            v18 = v29;
            ++v19;
          }
          while ( v29 );
        }
        if ( v8 == v6 )
          v8 = v4;
      }
      v6 += SizeOfElements;
      if ( v8 >= v6 )
        goto LABEL_44;
      do
      {
        v6 -= SizeOfElements;
        if ( v6 <= v8 )
          goto LABEL_44;
      }
      while ( !CompareFunction(v6, v8) );
      if ( v8 < v6 )
      {
LABEL_46:
        v21 = (unsigned int)v32;
      }
      else
      {
LABEL_44:
        while ( 1 )
        {
          v6 -= SizeOfElements;
          v21 = (unsigned int)v32;
          if ( v6 <= v32 )
            break;
          if ( CompareFunction(v6, v8) )
            goto LABEL_46;
        }
      }
      v22 = (unsigned int)v31;
      if ( (int)&v6[-v21] < v31 - v4 )
      {
        if ( v4 < v31 )
        {
          v24 = v30;
          v28[v30 + 30] = v4;
          v28[v24] = v22;
          v30 = v24 + 1;
        }
        if ( v21 >= (unsigned int)v6 )
          goto LABEL_56;
        v4 = v32;
        v5 = SizeOfElements;
        v31 = v6;
      }
      else
      {
        if ( v21 < (unsigned int)v6 )
        {
          v23 = v30;
          v28[v30 + 30] = v21;
          v28[v23] = v6;
          v30 = v23 + 1;
        }
        if ( (unsigned int)v4 >= v22 )
        {
LABEL_56:
          v5 = SizeOfElements;
LABEL_57:
          v25 = --v30;
          if ( v30 < 0 )
            return;
          v26 = (_BYTE *)v28[v25 + 30];
          v27 = (_BYTE *)v28[v25];
          v32 = v26;
          v31 = v27;
          v6 = v27;
          v4 = v26;
        }
        else
        {
          v6 = v31;
          v5 = SizeOfElements;
          v32 = v4;
        }
      }
    }
  }
}

// ===== __wsplitpath_helper @ 0x004ABAEB..0x004ABCF8 =====
int __cdecl _wsplitpath_helper(
        wchar_t *Source,
        wchar_t *Destination,
        unsigned int a3,
        wchar_t *a4,
        unsigned int a5,
        wchar_t *a6,
        unsigned int a7,
        wchar_t *a8,
        unsigned int a9)
{
  wchar_t *v9; // ecx
  int v10; // eax
  wchar_t *v11; // esi
  wchar_t v12; // ax
  wchar_t *v13; // edi
  const wchar_t *v14; // ebx
  wchar_t *v15; // esi
  rsize_t v16; // esi
  rsize_t v17; // esi
  int v19; // [esp+Ch] [ebp-4h]

  v9 = Source;
  v19 = 0;
  if ( !Source )
    goto $error_einval$30017;
  if ( Destination )
  {
    if ( !a3 )
      goto $error_einval$30017;
  }
  else if ( a3 )
  {
    goto $error_einval$30017;
  }
  if ( a4 )
  {
    if ( !a5 )
      goto $error_einval$30017;
  }
  else if ( a5 )
  {
    goto $error_einval$30017;
  }
  if ( a6 )
  {
    if ( !a7 )
      goto $error_einval$30017;
  }
  else if ( a7 )
  {
    goto $error_einval$30017;
  }
  if ( a8 )
  {
    if ( a9 )
      goto LABEL_16;
$error_einval$30017:
    v19 = 1;
    goto $error_erange$30047;
  }
  if ( a9 )
    goto $error_einval$30017;
LABEL_16:
  v10 = 1;
  v11 = Source;
  do
  {
    if ( !*v11 )
      break;
    --v10;
    ++v11;
  }
  while ( v10 );
  if ( *v11 == 58 )
  {
    if ( Destination )
    {
      if ( a3 < 3 )
        goto $error_erange$30047;
      wcsncpy_s(Destination, 0xFFFFFFFF, Source, 2u);
    }
    Source = v11 + 1;
    v9 = v11 + 1;
  }
  else if ( Destination )
  {
    *Destination = 0;
  }
  v12 = *v9;
  v13 = 0;
  v14 = 0;
  v15 = v9;
  if ( !*v9 )
    goto LABEL_40;
  do
  {
    if ( v12 == 47 || v12 == 92 )
    {
      v13 = v15 + 1;
    }
    else if ( v12 == 46 )
    {
      v14 = v15;
    }
    v12 = *++v15;
  }
  while ( *v15 );
  if ( v13 )
  {
    if ( a4 )
    {
      if ( a5 <= v13 - v9 )
        goto $error_erange$30047;
      wcsncpy_s(a4, 0xFFFFFFFF, v9, v13 - v9);
    }
    Source = v13;
    v9 = v13;
  }
  else
  {
LABEL_40:
    if ( a4 )
      *a4 = 0;
  }
  if ( !v14 || v14 < v9 )
  {
    if ( a6 )
    {
      v17 = v15 - v9;
      if ( a7 <= v17 )
        goto $error_erange$30047;
      wcsncpy_s(a6, 0xFFFFFFFF, v9, v17);
    }
    if ( a8 )
      *a8 = 0;
    return 0;
  }
  if ( a6 )
  {
    if ( a7 <= v14 - v9 )
      goto $error_erange$30047;
    wcsncpy_s(a6, 0xFFFFFFFF, v9, v14 - v9);
    v9 = Source;
  }
  if ( !a8 )
    return 0;
  v16 = v15 - v14;
  if ( a9 > v16 )
  {
    wcsncpy_s(a8, 0xFFFFFFFF, v14, v16);
    return 0;
  }
$error_erange$30047:
  if ( Destination && a3 )
    *Destination = 0;
  if ( a4 && a5 )
    *a4 = 0;
  if ( a6 && a7 )
    *a6 = 0;
  if ( a8 && a9 )
    *a8 = 0;
  if ( !v9 || v19 )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 22;
  }
  else
  {
    *_errno() = 34;
    return 34;
  }
}

// ===== __wsplitpath @ 0x004ABCF8..0x004ABD44 =====
void __cdecl _wsplitpath(const wchar_t *FullPath, wchar_t *Drive, wchar_t *Dir, wchar_t *Filename, wchar_t *Ext)
{
  _wsplitpath_helper(
    (wchar_t *)FullPath,
    Drive,
    Drive != 0 ? 3 : 0,
    Dir,
    Dir != 0 ? 0x100 : 0,
    Filename,
    Filename != 0 ? 0x100 : 0,
    Ext,
    Ext != 0 ? 0x100 : 0);
}

// ===== ??0_LocaleUpdate@@QAE@PAUlocaleinfo_struct@@@Z @ 0x004ABD44..0x004ABDCB =====
_LocaleUpdate *__thiscall _LocaleUpdate::_LocaleUpdate(_LocaleUpdate *this, struct localeinfo_struct *a2)
{
  _DWORD *v3; // eax
  int v4; // eax

  *((_BYTE *)this + 12) = 0;
  if ( a2 )
  {
    *(_DWORD *)this = *(_DWORD *)a2;
    *((_DWORD *)this + 1) = *((_DWORD *)a2 + 1);
  }
  else
  {
    v3 = (_DWORD *)_getptd();
    *((_DWORD *)this + 2) = v3;
    *(_DWORD *)this = v3[27];
    *((_DWORD *)this + 1) = v3[26];
    if ( *(volatile LONG **)this != off_4FC058 && (dword_4FBE10 & v3[28]) == 0 )
      *(_DWORD *)this = __updatetlocinfo();
    if ( *((volatile LONG **)this + 1) != lpAddend && (dword_4FBE10 & *(_DWORD *)(*((_DWORD *)this + 2) + 112)) == 0 )
      *((_DWORD *)this + 1) = __updatetmbcinfo();
    v4 = *((_DWORD *)this + 2);
    if ( (*(_BYTE *)(v4 + 112) & 2) == 0 )
    {
      *(_DWORD *)(v4 + 112) |= 2u;
      *((_BYTE *)this + 12) = 1;
    }
  }
  return this;
}

// ===== ___ascii_stricmp @ 0x004ABDCB..0x004ABE04 =====
int __cdecl __ascii_stricmp(unsigned __int8 *a1, unsigned __int8 *a2)
{
  int v4; // eax
  int v5; // ecx

  do
  {
    v4 = *a1++;
    if ( (unsigned int)(v4 - 65) <= 0x19 )
      v4 += 32;
    v5 = *a2++;
    if ( (unsigned int)(v5 - 65) <= 0x19 )
      v5 += 32;
  }
  while ( v4 && v4 == v5 );
  return v4 - v5;
}

// ===== __stricmp_l @ 0x004ABE04..0x004ABEC4 =====
int __cdecl _stricmp_l(const char *String1, const char *String2, _locale_t Locale)
{
  int result; // eax
  const char *v4; // esi
  int v5; // edi
  int v6; // eax
  __crt_locale_pointers v7; // [esp+4h] [ebp-10h] BYREF
  int v8; // [esp+Ch] [ebp-8h]
  char v9; // [esp+10h] [ebp-4h]

  _LocaleUpdate::_LocaleUpdate((_LocaleUpdate *)&v7, (struct localeinfo_struct *)Locale);
  if ( String1 )
  {
    v4 = String2;
    if ( String2 )
    {
      if ( *((_DWORD *)v7.locinfo + 5) )
      {
        do
        {
          v5 = _tolower_l((unsigned __int8)v4[String1 - String2], &v7);
          v6 = _tolower_l(*(unsigned __int8 *)v4++, &v7);
        }
        while ( v5 && v5 == v6 );
        result = v5 - v6;
      }
      else
      {
        result = __ascii_stricmp((unsigned __int8 *)String1, (unsigned __int8 *)String2);
      }
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
    }
    else
    {
      *_errno() = 22;
      _invalid_parameter_noinfo();
      if ( v9 )
        *(_DWORD *)(v8 + 112) &= ~2u;
      return 0x7FFFFFFF;
    }
  }
  else
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    if ( v9 )
      *(_DWORD *)(v8 + 112) &= ~2u;
    return 0x7FFFFFFF;
  }
  return result;
}

// ===== __stricmp @ 0x004ABEC4..0x004ABF0B =====
int __cdecl _stricmp(const char *String1, const char *String2)
{
  if ( dword_509F0C )
    return _stricmp_l(String1, String2, 0);
  if ( String1 && String2 )
    return __ascii_stricmp((unsigned __int8 *)String1, (unsigned __int8 *)String2);
  *_errno() = 22;
  _invalid_parameter_noinfo();
  return 0x7FFFFFFF;
}

// ===== __aligned_offset_malloc @ 0x004ABF0B..0x004ABFA3 =====
void *__cdecl _aligned_offset_malloc(size_t Size, size_t Alignment, size_t Offset)
{
  size_t v3; // esi
  void *result; // eax
  size_t v5; // esi
  size_t v6; // edi
  int v7; // ebx
  void *v8; // eax
  void *v9; // ecx
  int v10; // esi

  v3 = Alignment;
  if ( ((Alignment - 1) & Alignment) != 0 || Offset && Offset >= Size )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 0;
  }
  if ( Alignment <= 4 )
    v3 = 4;
  v5 = v3 - 1;
  v6 = -Offset & 3;
  v7 = v6 + v5 + 4;
  if ( Size > v7 + Size )
  {
    *_errno() = 12;
    return 0;
  }
  v8 = malloc(v7 + Size);
  v9 = v8;
  if ( !v8 )
    return 0;
  v10 = ~v5;
  result = (void *)((v10 & ((unsigned int)v8 + v7 + Offset)) - Offset);
  *(_DWORD *)((v10 & ((unsigned int)v9 + v7 + Offset)) - Offset - v6 - 4) = v9;
  return result;
}

// ===== __aligned_free @ 0x004ABFA3..0x004ABFBD =====
void __cdecl _aligned_free(void *Block)
{
  if ( Block )
    free(*(void **)(((unsigned int)Block & 0xFFFFFFFC) - 4));
}

// ===== __aligned_malloc @ 0x004ABFBD..0x004ABFD4 =====
void *__cdecl _aligned_malloc(size_t Size, size_t Alignment)
{
  return _aligned_offset_malloc(Size, Alignment, 0);
}

// ===== _strstr @ 0x004ABFE0..0x004AC066 =====
char *__cdecl strstr(const char *Str, const char *SubStr)
{
  char v2; // dl
  const char *v3; // edi
  char v4; // dh
  const char *v5; // ecx
  char *v6; // esi
  char v7; // al
  char v9; // ah
  char v10; // al
  char v11; // al

  v2 = *SubStr;
  v3 = Str;
  if ( !*SubStr )
    return (char *)Str;
  v4 = SubStr[1];
  if ( !v4 )
    JUMPOUT(0x4AC1C6);
findnext:
  v5 = SubStr;
  v6 = (char *)(v3 + 1);
  if ( *v3 == v2 )
    goto first_char_found;
  if ( *v3 )
  {
    while ( 2 )
    {
      v7 = *v6++;
      while ( v7 == v2 )
      {
first_char_found:
        v7 = *v6++;
        if ( v7 == v4 )
        {
          v3 = v6 - 1;
          while ( 1 )
          {
            v9 = v5[2];
            if ( !v9 )
              break;
            v10 = *v6;
            v6 += 2;
            if ( v10 != v9 )
              goto findnext;
            v11 = v5[3];
            if ( !v11 )
              break;
            v5 += 2;
            if ( v11 != *(v6 - 1) )
              goto findnext;
          }
          return (char *)(v3 - 1);
        }
      }
      if ( v7 )
        continue;
      break;
    }
  }
  return 0;
}
