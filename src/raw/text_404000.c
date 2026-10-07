#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_404130 @ 0x00404130..0x0040428A =====
int __cdecl sub_404130(_DWORD *a1, int a2, int a3, unsigned int a4, unsigned int a5, unsigned int a6)
{
  int v6; // ebx
  int v7; // ecx
  char *v8; // edi
  void *v9; // ebx
  unsigned int v10; // esi
  long double v11; // st7
  int v12; // esi
  void *v14[3]; // [esp+10h] [ebp-50h] BYREF
  int v15; // [esp+1Ch] [ebp-44h]
  double v16; // [esp+28h] [ebp-38h]
  double v17; // [esp+30h] [ebp-30h]
  double v18; // [esp+38h] [ebp-28h]
  double v19; // [esp+40h] [ebp-20h]
  __int64 v20; // [esp+48h] [ebp-18h]
  int v21; // [esp+50h] [ebp-10h]
  int v22; // [esp+54h] [ebp-Ch]
  int v23; // [esp+58h] [ebp-8h]

  v6 = (int)a1;
  if ( a1[4] != 4 )
    return -2147483645;
  v7 = a1[2];
  if ( !v7 || !a1[3] )
    return -2147483645;
  if ( !a4 )
    return -2147483642;
  sub_409030(v7 + 1);
  v8 = (char *)v14[0];
  if ( v15 )
  {
    v9 = v14[2];
    v22 = a3;
    v21 = v15;
    do
    {
      v10 = 0;
      if ( v9 )
      {
        v19 = (double)v22 * (double)v22;
        v18 = (double)a5;
        v17 = (double)a4;
        v16 = (double)a6;
        v23 = a2;
        do
        {
          v11 = cos((sqrt((double)v23 * (double)v23 + v19) - v18) * 6.283185307179586 / v17);
          --v23;
          ++v10;
          v20 = (__int64)((1.0 - v11) * v16);
          *(_DWORD *)&v8[4 * v10 - 4] = v20;
        }
        while ( v10 < (unsigned int)v9 );
      }
      v8 += (unsigned int)v14[1];
      --v22;
      --v21;
    }
    while ( v21 );
    v6 = (int)a1;
  }
  v12 = sub_403F40((int *)v14, v6, 0x10000);
  operator delete[](v14[0]);
  return v12;
}

// ===== sub_404290 @ 0x00404290..0x004042DB =====
int __cdecl sub_404290(int a1, int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
  int v5; // ecx
  _DWORD v7[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v5, dword_566750) )
    return sub_404130(v7, a1, a2, a3, a4, a5);
  else
    return -2147483647;
}

// ===== sub_4042E0 @ 0x004042E0..0x004044D5 =====
int __usercall sub_4042E0@<eax>(int *a1@<eax>, int a2, int a3, unsigned int a4, unsigned int a5)
{
  unsigned int v5; // edi
  int v6; // ebx
  long double v7; // st7
  double v8; // st6
  unsigned int v9; // edi
  long double v10; // st7
  double v11; // st6
  int v12; // eax
  int v13; // esi
  double v15; // [esp+10h] [ebp-48h]
  double v16; // [esp+18h] [ebp-40h]
  double v17; // [esp+28h] [ebp-30h]
  long double v18; // [esp+30h] [ebp-28h]
  double v19; // [esp+30h] [ebp-28h]
  unsigned int v20; // [esp+40h] [ebp-18h]
  int v21; // [esp+44h] [ebp-14h]
  int i; // [esp+48h] [ebp-10h]
  int v23; // [esp+4Ch] [ebp-Ch]
  unsigned int v24; // [esp+50h] [ebp-8h]
  int v25; // [esp+54h] [ebp-4h]

  if ( a1[4] == 4 )
  {
    v24 = a1[2];
    if ( v24 )
    {
      v5 = a1[3];
      v20 = v5;
      if ( v5 )
      {
        v6 = *a1;
        v18 = (double)a4 * 3.141592653589793 / 11796480.0;
        v17 = sin(v18);
        v7 = cos(v18);
        v15 = v7;
        if ( v7 <= 0.0 )
          v8 = 2147483647.0;
        else
          v8 = (double)v5 / v7;
        v19 = v8;
        v23 = 0;
        v21 = a1[1];
        for ( i = -a3; ; ++i )
        {
          v9 = 0;
          v25 = -a2;
          do
          {
            if ( v7 <= 0.0 )
            {
              *(_DWORD *)(v6 + 4 * v9) = 0;
            }
            else
            {
              v16 = (double)v25;
              v10 = sqrt(v16 * v16 + (double)i * (double)i);
              sub_4D58DA(v16);
              v11 = (double)(int)(16 * (v24 - 1));
              if ( ((v24 - 1) & 0x8000000) != 0 )
                v11 = v11 + 4294967296.0;
              v12 = (int)((1.0 - (180.0 - v16 * 180.0 / 3.141592653589793) / 360.0) * v11) - 16 * v9;
              if ( v12 >= 0x8000 )
              {
                LOWORD(v12) = 0x7FFF;
              }
              else if ( v12 < -32768 )
              {
                LOWORD(v12) = 0x8000;
              }
              *(_WORD *)(v6 + 4 * v9) = v12;
              v13 = (int)((v19 + (double)a5) * v10 * 16.0 / ((double)a5 + v10 * v17)) - 16 * v23;
              if ( v13 >= 0x8000 )
              {
                LOWORD(v13) = 0x7FFF;
              }
              else if ( v13 < -32768 )
              {
                v7 = v15;
                *(_WORD *)(v6 + 4 * v9 + 2) = 0x8000;
                goto LABEL_24;
              }
              v7 = v15;
              *(_WORD *)(v6 + 4 * v9 + 2) = v13;
            }
LABEL_24:
            ++v25;
            ++v9;
          }
          while ( v9 < v24 );
          v6 += v21;
          if ( ++v23 >= v20 )
            return 0;
        }
      }
    }
  }
  return -2147483645;
}

// ===== sub_4044E0 @ 0x004044E0..0x00404526 =====
int __cdecl sub_4044E0(int a1, int a2, unsigned int a3, unsigned int a4)
{
  int v4; // ecx
  int v6[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v4, dword_566750) )
    return sub_4042E0(v6, a1, a2, a3, a4);
  else
    return -2147483647;
}

// ===== sub_404530 @ 0x00404530..0x004046E4 =====
int __usercall sub_404530@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int *a3, int a4, unsigned int a5)
{
  int v6; // ebx
  int v7; // ebx
  int v8; // esi
  int v9; // edi
  long double v10; // st7
  long double v11; // st6
  long double v12; // st7
  long double v13; // st7
  bool v14; // zf
  double v16; // [esp+18h] [ebp-54h]
  double v17; // [esp+20h] [ebp-4Ch]
  double v18; // [esp+28h] [ebp-44h]
  long double v19; // [esp+38h] [ebp-34h]
  int v20; // [esp+3Ch] [ebp-30h]
  int v21; // [esp+44h] [ebp-28h]
  int v22; // [esp+48h] [ebp-24h]
  int v23; // [esp+4Ch] [ebp-20h]
  long double v24; // [esp+50h] [ebp-1Ch]
  long double v25; // [esp+50h] [ebp-1Ch]
  int v26; // [esp+54h] [ebp-18h]
  int v27; // [esp+5Ch] [ebp-10h]
  int v28; // [esp+60h] [ebp-Ch]
  int v29; // [esp+64h] [ebp-8h]
  int v30; // [esp+74h] [ebp+8h]

  if ( a3[4] != 4 )
    return -2147483645;
  v27 = a3[2];
  if ( !v27 )
    return -2147483645;
  v6 = a3[3];
  if ( !v6 )
    return -2147483645;
  if ( !a2 || !a5 )
    return -2147483640;
  v30 = *a3;
  v19 = (double)a2 * 3.141592653589793 / 11796480.0;
  v18 = sin(v19);
  v17 = cos(v19);
  v20 = a3[1];
  v29 = -a1;
  v28 = 16 * a1;
  v21 = v6;
  do
  {
    v7 = v30;
    v16 = (double)v29;
    v8 = -a4;
    v9 = 16 * a4;
    v26 = -a4;
    v23 = 16 * a4;
    v22 = v27;
    do
    {
      v10 = sqrt((double)v26 * (double)v26 + v16 * v16);
      if ( v10 <= 0.0 || (v11 = (double)a5, v11 <= v10) )
      {
        *(_DWORD *)v7 = 0;
      }
      else
      {
        v24 = v10 * v18 / v11;
        v12 = sqrt(1.0 - v24 * v24);
        v25 = (v12 - v17) * v24 / v12;
        v13 = fabs(v16);
        sub_4D58DA(v13);
        *(_WORD *)v7 = (int)(cos(v13) * (double)v23 * v25);
        *(_WORD *)(v7 + 2) = (int)(sin(v13) * (double)v28 * v25);
      }
      ++v8;
      v9 -= 16;
      v7 += 4;
      v14 = v22-- == 1;
      v26 = v8;
      v23 = v9;
    }
    while ( !v14 );
    v30 += v20;
    v28 -= 16;
    ++v29;
    --v21;
  }
  while ( v21 );
  return 0;
}

// ===== sub_4046F0 @ 0x004046F0..0x00404735 =====
int __cdecl sub_4046F0(int a1, int a2, unsigned int a3, unsigned int a4)
{
  int v4; // ecx
  int v6[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v4, dword_566750) )
    return sub_404530(a2, a3, v6, a1, a4);
  else
    return -2147483647;
}

// ===== sub_404740 @ 0x00404740..0x004048B1 =====
int __cdecl sub_404740(_DWORD *a1, int a2, int a3, unsigned int a4, unsigned int a5)
{
  int v5; // ecx
  char *v6; // edi
  unsigned int v7; // esi
  int v8; // ebx
  long double v9; // st7
  long double v10; // st7
  double v11; // st6
  int v12; // esi
  void *v14[2]; // [esp+10h] [ebp-44h] BYREF
  unsigned int v15; // [esp+18h] [ebp-3Ch]
  int v16; // [esp+1Ch] [ebp-38h]
  long double v17; // [esp+28h] [ebp-2Ch]
  double v18; // [esp+30h] [ebp-24h]
  __int64 v19; // [esp+38h] [ebp-1Ch]
  int v20; // [esp+44h] [ebp-10h]
  int v21; // [esp+48h] [ebp-Ch]
  int v22; // [esp+4Ch] [ebp-8h]

  if ( a1[4] != 4 )
    return -2147483645;
  v5 = a1[2];
  if ( !v5 || !a1[3] )
    return -2147483645;
  if ( !a4 || !a5 )
    return -2147483640;
  sub_409030(v5 + 1);
  v6 = (char *)v14[0];
  if ( v16 )
  {
    v21 = -a3;
    v20 = v16;
    do
    {
      v7 = 0;
      if ( v15 )
      {
        v18 = (double)v21 * (double)v21;
        v17 = (double)a5;
        v8 = -a2;
        v22 = -a2;
        do
        {
          v9 = sqrt((double)v22 * (double)v22 + v18);
          if ( v17 <= v9 )
          {
            *(_DWORD *)&v6[4 * v7] = 0;
          }
          else
          {
            v10 = v9 * 3.141592653589793;
            HIDWORD(v19) = 2 * a5;
            v11 = (double)(int)(2 * a5);
            if ( (a5 & 0x40000000) != 0 )
              v11 = v11 + 4294967296.0;
            v19 = (__int64)(4194304.0 - sin(v10 / v11) * 4194304.0);
            *(_DWORD *)&v6[4 * v7] = v19;
          }
          ++v7;
          v22 = ++v8;
        }
        while ( v7 < v15 );
      }
      v6 += (unsigned int)v14[1];
      ++v21;
      --v20;
    }
    while ( v20 );
  }
  v12 = sub_403F40((int *)v14, (int)a1, 0x40000000 / a4);
  operator delete[](v14[0]);
  return v12;
}

// ===== sub_4048C0 @ 0x004048C0..0x00404907 =====
int __cdecl sub_4048C0(int a1, int a2, unsigned int a3, unsigned int a4)
{
  int v4; // ecx
  _DWORD v6[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v4, dword_566750) )
    return sub_404740(v6, a1, a2, a3, a4);
  else
    return -2147483647;
}

// ===== sub_404910 @ 0x00404910..0x00404AAB =====
int __usercall sub_404910@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int *a3, int a4, int a5, int a6, int a7)
{
  int v8; // esi
  _WORD *v9; // eax
  int v10; // edi
  int v11; // esi
  int v12; // esi
  int v13; // ecx
  _WORD *v14; // edx
  int k; // eax
  double v17; // [esp+18h] [ebp-34h]
  double v18; // [esp+30h] [ebp-1Ch]
  _WORD *v19; // [esp+40h] [ebp-Ch]
  _WORD *v20; // [esp+44h] [ebp-8h]
  int i; // [esp+54h] [ebp+8h]
  int j; // [esp+54h] [ebp+8h]
  int v23; // [esp+54h] [ebp+8h]

  if ( a3[4] != 4 )
    return -2147483645;
  if ( !a3[2] )
    return -2147483645;
  v8 = a3[3];
  if ( !v8 )
    return -2147483645;
  if ( !a2 )
  {
    a2 = 1;
    a5 = 0;
  }
  if ( !a1 )
  {
    a1 = 1;
    a7 = 0;
  }
  v17 = (double)a2;
  v18 = (double)a1;
  v20 = operator new[](2 * v8);
  v9 = operator new[](2 * a3[2]);
  v10 = a3[3];
  v11 = 0;
  v19 = v9;
  for ( i = 0; v11 < v10; i = v11 )
    v20[v11++] = (int)(sin(((double)i + (double)a6) * 6.283185307179586 / v18) * (double)a7 * 16.0);
  v12 = a3[2];
  for ( j = 0; j < v12; ++j )
    v9[j] = (int)(sin(((double)j + (double)a4) * 6.283185307179586 / v17) * (double)a5 * 16.0);
  v13 = *a3;
  if ( v10 > 0 )
  {
    v14 = v20;
    v23 = a3[1];
    do
    {
      for ( k = 0; k < v12; ++k )
      {
        *(_WORD *)(v13 + 4 * k) = *v14;
        *(_WORD *)(v13 + 4 * k + 2) = v19[k];
      }
      v13 += v23;
      ++v14;
      --v10;
    }
    while ( v10 );
  }
  operator delete[](v20);
  operator delete[](v19);
  return 0;
}

// ===== sub_404AB0 @ 0x00404AB0..0x00404AFD =====
int __cdecl sub_404AB0(unsigned int a1, int a2, int a3, unsigned int a4, int a5, int a6)
{
  int v6; // ecx
  int v8[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v6, dword_566750) )
    return sub_404910(a4, a1, v8, a2, a3, a5, a6);
  else
    return -2147483647;
}

// ===== sub_404B00 @ 0x00404B00..0x00404C4B =====
int __usercall sub_404B00@<eax>(int *a1@<eax>, int a2, int a3, int a4, int a5, unsigned int a6)
{
  int v6; // ebx
  int v7; // edi
  int v8; // esi
  int v9; // ebx
  int v10; // eax
  double v12; // [esp+18h] [ebp-3Ch]
  int v13; // [esp+28h] [ebp-2Ch]
  int v14; // [esp+2Ch] [ebp-28h]
  int v15; // [esp+30h] [ebp-24h]
  int v16; // [esp+34h] [ebp-20h]
  int v17; // [esp+38h] [ebp-1Ch]
  int v18; // [esp+3Ch] [ebp-18h]
  int v19; // [esp+40h] [ebp-14h]
  int v20; // [esp+44h] [ebp-10h]
  int v21; // [esp+48h] [ebp-Ch]

  if ( a1[4] != 4 )
    return -2147483645;
  v15 = a1[2];
  if ( !v15 )
    return -2147483645;
  v6 = a1[3];
  v14 = v6;
  if ( !v6 )
    return -2147483645;
  v7 = *a1;
  v20 = 0;
  v12 = sqrt((double)(a4 - a2) * (double)(a4 - a2) + (double)(a5 - a3) * (double)(a5 - a3)) + (double)a6;
  if ( v6 > 0 )
  {
    v13 = a1[1];
    v18 = -a5;
    v19 = a5;
    do
    {
      v8 = 0;
      v17 = 0;
      if ( v15 > 0 )
      {
        v9 = -a4;
        v16 = -a4;
        v21 = a4;
        do
        {
          v10 = (int)(sin(sqrt((double)v21 * (double)v21 + (double)v19 * (double)v19) * 1.570796326794897 / v12)
                    * (double)v16
                    + (double)a2
                    - (double)v17);
          --v21;
          *(_WORD *)(v7 + 4 * v8) = 16 * v10;
          *(_WORD *)(v7 + 4 * v8++ + 2) = 16 * (int)((double)v18 + (double)a3 - (double)v20);
          ++v9;
          v17 = v8;
          v16 = v9;
        }
        while ( v8 < v15 );
        v6 = v14;
      }
      v7 += v13;
      --v19;
      ++v18;
      ++v20;
    }
    while ( v20 < v6 );
  }
  return 0;
}

// ===== sub_404C50 @ 0x00404C50..0x00404C9A =====
int __cdecl sub_404C50(int a1, int a2, int a3, int a4, unsigned int a5)
{
  int v5; // ecx
  int v7[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v5, dword_566750) )
    return sub_404B00(v7, a1, a2, a3, a4, a5);
  else
    return -2147483647;
}

// ===== sub_404CA0 @ 0x00404CA0..0x00404E1A =====
int __usercall sub_404CA0@<eax>(int a1@<eax>, int *a2@<ecx>, unsigned int a3, int a4, int a5)
{
  int v5; // edx
  int v6; // esi
  int v7; // edi
  int v8; // ecx
  int v9; // ebx
  int v10; // edi
  int v11; // esi
  long double v12; // st7
  double v13; // st6
  bool v14; // zf
  double v15; // st6
  int v16; // eax
  long double v17; // st7
  double v19; // [esp+10h] [ebp-44h]
  _DWORD v20[2]; // [esp+1Ch] [ebp-38h]
  int v21; // [esp+24h] [ebp-30h]
  int v22; // [esp+28h] [ebp-2Ch]
  int v23; // [esp+2Ch] [ebp-28h]
  int v24; // [esp+30h] [ebp-24h]
  int v25; // [esp+34h] [ebp-20h]
  __int64 v26; // [esp+38h] [ebp-1Ch]
  int v27; // [esp+40h] [ebp-14h]
  int v28; // [esp+44h] [ebp-10h]
  int v29; // [esp+48h] [ebp-Ch]
  int v30; // [esp+4Ch] [ebp-8h]
  unsigned int v31; // [esp+64h] [ebp+10h]

  if ( a2[4] != 6 )
    return -2147483645;
  v5 = a2[2];
  v22 = v5;
  if ( !v5 )
    return -2147483645;
  v6 = a2[3];
  if ( !v6 )
    return -2147483645;
  if ( a3 >= 2 )
    return -2147483639;
  v7 = *a2;
  v20[0] = 1;
  v20[1] = 0;
  if ( a5 )
    v31 = 4 * a5;
  else
    v31 = -1;
  v21 = a2[1];
  v28 = -a1;
  v8 = v7 + 4;
  v29 = -32767 * a1;
  v24 = v7 + 4;
  v23 = v6;
  do
  {
    if ( v5 )
    {
      v9 = -a4;
      v10 = -32767 * a4;
      v19 = (double)v28 * (double)v28;
      v27 = v20[a3];
      HIDWORD(v26) = -a4;
      v30 = -32767 * a4;
      v11 = v8;
      v25 = v5;
      do
      {
        v12 = sqrt((double)SHIDWORD(v26) * (double)SHIDWORD(v26) + v19);
        if ( v27 )
          v13 = (double)v30;
        else
          v13 = (double)v29;
        v14 = v27 == 0;
        *(_WORD *)(v11 - 4) = (int)(v13 / v12);
        if ( v14 )
          v15 = (double)v30;
        else
          v15 = (double)v29;
        v16 = (int)(v15 / v12);
        v17 = v12 * 4.0;
        *(_WORD *)(v11 - 2) = v16;
        v10 += 0x7FFF;
        ++v9;
        v11 += 6;
        v26 = (__int64)v17;
        HIDWORD(v26) = v9;
        v14 = v25-- == 1;
        v30 = v10;
        *(_WORD *)(v11 - 6) = (unsigned int)(__int64)v17 % v31;
      }
      while ( !v14 );
      v5 = v22;
    }
    v8 = v21 + v24;
    v29 += 0x7FFF;
    ++v28;
    v14 = v23-- == 1;
    v24 += v21;
  }
  while ( !v14 );
  return 0;
}

// ===== sub_404E20 @ 0x00404E20..0x00404E65 =====
int __cdecl sub_404E20(unsigned int a1, int a2, int a3, int a4)
{
  int v4; // ecx
  int v6[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v4, dword_566750) )
    return sub_404CA0(a3, v6, a1, a2, a4);
  else
    return -2147483647;
}

// ===== sub_404E70 @ 0x00404E70..0x00404F9C =====
int __usercall sub_404E70@<eax>(int *a1@<esi>, unsigned int a2)
{
  unsigned int v2; // ecx
  unsigned int v3; // ebx
  int v4; // eax
  unsigned int v5; // edx
  int v6; // edi
  int v7; // ebx
  _WORD *v8; // eax
  __int16 v9; // cx
  int v11; // [esp+0h] [ebp-40h]
  int v12; // [esp+4h] [ebp-3Ch]
  unsigned int i; // [esp+8h] [ebp-38h]
  _DWORD v14[4]; // [esp+Ch] [ebp-34h]
  _DWORD v15[4]; // [esp+1Ch] [ebp-24h]
  _DWORD v16[4]; // [esp+2Ch] [ebp-14h]

  if ( a1[4] != 6 )
    return -2147483645;
  v2 = a1[2];
  if ( !v2 || !a1[3] )
    return -2147483645;
  v3 = a2;
  if ( a2 >= 4 )
    return -2147483638;
  v4 = *a1;
  v12 = *a1;
  v14[0] = 1;
  v14[1] = 0;
  v14[2] = 1;
  v14[3] = 0;
  v15[0] = 0;
  v15[1] = 1;
  v15[2] = 0;
  v15[3] = 1;
  v16[0] = 1;
  v16[1] = 0;
  v16[2] = 0;
  v16[3] = 1;
  for ( i = 0; i < a1[3]; ++i )
  {
    v5 = 0;
    if ( v2 )
    {
      v6 = v14[v3] != 0 ? 0x7FFF : 0;
      v7 = v15[v3] != 0 ? 0x7FFF : 0;
      v11 = v16[a2];
      v8 = (_WORD *)(v4 + 4);
      do
      {
        v9 = i;
        *(v8 - 2) = v6;
        *(v8 - 1) = v7;
        if ( !v11 )
          v9 = v5;
        *v8 = 4 * v9;
        v2 = a1[2];
        ++v5;
        v8 += 3;
      }
      while ( v5 < v2 );
      v4 = v12;
      v3 = a2;
    }
    v4 += a1[1];
    v12 = v4;
  }
  return 0;
}

// ===== sub_404FA0 @ 0x00404FA0..0x00404FD8 =====
int __cdecl sub_404FA0(unsigned int a1)
{
  int v1; // ecx
  int v3[6]; // [esp+8h] [ebp-18h] BYREF

  if ( sub_407F20(v1, dword_566750) )
    return sub_404E70(v3, a1);
  else
    return -2147483647;
}

// ===== sub_404FE0 @ 0x00404FE0..0x00405081 =====
int __cdecl sub_404FE0(
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
        int a13,
        int a14,
        int a15,
        int a16,
        int a17)
{
  int v17; // ecx
  _BYTE v19[24]; // [esp+8h] [ebp-30h] BYREF

  if ( !sub_407F20(v17, dword_566750) )
    return -2147483639;
  if ( !sub_407F20(a3, dword_566750) )
    return -2147483638;
  sub_40F970(a1, a2, v19, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
  return 0;
}

// ===== sub_405090 @ 0x00405090..0x0040511A =====
int __usercall sub_405090@<eax>(int a1@<ecx>, int a2@<edi>)
{
  unsigned int v3; // [esp+10h] [ebp-28h]
  unsigned int v4; // [esp+14h] [ebp-24h]
  int v5; // [esp+18h] [ebp-20h]
  _BYTE v6[24]; // [esp+20h] [ebp-18h] BYREF

  if ( !sub_407F20(a1, dword_566750) )
    return -2147483638;
  if ( v3 < 2 || v4 < 2 )
    return -2147483642;
  if ( !sub_4026C0(v5, (v4 + 1) >> 1, a2, (v3 + 1) >> 1) )
    return -2147483639;
  sub_407F20(a2, dword_566750);
  sub_418FA0(v6);
  return 0;
}

// ===== sub_405120 @ 0x00405120..0x00405411 =====
int __cdecl sub_405120(int a1)
{
  int v1; // ecx
  double v2; // st7
  double v3; // st6
  double v4; // st5
  double v5; // st4
  double v6; // st3
  int v7; // esi
  double v8; // rt0
  double v9; // st3
  double v10; // st5
  double v11; // st7
  int v12; // esi
  int v13; // ebx
  int v14; // edi
  int v15; // esi
  double v16; // st7
  int v17; // esi
  int v18; // edi
  int v19; // esi
  float v21; // [esp+4h] [ebp-9Ch]
  int v22; // [esp+Ch] [ebp-94h]
  int v23; // [esp+10h] [ebp-90h]
  float v24; // [esp+14h] [ebp-8Ch]
  float X; // [esp+18h] [ebp-88h]
  float v26; // [esp+48h] [ebp-58h]
  int v27; // [esp+48h] [ebp-58h]
  int v28; // [esp+48h] [ebp-58h]
  float v29; // [esp+48h] [ebp-58h]
  float v30; // [esp+48h] [ebp-58h]
  int v31; // [esp+50h] [ebp-50h] BYREF
  int v32; // [esp+54h] [ebp-4Ch]
  int v33; // [esp+58h] [ebp-48h]
  int v34; // [esp+5Ch] [ebp-44h]
  int v35; // [esp+60h] [ebp-40h]
  int v36; // [esp+64h] [ebp-3Ch]
  int v37; // [esp+68h] [ebp-38h]
  int v38; // [esp+6Ch] [ebp-34h]
  int v39[2]; // [esp+70h] [ebp-30h] BYREF
  unsigned int v40; // [esp+78h] [ebp-28h]
  unsigned int v41; // [esp+7Ch] [ebp-24h]
  int v42; // [esp+88h] [ebp-18h] BYREF
  unsigned int v43; // [esp+90h] [ebp-10h]
  unsigned int v44; // [esp+94h] [ebp-Ch]

  if ( !sub_407F20(v1, dword_566750) )
    return -2147483639;
  if ( !sub_407F20(a1, dword_566750) )
    return -2147483638;
  v2 = (double)v40;
  v3 = (double)v43;
  v4 = v2 / v3;
  v5 = (double)v41;
  v6 = (double)v44;
  if ( v5 / v6 < v2 / v3 )
  {
    v4 = v5 / v6;
    v7 = 1;
  }
  else
  {
    v7 = 0;
  }
  v8 = v6;
  v9 = v4;
  v10 = v8;
  sub_409190(v39);
  if ( v7 )
  {
    v16 = sub_4010B0((v2 - v3 * v9) * 0.5, 0.0009765625);
    v17 = v37;
    v18 = (int)floor(v16);
    v32 = v36;
    v34 = v38;
    v31 = v35;
    v33 = v18 - 1;
    sub_40A620(&v31);
    v33 = v17;
    v32 = v36;
    v34 = v38;
    v19 = v17 - v18 + 1;
    v31 = v19;
    sub_40A620(&v31);
    v33 = v19 - 1;
    v31 = v18;
  }
  else
  {
    v11 = sub_4010B0((v5 - v9 * v10) * 0.5, 0.0009765625);
    v12 = v38;
    v13 = v35;
    v14 = (int)floor(v11);
    v32 = v36;
    v34 = v14 - 1;
    v31 = v35;
    v33 = v37;
    sub_40A620(&v31);
    v34 = v12;
    v15 = v12 - v14 + 1;
    v31 = v13;
    v33 = v37;
    v32 = v15;
    sub_40A620(&v31);
    v34 = v15 - 1;
    v32 = v14;
  }
  sub_4091B0(&v31);
  v26 = v9;
  X = v26;
  v24 = v26;
  *(float *)&v27 = (double)v44 * 0.5;
  v23 = v27;
  *(float *)&v28 = (double)v43 * 0.5;
  v22 = v28;
  v29 = (double)v41 * 0.5;
  v21 = v29;
  v30 = 0.5 * (double)v40;
  sub_410820((int)v39, v30, v21, (int)&v42, v22, v23, v24, X, 1);
  return 0;
}

// ===== sub_405420 @ 0x00405420..0x0040544C =====
int __usercall sub_405420@<eax>(int a1@<eax>, int a2@<ecx>)
{
  if ( a2 )
    return sub_40A070(a1) != 0 ? 0x16 : 0;
  else
    return sub_40A260(0, dword_566750) != 0 ? 0x8000000F : 0;
}

// ===== sub_405450 @ 0x00405450..0x00405552 =====
int __cdecl sub_405450(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // ecx
  int v8; // edi
  int result; // eax
  _BYTE v10[8]; // [esp+10h] [ebp-30h] BYREF
  int v11; // [esp+18h] [ebp-28h]
  int v12; // [esp+1Ch] [ebp-24h]
  int v13; // [esp+20h] [ebp-20h]
  _BYTE v14[24]; // [esp+28h] [ebp-18h] BYREF

  v8 = -1;
  if ( sub_407F20(v7, dword_566750) )
  {
    if ( sub_407DA0(v11, v12, v13) )
    {
      sub_407F20(a1, dword_566750);
      switch ( sub_40A2A0(v14, v10, a2, a3, a5, a6, a7) )
      {
        case 0:
          result = 0;
          break;
        case 2:
          result = -2147483629;
          break;
        case 3:
          result = -2147483628;
          break;
        case 9:
          result = -2147483631;
          break;
        case 0x10:
          result = -2147483633;
          break;
        case 0x17:
          result = -2147483627;
          break;
        default:
          return v8;
      }
    }
    else
    {
      return -2147483639;
    }
  }
  else
  {
    return -2147483638;
  }
  return result;
}

// ===== sub_405590 @ 0x00405590..0x004056E6 =====
int __usercall sub_405590@<eax>(int a1@<eax>, int a2, int a3)
{
  size_t v4; // ecx
  char *v5; // eax
  char *v6; // ebx
  char *v7; // edi
  char *v8; // esi
  bool v9; // zf
  char *v11; // ebx
  char *v12; // esi
  int v13; // [esp+Ch] [ebp-3Ch]
  char *v14; // [esp+10h] [ebp-38h]
  int v15; // [esp+14h] [ebp-34h]
  int v16; // [esp+14h] [ebp-34h]
  int v17; // [esp+18h] [ebp-30h]
  int v18; // [esp+1Ch] [ebp-2Ch]
  int v19; // [esp+20h] [ebp-28h]
  int v20; // [esp+24h] [ebp-24h]
  int v21; // [esp+28h] [ebp-20h]
  size_t Size; // [esp+2Ch] [ebp-1Ch]
  char *v23; // [esp+30h] [ebp-18h]
  int v24; // [esp+34h] [ebp-14h]
  int v25; // [esp+38h] [ebp-10h]
  int v26; // [esp+3Ch] [ebp-Ch]
  int v27; // [esp+44h] [ebp-4h]

  if ( !sub_407F20(a1, dword_566750) )
    return -2147483638;
  if ( a2 == a1 || !sub_407DA0(v19, v20, v21) )
    return -2147483639;
  sub_407F20(a2, dword_566750);
  if ( a3 )
  {
    if ( a3 == 1 )
    {
      if ( v26 )
      {
        v11 = v23;
        v12 = (char *)(v17 + v18 * (v20 - 1));
        v16 = v26;
        do
        {
          memcpy_0(v11, v12, v19 * Size);
          v12 -= v18;
          v11 += v24;
          --v16;
        }
        while ( v16 );
      }
      return 0;
    }
    else
    {
      return -2147483626;
    }
  }
  else
  {
    if ( v26 )
    {
      v4 = Size;
      v5 = v23;
      v6 = (char *)(v17 + v19 * Size);
      v14 = v23;
      v15 = v26;
      do
      {
        v7 = v5;
        v8 = v6;
        if ( v25 )
        {
          v13 = v25;
          do
          {
            v8 -= v4;
            memcpy_0(v7, v8, v4);
            v7 += v27;
            v4 = Size;
            --v13;
          }
          while ( v13 );
          v5 = v14;
        }
        v5 += v24;
        v6 += v18;
        v9 = v15-- == 1;
        v14 = v5;
      }
      while ( !v9 );
    }
    return 0;
  }
}

// ===== sub_4056F0 @ 0x004056F0..0x004058C7 =====
int __cdecl sub_4056F0(void *a1, size_t *a2, int a3, int a4, unsigned int a5)
{
  unsigned int v5; // ebx
  int v6; // edi
  _DWORD *v7; // eax
  _WORD *v8; // esi
  __int16 v9; // dx
  __int16 v10; // ax
  __int16 v11; // dx
  void *v12; // edi
  int v13; // eax
  int v14; // eax
  int v16; // [esp+10h] [ebp-20h]
  size_t Size; // [esp+14h] [ebp-1Ch] BYREF
  int v18; // [esp+20h] [ebp-10h]
  int v19; // [esp+24h] [ebp-Ch]
  int v20; // [esp+28h] [ebp-8h]

  v16 = -1;
  if ( !sub_407F20(a3, dword_566750) )
    return -2147483638;
  if ( (unsigned int)(v19 * v18) > 0x400000 )
    return -2147483642;
  if ( v20 == 1 )
  {
    v5 = 24;
  }
  else
  {
    if ( v20 != 2 )
      return -1;
    v5 = 32;
  }
  v6 = v19 * v18 * (v5 >> 3);
  v7 = operator new[](v6 + 16);
  v8 = v7;
  if ( !v7 )
    return -2147483640;
  v9 = v18;
  *v7 = 0;
  v7[1] = 0;
  v7[2] = 0;
  v7[3] = 0;
  v10 = v19;
  *v8 = v9;
  v11 = v20;
  v8[3] = 0;
  v8[1] = v10;
  v8[4] = v11;
  v8[5] = 0;
  v8[2] = v5;
  sub_4026A0(v6, (int)&Size);
  v12 = operator new[](4 * v6 + 48);
  if ( v12 )
  {
    if ( a4 )
    {
      if ( a4 != 1 )
      {
        v16 = -2147483625;
LABEL_22:
        *a2 = Size;
        operator delete[](v12);
        operator delete[](v8);
        return v16;
      }
      if ( a5 > 0x64 )
      {
        v16 = -2147483624;
        goto LABEL_22;
      }
      v13 = sub_4A0DC0(v12, a5);
      if ( v13 )
      {
        v14 = v13 - 6;
        if ( v14 )
        {
          if ( v14 == 2 )
            v16 = -2147483640;
        }
        else
        {
          v16 = -2;
        }
        goto LABEL_22;
      }
    }
    else if ( sub_469DC0(v12, v8) )
    {
      goto LABEL_22;
    }
    v16 = 0;
    if ( a1 )
      memcpy_0(a1, v12, Size);
    goto LABEL_22;
  }
  operator delete[](v8);
  return -2147483640;
}

// ===== sub_4058D0 @ 0x004058D0..0x00405916 =====
int __usercall sub_4058D0@<eax>(unsigned __int16 *a1@<eax>, void *a2@<edx>)
{
  if ( a1[3] )
  {
    if ( a1[3] == 1 )
    {
      sub_405920(a2, a1);
      return 1;
    }
  }
  else
  {
    memcpy_0(a2, a1, *a1 * a1[1] * (a1[2] >> 3) + 16);
  }
  return 1;
}

// ===== sub_405920 @ 0x00405920..0x00405A13 =====
int __cdecl sub_405920(int a1, unsigned __int16 *a2)
{
  _DWORD *v2; // esi
  int v3; // ecx
  unsigned int v4; // edx
  int v5; // eax
  int v6; // edx
  int v7; // edi
  unsigned __int16 *v8; // ecx
  int v9; // ecx
  int v10; // eax
  int v11; // ebx
  int v12; // edi
  int v13; // esi
  int v14; // ecx
  int v15; // eax
  int v16; // esi
  char v17; // cl
  _DWORD v19[4]; // [esp+Ch] [ebp-34h]
  int v20; // [esp+1Ch] [ebp-24h]
  int v21; // [esp+20h] [ebp-20h]
  int v22; // [esp+24h] [ebp-1Ch]
  int v23; // [esp+28h] [ebp-18h]
  int v24; // [esp+2Ch] [ebp-14h]
  int v25; // [esp+30h] [ebp-10h]
  int v26; // [esp+34h] [ebp-Ch]
  int v27; // [esp+38h] [ebp-8h]
  int v28; // [esp+3Ch] [ebp-4h]

  v2 = a2;
  v3 = *a2;
  v4 = a2[2];
  v21 = a2[1];
  v5 = v3 * v21;
  v6 = v4 >> 3;
  v7 = 0;
  v24 = v3;
  v27 = 0;
  if ( v6 )
  {
    v8 = a2 + 8;
    do
    {
      v19[v7++] = v8;
      v8 = (unsigned __int16 *)((char *)v8 + v5);
    }
    while ( v7 < v6 );
    v27 = v7;
  }
  v9 = a1;
  v25 = a1 + 16;
  v10 = 0;
  v11 = 0;
  v12 = 0;
  v28 = 0;
  v22 = 0;
  if ( v21 > 0 )
  {
    v13 = v24;
    v20 = v24 * v6;
    while ( 1 )
    {
      v14 = v27;
      if ( (v10 & 1) != 0 )
        v14 = -v27;
      v26 = v14;
      if ( v13 > 0 )
      {
        v23 = v13;
        do
        {
          v15 = 0;
          if ( v6 > 0 )
          {
            v16 = v11 + v25;
            do
            {
              v17 = *((_BYTE *)&v28 + v15) + *(_BYTE *)(v19[v15] + v12);
              ++v15;
              *(_BYTE *)(v16 + v15 - 1) = v17;
              *((_BYTE *)&v27 + v15 + 3) = v17;
            }
            while ( v15 < v6 );
            v14 = v26;
          }
          v11 += v14;
          ++v12;
          --v23;
        }
        while ( v23 );
        v10 = v22;
      }
      v11 -= v14;
      v25 += v20;
      v22 = ++v10;
      if ( v10 >= v21 )
        break;
      v13 = v24;
    }
    v9 = a1;
    v2 = a2;
  }
  *(_DWORD *)v9 = *v2;
  *(_DWORD *)(v9 + 4) = v2[1];
  *(_DWORD *)(v9 + 8) = v2[2];
  *(_DWORD *)(v9 + 12) = v2[3];
  *(_WORD *)(v9 + 6) = 0;
  return 1;
}

// ===== sub_405A20 @ 0x00405A20..0x00405ACA =====
int __cdecl sub_405A20(int a1)
{
  int result; // eax

  InitializeCriticalSection(&stru_50A86C);
  dword_50A884 = 0;
  dword_50A8A0 = 0;
  dword_50A8A4 = 0;
  dword_50A8A8 = 0;
  dword_50A8AC = 0;
  dword_50A8B0 = 0;
  dword_50A8B4 = 0;
  dword_565AE8 = 0;
  dword_565AEC = 0;
  if ( operator new(0x40u) )
    result = sub_4461E0(a1);
  else
    result = 0;
  dword_565AF0 = result;
  dword_565AF4 = 0;
  return result;
}

// ===== sub_405AD0 @ 0x00405AD0..0x00405B28 =====
void sub_405AD0()
{
  _DWORD *i; // eax

  for ( i = dword_565AEC; dword_565AEC; i = dword_565AEC )
  {
    dword_565AEC = (void *)i[1];
    operator delete(i);
  }
  if ( dword_565AF0 )
    (**(void (__thiscall ***)(int, int))dword_565AF0)(dword_565AF0, 1);
  dword_565AF0 = 0;
  sub_405DB0();
  DeleteCriticalSection(&stru_50A86C);
}

// ===== sub_405B30 @ 0x00405B30..0x00405B3C =====
void sub_405B30()
{
  EnterCriticalSection(&stru_50A86C);
}

// ===== sub_405B40 @ 0x00405B40..0x00405B4C =====
void sub_405B40()
{
  LeaveCriticalSection(&stru_50A86C);
}

// ===== sub_405B50 @ 0x00405B50..0x00405CC7 =====
int __cdecl sub_405B50(_DWORD *a1, _DWORD *a2, _BYTE *Src, size_t Size, int a5)
{
  const char *v5; // eax
  unsigned int i; // ecx
  _DWORD *v7; // esi
  void *v8; // edi
  _DWORD *v9; // ecx
  const char *v10; // ecx
  _BYTE *v11; // edx
  char v12; // al
  char *v14; // ecx
  _BYTE *v15; // edx
  char v16; // al

  sub_405B30();
  v5 = "BF_Movie_______";
  for ( i = 16; i >= 4; i -= 4 )
  {
    if ( *(_DWORD *)&v5[Src - "BF_Movie_______"] != *(_DWORD *)v5 )
    {
      sub_405B40();
      return -2147483646;
    }
    v5 += 4;
  }
  v7 = operator new(0x34u);
  v8 = operator new[](Size);
  memcpy_0(v8, Src, Size);
  *v7 = ++dword_50A884;
  InitializeCriticalSection((LPCRITICAL_SECTION)(v7 + 1));
  v7[7] = v8;
  v7[8] = v8;
  if ( a5 )
  {
    v9 = operator new(0xCu);
    v7[9] = v9;
    if ( *(_DWORD *)a5 )
    {
      *(_DWORD *)v7[9] = operator new[](strlen(*(const char **)a5) + 1);
      v10 = *(const char **)a5;
      v11 = *(_BYTE **)v7[9];
      do
      {
        v12 = *v10;
        *v11++ = *v10++;
      }
      while ( v12 );
    }
    else
    {
      *v9 = 0;
    }
    *(_DWORD *)(v7[9] + 4) = operator new[](strlen(*(const char **)(a5 + 4)) + 1);
    v14 = *(char **)(a5 + 4);
    v15 = *(_BYTE **)(v7[9] + 4);
    do
    {
      v16 = *v14;
      *v15++ = *v14++;
    }
    while ( v16 );
    *(_DWORD *)(v7[9] + 8) = *(_DWORD *)(a5 + 8);
  }
  else
  {
    v7[9] = 0;
  }
  v7[10] = 0;
  v7[11] = 0;
  v7[12] = dword_50A8B4;
  dword_50A8B4 = v7;
  *a1 = *v7;
  *a2 = *(_DWORD *)(v7[8] + 20);
  a2[1] = *(_DWORD *)(v7[8] + 24);
  a2[2] = *(_DWORD *)(v7[8] + 32);
  a2[3] = *(_DWORD *)(v7[8] + 36);
  a2[4] = *(_DWORD *)(v7[8] + 40);
  sub_405B40();
  return 0;
}

// ===== sub_405CD0 @ 0x00405CD0..0x00405DA7 =====
int __cdecl sub_405CD0(int a1)
{
  char *v1; // esi
  int v2; // edi
  int *v3; // ebx
  void **v5; // eax

  sub_405B30();
  v1 = (char *)dword_50A8B4;
  v2 = -2147483645;
  v3 = &dword_50A884;
  if ( dword_50A8B4 )
  {
    while ( a1 != *(_DWORD *)v1 )
    {
      v3 = (int *)v1;
      v1 = (char *)*((_DWORD *)v1 + 12);
      if ( !v1 )
      {
        sub_405B40();
        return -2147483645;
      }
    }
    EnterCriticalSection((LPCRITICAL_SECTION)(v1 + 4));
    LeaveCriticalSection((LPCRITICAL_SECTION)(v1 + 4));
    DeleteCriticalSection((LPCRITICAL_SECTION)(v1 + 4));
    v3[12] = *((_DWORD *)v1 + 12);
    if ( *((_DWORD *)v1 + 10) )
      *(_DWORD *)(sub_405DE0() + 44) = *((_DWORD *)v1 + 11);
    if ( *((_DWORD *)v1 + 11) )
      *(_DWORD *)(sub_405DE0() + 40) = *((_DWORD *)v1 + 10);
    if ( !*((_DWORD *)v1 + 10) && !*((_DWORD *)v1 + 11) )
    {
      operator delete[](*((void **)v1 + 7));
      v5 = (void **)*((_DWORD *)v1 + 9);
      if ( v5 )
      {
        operator delete[](*v5);
        operator delete[](*(void **)(*((_DWORD *)v1 + 9) + 4));
        operator delete(*((void **)v1 + 9));
      }
    }
    operator delete(v1);
    v2 = 0;
  }
  sub_405B40();
  return v2;
}

// ===== sub_405DB0 @ 0x00405DB0..0x00405DD5 =====
int *sub_405DB0()
{
  int *result; // eax

  for ( result = (int *)dword_50A8B4; dword_50A8B4; result = (int *)dword_50A8B4 )
    sub_405CD0(*result);
  return result;
}

// ===== sub_405DE0 @ 0x00405DE0..0x00405DFC =====
void **__thiscall sub_405DE0(void *this)
{
  void **result; // eax

  result = (void **)dword_50A8B4;
  if ( dword_50A8B4 )
  {
    do
    {
      if ( this == *result )
        break;
      result = (void **)result[12];
    }
    while ( result );
  }
  return result;
}

// ===== sub_405E00 @ 0x00405E00..0x00405F02 =====
int __cdecl sub_405E00(int a1, void *a2, unsigned int a3)
{
  void **v3; // eax
  void **v4; // edi
  _DWORD *v5; // edi
  void *v6; // eax
  int v7; // eax
  int v9; // esi
  int v10; // [esp+18h] [ebp-20h]
  int v11; // [esp+1Ch] [ebp-1Ch]
  int v12; // [esp+20h] [ebp-18h]

  sub_405B30();
  v3 = sub_405DE0(a2);
  v4 = v3;
  if ( !v3 )
  {
    v9 = -2147483645;
    goto LABEL_14;
  }
  if ( a3 >= *((_DWORD *)v3[8] + 10) )
  {
    v9 = -2147483644;
    goto LABEL_14;
  }
  if ( !sub_407F20(a1, dword_566750) || (v5 = v4[8], v10 != v5[5]) || v11 != v5[6] || v12 != v5[8] )
  {
    v9 = -2147483643;
LABEL_14:
    sub_405B40();
    return v9;
  }
  sub_405B40();
  v6 = operator new(0x34u);
  if ( v6 )
    v7 = sub_4506E0(v6, a1, a2);
  else
    v7 = 0;
  sub_4451C0(v7);
  return 0;
}

// ===== sub_405F10 @ 0x00405F10..0x00406119 =====
int __usercall sub_405F10@<eax>(void *a1@<ecx>, int a2@<ebp>, int a3, unsigned int a4)
{
  void **v5; // eax
  void **v6; // edi
  _DWORD *v7; // esi
  void *v8; // esi
  unsigned int v10; // [esp-30h] [ebp-310h]
  _DWORD v11[6]; // [esp-1Ch] [ebp-2FCh] BYREF
  int v12; // [esp-4h] [ebp-2E4h] BYREF
  void **v13; // [esp+0h] [ebp-2E0h]
  _BYTE v14[700]; // [esp+4h] [ebp-2DCh] BYREF
  unsigned int v15; // [esp+2C0h] [ebp-20h]
  int *v16; // [esp+2C4h] [ebp-1Ch]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+2C8h] [ebp-18h]
  void *v18; // [esp+2CCh] [ebp-14h]
  int v19; // [esp+2D0h] [ebp-10h]
  _DWORD v20[2]; // [esp+2D4h] [ebp-Ch] BYREF
  int v21; // [esp+2DCh] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+2E0h] [ebp+0h]

  v20[0] = a2;
  v20[1] = retaddr;
  v19 = -1;
  v18 = &loc_4D910B;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v16 = &v21;
  v15 = (unsigned int)v20 ^ dword_4FB734;
  v10 = (unsigned int)v20 ^ dword_4FB734;
  sub_405B30();
  v5 = sub_405DE0(a1);
  v6 = v5;
  v13 = v5;
  if ( v5 )
  {
    if ( a4 < *((_DWORD *)v5[8] + 10) )
    {
      if ( sub_407F20(a3, dword_566750) )
      {
        v7 = v6[8];
        if ( v11[2] == v7[5] && v11[3] == v7[6] && v11[4] == v7[8] )
        {
          if ( sub_49B210(v10) )
          {
            if ( !v7[4] )
              sub_49A350(v6[7]);
          }
          else
          {
            sub_49B040(dword_5666F8);
            v19 = 0;
            if ( v6[9] )
            {
              if ( sub_49B640(&v12, (char *)v6[7] + 64) )
              {
                v8 = operator new[]((unsigned int)v13);
                if ( !sub_467F50(v8, *((_DWORD *)v6[9] + 1), v12, v13) )
                  sub_49CC10(v14, v11, (char *)v6[7] + 64, v8);
                operator delete[](v8);
                v19 = -1;
              }
              else
              {
                v19 = -1;
              }
            }
            else
            {
              sub_49CBD0(v14, v11);
              v19 = -1;
            }
          }
        }
      }
    }
  }
  sub_405B40();
  return sub_4AB245((unsigned int)v20 ^ v15);
}

// ===== sub_406120 @ 0x00406120..0x004061CB =====
int __cdecl sub_406120(_DWORD *a1, void *a2)
{
  void **v2; // eax
  void **v3; // edi
  char *v5; // esi
  void *v6; // edx

  sub_405B30();
  v2 = sub_405DE0(a2);
  v3 = v2;
  if ( v2 )
  {
    if ( v2[11] )
    {
      sub_405B40();
      return -2147483641;
    }
    else
    {
      v5 = (char *)operator new(0x34u);
      *(_DWORD *)v5 = ++dword_50A884;
      InitializeCriticalSection((LPCRITICAL_SECTION)(v5 + 4));
      *((_DWORD *)v5 + 7) = v3[7];
      *((_DWORD *)v5 + 8) = v3[8];
      v6 = *(void **)v5;
      *((_DWORD *)v5 + 9) = v3[9];
      *((_DWORD *)v5 + 10) = a2;
      *((_DWORD *)v5 + 11) = 0;
      *((_DWORD *)v5 + 12) = dword_50A8B4;
      v3[11] = v6;
      dword_50A8B4 = v5;
      *a1 = *(_DWORD *)v5;
      sub_405B40();
      return 0;
    }
  }
  else
  {
    sub_405B40();
    return -2147483645;
  }
}

// ===== sub_4061D0 @ 0x004061D0..0x0040620A =====
int __cdecl sub_4061D0(void *a1)
{
  void **v1; // eax

  sub_405B30();
  v1 = sub_405DE0(a1);
  if ( v1 )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(v1 + 1));
    sub_405B40();
    return 0;
  }
  else
  {
    sub_405B40();
    return -2147483645;
  }
}

// ===== sub_406210 @ 0x00406210..0x0040622C =====
int __thiscall sub_406210(void *this)
{
  void **v1; // eax

  v1 = sub_405DE0(this);
  if ( !v1 )
    return -2147483645;
  LeaveCriticalSection((LPCRITICAL_SECTION)(v1 + 1));
  return 0;
}

// ===== sub_406230 @ 0x00406230..0x004062A4 =====
int __usercall sub_406230@<eax>(int a1@<edi>)
{
  int *v1; // eax
  int *v3; // esi
  int *v4; // eax
  void *v5; // [esp-4h] [ebp-8h]

  if ( dword_565AF0 )
  {
    v1 = (int *)dword_565AEC;
    if ( !dword_565AE8 )
    {
      if ( !dword_565AEC )
      {
LABEL_6:
        dword_565AE8 = a1;
        return 1;
      }
      if ( a1 == *(_DWORD *)dword_565AEC )
      {
        v5 = dword_565AEC;
        dword_565AEC = (void *)*((_DWORD *)dword_565AEC + 1);
        operator delete(v5);
        goto LABEL_6;
      }
    }
    v3 = &dword_565AE8;
    if ( dword_565AEC )
    {
      while ( a1 != *v1 )
      {
        v3 = v1;
        v1 = (int *)v1[1];
        if ( !v1 )
          goto LABEL_10;
      }
    }
    else
    {
LABEL_10:
      v4 = (int *)operator new(8u);
      *v4 = a1;
      v4[1] = 0;
      v3[1] = (int)v4;
    }
  }
  return 0;
}

// ===== sub_4062B0 @ 0x004062B0..0x004062C4 =====
void sub_4062B0()
{
  sub_405B30();
  dword_565AE8 = 0;
  sub_405B40();
}

// ===== sub_4062D0 @ 0x004062D0..0x004064FB =====
void __cdecl __noreturn sub_4062D0(void **a1)
{
  void **v1; // esi
  int v2; // edx
  void **v3; // edi
  _DWORD *v4; // esi
  void *v5; // esi
  unsigned int v6; // [esp-310h] [ebp-31Ch] BYREF
  _BYTE v7[24]; // [esp-304h] [ebp-310h] BYREF
  int v8; // [esp-2ECh] [ebp-2F8h] BYREF
  void **v9; // [esp-2E8h] [ebp-2F4h]
  void **v10; // [esp-2E4h] [ebp-2F0h]
  _DWORD v11[183]; // [esp-2E0h] [ebp-2ECh] BYREF
  int v12; // [esp-4h] [ebp-10h]
  _DWORD v13[2]; // [esp+0h] [ebp-Ch] BYREF
  int v14; // [esp+8h] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v13[1] = retaddr;
  v12 = -1;
  v11[182] = &loc_4D987B;
  v11[181] = NtCurrentTeb()->NtTib.ExceptionList;
  v11[179] = &v14;
  v11[175] = (unsigned int)v13 ^ dword_4FB734;
  v6 = (unsigned int)v13 ^ dword_4FB734;
  v11[180] = &v6;
  v1 = a1;
  v10 = a1;
  if ( !sub_407C40(dword_566750) )
  {
LABEL_18:
    v1[2] = (void *)1;
    _endthread();
  }
  if ( sub_4061D0(a1[4]) )
  {
LABEL_17:
    sub_407C90(v1[3], dword_566750);
    goto LABEL_18;
  }
  if ( !sub_407F20(a1[3], dword_566750) || (v3 = sub_405DE0(v10[4]), (v9 = v3) == 0) )
  {
LABEL_16:
    sub_406210(v10[4]);
    v1 = v10;
    goto LABEL_17;
  }
  SetEvent(*(HANDLE *)(v2 + 4));
  v12 = 0;
  v4 = v3[8];
  if ( sub_49B210(v6) )
  {
    if ( !v4[4] )
      sub_49A350(v3[7]);
    goto LABEL_15;
  }
  sub_49B040(dword_565AF0);
  LOBYTE(v12) = 1;
  if ( v3[9] )
  {
    if ( sub_49B640(&v8, (char *)v3[7] + 64) )
    {
      v5 = operator new[]((unsigned int)v9);
      if ( !sub_467F50(v5, *((_DWORD *)v3[9] + 1), v8, v9) )
        sub_49CC10(v11, v7, (char *)v3[7] + 64, v5);
      operator delete[](v5);
      sub_4062B0();
      LOBYTE(v12) = 0;
      goto LABEL_15;
    }
  }
  else
  {
    sub_49CBD0(v11, v7);
  }
  sub_4062B0();
  LOBYTE(v12) = 0;
LABEL_15:
  v10[6] = (void *)1;
  v12 = -1;
  goto LABEL_16;
}

// ===== sub_406520 @ 0x00406520..0x00406640 =====
int __usercall sub_406520@<eax>(void *a1@<eax>, _DWORD *a2, int a3, int a4)
{
  int v5; // esi
  _DWORD *v6; // esi
  void *v7; // eax
  int v9; // [esp+24h] [ebp-8h]

  if ( !sub_407F20(a3, dword_566750) )
    return -2147483643;
  sub_405B30();
  if ( sub_405DE0(a1) )
  {
    v9 = 1;
    if ( sub_49B210() || sub_406230((int)a1) )
    {
      v5 = 1;
    }
    else
    {
      v9 = 0;
      v5 = -2147483639;
    }
    sub_405B40();
    if ( v9 )
    {
      v6 = operator new(0x1Cu);
      v6[1] = CreateEventA(0, 0, 0, 0);
      v6[2] = 0;
      v6[3] = a3;
      v6[4] = a1;
      v6[5] = a4;
      v6[6] = 0;
      v7 = (void *)_beginthread((_beginthread_proc_type)sub_4062D0, 0, v6);
      if ( v7 == (void *)-1 )
      {
        operator delete(v6);
        return -2147483640;
      }
      else
      {
        *v6 = v7;
        SetThreadPriority(v7, 2);
        WaitForSingleObject((HANDLE)v6[1], 0xFFFFFFFF);
        CloseHandle((HANDLE)v6[1]);
        v6[1] = 0;
        *a2 = v6;
        return 0;
      }
    }
    else
    {
      return v5;
    }
  }
  else
  {
    sub_405B40();
    return -2147483645;
  }
}

// ===== sub_406640 @ 0x00406640..0x004066B9 =====
int __usercall sub_406640@<eax>(int a1@<ecx>, int a2@<edi>, _DWORD *a3, int a4, int a5)
{
  const char *v5; // eax
  unsigned int i; // edx
  int v7; // edx
  unsigned int v8; // eax

  v5 = "bwef    ";
  for ( i = 8; i >= 4; i -= 4 )
  {
    if ( *(_DWORD *)&v5[a1 - (_DWORD)"bwef    "] != *(_DWORD *)v5 )
      return -2147483646;
    v5 += 4;
  }
  v7 = *(_DWORD *)(a1 + 20);
  if ( a4 != 4 * v7 + 288 )
    return -2147483645;
  v8 = 0;
  if ( v7 )
  {
    do
    {
      *(_DWORD *)(a2 + 8 * v8) = a5 + *(_DWORD *)(a1 + 4 * v8 + 288);
      *(_DWORD *)(a2 + 8 * v8++ + 4) = *(_DWORD *)(a1 + 24);
    }
    while ( v8 < *(_DWORD *)(a1 + 20) );
  }
  *a3 = *(_DWORD *)(a1 + 20);
  return 0;
}

// ===== sub_4066C0 @ 0x004066C0..0x0040671C =====
int __usercall sub_4066C0@<eax>(int a1@<eax>, int a2, _DWORD *a3, int a4, int a5)
{
  void *v6; // esi
  int v7; // eax
  int v8; // edi

  if ( !sub_4662E0(a1) )
    return -2147483647;
  v6 = operator new[](0x4000000u);
  v7 = sub_465AB0(a1, a4);
  v8 = sub_406640((int)v6, a2, a3, v7, a5);
  operator delete[](v6);
  return v8;
}

// ===== sub_406720 @ 0x00406720..0x00406764 =====
int __usercall sub_406720@<eax>(int a1@<esi>)
{
  *(_DWORD *)a1 = &CArchive::`vftable';
  *(_DWORD *)(a1 + 4) = 0;
  memset((void *)(a1 + 8), 0, 0x30Cu);
  *(_DWORD *)(a1 + 800) = 0;
  *(_DWORD *)(a1 + 796) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(a1 + 804));
  return a1;
}

// ===== sub_406770 @ 0x00406770..0x00406791 =====
void *__thiscall sub_406770(void *this, char a2)
{
  sub_4067A0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4067A0 @ 0x004067A0..0x004067BF =====
void __thiscall sub_4067A0(char *this)
{
  *(_DWORD *)this = &CArchive::`vftable';
  sub_406B50(1);
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 804));
}

// ===== sub_4067C0 @ 0x004067C0..0x0040680D =====
int __thiscall sub_4067C0(void *this, int a2)
{
  int v3; // eax

  if ( (*(int (__thiscall **)(void *, int, _DWORD))(*(_DWORD *)this + 32))(this, a2, 0) )
  {
    sub_406B50(0);
    return 0;
  }
  else
  {
    v3 = sub_406CA0();
    if ( v3 )
      return (*(int (__thiscall **)(int, int))(*(_DWORD *)v3 + 4))(v3, a2);
    else
      return -2147483632;
  }
}

// ===== sub_406810 @ 0x00406810..0x004068A2 =====
int __thiscall sub_406810(void *this, _DWORD *a2, _DWORD *a3, int a4)
{
  void *v5; // esi

  if ( (*(int (__thiscall **)(void *, int, _DWORD))(*(_DWORD *)this + 32))(this, a4, 0) )
  {
    v5 = operator new(*((_DWORD *)this + 197) << 7);
    memcpy_0(v5, *((const void **)this + 199), *((_DWORD *)this + 197) << 7);
    *a2 = v5;
    *a3 = *((_DWORD *)this + 197);
    return 0;
  }
  else if ( sub_406CA0() )
  {
    return sub_406810(a2, a3, a4);
  }
  else
  {
    return -2147483632;
  }
}

// ===== sub_4068B0 @ 0x004068B0..0x0040690C =====
int __thiscall sub_4068B0(void *this, _DWORD *a2, int a3)
{
  int v3; // esi

  v3 = a3;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a3, &a3) )
  {
    *a2 = (*(int (__thiscall **)(void *, int))(*(_DWORD *)this + 8))(this, a3);
    return 1;
  }
  else if ( sub_406CA0() )
  {
    return sub_4068B0(a2, v3);
  }
  else
  {
    return 0;
  }
}

// ===== sub_406910 @ 0x00406910..0x0040696E =====
int __thiscall sub_406910(void *this, int a2, int a3, int a4)
{
  int v4; // esi

  v4 = a3;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a3, &a3) )
    return (*(int (__thiscall **)(void *, int, int, int))(*(_DWORD *)this + 12))(this, a2, a4, a3);
  if ( sub_406CA0() )
    return sub_406910(a2, v4, a4);
  return 0;
}

// ===== sub_406970 @ 0x00406970..0x004069C6 =====
int __thiscall sub_406970(void *this, int a2, int a3)
{
  int v3; // esi

  v3 = a2;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a2, &a2) )
    return (*(int (__thiscall **)(void *, int, int))(*(_DWORD *)this + 16))(this, a3, a2);
  if ( sub_406CA0() )
    return sub_406970(v3, a3);
  return 0;
}

// ===== sub_4069D0 @ 0x004069D0..0x00406A46 =====
int __thiscall sub_4069D0(void *this, int a2, int a3, int a4)
{
  int v5; // ebx

  if ( !a2 )
    return sub_406970(this, a3, a4);
  v5 = a3;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a3, &a3) )
    return (*(int (__thiscall **)(void *, int, int, int))(*(_DWORD *)this + 20))(this, a2, a4, a3);
  if ( sub_406CA0() )
    return sub_4069D0(a2, v5, a4);
  return 0;
}

// ===== sub_406A50 @ 0x00406A50..0x00406A6C =====
int __userpurge sub_406A50@<eax>(int a1@<eax>, int a2@<edx>, int a3)
{
  return sub_406A70(a3, a2, a1, 0, 0);
}

// ===== sub_406A70 @ 0x00406A70..0x00406AE1 =====
int __thiscall sub_406A70(void *this, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // esi

  v6 = a3;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a3, &a3) )
    return (*(int (__thiscall **)(void *, int, int, int, int, int))(*(_DWORD *)this + 24))(this, a2, a4, a5, a6, a3);
  if ( sub_406CA0() )
    return sub_406A70(a2, v6, a4, a5, a6);
  return -2147483632;
}

// ===== sub_406AF0 @ 0x00406AF0..0x00406B49 =====
int __thiscall sub_406AF0(void *this, int a2, int a3)
{
  int v3; // esi

  v3 = a2;
  if ( (*(int (__thiscall **)(void *, int, int *))(*(_DWORD *)this + 32))(this, a2, &a2) )
    return (*(int (__thiscall **)(void *, int, int))(*(_DWORD *)this + 28))(this, a3, a2);
  if ( sub_406CA0() )
    return sub_406AF0(v3, a3);
  return -2147483632;
}

// ===== sub_406B50 @ 0x00406B50..0x00406BBC =====
int __thiscall sub_406B50(int this, int a2)
{
  void (__thiscall ***v3)(_DWORD, int); // ecx

  sub_406D30();
  *(_DWORD *)(this + 4) = 0;
  if ( a2 && *(_DWORD *)(this + 800) )
  {
    sub_406B50(1);
    v3 = *(void (__thiscall ****)(_DWORD, int))(this + 800);
    if ( v3 )
      (**v3)(v3, 1);
    *(_DWORD *)(this + 800) = 0;
  }
  operator delete(*(void **)(this + 796));
  *(_DWORD *)(this + 796) = 0;
  return sub_406D40();
}

// ===== sub_406BC0 @ 0x00406BC0..0x00406C64 =====
int __thiscall sub_406BC0(_DWORD *this, int a2)
{
  int v3; // ebx
  _BYTE *v4; // ecx
  _BYTE *v5; // eax
  bool v6; // cf
  unsigned __int8 v7; // dl
  int v8; // eax
  _BYTE v10[780]; // [esp+Ch] [ebp-310h] BYREF

  v3 = 0;
  sub_406C70(v10);
  sub_406D30();
  v4 = this + 2;
  v5 = v10;
  while ( 1 )
  {
    v6 = *v5 < *v4;
    if ( *v5 != *v4 )
      break;
    if ( !*v5 )
      goto LABEL_6;
    v7 = v5[1];
    v6 = v7 < v4[1];
    if ( v7 != v4[1] )
      break;
    v5 += 2;
    v4 += 2;
    if ( !v7 )
    {
LABEL_6:
      v8 = 0;
      goto LABEL_8;
    }
  }
  v8 = -v6 - (v6 - 1);
LABEL_8:
  if ( v8 )
  {
    v4 = (_BYTE *)this[200];
    if ( v4 )
    {
      v3 = sub_406BC0(a2);
    }
    else
    {
      this[200] = a2;
      v3 = 1;
    }
  }
  sub_406D40(v4);
  return v3;
}

// ===== sub_406C70 @ 0x00406C70..0x00406C97 =====
int __userpurge sub_406C70@<eax>(int a1@<esi>, int a2)
{
  int v2; // ecx
  _BYTE *v3; // eax

  sub_406D30();
  v3 = (_BYTE *)(a1 + 8);
  do
  {
    LOBYTE(v2) = *v3;
    v3[a2 - (a1 + 8)] = *v3;
    ++v3;
  }
  while ( (_BYTE)v2 );
  return sub_406D40(v2);
}

// ===== sub_406CA0 @ 0x00406CA0..0x00406D26 =====
int __usercall sub_406CA0@<eax>(int a1@<edi>)
{
  int v1; // esi
  int v2; // ecx
  int v3; // eax
  void *v4; // eax

  v1 = 0;
  sub_406D30();
  v3 = *(_DWORD *)(a1 + 800);
  if ( v3 )
  {
LABEL_7:
    v1 = v3;
    goto LABEL_8;
  }
  if ( *(_DWORD *)(a1 + 4) )
  {
    v4 = operator new(0x33Cu);
    if ( v4 )
      v3 = sub_406720((int)v4);
    else
      v3 = 0;
    *(_DWORD *)(a1 + 800) = v3;
    goto LABEL_7;
  }
LABEL_8:
  sub_406D40(v2);
  return v1;
}

// ===== sub_406D30 @ 0x00406D30..0x00406D3D =====
void __usercall sub_406D30(int a1@<eax>)
{
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 804));
}

// ===== sub_406D40 @ 0x00406D40..0x00406D4D =====
void __usercall sub_406D40(int a1@<eax>)
{
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 804));
}

// ===== sub_406D50 @ 0x00406D50..0x00406E6B =====
BOOL __thiscall sub_406D50(void *this, const char *a2, int a3)
{
  int v3; // ebx
  BOOL v4; // esi
  int v5; // ecx
  const char *v6; // eax
  char v9[780]; // [esp+14h] [ebp-310h] BYREF

  v3 = (int)this;
  v4 = 0;
  sub_406D30((int)this);
  strcpy(v9, a2);
  sub_42EA80(v5, v9);
  if ( *(_DWORD *)(v3 + 4) )
  {
    v4 = strcmp(v9, (const char *)(v3 + 8)) == 0;
  }
  else
  {
    v6 = (const char *)(v3 + 8);
    if ( strcmp(v9, (const char *)(v3 + 8)) && strlen(v6) )
    {
      v3 = (int)this;
    }
    else
    {
      v4 = sub_406E70(a2) == 0;
      v3 = (int)this;
    }
  }
  sub_406D40(v3);
  return v4;
}

// ===== sub_406E70 @ 0x00406E70..0x00407123 =====
int __userpurge sub_406E70@<eax>(int a1@<edi>, const char *a2)
{
  int result; // eax
  unsigned int v3; // eax
  int v4; // ecx
  int v5; // eax
  DWORD v6; // esi
  _DWORD *v7; // ebx
  int v8; // esi
  _DWORD *v9; // ebx
  int v10; // ecx
  _BYTE *v11; // edx
  _BYTE *v12; // eax
  int v13; // eax
  unsigned int v14; // eax
  int v15; // ecx
  int v16; // eax
  DWORD v17; // esi
  void *v18; // eax
  int v19; // ecx
  unsigned int v20; // esi
  int v21; // ebx
  _DWORD *v22; // [esp+Ch] [ebp-38h]
  DWORD NumberOfBytesRead[3]; // [esp+10h] [ebp-34h] BYREF
  int v24; // [esp+1Ch] [ebp-28h]
  int v25; // [esp+20h] [ebp-24h]
  _DWORD Buffer[3]; // [esp+24h] [ebp-20h] BYREF
  int v27; // [esp+30h] [ebp-14h]
  int v28; // [esp+40h] [ebp-4h]

  result = 0;
  v25 = 0;
  if ( !*(_DWORD *)(a1 + 4) )
  {
    sub_42D3B0();
    v28 = 0;
    if ( sub_42D520() )
    {
      if ( sub_42D5D0(0x10u, Buffer, (DWORD)NumberOfBytesRead) == 16 )
      {
        v3 = 12;
        v4 = 0;
        while ( Buffer[v4] == dword_4E4168[v4] )
        {
          v3 -= 4;
          ++v4;
          if ( v3 < 4 )
          {
            strcpy((char *)(a1 + 8), a2);
            sub_42EA80(v4 * 4, a1 + 8);
            v5 = v27;
            v6 = 32 * v27;
            *(_DWORD *)(a1 + 792) = 32 * v27 + 16;
            *(_DWORD *)(a1 + 788) = v5;
            *(_DWORD *)(a1 + 796) = operator new(v5 << 7);
            v7 = operator new(32 * *(_DWORD *)(a1 + 788));
            v22 = v7;
            sub_42D5D0(v6, v7, (DWORD)NumberOfBytesRead);
            v8 = 0;
            v24 = 0;
            if ( *(_DWORD *)(a1 + 788) )
            {
              v9 = v7 + 5;
              do
              {
                memset((void *)(v8 + *(_DWORD *)(a1 + 796)), 0, 0x80u);
                v11 = v9 - 5;
                v12 = (_BYTE *)(v8 + *(_DWORD *)(a1 + 796));
                do
                {
                  LOBYTE(v10) = *v11;
                  *v12++ = *v11++;
                }
                while ( (_BYTE)v10 );
                sub_42EA80(v10, v8 + *(_DWORD *)(a1 + 796));
                *(_DWORD *)(v8 + *(_DWORD *)(a1 + 796) + 96) = *(v9 - 1);
                v13 = v24;
                *(_DWORD *)(v8 + *(_DWORD *)(a1 + 796) + 100) = *v9;
                v9 += 8;
                v8 += 128;
                v24 = v13 + 1;
              }
              while ( (unsigned int)(v13 + 1) < *(_DWORD *)(a1 + 788) );
              v7 = v22;
            }
            operator delete(v7);
            *(_DWORD *)(a1 + 4) = 1;
            sub_42D5B0();
            goto LABEL_23;
          }
        }
        v14 = 12;
        v15 = 0;
        while ( Buffer[v15] == dword_4E4178[v15] )
        {
          v14 -= 4;
          ++v15;
          if ( v14 < 4 )
          {
            strcpy((char *)(a1 + 8), a2);
            sub_42EA80(v15 * 4, a1 + 8);
            v16 = v27;
            v17 = v27 << 7;
            *(_DWORD *)(a1 + 792) = (v27 << 7) + 16;
            *(_DWORD *)(a1 + 788) = v16;
            v18 = operator new(v16 << 7);
            *(_DWORD *)(a1 + 796) = v18;
            sub_42D5D0(v17, v18, (DWORD)NumberOfBytesRead);
            v20 = 0;
            if ( *(_DWORD *)(a1 + 788) )
            {
              v21 = 0;
              do
              {
                sub_42EA80(v19, v21 + *(_DWORD *)(a1 + 796));
                ++v20;
                v21 += 128;
              }
              while ( v20 < *(_DWORD *)(a1 + 788) );
            }
            *(_DWORD *)(a1 + 4) = 1;
            sub_42D5B0();
            goto LABEL_23;
          }
        }
      }
      v25 = -2147483646;
      sub_42D5B0();
    }
    else
    {
      v25 = -2147483647;
    }
LABEL_23:
    v28 = -1;
    sub_42D400(NumberOfBytesRead);
    return v25;
  }
  return result;
}

// ===== sub_407130 @ 0x00407130..0x00407139 =====
int __thiscall sub_407130(_DWORD *this, int a2)
{
  return this[198];
}

// ===== sub_407140 @ 0x00407140..0x00407158 =====
int __thiscall sub_407140(void *this, int a2, int a3)
{
  return (*(int (__thiscall **)(void *, _DWORD, int, int))(*(_DWORD *)this + 12))(this, 0, a2, a3);
}

// ===== sub_407160 @ 0x00407160..0x0040724A =====
BOOL __thiscall sub_407160(_DWORD *this, void *a2, const char *a3, int a4)
{
  _DWORD *v4; // edi
  int v5; // ebx
  BOOL result; // eax
  unsigned int i; // [esp+10h] [ebp-314h]
  char v9[780]; // [esp+14h] [ebp-310h] BYREF

  v4 = this;
  strcpy(v9, a3);
  sub_42EA80(this, v9);
  v5 = 0;
  result = 0;
  for ( i = 0; i < v4[197]; ++i )
  {
    if ( result )
      break;
    result = strcmp((const char *)(v5 + v4[199]), v9) == 0;
    if ( result )
    {
      if ( a2 )
      {
        qmemcpy(a2, (const void *)(v5 + v4[199]), 0x80u);
        v4 = this;
      }
    }
    v5 += 128;
  }
  return result;
}

// ===== sub_407250 @ 0x00407250..0x00407283 =====
int __thiscall sub_407250(void *this, int a2, int a3, int a4)
{
  int result; // eax
  int v6; // edi

  result = (*(int (__thiscall **)(void *, int, int))(*(_DWORD *)this + 16))(this, a3, a4);
  v6 = result;
  if ( result )
  {
    if ( a2 )
      sub_406C70((int)this, a2);
    return v6;
  }
  return result;
}

// ===== sub_407290 @ 0x00407290..0x004074E6 =====
int __thiscall sub_407290(_DWORD *this, void *a2, const char *a3, int a4, DWORD nNumberOfBytesToRead, int a6)
{
  int v7; // ecx
  unsigned int v8; // edi
  int v9; // ebx
  int result; // eax
  _BYTE *i; // esi
  char *v12; // edx
  _BYTE *v13; // ecx
  bool v14; // cf
  unsigned __int8 v15; // al
  int v16; // eax
  int v17; // edi
  DWORD v18; // esi
  unsigned int v19; // eax
  int v20; // edi
  _DWORD *v21; // [esp+14h] [ebp-348h]
  DWORD NumberOfBytesRead[3]; // [esp+18h] [ebp-344h] BYREF
  LPVOID lpBuffer; // [esp+24h] [ebp-338h]
  int v24; // [esp+28h] [ebp-334h]
  _BYTE v25[8]; // [esp+2Ch] [ebp-330h] BYREF
  char v26[788]; // [esp+34h] [ebp-328h] BYREF
  int v27; // [esp+358h] [ebp-4h]

  v7 = a6;
  lpBuffer = a2;
  v21 = this;
  v24 = a6;
  strcpy(v26, a3);
  sub_42EA80(v7, v26);
  v8 = this[197];
  v9 = 0;
  result = -2147483616;
  if ( v8 )
  {
    for ( i = (_BYTE *)this[199]; ; i += 128 )
    {
      v12 = v26;
      v13 = i;
      while ( 1 )
      {
        v14 = *v13 < (unsigned __int8)*v12;
        if ( *v13 != *v12 )
          break;
        if ( !*v13 )
          goto LABEL_8;
        v15 = v13[1];
        v14 = v15 < (unsigned __int8)v12[1];
        if ( v15 != v12[1] )
          break;
        v13 += 2;
        v12 += 2;
        if ( !v15 )
        {
LABEL_8:
          v16 = 0;
          goto LABEL_10;
        }
      }
      v16 = -v14 - (v14 - 1);
LABEL_10:
      if ( !v16 )
        break;
      if ( ++v9 >= v8 )
        return -2147483616;
    }
    sub_42D3B0(v13, v12);
    v27 = 0;
    sub_4526E0();
    LOBYTE(v27) = 1;
    sub_406C70((int)v21, (int)v26);
    v17 = 4000;
    if ( sub_42D520() )
    {
LABEL_18:
      v18 = nNumberOfBytesToRead;
      if ( !nNumberOfBytesToRead )
        v18 = *(_DWORD *)((v9 << 7) + v21[199] + 100);
      v19 = *(_DWORD *)(v21[199] + (v9 << 7) + 100);
      if ( v18 > v19 )
      {
        v20 = -2147483584;
        sub_42D5B0();
      }
      else
      {
        if ( a4 + v18 > v19 )
        {
          v20 = -2147483600;
        }
        else
        {
          (*(void (__thiscall **)(_DWORD *, int))(*v21 + 8))(v21, v24);
          sub_42D630(NumberOfBytesRead);
          v20 = sub_42D5D0(v18, lpBuffer, (DWORD)NumberOfBytesRead);
        }
        sub_42D5B0();
      }
    }
    else
    {
      while ( sub_42D560() == 5 && v17 )
      {
        sub_4527C0();
        --v17;
        if ( sub_42D520() )
        {
          if ( v17 )
            goto LABEL_18;
          break;
        }
      }
      v20 = -2147483632;
    }
    LOBYTE(v27) = 0;
    sub_4527A0(v25);
    v27 = -1;
    sub_42D400(NumberOfBytesRead);
    return v20;
  }
  return result;
}

// ===== sub_4074F0 @ 0x004074F0..0x004075D5 =====
int __thiscall sub_4074F0(_DWORD *this, const char *a2, int a3)
{
  unsigned int v4; // eax
  unsigned int v5; // edi
  const char *v6; // esi
  const char *v8; // [esp+8h] [ebp-314h]
  char v9[780]; // [esp+Ch] [ebp-310h] BYREF

  strcpy(v9, a2);
  sub_42EA80(this, v9);
  v4 = this[197];
  v5 = 0;
  if ( !v4 )
    return -2147483616;
  v6 = (const char *)this[199];
  v8 = v6;
  while ( strcmp(v6, v9) )
  {
    ++v5;
    v6 += 128;
    if ( v5 >= v4 )
      return -2147483616;
  }
  return *(_DWORD *)&v8[128 * v5 + 100];
}

// ===== sub_4075E0 @ 0x004075E0..0x004076CE =====
_DWORD *__usercall sub_4075E0@<eax>(_DWORD *a1@<edi>)
{
  int v1; // eax
  int i; // esi

  if ( operator new(0xB0u) )
    v1 = sub_42ECD0();
  else
    v1 = 0;
  a1[1] = v1;
  a1[3] = 0x4000;
  a1[2] = operator new[](0x120000u);
  for ( i = 0; i < 1179648; i += 72 )
  {
    *(_DWORD *)(i + a1[2]) = 0;
    *(_DWORD *)(a1[2] + i + 32) = -1;
    *(_DWORD *)(a1[2] + i + 36) = -1;
    *(_DWORD *)(a1[2] + i + 40) = -1;
    *(_DWORD *)(a1[2] + i + 44) = -1;
    InitializeCriticalSection((LPCRITICAL_SECTION)(a1[2] + i + 48));
  }
  a1[4] = 0;
  a1[5] = 0;
  memset(a1 + 6, 0, 0xC0u);
  a1[54] = 0;
  *a1 = 0;
  return a1;
}

// ===== sub_4076D0 @ 0x004076D0..0x00407752 =====
void __usercall sub_4076D0(int a1@<eax>)
{
  int v2; // ecx
  int v3; // esi
  int v4; // ebx
  void (__thiscall ***v5)(_DWORD, int); // edi

  *(_DWORD *)a1 = 240;
  sub_409AB0();
  while ( *(_DWORD *)(a1 + 216) )
    sub_40A260(v2, a1);
  v3 = 0;
  if ( *(int *)(a1 + 12) > 0 )
  {
    v4 = 0;
    do
    {
      sub_407CF0(a1);
      DeleteCriticalSection((LPCRITICAL_SECTION)(*(_DWORD *)(a1 + 8) + v4 + 48));
      ++v3;
      v4 += 72;
    }
    while ( v3 < *(_DWORD *)(a1 + 12) );
  }
  operator delete[](*(void **)(a1 + 8));
  v5 = *(void (__thiscall ****)(_DWORD, int))(a1 + 4);
  if ( v5 )
    (**v5)(v5, 1);
}

// ===== sub_407760 @ 0x00407760..0x00407763 =====
int __usercall sub_407760@<eax>(int a1@<eax>)
{
  return *(_DWORD *)a1;
}

// ===== sub_407770 @ 0x00407770..0x00407A50 =====
void sub_407770()
{
  int v0; // ecx
  __m128i *v1; // edx
  __m128i v2; // xmm3
  int v3; // esi
  __m128i *v4; // edi
  __int64 v5; // rax
  int v6; // eax
  int v7; // ebx
  int v8; // eax
  int v9; // eax
  int v10; // esi
  int v11; // edi
  long double v12; // st7
  int v13; // [esp+10h] [ebp-30h]
  int v14; // [esp+14h] [ebp-2Ch]
  int v15; // [esp+18h] [ebp-28h]
  int v16; // [esp+1Ch] [ebp-24h]
  int v17; // [esp+20h] [ebp-20h]
  int v18; // [esp+24h] [ebp-1Ch]
  int v19; // [esp+28h] [ebp-18h]
  int v20; // [esp+2Ch] [ebp-14h]

  if ( !dword_565AF8 )
  {
    v0 = 0;
    v1 = (__m128i *)&unk_50B0F0;
    do
    {
      v2 = _mm_cvtsi32_si128((unsigned __int16)v0);
      ++v1;
      ++v0;
      v1[-1] = _mm_unpacklo_epi16(
                 _mm_unpacklo_epi16(_mm_unpacklo_epi16(v2, v2), _mm_unpacklo_epi16(v2, v2)),
                 _mm_unpacklo_epi16(
                   _mm_unpacklo_epi16(v2, v2),
                   _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    }
    while ( (int)v1 < (int)xmmword_50B8F0 );
    v3 = 0;
    v4 = xmmword_50B8F0;
    xmmword_50B0E0[v0] = (__int128)_mm_unpacklo_epi16(
                                     _mm_unpacklo_epi16(
                                       _mm_unpacklo_epi16(
                                         _mm_cvtsi32_si128((unsigned __int16)v0),
                                         _mm_cvtsi32_si128((unsigned __int16)v0)),
                                       _mm_unpacklo_epi16(
                                         _mm_cvtsi32_si128((unsigned __int16)v0),
                                         _mm_cvtsi32_si128((unsigned __int16)v0))),
                                     _mm_unpacklo_epi16(
                                       _mm_unpacklo_epi16(
                                         _mm_cvtsi32_si128((unsigned __int16)v0),
                                         _mm_cvtsi32_si128((unsigned __int16)v0)),
                                       _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    do
    {
      *v4 = _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128((unsigned __int16)v3), _mm_cvtsi32_si128((unsigned __int16)v3)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128((unsigned __int16)v3), _mm_cvtsi32_si128((unsigned __int16)v3))),
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128((unsigned __int16)v3), _mm_cvtsi32_si128((unsigned __int16)v3)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
      v5 = 0x100010001LL * v3;
      dword_50A8D0[2 * v3] = v5;
      dword_50A8D4[2 * v3] = HIDWORD(v5);
      ++v4;
      ++v3;
    }
    while ( (int)v4 < (int)xmmword_50C900 );
    v6 = 3;
    v7 = 0;
    v14 = -1;
    v13 = 3;
    v17 = 12;
    while ( 1 )
    {
      dword_50B0D8[v7] = (int)operator new[](4 * v6 * v6);
      if ( v13 > 0 )
      {
        v8 = v14;
        v16 = 0;
        v15 = v14;
        v19 = v13;
        while ( 1 )
        {
          v9 = v8 * v8;
          v10 = v16;
          v11 = v14;
          v20 = v9;
          v18 = v13;
          while ( 1 )
          {
            v12 = sqrt((double)(v9 + v11 * v11)) - (double)(v7 + 1);
            if ( v12 > 0.0 )
              *(_DWORD *)(v10 + dword_50B0D8[v7]) = v12 < 1.0 ? (__int64)(v12 * 65536.0) : 0;
            else
              *(_DWORD *)(v10 + dword_50B0D8[v7]) = 0x10000;
            v10 += 4;
            ++v11;
            if ( !--v18 )
              break;
            v9 = v20;
          }
          v16 += v17;
          ++v15;
          if ( !--v19 )
            break;
          v8 = v15;
        }
      }
      v13 += 2;
      --v14;
      ++v7;
      v17 += 8;
      if ( v17 >= 52 )
        break;
      v6 = v13;
    }
    dword_565AF8 = 1;
  }
}

// ===== sub_407A50 @ 0x00407A50..0x00407A88 =====
void sub_407A50()
{
  void **v0; // esi

  if ( dword_565AF8 )
  {
    v0 = (void **)dword_50B0D8;
    do
    {
      operator delete[](*v0);
      *v0++ = 0;
    }
    while ( (int)v0 < (int)((void **)xmmword_50B0E0 + 3) );
    dword_565AF8 = 0;
  }
}

// ===== sub_407A90 @ 0x00407A90..0x00407A96 =====
int __usercall sub_407A90@<eax>(int result@<eax>)
{
  dword_565AFC = result;
  return result;
}

// ===== sub_407AA0 @ 0x00407AA0..0x00407AA6 =====
int __usercall sub_407AA0@<eax>(int result@<eax>)
{
  dword_565B00 = result;
  return result;
}

// ===== sub_407AB0 @ 0x00407AB0..0x00407AB6 =====
int __usercall sub_407AB0@<eax>(int result@<eax>)
{
  dword_565B04 = result;
  return result;
}

// ===== sub_407AC0 @ 0x00407AC0..0x00407AD9 =====
int __usercall sub_407AC0@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  dword_565B08 = result;
  dword_565B0C = a2;
  dword_565B10 = a3;
  return result;
}

// ===== sub_407AE0 @ 0x00407AE0..0x00407AFB =====
BOOL sub_407AE0()
{
  return !dword_565B08 && dword_565B0C == 15;
}

// ===== sub_407B00 @ 0x00407B00..0x00407B06 =====
int __usercall sub_407B00@<eax>(int result@<eax>)
{
  dword_565B14 = result;
  return result;
}

// ===== sub_407B10 @ 0x00407B10..0x00407B16 =====
int sub_407B10()
{
  return dword_565B14;
}

// ===== sub_407B20 @ 0x00407B20..0x00407B2A =====
int sub_407B20()
{
  sub_407B10();
  return sub_407B30();
}

// ===== sub_407B30 @ 0x00407B30..0x00407B38 =====
int __usercall sub_407B30@<eax>(int a1@<eax>)
{
  return dword_4E41B0[a1];
}

// ===== sub_407B40 @ 0x00407B40..0x00407B4C =====
int __usercall sub_407B40@<eax>(int a1@<eax>)
{
  return 8 * sub_407B30(a1);
}

// ===== sub_407B50 @ 0x00407B50..0x00407B56 =====
int __usercall sub_407B50@<eax>(int result@<eax>)
{
  dword_565B18 = result;
  return result;
}

// ===== sub_407B60 @ 0x00407B60..0x00407B66 =====
int sub_407B60()
{
  return dword_565B18;
}

// ===== sub_407B70 @ 0x00407B70..0x00407B8C =====
int __fastcall sub_407B70(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 )
  {
    dword_5076B0 = (a1 + 0xFFFF) / a1;
    return 1;
  }
  return result;
}

// ===== sub_407B90 @ 0x00407B90..0x00407B96 =====
int __usercall sub_407B90@<eax>(int result@<eax>)
{
  dword_565B1C = result;
  return result;
}

// ===== sub_407BA0 @ 0x00407BA0..0x00407BA6 =====
int sub_407BA0()
{
  return dword_565B1C;
}

// ===== sub_407BB0 @ 0x00407BB0..0x00407BB9 =====
void *__thiscall sub_407BB0(void *this)
{
  void *result; // eax

  result = dword_566750;
  *((_DWORD *)dword_566750 + 5) = this;
  return result;
}

// ===== sub_407BC0 @ 0x00407BC0..0x00407BDD =====
int __fastcall sub_407BC0(int a1, int a2, int a3, int a4)
{
  return sub_42EDD0(*((_DWORD *)dword_566750 + 1), a2, a1);
}

// ===== sub_407BE0 @ 0x00407BE0..0x00407BED =====
int sub_407BE0()
{
  return sub_42EEC0(*(_DWORD *)(dword_565B70 + 4));
}

// ===== sub_407BF0 @ 0x00407BF0..0x00407C17 =====
int __userpurge sub_407BF0@<eax>(int a1@<ecx>, int a2@<eax>, LPCSTR pszFaceName, int a4, int a5, int a6)
{
  return sub_42EF10(a5, pszFaceName, a4, a1, a2);
}

// ===== sub_407C20 @ 0x00407C20..0x00407C3F =====
int __fastcall sub_407C20(int a1, int a2, int a3, int a4)
{
  return sub_42EF60(*((_DWORD *)dword_566750 + 1), a3, a4, a2, a1);
}

// ===== sub_407C40 @ 0x00407C40..0x00407C8E =====
int __userpurge sub_407C40@<eax>(int a1@<edi>, int a2)
{
  int result; // eax

  result = 0;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
  {
    EnterCriticalSection((LPCRITICAL_SECTION)(*(_DWORD *)(a2 + 8) + 72 * a1 + 48));
    if ( *(_DWORD *)(72 * a1 + *(_DWORD *)(a2 + 8)) )
    {
      return 1;
    }
    else
    {
      sub_407C90(a1, a2);
      return 0;
    }
  }
  return result;
}

// ===== sub_407C90 @ 0x00407C90..0x00407CB2 =====
int __fastcall sub_407C90(int a1, int a2)
{
  int result; // eax

  result = 0;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
  {
    LeaveCriticalSection((LPCRITICAL_SECTION)(*(_DWORD *)(a2 + 8) + 72 * a1 + 48));
    return 1;
  }
  return result;
}

// ===== sub_407CF0 @ 0x00407CF0..0x00407D9E =====
int __userpurge sub_407CF0@<eax>(int a1@<eax>, int a2)
{
  int v3; // esi
  void (__thiscall ***v4)(_DWORD, int); // ecx

  if ( a1 < 0 || a1 >= *(_DWORD *)(a2 + 12) || !sub_407C40(a1, a2) )
    return 0;
  v3 = 72 * a1;
  v4 = *(void (__thiscall ****)(_DWORD, int))(72 * a1 + *(_DWORD *)(a2 + 8));
  if ( v4 )
    (**v4)(v4, 1);
  *(_DWORD *)(v3 + *(_DWORD *)(a2 + 8)) = 0;
  sub_407C90(a1, a2);
  if ( *(_DWORD *)(*(_DWORD *)(a2 + 8) + v3 + 32) != -1 )
  {
    if ( !sub_408A10() )
      sub_408DC0();
    *(_DWORD *)(*(_DWORD *)(a2 + 8) + v3 + 32) = -1;
    *(_DWORD *)(*(_DWORD *)(a2 + 8) + v3 + 36) = -1;
  }
  *(_DWORD *)(*(_DWORD *)(a2 + 8) + v3 + 40) = -1;
  *(_DWORD *)(*(_DWORD *)(a2 + 8) + v3 + 44) = -1;
  return 1;
}

// ===== sub_407DA0 @ 0x00407DA0..0x00407EF1 =====
int __fastcall sub_407DA0(int a1, _DWORD *a2, int a3, int a4, unsigned int a5)
{
  int v7; // eax
  int *v8; // ebx
  int v9; // eax
  int v10; // eax
  int v12; // [esp+14h] [ebp-18h]
  int v13; // [esp+1Ch] [ebp-10h]

  if ( a1 < 0 || a1 >= a2[3] )
    return 0;
  v13 = sub_408300();
  if ( v13 == -1 || !a2[5] )
  {
    v13 = a2[4];
    a2[4] = v13 + 1;
  }
  sub_407CF0(a1, (int)a2);
  v7 = a5;
  if ( a5 <= 6 )
    goto LABEL_9;
  if ( a5 != 7 )
    return 0;
  a5 = 1;
  v7 = 1;
LABEL_9:
  v8 = (int *)(a2[2] + 72 * a1);
  v12 = sub_407B30(v7);
  if ( operator new(0x10u) )
    v9 = sub_430260();
  else
    v9 = 0;
  *v8 = v9;
  v10 = sub_4302C0();
  v8[1] = v10;
  if ( v10 )
  {
    v8[2] = a3 * v12;
    v8[3] = a3;
    v8[4] = a4;
    v8[5] = a5;
    v8[6] = v12;
    v8[7] = v13;
    return 1;
  }
  else
  {
    if ( *v8 )
      (**(void (__thiscall ***)(int, int))*v8)(*v8, 1);
    *v8 = 0;
    return 0;
  }
}

// ===== sub_407F00 @ 0x00407F00..0x00407F15 =====
int __fastcall sub_407F00(int a1, int a2)
{
  int result; // eax

  result = 0;
  if ( a1 >= 0 && a1 < *(_DWORD *)(a2 + 12) )
    return *(_DWORD *)(a2 + 8) + 72 * a1;
  return result;
}

// ===== sub_407F20 @ 0x00407F20..0x00407FAF =====
int __usercall sub_407F20@<eax>(int a1@<edx>, int a2@<ecx>, _DWORD *a3@<esi>)
{
  int result; // eax
  int v4; // edx
  int v5; // ecx
  bool v6; // zf
  _DWORD *v7; // edx

  result = 0;
  if ( a2 >= 0 && a2 < *(_DWORD *)(a1 + 12) )
  {
    v4 = *(_DWORD *)(a1 + 8);
    v5 = 9 * a2;
    v6 = *(_DWORD *)(v4 + 8 * v5) == 0;
    v7 = (_DWORD *)(v4 + 8 * v5);
    if ( !v6 )
    {
      if ( v7[8] == -1 || !sub_408E50() )
      {
        *a3 = v7[1];
        a3[1] = v7[2];
        a3[2] = v7[3];
        a3[3] = v7[4];
        a3[4] = v7[5];
        a3[5] = v7[6];
        return 1;
      }
      else
      {
        *a3 = v7[1];
        a3[1] = v7[2];
        a3[2] = v7[3];
        a3[3] = v7[4];
        a3[4] = v7[5];
        a3[5] = v7[6];
        sub_493350(a3, a3 + 1);
        return 1;
      }
    }
  }
  return result;
}

// ===== sub_407FB0 @ 0x00407FB0..0x00408016 =====
BOOL __usercall sub_407FB0@<eax>(int a1@<eax>, _DWORD *a2@<edi>)
{
  int v2; // ecx
  int v3; // eax
  bool v4; // zf
  int v5; // eax

  if ( a1 < 0 )
    return 0;
  if ( a1 >= *((_DWORD *)dword_566750 + 3) )
    return 0;
  v2 = *((_DWORD *)dword_566750 + 2);
  v3 = 9 * a1;
  v4 = *(_DWORD *)(v2 + 8 * v3) == 0;
  v5 = v2 + 8 * v3;
  if ( v4 )
    return 0;
  if ( *(_DWORD *)(v5 + 32) == -1 )
  {
    *a2 = *(_DWORD *)(v5 + 36);
    return 1;
  }
  if ( !sub_408A90() )
    return 0;
  if ( sub_42C570() )
    return sub_44D560(a2) == 0;
  *a2 = -1;
  return 1;
}
