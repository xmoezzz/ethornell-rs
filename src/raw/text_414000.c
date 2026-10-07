#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_4140A0 @ 0x004140A0..0x00414253 =====
int __fastcall sub_4140A0(int *a1, int *a2, int *a3, unsigned int *a4, int a5, int a6)
{
  int v6; // esi
  unsigned int v7; // ebx
  __int64 v8; // rax
  int v9; // ecx
  int v10; // edi
  int v11; // esi
  int v12; // ecx
  int v13; // ebx
  unsigned int *i; // eax
  __m64 v15; // mm0
  __m64 v16; // mm1
  int result; // eax
  int v18; // [esp-8h] [ebp-854h]
  int v19; // [esp-4h] [ebp-850h]
  unsigned int v20; // [esp+14h] [ebp-838h]
  int v21; // [esp+18h] [ebp-834h]
  int v22; // [esp+1Ch] [ebp-830h]
  int v23; // [esp+20h] [ebp-82Ch]
  int v24; // [esp+28h] [ebp-824h]
  int v25; // [esp+2Ch] [ebp-820h]
  int v26; // [esp+30h] [ebp-81Ch]
  int v27; // [esp+34h] [ebp-818h]
  int v28; // [esp+38h] [ebp-814h]
  unsigned int v29; // [esp+3Ch] [ebp-810h]
  int v30; // [esp+40h] [ebp-80Ch]
  __m64 v31[256]; // [esp+44h] [ebp-808h]

  v24 = *a1;
  v26 = *a2;
  v27 = a1[1];
  v21 = a3[1];
  v22 = a2[1];
  v28 = a1[2];
  v29 = *a4;
  v20 = a4[1] * a4[3] + *a4;
  v6 = 0;
  v23 = a1[3];
  v30 = *a3;
  v25 = a4[1] * a4[3];
  v7 = 0;
  do
  {
    v8 = 0x100010001LL * ((v7 >> 4) & 0xFFFFFF0);
    v31[v6].m64_i32[0] = v8;
    v31[v6++].m64_i32[1] = HIDWORD(v8);
    v7 += 256 - a6;
  }
  while ( v6 < 256 );
  v9 = v23;
  v10 = v24;
  v11 = v26;
  do
  {
    v19 = v9;
    v18 = v11;
    v12 = v28;
    v13 = 0;
    do
    {
      for ( i = (unsigned int *)(v13
                               + v30
                               + _mm_cvtsi64_si32(
                                   _m_pmaddwd(
                                     _m_pmulhw(
                                       _mm_cvtsi32_si64(*(_DWORD *)v11),
                                       _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v11 + 4)))),
                                     (__m64)((v21 << 16) + 4))));
            (unsigned int)i < v29;
            i = (unsigned int *)((char *)i + v25) )
      {
        ;
      }
      while ( (unsigned int)i >= v20 )
        i = (unsigned int *)((char *)i - v25);
      v11 += 6;
      v15 = _mm_cvtsi32_si64(*i);
      if ( HIBYTE(*i) )
      {
        v16 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v10 + v13)), 0LL);
        *(_DWORD *)(v10 + v13) = _mm_cvtsi64_si32(
                                   _m_packuswb(
                                     _m_paddw(
                                       _m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v15, 0LL), v16), 4u), v31[HIBYTE(*i)]),
                                       v16),
                                     0LL));
      }
      v13 += 4;
      --v12;
    }
    while ( v12 );
    result = v27;
    v10 += v27;
    v30 += v21;
    v11 = v22 + v18;
    v9 = v19 - 1;
  }
  while ( v19 != 1 );
  _m_empty();
  return result;
}

// ===== sub_414260 @ 0x00414260..0x00414288 =====
int __cdecl sub_414260(int a1, int a2, int a3)
{
  int v3; // ecx

  return sub_414290(a1, a3, *(_DWORD *)(v3 + 8) << 15, *(_DWORD *)(v3 + 12) << 15);
}

// ===== sub_414290 @ 0x00414290..0x004142D7 =====
int __fastcall sub_414290(int a1, int a2, int a3, int a4, int a5, int a6)
{
  int v6; // esi

  if ( !a2 )
    return 19;
  v6 = *(_DWORD *)(a1 + 16);
  if ( *(_DWORD *)(a3 + 16) != v6 )
    return 1;
  if ( (unsigned int)(v6 - 1) <= 1 )
    sub_4142E0(a2, a4, a5, a6);
  return 0;
}

// ===== sub_4142E0 @ 0x004142E0..0x0041455B =====
int __usercall sub_4142E0@<eax>(int *a1@<eax>, int *a2@<ecx>, unsigned int a3, int a4, int a5, int a6)
{
  double v7; // st2
  long double v8; // st0
  long double v9; // st7
  __m64 v10; // mm6
  int v11; // ecx
  int v12; // edi
  int v13; // esi
  int v14; // ecx
  int v15; // ebx
  __m64 v16; // mm2
  int v17; // edx
  int result; // eax
  int v19; // [esp-8h] [ebp-64h]
  long double v20; // [esp+Ch] [ebp-50h]
  double v21; // [esp+14h] [ebp-48h]
  __m64 v22; // [esp+14h] [ebp-48h]
  double v23; // [esp+1Ch] [ebp-40h]
  long double v24; // [esp+24h] [ebp-38h]
  __m64 v25; // [esp+24h] [ebp-38h]
  long double v26; // [esp+2Ch] [ebp-30h]
  __m64 v27; // [esp+2Ch] [ebp-30h]
  double v28; // [esp+3Ch] [ebp-20h]
  __m64 v29; // [esp+3Ch] [ebp-20h]
  int v30; // [esp+54h] [ebp-8h]
  int v31; // [esp+58h] [ebp-4h]

  v23 = -((double)(unsigned int)(a1[2] - 1) * 0.5);
  v28 = 0.5 * (double)(unsigned int)(a1[3] - 1);
  v21 = (double)a3;
  v31 = a1[1];
  v30 = a1[2];
  v26 = -((double)a4 * 3.141592653589793 / 11796480.0);
  v24 = cos(v26);
  v7 = 65536.0 / v21;
  v8 = sin(v26);
  v9 = v23 * v24 - v28 * v8;
  v29.m64_i32[1] = (int)(((double)a6 * v21 * 0.0000152587890625 * 0.0000152587890625 - (v24 * v28 + v8 * v23))
                       * v7
                       * 65536.0);
  v29.m64_i32[0] = (__int64)((v9 - -(v21 * (double)a5 * 0.0000152587890625 * 0.0000152587890625)) * v7 * 65536.0);
  v22.m64_i32[1] = (int)(v8 * v7 * -65536.0);
  v22.m64_i32[0] = (__int64)(v24 * v7 * 65536.0);
  v20 = 1.570796326794897 - (double)a4 * 3.141592653589793 / 11796480.0;
  v27.m64_i32[0] = (__int64)(cos(v20) * v7 * -65536.0);
  v25.m64_i32[1] = (a2[3] << 16) - 1;
  v27.m64_i32[1] = (int)(sin(v20) * v7 * 65536.0);
  v25.m64_i32[0] = (a2[2] << 16) - 1;
  v10.m64_u64 = (a2[1] << 16) | 4;
  v11 = a1[3];
  v12 = *a1;
  v13 = *a2;
  do
  {
    v19 = v11;
    v14 = v30;
    v15 = 0;
    v16 = v29;
    v29 = _m_paddd(v27, v29);
    do
    {
      v17 = 0;
      if ( _mm_cvtsi64_si32(_m_packssdw(_m_pandn(_m_pcmpgtd(v16, v25), _m_pcmpgtd(v16, (__m64)-1LL)), 0LL)) == -1 )
        v17 = *(_DWORD *)(v13 + _mm_cvtsi64_si32(_m_pmaddwd(_m_packssdw(_m_psrldi(v16, 0x10u), 0LL), v10)));
      v16 = _m_paddd(v16, v22);
      *(_DWORD *)(v12 + v15) = v17;
      v15 += 4;
      --v14;
    }
    while ( v14 );
    result = v31;
    v12 += v31;
    v11 = v19 - 1;
  }
  while ( v19 != 1 );
  _m_empty();
  return result;
}

// ===== sub_414560 @ 0x00414560..0x004145A0 =====
int __fastcall sub_414560(unsigned int a1, unsigned int a2, int a3, int a4, int a5, int a6)
{
  return sub_4145A0(a3, a5, a6, a2, a1, *(_DWORD *)(a3 + 8) << 15, *(_DWORD *)(a3 + 12) << 15, a2 >> 1, a1 >> 1);
}

// ===== sub_4145A0 @ 0x004145A0..0x0041460B =====
int __usercall sub_4145A0@<eax>(
        int a1@<eax>,
        int a2@<edx>,
        int a3@<esi>,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  int v12; // edi

  if ( a7 < 0x20000 || a8 < 0x20000 )
    return 20;
  if ( !a3 || !a1 )
    return 19;
  v12 = *(_DWORD *)(a2 + 16);
  if ( *(_DWORD *)(a4 + 16) != v12 )
    return 1;
  if ( (unsigned int)(v12 - 1) <= 1 )
    sub_414610(a5, a6, a3, a1, a9, a10, a11, a12);
  return 0;
}

// ===== sub_414610 @ 0x00414610..0x004149B0 =====
int __fastcall sub_414610(
        int *a1,
        int *a2,
        int a3,
        int a4,
        unsigned int a5,
        unsigned int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  double v10; // st7
  double v11; // st5
  int v12; // ecx
  int v13; // esi
  __m64 v14; // mm4
  int v15; // edi
  unsigned int v16; // ebx
  __m64 v17; // mm6
  int v18; // ecx
  unsigned int v19; // edx
  __m64 v20; // mm0
  __m64 v21; // mm1
  unsigned int v22; // edx
  unsigned int v23; // eax
  unsigned int v24; // eax
  __m64 v25; // mm1
  __m64 v26; // mm2
  __int16 v27; // di
  __m64 v28; // mm3
  unsigned int v29; // edx
  int v30; // ecx
  unsigned int v31; // eax
  __m64 v32; // mm3
  __m64 v33; // mm5
  __m64 v34; // mm2
  int result; // eax
  int v36; // [esp-4h] [ebp-45Ch]
  unsigned int v37; // [esp+10h] [ebp-448h]
  int v38; // [esp+14h] [ebp-444h]
  int v39; // [esp+18h] [ebp-440h]
  int v40; // [esp+1Ch] [ebp-43Ch]
  int v41; // [esp+20h] [ebp-438h]
  unsigned int v42; // [esp+24h] [ebp-434h]
  int v43; // [esp+2Ch] [ebp-42Ch]
  __m64 v44; // [esp+38h] [ebp-420h]
  unsigned int v45; // [esp+40h] [ebp-418h]
  int v46; // [esp+44h] [ebp-414h]
  __m64 v47; // [esp+48h] [ebp-410h]
  int v48; // [esp+4Ch] [ebp-40Ch]
  _DWORD v49[257]; // [esp+50h] [ebp-408h]

  v10 = 65536.0 / (double)a5;
  v11 = 65536.0 / (double)a6;
  if ( a5 >= 0x10000 )
    v43 = 0;
  else
    v43 = (int)((v10 * 0.5 - 0.5) * 65536.0);
  if ( a6 >= 0x10000 )
    v48 = 0;
  else
    v48 = (int)((v11 * 0.5 - 0.5) * 65536.0);
  v46 = *a1;
  v40 = *a2;
  v39 = a1[1];
  v45 = a1[2];
  v41 = a2[1];
  v38 = a1[3];
  v42 = a2[2];
  v37 = a2[3];
  v44.m64_i32[1] = a4
                 + v48
                 - (int)((0.0000152587890625 * (double)a10 - (double)a8 * v11 * 0.0000152587890625) * -65536.0);
  v44.m64_i32[0] = a3
                 + v43
                 + (__int64)(((double)a9 * 0.0000152587890625 - (double)a7 * v10 * 0.0000152587890625) * 65536.0);
  v47.m64_i32[1] = (int)(v11 * 65536.0);
  v12 = 0;
  v47.m64_i32[0] = 0;
  v13 = 128;
  do
  {
    v49[2 * v12] = v13 | ((v13 | ((v13 | (v13 << 16)) << 16)) << 16);
    v49[2 * v12++ + 1] = (v13 >> 31) | ((v13 | ((v13 | (unsigned __int64)((__int64)v13 << 16)) << 16)) >> 16);
    --v13;
  }
  while ( v12 < 128 );
  v14 = _mm_cvtsi32_si64(v45);
  v15 = v38;
  do
  {
    v36 = v15;
    v16 = 0;
    v17 = v44;
    v44 = _m_paddd(v47, v44);
    do
    {
      v18 = _mm_cvtsi64_si32(_m_pmaddwd(_m_packssdw(_m_psrldi(v17, 0x10u), 0LL), (__m64)((v41 << 16) | 4)));
      v19 = _mm_cvtsi64_si32(_m_packssdw(_m_psrldi(v17, 0x10u), 0LL));
      v20.m64_u64 = 0LL;
      LOWORD(v23) = v19;
      v21.m64_u64 = 0LL;
      v22 = HIWORD(v19);
      v23 = (unsigned __int16)v23;
      if ( v22 < v37 )
      {
        if ( (unsigned __int16)v23 < v42 )
          v20 = _mm_cvtsi32_si64(*(_DWORD *)(v40 + v18));
        v20 = _m_punpcklbw(v20, 0LL);
        if ( (_mm_cvtsi64_si32(v17) & 0xFE00) != 0 )
        {
          v24 = (unsigned __int16)v23 + 1;
          if ( v24 < v42 )
            v21 = _mm_cvtsi32_si64(*(_DWORD *)(v40 + v18 + 4));
          v23 = v24 - 1;
        }
      }
      v25 = _m_punpcklbw(v21, 0LL);
      v26.m64_u64 = 0LL;
      v27 = _mm_cvtsi64_si32(_m_psrlqi(v17, 0x20u));
      v28.m64_u64 = 0LL;
      v29 = v22 + 1;
      if ( (v27 & 0xFE00) != 0 )
      {
        v30 = v41 + v18;
        if ( v29 < v37 )
        {
          if ( v23 < v42 )
            v26 = _mm_cvtsi32_si64(*(_DWORD *)(v40 + v30));
          v26 = _m_punpcklbw(v26, 0LL);
          v31 = v23 + 1;
          if ( (_mm_cvtsi64_si32(v17) & 0xFE00) != 0 && v31 < v42 )
            v28 = _mm_cvtsi32_si64(*(_DWORD *)(v40 + v30 + 4));
        }
      }
      v32 = _m_punpcklbw(v28, 0LL);
      v33 = *(__m64 *)((char *)v49 + ((_mm_cvtsi64_si32(v17) & 0xFE00u) >> 6));
      v34 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v26, v32), v33), 7u), v32);
      *(_DWORD *)(v46 + 4 * v16) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v20, v25), v33), 7u), v25),
                                               v34),
                                             *(__m64 *)((char *)v49 + ((unsigned __int16)(v27 & 0xFE00) >> 6))),
                                           7u),
                                         v34),
                                       *(__m64 *)((char *)v49 + ((unsigned __int16)(v27 & 0xFE00) >> 6))));
      v17 = _m_paddd(v17, (__m64)(int)(v10 * 65536.0));
      ++v16;
    }
    while ( v16 < _mm_cvtsi64_si32(v14) );
    result = v39;
    v46 += v39;
    v15 = v36 - 1;
  }
  while ( v36 != 1 );
  _m_empty();
  return result;
}

// ===== sub_4149B0 @ 0x004149B0..0x00414A0D =====
int __usercall sub_4149B0@<eax>(unsigned int a1@<eax>, _DWORD *a2@<edx>, int a3@<edi>, int a4)
{
  int v4; // esi

  v4 = a2[4];
  if ( *(_DWORD *)(a4 + 16) != v4 )
    return 1;
  if ( (unsigned int)(v4 - 1) <= 1 )
  {
    if ( a3 )
    {
      if ( !a1 )
      {
        sub_414A10(a3);
        return 0;
      }
      if ( a1 < 0x100 )
      {
        sub_414AF0(a2, a3);
        return 0;
      }
    }
    else
    {
      sub_40B320((int)a2, a1, a4);
    }
  }
  return 0;
}

// ===== sub_414A10 @ 0x00414A10..0x00414AEA =====
void __usercall sub_414A10(unsigned int *a1@<eax>, int a2@<edx>, int a3)
{
  _DWORD *v3; // esi
  int v4; // edx
  __m64 v5; // mm4
  __m64 v6; // mm5
  __m64 v7; // mm6
  __m64 v8; // mm7
  int v9; // ecx
  unsigned int v10; // edi
  unsigned int v11; // ebx
  __m64 v12; // mm3
  unsigned int v13; // ecx
  _DWORD *v14; // edi
  unsigned int v15; // eax
  int v16; // edx
  unsigned int v17; // eax
  __m64 v18; // mm0
  char *v19; // ebx
  const void *v20; // edx
  _DWORD *v21; // [esp-4h] [ebp-30h]
  unsigned int v22; // [esp+Ch] [ebp-20h]
  int v23; // [esp+14h] [ebp-18h]
  unsigned int v24; // [esp+18h] [ebp-14h]
  unsigned int v25; // [esp+20h] [ebp-Ch]
  int v26; // [esp+24h] [ebp-8h]
  void *v27; // [esp+28h] [ebp-4h]
  unsigned int v28; // [esp+34h] [ebp+8h]

  v24 = *a1;
  v3 = *(_DWORD **)a2;
  v4 = (a3 + 1) * *(_DWORD *)(a2 + 4);
  v28 = a3 + 1;
  v26 = v4;
  v25 = a1[1];
  v22 = a1[2];
  v23 = a1[3];
  v27 = operator new[](4 * v22);
  v5 = _mm_cvtsi32_si64(v22);
  v6 = _mm_cvtsi32_si64(v28);
  v7 = _mm_cvtsi32_si64((unsigned int)v27);
  v8 = _mm_cvtsi32_si64(4 * v28);
  v9 = v23;
  v10 = v24;
  do
  {
    v21 = v3;
    v11 = v9;
    v12 = _mm_cvtsi32_si64(v10);
    v13 = _mm_cvtsi64_si32(v5);
    v14 = (_DWORD *)_mm_cvtsi64_si32(v7);
    do
    {
      v15 = _mm_cvtsi64_si32(v6);
      if ( v13 < v15 )
        v15 = v13;
      v16 = *v3;
      v13 -= v15;
      do
      {
        *v14++ = v16;
        --v15;
      }
      while ( v15 );
      v3 = (_DWORD *)((char *)v3 + _mm_cvtsi64_si32(v8));
    }
    while ( v13 );
    v17 = _mm_cvtsi64_si32(v6);
    if ( v11 < v17 )
      v17 = v11;
    v18 = _mm_cvtsi32_si64(v11 - v17);
    v19 = (char *)_mm_cvtsi64_si32(v12);
    v20 = (const void *)_mm_cvtsi64_si32(v7);
    do
    {
      qmemcpy(v19, v20, 4 * _mm_cvtsi64_si32(v5));
      v19 += v25;
      --v17;
    }
    while ( v17 );
    v9 = _mm_cvtsi64_si32(v18);
    v10 = (unsigned int)v19;
    v3 = (_DWORD *)((char *)v21 + v26);
  }
  while ( v9 );
  _m_empty();
  operator delete[](v27);
}

// ===== sub_414AF0 @ 0x00414AF0..0x00414C1C =====
void __usercall sub_414AF0(unsigned int a1@<eax>, int *a2@<ecx>, _DWORD *a3, int a4)
{
  int v4; // esi
  unsigned __int64 v5; // rt0
  __m64 v6; // mm4
  __m64 v7; // mm5
  __m64 v8; // mm7
  int v9; // ecx
  int v10; // edi
  _DWORD *v11; // esi
  unsigned int v12; // ebx
  unsigned int v13; // ecx
  _DWORD *v14; // edi
  unsigned int v15; // eax
  int v16; // edx
  unsigned int v17; // eax
  __m64 v18; // mm2
  int v19; // ebx
  int v20; // ecx
  __m64 v21; // mm1
  int v22; // [esp-8h] [ebp-3Ch]
  _DWORD *v23; // [esp-4h] [ebp-38h]
  __m64 v24; // [esp+Ch] [ebp-28h]
  unsigned int v25; // [esp+18h] [ebp-1Ch]
  int v26; // [esp+20h] [ebp-14h]
  int v27; // [esp+24h] [ebp-10h]
  _DWORD *v28; // [esp+28h] [ebp-Ch]
  int v29; // [esp+2Ch] [ebp-8h]
  int v30; // [esp+30h] [ebp-4h]
  _DWORD *v31; // [esp+3Ch] [ebp+8h]
  unsigned int v32; // [esp+40h] [ebp+Ch]

  v27 = *a2;
  v4 = (a4 + 1) * a3[1];
  v32 = a4 + 1;
  v28 = (_DWORD *)*a3;
  v30 = v4;
  v29 = a2[1];
  v26 = a2[3];
  HIDWORD(v5) = (unsigned __int64)a1 >> 16;
  LODWORD(v5) = a1 | (a1 << 16);
  HIDWORD(v5) = v5 >> 16;
  LODWORD(v5) = a1 | ((_DWORD)v5 << 16);
  v25 = a2[2];
  v24.m64_i32[0] = (16 * a1) | ((a1 | ((a1 | (a1 << 16)) << 16)) << 20);
  v24.m64_i32[1] = v5 >> 12;
  v31 = operator new[](4 * v25);
  v6 = _mm_cvtsi32_si64(v25);
  v7 = _mm_cvtsi32_si64(v32);
  v8 = _mm_cvtsi32_si64(4 * v32);
  v9 = v26;
  v10 = v27;
  v11 = v28;
  do
  {
    v23 = v11;
    v22 = v10;
    v12 = v9;
    v13 = _mm_cvtsi64_si32(v6);
    v14 = v31;
    do
    {
      v15 = _mm_cvtsi64_si32(v7);
      if ( v13 < v15 )
        v15 = v13;
      v16 = *v11;
      v13 -= v15;
      do
      {
        *v14++ = v16;
        --v15;
      }
      while ( v15 );
      v11 = (_DWORD *)((char *)v11 + _mm_cvtsi64_si32(v8));
    }
    while ( v13 );
    v17 = _mm_cvtsi64_si32(v7);
    if ( v12 < v17 )
      v17 = v12;
    v18 = _mm_cvtsi32_si64(v12 - v17);
    v10 = v22;
    do
    {
      v19 = 0;
      v20 = _mm_cvtsi64_si32(v6);
      do
      {
        v21 = _m_punpcklbw(_mm_cvtsi32_si64(v31[v19]), 0LL);
        *(_DWORD *)(v10 + v19 * 4) = _mm_cvtsi64_si32(
                                       _m_packuswb(
                                         _m_paddw(
                                           _m_pmulhw(
                                             _m_psllwi(
                                               _m_psubw(
                                                 _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v10 + v19 * 4)), 0LL),
                                                 v21),
                                               4u),
                                             v24),
                                           v21),
                                         0LL));
        ++v19;
        --v20;
      }
      while ( v20 );
      v10 += v29;
      --v17;
    }
    while ( v17 );
    v9 = _mm_cvtsi64_si32(v18);
    v11 = (_DWORD *)((char *)v23 + v30);
  }
  while ( v9 );
  _m_empty();
  operator delete[](v31);
}

// ===== sub_414C20 @ 0x00414C20..0x00414C51 =====
int __usercall sub_414C20@<eax>(int result@<eax>, int a2@<ecx>, int a3@<esi>)
{
  if ( a3 )
  {
    if ( *(_DWORD *)(result + 16) == 1 )
    {
      if ( *(_DWORD *)(a2 + 16) == 1 )
        return sub_414C60(a3);
    }
    else if ( *(_DWORD *)(result + 16) == 2 && *(_DWORD *)(a2 + 16) == 1 )
    {
      return sub_414D10(result, a3);
    }
  }
  return result;
}

// ===== sub_414C60 @ 0x00414C60..0x00414D04 =====
int __usercall sub_414C60@<eax>(int *a1@<eax>, int *a2@<ecx>, unsigned int a3)
{
  int v3; // edx
  int v4; // edi
  int v5; // esi
  int v6; // ecx
  int result; // eax
  __m64 v8; // mm2
  __m64 v9; // mm3
  int v10; // [esp+1Ch] [ebp-Ch]
  int v11; // [esp+20h] [ebp-8h]
  int v12; // [esp+24h] [ebp-4h]

  v11 = a2[1];
  v12 = a1[1];
  v10 = a1[2];
  v3 = a1[3];
  v4 = *a2;
  v5 = *a1;
  do
  {
    v6 = v10;
    result = 0;
    do
    {
      v8 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v4 + result)), 0LL);
      v9 = _m_pmulhw(
             _m_psllwi(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v5 + result)), 0LL), 4u),
             (__m64)(0x10001000100010LL * a3));
      *(_DWORD *)(v4 + result) = _mm_cvtsi64_si32(
                                   _m_packuswb(
                                     _m_psubsw(_m_paddw(v8, v9), _m_pmulhw(_m_psllwi(v8, 4u), _m_psllwi(v9, 4u))),
                                     0LL));
      result += 4;
      --v6;
    }
    while ( v6 );
    v4 += v11;
    v5 += v12;
    --v3;
  }
  while ( v3 );
  _m_empty();
  return result;
}

// ===== sub_414D10 @ 0x00414D10..0x00414EF1 =====
unsigned int __cdecl sub_414D10(unsigned int **a1, unsigned int a2)
{
  unsigned int **v2; // ecx
  int v3; // esi
  unsigned int v4; // edi
  __int64 v5; // rax
  unsigned int *v6; // ecx
  unsigned int *v7; // edi
  unsigned int *v8; // esi
  unsigned int *v9; // edx
  unsigned int result; // eax
  __m64 v11; // mm1
  __m64 v12; // mm1
  __m64 v13; // mm2
  unsigned int *v14; // ecx
  unsigned int *v15; // edi
  unsigned int *v16; // esi
  unsigned int *v17; // edx
  __m64 v18; // mm1
  __m64 v19; // mm1
  __m64 v20; // mm2
  unsigned int *v21; // [esp-8h] [ebp-438h]
  unsigned int *v22; // [esp-8h] [ebp-438h]
  unsigned int *v23; // [esp-4h] [ebp-434h]
  unsigned int *v24; // [esp-4h] [ebp-434h]
  unsigned int *v25; // [esp+Ch] [ebp-424h]
  unsigned int *v26; // [esp+10h] [ebp-420h]
  unsigned int *v27; // [esp+14h] [ebp-41Ch]
  unsigned int *v28; // [esp+18h] [ebp-418h]
  unsigned int *v29; // [esp+1Ch] [ebp-414h]
  unsigned int *v30; // [esp+20h] [ebp-410h]
  int *v31; // [esp+24h] [ebp-40Ch]
  _DWORD v32[257]; // [esp+28h] [ebp-408h] BYREF

  v29 = *v2;
  v26 = *a1;
  v25 = v2[1];
  v28 = a1[1];
  v30 = a1[2];
  v27 = a1[3];
  if ( a2 >= 0x100 )
  {
    v31 = dword_50A8D0;
  }
  else
  {
    v3 = 0;
    v4 = 0;
    do
    {
      v5 = 0x100010001LL * (v4 >> 8);
      v32[2 * v3] = v5;
      v32[2 * v3++ + 1] = HIDWORD(v5);
      v4 += a2;
    }
    while ( v3 < 128 );
    v31 = v32;
  }
  if ( !dword_565AFC || sub_407AE0() )
  {
    v14 = v27;
    v15 = v29;
    v16 = v26;
    do
    {
      v24 = v16;
      v22 = v15;
      v17 = v30;
      do
      {
        result = *v16;
        if ( (*v16 & 0xFE000000) != 0 )
        {
          v18 = _mm_cvtsi32_si64(result);
          result >>= 25;
          v19 = _m_psrawi(_m_pmullw(_m_punpcklbw(v18, 0LL), *(__m64 *)&v31[2 * result]), 7u);
          v20 = _m_punpcklbw(_mm_cvtsi32_si64(*v15), 0LL);
          *v15 = _mm_cvtsi64_si32(_m_packuswb(_m_psubsw(_m_paddw(v20, v19), _m_pmulhw(_m_psllwi(v20, 4u), _m_psllwi(v19, 4u))), 0LL));
        }
        ++v15;
        ++v16;
        v17 = (unsigned int *)((char *)v17 - 1);
      }
      while ( v17 );
      v15 = (unsigned int *)((char *)v22 + (_DWORD)v25);
      v16 = (unsigned int *)((char *)v24 + (_DWORD)v28);
      v14 = (unsigned int *)((char *)v14 - 1);
    }
    while ( v14 );
  }
  else
  {
    v6 = v27;
    v7 = v29;
    v8 = v26;
    do
    {
      v23 = v8;
      v21 = v7;
      v9 = v30;
      do
      {
        _mm_prefetch((const char *)v8 + 256, 0);
        result = *v8;
        if ( (*v8 & 0xFE000000) != 0 )
        {
          _mm_prefetch((const char *)v7 + 128, 0);
          v11 = _mm_cvtsi32_si64(result);
          result >>= 25;
          v12 = _m_psrawi(_m_pmullw(_m_punpcklbw(v11, 0LL), *(__m64 *)&v31[2 * result]), 7u);
          v13 = _m_punpcklbw(_mm_cvtsi32_si64(*v7), 0LL);
          *v7 = _mm_cvtsi64_si32(_m_packuswb(_m_psubsw(_m_paddw(v13, v12), _m_pmulhw(_m_psllwi(v13, 4u), _m_psllwi(v12, 4u))), 0LL));
        }
        ++v7;
        ++v8;
        v9 = (unsigned int *)((char *)v9 - 1);
      }
      while ( v9 );
      v7 = (unsigned int *)((char *)v21 + (_DWORD)v25);
      v8 = (unsigned int *)((char *)v23 + (_DWORD)v28);
      v6 = (unsigned int *)((char *)v6 - 1);
    }
    while ( v6 );
  }
  _m_empty();
  return result;
}

// ===== sub_414F00 @ 0x00414F00..0x00414F42 =====
int __usercall sub_414F00@<eax>(int result@<eax>, int a2@<ecx>, unsigned int a3@<esi>)
{
  if ( a3 )
  {
    if ( *(_DWORD *)(result + 16) == 1 )
    {
      if ( *(_DWORD *)(a2 + 16) == 1 )
      {
        if ( a3 >= 0x100 )
          return sub_414F50(a2);
        else
          return sub_415000(a2);
      }
    }
    else if ( *(_DWORD *)(result + 16) == 2 && *(_DWORD *)(a2 + 16) == 1 )
    {
      return sub_4150E0(result, a3);
    }
  }
  return result;
}

// ===== sub_414F50 @ 0x00414F50..0x00414FFA =====
int __usercall sub_414F50@<eax>(int result@<eax>, unsigned int **a2@<ecx>)
{
  unsigned int *v2; // edx
  __m64 v3; // mm6
  int v4; // ecx
  unsigned int *v5; // edi
  unsigned int *v6; // esi
  int v7; // edx
  __m64 v8; // mm1
  __m64 v9; // mm4
  __m64 v10; // mm2
  unsigned int *v11; // [esp-8h] [ebp-28h]
  unsigned int *v12; // [esp-4h] [ebp-24h]
  int v13; // [esp+14h] [ebp-Ch]
  unsigned int *v14; // [esp+18h] [ebp-8h]
  int v15; // [esp+1Ch] [ebp-4h]

  v2 = *a2;
  v14 = a2[1];
  v15 = *(_DWORD *)(result + 4);
  v13 = *(_DWORD *)(result + 8);
  v3 = _m_psrlwi((__m64)-1LL, 9u);
  v4 = *(_DWORD *)(result + 12);
  v5 = v2;
  v6 = *(unsigned int **)result;
  do
  {
    v12 = v6;
    v11 = v5;
    v7 = v13;
    do
    {
      v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v5), 0LL);
      v9 = _m_pcmpgtw(v8, v3);
      v10 = _m_psllwi(_m_psrlwi(v9, 0xFu), 8u);
      *v5++ = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psubw(
                      _m_pxor(
                        _m_pmulhw(
                          _m_psllwi(
                            _m_paddw(_m_psubw(_m_pxor(_m_punpcklbw(_mm_cvtsi32_si64(*v6++), 0LL), v9), v9), v10),
                            5u),
                          _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v8, v9), v9), v10), 4u)),
                        v9),
                      v9),
                    _m_paddw(v10, v9)),
                  0LL));
      --v7;
    }
    while ( v7 );
    v5 = (unsigned int *)((char *)v11 + (_DWORD)v14);
    v6 = (unsigned int *)((char *)v12 + v15);
    --v4;
  }
  while ( v4 );
  _m_empty();
  return result;
}

// ===== sub_415000 @ 0x00415000..0x004150DD =====
int __usercall sub_415000@<eax>(int a1@<eax>, unsigned int a2@<ecx>, unsigned int **a3)
{
  unsigned int *v3; // esi
  unsigned int *v4; // esi
  int result; // eax
  __int64 v6; // mm5
  __m64 v7; // mm6
  int v8; // ecx
  unsigned int *v9; // edi
  int v10; // edx
  __m64 v11; // mm3
  __m64 v12; // mm4
  __m64 v13; // mm2
  unsigned int *v14; // [esp-8h] [ebp-30h]
  unsigned int *v15; // [esp-4h] [ebp-2Ch]
  int v16; // [esp+14h] [ebp-14h]
  unsigned int *v17; // [esp+18h] [ebp-10h]
  int v18; // [esp+20h] [ebp-8h]
  unsigned int *v19; // [esp+24h] [ebp-4h]
  int v20; // [esp+30h] [ebp+8h]

  v3 = *a3;
  v19 = a3[1];
  v20 = *(_DWORD *)(a1 + 4);
  v17 = v3;
  v4 = *(unsigned int **)a1;
  v16 = *(_DWORD *)(a1 + 12);
  v18 = *(_DWORD *)(a1 + 8);
  result = (a2 >> 1) - 2147450880 * (a2 & 0x1FE);
  v6 = (a2 >> 1) + 2147516416LL * (a2 & 0x1FE);
  v7 = _m_psrlwi((__m64)-1LL, 9u);
  v8 = v16;
  v9 = v17;
  do
  {
    v15 = v4;
    v14 = v9;
    v10 = v18;
    do
    {
      v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v9), 0LL);
      v12 = _m_pcmpgtw(v11, v7);
      v13 = _m_psllwi(_m_psrlwi(v12, 0xFu), 8u);
      *v9++ = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psrawi(
                      _m_pmullw(
                        _m_psubw(
                          _m_paddw(
                            _m_psubw(
                              _m_pxor(
                                _m_pmulhw(
                                  _m_psllwi(
                                    _m_paddw(
                                      _m_psubw(_m_pxor(_m_punpcklbw(_mm_cvtsi32_si64(*v4++), 0LL), v12), v12),
                                      v13),
                                    5u),
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v11, v12), v12), v13), 4u)),
                                v12),
                              v12),
                            _m_paddw(v13, v12)),
                          v11),
                        (__m64)v6),
                      7u),
                    v11),
                  0LL));
      --v10;
    }
    while ( v10 );
    v9 = (unsigned int *)((char *)v14 + (_DWORD)v19);
    v4 = (unsigned int *)((char *)v15 + v20);
    --v8;
  }
  while ( v8 );
  _m_empty();
  return result;
}

// ===== sub_4150E0 @ 0x004150E0..0x00415249 =====
unsigned int __cdecl sub_4150E0(unsigned int **a1, unsigned int a2)
{
  unsigned int **v2; // ecx
  int v3; // esi
  unsigned int v4; // edi
  __int64 v5; // rax
  __m64 v6; // mm6
  unsigned int *v7; // ecx
  unsigned int *v8; // edi
  unsigned int *v9; // esi
  unsigned int *v10; // edx
  unsigned int result; // eax
  __m64 v12; // mm0
  __m64 v13; // mm3
  __m64 v14; // mm4
  __m64 v15; // mm2
  unsigned int *v16; // [esp-8h] [ebp-438h]
  unsigned int *v17; // [esp-4h] [ebp-434h]
  unsigned int *v18; // [esp+Ch] [ebp-424h]
  unsigned int *v19; // [esp+10h] [ebp-420h]
  unsigned int *v20; // [esp+14h] [ebp-41Ch]
  unsigned int *v21; // [esp+18h] [ebp-418h]
  unsigned int *v22; // [esp+1Ch] [ebp-414h]
  unsigned int *v23; // [esp+20h] [ebp-410h]
  int *v24; // [esp+24h] [ebp-40Ch]
  _DWORD v25[257]; // [esp+28h] [ebp-408h] BYREF

  v22 = *v2;
  v19 = *a1;
  v18 = v2[1];
  v21 = a1[1];
  v23 = a1[2];
  v20 = a1[3];
  if ( a2 >= 0x100 )
  {
    v24 = dword_50A8D0;
  }
  else
  {
    v3 = 0;
    v4 = 0;
    do
    {
      v5 = 0x100010001LL * (v4 >> 8);
      v25[2 * v3] = v5;
      v25[2 * v3++ + 1] = HIDWORD(v5);
      v4 += a2;
    }
    while ( v3 < 128 );
    v24 = v25;
  }
  v6 = _m_psrlwi((__m64)-1LL, 9u);
  v7 = v20;
  v8 = v22;
  v9 = v19;
  do
  {
    v17 = v9;
    v16 = v8;
    v10 = v23;
    do
    {
      result = *v9;
      if ( (*v9 & 0xFE000000) != 0 )
      {
        v12 = _m_punpcklbw(_mm_cvtsi32_si64(result), 0LL);
        v13 = _m_punpcklbw(_mm_cvtsi32_si64(*v8), 0LL);
        v14 = _m_pcmpgtw(v13, v6);
        v15 = _m_psllwi(_m_psrlwi(v14, 0xFu), 8u);
        result >>= 25;
        *v8 = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psrawi(
                      _m_pmullw(
                        _m_psubw(
                          _m_paddw(
                            _m_psubw(
                              _m_pxor(
                                _m_pmulhw(
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v12, v14), v14), v15), 5u),
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v13, v14), v14), v15), 4u)),
                                v14),
                              v14),
                            _m_paddw(v15, v14)),
                          v13),
                        *(__m64 *)&v24[2 * result]),
                      7u),
                    v13),
                  0LL));
      }
      ++v8;
      ++v9;
      v10 = (unsigned int *)((char *)v10 - 1);
    }
    while ( v10 );
    v8 = (unsigned int *)((char *)v16 + (_DWORD)v18);
    v9 = (unsigned int *)((char *)v17 + (_DWORD)v21);
    v7 = (unsigned int *)((char *)v7 - 1);
  }
  while ( v7 );
  _m_empty();
  return result;
}

// ===== sub_415250 @ 0x00415250..0x00415292 =====
int __usercall sub_415250@<eax>(int result@<eax>, int a2@<ecx>, unsigned int a3@<esi>)
{
  if ( a3 )
  {
    if ( *(_DWORD *)(result + 16) == 1 )
    {
      if ( *(_DWORD *)(a2 + 16) == 1 )
      {
        if ( a3 >= 0x100 )
          return sub_4152A0(a2);
        else
          return sub_415350(a2);
      }
    }
    else if ( *(_DWORD *)(result + 16) == 2 && *(_DWORD *)(a2 + 16) == 1 )
    {
      return sub_415430(result, a3);
    }
  }
  return result;
}

// ===== sub_4152A0 @ 0x004152A0..0x0041534A =====
int __usercall sub_4152A0@<eax>(int result@<eax>, unsigned int **a2@<ecx>)
{
  unsigned int *v2; // edx
  __m64 v3; // mm6
  int v4; // ecx
  unsigned int *v5; // edi
  unsigned int *v6; // esi
  int v7; // edx
  __m64 v8; // mm0
  __m64 v9; // mm4
  __m64 v10; // mm2
  unsigned int *v11; // [esp-8h] [ebp-28h]
  unsigned int *v12; // [esp-4h] [ebp-24h]
  int v13; // [esp+14h] [ebp-Ch]
  unsigned int *v14; // [esp+18h] [ebp-8h]
  int v15; // [esp+1Ch] [ebp-4h]

  v2 = *a2;
  v14 = a2[1];
  v15 = *(_DWORD *)(result + 4);
  v13 = *(_DWORD *)(result + 8);
  v3 = _m_psrlwi((__m64)-1LL, 9u);
  v4 = *(_DWORD *)(result + 12);
  v5 = v2;
  v6 = *(unsigned int **)result;
  do
  {
    v12 = v6;
    v11 = v5;
    v7 = v13;
    do
    {
      v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v6), 0LL);
      v9 = _m_pcmpgtw(v8, v3);
      v10 = _m_psllwi(_m_psrlwi(v9, 0xFu), 8u);
      *v5 = _mm_cvtsi64_si32(
              _m_packuswb(
                _m_paddw(
                  _m_psubw(
                    _m_pxor(
                      _m_pmulhw(
                        _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v8, v9), v9), v10), 5u),
                        _m_psllwi(
                          _m_paddw(_m_psubw(_m_pxor(_m_punpcklbw(_mm_cvtsi32_si64(*v5), 0LL), v9), v9), v10),
                          4u)),
                      v9),
                    v9),
                  _m_paddw(v10, v9)),
                0LL));
      ++v5;
      ++v6;
      --v7;
    }
    while ( v7 );
    v5 = (unsigned int *)((char *)v11 + (_DWORD)v14);
    v6 = (unsigned int *)((char *)v12 + v15);
    --v4;
  }
  while ( v4 );
  _m_empty();
  return result;
}

// ===== sub_415350 @ 0x00415350..0x0041542D =====
int __usercall sub_415350@<eax>(int a1@<eax>, unsigned int a2@<ecx>, unsigned int **a3)
{
  unsigned int *v3; // esi
  unsigned int *v4; // esi
  int result; // eax
  __int64 v6; // mm5
  __m64 v7; // mm6
  int v8; // ecx
  unsigned int *v9; // edi
  int v10; // edx
  __m64 v11; // mm0
  __m64 v12; // mm3
  __m64 v13; // mm4
  __m64 v14; // mm2
  unsigned int *v15; // [esp-8h] [ebp-30h]
  unsigned int *v16; // [esp-4h] [ebp-2Ch]
  int v17; // [esp+14h] [ebp-14h]
  unsigned int *v18; // [esp+18h] [ebp-10h]
  int v19; // [esp+20h] [ebp-8h]
  unsigned int *v20; // [esp+24h] [ebp-4h]
  int v21; // [esp+30h] [ebp+8h]

  v3 = *a3;
  v20 = a3[1];
  v21 = *(_DWORD *)(a1 + 4);
  v18 = v3;
  v4 = *(unsigned int **)a1;
  v17 = *(_DWORD *)(a1 + 12);
  v19 = *(_DWORD *)(a1 + 8);
  result = (a2 >> 1) - 2147450880 * (a2 & 0x1FE);
  v6 = (a2 >> 1) + 2147516416LL * (a2 & 0x1FE);
  v7 = _m_psrlwi((__m64)-1LL, 9u);
  v8 = v17;
  v9 = v18;
  do
  {
    v16 = v4;
    v15 = v9;
    v10 = v19;
    do
    {
      v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), 0LL);
      v12 = _m_punpcklbw(_mm_cvtsi32_si64(*v9), 0LL);
      v13 = _m_pcmpgtw(v11, v7);
      v14 = _m_psllwi(_m_psrlwi(v13, 0xFu), 8u);
      *v9++ = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psrawi(
                      _m_pmullw(
                        _m_psubw(
                          _m_paddw(
                            _m_psubw(
                              _m_pxor(
                                _m_pmulhw(
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v11, v13), v13), v14), 5u),
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v12, v13), v13), v14), 4u)),
                                v13),
                              v13),
                            _m_paddw(v14, v13)),
                          v12),
                        (__m64)v6),
                      7u),
                    v12),
                  0LL));
      ++v4;
      --v10;
    }
    while ( v10 );
    v9 = (unsigned int *)((char *)v15 + (_DWORD)v20);
    v4 = (unsigned int *)((char *)v16 + v21);
    --v8;
  }
  while ( v8 );
  _m_empty();
  return result;
}

// ===== sub_415430 @ 0x00415430..0x00415599 =====
unsigned int __cdecl sub_415430(unsigned int **a1, unsigned int a2)
{
  unsigned int **v2; // ecx
  int v3; // esi
  unsigned int v4; // edi
  __int64 v5; // rax
  __m64 v6; // mm6
  unsigned int *v7; // ecx
  unsigned int *v8; // edi
  unsigned int *v9; // esi
  unsigned int *v10; // edx
  unsigned int result; // eax
  __m64 v12; // mm0
  __m64 v13; // mm3
  __m64 v14; // mm4
  __m64 v15; // mm2
  unsigned int *v16; // [esp-8h] [ebp-438h]
  unsigned int *v17; // [esp-4h] [ebp-434h]
  unsigned int *v18; // [esp+Ch] [ebp-424h]
  unsigned int *v19; // [esp+10h] [ebp-420h]
  unsigned int *v20; // [esp+14h] [ebp-41Ch]
  unsigned int *v21; // [esp+18h] [ebp-418h]
  unsigned int *v22; // [esp+1Ch] [ebp-414h]
  unsigned int *v23; // [esp+20h] [ebp-410h]
  int *v24; // [esp+24h] [ebp-40Ch]
  _DWORD v25[257]; // [esp+28h] [ebp-408h] BYREF

  v22 = *v2;
  v19 = *a1;
  v18 = v2[1];
  v21 = a1[1];
  v23 = a1[2];
  v20 = a1[3];
  if ( a2 >= 0x100 )
  {
    v24 = dword_50A8D0;
  }
  else
  {
    v3 = 0;
    v4 = 0;
    do
    {
      v5 = 0x100010001LL * (v4 >> 8);
      v25[2 * v3] = v5;
      v25[2 * v3++ + 1] = HIDWORD(v5);
      v4 += a2;
    }
    while ( v3 < 128 );
    v24 = v25;
  }
  v6 = _m_psrlwi((__m64)-1LL, 9u);
  v7 = v20;
  v8 = v22;
  v9 = v19;
  do
  {
    v17 = v9;
    v16 = v8;
    v10 = v23;
    do
    {
      result = *v9;
      if ( (*v9 & 0xFE000000) != 0 )
      {
        v12 = _m_punpcklbw(_mm_cvtsi32_si64(result), 0LL);
        v13 = _m_punpcklbw(_mm_cvtsi32_si64(*v8), 0LL);
        v14 = _m_pcmpgtw(v12, v6);
        v15 = _m_psllwi(_m_psrlwi(v14, 0xFu), 8u);
        result >>= 25;
        *v8 = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psrawi(
                      _m_pmullw(
                        _m_psubw(
                          _m_paddw(
                            _m_psubw(
                              _m_pxor(
                                _m_pmulhw(
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v12, v14), v14), v15), 5u),
                                  _m_psllwi(_m_paddw(_m_psubw(_m_pxor(v13, v14), v14), v15), 4u)),
                                v14),
                              v14),
                            _m_paddw(v15, v14)),
                          v13),
                        *(__m64 *)&v24[2 * result]),
                      7u),
                    v13),
                  0LL));
      }
      ++v8;
      ++v9;
      v10 = (unsigned int *)((char *)v10 - 1);
    }
    while ( v10 );
    v8 = (unsigned int *)((char *)v16 + (_DWORD)v18);
    v9 = (unsigned int *)((char *)v17 + (_DWORD)v21);
    v7 = (unsigned int *)((char *)v7 - 1);
  }
  while ( v7 );
  _m_empty();
  return result;
}

// ===== sub_4155A0 @ 0x004155A0..0x00415625 =====
int __usercall sub_4155A0@<eax>(int a1@<eax>, int a2@<edx>, int a3)
{
  if ( *(_DWORD *)(a1 + 16) == 2 )
    return sub_415B30(0);
  if ( *(_DWORD *)(a1 + 16) != 3 )
    return 7;
  if ( *(_DWORD *)(a3 + 16) != 2 )
  {
    if ( *(_DWORD *)(a3 + 16) != 3 )
      return 10;
    if ( *(_DWORD *)(a2 + 16) == 3 )
    {
      sub_415630(a3);
      return 0;
    }
    return 9;
  }
  if ( *(_DWORD *)(a2 + 16) == 1 )
  {
    sub_4156F0(a3);
    return 0;
  }
  else
  {
    if ( *(_DWORD *)(a2 + 16) != 2 )
      return 9;
    sub_415790(a3);
    return 0;
  }
}

// ===== sub_415630 @ 0x00415630..0x004156EB =====
unsigned int __usercall sub_415630@<eax>(int *a1@<eax>, int *a2@<ecx>, int *a3)
{
  int v3; // ebx
  unsigned int v4; // edx
  unsigned int v5; // ecx
  unsigned int result; // eax
  unsigned int v7; // edi
  int v8; // eax
  int v9; // ecx
  unsigned __int8 v10; // dl
  unsigned __int8 v11; // al
  int v12; // eax
  int v13; // [esp+8h] [ebp-1Ch]
  int v14; // [esp+Ch] [ebp-18h]
  int v15; // [esp+10h] [ebp-14h]
  int v16; // [esp+14h] [ebp-10h]
  int v17; // [esp+1Ch] [ebp-8h]
  int v18; // [esp+20h] [ebp-4h]
  unsigned int v19; // [esp+2Ch] [ebp+8h]

  v3 = *a2;
  v15 = a3[1];
  v18 = *a3;
  v14 = a2[1];
  v17 = *a1;
  v13 = a1[1];
  v4 = a2[2];
  if ( v4 > a1[2] )
  {
    v19 = a1[2];
    v4 = v19;
  }
  else
  {
    v19 = a2[2];
  }
  v5 = a2[3];
  result = a1[3];
  if ( v5 <= result )
    result = v5;
  for ( ; result; v17 += v13 )
  {
    v16 = --result;
    v7 = v4;
    if ( v4 )
    {
      v8 = v17 - v3;
      v9 = v4 + v3;
      while ( 1 )
      {
        v10 = *(_BYTE *)--v9;
        --v7;
        if ( v10 && (v11 = *(_BYTE *)(v8 + v9)) != 0 )
          v12 = (v10 * (v11 + 1)) >> 8;
        else
          LOBYTE(v12) = 0;
        *(_BYTE *)(v18 - v3 + v9) = v12;
        if ( !v7 )
          break;
        v8 = v17 - v3;
      }
      v4 = v19;
      result = v16;
    }
    v18 += v15;
    v3 += v14;
  }
  return result;
}

// ===== sub_4156F0 @ 0x004156F0..0x00415781 =====
int __usercall sub_4156F0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>, unsigned int **a3)
{
  unsigned int v3; // edx
  unsigned int v4; // eax
  unsigned int v5; // ecx
  unsigned int *v6; // edi
  _DWORD *v7; // esi
  _BYTE *v8; // ebx
  unsigned int v9; // ecx
  int v10; // eax
  int result; // eax
  _BYTE *v12; // [esp-10h] [ebp-38h]
  _DWORD *v13; // [esp-Ch] [ebp-34h]
  unsigned int *v14; // [esp-8h] [ebp-30h]
  unsigned int v15; // [esp-4h] [ebp-2Ch]
  unsigned int *v16; // [esp+Ch] [ebp-1Ch]
  _DWORD *v17; // [esp+10h] [ebp-18h]
  _BYTE *v18; // [esp+14h] [ebp-14h]
  int v19; // [esp+18h] [ebp-10h]
  int v20; // [esp+1Ch] [ebp-Ch]
  unsigned int *v21; // [esp+20h] [ebp-8h]
  unsigned int v22; // [esp+24h] [ebp-4h]
  unsigned int v23; // [esp+30h] [ebp+8h]

  v16 = *a3;
  v21 = a3[1];
  v17 = (_DWORD *)*a2;
  v20 = a2[1];
  v18 = (_BYTE *)*a1;
  v19 = a1[1];
  v3 = a2[2];
  v23 = v3;
  if ( v3 > a1[2] )
    v23 = a1[2];
  v4 = a1[3];
  v22 = a2[3];
  if ( v22 > v4 )
    v22 = v4;
  v5 = v22;
  v6 = v16;
  v7 = v17;
  v8 = v18;
  do
  {
    v15 = v5;
    v14 = v6;
    v13 = v7;
    v12 = v8;
    v9 = v23;
    do
    {
      v10 = *v7;
      LOBYTE(v3) = *v8;
      ++v7;
      ++v8;
      result = v10 & 0xFFFFFF;
      v3 = result | (v3 << 24);
      *v6++ = v3;
      --v9;
    }
    while ( v9 );
    v8 = &v12[v19];
    v7 = (_DWORD *)((char *)v13 + v20);
    v6 = (unsigned int *)((char *)v14 + (_DWORD)v21);
    v5 = v15 - 1;
  }
  while ( v15 != 1 );
  return result;
}

// ===== sub_415790 @ 0x00415790..0x004158BD =====
__int16 __usercall sub_415790@<ax>(unsigned __int8 **a1@<eax>, __m64 **a2@<ecx>, __m64 **a3)
{
  unsigned int v3; // edx
  unsigned int v4; // eax
  unsigned int v5; // eax
  __m64 *v6; // ecx
  __m64 *v7; // edi
  __m64 *v8; // esi
  unsigned __int8 *v9; // ebx
  unsigned int v10; // ecx
  __m64 m64_u64; // mm0
  __m64 *v12; // ecx
  unsigned int *v13; // edi
  __m64 *v14; // esi
  unsigned __int8 *v15; // ebx
  unsigned int v16; // ecx
  int v17; // eax
  int v18; // edx
  int v19; // eax
  unsigned __int8 *v21; // [esp-10h] [ebp-4Ch]
  unsigned __int8 *v22; // [esp-10h] [ebp-4Ch]
  __m64 *v23; // [esp-Ch] [ebp-48h]
  __m64 *v24; // [esp-Ch] [ebp-48h]
  __m64 *v25; // [esp-8h] [ebp-44h]
  unsigned int *v26; // [esp-8h] [ebp-44h]
  __m64 *v27; // [esp-4h] [ebp-40h]
  __m64 *v28; // [esp-4h] [ebp-40h]
  __m64 *v29; // [esp+20h] [ebp-1Ch]
  __m64 *v30; // [esp+24h] [ebp-18h]
  unsigned __int8 *v31; // [esp+28h] [ebp-14h]
  unsigned __int8 *v32; // [esp+2Ch] [ebp-10h]
  __m64 *v33; // [esp+30h] [ebp-Ch]
  __m64 *v34; // [esp+34h] [ebp-8h]
  __m64 *v35; // [esp+38h] [ebp-4h]
  unsigned int v36; // [esp+44h] [ebp+8h]

  v29 = *a3;
  v34 = a3[1];
  v30 = *a2;
  v33 = a2[1];
  v31 = *a1;
  v32 = a1[1];
  v3 = (unsigned int)a2[2];
  if ( v3 > (unsigned int)a1[2] )
    v3 = (unsigned int)a1[2];
  v4 = (unsigned int)a1[3];
  v36 = v3;
  v35 = a2[3];
  if ( (unsigned int)v35 > v4 )
    v35 = (__m64 *)v4;
  if ( (v3 & 1) != 0 )
  {
    v12 = v35;
    v13 = (unsigned int *)v29;
    v14 = v30;
    v15 = v31;
    do
    {
      v28 = v12;
      v26 = v13;
      v24 = v14;
      v22 = v15;
      v16 = v36;
      do
      {
        v17 = v14->m64_i32[0];
        v14 = (__m64 *)((char *)v14 + 4);
        v18 = v17 & 0xFFFFFF;
        v19 = (unsigned __int16)(*v15++ * HIBYTE(v17));
        v5 = v18 | (v19 << 16) & 0xFF000000;
        *v13++ = v5;
        --v16;
      }
      while ( v16 );
      v15 = &v22[(_DWORD)v32];
      v14 = (__m64 *)((char *)v24 + (_DWORD)v33);
      v13 = (unsigned int *)((char *)v26 + (_DWORD)v34);
      v12 = (__m64 *)((char *)v28 - 1);
    }
    while ( v28 != (__m64 *)1 );
  }
  else
  {
    HIWORD(v5) = -256;
    v6 = v35;
    v7 = v29;
    v8 = v30;
    v9 = v31;
    do
    {
      v27 = v6;
      v25 = v7;
      v23 = v8;
      v21 = v9;
      v10 = v3 >> 1;
      do
      {
        m64_u64 = (__m64)v8->m64_u64;
        ++v8;
        LOWORD(v5) = *(_WORD *)v9;
        v9 += 2;
        v7->m64_u64 = (unsigned __int64)_m_por(
                                          _m_pand(
                                            _m_pslldi(
                                              _m_pmaddwd(
                                                _m_psrldi(m64_u64, 0x18u),
                                                _m_punpcklwd(_m_punpcklbw(_mm_cvtsi32_si64(v5), 0LL), 0LL)),
                                              0x10u),
                                            (__m64)0xFF000000FF000000uLL),
                                          _m_pand(m64_u64, (__m64)0xFFFFFF00FFFFFFLL));
        ++v7;
        --v10;
      }
      while ( v10 );
      v9 = &v21[(_DWORD)v32];
      v8 = (__m64 *)((char *)v23 + (_DWORD)v33);
      v7 = (__m64 *)((char *)v25 + (_DWORD)v34);
      v6 = (__m64 *)((char *)v27 - 1);
    }
    while ( v27 != (__m64 *)1 );
    _m_empty();
  }
  return v5;
}

// ===== sub_4158C0 @ 0x004158C0..0x004158FC =====
int __usercall sub_4158C0@<eax>(int result@<eax>, int a2@<ecx>, unsigned int a3@<esi>)
{
  if ( a3 && *(_DWORD *)(result + 16) == 2 )
  {
    if ( *(_DWORD *)(a2 + 16) == 1 )
    {
      return sub_415900(result, a3);
    }
    else if ( *(_DWORD *)(a2 + 16) == 2 )
    {
      if ( a3 >= 0x100 )
        return sub_415AB0(a2);
      else
        return sub_415A30(a3);
    }
  }
  return result;
}

// ===== sub_415900 @ 0x00415900..0x00415A22 =====
unsigned int __cdecl sub_415900(int *a1, int a2)
{
  int *v2; // ecx
  int v3; // esi
  unsigned int v4; // eax
  int v5; // ecx
  __m64 v6; // mm7
  int v7; // edx
  int v8; // edi
  int v9; // esi
  int v10; // ebx
  int v11; // ecx
  unsigned int result; // eax
  int v13; // [esp+Ch] [ebp-824h]
  int v14; // [esp+10h] [ebp-820h]
  unsigned int v15; // [esp+14h] [ebp-81Ch]
  int v16; // [esp+18h] [ebp-818h]
  int v17; // [esp+1Ch] [ebp-814h]
  int v18; // [esp+20h] [ebp-810h]
  int v19; // [esp+24h] [ebp-80Ch]
  __m64 v20[256]; // [esp+28h] [ebp-808h]

  v14 = *v2;
  v18 = *a1;
  v13 = v2[1];
  v16 = a1[1];
  v3 = 0;
  v4 = 0;
  v15 = a1[2];
  v17 = a1[3];
  v19 = 0;
  do
  {
    v5 = 256 - (v4 >> 8);
    v4 = a2 + v19;
    v20[v3].m64_i32[0] = 1048592 * v5;
    v20[v3++].m64_i32[1] = (unsigned __int64)(0x1000100000LL * v5 + 16 * v5) >> 32;
    v19 += a2;
  }
  while ( v3 < 256 );
  v6 = _mm_cvtsi32_si64(v15);
  v7 = v17;
  v8 = v14;
  v9 = v18;
  do
  {
    v10 = 0;
    v11 = _mm_cvtsi64_si32(v6);
    do
    {
      result = *(_DWORD *)(v9 + v10);
      if ( (result & 0xFF000000) != 0 )
      {
        result >>= 24;
        *(_DWORD *)(v8 + v10) = _mm_cvtsi64_si32(
                                  _m_packuswb(
                                    _m_pmulhw(
                                      _m_psllwi(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + v10)), 0LL), 4u),
                                      v20[result]),
                                    0LL));
      }
      v10 += 4;
      --v11;
    }
    while ( v11 );
    v8 += v13;
    v9 += v16;
    --v7;
  }
  while ( v7 );
  _m_empty();
  return result;
}

// ===== sub_415A30 @ 0x00415A30..0x00415AAB =====
int __usercall sub_415A30@<eax>(int *a1@<eax>, int *a2@<ecx>, int a3)
{
  int v3; // edx
  int v4; // ecx
  int v5; // edi
  int v6; // esi
  int v7; // ecx
  __int16 v8; // bx
  int v9; // edx
  int result; // eax
  unsigned int v11; // edx
  int v12; // [esp-Ch] [ebp-30h]
  int v13; // [esp-8h] [ebp-2Ch]
  int v14; // [esp-4h] [ebp-28h]
  int v15; // [esp+18h] [ebp-Ch]
  int v16; // [esp+1Ch] [ebp-8h]
  int v17; // [esp+20h] [ebp-4h]

  v3 = *a2;
  v17 = a2[1];
  v16 = a1[1];
  v15 = a1[2];
  v4 = a1[3];
  v5 = v3;
  v6 = *a1;
  do
  {
    v14 = v4;
    v13 = v5;
    v12 = v6;
    v7 = v15;
    do
    {
      HIBYTE(v8) = 0;
      v9 = *(unsigned __int8 *)(v6 + 3);
      v6 += 4;
      result = 256;
      if ( v9 )
      {
        LOBYTE(v8) = *(_BYTE *)(v5 + 3);
        v11 = (unsigned int)(a3 * v9) >> 8;
        result = 256 - v11;
        *(_BYTE *)(v5 + 3) = (unsigned __int16)((256 - v11) * v8) >> 8;
      }
      v5 += 4;
      --v7;
    }
    while ( v7 );
    v6 = v16 + v12;
    v5 = v17 + v13;
    v4 = v14 - 1;
  }
  while ( v14 != 1 );
  return result;
}

// ===== sub_415AB0 @ 0x00415AB0..0x00415B24 =====
int __usercall sub_415AB0@<eax>(int *a1@<eax>, int *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx
  int v4; // edi
  int v5; // esi
  int v6; // ecx
  __int16 v7; // bx
  int v8; // edx
  int result; // eax
  int v10; // [esp-Ch] [ebp-30h]
  int v11; // [esp-8h] [ebp-2Ch]
  int v12; // [esp-4h] [ebp-28h]
  int v13; // [esp+18h] [ebp-Ch]
  int v14; // [esp+1Ch] [ebp-8h]
  int v15; // [esp+20h] [ebp-4h]

  v2 = *a2;
  v15 = a2[1];
  v14 = a1[1];
  v13 = a1[2];
  v3 = a1[3];
  v4 = v2;
  v5 = *a1;
  do
  {
    v12 = v3;
    v11 = v4;
    v10 = v5;
    v6 = v13;
    do
    {
      HIBYTE(v7) = 0;
      v8 = *(unsigned __int8 *)(v5 + 3);
      v5 += 4;
      result = 256;
      if ( v8 )
      {
        result = 256 - v8;
        LOBYTE(v7) = *(_BYTE *)(v4 + 3);
        *(_BYTE *)(v4 + 3) = (unsigned __int16)((256 - v8) * v7) >> 8;
      }
      v4 += 4;
      --v6;
    }
    while ( v6 );
    v5 = v14 + v10;
    v4 = v15 + v11;
    v3 = v12 - 1;
  }
  while ( v12 != 1 );
  return result;
}

// ===== sub_415B30 @ 0x00415B30..0x00415BA8 =====
int __usercall sub_415B30@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<edi>, int a4)
{
  int v5; // [esp-4h] [ebp-8h]

  if ( *(_DWORD *)(a3 + 16) != 2 )
    return 10;
  if ( *(_DWORD *)(a1 + 16) == 1 )
  {
    if ( *(_DWORD *)(a2 + 16) == 2 )
    {
      sub_415CE0(a3, 256 - a4);
      return 0;
    }
    return 7;
  }
  if ( *(_DWORD *)(a1 + 16) != 2 )
    return 9;
  if ( *(_DWORD *)(a2 + 16) != 2 )
    return 7;
  v5 = 256 - a4;
  if ( a3 == a1 )
    sub_415C60(v5);
  else
    sub_415BB0(a3, v5);
  return 0;
}

// ===== sub_415BB0 @ 0x00415BB0..0x00415C55 =====
int __usercall sub_415BB0@<eax>(_DWORD *a1@<eax>, int *a2@<ecx>, _DWORD *a3)
{
  int *v3; // ebp
  _DWORD *v4; // esi
  int v5; // esi
  int v6; // edx
  int v7; // eax
  int v8; // ecx
  int v9; // ebx
  _DWORD *v10; // esi
  _DWORD *v11; // edi
  int v12; // ecx
  int v13; // ebp
  __int64 v14; // rax
  unsigned int v15; // edx
  int *v17; // [esp-14h] [ebp-3Ch]
  _DWORD *v18; // [esp-10h] [ebp-38h]
  _DWORD *v19; // [esp-Ch] [ebp-34h]
  int v20; // [esp-8h] [ebp-30h]
  int v21; // [esp-4h] [ebp-2Ch]
  _DWORD *v22; // [esp+14h] [ebp-14h]
  _DWORD *v23; // [esp+18h] [ebp-10h]
  int v24; // [esp+1Ch] [ebp-Ch]
  int v25; // [esp+20h] [ebp-8h]
  int v26; // [esp+24h] [ebp-4h]
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  v3 = &savedregs;
  v23 = (_DWORD *)*a3;
  v4 = (_DWORD *)*a1;
  v25 = a3[1];
  v22 = v4;
  v5 = *a2;
  v26 = a1[1];
  v6 = a1[2];
  v7 = a1[3];
  v24 = v6;
  v8 = v7;
  v9 = v5;
  v10 = v22;
  v11 = v23;
  do
  {
    v21 = v8;
    v20 = v9;
    v19 = v10;
    v18 = v11;
    v12 = *(v3 - 3);
    v17 = v3;
    v13 = v3[3];
    do
    {
      v14 = *(unsigned __int8 *)(v9 + 3);
      v9 += 4;
      if ( (_DWORD)v14 )
      {
        HIDWORD(v14) = *v10;
        LODWORD(v14) = v13 * v14;
        if ( (_DWORD)v14 != 65280 )
        {
          v15 = (((_DWORD)v14 * HIBYTE(HIDWORD(v14))) << 8) & 0xFF000000;
          LODWORD(v14) = *v10 & 0xFFFFFF;
          HIDWORD(v14) = v14 | v15;
        }
      }
      ++v10;
      *v11++ = HIDWORD(v14);
      --v12;
    }
    while ( v12 );
    v3 = v17;
    v11 = (_DWORD *)((char *)v18 + *(v17 - 2));
    v10 = (_DWORD *)((char *)v19 + *(v17 - 1));
    v9 = v17[2] + v20;
    v8 = v21 - 1;
  }
  while ( v21 != 1 );
  return v14;
}

// ===== sub_415C60 @ 0x00415C60..0x00415CDA =====
int __usercall sub_415C60@<eax>(int *a1@<eax>, int *a2@<ecx>, int a3)
{
  int v3; // edx
  int v4; // ecx
  int v5; // esi
  int v6; // edi
  int v7; // ecx
  int result; // eax
  unsigned int v9; // edx
  int v10; // [esp-Ch] [ebp-30h]
  int v11; // [esp-8h] [ebp-2Ch]
  int v12; // [esp-4h] [ebp-28h]
  int v13; // [esp+18h] [ebp-Ch]
  int v14; // [esp+1Ch] [ebp-8h]
  int v15; // [esp+20h] [ebp-4h]

  v3 = *a2;
  v14 = a2[1];
  v15 = a1[1];
  v13 = a1[2];
  v4 = a1[3];
  v5 = *a1;
  v6 = v3;
  do
  {
    v12 = v4;
    v11 = v5;
    v10 = v6;
    v7 = v13;
    do
    {
      result = *(unsigned __int8 *)(v5 + 3);
      v9 = 0;
      v5 += 4;
      if ( result )
      {
        result *= a3;
        if ( result == 65280 )
          goto LABEL_7;
        LOBYTE(v9) = *(_BYTE *)(v6 + 3);
        v9 = (result * v9) >> 16;
      }
      *(_BYTE *)(v6 + 3) = v9;
LABEL_7:
      v6 += 4;
      --v7;
    }
    while ( v7 );
    v6 = v14 + v10;
    v5 = v15 + v11;
    v4 = v12 - 1;
  }
  while ( v12 != 1 );
  return result;
}

// ===== sub_415CE0 @ 0x00415CE0..0x00415D83 =====
int __usercall sub_415CE0@<eax>(_DWORD *a1@<eax>, int *a2@<ecx>, _DWORD *a3)
{
  int *v3; // ebp
  _DWORD *v4; // esi
  int v5; // esi
  int v6; // edx
  int v7; // eax
  int v8; // ecx
  int v9; // ebx
  _DWORD *v10; // esi
  _DWORD *v11; // edi
  int v12; // ecx
  int v13; // ebp
  __int64 v14; // rax
  int *v16; // [esp-14h] [ebp-3Ch]
  _DWORD *v17; // [esp-10h] [ebp-38h]
  _DWORD *v18; // [esp-Ch] [ebp-34h]
  int v19; // [esp-8h] [ebp-30h]
  int v20; // [esp-4h] [ebp-2Ch]
  _DWORD *v21; // [esp+14h] [ebp-14h]
  _DWORD *v22; // [esp+18h] [ebp-10h]
  int v23; // [esp+1Ch] [ebp-Ch]
  int v24; // [esp+20h] [ebp-8h]
  int v25; // [esp+24h] [ebp-4h]
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  v3 = &savedregs;
  v22 = (_DWORD *)*a3;
  v4 = (_DWORD *)*a1;
  v24 = a3[1];
  v21 = v4;
  v5 = *a2;
  v25 = a1[1];
  v6 = a1[2];
  v7 = a1[3];
  v23 = v6;
  v8 = v7;
  v9 = v5;
  v10 = v21;
  v11 = v22;
  do
  {
    v20 = v8;
    v19 = v9;
    v18 = v10;
    v17 = v11;
    v12 = *(v3 - 3);
    v16 = v3;
    v13 = v3[3];
    do
    {
      v14 = *(unsigned __int8 *)(v9 + 3);
      v9 += 4;
      if ( (_DWORD)v14 )
      {
        HIDWORD(v14) = *v10++;
        HIDWORD(v14) |= 0xFF000000;
        LODWORD(v14) = v13 * v14;
        if ( (_DWORD)v14 != 65280 )
        {
          LODWORD(v14) = ((_DWORD)v14 << 16) & 0xFF000000;
          HIDWORD(v14) = v14 | HIDWORD(v14) & 0xFFFFFF;
        }
      }
      *v11++ = HIDWORD(v14);
      --v12;
    }
    while ( v12 );
    v3 = v16;
    v11 = (_DWORD *)((char *)v17 + *(v16 - 2));
    v10 = (_DWORD *)((char *)v18 + *(v16 - 1));
    v9 = v16[2] + v19;
    v8 = v20 - 1;
  }
  while ( v20 != 1 );
  return v14;
}

// ===== sub_415D90 @ 0x00415D90..0x00415E00 =====
int __usercall sub_415D90@<eax>(int result@<eax>, int a2@<edx>, int a3, int a4, unsigned int a5, unsigned int a6)
{
  if ( *(_DWORD *)(a2 + 16) == 3 && *(_DWORD *)(a3 + 16) == 1 && a6 < 0x100 )
  {
    if ( a5 >= 0x100 )
    {
      return sub_40B320(result, a6, a3);
    }
    else if ( *(_DWORD *)(result + 16) == 1 )
    {
      return sub_415E00(result, a4, a5, a6);
    }
    else if ( *(_DWORD *)(result + 16) == 2 )
    {
      return sub_415F60(result, a4, a5, a6);
    }
  }
  return result;
}

// ===== sub_415E00 @ 0x00415E00..0x00415F56 =====
unsigned int *__fastcall sub_415E00(
        unsigned int **a1,
        unsigned __int8 **a2,
        unsigned int **a3,
        char a4,
        int a5,
        int a6)
{
  int v6; // esi
  unsigned int v7; // ebx
  __int64 v8; // rax
  unsigned int *result; // eax
  unsigned int *v10; // edi
  unsigned int *v11; // esi
  unsigned __int8 *v12; // ebx
  unsigned int *v13; // eax
  int v14; // edx
  int v15; // ecx
  __m64 v16; // mm1
  unsigned __int8 *v17; // [esp-10h] [ebp-448h]
  unsigned int *v18; // [esp-Ch] [ebp-444h]
  unsigned int *v19; // [esp-8h] [ebp-440h]
  unsigned int *v20; // [esp-4h] [ebp-43Ch]
  unsigned int *v21; // [esp+Ch] [ebp-42Ch]
  unsigned int *v22; // [esp+10h] [ebp-428h]
  unsigned int *v23; // [esp+14h] [ebp-424h]
  unsigned __int8 *v24; // [esp+18h] [ebp-420h]
  unsigned int *v25; // [esp+1Ch] [ebp-41Ch]
  int v26; // [esp+20h] [ebp-418h]
  unsigned int *v27; // [esp+24h] [ebp-414h]
  unsigned __int8 *v28; // [esp+28h] [ebp-410h]
  unsigned int *v29; // [esp+2Ch] [ebp-40Ch]
  __m64 v30[128]; // [esp+30h] [ebp-408h]

  v25 = *a1;
  v27 = *a3;
  v22 = a1[1];
  v21 = a3[1];
  v24 = a2[1];
  v28 = *a2;
  v6 = 0;
  v7 = 0;
  v23 = a3[3];
  v29 = a3[2];
  do
  {
    v8 = 0x100010001LL * (v7 >> 8);
    v30[v6].m64_i32[0] = v8;
    v30[v6++].m64_i32[1] = HIDWORD(v8);
    v7 += 256 - a6;
  }
  while ( v6 < 128 );
  v26 = a5 * ((1 << a4) + 1);
  result = v23;
  v10 = v25;
  v11 = v27;
  v12 = v28;
  do
  {
    v20 = result;
    v19 = v10;
    v18 = v11;
    v17 = v12;
    v13 = v29;
    do
    {
      v14 = *v12++ << a4;
      v15 = v26 - v14;
      if ( v26 > v14 )
      {
        if ( v15 > 255 )
          v15 = 255;
        v16 = _m_punpcklbw(_mm_cvtsi32_si64(*v10), 0LL);
        *v10 = _mm_cvtsi64_si32(
                 _m_packuswb(
                   _m_paddw(
                     _m_psrawi(
                       _m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v11), 0LL), v16), v30[(unsigned int)v15 >> 1]),
                       7u),
                     v16),
                   0LL));
      }
      ++v10;
      ++v11;
      v13 = (unsigned int *)((char *)v13 - 1);
    }
    while ( v13 );
    v12 = &v17[(_DWORD)v24];
    v11 = (unsigned int *)((char *)v18 + (_DWORD)v21);
    v10 = (unsigned int *)((char *)v19 + (_DWORD)v22);
    result = (unsigned int *)((char *)v20 - 1);
  }
  while ( v20 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_415F60 @ 0x00415F60..0x004160C7 =====
unsigned int *__fastcall sub_415F60(
        unsigned int **a1,
        unsigned __int8 **a2,
        unsigned int **a3,
        char a4,
        int a5,
        int a6)
{
  int v6; // esi
  unsigned int v7; // ebx
  __int64 v8; // rax
  unsigned int *result; // eax
  unsigned int *v10; // edi
  unsigned int *v11; // esi
  unsigned __int8 *v12; // ebx
  unsigned int *v13; // eax
  int v14; // edx
  int v15; // ecx
  __m64 v16; // mm1
  unsigned __int8 *v17; // [esp-10h] [ebp-448h]
  unsigned int *v18; // [esp-Ch] [ebp-444h]
  unsigned int *v19; // [esp-8h] [ebp-440h]
  unsigned int *v20; // [esp-4h] [ebp-43Ch]
  unsigned int *v21; // [esp+Ch] [ebp-42Ch]
  unsigned int *v22; // [esp+10h] [ebp-428h]
  unsigned int *v23; // [esp+14h] [ebp-424h]
  unsigned __int8 *v24; // [esp+18h] [ebp-420h]
  unsigned int *v25; // [esp+1Ch] [ebp-41Ch]
  int v26; // [esp+20h] [ebp-418h]
  unsigned int *v27; // [esp+24h] [ebp-414h]
  unsigned __int8 *v28; // [esp+28h] [ebp-410h]
  unsigned int *v29; // [esp+2Ch] [ebp-40Ch]
  __m64 v30[128]; // [esp+30h] [ebp-408h]

  v25 = *a1;
  v27 = *a3;
  v22 = a1[1];
  v21 = a3[1];
  v24 = a2[1];
  v28 = *a2;
  v6 = 0;
  v7 = 0;
  v23 = a3[3];
  v29 = a3[2];
  do
  {
    v8 = 0x100010001LL * (v7 >> 8);
    v30[v6].m64_i32[0] = v8;
    v30[v6++].m64_i32[1] = HIDWORD(v8);
    v7 += 256 - a6;
  }
  while ( v6 < 128 );
  v26 = a5 * ((1 << a4) + 1);
  result = v23;
  v10 = v25;
  v11 = v27;
  v12 = v28;
  do
  {
    v20 = result;
    v19 = v10;
    v18 = v11;
    v17 = v12;
    v13 = v29;
    do
    {
      v14 = *v12++ << a4;
      v15 = v26 - v14;
      if ( v26 > v14 )
      {
        if ( v15 > 256 )
          v15 = 256;
        v16 = _m_punpcklbw(_mm_cvtsi32_si64(*v10), 0LL);
        *v10 = _mm_cvtsi64_si32(
                 _m_packuswb(
                   _m_paddw(
                     _m_psrawi(
                       _m_pmullw(
                         _m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v11), 0LL), v16),
                         v30[(unsigned int)(HIBYTE(*v11) * v15) >> 9]),
                       7u),
                     v16),
                   0LL));
      }
      ++v10;
      ++v11;
      v13 = (unsigned int *)((char *)v13 - 1);
    }
    while ( v13 );
    v12 = &v17[(_DWORD)v24];
    v11 = (unsigned int *)((char *)v18 + (_DWORD)v21);
    v10 = (unsigned int *)((char *)v19 + (_DWORD)v22);
    result = (unsigned int *)((char *)v20 - 1);
  }
  while ( v20 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_4160D0 @ 0x004160D0..0x00416132 =====
int __usercall sub_4160D0@<eax>(int result@<eax>, int a2@<ecx>, unsigned int a3@<edi>, size_t *a4, int a5)
{
  if ( *(_DWORD *)(a2 + 16) == 3 && a4[4] == 2 )
  {
    if ( a3 )
    {
      if ( a3 >= 0x100 )
      {
        return sub_40AF50(result, a4);
      }
      else if ( *(_DWORD *)(result + 16) == 1 )
      {
        return sub_416140(a4, a5, a3);
      }
      else if ( *(_DWORD *)(result + 16) == 2 )
      {
        return sub_4161F0(a4, a5, a3);
      }
    }
    else
    {
      return sub_40A620((int)a4, 0);
    }
  }
  return result;
}

// ===== sub_416140 @ 0x00416140..0x004161F0 =====
int __usercall sub_416140@<eax>(int a1@<eax>, unsigned __int8 **a2@<ecx>, int **a3, char a4, int a5)
{
  int *v5; // esi
  _DWORD *v6; // esi
  int v7; // ecx
  int *v8; // edi
  unsigned __int8 *v9; // ebx
  int v10; // ecx
  int v11; // edx
  int v12; // ecx
  int result; // eax
  int v14; // [esp-14h] [ebp-3Ch]
  unsigned __int8 *v15; // [esp-10h] [ebp-38h]
  _DWORD *v16; // [esp-Ch] [ebp-34h]
  int *v17; // [esp-8h] [ebp-30h]
  int v18; // [esp-4h] [ebp-2Ch]
  int *v19; // [esp+10h] [ebp-18h]
  unsigned __int8 *v20; // [esp+18h] [ebp-10h]
  int v21; // [esp+1Ch] [ebp-Ch]
  unsigned __int8 *v22; // [esp+20h] [ebp-8h]
  int v23; // [esp+24h] [ebp-4h]
  int *v24; // [esp+30h] [ebp+8h]
  int v25; // [esp+38h] [ebp+10h]

  v5 = *a3;
  v24 = a3[1];
  v19 = v5;
  v6 = *(_DWORD **)a1;
  v23 = *(_DWORD *)(a1 + 4);
  v22 = a2[1];
  v20 = *a2;
  v21 = *(_DWORD *)(a1 + 8);
  v25 = a5 * ((1 << a4) + 1);
  v7 = *(_DWORD *)(a1 + 12);
  v8 = v19;
  v9 = v20;
  do
  {
    v18 = v7;
    v17 = v8;
    v16 = v6;
    v15 = v9;
    v10 = v21;
    do
    {
      v14 = v10;
      v11 = *v9 << a4;
      v12 = v25 - v11;
      if ( v25 < v11 )
        v12 = 0;
      result = 255;
      if ( v12 > 255 )
        v12 = 255;
      *v8++ = (v12 << 24) | *v6++ & 0xFFFFFF;
      ++v9;
      v10 = v14 - 1;
    }
    while ( v14 != 1 );
    v9 = &v15[(_DWORD)v22];
    v6 = (_DWORD *)((char *)v16 + v23);
    v8 = (int *)((char *)v17 + (_DWORD)v24);
    v7 = v18 - 1;
  }
  while ( v18 != 1 );
  return result;
}

// ===== sub_4161F0 @ 0x004161F0..0x00416393 =====
unsigned int __usercall sub_4161F0@<eax>(int a1@<eax>, unsigned __int8 **a2@<ecx>, int *a3, char a4, int a5)
{
  __m64 *v5; // esi
  int v6; // edx
  unsigned int v7; // eax
  __m64 v8; // mm0
  __m64 v9; // mm7
  int v10; // edi
  unsigned __int8 *v11; // ebx
  unsigned int v12; // edx
  unsigned int result; // eax
  __m64 v14; // mm2
  __m64 v15; // mm2
  __m64 v16; // mm3
  __m64 v17; // mm0
  int v18; // ecx
  int v19; // edi
  unsigned __int8 *v20; // ebx
  unsigned int v21; // ecx
  int v22; // edx
  int v23; // ecx
  unsigned __int32 v24; // edx
  unsigned int v25; // edx
  unsigned int v26; // [esp-14h] [ebp-58h]
  unsigned __int8 *v27; // [esp-10h] [ebp-54h]
  unsigned __int8 *v28; // [esp-10h] [ebp-54h]
  __m64 *v29; // [esp-Ch] [ebp-50h]
  __m64 *v30; // [esp-Ch] [ebp-50h]
  int v31; // [esp-8h] [ebp-4Ch]
  int v32; // [esp-8h] [ebp-4Ch]
  int v33; // [esp-4h] [ebp-48h]
  int v34; // [esp-4h] [ebp-48h]
  int v35; // [esp+2Ch] [ebp-18h]
  unsigned __int8 *v36; // [esp+34h] [ebp-10h]
  unsigned __int8 *v37; // [esp+38h] [ebp-Ch]
  int v38; // [esp+3Ch] [ebp-8h]
  int v39; // [esp+40h] [ebp-4h]
  unsigned int v40; // [esp+4Ch] [ebp+8h]
  unsigned int v41; // [esp+4Ch] [ebp+8h]
  signed int v42; // [esp+54h] [ebp+10h]

  v39 = a3[1];
  v35 = *a3;
  v5 = *(__m64 **)a1;
  v38 = *(_DWORD *)(a1 + 4);
  v6 = *(_DWORD *)(a1 + 12);
  v7 = *(_DWORD *)(a1 + 8);
  v37 = a2[1];
  v36 = *a2;
  v40 = v7;
  v42 = a5 * ((1 << a4) + 1);
  if ( (v7 & 1) != 0 )
  {
    v18 = v6;
    v19 = v35;
    v20 = v36;
    do
    {
      v34 = v18;
      v32 = v19;
      v30 = v5;
      v28 = v20;
      v21 = v40;
      do
      {
        v26 = v21;
        v22 = *v20 << a4;
        v23 = v42 - v22;
        if ( v42 < v22 )
          v23 = 0;
        if ( v23 > 256 )
          v23 = 256;
        v24 = v5->m64_i32[0];
        v5 = (__m64 *)((char *)v5 + 4);
        ++v20;
        result = v24 & 0xFFFFFF;
        v19 += 4;
        v25 = v24 & 0xFFFFFF | (v23 * (v24 >> 8)) & 0xFF000000;
        v21 = v26 - 1;
        *(_DWORD *)(v19 - 4) = v25;
      }
      while ( v26 != 1 );
      v20 = &v28[(_DWORD)v37];
      v5 = (__m64 *)((char *)v30 + v38);
      v19 = v39 + v32;
      v18 = v34 - 1;
    }
    while ( v34 != 1 );
  }
  else
  {
    v41 = v7 >> 1;
    v8 = _mm_cvtsi32_si64(v42);
    v9 = _m_por(_m_psllqi(v8, 0x20u), v8);
    v10 = v35;
    v11 = *a2;
    do
    {
      v33 = v6;
      v31 = v10;
      v29 = v5;
      v27 = v11;
      v12 = v41;
      do
      {
        result = *v11 << a4;
        v14 = _m_psubd(v9, _m_por(_mm_cvtsi32_si64(result), _m_psllqi(_mm_cvtsi32_si64(v11[1] << a4), 0x20u)));
        v15 = _m_pand(v14, _m_pcmpgtd(v14, 0LL));
        v16 = _m_pcmpgtd((__m64)0x10000000100LL, v15);
        v11 += 2;
        v10 += 8;
        v17 = _m_por(
                _m_pslldi(
                  _m_pmulhw(
                    _m_psrldi(_m_pand((__m64)v5->m64_u64, (__m64)0xFF000000FF000000uLL), 0x14u),
                    _m_pslldi(_m_por(_m_pand(v15, v16), _m_pand(_m_pxor(v16, (__m64)-1LL), (__m64)0x10000000100LL)), 4u)),
                  0x18u),
                _m_pand((__m64)v5->m64_u64, (__m64)0xFFFFFF00FFFFFFLL));
        ++v5;
        *(__m64 *)(v10 - 8) = v17;
        --v12;
      }
      while ( v12 );
      v11 = &v27[(_DWORD)v37];
      v5 = (__m64 *)((char *)v29 + v38);
      v10 = v39 + v31;
      v6 = v33 - 1;
    }
    while ( v33 != 1 );
    _m_empty();
  }
  return result;
}

// ===== sub_4163A0 @ 0x004163A0..0x004163C9 =====
int __usercall sub_4163A0@<eax>(int a1@<edx>, int a2@<ecx>, unsigned int a3@<edi>, int a4@<esi>)
{
  int result; // eax

  if ( *(_DWORD *)(a1 + 16) == 3 )
  {
    result = *(_DWORD *)(a2 + 16) - 1;
    if ( *(_DWORD *)(a2 + 16) == 1 )
    {
      result = *(_DWORD *)(a4 + 16) - 2;
      if ( *(_DWORD *)(a4 + 16) == 2 && a3 < 0x100 )
        return sub_4163D0(a4, a3);
    }
  }
  return result;
}

// ===== sub_4163D0 @ 0x004163D0..0x00416505 =====
int __fastcall sub_4163D0(unsigned int **a1, _DWORD *a2, unsigned int **a3, int a4)
{
  int v4; // esi
  unsigned int v5; // edi
  __int64 v6; // rax
  int result; // eax
  unsigned int *v8; // ecx
  unsigned int *v9; // edi
  unsigned int *v10; // esi
  _BYTE *v11; // ebx
  unsigned int *v12; // ecx
  __m64 v13; // mm1
  _BYTE *v14; // [esp-10h] [ebp-444h]
  unsigned int *v15; // [esp-Ch] [ebp-440h]
  unsigned int *v16; // [esp-8h] [ebp-43Ch]
  unsigned int *v17; // [esp-4h] [ebp-438h]
  unsigned int *v18; // [esp+Ch] [ebp-428h]
  unsigned int *v19; // [esp+10h] [ebp-424h]
  unsigned int *v20; // [esp+14h] [ebp-420h]
  unsigned int *v21; // [esp+18h] [ebp-41Ch]
  unsigned int *v22; // [esp+1Ch] [ebp-418h]
  int v23; // [esp+20h] [ebp-414h]
  unsigned int *v24; // [esp+24h] [ebp-410h]
  _BYTE *v25; // [esp+28h] [ebp-40Ch]
  __m64 v26[128]; // [esp+2Ch] [ebp-408h]

  v22 = *a1;
  v24 = *a3;
  v21 = a1[1];
  v18 = a3[1];
  v23 = a2[1];
  v25 = (_BYTE *)*a2;
  v4 = 0;
  v5 = 0;
  v20 = a3[3];
  v19 = a3[2];
  do
  {
    v6 = 0x100010001LL * (v5 >> 8);
    v26[v4].m64_i32[0] = v6;
    v26[v4++].m64_i32[1] = HIDWORD(v6);
    v5 += 256 - a4;
  }
  while ( v4 < 128 );
  result = 255;
  v8 = v20;
  v9 = v22;
  v10 = v24;
  v11 = v25;
  do
  {
    v17 = v8;
    v16 = v9;
    v15 = v10;
    v14 = v11;
    v12 = v19;
    do
    {
      if ( *v11 && (*v10 & 0xFE000000) != 0 )
      {
        v13 = _m_punpcklbw(_mm_cvtsi32_si64(*v9), 0LL);
        *v9 = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(
                    _m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v10), 0LL), v13), v26[*v10 >> 25]), 7u),
                    v13),
                  0LL));
      }
      ++v11;
      ++v10;
      ++v9;
      v12 = (unsigned int *)((char *)v12 - 1);
    }
    while ( v12 );
    v11 = &v14[v23];
    v10 = (unsigned int *)((char *)v15 + (_DWORD)v18);
    v9 = (unsigned int *)((char *)v16 + (_DWORD)v21);
    v8 = (unsigned int *)((char *)v17 - 1);
  }
  while ( v17 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_416510 @ 0x00416510..0x0041652E =====
int __usercall sub_416510@<eax>(int result@<eax>, int a2@<ecx>, int a3@<esi>)
{
  if ( *(_DWORD *)(a2 + 16) == 3 && *(_DWORD *)(a3 + 16) == 2 && *(_DWORD *)(result + 16) == 2 )
    return sub_416530(a3);
  return result;
}

// ===== sub_416530 @ 0x00416530..0x0041660D =====
unsigned int __usercall sub_416530@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, unsigned int **a3)
{
  __m64 *v3; // esi
  int v4; // edx
  unsigned int v5; // eax
  int v6; // ecx
  unsigned int *v7; // edi
  unsigned __int16 *v8; // ebx
  unsigned int v9; // ecx
  unsigned int result; // eax
  __m64 v11; // mm0
  int v12; // ecx
  unsigned int *v13; // edi
  _BYTE *v14; // ebx
  unsigned int v15; // ecx
  __int32 v16; // edx
  unsigned __int16 *v17; // [esp-10h] [ebp-38h]
  _BYTE *v18; // [esp-10h] [ebp-38h]
  __m64 *v19; // [esp-Ch] [ebp-34h]
  __m64 *v20; // [esp-Ch] [ebp-34h]
  unsigned int *v21; // [esp-8h] [ebp-30h]
  unsigned int *v22; // [esp-8h] [ebp-30h]
  int v23; // [esp-4h] [ebp-2Ch]
  int v24; // [esp-4h] [ebp-2Ch]
  unsigned int *v25; // [esp+10h] [ebp-18h]
  _BYTE *v26; // [esp+18h] [ebp-10h]
  int v27; // [esp+1Ch] [ebp-Ch]
  int v28; // [esp+20h] [ebp-8h]
  unsigned int *v29; // [esp+24h] [ebp-4h]
  unsigned int v30; // [esp+30h] [ebp+8h]
  unsigned int v31; // [esp+30h] [ebp+8h]

  v29 = a3[1];
  v25 = *a3;
  v3 = *(__m64 **)a1;
  v28 = *(_DWORD *)(a1 + 4);
  v4 = *(_DWORD *)(a1 + 12);
  v5 = *(_DWORD *)(a1 + 8);
  v26 = (_BYTE *)*a2;
  v27 = a2[1];
  v30 = v5;
  if ( (v5 & 1) != 0 )
  {
    v12 = v4;
    v13 = v25;
    v14 = v26;
    do
    {
      v24 = v12;
      v22 = v13;
      v20 = v3;
      v18 = v14;
      v15 = v30;
      do
      {
        v16 = v3->m64_i32[0];
        v3 = (__m64 *)((char *)v3 + 4);
        result = (*v14++ == 0) - 1;
        *v13++ = result & v16;
        --v15;
      }
      while ( v15 );
      v14 = &v18[v27];
      v3 = (__m64 *)((char *)v20 + v28);
      v13 = (unsigned int *)((char *)v22 + (_DWORD)v29);
      v12 = v24 - 1;
    }
    while ( v24 != 1 );
  }
  else
  {
    v31 = v5 >> 1;
    v6 = v4;
    v7 = v25;
    v8 = (unsigned __int16 *)v26;
    do
    {
      v23 = v6;
      v21 = v7;
      v19 = v3;
      v17 = v8;
      v9 = v31;
      do
      {
        result = *v8;
        v7 += 2;
        ++v8;
        v11 = _m_pand(
                _m_pcmpgtd(_m_punpcklbw(_m_punpcklbw(_mm_cvtsi32_si64(result), 0LL), 0LL), 0LL),
                (__m64)v3->m64_u64);
        ++v3;
        *((__m64 *)v7 - 1) = v11;
        --v9;
      }
      while ( v9 );
      v8 = (unsigned __int16 *)((char *)v17 + v27);
      v3 = (__m64 *)((char *)v19 + v28);
      v7 = (unsigned int *)((char *)v21 + (_DWORD)v29);
      v6 = v23 - 1;
    }
    while ( v23 != 1 );
  }
  _m_empty();
  return result;
}

// ===== sub_416610 @ 0x00416610..0x00416744 =====
int __usercall sub_416610@<eax>(
        _DWORD *a1@<eax>,
        int a2@<ecx>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  _DWORD *v12; // eax
  _DWORD *v13; // eax
  int v15[4]; // [esp+10h] [ebp-20h] BYREF
  int v16[4]; // [esp+20h] [ebp-10h] BYREF
  int v17; // [esp+40h] [ebp+10h]

  v17 = 0;
  if ( !(_WORD)a5 && !(_WORD)a6 && !(_WORD)a7 && !(_WORD)a8 && !(a2 % 23592960) && a9 == 0x10000 && a10 == 0x10000 )
  {
    v12 = sub_409190(v15, a4);
    sub_409170(-(a6 >> 16), -(a5 >> 16), v12);
    v13 = sub_409190(v16, (int)a1);
    sub_409170(-(a8 >> 16), -(a7 >> 16), v13);
    if ( sub_4090B0(v16, v15) )
    {
      if ( a3 )
      {
        sub_409110(v16, v15);
        sub_409170(a8 >> 16, a7 >> 16, v16);
        *a3 = *a1;
        a3[1] = a1[1];
        a3[2] = a1[2];
        a3[3] = a1[3];
        a3[4] = a1[4];
        a3[5] = a1[5];
        sub_4091B0(a3, v16);
      }
      return 1;
    }
  }
  return v17;
}

// ===== sub_416750 @ 0x00416750..0x004168D5 =====
int __usercall sub_416750@<eax>(
        _DWORD *a1@<eax>,
        _DWORD *a2@<esi>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        unsigned int a9,
        unsigned int a10)
{
  long double v10; // st7
  long double v11; // st3
  int result; // eax
  double v13; // [esp+0h] [ebp-30h]
  double v14; // [esp+8h] [ebp-28h]
  long double v15; // [esp+10h] [ebp-20h]
  double v16; // [esp+18h] [ebp-18h]
  long double v17; // [esp+28h] [ebp-8h]

  v17 = -((double)a8 * 3.141592653589793 / 11796480.0);
  v10 = sin(v17);
  v13 = 65536.0 / (double)a9;
  v14 = 65536.0 / (double)a10;
  v16 = -((double)a4 * 0.0000152587890625);
  v11 = cos(v17);
  *a3 = (__int64)(((v16 * v11 - (double)a5 * 0.0000152587890625 * v10) * v13 + (double)a6 * 0.0000152587890625) * 65536.0);
  a3[1] = (int)((0.0000152587890625 * (double)a7 - ((double)a5 * 0.0000152587890625 * v11 + v16 * v10) * v14) * 65536.0);
  a1[1] = (int)(v10 * v14 * -65536.0);
  *a1 = (__int64)(v11 * v13 * 65536.0);
  v15 = 1.570796326794897 - (double)a8 * 3.141592653589793 / 11796480.0;
  result = HIWORD(a8) | 0xC00;
  *a2 = (__int64)(cos(v15) * v13 * -65536.0);
  a2[1] = (int)(sin(v15) * v14 * 65536.0);
  return result;
}

// ===== sub_4168E0 @ 0x004168E0..0x00416B92 =====
int __cdecl sub_4168E0(
        int a1,
        int a2,
        int a3,
        _DWORD *a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        unsigned int a10,
        int a11,
        int a12)
{
  int v13; // [esp+10h] [ebp-30h] BYREF
  _DWORD v14[2]; // [esp+14h] [ebp-2Ch] BYREF
  _DWORD v15[8]; // [esp+1Ch] [ebp-24h] BYREF

  if ( !a8 || !a9 )
    return 19;
  if ( sub_416610(a4, a7, v15, a1, a2, a3, a5, a6, a8, a9) )
  {
    sub_40B320((int)v15, a10, a1);
    return 0;
  }
  if ( a12 )
  {
    v15[1] = a5;
    v15[3] = a7;
    v15[5] = a9;
    v15[2] = a6;
    v15[7] = a11;
    v14[0] = a2;
    v15[6] = a10;
    v14[1] = a3;
    v13 = a1;
    v15[0] = a4;
    v15[4] = a8;
    if ( sub_419DE0(sub_41A1B0, &v13, 1, 2, v14, v15) )
      return 0;
  }
  if ( *(_DWORD *)(a1 + 16) != 1 )
    return 0;
  if ( a4[4] != 1 )
  {
    if ( a4[4] == 2 )
    {
      if ( a11 )
      {
        if ( a10 < 0x100 )
        {
          sub_417260(a1, a2, a3, a5, a6, a7, a8, a9, a10);
          return 0;
        }
      }
      else if ( a10 < 0x100 )
      {
        sub_417560(a1, a2, a3, a5, a6, a7, a8, a9, a10);
        return 0;
      }
    }
    return 0;
  }
  if ( !a11 )
  {
    if ( a10 < 0x100 )
    {
      if ( a10 )
      {
        sub_417130(a2, a3, a5, a6, a7, a8, a9, a10);
        return 0;
      }
      sub_418280(a2, a3, a5, a6, a7, a8, a9);
    }
    return 0;
  }
  if ( a10 >= 0x100 )
    return 0;
  if ( a10 )
    sub_416BA0(a1, a2, a3, a5, a6, a7, a8, a9, a10);
  else
    sub_417C50(a1, a2, a3, a5, a6, a7, a8, a9);
  return 0;
}

// ===== sub_416BA0 @ 0x00416BA0..0x00417123 =====
unsigned int __cdecl sub_416BA0(
        __m64 **a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9)
{
  int *v9; // ecx
  __m64 *v10; // eax
  int v11; // edi
  unsigned int v12; // edx
  unsigned int v13; // eax
  int v14; // ecx
  int v15; // esi
  __int64 v16; // rax
  __m64 *v17; // ecx
  unsigned int *v18; // edi
  int v19; // esi
  __m64 *v20; // ecx
  __m64 v21; // mm7
  __m64 v22; // mm0
  int v23; // edx
  int v24; // ebx
  int v25; // edx
  unsigned int result; // eax
  __m64 v27; // mm4
  __m64 v28; // mm4
  __m64 v29; // mm0
  __m64 v30; // mm1
  unsigned int v31; // eax
  __m64 v32; // mm2
  __m64 v33; // mm3
  int v34; // ebx
  unsigned int v35; // eax
  int v36; // edx
  __m64 v37; // mm4
  __m64 v38; // mm3
  __m64 v39; // mm2
  __m64 v40; // mm1
  __m64 *v41; // ecx
  __m64 *v42; // edi
  int v43; // esi
  __m64 *v44; // ecx
  __m64 v45; // mm7
  __m64 v46; // mm0
  int v47; // edx
  int v48; // ebx
  int v49; // edx
  unsigned int v50; // eax
  __m64 v51; // mm4
  __m64 v52; // mm7
  __m64 v53; // mm4
  __m64 v54; // mm0
  __m64 v55; // mm1
  unsigned int v56; // eax
  __m64 v57; // mm2
  __m64 v58; // mm3
  int v59; // ebx
  unsigned int v60; // edx
  __m64 v61; // mm4
  __m64 v62; // mm3
  __m64 v63; // mm2
  __m64 v64; // mm0
  int v65; // edx
  int v66; // ebx
  int v67; // edx
  __m64 v68; // mm4
  __m64 v69; // mm4
  __m64 v70; // mm0
  __m64 v71; // mm1
  unsigned int v72; // eax
  __m64 v73; // mm2
  __m64 v74; // mm3
  int v75; // ebx
  unsigned int v76; // eax
  int v77; // edx
  __m64 v78; // mm4
  __m64 v79; // mm3
  __m64 v80; // mm2
  __m64 v81; // mm3
  __m64 v82; // mm1
  unsigned int *v83; // [esp-8h] [ebp-FCh]
  __m64 *v84; // [esp-8h] [ebp-FCh]
  __m64 *v85; // [esp-4h] [ebp-F8h]
  __m64 *v86; // [esp-4h] [ebp-F8h]
  __m64 v87; // [esp+Ch] [ebp-E8h] BYREF
  __m64 v88; // [esp+14h] [ebp-E0h]
  __m64 v89; // [esp+1Ch] [ebp-D8h] BYREF
  __m64 v90; // [esp+24h] [ebp-D0h]
  __m64 v91; // [esp+2Ch] [ebp-C8h]
  __m64 v92; // [esp+34h] [ebp-C0h]
  unsigned int v93; // [esp+40h] [ebp-B4h]
  unsigned int v94; // [esp+44h] [ebp-B0h]
  int v95; // [esp+48h] [ebp-ACh]
  int v96; // [esp+4Ch] [ebp-A8h]
  __m64 *v97; // [esp+50h] [ebp-A4h]
  __m64 *v98; // [esp+54h] [ebp-A0h]
  __m64 *v99; // [esp+58h] [ebp-9Ch]
  __m64 v100; // [esp+5Ch] [ebp-98h] BYREF
  __m64 *v101; // [esp+68h] [ebp-8Ch]
  _DWORD v102[33]; // [esp+6Ch] [ebp-88h]

  v98 = *a1;
  v96 = *v9;
  v97 = a1[1];
  v10 = a1[3];
  v11 = v9[1];
  v101 = a1[2];
  v12 = v9[2];
  v99 = v10;
  v13 = v9[3];
  v94 = v12;
  v93 = v13;
  v95 = v11;
  sub_416750(&v87, &v89, &v100, a2, a3, a4, a5, a6, a7, a8);
  v90.m64_u64 = (v11 << 14) | 1;
  v91.m64_u64 = 0xF0000000F000LL;
  v92.m64_u64 = 0x1000100000LL * (256 - a9) + 16 * (256 - a9);
  v14 = 0;
  v15 = 16;
  do
  {
    v16 = v15 << 8;
    v102[2 * v14] = v16 | ((v15 | ((v15 | (v15 << 16)) << 16)) << 24);
    v102[2 * v14++ + 1] = HIDWORD(v16) | ((v15 | ((v15 | (unsigned __int64)((__int64)v15 << 16)) << 16)) >> 8);
    --v15;
  }
  while ( v14 < 16 );
  if ( ((unsigned __int8)v101 & 1) != 0 )
  {
    v17 = v99;
    v18 = (unsigned int *)v98;
    v19 = v96;
    while ( 1 )
    {
      v85 = v17;
      v83 = v18;
      v20 = v101;
      v21 = v100;
      v100 = _m_paddd(v89, v100);
      do
      {
        v22 = _m_packssdw(_m_psradi(v21, 0x10u), 0LL);
        v23 = _mm_cvtsi64_si32(v22);
        v24 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v22, v90));
        LOWORD(result) = v23;
        v25 = v23 >> 16;
        result = (__int16)result;
        v27 = v21;
        v21 = _m_paddd(v21, v87);
        v28 = _m_packssdw(_m_psrlwi(_m_pand(v27, v91), 9u), 0LL);
        v29.m64_u64 = 0LL;
        v30.m64_u64 = 0LL;
        if ( v25 > -2 && (__int16)result > -2 && v25 < (int)v93 )
        {
          if ( v25 < v93 )
          {
            if ( (__int16)result >= (int)v94 )
              goto LABEL_23;
            if ( (__int16)result < v94 )
              v29 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v24));
            v31 = (__int16)result + 1;
            if ( v31 < v94 )
              v30 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v24 + 4));
            v29 = _m_punpcklbw(v29, 0LL);
            v30 = _m_punpcklbw(v30, 0LL);
            result = v31 - 1;
          }
          v32.m64_u64 = 0LL;
          v33.m64_u64 = 0LL;
          if ( v25 + 1 < v93 )
          {
            v34 = v95 + v24;
            if ( (int)result >= (int)v94 )
              goto LABEL_23;
            if ( result < v94 )
              v32 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v34));
            if ( result + 1 < v94 )
              v33 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v34 + 4));
          }
          v35 = _mm_cvtsi64_si32(v28);
          v36 = (unsigned __int16)v35;
          result = HIWORD(v35);
          v37 = *(__m64 *)((char *)v102 + v36);
          v38 = _m_punpcklbw(v33, 0LL);
          v39 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v32, 0LL), v38), 4u), v37), v38);
          v29 = _m_paddw(
                  _m_pmulhw(
                    _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v29, v30), 4u), v37), v30), v39), 4u),
                    *(__m64 *)((char *)v102 + result)),
                  v39);
        }
LABEL_23:
        v40 = _m_punpcklbw(_mm_cvtsi32_si64(*v18), 0LL);
        *v18++ = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v29, v40), 4u), v92), v40), 0LL));
        v20 = (__m64 *)((char *)v20 - 1);
      }
      while ( v20 );
      v18 = (unsigned int *)((char *)v83 + (_DWORD)v97);
      v17 = (__m64 *)((char *)v85 - 1);
      if ( v85 == (__m64 *)1 )
        goto LABEL_64;
    }
  }
  v101 = (__m64 *)((unsigned int)v101 >> 1);
  v41 = v99;
  v42 = v98;
  v43 = v96;
  do
  {
    v86 = v41;
    v84 = v42;
    v44 = v101;
    v45 = v100;
    v100 = _m_paddd(v89, v100);
    do
    {
      v46 = _m_packssdw(_m_psradi(v45, 0x10u), 0LL);
      v47 = _mm_cvtsi64_si32(v46);
      v48 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v46, v90));
      LOWORD(v50) = v47;
      v49 = v47 >> 16;
      v50 = (__int16)v50;
      v51 = v45;
      v52 = _m_paddd(v45, v87);
      v53 = _m_packssdw(_m_psrlwi(_m_pand(v51, v91), 9u), 0LL);
      v54.m64_u64 = 0LL;
      v55.m64_u64 = 0LL;
      if ( v49 > -2 && (__int16)v50 > -2 && v49 < (int)v93 )
      {
        if ( v49 < v93 )
        {
          if ( (__int16)v50 >= (int)v94 )
            goto LABEL_45;
          if ( (__int16)v50 < v94 )
            v54 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v48));
          v56 = (__int16)v50 + 1;
          if ( v56 < v94 )
            v55 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v48 + 4));
          v54 = _m_punpcklbw(v54, 0LL);
          v55 = _m_punpcklbw(v55, 0LL);
          v50 = v56 - 1;
        }
        v57.m64_u64 = 0LL;
        v58.m64_u64 = 0LL;
        if ( v49 + 1 >= v93 )
          goto LABEL_44;
        v59 = v95 + v48;
        if ( (int)v50 < (int)v94 )
        {
          if ( v50 < v94 )
            v57 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v59));
          if ( v50 + 1 < v94 )
            v58 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v59 + 4));
LABEL_44:
          v60 = _mm_cvtsi64_si32(v53);
          v61 = *(__m64 *)((char *)v102 + (unsigned __int16)v60);
          v62 = _m_punpcklbw(v58, 0LL);
          v63 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v57, 0LL), v62), 4u), v61), v62);
          v54 = _m_paddw(
                  _m_pmulhw(
                    _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v54, v55), 4u), v61), v55), v63), 4u),
                    *(__m64 *)((char *)v102 + HIWORD(v60))),
                  v63);
        }
      }
LABEL_45:
      v88 = v54;
      v64 = _m_packssdw(_m_psradi(v52, 0x10u), 0LL);
      v65 = _mm_cvtsi64_si32(v64);
      v66 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v64, v90));
      LOWORD(result) = v65;
      v67 = v65 >> 16;
      result = (__int16)result;
      v68 = v52;
      v45 = _m_paddd(v52, v87);
      v69 = _m_packssdw(_m_psrlwi(_m_pand(v68, v91), 9u), 0LL);
      v70.m64_u64 = 0LL;
      v71.m64_u64 = 0LL;
      if ( v67 > -2 && (__int16)result > -2 && v67 < (int)v93 )
      {
        if ( v67 < v93 )
        {
          if ( (__int16)result >= (int)v94 )
            goto LABEL_62;
          if ( (__int16)result < v94 )
            v70 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v66));
          v72 = (__int16)result + 1;
          if ( v72 < v94 )
            v71 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v66 + 4));
          v70 = _m_punpcklbw(v70, 0LL);
          v71 = _m_punpcklbw(v71, 0LL);
          result = v72 - 1;
        }
        v73.m64_u64 = 0LL;
        v74.m64_u64 = 0LL;
        if ( v67 + 1 < v93 )
        {
          v75 = v95 + v66;
          if ( (int)result >= (int)v94 )
            goto LABEL_62;
          if ( result < v94 )
            v73 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v75));
          if ( result + 1 < v94 )
            v74 = _mm_cvtsi32_si64(*(_DWORD *)(v43 + v75 + 4));
        }
        v76 = _mm_cvtsi64_si32(v69);
        v77 = (unsigned __int16)v76;
        result = HIWORD(v76);
        v78 = *(__m64 *)((char *)v102 + v77);
        v79 = _m_punpcklbw(v74, 0LL);
        v80 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v73, 0LL), v79), 4u), v78), v79);
        v70 = _m_paddw(
                _m_pmulhw(
                  _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v70, v71), 4u), v78), v71), v80), 4u),
                  *(__m64 *)((char *)v102 + result)),
                v80);
      }
LABEL_62:
      v81 = _m_punpcklbw((__m64)v42->m64_u64, 0LL);
      v82 = _m_punpckhbw((__m64)v42->m64_u64, 0LL);
      v42->m64_u64 = (unsigned __int64)_m_por(
                                         _m_psllqi(
                                           _m_packuswb(
                                             _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v70, v82), 4u), v92), v82),
                                             0LL),
                                           0x20u),
                                         _m_packuswb(
                                           _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v88, v81), 4u), v92), v81),
                                           0LL));
      ++v42;
      v44 = (__m64 *)((char *)v44 - 1);
    }
    while ( v44 );
    v42 = (__m64 *)((char *)v84 + (_DWORD)v97);
    v41 = (__m64 *)((char *)v86 - 1);
  }
  while ( v86 != (__m64 *)1 );
LABEL_64:
  _m_empty();
  return result;
}

// ===== sub_417130 @ 0x00417130..0x0041725E =====
int __usercall sub_417130@<eax>(
        unsigned int **a1@<eax>,
        int *a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9,
        int a10)
{
  int v11; // edx
  unsigned int *v12; // ecx
  unsigned int *v13; // edx
  unsigned int *v14; // eax
  unsigned int v15; // ecx
  unsigned int v16; // edx
  __m64 v17; // mm4
  __m64 v18; // mm5
  unsigned int *v19; // ecx
  unsigned int *v20; // edi
  int v21; // esi
  unsigned int *v22; // ecx
  __m64 v23; // mm7
  __m64 v24; // mm0
  __m64 v25; // mm0
  int v26; // edx
  int v27; // ebx
  int result; // eax
  __m64 v29; // mm0
  __m64 v30; // mm1
  unsigned int *v31; // [esp-8h] [ebp-60h]
  unsigned int *v32; // [esp-4h] [ebp-5Ch]
  __m64 v33; // [esp+10h] [ebp-48h] BYREF
  __m64 v34; // [esp+18h] [ebp-40h] BYREF
  __m64 v35; // [esp+20h] [ebp-38h]
  __m64 v36; // [esp+28h] [ebp-30h]
  __m64 v37; // [esp+30h] [ebp-28h] BYREF
  unsigned int *v38; // [esp+3Ch] [ebp-1Ch]
  unsigned int *v39; // [esp+40h] [ebp-18h]
  int v40; // [esp+44h] [ebp-14h]
  unsigned int *v41; // [esp+48h] [ebp-10h]
  unsigned int v42; // [esp+4Ch] [ebp-Ch]
  unsigned int v43; // [esp+50h] [ebp-8h]
  unsigned int *v44; // [esp+54h] [ebp-4h]

  v11 = *a2;
  v39 = *a1;
  v12 = a1[1];
  v40 = v11;
  v13 = a1[2];
  v14 = a1[3];
  v44 = v12;
  v15 = a2[2];
  v38 = v14;
  v41 = v13;
  v16 = a2[3];
  v43 = v15;
  v42 = v16;
  sub_416750(&v33, &v34, &v37, a3, a4, a5, a6, a7, a8, a9);
  v35.m64_u64 = (a2[1] << 14) | 1;
  v36.m64_u64 = 0x1000100000LL * (256 - a10) + 16 * (256 - a10);
  v17 = v33;
  v18 = v35;
  v19 = v38;
  v20 = v39;
  v21 = v40;
  do
  {
    v32 = v19;
    v31 = v20;
    v22 = v41;
    v23 = v37;
    v37 = _m_paddd(v34, v37);
    do
    {
      v24 = v23;
      v23 = _m_paddd(v23, v17);
      v25 = _m_packssdw(_m_psradi(v24, 0x10u), 0LL);
      v26 = _mm_cvtsi64_si32(v25);
      v27 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v25, v18));
      result = (__int16)v26;
      v29.m64_u64 = 0LL;
      if ( v26 >> 16 < v42 && (__int16)v26 < v43 )
        v29 = _mm_cvtsi32_si64(*(_DWORD *)(v21 + v27));
      v30 = _m_punpcklbw(_mm_cvtsi32_si64(*v20), 0LL);
      *v20++ = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v29, 0LL), v30), 4u), v36), v30), 0LL));
      v22 = (unsigned int *)((char *)v22 - 1);
    }
    while ( v22 );
    v20 = (unsigned int *)((char *)v31 + (_DWORD)v44);
    v19 = (unsigned int *)((char *)v32 - 1);
  }
  while ( v32 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_417260 @ 0x00417260..0x00417553 =====
unsigned int __cdecl sub_417260(
        unsigned int **a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9)
{
  int *v9; // ecx
  unsigned int *v10; // eax
  int v11; // edi
  unsigned int v12; // edx
  unsigned int v13; // eax
  int v14; // esi
  unsigned int v15; // ebx
  int v16; // edi
  __int64 v17; // rax
  __int64 v18; // rax
  int v19; // ecx
  int v20; // esi
  __int64 v21; // rax
  unsigned int *v22; // ecx
  unsigned int *v23; // edi
  int v24; // esi
  unsigned int *v25; // ecx
  __m64 v26; // mm7
  __m64 v27; // mm0
  int v28; // edx
  int v29; // ebx
  int v30; // edx
  unsigned int result; // eax
  __m64 v32; // mm4
  __m64 v33; // mm4
  __m64 v34; // mm0
  __m64 v35; // mm1
  unsigned int v36; // eax
  __m64 v37; // mm2
  __m64 v38; // mm3
  int v39; // ebx
  unsigned int v40; // eax
  __m64 v41; // mm4
  __m64 v42; // mm3
  __m64 v43; // mm2
  __m64 v44; // mm0
  __m64 v45; // mm1
  unsigned int *v46; // [esp-8h] [ebp-4ECh]
  unsigned int *v47; // [esp-4h] [ebp-4E8h]
  __m64 v48; // [esp+Ch] [ebp-4D8h] BYREF
  __m64 v49; // [esp+14h] [ebp-4D0h] BYREF
  __m64 v50; // [esp+1Ch] [ebp-4C8h]
  __m64 v51; // [esp+24h] [ebp-4C0h]
  int v52; // [esp+30h] [ebp-4B4h]
  int v53; // [esp+34h] [ebp-4B0h]
  unsigned int *v54; // [esp+38h] [ebp-4ACh]
  unsigned int *v55; // [esp+3Ch] [ebp-4A8h]
  unsigned int v56; // [esp+40h] [ebp-4A4h]
  unsigned int v57; // [esp+44h] [ebp-4A0h]
  unsigned int *v58; // [esp+48h] [ebp-49Ch]
  __m64 v59; // [esp+4Ch] [ebp-498h] BYREF
  unsigned int *v60; // [esp+58h] [ebp-48Ch]
  _QWORD v61[144]; // [esp+5Ch] [ebp-488h]

  v55 = *a1;
  v53 = *v9;
  v58 = a1[1];
  v10 = a1[3];
  v11 = v9[1];
  v54 = a1[2];
  v12 = v9[2];
  v60 = v10;
  v13 = v9[3];
  v57 = v12;
  v56 = v13;
  v52 = v11;
  sub_416750(&v48, &v49, &v59, a2, a3, a4, a5, a6, a7, a8);
  v50.m64_u64 = (v11 << 14) | 1;
  v14 = 0;
  v15 = 0;
  v16 = 256 - a9;
  v51.m64_u64 = 0xF0000000F000LL;
  do
  {
    v17 = 0x100010001LL * (v15 >> 8);
    LODWORD(v61[v14]) = v17;
    HIDWORD(v61[v14++]) = HIDWORD(v17);
    v15 += v16;
  }
  while ( v14 < 127 );
  v18 = 0x100010001LL * ((unsigned int)(v16 << 7) >> 8);
  LODWORD(v61[v14]) = v18;
  HIDWORD(v61[v14]) = HIDWORD(v18);
  v19 = 0;
  v20 = 16;
  do
  {
    v21 = v20 << 8;
    LODWORD(v61[v19 + 128]) = v21 | ((v20 | ((v20 | (v20 << 16)) << 16)) << 24);
    HIDWORD(v61[v19++ + 128]) = HIDWORD(v21) | ((v20 | ((v20 | (unsigned __int64)((__int64)v20 << 16)) << 16)) >> 8);
    --v20;
  }
  while ( v19 < 16 );
  v22 = v60;
  v23 = v55;
  v24 = v53;
  do
  {
    v47 = v22;
    v46 = v23;
    v25 = v54;
    v26 = v59;
    v59 = _m_paddd(v49, v59);
    do
    {
      v27 = _m_packssdw(_m_psradi(v26, 0x10u), 0LL);
      v28 = _mm_cvtsi64_si32(v27);
      v29 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v27, v50));
      LOWORD(result) = v28;
      v30 = v28 >> 16;
      result = (__int16)result;
      v32 = v26;
      v26 = _m_paddd(v26, v48);
      v33 = _m_packssdw(_m_psrlwi(_m_pand(v32, v51), 9u), 0LL);
      v34.m64_u64 = 0LL;
      v35.m64_u64 = 0LL;
      if ( v30 > -2 && v30 < (int)v56 )
      {
        if ( v30 < v56 )
        {
          if ( (__int16)result <= -2 || (__int16)result >= (int)v57 )
            goto LABEL_24;
          if ( (__int16)result < v57 )
            v34 = _mm_cvtsi32_si64(*(_DWORD *)(v24 + v29));
          v36 = (__int16)result + 1;
          if ( v36 < v57 )
            v35 = _mm_cvtsi32_si64(*(_DWORD *)(v24 + v29 + 4));
          v34 = _m_punpcklbw(v34, 0LL);
          v35 = _m_punpcklbw(v35, 0LL);
          result = v36 - 1;
        }
        v37.m64_u64 = 0LL;
        v38.m64_u64 = 0LL;
        if ( v30 + 1 < v56 )
        {
          v39 = v52 + v29;
          if ( result < v57 )
            v37 = _mm_cvtsi32_si64(*(_DWORD *)(v24 + v39));
          if ( result + 1 < v57 )
            v38 = _mm_cvtsi32_si64(*(_DWORD *)(v24 + v39 + 4));
        }
        v40 = _mm_cvtsi64_si32(v33);
        v41 = *(__m64 *)((char *)&v61[128] + (unsigned __int16)v40);
        v42 = _m_punpcklbw(v38, 0LL);
        v43 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v37, 0LL), v42), 4u), v41), v42);
        v44 = _m_paddw(
                _m_pmulhw(
                  _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v34, v35), 4u), v41), v35), v43), 4u),
                  *(__m64 *)((char *)&v61[128] + HIWORD(v40))),
                v43);
        result = (unsigned int)_mm_cvtsi64_si32(_m_packuswb(v44, 0LL)) >> 25;
        if ( result )
        {
          v45 = _m_punpcklbw(_mm_cvtsi32_si64(*v23), 0LL);
          *v23 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v44, v45), (__m64)v61[result]), 7u), v45), 0LL));
        }
      }
LABEL_24:
      ++v23;
      v25 = (unsigned int *)((char *)v25 - 1);
    }
    while ( v25 );
    v23 = (unsigned int *)((char *)v46 + (_DWORD)v58);
    v22 = (unsigned int *)((char *)v47 - 1);
  }
  while ( v47 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_417560 @ 0x00417560..0x0041772A =====
unsigned int __cdecl sub_417560(
        unsigned int **a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9)
{
  _DWORD *v9; // ecx
  _DWORD *v10; // edi
  int v11; // edx
  unsigned int *v12; // ecx
  unsigned int *v13; // edx
  unsigned int *v14; // eax
  unsigned int v15; // ecx
  unsigned int v16; // edx
  __m64 v17; // rax
  int v18; // esi
  unsigned int v19; // ebx
  int v20; // edi
  __int64 v21; // rax
  __int64 v22; // rax
  __m64 v23; // mm4
  __m64 v24; // mm5
  unsigned int *v25; // ecx
  unsigned int *v26; // edi
  int v27; // esi
  unsigned int *v28; // ecx
  __m64 v29; // mm7
  __m64 v30; // mm0
  __m64 v31; // mm0
  int v32; // edx
  int v33; // ebx
  unsigned int result; // eax
  unsigned int v35; // eax
  __m64 v36; // mm0
  __m64 v37; // mm1
  unsigned int *v38; // [esp-8h] [ebp-45Ch]
  unsigned int *v39; // [esp-4h] [ebp-458h]
  __m64 v40; // [esp+Ch] [ebp-448h] BYREF
  __m64 v41; // [esp+14h] [ebp-440h] BYREF
  __m64 v42; // [esp+1Ch] [ebp-438h]
  unsigned int *v43; // [esp+28h] [ebp-42Ch]
  unsigned int v44; // [esp+2Ch] [ebp-428h]
  int v45; // [esp+30h] [ebp-424h]
  unsigned int v46; // [esp+34h] [ebp-420h]
  unsigned int *v47; // [esp+38h] [ebp-41Ch]
  __m64 v48; // [esp+3Ch] [ebp-418h] BYREF
  unsigned int *v49; // [esp+44h] [ebp-410h]
  unsigned int *v50; // [esp+48h] [ebp-40Ch]
  __m64 v51[128]; // [esp+4Ch] [ebp-408h]

  v10 = v9;
  v11 = *v9;
  v50 = *a1;
  v12 = a1[1];
  v45 = v11;
  v13 = a1[2];
  v14 = a1[3];
  v43 = v12;
  v15 = v10[2];
  v49 = v14;
  v47 = v13;
  v16 = v10[3];
  v44 = v15;
  v46 = v16;
  sub_416750(&v41, &v40, &v48, a2, a3, a4, a5, a6, a7, a8);
  v17.m64_u64 = (v10[1] << 14) | 1;
  v18 = 0;
  v19 = 0;
  v20 = 256 - a9;
  v42 = v17;
  do
  {
    v21 = 0x100010001LL * (v19 >> 8);
    v51[v18].m64_i32[0] = v21;
    v51[v18++].m64_i32[1] = HIDWORD(v21);
    v19 += v20;
  }
  while ( v18 < 127 );
  v22 = 0x100010001LL * ((unsigned int)(v20 << 7) >> 8);
  v51[v18].m64_i32[0] = v22;
  v51[v18].m64_i32[1] = HIDWORD(v22);
  v23 = v41;
  v24 = v42;
  v25 = v49;
  v26 = v50;
  v27 = v45;
  do
  {
    v39 = v25;
    v38 = v26;
    v28 = v47;
    v29 = v48;
    v48 = _m_paddd(v40, v48);
    do
    {
      v30 = v29;
      v29 = _m_paddd(v29, v23);
      v31 = _m_packssdw(_m_psradi(v30, 0x10u), 0LL);
      v32 = _mm_cvtsi64_si32(v31);
      v33 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v31, v24));
      result = (__int16)v32;
      if ( v32 >> 16 < v46 && (__int16)v32 < v44 )
      {
        v35 = *(_DWORD *)(v27 + v33);
        v36 = _mm_cvtsi32_si64(v35);
        result = v35 >> 25;
        if ( result )
        {
          v37 = _m_punpcklbw(_mm_cvtsi32_si64(*v26), 0LL);
          *v26 = _mm_cvtsi64_si32(
                   _m_packuswb(
                     _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v36, 0LL), v37), v51[result]), 7u), v37),
                     0LL));
        }
      }
      ++v26;
      v28 = (unsigned int *)((char *)v28 - 1);
    }
    while ( v28 );
    v26 = (unsigned int *)((char *)v38 + (_DWORD)v43);
    v25 = (unsigned int *)((char *)v39 - 1);
  }
  while ( v39 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_417730 @ 0x00417730..0x00417997 =====
int __cdecl sub_417730(
        int a1,
        int a2,
        int a3,
        _DWORD *a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        unsigned int a10,
        int a11,
        int a12)
{
  int v13; // ecx
  int v14; // eax
  int v15; // [esp+10h] [ebp-30h] BYREF
  _DWORD v16[2]; // [esp+14h] [ebp-2Ch] BYREF
  _DWORD v17[8]; // [esp+1Ch] [ebp-24h] BYREF

  if ( !a8 || !a9 )
    return 19;
  if ( sub_416610(a4, a7, v17, a1, a2, a3, a5, a6, a8, a9) )
  {
    sub_418800();
    return 0;
  }
  if ( a12 )
  {
    v17[1] = a5;
    v17[3] = a7;
    v17[5] = a9;
    v17[2] = a6;
    v17[7] = a11;
    v16[0] = a2;
    v17[6] = a10;
    v16[1] = a3;
    v15 = a1;
    v17[0] = a4;
    v17[4] = a8;
    if ( sub_419DE0(sub_41A220, &v15, 1, 2, v16, v17) )
      return 0;
  }
  v13 = *(_DWORD *)(a1 + 16);
  v14 = a4[4];
  if ( v13 != v14 )
  {
    if ( v14 == 1 && v13 == 2 )
    {
      if ( a11 )
      {
        sub_417EC0(a1, a2, a3, a5, a6, a7, a8, a9);
        return 0;
      }
      sub_418370(a2, a3, a5, a6, a7, a8, a9);
    }
    return 0;
  }
  if ( (unsigned int)(v14 - 1) > 1 )
    return 0;
  if ( a10 >= 0x100 )
  {
    sub_40A620(a1, 0);
    return 0;
  }
  else if ( a10 )
  {
    if ( a11 )
      sub_4179A0(a1, a2, a3, a5, a6, a7, a8, a9, a10);
    else
      sub_418160(a2, a3, a5, a6, a7, a8, a9, a10);
    return 0;
  }
  else
  {
    if ( a11 )
      sub_417C50(a1, a2, a3, a5, a6, a7, a8, a9);
    else
      sub_418280(a2, a3, a5, a6, a7, a8, a9);
    return 0;
  }
}

// ===== sub_4179A0 @ 0x004179A0..0x00417C47 =====
unsigned int __cdecl sub_4179A0(
        _DWORD *a1,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9)
{
  int *v9; // ecx
  int v10; // eax
  int v11; // edi
  unsigned int v12; // edx
  unsigned int v13; // eax
  int v14; // ecx
  int v15; // esi
  __int64 v16; // rax
  int v17; // ecx
  _DWORD *v18; // edi
  int v19; // esi
  int v20; // ecx
  __m64 v21; // mm7
  __m64 v22; // mm0
  int v23; // edx
  int v24; // ebx
  int v25; // edx
  unsigned int result; // eax
  __m64 v27; // mm4
  __m64 v28; // mm4
  __m64 v29; // mm0
  __m64 v30; // mm1
  unsigned int v31; // eax
  __m64 v32; // mm2
  __m64 v33; // mm3
  int v34; // ebx
  unsigned int v35; // eax
  int v36; // edx
  __m64 v37; // mm4
  __m64 v38; // mm3
  __m64 v39; // mm2
  _DWORD *v40; // [esp-8h] [ebp-F4h]
  int v41; // [esp-4h] [ebp-F0h]
  __m64 v42; // [esp+Ch] [ebp-E0h] BYREF
  __m64 v43; // [esp+14h] [ebp-D8h] BYREF
  __m64 v44; // [esp+1Ch] [ebp-D0h]
  __m64 v45; // [esp+24h] [ebp-C8h]
  __m64 v46; // [esp+2Ch] [ebp-C0h]
  int v47; // [esp+38h] [ebp-B4h]
  int v48; // [esp+3Ch] [ebp-B0h]
  unsigned int v49; // [esp+40h] [ebp-ACh]
  int v50; // [esp+44h] [ebp-A8h]
  int v51; // [esp+48h] [ebp-A4h]
  _DWORD *v52; // [esp+4Ch] [ebp-A0h]
  unsigned int v53; // [esp+50h] [ebp-9Ch]
  __m64 v54; // [esp+54h] [ebp-98h] BYREF
  int v55; // [esp+60h] [ebp-8Ch]
  _DWORD v56[33]; // [esp+64h] [ebp-88h]

  v52 = (_DWORD *)*a1;
  v50 = *v9;
  v51 = a1[1];
  v10 = a1[3];
  v11 = v9[1];
  v47 = a1[2];
  v12 = v9[2];
  v55 = v10;
  v13 = v9[3];
  v53 = v12;
  v49 = v13;
  v48 = v11;
  sub_416750(&v43, &v42, &v54, a2, a3, a4, a5, a6, a7, a8);
  v44.m64_u64 = (v11 << 14) | 1;
  v45.m64_u64 = 0xF0000000F000LL;
  v14 = 0;
  v15 = 16;
  do
  {
    v16 = v15 << 8;
    v56[2 * v14] = v16 | ((v15 | ((v15 | (v15 << 16)) << 16)) << 24);
    v56[2 * v14++ + 1] = HIDWORD(v16) | ((v15 | ((v15 | (unsigned __int64)((__int64)v15 << 16)) << 16)) >> 8);
    --v15;
  }
  while ( v14 < 16 );
  v46.m64_u64 = 0x100010001LL * (unsigned int)(256 - a9) + 0x100000000000000LL;
  v17 = v55;
  v18 = v52;
  v19 = v50;
  do
  {
    v41 = v17;
    v40 = v18;
    v20 = v47;
    v21 = v54;
    v54 = _m_paddd(v42, v54);
    do
    {
      v22 = _m_packssdw(_m_psradi(v21, 0x10u), 0LL);
      v23 = _mm_cvtsi64_si32(v22);
      v24 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v22, v44));
      LOWORD(result) = v23;
      v25 = v23 >> 16;
      result = (__int16)result;
      v27 = v21;
      v21 = _m_paddd(v21, v43);
      v28 = _m_packssdw(_m_psrlwi(_m_pand(v27, v45), 9u), 0LL);
      v29.m64_u64 = 0LL;
      v30.m64_u64 = 0LL;
      if ( v25 > -2 && v25 < (int)v49 )
      {
        if ( v25 < v49 )
        {
          if ( (__int16)result <= -2 || (__int16)result >= (int)v53 )
            goto LABEL_21;
          if ( (__int16)result < v53 )
            v29 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v24));
          v31 = (__int16)result + 1;
          if ( v31 < v53 )
            v30 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v24 + 4));
          v29 = _m_punpcklbw(v29, 0LL);
          v30 = _m_punpcklbw(v30, 0LL);
          result = v31 - 1;
        }
        v32.m64_u64 = 0LL;
        v33.m64_u64 = 0LL;
        if ( v25 + 1 < v49 )
        {
          v34 = v48 + v24;
          if ( result < v53 )
            v32 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v34));
          if ( result + 1 < v53 )
            v33 = _mm_cvtsi32_si64(*(_DWORD *)(v19 + v34 + 4));
        }
        v35 = _mm_cvtsi64_si32(v28);
        v36 = (unsigned __int16)v35;
        result = HIWORD(v35);
        v37 = *(__m64 *)((char *)v56 + v36);
        v38 = _m_punpcklbw(v33, 0LL);
        v39 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v32, 0LL), v38), 4u), v37), v38);
        v29 = _m_packuswb(
                _m_psrlwi(
                  _m_pmullw(
                    _m_paddw(
                      _m_pmulhw(
                        _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v29, v30), 4u), v37), v30), v39), 4u),
                        *(__m64 *)((char *)v56 + result)),
                      v39),
                    v46),
                  8u),
                0LL);
      }
LABEL_21:
      *v18++ = _mm_cvtsi64_si32(v29);
      --v20;
    }
    while ( v20 );
    v18 = (_DWORD *)((char *)v40 + v51);
    v17 = v41 - 1;
  }
  while ( v41 != 1 );
  _m_empty();
  return result;
}

// ===== sub_417C50 @ 0x00417C50..0x00417EC0 =====
unsigned int __cdecl sub_417C50(_DWORD *a1, int a2, int a3, int a4, int a5, int a6, unsigned int a7, unsigned int a8)
{
  int *v8; // ecx
  int v9; // eax
  int v10; // edi
  unsigned int v11; // edx
  unsigned int v12; // eax
  int v13; // ecx
  int v14; // esi
  __int64 v15; // rax
  int v16; // ecx
  _DWORD *v17; // edi
  int v18; // esi
  int v19; // ecx
  __m64 v20; // mm7
  __m64 v21; // mm0
  int v22; // edx
  int v23; // ebx
  int v24; // edx
  unsigned int result; // eax
  __m64 v26; // mm4
  __m64 v27; // mm4
  __m64 v28; // mm0
  __m64 v29; // mm1
  unsigned int v30; // eax
  __m64 v31; // mm2
  __m64 v32; // mm3
  int v33; // ebx
  unsigned int v34; // eax
  int v35; // edx
  __m64 v36; // mm4
  __m64 v37; // mm3
  __m64 v38; // mm2
  _DWORD *v39; // [esp-8h] [ebp-E8h]
  int v40; // [esp-4h] [ebp-E4h]
  __m64 v41; // [esp+10h] [ebp-D0h] BYREF
  __m64 v42; // [esp+18h] [ebp-C8h] BYREF
  __m64 v43; // [esp+20h] [ebp-C0h]
  __m64 v44; // [esp+28h] [ebp-B8h]
  int v45; // [esp+30h] [ebp-B0h]
  int v46; // [esp+34h] [ebp-ACh]
  int v47; // [esp+38h] [ebp-A8h]
  int v48; // [esp+3Ch] [ebp-A4h]
  unsigned int v49; // [esp+40h] [ebp-A0h]
  unsigned int v50; // [esp+44h] [ebp-9Ch]
  __m64 v51; // [esp+48h] [ebp-98h] BYREF
  int v52; // [esp+50h] [ebp-90h]
  _DWORD *v53; // [esp+54h] [ebp-8Ch]
  _DWORD v54[33]; // [esp+58h] [ebp-88h]

  v53 = (_DWORD *)*a1;
  v45 = *v8;
  v46 = a1[1];
  v9 = a1[3];
  v10 = v8[1];
  v47 = a1[2];
  v11 = v8[2];
  v52 = v9;
  v12 = v8[3];
  v49 = v11;
  v50 = v12;
  v48 = v10;
  sub_416750(&v41, &v42, &v51, a2, a3, a4, a5, a6, a7, a8);
  v43.m64_u64 = (v10 << 14) | 1;
  v44.m64_u64 = 0xF0000000F000LL;
  v13 = 0;
  v14 = 16;
  do
  {
    v15 = v14 << 8;
    v54[2 * v13] = v15 | ((v14 | ((v14 | (v14 << 16)) << 16)) << 24);
    v54[2 * v13++ + 1] = HIDWORD(v15) | ((v14 | ((v14 | (unsigned __int64)((__int64)v14 << 16)) << 16)) >> 8);
    --v14;
  }
  while ( v13 < 16 );
  v16 = v52;
  v17 = v53;
  v18 = v45;
  do
  {
    v40 = v16;
    v39 = v17;
    v19 = v47;
    v20 = v51;
    v51 = _m_paddd(v42, v51);
    do
    {
      v21 = _m_packssdw(_m_psradi(v20, 0x10u), 0LL);
      v22 = _mm_cvtsi64_si32(v21);
      v23 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v21, v43));
      LOWORD(result) = v22;
      v24 = v22 >> 16;
      result = (__int16)result;
      v26 = v20;
      v20 = _m_paddd(v20, v41);
      v27 = _m_packssdw(_m_psrlwi(_m_pand(v26, v44), 9u), 0LL);
      v28.m64_u64 = 0LL;
      v29.m64_u64 = 0LL;
      if ( v24 > -2 && v24 < (int)v50 )
      {
        if ( v24 < v50 )
        {
          if ( (__int16)result <= -2 || (__int16)result >= (int)v49 )
            goto LABEL_21;
          if ( (__int16)result < v49 )
            v28 = _mm_cvtsi32_si64(*(_DWORD *)(v18 + v23));
          v30 = (__int16)result + 1;
          if ( v30 < v49 )
            v29 = _mm_cvtsi32_si64(*(_DWORD *)(v18 + v23 + 4));
          v28 = _m_punpcklbw(v28, 0LL);
          v29 = _m_punpcklbw(v29, 0LL);
          result = v30 - 1;
        }
        v31.m64_u64 = 0LL;
        v32.m64_u64 = 0LL;
        if ( v24 + 1 < v50 )
        {
          v33 = v48 + v23;
          if ( result < v49 )
            v31 = _mm_cvtsi32_si64(*(_DWORD *)(v18 + v33));
          if ( result + 1 < v49 )
            v32 = _mm_cvtsi32_si64(*(_DWORD *)(v18 + v33 + 4));
        }
        v34 = _mm_cvtsi64_si32(v27);
        v35 = (unsigned __int16)v34;
        result = HIWORD(v34);
        v36 = *(__m64 *)((char *)v54 + v35);
        v37 = _m_punpcklbw(v32, 0LL);
        v38 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v31, 0LL), v37), 4u), v36), v37);
        v28 = _m_packuswb(
                _m_paddw(
                  _m_pmulhw(
                    _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v28, v29), 4u), v36), v29), v38), 4u),
                    *(__m64 *)((char *)v54 + result)),
                  v38),
                0LL);
      }
LABEL_21:
      *v17++ = _mm_cvtsi64_si32(v28);
      --v19;
    }
    while ( v19 );
    v17 = (_DWORD *)((char *)v39 + v46);
    v16 = v40 - 1;
  }
  while ( v40 != 1 );
  _m_empty();
  return result;
}

// ===== sub_417EC0 @ 0x00417EC0..0x0041815C =====
unsigned int __cdecl sub_417EC0(_DWORD *a1, int a2, int a3, int a4, int a5, int a6, unsigned int a7, unsigned int a8)
{
  int *v8; // ecx
  int v9; // eax
  int v10; // edi
  unsigned int v11; // edx
  unsigned int v12; // eax
  int v13; // ecx
  int v14; // esi
  __int64 v15; // rax
  int v16; // ecx
  _DWORD *v17; // edi
  int v18; // esi
  int v19; // ecx
  __m64 v20; // mm7
  __m64 v21; // mm0
  int v22; // edx
  int v23; // ebx
  int v24; // edx
  unsigned int result; // eax
  __m64 v26; // mm4
  __m64 v27; // mm4
  __m64 v28; // mm0
  __m64 v29; // mm1
  unsigned int v30; // eax
  __m64 v31; // mm2
  __m64 v32; // mm3
  int v33; // ebx
  unsigned int v34; // eax
  int v35; // edx
  __m64 v36; // mm4
  __m64 v37; // mm3
  __m64 v38; // mm2
  _DWORD *v39; // [esp-8h] [ebp-F8h]
  int v40; // [esp-4h] [ebp-F4h]
  __m64 v41; // [esp+10h] [ebp-E0h] BYREF
  __m64 v42; // [esp+18h] [ebp-D8h] BYREF
  __m64 v43; // [esp+20h] [ebp-D0h]
  __m64 v44; // [esp+28h] [ebp-C8h]
  __m64 v45; // [esp+30h] [ebp-C0h]
  int v46; // [esp+3Ch] [ebp-B4h]
  int v47; // [esp+40h] [ebp-B0h]
  unsigned int v48; // [esp+44h] [ebp-ACh]
  int v49; // [esp+48h] [ebp-A8h]
  int v50; // [esp+4Ch] [ebp-A4h]
  _DWORD *v51; // [esp+50h] [ebp-A0h]
  unsigned int v52; // [esp+54h] [ebp-9Ch]
  __m64 v53; // [esp+58h] [ebp-98h] BYREF
  int v54; // [esp+64h] [ebp-8Ch]
  _DWORD v55[33]; // [esp+68h] [ebp-88h]

  v51 = (_DWORD *)*a1;
  v49 = *v8;
  v50 = a1[1];
  v9 = a1[3];
  v10 = v8[1];
  v46 = a1[2];
  v11 = v8[2];
  v54 = v9;
  v12 = v8[3];
  v52 = v11;
  v48 = v12;
  v47 = v10;
  sub_416750(&v42, &v41, &v53, a2, a3, a4, a5, a6, a7, a8);
  v43.m64_u64 = (v10 << 14) | 1;
  v13 = 0;
  v44.m64_u64 = 0xF0000000F000LL;
  v45.m64_u64 = 4278190080LL;
  v14 = 16;
  do
  {
    v15 = v14 << 8;
    v55[2 * v13] = v15 | ((v14 | ((v14 | (v14 << 16)) << 16)) << 24);
    v55[2 * v13++ + 1] = HIDWORD(v15) | ((v14 | ((v14 | (unsigned __int64)((__int64)v14 << 16)) << 16)) >> 8);
    --v14;
  }
  while ( v13 < 16 );
  v16 = v54;
  v17 = v51;
  v18 = v49;
  do
  {
    v40 = v16;
    v39 = v17;
    v19 = v46;
    v20 = v53;
    v53 = _m_paddd(v41, v53);
    do
    {
      v21 = _m_packssdw(_m_psradi(v20, 0x10u), 0LL);
      v22 = _mm_cvtsi64_si32(v21);
      v23 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v21, v43));
      LOWORD(result) = v22;
      v24 = v22 >> 16;
      result = (__int16)result;
      v26 = v20;
      v20 = _m_paddd(v20, v42);
      v27 = _m_packssdw(_m_psrlwi(_m_pand(v26, v44), 9u), 0LL);
      v28.m64_u64 = 0LL;
      v29.m64_u64 = 0LL;
      if ( v24 > -2 && v24 < (int)v48 )
      {
        if ( v24 < v48 )
        {
          if ( (__int16)result <= -2 || (__int16)result >= (int)v52 )
            goto LABEL_21;
          if ( (__int16)result < v52 )
            v28 = _m_por(_mm_cvtsi32_si64(*(_DWORD *)(v18 + v23)), v45);
          v30 = (__int16)result + 1;
          if ( v30 < v52 )
            v29 = _m_por(_mm_cvtsi32_si64(*(_DWORD *)(v18 + v23 + 4)), v45);
          v28 = _m_punpcklbw(v28, 0LL);
          v29 = _m_punpcklbw(v29, 0LL);
          result = v30 - 1;
        }
        v31.m64_u64 = 0LL;
        v32.m64_u64 = 0LL;
        if ( v24 + 1 < v48 )
        {
          v33 = v47 + v23;
          if ( result < v52 )
            v31 = _m_por(_mm_cvtsi32_si64(*(_DWORD *)(v18 + v33)), v45);
          if ( result + 1 < v52 )
            v32 = _m_por(_mm_cvtsi32_si64(*(_DWORD *)(v18 + v33 + 4)), v45);
        }
        v34 = _mm_cvtsi64_si32(v27);
        v35 = (unsigned __int16)v34;
        result = HIWORD(v34);
        v36 = *(__m64 *)((char *)v55 + v35);
        v37 = _m_punpcklbw(v32, 0LL);
        v38 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v31, 0LL), v37), 4u), v36), v37);
        v28 = _m_packuswb(
                _m_paddw(
                  _m_pmulhw(
                    _m_psllwi(_m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v28, v29), 4u), v36), v29), v38), 4u),
                    *(__m64 *)((char *)v55 + result)),
                  v38),
                0LL);
      }
LABEL_21:
      *v17++ = _mm_cvtsi64_si32(v28);
      --v19;
    }
    while ( v19 );
    v17 = (_DWORD *)((char *)v39 + v50);
    v16 = v40 - 1;
  }
  while ( v40 != 1 );
  _m_empty();
  return result;
}
