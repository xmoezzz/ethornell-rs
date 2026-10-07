#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4CC030 @ 0x004CC030..0x004CC22B =====
int __cdecl sub_4CC030(_DWORD *a1, int a2, int a3, int a4)
{
  _DWORD *v4; // edi
  int v5; // esi
  int v6; // eax
  void *v7; // esp
  int v8; // eax
  void *v9; // esp
  int *v10; // ecx
  int v11; // eax
  int v12; // ebx
  int v13; // esi
  int v14; // edi
  int v15; // ebx
  int v16; // ebx
  signed int v17; // eax
  unsigned int v18; // eax
  int v19; // edx
  int v20; // edx
  int v21; // ecx
  _BYTE *v22; // eax
  _BYTE *v23; // eax
  int v24; // ecx
  int i; // ebx
  int v26; // eax
  int v27; // edx
  int v28; // esi
  _BYTE v30[12]; // [esp+0h] [ebp-24h] BYREF
  int v31; // [esp+Ch] [ebp-18h]
  int v32; // [esp+10h] [ebp-14h]
  int *v33; // [esp+14h] [ebp-10h]
  _BYTE *v34; // [esp+18h] [ebp-Ch]
  int v35; // [esp+1Ch] [ebp-8h]
  int v36; // [esp+20h] [ebp-4h]
  int v38; // [esp+38h] [ebp+14h]

  v4 = a1;
  v5 = a4 / *a1;
  v36 = v5;
  v6 = 4 * v5 + 3;
  LOBYTE(v6) = v6 & 0xFC;
  v7 = alloca(v6);
  v8 = 4 * v5 + 3;
  LOBYTE(v8) = (4 * v5 + 3) & 0xFC;
  v9 = alloca(v8);
  v10 = (int *)v30;
  v35 = 0;
  v33 = (int *)v30;
  if ( v5 > 0 )
  {
    v34 = v30;
    v31 = 0;
    while ( 1 )
    {
      v38 = v4[10];
      v11 = sub_4D4C20(a3, v4[9]);
      if ( v11 < 0 )
      {
        v14 = v4[2];
        v13 = 0;
      }
      else
      {
        v12 = *(_DWORD *)(v4[8] + 4 * v11);
        if ( v12 >= 0 )
        {
          sub_4D4CD0(a3, *(char *)(v4[7] + v12 - 1));
          v15 = v12 - 1;
          goto LABEL_22;
        }
        v13 = (v12 >> 15) & 0x7FFF;
        v14 = v4[2] - (*(_DWORD *)(v4[8] + 4 * v11) & 0x7FFF);
      }
      v16 = v38;
      v17 = sub_4D4C20(a3, v38);
      if ( v17 < 0 )
        break;
LABEL_13:
      v18 = sub_4CBC00(v17);
      v19 = v14 - v13;
      if ( v14 - v13 > 1 )
      {
        v32 = a1[5];
        do
        {
          v20 = v19 >> 1;
          v21 = v18 < *(_DWORD *)(v32 + 4 * (v20 + v13));
          v14 -= v20 & -v21;
          v13 += v20 & (v21 - 1);
          v19 = v14 - v13;
        }
        while ( v14 - v13 > 1 );
        v16 = v38;
      }
      if ( *(char *)(a1[7] + v13) > v16 )
      {
        sub_4D4CD0(a3, v16);
LABEL_20:
        v15 = -1;
        goto LABEL_21;
      }
      sub_4D4CD0(a3, *(char *)(a1[7] + v13));
      v15 = v13;
LABEL_21:
      v5 = v36;
      v4 = a1;
LABEL_22:
      v22 = v34;
      *(_DWORD *)&v34[v31] = v15;
      if ( v15 == -1 )
        return -1;
      v23 = v22 + 4;
      v24 = v35;
      *((_DWORD *)v23 - 1) = v4[4] + 4 * v15 * *v4;
      v35 = v24 + 1;
      v34 = v23;
      if ( v24 + 1 >= v5 )
      {
        v10 = v33;
        goto LABEL_25;
      }
    }
    while ( v16 > 1 )
    {
      v17 = sub_4D4C20(a3, --v16);
      if ( v17 >= 0 )
      {
        v38 = v16;
        goto LABEL_13;
      }
    }
    goto LABEL_20;
  }
LABEL_25:
  for ( i = 0; i < *v4; a2 += 4 * v5 )
  {
    if ( v5 > 0 )
    {
      v26 = a2;
      v27 = v5;
      do
      {
        v28 = *v10++;
        v26 += 4;
        --v27;
        *(float *)(v26 - 4) = *(float *)(v28 + 4 * i) + *(float *)(v26 - 4);
      }
      while ( v27 );
      v5 = v36;
      v10 = v33;
    }
    ++i;
  }
  return 0;
}

// ===== sub_4CC230 @ 0x004CC230..0x004CC5A0 =====
int __cdecl sub_4CC230(int *a1, float *a2, int a3, int a4)
{
  int *v4; // ebx
  int v5; // edi
  int v6; // eax
  int v7; // ebp
  int v8; // esi
  int v9; // edi
  int v10; // ebp
  signed int v11; // eax
  unsigned int v12; // eax
  int v13; // edx
  int v14; // edx
  int v15; // ecx
  int v16; // ecx
  float *v17; // ecx
  int v18; // edx
  float *v19; // eax
  double v20; // st7
  int v22; // edi
  float *v23; // esi
  int v24; // eax
  int v25; // ebp
  int v26; // edi
  int v27; // ebx
  int v28; // ebp
  int v29; // ebp
  signed int v30; // eax
  unsigned int v31; // eax
  int v32; // edx
  int v33; // edx
  int v34; // ecx
  float *v35; // ecx
  int v36; // eax
  double v37; // st7
  double v38; // st7
  double v39; // st7
  double v40; // st7
  double v41; // st7
  double v42; // st7
  double v43; // st7
  double v44; // st7
  int v45; // [esp+10h] [ebp-8h]
  int v46; // [esp+10h] [ebp-8h]
  int v47; // [esp+1Ch] [ebp+4h]
  int v48; // [esp+20h] [ebp+8h]

  v4 = a1;
  if ( *a1 <= 8 )
  {
    v22 = 0;
    v46 = 0;
    if ( a4 <= 0 )
      return 0;
    v23 = a2;
    while ( 1 )
    {
      v48 = v4[10];
      v24 = sub_4D4C20(a3, v4[9]);
      if ( v24 < 0 )
        break;
      v25 = *(_DWORD *)(v4[8] + 4 * v24);
      if ( v25 < 0 )
      {
        v26 = (v25 >> 15) & 0x7FFF;
        v27 = v4[2] - (*(_DWORD *)(v4[8] + 4 * v24) & 0x7FFF);
LABEL_33:
        v29 = v48;
        v30 = sub_4D4C20(a3, v48);
        if ( v30 < 0 )
        {
          do
          {
            if ( v29 <= 1 )
              return -1;
            v30 = sub_4D4C20(a3, --v29);
          }
          while ( v30 < 0 );
          v48 = v29;
        }
        v31 = sub_4CBC00(v30);
        v32 = v27 - v26;
        if ( v27 - v26 > 1 )
        {
          do
          {
            v33 = v32 >> 1;
            v34 = v31 < *(_DWORD *)(a1[5] + 4 * (v33 + v26));
            v27 -= v33 & -v34;
            v26 += v33 & (v34 - 1);
            v32 = v27 - v26;
          }
          while ( v27 - v26 > 1 );
          v29 = v48;
        }
        if ( *(char *)(a1[7] + v26) > v29 )
        {
          sub_4D4CD0(a3, v29);
          return -1;
        }
        sub_4D4CD0(a3, *(char *)(a1[7] + v26));
        v4 = a1;
        v28 = v26;
        v22 = v46;
        goto LABEL_43;
      }
      sub_4D4CD0(a3, *(char *)(v4[7] + v25 - 1));
      v28 = v25 - 1;
LABEL_43:
      if ( v28 == -1 )
        return -1;
      v35 = (float *)(v4[4] + 4 * v28 * *v4);
      v36 = 0;
      switch ( *v4 )
      {
        case 1:
          goto LABEL_52;
        case 2:
          goto LABEL_51;
        case 3:
          goto LABEL_50;
        case 4:
          goto LABEL_49;
        case 5:
          goto LABEL_48;
        case 6:
          goto LABEL_47;
        case 7:
          goto LABEL_46;
        case 8:
          v37 = *v35 + *v23;
          ++v22;
          v36 = 1;
          *v23++ = v37;
LABEL_46:
          v38 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v38;
LABEL_47:
          v39 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v39;
LABEL_48:
          v40 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v40;
LABEL_49:
          v41 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v41;
LABEL_50:
          v42 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v42;
LABEL_51:
          v43 = v35[v36++] + *v23;
          ++v22;
          *v23++ = v43;
LABEL_52:
          v44 = v35[v36] + *v23;
          ++v22;
          ++v23;
          v46 = v22;
          *(v23 - 1) = v44;
          break;
        default:
          break;
      }
      if ( v22 >= a4 )
        return 0;
    }
    v27 = v4[2];
    v26 = 0;
    goto LABEL_33;
  }
  v5 = 0;
  v45 = 0;
  if ( a4 <= 0 )
    return 0;
  while ( 1 )
  {
    v47 = v4[10];
    v6 = sub_4D4C20(a3, v4[9]);
    if ( v6 < 0 )
    {
      v9 = v4[2];
      v8 = 0;
    }
    else
    {
      v7 = *(_DWORD *)(v4[8] + 4 * v6);
      if ( v7 >= 0 )
      {
        sub_4D4CD0(a3, *(char *)(v4[7] + v7 - 1));
        v8 = v7 - 1;
        goto LABEL_18;
      }
      v8 = (v7 >> 15) & 0x7FFF;
      v9 = v4[2] - (*(_DWORD *)(v4[8] + 4 * v6) & 0x7FFF);
    }
    v10 = v47;
    v11 = sub_4D4C20(a3, v47);
    if ( v11 < 0 )
      break;
LABEL_13:
    v12 = sub_4CBC00(v11);
    v13 = v9 - v8;
    if ( v9 - v8 > 1 )
    {
      do
      {
        v14 = v13 >> 1;
        v15 = v12 < *(_DWORD *)(v4[5] + 4 * (v14 + v8));
        v9 -= v14 & -v15;
        v8 += v14 & (v15 - 1);
        v13 = v9 - v8;
      }
      while ( v9 - v8 > 1 );
      v10 = v47;
    }
    v16 = v4[7];
    if ( *(char *)(v16 + v8) > v10 )
    {
      sub_4D4CD0(a3, v10);
      return -1;
    }
    sub_4D4CD0(a3, *(char *)(v16 + v8));
    v5 = v45;
LABEL_18:
    if ( v8 == -1 )
      return -1;
    v17 = (float *)(v4[4] + 4 * v8 * *v4);
    v18 = 0;
    if ( *v4 > 0 )
    {
      v19 = &a2[v5];
      do
      {
        v20 = *v17 + *v19;
        ++v18;
        ++v17;
        ++v5;
        *v19++ = v20;
      }
      while ( v18 < *v4 );
      v45 = v5;
    }
    if ( v5 >= a4 )
      return 0;
  }
  while ( v10 > 1 )
  {
    v11 = sub_4D4C20(a3, --v10);
    if ( v11 >= 0 )
    {
      v47 = v10;
      goto LABEL_13;
    }
  }
  return -1;
}

// ===== sub_4CC5C0 @ 0x004CC5C0..0x004CC735 =====
int __cdecl sub_4CC5C0(int *a1, int a2, int a3, int a4)
{
  int v4; // edi
  int v6; // eax
  int v7; // ebx
  int v8; // esi
  int v9; // edi
  int v10; // ebx
  signed int v11; // eax
  unsigned int v12; // eax
  int v13; // edx
  int v14; // edx
  int v15; // ecx
  int v16; // ecx
  _DWORD *v17; // ecx
  int v18; // edx
  _DWORD *v19; // eax
  int v21; // [esp+10h] [ebp-4h]
  int v22; // [esp+18h] [ebp+4h]

  v4 = 0;
  v21 = 0;
  if ( a4 <= 0 )
    return 0;
  while ( 1 )
  {
    v22 = a1[10];
    v6 = sub_4D4C20(a3, a1[9]);
    if ( v6 < 0 )
    {
      v9 = a1[2];
      v8 = 0;
    }
    else
    {
      v7 = *(_DWORD *)(a1[8] + 4 * v6);
      if ( v7 >= 0 )
      {
        sub_4D4CD0(a3, *(char *)(a1[7] + v7 - 1));
        v8 = v7 - 1;
        goto LABEL_17;
      }
      v8 = (v7 >> 15) & 0x7FFF;
      v9 = a1[2] - (*(_DWORD *)(a1[8] + 4 * v6) & 0x7FFF);
    }
    v10 = v22;
    v11 = sub_4D4C20(a3, v22);
    if ( v11 < 0 )
      break;
LABEL_12:
    v12 = sub_4CBC00(v11);
    v13 = v9 - v8;
    if ( v9 - v8 > 1 )
    {
      do
      {
        v14 = v13 >> 1;
        v15 = v12 < *(_DWORD *)(a1[5] + 4 * (v14 + v8));
        v9 -= v14 & -v15;
        v8 += v14 & (v15 - 1);
        v13 = v9 - v8;
      }
      while ( v9 - v8 > 1 );
      v10 = v22;
    }
    v16 = a1[7];
    if ( *(char *)(v16 + v8) > v10 )
    {
      sub_4D4CD0(a3, v10);
      return -1;
    }
    sub_4D4CD0(a3, *(char *)(v16 + v8));
    v4 = v21;
LABEL_17:
    if ( v8 == -1 )
      return -1;
    v17 = (_DWORD *)(a1[4] + 4 * v8 * *a1);
    v18 = 0;
    if ( *a1 > 0 )
    {
      v19 = (_DWORD *)(a2 + 4 * v4);
      do
      {
        ++v4;
        *v19++ = *v17;
        ++v18;
        ++v17;
      }
      while ( v18 < *a1 );
      v21 = v4;
    }
    if ( v4 >= a4 )
      return 0;
  }
  while ( v10 > 1 )
  {
    v11 = sub_4D4C20(a3, --v10);
    if ( v11 >= 0 )
    {
      v22 = v10;
      goto LABEL_12;
    }
  }
  return -1;
}

// ===== sub_4CC740 @ 0x004CC740..0x004CC8FB =====
int __cdecl sub_4CC740(_DWORD *a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // esi
  int v7; // eax
  _DWORD *v8; // edi
  int v9; // ebp
  int v10; // eax
  int v11; // ebx
  int v12; // esi
  int v13; // edi
  int v14; // ebx
  int v15; // ebx
  signed int v16; // eax
  unsigned int v17; // eax
  int v18; // edx
  int v19; // edx
  int v20; // ecx
  float *v21; // ecx
  int i; // edx
  int v23; // eax
  int v25; // [esp+10h] [ebp-8h]
  int v26; // [esp+14h] [ebp-4h]
  int v27; // [esp+24h] [ebp+Ch]
  int v28; // [esp+30h] [ebp+18h]

  v6 = 0;
  v25 = 0;
  v7 = a3 + a6;
  v27 = a3 / a4;
  v26 = v7 / a4;
  if ( v27 >= v7 / a4 )
    return 0;
  while ( 1 )
  {
    v8 = a1;
    v9 = a1[10];
    v28 = v9;
    v10 = sub_4D4C20(a5, a1[9]);
    if ( v10 < 0 )
    {
      v13 = a1[2];
      v12 = 0;
    }
    else
    {
      v11 = *(_DWORD *)(a1[8] + 4 * v10);
      if ( v11 >= 0 )
      {
        sub_4D4CD0(a5, *(char *)(a1[7] + v11 - 1));
        v14 = v11 - 1;
        goto LABEL_17;
      }
      v12 = (v11 >> 15) & 0x7FFF;
      v13 = a1[2] - (*(_DWORD *)(a1[8] + 4 * v10) & 0x7FFF);
    }
    v15 = a5;
    v16 = sub_4D4C20(a5, v9);
    if ( v16 < 0 )
      break;
LABEL_12:
    v17 = sub_4CBC00(v16);
    v18 = v13 - v12;
    if ( v13 - v12 > 1 )
    {
      do
      {
        v19 = v18 >> 1;
        v20 = v17 < *(_DWORD *)(a1[5] + 4 * (v19 + v12));
        v13 -= v19 & -v20;
        v12 += v19 & (v20 - 1);
        v18 = v13 - v12;
      }
      while ( v13 - v12 > 1 );
      v9 = v28;
      v15 = a5;
    }
    if ( *(char *)(a1[7] + v12) > v9 )
    {
      sub_4D4CD0(v15, v9);
      return -1;
    }
    sub_4D4CD0(v15, *(char *)(a1[7] + v12));
    v8 = a1;
    v14 = v12;
    v6 = v25;
LABEL_17:
    if ( v14 == -1 )
      return -1;
    v21 = (float *)(v8[4] + 4 * v14 * *v8);
    for ( i = 0; i < *v8; ++v21 )
    {
      v23 = *(_DWORD *)(a2 + 4 * v6++);
      v25 = v6;
      *(float *)(v23 + 4 * v27) = *(float *)(v23 + 4 * v27) + *v21;
      if ( v6 == a4 )
      {
        v25 = 0;
        v6 = 0;
        ++v27;
      }
      ++i;
    }
    if ( v27 >= v26 )
      return 0;
  }
  while ( v9 > 1 )
  {
    v16 = sub_4D4C20(a5, --v9);
    if ( v16 >= 0 )
    {
      v28 = v9;
      goto LABEL_12;
    }
  }
  return -1;
}

// ===== sub_4CC900 @ 0x004CC900..0x004CC9D3 =====
void __cdecl sub_4CC900(int a1)
{
  int v1; // ecx
  int v2; // eax
  int v3; // esi
  int v4; // ebx

  if ( a1 )
  {
    if ( *(_DWORD *)a1 )
      free(*(void **)a1);
    if ( *(_DWORD *)(a1 + 4) )
      free(*(void **)(a1 + 4));
    if ( *(_DWORD *)(a1 + 20) )
      free(*(void **)(a1 + 20));
    if ( *(_DWORD *)(a1 + 48) )
      free(*(void **)(a1 + 48));
    if ( *(_DWORD *)(a1 + 52) )
      free(*(void **)(a1 + 52));
    if ( *(_DWORD *)(a1 + 56) )
      free(*(void **)(a1 + 56));
    v1 = *(_DWORD *)(a1 + 96);
    if ( v1 )
    {
      v2 = *(_DWORD *)(a1 + 8);
      if ( v2 )
      {
        v3 = 0;
        if ( v2 > 0 )
        {
          v4 = 0;
          do
          {
            sub_4D4AC0(*(_DWORD *)(a1 + 96) + v4);
            ++v3;
            v4 += 20;
          }
          while ( v3 < *(_DWORD *)(a1 + 8) );
        }
      }
      else
      {
        sub_4D4AC0(v1);
      }
      free(*(void **)(a1 + 96));
    }
    if ( *(_DWORD *)(a1 + 100) )
      free(*(void **)(a1 + 100));
    memset((void *)a1, 0, 0x68u);
  }
}

// ===== sub_4CC9E0 @ 0x004CC9E0..0x004CC9F5 =====
BOOL __cdecl sub_4CC9E0(int a1)
{
  return *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 64) + 104) + 80) != 0;
}

// ===== sub_4CCA00 @ 0x004CCA00..0x004CCA5D =====
int __cdecl sub_4CCA00(void **a1)
{
  void **v1; // esi
  int v2; // ebx
  int result; // eax

  sub_4CE3E0(a1 + 4);
  v1 = a1 + 12;
  v2 = 7;
  do
  {
    free(*v1);
    v1 += 4;
    --v2;
  }
  while ( v2 );
  free(a1[9]);
  free(a1[38]);
  free(a1[40]);
  result = 0;
  memset(a1, 0, 0xB4u);
  return result;
}

// ===== sub_4CCA60 @ 0x004CCA60..0x004CCA86 =====
int __cdecl sub_4CCA60(int *a1, int a2)
{
  int result; // eax

  result = *a1;
  if ( *a1 != 1 )
    return sub_4CCA90(result, a2, a1[1], a1[1] + 4 * result, a1[2]);
  return result;
}

// ===== sub_4CCA90 @ 0x004CCA90..0x004CCC37 =====
int __cdecl sub_4CCA90(int a1, _DWORD *a2, int a3, int a4, int a5)
{
  int v6; // ecx
  int result; // eax
  _DWORD *v8; // edi
  int v10; // esi
  int v11; // eax
  _DWORD *v12; // edx
  bool v13; // zf
  int v14; // [esp-14h] [ebp-28h]
  int v15; // [esp-10h] [ebp-24h]
  int v16; // [esp-Ch] [ebp-20h]
  int v17; // [esp-Ch] [ebp-20h]
  int v18; // [esp-Ch] [ebp-20h]
  int v19; // [esp+8h] [ebp-Ch]
  _DWORD *v20; // [esp+Ch] [ebp-8h]
  int v21; // [esp+10h] [ebp-4h]
  int v22; // [esp+24h] [ebp+10h]
  int v23; // [esp+28h] [ebp+14h]

  v6 = a1;
  result = *(_DWORD *)(a5 + 4);
  v23 = 1;
  v19 = a1;
  if ( result > 0 )
  {
    v8 = a2;
    v20 = (_DWORD *)(a5 + 4 * result + 4);
    v21 = result;
    do
    {
      v10 = v6 / *v20;
      v11 = a1 / v6;
      v22 = v10 * (a1 / v6);
      v19 -= a1 / v6 * (*v20 - 1);
      v23 = 1 - v23;
      v12 = v20;
      if ( *v20 == 4 )
      {
        v18 = a4 + 4 * (v11 + v19 + v11) - 4;
        v15 = a4 + 4 * (v11 + v19) - 4;
        v14 = a4 + 4 * v19 - 4;
        if ( v23 )
          sub_4CCE00(v11, v10, a3, a2, v14, v15, v18);
        else
          sub_4CCE00(v11, v10, a2, a3, v14, v15, v18);
      }
      else if ( *v20 == 2 )
      {
        v17 = a4 + 4 * v19 - 4;
        if ( v23 )
          sub_4CCC40(v11, v10, a3, a2, v17);
        else
          sub_4CCC40(v11, v10, a2, a3, v17);
      }
      else
      {
        if ( v11 == 1 )
        {
          v12 = v20;
          v23 = 1 - v23;
        }
        v16 = a4 + 4 * v19 - 4;
        if ( v23 )
        {
          sub_4CD270(v11, *v12, v10, v22, a3, a3, a3, a2, a2, v16);
          v23 = 0;
        }
        else
        {
          sub_4CD270(v11, *v12, v10, v22, a2, a2, a2, a3, a3, v16);
          v23 = 1;
        }
      }
      v13 = v21 == 1;
      v6 = v10;
      --v20;
      --v21;
    }
    while ( !v13 );
    result = v23;
    if ( v23 != 1 )
    {
      result = a1;
      if ( a1 > 0 )
      {
        do
        {
          *v8 = *(_DWORD *)((char *)v8 + a3 - (_DWORD)a2);
          ++v8;
          --result;
        }
        while ( result );
      }
    }
  }
  return result;
}

// ===== sub_4CCC40 @ 0x004CCC40..0x004CCE00 =====
void __cdecl sub_4CCC40(int a1, int a2, float *a3, float *a4, int a5)
{
  int v6; // edi
  float *v7; // edx
  int v8; // esi
  float *v9; // eax
  float *v10; // ecx
  float *v11; // ecx
  float *v12; // esi
  float *v13; // ecx
  float *v14; // edi
  float *v15; // eax
  float *v16; // edx
  unsigned int v17; // ebx
  double v18; // st7
  double v19; // st6
  double v20; // st7
  double v21; // st6
  double v22; // st5
  double v23; // st6
  bool v24; // zf
  int v25; // ecx
  int v26; // esi
  int v27; // ebx
  int v28; // ebp
  float *v29; // ecx
  float *v30; // eax
  float *v31; // edx
  int v32; // [esp+10h] [ebp-14h]
  float *v33; // [esp+10h] [ebp-14h]
  float *v34; // [esp+14h] [ebp-10h]
  float *v35; // [esp+14h] [ebp-10h]
  float *v36; // [esp+18h] [ebp-Ch]
  int v37; // [esp+1Ch] [ebp-8h]
  int v38; // [esp+20h] [ebp-4h]
  float *v39; // [esp+28h] [ebp+4h]
  float *v40; // [esp+28h] [ebp+4h]

  v6 = a2;
  v7 = a3;
  v8 = a2 * a1;
  v38 = a2 * a1;
  if ( a2 > 0 )
  {
    v9 = a3;
    v34 = a4;
    v10 = &a3[v8];
    v39 = &a4[2 * a1 - 1];
    v32 = a2;
    do
    {
      *v34 = *v9 + *v10;
      *v39 = *v9 - *v10;
      v34 += 2 * a1;
      v9 += a1;
      v10 += a1;
      v39 += 2 * a1;
      --v32;
    }
    while ( v32 );
    v6 = a2;
  }
  if ( a1 >= 2 )
  {
    if ( a1 == 2 )
      goto LABEL_13;
    if ( v6 > 0 )
    {
      v11 = &a3[v8];
      v35 = a4;
      v40 = a3;
      v12 = &a4[2 * a1];
      v33 = v11;
      v36 = v12;
      v37 = v6;
      do
      {
        v13 = v40;
        v14 = v35;
        v15 = v33;
        v16 = (float *)(a5 + 4);
        v17 = (unsigned int)(a1 - 1) >> 1;
        do
        {
          v18 = *(v16 - 1) * v15[1];
          v19 = v15[2] * *v16;
          v15 += 2;
          v13 += 2;
          v14 += 2;
          v12 -= 2;
          v20 = v18 + v19;
          v21 = *(v16 - 1) * *v15;
          v22 = *v16 * *(v15 - 1);
          v16 += 2;
          --v17;
          v23 = v21 - v22;
          *v14 = v23 + *v13;
          *v12 = v23 - *v13;
          *(v14 - 1) = v20 + *(v13 - 1);
          *(v12 - 1) = *(v13 - 1) - v20;
        }
        while ( v17 );
        v6 = a2;
        v7 = a3;
        v12 = &v36[2 * a1];
        v40 += a1;
        v24 = v37 == 1;
        v36 = v12;
        v33 += a1;
        v35 += 2 * a1;
        --v37;
      }
      while ( !v24 );
    }
    if ( (a1 & 1) != 1 )
    {
LABEL_13:
      v25 = a1 - 1;
      v26 = v38 + a1 - 1;
      if ( v6 > 0 )
      {
        v27 = 8 * a1;
        v28 = 4 * a1;
        v29 = &v7[v25];
        v30 = &a4[v28 / 4u];
        v31 = &v7[v26];
        do
        {
          *v30 = -*v31;
          v31 = (float *)((char *)v31 + v28);
          *(v30 - 1) = *v29;
          v30 = (float *)((char *)v30 + v27);
          v29 = (float *)((char *)v29 + v28);
          --v6;
        }
        while ( v6 );
      }
    }
  }
}

// ===== sub_4CCE00 @ 0x004CCE00..0x004CD262 =====
int __cdecl sub_4CCE00(int a1, int a2, float *a3, float *a4, int a5, int a6, float *a7)
{
  int result; // eax
  int v8; // ebx
  float *v9; // edx
  int v10; // ebp
  float *v11; // esi
  float *v12; // ebx
  int v13; // edi
  double v14; // st7
  double v15; // st6
  int v16; // ecx
  double v17; // st7
  int v18; // edi
  int v19; // ebx
  int v20; // ecx
  float *v21; // ebp
  float *v22; // esi
  float *v23; // edi
  float *v24; // eax
  float *v25; // ecx
  float *v26; // edx
  double v27; // st7
  double v28; // st7
  double v29; // st6
  double v30; // st7
  double v31; // st6
  double v32; // st5
  bool v33; // zf
  int v34; // edi
  int v35; // ecx
  float *v36; // edx
  int v37; // eax
  float *v38; // ebx
  int v39; // edi
  float *v40; // ebp
  float *v41; // esi
  float *i; // edi
  double v43; // st7
  double v44; // st6
  double v45; // st6
  double v46; // st5
  double v47; // st7
  float *v48; // [esp+10h] [ebp-5Ch]
  float *v49; // [esp+10h] [ebp-5Ch]
  float *v50; // [esp+14h] [ebp-58h]
  float *v51; // [esp+14h] [ebp-58h]
  float *v52; // [esp+18h] [ebp-54h]
  float *v53; // [esp+18h] [ebp-54h]
  int v54; // [esp+1Ch] [ebp-50h]
  float *v55; // [esp+1Ch] [ebp-50h]
  float *v56; // [esp+20h] [ebp-4Ch]
  int v57; // [esp+24h] [ebp-48h]
  float *v58; // [esp+24h] [ebp-48h]
  float *v59; // [esp+28h] [ebp-44h]
  float *v60; // [esp+28h] [ebp-44h]
  int v61; // [esp+2Ch] [ebp-40h]
  int v62; // [esp+30h] [ebp-3Ch]
  float v63; // [esp+34h] [ebp-38h]
  float v64; // [esp+34h] [ebp-38h]
  float v65; // [esp+38h] [ebp-34h]
  float v66; // [esp+38h] [ebp-34h]
  float v67; // [esp+3Ch] [ebp-30h]
  float v68; // [esp+3Ch] [ebp-30h]
  float v69; // [esp+40h] [ebp-2Ch]
  float v70; // [esp+44h] [ebp-28h]
  float v71; // [esp+48h] [ebp-24h]
  unsigned int v72; // [esp+4Ch] [ebp-20h]
  int v73; // [esp+54h] [ebp-18h]
  int v74; // [esp+58h] [ebp-14h]
  char *v75; // [esp+60h] [ebp-Ch]
  int v76; // [esp+68h] [ebp-4h]
  float *v77; // [esp+70h] [ebp+4h]
  int v78; // [esp+80h] [ebp+14h]
  float *v79; // [esp+88h] [ebp+1Ch]

  result = a1;
  v8 = a2;
  v9 = a4;
  v10 = a2 * a1;
  v11 = a3;
  v54 = 0;
  if ( a2 > 0 )
  {
    v12 = &a3[v10];
    v13 = 4 * a1;
    v48 = a3;
    v59 = v12;
    v52 = &a3[3 * v10];
    v50 = &a3[2 * v10];
    v57 = a2;
    do
    {
      v14 = *v12 + *v52;
      v15 = *v50 + *v48;
      a4[4 * v54] = v15 + v14;
      a4[4 * v54 - 1 + v13] = v15 - v14;
      v16 = 2 * a1 + 4 * v54;
      a4[v16 - 1] = *v48 - *v50;
      v17 = *v52 - *v59;
      v12 = &v59[v13 / 4u];
      v59 = (float *)((char *)v59 + v13);
      a4[v16] = v17;
      v54 += a1;
      v48 = (float *)((char *)v48 + v13);
      v50 = (float *)((char *)v50 + v13);
      v52 = (float *)((char *)v52 + v13);
      --v57;
    }
    while ( v57 );
    v8 = a2;
  }
  if ( a1 >= 2 )
  {
    if ( a1 == 2 )
      goto LABEL_17;
    v18 = 0;
    v73 = 0;
    if ( v8 > 0 )
    {
      v19 = 2 * a1;
      v76 = 4 * a1;
      v56 = a4;
      v60 = &a3[3 * v10];
      v58 = &a3[v10];
      v74 = a2;
      while ( 1 )
      {
        v20 = v19 + 4 * v18;
        if ( result > 2 )
        {
          v53 = &v9[v20];
          v21 = &v11[v18];
          v51 = &v9[4 * v18 + v19];
          v49 = &v9[v19 + v20];
          v22 = &v11[2 * a2 * a1 + v18];
          v23 = v60;
          v62 = a6 - a5;
          v75 = (char *)a7 - a5;
          v24 = (float *)(a5 + 4);
          v61 = a6 - (_DWORD)a7;
          v55 = v56;
          v25 = v58;
          v72 = (unsigned int)(a1 - 1) >> 1;
          v26 = a7;
          do
          {
            v27 = v25[1] * *(v24 - 1);
            v25 += 2;
            v22 += 2;
            v55 += 2;
            v23 += 2;
            v28 = v27 + *v24 * *v25;
            v51 += 2;
            v21 += 2;
            v53 -= 2;
            v49 -= 2;
            v63 = *(v24 - 1) * *v25 - *v24 * *(v25 - 1);
            v69 = *(v22 - 1) * *(float *)((char *)v26 + v61) + *(float *)((char *)v24 + v62) * *v22;
            v67 = *v22 * *(float *)((char *)v26 + v61) - *(v22 - 1) * *(float *)((char *)v24 + v62);
            v29 = *(float *)((char *)v24 + (_DWORD)v75) * *v23 + *(v23 - 1) * *v26;
            v65 = *v23 * *v26 - *(v23 - 1) * *(float *)((char *)v24 + (_DWORD)v75);
            v70 = v29 + v28;
            v30 = v29 - v28;
            v71 = v65 + v63;
            v31 = v63 - v65;
            v64 = v67 + *v21;
            v68 = *v21 - v67;
            v66 = v69 + *(v21 - 1);
            v32 = *(v21 - 1) - v69;
            *(v55 - 1) = v66 + v70;
            *v55 = v64 + v71;
            v24 += 2;
            v26 += 2;
            *(v53 - 1) = v32 - v31;
            *v53 = v30 - v68;
            *(v51 - 1) = v32 + v31;
            *v51 = v68 + v30;
            *(v49 - 1) = v66 - v70;
            *v49 = v71 - v64;
            --v72;
          }
          while ( v72 );
          v11 = a3;
          v9 = a4;
          v18 = v73;
          v10 = a2 * a1;
          result = a1;
        }
        v56 += 4 * a1;
        v18 += result;
        v58 = (float *)((char *)v58 + v76);
        v33 = v74 == 1;
        v73 = v18;
        v60 = (float *)((char *)v60 + v76);
        --v74;
        if ( v33 )
          break;
        v19 = 2 * a1;
      }
      v8 = a2;
    }
    if ( (result & 1) == 0 )
    {
LABEL_17:
      v34 = result + v10 - 1;
      if ( v8 > 0 )
      {
        v78 = 16 * result;
        v35 = 4 * result;
        v77 = &v9[result];
        v36 = &v9[3 * result];
        v37 = v34 + v10;
        v79 = &v11[2 * v10 + v34];
        v38 = &v11[v34];
        v39 = v34 - v10;
        v40 = v77;
        result = (int)&v11[v37];
        v41 = &v11[v39];
        for ( i = v79; ; i = (float *)((char *)i + v35) )
        {
          v43 = -((*i + *v38) * flt_502E04);
          v44 = *v38 - *i;
          v38 = (float *)((char *)v38 + v35);
          v45 = v44 * flt_502E04;
          *(v40 - 1) = v45 + *v41;
          v46 = *v41 - v45;
          v41 = (float *)((char *)v41 + v35);
          *(v36 - 1) = v46;
          *v40 = v43 - *(float *)result;
          v40 = (float *)((char *)v40 + v78);
          v47 = v43 + *(float *)result;
          result += v35;
          *v36 = v47;
          v36 = (float *)((char *)v36 + v78);
          if ( !--a2 )
            break;
        }
      }
    }
  }
  return result;
}

// ===== sub_4CD270 @ 0x004CD270..0x004CDE20 =====
int __cdecl sub_4CD270(
        int a1,
        int a2,
        int a3,
        int a4,
        _DWORD *a5,
        float *a6,
        char *a7,
        _DWORD *a8,
        _DWORD *a9,
        int a10)
{
  long double v11; // st7
  int v12; // esi
  _DWORD *v13; // eax
  int v14; // esi
  _DWORD *v15; // eax
  int v16; // ebx
  int v17; // esi
  int v18; // edi
  int v19; // eax
  float *v20; // ebp
  float *v21; // ecx
  float *v22; // eax
  float *v23; // ebx
  unsigned int v24; // esi
  double v25; // st7
  double v26; // st6
  float *v27; // esi
  int v28; // ebp
  float *v29; // ecx
  float *v30; // edi
  float *v31; // eax
  int v32; // ebx
  double v33; // st7
  double v34; // st6
  int v35; // ebp
  int v36; // ebx
  int v37; // ebp
  int v38; // edi
  int v39; // esi
  int v40; // eax
  int v41; // ecx
  float *v42; // ebx
  int v43; // edi
  float *v44; // eax
  float *v45; // ecx
  double v46; // st7
  float *v47; // ebx
  float *v48; // edi
  float *v49; // ecx
  float *v50; // esi
  float *v51; // ebp
  float *v52; // eax
  int v53; // esi
  unsigned int v54; // edi
  double v55; // st7
  bool v56; // zf
  int v57; // ebp
  _DWORD *v58; // eax
  int v59; // esi
  int v60; // ebx
  int v61; // ecx
  int v62; // edi
  int v63; // esi
  int v64; // ecx
  float *v65; // ecx
  int v66; // esi
  float *v67; // eax
  double v68; // st7
  double v69; // st6
  int v70; // eax
  float *v71; // ebx
  float *v72; // edi
  int v73; // ebp
  float *v74; // esi
  float *v75; // ecx
  float *v76; // eax
  double v77; // st5
  double v78; // st5
  double v79; // st5
  double v80; // st4
  float *v81; // esi
  float *v82; // ecx
  char *v83; // edi
  float *v84; // eax
  int v85; // ebx
  double v86; // st3
  float *v87; // edi
  float *v88; // eax
  float *v89; // ecx
  int v90; // esi
  double v91; // st7
  int v92; // ebx
  _DWORD *v93; // edi
  _DWORD *v94; // esi
  int v95; // ecx
  _DWORD *v96; // ecx
  _DWORD *v97; // eax
  int v98; // ebp
  _DWORD *v99; // ecx
  int *v100; // eax
  int v101; // esi
  int v102; // ebp
  int v103; // ebp
  int result; // eax
  _DWORD *v105; // ecx
  _DWORD *v106; // esi
  _DWORD *v107; // eax
  int v108; // edi
  int v109; // ebp
  int v110; // ebx
  int v111; // ebp
  int v112; // esi
  int v113; // ecx
  float *v114; // edi
  int v115; // eax
  int v116; // ebx
  float *v117; // ecx
  float *v118; // eax
  float *v119; // esi
  double v120; // st7
  int v121; // ebp
  _DWORD *v122; // ecx
  _DWORD *v123; // edi
  int v124; // esi
  int v125; // ebp
  float *v126; // ecx
  int v127; // eax
  float *v128; // edi
  int v129; // esi
  unsigned int v130; // ebx
  double v131; // st7
  int v132; // [esp+0h] [ebp-38h]
  int v133; // [esp+4h] [ebp-34h]
  int v134; // [esp+4h] [ebp-34h]
  float *v135; // [esp+4h] [ebp-34h]
  float *v136; // [esp+4h] [ebp-34h]
  int v137; // [esp+4h] [ebp-34h]
  int v138; // [esp+8h] [ebp-30h]
  int v139; // [esp+Ch] [ebp-2Ch]
  float *v140; // [esp+Ch] [ebp-2Ch]
  int v141; // [esp+Ch] [ebp-2Ch]
  int v142; // [esp+Ch] [ebp-2Ch]
  char *v143; // [esp+Ch] [ebp-2Ch]
  int v144; // [esp+Ch] [ebp-2Ch]
  int v145; // [esp+10h] [ebp-28h]
  int v146; // [esp+10h] [ebp-28h]
  int v147; // [esp+10h] [ebp-28h]
  float *v148; // [esp+10h] [ebp-28h]
  int v149; // [esp+10h] [ebp-28h]
  float *v150; // [esp+10h] [ebp-28h]
  int v151; // [esp+14h] [ebp-24h]
  int v152; // [esp+18h] [ebp-20h]
  float *v153; // [esp+18h] [ebp-20h]
  int v154; // [esp+18h] [ebp-20h]
  float *v155; // [esp+1Ch] [ebp-1Ch]
  unsigned int v156; // [esp+1Ch] [ebp-1Ch]
  float *v157; // [esp+1Ch] [ebp-1Ch]
  int v158; // [esp+1Ch] [ebp-1Ch]
  float v159; // [esp+1Ch] [ebp-1Ch]
  float *v160; // [esp+20h] [ebp-18h]
  int v161; // [esp+20h] [ebp-18h]
  float *v162; // [esp+20h] [ebp-18h]
  int v163; // [esp+20h] [ebp-18h]
  int v164; // [esp+20h] [ebp-18h]
  float *v165; // [esp+20h] [ebp-18h]
  float v166; // [esp+24h] [ebp-14h]
  int v167; // [esp+24h] [ebp-14h]
  float v168; // [esp+28h] [ebp-10h]
  int v169; // [esp+28h] [ebp-10h]
  _DWORD *v170; // [esp+2Ch] [ebp-Ch]
  float *v171; // [esp+2Ch] [ebp-Ch]
  int v172; // [esp+2Ch] [ebp-Ch]
  float *v173; // [esp+2Ch] [ebp-Ch]
  int v174; // [esp+2Ch] [ebp-Ch]
  int v175; // [esp+2Ch] [ebp-Ch]
  int v176; // [esp+30h] [ebp-8h]
  float *v177; // [esp+30h] [ebp-8h]
  int v178; // [esp+30h] [ebp-8h]
  _DWORD *v179; // [esp+30h] [ebp-8h]
  float *v180; // [esp+34h] [ebp-4h]
  int v181; // [esp+34h] [ebp-4h]
  int v182; // [esp+34h] [ebp-4h]
  _DWORD *v183; // [esp+34h] [ebp-4h]
  int v184; // [esp+3Ch] [ebp+4h]
  unsigned int v185; // [esp+3Ch] [ebp+4h]
  int v186; // [esp+3Ch] [ebp+4h]
  int v187; // [esp+40h] [ebp+8h]
  int v188; // [esp+40h] [ebp+8h]
  float *v189; // [esp+40h] [ebp+8h]
  int v190; // [esp+48h] [ebp+10h]
  _DWORD *v191; // [esp+48h] [ebp+10h]
  int v192; // [esp+48h] [ebp+10h]
  int v193; // [esp+48h] [ebp+10h]
  int v194; // [esp+4Ch] [ebp+14h]
  float v195; // [esp+50h] [ebp+18h]
  int v196; // [esp+50h] [ebp+18h]
  int v197; // [esp+50h] [ebp+18h]
  int *v198; // [esp+50h] [ebp+18h]
  int v199; // [esp+50h] [ebp+18h]
  int v200; // [esp+50h] [ebp+18h]
  float *v201; // [esp+50h] [ebp+18h]
  _DWORD *v202; // [esp+54h] [ebp+1Ch]
  int v203; // [esp+54h] [ebp+1Ch]
  int v204; // [esp+54h] [ebp+1Ch]
  int v205; // [esp+58h] [ebp+20h]
  int v206; // [esp+5Ch] [ebp+24h]
  _DWORD *v207; // [esp+5Ch] [ebp+24h]
  int v208; // [esp+5Ch] [ebp+24h]
  int v209; // [esp+5Ch] [ebp+24h]
  int v210; // [esp+60h] [ebp+28h]
  unsigned int v211; // [esp+60h] [ebp+28h]
  int v212; // [esp+60h] [ebp+28h]
  float *v213; // [esp+60h] [ebp+28h]
  int v214; // [esp+60h] [ebp+28h]
  float *v215; // [esp+60h] [ebp+28h]
  _DWORD *v216; // [esp+60h] [ebp+28h]
  float *v217; // [esp+60h] [ebp+28h]
  int v218; // [esp+60h] [ebp+28h]

  v11 = flt_502E08 / (double)a2;
  v132 = (a2 + 1) >> 1;
  v151 = (a1 - 1) >> 1;
  v184 = a3 * a1;
  v168 = cos(v11);
  v138 = a2 * a1;
  v166 = sin(v11);
  if ( a1 != 1 )
  {
    v12 = a4;
    if ( a4 > 0 )
    {
      v13 = a9;
      do
      {
        *v13 = *(_DWORD *)((char *)v13 + a7 - (char *)a9);
        ++v13;
        --v12;
      }
      while ( v12 );
    }
    v14 = a2;
    if ( a2 <= 1 )
    {
      v16 = a3;
    }
    else
    {
      v15 = a8;
      v16 = a3;
      v145 = a2 - 1;
      do
      {
        v15 += v184;
        v170 = v15;
        if ( a3 > 0 )
        {
          v17 = a3;
          do
          {
            *v15 = *(_DWORD *)((char *)v15 + (char *)a6 - (char *)a8);
            v15 += a1;
            --v17;
          }
          while ( v17 );
          v15 = v170;
          v14 = a2;
        }
        --v145;
      }
      while ( v145 );
    }
    if ( v151 <= v16 )
    {
      if ( v14 > 1 )
      {
        v27 = (float *)a8;
        v28 = 4 * a1;
        v29 = (float *)(a10 - 4 * a1 - 4);
        v30 = a6 - 1;
        v147 = a2 - 1;
        do
        {
          v29 = (float *)((char *)v29 + v28);
          v27 += v184;
          v30 += v184;
          v155 = v29;
          v140 = v27;
          if ( a1 > 2 )
          {
            v31 = v30;
            v211 = (unsigned int)(a1 - 1) >> 1;
            do
            {
              v29 += 2;
              v27 += 2;
              v31 += 2;
              v160 = v27;
              v171 = v31;
              if ( a3 > 0 )
              {
                v32 = a3;
                do
                {
                  *(float *)((char *)v31 + (char *)a8 - (char *)a6) = *v31 * *(v29 - 1) + v31[1] * *v29;
                  v33 = v31[1] * *(v29 - 1);
                  v34 = *v29 * *v31;
                  v31 = (float *)((char *)v31 + v28);
                  *v27 = v33 - v34;
                  v27 = (float *)((char *)v27 + v28);
                  --v32;
                }
                while ( v32 );
                v31 = v171;
                v27 = v160;
              }
              --v211;
            }
            while ( v211 );
            v29 = v155;
            v27 = v140;
          }
          --v147;
        }
        while ( v147 );
      }
    }
    else if ( v14 > 1 )
    {
      v18 = 0;
      v133 = v14 - 1;
      v210 = a10 - 4 * a1 - 4;
      do
      {
        v18 += 4 * v184;
        v19 = v18 - 4 * a1;
        v210 += 4 * a1;
        if ( v16 > 0 )
        {
          v146 = v16;
          v20 = (float *)((char *)a8 + v19);
          v139 = (int)a6 + v19 - 4;
          do
          {
            v20 += a1;
            v139 += 4 * a1;
            if ( a1 > 2 )
            {
              v21 = (float *)v210;
              v22 = (float *)v139;
              v23 = v20;
              v24 = (unsigned int)(a1 - 1) >> 1;
              do
              {
                v25 = v22[3] * v21[2];
                v26 = v22[2];
                v22 += 2;
                v21 += 2;
                v23 += 2;
                --v24;
                *(float *)((char *)v22 + (char *)a8 - (char *)a6) = v25 + v26 * *(v21 - 1);
                *v23 = *(v21 - 1) * v22[1] - *v22 * *v21;
              }
              while ( v24 );
              v16 = a3;
            }
            --v146;
          }
          while ( v146 );
        }
        --v133;
      }
      while ( v133 );
    }
    v35 = a2 * v184;
    v36 = 0;
    if ( v151 >= a3 )
    {
      if ( v132 > 1 )
      {
        v47 = a6;
        v48 = &a6[v35];
        v49 = (float *)&a8[v35 - 1];
        v50 = (float *)(a8 - 1);
        v142 = v132 - 1;
        do
        {
          v47 += v184;
          v50 += v184;
          v48 -= v184;
          v49 -= v184;
          v177 = v47;
          v135 = v50;
          v180 = v48;
          v173 = v49;
          if ( a3 > 0 )
          {
            v51 = v48;
            v162 = v47;
            v213 = v50;
            v157 = v48;
            v153 = v49;
            v149 = a3;
            do
            {
              if ( a1 > 2 )
              {
                v52 = v213;
                v53 = (char *)a6 - (char *)a8;
                v54 = (unsigned int)(a1 - 1) >> 1;
                do
                {
                  v55 = v49[2] + v52[2];
                  v49 += 2;
                  v52 += 2;
                  v47 += 2;
                  v51 += 2;
                  --v54;
                  *(float *)((char *)v52 + v53) = v55;
                  *(float *)((char *)v49 + v53) = v52[1] - v49[1];
                  *v47 = v49[1] + v52[1];
                  *v51 = *v49 - *v52;
                }
                while ( v54 );
              }
              v47 = &v162[a1];
              v213 += a1;
              v51 = &v157[a1];
              v49 = &v153[a1];
              v162 = v47;
              v56 = v149 == 1;
              v157 = v51;
              v153 = v49;
              --v149;
            }
            while ( !v56 );
            v47 = v177;
            v49 = v173;
            v50 = v135;
            v48 = v180;
          }
          --v142;
        }
        while ( v142 );
      }
    }
    else if ( v132 > 1 )
    {
      v161 = v132 - 1;
      do
      {
        v36 += v184;
        v35 -= v184;
        v141 = v36;
        v212 = v35;
        if ( a1 > 2 )
        {
          v37 = v35 - v36;
          v38 = 4 * v36;
          v39 = 4 * a1;
          v176 = v37;
          v156 = (unsigned int)(a1 - 1) >> 1;
          do
          {
            v36 += 2;
            v38 += 8;
            v40 = v38 - v39;
            v134 = v36;
            v172 = v38;
            if ( a3 > 0 )
            {
              v41 = v37 + v36 - a1;
              v42 = (float *)((char *)a6 + v40);
              v148 = &a6[v41];
              v43 = (char *)a6 - (char *)a8;
              v44 = (float *)((char *)a8 + v40 - 4);
              v45 = (float *)&a8[v41 - 1];
              v152 = a3;
              do
              {
                v46 = v44[a1] + v45[a1];
                v44 = (float *)((char *)v44 + v39);
                v45 = (float *)((char *)v45 + v39);
                v42 = (float *)((char *)v42 + v39);
                *(float *)((char *)v44 + v43) = v46;
                v148 = (float *)((char *)v148 + v39);
                *(float *)((char *)v45 + v43) = v44[1] - v45[1];
                *v42 = v45[1] + v44[1];
                *v148 = *v45 - *v44;
                --v152;
              }
              while ( v152 );
              v38 = v172;
              v37 = v176;
              v36 = v134;
            }
            --v156;
          }
          while ( v156 );
          v36 = v141;
          v35 = v212;
        }
        --v161;
      }
      while ( v161 );
    }
  }
  v57 = a4;
  if ( a4 > 0 )
  {
    v58 = a7;
    v59 = a4;
    do
    {
      *v58 = *(_DWORD *)((char *)v58 + (char *)a9 - a7);
      ++v58;
      --v59;
    }
    while ( v59 );
  }
  v60 = (a2 + 1) >> 1;
  v61 = a4 * a2;
  v62 = 0;
  v214 = a4 * a2;
  if ( v132 > 1 )
  {
    v63 = v184;
    v64 = 0;
    v158 = v132 - 1;
    while ( 1 )
    {
      v62 += v63;
      v181 = 4 * v63 + v64;
      v214 -= v63;
      if ( a3 > 0 )
      {
        v65 = (float *)((char *)a8 + v181 - 4 * a1);
        v66 = (char *)a6 - (char *)a8;
        v67 = (float *)&a8[v214 - a1];
        v163 = a3;
        do
        {
          v67 += a1;
          v65 += a1;
          v56 = v163-- == 1;
          *(float *)((char *)v65 + v66) = *v67 + *v65;
          *(float *)((char *)v67 + v66) = *v67 - *v65;
        }
        while ( !v56 );
        v63 = v184;
      }
      if ( !--v158 )
        break;
      v64 = v181;
    }
    v61 = a4 * a2;
    v60 = (a2 + 1) >> 1;
  }
  v68 = 0.0;
  v164 = a4 * (a2 - 1);
  if ( v60 > 1 )
  {
    v69 = 1.0;
    v136 = (float *)a9;
    v215 = (float *)&a9[v61];
    v174 = v60 - 1;
    do
    {
      v70 = 4 * v57;
      v195 = v69 * v168 - v68 * v166;
      v136 += v57;
      v215 -= v57;
      v68 = v69 * v166 + v68 * v168;
      v69 = v195;
      if ( v57 > 0 )
      {
        v71 = (float *)a7;
        v72 = v215;
        v73 = a4;
        v74 = (float *)&a7[4 * v164];
        v75 = v136;
        v76 = (float *)&a7[v70];
        do
        {
          v77 = v195 * *v76++;
          ++v75;
          ++v74;
          ++v72;
          v78 = v77 + *v71++;
          --v73;
          *(v75 - 1) = v78;
          *(v72 - 1) = v68 * *(v74 - 1);
        }
        while ( v73 );
        v57 = a4;
      }
      v79 = v68;
      if ( v132 > 2 )
      {
        v80 = v195;
        v154 = v132 - 2;
        v150 = (float *)&a7[4 * v57];
        v143 = &a7[4 * v164];
        do
        {
          v81 = &v150[v57];
          v159 = v80 * v195 - v79 * v68;
          v150 = v81;
          v143 -= 4 * v57;
          v79 = v79 * v195 + v80 * v68;
          v80 = v159;
          if ( v57 > 0 )
          {
            v82 = v215;
            v83 = v143;
            v84 = v136;
            v85 = v57;
            do
            {
              v86 = v159 * *v81++;
              ++v84;
              v83 += 4;
              ++v82;
              --v85;
              *(v84 - 1) = v86 + *(v84 - 1);
              *(v82 - 1) = v79 * *((float *)v83 - 1) + *(v82 - 1);
            }
            while ( v85 );
          }
          --v154;
        }
        while ( v154 );
      }
      --v174;
    }
    while ( v174 );
  }
  if ( v132 > 1 )
  {
    v87 = (float *)a7;
    v196 = v132 - 1;
    do
    {
      v87 += v57;
      if ( v57 > 0 )
      {
        v88 = (float *)a9;
        v89 = v87;
        v90 = v57;
        do
        {
          v91 = *v89++ + *v88++;
          --v90;
          *(v88 - 1) = v91;
        }
        while ( v90 );
      }
      --v196;
    }
    while ( v196 );
  }
  v92 = a3;
  v93 = a5;
  if ( a1 >= a3 )
  {
    if ( a3 > 0 )
    {
      v190 = a3;
      v198 = a8;
      v99 = a5;
      v206 = v138;
      v202 = a5;
      do
      {
        if ( a1 > 0 )
        {
          v100 = v198;
          v101 = a1;
          do
          {
            v102 = *v100++;
            *v99++ = v102;
            --v101;
          }
          while ( v101 );
        }
        v99 = &v202[v206];
        v56 = v190 == 1;
        v198 += a1;
        v202 = (_DWORD *)((char *)v202 + v206 * 4);
        --v190;
      }
      while ( !v56 );
    }
  }
  else if ( a1 > 0 )
  {
    v94 = a8;
    v95 = (char *)a5 - (char *)a8;
    v197 = a1;
    do
    {
      if ( a3 > 0 )
      {
        v96 = (_DWORD *)((char *)v94 + v95);
        v97 = v94;
        v98 = a3;
        do
        {
          *v96 = *v97;
          v97 += a1;
          v96 += v138;
          --v98;
        }
        while ( v98 );
        v95 = (char *)a5 - (char *)a8;
        v93 = a5;
      }
      ++v94;
      --v197;
    }
    while ( v197 );
  }
  v103 = v184;
  result = a2 * v184;
  v182 = a2 * v184;
  if ( v132 > 1 )
  {
    v105 = a8;
    v106 = &a8[result];
    v107 = v93;
    v187 = v132 - 1;
    do
    {
      v107 += 2 * a1;
      v105 += v103;
      v106 -= v103;
      v191 = v107;
      v207 = v105;
      v216 = v106;
      if ( a3 > 0 )
      {
        v108 = 4 * a1;
        v199 = a3;
        do
        {
          v109 = *v105;
          v105 = (_DWORD *)((char *)v105 + v108);
          *(v107 - 1) = v109;
          *v107 = *v106;
          v107 += v138;
          v106 = (_DWORD *)((char *)v106 + v108);
          --v199;
        }
        while ( v199 );
        v107 = v191;
        v105 = v207;
        v106 = v216;
        v103 = v184;
      }
      --v187;
    }
    while ( v187 );
    result = v182;
    v93 = a5;
  }
  if ( a1 != 1 )
  {
    if ( v151 >= a3 )
    {
      if ( v132 > 1 )
      {
        v194 = 8 * a1;
        v121 = 4 * v103;
        v122 = a8 + 2;
        v205 = (int)&a8[result + 2];
        result = (int)(v93 + 2);
        v123 = v93 - 2;
        v218 = v121;
        v209 = v132 - 1;
        do
        {
          v122 = (_DWORD *)((char *)v122 + v121);
          v123 = (_DWORD *)((char *)v123 + v194);
          result += v194;
          v183 = v123;
          v124 = v205 - v121;
          v186 = result;
          v179 = v122;
          v205 -= v121;
          if ( v92 > 0 )
          {
            v189 = (float *)v122;
            v201 = (float *)v123;
            v125 = result;
            v204 = v124;
            v193 = v92;
            do
            {
              if ( a1 > 2 )
              {
                v126 = v189;
                v127 = v204;
                v128 = v201;
                v129 = v125;
                v130 = (unsigned int)(a1 - 1) >> 1;
                do
                {
                  v131 = *(v126 - 1) + *(float *)(v127 - 4);
                  v129 += 8;
                  v127 += 8;
                  v126 += 2;
                  v128 -= 2;
                  --v130;
                  *(float *)(v129 - 12) = v131;
                  v128[1] = *(v126 - 3) - *(float *)(v127 - 12);
                  *(float *)(v129 - 8) = *(v126 - 2) + *(float *)(v127 - 8);
                  v128[2] = *(float *)(v127 - 8) - *(v126 - 2);
                }
                while ( v130 );
              }
              v125 += 4 * v138;
              v201 += v138;
              v189 += a1;
              v56 = v193 == 1;
              v204 += 4 * a1;
              --v193;
            }
            while ( !v56 );
            v92 = a3;
            v121 = v218;
            result = v186;
            v123 = v183;
            v122 = v179;
          }
          --v209;
        }
        while ( v209 );
      }
    }
    else
    {
      v110 = -a1;
      v137 = 0;
      if ( v132 > 1 )
      {
        v111 = 0;
        v200 = 4 * result;
        v112 = 4 * v184;
        v113 = -2;
        v169 = 4 * v184;
        v114 = (float *)(v93 - 2);
        v175 = v132 - 1;
        while ( 1 )
        {
          v111 += v112;
          v113 += 2 * a1;
          v144 = 2 * a1 + v110;
          v114 += 2 * a1;
          v167 = v113;
          v137 += 2 * a1;
          v165 = v114;
          v178 = v111;
          v200 -= v112;
          if ( a1 > 2 )
          {
            v188 = v113;
            v115 = v111 + 8;
            v203 = 4;
            v208 = v111 + 8;
            v217 = v114;
            v185 = (unsigned int)(a1 - 1) >> 1;
            do
            {
              if ( a3 > 0 )
              {
                v116 = 4 * v138;
                v117 = (float *)((char *)a8 + v115);
                v118 = (float *)((char *)a8 + v200 + v115 - v111);
                v119 = (float *)&a5[v137 + v203 + v188 - v144 - a1];
                v192 = a3;
                do
                {
                  *(v119 - 1) = *(v117 - 1) + *(v118 - 1);
                  *(v114 - 1) = *(v117 - 1) - *(v118 - 1);
                  *v119 = *v118 + *v117;
                  v120 = *v118 - *v117;
                  v118 += a1;
                  v117 += a1;
                  v119 = (float *)((char *)v119 + v116);
                  *v114 = v120;
                  v114 = (float *)((char *)v114 + v116);
                  --v192;
                }
                while ( v192 );
                v111 = v178;
                v115 = v208;
              }
              v115 += 8;
              v114 = v217 - 2;
              v56 = v185 == 1;
              v208 = v115;
              v217 -= 2;
              v188 -= 2;
              v203 += 4;
              --v185;
            }
            while ( !v56 );
            v112 = v169;
            v113 = v167;
            v114 = v165;
          }
          result = --v175;
          if ( !v175 )
            break;
          v110 = v144;
        }
      }
    }
  }
  return result;
}

// ===== sub_4CDE20 @ 0x004CDE20..0x004CDE57 =====
int __cdecl sub_4CDE20(_DWORD *a1, int a2)
{
  void *v2; // eax
  int v4; // [esp-18h] [ebp-20h]

  *a1 = a2;
  a1[1] = calloc(3 * a2, 4u);
  v2 = calloc(0x20u, 4u);
  v4 = a1[1];
  a1[2] = v2;
  return sub_4CDE60(a2, v4, v2);
}

// ===== sub_4CDE60 @ 0x004CDE60..0x004CDE80 =====
int __cdecl sub_4CDE60(int a1, int a2, int a3)
{
  int result; // eax

  result = a1;
  if ( a1 != 1 )
    return sub_4CDE80(a1, a2 + 4 * a1, a3);
  return result;
}

// ===== sub_4CDE80 @ 0x004CDE80..0x004CE00B =====
int *__cdecl sub_4CDE80(int a1, int a2, int *a3)
{
  int v3; // ecx
  int v4; // esi
  int v5; // edi
  int *v6; // ebx
  int v7; // eax
  int *v8; // ebx
  int *v9; // ecx
  int v10; // edx
  int *result; // eax
  int v12; // edi
  int v13; // ebx
  int v14; // ebp
  int v15; // esi
  int v16; // eax
  int v17; // edi
  double v18; // st7
  int v19; // ecx
  unsigned int v20; // edx
  long double v21; // st6
  bool v22; // zf
  _DWORD *v23; // [esp+10h] [ebp-14h]
  int *v24; // [esp+14h] [ebp-10h]
  int v25; // [esp+14h] [ebp-10h]
  int v26; // [esp+18h] [ebp-Ch]
  float v27; // [esp+1Ch] [ebp-8h]
  float v28; // [esp+20h] [ebp-4h]
  int v29; // [esp+30h] [ebp+Ch]

  v3 = a1;
  v4 = 0;
  v5 = 0;
  v6 = (int *)((char *)&unk_502DF0 - 4);
  do
  {
LABEL_2:
    v24 = ++v6;
    if ( (int)v6 >= (int)&flt_502E00 )
      v4 += 2;
    else
      v4 = *v6;
    v7 = v3 / v4;
  }
  while ( v3 % v4 );
  v8 = &a3[v5];
  while ( 1 )
  {
    ++v8;
    ++v5;
    v3 = v7;
    v8[1] = v4;
    if ( v4 == 2 && v5 != 1 )
    {
      if ( v5 > 1 )
      {
        v9 = v8;
        v10 = v5 - 1;
        do
        {
          v9[1] = *v9;
          --v9;
          --v10;
        }
        while ( v10 );
        v3 = v7;
      }
      a3[2] = 2;
    }
    if ( v7 == 1 )
      break;
    v7 /= v4;
    if ( v3 != v4 * v7 )
    {
      v6 = v24;
      goto LABEL_2;
    }
  }
  result = a3;
  *a3 = a1;
  a3[1] = v5;
  v12 = v5 - 1;
  v29 = 0;
  v13 = 1;
  v28 = flt_502E00 / (double)a1;
  if ( v12 > 0 )
  {
    v26 = v12;
    v23 = result + 2;
    do
    {
      v14 = 0;
      v15 = v13 * *v23;
      v16 = a1 / v15;
      if ( *v23 - 1 > 0 )
      {
        v25 = *v23 - 1;
        v17 = a2 + 4 * v29;
        v29 += v16 * v25;
        do
        {
          v14 += v13;
          v18 = 0.0;
          if ( v16 > 2 )
          {
            v19 = v17;
            v20 = (unsigned int)(v16 - 1) >> 1;
            do
            {
              v18 = v18 + 1.0;
              v19 += 8;
              --v20;
              v27 = (double)v14 * v28;
              v21 = v18 * v27;
              *(float *)(v19 - 8) = cos(v21);
              *(float *)(v19 - 4) = sin(v21);
            }
            while ( v20 );
          }
          v17 += 4 * v16;
          --v25;
        }
        while ( v25 );
      }
      result = (int *)(v26 - 1);
      v22 = v26 == 1;
      v13 = v15;
      ++v23;
      --v26;
    }
    while ( !v22 );
  }
  return result;
}

// ===== sub_4CE010 @ 0x004CE010..0x004CE045 =====
int __cdecl sub_4CE010(int a1)
{
  int result; // eax

  if ( a1 )
  {
    if ( *(_DWORD *)(a1 + 4) )
      free(*(void **)(a1 + 4));
    if ( *(_DWORD *)(a1 + 8) )
      free(*(void **)(a1 + 8));
    result = 0;
    *(_DWORD *)a1 = 0;
    *(_DWORD *)(a1 + 4) = 0;
    *(_DWORD *)(a1 + 8) = 0;
  }
  return result;
}

// ===== sub_4CE050 @ 0x004CE050..0x004CE0BE =====
void *__cdecl sub_4CE050(int a1, int Count)
{
  void *result; // eax
  signed int v3; // ecx
  float *v4; // edx
  double v5; // st6
  long double v6; // st5
  int v7; // [esp+8h] [ebp+4h]

  result = calloc(Count, 4u);
  if ( a1 )
  {
    free(result);
    return 0;
  }
  else
  {
    v3 = 0;
    v7 = 0;
    if ( Count > 0 )
    {
      v4 = (float *)result;
      do
      {
        v5 = (double)v7;
        ++v3;
        ++v4;
        v7 = v3;
        v6 = sin((v5 + 0.5) / (double)Count * 3.1415927 * 0.5);
        *(v4 - 1) = sin(v6 * v6 * 1.5707964);
      }
      while ( v3 < Count );
    }
  }
  return result;
}

// ===== sub_4CE0C0 @ 0x004CE0C0..0x004CE200 =====
int __cdecl sub_4CE0C0(char *a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // esi
  int v7; // ebp
  int v8; // kr00_4
  int v9; // ecx
  int v10; // ebx
  int v11; // edx
  int v12; // edi
  int result; // eax
  int v14; // edi
  float *v15; // ecx
  bool v16; // zf
  int v17; // ecx
  int v18; // edx
  int v19; // [esp+14h] [ebp-4h]
  int v20; // [esp+2Ch] [ebp+14h]
  int v21; // [esp+2Ch] [ebp+14h]
  int v22; // [esp+30h] [ebp+18h]

  v6 = a5 != 0 ? a4 : 0;
  v7 = a5 != 0 ? a6 : 0;
  v19 = *(_DWORD *)(a3 + 4 * a5);
  v20 = *(_DWORD *)(a3 + 4 * v7);
  v8 = *(_DWORD *)(a3 + 4 * v6);
  v9 = v19 / 4 - v8 / 4;
  v10 = v19 / 4 + v19 / 2 - v20 / 4;
  v11 = v20 / 2;
  v12 = v9 + v8 / 2;
  v22 = v20 / 2 + v10;
  result = 0;
  if ( v9 > 0 )
  {
    memset(a1, 0, 4 * v9);
    result = v19 / 4 - v8 / 4;
    v12 = v9 + v8 / 2;
  }
  if ( result < v12 )
  {
    v14 = v12 - result;
    v15 = (float *)&a1[4 * result];
    result = 0;
    v21 = v14;
    do
    {
      ++v15;
      result += 4;
      v16 = v21-- == 1;
      *(v15 - 1) = *(float *)(result + *(_DWORD *)(a2 + 4 * v6) - 4) * *(v15 - 1);
    }
    while ( !v16 );
  }
  if ( v10 < v22 )
  {
    v17 = v22 - v10;
    v18 = 4 * v11 - 4;
    result = (int)&a1[4 * v10];
    v10 = v22;
    do
    {
      result += 4;
      v18 -= 4;
      --v17;
      *(float *)(result - 4) = *(float *)(v18 + *(_DWORD *)(a2 + 4 * v7) + 4) * *(float *)(result - 4);
    }
    while ( v17 );
  }
  if ( v10 < v19 )
  {
    result = 0;
    memset(&a1[4 * v10], 0, 4 * (v19 - v10));
  }
  return result;
}

// ===== sub_4CE200 @ 0x004CE200..0x004CE3D5 =====
int __cdecl sub_4CE200(int a1, int a2)
{
  int v3; // esi
  long double v4; // st7
  float *v5; // ebp
  int v6; // ebx
  float *v7; // eax
  float *v8; // ecx
  long double v9; // st5
  double v10; // st5
  long double v11; // st5
  int v12; // edx
  float *v13; // edi
  int v14; // ecx
  long double v15; // st6
  int v16; // ebx
  int v17; // esi
  int result; // eax
  _DWORD *v19; // ebp
  int v20; // edi
  char v21; // cl
  int v22; // edx
  int v23; // [esp+18h] [ebp-10h]
  int v24; // [esp+18h] [ebp-10h]
  int v25; // [esp+1Ch] [ebp-Ch]
  int v26; // [esp+1Ch] [ebp-Ch]
  _DWORD *v27; // [esp+20h] [ebp-8h]
  int v28; // [esp+24h] [ebp-4h]
  float v29; // [esp+30h] [ebp+8h]

  v3 = a2 / 4;
  v27 = malloc(4 * (a2 / 4));
  v4 = (double)a2;
  v5 = (float *)malloc(4 * (a2 / 4 + a2));
  v6 = a2 >> 1;
  v29 = v4;
  v28 = (__int64)floor(__FYL2X__(v4, 0.6931471805599453094) / __FYL2X__(2.0, 0.6931471805599453094) + 0.5);
  *(_DWORD *)(a1 + 4) = v28;
  *(_DWORD *)a1 = a2;
  *(_DWORD *)(a1 + 8) = v5;
  *(_DWORD *)(a1 + 12) = v27;
  if ( v3 > 0 )
  {
    v23 = 0;
    v7 = v5;
    v8 = &v5[v6];
    v25 = 1;
    do
    {
      v7 += 2;
      v9 = (double)v23 * (3.1415927 / v29);
      v8 += 2;
      --v3;
      v23 += 4;
      *(v7 - 2) = cos(v9);
      *(v7 - 1) = -sin(v9);
      v10 = (double)v25;
      v25 += 2;
      v11 = v10 * (3.1415927 / (double)(2 * a2));
      *(v8 - 2) = cos(v11);
      *(v8 - 1) = sin(v11);
    }
    while ( v3 );
  }
  v12 = a2 / 8;
  v24 = a2 / 8;
  if ( a2 / 8 > 0 )
  {
    v26 = 2;
    v13 = &v5[a2];
    v14 = v12;
    do
    {
      v13 += 2;
      --v14;
      v15 = (double)v26 * (3.1415927 / v29);
      v26 += 4;
      *(v13 - 2) = cos(v15) * 0.5;
      *(v13 - 1) = sin(v15) * -0.5;
    }
    while ( v14 );
  }
  v16 = 0;
  v17 = 1 << (v28 - 2);
  result = (1 << (v28 - 1)) - 1;
  if ( v12 > 0 )
  {
    v19 = v27;
    do
    {
      v20 = 0;
      v21 = 0;
      if ( v17 )
      {
        v22 = 1 << (v28 - 2);
        do
        {
          if ( (v22 & v16) != 0 )
            v20 |= 1 << v21;
          v22 = v17 >> ++v21;
        }
        while ( v17 >> v21 );
        v12 = v24;
      }
      v19 += 2;
      ++v16;
      *(v19 - 2) = (result & ~v20) - 1;
      *(v19 - 1) = v20;
    }
    while ( v16 < v12 );
  }
  *(float *)(a1 + 16) = 4.0 / v29;
  return result;
}

// ===== sub_4CE3E0 @ 0x004CE3E0..0x004CE41B =====
int __cdecl sub_4CE3E0(int a1)
{
  int result; // eax

  if ( a1 )
  {
    if ( *(_DWORD *)(a1 + 8) )
      free(*(void **)(a1 + 8));
    if ( *(_DWORD *)(a1 + 12) )
      free(*(void **)(a1 + 12));
    result = 0;
    *(_DWORD *)a1 = 0;
    *(_DWORD *)(a1 + 4) = 0;
    *(_DWORD *)(a1 + 8) = 0;
    *(_DWORD *)(a1 + 12) = 0;
    *(_DWORD *)(a1 + 16) = 0;
  }
  return result;
}

// ===== sub_4CE420 @ 0x004CE420..0x004CE921 =====
float *__cdecl sub_4CE420(int *a1, unsigned int a2, int a3)
{
  int *v3; // ebx
  int v4; // edi
  int v5; // eax
  float *v6; // ecx
  float *v7; // ebp
  int v8; // eax
  float *v9; // edx
  double v10; // st7
  float *v11; // edx
  float *v12; // ecx
  float *v13; // eax
  double v14; // st7
  double v15; // st6
  unsigned int v16; // ebp
  int v17; // eax
  float *v18; // edx
  float *v19; // eax
  float *v20; // ecx
  double v21; // st7
  double v22; // st6
  double v23; // st7
  double v24; // st6
  double v25; // st7
  double v26; // st6
  double v27; // st7
  double v28; // st6
  double v29; // st5
  double v30; // st4
  int v31; // eax
  int v32; // ecx
  int v33; // ebx
  int v34; // esi
  bool v35; // zf
  unsigned int v36; // edi
  _DWORD *v37; // esi
  int v38; // ebp
  float *v39; // edx
  float *v40; // eax
  float *v41; // ecx
  int v42; // ebx
  double v43; // st7
  double v44; // st6
  float *v45; // edi
  float *v46; // ebx
  double v47; // st6
  double v48; // st7
  double v49; // st6
  int v50; // edi
  int v51; // ebx
  double v52; // st7
  double v53; // st6
  float *v54; // edi
  float *v55; // ebx
  double v56; // st6
  double v57; // st7
  double v58; // st6
  float *v59; // edx
  float *v60; // ecx
  float *v61; // esi
  int v62; // eax
  double v63; // st7
  double v64; // st6
  float *v65; // eax
  int v66; // ecx
  float *v67; // edx
  double v68; // st7
  double v69; // st7
  double v70; // st7
  double v71; // st7
  float *v72; // ecx
  float *result; // eax
  int v74; // esi
  int v75; // [esp+10h] [ebp-20h]
  float v76; // [esp+10h] [ebp-20h]
  float v77; // [esp+10h] [ebp-20h]
  int v78; // [esp+14h] [ebp-1Ch]
  int v79; // [esp+18h] [ebp-18h]
  unsigned int v80; // [esp+1Ch] [ebp-14h]
  int v81; // [esp+20h] [ebp-10h]
  float *v82; // [esp+24h] [ebp-Ch]
  int v83; // [esp+28h] [ebp-8h]
  int v84; // [esp+2Ch] [ebp-4h]
  int v85; // [esp+38h] [ebp+8h]
  int v86; // [esp+38h] [ebp+8h]
  float v87; // [esp+38h] [ebp+8h]
  float v88; // [esp+38h] [ebp+8h]

  v3 = a1;
  v4 = *a1 >> 1;
  v5 = *a1 >> 2;
  v84 = v4;
  v6 = (float *)(a2 + 4 * v4 - 28);
  v7 = (float *)(a3 + 4 * (v5 + v4));
  v79 = 4 * v5;
  v8 = a1[2] + 4 * v5;
  v82 = v7;
  v9 = v7;
  do
  {
    v10 = v6[2] * *(float *)(v8 + 12);
    v9 -= 4;
    v6 -= 8;
    v8 += 16;
    *v9 = -v10 - v6[8] * *(float *)(v8 - 8);
    v9[1] = v6[8] * *(float *)(v8 - 4) - v6[10] * *(float *)(v8 - 8);
    v9[2] = -(*(float *)(v8 - 12) * v6[14]) - v6[12] * *(float *)(v8 - 16);
    v9[3] = *(float *)(v8 - 12) * v6[12] - *(float *)(v8 - 16) * v6[14];
  }
  while ( (unsigned int)v6 >= a2 );
  v11 = v7;
  v12 = (float *)(a2 + 4 * v4 - 32);
  v13 = (float *)(a1[2] + v79);
  do
  {
    v14 = v12[4] * *(v13 - 1);
    v15 = *(v13 - 2) * v12[6];
    v13 -= 4;
    v12 -= 8;
    v11 += 4;
    *(v11 - 4) = v14 + v15;
    *(v11 - 3) = v12[12] * v13[2] - v13[3] * v12[14];
    *(v11 - 2) = v12[10] * *v13 + v12[8] * v13[1];
    *(v11 - 1) = v12[8] * *v13 - v13[1] * v12[10];
  }
  while ( (unsigned int)v12 >= a2 );
  v81 = a1[2];
  v83 = 4 * v4;
  v16 = a3 + 4 * v4;
  v80 = v16;
  v17 = a1[1] - 6;
  v85 = v17;
  if ( v17 > 0 )
  {
    v18 = (float *)(a1[2] + 16);
    v19 = (float *)(v16 + 4 * (v4 >> 1) - 32);
    v20 = &v19[v4 - (v4 >> 1) + 7];
    do
    {
      v21 = *(v20 - 1) - v19[6];
      v22 = *v20 - v19[7];
      *(v20 - 1) = *(v20 - 1) + v19[6];
      *v20 = v19[7] + *v20;
      v19[6] = v22 * *(v18 - 3) + v21 * *(v18 - 4);
      v19[7] = v22 * *(v18 - 4) - v21 * *(v18 - 3);
      v23 = *(v20 - 3) - v19[4];
      v24 = *(v20 - 2) - v19[5];
      *(v20 - 3) = v19[4] + *(v20 - 3);
      *(v20 - 2) = *(v20 - 2) + v19[5];
      v19[4] = v24 * v18[1] + v23 * *v18;
      v19[5] = v24 * *v18 - v23 * v18[1];
      v25 = *(v20 - 5) - v19[2];
      v26 = *(v20 - 4) - v19[3];
      *(v20 - 5) = v19[2] + *(v20 - 5);
      *(v20 - 4) = *(v20 - 4) + v19[3];
      v19[2] = v26 * v18[5] + v25 * v18[4];
      v19[3] = v26 * v18[4] - v25 * v18[5];
      v27 = *(v20 - 7) - *v19;
      v28 = *(v20 - 6) - v19[1];
      *(v20 - 7) = *(v20 - 7) + *v19;
      *(v20 - 6) = v19[1] + *(v20 - 6);
      v29 = v28 * v18[9];
      v30 = v27 * v18[8];
      v19 -= 8;
      v20 -= 8;
      v18 += 16;
      v19[8] = v29 + v30;
      v19[9] = v28 * *(v18 - 8) - v27 * *(v18 - 7);
    }
    while ( (unsigned int)v19 >= v16 );
    v17 = v85;
  }
  v31 = v17 - 1;
  v32 = 1;
  v75 = 1;
  if ( v31 > 0 )
  {
    v78 = v31;
    do
    {
      if ( 1 << v32 > 0 )
      {
        v33 = 4 << v32;
        v34 = v4 >> v32;
        v86 = 1 << v32;
        do
        {
          sub_4CEEC0(v81, v16, v34, v33);
          v16 += 4 * v34;
          --v86;
        }
        while ( v86 );
        v3 = a1;
        v16 = a3 + 4 * v4;
        v32 = v75;
      }
      ++v32;
      v35 = v78 == 1;
      v75 = v32;
      --v78;
    }
    while ( !v35 );
  }
  if ( v4 > 0 )
  {
    v36 = (unsigned int)(v4 + 31) >> 5;
    do
    {
      sub_4CE930(v16);
      v16 += 128;
      --v36;
    }
    while ( v36 );
  }
  v37 = (_DWORD *)v3[3];
  v38 = a3 + 4 * (*v3 >> 1);
  v39 = (float *)(a3 + 8);
  v40 = (float *)(v3[2] + 4 * *v3);
  v41 = (float *)(v38 + 12);
  do
  {
    v42 = v37[1];
    v41 -= 4;
    v43 = *(float *)(v38 + 4 * *v37 + 4) - *(float *)(v38 + 4 * v42 + 4);
    v44 = *(float *)(v38 + 4 * v42);
    v45 = (float *)(v38 + 4 * *v37);
    v46 = (float *)(v38 + 4 * v42);
    v47 = v44 + *v45;
    v87 = v47 * *v40 + v43 * v40[1];
    v48 = v47 * v40[1] - v43 * *v40;
    v49 = (v46[1] + v45[1]) * 0.5;
    v76 = (*v45 - *v46) * 0.5;
    *(v39 - 2) = v87 + v49;
    *(v41 - 1) = v49 - v87;
    *(v39 - 1) = v48 + v76;
    *v41 = v48 - v76;
    v50 = v37[2];
    v51 = v37[3];
    v52 = *(float *)(v38 + 4 * v50 + 4) - *(float *)(v38 + 4 * v51 + 4);
    v53 = *(float *)(v38 + 4 * v51);
    v54 = (float *)(v38 + 4 * v50);
    v55 = (float *)(v38 + 4 * v51);
    v56 = v53 + *v54;
    v88 = v56 * v40[2] + v52 * v40[3];
    v57 = v56 * v40[3] - v52 * v40[2];
    v58 = (v55[1] + v54[1]) * 0.5;
    v77 = (*v54 - *v55) * 0.5;
    *v39 = v88 + v58;
    *(v41 - 3) = v58 - v88;
    v39[1] = v57 + v77;
    v39 += 4;
    v40 += 4;
    v37 += 4;
    *(v41 - 2) = v57 - v77;
  }
  while ( v39 - 2 < v41 - 3 );
  v59 = v82;
  v60 = (float *)(a1[2] + v83);
  v61 = v82;
  v62 = a3 + 12;
  do
  {
    v63 = v60[1] * *(float *)(v62 - 12);
    v64 = *(float *)(v62 - 8) * *v60;
    v59 -= 4;
    v62 += 32;
    v61 += 4;
    v60 += 8;
    v59[3] = v63 - v64;
    *(v61 - 4) = -(*(v60 - 7) * *(float *)(v62 - 40) + *(float *)(v62 - 44) * *(v60 - 8));
    v59[2] = *(float *)(v62 - 36) * *(v60 - 5) - *(float *)(v62 - 32) * *(v60 - 6);
    *(v61 - 3) = -(*(float *)(v62 - 32) * *(v60 - 5) + *(float *)(v62 - 36) * *(v60 - 6));
    v59[1] = *(float *)(v62 - 28) * *(v60 - 3) - *(float *)(v62 - 24) * *(v60 - 4);
    *(v61 - 2) = -(*(float *)(v62 - 24) * *(v60 - 3) + *(float *)(v62 - 28) * *(v60 - 4));
    *v59 = *(float *)(v62 - 20) * *(v60 - 1) - *(float *)(v62 - 16) * *(v60 - 2);
    *(v61 - 1) = -(*(float *)(v62 - 16) * *(v60 - 1) + *(float *)(v62 - 20) * *(v60 - 2));
  }
  while ( v62 - 12 < (unsigned int)v59 );
  v65 = v82;
  v66 = v79 + a3 + 8;
  v67 = &v82[2 - v84];
  do
  {
    v68 = *(v65 - 1);
    v65 -= 4;
    v67 -= 4;
    v66 += 16;
    v67[1] = v68;
    *(float *)(v66 - 24) = -v68;
    v69 = v65[2];
    *v67 = v65[2];
    *(float *)(v66 - 20) = -v69;
    v70 = v65[1];
    *(v67 - 1) = v65[1];
    *(float *)(v66 - 16) = -v70;
    v71 = *v65;
    *(v67 - 2) = *v65;
    *(float *)(v66 - 12) = -v71;
  }
  while ( v66 - 8 < (unsigned int)v65 );
  v72 = v82;
  result = v82;
  do
  {
    v74 = *((_DWORD *)v72 + 3);
    result -= 4;
    v72 += 4;
    *(_DWORD *)result = v74;
    result[1] = *(v72 - 2);
    result[2] = *(v72 - 3);
    result[3] = *(v72 - 4);
  }
  while ( (unsigned int)result > v80 );
  return result;
}

// ===== sub_4CE930 @ 0x004CE930..0x004CEE25 =====
int __cdecl sub_4CE930(float *a1)
{
  double v2; // st7
  double v3; // st6
  double v4; // st7
  double v5; // st6
  double v6; // st7
  double v7; // st6
  double v8; // st7
  double v9; // st6
  double v10; // st7
  double v11; // st6
  double v12; // st7
  double v13; // st6
  double v14; // st7
  double v15; // st6
  double v16; // st7
  double v17; // st6
  double v18; // st7
  double v19; // st7
  double v20; // st6
  double v21; // st7
  double v22; // st6
  double v23; // st7
  double v24; // st7
  double v25; // st6
  double v26; // st6
  double v27; // st7
  double v28; // st6
  double v29; // st7
  double v30; // st6
  double v31; // st7
  double v32; // st7
  double v33; // st6
  double v34; // st7
  double v35; // st6
  double v36; // st7
  double v37; // st6
  double v38; // st7
  double v39; // st6
  double v40; // st7
  double v41; // st7
  double v42; // st6
  double v43; // st6
  double v44; // st7
  double v45; // st6
  float v47; // [esp+0h] [ebp-8h]
  float v48; // [esp+0h] [ebp-8h]
  float v49; // [esp+0h] [ebp-8h]
  float v50; // [esp+0h] [ebp-8h]
  float v51; // [esp+4h] [ebp-4h]
  float v52; // [esp+4h] [ebp-4h]
  float v53; // [esp+4h] [ebp-4h]
  float v54; // [esp+4h] [ebp-4h]
  float v55; // [esp+4h] [ebp-4h]
  float v56; // [esp+4h] [ebp-4h]
  float v57; // [esp+Ch] [ebp+4h]
  float v58; // [esp+Ch] [ebp+4h]
  float v59; // [esp+Ch] [ebp+4h]
  float v60; // [esp+Ch] [ebp+4h]
  float v61; // [esp+Ch] [ebp+4h]
  float v62; // [esp+Ch] [ebp+4h]
  float v63; // [esp+Ch] [ebp+4h]

  v2 = a1[30] - a1[14];
  v57 = a1[31] - a1[15];
  a1[30] = a1[14] + a1[30];
  v3 = a1[31] + a1[15];
  a1[15] = v57;
  a1[31] = v3;
  a1[14] = v2;
  v4 = a1[28] - a1[12];
  v5 = a1[29] - a1[13];
  a1[28] = a1[12] + a1[28];
  a1[29] = a1[29] + a1[13];
  a1[12] = v4 * 0.9238795 - v5 * 0.38268343;
  a1[13] = v5 * 0.9238795 + v4 * 0.38268343;
  v6 = a1[26] - a1[10];
  v7 = a1[27] - a1[11];
  a1[26] = a1[10] + a1[26];
  a1[27] = a1[27] + a1[11];
  a1[10] = (v6 - v7) * 0.70710677;
  a1[11] = (v7 + v6) * 0.70710677;
  v8 = a1[24] - a1[8];
  v9 = a1[25] - a1[9];
  a1[24] = a1[8] + a1[24];
  a1[25] = a1[25] + a1[9];
  a1[8] = v8 * 0.38268343 - v9 * 0.9238795;
  a1[9] = v9 * 0.38268343 + v8 * 0.9238795;
  v10 = a1[22] - a1[6];
  v11 = a1[7] - a1[23];
  a1[22] = a1[6] + a1[22];
  a1[23] = a1[7] + a1[23];
  a1[6] = v11;
  a1[7] = v10;
  v12 = a1[4] - a1[20];
  v13 = a1[5] - a1[21];
  a1[20] = a1[20] + a1[4];
  a1[21] = a1[5] + a1[21];
  a1[4] = v13 * 0.9238795 + v12 * 0.38268343;
  a1[5] = v13 * 0.38268343 - v12 * 0.9238795;
  v14 = a1[2] - a1[18];
  v15 = a1[3] - a1[19];
  a1[18] = a1[18] + a1[2];
  a1[19] = a1[19] + a1[3];
  a1[2] = (v15 + v14) * 0.70710677;
  a1[3] = (v15 - v14) * 0.70710677;
  v16 = *a1 - a1[16];
  v17 = a1[1] - a1[17];
  a1[16] = a1[16] + *a1;
  a1[17] = a1[17] + a1[1];
  v47 = v17 * 0.38268343 + v16 * 0.9238795;
  v18 = v17 * 0.9238795 - v16 * 0.38268343;
  v51 = v18 - a1[9];
  v58 = v47 - a1[8];
  a1[8] = v47 + a1[8];
  a1[9] = v18 + a1[9];
  *a1 = (v58 + v51) * 0.70710677;
  a1[1] = (v51 - v58) * 0.70710677;
  v19 = a1[3] - a1[11];
  v59 = a1[10] - a1[2];
  a1[10] = a1[2] + a1[10];
  v20 = a1[3] + a1[11];
  a1[3] = v59;
  a1[11] = v20;
  a1[2] = v19;
  v21 = a1[12] - a1[4];
  v22 = a1[13] - a1[5];
  a1[12] = a1[12] + a1[4];
  a1[13] = a1[5] + a1[13];
  a1[4] = (v21 - v22) * 0.70710677;
  a1[5] = (v22 + v21) * 0.70710677;
  v23 = a1[14] - a1[6];
  v60 = a1[15] - a1[7];
  a1[14] = a1[14] + a1[6];
  a1[15] = a1[7] + a1[15];
  v52 = v23 + a1[2];
  v24 = v23 - a1[2];
  v25 = *a1 + a1[4];
  v48 = a1[4] - *a1;
  a1[6] = v25 + v52;
  a1[4] = v52 - v25;
  v26 = a1[5] - a1[1];
  v53 = v60 - a1[3];
  *a1 = v24 + v26;
  a1[2] = v24 - v26;
  v27 = a1[5] + a1[1];
  v28 = v60 + a1[3];
  a1[3] = v48 + v53;
  a1[1] = v53 - v48;
  a1[7] = v28 + v27;
  a1[5] = v28 - v27;
  v29 = a1[10] + a1[14];
  v61 = a1[14] - a1[10];
  v30 = a1[8] + a1[12];
  v49 = a1[12] - a1[8];
  a1[14] = v30 + v29;
  a1[12] = v29 - v30;
  v31 = a1[13] - a1[9];
  v54 = a1[15] - a1[11];
  a1[8] = v61 + v31;
  a1[10] = v61 - v31;
  v32 = a1[9] + a1[13];
  v33 = a1[11] + a1[15];
  a1[11] = v49 + v54;
  a1[9] = v54 - v49;
  a1[15] = v33 + v32;
  a1[13] = v33 - v32;
  v34 = a1[17] - a1[25];
  v35 = a1[16] - a1[24];
  a1[24] = a1[16] + a1[24];
  a1[25] = a1[25] + a1[17];
  a1[16] = (v35 + v34) * 0.70710677;
  a1[17] = (v34 - v35) * 0.70710677;
  v36 = a1[19] - a1[27];
  v62 = a1[26] - a1[18];
  a1[26] = a1[18] + a1[26];
  v37 = a1[27] + a1[19];
  a1[19] = v62;
  a1[27] = v37;
  a1[18] = v36;
  v38 = a1[28] - a1[20];
  v39 = a1[29] - a1[21];
  a1[28] = a1[20] + a1[28];
  a1[29] = a1[21] + a1[29];
  a1[20] = (v38 - v39) * 0.70710677;
  a1[21] = (v39 + v38) * 0.70710677;
  v40 = a1[30] - a1[22];
  v63 = a1[31] - a1[23];
  a1[30] = a1[22] + a1[30];
  a1[31] = a1[23] + a1[31];
  v55 = v40 + a1[18];
  v41 = v40 - a1[18];
  v42 = a1[20] + a1[16];
  v50 = a1[20] - a1[16];
  a1[22] = v42 + v55;
  a1[20] = v55 - v42;
  v43 = a1[21] - a1[17];
  v56 = v63 - a1[19];
  a1[16] = v41 + v43;
  a1[18] = v41 - v43;
  v44 = a1[17] + a1[21];
  v45 = v63 + a1[19];
  a1[19] = v50 + v56;
  a1[17] = v56 - v50;
  a1[23] = v45 + v44;
  a1[21] = v45 - v44;
  return sub_4CEE30(a1 + 24);
}

// ===== sub_4CEE30 @ 0x004CEE30..0x004CEEBC =====
float *__cdecl sub_4CEE30(float *a1)
{
  float *result; // eax
  double v2; // st7
  double v3; // st6
  double v4; // st7
  double v5; // st7
  double v6; // st6
  float v7; // [esp+0h] [ebp-8h]
  float v8; // [esp+4h] [ebp-4h]
  float v9; // [esp+Ch] [ebp+4h]

  result = a1;
  v2 = a1[2] + a1[6];
  v9 = a1[6] - a1[2];
  v3 = *result + result[4];
  v8 = result[4] - *result;
  result[6] = v3 + v2;
  result[4] = v2 - v3;
  v4 = result[5] - result[1];
  v7 = result[7] - result[3];
  *result = v9 + v4;
  result[2] = v9 - v4;
  v5 = result[1] + result[5];
  v6 = result[3] + result[7];
  result[3] = v8 + v7;
  result[1] = v7 - v8;
  result[7] = v6 + v5;
  result[5] = v6 - v5;
  return result;
}

// ===== sub_4CEEC0 @ 0x004CEEC0..0x004CEFD7 =====
float *__cdecl sub_4CEEC0(float *a1, unsigned int a2, int a3, int a4)
{
  int v4; // esi
  float *result; // eax
  float *v7; // ecx
  double v8; // st7
  double v9; // st6
  double v10; // rt0
  double v11; // st6
  float *v12; // edx
  double v13; // st7
  double v14; // st6
  double v15; // rt1
  double v16; // st6
  float *v17; // edx
  double v18; // st7
  double v19; // st6
  double v20; // rt2
  double v21; // st6
  float *v22; // edx
  double v23; // st7
  double v24; // st6
  double v25; // rtt
  double v26; // st6

  v4 = a4;
  result = (float *)(a2 + 4 * (a3 >> 1) - 32);
  v7 = &result[a3 - (a3 >> 1) + 7];
  do
  {
    v8 = *(v7 - 1) - result[6];
    v9 = *v7 - result[7];
    *(v7 - 1) = *(v7 - 1) + result[6];
    *v7 = result[7] + *v7;
    result[6] = v9 * a1[1] + v8 * *a1;
    v10 = v9 * *a1;
    v11 = v8 * a1[1];
    v12 = &a1[v4];
    result[7] = v10 - v11;
    v13 = *(v7 - 3) - result[4];
    v14 = *(v7 - 2) - result[5];
    *(v7 - 3) = result[4] + *(v7 - 3);
    *(v7 - 2) = *(v7 - 2) + result[5];
    result[4] = v14 * v12[1] + v13 * *v12;
    v15 = v14 * *v12;
    v16 = v13 * v12[1];
    v17 = &v12[v4];
    result[5] = v15 - v16;
    v18 = *(v7 - 5) - result[2];
    v19 = *(v7 - 4) - result[3];
    *(v7 - 5) = result[2] + *(v7 - 5);
    *(v7 - 4) = result[3] + *(v7 - 4);
    result[2] = v19 * v17[1] + v18 * *v17;
    v20 = v19 * *v17;
    v21 = v18 * v17[1];
    v22 = &v17[v4];
    result[3] = v20 - v21;
    v23 = *(v7 - 7) - *result;
    v24 = *(v7 - 6) - result[1];
    *(v7 - 7) = *result + *(v7 - 7);
    *(v7 - 6) = *(v7 - 6) + result[1];
    result -= 8;
    v7 -= 8;
    result[8] = v24 * v22[1] + v23 * *v22;
    v25 = v24 * *v22;
    v26 = v23 * v22[1];
    a1 = &v22[v4];
    result[9] = v25 - v26;
  }
  while ( (unsigned int)result >= a2 );
  return result;
}

// ===== sub_4CEFE0 @ 0x004CEFE0..0x004CF460 =====
float *__cdecl sub_4CEFE0(int a1, int a2, int a3)
{
  int v3; // esi
  int v4; // edi
  int v5; // eax
  int v6; // ebx
  int v7; // esi
  int v8; // edi
  void *v9; // esp
  float *v10; // ecx
  float *v11; // eax
  float *v12; // ebx
  double v13; // st7
  double v14; // st6
  double v15; // st6
  float *v16; // edx
  int v17; // ebx
  float v18; // edi
  float *v19; // edx
  unsigned int v20; // ebx
  float *v21; // edi
  double v22; // st7
  double v23; // st6
  double v24; // st6
  float *v25; // ebx
  float *v26; // edi
  float *v27; // ecx
  unsigned int v28; // ebx
  double v29; // st7
  double v30; // st7
  double v31; // st6
  int *v32; // edx
  float *v33; // edi
  int v34; // eax
  float *v35; // edx
  float *v36; // eax
  float *v37; // ecx
  double v38; // st7
  double v39; // st6
  double v40; // st7
  double v41; // st6
  double v42; // st7
  double v43; // st6
  double v44; // st7
  double v45; // st6
  double v46; // st5
  double v47; // st4
  int v48; // eax
  int v49; // ecx
  int v50; // eax
  int v51; // edi
  bool v52; // zf
  unsigned int v53; // esi
  _DWORD *v54; // esi
  int v55; // edi
  float *v56; // eax
  float *v57; // edx
  float *v58; // edi
  float *v59; // ecx
  float *v60; // edi
  float *v61; // ebx
  double v62; // st7
  double v63; // st6
  double v64; // st7
  double v65; // st6
  double v66; // st5
  unsigned int v67; // edi
  float *v68; // edi
  float *v69; // ebx
  double v70; // st7
  double v71; // st6
  double v72; // st7
  double v73; // st6
  int v74; // esi
  int v75; // ebx
  float *result; // eax
  float *v77; // edx
  float *v78; // ecx
  double v79; // st7
  double v80; // st6
  _DWORD v81[3]; // [esp+0h] [ebp-2Ch] BYREF
  int v82; // [esp+Ch] [ebp-20h]
  int v83; // [esp+10h] [ebp-1Ch]
  int i; // [esp+14h] [ebp-18h]
  float *v85; // [esp+18h] [ebp-14h]
  unsigned int v86; // [esp+1Ch] [ebp-10h]
  float *v87; // [esp+20h] [ebp-Ch]
  float v88; // [esp+24h] [ebp-8h]
  float *v89; // [esp+28h] [ebp-4h]
  int v90; // [esp+38h] [ebp+Ch]
  int v91; // [esp+38h] [ebp+Ch]
  float *v92; // [esp+38h] [ebp+Ch]
  float v93; // [esp+38h] [ebp+Ch]
  float *v94; // [esp+38h] [ebp+Ch]
  float v95; // [esp+38h] [ebp+Ch]

  v3 = *(_DWORD *)a1;
  v4 = *(_DWORD *)a1;
  i = 4 * *(_DWORD *)a1;
  v5 = i + 3;
  v6 = v3 >> 2;
  LOBYTE(v5) = (i + 3) & 0xFC;
  v82 = v3 >> 2;
  v7 = v3 >> 1;
  v8 = v4 >> 3;
  v9 = alloca(v5);
  v85 = (float *)v81;
  v89 = (float *)&v81[v7];
  v83 = 4 * v7;
  v10 = (float *)(a2 + 4 * (v7 + v6));
  v88 = 0.0;
  v11 = (float *)(*(_DWORD *)(a1 + 8) + 4 * v7);
  v12 = v10 + 1;
  if ( v8 > 0 )
  {
    v87 = v89;
    v86 = (unsigned int)(v8 + 1) >> 1;
    LODWORD(v88) = 2 * v86;
    do
    {
      v13 = *(v10 - 2) + *v12;
      v14 = v12[2];
      v10 -= 4;
      v11 -= 2;
      v12 += 4;
      v15 = v14 + *v10;
      v16 = v87 + 2;
      v87 = v16;
      *(v16 - 2) = v15 * v11[1] + v13 * *v11;
      *(v16 - 1) = v15 * *v11 - v13 * v11[1];
      --v86;
    }
    while ( v86 );
  }
  v17 = v7 - v8;
  v18 = v88;
  v19 = (float *)(a2 + 4);
  v86 = v17;
  if ( SLODWORD(v88) < v17 )
  {
    v87 = &v89[LODWORD(v88)];
    v20 = (v86 - LODWORD(v88) + 1) >> 1;
    LODWORD(v88) += 2 * v20;
    v21 = &v89[LODWORD(v18)];
    do
    {
      v22 = *(v10 - 2) - *v19;
      v23 = *(v10 - 4);
      v10 -= 4;
      v11 -= 2;
      v24 = v23 - v19[2];
      v19 += 4;
      v21 += 2;
      --v20;
      *(v21 - 2) = v24 * v11[1] + v22 * *v11;
      *(v21 - 1) = v24 * *v11 - v22 * v11[1];
    }
    while ( v20 );
  }
  v25 = v89;
  v26 = (float *)(i + a2);
  if ( SLODWORD(v88) < v7 )
  {
    v27 = &v89[LODWORD(v88)];
    v28 = (unsigned int)(v7 - LODWORD(v88) + 1) >> 1;
    do
    {
      v29 = *(v26 - 2);
      v26 -= 4;
      v11 -= 2;
      v30 = -v29 - *v19;
      v31 = -*v26 - v19[2];
      v19 += 4;
      v27 += 2;
      --v28;
      *(v27 - 2) = v31 * v11[1] + v30 * *v11;
      *(v27 - 1) = v31 * *v11 - v30 * v11[1];
    }
    while ( v28 );
    v25 = v89;
  }
  v32 = (int *)a1;
  v33 = *(float **)(a1 + 8);
  v34 = *(_DWORD *)(a1 + 4) - 6;
  v88 = *(float *)&v33;
  v90 = v34;
  if ( v34 > 0 )
  {
    v35 = v33 + 4;
    v36 = &v25[(v7 >> 1) - 8];
    v37 = &v36[v7 - (v7 >> 1) + 7];
    do
    {
      v38 = *(v37 - 1) - v36[6];
      v39 = *v37 - v36[7];
      *(v37 - 1) = v36[6] + *(v37 - 1);
      *v37 = v36[7] + *v37;
      v36[6] = v38 * *(v35 - 4) + v39 * *(v35 - 3);
      v36[7] = v39 * *(v35 - 4) - v38 * *(v35 - 3);
      v40 = *(v37 - 3) - v36[4];
      v41 = *(v37 - 2) - v36[5];
      *(v37 - 3) = v36[4] + *(v37 - 3);
      *(v37 - 2) = *(v37 - 2) + v36[5];
      v36[4] = v41 * v35[1] + v40 * *v35;
      v36[5] = v41 * *v35 - v40 * v35[1];
      v42 = *(v37 - 5) - v36[2];
      v43 = *(v37 - 4) - v36[3];
      *(v37 - 5) = v36[2] + *(v37 - 5);
      *(v37 - 4) = v36[3] + *(v37 - 4);
      v36[2] = v43 * v35[5] + v42 * v35[4];
      v36[3] = v43 * v35[4] - v42 * v35[5];
      v44 = *(v37 - 7) - *v36;
      v45 = *(v37 - 6) - v36[1];
      *(v37 - 7) = *(v37 - 7) + *v36;
      *(v37 - 6) = v36[1] + *(v37 - 6);
      v46 = v45 * v35[9];
      v47 = v44 * v35[8];
      v36 -= 8;
      v37 -= 8;
      v35 += 16;
      v36[8] = v46 + v47;
      v36[9] = v45 * *(v35 - 8) - v44 * *(v35 - 7);
    }
    while ( v36 >= v25 );
    v32 = (int *)a1;
    v34 = v90;
  }
  v48 = v34 - 1;
  v49 = 1;
  v86 = 1;
  if ( v48 > 0 )
  {
    v87 = (float *)v48;
    do
    {
      if ( 1 << v49 > 0 )
      {
        v50 = 4 << v49;
        v51 = v7 >> v49;
        v91 = 1 << v49;
        for ( i = 4 << v49; ; v50 = i )
        {
          sub_4CEEC0((float *)LODWORD(v88), (unsigned int)v25, v51, v50);
          v25 += v51;
          if ( !--v91 )
            break;
        }
        v49 = v86;
        v25 = v89;
      }
      ++v49;
      v52 = v87 == (float *)1;
      v86 = v49;
      v87 = (float *)((char *)v87 - 1);
    }
    while ( !v52 );
    v32 = (int *)a1;
  }
  if ( v7 > 0 )
  {
    v53 = (unsigned int)(v7 + 31) >> 5;
    do
    {
      sub_4CE930(v25);
      v25 += 32;
      --v53;
    }
    while ( v53 );
    v32 = (int *)a1;
  }
  v54 = (_DWORD *)v32[3];
  v55 = *v32 >> 1;
  v56 = (float *)(v32[2] + 4 * *v32);
  v57 = v85 + 2;
  v58 = &v85[v55];
  v86 = (unsigned int)v58;
  v59 = v58 + 3;
  while ( 1 )
  {
    v59 -= 4;
    v92 = &v58[*v54];
    v60 = &v58[v54[1]];
    v61 = v92;
    v62 = v92[1] - v60[1];
    v63 = *v60 + *v92;
    v93 = v62 * v56[1] + v63 * *v56;
    v64 = v63 * v56[1] - v62 * *v56;
    v65 = (v60[1] + v61[1]) * 0.5;
    v66 = *v61 - *v60;
    v67 = v86;
    v88 = v66 * 0.5;
    *(v57 - 2) = v93 + v65;
    *(v59 - 1) = v65 - v93;
    *(v57 - 1) = v64 + v88;
    *v59 = v64 - v88;
    v94 = (float *)(v67 + 4 * v54[2]);
    v68 = (float *)(v67 + 4 * v54[3]);
    v69 = v94;
    v70 = v94[1] - v68[1];
    v71 = *v68 + *v94;
    v95 = v71 * v56[2] + v70 * v56[3];
    v72 = v71 * v56[3] - v70 * v56[2];
    v73 = (v68[1] + v69[1]) * 0.5;
    v88 = (*v69 - *v68) * 0.5;
    *v57 = v95 + v73;
    v57 += 4;
    v56 += 4;
    v54 += 4;
    *(v59 - 3) = v73 - v95;
    *(v57 - 3) = v72 + v88;
    *(v59 - 2) = v72 - v88;
    if ( v57 - 2 >= v59 - 3 )
      break;
    v58 = (float *)v86;
  }
  v74 = a3;
  v75 = v82;
  result = (float *)(*(_DWORD *)(a1 + 8) + v83);
  v77 = (float *)(v83 + a3);
  if ( v82 > 0 )
  {
    v78 = v85;
    do
    {
      v79 = *result * *v78;
      v80 = result[1] * v78[1];
      --v77;
      v78 += 2;
      result += 2;
      v74 += 4;
      --v75;
      *(float *)(v74 - 4) = (v79 + v80) * *(float *)(a1 + 16);
      *v77 = (*(result - 1) * *(v78 - 2) - *(result - 2) * *(v78 - 1)) * *(float *)(a1 + 16);
    }
    while ( v75 );
  }
  return result;
}

// ===== sub_4CF460 @ 0x004CF460..0x004CF491 =====
int __cdecl sub_4CF460(_DWORD *a1, int a2, int a3)
{
  *a1 = 0;
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  a1[4] = 0;
  a1[3] = a2;
  a1[4] = a3;
  return sub_4CDE20(a1, 2 * a2);
}

// ===== sub_4CF4A0 @ 0x004CF4A0..0x004CF4B0 =====
int __cdecl sub_4CF4A0(int a1)
{
  int result; // eax

  result = a1;
  if ( a1 )
    return sub_4CE010(a1);
  return result;
}

// ===== sub_4CF4B0 @ 0x004CF4B0..0x004CF4CF =====
void __cdecl sub_4CF4B0(void *Block)
{
  if ( Block )
  {
    memset(Block, 0, 0xC88u);
    free(Block);
  }
}

// ===== sub_4CF4D0 @ 0x004CF4D0..0x004CF615 =====
int __cdecl sub_4CF4D0(int a1, int *a2, int a3)
{
  int v3; // ebp
  _DWORD *v4; // ebx
  int v5; // eax
  int v6; // eax
  int v7; // ebx
  _DWORD *v8; // ebp
  int result; // eax
  int v10; // ebp
  _DWORD *v11; // ebx

  if ( *a2 <= 1 )
  {
    sub_4D4B20(a3, 0, 1);
  }
  else
  {
    sub_4D4B20(a3, 1, 1);
    sub_4D4B20(a3, *a2 - 1, 4);
  }
  if ( a2[289] <= 0 )
  {
    sub_4D4B20(a3, 0, 1);
  }
  else
  {
    sub_4D4B20(a3, 1, 1);
    sub_4D4B20(a3, a2[289] - 1, 8);
    v3 = 0;
    if ( a2[289] > 0 )
    {
      v4 = a2 + 546;
      do
      {
        v5 = sub_4D1C30(*(_DWORD *)(a1 + 4));
        sub_4D4B20(a3, *(v4 - 256), v5);
        v6 = sub_4D1C30(*(_DWORD *)(a1 + 4));
        sub_4D4B20(a3, *v4, v6);
        ++v3;
        ++v4;
      }
      while ( v3 < a2[289] );
    }
  }
  sub_4D4B20(a3, 0, 2);
  if ( *a2 > 1 )
  {
    v7 = 0;
    if ( *(int *)(a1 + 4) > 0 )
    {
      v8 = a2 + 1;
      do
      {
        sub_4D4B20(a3, *v8, 4);
        ++v7;
        ++v8;
      }
      while ( v7 < *(_DWORD *)(a1 + 4) );
    }
  }
  result = *a2;
  v10 = 0;
  if ( *a2 > 0 )
  {
    v11 = a2 + 273;
    do
    {
      sub_4D4B20(a3, 0, 8);
      sub_4D4B20(a3, *(v11 - 16), 8);
      sub_4D4B20(a3, *v11, 8);
      result = *a2;
      ++v10;
      ++v11;
    }
    while ( v10 < *a2 );
  }
  return result;
}

// ===== sub_4CF620 @ 0x004CF620..0x004CF7E8 =====
int *__cdecl sub_4CF620(int a1, int a2)
{
  int *v3; // ebx
  int v4; // ebp
  int v5; // eax
  int *v6; // ebp
  int v7; // eax
  int v8; // edi
  int v9; // eax
  int v10; // eax
  int v11; // ecx
  int v12; // ebp
  int *v13; // edi
  int v14; // eax
  _DWORD *i; // edi
  int v16; // eax
  int v17; // eax
  int v19; // [esp+10h] [ebp-4h]
  int v20; // [esp+1Ch] [ebp+8h]
  int v21; // [esp+1Ch] [ebp+8h]

  v3 = (int *)calloc(1u, 0xC88u);
  v4 = *(_DWORD *)(a1 + 28);
  memset(v3, 0, 0xC88u);
  v19 = v4;
  if ( sub_4D4D00(a2, 1) )
    *v3 = sub_4D4D00(a2, 4) + 1;
  else
    *v3 = 1;
  if ( sub_4D4D00(a2, 1) )
  {
    v5 = sub_4D4D00(a2, 8) + 1;
    v3[289] = v5;
    v20 = 0;
    if ( v5 > 0 )
    {
      v6 = v3 + 546;
      while ( 1 )
      {
        v7 = sub_4D1C30(*(_DWORD *)(a1 + 4));
        v8 = sub_4D4D00(a2, v7);
        *(v6 - 256) = v8;
        v9 = sub_4D1C30(*(_DWORD *)(a1 + 4));
        v10 = sub_4D4D00(a2, v9);
        *v6 = v10;
        if ( v8 < 0 )
          goto LABEL_27;
        if ( v10 < 0 )
          goto LABEL_27;
        if ( v8 == v10 )
          goto LABEL_27;
        v11 = *(_DWORD *)(a1 + 4);
        if ( v8 >= v11 || v10 >= v11 )
          goto LABEL_27;
        ++v6;
        if ( ++v20 >= v3[289] )
        {
          v4 = v19;
          break;
        }
      }
    }
  }
  if ( (int)sub_4D4D00(a2, 2) <= 0 )
  {
    if ( *v3 > 1 )
    {
      v12 = 0;
      if ( *(int *)(a1 + 4) > 0 )
      {
        v13 = v3 + 1;
        do
        {
          v14 = sub_4D4D00(a2, 4);
          *v13 = v14;
          if ( v14 >= *v3 )
            goto LABEL_27;
          ++v12;
          ++v13;
        }
        while ( v12 < *(_DWORD *)(a1 + 4) );
      }
      v4 = v19;
    }
    v21 = 0;
    if ( *v3 <= 0 )
      return v3;
    for ( i = v3 + 273; ; ++i )
    {
      sub_4D4D00(a2, 8);
      v16 = sub_4D4D00(a2, 8);
      *(i - 16) = v16;
      if ( v16 >= *(_DWORD *)(v4 + 16) )
        break;
      v17 = sub_4D4D00(a2, 8);
      *i = v17;
      if ( v17 >= *(_DWORD *)(v4 + 20) )
        break;
      if ( ++v21 >= *v3 )
        return v3;
    }
  }
LABEL_27:
  sub_4CF4B0(v3);
  return 0;
}

// ===== sub_4CF7F0 @ 0x004CF7F0..0x004D0165 =====
int __usercall sub_4CF7F0@<eax>(double a1@<st0>, int *a2)
{
  int v3; // eax
  int v4; // edi
  _DWORD *v5; // edx
  float *v6; // eax
  int v7; // ecx
  int v8; // ebx
  int v9; // ecx
  int v10; // eax
  void *v11; // esp
  int v12; // eax
  int v13; // edx
  int v14; // eax
  void *v15; // esp
  int *v16; // eax
  int *v17; // ebx
  int v18; // eax
  int v19; // eax
  int v20; // edx
  char *v21; // ebx
  int v22; // eax
  double v23; // st7
  double v24; // st7
  double v25; // st7
  float *v26; // ecx
  bool v27; // cc
  float *v28; // eax
  double v29; // st7
  int v30; // ecx
  double v31; // st7
  _DWORD **v32; // ebx
  int *v33; // ecx
  float v34; // eax
  _BYTE *v35; // eax
  int v36; // ecx
  _DWORD *v37; // edi
  float *v38; // edi
  float v39; // ecx
  int v40; // edi
  int v41; // eax
  int v42; // eax
  float *v43; // edx
  int v44; // eax
  _DWORD *v45; // ecx
  int v46; // edi
  int v47; // eax
  int v48; // ecx
  int k; // edi
  int v50; // eax
  int v51; // ecx
  int v52; // eax
  void *v53; // esp
  int v54; // eax
  void *v55; // esp
  int v56; // eax
  void *v57; // esp
  int v58; // eax
  void *v59; // esp
  float v60; // edx
  int *v61; // eax
  int v62; // eax
  int v63; // ecx
  float v64; // edi
  int v66; // ebx
  int v67; // eax
  void *v68; // esp
  int v69; // edi
  _DWORD *v70; // ebx
  int *v71; // edi
  int v72; // eax
  int v73; // ecx
  int v74; // ebx
  int v75; // eax
  int v76; // ecx
  int v77; // eax
  float v78; // ecx
  int *v79; // ecx
  int v80; // ebx
  int v81; // ecx
  int v82; // edi
  float v83; // eax
  int v84; // edx
  int v85; // eax
  int v86; // eax
  int v87; // ecx
  int v88; // [esp-18h] [ebp-90h]
  int v89; // [esp-14h] [ebp-8Ch]
  int v90; // [esp-10h] [ebp-88h]
  int *v91; // [esp-Ch] [ebp-84h]
  _BYTE v92[12]; // [esp+0h] [ebp-78h] BYREF
  _BYTE *v93; // [esp+Ch] [ebp-6Ch]
  int v94; // [esp+10h] [ebp-68h]
  int v95; // [esp+14h] [ebp-64h]
  int j; // [esp+18h] [ebp-60h]
  float v97; // [esp+1Ch] [ebp-5Ch]
  int v98; // [esp+20h] [ebp-58h]
  int v99; // [esp+24h] [ebp-54h]
  float v100; // [esp+28h] [ebp-50h]
  int v101; // [esp+2Ch] [ebp-4Ch] BYREF
  _BYTE *v102; // [esp+30h] [ebp-48h]
  int v103; // [esp+34h] [ebp-44h]
  float *v104; // [esp+38h] [ebp-40h]
  float v105; // [esp+3Ch] [ebp-3Ch]
  float v106; // [esp+40h] [ebp-38h]
  float *i; // [esp+44h] [ebp-34h]
  int v108; // [esp+48h] [ebp-30h]
  int *v109; // [esp+4Ch] [ebp-2Ch]
  int v110; // [esp+50h] [ebp-28h]
  float v111; // [esp+54h] [ebp-24h] BYREF
  int v112; // [esp+58h] [ebp-20h]
  float *v113; // [esp+5Ch] [ebp-1Ch]
  int v114; // [esp+60h] [ebp-18h]
  int v115; // [esp+64h] [ebp-14h]
  int v116; // [esp+68h] [ebp-10h]
  int v117; // [esp+6Ch] [ebp-Ch]
  _DWORD *v118; // [esp+70h] [ebp-8h]
  int *v119; // [esp+74h] [ebp-4h]
  int v120; // [esp+80h] [ebp+8h]
  int v121; // [esp+80h] [ebp+8h]
  int v122; // [esp+80h] [ebp+8h]
  int v123; // [esp+80h] [ebp+8h]
  int v124; // [esp+80h] [ebp+8h]
  int v125; // [esp+80h] [ebp+8h]

  v3 = a2[16];
  v4 = *(_DWORD *)(v3 + 4);
  v5 = *(_DWORD **)(v3 + 104);
  v6 = (float *)a2[26];
  v116 = v4;
  v7 = *(_DWORD *)(v4 + 28);
  v8 = 4 * *(_DWORD *)(v4 + 4);
  v104 = v6;
  v115 = v7;
  v9 = a2[9];
  v10 = v8 + 3;
  v118 = v5;
  LOBYTE(v10) = (v8 + 3) & 0xFC;
  v112 = v9;
  v11 = alloca(v10);
  v102 = v92;
  v12 = sub_4C7BE0(a2, v8);
  v13 = *(_DWORD *)(v4 + 4);
  v110 = v12;
  v98 = sub_4C7BE0(a2, 4 * v13);
  v103 = sub_4C7BE0(a2, 4 * *(_DWORD *)(v4 + 4));
  v14 = 4 * *(_DWORD *)(v4 + 4) + 3;
  v106 = v104[1];
  LOBYTE(v14) = v14 & 0xFC;
  v15 = alloca(v14);
  v99 = a2[7];
  v16 = *(int **)(v115 + 4 * v99 + 544);
  v97 = COERCE_FLOAT(v92);
  v119 = v16;
  v120 = 0;
  v17 = (int *)(v118[14] + 48 * (*((_DWORD *)v104 + 2) + (v99 != 0 ? 2 : 0)));
  a2[10] = v99;
  v18 = *(_DWORD *)(v4 + 4);
  v117 = (int)v17;
  if ( v18 > 0 )
  {
    v113 = (float *)v92;
    v19 = 4 * (v112 / 2);
    v114 = v19;
    v109 = (int *)(v110 - (_DWORD)v92);
    v100 = 4.0 / (double)v112;
    while ( 1 )
    {
      v20 = *a2;
      v101 = LODWORD(v100);
      v21 = *(char **)(v20 + 4 * v120);
      v22 = sub_4C7BE0(a2, v19);
      *(_DWORD *)((char *)v113 + (_DWORD)v109) = v22;
      v23 = sub_4D0170(&v101);
      v90 = a2[8];
      v89 = a2[7];
      v88 = a2[6];
      v105 = v23;
      sub_4CE0C0(v21, (int)(v118 + 1), v115, v88, v89, v90);
      sub_4CEFE0(*(_DWORD *)v118[a2[7] + 3], (int)v21, *(_DWORD *)((char *)v113 + (_DWORD)v109));
      sub_4CCA60(&v118[3 * a2[7] + 5], (int)v21);
      v24 = sub_4D0170(v21);
      v25 = v24 + v105;
      v26 = v113;
      v27 = v112 - 1 <= 1;
      v108 = 1;
      *(float *)v21 = v25;
      *v26 = v25;
      if ( !v27 )
      {
        v28 = (float *)(v21 + 4);
        for ( i = (float *)(v21 + 4); ; v28 = i )
        {
          v111 = v28[1] * v28[1] + *v28 * *v28;
          v29 = sub_4D0170(&v111);
          v30 = v108;
          v31 = v29 * 0.5 + v105;
          *(float *)&v21[4 * ((v108 + 1) >> 1)] = v31;
          if ( v31 > *v113 )
            *v113 = v31;
          v108 = v30 + 2;
          i += 2;
          if ( v30 + 2 >= v112 - 1 )
            break;
        }
        v26 = v113;
      }
      if ( *v26 > 0.0 )
        *v26 = 0.0;
      a1 = *v26;
      if ( a1 > v106 )
        v106 = *v26;
      v113 = v26 + 1;
      if ( ++v120 >= *(_DWORD *)(v4 + 4) )
        break;
      v19 = v114;
    }
    v17 = (int *)v117;
  }
  j = v112 / 2;
  v114 = 4 * (v112 / 2);
  i = (float *)sub_4C7BE0(a2, v114);
  v108 = sub_4C7BE0(a2, v114);
  v121 = 0;
  if ( *(int *)(v4 + 4) <= 0 )
  {
LABEL_33:
    v104[1] = v106;
    v122 = 4 * *(_DWORD *)(v4 + 4);
    v52 = v122 + 3;
    LOBYTE(v52) = (v122 + 3) & 0xFC;
    v53 = alloca(v52);
    v93 = v92;
    v54 = v122 + 3;
    LOBYTE(v54) = (v122 + 3) & 0xFC;
    v55 = alloca(v54);
    i = (float *)v92;
    v56 = v122 + 3;
    LOBYTE(v56) = (v122 + 3) & 0xFC;
    v57 = alloca(v56);
    v105 = COERCE_FLOAT(v92);
    v58 = v122 + 3;
    LOBYTE(v58) = (v122 + 3) & 0xFC;
    v59 = alloca(v58);
    v60 = COERCE_FLOAT(v92);
    v100 = COERCE_FLOAT(v92);
    if ( v119[289] )
    {
      v94 = sub_4CA5A0(a1, a2, v115 + 2868, (int)v17, (int)v119, v110);
      v61 = sub_4CA850(a2, v17, (int)v119, v94);
      v60 = v100;
      v113 = (float *)v61;
    }
    memset((void *)LODWORD(v60), 0, 4 * *(_DWORD *)(v4 + 4));
    if ( *(_DWORD *)(*(_DWORD *)(v117 + 4) + 500) )
    {
      v123 = 0;
      if ( *(int *)(v116 + 4) > 0 )
      {
        v62 = (unsigned int)(4 * v112) >> 1;
        v63 = v110 - LODWORD(v60);
        v95 = v62;
        v64 = v60;
        for ( j = v110 - LODWORD(v60); ; v63 = j )
        {
          v66 = *(_DWORD *)(v63 + LODWORD(v64));
          v67 = v62 + 3;
          LOBYTE(v67) = v67 & 0xFC;
          v68 = alloca(v67);
          v91 = (int *)v117;
          *(_DWORD *)LODWORD(v64) = v92;
          sub_4CA9E0(v91, v66, (int)v92);
          LODWORD(v64) += 4;
          if ( ++v123 >= *(_DWORD *)(v116 + 4) )
            break;
          v62 = v95;
        }
      }
    }
    v69 = sub_4CC9E0((int)a2) ? 0 : 7;
    v111 = *(float *)&v69;
    if ( v69 <= (sub_4CC9E0((int)a2) ? 14 : 7) )
    {
      v70 = a2 + 1;
      v101 = (int)&v104[v69 + 3];
      do
      {
        sub_4D4B20(v70, 0, 1);
        sub_4D4B20(v70, v99, v118[11]);
        if ( a2[7] )
        {
          sub_4D4B20(v70, a2[6], 1);
          sub_4D4B20(v70, a2[8], 1);
        }
        v124 = 0;
        if ( *(int *)(v116 + 4) > 0 )
        {
          v71 = (int *)v98;
          j = v103 - v98;
          v72 = v110 - v98;
          LODWORD(v106) = &v102[-v98];
          v104 = (float *)(v119 + 1);
          v95 = v110 - v98;
          v112 = LODWORD(v100) - v98;
          while ( 1 )
          {
            v109 = *(int **)((char *)v71 + v72);
            v73 = *a2;
            v97 = *v104;
            v74 = *(_DWORD *)(v73 + 4 * v124);
            v75 = sub_4C7BE0(a2, v114);
            v76 = j;
            v108 = v75;
            *v71 = v75;
            v77 = sub_4D2CE0(
                    a2,
                    *(_DWORD *)(v118[12] + 4 * v119[LODWORD(v97) + 257]),
                    *(_DWORD *)(*(int *)((char *)v71 + v76) + 4 * LODWORD(v111)),
                    v75);
            v78 = v111;
            *(int *)((char *)v71 + LODWORD(v106)) = v77;
            sub_4C98D0(
              (int *)v117,
              v109,
              v108,
              v74,
              *(_DWORD *)(v115 + 4 * (LODWORD(v78) + 12 * (a2[7] + 54) + 3 * (a2[7] + 54))));
            sub_4CAAB0((int *)v117, v74, (float *)(v74 + v114), *(int *)((char *)v71 + v112));
            ++v104;
            ++v71;
            if ( ++v124 >= *(_DWORD *)(v116 + 4) )
              break;
            v72 = v95;
          }
          *(float *)&v69 = v111;
        }
        v79 = v119;
        v80 = v115;
        if ( v119[289] )
        {
          sub_4CAD20(
            v69,
            v115 + 2868,
            (int *)v117,
            v119,
            *a2,
            v94,
            v113,
            v98,
            (int)v102,
            *(_DWORD *)(v115 + 4 * (v69 + 12 * (a2[7] + 54) + 3 * (a2[7] + 54))));
          v79 = v119;
        }
        v125 = 0;
        if ( *v79 > 0 )
        {
          v108 = (int)(v79 + 273);
          do
          {
            v81 = 0;
            v106 = 0.0;
            v82 = *(_DWORD *)v108;
            if ( *(int *)(v116 + 4) > 0 )
            {
              v83 = v105;
              v104 = (float *)(v119 + 1);
              v95 = (int)&v93[-LODWORD(v105)];
              j = (int)i - LODWORD(v105);
              do
              {
                if ( *(_DWORD *)v104 == v125 )
                {
                  v84 = (int)v102;
                  *(_DWORD *)LODWORD(v83) = 0;
                  if ( *(_DWORD *)(v84 + 4 * v81) )
                    *(_DWORD *)LODWORD(v83) = 1;
                  *(_DWORD *)(v95 + LODWORD(v83)) = *(_DWORD *)(*a2 + 4 * v81);
                  *(_DWORD *)(j + LODWORD(v83)) = v114 + *(_DWORD *)(*a2 + 4 * v81);
                  LODWORD(v83) += 4;
                  ++LODWORD(v106);
                }
                ++v81;
                ++v104;
              }
              while ( v81 < *(_DWORD *)(v116 + 4) );
              v80 = v115;
            }
            v85 = (*((int (__cdecl **)(int *, _DWORD, float *, float, float))*(&off_502DE0
                                                                             + *(_DWORD *)(v80 + 4 * v82 + 1312))
                   + 5))(
                    a2,
                    *(_DWORD *)(v118[13] + 4 * v82),
                    i,
                    COERCE_FLOAT(LODWORD(v105)),
                    COERCE_FLOAT(LODWORD(v106)));
            (*((void (__cdecl **)(int *, _DWORD, float *, _DWORD, float, float, int))*(&off_502DE0
                                                                                     + *(_DWORD *)(v80 + 4 * v82 + 1312))
             + 6))(
              a2,
              *(_DWORD *)(v118[13] + 4 * v82),
              i,
              0,
              COERCE_FLOAT(LODWORD(v105)),
              COERCE_FLOAT(LODWORD(v106)),
              v85);
            v108 += 4;
            ++v125;
          }
          while ( v125 < *v119 );
          *(float *)&v69 = v111;
        }
        v70 = a2 + 1;
        sub_4D4AA0(a2 + 1);
        v86 = sub_4D4DD0(a2 + 1);
        v87 = v101;
        ++v69;
        v111 = *(float *)&v69;
        *(_DWORD *)v101 = v86;
        v101 = v87 + 4;
      }
      while ( v69 <= (sub_4CC9E0((int)a2) ? 14 : 7) );
    }
    return 0;
  }
  else
  {
    v32 = (_DWORD **)v103;
    LODWORD(v105) = v119 + 1;
    v33 = (int *)(v110 - LODWORD(v97));
    LODWORD(v34) = LODWORD(v97) - v103;
    v109 = (int *)(v110 - LODWORD(v97));
    LODWORD(v100) = LODWORD(v97) - v103;
    while ( 1 )
    {
      v35 = *(_BYTE **)((char *)v33 + (_DWORD)v32 + LODWORD(v34));
      v36 = *a2;
      v111 = *(float *)LODWORD(v105);
      v97 = *(float *)&v35;
      v95 = *(_DWORD *)(v36 + 4 * v121);
      v113 = (float *)(v95 + v114);
      a2[10] = v99;
      v37 = (_DWORD *)sub_4C7BE0(a2, 60);
      *v32 = v37;
      memset(v37, 0, 0x3Cu);
      if ( j > 0 )
      {
        v38 = v113;
        LODWORD(v39) = LODWORD(v97) - (_DWORD)v113;
        v101 = j;
        LODWORD(v97) -= v113;
        while ( 1 )
        {
          a1 = sub_4D0170((char *)v38 + LODWORD(v39));
          *v38++ = a1;
          if ( !--v101 )
            break;
          v39 = v97;
        }
      }
      sub_4C9930((int *)v117, (int)v113, (int)i);
      v40 = v95;
      sub_4CA020((int *)v117, v95, v108, v106, *(float *)((char *)v32 + LODWORD(v100)));
      sub_4CA510((int *)v117, (int)i, (float *)v108, 1, v40);
      v41 = v119[LODWORD(v111) + 257];
      if ( *(_DWORD *)(v115 + 4 * v41 + 800) != 1 )
        return -1;
      (*v32)[7] = sub_4D20C0(a2, *(_DWORD *)(v118[12] + 4 * v41), v113, v40);
      if ( sub_4CC9E0((int)a2) && (*v32)[7] )
      {
        sub_4CA510((int *)v117, (int)i, (float *)v108, 2, v40);
        v42 = sub_4D20C0(a2, *(_DWORD *)(v118[12] + 4 * v119[LODWORD(v111) + 257]), v113, v40);
        v43 = (float *)v108;
        (*v32)[14] = v42;
        sub_4CA510((int *)v117, (int)i, v43, 0, v40);
        v44 = sub_4D20C0(a2, *(_DWORD *)(v118[12] + 4 * v119[LODWORD(v111) + 257]), v113, v40);
        v45 = *v32;
        v101 = 4;
        v46 = 0x10000;
        *v45 = v44;
        do
        {
          v47 = sub_4D2C30(a2, *(_DWORD *)(v118[12] + 4 * v119[LODWORD(v111) + 257]), **v32, (*v32)[7], v46 / 7);
          v48 = v101;
          v46 += 0x10000;
          *(_DWORD *)((char *)*v32 + v101) = v47;
          v101 = v48 + 4;
        }
        while ( v46 < 458752 );
        v101 = 32;
        for ( k = 0x10000; k < 458752; k += 0x10000 )
        {
          v50 = sub_4D2C30(a2, *(_DWORD *)(v118[12] + 4 * v119[LODWORD(v111) + 257]), (*v32)[7], (*v32)[14], k / 7);
          v51 = v101;
          *(_DWORD *)((char *)*v32 + v101) = v50;
          v101 = v51 + 4;
        }
      }
      LODWORD(v105) += 4;
      ++v32;
      if ( ++v121 >= *(_DWORD *)(v116 + 4) )
      {
        v17 = (int *)v117;
        v4 = v116;
        goto LABEL_33;
      }
      v33 = v109;
      v34 = v100;
    }
  }
}
