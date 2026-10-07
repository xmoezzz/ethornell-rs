#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_444000 @ 0x00444000..0x00444067 =====
double __userpurge sub_444000@<st0>(int *a1@<esi>, double a2)
{
  int v2; // edi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-4h]
  double X; // [esp+18h] [ebp+8h]

  v2 = *a1;
  X = (double)*a1 * a2;
  v3 = (int)floor(X);
  v5 = v3;
  if ( v3 < 0 )
  {
    v3 = 0;
LABEL_5:
    v5 = v3;
    return (X - (double)v5)
         * ((*(double *)&a1[2 * v3 + 608] * (X - (double)v5) + *(double *)&a1[2 * v3 + 406]) * (X - (double)v5)
          + *(double *)&a1[2 * v3 + 204])
         + *(double *)&a1[2 * v3 + 2];
  }
  if ( v3 >= v2 )
  {
    v3 = v2 - 1;
    goto LABEL_5;
  }
  return (X - (double)v5)
       * ((*(double *)&a1[2 * v3 + 608] * (X - (double)v5) + *(double *)&a1[2 * v3 + 406]) * (X - (double)v5)
        + *(double *)&a1[2 * v3 + 204])
       + *(double *)&a1[2 * v3 + 2];
}

// ===== sub_444070 @ 0x00444070..0x004440F3 =====
_DWORD *__stdcall sub_444070(_DWORD *a1)
{
  *a1 = &CSpline::`vftable';
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  a1[6] = 0;
  a1[7] = 0;
  a1[8] = 0;
  a1[10] = 0;
  a1[11] = 0;
  a1[12] = 0;
  a1[14] = 0;
  a1[824] = 0;
  a1[1634] = 0;
  sub_4441D0();
  return a1;
}

// ===== sub_444100 @ 0x00444100..0x00444121 =====
void *__thiscall sub_444100(void *this, char a2)
{
  sub_444130();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_444130 @ 0x00444130..0x004441CF =====
void __thiscall sub_444130(void **this)
{
  *this = &CSpline::`vftable';
  if ( this[10] )
    operator delete(this[10]);
  this[10] = 0;
  this[11] = 0;
  this[12] = 0;
  if ( this[6] )
    operator delete(this[6]);
  this[6] = 0;
  this[7] = 0;
  this[8] = 0;
  if ( this[2] )
    operator delete(this[2]);
  this[2] = 0;
  this[3] = 0;
  this[4] = 0;
}

// ===== sub_4441D0 @ 0x004441D0..0x00444261 =====
int __usercall sub_4441D0@<eax>(int a1@<esi>)
{
  int result; // eax
  int v2; // edi
  int v3; // edi
  int v4; // edi

  result = 0;
  *(_DWORD *)(a1 + 9780) = 0;
  *(_DWORD *)(a1 + 9776) = 0;
  *(_BYTE *)(a1 + 9784) = 0;
  v2 = *(_DWORD *)(a1 + 8);
  if ( v2 != *(_DWORD *)(a1 + 12) )
  {
    result = *(_DWORD *)(a1 + 8);
    *(_DWORD *)(a1 + 12) = v2;
  }
  v3 = *(_DWORD *)(a1 + 24);
  if ( v3 != *(_DWORD *)(a1 + 28) )
  {
    result = *(_DWORD *)(a1 + 24);
    *(_DWORD *)(a1 + 28) = v3;
  }
  v4 = *(_DWORD *)(a1 + 40);
  if ( v4 != *(_DWORD *)(a1 + 44) )
  {
    result = *(_DWORD *)(a1 + 40);
    *(_DWORD *)(a1 + 44) = v4;
  }
  return result;
}

// ===== sub_444270 @ 0x00444270..0x004442B8 =====
char __userpurge sub_444270@<al>(int a1@<edi>, char a2, char a3, char a4)
{
  if ( (unsigned int)((*(_DWORD *)(a1 + 12) - *(_DWORD *)(a1 + 8)) >> 2) >= 0x64 )
    return 0;
  *(_BYTE *)(a1 + 9784) = 0;
  sub_4444F0();
  sub_4444F0();
  sub_4444F0();
  return 1;
}

// ===== sub_4442C0 @ 0x004442C0..0x004442D1 =====
int __usercall sub_4442C0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 9776) = 0;
  *(_DWORD *)(result + 9780) = a2;
  return result;
}

// ===== sub_4442E0 @ 0x004442E0..0x00444420 =====
char __usercall sub_4442E0@<al>(int a1@<eax>, unsigned int a2@<edx>, _DWORD *a3@<esi>)
{
  char result; // al
  unsigned int v5; // ecx
  int v6; // eax
  double v7; // st7

  if ( !a3 )
    return 0;
  a3[2] = 0;
  a3[1] = 0;
  *a3 = 0;
  v5 = *(_DWORD *)(a1 + 9780);
  if ( !v5 )
    return 0;
  v6 = (*(_DWORD *)(a1 + 12) - *(_DWORD *)(a1 + 8)) >> 2;
  if ( !v6 )
    return 0;
  if ( v5 + *(_DWORD *)(a1 + 9776) > a2 )
  {
    v7 = (double)(a2 - *(_DWORD *)(a1 + 9776)) / (double)v5;
    if ( v6 == 1 )
    {
      *a3 = **(_DWORD **)(a1 + 8);
      a3[1] = **(_DWORD **)(a1 + 24);
      result = 1;
      a3[2] = **(_DWORD **)(a1 + 40);
    }
    else if ( v6 == 2 )
    {
      *a3 = **(_DWORD **)(a1 + 8) + (int)((double)(*(_DWORD *)(*(_DWORD *)(a1 + 8) + 4) - **(_DWORD **)(a1 + 8)) * v7);
      a3[1] = **(_DWORD **)(a1 + 24)
            + (int)((double)(*(_DWORD *)(*(_DWORD *)(a1 + 24) + 4) - **(_DWORD **)(a1 + 24)) * v7);
      a3[2] = **(_DWORD **)(a1 + 40)
            + (int)(v7 * (double)(*(_DWORD *)(*(_DWORD *)(a1 + 40) + 4) - **(_DWORD **)(a1 + 40)));
      return 1;
    }
    else
    {
      return sub_444420((int)a3, v7);
    }
  }
  else
  {
    *a3 = *(_DWORD *)(*(_DWORD *)(a1 + 8) + 4 * v6 - 4);
    a3[1] = *(_DWORD *)(*(_DWORD *)(a1 + 24) + 4 * v6 - 4);
    a3[2] = *(_DWORD *)(*(_DWORD *)(a1 + 40) + 4 * v6 - 4);
    return 1;
  }
  return result;
}

// ===== sub_444420 @ 0x00444420..0x004444B9 =====
char __userpurge sub_444420@<al>(int a1@<edi>, _DWORD *a2, double a3)
{
  if ( !*(_BYTE *)(a1 + 9784) )
  {
    *(_BYTE *)(a1 + 9784) = 1;
    sub_443E50((int *)(a1 + 56), (_DWORD *)(a1 + 8));
    sub_443E50((int *)(a1 + 3296), (_DWORD *)(a1 + 24));
    sub_443E50((int *)(a1 + 6536), (_DWORD *)(a1 + 40));
  }
  *a2 = (int)sub_444000((int *)(a1 + 56), a3);
  a2[1] = (int)sub_444000((int *)(a1 + 3296), a3);
  a2[2] = (int)sub_444000((int *)(a1 + 6536), a3);
  return 1;
}

// ===== sub_4444F0 @ 0x004444F0..0x00444548 =====
int __usercall sub_4444F0@<eax>(unsigned int a1@<eax>, unsigned int *a2@<esi>)
{
  unsigned int v3; // eax
  int result; // eax

  v3 = a2[1];
  if ( a1 >= v3 || *a2 > a1 )
  {
    if ( v3 == a2[2] )
      sub_444550(a2);
  }
  else if ( v3 == a2[2] )
  {
    sub_444550(a2);
  }
  result = sub_444710(a2[1]);
  a2[1] += 4;
  return result;
}

// ===== sub_444550 @ 0x00444550..0x0044459E =====
unsigned int __thiscall sub_444550(_DWORD *this)
{
  unsigned int v1; // eax
  unsigned int result; // eax
  unsigned int v3; // edx
  unsigned int v4; // edx

  v1 = (this[1] - *this) >> 2;
  if ( v1 > 0x3FFFFFFE )
    std::_Xlength_error("vector<T> too long");
  result = v1 + 1;
  v3 = (this[2] - *this) >> 2;
  if ( result > v3 )
  {
    if ( 0x3FFFFFFF - (v3 >> 1) >= v3 )
      v4 = (v3 >> 1) + v3;
    else
      v4 = 0;
    if ( v4 < result )
      v4 = result;
    return sub_4445A0(v4);
  }
  return result;
}

// ===== sub_4445A0 @ 0x004445A0..0x00444673 =====
unsigned int __thiscall sub_4445A0(int this, unsigned int a2)
{
  unsigned int result; // eax
  char *v4; // ebx
  int v5; // edi
  void *v6[6]; // [esp+0h] [ebp-24h] BYREF
  int v7; // [esp+20h] [ebp-4h]

  v6[5] = v6;
  if ( a2 > 0x3FFFFFFF )
    std::_Xlength_error("vector<T> too long");
  result = (*(_DWORD *)(this + 8) - *(_DWORD *)this) >> 2;
  if ( result < a2 )
  {
    v4 = (char *)sub_444680(v6[0], v6[1]);
    v6[4] = v4;
    v7 = 0;
    memcpy(v4, *(const void **)this, 4 * ((*(_DWORD *)(this + 4) - *(_DWORD *)this) >> 2));
    v7 = -1;
    v5 = (*(_DWORD *)(this + 4) - *(_DWORD *)this) >> 2;
    if ( *(_DWORD *)this )
      operator delete(*(void **)this);
    result = a2;
    *(_DWORD *)(this + 8) = &v4[4 * a2];
    *(_DWORD *)(this + 4) = &v4[4 * v5];
    *(_DWORD *)this = v4;
  }
  return result;
}

// ===== sub_444680 @ 0x00444680..0x0044470C =====
void *__fastcall sub_444680(unsigned int a1)
{
  void *result; // eax
  _DWORD pExceptionObject[3]; // [esp+4h] [ebp-1Ch] BYREF
  char *v3[4]; // [esp+10h] [ebp-10h] BYREF

  result = 0;
  if ( a1 )
  {
    if ( a1 > 0x3FFFFFFF || (result = operator new(4 * a1)) == 0 )
    {
      v3[0] = 0;
      std::exception::exception((std::exception *)pExceptionObject, (const char *const *)v3);
      pExceptionObject[0] = &std::bad_alloc::`vftable';
      v3[3] = (char *)-1;
      _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI2_AVbad_alloc_std__);
    }
  }
  return result;
}

// ===== sub_444710 @ 0x00444710..0x0044475A =====
_DWORD *__cdecl sub_444710(_DWORD *a1)
{
  _DWORD *v1; // ecx
  _DWORD *result; // eax

  result = a1;
  if ( a1 )
    *a1 = *v1;
  return result;
}

// ===== sub_444760 @ 0x00444760..0x004447BA =====
std::exception *__thiscall sub_444760(std::exception *this, struct exception *a2)
{
  std::exception::exception(this, a2);
  *(_DWORD *)this = &std::bad_alloc::`vftable';
  return this;
}

// ===== sub_4447C0 @ 0x004447C0..0x00444989 =====
_DWORD *__fastcall sub_4447C0(int a1, int a2, _DWORD *a3, int a4, size_t a5, size_t a6, int a7)
{
  int v8; // eax
  _DWORD *v9; // eax
  _DWORD *v10; // eax
  _DWORD *v11; // eax
  _DWORD *v12; // eax
  _DWORD *v13; // eax
  _DWORD *v14; // eax
  _DWORD *v15; // eax
  _DWORD *v16; // eax

  a3[1] = a4;
  v8 = dword_565D60;
  a3[2] = dword_565D60;
  *a3 = &CThread::`vftable';
  dword_565D60 = v8 + 1;
  a3[3] = 0;
  if ( a1 )
  {
    a3[4] = a1;
    v9 = operator new(0x10u);
    if ( v9 )
      v10 = sub_430260(v9);
    else
      v10 = 0;
    a3[5] = v10;
    a3[6] = sub_4302C0(4 * a1, v10);
  }
  else
  {
    a3[4] = 0;
    a3[5] = 0;
    a3[6] = 0;
  }
  a3[8] = 0;
  if ( a5 )
  {
    a3[7] = a5;
    a3[9] = a5;
    v11 = operator new(0x10u);
    if ( v11 )
      v12 = sub_430260(v11);
    else
      v12 = 0;
    a3[10] = v12;
    a3[11] = sub_4302C0(a5, v12);
  }
  else
  {
    a3[7] = 0;
    a3[9] = 0;
    a3[10] = 0;
    a3[11] = 0;
  }
  a3[12] = 0;
  a3[13] = 0;
  a3[14] = 0;
  a3[16] = 0;
  if ( a6 )
  {
    a3[15] = a6;
    a3[17] = a6;
    v13 = operator new(0x10u);
    if ( v13 )
      v14 = sub_430260(v13);
    else
      v14 = 0;
    a3[18] = v14;
    a3[19] = sub_4302C0(a6, v14);
  }
  else
  {
    a3[15] = 0;
    a3[17] = 0;
    a3[18] = 0;
    a3[19] = 0;
  }
  if ( a7 )
  {
    v15 = operator new(0x20u);
    if ( v15 )
      v16 = sub_430310(v15);
    else
      v16 = 0;
  }
  else
  {
    v16 = 0;
  }
  a3[20] = v16;
  a3[21] = 0;
  a3[22] = 0;
  a3[24] = 0;
  a3[25] = 0;
  a3[26] = 0;
  a3[27] = 0;
  a3[28] = 0;
  a3[29] = 0;
  a3[30] = 0;
  a3[31] = 0;
  a3[32] = 0;
  a3[33] = 0;
  return a3;
}

// ===== sub_444990 @ 0x00444990..0x004449B1 =====
void *__thiscall sub_444990(void *this, char a2)
{
  sub_4449C0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4449C0 @ 0x004449C0..0x00444A5B =====
int __thiscall sub_4449C0(_DWORD *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx
  void (__thiscall ***v3)(_DWORD, int); // ecx
  void (__thiscall ***v4)(_DWORD, int); // ecx
  void *v5; // edi
  void (__thiscall ***v6)(_DWORD, int); // ecx
  int result; // eax

  v2 = (void (__thiscall ***)(_DWORD, int))this[5];
  *this = &CThread::`vftable';
  if ( v2 )
    (**v2)(v2, 1);
  v3 = (void (__thiscall ***)(_DWORD, int))this[10];
  if ( v3 )
    (**v3)(v3, 1);
  v4 = (void (__thiscall ***)(_DWORD, int))this[18];
  if ( v4 )
    (**v4)(v4, 1);
  v5 = (void *)this[20];
  if ( v5 )
  {
    sub_430350((int)v5);
    operator delete(v5);
  }
  sub_444BF0(this);
  while ( sub_445150(this) )
    ;
  v6 = (void (__thiscall ***)(_DWORD, int))this[22];
  if ( v6 )
    (**v6)(v6, 1);
  do
    result = sub_445300(this);
  while ( result );
  for ( ; this[26]; result = sub_444B10(*(_DWORD *)this[26], this) )
    ;
  return result;
}

// ===== sub_444A60 @ 0x00444A60..0x00444A6A =====
_DWORD *__thiscall sub_444A60(_DWORD *this)
{
  _DWORD *result; // eax

  result = (_DWORD *)this[1];
  if ( !result )
    return this;
  return result;
}

// ===== sub_444A70 @ 0x00444A70..0x00444B01 =====
_DWORD *__thiscall sub_444A70(_DWORD *this, int a2, size_t a3, size_t a4)
{
  _DWORD *i; // esi
  _DWORD *v5; // eax
  _DWORD *v6; // edx
  _DWORD *result; // eax

  for ( i = this; i[3]; i = (_DWORD *)i[3] )
    ;
  if ( operator new(0x88u) )
  {
    v5 = sub_444A60(i);
    result = sub_4447C0(a2, (int)v6, v6, (int)v5, a3, a4, 1);
  }
  else
  {
    result = 0;
  }
  i[3] = result;
  return result;
}

// ===== sub_444B10 @ 0x00444B10..0x00444B44 =====
int __fastcall sub_444B10(int a1, int a2)
{
  int v2; // eax
  __int64 v4; // rax
  void (__thiscall ***v5)(_DWORD, int); // ecx

  if ( !*(_DWORD *)(a2 + 12) )
    return 0;
  while ( 1 )
  {
    v2 = *(_DWORD *)(a2 + 12);
    if ( v2 == a1 )
      break;
    a2 = *(_DWORD *)(a2 + 12);
    if ( !*(_DWORD *)(v2 + 12) )
      return 0;
  }
  v4 = sub_444B50();
  *(_DWORD *)(HIDWORD(v4) + 12) = v4;
  if ( v5 )
    (**v5)(v5, 1);
  return 1;
}

// ===== sub_444B50 @ 0x00444B50..0x00444B54 =====
int __usercall sub_444B50@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 12);
}

// ===== sub_444B60 @ 0x00444B60..0x00444B87 =====
int __thiscall sub_444B60(_DWORD *this)
{
  int result; // eax
  int (__thiscall ***v3)(_DWORD, int); // ecx

  if ( this[3] )
  {
    result = sub_444B60();
    v3 = (int (__thiscall ***)(_DWORD, int))this[3];
    if ( v3 )
      result = (**v3)(v3, 1);
    this[3] = 0;
  }
  return result;
}

// ===== sub_444B90 @ 0x00444B90..0x00444BC8 =====
int __thiscall sub_444B90(void *this, int a2)
{
  int v2; // ecx

  if ( a2 == sub_42D560((int)this) )
  {
    if ( a2 )
      return v2;
  }
  else if ( *(_DWORD *)(v2 + 12) )
  {
    return sub_444B90(a2);
  }
  return 0;
}

// ===== sub_444BD0 @ 0x00444BD0..0x00444BEF =====
int __fastcall sub_444BD0(int a1, int a2)
{
  int result; // eax
  bool v3; // zf

  result = a1 + 12;
  if ( *(_DWORD *)(a1 + 12) )
  {
    do
    {
      a1 = *(_DWORD *)result;
      v3 = *(_DWORD *)(*(_DWORD *)result + 12) == 0;
      result = *(_DWORD *)result + 12;
    }
    while ( !v3 );
  }
  *(_DWORD *)(a1 + 12) = a2;
  return result;
}

// ===== sub_444BF0 @ 0x00444BF0..0x00444C2E =====
int __stdcall sub_444BF0(_DWORD *a1)
{
  void **v1; // esi
  int result; // eax
  void **v3; // edi
  void *v4; // eax

  v1 = (void **)a1[12];
  result = 0;
  if ( v1 )
  {
    do
    {
      v3 = v1;
      v4 = *v1;
      v1 = (void **)v1[3];
      operator delete[](v4);
      operator delete(v3);
    }
    while ( v1 );
    result = 0;
  }
  a1[12] = 0;
  a1[13] = 0;
  a1[14] = 0;
  return result;
}

// ===== sub_444C30 @ 0x00444C30..0x00444C37 =====
int __thiscall sub_444C30(_DWORD *this)
{
  return this[8] + this[9];
}

// ===== sub_444C40 @ 0x00444C40..0x00444C47 =====
int __thiscall sub_444C40(_DWORD *this)
{
  return this[16] + this[17];
}

// ===== sub_444C50 @ 0x00444C50..0x00444C54 =====
int __usercall sub_444C50@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 64);
}

// ===== sub_444C60 @ 0x00444C60..0x00444C64 =====
int __usercall sub_444C60@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 44);
}

// ===== sub_444C70 @ 0x00444C70..0x00444C74 =====
int __usercall sub_444C70@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 76);
}

// ===== sub_444C80 @ 0x00444C80..0x00444C95 =====
int __thiscall sub_444C80(int *this, unsigned int a2)
{
  return sub_4303A0(this[20], a2);
}

// ===== sub_444CA0 @ 0x00444CA0..0x00444CB4 =====
int __thiscall sub_444CA0(_DWORD **this, int a2)
{
  return sub_4304B0(a2, this[20]);
}

// ===== sub_444CC0 @ 0x00444CC0..0x00444CCC =====
int __thiscall sub_444CC0(int *this, int a2)
{
  return sub_430510(this[20], a2);
}

// ===== sub_444CD0 @ 0x00444CD0..0x00444CD8 =====
int __thiscall sub_444CD0(int *this)
{
  return sub_407760(this[20]);
}

// ===== sub_444CE0 @ 0x00444CE0..0x00444D7B =====
int __userpurge sub_444CE0@<eax>(_DWORD *a1@<edi>, _DWORD *a2, const char *a3)
{
  int v3; // edx
  _DWORD *v4; // esi
  const char *v5; // ecx
  _BYTE *v6; // edx
  char v7; // al

  if ( a2[1] + a1[14] > (unsigned int)sub_444C30(a1) )
    return v3;
  v4 = operator new(0x10u);
  *v4 = operator new[](strlen(a3) + 1);
  v4[1] = a2[1];
  v4[2] = a1[14];
  v4[3] = a1[12];
  v5 = a3;
  a1[12] = v4;
  v6 = (_BYTE *)*v4;
  do
  {
    v7 = *v5;
    *v6++ = *v5++;
  }
  while ( v7 );
  memcpy_0((void *)(v4[2] + a1[11]), (char *)a2 + *a2, a2[1]);
  a1[14] += a2[1];
  ++a1[13];
  return v4[2];
}

// ===== sub_444D80 @ 0x00444D80..0x00444DB2 =====
int __usercall sub_444D80@<eax>(_DWORD *a1@<esi>)
{
  int v1; // edi
  int result; // eax

  v1 = a1[12];
  result = -2147483647;
  if ( v1 )
  {
    a1[12] = *(_DWORD *)(v1 + 12);
    a1[14] -= *(_DWORD *)(v1 + 4);
    --a1[13];
    operator delete[](*(void **)v1);
    operator delete((void *)v1);
    return a1[13];
  }
  return result;
}

// ===== sub_444DC0 @ 0x00444DC0..0x00444F98 =====
int __thiscall sub_444DC0(_DWORD *this, char **a2, int a3)
{
  int v4; // eax
  int *v5; // eax
  _DWORD *v6; // edi
  int v7; // edx
  int *v8; // ebx
  int v9; // eax
  char *v10; // eax
  char *v11; // ebx
  const void **v12; // edi
  int j; // eax
  int v14; // eax
  int v16; // eax
  char *v17; // eax
  char *v18; // ecx
  _DWORD *v19; // eax
  int i; // [esp+8h] [ebp-10h]
  int *v21; // [esp+Ch] [ebp-Ch]
  const void **v22; // [esp+10h] [ebp-8h]
  int v23; // [esp+14h] [ebp-4h]
  int v24; // [esp+24h] [ebp+Ch]
  int v25; // [esp+24h] [ebp+Ch]

  if ( !a3 )
  {
    v16 = this[13];
    if ( v16 )
    {
      v17 = (char *)operator new[](16 * v16);
      *a2 = v17;
      v18 = v17;
      v19 = (_DWORD *)this[12];
      if ( v19 )
      {
        do
        {
          *(_DWORD *)v18 = *v19;
          *((_DWORD *)v18 + 1) = v19[1];
          *((_DWORD *)v18 + 2) = v19[2];
          *((_DWORD *)v18 + 3) = 0;
          v19 = (_DWORD *)v19[3];
          v18 += 16;
        }
        while ( v19 );
        return this[13];
      }
    }
    else
    {
      *a2 = 0;
    }
    return this[13];
  }
  v4 = this[25];
  if ( !v4 )
    return (*(int (__thiscall **)(_DWORD *, char **, _DWORD))(*this + 20))(this, a2, 0);
  v22 = (const void **)operator new[](4 * v4);
  v5 = (int *)operator new[](4 * this[25]);
  v6 = (_DWORD *)this[26];
  v21 = v5;
  v23 = this[13];
  v24 = 0;
  if ( this[25] )
  {
    v7 = (char *)v22 - (char *)v5;
    v8 = v5;
    for ( i = (char *)v22 - (char *)v5; ; v7 = i )
    {
      v9 = (*(int (__thiscall **)(_DWORD, int, _DWORD))(*(_DWORD *)*v6 + 20))(*v6, (int)v8 + v7, 0);
      v23 += v9;
      *v8 = v9;
      v6 = (_DWORD *)v6[3];
      ++v8;
      if ( (unsigned int)++v24 >= this[25] )
        break;
    }
  }
  v10 = (char *)operator new[](16 * v23);
  *a2 = v10;
  v11 = v10;
  v25 = 0;
  if ( this[25] )
  {
    v12 = v22;
    for ( j = (char *)v21 - (char *)v22; ; j = (char *)v21 - (char *)v22 )
    {
      memcpy_0(v11, *v12, 16 * *(_DWORD *)((char *)v12 + j));
      v11 += 16 * *(_DWORD *)((char *)v12 + (char *)v21 - (char *)v22);
      operator delete[]((void *)*v12++);
      if ( (unsigned int)++v25 >= this[25] )
        break;
    }
  }
  v14 = (*(int (__thiscall **)(_DWORD *, const void **))(*this + 20))(this, v22);
  *v21 = v14;
  memcpy_0(v11, *v22, 16 * v14);
  operator delete[]((void *)*v22);
  operator delete[](v22);
  operator delete[](v21);
  return v23;
}

// ===== sub_444FA0 @ 0x00444FA0..0x00444FA4 =====
int __usercall sub_444FA0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 112);
}

// ===== sub_444FB0 @ 0x00444FB0..0x00444FB5 =====
int __usercall sub_444FB0@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 112) &= ~1u;
  return result;
}

// ===== sub_444FC0 @ 0x00444FC0..0x00444FC4 =====
int __usercall sub_444FC0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 112) |= a2;
  return result;
}

// ===== sub_444FD0 @ 0x00444FD0..0x00444FD4 =====
int __usercall sub_444FD0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 120);
}

// ===== sub_444FE0 @ 0x00444FE0..0x00444FE7 =====
int __usercall sub_444FE0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(a2 + 120) = result;
  *(_DWORD *)(a2 + 124) = result;
  return result;
}

// ===== sub_444FF0 @ 0x00444FF0..0x00444FF7 =====
int __usercall sub_444FF0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 128);
}

// ===== sub_445000 @ 0x00445000..0x00445007 =====
int __usercall sub_445000@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 128) = a2;
  return result;
}

// ===== sub_445010 @ 0x00445010..0x00445024 =====
char __usercall sub_445010@<al>(_DWORD *a1@<eax>)
{
  int v1; // ecx
  int v2; // edx

  v1 = a1[31];
  a1[30] = v1;
  v2 = a1[30];
  a1[31] = v1 + 1;
  return *(_BYTE *)(a1[11] + v2);
}

// ===== sub_445030 @ 0x00445030..0x0044503E =====
int __thiscall sub_445030(_DWORD *this)
{
  int v1; // edx
  int result; // eax

  v1 = this[31];
  result = this[11];
  LOBYTE(result) = *(_BYTE *)(v1 + result);
  this[31] = v1 + 1;
  return result;
}

// ===== sub_445040 @ 0x00445040..0x00445051 =====
int __thiscall sub_445040(_DWORD *this)
{
  int v1; // edx
  int result; // eax

  v1 = this[31];
  result = *(unsigned __int16 *)(v1 + this[11]);
  this[31] = v1 + 2;
  return result;
}

// ===== sub_445060 @ 0x00445060..0x00445070 =====
int __thiscall sub_445060(_DWORD *this)
{
  int v1; // edx
  int result; // eax

  v1 = this[31];
  result = *(_DWORD *)(v1 + this[11]);
  this[31] = v1 + 4;
  return result;
}

// ===== sub_445070 @ 0x00445070..0x004450A9 =====
int __userpurge sub_445070@<eax>(size_t a1@<edi>, _DWORD *a2@<esi>, void *a3)
{
  unsigned int v3; // eax
  int v4; // edx

  v3 = sub_444C30(a2);
  if ( v4 + a1 > v3 )
    return 0;
  memcpy_0(a3, (const void *)(v4 + a2[11]), a1);
  a2[31] += a1;
  return 1;
}

// ===== sub_4450B0 @ 0x004450B0..0x004450C5 =====
int __thiscall sub_4450B0(_DWORD *this)
{
  int v1; // eax
  int v2; // eax

  v1 = this[29];
  if ( !v1 )
    v1 = this[4];
  v2 = v1 - 1;
  this[29] = v2;
  return *(_DWORD *)(this[6] + 4 * v2);
}

// ===== sub_4450D0 @ 0x004450D0..0x004450E8 =====
_DWORD *__usercall sub_4450D0@<eax>(_DWORD *result@<eax>, int a2@<esi>)
{
  *(_DWORD *)(result[6] + 4 * result[29]) = a2;
  result[29] = (unsigned int)(result[29] + 1) < result[4] ? result[29] + 1 : 0;
  return result;
}

// ===== sub_4450F0 @ 0x004450F0..0x00445104 =====
int __usercall sub_4450F0@<eax>(int a1@<eax>)
{
  *(_DWORD *)(a1 + 128) -= 4;
  return *(_DWORD *)(*(_DWORD *)(a1 + 128) + *(_DWORD *)(a1 + 76));
}

// ===== sub_445110 @ 0x00445110..0x00445124 =====
int __usercall sub_445110@<eax>(int result@<eax>, int a2@<esi>)
{
  *(_DWORD *)(*(_DWORD *)(result + 76) + *(_DWORD *)(result + 128)) = a2;
  *(_DWORD *)(result + 128) += 4;
  return result;
}

// ===== sub_445130 @ 0x00445130..0x00445149 =====
_DWORD *__usercall sub_445130@<eax>(int a1@<esi>)
{
  _DWORD *result; // eax

  result = operator new(8u);
  *result = *(_DWORD *)(a1 + 120);
  result[1] = *(_DWORD *)(a1 + 84);
  *(_DWORD *)(a1 + 84) = result;
  return result;
}

// ===== sub_445150 @ 0x00445150..0x00445181 =====
BOOL __stdcall sub_445150(int a1)
{
  _DWORD *v1; // eax
  BOOL v2; // esi
  int v3; // edi

  v1 = *(_DWORD **)(a1 + 84);
  v2 = v1 != 0;
  if ( v1 )
  {
    v3 = v1[1];
    operator delete(v1);
    *(_DWORD *)(a1 + 84) = v3;
  }
  return v2;
}

// ===== sub_445190 @ 0x00445190..0x004451B3 =====
int __fastcall sub_445190(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = *(_DWORD **)(a1 + 84);
  for ( result = 0; v2; ++result )
  {
    if ( a2 )
      *(_DWORD *)(a2 + 4 * result) = *v2;
    v2 = (_DWORD *)v2[1];
  }
  return result;
}

// ===== sub_4451C0 @ 0x004451C0..0x004451E8 =====
int __userpurge sub_4451C0@<eax>(int a1@<esi>, int a2)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx

  v2 = *(void (__thiscall ****)(_DWORD, int))(a1 + 88);
  if ( v2 )
    (**v2)(v2, 1);
  *(_DWORD *)(a1 + 88) = a2;
  return sub_444FC0(a1, 1);
}

// ===== sub_4451F0 @ 0x004451F0..0x0044522F =====
int __usercall sub_4451F0@<eax>(int a1@<esi>)
{
  int v1; // ecx
  int result; // eax
  int v3; // edi
  void (__thiscall ***v4)(_DWORD, int); // ecx

  v1 = *(_DWORD *)(a1 + 88);
  result = -1;
  if ( v1 )
  {
    result = (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 4))(v1);
    v3 = result;
    if ( result == 1 || result == -1 )
    {
      v4 = *(void (__thiscall ****)(_DWORD, int))(a1 + 88);
      if ( v4 )
        (**v4)(v4, 1);
      *(_DWORD *)(a1 + 88) = 0;
      sub_444FB0(a1);
      return v3;
    }
  }
  return result;
}

// ===== sub_445230 @ 0x00445230..0x00445253 =====
BOOL __userpurge sub_445230@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<edi>, int a4)
{
  int v4; // esi

  v4 = *(_DWORD *)(a3 + 88);
  if ( v4 )
    sub_431B40(v4, a4, a2, a1);
  return *(_DWORD *)(a3 + 88) != 0;
}

// ===== sub_445260 @ 0x00445260..0x00445281 =====
int __stdcall sub_445260(int a1)
{
  int v1; // eax

  v1 = sub_498720();
  return (*(_DWORD *)(a1 + 132) - v1) & ((*(_DWORD *)(a1 + 132) - v1 <= 0) - 1);
}

// ===== sub_445290 @ 0x00445290..0x004452A8 =====
int __stdcall sub_445290(int a1, int a2)
{
  int result; // eax

  result = a2 + sub_498720();
  *(_DWORD *)(a1 + 132) = result;
  return result;
}

// ===== sub_4452B0 @ 0x004452B0..0x004452B7 =====
int __usercall sub_4452B0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 132) += a2;
  return result;
}

// ===== sub_4452C0 @ 0x004452C0..0x004452F7 =====
_DWORD *__userpurge sub_4452C0@<eax>(int a1@<eax>, int a2)
{
  int v2; // esi
  int i; // eax
  _DWORD *result; // eax

  v2 = a1 + 92;
  for ( i = *(_DWORD *)(a1 + 96); i; i = *(_DWORD *)(i + 4) )
    v2 = i;
  result = operator new(8u);
  *result = a2;
  result[1] = 0;
  *(_DWORD *)(v2 + 4) = result;
  return result;
}

// ===== sub_445300 @ 0x00445300..0x00445328 =====
BOOL __usercall sub_445300@<eax>(int a1@<ecx>, _DWORD *a2@<edi>)
{
  _DWORD *v2; // eax
  BOOL v3; // esi

  v2 = *(_DWORD **)(a1 + 96);
  v3 = v2 != 0;
  if ( v2 )
  {
    *a2 = *v2;
    *(_DWORD *)(a1 + 96) = v2[1];
    operator delete(v2);
  }
  return v3;
}

// ===== sub_445330 @ 0x00445330..0x00445333 =====
void *__thiscall sub_445330(void *this)
{
  return this;
}

// ===== sub_445340 @ 0x00445340..0x0044547E =====
int __thiscall sub_445340(_DWORD *this, _DWORD *a2, _DWORD *a3, int a4, int a5, unsigned int a6)
{
  _DWORD *v7; // ebx
  unsigned int v8; // ecx
  unsigned int v9; // eax
  _DWORD *v10; // edi
  unsigned int v11; // eax
  _DWORD *v13; // eax
  int v14; // eax
  unsigned int k; // ecx
  _DWORD *v16; // eax
  int v17; // eax
  unsigned int m; // ecx
  int v19; // [esp+Ch] [ebp-10h]
  _DWORD *j; // [esp+10h] [ebp-Ch]
  _DWORD *i; // [esp+14h] [ebp-8h]
  unsigned int v22; // [esp+18h] [ebp-4h]

  v7 = (_DWORD *)this[26];
  v8 = this[7];
  for ( i = this + 26; v7; v8 = v9 )
  {
    v9 = v7[1];
    if ( a5 + v9 + v7[2] <= v8 )
      break;
    i = v7 + 3;
    v7 = (_DWORD *)v7[3];
  }
  v10 = (_DWORD *)this[27];
  v22 = this[15];
  for ( j = this + 27; v10; v22 = v11 )
  {
    v11 = v10[1];
    if ( a6 + v11 + v10[2] <= v22 )
      break;
    j = v10 + 3;
    v10 = (_DWORD *)v10[3];
  }
  v19 = v8 - a5;
  if ( (signed int)(v8 - a5) < this[14] )
    return -2147483646;
  if ( v22 < a6 )
    return -2147483645;
  ++this[25];
  v13 = operator new(0x10u);
  *v13 = a4;
  v13[1] = v19;
  v13[2] = a5;
  v13[3] = v7;
  *i = v13;
  *a2 = v13[1];
  v14 = this[26];
  for ( k = this[7]; v14; v14 = *(_DWORD *)(v14 + 12) )
  {
    if ( *(_DWORD *)(v14 + 4) < k )
      k = *(_DWORD *)(v14 + 4);
  }
  this[9] = k;
  v16 = operator new(0x10u);
  *v16 = a4;
  v16[1] = v22 - a6;
  v16[2] = a6;
  v16[3] = v10;
  *j = v16;
  *a3 = v16[1];
  v17 = this[27];
  for ( m = this[15]; v17; v17 = *(_DWORD *)(v17 + 12) )
  {
    if ( *(_DWORD *)(v17 + 4) < m )
      m = *(_DWORD *)(v17 + 4);
  }
  this[17] = m;
  return 0;
}

// ===== sub_445480 @ 0x00445480..0x004454F8 =====
int __thiscall sub_445480(_DWORD *this, int a2)
{
  _DWORD *v3; // ecx
  _DWORD *v4; // edx
  int result; // eax
  _DWORD *v6; // ecx
  _DWORD *v7; // edx

  v3 = (_DWORD *)this[26];
  v4 = this + 26;
  result = 0;
  if ( v3 )
  {
    while ( a2 != *v3 )
    {
      v4 = v3 + 3;
      v3 = (_DWORD *)v3[3];
      if ( !v3 )
        goto LABEL_6;
    }
    *v4 = v3[3];
    operator delete(v3);
    result = 1;
  }
LABEL_6:
  v6 = (_DWORD *)this[27];
  v7 = this + 27;
  if ( v6 )
  {
    while ( a2 != *v6 )
    {
      v7 = v6 + 3;
      v6 = (_DWORD *)v6[3];
      if ( !v6 )
        goto LABEL_9;
    }
    *v7 = v6[3];
    operator delete(v6);
    --this[25];
    return 1;
  }
  else
  {
LABEL_9:
    if ( result )
      --this[25];
  }
  return result;
}

// ===== sub_445500 @ 0x00445500..0x00445504 =====
int __usercall sub_445500@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 100);
}

// ===== sub_445510 @ 0x00445510..0x00445580 =====
int __cdecl sub_445510(_DWORD *a1, _DWORD *a2)
{
  unsigned int v4; // eax

  v4 = 16;
  while ( *a1 == *a2 )
  {
    v4 -= 4;
    ++a2;
    ++a1;
    if ( v4 < 4 )
      return 1;
  }
  return 0;
}

// ===== sub_4455A0 @ 0x004455A0..0x00445630 =====
_DWORD *__stdcall sub_4455A0(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  sub_406720((int)a1);
  *a1 = &DCArchiveComplex::`vftable';
  a1[207] = 0;
  a1[208] = 0;
  v1 = operator new(0x33Cu);
  if ( v1 )
    v2 = sub_406720((int)v1);
  else
    v2 = 0;
  a1[209] = v2;
  return a1;
}

// ===== sub_445630 @ 0x00445630..0x00445652 =====
void *__thiscall sub_445630(void *this, char a2)
{
  sub_445660(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_445660 @ 0x00445660..0x004456FD =====
void __stdcall sub_445660(int a1)
{
  int i; // edi
  void (__thiscall ***v2)(_DWORD, int); // ecx

  *(_DWORD *)a1 = &DCArchiveComplex::`vftable';
  for ( i = 0; i < *(_DWORD *)(a1 + 828); ++i )
    operator delete(*(void **)(*(_DWORD *)(a1 + 832) + 4 * i));
  operator delete(*(void **)(a1 + 832));
  v2 = *(void (__thiscall ****)(_DWORD, int))(a1 + 836);
  if ( v2 )
    (**v2)(v2, 1);
  sub_4067A0((char *)a1);
}

// ===== sub_445700 @ 0x00445700..0x00445821 =====
int __thiscall sub_445700(int this, int a2)
{
  int v3; // esi
  int v4; // ecx
  int v5; // ebx
  int v6; // eax
  char v8[780]; // [esp+Ch] [ebp-310h] BYREF

  v3 = -2147483632;
  if ( !sub_4459A0(v8) )
    return -2147483632;
  sub_42EA80(v4, v8);
  v5 = 0;
  if ( *(int *)(this + 828) > 0 )
  {
    do
    {
      if ( !strcmp(v8, *(const char **)(*(_DWORD *)(this + 832) + 4 * v5)) )
      {
        v3 = (*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(this + 836) + 4))(*(_DWORD *)(this + 836), a2);
        if ( !v3 )
          break;
      }
      ++v5;
    }
    while ( v5 < *(_DWORD *)(this + 828) );
    if ( v3 != -2147483632 )
      return v3;
  }
  v6 = sub_406CA0(this);
  if ( v6 )
    return (*(int (__thiscall **)(int, int))(*(_DWORD *)v6 + 4))(v6, a2);
  else
    return -2147483632;
}

// ===== sub_445830 @ 0x00445830..0x0044587A =====
int __userpurge sub_445830@<eax>(const char *a1@<edi>, int a2)
{
  int result; // eax
  int v3; // ecx

  result = 0;
  if ( a1 )
  {
    sub_406D30(a2);
    strcpy((char *)(a2 + 8), a1);
    sub_42EA80(v3, (_BYTE *)(a2 + 8));
    *(_DWORD *)(a2 + 4) = 1;
    sub_406D40(a2);
    return 1;
  }
  return result;
}

// ===== sub_445880 @ 0x00445880..0x00445966 =====
int __userpurge sub_445880@<eax>(int a1@<esi>, const char **a2)
{
  int result; // eax
  int v3; // ecx
  const char **v4; // ebx
  int v5; // edi
  const char *v6; // ecx
  _BYTE *v7; // edx
  char v8; // al
  int v9; // [esp+8h] [ebp-4h]

  result = 0;
  if ( a2 )
  {
    *(_DWORD *)(a1 + 828) = 0;
    if ( *a2 )
    {
      v3 = 0;
      do
        *(_DWORD *)(a1 + 828) = ++v3;
      while ( a2[v3] );
    }
    if ( *(int *)(a1 + 828) > 0 )
    {
      operator delete(*(void **)(a1 + 832));
      *(_DWORD *)(a1 + 832) = operator new(4 * *(_DWORD *)(a1 + 828));
      v9 = 0;
      if ( *a2 )
      {
        v4 = a2;
        v5 = 0;
        do
        {
          *(_DWORD *)(v5 * 4 + *(_DWORD *)(a1 + 832)) = operator new(&(*v4)[strlen(*v4) + 1] - *v4);
          v6 = *v4;
          v7 = *(_BYTE **)(v5 * 4 + *(_DWORD *)(a1 + 832));
          do
          {
            v8 = *v6;
            *v7++ = *v6++;
          }
          while ( v8 );
          sub_42EA80((int)v6, *(_BYTE **)(*(_DWORD *)(a1 + 832) + v5 * 4));
          v5 = v9 + 1;
          v4 = &a2[v5];
          ++v9;
        }
        while ( a2[v5] );
      }
      return 1;
    }
  }
  return result;
}

// ===== sub_445970 @ 0x00445970..0x0044599E =====
int __usercall sub_445970@<eax>(char *a1@<eax>, const char *a2@<esi>)
{
  int v2; // edx
  char v3; // cl
  char *v4; // eax

  v2 = a2 - a1;
  do
  {
    v3 = *a1;
    a1[v2] = *a1;
    ++a1;
  }
  while ( v3 );
  v4 = strrchr(a2, 92);
  if ( !v4 )
    return 0;
  *v4 = 0;
  return 1;
}

// ===== sub_4459A0 @ 0x004459A0..0x004459DB =====
int __userpurge sub_4459A0@<eax>(const char *a1@<eax>, int a2)
{
  char *v2; // eax
  char *v3; // eax
  int v4; // edx
  char v5; // cl

  v2 = strrchr(a1, 92);
  if ( !v2 )
    return 0;
  v3 = v2 + 1;
  v4 = a2 - (_DWORD)v3;
  do
  {
    v5 = *v3;
    v3[v4] = *v3;
    ++v3;
  }
  while ( v5 );
  return 1;
}

// ===== sub_4459E0 @ 0x004459E0..0x00445AC0 =====
BOOL __thiscall sub_4459E0(void *this, char *a2, char **a3)
{
  BOOL v4; // edi
  int v5; // ecx
  int v6; // eax
  char *v7; // esi
  int v8; // ecx
  char v10[780]; // [esp+10h] [ebp-61Ch] BYREF
  char v11[780]; // [esp+31Ch] [ebp-310h] BYREF

  v4 = 0;
  if ( sub_4459A0(a2, (int)v11) )
  {
    sub_406C70((int)this, (int)v10);
    sub_42EA80(v5, v11);
    v6 = strcmp(v11, v10);
    v4 = v6 == 0;
    if ( !v6 )
    {
      if ( a3 )
      {
        v7 = (char *)operator new(strlen(a2) + 1);
        sub_445970(a2, v7);
        sub_42EA80(v8, v7);
        *a3 = v7;
      }
    }
  }
  return v4;
}

// ===== sub_445AC0 @ 0x00445AC0..0x00445B57 =====
int __thiscall sub_445AC0(int this, char *a2)
{
  int v2; // edi
  int i; // [esp+Ch] [ebp-314h] BYREF
  char Buffer[780]; // [esp+10h] [ebp-310h] BYREF

  v2 = 0;
  for ( i = 0; v2 < *(_DWORD *)(this + 828); ++v2 )
  {
    sprintf(Buffer, "%s\\%s", a2, *(const char **)(*(_DWORD *)(this + 832) + 4 * v2));
    if ( sub_4068B0(*(void **)(this + 836), &i, (int)Buffer) )
      break;
  }
  operator delete(a2);
  return i;
}

// ===== sub_445B60 @ 0x00445B60..0x00445B78 =====
int __thiscall sub_445B60(void *this, int a2, int a3)
{
  return (*(int (__thiscall **)(void *, _DWORD, int, int))(*(_DWORD *)this + 20))(this, 0, a2, a3);
}

// ===== sub_445B80 @ 0x00445B80..0x00445C3A =====
int __thiscall sub_445B80(int this, int a2, int a3, char *a4)
{
  int v5; // edi
  int v7; // [esp+14h] [ebp-314h]
  char Buffer[780]; // [esp+18h] [ebp-310h] BYREF

  v5 = 0;
  v7 = 0;
  if ( *(int *)(this + 828) > 0 )
  {
    while ( 1 )
    {
      sprintf(Buffer, "%s\\%s", a4, *(const char **)(*(_DWORD *)(this + 832) + 4 * v5));
      if ( sub_406910(*(void **)(this + 836), a2, (int)Buffer, a3) )
        break;
      if ( ++v5 >= *(_DWORD *)(this + 828) )
        goto LABEL_6;
    }
    v7 = 1;
  }
LABEL_6:
  operator delete(a4);
  return v7;
}

// ===== sub_445C40 @ 0x00445C40..0x00445D1C =====
int __thiscall sub_445C40(int this, char *a2, int a3, char *a4)
{
  int v5; // edi
  int v7; // [esp+14h] [ebp-314h]
  char Buffer[780]; // [esp+18h] [ebp-310h] BYREF

  v5 = 0;
  v7 = 0;
  if ( *(int *)(this + 828) > 0 )
  {
    while ( 1 )
    {
      sprintf(Buffer, "%s\\%s", a4, *(const char **)(*(_DWORD *)(this + 832) + 4 * v5));
      if ( sub_406970(*(void **)(this + 836), (int)Buffer, a3) )
        break;
      if ( ++v5 >= *(_DWORD *)(this + 828) )
        goto LABEL_8;
    }
    if ( a2 )
      strcpy(a2, Buffer);
    v7 = 1;
  }
LABEL_8:
  operator delete(a4);
  return v7;
}

// ===== sub_445D20 @ 0x00445D20..0x00445DFC =====
unsigned int __thiscall sub_445D20(int this, int a2, int a3, int a4, int a5, char *a6)
{
  int v7; // ebx
  unsigned int v8; // eax
  unsigned int v9; // edi
  char Buffer[780]; // [esp+18h] [ebp-310h] BYREF

  v7 = 0;
  if ( *(int *)(this + 828) <= 0 )
  {
    v9 = a2;
  }
  else
  {
    do
    {
      sprintf(Buffer, "%s\\%s", a6, *(const char **)(*(_DWORD *)(this + 832) + 4 * v7));
      v8 = sub_406A70(*(void **)(this + 836), a2, (int)Buffer, a3, a4, a5);
      v9 = v8;
      if ( v8 < 0x80000000 )
        break;
      if ( v8 == -2147483600 )
        break;
      if ( v8 == -2147483584 )
        break;
      ++v7;
    }
    while ( v7 < *(_DWORD *)(this + 828) );
  }
  operator delete(a6);
  return v9;
}

// ===== sub_445E00 @ 0x00445E00..0x00445EA6 =====
unsigned int __thiscall sub_445E00(int this, int a2, char *a3)
{
  unsigned int v4; // ebx
  int v5; // esi
  char Buffer[780]; // [esp+14h] [ebp-310h] BYREF

  v4 = 0x80000000;
  v5 = 0;
  do
  {
    if ( v5 >= *(_DWORD *)(this + 828) )
      break;
    sprintf(Buffer, "%s\\%s", a3, *(const char **)(*(_DWORD *)(this + 832) + 4 * v5));
    v4 = sub_406AF0(*(void **)(this + 836), (int)Buffer, a2);
    ++v5;
  }
  while ( v4 >= 0x80000000 );
  operator delete(a3);
  return v4;
}

// ===== sub_445EB0 @ 0x00445EB0..0x00445ED6 =====
_DWORD *__usercall sub_445EB0@<eax>(_DWORD *result@<eax>, unsigned int a2@<ecx>)
{
  result[1] = a2;
  *result = &DCCache::`vftable';
  result[2] = 0;
  result[3] = a2 >> 1;
  result[4] = 0;
  result[5] = 0;
  result[6] = 0;
  result[7] = 0;
  result[8] = 0;
  result[9] = 0;
  return result;
}

// ===== sub_445EE0 @ 0x00445EE0..0x00445F01 =====
void *__thiscall sub_445EE0(void *this, char a2)
{
  sub_445F10();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_445F10 @ 0x00445F10..0x00445F31 =====
int __thiscall sub_445F10(_DWORD *this)
{
  int result; // eax

  *this = &DCCache::`vftable';
  do
    result = sub_446130(this);
  while ( result );
  return result;
}

// ===== sub_445F40 @ 0x00445F40..0x00446051 =====
int __stdcall sub_445F40(const char *a1, const char *a2, void *Src, size_t Size)
{
  int result; // eax
  _DWORD *v5; // ebx
  _DWORD *v6; // eax
  _DWORD *v7; // esi
  _BYTE *v8; // eax
  const char *v9; // ecx
  _BYTE *v10; // edx
  char v11; // al
  _BYTE *v12; // eax
  const char *v13; // ecx
  _BYTE *v14; // edx
  char v15; // al
  void *v16; // eax

  result = 0;
  v5 = (_DWORD *)dword_565D3C;
  if ( Size && Size <= *(_DWORD *)(dword_565D3C + 12) )
  {
    if ( Size + *(_DWORD *)(dword_565D3C + 8) > *(_DWORD *)(dword_565D3C + 4) )
    {
      do
      {
        sub_446100(v5);
        sub_446130(v5);
      }
      while ( Size + v5[2] > v5[1] );
    }
    v6 = operator new(0x18u);
    v7 = v6;
    if ( a1 )
    {
      v8 = operator new(strlen(a1) + 1);
      *v7 = v8;
      v9 = a1;
      v10 = v8;
      do
      {
        v11 = *v9;
        *v10++ = *v9++;
      }
      while ( v11 );
    }
    else
    {
      *v6 = 0;
    }
    if ( a2 )
    {
      v12 = operator new(strlen(a2) + 1);
      v7[1] = v12;
      v13 = a2;
      v14 = v12;
      do
      {
        v15 = *v13;
        *v14++ = *v13++;
      }
      while ( v15 );
    }
    else
    {
      v7[1] = 0;
    }
    v16 = operator new(Size);
    v7[2] = v16;
    memcpy_0(v16, Src, Size);
    v7[3] = Size;
    v7[4] = timeGetTime();
    v7[5] = v5[9];
    v5[2] += Size;
    v5[9] = v7;
    return 1;
  }
  return result;
}

// ===== sub_446060 @ 0x00446060..0x004460F3 =====
int __stdcall sub_446060(void *a1, _DWORD *a2, int a3, int a4)
{
  int v4; // edi
  int v5; // esi
  int result; // eax
  int v7; // ebx

  v4 = dword_565D3C;
  v5 = *(_DWORD *)(dword_565D3C + 36);
  result = 0;
  v7 = dword_565D3C + 16;
  if ( v5 )
  {
    while ( !sub_446190(*(_DWORD *)v5, a3) || !sub_446190(*(_DWORD *)(v5 + 4), a4) )
    {
      v7 = v5;
      v5 = *(_DWORD *)(v5 + 20);
      if ( !v5 )
        return 0;
    }
    if ( a1 )
      memcpy_0(a1, *(const void **)(v5 + 8), *(_DWORD *)(v5 + 12));
    *a2 = *(_DWORD *)(v5 + 12);
    *(_DWORD *)(v7 + 20) = *(_DWORD *)(v5 + 20);
    *(_DWORD *)(v5 + 20) = *(_DWORD *)(v4 + 36);
    *(_DWORD *)(v4 + 36) = v5;
    *(_DWORD *)(v5 + 16) = timeGetTime();
    return 1;
  }
  return result;
}

// ===== sub_446100 @ 0x00446100..0x00446124 =====
int __thiscall sub_446100(_DWORD *this)
{
  int v1; // ecx
  int result; // eax
  unsigned int i; // esi

  v1 = this[9];
  result = 0;
  for ( i = -1; v1; v1 = *(_DWORD *)(v1 + 20) )
  {
    if ( i > *(_DWORD *)(v1 + 16) )
    {
      i = *(_DWORD *)(v1 + 16);
      result = v1;
    }
  }
  return result;
}

// ===== sub_446130 @ 0x00446130..0x00446185 =====
int __usercall sub_446130@<eax>(int a1@<ecx>, int a2@<edi>)
{
  int v2; // esi
  int result; // eax
  int v4; // edx

  v2 = *(_DWORD *)(a1 + 36);
  result = 0;
  v4 = a1 + 16;
  if ( v2 )
  {
    while ( v2 != a2 )
    {
      v4 = v2;
      v2 = *(_DWORD *)(v2 + 20);
      if ( !v2 )
        return result;
    }
    *(_DWORD *)(v4 + 20) = *(_DWORD *)(v2 + 20);
    *(_DWORD *)(a1 + 8) -= *(_DWORD *)(v2 + 12);
    operator delete(*(void **)v2);
    operator delete(*(void **)(v2 + 4));
    operator delete(*(void **)(v2 + 8));
    operator delete((void *)v2);
    return 1;
  }
  return result;
}

// ===== sub_446190 @ 0x00446190..0x004461DB =====
BOOL __fastcall sub_446190(const char *a1, const char *a2)
{
  BOOL result; // eax

  result = 0;
  if ( !a2 )
    return a1 == 0;
  if ( a1 )
    return strcmp(a2, a1) == 0;
  return result;
}

// ===== sub_4461E0 @ 0x004461E0..0x00446350 =====
int __userpurge sub_4461E0@<eax>(int a1@<eax>, unsigned int a2)
{
  unsigned int v3; // ebx
  int v4; // edi
  void *v5; // eax
  int v6; // ebx
  unsigned int ThrdAddr; // [esp+Ch] [ebp-Ch] BYREF
  int v9; // [esp+10h] [ebp-8h]
  int v10; // [esp+14h] [ebp-4h]
  unsigned int v11; // [esp+20h] [ebp+8h]

  *(_DWORD *)(a1 + 16) = 0;
  *(_DWORD *)(a1 + 20) = 0;
  *(_DWORD *)(a1 + 24) = 0;
  *(_DWORD *)(a1 + 32) = 0;
  v3 = 1;
  *(_DWORD *)a1 = &DCDstrbtdPrcssng::`vftable';
  *(_DWORD *)(a1 + 4) = a2;
  *(_DWORD *)(a1 + 8) = a2;
  *(_DWORD *)(a1 + 12) = 2;
  *(_DWORD *)(a1 + 28) = 1;
  *(_DWORD *)(a1 + 36) = operator new(124 * a2);
  InitializeCriticalSection((LPCRITICAL_SECTION)(a1 + 40));
  if ( a2 > 1 )
  {
    v4 = 124;
    do
    {
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36)) = 0;
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 8) = v3;
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 12) = CreateEventA(0, 1, 1, 0);
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 16) = CreateEventA(0, 1, 0, 0);
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 20) = 0;
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 24) = a1;
      v10 = v4;
      v9 = 4;
      do
      {
        InitializeCriticalSection((LPCRITICAL_SECTION)(v10 + *(_DWORD *)(a1 + 36) + 28));
        v10 += 24;
        --v9;
      }
      while ( v9 );
      sub_446750(v3, 2);
      v5 = (void *)_beginthreadex(0, 0, sub_4468A0, (void *)(v4 + *(_DWORD *)(a1 + 36)), 0, &ThrdAddr);
      *(_DWORD *)(v4 + *(_DWORD *)(a1 + 36) + 4) = v5;
      SetThreadPriority(v5, 0);
      ++v3;
      v4 += 124;
    }
    while ( v3 < a2 );
    if ( a2 > 1 )
    {
      v11 = a2 - 1;
      v6 = 124;
      do
      {
        while ( !*(_DWORD *)(v6 + *(_DWORD *)(a1 + 36)) )
          Sleep(1u);
        v6 += 124;
        --v11;
      }
      while ( v11 );
    }
  }
  return a1;
}

// ===== sub_446350 @ 0x00446350..0x00446371 =====
void *__thiscall sub_446350(void *this, char a2)
{
  sub_446380();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_446380 @ 0x00446380..0x0044645C =====
void __thiscall sub_446380(char *this)
{
  unsigned int i; // edi
  int v3; // ebx
  int v4; // edi
  unsigned int v5; // [esp+Ch] [ebp-8h]
  int v6; // [esp+10h] [ebp-4h]

  *(_DWORD *)this = &DCDstrbtdPrcssng::`vftable';
  sub_446960(this, 0);
  *((_DWORD *)this + 7) = 0;
  for ( i = 1; i < *((_DWORD *)this + 1); ++i )
    sub_446780(i, 2);
  v5 = 1;
  if ( *((_DWORD *)this + 1) > 1u )
  {
    v3 = 124;
    do
    {
      while ( WaitForSingleObject(*(HANDLE *)(v3 + *((_DWORD *)this + 9) + 4), 1u) == 258 )
        ;
      CloseHandle(*(HANDLE *)(v3 + *((_DWORD *)this + 9) + 4));
      v4 = v3;
      v6 = 4;
      do
      {
        DeleteCriticalSection((LPCRITICAL_SECTION)(v4 + *((_DWORD *)this + 9) + 28));
        v4 += 24;
        --v6;
      }
      while ( v6 );
      CloseHandle(*(HANDLE *)(v3 + *((_DWORD *)this + 9) + 12));
      CloseHandle(*(HANDLE *)(v3 + *((_DWORD *)this + 9) + 16));
      v3 += 124;
      ++v5;
    }
    while ( v5 < *((_DWORD *)this + 1) );
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 40));
  operator delete(*((void **)this + 9));
}

// ===== sub_446460 @ 0x00446460..0x0044647F =====
void sub_446460()
{
  if ( !dword_565D64 )
  {
    InitializeCriticalSection(&stru_50EA40);
    dword_565D64 = 1;
  }
}

// ===== sub_4464C0 @ 0x004464C0..0x004464D0 =====
int __userpurge sub_4464C0@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 16) = a2;
  *(_DWORD *)(result + 24) = a3;
  return result;
}

// ===== sub_4464D0 @ 0x004464D0..0x004464E0 =====
int __userpurge sub_4464D0@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 20) = a2;
  *(_DWORD *)(result + 24) = a3;
  return result;
}

// ===== sub_4464E0 @ 0x004464E0..0x004465C9 =====
int __usercall sub_4464E0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  unsigned int i; // edi
  unsigned int j; // edi
  int result; // eax
  unsigned int k; // edi
  unsigned int m; // edi

  *(_DWORD *)(a2 + 32) = a1;
  if ( *(_DWORD *)(a2 + 4) > 1u && a1 )
  {
    sub_446960(a2, 1);
    for ( i = 1; i < *(_DWORD *)(a2 + 4); ++i )
    {
      sub_446750(i, 1);
      sub_446780(i, 2);
    }
    for ( j = 1; j < *(_DWORD *)(a2 + 4); ++j )
    {
      sub_446750(j, 3);
      sub_446780(j, 3);
    }
    while ( sub_4467C0() )
      ;
    result = sub_446960(a2, 0);
    for ( k = 1; k < *(_DWORD *)(a2 + 4); ++k )
    {
      sub_446750(k, 2);
      result = sub_446780(k, 1);
    }
    for ( m = 1; m < *(_DWORD *)(a2 + 4); ++m )
    {
      sub_446750(m, 0);
      result = sub_446780(m, 0);
    }
    *(_DWORD *)(a2 + 32) = 0;
  }
  else
  {
    do
      result = sub_4467C0();
    while ( result );
    *(_DWORD *)(a2 + 32) = 0;
  }
  return result;
}

// ===== sub_4465D0 @ 0x004465D0..0x004465ED =====
int __thiscall sub_4465D0(int this)
{
  int result; // eax

  result = 0;
  if ( *(_DWORD *)(this + 4) > 1u )
  {
    if ( *(_DWORD *)(this + 32) )
    {
      EnterCriticalSection((LPCRITICAL_SECTION)(this + 40));
      return 1;
    }
  }
  return result;
}

// ===== sub_4465F0 @ 0x004465F0..0x00446613 =====
void __userpurge sub_4465F0(int a1@<eax>, int a2)
{
  if ( *(_DWORD *)(a1 + 4) > 1u && *(_DWORD *)(a1 + 32) || a2 )
    LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 40));
}

// ===== sub_446620 @ 0x00446620..0x004466CB =====
int __usercall sub_446620@<eax>(unsigned int a1@<eax>, int a2@<esi>)
{
  int v3; // ebx
  unsigned int v4; // edx
  int v5; // eax
  int i; // ecx
  int v8; // edi

  v3 = 0;
  if ( a1 < *(_DWORD *)(a2 + 4) )
  {
    sub_4465D0(a2);
    v4 = *(_DWORD *)(a2 + 4);
    v5 = 0;
    if ( v4 )
    {
      for ( i = 0; v5 == a1 || *(_DWORD *)(*(_DWORD *)(a2 + 36) + i + 20); i += 124 )
      {
        if ( ++v5 >= v4 )
        {
          sub_4465F0(a2, 1);
          return 0;
        }
      }
      v8 = 124 * a1;
      v3 = 1;
      *(_DWORD *)(*(_DWORD *)(a2 + 36) + v8 + 20) = 1;
      sub_4465F0(a2, 1);
      WaitForSingleObjectEx(*(HANDLE *)(*(_DWORD *)(a2 + 36) + v8 + 16), 0xFFFFFFFF, 0);
      ResetEvent(*(HANDLE *)(*(_DWORD *)(a2 + 36) + v8 + 16));
      sub_4465D0(a2);
      *(_DWORD *)(*(_DWORD *)(a2 + 36) + v8 + 20) = 0;
    }
    sub_4465F0(a2, 1);
  }
  return v3;
}

// ===== sub_4466D0 @ 0x004466D0..0x00446744 =====
void __userpurge sub_4466D0(unsigned int a1@<eax>, int a2)
{
  unsigned int v3; // edi
  unsigned int v4; // esi
  unsigned int v5; // ecx
  int v6; // edx
  _DWORD *v7; // eax
  unsigned int v8; // [esp+14h] [ebp+8h]

  v3 = a1;
  if ( !a1 || a1 > *(_DWORD *)(a2 + 4) )
    v3 = *(_DWORD *)(a2 + 4);
  sub_4465D0(a2);
  v4 = 0;
  if ( v3 )
  {
    v8 = v3;
    do
    {
      v5 = *(_DWORD *)(a2 + 4);
      if ( v4 < v5 )
      {
        v6 = *(_DWORD *)(a2 + 36);
        v7 = (_DWORD *)(124 * v4 + v6 + 20);
        while ( !*v7 )
        {
          ++v4;
          v7 += 31;
          if ( v4 >= v5 )
            goto LABEL_12;
        }
        SetEvent(*(HANDLE *)(124 * v4++ + v6 + 16));
      }
LABEL_12:
      --v8;
    }
    while ( v8 );
  }
  sub_4465F0(a2, 1);
}

// ===== sub_446750 @ 0x00446750..0x00446779 =====
int __usercall sub_446750@<eax>(int a1@<edx>, unsigned int a2@<ecx>, int a3@<esi>)
{
  int result; // eax

  result = 0;
  if ( a2 )
  {
    if ( a2 < *(_DWORD *)(a3 + 4) )
    {
      EnterCriticalSection((LPCRITICAL_SECTION)(124 * a2 + 24 * a1 + *(_DWORD *)(a3 + 36) + 28));
      return 1;
    }
  }
  return result;
}

// ===== sub_446780 @ 0x00446780..0x004467A9 =====
int __usercall sub_446780@<eax>(int a1@<edx>, unsigned int a2@<ecx>, int a3@<esi>)
{
  int result; // eax

  result = 0;
  if ( a2 )
  {
    if ( a2 < *(_DWORD *)(a3 + 4) )
    {
      LeaveCriticalSection((LPCRITICAL_SECTION)(124 * a2 + 24 * a1 + *(_DWORD *)(a3 + 36) + 28));
      return 1;
    }
  }
  return result;
}

// ===== sub_4467B0 @ 0x004467B0..0x004467B4 =====
int __usercall sub_4467B0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 28);
}

// ===== sub_4467C0 @ 0x004467C0..0x00446807 =====
int __usercall sub_4467C0@<eax>(unsigned int a1@<edi>, int a2@<esi>)
{
  int result; // eax
  int (__cdecl *v3)(_DWORD); // eax

  result = 0;
  if ( *(_DWORD *)(a2 + 16) || *(_DWORD *)(a2 + 20) )
  {
    if ( a1 >= *(_DWORD *)(a2 + 8) )
      WaitForSingleObjectEx(*(HANDLE *)(124 * a1 + *(_DWORD *)(a2 + 36) + 12), 0xFFFFFFFF, 0);
    v3 = *(int (__cdecl **)(_DWORD))(a2 + 16);
    if ( v3 )
      return v3(*(_DWORD *)(a2 + 24));
    else
      return (*(int (__cdecl **)(_DWORD, unsigned int))(a2 + 20))(*(_DWORD *)(a2 + 24), a1);
  }
  return result;
}

// ===== sub_446810 @ 0x00446810..0x00446896 =====
int __userpurge sub_446810@<eax>(_DWORD *a1@<esi>, unsigned int a2)
{
  unsigned int v2; // ebx
  int result; // eax
  unsigned int v4; // eax
  int v5; // edi
  int v6; // edi
  unsigned int v7; // ebx

  v2 = a2;
  result = 0;
  if ( !a2 )
  {
    a2 = a1[1];
    v2 = a2;
  }
  if ( v2 && v2 <= a1[1] )
  {
    v4 = a1[2];
    if ( v4 <= v2 )
    {
      if ( v4 < v2 )
      {
        v6 = 124 * v4;
        v7 = v2 - v4;
        do
        {
          SetEvent(*(HANDLE *)(v6 + a1[9] + 12));
          v6 += 124;
          --v7;
        }
        while ( v7 );
        v2 = a2;
      }
      a1[2] = v2;
      return 1;
    }
    else
    {
      v5 = 124 * v2;
      do
      {
        ResetEvent(*(HANDLE *)(v5 + a1[9] + 12));
        ++v2;
        v5 += 124;
      }
      while ( v2 < a1[2] );
      a1[2] = a2;
      return 1;
    }
  }
  return result;
}

// ===== sub_4468A0 @ 0x004468A0..0x0044694A =====
void __stdcall __noreturn sub_4468A0(_DWORD *a1)
{
  int v1; // esi
  unsigned int v2; // edi

  v1 = a1[6];
  v2 = a1[2];
  sub_446750(0, v2, v1);
  while ( sub_4467B0(v1) )
  {
    if ( !sub_4467C0(v2, v1) )
    {
      sub_446750(1, v2, v1);
      sub_446780(1, v2, v1);
      sub_446750(3, v2, v1);
      sub_446780(0, v2, v1);
      if ( !*a1 )
        *a1 = 1;
      sub_446750(2, v2, v1);
      sub_446780(2, v2, v1);
      sub_446750(0, v2, v1);
      sub_446780(3, v2, v1);
    }
  }
  sub_446780(0, v2, v1);
  _endthreadex(0);
}

// ===== sub_446960 @ 0x00446960..0x00446ABB =====
void __cdecl sub_446960(int a1, int a2)
{
  char *v2; // edi
  void **v3; // ebx
  _DWORD *v4; // esi
  int v5; // eax
  int v6; // ecx
  _DWORD *v7; // eax
  int v8; // esi
  int v9; // ecx
  void *v10; // edx
  DWORD v11; // esi
  int *v12; // ebx
  _DWORD *v13; // esi
  int v14; // eax
  int v15; // edx
  DWORD v16; // edi
  unsigned int v17; // eax
  int v18; // ecx
  _DWORD *v19; // ecx
  _SYSTEM_INFO SystemInfo; // [esp+Ch] [ebp-34h] BYREF
  DWORD v21; // [esp+30h] [ebp-10h]
  _DWORD *v22; // [esp+34h] [ebp-Ch]
  unsigned int v23; // [esp+38h] [ebp-8h]
  unsigned int v24; // [esp+3Ch] [ebp-4h]
  DWORD v25; // [esp+4Ch] [ebp+Ch]

  EnterCriticalSection(&stru_50EA40);
  v2 = (char *)dword_565D68;
  v3 = &dword_565D68;
  v24 = 0;
  if ( dword_565D68 )
  {
    do
    {
      v4 = *(_DWORD **)v2;
      if ( a1 == *(_DWORD *)v2 )
      {
        if ( !a2 )
          sub_446810(v4, 0);
        *v3 = (void *)*((_DWORD *)v2 + 1);
        operator delete(v2);
      }
      else
      {
        sub_490AE0();
        v5 = sub_444B50((int)v4);
        v24 += v5 * v6;
        v3 = (void **)(v2 + 4);
      }
      v2 = (char *)*v3;
    }
    while ( *v3 );
  }
  if ( a2 )
  {
    v7 = operator new(8u);
    *v7 = a1;
    v7[1] = dword_565D68;
    v8 = sub_490AE0();
    v24 += sub_444B50(v9) * v8;
    dword_565D68 = v10;
  }
  if ( v24 )
  {
    v11 = 0;
    v25 = 0;
    v23 = 0;
    v22 = 0;
    GetSystemInfo(&SystemInfo);
    v12 = (int *)dword_565D68;
    if ( dword_565D68 )
    {
      do
      {
        v13 = (_DWORD *)*v12;
        sub_490AE0();
        v14 = sub_444B50((int)v13);
        v16 = SystemInfo.dwNumberOfProcessors * v15 * v14 / v24;
        if ( !v16 )
          v16 = 1;
        sub_446810(v13, v16);
        v25 += v16;
        v17 = sub_444B50(*v12);
        if ( v23 < v17 )
        {
          v23 = sub_444B50(v18);
          v21 = v16;
          v22 = v19;
        }
        v12 = (int *)v12[1];
      }
      while ( v12 );
      v11 = v25;
    }
    if ( v11 < SystemInfo.dwNumberOfProcessors )
      sub_446810(v22, SystemInfo.dwNumberOfProcessors + v21 - v11);
  }
  LeaveCriticalSection(&stru_50EA40);
}

// ===== sub_446AC0 @ 0x00446AC0..0x00446ACE =====
_DWORD *__usercall sub_446AC0@<eax>(_DWORD *result@<eax>)
{
  *result = &DCFlag::`vftable';
  result[1] = 0;
  return result;
}

// ===== sub_446AD0 @ 0x00446AD0..0x00446AF1 =====
void *__thiscall sub_446AD0(void *this, char a2)
{
  sub_446B00();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_446B00 @ 0x00446B00..0x00446B10 =====
int __thiscall sub_446B00(_DWORD *this)
{
  *this = &DCFlag::`vftable';
  return sub_446B40();
}

// ===== sub_446B10 @ 0x00446B10..0x00446B17 =====
unsigned int __usercall sub_446B10@<eax>(int a1@<eax>)
{
  return (unsigned int)(a1 + 7) >> 3;
}

// ===== sub_446B20 @ 0x00446B20..0x00446B24 =====
unsigned int __usercall sub_446B20@<eax>(unsigned int a1@<eax>)
{
  return a1 >> 3;
}

// ===== sub_446B30 @ 0x00446B30..0x00446B3B =====
int __fastcall sub_446B30(char a1)
{
  return 128 >> (a1 & 7);
}

// ===== sub_446B40 @ 0x00446B40..0x00446B73 =====
void __usercall sub_446B40(int a1@<edi>)
{
  int v1; // esi

  while ( *(_DWORD *)(a1 + 4) )
  {
    v1 = *(_DWORD *)(a1 + 4);
    *(_DWORD *)(a1 + 4) = *(_DWORD *)(v1 + 16);
    operator delete(*(void **)(v1 + 4));
    operator delete(*(void **)(v1 + 12));
    operator delete((void *)v1);
  }
}

// ===== sub_446B80 @ 0x00446B80..0x00446C7D =====
int __stdcall sub_446B80(size_t Size, const char *a2, int a3)
{
  size_t v3; // edi
  int v5; // esi
  void *v6; // esi
  _BYTE *v7; // edx
  const char *v8; // ecx
  char v9; // al
  void *v10; // ebx
  unsigned int v11; // eax
  size_t Sizea; // [esp+10h] [ebp+8h]

  v3 = sub_446B10(a3);
  if ( !v3 )
    return -2147483647;
  v5 = sub_447040(Size, a2);
  if ( v5 )
  {
    v10 = operator new(v3);
    v11 = sub_446B10(*(_DWORD *)(v5 + 4));
    Sizea = v3;
    if ( v3 >= v11 )
      Sizea = v11;
    memset(v10, 0, v3);
    memcpy_0(v10, *(const void **)(v5 + 8), Sizea);
    operator delete(*(void **)(v5 + 8));
    *(_DWORD *)(v5 + 4) = a3;
    *(_DWORD *)(v5 + 8) = v10;
    return 0;
  }
  else
  {
    v6 = operator new(0x14u);
    *(_DWORD *)v6 = sub_452AE0();
    *((_DWORD *)v6 + 1) = operator new(strlen(a2) + 1);
    *((_DWORD *)v6 + 2) = a3;
    *((_DWORD *)v6 + 3) = operator new(v3);
    v7 = (_BYTE *)*((_DWORD *)v6 + 1);
    *((_DWORD *)v6 + 4) = *(_DWORD *)(Size + 4);
    v8 = a2;
    do
    {
      v9 = *v8;
      *v7++ = *v8++;
    }
    while ( v9 );
    memset(*((void **)v6 + 3), 0, v3);
    *(_DWORD *)(Size + 4) = v6;
    return 0;
  }
}

// ===== sub_446C80 @ 0x00446C80..0x00446CC3 =====
int __userpurge sub_446C80@<eax>(int a1@<edi>, size_t Size, const char *a3, void *Src)
{
  int result; // eax
  int v5; // eax
  size_t v6; // [esp-4h] [ebp-Ch]

  result = sub_446B80(Size, a3, a1);
  if ( !result )
  {
    v6 = sub_446B10(a1);
    v5 = sub_447040(Size, a3);
    memcpy_0(*(void **)(v5 + 8), Src, v6);
    return 0;
  }
  return result;
}

// ===== sub_446CD0 @ 0x00446CD0..0x00446D3C =====
int __userpurge sub_446CD0@<eax>(int a1@<eax>, unsigned int a2@<esi>, int a3)
{
  int v3; // eax
  char v4; // al
  _BYTE *v5; // edx

  v3 = sub_447040(dword_56676C, a1);
  if ( !v3 )
    return -2147483646;
  if ( a2 >= *(_DWORD *)(v3 + 4) )
    return -2147483645;
  sub_446B20(a2);
  v4 = sub_446B30(a2);
  if ( a3 )
    *v5 |= v4;
  else
    *v5 &= ~v4;
  return 0;
}

// ===== sub_446D40 @ 0x00446D40..0x00446DD3 =====
int __userpurge sub_446D40@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4)
{
  int v5; // eax
  int v6; // esi
  unsigned int v7; // eax
  unsigned int v8; // ebx
  unsigned int i; // edx
  _BYTE *v10; // edi
  char v11; // dl
  char v12; // al
  int v13; // edx

  v5 = sub_447040(dword_56676C, a1);
  v6 = v5;
  if ( !v5 )
    return -2147483646;
  v7 = *(_DWORD *)(v5 + 4);
  if ( a2 >= v7 )
    return -2147483645;
  if ( (unsigned int)(a4 - 1) > 0xFFFF )
    return -2147483644;
  v8 = a2 + a4;
  if ( a2 + a4 > v7 )
    return -2147483644;
  for ( i = a2; i < v8; i = v13 + 1 )
  {
    v10 = (_BYTE *)(sub_446B20(i) + *(_DWORD *)(v6 + 8));
    v12 = sub_446B30(v11);
    if ( a3 )
      *v10 |= v12;
    else
      *v10 &= ~v12;
  }
  return 0;
}

// ===== sub_446DE0 @ 0x00446DE0..0x00446E3C =====
int __userpurge sub_446DE0@<eax>(int a1@<eax>, unsigned int a2@<esi>, _DWORD *a3)
{
  int v3; // eax
  int v4; // ecx
  int v5; // edi
  unsigned __int8 v6; // al
  int v7; // edx

  v3 = sub_447040(dword_56676C, a1);
  if ( !v3 )
    return -2147483646;
  if ( a2 >= *(_DWORD *)(v3 + 4) )
    return -2147483645;
  sub_446B20(a2);
  v5 = *(_DWORD *)(v4 + 8);
  v6 = sub_446B30(a2);
  *a3 = (v6 & *(_BYTE *)(v7 + v5)) != 0;
  return 0;
}

// ===== sub_446E40 @ 0x00446E40..0x00446EFC =====
int __stdcall sub_446E40(_DWORD *a1, int *a2)
{
  _DWORD *v2; // ecx
  _BYTE *v3; // edi
  int v4; // esi
  int v5; // edx
  char *v6; // ecx
  size_t v7; // ebx
  _BYTE *v8; // edx
  char v9; // al
  _DWORD *v10; // edi
  int v11; // ecx
  unsigned int v13; // [esp+8h] [ebp-Ch]
  int v14; // [esp+Ch] [ebp-8h]
  int v15; // [esp+10h] [ebp-4h]

  v2 = a1;
  v3 = a1;
  if ( a1 )
    v3 = a1 + 1;
  v4 = *(_DWORD *)(dword_56676C + 4);
  v5 = 4;
  v15 = 4;
  v14 = 0;
  if ( v4 )
  {
    do
    {
      v13 = strlen(*(const char **)(v4 + 4)) + 1;
      v7 = sub_446B10(*(_DWORD *)(v4 + 8));
      if ( v3 )
      {
        v8 = v3;
        do
        {
          v9 = *v6;
          *v8++ = *v6++;
        }
        while ( v9 );
        v10 = &v3[v13];
        *v10++ = *(_DWORD *)(v4 + 8);
        memcpy_0(v10, *(const void **)(v4 + 12), v7);
        v3 = (char *)v10 + v7;
      }
      v4 = *(_DWORD *)(v4 + 16);
      ++v14;
      v11 = v15 + v13 + v7 + 4;
      v15 = v11;
    }
    while ( v4 );
    v5 = v11;
    v2 = a1;
  }
  if ( v3 )
    *v2 = v14;
  *a2 = v5;
  return 0;
}

// ===== sub_446F00 @ 0x00446F00..0x0044703C =====
int __usercall sub_446F00@<eax>(_DWORD *a1@<eax>)
{
  int v1; // edi
  const char *v2; // esi
  int *v3; // ebx
  _BYTE *v4; // eax
  _BYTE *v5; // edx
  int v6; // eax
  int v7; // edx
  int *v8; // ecx
  int **v9; // edx
  int *v10; // esi
  int v11; // ecx
  int v12; // edx
  int v13; // eax
  int *v14; // esi
  void *v15; // ecx
  const char *v16; // edx
  int v17; // edi
  size_t v19; // [esp+Ch] [ebp-20h]
  int *v20; // [esp+10h] [ebp-1Ch]
  int v21; // [esp+18h] [ebp-14h]
  _BYTE *v22; // [esp+1Ch] [ebp-10h]
  _BYTE *v23; // [esp+20h] [ebp-Ch]
  int v24; // [esp+24h] [ebp-8h]
  int v25; // [esp+24h] [ebp-8h]
  int v26; // [esp+28h] [ebp-4h]
  int i; // [esp+28h] [ebp-4h]

  v1 = *a1;
  v19 = dword_56676C;
  v2 = (const char *)(a1 + 1);
  v23 = operator new(4 * *a1);
  v3 = (int *)operator new(4 * v1);
  v20 = v3;
  v4 = operator new(4 * v1);
  v5 = v4;
  v22 = v4;
  if ( v1 )
  {
    v6 = v23 - v4;
    v7 = v5 - (_BYTE *)v3;
    v8 = v3;
    v21 = v6;
    v26 = v7;
    v24 = v1;
    while ( 1 )
    {
      v9 = (int **)((char *)v8 + v7);
      *(int **)((char *)v9 + v6) = (int *)v2;
      v10 = (int *)&v2[strlen(v2) + 1];
      *v8 = *v10++;
      *v9 = v10;
      v2 = (char *)v10 + sub_446B10(*v8);
      v8 = (int *)(v11 + 4);
      if ( !--v24 )
        break;
      v6 = v21;
      v7 = v26;
    }
    v3 = v20;
    v5 = v22;
  }
  v25 = v1;
  if ( v1 )
  {
    v12 = v5 - (_BYTE *)v3;
    v13 = v23 - (_BYTE *)v3;
    v14 = &v3[v1];
    for ( i = v12; ; v12 = i )
    {
      v15 = *(void **)((char *)v14 + v12 - 4);
      v16 = *(const char **)((char *)v14 + v13 - 4);
      v17 = *(v14 - 1);
      --v25;
      --v14;
      sub_446C80(v17, v19, v16, v15);
      if ( !v25 )
        break;
      v13 = v23 - (_BYTE *)v3;
    }
  }
  operator delete(v23);
  operator delete(v3);
  operator delete(v22);
  return 0;
}

// ===== sub_447040 @ 0x00447040..0x004470D2 =====
int __stdcall sub_447040(int a1, const char *a2)
{
  int v2; // esi
  _DWORD *v3; // edi
  int v5; // [esp+Ch] [ebp-4h]

  v5 = sub_452AE0();
  v2 = *(_DWORD *)(a1 + 4);
  v3 = (_DWORD *)(a1 + 4);
  if ( !v2 )
    return 0;
  while ( v5 != *(_DWORD *)v2 || strcmp(a2, *(const char **)(v2 + 4)) )
  {
    v3 = (_DWORD *)(v2 + 16);
    v2 = *(_DWORD *)(v2 + 16);
    if ( !v2 )
      return 0;
  }
  *v3 = *(_DWORD *)(v2 + 16);
  *(_DWORD *)(v2 + 16) = *(_DWORD *)(a1 + 4);
  *(_DWORD *)(a1 + 4) = v2;
  return v2 + 4;
}

// ===== sub_4470E0 @ 0x004470E0..0x004470FE =====
int __usercall sub_4470E0@<eax>(unsigned int a1@<edx>, int a2@<esi>)
{
  int result; // eax
  char v3; // cl

  result = 0;
  do
  {
    v3 = a1;
    if ( a1 >= 0x80 )
      v3 = a1 & 0x7F | 0x80;
    *(_BYTE *)(result + a2) = v3;
    a1 >>= 7;
    ++result;
  }
  while ( a1 );
  return result;
}

// ===== sub_447100 @ 0x00447100..0x00447132 =====
int __usercall sub_447100@<eax>(int a1@<edi>, _DWORD *a2)
{
  int v2; // esi
  int result; // eax
  int v4; // ecx
  char v5; // dl
  int v6; // ebx

  v2 = 0;
  result = 0;
  v4 = 0;
  do
  {
    v5 = *(_BYTE *)(result + a1);
    v6 = (v5 & 0x7F) << v4;
    ++result;
    v4 += 7;
    v2 |= v6;
  }
  while ( v5 < 0 );
  *a2 = v2;
  return result;
}

// ===== sub_447140 @ 0x00447140..0x00447323 =====
int __usercall sub_447140@<eax>(int a1@<edi>, _DWORD *a2, _DWORD *a3, char *Src, size_t Size, unsigned int a6)
{
  size_t v7; // esi
  _BYTE *v8; // ebx
  unsigned int v9; // eax
  char *v10; // ecx
  unsigned int v11; // edi
  _BYTE *v12; // eax
  int v13; // ecx
  char v14; // dl
  _BYTE *v15; // eax
  int v16; // ecx
  char v17; // dl
  int v18; // eax
  size_t v19; // esi
  unsigned __int8 *v20; // ebx
  unsigned __int8 *v21; // eax
  unsigned __int8 *v22; // ecx
  int v23; // edi
  int v24; // edi
  unsigned int v26; // [esp+0h] [ebp-18h]
  int v27; // [esp+4h] [ebp-14h]
  size_t v28; // [esp+8h] [ebp-10h]
  size_t v29; // [esp+Ch] [ebp-Ch]
  char *v30; // [esp+10h] [ebp-8h]
  char *v31; // [esp+14h] [ebp-4h]

  if ( !a6 )
    return -2147483647;
  v7 = Size;
  if ( !Size )
    return -2147483646;
  v8 = Src;
  qmemcpy(a2, "DCFS FORMAT 1.00", 16);
  a2[5] = a6;
  a2[4] = Size;
  memcpy_0(a2 + 6, Src, Size);
  v29 = Size + 24;
  v31 = (char *)a2 + Size + 24;
  v9 = a6;
  v10 = &Src[Size];
  v30 = &Src[Size];
  if ( a6 > 1 )
  {
    v26 = a6 - 1;
    do
    {
      v27 = 0;
      v28 = v7;
      if ( v7 )
      {
        do
        {
          v11 = 0;
          if ( v27 )
          {
            if ( *v8 != *v10 )
            {
              v15 = v8;
              v16 = v10 - v8;
              do
              {
                if ( v11 >= v28 )
                  break;
                v17 = *++v15;
                ++v11;
              }
              while ( v17 != v15[v16] );
            }
          }
          else if ( *v8 == *v10 )
          {
            v12 = v8;
            v13 = v10 - v8;
            do
            {
              if ( v11 >= v28 )
                break;
              v14 = *++v12;
              ++v11;
            }
            while ( v14 == v12[v13] );
          }
          v18 = sub_4470E0(v11, (int)v31);
          v29 += v18;
          v31 += v18;
          if ( v27 )
          {
            memcpy_0(v31, v30, v11);
            v29 += v11;
            v31 += v11;
          }
          v30 += v11;
          v27 ^= 1u;
          v10 = v30;
          v8 += v11;
          v28 -= v11;
        }
        while ( v28 );
        v7 = Size;
      }
      --v26;
    }
    while ( v26 );
    v9 = a6;
  }
  v19 = v9 * v7;
  v20 = (unsigned __int8 *)operator new(v19);
  sub_447330(a1);
  v21 = (unsigned __int8 *)Src;
  v22 = v20;
  if ( v19 < 4 )
  {
LABEL_27:
    if ( !v19 )
      goto LABEL_36;
  }
  else
  {
    while ( *(_DWORD *)v22 == *(_DWORD *)v21 )
    {
      v19 -= 4;
      v21 += 4;
      v22 += 4;
      if ( v19 < 4 )
        goto LABEL_27;
    }
  }
  v23 = *v22 - *v21;
  if ( v23 )
    goto LABEL_35;
  if ( v19 <= 1 )
    goto LABEL_36;
  v23 = v22[1] - v21[1];
  if ( v23 )
    goto LABEL_35;
  if ( v19 <= 2 )
    goto LABEL_36;
  v23 = v22[2] - v21[2];
  if ( v23 )
  {
LABEL_35:
    v24 = (v23 >> 31) | 1;
    goto LABEL_37;
  }
  if ( v19 > 3 )
  {
    v23 = v22[3] - v21[3];
    goto LABEL_35;
  }
LABEL_36:
  v24 = 0;
LABEL_37:
  operator delete(v20);
  if ( v24 )
    return -1;
  *a3 = v29;
  return 0;
}

// ===== sub_447330 @ 0x00447330..0x0044744C =====
int __usercall sub_447330@<eax>(char *a1@<eax>, char *a2@<edx>)
{
  int *v2; // ecx
  unsigned int i; // esi
  size_t v4; // ebx
  unsigned int v5; // esi
  char *v6; // edi
  int v7; // eax
  size_t v8; // esi
  unsigned int v10; // [esp+Ch] [ebp-1Ch]
  int v11; // [esp+10h] [ebp-18h]
  size_t Size; // [esp+14h] [ebp-14h] BYREF
  size_t j; // [esp+18h] [ebp-10h]
  int v14; // [esp+1Ch] [ebp-Ch]
  void *Src; // [esp+20h] [ebp-8h]
  void *v16; // [esp+24h] [ebp-4h]

  v2 = &dword_4E5B34;
  for ( i = 16; i >= 4; i -= 4 )
  {
    if ( *(int *)((char *)v2 + a1 - (char *)&dword_4E5B34) != *v2 )
      return -2147483645;
    ++v2;
  }
  v4 = *((_DWORD *)a1 + 4);
  v5 = *((_DWORD *)a1 + 5);
  v10 = v5;
  if ( v4 && v5 )
  {
    v6 = &a1[v4 + 24];
    Src = a2;
    v16 = &a2[v4];
    memcpy_0(a2, a1 + 24, v4);
    v11 = 1;
    if ( v5 <= 1 )
      return 0;
    while ( 1 )
    {
      v14 = 0;
      for ( j = 0; j < v4; j += v8 )
      {
        v7 = sub_447100((int)v6, &Size);
        v8 = Size;
        v6 += v7;
        if ( v14 )
        {
          memcpy_0(v16, v6, Size);
          v6 += v8;
        }
        else if ( Size )
        {
          memcpy_0(v16, Src, Size);
        }
        v16 = (char *)v16 + v8;
        Src = (char *)Src + v8;
        v14 ^= 1u;
      }
      if ( j != v4 )
        break;
      if ( ++v11 >= v10 )
        return 0;
    }
  }
  return -2147483644;
}

// ===== sub_447450 @ 0x00447450..0x0044748C =====
int *__usercall sub_447450@<eax>(int a1@<eax>, int *a2@<esi>)
{
  void *v2; // eax
  size_t v4; // [esp-8h] [ebp-8h]

  if ( !a1 )
    a1 = 32;
  *a2 = a1;
  v2 = operator new(8 * a1);
  v4 = 8 * *a2;
  a2[1] = (int)v2;
  memset(v2, 0, v4);
  return a2;
}

// ===== sub_447490 @ 0x00447490..0x004474BB =====
void __usercall sub_447490(int a1@<eax>)
{
  unsigned int i; // esi

  for ( i = 0; i < *(_DWORD *)a1; ++i )
    sub_4475D0(i);
  operator delete(*(void **)(a1 + 4));
}

// ===== sub_4474C0 @ 0x004474C0..0x00447504 =====
int __userpurge sub_4474C0@<eax>(int *a1@<eax>, int *a2, void *Src, size_t Size)
{
  int v4; // ecx
  _DWORD *v5; // esi
  int v6; // esi
  int result; // eax

  v4 = 0;
  if ( !*a1 )
    goto LABEL_7;
  v5 = (_DWORD *)(a1[1] + 4);
  while ( *v5 )
  {
    ++v4;
    v5 += 2;
    if ( v4 >= (unsigned int)*a1 )
      goto LABEL_7;
  }
  v6 = v4;
  if ( v4 < 0 )
LABEL_7:
    v6 = *a1;
  result = sub_447510(Src, Size);
  if ( !result )
    *a2 = v6;
  return result;
}

// ===== sub_447510 @ 0x00447510..0x004475D0 =====
int __userpurge sub_447510@<eax>(unsigned int *a1@<eax>, unsigned int a2@<esi>, void *Src, size_t Size)
{
  int result; // eax
  unsigned int v6; // ebx
  char *v7; // [esp+4h] [ebp-4h]

  result = -2147483647;
  if ( Size )
  {
    v6 = *a1;
    if ( a2 < *a1 )
    {
      sub_4475D0(a2);
    }
    else
    {
      do
        v6 *= 2;
      while ( a2 >= v6 );
      v7 = (char *)operator new(8 * v6);
      memcpy_0(v7, (const void *)a1[1], 8 * *a1);
      memset(&v7[8 * *a1], 0, 8 * (v6 - *a1));
      operator delete((void *)a1[1]);
      a1[1] = (unsigned int)v7;
      *a1 = v6;
    }
    *(_DWORD *)(a1[1] + 8 * a2) = operator new(Size);
    *(_DWORD *)(a1[1] + 8 * a2 + 4) = Size;
    memcpy_0(*(void **)(a1[1] + 8 * a2), Src, Size);
    return 0;
  }
  return result;
}

// ===== sub_4475D0 @ 0x004475D0..0x00447607 =====
int __usercall sub_4475D0@<eax>(unsigned int a1@<ecx>, unsigned int *a2@<edi>)
{
  int result; // eax
  int v3; // esi
  unsigned int v4; // ecx
  unsigned int v5; // eax

  result = -2147483646;
  if ( a1 < *a2 )
  {
    v3 = 8 * a1;
    v4 = a2[1];
    if ( *(_DWORD *)(v4 + v3 + 4) )
    {
      operator delete(*(void **)(v4 + v3));
      v5 = a2[1];
      *(_DWORD *)(v5 + v3) = 0;
      *(_DWORD *)(v5 + v3 + 4) = 0;
      return 0;
    }
  }
  return result;
}

// ===== sub_447610 @ 0x00447610..0x0044764F =====
int __userpurge sub_447610@<eax>(void *a1@<ecx>, unsigned int *a2@<edi>, unsigned int a3@<esi>, _DWORD *a4)
{
  int result; // eax
  unsigned int v5; // edx

  result = -2147483646;
  if ( a3 < *a2 )
  {
    v5 = a2[1];
    if ( *(_DWORD *)(v5 + 8 * a3 + 4) )
    {
      if ( a1 )
        memcpy_0(a1, *(const void **)(v5 + 8 * a3), *(_DWORD *)(v5 + 8 * a3 + 4));
      *a4 = *(_DWORD *)(a2[1] + 8 * a3 + 4);
      return 0;
    }
  }
  return result;
}

// ===== sub_447650 @ 0x00447650..0x004476CD =====
int __userpurge sub_447650@<eax>(unsigned int *a1@<edi>, void *a2)
{
  _DWORD *v2; // ebx
  unsigned int v3; // eax
  int v4; // esi

  v2 = operator new(8 * *a1);
  v3 = 0;
  v4 = 0;
  if ( *a1 )
  {
    do
    {
      if ( *(_DWORD *)(a1[1] + 8 * v3 + 4) )
      {
        v2[2 * v4] = v3;
        v2[2 * v4++ + 1] = *(_DWORD *)(a1[1] + 8 * v3 + 4);
      }
      ++v3;
    }
    while ( v3 < *a1 );
    if ( v4 && a2 )
      memcpy_0(a2, v2, 8 * v4);
  }
  operator delete(v2);
  return v4;
}

// ===== sub_4476D0 @ 0x004476D0..0x0044770F =====
_DWORD *__usercall sub_4476D0@<eax>(int a1@<eax>, _DWORD *a2@<ecx>)
{
  _DWORD *v3; // ecx

  *a2 = &DCIndProc::`vftable';
  sub_41ACF0();
  v3[1] = ++dword_565D74;
  v3[3] = a1;
  v3[5] = 0;
  v3[6] = 0;
  v3[7] = 0;
  v3[8] = 0;
  v3[2] = 128;
  v3[4] = 1;
  return v3;
}

// ===== sub_447710 @ 0x00447710..0x00447731 =====
void *__thiscall sub_447710(void *this, char a2)
{
  sub_447740();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_447740 @ 0x00447740..0x00447788 =====
BOOL __thiscall sub_447740(_DWORD *this)
{
  *this = &DCIndProc::`vftable';
  while ( sub_447930(this) > 0 )
    ;
  return sub_41AD10((int)this);
}

// ===== sub_447790 @ 0x00447790..0x0044779B =====
void *sub_447790()
{
  void *result; // eax

  result = dword_56674C;
  dword_565D6C = (int)dword_56674C;
  return result;
}

// ===== sub_4477A0 @ 0x004477A0..0x004477AB =====
void *sub_4477A0()
{
  void *result; // eax

  result = dword_566750;
  dword_565D70 = (int)dword_566750;
  return result;
}

// ===== sub_4477B0 @ 0x004477B0..0x004477B4 =====
int __usercall sub_4477B0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 16);
}

// ===== sub_4477C0 @ 0x004477C0..0x004477D7 =====
int __thiscall sub_4477C0(_DWORD *this)
{
  sub_447860();
  if ( this[4] )
    sub_4478E0();
  return 0;
}

// ===== sub_4477E0 @ 0x004477E0..0x00447859 =====
int __userpurge sub_4477E0@<eax>(int a1@<ecx>, int a2@<edi>, void *Src)
{
  int result; // eax
  int v4; // eax
  int i; // ebx
  _DWORD *v6; // esi
  void *v7; // eax

  result = 0;
  if ( (unsigned int)(a2 - 1) <= 0xFF )
  {
    v4 = *(_DWORD *)(a1 + 32);
    for ( i = a1 + 24; v4; v4 = *(_DWORD *)(v4 + 8) )
      i = v4;
    v6 = operator new(0xCu);
    *v6 = a2;
    v7 = operator new[](4 * a2);
    v6[1] = v7;
    v6[2] = 0;
    memcpy_0(v7, Src, 4 * a2);
    *(_DWORD *)(i + 8) = v6;
    return 1;
  }
  return result;
}

// ===== sub_447860 @ 0x00447860..0x004478C8 =====
int __usercall sub_447860@<eax>(int a1@<esi>)
{
  int result; // eax
  _DWORD *v2; // [esp+0h] [ebp-408h]
  _DWORD v3[256]; // [esp+4h] [ebp-404h] BYREF

  while ( 1 )
  {
    result = sub_447930(v2);
    if ( result <= 0 )
      break;
    if ( v3[0] )
    {
      v2 = v3;
      (*(void (__thiscall **)(int, int))(*(_DWORD *)a1 + 8))(a1, result);
    }
    else if ( result == 2 )
    {
      sub_41ADE0(a1, v3[1]);
    }
  }
  return result;
}

// ===== sub_4478D0 @ 0x004478D0..0x004478D8 =====
int __usercall sub_4478D0@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 20) = 1;
  return result;
}

// ===== sub_4478E0 @ 0x004478E0..0x004478FE =====
void __usercall sub_4478E0(int a1@<esi>)
{
  if ( *(_DWORD *)(a1 + 20) && sub_447900(a1) )
    sub_461D80();
  *(_DWORD *)(a1 + 20) = 0;
}

// ===== sub_447900 @ 0x00447900..0x00447923 =====
BOOL __thiscall sub_447900(_DWORD **this)
{
  BOOL result; // eax
  unsigned int v2; // eax
  unsigned int v3; // ecx

  result = 0;
  if ( this[3] )
  {
    (*(void (__thiscall **)(_DWORD *))(*this[3] + 28))(this[3]);
    v2 = sub_443160(dword_565D6C);
    return v3 >= v2;
  }
  return result;
}

// ===== sub_447930 @ 0x00447930..0x00447977 =====
const void *__fastcall sub_447930(int a1, void *a2, int a3)
{
  const void **v3; // esi
  const void *result; // eax
  const void *v5; // edi

  v3 = *(const void ***)(a3 + 32);
  result = 0;
  if ( v3 )
  {
    v5 = *v3;
    memcpy_0(a2, v3[1], 4 * (_DWORD)*v3);
    *(_DWORD *)(a3 + 32) = v3[2];
    operator delete[]((void *)v3[1]);
    operator delete(v3);
    return v5;
  }
  return result;
}

// ===== sub_447980 @ 0x00447980..0x00447988 =====
int __stdcall sub_447980(int a1, int a2)
{
  return 1;
}

// ===== sub_447990 @ 0x00447990..0x00447AD9 =====
_DWORD *__thiscall sub_447990(void *this, _DWORD *a2)
{
  _DWORD *v3; // eax
  int v4; // eax
  int v5; // eax
  int v7; // [esp+10h] [ebp-14h] BYREF
  void *v8; // [esp+14h] [ebp-10h]
  int v9; // [esp+20h] [ebp-4h]

  sub_4476D0((int)this, a2);
  v9 = 0;
  *a2 = &DCIPIcon::`vftable';
  a2[9] = 0;
  sub_447B90();
  a2[12] = 0;
  a2[10] = this;
  v3 = operator new(0x134u);
  v8 = v3;
  LOBYTE(v9) = 1;
  if ( v3 )
    v4 = sub_42AC50(0, v3, a2[10]);
  else
    v4 = 0;
  LOBYTE(v9) = 0;
  a2[11] = v4;
  sub_41AB10(0);
  sub_41B310(&v7, a2[10]);
  (*(void (__thiscall **)(_DWORD, int, void *))(*(_DWORD *)a2[11] + 56))(a2[11], v7, v8);
  sub_41B360(&v7, a2[10]);
  sub_41B320((_DWORD *)a2[11], v7, (int)v8);
  v5 = sub_42E9A0(a2[10]);
  sub_41ADF0(a2[11], v5);
  sub_41AB40(a2[11], (_DWORD *)a2[10], 0, 0);
  a2[13] = 0;
  a2[14] = 0;
  a2[15] = -1;
  a2[16] = 0;
  a2[17] = 0;
  a2[18] = 0;
  a2[19] = 0;
  a2[20] = 0;
  a2[21] = 0;
  a2[22] = 0;
  a2[23] = 0;
  a2[29] = -1;
  a2[34] = 1;
  a2[36] = -1;
  a2[37] = 0;
  a2[38] = 0;
  a2[39] = 0;
  a2[40] = 0;
  return a2;
}

// ===== sub_447AE0 @ 0x00447AE0..0x00447B01 =====
void *__thiscall sub_447AE0(void *this, char a2)
{
  sub_447B10();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_447B10 @ 0x00447B10..0x00447B90 =====
BOOL __thiscall sub_447B10(_DWORD *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx

  *this = &DCIPIcon::`vftable';
  sub_44A050();
  sub_41AC40(this[10], this[11]);
  v2 = (void (__thiscall ***)(_DWORD, int))this[11];
  if ( v2 )
    (**v2)(v2, 1);
  while ( sub_44A220(this) )
    ;
  return sub_447740(this);
}

// ===== sub_447B90 @ 0x00447B90..0x00447BAF =====
int sub_447B90()
{
  int result; // eax

  if ( dword_507224 )
  {
    result = 0;
    memset(&unk_50EA58, 0, 0x180u);
    dword_507224 = 0;
  }
  return result;
}

// ===== sub_447BB0 @ 0x00447BB0..0x00447BE0 =====
BOOL __usercall sub_447BB0@<eax>(int a1@<esi>)
{
  int v1; // edx
  unsigned int v2; // edx
  BOOL result; // eax
  int v4; // ecx
  _DWORD *v5; // edx

  sub_447B90();
  v2 = v1 - 4;
  result = v2 < 4;
  if ( v2 < 4 )
  {
    v4 = 0;
    v5 = (_DWORD *)((char *)&unk_50EA58 + 96 * v2);
    do
      *v5++ = *(_DWORD *)(a1 + 4 * v4++);
    while ( v4 < 24 );
  }
  return result;
}

// ===== sub_447BF0 @ 0x00447BF0..0x00447C04 =====
int sub_447BF0()
{
  int result; // eax

  result = 1;
  if ( dword_565D84 )
    return sub_49A230();
  return result;
}

// ===== sub_447C10 @ 0x00447C10..0x00448311 =====
int __stdcall sub_447C10(int a1, int *a2)
{
  int v2; // edx
  int v3; // edi
  BOOL v4; // esi
  int *v5; // eax
  int v6; // edi
  int v7; // eax
  int v8; // ecx
  int v9; // esi
  int v10; // ecx
  int *v11; // eax
  int *v12; // edi
  int *v13; // ecx
  int *v14; // esi
  char *v15; // eax
  int *v16; // esi
  int *v17; // edx
  int v18; // ecx
  int v19; // ecx
  _DWORD *v20; // edi
  int v21; // eax
  int v22; // eax
  int *v23; // ecx
  _DWORD *v24; // eax
  int v25; // eax
  int v26; // edi
  int v27; // eax
  int v28; // eax
  int v29; // ecx
  int v30; // eax
  int v31; // eax
  int v32; // eax
  int v33; // esi
  int v34; // eax
  int v35; // esi
  int v36; // ecx
  int v37; // eax
  int v38; // eax
  int v39; // ecx
  int v40; // eax
  int v41; // edx
  int v42; // eax
  BOOL v43; // ecx
  int v44; // eax
  unsigned int v45; // eax
  int v47; // [esp-4h] [ebp-98h]
  int v48; // [esp-4h] [ebp-98h]
  int v49; // [esp-4h] [ebp-98h]
  _DWORD *v50; // [esp+10h] [ebp-84h]
  int *v51; // [esp+14h] [ebp-80h]
  int v52; // [esp+18h] [ebp-7Ch]
  int *v53; // [esp+18h] [ebp-7Ch]
  int v54; // [esp+1Ch] [ebp-78h]
  int *v55; // [esp+1Ch] [ebp-78h]
  _DWORD *v56; // [esp+20h] [ebp-74h]
  char *v57; // [esp+24h] [ebp-70h]
  int v58; // [esp+28h] [ebp-6Ch]
  int v59; // [esp+2Ch] [ebp-68h]
  int v60; // [esp+2Ch] [ebp-68h]
  int v61; // [esp+30h] [ebp-64h]
  int v62; // [esp+34h] [ebp-60h]
  int v63; // [esp+3Ch] [ebp-58h] BYREF
  int v64; // [esp+40h] [ebp-54h]
  _DWORD v65[2]; // [esp+44h] [ebp-50h] BYREF
  _DWORD v66[4]; // [esp+4Ch] [ebp-48h] BYREF
  int v67[4]; // [esp+5Ch] [ebp-38h] BYREF
  int v68[7]; // [esp+6Ch] [ebp-28h] BYREF
  int v69; // [esp+90h] [ebp-4h]

  (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 52))(a1);
  sub_42BBB0(*(_DWORD *)(a1 + 40));
  sub_42BBD0(0, v2, *(void **)(a1 + 40));
  sub_42B9B0(*(_DWORD *)(a1 + 40));
  sub_42B550(*(_DWORD *)(a1 + 40), 0);
  sub_42B540(*(_DWORD *)(a1 + 40), 1);
  sub_42CA00(*(_DWORD *)(a1 + 40));
  v3 = *a2;
  v59 = -2147483647;
  if ( *a2 > 0 && v3 <= 256 )
  {
    sub_42C2A0(v66, *(_DWORD **)(a1 + 40));
    v4 = 1;
    v52 = 0;
    *(_DWORD *)(a1 + 84) = operator new[](8 * v3);
    v5 = a2;
    v6 = 0;
    if ( *a2 <= 0 )
    {
LABEL_13:
      *(_DWORD *)(a1 + 52) = *v5;
      *(_DWORD *)(a1 + 56) = operator new[](52 * *v5);
      v10 = a2[2];
      if ( v10 < 0 || v10 >= *a2 )
        v10 = -1;
      *(_DWORD *)(a1 + 60) = v10;
      *(_DWORD *)(a1 + 64) = a2[3];
      *(_DWORD *)(a1 + 68) = a2[4];
      *(_DWORD *)(a1 + 72) = a2[5];
      *(_DWORD *)(a1 + 76) = a2[6] & 7;
      *(_DWORD *)(a1 + 80) = a2[7];
      *(_DWORD *)(a1 + 88) = v52;
      v11 = (int *)operator new[](20 * v52);
      v12 = a2;
      v13 = *(int **)(a1 + 56);
      *(_DWORD *)(a1 + 92) = v11;
      v14 = (int *)a2[1];
      v51 = v11;
      v53 = v13;
      v55 = v14;
      v57 = 0;
      v60 = 0;
      if ( *a2 > 0 )
      {
        while ( 1 )
        {
          *v53 = *v14;
          v15 = (char *)operator new[](60 * *v14);
          v16 = v53;
          v17 = v55;
          v53[1] = (int)v15;
          v18 = v55[2];
          if ( v18 < 0 || v18 >= *v55 )
            v18 = -1;
          v53[2] = v18;
          v53[3] = v55[3];
          v53[4] = v55[4];
          v53[5] = v55[5];
          v53[6] = v55[6];
          v53[7] = v55[7];
          v53[8] = v55[8];
          v53[9] = v55[9];
          v53[10] = v55[10];
          v53[11] = v55[11];
          v53[12] = v55[12];
          v50 = (_DWORD *)v55[1];
          v19 = 0;
          v58 = 0;
          if ( *v55 > 0 )
          {
            v20 = v15 + 8;
            v56 = v15 + 8;
            do
            {
              if ( v19 == v16[2] && !*v50 )
                v16[2] = -1;
              v21 = v50[3];
              if ( v21 != -1 && v19 == v16[2] && v50[4] != -1 )
                v21 = v50[4];
              if ( *v50 )
              {
                v22 = sub_407F20(dword_565D70, v21, v68);
                v16 = v53;
                v17 = v55;
              }
              else
              {
                v22 = 0;
              }
              v23 = v51;
              *v51 = v22;
              v51[1] = v60;
              v51[2] = v58;
              v51[3] = (int)(v20 - 2);
              v51[4] = 0;
              if ( v22 )
              {
                qmemcpy(v20 - 2, v50, 0x3Cu);
                v61 = v66[0] + v50[1];
                v62 = v66[1] + v50[2];
                sub_42B5B0(*(_DWORD **)(a1 + 40), v67, v61, v62, v68, 0, 0);
                v24 = operator new(0x134u);
                v69 = 0;
                if ( v24 )
                  v25 = sub_42AC50(v57, v24, *(_DWORD *)(a1 + 40));
                else
                  v25 = 0;
                v69 = -1;
                v47 = v68[3];
                v51[4] = v25;
                sub_41AB10(v47);
                v26 = *(_DWORD *)v51[4];
                v27 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 8))(*(_DWORD *)(a1 + 40));
                (*(void (__thiscall **)(int, int))(v26 + 4))(v51[4], v27);
                sub_41B310(&v63, *(_DWORD *)(a1 + 40));
                (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v51[4] + 56))(v51[4], v63, v64);
                sub_41B360(&v63, *(_DWORD *)(a1 + 40));
                sub_41B320((_DWORD *)v51[4], v63, v64);
                v28 = sub_42E9A0(*(_DWORD *)(a1 + 40));
                sub_41ADF0(v51[4], v28);
                sub_41AB40(v51[4], *(_DWORD **)(a1 + 40), v61, v62);
                v29 = v50[6];
                if ( v29 == -2 )
                {
                  sub_41BDD0(v51[4]);
                }
                else if ( sub_407F20(dword_565D70, v29, v68) )
                {
                  sub_41BC70(v51[4], (char *)v68);
                }
                sub_46D6E0(v51[4]);
                v30 = v50[14];
                if ( v30 )
                  sub_46DB40(v30 & 0x7FFFFFFF);
                v17 = v55;
                v16 = v53;
                v20 = v56;
                v23 = v51;
              }
              else
              {
                *(v20 - 1) = 0;
                *v20 = 0;
                v20[1] = -1;
                v20[2] = -1;
                v20[3] = -1;
                v20[4] = -1;
                v20[5] = 0;
                v20[6] = 0;
                v20[7] = -1;
                v20[8] = 0;
                v20[9] = 0;
                v20[10] = -1;
                v20[12] = 0;
              }
              v50 += 15;
              ++v57;
              v51 = v23 + 5;
              v19 = v58 + 1;
              v20 += 15;
              v56 = v20;
              v58 = v19;
            }
            while ( v19 < *v17 );
          }
          v55 = v17 + 13;
          v53 = v16 + 13;
          if ( ++v60 >= *a2 )
            break;
          v14 = v17 + 13;
        }
        v12 = a2;
      }
      (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(a1 + 40) + 48))(*(_DWORD *)(a1 + 40), v65);
      (*(void (__thiscall **)(_DWORD, _DWORD, _DWORD))(**(_DWORD **)(a1 + 40) + 44))(
        *(_DWORD *)(a1 + 40),
        v65[0],
        v65[1]);
      if ( *(_DWORD *)(a1 + 64) )
      {
        (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
        sub_46D6C0();
        v31 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
        sub_46D720(v31);
        v48 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
        v32 = sub_490AE0();
        sub_46E500(v32 | 0x80000000, v48);
        sub_449A60(a1, *(_DWORD *)(a1 + 60));
      }
      else if ( *(_DWORD *)(a1 + 68) )
      {
        sub_46D6E0(*(_DWORD *)(a1 + 40));
      }
      v33 = *(_DWORD *)(a1 + 40);
      v49 = (*(int (__thiscall **)(int))(*(_DWORD *)v33 + 28))(v33);
      (*(void (__thiscall **)(int))(*(_DWORD *)v33 + 28))(v33);
      sub_46DF00(v49);
      v34 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 44) + 28))(*(_DWORD *)(a1 + 44));
      sub_46DF00(v34);
      v35 = *(_DWORD *)a1;
      v37 = sub_449760(v36);
      if ( (*(int (__thiscall **)(int, int, int))(v35 + 28))(a1, v37, 1) )
      {
        v38 = *(_DWORD *)(a1 + 116);
        if ( v38 == -1 )
        {
          v42 = -1;
          v43 = 0;
        }
        else
        {
          v39 = *(_DWORD *)(a1 + 92);
          v40 = 5 * v38;
          v41 = *(_DWORD *)(v39 + 4 * v40 + 12);
          v42 = *(_DWORD *)(v39 + 4 * v40 + 8) | (*(_DWORD *)(v39 + 4 * v40 + 4) << 16);
          v43 = *(_DWORD *)(v41 + 20) != -1;
        }
        (*(void (__thiscall **)(int, int, BOOL))(*(_DWORD *)a1 + 16))(a1, v42, v43);
      }
      v44 = v12[2];
      if ( v44 == -1 || *(_DWORD *)(52 * v44 + v12[1] + 8) == -1 )
      {
        *(_DWORD *)(a1 + 124) = 0x7FFFFFFF;
        *(_DWORD *)(a1 + 120) = 0x7FFFFFFF;
      }
      else
      {
        sub_48E680(a1 + 120);
      }
      *(_DWORD *)(a1 + 128) = sub_4495C0(0, 0, 0);
      *(_DWORD *)(a1 + 132) = -1;
      *(_DWORD *)(a1 + 48) = 1;
      v59 = 0;
    }
    else
    {
      v54 = 0;
      while ( v4 )
      {
        v7 = v5[1];
        v8 = (unsigned __int16)HIWORD(*(_DWORD *)(v54 + v7));
        v9 = (unsigned __int16)*(_DWORD *)(v54 + v7);
        if ( v8 > v9 || !HIWORD(*(_DWORD *)(v54 + v7)) )
          v8 = (unsigned __int16)*(_DWORD *)(v54 + v7);
        v52 += v9;
        *(_DWORD *)(*(_DWORD *)(a1 + 84) + 8 * v6) = v8;
        *(_DWORD *)(*(_DWORD *)(a1 + 84) + 8 * v6 + 4) = (v9 + v8 - 1) / v8;
        *(_DWORD *)(v54 + a2[1]) = v9;
        ++v6;
        v4 = (unsigned int)(v9 - 1) <= 0xFF;
        v54 += 52;
        if ( v6 >= *a2 )
        {
          if ( v4 )
          {
            v5 = a2;
            goto LABEL_13;
          }
          break;
        }
        v5 = a2;
      }
      operator delete[](*(void **)(a1 + 84));
      v59 = -2147483646;
    }
  }
  sub_42C2F0(v67, *(_DWORD *)(a1 + 40));
  v45 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
  sub_443240(v67, v45, dword_565D6C);
  sub_4478D0(a1);
  return v59;
}
