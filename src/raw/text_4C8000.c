#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4C80A0 @ 0x004C80A0..0x004C82E5 =====
void __cdecl sub_4C80A0(int a1)
{
  void **v1; // edi
  int v2; // ecx
  _DWORD *v3; // ebp
  int v4; // esi
  _DWORD *v5; // eax
  _DWORD *v6; // eax
  int v7; // ebx
  _DWORD *v8; // edi
  int v9; // ebx
  _DWORD *v10; // edi
  int v11; // ebx
  int v12; // edi
  int i; // ebx
  int v14; // [esp+4h] [ebp-4h]

  v1 = (void **)a1;
  if ( a1 )
  {
    v2 = *(_DWORD *)(a1 + 4);
    v14 = v2;
    if ( v2 )
      v3 = *(_DWORD **)(v2 + 28);
    else
      v3 = 0;
    v4 = *(_DWORD *)(a1 + 104);
    if ( v4 )
    {
      if ( *(_DWORD *)(v4 + 4) )
        free(*(void **)(v4 + 4));
      if ( *(_DWORD *)(v4 + 8) )
        free(*(void **)(v4 + 8));
      if ( *(_DWORD *)v4 )
      {
        sub_4CCA00(*(_DWORD *)v4);
        free(*(void **)v4);
      }
      v5 = *(_DWORD **)(v4 + 12);
      if ( v5 )
      {
        sub_4CE3E0(*v5);
        free(**(void ***)(v4 + 12));
        free(*(void **)(v4 + 12));
      }
      v6 = *(_DWORD **)(v4 + 16);
      if ( v6 )
      {
        sub_4CE3E0(*v6);
        free(**(void ***)(v4 + 16));
        free(*(void **)(v4 + 16));
      }
      if ( *(_DWORD *)(v4 + 48) )
      {
        v7 = 0;
        if ( (int)v3[4] > 0 )
        {
          v8 = v3 + 200;
          do
            (*((void (__cdecl **)(_DWORD))*(&off_502DD8 + *v8++) + 4))(*(_DWORD *)(*(_DWORD *)(v4 + 48) + 4 * v7++));
          while ( v7 < v3[4] );
        }
        free(*(void **)(v4 + 48));
        v1 = (void **)a1;
      }
      if ( *(_DWORD *)(v4 + 52) )
      {
        v9 = 0;
        if ( (int)v3[5] > 0 )
        {
          v10 = v3 + 328;
          do
            (*((void (__cdecl **)(_DWORD))*(&off_502DE0 + *v10++) + 4))(*(_DWORD *)(*(_DWORD *)(v4 + 52) + 4 * v9++));
          while ( v9 < v3[5] );
        }
        free(*(void **)(v4 + 52));
        v1 = (void **)a1;
      }
      if ( *(_DWORD *)(v4 + 56) )
      {
        v11 = 0;
        if ( (int)v3[7] > 0 )
        {
          v12 = 0;
          do
          {
            sub_4C9810(*(_DWORD *)(v4 + 56) + v12);
            ++v11;
            v12 += 48;
          }
          while ( v11 < v3[7] );
        }
        free(*(void **)(v4 + 56));
        v1 = (void **)a1;
      }
      if ( *(_DWORD *)(v4 + 60) )
        sub_4C8A80(*(void **)(v4 + 60));
      sub_4CC900(v4 + 80);
      sub_4CE010(v4 + 20);
      sub_4CE010(v4 + 32);
      v2 = v14;
    }
    if ( v1[2] )
    {
      for ( i = 0; i < *(_DWORD *)(v2 + 4); ++i )
      {
        if ( *((_DWORD *)v1[2] + i) )
        {
          free(*((void **)v1[2] + i));
          v2 = v14;
        }
      }
      free(v1[2]);
      if ( v1[3] )
        free(v1[3]);
    }
    if ( v4 )
    {
      if ( *(_DWORD *)(v4 + 64) )
        free(*(void **)(v4 + 64));
      if ( *(_DWORD *)(v4 + 68) )
        free(*(void **)(v4 + 68));
      if ( *(_DWORD *)(v4 + 72) )
        free(*(void **)(v4 + 72));
      free((void *)v4);
    }
    memset(v1, 0, 0x70u);
  }
}

// ===== sub_4C82F0 @ 0x004C82F0..0x004C831B =====
int __cdecl sub_4C82F0(_DWORD *a1, _DWORD *a2)
{
  sub_4C7D20(a1, a2, 0);
  a1[14] = -1;
  a1[16] = -1;
  a1[6] = -1;
  a1[15] = -1;
  a1[17] = -1;
  return 0;
}

// ===== sub_4C8320 @ 0x004C8320..0x004C872F =====
int __cdecl sub_4C8320(_DWORD *a1, int a2)
{
  int v4; // ebp
  _DWORD *v5; // ebx
  int v7; // eax
  __int64 v8; // rax
  __int64 v9; // rax
  int v10; // esi
  int v11; // ebx
  bool v12; // cf
  __int64 v13; // rax
  __int64 v14; // rax
  __int64 v15; // rax
  int v16; // eax
  int v17; // edx
  int v18; // eax
  bool v19; // zf
  float *v20; // eax
  int v21; // edx
  int v22; // ebx
  double v23; // st7
  float *v24; // eax
  int v25; // ebx
  int v26; // ebp
  int v27; // eax
  float *v28; // edx
  int v29; // ebp
  double v30; // st7
  int v31; // ebx
  int v32; // ebp
  int v33; // ebx
  float *v34; // edx
  int v35; // eax
  int v36; // edx
  int v37; // ebx
  double v38; // st7
  __int64 v39; // rax
  int v40; // edx
  __int64 v41; // rax
  unsigned int v42; // esi
  unsigned int v43; // ebx
  int v44; // eax
  int v45; // ebx
  int v46; // eax
  float *v47; // [esp+10h] [ebp-28h]
  int v48; // [esp+14h] [ebp-24h]
  int v49; // [esp+14h] [ebp-24h]
  int v50; // [esp+18h] [ebp-20h]
  _DWORD *v51; // [esp+1Ch] [ebp-1Ch]
  int v52; // [esp+20h] [ebp-18h]
  int v53; // [esp+20h] [ebp-18h]
  int v54; // [esp+20h] [ebp-18h]
  int v55; // [esp+24h] [ebp-14h]
  int v56; // [esp+28h] [ebp-10h]
  int i; // [esp+2Ch] [ebp-Ch]
  int v58; // [esp+30h] [ebp-8h]
  int v59; // [esp+34h] [ebp-4h]
  int v60; // [esp+3Ch] [ebp+4h]
  int v61; // [esp+40h] [ebp+8h]

  v4 = 0;
  v5 = *(_DWORD **)(a1[1] + 28);
  v56 = a1[1];
  v51 = v5;
  if ( !a2 )
    return -131;
  v7 = a1[6];
  if ( a1[5] > v7 && v7 != -1 )
    return -131;
  a1[9] = a1[10];
  LODWORD(v8) = a1[16];
  a1[10] = *(_DWORD *)(a2 + 28);
  HIDWORD(v8) = a1[17];
  a1[11] = -1;
  if ( v8 + 1 != *(_QWORD *)(a2 + 56) )
  {
    a1[14] = -1;
    a1[15] = -1;
  }
  a1[16] = *(_DWORD *)(a2 + 56);
  a1[17] = *(_DWORD *)(a2 + 60);
  if ( *(_DWORD *)a2 )
  {
    v55 = v5[a1[10]] / 2;
    v9 = *(int *)(a2 + 88);
    v10 = *v5 / 2;
    v11 = v5[1] / 2;
    v12 = __CFADD__((_DWORD)v9, a1[18]);
    a1[18] += v9;
    v60 = v11;
    a1[19] += HIDWORD(v9) + v12;
    v13 = *(int *)(a2 + 92);
    v12 = __CFADD__((_DWORD)v13, a1[20]);
    a1[20] += v13;
    a1[21] += HIDWORD(v13) + v12;
    v14 = *(int *)(a2 + 96);
    v12 = __CFADD__((_DWORD)v14, a1[22]);
    a1[22] += v14;
    a1[23] += HIDWORD(v14) + v12;
    v15 = *(int *)(a2 + 100);
    v12 = __CFADD__((_DWORD)v15, a1[24]);
    a1[24] += v15;
    a1[25] += HIDWORD(v15) + v12;
    if ( a1[12] )
    {
      v16 = 0;
      v50 = v11;
      v61 = 0;
    }
    else
    {
      v50 = 0;
      v61 = v11;
      v16 = v11;
    }
    v48 = 0;
    if ( *(int *)(v56 + 4) > 0 )
    {
      do
      {
        v17 = a1[10];
        if ( a1[9] )
        {
          if ( v17 )
          {
            v18 = *(_DWORD *)(a1[2] + 4 * v4) + 4 * v16;
            if ( v11 > 0 )
            {
              v11 = v60;
              v52 = *(_DWORD *)(*(_DWORD *)a2 + 4 * v4) - v18;
              v49 = v60;
              do
              {
                v18 += 4;
                v19 = v49-- == 1;
                *(float *)(v18 - 4) = *(float *)(v52 + v18 - 4) + *(float *)(v18 - 4);
              }
              while ( !v19 );
            }
            goto LABEL_34;
          }
          v20 = (float *)(*(_DWORD *)(a1[2] + 4 * v4) + 4 * (v61 + v11 / 2 - v10 / 2));
          if ( v10 > 0 )
          {
            v21 = *(_DWORD *)(*(_DWORD *)a2 + 4 * v4) - (_DWORD)v20;
            v22 = v10;
            do
            {
              v23 = *(float *)((char *)v20 + v21) + *v20;
              ++v20;
              --v22;
              *(v20 - 1) = v23;
            }
            while ( v22 );
          }
        }
        else
        {
          v24 = (float *)(*(_DWORD *)(a1[2] + 4 * v4) + 4 * v16);
          if ( v17 )
          {
            v47 = v24;
            v25 = v11 / 2;
            v26 = *(_DWORD *)(*(_DWORD *)a2 + 4 * v4) + 4 * (v25 - v10 / 2);
            v27 = 0;
            v58 = v26;
            if ( v10 > 0 )
            {
              v28 = v47;
              v53 = v10;
              v29 = v26 - (_DWORD)v47;
              v27 = v10;
              for ( i = v29; ; v29 = i )
              {
                v30 = *(float *)((char *)v28 + v29) + *v28;
                ++v28;
                v19 = v53-- == 1;
                *(v28 - 1) = v30;
                if ( v19 )
                  break;
              }
              v26 = v58;
            }
            v31 = v10 / 2 + v25;
            if ( v27 < v31 )
            {
              v32 = v26 - (_DWORD)v47;
              v33 = v31 - v27;
              v34 = &v47[v27];
              do
              {
                *v34 = *(float *)((char *)v34 + v32);
                ++v34;
                --v33;
              }
              while ( v33 );
            }
            v4 = v48;
          }
          else
          {
            if ( v10 <= 0 )
              goto LABEL_34;
            v36 = *(_DWORD *)(*(_DWORD *)a2 + 4 * v4) - (_DWORD)v24;
            v37 = v10;
            do
            {
              v38 = *(float *)((char *)v24 + v36) + *v24;
              ++v24;
              --v37;
              *(v24 - 1) = v38;
            }
            while ( v37 );
          }
        }
        v11 = v60;
LABEL_34:
        if ( v55 > 0 )
        {
          v35 = *(_DWORD *)(a1[2] + 4 * v4) + 4 * v50;
          v59 = *(_DWORD *)(*(_DWORD *)a2 + 4 * v4) + 4 * v55 - v35;
          v54 = v55;
          do
          {
            v35 += 4;
            *(_DWORD *)(v35 - 4) = *(_DWORD *)(v59 + v35 - 4);
            --v54;
          }
          while ( v54 );
        }
        v48 = ++v4;
        v16 = v61;
      }
      while ( v4 < *(_DWORD *)(v56 + 4) );
    }
    a1[12] = a1[12] == 0 ? v11 : 0;
    if ( a1[6] == -1 )
    {
      a1[6] = v50;
      a1[5] = v50;
    }
    else
    {
      a1[6] = v16;
      v39 = (int)v51[a1[10]];
      a1[5] = (((BYTE4(v39) & 3) + (int)v39) >> 2) + v61 + v51[a1[9]] / 4;
    }
    v5 = v51;
  }
  if ( (a1[15] & a1[14]) == -1 )
  {
    v40 = *(_DWORD *)(a2 + 52);
    if ( (v40 & *(_DWORD *)(a2 + 48)) != 0xFFFFFFFF )
    {
      a1[14] = *(_DWORD *)(a2 + 48);
LABEL_59:
      a1[15] = v40;
    }
  }
  else
  {
    *((_QWORD *)a1 + 7) += v5[a1[10]] / 4 + v5[a1[9]] / 4;
    v41 = *(_QWORD *)(a2 + 48);
    if ( (HIDWORD(v41) & *(_DWORD *)(a2 + 48)) != -1 )
    {
      v42 = a1[14];
      v43 = a1[15];
      if ( __PAIR64__(v43, v42) != v41 )
      {
        if ( __SPAIR64__(v43, v42) > v41 )
        {
          v44 = a1[14] - *(_DWORD *)(a2 + 48);
          if ( *(_DWORD *)(a2 + 44) )
          {
            a1[5] -= v44;
          }
          else if ( *(_DWORD *)(a2 + 56) == 1 && !*(_DWORD *)(a2 + 60) )
          {
            v45 = v44 + a1[6];
            v46 = a1[5];
            a1[6] = v45;
            if ( v45 > v46 )
              a1[6] = v46;
          }
        }
        a1[14] = *(_DWORD *)(a2 + 48);
        v40 = *(_DWORD *)(a2 + 52);
        goto LABEL_59;
      }
    }
  }
  if ( *(_DWORD *)(a2 + 44) )
    a1[8] = 1;
  return 0;
}

// ===== sub_4C8730 @ 0x004C8730..0x004C8788 =====
int __cdecl sub_4C8730(_DWORD *a1, _DWORD *a2)
{
  int v2; // eax
  int v3; // edx
  int i; // eax
  int v5; // edi

  v2 = a1[6];
  v3 = a1[1];
  if ( v2 <= -1 || v2 >= a1[5] )
    return 0;
  if ( a2 )
  {
    for ( i = 0; i < *(_DWORD *)(v3 + 4); *(_DWORD *)(a1[3] + 4 * i - 4) = v5 + 4 * a1[6] )
      v5 = *(_DWORD *)(a1[2] + 4 * i++);
    *a2 = a1[3];
  }
  return a1[5] - a1[6];
}

// ===== sub_4C8790 @ 0x004C8790..0x004C87BB =====
int __cdecl sub_4C8790(int a1, int a2)
{
  if ( a2 && a2 + *(_DWORD *)(a1 + 24) > *(_DWORD *)(a1 + 20) )
    return -131;
  *(_DWORD *)(a1 + 24) += a2;
  return 0;
}

// ===== sub_4C87C0 @ 0x004C87C0..0x004C8916 =====
int __cdecl sub_4C87C0(int *a1, int a2)
{
  int v3; // eax
  _DWORD *v4; // ebx
  int v5; // ebp
  int v8; // eax
  int *v9; // edx
  bool v10; // zf
  int v11; // eax
  __int64 v12; // kr00_8
  int v13; // edx
  int i; // edi
  int v15; // eax
  int v17; // [esp+10h] [ebp-4h]
  int v18; // [esp+18h] [ebp+4h]
  int v19; // [esp+1Ch] [ebp+8h]

  v3 = a1[16];
  v4 = a1 + 1;
  v5 = *(_DWORD *)(v3 + 4);
  v17 = *(_DWORD *)(v3 + 104);
  v18 = *(_DWORD *)(v5 + 28);
  sub_4C7C50((int)a1);
  sub_4D4AF0(v4, *(_DWORD *)a2, *(_DWORD *)(a2 + 4));
  if ( sub_4D4D00(v4, 1) )
    return -135;
  v8 = sub_4D4D00(v4, *(_DWORD *)(v17 + 44));
  v19 = v8;
  if ( v8 == -1 )
    return -136;
  a1[10] = v8;
  v9 = *(int **)(v18 + 4 * v8 + 32);
  v10 = *v9 == 0;
  a1[7] = *v9;
  if ( v10 )
  {
    a1[6] = 0;
    a1[8] = 0;
  }
  else
  {
    a1[6] = sub_4D4D00(v4, 1);
    v11 = sub_4D4D00(v4, 1);
    a1[8] = v11;
    if ( v11 == -1 )
      return -136;
  }
  a1[12] = *(_DWORD *)(a2 + 16);
  a1[13] = *(_DWORD *)(a2 + 20);
  v12 = *(_QWORD *)(a2 + 24) - 3LL;
  a1[14] = v12;
  v13 = a1[7];
  a1[15] = HIDWORD(v12);
  a1[11] = *(_DWORD *)(a2 + 12);
  a1[9] = *(_DWORD *)(v18 + 4 * v13);
  *a1 = sub_4C7BE0(a1, 4 * *(_DWORD *)(v5 + 4));
  for ( i = 0; i < *(_DWORD *)(v5 + 4); ++i )
    *(_DWORD *)(*a1 + 4 * i) = sub_4C7BE0(a1, 4 * a1[9]);
  v15 = *(_DWORD *)(*(_DWORD *)(v18 + 4 * v19 + 32) + 12);
  return ((int (__cdecl *)(int *, _DWORD))(&off_502DEC)[*(_DWORD *)(v18 + 4 * v15 + 288)][4])(
           a1,
           *(_DWORD *)(v18 + 4 * v15 + 544));
}

// ===== sub_4C8920 @ 0x004C8920..0x004C89FA =====
int __cdecl sub_4C8920(int a1, int a2)
{
  int v3; // eax
  int v4; // edi
  int v5; // ebp
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v11; // [esp+14h] [ebp+4h]

  v3 = *(_DWORD *)(a1 + 64);
  v4 = a1 + 4;
  v5 = *(_DWORD *)(v3 + 104);
  v11 = *(_DWORD *)(*(_DWORD *)(v3 + 4) + 28);
  sub_4C7C50(a1);
  sub_4D4AF0(v4, *(_DWORD *)a2, *(_DWORD *)(a2 + 4));
  if ( sub_4D4D00(v4, 1) )
    return -135;
  v7 = sub_4D4D00(v4, *(_DWORD *)(v5 + 44));
  if ( v7 == -1 )
    return -136;
  *(_DWORD *)(a1 + 40) = v7;
  v8 = **(_DWORD **)(v11 + 4 * v7 + 32);
  *(_DWORD *)(a1 + 28) = v8;
  if ( v8 )
  {
    *(_DWORD *)(a1 + 24) = sub_4D4D00(v4, 1);
    v9 = sub_4D4D00(v4, 1);
    *(_DWORD *)(a1 + 32) = v9;
    if ( v9 == -1 )
      return -136;
  }
  else
  {
    *(_DWORD *)(a1 + 24) = 0;
    *(_DWORD *)(a1 + 32) = 0;
  }
  *(_DWORD *)(a1 + 48) = *(_DWORD *)(a2 + 16);
  *(_DWORD *)(a1 + 52) = *(_DWORD *)(a2 + 20);
  *(_QWORD *)(a1 + 56) = *(_QWORD *)(a2 + 24) - 3LL;
  *(_DWORD *)(a1 + 44) = *(_DWORD *)(a2 + 12);
  *(_DWORD *)(a1 + 36) = 0;
  *(_DWORD *)a1 = 0;
  return 0;
}

// ===== sub_4C8A00 @ 0x004C8A00..0x004C8A7A =====
int __cdecl sub_4C8A00(int a1, _DWORD *a2)
{
  int v2; // esi
  int v4; // eax
  int i; // ecx
  int v6; // eax
  _BYTE v7[20]; // [esp+4h] [ebp-14h] BYREF

  v2 = *(_DWORD *)(a1 + 28);
  sub_4D4AF0(v7, *a2, a2[1]);
  if ( sub_4D4D00(v7, 1) )
    return -135;
  v4 = *(_DWORD *)(v2 + 8);
  for ( i = 0; v4 > 1; ++i )
    v4 >>= 1;
  v6 = sub_4D4D00(v7, i);
  if ( v6 == -1 )
    return -136;
  else
    return *(_DWORD *)(v2 + 4 * **(_DWORD **)(v2 + 4 * v6 + 32));
}

// ===== sub_4C8A80 @ 0x004C8A80..0x004C8A9F =====
void __cdecl sub_4C8A80(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x24u);
    free(Block);
  }
}

// ===== sub_4C8AA0 @ 0x004C8AA0..0x004C8ABF =====
void __cdecl sub_4C8AA0(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0x210u);
    free(Block);
  }
}

// ===== sub_4C8AC0 @ 0x004C8AC0..0x004C9004 =====
char *__cdecl sub_4C8AC0(int *a1, int a2, int *a3, int a4, int a5)
{
  int v6; // ebp
  __int64 v7; // rax
  double v8; // st7
  double v9; // st5
  int v10; // edi
  int v11; // edi
  int v12; // eax
  int v13; // ebx
  double v14; // st7
  long double v17; // st6
  __int64 v18; // rax
  float *v19; // ecx
  double v20; // st7
  int v21; // ebx
  int v22; // ecx
  int v23; // edx
  int v24; // edi
  int v25; // edi
  int v26; // ebx
  __int64 v27; // rax
  int j; // ebx
  char *result; // eax
  int v30; // ebx
  long double v31; // st6
  long double v32; // st6
  int v33; // ecx
  long double v34; // st6
  int v35; // edx
  long double v36; // st4
  long double v37; // st3
  float v38; // [esp+0h] [ebp-44h]
  int v39; // [esp+20h] [ebp-24h]
  int v40; // [esp+24h] [ebp-20h]
  float v41; // [esp+28h] [ebp-1Ch]
  float v42; // [esp+30h] [ebp-14h]
  double v43; // [esp+34h] [ebp-10h]
  double v44; // [esp+3Ch] [ebp-8h]
  int v45; // [esp+48h] [ebp+4h]
  int v46; // [esp+48h] [ebp+4h]
  float *v47; // [esp+50h] [ebp+Ch]
  float v48; // [esp+50h] [ebp+Ch]
  int v49; // [esp+50h] [ebp+Ch]
  int i; // [esp+54h] [ebp+10h]
  int k; // [esp+54h] [ebp+10h]
  int v52; // [esp+54h] [ebp+10h]
  int v53; // [esp+58h] [ebp+14h]
  int v54; // [esp+58h] [ebp+14h]

  memset(a1, 0, 0x30u);
  v39 = -99;
  v6 = 0;
  a1[9] = *a3;
  v7 = (__int64)(floor(__FYL2X__((double)*a3 * 8.0, 0.6931471805599453094) / __FYL2X__(2.0, 0.6931471805599453094) + 0.5)
               - 1.0);
  v8 = (double)a5;
  v43 = (double)a4;
  a1[8] = v7;
  v9 = (double)(1 << (v7 + 1));
  v10 = (__int64)((__FYL2X__(v8 * 0.25 * 0.5 / v43, 0.6931471805599453094) * 1.442695021629333 - 5.965784072875977) * v9
                - (double)*a3);
  a1[7] = v10;
  LODWORD(v7) = (__int64)((__FYL2X__((v43 + 0.25) * v8 * 0.5 / v43, 0.6931471805599453094) * 1.442695021629333
                         - 5.965784072875977)
                        * v9
                        + 0.5)
              - v10;
  v11 = a4;
  a1[10] = v7 + 1;
  a1[4] = (int)malloc(4 * a4);
  a1[5] = (int)malloc(4 * a4);
  a1[6] = (int)malloc(4 * a4);
  v44 = v8;
  a1[1] = a2;
  v12 = 0;
  *a1 = a4;
  a1[11] = a5;
  v13 = 0;
  v47 = (float *)&unk_4FCE74;
  do
  {
    v45 = v12 + 1;
    v14 = ((double)(v12 + 1) * 0.125 - 2.0 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
    _ST6 = v14;
    __asm { frndint }
    v17 = __FSCALE__(__F2XM1__(v14 - _ST6) + 1.0, _ST6);
    v18 = (__int64)floor((v17 * v43 + v17 * v43) / v44 + 0.5);
    v19 = v47;
    v20 = *(v47 - 1);
    if ( v13 < (int)v18 )
    {
      v48 = (*v47 - v20) / (double)((int)v18 - v13);
      do
      {
        if ( v13 >= a4 )
          break;
        ++v13;
        *(float *)(a1[4] + 4 * v13 - 4) = v20 + 100.0;
        v20 = v20 + v48;
      }
      while ( v13 < (int)v18 );
    }
    v12 = v45;
    v47 = v19 + 1;
  }
  while ( (int)(v19 + 1) < (int)&unk_4FCFD0 );
  v21 = 0;
  if ( a4 > 0 )
  {
    v49 = 0;
    v46 = 0;
    v22 = a5 / (2 * a4);
    v23 = v22 * v22;
    do
    {
      v41 = atan2((double)(v21 * v46) * 0.0000000185, 1.0) * 2.240000009536743
          + atan2((double)v49 * 0.00073999999, 1.0) * 13.10000038146973
          + (double)v49 * 0.000099999997;
      if ( v39 + *(_DWORD *)(a2 + 120) < v21 )
      {
        v40 = v39 + *(_DWORD *)(a2 + 120);
        v24 = v39 * v23;
        v53 = v39 * v22;
        do
        {
          v42 = v41 - *(float *)(a2 + 112);
          if ( v42 <= atan2((double)(v39 * v24) * 0.0000000185, 1.0) * 2.240000009536743
                    + atan2((double)v53 * 0.00073999999, 1.0) * 13.10000038146973
                    + (double)v53 * 0.000099999997 )
            break;
          v24 += v23;
          ++v39;
          v53 += v22;
          ++v40;
        }
        while ( v40 < v21 );
      }
      if ( v6 < a4 )
      {
        v25 = v6 * v23;
        v54 = v6 * v22;
        do
        {
          if ( v6 >= v21 + *(_DWORD *)(a2 + 124)
            && v41 + *(float *)(a2 + 116) <= atan2((double)(v6 * v25) * 0.0000000185, 1.0) * 2.240000009536743
                                           + atan2((double)v54 * 0.00073999999, 1.0) * 13.10000038146973
                                           + (double)v54 * 0.000099999997 )
          {
            break;
          }
          ++v6;
          v25 += v23;
          v54 += v22;
        }
        while ( v6 < a4 );
      }
      *(_DWORD *)(a1[6] + 4 * v21) = v6 + (v39 << 16);
      v46 += v23;
      ++v21;
      v49 += v22;
    }
    while ( v21 < a4 );
    v11 = a4;
  }
  v26 = 0;
  for ( i = 0; v26 < v11; *(_DWORD *)(a1[5] + 4 * v26 - 4) = v27 )
  {
    v27 = (__int64)((__FYL2X__(((double)i + 0.25) * v44 * 0.5 / v43, 0.6931471805599453094) * 1.442695021629333
                   - 5.965784072875977)
                  * (double)(1 << (*((_BYTE *)a1 + 32) + 1))
                  + 0.5);
    i = ++v26;
  }
  v38 = v44 * 0.5 / v43;
  a1[2] = sub_4C9010(a2 + 36, v38, v11, *(float *)(a2 + 24), *(float *)(a2 + 28));
  a1[3] = (int)malloc(0xCu);
  for ( j = 0; j < 12; *(_DWORD *)(a1[3] + j - 4) = result )
  {
    result = (char *)malloc(4 * v11);
    j += 4;
  }
  v30 = 0;
  for ( k = 0; v30 < v11; k = v30 )
  {
    v31 = __FYL2X__(((double)k + 0.5) * v44 / (v43 + v43), 0.6931471805599453094);
    v32 = v31 * 1.442695021629333 - 5.965784072875977 + v31 * 1.442695021629333 - 5.965784072875977;
    if ( v32 >= 0.0 )
    {
      if ( v32 >= 16.0 )
        v32 = 16.0;
    }
    else
    {
      v32 = 0.0;
    }
    v52 = (__int64)v32;
    v33 = 0;
    v34 = v32 - (double)v52;
    result = (char *)(4 * v52 + 136);
    do
    {
      v35 = a1[1];
      v33 += 4;
      v36 = *(float *)&result[v35 - 4] * (1.0 - v34);
      v37 = v34 * *(float *)&result[v35];
      result += 68;
      *(float *)(*(_DWORD *)(a1[3] + v33 - 4) + 4 * v30) = v36 + v37;
    }
    while ( v33 < 12 );
    ++v30;
  }
  return result;
}

// ===== sub_4C9010 @ 0x004C9010..0x004C978D =====
_DWORD *__cdecl sub_4C9010(int a1, float a2, int a3, float a4, float a5)
{
  int v5; // eax
  void *v6; // esp
  float *v7; // esi
  float *v8; // edi
  int v9; // ebx
  double v10; // st7
  float *v11; // ecx
  int v12; // edx
  int v13; // eax
  float *v14; // edx
  float *v15; // ecx
  int v16; // edi
  int i; // esi
  double v18; // st7
  double v19; // st7
  int *v20; // ebx
  int v21; // esi
  bool v22; // cc
  int v23; // edi
  char *v24; // esi
  int v25; // ebx
  double v26; // st7
  int v29; // edi
  long double v30; // st7
  int v31; // esi
  long double v32; // st7
  __int64 v33; // rax
  int v34; // ebx
  int v35; // eax
  int v36; // esi
  double v37; // st6
  double v38; // st5
  int v41; // edi
  double v42; // st6
  __int64 v45; // rax
  int v46; // ebx
  int v47; // edx
  float *v48; // ecx
  int v49; // edx
  float *v50; // ecx
  int v51; // esi
  double v52; // st7
  double v53; // st6
  int v56; // edi
  double v57; // st7
  int v60; // ecx
  int v61; // eax
  float *v62; // edx
  double v63; // st7
  int v64; // ecx
  bool v65; // c0
  bool v66; // c3
  float *v67; // ecx
  int v68; // ecx
  float *v69; // edi
  float *v70; // edx
  int n; // esi
  double v72; // st7
  __int64 v75; // rax
  int v76; // ecx
  float *v77; // esi
  float *v78; // edx
  _DWORD *v79; // edx
  double v80; // st7
  int v81; // ecx
  int v82; // esi
  float *v83; // edx
  float v85; // [esp+4h] [ebp-7F18h]
  float v86; // [esp+4h] [ebp-7F18h]
  int v87; // [esp+8h] [ebp-7F14h] BYREF
  float v88[7616]; // [esp+14h] [ebp-7F08h] BYREF
  int v89; // [esp+7714h] [ebp-808h] BYREF
  char v90; // [esp+77F4h] [ebp-728h] BYREF
  _BYTE v91[224]; // [esp+7E14h] [ebp-108h] BYREF
  int v92; // [esp+7EF4h] [ebp-28h]
  double v93; // [esp+7EF8h] [ebp-24h]
  int v94; // [esp+7F00h] [ebp-1Ch]
  char *v95; // [esp+7F04h] [ebp-18h]
  int v96; // [esp+7F08h] [ebp-14h]
  float *v97; // [esp+7F0Ch] [ebp-10h]
  int v98; // [esp+7F10h] [ebp-Ch]
  _DWORD *v99; // [esp+7F14h] [ebp-8h]
  int v100; // [esp+7F18h] [ebp-4h]
  int v101; // [esp+7F24h] [ebp+8h]
  int k; // [esp+7F24h] [ebp+8h]
  int m; // [esp+7F24h] [ebp+8h]
  int v104; // [esp+7F24h] [ebp+8h]
  int j; // [esp+7F30h] [ebp+14h]
  int v106; // [esp+7F34h] [ebp+18h]
  int v107; // [esp+7F34h] [ebp+18h]

  v5 = 4 * a3 + 3;
  LOBYTE(v5) = v5 & 0xFC;
  v6 = alloca(v5);
  v95 = (char *)&v87;
  v99 = malloc(0x44u);
  memset(v88, 0, sizeof(v88));
  v98 = a1;
  v100 = (int)v88;
  v97 = flt_4FCFD0;
  v96 = (int)&unk_4FCE70;
  do
  {
    v7 = (float *)v96;
    v8 = (float *)v91;
    v9 = 56;
    do
    {
      v10 = 999.0;
      v11 = v7;
      v12 = 4;
      do
      {
        if ( (int)v11 >= (int)flt_4FCFD0 )
        {
          if ( v10 > flt_4FCFCC )
            v10 = flt_4FCFCC;
        }
        else if ( v10 > *v11 )
        {
          v10 = *v11;
        }
        ++v11;
        --v12;
      }
      while ( v12 );
      *v8 = v10;
      ++v7;
      ++v8;
      --v9;
    }
    while ( v9 );
    v13 = v100;
    v14 = v97;
    qmemcpy((void *)(v100 + 448), v97, 0x540u);
    qmemcpy((void *)v13, v14, 0xE0u);
    v92 = v13 + 224;
    qmemcpy((void *)(v13 + 224), v14, 0xE0u);
    v15 = (float *)v13;
    v16 = 8;
    do
    {
      for ( i = 16; i > -40; --i )
      {
        v18 = (double)(int)abs32(i) * a5 + a4;
        if ( v18 < 0.0 && a4 > 0.0 )
          v18 = 0.0;
        if ( v18 > 0.0 && a4 < 0.0 )
          v18 = 0.0;
        v19 = v18 + *v15++;
        *(v15 - 1) = v19;
      }
      --v16;
      HIDWORD(v93) = v15;
    }
    while ( v16 );
    v101 = 0;
    v20 = &v89;
    do
    {
      v94 = 2;
      if ( v101 >= 2 )
        v94 = v101;
      v85 = *(float *)v98 + 100.0 - (double)v94 * 10.0 - 30.0;
      sub_4C97F0(v100, v85);
      qmemcpy(v20, v91, 0xE0u);
      v86 = 100.0 - (double)v101 * 10.0 - 30.0;
      sub_4C97F0((int)v20, v86);
      v21 = v100;
      sub_4C97C0(v20, v100);
      v20 += 56;
      v22 = ++v101 < 8;
      v100 = v21 + 224;
    }
    while ( v22 );
    v23 = v92;
    v24 = &v90;
    v25 = 7;
    do
    {
      sub_4C9790(v24, v24 - 224);
      sub_4C9790(v23, v24);
      v24 += 224;
      v23 += 224;
      --v25;
    }
    while ( v25 );
    v22 = v96 + 16 < (int)&unk_4FCF80;
    v97 += 336;
    v96 += 16;
    v98 += 4;
    v100 = HIDWORD(v93);
  }
  while ( v22 );
  for ( j = 0; j < 17; ++j )
  {
    v99[j] = malloc(0x20u);
    v93 = (double)j * 0.5;
    v26 = (v93 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
    _ST6 = v26;
    __asm { frndint }
    v29 = (__int64)floor(__FSCALE__(__F2XM1__(v26 - _ST6) + 1.0, _ST6) / a2);
    v30 = __FYL2X__((double)v29 * a2 + 1.0, 0.6931471805599453094);
    v31 = (__int64)ceil(v30 * 1.442695021629333 - 5.965784072875977 + v30 * 1.442695021629333 - 5.965784072875977);
    v98 = v31;
    v32 = __FYL2X__((double)(v29 + 1) * a2, 0.6931471805599453094);
    v33 = (__int64)floor(v32 * 1.442695021629333 - 5.965784072875977 + v32 * 1.442695021629333 - 5.965784072875977);
    v100 = v33;
    if ( v31 > j )
    {
      v31 = j;
      v98 = j;
    }
    v34 = 0;
    if ( v31 < 0 )
      v98 = 0;
    if ( (int)v33 >= 17 )
      v100 = 16;
    v94 = 0;
    do
    {
      *(_DWORD *)(v99[j] + 4 * v34) = malloc(0xE8u);
      if ( a3 > 0 )
        memset32(v95, 1148829696, a3);
      v106 = v98;
      if ( v98 <= v100 )
      {
        v35 = v34 + 8 * v98;
        v96 = 56 * v35;
        v97 = &v88[56 * v35 + 55];
        do
        {
          v36 = 0;
          for ( k = 0; k < 56; ++k )
          {
            v37 = (double)k * 0.125 + (double)v106 * 0.5;
            v38 = (v37 - 2.0625 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
            _ST4 = v38;
            __asm { frndint }
            v41 = (__int64)(__FSCALE__(__F2XM1__(v38 - _ST4) + 1.0, _ST4) / a2);
            v42 = (v37 - 1.9375 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
            _ST5 = v42;
            __asm { frndint }
            v45 = (__int64)(__FSCALE__(__F2XM1__(v42 - _ST5) + 1.0, _ST5) / a2 + 1.0);
            v46 = v45;
            if ( v41 < 0 )
              v41 = 0;
            v47 = a3;
            if ( v41 > a3 )
              v41 = a3;
            if ( v41 < v36 )
              v36 = v41;
            if ( (int)v45 < 0 )
              v46 = 0;
            if ( v46 > a3 )
              v46 = a3;
            if ( v36 < v46 )
            {
              v48 = (float *)&v95[4 * v36];
              do
              {
                if ( v36 >= v47 )
                  break;
                if ( *v48 > (double)v88[k + v96] )
                  *v48 = v88[k + v96];
                v47 = a3;
                ++v36;
                ++v48;
              }
              while ( v36 < v46 );
            }
          }
          if ( v36 < v47 )
          {
            v49 = v47 - v36;
            v50 = (float *)&v95[4 * v36];
            do
            {
              if ( *v50 > (double)*v97 )
                *v50 = *v97;
              ++v50;
              --v49;
            }
            while ( v49 );
          }
          v22 = ++v106 <= v100;
          v96 += 448;
          v97 += 448;
        }
        while ( v22 );
        v34 = v94;
      }
      if ( j + 1 < 17 )
      {
        v51 = 0;
        for ( m = 0; m < 56; ++m )
        {
          v52 = (double)m * 0.125 + v93;
          v53 = (v52 - 2.0625 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
          _ST5 = v53;
          __asm { frndint }
          v56 = (__int64)(__FSCALE__(__F2XM1__(v53 - _ST5) + 1.0, _ST5) / a2);
          v57 = (v52 - 1.9375 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
          _ST6 = v57;
          __asm { frndint }
          v107 = (__int64)(__FSCALE__(__F2XM1__(v57 - _ST6) + 1.0, _ST6) / a2 + 1.0);
          if ( v56 < 0 )
            v56 = 0;
          v60 = a3;
          if ( v56 > a3 )
            v56 = a3;
          if ( v56 < v51 )
            v51 = v56;
          v61 = v107;
          if ( v107 < 0 )
          {
            v61 = 0;
            v107 = 0;
          }
          if ( v61 > a3 )
          {
            v61 = a3;
            v107 = a3;
          }
          if ( v51 < v61 )
          {
            v62 = (float *)&v95[4 * v51];
            do
            {
              if ( v51 >= v60 )
                break;
              v63 = *v62;
              v64 = m + 56 * (v34 + 8 * (j + 1));
              v65 = v63 < v88[v64];
              v66 = v63 == v88[v64];
              v67 = &v88[v64];
              if ( !v65 && !v66 )
                *v62 = *v67;
              v60 = a3;
              ++v51;
              ++v62;
            }
            while ( v51 < v107 );
          }
        }
        if ( v51 < v60 )
        {
          v68 = a3 - v51;
          v69 = &v88[448 * j + 503 + 56 * v34];
          v70 = (float *)&v95[4 * v51];
          do
          {
            if ( *v70 > (double)*v69 )
              *v70 = *v69;
            ++v70;
            --v68;
          }
          while ( v68 );
        }
      }
      v104 = 0;
      for ( n = 8; n < 232; n += 4 )
      {
        v72 = ((double)v104 * 0.125 + v93 - 2.0 + 5.965784072875977) * 0.6931470036506653 * 1.442695040888963407;
        _ST6 = v72;
        __asm { frndint }
        v75 = (__int64)(__FSCALE__(__F2XM1__(v72 - _ST6) + 1.0, _ST6) / a2);
        if ( (int)v75 >= 0 )
        {
          HIDWORD(v75) = v99[j];
          if ( (int)v75 < a3 )
            *(_DWORD *)(*(_DWORD *)(HIDWORD(v75) + 4 * v34) + n) = *(_DWORD *)&v95[4 * v75];
          else
            *(_DWORD *)(*(_DWORD *)(HIDWORD(v75) + 4 * v34) + n) = -998653952;
        }
        else
        {
          *(_DWORD *)(*(_DWORD *)(v99[j] + 4 * v34) + n) = -998653952;
        }
        ++v104;
      }
      v76 = 0;
      v77 = *(float **)(v99[j] + 4 * v34);
      v78 = v77 + 2;
      do
      {
        if ( *v78 > -200.0 )
          break;
        ++v76;
        ++v78;
      }
      while ( v76 < 16 );
      v79 = v99;
      v80 = (double)v76;
      v81 = 55;
      *v77 = v80;
      v82 = *(_DWORD *)(v79[j] + 4 * v34);
      v83 = (float *)(v82 + 228);
      do
      {
        if ( *v83 > -200.0 )
          break;
        --v81;
        --v83;
      }
      while ( v81 > 17 );
      v94 = ++v34;
      *(float *)(v82 + 4) = (float)v81;
    }
    while ( v34 < 8 );
  }
  return v99;
}

// ===== sub_4C9790 @ 0x004C9790..0x004C97B9 =====
void __cdecl sub_4C9790(float *a1, int a2)
{
  float *v2; // ecx
  int v3; // edx
  int v4; // esi

  v2 = a1;
  v3 = a2 - (_DWORD)a1;
  v4 = 56;
  do
  {
    if ( *(float *)((char *)v2 + v3) < (double)*v2 )
      *v2 = *(float *)((char *)v2 + v3);
    ++v2;
    --v4;
  }
  while ( v4 );
}

// ===== sub_4C97C0 @ 0x004C97C0..0x004C97E9 =====
void __cdecl sub_4C97C0(float *a1, int a2)
{
  float *v2; // ecx
  int v3; // edx
  int v4; // esi

  v2 = a1;
  v3 = a2 - (_DWORD)a1;
  v4 = 56;
  do
  {
    if ( *(float *)((char *)v2 + v3) > (double)*v2 )
      *v2 = *(float *)((char *)v2 + v3);
    ++v2;
    --v4;
  }
  while ( v4 );
}

// ===== sub_4C97F0 @ 0x004C97F0..0x004C9809 =====
float *__cdecl sub_4C97F0(float *a1, float a2)
{
  float *result; // eax
  int v3; // ecx
  double v4; // st7

  result = a1;
  v3 = 56;
  do
  {
    v4 = a2 + *result++;
    --v3;
    *(result - 1) = v4;
  }
  while ( v3 );
  return result;
}

// ===== sub_4C9810 @ 0x004C9810..0x004C98D0 =====
int __cdecl sub_4C9810(void **a1)
{
  int i; // ebx
  int j; // esi
  int k; // esi
  int result; // eax

  if ( a1 )
  {
    if ( a1[4] )
      free(a1[4]);
    if ( a1[5] )
      free(a1[5]);
    if ( a1[6] )
      free(a1[6]);
    if ( a1[2] )
    {
      for ( i = 0; i < 68; i += 4 )
      {
        for ( j = 0; j < 32; j += 4 )
          free(*(void **)(*(_DWORD *)((char *)a1[2] + i) + j));
        free(*(void **)((char *)a1[2] + i));
      }
      free(a1[2]);
    }
    if ( a1[3] )
    {
      for ( k = 0; k < 12; k += 4 )
        free(*(void **)((char *)a1[3] + k));
      free(a1[3]);
    }
    result = 0;
    memset(a1, 0, 0x30u);
  }
  return result;
}

// ===== sub_4C98D0 @ 0x004C98D0..0x004C9930 =====
int *__cdecl sub_4C98D0(int *a1, int *a2, int a3, int a4, int a5)
{
  int *result; // eax
  int v6; // ecx
  int v7; // edi
  bool v8; // cc
  int v9; // ebx
  int v10; // ecx
  int v11; // [esp+10h] [ebp+4h]

  result = a1;
  v6 = *a1;
  v7 = a5;
  v8 = a5 <= *a1;
  v11 = *a1;
  if ( !v8 )
    v7 = v6;
  v9 = 0;
  if ( v7 > 0 )
  {
    result = a2;
    v9 = v7;
    do
    {
      v10 = *(int *)((char *)result++ + a3 - (_DWORD)a2);
      --v7;
      *(float *)((char *)result + a4 - (_DWORD)a2 - 4) = flt_502958[v10] * *((float *)result - 1);
    }
    while ( v7 );
    v6 = v11;
  }
  if ( v9 < v6 )
  {
    result = 0;
    memset((void *)(a4 + 4 * v9), 0, 4 * (v6 - v9));
  }
  return result;
}

// ===== sub_4C9930 @ 0x004C9930..0x004C9A15 =====
int __cdecl sub_4C9930(int *a1, int a2, int a3)
{
  int v3; // esi
  int v4; // eax
  void *v5; // esp
  int v6; // edi
  int v7; // ecx
  float *v8; // eax
  double v9; // st7
  __int64 v10; // rax
  float *v11; // eax
  int v12; // edx
  double v13; // st7
  _BYTE *v14; // ebx
  _BYTE v16[12]; // [esp+0h] [ebp-10h] BYREF
  _BYTE *v17; // [esp+Ch] [ebp-4h]
  int v18; // [esp+20h] [ebp+10h]

  v3 = *a1;
  v4 = 4 * *a1 + 3;
  LOBYTE(v4) = v4 & 0xFC;
  v5 = alloca(v4);
  v6 = a3;
  sub_4C9A20(*a1, a1[6], a2, a3, 140.0, -1);
  if ( v3 > 0 )
  {
    v7 = a2 - a3;
    v8 = (float *)a3;
    v17 = &v16[-a3];
    v18 = v3;
    do
    {
      v9 = *(float *)((char *)v8 + v7) - *v8;
      *(float *)((char *)++v8 + (_DWORD)v17 - 4) = v9;
      --v18;
    }
    while ( v18 );
  }
  LODWORD(v10) = sub_4C9A20(v3, a1[6], (int)v16, v6, 0.0, *(_DWORD *)(a1[1] + 128));
  if ( v3 > 0 )
  {
    v11 = (float *)v16;
    v12 = v3;
    do
    {
      v13 = *(float *)((char *)v11 + a2 - (_DWORD)v16) - *v11;
      ++v11;
      --v12;
      *(v11 - 1) = v13;
    }
    while ( v12 );
    v14 = &v16[-v6];
    do
    {
      v10 = (__int64)(*(float *)v6 + 0.5);
      if ( (int)v10 >= 40 )
        LODWORD(v10) = 39;
      v6 += 4;
      --v3;
      *(float *)(v6 - 4) = *(float *)(a1[1] + 4 * v10 + 336) + *(float *)&v14[v6 - 4];
    }
    while ( v3 );
  }
  return v10;
}

// ===== sub_4C9A20 @ 0x004C9A20..0x004CA018 =====
void __cdecl sub_4C9A20(int a1, int *a2, int a3, float *a4, float a5, int a6)
{
  int v6; // eax
  void *v7; // esp
  int v8; // eax
  void *v9; // esp
  int v10; // eax
  void *v11; // esp
  int v12; // eax
  void *v13; // esp
  int v14; // eax
  void *v15; // esp
  double v16; // st7
  double v17; // st6
  double v18; // st5
  int v19; // edx
  double v20; // st4
  double v21; // st3
  float *v22; // ecx
  double v23; // st2
  float *v24; // eax
  bool v25; // zf
  double v26; // st0
  _BYTE *v27; // ecx
  _BYTE *v28; // eax
  int *v29; // edx
  double v30; // st7
  int v31; // eax
  int v32; // edx
  int v33; // eax
  int v34; // ecx
  double v35; // st6
  double v36; // st5
  double v37; // st3
  double v38; // st2
  double v39; // st6
  int *v40; // eax
  int *v41; // eax
  int v42; // eax
  double v43; // st6
  double v44; // st3
  double v45; // st2
  double v46; // st6
  int v47; // edx
  double v48; // st6
  double v49; // st7
  int v50; // ecx
  int v51; // eax
  int v52; // eax
  double v53; // st6
  double v54; // st5
  double v55; // st4
  double v56; // st3
  double v57; // st2
  double v58; // st6
  double v59; // st6
  int v60; // eax
  int v62; // eax
  int v63; // ecx
  double v64; // st6
  double v65; // st3
  double v66; // st2
  double v67; // st6
  double v68; // st6
  float *v69; // ecx
  int v70; // edx
  double v71; // st6
  double v72; // st6
  _BYTE v73[12]; // [esp+0h] [ebp-44h] BYREF
  int v74; // [esp+Ch] [ebp-38h]
  int v75; // [esp+10h] [ebp-34h]
  int v76; // [esp+14h] [ebp-30h]
  float *v77; // [esp+18h] [ebp-2Ch]
  float *v78; // [esp+1Ch] [ebp-28h]
  float *v79; // [esp+20h] [ebp-24h]
  float *v80; // [esp+24h] [ebp-20h]
  float *v81; // [esp+28h] [ebp-1Ch]
  float *v82; // [esp+2Ch] [ebp-18h]
  _BYTE *v83; // [esp+30h] [ebp-14h]
  _BYTE *v84; // [esp+34h] [ebp-10h]
  float v85; // [esp+38h] [ebp-Ch]
  float v86; // [esp+3Ch] [ebp-8h]
  float v87; // [esp+40h] [ebp-4h]
  int v88; // [esp+50h] [ebp+Ch]
  int v89; // [esp+50h] [ebp+Ch]
  int v90; // [esp+50h] [ebp+Ch]
  int v91; // [esp+50h] [ebp+Ch]
  float v92; // [esp+54h] [ebp+10h]
  float v93; // [esp+54h] [ebp+10h]
  int v94; // [esp+54h] [ebp+10h]
  int v95; // [esp+54h] [ebp+10h]
  float *v96; // [esp+60h] [ebp+1Ch]

  v6 = 4 * a1 + 7;
  LOBYTE(v6) = v6 & 0xFC;
  v7 = alloca(v6);
  v8 = 4 * a1 + 7;
  LOBYTE(v8) = (4 * a1 + 7) & 0xFC;
  v9 = alloca(v8);
  v10 = 4 * a1 + 7;
  LOBYTE(v10) = (4 * a1 + 7) & 0xFC;
  v11 = alloca(v10);
  v12 = 4 * a1 + 7;
  LOBYTE(v12) = v12 & 0xFC;
  v13 = alloca(v12);
  v84 = v73;
  v14 = 4 * a1 + 7;
  LOBYTE(v14) = v14 & 0xFC;
  v15 = alloca(v14);
  v16 = 0.0;
  v17 = 0.0;
  v18 = 0.0;
  v19 = 0;
  v20 = 0.0;
  v21 = 0.0;
  v83 = v73;
  v85 = 0.0;
  if ( a1 > 0 )
  {
    v22 = (float *)v73;
    v76 = a1;
    v77 = (float *)(a3 - (_DWORD)v73);
    v78 = 0;
    v79 = 0;
    v80 = (float *)(v84 - v73);
    v81 = 0;
    v19 = a1;
    do
    {
      v23 = a5 + *(float *)((char *)v22 + (_DWORD)v77);
      if ( v23 < 1.0 )
        v23 = 1.0;
      v24 = v78;
      *v22++ = v21;
      *(float *)((char *)v22 + (_DWORD)v24 - 4) = v20;
      *(float *)((char *)v22 + (_DWORD)v79 - 4) = v18;
      *(float *)((char *)v22 + (_DWORD)v80 - 4) = v17;
      *(float *)((char *)v22 + (_DWORD)v81 - 4) = v16;
      v21 = v21 + v23 * v23;
      v25 = v76-- == 1;
      v26 = v23 * v23 * v85;
      v92 = v26;
      v20 = v20 + v26;
      v18 = v18 + v92 * v85;
      v93 = v23 * v23 * v23;
      v17 = v17 + v93;
      v16 = v16 + v93 * v85;
      v85 = v85 + 1.0;
    }
    while ( !v25 );
  }
  *(float *)&v73[4 * v19] = v21;
  v27 = v84;
  v28 = v83;
  *(float *)&v73[4 * v19] = v20;
  v94 = 0;
  *(float *)&v73[4 * v19] = v18;
  *(float *)&v27[4 * v19] = v17;
  *(float *)&v28[4 * v19] = v16;
  v29 = a2;
  v30 = 0.0;
  v31 = *a2 >> 16;
  if ( v31 >= 0 )
  {
    v34 = (int)a2;
  }
  else
  {
    v77 = (float *)*a2;
    v76 = (int)a2;
    LOWORD(v32) = (_WORD)v77;
    v82 = (float *)((char *)a4 - (char *)a2);
    do
    {
      v33 = -4 * v31;
      v34 = (unsigned __int16)v32;
      v35 = *(float *)&v73[v33] + *(float *)&v73[4 * (unsigned __int16)v32];
      v36 = *(float *)&v73[4 * (unsigned __int16)v32] - *(float *)&v73[v33];
      v37 = *(float *)&v84[v33] + *(float *)&v84[4 * (unsigned __int16)v32];
      v38 = *(float *)&v83[4 * (unsigned __int16)v32] - *(float *)&v83[v33];
      v86 = v37 * v35 - v38 * v36;
      v87 = v38 * v35 - v37 * v36;
      v85 = v35 * v35 - v36 * v36;
      v39 = (v87 * v30 + v86) / v85;
      if ( v39 < 0.0 )
        v39 = 0.0;
      v40 = (int *)(v76 + 4);
      v76 = (int)v40;
      *(float *)((char *)v40 + (_DWORD)v82 - 4) = v39 - a5;
      v30 = v30 + 1.0;
      ++v94;
      v32 = *v40;
      v31 = *v40 >> 16;
    }
    while ( v31 < 0 );
    v29 = a2;
  }
  if ( v34 < a1 )
  {
    v41 = &v29[v94];
    v82 = (float *)((char *)a4 - (char *)v29);
    v88 = (int)v41;
    do
    {
      v42 = *v41;
      v34 = (unsigned __int16)v42;
      v42 >>= 16;
      v43 = *(float *)&v73[4 * v34] - *(float *)&v73[4 * v42];
      v44 = *(float *)&v84[4 * v34] - *(float *)&v84[4 * v42];
      v45 = *(float *)&v83[4 * v34] - *(float *)&v83[4 * v42];
      v86 = v44 * v43 - v45 * v43;
      v87 = v45 * v43 - v44 * v43;
      v85 = v43 * v43 - v43 * v43;
      v46 = (v87 * v30 + v86) / v85;
      if ( v46 < 0.0 )
        v46 = 0.0;
      v41 = (int *)(v88 + 4);
      v88 = (int)v41;
      *(float *)((char *)v82 + (_DWORD)v41 - 4) = v46 - a5;
      v30 = v30 + 1.0;
      ++v94;
    }
    while ( v34 < a1 );
  }
  if ( v94 < a1 )
  {
    v89 = (int)&a4[v94];
    v47 = a1 - v94;
    do
    {
      v48 = (v87 * v30 + v86) / v85;
      if ( v48 < 0.0 )
        v48 = 0.0;
      --v47;
      v89 += 4;
      *(float *)(v89 - 4) = v48 - a5;
      v30 = v30 + 1.0;
    }
    while ( v47 );
  }
  if ( a6 > 0 )
  {
    v95 = 0;
    v49 = 0.0;
    v74 = (a6 + 1) / 2;
    if ( v74 > 0 )
    {
      v77 = a4;
      v75 = -4 * a6;
      v50 = a6 / 2;
      v76 = a6 / 2;
      v78 = (float *)&v83[4 * (a6 / 2)];
      v79 = (float *)&v84[4 * (a6 / 2)];
      v80 = (float *)&v73[4 * (a6 / 2)];
      v81 = v80;
      v82 = v80;
      v51 = 4 * (a6 / -2);
      v90 = v51;
      while ( 1 )
      {
        v34 = v95 + v50;
        v52 = v51 - v75;
        v53 = *(float *)&v73[v52] + *v82;
        v54 = *v81 - *(float *)&v73[v52];
        v55 = *(float *)&v73[v52] + *v80;
        v56 = *(float *)&v84[v52] + *v79;
        v57 = *v78 - *(float *)&v83[v52];
        v86 = v56 * v55 - v57 * v54;
        v87 = v57 * v53 - v56 * v54;
        v85 = v55 * v53 - v54 * v54;
        v58 = (v87 * v49 + v86) / v85;
        if ( v58 > 0.0 )
        {
          v59 = v58 - a5;
          if ( v59 < *v77 )
            *v77 = v59;
        }
        v90 -= 4;
        ++v95;
        ++v82;
        ++v81;
        ++v80;
        ++v79;
        ++v78;
        v49 = v49 + 1.0;
        ++v77;
        if ( v95 >= v74 )
          break;
        v51 = v90;
        v50 = v76;
      }
    }
    v60 = a1;
    if ( v34 < a1 )
    {
      v74 = 4 * a6;
      v96 = &a4[v95];
      v62 = a6 / 2;
      v76 = v62;
      v63 = 4 * (v62 + v95);
      while ( 1 )
      {
        v91 = v95 + v62;
        v64 = *(float *)&v73[v63] - *(float *)&v73[v63 - v74];
        v65 = *(float *)&v84[v63] - *(float *)&v84[v63 - v74];
        v66 = *(float *)&v83[v63] - *(float *)&v83[v63 - v74];
        v86 = v65 * v64 - v66 * v64;
        v87 = v66 * v64 - v65 * v64;
        v85 = v64 * v64 - v64 * v64;
        v67 = (v87 * v49 + v86) / v85;
        if ( v67 > 0.0 )
        {
          v68 = v67 - a5;
          if ( v68 < *v96 )
            *v96 = v68;
        }
        v63 += 4;
        ++v95;
        v49 = v49 + 1.0;
        ++v96;
        v60 = a1;
        if ( v91 >= a1 )
          break;
        v62 = v76;
      }
    }
    if ( v95 < v60 )
    {
      v69 = &a4[v95];
      v70 = v60 - v95;
      do
      {
        v71 = (v87 * v49 + v86) / v85;
        if ( v71 > 0.0 )
        {
          v72 = v71 - a5;
          if ( v72 < *v69 )
            *v69 = v72;
        }
        v49 = v49 + 1.0;
        ++v69;
        --v70;
      }
      while ( v70 );
    }
  }
}

// ===== sub_4CA020 @ 0x004CA020..0x004CA0BF =====
int __cdecl sub_4CA020(int *a1, int a2, int a3, float a4, float a5)
{
  int v5; // edi
  int v6; // ebx
  int v7; // eax
  void *v8; // esp
  double v9; // st7
  int v10; // eax
  _DWORD *v11; // ecx
  int v12; // ecx
  int i; // eax
  _BYTE v15[12]; // [esp+0h] [ebp-Ch] BYREF

  v5 = a1[10];
  v6 = *a1;
  v7 = 4 * v5 + 3;
  LOBYTE(v7) = v7 & 0xFC;
  v8 = alloca(v7);
  v9 = a5 + *(float *)(a1[1] + 4);
  v10 = 0;
  if ( v5 > 0 )
  {
    v11 = v15;
    do
    {
      *v11 = -971228160;
      ++v10;
      ++v11;
    }
    while ( v10 < a1[10] );
  }
  v12 = a1[1];
  if ( v9 < *(float *)(v12 + 8) )
    v9 = *(float *)(v12 + 8);
  for ( i = 0; i < v6; *(float *)(a3 + 4 * i - 4) = v9 + *(float *)(a1[4] + 4 * i - 4) )
    ++i;
  sub_4CA0C0((int)a1, a1[2], a2, a3, (int)v15, a4);
  return sub_4CA270(a1, v15, a3);
}

// ===== sub_4CA0C0 @ 0x004CA0C0..0x004CA1A0 =====
void __cdecl sub_4CA0C0(int *a1, int a2, int a3, int a4, int a5, float a6)
{
  int v7; // esi
  int v8; // ebx
  bool v9; // cc
  double v10; // st7
  int v11; // edi
  double v12; // st7
  int v13; // edx
  float *v14; // ecx
  int v15; // ebx
  _DWORD *v16; // edi
  bool v17; // c0
  int v18; // edx
  float v19; // [esp+Ch] [ebp-4h]
  int v20; // [esp+14h] [ebp+4h]
  float v21; // [esp+28h] [ebp+18h]

  v7 = 0;
  v8 = *a1;
  v9 = *a1 <= 0;
  v10 = *(float *)(a1[1] + 496) - a6;
  v20 = *a1;
  v19 = v10;
  if ( !v9 )
  {
    do
    {
      v11 = a1[5];
      v12 = *(float *)(a3 + 4 * v7);
      v13 = *(_DWORD *)(v11 + 4 * v7);
      v14 = (float *)(a3 + 4 * v7);
      v21 = *v14;
      if ( v7 + 1 < v8 )
      {
        v15 = v7 + 1;
        v16 = (_DWORD *)(v11 + 4 * (v7 + 1));
        do
        {
          if ( *v16 != v13 )
            break;
          v17 = v12 < v14[1];
          ++v14;
          ++v7;
          ++v16;
          ++v15;
          if ( v17 )
            v12 = *v14;
        }
        while ( v15 < v20 );
        v21 = v12;
        v8 = v20;
      }
      if ( v12 + 6.0 > *(float *)(a4 + 4 * v7) )
      {
        v18 = v13 >> a1[8];
        if ( v18 < 17 )
        {
          if ( v18 < 0 )
            v18 = 0;
        }
        else
        {
          v18 = 16;
        }
        sub_4CA1A0(a5, *(_DWORD *)(a2 + 4 * v18), v21, *(_DWORD *)(a1[5] + 4 * v7) - a1[7], a1[10], a1[9], v19);
      }
      ++v7;
    }
    while ( v7 < v8 );
  }
}

// ===== sub_4CA1A0 @ 0x004CA1A0..0x004CA262 =====
int __cdecl sub_4CA1A0(int a1, int a2, float a3, int a4, int a5, int a6, float a7)
{
  __int64 v7; // rax
  int v8; // eax
  float *v9; // esi
  int v10; // edi
  __int64 v11; // rax
  int v12; // ecx
  float *v13; // esi
  double v14; // st7
  int v16; // [esp+1Ch] [ebp+1Ch]

  v7 = (__int64)((a3 + a7 - 30.0) * 0.1000000014901161);
  v8 = (int)v7 <= 0 ? 0 : v7;
  if ( v8 >= 7 )
    v8 = 7;
  v9 = *(float **)(a2 + 4 * v8);
  v16 = (__int64)v9[1];
  v10 = (__int64)((*v9 - 16.0) * (double)a6 + (double)a4 - (double)(a6 >> 1));
  v11 = (__int64)*v9;
  v12 = v11;
  if ( (int)v11 < v16 )
  {
    v13 = &v9[v11 + 2];
    HIDWORD(v11) = a1 + 4 * v10;
    do
    {
      if ( v10 > 0 )
      {
        v14 = a3 + *v13;
        if ( v14 > *(float *)HIDWORD(v11) )
          *(float *)HIDWORD(v11) = v14;
      }
      LODWORD(v11) = a5;
      v10 += a6;
      HIDWORD(v11) += 4 * a6;
      if ( v10 >= a5 )
        break;
      LODWORD(v11) = v16;
      ++v12;
      ++v13;
    }
    while ( v12 < v16 );
  }
  return v11;
}

// ===== sub_4CA270 @ 0x004CA270..0x004CA3A2 =====
int __cdecl sub_4CA270(int *a1, int a2, int a3)
{
  int v4; // esi
  int v5; // ebx
  int v6; // edx
  float *v7; // ebx
  int v8; // ecx
  int v9; // ebp
  int i; // ebp
  double v11; // st7
  int j; // ebx
  int result; // eax
  float *v14; // ecx
  float v15; // [esp+10h] [ebp+4h]
  float v16; // [esp+14h] [ebp+8h]

  v4 = 0;
  v5 = a1[9];
  sub_4CA3B0(a2, v5, a1[10]);
  v6 = *(_DWORD *)a1[5] - (v5 >> 1) - a1[7];
  if ( *a1 > 1 )
  {
    do
    {
      v7 = (float *)(a2 + 4 * v6);
      v15 = *v7;
      v8 = ((*(_DWORD *)(a1[5] + 4 * v4) + *(_DWORD *)(a1[5] + 4 * v4 + 4)) >> 1) - a1[7];
      v9 = a1[1];
      if ( *v7 > (double)*(float *)(v9 + 32) )
        v15 = *(float *)(v9 + 32);
      for ( i = v6 + 1; i <= v8; ++i )
      {
        v11 = v7[1];
        ++v7;
        ++v6;
        if ( v11 > -9999.0 && *v7 < (double)v15 || v15 == -9999.0 )
          v15 = *v7;
      }
      for ( j = a1[7] + v6; v4 < *a1; ++v4 )
      {
        if ( *(_DWORD *)(a1[5] + 4 * v4) > j )
          break;
        if ( *(float *)(a3 + 4 * v4) < (double)v15 )
          *(float *)(a3 + 4 * v4) = v15;
      }
    }
    while ( v4 + 1 < *a1 );
  }
  result = *a1;
  v16 = *(float *)(a2 + 4 * a1[10] - 4);
  if ( v4 < *a1 )
  {
    v14 = (float *)(a3 + 4 * v4);
    do
    {
      if ( *v14 < (double)v16 )
        *v14 = v16;
      result = *a1;
      ++v4;
      ++v14;
    }
    while ( v4 < *a1 );
  }
  return result;
}

// ===== sub_4CA3B0 @ 0x004CA3B0..0x004CA502 =====
int __cdecl sub_4CA3B0(float *a1, int a2, int a3)
{
  int v3; // eax
  void *v4; // esp
  int result; // eax
  void *v6; // esp
  int v7; // edx
  float *v8; // ebx
  int v9; // ecx
  float *v10; // eax
  float v11; // eax
  float *v12; // esi
  float *v13; // eax
  float *v14; // esi
  float *v15; // edx
  int *v16; // esi
  char *v17; // eax
  float *v18; // edx
  int v19; // [esp+0h] [ebp-20h] BYREF
  int v20; // [esp+4h] [ebp-1Ch] BYREF
  char *i; // [esp+Ch] [ebp-14h]
  int v22; // [esp+10h] [ebp-10h]
  float *v23; // [esp+14h] [ebp-Ch]
  int *v24; // [esp+18h] [ebp-8h]
  float *v25; // [esp+1Ch] [ebp-4h]

  v3 = 4 * a3 + 3;
  LOBYTE(v3) = v3 & 0xFC;
  v4 = alloca(v3);
  result = 4 * a3 + 3;
  LOBYTE(result) = (4 * a3 + 3) & 0xFC;
  v6 = alloca(result);
  v7 = 0;
  v8 = (float *)&v19;
  v9 = 0;
  v23 = (float *)&v19;
  v22 = 0;
  if ( a3 > 0 )
  {
    v25 = a1;
    do
    {
      if ( v9 >= 2 )
      {
        v12 = &v8[v9 - 1];
        v24 = &v19 + v9 - 2;
        i = (char *)((char *)&v19 - (char *)v8);
        while ( *v25 >= (double)*v12 )
        {
          if ( v7 >= a2 + *(_DWORD *)((char *)v12 + (_DWORD)i)
            || v9 <= 1
            || *v12 > (double)*(v12 - 1)
            || v7 >= a2 + *v24 )
          {
            v13 = v25;
            v8 = v23;
            *(&v19 + v9) = v7;
            v11 = *v13;
            goto LABEL_13;
          }
          v8 = v23;
          --v9;
          --v12;
          --v24;
        }
      }
      v10 = v25;
      *(&v19 + v9) = v7;
      v11 = *v10;
LABEL_13:
      v14 = v25;
      v8[v9] = v11;
      result = a3;
      ++v9;
      ++v7;
      v25 = v14 + 1;
    }
    while ( v7 < a3 );
  }
  v15 = 0;
  v23 = 0;
  if ( v9 > 0 )
  {
    v16 = &v20;
    v17 = (char *)((char *)v8 - (char *)&v19);
    for ( i = (char *)((char *)v8 - (char *)&v19); ; v17 = i )
    {
      if ( (int)v15 >= v9 - 1 || *(float *)((char *)v16 + (_DWORD)v17) <= (double)*v8 )
        result = *(v16 - 1) + a2 + 1;
      else
        result = *v16;
      if ( result > a3 )
        result = a3;
      if ( v22 < result )
      {
        result -= v22;
        v18 = &a1[v22];
        v22 += result;
        do
        {
          *v18++ = *v8;
          --result;
        }
        while ( result );
        v15 = v23;
      }
      v15 = (float *)((char *)v15 + 1);
      ++v16;
      ++v8;
      v23 = v15;
      if ( (int)v15 >= v9 )
        break;
    }
  }
  return result;
}

// ===== sub_4CA510 @ 0x004CA510..0x004CA59A =====
int __cdecl sub_4CA510(int *a1, int a2, float *a3, int a4, int a5)
{
  unsigned __int64 v6; // rax
  float *v7; // ecx
  unsigned int v8; // esi
  double v9; // st7
  float v11; // [esp+4h] [ebp-8h]
  int v12; // [esp+8h] [ebp-4h]
  float v13; // [esp+10h] [ebp+4h]

  v12 = *a1;
  v6 = (unsigned int)*a1;
  v11 = *(float *)(a1[1] + 4 * a4 + 12);
  if ( *a1 > 0 )
  {
    v7 = a3;
    do
    {
      v8 = a1[1];
      v9 = *(float *)(*(_DWORD *)(a1[3] + 4 * a4) + 4 * HIDWORD(v6)) + *(float *)((char *)v7 + a2 - (_DWORD)a3);
      if ( v9 > *(float *)(v8 + 108) )
        v9 = *(float *)(v8 + 108);
      v13 = v11 + *v7;
      if ( v9 <= v13 )
        v9 = v13;
      *(float *)((char *)v7 + a5 - (_DWORD)a3) = v9;
      v6 = __PAIR64__(HIDWORD(v6), v12) + 0x100000000LL;
      ++v7;
    }
    while ( SHIDWORD(v6) < v12 );
  }
  return v6;
}

// ===== sub_4CA5A0 @ 0x004CA5A0..0x004CA6EA =====
int __usercall sub_4CA5A0@<eax>(double a1@<st0>, _DWORD *a2, int a3, int a4, int a5, int a6)
{
  int result; // eax
  _DWORD *v7; // ebp
  int *v8; // ebx
  float *v9; // edi
  int v10; // esi
  int v11; // eax
  float *v12; // edi
  bool v13; // cc
  int v14; // [esp+8h] [ebp-18h]
  int v15; // [esp+Ch] [ebp-14h]
  int v16; // [esp+10h] [ebp-10h]
  _DWORD *v17; // [esp+14h] [ebp-Ch]
  int i; // [esp+18h] [ebp-8h]
  int v19; // [esp+1Ch] [ebp-4h]
  int v20; // [esp+28h] [ebp+8h]
  float *v21; // [esp+2Ch] [ebp+Ch]

  v14 = *(_DWORD *)a4;
  result = sub_4C7BE0(a2, 4 * *(_DWORD *)(a5 + 1156));
  v19 = result;
  v16 = 0;
  v20 = *(_DWORD *)(a3 + 60 * **(_DWORD **)(a4 + 4) + 160);
  if ( *(int *)(a5 + 1156) > 0 )
  {
    v7 = (_DWORD *)(a5 + 2184);
    v8 = (int *)result;
    v17 = (_DWORD *)(a5 + 2184);
    do
    {
      v9 = *(float **)(a6 + 4 * *(v7 - 256));
      v15 = *(_DWORD *)(a6 + 4 * *v7);
      v21 = v9;
      *v8 = sub_4C7BE0(a2, 4 * v14);
      v10 = 0;
      if ( v20 > 0 )
      {
        v11 = v15 - (_DWORD)v9;
        for ( i = v15 - (_DWORD)v9; ; v11 = i )
        {
          sub_4CA6F0(*v9, *(float *)((char *)v9 + v11));
          ++v9;
          *(float *)(*v8 + 4 * v10++) = a1;
          if ( v10 >= v20 )
            break;
        }
        v9 = v21;
      }
      if ( v10 < v14 )
      {
        v12 = &v9[v10];
        do
        {
          sub_4CA7C0(*v12, *(float *)((char *)v12 + v15 - (_DWORD)v21));
          ++v12;
          *(float *)(*v8 + 4 * v10++) = a1;
        }
        while ( v10 < v14 );
        v7 = v17;
      }
      ++v7;
      ++v8;
      v13 = ++v16 < *(_DWORD *)(a5 + 1156);
      v17 = v7;
    }
    while ( v13 );
    return v19;
  }
  return result;
}

// ===== sub_4CA6F0 @ 0x004CA6F0..0x004CA7B9 =====
double __cdecl sub_4CA6F0(float a1, float a2)
{
  unsigned __int8 v3; // c0
  unsigned __int8 v4; // c3
  double v6; // st7

  if ( v3 | v4 )
  {
    v6 = a1;
    if ( a2 >= 0.0 )
    {
      if ( -v6 <= a2 )
        return sqrt(a2 * a2 - a1 * a1);
      else
        return -sqrt(a1 * a1 - a2 * a2);
    }
    else
    {
      return -sqrt(v6 * a1 + a2 * a2);
    }
  }
  else if ( a2 <= 0.0 )
  {
    if ( -a2 >= a1 )
      return -sqrt(a2 * a2 - a1 * a1);
    else
      return sqrt(a1 * a1 - a2 * a2);
  }
  else
  {
    return sqrt(a1 * a1 + a2 * a2);
  }
}

// ===== sub_4CA7C0 @ 0x004CA7C0..0x004CA84C =====
double __cdecl sub_4CA7C0(float a1, float a2)
{
  unsigned __int8 v3; // c0
  unsigned __int8 v4; // c3
  long double v6; // st7
  char v7; // c0
  double v9; // st7

  if ( v3 | v4 )
  {
    v9 = a1;
    if ( a2 >= 0.0 )
    {
      if ( -v9 <= a2 )
        goto LABEL_9;
      v9 = a1;
    }
    return -sqrt(v9 * a1 + a2 * a2);
  }
  if ( a2 <= 0.0 )
  {
    v6 = a1 * a1 + a2 * a2;
    if ( !v7 )
      return -sqrt(v6);
    return sqrt(v6);
  }
LABEL_9:
  v6 = a1 * a1 + a2 * a2;
  return sqrt(v6);
}

// ===== sub_4CA850 @ 0x004CA850..0x004CA9AF =====
int *__cdecl sub_4CA850(_DWORD *a1, int *a2, int a3, int a4)
{
  int v4; // edx
  int v5; // eax
  int v6; // ecx
  signed int v7; // edi
  int v8; // eax
  void *v9; // esp
  int v10; // eax
  _BYTE *v11; // ebx
  int *v12; // esi
  int v13; // eax
  int v14; // ecx
  int v15; // eax
  _BYTE *v16; // ecx
  signed int v17; // edx
  int v18; // ebx
  int v19; // ecx
  int *v20; // eax
  int v21; // edx
  bool v22; // cc
  int v23; // edx
  _BYTE v25[12]; // [esp+0h] [ebp-24h] BYREF
  int v26; // [esp+Ch] [ebp-18h]
  int v27; // [esp+10h] [ebp-14h]
  _BYTE *v28; // [esp+14h] [ebp-10h]
  signed int v29; // [esp+18h] [ebp-Ch]
  int *v30; // [esp+1Ch] [ebp-8h]
  int v31; // [esp+20h] [ebp-4h]
  int v32; // [esp+30h] [ebp+Ch]
  int v33; // [esp+38h] [ebp+14h]

  if ( !*(_DWORD *)(a2[1] + 504) )
    return 0;
  v4 = *(_DWORD *)(a3 + 1156);
  v31 = *a2;
  v5 = sub_4C7BE0(a1, 4 * v4);
  v6 = a2[1];
  v30 = (int *)v5;
  v7 = *(_DWORD *)(v6 + 512);
  v8 = 4 * v7 + 3;
  LOBYTE(v8) = v8 & 0xFC;
  v9 = alloca(v8);
  v10 = *(_DWORD *)(a3 + 1156);
  v11 = v25;
  v28 = v25;
  v26 = 0;
  if ( v10 > 0 )
  {
    v12 = v30;
    v33 = a4 - (_DWORD)v30;
    do
    {
      v13 = sub_4C7BE0(a1, 4 * v31);
      v14 = v31;
      *v12 = v13;
      v27 = 0;
      if ( v14 > 0 )
      {
        v32 = 0;
        do
        {
          if ( v7 > 0 )
          {
            v15 = v32;
            v16 = v11;
            v17 = v7;
            do
            {
              v16 += 4;
              v18 = v15 + *(int *)((char *)v12 + v33);
              v15 += 4;
              *((_DWORD *)v16 - 1) = v18;
              --v17;
            }
            while ( v17 );
            v11 = v28;
          }
          qsort(v11, v7, 4u, sub_4CA9B0);
          if ( v7 > 0 )
          {
            v19 = v32;
            v20 = (int *)v11;
            v29 = v7;
            do
            {
              v21 = *v20++;
              v19 += 4;
              *(_DWORD *)(*v12 + v19 - 4) = (v21 - *(int *)((char *)v12 + v33)) >> 2;
              --v29;
            }
            while ( v29 );
            v11 = v28;
          }
          v22 = v7 + v27 < v31;
          v27 += v7;
          v32 += 4 * v7;
        }
        while ( v22 );
      }
      ++v12;
      v23 = *(_DWORD *)(a3 + 1156);
      ++v26;
    }
    while ( v26 < v23 );
  }
  return v30;
}

// ===== sub_4CA9B0 @ 0x004CA9B0..0x004CA9D7 =====
int __cdecl sub_4CA9B0(float **a1, float **a2)
{
  if ( fabs(**a2) >= fabs(**a1) )
    return 1;
  else
    return -1;
}

// ===== sub_4CA9E0 @ 0x004CA9E0..0x004CAAA3 =====
int __cdecl sub_4CA9E0(int *a1, int a2, int a3)
{
  int v3; // edi
  signed int v4; // esi
  int v5; // eax
  void *v6; // esp
  int v7; // edi
  int result; // eax
  _BYTE *v9; // ebx
  _DWORD *v10; // ecx
  signed int v11; // edx
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  int v15; // edx
  _BYTE v16[12]; // [esp+0h] [ebp-18h] BYREF
  int v17; // [esp+Ch] [ebp-Ch]
  _BYTE *v18; // [esp+10h] [ebp-8h]
  signed int v19; // [esp+14h] [ebp-4h]
  int v20; // [esp+20h] [ebp+8h]

  v3 = a1[1];
  v20 = *a1;
  v4 = *(_DWORD *)(v3 + 512);
  v5 = 4 * v4 + 3;
  LOBYTE(v5) = v5 & 0xFC;
  v6 = alloca(v5);
  v7 = *(_DWORD *)(v3 + 508);
  result = v20;
  v9 = v16;
  v18 = v16;
  v17 = v7;
  if ( v7 < v20 )
  {
    while ( 1 )
    {
      if ( v7 + v4 > result )
        v4 = result - v7;
      if ( v4 > 0 )
      {
        v10 = v9;
        v11 = v4;
        v12 = a2 + 4 * v7;
        do
        {
          *v10 = v12;
          v12 += 4;
          ++v10;
          --v11;
        }
        while ( v11 );
      }
      qsort(v9, v4, 4u, (_CoreCrtNonSecureSearchSortCompareFunction)sub_4CA9B0);
      if ( v4 > 0 )
      {
        v13 = 0;
        v19 = v4;
        do
        {
          v14 = v13 - 4 * v17;
          v15 = *(_DWORD *)&v9[v13];
          v13 += 4;
          *(_DWORD *)(v14 + 4 * v7 + a3) = (v15 - a2) >> 2;
          v9 = v18;
          --v19;
        }
        while ( v19 );
      }
      result = v20;
      v7 += v4;
      if ( v7 >= v20 )
        break;
      result = v20;
    }
  }
  return result;
}

// ===== sub_4CAAB0 @ 0x004CAAB0..0x004CACFF =====
void __cdecl sub_4CAAB0(int *a1, int a2, float *a3, int a4)
{
  double *v4; // ecx
  int v5; // edi
  int v6; // edx
  int v7; // eax
  int v8; // ebx
  float *v9; // esi
  int v10; // ecx
  int v11; // esi
  int v12; // edi
  double v13; // st7
  float *v14; // ecx
  int v15; // eax
  double v16; // st5
  int v17; // ebx
  int v18; // esi
  bool v19; // cc
  int v20; // edi
  float *v21; // esi
  int v22; // ecx
  int v23; // edx
  int v24; // eax
  int v25; // [esp+18h] [ebp-28h]
  float v26; // [esp+1Ch] [ebp-24h]
  int v27; // [esp+20h] [ebp-20h]
  int v28; // [esp+24h] [ebp-1Ch]
  int v29; // [esp+28h] [ebp-18h]
  float v30; // [esp+2Ch] [ebp-14h]
  int v31; // [esp+30h] [ebp-10h]
  int v32; // [esp+30h] [ebp-10h]
  int v33; // [esp+34h] [ebp-Ch]
  int v34; // [esp+38h] [ebp-8h]
  double *v35; // [esp+3Ch] [ebp-4h]

  v4 = (double *)a1[1];
  v5 = *a1;
  v6 = 0;
  v34 = *a1;
  v7 = *((_DWORD *)v4 + 127);
  v8 = *((_DWORD *)v4 + 128);
  v28 = 0;
  v35 = v4;
  v25 = v8;
  v29 = v7;
  if ( v7 > *a1 )
  {
    v29 = *a1;
    v7 = *a1;
  }
  if ( *((_DWORD *)v4 + 125) )
  {
    if ( v7 > 0 )
    {
      v9 = a3;
      v10 = a2 - (_DWORD)a3;
      v31 = v7;
      v28 = v7;
      while ( 1 )
      {
        *v9 = floor(*(float *)((char *)v9 + v10) + 0.5);
        ++v9;
        if ( !--v31 )
          break;
        v10 = a2 - (_DWORD)a3;
      }
    }
    v6 = v28;
    if ( v8 + v28 <= v5 )
    {
      v11 = v8 + v28;
      v12 = 4 * v28;
      v33 = v8 + v28;
      while ( 1 )
      {
        v13 = 0.0;
        v26 = 0.0;
        if ( v6 < v11 )
        {
          v14 = (float *)(v12 + a2);
          v15 = v11 - v6;
          do
          {
            v16 = *v14 * *v14;
            ++v14;
            --v15;
            v13 = v13 + v16;
          }
          while ( v15 );
          v26 = v13;
        }
        v27 = 0;
        if ( v8 <= 0 )
          goto LABEL_22;
        v32 = 0;
        v17 = 4 * v29;
        while ( 1 )
        {
          v18 = *(_DWORD *)(v12 + v32 - v17 + a4);
          v30 = *(float *)(a2 + 4 * v18);
          if ( v30 * v30 < 0.25 )
            break;
          a3[v18] = floor(*(float *)(a2 + 4 * v18) + 0.5);
          v13 = v26 - *(float *)(a2 + 4 * v18) * *(float *)(a2 + 4 * v18);
LABEL_20:
          v26 = v13;
          v19 = ++v27 < v25;
          v32 += 4;
          if ( !v19 )
            goto LABEL_21;
        }
        if ( v13 >= v35[65] )
          break;
        if ( v27 < v25 )
        {
          v22 = 4 * v27;
          v23 = v25 - v27;
          do
          {
            v24 = v22 - v17;
            v22 += 4;
            --v23;
            a3[*(_DWORD *)(v12 + v24 + a4)] = 0.0;
          }
          while ( v23 );
        }
LABEL_21:
        v8 = v25;
LABEL_22:
        v11 = v8 + v33;
        v12 += 4 * v8;
        v6 = v8 + v28;
        v19 = v8 + v33 <= v34;
        v28 += v8;
        v33 += v8;
        if ( !v19 )
        {
          v5 = v34;
          goto LABEL_24;
        }
      }
      a3[v18] = sub_4CAD00(LODWORD(v30));
      v13 = v26 - 1.0;
      goto LABEL_20;
    }
  }
LABEL_24:
  if ( v6 < v5 )
  {
    v20 = v5 - v6;
    v21 = &a3[v6];
    do
    {
      *v21 = floor(*(float *)((char *)v21 + a2 - (_DWORD)a3) + 0.5);
      ++v21;
      --v20;
    }
    while ( v20 );
  }
}

// ===== sub_4CAD00 @ 0x004CAD00..0x004CAD17 =====
double __cdecl sub_4CAD00(int a1)
{
  return COERCE_FLOAT(a1 & 0x80000000 | 0x3F800000);
}

// ===== sub_4CAD20 @ 0x004CAD20..0x004CB103 =====
_DWORD *__cdecl sub_4CAD20(int a1, int a2, int *a3, _DWORD *a4, int a5, int a6, _DWORD *a7, int a8, int a9, int a10)
{
  int v10; // esi
  _DWORD *result; // eax
  int v12; // ecx
  int v13; // eax
  int v14; // edi
  _DWORD *v15; // eax
  int v16; // edx
  int v17; // ecx
  float *v18; // esi
  int v19; // edx
  int v20; // ebx
  int v21; // ecx
  bool v22; // zf
  int v23; // ecx
  int v24; // ebx
  bool v25; // cc
  int v26; // [esp+18h] [ebp-68h]
  float v27; // [esp+1Ch] [ebp-64h]
  _DWORD *v28; // [esp+20h] [ebp-60h]
  _DWORD *v29; // [esp+24h] [ebp-5Ch]
  int v30; // [esp+28h] [ebp-58h]
  int v31; // [esp+2Ch] [ebp-54h]
  int v32; // [esp+2Ch] [ebp-54h]
  int v33; // [esp+30h] [ebp-50h]
  int v34; // [esp+34h] [ebp-4Ch]
  float v35; // [esp+38h] [ebp-48h]
  float v36; // [esp+3Ch] [ebp-44h]
  int v37; // [esp+40h] [ebp-40h]
  int v38; // [esp+48h] [ebp-38h]
  int v39; // [esp+4Ch] [ebp-34h]
  int v40; // [esp+4Ch] [ebp-34h]
  int v41; // [esp+50h] [ebp-30h]
  int v42; // [esp+54h] [ebp-2Ch]
  int v43; // [esp+58h] [ebp-28h]
  int v44; // [esp+5Ch] [ebp-24h]
  int v45; // [esp+60h] [ebp-20h]
  int v46; // [esp+64h] [ebp-1Ch]
  int v47; // [esp+68h] [ebp-18h]
  int v48; // [esp+6Ch] [ebp-14h]
  int v49; // [esp+70h] [ebp-10h]
  int v50; // [esp+74h] [ebp-Ch]

  v10 = *a3;
  result = a4;
  v44 = *a3;
  v43 = 0;
  if ( (int)a4[289] > 0 )
  {
    result = a4 + 546;
    v29 = a7;
    v28 = a4 + 546;
    do
    {
      v12 = 4 * *(result - 256);
      if ( *(_DWORD *)(v12 + a9) || *(_DWORD *)(a9 + 4 * *result) )
      {
        v13 = 4 * *result;
        v14 = *(_DWORD *)(v13 + a5);
        v47 = *(_DWORD *)(v12 + a5);
        v38 = v14;
        v48 = v14 + 4 * v44;
        v33 = v47 + 4 * v44;
        v46 = *(_DWORD *)(v12 + a8);
        v45 = *(_DWORD *)(v13 + a8);
        v36 = dbl_502910[*(_DWORD *)(a2 + 4 * a1 + 252)];
        v35 = dbl_502910[*(_DWORD *)(a2 + 4 * a1 + 312)];
        v15 = (_DWORD *)a3[1];
        if ( v15[126] )
          v10 = v15[128];
        v26 = v10;
        v41 = *(_DWORD *)(a2 + 4 * (a1 + 12 * *v15 + 3 * *v15) + 132);
        *(_DWORD *)(v12 + a9) = 1;
        *(_DWORD *)(a9 + 4 * *v28) = 1;
        v16 = 0;
        v10 = *a3;
        v42 = 0;
        if ( *a3 > 0 )
        {
          v17 = 0;
          v34 = 0;
          do
          {
            v27 = 0.0;
            if ( v26 > 0 )
            {
              v30 = v17;
              v18 = (float *)(v17 + v14);
              v31 = v16;
              v50 = v46 - v14;
              v19 = v47 - v14;
              v20 = v33 - v14;
              v21 = v48 - v14;
              v49 = v47 - v14;
              v37 = v48 - v14;
              v39 = v26;
              while ( 1 )
              {
                if ( v31 >= a10 )
                {
                  *(float *)((char *)v18 + v20) = 0.0;
                  *(float *)((char *)v18 + v21) = 0.0;
                }
                else if ( (v31 < v41 || v35 <= fabs(*(float *)((char *)v18 + v19)) || v35 <= fabs(*v18))
                       && (v36 <= fabs(*(float *)((char *)v18 + v19)) || v36 <= fabs(*v18)) )
                {
                  sub_4CB110(*(float *)((char *)v18 + v19), *v18, (int)v18 + v20, (int)v18 + v21);
                }
                else
                {
                  sub_4CB1E0(
                    *(float *)(*(_DWORD *)((char *)v29 + a6 - (_DWORD)a7) + v30),
                    *(_DWORD *)((char *)v18 + v50),
                    *(_DWORD *)((char *)v18 + v50 + v45 - v46),
                    (int)v18 + v20,
                    (int)v18 + v37);
                  if ( floor(*(float *)((char *)v18 + v20) + 0.5) == 0.0 )
                    v27 = *(float *)((char *)v18 + v20) * *(float *)((char *)v18 + v20) + v27;
                  v14 = v38;
                }
                ++v18;
                v30 += 4;
                v22 = v39 == 1;
                ++v31;
                --v39;
                if ( v22 )
                  break;
                v21 = v37;
                v19 = v49;
              }
            }
            if ( *(_DWORD *)(a3[1] + 504) )
            {
              v23 = v26;
              v40 = 0;
              if ( v26 > 0 )
              {
                v32 = v34;
                do
                {
                  if ( v27 < *(double *)(a3[1] + 520) )
                    break;
                  v24 = *(_DWORD *)(v32 + *v29);
                  if ( v24 < a10 && v24 >= v41 && floor(*(float *)(v33 + 4 * v24) + 0.5) == 0.0 )
                  {
                    *(float *)(v33 + 4 * v24) = sub_4CAD00(*(_DWORD *)(v33 + 4 * v24));
                    v27 = v27 - 1.0;
                  }
                  v23 = v26;
                  v25 = ++v40 < v26;
                  v32 += 4;
                }
                while ( v25 );
              }
            }
            else
            {
              v23 = v26;
            }
            v16 = v23 + v42;
            v17 = 4 * v23 + v34;
            v42 = v16;
            v34 = v17;
            v10 = *a3;
          }
          while ( v16 < *a3 );
        }
        result = v28;
      }
      ++result;
      v25 = ++v43 < a4[289];
      v28 = result;
      ++v29;
    }
    while ( v25 );
  }
  return result;
}

// ===== sub_4CB110 @ 0x004CB110..0x004CB1DE =====
void __cdecl sub_4CB110(float a1, float a2, float *a3, float *a4)
{
  long double v4; // st7
  long double v5; // st6
  int v6; // esi
  double v7; // st7
  double v8; // st7
  double v9; // st6
  long double v10; // st7

  v4 = fabs(*a3);
  v5 = fabs(*a4);
  v6 = (v4 > v5) - (v4 < v5);
  if ( !v6 )
    v6 = 2 * (fabs(a2) < fabs(a1)) - 1;
  if ( v6 == 1 )
  {
    if ( *a3 <= 0.0 )
      v7 = *a4 - *a3;
    else
      v7 = *a3 - *a4;
    *a4 = v7;
  }
  else
  {
    v8 = *a4;
    if ( v8 <= 0.0 )
      v9 = *a4 - *a3;
    else
      v9 = *a3 - *a4;
    *a4 = v9;
    *a3 = v8;
  }
  v10 = fabs(*a3);
  if ( v10 * 1.999899983406067 < *a4 )
  {
    *a4 = v10 * -2.0;
    *a3 = -*a3;
  }
}

// ===== sub_4CB1E0 @ 0x004CB1E0..0x004CB245 =====
_DWORD *__cdecl sub_4CB1E0(float a1, int a2, int a3, float *a4, _DWORD *a5)
{
  unsigned int v5; // ebx
  _DWORD *result; // eax

  v5 = abs32(a2 - a3);
  result = a5;
  *a4 = (flt_502D58[(31 - v5) & (((int)(31 - v5) < 0) - 1)] + 1.0)
      * flt_502958[(a2 > a3 ? 0 : a3) | a2 & ~((a2 > a3) - 1)]
      * a1;
  *a5 = 0;
  return result;
}

// ===== sub_4CB250 @ 0x004CB250..0x004CB29F =====
double __cdecl sub_4CB250(int a1)
{
  double X; // [esp+0h] [ebp-8h]

  X = (double)(a1 & 0x1FFFFF);
  if ( a1 < 0 )
    X = -X;
  return ldexp(X, ((a1 >> 21) & 0x3FFu) - 788);
}

// ===== sub_4CB2A0 @ 0x004CB2A0..0x004CB41F =====
void *__cdecl sub_4CB2A0(int *a1, int a2, int a3)
{
  int v3; // esi
  int v4; // eax
  void *v5; // edx
  unsigned int *v6; // edi
  int v7; // ecx
  unsigned int v8; // esi
  _DWORD *v9; // eax
  int v10; // edx
  int v11; // ecx
  unsigned int *v12; // eax
  bool v13; // cc
  int *v14; // ebx
  int *v15; // edi
  int v16; // edx
  int v17; // eax
  int i; // ecx
  int v20; // [esp+10h] [ebp-90h]
  int v21; // [esp+10h] [ebp-90h]
  void *Block; // [esp+14h] [ebp-8Ch]
  int *v23; // [esp+18h] [ebp-88h]
  _DWORD v24[33]; // [esp+1Ch] [ebp-84h] BYREF

  v3 = a3;
  v4 = a3;
  if ( !a3 )
    v4 = a2;
  v5 = malloc(4 * v4);
  Block = v5;
  memset(v24, 0, sizeof(v24));
  v20 = 0;
  if ( a2 > 0 )
  {
    v6 = (unsigned int *)v5;
    v23 = a1;
    while ( 1 )
    {
      v7 = *v23;
      if ( *v23 <= 0 )
      {
        if ( !v3 )
          ++v6;
      }
      else
      {
        v8 = v24[v7];
        v9 = &v24[v7];
        if ( v7 < 32 && v8 >> v7 )
        {
          free(Block);
          return 0;
        }
        *v6++ = v8;
        v10 = v7;
        while ( (*v9 & 1) == 0 )
        {
          --v10;
          ++*v9--;
          if ( v10 <= 0 )
            goto LABEL_16;
        }
        if ( v10 == 1 )
          ++v24[1];
        else
          v24[v10] = 2 * v24[v10 - 1];
LABEL_16:
        v11 = v7 + 1;
        if ( v11 < 33 )
        {
          v12 = &v24[v11];
          do
          {
            if ( *v12 >> 1 != v8 )
              break;
            v8 = *v12;
            *v12 = 2 * *(v12 - 1);
            ++v11;
            ++v12;
          }
          while ( v11 < 33 );
        }
      }
      v13 = ++v20 < a2;
      ++v23;
      if ( !v13 )
        break;
      v3 = a3;
    }
    v14 = a1;
    v15 = (int *)Block;
    v21 = a2;
    do
    {
      v16 = *v14;
      v17 = 0;
      for ( i = 0; i < v16; ++i )
        v17 = ((unsigned int)*v15 >> i) & 1 | (2 * v17);
      if ( !a3 || v16 )
        *v15++ = v17;
      ++v14;
      --v21;
    }
    while ( v21 );
    return Block;
  }
  return v5;
}

// ===== sub_4CB420 @ 0x004CB420..0x004CB486 =====
int __cdecl sub_4CB420(int *a1)
{
  int *v1; // edi
  __int64 i; // rax
  int v3; // ecx
  int v4; // esi
  int v5; // esi

  v1 = a1;
  for ( i = (__int64)floor(pow((double)a1[1], 1.0 / (double)*a1)); ; LODWORD(i) = i + 1 )
  {
    while ( 1 )
    {
      v3 = 1;
      HIDWORD(i) = 1;
      if ( *a1 > 0 )
      {
        v4 = *a1;
        do
        {
          v3 *= (_DWORD)i;
          HIDWORD(i) *= (_DWORD)i + 1;
          --v4;
        }
        while ( v4 );
        v1 = a1;
      }
      v5 = v1[1];
      if ( v3 <= v5 )
        break;
LABEL_9:
      LODWORD(i) = i - 1;
    }
    if ( SHIDWORD(i) > v5 )
      break;
    if ( v3 > v5 )
      goto LABEL_9;
  }
  return i;
}

// ===== sub_4CB490 @ 0x004CB490..0x004CB67B =====
float *__cdecl sub_4CB490(int *a1, int a2, _DWORD *a3)
{
  int v4; // ebp
  int v5; // eax
  float *v7; // edi
  int v8; // edx
  _DWORD *v9; // ebx
  int v10; // eax
  int v11; // ecx
  long double v12; // st7
  int v13; // ecx
  int v14; // edi
  int i; // ebx
  long double v16; // st7
  int v17; // ecx
  int v18; // [esp+8h] [ebp-14h]
  float v19; // [esp+Ch] [ebp-10h]
  float v20; // [esp+10h] [ebp-Ch]
  float v21; // [esp+14h] [ebp-8h]
  int v22; // [esp+18h] [ebp-4h]
  _DWORD *v23; // [esp+20h] [ebp+4h]
  _DWORD *v24; // [esp+20h] [ebp+4h]
  float *v25; // [esp+24h] [ebp+8h]
  float j; // [esp+24h] [ebp+8h]

  v4 = 0;
  v18 = 0;
  v5 = a1[3];
  if ( v5 != 1 && v5 != 2 )
    return 0;
  v20 = sub_4CB250(a1[4]);
  v19 = sub_4CB250(a1[5]);
  v7 = (float *)calloc(a2 * *a1, 4u);
  v25 = v7;
  if ( a1[3] == 1 )
  {
    v22 = sub_4CB420(a1);
    if ( a1[1] > 0 )
    {
      v24 = a3;
      do
      {
        if ( !a3 || *(_DWORD *)(a1[2] + 4 * v4) )
        {
          v13 = *a1;
          v14 = 0;
          v21 = 0.0;
          for ( i = 1; v14 < *a1; ++v14 )
          {
            v16 = fabs((double)*(int *)(a1[8] + 4 * (v4 / i % v22))) * v19 + v21 + v20;
            if ( a1[7] )
              v21 = v16;
            if ( a3 )
              v17 = *v24 * v13;
            else
              v17 = v18 * v13;
            v25[v14 + v17] = v16;
            i *= v22;
            v13 = *a1;
          }
          v7 = v25;
          ++v18;
          ++v24;
        }
        ++v4;
      }
      while ( v4 < a1[1] );
    }
    return v7;
  }
  if ( a1[3] != 2 )
    return v7;
  v8 = 0;
  if ( a1[1] <= 0 )
    return v7;
  v9 = a3;
  v23 = a3;
  do
  {
    if ( !v9 || *(_DWORD *)(a1[2] + 4 * v8) )
    {
      v10 = *a1;
      v11 = 0;
      for ( j = 0.0; v11 < *a1; ++v11 )
      {
        v12 = fabs((double)*(int *)(a1[8] + 4 * (v11 + v8 * v10))) * v19 + j + v20;
        if ( a1[7] )
          j = v12;
        v9 = a3;
        if ( a3 )
          v7[v11 + v10 * *v23] = v12;
        else
          v7[v11 + v18 * v10] = v12;
        v10 = *a1;
      }
      ++v18;
      ++v23;
    }
    ++v8;
  }
  while ( v8 < a1[1] );
  return v7;
}

// ===== sub_4CB680 @ 0x004CB680..0x004CB741 =====
int __cdecl sub_4CB680(int a1)
{
  int result; // eax
  void **v2; // eax
  void **v3; // eax
  _DWORD *v4; // eax

  result = *(_DWORD *)(a1 + 48);
  if ( result )
  {
    if ( *(_DWORD *)(a1 + 32) )
      free(*(void **)(a1 + 32));
    if ( *(_DWORD *)(a1 + 8) )
      free(*(void **)(a1 + 8));
    v2 = *(void ***)(a1 + 36);
    if ( v2 )
    {
      free(*v2);
      free(*(void **)(*(_DWORD *)(a1 + 36) + 4));
      free(*(void **)(*(_DWORD *)(a1 + 36) + 8));
      free(*(void **)(*(_DWORD *)(a1 + 36) + 12));
      memset(*(void **)(a1 + 36), 0, 0x18u);
      free(*(void **)(a1 + 36));
    }
    v3 = *(void ***)(a1 + 40);
    if ( v3 )
    {
      free(*v3);
      free(*(void **)(*(_DWORD *)(a1 + 40) + 4));
      v4 = *(_DWORD **)(a1 + 40);
      *v4 = 0;
      v4[1] = 0;
      v4[2] = 0;
      v4[3] = 0;
      free(*(void **)(a1 + 40));
    }
    result = 0;
    memset((void *)a1, 0, 0x34u);
  }
  return result;
}

// ===== sub_4CB750 @ 0x004CB750..0x004CB76D =====
void __cdecl sub_4CB750(void *Block)
{
  if ( *((_DWORD *)Block + 12) )
  {
    sub_4CB680((int)Block);
    free(Block);
  }
}

// ===== sub_4CB770 @ 0x004CB770..0x004CB7D0 =====
int __cdecl sub_4CB770(void **a1)
{
  int result; // eax

  if ( a1[4] )
    free(a1[4]);
  if ( a1[5] )
    free(a1[5]);
  if ( a1[6] )
    free(a1[6]);
  if ( a1[7] )
    free(a1[7]);
  if ( a1[8] )
    free(a1[8]);
  result = 0;
  memset(a1, 0, 0x2Cu);
  return result;
}

// ===== sub_4CB7D0 @ 0x004CB7D0..0x004CB823 =====
int __cdecl sub_4CB7D0(_DWORD *a1, int a2)
{
  memset(a1, 0, 0x2Cu);
  a1[3] = a2;
  a1[1] = *(_DWORD *)(a2 + 4);
  a1[2] = *(_DWORD *)(a2 + 4);
  *a1 = *(_DWORD *)a2;
  a1[5] = sub_4CB2A0(*(int **)(a2 + 8), *(_DWORD *)(a2 + 4), 0);
  a1[4] = sub_4CB490((int *)a2, *(_DWORD *)(a2 + 4), 0);
  return 0;
}

// ===== sub_4CB830 @ 0x004CB830..0x004CBBF8 =====
int __cdecl sub_4CB830(_DWORD *a1, int a2)
{
  int v3; // ebx
  int *v4; // edi
  int v5; // eax
  int *v6; // ecx
  int v7; // edx
  int v8; // eax
  void *v9; // esp
  _DWORD *v10; // eax
  int v12; // eax
  int v13; // edx
  _DWORD *v14; // eax
  int v15; // eax
  void *v16; // esp
  int v17; // eax
  int *v18; // eax
  int v19; // ecx
  int v20; // edx
  int v21; // ecx
  float *v22; // eax
  size_t v23; // ecx
  size_t v24; // ecx
  int v25; // eax
  int v26; // ecx
  _BYTE *v27; // eax
  int v28; // eax
  int v29; // edi
  int v30; // ebx
  int v31; // edx
  int v32; // eax
  int v33; // edi
  char v34; // dl
  int v35; // eax
  int v36; // eax
  unsigned int v37; // ebx
  size_t v38; // edi
  int v39; // eax
  size_t *v40; // ecx
  _DWORD *v41; // eax
  unsigned int v42; // eax
  _DWORD v43[3]; // [esp+0h] [ebp-20h] BYREF
  int i; // [esp+Ch] [ebp-14h]
  _DWORD *v45; // [esp+10h] [ebp-10h]
  size_t Size; // [esp+14h] [ebp-Ch]
  _DWORD *v47; // [esp+18h] [ebp-8h]
  void *Block; // [esp+1Ch] [ebp-4h]
  _DWORD *v49; // [esp+28h] [ebp+8h]
  _DWORD *v50; // [esp+28h] [ebp+8h]
  int v51; // [esp+28h] [ebp+8h]
  unsigned int v52; // [esp+28h] [ebp+8h]
  _DWORD *v53; // [esp+2Ch] [ebp+Ch]
  int j; // [esp+2Ch] [ebp+Ch]

  v3 = 0;
  memset(a1, 0, 0x2Cu);
  v4 = (int *)a2;
  v5 = *(_DWORD *)(a2 + 4);
  if ( v5 > 0 )
  {
    v6 = *(int **)(a2 + 8);
    v7 = *(_DWORD *)(a2 + 4);
    do
    {
      if ( *v6 > 0 )
        ++v3;
      ++v6;
      --v7;
    }
    while ( v7 );
  }
  a1[1] = v5;
  a1[2] = v3;
  *a1 = *(_DWORD *)a2;
  Block = sub_4CB2A0(*(int **)(a2 + 8), *(_DWORD *)(a2 + 4), v3);
  Size = 4 * v3;
  v8 = 4 * v3 + 3;
  LOBYTE(v8) = v8 & 0xFC;
  v9 = alloca(v8);
  v10 = v43;
  v45 = v43;
  if ( Block )
  {
    if ( v3 > 0 )
    {
      v49 = Block;
      i = (char *)v43 - (_BYTE *)Block;
      v47 = (_DWORD *)v3;
      do
      {
        v12 = sub_4CBC00(*v49);
        v13 = i;
        *v49 = v12;
        v14 = v47;
        *(_DWORD *)((char *)v49 + v13) = v49;
        ++v49;
        v47 = (_DWORD *)((char *)v14 - 1);
      }
      while ( v14 != (_DWORD *)1 );
      v10 = v45;
    }
    qsort(v10, v3, 4u, sub_4CBC60);
    v15 = Size + 3;
    LOBYTE(v15) = (Size + 3) & 0xFC;
    v16 = alloca(v15);
    v50 = v43;
    a1[5] = malloc(Size);
    v17 = 0;
    if ( v3 > 0 )
    {
      v47 = v45;
      do
        v43[(*v47++ - (int)Block) >> 2] = v17++;
      while ( v17 < v3 );
      v18 = v43;
      v19 = (_BYTE *)Block - (_BYTE *)v43;
      v45 = (_DWORD *)v3;
      for ( i = (_BYTE *)Block - (_BYTE *)v43; ; v19 = i )
      {
        v20 = *v18;
        v21 = *(int *)((char *)v18++ + v19);
        *(_DWORD *)(a1[5] + 4 * v20) = v21;
        v45 = (_DWORD *)((char *)v45 - 1);
        if ( !v45 )
          break;
      }
      v4 = (int *)a2;
    }
    free(Block);
    v22 = sub_4CB490(v4, v3, v43);
    v23 = Size;
    a1[4] = v22;
    a1[6] = malloc(v23);
    v24 = 0;
    v25 = 0;
    if ( v4[1] > 0 )
    {
      v53 = v43;
      do
      {
        if ( *(int *)(v4[2] + 4 * v25) > 0 )
        {
          ++v24;
          *(_DWORD *)(a1[6] + 4 * *v53++) = v25;
        }
        ++v25;
      }
      while ( v25 < v4[1] );
    }
    a1[7] = malloc(v24);
    v26 = 0;
    for ( j = 0; v26 < v4[1]; ++v26 )
    {
      v27 = (_BYTE *)(v4[2] + 4 * v26);
      if ( *(int *)v27 > 0 )
      {
        *(_BYTE *)(a1[7] + *v50) = *v27;
        ++j;
        ++v50;
      }
    }
    v28 = sub_4D06F0(a1[2]) - 4;
    a1[9] = v28;
    if ( v28 < 5 )
      a1[9] = 5;
    if ( (int)a1[9] > 8 )
      a1[9] = 8;
    v45 = (_DWORD *)(1 << a1[9]);
    v29 = (int)v45;
    a1[8] = calloc((size_t)v45, 4u);
    v30 = 0;
    for ( a1[10] = 0; v30 < j; ++v30 )
    {
      v31 = a1[7];
      v32 = *(char *)(v31 + v30);
      if ( a1[10] < v32 )
        a1[10] = v32;
      if ( *(char *)(v31 + v30) <= a1[9] )
      {
        v51 = sub_4CBC00(*(_DWORD *)(a1[5] + 4 * v30));
        v33 = 0;
        v34 = *(_BYTE *)(a1[7] + v30);
        if ( 1 << (*((_BYTE *)a1 + 36) - v34) > 0 )
        {
          do
          {
            v35 = v33++ << v34;
            *(_DWORD *)(a1[8] + 4 * (v51 | v35)) = v30 + 1;
            v34 = *(_BYTE *)(a1[7] + v30);
          }
          while ( v33 < 1 << (*((_BYTE *)a1 + 36) - v34) );
        }
        v29 = (int)v45;
      }
    }
    v36 = -2 << (31 - *((_BYTE *)a1 + 36));
    v37 = 0;
    v52 = 0;
    Block = 0;
    Size = 0;
    i = v36;
    if ( v29 > 0 )
    {
      do
      {
        v38 = Size << (32 - a1[9]);
        if ( !*(_DWORD *)(a1[8] + 4 * sub_4CBC00(v38)) )
        {
          v39 = v37 + 1;
          if ( (int)(v37 + 1) < j )
          {
            v40 = (size_t *)(a1[5] + 4 * v39);
            do
            {
              if ( *v40 > v38 )
                break;
              ++v37;
              ++v39;
              ++v40;
            }
            while ( v39 < j );
            v52 = v37;
          }
          if ( (int)Block < j )
          {
            v41 = (_DWORD *)(a1[5] + 4 * (_DWORD)Block);
            do
            {
              if ( v38 < ((unsigned int)i & *v41) )
                break;
              ++v41;
              Block = (char *)Block + 1;
            }
            while ( (int)Block < j );
            LOWORD(v37) = v52;
          }
          v42 = j - (_DWORD)Block;
          if ( v52 > 0x7FFF )
            LOWORD(v37) = 0x7FFF;
          if ( v42 > 0x7FFF )
            v42 = 0x7FFF;
          *(_DWORD *)(a1[8] + 4 * sub_4CBC00(v38)) = v42 | ((v37 | 0xFFFF0000) << 15);
          v37 = v52;
        }
        ++Size;
      }
      while ( (int)Size < (int)v45 );
    }
    return 0;
  }
  else
  {
    sub_4CB770((void **)a1);
    return -1;
  }
}

// ===== sub_4CBC00 @ 0x004CBC00..0x004CBC60 =====
unsigned int __cdecl sub_4CBC00(unsigned int a1)
{
  int v1; // eax
  unsigned int v2; // eax
  unsigned int v3; // eax

  v1 = (a1 << 16) | ((unsigned __int64)a1 >> 16);
  BYTE1(v1) = 0;
  v2 = (((a1 << 16) | ((unsigned __int64)a1 >> 16)) >> 8) & 0xFF00FF | (v1 << 8);
  v3 = (((v2 >> 4) & 0xF0F0F0F | (16 * (v2 & 0xFF0F0F0F))) >> 2) & 0x33333333 | (4
                                                                               * ((v2 >> 4) & 0x3030303 | (16 * (v2 & 0xFF0F0F0F)) & 0xF3333333));
  return (2 * (v3 & 0xD5555555)) | (v3 >> 1) & 0x55555555;
}

// ===== sub_4CBC60 @ 0x004CBC60..0x004CBC7B =====
int __cdecl sub_4CBC60(_DWORD **a1, _DWORD **a2)
{
  return 2 * (**a2 < **a1) - 1;
}

// ===== sub_4CBC80 @ 0x004CBC80..0x004CBEBE =====
int __cdecl sub_4CBC80(int a1, _DWORD *a2)
{
  int v2; // eax
  int v3; // eax
  int v5; // ebp
  int v6; // eax
  int i; // edi
  int v8; // eax
  int v9; // eax
  int j; // ecx
  int k; // edi
  int v12; // eax
  int m; // edi
  int v14; // eax
  int v15; // eax
  int v16; // edi
  void *v17; // eax
  int v18; // ebp
  bool v19; // zf

  memset(a2, 0, 0x34u);
  a2[12] = 1;
  if ( (_UNKNOWN *)sub_4D4D00(a1, 24) != &unk_564342 )
    goto LABEL_36;
  *a2 = sub_4D4D00(a1, 16);
  v2 = sub_4D4D00(a1, 24);
  a2[1] = v2;
  if ( v2 == -1 )
    goto LABEL_36;
  v3 = sub_4D4D00(a1, 1);
  if ( v3 )
  {
    if ( v3 != 1 )
      return -1;
    v5 = sub_4D4D00(a1, 5) + 1;
    a2[2] = malloc(4 * a2[1]);
    v6 = a2[1];
    for ( i = 0; i < v6; ++v5 )
    {
      v8 = sub_4D06F0(v6 - i);
      v9 = sub_4D4D00(a1, v8);
      if ( v9 == -1 )
        goto LABEL_36;
      for ( j = 0; j < v9; *(_DWORD *)(a2[2] + 4 * i++) = v5 )
      {
        if ( i >= a2[1] )
          break;
        ++j;
      }
      v6 = a2[1];
    }
  }
  else
  {
    a2[2] = malloc(4 * a2[1]);
    if ( sub_4D4D00(a1, 1) )
    {
      for ( k = 0; k < a2[1]; ++k )
      {
        if ( sub_4D4D00(a1, 1) )
        {
          v12 = sub_4D4D00(a1, 5);
          if ( v12 == -1 )
            goto LABEL_36;
          *(_DWORD *)(a2[2] + 4 * k) = v12 + 1;
        }
        else
        {
          *(_DWORD *)(a2[2] + 4 * k) = 0;
        }
      }
    }
    else
    {
      for ( m = 0; m < a2[1]; *(_DWORD *)(a2[2] + 4 * m++) = v14 + 1 )
      {
        v14 = sub_4D4D00(a1, 5);
        if ( v14 == -1 )
          goto LABEL_36;
      }
    }
  }
  v15 = sub_4D4D00(a1, 4);
  a2[3] = v15;
  if ( v15 )
  {
    if ( v15 <= 0 || v15 > 2 )
      goto LABEL_36;
    a2[4] = sub_4D4D00(a1, 32);
    a2[5] = sub_4D4D00(a1, 32);
    a2[6] = sub_4D4D00(a1, 4) + 1;
    a2[7] = sub_4D4D00(a1, 1);
    v16 = 0;
    if ( a2[3] == 1 )
    {
      v16 = sub_4CB420(a2);
    }
    else if ( a2[3] == 2 )
    {
      v16 = a2[1] * *a2;
    }
    v17 = malloc(4 * v16);
    v18 = 0;
    v19 = v16 == 0;
    a2[8] = v17;
    if ( v16 > 0 )
    {
      do
        *(_DWORD *)(a2[8] + 4 * v18++) = sub_4D4D00(a1, a2[6]);
      while ( v18 < v16 );
      v19 = v16 == 0;
    }
    if ( !v19 && *(_DWORD *)(4 * v16 + a2[8] - 4) == -1 )
    {
LABEL_36:
      sub_4CB680((int)a2);
      return -1;
    }
  }
  return 0;
}

// ===== sub_4CBEC0 @ 0x004CBEC0..0x004CBEF4 =====
int __cdecl sub_4CBEC0(int a1, int a2, int a3)
{
  sub_4D4B20(
    a3,
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 4 * a2),
    *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 12) + 8) + 4 * a2));
  return *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 12) + 8) + 4 * a2);
}

// ===== sub_4CBF00 @ 0x004CBF00..0x004CC027 =====
int __cdecl sub_4CBF00(_DWORD *a1, int a2)
{
  _DWORD *v2; // edi
  int v3; // ebp
  int v4; // eax
  int v5; // ebx
  int v6; // esi
  int v7; // edi
  int v9; // ebx
  signed int v10; // eax
  unsigned int v11; // eax
  int v12; // edx
  int v13; // edx
  int v14; // ecx
  int v15; // [esp+10h] [ebp-4h]

  v2 = a1;
  v3 = a1[10];
  v15 = v3;
  v4 = sub_4D4C20(a2, a1[9]);
  if ( v4 < 0 )
  {
    v7 = a1[2];
    v6 = 0;
  }
  else
  {
    v5 = *(_DWORD *)(a1[8] + 4 * v4);
    if ( v5 >= 0 )
    {
      sub_4D4CD0(a2, *(char *)(a1[7] + v5 - 1));
      v6 = v5 - 1;
      goto LABEL_5;
    }
    v6 = (v5 >> 15) & 0x7FFF;
    v7 = a1[2] - (*(_DWORD *)(a1[8] + 4 * v4) & 0x7FFF);
  }
  v9 = a2;
  v10 = sub_4D4C20(a2, v3);
  if ( v10 < 0 )
  {
    while ( v3 > 1 )
    {
      v10 = sub_4D4C20(a2, --v3);
      if ( v10 >= 0 )
      {
        v15 = v3;
        goto LABEL_13;
      }
    }
    return -1;
  }
LABEL_13:
  v11 = sub_4CBC00(v10);
  v12 = v7 - v6;
  if ( v7 - v6 > 1 )
  {
    do
    {
      v13 = v12 >> 1;
      v14 = v11 < *(_DWORD *)(a1[5] + 4 * (v13 + v6));
      v7 -= v13 & -v14;
      v6 += v13 & (v14 - 1);
      v12 = v7 - v6;
    }
    while ( v7 - v6 > 1 );
    v3 = v15;
    v9 = a2;
  }
  if ( *(char *)(a1[7] + v6) > v3 )
  {
    sub_4D4CD0(v9, v3);
    return -1;
  }
  sub_4D4CD0(v9, *(char *)(a1[7] + v6));
  v2 = a1;
LABEL_5:
  if ( v6 >= 0 )
    return *(_DWORD *)(v2[6] + 4 * v6);
  return v6;
}
