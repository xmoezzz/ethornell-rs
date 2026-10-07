#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_46C090 @ 0x0046C090..0x0046C096 =====
int sub_46C090()
{
  return dword_5667C4;
}

// ===== sub_46C0A0 @ 0x0046C0A0..0x0046C195 =====
BOOL __usercall sub_46C0A0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<edi>, int a3)
{
  BOOL v3; // ebx
  _DWORD *i; // esi
  char *v5; // ecx
  _BYTE *v6; // edx
  char v7; // al
  char *v8; // ecx
  _BYTE *v9; // edx
  char v10; // al
  char *v11; // ecx
  _BYTE *v12; // edx
  char v13; // al
  char *v14; // ecx
  _BYTE *v15; // edx
  char v16; // al
  char *v17; // ecx
  _BYTE *v18; // edx
  char v19; // al

  v3 = a1 < dword_5667C4;
  if ( a1 >= dword_5667C4 )
    return 0;
  for ( i = (_DWORD *)dword_5667BC; a1; i = (_DWORD *)i[15] )
    --a1;
  memset(a2, 0, 0x200u);
  *a2 = *i;
  a2[16] = i[1];
  a2[17] = i[2];
  a2[18] = i[3];
  a2[19] = i[4];
  a2[20] = i[5];
  a2[21] = i[6];
  a2[22] = i[7];
  a2[23] = i[8];
  v5 = (char *)i[9];
  if ( v5 )
  {
    v6 = a2 + 40;
    do
    {
      v7 = *v5;
      *v6++ = *v5++;
    }
    while ( v7 );
  }
  v8 = (char *)i[10];
  if ( v8 )
  {
    v9 = a2 + 48;
    do
    {
      v10 = *v8;
      *v9++ = *v8++;
    }
    while ( v10 );
  }
  v11 = (char *)i[11];
  if ( v11 )
  {
    v12 = a2 + 56;
    do
    {
      v13 = *v11;
      *v12++ = *v11++;
    }
    while ( v13 );
  }
  v14 = (char *)i[12];
  v15 = a2 + 64;
  do
  {
    v16 = *v14;
    *v15++ = *v14++;
  }
  while ( v16 );
  if ( a3 )
  {
    v17 = (char *)i[13];
    if ( v17 )
    {
      v18 = a2 + 128;
      do
      {
        v19 = *v17;
        *v18++ = *v17++;
      }
      while ( v19 );
    }
  }
  return v3;
}

// ===== sub_46C1A0 @ 0x0046C1A0..0x0046C1C1 =====
_DWORD *__thiscall sub_46C1A0(void *this)
{
  _DWORD *result; // eax

  for ( result = dword_5667D8; dword_5667D8; result = dword_5667D8 )
    sub_46C220(this, *result);
  return result;
}

// ===== sub_46C1D0 @ 0x0046C1D0..0x0046C21F =====
int __usercall sub_46C1D0@<eax>(int a1@<edi>, int a2@<esi>, _DWORD *a3)
{
  int result; // eax
  _DWORD *v4; // eax

  result = 2;
  if ( a2 )
  {
    if ( a1 )
    {
      v4 = operator new(0x14u);
      *v4 = ++dword_5667C8;
      v4[1] = a2;
      v4[2] = a1;
      v4[3] = 0;
      v4[4] = dword_5667D8;
      dword_5667D8 = v4;
      *a3 = *v4;
      return 0;
    }
  }
  return result;
}

// ===== sub_46C220 @ 0x0046C220..0x0046C27C =====
int __fastcall sub_46C220(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void **v5; // edi
  void **v6; // ebx
  void *v7; // ecx

  v2 = (int *)dword_5667D8;
  result = 1;
  v4 = &dword_5667C8;
  if ( dword_5667D8 )
  {
    while ( *v2 != a2 )
    {
      v4 = v2;
      v2 = (int *)v2[4];
      if ( !v2 )
        return result;
    }
    v4[4] = v2[4];
    v5 = (void **)v2[3];
    while ( v5 )
    {
      v6 = v5;
      v7 = *v5;
      v5 = (void **)v5[1];
      operator delete[](v7);
      operator delete(v6);
    }
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_46C280 @ 0x0046C280..0x0046C29C =====
void **__thiscall sub_46C280(void *this)
{
  void **result; // eax

  result = (void **)dword_5667D8;
  if ( dword_5667D8 )
  {
    do
    {
      if ( *result == this )
        break;
      result = (void **)result[4];
    }
    while ( result );
  }
  return result;
}

// ===== sub_46C2A0 @ 0x0046C2A0..0x0046C2D5 =====
int __cdecl sub_46C2A0(_DWORD *a1)
{
  void *v1; // ecx
  void **v2; // eax
  int v3; // edx
  _DWORD *v4; // eax
  int i; // ecx

  v2 = sub_46C280(v1);
  if ( !v2 )
    return v3;
  v4 = v2[3];
  for ( i = 0; v4; ++i )
    v4 = (_DWORD *)v4[1];
  *a1 = i;
  return 0;
}

// ===== sub_46C2E0 @ 0x0046C2E0..0x0046C378 =====
int __cdecl sub_46C2E0(void *Src)
{
  void *v1; // ecx
  int v2; // edx
  void **v3; // edi
  void **v4; // esi
  void *v5; // eax
  void **v6; // ebx
  void *v7; // eax
  void **v9; // [esp+8h] [ebp-8h]
  unsigned int v10; // [esp+Ch] [ebp-4h]

  v3 = sub_46C280(v1);
  if ( !v3 )
    return v2;
  v4 = (void **)operator new(8u);
  v5 = operator new[]((unsigned int)v3[2]);
  *v4 = v5;
  v4[1] = v3[3];
  memcpy_0(v5, Src, (size_t)v3[2]);
  v3[3] = v4;
  v9 = 0;
  v10 = 0;
  do
  {
    if ( v10 >= (unsigned int)v3[1] )
    {
      v6 = v4;
      v7 = *v4;
      v4 = (void **)v4[1];
      operator delete[](v7);
      operator delete(v6);
      v9[1] = 0;
    }
    else
    {
      v9 = v4;
      v4 = (void **)v4[1];
    }
    ++v10;
  }
  while ( v4 );
  return 0;
}

// ===== sub_46C380 @ 0x0046C380..0x0046C3DD =====
int __usercall sub_46C380@<eax>(void *a1@<ecx>, int a2@<edi>, void *a3)
{
  void **v3; // eax
  int v4; // edx
  const void **v5; // esi
  int v6; // ecx

  v3 = sub_46C280(a1);
  if ( !v3 )
    return v4;
  v5 = (const void **)v3[3];
  v6 = 0;
  if ( v5 )
  {
    while ( v6 != a2 )
    {
      v5 = (const void **)v5[1];
      ++v6;
      if ( !v5 )
        return 2;
    }
    memcpy_0(a3, *v5, (size_t)v3[2]);
  }
  return v5 != 0 ? 0 : 2;
}

// ===== sub_46C3E0 @ 0x0046C3E0..0x0046C463 =====
int __cdecl sub_46C3E0(unsigned int a1)
{
  void *v1; // ecx
  void **v2; // eax
  int v3; // edx
  void **v4; // esi
  void ***v5; // ebx
  int v6; // eax
  void **v7; // edi
  void *v8; // eax
  unsigned int i; // [esp+4h] [ebp-4h]

  v2 = sub_46C280(v1);
  if ( !v2 )
    return 1;
  v4 = (void **)v2[3];
  v5 = (void ***)(v2 + 3);
  v6 = 0;
  if ( v3 )
  {
    while ( v4 )
    {
      v5 = (void ***)(v4 + 1);
      v4 = (void **)v4[1];
      if ( ++v6 == v3 )
        goto LABEL_5;
    }
    return 2;
  }
LABEL_5:
  if ( !v4 )
    return 2;
  for ( i = 0; i < a1; ++i )
  {
    if ( !v4 )
      break;
    v7 = v4;
    v8 = *v4;
    v4 = (void **)v4[1];
    operator delete[](v8);
    operator delete(v7);
  }
  *v5 = v4;
  return 0;
}

// ===== sub_46C470 @ 0x0046C470..0x0046C483 =====
int __fastcall sub_46C470(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    dword_5667DC = a1;
    return 1;
  }
  return result;
}

// ===== sub_46C490 @ 0x0046C490..0x0046C496 =====
int sub_46C490()
{
  return dword_5667DC;
}

// ===== sub_46C4A0 @ 0x0046C4A0..0x0046C4CF =====
int __usercall sub_46C4A0@<eax>(int a1@<esi>)
{
  void *v1; // eax
  int v2; // eax
  int *v3; // ecx

  v1 = operator new(0xCu);
  v2 = sub_490AE0(v1);
  *v3 = v2;
  v3[1] = a1;
  v3[2] = (int)dword_5667E8;
  ++dword_5667E0;
  dword_5667E8 = v3;
  return *v3;
}

// ===== sub_46C4D0 @ 0x0046C4D0..0x0046C4F0 =====
int __fastcall sub_46C4D0(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = dword_5667E8;
  result = 0;
  if ( dword_5667E8 )
  {
    while ( *v2 != a2 )
    {
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return result;
    }
    return v2[1];
  }
  return result;
}

// ===== sub_46C4F0 @ 0x0046C4F0..0x0046C536 =====
int __fastcall sub_46C4F0(int a1, int a2)
{
  int *v2; // esi
  int result; // eax
  int *v4; // ecx
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = (int *)dword_5667E8;
  result = 0;
  v4 = &dword_5667E0;
  if ( dword_5667E8 )
  {
    while ( *v2 != a2 )
    {
      v4 = v2;
      v2 = (int *)v2[2];
      if ( !v2 )
        return result;
    }
    v4[2] = v2[2];
    v5 = (void (__thiscall ***)(_DWORD, int))v2[1];
    if ( v5 )
      (**v5)(v5, 1);
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_46C540 @ 0x0046C540..0x0046C56B =====
int *__thiscall sub_46C540(void *this)
{
  int *result; // eax

  for ( result = (int *)dword_5667E8; dword_5667E8; result = (int *)dword_5667E8 )
    sub_46C4F0((int)this, *result);
  dword_5667E0 = 0;
  return result;
}

// ===== sub_46C570 @ 0x0046C570..0x0046C5AC =====
BOOL sub_46C570()
{
  _DWORD *v0; // esi
  BOOL v1; // ecx

  v0 = dword_5667E8;
  v1 = 1;
  if ( dword_5667E8 )
  {
    do
    {
      if ( !v1 )
        break;
      if ( sub_4477B0(v0[1]) )
        v1 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)v0[1] + 4))(v0[1]) == 0;
      v0 = (_DWORD *)v0[2];
    }
    while ( v0 );
  }
  return v1;
}

// ===== sub_46C5B0 @ 0x0046C5B0..0x0046C5DC =====
void sub_46C5B0()
{
  _DWORD *v0; // esi
  _DWORD *v1; // ecx

  v0 = dword_5667E8;
  if ( dword_5667E8 )
  {
    do
    {
      if ( sub_4477B0(v0[1]) )
        sub_4477C0(v1);
      v0 = (_DWORD *)v0[2];
    }
    while ( v0 );
  }
}

// ===== sub_46C5E0 @ 0x0046C5E0..0x0046C606 =====
BOOL __usercall sub_46C5E0@<eax>(int a1@<esi>)
{
  int v1; // eax

  v1 = sub_443270((int)dword_56674C, a1);
  return v1 && sub_41AD50(v1) != 0;
}

// ===== sub_46C610 @ 0x0046C610..0x0046C630 =====
int __fastcall sub_46C610(int a1, int a2)
{
  int v2; // eax
  bool v3; // zf
  int v4; // ecx
  int result; // eax

  v2 = sub_46C4D0(a1, a2);
  if ( !v2 )
    return 0;
  v3 = sub_42D560(v2) == 128;
  result = v4;
  if ( !v3 )
    return 0;
  return result;
}

// ===== sub_46C630 @ 0x0046C630..0x0046C6EE =====
int __cdecl sub_46C630(int a1, void *a2)
{
  void *v2; // esi
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  _DWORD *v5; // eax

  v2 = (void *)sub_4406C0(a1, (int)dword_56674C);
  if ( a2 )
  {
    if ( a2 != (void *)1 )
      return 0;
    v3 = operator new(0xD8u);
    if ( v3 )
    {
      v4 = sub_44A7C0(v2, v3);
      goto LABEL_8;
    }
  }
  else
  {
    v5 = operator new(0xA4u);
    if ( v5 )
    {
      v4 = sub_447990(v2, v5);
      goto LABEL_8;
    }
  }
  v4 = 0;
LABEL_8:
  if ( v4 )
    return sub_46C4A0((int)v4);
  return 0;
}

// ===== sub_46C6F0 @ 0x0046C6F0..0x0046C6F5 =====
// attributes: thunk
int __fastcall sub_46C6F0(int a1, int a2)
{
  return sub_46C4F0(a1, a2);
}

// ===== sub_46C700 @ 0x0046C700..0x0046C742 =====
void __usercall sub_46C700(void *a1@<esi>)
{
  int v1; // ebx
  int v2; // edi

  if ( *((_DWORD *)a1 + 1) )
  {
    v1 = 0;
    if ( *(int *)a1 > 0 )
    {
      v2 = 0;
      do
      {
        if ( *(_DWORD *)(v2 + *((_DWORD *)a1 + 1) + 4) )
          operator delete[](*(void **)(v2 + *((_DWORD *)a1 + 1) + 4));
        ++v1;
        v2 += 52;
      }
      while ( v1 < *(_DWORD *)a1 );
    }
    operator delete[](*((void **)a1 + 1));
  }
  operator delete(a1);
}

// ===== sub_46C750 @ 0x0046C750..0x0046C8D9 =====
int __cdecl sub_46C750(_DWORD *a1, int *a2, int a3)
{
  _DWORD *v3; // ebx
  const void *v4; // edi
  int v5; // esi
  const void *v6; // eax
  _DWORD *v7; // ebx
  int v8; // eax
  int v9; // esi
  int v10; // eax
  int v11; // edx
  const void *v12; // esi
  void *v13; // edi
  int result; // eax
  int v15; // edi
  int v16; // [esp+Ch] [ebp-18h]
  int v17; // [esp+10h] [ebp-14h]
  _DWORD *v18; // [esp+14h] [ebp-10h]
  _DWORD *v19; // [esp+18h] [ebp-Ch]
  int v20; // [esp+1Ch] [ebp-8h]
  int v21; // [esp+20h] [ebp-4h]

  v21 = 0;
  v3 = operator new(0x20u);
  qmemcpy(v3, a2, 0x20u);
  v18 = v3;
  v4 = (const void *)sub_48DDD0(a3);
  if ( !v4 || (v5 = *a2, *a2 <= 0) || v5 > 256 )
  {
    v3[1] = 0;
    v15 = 2;
    goto LABEL_21;
  }
  v3[1] = operator new[](52 * v5);
  v17 = 0;
  if ( *a2 <= 0 )
    goto LABEL_18;
  v20 = 0;
  v6 = v4;
  v19 = v4;
  while ( 1 )
  {
    v7 = (_DWORD *)(v20 + v3[1]);
    if ( v21 )
      goto LABEL_15;
    qmemcpy(v7, v6, 0x34u);
    v8 = sub_48DDD0(a3);
    v9 = (unsigned __int16)*v19;
    v16 = v8;
    if ( !v8 || (unsigned int)(v9 - 1) > 0xFF )
    {
      v21 = 3;
LABEL_15:
      v7[1] = 0;
      goto LABEL_16;
    }
    v7[1] = operator new[](60 * v9);
    if ( v9 > 0 )
    {
      v10 = 0;
      v11 = v9;
      do
      {
        v12 = (const void *)(v10 + v16);
        v13 = (void *)(v10 + v7[1]);
        v10 += 60;
        --v11;
        qmemcpy(v13, v12, 0x3Cu);
      }
      while ( v11 );
    }
LABEL_16:
    v20 += 52;
    v6 = v19 + 13;
    ++v17;
    v19 += 13;
    if ( v17 >= *a2 )
      break;
    v3 = v18;
  }
  v3 = v18;
  if ( !v21 )
  {
LABEL_18:
    *a1 = v3;
    return v21;
  }
  v15 = v21;
LABEL_21:
  sub_46C700(v3);
  result = v15;
  *a1 = 0;
  return result;
}

// ===== sub_46C8E0 @ 0x0046C8E0..0x0046C97F =====
int __fastcall sub_46C8E0(int a1, int a2, int *a3, int a4)
{
  int v4; // eax
  int v5; // edi
  int result; // eax
  int *v7; // esi
  int v8; // eax
  int *v9; // [esp+4h] [ebp-4h] BYREF

  v4 = sub_46C610(a1, a2);
  v5 = v4;
  if ( !v4 )
    return 1;
  if ( sub_4484B0(v4) )
    return 4;
  result = sub_46C750(&v9, a3, a4);
  if ( !result )
  {
    v7 = v9;
    v8 = sub_447C10(v5, v9);
    if ( v8 )
    {
      if ( v8 == -2147483647 )
      {
        sub_46C700(v7);
        return 2;
      }
      if ( v8 == -2147483646 )
      {
        sub_46C700(v7);
        return 3;
      }
    }
    sub_46C700(v7);
    return 0;
  }
  return result;
}

// ===== sub_46C980 @ 0x0046C980..0x0046C9C2 =====
void __usercall sub_46C980(void *a1@<esi>)
{
  int v1; // ebx
  int v2; // edi

  if ( *((_DWORD *)a1 + 1) )
  {
    v1 = 0;
    if ( *(int *)a1 > 0 )
    {
      v2 = 0;
      do
      {
        if ( *(_DWORD *)(v2 + *((_DWORD *)a1 + 1) + 8) )
          operator delete[](*(void **)(v2 + *((_DWORD *)a1 + 1) + 8));
        ++v1;
        v2 += 64;
      }
      while ( v1 < *(_DWORD *)a1 );
    }
    operator delete[](*((void **)a1 + 1));
  }
  operator delete(a1);
}

// ===== sub_46C9D0 @ 0x0046C9D0..0x0046CB5F =====
int __cdecl sub_46C9D0(_DWORD *a1, int *a2, int a3)
{
  _DWORD *v3; // ebx
  const void *v4; // edi
  int v5; // esi
  const void *v6; // eax
  _DWORD *v7; // ebx
  int v8; // eax
  int v9; // esi
  int v10; // eax
  int v11; // edx
  const void *v12; // esi
  void *v13; // edi
  int result; // eax
  int v15; // edi
  int v16; // [esp+Ch] [ebp-18h]
  int v17; // [esp+10h] [ebp-14h]
  _DWORD *v18; // [esp+14h] [ebp-10h]
  _DWORD *v19; // [esp+18h] [ebp-Ch]
  int v20; // [esp+1Ch] [ebp-8h]
  int v21; // [esp+20h] [ebp-4h]

  v21 = 0;
  v3 = operator new(0x28u);
  qmemcpy(v3, a2, 0x28u);
  v18 = v3;
  v4 = (const void *)sub_48DDD0(a3);
  if ( !v4 || (v5 = *a2, *a2 <= 0) || v5 > 256 )
  {
    v3[1] = 0;
    v15 = 2;
    goto LABEL_21;
  }
  v3[1] = operator new[](v5 << 6);
  v17 = 0;
  if ( *a2 <= 0 )
    goto LABEL_18;
  v20 = 0;
  v6 = v4;
  v19 = v4;
  while ( 1 )
  {
    v7 = (_DWORD *)(v20 + v3[1]);
    if ( v21 )
      goto LABEL_15;
    qmemcpy(v7, v6, 0x40u);
    v8 = sub_48DDD0(a3);
    v9 = (unsigned __int16)*v19;
    v16 = v8;
    if ( !v8 || (unsigned int)(v9 - 1) > 0xFF )
    {
      v21 = 3;
LABEL_15:
      v7[2] = 0;
      goto LABEL_16;
    }
    v7[2] = operator new[](196 * v9);
    if ( v9 > 0 )
    {
      v10 = 0;
      v11 = v9;
      do
      {
        v12 = (const void *)(v10 + v16);
        v13 = (void *)(v10 + v7[2]);
        v10 += 196;
        --v11;
        qmemcpy(v13, v12, 0xC4u);
      }
      while ( v11 );
    }
LABEL_16:
    v20 += 64;
    v6 = v19 + 16;
    ++v17;
    v19 += 16;
    if ( v17 >= *a2 )
      break;
    v3 = v18;
  }
  v3 = v18;
  if ( !v21 )
  {
LABEL_18:
    *a1 = v3;
    return v21;
  }
  v15 = v21;
LABEL_21:
  sub_46C980(v3);
  result = v15;
  *a1 = 0;
  return result;
}

// ===== sub_46CB60 @ 0x0046CB60..0x0046CC00 =====
int __fastcall sub_46CB60(int a1, int a2, int *a3, int a4)
{
  int v4; // eax
  int v5; // edi
  int result; // eax
  int *v7; // esi
  int v8; // eax
  int *v9; // [esp+4h] [ebp-4h] BYREF

  v4 = sub_46C610(a1, a2);
  v5 = v4;
  if ( !v4 )
    return 1;
  if ( sub_4484B0(v4) != 1 )
    return 4;
  result = sub_46C9D0(&v9, a3, a4);
  if ( !result )
  {
    v7 = v9;
    v8 = sub_44A900(v5, v9);
    if ( v8 )
    {
      if ( v8 == -2147483647 )
      {
        sub_46C980(v7);
        return 2;
      }
      if ( v8 == -2147483646 )
      {
        sub_46C980(v7);
        return 3;
      }
    }
    sub_46C980(v7);
    return 0;
  }
  return result;
}

// ===== sub_46CC00 @ 0x0046CC00..0x0046CC1C =====
BOOL __usercall sub_46CC00@<eax>(int a1@<edx>, int a2@<ecx>, _DWORD *a3@<esi>)
{
  _DWORD *v3; // eax
  BOOL v4; // edi

  v3 = (_DWORD *)sub_46C610(a2, a1);
  v4 = v3 != 0;
  if ( v3 )
    sub_4484C0(v3, a3);
  return v4;
}

// ===== sub_46CC20 @ 0x0046CC20..0x0046CC41 =====
BOOL __fastcall sub_46CC20(int a1, int a2, int *a3)
{
  int v3; // eax
  BOOL v4; // ecx

  v3 = sub_46C610(a1, a2);
  v4 = v3 != 0;
  if ( v3 )
    *a3 = sub_448520(v3);
  return v4;
}

// ===== sub_46CC50 @ 0x0046CC50..0x0046CC6E =====
BOOL __usercall sub_46CC50@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int v3; // eax
  BOOL v4; // edi

  v3 = sub_46C610(a2, a1);
  v4 = v3 != 0;
  if ( v3 )
    sub_448530(v3, a3);
  return v4;
}

// ===== sub_46CC70 @ 0x0046CC70..0x0046CC97 =====
BOOL __fastcall sub_46CC70(int a1, int a2, _DWORD *a3)
{
  int v3; // eax
  BOOL v4; // esi

  v3 = sub_46C610(a1, a2);
  v4 = v3 != 0;
  if ( v3 )
    sub_448560(a3, v3);
  return v4;
}

// ===== sub_46CCA0 @ 0x0046CCA0..0x0046CD0D =====
int __fastcall sub_46CCA0(int a1, int a2, int a3, int a4, int a5)
{
  int v5; // eax
  int *v6; // ecx
  int v7; // eax

  v5 = sub_46C610(a1, a2);
  if ( !v5 )
    return 1;
  if ( sub_4484B0(v5) != 1 )
    return 4;
  v7 = sub_44B460(a4, a3, v6, a3, a5);
  switch ( v7 )
  {
    case 0:
      return 0;
    case -2147483647:
      return 2;
    case -2147483646:
      return 3;
  }
  return a1;
}

// ===== sub_46CD10 @ 0x0046CD10..0x0046CDA4 =====
int __cdecl sub_46CD10(int a1, int *a2, int a3)
{
  _DWORD *v3; // ebx
  int result; // eax
  void *v5; // esi
  int v6; // eax
  int v7; // [esp+4h] [ebp-4h] BYREF

  v3 = (_DWORD *)sub_4406C0(a1, (int)dword_56674C);
  if ( !v3 )
    return 1;
  result = sub_46C750(&v7, a2, a3);
  if ( !result )
  {
    v5 = (void *)v7;
    v6 = sub_448320(v3, (int *)v7);
    if ( v6 )
    {
      if ( v6 == -2147483647 )
      {
        sub_46C700(v5);
        return 2;
      }
      if ( v6 == -2147483646 )
      {
        sub_46C700(v5);
        return 3;
      }
    }
    sub_46C700(v5);
    return 0;
  }
  return result;
}

// ===== sub_46CDB0 @ 0x0046CDB0..0x0046CE45 =====
int __cdecl sub_46CDB0(int a1, int *a2, int a3)
{
  int result; // eax
  void *v4; // esi
  int v5; // eax
  int v6; // [esp+4h] [ebp-4h] BYREF

  if ( !sub_4406C0(a1, (int)dword_56674C) )
    return 1;
  result = sub_46C9D0(&v6, a2, a3);
  if ( !result )
  {
    v4 = (void *)v6;
    v5 = sub_44B260((_DWORD *)v6);
    if ( v5 )
    {
      if ( v5 == -2147483647 )
      {
        sub_46C980(v4);
        return 2;
      }
      if ( v5 == -2147483646 )
      {
        sub_46C980(v4);
        return 3;
      }
    }
    sub_46C980(v4);
    return 0;
  }
  return result;
}

// ===== sub_46CE50 @ 0x0046CE50..0x0046D16B =====
int sub_46CE50()
{
  int v1; // esi
  int v2; // esi
  int v3; // ecx
  int v4; // edi
  int v5; // esi
  int v6; // esi
  int v7; // eax
  HANDLE ImageA; // eax
  int v9; // eax
  int v10; // [esp-10h] [ebp-470h]
  int v11; // [esp-8h] [ebp-468h]
  int SystemMetrics; // [esp+Ch] [ebp-454h]
  int v13; // [esp+10h] [ebp-450h]
  int nWidth; // [esp+14h] [ebp-44Ch]
  WNDCLASSEXA v15; // [esp+18h] [ebp-448h] BYREF
  int v16; // [esp+48h] [ebp-418h]
  int nHeight; // [esp+4Ch] [ebp-414h]
  _DWORD v18[2]; // [esp+50h] [ebp-410h] BYREF
  CHAR WindowName[1028]; // [esp+58h] [ebp-408h] BYREF

  v15.cbSize = 48;
  v15.style = 40;
  v15.lpfnWndProc = (WNDPROC)sub_498DC0;
  v15.cbClsExtra = 0;
  v15.cbWndExtra = 0;
  v15.hInstance = hInst;
  v15.hIcon = LoadIconA(hInst, (LPCSTR)0x65);
  v15.hIconSm = 0;
  v15.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
  v15.hbrBackground = 0;
  v15.lpszMenuName = 0;
  v15.lpszClassName = aBgiMainWindow;
  if ( !RegisterClassExA(&v15) )
  {
    sub_464500((int)&unk_4E70E8);
    return 0;
  }
  v15.cbSize = 48;
  v15.style = 12320;
  v15.lpfnWndProc = sub_430030;
  v15.cbClsExtra = 0;
  v15.cbWndExtra = 0;
  v15.hInstance = hInst;
  v15.hIcon = LoadIconA(hInst, (LPCSTR)0x65);
  v15.hIconSm = 0;
  v15.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
  v15.hbrBackground = 0;
  v15.lpszMenuName = 0;
  v15.lpszClassName = ClassName;
  if ( !RegisterClassExA(&v15) )
  {
    sub_464500((int)&unk_4E7114);
    return 0;
  }
  dword_517F14 = 2 * GetSystemMetrics(7);
  v1 = 2 * GetSystemMetrics(8);
  dword_517B04 = v1 + GetSystemMetrics(4);
  v2 = sub_461090(2);
  v13 = sub_4610B0(2);
  nWidth = v2 + dword_517F14;
  nHeight = v13 + v3;
  SystemMetrics = GetSystemMetrics(0);
  v16 = GetSystemMetrics(1);
  if ( 4 * SystemMetrics / v16 >= 10 )
    SystemMetrics >>= 1;
  sub_45E740(v2, v13);
  if ( dword_5666DC )
  {
    strcpy(WindowName, "Buriko General Interpreter in Launcher mode");
    goto LABEL_9;
  }
  strcpy(WindowName, "Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )");
  if ( !sub_46BA20(v18) )
  {
LABEL_9:
    v4 = (SystemMetrics - nWidth) / 2;
    v5 = (v16 - v13) / 2 - GetSystemMetrics(4);
    v6 = v5 - GetSystemMetrics(8);
    goto LABEL_12;
  }
  v6 = v18[1];
  v4 = v18[0];
LABEL_12:
  hWndParent = CreateWindowExA(
                 0x40000u,
                 aBgiMainWindow,
                 WindowName,
                 0x80CA0000,
                 v4,
                 v6,
                 nWidth,
                 nHeight,
                 0,
                 0,
                 hInst,
                 0);
  if ( hWndParent )
  {
    v11 = GetSystemMetrics(50);
    v7 = GetSystemMetrics(49);
    ImageA = LoadImageA(hInst, (LPCSTR)0x65, 1u, v7, v11, 0x8000u);
    SendMessageA(hWndParent, 0x80u, 0, (LPARAM)ImageA);
    v10 = 8 * GetSystemMetrics(1);
    v9 = GetSystemMetrics(0);
    SetWindowPos(hWndParent, (HWND)0xFFFFFFFE, 8 * v9, v10, 0, 0, 0x559u);
    SetWindowPos(hWndParent, (HWND)0xFFFFFFFE, 0, 0, 0, 0, 0x59Bu);
    return 1;
  }
  else
  {
    sub_464500((int)&unk_4E7170);
    return 0;
  }
}

// ===== sub_46D170 @ 0x0046D170..0x0046D1BC =====
BOOL sub_46D170()
{
  hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
  dword_5666C8 = (int)LoadCursorA(hInst, (LPCSTR)0x6A);
  dword_5666CC = 0;
  dword_5666D0 = 0;
  dword_5666D4 = 0;
  dword_5666D8 = 0;
  return hCursor != 0;
}

// ===== sub_46D1C0 @ 0x0046D1C0..0x0046D1E2 =====
int sub_46D1C0()
{
  sub_46BB90(4u);
  dword_566760 = operator new[](0x100000u);
  return 1;
}

// ===== sub_46D1F0 @ 0x0046D1F0..0x0046D3CC =====
void *sub_46D1F0()
{
  void *v0; // eax
  int v1; // edi
  int v2; // eax
  int *v3; // esi
  void *v4; // eax
  int *v5; // eax
  int v6; // ebx
  int v7; // esi
  _DWORD *v8; // eax
  _DWORD *v9; // eax
  void *v10; // eax
  int v11; // eax
  unsigned int v13; // [esp+14h] [ebp-10h] BYREF
  int v14; // [esp+20h] [ebp-4h]

  v13 = 1;
  sub_46F2B0(&v13, 9);
  sub_446460();
  v0 = operator new(0x40u);
  v1 = 0;
  v14 = 0;
  if ( v0 )
    v2 = sub_4461E0((int)v0, v13);
  else
    v2 = 0;
  v14 = -1;
  dword_5666F8 = v2;
  sub_44DDA0();
  v3 = (int *)operator new(0xA40u);
  v14 = 1;
  if ( v3 )
  {
    v4 = (void *)sub_46F5D0();
    v5 = sub_442850(v3, v4);
  }
  else
  {
    v5 = 0;
  }
  v14 = -1;
  dword_56674C = v5;
  sub_41A650();
  sub_431990();
  sub_447790();
  v6 = sub_46E990();
  v7 = sub_46E980();
  if ( v7 || dword_518920 == 1 && dword_518924 == 6 )
    v1 = 1;
  sub_49B020();
  sub_49B030();
  sub_49B270();
  sub_407770();
  sub_407A90(v1);
  sub_407AA0(v7);
  sub_407AB0(v6);
  sub_407AC0(dword_518920, dword_518924, dword_518928);
  sub_408740(32769, (int)hWndParent);
  v8 = operator new(0xDCu);
  v14 = 2;
  if ( v8 )
    v9 = sub_4075E0(v8);
  else
    v9 = 0;
  v14 = -1;
  dword_566750 = v9;
  sub_442C90();
  sub_41A660();
  sub_4319A0();
  sub_4477A0();
  sub_44DD90();
  sub_44D870();
  v10 = operator new(0x33Cu);
  v14 = 3;
  if ( v10 )
    v11 = sub_406720((int)v10);
  else
    v11 = 0;
  v14 = -1;
  dword_566754 = v11;
  return sub_432DD0();
}

// ===== sub_46D3D0 @ 0x0046D3D0..0x0046D400 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x0046D3D0

// ===== sub_46D400 @ 0x0046D400..0x0046D471 =====
_DWORD *sub_46D400()
{
  _DWORD *v0; // eax
  _DWORD *result; // eax

  memset(dword_566760, 0, 0x100000u);
  v0 = operator new(8u);
  if ( v0 )
    result = sub_446AC0(v0);
  else
    result = 0;
  dword_56676C = (size_t)result;
  return result;
}

// ===== sub_46D480 @ 0x0046D480..0x0046D53E =====
BOOL sub_46D480()
{
  BOOL v0; // esi
  void *v1; // ecx
  int v2; // ecx
  int v3; // eax

  sub_4978C0();
  sub_46BB30();
  sub_4986D0();
  sub_493A50();
  v0 = sub_46CE50() != 0;
  sub_46D1F0();
  if ( !sub_46D170() )
    v0 = 0;
  if ( !sub_460040() )
    v0 = 0;
  sub_460A00(v1);
  if ( !sub_46D1C0() )
    v0 = 0;
  sub_46D400();
  if ( !sub_46D3D0() )
    v0 = 0;
  sub_48DA80();
  sub_498900();
  sub_498350();
  sub_42FAA0();
  sub_490AF0();
  sub_494B50();
  v3 = sub_490AE0(v2);
  sub_405A20(v3);
  sub_468900();
  sub_468B70();
  sub_496470();
  sub_46A210();
  sub_46AAD0();
  sub_495E60();
  sub_4965D0();
  dword_5666B8 = v0;
  return v0;
}

// ===== sub_46D540 @ 0x0046D540..0x0046D546 =====
int __usercall sub_46D540@<eax>(int result@<eax>)
{
  dword_5667EC = result;
  return result;
}

// ===== sub_46D550 @ 0x0046D550..0x0046D556 =====
int sub_46D550()
{
  return dword_5667EC;
}

// ===== sub_46D560 @ 0x0046D560..0x0046D5E1 =====
__int16 __fastcall sub_46D560(int a1)
{
  int v1; // esi
  int v2; // ecx
  int AsyncKeyState; // edi
  int v4; // eax

  v1 = 0;
  if ( a1 == 1 )
  {
    if ( sub_48EE70() == 1 )
      v1 = 2;
  }
  else if ( a1 == 2 && sub_48EE70() == 1 )
  {
    a1 = 0;
  }
  if ( sub_48EBC0(a1) )
  {
    if ( v2 == 1 )
    {
      v2 = 2;
      v1 = v1 != 0;
    }
    else if ( v2 == 2 )
    {
      v2 = 1;
    }
  }
  AsyncKeyState = GetAsyncKeyState(v2);
  if ( v1 )
    AsyncKeyState |= GetAsyncKeyState(v1);
  if ( sub_49A230() || (v4 = sub_46D550()) != 0 )
    LOWORD(v4) = AsyncKeyState;
  return v4;
}

// ===== sub_46D5F0 @ 0x0046D5F0..0x0046D619 =====
void __usercall sub_46D5F0(int a1@<edi>)
{
  _DWORD *v1; // esi
  void *v2; // [esp-4h] [ebp-8h]

  v1 = *(_DWORD **)(a1 + 24);
  while ( v1 )
  {
    v2 = v1;
    v1 = (_DWORD *)v1[6];
    operator delete(v2);
  }
  *(_DWORD *)(a1 + 24) = 0;
}

// ===== sub_46D620 @ 0x0046D620..0x0046D637 =====
void sub_46D620()
{
  sub_46D5F0((int)&unk_5667F0);
  sub_46D5F0((int)&unk_56680C);
}

// ===== sub_46D640 @ 0x0046D640..0x0046D658 =====
int sub_46D640()
{
  sub_46D620();
  sub_46D6C0();
  return sub_46D720(1);
}

// ===== sub_46D660 @ 0x0046D660..0x0046D6B6 =====
unsigned int *__usercall sub_46D660@<eax>(
        unsigned int *a1@<edi>,
        unsigned int *a2@<esi>,
        unsigned int a3,
        unsigned int a4)
{
  unsigned int *result; // eax
  unsigned int *v5; // ecx
  unsigned int *i; // edx

  result = (unsigned int *)operator new(0x1Cu);
  *result = a3;
  result[1] = *a2;
  result[2] = a2[1];
  result[3] = a2[2];
  result[4] = a2[3];
  result[5] = a4;
  v5 = (unsigned int *)a1[6];
  for ( i = a1; v5; v5 = (unsigned int *)v5[6] )
  {
    if ( *v5 <= a3 )
      break;
    i = v5;
  }
  result[6] = (unsigned int)v5;
  i[6] = (unsigned int)result;
  return result;
}

// ===== sub_46D6C0 @ 0x0046D6C0..0x0046D6DA =====
unsigned int *__usercall sub_46D6C0@<eax>(unsigned int a1@<eax>)
{
  return sub_46D660(dword_5667F0, dword_506A4C, a1, 0);
}

// ===== sub_46D6E0 @ 0x0046D6E0..0x0046D715 =====
unsigned int *__thiscall sub_46D6E0(void *this)
{
  unsigned int v1; // eax
  unsigned int v4[4]; // [esp+8h] [ebp-10h] BYREF

  memset(v4, 0, sizeof(v4));
  v1 = (*(int (__fastcall **)(void *))(*(_DWORD *)this + 28))(this);
  return sub_46D660(dword_5667F0, v4, v1, (unsigned int)this);
}

// ===== sub_46D720 @ 0x0046D720..0x0046D751 =====
unsigned int *__cdecl sub_46D720(unsigned int a1)
{
  unsigned int v2[5]; // [esp+8h] [ebp-14h] BYREF

  memset(v2, 0, 16);
  return sub_46D660(dword_56680C, v2, a1, 0);
}

// ===== sub_46D7A0 @ 0x0046D7A0..0x0046D7AA =====
int __usercall sub_46D7A0@<eax>(int a1@<esi>)
{
  unsigned int *v1; // ecx
  int result; // eax
  unsigned int *v3; // edx

  v3 = dword_5667F0;
  v1 = (unsigned int *)dword_5667F0[6];
  result = 0;
  if ( v1 )
  {
    while ( *v1 != a1 )
    {
      v3 = v1;
      v1 = (unsigned int *)v1[6];
      if ( !v1 )
        return result;
    }
    v3[6] = v1[6];
    operator delete(v1);
    return 1;
  }
  return result;
}

// ===== sub_46D7B0 @ 0x0046D7B0..0x0046D7BA =====
int __usercall sub_46D7B0@<eax>(int a1@<esi>)
{
  unsigned int *v1; // ecx
  int result; // eax
  unsigned int *v3; // edx

  v3 = dword_56680C;
  v1 = (unsigned int *)dword_56680C[6];
  result = 0;
  if ( v1 )
  {
    while ( *v1 != a1 )
    {
      v3 = v1;
      v1 = (unsigned int *)v1[6];
      if ( !v1 )
        return result;
    }
    v3[6] = v1[6];
    operator delete(v1);
    return 1;
  }
  return result;
}

// ===== sub_46D7C0 @ 0x0046D7C0..0x0046D7F5 =====
int __usercall sub_46D7C0@<eax>(int a1@<esi>)
{
  unsigned int *v1; // ecx
  int result; // eax
  unsigned int *v3; // edx

  v1 = (unsigned int *)dword_566808;
  result = 0;
  v3 = dword_5667F0;
  if ( dword_566808 )
  {
    while ( v1[5] != a1 )
    {
      v3 = v1;
      v1 = (unsigned int *)v1[6];
      if ( !v1 )
        return result;
    }
    v3[6] = v1[6];
    operator delete(v1);
    return 1;
  }
  return result;
}

// ===== sub_46D800 @ 0x0046D800..0x0046D805 =====
// attributes: thunk
int __usercall sub_46D800@<eax>(int a1@<esi>)
{
  return sub_46D7C0(a1);
}

// ===== sub_46D810 @ 0x0046D810..0x0046D830 =====
int __cdecl sub_46D810(unsigned int a1)
{
  int result; // eax

  result = dword_506A44;
  if ( dword_506A44 )
  {
    if ( dword_566824 )
      return a1 >= *(_DWORD *)dword_566824;
  }
  return result;
}

// ===== sub_46D830 @ 0x0046D830..0x0046D966 =====
int __cdecl sub_46D830(_DWORD *a1)
{
  int result; // eax
  int v2; // edi
  int v3; // ebx
  unsigned int *v4; // esi
  int v5; // eax
  int v6; // eax
  int v7; // [esp+Ch] [ebp-Ch]
  int v8; // [esp+10h] [ebp-8h] BYREF
  int v9; // [esp+14h] [ebp-4h]

  result = 0;
  if ( dword_506A44 )
  {
    sub_48E680(&v8);
    v2 = v8;
    v3 = v9;
    if ( v8 < 0 || v2 >= sub_4610A0() || v3 < 0 || (v7 = 1, v3 >= sub_4610C0()) )
      v7 = 0;
    v4 = (unsigned int *)dword_566808;
    if ( dword_566808 )
    {
      while ( 1 )
      {
        if ( v4[5] )
        {
          if ( !v7 || !(*(int (__thiscall **)(unsigned int))(*(_DWORD *)v4[5] + 8))(v4[5]) )
            goto LABEL_29;
          (*(void (__thiscall **)(unsigned int, _DWORD **))(*(_DWORD *)v4[5] + 36))(v4[5], (_DWORD **)v4 + 1);
        }
        if ( *v4 <= (unsigned int)a1 )
        {
          if ( (_DWORD *)*v4 != a1 )
            return 0;
          if ( (int)v4[1] > v2 )
            goto LABEL_29;
          if ( v2 > (int)v4[3] )
            goto LABEL_29;
          v6 = v4[2];
          if ( v6 > v3 || v3 > (int)v4[4] )
            goto LABEL_29;
          if ( !v4[5]
            || (*(int (__thiscall **)(unsigned int, unsigned int, int, int))(*(_DWORD *)v4[5] + 100))(
                 v4[5],
                 v2 - v4[1],
                 v3 - v6,
                 1) )
          {
            return 1;
          }
        }
        else
        {
          if ( (int)v4[1] > v2 )
            goto LABEL_29;
          if ( v2 > (int)v4[3] )
            goto LABEL_29;
          v5 = v4[2];
          if ( v5 > v3 || v3 > (int)v4[4] )
            goto LABEL_29;
          if ( !v4[5]
            || (*(int (__thiscall **)(unsigned int, unsigned int, int, int))(*(_DWORD *)v4[5] + 100))(
                 v4[5],
                 v2 - v4[1],
                 v3 - v5,
                 1) )
          {
            return 0;
          }
        }
        v3 = v9;
LABEL_29:
        v4 = (unsigned int *)v4[6];
        if ( !v4 )
          return 0;
      }
    }
    return 0;
  }
  return result;
}

// ===== sub_46D970 @ 0x0046D970..0x0046D976 =====
int __usercall sub_46D970@<eax>(int result@<eax>)
{
  dword_506A44 = result;
  return result;
}

// ===== sub_46D980 @ 0x0046D980..0x0046D986 =====
int __usercall sub_46D980@<eax>(int result@<eax>)
{
  dword_506A48 = result;
  return result;
}

// ===== sub_46D990 @ 0x0046D990..0x0046D996 =====
int __usercall sub_46D990@<eax>(int result@<eax>)
{
  dword_566828 = result;
  return result;
}

// ===== sub_46D9A0 @ 0x0046D9A0..0x0046D9CD =====
int sub_46D9A0()
{
  int result; // eax
  int v1; // esi

  result = dword_5069F8;
  v1 = 0;
  for ( dword_56682C = 1; result; ++v1 )
  {
    sub_46DB40(result);
    result = dword_5069FC[v1];
  }
  return result;
}

// ===== sub_46D9D0 @ 0x0046D9D0..0x0046DA17 =====
int sub_46D9D0()
{
  int result; // eax

  dword_518CA4 = 0;
  dword_518CA8 = 0;
  dword_518C9C = 0;
  result = 0;
  dword_518CAC = 0;
  dword_518C98 = 0;
  dword_518CA0 = 0;
  qmemcpy(&unk_518CB0, &dword_518C98, 0x17E8u);
  return result;
}

// ===== sub_46DA20 @ 0x0046DA20..0x0046DA3D =====
int *sub_46DA20()
{
  int *result; // eax

  result = &dword_518C9C;
  do
  {
    *(result - 1) = 0;
    *result = 0;
    result[1] = 0;
    result[3] = 0;
    result += 6;
  }
  while ( (int)result < (int)&VersionInformation.dwMajorVersion );
  return result;
}

// ===== sub_46DA40 @ 0x0046DA40..0x0046DA60 =====
int __usercall sub_46DA40@<eax>(int a1@<esi>, int a2)
{
  int result; // eax

  sub_497B40();
  result = dword_518CAC[6 * a1];
  dword_518CAC[6 * a1] = a2;
  return result;
}

// ===== sub_46DA60 @ 0x0046DA60..0x0046DA72 =====
int __usercall sub_46DA60@<eax>(int a1@<esi>)
{
  sub_497B40();
  return dword_518CAC[6 * a1];
}

// ===== sub_46DA80 @ 0x0046DA80..0x0046DAEA =====
BOOL __usercall sub_46DA80@<eax>(int a1@<eax>)
{
  int v2; // eax
  int *v3; // esi
  BOOL v4; // ebx

  sub_497B40();
  v2 = dword_518C98[6 * a1];
  v3 = &dword_518C98[6 * a1];
  v4 = v2 == 0;
  switch ( a1 )
  {
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
      v3[1] = 0;
      goto LABEL_4;
    default:
      if ( !v2 )
      {
LABEL_4:
        ++v3[2];
        ++v3[3];
        v3[4] = sub_498720() + 500;
      }
      *v3 = 1;
      return v4;
  }
}

// ===== sub_46DB00 @ 0x0046DB00..0x0046DB1F =====
int __usercall sub_46DB00@<eax>(int a1@<esi>)
{
  int result; // eax

  sub_497B40();
  result = 6 * a1;
  dword_518C98[result] = 0;
  dword_518C9C[result] = 0;
  return result * 4;
}

// ===== sub_46DB20 @ 0x0046DB20..0x0046DB32 =====
int __usercall sub_46DB20@<eax>(int a1@<esi>)
{
  int result; // eax

  sub_497B40();
  result = 3 * a1;
  ++dword_518CA4[6 * a1];
  return result;
}

// ===== sub_46DB40 @ 0x0046DB40..0x0046DC2B =====
int __cdecl sub_46DB40(int a1)
{
  unsigned int v2; // eax
  int v3; // ebx
  int v4; // eax
  int v5; // ecx
  int *v6; // edi
  int v7; // esi
  int *v8; // eax
  int v9; // eax
  int result; // eax
  int v11; // [esp+Ch] [ebp-4h] BYREF
  int v12; // [esp+18h] [ebp+8h]

  sub_497B40();
  LOWORD(v2) = sub_46D560(a1);
  v3 = 0;
  v4 = (v2 >> 15) & 1;
  if ( dword_518C98[6 * a1] && !v4 )
    sub_46DB00(a1);
  v5 = 0;
  v12 = 0;
  if ( (unsigned int)(a1 - 193) > 0x16 )
  {
    result = dword_518CA0[6 * a1];
    if ( dword_518C98[6 * a1] && !dword_518C9C[6 * a1] )
    {
      result |= 0x80000000;
      dword_518C9C[6 * a1] = 1;
    }
    dword_518CA0[6 * a1] = 0;
  }
  else
  {
    v11 = 0;
    v6 = &v11;
    switch ( a1 )
    {
      case 193:
        v6 = (int *)&unk_5067C0;
        break;
      case 194:
        v6 = (int *)&unk_506800;
        break;
      case 195:
        v6 = (int *)&unk_5069AC;
        break;
      case 196:
        v6 = (int *)&unk_506A38;
        break;
      default:
        break;
    }
    v7 = 0;
    if ( *v6 )
    {
      v8 = v6;
      do
      {
        v9 = sub_46DB40(*v8);
        if ( v9 )
        {
          v12 |= v9 & 0x80000000;
          v3 += v9 & 0x7FFFFFFF;
        }
        v8 = &v6[++v7];
      }
      while ( *v8 );
      v5 = v12;
    }
    return v3 | v5;
  }
  return result;
}

// ===== sub_46DC40 @ 0x0046DC40..0x0046DC64 =====
BOOL __fastcall sub_46DC40(int a1)
{
  BOOL result; // eax

  result = 0;
  if ( dword_518C98[6 * a1] )
    return sub_498720() >= (unsigned int)dword_518CA8[6 * a1];
  return result;
}

// ===== sub_46DC70 @ 0x0046DC70..0x0046DC7B =====
int __usercall sub_46DC70@<eax>(int a1@<eax>)
{
  return dword_518CA4[6 * a1];
}

// ===== sub_46DC80 @ 0x0046DC80..0x0046DE27 =====
int sub_46DC80()
{
  int v0; // edi
  int *v1; // esi
  int v2; // ebx
  int *v3; // eax
  _DWORD v5[20]; // [esp+Ch] [ebp-A8h]
  int v6; // [esp+5Ch] [ebp-58h]
  int v7; // [esp+60h] [ebp-54h]
  _DWORD v8[19]; // [esp+64h] [ebp-50h]

  v0 = 0;
  v1 = (int *)&unk_506740;
  v5[0] = &unk_506740;
  v5[1] = &unk_506780;
  v5[2] = &unk_5067C0;
  v5[3] = &unk_506800;
  v5[4] = &unk_506840;
  v5[5] = &unk_506880;
  v5[6] = &unk_5068C0;
  v5[7] = &unk_506900;
  v5[8] = &unk_506734;
  v5[9] = &unk_506940;
  v5[10] = &unk_50694C;
  v5[11] = &unk_506958;
  v5[12] = &unk_506964;
  v5[13] = &unk_506970;
  v5[14] = &unk_50697C;
  v5[15] = &unk_506988;
  v5[16] = &unk_506994;
  v5[17] = &unk_5069A0;
  v5[18] = &unk_5069B8;
  v5[19] = 0;
  v8[0] = 64;
  v8[1] = 128;
  v8[2] = 256;
  v8[3] = 512;
  v8[4] = 4096;
  v8[5] = 0x2000;
  v8[6] = 0x4000;
  v8[7] = 0x8000;
  v8[8] = 0x10000;
  v8[9] = 0x20000;
  v8[10] = 0x40000;
  v8[11] = 0x80000;
  v8[12] = 0x100000;
  v8[13] = 0x200000;
  v8[14] = 0x400000;
  v8[15] = 0x800000;
  v8[16] = 0x1000000;
  v8[17] = 0x2000000;
  v8[18] = 0x40000000;
  v7 = 0;
  v6 = 0;
  v2 = 0;
  do
  {
    if ( *v1 )
    {
      v3 = v1;
      while ( !sub_46DB40(*v3) )
      {
        v3 = &v1[++v0];
        if ( !*v3 )
          goto LABEL_8;
      }
      v7 |= v8[v2];
    }
LABEL_8:
    v2 = v6 + 1;
    v1 = (int *)v5[v2];
    v0 = 0;
    ++v6;
  }
  while ( v1 );
  return v7;
}

// ===== sub_46DE30 @ 0x0046DE30..0x0046DEFA =====
int sub_46DE30()
{
  int v0; // edi
  int v1; // ebx
  int *v2; // esi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-4h]

  v5 = dword_566828;
  v0 = 0;
  if ( sub_49A230() )
  {
    if ( dword_506A44 )
    {
      v1 = 0;
      if ( dword_5069F8[0] )
      {
        v2 = dword_5069F8;
        do
        {
          v3 = sub_46D560(*v2) & 0x8000;
          if ( v0 || v3 )
            v0 = 1;
          if ( dword_56682C )
          {
            if ( sub_46DB40(*v2) && dword_5666EC )
              v5 = 1;
          }
          else if ( v3 && dword_5666EC )
          {
            goto LABEL_19;
          }
          v2 = &dword_5069F8[++v1];
        }
        while ( *v2 );
      }
    }
  }
  if ( dword_56682C )
  {
    if ( !v0 )
      dword_56682C = 0;
  }
  if ( v5 )
  {
LABEL_19:
    if ( dword_506A48 )
      return 1;
  }
  return 0;
}

// ===== sub_46DF00 @ 0x0046DF00..0x0046DF93 =====
int __usercall sub_46DF00@<eax>(unsigned int a1@<eax>, _DWORD *a2)
{
  int v2; // esi

  v2 = 0;
  if ( sub_46D810(a1) )
  {
    v2 = sub_46DC80();
    if ( sub_46DE30() )
      v2 |= 0x80000000;
  }
  if ( sub_46D830(a2) )
  {
    if ( sub_46DB40(1) )
      v2 |= 1u;
    if ( sub_46DB40(2) )
      v2 |= 2u;
    if ( sub_46DB40(4) )
      v2 |= 4u;
    if ( sub_46DB40(5) )
      v2 |= 0x10u;
    if ( sub_46DB40(6) )
      return v2 | 0x20;
  }
  return v2;
}

// ===== sub_46DFA0 @ 0x0046DFA0..0x0046E06B =====
int __usercall sub_46DFA0@<eax>(unsigned int a1@<eax>, char *a2@<ecx>)
{
  int v2; // edx
  unsigned int v3; // eax
  int *v4; // eax
  int v5; // edx
  int v6; // ecx

  v2 = 0;
  if ( *(_DWORD *)a2 )
  {
    do
      ++v2;
    while ( *(_DWORD *)&a2[4 * v2] );
    if ( v2 >= 16 )
      return -2147483646;
  }
  if ( a1 > 0x2000 )
  {
    if ( a1 > 0x40000000 )
    {
      if ( a1 == 0x80000000 )
      {
        v4 = dword_5069F8;
        goto LABEL_27;
      }
    }
    else
    {
      switch ( a1 )
      {
        case 0x40000000u:
          v4 = (int *)&unk_5069B8;
          goto LABEL_27;
        case 0x4000u:
          v4 = (int *)&unk_5068C0;
          goto LABEL_27;
        case 0x8000u:
          v4 = (int *)&unk_506900;
          goto LABEL_27;
      }
    }
    return -2147483647;
  }
  if ( a1 == 0x2000 )
  {
    v4 = (int *)&unk_506880;
    goto LABEL_27;
  }
  if ( a1 > 0x100 )
  {
    if ( a1 == 512 )
    {
      v4 = (int *)&unk_506800;
      goto LABEL_27;
    }
    if ( a1 == 4096 )
    {
      v4 = (int *)&unk_506840;
      goto LABEL_27;
    }
    return -2147483647;
  }
  if ( a1 == 256 )
  {
    v4 = (int *)&unk_5067C0;
    goto LABEL_27;
  }
  v3 = a1 - 64;
  if ( !v3 )
  {
    v4 = (int *)&unk_506740;
    goto LABEL_27;
  }
  if ( v3 != 64 )
    return -2147483647;
  v4 = (int *)&unk_506780;
LABEL_27:
  v5 = v2 + 1;
  if ( v5 > 0 )
  {
    v6 = a2 - (char *)v4;
    do
    {
      *v4 = *(int *)((char *)v4 + v6);
      ++v4;
      --v5;
    }
    while ( v5 );
  }
  return 0;
}

// ===== sub_46E070 @ 0x0046E070..0x0046E265 =====
int __cdecl sub_46E070(int a1)
{
  int v1; // eax
  int v2; // ecx
  int v3; // esi
  int v4; // edi
  int i; // eax
  int v6; // ecx
  int v7; // edx
  _DWORD v9[24]; // [esp+Ch] [ebp-C8h]
  _DWORD v10[25]; // [esp+6Ch] [ebp-68h]

  v1 = 0;
  v2 = 1;
  v3 = 0;
  v10[0] = 1;
  v10[1] = 2;
  v10[2] = 4;
  v10[3] = 16;
  v10[4] = 32;
  v10[5] = 64;
  v10[6] = 128;
  v10[7] = 256;
  v10[8] = 512;
  v10[9] = 4096;
  v10[10] = 0x2000;
  v10[11] = 0x4000;
  v10[12] = 0x8000;
  v10[13] = 0x10000;
  v10[14] = 0x20000;
  v10[15] = 0x40000;
  v10[16] = 0x80000;
  v10[17] = 0x100000;
  v10[18] = 0x200000;
  v10[19] = 0x400000;
  v10[20] = 0x800000;
  v10[21] = 0x1000000;
  v10[22] = 0x2000000;
  v10[23] = 0x40000000;
  v10[24] = 0;
  v9[0] = &unk_50670C;
  v9[1] = &unk_506714;
  v9[2] = &unk_50671C;
  v9[3] = &unk_506724;
  v9[4] = &unk_50672C;
  v9[5] = &unk_506740;
  v9[6] = &unk_506780;
  v9[7] = &unk_5067C0;
  v9[8] = &unk_506800;
  v9[9] = &unk_506840;
  v9[10] = &unk_506880;
  v9[11] = &unk_5068C0;
  v9[12] = &unk_506900;
  v9[13] = &unk_506734;
  v9[14] = &unk_506940;
  v9[15] = &unk_50694C;
  v9[16] = &unk_506958;
  v9[17] = &unk_506964;
  v9[18] = &unk_506970;
  v9[19] = &unk_50697C;
  v9[20] = &unk_506988;
  v9[21] = &unk_506994;
  v9[22] = &unk_5069A0;
  v9[23] = &unk_5069B8;
  v4 = 0;
  do
  {
    if ( (v2 & a1) != 0 )
    {
      for ( i = *(_DWORD *)v9[v1]; i; i = *(_DWORD *)(v7 + 4 * (v6 + 1)) )
        v3 += sub_46DC70(i);
    }
    v1 = ++v4;
    v2 = v10[v4];
  }
  while ( v2 );
  return v3;
}

// ===== sub_46E270 @ 0x0046E270..0x0046E35B =====
int __fastcall sub_46E270(unsigned int a1, int a2)
{
  int result; // eax

  result = 0;
  switch ( a2 )
  {
    case 1:
      goto LABEL_25;
    case 2:
      result = (a1 >> 1) & 1;
      break;
    case 4:
      result = (a1 >> 2) & 1;
      break;
    case 5:
      result = (a1 >> 4) & 1;
      break;
    case 6:
      result = (a1 >> 5) & 1;
      break;
    case 9:
      a1 >>= 30;
LABEL_25:
      result = a1 & 1;
      break;
    case 14:
      result = (a1 >> 6) & 1;
      break;
    case 15:
      result = (a1 >> 7) & 1;
      break;
    case 37:
      result = (a1 >> 14) & 1;
      break;
    case 38:
      result = (a1 >> 12) & 1;
      break;
    case 39:
      result = (a1 >> 15) & 1;
      break;
    case 40:
      result = (a1 >> 13) & 1;
      break;
    case 48:
    case 96:
      result = (a1 >> 25) & 1;
      break;
    case 49:
    case 97:
      result = HIWORD(a1) & 1;
      break;
    case 50:
    case 98:
      result = (a1 >> 17) & 1;
      break;
    case 51:
    case 99:
      result = (a1 >> 18) & 1;
      break;
    case 52:
    case 100:
      result = (a1 >> 19) & 1;
      break;
    case 53:
    case 101:
      result = (a1 >> 20) & 1;
      break;
    case 54:
    case 102:
      result = (a1 >> 21) & 1;
      break;
    case 55:
    case 103:
      result = (a1 >> 22) & 1;
      break;
    case 56:
    case 104:
      result = (a1 >> 23) & 1;
      break;
    case 57:
    case 105:
      result = HIBYTE(a1) & 1;
      break;
    case 193:
      result = (a1 >> 8) & 1;
      break;
    case 194:
      result = (a1 >> 9) & 1;
      break;
    default:
      return result;
  }
  return result;
}

// ===== sub_46E490 @ 0x0046E490..0x0046E4F9 =====
int __usercall sub_46E490@<eax>(_DWORD *a1@<eax>)
{
  int v1; // esi

  v1 = 0;
  if ( sub_46D830(a1) )
  {
    v1 = sub_46DC40(1);
    if ( sub_46DC40(2) )
      v1 |= 2u;
    if ( sub_46DC40(4) )
      v1 |= 4u;
    if ( sub_46DC40(5) )
      v1 |= 0x10u;
    if ( sub_46DC40(6) )
      return v1 | 0x20;
  }
  return v1;
}

// ===== sub_46E500 @ 0x0046E500..0x0046E543 =====
_DWORD *__cdecl sub_46E500(int a1, unsigned int a2)
{
  _DWORD *v2; // esi
  _DWORD *v3; // edi
  _DWORD *result; // eax

  v2 = dword_566838;
  v3 = &unk_566830;
  if ( dword_566838 )
  {
    do
    {
      if ( v2[1] < a2 )
        break;
      v3 = v2;
      v2 = (_DWORD *)v2[2];
    }
    while ( v2 );
  }
  result = operator new(0xCu);
  result[1] = a2;
  result[2] = v2;
  *result = a1;
  v3[2] = result;
  return result;
}

// ===== sub_46E550 @ 0x0046E550..0x0046E57C =====
void __fastcall sub_46E550(int a1, int a2)
{
  _DWORD *v2; // eax
  _DWORD *v3; // ecx

  v2 = dword_566838;
  v3 = &unk_566830;
  if ( dword_566838 )
  {
    while ( *v2 != a2 )
    {
      v3 = v2;
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return;
    }
    v3[2] = v2[2];
    operator delete(v2);
  }
}

// ===== sub_46E580 @ 0x0046E580..0x0046E59D =====
BOOL __cdecl sub_46E580(unsigned int a1)
{
  BOOL result; // eax

  result = 1;
  if ( dword_566838 )
    return a1 >= *((_DWORD *)dword_566838 + 1);
  return result;
}

// ===== sub_46E5A0 @ 0x0046E5A0..0x0046E5A5 =====
// attributes: thunk
int __usercall sub_46E5A0@<eax>(int result@<eax>)
{
  return sub_4319F0(result);
}

// ===== sub_46E5B0 @ 0x0046E5B0..0x0046E5B7 =====
void sub_46E5B0()
{
  ++dword_56683C;
}

// ===== sub_46E5C0 @ 0x0046E5C0..0x0046E5C6 =====
int sub_46E5C0()
{
  return dword_56683C;
}

// ===== sub_46E5D0 @ 0x0046E5D0..0x0046E877 =====
int __cdecl sub_46E5D0(int a1)
{
  int v1; // edi
  WPARAM v2; // esi
  int v3; // ebx
  int v4; // eax
  int result; // eax
  struct tagPOINT Point; // [esp+Ch] [ebp-48h] BYREF
  _DWORD v7[8]; // [esp+14h] [ebp-40h]
  _DWORD v8[7]; // [esp+34h] [ebp-20h]

  v1 = 0;
  v7[1] = 1;
  v8[1] = 1;
  v7[2] = 4;
  v8[4] = 4;
  v2 = 0;
  v7[0] = 17;
  v7[3] = 2;
  v7[4] = 16;
  v7[5] = 5;
  v7[6] = 6;
  v7[7] = 0;
  v8[0] = 8;
  v8[2] = 16;
  v8[3] = 2;
  v8[5] = 32;
  v8[6] = 64;
  v3 = 0;
  v4 = 17;
  do
  {
    if ( GetAsyncKeyState(v4) < 0 )
      v2 |= v8[v1];
    v1 = ++v3;
    v4 = v7[v3];
  }
  while ( v4 );
  GetCursorPos(&Point);
  switch ( a1 )
  {
    case 1:
      SendMessageA(hWndParent, 0x201u, v2 | 1, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      SendMessageA(hWndParent, 0x202u, v2, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      result = 1;
      break;
    case 2:
      SendMessageA(hWndParent, 0x204u, v2 | 2, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      SendMessageA(hWndParent, 0x205u, v2, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      result = 1;
      break;
    case 4:
      SendMessageA(hWndParent, 0x207u, v2 | 0x10, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      SendMessageA(hWndParent, 0x208u, v2, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      result = 1;
      break;
    case 5:
      SendMessageA(hWndParent, 0x20Bu, (unsigned __int16)v2 | 0x10020, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      SendMessageA(hWndParent, 0x20Cu, (unsigned __int16)v2 | 0x10000, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      result = 1;
      break;
    case 6:
      SendMessageA(hWndParent, 0x20Bu, (unsigned __int16)v2 | 0x20040, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      SendMessageA(hWndParent, 0x20Cu, (unsigned __int16)v2 | 0x20000, LOWORD(Point.x) | (LOWORD(Point.y) << 16));
      result = 1;
      break;
    default:
      result = 0;
      break;
  }
  return result;
}

// ===== sub_46E890 @ 0x0046E890..0x0046E92E =====
int __cdecl sub_46E890(_DWORD *a1, unsigned int a2)
{
  unsigned int v13; // [esp+Ch] [ebp-14h]

  _EAX = a2 < 0x80000000 ? 0 : 0x80000000;
  __asm { cpuid }
  v13 = _EAX;
  _EAX = a2;
  if ( v13 < a2 )
    return 0;
  __asm { cpuid }
  *a1 = _EAX;
  a1[1] = _EBX;
  a1[2] = _ECX;
  a1[3] = _EDX;
  return 1;
}

// ===== sub_46E930 @ 0x0046E930..0x0046E96B =====
BOOL __cdecl sub_46E930(char a1)
{
  _DWORD v2[4]; // [esp+4h] [ebp-10h] BYREF

  return sub_46E890(v2, 1u) && (v2[3] & (1 << a1)) != 0;
}

// ===== sub_46E970 @ 0x0046E970..0x0046E97B =====
BOOL sub_46E970()
{
  return sub_46E930(15);
}

// ===== sub_46E980 @ 0x0046E980..0x0046E98B =====
BOOL sub_46E980()
{
  return sub_46E930(25);
}

// ===== sub_46E990 @ 0x0046E990..0x0046E99B =====
BOOL sub_46E990()
{
  return sub_46E930(26);
}

// ===== sub_46E9A0 @ 0x0046E9A0..0x0046E9AB =====
BOOL sub_46E9A0()
{
  return sub_46E930(28);
}

// ===== sub_46E9B0 @ 0x0046E9B0..0x0046EAEE =====
int __usercall sub_46E9B0@<eax>(_DWORD *a1@<edi>, int *a2, unsigned int *a3, int *a4, _DWORD *a5)
{
  int v5; // esi
  const char *v6; // ecx
  unsigned int v7; // ecx
  unsigned int v8; // eax
  int v9; // edx
  _DWORD v11[4]; // [esp+8h] [ebp-30h]
  unsigned int v12; // [esp+18h] [ebp-20h] BYREF
  int v13; // [esp+1Ch] [ebp-1Ch]
  int v14; // [esp+20h] [ebp-18h]
  int v15; // [esp+24h] [ebp-14h]
  _DWORD v16[3]; // [esp+28h] [ebp-10h] BYREF
  char v17; // [esp+34h] [ebp-4h]

  v5 = 0;
  if ( !sub_46E890(&v12, 0) )
    return 0;
  v16[0] = v13;
  v6 = "GenuineIntel";
  v16[1] = v15;
  v16[2] = v14;
  v17 = 0;
  v11[0] = "AuthenticAMD";
  v11[1] = "CentaurHauls";
  v11[2] = "GenuineTMx86";
  v11[3] = 0;
  do
  {
    if ( !strcmp((const char *)v16, v6) )
      break;
    v6 = (const char *)v11[v5++];
  }
  while ( v6 );
  *a1 = v5;
  if ( !sub_46E890(&v12, 1u) )
    return 0;
  v7 = v12;
  v8 = v12 >> 8;
  v9 = BYTE1(v12) & 0xF;
  *a2 = v9;
  if ( !v9 || v9 == 15 )
    *a2 = v9 | HIWORD(v7) & 0xFF0;
  *a3 = (v7 & 0xF0 | v8 & 0xF00) >> 4;
  *a4 = v7 & 0xF;
  if ( !*a1 )
  {
    *a5 = (unsigned __int8)v13;
    return 1;
  }
  if ( !sub_46E890(&v12, 0x80000001) )
    return 0;
  *a5 = (unsigned __int16)v13;
  return 1;
}

// ===== sub_46EAF0 @ 0x0046EAF0..0x0046EB14 =====
unsigned __int64 sub_46EAF0()
{
  return __rdtsc();
}

// ===== sub_46EB20 @ 0x0046EB20..0x0046EB99 =====
int sub_46EB20()
{
  HANDLE CurrentThread; // edi
  int v1; // esi
  unsigned __int64 v2; // kr00_8
  unsigned int v3; // esi
  int v4; // esi
  DWORD_PTR dwThreadAffinityMask; // [esp+18h] [ebp-4h]

  CurrentThread = GetCurrentThread();
  dwThreadAffinityMask = SetThreadAffinityMask(CurrentThread, 1u);
  v1 = sub_498720();
  while ( v1 == sub_498720() )
    ;
  v2 = sub_46EAF0();
  v3 = sub_498720() + 1000;
  while ( v3 > sub_498720() )
    ;
  v4 = (__int64)(sub_46EAF0() - v2) / 1000000;
  SetThreadAffinityMask(CurrentThread, dwThreadAffinityMask);
  return v4;
}

// ===== sub_46EBA0 @ 0x0046EBA0..0x0046F0DB =====
int __fastcall sub_46EBA0(
        int *a1,
        _DWORD *a2,
        _DWORD *a3,
        int *a4,
        unsigned int *a5,
        unsigned int *a6,
        unsigned int *a7,
        unsigned int *a8,
        int *a9,
        DWORD *a10)
{
  unsigned int *v10; // ebx
  int *v11; // esi
  unsigned int *v12; // edx
  int v13; // edi
  void *v14; // eax
  _DWORD *v15; // esi
  unsigned int i; // eax
  unsigned int v17; // eax
  unsigned int v18; // edx
  int v19; // eax
  unsigned int *v20; // eax
  unsigned int k; // edi
  int v22; // eax
  unsigned int v23; // eax
  int v24; // eax
  struct _SYSTEM_INFO SystemInfo; // [esp+Ch] [ebp-5Ch] BYREF
  int *v27; // [esp+30h] [ebp-38h]
  DWORD *v28; // [esp+34h] [ebp-34h]
  unsigned int v29; // [esp+38h] [ebp-30h]
  int *v30; // [esp+3Ch] [ebp-2Ch]
  unsigned int j; // [esp+40h] [ebp-28h]
  void *v32; // [esp+44h] [ebp-24h]
  unsigned int *v33; // [esp+48h] [ebp-20h]
  unsigned int *v34; // [esp+4Ch] [ebp-1Ch]
  unsigned int *v35; // [esp+50h] [ebp-18h]
  int v36; // [esp+54h] [ebp-14h] BYREF
  unsigned int v37; // [esp+58h] [ebp-10h]
  unsigned int v38; // [esp+5Ch] [ebp-Ch]
  int v39; // [esp+60h] [ebp-8h]

  v10 = a8;
  v34 = a6;
  v35 = a7;
  v30 = a4;
  v27 = a9;
  v28 = a10;
  v33 = a8;
  if ( !sub_46E9B0(a3, a4, a5, a1, a2) )
    return 0;
  v11 = (int *)v34;
  v12 = v35;
  *v34 = 0;
  *v12 = 0;
  *a8 = 0;
  if ( *a3 )
  {
    if ( sub_46E890(&v36, 0x80000005) )
    {
      *v11 = (v38 << 24) | HIBYTE(v38) | v38 & 0xFF0000;
      if ( sub_46E890(&v36, 0x80000006) )
      {
        if ( (unsigned __int16)v38 >> 12 )
          v24 = 1 << ((unsigned __int16)((unsigned __int16)v38 >> 12) >> 1);
        else
          v24 = 0;
        *v35 = __SPAIR64__(v24 | (v38 << 8), v38) >> 16;
      }
    }
  }
  else if ( sub_46E890(&v36, 2u) )
  {
    v13 = (unsigned __int8)v36;
    v29 = 16 * (unsigned __int8)v36;
    v14 = operator new[](v29);
    v32 = v14;
    if ( v13 )
    {
      v15 = v14;
      do
      {
        sub_46E890(&v36, 2u);
        v36 &= 0xFFFFFF00;
        for ( i = 0; i < 4; ++i )
        {
          if ( *(&v36 + i) < 0 )
            *(&v36 + i) = 0;
        }
        v17 = v37;
        *v15 = v36;
        v18 = v38;
        v15[1] = v17;
        v19 = v39;
        v15[2] = v18;
        v15[3] = v19;
        v15 += 4;
        --v13;
      }
      while ( v13 );
    }
    for ( j = 0; j < v29; ++j )
    {
      switch ( *((_BYTE *)v32 + j) )
      {
        case 0xA:
          *v34 = 537001992;
          break;
        case 0xC:
          *v34 = 537133072;
          break;
        case 0xD:
          *v34 = 1074003984;
          break;
        case 0xE:
          *v34 = 1074135064;
          break;
        case 0x21:
          *v35 = 1074266368;
          break;
        case 0x22:
        case 0xD0:
          *v10 = 1074004480;
          break;
        case 0x23:
        case 0xD6:
          *v10 = 1074267136;
          break;
        case 0x25:
        case 0xD7:
          *v10 = 1074268160;
          break;
        case 0x29:
        case 0xD8:
          *v10 = 1074270208;
          break;
        case 0x2C:
          *v34 = 1074266144;
          break;
        case 0x41:
          *v35 = 537133184;
          break;
        case 0x42:
          *v35 = 537133312;
          break;
        case 0x43:
          *v35 = 537133568;
          break;
        case 0x44:
          *v35 = 537134080;
          break;
        case 0x45:
          *v35 = 537135104;
          break;
        case 0x46:
          *v10 = 1074008064;
          break;
        case 0x47:
          *v10 = 1074274304;
          break;
        case 0x48:
          *v35 = 1074531328;
          break;
        case 0x49:
          v20 = v10;
          if ( *v30 != 15 )
            v20 = v35;
          *v20 = 1074794496;
          break;
        case 0x4A:
        case 0xDE:
          *v10 = 1074534400;
          break;
        case 0x4B:
        case 0xE4:
          *v10 = 1074798592;
          break;
        case 0x4C:
          *v10 = 1074540544;
          break;
        case 0x4D:
          *v10 = 1074806784;
          break;
        case 0x4E:
          *v35 = 1075320832;
          break;
        case 0x60:
          *v34 = 1074266128;
          break;
        case 0x66:
          *v34 = 1074003976;
          break;
        case 0x67:
          *v34 = 1074003984;
          break;
        case 0x68:
          *v34 = 1074004000;
          break;
        case 0x78:
          *v35 = 1074004992;
          break;
        case 0x79:
          *v35 = 1074266240;
          break;
        case 0x7A:
          *v35 = 1074266368;
          break;
        case 0x7B:
          *v35 = 1074266624;
          break;
        case 0x7C:
          *v35 = 1074267136;
          break;
        case 0x7D:
          *v35 = 1074268160;
          break;
        case 0x7F:
          *v35 = 1073873408;
          break;
        case 0x80:
          *v35 = 1074266624;
          break;
        case 0x81:
          *v35 = 537395328;
          break;
        case 0x82:
          *v35 = 537395456;
          break;
        case 0x83:
          *v35 = 537395712;
          break;
        case 0x84:
          *v35 = 537396224;
          break;
        case 0x85:
          *v35 = 537397248;
          break;
        case 0x86:
          *v35 = 1074004480;
          break;
        case 0x87:
          *v35 = 1074267136;
          break;
        case 0xD1:
          *v10 = 1074004992;
          break;
        case 0xD2:
          *v10 = 1074006016;
          break;
        case 0xDC:
          *v10 = 1074529792;
          break;
        case 0xDD:
          *v10 = 1074531328;
          break;
        case 0xE2:
          *v10 = 1074792448;
          break;
        case 0xE3:
          *v10 = 1074794496;
          break;
        case 0xEA:
          *v10 = 1075326976;
          break;
        case 0xEB:
          *v10 = 1075333120;
          break;
        case 0xEC:
          *v10 = 1075339264;
          break;
        case 0xFF:
          for ( k = 0; k < 4; ++k )
          {
            if ( sub_46E890(&v36, 4u) )
            {
              v22 = v36 & 0x1F;
              if ( v22 == 1 || v22 == 3 )
              {
                v10 = v33;
                v23 = ((((v37 >> 22) + 1) | (((v37 & 0xFFF) + 1) << 8)) << 16) | ((((v37 & 0xFFF) + 1)
                                                                                 * ((v37 >> 22) + 1)
                                                                                 * (v38 + 1)
                                                                                 * (((v37 >> 12) & 0x3FF) + 1)) >> 10);
                switch ( (unsigned __int8)v36 >> 5 )
                {
                  case 1:
                    *v34 = v23;
                    break;
                  case 2:
                    *v35 = v23;
                    break;
                  case 3:
                    *v33 = v23;
                    break;
                }
              }
            }
          }
          break;
        default:
          continue;
      }
    }
    operator delete[](v32);
  }
  *v27 = sub_46EB20();
  GetSystemInfo(&SystemInfo);
  *v28 = SystemInfo.dwNumberOfProcessors;
  return 1;
}

// ===== sub_46F2B0 @ 0x0046F2B0..0x0046F308 =====
int __fastcall sub_46F2B0(_DWORD *a1, unsigned int a2)
{
  int result; // eax

  result = 0;
  if ( !a1 )
    return sub_46EBA0(
             &dword_51892C,
             &dword_518930,
             dword_518920,
             &dword_518924,
             (unsigned int *)&dword_518928,
             &dword_518934,
             &dword_518938,
             &dword_51893C,
             &dword_518940,
             dword_518944);
  if ( a2 < 0xA )
  {
    *a1 = dword_518920[a2];
    return 1;
  }
  return result;
}

// ===== sub_46F310 @ 0x0046F310..0x0046F377 =====
int sub_46F310()
{
  int v1; // [esp+8h] [ebp-24h] BYREF
  int v2; // [esp+Ch] [ebp-20h]
  int v3; // [esp+18h] [ebp-14h] BYREF
  unsigned int v4; // [esp+1Ch] [ebp-10h] BYREF
  int v5; // [esp+20h] [ebp-Ch] BYREF
  int v6; // [esp+24h] [ebp-8h] BYREF
  int v7; // [esp+28h] [ebp-4h] BYREF

  if ( sub_46E9B0(&v7, &v3, &v4, &v5, &v6) && !v7 && sub_46E9A0() && sub_46E890(&v1, 1u) )
    return BYTE2(v2);
  else
    return 1;
}

// ===== sub_46F380 @ 0x0046F380..0x0046F3BF =====
__int64 __usercall sub_46F380@<edx:eax>(_QWORD *a1@<edi>, int *a2)
{
  int v2; // ebx
  __int64 v4; // [esp+8h] [ebp-Ch]

  v2 = sub_45E480();
  sub_498840();
  if ( a1 )
    *a1 = v4;
  if ( a2 )
    *a2 = v2;
  return v4 / (unsigned int)v2;
}

// ===== sub_46F3C0 @ 0x0046F3C0..0x0046F5CE =====
unsigned int sub_46F3C0()
{
  int v0; // eax
  int v1; // ecx
  unsigned int v2; // esi
  int v3; // eax
  __int64 v4; // rax
  unsigned int v5; // ebx
  unsigned int v6; // edi
  int v7; // ecx
  int v8; // ecx
  unsigned int result; // eax
  _QWORD v10[2]; // [esp+10h] [ebp-50h] BYREF
  double v11; // [esp+20h] [ebp-40h]
  unsigned int v12; // [esp+2Ch] [ebp-34h]
  __int64 v13; // [esp+30h] [ebp-30h]
  signed __int64 v14; // [esp+38h] [ebp-28h]
  signed __int64 v15; // [esp+40h] [ebp-20h]
  unsigned int v16; // [esp+4Ch] [ebp-14h] BYREF
  unsigned int v17; // [esp+50h] [ebp-10h]
  unsigned int v18; // [esp+54h] [ebp-Ch]
  unsigned int v19; // [esp+58h] [ebp-8h]
  unsigned int v20; // [esp+5Ch] [ebp-4h] BYREF

  sub_45E5F0();
  v0 = sub_45E480();
  v2 = v1 * v0 / 0x3E8u;
  v17 = v2;
  v19 = v1 - 1;
  if ( sub_460320() )
  {
    v3 = sub_45F690(&v20);
    if ( !v3 || v3 == -2130706432 )
    {
      v4 = sub_46F380(v10, (int *)&v16);
      v5 = v4;
      v11 = 0.0;
      v6 = HIDWORD(v4);
      v12 = HIDWORD(v4);
      v18 = 0;
      if ( v16 >> 2 )
      {
        v16 >>= 2;
        do
        {
          ((void (*)(void))sub_498880)();
          v14 += __PAIR64__(v6, v5);
          while ( sub_45F690(&v20) || v20 )
          {
            sub_498880(v7);
            if ( v15 >= v14 )
              goto LABEL_19;
            Sleep(0);
          }
          ((void (*)(void))sub_498880)();
          if ( !sub_45F690(&v20) )
          {
            do
            {
              if ( v19 < v20 )
                v19 = v20;
              ((void (*)(void))sub_498880)();
              v8 = v5 + v13;
            }
            while ( v15 < (__int64)(__PAIR64__(v6, v5) + v13) && !sub_45F690(&v20) );
          }
          sub_498880(v8);
          if ( v15 < (__int64)(__PAIR64__(v12, v5) + v13) )
          {
            ++v18;
            v10[1] = v15 - v13;
            v11 = (double)(v15 - v13) + v11;
          }
          v6 = v12;
LABEL_19:
          --v16;
        }
        while ( v16 );
        if ( v18 && v11 > 0.0 )
        {
          v16 = HIWORD(v20) | 0xC00;
          dword_5172DC = v19;
          return (__int64)((double)(v19 + 1) / (v11 * 1000.0 / (double)(__int64)(v18 * v10[0])) + 0.95);
        }
        v2 = v17;
      }
    }
  }
  result = v2;
  dword_5172DC = v19;
  return result;
}

// ===== sub_46F5D0 @ 0x0046F5D0..0x0046F6E8 =====
int sub_46F5D0()
{
  __int64 v0; // rax
  unsigned int v1; // ebx
  int v2; // edi
  unsigned int v3; // esi
  unsigned __int64 v4; // rax
  int v5; // ecx
  unsigned int v6; // eax
  unsigned int v8; // [esp+Ch] [ebp-4h] BYREF

  sub_46F2B0(&v8, 5u);
  if ( (unsigned __int16)v8 < 0x40u )
  {
    sub_46F2B0(&v8, 9u);
    v1 = sub_4610C0() / (6 * v8);
    v2 = sub_4610A0();
    sub_46F2B0(&v8, 6u);
    v3 = (unsigned __int16)v8;
    if ( (_WORD)v8 )
    {
      sub_46F2B0(&v8, 9u);
      if ( v8 <= 1 )
      {
        return 29297 * ((__int64)((unsigned __int64)v3 << 10) / 4) / 100000;
      }
      else
      {
        if ( (unsigned int)sub_46F310() >= 2 )
          v3 >>= 1;
        v4 = v3 << 10 >> 2;
        if ( v3 < 0x100 )
        {
          LODWORD(v4) = 3125 * v4;
          v5 = 10000 * v2;
        }
        else
        {
          LODWORD(v4) = 293 * v4;
          v5 = 1000 * v2;
        }
        v6 = v4 / (unsigned int)v5;
        if ( v6 >= v1 )
          v6 = v1;
        LODWORD(v0) = v2 * v6;
      }
    }
    else
    {
      LODWORD(v0) = 10240;
    }
  }
  else
  {
    LODWORD(v0) = 100096 * (unsigned int)(unsigned __int16)v8 / 0x3E8;
  }
  return v0;
}

// ===== sub_46F6F0 @ 0x0046F6F0..0x0046F70A =====
BOOL sub_46F6F0()
{
  return (GetSystemDefaultLangID() & 0x3FF) == 17;
}

// ===== sub_46F710 @ 0x0046F710..0x0046F715 =====
// attributes: thunk
BOOL __cdecl sub_46F710()
{
  return sub_46F6F0();
}

// ===== sub_46F720 @ 0x0046F720..0x0046F7F1 =====
int __usercall sub_46F720@<eax>(int a1@<esi>)
{
  int v2; // [esp+4h] [ebp-44h] BYREF
  int v3; // [esp+8h] [ebp-40h]
  int v4; // [esp+Ch] [ebp-3Ch]
  int v5; // [esp+10h] [ebp-38h]
  int v6; // [esp+14h] [ebp-34h]
  int v7; // [esp+18h] [ebp-30h]
  int v8; // [esp+1Ch] [ebp-2Ch]
  int v9; // [esp+20h] [ebp-28h]
  int v10; // [esp+24h] [ebp-24h]
  int v11; // [esp+28h] [ebp-20h]
  int v12; // [esp+2Ch] [ebp-1Ch]
  int v13; // [esp+30h] [ebp-18h]
  int v14; // [esp+34h] [ebp-14h]
  int v15; // [esp+38h] [ebp-10h]
  int v16; // [esp+3Ch] [ebp-Ch]
  int v17; // [esp+40h] [ebp-8h]

  if ( !sub_46E890(&v2, 0x80000000) )
    return 0;
  sub_46E890(&v2, 0x80000002);
  v7 = v3;
  v6 = v2;
  v8 = v4;
  v9 = v5;
  sub_46E890(&v2, 0x80000003);
  v11 = v3;
  v10 = v2;
  v12 = v4;
  v13 = v5;
  sub_46E890(&v2, 0x80000004);
  v14 = v2;
  v17 = v5;
  v15 = v3;
  v16 = v4;
  sub_495B30(a1);
  return 1;
}

// ===== sub_46F800 @ 0x0046F800..0x0046F854 =====
BOOL __cdecl sub_46F800(struct _OSVERSIONINFOA *a1)
{
  BOOL result; // eax

  if ( dword_506708 )
  {
    memset(&VersionInformation, 0, sizeof(VersionInformation));
    VersionInformation.dwOSVersionInfoSize = 148;
    result = GetVersionExA(&VersionInformation);
    dword_506708 = 0;
  }
  qmemcpy(a1, &VersionInformation, sizeof(struct _OSVERSIONINFOA));
  return result;
}

// ===== sub_46F860 @ 0x0046F860..0x0046F896 =====
_DWORD *sub_46F860()
{
  _DWORD *result; // eax

  for ( result = dword_566840; dword_566840; result = dword_566840 )
  {
    dword_566840 = (void *)result[5];
    operator delete(result);
  }
  dword_566844 = 0;
  return result;
}

// ===== fnEnum @ 0x0046F8A0..0x0046F902 =====
BOOL __stdcall fnEnum(HMONITOR a1, HDC a2, LPRECT a3, LPARAM a4)
{
  char *v4; // eax
  _DWORD *v5; // ecx

  v4 = (char *)operator new(0x18u);
  *(struct tagRECT *)(v4 + 4) = *a3;
  v5 = (_DWORD *)dword_566844;
  *((_DWORD *)v4 + 5) = 0;
  dword_566844 = (int)v4;
  if ( v5 )
  {
    *(_DWORD *)v4 = *v5 + 1;
    v5[5] = v4;
  }
  else
  {
    *(_DWORD *)v4 = 0;
    dword_566840 = v4;
  }
  return 1;
}

// ===== sub_46F910 @ 0x0046F910..0x0046F922 =====
BOOL sub_46F910()
{
  return EnumDisplayMonitors(0, 0, fnEnum, 0);
}

// ===== sub_46F930 @ 0x0046F930..0x0046F9BA =====
int __usercall sub_46F930@<eax>(int a1@<esi>, int a2)
{
  int result; // eax
  unsigned int v3; // edi
  int v4; // [esp+Ch] [ebp-10h] BYREF
  int v5; // [esp+10h] [ebp-Ch]
  unsigned int v6; // [esp+14h] [ebp-8h]

  sub_45E750(&v4);
  result = (int)dword_566840;
  if ( dword_566840 )
  {
    v3 = (unsigned int)v4 >> 3;
    v6 = (unsigned int)v4 >> 3;
    while ( 1 )
    {
      if ( (int)(v3 + *(_DWORD *)(result + 4) - v4) <= a1 )
      {
        if ( a1 <= *(_DWORD *)(result + 12) - (v4 >> 3)
          && (int)(*(_DWORD *)(result + 8) + ((unsigned int)v5 >> 3) - v5) <= a2
          && a2 <= *(_DWORD *)(result + 16) - (v5 >> 3) )
        {
          return 1;
        }
        v3 = v6;
      }
      result = *(_DWORD *)(result + 20);
      if ( !result )
        return result;
    }
  }
  return 0;
}

// ===== sub_46F9C0 @ 0x0046F9C0..0x0046FA45 =====
BOOL __usercall sub_46F9C0@<eax>(const CHAR *a1@<eax>, HWND a2@<ecx>, LPSTR pszPath, LPARAM a4)
{
  const ITEMIDLIST *v4; // eax
  ITEMIDLIST *v5; // esi
  BOOL v6; // edi
  _browseinfoA bi; // [esp+Ch] [ebp-20h] BYREF

  bi.hwndOwner = a2;
  bi.pidlRoot = (LPCITEMIDLIST)17;
  bi.pszDisplayName = pszPath;
  if ( a1 )
  {
    bi.lpszTitle = a1;
  }
  else
  {
    bi.lpszTitle = (LPCSTR)&unk_4E7218;
    if ( !sub_46F710() )
      bi.lpszTitle = "Please select the folder.";
  }
  bi.ulFlags = 3;
  bi.lpfn = (BFFCALLBACK)sub_472C00;
  bi.lParam = a4;
  bi.iImage = 0;
  v4 = SHBrowseForFolderA(&bi);
  v5 = (ITEMIDLIST *)v4;
  v6 = v4 != 0;
  if ( v4 )
  {
    SHGetPathFromIDListA(v4, pszPath);
    CoTaskMemFree(v5);
  }
  return v6;
}

// ===== sub_46FA50 @ 0x0046FA50..0x0046FB03 =====
BOOL __cdecl sub_46FA50(const char *a1)
{
  char v1; // cl
  char *v2; // ecx
  char v3; // al
  char v6[2]; // [esp+4h] [ebp-310h] BYREF
  char v7; // [esp+6h] [ebp-30Eh] BYREF

  strcpy(v6, a1);
  sub_42EA80(v1, v6);
  if ( (unsigned __int8)(v6[0] - 97) > 0x19u || v6[1] != 58 || v7 != 92 )
    return 0;
  v2 = &v7;
  v3 = 92;
  while ( !(v3 == 92 ? v2[1] == 92 : v3 == 58) )
  {
    v3 = *++v2;
    if ( !v3 )
      return 1;
  }
  return !*v2;
}

// ===== sub_46FB10 @ 0x0046FB10..0x0046FB83 =====
BOOL __fastcall sub_46FB10(unsigned __int8 *a1, _BYTE *a2, int a3)
{
  int v3; // esi
  unsigned __int8 v4; // bl
  BOOL v5; // eax
  unsigned __int8 *v6; // edx
  int v7; // ecx
  BOOL result; // eax

  v3 = a3;
  while ( v3 >= 0 )
  {
    v4 = *a1;
    if ( !*a1 )
      break;
    if ( v4 == 92 && a1[1] )
      --v3;
    v5 = sub_42FA80(*a1);
    *v6 = v4;
    if ( v5 )
    {
      v6[1] = *(_BYTE *)(v7 + 1);
      a2 = v6 + 2;
      a1 = (unsigned __int8 *)(v7 + 2);
    }
    else
    {
      a2 = v6 + 1;
      a1 = (unsigned __int8 *)(v7 + 1);
    }
    if ( !*a1 )
      --v3;
  }
  result = v3 < 0;
  if ( v3 < 0 )
  {
    if ( a3 > 0 && *a1 )
      *(a2 - 1) = 0;
    else
      *a2 = 0;
  }
  return result;
}

// ===== sub_46FB90 @ 0x0046FB90..0x0046FD10 =====
int __cdecl sub_46FB90(_DWORD *a1)
{
  unsigned __int8 *v1; // ecx
  DWORD FileAttributesA; // eax
  _DWORD *v4; // esi
  _BYTE *v5; // eax
  CHAR *v6; // ecx
  _BYTE *v7; // edx
  CHAR v8; // al
  int v9; // eax
  unsigned __int8 *v11; // [esp+Ch] [ebp-318h]
  int v12; // [esp+10h] [ebp-314h]
  CHAR FileName[780]; // [esp+14h] [ebp-310h] BYREF

  v11 = v1;
  v12 = 0;
  if ( !sub_46FB10(v1, FileName, 0) )
    return 1;
  while ( 1 )
  {
    FileAttributesA = GetFileAttributesA(FileName);
    if ( FileAttributesA == -1 )
      break;
    if ( (FileAttributesA & 0x10) == 0 )
      return 0;
    v4 = (_DWORD *)a1[1];
    if ( v4 )
    {
      while ( strcmp((const char *)*v4, FileName) )
      {
        v4 = (_DWORD *)v4[2];
        if ( !v4 )
          goto LABEL_18;
      }
      goto LABEL_17;
    }
LABEL_18:
    if ( !sub_46FB10(v11, FileName, ++v12) )
      return 1;
  }
  if ( v12 > 0 && CreateDirectoryA(FileName, 0) )
  {
    v4 = operator new(0xCu);
    v5 = operator new[](strlen(FileName) + 1);
    *v4 = v5;
    v4[1] = 0;
    v4[2] = 0;
    v6 = FileName;
    v7 = v5;
    do
    {
      v8 = *v6;
      *v7++ = *v6++;
    }
    while ( v8 );
    v9 = a1[1];
    if ( v9 )
    {
      for ( ; *(_DWORD *)(v9 + 8); v9 = *(_DWORD *)(v9 + 8) )
        ;
      *(_DWORD *)(v9 + 8) = v4;
    }
    else
    {
      a1[1] = v4;
    }
LABEL_17:
    a1 = v4;
    goto LABEL_18;
  }
  return 0;
}

// ===== sub_46FD10 @ 0x0046FD10..0x0046FD80 =====
int __usercall sub_46FD10@<eax>(const char **a1@<ecx>, _DWORD *a2@<edi>, const char *a3)
{
  const char **v3; // esi
  int result; // eax
  const char *v5; // ecx
  char Buffer[780]; // [esp+8h] [ebp-310h] BYREF

  v3 = a1;
  result = 1;
  if ( a1 )
  {
    v5 = *a1;
    if ( *v3 )
    {
      do
      {
        if ( !result )
          break;
        sprintf(Buffer, "%s\\%s", a3, v5);
        result = sub_46FB90(a2);
        v5 = v3[1];
        ++v3;
      }
      while ( v5 );
    }
  }
  return result;
}

// ===== sub_46FD80 @ 0x0046FD80..0x0046FDE9 =====
BOOL __cdecl sub_46FD80(int a1)
{
  BOOL result; // eax

  result = 1;
  if ( a1 )
  {
    if ( *(_DWORD *)(a1 + 4) )
      result = sub_46FD80(*(_DWORD *)(a1 + 4)) != 0;
    if ( *(_DWORD *)(a1 + 8) )
      result = result && sub_46FD80(*(_DWORD *)(a1 + 8));
    if ( *(_DWORD *)a1 )
      return result && RemoveDirectoryA(*(LPCSTR *)a1);
  }
  return result;
}

// ===== sub_46FDF0 @ 0x0046FDF0..0x0046FE36 =====
void __cdecl sub_46FDF0(void **a1)
{
  if ( a1 )
  {
    if ( a1[1] )
      sub_46FDF0(a1[1]);
    if ( a1[2] )
      sub_46FDF0(a1[2]);
    if ( *a1 )
      operator delete[](*a1);
    operator delete(a1);
  }
}

// ===== sub_46FE40 @ 0x0046FE40..0x0046FE57 =====
int __thiscall sub_46FE40(void *this)
{
  int result; // eax
  int i; // ecx

  result = 0;
  for ( i = (int)this - *(_DWORD *)dword_56684C; i >= 0; i -= *(_DWORD *)(dword_56684C + 4 * result) )
    ++result;
  return result;
}

// ===== sub_46FE60 @ 0x0046FE60..0x0046FFDE =====
int __thiscall sub_46FE60(char *Buffer)
{
  int v2; // eax
  unsigned __int8 v3; // cl
  int v4; // ebx
  unsigned __int8 *v5; // eax
  CHAR *v6; // esi
  char *v7; // edi
  UINT DriveTypeA; // eax
  CHAR Buffera[1024]; // [esp+Ch] [ebp-A1Ch] BYREF
  char v11[780]; // [esp+40Ch] [ebp-61Ch] BYREF
  unsigned __int8 String; // [esp+718h] [ebp-310h] BYREF
  char v13; // [esp+719h] [ebp-30Fh]
  unsigned __int8 Str[778]; // [esp+71Ah] [ebp-30Eh] BYREF

  v2 = 0;
  do
  {
    v3 = *(&::Buffer + v2);
    *(&String + v2++) = v3;
  }
  while ( v3 );
  _mbslwr(&String);
  if ( (unsigned __int8)(String - 97) <= 0x19u && v13 == 58 && Str[0] == 92 )
  {
    sprintf(Buffer, "%c%c", (char)String, 58);
    v4 = 1;
  }
  else if ( String == 92 && v13 == 92 && Str[0] != 63 )
  {
    memset(v11, 0, sizeof(v11));
    v5 = _mbschr(Str, 0x5Cu);
    memcpy_0(v11, &String, v5 - &String);
    v4 = 1;
    strcpy(Buffer, v11);
  }
  else
  {
    v4 = 1;
    strcpy(Buffer, (const char *)&String);
  }
  v6 = Buffera;
  GetLogicalDriveStringsA(0x400u, Buffera);
  if ( Buffera[0] )
  {
    v7 = Buffer + 780;
    do
    {
      DriveTypeA = GetDriveTypeA(v6);
      if ( DriveTypeA == 5 || DriveTypeA == 2 )
      {
        sprintf(v7, "%c%c", *v6, v6[1]);
        ++v4;
        v7 += 780;
      }
      v6 += strlen(v6) + 1;
    }
    while ( *v6 );
  }
  return v4;
}

// ===== StartAddress @ 0x0046FFE0..0x004708E0 =====
void __stdcall __noreturn StartAddress(_DWORD *lpThreadParameter)
{
  DWORD FileAttributesA; // eax
  int v2; // edi
  char *v3; // ebx
  int v4; // eax
  char v5; // cl
  int v6; // eax
  char v7; // cl
  DWORD v8; // edi
  DWORD v9; // edi
  int v10; // ebx
  bool v11; // zf
  const CHAR *v12; // eax
  DWORD v13; // esi
  const CHAR *v14; // eax
  int v15; // esi
  int v16; // ecx
  const CHAR *v17; // eax
  CHAR *v18; // edx
  CHAR *v19; // ecx
  unsigned int v20; // edi
  char *v21; // ebx
  int v22; // esi
  _BYTE *v23; // eax
  CHAR *v24; // eax
  bool v25; // cf
  unsigned __int8 v26; // dl
  int v27; // eax
  unsigned __int8 *v28; // ecx
  unsigned int v29; // eax
  unsigned __int8 *v30; // edx
  int v31; // eax
  DWORD v32; // eax
  int v33; // eax
  const CHAR *v34; // eax
  _DWORD *v35; // esi
  const CHAR *v36; // eax
  unsigned int v37; // [esp-4h] [ebp-6858h]
  int v38; // [esp+18h] [ebp-683Ch]
  int i; // [esp+1Ch] [ebp-6838h]
  BOOL v40; // [esp+1Ch] [ebp-6838h]
  BOOL v41; // [esp+20h] [ebp-6834h]
  BOOL v42; // [esp+24h] [ebp-6830h]
  void *lpBuffer; // [esp+28h] [ebp-682Ch]
  int v44; // [esp+2Ch] [ebp-6828h]
  int v45; // [esp+2Ch] [ebp-6828h]
  DWORD nNumberOfBytesToRead; // [esp+30h] [ebp-6824h]
  DWORD NumberOfBytesWritten[3]; // [esp+34h] [ebp-6820h] BYREF
  DWORD NumberOfBytesRead[3]; // [esp+40h] [ebp-6814h] BYREF
  unsigned __int64 v49; // [esp+4Ch] [ebp-6808h] BYREF
  struct _FILETIME v50; // [esp+54h] [ebp-6800h] BYREF
  struct _FILETIME LastAccessTime; // [esp+5Ch] [ebp-67F8h] BYREF
  struct _FILETIME CreationTime; // [esp+64h] [ebp-67F0h] BYREF
  struct _FILETIME v53; // [esp+6Ch] [ebp-67E8h] BYREF
  struct _FILETIME v54; // [esp+74h] [ebp-67E0h] BYREF
  _DWORD v55[2]; // [esp+7Ch] [ebp-67D8h] BYREF
  CHAR v56[784]; // [esp+84h] [ebp-67D0h] BYREF
  CHAR v57[784]; // [esp+394h] [ebp-64C0h] BYREF
  CHAR v58[784]; // [esp+6A4h] [ebp-61B0h] BYREF
  CHAR FileName[784]; // [esp+9B4h] [ebp-5EA0h] BYREF
  CHAR v60[784]; // [esp+CC4h] [ebp-5B90h] BYREF
  char Buffer[784]; // [esp+FD4h] [ebp-5880h] BYREF
  char v62[784]; // [esp+12E4h] [ebp-5570h] BYREF
  char v63[21068]; // [esp+15F4h] [ebp-5260h] BYREF
  int v64; // [esp+6850h] [ebp-4h]

  sub_42D3B0(NumberOfBytesRead);
  v64 = 0;
  sub_42D3B0(NumberOfBytesWritten);
  LOBYTE(v64) = 1;
  sub_464D20();
  sprintf(FileName, "%s\\%s", v60, "inst.tmp");
  FileAttributesA = GetFileAttributesA(FileName);
  SetFileAttributesA(FileName, FileAttributesA & 0xFFFFFFFC);
  v42 = __uncaught_exception();
  v41 = 0;
  v38 = 1;
  while ( 1 )
  {
    v44 = sub_46FE60(v63);
    if ( byte_517C08[0] )
    {
      sub_42D410(v58, 0, 0, v57, (const CHAR *)lpThreadParameter[3]);
      sprintf(Buffer, "%s%s%s", byte_517C08, v57, v58);
      sub_42D410(v58, 0, 0, v57, (const CHAR *)lpThreadParameter[2]);
      sprintf(v62, "%s%s%s", byte_517C08, v57, v58);
    }
    for ( i = 0; i < 20; ++i )
    {
      if ( !v38 )
        goto LABEL_28;
      v2 = 0;
      v3 = v63;
      while ( v2 < v44 || v2 == v44 && byte_517C08[0] )
      {
        if ( v2 == v44 )
        {
          v4 = 0;
          do
          {
            v5 = Buffer[v4];
            v56[v4++] = v5;
          }
          while ( v5 );
        }
        else
        {
          sprintf(v56, "%s\\%s", v3, (const char *)lpThreadParameter[3]);
        }
        if ( sub_464B80(v56) && GetFileAttributesA(v56) != -1 )
        {
          if ( v2 == v44 )
          {
            v6 = 0;
            do
            {
              v7 = v62[v6];
              v56[v6++] = v7;
            }
            while ( v7 );
          }
          else
          {
            sprintf(v56, "%s\\%s", v3, (const char *)lpThreadParameter[2]);
          }
          if ( sub_42D520(v56, (int)NumberOfBytesRead) )
          {
            sub_42D660(&LastAccessTime, (struct _FILETIME *)&v49, (int)NumberOfBytesRead, &CreationTime);
            sub_42D5B0((int)NumberOfBytesRead);
            v38 = 0;
            goto LABEL_26;
          }
        }
        ++v2;
        v3 += 780;
      }
      Sleep(0x32u);
LABEL_26:
      ;
    }
    if ( v38 )
    {
      EnterCriticalSection(&stru_51A940);
      dword_566858 = 1;
      if ( !sub_472B60() )
        goto LABEL_124;
      if ( sub_46BC80(0, (HWND)lpThreadParameter[5], (const CHAR *)lpThreadParameter[4], 0x41u) != 1 )
      {
        v11 = !sub_46F710();
        v36 = (const CHAR *)&unk_4E6DC4;
        if ( v11 )
          v36 = "Are you sure you want to quit?";
        if ( sub_46BC80(0, (HWND)lpThreadParameter[5], v36, 0x124u) == 6 )
        {
          if ( v42 )
            sub_451630(5, lpThreadParameter[6]);
          else
            PostMessageA((HWND)lpThreadParameter[5], 0x111u, 0x8002u, lpThreadParameter[6]);
LABEL_124:
          v38 = 0;
        }
      }
      dword_566858 = 0;
      goto LABEL_126;
    }
LABEL_28:
    if ( sub_42D520((const CHAR *)lpThreadParameter[1], (int)NumberOfBytesWritten)
      && (v8 = sub_42D650((int)NumberOfBytesWritten),
          sub_42D660(&v53, &v50, (int)NumberOfBytesWritten, &v54),
          sub_42D5B0((int)NumberOfBytesWritten),
          v8)
      && *(_QWORD *)&v50 >= v49 )
    {
      v41 = 1;
    }
    else
    {
      v40 = 0;
      v45 = 0;
      while ( 1 )
      {
        v55[0] = 0;
        v55[1] = 0;
        if ( sub_42D570(FileName, (int)NumberOfBytesWritten, 0) )
        {
          sub_42D520(v56, (int)NumberOfBytesRead);
          v9 = sub_42D650((int)NumberOfBytesRead);
          nNumberOfBytesToRead = v9;
          lpBuffer = operator new[](0x10000u);
          v10 = 1;
          if ( sub_472B60() )
          {
            v37 = (v9 + 0xFFFF) >> 16;
            if ( v42 )
              sub_451630(3, v37);
            else
              PostMessageA((HWND)lpThreadParameter[5], 0x111u, 0x8003u, v37);
          }
          if ( !v9 )
            goto LABEL_62;
          while ( 1 )
          {
            if ( !sub_472B60() )
              goto LABEL_62;
            if ( sub_473580() )
            {
              sub_473570();
              v11 = !sub_46F710();
              v12 = (const CHAR *)&unk_4E6DC4;
              if ( v11 )
                v12 = "Are you sure you want to quit?";
              if ( sub_46BC80(0, (HWND)lpThreadParameter[5], v12, 0x124u) == 6 )
                goto LABEL_62;
            }
            v13 = nNumberOfBytesToRead;
            if ( nNumberOfBytesToRead >= 0x10000 )
              v13 = 0x10000;
            if ( sub_42D5D0(v13, lpBuffer, (DWORD)NumberOfBytesRead) != v13 )
              break;
            sub_401900((int)v55, (int)lpBuffer, v13);
            if ( sub_42D600(v13, lpBuffer, (DWORD)NumberOfBytesWritten) != v13 )
            {
              if ( sub_472B60() )
              {
                v11 = !sub_46F710();
                v14 = (const CHAR *)&unk_4E72A0;
                if ( v11 )
                  v14 = "File write error.";
                goto LABEL_61;
              }
              goto LABEL_62;
            }
            if ( sub_472B60() )
            {
              if ( v42 )
                sub_451630(4, v10);
              else
                PostMessageA((HWND)lpThreadParameter[5], 0x111u, 0x8004u, v10);
              ++v10;
            }
            nNumberOfBytesToRead -= v13;
            if ( !nNumberOfBytesToRead )
              goto LABEL_62;
          }
          if ( sub_472B60() )
          {
            v11 = !sub_46F710();
            v14 = (const CHAR *)&unk_4E726C;
            if ( v11 )
              v14 = "File read error.";
LABEL_61:
            sub_46BC80(0, (HWND)lpThreadParameter[5], v14, 0x10u);
          }
LABEL_62:
          if ( !nNumberOfBytesToRead )
          {
            v15 = dword_56685C;
            v41 = 1;
            if ( !dword_56685C )
              goto LABEL_85;
            v16 = (int)lpThreadParameter;
            v17 = (const CHAR *)*lpThreadParameter;
            v18 = &v58[-*lpThreadParameter];
            do
            {
              LOBYTE(v16) = *v17;
              v18[(_DWORD)v17] = *v17;
              ++v17;
            }
            while ( (_BYTE)v16 );
            sub_42EA80(v16, v58);
            v20 = 0;
            if ( !v15 )
              goto LABEL_85;
            v21 = (char *)dword_566860;
            v22 = v57 - (_BYTE *)dword_566860;
            while ( 1 )
            {
              v23 = v21;
              do
              {
                LOBYTE(v19) = *v23;
                v23[v22] = *v23;
                ++v23;
              }
              while ( (_BYTE)v19 );
              sub_42EA80((int)v19, v57);
              v19 = v57;
              v24 = v58;
              while ( 1 )
              {
                v25 = (unsigned __int8)*v24 < (unsigned __int8)*v19;
                if ( *v24 != *v19 )
                  break;
                if ( !*v24 )
                  goto LABEL_75;
                v26 = v24[1];
                v25 = v26 < (unsigned __int8)v19[1];
                if ( v26 != v19[1] )
                  break;
                v24 += 2;
                v19 += 2;
                if ( !v26 )
                {
LABEL_75:
                  v27 = 0;
                  goto LABEL_77;
                }
              }
              v27 = -v25 - (v25 - 1);
LABEL_77:
              if ( !v27 )
                break;
              ++v20;
              v22 -= 64;
              v21 += 64;
              if ( v20 >= dword_56685C )
                goto LABEL_85;
            }
            v28 = (unsigned __int8 *)dword_566860 + 64 * v20 + 56;
            v29 = 8;
            v30 = (unsigned __int8 *)v55;
            while ( *(_DWORD *)v30 == *(_DWORD *)v28 )
            {
              v29 -= 4;
              v28 += 4;
              v30 += 4;
              if ( v29 < 4 )
              {
                v31 = 0;
                goto LABEL_84;
              }
            }
            v33 = *v30 - *v28;
            if ( !v33 )
            {
              v33 = v30[1] - v28[1];
              if ( !v33 )
              {
                v33 = v30[2] - v28[2];
                if ( !v33 )
                  v33 = v30[3] - v28[3];
              }
            }
            v31 = (v33 >> 31) | 1;
LABEL_84:
            v41 = v31 == 0;
            v40 = v31 != 0;
            if ( !v31 )
LABEL_85:
              sub_42D680(&LastAccessTime, (const FILETIME *)&v49, (int)NumberOfBytesWritten, &CreationTime);
          }
          operator delete[](lpBuffer);
          sub_42D5B0((int)NumberOfBytesRead);
          sub_42D5B0((int)NumberOfBytesWritten);
          if ( v41 )
          {
            v32 = GetFileAttributesA((LPCSTR)lpThreadParameter[1]);
            SetFileAttributesA((LPCSTR)lpThreadParameter[1], v32 & 0xFFFFFFFC);
            DeleteFileA((LPCSTR)lpThreadParameter[1]);
            MoveFileA(FileName, (LPCSTR)lpThreadParameter[1]);
          }
        }
        else if ( sub_472B60() )
        {
          v11 = !sub_46F710();
          v34 = (const CHAR *)&unk_4E72D4;
          if ( v11 )
            v34 = "Unable to create file.";
          sub_46BC80(0, (HWND)lpThreadParameter[5], v34, 0x10u);
        }
        if ( (unsigned int)++v45 >= 4 )
          break;
        if ( !v40 )
          goto LABEL_107;
      }
      if ( v40 && sub_472B60() )
      {
        if ( sub_46F710() )
        {
          v35 = lpThreadParameter;
          sprintf(v60, &byte_4E730C, *lpThreadParameter);
        }
        else
        {
          strcpy(v60, "The installation file is corrupted.");
          v35 = lpThreadParameter;
        }
        sub_46BC80(0, (HWND)v35[5], v60, 0x10u);
        goto LABEL_108;
      }
    }
LABEL_107:
    v35 = lpThreadParameter;
LABEL_108:
    EnterCriticalSection(&stru_51A940);
    if ( sub_472B60() )
    {
      if ( v41 )
      {
        if ( v42 )
          sub_451630(2, v35[6]);
        else
          PostMessageA((HWND)v35[5], 0x111u, 0x8001u, v35[6]);
      }
      else if ( v42 )
      {
        sub_451630(5, v35[6]);
      }
      else
      {
        PostMessageA((HWND)v35[5], 0x111u, 0x8002u, v35[6]);
      }
    }
LABEL_126:
    LeaveCriticalSection(&stru_51A940);
    if ( !v38 )
    {
      DeleteFileA(FileName);
      sub_470AE0();
      ExitThread(0xFFFFFEFC);
    }
  }
}
