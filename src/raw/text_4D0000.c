#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4D0170 @ 0x004D0170..0x004D0191 =====
double __cdecl sub_4D0170(_DWORD *a1)
{
  return (double)(*a1 & 0x7FFFFFFF) * 0.00000071771143 - 764.27118;
}

// ===== sub_4D01A0 @ 0x004D01A0..0x004D0558 =====
int __cdecl sub_4D01A0(_DWORD *a1, _DWORD *a2)
{
  int v3; // eax
  int v4; // ecx
  int v5; // eax
  int v6; // edx
  int v7; // edi
  int v8; // eax
  void *v9; // esp
  int v10; // eax
  void *v11; // esp
  int v12; // eax
  void *v13; // esp
  int v14; // eax
  void *v15; // esp
  int v16; // ebx
  _DWORD *v17; // edi
  int v18; // eax
  int v19; // eax
  unsigned int v20; // ecx
  char *v21; // edi
  char v22; // dl
  char *v23; // edi
  _DWORD *v24; // edx
  _DWORD *v25; // ebx
  _DWORD *v26; // eax
  int v27; // edx
  _BYTE *v28; // ecx
  int v29; // edi
  _DWORD *v30; // edx
  int v31; // edx
  int v32; // eax
  int v33; // edi
  int v34; // ecx
  _DWORD *v35; // eax
  unsigned int v36; // ebx
  bool v37; // cc
  int v38; // ecx
  _DWORD *v39; // ebx
  int v40; // edi
  float *v41; // ecx
  int v42; // eax
  int v43; // edx
  double v44; // st7
  double v45; // st6
  char v46; // fps^1
  bool v47; // c0
  char v48; // c2
  bool v49; // c3
  char v50; // ah
  bool v51; // c0
  bool v52; // c3
  double v53; // st6
  int v54; // ebx
  _DWORD *v55; // edi
  int v56; // eax
  int i; // edi
  int j; // ebx
  char *v59; // edi
  _BYTE v61[12]; // [esp+0h] [ebp-38h] BYREF
  _BYTE *v62; // [esp+Ch] [ebp-2Ch]
  unsigned int v63; // [esp+10h] [ebp-28h]
  _BYTE *v64; // [esp+14h] [ebp-24h]
  _BYTE *v65; // [esp+18h] [ebp-20h]
  int v66; // [esp+1Ch] [ebp-1Ch]
  _DWORD *v67; // [esp+20h] [ebp-18h]
  _DWORD *v68; // [esp+24h] [ebp-14h]
  int v69; // [esp+28h] [ebp-10h]
  _BYTE *v70; // [esp+2Ch] [ebp-Ch]
  int v71; // [esp+30h] [ebp-8h]
  _DWORD *v72; // [esp+34h] [ebp-4h]
  int v73; // [esp+40h] [ebp+8h]

  v3 = a1[16];
  v4 = *(_DWORD *)(v3 + 4);
  v71 = *(_DWORD *)(v3 + 104);
  v5 = a1[7];
  v73 = v4;
  v69 = *(_DWORD *)(v4 + 28);
  v6 = *(_DWORD *)(v69 + 4 * v5);
  a1[9] = v6;
  v7 = *(_DWORD *)(v4 + 4);
  v66 = v6;
  v8 = 4 * v7 + 3;
  LOBYTE(v8) = v8 & 0xFC;
  v9 = alloca(v8);
  v64 = v61;
  v10 = 4 * v7 + 3;
  LOBYTE(v10) = (4 * v7 + 3) & 0xFC;
  v11 = alloca(v10);
  v65 = v61;
  v12 = 4 * v7 + 3;
  LOBYTE(v12) = (4 * v7 + 3) & 0xFC;
  v13 = alloca(v12);
  v70 = v61;
  v14 = 4 * v7 + 3;
  LOBYTE(v14) = (4 * v7 + 3) & 0xFC;
  v15 = alloca(v14);
  v16 = 0;
  v62 = v61;
  if ( v7 > 0 )
  {
    v17 = v70;
    v63 = (unsigned int)(4 * v66) >> 1;
    v68 = v70;
    v72 = a2 + 1;
    v67 = (_DWORD *)(v61 - v70);
    while ( 1 )
    {
      v18 = a2[*v72 + 257];
      v19 = (*((int (__cdecl **)(_DWORD *, _DWORD))*(&off_502DD8 + *(_DWORD *)(v69 + 4 * v18 + 800)) + 5))(
              a1,
              *(_DWORD *)(*(_DWORD *)(v71 + 48) + 4 * v18));
      *(_DWORD *)((char *)v17 + (_DWORD)v67) = v19;
      v20 = v63;
      *v17 = v19 != 0;
      v21 = *(char **)(*a1 + 4 * v16);
      v22 = v20;
      v20 >>= 2;
      memset(v21, 0, 4 * v20);
      v23 = &v21[4 * v20];
      LOBYTE(v20) = v22;
      v24 = v68;
      ++v16;
      memset(v23, 0, v20 & 3);
      ++v72;
      v68 = v24 + 1;
      if ( v16 >= *(_DWORD *)(v73 + 4) )
        break;
      v17 = v68;
    }
  }
  v25 = a2;
  v72 = 0;
  if ( (int)a2[289] > 0 )
  {
    v26 = a2 + 546;
    do
    {
      v27 = *(v26 - 256);
      v28 = v70;
      v29 = *(_DWORD *)&v70[4 * v27];
      v30 = &v70[4 * v27];
      if ( v29 || *(_DWORD *)&v70[4 * *v26] )
      {
        *v30 = 1;
        *(_DWORD *)&v28[4 * *v26] = 1;
      }
      v31 = a2[289];
      ++v26;
      v72 = (_DWORD *)((char *)v72 + 1);
    }
    while ( (int)v72 < v31 );
  }
  v32 = *a2;
  v72 = 0;
  if ( v32 > 0 )
  {
    v67 = a2 + 273;
    do
    {
      v33 = 0;
      v34 = 0;
      if ( *(int *)(v73 + 4) > 0 )
      {
        v35 = v65;
        v68 = v25 + 1;
        v63 = v64 - v65;
        do
        {
          if ( (_DWORD *)*v68 == v72 )
          {
            v36 = v63;
            *v35 = *(_DWORD *)&v70[4 * v34] != 0;
            ++v33;
            *(_DWORD *)((char *)++v35 + v36 - 4) = *(_DWORD *)(*a1 + 4 * v34);
          }
          ++v34;
          ++v68;
        }
        while ( v34 < *(_DWORD *)(v73 + 4) );
        v25 = a2;
      }
      (*((void (__cdecl **)(_DWORD *, _DWORD, _BYTE *, _BYTE *, int))*(&off_502DE0 + *(_DWORD *)(v69 + 4 * *v67 + 1312))
       + 7))(
        a1,
        *(_DWORD *)(*(_DWORD *)(v71 + 52) + 4 * *v67),
        v64,
        v65,
        v33);
      v37 = (int)v72 + 1 < *v25;
      v72 = (_DWORD *)((char *)v72 + 1);
      ++v67;
    }
    while ( v37 );
  }
  v38 = v25[289] - 1;
  if ( v38 >= 0 )
  {
    v39 = &v25[v38 + 546];
    v40 = v66 / 2;
    v65 = (_BYTE *)(v38 + 1);
    while ( 1 )
    {
      v41 = *(float **)(*a1 + 4 * *(v39 - 256));
      v42 = *(_DWORD *)(*a1 + 4 * *v39);
      if ( v40 > 0 )
        break;
LABEL_33:
      --v39;
      if ( !--v65 )
        goto LABEL_34;
    }
    v64 = (_BYTE *)v40;
    v43 = v42 - (_DWORD)v41;
    while ( 1 )
    {
      v44 = *v41;
      v45 = *(float *)((char *)v41 + v43);
      v47 = v44 < 0.0;
      v48 = 0;
      v49 = v44 == 0.0;
      v50 = v46;
      v51 = v45 < 0.0;
      v52 = v45 == 0.0;
      if ( (v50 & 0x41) != 0 )
      {
        if ( v51 || v52 )
        {
          *(float *)((char *)v41 + v43) = *v41;
          v53 = v44 - v45;
          goto LABEL_31;
        }
        *(float *)((char *)v41 + v43) = v45 + v44;
      }
      else
      {
        if ( v51 || v52 )
        {
          *(float *)((char *)v41 + v43) = *v41;
          v53 = v45 + v44;
LABEL_31:
          *v41 = v53;
          goto LABEL_32;
        }
        *(float *)((char *)v41 + v43) = *v41 - *(float *)((char *)v41 + v43);
      }
LABEL_32:
      ++v41;
      if ( !--v64 )
        goto LABEL_33;
    }
  }
LABEL_34:
  v54 = 0;
  if ( *(int *)(v73 + 4) > 0 )
  {
    v55 = a2 + 1;
    do
    {
      v56 = a2[*v55 + 257];
      (*((void (__cdecl **)(_DWORD *, _DWORD, _DWORD, _DWORD))*(&off_502DD8 + *(_DWORD *)(v69 + 4 * v56 + 800)) + 6))(
        a1,
        *(_DWORD *)(*(_DWORD *)(v71 + 48) + 4 * v56),
        *(_DWORD *)&v62[4 * v54],
        *(_DWORD *)(*a1 + 4 * v54));
      ++v54;
      ++v55;
    }
    while ( v54 < *(_DWORD *)(v73 + 4) );
  }
  for ( i = 0; i < *(_DWORD *)(v73 + 4); ++i )
    sub_4CE420(**(int ***)(v71 + 4 * a1[7] + 12), *(_DWORD *)(*a1 + 4 * i), *(_DWORD *)(*a1 + 4 * i));
  for ( j = 0; j < *(_DWORD *)(v73 + 4); ++j )
  {
    v59 = *(char **)(*a1 + 4 * j);
    if ( *(_DWORD *)&v70[4 * j] )
    {
      sub_4CE0C0(v59, v71 + 4, v69, a1[6], a1[7], a1[8]);
    }
    else if ( v66 > 0 )
    {
      memset(v59, 0, 4 * v66);
    }
  }
  return 0;
}

// ===== sub_4D0560 @ 0x004D0560..0x004D057F =====
void __cdecl sub_4D0560(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x714u);
    free(Block);
  }
}

// ===== sub_4D0580 @ 0x004D0580..0x004D05FA =====
void __cdecl sub_4D0580(void *Block)
{
  int i; // edi
  int j; // edi

  if ( Block )
  {
    for ( i = 0; i < *((_DWORD *)Block + 1); ++i )
    {
      if ( *(_DWORD *)(*((_DWORD *)Block + 5) + 4 * i) )
        free(*(void **)(*((_DWORD *)Block + 5) + 4 * i));
    }
    free(*((void **)Block + 5));
    for ( j = 0; j < *((_DWORD *)Block + 6); ++j )
      free(*(void **)(*((_DWORD *)Block + 7) + 4 * j));
    free(*((void **)Block + 7));
    memset(Block, 0, 0x2Cu);
    free(Block);
  }
}

// ===== sub_4D0600 @ 0x004D0600..0x004D06EB =====
int __cdecl sub_4D0600(_DWORD *a1, int a2)
{
  int v2; // ebx
  int result; // eax
  int v4; // ebp
  _DWORD *v5; // ebx
  int v6; // ecx
  _DWORD *v7; // edi
  int v8; // [esp+10h] [ebp-4h]

  v2 = 0;
  v8 = 0;
  sub_4D4B20(a2, *a1, 24);
  sub_4D4B20(a2, a1[1], 24);
  sub_4D4B20(a2, a1[2] - 1, 24);
  sub_4D4B20(a2, a1[3] - 1, 6);
  sub_4D4B20(a2, a1[4], 8);
  result = a1[3];
  v4 = 0;
  if ( result > 0 )
  {
    v5 = a1 + 5;
    do
    {
      if ( (int)sub_4D06F0(*v5) <= 3 )
      {
        sub_4D4B20(a2, *v5, 4);
      }
      else
      {
        sub_4D4B20(a2, *v5, 3);
        sub_4D4B20(a2, 1, 1);
        sub_4D4B20(a2, (int)*v5 >> 3, 5);
      }
      v6 = sub_4D0700(*v5) + v8;
      result = a1[3];
      ++v4;
      ++v5;
      v8 = v6;
    }
    while ( v4 < result );
    v2 = v6;
  }
  if ( v2 > 0 )
  {
    v7 = a1 + 69;
    do
    {
      result = sub_4D4B20(a2, *v7++, 8);
      --v2;
    }
    while ( v2 );
  }
  return result;
}

// ===== sub_4D06F0 @ 0x004D06F0..0x004D0700 =====
int __cdecl sub_4D06F0(unsigned int a1)
{
  unsigned int v1; // ecx
  int result; // eax

  v1 = a1;
  for ( result = 0; v1; v1 >>= 1 )
    ++result;
  return result;
}

// ===== sub_4D0700 @ 0x004D0700..0x004D0716 =====
int __cdecl sub_4D0700(unsigned int a1)
{
  unsigned int v1; // ecx
  int result; // eax

  v1 = a1;
  for ( result = 0; v1; v1 >>= 1 )
    result += v1 & 1;
  return result;
}

// ===== sub_4D0720 @ 0x004D0720..0x004D0857 =====
unsigned int *__cdecl sub_4D0720(int a1, int a2)
{
  int v2; // ebx
  unsigned int *v3; // esi
  int v4; // ebp
  unsigned int v5; // ebx
  _DWORD *v6; // ebp
  int v7; // edx
  int v8; // eax
  int *i; // ecx
  int v11; // [esp+10h] [ebp-8h]
  int v12; // [esp+14h] [ebp-4h]
  unsigned int *v13; // [esp+1Ch] [ebp+4h]
  int v14; // [esp+1Ch] [ebp+4h]

  v2 = 0;
  v11 = 0;
  v3 = (unsigned int *)calloc(1u, 0x714u);
  v12 = *(_DWORD *)(a1 + 28);
  *v3 = sub_4D4D00(a2, 24);
  v3[1] = sub_4D4D00(a2, 24);
  v3[2] = sub_4D4D00(a2, 24) + 1;
  v3[3] = sub_4D4D00(a2, 6) + 1;
  v3[4] = sub_4D4D00(a2, 8);
  v4 = 0;
  if ( (int)v3[3] > 0 )
  {
    v13 = v3 + 5;
    do
    {
      v5 = sub_4D4D00(a2, 3);
      if ( sub_4D4D00(a2, 1) )
        v5 |= 8 * sub_4D4D00(a2, 5);
      *v13 = v5;
      v2 = sub_4D0700(v5) + v11;
      ++v4;
      v11 = v2;
      ++v13;
    }
    while ( v4 < (int)v3[3] );
  }
  if ( v2 > 0 )
  {
    v6 = v3 + 69;
    v14 = v2;
    do
    {
      *v6++ = sub_4D4D00(a2, 8);
      --v14;
    }
    while ( v14 );
  }
  v7 = *(_DWORD *)(v12 + 24);
  if ( (int)v3[4] < v7 )
  {
    v8 = 0;
    if ( v2 <= 0 )
      return v3;
    for ( i = (int *)(v3 + 69); *i < v7; ++i )
    {
      if ( ++v8 >= v2 )
        return v3;
    }
  }
  sub_4D0560(v3);
  return 0;
}

// ===== sub_4D0860 @ 0x004D0860..0x004D0A34 =====
_DWORD *__cdecl sub_4D0860(int a1, _DWORD *a2)
{
  _DWORD *v2; // esi
  int v3; // edi
  int v4; // ebp
  int *v5; // eax
  int v6; // ebx
  signed int v7; // eax
  int v8; // ebx
  signed int v9; // ecx
  _DWORD *v10; // edx
  __int64 v11; // rax
  int v12; // eax
  int i; // ebp
  int v14; // edi
  int v15; // ecx
  int v16; // ebx
  size_t v18; // [esp-8h] [ebp-30h]
  int v19; // [esp+18h] [ebp-10h]
  int v20; // [esp+1Ch] [ebp-Ch]
  signed int v21; // [esp+20h] [ebp-8h]
  signed int v22; // [esp+24h] [ebp-4h]
  unsigned int *v23; // [esp+2Ch] [ebp+4h]
  int v24; // [esp+30h] [ebp+8h]

  v2 = calloc(1u, 0x2Cu);
  v3 = 0;
  v20 = 0;
  v21 = 0;
  v4 = *(_DWORD *)(*(_DWORD *)(a1 + 4) + 28);
  *v2 = a2;
  v2[1] = a2[3];
  v2[3] = *(_DWORD *)(v4 + 2848);
  v18 = v2[1];
  v5 = (int *)(*(_DWORD *)(v4 + 2848) + 44 * a2[4]);
  v2[4] = v5;
  v6 = *v5;
  v19 = *v5;
  v2[5] = calloc(v18, 4u);
  if ( (int)v2[1] > 0 )
  {
    v23 = a2 + 5;
    do
    {
      v7 = sub_4D06F0(*v23);
      v8 = v7;
      v22 = v7;
      if ( v7 )
      {
        if ( v7 > v21 )
          v21 = v7;
        *(_DWORD *)(v2[5] + 4 * v3) = calloc(v7, 4u);
        v9 = 0;
        if ( v8 > 0 )
        {
          v10 = &a2[v20 + 69];
          do
          {
            if ( ((1 << v9) & *v23) != 0 )
            {
              *(_DWORD *)(*(_DWORD *)(v2[5] + 4 * v3) + 4 * v9) = *(_DWORD *)(v4 + 2848) + 44 * *v10++;
              ++v20;
            }
            ++v9;
          }
          while ( v9 < v22 );
        }
      }
      ++v3;
      ++v23;
    }
    while ( v3 < v2[1] );
    v6 = v19;
  }
  v11 = (__int64)floor(pow((double)(int)v2[1], (double)v19) + 0.5);
  v2[6] = v11;
  v2[2] = v21;
  v2[7] = malloc(4 * v11);
  v12 = v2[6];
  for ( i = 0; i < v12; ++i )
  {
    v24 = i;
    v14 = v12 / v2[1];
    *(_DWORD *)(v2[7] + 4 * i) = malloc(4 * v6);
    v15 = 0;
    if ( v6 > 0 )
    {
      do
      {
        v16 = v24 / v14;
        v24 %= v14;
        ++v15;
        v14 /= (int)v2[1];
        *(_DWORD *)(*(_DWORD *)(v2[7] + 4 * i) + 4 * v15 - 4) = v16;
      }
      while ( v15 < v19 );
      v6 = v19;
    }
    v12 = v2[6];
  }
  return v2;
}

// ===== sub_4D0A40 @ 0x004D0A40..0x004D0A98 =====
int __cdecl sub_4D0A40(int a1, int a2, _DWORD *a3, int a4, int a5)
{
  int v5; // esi
  int v6; // ebx
  _DWORD *v7; // ecx
  _DWORD *v8; // eax

  v5 = a5;
  v6 = 0;
  if ( a5 <= 0 )
    return 0;
  v7 = a3;
  v8 = a3;
  do
  {
    if ( *(_DWORD *)((char *)v8 + a4 - (_DWORD)a3) )
    {
      ++v6;
      *v7++ = *v8;
    }
    ++v8;
    --v5;
  }
  while ( v5 );
  if ( v6 )
    return sub_4D0AA0(a1, a2, a3, v6, sub_4CC030);
  else
    return 0;
}

// ===== sub_4D0AA0 @ 0x004D0AA0..0x004D0CB6 =====
int __cdecl sub_4D0AA0(_DWORD *a1, int a2, int a3, int a4, int (__cdecl *a5)(int, int, _DWORD *, int))
{
  int *v5; // eax
  int v6; // ebx
  int v7; // esi
  int v8; // edi
  int v9; // eax
  int v10; // esi
  int v11; // eax
  void *v12; // esp
  int v13; // ecx
  int *v14; // esi
  int v15; // ecx
  int v16; // eax
  int v17; // ebx
  int v18; // esi
  _DWORD *v19; // edi
  int v20; // eax
  _DWORD *v21; // esi
  int v22; // edi
  _DWORD *v23; // ecx
  int v24; // ecx
  bool v25; // cc
  _BYTE v27[12]; // [esp+0h] [ebp-40h] BYREF
  int v28; // [esp+Ch] [ebp-34h]
  int v29; // [esp+10h] [ebp-30h]
  _DWORD *v30; // [esp+14h] [ebp-2Ch]
  int v31; // [esp+18h] [ebp-28h]
  _DWORD *v32; // [esp+1Ch] [ebp-24h]
  _BYTE *v33; // [esp+20h] [ebp-20h]
  int v34; // [esp+24h] [ebp-1Ch]
  int v35; // [esp+28h] [ebp-18h]
  int v36; // [esp+2Ch] [ebp-14h]
  int v37; // [esp+30h] [ebp-10h]
  int v38; // [esp+34h] [ebp-Ch]
  int v39; // [esp+38h] [ebp-8h]
  int v40; // [esp+3Ch] [ebp-4h]

  v5 = *(int **)(a2 + 16);
  v6 = a4;
  v32 = *(_DWORD **)a2;
  v7 = *v5;
  v8 = v32[2];
  v9 = v32[1] - *v32;
  v29 = v7;
  v38 = v8;
  v34 = v9 / v8;
  v10 = (v9 / v8 + v7 - 1) / v7;
  v11 = 4 * a4 + 3;
  LOBYTE(v11) = v11 & 0xFC;
  v12 = alloca(v11);
  v33 = v27;
  if ( a4 > 0 )
  {
    v13 = 4 * v10;
    v14 = (int *)v27;
    v30 = (_DWORD *)v13;
    do
    {
      *v14++ = sub_4C7BE0(a1, (int)v30);
      --v6;
    }
    while ( v6 );
  }
  v15 = 0;
  v39 = 0;
  if ( *(int *)(a2 + 8) > 0 )
  {
    while ( 1 )
    {
      v16 = 0;
      v37 = 0;
      if ( v34 > 0 )
        break;
LABEL_26:
      v39 = ++v15;
      if ( v15 >= *(_DWORD *)(a2 + 8) )
        return 0;
    }
    v17 = 0;
    v35 = 0;
    while ( 1 )
    {
      if ( !v15 )
      {
        v18 = 0;
        if ( a4 > 0 )
          break;
      }
LABEL_14:
      v30 = 0;
      if ( v29 > 0 )
      {
        v40 = 0;
        v36 = v8 * v16;
        while ( v16 < v34 )
        {
          v31 = 0;
          if ( a4 > 0 )
          {
            v21 = v33;
            v22 = 1 << v15;
            v28 = a3 - (_DWORD)v33;
            while ( 1 )
            {
              v23 = (_DWORD *)(v17 + *v21);
              if ( (v22 & v32[*(_DWORD *)(v40 + *v23) + 5]) != 0 )
              {
                v24 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a2 + 20) + 4 * *(_DWORD *)(v40 + *v23)) + 4 * v39);
                if ( v24 )
                {
                  if ( a5(v24, *(_DWORD *)((char *)v21 + v28) + 4 * (v36 + *v32), a1 + 1, v38) == -1 )
                    return 0;
                }
              }
              v17 = v35;
              ++v21;
              if ( ++v31 >= a4 )
              {
                v16 = v37;
                v8 = v38;
                v15 = v39;
                break;
              }
            }
          }
          v40 += 4;
          ++v16;
          v36 += v8;
          v25 = (int)v30 + 1 < v29;
          v30 = (_DWORD *)((char *)v30 + 1);
          v37 = v16;
          if ( !v25 )
            break;
        }
      }
      v17 += 4;
      v35 = v17;
      if ( v16 >= v34 )
        goto LABEL_26;
    }
    v19 = v33;
    v30 = a1 + 1;
    while ( 1 )
    {
      v20 = sub_4CBF00(*(_DWORD **)(a2 + 16), (int)v30);
      if ( v20 == -1 )
        break;
      *(_DWORD *)(v17 + *v19) = *(_DWORD *)(*(_DWORD *)(a2 + 28) + 4 * v20);
      if ( !*(_DWORD *)(v17 + *v19) )
        break;
      ++v18;
      ++v19;
      if ( v18 >= a4 )
      {
        v16 = v37;
        v8 = v38;
        v15 = v39;
        goto LABEL_14;
      }
    }
  }
  return 0;
}

// ===== sub_4D0CC0 @ 0x004D0CC0..0x004D0DDE =====
int __cdecl sub_4D0CC0(int a1, int a2, _DWORD *a3, _DWORD *a4, int a5, int a6, int a7)
{
  int v7; // edi
  _DWORD *v8; // ebp
  _DWORD *v9; // ebx
  int v10; // eax
  int v11; // ecx
  _DWORD *v12; // edx
  _DWORD *v13; // esi
  int v14; // eax
  double v15; // st7
  float *v16; // ecx
  _DWORD *v17; // esi
  int v18; // ebp
  int i; // ecx
  float *v20; // edx
  double v21; // st7
  int v23; // [esp+10h] [ebp-Ch]
  int v24; // [esp+14h] [ebp-8h]
  int v25; // [esp+18h] [ebp-4h]
  int v26; // [esp+2Ch] [ebp+10h]
  int v27; // [esp+30h] [ebp+14h]

  v23 = 0;
  v7 = *(_DWORD *)(a1 + 36) / 2;
  if ( a6 <= 0 )
    return 0;
  v8 = a4;
  v9 = a3;
  v10 = a5 - (_DWORD)a4;
  v11 = (char *)a4 - (char *)a3;
  v12 = a3;
  v24 = a5 - (_DWORD)a4;
  v25 = (char *)a4 - (char *)a3;
  v27 = a6;
  do
  {
    v13 = (_DWORD *)((char *)v12 + v11);
    if ( *(_DWORD *)((char *)v12 + v11 + v10) )
    {
      if ( v8 )
      {
        v14 = 0;
        if ( v7 > 0 )
        {
          do
          {
            v15 = *(float *)(*v12 + 4 * v14) + *(float *)(*v13 + 4 * v14);
            v16 = (float *)(*v13 + 4 * v14++);
            *v16 = v15;
          }
          while ( v14 < v7 );
          v8 = a4;
          v11 = v25;
        }
      }
      *a3++ = *v12;
      v10 = v24;
      ++v23;
    }
    ++v12;
    --v27;
  }
  while ( v27 );
  if ( !v23 )
    return 0;
  v26 = sub_4D0FF0(a1, a2, v9, v23, a7, sub_4D0DE0);
  if ( v8 )
  {
    v17 = v8;
    v18 = a6;
    do
    {
      if ( *(_DWORD *)((char *)v17 + v24) )
      {
        for ( i = 0; i < v7; *v20 = v21 )
        {
          v20 = (float *)(*v17 + 4 * i);
          v21 = *v20 - *(float *)(*v9 + 4 * i++);
        }
        ++v9;
      }
      ++v17;
      --v18;
    }
    while ( v18 );
  }
  return v26;
}

// ===== sub_4D0DE0 @ 0x004D0DE0..0x004D0E31 =====
int __cdecl sub_4D0DE0(int a1, int a2, int a3, _DWORD *a4)
{
  int v5; // ebx
  int v7; // ebp
  int v8; // eax
  int v10; // [esp+18h] [ebp+10h]

  v5 = 0;
  if ( a3 / *a4 <= 0 )
    return 0;
  v7 = a3 / *a4;
  v10 = 4 * *a4;
  do
  {
    v8 = sub_4D0E40(a4, a2);
    v5 += sub_4CBEC0((int)a4, v8, a1);
    a2 += v10;
    --v7;
  }
  while ( v7 );
  return v5;
}

// ===== sub_4D0E40 @ 0x004D0E40..0x004D0FEC =====
int __cdecl sub_4D0E40(int *a1, float *a2)
{
  int *v2; // eax
  int v3; // edx
  int v4; // ecx
  int v5; // esi
  int *v6; // ebx
  int v7; // edi
  int v8; // ecx
  float *v9; // ebp
  float v10; // eax
  float *v11; // edx
  int v12; // esi
  float *v13; // edx
  int v14; // ecx
  bool v15; // zf
  int v16; // ebx
  int v17; // esi
  int v18; // edi
  int *v19; // ebp
  double v20; // st7
  float *v21; // eax
  int v22; // edx
  double v23; // st6
  int v24; // eax
  int v25; // ecx
  double v26; // st7
  int v28; // [esp+10h] [ebp-18h]
  int v29; // [esp+10h] [ebp-18h]
  int v30; // [esp+1Ch] [ebp-Ch]
  float v31; // [esp+1Ch] [ebp-Ch]

  v2 = a1;
  v3 = *a1;
  v4 = a1[3];
  v5 = 0;
  v6 = *(int **)(v4 + 40);
  v28 = 0;
  if ( *a1 > 0 )
  {
    v7 = *v6;
    v30 = *a1;
    v8 = v6[3] >> 1;
    v9 = &a2[v3];
    while ( 1 )
    {
      v10 = *--v9;
      if ( v10 >= (double)*(float *)(v7 + 4 * v8) )
        break;
      if ( v10 < (double)*(float *)(v7 + 4 * v8 - 4) && --v8 > 0 )
      {
        v11 = (float *)(v7 + 4 * v8 - 4);
        do
        {
          if ( v10 >= (double)*v11 )
            break;
          --v8;
          --v11;
        }
        while ( v8 > 0 );
LABEL_15:
        v3 = *a1;
      }
LABEL_16:
      v14 = v28 * v6[2] + *(_DWORD *)(v6[1] + 4 * v8);
      v15 = v30 == 1;
      v28 = v14;
      --v30;
      if ( v15 )
      {
        v2 = a1;
        v5 = v14;
        v4 = a1[3];
        goto LABEL_18;
      }
      v8 = v6[3] >> 1;
    }
    ++v8;
    v12 = v6[3] - 1;
    if ( v8 >= v12 )
      goto LABEL_16;
    v13 = (float *)(v7 + 4 * v8);
    do
    {
      if ( v10 < (double)*v13 )
        break;
      ++v8;
      ++v13;
    }
    while ( v8 < v12 );
    goto LABEL_15;
  }
LABEL_18:
  if ( *(int *)(*(_DWORD *)(v4 + 8) + 4 * v5) <= 0 )
  {
    v16 = v2[1];
    v17 = v2[4];
    v18 = 0;
    v31 = 0.0;
    v29 = -1;
    if ( v16 <= 0 )
    {
      v5 = -1;
    }
    else
    {
      v19 = *(int **)(v4 + 8);
      do
      {
        if ( *v19 > 0 )
        {
          v20 = 0.0;
          if ( v3 > 0 )
          {
            v21 = a2;
            v22 = *a1;
            do
            {
              v23 = *(float *)((char *)v21 + v17 - (_DWORD)a2) - *v21;
              ++v21;
              --v22;
              v20 = v20 + v23 * v23;
            }
            while ( v22 );
            v3 = *a1;
          }
          if ( v29 == -1 || v20 < v31 )
          {
            v31 = v20;
            v29 = v18;
          }
        }
        v17 += 4;
        ++v18;
        ++v19;
      }
      while ( v18 < v16 );
      v5 = v29;
      v2 = a1;
    }
  }
  v24 = v2[4] + 4 * v3 * v5;
  if ( v3 > 0 )
  {
    v25 = v3;
    do
    {
      v24 += 4;
      v26 = *a2 - *(float *)(v24 - 4);
      --v25;
      *a2++ = v26;
    }
    while ( v25 );
  }
  return v5;
}

// ===== sub_4D0FF0 @ 0x004D0FF0..0x004D125A =====
int __cdecl sub_4D0FF0(int a1, _DWORD **a2, int a3, int a4, _DWORD *a5, int (__cdecl *a6)(int, int, int, int, _DWORD))
{
  int v6; // ebp
  int v7; // edx
  int v8; // esi
  int v9; // ecx
  int v10; // eax
  _DWORD *v11; // edx
  int v12; // ecx
  bool v13; // zf
  int v14; // edi
  int v15; // eax
  _DWORD *v16; // esi
  _DWORD *v17; // eax
  _DWORD *v18; // eax
  int v19; // eax
  int v20; // eax
  _DWORD *v21; // ecx
  bool v22; // cc
  _DWORD *v24; // [esp+10h] [ebp-430h]
  int v25; // [esp+10h] [ebp-430h]
  int v26; // [esp+14h] [ebp-42Ch]
  int v27; // [esp+18h] [ebp-428h]
  int v28; // [esp+18h] [ebp-428h]
  int v29; // [esp+1Ch] [ebp-424h]
  int i; // [esp+20h] [ebp-420h]
  int v31; // [esp+24h] [ebp-41Ch]
  _DWORD *v32; // [esp+28h] [ebp-418h]
  int v33; // [esp+2Ch] [ebp-414h]
  int v34; // [esp+34h] [ebp-40Ch]
  int v35; // [esp+38h] [ebp-408h]
  int v36; // [esp+3Ch] [ebp-404h]
  _DWORD v37[128]; // [esp+40h] [ebp-400h] BYREF
  _DWORD v38[128]; // [esp+240h] [ebp-200h] BYREF

  v32 = *a2;
  v6 = *a2[4];
  v35 = (*a2)[3];
  v29 = (*a2)[2];
  v34 = v6;
  v7 = ((*a2)[1] - **a2) / v29;
  memset(v38, 0, sizeof(v38));
  memset(v37, 0, sizeof(v37));
  v26 = 0;
  for ( i = v7; v26 < (int)a2[2]; ++v26 )
  {
    v8 = 0;
    v31 = 0;
    if ( v7 > 0 )
    {
      while ( 1 )
      {
        if ( !v26 && a4 > 0 )
        {
          v27 = a4;
          v24 = a5;
          do
          {
            v9 = 1;
            v10 = *(_DWORD *)(*v24 + 4 * v8);
            if ( v6 > 1 )
            {
              v11 = (_DWORD *)(*v24 + 4 * v8 + 4);
              do
              {
                v10 *= v35;
                if ( v9 + v8 < i )
                  v10 += *v11;
                ++v9;
                ++v11;
              }
              while ( v9 < v6 );
            }
            v12 = (int)a2[4];
            if ( v10 < *(_DWORD *)(v12 + 4) )
              a2[9] = (_DWORD *)((char *)a2[9] + sub_4CBEC0(v12, v10, a1 + 4));
            v13 = v27 == 1;
            ++v24;
            --v27;
          }
          while ( !v13 );
        }
        v25 = 0;
        if ( v6 > 0 )
          break;
LABEL_27:
        v7 = i;
        if ( v8 >= i )
          goto LABEL_28;
      }
      v14 = 4 * v8;
      v15 = v29 * v8;
      v33 = v29 * v8;
      while ( 1 )
      {
        v7 = i;
        if ( v8 >= i )
          break;
        v36 = *v32 + v15;
        if ( a4 > 0 )
        {
          v16 = a5;
          v28 = a4;
          do
          {
            if ( !v26 )
            {
              v17 = &v37[*(_DWORD *)(v14 + *v16)];
              *v17 += v29;
            }
            v18 = (_DWORD *)(v14 + *v16);
            if ( ((1 << v26) & v32[*v18 + 5]) != 0 )
            {
              v19 = *(_DWORD *)(a2[5][*v18] + 4 * v26);
              if ( v19 )
              {
                v20 = a6(a1 + 4, *(_DWORD *)((char *)v16 + a3 - (_DWORD)a5) + 4 * v36, v29, v19, 0);
                a2[8] = (_DWORD *)((char *)a2[8] + v20);
                v21 = &v38[*(_DWORD *)(v14 + *v16)];
                *v21 += v20;
              }
            }
            ++v16;
            --v28;
          }
          while ( v28 );
          v6 = v34;
          v8 = v31;
          v15 = v33;
        }
        ++v8;
        v15 += v29;
        v14 += 4;
        v22 = ++v25 < v6;
        v31 = v8;
        v33 = v15;
        if ( !v22 )
          goto LABEL_27;
      }
    }
LABEL_28:
    ;
  }
  return 0;
}

// ===== sub_4D1260 @ 0x004D1260..0x004D12B3 =====
int __cdecl sub_4D1260(int a1, int a2, _DWORD *a3, int a4, int a5)
{
  int v5; // esi
  int v6; // ebx
  _DWORD *v7; // ecx
  _DWORD *v8; // eax

  v5 = a5;
  v6 = 0;
  if ( a5 <= 0 )
    return 0;
  v7 = a3;
  v8 = a3;
  do
  {
    if ( *(_DWORD *)((char *)v8 + a4 - (_DWORD)a3) )
    {
      ++v6;
      *v7++ = *v8;
    }
    ++v8;
    --v5;
  }
  while ( v5 );
  if ( v6 )
    return sub_4D12C0(a1, a2, a3, v6);
  else
    return 0;
}

// ===== sub_4D12C0 @ 0x004D12C0..0x004D14EA =====
int __cdecl sub_4D12C0(_DWORD *a1, _DWORD *a2, _DWORD *a3, int a4)
{
  _DWORD *v4; // esi
  int v5; // ebx
  int v6; // edi
  int result; // eax
  void *v8; // edi
  bool v9; // zf
  int v10; // edx
  int v11; // esi
  float *v12; // ecx
  int v13; // esi
  float *v14; // edi
  int v15; // edx
  _DWORD *v16; // [esp+14h] [ebp-3Ch]
  float v17; // [esp+14h] [ebp-3Ch]
  float v18; // [esp+14h] [ebp-3Ch]
  int v19; // [esp+18h] [ebp-38h]
  int v20; // [esp+1Ch] [ebp-34h]
  float v21; // [esp+20h] [ebp-30h]
  _DWORD *v22; // [esp+24h] [ebp-2Ch]
  int v23; // [esp+28h] [ebp-28h]
  int v24; // [esp+2Ch] [ebp-24h]
  int v25; // [esp+2Ch] [ebp-24h]
  int v26; // [esp+30h] [ebp-20h]
  _DWORD *v27; // [esp+34h] [ebp-1Ch]
  int v28; // [esp+38h] [ebp-18h]
  int v29; // [esp+3Ch] [ebp-14h]
  float v30; // [esp+40h] [ebp-10h]
  int v31; // [esp+44h] [ebp-Ch]
  int v32; // [esp+4Ch] [ebp-4h]

  v4 = (_DWORD *)*a2;
  v5 = a4;
  v27 = (_DWORD *)*a2;
  v6 = *(_DWORD *)(*a2 + 8);
  v31 = *(_DWORD *)(*a2 + 12);
  v19 = v6;
  v20 = (*(_DWORD *)(*a2 + 4) - *(_DWORD *)*a2) / v6;
  result = sub_4C7BE0(a1, 4 * a4);
  v28 = result;
  v30 = 100.0 / (double)v6;
  if ( a4 > 0 )
  {
    v16 = (_DWORD *)result;
    v24 = a4;
    do
    {
      v8 = (void *)sub_4C7BE0(a1, 4 * v20);
      *v16 = v8;
      memset(v8, 0, 4 * ((unsigned int)(4 * v20) >> 2));
      v9 = v24 == 1;
      ++v16;
      --v24;
    }
    while ( !v9 );
    result = v28;
    v6 = v19;
  }
  v23 = 0;
  if ( v20 > 0 )
  {
    v25 = 0;
    do
    {
      v10 = v25 + *v4;
      v29 = v10;
      if ( v5 > 0 )
      {
        v26 = v5;
        v22 = a3;
        v32 = result - (_DWORD)a3;
        while ( 1 )
        {
          v21 = 0.0;
          v17 = 0.0;
          if ( v6 > 0 )
          {
            v11 = 4 * v10;
            do
            {
              v12 = (float *)(v11 + *v22);
              if ( v21 < fabs(*v12) )
                v21 = fabs(*v12);
              v11 += 4;
              --v6;
              v17 = fabs(floor(*v12 + 0.5)) + v17;
            }
            while ( v6 );
          }
          v13 = 0;
          v18 = v17 * v30;
          if ( v31 - 1 > 0 )
          {
            v14 = (float *)(v27 + 389);
            do
            {
              if ( v21 <= (double)*(v14 - 64) && (*v14 < 0.0 || (double)(int)(__int64)v18 < *v14) )
                break;
              ++v13;
              ++v14;
            }
            while ( v13 < v31 - 1 );
          }
          v15 = *(_DWORD *)((char *)v22++ + v32);
          v9 = v26 == 1;
          *(_DWORD *)(v15 + 4 * v23) = v13;
          --v26;
          if ( v9 )
            break;
          v6 = v19;
          v10 = v29;
        }
        v4 = v27;
        v5 = a4;
        result = v28;
        v6 = v19;
      }
      v25 += v6;
      ++v23;
    }
    while ( v23 < v20 );
  }
  ++a2[10];
  return result;
}

// ===== sub_4D14F0 @ 0x004D14F0..0x004D1548 =====
int __cdecl sub_4D14F0(_DWORD *a1, int a2, int a3, int a4, int a5)
{
  int v5; // esi
  int v6; // ebx
  _DWORD *v7; // ecx
  _DWORD *v8; // eax

  v5 = a5;
  v6 = 0;
  if ( a5 <= 0 )
    return 0;
  v7 = (_DWORD *)a3;
  v8 = (_DWORD *)a3;
  do
  {
    if ( *(_DWORD *)((char *)v8 + a4 - a3) )
    {
      ++v6;
      *v7++ = *v8;
    }
    ++v8;
    --v5;
  }
  while ( v5 );
  if ( v6 )
    return sub_4D0AA0(a1, a2, a3, v6, (int (__cdecl *)(int, int, _DWORD *, int))sub_4CC230);
  else
    return 0;
}

// ===== sub_4D1550 @ 0x004D1550..0x004D158F =====
int __cdecl sub_4D1550(int a1, int a2, int a3, _DWORD *a4, int a5)
{
  int v5; // edx
  int v7; // ecx

  v5 = 0;
  if ( a5 <= 0 )
    return 0;
  v7 = a5;
  do
  {
    if ( *a4 )
      ++v5;
    ++a4;
    --v7;
  }
  while ( v7 );
  if ( v5 )
    return sub_4D1590(a1, a2, a3, a5);
  else
    return 0;
}

// ===== sub_4D1590 @ 0x004D1590..0x004D172F =====
_DWORD *__cdecl sub_4D1590(_DWORD *a1, _DWORD *a2, _DWORD *a3, int a4)
{
  _DWORD *v4; // ebp
  int v5; // esi
  int v7; // esi
  void *v8; // edi
  int v9; // edi
  long double v10; // st7
  int v11; // esi
  long double v12; // st6
  int v13; // edi
  _DWORD *v14; // ecx
  float *v15; // edx
  int v16; // ecx
  float *v17; // edx
  int v19; // [esp+10h] [ebp-1Ch]
  _DWORD *v20; // [esp+14h] [ebp-18h]
  int v21; // [esp+18h] [ebp-14h]
  int v22; // [esp+1Ch] [ebp-10h]
  int v23; // [esp+20h] [ebp-Ch]
  int v24; // [esp+28h] [ebp-4h]
  int i; // [esp+30h] [ebp+4h]
  float v26; // [esp+3Ch] [ebp+10h]

  v4 = (_DWORD *)*a2;
  v24 = *(_DWORD *)(*a2 + 12);
  v5 = *(_DWORD *)(*a2 + 4) - *(_DWORD *)*a2;
  v19 = *(_DWORD *)(*a2 + 8);
  v23 = v5 / v19;
  v20 = (_DWORD *)sub_4C7BE0(a1, 4);
  v7 = 4 * (a4 * v5 / v19);
  v8 = (void *)sub_4C7BE0(a1, v7);
  *v20 = v8;
  memset(v8, 0, v7);
  v9 = 0;
  v22 = 0;
  for ( i = *v4 / a4; v9 < v23; *(_DWORD *)(*v20 + 4 * v9 - 4) = v16 )
  {
    v26 = 0.0;
    v10 = 0.0;
    v21 = 0;
    if ( v19 > 0 )
    {
      v11 = 4 * i;
      do
      {
        v12 = fabs(*(float *)(v11 + *a3));
        if ( v12 > v26 )
          v26 = v12;
        if ( a4 > 1 )
        {
          v13 = a4 - 1;
          v14 = a3 + 1;
          do
          {
            v15 = (float *)(v11 + *v14);
            if ( v10 < fabs(*v15) )
              v10 = fabs(*v15);
            ++v14;
            --v13;
          }
          while ( v13 );
        }
        v11 += 4;
        ++i;
        v21 += a4;
      }
      while ( v21 < v19 );
      v9 = v22;
    }
    v16 = 0;
    if ( v24 - 1 > 0 )
    {
      v17 = (float *)(v4 + 389);
      do
      {
        if ( v26 <= (double)*(v17 - 64) && v10 <= *v17 )
          break;
        ++v16;
        ++v17;
      }
      while ( v16 < v24 - 1 );
    }
    v22 = ++v9;
  }
  ++a2[10];
  return v20;
}

// ===== sub_4D1730 @ 0x004D1730..0x004D1889 =====
int __cdecl sub_4D1730(_DWORD *a1, _DWORD **a2, int a3, _DWORD *a4, _DWORD *a5, int a6, _DWORD *a7)
{
  int v7; // edi
  int v8; // ebp
  int v9; // eax
  int v10; // ecx
  _DWORD *v11; // edx
  int v12; // esi
  int *v13; // eax
  int v14; // ecx
  int v15; // esi
  int v16; // edx
  int v17; // edi
  _DWORD *v18; // eax
  int v19; // edx
  int v20; // ebx
  int v21; // esi
  int v22; // ecx
  int v23; // edx
  int v24; // ebx
  int v25; // esi
  int v26; // edi
  double v27; // st7
  int v29; // [esp+10h] [ebp-10h]
  int v30; // [esp+10h] [ebp-10h]
  int v31; // [esp+14h] [ebp-Ch]
  int v32; // [esp+18h] [ebp-8h] BYREF
  int v33; // [esp+1Ch] [ebp-4h]
  _DWORD *v34; // [esp+24h] [ebp+4h]
  int v35; // [esp+2Ch] [ebp+Ch]
  int v36; // [esp+34h] [ebp+14h]

  v7 = a6;
  v31 = 0;
  v8 = a1[9] / 2;
  v9 = sub_4C7BE0(a1, 4 * a6 * v8);
  v10 = 0;
  v32 = v9;
  v29 = 0;
  if ( a6 <= 0 )
    return 0;
  v11 = a5;
  v12 = a3 - (_DWORD)a5;
  v33 = a3 - (_DWORD)a5;
  do
  {
    v13 = *(int **)((char *)v11 + v12);
    if ( *v11 )
      ++v31;
    if ( v8 > 0 )
    {
      v14 = 4 * v10;
      v15 = 4 * v7;
      v16 = v8;
      do
      {
        v17 = *v13++;
        *(_DWORD *)(v14 + v32) = v17;
        v14 += v15;
        --v16;
      }
      while ( v16 );
      v7 = a6;
      v10 = v29;
      v11 = a5;
      v12 = v33;
    }
    ++v10;
    ++v11;
    v29 = v10;
    a5 = v11;
  }
  while ( v10 < v7 );
  if ( !v31 )
    return 0;
  v36 = sub_4D0FF0((int)a1, a2, (int)&v32, 1, a7, (int (__cdecl *)(int, int, int, int, _DWORD))sub_4D0DE0);
  v18 = a4;
  if ( a4 )
  {
    v19 = 0;
    v20 = a3 - (_DWORD)a4;
    v30 = 0;
    v34 = a4;
    v35 = a3 - (_DWORD)a4;
    do
    {
      v21 = *(_DWORD *)((char *)v18 + v20);
      v22 = *v18;
      if ( v8 > 0 )
      {
        v23 = 4 * v19;
        v24 = 4 * v7;
        v25 = v21 - v22;
        v26 = v8;
        do
        {
          v27 = *(float *)(v25 + v22) - *(float *)(v23 + v32);
          v22 += 4;
          v23 += v24;
          --v26;
          *(float *)(v22 - 4) = v27 + *(float *)(v22 - 4);
        }
        while ( v26 );
        v7 = a6;
        v19 = v30;
        v18 = v34;
        v20 = v35;
      }
      ++v19;
      ++v18;
      v30 = v19;
      v34 = v18;
    }
    while ( v19 < v7 );
  }
  return v36;
}

// ===== sub_4D1890 @ 0x004D1890..0x004D1A25 =====
int __cdecl sub_4D1890(_DWORD *a1, int *a2, int a3, _DWORD *a4, int a5)
{
  int *v5; // esi
  _DWORD *v6; // ebx
  int v7; // ebp
  int v8; // edi
  int i; // eax
  int v11; // eax
  int v12; // eax
  int v13; // edi
  int v14; // esi
  int v15; // eax
  _DWORD *v16; // eax
  int v18; // [esp+10h] [ebp-14h]
  _DWORD *v19; // [esp+14h] [ebp-10h]
  int v20; // [esp+18h] [ebp-Ch]
  int v21; // [esp+1Ch] [ebp-8h]
  int v22; // [esp+20h] [ebp-4h]
  int v23; // [esp+34h] [ebp+10h]

  v5 = a2;
  v6 = (_DWORD *)*a2;
  v7 = *(_DWORD *)(*a2 + 8);
  v21 = *(_DWORD *)a2[4];
  v8 = (*(_DWORD *)(*a2 + 4) - *(_DWORD *)*a2) / v7;
  v20 = v8;
  v22 = sub_4C7BE0(a1, 4 * ((v8 + v21 - 1) / v21));
  for ( i = 0; i < a5; ++a4 )
  {
    if ( *a4 )
      break;
    ++i;
  }
  if ( i != a5 )
  {
    v18 = 0;
    if ( a2[2] > 0 )
    {
      while ( 1 )
      {
        v23 = 0;
        if ( v8 > 0 )
          break;
LABEL_21:
        if ( ++v18 >= v5[2] )
          return 0;
      }
      v19 = (_DWORD *)v22;
      while ( 1 )
      {
        if ( !v18 )
        {
          v11 = sub_4CBF00((_DWORD *)v5[4], (int)(a1 + 1));
          if ( v11 == -1 )
            break;
          v12 = *(_DWORD *)(v5[7] + 4 * v11);
          *v19 = v12;
          if ( !v12 )
            break;
        }
        v13 = 0;
        if ( v21 > 0 )
        {
          v14 = v7 * v23;
          do
          {
            if ( v23 >= v20 )
              break;
            v15 = *(_DWORD *)(*v19 + 4 * v13);
            if ( ((1 << v18) & v6[v15 + 5]) != 0 )
            {
              v16 = *(_DWORD **)(*(_DWORD *)(a2[5] + 4 * v15) + 4 * v18);
              if ( v16 )
              {
                if ( sub_4CC740(v16, a3, v14 + *v6, a5, (int)(a1 + 1), v7) == -1 )
                  return 0;
              }
            }
            ++v13;
            v14 += v7;
            ++v23;
          }
          while ( v13 < v21 );
          v5 = a2;
        }
        ++v19;
        if ( v23 >= v20 )
        {
          v8 = v20;
          goto LABEL_21;
        }
      }
    }
  }
  return 0;
}

// ===== sub_4D1A30 @ 0x004D1A30..0x004D1A4F =====
void __cdecl sub_4D1A30(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x460u);
    free(Block);
  }
}

// ===== sub_4D1A50 @ 0x004D1A50..0x004D1A6F =====
void __cdecl sub_4D1A50(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x520u);
    free(Block);
  }
}

// ===== sub_4D1A70 @ 0x004D1A70..0x004D1C2E =====
int __cdecl sub_4D1A70(int *a1, int a2)
{
  int *v2; // ebp
  int v4; // esi
  int *v5; // ebx
  int v6; // eax
  _DWORD *v7; // esi
  int v8; // ebx
  _DWORD *v9; // ebp
  bool v10; // zf
  int v11; // eax
  int result; // eax
  int *v13; // ebx
  int v14; // esi
  bool v15; // cc
  int v16; // [esp+10h] [ebp-Ch]
  int v17; // [esp+14h] [ebp-8h]
  int v18; // [esp+18h] [ebp-4h]
  int v19; // [esp+18h] [ebp-4h]
  _DWORD *v20; // [esp+20h] [ebp+4h]
  int v21; // [esp+24h] [ebp+8h]
  _DWORD *v22; // [esp+24h] [ebp+8h]
  int v23; // [esp+24h] [ebp+8h]

  v2 = a1;
  v16 = 0;
  v18 = a1[210];
  v4 = -1;
  sub_4D4B20(a2, *a1, 5);
  v21 = 0;
  if ( *a1 > 0 )
  {
    v5 = a1 + 1;
    do
    {
      sub_4D4B20(a2, *v5, 4);
      if ( v4 < *v5 )
        v4 = *v5;
      ++v5;
      ++v21;
    }
    while ( v21 < *a1 );
  }
  v6 = v4 + 1;
  if ( v4 + 1 > 0 )
  {
    v7 = a1 + 48;
    v22 = a1 + 80;
    v17 = v6;
    do
    {
      sub_4D4B20(a2, *(v7 - 16) - 1, 3);
      sub_4D4B20(a2, *v7, 2);
      if ( *v7 )
        sub_4D4B20(a2, v7[16], 8);
      v8 = 0;
      if ( 1 << *v7 > 0 )
      {
        v9 = v22;
        do
        {
          sub_4D4B20(a2, *v9 + 1, 8);
          ++v8;
          ++v9;
        }
        while ( v8 < 1 << *v7 );
        v2 = a1;
      }
      ++v7;
      v10 = v17 == 1;
      v22 += 8;
      --v17;
    }
    while ( !v10 );
  }
  sub_4D4B20(a2, v2[208] - 1, 2);
  v11 = sub_4D1C30(v18);
  sub_4D4B20(a2, v11, 4);
  v19 = sub_4D1C30(v18);
  result = 0;
  v23 = 0;
  if ( *v2 > 0 )
  {
    v20 = v2 + 1;
    do
    {
      v16 += v2[*v20 + 32];
      if ( result < v16 )
      {
        v13 = &v2[result + 211];
        v14 = v16 - result;
        do
        {
          sub_4D4B20(a2, *v13++, v19);
          --v14;
        }
        while ( v14 );
        result = v16;
      }
      v15 = ++v23 < *v2;
      ++v20;
    }
    while ( v15 );
  }
  return result;
}

// ===== sub_4D1C30 @ 0x004D1C30..0x004D1C43 =====
int __cdecl sub_4D1C30(int a1)
{
  int result; // eax
  unsigned int v2; // ecx

  result = 0;
  if ( a1 )
  {
    v2 = a1 - 1;
    if ( a1 != 1 )
    {
      do
      {
        ++result;
        v2 >>= 1;
      }
      while ( v2 );
    }
  }
  return result;
}

// ===== sub_4D1C50 @ 0x004D1C50..0x004D1E7A =====
int *__cdecl sub_4D1C50(int a1, int a2)
{
  int v2; // esi
  int v3; // edi
  int *v5; // ebp
  int v6; // eax
  int v7; // eax
  int v8; // edi
  int *v9; // esi
  int v10; // eax
  int v11; // ebp
  int *v12; // edi
  int v13; // eax
  bool v14; // cc
  int v15; // edi
  int *v16; // esi
  int v17; // eax
  int v19; // [esp+10h] [ebp-10h]
  int *v20; // [esp+14h] [ebp-Ch]
  int v21; // [esp+14h] [ebp-Ch]
  int v22; // [esp+18h] [ebp-8h]
  int v23; // [esp+1Ch] [ebp-4h]
  int *v24; // [esp+24h] [ebp+4h]
  int v25; // [esp+24h] [ebp+4h]
  int v26; // [esp+24h] [ebp+4h]
  int *v27; // [esp+28h] [ebp+8h]
  _DWORD *v28; // [esp+28h] [ebp+8h]

  v2 = 0;
  v22 = *(_DWORD *)(a1 + 28);
  v19 = 0;
  v3 = -1;
  v5 = (int *)calloc(1u, 0x460u);
  v20 = v5;
  v6 = sub_4D4D00(a2, 5);
  *v5 = v6;
  if ( v6 > 0 )
  {
    v24 = v5 + 1;
    do
    {
      v7 = sub_4D4D00(a2, 4);
      *v24 = v7;
      if ( v3 < v7 )
        v3 = v7;
      ++v2;
      ++v24;
    }
    while ( v2 < *v5 );
  }
  v8 = v3 + 1;
  v25 = 0;
  v23 = v8;
  if ( v8 <= 0 )
  {
LABEL_20:
    v5[208] = sub_4D4D00(a2, 2) + 1;
    v21 = sub_4D4D00(a2, 4);
    v15 = 0;
    v26 = 0;
    if ( *v5 <= 0 )
    {
LABEL_28:
      v5[209] = 0;
      v5[210] = 1 << v21;
      return v5;
    }
    v28 = v5 + 1;
    while ( 1 )
    {
      v19 += v5[*v28 + 32];
      if ( v15 < v19 )
        break;
LABEL_27:
      v14 = ++v26 < *v5;
      ++v28;
      if ( !v14 )
        goto LABEL_28;
    }
    v16 = &v5[v15 + 211];
    while ( 1 )
    {
      v17 = sub_4D4D00(a2, v21);
      *v16 = v17;
      if ( v17 < 0 || v17 >= 1 << v21 )
        break;
      ++v15;
      ++v16;
      if ( v15 >= v19 )
        goto LABEL_27;
    }
  }
  else
  {
    v9 = v5 + 64;
    v27 = v5 + 80;
    while ( 1 )
    {
      *(v9 - 32) = sub_4D4D00(a2, 3) + 1;
      v10 = sub_4D4D00(a2, 2);
      *(v9 - 16) = v10;
      if ( v10 < 0 )
        break;
      if ( v10 )
        *v9 = sub_4D4D00(a2, 8);
      if ( *v9 < 0 || *v9 >= *(_DWORD *)(v22 + 24) )
        break;
      v11 = 0;
      if ( 1 << *(v9 - 16) > 0 )
      {
        v12 = v27;
        while ( 1 )
        {
          v13 = sub_4D4D00(a2, 8) - 1;
          *v12 = v13;
          if ( v13 < -1 || v13 >= *(_DWORD *)(v22 + 24) )
            break;
          ++v11;
          ++v12;
          if ( v11 >= 1 << *(v9 - 16) )
          {
            v8 = v23;
            goto LABEL_19;
          }
        }
        v5 = v20;
        break;
      }
LABEL_19:
      v5 = v20;
      ++v9;
      v14 = ++v25 < v8;
      v27 += 8;
      if ( !v14 )
        goto LABEL_20;
    }
  }
  sub_4D1A30(v5);
  return 0;
}

// ===== sub_4D1E80 @ 0x004D1E80..0x004D2082 =====
_DWORD *__cdecl sub_4D1E80(int a1, int *a2)
{
  _DWORD *v2; // ebp
  int v3; // esi
  int v4; // ecx
  int *v5; // eax
  int v6; // edx
  signed int v7; // esi
  _DWORD *v8; // ecx
  _DWORD *v9; // eax
  signed int v10; // edx
  int *v11; // eax
  _DWORD *v12; // ecx
  signed int v13; // edx
  int v14; // ebx
  signed int v15; // eax
  int *v16; // ecx
  int v17; // edx
  _DWORD *v18; // eax
  signed int v19; // ecx
  int v20; // edx
  int *v21; // eax
  int v22; // edx
  int v23; // ebx
  int v24; // esi
  int v25; // edi
  int v26; // ecx
  int v27; // eax
  int *v29; // [esp+10h] [ebp-120h]
  int *v30; // [esp+14h] [ebp-11Ch]
  int v31; // [esp+18h] [ebp-118h]
  int v32; // [esp+1Ch] [ebp-114h]
  int *v33; // [esp+20h] [ebp-110h]
  _DWORD *v34; // [esp+24h] [ebp-10Ch]
  int v35; // [esp+28h] [ebp-108h]
  _BYTE Base[260]; // [esp+2Ch] [ebp-104h] BYREF

  v2 = calloc(1u, 0x520u);
  v3 = 0;
  v2[324] = a2;
  v2[322] = a2[210];
  v4 = *a2;
  v34 = v2;
  if ( *a2 > 0 )
  {
    v5 = a2 + 1;
    do
    {
      v6 = *v5++;
      v3 += a2[v6 + 32];
      --v4;
    }
    while ( v4 );
  }
  v7 = v3 + 2;
  v2[321] = v7;
  if ( v7 > 0 )
  {
    v8 = Base;
    v9 = a2 + 209;
    v10 = v7;
    do
    {
      *v8++ = v9++;
      --v10;
    }
    while ( v10 );
  }
  qsort(Base, v7, 4u, sub_4D20A0);
  if ( v7 > 0 )
  {
    v11 = (int *)Base;
    v12 = v2 + 65;
    v13 = v7;
    do
    {
      v14 = *v11++;
      *v12++ = (v14 - (int)a2 - 836) >> 2;
      --v13;
    }
    while ( v13 );
  }
  v15 = 0;
  if ( v7 > 0 )
  {
    v16 = v2 + 65;
    do
    {
      v17 = *v16++;
      v2[v17 + 130] = v15++;
    }
    while ( v15 < v7 );
    v18 = v2;
    v19 = v7;
    do
    {
      v20 = v18[65];
      ++v18;
      --v19;
      *(v18 - 1) = a2[v20 + 209];
    }
    while ( v19 );
  }
  switch ( a2[208] )
  {
    case 1:
      v2[323] = 256;
      break;
    case 2:
      v2[323] = 128;
      break;
    case 3:
      v2[323] = 86;
      break;
    case 4:
      v2[323] = 64;
      break;
    default:
      break;
  }
  v35 = v7 - 2;
  if ( v7 - 2 > 0 )
  {
    v21 = v2 + 195;
    v22 = 2;
    v33 = v2 + 195;
    v30 = a2 + 211;
    do
    {
      v23 = 0;
      v31 = v2[322];
      v24 = 0;
      v25 = 1;
      v32 = *v30;
      v26 = 0;
      if ( v22 > 0 )
      {
        v29 = a2 + 209;
        do
        {
          v27 = *v29;
          if ( *v29 > v24 && v27 < v32 )
          {
            v23 = v26;
            v24 = *v29;
          }
          if ( v27 < v31 && v27 > v32 )
          {
            v25 = v26;
            v31 = *v29;
          }
          ++v26;
          ++v29;
        }
        while ( v26 < v22 );
        v2 = v34;
        v21 = v33;
      }
      v21[63] = v23;
      *v21++ = v25;
      ++v22;
      ++v30;
      v33 = v21;
    }
    while ( v22 - 2 < v35 );
  }
  return v2;
}

// ===== sub_4D20A0 @ 0x004D20A0..0x004D20B5 =====
int __cdecl sub_4D20A0(_DWORD **a1, _DWORD **a2)
{
  return **a1 - **a2;
}

// ===== sub_4D20C0 @ 0x004D20C0..0x004D254F =====
_DWORD *__cdecl sub_4D20C0(_DWORD *a1, _DWORD *a2, int a3, int a4)
{
  _DWORD *v4; // ebp
  signed int v5; // ebx
  int v6; // esi
  int v7; // edx
  bool v8; // zf
  int v9; // eax
  _DWORD *v10; // esi
  _DWORD *v11; // edi
  int v12; // edx
  int v13; // eax
  int v14; // edi
  int v15; // esi
  int v16; // ebx
  int v17; // ebp
  int v18; // edx
  int v19; // eax
  int v20; // ebx
  int v21; // eax
  int v22; // ecx
  int v23; // edx
  int v24; // eax
  int v25; // eax
  int *v26; // ecx
  signed int v27; // eax
  int *v28; // ecx
  bool v29; // cc
  _DWORD *v30; // esi
  int v31; // ebx
  _DWORD *v32; // edi
  int v33; // ebp
  int v34; // ebx
  int v35; // eax
  int v37; // [esp-4h] [ebp-E58h]
  int v38; // [esp+10h] [ebp-E44h]
  int v39; // [esp+10h] [ebp-E44h]
  int *v40; // [esp+10h] [ebp-E44h]
  int v41; // [esp+14h] [ebp-E40h]
  int *v42; // [esp+14h] [ebp-E40h]
  _DWORD *v43; // [esp+14h] [ebp-E40h]
  int v44; // [esp+18h] [ebp-E3Ch]
  int v45; // [esp+18h] [ebp-E3Ch]
  signed int v46; // [esp+1Ch] [ebp-E38h]
  int v47; // [esp+20h] [ebp-E34h] BYREF
  int v48; // [esp+24h] [ebp-E30h]
  int v49; // [esp+28h] [ebp-E2Ch] BYREF
  int v50; // [esp+2Ch] [ebp-E28h] BYREF
  int v51; // [esp+30h] [ebp-E24h] BYREF
  int v52; // [esp+34h] [ebp-E20h]
  int v53; // [esp+38h] [ebp-E1Ch] BYREF
  int v54; // [esp+3Ch] [ebp-E18h] BYREF
  _DWORD v55[65]; // [esp+40h] [ebp-E14h] BYREF
  int v56; // [esp+144h] [ebp-D10h] BYREF
  int v57; // [esp+148h] [ebp-D0Ch]
  _DWORD v58[63]; // [esp+14Ch] [ebp-D08h]
  _DWORD v59[65]; // [esp+248h] [ebp-C0Ch] BYREF
  _DWORD v60[65]; // [esp+34Ch] [ebp-B08h] BYREF
  _DWORD v61[65]; // [esp+450h] [ebp-A04h] BYREF
  _DWORD v62[576]; // [esp+554h] [ebp-900h] BYREF

  v4 = a2;
  v38 = 0;
  v5 = a2[321];
  v6 = a2[324];
  v7 = a2[322];
  v48 = v6;
  v8 = v5 == 0;
  v47 = v7;
  v46 = v5;
  if ( v5 > 0 )
  {
    memset32(v55, -200, v5);
    memset32(&v56, -200, v5);
    memset(v60, 0, 4 * v5);
    memset32(v59, 1, v5);
    memset(v61, 0xFFu, 4 * v5);
    v8 = v5 == 0;
  }
  if ( v8 )
  {
    v9 = sub_4D25A0(a4, a3, 0, v7, v62, v7, v6);
  }
  else
  {
    if ( v5 - 1 <= 0 )
      return 0;
    v10 = a2;
    v11 = v62;
    v41 = v5 - 1;
    do
    {
      v12 = sub_4D25A0(a4, a3, *v10, v10[1], v11, v47, v48) + v38;
      v11 += 9;
      ++v10;
      v8 = v41 == 1;
      v38 = v12;
      --v41;
    }
    while ( !v8 );
    v9 = v12;
  }
  if ( !v9 )
    return 0;
  v54 = -200;
  v53 = -200;
  sub_4D27D0(v62, v5 - 1, &v54, &v53);
  v55[0] = v54;
  v56 = v54;
  v57 = v53;
  v55[1] = v53;
  v44 = 2;
  if ( v5 > 2 )
  {
    v39 = 0;
    v42 = a2 + 132;
    do
    {
      v13 = *v42;
      v52 = v13;
      v14 = v60[v13];
      v15 = v59[v13];
      if ( v61[v14] != v15 )
      {
        v16 = v4[v14 + 130];
        v17 = v4[v15 + 130];
        v37 = v60[v13];
        v18 = *(_DWORD *)(v48 + 4 * v15 + 836);
        v49 = *(_DWORD *)(v48 + 4 * v14 + 836);
        v61[v14] = v15;
        v50 = v18;
        v47 = sub_4D2C00(v55, &v56, v37);
        v19 = sub_4D2C00(v55, &v56, v15);
        if ( sub_4D29D0(v49, v50, v47, v19, a4, a3, v48) )
        {
          v51 = -200;
          v49 = -200;
          v50 = -200;
          v47 = -200;
          sub_4D27D0(&v62[9 * v16], v52 - v16, &v51, &v49);
          v20 = v52;
          sub_4D27D0(&v62[9 * v52], v17 - v52, &v50, &v47);
          v21 = v51;
          *(&v56 + v14) = v51;
          if ( !v14 )
            v55[0] = v21;
          v22 = v49;
          v23 = v50;
          v55[v39 + 2] = v49;
          v58[v39] = v23;
          v24 = v47;
          v55[v15] = v47;
          if ( v15 == 1 )
            v57 = v24;
          if ( v22 >= 0 || v23 >= 0 )
          {
            v25 = v20 - 1;
            if ( v20 - 1 >= 0 )
            {
              v26 = &v59[v25];
              do
              {
                if ( *v26 != v15 )
                  break;
                --v25;
                *v26-- = v44;
              }
              while ( v25 >= 0 );
            }
            v27 = v20 + 1;
            if ( v20 + 1 < v46 )
            {
              v28 = &v60[v27];
              do
              {
                if ( *v28 != v14 )
                  break;
                ++v27;
                *v28++ = v44;
              }
              while ( v27 < v46 );
            }
          }
        }
        else
        {
          v55[v39 + 2] = -200;
          v58[v39] = -200;
        }
        v4 = a2;
        v5 = v46;
      }
      v29 = ++v44 < v5;
      ++v42;
      ++v39;
    }
    while ( v29 );
  }
  v30 = (_DWORD *)sub_4C7BE0(a1, 4 * v5);
  *v30 = sub_4D2C00(v55, &v56, 0);
  v30[1] = sub_4D2C00(v55, &v56, 1);
  v45 = 2;
  if ( v5 > 2 )
  {
    v31 = v48;
    v40 = v30 + 2;
    v32 = v4 + 195;
    v43 = (_DWORD *)(v48 + 844);
    while ( 1 )
    {
      v33 = v45;
      v34 = sub_4D2550(
              *(_DWORD *)(v31 + 4 * v32[63] + 836),
              *(_DWORD *)(v31 + 4 * *v32 + 836),
              v30[v32[63]],
              v30[*v32],
              *v43);
      v35 = sub_4D2C00(v55, &v56, v45);
      if ( v35 < 0 || v34 == v35 )
      {
        BYTE1(v34) |= 0x80u;
        *v40 = v34;
      }
      else
      {
        *v40 = v35;
      }
      ++v32;
      ++v45;
      ++v43;
      ++v40;
      if ( v33 + 1 >= v46 )
        break;
      v31 = v48;
    }
  }
  return v30;
}

// ===== sub_4D2550 @ 0x004D2550..0x004D2598 =====
int __cdecl sub_4D2550(int a1, int a2, __int16 a3, __int16 a4, int a5)
{
  int v5; // esi
  int v6; // eax

  v5 = a3 & 0x7FFF;
  v6 = (int)((a5 - a1) * abs32((a4 & 0x7FFF) - v5)) / (a2 - a1);
  if ( (a4 & 0x7FFF) - v5 >= 0 )
    return v6 + v5;
  else
    return v5 - v6;
}

// ===== sub_4D25A0 @ 0x004D25A0..0x004D2793 =====
int __cdecl sub_4D25A0(int a1, int a2, int a3, int a4, _DWORD *a5, int a6, int a7)
{
  int v7; // esi
  int v9; // edi
  int v10; // ebp
  int v11; // ecx
  __int64 v12; // rax
  int result; // eax
  int v14; // [esp+10h] [ebp-2Ch]
  int v15; // [esp+14h] [ebp-28h]
  int v16; // [esp+18h] [ebp-24h]
  int v17; // [esp+1Ch] [ebp-20h]
  int v18; // [esp+20h] [ebp-1Ch]
  int v19; // [esp+24h] [ebp-18h]
  int v20; // [esp+28h] [ebp-14h]
  int v21; // [esp+2Ch] [ebp-10h]
  int v22; // [esp+30h] [ebp-Ch]
  int v23; // [esp+34h] [ebp-8h]
  float *v24; // [esp+38h] [ebp-4h]
  int v25; // [esp+40h] [ebp+4h]
  int v26; // [esp+48h] [ebp+Ch]
  int v27; // [esp+48h] [ebp+Ch]
  float *v28; // [esp+50h] [ebp+14h]

  v7 = a3;
  v24 = (float *)(a1 + 4 * a3);
  sub_4D27A0(v24);
  memset(a5, 0, 0x24u);
  v9 = a4;
  v10 = 0;
  v14 = 0;
  v16 = 0;
  v18 = 0;
  v20 = 0;
  v22 = 0;
  v15 = 0;
  v17 = 0;
  v19 = 0;
  v21 = 0;
  v23 = 0;
  v26 = 0;
  *a5 = v7;
  a5[1] = a4;
  if ( a4 >= a6 )
    v9 = a6 - 1;
  if ( v7 <= v9 )
  {
    v28 = v24;
    v25 = a2 - a1;
    do
    {
      v11 = sub_4D27A0(v28);
      if ( v11 )
      {
        if ( *(float *)((char *)v28 + v25) + *(float *)(a7 + 1112) < *v28 )
        {
          v17 += v11;
          v15 += v7;
          v19 += v7 * v7;
          v21 += v11 * v11;
          v23 += v7 * v11;
          ++v26;
        }
        else
        {
          v16 += v11;
          v14 += v7;
          v18 += v7 * v7;
          v20 += v11 * v11;
          v22 += v7 * v11;
          ++v10;
        }
      }
      ++v7;
      ++v28;
    }
    while ( v7 <= v9 );
  }
  v27 = v10 + v26;
  v12 = (__int64)((double)v27 * *(float *)(a7 + 1108) / (double)(v10 + 1));
  a5[2] = v15 + v14 * (v12 + 1);
  a5[3] = v17 + v16 * (v12 + 1);
  a5[4] = v19 + v18 * (v12 + 1);
  HIDWORD(v12) = v21 + v20 * (v12 + 1);
  a5[6] = v23 + v22 * (v12 + 1);
  a5[8] = v27 + v10 * v12;
  result = v10;
  a5[5] = HIDWORD(v12);
  a5[7] = v27;
  return result;
}

// ===== sub_4D27A0 @ 0x004D27A0..0x004D27CF =====
int __cdecl sub_4D27A0(float *a1)
{
  __int64 v1; // rax

  v1 = (__int64)(*a1 * 7.3142858 + 1023.5);
  if ( (int)v1 <= 1023 )
    return (int)v1 < 0 ? 0 : v1;
  else
    return 1023;
}

// ===== sub_4D27D0 @ 0x004D27D0..0x004D29C9 =====
int __cdecl sub_4D27D0(int *a1, int a2, int *a3, int *a4)
{
  int v4; // ecx
  int v5; // edx
  int v6; // esi
  int v7; // edi
  int v8; // ebx
  int v9; // eax
  _DWORD *v10; // eax
  int v11; // ebp
  int v12; // ebp
  double v13; // st7
  double v14; // st6
  double v15; // st5
  double v16; // st4
  double v17; // st3
  int result; // eax
  int v19; // [esp+18h] [ebp-2Ch]
  int v20; // [esp+1Ch] [ebp-28h]
  int v21; // [esp+20h] [ebp-24h]
  int v22; // [esp+24h] [ebp-20h]
  int v23; // [esp+28h] [ebp-1Ch]
  int v24; // [esp+2Ch] [ebp-18h]
  int v25; // [esp+30h] [ebp-14h]
  double v26; // [esp+30h] [ebp-14h]
  int v27; // [esp+38h] [ebp-Ch]
  double v28; // [esp+38h] [ebp-Ch]
  double v29; // [esp+38h] [ebp-Ch]
  int v30; // [esp+40h] [ebp-4h]

  v4 = 0;
  v5 = 0;
  v30 = *a1;
  v6 = 0;
  v7 = 0;
  v8 = 0;
  v19 = 0;
  v20 = 0;
  v9 = a1[9 * a2 - 8];
  v21 = 0;
  v25 = 0;
  v22 = 0;
  v27 = 0;
  v23 = 0;
  v24 = v9;
  if ( a2 > 0 )
  {
    v10 = a1 + 3;
    do
    {
      v11 = *(v10 - 1);
      v10 += 9;
      v4 += v11;
      v5 += *(v10 - 9);
      v6 += *(v10 - 8);
      v25 += *(v10 - 7);
      v7 += *(v10 - 6);
      v27 += *(v10 - 5);
      v8 += *(v10 - 4);
      --a2;
    }
    while ( a2 );
    v9 = v24;
    v23 = v8;
    v22 = v7;
    v21 = v6;
    v20 = v5;
    v19 = v4;
  }
  if ( *a3 >= 0 )
  {
    v5 += *a3;
    v4 += v30;
    v6 += v30 * v30;
    v9 = v24;
    v7 += v30 * *a3;
    ++v8;
    v19 = v4;
    v20 = v5;
    v21 = v6;
    v22 = v7;
    v23 = v8;
  }
  v12 = *a4;
  if ( *a4 >= 0 )
  {
    v19 = v9 + v4;
    v20 = v12 + v5;
    v21 = v9 * v9 + v6;
    v22 = v9 * v12 + v7;
    v23 = v8 + 1;
  }
  v13 = (double)v19;
  v14 = (double)v20;
  v28 = (double)v21;
  v15 = (double)v22;
  v16 = (double)v23;
  v17 = 1.0 / (v16 * v28 - v13 * v13);
  v26 = (v28 * v14 - v15 * v13) * v17;
  v29 = (v16 * v15 - v14 * v13) * v17;
  *a3 = (__int64)floor((double)v30 * v29 + v26 + 0.5);
  *a4 = (__int64)floor((double)v24 * v29 + v26 + 0.5);
  if ( *a3 > 1023 )
    *a3 = 1023;
  if ( *a4 > 1023 )
    *a4 = 1023;
  if ( *a3 < 0 )
    *a3 = 0;
  result = *a4;
  if ( *a4 < 0 )
    *a4 = 0;
  return result;
}

// ===== sub_4D29D0 @ 0x004D29D0..0x004D2BFC =====
BOOL __cdecl sub_4D29D0(int a1, int a2, int a3, int a4, int a5, int a6, float *a7)
{
  unsigned int v8; // ebx
  int v9; // eax
  int v10; // eax
  float *v11; // edi
  float *v12; // ecx
  int v13; // ebx
  double v14; // st7
  double v15; // st6
  int v17; // ebp
  float *v18; // esi
  int v19; // eax
  int v20; // eax
  int v21; // ecx
  double v22; // st7
  double v23; // st6
  int v24; // [esp+10h] [ebp-18h]
  int v25; // [esp+14h] [ebp-14h]
  int v26; // [esp+18h] [ebp-10h]
  int v27; // [esp+1Ch] [ebp-Ch]
  int v28; // [esp+20h] [ebp-8h]
  unsigned int v29; // [esp+24h] [ebp-4h]
  int v30; // [esp+2Ch] [ebp+4h]
  float v31; // [esp+30h] [ebp+8h]
  int v32; // [esp+38h] [ebp+10h]

  v8 = abs32(a4 - a3);
  v26 = a2 - a1;
  v9 = (a4 - a3) / (a2 - a1);
  v28 = v9;
  if ( a4 - a3 >= 0 )
    v10 = v9 + 1;
  else
    v10 = v9 - 1;
  v27 = v10;
  v32 = a3;
  v24 = 0;
  v11 = (float *)(a5 + 4 * a1);
  v25 = sub_4D27A0(v11);
  v12 = a7;
  v29 = v8 - abs32(v26 * v28);
  v13 = (a3 - v25) * (a3 - v25);
  v30 = 1;
  if ( *(float *)(a6 + 4 * a1) + a7[278] >= *v11 )
  {
    v14 = (double)a3;
    v15 = (double)v25;
    if ( v14 + a7[274] < v15 )
      return 1;
    if ( v14 - a7[275] > v15 )
      return 1;
  }
  v17 = a1 + 1;
  if ( v17 < a2 )
  {
    v18 = (float *)(a5 + 4 * v17);
    while ( 1 )
    {
      v19 = v29 + v24;
      v24 += v29;
      if ( v24 < v26 )
      {
        v20 = v28 + v32;
      }
      else
      {
        v24 = v19 - v26;
        v20 = v27 + v32;
      }
      v32 = v20;
      v21 = sub_4D27A0(v18);
      v13 += (v32 - v21) * (v32 - v21);
      ++v30;
      if ( *(float *)((char *)v18 + a6 - a5) + a7[278] >= *v18 )
      {
        if ( v21 )
        {
          v22 = (double)v32;
          v23 = (double)v21;
          if ( v22 + a7[274] < v23 )
            return 1;
          if ( v22 - a7[275] > v23 )
            return 1;
        }
      }
      ++v17;
      ++v18;
      if ( v17 >= a2 )
      {
        v12 = a7;
        break;
      }
    }
  }
  v31 = (float)v30;
  return v12[274] * v12[274] / v31 <= v12[276]
      && v12[275] * v12[275] / v31 <= v12[276]
      && (double)(v13 / v30) > v12[276];
}

// ===== sub_4D2C00 @ 0x004D2C00..0x004D2C23 =====
int __cdecl sub_4D2C00(int a1, int a2, int a3)
{
  int result; // eax
  int v4; // ecx

  result = *(_DWORD *)(a1 + 4 * a3);
  if ( result < 0 )
    return *(_DWORD *)(a2 + 4 * a3);
  v4 = *(_DWORD *)(a2 + 4 * a3);
  if ( v4 >= 0 )
    return (v4 + result) >> 1;
  return result;
}

// ===== sub_4D2C30 @ 0x004D2C30..0x004D2CD5 =====
int __cdecl sub_4D2C30(_DWORD *a1, int a2, int a3, _DWORD *a4, int a5)
{
  int v5; // ebp
  int result; // eax
  int v7; // esi
  _DWORD *v8; // ecx
  int v9; // edi
  int v10; // edx

  v5 = *(_DWORD *)(a2 + 1284);
  result = 0;
  if ( a3 )
  {
    if ( a4 )
    {
      result = sub_4C7BE0(a1, 4 * v5);
      if ( v5 > 0 )
      {
        v7 = a3 - (_DWORD)a4;
        v8 = a4;
        v9 = result - (_DWORD)a4;
        do
        {
          v10 = ((0x10000 - a5) * (*(_DWORD *)((char *)v8 + v7) & 0x7FFF) + a5 * (*v8 & 0x7FFF) + 0x8000) >> 16;
          *(_DWORD *)((char *)v8 + v9) = v10;
          if ( (BYTE1(*(_DWORD *)((char *)v8 + v7)) & 0x80u) != 0 && (BYTE1(*v8) & 0x80u) != 0 )
          {
            BYTE1(v10) |= 0x80u;
            *(_DWORD *)((char *)v8 + v9) = v10;
          }
          ++v8;
          --v5;
        }
        while ( v5 );
      }
    }
  }
  return result;
}

// ===== sub_4D2CE0 @ 0x004D2CE0..0x004D32B9 =====
int __cdecl sub_4D2CE0(int a1, _DWORD *a2, int *a3, char *a4)
{
  int v4; // esi
  _DWORD *v5; // ecx
  int *v6; // ebp
  int v7; // edx
  int v8; // ebx
  int *v9; // ebx
  int v10; // ecx
  int *v11; // ebx
  int *v12; // eax
  int v13; // esi
  int v14; // edi
  int v15; // eax
  int v16; // ecx
  int v17; // edx
  int v18; // ecx
  int v19; // ecx
  bool v20; // zf
  int v21; // ecx
  unsigned int v22; // eax
  int v23; // eax
  int v24; // eax
  int v25; // edi
  int v26; // ecx
  int v27; // edx
  int v28; // esi
  _DWORD *v29; // edx
  int *v30; // eax
  int *v31; // ebx
  int v32; // eax
  int *v33; // edx
  int v34; // eax
  int v35; // edi
  _DWORD *v36; // esi
  int *v37; // ebx
  int v38; // eax
  int v39; // eax
  bool v40; // cc
  int v41; // edi
  int v42; // ecx
  int v43; // eax
  int v44; // esi
  int v45; // ebx
  int *v46; // edi
  int v48; // [esp+10h] [ebp-174h]
  char *v49; // [esp+10h] [ebp-174h]
  int *v50; // [esp+10h] [ebp-174h]
  _DWORD *v51; // [esp+10h] [ebp-174h]
  int *v52; // [esp+14h] [ebp-170h]
  int v53; // [esp+14h] [ebp-170h]
  int *v54; // [esp+14h] [ebp-170h]
  int *v55; // [esp+18h] [ebp-16Ch]
  int v56; // [esp+18h] [ebp-16Ch]
  int v57; // [esp+1Ch] [ebp-168h]
  int v58; // [esp+1Ch] [ebp-168h]
  int v59; // [esp+1Ch] [ebp-168h]
  int v60; // [esp+20h] [ebp-164h]
  int v61; // [esp+20h] [ebp-164h]
  _DWORD *v62; // [esp+24h] [ebp-160h]
  int v63; // [esp+24h] [ebp-160h]
  int v64; // [esp+28h] [ebp-15Ch]
  int v65; // [esp+28h] [ebp-15Ch]
  int v66; // [esp+2Ch] [ebp-158h]
  int v67; // [esp+2Ch] [ebp-158h]
  int v68; // [esp+30h] [ebp-154h]
  int v69; // [esp+34h] [ebp-150h]
  int v70; // [esp+3Ch] [ebp-148h]
  _DWORD v71[8]; // [esp+40h] [ebp-144h] BYREF
  _BYTE v72[32]; // [esp+60h] [ebp-124h] BYREF
  int v73; // [esp+80h] [ebp-104h] BYREF
  int v74; // [esp+84h] [ebp-100h]

  v4 = a1;
  v5 = a2;
  v6 = (int *)a2[324];
  v7 = a2[321];
  v8 = 0;
  v52 = v6;
  v66 = v7;
  v70 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 64) + 4) + 28);
  v69 = *(_DWORD *)(v70 + 2848);
  if ( a3 )
  {
    if ( v7 > 0 )
    {
      v9 = a3;
      v48 = a2[321];
      do
      {
        v10 = *v9 & 0x7FFF;
        switch ( v6[208] )
        {
          case 1:
            v10 >>= 2;
            break;
          case 2:
            v10 >>= 3;
            break;
          case 3:
            v10 /= 12;
            v7 = v66;
            break;
          case 4:
            v10 >>= 4;
            break;
          default:
            break;
        }
        *v9 = v10 | *v9 & 0x8000;
        ++v9;
        --v48;
      }
      while ( v48 );
      v4 = a1;
      v5 = a2;
      v8 = 0;
    }
    v73 = *a3;
    v74 = a3[1];
    if ( v7 > 2 )
    {
      v11 = a3 + 2;
      v55 = v6 + 211;
      v12 = v5 + 195;
      v62 = v5 + 195;
      v49 = (char *)((char *)&v73 - (char *)a3);
      v64 = v7 - 2;
      do
      {
        v13 = v12[63];
        v14 = *v12;
        v15 = sub_4D2550(v6[v13 + 209], v6[v14 + 209], a3[v13], a3[v14], *v55);
        v16 = *v11;
        if ( (BYTE1(*v11) & 0x80u) != 0 || v15 == v16 )
        {
          BYTE1(v15) |= 0x80u;
          *v11 = v15;
          *(int *)((char *)v11 + (_DWORD)v49) = 0;
        }
        else
        {
          v17 = a2[323] - v15;
          if ( v17 >= v15 )
            v17 = v15;
          v18 = v16 - v15;
          if ( v18 >= 0 )
          {
            if ( v18 < v17 )
              v19 = 2 * v18;
            else
              v19 = v17 + v18;
          }
          else if ( v18 >= -v17 )
          {
            v19 = -1 - 2 * v18;
          }
          else
          {
            v19 = v17 - v18 - 1;
          }
          *(int *)((char *)v11 + (_DWORD)v49) = v19;
          a3[v13] &= 0x7FFFu;
          a3[v14] &= 0x7FFFu;
        }
        v6 = v52;
        v12 = v62 + 1;
        ++v11;
        v20 = v64 == 1;
        ++v62;
        ++v55;
        --v64;
      }
      while ( !v20 );
      v4 = a1;
      v8 = 0;
    }
    v56 = v4 + 4;
    sub_4D4B20(v4 + 4, 1, 1);
    ++a2[327];
    v21 = 2 * sub_4D06F0(a2[323] - 1) + a2[326];
    v22 = a2[323] - 1;
    a2[326] = v21;
    v23 = sub_4D06F0(v22);
    sub_4D4B20(v4 + 4, v73, v23);
    v24 = sub_4D06F0(a2[323] - 1);
    sub_4D4B20(v4 + 4, v74, v24);
    v67 = 0;
    v60 = 2;
    if ( *v6 > 0 )
    {
      v50 = v6 + 1;
      do
      {
        memset(v71, 0, sizeof(v71));
        v25 = *v50;
        v57 = *v50;
        v26 = v6[*v50 + 48];
        v27 = v6[*v50 + 32];
        v28 = 1 << v26;
        v63 = v27;
        v65 = 0;
        v68 = 0;
        if ( v26 )
        {
          if ( v28 > 0 )
          {
            v29 = v72;
            v30 = &v6[8 * v25 + 80];
            v53 = 1 << v26;
            do
            {
              if ( *v30 >= 0 )
              {
                v25 = v57;
                *v29 = *(_DWORD *)(*(_DWORD *)(v70 + 4 * *v30 + 1824) + 4);
              }
              else
              {
                *v29 = 1;
              }
              ++v30;
              ++v29;
              --v53;
            }
            while ( v53 );
            v27 = v63;
          }
          if ( v27 > 0 )
          {
            v31 = v71;
            v58 = v27;
            v54 = &v73 + v60;
            do
            {
              v32 = 0;
              if ( v28 > 0 )
              {
                v33 = (int *)v72;
                while ( *v54 >= *v33 )
                {
                  ++v32;
                  ++v33;
                  if ( v32 >= v28 )
                    goto LABEL_46;
                }
                *v31 = v32;
              }
LABEL_46:
              v34 = *v31++ << v68;
              ++v54;
              v65 |= v34;
              v68 += v26;
              --v58;
            }
            while ( v58 );
            v8 = 0;
          }
          a2[325] += sub_4CBEC0(v69 + 44 * v6[v25 + 64], v65, v56);
          v27 = v63;
        }
        if ( v27 > 0 )
        {
          v35 = 8 * v25 + 80;
          v36 = v71;
          v59 = v27;
          v37 = &v73 + v60;
          do
          {
            v38 = v6[*v36 + v35];
            if ( v38 >= 0 )
            {
              v39 = v69 + 44 * v38;
              if ( *v37 < *(_DWORD *)(v39 + 4) )
              {
                a2[326] += sub_4CBEC0(v39, *v37, v56);
                v27 = v63;
              }
            }
            ++v36;
            ++v37;
            --v59;
          }
          while ( v59 );
          v8 = 0;
        }
        v40 = v67 + 1 < *v6;
        v60 += v27;
        ++v67;
        ++v50;
      }
      while ( v40 );
      v4 = a1;
    }
    v41 = 0;
    v42 = *a3 * v6[208];
    v61 = 1;
    if ( (int)a2[321] > 1 )
    {
      v51 = a2 + 66;
      do
      {
        v43 = a3[*v51] & 0x7FFF;
        if ( v43 == a3[*v51] )
        {
          v41 = v6[*v51 + 209];
          v44 = v43 * v6[208];
          sub_4D32D0(v8, v41, v42, v44, a4);
          v8 = v41;
          v42 = v44;
        }
        ++v51;
        ++v61;
      }
      while ( v61 < a2[321] );
      v4 = a1;
    }
    v45 = v41;
    if ( v41 < *(_DWORD *)(v4 + 36) / 2 )
    {
      v46 = (int *)&a4[4 * v41];
      do
      {
        *v46 = v42;
        ++v45;
        ++v46;
      }
      while ( v45 < *(_DWORD *)(v4 + 36) / 2 );
    }
    ++dword_50A83C;
    return 1;
  }
  else
  {
    sub_4D4B20(a1 + 4, 0, 1);
    memset(a4, 0, 4 * (*(_DWORD *)(a1 + 36) / 2));
    ++dword_50A83C;
    return 0;
  }
}

// ===== sub_4D32D0 @ 0x004D32D0..0x004D335C =====
int __cdecl sub_4D32D0(int a1, int a2, int a3, int a4, int a5)
{
  int v6; // ebp
  int v7; // esi
  unsigned int v8; // edi
  int v9; // eax
  unsigned int v10; // edi
  int result; // eax
  _DWORD *v12; // ecx
  int v13; // ebx
  int v14; // [esp+18h] [ebp+8h]
  int v15; // [esp+20h] [ebp+10h]

  v6 = a3;
  v7 = a2 - a1;
  v8 = abs32(a4 - a3);
  v9 = (a4 - a3) / (a2 - a1);
  v14 = v9;
  if ( a4 - a3 >= 0 )
    v15 = v9 + 1;
  else
    v15 = v9 - 1;
  v10 = v8 - abs32(v7 * v9);
  *(_DWORD *)(a5 + 4 * a1) = a3;
  result = a1 + 1;
  if ( a1 + 1 < a2 )
  {
    v12 = (_DWORD *)(a5 + 4 * result);
    v13 = a2 - result;
    result = 0;
    do
    {
      result += v10;
      if ( result < v7 )
      {
        v6 += v14;
      }
      else
      {
        result -= v7;
        v6 += v15;
      }
      *v12++ = v6;
      --v13;
    }
    while ( v13 );
  }
  return result;
}

// ===== sub_4D3360 @ 0x004D3360..0x004D3647 =====
_DWORD *__cdecl sub_4D3360(_DWORD *a1, _DWORD *a2)
{
  _DWORD *v2; // ebx
  int *v3; // edi
  _DWORD *v4; // esi
  _DWORD *v5; // ebp
  int v6; // eax
  int v7; // eax
  int v8; // ebx
  int v9; // esi
  int v10; // ecx
  int v11; // edx
  int v12; // ebp
  int v13; // ebp
  int *v14; // esi
  int v15; // eax
  int v16; // eax
  int v17; // eax
  bool v18; // cc
  _DWORD *v19; // esi
  int v20; // eax
  int v21; // ecx
  int v22; // ecx
  int v24; // [esp+10h] [ebp-24h]
  int v25; // [esp+10h] [ebp-24h]
  int *v26; // [esp+14h] [ebp-20h]
  int *v27; // [esp+14h] [ebp-20h]
  int v28; // [esp+18h] [ebp-1Ch]
  int v29; // [esp+1Ch] [ebp-18h]
  int v30; // [esp+20h] [ebp-14h]
  char v31; // [esp+24h] [ebp-10h]
  int v32; // [esp+28h] [ebp-Ch]
  _DWORD *v33; // [esp+2Ch] [ebp-8h]
  int v34; // [esp+2Ch] [ebp-8h]
  int v35; // [esp+30h] [ebp-4h]
  int v36; // [esp+38h] [ebp+4h]
  int v37; // [esp+38h] [ebp+4h]
  int *v38; // [esp+3Ch] [ebp+8h]

  v2 = a2;
  v3 = (int *)a2[324];
  v4 = a1 + 1;
  v28 = (int)(a1 + 1);
  v29 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1[16] + 4) + 28) + 2848);
  if ( sub_4D4D00(a1 + 1, 1) != 1 )
    return 0;
  v5 = (_DWORD *)sub_4C7BE0(a1, 4 * a2[321]);
  v33 = v5;
  v6 = sub_4D06F0(a2[323] - 1);
  *v5 = sub_4D4D00(v4, v6);
  v7 = sub_4D06F0(a2[323] - 1);
  v5[1] = sub_4D4D00(v4, v7);
  v36 = 0;
  v24 = 2;
  if ( *v3 > 0 )
  {
    v26 = v3 + 1;
    do
    {
      v8 = 0;
      v9 = *v26;
      v10 = v3[*v26 + 48];
      v11 = v3[*v26 + 32];
      v12 = 1 << v10;
      v30 = v11;
      v31 = v10;
      if ( v10 )
      {
        v8 = sub_4CBF00((_DWORD *)(v29 + 44 * v3[v9 + 64]), v28);
        if ( v8 == -1 )
          return 0;
        v11 = v30;
        LOBYTE(v10) = v31;
      }
      v32 = 0;
      if ( v11 > 0 )
      {
        v35 = v12 - 1;
        v13 = 8 * v9 + 80;
        v14 = &v33[v24];
        do
        {
          v15 = v13 + (v8 & v35);
          v8 >>= v10;
          v16 = v3[v15];
          if ( v16 < 0 )
          {
            *v14 = 0;
          }
          else
          {
            v17 = sub_4CBF00((_DWORD *)(v29 + 44 * v16), v28);
            *v14 = v17;
            if ( v17 == -1 )
              return 0;
            v11 = v30;
            LOBYTE(v10) = v31;
          }
          ++v14;
          ++v32;
        }
        while ( v32 < v11 );
      }
      v18 = v36 + 1 < *v3;
      v24 += v11;
      ++v36;
      ++v26;
    }
    while ( v18 );
    v2 = a2;
    v5 = v33;
  }
  v37 = 2;
  if ( (int)v2[321] > 2 )
  {
    v38 = v5 + 2;
    v19 = v2 + 195;
    v27 = v3 + 211;
    do
    {
      v20 = sub_4D2550(v3[v19[63] + 209], v3[*v19 + 209], v5[v19[63]], v5[*v19], *v27);
      v21 = v2[323] - v20;
      v34 = v21;
      if ( v21 >= v20 )
        v34 = v20;
      v25 = *v38;
      if ( *v38 )
      {
        if ( v25 < 2 * v34 )
        {
          if ( (v25 & 1) != 0 )
            v22 = -((v25 + 1) >> 1);
          else
            v22 = v25 >> 1;
        }
        else if ( v21 <= v20 )
        {
          v22 = v21 - v25 - 1;
        }
        else
        {
          v22 = v25 - v20;
        }
        *v38 = v20 + v22;
        v5[v19[63]] &= 0x7FFFu;
        v5[*v19] &= 0x7FFFu;
      }
      else
      {
        BYTE1(v20) |= 0x80u;
        *v38 = v20;
      }
      ++v19;
      ++v27;
      v18 = ++v37 < v2[321];
      ++v38;
    }
    while ( v18 );
  }
  return v5;
}

// ===== sub_4D3650 @ 0x004D3650..0x004D3755 =====
int __cdecl sub_4D3650(int a1, _DWORD *a2, _DWORD *a3, char *a4)
{
  _DWORD *v4; // esi
  int v5; // ebx
  int v6; // kr00_4
  int v7; // eax
  int v8; // ecx
  int v9; // edi
  int v10; // ebp
  int v11; // esi
  int v12; // eax
  int v13; // esi
  int v14; // eax
  float *v15; // edx
  double v16; // st7
  int result; // eax
  int v18; // [esp+Ch] [ebp-8h]
  _DWORD *v19; // [esp+18h] [ebp+4h]

  v4 = a3;
  v5 = a2[324];
  v6 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 64) + 4) + 28) + 4 * *(_DWORD *)(a1 + 28));
  v7 = v6 / 2;
  if ( a3 )
  {
    v8 = *a3 * *(_DWORD *)(v5 + 832);
    v9 = 0;
    v10 = 0;
    v18 = 1;
    if ( (int)a2[321] > 1 )
    {
      v19 = a2 + 66;
      while ( 1 )
      {
        v11 = v4[*v19];
        v12 = v11 & 0x7FFF;
        if ( v12 == v11 )
        {
          v9 = *(_DWORD *)(v5 + 4 * *v19 + 836);
          v13 = v12 * *(_DWORD *)(v5 + 832);
          sub_4D3760(v10, v9, v8, v13, a4);
          v10 = v9;
          v8 = v13;
        }
        ++v19;
        if ( ++v18 >= a2[321] )
          break;
        v4 = a3;
      }
      v7 = v6 / 2;
    }
    if ( v9 < v7 )
    {
      v14 = v7 - v9;
      v15 = (float *)&a4[4 * v9];
      do
      {
        v16 = flt_502E98[v8] * *v15++;
        --v14;
        *(v15 - 1) = v16;
      }
      while ( v14 );
    }
    return 1;
  }
  else
  {
    result = 0;
    memset(a4, 0, 4 * (v6 / 2));
  }
  return result;
}

// ===== sub_4D3760 @ 0x004D3760..0x004D381C =====
int __cdecl sub_4D3760(int a1, int a2, int a3, int a4, int a5)
{
  int v6; // esi
  int v8; // edi
  int v9; // ebp
  float *v10; // edx
  int result; // eax
  float *v12; // esi
  int v13; // ebp
  int v14; // ecx
  double v15; // st7
  int v16; // [esp+14h] [ebp+4h]
  int v17; // [esp+18h] [ebp+8h]
  unsigned int v18; // [esp+20h] [ebp+10h]
  unsigned int v19; // [esp+20h] [ebp+10h]

  v6 = a4 - a3;
  v8 = a2 - a1;
  v18 = abs32(a4 - a3);
  v9 = v6 / (a2 - a1);
  if ( v6 >= 0 )
    v16 = v9 + 1;
  else
    v16 = v9 - 1;
  v17 = 0;
  v19 = v18 - abs32(v8 * v9);
  v10 = &flt_502E98[a3];
  result = a1 + 1;
  *(float *)(a5 + 4 * a1) = *v10 * *(float *)(a5 + 4 * a1);
  if ( a1 + 1 < a2 )
  {
    v12 = (float *)(a5 + 4 * result);
    v13 = 4 * v9;
    v14 = a2 - result;
    do
    {
      result = v19 + v17;
      v17 += v19;
      if ( v17 < v8 )
      {
        v10 = (float *)((char *)v10 + v13);
      }
      else
      {
        result -= v8;
        v10 += v16;
        v17 = result;
      }
      v15 = *v10 * *v12++;
      --v14;
      *(v12 - 1) = v15;
    }
    while ( v14 );
  }
  return result;
}

// ===== sub_4D3820 @ 0x004D3820..0x004D383F =====
void __cdecl sub_4D3820(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x60u);
    free(Block);
  }
}

// ===== sub_4D3840 @ 0x004D3840..0x004D38AF =====
void __cdecl sub_4D3840(void *Block)
{
  void **v1; // eax
  void *v2; // eax

  if ( Block )
  {
    v1 = (void **)*((_DWORD *)Block + 2);
    if ( v1 )
    {
      v2 = *v1;
      if ( v2 )
        free(v2);
      if ( *(_DWORD *)(*((_DWORD *)Block + 2) + 4) )
        free(*(void **)(*((_DWORD *)Block + 2) + 4));
      free(*((void **)Block + 2));
    }
    if ( *((_DWORD *)Block + 11) )
      free(*((void **)Block + 11));
    sub_4CF4A0((int)Block + 24);
    memset(Block, 0, 0x38u);
    free(Block);
  }
}

// ===== sub_4D38B0 @ 0x004D38B0..0x004D396F =====
int *__cdecl sub_4D38B0(int a1, int a2)
{
  int *v2; // esi
  int v3; // eax
  bool v4; // cc
  int v5; // ebp
  int *i; // ebx
  int v7; // eax
  int v9; // [esp+14h] [ebp+4h]

  v9 = *(_DWORD *)(a1 + 28);
  v2 = (int *)malloc(0x60u);
  *v2 = sub_4D4D00(a2, 8);
  v2[1] = sub_4D4D00(a2, 16);
  v2[2] = sub_4D4D00(a2, 16);
  v2[3] = sub_4D4D00(a2, 6);
  v2[4] = sub_4D4D00(a2, 8);
  v3 = sub_4D4D00(a2, 4) + 1;
  v4 = *v2 < 1;
  v2[5] = v3;
  if ( !v4 && v2[1] >= 1 && v2[2] >= 1 && v3 >= 1 )
  {
    v5 = 0;
    for ( i = v2 + 6; ; ++i )
    {
      v7 = sub_4D4D00(a2, 8);
      *i = v7;
      if ( v7 < 0 || v7 >= *(_DWORD *)(v9 + 24) )
        break;
      if ( ++v5 >= v2[5] )
        return v2;
    }
  }
  sub_4D3820(v2);
  return 0;
}

// ===== sub_4D3970 @ 0x004D3970..0x004D3A03 =====
int *__cdecl sub_4D3970(_DWORD *a1, int *a2)
{
  int *v2; // esi
  int v3; // edx
  void *v4; // eax
  int v5; // ecx
  void *v6; // eax
  int v7; // ecx
  int v8; // eax
  long double v9; // st7
  long double v10; // st7
  int v12; // [esp+8h] [ebp+4h]
  int v13; // [esp+Ch] [ebp+8h]

  v2 = (int *)calloc(1u, 0x38u);
  v2[1] = *a2;
  v3 = a2[2];
  v2[5] = (int)a2;
  *v2 = v3;
  if ( *a1 )
    sub_4CF460(v2 + 6, v3, v2[1]);
  v4 = calloc(2u, 4u);
  v5 = *v2;
  v2[2] = (int)v4;
  v6 = malloc(4 * v5);
  v7 = *v2;
  v2[11] = (int)v6;
  v8 = 0;
  v13 = 0;
  v12 = v7;
  if ( v7 > 0 )
  {
    do
    {
      ++v8;
      v9 = 3.1415927 / (double)v12 * (double)v13;
      v13 = v8;
      v10 = cos(v9);
      *(float *)(v2[11] + 4 * v8 - 4) = v10 + v10;
      v12 = *v2;
    }
    while ( v8 < *v2 );
  }
  return v2;
}

// ===== sub_4D3A10 @ 0x004D3A10..0x004D3B4C =====
int __cdecl sub_4D3A10(_DWORD *a1, int a2)
{
  _DWORD *v3; // esi
  int v4; // ebx
  int v5; // eax
  int v6; // eax
  int *v7; // esi
  int v8; // ebp
  int v9; // edi
  int i; // edx
  int v11; // ecx
  float *v12; // eax
  double v13; // st7
  float v15; // [esp+10h] [ebp-4h]
  int v16; // [esp+18h] [ebp+4h]
  float v17; // [esp+18h] [ebp+4h]

  v3 = *(_DWORD **)(a2 + 20);
  v4 = (int)(a1 + 1);
  v16 = sub_4D4D00(a1 + 1, v3[3]);
  if ( v16 <= 0 )
    return 0;
  v15 = (double)v16 / (double)((1 << v3[3]) - 1) * (double)(int)v3[4];
  v5 = sub_4D06F0(v3[5]);
  v6 = sub_4D4D00(v4, v5);
  if ( v6 == -1 || v6 >= v3[5] )
    return 0;
  v17 = 0.0;
  v7 = (int *)(*(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1[16] + 4) + 28) + 2848) + 44 * v3[v6 + 6]);
  v8 = sub_4C7BE0(a1, 4 * (*v7 + *(_DWORD *)(a2 + 4)) + 4);
  v9 = 0;
  if ( *(int *)(a2 + 4) > 0 )
  {
    while ( sub_4CC5C0(v7, v8 + 4 * v9, v4, *v7) != -1 )
    {
      v9 += *v7;
      if ( v9 >= *(_DWORD *)(a2 + 4) )
        goto LABEL_7;
    }
    return 0;
  }
LABEL_7:
  for ( i = 0; i < *(_DWORD *)(a2 + 4); v17 = *(float *)(v8 + 4 * i - 4) )
  {
    v11 = 0;
    if ( *v7 > 0 )
    {
      v12 = (float *)(v8 + 4 * i);
      do
      {
        v13 = v17 + *v12;
        ++v11;
        ++i;
        *v12++ = v13;
      }
      while ( v11 < *v7 );
    }
  }
  *(float *)(v8 + 4 * *(_DWORD *)(a2 + 4)) = v15;
  return v8;
}

// ===== sub_4D3B50 @ 0x004D3B50..0x004D3BD0 =====
int __cdecl sub_4D3B50(int a1, int *a2, int a3, void *a4)
{
  int v4; // edi
  float v6; // [esp+4h] [ebp-10h]

  v4 = a2[5];
  sub_4D3BD0(a1, v4, a2);
  if ( a3 )
  {
    v6 = (float)*(int *)(v4 + 16);
    sub_4D3D30(
      *(float *)&a4,
      *(_DWORD *)(a2[2] + 4 * *(_DWORD *)(a1 + 28)),
      a2[*(_DWORD *)(a1 + 28) + 3],
      *a2,
      a3,
      a2[1],
      *(float *)(a3 + 4 * a2[1]),
      v6);
    return 1;
  }
  else
  {
    memset(a4, 0, 4 * a2[*(_DWORD *)(a1 + 28) + 3]);
    return 0;
  }
}

// ===== sub_4D3BD0 @ 0x004D3BD0..0x004D3D2C =====
int __cdecl sub_4D3BD0(int a1, int a2, int *a3)
{
  int result; // eax
  int v4; // ebx
  int v5; // esi
  double v6; // st7
  int v7; // ebx
  double v8; // st6
  __int64 v9; // rax
  int i; // [esp+18h] [ebp-10h]
  int v11; // [esp+1Ch] [ebp-Ch]
  float v12; // [esp+20h] [ebp-8h]
  float v13; // [esp+24h] [ebp-4h]

  result = a1;
  v4 = *(_DWORD *)(a1 + 28);
  v11 = v4;
  if ( !*(_DWORD *)(a3[2] + 4 * v4) )
  {
    v5 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 64) + 4) + 28) + 4 * v4) / 2;
    v6 = (double)*(int *)(a2 + 4) * 0.5;
    v13 = (double)*a3
        / (atan2(v6 * v6 * 0.0000000185, 1.0) * 2.240000009536743
         + atan2(v6 * 0.00073999999, 1.0) * 13.10000038146973
         + v6 * 0.000099999997);
    *(_DWORD *)(a3[2] + 4 * v4) = malloc(4 * v5 + 4);
    v7 = 0;
    for ( i = 0; v7 < v5; *(_DWORD *)(*(_DWORD *)(a3[2] + 4 * v11) + 4 * v7 - 4) = v9 )
    {
      v12 = (float)v5;
      v8 = (double)*(int *)(a2 + 4) * 0.5 / v12 * (double)i;
      v9 = (__int64)floor(
                      (atan2(v8 * v8 * 0.0000000185, 1.0) * 2.240000009536743
                     + atan2(v8 * 0.00073999999, 1.0) * 13.10000038146973
                     + v8 * 0.000099999997)
                    * v13);
      if ( (int)v9 >= *a3 )
        LODWORD(v9) = *a3 - 1;
      i = ++v7;
    }
    result = v11;
    *(_DWORD *)(*(_DWORD *)(a3[2] + 4 * v11) + 4 * v7) = -1;
    a3[v11 + 3] = v5;
  }
  return result;
}

// ===== sub_4D3D30 @ 0x004D3D30..0x004D3EA1 =====
float *__cdecl sub_4D3D30(float a1, int a2, float *a3, int a4, float *a5, int a6, float a7, float a8)
{
  double v8; // st7
  float *v9; // esi
  int v10; // edi
  float *result; // eax
  float *v12; // esi
  int v15; // edi
  double v16; // st7
  double v17; // st7
  float *v18; // eax
  int v19; // ecx
  double v20; // st6
  double v21; // st5
  double v22; // st6
  double v23; // st7
  double v24; // st6
  double v25; // st7
  double v26; // st7
  double v27; // st6
  float v28; // [esp+8h] [ebp-20h]
  float v29; // [esp+8h] [ebp-20h]
  float v30; // [esp+1Ch] [ebp-Ch]
  int Y; // [esp+24h] [ebp-4h] BYREF
  float v32; // [esp+2Ch] [ebp+4h]
  int v33; // [esp+30h] [ebp+8h]
  int v34; // [esp+38h] [ebp+10h]
  float v35; // [esp+38h] [ebp+10h]

  v8 = 3.1415927 / (double)a4;
  v30 = v8;
  if ( a6 > 0 )
  {
    v9 = a5;
    v10 = a6;
    do
    {
      sub_4D3EB0(*v9);
      *v9++ = v8;
      --v10;
    }
    while ( v10 );
  }
  result = a3;
  v12 = 0;
  while ( (int)v12 < (int)a3 )
  {
    v15 = *(_DWORD *)(a2 + 4 * (_DWORD)v12);
    v16 = (double)v15 * v30;
    v28 = v16;
    sub_4D3EB0(v28);
    *(float *)&v34 = v16;
    v17 = 0.70710677;
    v18 = a5;
    v19 = a6 >> 1;
    v20 = 0.70710677;
    do
    {
      v21 = *v18 - *(float *)&v34;
      v18 += 2;
      --v19;
      v17 = v17 * v21;
      v20 = v20 * (*(v18 - 1) - *(float *)&v34);
    }
    while ( v19 );
    *(float *)&v33 = v20;
    if ( (a6 & 1) != 0 )
    {
      v22 = v17 * (*v18 - *(float *)&v34);
      v23 = v22 * v22;
      v24 = 1.0 - *(float *)&v34 * *(float *)&v34;
    }
    else
    {
      v23 = v17 * ((*(float *)&v34 + 1.0) * v17);
      v24 = 1.0 - *(float *)&v34;
    }
    v32 = frexp(v23 + v24 * *(float *)&v33 * *(float *)&v33, &Y);
    v25 = sub_4D3F50(a6 + Y);
    v35 = v25;
    sub_4D3F00(v32);
    v26 = v25 * v35 * a7 - a8;
    v29 = v26;
    sub_4D3F60(v29);
    result = (float *)(LODWORD(a1) + 4 * (_DWORD)v12);
    do
    {
      v27 = v26 * *result++;
      v12 = (float *)((char *)v12 + 1);
      *(result - 1) = v27;
    }
    while ( *(_DWORD *)((char *)result + a2 - LODWORD(a1)) == v15 );
  }
  return result;
}

// ===== sub_4D3EB0 @ 0x004D3EB0..0x004D3EF7 =====
double __cdecl sub_4D3EB0(float a1)
{
  double v2; // [esp+8h] [ebp-8h]

  v2 = a1 * 40.74366592;
  return (flt_5032D8[(int)(v2 - 0.5)] - flt_5032D4[(int)(v2 - 0.5)]) * (v2 - (double)(int)(v2 - 0.5))
       + flt_5032D4[(int)(v2 - 0.5)];
}

// ===== sub_4D3F00 @ 0x004D3F00..0x004D3F4D =====
double __cdecl sub_4D3F00(float a1)
{
  double v2; // [esp+8h] [ebp-8h]

  v2 = a1 * 64.0 - 32.0;
  return (flt_5034DC[(int)(v2 - 0.5)] - flt_5034D8[(int)(v2 - 0.5)]) * (v2 - (double)(int)(v2 - 0.5))
       + flt_5034D8[(int)(v2 - 0.5)];
}

// ===== sub_4D3F50 @ 0x004D3F50..0x004D3F5C =====
double __cdecl sub_4D3F50(int a1)
{
  return flt_5035DC[a1];
}

// ===== sub_4D3F60 @ 0x004D3F60..0x004D3FBA =====
double __cdecl sub_4D3F60(float a1)
{
  int v1; // eax
  double v3; // [esp+0h] [ebp-8h]

  v3 = a1 * -8.0 - 0.5;
  v1 = (int)v3;
  if ( (int)v3 < 0 )
    return 1.0;
  if ( v1 < 1120 )
    return flt_5036EC[(int)v3 & 0x1F] * flt_503660[v1 >> 5];
  return 0.0;
}

// ===== sub_4D3FC0 @ 0x004D3FC0..0x004D3FCC =====
int __cdecl sub_4D3FC0(int a1)
{
  return *(unsigned __int8 *)(*(_DWORD *)a1 + 4);
}

// ===== sub_4D3FD0 @ 0x004D3FD0..0x004D3FDD =====
int __cdecl sub_4D3FD0(int a1)
{
  return *(_BYTE *)(*(_DWORD *)a1 + 5) & 1;
}

// ===== sub_4D3FE0 @ 0x004D3FE0..0x004D3FED =====
int __cdecl sub_4D3FE0(int a1)
{
  return *(_BYTE *)(*(_DWORD *)a1 + 5) & 2;
}

// ===== sub_4D3FF0 @ 0x004D3FF0..0x004D3FFD =====
int __cdecl sub_4D3FF0(int a1)
{
  return *(_BYTE *)(*(_DWORD *)a1 + 5) & 4;
}
