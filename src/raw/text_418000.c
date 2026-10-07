#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_418160 @ 0x00418160..0x0041827E =====
int __usercall sub_418160@<eax>(
        _DWORD *a1@<eax>,
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
  int v12; // ecx
  int v13; // edx
  int v14; // eax
  unsigned int v15; // ecx
  unsigned int v16; // edx
  __m64 v17; // mm4
  __m64 v18; // mm5
  int v19; // ecx
  _DWORD *v20; // edi
  int v21; // esi
  int v22; // ecx
  __m64 v23; // mm7
  __m64 v24; // mm0
  __m64 v25; // mm0
  int v26; // edx
  int v27; // ebx
  int result; // eax
  __m64 v29; // mm0
  _DWORD *v30; // [esp-8h] [ebp-60h]
  int v31; // [esp-4h] [ebp-5Ch]
  __m64 v32; // [esp+10h] [ebp-48h] BYREF
  __m64 v33[2]; // [esp+18h] [ebp-40h] BYREF
  __m64 v34; // [esp+28h] [ebp-30h]
  __m64 v35; // [esp+30h] [ebp-28h] BYREF
  int v36; // [esp+3Ch] [ebp-1Ch]
  _DWORD *v37; // [esp+40h] [ebp-18h]
  int v38; // [esp+44h] [ebp-14h]
  int v39; // [esp+48h] [ebp-10h]
  unsigned int v40; // [esp+4Ch] [ebp-Ch]
  unsigned int v41; // [esp+50h] [ebp-8h]
  int v42; // [esp+54h] [ebp-4h]

  v11 = *a2;
  v37 = (_DWORD *)*a1;
  v12 = a1[1];
  v38 = v11;
  v13 = a1[2];
  v14 = a1[3];
  v42 = v12;
  v15 = a2[2];
  v36 = v14;
  v39 = v13;
  v16 = a2[3];
  v41 = v15;
  v40 = v16;
  sub_416750(&v32, v33, &v35, a3, a4, a5, a6, a7, a8, a9);
  v34.m64_u64 = (a2[1] << 14) | 1;
  v33[1].m64_u64 = 0x100010001LL * (unsigned int)(256 - a10) + 0x100000000000000LL;
  v17 = v32;
  v18 = v34;
  v19 = v36;
  v20 = v37;
  v21 = v38;
  do
  {
    v31 = v19;
    v30 = v20;
    v22 = v39;
    v23 = v35;
    v35 = _m_paddd(v33[0], v35);
    do
    {
      v24 = v23;
      v23 = _m_paddd(v23, v17);
      v25 = _m_packssdw(_m_psradi(v24, 0x10u), 0LL);
      v26 = _mm_cvtsi64_si32(v25);
      v27 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v25, v18));
      result = (__int16)v26;
      v29.m64_u64 = 0LL;
      if ( v26 >> 16 < v40 && (__int16)v26 < v41 )
        v29 = _m_packuswb(
                _m_psrlwi(
                  _m_pmullw(
                    _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v21 + v27)), 0LL),
                    (__m64)(0x100010001LL * (unsigned int)(256 - a10) + 0x100000000000000LL)),
                  8u),
                0LL);
      *v20++ = _mm_cvtsi64_si32(v29);
      --v22;
    }
    while ( v22 );
    v20 = (_DWORD *)((char *)v30 + v42);
    v19 = v31 - 1;
  }
  while ( v31 != 1 );
  _m_empty();
  return result;
}

// ===== sub_418280 @ 0x00418280..0x00418367 =====
int __usercall sub_418280@<eax>(
        _DWORD *a1@<eax>,
        int *a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9)
{
  int v10; // edx
  int v11; // ecx
  int v12; // edx
  int v13; // eax
  unsigned int v14; // ecx
  unsigned int v15; // edx
  __m64 v16; // mm4
  __m64 v17; // mm5
  int v18; // ecx
  _DWORD *v19; // edi
  int v20; // esi
  int v21; // ecx
  __m64 v22; // mm7
  __m64 v23; // mm0
  __m64 v24; // mm0
  int v25; // edx
  int v26; // ebx
  int result; // eax
  __m64 v28; // mm0
  _DWORD *v29; // [esp-8h] [ebp-54h]
  int v30; // [esp-4h] [ebp-50h]
  __m64 v31; // [esp+Ch] [ebp-40h] BYREF
  __m64 v32; // [esp+14h] [ebp-38h] BYREF
  __m64 v33; // [esp+1Ch] [ebp-30h]
  __m64 v34; // [esp+24h] [ebp-28h] BYREF
  int v35; // [esp+30h] [ebp-1Ch]
  _DWORD *v36; // [esp+34h] [ebp-18h]
  int v37; // [esp+38h] [ebp-14h]
  int v38; // [esp+3Ch] [ebp-10h]
  unsigned int v39; // [esp+40h] [ebp-Ch]
  unsigned int v40; // [esp+44h] [ebp-8h]
  int v41; // [esp+48h] [ebp-4h]

  v10 = *a2;
  v36 = (_DWORD *)*a1;
  v11 = a1[1];
  v37 = v10;
  v12 = a1[2];
  v13 = a1[3];
  v41 = v11;
  v14 = a2[2];
  v35 = v13;
  v38 = v12;
  v15 = a2[3];
  v40 = v14;
  v39 = v15;
  sub_416750(&v31, &v32, &v34, a3, a4, a5, a6, a7, a8, a9);
  v33.m64_u64 = (a2[1] << 14) | 1;
  v16 = v31;
  v17 = v33;
  v18 = v35;
  v19 = v36;
  v20 = v37;
  do
  {
    v30 = v18;
    v29 = v19;
    v21 = v38;
    v22 = v34;
    v34 = _m_paddd(v32, v34);
    do
    {
      v23 = v22;
      v22 = _m_paddd(v22, v16);
      v24 = _m_packssdw(_m_psradi(v23, 0x10u), 0LL);
      v25 = _mm_cvtsi64_si32(v24);
      v26 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v24, v17));
      result = (__int16)v25;
      v28.m64_u64 = 0LL;
      if ( v25 >> 16 < v39 && (__int16)v25 < v40 )
        v28 = _mm_cvtsi32_si64(*(_DWORD *)(v20 + v26));
      *v19++ = _mm_cvtsi64_si32(v28);
      --v21;
    }
    while ( v21 );
    v19 = (_DWORD *)((char *)v29 + v41);
    v18 = v30 - 1;
  }
  while ( v30 != 1 );
  _m_empty();
  return result;
}

// ===== sub_418370 @ 0x00418370..0x00418462 =====
int __usercall sub_418370@<eax>(
        _DWORD *a1@<eax>,
        int *a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        unsigned int a8,
        unsigned int a9)
{
  int v10; // edx
  int v11; // ecx
  int v12; // edx
  int v13; // eax
  unsigned int v14; // ecx
  unsigned int v15; // edx
  __m64 v16; // mm3
  __m64 v17; // mm4
  __m64 v18; // mm5
  int v19; // ecx
  _DWORD *v20; // edi
  int v21; // esi
  int v22; // ecx
  __m64 v23; // mm7
  __m64 v24; // mm0
  __m64 v25; // mm0
  int v26; // edx
  int v27; // ebx
  int result; // eax
  __m64 v29; // mm0
  _DWORD *v30; // [esp-8h] [ebp-54h]
  int v31; // [esp-4h] [ebp-50h]
  __m64 v32; // [esp+Ch] [ebp-40h] BYREF
  __m64 v33; // [esp+14h] [ebp-38h] BYREF
  __m64 v34; // [esp+1Ch] [ebp-30h]
  __m64 v35; // [esp+24h] [ebp-28h] BYREF
  int v36; // [esp+30h] [ebp-1Ch]
  _DWORD *v37; // [esp+34h] [ebp-18h]
  int v38; // [esp+38h] [ebp-14h]
  int v39; // [esp+3Ch] [ebp-10h]
  unsigned int v40; // [esp+40h] [ebp-Ch]
  unsigned int v41; // [esp+44h] [ebp-8h]
  int v42; // [esp+48h] [ebp-4h]

  v10 = *a2;
  v37 = (_DWORD *)*a1;
  v11 = a1[1];
  v38 = v10;
  v12 = a1[2];
  v13 = a1[3];
  v42 = v11;
  v14 = a2[2];
  v36 = v13;
  v39 = v12;
  v15 = a2[3];
  v41 = v14;
  v40 = v15;
  sub_416750(&v32, &v33, &v35, a3, a4, a5, a6, a7, a8, a9);
  v34.m64_u64 = (a2[1] << 14) | 1;
  v16 = _mm_cvtsi32_si64(0xFF000000);
  v17 = v32;
  v18 = v34;
  v19 = v36;
  v20 = v37;
  v21 = v38;
  do
  {
    v31 = v19;
    v30 = v20;
    v22 = v39;
    v23 = v35;
    v35 = _m_paddd(v33, v35);
    do
    {
      v24 = v23;
      v23 = _m_paddd(v23, v17);
      v25 = _m_packssdw(_m_psradi(v24, 0x10u), 0LL);
      v26 = _mm_cvtsi64_si32(v25);
      v27 = 4 * _mm_cvtsi64_si32(_m_pmaddwd(v25, v18));
      result = (__int16)v26;
      v29.m64_u64 = 0LL;
      if ( v26 >> 16 < v40 && (__int16)v26 < v41 )
        v29 = _m_por(_mm_cvtsi32_si64(*(_DWORD *)(v21 + v27)), v16);
      *v20++ = _mm_cvtsi64_si32(v29);
      --v22;
    }
    while ( v22 );
    v20 = (_DWORD *)((char *)v30 + v42);
    v19 = v31 - 1;
  }
  while ( v31 != 1 );
  _m_empty();
  return result;
}

// ===== sub_418470 @ 0x00418470..0x0041849A =====
int __fastcall sub_418470(int a1, int a2, int a3, int a4, int a5)
{
  int result; // eax
  bool v6; // cf

  result = *(_DWORD *)(a1 + 16);
  if ( *(_DWORD *)(a2 + 16) == result )
  {
    v6 = result-- == 1;
    if ( v6 || result == 1 )
      return sub_4184A0(a2, a3, a4, a5);
  }
  return result;
}

// ===== sub_4184A0 @ 0x004184A0..0x004187F6 =====
void __cdecl sub_4184A0(__m64 **a1, unsigned int a2, int a3, int a4)
{
  unsigned int **v4; // ecx
  unsigned int **v5; // edi
  int v6; // eax
  int v7; // ecx
  unsigned __int64 v8; // rt0
  __int64 v9; // rax
  unsigned int v10; // esi
  long double v11; // st7
  __m64 *v12; // ecx
  __m64 *v13; // edi
  unsigned int *v14; // esi
  int *v15; // ebx
  unsigned int v16; // ecx
  int v17; // ebx
  __int16 v18; // ax
  unsigned int v19; // ebx
  __m64 v20; // mm6
  __m64 v21; // mm0
  unsigned int v22; // ebx
  __m64 v23; // mm1
  unsigned int v24; // ebx
  __m64 v25; // mm3
  __m64 v26; // mm4
  __m64 v27; // mm1
  __m64 *v28; // ecx
  __m64 *v29; // edi
  unsigned int *v30; // esi
  int *v31; // ebx
  unsigned int v32; // ecx
  int v33; // ebx
  __int16 v34; // ax
  unsigned int v35; // ebx
  __m64 v36; // mm6
  __m64 v37; // mm2
  unsigned int v38; // ebx
  __m64 v39; // mm3
  __m64 v40; // mm0
  int *v41; // [esp-10h] [ebp-D8h]
  int *v42; // [esp-10h] [ebp-D8h]
  unsigned int *v43; // [esp-Ch] [ebp-D4h]
  unsigned int *v44; // [esp-Ch] [ebp-D4h]
  __m64 *v45; // [esp-8h] [ebp-D0h]
  __m64 *v46; // [esp-8h] [ebp-D0h]
  __m64 *v47; // [esp-4h] [ebp-CCh]
  __m64 *v48; // [esp-4h] [ebp-CCh]
  unsigned int *v49; // [esp+18h] [ebp-B0h]
  __m64 *v50; // [esp+1Ch] [ebp-ACh]
  unsigned int v51; // [esp+20h] [ebp-A8h]
  __m64 *v52; // [esp+24h] [ebp-A4h]
  unsigned int *v53; // [esp+28h] [ebp-A0h]
  unsigned int v54; // [esp+2Ch] [ebp-9Ch]
  __m64 *v55; // [esp+30h] [ebp-98h]
  unsigned int v56; // [esp+38h] [ebp-90h]
  int v57; // [esp+38h] [ebp-90h]
  int v58; // [esp+3Ch] [ebp-8Ch]
  int *v59; // [esp+3Ch] [ebp-8Ch]
  _DWORD v60[33]; // [esp+40h] [ebp-88h]

  v5 = v4;
  v50 = *a1;
  v52 = a1[1];
  v55 = a1[3];
  v53 = *v4;
  v51 = (unsigned int)v4[2];
  v6 = 16;
  v56 = (unsigned int)a1[2];
  v54 = v56;
  v49 = v4[1];
  v7 = 0;
  v58 = 16;
  do
  {
    v8 = v6 | ((v6 | (unsigned __int64)((__int64)v6 << 16)) << 16);
    v9 = v58 << 8;
    v60[2 * v7] = v9 | ((_DWORD)v8 << 24);
    v60[2 * v7++ + 1] = HIDWORD(v9) | (v8 >> 8);
    v6 = --v58;
  }
  while ( v7 < 16 );
  v57 = ((_DWORD)v5[2] - v56) << 15;
  v10 = 0;
  v59 = (int *)operator new[](4 * (_DWORD)v55);
  if ( v55 )
  {
    do
    {
      v11 = sin((double)(v10 + a3) * 6.283185307179586 / (double)a2) * (double)(unsigned int)(a4 * (_DWORD)v5[2]);
      v59[v10++] = v57 + ((int)v11 >> 1);
    }
    while ( v10 < (unsigned int)v55 );
  }
  if ( dword_565AFC && v54 >= 8 )
  {
    v12 = v55;
    v13 = v50;
    v14 = v53;
    v15 = v59;
    do
    {
      v47 = v12;
      v45 = v13;
      v43 = v14;
      v41 = v15;
      v16 = v54 >> 1;
      v17 = *v15;
      v18 = v17;
      v19 = v17 >> 16;
      v20 = *(__m64 *)((char *)v60 + ((unsigned __int16)(v18 & 0xF000) >> 9));
      v21.m64_u64 = 0LL;
      if ( v19 < v51 )
        v21 = _m_punpcklbw(_mm_cvtsi32_si64(*v14++), 0LL);
      v22 = v19 + 1;
      do
      {
        v23.m64_u64 = 0LL;
        if ( v22 < v51 )
          v23 = _m_punpcklbw(_mm_cvtsi32_si64(*v14++), 0LL);
        v24 = v22 + 1;
        v25.m64_u64 = 0LL;
        if ( v24 < v51 )
          v25 = _m_punpcklbw(_mm_cvtsi32_si64(*v14++), 0LL);
        v22 = v24 + 1;
        v26 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v21, v23), 4u), v20), v23);
        v21 = v25;
        _mm_stream_pi(v13++, _m_packuswb(v26, _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v23, v25), 4u), v20), v25)));
        --v16;
      }
      while ( v16 );
      if ( (v54 & 1) != 0 )
      {
        v27.m64_u64 = 0LL;
        if ( v22 + 1 < v51 )
          v27 = _m_punpcklbw(_mm_cvtsi32_si64(*v14), 0LL);
        v13->m64_i32[0] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v25, v27), 4u), v20), v27), 0LL));
      }
      v15 = v41 + 1;
      v14 = (unsigned int *)((char *)v43 + (_DWORD)v49);
      v13 = (__m64 *)((char *)v45 + (_DWORD)v52);
      v12 = (__m64 *)((char *)v47 - 1);
    }
    while ( v47 != (__m64 *)1 );
  }
  else
  {
    v28 = v55;
    v29 = v50;
    v30 = v53;
    v31 = v59;
    do
    {
      v48 = v28;
      v46 = v29;
      v44 = v30;
      v42 = v31;
      v32 = v54;
      v33 = *v31;
      v34 = v33;
      v35 = v33 >> 16;
      v36 = *(__m64 *)((char *)v60 + ((unsigned __int16)(v34 & 0xF000) >> 9));
      v37.m64_u64 = 0LL;
      if ( v35 < v51 )
        v37 = _m_punpcklbw(_mm_cvtsi32_si64(*v30++), 0LL);
      v38 = v35 + 1;
      do
      {
        v39.m64_u64 = 0LL;
        if ( v38 < v51 )
          v39 = _m_punpcklbw(_mm_cvtsi32_si64(*v30++), 0LL);
        ++v38;
        v40 = _m_packuswb(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v37, v39), 4u), v36), v39), 0LL);
        v37 = v39;
        v29->m64_i32[0] = _mm_cvtsi64_si32(v40);
        v29 = (__m64 *)((char *)v29 + 4);
        --v32;
      }
      while ( v32 );
      v31 = v42 + 1;
      v30 = (unsigned int *)((char *)v44 + (_DWORD)v49);
      v29 = (__m64 *)((char *)v46 + (_DWORD)v52);
      v28 = (__m64 *)((char *)v48 - 1);
    }
    while ( v48 != (__m64 *)1 );
  }
  _m_empty();
  operator delete[](v59);
}

// ===== sub_418800 @ 0x00418800..0x0041882B =====
size_t __usercall sub_418800@<eax>(size_t result@<eax>, size_t *a2@<edx>, int a3@<esi>)
{
  int v3; // ecx

  if ( !a3 )
    return sub_40AF50(result, a2);
  v3 = *(_DWORD *)(result + 16);
  if ( a2[4] == v3 && (unsigned int)(v3 - 1) <= 1 )
    return sub_418830(a3);
  return result;
}

// ===== sub_418830 @ 0x00418830..0x004188C3 =====
int __usercall sub_418830@<eax>(int *a1@<eax>, int *a2@<ecx>, int a3)
{
  int result; // eax
  int v4; // ecx
  int v5; // edi
  int v6; // esi
  int v7; // edx
  int v8; // ebx
  int v9; // [esp+14h] [ebp-18h]
  int v10; // [esp+18h] [ebp-14h]
  int v11; // [esp+1Ch] [ebp-10h]
  int v12; // [esp+20h] [ebp-Ch]
  int v13; // [esp+24h] [ebp-8h]
  int v14; // [esp+28h] [ebp-4h]

  v10 = *a1;
  v11 = *a2;
  v14 = a2[1];
  v13 = a1[1];
  v12 = a1[2];
  v9 = a1[3];
  result = 65537 * (256 - a3);
  v4 = v9;
  v5 = v10;
  v6 = v11;
  do
  {
    v7 = v12;
    v8 = 0;
    do
    {
      *(_DWORD *)(v5 + v8) = _mm_cvtsi64_si32(
                               _m_packuswb(
                                 _m_psrlwi(
                                   _m_pmullw(
                                     _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v6 + v8)), 0LL),
                                     (__m64)(0x100010001LL * (unsigned int)(256 - a3) + 0x100000000000000LL)),
                                   8u),
                                 0LL));
      v8 += 4;
      --v7;
    }
    while ( v7 );
    v5 += v13;
    v6 += v14;
    --v4;
  }
  while ( v4 );
  _m_empty();
  return result;
}

// ===== sub_4188D0 @ 0x004188D0..0x0041895F =====
int *__usercall sub_4188D0@<eax>(int *result@<eax>, int **a2@<ecx>, int a3)
{
  int v3; // edx
  int v4; // esi
  int *v5; // edi
  int v6; // ecx
  int v7; // edx
  unsigned int v8; // ebx
  int v9; // [esp+0h] [ebp-10h]
  int *v10; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  int v12; // [esp+Ch] [ebp-4h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF
  int v14; // [esp+18h] [ebp+8h]

  if ( result[4] == 2 && a2[4] == (int *)2 )
  {
    savedregs = (int)&savedregs;
    v3 = *result;
    v14 = a3 & 0xFFFFFF;
    v4 = result[3];
    v5 = *a2;
    v10 = a2[1];
    v9 = result[1];
    v6 = result[2];
    v12 = *result;
    for ( i = v6; v4; v12 += v9 )
    {
      --v4;
      result = v5;
      if ( v6 )
      {
        v7 = v3 - (_DWORD)v5;
        do
        {
          v8 = v14 | *(int *)((char *)result++ + v7) & 0xFF000000;
          --v6;
          *(result - 1) = v8;
        }
        while ( v6 );
        v6 = i;
      }
      v3 = v9 + v12;
      v5 = (int *)((char *)v5 + (_DWORD)v10);
    }
  }
  return result;
}

// ===== sub_418960 @ 0x00418960..0x00418B3F =====
int __usercall sub_418960@<eax>(int *a1@<eax>, _DWORD *a2@<edi>, int a3, int a4, int *a5, int a6)
{
  int v6; // edx
  int v7; // ecx
  int v8; // edx
  int v9; // ecx
  int v10; // edx
  int v11; // ecx
  int v12; // edx
  int v13; // ecx
  int v14; // edx
  int v15; // edx
  int v16; // ecx
  int v17; // edx
  int v18; // ecx
  int v19; // edx
  _DWORD *v20; // eax
  int *v21; // eax
  int result; // eax
  unsigned int v23; // ecx
  int v24; // ebx
  int *v25; // eax
  _DWORD v26[6]; // [esp+8h] [ebp-78h] BYREF
  _DWORD v27[6]; // [esp+20h] [ebp-60h] BYREF
  _DWORD v28[2]; // [esp+38h] [ebp-48h] BYREF
  int v29; // [esp+40h] [ebp-40h]
  int v30; // [esp+44h] [ebp-3Ch]
  int v31; // [esp+48h] [ebp-38h]
  int v32; // [esp+4Ch] [ebp-34h]
  int v33[4]; // [esp+50h] [ebp-30h] BYREF
  int v34[4]; // [esp+60h] [ebp-20h] BYREF
  int v35; // [esp+70h] [ebp-10h] BYREF
  int v36; // [esp+74h] [ebp-Ch]
  int v37; // [esp+78h] [ebp-8h]
  int v38; // [esp+7Ch] [ebp-4h]

  v6 = a2[1];
  v28[0] = *a2;
  v29 = a2[2];
  v7 = a2[4];
  v28[1] = v6;
  v8 = a2[3];
  v31 = v7;
  v9 = *a1;
  v30 = v8;
  v10 = a2[5];
  v26[0] = v9;
  v11 = a1[2];
  v32 = v10;
  v12 = a1[1];
  v26[2] = v11;
  v13 = a1[4];
  v26[1] = v12;
  v14 = a1[3];
  v26[4] = v13;
  v26[3] = v14;
  v26[5] = a1[5];
  if ( a5 )
    a1 = a5;
  v15 = a1[1];
  v27[0] = *a1;
  v16 = a1[2];
  v27[1] = v15;
  v17 = a1[3];
  v27[2] = v16;
  v18 = a1[4];
  v27[3] = v17;
  v19 = a1[5];
  v27[4] = v18;
  v27[5] = v19;
  v20 = sub_409190(&v35, (int)v28);
  sub_409170(-a4, -a3, v20);
  sub_409190(v34, (int)v26);
  v21 = sub_409190(v33, (int)v27);
  sub_409110(v34, v21);
  if ( sub_409110(v34, &v35) )
  {
    sub_4091B0(v26, v34);
    sub_4091B0(v27, v34);
    sub_409170(a4, a3, v34);
    sub_4091B0(v28, v34);
    result = sub_418B40(v28, a6);
    if ( !result )
    {
      if ( a4 > 0 )
      {
        sub_409190(&v35, (int)a2);
        v38 = a4 - 1;
        sub_40A620((int)a2, &v35);
      }
      if ( a3 > 0 )
      {
        v37 = a3 - 1;
        v38 = v30 + a4 - 1;
        v35 = 0;
        v36 = a4;
        sub_40A620((int)a2, &v35);
      }
      v23 = a2[2];
      if ( a3 + v29 < v23 )
      {
        v35 = a3 + v29;
        v37 = v23 - 1;
        v38 = v30 + a4 - 1;
        v36 = a4;
        sub_40A620((int)a2, &v35);
      }
      v24 = v30 + a4;
      if ( (unsigned int)(v30 + a4) < a2[3] )
      {
        v25 = sub_409190(&v35, (int)a2);
        v36 = v24;
        sub_40A620((int)a2, v25);
      }
      return 0;
    }
  }
  else
  {
    sub_40A620((int)a2, 0);
    return 0;
  }
  return result;
}

// ===== sub_418B40 @ 0x00418B40..0x00418B92 =====
int __fastcall sub_418B40(int a1, int a2, int a3, int a4)
{
  int v4; // esi

  if ( *(_DWORD *)(a3 + 16) != 3 )
    return 7;
  if ( a1 )
  {
    v4 = *(_DWORD *)(a2 + 16);
    if ( v4 == *(_DWORD *)(a1 + 16) )
    {
      if ( v4 == 2 )
        sub_418C10(a3, a4);
      return 0;
    }
    else
    {
      return 1;
    }
  }
  else
  {
    if ( *(_DWORD *)(a2 + 16) == 2 )
      sub_418BA0(a2);
    return 0;
  }
}

// ===== sub_418BA0 @ 0x00418BA0..0x00418C06 =====
_DWORD *__usercall sub_418BA0@<eax>(_DWORD *result@<eax>, int *a2@<ecx>)
{
  _BYTE *v2; // edx
  _DWORD *v3; // ebx
  int v4; // esi
  int v5; // edi
  _BYTE *v6; // ecx
  int v7; // [esp+Ch] [ebp-10h]
  int v8; // [esp+10h] [ebp-Ch]
  _DWORD *i; // [esp+14h] [ebp-8h]
  int v10; // [esp+18h] [ebp-4h]

  v2 = (_BYTE *)*result;
  v3 = (_DWORD *)result[2];
  v4 = *a2;
  v8 = result[1];
  v5 = result[3];
  v10 = *a2;
  v7 = a2[1];
  for ( i = v3; v5; v10 += v7 )
  {
    --v5;
    v6 = v2;
    result = v3;
    if ( v3 )
    {
      do
      {
        *v6++ = *(_BYTE *)(v4 + 3);
        v4 += 4;
        result = (_DWORD *)((char *)result - 1);
      }
      while ( result );
      v3 = i;
    }
    v4 = v7 + v10;
    v2 += v8;
  }
  return result;
}

// ===== sub_418C10 @ 0x00418C10..0x00418CA6 =====
int __fastcall sub_418C10(int *a1, int *a2, int a3, __int16 a4)
{
  int result; // eax
  int v5; // ebx
  int v6; // ecx
  _BYTE *v7; // edi
  int v8; // esi
  int v9; // [esp+Ch] [ebp-1Ch]
  int v10; // [esp+10h] [ebp-18h]
  int v11; // [esp+14h] [ebp-14h]
  int i; // [esp+1Ch] [ebp-Ch]
  int v13; // [esp+20h] [ebp-8h]
  _BYTE *v14; // [esp+24h] [ebp-4h]
  int v15; // [esp+30h] [ebp+8h]

  result = a3;
  v5 = *a2;
  v15 = *a1;
  v9 = a1[1];
  v6 = *(_DWORD *)(result + 12);
  v7 = *(_BYTE **)result;
  v11 = *(_DWORD *)(result + 4);
  v8 = *(_DWORD *)(result + 8);
  v14 = *(_BYTE **)result;
  v13 = *a2;
  v10 = a2[1];
  for ( i = v8; v6; v13 = v5 )
  {
    --v6;
    result = v5;
    if ( v8 )
    {
      do
      {
        *v7++ = *(_BYTE *)(result + 3)
              + ((unsigned __int16)(a4 * (*(unsigned __int8 *)(v15 - v5 + result + 3) - *(unsigned __int8 *)(result + 3))) >> 8);
        result += 4;
        --v8;
      }
      while ( v8 );
      v5 = v13;
      v8 = i;
    }
    v7 = &v14[v11];
    v5 += v10;
    v15 += v9;
    v14 += v11;
  }
  return result;
}

// ===== sub_418CB0 @ 0x00418CB0..0x00418CD7 =====
int __fastcall sub_418CB0(int a1, int a2, int a3, int a4)
{
  int result; // eax

  result = *(_DWORD *)(a2 + 16) - 1;
  if ( *(_DWORD *)(a2 + 16) == 1 )
  {
    result = *(_DWORD *)(a1 + 16) - 1;
    if ( *(_DWORD *)(a1 + 16) == 1 )
      return sub_418CE0(a1, a3, a4);
  }
  return result;
}

// ===== sub_418CE0 @ 0x00418CE0..0x00418F9E =====
__int16 __fastcall sub_418CE0(int a1, int *a2, unsigned int **a3, int a4, int a5)
{
  int v7; // edx
  int v8; // ecx
  unsigned int v9; // eax
  unsigned __int64 v10; // rt0
  unsigned int v11; // eax
  __m64 v12; // mm4
  unsigned int *v13; // esi
  unsigned int *v14; // ecx
  __m64 v15; // mm7
  unsigned int *v16; // ecx
  unsigned int *v17; // ebx
  __m64 v18; // mm2
  int v19; // edx
  unsigned int v20; // eax
  __m64 v21; // mm2
  __m64 v22; // mm1
  unsigned int v23; // edx
  __m64 v24; // mm2
  __m64 v25; // mm0
  __m64 v26; // mm0
  unsigned int v27; // edx
  __m64 v28; // mm0
  __m64 v29; // mm0
  unsigned int *v31; // [esp-8h] [ebp-464h]
  unsigned int *v32; // [esp-4h] [ebp-460h]
  __m64 v33; // [esp+Ch] [ebp-450h]
  __m64 v34; // [esp+24h] [ebp-438h]
  unsigned int *v35; // [esp+34h] [ebp-428h]
  unsigned int *v36; // [esp+38h] [ebp-424h]
  unsigned int *v37; // [esp+40h] [ebp-41Ch]
  int v38; // [esp+44h] [ebp-418h]
  unsigned int *v39; // [esp+48h] [ebp-414h]
  __m64 v40; // [esp+4Ch] [ebp-410h]
  __m64 v41[128]; // [esp+54h] [ebp-408h]

  v38 = *a2;
  v36 = a3[1];
  v37 = a3[3];
  v35 = a3[2];
  v40.m64_i32[1] = (unsigned int)(a5 * ((_DWORD)v37 - 1)) >> 1;
  v39 = *a3;
  v40.m64_i32[0] = (unsigned int)(a4 * ((_DWORD)v35 - 1)) >> 1;
  v33.m64_i32[1] = 0x10000 - a5;
  v34.m64_u64 = (a2[1] << 16) | 4;
  v7 = 0;
  v8 = (256 - a1) << 7;
  v33.m64_i32[0] = 0;
  do
  {
    v9 = v8;
    v8 += a1 - 256;
    v9 >>= 8;
    HIDWORD(v10) = (unsigned __int64)v9 >> 16;
    LODWORD(v10) = v9 | (v9 << 16);
    v41[v7].m64_i32[0] = v9 | ((_DWORD)v10 << 16);
    v41[v7++].m64_i32[1] = v10 >> 16;
  }
  while ( v7 < 128 );
  sub_40A620((int)a2, 0);
  HIWORD(v11) = -32640;
  v12 = _mm_cvtsi32_si64(0x80808080);
  v13 = v39;
  v14 = v37;
  do
  {
    v15 = v40;
    v40 = _m_paddd(v33, v40);
    v32 = v13;
    v31 = v14;
    v16 = v35;
    do
    {
      v17 = (unsigned int *)(v38 + _mm_cvtsi64_si32(_m_pmaddwd(_m_packssdw(_m_psradi(v15, 0x10u), 0LL), v34)));
      v18 = v15;
      v15 = _m_paddd(v15, (__m64)(0x10000 - a4));
      v19 = _mm_cvtsi64_si32(_m_packssdw(_m_psrlwi(_m_pand(v18, (__m64)0xFE000000FE00LL), 9u), 0LL));
      LOBYTE(v11) = 0x80;
      BYTE1(v11) = v19;
      v20 = v11 << 16;
      LOBYTE(v20) = 0x80;
      BYTE1(v20) = v19;
      v21 = _m_punpcklbw(_mm_cvtsi32_si64(v20), 0LL);
      LOBYTE(v20) = BYTE2(v19);
      BYTE1(v20) = BYTE2(v19);
      v11 = v20 << 16;
      LOWORD(v11) = -32640;
      v22 = _m_punpcklbw(_mm_cvtsi32_si64(v11), 0LL);
      v23 = _mm_cvtsi64_si32(
              _m_psubw(
                v12,
                _m_packssdw(
                  _m_psrlwi(_m_pmullw(_m_psubw(v22, _m_psrlqi(v22, 0x20u)), _m_psubw(v21, _m_psrldi(v21, 0x10u))), 7u),
                  0LL)));
      v24 = _m_punpcklbw(_mm_cvtsi32_si64(*v13), 0LL);
      if ( (unsigned __int8)v23 < 0x80u )
      {
        v11 = (unsigned __int8)v23;
        v25 = _m_psrlwi(_m_pmullw(v41[(unsigned __int8)v23], v24), 7u);
        *v17 = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(v25, v25), _mm_cvtsi32_si64(*v17)));
      }
      if ( BYTE1(v23) < 0x80u )
      {
        v11 = BYTE1(v23);
        v26 = _m_psrlwi(_m_pmullw(v41[BYTE1(v23)], v24), 7u);
        v17[1] = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(v26, v26), _mm_cvtsi32_si64(v17[1])));
      }
      v27 = HIWORD(v23);
      if ( (unsigned __int8)v27 < 0x80u )
      {
        v17 = (unsigned int *)((char *)v17 + ((unsigned int)_mm_cvtsi64_si32(v34) >> 16));
        v11 = (unsigned __int8)v27;
        v28 = _m_psrlwi(_m_pmullw(v41[(unsigned __int8)v27], v24), 7u);
        *v17 = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(v28, v28), _mm_cvtsi32_si64(*v17)));
      }
      if ( BYTE1(v27) < 0x80u )
      {
        v11 = BYTE1(v27);
        v29 = _m_psrlwi(_m_pmullw(v41[BYTE1(v27)], v24), 7u);
        v17[1] = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(v29, v29), _mm_cvtsi32_si64(v17[1])));
      }
      ++v13;
      v16 = (unsigned int *)((char *)v16 - 1);
    }
    while ( v16 );
    v13 = (unsigned int *)((char *)v32 + (_DWORD)v36);
    v14 = (unsigned int *)((char *)v31 - 1);
  }
  while ( v31 != (unsigned int *)1 );
  _m_empty();
  return v11;
}

// ===== sub_418FA0 @ 0x00418FA0..0x004190B8 =====
int __usercall sub_418FA0@<eax>(int result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // edx
  unsigned int v4; // ecx
  unsigned int v5; // eax
  unsigned int v6; // edx
  unsigned int v7; // ecx
  _DWORD *v8; // edi
  __m64 *v9; // esi
  unsigned int v10; // edx
  __m64 v11; // mm0
  unsigned int v12; // edx
  __m64 v13; // mm0
  __m64 v14; // mm1
  __m64 *v15; // [esp-34h] [ebp-34h]
  _DWORD *v16; // [esp-30h] [ebp-30h]
  _DWORD *v17; // [esp-1Ch] [ebp-1Ch]
  __m64 *v18; // [esp-18h] [ebp-18h]
  int v19; // [esp-14h] [ebp-14h]
  unsigned int v20; // [esp-Ch] [ebp-Ch]
  int v21; // [esp-8h] [ebp-8h]

  v2 = *(_DWORD *)(result + 16);
  if ( a2[4] == v2 && (unsigned int)(v2 - 1) <= 1 )
  {
    v17 = (_DWORD *)*a2;
    v18 = *(__m64 **)result;
    v3 = *(_DWORD *)(result + 4);
    v21 = a2[1];
    v4 = *(_DWORD *)(result + 8);
    v5 = *(_DWORD *)(result + 12);
    v19 = v3;
    v20 = v4 >> 1;
    v6 = v5 >> 1;
    result = v4 & 1 | (2 * (v5 & 1));
    v7 = v6;
    v8 = v17;
    v9 = v18;
    do
    {
      v16 = v8;
      v15 = v9;
      v10 = v20;
      do
      {
        v11 = _m_paddw(
                _m_paddw(_m_punpcklbw((__m64)v9->m64_u64, 0LL), _m_punpckhbw((__m64)v9->m64_u64, 0LL)),
                _m_paddw(
                  _m_punpcklbw(*(__m64 *)((char *)v9 + v19), 0LL),
                  _m_punpckhbw(*(__m64 *)((char *)v9 + v19), 0LL)));
        ++v9;
        *v8++ = _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(v11, 2u), 0LL));
        --v10;
      }
      while ( v10 );
      if ( (result & 1) != 0 )
        *v8 = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_psrlwi(
                    _m_paddw(
                      _m_punpcklbw(_mm_cvtsi32_si64(v9->m64_i32[0]), 0LL),
                      _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned __int32 *)((char *)v9->m64_u32 + v19)), 0LL)),
                    1u),
                  0LL));
      v9 = (__m64 *)((char *)v15 + 2 * v19);
      v8 = (_DWORD *)((char *)v16 + v21);
      --v7;
    }
    while ( v7 );
    if ( (result & 2) != 0 )
    {
      v12 = v20;
      do
      {
        v13 = _m_punpcklbw((__m64)v9->m64_u64, 0LL);
        v14 = _m_punpckhbw((__m64)v9->m64_u64, 0LL);
        ++v9;
        *v8++ = _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(v13, v14), 1u), 0LL));
        --v12;
      }
      while ( v12 );
      if ( (result & 1) != 0 )
      {
        result = v9->m64_i32[0];
        *v8 = v9->m64_i32[0];
      }
    }
    _m_empty();
  }
  return result;
}

// ===== sub_4190C0 @ 0x004190C0..0x00419145 =====
_DWORD *__usercall sub_4190C0@<eax>(_DWORD *result@<eax>, int *a2@<ecx>)
{
  _BYTE *v2; // edx
  int v3; // ebx
  int v4; // edi
  _DWORD *v5; // esi
  _BYTE *v6; // ecx
  _BYTE *v7; // esi
  int v8; // [esp-14h] [ebp-14h]
  int v9; // [esp-10h] [ebp-10h]
  _DWORD *i; // [esp-Ch] [ebp-Ch]
  int v11; // [esp-8h] [ebp-8h]

  if ( result[4] == 3 && a2[4] == 2 )
  {
    v2 = (_BYTE *)*result;
    v3 = *a2;
    v4 = result[3];
    v9 = result[1];
    v5 = (_DWORD *)result[2];
    v11 = *a2;
    v8 = a2[1];
    for ( i = v5; v4; v11 += v8 )
    {
      --v4;
      v6 = v2;
      result = v5;
      if ( v5 )
      {
        v7 = (_BYTE *)(v3 + 3);
        do
        {
          *v6++ = *v7;
          v7 += 4;
          result = (_DWORD *)((char *)result - 1);
        }
        while ( result );
        v5 = i;
      }
      v3 = v8 + v11;
      v2 += v9;
    }
  }
  return result;
}

// ===== sub_419150 @ 0x00419150..0x004192D2 =====
int __cdecl sub_419150(int a1, int a2, int a3, unsigned int a4, int a5, int a6, int a7, unsigned int a8, int a9)
{
  int v9; // eax
  int v10; // ecx
  unsigned int v12; // ecx
  _DWORD v13[2]; // [esp+14h] [ebp-24h] BYREF
  _DWORD v14[6]; // [esp+1Ch] [ebp-1Ch] BYREF

  v9 = a5;
  v10 = *(_DWORD *)(a2 + 16);
  if ( *(_DWORD *)(a1 + 16) != v10 )
    return 15;
  if ( (unsigned int)(v10 - 1) <= 1 )
  {
    v12 = a4;
    if ( a4 > 0x100 )
      return 23;
    if ( a7 != 8 && a7 != 38 )
      return 2;
    if ( a8 > 0x100 )
      return 3;
    if ( a9 )
    {
      v14[0] = a3;
      v14[2] = a5;
      v14[3] = a6;
      v14[1] = a4;
      v13[0] = a1;
      v13[1] = a2;
      v14[4] = a7;
      v14[5] = a8;
      if ( sub_419DE0(sub_41A290, v13, 2, 0, 0, v14) )
        return 0;
      v12 = a4;
      v9 = a5;
    }
    if ( *(_DWORD *)(a2 + 16) == 1 )
    {
      sub_4192E0(a1, a2, a3, v12, a6);
    }
    else if ( *(_DWORD *)(a2 + 16) == 2 )
    {
      sub_4197C0(a3, v12, v9, a6, a8);
      return 0;
    }
    return 0;
  }
  return 9;
}

// ===== sub_4192E0 @ 0x004192E0..0x004197BF =====
int __usercall sub_4192E0@<eax>(
        int result@<eax>,
        unsigned __int16 a2@<cx>,
        int a3@<ebp>,
        int *a4,
        unsigned int **a5,
        unsigned int a6,
        unsigned __int16 a7,
        unsigned int a8)
{
  unsigned int *v9; // ecx
  unsigned int *v10; // edx
  __m64 v11; // mm0
  __m64 v12; // mm0
  __int32 v13; // ecx
  unsigned int *v14; // esi
  unsigned int *v15; // edi
  unsigned int v16; // ecx
  __m64 v17; // mm0
  __m64 v18; // mm5
  __m64 v19; // mm0
  unsigned int v20; // edx
  __m64 v21; // mm0
  __m64 v22; // mm1
  unsigned __int32 v23; // edx
  __m128i v24; // xmm4
  __m128i v25; // xmm3
  int v26; // edi
  unsigned int *v27; // ecx
  __int32 v28; // edx
  __m128i v29; // xmm0
  __m128i v30; // xmm5
  __m128i v31; // xmm1
  __m128i v32; // xmm2
  __m128i v33; // xmm1
  __m128i v34; // [esp-C0h] [ebp-CCh]
  __m128i v35; // [esp-B0h] [ebp-BCh]
  __m128i v36; // [esp-A0h] [ebp-ACh] BYREF
  __m128i v37; // [esp-90h] [ebp-9Ch]
  __m128i v38; // [esp-80h] [ebp-8Ch]
  __m128i v39; // [esp-70h] [ebp-7Ch]
  __m128i si128; // [esp-60h] [ebp-6Ch]
  __m128i v41; // [esp-50h] [ebp-5Ch]
  __m128i v42; // [esp-40h] [ebp-4Ch] BYREF
  unsigned int *v43; // [esp-24h] [ebp-30h]
  int v44; // [esp-20h] [ebp-2Ch]
  unsigned __int32 v45; // [esp-1Ch] [ebp-28h]
  unsigned __int32 v46; // [esp-18h] [ebp-24h]
  unsigned int *v47; // [esp-14h] [ebp-20h]
  int v48; // [esp-10h] [ebp-1Ch]
  unsigned int *v49; // [esp-Ch] [ebp-18h]
  __m64 v50; // [esp-8h] [ebp-14h]
  int v51; // [esp+0h] [ebp-Ch]
  void *v52; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v51 = a3;
  v52 = retaddr;
  v48 = *a4;
  v49 = *a5;
  v44 = a4[1];
  v9 = a5[3];
  v43 = a5[1];
  v46 = a4[3];
  if ( v46 > (unsigned int)v9 )
    v46 = (unsigned __int32)v9;
  v10 = a5[2];
  if ( a4[2] <= (unsigned int)v10 )
    v10 = (unsigned int *)a4[2];
  v47 = v10;
  if ( ((unsigned __int8)v10 & 1) != 0 )
  {
    v11 = _mm_cvtsi32_si64(0xFFu);
    v12 = _m_punpcklwd(v11, v11);
    v37.m128i_u64[1] = (unsigned __int64)_m_punpcklwd(v12, v12);
    v41.m128i_i64[1] = 0x4D0096001DLL;
    si128.m128i_u64[1] = (unsigned __int64)_m_punpcklbw(_mm_cvtsi32_si64(a6), 0LL);
    v50.m64_i16[3] = 256;
    v50.m64_i16[2] = a7;
    v50.m64_i16[1] = a7;
    v50.m64_i16[0] = a7;
    v39.m128i_u64[1] = (unsigned __int64)_m_psrlwi(v50, 1u);
    v50.m64_i16[3] = 256;
    v13 = v46;
    v38.m128i_u64[1] = (unsigned __int64)_m_punpcklbw(_mm_cvtsi32_si64(a8), 0LL);
    v50.m64_i16[2] = a2;
    v50.m64_i16[1] = a2;
    v50.m64_i16[0] = a2;
    v42.m128i_u64[1] = (unsigned __int64)_m_psrlwi(v50, 1u);
    if ( v46 )
    {
      while ( 1 )
      {
        v14 = v49;
        v50.m64_i32[1] = --v13;
        v15 = v10;
        if ( v47 )
        {
          v46 = v48 - (_DWORD)v49;
          do
          {
            v16 = *v14;
            v17 = _m_punpcklbw(_mm_cvtsi32_si64(*v14), 0LL);
            v18 = _m_pmaddwd(v17, (__m64)v41.m128i_u64[1]);
            v19 = _m_paddw(
                    _m_psrawi(
                      _m_pmullw(
                        (__m64)v39.m128i_u64[1],
                        _m_psubw(
                          _m_pmulhuw(_m_pshufw(_m_paddd(v18, _m_psrlqi(v18, 0x20u)), 0), (__m64)si128.m128i_u64[1]),
                          v17)),
                      7u),
                    v17);
            v20 = *(_DWORD *)(result + 4 * _m_pextrw(v19, 0) + 2052);
            v45 = (unsigned __int32)v15 - 1;
            v15 = (unsigned int *)((char *)v15 - 1);
            v21 = _m_punpcklbw(
                    _mm_cvtsi32_si64(*(_DWORD *)(result + 4 * _m_pextrw(v19, 2) + 4) | *(_DWORD *)(result
                                                                                                 + 4 * _m_pextrw(v19, 1)
                                                                                                 + 1028) | v20),
                    0LL);
            v22 = _m_pand(_m_pcmpgtw(v21, (__m64)0xFF007F007F007FLL), (__m64)v37.m128i_u64[1]);
            *(unsigned int *)((char *)v14++ + v46) = v16 ^ (v16 ^ _mm_cvtsi64_si32(
                                                                    _m_packuswb(
                                                                      _m_paddw(
                                                                        v21,
                                                                        _m_psrawi(
                                                                          _m_pmullw(
                                                                            _m_psubw(
                                                                              _m_pxor(
                                                                                _m_psrlwi(
                                                                                  _m_pmullw(
                                                                                    _m_pxor(v21, v22),
                                                                                    _m_pxor(
                                                                                      (__m64)v38.m128i_u64[1],
                                                                                      v22)),
                                                                                  7u),
                                                                                v22),
                                                                              v21),
                                                                            (__m64)v42.m128i_u64[1]),
                                                                          7u)),
                                                                      0LL))) & 0xFFFFFF;
          }
          while ( v15 );
          v13 = v50.m64_i32[1];
        }
        v48 += v44;
        v49 = (unsigned int *)((char *)v49 + (_DWORD)v43);
        if ( !v13 )
          break;
        v10 = v47;
      }
    }
    _m_empty();
  }
  else
  {
    si128 = _mm_load_si128((const __m128i *)&xmmword_4E41E0);
    v47 = (unsigned int *)((unsigned int)v10 >> 1);
    v41 = _mm_load_si128((const __m128i *)&xmmword_4E41F0);
    v34 = _mm_load_si128((const __m128i *)&xmmword_4E4200);
    v42 = 0LL;
    v35 = _mm_load_si128((const __m128i *)&xmmword_4E4210);
    v38 = _mm_load_si128((const __m128i *)&xmmword_4E4220);
    v37 = _mm_shuffle_epi32(_mm_unpacklo_epi8(_mm_cvtsi32_si128(a6), (__m128i)0LL), 68);
    v39 = _mm_srli_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a7), _mm_cvtsi32_si128(a7)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a7), _mm_cvtsi32_si128(a7))),
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a7), _mm_cvtsi32_si128(a7)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0x100u), _mm_cvtsi32_si128(0x100u)))),
            1u);
    v36 = _mm_shuffle_epi32(_mm_unpacklo_epi8(_mm_cvtsi32_si128(a8), (__m128i)0LL), 68);
    v23 = v46;
    v24 = _mm_srli_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a2), _mm_cvtsi32_si128(a2)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a2), _mm_cvtsi32_si128(a2))),
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(a2), _mm_cvtsi32_si128(a2)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0x100u), _mm_cvtsi32_si128(0x100u)))),
            1u);
    if ( v46 )
    {
      v25 = _mm_load_si128(&v42);
      v26 = v48;
      do
      {
        v27 = v47;
        v45 = --v23;
        if ( v47 )
        {
          v28 = (__int32)v49 - v26;
          for ( v50.m64_i32[1] = (__int32)v49 - v26; ; v28 = v50.m64_i32[1] )
          {
            v29 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)(v28 + v26)), v25);
            v30 = _mm_madd_epi16(v29, v38);
            v31 = _mm_add_epi16(
                    v29,
                    _mm_srai_epi16(
                      _mm_mullo_epi16(
                        _mm_sub_epi16(
                          _mm_mulhi_epu16(
                            _mm_shufflehi_epi16(
                              _mm_shufflelo_epi16(_mm_add_epi32(v30, _mm_srli_epi64(v30, 0x20u)), 0),
                              0),
                            v37),
                          v29),
                        v39),
                      7u));
            v26 += 8;
            v27 = (unsigned int *)((char *)v27 - 1);
            v32 = _mm_unpacklo_epi8(
                    _mm_or_si128(
                      _mm_cvtsi32_si128(*(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 2) + 4) | (unsigned int)(*(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 1) + 1028) | *(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 0) + 2052))),
                      _mm_slli_si128(
                        _mm_cvtsi32_si128(*(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 6) + 4) | (unsigned int)(*(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 5) + 1028) | *(_DWORD *)(result + 4 * _mm_extract_epi16(v31, 4) + 2052))),
                        4)),
                    v25);
            v33 = _mm_and_si128(_mm_cmpgt_epi16(v32, si128), v41);
            *(_QWORD *)(v26 - 8) = _mm_packus_epi16(
                                     _mm_or_si128(
                                       _mm_and_si128(
                                         _mm_add_epi16(
                                           v32,
                                           _mm_srai_epi16(
                                             _mm_mullo_epi16(
                                               _mm_sub_epi16(
                                                 _mm_xor_si128(
                                                   _mm_srli_epi16(
                                                     _mm_mullo_epi16(
                                                       _mm_xor_si128(v32, v33),
                                                       _mm_xor_si128(_mm_load_si128(&v36), v33)),
                                                     7u),
                                                   v33),
                                                 v32),
                                               v24),
                                             7u)),
                                         v35),
                                       _mm_and_si128(v29, v34)),
                                     v25).m128i_u64[0];
            if ( !v27 )
              break;
          }
          v23 = v45;
          v26 = v48;
        }
        v26 += v44;
        v49 = (unsigned int *)((char *)v49 + (_DWORD)v43);
        v48 = v26;
      }
      while ( v23 );
    }
  }
  return result;
}

// ===== sub_4197C0 @ 0x004197C0..0x004199CE =====
int __usercall sub_4197C0@<eax>(
        unsigned int **a1@<eax>,
        int *a2@<ecx>,
        unsigned int a3,
        __int16 a4,
        int a5,
        unsigned int a6,
        __int16 a7)
{
  unsigned int *v7; // edx
  unsigned int *v8; // ecx
  unsigned int *v9; // eax
  __m64 v10; // mm0
  __m64 v11; // mm0
  int result; // eax
  unsigned int *i; // edi
  unsigned int *v14; // eax
  unsigned int *v15; // esi
  __m64 v16; // mm0
  __m64 v17; // mm5
  __m64 v18; // mm0
  __m64 v19; // mm0
  __m64 v20; // mm1
  __m64 v21; // [esp+8h] [ebp-5Ch]
  __m64 v22; // [esp+10h] [ebp-54h]
  __m64 v23; // [esp+18h] [ebp-4Ch]
  __m64 v24; // [esp+20h] [ebp-44h]
  __m64 v25; // [esp+28h] [ebp-3Ch]
  unsigned int *v26; // [esp+38h] [ebp-2Ch]
  int v27; // [esp+3Ch] [ebp-28h]
  __m64 v28; // [esp+40h] [ebp-24h]
  unsigned int *v29; // [esp+54h] [ebp-10h]
  int v30; // [esp+58h] [ebp-Ch]
  unsigned int *v31; // [esp+5Ch] [ebp-8h]

  v30 = *a2;
  v29 = *a1;
  v27 = a2[1];
  v26 = a1[1];
  v7 = (unsigned int *)a2[3];
  if ( v7 > a1[3] )
    v7 = a1[3];
  v8 = (unsigned int *)a2[2];
  v9 = a1[2];
  if ( v8 > v9 )
  {
    v31 = v9;
    v8 = v9;
  }
  else
  {
    v31 = v8;
  }
  v10 = _mm_cvtsi32_si64(0xFFu);
  v11 = _m_punpcklwd(v10, v10);
  v23 = _m_punpcklwd(v11, v11);
  v28.m64_i16[3] = 256;
  v25 = _m_punpcklbw(_mm_cvtsi32_si64(a3), 0LL);
  v28.m64_i16[2] = a4;
  v28.m64_i16[1] = a4;
  v28.m64_i16[0] = a4;
  v24 = _m_psrlwi(v28, 1u);
  result = 256;
  v28.m64_i16[2] = a7;
  v28.m64_i16[1] = a7;
  v28.m64_i16[0] = a7;
  v22 = _m_punpcklbw(_mm_cvtsi32_si64(a6), 0LL);
  v21 = _m_psrlwi(v28, 1u);
  for ( i = v7; i; v29 = (unsigned int *)result )
  {
    v14 = v29;
    i = (unsigned int *)((char *)i - 1);
    v15 = v8;
    if ( v8 )
    {
      do
      {
        v15 = (unsigned int *)((char *)v15 - 1);
        if ( (*v14 & 0xFF000000) != 0 )
        {
          v16 = _m_punpcklbw(_mm_cvtsi32_si64(*v14), 0LL);
          v17 = _m_pmaddwd(v16, (__m64)0x4D0096001DLL);
          v18 = _m_paddw(
                  _m_psrawi(
                    _m_pmullw(v24, _m_psubw(_m_pmulhuw(_m_pshufw(_m_paddd(v17, _m_psrlqi(v17, 0x20u)), 0), v25), v16)),
                    7u),
                  v16);
          v19 = _m_punpcklbw(
                  _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * _m_pextrw(v18, 2) + 4) | (unsigned int)(*(_DWORD *)(a5 + 4 * _m_pextrw(v18, 1) + 1028) | *(_DWORD *)(a5 + 4 * _m_pextrw(v18, 0) + 2052))),
                  0LL);
          v20 = _m_pand(_m_pcmpgtw(v19, (__m64)0xFF007F007F007FLL), v23);
          *(unsigned int *)((char *)v14 + v30 - (_DWORD)v29) = *v14 & 0xFF000000 | _mm_cvtsi64_si32(
                                                                                     _m_packuswb(
                                                                                       _m_paddw(
                                                                                         v19,
                                                                                         _m_psrawi(
                                                                                           _m_pmullw(
                                                                                             _m_psubw(
                                                                                               _m_pxor(
                                                                                                 _m_psrlwi(
                                                                                                   _m_pmullw(
                                                                                                     _m_pxor(v19, v20),
                                                                                                     _m_pxor(v22, v20)),
                                                                                                   7u),
                                                                                                 v20),
                                                                                               v19),
                                                                                             v21),
                                                                                           7u)),
                                                                                       0LL)) & 0xFFFFFF;
        }
        ++v14;
      }
      while ( v15 );
      v8 = v31;
      v14 = v29;
    }
    result = (int)v14 + (_DWORD)v26;
    v30 += v27;
  }
  _m_empty();
  return result;
}

// ===== sub_4199D0 @ 0x004199D0..0x00419AF1 =====
int __cdecl sub_4199D0(size_t *a1, size_t a2, unsigned int a3, int a4, int a5, int a6)
{
  unsigned int v6; // ecx
  int result; // eax
  _DWORD v8[2]; // [esp+10h] [ebp-18h] BYREF
  _DWORD v9[3]; // [esp+18h] [ebp-10h] BYREF

  v6 = a3;
  result = 24;
  if ( a3 <= 5 )
  {
    if ( a6 )
    {
      v9[0] = a3;
      v8[0] = a1;
      v8[1] = a2;
      v9[1] = a4;
      v9[2] = a5;
      if ( sub_419DE0(sub_41A3B0, v8, 2, 0, 0, v9) )
        return 0;
      v6 = a3;
    }
    switch ( v6 )
    {
      case 0u:
        sub_40AF50(a2, a1);
        result = 0;
        break;
      case 1u:
        sub_410CB0(a5, a2, a4, (int)a1);
        result = 0;
        break;
      case 2u:
        sub_411060(a5, a2, a4, (int)a1);
        result = 0;
        break;
      case 3u:
        sub_4113F0(a2, (int)a1, a4, a5);
        return 0;
      case 4u:
        sub_411690(a2, (int)a1, a4, a5);
        return 0;
      case 5u:
        sub_411810(a2, (int)a1, a4, a5);
        return 0;
    }
  }
  return result;
}

// ===== sub_419B10 @ 0x00419B10..0x00419B9B =====
int __usercall sub_419B10@<eax>(int a1@<edx>, unsigned int *a2, unsigned int *a3)
{
  int v3; // ebx
  unsigned int v4; // eax
  int v5; // edx
  unsigned int v6; // ecx
  unsigned int v7; // esi
  unsigned int v8; // edi
  unsigned int i; // eax

  if ( !dword_565B1C )
    return 0;
  v3 = *(_DWORD *)(a1 + 8);
  if ( (unsigned int)(v3 * *(_DWORD *)(a1 + 12)) < 0xA00 )
    return 0;
  v4 = sub_490AE0();
  v6 = v4;
  if ( v4 < 2 )
    return 0;
  v7 = *(_DWORD *)(v5 + 12);
  v8 = (v7 << 16) / v4;
  for ( i = v3 * (v7 / v4); i > 0xA000; v8 >>= 1 )
  {
    i >>= 1;
    v6 *= 2;
  }
  if ( v8 < 0x10000 )
    return 0;
  *a3 = v8;
  *a2 = v6;
  return 1;
}

// ===== sub_419BA0 @ 0x00419BA0..0x00419C4F =====
_DWORD *__usercall sub_419BA0@<eax>(
        _DWORD *result@<eax>,
        unsigned int *a2@<ecx>,
        int a3,
        unsigned int a4,
        int a5,
        int a6)
{
  int v6; // esi
  int v7; // edi
  unsigned int i; // ebx
  unsigned int v9; // edx
  _DWORD *v10; // ecx
  unsigned int v11; // ecx
  unsigned int v13; // [esp+20h] [ebp+10h]

  v6 = a5;
  v7 = 0;
  for ( i = 0; v6; i = (unsigned __int16)i )
  {
    i += a6;
    v9 = 0;
    --v6;
    v13 = i;
    if ( a4 )
    {
      do
      {
        v10 = *(_DWORD **)(a3 + 4 * v9);
        *result = *v10;
        result[1] = v10[1];
        result[2] = v10[2];
        result[3] = v10[3];
        result[4] = v10[4];
        i = v13;
        result[5] = v10[5];
        *result += v7 * result[1];
        if ( v6 )
          v11 = HIWORD(v13);
        else
          v11 = result[3] - v7;
        result[3] = v11;
        ++v9;
        result += 6;
      }
      while ( v9 < a4 );
    }
    if ( a2 )
      *a2++ = HIWORD(i);
    v7 += HIWORD(i);
  }
  return result;
}

// ===== sub_419C50 @ 0x00419C50..0x00419CBE =====
int *__usercall sub_419C50@<eax>(int *result@<eax>, _DWORD *a2, int a3, int a4)
{
  int v4; // edx
  int v5; // ebx
  int v6; // ecx
  int v7; // edx
  int v8; // edi
  int v10; // ecx
  int v11; // edx
  int v12; // eax
  int v13; // [esp+10h] [ebp-14h] BYREF
  int v14; // [esp+14h] [ebp-10h]
  int v15; // [esp+18h] [ebp-Ch]
  int i; // [esp+1Ch] [ebp-8h]

  v4 = result[1];
  v5 = a3;
  v13 = *result;
  v6 = result[2];
  v14 = v4;
  v7 = result[3];
  v8 = 0;
  v15 = v6;
  for ( i = v7; v5; --v5 )
  {
    v10 = v14;
    v11 = v15;
    v8 += a4;
    *a2 = v13;
    v12 = i;
    a2[1] = v10;
    a2[2] = v11;
    a2[3] = v12;
    result = sub_409170(-(v8 >> 16), 0, &v13);
    v8 = (unsigned __int16)v8;
    a2 += 4;
  }
  return result;
}

// ===== sub_419CC0 @ 0x00419CC0..0x00419CF8 =====
int __usercall sub_419CC0@<eax>(int a1@<eax>, int *a2@<ecx>, _DWORD *a3, int a4)
{
  int v4; // edx
  int v5; // esi
  int result; // eax

  v4 = a2[1];
  v5 = *a2;
  for ( result = 0; a1; --a1 )
  {
    result += a4;
    a3[1] = v4;
    *a3 = v5;
    v4 -= result & 0xFFFF0000;
    result = (unsigned __int16)result;
    a3 += 2;
  }
  return result;
}

// ===== sub_419D00 @ 0x00419D00..0x00419D53 =====
unsigned int __usercall sub_419D00@<eax>(int a1@<eax>, float *a2@<ecx>, int a3@<edi>, float *a4)
{
  float v4; // edx
  unsigned int result; // eax
  double v8; // st6
  float i; // [esp+Ch] [ebp-8h]

  v4 = *a2;
  result = 0;
  for ( i = a2[1]; a1; i = i - v8 )
  {
    result += a3;
    a4[1] = i;
    v8 = (double)HIWORD(result);
    *a4 = v4;
    result = (unsigned __int16)result;
    a4 += 2;
    --a1;
  }
  return result;
}

// ===== sub_419D60 @ 0x00419D60..0x00419DDE =====
int __usercall sub_419D60@<eax>(int result@<eax>, _DWORD *a2, unsigned int a3, int a4)
{
  int v4; // edx
  int v5; // ebx
  unsigned int v6; // esi
  int v8; // eax
  int v9; // edi
  int i; // [esp+8h] [ebp-Ch]
  int v11; // [esp+Ch] [ebp-8h]
  int v12; // [esp+10h] [ebp-4h]

  v4 = *(_DWORD *)(result + 8);
  v5 = *(_DWORD *)(result + 4);
  v6 = 0;
  v12 = *(_DWORD *)result;
  for ( i = v5; v6 < a3; i = v5 )
  {
    v8 = *(_DWORD *)(a4 + 4 * v6);
    if ( v8 > v4 )
    {
      v9 = v4;
      v11 = v4;
    }
    else
    {
      v9 = *(_DWORD *)(a4 + 4 * v6);
      v11 = v9;
    }
    result = v8 - v9;
    if ( result > v5 )
      result = v5;
    if ( result < 1 )
    {
      *a2 = 0;
    }
    else
    {
      *a2 = v12;
      v5 = i;
      v12 += 32 * result;
      v9 = v11;
    }
    a2[2] = v4;
    a2[1] = result;
    v5 -= result;
    ++v6;
    v4 -= v9;
    a2 += 3;
  }
  return result;
}

// ===== sub_419DE0 @ 0x00419DE0..0x00419FBB =====
int __usercall sub_419DE0@<eax>(int a1@<edx>, int a2, int a3, unsigned int a4, int a5, int *a6, int a7)
{
  unsigned int v7; // ebx
  int v8; // edi
  float *v9; // esi
  int v10; // edi
  _DWORD v12[6]; // [esp+Ch] [ebp-24h] BYREF
  int v13; // [esp+24h] [ebp-Ch] BYREF
  void *v14; // [esp+28h] [ebp-8h] BYREF
  void *v15; // [esp+2Ch] [ebp-4h]

  if ( !sub_419B10(a1, (unsigned int *)&v14, (unsigned int *)&v13) )
    return 0;
  v7 = (unsigned int)v14;
  v14 = operator new[](24 * a4 * (_DWORD)v14);
  if ( a5 == 4 )
    v15 = operator new[](4 * v7);
  else
    v15 = 0;
  v8 = v13;
  v9 = 0;
  sub_419BA0(v14, (unsigned int *)v15, a3, a4, v7, v13);
  switch ( a5 )
  {
    case 1:
      v9 = (float *)operator new[](16 * v7);
      sub_419C50(a6, v9, v7, v8);
      break;
    case 2:
      v9 = (float *)operator new[](8 * v7);
      sub_419CC0(v7, a6, v9, v8);
      break;
    case 3:
      v9 = (float *)operator new[](8 * v7);
      sub_419D00(v7, (float *)a6, v8, v9);
      break;
    case 4:
      v9 = (float *)operator new[](12 * v7);
      sub_419D60((int)a6, v9, v7, (int)v15);
      break;
    default:
      break;
  }
  operator delete[](v15);
  v10 = dword_565B1C;
  v12[2] = v14;
  v12[3] = a4;
  v12[5] = a7;
  v12[0] = 0;
  v12[1] = v7;
  v12[4] = v9;
  sub_4464C0(v12);
  sub_4464E0(v10);
  sub_4464C0(0);
  operator delete[](v14);
  operator delete[](v9);
  return 1;
}

// ===== sub_419FD0 @ 0x00419FD0..0x0041A032 =====
int __usercall sub_419FD0@<eax>(_DWORD *a1@<esi>, void *a2, size_t Size)
{
  int v3; // edi
  int v5; // [esp+8h] [ebp-4h]

  v3 = 0;
  v5 = sub_4465D0(dword_565B1C);
  if ( *a1 < a1[1] )
  {
    if ( a2 )
      memcpy_0(a2, (const void *)(a1[4] + Size * *a1), Size);
    v3 = a1[2] + 24 * *a1 * a1[3];
    ++*a1;
  }
  sub_4465F0(v5);
  return v3;
}

// ===== sub_41A040 @ 0x0041A040..0x0041A07D =====
int __cdecl sub_41A040(int a1)
{
  int v1; // eax

  v1 = sub_419FD0((_DWORD *)a1, 0, 0);
  if ( !v1 )
    return 0;
  sub_40A9E0(v1, v1 + 24, **(_DWORD **)(a1 + 20), *(_DWORD *)(*(_DWORD *)(a1 + 20) + 4), 0);
  return 1;
}

// ===== sub_41A080 @ 0x0041A080..0x0041A0BD =====
int __cdecl sub_41A080(int a1)
{
  int v1; // eax

  v1 = sub_419FD0((_DWORD *)a1, 0, 0);
  if ( !v1 )
    return 0;
  sub_40C0F0(v1, v1 + 24, v1 + 48, **(_DWORD **)(a1 + 20), 0);
  return 1;
}

// ===== sub_41A0C0 @ 0x0041A0C0..0x0041A164 =====
int __cdecl sub_41A0C0(int a1)
{
  int *v1; // eax
  _DWORD v3[4]; // [esp+8h] [ebp-10h] BYREF

  v1 = (int *)sub_419FD0((_DWORD *)a1, v3, 0x10u);
  if ( !v1 )
    return 0;
  if ( *(_DWORD *)(a1 + 12) == 3 )
  {
    if ( **(_DWORD **)(a1 + 20) == 256 )
      sub_4128D0(v3, v1 + 6, v1, v1 + 12);
    else
      sub_412CE0(v3, v1 + 6, v1, v1 + 12, **(_DWORD **)(a1 + 20));
    return 1;
  }
  else
  {
    sub_413280(v3, v1 + 6, v1, v1 + 12, v1 + 18, **(_DWORD **)(a1 + 20));
    return 1;
  }
}

// ===== sub_41A170 @ 0x0041A170..0x0041A1AD =====
int __cdecl sub_41A170(int a1)
{
  _DWORD *v1; // eax

  v1 = (_DWORD *)sub_419FD0((_DWORD *)a1, 0, 0);
  if ( !v1 )
    return 0;
  sub_413540(v1, v1 + 6, **(_DWORD **)(a1 + 20), *(_DWORD *)(*(_DWORD *)(a1 + 20) + 4), 0);
  return 1;
}

// ===== sub_41A1B0 @ 0x0041A1B0..0x0041A214 =====
int __cdecl sub_41A1B0(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v4[3]; // [esp+8h] [ebp-Ch] BYREF

  v1 = sub_419FD0(a1, v4, 8u);
  if ( !v1 )
    return 0;
  v2 = a1[5];
  sub_4168E0(
    v1,
    v4[0],
    v4[1],
    *(_DWORD **)v2,
    *(_DWORD *)(v2 + 4),
    *(_DWORD *)(v2 + 8),
    *(_DWORD *)(v2 + 12),
    *(_DWORD *)(v2 + 16),
    *(_DWORD *)(v2 + 20),
    *(_DWORD *)(v2 + 24),
    *(_DWORD *)(v2 + 28),
    0);
  return 1;
}

// ===== sub_41A220 @ 0x0041A220..0x0041A284 =====
int __cdecl sub_41A220(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v4[3]; // [esp+8h] [ebp-Ch] BYREF

  v1 = sub_419FD0(a1, v4, 8u);
  if ( !v1 )
    return 0;
  v2 = a1[5];
  sub_417730(
    v1,
    v4[0],
    v4[1],
    *(_DWORD **)v2,
    *(_DWORD *)(v2 + 4),
    *(_DWORD *)(v2 + 8),
    *(_DWORD *)(v2 + 12),
    *(_DWORD *)(v2 + 16),
    *(_DWORD *)(v2 + 20),
    *(_DWORD *)(v2 + 24),
    *(_DWORD *)(v2 + 28),
    0);
  return 1;
}

// ===== sub_41A290 @ 0x0041A290..0x0041A2DD =====
int __cdecl sub_41A290(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx

  v1 = sub_419FD0(a1, 0, 0);
  if ( !v1 )
    return 0;
  v2 = a1[5];
  sub_419150(
    v1,
    v1 + 24,
    *(_DWORD *)v2,
    *(_DWORD *)(v2 + 4),
    *(_DWORD *)(v2 + 8),
    *(_DWORD *)(v2 + 12),
    *(_DWORD *)(v2 + 16),
    *(_DWORD *)(v2 + 20),
    0);
  return 1;
}

// ===== sub_41A2E0 @ 0x0041A2E0..0x0041A34E =====
int __cdecl sub_41A2E0(int a1)
{
  int v1; // eax
  float v3[3]; // [esp+28h] [ebp-Ch] BYREF

  v1 = sub_419FD0((_DWORD *)a1, v3, 8u);
  if ( !v1 )
    return 0;
  sub_410820(
    v1,
    v3[0],
    v3[1],
    **(_DWORD **)(a1 + 20),
    COERCE_INT(*(float *)(*(_DWORD *)(a1 + 20) + 4)),
    COERCE_INT(*(float *)(*(_DWORD *)(a1 + 20) + 8)),
    *(float *)(*(_DWORD *)(a1 + 20) + 12),
    *(float *)(*(_DWORD *)(a1 + 20) + 16),
    0);
  return 1;
}

// ===== sub_41A350 @ 0x0041A350..0x0041A3A8 =====
int __cdecl sub_41A350(int a1)
{
  int v1; // eax
  int v3[3]; // [esp+8h] [ebp-Ch] BYREF

  v1 = sub_419FD0((_DWORD *)a1, v3, 0xCu);
  if ( !v1 )
    return 0;
  sub_40FA50(
    v1,
    **(_DWORD **)(a1 + 20),
    v3[0],
    v3[1],
    v3[2],
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 4),
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 8),
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 12),
    0);
  return 1;
}

// ===== sub_41A3B0 @ 0x0041A3B0..0x0041A3F1 =====
int __cdecl sub_41A3B0(int a1)
{
  size_t *v1; // eax

  v1 = (size_t *)sub_419FD0((_DWORD *)a1, 0, 0);
  if ( !v1 )
    return 0;
  sub_4199D0(
    v1,
    (size_t)(v1 + 6),
    **(_DWORD **)(a1 + 20),
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 4),
    *(_DWORD *)(*(_DWORD *)(a1 + 20) + 8),
    0);
  return 1;
}

// ===== sub_41A400 @ 0x0041A400..0x0041A5AE =====
int __userpurge sub_41A400@<eax>(int a1@<ecx>, _DWORD *a2@<esi>, int a3, int a4)
{
  int v4; // eax

  a2[6] = a3;
  a2[8] = a1;
  *a2 = &CDspObj::`vftable';
  a2[10] = 0;
  a2[11] = 0;
  sub_41ACD0(0);
  sub_41AD40();
  a2[72] = 0;
  a2[75] = 0;
  sub_41AD60(1);
  sub_493130(0);
  sub_41ADA0(0);
  sub_41ADE0(0);
  sub_41ADF0(1);
  sub_41AE00(0);
  sub_41BEA0(1);
  sub_41BEB0(0);
  sub_41BEF0();
  sub_41B370(0, 0, 0);
  sub_41B520(0, 0, 0);
  sub_41B580(0, 0, 0);
  sub_41B2D0(0, 0);
  sub_41B320(0, 0);
  sub_41B600(128);
  sub_41B620(0);
  sub_41B660(0);
  sub_41B6A0(256);
  sub_41B6E0(a2, 0);
  sub_41B8B0(0);
  sub_41BF10(0);
  sub_41C1C0();
  memset(a2 + 48, 0, 0x40u);
  if ( operator new(0x10u) )
    v4 = sub_430260();
  else
    v4 = 0;
  a2[35] = v4;
  a2[36] = 0;
  a2[37] = 0;
  a2[38] = 0;
  a2[39] = 0;
  a2[40] = 0;
  a2[41] = 0;
  a2[67] = 0;
  a2[68] = 0;
  a2[69] = 0;
  a2[70] = 0;
  a2[66] = 1;
  return sub_41C0B0(a4);
}

// ===== sub_41A5B0 @ 0x0041A5B0..0x0041A5D1 =====
void *__thiscall sub_41A5B0(void *this, char a2)
{
  sub_41A5E0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_41A5E0 @ 0x0041A5E0..0x0041A642 =====
int __thiscall sub_41A5E0(_DWORD *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx
  int result; // eax

  v2 = (void (__thiscall ***)(_DWORD, int))this[35];
  *this = &CDspObj::`vftable';
  if ( v2 )
    (**v2)(v2, 1);
  sub_41BC70(0);
  for ( result = sub_41AF00(0); this[75]; result = sub_41AC40() )
    ;
  if ( this[71] )
    return sub_41AC40();
  return result;
}

// ===== sub_41A650 @ 0x0041A650..0x0041A65B =====
void *sub_41A650()
{
  void *result; // eax

  result = dword_56674C;
  dword_565B2C = (int)dword_56674C;
  return result;
}

// ===== sub_41A660 @ 0x0041A660..0x0041A66B =====
void *sub_41A660()
{
  void *result; // eax

  result = dword_566750;
  dword_565B30 = (int)dword_566750;
  return result;
}

// ===== sub_41A670 @ 0x0041A670..0x0041A676 =====
int __usercall sub_41A670@<eax>(int result@<eax>)
{
  dword_5076AC = result;
  return result;
}

// ===== sub_41A680 @ 0x0041A680..0x0041A68C =====
int __usercall sub_41A680@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_565B34 = result;
  dword_565B38 = a2;
  return result;
}

// ===== sub_41A690 @ 0x0041A690..0x0041AA5C =====
int __cdecl sub_41A690(int a1)
{
  int v1; // ecx
  int result; // eax

  switch ( v1 )
  {
    case 1:
      result = (int)((cos((double)(int)(11796480 - 180LL * a1 / 256) * 3.141592653589793 / 11796480.0) + 1.0) * 32768.0);
      break;
    case 2:
      result = (int)(sin((double)(int)(90LL * a1 / 256) * 3.141592653589793 / 11796480.0) * 65536.0);
      break;
    case 3:
      result = (int)((1.0 - sin((double)(int)(5898240 - 90LL * a1 / 256) * 3.141592653589793 / 11796480.0)) * 65536.0);
      break;
    case 4:
      result = (int)(pow((double)a1, 2.0) * 65536.0 / pow(16777216.0, 2.0));
      break;
    case 5:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 2.0) / pow(16777216.0, 2.0)) * 65536.0);
      break;
    case 6:
      result = (int)(pow((double)a1, 2.5) * 65536.0 / pow(16777216.0, 2.5));
      break;
    case 7:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 2.5) / pow(16777216.0, 2.5)) * 65536.0);
      break;
    case 8:
      result = (int)(pow((double)a1, 3.0) * 65536.0 / pow(16777216.0, 3.0));
      break;
    case 9:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 3.0) / pow(16777216.0, 3.0)) * 65536.0);
      break;
    case 10:
      result = (int)(pow((double)a1, 4.0) * 65536.0 / pow(16777216.0, 4.0));
      break;
    case 11:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 4.0) / pow(16777216.0, 4.0)) * 65536.0);
      break;
    case 12:
      result = (int)(pow((double)a1, 5.0) * 65536.0 / pow(16777216.0, 5.0));
      break;
    case 13:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 5.0) / pow(16777216.0, 5.0)) * 65536.0);
      break;
    case 14:
      result = (int)(pow((double)a1, 6.0) * 65536.0 / pow(16777216.0, 6.0));
      break;
    case 15:
      result = (int)((1.0 - pow((double)(0x1000000 - a1), 6.0) / pow(16777216.0, 6.0)) * 65536.0);
      break;
    default:
      result = a1 / 256;
      break;
  }
  return result;
}

// ===== sub_41AAA0 @ 0x0041AAA0..0x0041AB02 =====
int __usercall sub_41AAA0@<eax>(__int64 a1@<edx:eax>, unsigned int a2@<esi>, _DWORD *a3)
{
  __int64 v4; // [esp-10h] [ebp-18h]

  if ( (_DWORD)a1 && a2 )
  {
    if ( (int)a1 < 0 )
    {
      a1 = (__int64)(((unsigned __int64)a2 << 16) - (int)a1) / a2;
    }
    else
    {
      HIDWORD(v4) = a2;
      LODWORD(v4) = 0;
      a1 = v4 / (__int64)((int)a1 + ((unsigned __int64)a2 << 16));
    }
    *a3 = a1;
  }
  else
  {
    *a3 = 0x10000;
  }
  return a1;
}

// ===== sub_41AB10 @ 0x0041AB10..0x0041AB32 =====
BOOL __stdcall sub_41AB10(int a1)
{
  BOOL result; // eax
  int v2; // edx
  int v3; // ecx

  result = sub_41C050() == 0;
  if ( result )
  {
    *(_DWORD *)(v3 + 152) = v2;
    *(_DWORD *)(v3 + 156) = a1;
  }
  return result;
}

// ===== sub_41AB40 @ 0x0041AB40..0x0041AC3E =====
int __userpurge sub_41AB40@<eax>(int a1@<edi>, _DWORD *a2@<esi>, int a3, int a4)
{
  _DWORD *v5; // eax
  _DWORD v6[2]; // [esp+4h] [ebp-14h] BYREF
  _DWORD v7[2]; // [esp+Ch] [ebp-Ch] BYREF

  if ( sub_41ACE0() && sub_41B0B0() != 8 )
    return 0;
  v5 = operator new(0x10u);
  v5[1] = a3;
  *v5 = a1;
  v5[2] = a4;
  v5[3] = a2[75];
  a2[75] = v5;
  sub_41ACD0(a2);
  if ( (*(int (**)(void))(*a2 + 112))() && (*(int (__thiscall **)(int))(*(_DWORD *)a1 + 112))(a1) )
  {
    (*(void (__thiscall **)(_DWORD *, _DWORD *))(*a2 + 64))(a2, v6);
    (*(void (__thiscall **)(int, int, int, _DWORD))(*(_DWORD *)a1 + 60))(a1, a3 + v6[0], a4 + v6[1], v7[0]);
    return 1;
  }
  else
  {
    (*(void (__thiscall **)(_DWORD *, _DWORD *))(*a2 + 48))(a2, v7);
    (*(void (__thiscall **)(int, int, int, _DWORD, int))(*(_DWORD *)a1 + 40))(a1, a3 + v7[0], a4 + v7[1], 0, 1);
    return 1;
  }
}

// ===== sub_41AC40 @ 0x0041AC40..0x0041AC7E =====
int __usercall sub_41AC40@<eax>(int a1@<edx>, int a2@<esi>)
{
  _DWORD *v2; // ecx
  _DWORD *v3; // edx
  int result; // eax
  void *v5; // edx

  v2 = (_DWORD *)(a1 + 288);
  v3 = *(_DWORD **)(a1 + 300);
  result = 0;
  if ( v3 )
  {
    while ( *v3 != a2 )
    {
      v2 = v3;
      v3 = (_DWORD *)v3[3];
      if ( !v3 )
        return result;
    }
    v2[3] = v3[3];
    sub_41ACD0(0);
    operator delete(v5);
    return 1;
  }
  return result;
}

// ===== sub_41AC80 @ 0x0041AC80..0x0041AC9A =====
BOOL sub_41AC80()
{
  int v0; // eax
  int v1; // ecx
  BOOL v2; // edx

  v0 = sub_41ACE0();
  v2 = v0 == 0;
  if ( !v0 )
    sub_41ACD0(v1);
  return v2;
}

// ===== sub_41ACA0 @ 0x0041ACA0..0x0041ACC5 =====
BOOL __stdcall sub_41ACA0(int a1)
{
  int v1; // eax
  BOOL v2; // edx

  v1 = sub_41ACE0();
  v2 = v1 == a1;
  if ( v1 == a1 )
    sub_41ACD0(0);
  return v2;
}

// ===== sub_41ACD0 @ 0x0041ACD0..0x0041ACD7 =====
int __usercall sub_41ACD0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 284) = a2;
  return result;
}

// ===== sub_41ACE0 @ 0x0041ACE0..0x0041ACE7 =====
int __usercall sub_41ACE0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 284);
}

// ===== sub_41ACF0 @ 0x0041ACF0..0x0041AD0A =====
BOOL sub_41ACF0()
{
  int v0; // eax
  BOOL v1; // edx

  v0 = sub_41AD50();
  v1 = v0 == 0;
  if ( !v0 )
    sub_41AD40();
  return v1;
}

// ===== sub_41AD10 @ 0x0041AD10..0x0041AD35 =====
BOOL __stdcall sub_41AD10(int a1)
{
  int v1; // eax
  BOOL v2; // edx

  v1 = sub_41AD50();
  v2 = v1 == a1;
  if ( v1 == a1 )
    sub_41AD40(0);
  return v2;
}

// ===== sub_41AD40 @ 0x0041AD40..0x0041AD47 =====
int __usercall sub_41AD40@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 304) = a2;
  return result;
}

// ===== sub_41AD50 @ 0x0041AD50..0x0041AD57 =====
int __usercall sub_41AD50@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 304);
}

// ===== sub_41AD60 @ 0x0041AD60..0x0041AD95 =====
int __thiscall sub_41AD60(_DWORD *this, int a2)
{
  bool v2; // zf
  int i; // esi
  int result; // eax

  v2 = this[2] == 0;
  this[1] = a2;
  if ( !v2 )
  {
    for ( i = this[75]; i; i = *(_DWORD *)(i + 12) )
      result = sub_41AD60(a2);
  }
  return result;
}

// ===== sub_41ADA0 @ 0x0041ADA0..0x0041ADD5 =====
int __thiscall sub_41ADA0(_DWORD *this, int a2)
{
  bool v2; // zf
  int i; // esi
  int result; // eax

  v2 = this[4] == 0;
  this[3] = a2;
  if ( !v2 )
  {
    for ( i = this[75]; i; i = *(_DWORD *)(i + 12) )
      result = sub_41ADA0(a2);
  }
  return result;
}

// ===== sub_41ADE0 @ 0x0041ADE0..0x0041ADE4 =====
int __usercall sub_41ADE0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 16) = a2;
  return result;
}

// ===== sub_41ADF0 @ 0x0041ADF0..0x0041ADF4 =====
int __usercall sub_41ADF0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 72) = a2;
  return result;
}

// ===== sub_41AE00 @ 0x0041AE00..0x0041AE2C =====
int __thiscall sub_41AE00(_DWORD *this, int a2)
{
  _DWORD *v2; // esi
  int result; // eax

  v2 = (_DWORD *)this[75];
  for ( this[5] = a2; v2; v2 = (_DWORD *)v2[3] )
    result = (*(int (__thiscall **)(_DWORD, int))(*(_DWORD *)*v2 + 4))(*v2, a2);
  return result;
}

// ===== sub_41AE30 @ 0x0041AE30..0x0041AE60 =====
BOOL __thiscall sub_41AE30(_DWORD *this)
{
  return this[5] && this[1] && !this[3] && this[44] < 0x100u && this[45];
}

// ===== sub_41AE60 @ 0x0041AE60..0x0041AED2 =====
int __thiscall sub_41AE60(_DWORD *this)
{
  int result; // eax
  _DWORD *i; // esi
  _BYTE v4[16]; // [esp+4h] [ebp-14h] BYREF

  if ( this[37] )
  {
    (*(void (__thiscall **)(_DWORD *, _BYTE *))(*this + 36))(this, v4);
    (*(void (__thiscall **)(_DWORD *))(*this + 28))(this);
    result = sub_443240(dword_565B2C);
  }
  for ( i = (_DWORD *)this[75]; i; i = (_DWORD *)i[3] )
    result = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)*i + 12))(*i);
  return result;
}

// ===== sub_41AEE0 @ 0x0041AEE0..0x0041AEF3 =====
int sub_41AEE0()
{
  int result; // eax
  int v1; // ecx

  result = sub_41C1D0();
  if ( result )
    return (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 12))(v1);
  return result;
}

// ===== sub_41AF00 @ 0x0041AF00..0x0041AF56 =====
void *__userpurge sub_41AF00@<eax>(int a1@<edi>, int a2@<esi>, void *Src)
{
  void *result; // eax
  void *v4; // eax

  operator delete[](*(void **)(a1 + 44));
  result = 0;
  if ( a2 )
  {
    *(_DWORD *)(a1 + 40) = a2;
    v4 = operator new[](4 * a2);
    *(_DWORD *)(a1 + 44) = v4;
    return memcpy_0(v4, Src, 4 * a2);
  }
  else
  {
    *(_DWORD *)(a1 + 40) = 0;
    *(_DWORD *)(a1 + 44) = 0;
  }
  return result;
}

// ===== sub_41AF60 @ 0x0041AF60..0x0041AF81 =====
int __usercall sub_41AF60@<eax>(void *a1@<ecx>, int a2@<esi>)
{
  int result; // eax

  result = *(_DWORD *)(a2 + 40);
  if ( result )
  {
    if ( a1 )
    {
      memcpy_0(a1, *(const void **)(a2 + 44), 4 * result);
      return *(_DWORD *)(a2 + 40);
    }
  }
  return result;
}

// ===== sub_41AF90 @ 0x0041AF90..0x0041AFCA =====
int __thiscall sub_41AF90(_DWORD *this, int a2)
{
  unsigned int v2; // esi
  int result; // eax
  int v4; // edx
  _DWORD *i; // ecx

  v2 = this[10];
  result = -1;
  if ( v2 )
  {
    v4 = 0;
    for ( i = (_DWORD *)this[11]; a2 != *i; ++i )
    {
      if ( ++v4 >= v2 )
        return result;
    }
    return v4;
  }
  return result;
}

// ===== sub_41AFD0 @ 0x0041AFD0..0x0041B0AB =====
int __userpurge sub_41AFD0@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, int *a3, int a4)
{
  BOOL v5; // ebx
  int v6; // ecx
  int v7; // edx
  int v8; // ecx
  int v9; // edx
  _DWORD v11[6]; // [esp+Ch] [ebp-2Ch] BYREF
  int v12[4]; // [esp+24h] [ebp-14h] BYREF

  v5 = 0;
  if ( !(*(int (__thiscall **)(int))(*(_DWORD *)a2 + 8))(a2) )
    return 1;
  (*(void (__thiscall **)(int, int *))(*(_DWORD *)a2 + 36))(a2, v12);
  if ( sub_409110(v12, a1 + 6) )
  {
    v5 = sub_4090B0(a3, v12);
    if ( sub_409110(v12, a3) )
    {
      v6 = a1[1];
      v7 = a1[2];
      v11[0] = *a1;
      v11[3] = a1[3];
      v11[1] = v6;
      v8 = a1[4];
      v11[2] = v7;
      v9 = a1[5];
      v11[4] = v8;
      v11[5] = v9;
      sub_4091B0(v11, v12);
      sub_41C190(v12);
      (*(void (__thiscall **)(int, _DWORD *, int *, int))(*(_DWORD *)a2 + 24))(a2, v11, v12, a4);
    }
  }
  return v5;
}

// ===== sub_41B0B0 @ 0x0041B0B0..0x0041B0B4 =====
int __usercall sub_41B0B0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 24);
}

// ===== sub_41B0C0 @ 0x0041B0C0..0x0041B125 =====
int __thiscall sub_41B0C0(_DWORD *this)
{
  unsigned int v2; // ebx
  int v3; // esi
  int v5; // [esp+14h] [ebp-8h]

  sub_41B4C0();
  v2 = sub_41B0B0((int)this);
  if ( v2 >= 8 )
    v2 = 7;
  if ( this[31] )
    LOWORD(v3) = sub_41C100();
  else
    v3 = 4095 - (v5 >> 19);
  return (v3 & 0x1FFF) + this[9] + ((v2 + 8 * (*(int (__thiscall **)(_DWORD *))(*this + 88))(this)) << 13);
}

// ===== sub_41B130 @ 0x0041B130..0x0041B15B =====
_DWORD *__thiscall sub_41B130(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = 0;
  a2[1] = 0;
  a2[2] = this[38] - 1;
  a2[3] = this[39] - 1;
  return result;
}

// ===== sub_41B160 @ 0x0041B160..0x0041B199 =====
_DWORD *__thiscall sub_41B160(void *this, _DWORD *a2)
{
  int v4[2]; // [esp+8h] [ebp-8h] BYREF

  (*(void (__thiscall **)(void *, _DWORD *))(*(_DWORD *)this + 32))(this, a2);
  (*(void (__thiscall **)(void *, int *))(*(_DWORD *)this + 52))(this, v4);
  return sub_409170(v4[1], v4[0], a2);
}

// ===== sub_41B1A0 @ 0x0041B1A0..0x0041B1A7 =====
int __usercall sub_41B1A0@<eax>(int a1@<eax>)
{
  return sub_41B260(a1);
}

// ===== sub_41B1B0 @ 0x0041B1B0..0x0041B1CA =====
int __thiscall sub_41B1B0(void *this, int a2, int a3)
{
  return (*(int (__thiscall **)(void *, int, int, int, int))(*(_DWORD *)this + 40))(this, a2, a3, 1, 1);
}

// ===== sub_41B1D0 @ 0x0041B1D0..0x0041B238 =====
int __thiscall sub_41B1D0(_DWORD *this, int a2, int a3, int a4, int a5)
{
  int result; // eax
  _DWORD *i; // esi

  result = a3;
  this[12] = a2;
  this[13] = a3;
  if ( a4 && this[71] )
    result = sub_41C130();
  if ( a5 )
  {
    for ( i = (_DWORD *)this[75]; i; i = (_DWORD *)i[3] )
      result = (*(int (__thiscall **)(_DWORD, int, int, _DWORD, int))(*(_DWORD *)*i + 40))(
                 *i,
                 a2 + i[1],
                 a3 + i[2],
                 0,
                 1);
  }
  return result;
}

// ===== sub_41B240 @ 0x0041B240..0x0041B255 =====
_DWORD *__thiscall sub_41B240(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = this[12];
  a2[1] = this[13];
  return result;
}

// ===== sub_41B260 @ 0x0041B260..0x0041B2CB =====
int __thiscall sub_41B260(_DWORD *this, int *a2)
{
  int v4; // ecx
  int v5; // ebx
  int v6; // eax
  int v7; // ebx
  int result; // eax
  int v9; // ecx
  int v10; // [esp+10h] [ebp-Ch] BYREF
  int v11; // [esp+14h] [ebp-8h]
  int v12; // [esp+24h] [ebp+8h]

  sub_41B240(this, a2);
  sub_41B310(this);
  v4 = v11;
  *a2 += v10;
  a2[1] += v4;
  v5 = *a2;
  sub_41B360(this);
  v6 = v11 + a2[1];
  v7 = v10 + v5;
  *a2 = v7;
  v12 = v6;
  a2[1] = v6;
  result = sub_41C0E0(&v10, this);
  if ( result )
  {
    v9 = v11 + v12;
    *a2 = v10 + v7;
    a2[1] = v9;
  }
  return result;
}

// ===== sub_41B2D0 @ 0x0041B2D0..0x0041B309 =====
int __thiscall sub_41B2D0(_DWORD *this, int a2, int a3)
{
  _DWORD *v3; // esi
  int result; // eax

  v3 = (_DWORD *)this[75];
  this[14] = a2;
  for ( this[15] = a3; v3; v3 = (_DWORD *)v3[3] )
    result = (*(int (__thiscall **)(_DWORD, int, int))(*(_DWORD *)*v3 + 56))(*v3, a2, a3);
  return result;
}

// ===== sub_41B310 @ 0x0041B310..0x0041B31C =====
_DWORD *__usercall sub_41B310@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  *result = *(_DWORD *)(a2 + 56);
  result[1] = *(_DWORD *)(a2 + 60);
  return result;
}

// ===== sub_41B320 @ 0x0041B320..0x0041B357 =====
int __thiscall sub_41B320(_DWORD *this, int a2, int a3)
{
  int v3; // esi
  int result; // eax

  v3 = this[75];
  this[16] = a2;
  for ( this[17] = a3; v3; v3 = *(_DWORD *)(v3 + 12) )
    result = sub_41B320(a2, a3);
  return result;
}

// ===== sub_41B360 @ 0x0041B360..0x0041B36C =====
_DWORD *__usercall sub_41B360@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  *result = *(_DWORD *)(a2 + 64);
  result[1] = *(_DWORD *)(a2 + 68);
  return result;
}

// ===== sub_41B370 @ 0x0041B370..0x0041B46C =====
signed int __thiscall sub_41B370(_DWORD *this, signed int a2, signed int a3, int a4)
{
  signed int result; // eax
  signed int v6; // edi
  bool v7; // zf
  _DWORD *i; // esi
  int v9; // [esp+10h] [ebp-28h] BYREF
  int v10; // [esp+14h] [ebp-24h]
  int v11; // [esp+18h] [ebp-20h]
  int v12; // [esp+1Ch] [ebp-1Ch]
  int v13; // [esp+20h] [ebp-18h]
  _DWORD v14[4]; // [esp+24h] [ebp-14h] BYREF

  if ( sub_41BED0(&v9) && (!a4 || v9 == 1) )
  {
    result = (a2 + 0x8000) & 0xFFFF0000;
    a2 = result;
    v6 = (a3 + 0x8000) & 0xFFFF0000;
  }
  else
  {
    v6 = a3;
    result = a2;
  }
  v7 = this[31] == 0;
  v10 = this[19];
  v11 = this[20];
  v12 = this[21];
  v13 = this[22];
  this[19] = result;
  this[20] = v6;
  this[21] = a4;
  if ( !v7 )
    result = (*(int (__thiscall **)(_DWORD *, signed int, signed int, int, int))(*this + 40))(
               this,
               result >> 16,
               v6 >> 16,
               1,
               1);
  for ( i = (_DWORD *)this[75]; i; i = (_DWORD *)i[3] )
  {
    (*(void (__thiscall **)(_DWORD, _DWORD *))(*(_DWORD *)*i + 64))(*i, v14);
    result = (*(int (__thiscall **)(_DWORD, int, int, int))(*(_DWORD *)*i + 60))(
               *i,
               a2 + v14[0] - v10,
               v6 + v14[1] - v11,
               a4 + v14[2] - v12);
  }
  return result;
}

// ===== sub_41B470 @ 0x0041B470..0x0041B48A =====
int __thiscall sub_41B470(_DWORD *this, int a2, int a3)
{
  return (*(int (__thiscall **)(_DWORD *, int, int, _DWORD))(*this + 60))(this, a2, a3, this[21]);
}

// ===== sub_41B490 @ 0x0041B490..0x0041B4B1 =====
_DWORD *__thiscall sub_41B490(_DWORD *this, _DWORD *a2)
{
  _DWORD *result; // eax

  result = a2;
  *a2 = this[19];
  a2[1] = this[20];
  a2[2] = this[21];
  a2[3] = this[22];
  return result;
}

// ===== sub_41B4C0 @ 0x0041B4C0..0x0041B512 =====
int __usercall sub_41B4C0@<eax>(_DWORD *a1@<edi>, int *a2@<esi>)
{
  int v2; // ebx
  int result; // eax
  int v4; // [esp+8h] [ebp-10h]
  int v5; // [esp+Ch] [ebp-Ch]
  int v6; // [esp+10h] [ebp-8h]

  sub_41B490(a1, a2);
  sub_41B560(a1);
  *a2 += v4;
  a2[1] += v5;
  v2 = *a2;
  a2[2] += v6;
  sub_41B5E0(a1);
  a2[1] += v5;
  result = v6 + a2[2];
  *a2 = v4 + v2;
  a2[2] = result;
  return result;
}

// ===== sub_41B520 @ 0x0041B520..0x0041B55F =====
int __thiscall sub_41B520(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax
  _DWORD *v5; // esi

  result = a4;
  v5 = (_DWORD *)this[75];
  this[23] = a2;
  this[24] = a3;
  for ( this[25] = a4; v5; v5 = (_DWORD *)v5[3] )
    result = (*(int (__thiscall **)(_DWORD, int, int, int))(*(_DWORD *)*v5 + 68))(*v5, a2, a3, a4);
  return result;
}

// ===== sub_41B560 @ 0x0041B560..0x0041B578 =====
_DWORD *__usercall sub_41B560@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  *result = a2[23];
  result[1] = a2[24];
  v2 = a2[25];
  v3 = a2[26];
  result[2] = v2;
  result[3] = v3;
  return result;
}

// ===== sub_41B580 @ 0x0041B580..0x0041B5D7 =====
int __thiscall sub_41B580(_DWORD *this, int a2, int a3, int a4)
{
  int v5; // esi

  v5 = this[75];
  this[27] = a2;
  this[28] = a3;
  for ( this[29] = a4; v5; v5 = *(_DWORD *)(v5 + 12) )
    sub_41B580(a2, a3, a4);
  return (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD, _DWORD))(*this + 68))(this, this[23], this[24], this[25]);
}

// ===== sub_41B5E0 @ 0x0041B5E0..0x0041B5F8 =====
_DWORD *__usercall sub_41B5E0@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  *result = a2[27];
  result[1] = a2[28];
  v2 = a2[29];
  v3 = a2[30];
  result[2] = v2;
  result[3] = v3;
  return result;
}

// ===== sub_41B600 @ 0x0041B600..0x0041B607 =====
int __usercall sub_41B600@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 168) = a2;
  return result;
}

// ===== sub_41B610 @ 0x0041B610..0x0041B617 =====
int __usercall sub_41B610@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 168);
}

// ===== sub_41B620 @ 0x0041B620..0x0041B64F =====
int __thiscall sub_41B620(_DWORD *this, int a2)
{
  _DWORD *v2; // esi
  int result; // eax

  v2 = (_DWORD *)this[75];
  for ( this[43] = a2; v2; v2 = (_DWORD *)v2[3] )
    result = (*(int (__thiscall **)(_DWORD, int))(*(_DWORD *)*v2 + 72))(*v2, a2);
  return result;
}

// ===== sub_41B650 @ 0x0041B650..0x0041B657 =====
int __thiscall sub_41B650(_DWORD *this)
{
  return this[43];
}

// ===== sub_41B660 @ 0x0041B660..0x0041B68D =====
int __thiscall sub_41B660(_DWORD *this, int a2)
{
  int v2; // esi
  int result; // eax

  v2 = this[75];
  for ( this[44] = a2; v2; v2 = *(_DWORD *)(v2 + 12) )
    result = sub_41B660(a2);
  return result;
}

// ===== sub_41B690 @ 0x0041B690..0x0041B697 =====
int __usercall sub_41B690@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 176);
}

// ===== sub_41B6A0 @ 0x0041B6A0..0x0041B6CD =====
int __thiscall sub_41B6A0(_DWORD *this, int a2)
{
  int v2; // esi
  int result; // eax

  v2 = this[75];
  for ( this[45] = a2; v2; v2 = *(_DWORD *)(v2 + 12) )
    result = sub_41B6A0(a2);
  return result;
}

// ===== sub_41B6D0 @ 0x0041B6D0..0x0041B6D7 =====
int __usercall sub_41B6D0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 180);
}

// ===== sub_41B6E0 @ 0x0041B6E0..0x0041B6EB =====
int __fastcall sub_41B6E0(int a1, int a2)
{
  return (*(int (__thiscall **)(int, _DWORD, int))(*(_DWORD *)a1 + 80))(a1, 0, a2);
}

// ===== sub_41B6F0 @ 0x0041B6F0..0x0041B739 =====
int __thiscall sub_41B6F0(_DWORD *this, int a2, int a3)
{
  int result; // eax
  _DWORD *i; // esi

  result = a2 - 1;
  if ( a2 == 1 )
  {
    this[46] = a3;
  }
  else
  {
    result = a3 << 16;
    this[46] = a3 << 16;
  }
  for ( i = (_DWORD *)this[75]; i; i = (_DWORD *)i[3] )
    result = (*(int (__thiscall **)(_DWORD, int, int))(*(_DWORD *)*i + 80))(*i, a2, a3);
  return result;
}

// ===== sub_41B740 @ 0x0041B740..0x0041B747 =====
int sub_41B740()
{
  return sub_41B750();
}

// ===== sub_41B750 @ 0x0041B750..0x0041B762 =====
int __usercall sub_41B750@<eax>(int a1@<eax>, int a2@<ecx>)
{
  if ( a1 == 1 )
    return *(_DWORD *)(a2 + 184);
  else
    return *(unsigned __int16 *)(a2 + 186);
}

// ===== sub_41B770 @ 0x0041B770..0x0041B7DB =====
unsigned int __thiscall sub_41B770(_DWORD *this)
{
  unsigned int result; // eax

  switch ( this[42] )
  {
    case 1:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
      result = 256 - ((unsigned int)(this[45] * (256 - this[43]) * (256 - this[44])) >> 16);
      break;
    case 2:
    case 3:
    case 4:
    case 0xC0:
    case 0xC1:
      result = (unsigned int)(this[43] * this[45] * (256 - this[44])) >> 16;
      break;
    default:
      result = this[43];
      break;
  }
  return result;
}

// ===== sub_41B8B0 @ 0x0041B8B0..0x0041B8CC =====
int __thiscall sub_41B8B0(_DWORD *this, unsigned int a2)
{
  int result; // eax

  result = 0;
  if ( a2 < 0x10000 )
  {
    this[7] = a2;
    return 1;
  }
  return result;
}

// ===== sub_41B8D0 @ 0x0041B8D0..0x0041B8D4 =====
int __thiscall sub_41B8D0(_DWORD *this)
{
  return this[7];
}

// ===== sub_41B8E0 @ 0x0041B8E0..0x0041B9F9 =====
int __thiscall sub_41B8E0(void *this, unsigned int a2, int a3, int a4)
{
  int result; // eax

  if ( a2 > 0xC4 )
  {
    if ( a2 > 0x8100 )
    {
      if ( a2 == 2147418112 )
      {
        sub_41C1C0(a3);
        return 0;
      }
      if ( a2 == 0x7FFFFFFF )
        return sub_41C1E0(a3, this) != 0 ? 0 : -65534;
    }
    else
    {
      switch ( a2 )
      {
        case 0x8100u:
          sub_41BF10(a3);
          return 0;
        case 0x8000u:
          sub_41BEB0(a4);
          return 0;
        case 0x8001u:
          sub_41BEF0(a3);
          return 0;
      }
    }
    return -65535;
  }
  if ( a2 == 196 )
  {
    sub_41ADF0((int)this, a3);
    return 0;
  }
  else
  {
    switch ( a2 )
    {
      case 0u:
        (*(void (__thiscall **)(void *, int, int))(*(_DWORD *)this + 44))(this, a3, a4);
        result = 0;
        break;
      case 1u:
        sub_41B600((int)this, a3);
        result = 0;
        break;
      case 2u:
        (*(void (__thiscall **)(void *, int))(*(_DWORD *)this + 72))(this, a3);
        result = 0;
        break;
      case 0xC0u:
        sub_493130(a3);
        result = 0;
        break;
      case 0xC1u:
        sub_41ADE0((int)this, a3);
        result = 0;
        break;
      default:
        return -65535;
    }
  }
  return result;
}

// ===== sub_41BAE0 @ 0x0041BAE0..0x0041BC53 =====
int __thiscall sub_41BAE0(void *this, int *a2, unsigned int a3)
{
  int v3; // ecx
  int result; // eax
  int v5; // ecx
  int v6; // edx
  _DWORD v7[2]; // [esp+4h] [ebp-1Ch] BYREF
  _DWORD v8[4]; // [esp+Ch] [ebp-14h] BYREF

  if ( a3 > 0x20 )
  {
    switch ( a3 )
    {
      case 0x7FFFFFFFu:
        return sub_41C200(a2) != 0 ? 0 : -65534;
      case 0xFFFFFFFE:
        *a2 = (*(int (__thiscall **)(void *))(*(_DWORD *)this + 28))(this);
        return 0;
      case 0xFFFFFFFF:
        *a2 = sub_41C120();
        return 0;
      default:
        return -65535;
    }
  }
  else if ( a3 == 32 )
  {
    (*(void (__thiscall **)(void *, _DWORD *))(*(_DWORD *)this + 64))(this, v8);
    v5 = v8[1];
    v6 = v8[2];
    *a2 = v8[0];
    a2[1] = v5;
    a2[2] = v6;
    return 0;
  }
  else
  {
    switch ( a3 )
    {
      case 0u:
        (*(void (__thiscall **)(void *, _DWORD *))(*(_DWORD *)this + 48))(this, v7);
        v3 = v7[1];
        *a2 = v7[0];
        a2[1] = v3;
        result = 0;
        break;
      case 1u:
        *a2 = sub_41B610((int)this);
        result = 0;
        break;
      case 2u:
        *a2 = (*(int (__thiscall **)(void *))(*(_DWORD *)this + 76))(this);
        result = 0;
        break;
      case 3u:
        *a2 = (*(int (__thiscall **)(void *))(*(_DWORD *)this + 88))(this);
        result = 0;
        break;
      default:
        return -65535;
    }
  }
  return result;
}

// ===== sub_41BC70 @ 0x0041BC70..0x0041BDC0 =====
char *__userpurge sub_41BC70@<eax>(int a1@<esi>, char *a2)
{
  char *result; // eax
  int v3; // eax
  unsigned int v4; // eax
  char *v5; // eax
  char *v6; // ebx
  _WORD *v7; // edi
  unsigned int j; // edx
  bool v9; // cf
  unsigned int v10; // eax
  unsigned int v11; // [esp-Ch] [ebp-18h]
  char *v12; // [esp+0h] [ebp-Ch]
  char *i; // [esp+4h] [ebp-8h]
  _WORD *v14; // [esp+8h] [ebp-4h]

  result = *(char **)(a1 + 280);
  if ( result )
  {
    operator delete[](*(void **)(a1 + 280));
    result = 0;
    *(_DWORD *)(a1 + 264) = 0;
    *(_DWORD *)(a1 + 268) = 0;
    *(_DWORD *)(a1 + 272) = 0;
    *(_DWORD *)(a1 + 276) = 0;
    *(_DWORD *)(a1 + 280) = 0;
  }
  if ( a2 )
  {
    v3 = *((_DWORD *)a2 + 2);
    *(_DWORD *)(a1 + 268) = v3;
    v4 = (unsigned int)(v3 + 7) >> 3;
    v11 = v4 * *((_DWORD *)a2 + 3);
    *(_DWORD *)(a1 + 272) = *((_DWORD *)a2 + 3);
    *(_DWORD *)(a1 + 276) = v4;
    v5 = (char *)operator new[](v11);
    *(_DWORD *)(a1 + 280) = v5;
    v6 = v5;
    v12 = v5;
    v14 = *(_WORD **)a2;
    result = *(char **)(a1 + 272);
    for ( i = result; i; v12 = v6 )
    {
      --i;
      v7 = v14;
      result = (char *)memset(v6, 0, *(_DWORD *)(a1 + 276));
      for ( j = 0; j < *(_DWORD *)(a1 + 268); ++j )
      {
        switch ( *((_DWORD *)a2 + 4) )
        {
          case 0:
            v9 = *v7 != 0;
            goto LABEL_11;
          case 1:
            v10 = *(_DWORD *)v7 & 0xFFFFFF;
            goto LABEL_12;
          case 2:
            v10 = *(_DWORD *)v7 & 0xFF000000;
            goto LABEL_12;
          case 3:
            v9 = *(_BYTE *)v7 != 0;
LABEL_11:
            v10 = v9;
LABEL_12:
            if ( v10 )
            {
              v6[j >> 3] |= 1 << (j & 7);
              v6 = v12;
            }
            break;
          default:
            break;
        }
        result = a2;
        v7 = (_WORD *)((char *)v7 + *((_DWORD *)a2 + 5));
      }
      v6 += *(_DWORD *)(a1 + 276);
      v14 = (_WORD *)((char *)v14 + *((_DWORD *)a2 + 1));
    }
    *(_DWORD *)(a1 + 264) = 1;
  }
  else
  {
    *(_DWORD *)(a1 + 264) = 1;
  }
  return result;
}

// ===== sub_41BDD0 @ 0x0041BDD0..0x0041BDE6 =====
char *__usercall sub_41BDD0@<eax>(int a1@<eax>)
{
  char *result; // eax

  result = sub_41BC70(a1, 0);
  *(_DWORD *)(a1 + 264) = 0;
  return result;
}

// ===== sub_41BDF0 @ 0x0041BDF0..0x0041BE73 =====
int __thiscall sub_41BDF0(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax
  int v5; // edx

  if ( !this[66] || a2 < 0 || a3 < 0 || a4 && ((unsigned int)a2 >= this[38] || (unsigned int)a3 >= this[39]) )
    return 0;
  v5 = this[70];
  result = v5 == 0;
  if ( v5 && (unsigned int)a2 < this[67] && (unsigned int)a3 < this[68] )
    return (1 << (a2 & 7)) & *(unsigned __int8 *)((a2 >> 3) + v5 + a3 * this[69]);
  return result;
}

// ===== nullsub_2 @ 0x0041BE80..0x0041BE83 =====
void __stdcall nullsub_2(int a1, int a2, int a3)
{
  ;
}

// ===== sub_41BE90 @ 0x0041BE90..0x0041BE96 =====
int sub_41BE90()
{
  return -2147483647;
}

// ===== sub_41BEA0 @ 0x0041BEA0..0x0041BEA4 =====
int __usercall sub_41BEA0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 124) = a2;
  return result;
}

// ===== sub_41BEB0 @ 0x0041BEB0..0x0041BEC6 =====
int __userpurge sub_41BEB0@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  *(_DWORD *)(result + 128) = a2;
  *(_DWORD *)(result + 132) = a3;
  return result;
}

// ===== sub_41BED0 @ 0x0041BED0..0x0041BEE3 =====
int __usercall sub_41BED0@<eax>(int a1@<eax>, _DWORD *a2@<ecx>)
{
  if ( a2 )
    *a2 = *(_DWORD *)(a1 + 132);
  return *(_DWORD *)(a1 + 128);
}

// ===== sub_41BEF0 @ 0x0041BEF0..0x0041BEF7 =====
int __usercall sub_41BEF0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 136) = a2;
  return result;
}

// ===== sub_41BF00 @ 0x0041BF00..0x0041BF07 =====
int __usercall sub_41BF00@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 136);
}

// ===== sub_41BF10 @ 0x0041BF10..0x0041BF14 =====
int __usercall sub_41BF10@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 36) = a2;
  return result;
}

// ===== sub_41BF20 @ 0x0041BF20..0x0041BF85 =====
BOOL __userpurge sub_41BF20@<eax>(int a1@<eax>, _DWORD *a2, int a3, int a4)
{
  int v5; // eax
  int v6; // ecx

  sub_4302E0();
  a2[2] = a1;
  a2[3] = a3;
  a2[4] = a4;
  v5 = sub_407B30(a4);
  a2[5] = v5;
  a2[1] = a1 * v5;
  if ( a1 && v6 )
  {
    *a2 = sub_4302C0();
    return *a2 != 0;
  }
  else
  {
    *a2 = 0;
    return *a2 != 0;
  }
}

// ===== sub_41BF90 @ 0x0041BF90..0x0041BFC0 =====
BOOL __userpurge sub_41BF90@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3, int a4, int a5)
{
  int v5; // eax

  if ( a1 )
  {
    v5 = *(_DWORD *)(a1 + 16);
  }
  else
  {
    v5 = sub_407B10();
    if ( v5 == 1 )
      v5 = 2;
  }
  return sub_41BF20(a4, a2, a5, v5);
}

// ===== sub_41BFC0 @ 0x0041BFC0..0x0041C019 =====
int __thiscall sub_41BFC0(_DWORD *this, int a2, int a3)
{
  int v3; // eax
  int v4; // ecx
  int v5; // eax
  _DWORD *v6; // ecx
  int v7; // edx
  int result; // eax

  if ( !a2 || !a3 )
    return 0;
  this[38] = a2;
  this[39] = a3;
  v3 = sub_407B10();
  *(_DWORD *)(v4 + 160) = v3;
  v5 = sub_407B30(v3);
  v6[41] = v5;
  v6[37] = v7 * v5;
  result = 1;
  v6[36] = 0;
  return result;
}
