#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_458130 @ 0x00458130..0x00458159 =====
int __usercall sub_458130@<eax>(_DWORD *a1@<eax>, int a2@<edi>)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx
  int result; // eax

  v2 = *(void (__thiscall ****)(_DWORD, int))(a2 + 16);
  if ( v2 )
    (**v2)(v2, 1);
  if ( a1 )
  {
    *(_DWORD *)(a2 + 16) = sub_452E40(a1);
    return 0;
  }
  else
  {
    result = 0;
    *(_DWORD *)(a2 + 16) = 0;
  }
  return result;
}

// ===== sub_458160 @ 0x00458160..0x004584BC =====
int __fastcall sub_458160(unsigned int a1, int a2, int *a3, void **a4, int a5)
{
  _DWORD *v5; // edx
  void *v6; // ecx
  int *v7; // esi
  unsigned int v8; // eax
  char *v9; // ecx
  unsigned int v10; // edi
  void *v11; // eax
  void **v12; // esi
  void *v13; // ecx
  int *v14; // edx
  void *v15; // eax
  int *v16; // edx
  void **v17; // edi
  unsigned int v18; // edx
  void *v19; // esi
  int v20; // eax
  void *v22; // [esp-10h] [ebp-194h]
  unsigned int v23; // [esp+4h] [ebp-180h] BYREF
  int *v24; // [esp+8h] [ebp-17Ch]
  unsigned int v25; // [esp+Ch] [ebp-178h] BYREF
  int v26; // [esp+10h] [ebp-174h]
  void **v27; // [esp+14h] [ebp-170h]
  _BYTE v28[20]; // [esp+18h] [ebp-16Ch] BYREF
  char v29[20]; // [esp+2Ch] [ebp-158h] BYREF
  char v30; // [esp+40h] [ebp-144h] BYREF

  v24 = a3;
  if ( a1 >= a3[9] )
    return -1879048189;
  v5 = (_DWORD *)(a3[10] + 2268 * a1);
  qmemcpy(v5, a4, 0x834u);
  v6 = *a4;
  v27 = (void **)v5;
  if ( !v6 )
  {
    v5[525] = -1;
    v5[526] = -1;
    return 0;
  }
  sub_4532D0(a3[4]);
  v7 = v24;
  sub_453340(&v25);
  v27[527] = (void *)(v25 - 2);
  if ( !a5 )
    return 0;
  sub_4530E0((_DWORD *)v7[4], &v25, &v23);
  v26 = 1;
  sub_459300(v29, 0);
  v8 = 0;
  v9 = &v30;
  do
  {
    v10 = v8 + 1;
    if ( !sub_459300(v9, v8 + 1) )
    {
      ++v26;
      v9 += 20;
    }
    v8 = v10;
  }
  while ( v10 < 0x10 );
  sub_453800((_DWORD *)v24[4], *a4, a4[141], (int)a4[142], v26, v29, 0, 1);
  operator delete(v27[533]);
  v26 = v23 * v25;
  v11 = operator new(28 * v23 * v25);
  v12 = v27;
  v27[533] = v11;
  operator delete(v12[534]);
  v13 = v12[533];
  v14 = v24;
  v12[534] = 0;
  if ( sub_453D80((int)*a4, (int)v29, v14[4], v13, 0) )
  {
    operator delete(v12[533]);
    v12[533] = 0;
  }
  else
  {
    v15 = operator new(28 * v26);
    v16 = v24;
    v12[534] = v15;
    sub_453D80((int)*a4, (int)v29, v16[4], v15, 1);
  }
  v17 = v12 + 536;
  v25 = 0;
  v27 = v12 + 536;
  do
  {
    operator delete(*(v17 - 1));
    v22 = *v17;
    *(v17 - 1) = 0;
    operator delete(v22);
    v18 = v25 + 1;
    *v17 = 0;
    v23 = v18;
    if ( !sub_459300(v28, v18) )
    {
      v19 = operator new(28 * v26);
      if ( sub_453D80((int)*a4, (int)v28, v24[4], v19, 0)
        || (v20 = v26, *(v27 - 1) = v19, v19 = operator new(28 * v20), sub_453D80((int)*a4, (int)v28, v24[4], v19, 1)) )
      {
        operator delete(v19);
      }
      else
      {
        *v27 = v19;
      }
      v17 = v27;
    }
    v17 += 2;
    v25 = v23;
    v27 = v17;
  }
  while ( v23 < 0x10 );
  return 0;
}

// ===== sub_4584C0 @ 0x004584C0..0x004584EC =====
int __userpurge sub_4584C0@<eax>(unsigned int a1@<eax>, int a2@<ecx>, void *a3)
{
  int result; // eax

  result = -1879048189;
  if ( a1 < *(_DWORD *)(a2 + 36) )
  {
    qmemcpy(a3, (const void *)(*(_DWORD *)(a2 + 40) + 2268 * a1), 0x834u);
    return 0;
  }
  return result;
}

// ===== sub_4584F0 @ 0x004584F0..0x004591F2 =====
int __thiscall sub_4584F0(size_t *this, _DWORD *a2, char *a3, unsigned int a4, size_t a5)
{
  unsigned int v5; // eax
  unsigned int v7; // esi
  unsigned int v8; // eax
  unsigned int v9; // ecx
  int v10; // esi
  char *v11; // edi
  int *v12; // esi
  void *v13; // eax
  int v14; // edx
  int v15; // eax
  int v16; // ecx
  unsigned int v17; // edi
  int *v18; // edx
  int *v19; // eax
  int *v20; // eax
  void **v21; // ecx
  void *v22; // ecx
  _DWORD *v23; // eax
  int *v24; // esi
  int v25; // esi
  int v26; // eax
  _DWORD *v27; // ecx
  void *v28; // eax
  _DWORD *v29; // esi
  int v30; // edx
  unsigned int v31; // ecx
  _DWORD *v32; // eax
  int v33; // ecx
  void (__thiscall *v34)(int, void *, _DWORD, unsigned int *, int, int, int, int, int, int, _DWORD, int); // edx
  int v35; // eax
  size_t v36; // eax
  void *v37; // eax
  void *v38; // edi
  void *v39; // esi
  _DWORD *v40; // eax
  _DWORD *v41; // ecx
  _DWORD *v42; // esi
  int v43; // eax
  int v44; // eax
  bool v45; // zf
  int v46; // edi
  _BYTE *v47; // esi
  int v48; // eax
  unsigned int v49; // eax
  size_t v50; // eax
  void *v51; // eax
  void *v52; // edi
  void *v53; // esi
  _DWORD *v54; // edx
  _DWORD *v55; // eax
  size_t v56; // eax
  void *v57; // eax
  void *v58; // edi
  void *v59; // esi
  unsigned int v60; // eax
  int v61; // esi
  void *v62; // ecx
  unsigned int v63; // eax
  int v64; // esi
  unsigned int v65; // edi
  _DWORD *v66; // edx
  unsigned int v67; // eax
  unsigned int i; // edi
  unsigned int j; // esi
  void (__thiscall ***v70)(_DWORD, int); // ecx
  size_t v71; // esi
  unsigned int v72; // edx
  char *v73; // edi
  _DWORD *v74; // eax
  int v75; // ecx
  int v76; // ebx
  _DWORD *v77; // ebx
  unsigned int v78; // edx
  int v79; // eax
  _DWORD *v80; // ecx
  int v81; // ecx
  _DWORD *v82; // eax
  size_t v83; // esi
  _DWORD *v84; // ebx
  size_t v85; // eax
  char *v86; // edx
  const void *v87; // esi
  void *v88; // edi
  int v89; // [esp-20h] [ebp-348h]
  int v90; // [esp-1Ch] [ebp-344h]
  int v91; // [esp-18h] [ebp-340h]
  int v92; // [esp-14h] [ebp-33Ch]
  int v93; // [esp-10h] [ebp-338h]
  const void *v94; // [esp-8h] [ebp-330h]
  int v95; // [esp-8h] [ebp-330h]
  size_t v96; // [esp-4h] [ebp-32Ch]
  int v97; // [esp-4h] [ebp-32Ch]
  _DWORD v98[19]; // [esp+10h] [ebp-318h] BYREF
  size_t *v99; // [esp+64h] [ebp-2C4h]
  int v100; // [esp+68h] [ebp-2C0h] BYREF
  int v101; // [esp+6Ch] [ebp-2BCh]
  char *v102; // [esp+70h] [ebp-2B8h]
  int v103; // [esp+74h] [ebp-2B4h] BYREF
  int v104; // [esp+78h] [ebp-2B0h]
  void **v105; // [esp+7Ch] [ebp-2ACh]
  void *v106; // [esp+80h] [ebp-2A8h]
  int v107; // [esp+84h] [ebp-2A4h]
  void *v108; // [esp+88h] [ebp-2A0h]
  int v109; // [esp+8Ch] [ebp-29Ch] BYREF
  unsigned int v110; // [esp+90h] [ebp-298h] BYREF
  unsigned int v111; // [esp+94h] [ebp-294h] BYREF
  unsigned int v112; // [esp+98h] [ebp-290h]
  void *v113; // [esp+9Ch] [ebp-28Ch]
  size_t NumOfElements; // [esp+A0h] [ebp-288h]
  int v115; // [esp+A4h] [ebp-284h]
  size_t Size; // [esp+A8h] [ebp-280h]
  int *v117; // [esp+ACh] [ebp-27Ch]
  void *v118; // [esp+B0h] [ebp-278h]
  void *v119; // [esp+B4h] [ebp-274h]
  unsigned int v120; // [esp+B8h] [ebp-270h] BYREF
  int *v121; // [esp+BCh] [ebp-26Ch]
  void *Src; // [esp+C0h] [ebp-268h]
  void *Base; // [esp+C4h] [ebp-264h]
  _DWORD *v124; // [esp+C8h] [ebp-260h]
  unsigned int v125; // [esp+CCh] [ebp-25Ch]
  _DWORD v126[19]; // [esp+D0h] [ebp-258h] BYREF
  _BYTE v127[256]; // [esp+120h] [ebp-208h] BYREF
  _DWORD v128[65]; // [esp+220h] [ebp-108h] BYREF

  v102 = a3;
  v5 = a2[9];
  v99 = this;
  v115 = a4;
  if ( a4 >= v5 )
    return -1879048189;
  v7 = 0;
  a2[11] = a4;
  if ( v5 )
  {
    do
    {
      sub_459770(a2);
      ++v7;
    }
    while ( v7 < a2[9] );
  }
  sub_4530E0((_DWORD *)a2[4], &v120, &v110);
  v8 = v120;
  v9 = v110;
  a2[6] = v120;
  a2[7] = v9;
  v10 = v9 * v8;
  v118 = operator new(8 * v9 * v8);
  v119 = operator new(8 * v10);
  v11 = (char *)operator new(2268 * a2[9]);
  v96 = 2268 * a2[9];
  v94 = (const void *)a2[10];
  v113 = v11;
  memcpy_0(v11, v94, v96);
  v12 = (int *)&v11[2268 * v115];
  v121 = v12;
  v108 = 0;
  v109 = -1;
  v105 = 0;
  if ( v12[3] >= 0 )
  {
    sub_453410(a2[4], v12[141], 0, -1, -1, 0);
    if ( !sub_453510(0, (int)&v109, v12[528], v12[529]) )
    {
      v13 = operator new(8 * v109);
      v97 = v12[529];
      v95 = v12[528];
      v14 = a2[4];
      v108 = v13;
      sub_453590(*v12, v14, (int)v13, &v109, v95, v97);
    }
    v15 = 2268 * v12[3];
    v16 = *(_DWORD *)&v11[v15 + 564];
    v105 = (void **)&v11[v15];
    sub_453410(a2[4], v16, 0, -1, -1, 0);
  }
  sub_453410(a2[4], v12[141], v12[142], -1, -1, 1);
  sub_453760((void *)*v12, (int)v118, (int)&v120, 1);
  v17 = v120;
  sub_4532D0(a2[4]);
  v112 = 4096;
  v125 = 0;
  Src = operator new(0x4C000u);
  memset(&v126[3], 255, 12);
  v126[7] = -1;
  v126[13] = 0x80000000;
  v126[14] = 0x80000000;
  v126[15] = 0x80000000;
  v126[16] = 0x80000000;
  v126[17] = 0x80000000;
  v126[18] = 0x80000000;
  v117 = (int *)v118;
  v126[6] = 0x80000000;
  v126[10] = 0;
  v103 = 6;
  v101 = -4 - (_DWORD)v108;
  v120 = v17 + 1;
  do
  {
    sub_453290(*v117, v117[1]);
    v18 = v117;
    v19 = v121;
    v121[525] = *v117;
    v19[526] = v18[1];
    v20 = (int *)((char *)Src + 76 * v125++);
    qmemcpy(v20, v126, 0x4Cu);
    *v20 = *v18;
    v20[1] = v18[1];
    v21 = (void **)v121;
    v20[2] = 0;
    v22 = *v21;
    v124 = v20;
    sub_453630(v22, (int)&v103);
    v23 = v124;
    v124[8] = v103 - 2;
    v24 = v23 + 9;
    sub_4536A0((void *)*v121, (int)(v23 + 9), *v117);
    if ( v105 && sub_4536A0(*v105, (int)(v124 + 10), *v117) )
      v124[10] = -1;
    v25 = *v24;
    if ( v25 )
      v26 = v121[143] + v25 * v121[144];
    else
      v26 = 0;
    v27 = v124;
    v124[11] = v26;
    v28 = (void *)v109;
    v27[12] = 0;
    if ( (int)v28 > 0 )
    {
      v29 = v108;
      v30 = (int)v117 + v101 + 4;
      Base = v28;
      do
      {
        v31 = 8;
        v32 = v29;
        while ( *(_DWORD *)((char *)v32 + v30) == *v32 )
        {
          v31 -= 4;
          ++v32;
          if ( v31 < 4 )
          {
            v124[12] = 1;
            break;
          }
        }
        v30 -= 8;
        v29 += 2;
        Base = (char *)Base - 1;
      }
      while ( Base );
    }
    qmemcpy(v98, v124, sizeof(v98));
    v33 = a2[4];
    v34 = *(void (__thiscall **)(int, void *, _DWORD, unsigned int *, int, int, int, int, int, int, _DWORD, int))(*(_DWORD *)v33 + 4);
    v93 = v121[151];
    v92 = v121[150];
    v91 = v121[149];
    v90 = v117[1];
    v89 = *v117;
    v98[2] = 6;
    v34(v33, v119, 0, &v111, v89, v90, v91, v92, v93, 1, 0, 1);
    Base = 0;
    if ( v111 )
    {
      Size = 76 * v125;
      v124 = v119;
      do
      {
        v35 = sub_459EA0(v113);
        if ( v35 && (*(_DWORD *)(v35 + 4) & v121[1]) == 0 )
        {
          if ( v125 >= v112 )
          {
            v36 = 152 * v112;
            v112 *= 2;
            v37 = operator new(v36);
            v38 = Src;
            v39 = v37;
            memcpy_0(v37, Src, Size);
            operator delete(v38);
            Src = v39;
          }
          ++v125;
          v40 = (char *)Src + Size;
          Size += 76;
          qmemcpy(v40, v98, 0x4Cu);
          v41 = v124;
          v40[4] = *v124;
          v40[5] = v41[1];
        }
        Base = (char *)Base + 1;
        v124 += 2;
      }
      while ( (unsigned int)Base < v111 );
    }
    v98[2] = 7;
    Base = 0;
    v124 = v121 + 186;
    do
    {
      if ( *(v124 - 13) )
      {
        v98[3] = Base;
        if ( (int)*v124 < 0 )
        {
          if ( v125 >= v112 )
          {
            v56 = 152 * v112;
            v112 *= 2;
            v57 = operator new(v56);
            v58 = Src;
            v59 = v57;
            memcpy_0(v57, Src, 76 * v125);
            operator delete(v58);
            Src = v59;
          }
          v60 = v125 + 1;
          qmemcpy((char *)Src + 76 * v125, v98, 0x4Cu);
          v125 = v60;
        }
        else
        {
          (*(void (__thiscall **)(_DWORD, void *, _DWORD, unsigned int *, int, int, _DWORD, _DWORD, _DWORD, bool, _DWORD, _DWORD))(*(_DWORD *)a2[4] + 4))(
            a2[4],
            v119,
            0,
            &v111,
            *v117,
            v117[1],
            *(v124 - 3),
            *(v124 - 2),
            *(v124 - 1),
            *v124 == 0,
            0,
            0);
          v110 = 0;
          if ( v111 )
          {
            NumOfElements = 76 * v125;
            Size = (size_t)v119;
            do
            {
              v42 = v124;
              v43 = *v124;
              if ( *v124 )
              {
                if ( v43 < 1 )
                  goto LABEL_49;
                (*(void (__thiscall **)(_DWORD, _BYTE *, _DWORD, int *, _DWORD, _DWORD, int, _DWORD, _DWORD, int, _DWORD, _DWORD))(*(_DWORD *)a2[4] + 4))(
                  a2[4],
                  v127,
                  0,
                  &v100,
                  *(_DWORD *)Size,
                  *(_DWORD *)(Size + 4),
                  v43,
                  0,
                  0,
                  1,
                  0,
                  0);
                v46 = v100;
                if ( v100 )
                {
                  v104 = 0;
                  v107 = 0;
                  v106 = (void *)v121[1];
                  v47 = v127;
                  do
                  {
                    v48 = sub_459EA0(v113);
                    v49 = (unsigned int)v106 & *(_DWORD *)(v48 + 4);
                    v104 += v49 != 0;
                    v47 += 8;
                    v107 += v49 == 0;
                    --v46;
                  }
                  while ( v46 );
                  switch ( *(v124 - 12) )
                  {
                    case 0:
                    case 4:
                      v45 = v107 == 0;
                      break;
                    case 1:
                    case 2:
                    case 3:
                      v45 = v104 == 0;
                      break;
                    default:
                      goto LABEL_49;
                  }
LABEL_48:
                  if ( !v45 )
                  {
LABEL_49:
                    if ( v125 >= v112 )
                    {
                      v50 = 152 * v112;
                      v112 *= 2;
                      v51 = operator new(v50);
                      v52 = Src;
                      v53 = v51;
                      memcpy_0(v51, Src, NumOfElements);
                      operator delete(v52);
                      Src = v53;
                    }
                    ++v125;
                    v54 = (char *)Src + NumOfElements;
                    NumOfElements += 76;
                    v55 = (_DWORD *)Size;
                    qmemcpy(v54, v98, 0x4Cu);
                    v54[4] = *v55;
                    v54[5] = v55[1];
                  }
                }
              }
              else
              {
                v44 = sub_459EA0(v113);
                switch ( *(v42 - 12) )
                {
                  case 0:
                  case 4:
                    if ( (*(_DWORD *)(v44 + 4) & v121[1]) == 0 )
                      goto LABEL_49;
                    break;
                  case 1:
                  case 2:
                  case 3:
                    v45 = (*(_DWORD *)(v44 + 4) & v121[1]) == 0;
                    goto LABEL_48;
                  default:
                    goto LABEL_49;
                }
              }
              Size += 8;
              ++v110;
            }
            while ( v110 < v111 );
          }
        }
      }
      v124 += 22;
      Base = (char *)Base + 1;
    }
    while ( (unsigned int)Base < 0x10 );
    v117 += 2;
    --v120;
  }
  while ( v120 );
  operator delete(v118);
  operator delete(v119);
  v61 = a2[9];
  v62 = Src;
  a2[82] = v125;
  a2[83] = 0;
  a2[84] = v62;
  memset(v128, 0, 4 * v61);
  v128[v115] = 1;
  v120 = 0;
  if ( v61 )
  {
    Base = a2 + 12;
    do
    {
      v63 = 0;
      v64 = -1;
      v115 = 0x7FFFFFFF;
      if ( a2[9] )
      {
        v65 = a2[9];
        v66 = (char *)v113 + 300;
        do
        {
          if ( !v128[v63] )
          {
            if ( *(v66 - 75) )
            {
              if ( *v66 + v66[33] < v115 )
              {
                v64 = v63;
                v115 = *v66 + v66[33];
              }
            }
            else
            {
              v128[v63] = 1;
            }
          }
          ++v63;
          v66 += 567;
        }
        while ( v63 < v65 );
      }
      *(_DWORD *)Base = v64;
      if ( v64 < 0 )
        break;
      Base = (char *)Base + 4;
      v67 = v120 + 1;
      v128[v64] = 1;
      v120 = v67;
    }
    while ( v67 < a2[9] );
  }
  for ( i = 0; i < a2[2]; ++i )
    *(_DWORD *)(a2[5] + 4 * i) = sub_452E40((_DWORD *)a2[4]);
  sub_4464E0(1, a2[3]);
  for ( j = 0; j < a2[2]; ++j )
  {
    v70 = *(void (__thiscall ****)(_DWORD, int))(a2[5] + 4 * j);
    if ( v70 )
      (**v70)(v70, 1);
  }
  v71 = 0;
  if ( v125 )
  {
    v72 = v125;
    v73 = (char *)Src + 52;
    do
    {
      v74 = v73;
      v75 = 6;
      do
      {
        v76 = *v74++ != 0x80000000;
        v71 += v76;
        --v75;
      }
      while ( v75 );
      v73 += 76;
      --v72;
    }
    while ( v72 );
    NumOfElements = v71;
    if ( v71 )
    {
      LOBYTE(v75) = (28 * (unsigned __int64)v71) >> 32 != 0;
      v106 = operator new((28 * v71) | -v75);
      v77 = v106;
      Base = operator new(4 * v71);
      v115 = (int)Base;
      v119 = (char *)Src + 28;
      v120 = v125;
      do
      {
        v78 = 0;
        v118 = (char *)v119 + 24;
        do
        {
          if ( *(_DWORD *)v118 != 0x80000000 )
          {
            v79 = 0;
            qmemcpy(v77, (char *)v119 - 28, 0x1Cu);
            if ( !v77[2] )
            {
              v80 = v119;
              v77[2] = v78;
              if ( *v80 == v78 )
                v79 = 2;
              if ( v80[1] == v78 )
                v79 = 1;
            }
            v81 = v79 + *(_DWORD *)v118;
            v82 = (_DWORD *)v115;
            v77[6] = v81;
            *v82 = v77;
            v77 += 7;
            v115 = (int)(v82 + 1);
          }
          v118 = (char *)v118 + 4;
          ++v78;
        }
        while ( v78 < 6 );
        v119 = (char *)v119 + 76;
        --v120;
      }
      while ( v120 );
      v83 = NumOfElements;
      qsort(Base, NumOfElements, 4u, sub_45B5C0);
      v84 = Base;
      v85 = 0;
      if ( v83 )
      {
        v86 = v102;
        do
        {
          if ( v85 >= a5 )
            break;
          v87 = (const void *)v84[v85];
          v88 = v86;
          ++v85;
          v86 += 28;
          qmemcpy(v88, v87, 0x1Cu);
        }
        while ( v85 < NumOfElements );
      }
      operator delete(v106);
      operator delete(v84);
      v71 = NumOfElements;
    }
  }
  operator delete(v113);
  operator delete(Src);
  operator delete(v108);
  if ( a5 <= v71 )
    v71 = a5;
  *v99 = v71;
  return 0;
}

// ===== sub_459220 @ 0x00459220..0x0045923B =====
int __usercall sub_459220@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<ecx>)
{
  int result; // eax

  a2[76] = *a1;
  a2[77] = a1[1];
  result = a1[2];
  a2[78] = result;
  return result;
}

// ===== sub_459240 @ 0x00459240..0x00459296 =====
int __userpurge sub_459240@<eax>(int a1@<ecx>, int a2@<edi>, _DWORD *a3)
{
  int result; // eax
  int v4; // esi

  result = 0;
  if ( a1 >= 0 )
  {
    v4 = a3[8];
    if ( a1 < v4 && a2 >= 0 && a2 < v4 )
    {
      if ( a1 == (a2 + 1) % v4 )
      {
        return a3[76];
      }
      else if ( a1 == (v4 + a2 - 1) % v4 )
      {
        return a3[78];
      }
      else
      {
        return a3[77];
      }
    }
  }
  return result;
}

// ===== sub_4592A0 @ 0x004592A0..0x004592AD =====
int __usercall sub_4592A0@<eax>(int result@<eax>)
{
  *(double *)(result + 320) = 2.0;
  return result;
}

// ===== sub_4592B0 @ 0x004592B0..0x004592FF =====
int __stdcall sub_4592B0(int a1, int a2)
{
  double v2; // st7

  v2 = sub_4010B0(cos((double)a2 / 11796480.0 * 1.570796326794897), 0.0009765625);
  return (int)((v2 + (1.0 - v2) * *(double *)(a1 + 320)) * 65536.0);
}

// ===== sub_459300 @ 0x00459300..0x0045936E =====
int __usercall sub_459300@<eax>(int a1@<edx>, _DWORD *a2@<ecx>, _DWORD *a3@<esi>)
{
  int result; // eax
  int v4; // edx
  bool v5; // zf
  _DWORD *v6; // edx

  result = -1879048188;
  if ( a1 )
  {
    if ( (unsigned int)(a1 - 1) <= 0xF )
    {
      v4 = 22 * a1;
      v5 = a3[v4 + 151] == 0;
      v6 = &a3[v4 + 151];
      if ( !v5 )
      {
        *a2 = v6[1];
        a2[1] = v6[10];
        a2[2] = v6[11];
        a2[3] = v6[12];
        a2[4] = v6[13];
        return 0;
      }
    }
  }
  else
  {
    *a2 = 0;
    a2[1] = a3[149];
    a2[2] = a3[150];
    a2[3] = a3[151];
    a2[4] = 0;
    return 0;
  }
  return result;
}

// ===== sub_459370 @ 0x00459370..0x0045961A =====
int __usercall sub_459370@<eax>(
        int a1@<eax>,
        _DWORD *a2@<ecx>,
        int a3@<edi>,
        _DWORD *a4,
        int a5,
        _DWORD *a6,
        _DWORD *a7,
        int a8,
        int a9)
{
  int v10; // ebx
  __int64 v11; // rax
  int v12; // ebx
  __int64 v13; // rax
  int v14; // ebx
  int v15; // eax
  int v16; // ebx
  __int64 v17; // rax
  int v18; // ebx
  __int64 v19; // rax
  int v20; // ebx
  __int64 v21; // rax
  unsigned int v22; // ecx
  __int64 v23; // [esp-10h] [ebp-30h]
  __int64 v24; // [esp-8h] [ebp-28h]
  __int64 v25; // [esp+8h] [ebp-18h]
  __int64 v26; // [esp+10h] [ebp-10h]
  __int64 v27; // [esp+18h] [ebp-8h]
  __int64 v28; // [esp+18h] [ebp-8h]
  __int64 v29; // [esp+18h] [ebp-8h]
  __int64 v30; // [esp+18h] [ebp-8h]
  __int64 v31; // [esp+18h] [ebp-8h]
  int v32; // [esp+3Ch] [ebp+1Ch]
  int v33; // [esp+3Ch] [ebp+1Ch]

  v25 = a1;
  v26 = a9;
  if ( !a5 )
  {
    v33 = a7[2];
    if ( v33 < 1 )
      v33 = 1;
    if ( *a2 * (a3 + *a6 + a2[1]) / v33 - a7[3] - a8 <= 0LL )
      v30 = 0LL;
    else
      v30 = *a2 * (a3 + *a6 + a2[1]) / v33 - a7[3] - a8;
    v16 = a7[6];
    if ( v16 < 1 )
      v16 = 1;
    v17 = a2[4] * (a3 + a6[1] + a2[5]) / v16 - a7[7] - a8;
    if ( v17 < 0 || (v17 < 0 || a2[4] * (a3 + a6[1] + a2[5]) / v16 - a7[7] - a8 >= 0) && !(_DWORD)v17 )
      v17 = 0LL;
    v18 = a7[10];
    v31 = v17 + v30;
    if ( v18 < 1 )
      v18 = 1;
    v19 = a2[8] * (a3 + a6[2] + a2[9]) / v18 - a7[11] - a8;
    if ( v19 < 0 || (v19 < 0 || a2[8] * (a3 + a6[2] + a2[9]) / v18 - a7[11] - a8 >= 0) && !(_DWORD)v19 )
      v19 = 0LL;
    v20 = a7[14];
    v29 = v19 + v31;
    if ( v20 < 1 )
      v20 = 1;
    v15 = a2[12] * (a3 + a6[3] + a2[13]) / v20 - a7[15];
    goto LABEL_46;
  }
  if ( a5 == 1 )
  {
    v32 = a7[18];
    if ( v32 < 1 )
      v32 = 1;
    if ( a2[16] * (a3 + *a6 + a2[17]) / v32 - a7[19] - a8 <= 0LL )
      v27 = 0LL;
    else
      v27 = a2[16] * (a3 + *a6 + a2[17]) / v32 - a7[19] - a8;
    v10 = a7[22];
    if ( v10 < 1 )
      v10 = 1;
    v11 = a2[20] * (a3 + a6[1] + a2[21]) / v10 - a7[23] - a8;
    if ( v11 < 0 || (v11 < 0 || a2[20] * (a3 + a6[1] + a2[21]) / v10 - a7[23] - a8 >= 0) && !(_DWORD)v11 )
      v11 = 0LL;
    v12 = a7[26];
    v28 = v11 + v27;
    if ( v12 < 1 )
      v12 = 1;
    v13 = a2[24] * (a3 + a6[2] + a2[25]) / v12 - a7[27] - a8;
    if ( v13 < 0 || (v13 < 0 || a2[24] * (a3 + a6[2] + a2[25]) / v12 - a7[27] - a8 >= 0) && !(_DWORD)v13 )
      v13 = 0LL;
    v14 = a7[30];
    v29 = v13 + v28;
    if ( v14 < 1 )
      v14 = 1;
    v15 = a2[28] * (a3 + a6[3] + a2[29]) / v14 - a7[31];
LABEL_46:
    v21 = v15 - a8;
    if ( v21 <= 0 )
    {
      LODWORD(v21) = 0;
      v22 = 0;
    }
    else
    {
      v22 = HIDWORD(v21);
    }
    v23 = v29 + __PAIR64__(v22, v21);
    v24 = v26;
    goto LABEL_50;
  }
  if ( a5 != 2 )
  {
    *a4 = 0;
    return -1879048187;
  }
  v24 = a9;
  v23 = a3 + *a6 + a2[17] + a3 + a6[1] + a2[21] + a3 + a6[2] + a2[25] + (__int64)(a3 + a6[3] + a2[29]);
LABEL_50:
  *a4 = (unsigned __int64)(v23 * v24 * v25) >> 32;
  return 0;
}

// ===== sub_459620 @ 0x00459620..0x00459762 =====
int __usercall sub_459620@<eax>(unsigned int a1@<edx>, int *a2@<ecx>, int *a3@<edi>, _DWORD *a4@<esi>)
{
  int result; // eax
  int v5; // eax
  int v6; // ecx
  bool v7; // cc
  int v8; // eax
  int v9; // eax
  int v10; // ecx
  int v11; // eax

  result = -1879048187;
  if ( a1 > 2 )
  {
    if ( a1 != 256 )
    {
      *a3 = 0;
      a3[1] = 0;
      return result;
    }
    if ( a2[85] <= 0 )
      v11 = 0;
    else
      v11 = a2[117];
    if ( a2[86] > 0 )
    {
      *a3 = v11 - a2[118];
      return 0;
    }
    goto LABEL_39;
  }
  if ( a1 == 2 )
  {
    if ( a2[81] <= 0 )
      v11 = 0;
    else
      v11 = a2[113];
    if ( a2[82] > 0 )
    {
      *a3 = v11 - a2[114];
      return 0;
    }
LABEL_39:
    *a3 = v11;
    return 0;
  }
  if ( a1 )
  {
    if ( a2[81] <= 0 )
      v5 = 0;
    else
      v5 = a2[113];
    if ( a2[82] <= 0 )
      v6 = 0;
    else
      v6 = a2[114];
    v7 = a4[83] <= 0;
    *a3 = v5 - v6;
    if ( v7 )
      v8 = 0;
    else
      v8 = a4[115];
    if ( (int)a4[84] > 0 )
    {
      a3[1] = v8 - a4[116];
      return 0;
    }
    goto LABEL_15;
  }
  if ( a2[77] <= 0 )
    v9 = 0;
  else
    v9 = a2[109];
  if ( a2[78] <= 0 )
    v10 = 0;
  else
    v10 = a2[110];
  v7 = a4[79] <= 0;
  *a3 = v9 - v10;
  if ( v7 )
    v8 = 0;
  else
    v8 = a4[111];
  if ( (int)a4[80] <= 0 )
  {
LABEL_15:
    a3[1] = v8;
    return 0;
  }
  a3[1] = v8 - a4[112];
  return 0;
}

// ===== sub_459770 @ 0x00459770..0x00459E9F =====
int __fastcall sub_459770(unsigned int a1, int a2, int *a3)
{
  int result; // eax
  _DWORD *v4; // edi
  int v5; // ecx
  bool v6; // zf
  _DWORD *v7; // eax
  unsigned int v8; // eax
  int *v9; // esi
  unsigned int v10; // edi
  int v11; // ecx
  int v12; // edx
  unsigned int v13; // eax
  unsigned int v14; // ecx
  unsigned int v15; // esi
  unsigned int v16; // ecx
  _DWORD *v17; // edi
  _DWORD *v18; // edx
  void *v19; // esi
  void *v20; // eax
  void *v21; // ecx
  int v22; // edi
  _DWORD *v23; // esi
  _DWORD *v24; // esi
  unsigned int v25; // ecx
  unsigned int v26; // edx
  int v27; // eax
  unsigned int v28; // ecx
  int v29; // edx
  int v30; // eax
  int v31; // eax
  int v32; // ecx
  int v33; // edx
  void *v34; // edi
  int v35; // edx
  BOOL v36; // edi
  int v37; // edx
  int v38; // esi
  int v39; // eax
  int v40; // ecx
  int v41; // edx
  int v42; // ecx
  _DWORD *v43; // eax
  void *v44; // eax
  int *v45; // esi
  _DWORD *v46; // eax
  int v47; // [esp-8h] [ebp-59Ch]
  int v48; // [esp-4h] [ebp-598h]
  int v49; // [esp-4h] [ebp-598h]
  int v50; // [esp-4h] [ebp-598h]
  int v51; // [esp+Ch] [ebp-588h]
  BOOL v52; // [esp+10h] [ebp-584h]
  void *v53; // [esp+10h] [ebp-584h]
  int v54; // [esp+14h] [ebp-580h]
  int v55; // [esp+14h] [ebp-580h]
  int v56; // [esp+18h] [ebp-57Ch] BYREF
  int v57; // [esp+1Ch] [ebp-578h]
  int v58; // [esp+20h] [ebp-574h]
  unsigned int v59; // [esp+24h] [ebp-570h] BYREF
  void *v60; // [esp+28h] [ebp-56Ch] BYREF
  unsigned int v61; // [esp+2Ch] [ebp-568h]
  _DWORD *v62; // [esp+30h] [ebp-564h]
  void *v63; // [esp+34h] [ebp-560h]
  unsigned int v64; // [esp+38h] [ebp-55Ch] BYREF
  int v65; // [esp+3Ch] [ebp-558h]
  _DWORD v66[128]; // [esp+40h] [ebp-554h] BYREF
  _DWORD v67[128]; // [esp+240h] [ebp-354h] BYREF
  _BYTE v68[308]; // [esp+440h] [ebp-154h] BYREF
  _DWORD v69[6]; // [esp+574h] [ebp-20h] BYREF

  result = -1879048189;
  if ( a1 < a3[9] )
  {
    v4 = (_DWORD *)(a3[10] + 2268 * a1);
    v5 = *v4;
    v62 = v4;
    if ( v5 )
    {
      sub_453410(a3[4], v4[141], 0, -1, -1, 0);
      if ( v4[149] == 1 )
      {
        v6 = a3[9] == 0;
        v65 = 0;
        if ( !v6 )
        {
          v63 = 0;
          v61 = (unsigned int)v67;
          do
          {
            v7 = (char *)v63 + a3[10];
            if ( (v7[1] & v62[1]) != 0 || !*v7 )
            {
              *(_DWORD *)&v68[4 * v65 + 48] = 0;
            }
            else
            {
              v48 = a3[4];
              v57 = 0x7FFFFFFF;
              sub_4532D0(v48);
              sub_4534A0((int)v69, (int)v68, v58, v59);
              v8 = 0;
              v64 = 0;
              v9 = (int *)v68;
              do
              {
                if ( (int)v69[v8] >= 0 )
                {
                  v10 = v9[1];
                  sub_4536A0((void *)*v62, (int)&v56, *v9);
                  if ( v56 < v57 )
                  {
                    v11 = *v9;
                    v57 = v56;
                    v58 = v11;
                    v59 = v10;
                  }
                }
                v8 = v64 + 1;
                v9 += 2;
                v64 = v8;
              }
              while ( v8 < 6 );
              if ( v57 == 0x7FFFFFFF )
              {
                *(_DWORD *)&v68[4 * v65 + 48] = -1;
              }
              else
              {
                v12 = v58;
                *(_DWORD *)&v68[4 * v65 + 48] = v57;
                v13 = v61;
                v14 = v59;
                *(_DWORD *)v61 = v12;
                *(_DWORD *)(v13 + 4) = v14;
              }
            }
            v63 = (char *)v63 + 2268;
            v61 += 8;
            ++v65;
          }
          while ( v65 < (unsigned int)a3[9] );
        }
      }
      else
      {
        v15 = 0;
        if ( a3[9] )
        {
          v16 = v4[1];
          v17 = (_DWORD *)a3[10];
          v59 = v16;
          v18 = v66;
          do
          {
            if ( (v59 & v17[1]) != 0 || !*v17 )
            {
              *(_DWORD *)&v68[4 * v15 + 48] = 0;
            }
            else
            {
              v49 = a3[4];
              *(_DWORD *)&v68[4 * v15 + 48] = -1;
              sub_4532D0(v49);
            }
            ++v15;
            v17 += 567;
            v18 += 2;
          }
          while ( v15 < a3[9] );
        }
        sub_4530E0((_DWORD *)a3[4], &v59, &v60);
        v19 = operator new(8 * (_DWORD)v60 * v59);
        v60 = v19;
        v20 = operator new(0x200u);
        v21 = (void *)*v62;
        v63 = v20;
        sub_453760(v21, (int)v19, (int)&v59, 1);
        v61 = 0;
        v65 = (int)v19;
        while ( v61 < v59 )
        {
          v22 = *(_DWORD *)(v65 + 4);
          v23 = v62;
          sub_4536A0((void *)*v62, (int)&v56, *(_DWORD *)v65);
          (*(void (__thiscall **)(int, void *, _DWORD, unsigned int *, _DWORD, int, _DWORD, _DWORD, _DWORD, int, int, int))(*(_DWORD *)a3[4] + 4))(
            a3[4],
            v63,
            0,
            &v64,
            *(_DWORD *)v65,
            v22,
            v23[149],
            v23[150],
            v23[151],
            1,
            1,
            1);
          v24 = v63;
          v25 = 0;
          if ( v64 )
          {
            v26 = a3[9];
            do
            {
              v27 = 0;
              if ( v26 )
              {
                while ( *(int *)&v68[4 * v27 + 48] >= 0
                     || v66[2 * v27] != v24[2 * v25]
                     || v66[2 * v27 + 1] != v24[2 * v25 + 1] )
                {
                  if ( ++v27 >= v26 )
                    goto LABEL_37;
                }
                *(_DWORD *)&v68[4 * v27 + 48] = v56;
                v67[2 * v27] = *(_DWORD *)v65;
                v67[2 * v27 + 1] = *(_DWORD *)(v65 + 4);
              }
LABEL_37:
              ++v25;
            }
            while ( v25 < v64 );
          }
          v28 = a3[9];
          v29 = 0;
          v30 = 0;
          if ( v28 )
          {
            while ( *(int *)&v68[4 * v30 + 48] >= 0 )
            {
              if ( ++v30 >= v28 )
                goto LABEL_43;
            }
            v29 = 1;
          }
LABEL_43:
          ++v61;
          v65 += 8;
          if ( !v29 )
            goto LABEL_46;
        }
        v24 = v63;
LABEL_46:
        operator delete(v60);
        operator delete(v24);
      }
      v31 = v62[3];
      v65 = 0;
      v63 = (void *)-1;
      if ( v31 >= 0 && *(int *)&v68[4 * v31 + 48] >= 1 )
      {
        v65 = *(_DWORD *)&v68[4 * v31 + 48];
        v63 = *(void **)(2268 * v31 + a3[10] + 8);
      }
      v57 = 0;
      if ( a3[9] )
      {
        v51 = 0;
        v64 = (unsigned int)v67;
        do
        {
          if ( *(int *)&v68[4 * v57 + 48] < 1 )
          {
            v38 = v57;
            goto LABEL_77;
          }
          v32 = v62[2];
          v33 = (v32 + 1) % a3[8];
          v59 = *(_DWORD *)(v51 + a3[10] + 8);
          v34 = (void *)v33;
          v52 = v59 == v33;
          if ( v59 == v33 || (v35 = (a3[8] + v32 - 1) % a3[8], v61 = 1, v59 == v35) )
            v61 = 0;
          v36 = v63 == v34;
          v37 = (a3[8] + v32 - 1) % a3[8];
          v60 = (void *)(v63 == (void *)v37);
          if ( v36 || (v54 = 1, v63 == (void *)v37) )
            v54 = 0;
          v38 = v57;
          v39 = *(_DWORD *)&v68[4 * v57 + 48];
          v40 = v65;
          if ( v39 >= v65 )
          {
            if ( v39 < v65 + v62[142] && (v52 && !v36 || v61 && v60) )
            {
LABEL_69:
              v41 = *(_DWORD *)v64;
              v42 = *(_DWORD *)(v64 + 4);
              v65 = *(_DWORD *)&v68[4 * v57 + 48];
              v63 = (void *)v59;
              v43 = v62;
              v62[3] = v57;
              v43[528] = v41;
              v43[529] = v42;
              goto LABEL_77;
            }
          }
          else
          {
            if ( v52 && (!v36 || 2 * v39 <= v65) || v61 && (v60 || v54 && 2 * v39 <= v65) )
              goto LABEL_69;
            v40 = v65;
          }
          if ( !v40 )
            goto LABEL_69;
LABEL_77:
          v51 += 2268;
          v64 += 8;
          v57 = v38 + 1;
        }
        while ( v38 + 1 < (unsigned int)a3[9] );
      }
      if ( (int)v62[3] >= 0 && !sub_453510(0, (int)&v64, v62[528], v62[529]) )
      {
        v44 = operator new(8 * v64);
        v50 = v62[529];
        v47 = v62[528];
        v60 = v44;
        sub_453590(*v62, a3[4], (int)v44, (int *)&v64, v47, v50);
        if ( (int)v64 > 0 )
        {
          v45 = (int *)v60;
          v53 = (void *)*v62;
          v55 = v62[142];
          v59 = v64;
          do
          {
            sub_4536A0(v53, (int)&v56, *v45);
            if ( v56 <= v55 )
            {
              v46 = v62;
              v62[530] = *v45;
              v46[531] = v45[1];
            }
            v45 += 2;
            --v59;
          }
          while ( v59 );
        }
        operator delete(v60);
      }
      return 0;
    }
  }
  return result;
}

// ===== sub_459EA0 @ 0x00459EA0..0x00459F25 =====
int __fastcall sub_459EA0(int a1, int a2, int a3)
{
  int v3; // edi
  int result; // eax
  unsigned int v5; // esi
  _DWORD *v6; // ebx
  int v7; // edi
  unsigned int v8; // edx
  _DWORD *i; // ecx
  unsigned int v10; // [esp+8h] [ebp-4h]

  v3 = a3;
  result = 0;
  if ( !a3 )
  {
    a3 = *(_DWORD *)(a2 + 40);
    v3 = a3;
  }
  v5 = 0;
  v10 = *(_DWORD *)(a2 + 36);
  if ( v10 )
  {
    v6 = (_DWORD *)(v3 + 2100);
    v7 = a1 - v3 - 2100;
    while ( 2 )
    {
      v8 = 8;
      for ( i = v6; ; ++i )
      {
        if ( v8 < 4 )
          return a3 + 2268 * v5;
        if ( *(_DWORD *)((char *)i + v7) != *i )
          break;
        v8 -= 4;
      }
      ++v5;
      v7 -= 2268;
      v6 += 567;
      if ( v5 < v10 )
        continue;
      break;
    }
    return 0;
  }
  return result;
}

// ===== sub_459F30 @ 0x00459F30..0x00459F51 =====
int __userpurge sub_459F30@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  _DWORD v5[2]; // [esp+0h] [ebp-8h] BYREF

  v5[1] = a2;
  v5[0] = a1;
  return sub_459EA0((int)v5, a3, a4);
}

// ===== sub_459F60 @ 0x00459F60..0x00459FA5 =====
int __userpurge sub_459F60@<eax>(_DWORD *a1@<eax>, int a2@<edx>, _DWORD *a3)
{
  int result; // eax
  unsigned int v5; // edi
  unsigned int v6; // ecx

  result = -1;
  if ( !a1 )
    a1 = *(_DWORD **)(a2 + 40);
  v5 = *(_DWORD *)(a2 + 36);
  v6 = 0;
  if ( v5 )
  {
    while ( !*a1 || *a3 != *a1 )
    {
      ++v6;
      a1 += 567;
      if ( v6 >= v5 )
        return result;
    }
    return v6;
  }
  return result;
}

// ===== sub_459FB0 @ 0x00459FB0..0x0045B584 =====
BOOL __thiscall sub_459FB0(int *this, int a2)
{
  int *v2; // edi
  int v3; // ecx
  int *v4; // esi
  int v5; // eax
  unsigned int v6; // ecx
  int v7; // eax
  char *v8; // ebx
  _DWORD *v9; // ecx
  int v10; // edx
  int v11; // eax
  int *v12; // ebx
  int v13; // eax
  int v14; // edx
  void **v15; // esi
  int v16; // eax
  int v17; // ecx
  int v18; // edx
  int v19; // ecx
  int v20; // esi
  int v21; // eax
  signed int v22; // eax
  int v23; // edx
  BOOL v24; // eax
  int v25; // eax
  _DWORD *v26; // esi
  int v27; // eax
  _DWORD *v28; // eax
  int v29; // ecx
  int v30; // esi
  int v31; // ecx
  int v32; // edx
  int v33; // ecx
  int v34; // eax
  bool v35; // zf
  int v36; // edx
  int v37; // ecx
  int v38; // esi
  int v39; // esi
  int v40; // edi
  int v41; // eax
  int *v42; // ecx
  int v43; // ecx
  int v44; // eax
  int v45; // eax
  int v46; // ecx
  int v47; // eax
  int v48; // eax
  unsigned int v49; // eax
  int v50; // edx
  int *v51; // eax
  int v52; // ecx
  int *v53; // eax
  int *v54; // esi
  int v55; // eax
  size_t v56; // eax
  int v57; // ecx
  int v58; // esi
  char *v59; // ecx
  unsigned int v60; // eax
  unsigned int v61; // edx
  int *v62; // eax
  int v63; // esi
  char *v64; // ecx
  int v65; // eax
  int v66; // edx
  int v67; // eax
  int v68; // eax
  unsigned int v69; // eax
  int v70; // eax
  unsigned int v71; // edx
  int v72; // eax
  int v73; // edx
  int v74; // eax
  int v75; // eax
  int v76; // ecx
  int v77; // eax
  int v78; // eax
  unsigned int v79; // eax
  int v80; // edx
  unsigned int v81; // ecx
  bool v82; // sf
  int v83; // ecx
  unsigned int v84; // esi
  int *v85; // edx
  int v86; // eax
  _DWORD *v87; // ecx
  _DWORD *v88; // ecx
  unsigned int v89; // eax
  _DWORD *v90; // edx
  unsigned int v91; // esi
  unsigned int i; // eax
  unsigned int v93; // eax
  int v94; // edx
  BOOL v95; // ecx
  int *v96; // esi
  int v97; // eax
  int v98; // esi
  char *v99; // ecx
  _DWORD *v100; // esi
  int v101; // eax
  int v102; // esi
  int v103; // eax
  _DWORD *v104; // ecx
  int v105; // ecx
  void **v106; // esi
  unsigned int v107; // edx
  int v108; // eax
  int v109; // esi
  int v110; // ecx
  _DWORD *v111; // eax
  int v112; // edx
  unsigned int v113; // eax
  int v114; // esi
  _DWORD *v115; // ecx
  const void *v117; // [esp+18h] [ebp-768h]
  int v118; // [esp+18h] [ebp-768h]
  size_t v119; // [esp+1Ch] [ebp-764h]
  int v120; // [esp+1Ch] [ebp-764h]
  int v121; // [esp+1Ch] [ebp-764h]
  int v122; // [esp+1Ch] [ebp-764h]
  int v123; // [esp+1Ch] [ebp-764h]
  int v124; // [esp+1Ch] [ebp-764h]
  int v125; // [esp+1Ch] [ebp-764h]
  int v126; // [esp+1Ch] [ebp-764h]
  int v127; // [esp+1Ch] [ebp-764h]
  int *v128; // [esp+30h] [ebp-750h]
  int *v129; // [esp+30h] [ebp-750h]
  int v130; // [esp+30h] [ebp-750h]
  int *v131; // [esp+34h] [ebp-74Ch]
  int v132; // [esp+38h] [ebp-748h]
  int v133; // [esp+38h] [ebp-748h]
  _DWORD *v134; // [esp+3Ch] [ebp-744h]
  unsigned int v135; // [esp+3Ch] [ebp-744h]
  unsigned int v136; // [esp+40h] [ebp-740h]
  unsigned int *v137; // [esp+40h] [ebp-740h]
  unsigned int v138; // [esp+40h] [ebp-740h]
  int *v139; // [esp+44h] [ebp-73Ch]
  int v140; // [esp+48h] [ebp-738h] BYREF
  int v141; // [esp+4Ch] [ebp-734h]
  unsigned int v142; // [esp+50h] [ebp-730h]
  int v143; // [esp+54h] [ebp-72Ch]
  size_t Size; // [esp+58h] [ebp-728h]
  int *v145; // [esp+5Ch] [ebp-724h]
  int v146; // [esp+60h] [ebp-720h]
  int v147; // [esp+64h] [ebp-71Ch]
  int v148; // [esp+68h] [ebp-718h] BYREF
  int v149; // [esp+6Ch] [ebp-714h] BYREF
  int v150; // [esp+70h] [ebp-710h]
  _DWORD *v151; // [esp+74h] [ebp-70Ch]
  int v152; // [esp+78h] [ebp-708h] BYREF
  int *v153; // [esp+7Ch] [ebp-704h]
  int v154; // [esp+80h] [ebp-700h]
  void *v155; // [esp+84h] [ebp-6FCh]
  void *v156; // [esp+88h] [ebp-6F8h]
  size_t v157; // [esp+8Ch] [ebp-6F4h]
  BOOL v158; // [esp+90h] [ebp-6F0h]
  int v159; // [esp+94h] [ebp-6ECh] BYREF
  int v160; // [esp+98h] [ebp-6E8h]
  unsigned int v161; // [esp+9Ch] [ebp-6E4h]
  int *v162; // [esp+A0h] [ebp-6E0h]
  int v163; // [esp+A4h] [ebp-6DCh]
  int v164; // [esp+A8h] [ebp-6D8h]
  _DWORD v165[5]; // [esp+ACh] [ebp-6D4h] BYREF
  _DWORD v166[6]; // [esp+C0h] [ebp-6C0h] BYREF
  _DWORD v167[6]; // [esp+D8h] [ebp-6A8h] BYREF
  _DWORD v168[6]; // [esp+F0h] [ebp-690h] BYREF
  _DWORD v169[28]; // [esp+108h] [ebp-678h] BYREF
  _DWORD Src[128]; // [esp+178h] [ebp-608h] BYREF
  int v171; // [esp+378h] [ebp-408h] BYREF
  _DWORD v172[127]; // [esp+37Ch] [ebp-404h]
  _DWORD v173[129]; // [esp+578h] [ebp-208h] BYREF

  v2 = this;
  v3 = this[3];
  v4 = 0;
  v139 = v2;
  v145 = 0;
  v5 = sub_4465D0(v3);
  v6 = v2[83];
  if ( v6 < v2[82] )
  {
    v4 = (int *)(v2[84] + 76 * v6);
    v145 = v4;
    v2[83] = v6 + 1;
  }
  sub_4465F0(v2[3], v5);
  if ( !v4 )
    return v4 != 0;
  v7 = v2[9];
  v143 = *(_DWORD *)(v2[5] + 4 * a2);
  v8 = (char *)operator new(2268 * v7);
  v119 = 2268 * v2[9];
  v117 = (const void *)v2[10];
  v155 = v8;
  memcpy_0(v8, v117, v119);
  v9 = v8;
  v10 = 2268 * v2[11];
  v11 = *(_DWORD *)&v8[v10 + 12];
  v12 = (int *)&v8[v10];
  if ( v11 < 0 )
    v151 = 0;
  else
    v151 = (char *)v155 + 2268 * v11;
  v12[525] = *v4;
  v9[567 * v2[11] + 526] = v4[1];
  v35 = v2[9] == 0;
  v147 = 0;
  if ( !v35 )
  {
    v142 = (unsigned int)v9;
    do
    {
      if ( *v9 )
      {
        sub_453290(*(_DWORD *)(v142 + 2100), *(_DWORD *)(v142 + 2104));
        v4 = v145;
        v2 = v139;
      }
      v9 = (_DWORD *)(v142 + 2268);
      ++v147;
      v142 += 2268;
    }
    while ( v147 < (unsigned int)v2[9] );
  }
  memcpy_0(Src, v2 + 12, 4 * v2[9]);
  v13 = v4[2];
  v132 = 0;
  v154 = v4[11];
  v152 = -1;
  if ( v13 )
  {
    if ( v13 == 6 )
    {
      v15 = (void **)sub_459F30(v145[4], v145[5], (int)v139, (int)v155);
      sub_459620(v12[152], v12, &v149, v15);
      sub_4537C0(*v15, (int)&v148, *v145, v145[1]);
      v120 = sub_4592B0((int)v139, v148);
      v16 = sub_459240((int)v15[2], v12[2], v139);
      sub_459370(v16, v12 + 11, v149, &v140, v12[152], v12 + 153, v15 + 11, v150, v120);
      v17 = v140;
      if ( v140 > (int)v15[7] )
      {
        v17 = (int)v15[7];
        v140 = v17;
      }
      v15[7] = (char *)v15[7] - v17;
      if ( (int)v15[7] > 0 )
        v18 = 0;
      else
        v18 = v12[161];
      v132 = v18 + v12[159] + v17 * v12[160];
      if ( v145[9] )
        v19 = v12[158];
      else
        v19 = 0;
      v121 = v145[1];
      v20 = *v145;
      v154 += v19 + v12[157];
      v21 = (*(int (__thiscall **)(int, int, int, int, int))(*(_DWORD *)v143 + 12))(v143, v145[4], v145[5], v20, v121);
      v4 = v145;
      v2 = v139;
      goto LABEL_92;
    }
    if ( v13 != 7 )
      goto LABEL_93;
    v128 = &v12[22 * v4[3] + 173];
    v22 = v12[22 * v4[3] + 186];
    Size = v22;
    if ( v22 < 0 )
    {
      v132 = v12[159];
    }
    else
    {
      if ( v22 )
      {
        v24 = v4[4] != *v4 || v4[5] != v4[1];
        (*(void (__thiscall **)(int, int *, _DWORD, int *, int, int, size_t, _DWORD, _DWORD, int, _DWORD, BOOL))(*(_DWORD *)v143 + 4))(
          v143,
          &v171,
          0,
          &v152,
          v4[4],
          v4[5],
          Size,
          0,
          0,
          1,
          0,
          v24);
      }
      else
      {
        v23 = v4[5];
        v171 = v4[4];
        v172[0] = v23;
        v152 = 1;
      }
      v146 = 0;
      if ( v152 )
      {
        while ( 1 )
        {
          v134 = (_DWORD *)sub_459F30(v172[2 * v146 - 1], v172[2 * v146], (int)v139, (int)v155);
          sub_459620(v128[3], v12, &v149, v134);
          v25 = 0x10000;
          if ( v128[1] )
          {
            v26 = v134;
          }
          else
          {
            if ( v172[2 * v146 - 1] == v145[4] && v172[2 * v146] == v145[5] )
            {
              v122 = v145[1];
              v118 = *v145;
            }
            else
            {
              v122 = v145[5];
              v118 = v145[4];
            }
            v26 = v134;
            sub_4537C0((void *)*v134, (int)&v148, v118, v122);
            v25 = sub_4592B0((int)v139, v148);
          }
          v123 = v25;
          v27 = sub_459240(v26[2], v12[2], v139);
          sub_459370(v27, v12 + 11, v149, &v140, v128[3], v128 + 4, v26 + 11, v150, v123);
          if ( (v26[1] & v12[1]) != 0 )
            v28 = v128 + 16;
          else
            v28 = v128 + 19;
          switch ( v128[1] )
          {
            case 0:
              v29 = v140;
              if ( v140 > v134[7] )
              {
                v29 = v134[7];
                v140 = v29;
              }
              v134[7] -= v29;
              if ( (int)v134[7] > 0 )
                v30 = *v28 + v29 * v28[1];
              else
                v30 = *v28 + v28[2] + v29 * v28[1];
              v132 += v30;
              goto LABEL_85;
            case 1:
              v31 = v140;
              if ( v140 > v134[8] - v134[7] )
              {
                v31 = v134[8] - v134[7];
                v140 = v31;
              }
              v134[7] += v31;
              v132 += *v28 + v31 * v28[1];
              goto LABEL_85;
            case 2:
              v32 = v140;
              if ( v140 > v134[v128[2] + 77] )
              {
                v32 = v134[v128[2] + 77];
                v140 = v32;
              }
              v134[v128[2] + 77] -= v32;
              v33 = *v28 + v32 * v28[1];
              v34 = v128[2];
              v132 += v33;
              if ( v34 >= 8 )
              {
                if ( v34 <= 9 )
                  goto LABEL_63;
                v35 = v34 == 31;
                goto LABEL_62;
              }
              goto LABEL_85;
            case 3:
            case 4:
              v134[v128[2] + 77] = v128[8];
              v36 = v140;
              v134[v128[2] + 109] = v140;
              v37 = v128[2];
              v132 += *v28 + v36 * v28[1];
              if ( v37 < 8 )
                goto LABEL_85;
              if ( v37 > 9 )
              {
                v35 = v37 == 31;
LABEL_62:
                if ( !v35 )
                  goto LABEL_85;
              }
LABEL_63:
              if ( *v134 == *v12 )
                goto LABEL_85;
              sub_459620(0x100u, v134, &v149, 0);
              v38 = v134[75];
              Size = v38 + v134[108] - v149 < 0 ? 0 : v38 + v134[108] - v149;
              if ( Size == v38 )
                goto LABEL_85;
              v147 = sub_459F60(v155, (int)v139, v134);
              v39 = -1;
              v40 = 0;
              v41 = 0;
              v141 = -1;
              v142 = 0;
              if ( Src[0] < 0 )
                goto LABEL_85;
              v42 = Src;
              break;
            default:
              goto LABEL_85;
          }
          while ( 1 )
          {
            v43 = *v42;
            if ( v147 != v43 )
              break;
            if ( v39 >= 0 )
            {
              v141 = v41 - 1;
              goto LABEL_79;
            }
            v40 = -1;
            v39 = v41 + 1;
            v142 = -1;
LABEL_75:
            v42 = &Src[++v41];
            if ( *v42 < 0 )
              goto LABEL_79;
          }
          if ( v40 > 0 )
            goto LABEL_75;
          if ( (signed int)Size >= *((_DWORD *)v155 + 567 * v43 + 75) )
          {
            v40 = v142;
            goto LABEL_75;
          }
          if ( v39 < 0 )
          {
            v40 = 1;
            v39 = v41;
            v142 = 1;
            goto LABEL_75;
          }
          v40 = v142;
          v141 = v41 - 1;
LABEL_79:
          if ( v39 != v141 )
          {
            if ( v141 < 0 )
              v141 = v41 - 1;
            Size = 4 * v41 + 4;
            memcpy_0(v173, Src, Size);
            v44 = v39;
            if ( v40 != 1 )
              v44 = v141;
            v173[v44] = v147;
            memcpy_0(&v173[v39 + v40], &Src[v39], 4 * (v141 - v39) + 4);
            memcpy_0(Src, v173, Size);
          }
LABEL_85:
          if ( ++v146 >= (unsigned int)v152 )
          {
            v4 = v145;
            v2 = v139;
            break;
          }
        }
      }
    }
    if ( v4[9] )
      v45 = v128[15];
    else
      v45 = 0;
    v154 += v45 + v128[14];
    v21 = (*(int (__thiscall **)(int, int, int, int, int))(*(_DWORD *)v143 + 12))(v143, v4[4], v4[5], *v4, v4[1]);
LABEL_92:
    v152 = v21 - 2;
    goto LABEL_93;
  }
  v14 = v12[148];
  v154 += v12[147];
  v132 = v14;
LABEL_93:
  v46 = v4[9];
  v47 = v4[10];
  v133 = v46 * v12[146] + v132;
  if ( v47 < 1 )
  {
    if ( v47 < 0 )
      v133 += v12[172] * (v2[6] + v2[7]);
  }
  else
  {
    v133 += v47 * v12[172];
  }
  v147 = 0;
  if ( v4[12] )
  {
    v133 += v12[4] + v46 * v46 * v12[5];
    if ( v4[2] || (v147 = 1, v12[149] != 1) )
      v147 = 0;
  }
  v12[75] = v154;
  sub_459620(0x100u, v12, &v149, 0);
  v48 = *(_DWORD *)v143;
  v154 += v12[108] - v149;
  v49 = (*(int (__thiscall **)(int))(v48 + 8))(v143);
  v50 = v139[6];
  v135 = v49;
  v164 = *v145 + v50 * v145[1];
  memset(v167, 0, sizeof(v167));
  if ( v49 )
    memset32(v169, v12[7], v49);
  v156 = operator new(28 * v50 * v139[7]);
  v163 = 0;
  if ( Src[0] >= 0 )
  {
    v51 = Src;
    v162 = Src;
    while ( 1 )
    {
      v52 = *v51;
      v53 = (int *)((char *)v155 + 2268 * *v51);
      v131 = v53;
      if ( v53[7] > 0 )
        break;
LABEL_208:
      v82 = (int)Src[++v163] < 0;
      v51 = &Src[v163];
      v162 = v51;
      if ( v82 )
        goto LABEL_209;
    }
    v54 = v53;
    v55 = v53[75];
    if ( v55 > v154 || v55 == v154 && v139[11] < v52 )
      goto LABEL_209;
    if ( (v54[1] & v12[1]) != 0 )
    {
      if ( v151 )
      {
        v35 = *(_DWORD *)(28 * (v151[525] + v139[6] * v151[526]) + v131[533]) == 0;
        v157 = 28 * (v151[525] + v139[6] * v151[526]);
        if ( !v35 )
        {
          sub_459300(0, v165, v131);
          v98 = v143;
          sub_453800((_DWORD *)v143, (void *)*v131, (void *)v131[141], v131[142], 1, v99, 1, 0);
          sub_453D80(*v131, (int)v165, v98, v156, 0);
          if ( sub_45B5E0(v139, (char *)v156 + v157, v151[527]) )
          {
            v100 = v151;
            Size = v151[7] >= 1;
            sub_459620(v131[152], v131, &v149, v151);
            (*(void (__thiscall **)(int, int *, _DWORD, _DWORD, int))(*(_DWORD *)v143 + 20))(
              v143,
              &v159,
              *v100,
              v168[v100[527]],
              1);
            sub_4537C0((void *)*v100, (int)&v148, v159, v160);
            v126 = sub_4592B0((int)v139, v148);
            v101 = sub_459240(v100[2], v131[2], v139);
            sub_459370(v101, v131 + 11, v149, &v140, v131[152], v131 + 153, v100 + 11, v150, v126);
            v102 = v100[7];
            v103 = v140;
            if ( v140 > v102 )
            {
              v103 = Size != 0 ? v102 : 0;
              v140 = v103;
            }
            v104 = v151;
            v151[7] -= v103;
            if ( (int)v104[7] <= 0 && Size )
              v105 = v12[161];
            else
              v105 = 0;
            v133 += v105 + v12[159] + v103 * v12[160];
          }
        }
      }
      goto LABEL_208;
    }
    v56 = v54[532] != 0;
    v57 = 28 * v164;
    v161 = 0;
    v141 = 28 * v164;
    v35 = *(_DWORD *)(28 * v164 + v54[v56 + 533]) == 0;
    Size = v56;
    if ( !v35 )
    {
      sub_459300(0, v165, v131);
      v58 = v143;
      sub_453800((_DWORD *)v143, (void *)*v131, (void *)v131[141], v131[142], 1, v59, 1, 0);
      sub_453D80(*v131, (int)v165, v58, v156, 0);
      v60 = sub_45B5E0(v139, (char *)v156 + v141, v152);
      v54 = v131;
      v57 = v141;
      v161 = v60;
      v56 = Size;
    }
    v142 = v161;
    v129 = &v171;
    v61 = 0;
    v62 = &v54[v56 + 535];
    v136 = 0;
    v146 = (int)(v54 + 174);
    v153 = v62;
    do
    {
      v169[v61 + 12] = 0;
      if ( *(_DWORD *)(v146 - 4) && *(_DWORD *)(v57 + *v62) )
      {
        sub_459300(v136 + 1, v165, v131);
        v63 = v143;
        sub_453800((_DWORD *)v143, (void *)*v131, (void *)v131[141], v131[142], 1, v64, 1, 0);
        sub_453D80(*v131, (int)v165, v63, v156, 0);
        if ( *(_DWORD *)v146 )
        {
          if ( *(_DWORD *)v146 == 4 )
            v169[v136 + 12] = *(_DWORD *)((char *)v156 + v141);
        }
        else
        {
          v169[v136 + 12] = sub_45B5E0(v139, (char *)v156 + v141, v152);
        }
        if ( v142 || (v35 = v169[v136 + 12] == 0, v142 = 0, !v35) )
          v142 = 1;
        v57 = v141;
        v54 = v131;
      }
      v146 += 88;
      v129 += 6;
      v61 = v136 + 1;
      v62 = v153 + 2;
      v136 = v61;
      v153 += 2;
    }
    while ( v61 < 0x10 );
    if ( v54[532] )
    {
      if ( v12[171] )
        v146 = v12[171];
      else
        v146 = v139[85];
    }
    else
    {
      v146 = 100;
    }
    v130 = 0;
    if ( v135 )
    {
      while ( v152 >= 0 && v152 != v130 )
      {
LABEL_150:
        if ( ++v130 >= v135 )
          goto LABEL_151;
      }
      sub_453300(v130 + 2);
      if ( v161 )
      {
        v158 = v169[v130] >= 1;
        sub_459620(v131[152], v131, &v149, v12);
        (*(void (__thiscall **)(int, int *, int, _DWORD, int))(*(_DWORD *)v143 + 20))(v143, &v159, *v12, v168[v130], 1);
        sub_4537C0((void *)*v12, (int)&v148, v159, v160);
        v124 = sub_4592B0((int)v139, v148);
        v65 = sub_459240(v12[2], v131[2], v139);
        sub_459370(v65, v131 + 11, v149, &v140, v131[152], v131 + 153, v12 + 11, v150, v124);
        v66 = v140;
        v67 = v169[v130] - v140;
        v169[v130] = v67;
        if ( v67 <= 0 && v158 )
          v68 = v12[164];
        else
          v68 = 0;
        v167[v130] += v146 * (v68 + v12[162] + v66 * v12[163]) / 100;
        v54 = v131;
      }
      v137 = (unsigned int *)(v54 + 176);
      v69 = 0;
      v158 = 0;
      v153 = &v172[v130 - 1];
      while ( 2 )
      {
        if ( v169[v69 + 12] )
        {
          v70 = *(v137 - 2);
          if ( v70 )
          {
            if ( v70 == 4 )
            {
              sub_459620(*v137, v131, &v149, v12);
              v77 = sub_459240(v12[2], v131[2], v139);
              sub_459370(v77, v131 + 11, v149, &v140, *v137, v137 + 1, v12 + 11, v150, 0x10000);
              v12[*(v137 - 1) + 77] = v137[5];
              v78 = v140;
              v12[*(v137 - 1) + 109] = v140;
              v76 = v12[168] + v78 * v12[169];
              goto LABEL_148;
            }
          }
          else
          {
            v71 = *v137;
            v157 = v169[v130] >= 1;
            sub_459620(v71, v131, &v149, v12);
            (*(void (__thiscall **)(int, int *, int, int, int))(*(_DWORD *)v143 + 20))(v143, &v159, *v12, *v153, 1);
            sub_4537C0((void *)*v12, (int)&v148, v159, v160);
            v125 = sub_4592B0((int)v139, v148);
            v72 = sub_459240(v12[2], v131[2], v139);
            sub_459370(v72, v131 + 11, v149, &v140, *v137, v137 + 1, v12 + 11, v150, v125);
            v73 = v140;
            v74 = v169[v130] - v140;
            v169[v130] = v74;
            if ( v74 <= 0 && v157 )
              v75 = v12[167];
            else
              v75 = 0;
            v76 = v75 + v12[165] + v73 * v12[166];
LABEL_148:
            v167[v130] += v146 * v76 / 100;
            v54 = v131;
          }
        }
        v137 += 22;
        v153 += 6;
        v69 = v158 + 1;
        v158 = v69;
        if ( v69 >= 0x10 )
          goto LABEL_150;
        continue;
      }
    }
LABEL_151:
    if ( v142 || v54[532] )
    {
LABEL_164:
      if ( !v145[2] && v142 && v145[7] < 0 )
      {
        v84 = 0;
        memset(v166, 0, sizeof(v166));
        v138 = 0;
        if ( v135 )
        {
          v85 = v131;
          v86 = v131[Size + 533];
          v157 = (size_t)&v131[Size + 537];
          v87 = (_DWORD *)(v141 + v86 + 4);
          Size = (size_t)v87;
          while ( 1 )
          {
            v166[v84] += *v87;
            v88 = (_DWORD *)v157;
            v89 = 0;
            v90 = v85 + 196;
            do
            {
              if ( v169[v89 + 12] && !*(v90 - 22) )
                v166[v138] += *(_DWORD *)(*(v88 - 2) + v141 + 4);
              if ( v169[v89 + 13] && !*v90 )
                v166[v138] += *(_DWORD *)(v141 + *v88 + 4);
              if ( v169[v89 + 14] && !v90[22] )
                v166[v138] += *(_DWORD *)(v88[2] + v141 + 4);
              if ( v169[v89 + 15] && !v90[44] )
                v166[v138] += *(_DWORD *)(v88[4] + v141 + 4);
              v89 += 4;
              v90 += 88;
              v88 += 8;
            }
            while ( v89 < 0x10 );
            v141 += 4;
            v84 = v138 + 1;
            v87 = (_DWORD *)(Size + 4);
            v138 = v84;
            Size += 4;
            if ( v84 >= v135 )
              break;
            v85 = v131;
          }
        }
        v91 = v166[0];
        for ( i = 1; i < v135; ++i )
        {
          if ( v91 < v166[i] )
            v91 = v166[i];
        }
        v93 = 0;
        v94 = 0;
        v153 = 0;
        if ( !v135 )
          goto LABEL_195;
        do
        {
          v95 = v91 == v166[v93];
          v168[v93] = v95;
          if ( v95 )
          {
            ++v94;
            v153 = (int *)v93;
          }
          ++v93;
        }
        while ( v93 < v135 );
        if ( v94 == 1 )
        {
          v145[7] = (int)v153;
        }
        else
        {
LABEL_195:
          v96 = v145;
          v97 = (*(int (__thiscall **)(int, int, int, int, int))(*(_DWORD *)v143 + 12))(
                  v143,
                  v131[525],
                  v131[526],
                  *v145,
                  v145[1])
              - 2;
          if ( v168[v97] )
            v96[7] = v97;
          else
            v96[7] = (int)v153;
        }
      }
      goto LABEL_208;
    }
    v79 = v163 + 1;
    v80 = v54[143] + v54[144] * ((3 * v54[142]) >> 2) + v54[75];
    v81 = 0;
    v54[75] = v80;
    if ( (int)Src[v79] < 0 )
      goto LABEL_159;
    do
    {
      if ( !v81 )
      {
        v54 = v131;
        if ( v80 < *((_DWORD *)v155 + 567 * Src[v79] + 75) )
          v81 = v79;
      }
      ++v79;
    }
    while ( (int)Src[v79] >= 0 );
    if ( v81 )
    {
      if ( v79 < v81 )
        goto LABEL_162;
    }
    else
    {
LABEL_159:
      v81 = v79;
    }
    do
    {
      Src[v79 + 1] = Src[v79];
      --v79;
    }
    while ( v79 >= v81 );
LABEL_162:
    v82 = v54[3] < 0;
    Src[v81] = *v162;
    v54[532] = 1;
    if ( !v82 )
    {
      v83 = v54[531];
      v54[525] = v54[530];
      v54[526] = v83;
    }
    goto LABEL_164;
  }
LABEL_209:
  operator delete(v156);
  if ( v147 )
  {
    sub_453410(v143, v12[141], v12[142], -1, -1, 1);
    sub_4534A0((int)v168, (int)v169, v151[525], v151[526]);
    v147 = 0x80000000;
    v142 = 0;
    if ( v135 )
    {
      do
      {
        if ( (int)v168[v142] >= 0 )
        {
          v106 = (void **)v151;
          v107 = v12[152];
          Size = v151[7] >= 1;
          sub_459620(v107, v12, &v149, v151);
          sub_4537C0(*v106, (int)&v148, v169[2 * v142], v169[2 * v142 + 1]);
          v127 = sub_4592B0((int)v139, v148);
          v108 = sub_459240((int)v106[2], v12[2], v139);
          sub_459370(v108, v12 + 11, v149, &v140, v12[152], v12 + 153, v106 + 11, v150, v127);
          v109 = (int)v106[7];
          v110 = v140;
          if ( v140 > v109 )
          {
            v110 = Size != 0 ? v109 : 0;
            v140 = v110;
          }
          v111 = v151;
          v151[7] -= v110;
          if ( (int)v111[7] <= 0 && Size )
            v112 = v12[161];
          else
            v112 = 0;
          if ( v147 < v112 + v12[159] + v110 * v12[160] )
            v147 = v112 + v12[159] + v110 * v12[160];
        }
        ++v142;
      }
      while ( v142 < v135 );
      if ( v147 != 0x80000000 )
        v133 += v147 * v139[86] / 100;
    }
  }
  v113 = 0;
  if ( v135 )
  {
    v114 = v152;
    v115 = v145 + 13;
    do
    {
      if ( v114 < 0 || v114 == v113 )
        *v115 = v133 + v167[v113];
      ++v113;
      ++v115;
    }
    while ( v113 < v135 );
  }
  operator delete(v155);
  v4 = v145;
  return v4 != 0;
}

// ===== sub_45B5A0 @ 0x0045B5A0..0x0045B5B1 =====
BOOL __cdecl sub_45B5A0(int *a1, int a2)
{
  return sub_459FB0(a1, a2);
}

// ===== sub_45B5C0 @ 0x0045B5C0..0x0045B5D5 =====
int __cdecl sub_45B5C0(const void *a1, const void *a2)
{
  return *(_DWORD *)(*(_DWORD *)a2 + 24) - *(_DWORD *)(*(_DWORD *)a1 + 24);
}

// ===== sub_45B5E0 @ 0x0045B5E0..0x0045B667 =====
int __userpurge sub_45B5E0@<eax>(int a1@<ecx>, unsigned int a2@<edi>, int a3, _DWORD *a4, int a5)
{
  int result; // eax
  unsigned int i; // esi
  int v7; // eax
  _DWORD v9[6]; // [esp+8h] [ebp-1Ch] BYREF

  result = 0;
  if ( *a4 )
  {
    for ( i = 0; i < a2; ++i )
    {
      if ( a5 < 0 || a5 == i )
      {
        (*(void (__thiscall **)(_DWORD, _DWORD *, unsigned int, int))(**(_DWORD **)(a3 + 16) + 16))(
          *(_DWORD *)(a3 + 16),
          v9,
          i,
          1);
        v7 = 0;
        while ( !a4[v9[v7] + 1] )
        {
          if ( ++v7 >= a2 )
            goto LABEL_10;
        }
        *(_DWORD *)(a1 + 4 * i) = v9[v7];
      }
LABEL_10:
      ;
    }
    return 1;
  }
  return result;
}

// ===== sub_45B670 @ 0x0045B670..0x0045B685 =====
int __stdcall sub_45B670(int a1)
{
  return (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(a1 - 8) + 4))(*(_DWORD *)(a1 - 8));
}

// ===== sub_45B690 @ 0x0045B690..0x0045B706 =====
_DWORD *__stdcall sub_45B690(_DWORD *a1)
{
  sub_45BE60(a1);
  *a1 = &DCTraditionalVR::`vftable';
  a1[3] = &DCTraditionalVR::`vftable';
  a1[4] = &DCTraditionalVR::`vftable';
  a1[56] = &DCTraditionalVR::`vftable';
  a1[57] = &DCTraditionalVR::`vftable';
  return a1;
}

// ===== sub_45B710 @ 0x0045B710..0x0045B732 =====
void *__thiscall sub_45B710(void *this, char a2)
{
  sub_45B740(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_45B740 @ 0x0045B740..0x0045B7AB =====
int __stdcall sub_45B740(_DWORD *a1)
{
  *a1 = &DCTraditionalVR::`vftable';
  a1[3] = &DCTraditionalVR::`vftable';
  a1[4] = &DCTraditionalVR::`vftable';
  a1[56] = &DCTraditionalVR::`vftable';
  a1[57] = &DCTraditionalVR::`vftable';
  return sub_45BF40(a1);
}

// ===== sub_45B7B0 @ 0x0045B7B0..0x0045B85A =====
int __stdcall sub_45B7B0(int a1)
{
  int result; // eax
  int v2; // eax
  int v3; // esi
  _DWORD v4[2]; // [esp+14h] [ebp-28h] BYREF
  _DWORD v5[2]; // [esp+1Ch] [ebp-20h] BYREF
  _DWORD v6[6]; // [esp+24h] [ebp-18h] BYREF

  result = sub_45C0B0(a1);
  if ( result >= 0 )
  {
    v2 = sub_45F020();
    v3 = v2;
    if ( v2
      && (*(int (__stdcall **)(int, _DWORD, _DWORD *, _DWORD, int))(*(_DWORD *)v2 + 76))(v2, 0, v4, 0, 0x2000) >= 0 )
    {
      sub_45E600(v5);
      v6[2] = v5[0];
      v6[0] = v4[1];
      v6[1] = v4[0];
      v6[3] = v5[1];
      v6[4] = 1;
      v6[5] = sub_407B30(1);
      sub_40A620((int)v6, 0);
      (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)v3 + 80))(v3, 0);
      return 0;
    }
    else
    {
      return -2147467259;
    }
  }
  return result;
}

// ===== sub_45B860 @ 0x0045B860..0x0045BABF =====
int __thiscall sub_45B860(_DWORD *this, int a2)
{
  int v3; // edx
  unsigned int v4; // edi
  unsigned int v5; // esi
  int v6; // eax
  double v7; // st7
  unsigned int v8; // edi
  double v9; // st6
  unsigned int v10; // edx
  double v11; // st5
  double v12; // st4
  int v13; // esi
  int v14; // edx
  int v15; // eax
  unsigned int v16; // ecx
  bool v17; // zf
  int v18; // eax
  int v19; // edi
  int v21; // [esp+0h] [ebp-60h] BYREF
  _DWORD v22[5]; // [esp+10h] [ebp-50h] BYREF
  int v23; // [esp+24h] [ebp-3Ch]
  __int64 v24; // [esp+28h] [ebp-38h] BYREF
  _DWORD v25[2]; // [esp+30h] [ebp-30h] BYREF
  _DWORD *v26; // [esp+38h] [ebp-28h]
  __int64 v27; // [esp+3Ch] [ebp-24h] BYREF
  int v28; // [esp+44h] [ebp-1Ch]
  int v29; // [esp+48h] [ebp-18h]
  int v30; // [esp+4Ch] [ebp-14h]
  int *v31; // [esp+50h] [ebp-10h]
  int v32; // [esp+5Ch] [ebp-4h]

  v31 = &v21;
  v26 = this;
  if ( !a2 )
    return -2147467261;
  sub_45E600(&v24);
  v4 = v24;
  if ( this[89] > (unsigned int)v24 )
    return v3;
  v5 = HIDWORD(v24);
  if ( this[90] > HIDWORD(v24) )
    return v3;
  v6 = sub_45F020();
  v28 = v6;
  if ( !v6 )
    return v3;
  if ( (*(int (__stdcall **)(int, _DWORD, _DWORD *, _DWORD, int))(*(_DWORD *)v6 + 76))(v6, 0, v25, 0, 0x2000) < 0 )
    return -2147467259;
  HIDWORD(v27) = v5;
  v7 = (double)v4 / (double)v5;
  v8 = this[89];
  HIDWORD(v27) = v8;
  v9 = (double)v8;
  v10 = this[90];
  HIDWORD(v27) = v10;
  v11 = (double)v10;
  v12 = v9 / v11;
  if ( v9 / v11 > v7 )
  {
    v29 = v8;
    HIDWORD(v27) = HIWORD(v30) | 0xC00;
    v24 = (__int64)(v12 * v11 / v7 + 0.5);
    v13 = v24;
    v30 = v24;
  }
  else
  {
    v27 = (__int64)(v7 * v9 / v12 + 0.5);
    v29 = v27;
    v13 = v10;
    v30 = v10;
  }
  v23 = sub_407B30(1);
  v15 = this[92];
  if ( v15 )
    v16 = v25[0] * (v13 - ((unsigned int)(v13 - v14) >> 1) - 1);
  else
    v16 = (unsigned int)(v13 - v14) >> 1;
  v22[0] = v25[1] + v16 + v23 * ((v29 - v8) >> 1);
  v17 = v15 == 0;
  v18 = v25[0];
  if ( !v17 )
    v18 = -v25[0];
  v22[1] = v18;
  v22[2] = v8;
  v22[3] = v14;
  v22[4] = 1;
  v32 = 0;
  sub_45C170(v26, v22);
  v32 = -1;
  v19 = v28;
  (*(void (__stdcall **)(int, _DWORD))(*(_DWORD *)v28 + 80))(v28, 0);
  sub_45FC30(v19, v29 - 1, v13 - 1);
  sub_45FF00((char *)&v27 + 4);
  return 0;
}

// ===== sub_45BAC0 @ 0x0045BAC0..0x0045BACD =====
int __stdcall sub_45BAC0(int a1, int a2, int a3)
{
  return sub_4C1840(a1 - 212, a2, a3);
}

// ===== sub_45BAD0 @ 0x0045BAD0..0x0045BADD =====
int __stdcall sub_45BAD0(int a1, int a2, int a3)
{
  return sub_4C1840(a1 - 216, a2, a3);
}

// ===== sub_45BAE0 @ 0x0045BAE0..0x0045BAED =====
int __stdcall sub_45BAE0(int a1)
{
  return sub_4BF8F0(a1 - 212);
}

// ===== sub_45BAF0 @ 0x0045BAF0..0x0045BAFD =====
int __stdcall sub_45BAF0(int a1)
{
  return sub_4BF8F0(a1 - 216);
}

// ===== sub_45BB00 @ 0x0045BB00..0x0045BB0D =====
int __stdcall sub_45BB00(int a1)
{
  return sub_45B670(a1 - 212);
}

// ===== sub_45BB10 @ 0x0045BB10..0x0045BB1D =====
int __stdcall sub_45BB10(int a1)
{
  return sub_45B670(a1 - 216);
}

// ===== sub_45BB20 @ 0x0045BB20..0x0045BBB1 =====
_DWORD *__stdcall sub_45BB20(_DWORD *a1, int a2, int a3, int a4)
{
  sub_45BE60(a1);
  *a1 = &DCVideoImageSender::`vftable';
  a1[3] = &DCVideoImageSender::`vftable';
  a1[4] = &DCVideoImageSender::`vftable';
  a1[56] = &DCVideoImageSender::`vftable';
  a1[57] = &DCVideoImageSender::`vftable';
  a1[96] = a2;
  a1[97] = a3;
  a1[98] = a4;
  return a1;
}

// ===== sub_45BBC0 @ 0x0045BBC0..0x0045BBE2 =====
void *__thiscall sub_45BBC0(void *this, char a2)
{
  sub_45BBF0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_45BBF0 @ 0x0045BBF0..0x0045BC5B =====
int __stdcall sub_45BBF0(_DWORD *a1)
{
  *a1 = &DCVideoImageSender::`vftable';
  a1[3] = &DCVideoImageSender::`vftable';
  a1[4] = &DCVideoImageSender::`vftable';
  a1[56] = &DCVideoImageSender::`vftable';
  a1[57] = &DCVideoImageSender::`vftable';
  return sub_45BF40(a1);
}

// ===== sub_45BC60 @ 0x0045BC60..0x0045BCF7 =====
int __thiscall sub_45BC60(int *this, int a2)
{
  int result; // eax
  int v4; // esi
  _DWORD v5[6]; // [esp+10h] [ebp-18h] BYREF

  result = sub_45C0B0(a2);
  if ( result >= 0 )
  {
    v4 = -2147467259;
    if ( sub_407DA0(this[97], (_DWORD *)this[96], this[89], this[90], 1u) )
    {
      if ( sub_407C40(this[97], this[96]) )
      {
        sub_407F20(this[96], this[97], v5);
        sub_40A620((int)v5, 0);
        v4 = 0;
        sub_407C90(this[97], this[96]);
      }
    }
    return v4;
  }
  return result;
}

// ===== sub_45BD00 @ 0x0045BD00..0x0045BE31 =====
int __thiscall sub_45BD00(_DWORD *this, int a2)
{
  int v3; // ecx
  int v4; // ecx
  int v6; // [esp+0h] [ebp-40h] BYREF
  int v7; // [esp+10h] [ebp-30h] BYREF
  int v8; // [esp+14h] [ebp-2Ch]
  int v9; // [esp+1Ch] [ebp-24h]
  _DWORD *v10; // [esp+28h] [ebp-18h]
  int v11; // [esp+2Ch] [ebp-14h]
  int *v12; // [esp+30h] [ebp-10h]
  int v13; // [esp+3Ch] [ebp-4h]

  v12 = &v6;
  v10 = this;
  v11 = -2147467259;
  if ( !a2 )
    return -2147467261;
  if ( sub_407760(this[96]) || !sub_407C40(this[97], v3) )
    return -2147467259;
  if ( sub_407F20(this[96], this[97], &v7) )
  {
    if ( this[92] )
    {
      v7 += v8 * (v9 - 1);
      v8 = -v8;
    }
    v13 = 0;
    sub_45C170(this, &v7);
    if ( sub_45BE40(v4, this[98]) )
      v11 = 0;
    v13 = -1;
  }
  sub_407C90(this[97], this[96]);
  return v11;
}

// ===== sub_45BE40 @ 0x0045BE40..0x0045BE59 =====
int __fastcall sub_45BE40(int a1, int a2)
{
  int result; // eax

  result = 1;
  if ( a2 )
  {
    if ( *(_DWORD *)a2 )
      return (*(int (__cdecl **)(_DWORD))a2)(*(_DWORD *)(a2 + 4));
  }
  return result;
}

// ===== sub_45BE60 @ 0x0045BE60..0x0045BF0B =====
_DWORD *__userpurge sub_45BE60@<eax>(_DWORD *a1@<edi>, _DWORD *a2)
{
  int v3; // [esp+0h] [ebp-18h]

  sub_4C32D0(&unk_4E6540, 0, 0, v3);
  *a2 = &DCVideoRenderer::`vftable';
  a2[3] = &DCVideoRenderer::`vftable';
  a2[4] = &DCVideoRenderer::`vftable';
  a2[56] = &DCVideoRenderer::`vftable';
  a2[57] = &DCVideoRenderer::`vftable';
  if ( a1 )
  {
    a2[88] = -1;
    *a1 = 0;
  }
  return a2;
}

// ===== sub_45BF10 @ 0x0045BF10..0x0045BF32 =====
void *__thiscall sub_45BF10(void *this, char a2)
{
  sub_45BF40(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_45BF40 @ 0x0045BF40..0x0045BFAA =====
int __stdcall sub_45BF40(_DWORD *a1)
{
  *a1 = &DCVideoRenderer::`vftable';
  a1[3] = &DCVideoRenderer::`vftable';
  a1[4] = &DCVideoRenderer::`vftable';
  a1[56] = &DCVideoRenderer::`vftable';
  a1[57] = &DCVideoRenderer::`vftable';
  return sub_4C3340();
}

// ===== sub_45BFB0 @ 0x0045BFB0..0x0045C0A6 =====
int __thiscall sub_45BFB0(_DWORD *this, _DWORD *a2)
{
  int v3; // esi
  _DWORD *v5; // ebx

  v3 = -2147467259;
  if ( !a2 )
    return -2147467261;
  if ( !sub_445510(a2 + 11, dword_4DB884) )
    return -2147024809;
  if ( sub_445510(a2, "vids") )
  {
    v5 = a2 + 4;
    if ( sub_445510(a2 + 4, dword_4DB814) )
    {
      this[88] = 0;
      return 0;
    }
    if ( sub_445510(v5, dword_4DB804) )
    {
      this[88] = 1;
      return 0;
    }
    if ( sub_445510(v5, dword_4DB7F4) )
    {
      this[88] = 2;
      return 0;
    }
    if ( sub_445510(v5, dword_4DB7E4) )
    {
      this[88] = 3;
      return 0;
    }
  }
  return v3;
}
