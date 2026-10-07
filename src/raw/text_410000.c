#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_410040 @ 0x00410040..0x00410419 =====
unsigned int __usercall sub_410040@<eax>(
        unsigned int result@<eax>,
        int a2@<ebp>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        __int16 a8)
{
  int v8; // ecx
  int v9; // eax
  unsigned int *v10; // ecx
  __m128 v11; // xmm1
  __m128i v12; // xmm7
  __m128 v13; // xmm6
  int v14; // edx
  __m128i v15; // xmm0
  __m128i v16; // xmm3
  __m128i v17; // xmm2
  float v18; // edx
  int v19; // esi
  double v20; // st7
  double v21; // st7
  int v22; // esi
  __m128 v23; // xmm4
  __m128 v24; // xmm7
  __m128 v25; // xmm5
  unsigned __int32 v26; // esi
  __m128 v27; // xmm0
  __m128i v28; // xmm1
  __m128i v29; // xmm0
  int v30; // edx
  __m128i v31; // xmm7
  int v32; // eax
  unsigned int v33; // esi
  __m128i v34; // xmm1
  __m128i v35; // xmm0
  int v36; // eax
  unsigned int v37; // esi
  __m128i v38; // xmm0
  __m128i v39; // xmm3
  __m128i v40; // xmm0
  bool v41; // zf
  __m128i si128; // [esp-90h] [ebp-9Ch]
  __m128 v43; // [esp-70h] [ebp-7Ch]
  __m128i v44; // [esp-60h] [ebp-6Ch]
  __m128i v45; // [esp-50h] [ebp-5Ch] BYREF
  __m128 v46; // [esp-40h] [ebp-4Ch]
  __m128i v47; // [esp-30h] [ebp-3Ch] BYREF
  unsigned int v48; // [esp-1Ch] [ebp-28h]
  unsigned int v49; // [esp-18h] [ebp-24h]
  int v50; // [esp-14h] [ebp-20h]
  int v51; // [esp-10h] [ebp-1Ch]
  unsigned int *v52; // [esp-Ch] [ebp-18h]
  unsigned int v53; // [esp-8h] [ebp-14h]
  unsigned int v54; // [esp-4h] [ebp-10h]
  int v55; // [esp+0h] [ebp-Ch]
  void *v56; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v55 = a2;
  v56 = retaddr;
  if ( a5 )
  {
    v54 = result - 32 * (a6 >= 0 ? 0 : a6);
    if ( a6 <= 0 )
      v8 = 0;
    else
      v8 = a6 * a3[1];
    v9 = v8 + *a3;
    v10 = (unsigned int *)a3[3];
    v50 = v9;
    v52 = (unsigned int *)(a6 + a5);
    if ( a6 + a5 >= (int)v10 )
      v52 = v10;
    si128 = _mm_load_si128((const __m128i *)&xmmword_4E42A0);
    v47 = 0LL;
    v11 = (__m128)_mm_cvtsi32_si128(0);
    v12 = _mm_cvtsi32_si128(*(unsigned __int16 *)(a4 + 4));
    v53 = _mm_getcsr() & 0xF3FF | 0x400;
    result = (__int16)(16 * a8);
    v13 = (__m128)_mm_unpacklo_epi16((__m128i)v11, (__m128i)v11);
    v14 = a6 <= 0 ? 0 : a6;
    v15 = _mm_cvtsi32_si128(result);
    v16 = _mm_unpacklo_epi16(
            _mm_unpacklo_epi16(_mm_unpacklo_epi16(_mm_cvtsi32_si128(4u), (__m128i)v11), (__m128i)v13),
            _mm_unpacklo_epi16(_mm_unpacklo_epi16(v12, (__m128i)v11), (__m128i)v13));
    v45 = v16;
    v44 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v15, v15), 0);
    _mm_setcsr(v53);
    if ( v14 < (int)v52 )
    {
      v17 = _mm_load_si128(&v47);
      result = v54 + 8;
      v51 = v54 + 8;
      v53 = (unsigned int)v52 - v14;
      do
      {
        LODWORD(v18) = a7 + *(_DWORD *)(result - 8);
        v19 = a7 + *(_DWORD *)(result - 4);
        *(float *)&v54 = v18;
        if ( SLODWORD(v18) <= v19 && SLODWORD(v18) < a3[2] && v19 >= 0 )
        {
          if ( SLODWORD(v18) < 1 )
          {
            v18 = 0.0;
            *(float *)&v54 = (float)(int)v54;
            v21 = *(float *)&v54;
            *(float *)&v48 = *(float *)result - *(float *)(result + 12) * *(float *)&v54;
            *(float *)&v54 = *(float *)(result + 4) - *(float *)(result + 16) * *(float *)&v54;
            v20 = *(float *)(result + 8) - v21 * *(float *)(result + 20);
          }
          else
          {
            v48 = *(unsigned int *)result;
            v54 = *(unsigned int *)(result + 4);
            v20 = *(float *)(result + 8);
          }
          *(float *)&v49 = v20;
          v52 = (unsigned int *)(v50 + 4 * LODWORD(v18));
          v22 = v19 >= a3[2] ? a3[2] : v19 + 1;
          v11.m128_f32[0] = 0.0;
          v13.m128_f32[0] = 0.0;
          v23 = _mm_shuffle_ps((__m128)v48, (__m128)v48, 0);
          v24 = _mm_unpacklo_ps((__m128)v49, v11);
          v11 = _mm_shuffle_ps((__m128)*(unsigned int *)(result + 12), (__m128)*(unsigned int *)(result + 12), 0);
          v25 = _mm_unpacklo_ps(_mm_unpacklo_ps((__m128)v54, v13), v24);
          v43 = v11;
          v11.m128_f32[0] = 0.0;
          v46 = _mm_unpacklo_ps(
                  _mm_unpacklo_ps((__m128)*(unsigned int *)(result + 16), v13),
                  _mm_unpacklo_ps((__m128)*(unsigned int *)(result + 20), v11));
          if ( SLODWORD(v18) < v22 )
          {
            v54 = v22 - LODWORD(v18);
            do
            {
              v26 = *(_DWORD *)(a4 + 12);
              v27 = _mm_rcp_ps(v23);
              v28 = _mm_cvtps_epi32(
                      _mm_mul_ps(
                        _mm_mul_ps(_mm_sub_ps(_mm_add_ps(v27, v27), _mm_mul_ps(_mm_mul_ps(v27, v27), v23)), v25),
                        (__m128)xmmword_4E41D0));
              v47 = _mm_srai_epi32(v28, 0x10u);
              v29 = _mm_srli_epi16(_mm_and_si128(v28, si128), 1u);
              v30 = 0;
              v13 = (__m128)_mm_shufflelo_epi16(v29, 0);
              v31 = _mm_shufflelo_epi16(v29, 170);
              v32 = _mm_cvtsi128_si32(_mm_madd_epi16(_mm_packs_epi32(v47, v17), v16));
              if ( v47.m128i_i32[1] >= v26 )
              {
                v11 = (__m128)v17;
              }
              else
              {
                v33 = *(_DWORD *)(a4 + 8);
                if ( v47.m128i_i32[0] >= v33 )
                {
                  v34 = v17;
                }
                else
                {
                  v34 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v32)), v17);
                  v30 = 1;
                }
                if ( v47.m128i_i32[0] + 1 >= v33 )
                {
                  v35 = v17;
                }
                else
                {
                  v35 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v32 + 4)), v17);
                  v30 = 1;
                }
                v11 = (__m128)_mm_add_epi16(
                                v34,
                                _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v35, v34), 1u), (__m128i)v13));
              }
              v36 = *(_DWORD *)(a4 + 4) + v32;
              if ( (unsigned int)(v47.m128i_i32[1] + 1) >= *(_DWORD *)(a4 + 12) )
              {
                v40 = v17;
              }
              else
              {
                v37 = *(_DWORD *)(a4 + 8);
                if ( v47.m128i_i32[0] >= v37 )
                {
                  v38 = v17;
                }
                else
                {
                  v38 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v36)), v17);
                  v30 = 1;
                }
                if ( v47.m128i_i32[0] + 1 >= v37 )
                {
                  v39 = v17;
                }
                else
                {
                  v39 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v36 + 4)), v17);
                  v30 = 1;
                }
                v40 = _mm_add_epi16(v38, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v39, v38), 1u), (__m128i)v13));
                v16 = _mm_load_si128(&v45);
              }
              if ( v30 )
              {
                v11 = (__m128)_mm_add_epi16(
                                (__m128i)v11,
                                _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v40, (__m128i)v11), 1u), v31));
                v13 = (__m128)_mm_mulhi_epi16(
                                _mm_slli_epi16(
                                  _mm_sub_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(*v52), v17), (__m128i)v11),
                                  4u),
                                v44);
                *v52 = _mm_cvtsi128_si32(_mm_packus_epi16(_mm_add_epi16((__m128i)v11, (__m128i)v13), v17));
              }
              ++v52;
              v41 = v54-- == 1;
              v23 = _mm_add_ps(v23, v43);
              v25 = _mm_add_ps(v46, v25);
            }
            while ( !v41 );
            result = v51;
          }
        }
        v50 += a3[1];
        result += 32;
        v41 = v53-- == 1;
        v51 = result;
      }
      while ( !v41 );
    }
  }
  return result;
}

// ===== sub_410420 @ 0x00410420..0x00410812 =====
unsigned int *__usercall sub_410420@<eax>(
        unsigned int *result@<eax>,
        int a2@<ebp>,
        unsigned int *a3,
        int a4,
        int a5,
        int a6,
        int a7,
        __int16 a8)
{
  int v8; // ecx
  int v9; // eax
  unsigned int *v10; // ecx
  __m128 v11; // xmm1
  __m128i v12; // xmm7
  __m128 v13; // xmm6
  __m128i v14; // xmm0
  __m128i v15; // xmm3
  __m128i v16; // xmm2
  float *v17; // esi
  float v18; // eax
  int v19; // edx
  double v20; // st7
  double v21; // st7
  int v22; // edx
  __m128 v23; // xmm4
  __m128 v24; // xmm7
  __m128 v25; // xmm5
  unsigned int v26; // edx
  __m128 v27; // xmm0
  __m128i v28; // xmm1
  __m128i v29; // xmm0
  __m128i v30; // xmm7
  int v31; // eax
  unsigned int v32; // esi
  __m128i v33; // xmm3
  __m128i v34; // xmm0
  __m128i v35; // xmm3
  int v36; // eax
  unsigned int v37; // edx
  __m128i v38; // xmm0
  __m128i v39; // xmm1
  __m128i v40; // xmm0
  __m128i v41; // xmm3
  __m128i v42; // xmm0
  bool v43; // zf
  __m128i v44; // [esp-80h] [ebp-8Ch]
  __m128 v45; // [esp-70h] [ebp-7Ch]
  __m128i si128; // [esp-60h] [ebp-6Ch]
  __m128i v47; // [esp-50h] [ebp-5Ch] BYREF
  __m128 v48; // [esp-40h] [ebp-4Ch]
  __m128i v49; // [esp-30h] [ebp-3Ch] BYREF
  float v50; // [esp-1Ch] [ebp-28h]
  unsigned int v51; // [esp-18h] [ebp-24h]
  int v52; // [esp-14h] [ebp-20h]
  int v53; // [esp-10h] [ebp-1Ch]
  unsigned int *v54; // [esp-Ch] [ebp-18h]
  unsigned int v55; // [esp-8h] [ebp-14h]
  unsigned int v56; // [esp-4h] [ebp-10h]
  int v57; // [esp+0h] [ebp-Ch]
  void *v58; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v57 = a2;
  v58 = retaddr;
  if ( a5 )
  {
    v56 = (unsigned int)&result[-8 * (a6 >= 0 ? 0 : a6)];
    if ( a6 <= 0 )
      v8 = 0;
    else
      v8 = a6 * a3[1];
    v9 = v8 + *a3;
    v10 = (unsigned int *)a3[3];
    v52 = v9;
    v54 = (unsigned int *)(a6 + a5);
    if ( a6 + a5 >= (int)v10 )
      v54 = v10;
    si128 = _mm_load_si128((const __m128i *)&xmmword_4E42A0);
    v49 = 0LL;
    v11 = (__m128)_mm_cvtsi32_si128(0);
    v12 = _mm_cvtsi32_si128(*(unsigned __int16 *)(a4 + 4));
    v55 = _mm_getcsr() & 0xF3FF | 0x400;
    v13 = (__m128)_mm_unpacklo_epi16((__m128i)v11, (__m128i)v11);
    v14 = _mm_cvtsi32_si128((__int16)(256 - a8));
    v15 = _mm_unpacklo_epi16(
            _mm_unpacklo_epi16(_mm_unpacklo_epi16(_mm_cvtsi32_si128(4u), (__m128i)v11), (__m128i)v13),
            _mm_unpacklo_epi16(_mm_unpacklo_epi16(v12, (__m128i)v11), (__m128i)v13));
    result = a6 <= 0 ? 0 : (unsigned int *)a6;
    v47 = v15;
    v44 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v14, v14), 0);
    _mm_setcsr(v55);
    if ( (int)result < (int)v54 )
    {
      v16 = _mm_load_si128(&v49);
      v17 = (float *)(v56 + 8);
      v53 = v56 + 8;
      v55 = (char *)v54 - (char *)result;
      do
      {
        LODWORD(v18) = a7 + *((_DWORD *)v17 - 2);
        v19 = a7 + *((_DWORD *)v17 - 1);
        *(float *)&v56 = v18;
        if ( SLODWORD(v18) <= v19 && SLODWORD(v18) < (int)a3[2] && v19 >= 0 )
        {
          if ( SLODWORD(v18) < 1 )
          {
            v18 = 0.0;
            *(float *)&v56 = (float)(int)v56;
            v21 = *(float *)&v56;
            v50 = *v17 - v17[3] * *(float *)&v56;
            *(float *)&v56 = v17[1] - v17[4] * *(float *)&v56;
            v20 = v17[2] - v21 * v17[5];
          }
          else
          {
            v50 = *v17;
            v56 = *((unsigned int *)v17 + 1);
            v20 = v17[2];
          }
          *(float *)&v51 = v20;
          v54 = (unsigned int *)(v52 + 4 * LODWORD(v18));
          v22 = v19 >= (int)a3[2] ? a3[2] : v19 + 1;
          v11.m128_f32[0] = 0.0;
          v13.m128_f32[0] = 0.0;
          v23 = _mm_shuffle_ps((__m128)LODWORD(v50), (__m128)LODWORD(v50), 0);
          v24 = _mm_unpacklo_ps((__m128)v51, v11);
          v11 = _mm_shuffle_ps((__m128)*((unsigned int *)v17 + 3), (__m128)*((unsigned int *)v17 + 3), 0);
          v25 = _mm_unpacklo_ps(_mm_unpacklo_ps((__m128)v56, v13), v24);
          v45 = v11;
          v11.m128_f32[0] = 0.0;
          v48 = _mm_unpacklo_ps(
                  _mm_unpacklo_ps((__m128)*((unsigned int *)v17 + 4), v13),
                  _mm_unpacklo_ps((__m128)*((unsigned int *)v17 + 5), v11));
          if ( SLODWORD(v18) < v22 )
          {
            v56 = v22 - LODWORD(v18);
            do
            {
              v26 = *(_DWORD *)(a4 + 12);
              v27 = _mm_rcp_ps(v23);
              v28 = _mm_cvtps_epi32(
                      _mm_mul_ps(
                        _mm_mul_ps(_mm_sub_ps(_mm_add_ps(v27, v27), _mm_mul_ps(_mm_mul_ps(v27, v27), v23)), v25),
                        (__m128)xmmword_4E41D0));
              v49 = _mm_srai_epi32(v28, 0x10u);
              v29 = _mm_srli_epi16(_mm_and_si128(v28, si128), 1u);
              v13 = (__m128)_mm_shufflelo_epi16(v29, 0);
              v30 = _mm_shufflelo_epi16(v29, 170);
              v31 = _mm_cvtsi128_si32(_mm_madd_epi16(_mm_packs_epi32(v49, v16), v15));
              if ( v49.m128i_i32[1] >= v26 )
              {
                v35 = v16;
              }
              else
              {
                v32 = *(_DWORD *)(a4 + 8);
                if ( v49.m128i_i32[0] >= v32 )
                  v33 = v16;
                else
                  v33 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v31)), v16);
                if ( v49.m128i_i32[0] + 1 >= v32 )
                  v34 = v16;
                else
                  v34 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v31 + 4)), v16);
                v17 = (float *)v53;
                v35 = _mm_add_epi16(v33, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v34, v33), 1u), (__m128i)v13));
              }
              v36 = *(_DWORD *)(a4 + 4) + v31;
              if ( v49.m128i_i32[1] + 1 >= v26 )
              {
                v40 = v16;
              }
              else
              {
                v37 = *(_DWORD *)(a4 + 8);
                if ( v49.m128i_i32[0] >= v37 )
                  v38 = v16;
                else
                  v38 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v36)), v16);
                if ( v49.m128i_i32[0] + 1 >= v37 )
                  v39 = v16;
                else
                  v39 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a4 + v36 + 4)), v16);
                v40 = _mm_add_epi16(v38, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v39, v38), 1u), (__m128i)v13));
              }
              v11 = (__m128)_mm_or_si128(v35, v40);
              if ( _mm_extract_epi16((__m128i)v11, 3) )
              {
                v41 = _mm_add_epi16(v35, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v40, v35), 1u), v30));
                v42 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*v54), v16);
                v11 = (__m128)_mm_srli_epi16(_mm_mullo_epi16(_mm_shufflelo_epi16(v41, 255), v44), 1u);
                v13 = (__m128)_mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v41, v42), 1u), (__m128i)v11);
                *v54 = _mm_cvtsi128_si32(_mm_packus_epi16(_mm_add_epi16(v42, (__m128i)v13), v16));
              }
              ++v54;
              v43 = v56-- == 1;
              v23 = _mm_add_ps(v23, v45);
              v15 = _mm_load_si128(&v47);
              v25 = _mm_add_ps(v48, v25);
            }
            while ( !v43 );
          }
        }
        result = a3;
        v52 += a3[1];
        v17 += 8;
        v43 = v55-- == 1;
        v53 = (int)v17;
      }
      while ( !v43 );
    }
  }
  return result;
}

// ===== sub_410820 @ 0x00410820..0x0041092B =====
int __cdecl sub_410820(int a1, float a2, float a3, int a4, int a5, int a6, float a7, float a8, int a9)
{
  int v9; // eax
  int v11; // [esp+24h] [ebp-24h] BYREF
  float v12[2]; // [esp+28h] [ebp-20h] BYREF
  _DWORD v13[5]; // [esp+30h] [ebp-18h] BYREF

  if ( a7 <= 0.0 || a8 <= 0.0 )
    return 19;
  if ( !a9 )
    goto LABEL_5;
  v12[0] = a2;
  v13[2] = a6;
  v12[1] = a3;
  *(float *)&v13[3] = a7;
  v13[1] = a5;
  *(float *)&v13[4] = a8;
  v11 = a1;
  v13[0] = a4;
  if ( !sub_419DE0(sub_41A2E0, &v11, 1, 3, v12, v13) )
  {
LABEL_5:
    v9 = *(_DWORD *)(a1 + 16);
    if ( v9 == *(_DWORD *)(a4 + 16) && (unsigned int)(v9 - 1) <= 1 )
      sub_410930(LODWORD(a2), LODWORD(a3), a5, a6, LODWORD(a7), LODWORD(a8));
  }
  return 0;
}

// ===== sub_410930 @ 0x00410930..0x00410C7D =====
signed int __usercall sub_410930@<eax>(
        int *a1@<eax>,
        float *a2@<ecx>,
        int a3@<ebp>,
        float a4,
        float a5,
        float a6,
        float a7,
        float a8,
        float a9)
{
  float v9; // edx
  int v10; // esi
  int v11; // edi
  int v12; // eax
  float v13; // esi
  float v14; // ecx
  double v15; // st6
  double v16; // st5
  signed int result; // eax
  double v18; // st7
  double v19; // st6
  double v20; // st5
  float v21; // edi
  double v22; // st4
  __m128i v23; // xmm5
  __m128 v24; // xmm6
  int v25; // esi
  double v26; // st3
  __m128 v27; // xmm3
  __m128 v28; // xmm4
  int v29; // edi
  __m128 v30; // xmm1
  __m128 v31; // xmm0
  __m128 v32; // xmm2
  int v33; // ecx
  int v34; // eax
  __m128 v35; // xmm1
  __m128 v36; // xmm2
  __m128 v37; // xmm0
  __m128 v38; // xmm0
  float v39; // ecx
  __m128 v40; // xmm0
  bool v41; // zf
  __m128 v42; // [esp-B0h] [ebp-BCh]
  __m128i v43; // [esp-70h] [ebp-7Ch] BYREF
  __m128 v44; // [esp-60h] [ebp-6Ch]
  __m128 v45; // [esp-50h] [ebp-5Ch]
  int v46; // [esp-3Ch] [ebp-48h]
  int v47; // [esp-38h] [ebp-44h]
  float v48; // [esp-34h] [ebp-40h]
  int v49; // [esp-30h] [ebp-3Ch]
  int v50; // [esp-2Ch] [ebp-38h]
  int v51; // [esp-28h] [ebp-34h]
  int v52; // [esp-24h] [ebp-30h]
  int v53; // [esp-20h] [ebp-2Ch]
  int v54; // [esp-1Ch] [ebp-28h]
  float v55; // [esp-18h] [ebp-24h]
  float v56; // [esp-14h] [ebp-20h]
  int v57; // [esp-10h] [ebp-1Ch]
  float v58; // [esp-Ch] [ebp-18h]
  float v59; // [esp-8h] [ebp-14h]
  unsigned int v60; // [esp-4h] [ebp-10h]
  int v61; // [esp+0h] [ebp-Ch]
  void *v62; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v61 = a3;
  v62 = retaddr;
  v9 = *a2;
  v49 = *a1;
  v46 = *((_DWORD *)a2 + 1);
  v10 = a1[1];
  v11 = a1[2];
  v12 = a1[3];
  v53 = v10;
  v13 = a2[2];
  v14 = a2[3];
  v47 = v12;
  v43 = 0LL;
  v48 = v13;
  v50 = v11;
  v59 = 1.0 / a8;
  v55 = 1.0 / a9;
  v15 = v59;
  v16 = v59 * a4;
  result = _mm_getcsr() & 0xF3FF;
  v59 = *(float *)&result;
  _mm_setcsr(result);
  v18 = v15;
  v56 = a6 - v16;
  v19 = v55;
  v58 = a7 - v55 * a5;
  if ( v14 != 0.0 )
  {
    v20 = v56;
    v21 = v9;
    v55 = v9;
    v59 = v14;
    do
    {
      *(float *)&v60 = v20;
      v22 = v58;
      if ( v13 != 0.0 )
      {
        v23 = _mm_load_si128(&v43);
        v24 = (__m128)xmmword_4E4250;
        result = (int)v22 + 2;
        v51 = (int)v22 - 1;
        v54 = result;
        v56 = v21;
        v58 = v13;
        do
        {
          v25 = v51;
          v26 = *(float *)&v60;
          v27 = 0LL;
          v28 = 0LL;
          v45 = 0LL;
          v44 = 0LL;
          v57 = v51;
          if ( v51 <= result )
          {
            v29 = v53 * v51;
            do
            {
              *(float *)&v60 = v22 - (double)v57;
              v30 = _mm_andnot_ps((__m128)xmmword_4E4230, _mm_shuffle_ps((__m128)v60, (__m128)v60, 0));
              if ( v30.m128_f32[0] < 2.0 )
              {
                v31 = _mm_mul_ps(v30, v30);
                v32 = v30.m128_f32[0] >= v24.m128_f32[0]
                    ? _mm_sub_ps(
                        _mm_add_ps(
                          _mm_sub_ps((__m128)xmmword_4E4280, _mm_mul_ps((__m128)xmmword_4E4270, v30)),
                          _mm_mul_ps((__m128)xmmword_4E4290, v31)),
                        _mm_mul_ps(v31, v30))
                    : _mm_add_ps(_mm_sub_ps(v24, _mm_add_ps(v31, v31)), _mm_mul_ps(v31, v30));
                v42 = v32;
                if ( v32.m128_f32[0] != 0.0 )
                {
                  v23 = _mm_load_si128(&v43);
                  v28 = v45;
                  v27 = v44;
                  v24 = (__m128)xmmword_4E4250;
                  v33 = (int)v26 - 1;
                  v34 = (int)v26 + 2;
                  v57 = v33;
                  v52 = v34;
                  if ( v33 <= v34 )
                  {
                    do
                    {
                      *(float *)&v60 = v26 - (double)v57;
                      v35 = _mm_andnot_ps((__m128)xmmword_4E4230, _mm_shuffle_ps((__m128)v60, (__m128)v60, 0));
                      if ( v35.m128_f32[0] < 2.0 )
                      {
                        v36 = _mm_mul_ps(v35, v35);
                        if ( v35.m128_f32[0] >= 1.0 )
                          v37 = _mm_sub_ps(
                                  _mm_add_ps(
                                    _mm_sub_ps((__m128)xmmword_4E4280, _mm_mul_ps((__m128)xmmword_4E4270, v35)),
                                    _mm_mul_ps((__m128)xmmword_4E4290, v36)),
                                  _mm_mul_ps(v35, v36));
                        else
                          v37 = _mm_add_ps(
                                  _mm_sub_ps((__m128)xmmword_4E4250, _mm_add_ps(v36, v36)),
                                  _mm_mul_ps(v35, v36));
                        if ( v37.m128_f32[0] != 0.0 && v33 >= 0 && v33 < v50 && v25 >= 0 && v25 < v47 )
                        {
                          v38 = _mm_mul_ps(v37, v42);
                          v34 = v52;
                          v28 = _mm_add_ps(
                                  v28,
                                  _mm_mul_ps(
                                    _mm_cvtepi32_ps(
                                      _mm_unpacklo_epi16(
                                        _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(v29 + 4 * v33 + v49)), v23),
                                        v23)),
                                    v38));
                          v27 = _mm_add_ps(v27, v38);
                        }
                      }
                      v57 = ++v33;
                    }
                    while ( v33 <= v34 );
                    v44 = v27;
                    v45 = v28;
                  }
                }
              }
              result = v54;
              v29 += v53;
              v57 = ++v25;
            }
            while ( v25 <= v54 );
          }
          v39 = v56;
          v40 = _mm_rcp_ps(v27);
          *(float *)&v60 = v26 + v18;
          *(_DWORD *)LODWORD(v56) = _mm_cvtsi128_si32(
                                      _mm_packus_epi16(
                                        _mm_packs_epi32(
                                          _mm_cvtps_epi32(
                                            _mm_mul_ps(
                                              _mm_sub_ps(_mm_add_ps(v40, v40), _mm_mul_ps(_mm_mul_ps(v40, v40), v27)),
                                              v28)),
                                          v23),
                                        v23));
          v41 = LODWORD(v58)-- == 1;
          LODWORD(v56) = LODWORD(v39) + 4;
        }
        while ( !v41 );
        v13 = v48;
      }
      LODWORD(v21) = v46 + LODWORD(v55);
      v41 = LODWORD(v59)-- == 1;
      v58 = v22 + v19;
      LODWORD(v55) += v46;
    }
    while ( !v41 );
  }
  return result;
}

// ===== sub_410C80 @ 0x00410C80..0x00410CAF =====
int *__usercall sub_410C80@<eax>(int *result@<eax>, int a2@<edx>, int a3@<edi>, _DWORD *a4)
{
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  if ( result[4] == 1 && a4[4] == 1 )
  {
    if ( a2 )
      return (int *)sub_4116C0(a4, a3);
    else
      return (int *)sub_40DB60(result, a4, (int)&savedregs, a3);
  }
  return result;
}

// ===== sub_410CB0 @ 0x00410CB0..0x00410CDB =====
int __usercall sub_410CB0@<eax>(int result@<eax>, int a2@<ecx>, int a3@<edi>, int a4@<esi>)
{
  int v4; // edx
  int v5; // edx

  v4 = *(_DWORD *)(a2 + 16);
  if ( *(_DWORD *)(a4 + 16) == v4 )
  {
    v5 = v4 - 1;
    if ( v5 )
    {
      if ( v5 == 1 )
        return sub_410F50(a3, result);
    }
    else
    {
      return sub_410CE0(a4, a3);
    }
  }
  return result;
}

// ===== sub_410CE0 @ 0x00410CE0..0x00410F4C =====
int __usercall sub_410CE0@<eax>(int result@<eax>, int *a2@<ecx>, int a3@<ebp>, unsigned __int32 *a4, int a5)
{
  unsigned __int32 v5; // edx
  unsigned __int32 v6; // ecx
  __m64 v7; // mm2
  __m64 v8; // mm3
  _DWORD *v9; // edi
  _DWORD *v10; // eax
  unsigned __int32 v11; // esi
  int v12; // ecx
  __m64 v13; // mm0
  __int32 v14; // edi
  __m128i v15; // xmm1
  __m128i v16; // xmm2
  __m128i v17; // xmm3
  _QWORD *v18; // esi
  unsigned __int32 v19; // ecx
  _QWORD *v20; // eax
  int v21; // edx
  __m128i v22; // xmm0
  __m128i v23; // [esp-50h] [ebp-5Ch] BYREF
  __m128i v24; // [esp-40h] [ebp-4Ch] BYREF
  int v25; // [esp-24h] [ebp-30h]
  unsigned __int32 v26; // [esp-20h] [ebp-2Ch]
  unsigned __int32 v27; // [esp-1Ch] [ebp-28h]
  __m64 v28; // [esp-18h] [ebp-24h]
  int v29; // [esp-Ch] [ebp-18h]
  unsigned __int32 v30; // [esp-8h] [ebp-14h]
  int v31; // [esp-4h] [ebp-10h]
  int v32; // [esp+0h] [ebp-Ch]
  void *v33; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v32 = a3;
  v33 = retaddr;
  v27 = *a4;
  v31 = *a2;
  v5 = a4[3];
  v26 = a4[1];
  v25 = a2[1];
  if ( v5 > a2[3] )
    v5 = a2[3];
  v28.m64_i32[1] = v5;
  v6 = a2[2];
  if ( a4[2] <= v6 )
    v6 = a4[2];
  v30 = v6;
  if ( (v6 & 1) != 0 )
  {
    v28.m64_i32[1] = BYTE2(a5);
    v28.m64_i16[1] = BYTE1(a5);
    v28.m64_i16[0] = (unsigned __int8)a5;
    v7 = v28;
    v28.m64_i32[1] = (unsigned __int16)result;
    v28.m64_i16[1] = result;
    v28.m64_i16[0] = result;
    v8 = _m_psrawi(v28, 1u);
    if ( v5 )
    {
      v9 = (_DWORD *)v27;
      do
      {
        v27 = --v5;
        v10 = v9;
        v11 = v6;
        if ( v6 )
        {
          v12 = v31 - (_DWORD)v9;
          do
          {
            v13 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)((char *)v10 + v12)), 0LL);
            *v10++ = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v13, _m_psrawi(_m_pmullw(_m_psubw(_m_pxor(v13, v7), v13), v8), 7u)), 0LL));
            --v11;
          }
          while ( v11 );
          v6 = v30;
          v5 = v27;
        }
        result = v25;
        v9 = (_DWORD *)((char *)v9 + v26);
        v31 += v25;
      }
      while ( v5 );
    }
    _m_empty();
  }
  else
  {
    v30 = v6 >> 1;
    v29 = (unsigned __int8)a5;
    v24 = 0LL;
    v23 = _mm_unpacklo_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(_mm_cvtsi32_si128((unsigned __int8)a5), _mm_cvtsi32_si128((unsigned __int8)a5)),
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(BYTE2(a5)), _mm_cvtsi32_si128(BYTE2(a5)))),
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(BYTE1(a5)), _mm_cvtsi32_si128(BYTE1(a5))),
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    v14 = v28.m64_i32[1];
    v15 = _mm_srai_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result)),
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result))),
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0)))),
            1u);
    if ( v28.m64_i32[1] )
    {
      v16 = _mm_load_si128(&v24);
      v17 = _mm_load_si128(&v23);
      v18 = (_QWORD *)v27;
      do
      {
        v19 = v30;
        --v14;
        v20 = v18;
        if ( v30 )
        {
          v21 = v31 - (_DWORD)v18;
          do
          {
            v22 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)((char *)v20 + v21)), v16);
            *v20++ = _mm_packus_epi16(
                       _mm_add_epi16(
                         v22,
                         _mm_srai_epi16(_mm_mullo_epi16(_mm_sub_epi16(_mm_xor_si128(v22, v17), v22), v15), 7u)),
                       v16).m128i_u64[0];
            --v19;
          }
          while ( v19 );
        }
        result = v25;
        v18 = (_QWORD *)((char *)v18 + v26);
        v31 += v25;
      }
      while ( v14 );
    }
  }
  return result;
}

// ===== sub_410F50 @ 0x00410F50..0x0041105D =====
_DWORD *__usercall sub_410F50@<eax>(int *a1@<eax>, _DWORD *a2@<ecx>, int a3, unsigned __int16 a4)
{
  unsigned int v4; // edx
  unsigned int v5; // ecx
  unsigned int v6; // eax
  _DWORD *result; // eax
  __m64 v8; // mm2
  __m64 v9; // mm3
  unsigned int i; // esi
  unsigned int v11; // edx
  unsigned int v12; // ecx
  __m64 v13; // mm0
  __m64 v14; // [esp+8h] [ebp-20h]
  int v15; // [esp+14h] [ebp-14h]
  int v16; // [esp+18h] [ebp-10h]
  _DWORD *v17; // [esp+1Ch] [ebp-Ch]
  int v18; // [esp+20h] [ebp-8h]
  unsigned int v19; // [esp+24h] [ebp-4h]

  v17 = (_DWORD *)*a2;
  v18 = *a1;
  v16 = a2[1];
  v15 = a1[1];
  v4 = a2[3];
  if ( v4 > a1[3] )
    v4 = a1[3];
  v5 = a2[2];
  v6 = a1[2];
  if ( v5 > v6 )
  {
    v19 = v6;
    v5 = v6;
  }
  else
  {
    v19 = v5;
  }
  v14.m64_i32[1] = BYTE2(a3);
  v14.m64_i16[1] = BYTE1(a3);
  v14.m64_i16[0] = (unsigned __int8)a3;
  result = 0;
  v8 = v14;
  v14.m64_i32[1] = a4;
  v14.m64_i16[1] = a4;
  v14.m64_i16[0] = a4;
  v9 = _m_psrawi(v14, 1u);
  for ( i = v4; i; v17 = (_DWORD *)((char *)v17 + v16) )
  {
    --i;
    result = v17;
    v11 = v5;
    if ( v5 )
    {
      do
      {
        v12 = *(_DWORD *)((char *)result + v18 - (_DWORD)v17);
        --v11;
        if ( (v12 & 0xFF000000) != 0 )
        {
          v13 = _m_punpcklbw(_mm_cvtsi32_si64(v12), 0LL);
          *result = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v13, _m_psrawi(_m_pmullw(_m_psubw(_m_pxor(v13, v8), v13), v9), 7u)), 0LL));
        }
        else
        {
          *result = 0;
        }
        ++result;
      }
      while ( v11 );
      v5 = v19;
    }
    v18 += v15;
  }
  _m_empty();
  return result;
}

// ===== sub_411060 @ 0x00411060..0x0041108B =====
int __usercall sub_411060@<eax>(int result@<eax>, int a2@<ecx>, int a3@<edi>, int a4@<esi>)
{
  int v4; // edx
  int v5; // edx

  v4 = *(_DWORD *)(a2 + 16);
  if ( *(_DWORD *)(a4 + 16) == v4 )
  {
    v5 = v4 - 1;
    if ( v5 )
    {
      if ( v5 == 1 )
        return sub_4112D0(a3, result);
    }
    else
    {
      return sub_411090(a4, a3);
    }
  }
  return result;
}

// ===== sub_411090 @ 0x00411090..0x004112C9 =====
int __usercall sub_411090@<eax>(int result@<eax>, int *a2@<ecx>, int a3@<ebp>, _DWORD *a4, unsigned int a5)
{
  _DWORD *v5; // edi
  unsigned __int32 v6; // edx
  unsigned __int32 v7; // esi
  unsigned int v8; // edx
  unsigned int v9; // ecx
  __m64 v10; // mm4
  __m64 v11; // mm5
  __int32 v12; // ecx
  _DWORD *v13; // eax
  unsigned int v14; // esi
  int v15; // ecx
  __m64 v16; // mm0
  __m64 v17; // mm7
  __int32 v18; // ecx
  unsigned int v19; // edx
  __m128i v20; // xmm3
  __m128i v21; // xmm2
  __m128i v22; // xmm4
  __m128i v23; // xmm5
  _QWORD *v24; // eax
  unsigned int v25; // esi
  int v26; // ecx
  __m128i v27; // xmm1
  __m128i v28; // xmm7
  __m128i v29; // [esp-50h] [ebp-5Ch] BYREF
  __m128i si128; // [esp-40h] [ebp-4Ch] BYREF
  __m128i v31; // [esp-30h] [ebp-3Ch] BYREF
  int v32; // [esp-18h] [ebp-24h]
  int v33; // [esp-14h] [ebp-20h]
  __m64 v34; // [esp-10h] [ebp-1Ch]
  unsigned int v35; // [esp-8h] [ebp-14h]
  int v36; // [esp-4h] [ebp-10h]
  int v37; // [esp+0h] [ebp-Ch]
  void *v38; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v37 = a3;
  v38 = retaddr;
  v36 = *a2;
  v33 = a4[1];
  v5 = (_DWORD *)*a4;
  v6 = a4[3];
  v32 = a2[1];
  v7 = a2[3];
  if ( v6 > v7 )
  {
    v34.m64_i32[1] = a2[3];
  }
  else
  {
    v7 = v6;
    v34.m64_i32[1] = v6;
  }
  v8 = a4[2];
  v9 = a2[2];
  if ( v8 > v9 )
    v8 = v9;
  v35 = v8;
  if ( (v8 & 1) != 0 )
  {
    v34.m64_i32[1] = (unsigned __int16)result;
    v34.m64_i16[1] = result;
    v34.m64_i16[0] = result;
    v10 = _m_punpcklbw(_mm_cvtsi32_si64(a5), 0LL);
    v11 = _m_psrlwi(v34, 1u);
    v12 = v7;
    if ( v7 )
    {
      do
      {
        v34.m64_i32[1] = --v12;
        v13 = v5;
        v14 = v8;
        if ( v8 )
        {
          v15 = v36 - (_DWORD)v5;
          do
          {
            v16 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)((char *)v13 + v15)), 0LL);
            v17 = _m_pmaddwd(v16, (__m64)0x4D0096001DLL);
            *v13++ = _mm_cvtsi64_si32(
                       _m_packuswb(
                         _m_paddw(
                           v16,
                           _m_psrawi(
                             _m_pmullw(
                               _m_psubw(_m_pmulhuw(_m_pshufw(_m_paddd(v17, _m_psrlqi(v17, 0x20u)), 0), v10), v16),
                               v11),
                             7u)),
                         0LL));
            --v14;
          }
          while ( v14 );
          v8 = v35;
          v12 = v34.m64_i32[1];
        }
        result = v32;
        v5 = (_DWORD *)((char *)v5 + v33);
        v36 += v32;
      }
      while ( v12 );
    }
    _m_empty();
  }
  else
  {
    si128 = _mm_load_si128((const __m128i *)&xmmword_4E4220);
    v31 = 0LL;
    v29 = _mm_shuffle_epi32(_mm_unpacklo_epi8(_mm_cvtsi32_si128(a5), (__m128i)0LL), 68);
    result = (unsigned __int16)result;
    v18 = v34.m64_i32[1];
    v19 = v8 >> 1;
    v20 = _mm_srli_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result)),
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result))),
              _mm_unpacklo_epi16(
                _mm_unpacklo_epi16(
                  _mm_cvtsi32_si128((unsigned __int16)result),
                  _mm_cvtsi32_si128((unsigned __int16)result)),
                _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0)))),
            1u);
    if ( v34.m64_i32[1] )
    {
      v21 = _mm_load_si128(&v31);
      v22 = _mm_load_si128(&si128);
      v23 = _mm_load_si128(&v29);
      do
      {
        v34.m64_i32[1] = --v18;
        v24 = v5;
        v25 = v19;
        if ( v19 )
        {
          v26 = v36 - (_DWORD)v5;
          do
          {
            v27 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)((char *)v24 + v26)), v21);
            v28 = _mm_madd_epi16(v27, v22);
            *v24++ = _mm_packus_epi16(
                       _mm_add_epi16(
                         v27,
                         _mm_srai_epi16(
                           _mm_mullo_epi16(
                             _mm_sub_epi16(
                               _mm_mulhi_epu16(
                                 _mm_shufflehi_epi16(
                                   _mm_shufflelo_epi16(_mm_add_epi32(v28, _mm_srli_epi64(v28, 0x20u)), 0),
                                   0),
                                 v23),
                               v27),
                             v20),
                           7u)),
                       v21).m128i_u64[0];
            --v25;
          }
          while ( v25 );
          v18 = v34.m64_i32[1];
        }
        result = v32;
        v5 = (_DWORD *)((char *)v5 + v33);
        v36 += v32;
      }
      while ( v18 );
    }
  }
  return result;
}

// ===== sub_4112D0 @ 0x004112D0..0x004113E2 =====
_DWORD *__usercall sub_4112D0@<eax>(int *a1@<eax>, _DWORD *a2@<ecx>, unsigned int a3, unsigned __int16 a4)
{
  unsigned int v4; // edx
  unsigned int v5; // ecx
  unsigned int v6; // eax
  _DWORD *result; // eax
  __m64 v8; // mm4
  __m64 v9; // mm5
  unsigned int i; // esi
  unsigned int v11; // edx
  unsigned int v12; // ecx
  __m64 v13; // mm0
  __m64 v14; // mm7
  int v15; // [esp+8h] [ebp-20h]
  int v16; // [esp+Ch] [ebp-1Ch]
  __m64 v17; // [esp+10h] [ebp-18h]
  _DWORD *v18; // [esp+1Ch] [ebp-Ch]
  int v19; // [esp+20h] [ebp-8h]
  unsigned int v20; // [esp+24h] [ebp-4h]

  v18 = (_DWORD *)*a2;
  v19 = *a1;
  v16 = a2[1];
  v15 = a1[1];
  v4 = a2[3];
  if ( v4 > a1[3] )
    v4 = a1[3];
  v5 = a2[2];
  v6 = a1[2];
  if ( v5 > v6 )
  {
    v20 = v6;
    v5 = v6;
  }
  else
  {
    v20 = v5;
  }
  result = 0;
  v17.m64_i32[1] = a4;
  v17.m64_i16[1] = a4;
  v17.m64_i16[0] = a4;
  v8 = _m_punpcklbw(_mm_cvtsi32_si64(a3), 0LL);
  v9 = _m_psrlwi(v17, 1u);
  for ( i = v4; i; v18 = (_DWORD *)((char *)v18 + v16) )
  {
    --i;
    result = v18;
    v11 = v5;
    if ( v5 )
    {
      do
      {
        v12 = *(_DWORD *)((char *)result + v19 - (_DWORD)v18);
        --v11;
        if ( (v12 & 0xFF000000) != 0 )
        {
          v13 = _m_punpcklbw(_mm_cvtsi32_si64(v12), 0LL);
          v14 = _m_pmaddwd(v13, (__m64)0x4D0096001DLL);
          *result = _mm_cvtsi64_si32(
                      _m_packuswb(
                        _m_paddw(
                          v13,
                          _m_psrawi(
                            _m_pmullw(
                              _m_psubw(_m_pmulhuw(_m_pshufw(_m_paddd(v14, _m_psrlqi(v14, 0x20u)), 0), v8), v13),
                              v9),
                            7u)),
                        0LL));
        }
        else
        {
          *result = 0;
        }
        ++result;
      }
      while ( v11 );
      v5 = v20;
    }
    v19 += v15;
  }
  _m_empty();
  return result;
}

// ===== sub_4113F0 @ 0x004113F0..0x00411413 =====
int __usercall sub_4113F0@<eax>(int result@<eax>, int a2@<edx>, int a3, int a4)
{
  int v4; // ecx

  v4 = *(_DWORD *)(result + 16);
  if ( *(_DWORD *)(a2 + 16) == v4 && (unsigned int)(v4 - 1) <= 1 )
    return sub_411420(a2, a4);
  return result;
}

// ===== sub_411420 @ 0x00411420..0x0041168A =====
_DWORD *__usercall sub_411420@<eax>(int *a1@<eax>, int a2@<edx>, int a3@<ebp>, _DWORD *a4, int a5)
{
  _DWORD *v5; // ecx
  _DWORD *v6; // edi
  unsigned int v7; // ecx
  unsigned int v8; // eax
  __m64 v9; // mm1
  _DWORD *result; // eax
  _DWORD *v11; // edx
  __m64 v12; // mm2
  unsigned int v13; // esi
  int v14; // edx
  __m128i v15; // xmm0
  __m128i v16; // xmm0
  _DWORD *v17; // edi
  __m128i v18; // xmm0
  __m128i v19; // xmm1
  __m128i v20; // xmm2
  _QWORD *v21; // edx
  __int32 v22; // esi
  _QWORD *v23; // eax
  int v24; // ecx
  __m128i v25; // [esp-40h] [ebp-4Ch] BYREF
  __m128i v26; // [esp-30h] [ebp-3Ch] BYREF
  __m64 v27; // [esp-20h] [ebp-2Ch]
  int v28; // [esp-14h] [ebp-20h]
  int v29; // [esp-10h] [ebp-1Ch]
  _DWORD *v30; // [esp-Ch] [ebp-18h]
  _DWORD *v31; // [esp-8h] [ebp-14h]
  int v32; // [esp-4h] [ebp-10h]
  int v33; // [esp+0h] [ebp-Ch]
  void *v34; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v33 = a3;
  v34 = retaddr;
  v31 = (_DWORD *)*a4;
  v32 = *a1;
  v5 = (_DWORD *)a4[3];
  v29 = a4[1];
  v28 = a1[1];
  v6 = (_DWORD *)a1[3];
  v30 = v5;
  if ( v5 > v6 )
    v30 = v6;
  v7 = a4[2];
  v8 = a1[2];
  if ( v7 > v8 )
    v7 = v8;
  if ( (v7 & 1) != 0 )
  {
    v27.m64_i16[2] = 256 - a5;
    v27.m64_i16[1] = 256 - a5;
    v27.m64_i16[0] = 256 - a5;
    v27.m64_i16[3] = 256;
    v9 = v27;
    v27.m64_i16[3] = 0;
    v27.m64_i16[2] = (a5 * (a2 & 0xFF0000u)) >> 16;
    result = (_DWORD *)((a5 * (a2 & 0xFF00u)) >> 8);
    v27.m64_i16[0] = a5 * (unsigned __int8)a2;
    v11 = v30;
    v27.m64_i16[1] = (__int16)result;
    v12 = v27;
    if ( v30 )
    {
      result = v31;
      do
      {
        v11 = (_DWORD *)((char *)v11 - 1);
        v30 = v11;
        v13 = v7;
        if ( v7 )
        {
          v14 = v32 - (_DWORD)result;
          do
          {
            *result = _mm_cvtsi64_si32(
                        _m_packuswb(
                          _m_psrlwi(
                            _m_paddusw(
                              _m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)((char *)result + v14)), 0LL), v9),
                              v12),
                            8u),
                          0LL));
            ++result;
            --v13;
          }
          while ( v13 );
          v11 = v30;
          result = v31;
        }
        result = (_DWORD *)((char *)result + v29);
        v32 += v28;
        v31 = result;
      }
      while ( v11 );
    }
    _m_empty();
  }
  else
  {
    v27.m64_i32[1] = v7 >> 1;
    v26 = 0LL;
    v15 = _mm_cvtsi32_si128((unsigned __int16)(256 - a5));
    v25 = _mm_unpacklo_epi16(
            _mm_unpacklo_epi16(_mm_unpacklo_epi16(v15, v15), _mm_unpacklo_epi16(v15, v15)),
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(v15, v15),
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0x100u), _mm_cvtsi32_si128(0x100u))));
    result = v30;
    v16 = _mm_cvtsi32_si128((unsigned __int16)(a5 * (unsigned __int8)a2));
    v17 = v30;
    v18 = _mm_unpacklo_epi16(
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(v16, v16),
              _mm_unpacklo_epi16(
                _mm_cvtsi32_si128((a5 * (a2 & 0xFF0000u)) >> 16),
                _mm_cvtsi32_si128((a5 * (a2 & 0xFF0000u)) >> 16))),
            _mm_unpacklo_epi16(
              _mm_unpacklo_epi16(
                _mm_cvtsi32_si128((unsigned __int16)((a5 * (a2 & 0xFF00u)) >> 8)),
                _mm_cvtsi32_si128((unsigned __int16)((a5 * (a2 & 0xFF00u)) >> 8))),
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    if ( v30 )
    {
      v19 = _mm_load_si128(&v26);
      v20 = _mm_load_si128(&v25);
      v21 = v31;
      do
      {
        v22 = v27.m64_i32[1];
        v17 = (_DWORD *)((char *)v17 - 1);
        v23 = v21;
        if ( v27.m64_i32[1] )
        {
          v24 = v32 - (_DWORD)v21;
          do
          {
            *v23 = _mm_packus_epi16(
                     _mm_srli_epi16(
                       _mm_adds_epu16(
                         _mm_mullo_epi16(
                           _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)((char *)v23 + v24)), v19),
                           v20),
                         v18),
                       8u),
                     v19).m128i_u64[0];
            ++v23;
            --v22;
          }
          while ( v22 );
        }
        result = (_DWORD *)v28;
        v21 = (_QWORD *)((char *)v21 + v29);
        v32 += v28;
      }
      while ( v17 );
    }
  }
  return result;
}

// ===== sub_411690 @ 0x00411690..0x004116B3 =====
int __usercall sub_411690@<eax>(int result@<eax>, int a2@<edx>, int a3, int a4)
{
  int v4; // ecx

  v4 = *(_DWORD *)(result + 16);
  if ( *(_DWORD *)(a2 + 16) == v4 && (unsigned int)(v4 - 1) <= 1 )
    return sub_4116C0(a2, a4);
  return result;
}

// ===== sub_4116C0 @ 0x004116C0..0x00411805 =====
__m64 *__usercall sub_4116C0@<eax>(int *a1@<eax>, int a2@<edx>, __m64 **a3, int a4)
{
  __m64 *v4; // edi
  __m64 *v5; // ecx
  __m64 *v6; // eax
  __m64 *result; // eax
  __m64 *v8; // edx
  __m64 *v9; // eax
  __m64 *v10; // esi
  __m64 *v11; // esi
  unsigned int v12; // ecx
  unsigned int v13; // [esp+8h] [ebp-18h]
  __m64 v14; // [esp+8h] [ebp-18h]
  __m64 *v15; // [esp+10h] [ebp-10h]
  __m64 *v16; // [esp+14h] [ebp-Ch]
  __m64 *v17; // [esp+18h] [ebp-8h]
  int v18; // [esp+1Ch] [ebp-4h]
  __m64 *v19; // [esp+28h] [ebp+8h]
  unsigned int v20; // [esp+28h] [ebp+8h]

  v18 = *a1;
  v16 = a3[1];
  v4 = *a3;
  v15 = (__m64 *)a1[1];
  v17 = a3[3];
  if ( (unsigned int)v17 > a1[3] )
    v17 = (__m64 *)a1[3];
  v5 = a3[2];
  v6 = (__m64 *)a1[2];
  if ( v5 > v6 )
    v5 = v6;
  result = (__m64 *)((a4 * (a2 & 0xFF0000u)) >> 24);
  v19 = v5;
  if ( ((unsigned __int8)v5 & 1) != 0 )
  {
    HIWORD(v13) = (unsigned __int8)result;
    result = (__m64 *)((a4 * (a2 & 0xFF00u)) >> 16);
    LOBYTE(v13) = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
    v8 = v17;
    for ( BYTE1(v13) = (_BYTE)result; v8; v18 += (int)v15 )
    {
      v8 = (__m64 *)((char *)v8 - 1);
      v9 = v4;
      v10 = v5;
      if ( v5 )
      {
        do
        {
          v9->m64_i32[0] = _mm_cvtsi64_si32(
                             _m_paddusb(
                               _mm_cvtsi32_si64(*(unsigned __int32 *)((char *)v9->m64_u32 + v18 - (_DWORD)v4)),
                               (__m64)v13));
          v9 = (__m64 *)((char *)v9 + 4);
          v10 = (__m64 *)((char *)v10 - 1);
        }
        while ( v10 );
        v5 = v19;
      }
      result = v15;
      v4 = (__m64 *)((char *)v4 + (_DWORD)v16);
    }
  }
  else
  {
    v20 = (unsigned int)v5 >> 1;
    v11 = v17;
    v14.m64_i16[3] = (unsigned __int8)result;
    v14.m64_i8[5] = (a4 * (a2 & 0xFF00u)) >> 16;
    v14.m64_i8[4] = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
    for ( v14.m64_i16[1] = (unsigned __int8)result; v11; v18 += (int)v15 )
    {
      v12 = v20;
      v11 = (__m64 *)((char *)v11 - 1);
      for ( result = v4; v12; --v12 )
      {
        v14.m64_i8[0] = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
        v14.m64_i8[1] = (a4 * (a2 & 0xFF00u)) >> 16;
        result->m64_u64 = (unsigned __int64)_m_paddusb(*(__m64 *)((char *)result + v18 - (_DWORD)v4), v14);
        ++result;
      }
      v4 = (__m64 *)((char *)v4 + (_DWORD)v16);
    }
  }
  _m_empty();
  return result;
}

// ===== sub_411810 @ 0x00411810..0x00411833 =====
int __usercall sub_411810@<eax>(int result@<eax>, int a2@<edx>, int a3, int a4)
{
  int v4; // ecx

  v4 = *(_DWORD *)(result + 16);
  if ( *(_DWORD *)(a2 + 16) == v4 && (unsigned int)(v4 - 1) <= 1 )
    return sub_411840(a2, a4);
  return result;
}

// ===== sub_411840 @ 0x00411840..0x00411985 =====
__m64 *__usercall sub_411840@<eax>(int *a1@<eax>, int a2@<edx>, __m64 **a3, int a4)
{
  __m64 *v4; // edi
  __m64 *v5; // ecx
  __m64 *v6; // eax
  __m64 *result; // eax
  __m64 *v8; // edx
  __m64 *v9; // eax
  __m64 *v10; // esi
  __m64 *v11; // esi
  unsigned int v12; // ecx
  unsigned int v13; // [esp+8h] [ebp-18h]
  __m64 v14; // [esp+8h] [ebp-18h]
  __m64 *v15; // [esp+10h] [ebp-10h]
  __m64 *v16; // [esp+14h] [ebp-Ch]
  __m64 *v17; // [esp+18h] [ebp-8h]
  int v18; // [esp+1Ch] [ebp-4h]
  __m64 *v19; // [esp+28h] [ebp+8h]
  unsigned int v20; // [esp+28h] [ebp+8h]

  v18 = *a1;
  v16 = a3[1];
  v4 = *a3;
  v15 = (__m64 *)a1[1];
  v17 = a3[3];
  if ( (unsigned int)v17 > a1[3] )
    v17 = (__m64 *)a1[3];
  v5 = a3[2];
  v6 = (__m64 *)a1[2];
  if ( v5 > v6 )
    v5 = v6;
  result = (__m64 *)((a4 * (a2 & 0xFF0000u)) >> 24);
  v19 = v5;
  if ( ((unsigned __int8)v5 & 1) != 0 )
  {
    HIWORD(v13) = (unsigned __int8)result;
    result = (__m64 *)((a4 * (a2 & 0xFF00u)) >> 16);
    LOBYTE(v13) = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
    v8 = v17;
    for ( BYTE1(v13) = (_BYTE)result; v8; v18 += (int)v15 )
    {
      v8 = (__m64 *)((char *)v8 - 1);
      v9 = v4;
      v10 = v5;
      if ( v5 )
      {
        do
        {
          v9->m64_i32[0] = _mm_cvtsi64_si32(
                             _m_psubusb(
                               _mm_cvtsi32_si64(*(unsigned __int32 *)((char *)v9->m64_u32 + v18 - (_DWORD)v4)),
                               (__m64)v13));
          v9 = (__m64 *)((char *)v9 + 4);
          v10 = (__m64 *)((char *)v10 - 1);
        }
        while ( v10 );
        v5 = v19;
      }
      result = v15;
      v4 = (__m64 *)((char *)v4 + (_DWORD)v16);
    }
  }
  else
  {
    v20 = (unsigned int)v5 >> 1;
    v11 = v17;
    v14.m64_i16[3] = (unsigned __int8)result;
    v14.m64_i8[5] = (a4 * (a2 & 0xFF00u)) >> 16;
    v14.m64_i8[4] = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
    for ( v14.m64_i16[1] = (unsigned __int8)result; v11; v18 += (int)v15 )
    {
      v12 = v20;
      v11 = (__m64 *)((char *)v11 - 1);
      for ( result = v4; v12; --v12 )
      {
        v14.m64_i8[0] = (unsigned __int16)(a4 * (unsigned __int8)a2) >> 8;
        v14.m64_i8[1] = (a4 * (a2 & 0xFF00u)) >> 16;
        result->m64_u64 = (unsigned __int64)_m_psubusb(*(__m64 *)((char *)result + v18 - (_DWORD)v4), v14);
        ++result;
      }
      v4 = (__m64 *)((char *)v4 + (_DWORD)v16);
    }
  }
  _m_empty();
  return result;
}

// ===== sub_411990 @ 0x00411990..0x00411B79 =====
int __usercall sub_411990@<eax>(
        _DWORD *a1@<eax>,
        _DWORD *a2@<ecx>,
        _DWORD *a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        int a8,
        int a9)
{
  int v9; // ebx
  int v10; // esi
  int v11; // edx
  int v12; // esi
  int v13; // edi
  int v14; // edx
  int v15; // ecx
  int v16; // esi
  int v17; // ecx
  int v18; // eax
  _DWORD *v19; // eax
  int *v20; // eax
  _DWORD v22[6]; // [esp+10h] [ebp-6Ch] BYREF
  _DWORD v23[6]; // [esp+28h] [ebp-54h] BYREF
  _DWORD v24[6]; // [esp+40h] [ebp-3Ch] BYREF
  int v25[4]; // [esp+58h] [ebp-24h] BYREF
  int v26[5]; // [esp+68h] [ebp-14h] BYREF

  v22[0] = *a3;
  v9 = a1[3];
  v22[1] = a3[1];
  v22[2] = a3[2];
  v22[3] = a3[3];
  v10 = a3[4];
  v22[5] = a3[5];
  v23[0] = *a2;
  v11 = a2[1];
  v22[4] = v10;
  v12 = a2[4];
  v13 = a2[3];
  v23[1] = v11;
  v14 = a2[2];
  v23[5] = a2[5];
  v24[0] = *a1;
  v15 = a1[1];
  v23[4] = v12;
  v16 = a1[2];
  v24[1] = v15;
  v17 = a1[4];
  v18 = a1[5];
  v23[2] = v14;
  v23[3] = v13;
  v24[2] = v16;
  v24[3] = v9;
  v24[4] = v17;
  v24[5] = v18;
  if ( v17 != 3 )
    return 7;
  if ( (v14 != v16 || v13 != v9) && !a9 )
    return 8;
  if ( a7 > 0x100 )
    return 3;
  sub_409190(v25, (int)v22);
  sub_409190(v26, (int)v23);
  sub_409170(-a5, -a4, v25);
  if ( sub_409110(v26, v25) )
  {
    if ( !a9 )
    {
      sub_4091B0(v23, v26);
      sub_4091B0(v24, v26);
      sub_409170(a5, a4, v26);
      goto LABEL_11;
    }
    v19 = sub_409190(v25, (int)v24);
    sub_409170(-a5, -a4, v19);
    if ( sub_409110(v26, v25) )
    {
      sub_4091B0(v23, v26);
      sub_409170(a5, a4, v26);
      v20 = sub_409190(v25, (int)v24);
      sub_409110(v26, v20);
      sub_4091B0(v24, v26);
LABEL_11:
      sub_4091B0(v22, v26);
      return sub_411EA0(v23, v24, a7);
    }
  }
  return 4;
}

// ===== sub_411B80 @ 0x00411B80..0x00411BC8 =====
int __usercall sub_411B80@<eax>(unsigned int a1@<eax>, int a2@<esi>, int a3, int a4)
{
  int v4; // edi

  v4 = *(_DWORD *)(a2 + 16);
  if ( *(_DWORD *)(a3 + 16) != v4 )
    return 1;
  if ( v4 == 1 )
  {
    if ( a1 < 8 )
    {
      sub_411BD0(a2, a1, a4);
      return 0;
    }
    sub_411D10(a2, a1 & 7, a4);
  }
  return 0;
}

// ===== sub_411BD0 @ 0x00411BD0..0x00411D0C =====
unsigned int __fastcall sub_411BD0(unsigned int **a1, unsigned __int8 **a2, unsigned int **a3, char a4, int a5)
{
  int i; // esi
  __int64 v6; // rax
  unsigned int *v7; // edx
  unsigned int *v8; // edi
  unsigned int *v9; // esi
  unsigned __int8 *v10; // ebx
  unsigned int *v11; // edx
  int v12; // eax
  int v13; // eax
  char v14; // cc
  unsigned int result; // eax
  __m64 v16; // mm0
  __m64 v17; // mm1
  unsigned int *v18; // [esp-10h] [ebp-448h]
  unsigned int *v19; // [esp-Ch] [ebp-444h]
  unsigned __int8 *v20; // [esp-8h] [ebp-440h]
  unsigned int *v21; // [esp-4h] [ebp-43Ch]
  unsigned int *v22; // [esp+Ch] [ebp-42Ch]
  unsigned __int8 *v23; // [esp+10h] [ebp-428h]
  unsigned int *v24; // [esp+14h] [ebp-424h]
  unsigned int *v25; // [esp+18h] [ebp-420h]
  unsigned int *v26; // [esp+1Ch] [ebp-41Ch]
  int v27; // [esp+20h] [ebp-418h]
  unsigned int *v28; // [esp+24h] [ebp-414h]
  unsigned __int8 *v29; // [esp+28h] [ebp-410h]
  unsigned int *v30; // [esp+2Ch] [ebp-40Ch]
  __m64 v31[128]; // [esp+30h] [ebp-408h]

  v26 = *a1;
  v28 = *a3;
  v25 = a1[1];
  v22 = a3[1];
  v23 = a2[1];
  v29 = *a2;
  v24 = a3[3];
  v30 = a3[2];
  for ( i = 0; i < 128; ++i )
  {
    v6 = 0x100010001LL * i;
    v31[i].m64_i32[0] = v6;
    v31[i].m64_i32[1] = HIDWORD(v6);
  }
  v27 = 256 - a5 * ((1 << a4) + 1);
  v7 = v24;
  v8 = v26;
  v9 = v28;
  v10 = v29;
  do
  {
    v21 = v7;
    v20 = v10;
    v19 = v9;
    v18 = v8;
    v11 = v30;
    do
    {
      v12 = *v10++;
      v13 = v12 << a4;
      v14 = (v27 + v13 < 0) ^ __OFADD__(v27, v13) | (v27 + v13 == 0);
      result = v27 + v13;
      if ( !v14 )
      {
        v16 = _mm_cvtsi32_si64(*v9);
        if ( result < 0x100 )
        {
          result >>= 1;
          v17 = _m_punpcklbw(_mm_cvtsi32_si64(*v8), 0LL);
          v16 = _m_packuswb(
                  _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v16, 0LL), v17), v31[result]), 7u), v17),
                  0LL);
        }
        *v8 = _mm_cvtsi64_si32(v16);
      }
      ++v8;
      ++v9;
      v11 = (unsigned int *)((char *)v11 - 1);
    }
    while ( v11 );
    v8 = (unsigned int *)((char *)v18 + (_DWORD)v25);
    v9 = (unsigned int *)((char *)v19 + (_DWORD)v22);
    v10 = &v20[(_DWORD)v23];
    v7 = (unsigned int *)((char *)v21 - 1);
  }
  while ( v21 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_411D10 @ 0x00411D10..0x00411E9E =====
int __fastcall sub_411D10(int *a1, int *a2, int *a3, int a4, int a5)
{
  int v5; // esi
  double v6; // st7
  double v7; // st5
  int v8; // eax
  double v9; // st5
  __int64 v10; // rax
  int v11; // ecx
  int v12; // edi
  int v13; // esi
  int v14; // ebx
  int v15; // ecx
  int result; // eax
  int v17; // edx
  char v18; // cc
  unsigned int v19; // edx
  __m64 v20; // mm0
  __m64 v21; // mm1
  int v22; // [esp-8h] [ebp-83Ch]
  int v23; // [esp+8h] [ebp-82Ch]
  int v24; // [esp+Ch] [ebp-828h]
  int v25; // [esp+10h] [ebp-824h]
  int v26; // [esp+14h] [ebp-820h]
  int v27; // [esp+18h] [ebp-81Ch]
  int v28; // [esp+1Ch] [ebp-818h]
  int v29; // [esp+20h] [ebp-814h]
  int v30; // [esp+24h] [ebp-810h]
  int v31; // [esp+28h] [ebp-80Ch]
  int v32; // [esp+28h] [ebp-80Ch]
  __m64 v33[256]; // [esp+2Ch] [ebp-808h]

  v27 = *a1;
  v28 = a1[1];
  v29 = *a3;
  v23 = a3[1];
  v24 = a2[1];
  v25 = a3[3];
  v26 = *a2;
  v30 = a3[2];
  v5 = 0;
  v31 = 0;
  v6 = 256.0 / (double)(unsigned int)(2 * a4 + 1);
  do
  {
    v7 = (double)v31;
    v8 = (int)(v7 / v6);
    v9 = v7 - (double)v8 * v6;
    if ( (v8 & 1) != 0 )
      v9 = v6 - v9;
    v10 = 0x1000100010LL * (int)(v9 * 256.0 / v6);
    v33[v5].m64_i32[0] = v10;
    v33[v5++].m64_i32[1] = HIDWORD(v10);
    v31 = v5;
  }
  while ( v5 < 256 );
  v32 = 2 * (128 - a5);
  v11 = v25;
  v12 = v27;
  v13 = v29;
  v14 = v26;
  do
  {
    v22 = v11;
    v15 = v30;
    result = 0;
    do
    {
      v17 = *(unsigned __int8 *)(v14 + result);
      v18 = (v32 + v17 < 0) ^ __OFADD__(v32, v17) | (v32 + v17 == 0);
      v19 = v32 + v17;
      if ( !v18 )
      {
        v20 = _mm_cvtsi32_si64(*(_DWORD *)(v13 + 4 * result));
        if ( v19 < 0x100 )
        {
          v21 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v12 + 4 * result)), 0LL);
          v20 = _m_packuswb(
                  _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(_m_punpcklbw(v20, 0LL), v21), 4u), v33[v19]), v21),
                  0LL);
        }
        *(_DWORD *)(v12 + 4 * result) = _mm_cvtsi64_si32(v20);
      }
      ++result;
      --v15;
    }
    while ( v15 );
    v12 += v28;
    v13 += v23;
    v14 += v24;
    v11 = v22 - 1;
  }
  while ( v22 != 1 );
  _m_empty();
  return result;
}

// ===== sub_411EA0 @ 0x00411EA0..0x00411F06 =====
int __usercall sub_411EA0@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3@<edi>, int a4, int a5, int a6)
{
  int v6; // edx

  if ( !a3 )
    return sub_411B80(a1, a4, a2, a6);
  v6 = *(_DWORD *)(a4 + 16);
  if ( *(_DWORD *)(a2 + 16) != v6 )
    return 1;
  if ( v6 == 1 )
  {
    if ( a1 < 8 )
    {
      sub_411F10(a4, a1, a6, a3);
      return 0;
    }
    sub_412070(a4, a1 & 7, a6, a3);
  }
  return 0;
}

// ===== sub_411F10 @ 0x00411F10..0x00412066 =====
unsigned int __fastcall sub_411F10(unsigned int **a1, unsigned __int8 **a2, unsigned int **a3, char a4, int a5, int a6)
{
  int v6; // esi
  unsigned int v7; // ebx
  __int64 v8; // rax
  unsigned int *v9; // edx
  unsigned int *v10; // edi
  unsigned int *v11; // esi
  unsigned __int8 *v12; // ebx
  unsigned int *v13; // edx
  int v14; // eax
  int v15; // eax
  char v16; // cc
  unsigned int result; // eax
  __m64 v18; // mm1
  unsigned int *v19; // [esp-10h] [ebp-450h]
  unsigned int *v20; // [esp-Ch] [ebp-44Ch]
  unsigned __int8 *v21; // [esp-8h] [ebp-448h]
  unsigned int *v22; // [esp-4h] [ebp-444h]
  unsigned int *v23; // [esp+Ch] [ebp-434h]
  unsigned __int8 *v24; // [esp+10h] [ebp-430h]
  unsigned int *v25; // [esp+14h] [ebp-42Ch]
  unsigned int *v26; // [esp+18h] [ebp-428h]
  unsigned int *v27; // [esp+1Ch] [ebp-424h]
  int v28; // [esp+20h] [ebp-420h]
  unsigned int *v29; // [esp+24h] [ebp-41Ch]
  unsigned __int8 *v30; // [esp+28h] [ebp-418h]
  unsigned int *v31; // [esp+2Ch] [ebp-414h]
  __m64 v32[129]; // [esp+30h] [ebp-410h]

  v27 = *a1;
  v29 = *a3;
  v26 = a1[1];
  v23 = a3[1];
  v24 = a2[1];
  v30 = *a2;
  v6 = 0;
  v7 = 0;
  v25 = a3[3];
  v31 = a3[2];
  do
  {
    v8 = 0x100010001LL * (v7 >> 8);
    v32[v6].m64_i32[0] = v8;
    v32[v6++].m64_i32[1] = HIDWORD(v8);
    v7 += 256 - a6;
  }
  while ( v6 <= 128 );
  v28 = 256 - a5 * ((1 << a4) + 1);
  v9 = v25;
  v10 = v27;
  v11 = v29;
  v12 = v30;
  do
  {
    v22 = v9;
    v21 = v12;
    v20 = v11;
    v19 = v10;
    v13 = v31;
    do
    {
      v14 = *v12++;
      v15 = v14 << a4;
      v16 = (v28 + v15 < 0) ^ __OFADD__(v28, v15) | (v28 + v15 == 0);
      result = v28 + v15;
      if ( !v16 )
      {
        if ( result > 0x100 )
          result = 256;
        result >>= 1;
        v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v10), 0LL);
        *v10 = _mm_cvtsi64_si32(
                 _m_packuswb(
                   _m_paddw(
                     _m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v11), 0LL), v18), v32[result]), 7u),
                     v18),
                   0LL));
      }
      ++v10;
      ++v11;
      v13 = (unsigned int *)((char *)v13 - 1);
    }
    while ( v13 );
    v10 = (unsigned int *)((char *)v19 + (_DWORD)v26);
    v11 = (unsigned int *)((char *)v20 + (_DWORD)v23);
    v12 = &v21[(_DWORD)v24];
    v9 = (unsigned int *)((char *)v22 - 1);
  }
  while ( v22 != (unsigned int *)1 );
  _m_empty();
  return result;
}

// ===== sub_412070 @ 0x00412070..0x0041221D =====
int __fastcall sub_412070(int *a1, int *a2, int *a3, int a4, int a5, unsigned int a6)
{
  int v6; // esi
  double v7; // st7
  double v8; // st3
  int v9; // eax
  double v10; // st3
  __int64 v11; // rax
  int v12; // ecx
  int v13; // edi
  int v14; // esi
  int v15; // ebx
  int v16; // ecx
  int result; // eax
  int v18; // edx
  char v19; // cc
  unsigned int v20; // edx
  __m64 v21; // mm1
  int v22; // [esp-8h] [ebp-840h]
  int v23; // [esp+Ch] [ebp-82Ch]
  int v24; // [esp+10h] [ebp-828h]
  int v25; // [esp+14h] [ebp-824h]
  int v26; // [esp+18h] [ebp-820h]
  int v27; // [esp+1Ch] [ebp-81Ch]
  int v28; // [esp+20h] [ebp-818h]
  int v29; // [esp+24h] [ebp-814h]
  int v30; // [esp+28h] [ebp-810h]
  int v31; // [esp+2Ch] [ebp-80Ch]
  int v32; // [esp+2Ch] [ebp-80Ch]
  __m64 v33[256]; // [esp+30h] [ebp-808h]

  v28 = *a1;
  v25 = a1[1];
  v29 = *a3;
  v24 = a3[1];
  v23 = a2[1];
  v26 = a3[3];
  v30 = *a2;
  v27 = a3[2];
  v6 = 0;
  v7 = 256.0 / (double)(unsigned int)(2 * a4 + 1);
  v31 = 0;
  do
  {
    v8 = (double)v31;
    v9 = (int)(v8 / v7);
    v10 = v8 - (double)v9 * v7;
    if ( (v9 & 1) != 0 )
      v10 = v7 - v10;
    v11 = 0x1000100010LL * (int)(v10 * 256.0 * (double)a6 / (v7 * 256.0));
    v33[v6].m64_i32[0] = v11;
    v33[v6++].m64_i32[1] = HIDWORD(v11);
    v31 = v6;
  }
  while ( v6 < 256 );
  v32 = 2 * (128 - a5);
  v12 = v26;
  v13 = v28;
  v14 = v29;
  v15 = v30;
  do
  {
    v22 = v12;
    v16 = v27;
    result = 0;
    do
    {
      v18 = *(unsigned __int8 *)(v15 + result);
      v19 = (v32 + v18 < 0) ^ __OFADD__(v32, v18) | (v32 + v18 == 0);
      v20 = v32 + v18;
      if ( !v19 )
      {
        if ( v20 > 0x100 )
          result = 256;
        v21 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v13 + 4 * result)), 0LL);
        *(_DWORD *)(v13 + 4 * result) = _mm_cvtsi64_si32(
                                          _m_packuswb(
                                            _m_paddw(
                                              _m_pmulhw(
                                                _m_psllwi(
                                                  _m_psubw(
                                                    _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v14 + 4 * result)), 0LL),
                                                    v21),
                                                  4u),
                                                v33[v20]),
                                              v21),
                                            0LL));
      }
      ++result;
      --v16;
    }
    while ( v16 );
    v13 += v25;
    v14 += v24;
    v15 += v23;
    v12 = v22 - 1;
  }
  while ( v22 != 1 );
  _m_empty();
  return result;
}

// ===== sub_412220 @ 0x00412220..0x004124CE =====
int __usercall sub_412220@<eax>(_DWORD *a1@<edi>, int a2@<esi>, _DWORD *a3, _DWORD *a4, unsigned int a5, int a6)
{
  unsigned int v6; // ebx
  unsigned int v7; // edx
  _DWORD v9[4]; // [esp+8h] [ebp-20h] BYREF
  _DWORD v10[2]; // [esp+18h] [ebp-10h] BYREF
  _DWORD *v11; // [esp+20h] [ebp-8h]
  _DWORD *v12; // [esp+24h] [ebp-4h]

  v6 = a5;
  if ( a5 > 0x100 )
    return 3;
  if ( a1[4] != 4 )
    return 12;
  v7 = a1[2];
  if ( a3[2] > v7 || a3[3] > a1[3] )
    return 12;
  if ( a4 && (a4[4] != 4 || v7 != a4[2] || a1[3] != a4[3]) )
    return 13;
  if ( a3[4] == *(_DWORD *)(a2 + 16) )
  {
    v10[0] = a3;
    v11 = 0;
    v12 = 0;
    v10[1] = a2;
    sub_409190(v9, a2);
    if ( (unsigned int)(*(_DWORD *)(a2 + 16) - 1) > 1 )
      return 0;
    if ( a4 )
    {
      if ( !a6 )
      {
        sub_412F20(a1, a4, v6);
        return 0;
      }
      if ( v6 )
      {
        if ( v6 != 256 )
        {
          v12 = a4;
          v11 = a1;
          a5 = v6;
          if ( !sub_419DE0(sub_41A0C0, v10, 4, 1, v9, &a5) )
          {
            sub_413040(a3, a4, v6);
            return 0;
          }
          return 0;
        }
        v11 = a4;
        a5 = 256;
        if ( !sub_419DE0(sub_41A0C0, v10, 3, 1, v9, &a5) )
        {
LABEL_28:
          sub_4126F0(a3);
          return 0;
        }
      }
      else
      {
        v11 = a1;
        a5 = 256;
        if ( !sub_419DE0(sub_41A0C0, v10, 3, 1, v9, &a5) )
          goto LABEL_28;
      }
      return 0;
    }
    if ( a6 )
    {
      if ( v6 )
      {
        v11 = a1;
        if ( v6 != 256 )
        {
          a5 = v6;
          if ( !sub_419DE0(sub_41A0C0, v10, 3, 1, v9, &a5) )
          {
            sub_412AE0(a3, v6);
            return 0;
          }
          return 0;
        }
        a5 = 256;
        if ( !sub_419DE0(sub_41A0C0, v10, 3, 1, v9, &a5) )
          goto LABEL_28;
        return 0;
      }
    }
    else if ( v6 )
    {
      sub_4124D0(a3, v6);
      return 0;
    }
    sub_40ADF0((int)a3, (char **)a2);
    return 0;
  }
  return 1;
}

// ===== sub_4124D0 @ 0x004124D0..0x004126E6 =====
int __usercall sub_4124D0@<eax>(int *a1@<eax>, __m64 **a2@<edx>, __m64 **a3, unsigned int a4)
{
  __m64 *v5; // ebx
  __int64 v6; // rax
  unsigned __int64 v7; // rt0
  __m64 *v8; // edi
  __m64 *v9; // ebx
  __m64 *v10; // ecx
  __m64 v11; // mm4
  unsigned int v12; // ecx
  __m64 v13; // mm0
  __m64 v14; // mm4
  __m64 v15; // mm0
  __m64 v16; // mm2
  __m64 v17; // mm0
  __m64 v18; // mm3
  int result; // eax
  __m64 v20; // mm1
  __m64 v21; // mm0
  int *v22; // edi
  unsigned int *v23; // ebx
  __m64 *v24; // ecx
  __m64 v25; // mm4
  __m64 *v26; // ecx
  __m64 v27; // mm4
  __m64 v28; // mm0
  __m64 v29; // mm2
  __m64 v30; // mm0
  __m64 v31; // mm1
  __m64 *v32; // [esp-Ch] [ebp-48h]
  int *v33; // [esp-Ch] [ebp-48h]
  __m64 *v34; // [esp-8h] [ebp-44h]
  unsigned int *v35; // [esp-8h] [ebp-44h]
  __m64 *v36; // [esp-4h] [ebp-40h]
  __m64 *v37; // [esp-4h] [ebp-40h]
  __m64 v38; // [esp+Ch] [ebp-30h]
  __m64 v39; // [esp+14h] [ebp-28h]
  __int64 v40; // [esp+1Ch] [ebp-20h]
  __m64 *v41; // [esp+28h] [ebp-14h]
  int v42; // [esp+2Ch] [ebp-10h]
  __m64 *v43; // [esp+30h] [ebp-Ch]
  __m64 *v44; // [esp+34h] [ebp-8h]
  __m64 *v45; // [esp+38h] [ebp-4h]
  __m64 *v46; // [esp+44h] [ebp+8h]
  __m64 *v47; // [esp+48h] [ebp+Ch]
  unsigned int v48; // [esp+48h] [ebp+Ch]

  v5 = a3[2];
  v41 = *a3;
  v42 = *a1;
  v46 = a2[1];
  v43 = *a2;
  v44 = a3[3];
  v38.m64_i32[0] = (unsigned int)v5 | ((_DWORD)v44 << 16);
  v45 = a3[1];
  v6 = (a1[1] << 16) | 4;
  v39.m64_i32[0] = v6;
  v38.m64_i32[1] = v38.m64_i32[0];
  HIDWORD(v7) = (unsigned __int64)a4 >> 16;
  LODWORD(v7) = a4 | (a4 << 16);
  HIDWORD(v7) = v7 >> 16;
  LODWORD(v7) = a4 | ((_DWORD)v7 << 16);
  HIDWORD(v7) = v7 >> 16;
  LODWORD(v7) = a4 | ((a4 | ((a4 | (a4 << 16)) << 16)) << 16);
  v47 = v5;
  v39.m64_i32[1] = HIDWORD(v6) | v6;
  v40 = 32 * v7;
  if ( ((unsigned __int8)v5 & 1) != 0 || !dword_565AFC || sub_407AE0() )
  {
    v22 = (int *)v41;
    v23 = (unsigned int *)v43;
    v24 = v44;
    v25.m64_u64 = 0LL;
    do
    {
      v37 = v24;
      v35 = v23;
      v33 = v22;
      v26 = v47;
      v27 = _m_pand(v25, _mm_cvtsi32_si64(0xFFFF0000));
      do
      {
        v28 = _m_paddw(_m_pmulhw(_mm_cvtsi32_si64(*v23++), (__m64)v40), _m_psrlwi((__m64)-1LL, 0xFu));
        v29 = _m_paddw(_m_psrawi(v28, 1u), v27);
        v30 = _m_pmaddwd(v29, v39);
        result = _mm_cvtsi64_si32(_m_pcmpeqd(_m_pand(_m_pcmpgtw(v29, (__m64)-1LL), _m_pcmpgtw(v38, v29)), (__m64)-1LL));
        v31 = _m_psrlqi((__m64)-1LL, 0x3Fu);
        if ( result )
          result = *(_DWORD *)(v42 + _mm_cvtsi64_si32(v30));
        v27 = _m_paddw(v27, v31);
        *v22++ = result;
        v26 = (__m64 *)((char *)v26 - 1);
      }
      while ( v26 );
      v25 = _m_paddw(v27, _m_psllqi(v31, 0x10u));
      v22 = (int *)((char *)v33 + (_DWORD)v45);
      v23 = (unsigned int *)((char *)v35 + (_DWORD)v46);
      v24 = (__m64 *)((char *)v37 - 1);
    }
    while ( v37 != (__m64 *)1 );
  }
  else
  {
    v48 = (unsigned int)v5 >> 1;
    v8 = v41;
    v9 = v43;
    v10 = v44;
    v11.m64_u64 = 0LL;
    do
    {
      v36 = v10;
      v34 = v9;
      v32 = v8;
      v12 = v48;
      v13 = _m_pslldi((__m64)-1LL, 0x10u);
      v14 = _m_por(_m_pand(v11, v13), _m_psllqi(_m_psrlqi(v13, 0x3Fu), 0x20u));
      do
      {
        v15 = _m_paddw(_m_pmulhw((__m64)v9->m64_u64, (__m64)v40), _m_psrlwi((__m64)-1LL, 0xFu));
        ++v9;
        v16 = _m_paddw(_m_psrawi(v15, 1u), v14);
        v17 = _m_pmaddwd(v16, v39);
        v18 = _m_paddd((__m64)-1LL, (__m64)-1LL);
        result = _m_pmovmskb(_m_pcmpeqd(_m_pand(_m_pcmpgtw(v16, (__m64)-1LL), _m_pcmpgtw(v38, v16)), (__m64)-1LL));
        v20.m64_u64 = 0LL;
        if ( (result & 0xF) != 0 )
          v20 = _mm_cvtsi32_si64(*(_DWORD *)(v42 + _mm_cvtsi64_si32(v17)));
        v21 = _m_psrlqi(v17, 0x20u);
        v14 = _m_psubd(v14, v18);
        if ( (result & 0xF0) != 0 )
          v20 = _m_por(v20, _m_psllqi(_mm_cvtsi32_si64(*(_DWORD *)(v42 + _mm_cvtsi64_si32(v21))), 0x20u));
        _mm_stream_pi(v8++, v20);
        --v12;
      }
      while ( v12 );
      v11 = _m_psubd(v14, _m_pslldi((__m64)-1LL, 0x10u));
      v8 = (__m64 *)((char *)v32 + (_DWORD)v45);
      v9 = (__m64 *)((char *)v34 + (_DWORD)v46);
      v10 = (__m64 *)((char *)v36 - 1);
    }
    while ( v36 != (__m64 *)1 );
  }
  _m_empty();
  return result;
}

// ===== sub_4126F0 @ 0x004126F0..0x004128CA =====
int __fastcall sub_4126F0(int *a1, int *a2, int *a3)
{
  int v3; // esi
  int v4; // edx
  int v5; // esi
  int v6; // edx
  int v7; // edi
  int v8; // edi
  int v9; // ebx
  unsigned int v10; // eax
  __m64 v11; // mm0
  unsigned int v12; // edx
  __m64 v13; // mm1
  unsigned int v14; // ecx
  unsigned int v15; // eax
  __m64 v16; // mm2
  __m64 v17; // mm3
  int v18; // ecx
  int v19; // eax
  __m64 v20; // mm3
  __m64 v21; // mm2
  int result; // eax
  int v23; // [esp-4h] [ebp-88h]
  int v24; // [esp+Ch] [ebp-78h]
  unsigned int v25; // [esp+10h] [ebp-74h]
  int v26; // [esp+14h] [ebp-70h]
  int v27; // [esp+1Ch] [ebp-68h]
  int v28; // [esp+20h] [ebp-64h]
  int v29; // [esp+24h] [ebp-60h]
  unsigned int v30; // [esp+28h] [ebp-5Ch]
  int v31; // [esp+2Ch] [ebp-58h]
  unsigned int v32; // [esp+30h] [ebp-54h]
  int v33; // [esp+34h] [ebp-50h]
  int v34; // [esp+38h] [ebp-4Ch]
  __int64 v35; // [esp+3Ch] [ebp-48h]
  int v36; // [esp+44h] [ebp-40h]
  int v37; // [esp+48h] [ebp-3Ch]
  int v38; // [esp+4Ch] [ebp-38h]
  int v39; // [esp+50h] [ebp-34h]
  int v40; // [esp+54h] [ebp-30h]
  int v41; // [esp+58h] [ebp-2Ch]
  int v42; // [esp+5Ch] [ebp-28h]
  int v43; // [esp+60h] [ebp-24h]
  int v44; // [esp+64h] [ebp-20h]
  int v45; // [esp+68h] [ebp-1Ch]
  int v46; // [esp+6Ch] [ebp-18h]
  int v47; // [esp+70h] [ebp-14h]
  int v48; // [esp+74h] [ebp-10h]
  int v49; // [esp+78h] [ebp-Ch]

  v34 = *a3;
  v27 = *a1;
  v3 = *a2;
  v28 = a2[1];
  v4 = a3[2];
  v33 = v3;
  v26 = a3[3];
  v24 = a3[1];
  v5 = a1[1];
  v25 = v5 * a1[3];
  v35 = 0x8000800080008LL;
  v36 = 458759;
  v37 = 458759;
  v38 = 393222;
  v39 = 393222;
  v40 = 327685;
  v41 = 327685;
  v42 = 262148;
  v43 = 262148;
  v29 = v4;
  v6 = a1[2];
  v44 = 196611;
  v45 = 196611;
  v46 = 131074;
  v47 = 131074;
  v30 = 4 * v6;
  v48 = 65537;
  v49 = 65537;
  v7 = v26;
  v31 = 0;
  do
  {
    v23 = v7;
    v8 = v29;
    v9 = 0;
    do
    {
      v32 = _mm_cvtsi64_si32(_m_psrawi(_mm_cvtsi32_si64(*(_DWORD *)(v33 + v9)), 1u));
      v10 = v9 + 4 * ((__int16)v32 >> 3);
      v11.m64_u64 = 0LL;
      v12 = v31 + v5 * (SHIWORD(v32) >> 3);
      v13.m64_u64 = 0LL;
      v14 = v12 + v10;
      if ( v12 < v25 )
      {
        if ( v10 < v30 )
          v11 = _mm_cvtsi32_si64(*(_DWORD *)(v27 + v14));
        v15 = v10 + 4;
        v11 = _m_punpcklbw(v11, 0LL);
        if ( v15 < v30 )
          v13 = _mm_cvtsi32_si64(*(_DWORD *)(v27 + v14 + 4));
        v10 = v15 - 4;
        v13 = _m_punpcklbw(v13, 0LL);
      }
      v16.m64_u64 = 0LL;
      v17.m64_u64 = 0LL;
      if ( (v32 & 0x70000) != 0 )
      {
        v18 = v5 + v14;
        if ( v5 + v12 < v25 )
        {
          if ( v10 < v30 )
            v16 = _mm_cvtsi32_si64(*(_DWORD *)(v27 + v18));
          v16 = _m_punpcklbw(v16, 0LL);
          if ( v10 + 4 < v30 )
            v17 = _mm_cvtsi32_si64(*(_DWORD *)(v27 + v18 + 4));
        }
      }
      v19 = v32 & 7;
      v20 = _m_punpcklbw(v17, 0LL);
      v21 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v16, v20), *((__m64 *)&v35 + v19)), 3u), v20);
      v9 += 4;
      *(_DWORD *)(v34 + v9 - 4) = _mm_cvtsi64_si32(
                                    _m_packuswb(
                                      _m_paddw(
                                        _m_psrawi(
                                          _m_pmullw(
                                            _m_psubw(
                                              _m_paddw(
                                                _m_psrawi(_m_pmullw(_m_psubw(v11, v13), *((__m64 *)&v35 + v19)), 3u),
                                                v13),
                                              v21),
                                            *((__m64 *)&v35 + (HIWORD(v32) & 7))),
                                          3u),
                                        v21),
                                      0LL));
      --v8;
    }
    while ( v8 );
    result = v24;
    v34 += v24;
    v31 += v5;
    v33 += v28;
    v7 = v23 - 1;
  }
  while ( v23 != 1 );
  _m_empty();
  return result;
}

// ===== sub_4128D0 @ 0x004128D0..0x00412AE0 =====
int __fastcall sub_4128D0(_DWORD *a1, int *a2, int *a3, int *a4)
{
  int v4; // edi
  int v5; // edx
  int v6; // edi
  int v7; // esi
  int v8; // eax
  int v9; // ecx
  int v10; // edi
  int v11; // edi
  int v12; // ebx
  int v13; // eax
  __m64 v14; // mm0
  int v15; // edx
  __m64 v16; // mm1
  int v17; // ecx
  int v18; // eax
  __m64 v19; // mm2
  __m64 v20; // mm3
  int v21; // edx
  int v22; // ecx
  int v23; // eax
  int v24; // eax
  __m64 v25; // mm3
  __m64 v26; // mm2
  int result; // eax
  int v28; // [esp-4h] [ebp-90h]
  int v29; // [esp+Ch] [ebp-80h]
  int v30; // [esp+10h] [ebp-7Ch]
  int v31; // [esp+14h] [ebp-78h]
  int v32; // [esp+18h] [ebp-74h]
  int v33; // [esp+1Ch] [ebp-70h]
  int v34; // [esp+20h] [ebp-6Ch]
  int v35; // [esp+28h] [ebp-64h]
  int v36; // [esp+2Ch] [ebp-60h]
  int v37; // [esp+30h] [ebp-5Ch]
  int v38; // [esp+34h] [ebp-58h]
  unsigned int v39; // [esp+38h] [ebp-54h]
  int v40; // [esp+3Ch] [ebp-50h]
  int v41; // [esp+40h] [ebp-4Ch]
  __int64 v42; // [esp+44h] [ebp-48h]
  int v43; // [esp+4Ch] [ebp-40h]
  int v44; // [esp+50h] [ebp-3Ch]
  int v45; // [esp+54h] [ebp-38h]
  int v46; // [esp+58h] [ebp-34h]
  int v47; // [esp+5Ch] [ebp-30h]
  int v48; // [esp+60h] [ebp-2Ch]
  int v49; // [esp+64h] [ebp-28h]
  int v50; // [esp+68h] [ebp-24h]
  int v51; // [esp+6Ch] [ebp-20h]
  int v52; // [esp+70h] [ebp-1Ch]
  int v53; // [esp+74h] [ebp-18h]
  int v54; // [esp+78h] [ebp-14h]
  int v55; // [esp+7Ch] [ebp-10h]
  int v56; // [esp+80h] [ebp-Ch]

  v41 = *a3;
  v4 = *a2;
  v5 = a2[1];
  v33 = v4;
  v40 = *a4;
  v6 = a3[1];
  v32 = a4[1];
  v7 = a3[2];
  v31 = a3[3];
  v37 = 4 * *a1;
  v35 = 4 * a1[2];
  v8 = a1[1];
  v9 = a1[3];
  v30 = v5 * v8;
  v42 = 0x8000800080008LL;
  v43 = 458759;
  v44 = 458759;
  v45 = 393222;
  v46 = 393222;
  v47 = 327685;
  v48 = 327685;
  v49 = 262148;
  v50 = 262148;
  v51 = 196611;
  v52 = 196611;
  v53 = 131074;
  v54 = 131074;
  v29 = v6;
  v36 = v5;
  v34 = v5 * v9;
  v55 = 65537;
  v56 = 65537;
  v10 = v31;
  v38 = 0;
  do
  {
    v28 = v10;
    v11 = v7;
    v12 = 0;
    do
    {
      v39 = _mm_cvtsi64_si32(_m_psrawi(_mm_cvtsi32_si64(*(_DWORD *)(v40 + v12)), 1u));
      v13 = v12 + 4 * ((__int16)v39 >> 3);
      v14.m64_u64 = 0LL;
      v15 = v38 + v36 * (SHIWORD(v39) >> 3);
      v16.m64_u64 = 0LL;
      v17 = v15 + v13;
      if ( v15 >= v30 && v15 <= v34 )
      {
        if ( v13 >= v37 && v13 <= v35 )
          v14 = _mm_cvtsi32_si64(*(_DWORD *)(v33 + v17));
        v18 = v13 + 4;
        v14 = _m_punpcklbw(v14, 0LL);
        if ( v18 >= v37 && v18 <= v35 )
          v16 = _mm_cvtsi32_si64(*(_DWORD *)(v33 + v17 + 4));
        v13 = v18 - 4;
        v16 = _m_punpcklbw(v16, 0LL);
      }
      v19.m64_u64 = 0LL;
      v20.m64_u64 = 0LL;
      if ( (v39 & 0x70000) != 0 )
      {
        v21 = v36 + v15;
        v22 = v36 + v17;
        if ( v21 >= v30 && v21 <= v34 )
        {
          if ( v13 >= v37 && v13 <= v35 )
            v19 = _mm_cvtsi32_si64(*(_DWORD *)(v33 + v22));
          v23 = v13 + 4;
          v19 = _m_punpcklbw(v19, 0LL);
          if ( v23 >= v37 && v23 <= v35 )
            v20 = _mm_cvtsi32_si64(*(_DWORD *)(v33 + v22 + 4));
        }
      }
      v24 = v39 & 7;
      v25 = _m_punpcklbw(v20, 0LL);
      v26 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v19, v25), *((__m64 *)&v42 + v24)), 3u), v25);
      v12 += 4;
      *(_DWORD *)(v41 + v12 - 4) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(
                                                 _m_psrawi(_m_pmullw(_m_psubw(v14, v16), *((__m64 *)&v42 + v24)), 3u),
                                                 v16),
                                               v26),
                                             *((__m64 *)&v42 + (HIWORD(v39) & 7))),
                                           3u),
                                         v26),
                                       0LL));
      --v11;
    }
    while ( v11 );
    result = v29;
    v41 += v29;
    v38 += v36;
    v40 += v32;
    v10 = v28 - 1;
  }
  while ( v28 != 1 );
  _m_empty();
  return result;
}

// ===== sub_412AE0 @ 0x00412AE0..0x00412CD7 =====
int __fastcall sub_412AE0(int *a1, int *a2, int *a3, int a4)
{
  int v4; // esi
  int v5; // edx
  int v6; // esi
  int v7; // edx
  int v8; // edi
  int v9; // edi
  int v10; // ebx
  unsigned int v11; // eax
  __m64 v12; // mm0
  unsigned int v13; // edx
  __m64 v14; // mm1
  unsigned int v15; // ecx
  unsigned int v16; // eax
  __m64 v17; // mm2
  __m64 v18; // mm3
  int v19; // ecx
  int v20; // eax
  __m64 v21; // mm3
  __m64 v22; // mm2
  int result; // eax
  int v24; // [esp-4h] [ebp-90h]
  int v25; // [esp+18h] [ebp-74h]
  int v26; // [esp+1Ch] [ebp-70h]
  int v27; // [esp+20h] [ebp-6Ch]
  unsigned int v28; // [esp+24h] [ebp-68h]
  int v29; // [esp+28h] [ebp-64h]
  int v30; // [esp+2Ch] [ebp-60h]
  unsigned int v31; // [esp+30h] [ebp-5Ch]
  int v32; // [esp+34h] [ebp-58h]
  unsigned int v33; // [esp+38h] [ebp-54h]
  int v34; // [esp+3Ch] [ebp-50h]
  int v35; // [esp+40h] [ebp-4Ch]
  __int64 v36; // [esp+44h] [ebp-48h]
  int v37; // [esp+4Ch] [ebp-40h]
  int v38; // [esp+50h] [ebp-3Ch]
  int v39; // [esp+54h] [ebp-38h]
  int v40; // [esp+58h] [ebp-34h]
  int v41; // [esp+5Ch] [ebp-30h]
  int v42; // [esp+60h] [ebp-2Ch]
  int v43; // [esp+64h] [ebp-28h]
  int v44; // [esp+68h] [ebp-24h]
  int v45; // [esp+6Ch] [ebp-20h]
  int v46; // [esp+70h] [ebp-1Ch]
  int v47; // [esp+74h] [ebp-18h]
  int v48; // [esp+78h] [ebp-14h]
  int v49; // [esp+7Ch] [ebp-10h]
  int v50; // [esp+80h] [ebp-Ch]

  v35 = *a3;
  v29 = *a1;
  v4 = *a2;
  v26 = a2[1];
  v5 = a3[2];
  v34 = v4;
  v27 = a3[3];
  v25 = a3[1];
  v6 = a1[1];
  v28 = v6 * a1[3];
  v36 = 0x8000800080008LL;
  v37 = 458759;
  v38 = 458759;
  v39 = 393222;
  v40 = 393222;
  v41 = 327685;
  v42 = 327685;
  v30 = v5;
  v7 = a1[2];
  v43 = 262148;
  v44 = 262148;
  v45 = 196611;
  v46 = 196611;
  v47 = 131074;
  v48 = 131074;
  v31 = 4 * v7;
  v49 = 65537;
  v50 = 65537;
  v8 = v27;
  v32 = 0;
  do
  {
    v24 = v8;
    v9 = v30;
    v10 = 0;
    do
    {
      v33 = _mm_cvtsi64_si32(_m_pmulhw(_mm_cvtsi32_si64(*(_DWORD *)(v34 + v10)), (__m64)((a4 | (unsigned int)(a4 << 16)) << 7)));
      v11 = v10 + 4 * ((__int16)v33 >> 3);
      v12.m64_u64 = 0LL;
      v13 = v32 + v6 * (SHIWORD(v33) >> 3);
      v14.m64_u64 = 0LL;
      v15 = v13 + v11;
      if ( v13 < v28 )
      {
        if ( v11 < v31 )
          v12 = _mm_cvtsi32_si64(*(_DWORD *)(v29 + v15));
        v16 = v11 + 4;
        v12 = _m_punpcklbw(v12, 0LL);
        if ( v16 < v31 )
          v14 = _mm_cvtsi32_si64(*(_DWORD *)(v29 + v15 + 4));
        v11 = v16 - 4;
        v14 = _m_punpcklbw(v14, 0LL);
      }
      v17.m64_u64 = 0LL;
      v18.m64_u64 = 0LL;
      if ( (v33 & 0x70000) != 0 )
      {
        v19 = v6 + v15;
        if ( v6 + v13 < v28 )
        {
          if ( v11 < v31 )
            v17 = _mm_cvtsi32_si64(*(_DWORD *)(v29 + v19));
          v17 = _m_punpcklbw(v17, 0LL);
          if ( v11 + 4 < v31 )
            v18 = _mm_cvtsi32_si64(*(_DWORD *)(v29 + v19 + 4));
        }
      }
      v20 = v33 & 7;
      v21 = _m_punpcklbw(v18, 0LL);
      v22 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v17, v21), *((__m64 *)&v36 + v20)), 3u), v21);
      v10 += 4;
      *(_DWORD *)(v35 + v10 - 4) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(
                                                 _m_psrawi(_m_pmullw(_m_psubw(v12, v14), *((__m64 *)&v36 + v20)), 3u),
                                                 v14),
                                               v22),
                                             *((__m64 *)&v36 + (HIWORD(v33) & 7))),
                                           3u),
                                         v22),
                                       0LL));
      --v9;
    }
    while ( v9 );
    result = v25;
    v35 += v25;
    v32 += v6;
    v34 += v26;
    v8 = v24 - 1;
  }
  while ( v24 != 1 );
  _m_empty();
  return result;
}

// ===== sub_412CE0 @ 0x00412CE0..0x00412F13 =====
int __fastcall sub_412CE0(_DWORD *a1, int *a2, int *a3, int *a4, int a5)
{
  int v5; // ebx
  int v6; // edx
  int v7; // ebx
  int v8; // esi
  int v9; // ecx
  int v10; // edi
  int v11; // edi
  int v12; // ebx
  int v13; // eax
  __m64 v14; // mm0
  int v15; // edx
  __m64 v16; // mm1
  int v17; // ecx
  int v18; // eax
  __m64 v19; // mm2
  __m64 v20; // mm3
  int v21; // edx
  int v22; // ecx
  int v23; // eax
  int v24; // eax
  __m64 v25; // mm3
  __m64 v26; // mm2
  int result; // eax
  int v28; // [esp-4h] [ebp-98h]
  int v29; // [esp+14h] [ebp-80h]
  int v30; // [esp+18h] [ebp-7Ch]
  int v31; // [esp+1Ch] [ebp-78h]
  int v32; // [esp+20h] [ebp-74h]
  int v33; // [esp+24h] [ebp-70h]
  int v34; // [esp+28h] [ebp-6Ch]
  int v35; // [esp+2Ch] [ebp-68h]
  int v36; // [esp+34h] [ebp-60h]
  int v37; // [esp+38h] [ebp-5Ch]
  int v38; // [esp+3Ch] [ebp-58h]
  unsigned int v39; // [esp+40h] [ebp-54h]
  int v40; // [esp+44h] [ebp-50h]
  int v41; // [esp+48h] [ebp-4Ch]
  __int64 v42; // [esp+4Ch] [ebp-48h]
  int v43; // [esp+54h] [ebp-40h]
  int v44; // [esp+58h] [ebp-3Ch]
  int v45; // [esp+5Ch] [ebp-38h]
  int v46; // [esp+60h] [ebp-34h]
  int v47; // [esp+64h] [ebp-30h]
  int v48; // [esp+68h] [ebp-2Ch]
  int v49; // [esp+6Ch] [ebp-28h]
  int v50; // [esp+70h] [ebp-24h]
  int v51; // [esp+74h] [ebp-20h]
  int v52; // [esp+78h] [ebp-1Ch]
  int v53; // [esp+7Ch] [ebp-18h]
  int v54; // [esp+80h] [ebp-14h]
  int v55; // [esp+84h] [ebp-10h]
  int v56; // [esp+88h] [ebp-Ch]

  v41 = *a3;
  v5 = *a2;
  v6 = a2[1];
  v34 = v5;
  v40 = *a4;
  v7 = a3[1];
  v35 = a4[1];
  v8 = a3[2];
  v32 = a3[3];
  v37 = 4 * *a1;
  v29 = 4 * a1[2];
  v33 = v6 * a1[1];
  v9 = v6 * a1[3];
  v42 = 0x8000800080008LL;
  v43 = 458759;
  v44 = 458759;
  v45 = 393222;
  v46 = 393222;
  v47 = 327685;
  v48 = 327685;
  v36 = v6;
  v49 = 262148;
  v50 = 262148;
  v51 = 196611;
  v52 = 196611;
  v53 = 131074;
  v54 = 131074;
  v30 = v7;
  v31 = v9;
  v55 = 65537;
  v56 = 65537;
  v10 = v32;
  v38 = 0;
  do
  {
    v28 = v10;
    v11 = v8;
    v12 = 0;
    do
    {
      v39 = _mm_cvtsi64_si32(_m_pmulhw(_mm_cvtsi32_si64(*(_DWORD *)(v40 + v12)), (__m64)((a5 | (unsigned int)(a5 << 16)) << 7)));
      v13 = v12 + 4 * ((__int16)v39 >> 3);
      v14.m64_u64 = 0LL;
      v15 = v38 + v36 * (SHIWORD(v39) >> 3);
      v16.m64_u64 = 0LL;
      v17 = v15 + v13;
      if ( v15 >= v33 && v15 <= v31 )
      {
        if ( v13 >= v37 && v13 <= v29 )
          v14 = _mm_cvtsi32_si64(*(_DWORD *)(v34 + v17));
        v18 = v13 + 4;
        v14 = _m_punpcklbw(v14, 0LL);
        if ( v18 >= v37 && v18 <= v29 )
          v16 = _mm_cvtsi32_si64(*(_DWORD *)(v34 + v17 + 4));
        v13 = v18 - 4;
        v16 = _m_punpcklbw(v16, 0LL);
      }
      v19.m64_u64 = 0LL;
      v20.m64_u64 = 0LL;
      if ( (v39 & 0x70000) != 0 )
      {
        v21 = v36 + v15;
        v22 = v36 + v17;
        if ( v21 >= v33 && v21 <= v31 )
        {
          if ( v13 >= v37 && v13 <= v29 )
            v19 = _mm_cvtsi32_si64(*(_DWORD *)(v34 + v22));
          v23 = v13 + 4;
          v19 = _m_punpcklbw(v19, 0LL);
          if ( v23 >= v37 && v23 <= v29 )
            v20 = _mm_cvtsi32_si64(*(_DWORD *)(v34 + v22 + 4));
        }
      }
      v24 = v39 & 7;
      v25 = _m_punpcklbw(v20, 0LL);
      v26 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v19, v25), *((__m64 *)&v42 + v24)), 3u), v25);
      v12 += 4;
      *(_DWORD *)(v41 + v12 - 4) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(
                                                 _m_psrawi(_m_pmullw(_m_psubw(v14, v16), *((__m64 *)&v42 + v24)), 3u),
                                                 v16),
                                               v26),
                                             *((__m64 *)&v42 + (HIWORD(v39) & 7))),
                                           3u),
                                         v26),
                                       0LL));
      --v11;
    }
    while ( v11 );
    result = v30;
    v41 += v30;
    v38 += v36;
    v40 += v35;
    v10 = v28 - 1;
  }
  while ( v28 != 1 );
  _m_empty();
  return result;
}

// ===== sub_412F20 @ 0x00412F20..0x00413039 =====
int __usercall sub_412F20@<eax>(int *a1@<eax>, int *a2@<ecx>, int *a3, int *a4, unsigned int a5)
{
  __int64 v7; // mm6
  int v8; // edi
  int v9; // esi
  int v10; // edi
  int v11; // ebx
  __m64 v12; // mm0
  int v13; // eax
  unsigned int v14; // edx
  int v15; // edi
  unsigned int v16; // eax
  int result; // eax
  int v18; // [esp-8h] [ebp-54h]
  int v19; // [esp-4h] [ebp-50h]
  int v20; // [esp+2Ch] [ebp-20h]
  int v21; // [esp+30h] [ebp-1Ch]
  unsigned int v22; // [esp+34h] [ebp-18h]
  unsigned int v23; // [esp+38h] [ebp-14h]
  int v24; // [esp+3Ch] [ebp-10h]
  int v25; // [esp+40h] [ebp-Ch]
  int v26; // [esp+44h] [ebp-8h]
  int v27; // [esp+48h] [ebp-4h]
  int v28; // [esp+54h] [ebp+8h]
  int v29; // [esp+58h] [ebp+Ch]
  int v30; // [esp+5Ch] [ebp+10h]

  v28 = *a2;
  v29 = *a3;
  v27 = *a4;
  v26 = a4[1];
  v24 = a2[1];
  v23 = a1[1] * a1[3];
  v22 = 4 * a1[2];
  v21 = a1[1];
  v25 = a3[1];
  v20 = a2[2];
  v7 = 4194368LL * a5;
  v8 = a2[3];
  v9 = *a1;
  v30 = 0;
  do
  {
    v19 = v8;
    v10 = v20;
    v11 = 0;
    do
    {
      v18 = v10;
      v12 = _m_pand(_mm_cvtsi32_si64(*(_DWORD *)(v29 + v11)), (__m64)4294770684LL);
      v13 = _mm_cvtsi64_si32(
              _m_psrawi(
                _m_paddw(
                  _m_paddw(
                    _m_pmulhw(_m_psubw(_mm_cvtsi32_si64(*(_DWORD *)(v27 + v11)), v12), (__m64)v7),
                    _m_psrawi(v12, 2u)),
                  (__m64)131074LL),
                2u));
      v14 = v11 + 4 * (__int16)v13;
      v15 = 0;
      v16 = v30 + v21 * (v13 >> 16);
      if ( v14 < v22 && v16 < v23 )
        v15 = *(_DWORD *)(v9 + v14 + v16);
      *(_DWORD *)(v28 + v11) = v15;
      v11 += 4;
      v10 = v18 - 1;
    }
    while ( v18 != 1 );
    result = v24;
    v28 += v24;
    v30 += v21;
    v29 += v25;
    v27 += v26;
    v8 = v19 - 1;
  }
  while ( v19 != 1 );
  _m_empty();
  return result;
}

// ===== sub_413040 @ 0x00413040..0x0041327D =====
int __fastcall sub_413040(int *a1, int *a2, int *a3, int *a4, int a5)
{
  int v5; // edx
  int v6; // edi
  int v7; // edx
  int v8; // edi
  int v9; // edi
  int v10; // ebx
  __m64 v11; // mm4
  unsigned int v12; // eax
  __m64 v13; // mm0
  unsigned int v14; // edx
  __m64 v15; // mm1
  unsigned int v16; // ecx
  unsigned int v17; // eax
  __m64 v18; // mm2
  __m64 v19; // mm3
  int v20; // ecx
  int v21; // eax
  __m64 v22; // mm3
  __m64 v23; // mm2
  int result; // eax
  int v25; // [esp-4h] [ebp-A0h]
  unsigned int v26; // [esp+1Ch] [ebp-80h]
  int v27; // [esp+20h] [ebp-7Ch]
  int v28; // [esp+24h] [ebp-78h]
  int v29; // [esp+28h] [ebp-74h]
  int v30; // [esp+2Ch] [ebp-70h]
  int v31; // [esp+30h] [ebp-6Ch]
  int v32; // [esp+34h] [ebp-68h]
  int v33; // [esp+38h] [ebp-64h]
  unsigned int v34; // [esp+3Ch] [ebp-60h]
  int v35; // [esp+40h] [ebp-5Ch]
  unsigned int v36; // [esp+44h] [ebp-58h]
  int v37; // [esp+48h] [ebp-54h]
  int v38; // [esp+4Ch] [ebp-50h]
  int v39; // [esp+50h] [ebp-4Ch]
  __int64 v40; // [esp+54h] [ebp-48h]
  int v41; // [esp+5Ch] [ebp-40h]
  int v42; // [esp+60h] [ebp-3Ch]
  int v43; // [esp+64h] [ebp-38h]
  int v44; // [esp+68h] [ebp-34h]
  int v45; // [esp+6Ch] [ebp-30h]
  int v46; // [esp+70h] [ebp-2Ch]
  int v47; // [esp+74h] [ebp-28h]
  int v48; // [esp+78h] [ebp-24h]
  int v49; // [esp+7Ch] [ebp-20h]
  int v50; // [esp+80h] [ebp-1Ch]
  int v51; // [esp+84h] [ebp-18h]
  int v52; // [esp+88h] [ebp-14h]
  int v53; // [esp+8Ch] [ebp-10h]
  int v54; // [esp+90h] [ebp-Ch]

  v37 = *a3;
  v32 = *a1;
  v27 = a2[1];
  v38 = *a2;
  v31 = a4[1];
  v5 = a3[2];
  v39 = *a4;
  v30 = a3[3];
  v28 = a3[1];
  v6 = a1[1];
  v26 = v6 * a1[3];
  v40 = 0x8000800080008LL;
  v41 = 458759;
  v42 = 458759;
  v43 = 393222;
  v44 = 393222;
  v45 = 327685;
  v46 = 327685;
  v47 = 262148;
  v48 = 262148;
  v33 = v5;
  v7 = a1[2];
  v49 = 196611;
  v50 = 196611;
  v51 = 131074;
  v52 = 131074;
  v53 = 65537;
  v54 = 65537;
  v29 = v6;
  v34 = 4 * v7;
  v8 = v30;
  v35 = 0;
  do
  {
    v25 = v8;
    v9 = v33;
    v10 = 0;
    do
    {
      v11 = _m_pand(_mm_cvtsi32_si64(*(_DWORD *)(v38 + v10)), (__m64)4294901758LL);
      v36 = _mm_cvtsi64_si32(
              _m_paddw(
                _m_pmulhw(
                  _m_psubw(_mm_cvtsi32_si64(*(_DWORD *)(v39 + v10)), v11),
                  (__m64)((a5 | (unsigned int)(a5 << 16)) << 7)),
                _m_psrawi(v11, 1u)));
      v12 = v10 + 4 * ((__int16)v36 >> 3);
      v13.m64_u64 = 0LL;
      v14 = v35 + v29 * (SHIWORD(v36) >> 3);
      v15.m64_u64 = 0LL;
      v16 = v14 + v12;
      if ( v14 < v26 )
      {
        if ( v12 < v34 )
          v13 = _mm_cvtsi32_si64(*(_DWORD *)(v32 + v16));
        v17 = v12 + 4;
        v13 = _m_punpcklbw(v13, 0LL);
        if ( v17 < v34 )
          v15 = _mm_cvtsi32_si64(*(_DWORD *)(v32 + v16 + 4));
        v12 = v17 - 4;
        v15 = _m_punpcklbw(v15, 0LL);
      }
      v18.m64_u64 = 0LL;
      v19.m64_u64 = 0LL;
      if ( (v36 & 0x70000) != 0 )
      {
        v20 = v29 + v16;
        if ( v29 + v14 < v26 )
        {
          if ( v12 < v34 )
            v18 = _mm_cvtsi32_si64(*(_DWORD *)(v32 + v20));
          v18 = _m_punpcklbw(v18, 0LL);
          if ( v12 + 4 < v34 )
            v19 = _mm_cvtsi32_si64(*(_DWORD *)(v32 + v20 + 4));
        }
      }
      v21 = v36 & 7;
      v22 = _m_punpcklbw(v19, 0LL);
      v23 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v18, v22), *((__m64 *)&v40 + v21)), 3u), v22);
      v10 += 4;
      *(_DWORD *)(v37 + v10 - 4) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(
                                                 _m_psrawi(_m_pmullw(_m_psubw(v13, v15), *((__m64 *)&v40 + v21)), 3u),
                                                 v15),
                                               v23),
                                             *((__m64 *)&v40 + (HIWORD(v36) & 7))),
                                           3u),
                                         v23),
                                       0LL));
      --v9;
    }
    while ( v9 );
    result = v28;
    v37 += v28;
    v35 += v29;
    v38 += v27;
    v39 += v31;
    v8 = v25 - 1;
  }
  while ( v25 != 1 );
  _m_empty();
  return result;
}

// ===== sub_413280 @ 0x00413280..0x004134FF =====
int __fastcall sub_413280(_DWORD *a1, int *a2, int *a3, int *a4, int *a5, int a6)
{
  int v6; // ebx
  int v7; // edx
  int v8; // ebx
  int v9; // esi
  int v10; // ecx
  int v11; // edi
  int v12; // edi
  int v13; // ebx
  __m64 v14; // mm4
  int v15; // eax
  __m64 v16; // mm0
  int v17; // edx
  __m64 v18; // mm1
  int v19; // ecx
  int v20; // eax
  __m64 v21; // mm2
  __m64 v22; // mm3
  int v23; // edx
  int v24; // ecx
  int v25; // eax
  int v26; // eax
  __m64 v27; // mm3
  __m64 v28; // mm2
  int result; // eax
  int v30; // [esp-4h] [ebp-A8h]
  int v31; // [esp+1Ch] [ebp-88h]
  int v32; // [esp+20h] [ebp-84h]
  int v33; // [esp+24h] [ebp-80h]
  int v34; // [esp+28h] [ebp-7Ch]
  int v35; // [esp+2Ch] [ebp-78h]
  int v36; // [esp+30h] [ebp-74h]
  int v37; // [esp+34h] [ebp-70h]
  int v38; // [esp+38h] [ebp-6Ch]
  int v39; // [esp+40h] [ebp-64h]
  int v40; // [esp+44h] [ebp-60h]
  int v41; // [esp+48h] [ebp-5Ch]
  unsigned int v42; // [esp+4Ch] [ebp-58h]
  int v43; // [esp+50h] [ebp-54h]
  int v44; // [esp+54h] [ebp-50h]
  int v45; // [esp+58h] [ebp-4Ch]
  __int64 v46; // [esp+5Ch] [ebp-48h]
  int v47; // [esp+64h] [ebp-40h]
  int v48; // [esp+68h] [ebp-3Ch]
  int v49; // [esp+6Ch] [ebp-38h]
  int v50; // [esp+70h] [ebp-34h]
  int v51; // [esp+74h] [ebp-30h]
  int v52; // [esp+78h] [ebp-2Ch]
  int v53; // [esp+7Ch] [ebp-28h]
  int v54; // [esp+80h] [ebp-24h]
  int v55; // [esp+84h] [ebp-20h]
  int v56; // [esp+88h] [ebp-1Ch]
  int v57; // [esp+8Ch] [ebp-18h]
  int v58; // [esp+90h] [ebp-14h]
  int v59; // [esp+94h] [ebp-10h]
  int v60; // [esp+98h] [ebp-Ch]

  v43 = *a3;
  v6 = *a2;
  v7 = a2[1];
  v37 = v6;
  v44 = *a4;
  v32 = a4[1];
  v45 = *a5;
  v8 = a3[1];
  v33 = a5[1];
  v9 = a3[2];
  v35 = a3[3];
  v40 = 4 * *a1;
  v38 = 4 * a1[2];
  v31 = v7 * a1[1];
  v10 = v7 * a1[3];
  v46 = 0x8000800080008LL;
  v47 = 458759;
  v48 = 458759;
  v49 = 393222;
  v50 = 393222;
  v51 = 327685;
  v52 = 327685;
  v53 = 262148;
  v54 = 262148;
  v55 = 196611;
  v56 = 196611;
  v57 = 131074;
  v58 = 131074;
  v39 = v7;
  v59 = 65537;
  v60 = 65537;
  v34 = v8;
  v36 = v10;
  v11 = v35;
  v41 = 0;
  do
  {
    v30 = v11;
    v12 = v9;
    v13 = 0;
    do
    {
      v14 = _m_pand(_mm_cvtsi32_si64(*(_DWORD *)(v44 + v13)), (__m64)4294901758LL);
      v42 = _mm_cvtsi64_si32(
              _m_paddw(
                _m_pmulhw(
                  _m_psubw(_mm_cvtsi32_si64(*(_DWORD *)(v45 + v13)), v14),
                  (__m64)((a6 | (unsigned int)(a6 << 16)) << 7)),
                _m_psrawi(v14, 1u)));
      v15 = v13 + 4 * ((__int16)v42 >> 3);
      v16.m64_u64 = 0LL;
      v17 = v41 + v39 * (SHIWORD(v42) >> 3);
      v18.m64_u64 = 0LL;
      v19 = v17 + v15;
      if ( v17 >= v31 && v17 <= v36 )
      {
        if ( v15 >= v40 && v15 <= v38 )
          v16 = _mm_cvtsi32_si64(*(_DWORD *)(v37 + v19));
        v20 = v15 + 4;
        v16 = _m_punpcklbw(v16, 0LL);
        if ( v20 >= v40 && v20 <= v38 )
          v18 = _mm_cvtsi32_si64(*(_DWORD *)(v37 + v19 + 4));
        v15 = v20 - 4;
        v18 = _m_punpcklbw(v18, 0LL);
      }
      v21.m64_u64 = 0LL;
      v22.m64_u64 = 0LL;
      if ( (v42 & 0x70000) != 0 )
      {
        v23 = v39 + v17;
        v24 = v39 + v19;
        if ( v23 >= v31 && v23 <= v36 )
        {
          if ( v15 >= v40 && v15 <= v38 )
            v21 = _mm_cvtsi32_si64(*(_DWORD *)(v37 + v24));
          v25 = v15 + 4;
          v21 = _m_punpcklbw(v21, 0LL);
          if ( v25 >= v40 && v25 <= v38 )
            v22 = _mm_cvtsi32_si64(*(_DWORD *)(v37 + v24 + 4));
        }
      }
      v26 = v42 & 7;
      v27 = _m_punpcklbw(v22, 0LL);
      v28 = _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v21, v27), *((__m64 *)&v46 + v26)), 3u), v27);
      v13 += 4;
      *(_DWORD *)(v43 + v13 - 4) = _mm_cvtsi64_si32(
                                     _m_packuswb(
                                       _m_paddw(
                                         _m_psrawi(
                                           _m_pmullw(
                                             _m_psubw(
                                               _m_paddw(
                                                 _m_psrawi(_m_pmullw(_m_psubw(v16, v18), *((__m64 *)&v46 + v26)), 3u),
                                                 v18),
                                               v28),
                                             *((__m64 *)&v46 + (HIWORD(v42) & 7))),
                                           3u),
                                         v28),
                                       0LL));
      --v12;
    }
    while ( v12 );
    result = v34;
    v43 += v34;
    v41 += v39;
    v44 += v32;
    v45 += v33;
    v11 = v30 - 1;
  }
  while ( v30 != 1 );
  _m_empty();
  return result;
}

// ===== sub_413500 @ 0x00413500..0x00413537 =====
int __usercall sub_413500@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>, int a4)
{
  if ( !a4 )
    return sub_413540(a3, a1, a2, 0, 1);
  if ( a4 == 1 )
    return sub_413540(a3, a1, a2, 1, 1);
  return 14;
}

// ===== sub_413540 @ 0x00413540..0x00413607 =====
int __cdecl sub_413540(_DWORD *a1, _DWORD *a2, unsigned int a3, int a4, int a5)
{
  unsigned int v5; // eax
  _DWORD v7[2]; // [esp+10h] [ebp-10h] BYREF
  _DWORD v8[2]; // [esp+18h] [ebp-8h] BYREF

  v5 = a3;
  if ( a3 > 0x100 )
    return 3;
  if ( a1[4] == a2[4] && a1[2] == a2[2] && a1[3] == a2[3] )
  {
    if ( a5 )
    {
      v7[0] = a3;
      v8[0] = a1;
      v8[1] = a2;
      v7[1] = a4;
      if ( sub_419DE0(sub_41A170, v8, 2, 0, 0, v7) )
        return 0;
      v5 = a3;
    }
    if ( (unsigned int)(a2[4] - 1) <= 1 )
    {
      if ( a4 )
      {
        sub_413780(v5);
        return 0;
      }
      sub_413610(v5);
    }
    return 0;
  }
  return 15;
}

// ===== sub_413610 @ 0x00413610..0x00413778 =====
void __usercall sub_413610(int *a1@<eax>, size_t *a2@<ecx>, unsigned int a3)
{
  int v3; // edx
  int v4; // esi
  unsigned int v5; // ecx
  int v6; // eax
  char *v7; // edi
  int v8; // esi
  int v9; // ecx
  __m64 v10; // mm4
  int v11; // eax
  unsigned int v12; // ecx
  __m64 v13; // mm0
  int v14; // ecx
  int v15; // ebx
  int v16; // [esp-4h] [ebp-48h]
  __m64 v17; // [esp+Ch] [ebp-38h]
  __int64 v18; // [esp+14h] [ebp-30h]
  size_t v19; // [esp+20h] [ebp-24h]
  int v20; // [esp+24h] [ebp-20h]
  int v21; // [esp+28h] [ebp-1Ch]
  int v22; // [esp+2Ch] [ebp-18h]
  int v23; // [esp+30h] [ebp-14h]
  unsigned int v24; // [esp+34h] [ebp-10h]
  size_t v25; // [esp+38h] [ebp-Ch]
  int v26; // [esp+3Ch] [ebp-8h]
  _DWORD *v27; // [esp+40h] [ebp-4h]
  unsigned int v28; // [esp+4Ch] [ebp+8h]

  v3 = a3;
  if ( a3 )
  {
    v19 = *a2;
    v20 = *a1;
    v4 = a1[2];
    v25 = a2[1];
    v26 = a1[1];
    v23 = v4;
    v21 = a1[3];
    if ( a3 >= 0x100 )
      v3 = 255;
    v5 = 2 * v3 + 1;
    v28 = v5;
    v22 = -((int)v5 >> 1);
    v24 = (unsigned int)(2 * v3) >> 1;
    if ( v5 >= 0x40 )
      v6 = 2 - (v5 < 0x80);
    else
      v6 = 0;
    v17.m64_u64 = v6;
    v18 = 0x1000100010001LL * (0x10003 / v5);
    v27 = operator new[](4 * v4);
    v7 = (char *)v19;
    v8 = v20;
    v9 = v21;
    do
    {
      v16 = v9;
      v10.m64_u64 = 0LL;
      v11 = v22;
      v12 = v28;
      do
      {
        v13.m64_u64 = 0LL;
        if ( v11 >= 0 && v11 < v23 )
          v13 = _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * v11)), 0LL), v17);
        v10 = _m_paddw(v10, v13);
        ++v11;
        --v12;
      }
      while ( v12 );
      v14 = v23;
      v15 = 0;
      do
      {
        v27[v15] = _mm_cvtsi64_si32(_m_packuswb(_m_psllw(_m_pmulhw(v10, (__m64)v18), v17), 0LL));
        if ( v15 + v22 >= 0 )
          v10 = _m_psubw(v10, _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * (v15 + v22))), 0LL), v17));
        if ( (int)(++v15 + v24) < v23 )
          v10 = _m_paddw(v10, _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * (v15 + v24))), 0LL), v17));
        --v14;
      }
      while ( v14 );
      qmemcpy(v7, v27, 4 * v23);
      v7 += v25;
      v8 += v26;
      v9 = v16 - 1;
    }
    while ( v16 != 1 );
    _m_empty();
    operator delete[](v27);
  }
  else
  {
    sub_40AF50((size_t)a1, a2);
  }
}

// ===== sub_413780 @ 0x00413780..0x004138FC =====
void __usercall sub_413780(int *a1@<eax>, size_t *a2@<ecx>, unsigned int a3)
{
  int v3; // edx
  int v4; // esi
  unsigned int v5; // ecx
  int v6; // eax
  char *v7; // edi
  int v8; // esi
  int v9; // ecx
  __m64 v10; // mm4
  int v11; // eax
  unsigned int v12; // ecx
  int v13; // ebx
  int v14; // ecx
  int v15; // ebx
  int v16; // edx
  __m64 v17; // mm4
  int v18; // edx
  int v19; // [esp-4h] [ebp-48h]
  __m64 v20; // [esp+Ch] [ebp-38h]
  __int64 v21; // [esp+14h] [ebp-30h]
  size_t v22; // [esp+1Ch] [ebp-28h]
  int v23; // [esp+20h] [ebp-24h]
  int v24; // [esp+24h] [ebp-20h]
  int v25; // [esp+28h] [ebp-1Ch]
  int v26; // [esp+2Ch] [ebp-18h]
  int v27; // [esp+30h] [ebp-14h]
  unsigned int v28; // [esp+34h] [ebp-10h]
  size_t v29; // [esp+38h] [ebp-Ch]
  int v30; // [esp+3Ch] [ebp-8h]
  _DWORD *v31; // [esp+40h] [ebp-4h]
  unsigned int v32; // [esp+4Ch] [ebp+8h]

  v3 = a3;
  if ( a3 )
  {
    v22 = *a2;
    v23 = *a1;
    v4 = a1[2];
    v29 = a2[1];
    v30 = a1[1];
    v27 = v4;
    v24 = a1[3];
    if ( a3 >= 0x100 )
      v3 = 255;
    v5 = 2 * v3 + 1;
    v25 = -((int)v5 >> 1);
    v32 = v5;
    v28 = (unsigned int)(2 * v3) >> 1;
    v26 = v4 - 1;
    if ( v5 >= 0x40 )
      v6 = 2 - (v5 < 0x80);
    else
      v6 = 0;
    v20.m64_u64 = v6;
    v21 = 0x1000100010001LL * (0x10003 / v5);
    v31 = operator new[](4 * v4);
    v7 = (char *)v22;
    v8 = v23;
    v9 = v24;
    do
    {
      v19 = v9;
      v10.m64_u64 = 0LL;
      v11 = v25;
      v12 = v32;
      do
      {
        v13 = 0;
        if ( v11 >= 0 )
        {
          v13 = v26;
          if ( v11 < v27 )
            v13 = v11;
        }
        ++v11;
        v10 = _m_paddw(v10, _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * v13)), 0LL), v20));
        --v12;
      }
      while ( v12 );
      v14 = v27;
      v15 = 0;
      do
      {
        v31[v15] = _mm_cvtsi64_si32(_m_packuswb(_m_psllw(_m_pmulhw(v10, (__m64)v21), v20), 0LL));
        v16 = 0;
        if ( v15 + v25 >= 0 )
          v16 = v15 + v25;
        v17 = _m_psubw(v10, _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * v16)), 0LL), v20));
        ++v15;
        v18 = v26;
        if ( (int)(v15 + v28) < v27 )
          v18 = v15 + v28;
        v10 = _m_paddw(v17, _m_psrlw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v8 + 4 * v18)), 0LL), v20));
        --v14;
      }
      while ( v14 );
      qmemcpy(v7, v31, 4 * v27);
      v7 += v29;
      v8 += v30;
      v9 = v19 - 1;
    }
    while ( v19 != 1 );
    _m_empty();
    operator delete[](v31);
  }
  else
  {
    sub_40AF50((size_t)a1, a2);
  }
}

// ===== sub_413900 @ 0x00413900..0x0041392B =====
size_t __usercall sub_413900@<eax>(size_t result@<eax>, unsigned int a2@<edx>, int a3)
{
  if ( a2 >= 4 )
    return sub_40B080(result, a3);
  if ( (unsigned int)(*(_DWORD *)(result + 16) - 1) <= 1 )
    return sub_413930(a3, a2);
  return result;
}

// ===== sub_413930 @ 0x00413930..0x004139FA =====
_DWORD *__usercall sub_413930@<eax>(int *a1@<esi>, _DWORD *a2, int a3)
{
  _DWORD *result; // eax
  unsigned int *v4; // ecx
  int v5; // edx
  int v6; // ebx
  unsigned int v7; // edi
  unsigned int *v8; // eax
  int v9; // edx
  unsigned int v10; // ebx
  _DWORD *v11; // [esp+4h] [ebp-Ch]
  int v12; // [esp+8h] [ebp-8h]
  unsigned int *v13; // [esp+Ch] [ebp-4h]

  result = a2;
  v4 = (unsigned int *)*a2;
  v5 = *a1;
  v6 = a3;
  v13 = (unsigned int *)*a2;
  v12 = *a1;
  v11 = 0;
  if ( a1[3] )
  {
    do
    {
      v7 = 0;
      if ( a1[2] )
      {
        v8 = v4;
        v9 = v5 - (_DWORD)v4;
        do
        {
          switch ( v6 )
          {
            case 0:
              v10 = *((unsigned __int8 *)v8 + v9) | 0xFF000000;
              goto LABEL_9;
            case 1:
              v10 = *(unsigned int *)((char *)v8 + v9) & 0xFF00 | 0xFF000000;
              goto LABEL_9;
            case 2:
              v10 = *(unsigned int *)((char *)v8 + v9) & 0xFF0000 | 0xFF000000;
              goto LABEL_9;
            case 3:
              v10 = *((unsigned __int8 *)v8 + v9 + 3) | ((*((unsigned __int8 *)v8 + v9 + 3) | ((*((unsigned __int8 *)v8
                                                                                                + v9
                                                                                                + 3) | 0xFFFFFF00) << 8)) << 8);
              v4 = v13;
LABEL_9:
              *v8 = v10;
              break;
            default:
              break;
          }
          v6 = a3;
          ++v7;
          ++v8;
        }
        while ( v7 < a1[2] );
        v5 = v12;
      }
      v4 = (unsigned int *)((char *)v4 + a2[1]);
      v5 += a1[1];
      result = (_DWORD *)((char *)v11 + 1);
      v13 = v4;
      v12 = v5;
      v11 = result;
    }
    while ( (unsigned int)result < a1[3] );
  }
  return result;
}

// ===== sub_413A10 @ 0x00413A10..0x00413A69 =====
int __usercall sub_413A10@<eax>(_DWORD *a1@<eax>, int a2@<edx>, _DWORD *a3, int a4, int a5, int a6)
{
  int v6; // esi

  if ( a1[4] != 6 || a3[2] != a1[2] || a3[3] != a1[3] )
    return 12;
  v6 = *(_DWORD *)(a2 + 16);
  if ( a3[4] != v6 )
    return 1;
  if ( (unsigned int)(v6 - 1) <= 1 )
    sub_413A70(a3, a1, a5, a6);
  return 0;
}

// ===== sub_413A70 @ 0x00413A70..0x00413F01 =====
int __fastcall sub_413A70(unsigned int *a1, int *a2, __m64 **a3, int *a4, int a5, int a6)
{
  __m64 *v6; // ebx
  int v7; // edi
  unsigned int v8; // eax
  int v9; // edx
  int v10; // ecx
  int v11; // esi
  __int64 v12; // rax
  __m64 *v13; // ecx
  __m64 *v14; // edi
  int v15; // esi
  int v16; // ebx
  __m64 *v17; // ecx
  __m64 v18; // mm1
  __m64 v19; // mm4
  unsigned int *v20; // eax
  __m64 v21; // mm4
  __m64 v22; // mm0
  __m64 v23; // mm1
  __m64 v24; // mm2
  __m64 v25; // mm3
  unsigned int *v26; // eax
  unsigned int *v27; // eax
  unsigned int *v28; // eax
  unsigned int v29; // eax
  __int64 v30; // rax
  __m64 v31; // mm4
  __m64 v32; // mm2
  __m64 *v33; // ecx
  __m64 *v34; // edi
  int v35; // esi
  int v36; // ebx
  unsigned int v37; // ecx
  __m64 v38; // mm4
  __m64 v39; // mm4
  __m64 *v40; // ecx
  __m64 *v41; // edi
  int v42; // esi
  int v43; // ebx
  unsigned int v44; // ecx
  __m64 v45; // mm4
  __m64 v46; // mm4
  __m64 *v47; // ecx
  __m64 *v48; // edi
  int v49; // esi
  int v50; // ebx
  __m64 *v51; // ecx
  int v53; // [esp-10h] [ebp-ECh]
  int v54; // [esp-10h] [ebp-ECh]
  int v55; // [esp-10h] [ebp-ECh]
  int v56; // [esp-10h] [ebp-ECh]
  int v57; // [esp-Ch] [ebp-E8h]
  int v58; // [esp-Ch] [ebp-E8h]
  int v59; // [esp-Ch] [ebp-E8h]
  int v60; // [esp-Ch] [ebp-E8h]
  __m64 *v61; // [esp-8h] [ebp-E4h]
  __m64 *v62; // [esp-8h] [ebp-E4h]
  __m64 *v63; // [esp-8h] [ebp-E4h]
  __m64 *v64; // [esp-8h] [ebp-E4h]
  __m64 *v65; // [esp-4h] [ebp-E0h]
  __m64 *v66; // [esp-4h] [ebp-E0h]
  __m64 *v67; // [esp-4h] [ebp-E0h]
  __m64 *v68; // [esp-4h] [ebp-E0h]
  int v69; // [esp+18h] [ebp-C4h]
  __m64 v70; // [esp+1Ch] [ebp-C0h]
  unsigned int v71; // [esp+28h] [ebp-B4h]
  __m64 *v72; // [esp+2Ch] [ebp-B0h]
  int v73; // [esp+30h] [ebp-ACh]
  int v74; // [esp+34h] [ebp-A8h]
  __m64 *v75; // [esp+3Ch] [ebp-A0h]
  unsigned int v76; // [esp+40h] [ebp-9Ch]
  __m64 *v77; // [esp+44h] [ebp-98h]
  int v78; // [esp+48h] [ebp-94h]
  int v79; // [esp+4Ch] [ebp-90h]
  __m64 *v80; // [esp+50h] [ebp-8Ch]
  unsigned int v81; // [esp+50h] [ebp-8Ch]
  _DWORD v82[33]; // [esp+54h] [ebp-88h]

  v6 = a3[2];
  v77 = *a3;
  v78 = *a2;
  v79 = *a4;
  v75 = a3[3];
  v72 = a3[1];
  v7 = a2[1];
  v74 = a4[1];
  v70.m64_u64 = (v7 << 16) + 4;
  v8 = *a1;
  v9 = *a1 + a1[1] * a1[3];
  v10 = 0;
  v73 = v7;
  v80 = v6;
  v71 = v8;
  v76 = v9;
  if ( a6 )
  {
    v69 = v7 - 4;
    v11 = 16;
    do
    {
      v12 = v11 << 8;
      v82[2 * v10] = v12 | ((v11 | ((v11 | (v11 << 16)) << 16)) << 24);
      v82[2 * v10++ + 1] = HIDWORD(v12) | ((v11 | ((v11 | (unsigned __int64)((__int64)v11 << 16)) << 16)) >> 8);
      --v11;
    }
    while ( v10 < 16 );
    v13 = v75;
    v14 = v77;
    v15 = v78;
    v16 = v79;
    do
    {
      v65 = v13;
      v61 = v14;
      v57 = v15;
      v53 = v16;
      v17 = v80;
      do
      {
        v18 = _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v16 + 4)));
        v19 = _mm_cvtsi32_si64(*(_DWORD *)v16);
        v20 = (unsigned int *)(v15 + _mm_cvtsi64_si32(_m_pmaddwd(_m_pmulhw(v19, v18), v70)));
        v21 = _m_psrlwi(_m_pand(_m_pmullw(v19, v18), (__m64)4026593280LL), 9u);
        v22.m64_u64 = 0LL;
        v23.m64_u64 = 0LL;
        v24.m64_u64 = 0LL;
        v25.m64_u64 = 0LL;
        if ( (unsigned int)v20 < v76 )
        {
          if ( (unsigned int)v20 >= v71 )
            v22 = _m_punpcklbw(_mm_cvtsi32_si64(*v20), 0LL);
          v26 = v20 + 1;
          if ( (unsigned int)v26 < v76 )
          {
            if ( (unsigned int)v26 >= v71 )
              v23 = _m_punpcklbw(_mm_cvtsi32_si64(*v26), 0LL);
            v27 = (unsigned int *)((char *)v26 + v69);
            if ( (unsigned int)v27 < v76 )
            {
              if ( (unsigned int)v27 >= v71 )
                v24 = _m_punpcklbw(_mm_cvtsi32_si64(*v27), 0LL);
              v28 = v27 + 1;
              if ( (unsigned int)v28 < v76 && (unsigned int)v28 >= v71 )
                v25 = _m_punpcklbw(_mm_cvtsi32_si64(*v28), 0LL);
            }
          }
        }
        v29 = _mm_cvtsi64_si32(v21);
        HIDWORD(v30) = (unsigned __int16)v29;
        LODWORD(v30) = HIWORD(v29);
        v31 = *(__m64 *)((char *)v82 + HIDWORD(v30));
        v32 = _m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v24, v25), 4u), v31), v25);
        v14->m64_i32[0] = _mm_cvtsi64_si32(
                            _m_packuswb(
                              _m_paddw(
                                _m_pmulhw(
                                  _m_psllwi(
                                    _m_psubw(_m_paddw(_m_pmulhw(_m_psllwi(_m_psubw(v22, v23), 4u), v31), v23), v32),
                                    4u),
                                  *(__m64 *)((char *)v82 + v30)),
                                v32),
                              0LL));
        v16 += 6;
        v15 += 4;
        v14 = (__m64 *)((char *)v14 + 4);
        v17 = (__m64 *)((char *)v17 - 1);
      }
      while ( v17 );
      v14 = (__m64 *)((char *)v61 + (_DWORD)v72);
      v15 = v73 + v57;
      v16 = v74 + v53;
      v13 = (__m64 *)((char *)v65 - 1);
    }
    while ( v65 != (__m64 *)1 );
  }
  else if ( ((unsigned __int8)v6 & 1) != 0 )
  {
    v47 = a3[3];
    v48 = *a3;
    v49 = v78;
    v50 = *a4;
    do
    {
      v68 = v47;
      v64 = v48;
      v60 = v49;
      v56 = v50;
      v51 = v80;
      do
      {
        v30 = (unsigned int)(v49
                           + _mm_cvtsi64_si32(
                               _m_pmaddwd(
                                 _m_pmulhw(
                                   _mm_cvtsi32_si64(*(_DWORD *)v50),
                                   _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v50 + 4)))),
                                 v70)));
        if ( (unsigned int)v30 >= v71 && (unsigned int)v30 < v76 )
          HIDWORD(v30) = *(_DWORD *)v30;
        v48->m64_i32[0] = HIDWORD(v30);
        v50 += 6;
        v49 += 4;
        v48 = (__m64 *)((char *)v48 + 4);
        v51 = (__m64 *)((char *)v51 - 1);
      }
      while ( v51 );
      v48 = (__m64 *)((char *)v64 + (_DWORD)v72);
      v49 = v73 + v60;
      v50 = v74 + v56;
      v47 = (__m64 *)((char *)v68 - 1);
    }
    while ( v68 != (__m64 *)1 );
  }
  else
  {
    v81 = (unsigned int)v6 >> 1;
    if ( dword_565AFC )
    {
      v33 = a3[3];
      v34 = *a3;
      v35 = v78;
      v36 = *a4;
      do
      {
        v66 = v33;
        v62 = v34;
        v58 = v35;
        v54 = v36;
        v37 = v81;
        do
        {
          LODWORD(v30) = v35
                       + _mm_cvtsi64_si32(
                           _m_pmaddwd(
                             _m_pmulhw(
                               _mm_cvtsi32_si64(*(_DWORD *)v36),
                               _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v36 + 4)))),
                             v70));
          HIDWORD(v30) = v35
                       + _mm_cvtsi64_si32(
                           _m_pmaddwd(
                             _m_pmulhw(
                               _mm_cvtsi32_si64(*(_DWORD *)(v36 + 6)),
                               _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v36 + 10)))),
                             v70))
                       + 4;
          v38.m64_u64 = 0LL;
          if ( HIDWORD(v30) >= v71 && HIDWORD(v30) < v76 )
            v38 = _mm_cvtsi32_si64(*(_DWORD *)HIDWORD(v30));
          v39 = _m_psllqi(v38, 0x20u);
          if ( (unsigned int)v30 >= v71 && (unsigned int)v30 < v76 )
            v39 = _m_por(v39, _mm_cvtsi32_si64(*(_DWORD *)v30));
          _mm_stream_pi(v34, v39);
          v36 += 12;
          v35 += 8;
          ++v34;
          --v37;
        }
        while ( v37 );
        v34 = (__m64 *)((char *)v62 + (_DWORD)v72);
        v35 = v73 + v58;
        v36 = v74 + v54;
        v33 = (__m64 *)((char *)v66 - 1);
      }
      while ( v66 != (__m64 *)1 );
    }
    else
    {
      v40 = a3[3];
      v41 = *a3;
      v42 = v78;
      v43 = *a4;
      do
      {
        v67 = v40;
        v63 = v41;
        v59 = v42;
        v55 = v43;
        v44 = v81;
        do
        {
          LODWORD(v30) = v42
                       + _mm_cvtsi64_si32(
                           _m_pmaddwd(
                             _m_pmulhw(
                               _mm_cvtsi32_si64(*(_DWORD *)v43),
                               _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v43 + 4)))),
                             v70));
          HIDWORD(v30) = v42
                       + _mm_cvtsi64_si32(
                           _m_pmaddwd(
                             _m_pmulhw(
                               _mm_cvtsi32_si64(*(_DWORD *)(v43 + 6)),
                               _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v43 + 10)))),
                             v70))
                       + 4;
          v45.m64_u64 = 0LL;
          if ( HIDWORD(v30) >= v71 && HIDWORD(v30) < v76 )
            v45 = _mm_cvtsi32_si64(*(_DWORD *)HIDWORD(v30));
          v46 = _m_psllqi(v45, 0x20u);
          if ( (unsigned int)v30 >= v71 && (unsigned int)v30 < v76 )
            v46 = _m_por(v46, _mm_cvtsi32_si64(*(_DWORD *)v30));
          v41->m64_u64 = (unsigned __int64)v46;
          v43 += 12;
          v42 += 8;
          ++v41;
          --v44;
        }
        while ( v44 );
        v41 = (__m64 *)((char *)v63 + (_DWORD)v72);
        v42 = v73 + v59;
        v43 = v74 + v55;
        v40 = (__m64 *)((char *)v67 - 1);
      }
      while ( v67 != (__m64 *)1 );
    }
  }
  _m_empty();
  return v30;
}

// ===== sub_413F10 @ 0x00413F10..0x00413F87 =====
int __usercall sub_413F10@<eax>(int a1@<eax>, _DWORD *a2@<edx>, int a3@<esi>, _DWORD *a4, int a5, int a6)
{
  if ( a2[4] != 6 || a4[2] != a2[2] || a4[3] != a2[3] )
    return 12;
  if ( *(_DWORD *)(a3 + 16) != 1 )
  {
    if ( *(_DWORD *)(a3 + 16) == 2 )
    {
      if ( a4[4] == 1 )
      {
        sub_4140A0(a3, a5, a1, a6);
        return 0;
      }
      return 15;
    }
    return 0;
  }
  if ( a4[4] == 1 )
  {
    sub_413F90(a3, a5, a1, a6);
    return 0;
  }
  return 15;
}

// ===== sub_413F90 @ 0x00413F90..0x0041409E =====
int __fastcall sub_413F90(int *a1, int *a2, int *a3, unsigned int *a4, int a5, int a6)
{
  int v8; // ecx
  int v9; // edi
  int v10; // esi
  int v11; // ecx
  int v12; // ebx
  unsigned int *v13; // eax
  __m64 v14; // mm0
  __m64 v15; // mm1
  int result; // eax
  int v17; // [esp-10h] [ebp-4Ch]
  int v18; // [esp-8h] [ebp-44h]
  int v19; // [esp-4h] [ebp-40h]
  int v20; // [esp+20h] [ebp-1Ch]
  int v21; // [esp+28h] [ebp-14h]
  unsigned int v22; // [esp+2Ch] [ebp-10h]
  int v23; // [esp+30h] [ebp-Ch]
  unsigned int v24; // [esp+34h] [ebp-8h]
  int v25; // [esp+38h] [ebp-4h]
  int v26; // [esp+44h] [ebp+8h]
  int v27; // [esp+48h] [ebp+Ch]
  int v28; // [esp+50h] [ebp+14h]

  v20 = *a1;
  v25 = a1[1];
  v26 = a3[1];
  v27 = a2[1];
  v21 = a1[2];
  v22 = *a4;
  v17 = 16 * a6;
  v28 = *a3;
  v23 = a4[1] * a4[3];
  v24 = v23 + *a4;
  v8 = a1[3];
  v9 = v20;
  v10 = *a2;
  do
  {
    v19 = v8;
    v18 = v10;
    v11 = v21;
    v12 = 0;
    do
    {
      v13 = (unsigned int *)(v12
                           + v28
                           + _mm_cvtsi64_si32(
                               _m_pmaddwd(
                                 _m_pmulhw(
                                   _mm_cvtsi32_si64(*(_DWORD *)v10),
                                   _mm_cvtsi32_si64(*(_DWORD *)(a5 + 4 * *(unsigned __int16 *)(v10 + 4)))),
                                 (__m64)((v26 << 16) + 4))));
      v14 = _mm_cvtsi32_si64(*(_DWORD *)(v9 + v12));
      while ( (unsigned int)v13 < v22 )
        v13 = (unsigned int *)((char *)v13 + v23);
      while ( (unsigned int)v13 >= v24 )
        v13 = (unsigned int *)((char *)v13 - v23);
      v10 += 6;
      v15 = _m_punpcklbw(_mm_cvtsi32_si64(*v13), 0LL);
      v12 += 4;
      *(_DWORD *)(v9 + v12 - 4) = _mm_cvtsi64_si32(
                                    _m_packuswb(
                                      _m_paddw(
                                        _m_pmulhw(
                                          _m_psllwi(_m_psubw(_m_punpcklbw(v14, 0LL), v15), 4u),
                                          (__m64)(0x100010001LL * (unsigned int)v17)),
                                        v15),
                                      0LL));
      --v11;
    }
    while ( v11 );
    result = v25;
    v9 += v25;
    v28 += v26;
    v10 = v27 + v18;
    v8 = v19 - 1;
  }
  while ( v19 != 1 );
  _m_empty();
  return result;
}
