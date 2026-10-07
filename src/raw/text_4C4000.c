#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4C4010 @ 0x004C4010..0x004C4064 =====
int __stdcall sub_4C4010(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  *a3 = 0;
  if ( sub_445510(a2, dword_4DB9B4) )
    return sub_4BCF30(a1 - 8, a3);
  else
    return sub_4C3EC0(a1, a2, a3);
}

// ===== sub_4C4070 @ 0x004C4070..0x004C40D1 =====
int __thiscall sub_4C4070(_DWORD **this, _DWORD *a2)
{
  _DWORD *v2; // edi
  int v4; // esi
  int v5; // [esp+4h] [ebp-4h] BYREF

  v2 = a2;
  *a2 = 0;
  if ( (*(int (__stdcall **)(_DWORD *, _DWORD **))(*this[6] + 24))(this[6], &a2) < 0 )
    return -2147467263;
  v4 = (*(int (__stdcall **)(_DWORD *, _DWORD *, int *))*a2)(a2, dword_4DB8E4, &v5);
  (*(void (__stdcall **)(_DWORD *))(*a2 + 8))(a2);
  if ( v4 < 0 )
    return -2147467263;
  *v2 = v5;
  return 0;
}

// ===== sub_4C40E0 @ 0x004C40E0..0x004C4141 =====
int __thiscall sub_4C40E0(_DWORD **this, _DWORD *a2)
{
  _DWORD *v2; // edi
  int v4; // esi
  int v5; // [esp+4h] [ebp-4h] BYREF

  v2 = a2;
  *a2 = 0;
  if ( (*(int (__stdcall **)(_DWORD *, _DWORD **))(*this[6] + 24))(this[6], &a2) < 0 )
    return -2147467263;
  v4 = (*(int (__stdcall **)(_DWORD *, _DWORD *, int *))*a2)(a2, dword_4DB9B4, &v5);
  (*(void (__stdcall **)(_DWORD *))(*a2 + 8))(a2);
  if ( v4 < 0 )
    return -2147467263;
  *v2 = v5;
  return 0;
}

// ===== sub_4C4150 @ 0x004C4150..0x004C4186 =====
int __stdcall sub_4C4150(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[3])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4190 @ 0x004C4190..0x004C41C6 =====
int __stdcall sub_4C4190(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[4])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C41D0 @ 0x004C41D0..0x004C4206 =====
int __stdcall sub_4C41D0(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[5])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4210 @ 0x004C4210..0x004C4246 =====
int __stdcall sub_4C4210(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[6])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4250 @ 0x004C4250..0x004C4286 =====
int __stdcall sub_4C4250(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[9])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4290 @ 0x004C4290..0x004C42C6 =====
int __stdcall sub_4C4290(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[7])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C42D0 @ 0x004C42D0..0x004C4306 =====
int __stdcall sub_4C42D0(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[8])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4310 @ 0x004C4310..0x004C4356 =====
int __stdcall sub_4C4310(_DWORD **a1, int a2, int a3, int a4, int a5, int a6)
{
  int result; // eax
  _DWORD **v7; // esi
  int v8; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v7 = a1;
    v8 = ((int (__stdcall *)(_DWORD **, int, int, int, int, int))(*a1)[13])(a1, a2, a3, a4, a5, a6);
    ((void (__stdcall *)(_DWORD **))(*v7)[2])(v7);
    return v8;
  }
  return result;
}

// ===== sub_4C4360 @ 0x004C4360..0x004C43A2 =====
int __stdcall sub_4C4360(_DWORD **a1, int a2, int a3, int a4, int a5)
{
  int result; // eax
  _DWORD **v6; // esi
  int v7; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v6 = a1;
    v7 = ((int (__stdcall *)(_DWORD **, int, int, int, int))(*a1)[14])(a1, a2, a3, a4, a5);
    ((void (__stdcall *)(_DWORD **))(*v6)[2])(v6);
    return v7;
  }
  return result;
}

// ===== sub_4C43B0 @ 0x004C43B0..0x004C43EA =====
int __stdcall sub_4C43B0(_DWORD **a1, int a2, int a3)
{
  int result; // eax
  _DWORD **v4; // esi
  int v5; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v4 = a1;
    v5 = ((int (__stdcall *)(_DWORD **, int, int))(*a1)[15])(a1, a2, a3);
    ((void (__stdcall *)(_DWORD **))(*v4)[2])(v4);
    return v5;
  }
  return result;
}

// ===== sub_4C43F0 @ 0x004C43F0..0x004C4422 =====
int __thiscall sub_4C43F0(_DWORD **this, int (__stdcall *a2)(_DWORD **, int), int a3)
{
  int result; // eax
  _DWORD **v4; // esi
  int v5; // edi
  _DWORD **v6; // [esp+0h] [ebp-4h] BYREF

  v6 = this;
  result = sub_4C40E0(this, &v6);
  if ( result >= 0 )
  {
    v4 = v6;
    v5 = a2(v6, a3);
    ((void (__stdcall *)(_DWORD **))(*v4)[2])(v4);
    return v5;
  }
  return result;
}

// ===== sub_4C4430 @ 0x004C4430..0x004C446A =====
int __stdcall sub_4C4430(_DWORD **a1, int a2, int a3)
{
  int result; // eax
  _DWORD **v4; // esi
  int v5; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v4 = a1;
    v5 = ((int (__stdcall *)(_DWORD **, int, int))(*a1)[16])(a1, a2, a3);
    ((void (__stdcall *)(_DWORD **))(*v4)[2])(v4);
    return v5;
  }
  return result;
}

// ===== sub_4C4470 @ 0x004C4470..0x004C44A6 =====
int __stdcall sub_4C4470(_DWORD **a1, int a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, int))(*a1)[18])(a1, a2);
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C44B0 @ 0x004C44B0..0x004C4502 =====
int __stdcall sub_4C44B0(_DWORD **a1, double a2)
{
  int result; // eax
  _DWORD **v3; // esi
  int v4; // edi

  if ( a2 == 0.0 )
    return -2147024809;
  result = sub_4C40E0(a1, &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = ((int (__stdcall *)(_DWORD **, _DWORD, _DWORD))(*a1)[17])(a1, LODWORD(a2), HIDWORD(a2));
    ((void (__stdcall *)(_DWORD **))(*v3)[2])(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4510 @ 0x004C4510..0x004C4549 =====
int __stdcall sub_4C4510(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 28))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4550 @ 0x004C4550..0x004C4589 =====
int __stdcall sub_4C4550(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 36))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4590 @ 0x004C4590..0x004C45CE =====
int __stdcall sub_4C4590(int a1, double a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, _DWORD, _DWORD))(*(_DWORD *)a1 + 32))(a1, LODWORD(a2), HIDWORD(a2));
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C45D0 @ 0x004C45D0..0x004C4609 =====
int __stdcall sub_4C45D0(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 40))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4610 @ 0x004C4610..0x004C464E =====
int __stdcall sub_4C4610(int a1, double a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, _DWORD, _DWORD))(*(_DWORD *)a1 + 44))(a1, LODWORD(a2), HIDWORD(a2));
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4650 @ 0x004C4650..0x004C4689 =====
int __stdcall sub_4C4650(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 48))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4690 @ 0x004C4690..0x004C46CE =====
int __stdcall sub_4C4690(int a1, double a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, _DWORD, _DWORD))(*(_DWORD *)a1 + 52))(a1, LODWORD(a2), HIDWORD(a2));
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C46D0 @ 0x004C46D0..0x004C4709 =====
int __stdcall sub_4C46D0(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 60))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4710 @ 0x004C4710..0x004C4765 =====
int __stdcall sub_4C4710(int a1, double a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  if ( a2 == 0.0 )
    return -2147024809;
  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, _DWORD, _DWORD))(*(_DWORD *)a1 + 56))(a1, LODWORD(a2), HIDWORD(a2));
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C4770 @ 0x004C4770..0x004C47A9 =====
int __stdcall sub_4C4770(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 64))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C47B0 @ 0x004C47B0..0x004C47E9 =====
int __stdcall sub_4C47B0(int a1, int a2)
{
  int result; // eax
  int v3; // esi
  int v4; // edi

  result = sub_4C4070((_DWORD **)(a1 - 4), &a1);
  if ( result >= 0 )
  {
    v3 = a1;
    v4 = (*(int (__stdcall **)(int, int))(*(_DWORD *)a1 + 68))(a1, a2);
    (*(void (__stdcall **)(int))(*(_DWORD *)v3 + 8))(v3);
    return v4;
  }
  return result;
}

// ===== sub_4C47F0 @ 0x004C47F0..0x004C489F =====
int __thiscall sub_4C47F0(int this, int a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int (__stdcall *v4)(int, _DWORD *, _DWORD *); // eax
  int v5; // ebx
  int v7; // edx
  int v8; // eax
  int v9; // ecx
  _DWORD v10[2]; // [esp+10h] [ebp-20h] BYREF
  _DWORD v11[6]; // [esp+18h] [ebp-18h] BYREF

  v3 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  v11[2] = this + 28;
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  v4 = *(int (__stdcall **)(int, _DWORD *, _DWORD *))(*(_DWORD *)a2 + 20);
  v11[5] = 0;
  v5 = v4(a2, v11, v10);
  if ( v5 >= 0 )
  {
    v7 = v11[1];
    v8 = v10[0];
    *(_DWORD *)(this + 56) = v11[0];
    v9 = v10[1];
    *(_DWORD *)(this + 60) = v7;
    *(_DWORD *)(this + 64) = v8;
    *(_DWORD *)(this + 68) = v9;
    *(_DWORD *)(this + 72) = 0;
    LeaveCriticalSection(v3);
    return 0;
  }
  else
  {
    LeaveCriticalSection(v3);
    return v5;
  }
}

// ===== sub_4C48A0 @ 0x004C48A0..0x004C4961 =====
int __thiscall sub_4C48A0(_DWORD *this, int a2, int a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // ebx
  int v6; // edi

  v4 = (struct _RTL_CRITICAL_SECTION *)(this + 7);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 7));
  if ( this[18] == 1 )
  {
    LeaveCriticalSection(v4);
    return -2147467259;
  }
  else
  {
    v6 = (*(int (__stdcall **)(_DWORD *, int, _DWORD, _DWORD, _DWORD, int *))(*this + 52))(
           this,
           a2,
           0,
           this[14],
           this[15],
           &dword_4DB8A4);
    if ( a3 )
    {
      if ( v6 >= 0 )
        v6 = (*(int (__stdcall **)(_DWORD *, int, _DWORD, _DWORD, _DWORD, int *))(*this + 52))(
               this,
               a3,
               0,
               this[16],
               this[17],
               &dword_4DB8A4);
    }
    LeaveCriticalSection(v4);
    return v6;
  }
}

// ===== sub_4C4970 @ 0x004C4970..0x004C499F =====
int __thiscall sub_4C4970(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  *(_DWORD *)(this + 56) = 0;
  *(_DWORD *)(this + 60) = 0;
  *(_DWORD *)(this + 64) = 0;
  *(_DWORD *)(this + 68) = 0;
  *(_DWORD *)(this + 72) = 1;
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C49A0 @ 0x004C49A0..0x004C49F9 =====
int __thiscall sub_4C49A0(char *this)
{
  int result; // eax
  int v3; // ebx
  int v4; // eax
  int v5; // ecx
  _DWORD v6[2]; // [esp+4h] [ebp-8h] BYREF

  if ( *((_DWORD *)this + 18) == 1 )
    return -2147467259;
  result = (*(int (__stdcall **)(char *, _DWORD *))(*(_DWORD *)this + 44))(this, v6);
  v3 = result;
  if ( result >= 0 )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
    v4 = v6[0];
    v5 = v6[1];
    *((_DWORD *)this + 16) = v6[0];
    *((_DWORD *)this + 17) = v5;
    *((_DWORD *)this + 14) = v4;
    *((_DWORD *)this + 15) = v5;
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 28));
    return v3;
  }
  return result;
}

// ===== sub_4C4A00 @ 0x004C4A00..0x004C4A09 =====
int __cdecl sub_4C4A00(int a1)
{
  return (*(int (**)(void))(*(_DWORD *)a1 + 40))();
}

// ===== sub_4C4A10 @ 0x004C4A10..0x004C4A19 =====
int __cdecl sub_4C4A10(int a1)
{
  return (*(int (**)(void))(*(_DWORD *)a1 + 44))();
}

// ===== sub_4C4A20 @ 0x004C4A20..0x004C4A29 =====
int __cdecl sub_4C4A20(int a1)
{
  return (*(int (**)(void))(*(_DWORD *)a1 + 76))();
}

// ===== sub_4C4A30 @ 0x004C4A30..0x004C4A39 =====
int __cdecl sub_4C4A30(int a1)
{
  return (*(int (**)(void))(*(_DWORD *)a1 + 48))();
}

// ===== sub_4C4A40 @ 0x004C4A40..0x004C4A64 =====
_DWORD *__thiscall sub_4C4A40(_DWORD *this, int a2, _DWORD *a3)
{
  sub_4BCF60(this + 1, a2, a3);
  this[4] = 0;
  return this;
}

// ===== sub_4C4A70 @ 0x004C4A70..0x004C4AB6 =====
_DWORD *__thiscall sub_4C4A70(_DWORD *this, int a2, _DWORD *a3, _DWORD *a4, int a5)
{
  _DWORD *result; // eax

  sub_4C4A40(this + 1, a2, a3);
  this[6] = a5;
  *this = &CPosPassThru::`vftable';
  this[1] = &CPosPassThru::`vftable';
  this[2] = &CPosPassThru::`vftable';
  result = this;
  if ( !a5 )
    *a4 = -2147467261;
  return result;
}

// ===== sub_4C4AC0 @ 0x004C4AC0..0x004C4AC8 =====
int __stdcall sub_4C4AC0(int a1, int a2)
{
  return -2147467259;
}

// ===== sub_4C4AD0 @ 0x004C4AD0..0x004C4ADA =====
int __stdcall sub_4C4AD0(int a1)
{
  return sub_4C4E00(a1 - 4);
}

// ===== sub_4C4AE0 @ 0x004C4AE0..0x004C4B7F =====
char *__thiscall sub_4C4AE0(int *this, char a2)
{
  int *v2; // esi

  v2 = this - 2;
  sub_4C3D60(this + 3);
  sub_4BCEE0();
  if ( (a2 & 1) != 0 )
    operator delete(v2);
  return (char *)v2;
}

// ===== sub_4C4B80 @ 0x004C4B80..0x004C4BB6 =====
int __stdcall sub_4C4B80(_DWORD **a1, int a2)
{
  if ( ((int (__thiscall *)(_DWORD **, int, _DWORD))(*a1)[20])(a1, a2, 0) < 0 )
    return sub_4C43F0(a1, (int (__stdcall *)(_DWORD **, int))sub_4C4A30, a2);
  else
    return 0;
}

// ===== sub_4C4BC0 @ 0x004C4BC0..0x004C4BD8 =====
int __stdcall sub_4C4BC0(_DWORD **a1, int a2)
{
  return sub_4C43F0(a1, (int (__stdcall *)(_DWORD **, int))sub_4C4A10, a2);
}

// ===== sub_4C4BE0 @ 0x004C4BE0..0x004C4BF8 =====
int __stdcall sub_4C4BE0(_DWORD **a1, int a2)
{
  return sub_4C43F0(a1, (int (__stdcall *)(_DWORD **, int))sub_4C4A00, a2);
}

// ===== sub_4C4C00 @ 0x004C4C00..0x004C4C18 =====
int __stdcall sub_4C4C00(_DWORD **a1, int a2)
{
  return sub_4C43F0(a1, (int (__stdcall *)(_DWORD **, int))sub_4C4A20, a2);
}

// ===== sub_4C4C20 @ 0x004C4C20..0x004C4C77 =====
int __thiscall sub_4C4C20(int this, int a2, _DWORD *a3, _DWORD *a4, int a5)
{
  sub_4C4A70((_DWORD *)this, a2, a3, a4, a5);
  *(_DWORD *)this = &CRendererPosPassThru::`vftable';
  *(_DWORD *)(this + 4) = &CRendererPosPassThru::`vftable';
  *(_DWORD *)(this + 8) = &CRendererPosPassThru::`vftable';
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 28));
  *(_DWORD *)(this + 56) = 0;
  *(_DWORD *)(this + 60) = 0;
  *(_DWORD *)(this + 64) = 0;
  *(_DWORD *)(this + 68) = 0;
  *(_DWORD *)(this + 72) = 1;
  return this;
}

// ===== sub_4C4C80 @ 0x004C4C80..0x004C4D09 =====
int *__thiscall sub_4C4C80(char *this, char a2)
{
  int *v2; // esi

  v2 = (int *)(this - 8);
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 20));
  sub_4C3D60(v2 + 5);
  sub_4BCEE0();
  if ( (a2 & 1) != 0 )
    operator delete(v2);
  return v2;
}

// ===== sub_4C4D10 @ 0x004C4D10..0x004C4D84 =====
int __stdcall sub_4C4D10(_DWORD *a1, _DWORD *a2)
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

// ===== sub_4C4D90 @ 0x004C4D90..0x004C4DB4 =====
HMODULE __thiscall sub_4C4D90(void **this)
{
  void **v2; // ecx

  v2 = this + 2;
  *this = &CAsyncOutputPin::`vftable';
  *v2 = &CAsyncOutputPin::`vftable';
  this[5] = &CAsyncOutputPin::`vftable';
  this[6] = &CAsyncOutputPin::`vftable';
  return sub_4BDDB0(v2);
}

// ===== sub_4C4DC0 @ 0x004C4DC0..0x004C4DDC =====
int __stdcall sub_4C4DC0(int a1, int a2, int a3)
{
  return (***(int (__stdcall ****)(_DWORD, int, int))(a1 + 12))(*(_DWORD *)(a1 + 12), a2, a3);
}

// ===== sub_4C4DE0 @ 0x004C4DE0..0x004C4DF5 =====
int __stdcall sub_4C4DE0(int a1)
{
  return (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 4))(*(_DWORD *)(a1 + 12));
}

// ===== sub_4C4E00 @ 0x004C4E00..0x004C4E15 =====
int __stdcall sub_4C4E00(int a1)
{
  return (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(a1 + 12) + 8))(*(_DWORD *)(a1 + 12));
}

// ===== sub_4C4E20 @ 0x004C4E20..0x004C4E33 =====
unsigned int __thiscall sub_4C4E20(_DWORD *this, int a2)
{
  this[40] = 0;
  return sub_4BE1B0(this, a2);
}

// ===== sub_4C4E40 @ 0x004C4E40..0x004C4E5B =====
int __thiscall sub_4C4E40(_DWORD *this, int a1)
{
  if ( this[40] )
    return sub_44DAE0(a1);
  else
    return -2147220890;
}

// ===== sub_4C4E60 @ 0x004C4E60..0x004C4E6F =====
int __thiscall sub_4C4E60(_DWORD *this)
{
  this[40] = 0;
  return sub_4BE5E0();
}

// ===== sub_4C4E70 @ 0x004C4E70..0x004C4EC2 =====
int __stdcall sub_4C4E70(int a1, _DWORD *a2, _DWORD *a3)
{
  if ( !a3 )
    return -2147467261;
  if ( !sub_4C4D10(a2, dword_4DBA04) )
    return sub_4BDE20(a1, a2, a3);
  *(_DWORD *)(a1 + 160) = 1;
  return sub_4BCF30(a1 - 8, a3);
}

// ===== sub_4C4ED0 @ 0x004C4ED0..0x004C4F08 =====
int __thiscall sub_4C4ED0(_DWORD *this, int a2, _DWORD *a3)
{
  if ( a2 < 0 )
    return -2147024809;
  if ( a2 > 0 )
    return 262403;
  sub_4C0B80(a3, this[38] + 416);
  return 0;
}

// ===== sub_4C4F10 @ 0x004C4F10..0x004C4F82 =====
int __thiscall sub_4C4F10(_DWORD *this, _DWORD *a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int v4; // esi
  _DWORD *v5; // esi

  v3 = (struct _RTL_CRITICAL_SECTION *)this[8];
  EnterCriticalSection(v3);
  v4 = this[38];
  if ( sub_4C4D10((_DWORD *)(v4 + 416), a2)
    && ((v5 = (_DWORD *)(v4 + 432), sub_4C4D10(v5, &dword_4DC2F8)) || sub_4C4D10(v5, a2 + 4)) )
  {
    LeaveCriticalSection(v3);
    return 0;
  }
  else
  {
    LeaveCriticalSection(v3);
    return 1;
  }
}

// ===== sub_4C4F90 @ 0x004C4F90..0x004C508C =====
int __stdcall sub_4C4F90(_DWORD *a1)
{
  _DWORD *v1; // eax
  _DWORD *v2; // esi
  int v4; // [esp+Ch] [ebp-10h] BYREF
  int v5; // [esp+18h] [ebp-4h]

  v4 = 0;
  *a1 = 0;
  v1 = operator new(0x64u);
  v5 = 0;
  if ( v1 )
    v2 = sub_4C06E0(v1, 0, 0, &v4);
  else
    v2 = 0;
  v5 = -1;
  if ( !v2 )
    return -2147024882;
  if ( v4 >= 0 )
  {
    v4 = (*(int (__stdcall **)(_DWORD *, const IID *, _DWORD *))v2[3])(v2 + 3, &stru_4DB974, a1);
    if ( v4 >= 0 )
    {
      return 0;
    }
    else
    {
      (*(void (__thiscall **)(_DWORD *, int))(*v2 + 12))(v2, 1);
      return -2147467262;
    }
  }
  else
  {
    (*(void (__thiscall **)(_DWORD *, int))(*v2 + 12))(v2, 1);
    return v4;
  }
}

// ===== sub_4C5090 @ 0x004C5090..0x004C51BD =====
int __stdcall sub_4C5090(int a1, int a2, int a3, _DWORD *a4)
{
  int v4; // edi
  int v5; // edi
  int result; // eax
  int v7; // edi
  int v8; // ebx
  int v9; // [esp+18h] [ebp-18h] BYREF
  _BYTE v10[8]; // [esp+1Ch] [ebp-14h] BYREF
  int v11; // [esp+24h] [ebp-Ch]

  v4 = *(_DWORD *)(a3 + 8);
  if ( !v4
    || (((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(*(_DWORD *)(a1 + 164) + 24) + 16))(*(_DWORD *)(*(_DWORD *)(a1 + 164) + 24))
       - 1) & v4) != 0 )
  {
    sub_4C5F20(a3 + 8);
  }
  if ( !a2
    || (*(int (__stdcall **)(int, int, _BYTE *))(*(_DWORD *)a2 + 12))(a2, a3, v10) < 0
    || (v5 = v11,
        (((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(*(_DWORD *)(a1 + 164) + 24) + 16))(*(_DWORD *)(*(_DWORD *)(a1 + 164) + 24))
        - 1) & v5) != 0) )
  {
    result = sub_4C4F90(&v9);
    if ( result >= 0 )
    {
      v7 = (*(int (__stdcall **)(int, int, _BYTE *))(*(_DWORD *)v9 + 12))(v9, a3, v10);
      if ( v7 < 0
        || (v8 = v11,
            (((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(*(_DWORD *)(a1 + 164) + 24) + 16))(*(_DWORD *)(*(_DWORD *)(a1 + 164) + 24))
            - 1) & v8) != 0) )
      {
        (*(void (__stdcall **)(int))(*(_DWORD *)v9 + 8))(v9);
        if ( v7 >= 0 )
          return -2147220978;
        return v7;
      }
      else
      {
        *a4 = v9;
        return 0;
      }
    }
  }
  else
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)a2 + 4))(a2);
    *a4 = a2;
    return 0;
  }
  return result;
}

// ===== sub_4C51C0 @ 0x004C51C0..0x004C52F4 =====
int __stdcall sub_4C51C0(int a1, int a2, int a3)
{
  int v3; // esi
  int result; // eax
  int v5; // ebx
  __int64 v6; // kr00_8
  __int64 v7; // kr08_8
  __int64 v8; // rax
  _BYTE v9[8]; // [esp+4h] [ebp-38h] BYREF
  unsigned int v10; // [esp+Ch] [ebp-30h]
  __int64 v11; // [esp+14h] [ebp-28h] BYREF
  __int64 v12; // [esp+1Ch] [ebp-20h] BYREF
  __int64 v13; // [esp+24h] [ebp-18h]
  __int64 v14; // [esp+2Ch] [ebp-10h] BYREF
  int v15; // [esp+34h] [ebp-8h] BYREF
  int v16; // [esp+38h] [ebp-4h] BYREF

  v3 = a2;
  result = (*(int (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)a2 + 20))(a2, &v11, &v12);
  if ( result >= 0 )
  {
    v13 = v11 / 10000000;
    v5 = (v12 - v11) / 10000000;
    sub_4C5E30(&v14, v9);
    v6 = v13 + v5;
    v10 = v13 + v5;
    if ( v6 > v14 )
    {
      sub_4C5F20(&v16);
      v7 = v14 + v16 - 1;
      v8 = ~(v16 - 1);
      v14 = v8 & v7;
      if ( __SPAIR64__(HIDWORD(v6), v10) > (v8 & v7) )
      {
        v5 = (v8 & v7) - v13;
        v12 = 10000000 * (v8 & v7);
        (*(void (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)a2 + 24))(a2, &v11, &v12);
      }
      v3 = a2;
    }
    result = (*(int (__stdcall **)(int, int *))(*(_DWORD *)v3 + 12))(v3, &v15);
    if ( result >= 0 )
      return sub_4C6690(*(LPVOID *)(a1 + 164), v13, SHIDWORD(v13), v5, 1, v15, v3, a3);
  }
  return result;
}

// ===== sub_4C5300 @ 0x004C5300..0x004C5442 =====
int __stdcall sub_4C5300(int a1, int a2)
{
  int v2; // esi
  int result; // eax
  int v4; // ebx
  __int64 v5; // kr00_8
  __int64 v6; // kr08_8
  __int64 v7; // rax
  int v8; // edi
  _BYTE v9[8]; // [esp+4h] [ebp-3Ch] BYREF
  unsigned int v10; // [esp+Ch] [ebp-34h]
  __int64 v11; // [esp+14h] [ebp-2Ch] BYREF
  __int64 v12; // [esp+1Ch] [ebp-24h] BYREF
  __int64 v13; // [esp+24h] [ebp-1Ch]
  __int64 v14; // [esp+2Ch] [ebp-14h] BYREF
  int v15; // [esp+34h] [ebp-Ch] BYREF
  int v16; // [esp+38h] [ebp-8h] BYREF
  int v17; // [esp+3Ch] [ebp-4h] BYREF

  v2 = a2;
  result = (*(int (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)a2 + 20))(a2, &v11, &v12);
  if ( result >= 0 )
  {
    v13 = v11 / 10000000;
    v4 = (v12 - v11) / 10000000;
    sub_4C5E30(&v14, v9);
    v5 = v13 + v4;
    v10 = v13 + v4;
    if ( v5 > v14 )
    {
      sub_4C5F20(&v17);
      v6 = v14 + v17 - 1;
      v7 = ~(v17 - 1);
      v14 = v7 & v6;
      if ( __SPAIR64__(HIDWORD(v5), v10) > (v7 & v6) )
      {
        v4 = (v7 & v6) - v13;
        v12 = 10000000 * (v7 & v6);
        (*(void (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)a2 + 24))(a2, &v11, &v12);
      }
      v2 = a2;
    }
    result = (*(int (__stdcall **)(int, int *))(*(_DWORD *)v2 + 12))(v2, &v16);
    if ( result >= 0 )
    {
      v8 = sub_4C5DA0(v13, HIDWORD(v13), v4, v16, &v15, v2);
      (*(void (__stdcall **)(int, int))(*(_DWORD *)v2 + 48))(v2, v15);
      return v8;
    }
  }
  return result;
}

// ===== sub_4C5450 @ 0x004C5450..0x004C54A2 =====
int __stdcall sub_4C5450(int a1, DWORD dwMilliseconds, _DWORD *a3, int a4)
{
  int v4; // esi

  v4 = sub_4C62D0(dwMilliseconds, (int)&a4, a4, (int)&dwMilliseconds);
  if ( v4 >= 0 )
    (*(void (__stdcall **)(int, DWORD))(*(_DWORD *)a4 + 48))(a4, dwMilliseconds);
  *a3 = a4;
  return v4;
}

// ===== sub_4C54B0 @ 0x004C54B0..0x004C54D5 =====
int __stdcall sub_4C54B0(int a1, int a2, int a3, int a4, int a5)
{
  return sub_4C5E90(a2, a3, a4, a5);
}

// ===== sub_4C54E0 @ 0x004C54E0..0x004C54FD =====
int __stdcall sub_4C54E0(int a1, int a2, int a3)
{
  return sub_4C5E30(a2, a3);
}

// ===== sub_4C5500 @ 0x004C5500..0x004C5515 =====
int __stdcall sub_4C5500(int a1)
{
  return sub_4C63D0(*(_DWORD *)(a1 + 164));
}

// ===== sub_4C5520 @ 0x004C5520..0x004C5535 =====
int __stdcall sub_4C5520(int a1)
{
  return sub_4C6000(*(_DWORD *)(a1 + 164));
}

// ===== sub_4C5540 @ 0x004C5540..0x004C555F =====
int __stdcall sub_4C5540(int a1, int a2, int a3)
{
  return (*(int (__thiscall **)(_DWORD, int, int))(**(_DWORD **)(a1 + 140) + 36))(*(_DWORD *)(a1 + 140), a2, a3);
}

// ===== sub_4C5560 @ 0x004C5560..0x004C55F2 =====
HMODULE __thiscall sub_4C5560(char *this)
{
  int v3; // [esp+0h] [ebp-18h]
  int v4; // [esp+4h] [ebp-14h]

  *(_DWORD *)this = &CAsyncReader::`vftable';
  *((_DWORD *)this + 3) = &CAsyncReader::`vftable';
  *((_DWORD *)this + 4) = &CAsyncReader::`vftable';
  sub_4C0AA0(this + 416);
  sub_4C4D90((void **)this + 60);
  sub_4C65B0((LPCRITICAL_SECTION)(this + 104), v3, v4);
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 80));
  return sub_4BD150((int)this);
}

// ===== sub_4C5600 @ 0x004C5600..0x004C5633 =====
char *__thiscall sub_4C5600(char *this, int a2)
{
  if ( (*(int (__thiscall **)(char *))(*(_DWORD *)this + 24))(this) <= 0 || a2 || this == (char *)-240 )
    return 0;
  else
    return this + 248;
}

// ===== sub_4C5640 @ 0x004C5640..0x004C564A =====
int __stdcall sub_4C5640(int a1)
{
  return sub_4C4DE0(a1 - 24);
}

// ===== sub_4C5650 @ 0x004C5650..0x004C565A =====
int __stdcall sub_4C5650(int a1)
{
  return sub_4C5520(a1 - 20);
}

// ===== sub_4C5660 @ 0x004C5660..0x004C566A =====
int __stdcall sub_4C5660(int a1)
{
  return sub_4C5500(a1 - 20);
}

// ===== sub_4C5670 @ 0x004C5670..0x004C567A =====
int __stdcall sub_4C5670(int a1, int a2, int a3)
{
  return sub_4C4DC0(a1 - 20, a2, a3);
}

// ===== sub_4C5680 @ 0x004C5680..0x004C568A =====
int __stdcall sub_4C5680(int a1, int a2, int a3)
{
  return sub_4C4DC0(a1 - 24, a2, a3);
}

// ===== sub_4C5690 @ 0x004C5690..0x004C5698 =====
int __thiscall sub_4C5690(char *this, char a2)
{
  return sub_4C5730(this - 8, a2);
}

// ===== sub_4C56A0 @ 0x004C56A0..0x004C56AA =====
int __stdcall sub_4C56A0(int a1)
{
  return sub_4C4E00(a1 - 20);
}

// ===== sub_4C56B0 @ 0x004C56B0..0x004C56BA =====
int __stdcall sub_4C56B0(int a1)
{
  return sub_4C4E00(a1 - 24);
}

// ===== sub_4C56C0 @ 0x004C56C0..0x004C56CA =====
int __stdcall sub_4C56C0(int a1)
{
  return sub_4C4DE0(a1 - 20);
}

// ===== sub_4C56D0 @ 0x004C56D0..0x004C5728 =====
_DWORD *__thiscall sub_4C56D0(_DWORD *this, int a2, int a3, int a4, int a5)
{
  sub_4C0050((int)(this + 2), 0, a3, a5, a2, L"Output", 1);
  this[40] = a3;
  *this = &CAsyncOutputPin::`vftable';
  this[2] = &CAsyncOutputPin::`vftable';
  this[5] = &CAsyncOutputPin::`vftable';
  this[6] = &CAsyncOutputPin::`vftable';
  this[41] = a4;
  return this;
}

// ===== sub_4C5730 @ 0x004C5730..0x004C5751 =====
void **__thiscall sub_4C5730(void **this, char a2)
{
  sub_4C4D90(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C5760 @ 0x004C5760..0x004C5814 =====
int __thiscall sub_4C5760(int this, int a2, _DWORD *a3, int a4, int a5)
{
  struct _RTL_CRITICAL_SECTION *v6; // edi

  v6 = (struct _RTL_CRITICAL_SECTION *)(this + 80);
  sub_4BFB00((_DWORD *)this, a2, a3, this + 80, dword_4DB864, 0);
  *(_DWORD *)this = &CAsyncReader::`vftable';
  *(_DWORD *)(this + 12) = &CAsyncReader::`vftable';
  *(_DWORD *)(this + 16) = &CAsyncReader::`vftable';
  InitializeCriticalSection(v6);
  sub_4C5F40((LPCRITICAL_SECTION)(this + 104), a4);
  sub_4C56D0((_DWORD *)(this + 240), a5, this, this + 104, (int)v6);
  sub_4C0AB0((void *)(this + 416));
  return this;
}

// ===== sub_4C5820 @ 0x004C5820..0x004C5841 =====
char *__thiscall sub_4C5820(char *this, char a2)
{
  sub_4C5560(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C5850 @ 0x004C5850..0x004C5857 =====
void __thiscall sub_4C5850(_DWORD *this)
{
  *this = &CAsyncStream::`vftable';
}

// ===== sub_4C5860 @ 0x004C5860..0x004C5882 =====
_DWORD *__thiscall sub_4C5860(_DWORD *this, char a2)
{
  *this = &CAsyncStream::`vftable';
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C5890 @ 0x004C5890..0x004C5919 =====
void __thiscall sub_4C5890(char *this)
{
  void *v2; // eax

  *(_DWORD *)this = &CMemStream::`vftable';
  v2 = (void *)*((_DWORD *)this + 79);
  if ( v2 != (void *)-1 )
  {
    CloseHandle(v2);
    *((_DWORD *)this + 79) = -1;
  }
  if ( *((_DWORD *)this + 7) )
  {
    operator delete(*((void **)this + 7));
    *((_DWORD *)this + 7) = 0;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 4));
  *(_DWORD *)this = &CAsyncStream::`vftable';
}

// ===== sub_4C5920 @ 0x004C5920..0x004C5954 =====
int __thiscall sub_4C5920(_QWORD *this, __int64 a2)
{
  if ( a2 < 0 || a2 > this[4] )
    return 1;
  this[5] = a2;
  return 0;
}

// ===== sub_4C5960 @ 0x004C5960..0x004C596B =====
void __thiscall sub_4C5960(int this)
{
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 4));
}

// ===== sub_4C5970 @ 0x004C5970..0x004C597B =====
void __thiscall sub_4C5970(int this)
{
  LeaveCriticalSection((LPCRITICAL_SECTION)(this + 4));
}

// ===== sub_4C5980 @ 0x004C5980..0x004C5A6D =====
int __thiscall sub_4C5980(int this, LPCSTR lpFileName, __int64 a3, LONG lDistanceToMove, const CHAR *a5)
{
  const char *v5; // ebx
  HANDLE FileA; // eax
  size_t v8; // edi
  void *v9; // eax
  LONG v11; // eax
  int v12; // edx
  const CHAR *v13; // ecx
  const CHAR *v14; // [esp-1Ch] [ebp-2Ch]
  DWORD NumberOfBytesRead; // [esp+Ch] [ebp-4h] BYREF

  v5 = lpFileName;
  v14 = lpFileName;
  *(_DWORD *)(this + 336) = 0;
  FileA = CreateFileA(v14, 0x80000000, 1u, 0, 3u, 0, 0);
  *(_DWORD *)(this + 316) = FileA;
  if ( FileA == (HANDLE)-1 )
    return 1;
  lpFileName = a5;
  if ( SetFilePointer(FileA, lDistanceToMove, (PLONG)&lpFileName, 0) == -1 )
    return 1;
  *(_DWORD *)(this + 328) = 0x20000;
  *(_DWORD *)(this + 332) = 0;
  v8 = a3 <= 0x20000 ? a3 : 0x20000;
  v9 = operator new(v8);
  *(_DWORD *)(this + 28) = v9;
  ReadFile(*(HANDLE *)(this + 316), v9, v8, &NumberOfBytesRead, 0);
  if ( NumberOfBytesRead != v8 )
    return 1;
  strcpy((char *)(this + 56), v5);
  v11 = lDistanceToMove;
  v12 = HIDWORD(a3);
  *(_DWORD *)(this + 32) = a3;
  v13 = a5;
  *(_DWORD *)(this + 320) = v11;
  *(_DWORD *)(this + 36) = v12;
  *(_DWORD *)(this + 324) = v13;
  *(_DWORD *)(this + 336) = 1;
  return 0;
}

// ===== sub_4C5A70 @ 0x004C5A70..0x004C5BD9 =====
int __thiscall sub_4C5A70(int this, void *a2, size_t Size, int a4, DWORD *a5)
{
  DWORD v8; // edi
  unsigned int v9; // eax
  int v10; // ecx
  __int64 v11; // kr00_8
  void *v12; // edx
  struct _RTL_CRITICAL_SECTION *v13; // eax
  bool v14; // cf
  DWORD NumberOfBytesRead; // [esp+10h] [ebp-18h] BYREF
  LPCRITICAL_SECTION lpCriticalSection; // [esp+14h] [ebp-14h]
  LONG DistanceToMoveHigh[4]; // [esp+18h] [ebp-10h] BYREF
  size_t Sizea; // [esp+34h] [ebp+Ch]

  if ( !*(_DWORD *)(this + 336) )
    return 1;
  lpCriticalSection = (LPCRITICAL_SECTION)(this + 4);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 4));
  DistanceToMoveHigh[3] = 0;
  if ( Size > *(__int64 *)(this + 328) )
  {
    if ( *(_DWORD *)(this + 28) )
    {
      operator delete(*(void **)(this + 28));
      *(_DWORD *)(this + 28) = 0;
    }
    *(_DWORD *)(this + 28) = operator new(Size);
    *(_DWORD *)(this + 328) = Size;
    *(_DWORD *)(this + 332) = 0;
  }
  Sizea = timeGetTime();
  if ( (signed __int64)(Size + *(_QWORD *)(this + 40)) <= *(_QWORD *)(this + 32) )
    v8 = Size;
  else
    v8 = *(_DWORD *)(this + 32) - *(_DWORD *)(this + 40);
  v9 = (v8 + *(_DWORD *)(this + 40)) / *(_DWORD *)(this + 48);
  v10 = *(_DWORD *)(this + 52);
  if ( Sizea - v10 < v9 )
    Sleep(v9 + v10 - Sizea);
  v11 = *(_QWORD *)(this + 40) + *(_QWORD *)(this + 320);
  v12 = *(void **)(this + 316);
  DistanceToMoveHigh[0] = HIDWORD(v11);
  SetFilePointer(v12, v11, DistanceToMoveHigh, 0);
  ReadFile(*(HANDLE *)(this + 316), *(LPVOID *)(this + 28), v8, &NumberOfBytesRead, 0);
  memcpy_0(a2, *(const void **)(this + 28), v8);
  v13 = lpCriticalSection;
  v14 = __CFADD__(v8, *(_DWORD *)(this + 40));
  *(_DWORD *)(this + 40) += v8;
  *(_DWORD *)(this + 44) += v14;
  *a5 = v8;
  LeaveCriticalSection(v13);
  return 0;
}

// ===== sub_4C5BE0 @ 0x004C5BE0..0x004C5C05 =====
__int64 __thiscall sub_4C5BE0(_DWORD *this, _DWORD *a2)
{
  timeGetTime();
  *a2 = this[8];
  a2[1] = this[9];
  return *((_QWORD *)this + 4);
}

// ===== sub_4C5C10 @ 0x004C5C10..0x004C5C59 =====
char *__thiscall sub_4C5C10(char *this, int a2)
{
  DWORD Time; // eax

  *(_DWORD *)this = &CMemStream::`vftable';
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 4));
  *((_DWORD *)this + 7) = 0;
  *((_DWORD *)this + 10) = 0;
  *((_DWORD *)this + 11) = 0;
  *((_DWORD *)this + 12) = a2;
  Time = timeGetTime();
  *((_DWORD *)this + 84) = 0;
  *((_DWORD *)this + 13) = Time;
  *((_DWORD *)this + 79) = -1;
  return this;
}

// ===== sub_4C5C60 @ 0x004C5C60..0x004C5C81 =====
char *__thiscall sub_4C5C60(char *this, char a2)
{
  sub_4C5890(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C5C90 @ 0x004C5C90..0x004C5CD5 =====
int __thiscall sub_4C5C90(_DWORD *this, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
  *this = a2;
  this[2] = a4;
  this[1] = a3;
  this[5] = a6;
  this[3] = a5;
  this[6] = a8;
  this[4] = a7;
  this[8] = a10;
  this[7] = a9;
  this[9] = -2147220946;
  return 0;
}

// ===== sub_4C5CE0 @ 0x004C5CE0..0x004C5D89 =====
int __thiscall sub_4C5CE0(int this)
{
  int v2; // eax
  int v3; // eax
  int v4; // eax
  int v5; // ecx
  int v7; // [esp+4h] [ebp-4h] BYREF

  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 4) + 20))(*(_DWORD *)(this + 4));
  v2 = (*(int (__thiscall **)(_DWORD, _DWORD, _DWORD))(**(_DWORD **)(this + 4) + 4))(
         *(_DWORD *)(this + 4),
         *(_DWORD *)(this + 8),
         *(_DWORD *)(this + 12));
  *(_DWORD *)(this + 36) = v2;
  if ( !v2 )
  {
    v3 = (*(int (__thiscall **)(_DWORD, _DWORD, _DWORD, _DWORD, int *))(**(_DWORD **)(this + 4) + 8))(
           *(_DWORD *)(this + 4),
           *(_DWORD *)(this + 24),
           *(_DWORD *)(this + 20),
           *(_DWORD *)(this + 16),
           &v7);
    *(_DWORD *)(this + 36) = v3;
    if ( v3 == 0x40000 )
    {
      v4 = *(_DWORD *)(this + 28);
      if ( v4 )
      {
        (*(void (__stdcall **)(int, int))(*(_DWORD *)v4 + 64))(v4, 1);
        *(_DWORD *)(this + 36) = 0;
      }
    }
    if ( *(int *)(this + 36) >= 0 )
    {
      if ( v7 != *(_DWORD *)(this + 20) )
      {
        v5 = *(_DWORD *)(this + 4);
        *(_DWORD *)(this + 20) = v7;
        *(_DWORD *)(this + 36) = 1;
        (*(void (__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
        return *(_DWORD *)(this + 36);
      }
      *(_DWORD *)(this + 36) = 0;
    }
  }
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 4) + 24))(*(_DWORD *)(this + 4));
  return *(_DWORD *)(this + 36);
}

// ===== sub_4C5D90 @ 0x004C5D90..0x004C5D95 =====
// attributes: thunk
void __thiscall sub_4C5D90(_DWORD *this)
{
  sub_4C3CE0(this);
}

// ===== sub_4C5DA0 @ 0x004C5DA0..0x004C5E27 =====
int __thiscall sub_4C5DA0(int *this, int a2, int a3, int a4, int a5, _DWORD *a6, int a7)
{
  int result; // eax
  int v9; // ecx
  _DWORD v10[10]; // [esp+Ch] [ebp-28h] BYREF

  if ( (((*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]) - 1) & a2) != 0
    || (((*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]) - 1) & a4) != 0
    || (((*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]) - 1) & a5) != 0 )
  {
    return -2147220978;
  }
  result = sub_4C5C90(v10, (int)this, this[6], a2, a3, a4, 1, a5, a7, 0);
  if ( result >= 0 )
  {
    result = sub_4C5CE0(v9);
    *a6 = v10[5];
  }
  return result;
}

// ===== sub_4C5E30 @ 0x004C5E30..0x004C5E4F =====
int __thiscall sub_4C5E30(_DWORD **this, _QWORD *a2, int a3)
{
  *a2 = ((__int64 (__thiscall *)(_DWORD *, int))*(_DWORD *)(*this[6] + 12))(this[6], a3);
  return 0;
}

// ===== sub_4C5E50 @ 0x004C5E50..0x004C5E8B =====
int __thiscall sub_4C5E50(HANDLE *this)
{
  HANDLE v2; // eax

  SetEvent(this[31]);
  v2 = this[32];
  if ( v2 )
  {
    WaitForSingleObject(v2, 0xFFFFFFFF);
    CloseHandle(this[32]);
    this[32] = 0;
  }
  return 0;
}

// ===== sub_4C5E90 @ 0x004C5E90..0x004C5F1E =====
int __thiscall sub_4C5E90(int *this, int a2, int a3, int a4, int a5)
{
  int v6; // eax
  int v7; // ebx
  int v8; // eax
  int result; // eax
  int v10; // ecx
  _DWORD v11[10]; // [esp+Ch] [ebp-28h] BYREF

  v6 = (*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]);
  v7 = a5;
  if ( ((v6 - 1) & a2) == 0 )
  {
    v8 = (*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]);
    if ( ((v8 - 1) & a4) == 0 && (((*(int (__thiscall **)(int))(*(_DWORD *)this[6] + 16))(this[6]) - 1) & v7) == 0 )
      return sub_4C5DA0(this, a2, a3, a4, v7, &a4, 0);
  }
  result = sub_4C5C90(v11, (int)this, this[6], a2, a3, a4, 0, v7, 0, 0);
  if ( result >= 0 )
    return sub_4C5CE0(v10);
  return result;
}

// ===== sub_4C5F20 @ 0x004C5F20..0x004C5F38 =====
int __thiscall sub_4C5F20(_DWORD **this, _DWORD *a2)
{
  *a2 = (*(int (__thiscall **)(_DWORD *))(*this[6] + 16))(this[6]);
  return 0;
}

// ===== sub_4C5F40 @ 0x004C5F40..0x004C5FFB =====
char *__thiscall sub_4C5F40(char *lpCriticalSection, struct _RTL_CRITICAL_SECTION_DEBUG *a2)
{
  InitializeCriticalSection((LPCRITICAL_SECTION)lpCriticalSection);
  *((_DWORD *)lpCriticalSection + 6) = a2;
  InitializeCriticalSection((LPCRITICAL_SECTION)(lpCriticalSection + 28));
  *((_DWORD *)lpCriticalSection + 13) = 0;
  sub_4C3AC0((_DWORD *)lpCriticalSection + 14, 0);
  sub_4C3AC0((_DWORD *)lpCriticalSection + 20, 0);
  sub_4C33D0((HANDLE *)lpCriticalSection + 26, 1);
  sub_4C33D0((HANDLE *)lpCriticalSection + 27, 1);
  *((_DWORD *)lpCriticalSection + 28) = 0;
  *((_DWORD *)lpCriticalSection + 29) = 0;
  sub_4C33D0((HANDLE *)lpCriticalSection + 30, 0);
  sub_4C33D0((HANDLE *)lpCriticalSection + 31, 1);
  *((_DWORD *)lpCriticalSection + 32) = 0;
  return lpCriticalSection;
}

// ===== sub_4C6000 @ 0x004C6000..0x004C6047 =====
int __thiscall sub_4C6000(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  bool v3; // cc

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  v3 = *(_DWORD *)(this + 88) <= 0;
  *(_DWORD *)(this + 52) = 0;
  if ( v3 )
    ResetEvent(*(HANDLE *)(this + 108));
  else
    SetEvent(*(HANDLE *)(this + 108));
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C6050 @ 0x004C6050..0x004C60C1 =====
_DWORD *__thiscall sub_4C6050(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  _DWORD *v3; // ebx

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  v3 = sub_4C3D50((_DWORD *)(this + 56));
  if ( !*(_DWORD *)(this + 64) )
    ResetEvent(*(HANDLE *)(this + 104));
  LeaveCriticalSection(v2);
  return v3;
}

// ===== sub_4C60D0 @ 0x004C60D0..0x004C614D =====
_DWORD *__thiscall sub_4C60D0(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  _DWORD *v3; // ebx

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  v3 = sub_4C3D50((_DWORD *)(this + 80));
  if ( !*(_DWORD *)(this + 88) && (!*(_DWORD *)(this + 52) || *(_DWORD *)(this + 116)) )
    ResetEvent(*(HANDLE *)(this + 108));
  LeaveCriticalSection(v2);
  return v3;
}

// ===== sub_4C6150 @ 0x004C6150..0x004C6181 =====
int __thiscall sub_4C6150(int this, int a2)
{
  if ( !sub_4C3C20((_DWORD *)(this + 80), a2) )
    return -2147024882;
  SetEvent(*(HANDLE *)(this + 108));
  return 0;
}

// ===== sub_4C6190 @ 0x004C6190..0x004C6267 =====
void __thiscall sub_4C6190(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // ebx
  _DWORD *i; // edi
  bool v4; // zf

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  for ( i = sub_4C6050(this); i; i = sub_4C6050(this) )
  {
    ++*(_DWORD *)(this + 112);
    LeaveCriticalSection(v2);
    sub_4C5CE0((int)i);
    EnterCriticalSection(v2);
    sub_4C6150(this, (int)i);
    v4 = (*(_DWORD *)(this + 112))-- == 1;
    if ( v4 && *(_DWORD *)(this + 116) )
      SetEvent(*(HANDLE *)(this + 120));
    LeaveCriticalSection(v2);
    v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  }
  LeaveCriticalSection(v2);
}

// ===== sub_4C6270 @ 0x004C6270..0x004C62C0 =====
int __thiscall sub_4C6270(_DWORD *this)
{
  void *v2; // eax
  void *v3; // ecx
  HANDLE Handles[2]; // [esp+8h] [ebp-8h] BYREF

  v2 = (void *)this[31];
  v3 = (void *)this[26];
  Handles[0] = v2;
  Handles[1] = v3;
  while ( WaitForMultipleObjects(2u, Handles, 0, 0xFFFFFFFF) == 1 )
    sub_4C6190((int)this);
  return 0;
}

// ===== sub_4C62C0 @ 0x004C62C0..0x004C62CF =====
int __stdcall sub_4C62C0(_DWORD *lpThreadParameter)
{
  return sub_4C6270(lpThreadParameter);
}

// ===== sub_4C62D0 @ 0x004C62D0..0x004C63C3 =====
int __thiscall sub_4C62D0(int this, DWORD dwMilliseconds, _DWORD *a3, _DWORD *a4, _DWORD *a5)
{
  _DWORD *v6; // eax
  _DWORD *v7; // esi
  int v9; // ebx
  int v10; // ebx
  __int64 v11; // [esp+Ch] [ebp-8h]

  *a3 = 0;
  if ( WaitForSingleObject(*(HANDLE *)(this + 108), dwMilliseconds) )
    return -2147220946;
  while ( 1 )
  {
    v6 = sub_4C60D0(this);
    v7 = v6;
    if ( v6 )
      break;
    EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
    if ( *(_DWORD *)(this + 52) && !*(_DWORD *)(this + 116) )
    {
      LeaveCriticalSection((LPCRITICAL_SECTION)(this + 28));
      return -2147220953;
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)(this + 28));
    if ( WaitForSingleObject(*(HANDLE *)(this + 108), dwMilliseconds) )
      return -2147220946;
  }
  v9 = v6[9];
  if ( v9 == 1 )
  {
    v10 = v6[5];
    v11 = *((_QWORD *)v6 + 1);
    if ( v11 + v10 == ((__int64 (__thiscall *)(_DWORD, _DWORD))*(_DWORD *)(**(_DWORD **)(this + 24) + 12))(
                        *(_DWORD *)(this + 24),
                        0) )
      v9 = 0;
    else
      v9 = -2147467259;
  }
  *a5 = v7[5];
  *a3 = v7[7];
  *a4 = v7[8];
  operator delete(v7);
  return v9;
}

// ===== sub_4C63D0 @ 0x004C63D0..0x004C64B1 =====
int __thiscall sub_4C63D0(int this)
{
  struct _RTL_CRITICAL_SECTION *v2; // edi
  _DWORD *i; // eax
  void *v4; // edx

  v2 = (struct _RTL_CRITICAL_SECTION *)(this + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(this + 28));
  *(_DWORD *)(this + 52) = 1;
  for ( i = sub_4C6050(this); i; i = sub_4C6050(this) )
    sub_4C6150(this, (int)i);
  if ( *(int *)(this + 112) <= 0 )
  {
    SetEvent(*(HANDLE *)(this + 108));
  }
  else
  {
    *(_DWORD *)(this + 116) = 1;
    LeaveCriticalSection(v2);
    WaitForSingleObject(*(HANDLE *)(this + 120), 0xFFFFFFFF);
    EnterCriticalSection(v2);
    while ( *(_DWORD *)(this + 112) )
    {
      LeaveCriticalSection(v2);
      WaitForSingleObject(*(HANDLE *)(this + 120), 0xFFFFFFFF);
      EnterCriticalSection(v2);
    }
    v4 = *(void **)(this + 108);
    *(_DWORD *)(this + 116) = 0;
    SetEvent(v4);
  }
  LeaveCriticalSection(v2);
  return 0;
}

// ===== sub_4C64C0 @ 0x004C64C0..0x004C651A =====
int __thiscall sub_4C64C0(LPVOID lpParameter)
{
  HANDLE v2; // eax
  int result; // eax
  DWORD ThreadId; // [esp+4h] [ebp-4h] BYREF

  if ( *((_DWORD *)lpParameter + 32) )
    return 0;
  ResetEvent(*((HANDLE *)lpParameter + 31));
  v2 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)sub_4C62C0, lpParameter, 0, &ThreadId);
  *((_DWORD *)lpParameter + 32) = v2;
  if ( v2 )
    return 0;
  result = GetLastError();
  if ( result > 0 )
    return (unsigned __int16)result | 0x80070000;
  return result;
}

// ===== sub_4C6520 @ 0x004C6520..0x004C65AE =====
int __thiscall sub_4C6520(char *lpParameter, int a2)
{
  struct _RTL_CRITICAL_SECTION *v3; // edi
  int v4; // esi

  v3 = (struct _RTL_CRITICAL_SECTION *)(lpParameter + 28);
  EnterCriticalSection((LPCRITICAL_SECTION)(lpParameter + 28));
  if ( *((_DWORD *)lpParameter + 13) )
  {
    v4 = -2147220953;
  }
  else if ( sub_4C3C20((_DWORD *)lpParameter + 14, a2) )
  {
    SetEvent(*((HANDLE *)lpParameter + 26));
    v4 = sub_4C64C0(lpParameter);
  }
  else
  {
    v4 = -2147024882;
  }
  LeaveCriticalSection(v3);
  return v4;
}

// ===== sub_4C65B0 @ 0x004C65B0..0x004C668B =====
void __thiscall sub_4C65B0(char *lpCriticalSection)
{
  void *v2; // eax
  LONG v3; // [esp+10h] [ebp-10h] BYREF
  int v4; // [esp+1Ch] [ebp-4h]

  v4 = 7;
  sub_4C63D0((int)lpCriticalSection);
  sub_4C5E50((HANDLE *)lpCriticalSection);
  v3 = *((_DWORD *)lpCriticalSection + 20);
  while ( v3 )
  {
    v2 = (void *)sub_4C3B30(&v3);
    operator delete(v2);
  }
  sub_4C3AE0((_DWORD *)lpCriticalSection + 20);
  LOBYTE(v4) = 6;
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((HANDLE *)lpCriticalSection + 31);
  LOBYTE(v4) = 5;
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((HANDLE *)lpCriticalSection + 30);
  LOBYTE(v4) = 4;
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((HANDLE *)lpCriticalSection + 27);
  LOBYTE(v4) = 3;
  Concurrency::details::UMSFreeVirtualProcessorRoot::InitialThreadParam::~InitialThreadParam((HANDLE *)lpCriticalSection + 26);
  LOBYTE(v4) = 2;
  sub_4C3CE0((_DWORD *)lpCriticalSection + 20);
  LOBYTE(v4) = 1;
  sub_4C3CE0((_DWORD *)lpCriticalSection + 14);
  DeleteCriticalSection((LPCRITICAL_SECTION)(lpCriticalSection + 28));
  DeleteCriticalSection((LPCRITICAL_SECTION)lpCriticalSection);
}

// ===== sub_4C6690 @ 0x004C6690..0x004C6731 =====
int __thiscall sub_4C6690(int lpParameter, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  _DWORD *v10; // ebx
  int v11; // edi

  if ( a5
    && ((((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(lpParameter + 24) + 16))(*(_DWORD *)(lpParameter + 24)) - 1) & a2) != 0
     || (((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(lpParameter + 24) + 16))(*(_DWORD *)(lpParameter + 24)) - 1) & a4) != 0
     || (((*(int (__thiscall **)(_DWORD))(**(_DWORD **)(lpParameter + 24) + 16))(*(_DWORD *)(lpParameter + 24)) - 1) & a6) != 0) )
  {
    return -2147220978;
  }
  v10 = operator new(0x28u);
  v11 = sub_4C5C90(v10, lpParameter, *(_DWORD *)(lpParameter + 24), a2, a3, a4, a5, a6, a7, a8);
  if ( v11 < 0 || (v11 = sub_4C6520((char *)lpParameter, (int)v10), v11 < 0) )
    operator delete(v10);
  return v11;
}

// ===== DirectSoundCreate8 @ 0x004C6732..0x004C6738 =====
// attributes: thunk
HRESULT __stdcall DirectSoundCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUND8 *ppDS8, LPUNKNOWN pUnkOuter)
{
  return __imp_DirectSoundCreate8(pcGuidDevice, ppDS8, pUnkOuter);
}

// ===== sub_4C6740 @ 0x004C6740..0x004C67B5 =====
_DWORD *__thiscall sub_4C6740(_DWORD *this, int a2)
{
  _BYTE v4[16]; // [esp+8h] [ebp-20h] BYREF
  _DWORD *v5; // [esp+18h] [ebp-10h]
  int v6; // [esp+24h] [ebp-4h]

  v5 = this;
  sub_4C6880(0);
  v6 = 0;
  *this = &CDebugMsgOut::`vftable';
  sub_4C6880(a2);
  sub_4C68C0(v4);
  this[4] = 1;
  return this;
}

// ===== sub_4C67C0 @ 0x004C67C0..0x004C681E =====
_DWORD *__thiscall sub_4C67C0(_DWORD *this, char a2)
{
  *this = &CDebugMsgOut::`vftable';
  sub_4C68C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C6820 @ 0x004C6820..0x004C6871 =====
unsigned int sub_4C6820(int a1, unsigned int a2, int a3, int a4, char *Format, ...)
{
  unsigned int result; // eax
  char v6; // [esp-20h] [ebp-24h]
  va_list va; // [esp+20h] [ebp+1Ch] BYREF

  va_start(va, Format);
  result = a2;
  if ( *(_DWORD *)(a1 + 16) >= a2 )
  {
    if ( *(_DWORD *)(a1 + 12) )
    {
      sub_4C68F0(a1, "<< %s >> %s[%d] : ", *(_DWORD *)&ArgList[4 * a2]);
      vsprintf(*(char *const *)(a1 + 8), Format, va);
      return sub_4C68F0(a1, *(char **)(a1 + 8), v6);
    }
  }
  return result;
}

// ===== sub_4C6880 @ 0x004C6880..0x004C68BF =====
_DWORD *__thiscall sub_4C6880(_DWORD *this, int a2)
{
  void *v3; // eax
  bool v4; // zf
  _DWORD *result; // eax

  *this = &CMsgOut::`vftable';
  this[3] = 0;
  v3 = operator new(0x2800u);
  this[2] = v3;
  v4 = v3 == 0;
  this[1] = a2;
  result = this;
  if ( !v4 )
  {
    if ( a2 )
      this[3] = 1;
  }
  return result;
}

// ===== sub_4C68C0 @ 0x004C68C0..0x004C68E2 =====
int __thiscall sub_4C68C0(_DWORD *this)
{
  int result; // eax
  void *v3; // [esp-4h] [ebp-8h]

  v3 = (void *)this[2];
  *this = &CMsgOut::`vftable';
  operator delete(v3);
  result = 0;
  this[2] = 0;
  this[1] = 0;
  this[3] = 0;
  return result;
}

// ===== sub_4C68F0 @ 0x004C68F0..0x004C6922 =====
int sub_4C68F0(int a1, char *Format, ...)
{
  int result; // eax
  va_list va; // [esp+14h] [ebp+10h] BYREF

  va_start(va, Format);
  if ( *(_DWORD *)(a1 + 12) )
  {
    vsprintf(*(char *const *)(a1 + 8), Format, va);
    return (*(int (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(a1 + 4) + 4))(*(_DWORD *)(a1 + 4), *(_DWORD *)(a1 + 8));
  }
  return result;
}

// ===== sub_4C6930 @ 0x004C6930..0x004C6934 =====
int __thiscall sub_4C6930(_DWORD *this)
{
  return this[1];
}

// ===== sub_4C6940 @ 0x004C6940..0x004C6961 =====
_DWORD *__thiscall sub_4C6940(_DWORD *this, char a2)
{
  sub_4C68C0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C6970 @ 0x004C6970..0x004C6987 =====
_DWORD *__thiscall sub_4C6970(_DWORD *this)
{
  _DWORD *result; // eax

  result = this;
  *this = &CThreadCtrl::`vftable';
  this[1] = 0;
  this[4] = 20;
  return result;
}

// ===== sub_4C6990 @ 0x004C6990..0x004C69BB =====
int __thiscall sub_4C6990(_DWORD *this, DWORD dwMilliseconds)
{
  void *v2; // ecx
  int result; // eax

  v2 = (void *)this[1];
  result = -1;
  if ( v2 )
    return WaitForSingleObject(v2, dwMilliseconds) != 258 ? 0 : -20;
  return result;
}

// ===== sub_4C69C0 @ 0x004C69C0..0x004C69FB =====
int __thiscall sub_4C69C0(int this, DWORD dwMilliseconds)
{
  int result; // eax
  int v4; // edi

  result = -1;
  if ( *(_DWORD *)(this + 4) )
  {
    *(_DWORD *)(this + 12) = 1;
    v4 = sub_4C6990((_DWORD *)this, dwMilliseconds);
    CloseHandle(*(HANDLE *)(this + 4));
    result = v4;
    *(_DWORD *)(this + 4) = 0;
  }
  return result;
}

// ===== sub_4C6A00 @ 0x004C6A00..0x004C6A33 =====
void __stdcall __noreturn sub_4C6A00(_DWORD *lpThreadParameter)
{
  DWORD v1; // edi

  *(_DWORD *)(lpThreadParameter[2] + 12) = 0;
  v1 = ((int (__cdecl *)(_DWORD, int))*lpThreadParameter)(lpThreadParameter[1], lpThreadParameter[2] + 12);
  operator delete(lpThreadParameter);
  ExitThread(v1);
}

// ===== sub_4C6A40 @ 0x004C6A40..0x004C6A50 =====
int __thiscall sub_4C6A40(_DWORD *this)
{
  DWORD v2; // [esp-4h] [ebp-4h]

  v2 = this[4];
  *this = &CThreadCtrl::`vftable';
  return sub_4C69C0((int)this, v2);
}

// ===== sub_4C6A50 @ 0x004C6A50..0x004C6AAC =====
int __thiscall sub_4C6A50(
        DWORD *this,
        int a2,
        int a3,
        int nPriority,
        SIZE_T dwStackSize,
        DWORD dwCreationFlags,
        LPSECURITY_ATTRIBUTES lpThreadAttributes)
{
  _DWORD *v8; // eax
  int v9; // edi
  HANDLE Thread; // eax

  v8 = operator new(0xCu);
  *v8 = a2;
  v8[1] = a3;
  v8[2] = this;
  v9 = 10000;
  Thread = CreateThread(
             lpThreadAttributes,
             dwStackSize,
             (LPTHREAD_START_ROUTINE)sub_4C6A00,
             v8,
             dwCreationFlags,
             this + 2);
  this[1] = (DWORD)Thread;
  if ( Thread )
    v9 = 0;
  SetThreadPriority(Thread, nPriority);
  return v9;
}

// ===== sub_4C6AB0 @ 0x004C6AB0..0x004C6AD1 =====
_DWORD *__thiscall sub_4C6AB0(_DWORD *this, char a2)
{
  sub_4C6A40(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C6AE0 @ 0x004C6AE0..0x004C6B49 =====
void *__thiscall sub_4C6AE0(void *this, size_t Size)
{
  sub_4C6E80(Size);
  *(_DWORD *)this = &CRotateBufferSecurity::`vftable';
  *((_DWORD *)this + 2) = &CRotateBufferSecurity::`vftable';
  sub_4A4A40((char *)this + 36);
  return this;
}

// ===== sub_4C6B50 @ 0x004C6B50..0x004C6B81 =====
int __thiscall sub_4C6B50(void *this, void *Src, size_t Size)
{
  int v3; // edi
  int v4; // esi

  v3 = (int)this + 28;
  sub_4A4A80((int)this + 28);
  v4 = sub_4C6F40(Src, Size);
  sub_4A4A90(v3);
  return v4;
}

// ===== sub_4C6B90 @ 0x004C6B90..0x004C6BC1 =====
int __thiscall sub_4C6B90(void *this, void *a2, size_t Size)
{
  int v3; // edi
  int v4; // esi

  v3 = (int)this + 36;
  sub_4A4A80((int)this + 36);
  v4 = sub_4C6FC0(a2, Size);
  sub_4A4A90(v3);
  return v4;
}

// ===== sub_4C6BD0 @ 0x004C6BD0..0x004C6BED =====
int __thiscall sub_4C6BD0(_DWORD *this)
{
  int v2; // edi
  int v3; // esi

  v2 = (int)(this + 9);
  sub_4A4A80((int)(this + 9));
  v3 = this[7];
  sub_4A4A90(v2);
  return v3;
}

// ===== sub_4C6BF0 @ 0x004C6BF0..0x004C6C0D =====
int __thiscall sub_4C6BF0(_DWORD *this)
{
  int v2; // edi
  int v3; // esi

  v2 = (int)(this + 9);
  sub_4A4A80((int)(this + 9));
  v3 = this[6];
  sub_4A4A90(v2);
  return v3;
}

// ===== sub_4C6C10 @ 0x004C6C10..0x004C6C31 =====
int __thiscall sub_4C6C10(_DWORD *this)
{
  int v2; // edi

  sub_4A4A80((int)(this + 7));
  v2 = this[5] - this[4];
  sub_4A4A90((int)(this + 7));
  return v2;
}

// ===== sub_4C6C40 @ 0x004C6C40..0x004C6C61 =====
int __thiscall sub_4C6C40(_DWORD *this)
{
  sub_4A4A80((int)(this + 9));
  this[6] = 0;
  this[4] = 0;
  this[5] = 0;
  return sub_4A4A90((int)(this + 9));
}

// ===== sub_4C6C70 @ 0x004C6C70..0x004C6C78 =====
int __thiscall sub_4C6C70(char *this, char a2)
{
  return sub_4C6C80(this - 8, a2);
}

// ===== sub_4C6C80 @ 0x004C6C80..0x004C6CE9 =====
char *__thiscall sub_4C6C80(char *this, char a2)
{
  sub_4A4A60(this + 36);
  sub_4C6CF0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C6CF0 @ 0x004C6CF0..0x004C6D69 =====
void __thiscall sub_4C6CF0(int this)
{
  *(_DWORD *)this = &CRotateBuffer::`vftable';
  *(_DWORD *)(this + 8) = &CRotateBuffer::`vftable';
  operator delete(*(void **)(this + 32));
  *(_DWORD *)(this + 32) = 0;
  sub_4A5360((_DWORD *)(this + 8));
  sub_4A5470((_DWORD *)this);
}

// ===== sub_4C6D70 @ 0x004C6D70..0x004C6D92 =====
unsigned int __thiscall sub_4C6D70(_DWORD *this, int a2)
{
  unsigned int result; // eax

  result = (unsigned int)(this[7] + this[4] + a2 % this[7]) % this[7];
  this[4] = result;
  return result;
}

// ===== sub_4C6DA0 @ 0x004C6DA0..0x004C6DC2 =====
unsigned int __thiscall sub_4C6DA0(_DWORD *this, int a2)
{
  unsigned int result; // eax

  result = (unsigned int)(this[7] + this[5] + a2 % this[7]) % this[7];
  this[5] = result;
  return result;
}

// ===== sub_4C6DD0 @ 0x004C6DD0..0x004C6E35 =====
_DWORD *__thiscall sub_4C6DD0(_DWORD *this, int a2, int a3, _DWORD *a4, _DWORD *a5, _DWORD *a6, _DWORD *a7)
{
  _DWORD *result; // eax

  if ( (unsigned int)(a2 + a3) <= this[7] )
  {
    *a4 = a2 + this[8];
    result = a7;
    *a5 = a3;
    *a6 = 0;
    *a7 = 0;
  }
  else
  {
    *a4 = a2 + this[8];
    *a5 = this[7] - a2;
    *a6 = this[8];
    result = (_DWORD *)(a3 + a2 - this[7]);
    *a7 = result;
  }
  return result;
}

// ===== sub_4C6E40 @ 0x004C6E40..0x004C6E44 =====
int __thiscall sub_4C6E40(_DWORD *this)
{
  return this[6];
}

// ===== sub_4C6E50 @ 0x004C6E50..0x004C6E57 =====
int __thiscall sub_4C6E50(_DWORD *this)
{
  return this[5] - this[4];
}

// ===== sub_4C6E60 @ 0x004C6E60..0x004C6E6C =====
int __thiscall sub_4C6E60(_DWORD *this)
{
  int result; // eax

  result = 0;
  this[6] = 0;
  this[4] = 0;
  this[5] = 0;
  return result;
}

// ===== sub_4C6E70 @ 0x004C6E70..0x004C6E78 =====
int __thiscall sub_4C6E70(char *this, char a2)
{
  return sub_4C6F10(this - 8, a2);
}

// ===== sub_4C6E80 @ 0x004C6E80..0x004C6F04 =====
_DWORD *__thiscall sub_4C6E80(_DWORD *this, size_t Size)
{
  sub_4A5450(this);
  sub_4A5340(this + 2);
  *this = &CRotateBuffer::`vftable';
  this[2] = &CRotateBuffer::`vftable';
  this[7] = Size;
  this[8] = operator new(Size);
  sub_4C6E60(this);
  return this;
}

// ===== sub_4C6F10 @ 0x004C6F10..0x004C6F31 =====
void *__thiscall sub_4C6F10(void *this, char a2)
{
  sub_4C6CF0((int)this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4C6F40 @ 0x004C6F40..0x004C6FBD =====
int __thiscall sub_4C6F40(int *this, char *Src, size_t Size)
{
  int v4; // edi
  size_t v6; // [esp+Ch] [ebp-Ch] BYREF
  void *v7; // [esp+10h] [ebp-8h] BYREF
  void *v8; // [esp+14h] [ebp-4h] BYREF

  v4 = Size;
  if ( this[5] - this[4] < Size )
    v4 = this[5] - this[4];
  sub_4C6DD0(this - 2, this[2], v4, &v8, &Size, &v7, &v6);
  memcpy_0(v8, Src, Size);
  if ( v7 )
    memcpy_0(v7, &Src[Size], v6);
  sub_4C6D70(this - 2, v4);
  this[4] += v4;
  return v4;
}

// ===== sub_4C6FC0 @ 0x004C6FC0..0x004C7035 =====
int __thiscall sub_4C6FC0(_DWORD *this, char *a2, size_t Size)
{
  int v4; // edi
  size_t v5; // ebx
  size_t v7; // [esp+Ch] [ebp-Ch] BYREF
  void *v8; // [esp+10h] [ebp-8h] BYREF
  void *Src; // [esp+14h] [ebp-4h] BYREF

  v4 = Size;
  if ( this[6] < Size )
    v4 = this[6];
  sub_4C6DD0(this, this[5], v4, &Src, &Size, &v8, &v7);
  v5 = Size;
  memcpy_0(a2, Src, Size);
  if ( v8 )
    memcpy_0(&a2[v5], v8, v7);
  sub_4C6DA0(this, v4);
  this[6] -= v4;
  return v4;
}

// ===== RtlUnwind @ 0x004C7036..0x004C703C =====
// attributes: thunk
void __stdcall RtlUnwind(PVOID TargetFrame, PVOID TargetIp, PEXCEPTION_RECORD ExceptionRecord, PVOID ReturnValue)
{
  __imp_RtlUnwind(TargetFrame, TargetIp, ExceptionRecord, ReturnValue);
}

// ===== _calloc @ 0x004C703C..0x004C707C =====
void *__cdecl calloc(size_t Count, size_t Size)
{
  void *v2; // esi
  int v4; // [esp+4h] [ebp-4h] BYREF

  v4 = 0;
  v2 = _calloc_impl(Count, Size, &v4);
  if ( !v2 && v4 && _errno() )
    *_errno() = v4;
  return v2;
}

// ===== __alldiv @ 0x004C7080..0x004C712A =====
int __stdcall _alldiv(unsigned __int64 a1, __int64 a2)
{
  int v2; // edi
  int v3; // eax
  unsigned __int64 v4; // rtt
  __int64 v5; // rax
  unsigned __int64 v6; // rcx
  unsigned __int64 v7; // rax
  unsigned int v8; // esi
  unsigned __int64 v9; // rax

  v2 = 0;
  if ( (a1 & 0x8000000000000000uLL) != 0LL )
  {
    v2 = 1;
    HIDWORD(a1) = -HIDWORD(a1) - ((_DWORD)a1 != 0);
    LODWORD(a1) = -(int)a1;
  }
  v3 = HIDWORD(a2);
  if ( a2 < 0 )
  {
    ++v2;
    v3 = -HIDWORD(a2) - ((_DWORD)a2 != 0);
    HIDWORD(a2) = v3;
    LODWORD(a2) = -(int)a2;
  }
  if ( v3 )
  {
    v6 = __PAIR64__(v3, a2);
    v7 = a1;
    do
    {
      v6 >>= 1;
      v7 >>= 1;
    }
    while ( HIDWORD(v6) );
    v8 = v7 / (unsigned int)v6;
    v9 = v8 * (unsigned __int64)(unsigned int)a2;
    if ( __CFADD__(HIDWORD(a2) * v8, HIDWORD(v9)) || (HIDWORD(v9) = (a2 * (unsigned __int64)v8) >> 32, v9 > a1) )
      --v8;
    v5 = v8;
  }
  else
  {
    LODWORD(v4) = a1;
    HIDWORD(v4) = HIDWORD(a1) % (unsigned int)a2;
    LODWORD(v5) = v4 / (unsigned int)a2;
    HIDWORD(v5) = HIDWORD(a1) / (unsigned int)a2;
  }
  if ( v2 == 1 )
    return -v5;
  return v5;
}

// ===== _floor @ 0x004C7130..0x004C7251 =====
double __cdecl floor(double X)
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
    goto _floor;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v13 & 0x7F) == 127;
  if ( v2 )
  {
    v3 = _mm_loadl_epi64((const __m128i *)&X);
    v4 = (__m128d)_mm_srli_epi64(v3, 0x34u);
    v5 = _mm_cvtsi128_si32((__m128i)v4);
    v6 = _mm_sub_epi32((__m128i)xmmword_4E27A0, (__m128i)_mm_and_pd(v4, (__m128d)xmmword_4E27D0));
    v7 = _mm_srl_epi64(v3, v6);
    if ( (v5 & 0x800) != 0 )
    {
      v10 = (__m128d)_mm_sll_epi64(v7, v6);
      v11 = (__m128d)_mm_loadl_epi64((const __m128i *)&X);
      v12 = *(_OWORD *)&_mm_cmplt_pd(v11, v10);
      if ( v5 < 3071 )
      {
        *(_QWORD *)&X = *(_OWORD *)&_mm_cmplt_pd(v11, (__m128d)xmmword_4E27C0) & 0x3FF0000000000000LL | 0x8000000000000000uLL;
        return X;
      }
      else
      {
        if ( v5 > 3122 )
          return X;
        return v10.m128d_f64[0] - COERCE_DOUBLE(v12 & 0x3FF0000000000000LL);
      }
    }
    else
    {
      if ( v5 >= 1023 )
      {
        *(_QWORD *)&v8 = v7.m128i_i64[0] << v6.m128i_i8[0];
        if ( v5 <= 1074 )
        {
          X = v8;
          return v8;
        }
        return X;
      }
      return 0.0;
    }
  }
  else
  {
_floor:
    _floor_default(X);
  }
  return result;
}

// ===== __ftol @ 0x004C7260..0x004C7287 =====
__int64 __usercall _ftol@<edx:eax>(double a1@<st0>)
{
  return (__int64)a1;
}

// ===== __aulldiv @ 0x004C7290..0x004C72F8 =====
unsigned int __stdcall _aulldiv(unsigned __int64 a1, __int64 a2)
{
  unsigned __int64 v3; // rtt
  unsigned int v4; // ecx
  unsigned int v5; // ebx
  unsigned __int64 v6; // rax
  char v7; // cf
  unsigned int v8; // esi
  unsigned __int64 v9; // rax

  if ( HIDWORD(a2) )
  {
    v4 = HIDWORD(a2);
    v5 = a2;
    v6 = a1;
    do
    {
      v7 = v4 & 1;
      v4 >>= 1;
      v5 = (v5 >> 1) | (v7 << 31);
      v6 >>= 1;
    }
    while ( v4 );
    v8 = v6 / v5;
    v9 = v8 * (unsigned __int64)(unsigned int)a2;
    if ( __CFADD__(HIDWORD(a2) * v8, HIDWORD(v9)) || (HIDWORD(v9) = (a2 * (unsigned __int64)v8) >> 32, v9 > a1) )
      --v8;
    return v8;
  }
  else
  {
    LODWORD(v3) = a1;
    HIDWORD(v3) = HIDWORD(a1) % (unsigned int)a2;
    return v3 / (unsigned int)a2;
  }
}

// ===== __floor_default @ 0x004C72F8..0x004C73D0 =====
double __cdecl _floor_default(double a1)
{
  __int16 v1; // cx
  int v2; // ebx
  int v3; // eax
  double result; // st7
  __int16 v5; // [esp+10h] [ebp-14h]
  __int16 v6; // [esp+10h] [ebp-14h]
  double v7; // [esp+1Ch] [ebp-8h]
  int savedregs; // [esp+24h] [ebp+0h] BYREF

  v2 = _ctrlfp(v1);
  if ( (HIWORD(a1) & 0x7FF0) == 0x7FF0 )
  {
    v3 = _sptype(SLODWORD(a1), SHIDWORD(a1));
    if ( v3 > 0 )
    {
      if ( v3 <= 2 )
      {
        _ctrlfp(v5);
        return a1;
      }
      if ( v3 == 3 )
        return _handle_qnan1(11, a1, v2);
    }
    result = a1;
    _except1((int)&savedregs, 8u, 11, SLODWORD(a1), SHIDWORD(a1), a1 + 1.0, v2);
  }
  else
  {
    v7 = _frnd(a1);
    if ( a1 == v7 || (v2 & 0x20) != 0 )
    {
      _ctrlfp(v6);
      return v7;
    }
    else
    {
      result = a1;
      _except1((int)&savedregs, 0x10u, 11, SLODWORD(a1), SHIDWORD(a1), v7, v2);
    }
  }
  return result;
}

// ===== sub_4C73D0 @ 0x004C73D0..0x004C73E2 =====
int __cdecl sub_4C73D0(_DWORD *a1)
{
  int result; // eax

  result = 0;
  *a1 = 0;
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  return result;
}

// ===== sub_4C73F0 @ 0x004C73F0..0x004C745C =====
void __cdecl sub_4C73F0(int a1)
{
  int i; // edi

  if ( a1 )
  {
    for ( i = 0; i < *(_DWORD *)(a1 + 8); ++i )
    {
      if ( *(_DWORD *)(*(_DWORD *)a1 + 4 * i) )
        free(*(void **)(*(_DWORD *)a1 + 4 * i));
    }
    if ( *(_DWORD *)a1 )
      free(*(void **)a1);
    if ( *(_DWORD *)(a1 + 4) )
      free(*(void **)(a1 + 4));
    if ( *(_DWORD *)(a1 + 12) )
      free(*(void **)(a1 + 12));
  }
  *(_DWORD *)a1 = 0;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 12) = 0;
}

// ===== sub_4C7460 @ 0x004C7460..0x004C7477 =====
int __cdecl sub_4C7460(int a1, int a2)
{
  int v2; // eax

  v2 = *(_DWORD *)(a1 + 28);
  if ( v2 )
    return *(_DWORD *)(v2 + 4 * a2);
  else
    return -1;
}

// ===== sub_4C7480 @ 0x004C7480..0x004C74A6 =====
void *__cdecl sub_4C7480(_DWORD *a1)
{
  void *result; // eax

  memset(a1, 0, 0x20u);
  result = calloc(1u, 0xE78u);
  a1[7] = result;
  return result;
}

// ===== sub_4C74B0 @ 0x004C74B0..0x004C7613 =====
int __cdecl sub_4C74B0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  void **v3; // ebp
  int v4; // ebp
  _DWORD *v5; // ebx
  int v6; // ebp
  _DWORD *v7; // ebx
  int v8; // ebp
  _DWORD *v9; // ebx
  int v10; // ebx
  int v11; // edi
  void **v12; // ebp
  int v13; // eax
  int v14; // ebx
  void **v15; // ebp
  int result; // eax

  v1 = a1[7];
  if ( v1 )
  {
    v2 = 0;
    if ( *(int *)(v1 + 8) > 0 )
    {
      v3 = (void **)(v1 + 32);
      do
      {
        if ( *v3 )
          free(*v3);
        ++v2;
        ++v3;
      }
      while ( v2 < *(_DWORD *)(v1 + 8) );
    }
    v4 = 0;
    if ( *(int *)(v1 + 12) > 0 )
    {
      v5 = (_DWORD *)(v1 + 544);
      do
      {
        ((void (__cdecl *)(_DWORD))(&off_502DEC)[*(v5 - 64)][2])(*v5);
        ++v4;
        ++v5;
      }
      while ( v4 < *(_DWORD *)(v1 + 12) );
    }
    v6 = 0;
    if ( *(int *)(v1 + 16) > 0 )
    {
      v7 = (_DWORD *)(v1 + 1056);
      do
      {
        (*((void (__cdecl **)(_DWORD))*(&off_502DD8 + *(v7 - 64)) + 3))(*v7);
        ++v6;
        ++v7;
      }
      while ( v6 < *(_DWORD *)(v1 + 16) );
    }
    v8 = 0;
    if ( *(int *)(v1 + 20) > 0 )
    {
      v9 = (_DWORD *)(v1 + 1568);
      do
      {
        (*((void (__cdecl **)(_DWORD))*(&off_502DE0 + *(v9 - 64)) + 3))(*v9);
        ++v8;
        ++v9;
      }
      while ( v8 < *(_DWORD *)(v1 + 20) );
    }
    v10 = 0;
    if ( *(int *)(v1 + 24) > 0 )
    {
      v11 = 0;
      v12 = (void **)(v1 + 1824);
      do
      {
        if ( *v12 )
          sub_4CB750(*v12);
        v13 = *(_DWORD *)(v1 + 2848);
        if ( v13 )
          sub_4CB770(v11 + v13);
        ++v10;
        ++v12;
        v11 += 44;
      }
      while ( v10 < *(_DWORD *)(v1 + 24) );
    }
    if ( *(_DWORD *)(v1 + 2848) )
      free(*(void **)(v1 + 2848));
    v14 = 0;
    if ( *(int *)(v1 + 28) > 0 )
    {
      v15 = (void **)(v1 + 2852);
      do
      {
        sub_4C8AA0(*v15);
        ++v14;
        ++v15;
      }
      while ( v14 < *(_DWORD *)(v1 + 28) );
    }
    free((void *)v1);
  }
  result = 0;
  memset(a1, 0, 0x20u);
  return result;
}

// ===== sub_4C7620 @ 0x004C7620..0x004C7720 =====
int __cdecl sub_4C7620(int a1, int a2, _DWORD *a3)
{
  int v3; // ebp
  int v4; // ecx
  char *v5; // edi
  int *v6; // esi
  bool v7; // zf
  int v9; // [esp+8h] [ebp-1Ch] BYREF
  __int16 v10; // [esp+Ch] [ebp-18h]
  _BYTE v11[20]; // [esp+10h] [ebp-14h] BYREF

  if ( a3 )
  {
    sub_4D4AF0(v11, *a3, a3[1]);
    v3 = sub_4D4D00(v11, 8);
    v9 = 0;
    v10 = 0;
    sub_4C7720(v11, &v9, 6);
    v4 = 3;
    v5 = aVorbis;
    v6 = &v9;
    v7 = 1;
    do
    {
      if ( !v4 )
        break;
      v7 = *(_WORD *)v6 == *(_WORD *)v5;
      v6 = (int *)((char *)v6 + 2);
      v5 += 2;
      --v4;
    }
    while ( v7 );
    if ( !v7 )
      return -132;
    if ( v3 == 1 )
    {
      if ( a3[2] && !*(_DWORD *)(a1 + 8) )
        return sub_4C7750(a1, v11);
    }
    else if ( v3 == 3 )
    {
      if ( *(_DWORD *)(a1 + 8) )
        return sub_4C7830(a2, v11);
    }
    else if ( v3 == 5 && *(_DWORD *)(a1 + 8) && *(_DWORD *)(a2 + 12) )
    {
      return sub_4C7910(a1, v11);
    }
  }
  return -133;
}

// ===== sub_4C7720 @ 0x004C7720..0x004C774E =====
int __cdecl sub_4C7720(int a1, _BYTE *a2, int a3)
{
  int result; // eax
  int v5; // edi

  result = a3 - 1;
  if ( a3 )
  {
    v5 = a3;
    do
    {
      result = sub_4D4D00(a1, 8);
      *a2++ = result;
      --v5;
    }
    while ( v5 );
  }
  return result;
}

// ===== sub_4C7750 @ 0x004C7750..0x004C7827 =====
int __cdecl sub_4C7750(_DWORD *a1, int a2)
{
  int *v2; // ebx
  int v4; // eax
  int v5; // edx
  int v6; // ebx

  v2 = (int *)a1[7];
  if ( !v2 )
    return -129;
  v4 = sub_4D4D00(a2, 32);
  *a1 = v4;
  if ( v4 )
    return -134;
  a1[1] = sub_4D4D00(a2, 8);
  a1[2] = sub_4D4D00(a2, 32);
  a1[3] = sub_4D4D00(a2, 32);
  a1[4] = sub_4D4D00(a2, 32);
  a1[5] = sub_4D4D00(a2, 32);
  *v2 = 1 << sub_4D4D00(a2, 4);
  v5 = 1 << sub_4D4D00(a2, 4);
  v2[1] = v5;
  if ( (int)a1[2] >= 1 && (int)a1[1] >= 1 )
  {
    v6 = *v2;
    if ( v6 >= 8 && v5 >= v6 && sub_4D4D00(a2, 1) == 1 )
      return 0;
  }
  sub_4C74B0(a1);
  return -133;
}

// ===== sub_4C7830 @ 0x004C7830..0x004C7908 =====
int __cdecl sub_4C7830(_DWORD *a1, int a2)
{
  int v2; // edi
  _BYTE *v3; // eax
  int v4; // eax
  void *v5; // eax
  size_t v6; // ecx
  int i; // edi
  int v8; // ebx

  v2 = sub_4D4D00(a2, 32);
  if ( v2 < 0 )
    goto LABEL_7;
  v3 = calloc(v2 + 1, 1u);
  a1[3] = v3;
  sub_4C7720(a2, v3, v2);
  v4 = sub_4D4D00(a2, 32);
  a1[2] = v4;
  if ( v4 < 0 )
    goto LABEL_7;
  v5 = calloc(v4 + 1, 4u);
  v6 = a1[2] + 1;
  *a1 = v5;
  a1[1] = calloc(v6, 4u);
  for ( i = 0; i < a1[2]; sub_4C7720(a2, *(_BYTE **)(*a1 + 4 * i++), v8) )
  {
    v8 = sub_4D4D00(a2, 32);
    if ( v8 < 0 )
      goto LABEL_7;
    *(_DWORD *)(a1[1] + 4 * i) = v8;
    *(_DWORD *)(*a1 + 4 * i) = calloc(v8 + 1, 1u);
  }
  if ( sub_4D4D00(a2, 1) != 1 )
  {
LABEL_7:
    sub_4C73F0((int)a1);
    return -133;
  }
  return 0;
}

// ===== sub_4C7910 @ 0x004C7910..0x004C7B87 =====
int __cdecl sub_4C7910(_DWORD *a1, int a2)
{
  _DWORD *v2; // ebx
  int v4; // eax
  int v5; // ebp
  _DWORD *v6; // edi
  void *v7; // eax
  int v8; // edi
  int v9; // ebp
  int v10; // eax
  int v11; // ebp
  int *v12; // edi
  unsigned int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // ebp
  int *v17; // edi
  unsigned int v18; // eax
  int v19; // eax
  int v20; // eax
  int v21; // ebp
  int *v22; // edi
  int v23; // eax
  int v24; // eax
  int v25; // eax
  int v26; // ebp
  void **v27; // edi
  int *v28; // eax

  v2 = (_DWORD *)a1[7];
  if ( !v2 )
    return -129;
  v4 = sub_4D4D00(a2, 8) + 1;
  v5 = 0;
  v2[6] = v4;
  if ( v4 > 0 )
  {
    v6 = v2 + 456;
    do
    {
      v7 = calloc(1u, 0x34u);
      *v6 = v7;
      if ( sub_4CBC80(a2, v7) )
        goto LABEL_32;
      ++v5;
      ++v6;
    }
    while ( v5 < v2[6] );
  }
  v8 = sub_4D4D00(a2, 6) + 1;
  v9 = 0;
  if ( v8 > 0 )
  {
    while ( !sub_4D4D00(a2, 16) )
    {
      if ( ++v9 >= v8 )
        goto LABEL_10;
    }
    goto LABEL_32;
  }
LABEL_10:
  v10 = sub_4D4D00(a2, 6) + 1;
  v11 = 0;
  v2[4] = v10;
  if ( v10 > 0 )
  {
    v12 = v2 + 264;
    do
    {
      v13 = sub_4D4D00(a2, 16);
      *(v12 - 64) = v13;
      if ( v13 > 1 )
        goto LABEL_32;
      v14 = (*((int (__cdecl **)(_DWORD *, int))*(&off_502DD8 + v13) + 1))(a1, a2);
      *v12 = v14;
      if ( !v14 )
        goto LABEL_32;
      ++v11;
      ++v12;
    }
    while ( v11 < v2[4] );
  }
  v15 = sub_4D4D00(a2, 6) + 1;
  v16 = 0;
  v2[5] = v15;
  if ( v15 > 0 )
  {
    v17 = v2 + 392;
    do
    {
      v18 = sub_4D4D00(a2, 16);
      *(v17 - 64) = v18;
      if ( v18 > 2 )
        goto LABEL_32;
      v19 = (*((int (__cdecl **)(_DWORD *, int))*(&off_502DE0 + v18) + 1))(a1, a2);
      *v17 = v19;
      if ( !v19 )
        goto LABEL_32;
      ++v16;
      ++v17;
    }
    while ( v16 < v2[5] );
  }
  v20 = sub_4D4D00(a2, 6) + 1;
  v21 = 0;
  v2[3] = v20;
  if ( v20 > 0 )
  {
    v22 = v2 + 136;
    do
    {
      v23 = sub_4D4D00(a2, 16);
      *(v22 - 64) = v23;
      if ( v23 )
        goto LABEL_32;
      v24 = ((int (__cdecl *)(_DWORD *, int))off_502DEC[1])(a1, a2);
      *v22 = v24;
      if ( !v24 )
        goto LABEL_32;
      ++v21;
      ++v22;
    }
    while ( v21 < v2[3] );
  }
  v25 = sub_4D4D00(a2, 6) + 1;
  v26 = 0;
  v2[2] = v25;
  if ( v25 > 0 )
  {
    v27 = (void **)(v2 + 8);
    do
    {
      *v27 = calloc(1u, 0x10u);
      *(_DWORD *)*v27 = sub_4D4D00(a2, 1);
      *((_DWORD *)*v27 + 1) = sub_4D4D00(a2, 16);
      *((_DWORD *)*v27 + 2) = sub_4D4D00(a2, 16);
      *((_DWORD *)*v27 + 3) = sub_4D4D00(a2, 8);
      v28 = (int *)*v27;
      if ( *((int *)*v27 + 1) >= 1 || v28[2] >= 1 || v28[3] >= v2[3] )
        goto LABEL_32;
      ++v26;
      ++v27;
    }
    while ( v26 < v2[2] );
  }
  if ( sub_4D4D00(a2, 1) != 1 )
  {
LABEL_32:
    sub_4C74B0(a1);
    return -133;
  }
  return 0;
}

// ===== sub_4C7B90 @ 0x004C7B90..0x004C7BDC =====
int __cdecl sub_4C7B90(_DWORD *a1, _DWORD *a2)
{
  _DWORD *v2; // edi

  memset(a2, 0, 0x70u);
  a2[16] = a1;
  a2[19] = 0;
  a2[17] = 0;
  if ( *a1 )
  {
    v2 = calloc(1u, 0x48u);
    a2[26] = v2;
    sub_4D4A60(a2 + 1);
    v2[1] = -971228160;
  }
  return 0;
}

// ===== sub_4C7BE0 @ 0x004C7BE0..0x004C7C4C =====
int __cdecl sub_4C7BE0(_DWORD *a1, int a2)
{
  size_t v2; // edi
  _DWORD *v3; // eax
  int v4; // ecx
  int result; // eax

  v2 = (a2 + 7) & 0xFFFFFFF8;
  if ( (signed int)(v2 + a1[18]) > a1[19] )
  {
    if ( a1[17] )
    {
      v3 = malloc(8u);
      a1[20] += a1[18];
      v3[1] = a1[21];
      *v3 = a1[17];
      a1[21] = v3;
    }
    a1[19] = v2;
    a1[17] = malloc(v2);
    a1[18] = 0;
  }
  v4 = a1[18];
  result = v4 + a1[17];
  a1[18] = v2 + v4;
  return result;
}

// ===== sub_4C7C50 @ 0x004C7C50..0x004C7CB9 =====
int __cdecl sub_4C7C50(int a1)
{
  int v1; // esi
  int v2; // ebx
  int result; // eax
  void *v4; // eax
  int v5; // ecx

  v1 = *(_DWORD *)(a1 + 84);
  if ( v1 )
  {
    do
    {
      v2 = *(_DWORD *)(v1 + 4);
      free(*(void **)v1);
      *(_DWORD *)v1 = 0;
      *(_DWORD *)(v1 + 4) = 0;
      free((void *)v1);
      v1 = v2;
    }
    while ( v2 );
  }
  result = *(_DWORD *)(a1 + 80);
  if ( result )
  {
    v4 = realloc(*(void **)(a1 + 68), result + *(_DWORD *)(a1 + 76));
    v5 = *(_DWORD *)(a1 + 76);
    *(_DWORD *)(a1 + 68) = v4;
    result = *(_DWORD *)(a1 + 80);
    *(_DWORD *)(a1 + 80) = 0;
    *(_DWORD *)(a1 + 76) = result + v5;
  }
  *(_DWORD *)(a1 + 72) = 0;
  *(_DWORD *)(a1 + 84) = 0;
  return result;
}

// ===== sub_4C7CC0 @ 0x004C7CC0..0x004C7D11 =====
int __cdecl sub_4C7CC0(int a1)
{
  _DWORD *v1; // eax
  int result; // eax

  v1 = *(_DWORD **)(a1 + 64);
  if ( v1 && *v1 )
    sub_4D4AC0(a1 + 4);
  sub_4C7C50(a1);
  if ( *(_DWORD *)(a1 + 68) )
    free(*(void **)(a1 + 68));
  if ( *(_DWORD *)(a1 + 104) )
    free(*(void **)(a1 + 104));
  result = 0;
  memset((void *)a1, 0, 0x70u);
  return result;
}

// ===== sub_4C7D20 @ 0x004C7D20..0x004C80A0 =====
int __cdecl sub_4C7D20(_DWORD *a1, _DWORD *a2, int a3)
{
  _DWORD *v4; // esi
  _DWORD *v5; // edi
  _DWORD *v6; // ebp
  bool v7; // cc
  _DWORD *v8; // ebp
  void **v9; // ebp
  int i; // ebp
  int v11; // eax
  _DWORD *v12; // ebp
  int v13; // eax
  char *v14; // edx
  _DWORD *v15; // ebp
  int v16; // eax
  char *v17; // edx
  int v19; // [esp+14h] [ebp+4h]
  int v20; // [esp+14h] [ebp+4h]
  int v21; // [esp+14h] [ebp+4h]
  int v22; // [esp+14h] [ebp+4h]
  int v23; // [esp+14h] [ebp+4h]
  int v24; // [esp+1Ch] [ebp+Ch]
  int v25; // [esp+1Ch] [ebp+Ch]
  int v26; // [esp+1Ch] [ebp+Ch]

  v4 = (_DWORD *)a2[7];
  memset(a1, 0, 0x70u);
  v5 = calloc(1u, 0xB8u);
  a1[1] = a2;
  a1[26] = v5;
  v5[11] = sub_4D1C30(v4[2]);
  v5[3] = calloc(1u, 4u);
  v5[4] = calloc(1u, 4u);
  *(_DWORD *)v5[3] = calloc(1u, 0x14u);
  *(_DWORD *)v5[4] = calloc(1u, 0x14u);
  sub_4CE200(*(_DWORD *)v5[3], *v4);
  sub_4CE200(*(_DWORD *)v5[4], v4[1]);
  v5[1] = sub_4CE050(0, *v4 / 2);
  v5[2] = sub_4CE050(0, v4[1] / 2);
  if ( a3 )
  {
    sub_4CDE20(v5 + 5, *v4);
    sub_4CDE20(v5 + 8, v4[1]);
    if ( !v4[712] )
    {
      v4[712] = calloc(v4[6], 0x2Cu);
      v19 = 0;
      if ( (int)v4[6] > 0 )
      {
        v24 = 0;
        v6 = v4 + 456;
        do
        {
          sub_4CB7D0(v4[712] + v24, *v6++);
          v7 = ++v19 < v4[6];
          v24 += 44;
        }
        while ( v7 );
      }
    }
    v5[14] = calloc(v4[7], 0x30u);
    v20 = 0;
    if ( (int)v4[7] > 0 )
    {
      v25 = 0;
      v8 = v4 + 713;
      do
      {
        sub_4C8AC0(v5[14] + v25, *v8, v4 + 717, v4[*(_DWORD *)*v8] / 2, a2[2]);
        ++v8;
        v7 = ++v20 < v4[7];
        v25 += 48;
      }
      while ( v7 );
    }
    *a1 = 1;
  }
  else if ( !v4[712] )
  {
    v4[712] = calloc(v4[6], 0x2Cu);
    v21 = 0;
    if ( (int)v4[6] > 0 )
    {
      v26 = 0;
      v9 = (void **)(v4 + 456);
      do
      {
        sub_4CB830(v4[712] + v26, *v9);
        sub_4CB750(*v9);
        *v9++ = 0;
        ++v21;
        v26 += 44;
      }
      while ( v21 < v4[6] );
    }
  }
  a1[4] = v4[1];
  a1[2] = malloc(4 * a2[1]);
  a1[3] = malloc(4 * a2[1]);
  for ( i = 0; i < a2[1]; ++i )
    *(_DWORD *)(a1[2] + 4 * i) = calloc(a1[4], 4u);
  a1[9] = 0;
  a1[10] = 0;
  v11 = v4[1] / 2;
  a1[12] = v11;
  a1[5] = v11;
  v5[12] = calloc(v4[4], 4u);
  v5[13] = calloc(v4[5], 4u);
  v22 = 0;
  if ( (int)v4[4] > 0 )
  {
    v12 = v4 + 264;
    do
    {
      v13 = (*((int (__cdecl **)(_DWORD *, _DWORD))*(&off_502DD8 + *(v12 - 64)) + 2))(a1, *v12);
      v14 = (char *)v12++ - 1056 - (_DWORD)v4;
      *(_DWORD *)&v14[v5[12]] = v13;
      ++v22;
    }
    while ( v22 < v4[4] );
  }
  v23 = 0;
  if ( (int)v4[5] > 0 )
  {
    v15 = v4 + 392;
    do
    {
      v16 = (*((int (__cdecl **)(_DWORD *, _DWORD))*(&off_502DE0 + *(v15 - 64)) + 2))(a1, *v15);
      v17 = (char *)v15++ - 1568 - (_DWORD)v4;
      *(_DWORD *)&v17[v5[13]] = v16;
      ++v23;
    }
    while ( v23 < v4[5] );
  }
  return 0;
}
