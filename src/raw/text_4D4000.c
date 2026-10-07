#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4D4000 @ 0x004D4000..0x004D40D7 =====
int __cdecl sub_4D4000(int a1)
{
  return *(unsigned __int8 *)(*(_DWORD *)a1 + 6) | (((((((((((((*(unsigned __int8 *)(*(_DWORD *)a1 + 13) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 12)) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 11)) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 10)) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 9)) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 8)) << 8) | *(unsigned __int8 *)(*(_DWORD *)a1 + 7)) << 8);
}

// ===== sub_4D40E0 @ 0x004D40E0..0x004D4105 =====
int __cdecl sub_4D40E0(int a1)
{
  return *(unsigned __int8 *)(*(_DWORD *)a1 + 14) | ((*(unsigned __int8 *)(*(_DWORD *)a1 + 15) | (*(unsigned __int16 *)(*(_DWORD *)a1 + 16) << 8)) << 8);
}

// ===== sub_4D4110 @ 0x004D4110..0x004D4135 =====
int __cdecl sub_4D4110(int a1)
{
  return *(unsigned __int8 *)(*(_DWORD *)a1 + 18) | ((*(unsigned __int8 *)(*(_DWORD *)a1 + 19) | (*(unsigned __int16 *)(*(_DWORD *)a1 + 20) << 8)) << 8);
}

// ===== sub_4D4140 @ 0x004D4140..0x004D41A2 =====
int __cdecl sub_4D4140(void **a1, void *a2)
{
  if ( !a1 )
    return -1;
  memset(a1, 0, 0x168u);
  a1[1] = (void *)0x4000;
  *a1 = malloc(0x4000u);
  a1[6] = (void *)1024;
  a1[4] = malloc(0x1000u);
  a1[5] = malloc(8 * (_DWORD)a1[6]);
  a1[84] = a2;
  return 0;
}

// ===== sub_4D41B0 @ 0x004D41B0..0x004D41F5 =====
int __cdecl sub_4D41B0(void **a1)
{
  if ( a1 )
  {
    if ( *a1 )
      free(*a1);
    if ( a1[4] )
      free(a1[4]);
    if ( a1[5] )
      free(a1[5]);
    memset(a1, 0, 0x168u);
  }
  return 0;
}

// ===== sub_4D4200 @ 0x004D4200..0x004D42A1 =====
unsigned int __usercall sub_4D4200@<eax>(unsigned int result@<eax>, _DWORD *a2)
{
  unsigned int v2; // eax
  int v3; // edi
  int i; // esi
  int v5; // edi
  int j; // esi

  if ( a2 )
  {
    v2 = 0;
    *(_BYTE *)(*a2 + 22) = 0;
    *(_BYTE *)(*a2 + 23) = 0;
    *(_BYTE *)(*a2 + 24) = 0;
    *(_BYTE *)(*a2 + 25) = 0;
    v3 = a2[1];
    for ( i = 0; i < v3; ++i )
      v2 = dword_50376C[HIBYTE(v2) ^ *(unsigned __int8 *)(i + *a2)] ^ (v2 << 8);
    v5 = a2[3];
    for ( j = 0; j < v5; ++j )
      v2 = dword_50376C[HIBYTE(v2) ^ *(unsigned __int8 *)(j + a2[2])] ^ (v2 << 8);
    *(_WORD *)(*a2 + 22) = v2;
    *(_BYTE *)(*a2 + 24) = BYTE2(v2);
    result = HIBYTE(v2);
    *(_BYTE *)(*a2 + 25) = result;
  }
  return result;
}

// ===== sub_4D42B0 @ 0x004D42B0..0x004D42DF =====
void *__cdecl sub_4D42B0(int a1, int a2)
{
  void *result; // eax
  size_t v3; // eax

  result = *(void **)(a1 + 4);
  if ( (int)result <= a2 + *(_DWORD *)(a1 + 8) )
  {
    v3 = (size_t)result + a2 + 1024;
    *(_DWORD *)(a1 + 4) = v3;
    result = realloc(*(void **)a1, v3);
    *(_DWORD *)a1 = result;
  }
  return result;
}

// ===== sub_4D42E0 @ 0x004D42E0..0x004D4324 =====
void *__cdecl sub_4D42E0(int a1, int a2)
{
  void *result; // eax
  int v3; // eax
  void *v4; // ecx
  void *v5; // eax
  int v6; // edx

  result = *(void **)(a1 + 24);
  if ( (int)result <= a2 + *(_DWORD *)(a1 + 28) )
  {
    v3 = (int)result + a2 + 32;
    v4 = *(void **)(a1 + 16);
    *(_DWORD *)(a1 + 24) = v3;
    v5 = realloc(v4, 4 * v3);
    v6 = *(_DWORD *)(a1 + 24);
    *(_DWORD *)(a1 + 16) = v5;
    result = realloc(*(void **)(a1 + 20), 8 * v6);
    *(_DWORD *)(a1 + 20) = result;
  }
  return result;
}

// ===== sub_4D4330 @ 0x004D4330..0x004D4346 =====
int __cdecl sub_4D4330(void *a1)
{
  if ( a1 )
    memset(a1, 0, 0x1Cu);
  return 0;
}

// ===== sub_4D4350 @ 0x004D4350..0x004D4375 =====
int __cdecl sub_4D4350(void **a1)
{
  if ( a1 )
  {
    if ( *a1 )
      free(*a1);
    sub_4D4330(a1);
  }
  return 0;
}

// ===== sub_4D4380 @ 0x004D4380..0x004D43FA =====
int __cdecl sub_4D4380(int a1, int a2)
{
  int v2; // eax
  signed int v3; // edx
  bool v4; // zf
  int v5; // eax
  size_t v6; // edi
  void *v7; // eax
  int v8; // ecx
  void *v10; // eax

  v2 = *(_DWORD *)(a1 + 12);
  if ( v2 )
  {
    v3 = *(_DWORD *)(a1 + 8) - v2;
    v4 = *(_DWORD *)(a1 + 8) == v2;
    *(_DWORD *)(a1 + 8) = v3;
    if ( v3 >= 0 && !v4 )
      memcpy(*(void **)a1, (const void *)(*(_DWORD *)a1 + v2), v3);
    *(_DWORD *)(a1 + 12) = 0;
  }
  v5 = *(_DWORD *)(a1 + 8);
  if ( a2 > *(_DWORD *)(a1 + 4) - v5 )
  {
    v6 = v5 + a2 + 4096;
    if ( *(_DWORD *)a1 )
    {
      v7 = realloc(*(void **)a1, v6);
      v8 = *(_DWORD *)(a1 + 8);
      *(_DWORD *)(a1 + 4) = v6;
      *(_DWORD *)a1 = v7;
      return (int)v7 + v8;
    }
    v10 = malloc(v5 + a2 + 4096);
    *(_DWORD *)(a1 + 4) = v6;
    *(_DWORD *)a1 = v10;
  }
  return *(_DWORD *)(a1 + 8) + *(_DWORD *)a1;
}

// ===== sub_4D4400 @ 0x004D4400..0x004D441E =====
int __cdecl sub_4D4400(int a1, int a2)
{
  int v2; // ecx

  v2 = a2 + *(_DWORD *)(a1 + 8);
  if ( v2 > *(_DWORD *)(a1 + 4) )
    return -1;
  *(_DWORD *)(a1 + 8) = v2;
  return 0;
}

// ===== sub_4D4420 @ 0x004D4420..0x004D4567 =====
int __cdecl sub_4D4420(_DWORD *a1, int *a2)
{
  int v2; // eax
  int v3; // edi
  int v4; // ebx
  int result; // eax
  int v6; // ebp
  int v7; // eax
  int v8; // ebp
  int v9; // ecx
  char *v10; // ecx
  int v11; // eax
  int v12; // ecx
  int v13; // [esp+10h] [ebp-10h] BYREF
  int v14; // [esp+14h] [ebp-Ch]
  int v15; // [esp+18h] [ebp-8h]
  int v16; // [esp+1Ch] [ebp-4h]

  v2 = a1[3];
  v3 = v2 + *a1;
  v4 = a1[2] - v2;
  if ( !a1[5] )
  {
    if ( v4 < 27 )
      return 0;
    if ( *(_DWORD *)v3 != *(_DWORD *)aOggs )
      goto LABEL_14;
    v6 = *(unsigned __int8 *)(v3 + 26) + 27;
    if ( v4 < v6 )
      return 0;
    v7 = 0;
    if ( *(_BYTE *)(v3 + 26) )
    {
      do
        a1[6] += *(unsigned __int8 *)(v3 + v7++ + 27);
      while ( v7 < *(unsigned __int8 *)(v3 + 26) );
    }
    a1[5] = v6;
  }
  if ( a1[6] + a1[5] > v4 )
    return 0;
  v8 = *(_DWORD *)(v3 + 22);
  *(_DWORD *)(v3 + 22) = 0;
  v9 = a1[6];
  v14 = a1[5];
  v13 = v3;
  v15 = v3 + v14;
  v16 = v9;
  sub_4D4200(v3 + v14, &v13);
  if ( v8 != *(_DWORD *)(v3 + 22) )
  {
    *(_DWORD *)(v3 + 22) = v8;
LABEL_14:
    a1[5] = 0;
    a1[6] = 0;
    v10 = (char *)memchr((const void *)(v3 + 1), 79, v4 - 1);
    if ( !v10 )
      v10 = (char *)(*a1 + a1[2]);
    a1[3] = &v10[-*a1];
    return v3 - (_DWORD)v10;
  }
  v11 = a1[3] + *a1;
  if ( a2 )
  {
    *a2 = v11;
    a2[1] = a1[5];
    a2[2] = v11 + a1[5];
    a2[3] = a1[6];
  }
  v12 = a1[3];
  result = a1[6] + a1[5];
  a1[4] = 0;
  a1[3] = result + v12;
  a1[5] = 0;
  a1[6] = 0;
  return result;
}

// ===== sub_4D4570 @ 0x004D4570..0x004D4853 =====
int __cdecl sub_4D4570(int a1, int *a2)
{
  int v2; // ebp
  int v3; // ebx
  unsigned int v4; // esi
  int v5; // edx
  int v6; // eax
  int v7; // edi
  size_t v8; // ecx
  int v9; // eax
  int v11; // edx
  int v12; // ecx
  int v13; // ecx
  int *v14; // eax
  int v15; // edi
  int v16; // eax
  int v17; // eax
  int v18; // edi
  int v19; // eax
  int v20; // edx
  int v21; // eax
  int v22; // ecx
  int v23; // esi
  _DWORD *v24; // ecx
  int v25; // ecx
  int v26; // eax
  int v27; // eax
  int v28; // edx
  int v29; // ecx
  char *v30; // [esp+10h] [ebp-2Ch]
  unsigned int v31; // [esp+14h] [ebp-28h]
  int v32; // [esp+18h] [ebp-24h]
  int v33; // [esp+1Ch] [ebp-20h]
  int v34; // [esp+20h] [ebp-1Ch]
  int v35; // [esp+24h] [ebp-18h]
  int v36; // [esp+28h] [ebp-14h]
  int v37; // [esp+2Ch] [ebp-10h]
  int v38; // [esp+30h] [ebp-Ch]
  int v39; // [esp+34h] [ebp-8h]
  int v40; // [esp+38h] [ebp-4h]
  int v41; // [esp+44h] [ebp+8h]

  v2 = 0;
  v3 = *a2;
  v4 = a2[3];
  v33 = *a2;
  v30 = (char *)a2[2];
  v31 = v4;
  v36 = sub_4D3FC0((int)a2);
  v37 = sub_4D3FD0((int)a2);
  v32 = sub_4D3FE0((int)a2);
  v38 = sub_4D3FF0((int)a2);
  v39 = sub_4D4000((int)a2);
  v40 = v5;
  v35 = sub_4D40E0((int)a2);
  v34 = sub_4D4110((int)a2);
  v41 = *(unsigned __int8 *)(v3 + 26);
  v6 = *(_DWORD *)(a1 + 12);
  v7 = *(_DWORD *)(a1 + 36);
  if ( v6 )
  {
    v8 = *(_DWORD *)(a1 + 8) - v6;
    *(_DWORD *)(a1 + 8) = v8;
    if ( v8 )
      memcpy(*(void **)a1, (const void *)(*(_DWORD *)a1 + v6), v8);
    *(_DWORD *)(a1 + 12) = 0;
  }
  if ( v7 )
  {
    if ( *(_DWORD *)(a1 + 28) != v7 )
    {
      memcpy(
        *(void **)(a1 + 16),
        (const void *)(*(_DWORD *)(a1 + 16) + 4 * v7),
        4 * (*(_DWORD *)(a1 + 28) + 0x3FFFFFFF * v7));
      memcpy(
        *(void **)(a1 + 20),
        (const void *)(*(_DWORD *)(a1 + 20) + 8 * v7),
        8 * (*(_DWORD *)(a1 + 28) + 0x1FFFFFFF * v7));
    }
    v9 = *(_DWORD *)(a1 + 32) - v7;
    *(_DWORD *)(a1 + 28) -= v7;
    *(_DWORD *)(a1 + 32) = v9;
    *(_DWORD *)(a1 + 36) = 0;
  }
  if ( v35 != *(_DWORD *)(a1 + 336) )
    return -1;
  if ( v36 > 0 )
    return -1;
  sub_4D42E0(a1, v41 + 1);
  if ( v34 == *(_DWORD *)(a1 + 340) )
    goto LABEL_25;
  v11 = *(_DWORD *)(a1 + 32);
  v12 = *(_DWORD *)(a1 + 28);
  if ( v11 < v12 )
  {
    v13 = v12 - v11;
    v14 = (int *)(*(_DWORD *)(a1 + 16) + 4 * v11);
    do
    {
      v15 = *v14++;
      *(_DWORD *)(a1 + 8) -= (unsigned __int8)v15;
      --v13;
    }
    while ( v13 );
  }
  v16 = *(_DWORD *)(a1 + 340);
  *(_DWORD *)(a1 + 28) = v11;
  if ( v16 != -1 )
  {
    *(_DWORD *)(*(_DWORD *)(a1 + 16) + 4 * v11) = 1024;
    v17 = *(_DWORD *)(a1 + 32) + 1;
    ++*(_DWORD *)(a1 + 28);
    *(_DWORD *)(a1 + 32) = v17;
  }
  if ( !v37 )
  {
LABEL_25:
    v18 = v41;
  }
  else
  {
    v18 = v41;
    v32 = 0;
    if ( v41 > 0 )
    {
      while ( 1 )
      {
        v19 = *(unsigned __int8 *)(v33 + v2 + 27);
        v4 -= v19;
        v30 += v19;
        if ( v19 != 255 )
          break;
        if ( ++v2 >= v41 )
        {
          v31 = v4;
          goto LABEL_26;
        }
      }
      v31 = v4;
      ++v2;
    }
  }
LABEL_26:
  if ( v4 )
  {
    sub_4D42B0(a1, v4);
    qmemcpy((void *)(*(_DWORD *)a1 + *(_DWORD *)(a1 + 8)), v30, v4);
    v18 = v41;
    *(_DWORD *)(a1 + 8) += v31;
  }
  v20 = -1;
  if ( v2 < v18 )
  {
    do
    {
      v21 = *(unsigned __int8 *)(v33 + v2 + 27);
      *(_DWORD *)(*(_DWORD *)(a1 + 16) + 4 * *(_DWORD *)(a1 + 28)) = v21;
      v22 = *(_DWORD *)(a1 + 28);
      v23 = *(_DWORD *)(a1 + 20);
      *(_DWORD *)(v23 + 8 * v22) = -1;
      *(_DWORD *)(v23 + 8 * v22 + 4) = -1;
      if ( v32 )
      {
        v32 = 0;
        v24 = (_DWORD *)(*(_DWORD *)(a1 + 16) + 4 * *(_DWORD *)(a1 + 28));
        *v24 |= 0x100u;
      }
      if ( v21 < 255 )
        v20 = *(_DWORD *)(a1 + 28);
      v25 = *(_DWORD *)(a1 + 28) + 1;
      ++v2;
      *(_DWORD *)(a1 + 28) = v25;
      if ( v21 < 255 )
        *(_DWORD *)(a1 + 32) = v25;
    }
    while ( v2 < v18 );
    if ( v20 != -1 )
    {
      v26 = *(_DWORD *)(a1 + 20);
      *(_DWORD *)(v26 + 8 * v20) = v39;
      *(_DWORD *)(v26 + 8 * v20 + 4) = v40;
    }
  }
  if ( v38 )
  {
    v27 = *(_DWORD *)(a1 + 28);
    *(_DWORD *)(a1 + 328) = 1;
    if ( v27 > 0 )
    {
      v28 = *(_DWORD *)(a1 + 16);
      v29 = *(_DWORD *)(v28 + 4 * v27 - 4);
      BYTE1(v29) |= 2u;
      *(_DWORD *)(v28 + 4 * v27 - 4) = v29;
    }
  }
  *(_DWORD *)(a1 + 340) = v34 + 1;
  return 0;
}

// ===== sub_4D4860 @ 0x004D4860..0x004D4878 =====
int __cdecl sub_4D4860(_DWORD *a1)
{
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  a1[5] = 0;
  a1[6] = 0;
  return 0;
}

// ===== sub_4D4880 @ 0x004D4880..0x004D48CC =====
int __cdecl sub_4D4880(_DWORD *a1)
{
  a1[86] = 0;
  a1[88] = 0;
  a1[2] = 0;
  a1[3] = 0;
  a1[7] = 0;
  a1[8] = 0;
  a1[9] = 0;
  a1[81] = 0;
  a1[82] = 0;
  a1[83] = 0;
  a1[85] = -1;
  a1[87] = 0;
  a1[89] = 0;
  return 0;
}

// ===== sub_4D48D0 @ 0x004D48D0..0x004D48EC =====
int __cdecl sub_4D48D0(_DWORD *a1, int a2)
{
  sub_4D4880(a1);
  a1[84] = a2;
  return 0;
}

// ===== sub_4D48F0 @ 0x004D48F0..0x004D4905 =====
int __cdecl sub_4D48F0(int a1, int a2)
{
  return sub_4D4910(a1, a2, 1);
}

// ===== sub_4D4910 @ 0x004D4910..0x004D4A3E =====
int __cdecl sub_4D4910(_DWORD *a1, _DWORD *a2, int a3)
{
  int v4; // ebx
  int *v6; // ebp
  int v7; // ecx
  unsigned int v8; // kr00_4
  unsigned __int64 v9; // kr08_8
  int v10; // edi
  int v11; // edx
  int v12; // edx
  int v13; // edx
  int v14; // edx
  unsigned int v15; // kr04_4
  unsigned __int64 v16; // kr10_8
  int v17; // [esp+10h] [ebp+4h]

  v4 = a1[9];
  if ( a1[8] <= v4 )
    return 0;
  v6 = (int *)(a1[4] + 4 * v4);
  v7 = *v6;
  if ( (*v6 & 0x400) != 0 )
  {
    v8 = a1[86];
    a1[86] = v8 + 1;
    v9 = __PAIR64__(a1[87], v8) + 1;
    a1[9] = v4 + 1;
    a1[87] = HIDWORD(v9);
    return -1;
  }
  else if ( a2 || a3 )
  {
    v10 = (unsigned __int8)v7;
    v11 = *v6 & 0x200;
    v17 = v11;
    if ( (unsigned __int8)v7 == 255 )
    {
      do
      {
        v12 = v6[1];
        ++v6;
        ++v4;
        if ( (v12 & 0x200) != 0 )
          v17 = 512;
        v10 += (unsigned __int8)v12;
      }
      while ( (unsigned __int8)v12 == 255 );
      v11 = v17;
    }
    if ( a2 )
    {
      a2[3] = v11;
      a2[2] = v7 & 0x100;
      *a2 = *a1 + a1[3];
      a2[6] = a1[86];
      a2[7] = a1[87];
      v13 = a1[5];
      a2[4] = *(_DWORD *)(v13 + 8 * v4);
      v14 = *(_DWORD *)(v13 + 8 * v4 + 4);
      a2[1] = v10;
      a2[5] = v14;
    }
    if ( a3 )
    {
      v15 = a1[86];
      a1[3] += v10;
      a1[86] = v15 + 1;
      v16 = __PAIR64__(a1[87], v15) + 1;
      a1[9] = v4 + 1;
      a1[87] = HIDWORD(v16);
    }
    return 1;
  }
  else
  {
    return 1;
  }
}

// ===== sub_4D4A40 @ 0x004D4A40..0x004D4A55 =====
int __cdecl sub_4D4A40(_DWORD *a1, _DWORD *a2)
{
  return sub_4D4910(a1, a2, 0);
}

// ===== sub_4D4A60 @ 0x004D4A60..0x004D4A96 =====
_BYTE *__cdecl sub_4D4A60(_DWORD *a1)
{
  _BYTE *result; // eax

  *a1 = 0;
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  result = malloc(0x100u);
  a1[2] = result;
  a1[3] = result;
  *result = 0;
  a1[4] = 256;
  return result;
}

// ===== sub_4D4AA0 @ 0x004D4AA0..0x004D4ABE =====
int __cdecl sub_4D4AA0(int a1)
{
  int result; // eax

  result = 8 - *(_DWORD *)(a1 + 4);
  if ( result < 8 )
    return sub_4D4B20(a1, 0, 8 - *(_DWORD *)(a1 + 4));
  return result;
}

// ===== sub_4D4AC0 @ 0x004D4AC0..0x004D4AE3 =====
void __cdecl sub_4D4AC0(int a1)
{
  free(*(void **)(a1 + 8));
  *(_DWORD *)a1 = 0;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 12) = 0;
  *(_DWORD *)(a1 + 16) = 0;
}

// ===== sub_4D4AF0 @ 0x004D4AF0..0x004D4B18 =====
_DWORD *__cdecl sub_4D4AF0(_DWORD *a1, int a2, int a3)
{
  _DWORD *result; // eax

  result = a1;
  *a1 = 0;
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  a1[3] = a2;
  a1[2] = a2;
  a1[4] = a3;
  return result;
}

// ===== sub_4D4B20 @ 0x004D4B20..0x004D4C14 =====
int __cdecl sub_4D4B20(int a1, int a2, int a3)
{
  int v3; // eax
  char *v4; // eax
  int v5; // edi
  char *v6; // ecx
  int v7; // ecx
  unsigned int v8; // eax
  int v9; // edi
  int v10; // edx
  int v11; // ecx
  int result; // eax
  int v13; // edx

  v3 = *(_DWORD *)(a1 + 16);
  if ( *(_DWORD *)a1 + 4 >= v3 )
  {
    v4 = (char *)realloc(*(void **)(a1 + 8), v3 + 256);
    v5 = *(_DWORD *)(a1 + 16) + 256;
    v6 = &v4[*(_DWORD *)a1];
    *(_DWORD *)(a1 + 8) = v4;
    *(_DWORD *)(a1 + 16) = v5;
    *(_DWORD *)(a1 + 12) = v6;
  }
  v7 = *(_DWORD *)(a1 + 4);
  v8 = dword_503B74[a3] & a2;
  v9 = v7 + a3;
  **(_BYTE **)(a1 + 12) |= (dword_503B74[a3] & (unsigned __int8)a2) << v7;
  if ( v7 + a3 >= 8 )
  {
    *(_BYTE *)(*(_DWORD *)(a1 + 12) + 1) = v8 >> (8 - *(_DWORD *)(a1 + 4));
    if ( v9 >= 16 )
    {
      *(_BYTE *)(*(_DWORD *)(a1 + 12) + 2) = v8 >> (16 - *(_DWORD *)(a1 + 4));
      if ( v9 >= 24 )
      {
        *(_BYTE *)(*(_DWORD *)(a1 + 12) + 3) = v8 >> (24 - *(_DWORD *)(a1 + 4));
        if ( v9 >= 32 )
        {
          v10 = *(_DWORD *)(a1 + 4);
          if ( v10 )
            *(_BYTE *)(*(_DWORD *)(a1 + 12) + 4) = v8 >> (32 - v10);
          else
            *(_BYTE *)(*(_DWORD *)(a1 + 12) + 4) = 0;
        }
      }
    }
  }
  v11 = *(_DWORD *)(a1 + 12);
  result = v9 / 8;
  v13 = v9 / 8 + *(_DWORD *)a1;
  *(_DWORD *)(a1 + 4) = v9 & 7;
  *(_DWORD *)a1 = v13;
  *(_DWORD *)(a1 + 12) = v9 / 8 + v11;
  return result;
}

// ===== sub_4D4C20 @ 0x004D4C20..0x004D4CC7 =====
int __cdecl sub_4D4C20(_DWORD *a1, int a2)
{
  int v2; // esi
  int v3; // edi
  int v4; // edx
  unsigned __int8 *v6; // esi
  int v7; // eax

  v2 = a1[4];
  v3 = a1[1];
  v4 = v3 + a2;
  if ( *a1 + 4 >= v2 && v4 + 8 * *a1 > 8 * v2 )
    return -1;
  v6 = (unsigned __int8 *)a1[3];
  v7 = *v6 >> *((_BYTE *)a1 + 4);
  if ( v4 > 8 )
  {
    v7 |= v6[1] << (8 - v3);
    if ( v4 > 16 )
    {
      v7 |= v6[2] << (16 - v3);
      if ( v4 > 24 )
      {
        v7 |= v6[3] << (24 - v3);
        if ( v4 > 32 )
        {
          if ( v3 )
            v7 |= v6[4] << (32 - v3);
        }
      }
    }
  }
  return dword_503B74[a2] & v7;
}

// ===== sub_4D4CD0 @ 0x004D4CD0..0x004D4CFD =====
int __cdecl sub_4D4CD0(_DWORD *a1, int a2)
{
  int v2; // eax
  int result; // eax

  v2 = a1[1] + a2;
  a1[1] = v2 & 7;
  result = v2 / 8;
  a1[3] += result;
  *a1 += result;
  return result;
}

// ===== sub_4D4D00 @ 0x004D4D00..0x004D4DCD =====
int __cdecl sub_4D4D00(_DWORD *a1, int a2)
{
  int v2; // ebp
  int v3; // ecx
  int v4; // esi
  int v5; // edi
  unsigned __int8 *v6; // eax
  int v7; // edi
  int v8; // ecx
  int v10; // [esp+18h] [ebp+8h]

  v2 = a1[1];
  v3 = a1[4];
  v4 = v2 + a2;
  v10 = dword_503B74[a2];
  if ( *a1 + 4 < v3 || (v5 = -1, v4 + 8 * *a1 <= 8 * v3) )
  {
    v6 = (unsigned __int8 *)a1[3];
    v7 = *v6 >> *((_BYTE *)a1 + 4);
    if ( v4 > 8 )
    {
      v7 |= v6[1] << (8 - v2);
      if ( v4 > 16 )
      {
        v7 |= v6[2] << (16 - v2);
        if ( v4 > 24 )
        {
          v7 |= v6[3] << (24 - v2);
          if ( v4 > 32 )
          {
            if ( v2 )
              v7 |= v6[4] << (32 - v2);
          }
        }
      }
    }
    v5 = v10 & v7;
  }
  v8 = a1[3];
  a1[1] = v4 & 7;
  a1[3] = v4 / 8 + v8;
  *a1 += v4 / 8;
  return v5;
}

// ===== sub_4D4DD0 @ 0x004D4DD0..0x004D4DE8 =====
int __cdecl sub_4D4DD0(_DWORD *a1)
{
  return *a1 + (a1[1] + 7) / 8;
}

// ===== _frexp @ 0x004D4DF0..0x004D4EBE =====
double __cdecl frexp(double X, int *Y)
{
  __int16 v2; // cx
  double result; // st7
  int v4; // ebx
  int v5; // eax
  double v6; // st7
  __int16 v7; // cx
  double v8; // [esp+8h] [ebp-20h]
  double v9; // [esp+20h] [ebp-8h]
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  if ( !Y )
  {
    *_errno() = 22;
    _invalid_parameter_noinfo();
    return 0.0;
  }
  v4 = _ctrlfp(v2);
  if ( (HIWORD(X) & 0x7FF0) == 0x7FF0 )
  {
    *Y = -1;
    v5 = _sptype(SLODWORD(X), SHIDWORD(X));
    if ( v5 > 0 )
    {
      if ( v5 <= 2 )
      {
        v6 = dbl_4FC208;
LABEL_10:
        v8 = v6;
        result = X;
        _except1((int)&savedregs, 8u, 23, SLODWORD(X), SHIDWORD(X), v8, v4);
        return result;
      }
      if ( v5 == 3 )
        return _handle_qnan1(23, X, v4);
    }
    v6 = X + 1.0;
    goto LABEL_10;
  }
  v9 = _decomp(X, Y);
  _ctrlfp(v7);
  return v9;
}

// ===== _acos @ 0x004D4EC0..0x004D4EFF =====
double __cdecl acos(double X)
{
  int v1; // ecx
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return start_2(X);
  }
  _fload_withFB(v1, &X);
  return start_1(X);
}

// ===== __CIacos_default @ 0x004D4F3B..0x004D4F4F =====
void __usercall _CIacos_default(unsigned __int64 a1@<st0>)
{
  double v1; // [esp+0h] [ebp-Ch]

  _checkTOS_withFB(a1, HIDWORD(a1));
  start_1(v1);
}

// ===== start_1 @ 0x004D4F58..0x004D5006 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D4F58

// ===== __allshl @ 0x004D5010..0x004D502F =====
int __usercall _allshl@<eax>(__int64 a1@<edx:eax>, unsigned __int8 a2@<cl>)
{
  __int64 v2; // rax

  if ( a2 >= 0x40u )
  {
    LODWORD(v2) = 0;
  }
  else if ( a2 >= 0x20u )
  {
    LODWORD(v2) = 0;
  }
  else
  {
    return a1 << (a2 & 0x1F);
  }
  return v2;
}

// ===== _memchr @ 0x004D5060..0x004D510D =====
void *__cdecl memchr(const void *Buf, int Val, size_t MaxCount)
{
  void *result; // eax
  unsigned __int8 *v4; // edx
  int v5; // ebx
  unsigned __int8 v6; // cl
  bool v7; // cf
  char *v8; // eax
  unsigned __int8 v9; // cl
  int v10; // ecx
  unsigned int v11; // ecx
  unsigned int v12; // ecx

  result = (void *)MaxCount;
  if ( MaxCount )
  {
    v4 = (unsigned __int8 *)Buf;
    LOBYTE(v5) = Val;
    if ( ((unsigned __int8)Buf & 3) != 0 )
    {
      do
      {
        v6 = *v4++;
        if ( (unsigned __int8)Val == v6 )
          return v4 - 1;
        result = (char *)result - 1;
        if ( !result )
          return result;
      }
      while ( ((unsigned __int8)v4 & 3) != 0 );
    }
    v7 = (unsigned int)result < 4;
    v8 = (char *)result - 4;
    if ( !v7 )
    {
      v5 = 16843009 * (unsigned __int8)Val;
      do
      {
        v10 = v5 ^ *(_DWORD *)v4;
        v4 += 4;
        if ( (((v10 + 2130640639) ^ ~v10) & 0x81010100) != 0 )
        {
          v11 = *((_DWORD *)v4 - 1);
          LOBYTE(v11) = Val ^ v11;
          if ( !(_BYTE)v11 )
            return v4 - 4;
          BYTE1(v11) ^= Val;
          if ( !BYTE1(v11) )
            return v4 - 3;
          v12 = HIWORD(v11);
          if ( (unsigned __int8)Val == (unsigned __int8)v12 )
            return v4 - 2;
          if ( (unsigned __int8)Val == BYTE1(v12) )
            return v4 - 1;
        }
        v7 = (unsigned int)v8 < 4;
        v8 -= 4;
      }
      while ( !v7 );
    }
    result = v8 + 4;
    if ( result )
    {
      while ( 1 )
      {
        v9 = *v4++;
        if ( (unsigned __int8)v5 == v9 )
          break;
        result = (char *)result - 1;
        if ( !result )
          return result;
      }
      return v4 - 1;
    }
  }
  return result;
}

// ===== __CIacos_pentium4 @ 0x004D5110..0x004D5128 =====
void __usercall _CIacos_pentium4(double a1@<st0>)
{
  start_2(a1);
}

// ===== start_2 @ 0x004D512E..0x004D5664 =====
double __usercall start_2@<st0>(
        __m128i a1@<xmm0>,
        __m128d a2@<xmm1>,
        __m128d a3@<xmm2>,
        __m128i a4@<xmm3>,
        __m128i a5@<xmm4>,
        __m128d a6@<xmm7>,
        double a7)
{
  int v7; // edx
  __m128d inserted; // xmm5
  __m128d v9; // xmm0
  int v10; // edx
  __m128i v11; // xmm2
  double v12; // xmm3_8
  double v13; // xmm1_8
  double v14; // xmm7_8
  double v15; // xmm3_8
  __m128i v16; // xmm4
  double result; // st7
  unsigned int v18; // eax
  double v19; // xmm7_8
  char v20; // al
  double v21; // xmm1_8
  double v22; // xmm6_8
  __int64 v23; // xmm5_8
  double v24; // xmm0_8
  double v25; // xmm4_8
  unsigned int v26; // eax
  double v27; // xmm2_8
  int v28; // edx
  double v29; // xmm7_8
  double v30; // xmm6_8
  double v31; // xmm4_8
  __m128i v32; // xmm3
  double v33; // xmm0_8
  unsigned int v34; // eax
  __m128d v35; // xmm0
  __m128d v36; // xmm1
  __m128d v37; // xmm0
  __m128d v38; // xmm1
  __m128d v39; // xmm6
  __m128i v40; // xmm1
  unsigned int v41; // eax
  __m128d v42; // xmm7
  __m128d v43; // xmm1
  __m128d v44; // xmm5
  unsigned __int16 epi16; // ax
  __m128d v46; // xmm7
  __m128d v47; // xmm2
  __m128i v48; // xmm0
  __m128d v49; // xmm2
  __m128i v50; // xmm3
  double v51; // xmm4_8
  double v52; // xmm4_8
  __m128d v53; // xmm3
  __m128d v54; // xmm3
  __m128i v55; // xmm2
  int v56; // ecx
  __m128i v57; // xmm7
  double v58; // xmm0_8
  int v59; // edx
  __int64 v60; // xmm7_8
  double v61; // [esp+10h] [ebp-Ch] BYREF

  a3.m128d_f64[0] = NAN;
  a2.m128d_f64[0] = *(double *)a1.m128i_i64;
  v9 = (__m128d)_mm_srli_epi64(a1, 0x2Cu);
  v7 = _mm_cvtsi128_si32((__m128i)v9);
  a6.m128d_f64[0] = a2.m128d_f64[0];
  inserted = (__m128d)_mm_insert_epi16((__m128i)0LL, 0x2000u, 2);
  v9.m128d_f64[0] = a2.m128d_f64[0];
  if ( (v7 & 0x7FFFFu) - 260864 >= 0x3BB )
  {
    v18 = (v7 & 0x7FFFF) - 261819;
    if ( v18 >= 0x41 )
    {
      v34 = v18 + 15291;
      if ( v34 >= 0x3800 )
      {
        v41 = v34 - 15356;
        if ( v41 >= 4 )
        {
          if ( v41 + 261884 < 0x3FF00 )
          {
            v61 = 6.123233995736766e-17 + 1.570796326794897;
            return 6.123233995736766e-17 + 1.570796326794897;
          }
          else
          {
            v56 = _mm_cvtsi128_si32((__m128i)a6);
            v57 = _mm_srli_epi64((__m128i)a6, 0x20u);
            if ( v56 | (1072693248 - (_mm_cvtsi128_si32(v57) & 0x7FFFFFFF)) )
            {
              a3.m128d_f64[0] = a7;
              if ( (((__PAIR64__(
                        _mm_cvtsi128_si32(_mm_srli_epi64((__m128i)a3, 0x20u)) & 0x7FFFFFFF,
                        _mm_cvtsi128_si32((__m128i)a3))
                    - 0x7FF0000000000001LL) >> 32) & 0x80000000) == 0LL )
              {
                v58 = a2.m128d_f64[0] + 0.0;
                v59 = 1008;
              }
              else
              {
                v58 = 0.0 * *(double *)_mm_insert_epi16((__m128i)0LL, 0x7FF0u, 3).m128i_i64;
                v59 = 58;
              }
              v61 = v58;
              __libm_error_support(&a7, &a7, &v61, v59);
              return v61;
            }
            else
            {
              COERCE_DOUBLE(v60 = _mm_shuffle_epi32(
                                    _mm_cvtsi32_si128(-((unsigned int)_mm_extract_epi16(v57, 1) >> 15)),
                                    0).m128i_i64[0]);
              return COERCE_DOUBLE(COERCE_UNSIGNED_INT64(3.141592653589793) & v60)
                   + COERCE_DOUBLE(COERCE_UNSIGNED_INT64(1.224646799147353e-16) & v60);
            }
          }
        }
        else
        {
          *(double *)a5.m128i_i64 = 0.5 - fabs(a2.m128d_f64[0]) * 0.5;
          v42 = (__m128d)_mm_shuffle_epi32(a5, 68);
          *(double *)a5.m128i_i64 = sqrt(*(double *)a5.m128i_i64);
          v43 = _mm_mul_pd((__m128d)xmmword_4E4070, v42);
          v44 = (__m128d)_mm_shuffle_epi32((__m128i)v42, 68);
          epi16 = _mm_extract_epi16((__m128i)v9, 3);
          v46 = _mm_mul_pd(v42, v42);
          v47 = _mm_add_pd(_mm_add_pd((__m128d)xmmword_4E4080, v43), _mm_mul_pd((__m128d)xmmword_4E4090, v46));
          v47.m128d_f64[0] = v47.m128d_f64[0] * (v46.m128d_f64[0] * v44.m128d_f64[0]);
          v48 = (__m128i)_mm_and_pd(
                           (__m128d)_mm_shuffle_epi32((__m128i)_mm_cmplt_sd(v9, (__m128d)0LL), 68),
                           (__m128d)xmmword_4E4000);
          v49 = _mm_mul_pd(v47, v44);
          *(_QWORD *)&v43.m128d_f64[0] = COERCE_UNSIGNED_INT64(NAN) & a5.m128i_i64[0];
          v50 = _mm_shuffle_epi32(a5, 68);
          v51 = *(double *)a5.m128i_i64 - COERCE_DOUBLE(COERCE_UNSIGNED_INT64(NAN) & a5.m128i_i64[0]);
          *(double *)v50.m128i_i64 = *(double *)v50.m128i_i64 + *(double *)v50.m128i_i64 - v51;
          v52 = v51 * *(double *)v50.m128i_i64;
          v53 = (__m128d)_mm_shuffle_epi32(v50, 238);
          v44.m128d_f64[0] = (v44.m128d_f64[0] - v43.m128d_f64[0] * v43.m128d_f64[0] - v52) / v53.m128d_f64[0];
          v54 = _mm_add_pd(v53, v53);
          v55 = (__m128i)_mm_mul_pd(v49, v54);
          *(_QWORD *)&result = COERCE_UNSIGNED_INT64(
                                 *(double *)_mm_shuffle_epi32(v48, 238).m128i_i64
                               + *(double *)v55.m128i_i64
                               + *(double *)v48.m128i_i64
                               + *(double *)_mm_shuffle_epi32(v55, 238).m128i_i64
                               + v44.m128d_f64[0]
                               + v54.m128d_f64[0]) ^ _mm_insert_epi16((__m128i)0LL, epi16 & 0x8000, 3).m128i_u64[0];
        }
      }
      else
      {
        v35 = _mm_unpacklo_pd(v9, v9);
        v36 = _mm_unpacklo_pd(a2, v35);
        v37 = _mm_mul_pd(v35, v35);
        v38 = _mm_mul_pd(v36, v37);
        v38.m128d_f64[0] = v38.m128d_f64[0] * v38.m128d_f64[0] * v38.m128d_f64[0];
        v39 = _mm_add_pd(
                _mm_add_pd(_mm_mul_pd((__m128d)xmmword_4E4070, v37), (__m128d)xmmword_4E4080),
                _mm_mul_pd((__m128d)xmmword_4E4090, _mm_mul_pd(v37, v37)));
        v37.m128d_f64[0] = *(double *)_mm_shuffle_epi32((__m128i)xmmword_4E3FF0, 238).m128i_i64;
        v40 = (__m128i)_mm_mul_pd(v38, v39);
        return v37.m128d_f64[0]
             - a6.m128d_f64[0]
             + 6.123233995736766e-17
             - *(double *)v40.m128i_i64
             - *(double *)_mm_shuffle_epi32(v40, 238).m128i_i64
             - (a6.m128d_f64[0]
              - (v37.m128d_f64[0]
               - (v37.m128d_f64[0]
                - a6.m128d_f64[0])));
      }
    }
    else
    {
      *(_QWORD *)&v19 = *(_QWORD *)&a2.m128d_f64[0] >> 38 << 38;
      v20 = _mm_movemask_epi8((__m128i)v9);
      v21 = a2.m128d_f64[0] - v19;
      v22 = v19;
      v23 = *(_QWORD *)&inserted.m128d_f64[0] | ~COERCE__INT64(NAN) & *(_QWORD *)&v9.m128d_f64[0];
      v24 = (v9.m128d_f64[0] + v19) * v21;
      v25 = 1.0 - v19 * v19;
      *(double *)a4.m128i_i64 = sqrt(v25 - v24);
      v26 = -((unsigned __int8)(v20 & 0x80) >> 7);
      *(_QWORD *)&v27 = COERCE_UNSIGNED_INT64(NAN) & a4.m128i_i64[0] | v23;
      v28 = _mm_extract_epi16(_mm_slli_epi64(a4, 2u), 3) - 65216;
      v29 = *(double *)a4.m128i_i64 * *(double *)&qword_4E37F0[v28];
      v30 = v22 * v27 - v29 + v21 * v27;
      v31 = v25 - v27 * v27 - v24;
      v32 = (__m128i)_mm_add_pd(
                       _mm_and_pd((__m128d)_mm_shuffle_epi32(_mm_cvtsi32_si128(v26), 0), (__m128d)xmmword_4E4000),
                       (__m128d)xmmword_4E28F0[v28]);
      v33 = (-0.04464285714285714 * (v30 * v30) + -0.075) * (v30 * (v30 * v30) * (v30 * v30))
          + -0.1666666666666667 * (v30 * (v30 * v30))
          + *(double *)v32.m128i_i64;
      v32.m128i_i64[0] = _mm_shuffle_epi32(v32, 238).m128i_i64[0];
      *(_QWORD *)&result = COERCE_UNSIGNED_INT64(
                             v33
                           + v31 / (v29 + v29 + v30)
                           + *(double *)v32.m128i_i64
                           - (v31 / (v29 + v29 + v30)
                            + *(double *)v32.m128i_i64)
                           + v31 / (v29 + v29 + v30)
                           + *(double *)v32.m128i_i64) ^ _mm_insert_epi16(
                                                           (__m128i)0LL,
                                                           (unsigned __int16)v26 & 0x8000,
                                                           3).m128i_u64[0];
    }
  }
  else
  {
    v10 = (v7 & 0xFFFC) - 64256;
    v11 = (__m128i)_mm_or_pd(_mm_and_pd(a3, a6), inserted);
    v12 = sqrt(1.0 - a2.m128d_f64[0] * a2.m128d_f64[0]) * *(double *)v11.m128i_i64;
    v13 = a2.m128d_f64[0] * *(double *)((char *)qword_4E37F0 + 2 * v10);
    v14 = (a6.m128d_f64[0] + *(double *)v11.m128i_i64) * (v9.m128d_f64[0] - *(double *)v11.m128i_i64) / (v13 + v12);
    v15 = v13 - v12;
    v16 = (__m128i)_mm_sub_pd(
                     _mm_xor_pd(
                       *(__m128d *)((char *)xmmword_4E28F0 + 4 * v10),
                       (__m128d)_mm_shuffle_epi32(_mm_slli_epi64(_mm_srli_epi64(v11, 0x3Fu), 0x3Fu), 68)),
                     (__m128d)xmmword_4E3FF0);
    return (-0.04464285714285714 * (v15 * v15) + -0.075) * (v15 * (v15 * v15) * (v15 * v15))
         + -0.1666666666666667 * (v15 * (v15 * v15))
         - *(double *)v16.m128i_i64
         - v14
         - *(double *)_mm_shuffle_epi32(v16, 238).m128i_i64;
  }
  return result;
}

// ===== _cos @ 0x004D5670..0x004D56AF =====
double __cdecl cos(double X)
{
  int v1; // ecx
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return start_8(X);
  }
  _fload_withFB(v1, &X);
  return start_3(X);
}

// ===== __CIcos @ 0x004D56B0..0x004D56FF =====
double __usercall _CIcos@<st0>(double x@<st0>)
{
  int v1; // eax
  bool v2; // zf
  int v3; // [esp+0h] [ebp-Ch]
  char v4; // [esp+4h] [ebp-8h]

  if ( !dword_567C18 )
    goto __CIcos_default;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v4 & 0x7F) == 127;
  if ( v2 )
  {
    _CIcos_pentium4();
  }
  else
  {
__CIcos_default:
    _checkTOS_withFB(SLODWORD(x), HIDWORD(*(unsigned __int64 *)&x));
    start_3(v3);
  }
  return x;
}

// ===== start_3 @ 0x004D5708..0x004D5799 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D5708

// ===== _sin @ 0x004D57A0..0x004D57DF =====
double __cdecl sin(double X)
{
  int v1; // ecx
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return start_9(X);
  }
  _fload_withFB(v1, &X);
  return start_4(X);
}

// ===== __CIsin @ 0x004D57E0..0x004D582F =====
double __usercall _CIsin@<st0>(double x@<st0>)
{
  int v1; // eax
  bool v2; // zf
  int v3; // [esp+0h] [ebp-Ch]
  char v4; // [esp+4h] [ebp-8h]

  if ( !dword_567C18 )
    goto __CIsin_default;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v4 & 0x7F) == 127;
  if ( v2 )
  {
    _CIsin_pentium4();
  }
  else
  {
__CIsin_default:
    _checkTOS_withFB(SLODWORD(x), HIDWORD(*(unsigned __int64 *)&x));
    start_4(v3);
  }
  return x;
}

// ===== start_4 @ 0x004D5838..0x004D58C9 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D5838

// ===== sub_4D58D0 @ 0x004D58D0..0x004D58DA =====
int __cdecl sub_4D58D0(double a1, double a2)
{
  return _ctrandisp2(a1, a2);
}

// ===== sub_4D58DA @ 0x004D58DA..0x004D58E4 =====
int __thiscall sub_4D58DA(void *this)
{
  return _cintrindisp2(this, &unk_4EDB20);
}

// ===== __CIsqrt @ 0x004D58F0..0x004D5904 =====
double __usercall _CIsqrt@<st0>(unsigned __int64 x@<st0>)
{
  int v2; // [esp+0h] [ebp-Ch]
  int v3; // [esp+4h] [ebp-8h]

  _checkTOS_withFB(x, HIDWORD(x));
  return start_5(v2, v3);
}

// ===== _sqrt @ 0x004D5904..0x004D590D =====
double __cdecl sqrt(double X)
{
  int v1; // ecx

  _fload_withFB(v1, &X);
  return start_5(LODWORD(X), HIDWORD(X));
}

// ===== start_5 @ 0x004D590D..0x004D59AA =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D590D

// ===== _atan @ 0x004D59B0..0x004D59EF =====
double __cdecl atan(double X)
{
  int v1; // ecx
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return start_10(X);
  }
  _fload_withFB(v1, &X);
  return start_6(X);
}

// ===== __CIatan @ 0x004D59F0..0x004D5A3F =====
double __usercall _CIatan@<st0>(double x@<st0>)
{
  int v1; // eax
  bool v2; // zf
  int v3; // [esp+0h] [ebp-Ch]
  char v4; // [esp+4h] [ebp-8h]

  if ( !dword_567C18 )
    goto __CIatan_default;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v4 & 0x7F) == 127;
  if ( v2 )
  {
    _CIatan_pentium4();
  }
  else
  {
__CIatan_default:
    _checkTOS_withFB(SLODWORD(x), HIDWORD(*(unsigned __int64 *)&x));
    start_6(v3);
  }
  return x;
}

// ===== start_6 @ 0x004D5A48..0x004D5ACA =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D5A48

// ===== _log @ 0x004D5AD0..0x004D5B0F =====
double __cdecl log(double X)
{
  int v1; // ecx
  int v2; // eax
  bool v3; // zf
  char v5; // [esp+0h] [ebp-8h]

  if ( dword_567C18 )
  {
    v2 = _mm_getcsr() & 0x7F80;
    v3 = v2 == 8064;
    if ( v2 == 8064 )
      v3 = (v5 & 0x7F) == 127;
    if ( v3 )
      return start_11(X);
  }
  _fload_withFB(v1, &X);
  return start_7(X);
}

// ===== __CIlog @ 0x004D5B10..0x004D5B5F =====
double __usercall _CIlog@<st0>(double x@<st0>)
{
  int v1; // eax
  bool v2; // zf
  int v3; // [esp+0h] [ebp-Ch]
  char v4; // [esp+4h] [ebp-8h]
  int v5; // [esp+4h] [ebp-8h]

  if ( !dword_567C18 )
    goto __CIlog_default;
  v1 = _mm_getcsr() & 0x7F80;
  v2 = v1 == 8064;
  if ( v1 == 8064 )
    v2 = (v4 & 0x7F) == 127;
  if ( v2 )
  {
    _CIlog_pentium4();
  }
  else
  {
__CIlog_default:
    _checkTOS_withFB(SLODWORD(x), HIDWORD(*(unsigned __int64 *)&x));
    start_7(v3, v5);
  }
  return x;
}

// ===== start_7 @ 0x004D5B68..0x004D5C20 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x004D5B68

// ===== __allrem @ 0x004D5C20..0x004D5CD2 =====
unsigned __int64 __stdcall _allrem(unsigned __int64 a1, __int64 a2)
{
  int v2; // edi
  int v3; // eax
  unsigned __int64 v4; // rtt
  unsigned __int64 result; // rax
  unsigned __int64 v6; // rcx
  unsigned __int64 v7; // rax
  unsigned int v8; // eax
  int v9; // ecx
  bool v10; // cf
  unsigned __int64 v11; // rax

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
    v3 = -HIDWORD(a2) - ((_DWORD)a2 != 0);
    HIDWORD(a2) = v3;
    LODWORD(a2) = -(int)a2;
  }
  if ( !v3 )
  {
    LODWORD(v4) = a1;
    HIDWORD(v4) = HIDWORD(a1) % (unsigned int)a2;
    result = v4 % (unsigned int)a2;
    if ( v2 - 1 < 0 )
      return result;
    return -(__int64)result;
  }
  v6 = __PAIR64__(v3, a2);
  v7 = a1;
  do
  {
    v6 >>= 1;
    v7 >>= 1;
  }
  while ( HIDWORD(v6) );
  v8 = v7 / (unsigned int)v6;
  v9 = HIDWORD(a2) * v8;
  v11 = (unsigned int)a2 * (unsigned __int64)v8;
  v10 = __CFADD__(v9, HIDWORD(v11));
  HIDWORD(v11) += v9;
  if ( v10 || v11 > a1 )
    v11 -= a2;
  result = v11 - a1;
  if ( v2 - 1 < 0 )
    return -(__int64)result;
  return result;
}

// ===== __CIcos_pentium4 @ 0x004D5CE0..0x004D5CF8 =====
void __usercall _CIcos_pentium4(double a1@<st0>)
{
  start_8(a1);
}

// ===== start_8 @ 0x004D5CFE..0x004D5E89 =====
double __usercall start_8@<st0>(int a1@<ecx>, __m128d a2@<xmm0>, __m128d a3@<xmm1>, int a4)
{
  __int16 v4; // ax
  double v5; // xmm3_8
  __m128d v6; // xmm1
  double v7; // xmm4_8
  double *v8; // eax
  __m128d v9; // xmm2
  __m128d v10; // xmm0
  double v11; // xmm3_8
  double v12; // xmm4_8
  __m128d v13; // xmm5
  __m128d v14; // xmm0
  double v15; // xmm7_8
  __m128d v16; // xmm5
  __m128d v17; // xmm0
  double v18; // xmm3_8
  __m128d v19; // xmm2
  double v20; // xmm3_8
  double v21; // xmm7_8
  __m128d v22; // xmm6
  __m128d v23; // xmm2
  __m128d v24; // xmm5
  double v25; // xmm3_8
  __m128d v26; // xmm6
  double result; // st7
  unsigned int epi16; // eax

  v4 = (_mm_extract_epi16((__m128i)a2, 3) & 0x7FFF) - 12336;
  if ( (unsigned __int16)v4 > 0x10C5u )
  {
    if ( v4 > 4293 )
    {
      result = _fload_withFB(a1, &a4);
      start_3(a4);
    }
    else
    {
      epi16 = _mm_extract_epi16((__m128i)a2, 3);
      LOWORD(epi16) = epi16 & 0x7FFF;
      return 1.0 - *(double *)_mm_insert_epi16((__m128i)a2, epi16, 3).m128i_i64;
    }
  }
  else
  {
    a3.m128d_f64[0] = 10.1859163578813 * a2.m128d_f64[0] + 6.755399441055744e15 - 6.755399441055744e15;
    v5 = 0.09817477042088285 * a3.m128d_f64[0];
    v6 = _mm_unpacklo_pd(a3, a3);
    v7 = a2.m128d_f64[0];
    v8 = (double *)((char *)&unk_4EC9D0 + 32
                                        * (((unsigned __int8)(int)(10.1859163578813 * a2.m128d_f64[0]) + 16) & 0x3F));
    v9 = _mm_mul_pd((__m128d)xmmword_4ED220, v6);
    a2.m128d_f64[0] = a2.m128d_f64[0] - v5;
    v10 = _mm_unpacklo_pd(a2, a2);
    v11 = v7 - v5;
    v12 = v11 - v9.m128d_f64[0];
    v13 = _mm_mul_pd((__m128d)xmmword_4ED200, v10);
    v14 = _mm_sub_pd(v10, v9);
    v15 = v8[1] * (v11 - v9.m128d_f64[0]);
    v16 = _mm_mul_pd(v13, v14);
    v17 = _mm_mul_pd(v14, v14);
    v18 = v11 - (v11 - v9.m128d_f64[0]) - v9.m128d_f64[0];
    v19 = *(__m128d *)v8;
    v6.m128d_f64[0] = v6.m128d_f64[0] * 1.263916405497469e-22 - v18;
    v20 = v8[3];
    v19.m128d_f64[0] = *v8 + v20;
    v21 = v15 - v19.m128d_f64[0];
    v19.m128d_f64[0] = v19.m128d_f64[0] * v12;
    v22 = _mm_mul_pd((__m128d)xmmword_4ED1E0, v17);
    v23 = _mm_mul_pd(v19, v17);
    v24 = _mm_mul_pd(_mm_add_pd(v16, (__m128d)xmmword_4ED1F0), _mm_mul_pd(v17, v17));
    v17.m128d_f64[0] = v20 * v12;
    v25 = v17.m128d_f64[0] + v8[1];
    v26 = _mm_mul_pd(_mm_add_pd(_mm_add_pd(v22, (__m128d)xmmword_4ED1D0), v24), v23);
    return v12 * *v8
         + v25
         + v6.m128d_f64[0] * v21
         + v8[2]
         + v8[1]
         - v25
         + v17.m128d_f64[0]
         + v25
         - (v12 * *v8
          + v25)
         + v12 * *v8
         + v26.m128d_f64[0]
         + _mm_unpackhi_pd(v26, v26).m128d_f64[0];
  }
  return result;
}

// ===== __CIsin_pentium4 @ 0x004D5E90..0x004D5EA8 =====
void __usercall _CIsin_pentium4(double a1@<st0>)
{
  start_9(a1);
}

// ===== start_9 @ 0x004D5EAE..0x004D6057 =====
double __usercall start_9@<st0>(int a1@<ecx>, __m128d a2@<xmm0>, __m128d a3@<xmm1>, int a4)
{
  __int16 v4; // ax
  double v5; // xmm3_8
  __m128d v6; // xmm1
  double v7; // xmm4_8
  double *v8; // eax
  __m128d v9; // xmm2
  __m128d v10; // xmm0
  double v11; // xmm3_8
  double v12; // xmm4_8
  __m128d v13; // xmm5
  __m128d v14; // xmm0
  double v15; // xmm7_8
  __m128d v16; // xmm5
  __m128d v17; // xmm0
  double v18; // xmm3_8
  __m128d v19; // xmm2
  double v20; // xmm3_8
  double v21; // xmm7_8
  __m128d v22; // xmm6
  __m128d v23; // xmm2
  __m128d v24; // xmm5
  double v25; // xmm3_8
  __m128d v26; // xmm6
  double result; // st7

  v4 = (_mm_extract_epi16((__m128i)a2, 3) & 0x7FFF) - 12336;
  if ( (unsigned __int16)v4 > 0x10C5u )
  {
    if ( v4 > 4293 )
    {
      result = _fload_withFB(a1, &a4);
      start_4(a4);
    }
    else
    {
      if ( (unsigned __int16)v4 >> 4 == 3325 )
        a2.m128d_f64[0] = a2.m128d_f64[0] * 0.9999999999999999;
      return a2.m128d_f64[0];
    }
  }
  else
  {
    a3.m128d_f64[0] = 10.1859163578813 * a2.m128d_f64[0] + 6.755399441055744e15 - 6.755399441055744e15;
    v5 = 0.09817477042088285 * a3.m128d_f64[0];
    v6 = _mm_unpacklo_pd(a3, a3);
    v7 = a2.m128d_f64[0];
    v8 = (double *)((char *)&unk_4ED280 + 32 * ((int)(10.1859163578813 * a2.m128d_f64[0]) & 0x3F));
    v9 = _mm_mul_pd((__m128d)xmmword_4EDAD0, v6);
    a2.m128d_f64[0] = a2.m128d_f64[0] - v5;
    v10 = _mm_unpacklo_pd(a2, a2);
    v11 = v7 - v5;
    v12 = v11 - v9.m128d_f64[0];
    v13 = _mm_mul_pd((__m128d)xmmword_4EDAB0, v10);
    v14 = _mm_sub_pd(v10, v9);
    v15 = v8[1] * (v11 - v9.m128d_f64[0]);
    v16 = _mm_mul_pd(v13, v14);
    v17 = _mm_mul_pd(v14, v14);
    v18 = v11 - (v11 - v9.m128d_f64[0]) - v9.m128d_f64[0];
    v19 = *(__m128d *)v8;
    v6.m128d_f64[0] = v6.m128d_f64[0] * 1.263916405497469e-22 - v18;
    v20 = v8[3];
    v19.m128d_f64[0] = *v8 + v20;
    v21 = v15 - v19.m128d_f64[0];
    v19.m128d_f64[0] = v19.m128d_f64[0] * v12;
    v22 = _mm_mul_pd((__m128d)xmmword_4EDA90, v17);
    v23 = _mm_mul_pd(v19, v17);
    v24 = _mm_mul_pd(_mm_add_pd(v16, (__m128d)xmmword_4EDAA0), _mm_mul_pd(v17, v17));
    v17.m128d_f64[0] = v20 * v12;
    v25 = v17.m128d_f64[0] + v8[1];
    v26 = _mm_mul_pd(_mm_add_pd(_mm_add_pd(v22, (__m128d)xmmword_4EDA80), v24), v23);
    return v12 * *v8
         + v25
         + v6.m128d_f64[0] * v21
         + v8[2]
         + v8[1]
         - v25
         + v17.m128d_f64[0]
         + v25
         - (v12 * *v8
          + v25)
         + v12 * *v8
         + v26.m128d_f64[0]
         + _mm_unpackhi_pd(v26, v26).m128d_f64[0];
  }
  return result;
}

// ===== fFASN @ 0x004D6060..0x004D609C =====
double __usercall fFASN@<st0>(long double a1@<st1>, long double a2@<st0>)
{
  char v2; // cl
  double result; // st7
  char v4; // ch

  AugmentSinCos();
  result = atan2(a2, a1);
  if ( v4 )
    result = 3.141592653589793238 - result;
  if ( v2 )
    return -result;
  return result;
}

// ===== AugmentSinCos @ 0x004D609C..0x004D60CC =====
// positive sp value has been detected, the output may be wrong!
int __usercall AugmentSinCos@<eax>(int a1@<ebp>, long double a2@<st0>)
{
  long double v2; // st7
  __int16 v3; // fps
  long double v4; // st6
  bool v5; // c0
  char v6; // c2
  bool v7; // c3
  int result; // eax

  v2 = fabs(a2);
  v4 = (1.0 - v2) * (v2 + 1.0);
  v5 = v4 < 0.0;
  v6 = 0;
  v7 = v4 == 0.0;
  *(_WORD *)(a1 - 160) = v3;
  if ( (*(_BYTE *)(a1 - 159) & 1) != 0 )
    return _rtindfpop(a1);
  return result;
}

// ===== __rtpiby2 @ 0x004D60CC..0x004D60D5 =====
double _rtpiby2()
{
  return -8.87960937049345e43;
}

// ===== _rtforatn20 @ 0x004D60D5..0x004D60E5 =====
double __fastcall rtforatn20(__int16 a1)
{
  double result; // st7

  if ( (_BYTE)a1 )
  {
    result = 3.141592653589793238;
    if ( HIBYTE(a1) )
      result = -3.141592653589793238;
  }
  else
  {
    result = 0.0;
    if ( HIBYTE(a1) )
      return -0.0;
  }
  postv();
  return result;
}

// ===== postv @ 0x004D60E5..0x004D60E6 =====
void postv()
{
  ;
}

// ===== _rtforatnby0 @ 0x004D60F8..0x004D6108 =====
void rtforatnby0()
{
  _rtpiby2();
  chsifnegret();
}

// ===== __cintrindisp2 @ 0x004D6110..0x004D614E =====
int __usercall _cintrindisp2@<eax>(int a1@<edx>, __int16 a2@<fpstat>, double a3@<st1>, double a4@<st0>)
{
  int savedregs; // [esp+2D4h] [ebp+0h] BYREF

  _trandisp2(a1, (int)&savedregs, a2, a3, a4);
  return cintrinexit();
}

// ===== __cintrindisp1 @ 0x004D614E..0x004D618B =====
int __usercall _cintrindisp1@<eax>(int a1@<edx>, __int16 a2@<cx>, __int16 a3@<fpstat>, double a4@<st0>)
{
  int savedregs; // [esp+2D4h] [ebp+0h] BYREF

  _trandisp1(a1, a2, (int)&savedregs, a3, a4);
  return cintrinexit();
}

// ===== __ctrandisp2 @ 0x004D618B..0x004D61CC =====
int __usercall _ctrandisp2@<eax>(double a1@<st1>, double a2@<st0>, double a3, double a4)
{
  int v4; // edx
  __int16 v5; // fps
  int savedregs; // [esp+2D4h] [ebp+0h] BYREF

  _fload(a3);
  _fload(a4);
  _trandisp2(v4, (int)&savedregs, v5, a1, a2);
  return ctranexit();
}

// ===== ctranexit @ 0x004D61CC..0x004D61D3 =====
int __usercall ctranexit@<eax>(int a1@<ebp>)
{
  *(_BYTE *)(a1 - 712) &= ~1u;
  return cintrinexit();
}

// ===== cintrinexit @ 0x004D61D3..0x004D631A =====
double __usercall cintrinexit@<st0>(double *a1@<ebp>, double result@<st0>)
{
  char v2; // fps
  char v6; // al
  __int16 v7; // ax
  int v8; // ebx

  if ( dword_509B6C )
    return result;
  *(a1 - 90) = result;
  v6 = *((_BYTE *)a1 - 144);
  switch ( v6 )
  {
    case 0:
      goto checkinexact;
    case -1:
      if ( (*((_WORD *)a1 - 357) & 0x7FF0) != 0x7FF0 )
        goto checkinexact;
      goto haveoverflow_0;
    case -2:
      v7 = *((_WORD *)a1 - 357) & 0x7FF0;
      if ( !v7 )
      {
        *(_DWORD *)((char *)a1 - 142) = 4;
        result = __FSCALE__(result, 1536.0);
        if ( fabs(result) < 2.225073858507201e-308 )
          result = result * 0.0;
        goto haveerror;
      }
      if ( v7 != 32752 )
      {
checkinexact:
        if ( (*((_WORD *)a1 - 82) & 0x20) != 0 || (v2 & 0x20) == 0 )
          return result;
        *(_DWORD *)((char *)a1 - 142) = 8;
        goto haveerror;
      }
haveoverflow_0:
      *(_DWORD *)((char *)a1 - 142) = 3;
      result = __FSCALE__(result, -1536.0);
      if ( fabs(result) > 1.797693134862316e308 )
        result = result * INFINITY;
      goto haveerror;
  }
  *(_DWORD *)((char *)a1 - 142) = v6;
haveerror:
  v8 = *((_DWORD *)a1 - 37) + 1;
  *(_DWORD *)((char *)a1 - 138) = v8;
  if ( (*(_BYTE *)(a1 - 89) & 1) == 0 )
  {
    *(_DWORD *)((char *)a1 - 134) = *((_DWORD *)a1 + 2);
    *(_DWORD *)((char *)a1 - 130) = *((_DWORD *)a1 + 3);
    if ( *(_BYTE *)(v8 + 12) != 1 )
    {
      *(_DWORD *)((char *)a1 - 126) = *((_DWORD *)a1 + 4);
      *(_DWORD *)((char *)a1 - 122) = *((_DWORD *)a1 + 5);
    }
  }
  *(double *)((char *)a1 - 118) = result;
  _87except((int)a1, *(char *)(*((_DWORD *)a1 - 37) + 14), (int)a1 - 142, (__int16 *)a1 - 82);
  return *(double *)((char *)a1 - 118);
}

// ===== __ctrandisp1 @ 0x004D631A..0x004D634D =====
int __usercall _ctrandisp1@<eax>(double a1@<st0>, double a2)
{
  int v2; // edx
  __int16 v3; // cx
  __int16 v4; // fps
  int savedregs; // [esp+2D4h] [ebp+0h] BYREF

  _fload(a2);
  _trandisp1(v2, v3, (int)&savedregs, v4, a1);
  return ctranexit((int)&savedregs);
}

// ===== __fload @ 0x004D634D..0x004D6389 =====
double __cdecl _fload(double a1)
{
  double v2; // [esp+6h] [ebp-Ah]

  if ( (HIWORD(a1) & 0x7FF0) != 0x7FF0 )
    return a1;
  HIDWORD(v2) = *(_QWORD *)&a1 >> 21;
  LODWORD(v2) = LODWORD(a1);
  return v2;
}

// ===== __CIatan_pentium4 @ 0x004D6390..0x004D63A8 =====
void __usercall _CIatan_pentium4(double a1@<st0>)
{
  start_10(a1);
}

// ===== start_10 @ 0x004D63AE..0x004D667B =====
double __usercall start_10@<st0>(__m128d a1@<xmm7>, double a2)
{
  __m128d v2; // xmm7
  __m128d v3; // xmm2
  __m128d v4; // xmm1
  __m128d v5; // xmm3
  __m128d v6; // xmm5
  double result; // st7
  __m128d v8; // xmm1
  __m128d v9; // xmm3
  __m128d v10; // xmm5
  unsigned __int64 v11; // xmm6_8
  __m128i v12; // xmm3
  int v13; // eax
  __m128d v14; // xmm2
  double v15; // xmm0_8
  __m128d v16; // xmm1
  __m128d v17; // xmm3
  __m128d v18; // xmm5

  v2 = _mm_unpacklo_pd(a1, a1);
  v3 = _mm_and_pd(v2, (__m128d)xmmword_4EDBB0);
  if ( v3.m128d_f64[0] >= 1.633123935319537e16 )
    return dbl_4EDBC0[HIDWORD(a2) >> 31] + 4.778309726736481e-299;
  if ( v3.m128d_f64[0] >= 0.03125 )
  {
    if ( v3.m128d_f64[0] >= 0.375 )
    {
      v11 = _mm_move_epi64((__m128i)v2).m128i_u64[0] ^ *(_QWORD *)&v3.m128d_f64[0];
      if ( v3.m128d_f64[0] >= 8.0 )
      {
        v13 = 768;
        v15 = *(double *)_mm_move_epi64((__m128i)v3).m128i_i64;
        v3 = (__m128d)_mm_loadl_epi64((const __m128i *)&qword_4EDCA0);
        v3.m128d_f64[0] = v3.m128d_f64[0] / v15;
      }
      else
      {
        v12 = _mm_move_epi64((__m128i)v3);
        *(double *)v12.m128i_i64 = *(double *)v12.m128i_i64 + 8.0;
        v13 = 3
            * _mm_cvtsi128_si32(_mm_sub_epi32(_mm_srli_epi64(v12, 0x2Cu), _mm_loadl_epi64((const __m128i *)&qword_4EDC90)));
        v3.m128d_f64[0] = (v3.m128d_f64[0] - qword_4EE600[v13])
                        / (*(double *)_mm_move_epi64((__m128i)v3).m128i_i64 * qword_4EE600[v13] + 1.0);
      }
      v14 = _mm_unpacklo_pd(v3, v3);
      v16 = _mm_mul_pd(v14, v14);
      v17 = _mm_mul_pd(v16, v16);
      v18 = _mm_add_pd(
              _mm_mul_pd(
                _mm_add_pd(
                  _mm_mul_pd(_mm_add_pd(_mm_mul_pd((__m128d)xmmword_4EDC80, v17), (__m128d)xmmword_4EDC70), v17),
                  (__m128d)xmmword_4EDC60),
                v17),
              (__m128d)xmmword_4EDC50);
      v18.m128d_f64[0] = v18.m128d_f64[0] * v16.m128d_f64[0];
      *(_QWORD *)&result = COERCE_UNSIGNED_INT64(
                             *(double *)&qword_4EE5F0[v13]
                           - ((v18.m128d_f64[0] + _mm_shuffle_pd(v18, v18, 1).m128d_f64[0]) * v14.m128d_f64[0]
                            - *(double *)&qword_4EE5F8[v13]
                            - v14.m128d_f64[0])) | v11;
    }
    else
    {
      v8 = _mm_mul_pd(v3, v3);
      v9 = _mm_mul_pd(v8, v8);
      v10 = _mm_add_pd(
              _mm_mul_pd(
                _mm_add_pd(
                  _mm_mul_pd(
                    _mm_add_pd(
                      _mm_mul_pd(
                        _mm_add_pd(
                          _mm_mul_pd(
                            _mm_add_pd(
                              _mm_mul_pd(
                                _mm_add_pd(
                                  _mm_mul_pd(
                                    _mm_add_pd(_mm_mul_pd((__m128d)xmmword_4EDC40, v9), (__m128d)xmmword_4EDC30),
                                    v9),
                                  (__m128d)xmmword_4EDC20),
                                v9),
                              (__m128d)xmmword_4EDC10),
                            v9),
                          (__m128d)xmmword_4EDC00),
                        v9),
                      (__m128d)xmmword_4EDBF0),
                    v9),
                  (__m128d)xmmword_4EDBE0),
                v9),
              (__m128d)xmmword_4EDBD0);
      v10.m128d_f64[0] = v10.m128d_f64[0] * v8.m128d_f64[0];
      return v2.m128d_f64[0] - (v10.m128d_f64[0] + _mm_shuffle_pd(v10, v10, 1).m128d_f64[0]) * v2.m128d_f64[0];
    }
  }
  else if ( v3.m128d_f64[0] < 0.000000007450580596923828 )
  {
    if ( v3.m128d_f64[0] == 0.0 )
      return a2;
    else
      return 4.778309726736481e-299 * 4.778309726736481e-299 + a2;
  }
  else
  {
    v4 = _mm_mul_pd(v3, v3);
    v5 = _mm_mul_pd(v4, v4);
    v6 = _mm_add_pd(
           _mm_mul_pd(
             _mm_add_pd(
               _mm_mul_pd(_mm_add_pd(_mm_mul_pd((__m128d)xmmword_4EDC80, v5), (__m128d)xmmword_4EDC70), v5),
               (__m128d)xmmword_4EDC60),
             v5),
           (__m128d)xmmword_4EDC50);
    v6.m128d_f64[0] = v6.m128d_f64[0] * v4.m128d_f64[0];
    return v2.m128d_f64[0] - (v6.m128d_f64[0] + _mm_shuffle_pd(v6, v6, 1).m128d_f64[0]) * v2.m128d_f64[0];
  }
  return result;
}

// ===== __CIlog_pentium4 @ 0x004D6680..0x004D6698 =====
void __usercall _CIlog_pentium4(double a1@<st0>)
{
  start_11(a1);
}

// ===== start_11 @ 0x004D669E..0x004D68EC =====
double __usercall start_11@<st0>(__m128d a1@<xmm0>, __m128i a2@<xmm2>, double a3)
{
  int i; // edx
  __m128i v4; // xmm5
  __m128d v5; // xmm0
  int v6; // eax
  __m128d v7; // xmm4
  __m128d v8; // xmm6
  __m128d v9; // xmm0
  __m128d v10; // xmm6
  __m128d v11; // xmm7
  __m128d v12; // xmm4
  unsigned int v13; // ecx
  int v14; // ecx
  __m128d v15; // xmm6
  int v16; // edx
  __m128d v17; // xmm3
  __m128d v18; // xmm7
  __m128d v19; // xmm0
  __m128d v21; // xmm0
  __m128i v22; // xmm1
  int v23; // edx
  double v24; // [esp+10h] [ebp-Ch] BYREF

  for ( i = 0; ; i = -52 )
  {
    v4 = (__m128i)a1;
    v5 = _mm_or_pd(_mm_and_pd(_mm_unpacklo_pd(a1, a1), (__m128d)xmmword_4EDCF0), (__m128d)xmmword_4EDD50);
    v6 = _mm_extract_epi16((__m128i)_mm_add_pd((__m128d)xmmword_4EDD00, v5), 0) & 0x7F0;
    v7 = *(__m128d *)((char *)&xmmword_4EDDD0 + v6);
    v11 = *(__m128d *)((char *)&xmmword_4EE1E0 + v6);
    v8 = _mm_and_pd((__m128d)xmmword_4EDD10, v5);
    v9 = _mm_sub_pd(v5, v8);
    v10 = _mm_sub_pd(_mm_mul_pd(v8, v7), (__m128d)xmmword_4EDD50);
    v11.m128d_f64[0] = v11.m128d_f64[0] + v10.m128d_f64[0];
    v12 = _mm_mul_pd(v9, v7);
    a1 = _mm_add_pd(v12, v10);
    v13 = (_mm_extract_epi16(_mm_srli_epi64(v4, 0x34u), 0) & 0xFFF) - 1;
    if ( v13 <= 0x7FD )
      break;
    v21.m128d_f64[0] = a3;
    v22 = (__m128i)_mm_cmpeq_sd((__m128d)xmmword_4EDD60, v21);
    if ( _mm_extract_epi16(v22, 0) )
    {
      *(double *)v22.m128i_i64 = -INFINITY;
      v23 = 2;
CALL_LIBM_ERROR_1:
      v24 = *(double *)v22.m128i_i64;
      __libm_error_support(&a3, &a3, &v24, v23);
      return v24;
    }
    if ( v13 != -1 )
    {
      if ( v13 > 0x7FE )
      {
        if ( (((_WORD)v13 + 1) & 0x7FFu) >= 0x7FF
          && (*(double *)a2.m128i_i64 = a3,
              _mm_cvtsi128_si32(_mm_srli_epi64(a2, 0x20u)) & 0xFFFFF | _mm_cvtsi128_si32(a2)) )
        {
          v23 = 1000;
        }
        else
        {
          *(double *)v22.m128i_i64 = 0.0 / 0.0;
          v23 = 3;
        }
      }
      else
      {
        a1.m128d_f64[0] = a3;
        v22.m128i_i64[0] = 0xFFFFFFFFFFFFFLL;
        if ( _mm_extract_epi16(
               (__m128i)_mm_cmpeq_sd(
                          (__m128d)xmmword_4EDD50,
                          _mm_or_pd(_mm_and_pd(a1, (__m128d)xmmword_4EDCF0), (__m128d)xmmword_4EDD50)),
               0) )
        {
          return INFINITY;
        }
        v23 = 1000;
      }
      goto CALL_LIBM_ERROR_1;
    }
    a1.m128d_f64[0] = a3 * 4.503599627370496e15;
  }
  v14 = i + v13 - 1022;
  v10.m128d_f64[0] = (double)v14;
  v15 = _mm_unpacklo_pd(v10, v10);
  v16 = 0;
  if ( !((v14 << 10) + v6) )
    v16 = 1;
  v17 = _mm_mul_pd(a1, a1);
  v18 = _mm_add_pd(
          _mm_add_pd(v11, _mm_mul_pd(v15, (__m128d)xmmword_4EDD20)),
          _mm_and_pd(v12, (__m128d)xmmword_4EDD30[v16]));
  v17.m128d_f64[0] = v17.m128d_f64[0] * v17.m128d_f64[0] * a1.m128d_f64[0];
  v19 = _mm_mul_pd(
          _mm_add_pd(
            _mm_mul_pd(_mm_add_pd(_mm_mul_pd((__m128d)xmmword_4EDDA0, a1), (__m128d)xmmword_4EDDB0), a1),
            (__m128d)xmmword_4EDDC0),
          v17);
  return v19.m128d_f64[0]
       + _mm_unpackhi_pd(v19, v19).m128d_f64[0]
       + _mm_unpackhi_pd(v18, v18).m128d_f64[0]
       + v18.m128d_f64[0];
}
