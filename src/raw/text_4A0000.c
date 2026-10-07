#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4A0010 @ 0x004A0010..0x004A00AE =====
int __userpurge sub_4A0010@<eax>(unsigned int a1@<edx>, _DWORD *a2@<edi>, _DWORD *a3@<esi>, int a4)
{
  unsigned int v4; // eax
  unsigned __int8 v5; // bl
  char v6; // cl
  unsigned int v7; // edx
  int v8; // ebx
  int result; // eax
  int i; // [esp+4h] [ebp-4h]

  v4 = 8 - *a3;
  for ( i = 0; v4 < a1; v4 = 8 - *a3 )
  {
    v5 = *(_BYTE *)(*a2 + a4) << *a3;
    if ( a1 >= 8 )
    {
      if ( a1 <= 8 )
        i |= v5;
      else
        i |= v5 << (a1 - 8);
    }
    else
    {
      i |= v5 >> (8 - a1);
    }
    a1 -= v4;
    ++*a2;
    *a3 = 0;
  }
  v6 = *a3 - a1;
  v7 = *a3 + a1;
  v8 = (unsigned __int8)(*(_BYTE *)(*a2 + a4) << *a3) >> (v4 + v6);
  *a3 = v7;
  result = v8 | i;
  if ( v7 == 8 )
  {
    ++*a2;
    *a3 = 0;
  }
  return result;
}

// ===== sub_4A00B0 @ 0x004A00B0..0x004A0109 =====
unsigned int __userpurge sub_4A00B0@<eax>(
        int a1@<edx>,
        int a2@<edi>,
        unsigned int *a3,
        int *a4,
        unsigned int a5,
        unsigned int a6)
{
  unsigned int result; // eax
  int v7; // esi
  char v8; // cl

  result = *a3;
  v7 = *a4;
  if ( a1 )
  {
    do
    {
      v8 = 7 - v7++;
      *(_BYTE *)(result + a2) |= ((a6 >> (a1 - 1)) & 1) << v8;
      if ( v7 == 8 )
      {
        ++result;
        v7 = 0;
        if ( result < a5 )
          *(_BYTE *)(result + a2) = 0;
      }
      --a1;
    }
    while ( a1 );
    *a4 = v7;
    *a3 = result;
  }
  else
  {
    *a4 = v7;
    *a3 = result;
  }
  return result;
}

// ===== sub_4A0110 @ 0x004A0110..0x004A0146 =====
int __userpurge sub_4A0110@<eax>(int a1@<edi>, int a2, unsigned int a3)
{
  int result; // eax
  unsigned int i; // esi
  int v5; // eax

  result = 0;
  for ( i = 0; i < a3; i += 64 )
  {
    v5 = sub_49FE60(16, *(__int16 *)(a1 + 2 * i) - result);
    ++*(_DWORD *)(a2 + 4 * v5);
    result = *(__int16 *)(a1 + 2 * i);
  }
  return result;
}

// ===== sub_4A0150 @ 0x004A0150..0x004A01D3 =====
void __stdcall sub_4A0150(_DWORD *a1, int a2, unsigned int a3)
{
  int v4; // eax
  int v5; // esi
  unsigned int i; // edi
  int v7; // ecx
  int v8; // eax
  unsigned int v9; // [esp+Ch] [ebp+8h]

  v9 = 0;
  if ( !a3 )
    return;
  do
  {
    v4 = 0;
    v5 = 0;
    for ( i = 1; i < 0x40; ++i )
    {
      v7 = *(__int16 *)(a2 + 2 * (v9 + (unsigned __int8)byte_503DA8[i]));
      if ( *(_WORD *)(a2 + 2 * (v9 + (unsigned __int8)byte_503DA8[i])) )
      {
        if ( v4 )
          a1[15] += v4;
        v8 = sub_49FE60(11, v7);
        ++a1[v5 | (16 * v8)];
        v4 = 0;
      }
      else
      {
        if ( (unsigned int)++v5 < 0x10 )
          continue;
        ++v4;
      }
      v5 = 0;
    }
    if ( v5 || v4 )
      ++*a1;
    v9 += 64;
  }
  while ( v9 < a3 );
}

// ===== sub_4A01E0 @ 0x004A01E0..0x004A0201 =====
int __userpurge sub_4A01E0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_4A0210(a4, a5, a6, a2, a1, 0, 0);
}

// ===== sub_4A0210 @ 0x004A0210..0x004A035E =====
unsigned int __stdcall sub_4A0210(int a1, int a2, int a3, unsigned int a4, unsigned int a5, int a6, int a7)
{
  unsigned int v7; // esi
  unsigned int result; // eax
  unsigned int v9; // edx
  int v10; // ebx
  unsigned int v11; // edx
  _DWORD *v12; // ecx
  unsigned int v13; // ebx
  unsigned int v14; // edx
  _DWORD *v15; // ecx

  while ( 1 )
  {
    v7 = a5;
    result = 3 * a4;
    v9 = *(_DWORD *)(a3 + 24 * a4 + 16);
    v10 = a3 + 24 * a4;
    if ( v9 >= a5 )
    {
      if ( v9 != -1 )
      {
        result = 2 * a6;
        if ( (unsigned int)(a7 + 1) < 8 )
        {
          result = sub_4A0210(a1, a2, a3, v9, a5, 2 * a6, a7 + 1);
        }
        else
        {
          *(_DWORD *)(a1 + 8 * a6) = 0;
          *(_DWORD *)(a2 + 8 * a6) = *(_DWORD *)(v10 + 16);
        }
      }
    }
    else
    {
      v11 = (2 * a6) << (8 - (a7 + 1));
      result = v11 | (unsigned __int8)byte_508541[a7];
      if ( v11 <= result )
      {
        v12 = (_DWORD *)(a2 + 4 * v11);
        result = result - v11 + 1;
        do
        {
          *(_DWORD *)((char *)v12 + a1 - a2) = a7 + 1;
          *v12++ = *(_DWORD *)(v10 + 16);
          --result;
        }
        while ( result );
        v7 = a5;
      }
    }
    v13 = *(_DWORD *)(v10 + 20);
    if ( v13 < v7 )
      break;
    if ( v13 == -1 )
      return result;
    result = (2 * a6) | 1;
    if ( (unsigned int)(a7 + 1) >= 8 )
    {
      *(_DWORD *)(a1 + 4 * result) = 0;
      *(_DWORD *)(a2 + 4 * result) = *(_DWORD *)(a3 + 24 * a4 + 20);
      return result;
    }
    ++a7;
    a6 = (2 * a6) | 1;
    a4 = v13;
  }
  v14 = ((2 * a6) | 1) << (8 - (a7 + 1));
  result = v14 | (unsigned __int8)byte_508541[a7];
  if ( v14 <= result )
  {
    v15 = (_DWORD *)(a2 + 4 * v14);
    result = result - v14 + 1;
    do
    {
      *(_DWORD *)((char *)v15 + a1 - a2) = a7 + 1;
      *v15++ = *(_DWORD *)(a3 + 24 * a4 + 20);
      --result;
    }
    while ( result );
  }
  return result;
}

// ===== sub_4A0360 @ 0x004A0360..0x004A04BD =====
unsigned int __userpurge sub_4A0360@<eax>(unsigned int result@<eax>, int a2@<ecx>, _DWORD *a3)
{
  int v4; // edx
  _DWORD *v6; // ecx
  unsigned int v7; // edx
  _DWORD *v8; // ecx
  int v9; // esi
  unsigned int v10; // edi
  unsigned int v11; // edx
  unsigned int v12; // ecx
  _DWORD *v13; // esi
  unsigned int v14; // esi
  _DWORD *v15; // esi
  int v16; // ecx
  _DWORD *v17; // ecx
  int *v18; // ecx
  int v19; // edx
  int v20; // [esp+Ch] [ebp-20h]
  int v21; // [esp+10h] [ebp-1Ch]
  unsigned int v22; // [esp+14h] [ebp-18h]
  unsigned int v23; // [esp+18h] [ebp-14h]
  int *v24; // [esp+1Ch] [ebp-10h]
  unsigned int v25; // [esp+20h] [ebp-Ch]
  unsigned int v26; // [esp+24h] [ebp-8h]
  _DWORD *v27; // [esp+28h] [ebp-4h]
  unsigned int v28; // [esp+34h] [ebp+8h]

  v4 = 2 * result - 1;
  v23 = v4;
  if ( 2 * result != 1 )
  {
    v6 = a3 + 2;
    do
    {
      *(v6 - 2) = 0;
      *(v6 - 1) = 0;
      *(_BYTE *)v6 = 0;
      v6[1] = -1;
      v6[2] = -1;
      v6[3] = -1;
      v6 += 6;
      --v4;
    }
    while ( v4 );
  }
  v7 = 0;
  v26 = 0;
  if ( result )
  {
    v8 = a3 + 1;
    do
    {
      v9 = *(_DWORD *)(a2 + 4 * v7);
      *v8 = v9;
      if ( v9 )
      {
        v26 += v9;
        *(v8 - 1) = 1;
      }
      ++v7;
      v8 += 6;
    }
    while ( v7 < result );
  }
  if ( result < v23 )
  {
    v24 = &a3[6 * result + 4];
    do
    {
      v10 = 0;
      v20 = -1;
      v21 = -1;
      v25 = 0;
      v11 = 0;
      v27 = a3 + 6;
      do
      {
        v12 = 0;
        if ( result )
        {
          v13 = a3;
          while ( !*v13 )
          {
            ++v12;
            v13 += 6;
            if ( v12 >= result )
              goto LABEL_18;
          }
          *(&v20 + v11) = v12;
        }
LABEL_18:
        v14 = v11 + 1;
        v22 = v11 + 1;
        v28 = v11 + 1;
        if ( v11 + 1 < result )
        {
          v15 = v27;
          do
          {
            if ( *v15 && v15[1] < a3[6 * *(&v20 + v11) + 1] )
              *(&v20 + v11) = v28;
            v15 += 6;
            ++v28;
          }
          while ( v28 < result );
          v10 = v25;
          v14 = v22;
        }
        v16 = *(&v20 + v11);
        if ( v16 == -1 )
          break;
        v27 += 6;
        v17 = &a3[6 * v16];
        *v17 = 0;
        v17[3] = result;
        *((_BYTE *)v17 + 8) = v11;
        v10 += v17[1];
        v11 = v14;
        v25 = v10;
      }
      while ( v14 < 2 );
      v18 = v24;
      *v24 = v20;
      v19 = v21;
      *(v18 - 4) = 1;
      *(v18 - 3) = v10;
      v18[1] = v19;
      *((_BYTE *)v18 - 8) = -1;
      if ( v10 >= v26 )
        break;
      ++result;
      v24 = v18 + 6;
    }
    while ( result < v23 );
  }
  return result;
}

// ===== sub_4A04C0 @ 0x004A04C0..0x004A0647 =====
int __userpurge sub_4A04C0@<eax>(
        size_t a1@<eax>,
        int a2,
        _DWORD *a3,
        void *a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9,
        int a10,
        int a11,
        unsigned int a12,
        unsigned int a13)
{
  unsigned int v13; // ecx
  int v14; // edi
  unsigned int v15; // ebx
  int v16; // esi
  int v17; // eax
  int v18; // ecx
  int v19; // ecx
  int v20; // edi
  unsigned int i; // eax
  int v23; // [esp+10h] [ebp-28h]
  int v24; // [esp+14h] [ebp-24h]
  unsigned int v25; // [esp+18h] [ebp-20h]
  unsigned int v26; // [esp+1Ch] [ebp-1Ch]
  int v27; // [esp+20h] [ebp-18h]
  int v28; // [esp+24h] [ebp-14h]
  unsigned int v29; // [esp+28h] [ebp-10h]
  int v30; // [esp+2Ch] [ebp-Ch]
  unsigned int v31; // [esp+30h] [ebp-8h]
  int v32; // [esp+34h] [ebp-4h]
  int v33; // [esp+4Ch] [ebp+14h]

  memset(a4, 0, a1);
  v13 = 0;
  v14 = 0;
  v15 = 0;
  v16 = 0;
  v24 = 0;
  v31 = 0;
  v23 = 0;
  if ( a13 )
  {
    v27 = a6;
    v26 = 8;
    v28 = a5 + 3;
    do
    {
      if ( v26 <= a9 )
        v25 = 8;
      else
        v25 = a9 - 8 * v13;
      v30 = 0;
      if ( a12 )
      {
        v17 = v27;
        v32 = v28;
        v33 = v27;
        v29 = 8;
        while ( 1 )
        {
          if ( !a7 || (v29 <= a8 ? (v18 = 8) : (v18 = a8 - 8 * v30), sub_49EFD0(v18, v17 + a7 - a6, v17, v25, a10)) )
          {
            v19 = v32;
            v20 = 8;
            do
            {
              for ( i = 0; i < 0x20; i += 4 )
              {
                *(_BYTE *)(v16 + a2) = *(_BYTE *)(v19 + i);
                ++v16;
              }
              v19 += a11;
              --v20;
            }
            while ( v20 );
            v15 = v31;
            v14 = v24;
            *((_BYTE *)a4 + v24) |= 1 << v31;
          }
          v33 += 32;
          v32 += 32;
          v31 = ++v15;
          if ( v15 >= 8 )
          {
            v15 = 0;
            ++v14;
            v31 = 0;
            v24 = v14;
          }
          v29 += 8;
          if ( ++v30 >= a12 )
            break;
          v17 = v33;
        }
        v13 = v23;
      }
      v26 += 8;
      v28 += 8 * a11;
      v27 += 8 * a10;
      v23 = ++v13;
    }
    while ( v13 < a13 );
    *a3 = v16;
    return 0;
  }
  else
  {
    *a3 = 0;
    return 0;
  }
}

// ===== sub_4A0650 @ 0x004A0650..0x004A071F =====
int __userpurge sub_4A0650@<eax>(int a1@<eax>, int a2@<edi>, int a3, int a4, _DWORD *a5, int a6, int a7)
{
  unsigned int v7; // ecx
  int v8; // edx
  int v9; // ebx
  int v10; // esi
  unsigned int i; // eax
  int v13; // [esp+0h] [ebp-1Ch]
  int v14; // [esp+4h] [ebp-18h]
  int v15; // [esp+8h] [ebp-14h]
  int v16; // [esp+Ch] [ebp-10h]
  int v17; // [esp+10h] [ebp-Ch]
  unsigned int v18; // [esp+14h] [ebp-8h]
  int v19; // [esp+18h] [ebp-4h]

  v7 = 0;
  v8 = 0;
  v16 = 0;
  v17 = 0;
  v18 = 0;
  if ( a1 )
  {
    v13 = a1;
    do
    {
      v9 = __ROL4__(1, v7);
      if ( a7 )
      {
        v19 = v16 + a3 + 3;
        v14 = a7;
        do
        {
          if ( ((unsigned __int8)v9 & *(_BYTE *)(v17 + a4)) != 0 )
          {
            v10 = v19;
            v15 = 8;
            do
            {
              for ( i = 0; i < 0x20; i += 4 )
              {
                *(_BYTE *)(v10 + i) = *(_BYTE *)(v8 + a2);
                ++v8;
              }
              v10 += a6;
              --v15;
            }
            while ( v15 );
            v7 = v18;
          }
          v19 += 32;
          ++v7;
          v9 = __ROL4__(v9, 1);
          v18 = v7;
          if ( v7 >= 8 )
          {
            v7 = 0;
            ++v17;
            v18 = 0;
            v9 = 1;
          }
          --v14;
        }
        while ( v14 );
      }
      v16 += 8 * a6;
      --v13;
    }
    while ( v13 );
    *a5 = v8;
    return 0;
  }
  else
  {
    *a5 = 0;
    return 0;
  }
}

// ===== sub_4A0720 @ 0x004A0720..0x004A0924 =====
int __stdcall sub_4A0720(_BYTE *a1, unsigned int *a2, int a3, unsigned int a4, unsigned int a5, int a6)
{
  unsigned int v6; // esi
  unsigned int v7; // ebx
  int v8; // eax
  int v9; // edx
  int v10; // eax
  int v11; // ecx
  unsigned int v12; // eax
  int v13; // edi
  int v14; // edx
  int v15; // ecx
  unsigned int v16; // esi
  _BYTE *v17; // edx
  __int16 v18; // di
  unsigned int v20; // [esp+8h] [ebp-30h]
  char v21; // [esp+Ch] [ebp-2Ch]
  int v22; // [esp+10h] [ebp-28h]
  __int16 v23; // [esp+14h] [ebp-24h]
  unsigned int v24; // [esp+18h] [ebp-20h]
  int v25; // [esp+1Ch] [ebp-1Ch]
  int v26; // [esp+20h] [ebp-18h]
  int v27; // [esp+24h] [ebp-14h]
  int v28; // [esp+28h] [ebp-10h]
  int v29; // [esp+2Ch] [ebp-Ch]
  int v30; // [esp+30h] [ebp-8h]
  char v31; // [esp+37h] [ebp-1h]

  v6 = 3;
  v20 = 0;
  v26 = 0;
  v7 = 1;
  *a1 = 0;
  v29 = 3;
  v30 = 1;
  if ( a4 <= 3 )
  {
    *a2 = 1;
    return 0;
  }
  while ( 1 )
  {
    if ( v7 >= *a2 )
    {
      *a2 = v7;
      return 0;
    }
    v8 = (v6 >> 2) / a5;
    v9 = (v6 >> 2) % a5;
    v28 = v9 <= 31 ? -v9 : -31;
    v10 = v8 <= 6 ? -v8 : -6;
    v27 = a5 - v9 <= 0x1F ? a5 - v9 : 31;
    v24 = 2;
    v11 = v10;
    v25 = v10;
    if ( v10 > 0 )
      goto LABEL_33;
    do
    {
      v12 = 0;
      if ( !v11 )
        v27 = -1;
      v13 = v28;
      if ( v28 <= v27 )
      {
        v14 = v6 + a3;
        v31 = *(_BYTE *)(a3 + v6);
        v22 = v6 + a3;
        v15 = a6 * v11 + 4 * v28;
        do
        {
          if ( v31 == *(_BYTE *)(v15 + v14) )
          {
            v16 = v6 + 4;
            v12 = 1;
            v17 = (_BYTE *)(v14 + 4);
            do
            {
              if ( v16 >= a4 )
                break;
              if ( *v17 != v17[v15] )
                break;
              ++v12;
              v16 += 4;
              v17 += 4;
            }
            while ( v12 < 0x82 );
            if ( v12 > v24 )
            {
              LOBYTE(v23) = v13;
              v21 = v25;
              v24 = v12;
              if ( v12 == 130 )
              {
                v6 = v29;
                v7 = v30;
                goto LABEL_31;
              }
            }
            v14 = v22;
            v6 = v29;
          }
          ++v13;
          v15 += 4;
        }
        while ( v13 <= v27 );
        v7 = v30;
        if ( v12 == 130 )
          break;
        v11 = v25;
      }
      v25 = ++v11;
    }
    while ( v11 <= 0 );
LABEL_31:
    if ( v24 < 3 )
    {
LABEL_33:
      a1[v7] = *(_BYTE *)(v6 + a3);
      v6 += 4;
      ++v7;
    }
    else
    {
      v23 &= 0x3Fu;
      v6 += 4 * v24;
      a1[v20] |= 1 << v26;
      v18 = v23 | ((v21 & 7 | (unsigned __int16)(8 * v24 - 24)) << 6);
      v21 &= 7u;
      *(_WORD *)&a1[v7] = v18;
      v7 += 2;
    }
    v30 = v7;
    v29 = v6;
    if ( (unsigned int)++v26 >= 8 )
      break;
LABEL_38:
    if ( v6 >= a4 )
    {
      *a2 = v7;
      return 0;
    }
  }
  if ( v6 < a4 )
  {
    if ( v7 < *a2 )
    {
      v20 = v7;
      a1[v7++] = 0;
      v26 = 0;
      v30 = v7;
    }
    goto LABEL_38;
  }
  *a2 = v7;
  return 0;
}

// ===== sub_4A0930 @ 0x004A0930..0x004A0A1B =====
int __stdcall sub_4A0930(int a1, unsigned int a2, int a3, unsigned int *a4, int a5)
{
  unsigned int *v5; // esi
  unsigned int v6; // ebx
  unsigned int v7; // edi
  unsigned int v8; // edx
  unsigned int v9; // eax
  unsigned int v10; // esi
  int v11; // edx
  int v12; // eax
  int v13; // ecx
  _BYTE *v14; // esi
  unsigned int v15; // eax
  unsigned int v17; // [esp+10h] [ebp-8h]
  int v18; // [esp+14h] [ebp-4h]

  v5 = a4;
  v6 = 1;
  v17 = 0;
  v18 = 0;
  v7 = 3;
  while ( v6 < *v5 )
  {
    if ( v7 >= a2 )
      break;
    if ( ((unsigned __int8)(1 << v18) & *(_BYTE *)(v17 + a3)) != 0 )
    {
      v8 = *(unsigned __int16 *)(v6 + a3);
      v9 = v8 >> 6;
      v10 = *(_WORD *)(v6 + a3) & 0x3F;
      v11 = (v8 >> 9) + 3;
      v12 = v9 & 7;
      if ( v10 > 0x1F )
        v10 |= 0xFFFFFFC0;
      if ( v12 > 0 )
        v12 |= 0xFFFFFFF8;
      v13 = v7 + a5 * v12 + 4 * v10;
      if ( 4 * v11 )
      {
        v14 = (_BYTE *)(v7 + a1);
        v15 = ((unsigned int)(4 * v11 - 1) >> 2) + 1;
        do
        {
          *v14 = v14[v13 - v7];
          v14 += 4;
          --v15;
        }
        while ( v15 );
      }
      v5 = a4;
      v6 += 2;
      v7 += 4 * v11;
    }
    else
    {
      *(_BYTE *)(v7 + a1) = *(_BYTE *)(v6 + a3);
      ++v6;
      v7 += 4;
    }
    if ( (unsigned int)++v18 >= 8 )
    {
      if ( v6 >= *v5 )
        break;
      if ( v7 < a2 )
      {
        v17 = v6;
        v18 = 0;
        ++v6;
      }
    }
  }
  *v5 = v6;
  return 0;
}

// ===== sub_4A0A20 @ 0x004A0A20..0x004A0B8A =====
int __fastcall sub_4A0A20(unsigned int *a1, int a2, int a3, unsigned int a4)
{
  unsigned int v4; // eax
  unsigned int v5; // ebx
  unsigned int v6; // esi
  unsigned int v7; // eax
  int v9; // [esp+Ch] [ebp-3404h] BYREF
  int v10; // [esp+10h] [ebp-3400h]
  int v11; // [esp+14h] [ebp-33FCh]
  unsigned int *v12; // [esp+18h] [ebp-33F8h]
  int v13; // [esp+1Ch] [ebp-33F4h] BYREF
  unsigned int *v14; // [esp+20h] [ebp-33F0h] BYREF
  int v15; // [esp+24h] [ebp-33ECh] BYREF
  char v16; // [esp+28h] [ebp-33E8h] BYREF
  _DWORD v17[256]; // [esp+300Ch] [ebp-404h] BYREF

  v12 = a1;
  v11 = a3;
  v4 = 0;
  v5 = 0;
  v10 = a2;
  memset(v17, 0, sizeof(v17));
  if ( a4 )
  {
    do
      ++v17[*(unsigned __int8 *)(a2 + v4++)];
    while ( v4 < a4 );
  }
  sub_4A0360(0x100u, (int)v17, &v15);
  v14 = (unsigned int *)&v16;
  v13 = 256;
  do
  {
    sub_49FA10(*v14, &v9, v11 + v5);
    v5 += v9;
    v14 += 6;
    --v13;
  }
  while ( v13 );
  v6 = 0;
  v14 = (unsigned int *)v5;
  v13 = 0;
  *(_BYTE *)(v11 + v5) = 0;
  if ( a4 )
  {
    do
      sub_49FF50(&v13, v11, (unsigned int *)&v14, *v12, *(unsigned __int8 *)(v10 + v6++), (int)&v15);
    while ( v6 < a4 );
    v7 = (unsigned int)v14;
    if ( v13 )
      v7 = (unsigned int)v14 + 1;
    *v12 = v7;
  }
  else
  {
    *v12 = v5;
  }
  return 0;
}

// ===== sub_4A0B90 @ 0x004A0B90..0x004A0CC0 =====
int __fastcall sub_4A0B90(int a1, int a2, int a3, unsigned int a4, unsigned int *a5)
{
  unsigned int v5; // esi
  unsigned int i; // ebx
  int v7; // eax
  unsigned int v8; // eax
  unsigned int v9; // edi
  char v10; // al
  unsigned int v13; // [esp+18h] [ebp-3BF8h] BYREF
  int v14; // [esp+1Ch] [ebp-3BF4h]
  unsigned int v15; // [esp+20h] [ebp-3BF0h] BYREF
  _DWORD v16[3066]; // [esp+24h] [ebp-3BECh] BYREF
  _DWORD v17[256]; // [esp+300Ch] [ebp-C04h] BYREF
  _BYTE v18[1024]; // [esp+340Ch] [ebp-804h] BYREF
  _BYTE v19[1024]; // [esp+380Ch] [ebp-404h] BYREF

  v5 = 0;
  v14 = a2;
  for ( i = 0; i < 0x100; ++i )
  {
    v7 = sub_49FA40(v14 + v5, &v15);
    v5 += v15;
    v17[i] = v7;
  }
  v13 = v5;
  v8 = sub_4A0360(0x100u, (int)v17, v16);
  sub_4A01E0(256, v8, a3, (int)v19, (int)v18, (int)v16);
  v9 = 0;
  v15 = 0;
  if ( v5 < *a5 )
  {
    do
    {
      if ( v9 >= a4 )
        break;
      v10 = sub_49FE90(v14, &v13, &v15, (int)v16, 0x100u, (int)v19, (int)v18);
      v5 = v13;
      *(_BYTE *)(a1 + v9++) = v10;
    }
    while ( v5 < *a5 );
    if ( v15 )
      ++v5;
  }
  *a5 = v5;
  return 0;
}

// ===== sub_4A0CC0 @ 0x004A0CC0..0x004A0D21 =====
int __fastcall sub_4A0CC0(int a1, _WORD *a2)
{
  int result; // eax
  int v3; // ecx

  result = 1;
  if ( *a2 && a2[1] )
  {
    v3 = (unsigned __int16)a2[2];
    switch ( a2[2] )
    {
      case 8:
      case 0x10:
      case 0x18:
      case 0x20:
      case 0x30:
        if ( a2[4] < 7u )
        {
          if ( v3 == 8 || v3 == 24 || v3 == 32 )
            result = 0;
          else
            result = 2;
        }
        break;
      default:
        return result;
    }
  }
  return result;
}

// ===== sub_4A0D60 @ 0x004A0D60..0x004A0DB1 =====
int __usercall sub_4A0D60@<eax>(int a1@<esi>)
{
  int result; // eax

  result = 0;
  if ( strcmp((const char *)a1, "CompressedBG___") )
    return 3;
  if ( *(_BYTE *)(a1 + 46) != 2 || *(_BYTE *)(a1 + 47) )
    return 4;
  return result;
}

// ===== sub_4A0DC0 @ 0x004A0DC0..0x004A0F35 =====
int __usercall sub_4A0DC0@<eax>(unsigned __int16 *a1@<edx>, int a2@<ecx>, int a3@<ebp>, int a4, int a5)
{
  int v6; // edx
  int v7; // eax
  unsigned int v8; // ecx
  int v9; // eax
  int v10; // edi
  int v12; // [esp-308h] [ebp-314h]
  _DWORD v13[6]; // [esp-2F4h] [ebp-300h] BYREF
  int v14; // [esp-2DCh] [ebp-2E8h]
  _DWORD *v15; // [esp-2D8h] [ebp-2E4h]
  unsigned int v16; // [esp-2D4h] [ebp-2E0h] BYREF
  _DWORD v17[175]; // [esp-2D0h] [ebp-2DCh] BYREF
  unsigned int v18; // [esp-14h] [ebp-20h]
  int *v19; // [esp-10h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp-Ch] [ebp-18h]
  void *v21; // [esp-8h] [ebp-14h]
  int v22; // [esp-4h] [ebp-10h]
  _DWORD v23[2]; // [esp+0h] [ebp-Ch] BYREF
  int v24; // [esp+8h] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v23[0] = a3;
  v23[1] = retaddr;
  v22 = -1;
  v21 = &loc_4D910B;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v19 = &v24;
  v18 = (unsigned int)v23 ^ dword_4FB734;
  v14 = a4;
  v15 = (_DWORD *)a2;
  if ( !sub_4A0CC0(a2, a1) )
  {
    sub_49B040(v17, dword_5666F8);
    v22 = 0;
    sub_49B290(a5, (int)v17);
    v6 = *a1;
    v7 = a1[1];
    v13[4] = a1[4];
    v8 = a1[2];
    v13[3] = v7;
    v8 >>= 3;
    v13[2] = v6;
    v13[1] = v6 * v8;
    v16 = 2 * v8 * ((v6 + 7) & 0xFFFFFFF8) * ((v7 + 7) & 0xFFFFFFF8);
    v13[5] = v8;
    v13[0] = a1 + 8;
    v12 = sub_49B620((int)v17);
    v9 = sub_49B540((int)v17);
    v10 = sub_4A1300(v14, v9, v12);
    if ( !sub_49B690(&v16, v13, (int)v17, v10 + v14) )
      *v15 = v16 + v10;
    v22 = -1;
  }
  return sub_4AB245((unsigned int)v23 ^ v18);
}

// ===== sub_4A0F40 @ 0x004A0F40..0x004A12FB =====
int __usercall sub_4A0F40@<eax>(int a1@<ecx>, int a2@<ebp>, unsigned __int16 *a3, int a4)
{
  void *v5; // esi
  unsigned __int16 *v6; // eax
  unsigned int v7; // esi
  unsigned __int16 *v8; // esi
  int v9; // eax
  unsigned __int16 *v10; // edi
  unsigned __int16 v11; // cx
  unsigned int k; // eax
  unsigned int v14; // [esp-380h] [ebp-38Ch] BYREF
  _DWORD v15[16]; // [esp-368h] [ebp-374h] BYREF
  unsigned __int16 *v16; // [esp-328h] [ebp-334h] BYREF
  int v17; // [esp-324h] [ebp-330h]
  unsigned int v18; // [esp-320h] [ebp-32Ch]
  unsigned int v19; // [esp-31Ch] [ebp-328h]
  int v20; // [esp-318h] [ebp-324h]
  int v21; // [esp-314h] [ebp-320h]
  unsigned __int16 *v22; // [esp-310h] [ebp-31Ch]
  int v23; // [esp-30Ch] [ebp-318h]
  unsigned int v24; // [esp-308h] [ebp-314h]
  unsigned __int16 *v25; // [esp-304h] [ebp-310h]
  unsigned int v26; // [esp-300h] [ebp-30Ch]
  unsigned __int16 *v27; // [esp-2FCh] [ebp-308h]
  unsigned __int16 *v28; // [esp-2F8h] [ebp-304h]
  void *v29; // [esp-2F4h] [ebp-300h]
  unsigned int j; // [esp-2F0h] [ebp-2FCh]
  unsigned int i; // [esp-2ECh] [ebp-2F8h]
  int v32; // [esp-2E8h] [ebp-2F4h]
  unsigned __int16 *v33; // [esp-2E4h] [ebp-2F0h]
  _DWORD v34[175]; // [esp-2E0h] [ebp-2ECh] BYREF
  unsigned int v35; // [esp-24h] [ebp-30h]
  int *v36; // [esp-14h] [ebp-20h]
  unsigned int *v37; // [esp-10h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp-Ch] [ebp-18h]
  void *v39; // [esp-8h] [ebp-14h]
  int v40; // [esp-4h] [ebp-10h]
  _DWORD v41[2]; // [esp+0h] [ebp-Ch] BYREF
  int v42; // [esp+8h] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v41[0] = a2;
  v41[1] = retaddr;
  v40 = -1;
  v39 = &loc_4D90CB;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v36 = &v42;
  v35 = (unsigned int)v41 ^ dword_4FB734;
  v14 = (unsigned int)v41 ^ dword_4FB734;
  v37 = &v14;
  v33 = a3;
  if ( !sub_4A0D60(a1) )
  {
    v29 = 0;
    v28 = 0;
    v40 = 0;
    v5 = operator new[](*(_DWORD *)(a1 + 40));
    v23 = (int)v5;
    v29 = v5;
    memcpy_0(v5, (const void *)(a1 + 48), *(_DWORD *)(a1 + 40));
    sub_469B80((int)v5, *(_DWORD *)(a1 + 40), *(_DWORD *)(a1 + 36));
    if ( sub_4A13D0(*(_DWORD *)(a1 + 40), *(_BYTE *)(a1 + 44), *(_BYTE *)(a1 + 45)) )
    {
      v6 = v33;
      *(_DWORD *)v33 = *(_DWORD *)(a1 + 16);
      *((_DWORD *)v6 + 1) = *(_DWORD *)(a1 + 20);
      *((_DWORD *)v6 + 2) = *(_DWORD *)(a1 + 24);
      *((_DWORD *)v6 + 3) = *(_DWORD *)(a1 + 28);
      v7 = (*v6 + 7) & 0xFFFFFFF8;
      v26 = (v6[1] + 7) & 0xFFFFFFF8;
      memset(v15, 0, sizeof(v15));
      strcpy((char *)v15, "BF_Movie_______");
      v15[4] = 65537;
      v15[5] = v7;
      v15[6] = v26;
      v15[7] = v33[2];
      v15[8] = v33[4];
      v15[9] = 1;
      v15[10] = 1;
      v15[11] = 0;
      v18 = v7;
      v19 = v26;
      v20 = v15[8];
      v21 = 4;
      v17 = 4 * v7;
      if ( *v33 == v7 && v33[1] == v26 && v33[2] == 32 )
      {
        v8 = v33 + 8;
      }
      else
      {
        v28 = (unsigned __int16 *)operator new[](v26 * v17);
        v8 = v28;
      }
      v16 = v8;
      sub_49B040(v34, a4);
      LOBYTE(v40) = 1;
      v9 = sub_49CC10((int)v15, (int)v34, (int)&v16, v23, *(_DWORD *)(a1 + 40) + a1 + 48);
      if ( v9 )
      {
        v32 = (v9 != 11) + 8;
        v40 = -1;
      }
      else
      {
        if ( v8 != v33 + 8 )
        {
          v27 = v8;
          v10 = v33 + 8;
          v25 = v33 + 8;
          v11 = v33[2] >> 3;
          for ( i = 0; i < v33[1]; ++i )
          {
            v22 = v8;
            for ( j = 0; j < *v33; ++j )
            {
              for ( k = 0; ; ++k )
              {
                v24 = k;
                if ( k >= v11 )
                  break;
                *((_BYTE *)v10 + k) = *((_BYTE *)v8 + k);
              }
              v8 = (unsigned __int16 *)((char *)v8 + v21);
              v22 = v8;
              v10 = (unsigned __int16 *)((char *)v10 + v11);
              v25 = v10;
            }
            v8 = &v27[v17 / 2u];
            v27 = (unsigned __int16 *)((char *)v27 + v17);
          }
        }
        v32 = 0;
        v40 = -1;
      }
    }
    else
    {
      v32 = 5;
      v40 = -1;
    }
    operator delete[](v29);
    operator delete[](v28);
  }
  return sub_4AB245((unsigned int)v41 ^ v35);
}

// ===== sub_4A1300 @ 0x004A1300..0x004A13C2 =====
int __cdecl sub_4A1300(int a1, const void *a2, const void *a3)
{
  _DWORD *v3; // ecx
  DWORD TickCount; // eax
  unsigned int v6; // [esp-14h] [ebp-20h]
  int v7; // [esp-10h] [ebp-1Ch]

  *(_DWORD *)a1 = 0;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 12) = 0;
  strcpy((char *)a1, "CompressedBG___");
  *(_DWORD *)(a1 + 16) = *v3;
  *(_DWORD *)(a1 + 20) = v3[1];
  *(_DWORD *)(a1 + 24) = v3[2];
  *(_DWORD *)(a1 + 28) = v3[3];
  *(_DWORD *)(a1 + 32) = 0;
  TickCount = GetTickCount();
  *(_DWORD *)(a1 + 40) = 132;
  *(_DWORD *)(a1 + 36) = TickCount;
  qmemcpy((void *)(a1 + 48), a2, 0x40u);
  qmemcpy((void *)(a1 + 112), a3, 0x40u);
  *(_DWORD *)(a1 + 176) = *(_DWORD *)(a1 + 40) + 48;
  sub_469AF0(a1 + 48, (_BYTE *)(a1 + 44), (_BYTE *)(a1 + 45), *(_DWORD *)(a1 + 40));
  v7 = *(_DWORD *)(a1 + 36);
  v6 = *(_DWORD *)(a1 + 40);
  *(_WORD *)(a1 + 46) = 2;
  sub_469B50(a1 + 48, v6, v7);
  return *(_DWORD *)(a1 + 40) + 48;
}

// ===== sub_4A13D0 @ 0x004A13D0..0x004A143B =====
BOOL __usercall sub_4A13D0@<eax>(int a1@<esi>, int a2, char a3, char a4)
{
  char v4; // al
  unsigned int v5; // edi
  char v6; // bl
  unsigned int v7; // ecx
  char v8; // al
  char v9; // dl
  char v11; // [esp+Ah] [ebp-2h]
  char v12; // [esp+Bh] [ebp-1h]

  v4 = 0;
  v5 = a2;
  v6 = 0;
  v7 = 0;
  v11 = 0;
  v12 = 0;
  if ( a2 >= 2 )
  {
    do
    {
      v8 = *(_BYTE *)(v7 + a1 + 1);
      v9 = *(_BYTE *)(v7 + a1);
      v11 += v9;
      v6 += v8;
      v12 ^= v9 ^ v8;
      v7 += 2;
    }
    while ( v7 < a2 - 1 );
    v4 = 0;
    v5 = a2;
  }
  if ( v7 < v5 )
  {
    v4 = *(_BYTE *)(v7 + a1);
    v12 ^= v4;
  }
  return a3 == v4 + v11 + v6 && a4 == v12;
}

// ===== sub_4A1440 @ 0x004A1440..0x004A150C =====
int __cdecl sub_4A1440(int a1, void *Src)
{
  size_t Size; // ecx
  size_t v3; // edi
  int v4; // ebx
  void *v5; // esi
  _DWORD v7[5]; // [esp+0h] [ebp-2Ch] BYREF
  void *v8; // [esp+14h] [ebp-18h]
  int v9; // [esp+18h] [ebp-14h]
  _DWORD *v10; // [esp+1Ch] [ebp-10h]
  int v11; // [esp+28h] [ebp-4h]

  v10 = v7;
  v3 = Size;
  v9 = 1;
  v4 = sub_4A1DA0();
  v8 = 0;
  v11 = 0;
  v7[4] = v4 + v3;
  v5 = operator new[](v4 + v3);
  v8 = v5;
  memcpy_0(v5, Src, v3);
  sub_4A1DC0(v4);
  v11 = -1;
  sub_4A1510(a1, v5);
  operator delete[](v8);
  return v9;
}

// ===== sub_4A1510 @ 0x004A1510..0x004A1C8B =====
int __cdecl sub_4A1510(int *a1, int a2)
{
  int v2; // ecx
  int v3; // edx
  int v4; // edi
  int result; // eax
  _DWORD *v6; // ebx
  int v7; // edx
  int v8; // edx
  int v9; // edx
  int v10; // edx
  int v11; // edx
  int v12; // edx
  int v13; // edx
  int v14; // edx
  int v15; // edx
  int v16; // edx
  int v17; // edx
  int v18; // edx
  int v19; // edx
  int v20; // edx
  int v21; // edx
  int v22; // edx
  int v23; // edi
  int v24; // edx
  int v25; // [esp+8h] [ebp-20h]
  int v26; // [esp+Ch] [ebp-1Ch]
  int v27; // [esp+10h] [ebp-18h]
  int v28; // [esp+14h] [ebp-14h]
  unsigned int v29; // [esp+18h] [ebp-10h]
  int v30; // [esp+1Ch] [ebp-Ch]
  int v31; // [esp+20h] [ebp-8h]

  v3 = -271733879;
  v4 = -1732584194;
  result = 271733878;
  v25 = 1732584193;
  v26 = -271733879;
  v27 = -1732584194;
  v28 = 271733878;
  if ( v2 )
  {
    v6 = (_DWORD *)(a2 + 56);
    v29 = ((unsigned int)(v2 - 1) >> 6) + 1;
    do
    {
      v30 = v3;
      v31 = result;
      sub_4A1CE0(v4, *(v6 - 14), 7, -680876936);
      sub_4A1CE0(v7, *(v6 - 13), 12, -389564586);
      sub_4A1CE0(v25, *(v6 - 12), 17, 606105819);
      sub_4A1CE0(v8, *(v6 - 11), 22, -1044525330);
      sub_4A1CE0(v4, *(v6 - 10), 7, -176418897);
      sub_4A1CE0(v9, *(v6 - 9), 12, 1200080426);
      sub_4A1CE0(v25, *(v6 - 8), 17, -1473231341);
      sub_4A1CE0(v10, *(v6 - 7), 22, -45705983);
      sub_4A1CE0(v4, *(v6 - 6), 7, 1770035416);
      sub_4A1CE0(v11, *(v6 - 5), 12, -1958414417);
      sub_4A1CE0(v25, *(v6 - 4), 17, -42063);
      sub_4A1CE0(v12, *(v6 - 3), 22, -1990404162);
      sub_4A1CE0(v4, *(v6 - 2), 7, 1804603682);
      sub_4A1CE0(v13, *(v6 - 1), 12, -40341101);
      sub_4A1CE0(v25, *v6, 17, -1502002290);
      sub_4A1CE0(v14, v6[1], 22, 1236535329);
      sub_4A1D10(*(v6 - 13), 5, -165796510);
      sub_4A1D10(*(v6 - 8), 9, -1069501632);
      sub_4A1D10(*(v6 - 3), 14, 643717713);
      sub_4A1D10(*(v6 - 14), 20, -373897302);
      sub_4A1D10(*(v6 - 9), 5, -701558691);
      sub_4A1D10(*(v6 - 4), 9, 38016083);
      sub_4A1D10(v6[1], 14, -660478335);
      sub_4A1D10(*(v6 - 10), 20, -405537848);
      sub_4A1D10(*(v6 - 5), 5, 568446438);
      sub_4A1D10(*v6, 9, -1019803690);
      sub_4A1D10(*(v6 - 11), 14, -187363961);
      sub_4A1D10(*(v6 - 6), 20, 1163531501);
      sub_4A1D10(*(v6 - 1), 5, -1444681467);
      sub_4A1D10(*(v6 - 12), 9, -51403784);
      sub_4A1D10(*(v6 - 7), 14, 1735328473);
      sub_4A1D10(*(v6 - 2), 20, -1926607734);
      sub_4A1D40(v4, *(v6 - 9), 4, -378558);
      sub_4A1D40(v30, *(v6 - 6), 11, -2022574463);
      sub_4A1D40(v15, *(v6 - 3), 16, 1839030562);
      sub_4A1D40(v31, *v6, 23, -35309556);
      sub_4A1D40(v16, *(v6 - 13), 4, -1530992060);
      sub_4A1D40(v30, *(v6 - 10), 11, 1272893353);
      sub_4A1D40(v17, *(v6 - 7), 16, -155497632);
      sub_4A1D40(v31, *(v6 - 4), 23, -1094730640);
      sub_4A1D40(v18, *(v6 - 1), 4, 681279174);
      sub_4A1D40(v30, *(v6 - 14), 11, -358537222);
      sub_4A1D40(v19, *(v6 - 11), 16, -722521979);
      sub_4A1D40(v31, *(v6 - 8), 23, 76029189);
      sub_4A1D40(v20, *(v6 - 5), 4, -640364487);
      sub_4A1D40(v30, *(v6 - 2), 11, -421815835);
      sub_4A1D40(v21, v6[1], 16, 530742520);
      sub_4A1D40(v31, *(v6 - 12), 23, -995338651);
      sub_4A1D70(v31, *(v6 - 14), 6, -198630844);
      sub_4A1D70(v4, *(v6 - 7), 10, 1126891415);
      sub_4A1D70(v30, *v6, 15, -1416354905);
      sub_4A1D70(v25, *(v6 - 9), 21, -57434055);
      sub_4A1D70(v31, *(v6 - 2), 6, 1700485571);
      sub_4A1D70(v4, *(v6 - 11), 10, -1894986606);
      sub_4A1D70(v30, *(v6 - 4), 15, -1051523);
      sub_4A1D70(v25, *(v6 - 13), 21, -2054922799);
      sub_4A1D70(v31, *(v6 - 6), 6, 1873313359);
      sub_4A1D70(v4, v6[1], 10, -30611744);
      sub_4A1D70(v30, *(v6 - 8), 15, -1560198380);
      sub_4A1D70(v25, *(v6 - 1), 21, 1309151649);
      v23 = v22;
      sub_4A1D70(v31, *(v6 - 10), 6, -145523070);
      sub_4A1D70(v23, *(v6 - 3), 10, -1120210379);
      sub_4A1D70(v30, *(v6 - 12), 15, 718787259);
      sub_4A1D70(v25, *(v6 - 5), 21, -343485551);
      v26 += v30;
      v27 += v24;
      v25 *= 2;
      v28 += v31;
      result = v28;
      v4 = v27;
      v3 = v26;
      v6 += 16;
      --v29;
    }
    while ( v29 );
  }
  *a1 = v25;
  a1[1] = v3;
  a1[2] = v4;
  a1[3] = result;
  return result;
}

// ===== sub_4A1C90 @ 0x004A1C90..0x004A1CA1 =====
int __cdecl sub_4A1C90(int a1, int a2)
{
  int v2; // ecx

  return a1 & v2 | a2 & ~v2;
}

// ===== sub_4A1CB0 @ 0x004A1CB0..0x004A1CC0 =====
int __fastcall sub_4A1CB0(int a1, int a2, int a3)
{
  return a2 & a1 | a3 & ~a2;
}

// ===== sub_4A1CC0 @ 0x004A1CC0..0x004A1CCB =====
int __usercall sub_4A1CC0@<eax>(int a1@<eax>, int a2, int a3)
{
  return a3 ^ a2 ^ a1;
}

// ===== sub_4A1CD0 @ 0x004A1CD0..0x004A1CDD =====
int __usercall sub_4A1CD0@<eax>(int a1@<eax>, int a2, int a3)
{
  return a3 ^ (a2 | ~a1);
}

// ===== sub_4A1CE0 @ 0x004A1CE0..0x004A1D08 =====
int __usercall sub_4A1CE0@<eax>(int a1@<eax>, _DWORD *a2@<esi>, int a3, int a4, int a5)
{
  __int64 v5; // rax
  int result; // eax

  sub_4A1C90(a3, a1);
  v5 = sub_4A1E00(a5);
  result = HIDWORD(v5) + v5;
  *a2 = result;
  return result;
}

// ===== sub_4A1D10 @ 0x004A1D10..0x004A1D34 =====
int __usercall sub_4A1D10@<eax>(int a1@<eax>, int a2@<edx>, _DWORD *a3@<edi>, int a4@<esi>, int a5, int a6)
{
  int result; // eax

  sub_4A1CB0(a4, a2, a1);
  result = a4 + sub_4A1E00(a6);
  *a3 = result;
  return result;
}

// ===== sub_4A1D40 @ 0x004A1D40..0x004A1D68 =====
int __usercall sub_4A1D40@<eax>(int a1@<eax>, int a2@<edx>, _DWORD *a3@<esi>, int a4, int a5, int a6)
{
  __int64 v6; // rax
  int result; // eax

  sub_4A1CC0(a2, a4, a1);
  v6 = sub_4A1E00(a6);
  result = HIDWORD(v6) + v6;
  *a3 = result;
  return result;
}

// ===== sub_4A1D70 @ 0x004A1D70..0x004A1D96 =====
int __usercall sub_4A1D70@<eax>(int a1@<eax>, int a2@<edx>, _DWORD *a3@<esi>, int a4, int a5, int a6)
{
  __int64 v6; // rax
  int result; // eax

  sub_4A1CD0(a4, a2, a1);
  v6 = sub_4A1E00(a6);
  result = HIDWORD(v6) + v6;
  *a3 = result;
  return result;
}

// ===== sub_4A1DA0 @ 0x004A1DA0..0x004A1DB3 =====
unsigned int __fastcall sub_4A1DA0(char a1)
{
  unsigned int result; // eax

  result = 64 - (a1 & 0x3F);
  if ( result < 9 )
    result += 64;
  return result;
}

// ===== sub_4A1DC0 @ 0x004A1DC0..0x004A1DFC =====
int __usercall sub_4A1DC0@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3)
{
  int v4; // edi
  int result; // eax

  v4 = a1 + a2;
  *(_BYTE *)(a1 + a2) = 0x80;
  memset((void *)(a1 + a2 + 1), 0, a3 - 9);
  result = (unsigned __int64)a2 >> 29;
  *(_DWORD *)(v4 + a3 - 8) = 8 * a2;
  *(_DWORD *)(v4 + a3 - 4) = result;
  return result;
}

// ===== sub_4A1E00 @ 0x004A1E00..0x004A1E03 =====
int __usercall sub_4A1E00@<eax>(int a1@<eax>, char a2@<cl>)
{
  return __ROL4__(a1, a2);
}

// ===== ??4SThreadParam@@QAEAAU0@ABU0@@Z @ 0x004A1E10..0x004A1E2C =====
_DWORD *__thiscall SThreadParam::operator=(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = this;
  *this = *a2;
  this[1] = a2[1];
  this[2] = a2[2];
  return result;
}

// ===== sub_4A1E30 @ 0x004A1E30..0x004A1E38 =====
void __thiscall sub_4A1E30(_DWORD *this)
{
  this[1] = 0;
}

// ===== sub_4A1E40 @ 0x004A1E40..0x004A1FA7 =====
int __cdecl sub_4A1E40(HWND a1)
{
  int i; // esi
  int v2; // eax
  int j; // esi
  int v4; // eax
  int v6; // [esp+0h] [ebp-34h] BYREF
  void *v7; // [esp+14h] [ebp-20h]
  int v8; // [esp+18h] [ebp-1Ch] BYREF
  int pExceptionObject; // [esp+1Ch] [ebp-18h] BYREF
  int v10; // [esp+20h] [ebp-14h]
  int *v11; // [esp+24h] [ebp-10h]
  int v12; // [esp+30h] [ebp-4h]

  v11 = &v6;
  v12 = 0;
  if ( (dword_5085A4 & 1) != 0 )
  {
    pExceptionObject = 1;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  dword_5085A0 = a1;
  dword_5085A4 = 0;
  for ( i = 0; ; ++i )
  {
    v10 = i;
    if ( i >= 16 )
      break;
    v7 = operator new(0xC8u);
    LOBYTE(v12) = 1;
    if ( v7 )
      v2 = sub_4A71D0(dword_5085A0, 30, 100);
    else
      v2 = 0;
    LOBYTE(v12) = 0;
    dword_5085AC[17 * i] = v2;
  }
  for ( j = 0; ; ++j )
  {
    v10 = j;
    if ( j >= 64 )
      break;
    v7 = operator new(0x88u);
    LOBYTE(v12) = 2;
    if ( v7 )
      v4 = sub_4A6B00(dword_5085A0);
    else
      v4 = 0;
    LOBYTE(v12) = 0;
    dword_508A0C[17 * j] = v4;
  }
  if ( (sub_4A5FF0() & 1) == 0 )
  {
    v8 = 5;
    _CxxThrowException(&v8, (_ThrowInfo *)&_TI1K);
  }
  dword_5085A4 |= 1u;
  return 0;
}

// ===== sub_4A1FB0 @ 0x004A1FB0..0x004A1FDB =====
int sub_4A1FB0()
{
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  dword_5085A4 &= ~2u;
  KillTimer(dword_5085A0, 1u);
  return 0;
}

// ===== sub_4A1FE0 @ 0x004A1FE0..0x004A2046 =====
int __cdecl sub_4A1FE0(unsigned int a1)
{
  void (__thiscall ***v2)(_DWORD, int); // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x40 )
    return 21;
  v2 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(int))(*(_DWORD *)dword_508A0C[17 * a1] + 28))(dword_508A0C[17 * a1]);
  if ( v2 )
    (**v2)(v2, 1);
  dword_508A10[17 * a1] = 0;
  dword_508A08[17 * a1] = 0;
  return 0;
}

// ===== sub_4A2050 @ 0x004A2050..0x004A20B6 =====
int __cdecl sub_4A2050(unsigned int a1)
{
  void (__thiscall ***v2)(_DWORD, int); // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v2 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(int))(*(_DWORD *)dword_5085AC[17 * a1] + 28))(dword_5085AC[17 * a1]);
  if ( v2 )
    (**v2)(v2, 1);
  dword_5085B0[17 * a1] = 0;
  dword_5085A8[17 * a1] = 0;
  return 0;
}

// ===== sub_4A20C0 @ 0x004A20C0..0x004A213D =====
int sub_4A20C0()
{
  unsigned int v0; // edi
  int *v1; // esi
  unsigned int v2; // edi
  int *v3; // esi
  int result; // eax

  if ( (dword_5085A4 & 1) != 0 )
  {
    sub_4A1FB0();
    v0 = 0;
    v1 = dword_5085AC;
    do
    {
      sub_4A2050(v0);
      if ( *v1 )
        (**(void (__thiscall ***)(int, int))*v1)(*v1, 1);
      *v1 = 0;
      v1 += 17;
      ++v0;
    }
    while ( (int)v1 < (int)dword_5089EC );
    v2 = 0;
    v3 = dword_508A0C;
    do
    {
      result = sub_4A1FE0(v2);
      if ( *v3 )
        result = (**(int (__thiscall ***)(int, int))*v3)(*v3, 1);
      *v3 = 0;
      v3 += 17;
      ++v2;
    }
    while ( (int)v3 < (int)&dword_509B0C );
    dword_5085A4 = 0;
  }
  return result;
}

// ===== sub_4A2140 @ 0x004A2140..0x004A217F =====
int __cdecl sub_4A2140(unsigned int a1)
{
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 < 0x40 )
    return dword_508A08[17 * a1] != 0 ? 0 : 19;
  return 21;
}

// ===== sub_4A2180 @ 0x004A2180..0x004A21BF =====
int __cdecl sub_4A2180(unsigned int a1)
{
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 < 0x10 )
    return dword_5085A8[17 * a1] != 0 ? 0 : 19;
  return 21;
}

// ===== sub_4A21C0 @ 0x004A21C0..0x004A227F =====
int __cdecl sub_4A21C0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // edi

  v1 = a1[10];
  v2 = a1[3];
  v3 = a1[4];
  if ( (double)(128 - v2) / 2.66666666 >= 48.0
    || (double)(128 - v3) / 2.66666666 >= 48.0
    || (double)(128 - v1) / 2.66666666 >= 48.0
    || (double)(128 - a1[16]) / 2.66666666 >= 48.0 )
  {
    return 128;
  }
  else
  {
    return (int)((double)(512 - v2 - v3 - v1 - a1[16]) / 2.66666666);
  }
}

// ===== sub_4A2280 @ 0x004A2280..0x004A22F3 =====
int __cdecl sub_4A2280(_DWORD *a1, int a2)
{
  int v2; // ecx
  double v3; // st6
  int result; // eax
  int v5; // [esp+8h] [ebp-Ch]
  int v6; // [esp+Ch] [ebp-8h]
  int v7; // [esp+10h] [ebp-4h]

  v2 = a1[1];
  v7 = a1[2] - v2;
  v5 = a1[3];
  v6 = a1[4] - v5;
  if ( v7 > 0 && (v3 = (double)(a2 - v2) / (double)v7, v3 < 1.0) )
  {
    result = (int)(v3 * (double)v6 + (double)v5);
    a1[5] = result;
  }
  else
  {
    *a1 = 0;
    result = (int)(1.0 * (double)v6 + (double)v5);
    a1[5] = result;
  }
  return result;
}

// ===== sub_4A2300 @ 0x004A2300..0x004A237E =====
int __cdecl sub_4A2300(int a1, int a2, int a3)
{
  int result; // eax
  int v4; // edi
  _DWORD *v5; // esi
  int v6; // ecx
  int v7; // edi
  int v8; // eax

  result = a2;
  if ( a2 > 0 )
  {
    v4 = a3;
    v5 = (_DWORD *)(a1 + 44);
    do
    {
      v6 = 0;
      if ( *(v5 - 11) )
      {
        result = (int)(v5 - 6);
        if ( *(v5 - 6) )
        {
          result = sub_4A2280(v5 - 6, v4);
          v6 = 1;
        }
        if ( *v5 )
        {
          sub_4A2280(v5, v4);
LABEL_9:
          v7 = *(v5 - 10);
          v8 = sub_4A21C0(v5 - 11);
          result = (*(int (__thiscall **)(int, int))(*(_DWORD *)v7 + 24))(v7, v8);
          v4 = a3;
          goto LABEL_10;
        }
        if ( v6 )
          goto LABEL_9;
      }
LABEL_10:
      v5 += 17;
      --a2;
    }
    while ( a2 );
  }
  return result;
}

// ===== sub_4A2380 @ 0x004A2380..0x004A23D4 =====
_DWORD *__thiscall sub_4A2380(_DWORD *this)
{
  sub_4A7B00();
  *this = &CStaticBurikoWaveBoxADPCM4::`vftable';
  return this;
}

// ===== sub_4A23E0 @ 0x004A23E0..0x004A2434 =====
_DWORD *__thiscall sub_4A23E0(_DWORD *this)
{
  sub_4A8770();
  *this = &CStaticBurikoWaveBoxHFADPCM8::`vftable';
  return this;
}

// ===== sub_4A2440 @ 0x004A2440..0x004A2494 =====
_DWORD *__thiscall sub_4A2440(_DWORD *this)
{
  sub_4A8E00();
  *this = &CStaticBurikoWaveBoxOGG::`vftable';
  return this;
}

// ===== sub_4A24A0 @ 0x004A24A0..0x004A24C1 =====
void *__thiscall sub_4A24A0(void *this, char a2)
{
  sub_4A8710();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A24D0 @ 0x004A24D0..0x004A24F1 =====
void *__thiscall sub_4A24D0(void *this, char a2)
{
  sub_4A8DA0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A2500 @ 0x004A2500..0x004A2521 =====
void *__thiscall sub_4A2500(void *this, char a2)
{
  sub_4A9330();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A2530 @ 0x004A2530..0x004A2551 =====
void *__thiscall sub_4A2530(void *this, char a2)
{
  sub_4A9390();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4A2560 @ 0x004A2560..0x004A2591 =====
int __cdecl sub_4A2560(unsigned int a1, _DWORD *a2)
{
  int result; // eax
  int v3; // edx

  result = sub_4A2180(a1);
  if ( !result )
  {
    *a2 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_5085AC[17 * v3] + 32))(dword_5085AC[17 * v3]);
    return 0;
  }
  return result;
}

// ===== sub_4A25A0 @ 0x004A25A0..0x004A25CF =====
int __cdecl sub_4A25A0(unsigned int a1, _DWORD *a2)
{
  int result; // eax
  int v3; // edx

  result = sub_4A2180(a1);
  if ( !result )
  {
    *a2 = sub_4A84E0(dword_5085B0[17 * v3]);
    return 0;
  }
  return result;
}

// ===== sub_4A25D0 @ 0x004A25D0..0x004A25F4 =====
void __thiscall sub_4A25D0(_DWORD *this)
{
  *this = 0;
  this[1] = 0;
  this[2] = 0;
  this[3] = 128;
  this[4] = 128;
  this[5] = 0;
  this[10] = 128;
  this[11] = 0;
  this[16] = 128;
}

// ===== sub_4A2600 @ 0x004A2600..0x004A2685 =====
int __cdecl sub_4A2600(HWND a1)
{
  int v1; // esi
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  sub_4A4A80(&unk_5089E8);
  v3[8] = 0;
  v1 = sub_4A1E40(a1);
  v3[4] = v1;
  sub_4A4A90(&unk_5089E8);
  return v1;
}

// ===== sub_4A2690 @ 0x004A2690..0x004A270E =====
int sub_4A2690()
{
  int v0; // esi
  _DWORD v2[9]; // [esp+0h] [ebp-24h] BYREF

  v2[5] = v2;
  sub_4A4A80(&unk_5089E8);
  v2[8] = 0;
  v0 = sub_4A1FB0();
  v2[4] = v0;
  sub_4A4A90(&unk_5089E8);
  return v0;
}

// ===== sub_4A2710 @ 0x004A2710..0x004A2795 =====
int __cdecl sub_4A2710(unsigned int a1)
{
  int v1; // esi
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  sub_4A4A80(&unk_5089E8);
  v3[8] = 0;
  v1 = sub_4A1FE0(a1);
  v3[4] = v1;
  sub_4A4A90(&unk_5089E8);
  return v1;
}

// ===== sub_4A27A0 @ 0x004A27A0..0x004A2815 =====
int sub_4A27A0()
{
  _DWORD v1[8]; // [esp+0h] [ebp-20h] BYREF

  v1[4] = v1;
  sub_4A4A80(&unk_5089E8);
  v1[7] = 0;
  sub_4A20C0();
  return sub_4A4A90(&unk_5089E8);
}

// ===== sub_4A2820 @ 0x004A2820..0x004A288D =====
int __cdecl sub_4A2820(unsigned int a1, unsigned int a2)
{
  int v3; // ecx
  int *v4; // esi
  int v5; // edi
  int v6; // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x40 )
    return 21;
  v3 = a2;
  if ( a2 > 0x80 )
    v3 = 128;
  v4 = (int *)dword_508A0C[17 * a1];
  dword_508A14[17 * a1] = v3;
  v5 = *v4;
  v6 = sub_4A21C0(&dword_508A08[17 * a1]);
  (*(void (__thiscall **)(int *, int))(v5 + 24))(v4, v6);
  return 0;
}

// ===== sub_4A2890 @ 0x004A2890..0x004A28FD =====
int __cdecl sub_4A2890(unsigned int a1, unsigned int a2)
{
  int v3; // ecx
  int *v4; // esi
  int v5; // edi
  int v6; // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x40 )
    return 21;
  v3 = a2;
  if ( a2 > 0x80 )
    v3 = 128;
  v4 = (int *)dword_508A0C[17 * a1];
  dword_508A18[17 * a1] = v3;
  v5 = *v4;
  v6 = sub_4A21C0(&dword_508A08[17 * a1]);
  (*(void (__thiscall **)(int *, int))(v5 + 24))(v4, v6);
  return 0;
}

// ===== sub_4A2900 @ 0x004A2900..0x004A2953 =====
int __cdecl sub_4A2900(unsigned int a1, int a2)
{
  int result; // eax
  int v3; // edx

  result = sub_4A2140(a1);
  if ( !result )
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)dword_508A0C[17 * v3] + 20))(
      dword_508A0C[17 * v3],
      (int)((double)(a2 - 64) * 0.015625 * 128.0));
    return 0;
  }
  return result;
}

// ===== sub_4A2960 @ 0x004A2960..0x004A298C =====
int __cdecl sub_4A2960(unsigned int a1)
{
  int result; // eax
  int v2; // edx

  result = sub_4A2140(a1);
  if ( !result )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)dword_508A0C[17 * v2] + 16))(dword_508A0C[17 * v2]);
    return 0;
  }
  return result;
}

// ===== sub_4A2990 @ 0x004A2990..0x004A29E9 =====
int __cdecl sub_4A2990(unsigned int a1, int a2)
{
  int result; // eax
  DWORD TickCount; // eax
  int v4; // ecx

  result = sub_4A2180(a1);
  if ( !result )
  {
    TickCount = GetTickCount();
    v4 = 17 * a1;
    dword_5085D8[v4] = TickCount;
    dword_5085E4[v4] = 0;
    dword_5085DC[v4] = a2 + TickCount;
    dword_5085E0[v4] = dword_5085E8[17 * a1];
    dword_5085D4[v4] = 1;
    return 0;
  }
  return result;
}

// ===== sub_4A29F0 @ 0x004A29F0..0x004A2A49 =====
int __cdecl sub_4A29F0(unsigned int a1, int a2)
{
  int result; // eax
  DWORD TickCount; // eax
  int v4; // ecx

  result = sub_4A2180(a1);
  if ( !result )
  {
    TickCount = GetTickCount();
    v4 = 17 * a1;
    dword_5085D8[v4] = TickCount;
    dword_5085E4[v4] = 128;
    dword_5085DC[v4] = a2 + TickCount;
    dword_5085E0[v4] = dword_5085E8[17 * a1];
    dword_5085D4[v4] = 1;
    return 0;
  }
  return result;
}

// ===== sub_4A2A50 @ 0x004A2A50..0x004A2AA9 =====
int __cdecl sub_4A2A50(unsigned int a1, int a2)
{
  int result; // eax
  DWORD TickCount; // eax
  int v4; // ecx

  result = sub_4A2140(a1);
  if ( !result )
  {
    TickCount = GetTickCount();
    v4 = 17 * a1;
    dword_508A38[v4] = TickCount;
    dword_508A44[v4] = 0;
    dword_508A3C[v4] = a2 + TickCount;
    dword_508A40[v4] = dword_508A48[17 * a1];
    dword_508A34[v4] = 1;
    return 0;
  }
  return result;
}

// ===== sub_4A2AB0 @ 0x004A2AB0..0x004A2AE0 =====
int __cdecl sub_4A2AB0(unsigned int a1, int a2)
{
  int result; // eax
  int v3; // edx

  result = sub_4A2180(a1);
  if ( !result )
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)dword_5085AC[17 * v3] + 12))(dword_5085AC[17 * v3], a2);
    return 0;
  }
  return result;
}

// ===== sub_4A2AE0 @ 0x004A2AE0..0x004A2B47 =====
int __cdecl sub_4A2AE0(unsigned int a1, unsigned int a2, int a3)
{
  int result; // eax
  int v4; // edi
  DWORD TickCount; // eax
  int v6; // ecx

  result = sub_4A2180(a1);
  if ( !result )
  {
    v4 = a2;
    if ( a2 > 0x80 )
      v4 = 128;
    TickCount = GetTickCount();
    v6 = 17 * a1;
    dword_5085C0[v6] = TickCount;
    dword_5085CC[v6] = v4;
    dword_5085C4[v6] = a3 + TickCount;
    dword_5085C8[v6] = dword_5085D0[17 * a1];
    dword_5085BC[v6] = 1;
    return 0;
  }
  return result;
}

// ===== sub_4A2B50 @ 0x004A2B50..0x004A2BB0 =====
int __cdecl sub_4A2B50(unsigned int a1, int a2)
{
  int *v3; // esi
  int v4; // edi
  int v5; // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v3 = (int *)dword_5085AC[17 * a1];
  dword_5085B4[17 * a1] = a2;
  v4 = *v3;
  v5 = sub_4A21C0(&dword_5085A8[17 * a1]);
  (*(void (__thiscall **)(int *, int))(v4 + 24))(v3, v5);
  return 0;
}

// ===== sub_4A2BB0 @ 0x004A2BB0..0x004A2C1D =====
int __cdecl sub_4A2BB0(unsigned int a1, unsigned int a2)
{
  int v3; // ecx
  int *v4; // esi
  int v5; // edi
  int v6; // eax

  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v3 = a2;
  if ( a2 > 0x80 )
    v3 = 128;
  v4 = (int *)dword_5085AC[17 * a1];
  dword_5085B8[17 * a1] = v3;
  v5 = *v4;
  v6 = sub_4A21C0(&dword_5085A8[17 * a1]);
  (*(void (__thiscall **)(int *, int))(v5 + 24))(v4, v6);
  return 0;
}

// ===== sub_4A2C20 @ 0x004A2C20..0x004A2C87 =====
int __cdecl sub_4A2C20(unsigned int a1, int a2)
{
  int result; // eax
  int v3; // edx
  int v4; // eax

  result = sub_4A2180(a1);
  if ( !result )
  {
    v4 = a2;
    if ( a2 <= 128 )
    {
      if ( a2 < 0 )
        v4 = 0;
    }
    else
    {
      v4 = 128;
    }
    (*(void (__thiscall **)(int, int))(*(_DWORD *)dword_5085AC[17 * v3] + 20))(
      dword_5085AC[17 * v3],
      (int)((double)(v4 - 64) * 0.015625 * 128.0));
    return 0;
  }
  return result;
}

// ===== sub_4A2C90 @ 0x004A2C90..0x004A2CBC =====
int __cdecl sub_4A2C90(unsigned int a1)
{
  int result; // eax
  int v2; // edx

  result = sub_4A2180(a1);
  if ( !result )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)dword_5085AC[17 * v2] + 16))(dword_5085AC[17 * v2]);
    return 0;
  }
  return result;
}

// ===== sub_4A2CC0 @ 0x004A2CC0..0x004A2CE8 =====
int sub_4A2CC0()
{
  DWORD TickCount; // esi

  TickCount = GetTickCount();
  sub_4A2300((int)dword_5085A8, 16, TickCount);
  return sub_4A2300((int)dword_508A08, 64, TickCount);
}

// ===== sub_4A2CF0 @ 0x004A2CF0..0x004A2D51 =====
_DWORD *__thiscall sub_4A2CF0(_DWORD *this)
{
  sub_4A82B0();
  *this = &CBurikoWaveBoxPCM16Model::`vftable';
  *this = &CStaticBurikoWaveBoxPCM16::`vftable';
  return this;
}

// ===== sub_4A2D60 @ 0x004A2D60..0x004A2D81 =====
void *__thiscall sub_4A2D60(void *this, char a2)
{
  sub_4A96E0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== TimerFunc @ 0x004A2D90..0x004A2DED =====
void __stdcall TimerFunc(HWND a1, UINT a2, UINT_PTR a3, DWORD a4)
{
  _DWORD v4[8]; // [esp+0h] [ebp-20h] BYREF

  v4[4] = v4;
  v4[7] = 0;
  sub_4A2CC0();
}

// ===== sub_4A2DF0 @ 0x004A2DF0..0x004A2E79 =====
int __cdecl sub_4A2DF0(unsigned int a1, unsigned int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2820(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A2E80 @ 0x004A2E80..0x004A2F09 =====
int __cdecl sub_4A2E80(unsigned int a1, unsigned int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2890(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A2F10 @ 0x004A2F10..0x004A2F95 =====
int __cdecl sub_4A2F10(unsigned int a1)
{
  int v1; // esi
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  sub_4A4A80(&unk_5089E8);
  v3[8] = 0;
  v1 = sub_4A2960(a1);
  v3[4] = v1;
  sub_4A4A90(&unk_5089E8);
  return v1;
}

// ===== sub_4A2FA0 @ 0x004A2FA0..0x004A3029 =====
int __cdecl sub_4A2FA0(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2990(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A3030 @ 0x004A3030..0x004A30B9 =====
int __cdecl sub_4A3030(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A29F0(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A30C0 @ 0x004A30C0..0x004A3149 =====
int __cdecl sub_4A30C0(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2A50(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A3150 @ 0x004A3150..0x004A31D9 =====
int __cdecl sub_4A3150(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2AB0(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A31E0 @ 0x004A31E0..0x004A326D =====
int __cdecl sub_4A31E0(unsigned int a1, unsigned int a2, int a3)
{
  int v3; // esi
  _DWORD v5[9]; // [esp+0h] [ebp-24h] BYREF

  v5[5] = v5;
  sub_4A4A80(&unk_5089E8);
  v5[8] = 0;
  v3 = sub_4A2AE0(a1, a2, a3);
  v5[4] = v3;
  sub_4A4A90(&unk_5089E8);
  return v3;
}

// ===== sub_4A3270 @ 0x004A3270..0x004A32F9 =====
int __cdecl sub_4A3270(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2B50(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A3300 @ 0x004A3300..0x004A3389 =====
int __cdecl sub_4A3300(unsigned int a1, unsigned int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2BB0(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A3390 @ 0x004A3390..0x004A3419 =====
int __cdecl sub_4A3390(unsigned int a1, int a2)
{
  int v2; // esi
  _DWORD v4[9]; // [esp+0h] [ebp-24h] BYREF

  v4[5] = v4;
  sub_4A4A80(&unk_5089E8);
  v4[8] = 0;
  v2 = sub_4A2C20(a1, a2);
  v4[4] = v2;
  sub_4A4A90(&unk_5089E8);
  return v2;
}

// ===== sub_4A3420 @ 0x004A3420..0x004A34A5 =====
int __cdecl sub_4A3420(unsigned int a1)
{
  int v1; // esi
  _DWORD v3[9]; // [esp+0h] [ebp-24h] BYREF

  v3[5] = v3;
  sub_4A4A80(&unk_5089E8);
  v3[8] = 0;
  v1 = sub_4A2C90(a1);
  v3[4] = v1;
  sub_4A4A90(&unk_5089E8);
  return v1;
}

// ===== sub_4A34B0 @ 0x004A34B0..0x004A34DE =====
int sub_4A34B0()
{
  if ( (dword_5085A4 & 1) == 0 )
    return 20;
  dword_5085A4 |= 2u;
  SetTimer(dword_5085A0, 1u, 0x14u, TimerFunc);
  return 0;
}

// ===== sub_4A34E0 @ 0x004A34E0..0x004A3568 =====
int __cdecl sub_4A34E0(unsigned int a1, int a2, int a3)
{
  int result; // eax
  int v4; // ebx
  int v5; // esi
  int v6; // ecx
  int v7; // edi
  int v8; // eax

  result = sub_4A2140(a1);
  v4 = result;
  if ( !result )
  {
    sub_4A2900(a1, a3);
    v5 = 17 * a1;
    dword_508A30[v5] = a2;
    v6 = dword_508A0C[17 * a1];
    dword_508A34[v5] = 0;
    dword_508A48[v5] = 128;
    (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 16))(v6);
    v7 = dword_508A0C[17 * a1];
    v8 = sub_4A21C0(&dword_508A08[17 * a1]);
    if ( (*(int (__thiscall **)(int, int))(*(_DWORD *)v7 + 8))(v7, v8) )
      return 23;
    return v4;
  }
  return result;
}

// ===== sub_4A3570 @ 0x004A3570..0x004A37F8 =====
int __cdecl sub_4A3570(int a1, int a2, int a3, int a4, double a5, char a6)
{
  void *v6; // eax
  int v7; // eax
  void *v8; // eax
  void *v9; // eax
  void *v10; // eax
  int v11; // edi
  unsigned int v12; // esi
  void (__thiscall ***v13)(_DWORD, int); // eax
  int *v14; // eax
  int *v15; // esi
  int v16; // edi
  int v17; // eax
  int v19; // [esp+8h] [ebp-8Ch] BYREF
  _BYTE v20[4]; // [esp+18h] [ebp-7Ch] BYREF
  int v21; // [esp+1Ch] [ebp-78h]
  int v22; // [esp+24h] [ebp-70h] BYREF
  int pExceptionObject; // [esp+28h] [ebp-6Ch] BYREF
  int v24; // [esp+2Ch] [ebp-68h] BYREF
  int v25; // [esp+30h] [ebp-64h] BYREF
  void *v26; // [esp+38h] [ebp-5Ch]
  unsigned int v27; // [esp+3Ch] [ebp-58h]
  _BYTE v28[48]; // [esp+40h] [ebp-54h] BYREF
  int v29; // [esp+70h] [ebp-24h]
  int *v30; // [esp+84h] [ebp-10h]
  int v31; // [esp+90h] [ebp-4h]

  v30 = &v19;
  v27 = a1;
  v31 = 0;
  if ( (unsigned int)(*(int (__thiscall **)(int, _BYTE *, int))(*(_DWORD *)a2 + 8))(a2, v28, 64) < 0x40 )
  {
    (**(void (__thiscall ***)(int, int))a2)(a2, 1);
    pExceptionObject = 14;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  switch ( v29 )
  {
    case 0:
      v6 = operator new(0xD0u);
      v26 = v6;
      LOBYTE(v31) = 1;
      if ( !v6 )
        goto LABEL_12;
      v7 = sub_4AA2D0(v6);
      break;
    case 1:
      v8 = operator new(0xB0u);
      v26 = v8;
      LOBYTE(v31) = 2;
      if ( !v8 )
        goto LABEL_12;
      v7 = sub_4A9F50(v8);
      break;
    case 2:
      v9 = operator new(0x8E8u);
      v26 = v9;
      LOBYTE(v31) = 3;
      if ( !v9 )
        goto LABEL_12;
      v7 = sub_4A9C80(v9);
      break;
    case 3:
      v10 = operator new(0x388u);
      v26 = v10;
      LOBYTE(v31) = 4;
      if ( v10 )
        v7 = sub_4A98C0(v10);
      else
LABEL_12:
        v7 = 0;
      break;
    default:
      (**(void (__thiscall ***)(int, int))a2)(a2, 1);
      v22 = 14;
      _CxxThrowException(&v22, (_ThrowInfo *)&_TI1K);
  }
  LOBYTE(v31) = 0;
  v11 = v7;
  (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)a2 + 20))(a2, 0);
  if ( (*(int (__thiscall **)(int, int, _DWORD, _DWORD))(*(_DWORD *)v11 + 32))(v11, a2, LODWORD(a5), HIDWORD(a5)) )
  {
    (**(void (__thiscall ***)(int, int))v11)(v11, 1);
    v24 = 14;
    _CxxThrowException(&v24, (_ThrowInfo *)&_TI1K);
  }
  if ( (a6 & 1) != 0 )
    sub_4A8520(a6 & 2, 0);
  v12 = 17 * v27;
  v13 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(int))(*(_DWORD *)dword_5085AC[17 * v27] + 28))(dword_5085AC[17 * v27]);
  if ( v13 )
    (**v13)(v13, 1);
  dword_5085B0[v12] = v11;
  v21 = v11;
  if ( (*(int (__thiscall **)(int, _BYTE *))(*(_DWORD *)dword_5085AC[v12] + 4))(dword_5085AC[v12], v20) )
  {
    v25 = 22;
    _CxxThrowException(&v25, (_ThrowInfo *)&_TI1K);
  }
  dword_5085BC[v12] = 0;
  dword_5085D0[v12] = 128;
  dword_5085D0[v12] = a3;
  dword_5085D4[v12] = 0;
  dword_5085E8[v12] = 128;
  v14 = &dword_5085A8[v12];
  *v14 = 1;
  v15 = (int *)dword_5085AC[v12];
  v16 = *v15;
  v17 = sub_4A21C0(v14);
  (*(void (__thiscall **)(int *, int))(v16 + 24))(v15, v17);
  sub_4A2C20(v27, a4);
  v31 = -1;
  return 0;
}

// ===== sub_4A3810 @ 0x004A3810..0x004A3AFC =====
int __cdecl sub_4A3810(int a1, int a2, int a3, int a4, int a5, int a6, double a7)
{
  int v7; // ebx
  void *v8; // eax
  unsigned int v9; // edi
  int v10; // esi
  void (__thiscall ***v11)(_DWORD, int); // eax
  int *v12; // eax
  int *v13; // esi
  int v14; // ebx
  int v15; // eax
  int v17; // [esp+40h] [ebp-D4h] BYREF
  _BYTE v18[4]; // [esp+54h] [ebp-C0h] BYREF
  int v19; // [esp+58h] [ebp-BCh]
  void *v20; // [esp+5Ch] [ebp-B8h]
  int pExceptionObject; // [esp+60h] [ebp-B4h] BYREF
  int v22; // [esp+64h] [ebp-B0h] BYREF
  int v23; // [esp+68h] [ebp-ACh] BYREF
  int v24; // [esp+6Ch] [ebp-A8h] BYREF
  int v25; // [esp+70h] [ebp-A4h] BYREF
  int v26; // [esp+74h] [ebp-A0h] BYREF
  int v27; // [esp+78h] [ebp-9Ch]
  _BYTE v28[48]; // [esp+80h] [ebp-94h] BYREF
  int v29; // [esp+B0h] [ebp-64h]
  _BYTE v30[48]; // [esp+C0h] [ebp-54h] BYREF
  int v31; // [esp+F0h] [ebp-24h]
  int *v32; // [esp+104h] [ebp-10h]
  int v33; // [esp+110h] [ebp-4h]

  v32 = &v17;
  v27 = a1;
  v7 = 0;
  v33 = 0;
  if ( (unsigned int)(*(int (__thiscall **)(int, _BYTE *, int))(*(_DWORD *)a2 + 8))(a2, v28, 64) < 0x40 )
  {
    (**(void (__thiscall ***)(int, int))a2)(a2, 1);
    if ( a3 )
      (**(void (__thiscall ***)(int, int))a3)(a3, 1);
    pExceptionObject = 14;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  if ( (unsigned int)(*(int (__thiscall **)(int, _BYTE *, int))(*(_DWORD *)a3 + 8))(a3, v30, 64) < 0x40 )
  {
    (**(void (__thiscall ***)(int, int))a2)(a2, 1);
    (**(void (__thiscall ***)(int, int))a3)(a3, 1);
    v22 = 14;
    _CxxThrowException(&v22, (_ThrowInfo *)&_TI1K);
  }
  if ( v29 != v31 )
  {
    (**(void (__thiscall ***)(int, int))a2)(a2, 1);
    (**(void (__thiscall ***)(int, int))a3)(a3, 1);
    v23 = 14;
    _CxxThrowException(&v23, (_ThrowInfo *)&_TI1K);
  }
  if ( v29 != 3 )
  {
    (**(void (__thiscall ***)(int, int))a2)(a2, 1);
    (**(void (__thiscall ***)(int, int))a3)(a3, 1);
    v26 = 14;
    _CxxThrowException(&v26, (_ThrowInfo *)&_TI1K);
  }
  v8 = operator new(0x9B8u);
  v20 = v8;
  LOBYTE(v33) = 1;
  if ( v8 )
    v7 = sub_4AA7D0(v8);
  LOBYTE(v33) = 0;
  (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)a2 + 20))(a2, 0);
  (*(void (__thiscall **)(int, _DWORD))(*(_DWORD *)a3 + 20))(a3, 0);
  if ( (*(int (__thiscall **)(int, int, int, _DWORD, _DWORD))(*(_DWORD *)v7 + 64))(v7, a2, a3, LODWORD(a7), HIDWORD(a7)) )
  {
    (**(void (__thiscall ***)(int, int))v7)(v7, 1);
    v24 = 14;
    _CxxThrowException(&v24, (_ThrowInfo *)&_TI1K);
  }
  sub_4AA7C0(a4);
  v9 = v27;
  v10 = 17 * v27;
  v11 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(int))(*(_DWORD *)dword_5085AC[17 * v27] + 28))(dword_5085AC[17 * v27]);
  if ( v11 )
    (**v11)(v11, 1);
  dword_5085B0[v10] = v7;
  v19 = v7;
  if ( (*(int (__thiscall **)(int, _BYTE *))(*(_DWORD *)dword_5085AC[v10] + 4))(dword_5085AC[v10], v18) )
  {
    v25 = 22;
    _CxxThrowException(&v25, (_ThrowInfo *)&_TI1K);
  }
  dword_5085BC[v10] = 0;
  dword_5085D0[v10] = 128;
  dword_5085D0[v10] = a5;
  dword_5085D4[v10] = 0;
  dword_5085E8[v10] = 128;
  v12 = &dword_5085A8[v10];
  *v12 = 1;
  v13 = (int *)dword_5085AC[v10];
  v14 = *v13;
  v15 = sub_4A21C0(v12);
  (*(void (__thiscall **)(int *, int))(v14 + 24))(v13, v15);
  sub_4A2C20(v9, a6);
  return 0;
}

// ===== sub_4A3B00 @ 0x004A3B00..0x004A3D14 =====
int __cdecl sub_4A3B00(
        int a1,
        void (__thiscall ***a2)(_DWORD, int),
        int pExceptionObject,
        int a4,
        double a5,
        double a6)
{
  _DWORD *v6; // eax
  _DWORD *v7; // eax
  _DWORD *v8; // eax
  _DWORD *v9; // eax
  _DWORD *v10; // eax
  void (__thiscall ***v11)(_DWORD, int); // edi
  bool v12; // zf
  void (__thiscall **v13)(_DWORD, int); // eax
  void (__thiscall ***v15)(_DWORD, int); // eax
  int v16; // ecx
  int *v17; // eax
  int *v18; // esi
  int v19; // edi
  int v20; // eax
  char v21[4]; // [esp+28h] [ebp-18h] BYREF
  void (__thiscall ***v22)(_DWORD, int); // [esp+2Ch] [ebp-14h]
  void *v23; // [esp+30h] [ebp-10h]
  int v24; // [esp+3Ch] [ebp-4h]

  switch ( *(_DWORD *)(pExceptionObject + 48) )
  {
    case 0:
      v6 = operator new(0x110u);
      v23 = v6;
      v24 = 0;
      if ( !v6 )
        goto LABEL_10;
      v7 = sub_4A2380(v6);
      break;
    case 1:
      v8 = operator new(0xF0u);
      v23 = v8;
      v24 = 1;
      if ( !v8 )
        goto LABEL_10;
      v7 = sub_4A2CF0(v8);
      break;
    case 2:
      v9 = operator new(0x928u);
      v23 = v9;
      v24 = 2;
      if ( !v9 )
        goto LABEL_10;
      v7 = sub_4A23E0(v9);
      break;
    case 3:
      v10 = operator new(0x3C8u);
      v23 = v10;
      v24 = 3;
      if ( v10 )
        v7 = sub_4A2440(v10);
      else
LABEL_10:
        v7 = 0;
      break;
    default:
      if ( a2 )
        (**a2)(a2, 1);
      pExceptionObject = 14;
      _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  v24 = -1;
  v11 = (void (__thiscall ***)(_DWORD, int))v7;
  v12 = (*(int (__thiscall **)(_DWORD *, void (__thiscall ***)(_DWORD, int), _DWORD, _DWORD))(*v7 + 32))(
          v7,
          a2,
          LODWORD(a5),
          HIDWORD(a5)) == 0;
  v13 = *v11;
  if ( v12 )
  {
    v13[13](v11, a4);
    v15 = (void (__thiscall ***)(_DWORD, int))(*(int (__thiscall **)(int))(*(_DWORD *)dword_508A0C[17 * a1] + 28))(dword_508A0C[17 * a1]);
    if ( v15 )
      (**v15)(v15, 1);
    (*(void (__thiscall **)(int, _DWORD, _DWORD))(*(_DWORD *)dword_508A0C[17 * a1] + 36))(
      dword_508A0C[17 * a1],
      LODWORD(a6),
      HIDWORD(a6));
    v16 = dword_508A0C[17 * a1];
    dword_508A10[17 * a1] = (int)v11;
    v22 = v11;
    if ( (*(int (__thiscall **)(int, char *))(*(_DWORD *)v16 + 4))(v16, v21) )
    {
      return 22;
    }
    else
    {
      dword_508A1C[17 * a1] = 0;
      dword_508A30[17 * a1] = 128;
      dword_508A48[17 * a1] = 128;
      dword_508A34[17 * a1] = 0;
      v17 = &dword_508A08[17 * a1];
      v18 = (int *)dword_508A0C[17 * a1];
      *v17 = 1;
      v19 = *v18;
      v20 = sub_4A21C0(v17);
      (*(void (__thiscall **)(int *, int))(v19 + 24))(v18, v20);
      return 0;
    }
  }
  else
  {
    (*v13)(v11, 1);
    return 14;
  }
}

// ===== sub_4A3D30 @ 0x004A3D30..0x004A3DAE =====
int sub_4A3D30()
{
  int v0; // esi
  _DWORD v2[9]; // [esp+0h] [ebp-24h] BYREF

  v2[5] = v2;
  sub_4A4A80(&unk_5089E8);
  v2[8] = 0;
  v0 = sub_4A34B0();
  v2[4] = v0;
  sub_4A4A90(&unk_5089E8);
  return v0;
}

// ===== sub_4A3DB0 @ 0x004A3DB0..0x004A3E3D =====
int __cdecl sub_4A3DB0(unsigned int a1, int a2, int a3)
{
  int v3; // esi
  _DWORD v5[9]; // [esp+0h] [ebp-24h] BYREF

  v5[5] = v5;
  sub_4A4A80(&unk_5089E8);
  v5[8] = 0;
  v3 = sub_4A34E0(a1, a2, a3);
  v5[4] = v3;
  sub_4A4A90(&unk_5089E8);
  return v3;
}

// ===== sub_4A3E40 @ 0x004A3E40..0x004A3F8C =====
int __cdecl sub_4A3E40(unsigned int a1, const void *a2, int a3, double a4, double a5)
{
  void (__thiscall ***v6)(_DWORD, int); // esi
  int v7; // [esp+20h] [ebp-70h] BYREF
  int v8[16]; // [esp+30h] [ebp-60h] BYREF
  void *v9; // [esp+74h] [ebp-1Ch]
  _DWORD pExceptionObject[3]; // [esp+78h] [ebp-18h] BYREF
  int v11; // [esp+8Ch] [ebp-4h]

  pExceptionObject[2] = &v7;
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x40 )
    return 21;
  v11 = 0;
  qmemcpy(v8, a2, sizeof(v8));
  v9 = operator new(0x20u);
  LOBYTE(v11) = 1;
  if ( v9 )
    v6 = (void (__thiscall ***)(_DWORD, int))sub_4A5200(v8[0] + v8[2]);
  else
    v6 = 0;
  LOBYTE(v11) = 0;
  (*v6)[4](v6, 3);
  ((void (__thiscall *)(void (__thiscall ***)(_DWORD, int), const void *, int))v6[2][2])(v6 + 2, a2, v8[0] + v8[2]);
  (*v6)[5](v6, 0);
  if ( sub_4A3B00(a1, v6, (int)v8, a3, a4, a5) )
  {
    pExceptionObject[0] = 22;
    _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  return 0;
}

// ===== sub_4A3F90 @ 0x004A3F90..0x004A40C1 =====
int __cdecl sub_4A3F90(unsigned int a1, LPCSTR lpFileName, int a3, int a4, double a5, char a6)
{
  void (__thiscall ***v7)(_DWORD, int); // esi
  void *v8[4]; // [esp+10h] [ebp-30h] BYREF
  void *v9; // [esp+24h] [ebp-1Ch]
  _DWORD pExceptionObject[3]; // [esp+28h] [ebp-18h] BYREF
  int v11; // [esp+3Ch] [ebp-4h]

  pExceptionObject[2] = v8;
  if ( (dword_5085A4 & 3) != 3 )
    return 20;
  if ( a1 >= 0x10 )
    return 21;
  v11 = 0;
  v9 = operator new(0x1Cu);
  LOBYTE(v11) = 1;
  if ( v9 )
    v7 = (void (__thiscall ***)(_DWORD, int))sub_4A4C60(v8[0], v8[1]);
  else
    v7 = 0;
  LOBYTE(v11) = 0;
  if ( !sub_4A4B00(lpFileName) )
  {
    if ( v7 )
      (**v7)(v7, 1);
    pExceptionObject[0] = 12;
    _CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  return sub_4A3570(a1, (int)v7, a3, a4, a5, a6);
}
