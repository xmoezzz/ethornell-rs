#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_40C0F0 @ 0x0040C0F0..0x0040C1AA =====
int __cdecl sub_40C0F0(int a1, int a2, int a3, int a4, int a5)
{
  int v5; // ecx
  int v7; // eax
  int v8; // [esp+10h] [ebp-10h] BYREF
  _DWORD v9[3]; // [esp+14h] [ebp-Ch] BYREF

  v5 = *(_DWORD *)(a1 + 16);
  if ( v5 != 1 && v5 != 2 )
    return 10;
  v7 = *(_DWORD *)(a2 + 16);
  if ( v7 != *(_DWORD *)(a3 + 16) || v5 != v7 )
    return 9;
  if ( !a5 || (v9[0] = a1, v9[1] = a2, v9[2] = a3, v8 = a4, !sub_419DE0(sub_41A080, v9, 3, 0, 0, &v8)) )
  {
    if ( *(_DWORD *)(a1 + 16) == 1 )
    {
      sub_40C1B0(a2, a4);
      return 0;
    }
    sub_40C430(a1, a3);
  }
  return 0;
}

// ===== sub_40C1B0 @ 0x0040C1B0..0x0040C427 =====
unsigned int __usercall sub_40C1B0@<eax>(__m128i **a1@<eax>, __m128i **a2@<edx>, __m128i **a3, __int16 a4)
{
  __m128i *v4; // esi
  unsigned int v5; // edi
  __m128i *v6; // edx
  __m128i *v7; // ecx
  __m128i *v8; // eax
  unsigned int result; // eax
  __m128i v10; // xmm1
  __m128i v11; // xmm2
  int v12; // edi
  unsigned int *v13; // eax
  int v14; // edx
  __m128i v15; // xmm1
  unsigned int *v16; // eax
  __m128i *v17; // edi
  __m128i v18; // xmm1
  unsigned int v19; // ecx
  const __m128i *v20; // eax
  int v21; // edi
  int v22; // edx
  __m128i v23; // xmm1
  __m128i *v24; // eax
  unsigned int v25; // edi
  __m128i v26; // xmm1
  unsigned int v27; // [esp+8h] [ebp-1Ch]
  unsigned int v28; // [esp+8h] [ebp-1Ch]
  unsigned int v29; // [esp+8h] [ebp-1Ch]
  int i; // [esp+Ch] [ebp-18h]
  unsigned int v31; // [esp+10h] [ebp-14h]
  __m128i *v32; // [esp+14h] [ebp-10h]
  __m128i *v33; // [esp+18h] [ebp-Ch]
  __m128i *v34; // [esp+1Ch] [ebp-8h]
  __m128i *v35; // [esp+20h] [ebp-4h]
  __m128i *v36; // [esp+2Ch] [ebp+8h]
  unsigned int v37; // [esp+2Ch] [ebp+8h]
  unsigned int v38; // [esp+30h] [ebp+Ch]

  v4 = *a2;
  v33 = a2[1];
  v31 = (unsigned int)a3[1];
  v32 = a1[1];
  v5 = (unsigned int)a1[3];
  v35 = *a3;
  v6 = *a1;
  v34 = *a1;
  if ( (unsigned int)a3[3] <= v5 )
    v5 = (unsigned int)a3[3];
  v7 = a3[2];
  v8 = a1[2];
  if ( v7 > v8 )
    v7 = v8;
  result = (__int16)(16 * a4);
  v10 = _mm_cvtsi32_si128(result);
  v36 = v7;
  v11 = _mm_shuffle_epi32(_mm_unpacklo_epi16(v10, v10), 0);
  if ( ((unsigned __int8)v7 & 1) != 0 )
  {
    if ( v4 == v35 )
    {
      result = v5;
      if ( v5 )
      {
        while ( 1 )
        {
          v28 = result - 1;
          v16 = (unsigned int *)v4;
          v17 = v7;
          if ( v36 )
          {
            do
            {
              v18 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*v16), (__m128i)0LL);
              *v16 = _mm_cvtsi128_si32(
                       _mm_packus_epi16(
                         _mm_add_epi16(
                           v18,
                           _mm_mulhi_epi16(
                             _mm_slli_epi16(
                               _mm_sub_epi16(
                                 _mm_unpacklo_epi8(
                                   _mm_cvtsi32_si128(*(unsigned int *)((char *)v16 + (char *)v6 - (char *)v4)),
                                   (__m128i)0LL),
                                 v18),
                               4u),
                             v11)),
                         (__m128i)0LL));
              ++v16;
              v17 = (__m128i *)((char *)v17 - 1);
            }
            while ( v17 );
          }
          result = v28;
          v4 = (__m128i *)((char *)v4 + (_DWORD)v33);
          v6 = (__m128i *)((char *)v6 + (_DWORD)v32);
          if ( !v28 )
            break;
          v7 = v36;
        }
      }
    }
    else
    {
      for ( ; v5; v34 = v6 )
      {
        v27 = --v5;
        if ( v7 )
        {
          v12 = (char *)v35 - (char *)v6;
          v13 = (unsigned int *)v6;
          v14 = (char *)v4 - (char *)v6;
          for ( i = v12; ; v12 = i )
          {
            v15 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(unsigned int *)((char *)v13 + v12)), (__m128i)0LL);
            _mm_stream_si32(
              (int *)((char *)v13 + v14),
              _mm_cvtsi128_si32(
                _mm_packus_epi16(
                  _mm_add_epi16(
                    v15,
                    _mm_mulhi_epi16(
                      _mm_slli_epi16(_mm_sub_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(*v13), (__m128i)0LL), v15), 4u),
                      v11)),
                  (__m128i)0LL)));
            ++v13;
            v7 = (__m128i *)((char *)v7 - 1);
            if ( !v7 )
              break;
          }
          v6 = v34;
          v5 = v27;
        }
        result = v31;
        v6 = (__m128i *)((char *)v6 + (_DWORD)v32);
        v4 = (__m128i *)((char *)v4 + (_DWORD)v33);
        v35 = (__m128i *)((char *)v35 + v31);
        v7 = v36;
      }
    }
  }
  else
  {
    v19 = (unsigned int)v7 >> 1;
    v37 = v19;
    if ( v4 == v35 )
    {
      result = v5;
      if ( v5 )
      {
        while ( 1 )
        {
          v38 = result - 1;
          v24 = v4;
          v25 = v19;
          if ( v37 )
          {
            do
            {
              v26 = _mm_unpacklo_epi8(_mm_loadl_epi64(v24), (__m128i)0LL);
              v24->m128i_i64[0] = _mm_packus_epi16(
                                    _mm_add_epi16(
                                      v26,
                                      _mm_mulhi_epi16(
                                        _mm_slli_epi16(
                                          _mm_sub_epi16(
                                            _mm_unpacklo_epi8(
                                              _mm_loadl_epi64((__m128i *)((char *)v24 + (char *)v6 - (char *)v4)),
                                              (__m128i)0LL),
                                            v26),
                                          4u),
                                        v11)),
                                    (__m128i)0LL).m128i_u64[0];
              v24 = (__m128i *)((char *)v24 + 8);
              --v25;
            }
            while ( v25 );
          }
          result = v38;
          v4 = (__m128i *)((char *)v4 + (_DWORD)v33);
          v6 = (__m128i *)((char *)v6 + (_DWORD)v32);
          if ( !v38 )
            break;
          v19 = v37;
        }
      }
    }
    else
    {
      for ( ; v5; v34 = v6 )
      {
        v29 = --v5;
        if ( v19 )
        {
          v20 = v6;
          v21 = (char *)v35 - (char *)v6;
          v22 = (char *)v4 - (char *)v6;
          do
          {
            v23 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)((char *)v20 + v21)), (__m128i)0LL);
            _mm_stream_pi(
              (__m64 *)((char *)v20->m128i_u64 + v22),
              _mm_movepi64_pi64(
                _mm_packus_epi16(
                  _mm_add_epi16(
                    v23,
                    _mm_mulhi_epi16(
                      _mm_slli_epi16(_mm_sub_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64(v20), (__m128i)0LL), v23), 4u),
                      v11)),
                  (__m128i)0LL)));
            v20 = (const __m128i *)((char *)v20 + 8);
            --v19;
          }
          while ( v19 );
          v5 = v29;
          v6 = v34;
        }
        result = v31;
        v6 = (__m128i *)((char *)v6 + (_DWORD)v32);
        v4 = (__m128i *)((char *)v4 + (_DWORD)v33);
        v35 = (__m128i *)((char *)v35 + v31);
        v19 = v37;
      }
      _m_empty();
    }
  }
  return result;
}

// ===== sub_40C430 @ 0x0040C430..0x0040CA6A =====
__m64 *__fastcall sub_40C430(int a1, __m64 **a2, __m64 **a3, __m64 **a4)
{
  __m64 *v4; // esi
  __m64 *result; // eax
  int i; // edx
  __m64 *v7; // edx
  int *v8; // edi
  int v9; // edx
  int v10; // eax
  __m64 v11; // mm2
  __m64 v12; // mm1
  int v13; // eax
  unsigned int v14; // esi
  unsigned int *v15; // edi
  int v16; // edx
  int v17; // eax
  __m64 v18; // mm2
  __m64 v19; // mm1
  int v20; // eax
  unsigned int v21; // esi
  __m64 *jj; // edi
  int v23; // edx
  int v24; // esi
  __m64 *v25; // edi
  __m64 v26; // mm3
  __m64 v27; // mm2
  __m64 v28; // mm1
  int v29; // eax
  unsigned int v30; // esi
  __m64 v31; // mm6
  __m64 v32; // mm3
  __m64 v33; // mm4
  int v34; // eax
  unsigned int v35; // esi
  __m64 v36; // mm1
  __m64 *v37; // edi
  int v38; // edx
  int v39; // eax
  __m64 v40; // mm2
  __m64 v41; // mm4
  __m64 v42; // mm1
  int v43; // eax
  unsigned int v44; // esi
  __m64 v45; // mm6
  __m64 v46; // mm3
  __m64 v47; // mm4
  int v48; // eax
  unsigned int v49; // esi
  __m64 v50; // mm1
  int m; // [esp+8h] [ebp-448h]
  int v52; // [esp+8h] [ebp-448h]
  char *v53; // [esp+Ch] [ebp-444h]
  int v54; // [esp+Ch] [ebp-444h]
  __m64 *v55; // [esp+10h] [ebp-440h]
  __m64 *v56; // [esp+14h] [ebp-43Ch]
  __m64 v57; // [esp+18h] [ebp-438h]
  __m64 *v58; // [esp+1Ch] [ebp-434h]
  int kk; // [esp+1Ch] [ebp-434h]
  __m64 *v60; // [esp+24h] [ebp-42Ch]
  int k; // [esp+24h] [ebp-42Ch]
  int ii; // [esp+24h] [ebp-42Ch]
  __m64 *v63; // [esp+28h] [ebp-428h]
  __m64 *v64; // [esp+2Ch] [ebp-424h]
  __m64 *v65; // [esp+30h] [ebp-420h]
  unsigned int v66; // [esp+34h] [ebp-41Ch]
  unsigned int v67; // [esp+34h] [ebp-41Ch]
  unsigned int v68; // [esp+38h] [ebp-418h]
  unsigned int v69; // [esp+38h] [ebp-418h]
  unsigned int v70; // [esp+38h] [ebp-418h]
  unsigned int v71; // [esp+38h] [ebp-418h]
  __m64 *v72; // [esp+3Ch] [ebp-414h]
  __m64 *j; // [esp+3Ch] [ebp-414h]
  __m64 *n; // [esp+3Ch] [ebp-414h]
  __m64 v75[129]; // [esp+40h] [ebp-410h]

  v4 = *a3;
  v55 = a3[1];
  v60 = a2[1];
  v56 = a4[1];
  v72 = *a2;
  result = *a4;
  v63 = *a3;
  v64 = *a4;
  v65 = a2[3];
  if ( v65 > a4[3] )
    v65 = a4[3];
  v66 = (unsigned int)a2[2];
  if ( v66 > (unsigned int)a4[2] )
    v66 = (unsigned int)a4[2];
  for ( i = 0; i < 129; ++i )
  {
    v57.m64_i32[1] = (unsigned __int16)i;
    v57.m64_i16[1] = i;
    v57.m64_i16[0] = i;
    v75[i] = v57;
  }
  if ( (v66 & 1) != 0 )
  {
    v7 = v65;
    if ( v4 == v72 )
    {
      for ( j = v65; j; v64 = result )
      {
        j = (__m64 *)((char *)j - 1);
        v15 = (unsigned int *)v4;
        v69 = v66;
        if ( v66 )
        {
          v16 = 256 - a1;
          v17 = (char *)result - (char *)v4;
          for ( k = v17; ; v17 = k )
          {
            --v69;
            v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v15), 0LL);
            v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v15 + v17)), 0LL);
            v20 = v16 * _m_pextrw(v18, 3);
            v21 = v20 + a1 * _m_pextrw(v19, 3);
            if ( v21 )
            {
              *v15 = _mm_cvtsi64_si32(
                       _m_packuswb(
                         _m_pinsrw(
                           _m_paddw(
                             _m_psrawi(_m_pmullw(_m_psubw(v18, v19), v75[(v20 << 7) / v21]), 7u),
                             _m_pand(v19, (__m64)0xFFFFFFFFFFFFLL)),
                           v21 >> 8,
                           3),
                         0LL));
              v16 = 256 - a1;
            }
            else
            {
              *v15 = 0;
            }
            ++v15;
            if ( !v69 )
              break;
          }
          result = v64;
          v4 = v63;
        }
        v4 = (__m64 *)((char *)v4 + (_DWORD)v55);
        result = (__m64 *)((char *)result + (_DWORD)v56);
        v63 = v4;
      }
    }
    else if ( v65 )
    {
      do
      {
        v58 = (__m64 *)((char *)v7 - 1);
        v8 = (int *)v4;
        v68 = v66;
        if ( v66 )
        {
          v53 = (char *)((char *)result - (char *)v72);
          v9 = 256 - a1;
          v10 = (char *)v72 - (char *)v4;
          for ( m = (char *)v72 - (char *)v4; ; v10 = m )
          {
            --v68;
            v11 = _m_punpcklbw(_mm_cvtsi32_si64(*(int *)((char *)v8 + v10)), 0LL);
            v12 = _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)&v53[(_DWORD)v8 + v10]), 0LL);
            v13 = v9 * _m_pextrw(v11, 3);
            v14 = v13 + a1 * _m_pextrw(v12, 3);
            if ( v14 )
            {
              _mm_stream_si32(
                v8,
                _mm_cvtsi64_si32(
                  _m_packuswb(
                    _m_pinsrw(
                      _m_paddw(
                        _m_psrawi(_m_pmullw(_m_psubw(v11, v12), v75[(v13 << 7) / v14]), 7u),
                        _m_pand(v12, (__m64)0xFFFFFFFFFFFFLL)),
                      v14 >> 8,
                      3),
                    0LL)));
              v9 = 256 - a1;
            }
            else
            {
              _mm_stream_si32(v8, 0);
            }
            ++v8;
            if ( !v68 )
              break;
          }
          result = v64;
          v4 = v63;
        }
        v72 = (__m64 *)((char *)v72 + (_DWORD)v60);
        v4 = (__m64 *)((char *)v4 + (_DWORD)v55);
        result = (__m64 *)((char *)result + (_DWORD)v56);
        v7 = v58;
        v63 = v4;
        v64 = result;
      }
      while ( v58 );
    }
  }
  else
  {
    v67 = v66 >> 1;
    if ( v4 == v72 )
    {
      for ( n = v65; n; v64 = result )
      {
        n = (__m64 *)((char *)n - 1);
        v37 = v4;
        v71 = v67;
        if ( v67 )
        {
          v38 = 256 - a1;
          v39 = (char *)result - (char *)v4;
          for ( ii = v39; ; v39 = ii )
          {
            --v71;
            v40 = _m_punpcklbw((__m64)v37->m64_u64, 0LL);
            v41 = *(__m64 *)((char *)v37 + v39);
            v42 = _m_punpcklbw(v41, 0LL);
            v43 = v38 * _m_pextrw(v40, 3);
            v44 = v43 + a1 * _m_pextrw(v42, 3);
            if ( v44 )
            {
              v38 = 256 - a1;
              v45 = _m_pinsrw(
                      _m_paddw(
                        _m_psrawi(_m_pmullw(v75[(v43 << 7) / v44], _m_psubw(v40, v42)), 7u),
                        _m_pand(v42, (__m64)0xFFFFFFFFFFFFLL)),
                      v44 >> 8,
                      3);
            }
            else
            {
              v45.m64_u64 = 0LL;
            }
            v46 = _m_punpckhbw((__m64)v37->m64_u64, 0LL);
            v47 = _m_punpckhbw(v41, 0LL);
            v48 = v38 * _m_pextrw(v46, 3);
            v49 = v48 + a1 * _m_pextrw(v47, 3);
            if ( v49 )
            {
              v38 = 256 - a1;
              v50 = _m_pinsrw(
                      _m_paddw(
                        _m_pand(v47, (__m64)0xFFFFFFFFFFFFLL),
                        _m_psrawi(_m_pmullw(_m_psubw(v46, v47), v75[(v48 << 7) / v49]), 7u)),
                      v49 >> 8,
                      3);
            }
            else
            {
              v50.m64_u64 = 0LL;
            }
            v37->m64_u64 = (unsigned __int64)_m_packuswb(v45, v50);
            ++v37;
            if ( !v71 )
              break;
          }
          result = v64;
          v4 = v63;
        }
        v4 = (__m64 *)((char *)v4 + (_DWORD)v55);
        result = (__m64 *)((char *)result + (_DWORD)v56);
        v63 = v4;
      }
    }
    else
    {
      for ( jj = v65; jj; v64 = result )
      {
        jj = (__m64 *)((char *)jj - 1);
        v52 = (int)jj;
        v70 = v67;
        if ( v67 )
        {
          v54 = (char *)v4 - (char *)result;
          v23 = 256 - a1;
          v24 = (char *)v72 - (char *)result;
          v25 = result;
          for ( kk = (char *)v72 - (char *)result; ; v24 = kk )
          {
            v26 = *(__m64 *)((char *)v25 + v24);
            --v70;
            v27 = _m_punpcklbw(v26, 0LL);
            v28 = _m_punpcklbw((__m64)v25->m64_u64, 0LL);
            v29 = v23 * _m_pextrw(v27, 3);
            v30 = v29 + a1 * _m_pextrw(v28, 3);
            if ( v30 )
            {
              v23 = 256 - a1;
              v31 = _m_pinsrw(
                      _m_paddw(
                        _m_psrawi(_m_pmullw(v75[(v29 << 7) / v30], _m_psubw(v27, v28)), 7u),
                        _m_pand(v28, (__m64)0xFFFFFFFFFFFFLL)),
                      v30 >> 8,
                      3);
            }
            else
            {
              v31.m64_u64 = 0LL;
            }
            v32 = _m_punpckhbw(v26, 0LL);
            v33 = _m_punpckhbw((__m64)v25->m64_u64, 0LL);
            v34 = v23 * _m_pextrw(v32, 3);
            v35 = v34 + a1 * _m_pextrw(v33, 3);
            if ( v35 )
            {
              v23 = 256 - a1;
              v36 = _m_pinsrw(
                      _m_paddw(
                        _m_pand(v33, (__m64)0xFFFFFFFFFFFFLL),
                        _m_psrawi(_m_pmullw(_m_psubw(v32, v33), v75[(v34 << 7) / v35]), 7u)),
                      v35 >> 8,
                      3);
            }
            else
            {
              v36.m64_u64 = 0LL;
            }
            _mm_stream_pi((__m64 *)((char *)v25++ + v54), _m_packuswb(v31, v36));
            if ( !v70 )
              break;
          }
          jj = (__m64 *)v52;
          result = v64;
          v4 = v63;
        }
        v4 = (__m64 *)((char *)v4 + (_DWORD)v55);
        result = (__m64 *)((char *)result + (_DWORD)v56);
        v72 = (__m64 *)((char *)v72 + (_DWORD)v60);
        v63 = v4;
      }
    }
  }
  _m_empty();
  return result;
}

// ===== sub_40CA70 @ 0x0040CA70..0x0040CAD3 =====
void __usercall sub_40CA70(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int v3; // eax
  int v4; // eax

  if ( a1 )
  {
    v3 = *(_DWORD *)(a3 + 16);
    if ( v3 )
    {
      v4 = v3 - 1;
      if ( v4 )
      {
        if ( v4 == 1 )
        {
          if ( *(_DWORD *)(a2 + 16) == 1 )
          {
            sub_40CDD0(a3);
          }
          else if ( *(_DWORD *)(a2 + 16) == 2 )
          {
            sub_40CFF0(a3);
          }
        }
      }
      else if ( *(_DWORD *)(a2 + 16) == 1 )
      {
        sub_40CBF0(a2, a3, a1);
      }
      else if ( *(_DWORD *)(a2 + 16) == 2 )
      {
        sub_40CCD0(a2, a3);
      }
    }
    else
    {
      sub_40CAE0(a2, a3, a1);
    }
  }
}

// ===== sub_40CAE0 @ 0x0040CAE0..0x0040CBEA =====
int __cdecl sub_40CAE0(int *a1, _DWORD *a2, int a3)
{
  _DWORD *v3; // ebx
  int v4; // edx
  int v5; // esi
  int result; // eax
  int v7; // ecx
  int v8; // edx
  __int16 *v9; // edi
  __int16 v10; // ax
  __int16 v11; // si
  unsigned int v12; // ecx
  unsigned int v13; // edx
  __int16 v14; // dx
  unsigned int v15; // eax
  __int16 v16; // ax
  __int16 v17; // cx
  int v18; // [esp+8h] [ebp-14h]
  int v19; // [esp+Ch] [ebp-10h]
  int v20; // [esp+10h] [ebp-Ch]
  int i; // [esp+14h] [ebp-8h]
  int v22; // [esp+18h] [ebp-4h]

  v3 = a2;
  v4 = *a2;
  v5 = *a1;
  result = a2[3];
  v22 = *a1;
  for ( i = *a2; result; i = v4 )
  {
    v7 = v3[2];
    v18 = --result;
    if ( v7 )
    {
      v8 = v4 - v5;
      v9 = (__int16 *)(v5 + 2 * v7);
      v19 = v8;
      do
      {
        v10 = *(__int16 *)((char *)v9-- + v8 - 2);
        v20 = --v7;
        if ( v10 )
        {
          v11 = *v9;
          v12 = (*v9 & 0x1F) + ((a3 * (v10 & 0x1Fu)) >> 8);
          if ( v12 >= 0x20 )
            LOWORD(v12) = 31;
          v13 = (v11 & 0x3E0) + ((a3 * (v10 & 0x3E0u)) >> 8);
          if ( v13 >= 0x400 )
            v14 = 992;
          else
            v14 = v13 & 0x3E0;
          v15 = (v11 & 0x7C00) + ((a3 * (v10 & 0x7C00u)) >> 8);
          if ( v15 >= 0x8000 )
            v16 = 31744;
          else
            v16 = v15 & 0x7C00;
          v3 = a2;
          v5 = v22;
          v17 = v14 + v12;
          v8 = v19;
          *v9 = v16 + v17;
          v7 = v20;
        }
      }
      while ( v7 );
      result = v18;
      v4 = i;
    }
    v5 += a1[1];
    v4 += v3[1];
    v22 = v5;
  }
  return result;
}

// ===== sub_40CBF0 @ 0x0040CBF0..0x0040CCCC =====
unsigned __int8 **__cdecl sub_40CBF0(unsigned __int8 **a1, unsigned __int8 **a2, int a3)
{
  unsigned __int8 **result; // eax
  unsigned __int8 *v4; // edx
  unsigned __int8 **v5; // ebx
  unsigned __int8 *v6; // ecx
  unsigned __int8 *v7; // esi
  unsigned __int8 *v8; // edi
  int v9; // eax
  unsigned int v10; // eax
  int v11; // esi
  unsigned int v12; // eax
  int v13; // esi
  unsigned int v14; // eax
  unsigned __int8 *v15; // [esp+8h] [ebp-Ch]
  unsigned __int8 *i; // [esp+Ch] [ebp-8h]
  unsigned __int8 *v17; // [esp+10h] [ebp-4h]

  result = a2;
  v4 = *a2;
  v5 = a1;
  v6 = *a1;
  v7 = a2[3];
  v17 = *a1;
  for ( i = *a2; v7; i = v4 )
  {
    v8 = result[2];
    v15 = --v7;
    if ( v8 )
    {
      do
      {
        v9 = *v4;
        --v8;
        if ( v9 + v4[2] + v4[1] > 0 )
        {
          v10 = ((unsigned int)(a3 * v9) >> 8) + *v6;
          if ( v10 >= 0x100 )
            LOBYTE(v10) = -1;
          v11 = v6[1];
          *v6 = v10;
          v12 = v11 + ((a3 * (unsigned int)v4[1]) >> 8);
          if ( v12 >= 0x100 )
            LOBYTE(v12) = -1;
          v13 = v6[2];
          v6[1] = v12;
          v14 = v13 + ((a3 * (unsigned int)v4[2]) >> 8);
          if ( v14 >= 0x100 )
            LOBYTE(v14) = -1;
          v6[2] = v14;
        }
        v5 = a1;
        result = a2;
        v6 = &v6[(_DWORD)a1[5]];
        v4 = &v4[(_DWORD)a2[5]];
      }
      while ( v8 );
      v4 = i;
      v6 = v17;
      v7 = v15;
    }
    v6 = &v6[(_DWORD)v5[1]];
    v4 = &v4[(_DWORD)result[1]];
    v17 = v6;
  }
  return result;
}

// ===== sub_40CCD0 @ 0x0040CCD0..0x0040CDCE =====
unsigned __int8 *__usercall sub_40CCD0@<eax>(int a1@<edi>, unsigned __int8 **a2, unsigned __int8 **a3)
{
  unsigned __int8 *result; // eax
  unsigned __int8 **v4; // esi
  unsigned __int8 *v5; // ecx
  unsigned __int8 *v6; // edx
  int v7; // ecx
  unsigned int v8; // ecx
  int v9; // esi
  unsigned int v10; // ecx
  int v11; // esi
  unsigned int v12; // ecx
  unsigned int v13; // esi
  char v14; // cl
  unsigned __int8 *v15; // [esp+4h] [ebp-10h]
  unsigned __int8 *i; // [esp+8h] [ebp-Ch]
  unsigned __int8 *v17; // [esp+Ch] [ebp-8h]
  unsigned __int8 *v18; // [esp+10h] [ebp-4h]

  result = *a2;
  v4 = a3;
  v5 = a3[3];
  v6 = *a3;
  v17 = *a2;
  for ( i = *a3; v5; i = v6 )
  {
    v15 = --v5;
    v18 = v4[2];
    if ( v18 )
    {
      do
      {
        v7 = *v6;
        --v18;
        if ( v7 + v6[2] + v6[1] > 0 )
        {
          v8 = ((unsigned int)(a1 * v7) >> 8) + *result;
          if ( v8 >= 0x100 )
            LOBYTE(v8) = -1;
          v9 = result[1];
          *result = v8;
          v10 = v9 + ((a1 * (unsigned int)v6[1]) >> 8);
          if ( v10 >= 0x100 )
            LOBYTE(v10) = -1;
          v11 = result[2];
          result[1] = v10;
          v12 = v11 + ((a1 * (unsigned int)v6[2]) >> 8);
          if ( v12 >= 0x100 )
            LOBYTE(v12) = -1;
          v13 = a1 + result[3];
          result[2] = v12;
          v14 = v13;
          if ( v13 >= 0x100 )
            v14 = -1;
          result[3] = v14;
        }
        result = &result[(_DWORD)a2[5]];
        v6 = &v6[(_DWORD)a3[5]];
      }
      while ( v18 );
      v5 = v15;
      v6 = i;
      result = v17;
      v4 = a3;
    }
    result = &result[(_DWORD)a2[1]];
    v6 = &v6[(_DWORD)v4[1]];
    v17 = result;
  }
  return result;
}

// ===== sub_40CDD0 @ 0x0040CDD0..0x0040CFEF =====
int __fastcall sub_40CDD0(int *a1, unsigned int a2, unsigned int **a3)
{
  int v4; // ecx
  unsigned int v5; // edx
  bool v6; // zf
  int v7; // ecx
  int result; // eax
  int v9; // ecx
  unsigned int *v10; // edx
  char *v11; // esi
  unsigned int v12; // eax
  unsigned int v13; // edi
  unsigned int *v14; // ecx
  unsigned int *v15; // edx
  int v16; // esi
  unsigned int *v17; // [esp+8h] [ebp-42Ch]
  int v18; // [esp+Ch] [ebp-428h]
  __int64 v19; // [esp+10h] [ebp-424h]
  int *v20; // [esp+14h] [ebp-420h]
  unsigned int *v21; // [esp+18h] [ebp-41Ch]
  int v22; // [esp+1Ch] [ebp-418h]
  int v23; // [esp+20h] [ebp-414h]
  unsigned int *v24; // [esp+24h] [ebp-410h]
  _QWORD v25[128]; // [esp+28h] [ebp-40Ch] BYREF

  v23 = *a1;
  v18 = a1[1];
  v24 = *a3;
  v21 = a3[2];
  v17 = a3[1];
  if ( a2 >= 0x100 )
  {
    v20 = dword_50A8D0;
  }
  else
  {
    v4 = 0;
    v5 = 0;
    do
    {
      HIDWORD(v19) = (unsigned __int16)(v5 >> 8);
      WORD1(v19) = v5 >> 8;
      LOWORD(v19) = WORD1(v19);
      v25[v4++] = v19;
      v5 += a2;
    }
    while ( v4 < 128 );
    v20 = (int *)v25;
  }
  v6 = !sub_407AE0();
  result = v7;
  if ( v6 )
  {
    if ( v7 )
    {
      v9 = v23;
      v10 = v21;
      do
      {
        v22 = --result;
        if ( v10 )
        {
          v11 = (char *)v24 - v9;
          do
          {
            v12 = *(_DWORD *)&v11[v9];
            v10 = (unsigned int *)((char *)v10 - 1);
            if ( (v12 & 0xFE000000) != 0 )
            {
              v13 = *(_DWORD *)v9;
              _mm_prefetch((const char *)(v9 + 128), 0);
              *(_DWORD *)v9 = _mm_cvtsi64_si32(
                                _m_paddusb(
                                  _mm_cvtsi32_si64(v13),
                                  _m_packuswb(
                                    _m_psrawi(
                                      _m_pmullw(
                                        _m_punpcklbw(_mm_cvtsi32_si64(v12), 0LL),
                                        *(__m64 *)&v20[2 * (v12 >> 25)]),
                                      7u),
                                    0LL)));
            }
            v9 += 4;
          }
          while ( v10 );
          v9 = v23;
          v10 = v21;
          result = v22;
        }
        v9 += v18;
        v24 = (unsigned int *)((char *)v24 + (_DWORD)v17);
        v23 = v9;
      }
      while ( result );
    }
  }
  else if ( v7 )
  {
    v14 = v24;
    v15 = v21;
    do
    {
      --result;
      if ( v15 )
      {
        v16 = v23 - (_DWORD)v14;
        do
        {
          v15 = (unsigned int *)((char *)v15 - 1);
          if ( (*v14 & 0xFE000000) != 0 )
            *(unsigned int *)((char *)v14 + v16) = _mm_cvtsi64_si32(
                                                     _m_paddusb(
                                                       _mm_cvtsi32_si64(*(unsigned int *)((char *)v14 + v16)),
                                                       _m_packuswb(
                                                         _m_psrawi(
                                                           _m_pmullw(
                                                             _m_punpcklbw(_mm_cvtsi32_si64(*v14), 0LL),
                                                             *(__m64 *)&v20[2 * (*v14 >> 25)]),
                                                           7u),
                                                         0LL)));
          ++v14;
        }
        while ( v15 );
        v14 = v24;
        v15 = v21;
      }
      v14 = (unsigned int *)((char *)v14 + (_DWORD)v17);
      v23 += v18;
      v24 = v14;
    }
    while ( result );
  }
  _m_empty();
  return result;
}

// ===== sub_40CFF0 @ 0x0040CFF0..0x0040D1F8 =====
unsigned int __fastcall sub_40CFF0(int *a1, unsigned int a2, unsigned int **a3)
{
  int v4; // ecx
  unsigned int v5; // esi
  unsigned int v6; // edx
  bool v7; // zf
  unsigned int result; // eax
  unsigned int v9; // esi
  unsigned int *v10; // edx
  int v11; // ecx
  unsigned int v12; // edi
  unsigned int *v13; // edx
  unsigned int *v14; // ecx
  int v15; // esi
  __m64 v16; // [esp+8h] [ebp-42Ch]
  unsigned int *v17; // [esp+10h] [ebp-424h]
  int v18; // [esp+14h] [ebp-420h]
  unsigned int *v19; // [esp+18h] [ebp-41Ch]
  int v20; // [esp+1Ch] [ebp-418h]
  unsigned int v21; // [esp+20h] [ebp-414h]
  unsigned int v22; // [esp+20h] [ebp-414h]
  unsigned int *v23; // [esp+24h] [ebp-410h]
  __m64 v24[128]; // [esp+28h] [ebp-40Ch]

  v20 = *a1;
  v23 = *a3;
  v18 = a1[1];
  v17 = a3[1];
  v21 = (unsigned int)a3[3];
  v19 = a3[2];
  v4 = 0;
  v5 = a2 >> 1;
  v6 = 0;
  do
  {
    v16.m64_i16[3] = v5;
    v16.m64_i16[2] = v6 >> 8;
    v16.m64_i16[1] = v16.m64_i16[2];
    v16.m64_i16[0] = v16.m64_i16[2];
    v24[v4++] = v16;
    v6 += a2;
  }
  while ( v4 < 128 );
  v7 = !sub_407AE0();
  result = v21;
  v9 = v21;
  if ( v7 )
  {
    if ( v21 )
    {
      do
      {
        v10 = v19;
        v11 = v20;
        --v9;
        if ( v19 )
        {
          do
          {
            result = *(unsigned int *)((char *)v23 + v11 - v20);
            v10 = (unsigned int *)((char *)v10 - 1);
            if ( (result & 0xFE000000) != 0 )
            {
              v12 = *(_DWORD *)v11;
              _mm_prefetch((const char *)(v11 + 128), 0);
              result = _mm_cvtsi64_si32(
                         _m_paddusb(
                           _mm_cvtsi32_si64(v12),
                           _m_packuswb(
                             _m_psrawi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(result), 0LL), v24[result >> 25]), 7u),
                             0LL)));
              *(_DWORD *)v11 = result;
            }
            v11 += 4;
          }
          while ( v10 );
          v11 = v20;
        }
        v20 = v18 + v11;
        v23 = (unsigned int *)((char *)v23 + (_DWORD)v17);
      }
      while ( v9 );
    }
  }
  else if ( v21 )
  {
    do
    {
      v13 = v19;
      v14 = v23;
      v22 = --v9;
      if ( v19 )
      {
        v15 = v20 - (_DWORD)v23;
        do
        {
          result = *v14;
          v13 = (unsigned int *)((char *)v13 - 1);
          if ( (*v14 & 0xFE000000) != 0 )
          {
            result = _mm_cvtsi64_si32(
                       _m_paddusb(
                         _mm_cvtsi32_si64(*(unsigned int *)((char *)v14 + v15)),
                         _m_packuswb(
                           _m_psrawi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(result), 0LL), v24[result >> 25]), 7u),
                           0LL)));
            *(unsigned int *)((char *)v14 + v15) = result;
          }
          ++v14;
        }
        while ( v13 );
        v14 = v23;
        v9 = v22;
      }
      v20 += v18;
      v23 = (unsigned int *)((char *)v14 + (_DWORD)v17);
    }
    while ( v9 );
  }
  _m_empty();
  return result;
}

// ===== sub_40D200 @ 0x0040D200..0x0040D21E =====
int __usercall sub_40D200@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<esi>)
{
  int result; // eax

  if ( a1 )
  {
    result = *(_DWORD *)(a3 + 16) - 2;
    if ( *(_DWORD *)(a3 + 16) == 2 )
    {
      result = *(_DWORD *)(a2 + 16) - 1;
      if ( *(_DWORD *)(a2 + 16) == 1 )
        return sub_40D220(a3);
    }
  }
  return result;
}

// ===== sub_40D220 @ 0x0040D220..0x0040D43F =====
int __fastcall sub_40D220(int *a1, unsigned int a2, unsigned int **a3)
{
  int v4; // ecx
  unsigned int v5; // edx
  bool v6; // zf
  int v7; // ecx
  int result; // eax
  int v9; // ecx
  unsigned int *v10; // edx
  char *v11; // esi
  unsigned int v12; // eax
  unsigned int v13; // edi
  unsigned int *v14; // ecx
  unsigned int *v15; // edx
  int v16; // esi
  unsigned int *v17; // [esp+8h] [ebp-42Ch]
  int v18; // [esp+Ch] [ebp-428h]
  __int64 v19; // [esp+10h] [ebp-424h]
  int *v20; // [esp+14h] [ebp-420h]
  unsigned int *v21; // [esp+18h] [ebp-41Ch]
  int v22; // [esp+1Ch] [ebp-418h]
  int v23; // [esp+20h] [ebp-414h]
  unsigned int *v24; // [esp+24h] [ebp-410h]
  _QWORD v25[128]; // [esp+28h] [ebp-40Ch] BYREF

  v23 = *a1;
  v18 = a1[1];
  v24 = *a3;
  v21 = a3[2];
  v17 = a3[1];
  if ( a2 >= 0x100 )
  {
    v20 = dword_50A8D0;
  }
  else
  {
    v4 = 0;
    v5 = 0;
    do
    {
      HIDWORD(v19) = (unsigned __int16)(v5 >> 8);
      WORD1(v19) = v5 >> 8;
      LOWORD(v19) = WORD1(v19);
      v25[v4++] = v19;
      v5 += a2;
    }
    while ( v4 < 128 );
    v20 = (int *)v25;
  }
  v6 = !sub_407AE0();
  result = v7;
  if ( v6 )
  {
    if ( v7 )
    {
      v9 = v23;
      v10 = v21;
      do
      {
        v22 = --result;
        if ( v10 )
        {
          v11 = (char *)v24 - v9;
          do
          {
            v12 = *(_DWORD *)&v11[v9];
            v10 = (unsigned int *)((char *)v10 - 1);
            if ( (v12 & 0xFE000000) != 0 )
            {
              v13 = *(_DWORD *)v9;
              _mm_prefetch((const char *)(v9 + 128), 0);
              *(_DWORD *)v9 = _mm_cvtsi64_si32(
                                _m_psubusb(
                                  _mm_cvtsi32_si64(v13),
                                  _m_packuswb(
                                    _m_psrawi(
                                      _m_pmullw(
                                        _m_punpcklbw(_mm_cvtsi32_si64(v12), 0LL),
                                        *(__m64 *)&v20[2 * (v12 >> 25)]),
                                      7u),
                                    0LL)));
            }
            v9 += 4;
          }
          while ( v10 );
          v9 = v23;
          v10 = v21;
          result = v22;
        }
        v9 += v18;
        v24 = (unsigned int *)((char *)v24 + (_DWORD)v17);
        v23 = v9;
      }
      while ( result );
    }
  }
  else if ( v7 )
  {
    v14 = v24;
    v15 = v21;
    do
    {
      --result;
      if ( v15 )
      {
        v16 = v23 - (_DWORD)v14;
        do
        {
          v15 = (unsigned int *)((char *)v15 - 1);
          if ( (*v14 & 0xFE000000) != 0 )
            *(unsigned int *)((char *)v14 + v16) = _mm_cvtsi64_si32(
                                                     _m_psubusb(
                                                       _mm_cvtsi32_si64(*(unsigned int *)((char *)v14 + v16)),
                                                       _m_packuswb(
                                                         _m_psrawi(
                                                           _m_pmullw(
                                                             _m_punpcklbw(_mm_cvtsi32_si64(*v14), 0LL),
                                                             *(__m64 *)&v20[2 * (*v14 >> 25)]),
                                                           7u),
                                                         0LL)));
          ++v14;
        }
        while ( v15 );
        v14 = v24;
        v15 = v21;
      }
      v14 = (unsigned int *)((char *)v14 + (_DWORD)v17);
      v23 += v18;
      v24 = v14;
    }
    while ( result );
  }
  _m_empty();
  return result;
}

// ===== sub_40D440 @ 0x0040D440..0x0040D49F =====
int __usercall sub_40D440@<eax>(int result@<eax>, int a2@<ecx>, int a3)
{
  if ( result )
  {
    if ( *(_DWORD *)(a2 + 16) == 1 )
    {
      if ( *(_DWORD *)(a3 + 16) == 1 )
      {
        return sub_40D4A0(a3);
      }
      else if ( *(_DWORD *)(a3 + 16) == 2 )
      {
        return sub_40D670(a3);
      }
    }
    else if ( *(_DWORD *)(a2 + 16) == 2 )
    {
      if ( *(_DWORD *)(a3 + 16) == 1 )
        return sub_40D820(a2, result);
    }
    else if ( *(_DWORD *)(a2 + 16) == 3 && *(_DWORD *)(a3 + 16) == 3 )
    {
      return sub_40DA40(result);
    }
  }
  return result;
}

// ===== sub_40D4A0 @ 0x0040D4A0..0x0040D664 =====
unsigned int __usercall sub_40D4A0@<eax>(unsigned int a1@<eax>, int *a2@<ecx>, int a3@<ebp>, __m128i **a4)
{
  int v4; // esi
  __m128i *v5; // edi
  unsigned int v6; // edx
  int v7; // esi
  unsigned int v8; // edx
  unsigned int result; // eax
  __m64 v10; // mm2
  int i; // ecx
  unsigned int *v12; // eax
  unsigned int v13; // esi
  __m64 v14; // mm1
  int v15; // ecx
  unsigned int v16; // edx
  __m128i si128; // xmm3
  __m128i v18; // xmm2
  __m128i v19; // xmm1
  __m128i *v20; // eax
  unsigned int v21; // esi
  int v22; // ecx
  __m128i v23; // xmm0
  __m128i v24; // [esp-30h] [ebp-3Ch] BYREF
  __int64 v25; // [esp-18h] [ebp-24h]
  int v26; // [esp-10h] [ebp-1Ch]
  unsigned int v27; // [esp-Ch] [ebp-18h]
  __m128i *v28; // [esp-8h] [ebp-14h]
  int v29; // [esp-4h] [ebp-10h]
  int v30; // [esp+0h] [ebp-Ch]
  void *v31; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v30 = a3;
  v31 = retaddr;
  v4 = *a2;
  v5 = *a4;
  v28 = a4[1];
  v6 = a2[1];
  v29 = v4;
  v7 = a2[3];
  v27 = v6;
  v8 = a2[2];
  result = a1 >> 1;
  HIDWORD(v25) = v7;
  if ( (v8 & 1) != 0 )
  {
    HIDWORD(v25) = (unsigned __int16)result;
    WORD1(v25) = result;
    LOWORD(v25) = result;
    v10.m64_u64 = v25;
    v25 = 0x10001000100LL;
    for ( i = v7; i; v29 += v27 )
    {
      HIDWORD(v25) = --i;
      v12 = (unsigned int *)v5;
      v13 = v8;
      if ( v8 )
      {
        v26 = v29 - (_DWORD)v5;
        do
        {
          v14 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), 0LL);
          *v12 = _mm_cvtsi64_si32(
                   _m_packuswb(
                     _m_paddw(
                       _m_pmulhw(
                         _m_pmullw(
                           _m_psubw(
                             _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + v26)), 0LL),
                             (__m64)0x10001000100LL),
                           v10),
                         _m_psllwi(v14, 1u)),
                       v14),
                     0LL));
          ++v12;
          --v13;
        }
        while ( v13 );
        i = HIDWORD(v25);
      }
      result = v27;
      v5 = (__m128i *)((char *)v5 + (_DWORD)v28);
    }
  }
  else
  {
    v24 = 0LL;
    result = (unsigned __int16)result;
    v15 = HIDWORD(v25);
    v16 = v8 >> 1;
    si128 = _mm_load_si128((const __m128i *)&xmmword_4E42B0);
    v18 = _mm_unpacklo_epi16(
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
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    if ( HIDWORD(v25) )
    {
      v19 = _mm_load_si128(&v24);
      do
      {
        HIDWORD(v25) = --v15;
        v20 = v5;
        v21 = v16;
        if ( v16 )
        {
          v22 = v29 - (_DWORD)v5;
          do
          {
            v23 = _mm_unpacklo_epi8(_mm_loadl_epi64(v20), v19);
            v20->m128i_i64[0] = _mm_packus_epi16(
                                  _mm_add_epi16(
                                    _mm_mulhi_epi16(
                                      _mm_mullo_epi16(
                                        _mm_sub_epi16(
                                          _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i *)((char *)v20 + v22)), v19),
                                          si128),
                                        v18),
                                      _mm_slli_epi16(v23, 1u)),
                                    v23),
                                  v19).m128i_u64[0];
            v20 = (__m128i *)((char *)v20 + 8);
            --v21;
          }
          while ( v21 );
          v15 = HIDWORD(v25);
        }
        result = v27;
        v5 = (__m128i *)((char *)v5 + (_DWORD)v28);
        v29 += v27;
      }
      while ( v15 );
    }
  }
  _m_empty();
  return result;
}

// ===== sub_40D670 @ 0x0040D670..0x0040D81A =====
unsigned int __usercall sub_40D670@<eax>(unsigned int a1@<eax>, int *a2@<ecx>, int a3@<ebp>, __m128i **a4)
{
  int v4; // esi
  __m128i *v5; // edi
  __int32 v6; // edx
  unsigned int v7; // ecx
  unsigned int result; // eax
  __m64 i; // mm2
  unsigned int *v10; // eax
  unsigned int v11; // esi
  __m64 v12; // mm0
  __int32 v13; // edx
  unsigned int v14; // ecx
  __m128i v15; // xmm2
  __m128i v16; // xmm1
  __m128i *v17; // eax
  unsigned int v18; // esi
  int v19; // edx
  __m128i v20; // xmm0
  __m128i v21; // [esp-30h] [ebp-3Ch] BYREF
  __m64 v22; // [esp-18h] [ebp-24h]
  int v23; // [esp-10h] [ebp-1Ch]
  unsigned int v24; // [esp-Ch] [ebp-18h]
  __m128i *v25; // [esp-8h] [ebp-14h]
  int v26; // [esp-4h] [ebp-10h]
  int v27; // [esp+0h] [ebp-Ch]
  void *v28; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v27 = a3;
  v28 = retaddr;
  v4 = *a2;
  v5 = *a4;
  v25 = a4[1];
  v24 = a2[1];
  v6 = a2[3];
  v7 = a2[2];
  v26 = v4;
  result = a1 >> 1;
  v22.m64_i32[1] = v6;
  if ( (v7 & 1) != 0 )
  {
    v22.m64_i32[1] = (unsigned __int16)result;
    v22.m64_i16[1] = result;
    v22.m64_i16[0] = result;
    for ( i = v22; v6; v26 += v24 )
    {
      v22.m64_i32[1] = --v6;
      v10 = (unsigned int *)v5;
      v11 = v7;
      if ( v7 )
      {
        v23 = v26 - (_DWORD)v5;
        do
        {
          v12 = _m_punpcklbw(_mm_cvtsi32_si64(*v10), 0LL);
          *v10 = _mm_cvtsi64_si32(
                   _m_packuswb(
                     _m_paddw(
                       _m_psrawi(
                         _m_pmullw(
                           _m_psubw(
                             _m_psrlwi(
                               _m_pmullw(v12, _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v10 + v23)), 0LL)),
                               8u),
                             v12),
                           i),
                         7u),
                       v12),
                     0LL));
          ++v10;
          --v11;
        }
        while ( v11 );
        v6 = v22.m64_i32[1];
      }
      result = v24;
      v5 = (__m128i *)((char *)v5 + (_DWORD)v25);
    }
  }
  else
  {
    v21 = 0LL;
    result = (unsigned __int16)result;
    v13 = v22.m64_i32[1];
    v14 = v7 >> 1;
    v15 = _mm_unpacklo_epi16(
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
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    if ( v22.m64_i32[1] )
    {
      v16 = _mm_load_si128(&v21);
      do
      {
        v22.m64_i32[1] = --v13;
        v17 = v5;
        v18 = v14;
        if ( v14 )
        {
          v19 = v26 - (_DWORD)v5;
          do
          {
            v20 = _mm_unpacklo_epi8(_mm_loadl_epi64(v17), v16);
            v17->m128i_i64[0] = _mm_packus_epi16(
                                  _mm_add_epi16(
                                    _mm_srai_epi16(
                                      _mm_mullo_epi16(
                                        _mm_sub_epi16(
                                          _mm_srli_epi16(
                                            _mm_mullo_epi16(
                                              v20,
                                              _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i *)((char *)v17 + v19)), v16)),
                                            8u),
                                          v20),
                                        v15),
                                      7u),
                                    v20),
                                  v16).m128i_u64[0];
            v17 = (__m128i *)((char *)v17 + 8);
            --v18;
          }
          while ( v18 );
          v13 = v22.m64_i32[1];
        }
        result = v24;
        v5 = (__m128i *)((char *)v5 + (_DWORD)v25);
        v26 += v24;
      }
      while ( v13 );
    }
  }
  _m_empty();
  return result;
}

// ===== sub_40D820 @ 0x0040D820..0x0040DA36 =====
unsigned int *__cdecl sub_40D820(unsigned int **a1, int a2)
{
  int *v2; // ecx
  unsigned int *v3; // edi
  int v4; // ecx
  unsigned int v5; // edx
  unsigned int v6; // eax
  unsigned int *result; // eax
  unsigned int *i; // ecx
  unsigned int *v9; // edx
  int v10; // esi
  unsigned int v11; // ecx
  unsigned int v12; // edi
  __m64 v13; // mm1
  __m64 v14; // mm1
  unsigned int *v15; // edx
  unsigned int *v16; // ecx
  int v17; // esi
  __m64 v18; // mm1
  __m64 v19; // mm1
  __m64 v20; // [esp+8h] [ebp-428h]
  unsigned int *v21; // [esp+10h] [ebp-420h]
  int v22; // [esp+14h] [ebp-41Ch]
  unsigned int *v23; // [esp+18h] [ebp-418h]
  unsigned int *v24; // [esp+1Ch] [ebp-414h]
  unsigned int *v25; // [esp+20h] [ebp-410h]
  int v26; // [esp+20h] [ebp-410h]
  int v27; // [esp+24h] [ebp-40Ch]
  __m64 v28[128]; // [esp+28h] [ebp-408h]

  v27 = *v2;
  v22 = v2[1];
  v21 = a1[1];
  v3 = *a1;
  v23 = a1[2];
  v25 = a1[3];
  v4 = 0;
  v24 = *a1;
  v5 = 0;
  do
  {
    v6 = v5;
    v5 += a2;
    v6 >>= 8;
    v20.m64_i32[1] = (unsigned __int16)v6;
    v20.m64_i16[1] = v6;
    v20.m64_i16[0] = v6;
    v28[v4++] = v20;
  }
  while ( v4 < 128 );
  result = (unsigned int *)sub_407AE0();
  if ( result )
  {
    for ( result = v25; result; v3 = (unsigned int *)((char *)v3 + (_DWORD)v21) )
    {
      v15 = v23;
      result = (unsigned int *)((char *)result - 1);
      v16 = v3;
      if ( v23 )
      {
        v17 = v27 - (_DWORD)v3;
        do
        {
          v15 = (unsigned int *)((char *)v15 - 1);
          if ( (*v16 & 0xFE000000) != 0 )
          {
            v18 = _mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + v17));
            v17 = v27 - (_DWORD)v3;
            v19 = _m_punpcklbw(v18, 0LL);
            *(unsigned int *)((char *)v16 + v27 - (_DWORD)v3) = _mm_cvtsi64_si32(
                                                                  _m_packuswb(
                                                                    _m_paddw(
                                                                      _m_psrawi(
                                                                        _m_pmullw(
                                                                          _m_psubw(
                                                                            _m_pmulhuw(
                                                                              v19,
                                                                              _m_psllwi(
                                                                                _m_punpcklbw(
                                                                                  _mm_cvtsi32_si64(*v16),
                                                                                  0LL),
                                                                                8u)),
                                                                            v19),
                                                                          v28[*v16 >> 25]),
                                                                        7u),
                                                                      v19),
                                                                    0LL));
          }
          ++v16;
        }
        while ( v15 );
      }
      v27 += v22;
    }
  }
  else
  {
    for ( i = v25; i; v24 = v3 )
    {
      v9 = v23;
      i = (unsigned int *)((char *)i - 1);
      v26 = (int)i;
      result = v3;
      if ( v23 )
      {
        v10 = v27 - (_DWORD)v3;
        do
        {
          v11 = *result;
          _mm_prefetch((const char *)result + 256, 0);
          v9 = (unsigned int *)((char *)v9 - 1);
          if ( (v11 & 0xFE000000) != 0 )
          {
            v12 = *(unsigned int *)((char *)result + v10);
            _mm_prefetch((const char *)result + v10 + 128, 0);
            v13 = _mm_cvtsi32_si64(v12);
            v3 = v24;
            v14 = _m_punpcklbw(v13, 0LL);
            *(unsigned int *)((char *)result + v10) = _mm_cvtsi64_si32(
                                                        _m_packuswb(
                                                          _m_paddw(
                                                            _m_psrawi(
                                                              _m_pmullw(
                                                                _m_psubw(
                                                                  _m_pmulhuw(
                                                                    v14,
                                                                    _m_psllwi(
                                                                      _m_punpcklbw(_mm_cvtsi32_si64(v11), 0LL),
                                                                      8u)),
                                                                  v14),
                                                                v28[v11 >> 25]),
                                                              7u),
                                                            v14),
                                                          0LL));
          }
          ++result;
        }
        while ( v9 );
        i = (unsigned int *)v26;
      }
      v3 = (unsigned int *)((char *)v3 + (_DWORD)v21);
      v27 += v22;
    }
  }
  _m_empty();
  return result;
}

// ===== sub_40DA40 @ 0x0040DA40..0x0040DACA =====
_BYTE *__fastcall sub_40DA40(int *a1, int a2, __int16 a3)
{
  _BYTE *result; // eax
  int v4; // edi
  int v5; // edx
  int v6; // ecx
  int v7; // edi
  int v8; // [esp+4h] [ebp-18h]
  int v9; // [esp+8h] [ebp-14h]
  int i; // [esp+10h] [ebp-Ch]
  int v11; // [esp+14h] [ebp-8h]
  _BYTE *v12; // [esp+18h] [ebp-4h]

  result = *(_BYTE **)a2;
  v9 = *(_DWORD *)(a2 + 4);
  v4 = *a1;
  v8 = a1[1];
  v5 = a1[2];
  v6 = a1[3];
  v12 = result;
  v11 = v4;
  for ( i = v5; v6; v11 += v8 )
  {
    --v6;
    if ( v5 )
    {
      v7 = v4 - (_DWORD)result;
      do
      {
        *result += (unsigned __int16)(a3
                                    * (((257 * ((unsigned __int8)*result * (unsigned int)(unsigned __int8)result[v7] + 1)) >> 16)
                                     - (unsigned __int8)*result)) >> 8;
        ++result;
        --v5;
      }
      while ( v5 );
      result = v12;
      v5 = i;
    }
    result += v9;
    v4 = v8 + v11;
    v12 = result;
  }
  return result;
}

// ===== sub_40DAD0 @ 0x0040DAD0..0x0040DB54 =====
int __usercall sub_40DAD0@<eax>(int result@<eax>, unsigned int a2@<ecx>, size_t *a3)
{
  int v3; // esi
  int v4; // esi

  if ( !a2 )
    return sub_40AF50(result, a3);
  v3 = *(_DWORD *)(result + 16);
  if ( v3 )
  {
    v4 = v3 - 1;
    if ( v4 )
    {
      if ( v4 == 1 )
      {
        if ( a3[4] == 1 )
        {
          return sub_40DCE0(result, a2);
        }
        else if ( a3[4] == 2 )
        {
          return sub_40DE60(a3, result, a2);
        }
      }
    }
    else if ( a3[4] == 1 )
    {
      if ( a2 >= 0x100 )
        return sub_40A620((int)a3, 0);
      else
        return sub_40DB60(a2);
    }
  }
  else if ( !a3[4] )
  {
    return sub_40E1F0(a3, result);
  }
  return result;
}

// ===== sub_40DB60 @ 0x0040DB60..0x0040DCDC =====
int __usercall sub_40DB60@<eax>(int *a1@<eax>, _DWORD *a2@<ecx>, int a3@<ebp>, int a4)
{
  _DWORD *v4; // edi
  int v5; // ecx
  int v6; // edx
  unsigned int v7; // ecx
  __int32 v8; // edx
  int result; // eax
  __m64 i; // mm1
  _DWORD *v11; // eax
  unsigned int v12; // esi
  int v13; // edx
  __int32 v14; // edx
  unsigned int v15; // ecx
  __m128i v16; // xmm0
  __m128i v17; // xmm1
  _QWORD *v18; // eax
  unsigned int v19; // esi
  int v20; // edx
  __m128i v21; // [esp-30h] [ebp-3Ch] BYREF
  __m64 v22; // [esp-18h] [ebp-24h]
  int j; // [esp-10h] [ebp-1Ch]
  int v24; // [esp-Ch] [ebp-18h]
  int v25; // [esp-8h] [ebp-14h]
  int v26; // [esp-4h] [ebp-10h]
  int v27; // [esp+0h] [ebp-Ch]
  void *v28; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v27 = a3;
  v28 = retaddr;
  v4 = (_DWORD *)*a2;
  v5 = a2[1];
  v26 = *a1;
  v6 = a1[1];
  v25 = v5;
  v7 = a1[2];
  v24 = v6;
  v8 = a1[3];
  result = 256 - a4;
  v22.m64_i32[1] = v8;
  if ( (v7 & 1) != 0 )
  {
    v22.m64_i32[1] = (unsigned __int16)result;
    v22.m64_i16[1] = 256 - a4;
    v22.m64_i16[0] = 256 - a4;
    for ( i = v22; v8; v26 += v24 )
    {
      v22.m64_i32[1] = --v8;
      v11 = v4;
      v12 = v7;
      if ( v7 )
      {
        v13 = v26 - (_DWORD)v4;
        for ( j = v26 - (_DWORD)v4; ; v13 = j )
        {
          *v11 = _mm_cvtsi64_si32(
                   _m_packuswb(
                     _m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)((char *)v11 + v13)), 0LL), i), 8u),
                     0LL));
          ++v11;
          if ( !--v12 )
            break;
        }
        v8 = v22.m64_i32[1];
      }
      result = v24;
      v4 = (_DWORD *)((char *)v4 + v25);
    }
  }
  else
  {
    v21 = 0LL;
    result = (unsigned __int16)result;
    v14 = v22.m64_i32[1];
    v15 = v7 >> 1;
    v16 = _mm_unpacklo_epi16(
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
              _mm_unpacklo_epi16(_mm_cvtsi32_si128(0), _mm_cvtsi32_si128(0))));
    if ( v22.m64_i32[1] )
    {
      v17 = _mm_load_si128(&v21);
      do
      {
        v22.m64_i32[1] = --v14;
        v18 = v4;
        v19 = v15;
        if ( v15 )
        {
          v20 = v26 - (_DWORD)v4;
          do
          {
            *v18 = _mm_packus_epi16(
                     _mm_srli_epi16(
                       _mm_mullo_epi16(
                         _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)((char *)v18 + v20)), v17),
                         v16),
                       8u),
                     v17).m128i_u64[0];
            ++v18;
            --v19;
          }
          while ( v19 );
          v14 = v22.m64_i32[1];
        }
        result = v24;
        v4 = (_DWORD *)((char *)v4 + v25);
        v26 += v24;
      }
      while ( v14 );
    }
  }
  _m_empty();
  return result;
}

// ===== sub_40DCE0 @ 0x0040DCE0..0x0040DE54 =====
unsigned int *__cdecl sub_40DCE0(unsigned int **a1, __int16 a2)
{
  int *v2; // ecx
  unsigned int *v3; // edi
  unsigned int *v4; // edx
  int i; // eax
  unsigned int *result; // eax
  unsigned int *v7; // edx
  unsigned int *v8; // ecx
  int v9; // esi
  __m64 v10; // mm3
  unsigned int *v11; // [esp+Ch] [ebp-424h]
  int v12; // [esp+10h] [ebp-420h]
  unsigned int *v13; // [esp+14h] [ebp-41Ch]
  unsigned int *v14; // [esp+18h] [ebp-418h]
  int v15; // [esp+1Ch] [ebp-414h]
  __m64 v16; // [esp+20h] [ebp-410h]
  __m64 v17; // [esp+20h] [ebp-410h]
  __m64 v18[128]; // [esp+28h] [ebp-408h]

  v15 = *v2;
  v3 = *a1;
  v11 = a1[1];
  v4 = a1[3];
  v14 = a1[2];
  v13 = *a1;
  v12 = v2[1];
  for ( i = 0; i < 128; ++i )
  {
    v16.m64_i32[1] = (unsigned __int16)i;
    v16.m64_i16[1] = i;
    v16.m64_i16[0] = i;
    v18[i] = v16;
  }
  result = v4;
  if ( v4 )
  {
    v7 = v14;
    do
    {
      result = (unsigned int *)((char *)result - 1);
      v8 = v3;
      if ( v7 )
      {
        v9 = v15 - (_DWORD)v3;
        do
        {
          v7 = (unsigned int *)((char *)v7 - 1);
          if ( (*v8 & 0xFE000000) != 0 )
          {
            v10 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v8 + v9)), 0LL);
            v17.m64_i16[0] = 256 - a2;
            v17.m64_i16[1] = 256 - a2;
            v17.m64_i32[1] = (unsigned __int16)(256 - a2);
            *(unsigned int *)((char *)v8 + v9) = _mm_cvtsi64_si32(
                                                   _m_packuswb(
                                                     _m_paddw(
                                                       v10,
                                                       _m_psrawi(
                                                         _m_pmullw(
                                                           _m_psubw(
                                                             _m_psrlwi(
                                                               _m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v8), 0LL), v17),
                                                               8u),
                                                             v10),
                                                           v18[*v8 >> 25]),
                                                         7u)),
                                                     0LL));
          }
          ++v8;
        }
        while ( v7 );
        v7 = v14;
        v3 = v13;
      }
      v3 = (unsigned int *)((char *)v3 + (_DWORD)v11);
      v15 += v12;
      v13 = v3;
    }
    while ( result );
  }
  _m_empty();
  return result;
}

// ===== sub_40DE60 @ 0x0040DE60..0x0040DF71 =====
unsigned __int8 *__cdecl sub_40DE60(unsigned __int8 **a1, unsigned __int8 **a2, int a3)
{
  unsigned __int8 *v3; // ecx
  _DWORD *v4; // esi
  unsigned __int8 *result; // eax
  int v6; // edx
  unsigned __int8 *v7; // edi
  int v8; // eax
  int v9; // eax
  int v10; // ebx
  unsigned int v11; // esi
  int v12; // eax
  int v13; // ebx
  int v14; // edx
  int v15; // ebx
  unsigned __int8 *v16; // [esp+8h] [ebp-18h]
  int v17; // [esp+Ch] [ebp-14h]
  unsigned __int8 *v18; // [esp+14h] [ebp-Ch]
  unsigned __int8 *v19; // [esp+18h] [ebp-8h]
  unsigned int v20; // [esp+1Ch] [ebp-4h]
  int v21; // [esp+30h] [ebp+10h]

  v3 = *a1;
  v4 = a2;
  result = a2[3];
  v6 = 256 - a3;
  v7 = *a2;
  v21 = 256 - a3;
  v19 = *a1;
  v18 = *a2;
  if ( result )
  {
    do
    {
      v16 = result - 1;
      v8 = v4[2];
      if ( v8 )
      {
        do
        {
          v17 = --v8;
          if ( (*(_DWORD *)v7 & 0xFF000000) != 0 )
          {
            v9 = v7[3];
            v10 = v3[3] * (256 - v9);
            v11 = v10 + (v9 << 8);
            v20 = ((v6 * v9) << 16) / v11;
            v12 = (v10 << 16) / v11;
            v13 = v3[1];
            *v3 = (v20 * *v7 + v12 * *v3) >> 16;
            v14 = v12 * v13 + v20 * v7[1];
            v15 = v12 * v3[2];
            v3[1] = BYTE2(v14);
            BYTE1(v12) = BYTE1(v11);
            v4 = a2;
            v3[2] = (v15 + v20 * v7[2]) >> 16;
            v6 = v21;
            v3[3] = BYTE1(v12);
            v8 = v17;
          }
          v3 = &v3[(_DWORD)a1[5]];
          v7 += v4[5];
        }
        while ( v8 );
        v3 = v19;
      }
      v3 = &v3[(_DWORD)a1[1]];
      v7 = &v18[v4[1]];
      result = v16;
      v19 = v3;
      v18 = v7;
    }
    while ( v16 );
  }
  return result;
}

// ===== sub_40DF80 @ 0x0040DF80..0x0040DFED =====
int __fastcall sub_40DF80(int a1, int a2, int a3)
{
  int v3; // eax
  int v4; // eax
  int result; // eax

  v3 = *(_DWORD *)(a3 + 16);
  if ( !v3 )
    return sub_40DFF0(a1, a3);
  v4 = v3 - 1;
  if ( v4 )
  {
    result = v4 - 1;
    if ( !result )
    {
      if ( *(_DWORD *)(a1 + 16) == 1 )
      {
        return sub_40E0D0(a1, a2);
      }
      else
      {
        result = *(_DWORD *)(a1 + 16) - 2;
        if ( *(_DWORD *)(a1 + 16) == 2 )
          return sub_40E0D0(a1, a2);
      }
    }
  }
  else
  {
    if ( *(_DWORD *)(a1 + 16) == 1 )
      return sub_40E050(a1);
    result = *(_DWORD *)(a1 + 16) - 2;
    if ( *(_DWORD *)(a1 + 16) == 2 )
      return sub_40E050(a1);
  }
  return result;
}

// ===== sub_40DFF0 @ 0x0040DFF0..0x0040E04E =====
_DWORD *__cdecl sub_40DFF0(_DWORD *a1, _DWORD *a2)
{
  _DWORD *result; // eax
  _DWORD *v3; // ebx
  int v4; // ecx
  int v5; // esi
  int i; // edi
  _WORD *v7; // ecx
  int v8; // [esp+Ch] [ebp-4h]

  result = a1;
  v3 = a2;
  v4 = a2[3];
  v5 = *a1;
  for ( i = *a2; v4; i += v3[1] )
  {
    result = (_DWORD *)v3[2];
    v8 = --v4;
    if ( result )
    {
      v7 = (_WORD *)(v5 + 2 * (_DWORD)result);
      do
      {
        --v7;
        result = (_DWORD *)((char *)result - 1);
        if ( *(_WORD *)((char *)v7 + i - v5) )
        {
          *v7 = 0;
          v3 = a2;
        }
      }
      while ( result );
      v4 = v8;
    }
    v5 += a1[1];
  }
  return result;
}

// ===== sub_40E050 @ 0x0040E050..0x0040E0C6 =====
unsigned __int8 *__usercall sub_40E050@<eax>(unsigned __int8 **a1@<esi>, int a2)
{
  unsigned __int8 *result; // eax
  int v3; // ebx
  _DWORD *v4; // ecx
  unsigned __int8 *v5; // edi
  unsigned __int8 *v6; // edx
  unsigned __int8 *i; // [esp+Ch] [ebp-8h]
  _DWORD *v8; // [esp+10h] [ebp-4h]

  result = *a1;
  v3 = a2;
  v4 = *(_DWORD **)a2;
  v5 = a1[3];
  v8 = *(_DWORD **)a2;
  for ( i = *a1; v5; i = result )
  {
    v6 = a1[2];
    --v5;
    if ( v6 )
    {
      do
      {
        --v6;
        if ( *result + result[1] + result[2] > 0 )
          *v4 = 0;
        v3 = a2;
        v4 = (_DWORD *)((char *)v4 + *(_DWORD *)(a2 + 20));
        result = &result[(_DWORD)a1[5]];
      }
      while ( v6 );
      result = i;
      v4 = v8;
    }
    v4 = (_DWORD *)((char *)v4 + *(_DWORD *)(v3 + 4));
    result = &result[(_DWORD)a1[1]];
    v8 = v4;
  }
  return result;
}

// ===== sub_40E0D0 @ 0x0040E0D0..0x0040E14D =====
int __usercall sub_40E0D0@<eax>(int *a1@<edi>, _DWORD *a2, int a3)
{
  int v3; // ecx
  _DWORD *v4; // ebx
  int result; // eax
  int v6; // edx
  _DWORD *v7; // esi
  unsigned int v8; // ecx
  int v9; // [esp+4h] [ebp-8h]
  int v10; // [esp+8h] [ebp-4h]

  v3 = a1[3];
  v4 = (_DWORD *)*a2;
  result = *a1;
  v10 = *a1;
  if ( v3 )
  {
    do
    {
      v6 = a1[2];
      v9 = v3 - 1;
      v7 = v4;
      if ( v6 )
      {
        do
        {
          --v6;
          if ( a3 )
            v8 = *(_DWORD *)result & 0xFF000000;
          else
            v8 = *(_BYTE *)(result + 3) == 0xFF;
          if ( v8 )
            *v7 = 0;
          v7 = (_DWORD *)((char *)v7 + a2[5]);
          result += a1[5];
        }
        while ( v6 );
        result = v10;
      }
      result += a1[1];
      v3 = v9;
      v4 = (_DWORD *)((char *)v4 + a2[1]);
      v10 = result;
    }
    while ( v9 );
  }
  return result;
}

// ===== sub_40E150 @ 0x0040E150..0x0040E187 =====
int __usercall sub_40E150@<eax>(int a1@<esi>, int a2)
{
  int *v2; // eax
  int result; // eax
  _DWORD v4[4]; // [esp+0h] [ebp-24h] BYREF
  int v5[5]; // [esp+10h] [ebp-14h] BYREF

  sub_409190(v5, a1);
  v2 = sub_409190(v4, a2);
  result = sub_409110(v5, v2);
  if ( result )
    return sub_40A620(a1, v5);
  return result;
}

// ===== sub_40E190 @ 0x0040E190..0x0040E1EF =====
int __usercall sub_40E190@<eax>(int result@<eax>, int a2@<edx>, int a3@<esi>, _DWORD *a4)
{
  int v4; // edi
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  v4 = *(_DWORD *)(a3 + 16);
  if ( v4 )
  {
    if ( v4 == 1 && a4[4] == 1 )
    {
      if ( a2 )
      {
        if ( (*(_BYTE *)(a3 + 8) & 1) != 0 )
          return sub_40E2E0(a4, a3);
        else
          return sub_40E3E0(a4, a3);
      }
      else
      {
        return sub_40DB60((int *)a3, a4, (int)&savedregs, result);
      }
    }
  }
  else if ( !a4[4] )
  {
    return sub_40E1F0(a4, a3);
  }
  return result;
}

// ===== sub_40E1F0 @ 0x0040E1F0..0x0040E2DE =====
int __usercall sub_40E1F0@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int *a3, _DWORD *a4)
{
  int v4; // ebx
  int v5; // edi
  _DWORD *v6; // edx
  int result; // eax
  int v8; // ecx
  int v9; // esi
  int v10; // edx
  __int16 v11; // ax
  unsigned int v12; // ebx
  int v13; // [esp+Ch] [ebp-1Ch]
  int v14; // [esp+10h] [ebp-18h]
  int v15; // [esp+14h] [ebp-14h]
  int v16; // [esp+18h] [ebp-10h]
  int v17; // [esp+1Ch] [ebp-Ch]
  int i; // [esp+20h] [ebp-8h]
  int v19; // [esp+24h] [ebp-4h]

  v4 = *a4;
  v19 = *a3;
  v5 = a2 * ((a1 >> 6) & 0x3E0);
  v16 = a2 * ((a1 >> 9) & 0x7C00);
  v6 = a4;
  v15 = a2 * ((unsigned __int8)a1 >> 3);
  result = a4[3];
  v8 = 256 - a2;
  v17 = *a4;
  for ( i = v5; result; v17 = v4 )
  {
    v9 = v6[2];
    v13 = --result;
    if ( v9 )
    {
      v10 = v4 + 2 * v9;
      v14 = v19 - v4;
      do
      {
        v11 = *(_WORD *)(v10 - 2);
        v10 -= 2;
        v12 = (((v16 + v8 * (v11 & 0x7C00u)) >> 8) & 0x7C00) + (((v5 + v8 * (v11 & 0x3E0u)) >> 8) & 0x3E0);
        v5 = i;
        --v9;
        *(_WORD *)(v14 + v10) = ((v15 + v8 * (v11 & 0x1Fu)) >> 8) + v12;
      }
      while ( v9 );
      result = v13;
      v4 = v17;
    }
    v19 += a3[1];
    v6 = a4;
    v4 += a4[1];
  }
  return result;
}

// ===== sub_40E2E0 @ 0x0040E2E0..0x0040E3D1 =====
int __usercall sub_40E2E0@<eax>(int a1@<eax>, int a2@<edx>, int *a3, int *a4)
{
  int v5; // edi
  int v6; // edi
  __int64 v7; // rdi
  __m64 v8; // mm5
  __int64 v9; // mm6
  int v10; // edx
  int v11; // edi
  int v12; // esi
  int v13; // ecx
  int result; // eax
  int v15; // [esp+28h] [ebp-Ch]
  int v16; // [esp+2Ch] [ebp-8h]
  int v17; // [esp+30h] [ebp-4h]
  int v18; // [esp+3Ch] [ebp+8h]
  int v19; // [esp+40h] [ebp+Ch]

  v5 = *a3;
  v18 = a3[1];
  v15 = v5;
  v6 = *a4;
  v19 = a4[1];
  v17 = a4[2];
  v16 = v6;
  HIDWORD(v7) = (((a1 * (a2 & 0xFF0000u)) >> 8) & 0xFF0000uLL) >> 24;
  LODWORD(v7) = ((a1 * (a2 & 0xFF00u)) >> 8) & 0xFF00;
  v8.m64_u64 = ((a1 * (unsigned int)(unsigned __int8)a2) >> 8)
             + ((v7 + ((((a1 * (a2 & 0xFF0000u)) >> 8) & 0xFF0000) << 8)) << 8);
  v9 = 0x100010001LL * (unsigned int)(256 - a1);
  v10 = a4[3];
  v11 = v15;
  v12 = v16;
  do
  {
    v13 = v17;
    result = 0;
    do
    {
      *(_DWORD *)(v11 + result) = _mm_cvtsi64_si32(
                                    _m_packuswb(
                                      _m_paddw(
                                        _m_psrlwi(
                                          _m_pmullw(
                                            _m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v12 + result)), 0LL),
                                            (__m64)v9),
                                          8u),
                                        v8),
                                      0LL));
      result += 4;
      --v13;
    }
    while ( v13 );
    v11 += v18;
    v12 += v19;
    --v10;
  }
  while ( v10 );
  _m_empty();
  return result;
}

// ===== sub_40E3E0 @ 0x0040E3E0..0x0040E4E3 =====
int __usercall sub_40E3E0@<eax>(int a1@<eax>, int a2@<edx>, int *a3, int *a4)
{
  int v5; // edi
  int v6; // edi
  __int64 v7; // rdi
  __m64 v8; // mm5
  __int64 v9; // mm6
  int v10; // edx
  int v11; // edi
  int v12; // esi
  unsigned int v13; // ecx
  int result; // eax
  int v15; // [esp+28h] [ebp-Ch]
  int v16; // [esp+2Ch] [ebp-8h]
  unsigned int v17; // [esp+30h] [ebp-4h]
  int v18; // [esp+3Ch] [ebp+8h]
  int v19; // [esp+40h] [ebp+Ch]

  v5 = *a3;
  v18 = a3[1];
  v15 = v5;
  v6 = *a4;
  v19 = a4[1];
  v17 = (unsigned int)a4[2] >> 1;
  v16 = v6;
  HIDWORD(v7) = (((a1 * (a2 & 0xFF0000u)) >> 8) & 0xFF0000uLL) >> 24;
  LODWORD(v7) = ((a1 * (a2 & 0xFF00u)) >> 8) & 0xFF00;
  v8.m64_u64 = ((a1 * (unsigned int)(unsigned __int8)a2) >> 8)
             + ((v7 + ((((a1 * (a2 & 0xFF0000u)) >> 8) & 0xFF0000) << 8)) << 8);
  v9 = 0x100010001LL * (unsigned int)(256 - a1);
  v10 = a4[3];
  v11 = v15;
  v12 = v16;
  do
  {
    v13 = v17;
    result = 0;
    do
    {
      *(__m64 *)(v11 + result) = _m_packuswb(
                                   _m_paddw(
                                     _m_psrlwi(_m_pmullw(_m_punpcklbw(*(__m64 *)(v12 + result), 0LL), (__m64)v9), 8u),
                                     v8),
                                   _m_paddw(
                                     _m_psrlwi(_m_pmullw(_m_punpckhbw(*(__m64 *)(v12 + result), 0LL), (__m64)v9), 8u),
                                     v8));
      result += 8;
      --v13;
    }
    while ( v13 );
    v11 += v18;
    v12 += v19;
    --v10;
  }
  while ( v10 );
  _m_empty();
  return result;
}

// ===== sub_40E4F0 @ 0x0040E4F0..0x0040E512 =====
int __fastcall sub_40E4F0(int a1, unsigned int a2, unsigned int a3, int a4, int a5)
{
  int result; // eax

  result = *(_DWORD *)(a1 + 16) - 1;
  if ( *(_DWORD *)(a1 + 16) == 1 )
    return sub_40E520(a1, __SPAIR64__(a2, a3), a5);
  return result;
}

// ===== sub_40E520 @ 0x0040E520..0x0040E680 =====
int __cdecl sub_40E520(int *a1, __m64 a2, int a3)
{
  int *v3; // ecx
  int i; // esi
  __int64 v5; // rax
  __int64 v6; // rax
  unsigned __int64 v7; // kr00_8
  int v8; // ebx
  int v9; // edi
  int v10; // esi
  int v11; // ebx
  int result; // eax
  __m64 v13; // mm0
  int v14; // edx
  char v15; // cc
  unsigned int v16; // edx
  int v17; // [esp-4h] [ebp-440h]
  __m64 v18; // [esp+Ch] [ebp-430h]
  int v19; // [esp+18h] [ebp-424h]
  int v20; // [esp+1Ch] [ebp-420h]
  int v21; // [esp+20h] [ebp-41Ch]
  int v22; // [esp+24h] [ebp-418h]
  int v23; // [esp+28h] [ebp-414h]
  int v24; // [esp+2Ch] [ebp-410h]
  int v25; // [esp+30h] [ebp-40Ch]
  __m64 v26[128]; // [esp+34h] [ebp-408h]

  v24 = *a1;
  v25 = *v3;
  v20 = a1[1];
  v21 = v3[1];
  v22 = a1[3];
  v19 = a1[2];
  for ( i = 0; i < 128; ++i )
  {
    v5 = 0x100010001LL * i;
    v26[i].m64_i32[0] = v5;
    v26[i].m64_i32[1] = HIDWORD(v5);
  }
  HIDWORD(v6) = (a2.m64_u32[0] & 0xFF0000uLL) >> 24;
  LODWORD(v6) = a2.m64_i16[0] & 0xFF00;
  v7 = v6 + ((a2.m64_i32[0] & 0xFF0000u) << 8);
  HIDWORD(v6) = v7 >> 24;
  LODWORD(v6) = a2.m64_u8[0];
  v18.m64_u64 = v6 + (unsigned int)((_DWORD)v7 << 8);
  v23 = 256 - a3 * ((1 << a2.m64_i8[4]) + 1);
  v8 = v22;
  v9 = v24;
  v10 = v25;
  do
  {
    v17 = v8;
    v11 = v19;
    result = 0;
    do
    {
      v13 = a2;
      v14 = *(unsigned __int8 *)(v10 + result) << a2.m64_i8[4];
      v15 = (v23 + v14 < 0) ^ __OFADD__(v23, v14) | (v23 + v14 == 0);
      v16 = v23 + v14;
      if ( !v15 )
      {
        if ( v16 >= 0x100 )
          goto LABEL_9;
        v13 = _m_packuswb(
                _m_paddw(
                  v18,
                  _m_psrawi(
                    _m_pmullw(
                      _m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*(_DWORD *)(v9 + 4 * result)), 0LL), v18),
                      v26[v16 >> 1]),
                    7u)),
                0LL);
      }
      *(_DWORD *)(v9 + 4 * result) = _mm_cvtsi64_si32(v13);
LABEL_9:
      ++result;
      --v11;
    }
    while ( v11 );
    v9 += v20;
    v10 += v21;
    v8 = v17 - 1;
  }
  while ( v17 != 1 );
  _m_empty();
  return result;
}

// ===== sub_40E680 @ 0x0040E680..0x0040E6B9 =====
int __fastcall sub_40E680(int a1, int a2)
{
  if ( *(_DWORD *)(a2 + 16) != 3 )
    return 10;
  if ( *(_DWORD *)(a1 + 16) == 1 )
  {
    sub_40E6C0(a2, a1);
    return 0;
  }
  else if ( *(_DWORD *)(a1 + 16) == 2 )
  {
    sub_40E760(a2, a1);
    return 0;
  }
  else
  {
    return 9;
  }
}

// ===== sub_40E6C0 @ 0x0040E6C0..0x0040E756 =====
unsigned __int8 *__cdecl sub_40E6C0(int a1, unsigned __int8 **a2)
{
  unsigned __int8 **v2; // edx
  unsigned __int8 *result; // eax
  int v4; // ebx
  _BYTE *v5; // esi
  unsigned __int8 *v6; // edi
  unsigned __int8 *v7; // ecx
  int v8; // [esp+Ch] [ebp-14h]
  int v9; // [esp+14h] [ebp-Ch]
  unsigned __int8 *v10; // [esp+18h] [ebp-8h]
  _BYTE *v11; // [esp+1Ch] [ebp-4h]

  v2 = a2;
  result = *a2;
  v4 = a1;
  v5 = *(_BYTE **)a1;
  v6 = a2[3];
  v11 = *(_BYTE **)a1;
  v10 = *a2;
  if ( v6 )
  {
    v8 = *(_DWORD *)(a1 + 4);
    do
    {
      v7 = v2[2];
      --v6;
      if ( v7 )
      {
        v9 = *(_DWORD *)(v4 + 20);
        do
        {
          *v5 = (unsigned __int16)(77 * result[2] + 151 * result[1] + 28 * *result) >> 8;
          v2 = a2;
          v5 += v9;
          result = &result[(_DWORD)a2[5]];
          --v7;
        }
        while ( v7 );
        v4 = a1;
        result = v10;
      }
      v5 = &v11[v8];
      result = &result[(_DWORD)v2[1]];
      v11 += v8;
      v10 = result;
    }
    while ( v6 );
  }
  return result;
}

// ===== sub_40E760 @ 0x0040E760..0x0040E7FD =====
unsigned __int8 *__cdecl sub_40E760(int a1, int a2)
{
  int v2; // edx
  unsigned __int8 *result; // eax
  int v4; // ebx
  _BYTE *v5; // esi
  int v6; // edi
  int v7; // ecx
  int v8; // [esp+Ch] [ebp-14h]
  int v9; // [esp+14h] [ebp-Ch]
  unsigned __int8 *v10; // [esp+18h] [ebp-8h]
  _BYTE *v11; // [esp+1Ch] [ebp-4h]

  v2 = a2;
  result = *(unsigned __int8 **)a2;
  v4 = a1;
  v5 = *(_BYTE **)a1;
  v6 = *(_DWORD *)(a2 + 12);
  v11 = *(_BYTE **)a1;
  v10 = *(unsigned __int8 **)a2;
  if ( v6 )
  {
    v8 = *(_DWORD *)(a1 + 4);
    do
    {
      v7 = *(_DWORD *)(v2 + 8);
      --v6;
      if ( v7 )
      {
        v9 = *(_DWORD *)(v4 + 20);
        do
        {
          *v5 = (result[3] * (77 * result[2] + 151 * result[1] + 28 * (unsigned int)*result)) >> 16;
          v2 = a2;
          v5 += v9;
          result += *(_DWORD *)(a2 + 20);
          --v7;
        }
        while ( v7 );
        v4 = a1;
        result = v10;
      }
      v5 = &v11[v8];
      result += *(_DWORD *)(v2 + 4);
      v11 += v8;
      v10 = result;
    }
    while ( v6 );
  }
  return result;
}

// ===== sub_40E800 @ 0x0040E800..0x0040E85F =====
BOOL __cdecl sub_40E800(_DWORD *a1)
{
  _DWORD *v1; // esi
  BOOL result; // eax
  _BYTE *v3; // ebx
  int v4; // edi
  int v5; // edx
  _BYTE *v6; // ecx
  int v7; // esi
  int v8; // [esp+4h] [ebp-Ch]
  BOOL v9; // [esp+8h] [ebp-8h]
  int v10; // [esp+Ch] [ebp-4h]

  v1 = a1;
  result = a1[4] == 3;
  v9 = result;
  if ( a1[4] == 3 )
  {
    v3 = (_BYTE *)*a1;
    v4 = a1[3];
    if ( v4 )
    {
      v10 = a1[2];
      v8 = a1[1];
      do
      {
        v5 = v10;
        --v4;
        v6 = v3;
        if ( v10 )
        {
          v7 = v1[5];
          do
          {
            *v6 = -1 - *v6;
            v6 += v7;
            --v5;
          }
          while ( v5 );
          v1 = a1;
          result = v9;
        }
        v3 += v8;
      }
      while ( v4 );
    }
  }
  return result;
}

// ===== sub_40E860 @ 0x0040E860..0x0040E881 =====
int __usercall sub_40E860@<eax>(int result@<eax>, int a2@<ecx>, int a3@<esi>)
{
  if ( *(_DWORD *)(result + 16) == 3 )
  {
    if ( *(_DWORD *)(a2 + 16) == 1 )
    {
      return sub_40E890(a3);
    }
    else if ( *(_DWORD *)(a2 + 16) == 2 )
    {
      return sub_40E920(a3);
    }
  }
  return result;
}

// ===== sub_40E890 @ 0x0040E890..0x0040E91F =====
int __usercall sub_40E890@<eax>(int a1@<eax>, unsigned int **a2@<ecx>, unsigned int a3)
{
  unsigned int *v3; // edx
  __m64 v4; // mm6
  int v5; // ecx
  unsigned int *v6; // edi
  _DWORD *v7; // esi
  int v8; // edx
  int result; // eax
  __m64 v10; // mm1
  unsigned int *v11; // [esp-8h] [ebp-30h]
  _DWORD *v12; // [esp-4h] [ebp-2Ch]
  int v13; // [esp+1Ch] [ebp-Ch]
  unsigned int *v14; // [esp+20h] [ebp-8h]
  int v15; // [esp+24h] [ebp-4h]

  v3 = *a2;
  v14 = a2[1];
  v15 = *(_DWORD *)(a1 + 4);
  v13 = *(_DWORD *)(a1 + 8);
  v4 = _m_punpcklbw(_mm_cvtsi32_si64(a3), 0LL);
  v5 = *(_DWORD *)(a1 + 12);
  v6 = v3;
  v7 = *(_DWORD **)a1;
  do
  {
    v12 = v7;
    v11 = v6;
    v8 = v13;
    do
    {
      result = *v7 >> 25;
      if ( result )
      {
        v10 = _m_punpcklbw(_mm_cvtsi32_si64(*v6), 0LL);
        *v6 = _mm_cvtsi64_si32(
                _m_packuswb(
                  _m_paddw(_m_psrawi(_m_pmullw(_m_psubw(v4, v10), *(__m64 *)&dword_50A8D0[2 * result]), 7u), v10),
                  0LL));
      }
      ++v6;
      v7 = (_DWORD *)((char *)v7 + 1);
      --v8;
    }
    while ( v8 );
    v6 = (unsigned int *)((char *)v11 + (_DWORD)v14);
    v7 = (_DWORD *)((char *)v12 + v15);
    --v5;
  }
  while ( v5 );
  _m_empty();
  return result;
}

// ===== sub_40E920 @ 0x0040E920..0x0040EA04 =====
// DECOMPILATION UNAVAILABLE (fail): see disassembly at 0x0040E920

// ===== sub_40EA10 @ 0x0040EA10..0x0040EA21 =====
int __usercall sub_40EA10@<eax>(int result@<eax>)
{
  if ( result >= 0 )
  {
    if ( result >= 4 )
      result -= 4;
  }
  else
  {
    result += 4;
  }
  return result;
}

// ===== sub_40EA30 @ 0x0040EA30..0x0040EA39 =====
double *__usercall sub_40EA30@<eax>(double *result@<eax>, double *a2@<ecx>)
{
  double v2; // st7

  v2 = *a2;
  *a2 = *result;
  *result = v2;
  return result;
}

// ===== sub_40EA40 @ 0x0040EA40..0x0040F62D =====
int __usercall sub_40EA40@<eax>(float *a1@<esi>, float **a2, int *a3, int *a4, int *a5, int a6)
{
  float *v6; // ebx
  int v7; // edi
  bool v8; // c0
  double v9; // st7
  int v10; // ebx
  double v11; // st7
  int v12; // edi
  double v13; // st7
  int v14; // ecx
  double v15; // st7
  int v16; // eax
  int v17; // eax
  float *v18; // eax
  int v19; // ecx
  float *v20; // ebx
  double v21; // st7
  double v22; // st6
  double v23; // st5
  double v24; // st4
  double v25; // st3
  double v26; // st2
  double v27; // st1
  float *v28; // edx
  int v29; // eax
  int v30; // ecx
  int v31; // edi
  double v32; // st1
  int v33; // ecx
  double v34; // st1
  float *v35; // eax
  double v36; // rt2
  double v37; // st2
  double v38; // st3
  double v39; // rtt
  double v40; // st2
  double v41; // st6
  double v42; // rt0
  double v43; // st2
  double v44; // st4
  double v45; // st1
  double v46; // rt1
  double v47; // st1
  double v48; // st3
  double v49; // rt2
  double v50; // st1
  double v51; // rtt
  double v52; // st1
  double v53; // rt0
  double v54; // st1
  double v55; // rt1
  float *v56; // edx
  int v57; // eax
  double v58; // st0
  double v59; // st7
  double v60; // st1
  double v61; // st5
  double v62; // st3
  double v63; // st2
  double v64; // st1
  double v65; // rt1
  double v66; // st1
  double v67; // rtt
  double v68; // st1
  double v69; // rt1
  int v70; // ecx
  double v71; // st0
  double v72; // st7
  int v73; // eax
  double v74; // st2
  float *v75; // ecx
  double v76; // st1
  double v77; // st2
  double v78; // st5
  double v79; // rtt
  double v80; // st2
  double v81; // st3
  double v82; // rt0
  double v83; // st2
  double v84; // rt1
  double v85; // st2
  double v86; // rt2
  double v87; // st1
  double v88; // st4
  double v89; // rt1
  double v90; // st1
  double v91; // st7
  double v92; // rt2
  double v93; // st1
  double v94; // st3
  double v95; // rtt
  double v96; // st1
  double v97; // st2
  double v98; // st6
  double v99; // st5
  double v100; // st2
  double v101; // rt2
  double v102; // st1
  double v103; // st7
  double v104; // st7
  int v105; // edx
  double v106; // st7
  double v107; // st5
  double v108; // st6
  double v109; // st7
  double v110; // st7
  int v111; // eax
  double v112; // st7
  int v113; // ecx
  int v114; // eax
  int v115; // edx
  double v116; // st6
  int v117; // ecx
  int v118; // eax
  double v119; // st6
  double v120; // st5
  double v121; // st3
  double v122; // st6
  double v123; // st1
  double v124; // st5
  double v125; // rt0
  double v126; // st2
  double v127; // st4
  double v128; // st2
  double v129; // rt0
  double v130; // st2
  double v131; // rt2
  double v132; // st2
  double v133; // rt0
  double v134; // rt1
  double v135; // st2
  double v136; // st4
  int v137; // edx
  int v138; // ecx
  int v139; // edx
  double v141; // [esp+18h] [ebp-13Ch]
  double v142; // [esp+20h] [ebp-134h]
  double v143; // [esp+28h] [ebp-12Ch]
  double v144; // [esp+28h] [ebp-12Ch]
  double v145; // [esp+30h] [ebp-124h]
  double v146; // [esp+30h] [ebp-124h]
  double v147; // [esp+38h] [ebp-11Ch] BYREF
  double v148; // [esp+40h] [ebp-114h]
  double v149; // [esp+48h] [ebp-10Ch]
  double v150; // [esp+50h] [ebp-104h]
  int v151; // [esp+58h] [ebp-FCh]
  int v152; // [esp+5Ch] [ebp-F8h]
  double v153; // [esp+60h] [ebp-F4h]
  int v154; // [esp+6Ch] [ebp-E8h]
  double v155; // [esp+70h] [ebp-E4h]
  double v156; // [esp+78h] [ebp-DCh]
  double v157; // [esp+80h] [ebp-D4h]
  int v158; // [esp+8Ch] [ebp-C8h]
  double v159; // [esp+90h] [ebp-C4h]
  int v160; // [esp+9Ch] [ebp-B8h]
  double v161; // [esp+A0h] [ebp-B4h]
  double v162; // [esp+A8h] [ebp-ACh]
  double v163; // [esp+B0h] [ebp-A4h] BYREF
  int v164; // [esp+B8h] [ebp-9Ch]
  int v165; // [esp+BCh] [ebp-98h]
  double v166; // [esp+C0h] [ebp-94h] BYREF
  int v167; // [esp+CCh] [ebp-88h]
  double v168; // [esp+D0h] [ebp-84h]
  double X; // [esp+D8h] [ebp-7Ch] BYREF
  double v170; // [esp+E0h] [ebp-74h] BYREF
  double v171; // [esp+E8h] [ebp-6Ch]
  double v172; // [esp+F0h] [ebp-64h]
  double v173; // [esp+F8h] [ebp-5Ch] BYREF
  double v174; // [esp+100h] [ebp-54h] BYREF
  double v175; // [esp+108h] [ebp-4Ch]
  double v176; // [esp+110h] [ebp-44h] BYREF
  double v177; // [esp+118h] [ebp-3Ch]
  double v178; // [esp+120h] [ebp-34h]
  double v179; // [esp+128h] [ebp-2Ch]
  double v180; // [esp+130h] [ebp-24h] BYREF
  int v181; // [esp+13Ch] [ebp-18h]
  int v182; // [esp+140h] [ebp-14h]
  int i; // [esp+144h] [ebp-10h]
  double v184; // [esp+148h] [ebp-Ch]

  v6 = a1;
  X = sub_4010B0(a1[1], 0.0009765625);
  v177 = sub_4010B0(a1[1], 0.0009765625);
  v172 = sub_4010B0(*a1, 0.0009765625);
  v170 = sub_4010B0(*a1, 0.0009765625);
  v7 = 0;
  for ( i = 1; i < 4; ++i )
  {
    v6 += 5;
    if ( sub_4010B0(v6[1], 0.0009765625) < X )
    {
      v7 = i;
      X = sub_4010B0(v6[1], 0.0009765625);
    }
    if ( sub_4010B0(v6[1], 0.0009765625) > v177 )
      v177 = sub_4010B0(v6[1], 0.0009765625);
    if ( sub_4010B0(*v6, 0.0009765625) < v172 )
      v172 = sub_4010B0(*v6, 0.0009765625);
    v8 = v170 < sub_4010B0(*v6, 0.0009765625);
    v9 = v170;
    if ( v8 )
    {
      v9 = sub_4010B0(*v6, 0.0009765625);
      v170 = v9;
    }
  }
  if ( v177 == X )
    return 0;
  if ( v172 == v9 )
    return 0;
  v10 = (int)floor(X);
  i = v10;
  v181 = (int)ceil(v177);
  if ( v181 < 0 )
    return 0;
  v154 = (int)floor(v172);
  v11 = ceil(v170);
  v168 = 0.0;
  v165 = v7;
  v150 = 0.0;
  v167 = v7;
  v159 = 0.0;
  v153 = 0.0;
  v171 = 0.0;
  v149 = 0.0;
  v12 = 5 * v7;
  v161 = 0.0;
  v177 = 0.0;
  v175 = a1[v12];
  v152 = (int)v11;
  v148 = a1[v12 + 2];
  v151 = v12 * 4;
  v13 = a1[v12 + 3];
  v160 = 0x7FFFFFFF;
  v156 = v13;
  v158 = 0x80000000;
  v157 = a1[v12 + 4];
  v14 = v10 < 0 ? 0 : v10;
  v179 = a1[v12];
  HIDWORD(v184) = v14;
  v155 = a1[v12 + 2];
  v172 = a1[v12 + 3];
  v15 = a1[v12 + 4];
  *a4 = v14;
  v178 = v15;
  v16 = v181;
  if ( v181 >= a6 )
    v16 = a6 - 1;
  v17 = v16 - v14 + 1;
  *a3 = v17;
  v18 = (float *)operator new[](32 * v17);
  *a2 = v18;
  v19 = HIDWORD(v184);
  v20 = v18;
  v182 = HIDWORD(v184);
  if ( SHIDWORD(v184) <= v181 )
  {
    v21 = v171;
    v22 = v156;
    v164 = HIDWORD(v184) + 1;
    v23 = v155;
    v24 = v157;
    v25 = v148;
    v26 = v172;
    while ( 1 )
    {
      if ( v19 >= a6 )
        goto LABEL_83;
      v27 = (double)v164;
      v28 = &a1[v12];
      v180 = v27;
      if ( a1[v12 + 1] >= v27 || v19 >= v181 )
      {
        v46 = v27;
        v47 = v25;
        v48 = v46;
        v49 = v47;
        v50 = v22;
        v41 = v49;
        v51 = v50;
        v52 = v24;
        v44 = v51;
        v53 = v52;
        v54 = v26;
        v43 = v53;
        v55 = v54;
        v45 = v48;
        v38 = v55;
      }
      else
      {
        if ( v19 != i )
        {
          v184 = v28[1] - (double)v182;
          v175 = v184 * v168 + v175;
          v25 = v25 + v184 * v150;
          v22 = v22 + v184 * v159;
          v24 = v24 + v184 * v153;
        }
        v29 = 0;
        v162 = v28[1];
        while ( 1 )
        {
          if ( v29 )
          {
            v175 = *v28;
            v22 = v28[3];
            v24 = v28[4];
            v25 = v28[2];
          }
          v31 = sub_40EA10(v165 + 1);
          v32 = a1[5 * v31 + 1];
          v28 = &a1[5 * v31];
          v165 = v31;
          HIDWORD(v184) = 1;
          if ( v32 < v162 || v28[1] >= v180 )
            break;
          v29 = HIDWORD(v184);
        }
        v12 = 5 * v31;
        v33 = 5 * v30;
        v34 = a1[v12 + 1] - a1[v33 + 1];
        v35 = &a1[v33];
        v19 = v182;
        v184 = v34;
        v168 = (a1[v12] - *v35) / v34;
        v150 = (a1[v12 + 2] - v35[2]) / v34;
        v159 = (a1[v12 + 3] - v35[3]) / v34;
        v153 = (a1[v12 + 4] - v35[4]) / v34;
        v184 = v180 - v35[1] - 1.0;
        v175 = v184 * v168 + v175;
        v36 = v26;
        v37 = v25 + v184 * v150;
        v38 = v36;
        v148 = v37;
        v39 = v37;
        v40 = v22 + v184 * v159;
        v41 = v39;
        v156 = v40;
        v42 = v40;
        v43 = v24 + v184 * v153;
        v44 = v42;
        v157 = v43;
        v45 = v180;
      }
      v56 = (float *)((char *)a1 + v151);
      if ( *(float *)((char *)a1 + v151 + 4) >= v45 || v19 >= v181 )
      {
        v82 = v43;
        v83 = v21;
        v72 = v82;
        v84 = v83;
        v85 = v23;
        v78 = v84;
        v86 = v85;
        v80 = v38;
        v81 = v86;
      }
      else
      {
        if ( v19 == i )
        {
          v59 = v45;
          v57 = 0;
        }
        else
        {
          v57 = 0;
          v58 = v56[1] - (double)v182;
          v179 = v21 * v58 + v179;
          v23 = v23 + v58 * v149;
          v38 = v38 + v58 * v161;
          v178 = v58 * v177 + v178;
          v59 = v180;
        }
        v60 = v56[1];
        while ( 1 )
        {
          if ( v57 )
          {
            v61 = v60;
            v62 = v43;
            v179 = *v56;
            v63 = v56[2];
            v64 = v56[3];
            v178 = v56[4];
            v65 = v64;
            v66 = v61;
            v23 = v63;
            v67 = v66;
            v68 = v62;
            v38 = v65;
            v69 = v68;
            v60 = v67;
            v43 = v69;
          }
          HIDWORD(v174) = v167;
          v70 = sub_40EA10(v167 - 1);
          v71 = a1[5 * v70 + 1];
          v56 = &a1[5 * v70];
          v167 = v70;
          HIDWORD(v184) = 1;
          if ( v71 < v60 || v56[1] >= v59 )
            break;
          v57 = HIDWORD(v184);
        }
        v72 = v43;
        v73 = 5 * v70;
        v74 = a1[5 * v70 + 1] - a1[5 * HIDWORD(v174) + 1];
        v75 = &a1[5 * HIDWORD(v174)];
        v151 = v73 * 4;
        v184 = v74;
        v171 = (a1[v73] - *v75) / v74;
        v149 = (a1[v73 + 2] - v75[2]) / v74;
        v161 = (a1[v73 + 3] - v75[3]) / v74;
        v177 = (a1[v73 + 4] - v75[4]) / v74;
        v76 = v180 - v75[1];
        v19 = v182;
        v184 = v76 - 1.0;
        v179 = v184 * v171 + v179;
        v77 = v23 + v184 * v149;
        v78 = v171;
        v155 = v77;
        v79 = v77;
        v80 = v38 + v184 * v161;
        v81 = v79;
        v172 = v80;
        v178 = v184 * v177 + v178;
      }
      if ( v19 < 0 )
      {
        v125 = v80;
        v126 = v72;
        v91 = v44;
        v127 = v126;
        v128 = v41;
        v98 = v125;
        v129 = v128;
        v130 = v78;
        v99 = v81;
        v131 = v130;
        v132 = v127;
        v88 = v129;
        v133 = v132;
        v100 = v131;
        v94 = v133;
        goto LABEL_82;
      }
      v174 = v175;
      v147 = v41;
      v87 = v44;
      v88 = v41;
      v170 = v87;
      v89 = v87;
      v90 = v72;
      v91 = v89;
      X = v90;
      v180 = v179;
      v92 = v90;
      v93 = v81;
      v94 = v92;
      v166 = v93;
      v95 = v93;
      v96 = v80;
      v97 = v95;
      v176 = v96;
      v173 = v178;
      if ( v19 != i )
        break;
      v98 = v96;
      if ( v179 != v175 )
        goto LABEL_51;
      v163 = v179 + v78;
      v162 = v175 + v168;
      v184 = v163 - v162;
      if ( 0.0 == v184 )
        goto LABEL_51;
      v99 = v97;
      v145 = (v97 + v149 - (v88 + v150)) / v184;
      v173 = (v96 + v161 - (v91 + v159)) / v184;
      v184 = (v178 + v177 - (v94 + v153)) / v184;
      v174 = v162;
      v147 = v145 * v168 + v88;
      v170 = v173 * v168 + v91;
      X = v168 * v184 + v94;
      v180 = v163;
      v100 = v171;
      v166 = v145 * v171 + v99;
      v176 = v173 * v171 + v96;
      v173 = v184 * v171 + v178;
LABEL_52:
      if ( v180 < v174 )
      {
        sub_40EA30(&v180, &v174);
        sub_40EA30(&v166, &v147);
        sub_40EA30(&v176, &v170);
        sub_40EA30(&v173, &X);
        v19 = v182;
        v99 = v155;
        v98 = v172;
        v91 = v156;
        v88 = v148;
        v100 = v171;
        v94 = v157;
      }
      v102 = v180 - v174;
      if ( v180 - v174 <= 0.0 )
      {
        v20[2] = 0.0;
        v20[3] = 0.0;
        v20[4] = 0.0;
        v20[5] = 0.0;
        v20[6] = 0.0;
        v20[7] = 0.0;
        *v20 = NAN;
        v20[1] = -0.0;
        v20 += 8;
      }
      else
      {
        if ( v19 == i || v102 >= 2.0 )
        {
          v142 = (v166 - v147) / v102;
          v146 = (v176 - v170) / v102;
          v103 = (v173 - X) / v102;
        }
        else
        {
          v146 = v143;
          v103 = v141;
        }
        v162 = v103;
        v144 = sub_4010B0(v174, 0.0009765625);
        v163 = sub_4010B0(v180, 0.0009765625);
        HIDWORD(v166) = (int)floor(v144);
        v104 = ceil(v163);
        v105 = HIDWORD(v166);
        HIDWORD(v174) = (int)v104;
        HIDWORD(v184) = (int)v104;
        HIDWORD(v180) = HIDWORD(v166);
        if ( v182 >= v181 )
        {
          v112 = v171;
        }
        else
        {
          v106 = v175 + v168;
          v163 = v175 + v168;
          v107 = v179 + v171;
          v108 = v171;
          v173 = v107;
          if ( v107 < v163 )
          {
            sub_40EA30(&v173, &v163);
            v106 = v163;
            v108 = v171;
            v107 = v173;
          }
          if ( v107 - v106 <= 0.0 )
          {
            v112 = v108;
          }
          else
          {
            v109 = sub_4010B0(v106, 0.0009765625);
            HIDWORD(v180) = (int)floor(v109);
            v110 = sub_4010B0(v173, 0.0009765625);
            v111 = (int)ceil(v110);
            v112 = v171;
            v105 = HIDWORD(v166);
            HIDWORD(v184) = v111;
          }
        }
        v113 = v160;
        v114 = HIDWORD(v174);
        HIDWORD(v176) = v105;
        if ( v160 >= v105 )
          v113 = HIDWORD(v176);
        else
          HIDWORD(v176) = v160;
        if ( SHIDWORD(v180) < v113 )
        {
          v113 = HIDWORD(v180);
          HIDWORD(v176) = HIDWORD(v180);
        }
        if ( v113 < v154 )
          HIDWORD(v176) = v154;
        v115 = HIDWORD(v174);
        if ( SHIDWORD(v174) < v158 )
          v114 = v158;
        if ( v114 < SHIDWORD(v184) )
          v114 = HIDWORD(v184);
        if ( v152 < v114 )
          v114 = v152;
        v116 = (double)SHIDWORD(v176);
        v117 = HIDWORD(v176);
        *((_DWORD *)v20 + 1) = v114;
        v118 = HIDWORD(v166);
        v119 = v144 - v116;
        *(_DWORD *)v20 = v117;
        v19 = v182;
        v160 = v118;
        v158 = v115;
        v20 += 8;
        v120 = v147 - v119 * v142;
        v121 = v170 - v119 * v146;
        v122 = X - v119 * v162;
        v123 = v120;
        v124 = v162;
        *(v20 - 6) = v123;
        *(v20 - 5) = v121;
        *(v20 - 4) = v122;
        *(v20 - 3) = v142;
        *(v20 - 2) = v146;
        *(v20 - 1) = v124;
        v143 = v146;
        v141 = v124;
        v99 = v155;
        v98 = v172;
        v100 = v112;
        v91 = v156;
        v88 = v148;
        v94 = v157;
      }
LABEL_82:
      ++v164;
      v182 = ++v19;
      v175 = v175 + v168;
      v134 = v100;
      v135 = v88;
      v136 = v134;
      v148 = v135 + v150;
      v156 = v91 + v159;
      v157 = v94 + v153;
      v179 = v179 + v134;
      v155 = v99 + v149;
      v26 = v98 + v161;
      v172 = v98 + v161;
      v178 = v178 + v177;
      if ( v19 > v181 )
        goto LABEL_83;
      v21 = v136;
      v25 = v148;
      v22 = v156;
      v23 = v155;
      v24 = v157;
    }
    v98 = v96;
LABEL_51:
    v101 = v97;
    v100 = v78;
    v99 = v101;
    goto LABEL_52;
  }
LABEL_83:
  if ( a5 )
  {
    v137 = v152;
    *a5 = v154;
    v138 = i;
    a5[2] = v137;
    v139 = v181;
    a5[1] = v138;
    a5[3] = v139;
  }
  return 1;
}

// ===== sub_40F630 @ 0x0040F630..0x0040F969 =====
void __usercall sub_40F630(
        float *a1@<eax>,
        int a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        unsigned int a15,
        int a16)
{
  double v18; // st7
  double v19; // st5
  double v20; // st4
  double v21; // st3
  double v22; // st7
  int v23; // edx
  double v24; // st2
  double v25; // rt0
  float *v26; // esi
  int v27; // ebx
  int v28; // edi
  char v29; // al
  long double v30; // st3
  long double v31; // st7
  long double v32; // st3
  long double v33; // st7
  long double v34; // st3
  long double v35; // st7
  long double v36; // st6
  long double v37; // st5
  double v38; // st4
  long double v39; // st5
  _DWORD v40[6]; // [esp+0h] [ebp-78h]
  double v41; // [esp+18h] [ebp-60h]
  double v42; // [esp+20h] [ebp-58h]
  double v43; // [esp+28h] [ebp-50h]
  double v44; // [esp+30h] [ebp-48h]
  double v45; // [esp+38h] [ebp-40h]
  long double v46; // [esp+40h] [ebp-38h]
  long double v47; // [esp+48h] [ebp-30h]
  long double v48; // [esp+50h] [ebp-28h]
  long double v49; // [esp+58h] [ebp-20h]
  long double v50; // [esp+60h] [ebp-18h]
  long double v51; // [esp+68h] [ebp-10h]
  long double v52; // [esp+70h] [ebp-8h]
  float v53; // [esp+ACh] [ebp+34h]
  float v54; // [esp+ACh] [ebp+34h]
  int v55; // [esp+ACh] [ebp+34h]
  float v56; // [esp+B0h] [ebp+38h]
  float v57; // [esp+B0h] [ebp+38h]

  v18 = (double)a7 * 0.0000152587890625;
  v19 = (double)a5 * 0.0000152587890625 * v18;
  v20 = (double)a8 * 0.0000152587890625;
  v21 = (double)a6 * 0.0000152587890625 * v20;
  v22 = v18 * (double)(unsigned int)(*(_DWORD *)(a2 + 8) - 1);
  v23 = *(_DWORD *)(a2 + 12) - 1;
  v56 = -v19;
  v24 = v56;
  *a1 = v56;
  v57 = -v21;
  a1[1] = v57;
  a1[2] = 0.0;
  a1[3] = 0.0;
  a1[4] = 0.0;
  a1[5] = v24;
  v53 = v20 * (double)(unsigned int)v23 - v21;
  a1[6] = v53;
  a1[7] = 0.0;
  a1[8] = 0.0;
  a1[9] = (float)(unsigned int)(*(_DWORD *)(a2 + 12) - 1);
  v25 = v53;
  v54 = v22 - v19;
  a1[10] = v54;
  a1[11] = v25;
  a1[12] = 0.0;
  a1[13] = (float)(unsigned int)(*(_DWORD *)(a2 + 8) - 1);
  a1[14] = (float)(unsigned int)(*(_DWORD *)(a2 + 12) - 1);
  a1[15] = v54;
  a1[16] = v57;
  a1[17] = 0.0;
  a1[18] = (float)(unsigned int)(*(_DWORD *)(a2 + 8) - 1);
  v40[0] = "phb";
  v40[1] = "pbh";
  a1[19] = 0.0;
  v40[2] = "hpb";
  v40[3] = "hbp";
  v40[4] = "bph";
  v40[5] = "bhp";
  v55 = v40[a15 % 6];
  v26 = a1 + 2;
  v27 = 4;
  v49 = (double)-a12 * 3.141592653589793 / 11796480.0;
  v48 = (double)a13 * 3.141592653589793 / 11796480.0;
  v47 = 3.141592653589793 * (double)-a14 / 11796480.0;
  v45 = (double)a9 * 0.0000152587890625;
  v44 = (double)a10 * 0.0000152587890625;
  v43 = (double)a11 * 0.0000152587890625;
  v42 = (double)a3 * 0.0000152587890625;
  v41 = 0.0000152587890625 * (double)a4;
  do
  {
    v28 = 0;
    v52 = *(v26 - 2);
    v51 = *(v26 - 1);
    v50 = *v26;
    do
    {
      v29 = *(_BYTE *)(v28 + v55);
      switch ( v29 )
      {
        case 'b':
          v46 = cos(v47);
          v34 = sin(v47);
          v35 = v46 * v52 - v34 * v51;
          v51 = v52 * v34 + v51 * v46;
          v52 = v35;
          break;
        case 'h':
          v46 = cos(v48);
          v32 = sin(v48);
          v33 = v46 * v50 - v32 * v52;
          v52 = v50 * v32 + v52 * v46;
          v50 = v33;
          break;
        case 'p':
          v46 = cos(v49);
          v30 = sin(v49);
          v31 = v46 * v51 - v30 * v50;
          v50 = v51 * v30 + v50 * v46;
          v51 = v31;
          break;
      }
      ++v28;
    }
    while ( v28 < 3 );
    v36 = v44 + v51;
    v37 = v43 + v50;
    if ( 0.0 == v43 + v50 || !a16 )
    {
      v39 = 1.0;
    }
    else
    {
      v38 = (double)a16;
      if ( v37 <= 0.0 )
        v39 = (v38 - v37) / v38;
      else
        v39 = v38 / (v37 + v38);
    }
    v26 += 5;
    --v27;
    *(v26 - 7) = (v45 + v52) * v39 + v42;
    *(v26 - 6) = v36 * v39 + v41;
    *(v26 - 5) = v39;
    *(v26 - 4) = *(v26 - 4) * v39;
    *(v26 - 3) = v39 * *(v26 - 3);
  }
  while ( v27 );
}

// ===== sub_40F970 @ 0x0040F970..0x0040FA49 =====
void __usercall sub_40F970(
        int a1@<edi>,
        int a2,
        int a3,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        unsigned int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        unsigned int a15,
        int a16,
        int a17,
        int a18)
{
  void *v18; // esi
  int v19; // [esp-3Ch] [ebp-B0h]
  int v20; // [esp+Ch] [ebp-68h] BYREF
  int v21; // [esp+10h] [ebp-64h] BYREF
  void *v22; // [esp+14h] [ebp-60h] BYREF
  float v23[21]; // [esp+18h] [ebp-5Ch] BYREF

  sub_40F630(v23, a4, a2, a3, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
  v19 = *(_DWORD *)(a1 + 12);
  v22 = 0;
  if ( sub_40EA40(v23, (float **)&v22, &v20, &v21, 0, v19) )
  {
    v18 = v22;
    sub_40FA50(a1, a4, v22, v20, v21, 0, a17, a18, 1);
    operator delete[](v18);
  }
  else if ( !a17 )
  {
    sub_40A620(a1, 0);
  }
}

// ===== sub_40FA50 @ 0x0040FA50..0x0040FB90 =====
int __cdecl sub_40FA50(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  int result; // eax
  int v10; // ecx
  bool v11; // cf
  int v12; // [esp+Ch] [ebp-24h] BYREF
  _DWORD v13[3]; // [esp+10h] [ebp-20h] BYREF
  _DWORD v14[4]; // [esp+1Ch] [ebp-14h] BYREF

  if ( !a9 )
    goto LABEL_3;
  v14[1] = a6;
  v14[3] = a8;
  v13[1] = a4;
  v14[2] = a7;
  v13[2] = a5;
  v12 = a1;
  v14[0] = a2;
  v13[0] = a3;
  result = sub_419DE0(sub_41A350, &v12, 1, 4, v13, v14);
  if ( !result )
  {
LABEL_3:
    if ( a7 )
    {
      result = a7 - 1;
      if ( a7 == 1 )
      {
        result = *(_DWORD *)(a1 + 16) - 1;
        if ( *(_DWORD *)(a1 + 16) == 1 )
        {
          if ( *(_DWORD *)(a2 + 16) == 1 )
          {
            return sub_410040(a1, a2, a4, a5, a6, a8);
          }
          else
          {
            result = *(_DWORD *)(a2 + 16) - 2;
            if ( *(_DWORD *)(a2 + 16) == 2 )
              return sub_410420(a1, a2, a4, a5, a6, a8);
          }
        }
      }
    }
    else
    {
      v10 = *(_DWORD *)(a1 + 16);
      result = *(_DWORD *)(a2 + 16);
      if ( v10 == result )
      {
        v11 = result-- == 1;
        if ( (v11 || result == 1) && (unsigned int)(v10 - 1) <= 1 )
          return sub_40FB90(a1, a2, a3, a4, a5, a6);
      }
    }
  }
  return result;
}

// ===== sub_40FB90 @ 0x0040FB90..0x0041003C =====
int __usercall sub_40FB90@<eax>(int a1@<ebp>, float *a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // eax
  const void *v8; // edx
  int v9; // eax
  __m128 v10; // xmm1
  unsigned int v11; // ecx
  __m128i v12; // xmm7
  int v13; // ecx
  int v14; // edi
  __m128 v15; // xmm5
  int v16; // eax
  __m128i v17; // xmm2
  int v18; // edx
  float v19; // eax
  int v20; // esi
  int v21; // edi
  float v22; // eax
  char *v23; // ecx
  int v24; // esi
  __m128 v25; // xmm4
  __m128 v26; // xmm6
  __m128 v27; // xmm3
  unsigned int v28; // ecx
  __m128 v29; // xmm0
  __m128i v30; // xmm1
  unsigned __int32 v31; // esi
  __m128i v32; // xmm0
  __m128i v33; // xmm6
  __m128i v34; // xmm7
  int v35; // eax
  unsigned int v36; // edx
  __m128i v37; // xmm5
  __m128i v38; // xmm0
  int v39; // eax
  unsigned int v40; // ecx
  __m128i v41; // xmm0
  __m128i v42; // xmm1
  __m128i v43; // xmm0
  char *v44; // ecx
  bool v45; // zf
  int v46; // eax
  int result; // eax
  int *v48; // eax
  __m128i v49; // [esp-D0h] [ebp-DCh]
  __m128 v50; // [esp-C0h] [ebp-CCh]
  __m128i si128; // [esp-B0h] [ebp-BCh]
  __m128 v52; // [esp-90h] [ebp-9Ch]
  _DWORD v53[4]; // [esp-80h] [ebp-8Ch] BYREF
  __m128i v54; // [esp-70h] [ebp-7Ch] BYREF
  _DWORD v55[8]; // [esp-58h] [ebp-64h] BYREF
  float v56; // [esp-38h] [ebp-44h]
  unsigned int v57; // [esp-34h] [ebp-40h]
  __m128i v58; // [esp-30h] [ebp-3Ch] BYREF
  char *v59; // [esp-1Ch] [ebp-28h]
  int v60; // [esp-18h] [ebp-24h]
  float *v61; // [esp-14h] [ebp-20h]
  char *v62; // [esp-10h] [ebp-1Ch]
  unsigned int v63; // [esp-Ch] [ebp-18h]
  int v64; // [esp-8h] [ebp-14h]
  int v65; // [esp-4h] [ebp-10h]
  int v66; // [esp+0h] [ebp-Ch]
  void *v67; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v66 = a1;
  v67 = retaddr;
  if ( a6 >= 1 )
  {
    sub_409190(&v58, (int)a2);
    v7 = *((_DWORD *)a2 + 3);
    if ( a6 <= v7 )
      v7 = a6;
    v58.m128i_i32[3] = v7 - 1;
    sub_40A620((int)a2, v58.m128i_i32);
  }
  v8 = (const void *)(a4 - 32 * (a6 >= 0 ? 0 : a6));
  v63 = (unsigned int)v8;
  if ( a6 <= 0 )
    v9 = 0;
  else
    v9 = a6 * *((_DWORD *)a2 + 1);
  v62 = (char *)(v9 + *(_DWORD *)a2);
  if ( a5 < 1 )
  {
    v60 = 0;
  }
  else if ( a5 + a6 >= *((_DWORD *)a2 + 3) )
  {
    v60 = *((_DWORD *)a2 + 3);
  }
  else
  {
    v60 = a5 + a6;
  }
  memset(v55, 0, sizeof(v55));
  if ( *(float *)&v8 != 0.0 )
    qmemcpy(v55, v8, sizeof(v55));
  v10 = (__m128)_mm_cvtsi32_si128(0);
  v11 = *(unsigned __int16 *)(a3 + 4);
  LOWORD(v65) = _mm_getcsr();
  v65 = v65 & 0xF3FF | 0x400;
  si128 = _mm_load_si128((const __m128i *)&xmmword_4E42A0);
  v12 = _mm_cvtsi32_si128(v11);
  v13 = a6;
  v54 = 0LL;
  v14 = v60;
  v15 = (__m128)_mm_unpacklo_epi16((__m128i)v10, (__m128i)v10);
  v16 = v13 <= 0 ? 0 : v13;
  v49 = _mm_unpacklo_epi16(
          _mm_unpacklo_epi16(_mm_unpacklo_epi16(_mm_cvtsi32_si128(4u), (__m128i)v10), (__m128i)v15),
          _mm_unpacklo_epi16(_mm_unpacklo_epi16(v12, (__m128i)v10), (__m128i)v15));
  _mm_setcsr(v65);
  if ( v16 < v60 )
  {
    v17 = _mm_load_si128(&v54);
    v18 = v63 + 8;
    v61 = (float *)(v63 + 8);
    v65 = v60 - v16;
    while ( 1 )
    {
      LODWORD(v19) = a7 + *(_DWORD *)(v18 - 8);
      v20 = a7 + *(_DWORD *)(v18 - 4);
      *(float *)&v64 = v19;
      if ( SLODWORD(v19) > v20 || SLODWORD(v19) >= *((_DWORD *)a2 + 2) || v20 < 0 )
      {
        memset(v62, 0, 4 * *((_DWORD *)a2 + 2));
      }
      else
      {
        if ( SLODWORD(v19) < 1 )
        {
          v21 = 0;
          *(float *)&v64 = (float)v64;
          v56 = *v61 - v61[3] * *(float *)&v64;
          *(float *)&v63 = v61[1] - v61[4] * *(float *)&v64;
          *(float *)&v57 = v61[2] - *(float *)&v64 * v61[5];
        }
        else
        {
          v56 = *v61;
          v21 = LODWORD(v19);
          v63 = *((unsigned int *)v61 + 1);
          v57 = *((unsigned int *)v61 + 2);
          memset(v62, 0, 4 * LODWORD(v19));
          v17 = _mm_load_si128(&v54);
        }
        v22 = a2[2];
        v23 = &v62[4 * v21];
        v59 = v23;
        if ( v20 >= SLODWORD(v22) )
        {
          *(float *)&v24 = v22;
          *(float *)&v64 = v22;
        }
        else
        {
          v24 = v20 + 1;
          v64 = v24;
        }
        v10.m128_f32[0] = 0.0;
        v15.m128_f32[0] = 0.0;
        v25 = _mm_shuffle_ps((__m128)LODWORD(v56), (__m128)LODWORD(v56), 0);
        v26 = _mm_unpacklo_ps((__m128)v57, v10);
        v10 = _mm_shuffle_ps((__m128)v55[5], (__m128)v55[5], 0);
        v27 = _mm_unpacklo_ps(_mm_unpacklo_ps((__m128)v63, v15), v26);
        v50 = v10;
        v10.m128_f32[0] = 0.0;
        v52 = _mm_unpacklo_ps(_mm_unpacklo_ps((__m128)v55[6], v15), _mm_unpacklo_ps((__m128)v55[7], v10));
        if ( v21 < v24 )
        {
          v63 = v24 - v21;
          do
          {
            v28 = *(_DWORD *)(a3 + 12);
            v29 = _mm_rcp_ps(v25);
            v30 = _mm_cvtps_epi32(
                    _mm_mul_ps(
                      _mm_mul_ps(_mm_sub_ps(_mm_add_ps(v29, v29), _mm_mul_ps(_mm_mul_ps(v29, v29), v25)), v27),
                      (__m128)xmmword_4E41D0));
            v58 = _mm_srai_epi32(v30, 0x10u);
            v31 = v58.m128i_i32[0];
            v32 = _mm_srli_epi16(_mm_and_si128(v30, si128), 1u);
            v33 = _mm_shufflelo_epi16(v32, 0);
            v34 = _mm_shufflelo_epi16(v32, 170);
            v35 = _mm_cvtsi128_si32(_mm_madd_epi16(_mm_packs_epi32(v58, v17), v49));
            if ( v58.m128i_i32[1] >= v28 )
            {
              v15 = (__m128)v17;
            }
            else
            {
              v36 = *(_DWORD *)(a3 + 8);
              if ( v58.m128i_i32[0] >= v36 )
              {
                v37 = v17;
              }
              else
              {
                v31 = v58.m128i_i32[0];
                v37 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a3 + v35)), v17);
              }
              if ( v31 + 1 >= v36 )
                v38 = v17;
              else
                v38 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a3 + v35 + 4)), v17);
              v31 = v58.m128i_i32[0];
              v15 = (__m128)_mm_add_epi16(v37, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v38, v37), 1u), v33));
            }
            v39 = *(_DWORD *)(a3 + 4) + v35;
            if ( v58.m128i_i32[1] + 1 >= v28 )
            {
              v43 = v17;
            }
            else
            {
              v40 = *(_DWORD *)(a3 + 8);
              if ( v31 >= v40 )
                v41 = v17;
              else
                v41 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a3 + v39)), v17);
              if ( v31 + 1 >= v40 )
                v42 = v17;
              else
                v42 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(_DWORD *)(*(_DWORD *)a3 + v39 + 4)), v17);
              v43 = _mm_add_epi16(v41, _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v42, v41), 1u), v33));
            }
            v44 = v59;
            v10 = (__m128)_mm_or_si128((__m128i)v15, v43);
            v58 = (__m128i)v10;
            if ( v10.m128_u64[0] )
            {
              v10 = (__m128)_mm_packus_epi16(
                              _mm_add_epi16(
                                (__m128i)v15,
                                _mm_mulhi_epi16(_mm_slli_epi16(_mm_sub_epi16(v43, (__m128i)v15), 1u), v34)),
                              v17);
              *(_DWORD *)v59 = _mm_cvtsi128_si32((__m128i)v10);
            }
            else
            {
              *(_DWORD *)v59 = 0;
            }
            v25 = _mm_add_ps(v25, v50);
            v23 = v44 + 4;
            v45 = v63-- == 1;
            v27 = _mm_add_ps(v52, v27);
            v59 = v23;
          }
          while ( !v45 );
          v24 = v64;
        }
        v46 = *((_DWORD *)a2 + 2);
        if ( v24 >= v46 )
          goto LABEL_55;
        memset(v23, 0, 4 * (v46 - v24));
      }
      v17 = _mm_load_si128(&v54);
LABEL_55:
      v62 += *((_DWORD *)a2 + 1);
      v18 = (int)(v61 + 8);
      v45 = v65-- == 1;
      v61 += 8;
      if ( v45 )
      {
        v14 = v60;
        v13 = a6;
        break;
      }
    }
  }
  result = *((_DWORD *)a2 + 3);
  if ( v14 < result && v13 < result )
  {
    v48 = sub_409190(v53, (int)a2);
    v53[1] = v14;
    return sub_40A620((int)a2, v48);
  }
  return result;
}
