#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_448320 @ 0x00448320..0x004484AB =====
int __cdecl sub_448320(_DWORD *a1, int *a2)
{
  void *v2; // esi
  int v3; // edx
  int v4; // eax
  int v5; // edi
  int v6; // ebx
  int v7; // ecx
  BOOL v8; // eax
  int v9; // edx
  _DWORD *v10; // eax
  _DWORD *v11; // edi
  int v12; // ebx
  int v13; // ecx
  unsigned int v14; // eax
  _DWORD *v16; // [esp+10h] [ebp-40h]
  int v17; // [esp+14h] [ebp-3Ch]
  _DWORD v18[4]; // [esp+18h] [ebp-38h] BYREF
  int v19[4]; // [esp+28h] [ebp-28h] BYREF
  int v20[6]; // [esp+38h] [ebp-18h] BYREF

  v2 = a1;
  sub_42BBB0((int)a1);
  sub_42BBD0(0, v3, a1);
  sub_42B9B0((int)a1);
  v4 = sub_42B550((int)a1, 0);
  sub_42B540(v4, 1);
  sub_42CA00((int)a1);
  v5 = *a2;
  v6 = -2147483647;
  if ( *a2 > 0 && v5 <= 256 )
  {
    sub_42C2A0(v18, a1);
    v7 = 0;
    v8 = 1;
    if ( v5 <= 0 )
    {
LABEL_12:
      v10 = (_DWORD *)a2[1];
      v16 = v10;
      v17 = 0;
      if ( v5 > 0 )
      {
        do
        {
          v11 = (_DWORD *)v10[1];
          v12 = 0;
          if ( (unsigned __int16)*v10 )
          {
            do
            {
              if ( *v11 )
              {
                v13 = v11[3];
                if ( v13 != -1 && v12 == v10[2] && v11[4] != -1 )
                  v13 = v11[4];
                if ( sub_407F20(dword_565D70, v13, v20) )
                  sub_42B5B0(a1, v19, v18[0] + v11[1], v18[1] + v11[2], v20, 0, 0);
                v10 = v16;
              }
              ++v12;
              v11 += 15;
            }
            while ( v12 < (unsigned __int16)*v10 );
          }
          v10 += 13;
          v16 = v10;
          ++v17;
        }
        while ( v17 < *a2 );
        v2 = a1;
      }
      v6 = 0;
    }
    else
    {
      v9 = 0;
      while ( v8 )
      {
        v8 = (unsigned __int16)*(_DWORD *)(a2[1] + v9) && (unsigned __int16)*(_DWORD *)(a2[1] + v9) <= 0x100u;
        ++v7;
        v9 += 52;
        if ( v7 >= v5 )
        {
          if ( v8 )
            goto LABEL_12;
          break;
        }
      }
      v6 = -2147483646;
    }
  }
  sub_42C2F0(v19, (int)v2);
  v14 = (*(int (__thiscall **)(void *))(*(_DWORD *)v2 + 28))(v2);
  sub_443240(v19, v14, dword_565D6C);
  return v6;
}

// ===== sub_4484B0 @ 0x004484B0..0x004484B4 =====
int __usercall sub_4484B0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 36);
}

// ===== sub_4484C0 @ 0x004484C0..0x00448516 =====
int __usercall sub_4484C0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<esi>)
{
  int result; // eax
  int v4; // ecx
  int v5; // [esp+8h] [ebp-8h] BYREF
  int v6; // [esp+Ch] [ebp-4h]

  v5 = 0;
  v6 = 0;
  if ( a1[28] )
    sub_44A6F0(&v5, a1[27]);
  *a2 = a1[12];
  a2[1] = a1[26];
  result = v5;
  a2[2] = a1[27];
  v4 = v6;
  a2[3] = a1[28];
  a2[4] = result;
  a2[5] = v4;
  return result;
}

// ===== sub_448520 @ 0x00448520..0x00448524 =====
int __usercall sub_448520@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 60);
}

// ===== sub_448530 @ 0x00448530..0x00448558 =====
int __usercall sub_448530@<eax>(int a1@<ecx>, int a2@<esi>)
{
  int v2; // eax
  int v3; // edx

  v2 = 0;
  if ( *(int *)(a1 + 52) > 0 )
  {
    v3 = 0;
    do
    {
      *(_DWORD *)(a2 + 4 * v2++) = *(_DWORD *)(*(_DWORD *)(a1 + 56) + v3 + 8);
      v3 += 52;
    }
    while ( v2 < *(_DWORD *)(a1 + 52) );
  }
  return *(_DWORD *)(a1 + 52);
}

// ===== sub_448560 @ 0x00448560..0x004485A0 =====
BOOL __usercall sub_448560@<eax>(_DWORD *a1@<edx>, int a2@<edi>)
{
  _DWORD *v2; // eax
  BOOL v3; // esi
  BOOL result; // eax

  v2 = *(_DWORD **)(a2 + 160);
  v3 = v2 != 0;
  if ( v2 )
  {
    *a1 = *v2;
    a1[1] = v2[1];
    a1[2] = v2[2];
    sub_44A220(a2);
    return v3;
  }
  else
  {
    result = 0;
    *a1 = 0;
    a1[1] = 0;
    a1[2] = 0;
  }
  return result;
}

// ===== sub_4485A0 @ 0x004485A0..0x00448651 =====
int __thiscall sub_4485A0(int this)
{
  int v2; // edi
  int v3; // edi
  int v4; // eax
  int v5; // eax
  bool v6; // zf
  int v8; // [esp-8h] [ebp-10h]

  sub_447860(this);
  if ( *(_DWORD *)(this + 48) )
  {
    if ( !*(_DWORD *)(this + 136) )
    {
      *(_DWORD *)(this + 140) = 0;
LABEL_11:
      *(_DWORD *)(this + 48) = sub_448690(this) == 0;
      sub_4478E0(this);
      return 0;
    }
    v2 = 0;
    if ( *(_DWORD *)(this + 64) )
    {
      v3 = *(_DWORD *)(this + 40);
      v8 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3);
      (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3);
    }
    else
    {
      if ( !*(_DWORD *)(this + 68) )
        goto LABEL_8;
      v8 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
    }
    v2 = sub_46DF00(v8);
LABEL_8:
    v4 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 44) + 28))(*(_DWORD *)(this + 44));
    v5 = v2 | sub_46DF00(v4);
    v6 = *(_DWORD *)(this + 64) == 0;
    *(_DWORD *)(this + 140) = v5;
    if ( v6 )
      *(_DWORD *)(this + 140) = v5 & 0x2C3;
    goto LABEL_11;
  }
  return 0;
}

// ===== nullsub_1 @ 0x00448660..0x00448661 =====
void nullsub_1()
{
  ;
}

// ===== sub_448670 @ 0x00448670..0x00448690 =====
int __stdcall sub_448670(int a1, int a2)
{
  sub_44A1D0(268435458, a1, a2);
  return 1;
}

// ===== sub_448690 @ 0x00448690..0x0044956D =====
int __thiscall sub_448690(int *this)
{
  void (__thiscall *v2)(int *); // edx
  int v3; // ebx
  int v4; // eax
  int v5; // edi
  int v6; // eax
  int v7; // ecx
  int v8; // eax
  int v9; // ecx
  int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  int v15; // edx
  int v16; // eax
  BOOL v17; // ecx
  int v18; // eax
  int v19; // edx
  int v20; // eax
  int v21; // eax
  int v22; // eax
  int v23; // edx
  int v24; // eax
  int v25; // eax
  int v26; // eax
  int v27; // ecx
  int v28; // eax
  int result; // eax
  int v30; // eax
  int v31; // eax
  int v32; // edi
  int v33; // eax
  int v34; // edx
  int v35; // ecx
  int v36; // eax
  int v37; // ecx
  int v38; // edx
  int v39; // ebx
  int v40; // ecx
  int v41; // eax
  int v42; // ecx
  int v43; // edx
  int v44; // ecx
  int v45; // ebx
  _DWORD *v46; // ecx
  int v47; // edi
  int v48; // eax
  int v49; // ecx
  int v50; // ebx
  int v51; // ecx
  int v52; // eax
  int v53; // ecx
  int v54; // eax
  int v55; // eax
  int v56; // [esp+Ch] [ebp-234h]
  int v57; // [esp+Ch] [ebp-234h]
  int (__thiscall **v58)(int *, int, _DWORD); // [esp+20h] [ebp-220h]
  int v59; // [esp+20h] [ebp-220h]
  int v60; // [esp+20h] [ebp-220h]
  int v61; // [esp+20h] [ebp-220h]
  int v62; // [esp+20h] [ebp-220h]
  int v63; // [esp+20h] [ebp-220h]
  int v64; // [esp+20h] [ebp-220h]
  int v65; // [esp+20h] [ebp-220h]
  int v66; // [esp+20h] [ebp-220h]
  int v67; // [esp+20h] [ebp-220h]
  int v68; // [esp+20h] [ebp-220h]
  int v69; // [esp+20h] [ebp-220h]
  int v70; // [esp+24h] [ebp-21Ch]
  int *v71; // [esp+28h] [ebp-218h]
  _DWORD *v72; // [esp+28h] [ebp-218h]
  _DWORD *v73; // [esp+28h] [ebp-218h]
  int v74; // [esp+28h] [ebp-218h]
  int v75; // [esp+28h] [ebp-218h]
  int v76; // [esp+2Ch] [ebp-214h] BYREF
  int v77; // [esp+30h] [ebp-210h]
  int v78; // [esp+34h] [ebp-20Ch]
  _DWORD v79[8]; // [esp+38h] [ebp-208h]
  _DWORD v80[24]; // [esp+58h] [ebp-1E8h] BYREF
  _DWORD v81[24]; // [esp+B8h] [ebp-188h] BYREF
  _DWORD v82[24]; // [esp+118h] [ebp-128h] BYREF
  _DWORD v83[24]; // [esp+178h] [ebp-C8h] BYREF
  _DWORD v84[25]; // [esp+1D8h] [ebp-68h]

  v2 = *(void (__thiscall **)(int *))(*this + 12);
  v3 = 0;
  v78 = 0;
  v2(this);
  v4 = this[15];
  v5 = -1;
  v71 = 0;
  v70 = -1;
  if ( v4 >= 0 && v4 < this[13] )
  {
    v5 = this[15];
    v71 = (int *)(this[14] + 52 * v4);
    v70 = v71[2];
  }
  v6 = sub_4495C0(0, 0, 0);
  if ( v6 != this[32] )
  {
    this[32] = v6;
    if ( v6 == -1 )
    {
      v9 = -1;
      v10 = -1;
    }
    else
    {
      v8 = this[23] + 20 * v6;
      v9 = *(_DWORD *)(v8 + 4);
      v10 = *(_DWORD *)(v8 + 8);
    }
    sub_44A1D0(268435457, v9, v10);
  }
  v58 = (int (__thiscall **)(int *, int, _DWORD))(*this + 28);
  v11 = sub_449760(v7);
  if ( (*v58)(this, v11, 0) )
  {
    v12 = this[29];
    if ( v12 == -1 )
    {
      v16 = -1;
      v17 = 0;
    }
    else
    {
      v13 = this[23];
      v14 = 5 * v12;
      v15 = *(_DWORD *)(v13 + 4 * v14 + 12);
      v16 = *(_DWORD *)(v13 + 4 * v14 + 8) | (*(_DWORD *)(v13 + 4 * v14 + 4) << 16);
      v17 = *(_DWORD *)(v15 + 20) != -1;
    }
    (*(void (__thiscall **)(int *, int, BOOL))(*this + 16))(this, v16, v17);
  }
  v18 = sub_4495C0(&v76, 1, 1);
  if ( v18 != this[33] )
  {
    v19 = v76;
    this[33] = v18;
    v20 = v77;
    this[24] = v19;
    this[25] = v20;
  }
  if ( this[34] )
  {
    v21 = this[29];
    if ( v21 != -1 )
    {
      if ( *(_DWORD *)(52 * *(_DWORD *)(this[23] + 20 * v21 + 4) + this[14] + 16) )
      {
        sub_48E680(&v76);
        if ( this[30] != v76 || this[31] != v77 )
        {
          sub_449A60(this, *(_DWORD *)(this[23] + 20 * this[29] + 4));
          v22 = (*(int (__thiscall **)(int *, _DWORD, _DWORD))(*this + 36))(
                  this,
                  *(_DWORD *)(this[23] + 20 * this[29] + 4),
                  *(_DWORD *)(this[23] + 20 * this[29] + 8));
          if ( v71 )
            v70 = v71[2];
          v23 = v77;
          this[30] = v76;
          this[31] = v23;
          if ( v22 )
            sub_44A1D0(
              268435460,
              *(_DWORD *)(this[23] + 20 * this[29] + 8) | (*(_DWORD *)(this[23] + 20 * this[29] + 4) << 16),
              0);
        }
      }
    }
    if ( this[34] )
    {
      v24 = this[29];
      if ( v24 != -1 )
      {
        if ( *(_DWORD *)(52 * *(_DWORD *)(this[23] + 20 * v24 + 4) + this[14] + 20) )
        {
          (*(void (__thiscall **)(int))(*(_DWORD *)this[11] + 28))(this[11]);
          if ( (sub_46E490() & 1) != 0 )
            this[35] |= 1u;
        }
      }
    }
  }
  v84[0] = 1;
  v84[1] = 2;
  v84[2] = 4;
  v84[3] = 16;
  v84[4] = 32;
  v84[5] = 64;
  v84[6] = 128;
  v84[7] = 256;
  v84[8] = 512;
  v84[9] = 4096;
  v84[10] = 0x2000;
  v84[11] = 0x4000;
  v84[12] = 0x8000;
  v84[13] = 0x10000;
  v84[14] = 0x20000;
  v84[15] = 0x40000;
  v84[16] = 0x80000;
  v84[17] = 0x100000;
  v84[18] = 0x200000;
  v84[19] = 0x400000;
  v84[20] = 0x800000;
  v84[21] = 0x1000000;
  v84[22] = 0x2000000;
  v84[23] = 0x40000000;
  v81[0] = 1;
  v81[1] = 2;
  memset(&v81[2], 0, 20);
  v81[7] = 3;
  v81[8] = 4;
  v81[9] = 7;
  v81[10] = 8;
  v81[11] = 5;
  v81[12] = 6;
  memset(&v81[13], 0, 40);
  v81[23] = 19;
  v83[0] = 1;
  v83[1] = 2;
  memset(&v83[2], 0, 20);
  v83[7] = 3;
  v83[8] = 4;
  v83[9] = 5;
  v83[10] = 6;
  v83[11] = 7;
  v83[12] = 8;
  memset(&v83[13], 0, 40);
  v83[23] = 19;
  v80[0] = 1;
  v80[1] = 2;
  memset(&v80[2], 0, 20);
  v80[8] = 4;
  v82[8] = 4;
  v79[1] = v83;
  v82[1] = 2;
  v79[3] = v82;
  v25 = this[18];
  v79[0] = v81;
  v26 = this[35] & ~v25;
  v79[2] = v80;
  v80[7] = 3;
  v80[9] = 11;
  v80[10] = 12;
  v80[11] = 9;
  v80[12] = 10;
  memset(&v80[13], 0, 40);
  v80[23] = 19;
  v82[0] = 1;
  memset(&v82[2], 0, 20);
  v82[7] = 3;
  v82[9] = 9;
  v82[10] = 10;
  v82[11] = 11;
  v82[12] = 12;
  memset(&v82[13], 0, 40);
  v82[23] = 19;
  v79[4] = &unk_50EA58;
  v79[5] = &unk_50EAB8;
  v79[6] = &unk_50EB18;
  v79[7] = &unk_50EB78;
  v59 = 0;
  v27 = 0;
  while ( (v26 & v84[v27]) == 0 )
  {
    if ( ++v27 >= 24 )
      goto LABEL_36;
  }
  v59 = *(_DWORD *)(v79[this[19]] + 4 * v27);
  if ( v59 == 19 )
    v59 = 8 - ((sub_46D560(16) & 0x8000) != 0);
LABEL_36:
  if ( this[36] != -1 )
  {
    if ( v59 )
      goto LABEL_41;
    if ( (sub_46D560(1) & 0x8000u) != 0 )
      goto LABEL_42;
    if ( this[33] != this[36] )
LABEL_41:
      this[36] = -1;
    else
      v59 = 1;
  }
LABEL_42:
  switch ( v59 )
  {
    case 1:
      v28 = this[33];
      if ( v28 == -1 )
      {
        (*(void (__thiscall **)(int *))(*this + 24))(this);
        return v78;
      }
      else
      {
        if ( !(*(int (__thiscall **)(int *, _DWORD, _DWORD))(*this + 40))(
                this,
                *(_DWORD *)(this[23] + 20 * v28 + 4),
                *(_DWORD *)(this[23] + 20 * v28 + 8)) )
          return v78;
        if ( (*(int (__thiscall **)(int *, int))(*this + 72))(this, this[33]) || this[36] != -1 )
        {
          (*(void (__thiscall **)(int *, int, int))(*this + 48))(this, this[33], 1);
          if ( this[20] )
          {
            v30 = this[23] + 20 * this[33];
            (*(void (__thiscall **)(int *, _DWORD, _DWORD))(*this + 36))(
              this,
              *(_DWORD *)(v30 + 4),
              *(_DWORD *)(v30 + 8));
          }
          v78 = (*(int (__thiscall **)(int *))(*this + 20))(this);
          if ( !v78 )
            sub_449A60(this, *(_DWORD *)(this[23] + 20 * this[33] + 4));
          this[36] = -1;
          return v78;
        }
        else
        {
          this[36] = this[33];
          return v78;
        }
      }
    case 2:
      (*(void (__thiscall **)(int *, int, int))(*this + 48))(this, -1, 1);
      goto LABEL_55;
    case 3:
      if ( !(*(int (__thiscall **)(int *, int, int))(*this + 40))(this, v5, v70) || v5 == -1 || v70 == -1 )
        return v78;
      (*(void (__thiscall **)(int *, int, int, _DWORD))(*this + 44))(this, v5, v70, 0);
      result = (*(int (__thiscall **)(int *))(*this + 20))(this);
      v78 = result;
      return result;
    case 4:
      (*(void (__thiscall **)(int *, int, _DWORD))(*this + 48))(this, -1, 0);
LABEL_55:
      result = (*(int (__thiscall **)(int *))(*this + 20))(this);
      v78 = result;
      return result;
    case 5:
      if ( !v71 )
        return v78;
      v31 = *v71;
      if ( *v71 <= 0 )
        return v78;
      while ( 1 )
      {
        if ( --v70 < 0 || v70 >= v31 )
          v70 = v31 - 1;
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70) )
          break;
        v31 = *v71;
        if ( ++v3 >= *v71 )
          return v78;
      }
      v56 = -1;
      goto LABEL_70;
    case 6:
      if ( !v71 )
        return v78;
      v33 = *v71;
      v60 = 0;
      if ( *v71 <= 0 )
        return v78;
      while ( 1 )
      {
        if ( ++v70 < 0 || v70 >= v33 )
          v70 = 0;
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70) )
          break;
        v33 = *v71;
        if ( ++v60 >= *v71 )
          return v78;
      }
      v56 = 1;
      goto LABEL_70;
    case 7:
      if ( this[13] <= 0 )
        return v78;
      while ( 1 )
      {
        if ( --v5 < 0 || v5 >= this[13] )
          v5 = this[13] - 1;
        if ( sub_449A60(this, v5) )
          break;
        if ( ++v3 >= this[13] )
          return v78;
      }
      sub_44A1D0(268435459, v5, -1);
      return v78;
    case 8:
      v61 = 0;
      if ( this[13] <= 0 )
        return v78;
      while ( 1 )
      {
        if ( ++v5 < 0 || v5 >= this[13] )
          v5 = 0;
        if ( sub_449A60(this, v5) )
          break;
        if ( ++v61 >= this[13] )
          return v78;
      }
      sub_44A1D0(268435459, v5, 1);
      return v78;
    case 9:
      if ( !v71 )
        return v78;
      v62 = 0;
      if ( *(int *)(this[21] + 8 * v5) <= 0 )
        return v78;
      while ( 1 )
      {
        v34 = this[21];
        v35 = *(_DWORD *)(v34 + 8 * v5);
        v72 = (_DWORD *)(v34 + 8 * v5);
        v36 = v70 / v35;
        v37 = v70 % v35 - 1;
        v38 = v37 < 0 ? *v72 - 1 : v37;
        v70 = v38 + v36 * *v72;
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70) )
          break;
        if ( ++v62 >= *(_DWORD *)(this[21] + 8 * v5) )
          return v78;
      }
      v56 = 0xFFFF;
      goto LABEL_70;
    case 10:
      if ( !v71 )
        return v78;
      v63 = 0;
      if ( *(int *)(this[21] + 8 * v5) <= 0 )
        return v78;
      v39 = v70;
      while ( 1 )
      {
        v40 = *(_DWORD *)(this[21] + 8 * v5);
        v39 = v39 / v40 * v40 + (v39 % v40 + 1 >= v40 ? 0 : v39 % v40 + 1);
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v39) )
          break;
        if ( ++v63 >= *(_DWORD *)(this[21] + 8 * v5) )
          return v78;
      }
      goto LABEL_113;
    case 11:
      if ( !v71 )
        return v78;
      v64 = 0;
      if ( *(int *)(this[21] + 8 * v5 + 4) <= 0 )
        return v78;
      while ( 1 )
      {
        v41 = this[21];
        v42 = *(_DWORD *)(v41 + 8 * v5);
        v73 = (_DWORD *)(v41 + 8 * v5);
        v43 = v70 % v42;
        v44 = v70 / v42 - 1 < 0 ? v73[1] - 1 : v70 / v42 - 1;
        v70 = v43 + v44 * *v73;
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70) )
          break;
        if ( ++v64 >= *(_DWORD *)(this[21] + 8 * v5 + 4) )
          return v78;
      }
      v56 = -65536;
LABEL_70:
      v32 = v70 | (v5 << 16);
      goto LABEL_71;
    case 12:
      if ( !v71 )
        return v78;
      v65 = 0;
      if ( *(int *)(this[21] + 8 * v5 + 4) <= 0 )
        return v78;
      v45 = v70;
      while ( 1 )
      {
        v46 = (_DWORD *)(this[21] + 8 * v5);
        v45 = v45 % *v46 + *v46 * (v45 / *v46 + 1 >= v46[1] ? 0 : v45 / *v46 + 1);
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v45) )
          break;
        if ( ++v65 >= *(_DWORD *)(this[21] + 8 * v5 + 4) )
          return v78;
      }
LABEL_129:
      v56 = 0x10000;
      v32 = v45 | (v5 << 16);
      goto LABEL_71;
    case 13:
      if ( !v71 )
        return v78;
      if ( v70 > 0 && (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70 - 1) )
      {
        v56 = -1;
        v32 = (v70 - 1) | (v5 << 16);
        goto LABEL_71;
      }
      v56 = -1;
      goto LABEL_135;
    case 14:
      if ( !v71 )
        return v78;
      v48 = (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v70 + 1);
      v47 = v5 << 16;
      v56 = 1;
      if ( !v48 )
        goto LABEL_136;
      v32 = (v70 + 1) | v47;
      goto LABEL_71;
    case 15:
      if ( !v71 )
        return v78;
      v49 = *(_DWORD *)(this[21] + 8 * v5);
      v76 = v70 / v49;
      v66 = v70 % v49;
      if ( v70 % v49 <= 0 )
        goto LABEL_144;
      while ( 1 )
      {
        v57 = --v66 + v76 * *(_DWORD *)(this[21] + 8 * v5);
        v74 = v57;
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v57) )
          break;
        if ( v66 <= 0 )
        {
LABEL_144:
          v56 = 0xFFFF;
          goto LABEL_135;
        }
      }
      v32 = v74 | (v5 << 16);
      v56 = 0xFFFF;
      goto LABEL_71;
    case 16:
      if ( !v71 )
        return v78;
      v50 = *(_DWORD *)(this[21] + 8 * v5);
      v76 = v70 / v50;
      v67 = v70 % v50 + 1;
      if ( v67 >= v50 )
        goto LABEL_150;
      do
      {
        v39 = v67 + v76 * *(_DWORD *)(this[21] + 8 * v5);
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v39) )
        {
LABEL_113:
          v56 = 1;
          v32 = v39 | (v5 << 16);
          goto LABEL_71;
        }
        ++v67;
      }
      while ( v67 < *(_DWORD *)(this[21] + 8 * v5) );
LABEL_150:
      v56 = 1;
      goto LABEL_135;
    case 17:
      if ( !v71 )
        return v78;
      v51 = *(_DWORD *)(this[21] + 8 * v5);
      v76 = v70 % v51;
      v68 = v70 / v51;
      if ( v70 / v51 <= 0 )
        goto LABEL_155;
      break;
    case 18:
      if ( !v71 )
        return v78;
      v53 = this[21];
      v54 = v70 / *(_DWORD *)(v53 + 8 * v5) + 1;
      v76 = v70 % *(_DWORD *)(v53 + 8 * v5);
      v69 = v54;
      if ( v54 >= *(_DWORD *)(v53 + 8 * v5 + 4) )
        goto LABEL_163;
      while ( 2 )
      {
        v45 = v76 + v54 * *(_DWORD *)(this[21] + 8 * v5);
        if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v45) )
          goto LABEL_129;
        if ( ++v69 < *(_DWORD *)(this[21] + 8 * v5 + 4) )
        {
          v54 = v69;
          continue;
        }
        break;
      }
LABEL_163:
      v56 = 0x10000;
      goto LABEL_135;
    default:
      v55 = sub_449ED0();
      v78 = v55 != -1;
      if ( v55 != -1 )
        (*(void (__thiscall **)(int *, int, _DWORD))(*this + 48))(this, v55, 0);
      return v78;
  }
  do
  {
    v52 = --v68 * *(_DWORD *)(this[21] + 8 * v5);
    v75 = v76 + v52;
    if ( (*(int (__thiscall **)(int *, int, int))(*this + 36))(this, v5, v76 + v52) )
    {
      v32 = v75 | (v5 << 16);
      v56 = -65536;
LABEL_71:
      sub_44A1D0(268435460, v32, v56);
      return v78;
    }
  }
  while ( v68 > 0 );
LABEL_155:
  v56 = -65536;
LABEL_135:
  v47 = v5 << 16;
LABEL_136:
  sub_44A1D0(268435461, (unsigned __int16)v70 | v47, v56);
  return v78;
}

// ===== sub_4495C0 @ 0x004495C0..0x0044975E =====
int __userpurge sub_4495C0@<eax>(_DWORD *a1@<esi>, _DWORD *a2, int a3, int a4)
{
  int v4; // eax
  int v5; // ebx
  int v6; // ecx
  int v7; // edi
  int v8; // eax
  int v9; // ecx
  int v10; // ecx
  int v11; // eax
  int v12; // ecx
  int result; // eax
  int v14; // ecx
  int v15; // [esp+10h] [ebp-24h]
  int v16; // [esp+14h] [ebp-20h] BYREF
  int v17; // [esp+18h] [ebp-1Ch]
  int v18; // [esp+1Ch] [ebp-18h] BYREF
  int v19; // [esp+20h] [ebp-14h]
  int v20; // [esp+24h] [ebp-10h]
  int v21; // [esp+28h] [ebp-Ch]

  if ( !a1[34] )
    return -1;
  v4 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)a1[10] + 28))(a1[10]);
  if ( !sub_46E580(v4) || !sub_447BF0() || !sub_48EE20() && !a4 )
    return -1;
  sub_48E680(&v16);
  v15 = 0;
  v5 = (*(int (__thiscall **)(_DWORD *, _DWORD))(*a1 + 68))(a1, 0);
  if ( v5 == -1 )
    return -1;
  while ( 1 )
  {
    v6 = a1[23];
    v7 = 20 * v5;
    if ( *(_DWORD *)(20 * v5 + v6) )
    {
      (*(void (__thiscall **)(_DWORD, int *))(**(_DWORD **)(v6 + v7 + 16) + 36))(*(_DWORD *)(v6 + v7 + 16), &v18);
      v8 = v16;
      if ( v18 <= v16 && v16 <= v20 )
      {
        v9 = v17;
        if ( v19 <= v17 && v17 <= v21 )
        {
          if ( !a3 )
            goto LABEL_19;
          if ( (*(int (__thiscall **)(_DWORD *, _DWORD, _DWORD))(*a1 + 40))(
                 a1,
                 *(_DWORD *)(a1[23] + v7 + 4),
                 *(_DWORD *)(a1[23] + v7 + 8)) )
          {
            v10 = *(_DWORD *)(a1[23] + v7 + 16);
            v11 = (*(int (__thiscall **)(int))(*(_DWORD *)v10 + 28))(v10);
            if ( sub_46D830(v11) )
            {
              v12 = *(_DWORD *)(a1[23] + v7 + 16);
              if ( (*(int (__thiscall **)(int, int, int, int))(*(_DWORD *)v12 + 100))(v12, v16 - v18, v17 - v19, 1) )
                break;
            }
          }
        }
      }
    }
    result = (*(int (__thiscall **)(_DWORD *, int))(*a1 + 68))(a1, ++v15);
    v5 = result;
    if ( result == -1 )
      return result;
  }
  v8 = v16;
  v9 = v17;
LABEL_19:
  if ( a2 )
  {
    v14 = v9 - v19;
    *a2 = v8 - v18;
    a2[1] = v14;
  }
  return v5;
}

// ===== sub_449760 @ 0x00449760..0x0044979E =====
int __usercall sub_449760@<eax>(_DWORD *a1@<eax>)
{
  int result; // eax
  int v3; // edx
  _DWORD v4[2]; // [esp+8h] [ebp-8h] BYREF

  a1[24] = -1;
  a1[25] = -1;
  result = sub_4495C0(a1, v4, 1, 0);
  if ( result == -1 )
    return -1;
  v3 = v4[1];
  a1[24] = v4[0];
  a1[25] = v3;
  return result;
}

// ===== sub_4497A0 @ 0x004497A0..0x004499E5 =====
int __thiscall sub_4497A0(int *this, int a2, int a3)
{
  int v3; // edi
  int i; // esi
  unsigned int v7; // eax
  int v8; // eax
  int v9; // ecx
  int v10; // eax
  int v11; // edi
  int v12; // esi
  int v13; // edx
  int v14; // eax
  int v15; // edx
  int v16; // ecx
  int v17; // edi
  int v18; // esi
  int v19; // eax
  void (__thiscall *v20)(int *, char *, int, int, int, int); // edx
  int v21; // edi
  unsigned int v22; // eax
  int v23; // [esp+50h] [ebp-4Ch]
  _DWORD *v24; // [esp+54h] [ebp-48h]
  int j; // [esp+54h] [ebp-48h]
  int v26; // [esp+58h] [ebp-44h] BYREF
  char v27[4]; // [esp+5Ch] [ebp-40h] BYREF
  int v28; // [esp+60h] [ebp-3Ch]
  int v29[4]; // [esp+64h] [ebp-38h] BYREF
  _DWORD v30[4]; // [esp+74h] [ebp-28h] BYREF
  _DWORD v31[6]; // [esp+84h] [ebp-18h] BYREF

  v3 = a2;
  if ( this[29] == a2 && !a3 )
    return 0;
  sub_42C2A0(v30, (_DWORD *)this[10]);
  for ( i = 0; i < 2; ++i )
  {
    if ( sub_42C330(i + 2, this[10], v29) )
    {
      v7 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
      sub_443240(v29, v7, dword_565D6C);
      sub_4478D0((int)this);
    }
  }
  v8 = this[29];
  if ( v8 != -1 )
  {
    v9 = this[23];
    v10 = 5 * v8;
    v11 = *(_DWORD *)(v9 + 4 * v10 + 8);
    v12 = *(_DWORD *)(v9 + 4 * v10 + 4);
    v13 = *(_DWORD *)(v9 + 4 * v10 + 12);
    v14 = *this;
    v28 = v13;
    (*(void (__thiscall **)(int *, char *, int, int, _DWORD, int))(v14 + 32))(this, v27, v12, v11, 0, 1);
    (*(void (__thiscall **)(int *, int *, int, int, int, int))(*this + 32))(this, &v26, v12, v11, 1, 1);
    sub_449DF0(this, v26);
    v3 = a2;
  }
  if ( v3 == -1 )
  {
    for ( j = 0; j < 2; ++j )
      sub_42BAB0(this[10], j + 2, 0);
    sub_4478D0((int)this);
    this[29] = a2;
    return 1;
  }
  else
  {
    v15 = this[23];
    v16 = 5 * v3;
    v17 = *(_DWORD *)(v15 + 20 * v3 + 4);
    v18 = *(_DWORD *)(v15 + 4 * v16 + 12);
    v19 = *(_DWORD *)(v15 + 4 * v16 + 8);
    v20 = *(void (__thiscall **)(int *, char *, int, int, int, int))(*this + 32);
    v28 = v19;
    v20(this, v27, v17, v19, 1, 1);
    (*(void (__thiscall **)(int *, int *, int, int, _DWORD, int))(*this + 32))(this, &v26, v17, v28, 0, 1);
    sub_449DF0(this, v26);
    v21 = 2;
    v23 = 2;
    v24 = (_DWORD *)(v18 + 32);
    v28 = 2;
    while ( 1 )
    {
      if ( sub_407F20(dword_565D70, v24[1], v31) )
      {
        sub_42BB30(v21, (int)v31, this[10]);
        sub_42BAE0(this[10], v23, v30[0] + *(v24 - 1), v30[1] + *v24, 0);
        sub_42BAB0(this[10], v23, 1);
        sub_42C330(v23, this[10], v29);
        v22 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
        sub_443240(v29, v22, dword_565D6C);
      }
      else
      {
        sub_42BAB0(this[10], v21, 0);
      }
      sub_4478D0((int)this);
      v24 += 3;
      ++v23;
      if ( !--v28 )
        break;
      v21 = v23;
    }
    this[29] = a2;
    return 1;
  }
}

// ===== sub_4499F0 @ 0x004499F0..0x00449A51 =====
int __thiscall sub_4499F0(_DWORD *this, _DWORD *a2, int a3, int a4, int a5, int a6)
{
  int result; // eax
  _DWORD *v7; // edx
  _DWORD *v8; // eax
  int v9; // esi
  int v10; // eax

  result = 0;
  if ( a3 < this[13] )
  {
    v7 = (_DWORD *)(this[14] + 52 * a3);
    if ( a4 < *v7 )
    {
      v8 = (_DWORD *)(v7[1] + 60 * a4);
      *a2 = v8[3];
      if ( a5 )
      {
        v9 = v8[5];
        if ( v9 != -1 )
          *a2 = v9;
      }
      if ( a6 && a4 == v7[2] )
      {
        v10 = v8[4];
        if ( v10 != -1 )
          *a2 = v10;
      }
      return 1;
    }
  }
  return result;
}

// ===== sub_449A60 @ 0x00449A60..0x00449BB5 =====
int __stdcall sub_449A60(int *a1, int a2)
{
  int result; // eax
  int v3; // esi
  int i; // edi
  unsigned int v5; // eax
  int v6; // edi
  unsigned int v7; // eax
  int v8; // [esp+10h] [ebp-40h]
  _DWORD *v9; // [esp+14h] [ebp-3Ch]
  int v10[4]; // [esp+18h] [ebp-38h] BYREF
  _DWORD v11[4]; // [esp+28h] [ebp-28h] BYREF
  _DWORD v12[6]; // [esp+38h] [ebp-18h] BYREF

  result = 0;
  if ( a2 >= 0 && a2 < a1[13] )
  {
    v3 = a1[14] + 52 * a2;
    if ( *(_DWORD *)(v3 + 12) )
    {
      sub_42C2A0(v11, (_DWORD *)a1[10]);
      for ( i = 0; i < 2; ++i )
      {
        if ( sub_42C330(i, a1[10], v10) )
        {
          v5 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[10] + 28))(a1[10]);
          sub_443240(v10, v5, dword_565D6C);
          sub_4478D0((int)a1);
        }
      }
      v6 = 0;
      v8 = 0;
      v9 = (_DWORD *)(v3 + 32);
      while ( 1 )
      {
        if ( sub_407F20(dword_565D70, v9[1], v12) )
        {
          sub_42BB30(v6, (int)v12, a1[10]);
          sub_42BAE0(a1[10], v8, v11[0] + *(v9 - 1), v11[1] + *v9, 0);
          sub_42BAB0(a1[10], v8, 1);
          sub_42C330(v8, a1[10], v10);
          v7 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[10] + 28))(a1[10]);
          sub_443240(v10, v7, dword_565D6C);
        }
        else
        {
          sub_42BAB0(a1[10], v6, 0);
        }
        sub_4478D0((int)a1);
        v9 += 3;
        if ( ++v8 >= 2 )
          break;
        v6 = v8;
      }
      a1[15] = a2;
      sub_449D60(a2);
      return 1;
    }
  }
  return result;
}

// ===== sub_449BC0 @ 0x00449BC0..0x00449D60 =====
int __thiscall sub_449BC0(_DWORD *this, int a2, int a3)
{
  int result; // eax
  int v5; // ecx
  int *v6; // esi
  int v7; // edx
  int v8; // eax
  int v9; // [esp+4Ch] [ebp-18h]
  int v10; // [esp+4Ch] [ebp-18h]
  int v11; // [esp+50h] [ebp-14h]
  int v12; // [esp+54h] [ebp-10h]
  int v13; // [esp+58h] [ebp-Ch]
  int v14; // [esp+5Ch] [ebp-8h] BYREF
  char v15[4]; // [esp+60h] [ebp-4h] BYREF

  result = 0;
  if ( a2 >= 0 && a2 < this[13] )
  {
    if ( (v5 = a3, v6 = (int *)(this[14] + 52 * a2), v7 = v6[2], v11 = v7, a3 >= 0) && a3 < *v6 || a3 == -1 )
    {
      if ( a3 != v7 )
      {
        v12 = -1;
        v13 = -1;
        if ( this[29] != -1 )
        {
          v8 = this[23] + 20 * this[29];
          v12 = *(_DWORD *)(v8 + 4);
          v7 = v6[2];
          v13 = *(_DWORD *)(v8 + 8);
        }
        if ( v7 != -1 )
        {
          if ( a2 != v12 || (v9 = 1, v7 != v13) )
            v9 = 0;
          (*(void (__thiscall **)(_DWORD *, char *, int, int, int, _DWORD))(*this + 32))(this, v15, a2, v7, v9, 0);
          (*(void (__thiscall **)(_DWORD *, int *, int, int, int, int))(*this + 32))(this, &v14, a2, v11, v9, 1);
          sub_449DF0(this, v14);
          v5 = a3;
        }
        v6[2] = v5;
        if ( v5 == -1 )
          return 1;
        if ( *(_DWORD *)(60 * v5 + v6[1] + 12) != -1 )
        {
          if ( a2 != v12 || (v10 = 1, v5 != v13) )
            v10 = 0;
          (*(void (__thiscall **)(_DWORD *, char *, int, int, int, int))(*this + 32))(this, v15, a2, v5, v10, 1);
          (*(void (__thiscall **)(_DWORD *, int *, int, int, int, _DWORD))(*this + 32))(this, &v14, a2, a3, v10, 0);
          sub_449DF0(this, v14);
          sub_449D60(a2);
          return 1;
        }
        return 0;
      }
    }
  }
  return result;
}

// ===== sub_449D60 @ 0x00449D60..0x00449DE7 =====
int __userpurge sub_449D60@<eax>(_DWORD *a1@<edi>, int a2)
{
  int v2; // eax
  int v3; // edx
  int v4; // esi
  int v5; // ecx
  int v6; // esi
  int v7; // ebx
  int v9; // [esp+0h] [ebp-4h]

  v2 = a2;
  if ( a2 < 0 )
    return 0;
  v3 = a1[13];
  if ( a2 >= v3 )
    return 0;
  v4 = a1[14];
  if ( *(_DWORD *)(52 * a2 + v4 + 8) == -1 )
    return 1;
  v5 = *(_DWORD *)(52 * a2 + v4 + 24);
  v9 = v5;
  if ( v5 == -1 )
    return 1;
  v6 = 0;
  if ( v3 <= 0 )
    return 1;
  v7 = 0;
  do
  {
    if ( v6 != v2 && v5 == *(_DWORD *)(a1[14] + v7 + 24) )
    {
      (*(void (__thiscall **)(_DWORD *, int, int))(*a1 + 36))(a1, v6, -1);
      v2 = a2;
      v5 = v9;
    }
    ++v6;
    v7 += 52;
  }
  while ( v6 < a1[13] );
  return 1;
}

// ===== sub_449DF0 @ 0x00449DF0..0x00449EC7 =====
int __userpurge sub_449DF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int v5; // esi
  int v6; // edi
  int v7; // esi
  unsigned int v8; // eax
  unsigned int v9; // eax
  int v11[6]; // [esp+10h] [ebp-40h] BYREF
  int v12[6]; // [esp+28h] [ebp-28h] BYREF
  int v13[4]; // [esp+40h] [ebp-10h] BYREF

  if ( !sub_407F20(dword_565D70, a2, v11) || !sub_407F20(dword_565D70, a4, v12) )
    return 0;
  sub_42C2A0(v13, *(_DWORD **)(a3 + 40));
  v5 = *(_DWORD *)(a1 + 4);
  v6 = v13[1] + *(_DWORD *)(a1 + 8);
  v7 = v13[0] + v5;
  sub_42B5B0(*(_DWORD **)(a3 + 40), v13, v7, v6, v12, 64, 1);
  v8 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a3 + 40) + 28))(*(_DWORD *)(a3 + 40));
  sub_443240(v13, v8, dword_565D6C);
  sub_42B5B0(*(_DWORD **)(a3 + 40), v13, v7, v6, v11, 0, 0);
  v9 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a3 + 40) + 28))(*(_DWORD *)(a3 + 40));
  sub_443240(v13, v9, dword_565D6C);
  sub_4478D0(a3);
  return 1;
}

// ===== sub_449ED0 @ 0x00449ED0..0x00449F9F =====
int __usercall sub_449ED0@<eax>(int a1@<edi>)
{
  int v1; // eax
  int v2; // eax
  int v3; // eax
  int v4; // edx
  int v5; // esi
  int v6; // ebx
  int v8; // [esp+8h] [ebp-8h]
  int i; // [esp+Ch] [ebp-4h]

  if ( !*(_DWORD *)(a1 + 64) )
    return -1;
  v1 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
  if ( !sub_46E580(v1) )
    return -1;
  v8 = 0;
  if ( *(int *)(a1 + 88) <= 0 )
    return -1;
  for ( i = 0; ; i += 20 )
  {
    v2 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 28))(*(_DWORD *)(a1 + 40));
    if ( sub_46D810(v2) )
    {
      v3 = *(_DWORD *)(a1 + 92);
      if ( *(_DWORD *)(v3 + i) )
      {
        v4 = *(_DWORD *)(v3 + i + 12);
        v5 = *(_DWORD *)(v4 + 56) & 0x7FFFFFFF;
        if ( v5 )
        {
          v6 = 0;
          if ( *(int *)(v4 + 56) < 0 )
            v6 = sub_46DC40(*(_DWORD *)(v4 + 56) & 0x7FFFFFFF);
          if ( sub_46DB40(v5) || sub_46E270(*(_DWORD *)(a1 + 140), v5) || v6 )
            break;
        }
      }
    }
    if ( ++v8 >= *(_DWORD *)(a1 + 88) )
      return -1;
  }
  return v8;
}

// ===== sub_449FA0 @ 0x00449FA0..0x00449FF8 =====
int __thiscall sub_449FA0(_DWORD *this, int a2, int a3)
{
  int result; // eax
  int v4; // edx

  if ( a2 != -1 && (a2 < 0 || a2 >= this[22]) )
    return 0;
  result = 1;
  if ( a2 == -1 )
  {
    this[26] = -1;
    this[27] = -1;
  }
  else
  {
    v4 = this[23] + 20 * a2;
    this[26] = *(_DWORD *)(v4 + 4);
    this[27] = *(_DWORD *)(v4 + 8);
  }
  this[28] = a3;
  return result;
}

// ===== sub_44A000 @ 0x0044A000..0x0044A04A =====
int __thiscall sub_44A000(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax

  result = 0;
  if ( (a2 == -1 || a2 >= 0 && a2 < this[13]) && (a3 == -1 || a3 >= 0 && a3 < *(_DWORD *)(52 * a2 + this[14])) )
  {
    this[28] = a4;
    this[26] = a2;
    this[27] = a3;
    return 1;
  }
  return result;
}

// ===== sub_44A050 @ 0x0044A050..0x0044A1C2 =====
int __thiscall sub_44A050(int this)
{
  int v2; // esi
  int v3; // eax
  _DWORD *v4; // esi
  _DWORD *v5; // ebx
  void (__thiscall ***v6)(_DWORD, int); // ecx
  int result; // eax
  int v8; // [esp-4h] [ebp-1Ch]
  int i; // [esp+Ch] [ebp-Ch]
  _DWORD *v10; // [esp+10h] [ebp-8h]
  int v11; // [esp+14h] [ebp-4h]

  v2 = *(_DWORD *)(this + 40);
  *(_DWORD *)(this + 48) = 0;
  v8 = (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 28))(v2);
  (*(void (__thiscall **)(int))(*(_DWORD *)v2 + 28))(v2);
  sub_46DF00(v8);
  v3 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 44) + 28))(*(_DWORD *)(this + 44));
  sub_46DF00(v3);
  v4 = *(_DWORD **)(this + 56);
  v5 = *(_DWORD **)(this + 92);
  v10 = v4;
  for ( i = 0; i < *(_DWORD *)(this + 52); ++i )
  {
    v11 = 0;
    if ( (int)*v4 > 0 )
    {
      do
      {
        if ( *v5 )
        {
          sub_46D800();
          sub_41AC40(*(_DWORD *)(this + 40), v5[4]);
          v6 = (void (__thiscall ***)(_DWORD, int))v5[4];
          if ( v6 )
            (**v6)(v6, 1);
        }
        v4 = v10;
        v5 += 5;
        ++v11;
      }
      while ( v11 < *v10 );
    }
    operator delete[]((void *)v4[1]);
    v4 += 13;
    v10 = v4;
  }
  operator delete[](*(void **)(this + 56));
  operator delete[](*(void **)(this + 92));
  operator delete[](*(void **)(this + 84));
  if ( *(_DWORD *)(this + 64) )
  {
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
    sub_46D7A0();
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
    sub_46D7B0();
    sub_490AE0();
    sub_46E550();
  }
  else if ( *(_DWORD *)(this + 68) )
  {
    sub_46D800();
  }
  *(_DWORD *)(this + 52) = 0;
  *(_DWORD *)(this + 56) = 0;
  *(_DWORD *)(this + 60) = -1;
  *(_DWORD *)(this + 64) = 0;
  *(_DWORD *)(this + 68) = 0;
  *(_DWORD *)(this + 72) = 0;
  *(_DWORD *)(this + 76) = 0;
  *(_DWORD *)(this + 80) = 0;
  *(_DWORD *)(this + 84) = 0;
  *(_DWORD *)(this + 88) = 0;
  *(_DWORD *)(this + 92) = 0;
  *(_DWORD *)(this + 116) = -1;
  do
    result = sub_44A220(this);
  while ( result );
  return result;
}

// ===== sub_44A1D0 @ 0x0044A1D0..0x0044A217 =====
_DWORD *__userpurge sub_44A1D0@<eax>(int a1@<eax>, int a2, int a3, int a4)
{
  int v4; // esi
  int i; // eax
  _DWORD *result; // eax

  v4 = a1 + 148;
  for ( i = *(_DWORD *)(a1 + 160); i; i = *(_DWORD *)(i + 12) )
    v4 = i;
  result = operator new(0x10u);
  *result = a2;
  result[1] = a3;
  result[2] = a4;
  result[3] = 0;
  *(_DWORD *)(v4 + 12) = result;
  return result;
}

// ===== sub_44A220 @ 0x0044A220..0x0044A24A =====
BOOL __thiscall sub_44A220(_DWORD *this)
{
  int v1; // eax
  BOOL v2; // esi
  void *v4; // [esp-4h] [ebp-8h]

  v1 = this[40];
  v2 = v1 != 0;
  if ( v1 )
  {
    v4 = (void *)this[40];
    this[40] = *(_DWORD *)(v1 + 12);
    operator delete(v4);
  }
  return v2;
}

// ===== sub_44A250 @ 0x0044A250..0x0044A3B9 =====
int __thiscall sub_44A250(int *this, int a2, int a3)
{
  int v4; // esi
  int result; // eax

  v4 = 0;
  switch ( *(_DWORD *)a3 )
  {
    case 0x10000000:
      if ( a2 != 3
        || !(*(int (__thiscall **)(int *, _DWORD, _DWORD, _DWORD))(*this + 44))(
              this,
              *(__int16 *)(a3 + 6),
              *(__int16 *)(a3 + 4),
              *(_DWORD *)(a3 + 8)) )
      {
        goto LABEL_21;
      }
      if ( (*(int (__thiscall **)(int *))(*this + 20))(this) )
        this[12] = 0;
      result = 1;
      break;
    case 0x10000001:
      if ( a2 != 2 )
        goto LABEL_21;
      result = sub_449A60(this, *(_DWORD *)(a3 + 4));
      break;
    case 0x10000002:
      if ( a2 != 3 )
        goto LABEL_21;
      result = (*(int (__thiscall **)(int *, _DWORD, _DWORD))(*this + 36))(
                 this,
                 *(_DWORD *)(a3 + 4),
                 *(_DWORD *)(a3 + 8));
      break;
    case 0x10000003:
      if ( a2 != 2 )
        goto LABEL_21;
      this[34] = *(_DWORD *)(a3 + 4);
      result = 1;
      break;
    case 0x10000004:
      if ( a2 != 5 )
        goto LABEL_21;
      result = (*(int (__thiscall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD))(*this + 56))(
                 this,
                 *(_DWORD *)(a3 + 4),
                 *(_DWORD *)(a3 + 8),
                 *(_DWORD *)(a3 + 12),
                 *(_DWORD *)(a3 + 16));
      break;
    case 0x10000005:
      if ( a2 != 4 )
        goto LABEL_21;
      result = sub_44A600(*(_DWORD *)(a3 + 12));
      break;
    case 0x10000006:
      if ( a2 != 5 )
        goto LABEL_21;
      result = (*(int (__thiscall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD))(*this + 64))(
                 this,
                 *(_DWORD *)(a3 + 4),
                 *(_DWORD *)(a3 + 8),
                 *(_DWORD *)(a3 + 12),
                 *(_DWORD *)(a3 + 16));
      break;
    case 0x10000007:
      if ( a2 == 6 )
        v4 = (*(int (__thiscall **)(int *, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD))(*this + 60))(
               this,
               *(_DWORD *)(a3 + 4),
               *(_DWORD *)(a3 + 8),
               *(_DWORD *)(a3 + 12),
               *(_DWORD *)(a3 + 16),
               *(_DWORD *)(a3 + 20));
      goto LABEL_21;
    default:
LABEL_21:
      result = v4;
      break;
  }
  return result;
}

// ===== sub_44A3E0 @ 0x0044A3E0..0x0044A5B2 =====
int __thiscall sub_44A3E0(_DWORD *this, int a2, int a3, int a4, int a5)
{
  int *v6; // esi
  int v7; // eax
  int v8; // edx
  int v9; // eax
  bool v10; // zf
  int v11; // eax
  int v12; // esi
  int result; // eax
  int v14; // eax
  int v15; // edi
  int v16; // [esp+28h] [ebp-28h]
  int v17; // [esp+2Ch] [ebp-24h] BYREF
  int *v18; // [esp+30h] [ebp-20h]
  int v19; // [esp+34h] [ebp-1Ch] BYREF
  _DWORD v20[6]; // [esp+38h] [ebp-18h] BYREF

  v16 = 0;
  if ( a2 < 0 )
    return v16;
  if ( a2 >= this[13] )
    return v16;
  v6 = (int *)(this[14] + 52 * a2);
  v18 = v6;
  if ( a3 < 0 || a3 >= *v6 )
    return v16;
  v17 = 0;
  v7 = this[29];
  if ( v7 != -1 )
  {
    v8 = this[23];
    v9 = 5 * v7;
    v10 = a2 == *(_DWORD *)(v8 + 4 * v9 + 4);
    v11 = v8 + 4 * v9;
    if ( !v10 || (v17 = 1, a3 != *(_DWORD *)(v11 + 8)) )
      v17 = 0;
  }
  (*(void (__thiscall **)(_DWORD *, int *, int, int, int, int))(*this + 32))(this, &v19, a2, a3, v17, 1);
  switch ( a4 )
  {
    case 0:
      v12 = 60 * a3;
      *(_DWORD *)(v18[1] + 60 * a3 + 12) = a5;
      goto LABEL_11;
    case 1:
      v12 = 60 * a3;
      *(_DWORD *)(v18[1] + 60 * a3 + 20) = a5;
      goto LABEL_11;
    case 2:
      v12 = 60 * a3;
      *(_DWORD *)(v18[1] + 60 * a3 + 16) = a5;
LABEL_11:
      (*(void (__thiscall **)(_DWORD *, int *, int, int, int, int))(*this + 32))(this, &v17, a2, a3, v17, 1);
      sub_449DF0(v12 + v18[1], v17, (int)this, v19);
      return 1;
    case 4:
      *(_DWORD *)(v6[1] + 60 * a3 + 24) = a5;
      v14 = sub_44A6A0(a2, a3);
      v15 = v14;
      if ( !v14 )
        return v16;
      if ( a5 == -2 )
      {
        sub_41BDD0(v14);
        result = 1;
      }
      else if ( a5 == -1 )
      {
        sub_41BC70(v14, 0);
        result = 1;
      }
      else
      {
        if ( !sub_407F20(dword_565D70, a5, v20) )
          return v16;
        sub_41BC70(v15, (char *)v20);
        result = 1;
      }
      break;
    default:
      return v16;
  }
  return result;
}

// ===== sub_44A5D0 @ 0x0044A5D0..0x0044A5FE =====
BOOL __thiscall sub_44A5D0(_DWORD *this, int a2, int a3, int a4, int a5, int a6)
{
  BOOL result; // eax

  result = 0;
  if ( a2 >= 0 && a2 < this[13] && a3 >= 0 )
    return a3 < *(_DWORD *)(52 * a2 + this[14]);
  return result;
}

// ===== sub_44A600 @ 0x0044A600..0x0044A63C =====
int __userpurge sub_44A600@<eax>(_DWORD *a1@<ecx>, int a2@<edi>, int a3@<esi>, int a4)
{
  int result; // eax
  int *v5; // edx

  result = 0;
  if ( a3 >= 0 && a3 < a1[13] )
  {
    v5 = (int *)(a1[14] + 52 * a3);
    if ( a2 >= 0 && a2 < *v5 )
      return (*(int (__thiscall **)(_DWORD *, int, int, _DWORD, _DWORD, int))(*a1 + 60))(
               a1,
               a3,
               a2,
               *(_DWORD *)(v5[1] + 4),
               *(_DWORD *)(v5[1] + 8),
               a4);
  }
  return result;
}

// ===== sub_44A640 @ 0x0044A640..0x0044A66E =====
BOOL __thiscall sub_44A640(_DWORD *this, int a2, int a3, int a4, int a5)
{
  BOOL result; // eax

  result = 0;
  if ( a2 >= 0 && a2 < this[13] && a3 >= 0 )
    return a3 < *(_DWORD *)(52 * a2 + this[14]);
  return result;
}

// ===== sub_44A670 @ 0x0044A670..0x0044A688 =====
int __thiscall sub_44A670(_DWORD *this, int a2)
{
  int result; // eax

  result = -1;
  if ( a2 >= 0 && a2 < this[22] )
    return a2;
  return result;
}

// ===== sub_44A690 @ 0x0044A690..0x0044A698 =====
int __stdcall sub_44A690(int a1)
{
  return 1;
}

// ===== sub_44A6A0 @ 0x0044A6A0..0x0044A6E7 =====
int __fastcall sub_44A6A0(int a1, int a2, int a3, int a4)
{
  int v4; // esi
  int result; // eax
  int v6; // ecx
  _DWORD *v7; // edi
  _DWORD *i; // edx

  v4 = *(_DWORD *)(a2 + 88);
  result = 0;
  v6 = 0;
  if ( v4 > 0 )
  {
    v7 = *(_DWORD **)(a2 + 92);
    for ( i = v7; !*i || i[1] != a3 || i[2] != a4; i += 5 )
    {
      if ( ++v6 >= v4 )
        return result;
    }
    return v7[5 * v6 + 4];
  }
  return result;
}

// ===== sub_44A6F0 @ 0x0044A6F0..0x0044A7BA =====
int __userpurge sub_44A6F0@<eax>(int a1@<ecx>, int a2@<edi>, _DWORD *a3, int a4)
{
  int v5; // ebx
  int v6; // ecx
  int v8; // [esp+Ch] [ebp-1Ch] BYREF
  int v9; // [esp+10h] [ebp-18h]
  int v10; // [esp+14h] [ebp-14h] BYREF
  int v11; // [esp+18h] [ebp-10h]
  int v12; // [esp+1Ch] [ebp-Ch]
  int v13; // [esp+20h] [ebp-8h]

  v5 = sub_44A6A0(a1, a2, a1, a4);
  if ( !v5 )
    return 0;
  if ( *(_DWORD *)(52 * a1 + *(_DWORD *)(a2 + 56) + 20)
    && ((*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a2 + 44) + 28))(*(_DWORD *)(a2 + 44)), (sub_46E490() & 1) != 0) )
  {
    sub_48E680(&v8);
  }
  else
  {
    sub_48E590(0, &v8);
  }
  (*(void (__thiscall **)(int, int *))(*(_DWORD *)v5 + 36))(v5, &v10);
  if ( v10 > v8 || v8 > v12 || v11 > v9 || v9 > v13 )
    return 0;
  v6 = v8 - v10;
  a3[1] = v9 - v11;
  *a3 = v6;
  return 1;
}

// ===== sub_44A7C0 @ 0x0044A7C0..0x0044A867 =====
_DWORD *__thiscall sub_44A7C0(void *this, _DWORD *a2)
{
  sub_447990(this, a2);
  *a2 = &DCIPIconEx::`vftable';
  a2[9] = 1;
  a2[41] = 0;
  a2[42] = 0;
  a2[43] = 0;
  a2[44] = 0;
  a2[45] = 0;
  a2[46] = 0;
  a2[47] = 0;
  a2[48] = 0;
  a2[49] = 0;
  a2[50] = 0;
  a2[51] = 0;
  a2[52] = 0;
  a2[53] = 0;
  return a2;
}

// ===== sub_44A870 @ 0x0044A870..0x0044A892 =====
void *__thiscall sub_44A870(void *this, char a2)
{
  sub_44A8A0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44A8A0 @ 0x0044A8A0..0x0044A8FA =====
BOOL __stdcall sub_44A8A0(_DWORD *a1)
{
  *a1 = &DCIPIconEx::`vftable';
  sub_44C2D0(a1);
  return sub_447B10(a1);
}

// ===== sub_44A900 @ 0x0044A900..0x0044B254 =====
int __stdcall sub_44A900(int a1, int *a2)
{
  int v2; // ebx
  int v3; // edx
  int v4; // ecx
  int v5; // esi
  int v6; // edi
  BOOL v7; // edi
  int v8; // edx
  int v9; // esi
  int v10; // edx
  int v11; // ecx
  int v12; // edi
  int v13; // eax
  int v14; // ecx
  bool v15; // zf
  void *v16; // eax
  int *v17; // edx
  int *v18; // eax
  int v19; // edx
  char *v20; // esi
  int v21; // edi
  int v22; // eax
  int v23; // ecx
  _DWORD *v24; // ebx
  int v25; // ecx
  int v26; // eax
  int *v27; // edx
  _DWORD *v28; // ecx
  int v29; // eax
  int v30; // eax
  _DWORD *v31; // eax
  int v32; // eax
  int v33; // eax
  int v34; // eax
  int v35; // ecx
  char *v36; // esi
  char *i; // edi
  int *v38; // eax
  _DWORD *v39; // eax
  _DWORD *v40; // edi
  int v41; // eax
  int v42; // eax
  int v43; // esi
  int v44; // eax
  int v45; // esi
  int v46; // eax
  int v47; // eax
  _DWORD *v48; // ecx
  int v49; // eax
  BOOL v50; // ecx
  int v51; // eax
  int v52; // eax
  int v54; // [esp+4h] [ebp-A8h]
  char *v55; // [esp+4h] [ebp-A8h]
  int v56; // [esp+4h] [ebp-A8h]
  int v57; // [esp+4h] [ebp-A8h]
  int v58; // [esp+20h] [ebp-8Ch]
  int v59; // [esp+20h] [ebp-8Ch]
  int v60; // [esp+28h] [ebp-84h]
  int v61; // [esp+28h] [ebp-84h]
  int *v62; // [esp+2Ch] [ebp-80h]
  int v63; // [esp+30h] [ebp-7Ch]
  int *v64; // [esp+34h] [ebp-78h]
  int *v65; // [esp+38h] [ebp-74h]
  int *v66; // [esp+3Ch] [ebp-70h]
  char *v67; // [esp+40h] [ebp-6Ch]
  void *v68; // [esp+44h] [ebp-68h]
  void (__thiscall **v69)(int, int); // [esp+44h] [ebp-68h]
  int v70; // [esp+48h] [ebp-64h]
  int v71; // [esp+4Ch] [ebp-60h]
  _DWORD *v72; // [esp+50h] [ebp-5Ch]
  int v73; // [esp+54h] [ebp-58h] BYREF
  int v74; // [esp+58h] [ebp-54h]
  _DWORD v75[2]; // [esp+5Ch] [ebp-50h] BYREF
  char v76; // [esp+64h] [ebp-48h] BYREF
  char *v77; // [esp+6Ch] [ebp-40h]
  _DWORD v78[6]; // [esp+70h] [ebp-3Ch] BYREF
  int v79[4]; // [esp+88h] [ebp-24h] BYREF
  int v80; // [esp+A8h] [ebp-4h]

  v2 = a1;
  (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 52))(a1);
  sub_42BBB0(*(_DWORD *)(a1 + 40));
  sub_42BBD0(0, v3, *(void **)(a1 + 40));
  sub_42B9B0(*(_DWORD *)(a1 + 40));
  sub_42B550(*(_DWORD *)(a1 + 40), 0);
  sub_42B540(*(_DWORD *)(a1 + 40), v4);
  sub_42CA00(*(_DWORD *)(a1 + 40));
  v5 = *a2;
  v6 = -2147483647;
  if ( *a2 > 0 && v5 <= 256 )
  {
    v7 = 1;
    v58 = 0;
    *(_DWORD *)(a1 + 84) = operator new[](8 * v5);
    v9 = 0;
    if ( *a2 <= 0 )
    {
LABEL_11:
      sub_42BBD0(v58, v8, *(void **)(a1 + 40));
      qmemcpy((void *)(a1 + 164), a2, 0x28u);
      *(_DWORD *)(a1 + 168) = operator new[](*a2 << 6);
      *(_DWORD *)(a1 + 52) = *a2;
      *(_DWORD *)(a1 + 56) = operator new[](52 * *a2);
      v14 = a2[2];
      if ( v14 < 0 || v14 >= *a2 )
        v14 = -1;
      *(_DWORD *)(a1 + 60) = v14;
      *(_DWORD *)(a1 + 64) = a2[3];
      *(_DWORD *)(a1 + 68) = a2[4];
      *(_DWORD *)(a1 + 72) = a2[5];
      *(_DWORD *)(a1 + 76) = a2[6] & 7;
      *(_DWORD *)(a1 + 80) = a2[7];
      v15 = a2[8] == 0;
      *(_DWORD *)(a1 + 88) = v58;
      *(_DWORD *)(a1 + 136) = v15;
      *(_DWORD *)(a1 + 92) = operator new[](20 * v58);
      v16 = operator new[](4 * *a2);
      v17 = *(int **)(a1 + 92);
      *(_DWORD *)(a1 + 204) = v16;
      v65 = *(int **)(a1 + 56);
      v18 = a2;
      v64 = v17;
      v19 = 0;
      v20 = 0;
      v21 = 0;
      v62 = (int *)a2[1];
      v67 = 0;
      v77 = 0;
      v71 = 0;
      v61 = 0;
      if ( *a2 > 0 )
      {
        v63 = 0;
        while ( 1 )
        {
          qmemcpy((void *)(v19 + *(_DWORD *)(v2 + 168)), (const void *)(v19 + v18[1]), 0x40u);
          *(_DWORD *)(*(_DWORD *)(v2 + 168) + v63 + 8) = operator new[](196 * *(_DWORD *)(v19 + v18[1]));
          *v65 = *v62;
          v65[1] = (int)operator new[](60 * *v62);
          v22 = v62[3];
          if ( v22 < 0 || v22 >= *v62 )
            v22 = -1;
          v65[2] = v22;
          v65[3] = v62[5];
          v65[4] = v62[6];
          v65[5] = v62[7];
          v65[6] = v62[8];
          v65[7] = v62[9];
          v65[8] = v62[10];
          v65[9] = v62[11];
          v65[10] = v62[12];
          v65[11] = v62[13];
          v65[12] = v62[14];
          *(_DWORD *)(*(_DWORD *)(v2 + 204) + 4 * v61) = operator new[](48 * *v62);
          v23 = *(_DWORD *)(*(_DWORD *)(v2 + 204) + 4 * v61);
          v24 = (_DWORD *)v62[2];
          v72 = (_DWORD *)v65[1];
          v59 = 0;
          if ( *v62 > 0 )
          {
            v70 = 0;
            v66 = (int *)(v23 + 12);
            do
            {
              if ( v59 == v65[2] && !v24[1] )
              {
                v65[2] = -1;
                *(_DWORD *)(*(_DWORD *)(a1 + 168) + v63 + 12) = -1;
              }
              memset(v66 - 3, 0, 0x30u);
              qmemcpy((void *)(v70 + *(_DWORD *)(*(_DWORD *)(a1 + 168) + v63 + 8)), v24, 0xC4u);
              v25 = v24[8];
              v68 = (void *)v25;
              if ( v25 != -1 && v59 == v65[2] && v24[10] != -1 )
              {
                v68 = (void *)v24[10];
                v25 = (int)v68;
              }
              if ( *v24 && v24[1] || v24[1] )
                v26 = sub_407F20(dword_565D70, v25, v78);
              else
                v26 = 0;
              v27 = v64;
              v64[1] = v61;
              v64[2] = v59;
              v28 = v72;
              *v64 = v26;
              v64[3] = (int)v72;
              v64[4] = 0;
              if ( v26 )
              {
                *(v66 - 3) = 1;
                *(v66 - 2) = v70 + *(_DWORD *)(*(_DWORD *)(a1 + 168) + v63 + 8);
                *(v66 - 1) = (int)v67;
                v29 = v24[48];
                if ( (v29 & 0x10) != 0 )
                {
                  v30 = v24[1];
                }
                else if ( (v29 & 2) != 0 )
                {
                  v30 = v24[3];
                }
                else
                {
                  v30 = (int)v67;
                }
                *v66 = v30;
                *v72 = *v24;
                v72[1] = v24[2];
                v72[2] = v24[3];
                v72[3] = v24[8];
                v72[4] = v24[10];
                v72[5] = v24[9];
                v72[6] = v24[12];
                v72[7] = v24[42];
                v72[8] = v24[43];
                v72[9] = v24[44];
                v72[10] = v24[45];
                v72[11] = v24[46];
                v72[12] = v24[47];
                v72[13] = 0;
                v72[14] = 0;
                sub_42BED0(
                  *(_DWORD **)(a1 + 40),
                  (unsigned int)v67,
                  (int)v68,
                  v24[4] + v24[2],
                  v24[5] + v24[3],
                  v24[4],
                  v24[5],
                  *v66);
                sub_42C1C0((unsigned int)v67, &v79, *(_DWORD **)(a1 + 40));
                v31 = operator new(0x134u);
                v80 = 0;
                if ( v31 )
                  v32 = sub_42AC50(v67, v31, *(_DWORD *)(a1 + 40));
                else
                  v32 = 0;
                v80 = -1;
                v54 = v78[3];
                v64[4] = v32;
                sub_41AB10(v54);
                v69 = (void (__thiscall **)(int, int))(*(_DWORD *)v64[4] + 4);
                v33 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 40) + 8))(*(_DWORD *)(a1 + 40));
                (*v69)(v64[4], v33);
                sub_41B310(&v73, *(_DWORD *)(a1 + 40));
                (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v64[4] + 56))(v64[4], v73, v74);
                sub_41B360(&v73, *(_DWORD *)(a1 + 40));
                sub_41B320((_DWORD *)v64[4], v73, v74);
                v34 = sub_42E9A0(*(_DWORD *)(a1 + 40));
                sub_41ADF0(v64[4], v34);
                sub_41AB40(v64[4], *(_DWORD **)(a1 + 40), v24[2], v24[3]);
                if ( !*v24 || (v35 = v24[12], v35 == -2) )
                {
                  sub_41BDD0(v64[4]);
                }
                else if ( sub_407F20(dword_565D70, v35, v78) )
                {
                  sub_41BC70(v64[4], (char *)v78);
                }
                sub_46D6E0(v64[4]);
                v36 = v77;
                for ( i = &v76; v36; v36 = (char *)*((_DWORD *)v36 + 2) )
                {
                  if ( *(_DWORD *)v36 <= (unsigned int)*v66 )
                    break;
                  i = v36;
                }
                v38 = (int *)operator new(0xCu);
                *v38 = *v66;
                v27 = v64;
                v38[1] = (int)v67;
                v28 = v72;
                ++v71;
                v38[2] = (int)v36;
                *((_DWORD *)i + 2) = v38;
              }
              else
              {
                v72[1] = 0;
                v72[2] = 0;
                v72[3] = -1;
                v72[4] = -1;
                v72[5] = -1;
                v72[6] = -1;
                v72[7] = 0;
                v72[8] = 0;
                v72[9] = -1;
                v72[10] = 0;
                v72[11] = 0;
                v72[12] = -1;
                v72[14] = 0;
              }
              ++v67;
              v70 += 196;
              v66 += 12;
              v64 = v27 + 5;
              v24 += 49;
              v72 = v28 + 15;
              ++v59;
            }
            while ( v59 < *v62 );
          }
          v65 += 13;
          v2 = a1;
          v62 += 16;
          v63 += 64;
          if ( ++v61 >= *a2 )
            break;
          v19 = v63;
          v18 = a2;
        }
        v21 = v71;
        v20 = v77;
      }
      v39 = operator new[](4 * v21);
      *(_DWORD *)(v2 + 212) = v21;
      *(_DWORD *)(v2 + 208) = v39;
      v40 = v39;
      while ( v20 )
      {
        *v40 = *((_DWORD *)v20 + 1);
        v55 = v20;
        v20 = (char *)*((_DWORD *)v20 + 2);
        ++v40;
        operator delete(v55);
      }
      (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(v2 + 40) + 48))(*(_DWORD *)(v2 + 40), v75);
      (*(void (__thiscall **)(_DWORD, _DWORD, _DWORD))(**(_DWORD **)(v2 + 40) + 44))(
        *(_DWORD *)(v2 + 40),
        v75[0],
        v75[1]);
      if ( *(_DWORD *)(v2 + 64) )
      {
        (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(v2 + 40) + 28))(*(_DWORD *)(v2 + 40));
        sub_46D6C0();
        v41 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(v2 + 40) + 28))(*(_DWORD *)(v2 + 40));
        sub_46D720(v41);
        v56 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(v2 + 40) + 28))(*(_DWORD *)(v2 + 40));
        v42 = sub_490AE0();
        sub_46E500(v42 | 0x80000000, v56);
        sub_449A60((int *)v2, *(_DWORD *)(v2 + 60));
      }
      else if ( *(_DWORD *)(v2 + 68) )
      {
        sub_46D6E0(*(_DWORD *)(v2 + 40));
      }
      v43 = *(_DWORD *)(v2 + 40);
      v57 = (*(int (__thiscall **)(int))(*(_DWORD *)v43 + 28))(v43);
      (*(void (__thiscall **)(int))(*(_DWORD *)v43 + 28))(v43);
      sub_46DF00(v57);
      v44 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(v2 + 44) + 28))(*(_DWORD *)(v2 + 44));
      sub_46DF00(v44);
      v45 = *(_DWORD *)v2;
      v46 = sub_449760((_DWORD *)v2);
      if ( (*(int (__thiscall **)(int, int, int))(v45 + 28))(v2, v46, 1) )
      {
        v47 = *(_DWORD *)(v2 + 116);
        if ( v47 == -1 )
        {
          v49 = -1;
          v50 = 0;
        }
        else
        {
          v48 = (_DWORD *)(*(_DWORD *)(v2 + 92) + 20 * v47);
          v49 = v48[2] | (v48[1] << 16);
          v50 = *(_DWORD *)(v48[3] + 20) != -1;
        }
        (*(void (__thiscall **)(int, int, BOOL))(*(_DWORD *)v2 + 16))(v2, v49, v50);
      }
      v51 = a2[2];
      if ( v51 == -1 || *(_DWORD *)((v51 << 6) + a2[1] + 12) == -1 )
      {
        *(_DWORD *)(v2 + 124) = 0x7FFFFFFF;
        *(_DWORD *)(v2 + 120) = 0x7FFFFFFF;
      }
      else
      {
        sub_48E680(v2 + 120);
      }
      v52 = sub_4495C0((_DWORD *)v2, 0, 0, 0);
      *(_DWORD *)(v2 + 132) = -1;
      *(_DWORD *)(v2 + 128) = v52;
      *(_DWORD *)(v2 + 48) = 1;
      v6 = 0;
    }
    else
    {
      v60 = 0;
      while ( v7 )
      {
        v10 = a2[1];
        v11 = *(_DWORD *)(v60 + v10 + 4);
        v12 = *(_DWORD *)(v60 + v10);
        if ( v11 > v12 || v11 <= 0 )
          v11 = *(_DWORD *)(v60 + v10);
        *(_DWORD *)(*(_DWORD *)(a1 + 84) + 8 * v9) = v11;
        v13 = (v12 + v11 - 1) / v11;
        v58 += v12;
        v8 = 255;
        v60 += 64;
        ++v9;
        v7 = (unsigned int)(v12 - 1) <= 0xFF;
        *(_DWORD *)(*(_DWORD *)(a1 + 84) + 8 * v9 - 4) = v13;
        if ( v9 >= *a2 )
        {
          if ( v7 )
            goto LABEL_11;
          break;
        }
      }
      operator delete[](*(void **)(a1 + 84));
      v6 = -2147483646;
    }
  }
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(v2 + 40) + 12))(*(_DWORD *)(v2 + 40));
  sub_4478D0(v2);
  return v6;
}

// ===== sub_44B260 @ 0x0044B260..0x0044B455 =====
int __cdecl sub_44B260(_DWORD *a1)
{
  int v1; // ecx
  _DWORD *v2; // ebx
  void *v3; // esi
  int v4; // edx
  int v5; // eax
  int v6; // ecx
  int v7; // edi
  int v8; // edx
  BOOL v9; // eax
  int v10; // eax
  int *v11; // eax
  unsigned int v12; // edi
  int v13; // edx
  int *v14; // ebx
  int v15; // ecx
  int v16; // eax
  int v17; // edx
  int v19; // [esp+10h] [ebp-48h]
  int *v20; // [esp+10h] [ebp-48h]
  int v21; // [esp+14h] [ebp-44h]
  int v22; // [esp+14h] [ebp-44h]
  int v23; // [esp+18h] [ebp-40h]
  int v24; // [esp+18h] [ebp-40h]
  _DWORD *v25; // [esp+1Ch] [ebp-3Ch]
  unsigned int v26; // [esp+20h] [ebp-38h]
  int v27; // [esp+24h] [ebp-34h]
  int v28[6]; // [esp+2Ch] [ebp-2Ch] BYREF
  int v29[4]; // [esp+44h] [ebp-14h] BYREF

  v2 = a1;
  v3 = (void *)v1;
  v25 = (_DWORD *)v1;
  sub_42BBB0(v1);
  sub_42BBD0(0, v4, v3);
  sub_42B9B0((int)v3);
  v5 = sub_42B550((int)v3, 0);
  sub_42B540(v5, v6);
  sub_42CA00((int)v3);
  v7 = *a1;
  v8 = 0;
  v19 = -2147483647;
  if ( (int)*a1 > 0 && v7 <= 256 )
  {
    v9 = 1;
    v21 = 0;
    v23 = 0;
    if ( v7 <= 0 )
    {
LABEL_11:
      sub_42BBD0(v21, v8, v3);
      v11 = (int *)a1[1];
      v12 = 0;
      v20 = v11;
      v26 = 0;
      v24 = 0;
      if ( (int)*a1 > 0 )
      {
        do
        {
          v13 = 0;
          v27 = 0;
          if ( *v11 > 0 )
          {
            v14 = (int *)(v11[2] + 40);
            do
            {
              if ( *(v14 - 9) )
              {
                v15 = *(v14 - 2);
                v22 = v15;
                if ( v15 != -1 && v13 == v11[3] && *v14 != -1 )
                {
                  v22 = *v14;
                  v15 = *v14;
                }
                if ( sub_407F20(dword_565D70, v15, v28) )
                {
                  v16 = v14[38];
                  if ( (v16 & 0x10) != 0 )
                  {
                    v17 = *(v14 - 9);
                  }
                  else if ( (v16 & 2) != 0 )
                  {
                    v17 = *(v14 - 7);
                  }
                  else
                  {
                    v17 = v12;
                  }
                  sub_42BED0(
                    v25,
                    v12,
                    v22,
                    *(v14 - 6) + *(v14 - 8),
                    *(v14 - 5) + *(v14 - 7),
                    *(v14 - 6),
                    *(v14 - 5),
                    v17);
                  sub_42C1C0(v26, v29, v25);
                  v12 = v26;
                }
                v11 = v20;
              }
              v13 = v27 + 1;
              ++v12;
              v14 += 49;
              v26 = v12;
              v27 = v13;
            }
            while ( v13 < *v11 );
            v2 = a1;
            v3 = v25;
          }
          v11 += 16;
          v20 = v11;
          ++v24;
        }
        while ( v24 < *v2 );
      }
      v19 = 0;
    }
    else
    {
      while ( v9 )
      {
        v10 = *(_DWORD *)(v8 + a1[1]);
        v21 += v10;
        v9 = v10 > 0 && *(int *)(v8 + a1[1]) <= 256;
        v8 += 64;
        if ( ++v23 >= v7 )
        {
          if ( v9 )
            goto LABEL_11;
          break;
        }
      }
      v19 = -2147483646;
    }
  }
  (*(void (__thiscall **)(void *))(*(_DWORD *)v3 + 12))(v3);
  return v19;
}

// ===== sub_44B460 @ 0x0044B460..0x0044B534 =====
int __fastcall sub_44B460(int a1, int a2, int *a3, int a4, int a5)
{
  _DWORD *v5; // esi
  unsigned int v6; // eax
  int v8[4]; // [esp+Ch] [ebp-14h] BYREF

  if ( a4 >= a3[41] )
    return -2147483647;
  if ( a1 >= *(_DWORD *)((a4 << 6) + a3[42]) )
    return -2147483646;
  v5 = (_DWORD *)(*(_DWORD *)(a3[51] + 4 * a4) + 48 * a1);
  if ( *v5 )
  {
    sub_42C060(v5[2], a3[10], a5);
    sub_42C1C0(v5[2], v8, (_DWORD *)a3[10]);
    v6 = (*(int (__thiscall **)(int))(*(_DWORD *)a3[10] + 28))(a3[10]);
    sub_443240(v8, v6, dword_565D6C);
    sub_4478D0((int)a3);
  }
  return 0;
}

// ===== sub_44B540 @ 0x0044B540..0x0044B96D =====
int __thiscall sub_44B540(int this)
{
  int result; // eax
  int *v2; // esi
  int v3; // ecx
  int v4; // ecx
  int v5; // edi
  int *v6; // eax
  _DWORD *v7; // ebx
  int *v8; // ecx
  int v9; // edx
  int v10; // ecx
  int v11; // eax
  unsigned int v12; // eax
  int v13; // eax
  int v14; // ecx
  unsigned int v15; // eax
  unsigned int v16; // eax
  int v17; // ecx
  int v18; // eax
  unsigned int v19; // eax
  unsigned int v20; // eax
  int v21; // esi
  int v22; // edi
  int v23; // [esp+10h] [ebp-3Ch]
  int v25; // [esp+18h] [ebp-34h]
  int v26; // [esp+1Ch] [ebp-30h]
  int v27; // [esp+20h] [ebp-2Ch]
  int v28; // [esp+24h] [ebp-28h]
  int v29; // [esp+28h] [ebp-24h]
  int v30; // [esp+2Ch] [ebp-20h]
  unsigned int v31; // [esp+30h] [ebp-1Ch]
  int v32; // [esp+34h] [ebp-18h] BYREF
  int v33[4]; // [esp+38h] [ebp-14h] BYREF

  result = -1;
  v2 = (int *)this;
  v28 = -1;
  v29 = -1;
  if ( *(_DWORD *)(this + 116) != -1 )
  {
    v3 = *(_DWORD *)(this + 92);
    result = *(_DWORD *)(v3 + 20 * v2[29] + 8);
    v28 = *(_DWORD *)(v3 + 20 * v2[29] + 4);
    v29 = result;
  }
  v4 = 0;
  v23 = 0;
  if ( v2[41] > 0 )
  {
    v30 = 0;
    while ( 1 )
    {
      v5 = 0;
      v6 = (int *)(v4 + v2[42]);
      v25 = 0;
      if ( *v6 > 0 )
        break;
LABEL_49:
      result = v23 + 1;
      v4 += 64;
      v23 = result;
      v30 = v4;
      if ( result >= v2[41] )
        return result;
    }
    v27 = 0;
    while ( 1 )
    {
      v7 = (_DWORD *)(v27 + *(_DWORD *)(v2[51] + 4 * v23));
      if ( !*v7 )
        goto LABEL_48;
      v8 = (int *)v7[1];
      if ( v8[6] > 1 && v8[7] > 0 )
      {
        if ( v23 == v28 && v5 == v29 )
        {
          v9 = 1;
          v26 = 1;
        }
        else
        {
          v26 = 0;
          v9 = 0;
        }
        v10 = v8[48];
        if ( (v10 & 1) != 0 )
        {
          if ( !v9 )
            goto LABEL_26;
        }
        else if ( (v10 & 8) != 0 && !v9 && v5 != v6[3] )
        {
          goto LABEL_26;
        }
        if ( v7[5] )
        {
          v31 = v7[5];
          if ( v31 <= sub_498720() )
          {
            ++v7[4];
            v11 = v7[1];
            if ( v7[4] == *(_DWORD *)(v11 + 24) )
              v7[4] = 0;
            v7[5] = v31 + *(_DWORD *)(v11 + 28);
            (*(void (__thiscall **)(int *, int *, int, int, int, int))(*v2 + 32))(v2, &v32, v23, v5, v26, 1);
            sub_42C090(v7[2], v2[10], v32);
            sub_42C1C0(v7[2], v33, (_DWORD *)v2[10]);
            v2 = (int *)this;
            v12 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
            sub_443240(v33, v12, dword_565D6C);
            sub_4478D0(this);
            v5 = v25;
          }
        }
        else
        {
          v7[4] = 0;
          v7[5] = *(_DWORD *)(v7[1] + 28) + sub_498720();
        }
      }
LABEL_26:
      v13 = v7[1];
      if ( *(int *)(v13 + 56) > 0 )
      {
        if ( !*(_DWORD *)(v13 + 160)
          || v2[29] != -1
          && (*(int (__thiscall **)(int *, int, int))(*v2 + 40))(v2, v23, v5)
          && (v14 = v2[23], v23 == *(_DWORD *)(v14 + 20 * v2[29] + 4))
          && v5 == *(_DWORD *)(v14 + 20 * v2[29] + 8) )
        {
          if ( v7[10] )
          {
            if ( v7[10] <= (unsigned int)sub_498720() )
            {
              v17 = v7[8];
              if ( !v17 )
              {
                v18 = *(_DWORD *)(v7[1] + 12 * v7[7] + 64) / 4;
                if ( v18 <= 0 )
                  v18 = 1;
                v7[9] = v18;
              }
              v7[8] = v17 + 1;
              sub_42C220((_DWORD *)v2[10], v33, v7[2]);
              v19 = (*(int (__thiscall **)(int))(*(_DWORD *)v2[10] + 28))(v2[10]);
              sub_443240(v33, v19, dword_565D6C);
              sub_42C140(
                v7[2],
                v2[10],
                v7[6] + (int)v7[8] * (__int64)*(int *)(v7[1] + 4 * (3 * v7[7] + 15)) / (int)v7[9]);
              sub_42C1C0(v7[2], v33, (_DWORD *)v2[10]);
              v20 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
              sub_443240(v33, v20, dword_565D6C);
              sub_4478D0(this);
              if ( v7[8] == v7[9] )
              {
                v21 = v7[7];
                v22 = v7[1];
                v7[6] = (v7[6] + *(_DWORD *)(v22 + 4 * (3 * v21 + 15))) % 23592960;
                v7[7] = v21 + 1;
                if ( v21 + 1 == *(_DWORD *)(v22 + 56) )
                  v7[7] = 0;
                v7[8] = 0;
              }
              v7[10] += 4;
              v2 = (int *)this;
              v5 = v25;
            }
            goto LABEL_48;
          }
          v7[6] = *(_DWORD *)(v7[1] + 156);
          v7[7] = 0;
          v7[8] = 0;
LABEL_47:
          v7[10] = sub_498720() + 4;
          goto LABEL_48;
        }
        if ( v7[10] )
        {
          if ( *(_DWORD *)(v7[1] + 164) )
            goto LABEL_47;
          sub_42C220((_DWORD *)v2[10], v33, v7[2]);
          v15 = (*(int (__thiscall **)(int))(*(_DWORD *)v2[10] + 28))(v2[10]);
          sub_443240(v33, v15, dword_565D6C);
          sub_42C140(v7[2], v2[10], 0);
          sub_42C1C0(v7[2], v33, (_DWORD *)v2[10]);
          v2 = (int *)this;
          v16 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 40) + 28))(*(_DWORD *)(this + 40));
          sub_443240(v33, v16, dword_565D6C);
          sub_4478D0(this);
          v5 = v25;
          v7[10] = 0;
        }
      }
LABEL_48:
      v4 = v30;
      v27 += 48;
      ++v5;
      v6 = (int *)(v30 + v2[42]);
      v25 = v5;
      if ( v5 >= *v6 )
        goto LABEL_49;
    }
  }
  return result;
}

// ===== sub_44B970 @ 0x0044B970..0x0044B9D9 =====
BOOL __thiscall sub_44B970(_DWORD *this, int a2, int a3)
{
  int v3; // edx
  BOOL v5; // esi
  int v6; // ecx
  int v7; // edx
  int v8; // edx
  int v9; // edi

  v3 = a2 >> 16;
  v5 = 1;
  if ( a2 >> 16 < 0
    || v3 >= this[41]
    || (a2 & 0x8000u) != 0
    || (v6 = this[42], v7 = v3 << 6, (__int16)a2 >= *(_DWORD *)(v6 + v7))
    || (v8 = *(_DWORD *)(v6 + v7 + 8), v9 = 196 * (__int16)a2, v5 = (*(_DWORD *)(v8 + v9 + 192) & 4) == 0) )
  {
    sub_44A1D0((int)this, 268435458, a2, a3);
  }
  return v5;
}

// ===== sub_44B9E0 @ 0x0044B9E0..0x0044BA3A =====
_DWORD *__thiscall sub_44B9E0(int this)
{
  unsigned __int16 v2; // cx
  int v3; // eax
  _DWORD v5[2]; // [esp+4h] [ebp-10h] BYREF
  int v6; // [esp+Ch] [ebp-8h] BYREF
  int v7; // [esp+10h] [ebp-4h]

  sub_48E590(0, &v6);
  if ( *(_DWORD *)(this + 64) )
  {
    v3 = v7;
    v2 = v6;
  }
  else
  {
    (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(this + 40) + 48))(*(_DWORD *)(this + 40), v5);
    v2 = v6 - LOWORD(v5[0]);
    v3 = v7 - v5[1];
  }
  return sub_44A1D0(this, 268435463, -1, v2 | (v3 << 16));
}

// ===== sub_44BA40 @ 0x0044BA40..0x0044BB6D =====
int __thiscall sub_44BA40(_DWORD *this, _DWORD *a2, int a3, int a4, int a5, int a6)
{
  int result; // eax
  _DWORD *v7; // edx
  _DWORD *v8; // ecx
  _DWORD *v9; // esi
  int v10; // edi
  int v11; // eax
  bool v12; // zf
  int v13; // edx
  _DWORD *v14; // ebx
  int v15; // eax
  int v16; // esi
  int v17; // [esp+10h] [ebp+Ch]
  BOOL v18; // [esp+14h] [ebp+10h]

  result = 0;
  if ( a3 < this[41] )
  {
    v7 = (_DWORD *)(this[42] + (a3 << 6));
    if ( a4 < *v7 )
    {
      v8 = (_DWORD *)(*(_DWORD *)(this[51] + 4 * a3) + 48 * a4);
      if ( *v8 )
      {
        v9 = (_DWORD *)v8[1];
        *a2 = v9[8];
        v10 = *(_DWORD *)(v8[1] + 192) & 1;
        v11 = (*(_DWORD *)(v8[1] + 192) >> 3) & 1;
        v18 = a6 && a4 == v7[3];
        if ( !a5 )
        {
          if ( v18 )
          {
            v12 = v10 == 0;
          }
          else
          {
            if ( v10 )
            {
LABEL_14:
              v17 = 0;
              goto LABEL_16;
            }
            v12 = v11 == 0;
          }
          if ( !v12 )
            goto LABEL_14;
        }
        v17 = 1;
LABEL_16:
        if ( !a5 )
        {
LABEL_22:
          v14 = a2;
          goto LABEL_23;
        }
        v13 = v9[9];
        if ( v13 != -1 )
        {
          v14 = a2;
          *a2 = v13;
LABEL_23:
          if ( v18 )
          {
            v15 = v9[10];
            if ( v15 == -1 )
              v10 = 0;
            else
              *v14 = v15;
            if ( a5 )
            {
              v16 = v9[11];
              if ( v16 == -1 )
              {
                if ( !v17 )
                  return 1;
                if ( v10 )
                  return 1;
                goto LABEL_34;
              }
              *v14 = v16;
            }
          }
          if ( !v17 )
            return 1;
LABEL_34:
          *v14 += v8[4];
          return 1;
        }
        if ( v18 )
        {
          if ( v10 )
          {
LABEL_21:
            v17 = 0;
            goto LABEL_22;
          }
        }
        else if ( v10 || v11 )
        {
          goto LABEL_21;
        }
        v17 = 1;
        goto LABEL_22;
      }
    }
  }
  return result;
}

// ===== sub_44BB70 @ 0x0044BB70..0x0044BEBA =====
int __thiscall sub_44BB70(int *this, int a2, int a3)
{
  int v3; // edi
  int i; // esi
  unsigned int v7; // eax
  int v8; // eax
  int v9; // ecx
  int v10; // eax
  int v11; // edi
  int v12; // eax
  _DWORD *v13; // esi
  int v14; // ecx
  unsigned int v15; // eax
  int v16; // ecx
  int v17; // eax
  int v18; // edi
  int v19; // esi
  int v20; // eax
  int v21; // esi
  unsigned int v22; // eax
  int v23; // edi
  unsigned int v24; // eax
  _DWORD *v25; // [esp+28h] [ebp-50h]
  int j; // [esp+28h] [ebp-50h]
  int v27; // [esp+2Ch] [ebp-4Ch]
  int v28; // [esp+2Ch] [ebp-4Ch]
  int v29; // [esp+2Ch] [ebp-4Ch]
  int v30; // [esp+30h] [ebp-48h]
  int v31; // [esp+34h] [ebp-44h] BYREF
  int v32; // [esp+38h] [ebp-40h]
  _DWORD v33[4]; // [esp+3Ch] [ebp-3Ch] BYREF
  _DWORD v34[6]; // [esp+4Ch] [ebp-2Ch] BYREF
  int v35[4]; // [esp+64h] [ebp-14h] BYREF

  v3 = a2;
  v32 = a2;
  if ( this[29] == a2 && !a3 )
    return 0;
  sub_42C2A0(v33, (_DWORD *)this[10]);
  for ( i = 0; i < 2; ++i )
  {
    if ( sub_42C330(i + 2, this[10], v35) )
    {
      v7 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
      sub_443240(v35, v7, dword_565D6C);
      sub_4478D0((int)this);
    }
  }
  v8 = this[29];
  if ( v8 != -1 )
  {
    v9 = this[23];
    v10 = 5 * v8;
    v11 = *(_DWORD *)(v9 + 4 * v10 + 8);
    v12 = *(_DWORD *)(v9 + 4 * v10 + 4);
    v13 = (_DWORD *)(*(_DWORD *)(this[51] + 4 * v12) + 48 * v11);
    v27 = v12;
    v14 = *(_DWORD *)(v13[1] + 192);
    if ( (v14 & 1) != 0 || (v14 & 8) != 0 && v11 != *(_DWORD *)(this[42] + (v12 << 6) + 12) )
    {
      v13[4] = 0;
      v13[5] = 0;
    }
    if ( (*(_BYTE *)(this[42] + (v12 << 6) + 60) & 1) != 0 )
    {
      sub_42C170(v13[2], this[10], v13[3]);
      v12 = v27;
    }
    (*(void (__thiscall **)(int *, int *, int, int, _DWORD, int))(*this + 32))(this, &v31, v12, v11, 0, 1);
    sub_42C090(v13[2], this[10], v31);
    sub_42C1C0(v13[2], v35, (_DWORD *)this[10]);
    v15 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
    sub_443240(v35, v15, dword_565D6C);
    sub_4478D0((int)this);
    v3 = v32;
  }
  if ( v3 == -1 )
  {
    for ( j = 0; j < 2; ++j )
      sub_42BAB0(this[10], j + 2, 0);
    sub_4478D0((int)this);
  }
  else
  {
    v16 = this[23];
    v17 = 20 * v3;
    v18 = *(_DWORD *)(20 * v3 + v16 + 4);
    v19 = *(_DWORD *)(v17 + v16 + 8);
    v28 = v17;
    if ( (*(_BYTE *)(this[42] + (v18 << 6) + 60) & 1) != 0 )
    {
      v20 = *(_DWORD *)(this[51] + 4 * v18);
      sub_42C170(*(_DWORD *)(v20 + 48 * v19 + 8), this[10], *(_DWORD *)(v20 + 48 * v19 + 12) + 256);
    }
    (*(void (__thiscall **)(int *, int *, int, int, int, int))(*this + 32))(this, &v31, v18, v19, 1, 1);
    v21 = 6 * v19;
    sub_42C090(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * v18) + 8 * v21 + 8), this[10], v31);
    sub_42C1C0(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * v18) + 8 * v21 + 8), v35, (_DWORD *)this[10]);
    v22 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
    sub_443240(v35, v22, dword_565D6C);
    sub_4478D0((int)this);
    v23 = 2;
    v30 = 2;
    v25 = (_DWORD *)(*(_DWORD *)(v28 + this[23] + 12) + 32);
    v29 = 2;
    while ( 1 )
    {
      if ( sub_407F20(dword_565D70, v25[1], v34) )
      {
        sub_42BB30(v23, (int)v34, this[10]);
        sub_42BAE0(this[10], v30, v33[0] + *(v25 - 1), v33[1] + *v25, 0);
        sub_42BAB0(this[10], v30, 1);
        sub_42C330(v30, this[10], v35);
        v24 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
        sub_443240(v35, v24, dword_565D6C);
      }
      else
      {
        sub_42BAB0(this[10], v23, 0);
      }
      sub_4478D0((int)this);
      v25 += 3;
      ++v30;
      if ( !--v29 )
        break;
      v23 = v30;
    }
  }
  this[29] = v32;
  return 1;
}

// ===== sub_44BEC0 @ 0x0044BEC0..0x0044C10D =====
int __thiscall sub_44BEC0(int *this, int a2, int a3)
{
  int v3; // esi
  int v4; // edi
  int result; // eax
  int *v7; // ecx
  int v8; // edx
  bool v9; // zf
  int v10; // eax
  int v11; // edx
  int v12; // eax
  _DWORD *v13; // eax
  BOOL v14; // eax
  _DWORD *v15; // esi
  unsigned int v16; // eax
  BOOL v17; // eax
  int v18; // edi
  unsigned int v19; // eax
  int v20; // [esp+28h] [ebp-34h] BYREF
  int v21; // [esp+2Ch] [ebp-30h]
  int v22; // [esp+30h] [ebp-2Ch]
  int v23; // [esp+34h] [ebp-28h]
  int v24; // [esp+38h] [ebp-24h]
  _DWORD *v25; // [esp+3Ch] [ebp-20h]
  int v26; // [esp+40h] [ebp-1Ch]
  int v27[4]; // [esp+48h] [ebp-14h] BYREF

  v3 = a2;
  v4 = a3;
  result = 0;
  v24 = a2;
  v26 = a3;
  if ( a2 < 0 || a2 >= this[13] )
    return result;
  v7 = (int *)(this[14] + 52 * a2);
  v8 = v7[2];
  v23 = this[14] + 52 * a2;
  v20 = v8;
  if ( a3 >= 0 && a3 < *v7 )
  {
    if ( !*(_DWORD *)(196 * a3 + *(_DWORD *)(this[42] + (a2 << 6) + 8)) )
      return 0;
    v7 = (int *)v23;
    v8 = v20;
  }
  else if ( a3 != -1 )
  {
    return result;
  }
  if ( a3 == v8 )
    return 0;
  v9 = this[29] == -1;
  v21 = -1;
  v22 = -1;
  if ( !v9 )
  {
    v10 = this[23] + 20 * this[29];
    v11 = *(_DWORD *)(v10 + 4);
    v12 = *(_DWORD *)(v10 + 8);
    v21 = v11;
    v8 = v20;
    v22 = v12;
  }
  if ( v8 != -1 )
  {
    v13 = (_DWORD *)(*(_DWORD *)(this[51] + 4 * a2) + 48 * v8);
    v9 = (*(_BYTE *)(v13[1] + 192) & 8) == 0;
    v25 = v13;
    if ( !v9 )
    {
      v13[4] = 0;
      v13[5] = 0;
    }
    v14 = a2 == v21 && v8 == v22;
    if ( (*(int (__thiscall **)(int *, int *, int, int, BOOL, _DWORD))(*this + 32))(this, &v20, a2, v8, v14, 0) )
    {
      v15 = v25;
      sub_42C090(v25[2], this[10], v20);
      sub_42C1C0(v15[2], v27, (_DWORD *)this[10]);
      v16 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
      sub_443240(v27, v16, dword_565D6C);
      sub_4478D0((int)this);
      v3 = v24;
      v4 = v26;
    }
    v7 = (int *)v23;
  }
  v7[2] = v4;
  *(_DWORD *)(this[42] + (v3 << 6) + 12) = v4;
  if ( v4 != -1 )
  {
    v17 = v3 == v21 && v4 == v22;
    (*(void (__thiscall **)(int *, int *, int, int, BOOL, int))(*this + 32))(this, &v20, v3, v4, v17, 1);
    v18 = 6 * v4;
    sub_42C090(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * v3) + 8 * v18 + 8), this[10], v20);
    sub_42C1C0(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * v3) + 8 * v18 + 8), v27, (_DWORD *)this[10]);
    v19 = (*(int (__thiscall **)(int))(*(_DWORD *)this[10] + 28))(this[10]);
    sub_443240(v27, v19, dword_565D6C);
    sub_449D60(this, v24);
    sub_4478D0((int)this);
  }
  return 1;
}
