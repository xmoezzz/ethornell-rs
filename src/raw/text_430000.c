#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_430030 @ 0x00430030..0x00430194 =====
int __stdcall sub_430030(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  char *v4; // eax
  char *v5; // edi
  HDC v6; // eax
  int result; // eax
  tagPAINTSTRUCT Paint; // [esp+10h] [ebp-48h] BYREF

  v4 = sub_42FC10((int)hWnd);
  v5 = v4;
  if ( Msg > 0x10 )
  {
    if ( Msg == 256 )
    {
      if ( wParam == 67 && GetAsyncKeyState(17) < 0 )
      {
        if ( *((_DWORD *)v5 + 9) )
          sub_4301E0();
        return 0;
      }
    }
    else if ( Msg != 257 )
    {
      return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    PostMessageA(hWndParent, Msg, wParam, lParam);
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
  if ( Msg != 16 )
  {
    if ( Msg == 2 )
    {
      operator delete[](*((void **)v4 + 3));
      operator delete(*((void **)v5 + 9));
      result = 0;
      *(_DWORD *)v5 = 0;
      *((_DWORD *)v5 + 1) = 0;
      *((_DWORD *)v5 + 2) = 0;
      *((_DWORD *)v5 + 3) = 0;
      *((_DWORD *)v5 + 4) = 0;
      *((_DWORD *)v5 + 5) = 0;
      *((_DWORD *)v5 + 6) = 0;
      *((_DWORD *)v5 + 7) = 0;
      *((_DWORD *)v5 + 8) = 0;
      *((_DWORD *)v5 + 9) = 0;
      return result;
    }
    if ( Msg == 15 )
    {
      v6 = BeginPaint(hWnd, &Paint);
      sub_4617E0(v6, 0, 0, v5 + 12, 0);
      EndPaint(hWnd, &Paint);
      return 0;
    }
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
  if ( *((_DWORD *)v4 + 2) )
  {
    if ( *((_DWORD *)v4 + 1) )
      sub_45FFB0(0);
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
  return 0;
}

// ===== sub_4301A0 @ 0x004301A0..0x004301DC =====
void sub_4301A0()
{
  __m128i *v0; // esi

  if ( dword_565B68 )
  {
    v0 = xmmword_50C900;
    do
    {
      if ( v0->m128i_i32[0] )
      {
        if ( v0->m128i_i32[1] )
          RedrawWindow((HWND)v0->m128i_i32[0], 0, 0, 0x401u);
      }
      v0 = (__m128i *)((char *)v0 + 40);
    }
    while ( (int)v0 < (int)&dword_50CA40 );
  }
}

// ===== sub_4301E0 @ 0x004301E0..0x00430253 =====
int __usercall sub_4301E0@<eax>(const char *a1@<eax>)
{
  const char *v1; // esi
  HGLOBAL v2; // eax
  void *v3; // edi
  int v4; // eax
  char v5; // cl

  v1 = a1;
  v2 = GlobalAlloc(0x2042u, strlen(a1) + 1);
  v3 = v2;
  if ( !v2 )
    return 0;
  v4 = (_BYTE *)GlobalLock(v2) - v1;
  do
  {
    v5 = *v1;
    v1[v4] = *v1;
    ++v1;
  }
  while ( v5 );
  GlobalUnlock(v3);
  if ( !OpenClipboard(hWndParent) )
    return 0;
  EmptyClipboard();
  SetClipboardData(1u, v3);
  CloseClipboard();
  return 1;
}

// ===== sub_430260 @ 0x00430260..0x00430272 =====
_DWORD *__usercall sub_430260@<eax>(_DWORD *result@<eax>)
{
  *result = &CMemoryDX::`vftable';
  result[1] = 0;
  result[2] = 0;
  result[3] = 0;
  return result;
}

// ===== sub_430280 @ 0x00430280..0x004302A1 =====
void *__thiscall sub_430280(void *this, char a2)
{
  sub_4302B0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4302B0 @ 0x004302B0..0x004302C0 =====
int __thiscall sub_4302B0(_DWORD *this)
{
  *this = &CMemoryDX::`vftable';
  return sub_4302E0();
}

// ===== sub_4302C0 @ 0x004302C0..0x004302E0 =====
void *__usercall sub_4302C0@<eax>(size_t a1@<edi>, _DWORD *a2@<esi>)
{
  void *result; // eax

  result = 0;
  if ( !a2[1] )
  {
    result = _aligned_malloc(a1, 0x10u);
    a2[2] = result;
    a2[3] = a1;
    a2[1] = 1;
  }
  return result;
}

// ===== sub_4302E0 @ 0x004302E0..0x0043030D =====
int __usercall sub_4302E0@<eax>(int a1@<esi>)
{
  int v1; // edi

  v1 = *(_DWORD *)(a1 + 4);
  if ( v1 )
  {
    _aligned_free(*(void **)(a1 + 8));
    *(_DWORD *)(a1 + 4) = 0;
    *(_DWORD *)(a1 + 8) = 0;
    *(_DWORD *)(a1 + 12) = 0;
  }
  return v1;
}

// ===== sub_430310 @ 0x00430310..0x00430344 =====
_DWORD *__usercall sub_430310@<eax>(_DWORD *a1@<esi>)
{
  _DWORD *v1; // eax

  *a1 = 0x8000;
  a1[1] = operator new(0x8000u);
  v1 = operator new(0xCu);
  *v1 = 0;
  v1[1] = 0x8000;
  v1[2] = 0;
  a1[4] = v1;
  a1[7] = 0;
  return a1;
}

// ===== sub_430350 @ 0x00430350..0x004303A0 =====
void __usercall sub_430350(int a1@<edi>)
{
  _DWORD *v1; // eax
  _DWORD *v2; // esi
  _DWORD *v3; // eax
  _DWORD *v4; // esi

  v1 = *(_DWORD **)(a1 + 16);
  if ( v1 )
  {
    do
    {
      v2 = (_DWORD *)v1[2];
      operator delete(v1);
      v1 = v2;
    }
    while ( v2 );
  }
  v3 = *(_DWORD **)(a1 + 28);
  if ( v3 )
  {
    do
    {
      v4 = (_DWORD *)v3[2];
      operator delete(v3);
      v3 = v4;
    }
    while ( v4 );
  }
  operator delete(*(void **)(a1 + 4));
}

// ===== sub_4303A0 @ 0x004303A0..0x004304A8 =====
int __userpurge sub_4303A0@<eax>(int a1@<esi>, unsigned int a2)
{
  _DWORD *v2; // edi
  size_t v3; // ebx
  _DWORD *v4; // eax
  size_t v5; // edx
  _DWORD *v6; // ebx
  unsigned int v7; // eax
  void *v9; // [esp+8h] [ebp-8h]
  _DWORD *v10; // [esp+Ch] [ebp-4h]

  while ( 1 )
  {
    v2 = *(_DWORD **)(a1 + 16);
    v10 = (_DWORD *)(a1 + 8);
    if ( v2 )
      break;
LABEL_4:
    v3 = 2 * *(_DWORD *)a1;
    if ( *(_DWORD *)a1 < a2 )
    {
      do
        v3 *= 2;
      while ( v3 - *(_DWORD *)a1 < a2 );
    }
    v9 = operator new(v3);
    memcpy_0(v9, *(const void **)(a1 + 4), *(_DWORD *)a1);
    operator delete(*(void **)(a1 + 4));
    v4 = operator new(0xCu);
    *v4 = *(_DWORD *)a1;
    v5 = v3 - *(_DWORD *)a1;
    v4[2] = 0;
    v4[1] = v5;
    v10[2] = v4;
    *(_DWORD *)a1 = v3;
    *(_DWORD *)(a1 + 4) = v9;
    sub_430520();
  }
  while ( a2 > v2[1] )
  {
    v10 = v2;
    v2 = (_DWORD *)v2[2];
    if ( !v2 )
      goto LABEL_4;
  }
  v6 = operator new(0xCu);
  *v6 = *v2;
  v6[1] = a2;
  v6[2] = *(_DWORD *)(a1 + 28);
  *(_DWORD *)(a1 + 28) = v6;
  v7 = v2[1];
  if ( a2 >= v7 )
  {
    v10[2] = v2[2];
    operator delete(v2);
  }
  else
  {
    *v2 += a2;
    v2[1] = v7 - a2;
  }
  return *v6;
}

// ===== sub_4304B0 @ 0x004304B0..0x00430502 =====
int __usercall sub_4304B0@<eax>(int a1@<eax>, _DWORD *a2@<esi>)
{
  _DWORD *v2; // ecx
  int result; // eax
  _DWORD *v5; // edx
  _DWORD *v6; // edx
  _DWORD *i; // eax

  v2 = (_DWORD *)a2[7];
  result = 0;
  v5 = a2 + 5;
  if ( v2 )
  {
    while ( a1 != *v2 )
    {
      v5 = v2;
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return result;
    }
    v5[2] = v2[2];
    v6 = (_DWORD *)a2[4];
    for ( i = a2 + 2; v6; v6 = (_DWORD *)v6[2] )
    {
      if ( *v2 < *v6 )
        break;
      i = v6;
    }
    v2[2] = v6;
    i[2] = v2;
    sub_430520();
    return 1;
  }
  return result;
}

// ===== sub_430510 @ 0x00430510..0x0043051D =====
int __userpurge sub_430510@<eax>(int a1@<eax>, int a2)
{
  return a2 + *(_DWORD *)(a1 + 4);
}

// ===== sub_430520 @ 0x00430520..0x00430562 =====
_DWORD *__usercall sub_430520@<eax>(_DWORD *result@<eax>)
{
  _DWORD *v1; // esi
  int v2; // ecx

  v1 = (_DWORD *)result[4];
  if ( v1 )
  {
    result = (_DWORD *)v1[2];
    while ( result )
    {
      v2 = v1[1];
      if ( v2 + *v1 == *result )
      {
        v1[1] = v2 + result[1];
        v1[2] = result[2];
        operator delete(result);
        result = (_DWORD *)v1[2];
      }
      else
      {
        v1 = result;
        result = (_DWORD *)result[2];
      }
    }
  }
  return result;
}

// ===== sub_430570 @ 0x00430570..0x0043064C =====
int __thiscall sub_430570(void *this, _DWORD *a2, int a3, int a4)
{
  int v4; // edx
  int v5; // eax
  int result; // eax

  a2[17] = a3;
  a2[21] = this;
  *a2 = &CObjectManager::`vftable';
  sub_431120(a4);
  if ( v4 )
  {
    a2[26] = v4;
    a2[27] = 0;
  }
  else
  {
    if ( operator new(0x40u) )
      v5 = sub_4461E0(1);
    else
      v5 = 0;
    a2[26] = v5;
    a2[27] = 1;
  }
  a2[1] = 0;
  a2[2] = 0;
  a2[3] = 0;
  a2[4] = 0;
  a2[5] = 0;
  a2[9] = 0;
  a2[10] = 0;
  a2[11] = 0;
  a2[12] = 0;
  a2[13] = 0;
  a2[14] = 0;
  a2[15] = 0;
  sub_430CD0();
  sub_430D20(a2);
  sub_431140(0);
  result = sub_430D10();
  a2[23] = 0;
  a2[24] = 0;
  a2[25] = 0;
  return result;
}

// ===== sub_430650 @ 0x00430650..0x00430671 =====
void *__thiscall sub_430650(void *this, char a2)
{
  sub_430680();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_430680 @ 0x00430680..0x004306A7 =====
int __thiscall sub_430680(_DWORD *this)
{
  int result; // eax
  int (__thiscall ***v3)(_DWORD, int); // ecx

  *this = &CObjectManager::`vftable';
  result = sub_4306B0();
  if ( this[27] )
  {
    v3 = (int (__thiscall ***)(_DWORD, int))this[26];
    if ( v3 )
      return (**v3)(v3, 1);
  }
  return result;
}

// ===== sub_4306B0 @ 0x004306B0..0x004306F0 =====
int __usercall sub_4306B0@<eax>(int a1@<eax>)
{
  while ( *(_DWORD *)(a1 + 12) )
    sub_430770(a1);
  while ( *(_DWORD *)(a1 + 20) )
    sub_4312E0();
  return sub_430CD0();
}

// ===== sub_4306F0 @ 0x004306F0..0x00430762 =====
unsigned int __userpurge sub_4306F0@<eax>(int a1@<eax>, int a2)
{
  unsigned int result; // eax
  unsigned int v5; // eax
  unsigned int *v6; // edi
  unsigned int *v7; // esi
  unsigned int *v8; // eax
  unsigned int i; // [esp+10h] [ebp+8h]

  if ( (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 16))(a2) )
    return sub_4312B0(a2);
  v5 = (*(int (__thiscall **)(int))(*(_DWORD *)a2 + 28))(a2);
  v6 = (unsigned int *)(a1 + 4);
  v7 = *(unsigned int **)(a1 + 12);
  for ( i = v5; v7; v7 = (unsigned int *)v7[2] )
  {
    if ( *v7 > v5 )
      break;
    v6 = v7;
  }
  v8 = (unsigned int *)operator new(0xCu);
  v6[2] = (unsigned int)v8;
  *v8 = i;
  *(_DWORD *)(v6[2] + 4) = a2;
  result = v6[2];
  *(_DWORD *)(result + 8) = v7;
  return result;
}

// ===== sub_430770 @ 0x00430770..0x004307CC =====
int __usercall sub_430770@<eax>(int a1@<eax>, int a2@<ecx>)
{
  _DWORD *v5; // eax
  _DWORD *v6; // ecx

  if ( !a1 )
    return 0;
  if ( (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1) )
    return sub_4312E0();
  v5 = *(_DWORD **)(a2 + 12);
  v6 = (_DWORD *)(a2 + 4);
  if ( !v5 )
    return 0;
  while ( v5[1] != a1 )
  {
    v6 = v5;
    v5 = (_DWORD *)v5[2];
    if ( !v5 )
      return 0;
  }
  v6[2] = v5[2];
  operator delete(v5);
  return 1;
}

// ===== sub_4307D0 @ 0x004307D0..0x004307F1 =====
int __usercall sub_4307D0@<eax>(int a1@<edi>, int a2@<esi>)
{
  if ( !sub_430770(a2, a1) )
    return 0;
  sub_4306F0(a1, a2);
  return 1;
}

// ===== sub_430830 @ 0x00430830..0x00430CC9 =====
void __thiscall sub_430830(int *this, unsigned int a2, int *a3)
{
  int v3; // ebx
  int v4; // edi
  int v5; // edx
  int *v6; // esi
  _DWORD *v7; // ecx
  int v8; // ecx
  int v9; // edx
  int v10; // eax
  int v11; // eax
  int v12; // edx
  unsigned int v13; // eax
  unsigned int v14; // ecx
  int v15; // eax
  bool v16; // cc
  int v17; // eax
  bool v18; // zf
  int v19; // eax
  int v20; // eax
  int v21; // edx
  int v22; // ecx
  int v23; // ebx
  int v24; // edi
  int v25; // eax
  int *v26; // eax
  int v27; // edx
  unsigned int v28; // eax
  int v29; // eax
  int v30; // ecx
  unsigned int v31; // esi
  unsigned int v32; // eax
  int *v33; // edx
  int v34; // [esp-8h] [ebp-60h]
  int *v36; // [esp+10h] [ebp-48h]
  int v37; // [esp+18h] [ebp-40h]
  int v38; // [esp+1Ch] [ebp-3Ch]
  unsigned int v39; // [esp+20h] [ebp-38h]
  int v40; // [esp+24h] [ebp-34h] BYREF
  int v41; // [esp+28h] [ebp-30h]
  int v42; // [esp+2Ch] [ebp-2Ch]
  int v43; // [esp+30h] [ebp-28h]
  int v44; // [esp+34h] [ebp-24h] BYREF
  int v45; // [esp+38h] [ebp-20h]
  int v46; // [esp+3Ch] [ebp-1Ch]
  int v47; // [esp+40h] [ebp-18h]
  int v48; // [esp+44h] [ebp-14h] BYREF
  int v49; // [esp+48h] [ebp-10h]
  int v50; // [esp+4Ch] [ebp-Ch]
  int v51; // [esp+50h] [ebp-8h]

  if ( this[16] >= this[17] )
  {
    sub_430D10();
    return;
  }
  if ( !sub_409110(a3, (int *)(this[21] + 24)) )
    return;
  v3 = *a3;
  v4 = a3[1];
  v5 = a3[2];
  v43 = a3[3];
  v6 = (int *)this[15];
  v40 = v3;
  v41 = v4;
  v42 = v5;
  v36 = this + 10;
  if ( !v6 )
  {
LABEL_60:
    v26 = (int *)operator new(0x18u);
    *v26 = v40;
    v26[1] = v41;
    v26[2] = v42;
    v27 = v43;
    v26[4] = a2;
    v26[3] = v27;
    v26[5] = this[15];
    ++this[16];
    this[15] = (int)v26;
    return;
  }
  while ( 1 )
  {
    if ( !sub_4090E0(v6, &v40) )
    {
      v20 = *v6;
      v21 = v42;
      v22 = v43;
      v48 = v3;
      v49 = v4;
      v50 = v42;
      v51 = v43;
      if ( v3 >= v20 )
        v48 = v20;
      v23 = v6[1];
      if ( v4 >= v23 )
        v49 = v6[1];
      v24 = v6[2];
      if ( v42 < v24 )
      {
        v21 = v6[2];
        v50 = v21;
      }
      v25 = v6[3];
      if ( v43 < v25 )
      {
        v22 = v6[3];
        v51 = v22;
      }
      if ( (v42 - v40 + 1) * (v43 - v41 + 1) + (v24 - *v6 + 1) * (v25 - v23 + 1) == (v21 - v48 + 1) * (v22 - v49 + 1) )
      {
        v36[5] = v6[5];
        --this[16];
        v32 = v6[4];
        if ( a2 <= v32 )
          v32 = a2;
        v33 = &v48;
        goto LABEL_77;
      }
      goto LABEL_58;
    }
    if ( sub_4090B0(v6, v7) )
      return;
    if ( v3 > *v6 )
      v3 = *v6;
    v8 = v42;
    v44 = v3;
    if ( v42 < v6[2] )
      v8 = v6[2];
    v46 = v8;
    v9 = v6[1];
    if ( v4 <= v9 )
      v9 = v4;
    v45 = v9;
    v10 = v6[3];
    if ( v43 >= v10 )
      v10 = v43;
    v47 = v10;
    v11 = v10 - v9;
    v12 = v6[1];
    v39 = (v8 - v3 + 1) * (v11 + 1);
    v38 = v6[2];
    v37 = *v6;
    v13 = (v38 - *v6 + 1) * (v6[3] - v12 + 1);
    v14 = (v42 - v40 + 1) * (v43 - v4 + 1);
    if ( v13 > v14 )
      v14 >>= 1;
    else
      v13 >>= 1;
    if ( v39 <= v14 + v13 )
    {
LABEL_74:
      v36[5] = v6[5];
      --this[16];
      v32 = v6[4];
      if ( a2 <= v32 )
        v32 = a2;
      v33 = &v44;
LABEL_77:
      sub_430830(v32, v33);
      operator delete(v6);
      return;
    }
    v15 = v6[3];
    v16 = v4 < v12;
    if ( v4 == v12 )
    {
      if ( v15 == v43 )
        goto LABEL_74;
      v16 = v4 < v12;
    }
    if ( v16 )
      goto LABEL_37;
    if ( v43 <= v15 )
    {
      if ( v40 > v37 )
        v40 = *v6;
      if ( v42 < v6[2] )
        v42 = v6[2];
      v17 = v6[1];
      v18 = v17 == v4;
      if ( v17 < v4 )
      {
        if ( v43 < v6[3] )
        {
          v44 = *v6;
          v19 = v6[2];
          v45 = v43 + 1;
          v46 = v19;
          v47 = v6[3];
          v34 = v6[4];
          v6[3] = v4 - 1;
          sub_430830(v34, &v44);
          goto LABEL_34;
        }
        v18 = v17 == v4;
      }
      if ( v18 )
      {
        v6[1] = v43 + 1;
        goto LABEL_34;
      }
LABEL_33:
      v6[3] = v4 - 1;
      goto LABEL_34;
    }
    if ( v4 <= v12 )
    {
LABEL_37:
      if ( v15 <= v43 )
        break;
    }
    v3 = v40;
    if ( v40 >= v37 )
    {
      if ( v42 <= v38 )
      {
        if ( v4 >= v12 )
        {
          v4 = v6[3] + 1;
          v41 = v4;
        }
        else
        {
          v43 = v12 - 1;
        }
        if ( a2 > v6[4] )
          a2 = v6[4];
        goto LABEL_59;
      }
      if ( v40 > v37 )
        goto LABEL_68;
    }
    if ( v38 > v42 )
    {
LABEL_68:
      if ( v4 >= v12 )
      {
        v45 = v4;
        v47 = v6[3];
        v41 = v47 + 1;
        v6[3] = v4 - 1;
      }
      else
      {
        v45 = v6[1];
        v47 = v43;
        v30 = v43 + 1;
        v43 = v12 - 1;
        v6[1] = v30;
      }
      sub_430830(a2, &v40);
      v31 = v6[4];
      if ( a2 <= v31 )
        v31 = a2;
      sub_430830(v31, &v44);
      return;
    }
    if ( v4 >= v12 )
      goto LABEL_33;
    v6[1] = v43 + 1;
LABEL_34:
    if ( a2 > v6[4] )
      a2 = v6[4];
LABEL_58:
    v4 = v41;
    v3 = v40;
LABEL_59:
    v36 = v6;
    v6 = (int *)v6[5];
    if ( !v6 )
      goto LABEL_60;
  }
  v36[5] = v6[5];
  --this[16];
  v45 = v6[1];
  v47 = v6[3];
  v28 = v6[4];
  if ( a2 <= v28 )
    v28 = a2;
  sub_430830(v28, &v44);
  v44 = v40;
  v46 = v42;
  if ( v41 < v6[1] )
  {
    v45 = v41;
    v47 = v6[1] - 1;
    sub_430830(a2, &v44);
  }
  v29 = v6[3];
  if ( v29 < v43 )
  {
    v47 = v43;
    v45 = v29 + 1;
    sub_430830(a2, &v44);
  }
  operator delete(v6);
}

// ===== sub_430CD0 @ 0x00430CD0..0x00430D07 =====
void __usercall sub_430CD0(_DWORD *a1@<edi>)
{
  _DWORD *v1; // esi
  void *v2; // [esp-4h] [ebp-8h]

  v1 = (_DWORD *)a1[15];
  while ( v1 )
  {
    v2 = v1;
    v1 = (_DWORD *)v1[5];
    operator delete(v2);
  }
  a1[15] = 0;
  a1[16] = 0;
  a1[19] = 0;
}

// ===== sub_430D10 @ 0x00430D10..0x00430D18 =====
int __usercall sub_430D10@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 76) = 1;
  return result;
}

// ===== sub_430D20 @ 0x00430D20..0x00430D27 =====
int __usercall sub_430D20@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int result; // eax

  result = a1 << 16;
  *(_DWORD *)(a2 + 72) = result;
  return result;
}

// ===== sub_430D30 @ 0x00430D30..0x00430DC6 =====
int __userpurge sub_430D30@<eax>(_DWORD *a1@<eax>, int a2, int a3)
{
  int v3; // edx
  int v4; // ecx
  int v5; // edx
  int v6; // ecx
  int v7; // edx
  int result; // eax
  int i; // esi
  _DWORD v10[6]; // [esp+10h] [ebp-28h] BYREF
  int v11[4]; // [esp+28h] [ebp-10h] BYREF

  v3 = a1[1];
  v10[0] = *a1;
  v4 = a1[2];
  v10[1] = v3;
  v5 = a1[3];
  v10[2] = v4;
  v6 = a1[4];
  v10[3] = v5;
  v7 = a1[5];
  v10[4] = v6;
  v10[5] = v7;
  sub_409190(v11, (int)v10);
  result = a2;
  for ( i = *(_DWORD *)(a2 + 12); i; i = *(_DWORD *)(i + 8) )
  {
    if ( ((a3 << 16) | 0xFFFFu) >= (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(i + 4) + 28))(*(_DWORD *)(i + 4)) )
      sub_41AFD0(v10, *(_DWORD *)(i + 4), v11, 0);
    result = sub_431450(a2, v10, v11, 0);
  }
  return result;
}

// ===== sub_430DD0 @ 0x00430DD0..0x00430EEB =====
int __stdcall sub_430DD0(_DWORD *a1)
{
  int v2; // esi
  _DWORD *v3; // eax
  int v4; // ecx
  _DWORD *v5; // edi
  unsigned int v6; // ecx
  _DWORD *v7; // eax
  _DWORD *v8; // eax
  int v10; // [esp+Ch] [ebp-8h]
  _DWORD *v11; // [esp+10h] [ebp-4h]
  unsigned int v12; // [esp+1Ch] [ebp+8h]

  v10 = (*(int (__thiscall **)(_DWORD *))(*a1 + 4))(a1);
  if ( v10 )
  {
    v2 = sub_431200(0);
    v12 = v2;
  }
  else
  {
    v12 = 1;
    v2 = 1;
  }
  a1[23] = v2;
  v3 = operator new[](20 * v2);
  v4 = 0;
  a1[24] = v3;
  a1[25] = 0;
  v5 = v3;
  if ( v12 <= 1 )
  {
    v8 = (_DWORD *)a1[21];
    *v5 = v8[6];
    v5[1] = v8[7];
    v5[2] = v8[8];
    v5[3] = v8[9];
    v5[4] = 0;
  }
  else
  {
    LOBYTE(v4) = (unsigned __int64)v12 >> 28 != 0;
    v11 = operator new[]((16 * v12) | -v4);
    sub_431200(v11);
    v6 = v12;
    v7 = v11;
    do
    {
      *v5 = *v7;
      v5[1] = v7[1];
      v5[2] = v7[2];
      v5[3] = v7[3];
      v5[4] = 0;
      v5 += 5;
      v7 += 4;
      --v6;
    }
    while ( v6 );
    operator delete[](v11);
  }
  sub_4314F0(v10);
  operator delete[]((void *)a1[24]);
  sub_430CD0(a1);
  return sub_431150();
}

// ===== sub_430EF0 @ 0x00430EF0..0x00431112 =====
int __stdcall sub_430EF0(_DWORD *a1, _DWORD *a2)
{
  _DWORD *v2; // ebx
  int i; // esi
  int j; // esi
  _BYTE *v5; // eax
  _DWORD *v6; // esi
  char *v7; // edi
  _DWORD *v8; // edi
  int v9; // eax
  _DWORD *v10; // eax
  int v11; // ecx
  _DWORD *v12; // edi
  unsigned int *v13; // ebx
  _DWORD *v14; // esi
  int v15; // eax
  unsigned int v16; // ecx
  _DWORD *v17; // eax
  int v19; // [esp+8h] [ebp-18h]
  _BYTE *v20; // [esp+Ch] [ebp-14h]
  _DWORD *v21; // [esp+10h] [ebp-10h]
  int v22; // [esp+10h] [ebp-10h]
  char *v23; // [esp+14h] [ebp-Ch]
  _DWORD *v24; // [esp+14h] [ebp-Ch]
  unsigned int *v25; // [esp+18h] [ebp-8h]
  int v26; // [esp+1Ch] [ebp-4h]

  v2 = a1;
  for ( i = a1[3]; i; i = *(_DWORD *)(i + 8) )
    sub_41AEE0();
  for ( j = a1[5]; j; j = *(_DWORD *)(j + 4) )
    sub_41AEE0();
  if ( (*(int (__thiscall **)(_DWORD *))(*a1 + 8))(a1) )
  {
    v25 = (unsigned int *)operator new[](4 * a1[17]);
    v5 = operator new[](4 * a1[17]);
    v6 = (_DWORD *)a1[15];
    v7 = 0;
    v20 = v5;
    v23 = 0;
    v26 = 0;
    if ( v6 )
    {
      v8 = a2;
      v21 = v5;
      v19 = (char *)v25 - v5;
      do
      {
        v9 = sub_431200(0);
        v23 += v9;
        ++v26;
        *(_DWORD *)((char *)v21 + v19) = v9;
        *v21 = v6[4];
        *v8 = *v6;
        v8[1] = v6[1];
        v8[2] = v6[2];
        v8[3] = v6[3];
        v6 = (_DWORD *)v6[5];
        v8 += 4;
        ++v21;
      }
      while ( v6 );
      v7 = v23;
    }
    a1[23] = v7;
    v10 = operator new[](20 * (_DWORD)v7);
    v11 = a1[21];
    a1[24] = v10;
    v12 = v10;
    a1[25] = 0;
    v24 = operator new[](16 * (*(_DWORD *)(v11 + 36) - *(_DWORD *)(v11 + 28) + 1));
    if ( v26 )
    {
      v13 = v25;
      v14 = a2;
      v15 = v20 - (_BYTE *)v25;
      v22 = v26;
      while ( 1 )
      {
        if ( *v13 <= 1 )
        {
          *v12 = *v14;
          v12[1] = v14[1];
          v12[2] = v14[2];
          v12[3] = v14[3];
          v12[4] = *(unsigned int *)((char *)v13 + v15);
          v12 += 5;
        }
        else
        {
          sub_431200(v24);
          v16 = 0;
          if ( *v13 )
          {
            v17 = v24;
            do
            {
              *v12 = *v17;
              v12[1] = v17[1];
              v12[2] = v17[2];
              v12[3] = v17[3];
              v12[4] = *(unsigned int *)((char *)v13 + v20 - (_BYTE *)v25);
              ++v16;
              v12 += 5;
              v17 += 4;
            }
            while ( v16 < *v13 );
          }
        }
        v14 += 4;
        ++v13;
        if ( !--v22 )
          break;
        v15 = v20 - (_BYTE *)v25;
      }
      v2 = a1;
    }
    sub_4314F0(1);
    operator delete[](v25);
    operator delete[](v20);
    operator delete[]((void *)v2[24]);
    operator delete[](v24);
    sub_430CD0(v2);
    sub_431150();
    return v26;
  }
  else
  {
    sub_430DD0(a1);
    return -1;
  }
}

// ===== sub_431120 @ 0x00431120..0x00431124 =====
int __usercall sub_431120@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 88) = a2;
  return result;
}

// ===== sub_431130 @ 0x00431130..0x00431134 =====
int __usercall sub_431130@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 88);
}

// ===== sub_431140 @ 0x00431140..0x00431144 =====
int __usercall sub_431140@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 80) = a2;
  return result;
}

// ===== sub_431150 @ 0x00431150..0x0043119B =====
int __usercall sub_431150@<eax>(int a1@<edi>)
{
  int i; // esi
  int result; // eax
  _DWORD *j; // esi

  for ( i = *(_DWORD *)(a1 + 12); i; i = *(_DWORD *)(i + 8) )
    result = (*(int (__thiscall **)(_DWORD, int, _DWORD, _DWORD))(**(_DWORD **)(i + 4) + 104))(
               *(_DWORD *)(i + 4),
               -268435456,
               0,
               0);
  for ( j = *(_DWORD **)(a1 + 20); j; j = (_DWORD *)j[1] )
    result = (*(int (__thiscall **)(_DWORD, int, _DWORD, _DWORD))(*(_DWORD *)*j + 104))(*j, -268435456, 0, 0);
  return result;
}

// ===== sub_4311A0 @ 0x004311A0..0x004311CE =====
BOOL __thiscall sub_4311A0(_DWORD *this)
{
  int v1; // eax

  v1 = this[20];
  return v1 != 6 && v1 != 7 && v1 != 9 && v1 != 10 && v1 != 11 && sub_41FE50();
}

// ===== sub_4311D0 @ 0x004311D0..0x004311F3 =====
BOOL __thiscall sub_4311D0(_DWORD *this)
{
  return !this[19] && (*(int (__thiscall **)(_DWORD *))(*this + 4))(this) && !sub_41FE20();
}

// ===== sub_431200 @ 0x00431200..0x004312B0 =====
int __userpurge sub_431200@<eax>(int a1@<eax>, _DWORD *a2@<esi>, int a3)
{
  unsigned int v3; // ebx
  int v4; // edx
  int v5; // eax
  _DWORD *v6; // ecx
  unsigned int v7; // edi
  int v9; // [esp+8h] [ebp-8h]
  int v10; // [esp+Ch] [ebp-4h]
  int v11; // [esp+18h] [ebp+8h]

  v3 = sub_431130(a1) / (unsigned int)(a2[2] - *a2 + 1);
  v10 = v3;
  if ( !v3 )
  {
    v3 = 1;
    v10 = 1;
  }
  v9 = (v3 + (__int64)(a2[3] - a2[1] + 1) - 1) / v3;
  if ( !a3 )
    return (v3 + (__int64)(a2[3] - a2[1] + 1) - 1) / v3;
  v4 = a2[1];
  v5 = a2[3] - v4 + 1;
  if ( !(unsigned int)((v3 + (__int64)v5 - 1) / v3) )
    return (v3 + (__int64)(a2[3] - a2[1] + 1) - 1) / v3;
  v6 = (_DWORD *)(a3 + 4);
  v11 = (v3 + (__int64)(a2[3] - a2[1] + 1) - 1) / v3;
  while ( 1 )
  {
    v7 = v3;
    if ( v5 < v3 )
      v7 = v5;
    *(v6 - 1) = *a2;
    v6[1] = a2[2];
    *v6 = v4;
    v6[2] = v7 + v4 - 1;
    v4 += v7;
    v5 -= v7;
    v6 += 4;
    if ( !--v11 )
      break;
    v3 = v10;
  }
  return v9;
}

// ===== sub_4312B0 @ 0x004312B0..0x004312D5 =====
int __userpurge sub_4312B0@<eax>(int a1@<esi>, int a2)
{
  _DWORD *v2; // eax

  v2 = operator new(8u);
  *v2 = a2;
  v2[1] = *(_DWORD *)(a1 + 20);
  *(_DWORD *)(a1 + 20) = v2;
  return sub_431320(a1);
}

// ===== sub_4312E0 @ 0x004312E0..0x00431319 =====
int __usercall sub_4312E0@<eax>(int a1@<edi>, int a2@<esi>)
{
  _DWORD *v2; // ecx
  int result; // eax
  _DWORD *v4; // edx

  v2 = *(_DWORD **)(a2 + 20);
  result = 0;
  v4 = (_DWORD *)(a2 + 16);
  if ( v2 )
  {
    while ( *v2 != a1 )
    {
      v4 = v2;
      v2 = (_DWORD *)v2[1];
      if ( !v2 )
        return result;
    }
    v4[1] = v2[1];
    operator delete(v2);
    sub_431320(a2);
    return 1;
  }
  return result;
}

// ===== sub_431320 @ 0x00431320..0x00431444 =====
void __stdcall sub_431320(int a1)
{
  int v1; // ebx
  int *v2; // edi
  int v3; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // edi
  void *v6; // eax
  int *v7; // edx
  _DWORD *v8; // eax
  _DWORD *v9; // ebx
  unsigned int v10; // eax
  unsigned int i; // ecx
  _DWORD *v12; // [esp+8h] [ebp-Ch]
  void *v13; // [esp+Ch] [ebp-8h]
  int v14; // [esp+10h] [ebp-4h]
  int *v15; // [esp+10h] [ebp-4h]

  v1 = a1;
  operator delete[](*(void **)(a1 + 36));
  *(_DWORD *)(a1 + 36) = 0;
  *(_DWORD *)(a1 + 24) = 0;
  *(_DWORD *)(a1 + 28) = 0;
  *(_DWORD *)(a1 + 32) = 0;
  v2 = *(int **)(a1 + 20);
  v14 = 0;
  if ( v2 )
  {
    do
    {
      v3 = sub_41AF60(0, *v2);
      v2 = (int *)v2[1];
      v14 += v3;
    }
    while ( v2 );
    if ( v14 )
    {
      v4 = operator new[](12 * v14);
      *(_DWORD *)(a1 + 36) = v4;
      v5 = v4;
      v6 = operator new[](4 * v14);
      v7 = *(int **)(a1 + 20);
      v13 = v6;
      v15 = v7;
      if ( v7 )
      {
        while ( 1 )
        {
          v8 = (_DWORD *)(v1 + 24);
          v9 = *(_DWORD **)(v1 + 32);
          v12 = v8;
          v10 = sub_41AF60(v13, *v7);
          for ( i = 0; i < v10; v5 += 3 )
          {
            for ( ; v9; v9 = (_DWORD *)v9[2] )
            {
              if ( *v9 > *((_DWORD *)v13 + i) )
                break;
              v12 = v9;
            }
            *v5 = *((_DWORD *)v13 + i);
            v5[1] = *v15;
            v5[2] = v9;
            v12[2] = v5;
            v12 = v5;
            ++i;
          }
          v15 = (int *)v15[1];
          if ( !v15 )
            break;
          v7 = v15;
          v1 = a1;
        }
      }
      operator delete[](v13);
    }
  }
}

// ===== sub_431450 @ 0x00431450..0x004314EC =====
unsigned int __userpurge sub_431450@<eax>(
        unsigned int a1@<eax>,
        int a2@<ecx>,
        unsigned int a3,
        _DWORD *a4,
        int *a5,
        unsigned int a6)
{
  unsigned int result; // eax
  unsigned int v9; // ebx
  int *i; // esi
  unsigned int v11; // [esp+Ch] [ebp-4h]

  result = a3;
  if ( *(_DWORD *)(a3 + 36) && a2 )
  {
    v9 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a2 + 4) + 28))(*(_DWORD *)(a2 + 4));
    if ( *(_DWORD *)(a2 + 8) )
      v11 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(*(_DWORD *)(a2 + 8) + 4) + 28))(*(_DWORD *)(*(_DWORD *)(a2 + 8) + 4));
    else
      v11 = -1;
    result = a6;
    if ( a6 )
    {
      result = a6 - 1;
      if ( a6 == 1 )
      {
        if ( v11 < a1 )
          return result;
        if ( a1 > v9 )
          v9 = a1;
      }
    }
    else
    {
      if ( a1 < v9 )
        return result;
      if ( v11 > a1 )
        v11 = a1;
    }
    for ( i = *(int **)(a3 + 32); i; i = (int *)i[2] )
    {
      result = *i;
      if ( v9 <= *i )
      {
        if ( result > v11 )
          return result;
        result = sub_41AFD0(a4, i[1], a5, *i);
      }
    }
  }
  return result;
}

// ===== sub_4314F0 @ 0x004314F0..0x00431547 =====
int __userpurge sub_4314F0@<eax>(int a1@<esi>, int a2)
{
  int v2; // edi

  sub_4464C0(a1);
  v2 = 0;
  if ( !a2 && !sub_407BA0() )
  {
    sub_407B90(*(_DWORD *)(a1 + 104));
    v2 = 1;
  }
  sub_4464E0(*(_DWORD *)(a1 + 104));
  if ( v2 )
    sub_407B90(0);
  return sub_4464C0(0);
}

// ===== sub_431550 @ 0x00431550..0x00431619 =====
int __usercall sub_431550@<eax>(unsigned int a1@<esi>)
{
  int v1; // edi
  int v2; // eax
  unsigned int v3; // edx
  int *v4; // ecx
  int v5; // edi
  int v6; // ecx
  int i; // ebx
  int v9; // [esp+8h] [ebp-18h]
  int v10[4]; // [esp+Ch] [ebp-14h] BYREF
  int v11; // [esp+1Ch] [ebp-4h]

  v1 = 0;
  v9 = 0;
  v2 = sub_4465D0(*(_DWORD *)(a1 + 104));
  v3 = *(_DWORD *)(a1 + 100);
  if ( v3 < *(_DWORD *)(a1 + 92) )
  {
    v4 = (int *)(*(_DWORD *)(a1 + 96) + 20 * v3);
    v10[0] = *v4;
    v10[1] = v4[1];
    v10[2] = v4[2];
    v5 = v4[3];
    v6 = v4[4];
    v9 = 1;
    v10[3] = v5;
    v1 = 1;
    v11 = v6;
    *(_DWORD *)(a1 + 100) = v3 + 1;
  }
  sub_4465F0(v2);
  if ( v1 )
  {
    for ( i = *(_DWORD *)(a1 + 12); i; i = *(_DWORD *)(i + 8) )
    {
      if ( *(_DWORD *)(a1 + 72) <= (unsigned int)(*(int (__thiscall **)(_DWORD))(**(_DWORD **)(i + 4) + 28))(*(_DWORD *)(i + 4))
        || !sub_41B0B0(*(_DWORD *)(i + 4)) )
      {
        sub_41AFD0(*(_DWORD **)(a1 + 84), *(_DWORD *)(i + 4), v10, v11);
        v1 = v9;
      }
      sub_431450(*(_DWORD *)(a1 + 72), i, a1, *(_DWORD **)(a1 + 84), v10, 1u);
    }
  }
  return v1;
}

// ===== sub_431620 @ 0x00431620..0x0043162F =====
int __cdecl sub_431620(unsigned int a1)
{
  return sub_431550(a1);
}

// ===== sub_431630 @ 0x00431630..0x0043163F =====
int __thiscall sub_431630(_BYTE *this)
{
  int result; // eax

  result = 0;
  if ( *this == 32 )
  {
    do
      ++result;
    while ( this[result] == 32 );
  }
  return result;
}

// ===== sub_431640 @ 0x00431640..0x004316A8 =====
int __usercall sub_431640@<eax>(char *a1@<eax>, _BYTE *a2@<edx>)
{
  char v3; // cl
  int v4; // edi
  int result; // eax
  int v6; // esi
  char v7; // cl
  char *v8; // esi

  v3 = *a1;
  v4 = 0;
  result = 0;
  if ( v3 == 34 )
  {
    v7 = a1[1];
    v8 = a1 + 1;
    if ( v7 != 10 )
    {
      while ( v7 )
      {
        if ( v7 == 34 )
        {
          if ( v4 > 0 )
          {
            result = v4 + 2;
            break;
          }
        }
        else
        {
          *a2++ = v7;
          ++v8;
          ++v4;
        }
        v7 = *v8;
        if ( *v8 == 10 )
        {
          *a2 = 0;
          return result;
        }
      }
    }
    *a2 = 0;
  }
  else
  {
    if ( v3 != 32 )
    {
      v6 = a1 - a2;
      do
      {
        if ( v3 == 10 )
          break;
        if ( !v3 )
          break;
        *a2 = v3;
        v3 = (a2++)[v6 + 1];
        ++v4;
      }
      while ( v3 != 32 );
    }
    result = v4;
    *a2 = 0;
  }
  return result;
}

// ===== sub_4316B0 @ 0x004316B0..0x004316EB =====
int __usercall sub_4316B0@<eax>(char *a1@<eax>, _DWORD *a2)
{
  int v2; // edi
  _BYTE **v4; // edx
  char *v5; // esi
  int v6; // eax

  v2 = 0;
  if ( *a2 )
  {
    do
    {
      v5 = &a1[sub_431630(a1)];
      v6 = sub_431640(v5, *v4);
      if ( v6 <= 0 )
        break;
      ++v2;
      a1 = &v5[v6];
    }
    while ( a2[v2] );
  }
  return v2;
}

// ===== sub_4316F0 @ 0x004316F0..0x00431819 =====
int __cdecl sub_4316F0(char *a1)
{
  int v1; // esi
  int result; // eax
  _DWORD v3[3]; // [esp+4h] [ebp-628h] BYREF
  char v4; // [esp+10h] [ebp-61Ch] BYREF
  char v5[780]; // [esp+31Ch] [ebp-310h] BYREF

  v3[0] = &v4;
  v3[1] = v5;
  v3[2] = 0;
  v1 = sub_4316B0(a1, v3);
  if ( !v1 )
    return sub_4650F0("ipl._bp");
  result = sub_4650F0("ipl._bp");
  if ( v1 == 2 )
  {
    if ( dword_5666DC || (dword_5666DC = 0, !strcmp(v5, "Execute as a launcher.")) )
      dword_5666DC = 1;
    result = strcmp(v5, "Do not use mutex.");
    dword_5666E0 = result == 0;
  }
  return result;
}

// ===== sub_431820 @ 0x00431820..0x004318EF =====
BOOL __cdecl sub_431820(HWND hWnd)
{
  BOOL result; // eax
  BOOL v2; // edi
  char v3[780]; // [esp+8h] [ebp-61Ch] BYREF
  char Buffer[780]; // [esp+314h] [ebp-310h] BYREF

  result = IsWindow(hWnd);
  v2 = result;
  if ( result )
  {
    sub_4650C0(Buffer);
    if ( strcmp(v3, "system.arc") )
    {
      sub_4649F0(Buffer);
      strlen(Buffer);
      sub_48D260(hWnd);
    }
    return v2;
  }
  return result;
}

// ===== sub_4318F0 @ 0x004318F0..0x0043191B =====
_DWORD *__usercall sub_4318F0@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  int v2; // ecx

  result[1] = a2;
  result[2] = 0;
  result[3] = 0;
  result[4] = 0;
  result[5] = 0;
  result[6] = 0;
  v2 = dword_565B74;
  result[7] = dword_565B74;
  *result = &CProcedure::`vftable';
  dword_565B74 = v2 + 1;
  return result;
}

// ===== sub_431920 @ 0x00431920..0x00431941 =====
void *__thiscall sub_431920(void *this, char a2)
{
  sub_431950();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_431950 @ 0x00431950..0x00431981 =====
int __thiscall sub_431950(_DWORD *this)
{
  _DWORD *v1; // esi
  void *v3; // [esp-4h] [ebp-Ch]

  v1 = (_DWORD *)this[5];
  *this = &CProcedure::`vftable';
  while ( v1 )
  {
    v3 = v1;
    v1 = (_DWORD *)v1[3];
    operator delete(v3);
  }
  sub_4467B0();
  return sub_46E550();
}

// ===== sub_431990 @ 0x00431990..0x0043199B =====
void *sub_431990()
{
  void *result; // eax

  result = dword_56674C;
  dword_565B6C = (int)dword_56674C;
  return result;
}

// ===== sub_4319A0 @ 0x004319A0..0x004319AB =====
void *sub_4319A0()
{
  void *result; // eax

  result = dword_566750;
  dword_565B70 = (int)dword_566750;
  return result;
}

// ===== sub_4319B0 @ 0x004319B0..0x004319B6 =====
int __usercall sub_4319B0@<eax>(int result@<eax>)
{
  dword_507688 = result;
  return result;
}

// ===== sub_4319C0 @ 0x004319C0..0x004319D8 =====
BOOL __usercall sub_4319C0@<eax>(int a1@<eax>)
{
  return dword_507688 && !*(_DWORD *)(a1 + 16);
}

// ===== sub_4319E0 @ 0x004319E0..0x004319EC =====
int __usercall sub_4319E0@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_50768C = result;
  dword_565B78 = a2;
  return result;
}

// ===== sub_4319F0 @ 0x004319F0..0x004319F6 =====
int __usercall sub_4319F0@<eax>(int result@<eax>)
{
  dword_507690 = result;
  return result;
}

// ===== sub_431A00 @ 0x00431A00..0x00431A08 =====
int __usercall sub_431A00@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 16) = 1;
  return result;
}

// ===== sub_431A10 @ 0x00431A10..0x00431A28 =====
int __thiscall sub_431A10(_DWORD *this, int a2)
{
  int result; // eax

  result = a2 + (*(int (__thiscall **)(_DWORD *))(*this + 12))(this);
  this[2] = result;
  return result;
}

// ===== sub_431A30 @ 0x00431A30..0x00431A34 =====
int __usercall sub_431A30@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 8) += a2;
  return result;
}

// ===== sub_431A40 @ 0x00431A40..0x00431A48 =====
__int64 sub_431A40()
{
  return (unsigned int)sub_498720();
}

// ===== sub_431A50 @ 0x00431A50..0x00431A5D =====
int __usercall sub_431A50@<eax>(_DWORD *a1@<esi>)
{
  return (*(int (__thiscall **)(_DWORD *))(*a1 + 12))(a1) - a1[2];
}

// ===== sub_431A60 @ 0x00431A60..0x00431A86 =====
BOOL __usercall sub_431A60@<eax>(unsigned int *a1@<eax>)
{
  __int64 v2; // rax

  LODWORD(v2) = (*(int (__thiscall **)(unsigned int *))(*a1 + 12))(a1);
  return a1[2] <= v2;
}

// ===== sub_431A90 @ 0x00431A90..0x00431A98 =====
void __thiscall sub_431A90(_DWORD *this)
{
  this[3] = 1;
}

// ===== sub_431AA0 @ 0x00431AA0..0x00431AD9 =====
int __usercall sub_431AA0@<eax>(_DWORD *a1@<esi>)
{
  int result; // eax

  result = (*(int (__thiscall **)(_DWORD *))(*a1 + 20))(a1);
  if ( result && dword_50768C )
  {
    if ( dword_565B78 )
      result = sub_461D70();
    else
      result = sub_461D80();
    a1[3] = 0;
  }
  return result;
}

// ===== sub_431AE0 @ 0x00431AE0..0x00431AE4 =====
int __thiscall sub_431AE0(_DWORD *this)
{
  return this[3];
}

// ===== sub_431AF0 @ 0x00431AF0..0x00431B37 =====
int __usercall sub_431AF0@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int result; // eax
  int v3; // ecx
  _DWORD v4[4]; // [esp+4h] [ebp-10h] BYREF

  result = sub_431B90(a1, v4);
  if ( result )
  {
    while ( v4[0] )
    {
      (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)a2 + 24))(a2, v4);
      result = sub_431B90(v3, v4);
      if ( !result )
        return result;
    }
    return sub_431A00(a2);
  }
  return result;
}

// ===== sub_431B40 @ 0x00431B40..0x00431B82 =====
_DWORD *__userpurge sub_431B40@<eax>(int a1@<esi>, int a2, int a3, int a4)
{
  _DWORD *result; // eax

  result = operator new(0x10u);
  *result = a2;
  result[1] = a3;
  result[2] = a4;
  result[3] = 0;
  if ( *(_DWORD *)(a1 + 20) )
    *(_DWORD *)(*(_DWORD *)(a1 + 24) + 12) = result;
  else
    *(_DWORD *)(a1 + 20) = result;
  *(_DWORD *)(a1 + 24) = result;
  return result;
}

// ===== sub_431B90 @ 0x00431B90..0x00431BC7 =====
int __usercall sub_431B90@<eax>(int a1@<eax>, _DWORD *a2@<edx>)
{
  _DWORD *v3; // ecx
  int result; // eax

  v3 = *(_DWORD **)(a1 + 20);
  result = 0;
  if ( v3 )
  {
    *(_DWORD *)(a1 + 20) = v3[3];
    *a2 = *v3;
    a2[1] = v3[1];
    a2[2] = v3[2];
    a2[3] = 0;
    operator delete(v3);
    return 1;
  }
  return result;
}

// ===== sub_431BD0 @ 0x00431BD0..0x00431C59 =====
_DWORD *__fastcall sub_431BD0(int a1, int a2, _DWORD *a3)
{
  sub_4318F0(a3, a1);
  *a3 = &CProcCtrlDspObj::`vftable';
  a3[8] = a2;
  a3[9] = sub_443270();
  a3[32] = 0;
  a3[31] = 0;
  a3[34] = 0;
  a3[35] = 0;
  a3[38] = 0;
  return a3;
}

// ===== sub_431C60 @ 0x00431C60..0x00431C81 =====
void *__thiscall sub_431C60(void *this, char a2)
{
  sub_431C90();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_431C90 @ 0x00431C90..0x00431D05 =====
int __thiscall sub_431C90(_DWORD *this)
{
  *this = &CProcCtrlDspObj::`vftable';
  if ( this[31] )
  {
    sub_46D7A0();
    sub_46D7B0();
  }
  return sub_431950(this);
}

// ===== sub_431D10 @ 0x00431D10..0x00431D48 =====
int __userpurge sub_431D10@<eax>(int a1@<esi>, int a2, int a3, int a4, int a5)
{
  _DWORD v6[2]; // [esp+0h] [ebp-8h] BYREF

  (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(a1 + 36) + 48))(*(_DWORD *)(a1 + 36), v6);
  return sub_431D50(a1, v6[0], v6[1], 0, a2, a3);
}

// ===== sub_431D50 @ 0x00431D50..0x00431D81 =====
int __userpurge sub_431D50@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8)
{
  int v8; // eax

  v8 = sub_41B740();
  return sub_431D90(a4, a5, a6, a7, 0, v8, a2, a1);
}

// ===== sub_431D90 @ 0x00431D90..0x00431E79 =====
int __userpurge sub_431D90@<eax>(
        int a1@<eax>,
        _DWORD *a2@<esi>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  int v10; // ecx
  int v11; // ecx
  int v12; // eax
  int v13; // eax
  int v14; // ecx
  int v15; // edx
  int v16; // eax

  a2[11] = 0;
  if ( !a1 )
    a1 = 1;
  v10 = a2[9];
  a2[12] = a1;
  a2[13] = 1;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v10 + 48))(v10, a2 + 17);
  v11 = a2[9];
  a2[19] = a3 - a2[17];
  a2[20] = a4 - a2[18];
  a2[21] = a5;
  v12 = (*(int (__thiscall **)(int))(*(_DWORD *)v11 + 76))(v11);
  a2[24] = a7;
  a2[22] = v12;
  a2[23] = a6 - v12;
  v13 = sub_41B740();
  a2[25] = v13;
  if ( a8 < 0 )
    v15 = 0;
  else
    v15 = a8 - v13;
  a2[27] = 0x80000000;
  a2[28] = 0x80000000;
  a2[29] = -1;
  a2[30] = -1;
  a2[26] = v15;
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v14 + 4))(v14, 1);
  (*(void (__thiscall **)(_DWORD))(*(_DWORD *)a2[9] + 12))(a2[9]);
  a2[14] = a9;
  a2[15] = a10;
  a2[16] = 0;
  a2[39] = sub_498720();
  v16 = 1000 * a2[15] / a2[14];
  a2[40] = v16;
  a2[41] = v16;
  return (*(int (__thiscall **)(_DWORD *, int))(*a2 + 8))(a2, 1);
}

// ===== sub_431E80 @ 0x00431E80..0x00431F00 =====
int __userpurge sub_431E80@<eax>(int result@<eax>, _DWORD *a2@<edi>, int a3)
{
  a2[32] = result;
  if ( result )
  {
    if ( a2[31] )
    {
      sub_46D7A0();
      sub_46D7B0();
    }
    a2[33] = a3;
    sub_46D6C0();
    sub_46D720((a3 << 16) | 0xFFFF);
    a2[36] = sub_46E070(1);
    result = sub_46E070(dword_507690 | 0x180);
    a2[37] = result;
    a2[31] = 1;
  }
  return result;
}

// ===== sub_431F00 @ 0x00431F00..0x004320AE =====
int __thiscall sub_431F00(unsigned int *this)
{
  int v2; // eax
  int v3; // ecx
  bool v4; // zf
  int v5; // esi
  unsigned int v6; // edi
  unsigned int v7; // edx
  unsigned int v8; // edi
  BOOL v9; // eax
  BOOL v10; // edi
  unsigned int v12; // [esp+Ch] [ebp-Ch]
  unsigned int v13; // [esp+10h] [ebp-8h]
  int v14; // [esp+10h] [ebp-8h]
  unsigned int v15; // [esp+14h] [ebp-4h]

  v2 = sub_443270();
  this[9] = v2;
  if ( !v2 )
    sub_4646F0(this[1]);
  sub_431AF0(v3, (int)this);
  v4 = this[32] == 0;
  this[10] = 0;
  if ( !v4 )
  {
    v5 = (this[33] << 16) | 0xFFFF;
    v6 = sub_46E070(1);
    v15 = v6;
    v13 = sub_46D830(v5);
    if ( v13 && this[34] && v6 > this[36] )
      this[10] = 1;
    v12 = sub_46E070(dword_507690 | 0x180);
    v8 = sub_46D810(v5);
    if ( v8 && this[35] && v7 > this[37] )
      this[10] = 256;
    if ( sub_46DE30() )
      this[10] = 0x80000000;
    if ( this[10] )
      sub_46DF00(v5);
    this[34] = v13;
    this[35] = v8;
    this[36] = v15;
    this[37] = v12;
  }
  v14 = 0;
  v9 = sub_4319C0((int)this);
  v10 = !v9;
  if ( !v9 )
    goto LABEL_24;
  if ( sub_431A60(this) || this[10] || this[38] )
  {
    if ( (*(int (__thiscall **)(unsigned int *))(*this + 28))(this) )
      v10 = 1;
    sub_431AA0(this);
    ++this[16];
    if ( v10 )
    {
LABEL_24:
      sub_4450D0();
      sub_4450D0();
      return 1;
    }
  }
  return v14;
}

// ===== sub_4320B0 @ 0x004320B0..0x004320D6 =====
int __thiscall sub_4320B0(_DWORD *this, _DWORD *a2)
{
  int result; // eax

  result = *a2 - 1;
  if ( *a2 == 1 && (a2[1] != result || this[32] != result) )
    this[38] = 1;
  return result;
}

// ===== sub_4320E0 @ 0x004320E0..0x0043215F =====
BOOL __usercall sub_4320E0@<eax>(unsigned int *a1@<esi>)
{
  int v1; // edi
  unsigned int *v2; // eax

  v1 = 0;
  if ( sub_431A60(a1) )
  {
    while ( (int)(v1 + a1[11]) < (int)a1[12] )
    {
      v2 = (unsigned int *)sub_431A30((int)a1, a1[13]);
      ++v1;
      if ( sub_431A60(v2) && v1 == a1[15] )
      {
        (*(void (__thiscall **)(unsigned int *, unsigned int))(*a1 + 8))(a1, a1[13]);
        break;
      }
      if ( !sub_431A60(a1) )
        break;
    }
  }
  a1[11] += v1;
  if ( sub_431A60(a1) && a1[11] == a1[12] )
    (*(void (__thiscall **)(unsigned int *, unsigned int))(*a1 + 8))(a1, a1[13]);
  return a1[11] == a1[12];
}

// ===== sub_432160 @ 0x00432160..0x0043235E =====
BOOL __thiscall sub_432160(_DWORD *this)
{
  unsigned int v2; // eax
  unsigned int v3; // ecx
  int v4; // ebx
  int v5; // eax
  int v6; // eax
  int v7; // ecx
  int v8; // ebx
  int v9; // edi
  int v10; // edi
  int v11; // ecx
  int v13; // [esp+Ch] [ebp-1Ch]
  int v14; // [esp+10h] [ebp-18h]
  __int64 v15; // [esp+10h] [ebp-18h]
  int v16; // [esp+18h] [ebp-10h]
  int v17; // [esp+18h] [ebp-10h]
  BOOL v18; // [esp+1Ch] [ebp-Ch]
  int v19; // [esp+24h] [ebp-4h]

  if ( this[10] || this[38] )
  {
    this[11] = this[12];
    v18 = 1;
  }
  else
  {
    v2 = sub_498720() - this[39];
    if ( this[15] )
    {
      v3 = this[41];
      if ( v2 >= v3 )
        this[11] = v3;
      else
        this[11] = v2;
      this[41] = this[11] + this[40];
    }
    else
    {
      this[11] = v2;
    }
    v4 = this[11];
    v5 = this[12];
    if ( v4 >= v5 )
    {
      v13 = this[12];
      v4 = v13;
    }
    else
    {
      v13 = this[11];
    }
    this[11] = v4;
    v18 = v4 == v5;
    if ( v4 != v5 )
    {
      v10 = v5;
      v19 = v5 >> 31;
      v17 = ((__int64)v4 << 24) / v5;
      v15 = sub_41A690(v17);
      v8 = this[17] + ((unsigned __int64)((int)this[19] * v15) >> 16);
      v14 = this[18] + ((unsigned __int64)((int)this[20] * v15) >> 16);
      v16 = this[22] + ((unsigned __int64)((int)this[23] * (__int64)sub_41A690(v17)) >> 16);
      v7 = v16;
      v9 = ((__int64)(v13 * this[26]) << 16) / __SPAIR64__(v19, v10) + (this[25] << 16);
      v6 = v14;
      goto LABEL_16;
    }
  }
  v6 = this[18] + this[20];
  v7 = this[22] + this[23];
  v8 = this[17] + this[19];
  v14 = v6;
  v16 = v7;
  v9 = (this[25] + this[26]) << 16;
LABEL_16:
  if ( v8 != this[27] || v6 != this[28] || v7 != this[29] || v9 != this[30] )
  {
    this[29] = v7;
    v11 = this[9];
    this[27] = v8;
    this[28] = v6;
    this[30] = v9;
    (*(void (__thiscall **)(int))(*(_DWORD *)v11 + 12))(v11);
    (*(void (__thiscall **)(_DWORD, int, int))(*(_DWORD *)this[9] + 44))(this[9], v8, v14);
    (*(void (__thiscall **)(_DWORD, int))(*(_DWORD *)this[9] + 72))(this[9], v16);
    (*(void (__thiscall **)(_DWORD, int, int))(*(_DWORD *)this[9] + 80))(this[9], 1, v9);
    (*(void (__thiscall **)(_DWORD))(*(_DWORD *)this[9] + 12))(this[9]);
    (*(void (__thiscall **)(_DWORD *))(*this + 16))(this);
  }
  (*(void (__thiscall **)(_DWORD *, int))(*this + 8))(this, 1);
  return v18;
}

// ===== sub_432360 @ 0x00432360..0x004323E1 =====
_DWORD *__fastcall sub_432360(int a1, int a2, _DWORD *a3)
{
  void *v3; // eax
  int v4; // eax

  sub_431BD0(a1, a2, a3);
  *a3 = &CProcCtrlDspObjBC::`vftable';
  v3 = operator new(0x2640u);
  if ( v3 )
    v4 = sub_444070(v3);
  else
    v4 = 0;
  a3[42] = v4;
  return a3;
}

// ===== sub_4323F0 @ 0x004323F0..0x00432412 =====
void *__thiscall sub_4323F0(void *this, char a2)
{
  sub_432420(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_432420 @ 0x00432420..0x00432485 =====
int __stdcall sub_432420(_DWORD *a1)
{
  void (__thiscall ***v1)(_DWORD, int); // ecx

  *a1 = &CProcCtrlDspObjBC::`vftable';
  v1 = (void (__thiscall ***)(_DWORD, int))a1[42];
  if ( v1 )
    (**v1)(v1, 1);
  return sub_431C90(a1);
}

// ===== sub_432490 @ 0x00432490..0x004325E0 =====
int __fastcall sub_432490(
        int a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        int a9,
        int a10,
        int a11)
{
  _DWORD *v11; // esi
  int v12; // eax
  int v14; // [esp+4h] [ebp-1Ch]
  int v15; // [esp+8h] [ebp-18h]
  int v16; // [esp+Ch] [ebp-14h] BYREF
  int v17; // [esp+10h] [ebp-10h]
  int v18; // [esp+14h] [ebp-Ch]

  if ( a4 <= 0 )
    return -2147483647;
  v14 = a5 + 16 * a4;
  sub_431D90(a10, (_DWORD *)a3, *(_DWORD *)(v14 - 16), *(_DWORD *)(v14 - 12), a6, a7, (unsigned __int16)a8, a9, a11, a2);
  (*(void (__thiscall **)(_DWORD, int *))(**(_DWORD **)(a3 + 36) + 64))(*(_DWORD *)(a3 + 36), &v16);
  sub_4441D0();
  sub_444270(v16, v17, v18);
  v11 = (_DWORD *)(a5 + 4);
  v15 = a4;
  do
  {
    sub_444270(*(v11 - 1), *v11, v11[1]);
    v11 += 4;
    --v15;
  }
  while ( v15 );
  sub_4442C0(0x10000);
  *(_DWORD *)(a3 + 172) = 0x80000000;
  *(_DWORD *)(a3 + 176) = 0x80000000;
  *(_DWORD *)(a3 + 180) = 0x80000000;
  *(_DWORD *)(a3 + 188) = *(_DWORD *)(v14 - 16);
  *(_DWORD *)(a3 + 192) = *(_DWORD *)(v14 - 12);
  *(_DWORD *)(a3 + 196) = *(_DWORD *)(v14 - 8);
  v12 = HIWORD(a8);
  *(_DWORD *)(a3 + 200) = *(_DWORD *)(v14 - 4);
  if ( !HIWORD(a8) )
    v12 = 0x10000;
  *(_QWORD *)(a3 + 208) = 0x100000000LL / v12;
  return 0;
}

// ===== sub_4325E0 @ 0x004325E0..0x0043286C =====
BOOL __thiscall sub_4325E0(int *this)
{
  unsigned int v2; // eax
  unsigned int v3; // ecx
  int v4; // eax
  int v5; // ecx
  int v6; // eax
  int v7; // ecx
  int v8; // edx
  int v9; // ebx
  int v10; // esi
  int v11; // esi
  __int64 v12; // kr00_8
  __int64 v13; // rax
  __int64 v14; // rax
  int v15; // ecx
  int v17; // [esp+10h] [ebp-28h]
  int v18; // [esp+14h] [ebp-24h]
  int v19; // [esp+18h] [ebp-20h]
  BOOL v20; // [esp+1Ch] [ebp-1Ch]
  __int64 v21; // [esp+20h] [ebp-18h]
  int v22; // [esp+20h] [ebp-18h]

  if ( this[10] || this[38] )
  {
    this[11] = this[12];
    v20 = 1;
LABEL_12:
    v6 = this[47];
    v7 = this[48];
    v8 = this[49];
    v9 = this[22] + this[23];
    v19 = v6;
    v18 = v7;
    v17 = v8;
    v10 = (this[25] + this[26]) << 16;
    goto LABEL_17;
  }
  v2 = sub_498720() - this[39];
  if ( this[15] )
  {
    v3 = this[41];
    if ( v2 >= v3 )
      this[11] = v3;
    else
      this[11] = v2;
    this[41] = this[11] + this[40];
  }
  else
  {
    this[11] = v2;
  }
  v4 = this[11];
  v5 = this[12];
  if ( v4 >= v5 )
    v4 = this[12];
  this[11] = v4;
  v20 = v4 == v5;
  if ( v4 == v5 )
    goto LABEL_12;
  sub_41A690(((__int64)v4 << 24) / v5);
  sub_4442E0();
  v11 = this[11];
  v18 = 0;
  v19 = 0;
  v12 = this[12];
  HIDWORD(v21) = HIDWORD(v12);
  v17 = 0;
  LODWORD(v21) = this[12];
  v13 = ((*((_QWORD *)this + 26) * v11) << 8) / v12;
  if ( v13 > 0x1000000 )
    LODWORD(v13) = 0x1000000;
  v9 = this[22] + ((unsigned __int64)(this[23] * (__int64)sub_41A690(v13)) >> 16);
  v14 = ((__int64)(v11 * this[26]) << 16) / v21;
  v8 = 0;
  v7 = 0;
  v10 = v14 + (this[25] << 16);
  v6 = 0;
LABEL_17:
  if ( v6 != this[43] || v7 != this[44] || v8 != this[45] || v9 != this[29] || v10 != this[30] )
  {
    this[44] = v7;
    v15 = this[9];
    this[45] = v8;
    this[43] = v6;
    this[29] = v9;
    this[30] = v10;
    (*(void (__thiscall **)(int))(*(_DWORD *)v15 + 12))(v15);
    v22 = (*(int (__thiscall **)(int))(*(_DWORD *)this[9] + 28))(this[9]);
    (*(void (__thiscall **)(int, int, int, int))(*(_DWORD *)this[9] + 60))(this[9], v19, v18, v17);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)this[9] + 72))(this[9], v9);
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)this[9] + 80))(this[9], 1, v10);
    if ( v22 != (*(int (__thiscall **)(int))(*(_DWORD *)this[9] + 28))(this[9]) )
      sub_443300();
    (*(void (__thiscall **)(int))(*(_DWORD *)this[9] + 12))(this[9]);
    (*(void (__thiscall **)(int *))(*this + 16))(this);
  }
  (*(void (__thiscall **)(int *, int))(*this + 8))(this, 1);
  return v20;
}

// ===== sub_432870 @ 0x00432870..0x004328C8 =====
_DWORD *__fastcall sub_432870(int a1, int a2, _DWORD *a3)
{
  sub_431BD0(a1, a2, a3);
  *a3 = &CProcCtrlDspObjSp::`vftable';
  a3[42] = 0;
  return a3;
}

// ===== sub_4328D0 @ 0x004328D0..0x004328F2 =====
void *__thiscall sub_4328D0(void *this, char a2)
{
  sub_432900(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_432900 @ 0x00432900..0x00432962 =====
int __stdcall sub_432900(int a1)
{
  *(_DWORD *)a1 = &CProcCtrlDspObjSp::`vftable';
  operator delete[](*(void **)(a1 + 168));
  return sub_431C90((_DWORD *)a1);
}

// ===== sub_432970 @ 0x00432970..0x00432AC0 =====
int __userpurge sub_432970@<eax>(
        unsigned int a1@<ecx>,
        unsigned int a2@<edi>,
        int a3@<esi>,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  signed int v10; // eax
  unsigned int v11; // eax
  int v12; // ecx
  void *v13; // eax
  int v15; // ecx
  int v16; // eax
  int v17; // ecx
  int v18; // [esp-18h] [ebp-28h]
  int v19; // [esp-14h] [ebp-24h]
  int v20; // [esp-4h] [ebp-14h]
  _DWORD v21[2]; // [esp+8h] [ebp-8h] BYREF

  if ( !a2 )
    return -2147483647;
  *(_DWORD *)(a3 + 44) = 0;
  v10 = a2 * a1 / 0x3E8 + (a2 * a1 / 0x3E8 == 0);
  *(_DWORD *)(a3 + 48) = v10;
  if ( v10 <= 1 )
    v11 = a1;
  else
    v11 = 0x3E8 / a2;
  v12 = *(_DWORD *)(a3 + 36);
  *(_DWORD *)(a3 + 52) = v11;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v12 + 48))(v12, v21);
  operator delete[](*(void **)(a3 + 168));
  v13 = operator new[](8 * (*(_DWORD *)(a3 + 48) + 1));
  v20 = *(_DWORD *)(a3 + 48) + 1;
  v19 = v21[1];
  v18 = v21[0];
  *(_DWORD *)(a3 + 168) = v13;
  if ( !sub_494730(v13, v18, v19, a5, a6, a7, v20) )
    return -2147483646;
  v15 = *(_DWORD *)(a3 + 36);
  *(_DWORD *)(a3 + 84) = a8;
  v16 = (*(int (__thiscall **)(int))(*(_DWORD *)v15 + 76))(v15);
  *(_DWORD *)(a3 + 88) = v16;
  *(_DWORD *)(a3 + 92) = a9 - v16;
  v17 = *(_DWORD *)(a3 + 36);
  *(_DWORD *)(a3 + 108) = 0x80000000;
  *(_DWORD *)(a3 + 112) = 0x80000000;
  *(_DWORD *)(a3 + 116) = -1;
  (*(void (__thiscall **)(int, int))(*(_DWORD *)v17 + 4))(v17, 1);
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a3 + 36) + 12))(*(_DWORD *)(a3 + 36));
  (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)a3 + 8))(a3, *(_DWORD *)(a3 + 52));
  *(_DWORD *)(a3 + 60) = a10;
  *(_DWORD *)(a3 + 56) = a2;
  *(_DWORD *)(a3 + 64) = 0;
  *(_DWORD *)(a3 + 172) = a6;
  *(_DWORD *)(a3 + 176) = a7;
  *(_DWORD *)(a3 + 180) = a9;
  return 0;
}

// ===== sub_432AC0 @ 0x00432AC0..0x00432BBF =====
BOOL __thiscall sub_432AC0(unsigned int *this)
{
  unsigned int v2; // eax
  unsigned int v3; // ebx
  int v4; // edi
  signed int v5; // edi
  int v6; // eax
  unsigned int v7; // ecx
  int v8; // eax
  unsigned int v9; // ecx
  BOOL v11; // [esp+Ch] [ebp-8h]
  unsigned int v12; // [esp+10h] [ebp-4h]

  if ( this[10] || this[38] )
  {
    this[11] = this[12];
    v11 = 1;
  }
  else
  {
    v11 = sub_4320E0(this);
    if ( !v11 )
    {
      v5 = this[12];
      v6 = sub_41A690(((__int64)(int)this[11] << 24) / v5);
      v7 = this[42];
      v8 = (unsigned __int64)((v5 + 1) * (__int64)v6) >> 16;
      v3 = *(_DWORD *)(v7 + 8 * v8);
      v12 = *(_DWORD *)(v7 + 8 * v8 + 4);
      v4 = this[22] + (int)(this[11] * this[23]) / v5;
      v2 = v12;
      goto LABEL_7;
    }
  }
  v2 = this[44];
  v3 = this[43];
  v4 = this[45];
  v12 = v2;
LABEL_7:
  if ( v3 != this[27] || v2 != this[28] || v4 != this[29] )
  {
    v9 = this[9];
    this[27] = v3;
    this[28] = v2;
    this[29] = v4;
    (*(void (__thiscall **)(unsigned int))(*(_DWORD *)v9 + 12))(v9);
    (*(void (__thiscall **)(unsigned int, unsigned int, unsigned int))(*(_DWORD *)this[9] + 44))(this[9], v3, v12);
    (*(void (__thiscall **)(unsigned int, int))(*(_DWORD *)this[9] + 72))(this[9], v4);
    (*(void (__thiscall **)(unsigned int))(*(_DWORD *)this[9] + 12))(this[9]);
    (*(void (__thiscall **)(unsigned int *))(*this + 16))(this);
  }
  return v11;
}

// ===== sub_432BC0 @ 0x00432BC0..0x00432CCC =====
_DWORD *__fastcall sub_432BC0(int a1, int a2, _DWORD *a3)
{
  int v4; // edx
  bool v5; // zf
  int v6; // eax
  int v7; // eax

  sub_4318F0(a3, a1);
  *a3 = &CProcDspMsg::`vftable';
  a3[8] = a2;
  sub_4337D0(0);
  v6 = dword_565BA4 - v4;
  v5 = dword_565BA4 == v4;
  a3[24] = v4;
  a3[25] = v4;
  a3[26] = v4;
  a3[27] = v4;
  a3[29] = v4;
  a3[23] = v4;
  if ( v5 )
  {
    a3[30] = 2;
  }
  else
  {
    v7 = v6 - 1;
    if ( v7 )
    {
      if ( v7 == 1 )
        a3[30] = ((*(int (__thiscall **)(int))(*(_DWORD *)a2 + 88))(a2) << 16) | 0xFFFF;
    }
    else
    {
      a3[30] = (dword_565BA8 << 16) | 0xFFFF;
    }
  }
  sub_46D6C0();
  sub_46D720(a3[30]);
  sub_46DF00(a3[30]);
  sub_42B550(a3[8], 0);
  sub_42B540(a3[8], 1);
  sub_42CA00(a3[8]);
  sub_42BBB0(a3[8]);
  a3[17] = dword_565B90;
  a3[18] = dword_565B94 + sub_498720();
  return a3;
}

// ===== sub_432CD0 @ 0x00432CD0..0x00432CF1 =====
void *__thiscall sub_432CD0(void *this, char a2)
{
  sub_432D00();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_432D00 @ 0x00432D00..0x00432D66 =====
int __thiscall sub_432D00(_DWORD *this)
{
  *this = &CProcDspMsg::`vftable';
  sub_46D7A0();
  sub_46D7B0();
  return sub_431950(this);
}

// ===== sub_432D70 @ 0x00432D70..0x00432D76 =====
int __usercall sub_432D70@<eax>(int result@<eax>)
{
  dword_565BA0 = result;
  return result;
}

// ===== sub_432D80 @ 0x00432D80..0x00432DBC =====
int __usercall sub_432D80@<eax>(int a1@<edx>, unsigned int a2@<esi>)
{
  int result; // eax

  result = -2147483644;
  if ( !a1 )
  {
LABEL_4:
    dword_565BA4 = a1;
    return 0;
  }
  if ( a1 != 1 )
  {
    if ( a1 != 2 )
      return result;
    goto LABEL_4;
  }
  if ( a2 >= 0x10000 )
    return -2147483643;
  dword_565BA4 = 1;
  dword_565BA8 = a2;
  return 0;
}

// ===== sub_432DC0 @ 0x00432DC0..0x00432DC6 =====
int __usercall sub_432DC0@<eax>(int result@<eax>)
{
  dword_565BAC = result;
  return result;
}

// ===== sub_432DD0 @ 0x00432DD0..0x00432E24 =====
void *sub_432DD0()
{
  unsigned int i; // esi

  if ( dword_507658 )
  {
    dword_507658 = 0;
  }
  else
  {
    for ( i = 0; i < 0x1800; i += 24 )
    {
      if ( *(void **)((char *)&dword_50CA40 + i) )
        operator delete[](*(void **)((char *)&dword_50CA40 + i));
    }
  }
  return memset(&dword_50CA40, 0, 0x1800u);
}

// ===== sub_432E30 @ 0x00432E30..0x00432E40 =====
BOOL __usercall sub_432E30@<eax>(int a1@<eax>)
{
  return (unsigned int)(a1 - 65281) <= 0xFE;
}

// ===== sub_432E40 @ 0x00432E40..0x00432F50 =====
int __usercall sub_432E40@<eax>(int a1@<eax>, int a2@<edx>, int a3, int a4, int a5, int a6)
{
  unsigned __int8 v7; // dl
  int v8; // edi
  void **v9; // esi
  int result; // eax
  int v11[6]; // [esp+10h] [ebp-18h] BYREF

  if ( !sub_432E30(a2) )
    return -2147483642;
  v8 = v7;
  if ( a1 == -1 )
  {
    operator delete[](*(&dword_50CA40 + 6 * v7));
    result = 0;
    *(&dword_50CA40 + 6 * v8) = 0;
    dword_50CA44[6 * v8] = 0;
    dword_50CA48[6 * v8] = 0;
    dword_50CA4C[6 * v8] = 0;
    dword_50CA50[6 * v8] = 0;
    dword_50CA54[6 * v8] = 0;
  }
  else if ( sub_407F20(dword_565B70, a1, v11) )
  {
    if ( a5 && a6 )
    {
      v9 = &dword_50CA40 + 6 * v8;
      operator delete[](*v9);
      sub_409030(v11[4], a6, v9, a5);
      sub_40A620((int)v9, 0);
      sub_40A530(v11, v9, -a4, -a3, 128, 0);
      return 0;
    }
    else
    {
      return -2147483641;
    }
  }
  else
  {
    return -2147483646;
  }
  return result;
}

// ===== sub_432F50 @ 0x00432F50..0x00432FFD =====
int __cdecl sub_432F50(int a1, int a2)
{
  int v2; // ebx
  int v3; // esi
  int v4; // edi
  int v6; // [esp+10h] [ebp-18h] BYREF
  unsigned int v7; // [esp+18h] [ebp-10h]
  int v8; // [esp+1Ch] [ebp-Ch]

  if ( (unsigned int)a1 > 0xFF )
    return -2147483647;
  if ( a1 <= 0 )
  {
    sub_432DD0();
    return 0;
  }
  else if ( sub_407F20(dword_565B70, a2, &v6) )
  {
    v2 = v7 / a1;
    if ( v7 % a1 )
    {
      return -2147483645;
    }
    else
    {
      v3 = 0;
      v4 = 0;
      do
      {
        sub_432E40(a2, v3 + 65281, v4, 0, v2, v8);
        ++v3;
        v4 += v2;
      }
      while ( v3 < a1 );
      return 0;
    }
  }
  else
  {
    return -2147483646;
  }
}

// ===== sub_433000 @ 0x00433000..0x00433024 =====
int __fastcall sub_433000(int a1, int a2)
{
  unsigned __int8 v2; // dl

  if ( sub_432E30(a2) )
    return dword_50CA48[6 * v2];
  else
    return 0;
}

// ===== sub_433030 @ 0x00433030..0x00433054 =====
int __fastcall sub_433030(int a1, int a2)
{
  unsigned __int8 v2; // dl

  if ( sub_432E30(a2) )
    return dword_50CA4C[6 * v2];
  else
    return 0;
}

// ===== sub_433060 @ 0x00433060..0x00433171 =====
unsigned int __usercall sub_433060@<eax>(int *a1@<edi>, int a2, int a3, void *a4, int a5)
{
  int v5; // ecx
  void **v6; // eax
  void *v7; // edx
  void *v8; // eax
  int v9; // edx
  int v10; // ecx
  int v11; // edx
  int v12; // ecx
  int v13; // edx
  int *v14; // eax
  int v15; // ecx
  unsigned int result; // eax
  _DWORD v17[4]; // [esp+8h] [ebp-40h] BYREF
  void *v18; // [esp+18h] [ebp-30h]
  void *v19; // [esp+1Ch] [ebp-2Ch]
  _DWORD v20[2]; // [esp+20h] [ebp-28h] BYREF
  _DWORD v21[4]; // [esp+28h] [ebp-20h] BYREF
  int v22[4]; // [esp+38h] [ebp-10h] BYREF

  if ( !sub_432E30(a3) )
    return sub_4092E0(a4, (BOOL **)a2, (int)a1, a5);
  v5 = (int)*(&dword_50CA40 + 6 * (unsigned __int8)a3);
  v6 = &dword_50CA40 + 6 * (unsigned __int8)a3;
  v17[1] = dword_50CA44[6 * (unsigned __int8)a3];
  v17[2] = v6[2];
  v17[3] = v6[3];
  v7 = v6[4];
  v8 = v6[5];
  v17[0] = v5;
  v18 = v7;
  v19 = v8;
  if ( v5 )
  {
    v9 = *(_DWORD *)(a2 + 4);
    v20[0] = *(_DWORD *)a2;
    v10 = *(_DWORD *)(a2 + 8);
    v20[1] = v9;
    v11 = *(_DWORD *)(a2 + 12);
    v21[0] = v10;
    v12 = *(_DWORD *)(a2 + 16);
    v21[1] = v11;
    v13 = *(_DWORD *)(a2 + 20);
    v21[2] = v12;
    v21[3] = v13;
    sub_409190(v22, (int)v20);
    v14 = sub_409190(v21, (int)v17);
    sub_409110(v22, v14);
    sub_4091B0(v17, v22);
    if ( v18 == (void *)3 )
      sub_40E860((int)v17, a2, a5);
    else
      sub_40B080((size_t)v17, a2);
  }
  *a1 = a3;
  a1[1] = 1;
  a1[2] = 0;
  a1[3] = 0;
  a1[5] = sub_433000(v5, a3) - 1;
  a1[4] = 0;
  result = sub_433030(v15, a3) - 1;
  a1[6] = result;
  return result;
}

// ===== sub_433180 @ 0x00433180..0x004332F4 =====
void **__usercall sub_433180@<eax>(int a1@<edx>, int a2, void *a3, int a4, int a5, int a6)
{
  int v6; // ebx
  int v7; // edi
  unsigned __int8 v8; // dl
  void **result; // eax
  int v10; // esi
  int *v11; // ecx
  void *v12; // edx
  int v13; // edx
  int v14; // ecx
  int v15; // ecx
  int v16; // esi
  int v17; // eax
  int v18; // eax
  int v19; // edx
  int v20; // eax
  void **v21; // [esp+10h] [ebp-2Ch]
  void *v22; // [esp+14h] [ebp-28h]
  void *v23; // [esp+18h] [ebp-24h]
  void **v24; // [esp+1Ch] [ebp-20h]
  int *v25; // [esp+20h] [ebp-1Ch]
  int v26; // [esp+24h] [ebp-18h]
  int *v27; // [esp+28h] [ebp-14h]
  int v28; // [esp+2Ch] [ebp-10h]
  int v29; // [esp+30h] [ebp-Ch]
  int v30; // [esp+34h] [ebp-8h]
  unsigned int v31; // [esp+38h] [ebp-4h]

  v6 = a5;
  v7 = a4;
  if ( !sub_432E30(a1) )
    return (void **)sub_4094C0(a3, (int **)a2, a4, a5, a6);
  result = &dword_50CA40 + 6 * v8;
  v21 = result;
  if ( *result )
  {
    if ( result[4] == (void *)3 )
    {
      v10 = a2;
      if ( *(_DWORD *)(a2 + 16) == 2 )
      {
        v11 = *(int **)a2;
        v23 = result[3];
        v12 = result[1];
        result = (void **)result[2];
        v25 = *(int **)a2;
        v22 = v12;
        v24 = result;
        v29 = 0;
        if ( *(int *)(a2 + 12) > 0 )
        {
          result = *(void ***)(a2 + 8);
          do
          {
            v13 = 0;
            v27 = v11;
            v26 = 0;
            if ( (int)result > 0 )
            {
              v14 = -v6;
              v30 = -v6;
              do
              {
                v31 = 0;
                if ( v14 > v6 )
                  goto LABEL_22;
                v15 = v29 + v14 - v6;
                v16 = -v7;
                v17 = v6 - v30 + 1;
                v28 = v17;
                do
                {
                  if ( v16 <= v7 )
                  {
                    v18 = v13 + v16 - v7;
                    v19 = v7 - v16 + 1;
                    do
                    {
                      if ( v18 >= 0 && v18 < (int)v24 && v15 >= 0 && v15 < (int)v23 )
                      {
                        v31 += *((unsigned __int8 *)*v21 + (_DWORD)v22 * v15 + v18);
                        v7 = a4;
                      }
                      ++v18;
                      --v19;
                    }
                    while ( v19 );
                    v17 = v28;
                    v6 = a5;
                    v13 = v26;
                  }
                  ++v15;
                  v28 = --v17;
                }
                while ( v17 );
                v14 = v30;
                if ( v31 >= 0x100 )
                  v20 = 255;
                else
LABEL_22:
                  v20 = v31;
                *v27++ = a6 | (v20 << 24);
                v10 = a2;
                result = *(void ***)(a2 + 8);
                v26 = ++v13;
              }
              while ( v13 < (int)result );
              v11 = v25;
            }
            v11 = (int *)((char *)v11 + *(_DWORD *)(v10 + 4));
            v25 = v11;
            ++v29;
          }
          while ( v29 < *(_DWORD *)(v10 + 12) );
        }
      }
    }
  }
  return result;
}

// ===== sub_433300 @ 0x00433300..0x00433461 =====
int __cdecl sub_433300(int a1, int a2, _DWORD *a3)
{
  int v3; // ecx
  int v4; // ebx
  int v5; // edi
  int v6; // esi
  int result; // eax
  char *v8; // esi
  int i; // edi
  int v10; // ecx
  _DWORD v11[2]; // [esp+10h] [ebp-18h] BYREF

  v3 = dword_565B7C;
  v4 = 0;
  if ( dword_565B7C > 0 )
  {
    v5 = 0;
    v6 = 0;
    do
    {
      if ( *(_DWORD *)((char *)dword_565B80 + v6) )
      {
        operator delete[](*(void **)((char *)dword_565B80 + v6));
        v3 = dword_565B7C;
      }
      ++v5;
      v6 += 24;
    }
    while ( v5 < v3 );
    operator delete[](dword_565B80);
  }
  result = 1;
  dword_565B7C = a1;
  dword_565B80 = 0;
  if ( a1 <= 1 )
    return result;
  v8 = (char *)operator new[](24 * a1);
  dword_565B80 = v8;
  memset(v8, 0, 24 * a1);
  for ( i = 0; ; i += 24 )
  {
    v10 = *(_DWORD *)(a2 + 4 * v4);
    if ( v10 == -1 )
    {
      *(_DWORD *)&v8[i] = 0;
      *(_DWORD *)&v8[i + 4] = 0;
      *(_DWORD *)&v8[i + 8] = 0;
      *(_DWORD *)&v8[i + 12] = 0;
      *(_DWORD *)&v8[i + 16] = 0;
      *(_DWORD *)&v8[i + 20] = 0;
      goto LABEL_15;
    }
    if ( !sub_407F20(dword_565B70, v10, v11) )
      break;
    sub_409080((char *)dword_565B80 + i, 1);
    if ( sub_40A9E0((int)dword_565B80 + i, (int)v11, 0x80u, 0, 1) == 1 )
      break;
    v8 = (char *)dword_565B80;
LABEL_15:
    if ( ++v4 >= a1 )
      return 1;
  }
  if ( v4 >= a1 )
    return 1;
  result = 0;
  *a3 = *(_DWORD *)(a2 + 4 * v4);
  return result;
}

// ===== sub_433470 @ 0x00433470..0x0043347F =====
int sub_433470()
{
  return sub_433300(0, 0, 0);
}

// ===== sub_433480 @ 0x00433480..0x00433486 =====
int __usercall sub_433480@<eax>(int result@<eax>)
{
  dword_50765C = result;
  return result;
}

// ===== sub_433490 @ 0x00433490..0x004334A9 =====
int __usercall sub_433490@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  dword_565B84 = result;
  dword_565B88 = a2;
  dword_565B8C = a3;
  return result;
}

// ===== sub_4334B0 @ 0x004334B0..0x004334BC =====
int __usercall sub_4334B0@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_565B90 = result;
  dword_565B94 = a2;
  return result;
}

// ===== sub_4334C0 @ 0x004334C0..0x004334C6 =====
int __usercall sub_4334C0@<eax>(int result@<eax>)
{
  dword_507660 = result;
  return result;
}

// ===== sub_4334D0 @ 0x004334D0..0x004334E9 =====
BOOL __usercall sub_4334D0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  if ( a1 > 0 )
  {
    dword_507664 = a1;
    dword_507668 = a2;
  }
  return a1 > 0;
}

// ===== sub_4334F0 @ 0x004334F0..0x00433509 =====
BOOL __usercall sub_4334F0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  if ( a1 > 0 )
  {
    dword_50766C = a1;
    dword_507670 = a2;
  }
  return a1 > 0;
}

// ===== sub_433510 @ 0x00433510..0x0043351C =====
int __usercall sub_433510@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_565B98 = result;
  dword_565B9C = a2;
  return result;
}

// ===== sub_433520 @ 0x00433520..0x00433526 =====
int __usercall sub_433520@<eax>(int result@<eax>)
{
  dword_507674 = result;
  return result;
}

// ===== sub_433530 @ 0x00433530..0x00433567 =====
int __usercall sub_433530@<eax>(unsigned int a1@<edx>, unsigned int a2@<ecx>, unsigned int a3@<esi>)
{
  int result; // eax

  if ( a2 > 0x64 || a1 > 0x64 || a3 > 0x100 )
    return 0;
  result = 1;
  dword_507678 = a2;
  dword_50767C = a1;
  dword_507680 = 0;
  dword_507684 = a3;
  return result;
}

// ===== sub_433570 @ 0x00433570..0x0043359D =====
_DWORD *__usercall sub_433570@<eax>(_DWORD *result@<eax>)
{
  int v1; // edx
  int v2; // ecx
  int v3; // edx
  int v4; // ecx

  v1 = dword_507678;
  *result = dword_507674;
  v2 = dword_50767C;
  result[1] = v1;
  v3 = dword_507680;
  result[2] = v2;
  v4 = dword_507684;
  result[3] = v3;
  result[4] = v4;
  return result;
}

// ===== sub_4335A0 @ 0x004335A0..0x004335B6 =====
int __thiscall sub_4335A0(int *this, int a2)
{
  int v2; // edx

  v2 = *this;
  this[10] = a2;
  return (*(int (__stdcall **)(_DWORD))(v2 + 8))(0);
}

// ===== sub_4335C0 @ 0x004335C0..0x004335CD =====
int __thiscall sub_4335C0(_DWORD *this, int a2)
{
  int result; // eax

  result = a2;
  this[11] = a2;
  return result;
}

// ===== sub_4335D0 @ 0x004335D0..0x004335D4 =====
int __usercall sub_4335D0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 48) = a2;
  return result;
}

// ===== sub_4335E0 @ 0x004335E0..0x004335E4 =====
int __usercall sub_4335E0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 60) = a2;
  return result;
}

// ===== sub_4335F0 @ 0x004335F0..0x004335F4 =====
int __usercall sub_4335F0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 64) = a2;
  return result;
}

// ===== sub_433600 @ 0x00433600..0x004336A2 =====
int __thiscall sub_433600(_DWORD *this)
{
  int v3; // eax
  int v4; // ecx
  unsigned int v5; // eax
  bool v6; // zf
  int v7; // ebx

  sub_431AF0((int)this, (int)this);
  v3 = sub_46DF00(this[30]);
  v4 = dword_507690;
  v5 = (dword_507690 | 0x80000181) & v3;
  v6 = this[15] == 0;
  this[9] = v5;
  if ( v6 )
    this[9] = v5 & 0x7FFFFFFF;
  if ( !this[16] )
    this[9] &= ~(v4 | 0x80);
  if ( this[9] )
  {
    sub_4337D0(1);
    if ( dword_565BA0 )
      sub_4337E0();
  }
  v7 = (*(int (__thiscall **)(_DWORD *))(*this + 36))(this);
  sub_431AA0(this);
  if ( !v7 && sub_4319C0((int)this) )
    return 0;
  sub_4450D0();
  return 1;
}

// ===== sub_4336B0 @ 0x004336B0..0x00433774 =====
int __thiscall sub_4336B0(int this)
{
  int v2; // ebx
  char v3; // al
  int v4; // eax
  int v5; // eax
  int v7; // [esp+Ch] [ebp-4h]

  v7 = 0;
  if ( !sub_431A60((unsigned int *)this)
    && !*(_DWORD *)(this + 52)
    && !*(_DWORD *)(this + 116)
    && !*(_DWORD *)(this + 92) )
  {
    return 0;
  }
  v2 = 1;
  do
  {
    if ( *(_DWORD *)(this + 96) )
    {
      sub_4339A0(this);
    }
    else
    {
      v3 = **(_BYTE **)(this + 40);
      if ( v3 )
      {
        v4 = v3 - 1;
        if ( v4 )
        {
          v5 = v4 - 9;
          if ( v5 )
          {
            if ( v5 == 2 )
              *(_DWORD *)(this + 40) += (*(int (__thiscall **)(int))(*(_DWORD *)this + 40))(this);
            else
              *(_DWORD *)(this + 40) += sub_433A70(this);
          }
          else
          {
            *(_DWORD *)(this + 40) += sub_433960();
          }
        }
        else
        {
          *(_DWORD *)(this + 40) += sub_4337F0();
        }
      }
      else
      {
        v7 = sub_433E40(this);
        v2 = 0;
      }
    }
  }
  while ( (sub_431A60((unsigned int *)this) || *(_DWORD *)(this + 52) || *(_DWORD *)(this + 48)) && v2 );
  return v7;
}

// ===== sub_433780 @ 0x00433780..0x004337B4 =====
int __thiscall sub_433780(_DWORD *this, int a2)
{
  if ( !a2 || this[13] || this[12] && !this[24] )
    return sub_431A10(this, 0);
  else
    return sub_431A30((int)this, a2);
}

// ===== sub_4337C0 @ 0x004337C0..0x004337CF =====
void __thiscall sub_4337C0(_DWORD *this)
{
  if ( !dword_565BAC )
    sub_431A90(this);
}

// ===== sub_4337D0 @ 0x004337D0..0x004337D4 =====
int __usercall sub_4337D0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 52) = a2;
  return result;
}

// ===== sub_4337E0 @ 0x004337E0..0x004337E8 =====
int __usercall sub_4337E0@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 116) = 1;
  return result;
}

// ===== sub_4337F0 @ 0x004337F0..0x0043388B =====
int __usercall sub_4337F0@<eax>(_DWORD *a1@<edi>)
{
  _DWORD *v1; // esi
  int v2; // eax
  int v3; // eax
  _DWORD v5[4]; // [esp+8h] [ebp-18h] BYREF
  int v6; // [esp+18h] [ebp-8h] BYREF

  if ( !a1[27] )
  {
    v1 = (_DWORD *)a1[8];
    sub_42C2A0(v5, v1);
    sub_42C720(&v6, (int)v1);
    v2 = sub_42C4E0(v1);
    a1[28] = v2 - (v6 == v5[0]);
    if ( sub_42C600(v1) )
    {
      do
        --a1[28];
      while ( sub_42C600((_DWORD *)a1[8]) );
    }
    a1[27] = 1;
  }
  v3 = a1[28];
  if ( v3 <= 0 )
  {
    sub_42C5B0((_DWORD *)a1[8]);
    a1[26] = 0;
    a1[27] = 0;
    return 1;
  }
  else
  {
    a1[28] = v3 - 1;
    sub_433990();
    return 0;
  }
}

// ===== sub_433890 @ 0x00433890..0x0043395B =====
int __thiscall sub_433890(int *this)
{
  int v2; // ecx
  int v3; // ecx
  int v4; // eax
  int v5; // eax
  int v6; // eax
  int v8; // [esp+Ch] [ebp-14h]
  _DWORD v9[4]; // [esp+10h] [ebp-10h] BYREF

  if ( !this[25] )
  {
    v2 = dword_507670;
    this[21] = dword_50766C;
    this[22] = v2;
  }
  (*(void (__thiscall **)(int *, int))(*this + 8))(this, this[22]);
  v3 = this[21];
  v4 = this[25];
  if ( v4 >= v3 - 1 )
  {
    v6 = this[8];
    this[25] = 0;
    sub_42B9B0(v6);
    sub_42B550(this[8], 0);
    sub_42CA00(this[8]);
    sub_42C5B0((_DWORD *)this[8]);
    this[26] = 0;
    v8 = 1;
  }
  else
  {
    v5 = v4 + 1;
    this[25] = v5;
    sub_42B550(this[8], (v5 << 8) / v3);
    sub_42CA00(this[8]);
    v8 = 0;
  }
  sub_42C2F0(v9, this[8]);
  (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
  sub_443240(dword_565B6C);
  (*(void (__thiscall **)(int *))(*this + 16))(this);
  return v8;
}

// ===== sub_433960 @ 0x00433960..0x00433982 =====
int __usercall sub_433960@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int result; // eax

  if ( !sub_42C600(*(_DWORD **)(a2 + 32)) )
    sub_433990(a1);
  result = 1;
  *(_DWORD *)(a2 + 104) = 0;
  return result;
}

// ===== sub_433990 @ 0x00433990..0x00433998 =====
int __usercall sub_433990@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 96) = 1;
  return result;
}

// ===== sub_4339A0 @ 0x004339A0..0x00433A66 =====
int __stdcall sub_4339A0(int *a1)
{
  int v2; // ecx
  _DWORD *v3; // esi
  int v4; // eax
  int v5; // edi
  _DWORD v7[4]; // [esp+Ch] [ebp-18h] BYREF
  int v8; // [esp+1Ch] [ebp-8h]
  int v9; // [esp+2Ch] [ebp+8h]

  if ( !a1[25] )
  {
    v2 = dword_507668;
    a1[19] = dword_507664;
    a1[20] = v2;
  }
  (*(void (__thiscall **)(int *, int))(*a1 + 8))(a1, a1[20]);
  if ( a1[13] )
    v9 = a1[19] - a1[25];
  else
    v9 = 1;
  v3 = (_DWORD *)a1[8];
  v4 = sub_42C4C0(v3);
  v5 = a1[19];
  v8 = v4 * (v9 + a1[25]) / v5;
  sub_42B9D0(v3, v8 - v4 * a1[25] / v5);
  a1[25] += v9;
  if ( a1[25] >= a1[19] )
  {
    a1[24] = 0;
    a1[25] = 0;
  }
  sub_42C2F0(v7, a1[8]);
  (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
  sub_443240(dword_565B6C);
  return (*(int (__thiscall **)(int *))(*a1 + 16))(a1);
}

// ===== sub_433A70 @ 0x00433A70..0x00433E35 =====
int __stdcall sub_433A70(int *a1)
{
  int v2; // edi
  int v3; // esi
  int v4; // edi
  _DWORD *v5; // esi
  int v6; // esi
  int v7; // edi
  int v9; // [esp+0h] [ebp-DCh] BYREF
  _BYTE v10[32]; // [esp+14h] [ebp-C8h] BYREF
  int v11; // [esp+34h] [ebp-A8h]
  int v12; // [esp+38h] [ebp-A4h]
  int v13; // [esp+44h] [ebp-98h]
  int v14; // [esp+4Ch] [ebp-90h] BYREF
  void *v15[6]; // [esp+50h] [ebp-8Ch] BYREF
  void *v16[6]; // [esp+68h] [ebp-74h] BYREF
  int v17[6]; // [esp+80h] [ebp-5Ch] BYREF
  int v18; // [esp+98h] [ebp-44h]
  int pExceptionObject; // [esp+9Ch] [ebp-40h] BYREF
  int v20[2]; // [esp+A0h] [ebp-3Ch] BYREF
  int v21; // [esp+A8h] [ebp-34h]
  int v22; // [esp+ACh] [ebp-30h]
  int v23; // [esp+B0h] [ebp-2Ch]
  int v24; // [esp+B4h] [ebp-28h]
  int v25; // [esp+B8h] [ebp-24h] BYREF
  int v26; // [esp+BCh] [ebp-20h]
  int v27; // [esp+C0h] [ebp-1Ch]
  int v28[3]; // [esp+C4h] [ebp-18h] BYREF
  int v29; // [esp+D8h] [ebp-4h]
  int v30; // [esp+E4h] [ebp+8h]
  int v31; // [esp+E4h] [ebp+8h]

  v28[2] = (int)&v9;
  v2 = 0;
  v23 = sub_42EA10(v28);
  (*(void (__thiscall **)(int *, int))(*a1 + 8))(a1, dword_507660);
  v3 = a1[8];
  sub_42C450(v3);
  if ( sub_4092B0((int)v10, dword_565B70) )
  {
    v4 = v11 * v12 / 100;
    v30 = sub_42C520(v3);
    sub_409080(v16, 1);
    sub_4092E0((void *)v13, (BOOL **)v16, (int)&v14, a1[11]);
    v17[0] = (int)v16[0];
    v17[1] = (int)v16[1];
    v17[2] = (int)v16[2];
    v17[3] = (int)v16[3];
    v17[4] = (int)v16[4];
    v17[5] = (int)v16[5];
    if ( v30 )
    {
      sub_409A80(v17, (int)&v14);
      v27 = sub_409710(&v14, v11);
      v31 = (int)v15[4] + 2 * v27 - (unsigned int)v15[2] + 1;
    }
    else
    {
      v27 = 0;
      if ( v23 )
        v31 = v4;
      else
        v31 = v4 / 2;
    }
    v29 = 0;
    if ( a1[25] )
    {
      sub_42C720(&v25, a1[8]);
      v25 += v27;
      v7 = v11 * dword_507678 / 100;
      v27 = v7;
      v28[0] = v11 * dword_50767C / 100;
      if ( dword_507674 )
      {
        sub_409080(v15, 1);
        sub_40A710(v15, 0, 0);
        sub_40A9E0((int)v15, (int)v17, 5u, 0x100u, 1);
        sub_42B5B0((_DWORD *)a1[8], v20, v25 + v7, v26 + v28[0], (int *)v15, 1, 256 - dword_507684);
        operator delete[](v15[0]);
      }
      sub_42B5B0((_DWORD *)a1[8], v20, v25, v26, v17, 0, 0);
      sub_42BAB0(a1[8], 0, 0);
      if ( dword_507674 )
      {
        v21 += v27;
        v22 += v28[0];
      }
      (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
      sub_443240(dword_565B6C);
      sub_42C6E0((void *)a1[8]);
      --a1[25];
      v2 = (v23 != 0) + 1;
    }
    else
    {
      v5 = (_DWORD *)a1[8];
      if ( !sub_42C680(v31, v5) )
      {
        v18 = 1;
        if ( sub_42C540((int)v5) && sub_42EA30(v28[0]) && !a1[26] )
        {
          a1[26] = 1;
          v18 = 0;
        }
        else
        {
          a1[26] = 0;
          if ( !sub_42C600(v5) )
          {
            sub_433990((int)a1);
            pExceptionObject = 0;
            _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1H);
          }
        }
      }
      v6 = a1[8];
      sub_42C720(&v25, v6);
      sub_42BB30(0, (int)v17, v6);
      sub_42BAE0(a1[8], 0, v27 + v25, v26, 170);
      sub_42BAB0(a1[8], 0, 1);
      sub_42C330(0, a1[8], v20);
      (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
      sub_443240(dword_565B6C);
      ++a1[25];
      v2 = 0;
    }
    v24 = v2;
    (*(void (__thiscall **)(int *))(*a1 + 16))(a1);
    v29 = -1;
    operator delete[](v16[0]);
  }
  return v2;
}

// ===== sub_433E40 @ 0x00433E40..0x00433F3A =====
int __stdcall sub_433E40(int *a1)
{
  int v2; // esi
  _DWORD v3[5]; // [esp+10h] [ebp-14h] BYREF

  if ( !a1[14] || a1[29] )
  {
    if ( sub_42C330(0, a1[8], v3) )
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
      sub_443240(dword_565B6C);
      sub_42BAB0(a1[8], 0, 0);
    }
    (*(void (__thiscall **)(int *))(*a1 + 16))(a1);
    return 1;
  }
  else
  {
    if ( dword_565B98 && !a1[25] )
      a1[23] = dword_565B9C + sub_498720();
    if ( a1[17] )
    {
      if ( a1[18] > (unsigned int)sub_498720() && !a1[13] )
        return 0;
      a1[17] = 0;
    }
    if ( sub_431A60((unsigned int *)a1) || a1[13] )
      v2 = sub_433F40(a1);
    else
      v2 = 0;
    if ( dword_565B98 && !v2 && a1[23] <= (unsigned int)sub_498720() )
      sub_4337E0((int)a1);
    return v2;
  }
}

// ===== sub_433F40 @ 0x00433F40..0x00434130 =====
BOOL __stdcall sub_433F40(int *a1)
{
  unsigned int v1; // edi
  unsigned int *v2; // ecx
  int v3; // edx
  _DWORD *v4; // esi
  bool v5; // zf
  int v6; // ecx
  int v7; // eax
  _DWORD *v8; // esi
  int v9; // ecx
  int v10; // eax
  int v11; // ecx
  _DWORD v13[4]; // [esp+Ch] [ebp-28h] BYREF
  _DWORD v14[2]; // [esp+1Ch] [ebp-18h] BYREF
  _DWORD v15[2]; // [esp+24h] [ebp-10h] BYREF
  BOOL v16; // [esp+2Ch] [ebp-8h]

  if ( !a1[25] )
  {
    v1 = 0;
    if ( dword_565B7C > 0 )
    {
      v2 = (unsigned int *)((char *)dword_565B80 + 8);
      v3 = dword_565B7C;
      do
      {
        if ( v1 < *v2 )
          v1 = *v2;
        v2 += 6;
        --v3;
      }
      while ( v3 );
    }
    v4 = (_DWORD *)a1[8];
    if ( !sub_42C680(v1, v4) )
    {
      if ( !sub_42C600(v4) )
      {
        sub_433990((int)a1);
        return 0;
      }
      (*(void (__thiscall **)(int *))(*a1 + 44))(a1);
    }
    v5 = a1[9] >= 0;
    a1[9] &= 0x80000000;
    if ( v5 )
      sub_4337D0((int)a1, 0);
    sub_4335D0((int)a1, 0);
    (*(void (__thiscall **)(int *, int))(*a1 + 8))(a1, v6);
    ++a1[25];
  }
  if ( sub_42C330(0, a1[8], v13) )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
  }
  v16 = a1[9] != 0;
  if ( v16 )
  {
    sub_42BAB0(a1[8], 0, 0);
    a1[25] = 0;
  }
  else
  {
    (*(void (__thiscall **)(int *, int))(*a1 + 8))(a1, dword_50765C);
    if ( dword_565B7C > 0 )
    {
      v7 = a1[25];
      v8 = (char *)dword_565B80 + 24 * v7 - 24;
      v9 = 1;
      if ( v7 >= dword_565B7C )
        v9 = 1 - dword_565B7C;
      a1[25] = v9 + v7;
      if ( *v8 )
      {
        if ( dword_565B84 == 1 )
        {
          v10 = dword_565B88;
          v11 = dword_565B8C;
        }
        else
        {
          sub_42C720(v14, a1[8]);
          (*(void (__thiscall **)(int *, _DWORD *))(*a1 + 48))(a1, v15);
          v10 = v15[0] + dword_565B88 + v14[0];
          v11 = dword_565B8C + v15[1] + v14[1];
        }
        sub_42BAE0(a1[8], 0, v10, v11, 0);
        sub_42BB30(0, (int)v8, a1[8]);
        sub_42BAB0(a1[8], 0, 1);
      }
      else
      {
        sub_42BAB0(a1[8], 0, 0);
      }
    }
  }
  if ( sub_42C330(0, a1[8], v13) )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
  }
  (*(void (__thiscall **)(int *))(*a1 + 16))(a1);
  return v16;
}
