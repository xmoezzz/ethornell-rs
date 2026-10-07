#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_438050 @ 0x00438050..0x00438056 =====
int __usercall sub_438050@<eax>(int result@<eax>)
{
  dword_507650 = result;
  return result;
}

// ===== sub_438060 @ 0x00438060..0x00438066 =====
int sub_438060()
{
  return dword_507650;
}

// ===== sub_438070 @ 0x00438070..0x0043810E =====
int __usercall sub_438070@<eax>(const char *Src@<ecx>, _BYTE *a2@<eax>)
{
  const char *v2; // esi
  char *v3; // eax
  char v4; // cl
  char *v5; // edi
  char *v6; // ebx
  size_t v7; // edi
  int result; // eax

  v2 = Src;
  if ( Src )
  {
    while ( 1 )
    {
      v3 = strstr(v2, "<");
      if ( !v3 )
        break;
      v4 = v3[1];
      if ( v4 >= 65 && v4 <= 90 || v4 >= 97 && v4 <= 122 || v4 == 47 )
      {
        v5 = v3;
        v6 = strstr(v2 + 2, ">");
        if ( !v6 )
          break;
        v7 = v5 - v2;
        memcpy_0(a2, v2, v7);
        a2 += v7;
        v2 = v6 + 1;
      }
      else
      {
        *a2++ = *v2++;
      }
    }
  }
  strcpy(a2, v2);
  return result;
}

// ===== sub_438110 @ 0x00438110..0x00438182 =====
__int64 __thiscall sub_438110(int this)
{
  __int64 v2; // rax
  __int64 v3; // kr00_8
  bool v4; // zf
  int v5; // eax
  __int64 v6; // rax
  bool v7; // cf
  __int64 v9; // rt0

  v2 = sub_431A40();
  v3 = v2 - *(_QWORD *)(this + 232);
  *(_QWORD *)(this + 232) = v2;
  v4 = sub_437600((_DWORD *)this) == 0;
  v5 = 0x10000;
  if ( v4 )
    v5 = dword_507654;
  v6 = (unsigned int)v5 * v3;
  v7 = __CFADD__((_DWORD)v6, *(_DWORD *)(this + 240));
  *(_DWORD *)(this + 240) += v6;
  LODWORD(v6) = *(_DWORD *)(this + 240);
  *(_DWORD *)(this + 244) += HIDWORD(v6) + v7;
  LODWORD(v9) = v6;
  HIDWORD(v9) = *(_DWORD *)(this + 244);
  return v9 >> 16;
}

// ===== sub_438190 @ 0x00438190..0x004381E4 =====
_DWORD *__fastcall sub_438190(int a1, int a2, _DWORD *a3)
{
  sub_4341D0(a1, a2, (int)a3);
  *a3 = &CProcDspMsgExVE::`vftable';
  return a3;
}

// ===== sub_4381F0 @ 0x004381F0..0x00438211 =====
void *__thiscall sub_4381F0(void *this, char a2)
{
  sub_438220();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_438220 @ 0x00438220..0x00438267 =====
int __thiscall sub_438220(_DWORD *this)
{
  *this = &CProcDspMsgExVE::`vftable';
  return sub_4342E0(this);
}

// ===== sub_438270 @ 0x00438270..0x0043838B =====
int __usercall sub_438270@<eax>(
        int a1@<eax>,
        _DWORD *a2,
        int *a3,
        _DWORD *a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        void *Src,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15,
        int a16)
{
  int result; // eax
  int v18; // edi
  int v19; // ebx
  int v20; // eax
  _BYTE v21[32]; // [esp+Ch] [ebp-A8h] BYREF
  int v22; // [esp+2Ch] [ebp-88h]
  _DWORD v23[18]; // [esp+40h] [ebp-74h] BYREF
  int v24[11]; // [esp+88h] [ebp-2Ch] BYREF

  result = sub_4092B0((int)v21, dword_565B70);
  v18 = result;
  if ( result )
  {
    memset(v23, 0, sizeof(v23));
    memset(v24, 0, 40);
    sub_434A40((int)v24);
    v19 = v22;
    v20 = v19 + sub_4097B0(v22, a13);
    sub_4383D0(v23, a8, a9, a6, a7, v20, a1, a11, a14, a16);
    if ( a9 )
      sub_438B10(v23, a15, a16, v24);
    sub_439120(v23, a6, a7, a11, a12);
    *a3 = sub_437940((int)v23, a2, a4);
    sub_437030(v23);
    sub_434810((int)v24);
    return v18;
  }
  return result;
}

// ===== sub_438390 @ 0x00438390..0x004383CD =====
int __stdcall sub_438390(
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
        int a13)
{
  return sub_4383D0(a1, a3, a4, a6, a7, a8, a9, a11, a12, a13);
}

// ===== sub_4383D0 @ 0x004383D0..0x00438ADF =====
int __fastcall sub_4383D0(
        _DWORD *a1,
        int a2,
        _DWORD *a3,
        char *a4,
        int a5,
        _DWORD *a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        _DWORD *a12)
{
  int result; // eax
  int v13; // edi
  int v14; // ebx
  int v15; // esi
  int v16; // eax
  char *v17; // ebx
  _DWORD *v18; // ecx
  int v19; // eax
  int v20; // edx
  int v21; // ecx
  _DWORD *v22; // edi
  int v23; // edx
  unsigned __int8 v24; // al
  _DWORD *v25; // eax
  int v26; // ecx
  _DWORD *v27; // ebx
  int v28; // edi
  int v29; // esi
  BOOL v30; // eax
  unsigned int v31; // kr00_4
  int v32; // edi
  _BYTE *v33; // eax
  char *v34; // ecx
  _BYTE *v35; // edx
  char v36; // al
  int v37; // eax
  bool v38; // zf
  int v39; // eax
  int v40; // edx
  int v41; // eax
  int v42; // edx
  int v43; // [esp+10h] [ebp-518h]
  int v44; // [esp+10h] [ebp-518h]
  int v45; // [esp+14h] [ebp-514h]
  int v46; // [esp+18h] [ebp-510h]
  char *v47; // [esp+1Ch] [ebp-50Ch]
  int v48; // [esp+20h] [ebp-508h]
  int v49; // [esp+24h] [ebp-504h]
  int v51; // [esp+30h] [ebp-4F8h]
  int v52; // [esp+30h] [ebp-4F8h]
  int v53; // [esp+34h] [ebp-4F4h]
  int v54; // [esp+38h] [ebp-4F0h] BYREF
  int v55; // [esp+3Ch] [ebp-4ECh]
  const char *v56; // [esp+40h] [ebp-4E8h]
  _DWORD *v57; // [esp+44h] [ebp-4E4h]
  int v58; // [esp+48h] [ebp-4E0h] BYREF
  char v59[4]; // [esp+4Ch] [ebp-4DCh] BYREF
  int v60; // [esp+50h] [ebp-4D8h]
  int v61; // [esp+54h] [ebp-4D4h]
  char *v62; // [esp+58h] [ebp-4D0h]
  int v63; // [esp+5Ch] [ebp-4CCh] BYREF
  int v64; // [esp+60h] [ebp-4C8h]
  int v65; // [esp+64h] [ebp-4C4h]
  int v66; // [esp+68h] [ebp-4C0h]
  int v67[6]; // [esp+6Ch] [ebp-4BCh] BYREF
  _DWORD v68[2]; // [esp+84h] [ebp-4A4h] BYREF
  int v69; // [esp+8Ch] [ebp-49Ch]
  int v70; // [esp+90h] [ebp-498h]
  int v71; // [esp+94h] [ebp-494h]
  char *v72; // [esp+98h] [ebp-490h]
  int v73; // [esp+9Ch] [ebp-48Ch]
  void *v74[6]; // [esp+A0h] [ebp-488h] BYREF
  void *v75[6]; // [esp+B8h] [ebp-470h] BYREF
  char v76[32]; // [esp+D0h] [ebp-458h] BYREF
  int v77; // [esp+F0h] [ebp-438h]
  void *v78; // [esp+100h] [ebp-428h]
  int v79[7]; // [esp+104h] [ebp-424h] BYREF
  char v80[1028]; // [esp+120h] [ebp-408h] BYREF

  v57 = a1;
  v73 = a2;
  v47 = a4;
  result = sub_4092B0((int)v76, dword_565B70);
  if ( result )
  {
    v43 = 0;
    if ( *a4 < 32 && (unsigned __int8)(*a4 - 4) <= 4u )
    {
      v43 = 1;
      v47 = a4 + 1;
    }
    v13 = v77;
    v66 = (v77 + 1) >> 1;
    if ( a5 )
      v65 = sub_4370A0(v77);
    else
      v65 = 0;
    v14 = v13 * a12[1] / 100;
    v60 = v14;
    if ( v14 <= 0 )
    {
      v60 = 1;
      v14 = 1;
    }
    v15 = v13 * a12[2] / 100;
    v61 = v15;
    if ( v15 <= 0 )
    {
      v61 = 1;
      v15 = 1;
    }
    v16 = *a12 != 0 ? v15 : 0;
    v71 = *a12 != 0 ? v14 : 0;
    v70 = v16;
    sub_409080(v74, 1);
    v49 = 0;
    v64 = 0;
    v58 = 0;
    if ( a10 && (v64 = v13, dword_565CF0) )
    {
      v17 = v47;
      if ( sub_437AB0() )
      {
        if ( !v43 && !sub_42EA10(&v63) )
          v13 = v66;
        v49 = dword_565BB0 + v13;
      }
      else if ( v43 )
      {
        v49 = dword_565BB0 + v13;
      }
    }
    else
    {
      v17 = v47;
    }
    v18 = a6;
    if ( a5 )
    {
      if ( a6[1] == *(_DWORD *)(a7 + 4) )
      {
        v19 = sub_4344D0();
        *(_DWORD *)(v21 + 4) = v20 + v19;
      }
      v49 += sub_4344D0();
    }
    v22 = v57;
    v23 = 0;
    *v57 = 1;
    v53 = 0;
    v44 = 0;
    v46 = 0;
    v55 = 0;
    if ( *v17 )
    {
      do
      {
        v67[0] = (int)v74[0];
        v67[1] = (int)v74[1];
        v67[2] = (int)v74[2];
        v67[3] = (int)v74[3];
        v67[4] = (int)v74[4];
        v67[5] = (int)v74[5];
        v24 = v17[v23];
        v56 = &v17[v23];
        if ( v24 >= 0x20u )
        {
          v25 = operator new(0x48u);
          v26 = v55;
          v27 = v25;
          v25[3] = dword_50763C;
          *v25 = 0;
          v25[1] = v26;
          v25[2] = 0;
          v25[14] = 0;
          v25[15] = 0;
          v25[17] = 0;
          v51 = sub_42EA10(&v54);
          sub_40A710(v67, 0, 0);
          sub_433060(v79, (int)v67, v54, v78, a11);
          sub_438F60(v68);
          if ( v68[0] )
            sub_439020();
          v27[16] = v69 != 0;
          sub_409080(v27 + 8, 1);
          sub_40A710(v27 + 8, 0, 0);
          if ( *a12 )
          {
            sub_409080(v75, 1);
            sub_40A710(v75, 0, 0);
            if ( a12[3] )
            {
              sub_433060(v79, (int)v75, v54, v78, a12[3]);
              if ( v68[0] )
                sub_439020();
            }
            else
            {
              sub_40A9E0((int)v75, (int)v67, 5u, 0x100u, 1);
            }
            sub_40A530((int *)v75, v27 + 8, v61, v60, 1, 256 - a12[4]);
            operator delete[](v75[0]);
          }
          v28 = 0;
          sub_40A530(v67, v27 + 8, 0, 0, 0, 0);
          v29 = v77;
          v30 = v51 != 0;
          v45 = v77 + dword_565BB0;
          v52 = 0;
          v63 = v30 + 1;
          v72 = &v47[v30 + 1 + v46];
          v62 = v72;
          if ( a5 )
          {
            if ( v53 > 0 )
            {
              --v53;
            }
            else
            {
              if ( sub_4348C0(v73, v56, v80) )
              {
                v31 = strlen(v80);
                v32 = (v29 + dword_565BB0) * (v31 >> 1);
                if ( v45 < v32 )
                  v45 = (v29 + dword_565BB0) * (v31 >> 1);
                v33 = operator new[](v31 + 1);
                v27[14] = v33;
                v27[15] = v32;
                v34 = v80;
                v35 = v33;
                do
                {
                  v36 = *v34;
                  *v35++ = *v34++;
                }
                while ( v36 );
                v53 = sub_434B50(v80, 0) - 1;
                v52 = v53;
                v62 = &v47[strlen(v80) + v46];
              }
              v28 = 0;
            }
          }
          if ( a10 )
          {
            if ( v44 > 0 )
            {
              --v44;
            }
            else
            {
              v37 = sub_437A10(v80, v62);
              if ( v37 <= 0 )
              {
                if ( sub_437B70() && *v72 )
                {
                  v38 = !sub_42FA80(*v72);
                  v39 = v29;
                  if ( v38 )
                    v39 = v66;
                  v45 += v39;
                }
              }
              else
              {
                v45 += (v29 + dword_565BB0) * (strlen(v80) >> 1);
                v44 = v37 + v52;
                sub_437C00(v80, v59, v37 - 1);
                v48 = sub_437B10();
                if ( v48 )
                {
                  sub_42EA10(&v58);
                  v29 = v77;
                }
                v28 = v48;
              }
            }
          }
          if ( v45 + a6[1] > *(_DWORD *)(a7 + 12) - (v28 == 0 ? v64 : 0) + 1 )
          {
            if ( v54 == v58 )
            {
              v58 = 0;
            }
            else
            {
              *a6 -= a8;
              a6[1] = v49 + *(_DWORD *)(a7 + 4);
              ++*v57;
            }
          }
          v40 = v29 * v69;
          v27[4] = v29 * v68[1] / 100 - v65 - v29 + *a6 + 1;
          v22 = v57;
          v41 = a6[1] + v40 / 100;
          v42 = v29 + dword_565BB0;
          v27[5] = v41;
          a6[1] += v42;
          v55 += dword_507638;
          v46 += v63;
          v23 = v46;
          v18 = a6;
          a3[17] = v27;
          a3 = v27;
          v17 = v47;
        }
        else
        {
          if ( v24 == 10 )
          {
            *v18 -= a8;
            v18[1] = v49 + *(_DWORD *)(a7 + 4);
            ++*v22;
          }
          v46 = ++v23;
        }
      }
      while ( v17[v23] );
    }
    operator delete[](v74[0]);
    return 1;
  }
  return result;
}

// ===== sub_438AE0 @ 0x00438AE0..0x00438B02 =====
int __stdcall sub_438AE0(int a1, int a2, int a3, int a4, int a5)
{
  return sub_438B10(a1, a3, a4, a5);
}

// ===== sub_438B10 @ 0x00438B10..0x00438C09 =====
int __cdecl sub_438B10(int a1, int a2, int a3, int a4)
{
  int v4; // esi
  int result; // eax
  int v6; // eax
  int v7; // eax
  BOOL v8; // edi
  _DWORD *i; // esi
  const char *v10; // ecx
  int v11; // [esp+Ch] [ebp-9Ch] BYREF
  int v12; // [esp+10h] [ebp-98h]
  _DWORD v13[10]; // [esp+14h] [ebp-94h] BYREF
  CHAR v14[52]; // [esp+3Ch] [ebp-6Ch] BYREF
  CHAR pszFaceName[32]; // [esp+70h] [ebp-38h] BYREF
  int v16; // [esp+90h] [ebp-18h]
  int v17; // [esp+94h] [ebp-14h]
  int v18; // [esp+98h] [ebp-10h]

  v4 = dword_565B70;
  v12 = a1;
  result = sub_4092B0((int)pszFaceName, dword_565B70);
  if ( result )
  {
    v6 = sub_4370A0(v16);
    v7 = sub_409290(v17, v18, v4, (int)&v11, pszFaceName, v6);
    v8 = v7 == 0;
    if ( !v7 )
    {
      sub_4092B0((int)v14, dword_565B70);
      for ( i = *(_DWORD **)(v12 + 68); i; i = (_DWORD *)i[17] )
      {
        v10 = (const char *)i[14];
        if ( v10 )
        {
          sub_434840(a4, v10, v13);
          sub_438C10(i, v13, v14, a2, a3, v16 + i[4], i[5], i[1]);
        }
      }
    }
    return v8;
  }
  return result;
}

// ===== sub_438C10 @ 0x00438C10..0x00438EF6 =====
void __cdecl sub_438C10(int a1, int a2, int a3, int a4, _DWORD *a5, int a6, int a7, int a8)
{
  int v8; // ecx
  int v9; // ebx
  int v10; // edi
  int v11; // esi
  int v12; // edx
  int v13; // eax
  unsigned int v14; // esi
  int v15; // edi
  char *v16; // ebx
  char v17; // [esp+10h] [ebp-C8h] BYREF
  int v18; // [esp+54h] [ebp-84h]
  int v19[7]; // [esp+5Ch] [ebp-7Ch] BYREF
  int v20[3]; // [esp+78h] [ebp-60h] BYREF
  void *v21[6]; // [esp+84h] [ebp-54h] BYREF
  void *v22[2]; // [esp+9Ch] [ebp-3Ch] BYREF
  unsigned int v23; // [esp+B4h] [ebp-24h]
  int v24; // [esp+B8h] [ebp-20h]
  int v25; // [esp+BCh] [ebp-1Ch]
  int v26; // [esp+C0h] [ebp-18h]
  int v27; // [esp+C4h] [ebp-14h]
  char *v28; // [esp+C8h] [ebp-10h]
  int v29; // [esp+CCh] [ebp-Ch]
  int v30; // [esp+D0h] [ebp-8h]
  int v31; // [esp+D4h] [ebp-4h]
  int v32; // [esp+F8h] [ebp+20h]

  v9 = *(_DWORD *)(a3 + 32);
  v10 = v9 * a5[1] / 100;
  v26 = v10;
  if ( v10 <= 0 )
  {
    v26 = 1;
    v10 = 1;
  }
  v11 = v9 * a5[2] / 100;
  v24 = v11;
  if ( v11 <= 0 )
  {
    v24 = 1;
    v11 = 1;
  }
  v12 = *a5 != 0 ? v10 : 0;
  v13 = *a5 != 0 ? v11 : 0;
  v14 = *(_DWORD *)(a2 + 24);
  v30 = v13;
  v29 = v12;
  v25 = v8 / (int)v14;
  if ( v8 / (int)v14 < v9 )
    v25 = v9;
  v32 = ((int)(v8 - v25 * (v14 - 1) - v9) >> 1) + a7 + (v9 >> 3);
  v15 = a8 + ((dword_507638 * *(_DWORD *)(a2 + 8) / v14) >> 1);
  v23 = dword_507638 * *(_DWORD *)(a2 + 8) / v14;
  v27 = v15;
  sub_409080(v22, 1);
  v28 = &v17;
  v31 = 0;
  if ( *(int *)(a2 + 24) > 0 )
  {
    v30 += v9;
    v29 += v9;
    while ( 1 )
    {
      v16 = (char *)operator new(0x48u);
      *((_DWORD *)v16 + 3) = dword_50763C;
      *(_DWORD *)v16 = 0;
      *((_DWORD *)v16 + 2) = 0;
      *((_DWORD *)v16 + 4) = a6;
      *((_DWORD *)v16 + 14) = 0;
      *((_DWORD *)v16 + 15) = 0;
      *((_DWORD *)v16 + 17) = 0;
      *((_DWORD *)v16 + 1) = v15;
      *((_DWORD *)v16 + 5) = v32;
      *((_DWORD *)v16 + 16) = 2;
      sub_40A710(v22, 0, 0);
      sub_4092E0(*(void **)(a3 + 48), (BOOL **)v22, (int)v19, a4);
      sub_438F60(v20);
      if ( v20[0] )
        sub_439020();
      sub_409080((_DWORD *)v16 + 8, 1);
      sub_40A710((_DWORD *)v16 + 8, 0, 0);
      if ( *a5 )
      {
        sub_409080(v21, 1);
        sub_40A710(v21, 0, 0);
        if ( a5[3] )
          sub_4092E0(*(void **)(a3 + 48), (BOOL **)v21, (int)v19, a5[3]);
        else
          sub_40A9E0((int)v21, (int)v22, 5u, 0x100u, 1);
        sub_40A530((int *)v21, (_DWORD *)v16 + 8, v24, v26, 1, 256 - a5[4]);
        operator delete[](v21[0]);
      }
      sub_40A530((int *)v22, (_DWORD *)v16 + 8, 0, 0, 0, 0);
      v27 += v23;
      v32 += v25;
      *((_DWORD *)v28 + 17) = v16;
      v28 = v16;
      if ( ++v31 >= *(_DWORD *)(a2 + 24) )
        break;
      v15 = v27;
    }
  }
  *((_DWORD *)v28 + 17) = *(_DWORD *)(a1 + 68);
  *(_DWORD *)(a1 + 68) = v18;
  operator delete[](v22[0]);
}

// ===== sub_438F00 @ 0x00438F00..0x00438F5F =====
_DWORD *__thiscall sub_438F00(int *this, _DWORD *a2)
{
  int v3; // eax
  int v4; // eax
  int v5; // edx
  _DWORD *result; // eax
  _BYTE v7[32]; // [esp+4h] [ebp-34h] BYREF
  int v8; // [esp+24h] [ebp-14h]

  sub_42C450(this[8]);
  v3 = sub_4092B0((int)v7, dword_565B70);
  if ( this[49] && v3 )
    v4 = sub_4370A0(v8);
  else
    v4 = 0;
  v5 = v4 + *((_DWORD *)dword_565B80 + 2);
  result = a2;
  *a2 = -v5;
  a2[1] = 0;
  return result;
}

// ===== sub_438F60 @ 0x00438F60..0x00439020 =====
int __cdecl sub_438F60(_DWORD *a1)
{
  _BYTE *v1; // edi
  int v2; // eax
  _BYTE *v3; // edi
  int i; // ebx
  int v5; // eax
  int v7; // [esp+Ch] [ebp-8h] BYREF
  int v8; // [esp+10h] [ebp-4h] BYREF

  sub_42EA10(&v7);
  *a1 = 0;
  a1[1] = 0;
  a1[2] = 0;
  v1 = &unk_4E53F8;
  do
  {
    v2 = sub_42EA10(&v8);
    if ( v7 == v8 )
    {
      *a1 = 1;
      return 1;
    }
    v1 += (v2 != 0) + 1;
  }
  while ( *v1 );
  v3 = &unk_4E5444;
  for ( i = 0; ; ++i )
  {
    v5 = sub_42EA10(&v8);
    if ( v7 == v8 )
      break;
    v3 += (v5 != 0) + 1;
    if ( !*v3 )
      return 0;
  }
  a1[1] = i >= 4 ? 20 : 67;
  a1[2] = i >= 4 ? -20 : -67;
  return 1;
}

// ===== sub_439020 @ 0x00439020..0x004390E4 =====
BOOL __usercall sub_439020@<eax>(_DWORD *a1@<edi>)
{
  int v1; // eax
  int v2; // edx
  BOOL v3; // ebx
  _DWORD *v4; // eax
  char *v5; // esi
  bool v6; // zf
  unsigned int v7; // edx
  _DWORD *v8; // ecx
  int v9; // ebx
  void *v11[5]; // [esp+Ch] [ebp-24h] BYREF
  int v12; // [esp+20h] [ebp-10h]
  BOOL v13; // [esp+24h] [ebp-Ch]
  unsigned int v14; // [esp+28h] [ebp-8h]
  _DWORD *v15; // [esp+2Ch] [ebp-4h]

  v1 = a1[2];
  v2 = a1[3];
  v3 = v1 == v2;
  v13 = v3;
  if ( v1 == v2 )
  {
    sub_409030(a1[4], v2, v11, v1);
    v4 = (_DWORD *)*a1;
    v5 = (char *)v11[0] + v12 * ((int)v11[2] - 1);
    v6 = a1[3] == 0;
    v15 = (_DWORD *)*a1;
    v14 = 0;
    if ( !v6 )
    {
      do
      {
        v7 = 0;
        v8 = v5;
        if ( a1[2] )
        {
          do
          {
            v9 = a1[5];
            if ( v9 == 4 )
            {
              *v8 = *v4;
            }
            else if ( v9 == 2 )
            {
              *(_WORD *)v8 = *(_WORD *)v4;
            }
            else
            {
              *(_BYTE *)v8 = *(_BYTE *)v4;
            }
            v8 = (_DWORD *)((char *)v8 + (unsigned int)v11[1]);
            v4 = (_DWORD *)((char *)v4 + a1[5]);
            ++v7;
          }
          while ( v7 < a1[2] );
          v4 = v15;
        }
        v4 = (_DWORD *)((char *)v4 + a1[1]);
        v5 -= v12;
        v15 = v4;
        ++v14;
      }
      while ( v14 < a1[3] );
      v3 = v13;
    }
    sub_40ADF0((int)a1, (char **)v11);
    operator delete[](v11[0]);
  }
  return v3;
}

// ===== sub_4390F0 @ 0x004390F0..0x00439116 =====
int __stdcall sub_4390F0(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  return sub_439120(a1, a2, a3, a5, a7);
}

// ===== sub_439120 @ 0x00439120..0x00439277 =====
void __cdecl sub_439120(int a1, int a2, int a3, int a4, int a5)
{
  _DWORD *v5; // edi
  _DWORD *v6; // esi
  int v7; // eax
  _DWORD *v8; // eax
  int v9; // eax
  int v10; // edi
  _DWORD *v11; // edx
  _DWORD *i; // ecx
  int v13; // esi
  int v14; // eax
  int v15; // ecx
  void *v16; // esi
  void *v17; // [esp-4h] [ebp-60h]
  _BYTE v18[32]; // [esp+10h] [ebp-4Ch] BYREF
  int v19; // [esp+30h] [ebp-2Ch]
  _DWORD v20[3]; // [esp+44h] [ebp-18h] BYREF
  void *v21; // [esp+50h] [ebp-Ch]
  int v22; // [esp+54h] [ebp-8h]

  if ( a5 )
  {
    v22 = 0;
    if ( sub_4092B0((int)v18, dword_565B70) )
      v22 = v19;
    v5 = *(_DWORD **)(a1 + 68);
    v20[0] = 0;
    v20[1] = 0;
    v21 = 0;
    v20[2] = 0x80000000;
    v6 = v20;
    if ( v5 )
    {
      while ( 1 )
      {
        v7 = v5[16];
        if ( !v7 )
          break;
        if ( v7 == 1 )
          goto LABEL_10;
LABEL_11:
        v5 = (_DWORD *)v5[17];
        if ( !v5 )
          goto LABEL_12;
      }
      if ( v6[2] != v5[4] )
      {
        v8 = operator new(0x10u);
        v6[3] = v8;
        v6 = v8;
        *v8 = v5[5];
        v8[1] = v5[5];
        v8[2] = v5[4];
        v8[3] = 0;
      }
LABEL_10:
      v6[1] += v22;
      goto LABEL_11;
    }
LABEL_12:
    v9 = 0;
    if ( dword_565B80 )
      v10 = *((_DWORD *)dword_565B80 + 3);
    else
      v10 = 0;
    v11 = *(_DWORD **)(a1 + 68);
    for ( i = v20; v11; v11 = (_DWORD *)v11[17] )
    {
      if ( !v11[16] && i[2] != v11[4] )
      {
        i = (_DWORD *)i[3];
        if ( a5 == 1 )
        {
          if ( a4 )
            v13 = i[1] >= *(_DWORD *)(a3 + 12) ? 0 : v22;
          else
            v13 = 0;
          v14 = sub_4344D0();
          v9 = (*(_DWORD *)(a3 + 12) - i[1] - v13 - v14) >> 1;
        }
        else if ( a5 == 2 )
        {
          v9 = *(_DWORD *)(a3 + 12) - i[1] - v10;
        }
      }
      v11[5] += v9;
    }
    v15 = *(_DWORD *)(a2 + 4);
    if ( *(_DWORD *)(a3 + 4) < v15 )
      *(_DWORD *)(a2 + 4) = v9 + v15;
    v16 = v21;
    while ( v16 )
    {
      v17 = v16;
      v16 = (void *)*((_DWORD *)v16 + 3);
      operator delete[](v17);
    }
  }
}

// ===== sub_439280 @ 0x00439280..0x00439301 =====
_DWORD *__stdcall sub_439280(_DWORD *a1, int a2, int a3)
{
  int v3; // edx
  int v4; // eax

  sub_43D0D0(a1);
  *a1 = &CProcEncodeData::`vftable';
  v4 = sub_498550(v3, a2, a3);
  a1[8] = v4;
  if ( !v4 )
    sub_4646F0(a1[1]);
  sub_43D1A0();
  return a1;
}

// ===== sub_439310 @ 0x00439310..0x00439331 =====
void *__thiscall sub_439310(void *this, char a2)
{
  sub_439340();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_439340 @ 0x00439340..0x004393A5 =====
int __thiscall sub_439340(void **this)
{
  *this = &CProcEncodeData::`vftable';
  operator delete(this[8]);
  sub_43D1B0();
  return sub_43D150();
}

// ===== sub_4393B0 @ 0x004393B0..0x00439435 =====
_DWORD *__stdcall sub_4393B0(_DWORD *a1, int a2, int a3, int a4)
{
  int v4; // edx
  int v5; // eax

  sub_43D0D0(a1);
  *a1 = &CProcEncodeStruct::`vftable';
  v5 = sub_496000(a2, v4, a3, a4);
  a1[8] = v5;
  if ( !v5 )
    sub_4646F0(a1[1]);
  sub_43D1A0();
  return a1;
}

// ===== sub_439440 @ 0x00439440..0x00439461 =====
void *__thiscall sub_439440(void *this, char a2)
{
  sub_439470();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_439470 @ 0x00439470..0x004394D5 =====
int __thiscall sub_439470(void **this)
{
  *this = &CProcEncodeStruct::`vftable';
  operator delete(this[8]);
  sub_43D1B0();
  return sub_43D150();
}

// ===== sub_4394E0 @ 0x004394E0..0x0043950C =====
int __thiscall sub_4394E0(_DWORD *this)
{
  sub_431AF0((int)this, (int)this);
  if ( !*(_DWORD *)(this[8] + 4) )
    return 0;
  sub_4450D0();
  return 1;
}

// ===== sub_439510 @ 0x00439510..0x0043959F =====
int __userpurge sub_439510@<eax>(const char *a1@<edi>, int a2, int a3, unsigned int a4)
{
  int v4; // ebx
  int v5; // eax

  v4 = dword_566770;
  sub_4318F0((_DWORD *)a2, a3);
  *(_DWORD *)a2 = &CProcExclusion::`vftable';
  *(_DWORD *)(a2 + 36) = v4;
  strcpy((char *)(a2 + 40), a1);
  v5 = sub_42D560(a3);
  *(_DWORD *)(a2 + 32) = sub_42D2C0(v4, a1, v5, a4);
  return a2;
}

// ===== sub_4395A0 @ 0x004395A0..0x004395C2 =====
void *__thiscall sub_4395A0(void *this, char a2)
{
  sub_4395D0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_4395D0 @ 0x004395D0..0x00439618 =====
int __stdcall sub_4395D0(_DWORD *a1)
{
  *a1 = &CProcExclusion::`vftable';
  return sub_431950(a1);
}

// ===== sub_439620 @ 0x00439620..0x0043968B =====
int __thiscall sub_439620(int this)
{
  int v2; // eax

  sub_431AF0(this, this);
  if ( *(_DWORD *)(this + 32) )
  {
    if ( sub_4319C0(this) )
    {
      v2 = sub_42D560(*(_DWORD *)(this + 4));
      if ( sub_42D2F0(*(_DWORD *)(this + 36), (const char *)(this + 40), v2) )
        return 0;
    }
  }
  sub_4450D0();
  return 1;
}

// ===== sub_439690 @ 0x00439690..0x004396FA =====
_DWORD *__stdcall sub_439690(_DWORD *a1, int a2, int a3)
{
  int v3; // edx

  sub_43D0D0(a1);
  a1[400] = 0;
  *a1 = &CProcLoad::`vftable';
  sub_439AE0(a2, v3, 0, 0, a3, 0, 0);
  return a1;
}

// ===== sub_439700 @ 0x00439700..0x00439721 =====
void *__thiscall sub_439700(void *this, char a2)
{
  sub_439850();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_439730 @ 0x00439730..0x004397DB =====
_DWORD *__userpurge sub_439730@<eax>(int a1@<edi>, _DWORD *a2, int a3, int a4)
{
  int v4; // edx
  int v5; // ebx
  int v6; // eax

  sub_43D0D0(a2);
  v5 = 0;
  *a2 = &CProcLoad::`vftable';
  a2[400] = 1;
  if ( a1 || v4 )
  {
    v6 = v4;
  }
  else
  {
    v5 = 1;
    v6 = sub_468310(a3);
  }
  sub_439AE0(a3, a4, a1, v6, 1, v5, 0);
  return a2;
}

// ===== sub_4397E0 @ 0x004397E0..0x0043984C =====
_DWORD *__stdcall sub_4397E0(_DWORD *a1, int a2, int a3)
{
  sub_43D0D0(a1);
  a1[400] = 0;
  *a1 = &CProcLoad::`vftable';
  sub_439AE0(a2, a3, 0, 0, 0, 1, 1);
  return a1;
}

// ===== sub_439850 @ 0x00439850..0x004398B5 =====
int __thiscall sub_439850(void **this)
{
  *this = &CProcLoad::`vftable';
  operator delete[](this[8]);
  sub_43D1B0();
  return sub_43D150();
}

// ===== sub_4398C0 @ 0x004398C0..0x0043992A =====
int __usercall sub_4398C0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = sub_439970();
  if ( a1 )
  {
    if ( operator new(0x28u) )
      result = sub_445EB0(a1);
    else
      result = 0;
    dword_565D3C = result;
  }
  return result;
}

// ===== sub_439930 @ 0x00439930..0x0043994E =====
int __fastcall sub_439930(void *Src, int a2, int a3, size_t Size)
{
  int result; // eax

  result = 0;
  if ( dword_565D3C )
    return sub_445F40(a3, a2, Src, Size);
  return result;
}

// ===== sub_439950 @ 0x00439950..0x0043996E =====
int __fastcall sub_439950(int a1, int a2, void *a3, int a4)
{
  int result; // eax

  result = 0;
  if ( dword_565D3C )
    return sub_446060(a3, a2, a1, a4);
  return result;
}

// ===== sub_439970 @ 0x00439970..0x0043998D =====
int sub_439970()
{
  int result; // eax

  if ( dword_565D3C )
    result = (**(int (__thiscall ***)(int, int))dword_565D3C)(dword_565D3C, 1);
  dword_565D3C = 0;
  return result;
}

// ===== sub_439990 @ 0x00439990..0x00439ADB =====
int __thiscall sub_439990(void *this)
{
  int v2; // eax
  int v3; // eax
  int result; // eax
  BOOL v5; // eax
  _DWORD v6[6]; // [esp+0h] [ebp-24h] BYREF
  int v7; // [esp+20h] [ebp-4h]

  v6[5] = v6;
  if ( !*((_DWORD *)this + 8) )
  {
    v5 = *((_DWORD *)this + 9) == 0;
    return 2 * v5 - 1;
  }
  sub_431AF0((int)this, (int)this);
  v2 = (*(int (__thiscall **)(void *))(*(_DWORD *)this + 28))(this);
  if ( v2 )
  {
    if ( v2 != 1 )
    {
      *((_DWORD *)this + 401) = v2;
      v5 = *((_DWORD *)this + 400) != 0;
      return 2 * v5 - 1;
    }
    return 0;
  }
  v3 = *((_DWORD *)this + 9);
  if ( !v3 )
    return 0;
  if ( v3 == -1 )
  {
    *((_DWORD *)this + 401) = 3;
    v5 = *((_DWORD *)this + 400) != 0;
    return 2 * v5 - 1;
  }
  v7 = 0;
  if ( *((_DWORD *)this + 402) )
  {
    if ( *((_DWORD *)this + 403) )
      sub_445F40(
        *((_BYTE *)this + 40) == 48 ? 0 : (unsigned int)this + 40,
        (int)this + 820,
        *((void **)this + 8),
        *((_DWORD *)this + 9));
  }
  result = (*(int (__thiscall **)(void *))(*(_DWORD *)this + 32))(this) != 0;
  v6[4] = result;
  v7 = -1;
  return result;
}

// ===== sub_439AE0 @ 0x00439AE0..0x00439C6D =====
int __userpurge sub_439AE0@<eax>(int a1@<esi>, const char *a2, const char *a3, int a4, int a5, int a6, int a7, int a8)
{
  int result; // eax
  int v9; // ecx
  char *v10; // edi
  BOOL v11; // eax
  bool v12; // zf
  int v13; // ecx

  if ( !a3 )
  {
    v13 = *(_DWORD *)(a1 + 4);
    *(_DWORD *)(a1 + 32) = 0;
    *(_DWORD *)(a1 + 36) = -1;
    *(_DWORD *)(a1 + 1604) = 1;
    sub_4646F0(v13);
  }
  result = sub_43D1A0();
  if ( a6 )
  {
    result = sub_4665C0(a2);
    if ( !result )
    {
      *(_DWORD *)(a1 + 32) = 0;
      *(_DWORD *)(a1 + 36) = 0;
      *(_DWORD *)(a1 + 1604) = 1;
      return result;
    }
  }
  if ( a4 && !a5 )
  {
    *(_DWORD *)(a1 + 32) = 0;
    *(_DWORD *)(a1 + 36) = 0;
    *(_DWORD *)(a1 + 1604) = 3;
    return result;
  }
  v10 = (char *)(a1 + 40);
  if ( a2 )
  {
    strcpy(v10, a2);
    sub_42EA80(v9, (_BYTE *)(a1 + 40));
  }
  else
  {
    *v10 = 0;
  }
  strcpy((char *)(a1 + 820), a3);
  sub_42EA80(v9, (_BYTE *)(a1 + 820));
  v11 = a7 && dword_565D3C;
  *(_DWORD *)(a1 + 1608) = v11;
  result = (int)operator new[](0x4000000u);
  v12 = *(_DWORD *)(a1 + 1608) == 0;
  *(_DWORD *)(a1 + 32) = result;
  *(_DWORD *)(a1 + 36) = 0;
  if ( v12 )
  {
    *(_DWORD *)(a1 + 1612) = 0;
  }
  else
  {
    result = sub_446060((void *)result, a1 + 36, *v10 == 48 ? 0 : (unsigned int)v10, a1 + 820) == 0;
    *(_DWORD *)(a1 + 1612) = result;
    if ( !result )
      return result;
  }
  if ( !a8 )
  {
    if ( a4 || a5 )
      return sub_497EA0(*(_DWORD *)(a1 + 32), *v10 == 48 ? 0 : v10, a1 + 820, a4, a5);
    else
      return sub_497EA0(*(_DWORD *)(a1 + 32), *v10 == 48 ? 0 : v10, a1 + 820, 0, 0);
  }
  return result;
}

// ===== sub_439C70 @ 0x00439C70..0x00439CD1 =====
_DWORD *__stdcall sub_439C70(_DWORD *a1, int a2, int a3)
{
  sub_450A80(a1, a3);
  *a1 = &CProcLoadBitmap::`vftable';
  a1[412] = a2;
  return a1;
}

// ===== sub_439CE0 @ 0x00439CE0..0x00439D02 =====
void *__thiscall sub_439CE0(void *this, char a2)
{
  sub_439D10(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_439D10 @ 0x00439D10..0x00439D59 =====
int __stdcall sub_439D10(_DWORD *a1)
{
  *a1 = &CProcLoadBitmap::`vftable';
  return sub_450B40(a1);
}

// ===== sub_439D60 @ 0x00439D60..0x00439ECB =====
int __thiscall sub_439D60(_DWORD *this)
{
  int v2; // ecx
  int v3; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  switch ( sub_402110(this[412], this[8]) )
  {
    case -2147483647:
      v3 = sub_402330(this[8], v2, this[412]) - 1;
      if ( !v3 )
      {
        sprintf(Buffer, &byte_4E55A8, this + 10, this + 205);
LABEL_6:
        sub_4646F0(this[1]);
      }
      if ( v3 == 1 )
        sub_4646F0(this[1]);
      return 1;
    case -2147483646:
      sprintf(Buffer, &byte_4E5628, this + 10, this + 205);
      sub_4646F0(this[1]);
    case -2147483645:
      sprintf(Buffer, &byte_4E5670, this + 10, this + 205);
      sub_4646F0(this[1]);
    case -2147483644:
      sprintf(Buffer, byte_4E56B8, this + 10, this + 205);
      goto LABEL_6;
    case -2147483643:
      sprintf(Buffer, &byte_4E5708, this + 10, this + 205);
      sub_4646F0(this[1]);
    case -2147483642:
      sprintf(Buffer, &byte_4E574C, this + 10, this + 205);
      sub_4646F0(this[1]);
    default:
      return 1;
  }
}

// ===== sub_439EF0 @ 0x00439EF0..0x00439F78 =====
int __stdcall sub_439EF0(int a1, int a2, int a3, int a4, double a5, double a6)
{
  sub_439690((_DWORD *)a1, a3, 0);
  *(double *)(a1 + 1624) = a5;
  *(double *)(a1 + 1632) = a6;
  *(_DWORD *)a1 = &CProcLoadSound::`vftable';
  *(_DWORD *)(a1 + 1616) = a2;
  *(_DWORD *)(a1 + 1620) = a4;
  *(_DWORD *)(a1 + 1640) = 0;
  return a1;
}

// ===== sub_439F80 @ 0x00439F80..0x00439FA2 =====
void *__thiscall sub_439F80(void *this, char a2)
{
  sub_439FB0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_439FB0 @ 0x00439FB0..0x00439FF8 =====
int __stdcall sub_439FB0(void **a1)
{
  *a1 = &CProcLoadSound::`vftable';
  return sub_439850(a1);
}

// ===== sub_43A000 @ 0x0043A000..0x0043A143 =====
int __thiscall sub_43A000(int this)
{
  int v2; // eax
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  if ( !*(_DWORD *)(this + 1640) )
  {
    sub_498100(
      *(_DWORD *)(this + 1616),
      *(_DWORD *)(this + 32),
      *(_DWORD *)(this + 1620),
      *(double *)(this + 1624),
      *(double *)(this + 1632));
    *(_DWORD *)(this + 1640) = 1;
    return 0;
  }
  v2 = *(_DWORD *)(this + 1644);
  switch ( v2 )
  {
    case -1:
      return 0;
    case -2147483647:
      sprintf(Buffer, &byte_4E57B4, this + 40, this + 820);
      sub_4646F0(*(_DWORD *)(this + 4));
    case -2147483646:
      sprintf(Buffer, &byte_4E57F0, this + 40, this + 820);
      sub_4646F0(*(_DWORD *)(this + 4));
    case -1879048193:
      sprintf(Buffer, &byte_4E5848, this + 40, this + 820);
      sub_4646F0(*(_DWORD *)(this + 4));
  }
  return 1;
}

// ===== sub_43A150 @ 0x0043A150..0x0043A275 =====
_DWORD *__thiscall sub_43A150(void *this, _DWORD *a2, int a3, int a4)
{
  int v4; // ecx
  int v5; // eax
  int v6; // ecx
  int v7; // eax
  int v8; // ecx
  int v9; // eax
  int v11; // [esp-4h] [ebp-20h]

  sub_4318F0(a2, (int)this);
  a2[8] = a3;
  a2[10] = a4;
  *a2 = &CProcSelectIcon::`vftable';
  a2[14] = 0;
  a2[335] = 0;
  a2[336] = 0;
  a2[401] = 0;
  a2[402] = -1;
  sub_43A820(0);
  sub_42B550(a2[8], v4);
  sub_42B540(a2[8], 1);
  sub_42CA00(a2[8]);
  sub_42BBB0(a2[8]);
  sub_499F40(a2[1], 512);
  v5 = a2[10];
  if ( v5 )
  {
    v6 = a2[8];
    if ( v5 == 3 )
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)v6 + 28))(v6);
      sub_46D6C0();
      v7 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)a2[8] + 28))(a2[8]);
      sub_46D720(v7);
      v8 = a2[8];
      a2[335] = 1;
      v11 = (*(int (__thiscall **)(int))(*(_DWORD *)v8 + 28))(v8);
      v9 = sub_4467B0();
      sub_46E500(v9, v11);
    }
    else
    {
      sub_46D6E0(v6);
    }
  }
  a2[404] = 0;
  a2[405] = 0;
  return a2;
}

// ===== sub_43A280 @ 0x0043A280..0x0043A2A2 =====
void *__thiscall sub_43A280(void *this, char a2)
{
  sub_43A2B0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43A2B0 @ 0x0043A2B0..0x0043A359 =====
int __thiscall sub_43A2B0(void *this, int a2)
{
  int v2; // eax

  *(_DWORD *)a2 = &CProcSelectIcon::`vftable';
  v2 = *(_DWORD *)(a2 + 40);
  if ( v2 )
  {
    if ( v2 == 3 )
    {
      (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a2 + 32) + 28))(*(_DWORD *)(a2 + 32));
      sub_46D7A0();
    }
    else
    {
      sub_46D800();
    }
  }
  if ( *(_DWORD *)(a2 + 1340) )
  {
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a2 + 32) + 28))(*(_DWORD *)(a2 + 32));
    sub_46D7B0();
  }
  sub_499FA0(this, 512);
  sub_43A390(a2);
  return sub_431950((_DWORD *)a2);
}

// ===== sub_43A360 @ 0x0043A360..0x0043A366 =====
int __usercall sub_43A360@<eax>(int result@<eax>)
{
  dword_565D40 = result;
  return result;
}

// ===== sub_43A370 @ 0x0043A370..0x0043A384 =====
int sub_43A370()
{
  int result; // eax

  result = 1;
  if ( dword_565D40 )
    return sub_49A230();
  return result;
}

// ===== sub_43A390 @ 0x0043A390..0x0043A3F3 =====
int __stdcall sub_43A390(int *a1)
{
  int result; // eax
  int *v3; // edi
  int v4; // [esp+Ch] [ebp+8h]

  result = 0;
  v4 = 0;
  if ( a1[401] > 0 )
  {
    v3 = a1 + 271;
    do
    {
      sub_46D800();
      sub_41AC40(a1[8], *v3);
      if ( *v3 )
        (**(void (__thiscall ***)(int, int))*v3)(*v3, 1);
      ++v3;
      ++v4;
    }
    while ( v4 < a1[401] );
    result = 0;
  }
  a1[401] = 0;
  a1[14] = 0;
  return result;
}

// ===== sub_43A400 @ 0x0043A400..0x0043A70F =====
int __stdcall sub_43A400(int *a1, int a2, _DWORD *a3, int a4)
{
  int v4; // eax
  int v5; // eax
  int *v6; // edi
  int v7; // esi
  _DWORD *v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v12; // ecx
  int v13; // esi
  int v14; // eax
  int v15; // eax
  int v16; // esi
  int v17; // eax
  int v19; // [esp-4h] [ebp-80h]
  int *v20; // [esp+10h] [ebp-6Ch]
  _DWORD *v22; // [esp+18h] [ebp-64h]
  int v23; // [esp+1Ch] [ebp-60h]
  int v24; // [esp+20h] [ebp-5Ch]
  void (__thiscall **v25)(int, int); // [esp+24h] [ebp-58h]
  int v26; // [esp+28h] [ebp-54h]
  int v27; // [esp+2Ch] [ebp-50h] BYREF
  int v28; // [esp+30h] [ebp-4Ch]
  _DWORD v29[4]; // [esp+34h] [ebp-48h] BYREF
  int v30[4]; // [esp+44h] [ebp-38h] BYREF
  int v31[7]; // [esp+54h] [ebp-28h] BYREF
  int v32; // [esp+78h] [ebp-4h]

  sub_43A390(a1);
  if ( (unsigned int)(a2 - 1) > 0x3F )
    return -2147483647;
  v4 = a1[8];
  a1[14] = a2;
  sub_42B9B0(v4);
  sub_42C2A0(v29, (_DWORD *)a1[8]);
  v5 = 1;
  v23 = 0;
  if ( a2 <= 0 )
  {
LABEL_15:
    v12 = a1[8];
    a1[9] = a1[271];
    (*(void (__thiscall **)(int))(*(_DWORD *)v12 + 48))(v12);
    (*(void (__thiscall **)(int, int, int))(*(_DWORD *)a1[8] + 44))(a1[8], v26, v27);
    v13 = a1[8];
    v14 = (*(int (__thiscall **)(int))(*(_DWORD *)v13 + 28))(v13);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v13 + 28))(v13, v14);
    sub_46DF00(&v27);
    v15 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[9] + 28))(a1[9]);
    sub_46DF00(v15);
    sub_42C2F0(v29, a1[8]);
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
    v16 = *a1;
    v17 = sub_43AC50(a1);
    (*(void (__thiscall **)(int *, int))(v16 + 28))(a1, v17);
    (*(void (__thiscall **)(int *))(*a1 + 16))(a1);
    return 0;
  }
  else
  {
    v6 = a1 + 271;
    v20 = a1 + 271;
    v22 = a1 + 15;
    while ( v5 )
    {
      v24 = sub_407F20(dword_565B70, a3[2], v31);
      if ( v24 )
      {
        *v22 = *a3;
        v22[1] = a3[1];
        v22[2] = a3[2];
        v22[3] = a3[3];
        v7 = v29[0] + *a3;
        v26 = v29[1] + a3[1];
        sub_42B5B0((_DWORD *)a1[8], v30, v7, v26, v31, 0, 0);
        v8 = operator new(0x134u);
        v32 = 0;
        if ( v8 )
          v9 = sub_42AC50(0, v8, a1[8]);
        else
          v9 = 0;
        v32 = -1;
        v19 = v31[3];
        *v6 = v9;
        sub_41AB10(v19);
        v25 = (void (__thiscall **)(int, int))(*(_DWORD *)*v6 + 4);
        v10 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[8] + 8))(a1[8]);
        (*v25)(*v6, v10);
        sub_41B310(&v27, a1[8]);
        (*(void (__thiscall **)(int, int, int))(*(_DWORD *)*v6 + 56))(*v6, v27, v28);
        sub_41B360(&v27, a1[8]);
        sub_41B320((_DWORD *)*v6, v27, v28);
        v11 = sub_42E9A0(a1[8]);
        sub_41ADF0(*v6, v11);
        sub_41AB40(*v6, (_DWORD *)a1[8], v7, v26);
        if ( a4 )
          sub_41BC70(*v20, (char *)v31);
        sub_46D6E0(*v20);
        ++a1[401];
        v6 = v20;
      }
      v22 += 4;
      a3 += 4;
      ++v6;
      ++v23;
      v20 = v6;
      if ( v23 >= a2 )
      {
        if ( v24 )
          goto LABEL_15;
        return -2147483646;
      }
      v5 = v24;
    }
    return -2147483646;
  }
}

// ===== sub_43A710 @ 0x0043A710..0x0043A7A5 =====
int __usercall sub_43A710@<eax>(int a1@<eax>, _DWORD *a2, int a3)
{
  _DWORD *v5; // edi
  int v7[6]; // [esp+10h] [ebp-38h] BYREF
  int v8[4]; // [esp+28h] [ebp-20h] BYREF
  _DWORD v9[4]; // [esp+38h] [ebp-10h] BYREF
  int v10; // [esp+50h] [ebp+8h]

  sub_42B9B0((int)a2);
  sub_42C2A0(v9, a2);
  if ( a1 > 0 )
  {
    v5 = (_DWORD *)(a3 + 4);
    v10 = a1;
    do
    {
      if ( sub_407F20(dword_565B70, v5[1], v7) )
        sub_42B5B0(a2, v8, v9[0] + *(v5 - 1), v9[1] + *v5, v7, 0, 0);
      v5 += 4;
      --v10;
    }
    while ( v10 );
  }
  sub_42C2F0(v8, (int)a2);
  (*(void (__thiscall **)(_DWORD *))(*a2 + 28))(a2);
  return sub_443240(dword_565B6C);
}

// ===== sub_43A7B0 @ 0x0043A7B0..0x0043A81D =====
void __userpurge sub_43A7B0(int a1@<edi>, int a2)
{
  int v2; // esi
  _DWORD *v3; // ebx
  int v4; // eax
  int v5; // [esp-8h] [ebp-Ch]

  v2 = 0;
  if ( *(int *)(a1 + 56) > 0 )
  {
    v3 = (_DWORD *)(a1 + 1348);
    do
    {
      v5 = *(_DWORD *)(a2 + 4 * v2);
      *v3 = v5;
      sub_46DB40(v5);
      ++v2;
      ++v3;
    }
    while ( v2 < *(_DWORD *)(a1 + 56) );
  }
  if ( !*(_DWORD *)(a1 + 1340) )
  {
    v4 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
    sub_46D720(v4);
    *(_DWORD *)(a1 + 1340) = 1;
  }
  *(_DWORD *)(a1 + 1344) = 1;
}

// ===== sub_43A820 @ 0x0043A820..0x0043A827 =====
int __usercall sub_43A820@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 1612) = a2;
  return result;
}

// ===== sub_43A830 @ 0x0043A830..0x0043A874 =====
int __thiscall sub_43A830(_DWORD *this, _DWORD *a2)
{
  int result; // eax

  result = (int)a2;
  if ( *a2 == 512 )
  {
    this[404] = 1;
    result = a2[1];
    if ( result < 0 || result >= this[14] )
    {
      this[402] = -1;
    }
    else
    {
      this[402] = result;
      this[12] = 0;
      this[13] = 0;
    }
  }
  return result;
}

// ===== sub_43A880 @ 0x0043A880..0x0043A9DE =====
int __thiscall sub_43A880(int this)
{
  int v2; // esi
  int v3; // esi
  int v4; // eax
  int v5; // eax
  bool v6; // zf
  int v7; // edi
  int v8; // esi
  int v9; // eax
  int v10; // eax
  int v12; // [esp-4h] [ebp-2Ch]
  int v13; // [esp+20h] [ebp-8h]

  sub_431AF0(this, this);
  v2 = 0;
  if ( *(_DWORD *)(this + 40) )
  {
    v3 = *(_DWORD *)(this + 32);
    v12 = (*(int (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3);
    (*(void (__thiscall **)(int))(*(_DWORD *)v3 + 28))(v3);
    v2 = sub_46DF00(v12) & 0x7FFFFFFF;
  }
  v4 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 36) + 28))(*(_DWORD *)(this + 36));
  v5 = v2 | sub_46DF00(v4) & 0x7FFFFFFF;
  v6 = *(_DWORD *)(this + 1612) == 0;
  *(_DWORD *)(this + 44) = v5;
  if ( v6 )
    *(_DWORD *)(this + 44) = v5 & 0xFFFFFDFD;
  sub_499FE0();
  v7 = *(_DWORD *)(this + 1616);
  if ( !v7 )
  {
    if ( *(_DWORD *)(this + 1344) )
      v7 = sub_43A9E0();
    v8 = *(_DWORD *)(this + 1620);
    v9 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
    v10 = sub_46E580(v9) && sub_43A370();
    *(_DWORD *)(this + 1620) = v10;
    if ( !v7 && (*(_DWORD *)(this + 44) || v13 || v10 != v8) )
      v7 = sub_43AD50();
  }
  sub_431AA0((_DWORD *)this);
  if ( !v7 && sub_4319C0(this) )
    return 0;
  sub_4450D0();
  sub_4450D0();
  sub_4319C0(this);
  sub_4450D0();
  return 1;
}

// ===== sub_43A9E0 @ 0x0043A9E0..0x0043AA5D =====
int __usercall sub_43A9E0@<eax>(int *a1@<esi>)
{
  int v1; // eax
  int v2; // edi
  _DWORD *i; // ebx
  int result; // eax
  int v5; // edx

  v1 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
  if ( !sub_46D810(v1) )
    return 0;
  v2 = 0;
  if ( a1[14] <= 0 )
    return 0;
  for ( i = a1 + 337; !sub_46DB40(*i); ++i )
  {
    result = sub_46E270(a1[11], *i);
    if ( result )
      break;
    if ( ++v2 >= a1[14] )
      return result;
  }
  (*(void (__thiscall **)(int *, int))(*a1 + 28))(a1, v2);
  v5 = *a1;
  a1[12] = 0;
  a1[13] = 0;
  (*(void (__thiscall **)(int *))(v5 + 16))(a1);
  return 1;
}

// ===== sub_43AA60 @ 0x0043AA60..0x0043AC42 =====
int __thiscall sub_43AA60(int this, int a2)
{
  int result; // eax
  int v4; // ecx
  int v5; // ebx
  int v6; // esi
  int v7; // ebx
  int v8; // [esp+Ch] [ebp-54h]
  _DWORD *v9; // [esp+Ch] [ebp-54h]
  int v10[4]; // [esp+10h] [ebp-50h] BYREF
  int v11; // [esp+20h] [ebp-40h] BYREF
  int v12; // [esp+24h] [ebp-3Ch]
  int v13[6]; // [esp+30h] [ebp-30h] BYREF
  int v14[6]; // [esp+48h] [ebp-18h] BYREF

  sub_42C2A0(&v11, *(_DWORD **)(this + 32));
  result = *(_DWORD *)(this + 1608);
  if ( result != -1 )
  {
    result = sub_407F20(dword_565B70, *(_DWORD *)(this + 16 * result + 72), v14);
    if ( result )
    {
      v4 = *(_DWORD *)(this + 1608);
      v5 = v11 + *(_DWORD *)(this + 16 * v4 + 60);
      v8 = v12 + *(_DWORD *)(this + 16 * (v4 + 4));
      sub_42B5B0(*(_DWORD **)(this + 32), v10, v5, v8, v14, 64, 1);
      (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
      sub_443240(dword_565B6C);
      result = sub_407F20(dword_565B70, *(_DWORD *)(this + 16 * *(_DWORD *)(this + 1608) + 68), v13);
      if ( result )
      {
        sub_42B5B0(*(_DWORD **)(this + 32), v10, v5, v8, v13, 0, 0);
        (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
        result = sub_443240(dword_565B6C);
      }
    }
  }
  if ( a2 != -1
    && (v9 = (_DWORD *)(this + 16 * a2), (result = sub_407F20(dword_565B70, v9[18], v14)) != 0)
    && (result = sub_407F20(dword_565B70, v9[17], v13)) != 0 )
  {
    v6 = v11 + v9[15];
    v7 = v12 + *(_DWORD *)(this + 16 * (a2 + 4));
    sub_42B5B0(*(_DWORD **)(this + 32), v10, v6, v7, v13, 64, 1);
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
    sub_443240(dword_565B6C);
    sub_42B5B0(*(_DWORD **)(this + 32), v10, v6, v7, v14, 0, 0);
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(this + 32) + 28))(*(_DWORD *)(this + 32));
    sub_443240(dword_565B6C);
    result = a2;
    *(_DWORD *)(this + 1608) = a2;
  }
  else
  {
    *(_DWORD *)(this + 1608) = a2;
  }
  return result;
}

// ===== sub_43AC50 @ 0x0043AC50..0x0043AD43 =====
int __stdcall sub_43AC50(_DWORD *a1)
{
  _DWORD *v1; // ebx
  bool v2; // zf
  bool v3; // cc
  int v4; // edi
  _DWORD *i; // esi
  int result; // eax
  int v7; // edi
  _DWORD *v8; // ecx
  int v9; // [esp+Ch] [ebp-28h] BYREF
  int v10; // [esp+10h] [ebp-24h]
  _DWORD *v11; // [esp+14h] [ebp-20h]
  int v12; // [esp+18h] [ebp-1Ch]
  int v13; // [esp+1Ch] [ebp-18h] BYREF
  int v14; // [esp+20h] [ebp-14h]
  int v15; // [esp+24h] [ebp-10h]
  int v16; // [esp+28h] [ebp-Ch]

  v1 = a1;
  v2 = a1[405] == 0;
  v11 = a1;
  if ( !v2 )
  {
    sub_48E680(&v9);
    v3 = a1[14] <= 0;
    v12 = 0;
    if ( !v3 )
    {
      v4 = v9;
      for ( i = a1 + 271; ; ++i )
      {
        (*(void (__thiscall **)(_DWORD, int *))(*(_DWORD *)*i + 36))(*i, &v13);
        if ( v13 <= v4 && v4 <= v15 && v14 <= v10 && v10 <= v16 )
        {
          if ( (*(int (__thiscall **)(_DWORD, int, int, int))(*(_DWORD *)*i + 100))(*i, v4 - v13, v10 - v14, 1) )
          {
            v7 = v4 - v13;
            v8 = v11;
            v11[13] = v10 - v14;
            result = v12;
            v8[12] = v7;
            return result;
          }
          v1 = v11;
        }
        if ( ++v12 >= v1[14] )
          return -1;
      }
    }
  }
  return -1;
}

// ===== sub_43AD50 @ 0x0043AD50..0x0043ADEA =====
int __usercall sub_43AD50@<eax>(int a1@<esi>)
{
  int result; // eax
  int v2; // eax
  int v3; // edi
  int v4; // eax
  int v5; // eax
  int v6; // [esp-4h] [ebp-8h]

  if ( (*(_DWORD *)(a1 + 44) & 0x202) != 0 )
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)a1 + 28))(a1, -1);
    (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
    return 1;
  }
  else
  {
    v2 = sub_43AC50((_DWORD *)a1);
    v3 = v2;
    if ( v2 != *(_DWORD *)(a1 + 1608) )
    {
      v4 = v2 != -1 && *(_DWORD *)(a1 + 16 * v2 + 72) != -1;
      v6 = (unsigned __int16)v3 | (v4 << 16);
      v5 = sub_42D560(*(_DWORD *)(a1 + 4));
      sub_496540(536870913, v5, v6);
      (*(void (__thiscall **)(int, int))(*(_DWORD *)a1 + 28))(a1, v3);
      (*(void (__thiscall **)(int))(*(_DWORD *)a1 + 16))(a1);
    }
    result = 1;
    if ( (*(_BYTE *)(a1 + 44) & 1) == 0 || *(_DWORD *)(a1 + 1608) == -1 )
      return 0;
  }
  return result;
}

// ===== sub_43ADF0 @ 0x0043ADF0..0x0043AE4C =====
_DWORD *__thiscall sub_43ADF0(void *this, _DWORD *a2, void *a3, int a4)
{
  sub_43A150(a3, a2, (int)this, a4);
  *a2 = &CProcSelectIconEx::`vftable';
  return a2;
}

// ===== sub_43AE50 @ 0x0043AE50..0x0043AE72 =====
void *__thiscall sub_43AE50(void *this, char a2)
{
  sub_43AE80(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43AE80 @ 0x0043AE80..0x0043AEC9 =====
int __thiscall sub_43AE80(void *this, _DWORD *a2)
{
  *a2 = &CProcSelectIconEx::`vftable';
  return sub_43A2B0(this, (int)a2);
}

// ===== sub_43AED0 @ 0x0043AED0..0x0043B230 =====
int __stdcall sub_43AED0(int *a1, int a2, int a3, int a4)
{
  int v4; // eax
  int v5; // eax
  int *v6; // edi
  int *v7; // eax
  int v8; // edi
  int v9; // esi
  _DWORD *v10; // eax
  int v11; // eax
  int v12; // ecx
  int v13; // eax
  int v14; // eax
  int *v15; // edx
  int *v16; // edx
  int v17; // ecx
  int v18; // ecx
  int v19; // esi
  int v20; // eax
  int v21; // eax
  int v22; // esi
  int v23; // eax
  int *v25; // [esp+10h] [ebp-6Ch]
  int *v26; // [esp+14h] [ebp-68h]
  _DWORD *v27; // [esp+18h] [ebp-64h]
  int *v28; // [esp+1Ch] [ebp-60h]
  int v29; // [esp+20h] [ebp-5Ch]
  int v30; // [esp+24h] [ebp-58h]
  void (__thiscall **v31)(int, int); // [esp+28h] [ebp-54h]
  int v32; // [esp+2Ch] [ebp-50h] BYREF
  int v33; // [esp+30h] [ebp-4Ch]
  _DWORD v34[4]; // [esp+34h] [ebp-48h] BYREF
  int v35[4]; // [esp+44h] [ebp-38h] BYREF
  int v36[7]; // [esp+54h] [ebp-28h] BYREF
  int v37; // [esp+78h] [ebp-4h]

  sub_43A390(a1);
  if ( (unsigned int)(a2 - 1) > 0x3F )
    return -2147483647;
  v4 = a1[8];
  a1[14] = a2;
  sub_42B9B0(v4);
  sub_42C2A0(v34, (_DWORD *)a1[8]);
  v5 = 1;
  v29 = 0;
  if ( a2 <= 0 )
  {
LABEL_17:
    v18 = a1[8];
    a1[9] = a1[271];
    (*(void (__thiscall **)(int))(*(_DWORD *)v18 + 48))(v18);
    (*(void (__thiscall **)(int, void (__thiscall **)(int, int), int))(*(_DWORD *)a1[8] + 44))(a1[8], v31, v32);
    v19 = a1[8];
    v20 = (*(int (__thiscall **)(int))(*(_DWORD *)v19 + 28))(v19);
    (*(void (__thiscall **)(int, int))(*(_DWORD *)v19 + 28))(v19, v20);
    sub_46DF00(&v32);
    v21 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[9] + 28))(a1[9]);
    sub_46DF00(v21);
    sub_42C2F0(v34, a1[8]);
    (*(void (__thiscall **)(int))(*(_DWORD *)a1[8] + 28))(a1[8]);
    sub_443240(dword_565B6C);
    v22 = *a1;
    v23 = sub_43AC50(a1);
    (*(void (__thiscall **)(int *, int))(v22 + 28))(a1, v23);
    (*(void (__thiscall **)(int *))(*a1 + 16))(a1);
    return 0;
  }
  else
  {
    v25 = a1 + 271;
    v6 = (int *)(a3 + 8);
    v28 = a1 + 406;
    v26 = (int *)(a3 + 8);
    v27 = a1 + 16;
    while ( v5 )
    {
      v30 = sub_407F20(dword_565B70, *v6, v36);
      if ( v30 )
      {
        v7 = v6 - 2;
        *(v27 - 1) = *(v6 - 2);
        *v27 = *(v6 - 1);
        v27[1] = *v6;
        v27[2] = *v6;
        qmemcpy(v28, v6 - 2, 0x40u);
        v8 = v34[1] + *(v26 - 1);
        v9 = v34[0] + *v7;
        sub_42B5B0((_DWORD *)a1[8], v35, v9, v8, v36, 0, 0);
        v10 = operator new(0x134u);
        v37 = 0;
        if ( v10 )
          v11 = sub_42AC50(0, v10, a1[8]);
        else
          v11 = 0;
        v37 = -1;
        v12 = v36[3];
        *v25 = v11;
        sub_41AB10(v12);
        v31 = (void (__thiscall **)(int, int))(*(_DWORD *)*v25 + 4);
        v13 = (*(int (__thiscall **)(int))(*(_DWORD *)a1[8] + 8))(a1[8]);
        (*v31)(*v25, v13);
        sub_41B310(&v32, a1[8]);
        (*(void (__thiscall **)(int, int, int))(*(_DWORD *)*v25 + 56))(*v25, v32, v33);
        sub_41B360(&v32, a1[8]);
        sub_41B320((_DWORD *)*v25, v32, v33);
        v14 = sub_42E9A0(a1[8]);
        sub_41ADF0(*v15, v14);
        sub_41AB40(*v16, (_DWORD *)a1[8], v9, v8);
        if ( a4 )
        {
          v17 = v26[13];
          if ( v17 == -1 || sub_407F20(dword_565B70, v17, v36) )
            sub_41BC70(*v25, (char *)v36);
        }
        sub_46D6E0(*v25);
        ++a1[401];
        v6 = v26;
      }
      ++v25;
      v27 += 4;
      v28 += 16;
      v6 += 16;
      ++v29;
      v26 = v6;
      if ( v29 >= a2 )
      {
        if ( v30 )
          goto LABEL_17;
        return -2147483646;
      }
      v5 = v30;
    }
    return -2147483646;
  }
}

// ===== sub_43B230 @ 0x0043B230..0x0043B37A =====
int __thiscall sub_43B230(int *this, int a2)
{
  int i; // esi
  int v4; // edi
  _DWORD *v5; // esi
  int result; // eax
  int v7; // [esp+10h] [ebp-40h]
  int *v8; // [esp+14h] [ebp-3Ch]
  _DWORD v9[4]; // [esp+18h] [ebp-38h] BYREF
  _DWORD v10[4]; // [esp+28h] [ebp-28h] BYREF
  _DWORD v11[6]; // [esp+38h] [ebp-18h] BYREF

  for ( i = 0; i < 8; ++i )
  {
    if ( sub_42C330(i, this[8], v9) )
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
      sub_443240(dword_565B6C);
    }
  }
  if ( a2 == -1 )
  {
    result = sub_42BBB0(this[8]);
    this[402] = -1;
  }
  else
  {
    v4 = 0;
    v7 = 0;
    v8 = &this[16 * a2 + 410];
    while ( 1 )
    {
      if ( sub_407F20(dword_565B70, v8[1], v11) )
      {
        v5 = (_DWORD *)this[8];
        sub_42C2A0(v10, v5);
        sub_42BB30(v4, (int)v11, (int)v5);
        sub_42BAE0(this[8], v7, v10[0] + *(v8 - 1), v10[1] + *v8, 0);
        sub_42BAB0(this[8], v7, 1);
        sub_42C330(v7, this[8], v9);
        (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
        sub_443240(dword_565B6C);
      }
      else
      {
        sub_42BAB0(this[8], v4, 0);
      }
      v8 += 3;
      if ( ++v7 >= 8 )
        break;
      v4 = v7;
    }
    result = a2;
    this[402] = a2;
  }
  return result;
}

// ===== sub_43B380 @ 0x0043B380..0x0043B478 =====
_DWORD *__thiscall sub_43B380(void *this, _DWORD *a2, int a3)
{
  int v4; // eax
  int v5; // ecx
  int v6; // esi
  int v7; // eax
  int v9; // [esp-4h] [ebp-20h]

  sub_4318F0(a2, (int)this);
  a2[8] = a3;
  *a2 = &CProcSelectItem::`vftable';
  a2[12] = 0;
  sub_43B8C0(0);
  sub_43B8D0();
  v4 = a2[8];
  a2[13] = -1;
  a2[98] = 0;
  a2[99] = 0;
  sub_42B550(v4, v5);
  sub_42B540(a2[8], 1);
  sub_42CA00(a2[8]);
  sub_42BBB0(a2[8]);
  sub_499F40(a2[1], 512);
  v6 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)a2[8] + 28))(a2[8]);
  sub_46D6C0();
  sub_46D720(v6);
  sub_46DF00(v6);
  v9 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)a2[8] + 28))(a2[8]);
  v7 = sub_4467B0();
  sub_46E500(v7, v9);
  a2[100] = 0;
  return a2;
}

// ===== sub_43B480 @ 0x0043B480..0x0043B4A2 =====
void *__thiscall sub_43B480(void *this, char a2)
{
  sub_43B4B0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_43B4B0 @ 0x0043B4B0..0x0043B556 =====
int __stdcall sub_43B4B0(int a1)
{
  int v1; // ecx
  int v2; // esi
  void **v3; // ebx

  *(_DWORD *)a1 = &CProcSelectItem::`vftable';
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
  sub_46D7A0();
  (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32));
  sub_46D7B0();
  sub_499FA0(v1, 512);
  v2 = 0;
  if ( *(int *)(a1 + 48) > 0 )
  {
    v3 = (void **)(a1 + 60);
    do
    {
      operator delete[](*v3);
      ++v2;
      ++v3;
    }
    while ( v2 < *(_DWORD *)(a1 + 48) );
  }
  return sub_431950((_DWORD *)a1);
}

// ===== sub_43B560 @ 0x0043B560..0x0043B566 =====
int __usercall sub_43B560@<eax>(int result@<eax>)
{
  dword_565D44 = result;
  return result;
}

// ===== sub_43B570 @ 0x0043B570..0x0043B584 =====
int sub_43B570()
{
  int result; // eax

  result = 1;
  if ( dword_565D44 )
    return sub_49A230();
  return result;
}

// ===== sub_43B590 @ 0x0043B590..0x0043B596 =====
int __usercall sub_43B590@<eax>(int result@<eax>)
{
  dword_50762C = result;
  return result;
}

// ===== sub_43B5A0 @ 0x0043B5A0..0x0043B5AC =====
int __usercall sub_43B5A0@<eax>(int result@<eax>, int a2@<ecx>)
{
  dword_507630 = result;
  dword_507634 = a2;
  return result;
}

// ===== sub_43B5B0 @ 0x0043B5B0..0x0043B5D1 =====
int __usercall sub_43B5B0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int result; // eax

  dword_565D48 = a1;
  result = a4;
  dword_565D4C = a2;
  dword_565D50 = a3;
  dword_565D54 = a4;
  return result;
}

// ===== sub_43B5E0 @ 0x0043B5E0..0x0043B670 =====
int __userpurge sub_43B5E0@<eax>(int a1@<ecx>, _DWORD *a2@<edi>, int a3, int a4, int a5, int a6)
{
  int v6; // esi
  _DWORD *v7; // ebx
  _BYTE *v8; // eax
  char *v9; // ecx
  _BYTE *v10; // edx
  char v11; // al
  int v12; // edx
  int v13; // eax

  v6 = 0;
  a2[12] = a3;
  a2[14] = a1;
  if ( a3 > 0 )
  {
    v7 = a2 + 15;
    do
    {
      v8 = operator new[](strlen(*(const char **)(a4 + 4 * v6)) + 1);
      *v7 = v8;
      v9 = *(char **)(a4 + 4 * v6);
      v10 = v8;
      do
      {
        v11 = *v9;
        *v10++ = *v9++;
      }
      while ( v11 );
      ++v6;
      ++v7;
    }
    while ( v6 < a3 );
  }
  v12 = a2[14];
  a2[96] = a6;
  v13 = a2[12];
  a2[95] = a5;
  sub_43B670(a2 + 31, v13, v12, a5, a6);
  return (*(int (__thiscall **)(_DWORD *))(*a2 + 16))(a2);
}

// ===== sub_43B670 @ 0x0043B670..0x0043B8B6 =====
int __fastcall sub_43B670(_DWORD *a1, const char **a2, char *a3, int a4, int a5, int a6, int a7)
{
  int v7; // ecx
  int v8; // esi
  int v9; // eax
  int v10; // eax
  int *v11; // edi
  int v12; // eax
  char *v13; // ecx
  int v14; // esi
  int v15; // edx
  int v16; // eax
  _DWORD *v17; // ecx
  _DWORD *v18; // esi
  int v20; // [esp-14h] [ebp-144h]
  int v21; // [esp+10h] [ebp-120h]
  int v22; // [esp+10h] [ebp-120h]
  const char **v23; // [esp+14h] [ebp-11Ch]
  const char **v24; // [esp+18h] [ebp-118h] BYREF
  char *v25; // [esp+1Ch] [ebp-114h]
  _DWORD *v26; // [esp+20h] [ebp-110h]
  int v27; // [esp+24h] [ebp-10Ch]
  int v28; // [esp+28h] [ebp-108h]
  int v29; // [esp+2Ch] [ebp-104h]
  int v30[4]; // [esp+30h] [ebp-100h] BYREF
  int v31; // [esp+40h] [ebp-F0h]
  int v32; // [esp+44h] [ebp-ECh]
  int v33; // [esp+48h] [ebp-E8h]
  int v34; // [esp+4Ch] [ebp-E4h]
  void *v35[22]; // [esp+50h] [ebp-E0h] BYREF
  _DWORD v36[33]; // [esp+A8h] [ebp-88h] BYREF

  v25 = a3;
  v26 = a1;
  v24 = a2;
  sub_42C2A0(v30, a1);
  v32 = v30[1];
  v7 = v30[2] - v30[0] + 1;
  v8 = 0;
  if ( a5 > 0 )
  {
    v21 = 0;
    do
    {
      v9 = v21 / a5;
      v21 += v7;
      ++v8;
      v10 = v30[0] + v9;
      v36[v8 + 15] = v10;
      v35[v8 + 5] = (void *)(v7 / a5 / 2 + v10);
    }
    while ( v8 < a5 );
  }
  v29 = sub_42C450((int)v26);
  v27 = sub_42C460((int)v26);
  v33 = sub_42C4C0(v26);
  v34 = sub_42C520((int)v26);
  v31 = sub_42C7D0(v26, v36);
  sub_42B9B0((int)v26);
  v28 = 0;
  v22 = 0;
  if ( a4 > 0 )
  {
    v23 = v24;
    v11 = (int *)(v25 + 4);
    v25 = (char *)((char *)v36 - (char *)v24);
    do
    {
      strlen(*v23);
      sub_409080(v35, 1);
      sub_40A620((int)v35, 0);
      if ( v31 )
        v12 = *(int *)((char *)v23 + (_DWORD)v25);
      else
        v12 = a7;
      sub_4097D0(v34, 128, dword_565B70, (int)v35, (int)&v24, 0, 0, (int)*v23, v29, v12);
      if ( a6 )
        v13 = (char *)v35[v22 + 6] - ((unsigned int)v24 >> 1);
      else
        v13 = (char *)v36[v22 + 16];
      v14 = v28;
      v11[1] = (int)v24 + (_DWORD)v13 - 1;
      v15 = v27;
      *(v11 - 1) = (int)v13;
      v16 = v32 + v33 * (v14 / a5);
      v11[2] = v15 + v16 - 1;
      v20 = (int)v13;
      v17 = v26;
      *v11 = v16;
      sub_42B5B0(v17, v30, v20, v16, (int *)v35, 128, 0);
      operator delete[](v35[0]);
      ++v23;
      v11 += 4;
      v28 = v14 + 1;
      v22 = (v22 + 1) % a5;
    }
    while ( v14 + 1 < a4 );
  }
  v18 = v26;
  sub_42C2F0(v30, (int)v26);
  (*(void (__thiscall **)(_DWORD *))(*v18 + 28))(v18);
  return sub_443240(dword_565B6C);
}

// ===== sub_43B8C0 @ 0x0043B8C0..0x0043B8C4 =====
int __usercall sub_43B8C0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 44) = a2;
  return result;
}

// ===== sub_43B8D0 @ 0x0043B8D0..0x0043B8D7 =====
int __usercall sub_43B8D0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 388) = a2;
  return result;
}

// ===== sub_43B8E0 @ 0x0043B8E0..0x0043B8EF =====
int __stdcall sub_43B8E0(int a1)
{
  return a1 & 0x3FFF3C3;
}

// ===== sub_43B8F0 @ 0x0043B8F0..0x0043BA0C =====
int __thiscall sub_43B8F0(unsigned int *this)
{
  int v2; // eax
  unsigned int v3; // esi
  int v4; // eax
  int v5; // eax
  bool v6; // zf
  unsigned int v7; // esi
  int v8; // edi
  int v9; // eax
  BOOL v10; // eax
  unsigned int v12; // [esp+20h] [ebp-8h]

  sub_431AF0((int)this, (int)this);
  v2 = (*(int (__thiscall **)(unsigned int))(*(_DWORD *)this[8] + 28))(this[8]);
  v3 = *this;
  v4 = sub_46DF00(v2);
  v5 = (*(int (__thiscall **)(unsigned int *, int))(v3 + 28))(this, v4);
  v6 = this[97] == 0;
  this[9] = v5;
  if ( v6 )
    this[9] = v5 & 0xFFFFFDFD;
  sub_499FE0();
  v7 = this[100];
  this[10] = v12;
  v8 = 0;
  v9 = (*(int (__thiscall **)(unsigned int))(*(_DWORD *)this[8] + 28))(this[8]);
  v10 = sub_46E580(v9) && sub_43B570();
  this[100] = v10;
  if ( sub_431A60(this) || this[9] || this[10] || this[100] != v7 )
    v8 = (*(int (__thiscall **)(unsigned int *))(*this + 44))(this);
  sub_431AA0(this);
  if ( !v8 && sub_4319C0((int)this) )
    return 0;
  sub_4450D0();
  sub_4319C0((int)this);
  sub_4450D0();
  return 1;
}

// ===== sub_43BA10 @ 0x0043BA10..0x0043BA94 =====
int __usercall sub_43BA10@<eax>(int a1@<esi>)
{
  int v1; // edi
  int v2; // ebx
  int v3; // eax
  int v4; // edx
  int *i; // ecx
  _DWORD v7[2]; // [esp+8h] [ebp-10h] BYREF
  _DWORD v8[2]; // [esp+10h] [ebp-8h] BYREF

  sub_48E680(v7);
  (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(a1 + 32) + 48))(*(_DWORD *)(a1 + 32), v8);
  v1 = *(_DWORD *)(a1 + 48);
  v2 = v7[0] - v8[0];
  v3 = v7[1] - v8[1];
  v4 = 0;
  if ( v1 <= 0 )
    return -1;
  for ( i = (int *)(a1 + 132); *(i - 2) > v2 || v2 > *i || *(i - 1) > v3 || v3 > i[1]; i += 4 )
  {
    if ( ++v4 >= v1 )
      return -1;
  }
  return v4;
}

// ===== sub_43BAA0 @ 0x0043BAA0..0x0043BABB =====
BOOL __userpurge sub_43BAA0@<eax>(int a1@<eax>, int a2, int a3)
{
  return a2 + a3 * *(_DWORD *)(a1 + 56) < *(_DWORD *)(a1 + 48);
}

// ===== sub_43BAC0 @ 0x0043BAC0..0x0043BB31 =====
int __usercall sub_43BAC0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v4; // ecx
  _DWORD v6[2]; // [esp+8h] [ebp-8h] BYREF

  (*(void (__thiscall **)(_DWORD, _DWORD *))(**(_DWORD **)(a2 + 32) + 48))(*(_DWORD *)(a2 + 32), v6);
  v4 = a1 + 8;
  a1 *= 2;
  return sub_48E780(
           v6[0]
         + *(_DWORD *)(a2 + 8 * a1 + 124)
         + ((*(_DWORD *)(a2 + 8 * a1 + 132) - *(_DWORD *)(a2 + 8 * a1 + 124) + 1) >> 1),
           v6[1] + *(_DWORD *)(a2 + 16 * v4) + ((*(_DWORD *)(a2 + 8 * a1 + 136) - *(_DWORD *)(a2 + 16 * v4) + 1) >> 1),
           dword_565D4C,
           dword_565D54,
           0);
}

// ===== sub_43BB40 @ 0x0043BB40..0x0043BE6E =====
int __stdcall sub_43BB40(_DWORD *a1)
{
  int v1; // eax
  int v2; // esi
  int v3; // ecx
  int v4; // eax
  int *v5; // edi
  int v6; // esi
  int v7; // eax
  int v8; // ecx
  int v9; // edi
  int v10; // esi
  int v11; // eax
  unsigned int v12; // ecx
  bool v14; // zf
  int v15; // [esp+Ch] [ebp-54h]
  int v16; // [esp+Ch] [ebp-54h]
  int v17; // [esp+Ch] [ebp-54h]
  int v18; // [esp+Ch] [ebp-54h]
  unsigned int v19; // [esp+10h] [ebp-50h]
  int v20; // [esp+14h] [ebp-4Ch]
  int v21; // [esp+18h] [ebp-48h]
  int v22; // [esp+1Ch] [ebp-44h] BYREF
  int v23; // [esp+20h] [ebp-40h]
  int v24; // [esp+24h] [ebp-3Ch]
  int v25; // [esp+28h] [ebp-38h]
  int v26; // [esp+2Ch] [ebp-34h]
  int v27; // [esp+30h] [ebp-30h]
  int v28; // [esp+34h] [ebp-2Ch]
  int v29; // [esp+38h] [ebp-28h]
  int v30; // [esp+3Ch] [ebp-24h]
  int v31; // [esp+40h] [ebp-20h]
  int v32; // [esp+44h] [ebp-1Ch]
  _DWORD v33[5]; // [esp+48h] [ebp-18h] BYREF

  v1 = a1[9];
  v20 = 0;
  if ( (v1 & 0x3FF0000) != 0 )
  {
    v2 = 0x10000;
    v22 = 0x10000;
    v23 = 0x20000;
    v24 = 0x40000;
    v25 = 0x80000;
    v26 = 0x100000;
    v27 = 0x200000;
    v28 = 0x400000;
    v29 = 0x800000;
    v30 = 0x1000000;
    v31 = 0x2000000;
    v32 = 0;
    v3 = 0;
    while ( (v2 & v1) == 0 || v3 >= a1[12] )
    {
      v2 = *(&v23 + v3++);
      if ( !v2 )
        return v20;
    }
    goto LABEL_13;
  }
  if ( (v1 & 0x303) == 0 )
  {
    if ( (v1 & 0xF0C0) == 0 )
      return v20;
    v33[1] = 0x2000;
    v25 = 0x2000;
    v33[0] = 4096;
    v33[2] = 0x4000;
    v33[3] = 0x8000;
    v33[4] = 0;
    v22 = 64;
    v23 = 128;
    v24 = 4096;
    v26 = 0x4000;
    v27 = 0x8000;
    v28 = 0;
    v5 = &v22;
    if ( !dword_565D48 )
      v5 = v33;
    v6 = *v5;
    v7 = 0;
    v19 = 0;
    if ( *v5 )
    {
      while ( (v6 & a1[9]) == 0 )
      {
        v6 = v5[++v7];
        if ( !v6 )
          goto LABEL_23;
      }
      v19 = v5[v7];
    }
LABEL_23:
    v8 = a1[14];
    v9 = a1[11] % v8;
    v10 = a1[11] / v8;
    v11 = (a1[12] + v8 - 1) / v8;
    v12 = v19;
    v21 = v11;
    if ( v19 > 0x2000 )
    {
      if ( v19 == 0x4000 )
      {
        v18 = 0;
        if ( (int)a1[14] <= 0 )
          return 2;
        while ( 1 )
        {
          if ( --v9 < 0 )
            v9 = a1[14] - 1;
          if ( sub_43BAA0((int)a1, v9, v10) )
            break;
          if ( ++v18 >= a1[14] )
            return 2;
        }
      }
      else
      {
        if ( v19 != 0x8000 )
          goto LABEL_36;
        v17 = 0;
        if ( (int)a1[14] <= 0 )
          return 2;
        while ( 1 )
        {
          v9 = v9 + 1 >= a1[14] ? 0 : v9 + 1;
          if ( sub_43BAA0((int)a1, v9, v10) )
            break;
          if ( ++v17 >= a1[14] )
            return 2;
        }
      }
      sub_43B8C0((int)a1, v9 + v10 * a1[14]);
      return 2;
    }
    if ( v19 != 0x2000 )
    {
      if ( v19 == 64 )
      {
LABEL_28:
        v15 = 0;
        if ( v11 <= 0 )
          goto LABEL_36;
        while ( 1 )
        {
          if ( --v10 < 0 )
            v10 = v11 - 1;
          if ( sub_43BAA0((int)a1, v9, v10) )
            break;
          if ( ++v15 >= v21 )
            goto LABEL_35;
          v11 = v21;
        }
        v14 = v19 == 64;
LABEL_40:
        if ( v14 )
        {
          sub_43BAC0(v9 + v10 * a1[14], (int)a1);
          return 2;
        }
        sub_43B8C0((int)a1, v9 + v10 * a1[14]);
LABEL_35:
        v12 = v19;
LABEL_36:
        if ( !v12 )
          return v20;
        return 2;
      }
      if ( v19 != 128 )
      {
        if ( v19 != 4096 )
          goto LABEL_36;
        goto LABEL_28;
      }
    }
    v16 = 0;
    if ( v11 <= 0 )
      goto LABEL_36;
    while ( 1 )
    {
      v10 = v10 + 1 >= v11 ? 0 : v10 + 1;
      if ( sub_43BAA0((int)a1, v9, v10) )
        break;
      if ( ++v16 >= v21 )
        goto LABEL_35;
      v11 = v21;
    }
    v14 = v19 == 128;
    goto LABEL_40;
  }
  if ( (v1 & 0x202) != 0 )
  {
    sub_43B8C0((int)a1, -1);
    return 1;
  }
  else
  {
    if ( (v1 & 1) == 0 )
      return 1;
    v4 = sub_43BA10((int)a1);
    if ( v4 >= 0 )
    {
      v3 = v4;
LABEL_13:
      sub_43B8C0((int)a1, v3);
      return 1;
    }
  }
  return v20;
}

// ===== sub_43BE70 @ 0x0043BE70..0x0043BEC7 =====
int __thiscall sub_43BE70(int *this)
{
  int v2; // ebx
  int v3; // eax
  int v4; // edi
  int v5; // eax

  v2 = 0;
  if ( !this[100] )
    return 0;
  v3 = sub_43BA10((int)this);
  v4 = v3;
  if ( v3 >= 0 && v3 != this[11] )
  {
    sub_43B8C0((int)this, v3);
    v2 = 1;
  }
  if ( v4 != this[13] )
  {
    v5 = sub_42D560(this[1]);
    sub_496540(268435458, v5, v4);
    this[13] = v4;
  }
  return v2;
}

// ===== sub_43BED0 @ 0x0043BED0..0x0043BED6 =====
int sub_43BED0()
{
  return dword_50762C;
}

// ===== sub_43BEE0 @ 0x0043BEE0..0x0043C06A =====
int __thiscall sub_43BEE0(int *this)
{
  int v2; // esi
  int v3; // eax
  int *v4; // edx
  int v5; // ecx
  int v6; // eax
  int v7; // edx
  int (__thiscall *v8)(int *); // edx
  int v10; // [esp-4h] [ebp-44h]
  int *v11; // [esp+Ch] [ebp-34h]
  BOOL v12; // [esp+10h] [ebp-30h]
  _BYTE v13[4]; // [esp+14h] [ebp-2Ch] BYREF
  _DWORD v14[4]; // [esp+18h] [ebp-28h] BYREF
  void *v15[6]; // [esp+28h] [ebp-18h] BYREF

  if ( sub_42C330(0, this[8], v14) )
  {
    (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
    sub_443240(dword_565B6C);
  }
  if ( this[11] == -1 )
  {
    (*(void (__thiscall **)(int *, _DWORD))(*this + 8))(this, 0);
    sub_42BAB0(this[8], 0, 0);
    return (*(int (__thiscall **)(int *))(*this + 16))(this);
  }
  else
  {
    v2 = *this;
    v3 = (*(int (__thiscall **)(int *))(*this + 36))(this);
    (*(void (__thiscall **)(int *, int))(v2 + 8))(this, v3);
    v4 = &this[4 * this[11] + 31];
    v11 = v4;
    v12 = dword_507630 != 0;
    if ( dword_507630 )
    {
      sub_42C460(this[8]);
      sub_409080(v15, 1);
      sub_40A620((int)v15, 0);
      sub_42C520(this[8]);
      v10 = dword_507630;
      v6 = sub_42C450(v5);
      sub_4097D0(v7, 128, dword_565B70, (int)v15, (int)v13, 0, 0, this[this[11] + 15], v6, v10);
      sub_42BB30(0, (int)v15, this[8]);
      operator delete[](v15[0]);
      v4 = v11;
    }
    sub_42BAE0(this[8], 0, *v4, v4[1], 0);
    sub_42BAB0(this[8], 0, v12);
    if ( sub_42C330(0, this[8], v14) )
    {
      (*(void (__thiscall **)(int))(*(_DWORD *)this[8] + 28))(this[8]);
      sub_443240(dword_565B6C);
    }
    v8 = *(int (__thiscall **)(int *))(*this + 16);
    this[99] = 0;
    return v8(this);
  }
}
