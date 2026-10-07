#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_49C7B0 @ 0x0049C7B0..0x0049CBBE =====
BOOL __stdcall sub_49C7B0(int *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // esi
  int v4; // ecx
  int v5; // edi
  int v6; // eax
  int v7; // ebx
  void *v8; // edx
  int v9; // eax
  unsigned int v10; // edi
  int v12; // ecx
  int v13; // edi
  unsigned int v14; // ebx
  int v15; // ecx
  int v16; // edx
  int v17; // eax
  int v18; // ecx
  size_t v19; // eax
  int v20; // ebx
  unsigned int v21; // edi
  int v22; // ecx
  unsigned int v23; // ebx
  int v24; // edi
  int v25; // [esp-8h] [ebp-3Ch]
  int v26; // [esp-4h] [ebp-38h]
  int v27; // [esp-4h] [ebp-38h]
  int v28; // [esp+Ch] [ebp-28h]
  int v29; // [esp+10h] [ebp-24h]
  int v30; // [esp+14h] [ebp-20h]
  void *v31; // [esp+14h] [ebp-20h]
  int v32; // [esp+18h] [ebp-1Ch]
  _DWORD *v33; // [esp+1Ch] [ebp-18h]
  char *v34; // [esp+1Ch] [ebp-18h]
  int v35; // [esp+20h] [ebp-14h]
  int v36; // [esp+20h] [ebp-14h]
  size_t Size; // [esp+24h] [ebp-10h] BYREF
  void *Src; // [esp+28h] [ebp-Ch]
  int v39; // [esp+2Ch] [ebp-8h] BYREF
  void *v40; // [esp+30h] [ebp-4h]
  int v41; // [esp+3Ch] [ebp+8h]
  int v42; // [esp+3Ch] [ebp+8h]
  int v43; // [esp+3Ch] [ebp+8h]

  v1 = sub_4465D0(a1[4]);
  v2 = sub_49EEF0(a1);
  v28 = v2;
  sub_4465F0(a1[4], v1);
  if ( v2 < 0 )
    return v28 >= 0;
  v3 = a1[5] + 132 * v2;
  if ( !*(_DWORD *)v3 )
  {
    v18 = *(_DWORD *)(v3 + 76);
    v19 = (unsigned int)(*(_DWORD *)(v3 + 80) + 7) >> 3;
    v20 = 0;
    v21 = 0;
    v34 = 0;
    v39 = 0;
    v32 = 3;
    if ( v18 != 4 )
      v32 = v18;
    v36 = *(_DWORD *)(v3 + 4);
    v29 = *(_DWORD *)(v3 + 12);
    v31 = *(void **)(v3 + 20);
    memset(v31, 0, v19);
    v40 = 0;
    if ( *(_DWORD *)(v3 + 80) )
    {
      Src = v31;
      Size = 8;
      do
      {
        v22 = *(_DWORD *)(v3 + 28);
        if ( !v22
          || sub_49EF10(
               a1,
               v20 + *(_DWORD *)(v3 + 24),
               v20 + v22,
               *(_DWORD *)(v3 + 56),
               *(_DWORD *)(v3 + 60),
               *(_DWORD *)(v3 + 76)) )
        {
          v39 += 8;
          v34 += 64 * v32;
          *(_BYTE *)Src |= 1 << v21;
        }
        ++v21;
        v20 += 8 * *(_DWORD *)(v3 + 76);
        if ( v21 >= 8 )
        {
          v21 = 0;
          Src = (char *)Src + 1;
        }
        Size += 8;
        v40 = (char *)v40 + 1;
      }
      while ( (unsigned int)v40 < *(_DWORD *)(v3 + 80) );
    }
    v23 = 0;
    Size = 0;
    v40 = 0;
    if ( *(_DWORD *)(v3 + 80) )
    {
      v24 = v29;
      Src = v31;
      do
      {
        if ( ((unsigned __int8)(1 << v23) & *(_BYTE *)Src) != 0 )
        {
          sub_49F040(v24, Size + v36, *(_DWORD *)(v3 + 72), *(_DWORD *)(v3 + 76));
          sub_49F1F0(a1, v39, *(_DWORD *)(v3 + 68));
          v24 += 128;
        }
        ++v23;
        Size += 8 * *(_DWORD *)(v3 + 76);
        if ( v23 >= 8 )
        {
          v23 = 0;
          Src = (char *)Src + 1;
        }
        v40 = (char *)v40 + 1;
      }
      while ( (unsigned int)v40 < *(_DWORD *)(v3 + 80) );
    }
    *(_DWORD *)(v3 + 16) = v34;
    return v28 >= 0;
  }
  if ( *(_DWORD *)v3 == 1 )
  {
    v27 = *(_DWORD *)(v3 + 88);
    v25 = *(_DWORD *)(v3 + 8);
    v43 = *(_DWORD *)(v3 + 4);
    v40 = *(void **)(v3 + 12);
    sub_49FA80(v40, v3 + 104, v43, v25, v27);
    v15 = *(_DWORD *)(v3 + 108);
    v16 = *(_DWORD *)(v3 + 8);
    v17 = *(_DWORD *)(v3 + 104);
    *(_DWORD *)(v3 + 124) -= v17;
    sub_49FC30((char *)v40 + v17, v3 + 124, v43, v16, v15);
    *(_DWORD *)(v3 + 16) = *(_DWORD *)(v3 + 124) + *(_DWORD *)(v3 + 104);
    return v2 >= 0;
  }
  if ( *(_DWORD *)v3 != 2 )
    return v28 >= 0;
  v4 = *(_DWORD *)(v3 + 84);
  v5 = v4 * *(_DWORD *)(v3 + 80);
  v6 = *(_DWORD *)(v3 + 48);
  v7 = *(_DWORD *)(v3 + 32);
  v40 = *(void **)(v3 + 12);
  v8 = *(void **)(v3 + 36);
  v39 = v6;
  v9 = *(_DWORD *)(v3 + 4);
  Src = v8;
  v10 = (unsigned int)(v5 + 7) >> 3;
  v35 = v9;
  v33 = *(_DWORD **)(v3 + 40);
  if ( *(_DWORD *)(v3 + 28) )
    v41 = a1[41];
  else
    v41 = 1;
  if ( v41 )
  {
    if ( v41 == 1 )
    {
      sub_4A0720(v40, v3 + 16, v9, *(_DWORD *)(v3 + 8), *(_DWORD *)(v3 + 64), *(_DWORD *)(v3 + 72));
      *(_DWORD *)(v3 + 128) = 1;
      return v28 >= 0;
    }
    if ( v41 == 2 )
    {
      sub_4A04C0(
        v7 + v10,
        &v39,
        v7,
        v9,
        *(_DWORD *)(v3 + 24),
        *(_DWORD *)(v3 + 28),
        *(_DWORD *)(v3 + 52),
        *(_DWORD *)(v3 + 56),
        *(_DWORD *)(v3 + 60),
        *(_DWORD *)(v3 + 72),
        *(_DWORD *)(v3 + 80),
        v4);
      v30 = v10 + v39;
      sub_4A0A20((char *)v40 + 4, v10 + v39);
      *(_DWORD *)v40 = v30;
      *(_DWORD *)(v3 + 16) += 4;
      *(_DWORD *)(v3 + 128) = 2;
      return v28 >= 0;
    }
  }
  else
  {
    v26 = *(_DWORD *)(v3 + 72);
    v12 = *(_DWORD *)(v3 + 8);
    Size = *(_DWORD *)(v3 + 16);
    sub_4A0720(Src, &Size, v9, v12, *(_DWORD *)(v3 + 64), v26);
    v42 = *(_DWORD *)(v3 + 16);
    sub_4A04C0(
      v7 + v10,
      &v39,
      v7,
      v35,
      *(_DWORD *)(v3 + 24),
      *(_DWORD *)(v3 + 28),
      *(_DWORD *)(v3 + 52),
      *(_DWORD *)(v3 + 56),
      *(_DWORD *)(v3 + 60),
      *(_DWORD *)(v3 + 72),
      *(_DWORD *)(v3 + 80),
      *(_DWORD *)(v3 + 84));
    v13 = v39 + v10;
    sub_4A0A20(v33 + 1, v13);
    v14 = Size;
    *v33 = v13;
    if ( v14 <= v42 + 4 )
    {
      memcpy_0(v40, Src, v14);
      *(_DWORD *)(v3 + 128) = 1;
      *(_DWORD *)(v3 + 16) = v14;
      return v28 >= 0;
    }
    memcpy_0(v40, v33, v42 + 4);
    *(_DWORD *)(v3 + 16) = v42 + 4;
    v41 = 2;
  }
  *(_DWORD *)(v3 + 128) = v41;
  return v28 >= 0;
}

// ===== sub_49CBC0 @ 0x0049CBC0..0x0049CBCE =====
BOOL __cdecl sub_49CBC0(int *a1)
{
  return sub_49C7B0(a1);
}

// ===== sub_49CBD0 @ 0x0049CBD0..0x0049CC0D =====
int __userpurge sub_49CBD0@<eax>(int a1@<eax>, unsigned int a2@<edi>, int a3, int a4)
{
  int result; // eax

  result = sub_49B210(a1);
  if ( !result )
  {
    if ( a2 < *(_DWORD *)(a1 + 40) )
      return sub_49CC40(a4, a1, a1 + 64);
    else
      return 10;
  }
  return result;
}

// ===== sub_49CC10 @ 0x0049CC10..0x0049CC38 =====
int __userpurge sub_49CC10@<eax>(int a1@<eax>, int a2, int a3, int a4, int a5)
{
  int result; // eax

  result = sub_49B210(a1);
  if ( !result )
    return sub_49CC40(a3, a1, a4);
  return result;
}

// ===== sub_49CC40 @ 0x0049CC40..0x0049D332 =====
int __fastcall sub_49CC40(int a1, int a2, int **a3, _DWORD *a4, int a5)
{
  int v6; // edi
  int v7; // eax
  unsigned int v8; // ebx
  int v9; // eax
  int v10; // eax
  unsigned int v11; // eax
  char *v12; // edi
  _DWORD *v13; // eax
  _DWORD *v14; // eax
  int v15; // edx
  unsigned int v16; // edx
  int v17; // ebx
  int v18; // ecx
  int v19; // edx
  int v20; // ebx
  int v21; // eax
  int v22; // ecx
  int v23; // eax
  _DWORD *v24; // ecx
  int v25; // eax
  unsigned int v26; // ecx
  int v27; // ecx
  unsigned int v28; // ecx
  int v29; // eax
  int **v30; // eax
  int v31; // edi
  int v33; // [esp+0h] [ebp-3764h] BYREF
  unsigned int v34; // [esp+14h] [ebp-3750h]
  int v35; // [esp+18h] [ebp-374Ch] BYREF
  int pExceptionObject; // [esp+1Ch] [ebp-3748h] BYREF
  int v37; // [esp+20h] [ebp-3744h] BYREF
  int v38; // [esp+24h] [ebp-3740h]
  void *Block; // [esp+28h] [ebp-373Ch]
  int v40; // [esp+2Ch] [ebp-3738h]
  int v41; // [esp+30h] [ebp-3734h]
  int v42; // [esp+34h] [ebp-3730h]
  unsigned int v43; // [esp+38h] [ebp-372Ch]
  int v44; // [esp+3Ch] [ebp-3728h]
  unsigned int v45; // [esp+40h] [ebp-3724h]
  unsigned int v46; // [esp+44h] [ebp-3720h]
  int v47; // [esp+48h] [ebp-371Ch]
  void *v48; // [esp+4Ch] [ebp-3718h]
  int v49; // [esp+50h] [ebp-3714h]
  int v50; // [esp+54h] [ebp-3710h]
  char *v51; // [esp+58h] [ebp-370Ch]
  unsigned int v52; // [esp+5Ch] [ebp-3708h]
  int v53; // [esp+60h] [ebp-3704h]
  _DWORD *v54; // [esp+64h] [ebp-3700h]
  int v55; // [esp+68h] [ebp-36FCh]
  int v56; // [esp+6Ch] [ebp-36F8h] BYREF
  int *v57; // [esp+70h] [ebp-36F4h]
  int v58; // [esp+74h] [ebp-36F0h]
  int v59; // [esp+78h] [ebp-36ECh]
  unsigned int v60; // [esp+7Ch] [ebp-36E8h]
  _BYTE v61[8424]; // [esp+80h] [ebp-36E4h] BYREF
  _BYTE v62[1024]; // [esp+2168h] [ebp-15FCh] BYREF
  _BYTE v63[1024]; // [esp+2568h] [ebp-11FCh] BYREF
  _BYTE v64[1024]; // [esp+2968h] [ebp-DFCh] BYREF
  _BYTE v65[1728]; // [esp+2D68h] [ebp-9FCh] BYREF
  _BYTE v66[808]; // [esp+3428h] [ebp-33Ch] BYREF
  int *v67; // [esp+3754h] [ebp-10h]
  int v68; // [esp+3760h] [ebp-4h]

  v67 = &v33;
  v50 = a2;
  v54 = a4;
  v59 = a1;
  v48 = 0;
  Block = 0;
  v68 = 0;
  sub_49B470(a5, a2);
  sub_49B550(a5 + 64, a2);
  v6 = 0;
  v58 = 0;
  v40 = a4[7] >> 3;
  v7 = a4[5];
  v41 = 4 * v7;
  v43 = (v7 + 7) & 0xFFFFFFF8;
  v8 = (a4[6] + 7) & 0xFFFFFFF8;
  v49 = v8 * 4 * v43;
  v38 = v43 * v8;
  v45 = v43 >> 3;
  v52 = v8 >> 3;
  v46 = ((v43 >> 3) + 7) >> 3;
  v60 = 0;
  while ( v60 < 0x10 )
  {
    v9 = sub_49FA40(&v56);
    *(_DWORD *)&v66[4 * v60 + 744] = v9;
    v58 += v56;
    ++v60;
    v6 = v58;
  }
  v60 = 0;
  while ( v60 < 0xB0 )
  {
    v10 = sub_49FA40(&v56);
    *(_DWORD *)&v65[4 * v60 + 1024] = v10;
    v58 += v56;
    ++v60;
    v6 = v58;
  }
  v44 = sub_4A0360(v66);
  v42 = sub_4A0360(v61);
  sub_4A01E0(a2, v64, v62, v66);
  sub_4A01E0(a2, v63, v65, v61);
  v11 = v52 + 1;
  v34 = v52 + 1;
  *(_DWORD *)(a2 + 24) = v52 + 1;
  v55 = v6 + v59;
  v58 = v6 + 4 * v11;
  v12 = (char *)_aligned_malloc(6 * v38, 0x10u);
  Block = v12;
  if ( !v12 )
  {
    pExceptionObject = 11;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  v13 = operator new[](132 * v34);
  *(_DWORD *)(a2 + 20) = v13;
  v57 = *a3;
  v51 = v12;
  *v13 = 2;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 8) = -1;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 16) = v49;
  v14 = v54;
  if ( v54[7] == 32 )
    v15 = v59 + *(_DWORD *)(v55 + 4 * v52);
  else
    v15 = 0;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 4) = v15;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 12) = v57;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 24) = 0;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 28) = 0;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 52) = v14[5];
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 56) = v14[6];
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 60) = v41;
  v16 = v43;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 64) = v43;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 68) = v8;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 72) = 4 * v16;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 76) = v40;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 80) = v45;
  *(_DWORD *)(*(_DWORD *)(a2 + 20) + 84) = v52;
  v17 = 1;
  v47 = 1;
  v18 = 0;
  v53 = 0;
  while ( 1 )
  {
    v19 = v55;
    if ( v17 >= *(_DWORD *)(a2 + 24) )
      break;
    v20 = 132 * v17;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20)) = 0;
    v49 = v18 + 1;
    if ( v18 + 1 >= *(_DWORD *)(a2 + 24) )
      v21 = -1;
    else
      v21 = *(_DWORD *)(v19 + 4 * v18 + 4) - *(_DWORD *)(v19 + 4 * v18) - v46;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 8) = v21;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 16) = sub_49FA40(&v56);
    v22 = v55;
    v23 = v53;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 4) = v46 + v56 + v59 + *(_DWORD *)(v55 + 4 * v53);
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 12) = v57;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 20) = v59 + *(_DWORD *)(v22 + 4 * v23);
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 24) = 0;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 28) = 0;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 32) = v51;
    v24 = v54;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 52) = v54[5];
    v25 = 8 * v23;
    v26 = v24[6];
    if ( v25 + 8 <= v26 )
      v27 = 8;
    else
      v27 = v26 - v25;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 56) = v27;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 60) = v41;
    v28 = v43;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 64) = v43;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 68) = 8;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 72) = 4 * v28;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 76) = v40;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 80) = v45;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 84) = 1;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 92) = v44;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 88) = v66;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 96) = v64;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 100) = v62;
    *(_DWORD *)(*(_DWORD *)(a2 + 20) + v20 + 104) = *(_DWORD *)(*(_DWORD *)(a2 + 20) + v20 + 8);
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 112) = v42;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 108) = v61;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 116) = v63;
    *(_DWORD *)(v20 + *(_DWORD *)(a2 + 20) + 120) = v65;
    *(_DWORD *)(*(_DWORD *)(a2 + 20) + v20 + 124) = *(_DWORD *)(*(_DWORD *)(a2 + 20) + v20 + 8);
    v57 += 8 * v28;
    v51 += 48 * v28;
    ++v47;
    v18 = v49;
    v53 = v49;
    v17 = v47;
    v14 = v54;
  }
  if ( v14[7] == 32 )
  {
    v29 = v14[4] - 0x10000;
    if ( v29 )
    {
      if ( v29 != 1 )
      {
        v35 = 9;
        _CxxThrowException(&v35, (_ThrowInfo *)&_TI1K);
      }
      v30 = (int **)(*(_DWORD *)(a2 + 20) + 4);
      v57 = *v30;
      v31 = *v57;
      *v30 = v57 + 1;
    }
    else
    {
      v31 = 1;
    }
    if ( v31 != 1 )
    {
      if ( v31 != 2 )
      {
        v37 = 9;
        _CxxThrowException(&v37, (_ThrowInfo *)&_TI1K);
      }
      v48 = operator new[](2 * v38);
    }
    *(_DWORD *)(*(_DWORD *)(a2 + 20) + 32) = v48;
    *(_DWORD *)(*(_DWORD *)(a2 + 20) + 128) = v31;
  }
  sub_49EEE0(0);
  sub_4464C0(*(_DWORD *)(a2 + 16), (int)sub_49EED0, a2);
  sub_4464E0(1, *(_DWORD *)(a2 + 16));
  sub_4464C0(*(_DWORD *)(a2 + 16), 0, 0);
  v68 = -1;
  operator delete[](*(void **)(a2 + 20));
  operator delete[](v48);
  _aligned_free(Block);
  return 0;
}

// ===== sub_49D340 @ 0x0049D340..0x0049D4B7 =====
BOOL __stdcall sub_49D340(int a1)
{
  int v1; // ebx
  int v2; // esi
  int v3; // edi
  _DWORD *v4; // esi
  _DWORD *v5; // eax
  int v6; // edx
  unsigned int v8; // eax
  int v9; // edi
  int v10; // [esp-18h] [ebp-2Ch]
  int v11; // [esp-Ch] [ebp-20h]
  int v12; // [esp-8h] [ebp-1Ch]
  int v13; // [esp-4h] [ebp-18h]
  int v14; // [esp+Ch] [ebp-8h]
  int v15; // [esp+10h] [ebp-4h]

  v1 = a1;
  v2 = sub_4465D0(*(_DWORD *)(a1 + 16));
  v3 = sub_49EEF0(v1);
  v14 = v3;
  sub_4465F0(*(_DWORD *)(v1 + 16), v2);
  if ( v3 < 0 )
    return v14 >= 0;
  v4 = (_DWORD *)(*(_DWORD *)(v1 + 20) + 132 * v3);
  if ( !*v4 )
  {
    if ( v4[4] )
    {
      v9 = v4[1];
      v13 = v4[25];
      v12 = v4[24];
      v11 = v4[22];
      v10 = v4[4];
      a1 = v4[8];
      sub_49FB40(a1, v10, v9, v4 + 26, v11, v12, v13);
      sub_49FD70(a1, v4[4], v9 + v4[26], v4 + 31, v4[27], v4[29], v4[30]);
      sub_49D4C0(v4);
    }
    return v14 >= 0;
  }
  if ( *v4 != 2 )
    return v14 >= 0;
  v5 = (_DWORD *)v4[1];
  v6 = v4[3];
  v15 = v6;
  a1 = v4[8];
  if ( v5 )
  {
    if ( v4[32] == 1 )
    {
      sub_4A0930(v6, v4[4], v5, v4 + 2, v4[18]);
      return v3 >= 0;
    }
    if ( v4[32] == 2 )
    {
      sub_4A0B90(v1, *v5, v4 + 2);
      sub_4A0650(v15, a1, &a1, v4[18], v4[20]);
      return v3 >= 0;
    }
    return v14 >= 0;
  }
  v8 = 3;
  if ( v4[4] <= 3u )
    return v14 >= 0;
  do
  {
    *(_BYTE *)(v8 + v6) = 0;
    v8 += 4;
  }
  while ( v8 < v4[4] );
  return v3 >= 0;
}

// ===== sub_49D4C0 @ 0x0049D4C0..0x0049D4F9 =====
int __usercall sub_49D4C0@<eax>(int a1@<esi>)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax
  int result; // eax

  v1 = sub_49B280();
  if ( !v1 )
    return sub_49D500(a1, v2);
  v3 = v1 - 1;
  if ( !v3 )
    return sub_49DE30();
  result = v3 - 1;
  if ( !result )
    return sub_49E6B0();
  return result;
}

// ===== sub_49D500 @ 0x0049D500..0x0049DE2A =====
unsigned int __fastcall sub_49D500(int a1, _DWORD *a2)
{
  _DWORD *v2; // esi
  unsigned int v3; // eax
  int v4; // ebx
  int v6; // ecx
  int v7; // eax
  int v8; // eax
  bool v9; // zf
  int v10; // edx
  __int16 *v11; // ecx
  unsigned int result; // eax
  _BYTE *v13; // eax
  double v14; // st5
  unsigned int v15; // ebx
  int v16; // esi
  float *v17; // eax
  unsigned int v18; // edx
  __int16 *v19; // ecx
  int v20; // esi
  _BYTE *v21; // edi
  char *v22; // esi
  double v23; // st4
  _BYTE *v24; // ebx
  double v25; // st4
  double v26; // st4
  double v27; // st3
  int v28; // ebx
  double v29; // st4
  double v30; // st3
  int v31; // ebx
  double v32; // st4
  int v33; // ebx
  double v34; // st4
  double v35; // st3
  int v36; // ebx
  double v37; // st4
  double v38; // st3
  int v39; // ebx
  double v40; // st4
  double v41; // st3
  double v42; // st4
  double v43; // st5
  double v44; // st4
  double v45; // st3
  double v46; // st2
  double v47; // rt2
  _BYTE *v48; // ebx
  double v49; // st3
  int v50; // ebx
  double v51; // st4
  double v52; // st5
  double v53; // st4
  unsigned int v54; // ebx
  float *v55; // esi
  _WORD *v56; // edi
  double v57; // st3
  double v58; // st2
  double v59; // st4
  double v60; // st0
  double v61; // st1
  int v62; // eax
  double v63; // st4
  double v64; // st3
  double v65; // st2
  int v66; // ebx
  _BYTE *v67; // edi
  __int16 *v68; // esi
  int v69; // edx
  float *v70; // eax
  int v71; // ecx
  float *v72; // edx
  int v73; // ecx
  _DWORD v74[3]; // [esp+Ch] [ebp-1B8h]
  __int16 *v75; // [esp+18h] [ebp-1ACh]
  int v76; // [esp+1Ch] [ebp-1A8h]
  _DWORD *v77; // [esp+20h] [ebp-1A4h]
  float v78; // [esp+24h] [ebp-1A0h]
  float v79; // [esp+28h] [ebp-19Ch]
  float v80; // [esp+2Ch] [ebp-198h]
  float v81; // [esp+30h] [ebp-194h]
  float v82; // [esp+38h] [ebp-18Ch]
  unsigned int v83; // [esp+44h] [ebp-180h]
  float v84; // [esp+48h] [ebp-17Ch]
  float v85; // [esp+4Ch] [ebp-178h]
  float v86; // [esp+50h] [ebp-174h]
  float v87; // [esp+54h] [ebp-170h]
  _BYTE *v88; // [esp+68h] [ebp-15Ch]
  unsigned int v89; // [esp+6Ch] [ebp-158h]
  _BYTE *v90; // [esp+70h] [ebp-154h]
  int v91; // [esp+74h] [ebp-150h]
  int v92; // [esp+78h] [ebp-14Ch]
  __int16 *v93; // [esp+7Ch] [ebp-148h]
  __int16 *v94; // [esp+80h] [ebp-144h]
  float *v95; // [esp+84h] [ebp-140h]
  unsigned int v96; // [esp+88h] [ebp-13Ch]
  int v97; // [esp+8Ch] [ebp-138h]
  unsigned int v98; // [esp+90h] [ebp-134h]
  float v99; // [esp+94h] [ebp-130h]
  float v100; // [esp+98h] [ebp-12Ch]
  float v101; // [esp+9Ch] [ebp-128h]
  float v102; // [esp+A0h] [ebp-124h]
  float v103; // [esp+A4h] [ebp-120h]
  float v104; // [esp+A8h] [ebp-11Ch]
  float v105; // [esp+ACh] [ebp-118h]
  float v106; // [esp+B0h] [ebp-114h]
  _BYTE *v107; // [esp+B4h] [ebp-110h]
  int v108; // [esp+B8h] [ebp-10Ch]
  int v109; // [esp+BCh] [ebp-108h]
  float v110[4]; // [esp+C0h] [ebp-104h] BYREF
  _BYTE v111[240]; // [esp+D0h] [ebp-F4h] BYREF

  v2 = a2;
  v3 = a2[19];
  v4 = a2[3];
  v6 = a2[8];
  v77 = a2;
  if ( v3 == 4 )
  {
    v96 = 3;
LABEL_3:
    v7 = (a2[4] >> 3) / v96;
    v95 = (float *)&unk_561390;
    v8 = a2[17] * v7;
    goto LABEL_4;
  }
  v96 = v3;
  if ( v3 != 1 )
    goto LABEL_3;
  v8 = 0;
  v95 = (float *)&unk_564A90;
LABEL_4:
  v9 = a2[20] == 0;
  v10 = a2[15];
  v92 = v6;
  v11 = (__int16 *)(v6 + 2 * v8);
  v76 = v10;
  v94 = &v11[v8];
  v74[0] = a1 + 176;
  result = a1 + 432;
  v89 = 0;
  v93 = v11;
  v74[1] = a1 + 432;
  v74[2] = a1 + 432;
  v83 = 0;
  v91 = 1;
  if ( !v9 )
  {
    v13 = (_BYTE *)v2[5];
    v14 = 1.0823922;
    v90 = (_BYTE *)(v4 + 1);
    v88 = v13;
    do
    {
      if ( ((unsigned __int8)v91 & *v88) != 0 )
      {
        v15 = 0;
        v98 = 0;
        if ( v96 )
        {
          do
          {
            v16 = v74[v15];
            v17 = (float *)(v16 + 128);
            v18 = 0;
            v19 = (__int16 *)(*(&v92 + v15) + 64);
            v20 = -v16;
            v107 = &v111[v20 + 48];
            v21 = &v111[v20 + 80];
            v108 = (int)&v111[v20 + 16];
            v22 = (char *)v110 + v20;
            do
            {
              if ( *(v19 - 24) || *(v19 - 16) || *(v19 - 8) || *v19 || v19[8] || v19[16] || v19[24] )
              {
                v26 = *(v17 - 32);
                v109 = *(v19 - 32);
                v27 = (double)v109;
                v109 = *(v19 - 16);
                v28 = *v19;
                v99 = v26 * v27;
                v29 = *(v17 - 16);
                v30 = (double)v109;
                v109 = v28;
                v31 = v19[16];
                v100 = v29 * v30;
                v32 = (double)v109;
                v109 = v31;
                v33 = *(v19 - 24);
                v101 = v32 * *v17;
                v34 = v17[16];
                v35 = (double)v109;
                v109 = v33;
                v36 = *(v19 - 8);
                v102 = v34 * v35;
                v84 = v101 + v99;
                v85 = v99 - v101;
                v87 = v102 + v100;
                v86 = (v100 - v102) * 1.414213562 - v87;
                v99 = v87 + v84;
                v102 = v84 - v87;
                v100 = v86 + v85;
                v101 = v85 - v86;
                v37 = *(v17 - 24);
                v38 = (double)v109;
                v109 = v36;
                v39 = v19[8];
                v103 = v37 * v38;
                v40 = *(v17 - 8);
                v41 = (double)v109;
                v109 = v39;
                v104 = v40 * v41;
                v105 = v17[8] * (double)v39;
                v42 = v17[24];
                v109 = v19[24];
                v106 = v42 * (double)v109;
                v81 = v105 + v104;
                v78 = v105 - v104;
                v79 = v106 + v103;
                v80 = v103 - v106;
                v106 = v79 + v81;
                v85 = (v79 - v81) * 1.414213562;
                v82 = (v80 + v78) * 1.847759065;
                v84 = v14 * v80 - v82;
                v86 = v82 - v78 * 2.61312593;
                v43 = v106;
                v105 = v86 - v106;
                v44 = v105;
                v104 = v85 - v105;
                v45 = v104;
                v103 = v104 + v84;
                v46 = v99;
                v110[v18] = v106 + v99;
                v47 = v45;
                *(float *)&v21[(_DWORD)v17] = v46 - v43;
                v48 = v107;
                v49 = v100;
                *(float *)&v111[4 * v18 + 16] = v44 + v100;
                *(float *)&v48[(_DWORD)v17] = v49 - v44;
                v50 = v108;
                v51 = v101;
                *(float *)&v111[4 * v18 + 48] = v47 + v101;
                *(float *)((char *)v17 + v50) = v51 - v47;
                v52 = v103;
                v53 = v102;
                *(float *)&v22[(_DWORD)v17] = v103 + v102;
                *(float *)&v111[4 * v18 + 80] = v53 - v52;
                v14 = 1.0823922;
              }
              else
              {
                v23 = *(v17 - 32);
                v109 = *(v19 - 32);
                v24 = v107;
                *(float *)&v109 = v23 * (double)v109;
                v25 = *(float *)&v109;
                *(float *)&v21[(_DWORD)v17] = *(float *)&v109;
                *(float *)&v24[(_DWORD)v17] = v25;
                *(float *)((char *)v17 + v108) = v25;
                *(float *)&v22[(_DWORD)v17] = v25;
                *(float *)&v111[4 * v18 + 80] = v25;
                *(float *)&v111[4 * v18 + 48] = v25;
                *(float *)&v111[4 * v18 + 16] = v25;
                v110[v18] = v25;
              }
              ++v18;
              ++v19;
              ++v17;
            }
            while ( v18 < 8 );
            v54 = v98;
            v55 = (float *)v111;
            v56 = (_WORD *)(*(&v92 + v98) + 2);
            v108 = 8;
            do
            {
              v84 = *v55 + *(v55 - 4);
              v85 = *(v55 - 4) - *v55;
              v87 = v55[2] + *(v55 - 2);
              v86 = (*(v55 - 2) - v55[2]) * 1.414213562 - v87;
              v99 = v87 + v84;
              v102 = v84 - v87;
              v100 = v86 + v85;
              v101 = v85 - v86;
              v81 = *(v55 - 1) + v55[1];
              v78 = v55[1] - *(v55 - 1);
              v79 = v55[3] + *(v55 - 3);
              v80 = *(v55 - 3) - v55[3];
              v106 = v79 + v81;
              v85 = (v79 - v81) * 1.414213562;
              v82 = (v80 + v78) * 1.847759065;
              v84 = v80 * v14 - v82;
              v86 = v82 - v78 * 2.61312593;
              v105 = v86 - v106;
              v57 = v105;
              v104 = v85 - v105;
              v103 = v104 + v84;
              v58 = v99 - v106;
              v59 = v104;
              *(v56 - 1) = (unsigned __int8)byte_562510[(int)(v106 + v99) >> 3];
              v60 = v100;
              v56[6] = (unsigned __int8)byte_562510[(int)v58 >> 3];
              *v56 = (unsigned __int8)byte_562510[(int)(v57 + v60) >> 3];
              v61 = v101;
              v56[5] = (unsigned __int8)byte_562510[(int)(v60 - v57) >> 3];
              v56[1] = (unsigned __int8)byte_562510[(int)(v59 + v61) >> 3];
              v62 = (int)(v61 - v59);
              v63 = v103;
              v64 = v103;
              v65 = v102;
              v56[4] = (unsigned __int8)byte_562510[v62 >> 3];
              v56[3] = (unsigned __int8)byte_562510[(int)(v64 + v65) >> 3];
              v56[2] = (unsigned __int8)byte_562510[(int)(v65 - v63) >> 3];
              v56 += 8;
              v55 += 8;
              --v108;
            }
            while ( v108 );
            v15 = v54 + 1;
            v98 = v15;
          }
          while ( v15 < v96 );
          v11 = v93;
        }
        v98 = (char *)v94 - (char *)v11;
        v66 = v92 - (_DWORD)v11;
        v107 = v90;
        v109 = 8;
        while ( 1 )
        {
          v67 = v107;
          v68 = v11;
          v108 = 8;
          v75 = v11 + 8;
          do
          {
            v69 = *(__int16 *)((char *)v68 + v98);
            v97 = *(__int16 *)((char *)v68 + v66);
            v70 = v95;
            v67[1] = byte_562390[(int)(v95[v69] + (double)v97 + 256.0)];
            v71 = *v68;
            v97 = *(__int16 *)((char *)v68 + v66);
            v72 = v95;
            *v67 = byte_562390[(int)(v70[v71 + 256] + (double)v97 + v70[*(__int16 *)((char *)v68 + v98) + 512] + 256.0)];
            v73 = *v68;
            v97 = *(__int16 *)((char *)v68 + v66);
            *(v67 - 1) = byte_562390[(int)(v72[v73 + 768] + (double)v97 + 256.0)];
            ++v68;
            v67 += 4;
            --v108;
          }
          while ( v108 );
          v107 += v76;
          --v109;
          if ( *(float *)&v109 == 0.0 )
            break;
          v11 = v75;
        }
        v2 = v77;
        v93 += 64;
        v92 += 128;
        v94 += 64;
        v11 = v93;
      }
      v90 += 32;
      ++v89;
      v91 = __ROL4__(v91, 1);
      if ( v89 >= 8 )
      {
        ++v88;
        v89 = 0;
        v91 = 1;
      }
      result = v83 + 1;
      v83 = result;
    }
    while ( result < v2[20] );
  }
  return result;
}

// ===== sub_49DE30 @ 0x0049DE30..0x0049E6A6 =====
int __usercall sub_49DE30@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3@<ebp>)
{
  __m64 *v3; // edx
  int v5; // eax
  unsigned __int8 *v6; // edi
  int v7; // edx
  int v8; // eax
  unsigned __int8 v9; // cl
  __m64 *v10; // esi
  char *v11; // edi
  int result; // eax
  __m64 v13; // mm6
  char v14; // cf
  __m128 *v15; // edx
  int v16; // eax
  __m128 *v17; // ebx
  int v18; // ecx
  __m64 v19; // mm3
  __m128 v20; // xmm0
  __m128 v21; // xmm0
  __m128 v22; // xmm1
  __m128 v23; // xmm2
  __m128 v24; // xmm3
  __m128 v25; // xmm5
  __m128 v26; // xmm0
  __m128 v27; // xmm6
  __m128 v28; // xmm1
  __m128 v29; // xmm0
  __m128 v30; // xmm1
  __m128 v31; // xmm2
  __m128 v32; // xmm3
  __m128 v33; // xmm5
  __m128 v34; // xmm2
  __m128 v35; // xmm6
  __m128 v36; // xmm0
  __m128 v37; // xmm7
  __m128 v38; // xmm1
  __m128 v39; // xmm2
  __m128 v40; // xmm6
  __m128 v41; // xmm0
  __m128 v42; // xmm3
  __m128 v43; // xmm5
  __m128 v44; // xmm3
  __m128 v45; // xmm5
  __m64 *v46; // esi
  __m128 *v47; // edx
  int v48; // ecx
  __m128 v49; // xmm0
  __m128 v50; // xmm1
  __m128 v51; // xmm2
  __m128 v52; // xmm3
  __m128 v53; // xmm4
  __m128 v54; // xmm0
  __m128 v55; // xmm5
  __m128 v56; // xmm1
  __m128 v57; // xmm0
  __m128 v58; // xmm1
  __m128 v59; // xmm2
  __m128 v60; // xmm3
  __m128 v61; // xmm5
  __m128 v62; // xmm2
  __m128 v63; // xmm6
  __m128 v64; // xmm0
  __m128 v65; // xmm7
  __m128 v66; // xmm1
  __m128 v67; // xmm2
  __m128 v68; // xmm6
  __m128 v69; // xmm0
  __m128 v70; // xmm1
  __m128 v71; // xmm3
  __m64 v72; // mm0
  __m128 v73; // xmm4
  __m128 v74; // xmm5
  __m64 v75; // mm2
  __m128 v76; // xmm1
  __m128 v77; // xmm7
  __m64 v78; // mm4
  __m128 v79; // xmm2
  __m128 v80; // xmm4
  __m64 v81; // mm1
  __m64 v82; // mm5
  __m64 v83; // mm3
  __m64 v84; // mm2
  __m64 v85; // mm3
  __m64 v86; // mm4
  __m64 v87; // mm4
  __m64 v88; // mm1
  __m64 v89; // mm5
  __m64 v90; // mm0
  __m64 v91; // mm5
  __m64 v92; // mm3
  __m64 v93; // mm2
  __m64 v94; // mm3
  __m64 *v95; // esi
  __m64 v96; // mm0
  __m64 v97; // mm1
  __m64 v98; // mm2
  unsigned int v99; // eax
  __m128 v100; // xmm0
  __m128 v101; // xmm3
  __m128 v102; // xmm0
  __m128 v103; // xmm3
  __m128 v104; // xmm0
  __m128 v105; // xmm3
  unsigned int v106; // eax
  __m128 v107; // xmm1
  __m128 v108; // xmm4
  __m128 v109; // xmm1
  __m128 v110; // xmm4
  __m128 v111; // xmm2
  __m128 v112; // xmm5
  __m128 v113; // xmm7
  __m128 v114; // xmm0
  __m128 v115; // xmm1
  __m128 v116; // xmm4
  __m64 v117; // mm3
  __m64 v118; // mm3
  __m64 v119; // mm3
  __m64 v120; // mm3
  __m64 v121; // mm4
  __m64 v122; // mm4
  __m64 v123; // mm4
  __m64 v124; // mm4
  __m64 v125; // mm5
  __m64 v126; // mm5
  __m64 v127; // mm5
  __m64 v128; // mm5
  char *v129; // edi
  __m64 v130; // mm3
  __m64 v131; // mm3
  __m64 v132; // mm3
  __m64 v133; // mm3
  __m64 v134; // mm4
  __m64 v135; // mm4
  __m64 v136; // mm4
  __m64 v137; // mm4
  __m64 v138; // mm5
  __m64 v139; // mm5
  __m64 v140; // mm5
  __m64 v141; // mm5
  int v142; // [esp-194h] [ebp-1A0h]
  int *v143; // [esp-190h] [ebp-19Ch]
  int v144; // [esp-180h] [ebp-18Ch] BYREF
  __m128 v145; // [esp-80h] [ebp-8Ch]
  __m128 v146; // [esp-70h] [ebp-7Ch]
  __m128 v147; // [esp-60h] [ebp-6Ch]
  __m128 v148; // [esp-50h] [ebp-5Ch]
  _DWORD v149[4]; // [esp-40h] [ebp-4Ch]
  __m64 *v150; // [esp-30h] [ebp-3Ch]
  int v151; // [esp-2Ch] [ebp-38h]
  int v152; // [esp-28h] [ebp-34h]
  unsigned int v153; // [esp-24h] [ebp-30h]
  int v154; // [esp-20h] [ebp-2Ch]
  unsigned int *v155; // [esp-1Ch] [ebp-28h]
  int v156; // [esp-18h] [ebp-24h]
  int v157; // [esp-14h] [ebp-20h]
  int v158; // [esp-10h] [ebp-1Ch]
  char *v159; // [esp-Ch] [ebp-18h]
  unsigned __int8 *v160; // [esp-8h] [ebp-14h]
  unsigned __int8 v161; // [esp-1h] [ebp-Dh]
  int v162; // [esp+0h] [ebp-Ch]
  void *v163; // [esp+4h] [ebp-8h]
  int v164; // [esp+8h] [ebp-4h] BYREF
  void *retaddr; // [esp+Ch] [ebp+0h]

  v162 = a3;
  v163 = retaddr;
  v3 = (__m64 *)a2[8];
  v159 = (char *)a2[3];
  v5 = a2[19];
  v6 = (unsigned __int8 *)a2[5];
  v150 = v3;
  v160 = v6;
  if ( v5 == 4 )
  {
    v156 = 3;
  }
  else
  {
    v156 = v5;
    if ( v5 == 1 )
    {
      v153 = 0;
      v155 = (unsigned int *)&unk_564A90;
      goto LABEL_4;
    }
  }
  v153 = 2 * a2[17] * ((unsigned int)((2863311531u * (unsigned __int64)(a2[4] >> 3)) >> 32) >> 1);
  v155 = (unsigned int *)&unk_561390;
LABEL_4:
  v7 = a2[20];
  v149[3] = a1 + 176;
  v149[2] = a1 + 432;
  v149[1] = a1 + 432;
  v8 = a2[15];
  v158 = v7;
  v9 = *v6;
  v151 = v8;
  v161 = v9;
  v157 = 8;
  v143 = &v164;
  v10 = v150;
  v11 = v159;
  result = -2139062144;
  v13 = _m_punpcklbw(_mm_cvtsi32_si64(0x80808080), 0LL);
  while ( 1 )
  {
    v14 = v161 & 1;
    v161 >>= 1;
    if ( v14 )
      break;
    if ( !--v158 )
      goto LABEL_30;
LABEL_28:
    v159 += 32;
    v11 = v159;
    if ( !--v157 )
    {
      result = (int)++v160;
      v161 = *v160;
      v157 = 8;
    }
  }
  v15 = (__m128 *)&v144;
  v16 = v156;
  while ( 1 )
  {
    v17 = (__m128 *)v149[v16];
    v18 = 2;
    v142 = v16;
    while ( 1 )
    {
      if ( v10[4].m64_i32[0] | v10[2].m64_i32[0]
        || (v19 = _m_por(
                    _m_por(_m_por(v10[4], v10[8]), v10[12]),
                    _m_por(_m_por(_m_por(v10[2], v10[6]), v10[10]), v10[14])),
            _mm_cvtsi64_si32(_m_packsswb(v19, v19))) )
      {
        v21 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd((__m64)v10->m64_u64, (__m64)v10->m64_u64), 0x10u)), *v17);
        v22 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[4], v10[4]), 0x10u)), v17[4]);
        v23 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[8], v10[8]), 0x10u)), v17[8]);
        v24 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[12], v10[12]), 0x10u)), v17[12]);
        v25 = _mm_add_ps(v21, v23);
        v26 = _mm_sub_ps(v21, v23);
        v27 = _mm_add_ps(v22, v24);
        v28 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v22, v24), (__m128)xmmword_5084C0), v27);
        *v15 = _mm_add_ps(v25, v27);
        v15[6] = _mm_sub_ps(v25, v27);
        v15[2] = _mm_add_ps(v26, v28);
        v15[4] = _mm_sub_ps(v26, v28);
        v29 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[2], v10[2]), 0x10u)), v17[2]);
        v30 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[6], v10[6]), 0x10u)), v17[6]);
        v31 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[10], v10[10]), 0x10u)), v17[10]);
        v32 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd(v10[14], v10[14]), 0x10u)), v17[14]);
        v33 = _mm_add_ps(v31, v30);
        v34 = _mm_sub_ps(v31, v30);
        v35 = _mm_add_ps(v29, v32);
        v36 = _mm_sub_ps(v29, v32);
        v37 = _mm_add_ps(v35, v33);
        v38 = _mm_mul_ps(_mm_add_ps(v34, v36), (__m128)xmmword_5084D0);
        v39 = _mm_sub_ps(_mm_add_ps(_mm_mul_ps(v34, (__m128)xmmword_5084F0), v38), v37);
        v40 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v35, v33), (__m128)xmmword_5084C0), v39);
        v41 = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v36, (__m128)xmmword_5084E0), v38), v40);
        v42 = *v15;
        *v15 = _mm_add_ps(*v15, v37);
        v15[14] = _mm_sub_ps(v42, v37);
        v43 = v15[2];
        v15[2] = _mm_add_ps(v43, v39);
        v15[12] = _mm_sub_ps(v43, v39);
        v44 = v15[4];
        v15[4] = _mm_add_ps(v44, v40);
        v15[10] = _mm_sub_ps(v44, v40);
        v45 = v15[6];
        v15[8] = _mm_add_ps(v45, v41);
        v15[6] = _mm_sub_ps(v45, v41);
      }
      else
      {
        v20 = _mm_mul_ps(_mm_cvt_pi32ps(_m_psradi(_m_punpcklwd((__m64)v10->m64_u64, (__m64)v10->m64_u64), 0x10u)), *v17);
        *v15 = v20;
        v15[2] = v20;
        v15[4] = v20;
        v15[6] = v20;
        v15[8] = v20;
        v15[10] = v20;
        v15[12] = v20;
        v15[14] = v20;
      }
      if ( !--v18 )
        break;
      ++v10;
      ++v15;
      ++v17;
    }
    v46 = v10 - 1;
    v47 = v15 - 1;
    v48 = 2;
    while ( 1 )
    {
      v49 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47->m128_u32[0], v47[2], 68),
              _mm_shuffle_ps((__m128)v47[4].m128_u32[0], v47[6], 68),
              136);
      v50 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47->m128_u32[2], v47[2], 100),
              _mm_shuffle_ps((__m128)v47[4].m128_u32[2], v47[6], 100),
              136);
      v51 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47[1].m128_u32[0], v47[3], 68),
              _mm_shuffle_ps((__m128)v47[5].m128_u32[0], v47[7], 68),
              136);
      v52 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47[1].m128_u32[2], v47[3], 100),
              _mm_shuffle_ps((__m128)v47[5].m128_u32[2], v47[7], 100),
              136);
      v53 = _mm_add_ps(v49, v51);
      v54 = _mm_sub_ps(v49, v51);
      v55 = _mm_add_ps(v50, v52);
      v56 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v50, v52), (__m128)xmmword_5084C0), v55);
      v145 = _mm_add_ps(v53, v55);
      v148 = _mm_sub_ps(v53, v55);
      v146 = _mm_add_ps(v54, v56);
      v147 = _mm_sub_ps(v54, v56);
      v57 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47->m128_u32[1], v47[2], 84),
              _mm_shuffle_ps((__m128)v47[4].m128_u32[1], v47[6], 84),
              136);
      v58 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47->m128_u32[3], v47[2], 116),
              _mm_shuffle_ps((__m128)v47[4].m128_u32[3], v47[6], 116),
              136);
      v59 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47[1].m128_u32[1], v47[3], 84),
              _mm_shuffle_ps((__m128)v47[5].m128_u32[1], v47[7], 84),
              136);
      v60 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v47[1].m128_u32[3], v47[3], 116),
              _mm_shuffle_ps((__m128)v47[5].m128_u32[3], v47[7], 116),
              136);
      v61 = _mm_add_ps(v59, v58);
      v62 = _mm_sub_ps(v59, v58);
      v63 = _mm_add_ps(v57, v60);
      v64 = _mm_sub_ps(v57, v60);
      v65 = _mm_add_ps(v63, v61);
      v66 = _mm_mul_ps(_mm_add_ps(v62, v64), (__m128)xmmword_5084D0);
      v67 = _mm_sub_ps(_mm_add_ps(_mm_mul_ps(v62, (__m128)xmmword_5084F0), v66), v65);
      v68 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v63, v61), (__m128)xmmword_5084C0), v67);
      v69 = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v64, (__m128)xmmword_5084E0), v66), v68);
      v70 = _mm_add_ps(v145, v65);
      v71 = _mm_sub_ps(v145, v65);
      v72 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v70), _mm_cvtt_ps2pi(_mm_movehl_ps(v70, v70))), 3u), v13),
                0LL),
              0LL);
      v73 = _mm_add_ps(v146, v67);
      v74 = _mm_sub_ps(v146, v67);
      v75 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v73), _mm_cvtt_ps2pi(_mm_movehl_ps(v73, v73))), 3u), v13),
                0LL),
              0LL);
      v76 = _mm_add_ps(v147, v68);
      v77 = _mm_sub_ps(v147, v68);
      v78 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v76), _mm_cvtt_ps2pi(_mm_movehl_ps(v76, v76))), 3u), v13),
                0LL),
              0LL);
      v79 = _mm_add_ps(v148, v69);
      v80 = _mm_sub_ps(v148, v69);
      v81 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v80), _mm_cvtt_ps2pi(_mm_movehl_ps(v80, v80))), 3u), v13),
                0LL),
              0LL);
      v82 = _m_punpckhwd(v72, v78);
      v83 = v75;
      v84 = _m_punpcklwd(v75, v81);
      v85 = _m_punpckhwd(v83, v81);
      v86 = _m_punpcklwd(v72, v78);
      v46->m64_u64 = (unsigned __int64)_m_punpcklwd(v86, v84);
      v46[2].m64_u64 = (unsigned __int64)_m_punpckhwd(v86, v84);
      v46[4].m64_u64 = (unsigned __int64)_m_punpcklwd(v82, v85);
      v46[6].m64_u64 = (unsigned __int64)_m_punpckhwd(v82, v85);
      v87 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v74), _mm_cvtt_ps2pi(_mm_movehl_ps(v74, v74))), 3u), v13),
                0LL),
              0LL);
      v88 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v71), _mm_cvtt_ps2pi(_mm_movehl_ps(v71, v71))), 3u), v13),
                0LL),
              0LL);
      v89 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v79), _mm_cvtt_ps2pi(_mm_movehl_ps(v79, v79))), 3u), v13),
                0LL),
              0LL);
      v90 = _m_punpcklwd(v89, v87);
      v91 = _m_punpckhwd(v89, v87);
      v92 = _m_punpcklbw(
              _m_packuswb(
                _m_paddw(_m_psrawi(_m_packssdw(_mm_cvtt_ps2pi(v77), _mm_cvtt_ps2pi(_mm_movehl_ps(v77, v77))), 3u), v13),
                0LL),
              0LL);
      v93 = _m_punpcklwd(v92, v88);
      v94 = _m_punpckhwd(v92, v88);
      v46[1].m64_u64 = (unsigned __int64)_m_punpcklwd(v90, v93);
      v46[3].m64_u64 = (unsigned __int64)_m_punpckhwd(v90, v93);
      v46[5].m64_u64 = (unsigned __int64)_m_punpcklwd(v91, v94);
      v46[7].m64_u64 = (unsigned __int64)_m_punpckhwd(v91, v94);
      if ( !--v48 )
        break;
      v47 += 8;
      v46 += 8;
    }
    --v16;
    if ( v142 == 1 )
      break;
    v10 = (__m64 *)((char *)v46 + v153 - 64);
    v15 = v47 - 8;
  }
  v95 = (__m64 *)((char *)v46 - 2 * v153 - 64);
  v152 = 8;
  v96 = _m_punpcklwd(_mm_cvtsi32_si64(0xFF00FFu), 0LL);
  v97 = _m_pslldi(v96, 8u);
  v98 = _m_pslldi(v96, 0x10u);
  while ( 1 )
  {
    v154 = 2;
    while ( 1 )
    {
      v99 = _mm_cvtsi64_si32(_m_packuswb(*(__m64 *)((char *)v95 + 2 * v153), *(__m64 *)((char *)v95 + 2 * v153)));
      v100 = (__m128)v155[(unsigned __int8)v99];
      v101 = (__m128)v155[(unsigned __int8)v99 + 512];
      v99 >>= 8;
      v102 = _mm_shuffle_ps(v100, (__m128)v155[(unsigned __int8)v99], 68);
      v103 = _mm_shuffle_ps(v101, (__m128)v155[(unsigned __int8)v99 + 512], 68);
      v99 >>= 8;
      v104 = _mm_shuffle_ps(v102, _mm_shuffle_ps((__m128)v155[(unsigned __int8)v99], (__m128)v155[BYTE1(v99)], 68), 136);
      v105 = _mm_shuffle_ps(
               v103,
               _mm_shuffle_ps((__m128)v155[(unsigned __int8)v99 + 512], (__m128)v155[BYTE1(v99) + 512], 68),
               136);
      v106 = _mm_cvtsi64_si32(_m_packuswb(*(__m64 *)((char *)v95 + v153), *(__m64 *)((char *)v95 + v153)));
      v107 = (__m128)v155[(unsigned __int8)v106 + 256];
      v108 = (__m128)v155[(unsigned __int8)v106 + 768];
      v106 >>= 8;
      v109 = _mm_shuffle_ps(v107, (__m128)v155[(unsigned __int8)v106 + 256], 68);
      v110 = _mm_shuffle_ps(v108, (__m128)v155[(unsigned __int8)v106 + 768], 68);
      v106 >>= 8;
      v111 = (__m128)v155[(unsigned __int8)v106 + 256];
      v112 = (__m128)v155[(unsigned __int8)v106 + 768];
      result = v106 >> 8;
      v113 = _mm_cvt_pi32ps(_m_punpcklwd((__m64)v95->m64_u64, 0LL));
      v114 = _mm_add_ps(v104, v113);
      v115 = _mm_add_ps(
               _mm_add_ps(
                 _mm_shuffle_ps(v109, _mm_shuffle_ps(v111, (__m128)v155[(unsigned __int8)result + 256], 68), 136),
                 v105),
               v113);
      v116 = _mm_add_ps(
               _mm_shuffle_ps(v110, _mm_shuffle_ps(v112, (__m128)v155[(unsigned __int8)result + 768], 68), 136),
               v113);
      v117 = _mm_cvtt_ps2pi(v114);
      v118 = _m_packssdw(v117, v117);
      v119 = _m_packuswb(v118, v118);
      v120 = _m_punpcklbw(v119, v119);
      _m_maskmovq(_m_punpcklwd(v120, v120), v98, v11);
      v121 = _mm_cvtt_ps2pi(v115);
      v122 = _m_packssdw(v121, v121);
      v123 = _m_packuswb(v122, v122);
      v124 = _m_punpcklbw(v123, v123);
      _m_maskmovq(_m_punpcklwd(v124, v124), v97, v11);
      v125 = _mm_cvtt_ps2pi(v116);
      v126 = _m_packssdw(v125, v125);
      v127 = _m_packuswb(v126, v126);
      v128 = _m_punpcklbw(v127, v127);
      _m_maskmovq(_m_punpcklwd(v128, v128), v96, v11);
      v129 = v11 + 8;
      v130 = _mm_cvtt_ps2pi(_mm_movehl_ps(v114, v114));
      v131 = _m_packssdw(v130, v130);
      v132 = _m_packuswb(v131, v131);
      v133 = _m_punpcklbw(v132, v132);
      _m_maskmovq(_m_punpcklwd(v133, v133), v98, v129);
      v134 = _mm_cvtt_ps2pi(_mm_movehl_ps(v115, v115));
      v135 = _m_packssdw(v134, v134);
      v136 = _m_packuswb(v135, v135);
      v137 = _m_punpcklbw(v136, v136);
      _m_maskmovq(_m_punpcklwd(v137, v137), v97, v129);
      v138 = _mm_cvtt_ps2pi(_mm_movehl_ps(v116, v116));
      v139 = _m_packssdw(v138, v138);
      v140 = _m_packuswb(v139, v139);
      v141 = _m_punpcklbw(v140, v140);
      _m_maskmovq(_m_punpcklwd(v141, v141), v96, v129);
      if ( !--v154 )
        break;
      ++v95;
      v11 = v129 + 8;
    }
    if ( !--v152 )
      break;
    ++v95;
    v11 = &v129[v151 - 24];
  }
  if ( --v158 )
  {
    v10 = v95 + 1;
    goto LABEL_28;
  }
LABEL_30:
  _m_empty();
  return result;
}

// ===== sub_49E6B0 @ 0x0049E6B0..0x0049EEC7 =====
int __usercall sub_49E6B0@<eax>(int a1@<eax>, _DWORD *a2@<ecx>, int a3@<ebp>)
{
  const __m128i *v3; // edx
  int v5; // eax
  unsigned __int8 *v6; // edi
  int v7; // edx
  int v8; // eax
  unsigned __int8 v9; // cl
  const __m128i *v10; // esi
  char *v11; // edi
  int result; // eax
  __m64 v13; // mm6
  char v14; // cf
  __m128 *v15; // edx
  int v16; // eax
  __m128 *v17; // ebx
  int v18; // ecx
  __m128i v19; // xmm1
  __m128i v20; // xmm0
  __m128 v21; // xmm0
  __m128i v22; // xmm0
  __m128 v23; // xmm0
  __m128i v24; // xmm1
  __m128 v25; // xmm1
  __m128i v26; // xmm2
  __m128 v27; // xmm2
  __m128i v28; // xmm3
  __m128 v29; // xmm3
  __m128 v30; // xmm5
  __m128 v31; // xmm0
  __m128 v32; // xmm6
  __m128 v33; // xmm1
  __m128i v34; // xmm0
  __m128 v35; // xmm0
  __m128i v36; // xmm1
  __m128 v37; // xmm1
  __m128i v38; // xmm2
  __m128 v39; // xmm2
  __m128i v40; // xmm3
  __m128 v41; // xmm3
  __m128 v42; // xmm5
  __m128 v43; // xmm2
  __m128 v44; // xmm6
  __m128 v45; // xmm0
  __m128 v46; // xmm7
  __m128 v47; // xmm1
  __m128 v48; // xmm2
  __m128 v49; // xmm6
  __m128 v50; // xmm0
  __m128 v51; // xmm3
  __m128 v52; // xmm5
  __m128 v53; // xmm3
  __m128 v54; // xmm5
  __m128i *v55; // esi
  __m128 *v56; // edx
  int v57; // ecx
  __m128 v58; // xmm0
  __m128 v59; // xmm1
  __m128 v60; // xmm2
  __m128 v61; // xmm3
  __m128 v62; // xmm4
  __m128 v63; // xmm0
  __m128 v64; // xmm5
  __m128 v65; // xmm1
  __m128 v66; // xmm0
  __m128 v67; // xmm1
  __m128 v68; // xmm2
  __m128 v69; // xmm3
  __m128 v70; // xmm5
  __m128 v71; // xmm2
  __m128 v72; // xmm6
  __m128 v73; // xmm0
  __m128 v74; // xmm7
  __m128 v75; // xmm1
  __m128 v76; // xmm2
  __m128 v77; // xmm6
  __m128 v78; // xmm0
  __m128i v79; // xmm1
  __m128i v80; // xmm5
  __m128 v81; // xmm3
  __m128i v82; // xmm6
  __m128i v83; // xmm2
  __m128i v84; // xmm2
  __m128i v85; // xmm5
  __m128i v86; // xmm6
  __m128i v87; // xmm7
  __m128i v88; // xmm1
  __m128i v89; // xmm7
  __m128i v90; // xmm4
  __m128i v91; // xmm3
  __m128i v92; // xmm4
  __m128i v93; // xmm0
  __m128i v94; // xmm2
  __m128i v95; // xmm3
  __m128i v96; // xmm2
  __m128i v97; // xmm6
  __m128i v98; // xmm1
  __m128 v99; // xmm6
  __m128i v100; // xmm5
  __m128i v101; // xmm0
  __m128 v102; // xmm5
  const __m128i *v103; // esi
  __m128i v104; // xmm0
  __m128i v105; // xmm1
  __m128i v106; // xmm2
  __m128 v107; // xmm3
  unsigned int v108; // eax
  __m128i inserted; // xmm4
  __m128i v110; // xmm5
  __m128i v111; // xmm4
  __m128i v112; // xmm5
  __m128i v113; // xmm4
  __m128i v114; // xmm4
  __m128i v115; // xmm4
  __m128i v116; // xmm4
  __m128i v117; // xmm4
  unsigned int v118; // eax
  __m128i v119; // xmm4
  __m128i v120; // xmm6
  __m128i v121; // xmm4
  __m128i v122; // xmm6
  __m128i v123; // xmm4
  __m128i v124; // xmm6
  __m128i v125; // xmm4
  __m128i v126; // xmm4
  __m128i v127; // xmm4
  __m128i v128; // xmm4
  __m128i v129; // xmm3
  __m128i v130; // xmm3
  __m128i v131; // xmm3
  __m128i v132; // xmm3
  int v133; // [esp-194h] [ebp-1A0h]
  int *v134; // [esp-190h] [ebp-19Ch]
  int v135; // [esp-180h] [ebp-18Ch] BYREF
  __m128 v136; // [esp-80h] [ebp-8Ch]
  __m128 v137; // [esp-70h] [ebp-7Ch]
  __m128 v138; // [esp-60h] [ebp-6Ch]
  __m128 v139; // [esp-50h] [ebp-5Ch]
  _DWORD v140[4]; // [esp-40h] [ebp-4Ch]
  const __m128i *v141; // [esp-30h] [ebp-3Ch]
  int v142; // [esp-2Ch] [ebp-38h]
  int v143; // [esp-28h] [ebp-34h]
  unsigned int v144; // [esp-24h] [ebp-30h]
  int v145; // [esp-20h] [ebp-2Ch]
  unsigned __int16 *v146; // [esp-1Ch] [ebp-28h]
  int v147; // [esp-18h] [ebp-24h]
  int v148; // [esp-14h] [ebp-20h]
  int v149; // [esp-10h] [ebp-1Ch]
  char *v150; // [esp-Ch] [ebp-18h]
  unsigned __int8 *v151; // [esp-8h] [ebp-14h]
  unsigned __int8 v152; // [esp-1h] [ebp-Dh]
  int v153; // [esp+0h] [ebp-Ch]
  void *v154; // [esp+4h] [ebp-8h]
  int v155; // [esp+8h] [ebp-4h] BYREF
  void *retaddr; // [esp+Ch] [ebp+0h]

  v153 = a3;
  v154 = retaddr;
  v3 = (const __m128i *)a2[8];
  v150 = (char *)a2[3];
  v5 = a2[19];
  v6 = (unsigned __int8 *)a2[5];
  v141 = v3;
  v151 = v6;
  if ( v5 == 4 )
  {
    v147 = 3;
  }
  else
  {
    v147 = v5;
    if ( v5 == 1 )
    {
      v144 = 0;
      v146 = (unsigned __int16 *)&unk_564A90;
      goto LABEL_4;
    }
  }
  v144 = 2 * a2[17] * ((unsigned int)((2863311531u * (unsigned __int64)(a2[4] >> 3)) >> 32) >> 1);
  v146 = (unsigned __int16 *)&unk_561390;
LABEL_4:
  v7 = a2[20];
  v140[3] = a1 + 176;
  v140[2] = a1 + 432;
  v140[1] = a1 + 432;
  v8 = a2[15];
  v149 = v7;
  v9 = *v6;
  v142 = v8;
  v152 = v9;
  v148 = 8;
  v134 = &v155;
  v10 = v141;
  v11 = v150;
  result = -2139062144;
  v13 = _m_punpcklbw(_mm_cvtsi32_si64(0x80808080), 0LL);
  while ( 1 )
  {
    v14 = v152 & 1;
    v152 >>= 1;
    if ( v14 )
      break;
    if ( !--v149 )
      goto LABEL_30;
LABEL_28:
    v150 += 32;
    v11 = v150;
    if ( !--v148 )
    {
      result = (int)++v151;
      v152 = *v151;
      v148 = 8;
    }
  }
  v15 = (__m128 *)&v135;
  v16 = v147;
  while ( 1 )
  {
    v17 = (__m128 *)v140[v16];
    v18 = 2;
    v133 = v16;
    while ( 1 )
    {
      if ( v10[2].m128i_i32[0] | v10[1].m128i_i32[0]
        || (v19 = _mm_or_si128(
                    _mm_or_si128(
                      _mm_or_si128(_mm_loadl_epi64(v10 + 1), _mm_loadl_epi64(v10 + 2)),
                      _mm_or_si128(_mm_loadl_epi64(v10 + 3), _mm_loadl_epi64(v10 + 4))),
                    _mm_or_si128(
                      _mm_or_si128(_mm_loadl_epi64(v10 + 5), _mm_loadl_epi64(v10 + 6)),
                      _mm_loadl_epi64(v10 + 7))),
            _mm_cvtsi128_si32(_mm_packs_epi16(v19, v19))) )
      {
        v22 = _mm_loadl_epi64(v10);
        v23 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v22, v22), 0x10u)), *v17);
        v24 = _mm_loadl_epi64(v10 + 2);
        v25 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v24, v24), 0x10u)), v17[4]);
        v26 = _mm_loadl_epi64(v10 + 4);
        v27 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v26, v26), 0x10u)), v17[8]);
        v28 = _mm_loadl_epi64(v10 + 6);
        v29 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v28, v28), 0x10u)), v17[12]);
        v30 = _mm_add_ps(v23, v27);
        v31 = _mm_sub_ps(v23, v27);
        v32 = _mm_add_ps(v25, v29);
        v33 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v25, v29), (__m128)xmmword_508500), v32);
        *v15 = _mm_add_ps(v30, v32);
        v15[6] = _mm_sub_ps(v30, v32);
        v15[2] = _mm_add_ps(v31, v33);
        v15[4] = _mm_sub_ps(v31, v33);
        v34 = _mm_loadl_epi64(v10 + 1);
        v35 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v34, v34), 0x10u)), v17[2]);
        v36 = _mm_loadl_epi64(v10 + 3);
        v37 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v36, v36), 0x10u)), v17[6]);
        v38 = _mm_loadl_epi64(v10 + 5);
        v39 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v38, v38), 0x10u)), v17[10]);
        v40 = _mm_loadl_epi64(v10 + 7);
        v41 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v40, v40), 0x10u)), v17[14]);
        v42 = _mm_add_ps(v39, v37);
        v43 = _mm_sub_ps(v39, v37);
        v44 = _mm_add_ps(v35, v41);
        v45 = _mm_sub_ps(v35, v41);
        v46 = _mm_add_ps(v44, v42);
        v47 = _mm_mul_ps(_mm_add_ps(v43, v45), (__m128)xmmword_508510);
        v48 = _mm_sub_ps(_mm_add_ps(_mm_mul_ps(v43, (__m128)xmmword_508530), v47), v46);
        v49 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v44, v42), (__m128)xmmword_508500), v48);
        v50 = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v45, (__m128)xmmword_508520), v47), v49);
        v51 = *v15;
        *v15 = _mm_add_ps(*v15, v46);
        v15[14] = _mm_sub_ps(v51, v46);
        v52 = v15[2];
        v15[2] = _mm_add_ps(v52, v48);
        v15[12] = _mm_sub_ps(v52, v48);
        v53 = v15[4];
        v15[4] = _mm_add_ps(v53, v49);
        v15[10] = _mm_sub_ps(v53, v49);
        v54 = v15[6];
        v15[8] = _mm_add_ps(v54, v50);
        v15[6] = _mm_sub_ps(v54, v50);
      }
      else
      {
        v20 = _mm_loadl_epi64(v10);
        v21 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v20, v20), 0x10u)), *v17);
        *v15 = v21;
        v15[2] = v21;
        v15[4] = v21;
        v15[6] = v21;
        v15[8] = v21;
        v15[10] = v21;
        v15[12] = v21;
        v15[14] = v21;
      }
      if ( !--v18 )
        break;
      v10 = (const __m128i *)((char *)v10 + 8);
      ++v15;
      ++v17;
    }
    v55 = (__m128i *)&v10[-1].m128i_u64[1];
    v56 = v15 - 1;
    v57 = 2;
    while ( 1 )
    {
      v58 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56->m128_u32[0], v56[2], 68),
              _mm_shuffle_ps((__m128)v56[4].m128_u32[0], v56[6], 68),
              136);
      v59 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56->m128_u32[2], v56[2], 100),
              _mm_shuffle_ps((__m128)v56[4].m128_u32[2], v56[6], 100),
              136);
      v60 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56[1].m128_u32[0], v56[3], 68),
              _mm_shuffle_ps((__m128)v56[5].m128_u32[0], v56[7], 68),
              136);
      v61 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56[1].m128_u32[2], v56[3], 100),
              _mm_shuffle_ps((__m128)v56[5].m128_u32[2], v56[7], 100),
              136);
      v62 = _mm_add_ps(v58, v60);
      v63 = _mm_sub_ps(v58, v60);
      v64 = _mm_add_ps(v59, v61);
      v65 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v59, v61), (__m128)xmmword_508500), v64);
      v136 = _mm_add_ps(v62, v64);
      v139 = _mm_sub_ps(v62, v64);
      v137 = _mm_add_ps(v63, v65);
      v138 = _mm_sub_ps(v63, v65);
      v66 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56->m128_u32[1], v56[2], 84),
              _mm_shuffle_ps((__m128)v56[4].m128_u32[1], v56[6], 84),
              136);
      v67 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56->m128_u32[3], v56[2], 116),
              _mm_shuffle_ps((__m128)v56[4].m128_u32[3], v56[6], 116),
              136);
      v68 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56[1].m128_u32[1], v56[3], 84),
              _mm_shuffle_ps((__m128)v56[5].m128_u32[1], v56[7], 84),
              136);
      v69 = _mm_shuffle_ps(
              _mm_shuffle_ps((__m128)v56[1].m128_u32[3], v56[3], 116),
              _mm_shuffle_ps((__m128)v56[5].m128_u32[3], v56[7], 116),
              136);
      v70 = _mm_add_ps(v68, v67);
      v71 = _mm_sub_ps(v68, v67);
      v72 = _mm_add_ps(v66, v69);
      v73 = _mm_sub_ps(v66, v69);
      v74 = _mm_add_ps(v72, v70);
      v75 = _mm_mul_ps(_mm_add_ps(v71, v73), (__m128)xmmword_508510);
      v76 = _mm_sub_ps(_mm_add_ps(_mm_mul_ps(v71, (__m128)xmmword_508530), v75), v74);
      v77 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(v72, v70), (__m128)xmmword_508500), v76);
      v78 = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v73, (__m128)xmmword_508520), v75), v77);
      v79 = _mm_srai_epi16(
              _mm_packs_epi32(_mm_cvttps_epi32(_mm_add_ps(v136, v74)), _mm_cvttps_epi32(_mm_add_ps(v138, v77))),
              3u);
      v80 = _mm_srai_epi16(
              _mm_packs_epi32(_mm_cvttps_epi32(_mm_sub_ps(v138, v77)), _mm_cvttps_epi32(_mm_sub_ps(v136, v74))),
              3u);
      v81 = _mm_add_ps(v137, v76);
      v82 = _mm_srai_epi16(
              _mm_packs_epi32(_mm_cvttps_epi32(_mm_add_ps(v139, v78)), _mm_cvttps_epi32(_mm_sub_ps(v137, v76))),
              3u);
      v83 = _mm_movpi64_epi64(v13);
      v84 = _mm_unpacklo_epi16(v83, v83);
      v85 = _mm_unpacklo_epi8(_mm_packus_epi16(_mm_add_epi16(v80, v84), (__m128i)0LL), (__m128i)0LL);
      v86 = _mm_unpacklo_epi8(_mm_packus_epi16(_mm_add_epi16(v82, v84), (__m128i)0LL), (__m128i)0LL);
      v87 = _mm_unpacklo_epi8(_mm_packus_epi16(_mm_add_epi16(v79, v84), (__m128i)0LL), (__m128i)0LL);
      v88 = _mm_unpacklo_epi16(v87, v86);
      v89 = _mm_unpackhi_epi16(v87, v86);
      v90 = _mm_unpacklo_epi8(
              _mm_packus_epi16(
                _mm_add_epi16(
                  _mm_srai_epi16(_mm_packs_epi32(_mm_cvttps_epi32(v81), _mm_cvttps_epi32(_mm_sub_ps(v139, v78))), 3u),
                  v84),
                (__m128i)0LL),
              (__m128i)0LL);
      v91 = _mm_unpacklo_epi16(v90, v85);
      v92 = _mm_unpackhi_epi16(v90, v85);
      v93 = v88;
      v94 = v91;
      v95 = _mm_unpacklo_epi16(v91, v92);
      v96 = _mm_unpackhi_epi16(v94, v92);
      v97 = _mm_unpacklo_epi16(v88, v89);
      v98 = _mm_unpacklo_epi16(v97, v95);
      v99 = (__m128)_mm_unpackhi_epi16(v97, v95);
      v100 = _mm_unpackhi_epi16(v93, v89);
      v101 = _mm_unpacklo_epi16(v100, v96);
      v102 = (__m128)_mm_unpackhi_epi16(v100, v96);
      *v55 = v98;
      v55[1] = (__m128i)v99;
      v55[2] = v101;
      v55[3] = (__m128i)v102;
      if ( !--v57 )
        break;
      v56 += 8;
      v55 += 4;
    }
    --v16;
    if ( v133 == 1 )
      break;
    v10 = (__m128i *)((char *)v55 + v144 - 64);
    v15 = v56 - 8;
  }
  v103 = (__m128i *)((char *)v55 - 2 * v144 - 64);
  v143 = 8;
  v104 = _mm_unpacklo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(0xFFFFFFFF), (__m128i)0LL), (__m128i)0LL);
  v105 = _mm_slli_epi32(v104, 8u);
  v106 = _mm_slli_epi32(v104, 0x10u);
  while ( 1 )
  {
    v145 = 2;
    while ( 1 )
    {
      v107 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_loadl_epi64(v103), (__m128i)0LL));
      v108 = _mm_cvtsi64_si32(
               _m_packuswb(
                 *(__m64 *)((char *)v103->m128i_u64 + 2 * v144),
                 *(__m64 *)((char *)v103->m128i_u64 + 2 * v144)));
      inserted = _mm_insert_epi16(
                   _mm_insert_epi16(v92, v146[2 * (unsigned __int8)v108], 0),
                   v146[2 * (unsigned __int8)v108 + 1],
                   1);
      v110 = _mm_insert_epi16(
               _mm_insert_epi16((__m128i)v102, v146[2 * (unsigned __int8)v108 + 1024], 0),
               v146[2 * (unsigned __int8)v108 + 1025],
               1);
      v108 >>= 8;
      v111 = _mm_insert_epi16(
               _mm_insert_epi16(inserted, v146[2 * (unsigned __int8)v108], 2),
               v146[2 * (unsigned __int8)v108 + 1],
               3);
      v112 = _mm_insert_epi16(
               _mm_insert_epi16(v110, v146[2 * (unsigned __int8)v108 + 1024], 2),
               v146[2 * (unsigned __int8)v108 + 1025],
               3);
      v108 >>= 8;
      v102 = (__m128)_mm_insert_epi16(
                       _mm_insert_epi16(
                         _mm_insert_epi16(
                           _mm_insert_epi16(v112, v146[2 * (unsigned __int8)v108 + 1024], 4),
                           v146[2 * (unsigned __int8)v108 + 1025],
                           5),
                         v146[2 * BYTE1(v108) + 1024],
                         6),
                       v146[2 * BYTE1(v108) + 1025],
                       7);
      v113 = _mm_cvttps_epi32(
               _mm_add_ps(
                 (__m128)_mm_insert_epi16(
                           _mm_insert_epi16(
                             _mm_insert_epi16(
                               _mm_insert_epi16(v111, v146[2 * (unsigned __int8)v108], 4),
                               v146[2 * (unsigned __int8)v108 + 1],
                               5),
                             v146[2 * BYTE1(v108)],
                             6),
                           v146[2 * BYTE1(v108) + 1],
                           7),
                 v107));
      v114 = _mm_packs_epi32(v113, v113);
      v115 = _mm_packus_epi16(v114, v114);
      v116 = _mm_unpacklo_epi8(v115, v115);
      v117 = _mm_unpacklo_epi16(v116, v116);
      _mm_maskmoveu_si128(v117, v106, v11);
      v118 = _mm_cvtsi64_si32(_m_packuswb(*(__m64 *)((char *)v103->m128i_u64 + v144), *(__m64 *)((char *)v103->m128i_u64
                                                                                               + v144)));
      v119 = _mm_insert_epi16(
               _mm_insert_epi16(v117, v146[2 * (unsigned __int8)v118 + 512], 0),
               v146[2 * (unsigned __int8)v118 + 513],
               1);
      v120 = _mm_insert_epi16(
               _mm_insert_epi16((__m128i)v99, v146[2 * (unsigned __int8)v118 + 1536], 0),
               v146[2 * (unsigned __int8)v118 + 1537],
               1);
      v118 >>= 8;
      v121 = _mm_insert_epi16(
               _mm_insert_epi16(v119, v146[2 * (unsigned __int8)v118 + 512], 2),
               v146[2 * (unsigned __int8)v118 + 513],
               3);
      v122 = _mm_insert_epi16(
               _mm_insert_epi16(v120, v146[2 * (unsigned __int8)v118 + 1536], 2),
               v146[2 * (unsigned __int8)v118 + 1537],
               3);
      v118 >>= 8;
      v123 = _mm_insert_epi16(
               _mm_insert_epi16(v121, v146[2 * (unsigned __int8)v118 + 512], 4),
               v146[2 * (unsigned __int8)v118 + 513],
               5);
      v124 = _mm_insert_epi16(
               _mm_insert_epi16(v122, v146[2 * (unsigned __int8)v118 + 1536], 4),
               v146[2 * (unsigned __int8)v118 + 1537],
               5);
      result = v118 >> 8;
      v99 = (__m128)_mm_insert_epi16(
                      _mm_insert_epi16(v124, v146[2 * (unsigned __int8)result + 1536], 6),
                      v146[2 * (unsigned __int8)result + 1537],
                      7);
      v125 = _mm_cvttps_epi32(
               _mm_add_ps(
                 _mm_add_ps(
                   (__m128)_mm_insert_epi16(
                             _mm_insert_epi16(v123, v146[2 * (unsigned __int8)result + 512], 6),
                             v146[2 * (unsigned __int8)result + 513],
                             7),
                   v107),
                 v102));
      v126 = _mm_packs_epi32(v125, v125);
      v127 = _mm_packus_epi16(v126, v126);
      v128 = _mm_unpacklo_epi8(v127, v127);
      v92 = _mm_unpacklo_epi16(v128, v128);
      _mm_maskmoveu_si128(v92, v105, v11);
      v129 = _mm_cvttps_epi32(_mm_add_ps(v107, v99));
      v130 = _mm_packs_epi32(v129, v129);
      v131 = _mm_packus_epi16(v130, v130);
      v132 = _mm_unpacklo_epi8(v131, v131);
      _mm_maskmoveu_si128(_mm_unpacklo_epi16(v132, v132), v104, v11);
      if ( !--v145 )
        break;
      v103 = (const __m128i *)((char *)v103 + 8);
      v11 += 16;
    }
    if ( !--v143 )
      break;
    v103 = (const __m128i *)((char *)v103 + 8);
    v11 = &v11[v142 - 16];
  }
  if ( --v149 )
  {
    v10 = (const __m128i *)&v103->m128i_u64[1];
    goto LABEL_28;
  }
LABEL_30:
  _m_empty();
  return result;
}

// ===== sub_49EED0 @ 0x0049EED0..0x0049EEDE =====
BOOL __cdecl sub_49EED0(int a1)
{
  return sub_49D340(a1);
}

// ===== sub_49EEE0 @ 0x0049EEE0..0x0049EEE4 =====
int __usercall sub_49EEE0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 28) = a2;
  return result;
}

// ===== sub_49EEF0 @ 0x0049EEF0..0x0049EF05 =====
int __thiscall sub_49EEF0(_DWORD *this)
{
  int result; // eax

  result = this[7];
  if ( result >= this[6] )
    return -1;
  this[7] = result + 1;
  return result;
}

// ===== sub_49EF10 @ 0x0049EF10..0x0049EFCA =====
int __fastcall sub_49EF10(int a1, int a2, int a3, int a4, int a5, unsigned int a6, int a7, unsigned int a8)
{
  unsigned int v8; // edx
  int v9; // eax
  int v10; // ebx
  unsigned int v11; // ecx
  unsigned int v12; // edi
  unsigned int v13; // esi
  unsigned __int8 *v14; // ecx
  int v15; // edx
  int v16; // eax
  unsigned int v18; // [esp+Ch] [ebp-10h]
  int v19; // [esp+14h] [ebp-8h]
  unsigned int v20; // [esp+18h] [ebp-4h]

  v8 = a8;
  v9 = a5;
  if ( a8 == 4 )
  {
    v10 = 3;
    v20 = 3;
  }
  else
  {
    v10 = a8;
    v20 = a8;
  }
  v19 = 0;
  if ( !a6 )
    return 0;
  v11 = a8 * a1;
  v18 = v11;
  while ( 1 )
  {
    v12 = 0;
    if ( v11 )
      break;
LABEL_15:
    a4 += a7;
    v9 += a7;
    a5 = v9;
    if ( ++v19 >= a6 )
      return 0;
  }
  while ( 1 )
  {
    v13 = 0;
    if ( v10 )
      break;
LABEL_14:
    v12 += v8;
    if ( v12 >= v11 )
      goto LABEL_15;
  }
  v14 = (unsigned __int8 *)(v12 + v9);
  v15 = a4 - v9;
  while ( 1 )
  {
    v16 = v14[v15] - *v14;
    if ( v16 < 0 )
      v16 = *v14 - v14[v15];
    if ( v16 > *(_DWORD *)(a3 + 160) )
      return 1;
    v10 = v20;
    ++v13;
    ++v14;
    if ( v13 >= v20 )
    {
      v8 = a8;
      v9 = a5;
      v11 = v18;
      goto LABEL_14;
    }
  }
}

// ===== sub_49EFD0 @ 0x0049EFD0..0x0049F03E =====
int __fastcall sub_49EFD0(int a1, int a2, int a3, unsigned int a4, int a5)
{
  int v5; // eax
  unsigned int v7; // edx
  _BYTE *v8; // eax
  unsigned int v10; // [esp+Ch] [ebp-4h]

  v5 = a5;
  v7 = 0;
  if ( !a4 )
    return 0;
  v10 = 4 * a1;
  while ( !v10 )
  {
LABEL_8:
    a3 += v5;
    ++v7;
    a2 += v5;
    if ( v7 >= a4 )
      return 0;
  }
  v8 = (_BYTE *)(a2 + 3);
  while ( v8[a3 - a2] == *v8 )
  {
    v8 += 4;
    if ( (unsigned int)&v8[-3 - a2] >= v10 )
    {
      v5 = a5;
      goto LABEL_8;
    }
  }
  return 1;
}

// ===== sub_49F040 @ 0x0049F040..0x0049F1E7 =====
int __userpurge sub_49F040@<eax>(int a1@<eax>, int a2@<edx>, int a3, int a4, int a5, int a6)
{
  int v6; // ecx
  unsigned __int8 *v7; // ecx
  _WORD *v8; // eax
  __int16 v9; // si
  int v11; // eax
  _WORD *v12; // edi
  unsigned __int8 *v13; // esi
  int v14; // [esp+8h] [ebp-18h]
  int v15; // [esp+Ch] [ebp-14h]
  int v16; // [esp+18h] [ebp-8h]
  int v17; // [esp+1Ch] [ebp-4h]

  v6 = a2 * a1;
  v17 = 0;
  if ( a6 != 1 )
  {
    v11 = a3 + 2 * v6;
    v14 = v11;
    v15 = v11 + 2 * v6;
    if ( a2 )
    {
      v16 = a2;
      do
      {
        if ( 8 * a6 )
        {
          v12 = (_WORD *)(v11 + 2 * v17);
          v13 = (unsigned __int8 *)(a4 + 1);
          do
          {
            *(_WORD *)((char *)v12 + a3 - v11) = (unsigned __int8)byte_562390[(int)(flt_562690[v13[1]]
                                                                                  + flt_562A90[*v13]
                                                                                  + flt_562E90[*(v13 - 1)]
                                                                                  + 256.0)];
            *v12 = (unsigned __int8)byte_562390[(int)(flt_563290[v13[1]]
                                                    + flt_563690[*v13]
                                                    + flt_563A90[*(v13 - 1)]
                                                    + 256.0)];
            ++v17;
            *(_WORD *)((char *)v12 + v15 - v11) = (unsigned __int8)byte_562390[(int)(flt_563E90[v13[1]]
                                                                                   + flt_564290[*v13]
                                                                                   + flt_564690[*(v13 - 1)]
                                                                                   + 256.0)];
            v13 += a6;
            ++v12;
          }
          while ( (unsigned int)&v13[-1 - a4] < 8 * a6 );
          v11 = v14;
        }
        a4 += a5;
        --v16;
      }
      while ( v16 );
    }
    return 0;
  }
  if ( !a2 )
    return 0;
  v7 = (unsigned __int8 *)(a4 + 2);
  v8 = (_WORD *)(a3 + 4);
  do
  {
    *(v8 - 2) = *(v7 - 2);
    *(v8 - 1) = *(v7 - 1);
    *v8 = *v7;
    v8[1] = v7[1];
    v8[2] = v7[2];
    v8[3] = v7[3];
    v8[4] = v7[4];
    v9 = v7[5];
    v7 += a5;
    v8[5] = v9;
    v8 += 8;
    --a2;
  }
  while ( a2 );
  return 0;
}

// ===== sub_49F1F0 @ 0x0049F1F0..0x0049FA0F =====
int __fastcall sub_49F1F0(int a1, unsigned int a2, int a3, int a4, int a5)
{
  int v5; // eax
  double v6; // st6
  double v7; // st5
  float *v8; // ecx
  __int16 *v9; // eax
  unsigned int v10; // ebx
  int v11; // esi
  int v12; // edx
  int v13; // edi
  float v14; // edx
  int v15; // esi
  double v16; // st4
  int v17; // edx
  int v18; // edi
  float v19; // edx
  int v20; // esi
  double v21; // st4
  int v22; // edx
  int v23; // edi
  float v24; // edx
  int v25; // esi
  int v26; // edx
  double v27; // st4
  double v28; // st3
  double v29; // st4
  double v30; // st3
  double v31; // st4
  double v32; // st3
  double v33; // st4
  double v34; // st3
  double v35; // st6
  int v36; // eax
  float *v37; // esi
  __int16 *v38; // edi
  int v39; // eax
  char *v40; // ecx
  _BYTE *v41; // edx
  _BYTE *v42; // eax
  double v43; // st5
  double v44; // st4
  double v45; // st5
  double v46; // st4
  double v47; // st3
  double v48; // rt2
  double v49; // st4
  _BYTE *v50; // ecx
  double v51; // st3
  double v52; // st2
  double v53; // st1
  double v54; // st2
  _BYTE *v55; // edx
  int v57; // [esp+4h] [ebp-1A4h]
  int v58; // [esp+8h] [ebp-1A0h]
  int v59; // [esp+Ch] [ebp-19Ch]
  _DWORD v60[3]; // [esp+10h] [ebp-198h]
  char *v61; // [esp+1Ch] [ebp-18Ch]
  _BYTE *v62; // [esp+20h] [ebp-188h]
  _BYTE *v63; // [esp+24h] [ebp-184h]
  _BYTE *v64; // [esp+28h] [ebp-180h]
  float v65; // [esp+2Ch] [ebp-17Ch]
  float v66; // [esp+30h] [ebp-178h]
  float v67; // [esp+34h] [ebp-174h]
  float v68; // [esp+38h] [ebp-170h]
  float v69; // [esp+4Ch] [ebp-15Ch]
  float v70; // [esp+50h] [ebp-158h]
  float v71; // [esp+54h] [ebp-154h]
  float v72; // [esp+58h] [ebp-150h]
  float v73; // [esp+5Ch] [ebp-14Ch]
  float v74; // [esp+60h] [ebp-148h]
  float v75; // [esp+64h] [ebp-144h]
  float v76; // [esp+6Ch] [ebp-13Ch]
  float v77; // [esp+70h] [ebp-138h]
  float v78; // [esp+74h] [ebp-134h]
  float v79; // [esp+78h] [ebp-130h]
  float v80; // [esp+7Ch] [ebp-12Ch]
  float v81; // [esp+80h] [ebp-128h]
  float v82; // [esp+84h] [ebp-124h]
  float v83; // [esp+88h] [ebp-120h]
  _BYTE *v84; // [esp+8Ch] [ebp-11Ch]
  _BYTE *v85; // [esp+90h] [ebp-118h]
  unsigned int v86; // [esp+94h] [ebp-114h]
  int v87; // [esp+98h] [ebp-110h]
  float v88; // [esp+9Ch] [ebp-10Ch]
  float v89; // [esp+A0h] [ebp-108h]
  float v90[4]; // [esp+A4h] [ebp-104h] BYREF
  _BYTE v91[240]; // [esp+B4h] [ebp-F4h] BYREF

  if ( a2 == 4 )
  {
    v86 = 3;
LABEL_3:
    v5 = a5 * a4;
    goto LABEL_4;
  }
  v86 = a2;
  if ( a2 != 1 )
    goto LABEL_3;
  v5 = 0;
LABEL_4:
  v57 = a1;
  v58 = a1 + 2 * v5;
  v59 = v58 + 2 * v5;
  v60[0] = a3 + 176;
  v60[1] = a3 + 432;
  v60[2] = a3 + 432;
  v87 = 0;
  if ( v86 )
  {
    v6 = 0.382683433;
    v7 = 16384.5;
    while ( 1 )
    {
      v88 = *((float *)&v57 + v87);
      v8 = (float *)v91;
      v9 = (__int16 *)(LODWORD(v88) + 12);
      v10 = 8;
      do
      {
        v11 = v9[1];
        v12 = *(v9 - 6);
        v13 = v12 + v11 - 256;
        LODWORD(v14) = v12 - v11;
        v15 = *v9;
        v16 = (double)v13;
        v89 = v14;
        v17 = *(v9 - 5);
        v18 = v17 + v15 - 256;
        v76 = v16;
        LODWORD(v19) = v17 - v15;
        v20 = *(v9 - 1);
        v83 = (float)SLODWORD(v89);
        v21 = (double)v18;
        v89 = v19;
        v22 = *(v9 - 4);
        v23 = v22 + v20 - 256;
        v77 = v21;
        LODWORD(v24) = v22 - v20;
        v25 = *(v9 - 2);
        v82 = (float)SLODWORD(v89);
        v89 = v24;
        v26 = *(v9 - 3);
        v78 = (float)v23;
        v81 = (float)SLODWORD(v89);
        LODWORD(v89) = v26 - v25;
        v79 = (float)(v26 + v25 - 256);
        v80 = (float)(v26 - v25);
        v65 = v79 + v76;
        v68 = v76 - v79;
        v66 = v78 + v77;
        v67 = v77 - v78;
        v27 = v66;
        v28 = v65;
        *(v8 - 4) = v66 + v65;
        *v8 = v28 - v27;
        v29 = v68;
        v69 = (v67 + v68) * 0.707106781;
        v30 = v69;
        *(v8 - 2) = v69 + v68;
        v8[2] = v29 - v30;
        v9 += 8;
        v8 += 8;
        --v10;
        v65 = v80 + v81;
        v66 = v81 + v82;
        v67 = v82 + v83;
        v73 = (v65 - v67) * v6;
        v70 = v65 * 0.5411961 + v73;
        v72 = v73 + v67 * 1.306562965;
        v71 = v66 * 0.707106781;
        v74 = v71 + v83;
        v75 = v83 - v71;
        v31 = v75;
        v32 = v70;
        *(v8 - 7) = v75 + v70;
        *(v8 - 9) = v31 - v32;
        v33 = v74;
        v34 = v72;
        *(v8 - 11) = v74 + v72;
        *(v8 - 5) = v33 - v34;
      }
      while ( v10 );
      v35 = v7;
      v36 = v60[v87];
      v37 = (float *)(v36 + 64);
      v38 = (__int16 *)(LODWORD(v88) + 32);
      v39 = -v36;
      v84 = &v91[v39 + 112];
      v63 = &v91[v39 + 80];
      v40 = (char *)v90 + v39;
      v64 = &v91[v39 + 16];
      v41 = &v91[v39 + 144];
      v61 = (char *)v90 + v39;
      v62 = &v91[v39 + 144];
      v85 = &v91[v39 + 48];
      while ( 1 )
      {
        v76 = v90[v10] + *(float *)((char *)v37 + (_DWORD)v41);
        v83 = v90[v10] - *(float *)((char *)v37 + (_DWORD)v41);
        v77 = *(float *)&v91[4 * v10 + 16] + *(float *)((char *)v37 + (_DWORD)v84);
        v82 = *(float *)&v91[4 * v10 + 16] - *(float *)((char *)v37 + (_DWORD)v84);
        v78 = *(float *)((char *)v37 + (_DWORD)v63) + *(float *)((char *)v37 + (_DWORD)v40);
        v81 = *(float *)((char *)v37 + (_DWORD)v40) - *(float *)((char *)v37 + (_DWORD)v63);
        v79 = *(float *)((char *)v37 + (_DWORD)v85) + *(float *)((char *)v37 + (_DWORD)v64);
        v42 = v85;
        v80 = *(float *)((char *)v37 + (_DWORD)v64) - *(float *)((char *)v37 + (_DWORD)v85);
        v65 = v79 + v76;
        v68 = v76 - v79;
        v66 = v78 + v77;
        v67 = v77 - v78;
        v43 = v66;
        v44 = v65;
        v90[v10] = v66 + v65;
        *(float *)((char *)v37 + (_DWORD)v42) = v44 - v43;
        v45 = v68;
        v69 = (v67 + v68) * 0.707106781;
        v46 = v69;
        v89 = v69 + v68;
        v47 = v89;
        *(float *)((char *)v37 + (_DWORD)v40) = v89;
        v48 = v47;
        v89 = v45 - v46;
        v49 = v89;
        *(float *)((char *)v37 + (_DWORD)v84) = v89;
        v65 = v80 + v81;
        v50 = v64;
        v66 = v81 + v82;
        v67 = v82 + v83;
        v73 = (v65 - v67) * 0.382683433;
        v70 = v65 * 0.5411961 + v73;
        v72 = v73 + v67 * 1.306562965;
        v71 = v66 * 0.707106781;
        v74 = v71 + v83;
        v75 = v83 - v71;
        v51 = v75;
        v52 = v70;
        v89 = v75 + v70;
        v53 = v89;
        *(float *)((char *)v37 + (_DWORD)v63) = v89;
        v89 = v51 - v52;
        v54 = v89;
        *(float *)((char *)v37 + (_DWORD)v50) = v89;
        v88 = v74 + v72;
        *(float *)&v91[4 * v10 + 16] = v88;
        v89 = v74 - v72;
        *(float *)((char *)v37 + (_DWORD)v41) = v89;
        *(v38 - 16) = (int)(*(v37 - 16) * v90[v10] + v35) - 0x4000;
        *(v38 - 8) = (int)(*(v37 - 8) * v88 + v35) - 0x4000;
        *v38 = (int)(v48 * *v37 + v35) - 0x4000;
        v55 = v85;
        v38[8] = (int)(v54 * v37[8] + v35) - 0x4000;
        v38[16] = (int)(v37[16] * *(float *)((char *)v37 + (_DWORD)v55) + v35) - 0x4000;
        v38[24] = (int)(v53 * v37[24] + v35) - 0x4000;
        v38[32] = (int)(v49 * v37[32] + v35) - 0x4000;
        v38[40] = (int)(v37[40] * v89 + v35) - 0x4000;
        if ( v10 )
        {
          if ( *(v38 - 16) < -1023 )
            *(v38 - 16) = -1023;
          if ( *(v38 - 16) > 1023 )
            *(v38 - 16) = 1023;
        }
        if ( *(v38 - 8) < -1023 )
          *(v38 - 8) = -1023;
        if ( *(v38 - 8) > 1023 )
          *(v38 - 8) = 1023;
        if ( *v38 >= -1023 )
        {
          if ( *v38 > 1023 )
            *v38 = 1023;
        }
        else
        {
          *v38 = -1023;
        }
        if ( v38[8] < -1023 )
          v38[8] = -1023;
        if ( v38[8] > 1023 )
          v38[8] = 1023;
        if ( v38[16] < -1023 )
          v38[16] = -1023;
        if ( v38[16] > 1023 )
          v38[16] = 1023;
        if ( v38[24] < -1023 )
          v38[24] = -1023;
        if ( v38[24] > 1023 )
          v38[24] = 1023;
        if ( v38[32] < -1023 )
          v38[32] = -1023;
        if ( v38[32] > 1023 )
          v38[32] = 1023;
        if ( v38[40] < -1023 )
          v38[40] = -1023;
        if ( v38[40] > 1023 )
          v38[40] = 1023;
        ++v10;
        ++v37;
        ++v38;
        if ( v10 >= 8 )
          break;
        v41 = v62;
        v40 = v61;
      }
      if ( ++v87 >= v86 )
        break;
      v7 = v35;
      v6 = 0.382683433;
    }
  }
  return 0;
}

// ===== sub_49FA10 @ 0x0049FA10..0x0049FA3F =====
unsigned int __usercall sub_49FA10@<eax>(unsigned int result@<eax>, _DWORD *a2@<edi>, int a3@<esi>)
{
  int v3; // ecx
  char v4; // dl

  v3 = 0;
  if ( a3 )
  {
    do
    {
      v4 = result & 0x7F;
      result >>= 7;
      *(_BYTE *)(v3 + a3) = v4;
      if ( result )
        *(_BYTE *)(v3 + a3) |= 0x80u;
      ++v3;
    }
    while ( result );
  }
  else
  {
    do
    {
      result >>= 7;
      ++v3;
    }
    while ( result );
  }
  if ( a2 )
    *a2 = v3;
  return result;
}

// ===== sub_49FA40 @ 0x0049FA40..0x0049FA74 =====
int __userpurge sub_49FA40@<eax>(int a1@<edi>, _DWORD *a2)
{
  int result; // eax
  int v3; // esi
  int v4; // ecx
  char v5; // dl
  int v6; // ebx

  result = 0;
  v3 = 0;
  v4 = 0;
  do
  {
    v5 = *(_BYTE *)(v3 + a1);
    v6 = (v5 & 0x7F) << v4;
    ++v3;
    v4 += 7;
    result |= v6;
  }
  while ( v5 < 0 );
  if ( a2 )
    *a2 = v3;
  return result;
}

// ===== sub_49FA80 @ 0x0049FA80..0x0049FB3B =====
int __stdcall sub_49FA80(_BYTE *a1, _DWORD *a2, int a3, unsigned int a4, int a5)
{
  int v5; // eax
  unsigned int v6; // ebx
  int v7; // esi
  int v8; // eax
  int v10; // [esp+4h] [ebp-8h] BYREF
  int v11; // [esp+8h] [ebp-4h] BYREF

  v5 = 0;
  v6 = 0;
  v11 = 0;
  v10 = 0;
  *a1 = 0;
  if ( a4 )
  {
    do
    {
      v7 = *(__int16 *)(a3 + 2 * v6) - v5;
      v8 = sub_49FE60(v7);
      sub_49FF50(a1, &v11, *a2, v8, a5);
      if ( v7 < 0 )
        --v7;
      sub_4A00B0(&v11, &v10, *a2, v7);
      v5 = *(__int16 *)(a3 + 2 * v6);
      v6 += 64;
    }
    while ( v6 < a4 );
    if ( v10 )
      *a2 = v11 + 1;
    else
      *a2 = v11;
    return 0;
  }
  else
  {
    *a2 = 0;
    return 0;
  }
}

// ===== sub_49FB40 @ 0x0049FB40..0x0049FC2D =====
int __stdcall sub_49FB40(__m64 *a1, unsigned int a2, int a3, unsigned int *a4, int a5, int a6, int a7)
{
  unsigned int v7; // eax
  __m64 *v8; // edi
  unsigned int v9; // ecx
  unsigned int *v10; // ecx
  int v11; // ebx
  int v12; // eax
  __int16 v13; // ax
  unsigned int v14; // ecx
  __int16 v16; // [esp+4h] [ebp-10h]
  int v17; // [esp+8h] [ebp-Ch] BYREF
  unsigned int v18; // [esp+Ch] [ebp-8h] BYREF
  unsigned int v19; // [esp+10h] [ebp-4h]

  v7 = 0;
  v18 = 0;
  v8 = a1;
  v9 = a2 >> 5;
  do
  {
    _mm_stream_pi(v8, 0LL);
    _mm_stream_pi(v8 + 1, 0LL);
    _mm_stream_pi(v8 + 2, 0LL);
    _mm_stream_pi(v8 + 3, 0LL);
    _mm_stream_pi(v8 + 4, 0LL);
    _mm_stream_pi(v8 + 5, 0LL);
    _mm_stream_pi(v8 + 6, 0LL);
    _mm_stream_pi(v8 + 7, 0LL);
    v8 += 8;
    --v9;
  }
  while ( v9 );
  _m_empty();
  v10 = a4;
  v17 = 0;
  v16 = 0;
  v19 = 0;
  if ( *a4 )
  {
    do
    {
      if ( v19 >= a2 )
        break;
      v11 = sub_49FE90(a3, &v18, &v17, a5, 16, a6, a7);
      v12 = sub_4A0010(a3);
      if ( v11 )
      {
        if ( ((1 << (v11 - 1)) & v12) == 0 )
          LOWORD(v12) = ((-1 << v11) | v12) + 1;
      }
      v13 = v16 + v12;
      v14 = v19;
      a1->m64_i16[v19] = v13;
      v16 = v13;
      v7 = v18;
      v19 = v14 + 64;
    }
    while ( v18 < *a4 );
    if ( v17 )
    {
      *a4 = v7 + 1;
      return 0;
    }
    v10 = a4;
  }
  *v10 = v7;
  return 0;
}

// ===== sub_49FC30 @ 0x0049FC30..0x0049FD6B =====
int __stdcall sub_49FC30(_BYTE *a1, _DWORD *a2, int a3, unsigned int a4, int a5)
{
  unsigned int v6; // edi
  int v7; // eax
  int v8; // eax
  unsigned int v9; // ecx
  unsigned int v10; // edx
  int v11; // esi
  int v12; // edi
  int v13; // eax
  unsigned int v15; // [esp+8h] [ebp-10h]
  int v16; // [esp+Ch] [ebp-Ch]
  int v17; // [esp+10h] [ebp-8h] BYREF
  int v18; // [esp+14h] [ebp-4h] BYREF
  int v19; // [esp+20h] [ebp+8h]

  v6 = 0;
  v7 = 0;
  v18 = 0;
  v17 = 0;
  *a1 = 0;
  v15 = 0;
  if ( !a4 )
    goto LABEL_19;
  do
  {
    v8 = 0;
    v9 = 1;
    v19 = 0;
    v16 = 1;
    do
    {
      v10 = v6 + (unsigned __int8)byte_503DA8[v9];
      v11 = *(__int16 *)(a3 + 2 * v10);
      if ( *(_WORD *)(a3 + 2 * v10) )
      {
        if ( v8 )
        {
          v12 = v8;
          do
          {
            sub_49FF50(a1, &v18, *a2, 15, a5);
            --v12;
          }
          while ( v12 );
        }
        v13 = sub_49FE60(v11);
        sub_49FF50(a1, &v18, *a2, v19 | (16 * v13), a5);
        if ( v11 < 0 )
          --v11;
        sub_4A00B0(&v18, &v17, *a2, v11);
        v6 = v15;
        v9 = v16;
        v8 = 0;
        v19 = 0;
      }
      else if ( (unsigned int)++v19 >= 0x10 )
      {
        ++v8;
        v19 = *(__int16 *)(a3 + 2 * v10);
      }
      v16 = ++v9;
    }
    while ( v9 < 0x40 );
    if ( v19 || v8 )
      sub_49FF50(a1, &v18, *a2, 0, a5);
    v6 += 64;
    v15 = v6;
  }
  while ( v6 < a4 );
  if ( v17 )
  {
    v7 = v18 + 1;
LABEL_19:
    *a2 = v7;
    return 0;
  }
  *a2 = v18;
  return 0;
}

// ===== sub_49FD70 @ 0x0049FD70..0x0049FE54 =====
int __stdcall sub_49FD70(int a1, unsigned int a2, int a3, unsigned int *a4, int a5, int a6, int a7)
{
  unsigned int v7; // eax
  unsigned int *v8; // ebx
  unsigned int v9; // edi
  int v10; // eax
  int v11; // ebx
  int v12; // eax
  unsigned int v13; // ecx
  int v15; // [esp+4h] [ebp-10h] BYREF
  unsigned int v16; // [esp+8h] [ebp-Ch] BYREF
  unsigned int v17; // [esp+Ch] [ebp-8h]
  unsigned int v18; // [esp+10h] [ebp-4h]

  v7 = 0;
  v8 = a4;
  v15 = 0;
  v16 = 0;
  v18 = 0;
  if ( *a4 )
  {
    do
    {
      if ( v18 >= a2 )
        break;
      v9 = 1;
      do
      {
        v10 = sub_49FE90(a3, &v16, &v15, a5, 176, a6, a7);
        if ( !v10 )
          break;
        if ( v10 == 15 )
        {
          v9 += 16;
        }
        else
        {
          v11 = (unsigned __int8)v10 >> 4;
          v17 = (v10 & 0xF) + v9;
          v12 = sub_4A0010(a3);
          if ( v11 && ((1 << (v11 - 1)) & v12) == 0 )
            LOWORD(v12) = ((-1 << v11) | v12) + 1;
          v8 = a4;
          v13 = v17 + 1;
          *(_WORD *)(a1 + 2 * (v18 + (unsigned __int8)byte_503DA8[v17])) = v12;
          v17 = v13;
          v9 = v13;
        }
      }
      while ( v9 < 0x40 );
      v7 = v16;
      v18 += 64;
    }
    while ( v16 < *v8 );
    if ( v15 )
      ++v7;
  }
  *v8 = v7;
  return 0;
}

// ===== sub_49FE60 @ 0x0049FE60..0x0049FE82 =====
int __usercall sub_49FE60@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // esi
  int result; // eax
  int i; // edx

  v2 = a2;
  if ( a2 < 0 )
    v2 = -a2;
  result = a1 - 1;
  for ( i = __ROL4__(1, result); result; --result )
  {
    i = __ROR4__(i, 1);
    if ( (i & v2) != 0 )
      break;
  }
  return result;
}

// ===== sub_49FE90 @ 0x0049FE90..0x0049FF48 =====
unsigned int __stdcall sub_49FE90(int a1, unsigned int *a2, unsigned int *a3, int a4, unsigned int a5, int a6, int a7)
{
  unsigned int v7; // ebx
  int v8; // eax
  int v9; // ecx
  unsigned int result; // eax
  unsigned int *v11; // edx
  unsigned int v12; // ecx
  unsigned int v13; // edi
  unsigned int v14; // esi
  int v15; // eax
  unsigned int v16; // [esp+Ch] [ebp-8h]
  unsigned int v17; // [esp+10h] [ebp-4h]

  v7 = *a3;
  v17 = *a2;
  v16 = *a3;
  v8 = sub_4A0010(a1);
  v9 = *(_DWORD *)(a6 + 4 * v8);
  result = *(_DWORD *)(a7 + 4 * v8);
  if ( v9 )
  {
    v11 = a3;
    v12 = v7 + v9;
    if ( v12 >= 8 )
    {
      *a3 = v12 - 8;
      v11 = a2;
      v12 = *a2 + 1;
    }
    *v11 = v12;
  }
  else
  {
    v13 = v17;
    v14 = v16;
    while ( result >= a5 )
    {
      v15 = 3 * result;
      if ( ((unsigned __int8)(1 << (7 - v14)) & *(_BYTE *)(v13 + a1)) != 0 )
        result = *(_DWORD *)(a4 + 8 * v15 + 20);
      else
        result = *(_DWORD *)(a4 + 8 * v15 + 16);
      if ( ++v14 == 8 )
      {
        ++v13;
        v14 = 0;
      }
    }
    *a2 = v13;
    *a3 = v14;
  }
  return result;
}

// ===== sub_49FF50 @ 0x0049FF50..0x004A0002 =====
unsigned int __thiscall sub_49FF50(int *this, int a2, unsigned int *a3, unsigned int a4, int a5, int a6)
{
  int v6; // esi
  unsigned int result; // eax
  unsigned int i; // edx
  int v10; // edi
  _BYTE *v11; // edi
  char v12; // dl
  unsigned int v13; // [esp+Ch] [ebp-210h]
  _DWORD v15[129]; // [esp+14h] [ebp-208h] BYREF

  v6 = *this;
  v15[0] = a3;
  result = *a3;
  for ( i = 0; i < 0x1FF; ++i )
  {
    v10 = a6 + 24 * a5;
    *((_BYTE *)&v15[1] + i) = *(_BYTE *)(v10 + 8);
    a5 = *(_DWORD *)(v10 + 12);
    if ( a5 == -1 )
      break;
  }
  if ( i )
  {
    v11 = (char *)v15 + i + 3;
    v13 = i;
    do
    {
      v12 = *v11 << (7 - v6++);
      *(_BYTE *)(a2 + result) |= v12;
      if ( v6 == 8 )
      {
        ++result;
        v6 = 0;
        if ( result < a4 )
          *(_BYTE *)(a2 + result) = 0;
      }
      --v11;
      --v13;
    }
    while ( v13 );
  }
  *(_DWORD *)v15[0] = result;
  *this = v6;
  return result;
}
