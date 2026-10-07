#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_434130 @ 0x00434130..0x00434147 =====
_DWORD *__stdcall sub_434130(_DWORD *a1)
{
  _DWORD *result; // eax

  result = a1;
  *a1 = 0;
  a1[1] = 0;
  return result;
}

// ===== sub_434150 @ 0x00434150..0x0043419F =====
int __thiscall sub_434150(void *this, int *a2)
{
  int result; // eax
  int v3; // ecx
  int v4; // ecx
  int v5; // eax

  result = (int)this;
  v3 = *a2;
  if ( *a2 > 257 )
  {
    if ( v3 != 258 )
      return result;
    goto LABEL_8;
  }
  if ( *a2 == 257 )
    return sub_4335E0(result, a2[1]);
  v4 = v3 - 1;
  if ( !v4 )
  {
LABEL_8:
    v5 = sub_4337E0(result);
    return sub_4335D0(v5, 1);
  }
  if ( v4 == 255 )
    return sub_4337E0(result);
  return result;
}

// ===== sub_4341A0 @ 0x004341A0..0x004341C9 =====
int __thiscall sub_4341A0(int this)
{
  int v2; // eax
  unsigned int v3; // eax
  unsigned int v4; // ecx

  v2 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
  v3 = sub_443160(v2);
  if ( v3 > v4 )
    return 0;
  else
    return sub_431AE0((_DWORD *)this);
}

// ===== sub_4341D0 @ 0x004341D0..0x004342A1 =====
int __fastcall sub_4341D0(int a1, int a2, int a3)
{
  __int64 v3; // rax

  sub_432BC0(a1, a2, (_DWORD *)a3);
  *(_DWORD *)a3 = &CProcDspMsgEx::`vftable';
  memset((void *)(a3 + 124), 0, 0x48u);
  *(_DWORD *)(a3 + 200) = dword_507674;
  *(_DWORD *)(a3 + 204) = dword_507678;
  *(_DWORD *)(a3 + 208) = dword_50767C;
  *(_DWORD *)(a3 + 212) = dword_507680;
  *(_DWORD *)(a3 + 216) = dword_507684;
  *(_DWORD *)(a3 + 224) = 1;
  v3 = sub_431A40();
  *(_QWORD *)(a3 + 232) = v3;
  *(_QWORD *)(a3 + 240) = v3 << 16;
  return a3;
}

// ===== sub_4342B0 @ 0x004342B0..0x004342D1 =====
void *__thiscall sub_4342B0(void *this, char a2)
{
  sub_4342E0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4342E0 @ 0x004342E0..0x00434340 =====
int __thiscall sub_4342E0(_DWORD *this)
{
  *this = &CProcDspMsgEx::`vftable';
  sub_437030(this + 31);
  return sub_432D00(this);
}

// ===== sub_434340 @ 0x00434340..0x004343A1 =====
int __usercall sub_434340@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  if ( (unsigned int)(a1 - 25) > 0x4B )
    return -2147483647;
  if ( a2 < 0 )
    return -2147483646;
  dword_507638 = a3;
  dword_50763C = a4;
  if ( !a4 )
    dword_50763C = 1;
  dword_507640 = a1;
  dword_565CF0 = a6;
  dword_565BB0 = a5;
  dword_565BDC = a2;
  return 0;
}

// ===== sub_4343B0 @ 0x004343B0..0x00434401 =====
int __fastcall sub_4343B0(int a1, unsigned int a2)
{
  int result; // eax

  result = -2147483641;
  if ( a2 > 0x80000001 )
  {
    if ( a2 == -2147483646 )
    {
      dword_565D30 = a1;
      return 0;
    }
  }
  else if ( a2 == -2147483647 )
  {
    if ( a1 < 0 )
    {
      return -2147483640;
    }
    else
    {
      dword_507654 = a1;
      return 0;
    }
  }
  else if ( a2 )
  {
    if ( a2 == 0x80000000 )
    {
      dword_565CF4 = a1;
      return 0;
    }
  }
  else
  {
    dword_50764C = a1;
    return 0;
  }
  return result;
}

// ===== sub_434410 @ 0x00434410..0x00434435 =====
int __cdecl sub_434410(int a1)
{
  _DWORD *v1; // ecx
  int result; // eax

  result = -2147483641;
  if ( a1 == 256 )
  {
    *v1 = dword_565D34;
    v1[1] = dword_565D38;
    return 0;
  }
  return result;
}

// ===== sub_434440 @ 0x00434440..0x004344C8 =====
int __usercall sub_434440@<eax>(const char *a1@<eax>, int a2, int a3, int a4, int a5, int a6, int a7)
{
  CHAR *v8; // edx
  char v9; // cl

  if ( a1 )
  {
    if ( strlen(a1) >= 0x100 )
      return -2147483645;
    v8 = (CHAR *)(byte_565BE0 - a1);
    do
    {
      v9 = *a1;
      a1[(_DWORD)v8] = *a1;
      ++a1;
    }
    while ( v9 );
  }
  else
  {
    memset(byte_565BE0, 0, sizeof(byte_565BE0));
  }
  dword_565CE0 = a2;
  dword_565CE4 = a3;
  dword_565CE8 = a4;
  dword_565CEC = a5;
  dword_507644 = a6;
  dword_507648 = a7;
  return 0;
}

// ===== sub_4344D0 @ 0x004344D0..0x004344D6 =====
int sub_4344D0()
{
  return dword_565BDC;
}

// ===== sub_4344E0 @ 0x004344E0..0x004344EC =====
int sub_4344E0()
{
  return sub_434500(&unk_565BB4);
}

// ===== sub_4344F0 @ 0x004344F0..0x004344FE =====
int __thiscall sub_4344F0(void *this)
{
  return sub_434A40(this, (int)&unk_565BB4);
}

// ===== sub_434500 @ 0x00434500..0x00434515 =====
int __usercall sub_434500@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_434520(a3, a2, a1, 0);
}

// ===== sub_434520 @ 0x00434520..0x00434711 =====
int __cdecl sub_434520(_DWORD *a1, const char *a2, const char *a3, int a4)
{
  _DWORD *v4; // ebx
  int v5; // esi
  signed int v6; // eax
  _DWORD *i; // edi
  int v8; // eax
  void *v9; // eax
  _BYTE *v10; // edx
  const char *v11; // ecx
  char v12; // al
  int result; // eax
  _DWORD *v14; // esi
  int v15; // eax
  _BYTE *v16; // eax
  const char *v17; // ecx
  _BYTE *v18; // edx
  char v19; // al
  int v20; // eax
  void *v21; // eax
  const char *v22; // ecx
  _BYTE *v23; // edx
  char v24; // al
  unsigned int v25; // [esp-8h] [ebp-14h]
  unsigned int v26; // [esp-8h] [ebp-14h]
  unsigned int v27; // [esp-8h] [ebp-14h]

  v4 = a1;
  if ( a4 || (v5 = a1[9]) == 0 )
  {
LABEL_5:
    v6 = strlen(a2) + 1;
    for ( i = (_DWORD *)a1[9]; i; i = (_DWORD *)i[9] )
    {
      if ( a4 )
      {
        if ( !i[7] )
          break;
      }
      else if ( v6 >= i[1] )
      {
        break;
      }
      v4 = i;
    }
    v14 = operator new(0x28u);
    v14[1] = strlen(a2) + 1;
    v15 = sub_434B50(0);
    v26 = v14[1];
    v14[2] = v15;
    v16 = operator new[](v26);
    v17 = a2;
    *v14 = v16;
    v18 = v16;
    do
    {
      v19 = *v17;
      *v18++ = *v17++;
    }
    while ( v19 );
    v14[4] = strlen(a3) + 1;
    v20 = sub_434B50(0);
    v27 = v14[4];
    v14[6] = v20;
    v14[3] = operator new[](v27);
    v21 = operator new[](4 * v14[6]);
    v22 = a3;
    v23 = (_BYTE *)v14[3];
    v14[5] = v21;
    do
    {
      v24 = *v22;
      *v23++ = *v22++;
    }
    while ( v24 );
    result = sub_434B50(v14[5]);
    v14[9] = i;
    v14[7] = a4;
    v14[8] = 0;
    v4[9] = v14;
  }
  else
  {
    while ( strcmp(a2, *(const char **)v5) )
    {
      v5 = *(_DWORD *)(v5 + 36);
      if ( !v5 )
        goto LABEL_5;
    }
    operator delete[](*(void **)(v5 + 12));
    operator delete[](*(void **)(v5 + 20));
    *(_DWORD *)(v5 + 16) = strlen(a3) + 1;
    v8 = sub_434B50(0);
    v25 = *(_DWORD *)(v5 + 16);
    *(_DWORD *)(v5 + 24) = v8;
    *(_DWORD *)(v5 + 12) = operator new[](v25);
    v9 = operator new[](4 * *(_DWORD *)(v5 + 24));
    v10 = *(_BYTE **)(v5 + 12);
    *(_DWORD *)(v5 + 20) = v9;
    v11 = a3;
    do
    {
      v12 = *v11;
      *v10++ = *v11++;
    }
    while ( v12 );
    return sub_434B50(*(_DWORD *)(v5 + 20));
  }
  return result;
}

// ===== sub_434730 @ 0x00434730..0x004347C3 =====
int __usercall sub_434730@<eax>(const char *a1@<eax>, int a2@<ecx>, int a3)
{
  int v3; // esi
  int v4; // edi

  v3 = *(_DWORD *)(a2 + 36);
  v4 = a2;
  if ( !v3 )
    return 0;
  while ( a3 && !*(_DWORD *)(v3 + 28) || strcmp(a1, *(const char **)v3) )
  {
    v4 = v3;
    v3 = *(_DWORD *)(v3 + 36);
    if ( !v3 )
      return 0;
  }
  *(_DWORD *)(v4 + 36) = *(_DWORD *)(v3 + 36);
  operator delete[](*(void **)v3);
  operator delete[](*(void **)(v3 + 12));
  operator delete[](*(void **)(v3 + 20));
  operator delete((void *)v3);
  return 1;
}

// ===== sub_4347D0 @ 0x004347D0..0x004347F8 =====
int __usercall sub_4347D0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 36);
  while ( result )
  {
    if ( *(_DWORD *)(result + 28) )
    {
      sub_434730(*(const char **)result, a1, 0);
      result = *(_DWORD *)(a1 + 36);
    }
    else
    {
      result = *(_DWORD *)(result + 36);
    }
  }
  return result;
}

// ===== sub_434800 @ 0x00434800..0x0043480D =====
int sub_434800()
{
  return sub_434810();
}

// ===== sub_434810 @ 0x00434810..0x0043483D =====
int __usercall sub_434810@<eax>(int a1@<esi>)
{
  int result; // eax

  for ( ; *(_DWORD *)(a1 + 36); result = sub_434730(**(const char ***)(a1 + 36), a1, 0) )
    ;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)a1 = 0;
  *(_DWORD *)(a1 + 16) = 0;
  *(_DWORD *)(a1 + 12) = 0;
  *(_DWORD *)(a1 + 36) = 0;
  return result;
}

// ===== sub_434840 @ 0x00434840..0x004348B1 =====
BOOL __usercall sub_434840@<eax>(int a1@<eax>, const char *a2@<ecx>, _DWORD *a3)
{
  int v3; // ebx

  v3 = *(_DWORD *)(a1 + 36);
  if ( v3 )
  {
    while ( strcmp(a2, *(const char **)v3) )
    {
      v3 = *(_DWORD *)(v3 + 36);
      if ( !v3 )
        return 0;
    }
    qmemcpy(a3, (const void *)v3, 0x28u);
    a3[9] = 0;
  }
  return v3 != 0;
}

// ===== sub_4348C0 @ 0x004348C0..0x00434912 =====
int __usercall sub_4348C0@<eax>(int a1@<eax>, const char *a2@<edi>, _BYTE *a3)
{
  int v3; // esi
  const char *v5; // ecx
  char v7; // al

  v3 = *(_DWORD *)(a1 + 36);
  if ( !v3 )
    return 0;
  while ( strstr(a2, *(const char **)v3) != a2 || *(_DWORD *)(v3 + 32) )
  {
    v3 = *(_DWORD *)(v3 + 36);
    if ( !v3 )
      return 0;
  }
  v5 = *(const char **)v3;
  do
  {
    v7 = *v5;
    *a3++ = *v5++;
  }
  while ( v7 );
  if ( *(_DWORD *)(v3 + 28) )
    ++*(_DWORD *)(v3 + 32);
  return 1;
}

// ===== sub_434920 @ 0x00434920..0x00434A33 =====
int __cdecl sub_434920(const char *a1)
{
  char *v1; // ecx
  int v2; // ebx
  const char *v3; // edi
  const char *v5; // [esp+10h] [ebp-440h]
  int v6; // [esp+14h] [ebp-43Ch]
  char *Buffer; // [esp+18h] [ebp-438h]
  int v8; // [esp+1Ch] [ebp-434h] BYREF
  const char *v9[10]; // [esp+20h] [ebp-430h] BYREF
  char v10[1028]; // [esp+48h] [ebp-408h] BYREF

  v2 = 0;
  v6 = 0;
  Buffer = v1;
  v3 = a1;
  v5 = a1;
  if ( !*a1 )
    return 0;
  do
  {
    v8 = sub_42EA10(&v8);
    if ( v2 > 0 )
    {
      --v2;
    }
    else
    {
      if ( sub_4348C0((int)&unk_565BB4, v3, v10) )
      {
        ++v6;
        sub_434840((int)&unk_565BB4, v10, v9);
        sprintf(Buffer, "%s\\%s\n", v9[0], v9[3]);
        Buffer += strlen(Buffer);
        v2 = sub_434B50(0) - 1;
      }
      v3 = v5;
    }
    v3 += (v8 != 0) + 1;
    v5 = v3;
  }
  while ( *v3 );
  return v6;
}

// ===== sub_434A40 @ 0x00434A40..0x00434B4A =====
BOOL __cdecl sub_434A40(int a1)
{
  const unsigned __int8 *Src; // ecx
  const unsigned __int8 *v2; // esi
  BOOL result; // eax
  unsigned __int8 *v4; // eax
  signed int v5; // eax
  signed int v6; // edi
  unsigned __int8 *v7; // eax
  unsigned __int8 *v8; // ebx
  signed int v9; // edi
  _BYTE v10[256]; // [esp+8h] [ebp-204h] BYREF
  _BYTE v11[256]; // [esp+108h] [ebp-104h] BYREF

  v2 = Src;
  result = Src != 0;
  if ( Src )
  {
    if ( *Src )
    {
      do
      {
        v4 = _mbsstr(v2, "\\");
        if ( v4 )
        {
          v5 = v4 - v2;
          v6 = v5;
          if ( v5 > 0 )
          {
            memcpy_0(v10, v2, v5);
            v2 += v6 + 1;
            v10[v6] = 0;
            v7 = _mbsstr(v2, "\n");
            v8 = v7;
            if ( v7 )
              v9 = v7 - v2;
            else
              v9 = strlen((const char *)v2);
            if ( v9 > 0 )
            {
              memcpy_0(v11, v2, v9);
              v11[v9] = 0;
              v2 += v9 + (v8 != 0);
              sub_434500((int)v11, (int)v10, a1);
            }
          }
        }
      }
      while ( *v2 );
    }
    return *v2 == 0;
  }
  return result;
}

// ===== sub_434B50 @ 0x00434B50..0x00434B8F =====
int __usercall sub_434B50@<eax>(_BYTE *a1@<eax>, int a2)
{
  _BYTE *v2; // edi
  int i; // ebx
  int v5; // [esp+8h] [ebp-4h] BYREF

  v2 = a1;
  for ( i = 0; *v2; ++i )
  {
    v2 += (sub_42EA10(&v5) != 0) + 1;
    if ( a2 )
      *(_DWORD *)(a2 + 4 * i) = v5;
  }
  return i;
}

// ===== sub_434B90 @ 0x00434B90..0x00434B95 =====
// attributes: thunk
int sub_434B90()
{
  return sub_434800();
}

// ===== sub_434BA0 @ 0x00434BA0..0x00434C48 =====
int __usercall sub_434BA0@<eax>(
        int a1@<eax>,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        void *Src,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14)
{
  int v14; // ebx
  void *v16; // esi
  int v17; // edi
  int v19[5]; // [esp+10h] [ebp-14h] BYREF

  v14 = a5;
  v16 = operator new[](strlen((const char *)a5) << 6);
  sub_409190(v19, a1);
  dword_565D34 = a3;
  dword_565D38 = a4;
  v17 = sub_434C50(
          a1,
          (int)&a3,
          (int)v16,
          a2,
          (int)&dword_565D34,
          (int)v19,
          v14,
          a6,
          Src,
          a9,
          a10,
          0,
          a11,
          a12,
          a13,
          a14);
  operator delete[](v16);
  return v17;
}

// ===== sub_434C50 @ 0x00434C50..0x00434D71 =====
int __usercall sub_434C50@<eax>(
        int a1@<eax>,
        int a2,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        void *Src,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15,
        int a16,
        int a17)
{
  int result; // eax
  int v19; // edi
  int v20; // ebx
  int v21; // eax
  _BYTE v22[32]; // [esp+Ch] [ebp-A4h] BYREF
  int v23; // [esp+2Ch] [ebp-84h]
  _BYTE v24[72]; // [esp+40h] [ebp-70h] BYREF
  int v25[10]; // [esp+88h] [ebp-28h] BYREF

  result = sub_4092B0((int)v22, dword_565B70);
  v19 = result;
  if ( result )
  {
    memset(v24, 0, sizeof(v24));
    memset(v25, 0, sizeof(v25));
    sub_434A40((int)v25);
    v20 = v23;
    v21 = v20 + sub_4097B0(v23, a14);
    sub_435290(v24, a8, a9, a6, a7, v21, a1, a11, a12, a15, a17);
    if ( a9 )
      sub_437110(v24, a16, v25);
    sub_437CB0(v24, a6, a7, a1, a12, a17, a13);
    *a3 = sub_437940(a2, a4);
    sub_437030(v24);
    sub_434810((int)v25);
    return v19;
  }
  return result;
}

// ===== sub_434D80 @ 0x00434D80..0x00434D9A =====
int __stdcall sub_434D80(int a1)
{
  return sub_434DA0(a1, 0, 1, 0);
}

// ===== sub_434DA0 @ 0x00434DA0..0x00434DC9 =====
int __userpurge sub_434DA0@<eax>(int a1@<esi>, int a2, int a3, int a4, int a5)
{
  sub_437030((void *)(a1 + 124));
  return sub_4350C0(a1, a2, a3, a4, a5);
}

// ===== sub_434DD0 @ 0x00434DD0..0x00434DE0 =====
int __stdcall sub_434DD0(int a1)
{
  return sub_434DE0(a1);
}

// ===== sub_434DE0 @ 0x00434DE0..0x00434DF6 =====
int __userpurge sub_434DE0@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3)
{
  int result; // eax
  int v4; // ecx

  result = sub_4335C0(a2, a1);
  *(_DWORD *)(v4 + 220) = a3;
  return result;
}

// ===== sub_434E00 @ 0x00434E00..0x00434E2C =====
int __userpurge sub_434E00@<eax>(int a1@<eax>, int a2)
{
  return sub_434E30(*(_DWORD *)(a1 + 12), *(_DWORD *)(a1 + 16));
}

// ===== sub_434E30 @ 0x00434E30..0x00434E84 =====
int __usercall sub_434E30@<eax>(
        unsigned int a1@<edx>,
        unsigned int *a2@<ecx>,
        unsigned int a3@<edi>,
        unsigned int a4@<esi>,
        unsigned int a5,
        unsigned int a6)
{
  int result; // eax

  result = 0;
  if ( a1 )
  {
    if ( a1 <= 2 && a3 <= 0x64 && a4 <= 0x64 && a6 <= 0x100 )
    {
      a2[3] = a5;
      *a2 = a1;
      a2[1] = a3;
      a2[2] = a4;
      a2[4] = a6;
      return 1;
    }
  }
  else
  {
    *a2 = 0;
    a2[1] = 0;
    a2[2] = 0;
    a2[3] = 0;
    a2[4] = 0;
    return 1;
  }
  return result;
}

// ===== sub_434E90 @ 0x00434E90..0x00434F00 =====
int __thiscall sub_434E90(unsigned int *this)
{
  if ( !dword_565CF4 )
    goto LABEL_8;
  if ( this[56] )
  {
    sub_431A10(this, 0);
    this[56] = 0;
  }
  if ( !dword_565CF4 )
  {
LABEL_8:
    if ( !sub_431A60(this) && !this[13] && !this[29] && !this[23] )
      return 0;
  }
  if ( !sub_437600(this) )
  {
    sub_437620();
    return 0;
  }
  return sub_433E40((int *)this);
}

// ===== sub_434F00 @ 0x00434F00..0x004350B5 =====
int __cdecl sub_434F00(_DWORD *a1, _BYTE *a2, int a3, int a4)
{
  int v4; // ebx
  int v5; // edi
  int v6; // esi
  void *v7; // eax
  int v8; // ebx
  BOOL v9; // ecx
  int v10; // eax
  int v11; // ecx
  int v12; // edi
  bool v13; // zf
  int v14; // edi
  int result; // eax
  int v16; // [esp+10h] [ebp-68h]
  int v17; // [esp+14h] [ebp-64h]
  _BYTE *v18; // [esp+18h] [ebp-60h]
  int v19; // [esp+1Ch] [ebp-5Ch]
  int v20; // [esp+20h] [ebp-58h]
  int v21; // [esp+24h] [ebp-54h]
  int v22; // [esp+28h] [ebp-50h] BYREF
  _DWORD v23[2]; // [esp+2Ch] [ebp-4Ch] BYREF
  void *v24; // [esp+34h] [ebp-44h]
  void *v25; // [esp+38h] [ebp-40h]
  void *v26; // [esp+3Ch] [ebp-3Ch]
  void *v27; // [esp+40h] [ebp-38h]
  void *v28[6]; // [esp+44h] [ebp-34h] BYREF
  int v29[7]; // [esp+5Ch] [ebp-1Ch] BYREF

  v4 = a3;
  sub_409080(v28, 1);
  v17 = sub_437080();
  v5 = 0;
  v6 = 0;
  v16 = 0;
  v21 = 0;
  v19 = 1;
  v18 = a2;
  if ( *a2 )
  {
    do
    {
      v24 = v28[2];
      v23[0] = v28[0];
      v23[1] = v28[1];
      v27 = v28[5];
      v25 = v28[3];
      v26 = v28[4];
      v20 = sub_42EA10(&v22);
      v7 = *(void **)(v4 + 48);
      v8 = v22;
      sub_433060(v29, (int)v23, v22, v7, 0xFFFFFF);
      if ( a4 )
      {
        sub_409A80(v23, (int)v29);
        v9 = sub_432E30(v8);
        if ( v9 )
          v6 = 0;
        else
          v6 = sub_409790(v17);
        if ( v9 )
          v10 = sub_433000(v9, v8);
        else
          v10 = (int)v24;
        v16 += v6 + v10;
        if ( v19 )
        {
          v21 = v6 >> 1;
          v19 = 0;
        }
      }
      else
      {
        v6 = 0;
        if ( sub_432E30(v8) )
        {
          v12 = sub_433000(v11, v8);
        }
        else if ( v20 )
        {
          v12 = v17;
        }
        else
        {
          v12 = v17 / 2;
        }
        v16 += v12 + sub_436FF0(0);
      }
      v4 = a3;
      v13 = v18[(v20 != 0) + 1] == 0;
      v18 += (v20 != 0) + 1;
    }
    while ( !v13 );
    v5 = v16;
  }
  operator delete[](v28[0]);
  v14 = v5 - sub_436FF0(a4);
  result = v14 + (v6 >> 1) - v6;
  *a1 = v14;
  a1[1] = v14 + (v6 >> 1) - v21 - v6;
  a1[2] = result;
  return result;
}

// ===== sub_4350C0 @ 0x004350C0..0x0043523F =====
int __stdcall sub_4350C0(int *a1, int a2, int a3, int a4, int a5)
{
  _DWORD *v5; // esi
  int v6; // edi
  unsigned int v7; // edx
  unsigned int v8; // eax
  unsigned int v9; // ecx
  unsigned int v10; // edx
  int result; // eax
  int v12; // eax
  int v13; // edx
  void (__thiscall *v14)(int *, _DWORD); // eax
  int v15; // [esp+68h] [ebp-40h]
  int v16; // [esp+6Ch] [ebp-3Ch] BYREF
  int v17[2]; // [esp+70h] [ebp-38h] BYREF
  int v18; // [esp+78h] [ebp-30h]
  int v19; // [esp+7Ch] [ebp-2Ch]
  unsigned int v20[5]; // [esp+80h] [ebp-28h] BYREF
  _DWORD v21[4]; // [esp+94h] [ebp-14h] BYREF

  v5 = (_DWORD *)a1[8];
  sub_42C720(v17, (int)v5);
  sub_42C2A0(v21, v5);
  v19 = sub_42C4C0(v5);
  v6 = sub_42C450((int)v5);
  v18 = v6;
  v16 = sub_42C520((int)v5);
  if ( a5 )
  {
    sub_434E30(0, v20, 0, 0, 0, 0);
    v6 = v18;
  }
  else
  {
    v7 = a1[51];
    v8 = a1[52];
    v20[0] = a1[50];
    v9 = a1[53];
    v20[1] = v7;
    v10 = a1[54];
    v20[2] = v8;
    v20[3] = v9;
    v20[4] = v10;
  }
  result = (*(int (__thiscall **)(int *, int *, int *, int, int, void *, int *, _DWORD *, int, int, int, int, int, unsigned int *))(*a1 + 52))(
             a1,
             a1 + 31,
             &v16,
             a2,
             a3,
             &unk_565BB4,
             v17,
             v21,
             v19,
             v6,
             v16,
             a4,
             a1[11],
             v20);
  v15 = result;
  if ( result )
  {
    if ( a3 )
      (*(void (__thiscall **)(int *, int *, int, int, unsigned int *, void *))(*a1 + 56))(
        a1,
        a1 + 31,
        v6,
        a1[55],
        v20,
        &unk_565BB4);
    v12 = sub_42C5A0(a1[8]);
    (*(void (__thiscall **)(int *, int *, int *, _DWORD *, int, int, unsigned int *, int))(v13 + 60))(
      a1,
      a1 + 31,
      v17,
      v21,
      v6,
      a4,
      v20,
      v12);
    sub_42C700(a1[8], v17[0], v17[1]);
    v14 = *(void (__thiscall **)(int *, _DWORD))(*a1 + 8);
    a1[49] = a3;
    v14(a1, 0);
    return v15;
  }
  return result;
}

// ===== sub_435240 @ 0x00435240..0x00435281 =====
int __stdcall sub_435240(
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
  return sub_435290(a1, a3, a4, a6, a7, a8, a9, a10, a11, a12, a13);
}

// ===== sub_435290 @ 0x00435290..0x00436F91 =====
int __fastcall sub_435290(
        _DWORD *a1,
        _DWORD *a2,
        void *a3,
        char *a4,
        int a5,
        int *a6,
        int *a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        _DWORD *a13)
{
  char *v13; // edi
  int result; // eax
  char v15; // al
  void *v16; // ecx
  int v17; // ecx
  int v18; // eax
  int v19; // esi
  int v20; // ecx
  int v21; // eax
  bool v22; // zf
  int v23; // edx
  _DWORD *v24; // ebx
  unsigned int v25; // kr10_4
  int v26; // esi
  unsigned int v27; // ecx
  unsigned __int8 v28; // al
  int v29; // eax
  int v30; // eax
  int v31; // ecx
  _DWORD *v32; // edx
  char *v33; // ecx
  unsigned int v34; // edi
  char v35; // al
  char *v36; // edx
  int v37; // ecx
  int *v38; // eax
  unsigned int v39; // esi
  char v40; // al
  char *v41; // eax
  int v42; // ebx
  int v43; // ecx
  const char *v44; // esi
  unsigned int v45; // ecx
  const char *v46; // edx
  char *v47; // esi
  int v48; // eax
  int v49; // eax
  char *v50; // edi
  _DWORD *v51; // eax
  void (__thiscall ***v52)(_DWORD, int); // esi
  _DWORD *v53; // eax
  _DWORD *v54; // eax
  _DWORD *v55; // eax
  char *v56; // ebx
  int v57; // eax
  int i; // eax
  _BYTE *v59; // edi
  int v60; // eax
  int v61; // esi
  char *v62; // ebx
  int v63; // eax
  int v64; // esi
  char v65; // bl
  char *v66; // edi
  int v67; // eax
  int v68; // esi
  char *v69; // ebx
  unsigned int v70; // ecx
  int v71; // ebx
  unsigned int j; // esi
  char *v73; // eax
  int v74; // edi
  void *v75; // esi
  int v76; // eax
  int v77; // edx
  int v78; // ebx
  int v79; // edi
  void *v80; // esi
  CHAR *v81; // edi
  int v82; // ebx
  _DWORD *v83; // eax
  int v84; // eax
  unsigned int v85; // esi
  size_t v86; // eax
  char *k; // ebx
  int v88; // edi
  char v89; // al
  char *m; // ebx
  char v91; // al
  _DWORD *v92; // esi
  int v93; // eax
  int v94; // ecx
  int v95; // eax
  const char *v96; // ebx
  BOOL v97; // ecx
  unsigned int v98; // esi
  int v99; // eax
  _BYTE *v100; // edi
  unsigned int v101; // ecx
  const char *v102; // edx
  unsigned int v103; // eax
  char *v104; // ecx
  unsigned int v105; // edx
  const char *v106; // edi
  int *v107; // ecx
  int v108; // edx
  int v109; // eax
  int v110; // ebx
  int v111; // esi
  int v112; // ecx
  int *v113; // esi
  int v114; // ecx
  int v115; // ecx
  void *v116; // ecx
  char *v117; // edx
  void *v118; // eax
  void *v119; // ecx
  void *v120; // edx
  int v121; // ecx
  int v122; // esi
  int v123; // ecx
  int v124; // edi
  char *v125; // eax
  int v126; // eax
  _DWORD *v127; // ebx
  int v128; // eax
  void *v129; // esi
  _DWORD *v130; // esi
  void *v131; // esi
  char *v132; // edi
  void *v133; // esi
  int v134; // eax
  int v135; // edi
  _BYTE *v136; // eax
  char *v137; // ecx
  _BYTE *v138; // edx
  char v139; // al
  int v140; // edi
  BOOL v141; // eax
  int v142; // edx
  char v143; // cl
  int *v144; // edi
  int v145; // ecx
  _DWORD *v146; // eax
  char *v147; // eax
  char *v148; // edx
  int v149; // eax
  int v150; // eax
  _DWORD *v151; // edx
  _DWORD *v152; // [esp-10h] [ebp-96Ch]
  int v153; // [esp-8h] [ebp-964h]
  void *v154; // [esp-4h] [ebp-960h]
  int v155; // [esp-4h] [ebp-960h]
  void *v156; // [esp+14h] [ebp-948h] BYREF
  void *v157; // [esp+18h] [ebp-944h]
  void *v158; // [esp+1Ch] [ebp-940h]
  CHAR *v159; // [esp+20h] [ebp-93Ch]
  int v160; // [esp+24h] [ebp-938h]
  char *v161; // [esp+28h] [ebp-934h]
  char v162; // [esp+2Ch] [ebp-930h] BYREF
  __int16 v163; // [esp+2Dh] [ebp-92Fh]
  unsigned int v164; // [esp+30h] [ebp-92Ch]
  int v165; // [esp+34h] [ebp-928h] BYREF
  int v166; // [esp+38h] [ebp-924h]
  int v167; // [esp+3Ch] [ebp-920h]
  void *v168; // [esp+40h] [ebp-91Ch]
  void *v169; // [esp+44h] [ebp-918h] BYREF
  unsigned __int8 *v170; // [esp+48h] [ebp-914h]
  int *v171; // [esp+4Ch] [ebp-910h]
  void *v172; // [esp+50h] [ebp-90Ch]
  int v173; // [esp+54h] [ebp-908h]
  int v174; // [esp+58h] [ebp-904h] BYREF
  int v175; // [esp+5Ch] [ebp-900h] BYREF
  int v176; // [esp+60h] [ebp-8FCh]
  int v177; // [esp+64h] [ebp-8F8h]
  int v178; // [esp+68h] [ebp-8F4h]
  int v179; // [esp+6Ch] [ebp-8F0h]
  int v180; // [esp+70h] [ebp-8ECh]
  int v181; // [esp+74h] [ebp-8E8h]
  _DWORD *v182; // [esp+78h] [ebp-8E4h]
  _DWORD *v183; // [esp+7Ch] [ebp-8E0h]
  int v184; // [esp+80h] [ebp-8DCh]
  _DWORD *v185; // [esp+84h] [ebp-8D8h]
  int v186; // [esp+88h] [ebp-8D4h]
  _BYTE *v187; // [esp+8Ch] [ebp-8D0h]
  int v188; // [esp+90h] [ebp-8CCh]
  int v189; // [esp+94h] [ebp-8C8h]
  int v190; // [esp+98h] [ebp-8C4h]
  void *v191; // [esp+9Ch] [ebp-8C0h] BYREF
  void *v192; // [esp+A0h] [ebp-8BCh]
  char *v193; // [esp+A4h] [ebp-8B8h]
  void *v194; // [esp+A8h] [ebp-8B4h]
  void *v195; // [esp+ACh] [ebp-8B0h]
  void *v196; // [esp+B0h] [ebp-8ACh]
  void *v197; // [esp+B4h] [ebp-8A8h]
  int v198; // [esp+B8h] [ebp-8A4h] BYREF
  int v199; // [esp+BCh] [ebp-8A0h]
  _DWORD *v200; // [esp+C0h] [ebp-89Ch]
  int v201; // [esp+C4h] [ebp-898h]
  int v202; // [esp+C8h] [ebp-894h]
  int v203; // [esp+CCh] [ebp-890h]
  void *v204[6]; // [esp+D0h] [ebp-88Ch] BYREF
  int v205; // [esp+E8h] [ebp-874h]
  int v206; // [esp+ECh] [ebp-870h]
  char *v207; // [esp+F0h] [ebp-86Ch]
  unsigned __int8 *v208; // [esp+F4h] [ebp-868h]
  int v209; // [esp+F8h] [ebp-864h]
  int v210; // [esp+FCh] [ebp-860h]
  int v211[2]; // [esp+100h] [ebp-85Ch] BYREF
  int v212; // [esp+108h] [ebp-854h]
  int v213; // [esp+10Ch] [ebp-850h]
  _DWORD v214[3]; // [esp+110h] [ebp-84Ch] BYREF
  void *v215[6]; // [esp+11Ch] [ebp-840h] BYREF
  int v216[6]; // [esp+134h] [ebp-828h] BYREF
  int v217[4]; // [esp+14Ch] [ebp-810h] BYREF
  void *v218; // [esp+15Ch] [ebp-800h]
  _DWORD v219[15]; // [esp+160h] [ebp-7FCh]
  int v220[5]; // [esp+19Ch] [ebp-7C0h] BYREF
  int v221; // [esp+1B0h] [ebp-7ACh]
  int v222[3]; // [esp+1B8h] [ebp-7A4h] BYREF
  void *v223; // [esp+1C4h] [ebp-798h] BYREF
  int v224[3]; // [esp+1D0h] [ebp-78Ch] BYREF
  CHAR pszFaceName[52]; // [esp+1DCh] [ebp-780h] BYREF
  _DWORD v226[13]; // [esp+210h] [ebp-74Ch] BYREF
  char String[256]; // [esp+244h] [ebp-718h] BYREF
  char Src[256]; // [esp+344h] [ebp-618h] BYREF
  char v229[256]; // [esp+444h] [ebp-518h] BYREF
  char v230; // [esp+544h] [ebp-418h] BYREF
  char v231; // [esp+545h] [ebp-417h]
  char v232; // [esp+546h] [ebp-416h]
  int v233; // [esp+958h] [ebp-4h]

  v13 = a4;
  v172 = a3;
  v183 = a1;
  v171 = a6;
  v200 = a2;
  v161 = a4;
  v182 = a13;
  result = sub_4092B0((int)v226, dword_565B70);
  v209 = result;
  if ( !result )
    return result;
  sub_437E30();
  v210 = sub_438060();
  v15 = *a4;
  v203 = (int)v16;
  v160 = (int)v16;
  v158 = v16;
  v17 = 1;
  if ( v15 < 32 )
  {
    switch ( v15 )
    {
      case 2:
        v203 = 1;
        v13 = a4 + 1;
        goto LABEL_7;
      case 3:
        v160 = 1;
        v13 = a4 + 1;
        goto LABEL_7;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        v158 = (void *)1;
        sub_42EA10((int *)&v156);
        v13 = a4 + 1;
LABEL_7:
        v161 = v13;
        break;
      default:
        break;
    }
  }
  v18 = sub_437080(v17);
  v19 = v226[8];
  v179 = v18;
  if ( a5 )
    v184 = sub_4370A0(v226[8]) - (dword_565D30 != 0 ? dword_565CEC : 0);
  else
    v184 = 0;
  v20 = v19 * a13[1] / 100;
  v181 = v20;
  if ( v20 <= 0 )
  {
    v20 = 1;
    v181 = 1;
  }
  v21 = v19 * a13[2] / 100;
  v186 = v21;
  if ( v21 <= 0 )
  {
    v21 = 1;
    v186 = 1;
  }
  v23 = *a13 - 1;
  v22 = *a13 == 1;
  v177 = 0;
  v189 = 0;
  if ( v22 )
  {
    v177 = v20;
    v189 = v21;
  }
  else if ( v23 == 1 )
  {
    v177 = 2 * v20;
    v189 = 2 * v21;
    if ( dword_565D30 )
      v184 += v21;
  }
  v24 = (_DWORD *)v226[12];
  sub_42E990(v226[12]);
  sub_42E9A0((int)v24);
  sub_409080(v215, 1);
  v25 = strlen(v13);
  v26 = 0;
  v197 = operator new[](v25 + 1);
  v176 = 0;
  v202 = 0;
  v198 = 0;
  if ( !a11 )
    goto LABEL_33;
  v202 = v226[8];
  if ( !dword_565CF0 || v160 )
    goto LABEL_33;
  if ( !sub_437AB0() )
  {
    if ( !v158 )
      goto LABEL_33;
    goto LABEL_27;
  }
  if ( v158 )
  {
LABEL_27:
    v27 = (unsigned int)v156 >> 8;
    v163 = (unsigned __int8)v156;
LABEL_31:
    v162 = v27;
    goto LABEL_32;
  }
  if ( !sub_42EA10((int *)&v169) )
  {
    LOBYTE(v27) = *v13;
    LOBYTE(v163) = 0;
    goto LABEL_31;
  }
  v28 = v13[1];
  v162 = *v13;
  v163 = v28;
LABEL_32:
  sub_434F00(v214, &v162, (int)v226, a10);
  v29 = sub_436FF0(a10);
  v26 = v214[0] + v29;
  v176 = v214[0] + v29;
LABEL_33:
  if ( a5 )
  {
    if ( *v171 == *a7 )
    {
      v30 = sub_4344D0();
      *v32 = v31 + v30;
    }
    v176 = sub_4344D0() + v26;
  }
  v185 = v172;
  qmemcpy(pszFaceName, v226, sizeof(pszFaceName));
  v173 = 0;
  v188 = 0;
  v190 = 0;
  v164 = 0;
  v180 = 0;
  v199 = 0;
  v178 = 1;
  v159 = (CHAR *)v226;
  sub_42E9C0(v217, v24);
  *(_DWORD *)&pszFaceName[48] = 0;
  sub_42E9F0(0, &v174, (int)v24);
  sub_42E9F0(1u, &v175, (int)v24);
  v33 = v161;
  *v183 = 1;
  v22 = *v33 == 0;
  v167 = 0;
  v158 = 0;
  v168 = 0;
  v187 = 0;
  v201 = 0;
  if ( v22 )
    goto LABEL_325;
  do
  {
    v34 = v164;
    v35 = v161[v164];
    v36 = &v161[v164];
    v156 = &v161[v164];
    if ( (unsigned __int8)v35 < 0x20u )
    {
      if ( v35 == 10 )
      {
        v37 = v176 + *a7;
        v38 = v171;
        v171[1] += a8;
        *v38 = v37;
        ++*v183;
        v178 = 1;
      }
      v164 = v34 + 1;
      continue;
    }
    if ( v35 != 60 )
      goto LABEL_221;
    v39 = 1;
    if ( !v161[v164 + 1] )
      goto LABEL_221;
    v40 = v36[1];
    do
    {
      if ( v40 == 62 )
        break;
      v40 = v36[++v39];
    }
    while ( v40 );
    v166 = v39;
    if ( v39 <= 1 )
      goto LABEL_221;
    v41 = &v161[v39 + v164];
    if ( *v41 != 62 )
      goto LABEL_221;
    v42 = 0;
    if ( v199 )
    {
      v199 = 0;
LABEL_221:
      if ( a11 )
      {
        v95 = sub_42EA10(&v165);
        v96 = (const char *)&unk_4E5254;
        if ( !v95 )
          v96 = " ";
        v97 = v95 != 0;
        v98 = v97 + 1;
        if ( v95 )
          v99 = sub_437080(v97);
        else
          v99 = sub_437080(v97) >> 1;
        v100 = v156;
        v160 = v99;
        v101 = v98;
        v102 = v96;
        if ( v98 < 4 )
        {
LABEL_230:
          if ( !v101 || *v102 == *v100 && (v101 <= 1 || v102[1] == v100[1] && (v101 <= 2 || v102[2] == v100[2])) )
          {
            v103 = v164;
            if ( v164 >= v98 )
            {
              v104 = &v161[v164 - v98];
              v105 = v98;
              v106 = v96;
              if ( v98 < 4 )
              {
LABEL_240:
                if ( !v105 || *v106 == *v104 && (v105 <= 1 || v106[1] == v104[1] && (v105 <= 2 || v106[2] == v104[2])) )
                  goto LABEL_248;
              }
              else
              {
                while ( *(_DWORD *)v104 == *(_DWORD *)v106 )
                {
                  v105 -= 4;
                  v106 += 4;
                  v104 += 4;
                  if ( v105 < 4 )
                    goto LABEL_240;
                }
              }
              v107 = v171;
              if ( v160 + *v171 > a7[2] - v226[8] + 1 )
              {
                v108 = v176 + *a7;
                v178 = 1;
                *v171 = v108;
                v107[1] += a8;
                ++*v183;
                v164 = v98 + v103;
                continue;
              }
            }
          }
        }
        else
        {
          while ( *(_DWORD *)v100 == *(_DWORD *)v102 )
          {
            v101 -= 4;
            v102 += 4;
            v100 += 4;
            if ( v101 < 4 )
              goto LABEL_230;
          }
        }
      }
LABEL_248:
      v172 = 0;
      v109 = sub_42EA10(&v165);
      v110 = v165;
      v111 = v109;
      v170 = (unsigned __int8 *)v109;
      v160 = sub_432E30(v165);
      if ( v160 )
      {
        if ( !sub_433000(v112, v110) )
        {
          v164 += (v111 != 0) + 1;
          continue;
        }
        v113 = (int *)operator new(0x18u);
        v172 = v113;
        sub_433030(v114, v110);
        sub_433000(v115, v110);
        sub_409080(v113, 1);
        v116 = (void *)v113[1];
        v117 = (char *)v113[2];
        v191 = (void *)*v113;
        v118 = (void *)v113[3];
        v192 = v116;
        v119 = (void *)v113[4];
        v193 = v117;
        v120 = (void *)v113[5];
        v194 = v118;
        v195 = v119;
        v196 = v120;
      }
      else
      {
        v191 = v215[0];
        v192 = v215[1];
        v193 = (char *)v215[2];
        v194 = v215[3];
        v195 = v215[4];
        v196 = v215[5];
      }
      sub_40A620((int)&v191, 0);
      sub_433060(v220, (int)&v191, v110, *((void **)v159 + 12), a12);
      if ( v203 || v168 )
      {
        v211[0] = 0;
        if ( a10 )
        {
          v212 = (int)(v193 - 1);
        }
        else if ( v170 )
        {
          v212 = v179;
        }
        else
        {
          v212 = v179 / 2;
        }
        v211[1] = v226[8] - 1;
        v213 = v226[8] - 1;
        sub_40A710(&v191, v211, a12 | 0xFF000000);
      }
      if ( a10 )
      {
        v122 = v177;
        v221 += v177;
        sub_409A80(&v191, (int)v220);
        v124 = v160;
        if ( v160 )
          v125 = (char *)sub_433000(v123, v110);
        else
          v125 = &v193[-v122];
        v157 = v125;
        if ( v124 )
          v166 = 0;
        else
          v166 = sub_409790(v179);
LABEL_275:
        v127 = operator new(0x48u);
        v128 = dword_50763C;
        v127[1] = v180;
        *v127 = 0;
        v127[2] = 0;
        v127[3] = v128;
        v127[14] = 0;
        v127[15] = 0;
        v127[16] = 0;
        v127[17] = 0;
        sub_409080(v127 + 8, 1);
        sub_40A620((int)(v127 + 8), 0);
        if ( *v182 )
        {
          if ( *v182 != 1 )
          {
            if ( *v182 == 2 )
            {
              sub_409080(v204, 1);
              sub_40A620((int)v204, 0);
              sub_433180(v165, (int)v204, *((void **)v159 + 12), v181, v186, v182[3]);
              v129 = v204[0];
              v216[1] = (int)v204[1];
              v216[2] = (int)v204[2];
              v216[0] = (int)v204[0];
              v216[3] = (int)v204[3];
              v216[4] = (int)v204[4];
              v216[5] = (int)v204[5];
              if ( a10 )
                sub_409A80(v216, (int)v220);
              sub_40A530(v216, v127 + 8, 0, 0, 1, 256 - v182[4]);
              sub_40A530((int *)&v191, v127 + 8, v186, v181, 0, 0);
              operator delete[](v129);
            }
            goto LABEL_283;
          }
          sub_409080(v204, 1);
          sub_40A620((int)v204, 0);
          v130 = v182;
          sub_4188D0((int *)&v191, (int **)v204, v182[3]);
          sub_40A530((int *)v204, v127 + 8, v186, v181, 1, 256 - v130[4]);
          operator delete[](v204[0]);
        }
        sub_40A530((int *)&v191, v127 + 8, 0, 0, 0, 0);
LABEL_283:
        v131 = v172;
        if ( v172 )
        {
          operator delete[](*(void **)v172);
          operator delete(v131);
        }
        v132 = (char *)v157 + v166;
        v133 = (void *)(v166 >> 1);
        v160 = 0;
        v207 = (char *)v157 + v166;
        v172 = (void *)(v166 >> 1);
        v157 = (void *)(v132 - (_BYTE *)v133 + sub_436FF0(a10));
        v166 = 0;
        v169 = (void *)((v170 != 0) + 1);
        v208 = (unsigned __int8 *)&v161[v164 + (_DWORD)v169];
        v170 = v208;
        v134 = sub_437BD0();
        v135 = v134;
        if ( v134 < 1 )
        {
          v178 = 0;
        }
        else if ( v173 > 0 )
        {
          --v173;
        }
        else if ( !v178 && v134 >= 2 )
        {
          sub_434F00(&v223, v197, (int)v159, a10);
          if ( (int)v157 < (int)v223 )
            v157 = v223;
          v173 = v135 - 1;
          v170 = (unsigned __int8 *)&v161[v135 + v164];
          v166 = v135 - 1;
        }
        if ( a5 )
        {
          if ( v188 > 0 )
          {
            --v188;
          }
          else if ( sub_4348C0((int)v200, (const char *)v156, &v230) )
          {
            sub_434F00(v214, &v230, (int)v159, a10);
            if ( (int)v157 < v214[0] )
              v157 = (void *)v214[0];
            v136 = operator new[](strlen(&v230) + 1);
            v127[15] = v214[1];
            v127[14] = v136;
            v137 = &v230;
            v138 = v136;
            do
            {
              v139 = *v137;
              *v138++ = *v137++;
            }
            while ( v139 );
            v188 = sub_434B50(&v230, 0) - 1;
            v173 = v188;
            v166 = v188;
            v170 = (unsigned __int8 *)&v161[strlen(&v230) + v164];
          }
        }
        if ( a11 )
        {
          if ( v190 > 0 )
          {
            --v190;
          }
          else
          {
            v140 = sub_437A10(v170);
            if ( v140 <= 0 )
            {
              if ( sub_437B70() && *v208 )
              {
                v141 = sub_42FA80(*v208);
                v230 = v143;
                if ( v141 )
                {
                  v231 = *(_BYTE *)(v142 + 1);
                  v232 = 0;
                }
                else
                {
                  v231 = 0;
                }
                sub_434F00(v222, &v230, (int)v159, a10);
                v157 = (char *)v157 + v222[0];
              }
            }
            else
            {
              sub_434F00(v224, &v230, (int)v159, a10);
              v157 = (char *)v157 + v224[0];
              v190 = v140 + v166;
              sub_437C00(&v162, v140 - 1);
              if ( dword_50764C )
              {
                v160 = sub_437B10();
                if ( v160 )
                {
                  sub_42EA10(&v198);
                  v133 = v172;
                }
              }
              else
              {
                v160 = 0;
              }
            }
          }
        }
        v144 = v171;
        if ( (int)v157 + *v171 > a7[2] - (v160 == 0 ? v202 : 0) + 1 )
        {
          if ( v165 == v198 )
          {
            v198 = 0;
          }
          else
          {
            if ( v168 )
              v187 = v156;
            v145 = v176 + *a7;
            v146 = v183;
            v171[1] += a8;
            *v144 = v145;
            ++*v146;
            v178 = 1;
          }
        }
        v147 = (char *)v133 + *v144;
        v127[4] = v147;
        v148 = v147;
        v149 = v184 + v144[1];
        v127[5] = v149;
        v127[6] = v148;
        v127[7] = v149;
        v150 = sub_436FF0(a10);
        v151 = v185;
        *v144 += (int)&v207[v150];
        v180 += dword_507638;
        v164 += (unsigned int)v169;
        v151[17] = v127;
        v185 = v127;
        continue;
      }
      if ( v160 )
      {
        v126 = sub_433000(v121, v110);
      }
      else
      {
        if ( v170 )
        {
          v157 = (void *)v179;
LABEL_274:
          v166 = 0;
          goto LABEL_275;
        }
        v126 = v179 / 2;
      }
      v157 = (void *)v126;
      goto LABEL_274;
    }
    v157 = v41 + 1;
    memcpy_0(Src, &v161[v164 + 1], v39 - 1);
    String[v39 + 255] = 0;
    sub_42EA80(v43, Src);
    v44 = (const char *)&unk_4E5264;
    v218 = &unk_4E5264;
    v219[0] = &unk_4E5268;
    v219[1] = &unk_4E526C;
    v219[2] = &unk_4E5270;
    v219[3] = &unk_4E5274;
    v219[4] = "ruby";
    v219[5] = "r";
    v219[6] = "/r";
    v219[7] = "cr";
    v219[8] = "c";
    v219[9] = "/c";
    v219[10] = "l";
    v219[11] = "/l";
    v219[12] = "t";
    v219[13] = "ev";
    v219[14] = 0;
    while ( strcmp(Src, v44) )
    {
      if ( !v42 )
        goto LABEL_67;
      v45 = strlen(v44);
      v46 = v44;
      v47 = Src;
      if ( v45 < 4 )
      {
LABEL_56:
        if ( !v45 )
          goto LABEL_65;
      }
      else
      {
        while ( *(_DWORD *)v47 == *(_DWORD *)v46 )
        {
          v45 -= 4;
          v46 += 4;
          v47 += 4;
          if ( v45 < 4 )
            goto LABEL_56;
        }
      }
      v48 = (unsigned __int8)*v47 - *(unsigned __int8 *)v46;
      if ( v48 )
        goto LABEL_64;
      if ( v45 > 1 )
      {
        v48 = (unsigned __int8)v47[1] - *((unsigned __int8 *)v46 + 1);
        if ( v48 )
          goto LABEL_64;
        if ( v45 > 2 )
        {
          v48 = (unsigned __int8)v47[2] - *((unsigned __int8 *)v46 + 2);
          if ( v48 )
            goto LABEL_64;
          if ( v45 > 3 )
          {
            v48 = (unsigned __int8)v47[3] - *((unsigned __int8 *)v46 + 3);
LABEL_64:
            v49 = (v48 >> 31) | 1;
            goto LABEL_66;
          }
        }
      }
LABEL_65:
      v49 = 0;
LABEL_66:
      if ( !v49 )
        break;
LABEL_67:
      v44 = (const char *)v219[v42++];
      if ( !v44 )
      {
        v164 += v166 + 1;
        goto LABEL_324;
      }
    }
    v50 = &Src[strlen((const char *)v219[v42 - 1])];
    switch ( v42 )
    {
      case 0:
        v199 = 1;
        v164 += v166 + 1;
        continue;
      case 1:
        if ( *(_DWORD *)&pszFaceName[40] )
          goto LABEL_219;
        v51 = operator new(0xACu);
        v169 = v51;
        v233 = 0;
        if ( v51 )
          v52 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v51);
        else
          v52 = 0;
        v233 = -1;
        if ( sub_42DDF0(
               (int)v52,
               pszFaceName,
               *(int *)&pszFaceName[32],
               *(int *)&pszFaceName[36],
               1,
               *(DWORD *)&pszFaceName[44],
               (int)v217,
               64,
               1) )
        {
          goto LABEL_79;
        }
        if ( *(_DWORD *)&pszFaceName[48] )
          (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
        v159 = pszFaceName;
        *(_DWORD *)&pszFaceName[40] = 1;
        *(_DWORD *)&pszFaceName[48] = v52;
        sub_42E9E0((int)v52, v174, v175);
        v164 += v166 + 1;
        continue;
      case 2:
        if ( !*(_DWORD *)&pszFaceName[40] )
          goto LABEL_219;
        if ( !strcmp(pszFaceName, (const char *)v226)
          && *(_DWORD *)&pszFaceName[32] == v226[8]
          && *(_DWORD *)&pszFaceName[36] == v226[9]
          && *(_DWORD *)&pszFaceName[44] == v226[11] )
        {
          if ( *(_DWORD *)&pszFaceName[48] )
            (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
          v159 = (CHAR *)v226;
          *(_DWORD *)&pszFaceName[40] = 0;
          *(_DWORD *)&pszFaceName[48] = 0;
          v164 += v166 + 1;
        }
        else
        {
          v53 = operator new(0xACu);
          v169 = v53;
          v233 = 1;
          if ( v53 )
            v52 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v53);
          else
            v52 = 0;
          v233 = -1;
          if ( sub_42DDF0(
                 (int)v52,
                 pszFaceName,
                 *(int *)&pszFaceName[32],
                 *(int *)&pszFaceName[36],
                 0,
                 *(DWORD *)&pszFaceName[44],
                 (int)v217,
                 64,
                 1) )
          {
            goto LABEL_96;
          }
          if ( *(_DWORD *)&pszFaceName[48] )
            (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
          *(_DWORD *)&pszFaceName[40] = 0;
          *(_DWORD *)&pszFaceName[48] = v52;
          sub_42E9E0((int)v52, v174, v175);
          v164 += v166 + 1;
        }
        continue;
      case 3:
        if ( *(_DWORD *)&pszFaceName[44] )
          goto LABEL_219;
        v54 = operator new(0xACu);
        v169 = v54;
        v233 = 2;
        if ( v54 )
          v52 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v54);
        else
          v52 = 0;
        v233 = -1;
        if ( sub_42DDF0(
               (int)v52,
               pszFaceName,
               *(int *)&pszFaceName[32],
               *(int *)&pszFaceName[36],
               *(int *)&pszFaceName[40],
               1u,
               (int)v217,
               64,
               1) )
        {
          goto LABEL_96;
        }
        if ( *(_DWORD *)&pszFaceName[48] )
          (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
        v159 = pszFaceName;
        *(_DWORD *)&pszFaceName[44] = 1;
        *(_DWORD *)&pszFaceName[48] = v52;
        sub_42E9E0((int)v52, v174, v175);
        v164 += v166 + 1;
        continue;
      case 4:
        if ( !*(_DWORD *)&pszFaceName[44] )
          goto LABEL_219;
        if ( !strcmp(pszFaceName, (const char *)v226)
          && *(_DWORD *)&pszFaceName[32] == v226[8]
          && *(_DWORD *)&pszFaceName[36] == v226[9]
          && *(_DWORD *)&pszFaceName[40] == v226[10] )
        {
          if ( *(_DWORD *)&pszFaceName[48] )
            (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
          v159 = (CHAR *)v226;
          *(_DWORD *)&pszFaceName[44] = 0;
          *(_DWORD *)&pszFaceName[48] = 0;
          v164 += v166 + 1;
        }
        else
        {
          v55 = operator new(0xACu);
          v169 = v55;
          v233 = 3;
          if ( v55 )
            v52 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v55);
          else
            v52 = 0;
          v233 = -1;
          if ( sub_42DDF0(
                 (int)v52,
                 pszFaceName,
                 *(int *)&pszFaceName[32],
                 *(int *)&pszFaceName[36],
                 *(int *)&pszFaceName[40],
                 0,
                 (int)v217,
                 64,
                 1) )
          {
LABEL_96:
            if ( !v52 )
              goto LABEL_219;
            goto LABEL_80;
          }
          if ( *(_DWORD *)&pszFaceName[48] )
            (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
          *(_DWORD *)&pszFaceName[44] = 0;
          *(_DWORD *)&pszFaceName[48] = v52;
          sub_42E9E0((int)v52, v174, v175);
          v164 += v166 + 1;
        }
        continue;
      case 5:
        String[0] = 0;
        v22 = *v50 == 32;
        v56 = String;
        v156 = v229;
        v229[0] = 0;
        if ( v22 )
        {
          do
            ++v50;
          while ( *v50 == 32 );
        }
        while ( 1 )
        {
          v57 = sub_42EA10(&v165);
          if ( !v57 )
          {
            if ( !*v50 )
              goto LABEL_219;
            if ( *v50 == 44 )
              break;
          }
          for ( i = (v57 != 0) + 1; i; --i )
            *v56++ = *v50++;
        }
        v59 = v50 + 1;
        while ( 1 )
        {
          v60 = sub_42EA10(&v165);
          if ( !v60 && !*v59 )
            break;
          v61 = (v60 != 0) + 1;
          if ( (v60 != 0) != -1 )
          {
            memcpy_0(v156, v59, (v60 != 0) + 1);
            v59 += v61;
            v156 = (char *)v156 + v61;
          }
        }
        *(_BYTE *)v156 = 0;
        v152 = v200;
        *v56 = 0;
        sub_434520(v152, String, v229, 1);
        v164 += v166 + 1;
        continue;
      case 6:
        for ( ; *v50 == 32; ++v50 )
          ;
        v62 = v229;
        while ( 1 )
        {
          v63 = sub_42EA10(&v165);
          if ( !v63 && !*v50 )
            break;
          v64 = (v63 != 0) + 1;
          if ( (v63 != 0) != -1 )
          {
            memcpy_0(v62, v50, (v63 != 0) + 1);
            v50 += v64;
            v62 += v64;
          }
        }
        *v62 = 0;
        if ( !v229[0] )
          goto LABEL_219;
        v65 = *(_BYTE *)v157;
        v156 = String;
        if ( !v65 )
          goto LABEL_219;
        v66 = (char *)v157;
        break;
      case 8:
        v70 = v164;
        *v171 = *a7;
        v164 = v70 + v166 + 1;
        continue;
      case 9:
        for ( ; *v50 == 32; ++v50 )
          ;
        v71 = 0;
        for ( j = 0; j < 6; ++j )
        {
          v73 = strchr("0123456789abcdef", v50[j]);
          if ( !v73 )
            break;
          v71 = (v73 - "0123456789abcdef") | (16 * v71);
        }
        if ( j != 6 )
          goto LABEL_219;
        v74 = v167 + 1;
        v75 = operator new[](4 * (v167 + 1));
        v76 = v167;
        if ( v167 )
        {
          memcpy_0(v75, v158, 4 * v167);
          v76 = v167;
        }
        *((_DWORD *)v75 + v76) = a12;
        v167 = v74;
        operator delete[](v158);
        v158 = v75;
        a12 = v71;
        v164 += v166 + 1;
        continue;
      case 10:
        if ( !v167 )
          goto LABEL_219;
        a12 = *((_DWORD *)v158 + --v167);
        v164 += v166 + 1;
        continue;
      case 11:
        if ( v168 )
          goto LABEL_219;
        v77 = v171[1];
        v78 = v167;
        v205 = *v171;
        v206 = v77;
        v168 = v157;
        v79 = v167 + 1;
        v80 = operator new[](4 * (v167 + 1));
        if ( v78 )
          memcpy_0(v80, v158, 4 * v78);
        v154 = v158;
        *((_DWORD *)v80 + v78) = a12;
        v167 = v79;
        operator delete[](v154);
        v158 = v80;
        if ( v210 != -1 )
          a12 = v210;
        if ( *(_DWORD *)&pszFaceName[40] || *(_DWORD *)&pszFaceName[44] )
          goto LABEL_219;
        v81 = ::pszFaceName;
        if ( !strlen(::pszFaceName) )
          v81 = pszFaceName;
        if ( dword_565D1C <= 0 )
          v160 = *(_DWORD *)&pszFaceName[32];
        else
          v160 = dword_565D1C;
        v82 = dword_565D20;
        if ( dword_565D20 <= 0 )
          v82 = *(_DWORD *)&pszFaceName[36];
        v83 = operator new(0xACu);
        v169 = v83;
        v233 = 4;
        if ( v83 )
          v52 = (void (__thiscall ***)(_DWORD, int))sub_42D6A0(v83);
        else
          v52 = 0;
        v233 = -1;
        v84 = sub_407BE0();
        if ( sub_42DDF0((int)v52, v81, v160, v82, dword_565D24, bItalic, v84, 64, 1) )
        {
LABEL_79:
          if ( !v52 )
            goto LABEL_219;
LABEL_80:
          (**v52)(v52, 1);
          v164 += v166 + 1;
        }
        else
        {
          if ( *(_DWORD *)&pszFaceName[48] )
            (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
          v159 = pszFaceName;
          *(_DWORD *)&pszFaceName[48] = v52;
          sub_42E9E0((int)v52, v174, v175);
          v164 += v166 + 1;
        }
        continue;
      case 12:
        if ( !v168 )
          goto LABEL_219;
        if ( v187 )
        {
          v85 = v187 - (_BYTE *)v168;
          v187 = 0;
        }
        else
        {
          v85 = v164 + v161 - (_BYTE *)v168;
        }
        if ( v85 )
        {
          v86 = v85;
          if ( v85 >= 0x60 )
            v86 = 95;
          memcpy_0(Src, v168, v86);
          v155 = v206;
          v153 = v205;
          Src[v85] = 0;
          v168 = 0;
          sub_437E40(Src, v153, v155);
        }
        if ( v167 )
        {
          --v167;
          a12 = *((_DWORD *)v158 + v167);
        }
        if ( *(_DWORD *)&pszFaceName[40] || *(_DWORD *)&pszFaceName[44] )
          goto LABEL_219;
        if ( *(_DWORD *)&pszFaceName[48] )
          (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
        v159 = (CHAR *)v226;
        *(_DWORD *)&pszFaceName[48] = 0;
        v164 += v166 + 1;
        continue;
      case 13:
        for ( k = String; *v50 == 32; ++v50 )
          ;
        if ( !sub_42EA10(&v165) )
        {
          v88 = v50 - String;
          do
          {
            v89 = k[v88];
            if ( v89 < 48 )
              break;
            if ( v89 > 57 )
              break;
            *k++ = v89;
          }
          while ( !sub_42EA10(&v165) );
        }
        *k = 0;
        v180 = dword_507638 * atoi(String);
        v164 += v166 + 1;
        continue;
      case 14:
        for ( m = String; *v50 == 32; ++v50 )
          ;
        for ( ; !sub_42EA10(&v165); *m++ = v91 )
        {
          v91 = *v50;
          if ( *v50 < 48 )
            break;
          if ( v91 > 57 )
            break;
          ++v50;
        }
        *m = 0;
        v92 = operator new(0x48u);
        memset(v92, 0, 0x48u);
        v93 = v201;
        v94 = v180;
        v92[4] = v201;
        v201 = v93 + 1;
        *v92 = 0;
        v92[1] = v94;
        v92[16] = 0x80000000;
        v92[17] = 0;
        if ( strlen(String) )
          v92[5] = atoi(String);
        else
          v92[5] = -1;
        v185[17] = v92;
        v185 = v92;
        goto LABEL_219;
      default:
LABEL_219:
        v164 += v166 + 1;
        continue;
    }
    while ( 1 )
    {
      v67 = sub_42EA10(&v165);
      if ( !v67 && v65 == 60 )
        break;
      v68 = (v67 != 0) + 1;
      if ( (v67 != 0) != -1 )
      {
        v69 = (char *)v156;
        memcpy_0(v156, v66, (v67 != 0) + 1);
        v66 += v68;
        v156 = &v69[v68];
      }
      v65 = *v66;
      if ( !*v66 )
      {
        v164 += v166 + 1;
        goto LABEL_324;
      }
    }
    *(_BYTE *)v156 = 0;
    if ( !String[0] )
      goto LABEL_219;
    sub_434520(v200, String, v229, 1);
    v164 += v166 + 1;
LABEL_324:
    ;
  }
  while ( v161[v164] );
LABEL_325:
  operator delete[](v158);
  if ( *(_DWORD *)&pszFaceName[48] )
    (***(void (__thiscall ****)(_DWORD, int))&pszFaceName[48])(*(_DWORD *)&pszFaceName[48], 1);
  operator delete[](v197);
  operator delete[](v215[0]);
  return v209;
}

// ===== sub_436FF0 @ 0x00436FF0..0x00437025 =====
int __cdecl sub_436FF0(int a1)
{
  int v1; // ecx
  int result; // eax

  result = 0;
  if ( !a1 )
  {
    a1 = 0;
    if ( v1 )
    {
      if ( *(_DWORD *)(v1 + 48) )
        sub_42E9F0(*(_DWORD *)(v1 + 44) != 0, &a1, *(_DWORD *)(v1 + 48));
    }
    return a1 + dword_565BB0;
  }
  return result;
}

// ===== sub_437030 @ 0x00437030..0x0043707D =====
void *__cdecl sub_437030(_DWORD *a1)
{
  int v1; // esi
  void **v2; // edi
  void *v3; // eax

  v1 = a1[17];
  while ( v1 )
  {
    v2 = (void **)v1;
    v3 = *(void **)(v1 + 56);
    v1 = *(_DWORD *)(v1 + 68);
    if ( v3 )
      operator delete[](v3);
    operator delete[](v2[8]);
    operator delete(v2);
  }
  return memset(a1, 0, 0x48u);
}

// ===== sub_437080 @ 0x00437080..0x00437099 =====
int __usercall sub_437080@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 32) * *(_DWORD *)(a1 + 36) / 100;
}

// ===== sub_4370A0 @ 0x004370A0..0x004370D3 =====
int __cdecl sub_4370A0(int a1)
{
  int result; // eax

  result = dword_565CE0;
  if ( dword_565CE0 <= 0 )
    result = a1 * dword_507640 / 100;
  if ( result < 4 )
    return 4;
  return result;
}

// ===== sub_4370E0 @ 0x004370E0..0x00437101 =====
int __stdcall sub_4370E0(int a1, int a2, int a3, int a4, int a5)
{
  return sub_437110(a1, a3, a5);
}

// ===== sub_437110 @ 0x00437110..0x0043729B =====
int __cdecl sub_437110(int a1, int a2, int a3)
{
  _DWORD *v3; // ecx
  int v4; // ebx
  _DWORD *v5; // edi
  int result; // eax
  const CHAR *v7; // eax
  int v8; // ecx
  int v9; // edx
  int v10; // eax
  BOOL v11; // ebx
  _DWORD *i; // edi
  const char *v13; // ecx
  int v14; // [esp+10h] [ebp-B8h]
  int v15; // [esp+14h] [ebp-B4h]
  int v16; // [esp+1Ch] [ebp-ACh] BYREF
  _DWORD v17[3]; // [esp+20h] [ebp-A8h] BYREF
  int v18; // [esp+2Ch] [ebp-9Ch]
  int v19; // [esp+30h] [ebp-98h]
  const char *v20[10]; // [esp+34h] [ebp-94h] BYREF
  _BYTE v21[52]; // [esp+5Ch] [ebp-6Ch] BYREF
  _BYTE v22[32]; // [esp+90h] [ebp-38h] BYREF
  int v23; // [esp+B0h] [ebp-18h]
  int v24; // [esp+B4h] [ebp-14h]
  int v25; // [esp+B8h] [ebp-10h]

  v4 = dword_565B70;
  v5 = v3;
  result = sub_4092B0((int)v22, dword_565B70);
  if ( result )
  {
    v14 = sub_4370A0(v23);
    v7 = byte_565BE0;
    if ( !byte_565BE0[0] )
      v7 = v22;
    v8 = dword_565CE4;
    if ( dword_565CE4 <= 0 )
      v8 = v24;
    v9 = dword_507644;
    if ( dword_507644 == -1 )
      v9 = a2;
    v15 = v9;
    v17[0] = *v5;
    v17[1] = v5[1];
    v17[2] = v5[2];
    v18 = v5[3];
    v19 = v5[4];
    if ( dword_507648 != -1 )
      v18 = dword_507648;
    v10 = sub_409290(v8, v25, v4, (int)&v16, v7, v14);
    v11 = v10 == 0;
    if ( !v10 )
    {
      sub_4092B0((int)v21, dword_565B70);
      for ( i = *(_DWORD **)(a1 + 68); i; i = (_DWORD *)i[17] )
      {
        v13 = (const char *)i[14];
        if ( v13 )
        {
          sub_434840(a3, v13, v20);
          sub_4372A0(i, v20, v14, v21, v15, v17, dword_565CE8 + i[6], dword_565CEC + i[7] - v14, i[15], i[1]);
          sub_434730(v20[0], a3, 1);
        }
      }
      sub_4347D0(a3);
    }
    return v11;
  }
  return result;
}

// ===== sub_4372A0 @ 0x004372A0..0x004375F8 =====
void __cdecl sub_4372A0(int a1, _DWORD *a2, int a3, int a4, int a5, char **a6, int a7, int a8, int a9, int a10)
{
  int v10; // ebx
  int v11; // esi
  int v12; // ecx
  int v13; // eax
  char *v14; // edi
  signed int v15; // edi
  int v16; // edi
  char *v17; // ebx
  int v18; // eax
  char v19; // [esp+10h] [ebp-B8h] BYREF
  int v20; // [esp+54h] [ebp-74h]
  int v21[7]; // [esp+5Ch] [ebp-6Ch] BYREF
  void *v22[2]; // [esp+78h] [ebp-50h] BYREF
  void *v23[6]; // [esp+90h] [ebp-38h] BYREF
  unsigned int v24; // [esp+A8h] [ebp-20h]
  int v25; // [esp+ACh] [ebp-1Ch]
  int v26; // [esp+B0h] [ebp-18h]
  int v27; // [esp+B4h] [ebp-14h]
  int v28; // [esp+B8h] [ebp-10h]
  int v29; // [esp+BCh] [ebp-Ch]
  char *v30; // [esp+C0h] [ebp-8h]
  int v31; // [esp+C4h] [ebp-4h]
  int v32; // [esp+D8h] [ebp+10h]
  int v33; // [esp+E8h] [ebp+20h]
  int v34; // [esp+F0h] [ebp+28h]
  int v35; // [esp+F4h] [ebp+2Ch]

  v10 = *(_DWORD *)(a4 + 32);
  v11 = a3 * *(_DWORD *)(a4 + 36) / 100;
  v12 = v10 * (int)a6[1] / 100;
  v26 = v11;
  v29 = v12;
  if ( v12 <= 0 )
  {
    v29 = 1;
    v12 = 1;
  }
  v13 = v10 * (int)a6[2] / 100;
  v32 = v13;
  if ( v13 <= 0 )
  {
    v32 = 1;
    v13 = 1;
  }
  v14 = *a6;
  v28 = 0;
  v31 = 0;
  v30 = v14;
  if ( v14 == (char *)1 )
  {
    v28 = v12;
  }
  else
  {
    if ( v14 != (char *)2 )
      goto LABEL_10;
    v28 = 2 * v12;
    v13 *= 2;
  }
  v31 = v13;
LABEL_10:
  v15 = a2[6];
  v27 = a9 / v15;
  if ( a9 / v15 < v11 )
    v27 = v11;
  v33 = ((a9 - v27 * (v15 - 1) - v11) >> 1) + a7 + (v10 >> 3);
  if ( v30 == (char *)2 )
  {
    v33 -= v12;
    a8 -= v32;
  }
  v24 = dword_507638 * a2[2] / (unsigned int)v15;
  v34 = a10 + (v24 >> 1);
  sub_409080(v22, 1);
  v16 = 0;
  v30 = &v19;
  v35 = 0;
  if ( (int)a2[6] > 0 )
  {
    v31 += v10;
    while ( 1 )
    {
      v17 = (char *)operator new(0x48u);
      *((_DWORD *)v17 + 3) = dword_50763C;
      *((_DWORD *)v17 + 1) = v34;
      *(_DWORD *)v17 = 0;
      *((_DWORD *)v17 + 2) = 0;
      *((_DWORD *)v17 + 5) = a8;
      *((_DWORD *)v17 + 14) = 0;
      *((_DWORD *)v17 + 15) = 0;
      *((_DWORD *)v17 + 17) = 0;
      *((_DWORD *)v17 + 4) = v33;
      *((_DWORD *)v17 + 16) = 2;
      sub_40A620((int)v22, 0);
      sub_4092E0(*(void **)(a4 + 48), (BOOL **)v22, (int)v21, a5);
      v18 = v26;
      if ( *(_DWORD *)(a2[5] + 4 * v16) < 0x100u )
        v18 = v26 / 2;
      v25 = v18 + v28;
      sub_409080((_DWORD *)v17 + 8, 1);
      sub_40A620((int)(v17 + 32), 0);
      if ( !*a6 )
        goto LABEL_26;
      if ( *a6 == (char *)1 )
        break;
      if ( *a6 == (char *)2 )
      {
        sub_409080(v23, 1);
        sub_40A620((int)v23, 0);
        sub_4094C0(*(void **)(a4 + 48), (int **)v23, v29, v32, (int)a6[3]);
        sub_40A530((int *)v23, (_DWORD *)v17 + 8, 0, 0, 1, 256 - (_DWORD)a6[4]);
        operator delete[](v23[0]);
        sub_40A530((int *)v22, (_DWORD *)v17 + 8, v32, v29, 0, 0);
        goto LABEL_27;
      }
LABEL_28:
      v34 += v24;
      v33 += v27;
      ++v16;
      *((_DWORD *)v30 + 17) = v17;
      v30 = v17;
      v35 = v16;
      if ( v16 >= a2[6] )
        goto LABEL_29;
    }
    sub_409080(v23, 1);
    sub_40A620((int)v23, 0);
    if ( a6[3] )
      sub_4092E0(*(void **)(a4 + 48), (BOOL **)v23, (int)v21, (int)a6[3]);
    else
      sub_40A9E0((int)v23, (int)v22, 5u, 0x100u, 1);
    sub_40A530((int *)v23, (_DWORD *)v17 + 8, v32, v29, 1, 256 - (_DWORD)a6[4]);
    operator delete[](v23[0]);
LABEL_26:
    sub_40A530((int *)v22, (_DWORD *)v17 + 8, 0, 0, 0, 0);
LABEL_27:
    v16 = v35;
    goto LABEL_28;
  }
LABEL_29:
  *((_DWORD *)v30 + 17) = *(_DWORD *)(a1 + 68);
  *(_DWORD *)(a1 + 68) = v20;
  operator delete[](v22[0]);
}

// ===== sub_437600 @ 0x00437600..0x00437620 =====
int __thiscall sub_437600(_DWORD *this)
{
  _DWORD *v1; // ecx
  int result; // eax

  v1 = (_DWORD *)this[48];
  result = 1;
  if ( v1 )
  {
    while ( *v1 )
    {
      v1 = (_DWORD *)v1[17];
      if ( !v1 )
        return result;
    }
    return 0;
  }
  return result;
}

// ===== sub_437620 @ 0x00437620..0x0043793E =====
unsigned int __usercall sub_437620@<eax>(int a1@<eax>)
{
  unsigned int result; // eax
  _DWORD *v3; // edi
  unsigned int v4; // ecx
  unsigned int v5; // eax
  unsigned int v6; // ebx
  unsigned int *v7; // eax
  _DWORD *v8; // edi
  int v9; // eax
  int v10; // eax
  int v11; // ecx
  unsigned int v12; // eax
  int v13[4]; // [esp+10h] [ebp-18h] BYREF
  unsigned int i; // [esp+20h] [ebp-8h]
  int v15; // [esp+24h] [ebp-4h]

  if ( *(_DWORD *)(a1 + 52) || *(_DWORD *)(a1 + 48) || (v15 = 0, !dword_507638) )
    v15 = 1;
  if ( !dword_565CF4 )
  {
    v6 = 0;
    if ( sub_431A60((unsigned int *)a1) )
    {
      do
      {
        if ( v15 )
          break;
        v7 = (unsigned int *)sub_431A30(a1, 1);
        ++v6;
      }
      while ( sub_431A60(v7) );
    }
    do
    {
      if ( !v6 && !v15 )
        break;
      v8 = *(_DWORD **)(a1 + 192);
      for ( i = --v6; v8; v8 = (_DWORD *)v8[17] )
      {
        if ( !*v8 )
        {
          v9 = v8[1];
          if ( v9 && !v15 )
          {
            v10 = v9 - 1;
            v8[1] = v10;
            if ( !v10 )
            {
              v11 = dword_50763C;
              v8[2] = 0;
              v8[3] = v11;
            }
            continue;
          }
          v12 = v8[2];
          if ( v12 >= v8[3] || v15 )
          {
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v8[4], v8[5], v8 + 8, 64, 1);
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v8[4], v8[5], v8 + 8, 1, 0);
            (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
            sub_443240(dword_565B6C);
            (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
            *v8 = 1;
            goto LABEL_38;
          }
          if ( !v6 )
          {
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v8[4], v8[5], v8 + 8, 64, 1);
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v8[4], v8[5], v8 + 8, 1, 256 - (v8[2] << 8) / v8[3]);
            (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
            sub_443240(dword_565B6C);
            (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
            ++v8[2];
LABEL_38:
            v6 = i;
            continue;
          }
          v8[2] = v12 + 1;
        }
      }
    }
    while ( !v15 );
    return (*(int (__thiscall **)(int, _DWORD))(*(_DWORD *)a1 + 8))(a1, 0);
  }
  result = sub_431A50((_DWORD *)a1);
  v3 = *(_DWORD **)(a1 + 192);
  i = result;
  if ( v3 )
  {
    while ( 1 )
    {
      if ( !*v3 )
      {
        v4 = v3[1];
        if ( v4 <= result || v15 )
        {
          if ( v3[16] == 0x80000000 )
          {
            result = sub_496540(805306369, v3[4], v3[5]);
          }
          else
          {
            v5 = result - v4;
            v3[2] = v5;
            if ( v5 < v3[3] && !v15 )
            {
              sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v3[4], v3[5], v3 + 8, 64, 1);
              sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v3[4], v3[5], v3 + 8, 1, 256 - (v3[2] << 8) / v3[3]);
              (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
              sub_443240(dword_565B6C);
              result = (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
              goto LABEL_19;
            }
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v3[4], v3[5], v3 + 8, 64, 1);
            sub_42B5B0(*(_DWORD **)(a1 + 32), v13, v3[4], v3[5], v3 + 8, 1, 0);
            (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
            sub_443240(dword_565B6C);
            result = (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
          }
          *v3 = 1;
        }
      }
LABEL_19:
      v3 = (_DWORD *)v3[17];
      if ( !v3 )
        return result;
      result = i;
    }
  }
  return result;
}

// ===== sub_437940 @ 0x00437940..0x0043799F =====
int __usercall sub_437940@<eax>(int a1@<eax>, _DWORD *a2, _DWORD *a3)
{
  int *v3; // esi
  int result; // eax
  int v6; // [esp+Ch] [ebp-4h]

  v3 = *(int **)(a1 + 68);
  result = 0;
  v6 = 0;
  if ( v3 )
  {
    do
    {
      sub_40A530(v3 + 8, a2, v3[5], v3[4], 0, 0);
      sub_409190(a3, (int)(v3 + 8));
      sub_409170(v3[5], v3[4], a3);
      v3 = (int *)v3[17];
      ++v6;
      a3 += 4;
    }
    while ( v3 );
    return v6;
  }
  return result;
}

// ===== sub_4379A0 @ 0x004379A0..0x004379AF =====
int sub_4379A0()
{
  int v0; // ecx

  sub_4344D0();
  return sub_42C6E0(*(void **)(v0 + 32));
}

// ===== sub_4379B0 @ 0x004379B0..0x00437A0D =====
int __thiscall sub_4379B0(int *this, _DWORD *a2)
{
  int v3; // eax
  int result; // eax
  _BYTE v5[32]; // [esp+8h] [ebp-34h] BYREF
  int v6; // [esp+28h] [ebp-14h]

  sub_42C450(this[8]);
  v3 = sub_4092B0((int)v5, dword_565B70);
  *a2 = 0;
  if ( this[49] && v3 )
  {
    result = sub_4370A0(v6);
    a2[1] = result;
  }
  else
  {
    result = 0;
    a2[1] = 0;
  }
  return result;
}

// ===== sub_437A10 @ 0x00437A10..0x00437AA9 =====
int __usercall sub_437A10@<eax>(_BYTE *a1@<eax>, _BYTE *a2)
{
  _BYTE *v3; // edi
  int v4; // eax
  int result; // eax
  int v6; // eax
  _BYTE *v7; // esi
  int v8; // [esp+Ch] [ebp-10h]
  int v9; // [esp+10h] [ebp-Ch] BYREF
  int v10; // [esp+14h] [ebp-8h] BYREF
  _BYTE *v11; // [esp+18h] [ebp-4h]
  int i; // [esp+24h] [ebp+8h]

  v11 = a1;
  for ( i = 0; ; ++i )
  {
    v8 = sub_42EA10(&v10);
    v3 = &unk_4E52C0;
    while ( 1 )
    {
      v4 = sub_42EA10(&v9);
      if ( v10 == v9 )
        break;
      v3 += (v4 != 0) + 1;
      if ( !*v3 )
      {
        result = i;
        *v11 = 0;
        return result;
      }
    }
    v6 = (v8 != 0) + 1;
    if ( (v8 != 0) != -1 )
    {
      do
      {
        v7 = v11;
        *v11 = *a2++;
        --v6;
        v11 = v7 + 1;
      }
      while ( v6 );
    }
    if ( !*v3 )
      break;
  }
  *v11 = 0;
  return i;
}

// ===== sub_437AB0 @ 0x00437AB0..0x00437B03 =====
int sub_437AB0()
{
  int v0; // ebx
  _BYTE *v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-4h] BYREF

  sub_42EA10(&v4);
  v0 = v4;
  v1 = &unk_4E5318;
  while ( 1 )
  {
    v2 = sub_42EA10(&v4);
    if ( v0 == v4 )
      break;
    v1 += (v2 != 0) + 1;
    if ( !*v1 )
      return 0;
  }
  return 1;
}

// ===== sub_437B10 @ 0x00437B10..0x00437B63 =====
int sub_437B10()
{
  int v0; // ebx
  _BYTE *v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-4h] BYREF

  sub_42EA10(&v4);
  v0 = v4;
  v1 = &unk_4E5328;
  while ( 1 )
  {
    v2 = sub_42EA10(&v4);
    if ( v0 == v4 )
      break;
    v1 += (v2 != 0) + 1;
    if ( !*v1 )
      return 0;
  }
  return 1;
}

// ===== sub_437B70 @ 0x00437B70..0x00437BC3 =====
int sub_437B70()
{
  int v0; // ebx
  _BYTE *v1; // edi
  int v2; // eax
  int v4; // [esp+Ch] [ebp-4h] BYREF

  sub_42EA10(&v4);
  v0 = v4;
  v1 = &unk_4E5374;
  while ( 1 )
  {
    v2 = sub_42EA10(&v4);
    if ( v0 == v4 )
      break;
    v1 += (v2 != 0) + 1;
    if ( !*v1 )
      return 0;
  }
  return 1;
}

// ===== sub_437BD0 @ 0x00437BD0..0x00437BFC =====
int __usercall sub_437BD0@<eax>(char *a1@<edx>, _BYTE *a2@<edi>)
{
  char v2; // cl
  int result; // eax
  int v4; // esi

  v2 = *a1;
  result = 0;
  if ( *a1 < 33 )
  {
    *a2 = 0;
  }
  else
  {
    v4 = a2 - a1;
    do
    {
      if ( v2 == 127 )
        break;
      a1[v4] = v2;
      v2 = *++a1;
      ++result;
    }
    while ( v2 >= 33 );
    a2[result] = 0;
  }
  return result;
}

// ===== sub_437C00 @ 0x00437C00..0x00437C78 =====
int __usercall sub_437C00@<eax>(char *a1@<edx>, _BYTE *a2, int a3)
{
  int result; // eax
  char v5; // bl
  int v6; // eax
  int v7; // ecx
  int v8; // [esp+0h] [ebp-Ch] BYREF
  int v9; // [esp+4h] [ebp-8h]
  int v10; // [esp+8h] [ebp-4h]

  result = 0;
  v10 = 0;
  if ( a3 >= 0 )
  {
    v9 = 0;
    do
    {
      v5 = *a1;
      if ( !*a1 )
        break;
      v6 = sub_42EA10(&v8);
      v7 = v9;
      if ( v9 >= a3 )
      {
        *a2 = v5;
        if ( v6 )
        {
          a2[1] = a1[1];
          a2[2] = 0;
        }
        else
        {
          a2[1] = 0;
        }
        v10 = 1;
      }
      else
      {
        a1 += (v6 != 0) + 1;
      }
      result = v10;
      v9 = v7 + 1;
    }
    while ( v7 + 1 <= a3 );
  }
  return result;
}

// ===== sub_437C80 @ 0x00437C80..0x00437CAB =====
int __stdcall sub_437C80(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  return sub_437CB0(a1, a2, a3, a4, a5, a6, a7);
}

// ===== sub_437CB0 @ 0x00437CB0..0x00437E27 =====
void __cdecl sub_437CB0(int a1, _DWORD *a2, _DWORD *a3, int a4, int a5, int a6, int a7)
{
  _DWORD *v7; // edi
  _DWORD *i; // esi
  _DWORD *v9; // eax
  int v10; // eax
  int v11; // esi
  int v12; // ecx
  _DWORD *v13; // edi
  int v14; // esi
  _DWORD *j; // edx
  int v16; // ebx
  int v17; // eax
  void *v18; // esi
  void *v19; // [esp-4h] [ebp-60h]
  _BYTE v20[32]; // [esp+Ch] [ebp-50h] BYREF
  int v21; // [esp+2Ch] [ebp-30h]
  _DWORD v22[3]; // [esp+40h] [ebp-1Ch] BYREF
  void *v23; // [esp+4Ch] [ebp-10h]
  int v24; // [esp+50h] [ebp-Ch]
  int v25; // [esp+54h] [ebp-8h]

  if ( a7 )
  {
    v7 = *(_DWORD **)(a1 + 68);
    v22[0] = 0;
    v22[1] = 0;
    v22[2] = 0x80000000;
    v23 = 0;
    for ( i = v22; v7; v7 = (_DWORD *)v7[17] )
    {
      if ( !v7[16] )
      {
        if ( i[2] != v7[5] )
        {
          v9 = operator new(0x10u);
          i[3] = v9;
          i = v9;
          *v9 = v7[4];
          v9[1] = 0x80000000;
          v9[2] = v7[5];
          v9[3] = 0;
        }
        v10 = v7[10] + v7[4] - 1;
        if ( i[1] < v10 )
          i[1] = v10;
      }
    }
    v11 = 0;
    v24 = 0;
    if ( sub_4092B0((int)v20, dword_565B70) )
    {
      v11 = v21;
      v24 = v21;
    }
    v12 = 0;
    if ( dword_565B80 )
      v25 = *((_DWORD *)dword_565B80 + 3);
    else
      v25 = 0;
    v13 = *(_DWORD **)(a1 + 68);
    v14 = v11 * *(_DWORD *)(a6 + 4) / 100;
    for ( j = v22; v13; v13 = (_DWORD *)v13[17] )
    {
      if ( !v13[16] && j[2] != v13[5] )
      {
        j = (_DWORD *)j[3];
        if ( a7 == 1 )
        {
          v16 = 0;
          if ( a5 )
          {
            LOBYTE(v16) = j[1] >= a3[2];
            v16 = v24 & (v16 - 1);
          }
          v17 = sub_4344D0();
          v12 = (v14 + a3[2] - j[1] - v16 - v17) >> 1;
        }
        else if ( a7 == 2 )
        {
          v12 = v14 + a3[2] - j[1] - v25;
        }
      }
      v13[4] += v12;
    }
    if ( *a3 < *a2 )
      *a2 += v12;
    v18 = v23;
    while ( v18 )
    {
      v19 = v18;
      v18 = (void *)*((_DWORD *)v18 + 3);
      operator delete[](v19);
    }
  }
}

// ===== sub_437E30 @ 0x00437E30..0x00437E3B =====
void sub_437E30()
{
  dword_565CF8 = 0;
}

// ===== sub_437E40 @ 0x00437E40..0x00437EB0 =====
BOOL __cdecl sub_437E40(const char *Src, int a2, int a3)
{
  BOOL v3; // ebx
  size_t v4; // edi
  _DWORD *v5; // esi

  v3 = (unsigned int)dword_565CF8 < 0x10;
  if ( (unsigned int)dword_565CF8 < 0x10 )
  {
    v4 = strlen(Src);
    if ( v4 >= 0x60 )
      v4 = 95;
    v5 = (_DWORD *)((char *)&unk_50E240 + 128 * dword_565CF8);
    memset(v5, 0, 0x60u);
    memcpy_0(v5, Src, v4);
    ++dword_565CF8;
    v5[30] = a2;
    v5[31] = a3;
  }
  return v3;
}

// ===== sub_437EB0 @ 0x00437EB0..0x00437ED8 =====
int __thiscall sub_437EB0(void *this)
{
  int v1; // esi
  int result; // eax

  v1 = dword_565CF8;
  result = dword_565CF8;
  if ( dword_565CF8 )
  {
    memcpy_0(this, &unk_50E240, dword_565CF8 << 7);
    sub_437E30();
    return v1;
  }
  return result;
}

// ===== sub_437EE0 @ 0x00437EE0..0x00437F9F =====
int __cdecl sub_437EE0(char *a1, char *Str)
{
  char *v3; // eax
  const char *v4; // edi
  char *v5; // ebx
  size_t v6; // esi
  int v8; // [esp+Ch] [ebp-4h]
  char *Stra; // [esp+1Ch] [ebp+Ch]

  v8 = 0;
  Stra = a1;
  while ( 1 )
  {
    v3 = strstr(Str, "<l>");
    if ( !v3 )
    {
      v3 = strstr(Str, "<L>");
      if ( !v3 )
        break;
    }
    v4 = v3 + 3;
    v5 = strstr(v3 + 3, "</l>");
    if ( !v5 )
    {
      v5 = strstr(v4, "</L>");
      if ( !v5 )
        break;
    }
    v6 = v5 - v4;
    if ( v5 != v4 )
    {
      if ( a1 )
      {
        memset(Stra, 0, 0x80u);
        if ( v6 >= 0x60 )
          v6 = 95;
        memcpy_0(Stra, v4, v6);
      }
      ++v8;
      Stra += 128;
    }
    Str = v5 + 3;
  }
  return v8;
}

// ===== sub_437FA0 @ 0x00437FA0..0x0043804F =====
unsigned int __usercall sub_437FA0@<eax>(int a1@<eax>, const char *a2, int a3, int a4, DWORD a5)
{
  unsigned int result; // eax

  result = sub_42EAB0(a2, 1, a1, a3);
  if ( result > 0x80000003 )
  {
    if ( result == -2147483644 )
      return result;
    return -1;
  }
  if ( result == -2147483645 )
    return -2147483642;
  if ( result )
  {
    if ( result == -2147483646 )
      return -2147483643;
    return -1;
  }
  memset(pszFaceName, 0, 0x34u);
  if ( a2 )
    strcpy(pszFaceName, a2);
  dword_565D1C = a1;
  bItalic = a5;
  dword_565D20 = a3;
  dword_565D24 = a4;
  return 0;
}
