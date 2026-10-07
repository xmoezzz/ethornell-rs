#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4500A0 @ 0x004500A0..0x004500C1 =====
void *__thiscall sub_4500A0(void *this, char a2)
{
  sub_4500D0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4500D0 @ 0x004500D0..0x00450117 =====
void __thiscall sub_4500D0(_DWORD *this)
{
  *this = &DCPSnow::`vftable';
  sub_44D860(this);
}

// ===== sub_450120 @ 0x00450120..0x00450169 =====
int sub_450120()
{
  _DWORD *v0; // esi
  int result; // eax

  v0 = &unk_5167E4;
  memset(&unk_5144D8, 0, 0x2300u);
  result = 64;
  do
  {
    *(v0 - 3) = 0;
    *(v0 - 2) = 0;
    *v0 = 0;
    *(v0 - 1) = 0;
    v0[1] = 0;
    v0[2] = 0;
    v0[3] = 0;
    v0[4] = 0;
    v0[5] = 0;
    v0[6] = 0;
    v0[7] = 0;
    v0 += 11;
    --result;
  }
  while ( result );
  return result;
}

// ===== sub_450170 @ 0x00450170..0x00450207 =====
BOOL __usercall sub_450170@<eax>(
        int a1@<eax>,
        unsigned int a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int *a11)
{
  BOOL v11; // esi
  _DWORD *v12; // ecx
  int v13; // edx

  v11 = a2 < 0x40;
  if ( a2 < 0x40 )
  {
    v12 = (_DWORD *)((char *)&unk_5167D8 + 44 * a2);
    v12[1] = (int)abs32(a1) >> 8;
    v12[2] = a3 >> 8;
    v12[3] = (int)abs32(a4) >> 8;
    v12[4] = a5 >> 8;
    v12[5] = (int)abs32(a6) >> 8;
    v12[7] = (int)abs32(a8) >> 8;
    v12[9] = (int)abs32(a10) >> 8;
    v13 = *a11 >> 8;
    *v12 = 1;
    v12[6] = a7 >> 8;
    v12[8] = a9 >> 8;
    v12[10] = v13;
  }
  return v11;
}

// ===== sub_450210 @ 0x00450210..0x00450241 =====
int __usercall sub_450210@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<ecx>,
        unsigned int a3,
        _DWORD *a4,
        unsigned int a5)
{
  if ( a1 >= 0x40 )
    return -2147483647;
  else
    return sub_44D880((unsigned int *)&unk_5144D8 + 35 * a1, a3, a4, a5, a2);
}

// ===== sub_450250 @ 0x00450250..0x00450275 =====
void *sub_450250()
{
  char *v0; // esi
  int v1; // edi
  void *result; // eax

  v0 = (char *)&unk_5144D8;
  v1 = 64;
  do
  {
    result = sub_44DA20(v0);
    v0 += 140;
    --v1;
  }
  while ( v1 );
  return result;
}

// ===== sub_450280 @ 0x00450280..0x004502E2 =====
int __thiscall sub_450280(void *this, unsigned int a2)
{
  int v2; // eax
  int v3; // edx
  int v4; // ecx
  int v5; // ecx
  int v6; // eax
  int v7; // edx
  int v8; // ecx

  if ( a2 > 0x1F )
    return 0;
  v2 = sub_42D560((int)this);
  if ( !*(_DWORD *)(dword_5144DC[35 * v2 + v3] + 24 * sub_44DBC0(v4)) )
    return 0;
  v6 = sub_42D560(v5);
  return dword_5144DC[35 * v6 + v7] + 24 * sub_44DBC0(v8);
}

// ===== sub_4502F0 @ 0x004502F0..0x0045030F =====
int __thiscall sub_4502F0(_DWORD *this)
{
  _DWORD *v1; // ecx
  int result; // eax

  sub_44DB40(this);
  result = v1[3];
  if ( result )
  {
    v1[4] += v1[16];
    v1[5] += v1[17];
    v1[6] += v1[18];
  }
  return result;
}

// ===== sub_450310 @ 0x00450310..0x0045033B =====
int *sub_450310()
{
  dword_566AA8 = 0;
  dword_566AAC = 0;
  dword_566AB0 = 0;
  dword_566AB4 = 0;
  dword_566AB8 = 0;
  dword_566AA4 = (int)&DCPrlddDtMngr::`vftable';
  return &dword_566AA4;
}

// ===== sub_450340 @ 0x00450340..0x00450361 =====
void *__thiscall sub_450340(void *this, char a2)
{
  sub_450370();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_450370 @ 0x00450370..0x00450380 =====
int __thiscall sub_450370(_DWORD *this)
{
  *this = &DCPrlddDtMngr::`vftable';
  return sub_4504F0();
}

// ===== sub_450380 @ 0x00450380..0x00450474 =====
int __userpurge sub_450380@<eax>(const char *a1@<eax>, const char *a2, void *Src, size_t Size)
{
  const char *v4; // ebx
  _DWORD *v6; // eax
  int v7; // esi
  _BYTE *v8; // eax
  const char *v9; // ecx
  _BYTE *v10; // edx
  char v11; // al
  _BYTE *v12; // eax
  const char *v13; // ecx
  _BYTE *v14; // edx
  char v15; // al
  size_t v16; // edi
  void *v17; // eax
  const void *v19; // [esp-10h] [ebp-1Ch]

  v4 = a2;
  if ( sub_450510(&dword_566AA4, &a2, a1) )
    return 0;
  v6 = operator new(0x14u);
  v7 = (int)v6;
  if ( a1 )
  {
    v8 = operator new(strlen(a1) + 1);
    *(_DWORD *)v7 = v8;
    v9 = a1;
    v10 = v8;
    do
    {
      v11 = *v9;
      *v10++ = *v9++;
    }
    while ( v11 );
    _mbslwr(*(unsigned __int8 **)v7);
  }
  else
  {
    *v6 = 0;
  }
  v12 = operator new(strlen(v4) + 1);
  *(_DWORD *)(v7 + 4) = v12;
  v13 = v4;
  v14 = v12;
  do
  {
    v15 = *v13;
    *v14++ = *v13++;
  }
  while ( v15 );
  _mbslwr(*(unsigned __int8 **)(v7 + 4));
  v16 = Size;
  v17 = operator new(Size);
  v19 = Src;
  *(_DWORD *)(v7 + 8) = v17;
  memcpy_0(v17, v19, v16);
  *(_DWORD *)(v7 + 12) = v16;
  *(_DWORD *)(v7 + 16) = dword_566AB8;
  dword_566AB8 = v7;
  return 1;
}

// ===== sub_450480 @ 0x00450480..0x004504E7 =====
int __userpurge sub_450480@<eax>(void *a1@<esi>, size_t *a2, int a3, void *a4, int a5)
{
  void *v5; // ebx
  const void **v6; // ecx
  size_t v7; // eax

  v5 = a4;
  if ( !sub_450510(&dword_566AA4, &a4, a3) )
    return 0;
  v6 = (const void **)a4;
  v7 = *((_DWORD *)a4 + 3);
  *a2 = v7;
  if ( a1 )
  {
    memcpy_0(a1, v6[2], v7);
    if ( a5 )
      sub_450680(v5);
  }
  return 1;
}

// ===== sub_4504F0 @ 0x004504F0..0x0045050D =====
int __usercall sub_4504F0@<eax>(int a1@<esi>)
{
  int result; // eax

  for ( ; *(_DWORD *)(a1 + 20); result = sub_450680(*(void **)(*(_DWORD *)(a1 + 20) + 4)) )
    ;
  return result;
}

// ===== sub_450510 @ 0x00450510..0x00450677 =====
int __fastcall sub_450510(_DWORD *a1, const char *a2, int a3, _DWORD *a4, const char *a5)
{
  int v6; // esi
  int v7; // edi
  _BYTE *v8; // ecx
  unsigned __int8 *v9; // eax
  unsigned __int8 v10; // dl
  BOOL v11; // eax
  unsigned __int8 v14[780]; // [esp+14h] [ebp-61Ch] BYREF
  unsigned __int8 String[780]; // [esp+320h] [ebp-310h] BYREF

  if ( a5 )
  {
    strcpy((char *)String, a5);
    _mbslwr(String);
  }
  else
  {
    String[0] = 0;
  }
  strcpy((char *)v14, a2);
  _mbslwr(v14);
  v6 = a3 + 4;
  v7 = *(_DWORD *)(a3 + 20);
  if ( !v7 )
    return 0;
  while ( 1 )
  {
    v8 = *(_BYTE **)v7;
    if ( *(_DWORD *)v7 )
    {
      v9 = String;
      while ( *v9 == *v8 )
      {
        if ( !*v9 )
          goto LABEL_11;
        v10 = v9[1];
        if ( v10 != v8[1] )
          break;
        v9 += 2;
        v8 += 2;
        if ( !v10 )
        {
LABEL_11:
          v11 = 1;
          goto LABEL_14;
        }
      }
      v11 = 0;
    }
    else
    {
      v11 = a5 == 0;
    }
LABEL_14:
    if ( v11 && !strcmp((const char *)v14, *(const char **)(v7 + 4)) )
      break;
    v6 = v7;
    v7 = *(_DWORD *)(v7 + 16);
    if ( !v7 )
      return 0;
  }
  *a4 = v7;
  if ( a1 )
    *a1 = v6;
  return 1;
}

// ===== sub_450680 @ 0x00450680..0x004506E0 =====
int __userpurge sub_450680@<eax>(const char *a1@<eax>, int a2@<edx>, const char *a3)
{
  void **v3; // esi
  int v5; // [esp+4h] [ebp-4h] BYREF

  if ( !sub_450510(&v5, a3, a2, &a3, a1) )
    return 0;
  v3 = (void **)a3;
  *(_DWORD *)(v5 + 16) = *((_DWORD *)a3 + 4);
  operator delete(*v3);
  operator delete(v3[1]);
  operator delete(v3[2]);
  operator delete(v3);
  return 1;
}

// ===== sub_4506E0 @ 0x004506E0..0x00450761 =====
_DWORD *__userpurge sub_4506E0@<eax>(void *a1@<ecx>, int a2@<edi>, _DWORD *a3, int a4, int a5)
{
  int v5; // eax

  sub_43D0D0(a1, a3);
  *a3 = &DCProcDecodeBMV::`vftable';
  sub_43D1A0();
  v5 = sub_450890(a3, a5);
  a3[8] = v5;
  if ( v5 == 0x7FFFFFFF )
  {
    a3[9] = a4;
    a3[10] = a5;
    a3[11] = a2;
  }
  return a3;
}

// ===== sub_450770 @ 0x00450770..0x00450791 =====
void *__thiscall sub_450770(void *this, char a2)
{
  sub_4507A0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4507A0 @ 0x004507A0..0x00450809 =====
int __thiscall sub_4507A0(int this)
{
  *(_DWORD *)this = &DCProcDecodeBMV::`vftable';
  if ( *(_DWORD *)(this + 48) )
    operator delete(*(void **)(this + 48));
  sub_43D1B0();
  return sub_43D150((_DWORD *)this);
}

// ===== sub_450810 @ 0x00450810..0x00450881 =====
int __thiscall sub_450810(int this)
{
  int v2; // esi
  int v3; // eax

  v2 = *(_DWORD *)(this + 32);
  if ( v2 < 0 )
    return -1;
  if ( v2 )
  {
    if ( v2 != 0x7FFFFFFF )
    {
LABEL_7:
      sub_4450D0(*(_DWORD **)(this + 4), v2);
      return 1;
    }
    *(_DWORD *)(this + 32) = sub_450890(this, *(_DWORD *)(this + 40));
  }
  else
  {
    sub_431AF0(this, this);
    v3 = *(_DWORD *)(this + 48);
    if ( *(_DWORD *)(v3 + 8) )
    {
      if ( *(_DWORD *)(v3 + 24) )
      {
        sub_4450D0(*(_DWORD **)(this + 4), *(_DWORD *)(this + 32));
        return 1;
      }
      v2 = 8;
      goto LABEL_7;
    }
  }
  return 0;
}

// ===== sub_450890 @ 0x00450890..0x0045090D =====
int __userpurge sub_450890@<eax>(int a1@<eax>, int a2@<ecx>, int a3, void *a4)
{
  int v4; // esi
  unsigned int v5; // eax

  v4 = -1;
  v5 = sub_406520(a4, (_DWORD *)(a3 + 48), a2, a1);
  if ( v5 > 0x80000005 )
  {
    if ( v5 == -2147483640 )
      sub_4646F0(*(_DWORD *)(a3 + 4));
    if ( v5 == -2147483639 )
      return 0x7FFFFFFF;
  }
  else
  {
    switch ( v5 )
    {
      case 0x80000005:
        return 5;
      case 0u:
        return 0;
      case 0x80000003:
        return 3;
    }
  }
  return v4;
}

// ===== sub_450910 @ 0x00450910..0x00450992 =====
_DWORD *__thiscall sub_450910(void *this, _DWORD *a2, int a3, int a4)
{
  int v4; // edx
  int v5; // eax

  sub_43D0D0(this, a2);
  *a2 = &DCProcDecodeData::`vftable';
  a2[8] = 0;
  v5 = sub_498670(v4, a3, a4);
  a2[9] = v5;
  if ( !v5 )
    sub_4646F0(a2[1]);
  sub_43D1A0();
  return a2;
}

// ===== sub_4509A0 @ 0x004509A0..0x004509C1 =====
void *__thiscall sub_4509A0(void *this, char a2)
{
  sub_4509D0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4509D0 @ 0x004509D0..0x00450A42 =====
int __thiscall sub_4509D0(int this)
{
  *(_DWORD *)this = &DCProcDecodeData::`vftable';
  sub_4450D0(*(_DWORD **)(this + 4), *(_DWORD *)(this + 32));
  operator delete(*(void **)(this + 36));
  sub_43D1B0();
  return sub_43D150((_DWORD *)this);
}

// ===== sub_450A50 @ 0x00450A50..0x00450A72 =====
int __thiscall sub_450A50(_DWORD *this)
{
  int v2; // eax

  sub_431AF0((int)this, (int)this);
  v2 = this[9];
  if ( !*(_DWORD *)(v2 + 4) )
    return 0;
  this[8] = *(_DWORD *)(v2 + 12);
  return 1;
}

// ===== sub_450A80 @ 0x00450A80..0x00450B0D =====
_DWORD *__stdcall sub_450A80(_DWORD *a1, int a2)
{
  int v3; // [esp+0h] [ebp-18h]

  sub_4397E0(a1, a2, v3);
  *a1 = &DCProcImageSynth::`vftable';
  a1[404] = 0;
  a1[411] = 0;
  if ( !sub_451070(a1) )
    a1[406] = *(_DWORD *)(a1[404] + 28) != 0;
  return a1;
}

// ===== sub_450B10 @ 0x00450B10..0x00450B32 =====
void *__thiscall sub_450B10(void *this, char a2)
{
  sub_450B40(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_450B40 @ 0x00450B40..0x00450BBF =====
int __stdcall sub_450B40(void **a1)
{
  void **v1; // eax

  *a1 = &DCProcImageSynth::`vftable';
  while ( sub_451040(a1) )
    ;
  v1 = (void **)a1[411];
  if ( v1 )
  {
    operator delete[](*v1);
    operator delete(a1[411]);
  }
  return sub_439850(a1);
}

// ===== sub_450BC0 @ 0x00450BC0..0x0045103A =====
int __fastcall sub_450BC0(int a1)
{
  int v1; // ebx
  _DWORD *v2; // edi
  BOOL v3; // esi
  int v4; // eax
  BOOL v5; // eax
  int v6; // esi
  _DWORD *v7; // edi
  int v8; // eax
  int v9; // esi
  int v10; // eax
  int v11; // ecx
  bool v12; // zf
  unsigned __int16 *v13; // eax
  int v14; // edx
  char *v15; // eax
  int v16; // ecx
  _DWORD *v17; // ecx
  _DWORD *v18; // ecx
  int v19; // eax
  _DWORD *v20; // eax
  int v21; // edx
  int v22; // edi
  _DWORD *v23; // eax
  int v24; // edx
  int v25; // eax
  int v26; // esi
  int v27; // ecx
  void *v29; // [esp-4h] [ebp-44Ch]
  void *v30; // [esp+10h] [ebp-438h]
  char *v31; // [esp+10h] [ebp-438h]
  char *v32; // [esp+10h] [ebp-438h]
  char *v33; // [esp+14h] [ebp-434h]
  int v34; // [esp+18h] [ebp-430h]
  char *v35; // [esp+1Ch] [ebp-42Ch] BYREF
  int v36; // [esp+20h] [ebp-428h]
  int v37; // [esp+24h] [ebp-424h]
  int v38; // [esp+28h] [ebp-420h]
  int v39; // [esp+2Ch] [ebp-41Ch]
  int v40; // [esp+30h] [ebp-418h]
  int v41; // [esp+34h] [ebp-414h]
  void *v42; // [esp+38h] [ebp-410h]
  int v43; // [esp+3Ch] [ebp-40Ch]
  char Buffer[1028]; // [esp+40h] [ebp-408h] BYREF

  v1 = a1;
  v2 = *(_DWORD **)(a1 + 1616);
  if ( !v2 )
    return -2147483647;
  if ( *v2 )
  {
    v3 = 1;
    if ( !dword_565D3C
      || (v4 = sub_446060(*(void **)(a1 + 32), (_DWORD *)(a1 + 36), *(_BYTE *)(a1 + 40) == 48 ? 0 : a1 + 40, v2[1]),
          v3 = v4 == 0,
          !v4) )
    {
      sub_497EA0(
        *(_DWORD *)(v1 + 32),
        *(_BYTE *)(v1 + 40) == 48 ? 0 : v1 + 40,
        *(_DWORD *)(*(_DWORD *)(v1 + 1616) + 4),
        0,
        0);
    }
    a1 = *(_DWORD *)(v1 + 1616);
    *(_DWORD *)a1 = 0;
    v5 = v3 && dword_565D3C;
    *(_DWORD *)(v1 + 1620) = v5;
  }
  v6 = *(_DWORD *)(v1 + 36);
  if ( !v6 )
    return 1;
  if ( v6 == -1 )
    return -2147483646;
  if ( !*(_DWORD *)(v1 + 1624) )
    return 0;
  if ( *(_DWORD *)(v1 + 1620) )
    sub_445F40(
      *(_BYTE *)(v1 + 40) == 48 ? 0 : (const char *)(v1 + 40),
      *(const char **)(*(_DWORD *)(v1 + 1616) + 4),
      *(void **)(v1 + 32),
      *(_DWORD *)(v1 + 36));
  v7 = *(_DWORD **)(v1 + 32);
  v8 = *((unsigned __int16 *)v7 + 1);
  v9 = *(unsigned __int16 *)v7;
  v33 = (char *)(v7 + 4);
  v43 = v9;
  v34 = v8;
  v41 = sub_401C10(a1, (int)v7);
  v10 = sub_407B30(v41);
  v12 = *((_WORD *)v7 + 3) == 1;
  v30 = (void *)v10;
  v42 = 0;
  if ( v12 )
  {
    v7 = operator new[](v9 * v34 * (*((unsigned __int16 *)v7 + 2) >> 3) + 16);
    v13 = *(unsigned __int16 **)(v1 + 32);
    v42 = v7;
    if ( !sub_4058D0(v13, v7) )
    {
      sprintf(Buffer, &byte_4E55A8, v1 + 40, *(_DWORD *)(*(_DWORD *)(v1 + 1616) + 4));
      sub_4646F0(*(_DWORD *)(v1 + 4));
    }
    v11 = v41;
    v10 = (int)v30;
    v33 = (char *)(v7 + 4);
  }
  v31 = 0;
  if ( v11 == 1 )
  {
    sub_409030(1, v34, &v35, v9);
    v14 = v38;
    v32 = v35;
    v15 = v33;
    if ( v38 )
    {
      v16 = v37;
      do
      {
        --v14;
        if ( v16 )
        {
          do
          {
            *(_DWORD *)v32 = (unsigned __int8)*v15 | (*(unsigned __int16 *)(v15 + 1) << 8);
            v15 += 3;
            --v16;
            v32 += 4;
          }
          while ( v16 );
          v16 = v37;
        }
      }
      while ( v14 );
    }
    v9 = v43;
    v31 = v35;
  }
  else
  {
    v35 = v33;
    v36 = v9 * v10;
    v37 = v9;
    v38 = v34;
    v39 = v11;
    v40 = v10;
  }
  v17 = *(_DWORD **)(v1 + 1644);
  if ( v17 )
  {
    v20 = *(_DWORD **)(v1 + 1616);
    if ( v20[2] )
    {
      v21 = v20[3];
      v22 = v20[4];
    }
    else if ( *((_WORD *)v7 + 5) == 1 )
    {
      v21 = *((unsigned __int16 *)v7 + 6);
      v22 = *((unsigned __int16 *)v7 + 7);
    }
    else
    {
      v21 = 0;
      v22 = 0;
    }
    sub_40A530((int *)&v35, v17, v22, v21, v20[5], v20[6]);
  }
  else
  {
    *(_DWORD *)(v1 + 1628) = *v7;
    *(_DWORD *)(v1 + 1632) = v7[1];
    *(_DWORD *)(v1 + 1636) = v7[2];
    *(_DWORD *)(v1 + 1640) = v7[3];
    v18 = operator new(0x18u);
    v19 = v41;
    *(_DWORD *)(v1 + 1644) = v18;
    sub_409030(v19, v34, v18, v9);
    sub_40ADF0(*(_DWORD *)(v1 + 1644), &v35);
  }
  operator delete[](v31);
  sub_451040(v1);
  if ( *(_DWORD *)(v1 + 1616) )
  {
    operator delete[](v42);
    return 1;
  }
  else
  {
    v23 = *(_DWORD **)(v1 + 32);
    *v23 = *(_DWORD *)(v1 + 1628);
    v23[1] = *(_DWORD *)(v1 + 1632);
    v23[2] = *(_DWORD *)(v1 + 1636);
    v23[3] = *(_DWORD *)(v1 + 1640);
    if ( *((_WORD *)v23 + 2) == 24 )
    {
      *((_WORD *)v23 + 2) = 32;
      *((_WORD *)v23 + 4) = 7;
    }
    v35 = (char *)(*(_DWORD *)(v1 + 32) + 16);
    v24 = *(_DWORD *)(v1 + 1644);
    v25 = *(_DWORD *)(v24 + 20);
    v26 = v25 * *(_DWORD *)(v24 + 8);
    v37 = *(_DWORD *)(v24 + 8);
    v38 = *(_DWORD *)(v24 + 12);
    v27 = *(_DWORD *)(v24 + 16);
    v40 = v25;
    v36 = v26;
    v39 = v27;
    sub_40ADF0((int)&v35, (char **)v24);
    v29 = v42;
    *(_DWORD *)(v1 + 36) = v26 * *(_DWORD *)(*(_DWORD *)(v1 + 1644) + 12) + 16;
    operator delete[](v29);
    return 0;
  }
}

// ===== sub_451040 @ 0x00451040..0x0045106F =====
int __thiscall sub_451040(_DWORD *this)
{
  int v1; // esi
  int result; // eax

  v1 = this[404];
  result = 0;
  if ( v1 )
  {
    this[404] = *(_DWORD *)(v1 + 28);
    operator delete[](*(void **)(v1 + 4));
    operator delete((void *)v1);
    return 1;
  }
  return result;
}

// ===== sub_451070 @ 0x00451070..0x004512D5 =====
int __thiscall sub_451070(const char *this, int a2)
{
  const char *v2; // edi
  char *v3; // esi
  int *v4; // ebx
  int v5; // eax
  char v6; // cl
  char *v7; // eax
  char *v8; // ecx
  int v9; // edx
  char v10; // al
  int v11; // esi
  void *v12; // eax
  int v14; // eax
  _DWORD v15[2]; // [esp+Ch] [ebp-54Ch] BYREF
  int v16; // [esp+14h] [ebp-544h]
  char *v17; // [esp+18h] [ebp-540h]
  int v18; // [esp+1Ch] [ebp-53Ch] BYREF
  int v19; // [esp+20h] [ebp-538h]
  int v20; // [esp+24h] [ebp-534h]
  _DWORD v21[8]; // [esp+28h] [ebp-530h] BYREF
  const char *v22; // [esp+48h] [ebp-510h]
  int v23; // [esp+4Ch] [ebp-50Ch]
  char v24[256]; // [esp+50h] [ebp-508h] BYREF
  char Buffer[1028]; // [esp+150h] [ebp-408h] BYREF

  v2 = this;
  v20 = a2;
  v22 = this;
  v19 = -1;
  v3 = (char *)operator new[](strlen(this) + 1);
  v17 = strcpy(v3, v2);
  v16 = 1;
  v18 = 0;
  v4 = &v18;
  if ( v3 )
  {
    while ( 2 )
    {
      v23 = sub_4512E0((unsigned __int8 *)v3);
      v5 = sub_4512E0((unsigned __int8 *)v3);
      v6 = *v3;
      v15[1] = v5;
      if ( !v6 )
      {
        v14 = sub_464760(v24);
        sprintf(Buffer, &byte_4E5E10, v2, v14);
        sub_4646F0(*(_DWORD *)(v20 + 4));
      }
      memset(&v21[1], 0, 28);
      v21[0] = 1;
      if ( v6 == 32 )
      {
        do
          ++v3;
        while ( *v3 == 32 );
      }
      v7 = &v3[strlen(v3)];
      while ( v3 != v7 )
      {
        if ( *--v7 != 32 )
        {
          v7[1] = 0;
          break;
        }
      }
      v21[1] = operator new[](strlen(v3) + 1);
      v8 = v3;
      v9 = v21[1] - (_DWORD)v3;
      do
      {
        v10 = *v8;
        v8[v9] = *v8;
        ++v8;
      }
      while ( v10 );
      v11 = 0;
      if ( sub_451300(v15) )
      {
        v21[3] = v15[0];
        v11 = 1;
      }
      if ( sub_451300(v15) )
      {
        v21[4] = v15[0];
        v21[2] = v11;
      }
      if ( sub_451300(v15) )
      {
        if ( v15[0] > 7u )
          v21[5] = 128;
        else
          v21[5] = v15[0] + 32;
      }
      if ( sub_451300(v15) )
        v21[6] = v15[0] <= 0x100u ? v15[0] : 0;
      v12 = operator new(0x20u);
      ++v16;
      *v4 = (int)v12;
      qmemcpy(v12, v21, 0x20u);
      v3 = (char *)v23;
      v4 = (int *)(*v4 + 28);
      if ( v23 )
      {
        v2 = v22;
        continue;
      }
      break;
    }
    v3 = v17;
    if ( v18 )
    {
      *(_DWORD *)(v20 + 1616) = v18;
      v19 = 0;
    }
  }
  operator delete[](v3);
  return v19;
}

// ===== sub_4512E0 @ 0x004512E0..0x004512FE =====
unsigned __int8 *__usercall sub_4512E0@<eax>(unsigned __int8 *Str@<ecx>, char a2@<al>)
{
  unsigned __int8 *v2; // eax

  v2 = _mbschr(Str, a2);
  if ( !v2 )
    return 0;
  *v2 = 0;
  return v2 + 1;
}

// ===== sub_451300 @ 0x00451300..0x00451476 =====
int __userpurge sub_451300@<eax>(unsigned __int8 **a1@<eax>, int *a2)
{
  unsigned __int8 *v2; // esi
  int v3; // ebx
  char *v4; // edi
  const char *v5; // ecx
  int v6; // edi
  char v7; // al
  int v8; // esi
  int i; // [esp+Ch] [ebp-8h]
  char *v11; // [esp+10h] [ebp-4h]

  v2 = *a1;
  v3 = 0;
  if ( *a1 )
  {
    *a1 = sub_4512E0(v2, 44);
    v4 = (char *)operator new[](strlen((const char *)v2) + 1);
    v11 = strcpy(v4, (const char *)v2);
    _mbslwr((unsigned __int8 *)v4);
    v5 = v11;
    v6 = 0;
    for ( i = 0; *v5 == 32; ++v5 )
      ;
    if ( !strcmp(v5, "-") )
    {
      ++v5;
      i = 1;
    }
    if ( !strcmp(v5, "0x") )
    {
      v5 += 2;
      v6 = 1;
    }
    v7 = *v5;
    v8 = 0;
    if ( v6 )
    {
      for ( ; v7; v3 = 1 )
      {
        if ( v7 < 48 || v7 > 57 )
        {
          if ( v7 < 97 || v7 > 102 )
            break;
          v8 = v7 + 16 * v8 - 97;
        }
        else
        {
          v8 = v7 + 16 * (v8 - 3);
        }
        v7 = *++v5;
      }
    }
    else if ( v7 )
    {
      do
      {
        if ( v7 < 48 )
          break;
        if ( v7 > 57 )
          break;
        ++v5;
        v8 = v7 + 10 * v8 - 48;
        v7 = *v5;
        v3 = 1;
      }
      while ( *v5 );
    }
    operator delete[](v11);
    if ( v3 )
    {
      if ( i )
        v8 = -v8;
      *a2 = v8;
    }
  }
  return v3;
}

// ===== sub_451480 @ 0x00451480..0x0045151C =====
int __thiscall sub_451480(void *this, int a2)
{
  int v2; // eax

  sub_43D0D0(this, (_DWORD *)a2);
  *(_DWORD *)a2 = &DCProcInstallation::`vftable';
  *(_DWORD *)(a2 + 32) = 0;
  LOBYTE(v2) = __uncaught_exception();
  if ( !v2 )
  {
    dword_565D9C = a2;
    sub_43D1A0();
    InitializeCriticalSection((LPCRITICAL_SECTION)(a2 + 4208));
    *(_DWORD *)(a2 + 4240) = 0;
    *(_DWORD *)(a2 + 1600) = 0;
    *(_DWORD *)(a2 + 1604) = 0;
    *(_DWORD *)(a2 + 1608) = 0;
    *(_DWORD *)(a2 + 1612) = 0;
    *(_DWORD *)(a2 + 1616) = 0;
  }
  return a2;
}

// ===== sub_451520 @ 0x00451520..0x00451541 =====
void *__thiscall sub_451520(void *this, char a2)
{
  sub_451550();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_451550 @ 0x00451550..0x00451612 =====
int __thiscall sub_451550(int this)
{
  *(_DWORD *)this = &DCProcInstallation::`vftable';
  if ( sub_451660(this) )
  {
    while ( sub_451B00() )
      ;
    DeleteCriticalSection((LPCRITICAL_SECTION)(this + 4208));
    sub_43D1B0();
    dword_565D9C = 0;
    sub_451D90();
    sub_451D90();
    operator delete[](*(void **)(this + 1608));
    sub_451D90();
    sub_451D90();
  }
  return sub_43D150((_DWORD *)this);
}

// ===== ?__uncaught_exception@@YA_NXZ @ 0x00451620..0x0045162C =====
BOOL __cdecl __uncaught_exception()
{
  return dword_565D9C != 0;
}

// ===== sub_451630 @ 0x00451630..0x0045165B =====
int __cdecl sub_451630(int a1, int a2)
{
  int v2; // ecx

  if ( !__uncaught_exception() )
    return v2;
  sub_451AA0(a1, a2);
  return 1;
}

// ===== sub_451660 @ 0x00451660..0x00451675 =====
BOOL __stdcall sub_451660(int a1)
{
  return dword_565D9C == a1;
}

// ===== sub_451680 @ 0x00451680..0x0045197F =====
unsigned int __userpurge sub_451680@<eax>(
        const char *a1@<edx>,
        const void *a2@<ecx>,
        int a3@<esi>,
        int a4,
        _DWORD *a5,
        int a6,
        int a7,
        int a8,
        int a9,
        char *a10,
        char *a11,
        char *a12,
        char *a13,
        char *a14)
{
  _DWORD *v15; // eax
  void *v16; // eax
  int v17; // eax
  int v18; // eax
  char *v19; // eax
  char v20; // cl
  char *v21; // eax
  char v22; // cl
  char *v23; // eax
  char v24; // cl
  char *v25; // eax
  char v26; // cl
  char *v27; // eax
  char v28; // cl
  unsigned int v29; // eax
  unsigned int v30; // ebx
  LPARAM v31; // eax
  int v33; // edi
  int v34; // [esp+0h] [ebp-328h]
  char Buffer[780]; // [esp+18h] [ebp-310h] BYREF

  if ( !sub_451660(v34) )
    return 0x80000000;
  v15 = operator new(0xCu);
  *(_DWORD *)(a3 + 36) = v15;
  *v15 = 0;
  *(_DWORD *)(*(_DWORD *)(a3 + 36) + 4) = 0;
  *(_DWORD *)(*(_DWORD *)(a3 + 36) + 8) = 0;
  if ( !sub_46FB90(*(_DWORD *)(a3 + 36)) )
  {
    v33 = -2147483647;
    goto LABEL_22;
  }
  if ( !sub_46FD10(a1) )
  {
    v33 = -2147483646;
LABEL_22:
    sub_46FD80(*(_DWORD *)(a3 + 36));
    sub_46FDF0(*(void **)(a3 + 36));
    return v33;
  }
  strcpy(byte_51A630, a1);
  strcpy((char *)(a3 + 40), a1);
  *(_DWORD *)(a3 + 1600) = sub_451CF0(a4);
  *(_DWORD *)(a3 + 1604) = sub_451CF0(a5);
  dword_566848 = a6;
  v16 = operator new[](4 * a6);
  *(_DWORD *)(a3 + 1608) = v16;
  memcpy_0(v16, a2, 4 * a6);
  dword_56684C = *(_DWORD *)(a3 + 1608);
  v17 = sub_451CF0(a7);
  *(_DWORD *)(a3 + 1612) = v17;
  dword_566850 = v17;
  v18 = sub_451CF0(a8);
  *(_DWORD *)(a3 + 1616) = v18;
  dword_566854 = v18;
  v19 = a10;
  *(_DWORD *)(a3 + 1620) = a9;
  do
  {
    v20 = *v19;
    v19[a3 - (_DWORD)a10 + 1624] = *v19;
    ++v19;
  }
  while ( v20 );
  v21 = a11;
  do
  {
    v22 = *v21;
    v21[a3 - (_DWORD)a11 + 2404] = *v21;
    ++v21;
  }
  while ( v22 );
  v23 = a12;
  do
  {
    v24 = *v23;
    v23[a3 - (_DWORD)a12 + 2660] = *v23;
    ++v23;
  }
  while ( v24 );
  v25 = a13;
  do
  {
    v26 = *v25;
    v25[a3 - (_DWORD)a13 + 2916] = *v25;
    ++v25;
  }
  while ( v26 );
  v27 = a14;
  do
  {
    v28 = *v27;
    v27[a3 - (_DWORD)a14 + 3696] = *v27;
    ++v27;
  }
  while ( v28 );
  dword_518914 = 0;
  dword_518918 = 0;
  dword_51891C = 0;
  v29 = strlen(&::Buffer);
  v30 = v29 - 4;
  if ( (int)(v29 - 4) <= 0 )
  {
    *(_BYTE *)(a3 + 820) = 0;
  }
  else
  {
    memcpy_0((void *)(a3 + 820), &byte_517F1B, v29 - 4);
    *(_BYTE *)(a3 + v30 + 820) = 0;
  }
  v31 = 0;
  if ( *a5 )
  {
    do
      ++v31;
    while ( a5[v31] );
  }
  dword_518C84 = a3 + 40;
  dword_518C88 = a3 + 820;
  dword_518C8C = v31;
  dword_518C90 = *(_DWORD *)(a3 + 1604);
  dword_518C94 = (int)hWndParent;
  *(_DWORD *)(a3 + 4244) = v31;
  dword_56685C = 0;
  dword_566860 = 0;
  sprintf(Buffer, "%s%s", &::Buffer, "BGI.hvl");
  sub_401A50();
  sub_451AA0(0, 0);
  *(_DWORD *)(a3 + 32) = 1;
  return 0;
}

// ===== sub_451980 @ 0x00451980..0x00451A9A =====
int __thiscall sub_451980(int this)
{
  int v2; // eax
  int v4; // [esp+8h] [ebp-4h]

  if ( !*(_DWORD *)(this + 32) )
    return -1;
  sub_431AF0(this, this);
  v4 = 0;
  v2 = sub_451B60();
  if ( v2 )
  {
    if ( v2 == 1 )
      return 0;
  }
  else if ( sub_470F70(&dword_518914, this + 2916, *(_DWORD *)(this + 1620), &dword_518C84)
         && sub_470BB0(&dword_518C84, this + 2404, this + 2916) )
  {
    v4 = sub_4720B0(this + 2404);
  }
  if ( dword_566860 )
    operator delete[](dword_566860);
  if ( !v4 )
  {
    sub_470B50();
    sub_46FD80(*(_DWORD *)(this + 36));
  }
  sub_4450D0(*(_DWORD **)(this + 4), v4 != 0 ? 0 : 4);
  sub_470B70(&dword_518914);
  sub_46FDF0(*(void **)(this + 36));
  return 1;
}

// ===== sub_451AA0 @ 0x00451AA0..0x00451AF5 =====
void __userpurge sub_451AA0(int a1@<eax>, int a2, int a3)
{
  struct _RTL_CRITICAL_SECTION *v4; // edi
  int i; // esi
  _DWORD *v6; // eax

  v4 = (struct _RTL_CRITICAL_SECTION *)(a1 + 4208);
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 4208));
  for ( i = a1 + 4232; *(_DWORD *)(i + 8); i = *(_DWORD *)(i + 8) )
    ;
  v6 = operator new(0xCu);
  *v6 = a2;
  v6[1] = a3;
  v6[2] = 0;
  *(_DWORD *)(i + 8) = v6;
  LeaveCriticalSection(v4);
}

// ===== sub_451B00 @ 0x00451B00..0x00451B5A =====
int __usercall sub_451B00@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // ebx
  _DWORD *v3; // eax

  v2 = 0;
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 4208));
  v3 = *(_DWORD **)(a1 + 4240);
  if ( v3 )
  {
    if ( a2 )
    {
      *a2 = *v3;
      a2[1] = v3[1];
      a2[2] = 0;
    }
    *(_DWORD *)(a1 + 4240) = v3[2];
    operator delete(v3);
    v2 = 1;
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 4208));
  return v2;
}

// ===== sub_451B60 @ 0x00451B60..0x00451CC7 =====
int __usercall sub_451B60@<eax>(_DWORD *a1@<eax>)
{
  int v2; // esi
  int v3; // esi
  int v4; // esi
  int v6; // [esp-8h] [ebp-24h]
  int v7; // [esp-4h] [ebp-20h]
  int v8; // [esp+Ch] [ebp-10h] BYREF
  int v9; // [esp+10h] [ebp-Ch]
  int v10; // [esp+18h] [ebp-4h]

  v10 = 1;
  if ( !sub_451B00((int)a1, &v8) )
    return 1;
  do
  {
    switch ( v8 )
    {
      case 0:
        InitializeCriticalSection(&stru_51A940);
        dword_506704 = 1;
        sub_451AA0((int)a1, 1, 0);
        break;
      case 1:
        v2 = v9;
        if ( sub_4708F0(v9) )
        {
          sub_473510();
          sub_496540(-268435456, v2, a1[1061]);
        }
        else
        {
          sub_451AA0((int)a1, 6, 1);
        }
        break;
      case 2:
        sub_473520();
        v3 = v9;
        sub_496540(-268435455, v9, a1[1061]);
        sub_451AA0((int)a1, 1, v3 + 1);
        break;
      case 3:
        a1[1062] = v9;
        break;
      case 4:
        v7 = a1[1062];
        v6 = v9;
        a1[1063] = v9;
        sub_496540(-268435454, v6, v7);
        break;
      case 5:
        sub_451AA0((int)a1, 6, 0);
        break;
      case 6:
        sub_473520();
        dword_506704 = 0;
        DeleteCriticalSection(&stru_51A940);
        v4 = v9;
        sub_496540(-268435453, v9, 0);
        v10 = v4 != 0 ? 0 : 2;
        break;
      default:
        break;
    }
  }
  while ( sub_451B00((int)a1, &v8) );
  return v10;
}

// ===== sub_451CF0 @ 0x00451CF0..0x00451D86 =====
_BYTE *__usercall sub_451CF0@<eax>(int a1@<eax>, const char **a2@<ecx>)
{
  const char **v3; // edi
  _BYTE *result; // eax
  int v5; // ecx
  int v6; // ebx
  const char *v7; // eax
  const char *v8; // ecx
  char *v9; // edx
  char v10; // al
  int v11; // [esp+8h] [ebp-8h]
  _BYTE *v12; // [esp+Ch] [ebp-4h]

  v3 = a2;
  result = 0;
  if ( a2 )
  {
    if ( a1 <= 0 )
    {
      a1 = 0;
      if ( *a2 )
      {
        do
          ++a1;
        while ( a2[a1] );
      }
    }
    result = operator new[](4 * (a1 + 1));
    v5 = 0;
    v12 = result;
    if ( a1 > 0 )
    {
      v6 = result - (_BYTE *)v3;
      v11 = a1;
      do
      {
        v7 = (const char *)operator new[](&(*v3)[strlen(*v3) + 1] - *v3);
        *(const char **)((char *)v3 + v6) = v7;
        v8 = *v3;
        v9 = (char *)v7;
        do
        {
          v10 = *v8;
          *v9++ = *v8++;
        }
        while ( v10 );
        ++v3;
        --a1;
      }
      while ( a1 );
      result = v12;
      v5 = v11;
    }
    *(_DWORD *)&result[4 * v5] = 0;
  }
  return result;
}

// ===== sub_451D90 @ 0x00451D90..0x00451DBD =====
void __usercall sub_451D90(void **a1@<edi>)
{
  void *v1; // eax
  int i; // esi

  if ( a1 )
  {
    v1 = *a1;
    for ( i = 0; v1; ++i )
    {
      operator delete[](v1);
      v1 = a1[i + 1];
    }
    operator delete(a1);
  }
}

// ===== sub_451DC0 @ 0x00451DC0..0x00451DDB =====
int __stdcall sub_451DC0(_DWORD *a1)
{
  int result; // eax

  result = *a1 - 2;
  if ( *a1 == 2 )
    return sub_473570();
  return result;
}

// ===== sub_451DE0 @ 0x00451DE0..0x00451EC2 =====
_DWORD *__userpurge sub_451DE0@<eax>(void *a1@<ecx>, const char *a2@<edi>, _DWORD *a3, int a4, const char *a5)
{
  const char *v5; // eax
  int v6; // edx
  _BYTE *v7; // eax
  const char *v8; // ecx
  int v9; // edx
  char v10; // al
  _BYTE *v11; // eax
  const char *v12; // ecx
  int v13; // edx
  char v14; // al

  sub_43D0D0(a1, a3);
  *a3 = &DCProcLoadBMVHeader::`vftable';
  a3[29] = 0;
  if ( a5 )
  {
    sub_43D1A0();
    a3[8] = a4;
    a3[9] = v6;
    if ( a2 == v5 )
    {
      a3[10] = v5;
    }
    else
    {
      v7 = operator new[](strlen(a2) + 1);
      a3[10] = v7;
      v8 = a2;
      v9 = v7 - a2;
      do
      {
        v10 = *v8;
        v8[v9] = *v8;
        ++v8;
      }
      while ( v10 );
    }
    v11 = operator new[](strlen(a5) + 1);
    a3[11] = v11;
    v12 = a5;
    v13 = v11 - a5;
    do
    {
      v14 = *v12;
      v12[v13] = *v12;
      ++v12;
    }
    while ( v14 );
    a3[12] = 1;
  }
  else
  {
    a3[12] = 0;
  }
  return a3;
}

// ===== sub_451ED0 @ 0x00451ED0..0x00451EF1 =====
void *__thiscall sub_451ED0(void *this, char a2)
{
  sub_451F00();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_451F00 @ 0x00451F00..0x00451F84 =====
int __thiscall sub_451F00(int this)
{
  *(_DWORD *)this = &DCProcLoadBMVHeader::`vftable';
  sub_4450D0(*(_DWORD **)(this + 4), *(_DWORD *)(this + 124));
  operator delete[](*(void **)(this + 116));
  operator delete[](*(void **)(this + 40));
  operator delete[](*(void **)(this + 44));
  sub_43D1B0();
  return sub_43D150((_DWORD *)this);
}

// ===== sub_451F90 @ 0x00451F90..0x004520F6 =====
int __thiscall sub_451F90(int this)
{
  int result; // eax
  const void *v3; // esi
  int v4; // eax
  int v5; // ecx
  char *v6; // eax
  const void *v7; // edx
  void *v8; // esi
  int v9; // eax
  unsigned int v10; // esi
  void *v11; // eax
  int v12; // [esp-20h] [ebp-38h]
  int v13; // [esp-18h] [ebp-30h]
  int v14; // [esp-14h] [ebp-2Ch]
  size_t v15; // [esp-Ch] [ebp-24h]
  int v16[2]; // [esp+8h] [ebp-10h] BYREF
  void *Src; // [esp+14h] [ebp-4h]

  if ( *(_DWORD *)(this + 48) == 1 )
  {
    sub_497EA0(this + 52, *(_DWORD *)(this + 40), *(_DWORD *)(this + 44), 0, 64);
    result = 0;
    *(_DWORD *)(this + 48) = 2;
  }
  else
  {
    if ( *(_DWORD *)(this + 48) == 2 )
    {
      v9 = *(_DWORD *)(this + 120);
      if ( v9 )
      {
        if ( v9 == 64 && !sub_49B210() )
        {
          v10 = sub_49B630();
          v11 = operator new[](v10);
          v14 = *(_DWORD *)(this + 44);
          v13 = *(_DWORD *)(this + 40);
          *(_DWORD *)(this + 116) = v11;
          sub_497EA0(v11, v13, v14, 64, v10);
          *(_DWORD *)(this + 48) = 3;
          return 0;
        }
        goto LABEL_12;
      }
    }
    else
    {
      if ( *(_DWORD *)(this + 48) != 3 )
      {
        result = 1;
        *(_DWORD *)(this + 124) = 1;
        return result;
      }
      if ( *(_DWORD *)(this + 120) )
      {
        v3 = (const void *)(this + 52);
        v4 = sub_49B630();
        if ( v5 == v4 )
        {
          v6 = (char *)operator new[](v5 + 64);
          qmemcpy(v6, v3, 0x40u);
          v7 = *(const void **)(this + 116);
          v15 = *(_DWORD *)(this + 120);
          Src = v6;
          memcpy_0(v6 + 64, v7, v15);
          v12 = *(_DWORD *)(this + 44);
          v16[0] = *(_DWORD *)(this + 40);
          v16[1] = v12;
          sub_467F50(0, v12, 0, 0);
          v8 = Src;
          *(_DWORD *)(this + 124) = sub_405B50(
                                      *(_DWORD **)(this + 32),
                                      *(_DWORD **)(this + 36),
                                      Src,
                                      *(_DWORD *)(this + 120) + 64,
                                      (int)v16) != 0
                                  ? 2
                                  : 0;
          operator delete[](v8);
          return 1;
        }
LABEL_12:
        *(_DWORD *)(this + 124) = 2;
        return 1;
      }
    }
    return 0;
  }
  return result;
}

// ===== sub_452100 @ 0x00452100..0x00452176 =====
_DWORD *__stdcall sub_452100(_DWORD *a1, int a2, int a3, int a4)
{
  sub_439690(a1, a4, 1);
  *a1 = &DCProcLoadBurikoMV::`vftable';
  a1[404] = a2;
  a1[405] = a3;
  a1[406] = 1;
  return a1;
}

// ===== sub_452180 @ 0x00452180..0x004521A2 =====
void *__thiscall sub_452180(void *this, char a2)
{
  sub_4521B0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4521B0 @ 0x004521B0..0x00452213 =====
int __stdcall sub_4521B0(int a1)
{
  *(_DWORD *)a1 = &DCProcLoadBurikoMV::`vftable';
  sub_4450D0(*(_DWORD **)(a1 + 4), *(_DWORD *)(a1 + 1624));
  return sub_439850((void **)a1);
}

// ===== sub_452220 @ 0x00452220..0x00452283 =====
int __thiscall sub_452220(int this)
{
  int v2; // eax
  int result; // eax

  v2 = sub_405B50(
         *(_DWORD **)(this + 1616),
         *(_DWORD **)(this + 1620),
         *(_BYTE **)(this + 32),
         *(_DWORD *)(this + 36),
         0);
  if ( v2 )
  {
    if ( v2 == -2147483647 )
    {
      result = 1;
      *(_DWORD *)(this + 1624) = 1;
      return result;
    }
    if ( v2 == -2147483646 )
    {
      *(_DWORD *)(this + 1624) = 2;
      return 1;
    }
  }
  else
  {
    v2 = 0;
  }
  *(_DWORD *)(this + 1624) = v2;
  return 1;
}

// ===== sub_452290 @ 0x00452290..0x004522E8 =====
_DWORD *__stdcall sub_452290(_DWORD *a1, int a2)
{
  sub_450A80(a1, a2);
  *a1 = &DCProcPreloadBmp::`vftable';
  return a1;
}

// ===== sub_4522F0 @ 0x004522F0..0x00452312 =====
void *__thiscall sub_4522F0(void *this, char a2)
{
  sub_452320(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_452320 @ 0x00452320..0x00452369 =====
int __stdcall sub_452320(void **a1)
{
  *a1 = &DCProcPreloadBmp::`vftable';
  return sub_450B40(a1);
}

// ===== sub_452370 @ 0x00452370..0x004523EC =====
int __thiscall sub_452370(int this)
{
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  if ( sub_401CB0(
         *(_DWORD *)(this + 36),
         *(_DWORD *)(this + 32),
         *(_BYTE *)(this + 40) == 48 ? 0 : this + 40,
         this + 820,
         *(_DWORD *)(this + 36)) == 1 )
  {
    sprintf(Buffer, &byte_4E55A8, this + 40, this + 820);
    sub_4646F0(*(_DWORD *)(this + 4));
  }
  return 1;
}

// ===== sub_4523F0 @ 0x004523F0..0x00452455 =====
_DWORD *__userpurge sub_4523F0@<eax>(int a1@<ecx>, int a2@<edi>, _DWORD *a3, int a4, int a5, int a6)
{
  sub_439730(a2, a3, a1, a6);
  *a3 = &DCProcReadBinary::`vftable';
  a3[404] = a5;
  return a3;
}

// ===== sub_452460 @ 0x00452460..0x00452482 =====
void *__thiscall sub_452460(void *this, char a2)
{
  sub_452490(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_452490 @ 0x00452490..0x004524F3 =====
int __stdcall sub_452490(int a1)
{
  *(_DWORD *)a1 = &DCProcReadBinary::`vftable';
  sub_4450D0(*(_DWORD **)(a1 + 4), *(_DWORD *)(a1 + 1604));
  return sub_439850((void **)a1);
}

// ===== sub_452500 @ 0x00452500..0x0045252B =====
int __thiscall sub_452500(int this)
{
  memcpy_0(*(void **)(this + 1616), *(const void **)(this + 32), *(_DWORD *)(this + 36));
  *(_DWORD *)(this + 1604) = 0;
  return 1;
}

// ===== sub_452530 @ 0x00452530..0x004525A6 =====
int __thiscall sub_452530(void *this, _DWORD *a2, int a3, int a4, int a5, double a6, double a7)
{
  int v7; // edx
  int v8; // edx

  sub_43D0D0(this, a2);
  *(double *)(v7 + 48) = a6;
  *(_DWORD *)(v7 + 32) = a3;
  *(double *)(v7 + 56) = a7;
  *(_DWORD *)(v7 + 36) = a4;
  *(_DWORD *)v7 = &DCProcRgstrSound::`vftable';
  *(_DWORD *)(v7 + 40) = a5;
  *(_DWORD *)(v7 + 64) = 0;
  sub_43D1A0();
  return v8;
}

// ===== sub_4525B0 @ 0x004525B0..0x004525D1 =====
void *__thiscall sub_4525B0(void *this, char a2)
{
  sub_4525E0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4525E0 @ 0x004525E0..0x00452633 =====
int __thiscall sub_4525E0(_DWORD *this)
{
  _DWORD *v1; // ecx

  *this = &DCProcRgstrSound::`vftable';
  sub_43D1B0();
  return sub_43D150(v1);
}

// ===== sub_452640 @ 0x00452640..0x004526D8 =====
int __thiscall sub_452640(int this)
{
  unsigned int v2; // eax

  if ( *(_DWORD *)(this + 64) )
  {
    v2 = *(_DWORD *)(this + 68);
    if ( v2 != -1 )
    {
      if ( v2 > 0x80000002 )
      {
        if ( v2 == -1879048193 )
          sub_4646F0(*(_DWORD *)(this + 4));
      }
      else
      {
        switch ( v2 )
        {
          case 0x80000002:
            sub_4646F0(*(_DWORD *)(this + 4));
          case 0u:
            return 1;
          case 0x80000001:
            sub_4646F0(*(_DWORD *)(this + 4));
        }
      }
    }
  }
  else
  {
    sub_498100(
      *(_DWORD *)(this + 32),
      *(_DWORD *)(this + 36),
      *(_DWORD *)(this + 40),
      *(double *)(this + 48),
      *(double *)(this + 56));
    *(_DWORD *)(this + 64) = 1;
  }
  return 0;
}

// ===== sub_4526E0 @ 0x004526E0..0x00452765 =====
_DWORD *__usercall sub_4526E0@<eax>(_DWORD *a1@<esi>)
{
  _OSVERSIONINFOA VersionInformation; // [esp+0h] [ebp-98h] BYREF

  *a1 = &DCSleep::`vftable';
  memset(&VersionInformation, 0, sizeof(VersionInformation));
  VersionInformation.dwOSVersionInfoSize = 148;
  GetVersionExA(&VersionInformation);
  if ( VersionInformation.dwPlatformId == 2 )
    a1[1] = CreateWaitableTimerA(0, 1, 0);
  else
    a1[1] = 0;
  return a1;
}

// ===== sub_452770 @ 0x00452770..0x00452791 =====
void *__thiscall sub_452770(void *this, char a2)
{
  sub_4527A0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4527A0 @ 0x004527A0..0x004527C0 =====
void *__thiscall sub_4527A0(_DWORD *this)
{
  void *result; // eax

  result = (void *)this[1];
  *this = &DCSleep::`vftable';
  if ( result )
  {
    result = (void *)CloseHandle(result);
    this[1] = 0;
  }
  return result;
}

// ===== sub_4527C0 @ 0x004527C0..0x0045282C =====
DWORD __usercall sub_4527C0@<eax>(DWORD result@<eax>, int a2@<esi>)
{
  int v2; // eax
  void *v3; // ecx
  LARGE_INTEGER DueTime; // [esp+8h] [ebp-8h] BYREF

  while ( !*(_DWORD *)(a2 + 4) || !result )
  {
    if ( !result )
    {
      result = SwitchToThread();
      if ( result )
        return result;
    }
  }
  if ( result == 1 )
    v2 = -5000;
  else
    v2 = -10000 * result;
  v3 = *(void **)(a2 + 4);
  DueTime.QuadPart = v2;
  SetWaitableTimer(v3, &DueTime, 0, 0, 0, 0);
  return WaitForSingleObjectEx(*(HANDLE *)(a2 + 4), 0xAu, 1);
}

// ===== sub_452830 @ 0x00452830..0x00452847 =====
int *__usercall sub_452830@<eax>(int *result@<eax>, int a2@<ecx>)
{
  if ( !a2 )
    a2 = 1024;
  *result = a2;
  result[2] = 0;
  result[3] = 0;
  result[4] = 0;
  return result;
}

// ===== sub_452850 @ 0x00452850..0x00452873 =====
int __usercall sub_452850@<eax>(int a1@<esi>)
{
  int result; // eax

  for ( result = *(_DWORD *)(a1 + 16); result; result = *(_DWORD *)(a1 + 16) )
    sub_452960();
  return result;
}

// ===== sub_452880 @ 0x00452880..0x00452952 =====
void *__thiscall sub_452880(void *this, size_t *a2, const char *a3, void *Src)
{
  void *v5; // esi
  size_t *v6; // edi
  void *v7; // eax
  _BYTE *v8; // edx
  const char *v9; // ecx
  char v10; // al
  int v12; // [esp+18h] [ebp+Ch]

  v12 = sub_452AE0(this, a3);
  v5 = (void *)a2[4];
  v6 = a2 + 1;
  if ( v5 )
  {
    while ( v12 != *(_DWORD *)v5 || strcmp(a3, *((const char **)v5 + 1)) )
    {
      v6 = (size_t *)v5;
      v5 = (void *)*((_DWORD *)v5 + 3);
      if ( !v5 )
        goto LABEL_5;
    }
  }
  else
  {
LABEL_5:
    v5 = operator new(0x10u);
    *(_DWORD *)v5 = v12;
    *((_DWORD *)v5 + 1) = operator new(strlen(a3) + 1);
    v7 = operator new(*a2);
    v8 = (_BYTE *)*((_DWORD *)v5 + 1);
    *((_DWORD *)v5 + 2) = v7;
    *((_DWORD *)v5 + 3) = 0;
    v9 = a3;
    do
    {
      v10 = *v9;
      *v8++ = *v9++;
    }
    while ( v10 );
    v6[3] = (size_t)v5;
  }
  return memcpy_0(*((void **)v5 + 2), Src, *a2);
}

// ===== sub_452960 @ 0x00452960..0x004529FE =====
int __usercall sub_452960@<eax>(int a1@<eax>, int a2@<ecx>, const char *a3@<edi>)
{
  _DWORD *v4; // eax
  _DWORD *v5; // esi
  int v7; // [esp+8h] [ebp-4h]

  v7 = sub_452AE0(a2, a3);
  v4 = (_DWORD *)(a1 + 4);
  v5 = *(_DWORD **)(a1 + 16);
  if ( !v5 )
    return -2147483647;
  while ( v7 != *v5 || strcmp(a3, (const char *)v5[1]) )
  {
    v4 = v5;
    v5 = (_DWORD *)v5[3];
    if ( !v5 )
      return -2147483647;
  }
  v4[3] = v5[3];
  operator delete((void *)v5[1]);
  operator delete((void *)v5[2]);
  operator delete(v5);
  return 0;
}

// ===== sub_452A00 @ 0x00452A00..0x00452A94 =====
int __userpurge sub_452A00@<eax>(int a1@<ecx>, const char *a2@<edi>, size_t *a3, void *a4)
{
  size_t v4; // esi
  int v6; // [esp+8h] [ebp-4h]

  v6 = sub_452AE0(a1, a2);
  v4 = a3[4];
  if ( !v4 )
    return -2147483647;
  while ( v6 != *(_DWORD *)v4 || strcmp(a2, *(const char **)(v4 + 4)) )
  {
    v4 = *(_DWORD *)(v4 + 12);
    if ( !v4 )
      return -2147483647;
  }
  memcpy_0(a4, *(const void **)(v4 + 8), *a3);
  return 0;
}

// ===== sub_452AA0 @ 0x00452AA0..0x00452ADA =====
int __userpurge sub_452AA0@<eax>(int a1@<edi>, size_t *a2@<esi>, void *a3)
{
  size_t v3; // ecx
  int v4; // edx
  int result; // eax

  v3 = a2[4];
  v4 = 0;
  result = -2147483646;
  if ( v3 )
  {
    while ( v4 != a1 )
    {
      v3 = *(_DWORD *)(v3 + 12);
      ++v4;
      if ( !v3 )
        return result;
    }
    memcpy_0(a3, *(const void **)(v3 + 8), *a2);
    return 0;
  }
  return result;
}

// ===== sub_452AE0 @ 0x00452AE0..0x00452B03 =====
int __fastcall sub_452AE0(int a1, char *a2)
{
  char v2; // cl
  int result; // eax

  v2 = *a2;
  for ( result = 0; *a2; v2 = *a2 )
  {
    ++a2;
    result = v2 + 233 * result;
  }
  return result;
}

// ===== sub_452B10 @ 0x00452B10..0x00452B78 =====
_DWORD *__fastcall sub_452B10(int a1, int a2, _DWORD *a3, int a4)
{
  sub_4447C0(a1, a2, a3, a4, 0, 0, 0);
  *a3 = &DCTChildThread::`vftable';
  a3[34] = 0;
  return a3;
}

// ===== sub_452B80 @ 0x00452B80..0x00452BA2 =====
void *__thiscall sub_452B80(void *this, char a2)
{
  sub_452BB0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_452BB0 @ 0x00452BB0..0x00452C1A =====
int __stdcall sub_452BB0(int a1)
{
  *(_DWORD *)a1 = &DCTChildThread::`vftable';
  if ( *(_DWORD *)(a1 + 136) )
    (*(void (__thiscall **)(_DWORD, int))(**(_DWORD **)(a1 + 140) + 32))(*(_DWORD *)(a1 + 140), a1);
  return sub_4449C0((_DWORD *)a1);
}

// ===== sub_452C20 @ 0x00452C20..0x00452CD0 =====
int __userpurge sub_452C20@<eax>(int a1@<edi>, _DWORD *a2@<esi>, int a3, int a4, int a5)
{
  int v5; // ebx
  int result; // eax
  int v7; // ecx
  int v8; // eax
  int v9; // ecx
  int v10; // ecx
  _DWORD *v11; // eax
  int v12; // [esp+8h] [ebp-4h] BYREF

  v5 = a4;
  result = -2147483644;
  if ( !a2[34] )
  {
    result = (*(int (__thiscall **)(int, int *, int *, _DWORD *, int, int))(*(_DWORD *)a1 + 28))(
               a1,
               &a4,
               &v12,
               a2,
               a3,
               a4);
    if ( !result )
    {
      v7 = a4;
      a2[7] = a3;
      a2[9] = a3;
      a2[8] = v7;
      a2[11] = sub_444C60(a1);
      v8 = a5;
      a2[14] = v9;
      sub_444FE0(v8, (int)a2);
      a2[16] = v12;
      a2[15] = v5;
      a2[17] = v5;
      a2[19] = sub_444C70(a1);
      sub_445000((int)a2, v10);
      a2[35] = (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 24))(a1);
      v11 = sub_444A60(a2);
      sub_444BD0((int)v11, (int)a2);
      result = 0;
      a2[34] = 1;
    }
  }
  return result;
}

// ===== sub_452CD0 @ 0x00452CD0..0x00452CE1 =====
int __thiscall sub_452CD0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 4))(this[35]);
}

// ===== sub_452CF0 @ 0x00452CF0..0x00452D01 =====
int __thiscall sub_452CF0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 8))(this[35]);
}

// ===== sub_452D10 @ 0x00452D10..0x00452D21 =====
int __thiscall sub_452D10(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 12))(this[35]);
}

// ===== sub_452D30 @ 0x00452D30..0x00452D3D =====
int __thiscall sub_452D30(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 16))(this[35]);
}

// ===== sub_452D40 @ 0x00452D40..0x00452D6C =====
int __thiscall sub_452D40(int this, char **a2, int a3)
{
  if ( a3 )
    return (*(int (__thiscall **)(_DWORD, char **, int))(**(_DWORD **)(this + 140) + 20))(
             *(_DWORD *)(this + 140),
             a2,
             1);
  else
    return sub_444DC0((_DWORD *)this, a2, 0);
}

// ===== sub_452D70 @ 0x00452D70..0x00452D7D =====
int __thiscall sub_452D70(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 24))(this[35]);
}

// ===== sub_452D80 @ 0x00452D80..0x00452D91 =====
int __thiscall sub_452D80(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 28))(this[35]);
}

// ===== sub_452DA0 @ 0x00452DA0..0x00452DB1 =====
int __thiscall sub_452DA0(_DWORD **this)
{
  return (*(int (__thiscall **)(_DWORD *))(*this[35] + 32))(this[35]);
}

// ===== sub_452DC0 @ 0x00452DC0..0x00452DF5 =====
_DWORD *__usercall sub_452DC0@<eax>(int a1@<eax>, _DWORD *a2@<esi>)
{
  a2[1] = a1;
  *a2 = &DCTELgclFldMngr::`vftable';
  a2[4] = 0;
  a2[6] = 0;
  a2[7] = 0;
  a2[5] = 0x10000000;
  memset(a2 + 8, 0, 0x34u);
  a2[8] = 0x20000000;
  return a2;
}

// ===== sub_452E00 @ 0x00452E00..0x00452E21 =====
void *__thiscall sub_452E00(void *this, char a2)
{
  sub_452E30();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_452E30 @ 0x00452E30..0x00452E40 =====
int __thiscall sub_452E30(_DWORD *this)
{
  *this = &DCTELgclFldMngr::`vftable';
  return sub_453E90();
}

// ===== sub_452E40 @ 0x00452E40..0x00452E71 =====
int __usercall sub_452E40@<eax>(_DWORD *a1@<esi>)
{
  int v2; // edi

  if ( !a1[4] )
    return 0;
  v2 = (*(int (__thiscall **)(_DWORD *))(*a1 + 32))(a1);
  sub_452E80(a1[3], v2, a1[2], a1[7], a1[20]);
  return v2;
}

// ===== sub_452E80 @ 0x00452E80..0x0045305C =====
int __userpurge sub_452E80@<eax>(int a1@<ecx>, void *a2@<eax>, _DWORD *a3, int a4, size_t Size, _DWORD *a6)
{
  int v6; // esi
  int result; // eax
  int v9; // ebx
  _DWORD *v10; // esi
  void *v11; // eax
  _DWORD *v12; // eax
  int v13; // esi
  _DWORD *v14; // edi
  void *v15; // eax
  void *v16; // eax
  _DWORD *v17; // [esp+8h] [ebp-Ch]
  _DWORD *v18; // [esp+10h] [ebp-4h]
  _DWORD *v19; // [esp+10h] [ebp-4h]
  _DWORD *v20; // [esp+1Ch] [ebp+8h]
  int v21; // [esp+20h] [ebp+Ch]
  size_t Sizea; // [esp+24h] [ebp+10h]
  int Sizeb; // [esp+24h] [ebp+10h]
  _DWORD *v24; // [esp+28h] [ebp+14h]

  v6 = (int)a3;
  result = sub_453060(a1, a2);
  if ( !result )
  {
    v9 = a3[2] * a3[3];
    v21 = v9;
    Sizea = 0;
    v18 = a3 + 5;
    if ( Size )
    {
      do
      {
        v10 = operator new(0xCu);
        *v10 = *(_DWORD *)Size;
        v11 = operator new(4 * v9);
        v10[1] = v11;
        v10[2] = 0;
        memcpy_0(v11, *(const void **)(Size + 4), 4 * v9);
        if ( Sizea < *(_DWORD *)Size )
          Sizea = *(_DWORD *)Size;
        v18[2] = v10;
        Size = *(_DWORD *)(Size + 8);
        v18 = v10;
      }
      while ( Size );
      result = 0;
      v6 = (int)a3;
    }
    *(_DWORD *)(v6 + 20) = Sizea + 1;
    v19 = (_DWORD *)(v6 + 32);
    v20 = a6;
    if ( a6 )
    {
      do
      {
        v12 = operator new(0x34u);
        qmemcpy(v12, v20, 0x34u);
        v12[11] = 0;
        v24 = v12 + 11;
        v12[10] = 0;
        v12[12] = 0;
        v13 = v20[11];
        v17 = v12;
        if ( v13 )
        {
          Sizeb = 28 * v9;
          do
          {
            v14 = operator new(0x20u);
            *v14 = *(_DWORD *)v13;
            v14[1] = *(_DWORD *)(v13 + 4);
            v14[2] = *(_DWORD *)(v13 + 8);
            v14[3] = *(_DWORD *)(v13 + 12);
            v14[4] = *(_DWORD *)(v13 + 16);
            v15 = operator new(28 * v9);
            v14[5] = v15;
            v14[7] = 0;
            memcpy_0(v15, *(const void **)(v13 + 20), Sizeb);
            if ( *(_DWORD *)(v13 + 24) )
            {
              v16 = operator new(28 * v21);
              v14[6] = v16;
              memcpy_0(v16, *(const void **)(v13 + 24), Sizeb);
            }
            else
            {
              v14[6] = 0;
            }
            *v24 = v14;
            v13 = *(_DWORD *)(v13 + 28);
            v24 = v14 + 7;
            v9 = v21;
          }
          while ( v13 );
          v12 = v17;
        }
        v19[12] = v12;
        v19 = v12;
        v20 = (_DWORD *)v20[12];
      }
      while ( v20 );
      return 0;
    }
  }
  return result;
}

// ===== sub_453060 @ 0x00453060..0x004530D6 =====
int __userpurge sub_453060@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3, void *Src)
{
  int v6; // esi
  void *v7; // eax

  if ( !a1 || !a3 )
    return -2147483647;
  sub_453E90();
  (*(void (__thiscall **)(_DWORD *))(*a2 + 48))(a2);
  a2[2] = a1;
  v6 = a3 * a1;
  a2[3] = a3;
  v7 = operator new(16 * v6);
  a2[4] = v7;
  memcpy_0(v7, Src, 16 * v6);
  (*(void (__thiscall **)(_DWORD *))(*a2 + 44))(a2);
  return 0;
}

// ===== sub_4530E0 @ 0x004530E0..0x00453105 =====
int __userpurge sub_4530E0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<edx>, _DWORD *a3)
{
  int v4; // ecx
  int v5; // eax

  if ( !a1[4] )
    return -2147483646;
  v4 = a1[2];
  v5 = a1[3];
  *a2 = v4;
  *a3 = v5;
  return 0;
}

// ===== sub_453110 @ 0x00453110..0x00453184 =====
int __userpurge sub_453110@<eax>(_DWORD *a1@<esi>, _DWORD *a2, void *Src)
{
  _DWORD *v4; // edi
  void *v5; // eax

  if ( !a1[4] )
    return -2147483646;
  v4 = operator new(0xCu);
  *v4 = a1[5];
  v5 = operator new(4 * a1[3] * a1[2]);
  v4[1] = v5;
  v4[2] = a1[7];
  memcpy_0(v5, Src, 4 * a1[3] * a1[2]);
  ++a1[5];
  a1[7] = v4;
  *a2 = *v4;
  return 0;
}

// ===== sub_453190 @ 0x00453190..0x0045320C =====
int __userpurge sub_453190@<eax>(_DWORD *a1@<edi>, _DWORD *a2)
{
  _DWORD *v3; // esi

  if ( !a1[4] )
    return -2147483646;
  v3 = operator new(0x34u);
  memset(v3, 0, 0x34u);
  *v3 = a1[8];
  v3[1] = -1;
  v3[2] = -1;
  v3[3] = 0;
  v3[4] = 3;
  v3[5] = 128;
  v3[8] = 0;
  v3[9] = 1;
  v3[10] = 0;
  v3[12] = a1[20];
  ++a1[8];
  a1[20] = v3;
  *a2 = *v3;
  return 0;
}

// ===== sub_453210 @ 0x00453210..0x0045325B =====
int __fastcall sub_453210(int a1, int a2)
{
  _DWORD *v2; // esi
  int result; // eax
  _DWORD *v4; // edi
  void (__thiscall ***v5)(_DWORD, int); // ecx

  v2 = *(_DWORD **)(a2 + 80);
  result = -2147483645;
  v4 = (_DWORD *)(a2 + 32);
  if ( v2 )
  {
    while ( a1 != *v2 )
    {
      v4 = v2;
      v2 = (_DWORD *)v2[12];
      if ( !v2 )
        return result;
    }
    sub_453EF0();
    v4[12] = v2[12];
    v5 = (void (__thiscall ***)(_DWORD, int))v2[10];
    if ( v5 )
      (**v5)(v5, 1);
    operator delete(v2);
    return 0;
  }
  return result;
}

// ===== sub_453260 @ 0x00453260..0x0045328D =====
int sub_453260()
{
  int v0; // eax
  int v1; // esi
  void (__thiscall ***v2)(_DWORD, int); // ecx

  v0 = sub_453E70();
  v1 = v0;
  if ( !v0 )
    return -2147483645;
  v2 = *(void (__thiscall ****)(_DWORD, int))(v0 + 40);
  if ( v2 )
    (**v2)(v2, 1);
  *(_DWORD *)(v1 + 40) = 0;
  return 0;
}

// ===== sub_453290 @ 0x00453290..0x004532C2 =====
int __usercall sub_453290@<eax>(int a1@<edi>, int a2@<esi>)
{
  int v2; // eax
  int v3; // edx

  v2 = sub_453E70();
  if ( !v2 )
    return -2147483645;
  if ( a1 < 0 || a1 >= *(_DWORD *)(v3 + 8) || a2 < 0 || a2 >= *(_DWORD *)(v3 + 12) )
    return -2147483644;
  *(_DWORD *)(v2 + 4) = a1;
  *(_DWORD *)(v2 + 8) = a2;
  return 0;
}

// ===== sub_4532D0 @ 0x004532D0..0x004532F9 =====
int __stdcall sub_4532D0(int a1)
{
  int v1; // eax
  _DWORD *v2; // edx

  v1 = sub_453E70();
  if ( !v1 )
    return -2147483645;
  *v2 = *(_DWORD *)(v1 + 4);
  v2[1] = *(_DWORD *)(v1 + 8);
  return 0;
}

// ===== sub_453300 @ 0x00453300..0x00453331 =====
int __usercall sub_453300@<eax>(int a1@<edi>)
{
  int v1; // edx
  int v2; // esi

  v2 = sub_453E70();
  if ( !v2 )
    return -2147483645;
  if ( !(*(int (__thiscall **)(int, int))(*(_DWORD *)v1 + 24))(v1, a1) )
    return -2147483637;
  *(_DWORD *)(v2 + 12) = a1;
  return 0;
}

// ===== sub_453340 @ 0x00453340..0x00453363 =====
int __stdcall sub_453340(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_453E70();
  if ( !v1 )
    return -2147483645;
  *a1 = *(_DWORD *)(v1 + 12);
  return 0;
}

// ===== sub_453370 @ 0x00453370..0x004533AE =====
int __userpurge sub_453370@<eax>(int a1@<esi>, int a2)
{
  int v2; // eax
  int v3; // edx

  v2 = sub_453E70();
  if ( !v2 )
    return -2147483645;
  if ( a1 < 0 )
    return -2147483639;
  if ( v3 < 0 )
    return -2147483638;
  *(_DWORD *)(v2 + 16) = a1;
  *(_DWORD *)(v2 + 20) = v3;
  return 0;
}

// ===== sub_4533B0 @ 0x004533B0..0x004533D9 =====
int __stdcall sub_4533B0(int a1)
{
  int v1; // eax
  _DWORD *v2; // edx

  v1 = sub_453E70();
  if ( !v1 )
    return -2147483645;
  *(_DWORD *)(v1 + 24) = *v2;
  *(_DWORD *)(v1 + 28) = v2[1];
  return 0;
}

// ===== sub_4533E0 @ 0x004533E0..0x0045340E =====
int __usercall sub_4533E0@<eax>(int a1@<edi>)
{
  int v1; // esi
  int v2; // edx

  v1 = sub_453E70();
  if ( !v1 )
    return -2147483645;
  if ( !sub_453F40(a1) )
    return -2147483643;
  *(_DWORD *)(v1 + 32) = v2;
  return 0;
}

// ===== sub_453410 @ 0x00453410..0x00453493 =====
int __userpurge sub_453410@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5, int a6)
{
  int v7; // eax
  _DWORD *v8; // edi
  void (__thiscall ***v9)(_DWORD, int); // ecx
  int v10; // eax
  int result; // eax
  int v12; // [esp-8h] [ebp-14h]

  v7 = sub_453E70();
  v8 = (_DWORD *)v7;
  if ( !v7 )
    return -2147483645;
  v9 = *(void (__thiscall ****)(_DWORD, int))(v7 + 40);
  if ( v9 )
    (**v9)(v9, 1);
  v10 = (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 36))(a1);
  v12 = v8[8];
  v8[10] = v10;
  sub_453F60(v10, v12, a6);
  result = sub_4575C0(v8[10], v8[1], v8[2], a2, a3, a4, a5);
  if ( result )
    return -1;
  return result;
}

// ===== sub_4534A0 @ 0x004534A0..0x0045350B =====
int __stdcall sub_4534A0(int a1, int a2, int a3, int a4)
{
  int v4; // eax
  int result; // eax

  v4 = sub_453E70();
  if ( !v4 )
    return -2147483645;
  if ( !*(_DWORD *)(v4 + 40) )
    return -2147483642;
  result = (*(int (__thiscall **)(_DWORD, int, int, int, int, int))(**(_DWORD **)(v4 + 40) + 4))(
             *(_DWORD *)(v4 + 40),
             a1,
             a2,
             a3,
             a4,
             1);
  if ( !result )
    return 0;
  if ( result == -2147483645 )
    return -2147483642;
  if ( result != -2 )
    return -1;
  return result;
}

// ===== sub_453510 @ 0x00453510..0x0045358F =====
unsigned int __stdcall sub_453510(int a1, int a2, int a3, int a4)
{
  int v4; // eax
  unsigned int result; // eax

  v4 = sub_453E70();
  if ( !v4 )
    return -2147483645;
  if ( !*(_DWORD *)(v4 + 40) )
    return -2147483642;
  result = sub_457690(a1, a2);
  if ( result <= 0x80000005 )
  {
    if ( result >= 0x80000004 )
      return -2147483641;
    if ( !result )
      return 0;
    if ( result == -2147483645 )
      return -2147483642;
    return -1;
  }
  if ( result != -2 )
    return -1;
  return result;
}

// ===== sub_453590 @ 0x00453590..0x00453629 =====
unsigned int __userpurge sub_453590@<eax>(int a1@<eax>, int a2, int a3, int *a4, int a5, int a6)
{
  unsigned int result; // eax
  __int64 v8; // rax
  int v9; // edi
  void *v10; // ebx

  result = sub_453510(0, (int)a4, a5, a6);
  if ( !result && a3 && *a4 >= 1 )
  {
    v8 = sub_453E70(a1);
    v9 = v8;
    v10 = operator new(4 * HIDWORD(v8));
    sub_457690(v10, a4);
    sub_457790(*(_DWORD *)(v9 + 40), a3, v10);
    operator delete(v10);
    return 0;
  }
  return result;
}

// ===== sub_453630 @ 0x00453630..0x00453693 =====
unsigned int __thiscall sub_453630(void *this, int a2)
{
  int v2; // eax
  unsigned int result; // eax

  v2 = sub_453E70(this);
  if ( !v2 )
    return -2147483645;
  if ( !*(_DWORD *)(v2 + 40) )
    return -2147483642;
  result = sub_4577F0(a2);
  if ( result > 0x80000003 )
  {
    if ( result == -2147483644 || result == -2147483643 )
      return -2147483641;
    return -1;
  }
  if ( result == -2147483645 )
    return -2147483642;
  if ( result )
    return -1;
  return result;
}

// ===== sub_4536A0 @ 0x004536A0..0x00453708 =====
unsigned int __thiscall sub_4536A0(void *this, int a2, int a3)
{
  int v3; // eax
  unsigned int result; // eax

  v3 = sub_453E70(this);
  if ( !v3 )
    return -2147483645;
  if ( !*(_DWORD *)(v3 + 40) )
    return -2147483642;
  result = sub_457860(a2);
  if ( result > 0x80000003 )
  {
    if ( result == -2147483644 || result == -2147483643 )
      return -2147483641;
    return -1;
  }
  if ( result == -2147483645 )
    return -2147483642;
  if ( result )
    return -1;
  return result;
}

// ===== sub_453710 @ 0x00453710..0x0045375F =====
int __thiscall sub_453710(void *this, void *a2)
{
  int v2; // eax
  int v3; // eax

  v2 = sub_453E70(this);
  if ( !v2 )
    return -2147483645;
  if ( !*(_DWORD *)(v2 + 40) )
    return -2147483642;
  v3 = sub_4578C0(a2);
  if ( !v3 )
    return 0;
  if ( v3 == -2147483645 )
    return -2147483642;
  return -1;
}

// ===== sub_453760 @ 0x00453760..0x004537B5 =====
int __thiscall sub_453760(void *this, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // eax

  v4 = sub_453E70(this);
  if ( !v4 )
    return -2147483645;
  if ( !*(_DWORD *)(v4 + 40) )
    return -2147483642;
  v5 = sub_457900(a2, a3, a4);
  if ( !v5 )
    return 0;
  if ( v5 == -2147483645 )
    return -2147483642;
  return -1;
}

// ===== sub_4537C0 @ 0x004537C0..0x004537FE =====
int __thiscall sub_4537C0(void *this, int a2, int a3, int a4)
{
  __int64 v4; // rax

  v4 = sub_453E70(this);
  if ( (_DWORD)v4 )
    return (*(int (__thiscall **)(_DWORD, int, int, int, _DWORD, _DWORD, _DWORD))(*(_DWORD *)HIDWORD(v4) + 56))(
             HIDWORD(v4),
             a2,
             a3,
             a4,
             *(_DWORD *)(v4 + 4),
             *(_DWORD *)(v4 + 8),
             *(_DWORD *)(v4 + 12));
  else
    return -2147483645;
}

// ===== sub_453800 @ 0x00453800..0x00453D72 =====
int __stdcall sub_453800(_DWORD *a1, void *a2, void *a3, int a4, int a5, char *a6, int a7, int a8)
{
  _DWORD *v9; // esi
  int v10; // eax
  void *v11; // edi
  int v12; // edx
  char **v14; // ecx
  char *v15; // esi
  char *i; // ecx
  char *v17; // edi
  unsigned int v18; // edx
  char *j; // eax
  int v20; // eax
  int v21; // edi
  char *v22; // edi
  char *v23; // esi
  int v24; // edx
  int v25; // edi
  void *v26; // eax
  bool v27; // zf
  _DWORD *v28; // eax
  _DWORD *v29; // esi
  unsigned int v30; // ecx
  _DWORD *v31; // eax
  int v32; // eax
  _DWORD *v33; // ecx
  int v34; // edx
  int v35; // eax
  int v36; // edx
  int v37; // eax
  int v38; // eax
  _DWORD *v39; // ecx
  unsigned int k; // edi
  _DWORD *v41; // edi
  void *v42; // eax
  void *v43; // eax
  _DWORD *v44; // eax
  int v46; // [esp-1Ch] [ebp-6Ch]
  void *v47; // [esp-10h] [ebp-60h]
  size_t v48; // [esp-Ch] [ebp-5Ch]
  void *v49; // [esp-Ch] [ebp-5Ch]
  size_t v50; // [esp-8h] [ebp-58h]
  int v51; // [esp+Ch] [ebp-44h] BYREF
  char **v52; // [esp+10h] [ebp-40h]
  void *v53; // [esp+14h] [ebp-3Ch]
  int v54; // [esp+18h] [ebp-38h]
  _DWORD *v55; // [esp+1Ch] [ebp-34h]
  size_t Size; // [esp+20h] [ebp-30h]
  unsigned int v57; // [esp+24h] [ebp-2Ch] BYREF
  unsigned int v58; // [esp+28h] [ebp-28h]
  int v59; // [esp+2Ch] [ebp-24h]
  _DWORD *v60; // [esp+30h] [ebp-20h]
  char *v61; // [esp+34h] [ebp-1Ch]
  void *v62; // [esp+38h] [ebp-18h]
  void *Src; // [esp+3Ch] [ebp-14h]
  unsigned int v64; // [esp+40h] [ebp-10h] BYREF
  void *v65; // [esp+44h] [ebp-Ch]
  int v66; // [esp+48h] [ebp-8h]
  void *v67; // [esp+4Ch] [ebp-4h]
  unsigned int v68; // [esp+58h] [ebp+8h]
  _DWORD *v69; // [esp+60h] [ebp+10h]
  char *v70; // [esp+68h] [ebp+18h]
  int v71; // [esp+6Ch] [ebp+1Ch]
  char *v72; // [esp+6Ch] [ebp+1Ch]

  v9 = (_DWORD *)sub_453E70(a2);
  if ( !v9 )
    return -2147483645;
  sub_453EF0();
  v10 = a4;
  if ( a8 )
    v10 = 2 * a4;
  sub_453410((int)a1, (int)a3, v10, -1, -1, a7);
  v68 = a1[2] * a1[3];
  v69 = operator new(8 * a1[2] * a1[3]);
  v53 = operator new(8 * v68);
  v67 = operator new(4 * v68);
  v65 = operator new(4 * v68);
  Src = operator new(28 * v68);
  v11 = v69;
  v62 = operator new(28 * v68);
  sub_453760(a2, (int)(v69 + 2), (int)&v64, 0);
  v12 = v9[1];
  ++v64;
  *v69 = v12;
  v69[1] = v9[2];
  v14 = (char **)(v9 + 11);
  v52 = (char **)(v9 + 11);
  v55 = v9 + 11;
  if ( a5 )
  {
    v15 = a6;
    v70 = a6;
    v54 = a5;
    while ( 1 )
    {
      for ( i = *v14; i; i = (char *)*((_DWORD *)i + 7) )
      {
        v17 = (char *)(v15 - i);
        v18 = 20;
        for ( j = i; ; j += 4 )
        {
          if ( v18 < 4 )
            goto LABEL_50;
          if ( *(_DWORD *)&v17[(_DWORD)j] != *(_DWORD *)j )
            break;
          v17 = (char *)(v15 - i);
          v18 -= 4;
        }
      }
      Size = 28 * v68;
      memset(Src, 0, 28 * v68);
      memset(v62, 0, 28 * v68);
      memset(v67, 0, 4 * v68);
      memset(v65, 0, 4 * v68);
      v20 = 0;
      v66 = 0;
      if ( v64 )
      {
        while ( 1 )
        {
          v21 = v69[2 * v20 + 1];
          v71 = v69[2 * v20];
          sub_4536A0(a2, (int)&v51, v71);
          if ( v51 <= a4 )
          {
            v60 = Src;
          }
          else
          {
            if ( !a8 || *(_DWORD *)v15 || v51 > 2 * a4 )
              goto LABEL_42;
            v60 = v62;
          }
          v46 = v21;
          v22 = (char *)v53;
          (*(void (__thiscall **)(_DWORD *, void *, _DWORD, unsigned int *, int, int, _DWORD, _DWORD, _DWORD, _DWORD, bool, int))(*a1 + 4))(
            a1,
            v53,
            0,
            &v57,
            v71,
            v46,
            *((_DWORD *)v15 + 1),
            *((_DWORD *)v15 + 2),
            *((_DWORD *)v15 + 3),
            0,
            a7 == 0,
            1);
          v58 = 0;
          if ( v57 )
          {
            v23 = v22;
            v72 = v22;
            do
            {
              v24 = *((_DWORD *)v70 + 4);
              v25 = *(_DWORD *)v23 + *((_DWORD *)v23 + 1) * a1[2];
              if ( v24 < 1 )
              {
                if ( !v24 )
                {
                  v38 = (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD, _DWORD, _DWORD))(*a1 + 12))(
                          a1,
                          v69[2 * v66],
                          v69[2 * v66 + 1],
                          *(_DWORD *)v23,
                          *((_DWORD *)v23 + 1));
                  v39 = v60;
                  ++v60[7 * v25 - 1 + v38];
                  v39[7 * v25] = 1;
                }
              }
              else
              {
                if ( !*((_DWORD *)v67 + v25) )
                {
                  v26 = operator new(8 * v68);
                  *((_DWORD *)v67 + v25) = v26;
                  (*(void (__thiscall **)(_DWORD *, void *, _DWORD, char *, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, int))(*a1 + 4))(
                    a1,
                    v26,
                    0,
                    (char *)v65 + 4 * v25,
                    *(_DWORD *)v23,
                    *((_DWORD *)v23 + 1),
                    *((_DWORD *)v70 + 4),
                    0,
                    0,
                    0,
                    0,
                    1);
                }
                v27 = *((_DWORD *)v65 + v25) == 0;
                v28 = (_DWORD *)*((_DWORD *)v67 + v25);
                v59 = 0;
                if ( !v27 )
                {
                  v29 = v28;
                  v61 = (char *)(v72 - (char *)v28);
                  do
                  {
                    v30 = 8;
                    v31 = v29;
                    while ( *(_DWORD *)&v61[(_DWORD)v31] == *v31 )
                    {
                      v30 -= 4;
                      ++v31;
                      if ( v30 < 4 )
                      {
                        v32 = (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD, _DWORD, _DWORD))(*a1 + 12))(
                                a1,
                                v69[2 * v66],
                                v69[2 * v66 + 1],
                                *(_DWORD *)v72,
                                *((_DWORD *)v72 + 1));
                        goto LABEL_34;
                      }
                    }
                    v32 = (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD, _DWORD, _DWORD))(*a1 + 12))(
                            a1,
                            *(_DWORD *)v72,
                            *((_DWORD *)v72 + 1),
                            *v29,
                            v29[1]);
LABEL_34:
                    v33 = v60;
                    v34 = v32 - 2 + 7 * (*v29 + a1[2] * v29[1]);
                    ++v60[v34 + 1];
                    v61 -= 8;
                    v35 = *v29 + a1[2] * v29[1];
                    v29 += 2;
                    v36 = 7 * v35;
                    v37 = v59;
                    v33[v36] = 1;
                    v59 = v37 + 1;
                  }
                  while ( (unsigned int)(v37 + 1) < *((_DWORD *)v65 + v25) );
                  v23 = v72;
                }
              }
              v23 += 8;
              ++v58;
              v72 = v23;
            }
            while ( v58 < v57 );
            v15 = v70;
          }
LABEL_42:
          if ( ++v66 >= v64 )
            break;
          v20 = v66;
        }
      }
      for ( k = 0; k < v68; ++k )
        operator delete(*((void **)v67 + k));
      v41 = operator new(0x20u);
      *v41 = *(_DWORD *)v15;
      v41[1] = *((_DWORD *)v15 + 1);
      v41[2] = *((_DWORD *)v15 + 2);
      v41[3] = *((_DWORD *)v15 + 3);
      v41[4] = *((_DWORD *)v15 + 4);
      v42 = operator new(28 * v68);
      v48 = Size;
      v47 = Src;
      v41[5] = v42;
      v41[7] = 0;
      memcpy_0(v42, v47, v48);
      if ( !a8 || *(_DWORD *)v70 )
      {
        v41[6] = 0;
      }
      else
      {
        v43 = operator new(28 * v68);
        v50 = Size;
        v49 = v62;
        v41[6] = v43;
        memcpy_0(v43, v49, v50);
      }
      v44 = v55;
      v55 = v41 + 7;
      v15 = v70;
      *v44 = v41;
LABEL_50:
      v15 += 20;
      v27 = v54-- == 1;
      v70 = v15;
      if ( v27 )
        break;
      v14 = v52;
    }
    v11 = v69;
  }
  operator delete(v11);
  operator delete(v53);
  operator delete(v67);
  operator delete(v65);
  operator delete(Src);
  operator delete(v62);
  return 0;
}

// ===== sub_453D80 @ 0x00453D80..0x00453E29 =====
int __userpurge sub_453D80@<eax>(int a1@<ecx>, int a2@<edi>, int a3, void *a4, int a5)
{
  int v5; // eax
  _DWORD *v6; // ecx
  unsigned int v7; // edx
  _DWORD *i; // eax
  const void *v10; // ecx

  v5 = sub_453E70(a1);
  if ( !v5 )
    return -2147483645;
  v6 = *(_DWORD **)(v5 + 44);
  if ( v6 )
  {
    while ( 2 )
    {
      v7 = 20;
      for ( i = v6; ; ++i )
      {
        if ( v7 < 4 )
        {
          if ( a5 )
            v10 = (const void *)v6[6];
          else
            v10 = (const void *)v6[5];
          if ( !v10 )
            return -2147483634;
          memcpy_0(a4, v10, 28 * *(_DWORD *)(a3 + 8) * *(_DWORD *)(a3 + 12));
          return 0;
        }
        if ( *(_DWORD *)((char *)i + a2 - (_DWORD)v6) != *i )
          break;
        v7 -= 4;
      }
      v6 = (_DWORD *)v6[7];
      if ( v6 )
        continue;
      break;
    }
  }
  return -2147483635;
}

// ===== sub_453E30 @ 0x00453E30..0x00453E3F =====
BOOL __stdcall sub_453E30(int a1)
{
  return a1 == 0;
}

// ===== sub_453E40 @ 0x00453E40..0x00453E65 =====
_DWORD *__fastcall sub_453E40(int a1, int a2, int a3)
{
  _DWORD *result; // eax

  for ( result = *(_DWORD **)(a3 + 80); result; result = (_DWORD *)result[12] )
  {
    if ( a2 == result[1] && a1 == result[2] )
      break;
  }
  return result;
}

// ===== sub_453E70 @ 0x00453E70..0x00453E83 =====
_DWORD *__usercall sub_453E70@<eax>(int a1@<eax>, int a2@<ecx>)
{
  _DWORD *result; // eax

  for ( result = *(_DWORD **)(a1 + 80); result; result = (_DWORD *)result[12] )
  {
    if ( a2 == *result )
      break;
  }
  return result;
}

// ===== sub_453E90 @ 0x00453E90..0x00453EE3 =====
void __usercall sub_453E90(int a1@<edi>)
{
  _DWORD *v1; // esi
  void *v2; // ebx
  void *v3; // ecx
  void *v4; // [esp-8h] [ebp-8h]

  while ( *(_DWORD *)(a1 + 80) )
    sub_453210(**(_DWORD **)(a1 + 80), a1);
  v1 = *(_DWORD **)(a1 + 28);
  while ( v1 )
  {
    v2 = v1;
    v3 = (void *)v1[1];
    v1 = (_DWORD *)v1[2];
    operator delete(v3);
    operator delete(v2);
  }
  v4 = *(void **)(a1 + 16);
  *(_DWORD *)(a1 + 28) = 0;
  operator delete(v4);
  *(_DWORD *)(a1 + 16) = 0;
}

// ===== sub_453EF0 @ 0x00453EF0..0x00453F39 =====
int __usercall sub_453EF0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  _DWORD *v2; // eax
  _DWORD *v3; // ebx
  int v4; // esi
  void **v5; // edi
  void *v6; // eax
  int result; // eax

  v2 = sub_453E70(a1, a2);
  v3 = v2;
  if ( !v2 )
    return -2147483645;
  v4 = v2[11];
  while ( v4 )
  {
    v5 = (void **)v4;
    v6 = *(void **)(v4 + 20);
    v4 = *(_DWORD *)(v4 + 28);
    operator delete(v6);
    operator delete(v5[6]);
    operator delete(v5);
  }
  result = 0;
  v3[11] = 0;
  return result;
}

// ===== sub_453F40 @ 0x00453F40..0x00453F60 =====
int __fastcall sub_453F40(int a1, int a2)
{
  _DWORD *v2; // ecx
  int result; // eax

  v2 = *(_DWORD **)(a1 + 28);
  result = 0;
  if ( v2 )
  {
    while ( a2 != *v2 )
    {
      v2 = (_DWORD *)v2[2];
      if ( !v2 )
        return result;
    }
    return v2[1];
  }
  return result;
}

// ===== sub_453F60 @ 0x00453F60..0x00454066 =====
int __userpurge sub_453F60@<eax>(int a1@<esi>, int a2, int a3, int a4)
{
  char *v5; // ebx
  int v6; // edi
  int v7; // edx
  int v8; // eax
  _DWORD *v9; // ecx
  int v10; // ecx
  _DWORD *i; // edi
  int v12; // eax
  char *v13; // [esp+0h] [ebp-4h]
  int v14; // [esp+10h] [ebp+Ch]

  if ( !*(_DWORD *)(a1 + 16) )
    return -2147483646;
  v5 = (char *)operator new(16 * *(_DWORD *)(a1 + 12) * *(_DWORD *)(a1 + 8));
  v13 = v5;
  v6 = sub_453F40(a1, a3);
  memcpy_0(v5, *(const void **)(a1 + 16), 16 * *(_DWORD *)(a1 + 12) * *(_DWORD *)(a1 + 8));
  if ( v6 )
  {
    v14 = 0;
    if ( *(int *)(a1 + 12) > 0 )
    {
      v7 = *(_DWORD *)(a1 + 8);
      do
      {
        v8 = 0;
        if ( v7 > 0 )
        {
          v9 = v5 + 4;
          do
          {
            *v9 += *(_DWORD *)(v6 + 4 * v8);
            v7 = *(_DWORD *)(a1 + 8);
            ++v8;
            v9 += 4;
          }
          while ( v8 < v7 );
        }
        v10 = *(_DWORD *)(a1 + 8);
        v6 += 4 * v10;
        v5 += 16 * v10;
        ++v14;
      }
      while ( v14 < *(_DWORD *)(a1 + 12) );
      v5 = v13;
    }
  }
  if ( a4 )
  {
    for ( i = *(_DWORD **)(a1 + 80); i; i = (_DWORD *)i[12] )
    {
      v12 = 2 * (i[1] + *(_DWORD *)(a1 + 8) * i[2]);
      *(_DWORD *)&v5[8 * v12 + 8] |= 1u;
      (*(void (__thiscall **)(int, char *, _DWORD *))(*(_DWORD *)a1 + 28))(a1, v5, i);
    }
  }
  sub_457540(*(_DWORD *)(a1 + 12), v5);
  operator delete(v5);
  return 0;
}
