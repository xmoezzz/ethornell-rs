#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_44C110 @ 0x0044C110..0x0044C16F =====
BOOL __thiscall sub_44C110(_DWORD *this, int a2, int a3)
{
  BOOL result; // eax
  int v4; // ebx

  result = 0;
  if ( a2 >= 0 && a2 < this[13] && a3 >= 0 )
  {
    v4 = this[14];
    if ( a3 < *(_DWORD *)(52 * a2 + v4) )
      return !*(_DWORD *)(*(_DWORD *)(this[42] + (a2 << 6) + 8) + 196 * a3 + 52) || a3 != *(_DWORD *)(52 * a2 + v4 + 8);
  }
  return result;
}

// ===== sub_44C170 @ 0x0044C170..0x0044C230 =====
int __thiscall sub_44C170(_DWORD *this, int a2, int a3)
{
  int v4; // esi
  int v6; // [esp+10h] [ebp-8h] BYREF
  int v7; // [esp+14h] [ebp-4h]

  v4 = sub_449FA0(this, a2, a3);
  if ( v4 )
  {
    if ( a2 == -1 )
    {
      sub_44A1D0((int)this, 268435462, -1, a3);
    }
    else
    {
      if ( a3 )
      {
        v6 = 0;
        v7 = 0;
        sub_44A6F0(*(_DWORD *)(this[23] + 20 * a2 + 4), (int)this, &v6, *(_DWORD *)(this[23] + 20 * a2 + 8));
        sub_44A1D0(
          (int)this,
          268435463,
          *(_DWORD *)(this[23] + 20 * a2 + 8) | (*(_DWORD *)(this[23] + 20 * a2 + 4) << 16),
          (unsigned __int16)v6 | (v7 << 16));
      }
      sub_44A1D0(
        (int)this,
        268435462,
        *(_DWORD *)(this[23] + 20 * a2 + 8) | (*(_DWORD *)(this[23] + 20 * a2 + 4) << 16),
        a3);
    }
  }
  return v4;
}

// ===== sub_44C230 @ 0x0044C230..0x0044C2CF =====
int __thiscall sub_44C230(_DWORD *this, int a2, int a3, int a4)
{
  int result; // eax
  int v6; // [esp+Ch] [ebp-Ch]
  int v7; // [esp+10h] [ebp-8h] BYREF
  int v8; // [esp+14h] [ebp-4h]

  result = sub_44A000(this, a2, a3, a4);
  v6 = result;
  if ( result )
  {
    if ( a2 == -1 || a3 == -1 )
    {
      sub_44A1D0((int)this, 268435462, -1, a4);
    }
    else
    {
      if ( a4 )
      {
        v7 = 0;
        v8 = 0;
        sub_44A6F0(a2, (int)this, &v7, a3);
        sub_44A1D0((int)this, 268435463, a3 | (a2 << 16), (unsigned __int16)v7 | (v8 << 16));
      }
      sub_44A1D0((int)this, 268435462, a3 | (a2 << 16), a4);
    }
    return v6;
  }
  return result;
}

// ===== sub_44C2D0 @ 0x0044C2D0..0x0044C35A =====
int __thiscall sub_44C2D0(int this)
{
  int v2; // edi
  int v3; // ebx

  v2 = 0;
  if ( *(int *)(this + 164) > 0 )
  {
    v3 = 0;
    do
    {
      operator delete[](*(void **)(v3 + *(_DWORD *)(this + 168) + 8));
      operator delete[](*(void **)(*(_DWORD *)(this + 204) + 4 * v2++));
      v3 += 64;
    }
    while ( v2 < *(_DWORD *)(this + 164) );
  }
  operator delete[](*(void **)(this + 168));
  operator delete[](*(void **)(this + 204));
  operator delete[](*(void **)(this + 208));
  *(_DWORD *)(this + 168) = 0;
  *(_DWORD *)(this + 204) = 0;
  *(_DWORD *)(this + 208) = 0;
  *(_DWORD *)(this + 212) = 0;
  return sub_44A050(this);
}

// ===== sub_44C360 @ 0x0044C360..0x0044C498 =====
int __thiscall sub_44C360(_DWORD *this, int a2, int a3, unsigned int a4, int a5)
{
  int result; // eax
  int *v7; // ecx
  int v8; // eax
  BOOL v9; // ecx
  int v10; // ecx
  int v11; // eax
  bool v12; // zf
  int v13; // eax
  unsigned int v14; // eax
  int v15; // [esp+14h] [ebp-18h] BYREF
  int v16[4]; // [esp+18h] [ebp-14h] BYREF

  result = 0;
  if ( a2 >= 0 && a2 < this[13] )
  {
    v7 = (int *)(this[42] + (a2 << 6));
    if ( a3 >= 0 && a3 < *v7 )
    {
      if ( a4 >= 4 )
      {
        return sub_44A3E0(this, a2, a3, a4, a5);
      }
      else
      {
        *(_DWORD *)(v7[2] + 4 * (a4 + 49 * a3) + 32) = a5;
        v8 = this[29];
        v9 = 0;
        if ( v8 != -1 )
        {
          v10 = this[23];
          v11 = 5 * v8;
          v12 = a2 == *(_DWORD *)(v10 + 4 * v11 + 4);
          v13 = v10 + 4 * v11;
          v9 = v12 && a3 == *(_DWORD *)(v13 + 8);
        }
        (*(void (__thiscall **)(_DWORD *, int *, int, int, BOOL, int))(*this + 32))(this, &v15, a2, a3, v9, 1);
        sub_42C090(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * a2) + 48 * a3 + 8), this[10], v15);
        sub_42C1C0(*(_DWORD *)(*(_DWORD *)(this[51] + 4 * a2) + 48 * a3 + 8), v16, (_DWORD *)this[10]);
        v14 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)this[10] + 28))(this[10]);
        sub_443240(v16, v14, dword_565D6C);
        sub_4478D0((int)this);
        return 1;
      }
    }
  }
  return result;
}

// ===== sub_44C4A0 @ 0x0044C4A0..0x0044C658 =====
int __thiscall sub_44C4A0(_DWORD *this, int a2, int a3, int a4, int a5, int a6)
{
  int result; // eax
  _DWORD *v8; // esi
  int v9; // ecx
  int v10; // eax
  int v11; // edi
  int v12; // eax
  int v13; // ecx
  unsigned int v14; // eax
  unsigned int v15; // eax
  int v16; // [esp+Ch] [ebp-24h]
  _DWORD v17[2]; // [esp+10h] [ebp-20h] BYREF
  int v18; // [esp+18h] [ebp-18h]
  int v19[4]; // [esp+1Ch] [ebp-14h] BYREF

  result = 0;
  v18 = a2;
  v17[0] = a3;
  if ( a2 >= 0 && a2 < this[13] && a3 >= 0 && a3 < *(_DWORD *)((a2 << 6) + this[42]) )
  {
    v8 = (_DWORD *)(*(_DWORD *)(this[51] + 4 * a2) + 48 * a3);
    v9 = 52 * a2;
    *(_DWORD *)(v8[1] + 8) = a4;
    *(_DWORD *)(v8[1] + 12) = a5;
    v8[11] = a6;
    v10 = 60 * a3;
    *(_DWORD *)(*(_DWORD *)(v9 + this[14] + 4) + v10 + 4) = a4;
    *(_DWORD *)(*(_DWORD *)(v9 + this[14] + 4) + v10 + 8) = a5;
    v16 = 0;
    v11 = (*(int (__thiscall **)(_DWORD *, _DWORD))(*this + 68))(this, 0);
    if ( v11 != -1 )
    {
      while ( 1 )
      {
        v12 = this[23] + 20 * v11;
        if ( v18 == *(_DWORD *)(v12 + 4) && v17[0] == *(_DWORD *)(v12 + 8) )
          break;
        v11 = (*(int (__thiscall **)(_DWORD *, int))(*this + 68))(this, ++v16);
        if ( v11 == -1 )
          goto LABEL_11;
      }
      (*(void (__thiscall **)(_DWORD, _DWORD *))(*(_DWORD *)this[10] + 48))(this[10], v17);
      v13 = *(_DWORD *)(this[23] + 20 * v11 + 16);
      (*(void (__thiscall **)(int, int, int))(*(_DWORD *)v13 + 44))(v13, a4 + v17[0], a5 + v17[1]);
    }
LABEL_11:
    sub_42C220((_DWORD *)this[10], v19, v8[2]);
    v14 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)this[10] + 28))(this[10]);
    sub_443240(v19, v14, dword_565D6C);
    sub_42C0D0(v8[2], a6, (_DWORD *)this[10], a4 + *(_DWORD *)(v8[1] + 16), a5 + *(_DWORD *)(v8[1] + 20));
    sub_42C1C0(v8[2], v19, (_DWORD *)this[10]);
    v15 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)this[10] + 28))(this[10]);
    sub_443240(v19, v15, dword_565D6C);
    sub_4478D0((int)this);
    return 1;
  }
  return result;
}

// ===== sub_44C660 @ 0x0044C660..0x0044C6BB =====
int __thiscall sub_44C660(_DWORD *this, int a2, int a3, int a4, int a5)
{
  int result; // eax
  _DWORD *v6; // eax

  result = 0;
  if ( a2 >= 0 && a2 < this[13] && a3 >= 0 && a3 < *(_DWORD *)(52 * a2 + this[14]) )
  {
    v6 = (_DWORD *)(*(_DWORD *)(this[51] + 4 * a2) + 48 * a3);
    *(_DWORD *)(v6[1] + 24) = a4;
    *(_DWORD *)(v6[1] + 28) = a5;
    v6[4] = 0;
    v6[5] = 0;
    return 1;
  }
  return result;
}

// ===== sub_44C6C0 @ 0x0044C6C0..0x0044C6E2 =====
int __thiscall sub_44C6C0(_DWORD *this, int a2)
{
  int result; // eax

  result = -1;
  if ( a2 >= 0 && a2 < this[53] )
    return *(_DWORD *)(this[52] + 4 * a2);
  return result;
}

// ===== sub_44C6F0 @ 0x0044C6F0..0x0044C751 =====
BOOL __thiscall sub_44C6F0(_DWORD *this, int a2)
{
  BOOL result; // eax
  int v3; // edx

  result = 1;
  if ( a2 != -1 )
  {
    v3 = this[42];
    return (*(_BYTE *)((*(_DWORD *)(this[23] + 20 * a2 + 4) << 6) + v3 + 60) & 2) == 0
        && (*(_BYTE *)(*(_DWORD *)((*(_DWORD *)(this[23] + 20 * a2 + 4) << 6) + v3 + 8)
                     + 196 * *(_DWORD *)(this[23] + 20 * a2 + 8)
                     + 192) & 0x20) == 0;
  }
  return result;
}

// ===== sub_44C760 @ 0x0044C760..0x0044C7BC =====
_DWORD *__thiscall sub_44C760(void *this, _DWORD *a2, int a3)
{
  sub_430570(this, a2, a3, -1);
  *a2 = &DCInnerDspObjMngr::`vftable';
  return a2;
}

// ===== sub_44C7C0 @ 0x0044C7C0..0x0044C7E2 =====
void *__thiscall sub_44C7C0(void *this, char a2)
{
  sub_44C7F0(this);
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44C7F0 @ 0x0044C7F0..0x0044C838 =====
int __stdcall sub_44C7F0(_DWORD *a1)
{
  *a1 = &DCInnerDspObjMngr::`vftable';
  return sub_430680(a1);
}

// ===== sub_44C840 @ 0x0044C840..0x0044C85B =====
int __thiscall sub_44C840(char *this, int a2, int a3)
{
  return sub_4C0150(this + 260, a2, a3);
}

// ===== sub_44C860 @ 0x0044C860..0x0044C8DB =====
_DWORD *__thiscall sub_44C860(void *this, _DWORD *a2, int a3, int a4)
{
  sub_4C5760(0, 0, this, a4);
  *a2 = &CMemReader::`vftable';
  a2[3] = &CMemReader::`vftable';
  a2[4] = &CMemReader::`vftable';
  sub_4C0B80(a3);
  return a2;
}

// ===== sub_44C8E0 @ 0x0044C8E0..0x0044C938 =====
void *__thiscall sub_44C8E0(void *this, char a2)
{
  sub_4C5560();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44C940 @ 0x0044C940..0x0044C984 =====
_DWORD *__usercall sub_44C940@<eax>(_DWORD *result@<eax>)
{
  *result = &DCMovieRenderer::`vftable';
  result[2] = 0;
  result[3] = 0;
  result[4] = 0;
  result[5] = 0;
  result[6] = 0;
  result[7] = 0;
  result[8] = 0;
  result[9] = 0;
  result[10] = 0;
  result[11] = 0;
  result[12] = sub_44D790;
  result[13] = result;
  result[14] = 0;
  result[15] = 0;
  result[20] = 0;
  result[224] = -1;
  return result;
}

// ===== sub_44C990 @ 0x0044C990..0x0044C9B1 =====
void *__thiscall sub_44C990(void *this, char a2)
{
  sub_44C9C0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44C9C0 @ 0x0044C9C0..0x0044C9D0 =====
int __thiscall sub_44C9C0(_DWORD *this)
{
  *this = &DCMovieRenderer::`vftable';
  return sub_44D080();
}

// ===== sub_44C9D0 @ 0x0044C9D0..0x0044CFEF =====
int __thiscall sub_44C9D0(
        int this,
        int a2,
        int a3,
        int a4,
        const CHAR *a5,
        LONG lDistanceToMove,
        int a7,
        int a8,
        int a9)
{
  _DWORD *v10; // ebx
  void *v11; // eax
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  int *v16; // edi
  int v17; // eax
  int v18; // ecx
  int v19; // edx
  int v20; // eax
  _DWORD *v21; // eax
  _DWORD *v22; // eax
  bool v23; // sf
  int v24; // eax
  int v25; // ecx
  int v26; // eax
  int v27; // ecx
  int v28; // eax
  int v29; // ecx
  int v30; // eax
  int v31; // eax
  int v32; // eax
  int v33; // [esp+64h] [ebp-9DCh]
  int v34; // [esp+68h] [ebp-9D8h]
  unsigned int v35; // [esp+70h] [ebp-9D0h]
  int v36; // [esp+74h] [ebp-9CCh]
  void *v37; // [esp+78h] [ebp-9C8h]
  int v38; // [esp+78h] [ebp-9C8h]
  HRESULT Instance; // [esp+7Ch] [ebp-9C4h] BYREF
  _DWORD v40[18]; // [esp+80h] [ebp-9C0h] BYREF
  _SYSTEMTIME SystemTime; // [esp+C8h] [ebp-978h] BYREF
  _DWORD v42[12]; // [esp+D8h] [ebp-968h] BYREF
  CHAR MultiByteStr[784]; // [esp+108h] [ebp-938h] BYREF
  WCHAR WideCharStr[780]; // [esp+418h] [ebp-628h] BYREF
  int v45; // [esp+A3Ch] [ebp-4h]

  v33 = dword_565B24;
  v34 = dword_565B20;
  if ( *(_DWORD *)(this + 8) )
    sub_44D080();
  v10 = (_DWORD *)(this + 16);
  v36 = -2147483647;
  Instance = CoCreateInstance(&rclsid, 0, 1u, &riid, (LPVOID *)(this + 16));
  if ( Instance < 0 )
    goto LABEL_22;
  v11 = operator new(0x190u);
  v45 = 0;
  v12 = v11 ? sub_45BB20(v11, a2, a3, this + 48) : 0;
  v45 = -1;
  v13 = v12 ? v12 + 12 : 0;
  *(_DWORD *)(this + 20) = v13;
  if ( Instance < 0 )
    goto LABEL_22;
  if ( !v13 )
    goto LABEL_22;
  Instance = (*(int (__stdcall **)(_DWORD, int, const wchar_t *))(*(_DWORD *)*v10 + 12))(
               *v10,
               v13,
               L"TEXTURERENDERER_BURIKO");
  if ( Instance < 0 )
    goto LABEL_22;
  Instance = (**(int (__stdcall ***)(_DWORD, void *, int))*v10)(*v10, &unk_4DB8C4, this + 24);
  if ( Instance < 0 )
    goto LABEL_22;
  Instance = (**(int (__stdcall ***)(_DWORD, void *, int))*v10)(*v10, &unk_4DB8D4, this + 28);
  if ( Instance < 0 )
    goto LABEL_22;
  Instance = (**(int (__stdcall ***)(_DWORD, void *, int))*v10)(*v10, &unk_4DB9B4, this + 32);
  if ( Instance < 0 )
    goto LABEL_22;
  (**(void (__stdcall ***)(_DWORD, void *, int))*v10)(*v10, &unk_4DB8F4, this + 36);
  if ( a7 )
  {
    GetSystemTime(&SystemTime);
    sprintf(
      MultiByteStr,
      "DCMovieRenderer%02d%02d%02d%03d",
      SystemTime.wHour,
      SystemTime.wMinute,
      SystemTime.wSecond,
      SystemTime.wMilliseconds);
    MultiByteToWideChar(0, 0, MultiByteStr, -1, WideCharStr, 780);
    Instance = (*(int (__stdcall **)(_DWORD, WCHAR *, _DWORD))(*(_DWORD *)*v10 + 52))(*v10, WideCharStr, 0);
    v37 = operator new(0x158u);
    v45 = 1;
    if ( v37 )
      v14 = sub_4C5C10(-1);
    else
      v14 = 0;
    v45 = -1;
    *(_DWORD *)(this + 40) = v14;
    if ( sub_4C5980(a5, a7, 0, lDistanceToMove, 0) )
      goto LABEL_21;
    v42[0] = -466162812;
    v42[1] = 298734159;
    v42[2] = 536892319;
    v42[3] = 1889995695;
    v42[4] = -466162810;
    v42[5] = 298734159;
    v42[6] = 536892319;
    v42[7] = 1889995695;
    memset(&v42[8], 0, 16);
    v35 = 0;
    v38 = 1;
    v16 = v42;
    while ( v35 < 3 )
    {
      sub_4C0AB0(v40);
      v45 = 2;
      v40[0] = -466162813;
      v40[1] = 298734159;
      v17 = *v16;
      v40[2] = 536892319;
      v18 = v16[1];
      v40[3] = 1889995695;
      v19 = v16[2];
      v40[4] = v17;
      v20 = v16[3];
      v40[5] = v18;
      v40[6] = v19;
      v40[7] = v20;
      Instance = 0;
      v21 = operator new(0x1E8u);
      LOBYTE(v45) = 3;
      if ( v21 )
        v22 = sub_44C860(*(void **)(this + 40), v21, (int)v40, (int)&Instance);
      else
        v22 = 0;
      LOBYTE(v45) = 2;
      v23 = Instance < 0;
      *(_DWORD *)(this + 44) = v22;
      if ( v23 || !v22 )
        goto LABEL_47;
      (*(void (__stdcall **)(_DWORD *))(v22[3] + 4))(v22 + 3);
      v24 = *(_DWORD *)(this + 44);
      v25 = v24 ? v24 + 12 : 0;
      Instance = (*(int (__stdcall **)(_DWORD, int, _DWORD))(*(_DWORD *)*v10 + 12))(*v10, v25, 0);
      if ( Instance < 0 )
        goto LABEL_47;
      v26 = (*(int (__thiscall **)(_DWORD, _DWORD))(**(_DWORD **)(this + 44) + 28))(*(_DWORD *)(this + 44), 0);
      if ( v26 )
        v27 = v26 + 12;
      else
        v27 = 0;
      v28 = (*(int (__stdcall **)(_DWORD, int))(*(_DWORD *)*v10 + 48))(*v10, v27);
      v29 = 0;
      Instance = v28;
      if ( v28 < 0 )
      {
        v30 = *(_DWORD *)(this + 44);
        if ( v30 )
          v29 = v30 + 12;
        Instance = (*(int (__stdcall **)(_DWORD, int))(*(_DWORD *)*v10 + 16))(*v10, v29);
        if ( Instance < 0 )
        {
LABEL_47:
          v45 = -1;
          sub_4C0AA0(v40);
          break;
        }
        v31 = *(_DWORD *)(this + 44);
        if ( v31 )
        {
          (*(void (__stdcall **)(int))(*(_DWORD *)(v31 + 12) + 8))(v31 + 12);
          *(_DWORD *)(this + 44) = 0;
        }
        Instance = -2147467259;
      }
      else
      {
        v38 = 0;
      }
      v45 = -1;
      sub_4C0AA0(v40);
      ++v35;
      v16 += 4;
      if ( !v38 )
        break;
    }
    if ( Instance < 0 )
      goto LABEL_22;
  }
  else
  {
    MultiByteToWideChar(0, 0, a5, -1, WideCharStr, 780);
    Instance = (*(int (__stdcall **)(_DWORD, WCHAR *, _DWORD))(*(_DWORD *)*v10 + 52))(*v10, WideCharStr, 0);
    if ( Instance < 0 )
    {
LABEL_21:
      v36 = -2147483646;
LABEL_22:
      sub_44D080();
      return v36;
    }
  }
  Instance = (*(int (__stdcall **)(_DWORD, _DWORD, _DWORD))(**(_DWORD **)(this + 32) + 68))(
               *(_DWORD *)(this + 32),
               COERCE_UNSIGNED_INT64(1.0),
               HIDWORD(COERCE_UNSIGNED_INT64(1.0)));
  if ( Instance < 0 )
    goto LABEL_22;
  if ( sub_44D450(a9) )
  {
    v36 = -2147483645;
    goto LABEL_22;
  }
  Instance = (*(int (__stdcall **)(_DWORD, int, int, int))(**(_DWORD **)(this + 28) + 52))(
               *(_DWORD *)(this + 28),
               v34,
               v33,
               dword_565D88);
  if ( Instance < 0 )
    goto LABEL_22;
  *(_DWORD *)(this + 72) = a2;
  *(_DWORD *)(this + 64) = v34;
  *(_DWORD *)(this + 76) = a3;
  *(_DWORD *)(this + 68) = v33;
  *(_DWORD *)(this + 80) = a4;
  strcpy((char *)(this + 84), a5);
  *(_DWORD *)(this + 868) = a7;
  v32 = dword_565D88;
  *(_DWORD *)(this + 896) = dword_565D88;
  *(_DWORD *)(this + 864) = lDistanceToMove;
  dword_565D88 = v32 + 1;
  *(_DWORD *)(this + 872) = a8;
  *(_DWORD *)(this + 876) = a9;
  *(_DWORD *)(this + 880) = 0;
  *(_DWORD *)(this + 888) = 0;
  *(_DWORD *)(this + 892) = 0;
  *(_DWORD *)(this + 8) = 1;
  return 0;
}

// ===== sub_44CFF0 @ 0x0044CFF0..0x0044D040 =====
int __usercall sub_44CFF0@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax

  if ( a1[2] )
  {
    v1 = a1[6];
    if ( v1 )
    {
      if ( a1[3] )
        return -2147483643;
      if ( a1[15] || !a1[14] || (*(int (__stdcall **)(_DWORD))(*(_DWORD *)v1 + 32))(a1[6]) >= 0 )
      {
        a1[3] = 1;
        return 0;
      }
    }
  }
  return -2147483647;
}

// ===== sub_44D040 @ 0x0044D040..0x0044D07E =====
int __usercall sub_44D040@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax

  if ( a1[2] )
  {
    v1 = a1[6];
    if ( v1 )
    {
      if ( a1[3] && (a1[15] || !a1[14] || (*(int (__stdcall **)(_DWORD))(*(_DWORD *)v1 + 28))(a1[6]) >= 0) )
        a1[3] = 0;
    }
  }
  return 0;
}

// ===== sub_44D080 @ 0x0044D080..0x0044D110 =====
int __usercall sub_44D080@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax
  int v2; // eax
  int v3; // eax
  int v4; // eax
  int v5; // eax
  int result; // eax
  int (__thiscall ***v7)(_DWORD, int); // ecx

  v1 = a1[9];
  a1[14] = 0;
  if ( v1 )
  {
    (*(void (__stdcall **)(int))(*(_DWORD *)v1 + 8))(v1);
    a1[9] = 0;
  }
  v2 = a1[8];
  if ( v2 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 8))(a1[8]);
    a1[8] = 0;
  }
  v3 = a1[7];
  if ( v3 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v3 + 8))(a1[7]);
    a1[7] = 0;
  }
  v4 = a1[6];
  if ( v4 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v4 + 8))(a1[6]);
    a1[6] = 0;
  }
  v5 = a1[4];
  if ( v5 )
  {
    (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v5 + 8))(a1[4]);
    a1[4] = 0;
  }
  result = a1[11];
  if ( result )
  {
    result = (*(int (__stdcall **)(int))(*(_DWORD *)(result + 12) + 8))(result + 12);
    a1[11] = 0;
  }
  v7 = (int (__thiscall ***)(_DWORD, int))a1[10];
  if ( v7 )
    result = (**v7)(v7, 1);
  a1[10] = 0;
  a1[3] = 0;
  a1[2] = 0;
  return result;
}

// ===== sub_44D110 @ 0x0044D110..0x0044D209 =====
int __usercall sub_44D110@<eax>(_DWORD *a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // eax
  int v3; // eax
  __int64 v5; // [esp+20h] [ebp-30h] BYREF
  __int64 v6; // [esp+28h] [ebp-28h] BYREF
  __int64 v7; // [esp+30h] [ebp-20h] BYREF
  _DWORD v8[4]; // [esp+3Ch] [ebp-14h] BYREF

  v2 = a2[6];
  if ( !v2 || !a2[3] && (*(int (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 28))(a2[6]) < 0 )
    return -2147483647;
  if ( a1 )
  {
    v3 = a2[8];
    if ( v3 )
    {
      v8[1] = 298814594;
      v8[3] = -160125952;
      v8[0] = 2071483764;
      v8[2] = -1442837316;
      (*(void (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)v3 + 60))(v3, &v5, &v7);
      (*(void (__stdcall **)(_DWORD, __int64 *, _DWORD *, _DWORD, _DWORD, _DWORD))(*(_DWORD *)a2[8] + 52))(
        a2[8],
        &v6,
        v8,
        v7 - v5,
        (unsigned __int64)(v7 - v5) >> 32,
        0);
      *a1 = v6 / 10000;
    }
  }
  a2[14] = 1;
  return 0;
}

// ===== sub_44D210 @ 0x0044D210..0x0044D233 =====
int __usercall sub_44D210@<eax>(int a1@<eax>)
{
  if ( *(_DWORD *)(a1 + 24) && (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(a1 + 24) + 36))(*(_DWORD *)(a1 + 24)) >= 0 )
    return 0;
  else
    return -2147483647;
}

// ===== sub_44D240 @ 0x0044D240..0x0044D2B3 =====
int __usercall sub_44D240@<eax>(int a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // ecx
  int result; // eax

  v2 = a2[6];
  result = -2147483647;
  if ( v2 )
  {
    if ( a2[14] )
    {
      if ( a2[15] )
      {
        if ( a1 )
          return 0;
        if ( a2[3] || (*(int (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 28))(a2[6]) >= 0 )
        {
          a2[15] = 0;
          return 0;
        }
      }
      else
      {
        if ( !a1 )
          return 0;
        if ( a2[3] || (*(int (__stdcall **)(_DWORD))(*(_DWORD *)v2 + 32))(a2[6]) >= 0 )
        {
          a2[15] = a1;
          return 0;
        }
      }
      return -1;
    }
    else
    {
      return -2147483644;
    }
  }
  return result;
}

// ===== sub_44D2C0 @ 0x0044D2C0..0x0044D390 =====
unsigned int __thiscall sub_44D2C0(_DWORD *this, int a2)
{
  int v3; // edi
  _BYTE v5[8]; // [esp+4h] [ebp-1Ch] BYREF
  _DWORD v6[4]; // [esp+Ch] [ebp-14h] BYREF

  if ( !this[8] )
    return -2147483647;
  if ( a2 < 0 )
  {
    (*(void (__stdcall **)(_DWORD, _BYTE *))(*(_DWORD *)this[8] + 44))(this[8], v5);
  }
  else
  {
    v6[0] = 2071483764;
    v3 = this[8];
    v6[2] = -1442837316;
    v6[1] = 298814594;
    v6[3] = -160125952;
    (*(void (__stdcall **)(int, _BYTE *, _DWORD, int, _DWORD, _DWORD *))(*(_DWORD *)v3 + 52))(
      v3,
      v5,
      0,
      10000 * a2,
      (unsigned __int64)(10000LL * a2) >> 32,
      v6);
  }
  return (*(int (__stdcall **)(_DWORD, _BYTE *, int, _DWORD, _DWORD))(*(_DWORD *)this[8] + 56))(this[8], v5, 1, 0, 0) >= 0
       ? 0
       : 0x80000007;
}

// ===== sub_44D390 @ 0x0044D390..0x0044D44A =====
int __usercall sub_44D390@<eax>(int a1@<ecx>, _DWORD *a2@<edi>)
{
  int result; // eax
  int v4; // eax
  __int64 v5; // [esp+28h] [ebp-38h] BYREF
  _DWORD v6[3]; // [esp+30h] [ebp-30h] BYREF
  _DWORD v7[4]; // [esp+3Ch] [ebp-24h] BYREF
  _BYTE v8[16]; // [esp+4Ch] [ebp-14h] BYREF

  result = -2147483647;
  if ( *(_DWORD *)(a1 + 32) )
  {
    v7[0] = 2071483764;
    v7[2] = -1442837316;
    v7[3] = -160125952;
    v4 = *(_DWORD *)(a1 + 32);
    v7[1] = 298814594;
    (*(void (__stdcall **)(int, _DWORD *))(*(_DWORD *)v4 + 48))(v4, v6);
    (*(void (__stdcall **)(_DWORD, _BYTE *))(**(_DWORD **)(a1 + 32) + 28))(*(_DWORD *)(a1 + 32), v8);
    (*(void (__stdcall **)(_DWORD, __int64 *, _DWORD *, _DWORD, _DWORD, _BYTE *))(**(_DWORD **)(a1 + 32) + 52))(
      *(_DWORD *)(a1 + 32),
      &v5,
      v7,
      v6[0],
      v6[1],
      v8);
    *a2 = v5 / 10000;
    return 0;
  }
  return result;
}

// ===== sub_44D450 @ 0x0044D450..0x0044D4CF =====
int __usercall sub_44D450@<eax>(unsigned int a1@<ecx>, int a2@<esi>)
{
  int result; // eax
  int v3; // eax
  int v4; // eax

  result = -2147483645;
  if ( a1 <= 0x80 )
  {
    if ( a1 )
      v3 = (int)-((double)(100 * (128 - a1)) / 2.6666666666);
    else
      v3 = -10000;
    if ( *(_DWORD *)(a2 + 36) )
    {
      v4 = (*(int (__stdcall **)(_DWORD, int))(**(_DWORD **)(a2 + 36) + 28))(*(_DWORD *)(a2 + 36), v3);
      if ( v4 != -2147467263 )
      {
        if ( v4 == -2147024809 )
          return -2147483645;
        if ( v4 )
          return -2147483647;
      }
    }
    return 0;
  }
  return result;
}

// ===== sub_44D4D0 @ 0x0044D4D0..0x0044D555 =====
BOOL __usercall sub_44D4D0@<eax>(_DWORD *a1@<esi>)
{
  int v1; // eax
  __int64 v3; // [esp+4h] [ebp-18h] BYREF
  __int64 v4; // [esp+Ch] [ebp-10h] BYREF
  int v5; // [esp+18h] [ebp-4h] BYREF

  if ( !a1[14] )
    return 0;
  if ( !a1[8] )
    return 0;
  if ( !a1[6] )
    return 0;
  if ( (*(int (__stdcall **)(_DWORD, __int64 *, __int64 *))(*(_DWORD *)a1[8] + 60))(a1[8], &v4, &v3) < 0 )
    return 0;
  v1 = (*(int (__stdcall **)(_DWORD, int, int *))(*(_DWORD *)a1[6] + 40))(a1[6], 1000, &v5);
  if ( v1 )
  {
    if ( v1 != 262711 )
      return 0;
  }
  return a1[218] || v4 < v3 && v5;
}

// ===== sub_44D560 @ 0x0044D560..0x0044D680 =====
int __thiscall sub_44D560(_DWORD *this, _DWORD *a2)
{
  bool v3; // zf
  int v4; // eax
  int v5; // ecx
  __int64 v6; // kr00_8
  __int64 v7; // rax
  int result; // eax
  __int64 v9; // [esp+28h] [ebp-40h] BYREF
  _DWORD *v10; // [esp+34h] [ebp-34h]
  _DWORD v11[3]; // [esp+38h] [ebp-30h] BYREF
  _DWORD v12[4]; // [esp+44h] [ebp-24h] BYREF
  _BYTE v13[16]; // [esp+54h] [ebp-14h] BYREF

  v3 = this[8] == 0;
  v10 = a2;
  if ( v3 )
    return -2147483647;
  v12[2] = -1442837316;
  v4 = this[5];
  v12[0] = 2071483764;
  v12[1] = 298814594;
  v12[3] = -160125952;
  if ( v4 )
    v5 = v4 - 12;
  else
    v5 = 0;
  v6 = sub_45C160(v5);
  (*(void (__stdcall **)(_DWORD, _DWORD *))(*(_DWORD *)this[8] + 48))(this[8], v11);
  (*(void (__stdcall **)(_DWORD, _BYTE *))(*(_DWORD *)this[8] + 28))(this[8], v13);
  (*(void (__stdcall **)(_DWORD, __int64 *, _DWORD *, _DWORD, _DWORD, _BYTE *))(*(_DWORD *)this[8] + 52))(
    this[8],
    &v9,
    v12,
    v11[0],
    v11[1],
    v13);
  if ( v6 <= 0 )
  {
    result = 0;
    HIDWORD(v9) = 0;
    *v10 = 0;
  }
  else
  {
    v7 = v9 / v6;
    *v10 = v9 / v6;
    HIDWORD(v9) = HIDWORD(v7);
    return 0;
  }
  return result;
}

// ===== sub_44D680 @ 0x0044D680..0x0044D687 =====
int __usercall sub_44D680@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 896);
}

// ===== sub_44D690 @ 0x0044D690..0x0044D70E =====
int __usercall sub_44D690@<eax>(int result@<eax>)
{
  int v1; // esi
  int v2; // eax
  _DWORD v3[2]; // [esp+4h] [ebp-18h] BYREF
  __int64 v4; // [esp+Ch] [ebp-10h] BYREF
  __int64 v5; // [esp+14h] [ebp-8h] BYREF

  v1 = result;
  if ( *(_DWORD *)(result + 872) )
  {
    if ( *(_DWORD *)(result + 24) )
    {
      if ( *(_DWORD *)(result + 32) )
      {
        result = (*(int (__stdcall **)(_DWORD, __int64 *, __int64 *))(**(_DWORD **)(result + 32) + 60))(
                   *(_DWORD *)(result + 32),
                   &v5,
                   &v4);
        if ( result >= 0 && v5 >= v4 )
        {
          v2 = *(_DWORD *)(v1 + 32);
          v3[0] = 0;
          v3[1] = 0;
          result = (*(int (__stdcall **)(int, _DWORD *, int, _DWORD, _DWORD))(*(_DWORD *)v2 + 56))(v2, v3, 1, 0, 0);
          if ( result >= 0 )
            return (*(int (__stdcall **)(_DWORD))(**(_DWORD **)(v1 + 24) + 28))(*(_DWORD *)(v1 + 24));
        }
      }
    }
  }
  return result;
}

// ===== sub_44D710 @ 0x0044D710..0x0044D78E =====
int __usercall sub_44D710@<eax>(_DWORD *a1@<esi>)
{
  int v1; // ecx
  int result; // eax
  int v3; // eax
  int v4; // [esp+0h] [ebp-Ch] BYREF
  int v5; // [esp+4h] [ebp-8h] BYREF
  int v6; // [esp+8h] [ebp-4h] BYREF

  v1 = a1[7];
  result = 0;
  if ( v1 )
  {
    if ( (*(int (__stdcall **)(int, int *, int *, int *, _DWORD))(*(_DWORD *)v1 + 32))(v1, &v6, &v4, &v5, 0) >= 0 )
    {
      do
      {
        (*(void (__stdcall **)(_DWORD, int, int, int))(*(_DWORD *)a1[7] + 48))(a1[7], v6, v4, v5);
        v3 = v6 - 1;
        if ( v6 == 1 && a1[218] == v3 )
          a1[14] = v3;
      }
      while ( (*(int (__stdcall **)(_DWORD, int *, int *, int *, _DWORD))(*(_DWORD *)a1[7] + 32))(
                a1[7],
                &v6,
                &v4,
                &v5,
                0) >= 0 );
    }
    return 1;
  }
  return result;
}

// ===== sub_44D790 @ 0x0044D790..0x0044D7A5 =====
int __cdecl sub_44D790(int a1)
{
  int v1; // ecx

  ++*(_DWORD *)(a1 + 880);
  return sub_45BE40(v1, *(_DWORD *)(a1 + 80));
}

// ===== sub_44D7B0 @ 0x0044D7B0..0x0044D7BA =====
int __stdcall sub_44D7B0(int a1, int a2, int a3)
{
  return sub_4C1840(a1 - 4, a2, a3);
}

// ===== sub_44D7C0 @ 0x0044D7C0..0x0044D7CA =====
int __stdcall sub_44D7C0(int a1)
{
  return sub_4BF8F0(a1 - 4);
}

// ===== sub_44D7D0 @ 0x0044D7D0..0x0044D7DA =====
int __stdcall sub_44D7D0(int a1)
{
  return sub_45B670(a1 - 4);
}

// ===== sub_44D7E0 @ 0x0044D7E0..0x0044D82A =====
_DWORD *__userpurge sub_44D7E0@<eax>(int a1@<eax>, int a2@<ecx>, _DWORD *a3@<esi>, _DWORD *a4)
{
  int v4; // eax

  a3[1] = a1;
  *a3 = &DCParticle::`vftable';
  a3[2] = a2;
  a3[8] = 0;
  a3[9] = 0;
  v4 = a4[33] + sub_44DA90();
  a3[10] = v4;
  a3[11] = v4;
  a3[12] = *a4;
  a3[13] = a4;
  a3[3] = 0;
  return a3;
}

// ===== sub_44D830 @ 0x0044D830..0x0044D851 =====
void *__thiscall sub_44D830(void *this, char a2)
{
  sub_44D860();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44D860 @ 0x0044D860..0x0044D867 =====
void __thiscall sub_44D860(_DWORD *this)
{
  *this = &DCParticle::`vftable';
}

// ===== sub_44D870 @ 0x0044D870..0x0044D87B =====
void *sub_44D870()
{
  void *result; // eax

  result = dword_566750;
  dword_565D8C = (int)dword_566750;
  return result;
}

// ===== sub_44D880 @ 0x0044D880..0x0044DA1C =====
int __usercall sub_44D880@<eax>(unsigned int *a1@<eax>, unsigned int a2, _DWORD *a3, unsigned int a4, unsigned int a5)
{
  unsigned int v5; // esi
  _DWORD *v7; // ebx
  int v8; // eax
  unsigned int v9; // edx
  BOOL v10; // ecx
  _DWORD *v11; // eax
  int v12; // edi
  unsigned int v13; // edx
  unsigned int v14; // ebx
  unsigned int v16; // [esp+Ch] [ebp-14h]
  unsigned int v17; // [esp+10h] [ebp-10h]
  int v18; // [esp+14h] [ebp-Ch]
  int v19; // [esp+18h] [ebp-8h]
  _DWORD *v20; // [esp+1Ch] [ebp-4h]

  v5 = a2;
  if ( a2 && (v7 = a3) != 0 )
  {
    if ( a2 > 0x20 )
    {
      return -2147483644;
    }
    else
    {
      v8 = a3[4];
      if ( v8 == 1 || v8 == 2 )
      {
        v9 = 0;
        v10 = 1;
        v11 = a3 + 3;
        while ( v10 )
        {
          v10 = a3[2] == *(v11 - 1) && a3[3] == *v11 && a3[4] == v11[1];
          ++v9;
          v11 += 6;
          if ( v9 >= a2 )
          {
            if ( !v10 )
              return -2147483645;
            sub_44DA20(a1);
            *a1 = a2;
            a1[33] = a4;
            a1[34] = a5;
            v18 = 0;
            v20 = a1 + 1;
            while ( 1 )
            {
              *v20 = operator new[](24 * v5);
              v12 = ((32 - v18) << 11) & 0x7FFFFFF;
              v13 = (unsigned int)(v12 * a3[3]) >> 16;
              v14 = (unsigned int)(v12 * v7[2]) >> 16;
              v16 = v13;
              if ( v5 )
              {
                v19 = 0;
                v17 = v5;
                while ( 1 )
                {
                  if ( sub_409030(a3[4], v13, (_DWORD *)(v19 * 4 + *v20), v14) )
                    sub_44DC10(&a3[v19]);
                  v19 += 6;
                  if ( !--v17 )
                    break;
                  v13 = v16;
                }
                v5 = a2;
              }
              ++v20;
              if ( (unsigned int)++v18 >= 0x20 )
                break;
              v7 = a3;
            }
            return 0;
          }
        }
      }
      return -2147483645;
    }
  }
  else
  {
    sub_44DA20(a1);
    return 0;
  }
}

// ===== sub_44DA20 @ 0x0044DA20..0x0044DA8A =====
void *__cdecl sub_44DA20(char *a1)
{
  void **v1; // ebx
  unsigned int v2; // edi
  int v3; // esi
  int v5; // [esp+Ch] [ebp-4h]

  v1 = (void **)(a1 + 4);
  v5 = 32;
  do
  {
    v2 = 0;
    if ( *(_DWORD *)a1 )
    {
      v3 = 0;
      do
      {
        if ( *(_DWORD *)((char *)*v1 + v3) )
          operator delete[](*(void **)((char *)*v1 + v3));
        ++v2;
        v3 += 24;
      }
      while ( v2 < *(_DWORD *)a1 );
    }
    operator delete[](*v1++);
    --v5;
  }
  while ( v5 );
  return memset(a1, 0, 0x8Cu);
}

// ===== sub_44DA90 @ 0x0044DA90..0x0044DABD =====
int __usercall sub_44DA90@<eax>(int a1@<edi>)
{
  int v1; // esi
  int result; // eax

  v1 = rand() << 15;
  result = (v1 | rand()) % (int)abs32(a1 + 1);
  if ( a1 < 0 )
    return -result;
  return result;
}

// ===== sub_44DAC0 @ 0x0044DAC0..0x0044DAD2 =====
_DWORD *__usercall sub_44DAC0@<eax>(_DWORD *result@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // edx
  int v3; // ecx

  *result = a2[4];
  v2 = a2[5];
  v3 = a2[6];
  result[1] = v2;
  result[2] = v3;
  return result;
}

// ===== sub_44DAE0 @ 0x0044DAE0..0x0044DAE5 =====
int __stdcall sub_44DAE0(int a1)
{
  return 0;
}

// ===== sub_44DAF0 @ 0x0044DAF0..0x0044DAF4 =====
int __usercall sub_44DAF0@<eax>(int a1@<eax>)
{
  return *(_DWORD *)(a1 + 56);
}

// ===== sub_44DB00 @ 0x0044DB00..0x0044DB04 =====
int __thiscall sub_44DB00(_DWORD *this)
{
  return this[15];
}

// ===== sub_44DB10 @ 0x0044DB10..0x0044DB40 =====
int __fastcall sub_44DB10(_DWORD *a1, _DWORD *a2)
{
  int result; // eax

  result = a1[3];
  if ( result )
  {
    a1[4] += (a1[7] * *a2) >> 8;
    a1[5] += (a1[7] * a2[1]) >> 8;
    a1[6] += (a1[7] * a2[2]) >> 8;
  }
  return result;
}

// ===== sub_44DB40 @ 0x0044DB40..0x0044DB95 =====
int __thiscall sub_44DB40(_DWORD *this)
{
  unsigned int v1; // edx
  unsigned int v2; // eax
  _DWORD *i; // esi
  unsigned int v4; // edx

  this[8] += this[10];
  this[9] += this[11];
  v1 = this[8];
  v2 = this[9];
  for ( i = (_DWORD *)this[13]; v1 >= *i << 16; this[8] = v1 )
    v1 -= *i << 16;
  v4 = this[12] << 16;
  if ( v2 >= v4 )
  {
    do
    {
      v2 -= v4;
      this[9] = v2;
    }
    while ( v2 >= this[12] << 16 );
  }
  return this[3];
}

// ===== sub_44DBA0 @ 0x0044DBA0..0x0044DBA4 =====
int __usercall sub_44DBA0@<eax>(int result@<eax>, int a2@<ecx>)
{
  *(_DWORD *)(result + 56) = a2;
  return result;
}

// ===== sub_44DBB0 @ 0x0044DBB0..0x0044DBB8 =====
int __usercall sub_44DBB0@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 60) = 0;
  return result;
}

// ===== sub_44DBC0 @ 0x0044DBC0..0x0044DBC5 =====
int __usercall sub_44DBC0@<eax>(int a1@<eax>)
{
  return *(unsigned __int16 *)(a1 + 34);
}

// ===== sub_44DBD0 @ 0x0044DBD0..0x0044DBD5 =====
int __usercall sub_44DBD0@<eax>(int a1@<eax>)
{
  return *(unsigned __int16 *)(a1 + 38);
}

// ===== sub_44DBE0 @ 0x0044DBE0..0x0044DC07 =====
int __userpurge sub_44DBE0@<eax>(int *a1@<esi>, int a2)
{
  int result; // eax

  *(_DWORD *)(a2 + 44) = a1[33] + sub_44DA90(a1[34]);
  result = *a1;
  *(_DWORD *)(a2 + 48) = *a1;
  return result;
}

// ===== sub_44DC10 @ 0x0044DC10..0x0044DC23 =====
int __usercall sub_44DC10@<eax>(int a1@<eax>, int a2)
{
  return sub_494D90(a2, a1, a1);
}

// ===== sub_44DC30 @ 0x0044DC30..0x0044DD30 =====
int __stdcall sub_44DC30(int a1)
{
  _DWORD *v1; // eax
  int v2; // edx
  int v3; // ecx
  void *v4; // eax
  int v5; // eax
  _DWORD *v6; // eax
  int v7; // ecx

  *(_DWORD *)a1 = &DCParticleControl::`vftable';
  *(_DWORD *)(a1 + 8) = 10;
  *(_DWORD *)(a1 + 12) = sub_498720();
  *(_DWORD *)(a1 + 16) = 0;
  memset((void *)(a1 + 20), 0, 0x20000u);
  v1 = (_DWORD *)(a1 + 131604);
  v2 = 2;
  do
  {
    v3 = 64;
    do
    {
      *(v1 - 128) = 0;
      *v1 = 0;
      v1[128] = 0;
      v1[256] = 0;
      ++v1;
      --v3;
    }
    while ( v3 );
    --v2;
  }
  while ( v2 );
  v4 = operator new(0x74u);
  if ( v4 )
    v5 = sub_44F140(v4);
  else
    v5 = 0;
  *(_DWORD *)(a1 + 136788) = v5;
  v6 = (_DWORD *)(a1 + 133208);
  v7 = 64;
  do
  {
    *(v6 - 1) = 0;
    *v6 = 0;
    v6[1] = 0;
    v6[2] = 0;
    v6[9] = 1;
    v6[10] = 0;
    v6 += 14;
    --v7;
  }
  while ( v7 );
  InitializeCriticalSection((LPCRITICAL_SECTION)(a1 + 136840));
  return a1;
}

// ===== sub_44DD30 @ 0x0044DD30..0x0044DD51 =====
void *__thiscall sub_44DD30(void *this, char a2)
{
  sub_44DD60();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44DD60 @ 0x0044DD60..0x0044DD90 =====
void __thiscall sub_44DD60(char *this)
{
  void (__thiscall ***v2)(_DWORD, int); // ecx

  *(_DWORD *)this = &DCParticleControl::`vftable';
  sub_44E260(this);
  v2 = (void (__thiscall ***)(_DWORD, int))*((_DWORD *)this + 34197);
  if ( v2 )
    (**v2)(v2, 1);
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 136840));
}

// ===== sub_44DD90 @ 0x0044DD90..0x0044DD9B =====
void *sub_44DD90()
{
  void *result; // eax

  result = dword_566750;
  dword_565D90 = (int)dword_566750;
  return result;
}

// ===== sub_44DDA0 @ 0x0044DDA0..0x0044DDAB =====
int sub_44DDA0()
{
  int result; // eax

  result = dword_5666F8;
  dword_565D94 = dword_5666F8;
  return result;
}

// ===== sub_44DDB0 @ 0x0044DDB0..0x0044DDCE =====
int sub_44DDB0()
{
  int result; // eax

  if ( !dword_565D98 )
  {
    sub_450120();
    result = sub_44F670();
    dword_565D98 = 1;
  }
  return result;
}

// ===== sub_44DDD0 @ 0x0044DDD0..0x0044DDDA =====
void *sub_44DDD0()
{
  int v0; // edi
  unsigned int i; // esi
  void *result; // eax

  sub_450250();
  v0 = 0;
  for ( i = 0; i < 64; ++i )
  {
    sub_44DA20(&byte_50FED8[v0 * 4]);
    dword_50EBD8[i] = 0;
    result = sub_44DA20((char *)&dword_5121D8[v0]);
    v0 += 35;
  }
  return result;
}

// ===== sub_44DDE0 @ 0x0044DDE0..0x0044DE5A =====
int __usercall sub_44DDE0@<eax>(int a1@<edx>, int a2@<edi>, int a3@<esi>, unsigned int a4)
{
  unsigned int v4; // eax

  if ( a4 >= 2 )
    return -2147483647;
  if ( a4 )
    v4 = sub_44F7E0(a2, a3, a1);
  else
    v4 = sub_450210(a2, a3, a1);
  if ( v4 > 0x80000003 )
  {
    if ( v4 == -2147483644 )
      return -2147483642;
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000003:
        return -2147483643;
      case 0u:
        return 0;
      case 0x80000001:
        return -2147483646;
    }
  }
  return a4;
}

// ===== sub_44DE60 @ 0x0044DE60..0x0044DEBD =====
int __usercall sub_44DE60@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  unsigned int v4; // eax

  if ( a3 != 1 )
    return -2147483647;
  v4 = sub_44F820(a4, a2, a1);
  if ( v4 > 0x80000003 )
  {
    if ( v4 == -2147483644 )
      return -2147483642;
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000003:
        return -2147483643;
      case 0u:
        return 0;
      case 0x80000001:
        return -2147483646;
    }
  }
  return 1;
}

// ===== sub_44DEC0 @ 0x0044DEC0..0x0044DF02 =====
int __usercall sub_44DEC0@<eax>(int a1@<eax>, int a2)
{
  int v2; // eax

  if ( a2 != 1 )
    return -2147483647;
  v2 = sub_44F860(a1);
  switch ( v2 )
  {
    case 0:
      return 0;
    case -2147483647:
      return -2147483646;
    case -2147483643:
      return -2147483641;
  }
  return 1;
}

// ===== sub_44DF10 @ 0x0044DF10..0x0044DF41 =====
int __fastcall sub_44DF10(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
  return sub_450170(a5, a6, a7, a8, a9, a10, a2, a1, a11);
}

// ===== sub_44DF50 @ 0x0044DF50..0x0044DFA3 =====
int __cdecl sub_44DF50(
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
        int a17,
        int a18)
{
  return sub_44F6F0(a2, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
}

// ===== sub_44DFB0 @ 0x0044DFB0..0x0044DFC4 =====
int __userpurge sub_44DFB0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_44E800(0, a3, a2, a1);
}

// ===== sub_44DFD0 @ 0x0044DFD0..0x0044DFE4 =====
int __userpurge sub_44DFD0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_44E800(1, a3, a2, a1);
}

// ===== sub_44DFF0 @ 0x0044DFF0..0x0044E0E5 =====
int __userpurge sub_44DFF0@<eax>(
        int a1@<eax>,
        unsigned int a2@<esi>,
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
        int a16)
{
  _DWORD *v16; // ecx

  if ( a2 >= 0x40 )
    return -2147483646;
  *(_DWORD *)(a3 + 56 * a2 + 133204) = a1 >> 8;
  v16 = (_DWORD *)(a3 + 56 * a2);
  v16[33302] = (int)abs32(a4) >> 8;
  v16[33304] = a6;
  v16[33305] = (int)abs32(a7) >> 8;
  v16[33303] = a5 >> 8;
  *(_DWORD *)(a3 + 56 * (a2 + 2379)) = (int)abs32(a8) >> 8;
  v16[33307] = abs32(a9);
  v16[33308] = abs32(a10);
  v16[33309] = abs32(a11);
  v16[33311] = a13;
  v16[33310] = a12;
  v16[33312] = a14;
  v16[33313] = a15 >> 8;
  v16[33314] = (int)abs32(a16) >> 8;
  return 0;
}

// ===== sub_44E110 @ 0x0044E110..0x0044E14E =====
int __fastcall sub_44E110(
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
  return sub_44F250(a4, a5, a6, a7, a8, a11, a2, a12, a13);
}

// ===== sub_44E150 @ 0x0044E150..0x0044E225 =====
BOOL __userpurge sub_44E150@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<esi>, int a4, int a5, int a6, int a7, int a8)
{
  long double v9; // [esp+8h] [ebp-8h]
  long double v10; // [esp+8h] [ebp-8h]
  long double v11; // [esp+8h] [ebp-8h]

  if ( a8 > 0 )
  {
    *(_DWORD *)(a3 + 133140) = a4 >> 8;
    *(_DWORD *)(a3 + 133144) = a2 >> 8;
    *(_DWORD *)(a3 + 133148) = a1 >> 8;
    v9 = (double)a5 * 3.141592653589793 / 11796480.0;
    *(long double *)(a3 + 133152) = sin(v9);
    *(long double *)(a3 + 133160) = cos(v9);
    v10 = (double)a6 * 3.141592653589793 / 11796480.0;
    *(long double *)(a3 + 133168) = sin(v10);
    *(long double *)(a3 + 133176) = cos(v10);
    v11 = (double)a7 * 3.141592653589793 / 11796480.0;
    *(long double *)(a3 + 133184) = sin(v11);
    *(long double *)(a3 + 133192) = cos(v11);
    *(_DWORD *)(a3 + 133200) = a8;
  }
  return a8 > 0;
}

// ===== sub_44E230 @ 0x0044E230..0x0044E252 =====
int __userpurge sub_44E230@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  return sub_44E9E0(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_44E260 @ 0x0044E260..0x0044E2BD =====
_DWORD *__stdcall sub_44E260(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // edi
  _DWORD *result; // eax
  int v4; // edx
  int v5; // ecx

  v1 = a1 + 5;
  v2 = 0x8000;
  do
  {
    if ( *v1 )
    {
      (**(void (__thiscall ***)(_DWORD, int))*v1)(*v1, 1);
      *v1 = 0;
    }
    ++v1;
    --v2;
  }
  while ( v2 );
  a1[4] = 0;
  result = a1 + 33157;
  v4 = 2;
  do
  {
    v5 = 64;
    do
    {
      *(result - 256) = 0;
      *result++ = 0;
      --v5;
    }
    while ( v5 );
    --v4;
  }
  while ( v4 );
  return result;
}

// ===== sub_44E2C0 @ 0x0044E2C0..0x0044E2D6 =====
int __stdcall sub_44E2C0(int a1)
{
  int v1; // eax

  v1 = sub_498720();
  return sub_44E3F0(a1, v1);
}

// ===== sub_44E2E0 @ 0x0044E2E0..0x0044E371 =====
void __stdcall sub_44E2E0(_DWORD *a1, unsigned int a2)
{
  unsigned int v2; // esi
  int i; // edi
  int v4; // [esp+4h] [ebp-20Ch]
  _BYTE v5[512]; // [esp+Ch] [ebp-204h] BYREF

  if ( a1[2] )
  {
    v4 = a1[3];
    qmemcpy(v5, a1 + 33157, sizeof(v5));
    v2 = 1;
    for ( i = sub_498720(); v2 <= a2; ++v2 )
      sub_44E3F0(a1, i + v2);
    a1[3] = v4;
    qmemcpy(a1 + 33157, v5, 0x200u);
  }
}

// ===== sub_44E380 @ 0x0044E380..0x0044E3E3 =====
int __stdcall sub_44E380(int a1, unsigned int a2)
{
  int v2; // edx
  _DWORD *v3; // esi
  int v4; // edi

  if ( a1 != 1 )
    return -2147483647;
  if ( a2 >= 0x40 )
    return -2147483646;
  if ( sub_44F890(a2) )
  {
    v3 = (_DWORD *)(v2 + 20);
    v4 = 0x8000;
    do
    {
      if ( *v3 )
      {
        if ( sub_490AE0() == 1 )
          sub_44FDD0();
      }
      ++v3;
      --v4;
    }
    while ( v4 );
  }
  return 0;
}

// ===== sub_44E3F0 @ 0x0044E3F0..0x0044E43B =====
int __stdcall sub_44E3F0(int a1, int a2)
{
  int result; // eax
  unsigned int v3; // esi
  BOOL v4; // ebx
  unsigned int i; // edi
  int v6; // [esp+4h] [ebp-4h] BYREF

  result = sub_44E440(&v6, a2);
  if ( result )
  {
    v3 = 0;
    v4 = v6 == 0;
    do
    {
      for ( i = 0; i < 0x40; ++i )
        result = sub_44E620(v3, i, a2, (void *)v4);
      ++v3;
    }
    while ( v3 < 2 );
  }
  return result;
}

// ===== sub_44E440 @ 0x0044E440..0x0044E4ED =====
BOOL __userpurge sub_44E440@<eax>(int a1@<esi>, _DWORD *a2, unsigned int a3)
{
  BOOL v4; // ecx
  bool v5; // zf
  int v6; // edi
  BOOL v8; // [esp+10h] [ebp+Ch]

  v4 = *(_DWORD *)(a1 + 8) != 0;
  v8 = v4;
  if ( !*(_DWORD *)(a1 + 8) )
    return v4;
  v5 = *(_DWORD *)(a1 + 12) + 500 < a3;
  *a2 = *(_DWORD *)(a1 + 12) + 500 >= a3;
  if ( v5 )
    *(_DWORD *)(a1 + 12) = a3;
  if ( *(_DWORD *)(a1 + 12) > a3 )
    return v4;
  do
  {
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(a1 + 136788) + 8))(*(_DWORD *)(a1 + 136788));
    sub_44F0F0();
    sub_44F010();
    v6 = dword_565D94;
    sub_4464C0(dword_565D94, (int)sub_44E610, a1);
    sub_4464E0(1, v6);
    sub_4464C0(dword_565D94, 0, 0);
    *(_DWORD *)(a1 + 12) += *(_DWORD *)(a1 + 8);
  }
  while ( *(_DWORD *)(a1 + 12) <= a3 );
  return v8;
}

// ===== sub_44E4F0 @ 0x0044E4F0..0x0044E60E =====
BOOL __stdcall sub_44E4F0(int a1)
{
  int v1; // edi
  int v2; // esi
  int v3; // ebx
  int i; // esi
  _DWORD *v5; // ecx
  int v6; // eax
  _DWORD *v7; // edi
  int v8; // eax
  int v9; // eax
  int v10; // edx
  void (__thiscall ***v11)(_DWORD, int); // ecx
  int v13; // [esp+Ch] [ebp-18h] BYREF
  int v14; // [esp+10h] [ebp-14h]
  int v15; // [esp+14h] [ebp-10h]
  int v16; // [esp+18h] [ebp-Ch]
  int v17; // [esp+1Ch] [ebp-8h]
  int v18; // [esp+20h] [ebp-4h]

  v1 = sub_4465D0(dword_565D94);
  v2 = sub_44F020();
  v17 = v2;
  sub_4465F0(dword_565D94, v1);
  if ( v2 >= 0 )
  {
    v3 = a1 + 4 * v2 + 20;
    for ( i = 0; i < v18; ++i )
    {
      v5 = *(_DWORD **)(v3 + 4 * i);
      if ( v5 )
      {
        sub_44DB10(v5, (_DWORD *)(a1 + 136816));
        v6 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(v3 + 4 * i) + 16))(*(_DWORD *)(v3 + 4 * i));
        v7 = *(_DWORD **)(v3 + 4 * i);
        v16 = v6;
        sub_44DAC0(&v13, v7);
        v13 >>= 8;
        v14 >>= 8;
        v8 = v15 >> 8;
        v15 >>= 8;
        if ( !v16 || (unsigned int)(v13 + 16000) > 0x7D00 || (unsigned int)v14 > 0x3E80 || v8 > 8000 || v8 < -8000 )
        {
          sub_490AE0();
          v9 = sub_42D560((int)v7);
          --*(_DWORD *)(a1 + 4 * (v9 + v10) + 131604);
          v11 = *(void (__thiscall ****)(_DWORD, int))(v3 + 4 * i);
          if ( v11 )
            (**v11)(v11, 1);
          *(_DWORD *)(v3 + 4 * i) = 0;
          --*(_DWORD *)(a1 + 16);
        }
      }
    }
    v2 = v17;
  }
  return v2 >= 0;
}

// ===== sub_44E610 @ 0x0044E610..0x0044E61E =====
BOOL __cdecl sub_44E610(int a1)
{
  return sub_44E4F0(a1);
}

// ===== sub_44E620 @ 0x0044E620..0x0044E7BD =====
int __thiscall sub_44E620(_DWORD *this, unsigned int a2, unsigned int a3, unsigned int a4, void *a5)
{
  unsigned int v6; // ecx
  int result; // eax
  unsigned int v8; // edi
  int v9; // ebx
  void *v10; // eax
  void *v11; // eax
  int v12; // eax
  int v13; // [esp+10h] [ebp-10h]

  v6 = a2;
  result = 0;
  v13 = 0;
  if ( a2 < 2 && a3 < 0x40 )
  {
    if ( a5 )
      this[64 * a2 + 33157 + a3] = a4;
    v8 = a3 + (a2 << 6);
    if ( this[v8 + 33157] <= a4 )
    {
      while ( this[v8 + 32901] < this[v8 + 32773] )
      {
        v9 = 0;
        if ( v6 )
        {
          if ( v6 == 1 )
          {
            v10 = operator new(0x78u);
            v9 = v10 ? sub_44F470(v10, &this[14 * a3 + 33301]) : 0;
            if ( sub_44F890(a3) )
              sub_44FDD0(v9);
          }
        }
        else
        {
          v11 = operator new(0x4Cu);
          if ( v11 )
            v12 = sub_44FF90(v11, a3);
          else
            v12 = 0;
          v9 = v12;
        }
        sub_44E7C0(v9);
        ++this[v8 + 32901];
        if ( !this[v8 + 33157] )
          this[v8 + 33157] = a4;
        ++v13;
        this[v8 + 33157] += this[v8 + 33029];
        result = v13;
        if ( this[v8 + 33157] > a4 )
          return result;
        v6 = a2;
      }
      this[v8 + 33157] = 0;
    }
  }
  return result;
}

// ===== sub_44E7C0 @ 0x0044E7C0..0x0044E7FD =====
int __userpurge sub_44E7C0@<eax>(int a1@<esi>, int a2)
{
  int result; // eax
  int v3; // ecx
  _DWORD *i; // edx

  result = 0;
  if ( *(_DWORD *)(a1 + 16) + 1 <= 0x8000 )
  {
    v3 = 0;
    for ( i = (_DWORD *)(a1 + 20); *i; ++i )
    {
      if ( ++v3 >= 0x8000 )
        return result;
    }
    *(_DWORD *)(a1 + 4 * v3 + 20) = a2;
    ++*(_DWORD *)(a1 + 16);
    return 1;
  }
  return result;
}

// ===== sub_44E800 @ 0x0044E800..0x0044E8D9 =====
int __userpurge sub_44E800@<eax>(int a1@<esi>, int a2, unsigned int a3, int a4, int a5)
{
  int v5; // ecx
  int v6; // edi
  unsigned int i; // edx
  unsigned int j; // eax
  unsigned int v9; // eax
  int v11; // [esp+0h] [ebp-4h]

  if ( a3 >= 0x40 )
    return -2147483646;
  v5 = 0;
  v6 = 0;
  v11 = 0;
  for ( i = 32774; i < 0x8086; i += 64 )
  {
    for ( j = 0; j < 0x40; j += 4 )
    {
      if ( v6 != a2 || j != a3 )
        v5 += *(_DWORD *)(a1 + 4 * (i + j) - 4);
      if ( v6 != a2 || j + 1 != a3 )
        v5 += *(_DWORD *)(a1 + 4 * (i + j));
      if ( v11 != a2 || j + 2 != a3 )
        v5 += *(_DWORD *)(a1 + 4 * (i + j) + 4);
      if ( v11 != a2 || j + 3 != a3 )
        v5 += *(_DWORD *)(a1 + 4 * (i + j) + 8);
      v6 = v11;
    }
    v6 = ++v11;
  }
  if ( a4 < 0 || a4 + v5 > 0x8000 )
    return -2147483645;
  v9 = a3 + (a2 << 6);
  *(_DWORD *)(a1 + 4 * v9 + 131092) = a4;
  *(_DWORD *)(a1 + 4 * v9 + 132116) = a5;
  return 0;
}

// ===== sub_44E8E0 @ 0x0044E8E0..0x0044E9D4 =====
void __stdcall sub_44E8E0(int a1, unsigned int a2)
{
  int i; // ebx
  unsigned int v3; // edx
  int v4; // edi
  unsigned int j; // esi
  _DWORD *v6; // eax
  _DWORD *k; // eax
  int *v8; // eax
  int *v9; // ecx
  int v10; // [esp+0h] [ebp-14h]
  int v11; // [esp+4h] [ebp-10h]
  int v12; // [esp+8h] [ebp-Ch]
  int v13; // [esp+Ch] [ebp-8h]

  if ( a2 >= 2 )
  {
    for ( i = a1; ; a1 = i )
    {
      v3 = a2;
      v4 = *(_DWORD *)(i + 16 * (a2 >> 1) + 8);
      for ( j = 0; ; ++j )
      {
        v6 = (_DWORD *)(i + 16 * j + 8);
        --v3;
        for ( ; *v6 > v4; ++j )
          v6 += 4;
        for ( k = (_DWORD *)(i + 16 * v3 + 8); *k < v4; --v3 )
          k -= 4;
        if ( j >= v3 )
          break;
        v8 = (int *)(i + 16 * j);
        v10 = *v8;
        v11 = v8[1];
        v12 = v8[2];
        v13 = v8[3];
        v9 = (int *)(i + 16 * v3);
        *v8 = *v9;
        v8[1] = v9[1];
        v8[2] = v9[2];
        v8[3] = v9[3];
        i = a1;
        *v9 = v10;
        v9[1] = v11;
        v9[2] = v12;
        v9[3] = v13;
      }
      if ( j >= 2 )
        sub_44E8E0(i, j);
      if ( a2 - j < 2 )
        break;
      i += 16 * j;
      a2 -= j;
    }
  }
}

// ===== sub_44E9E0 @ 0x0044E9E0..0x0044EAD3 =====
void __userpurge sub_44E9E0(_DWORD *a1@<esi>, int a2, int a3, int a4, int a5, int a6, _DWORD *a7, int a8)
{
  int v8; // edi
  int v9; // edi
  _DWORD v10[7]; // [esp+4h] [ebp-1Ch] BYREF

  a1[34207] = 0;
  a1[34208] = operator new[](0x80000u);
  sub_44F010();
  v8 = dword_565D94;
  sub_4464C0(dword_565D94, (int)sub_44ED30, (int)a1);
  sub_4464E0(1, v8);
  v9 = dword_565D94;
  sub_4464C0(dword_565D94, 0, 0);
  sub_44E8E0(a1[34208], a1[34207]);
  v10[2] = a3;
  v10[1] = a2;
  v10[3] = a4;
  v10[5] = a6;
  a1[34209] = 0;
  v10[0] = a1;
  v10[4] = a5;
  v10[6] = a8;
  sub_44F060();
  sub_4464C0(v9, (int)sub_44EFF0, (int)v10);
  sub_4464E0(1, v9);
  sub_4464C0(dword_565D94, 0, 0);
  *a7 = a1[34209];
  operator delete[]((void *)a1[34208]);
}

// ===== sub_44EAE0 @ 0x0044EAE0..0x0044ED27 =====
BOOL __usercall sub_44EAE0@<eax>(int a1@<edi>)
{
  int v1; // ebx
  int v2; // esi
  int v3; // ecx
  int v4; // ebx
  _DWORD *v5; // ebx
  int v6; // eax
  int v7; // ecx
  double v8; // st7
  double v9; // st6
  double v10; // st5
  double v11; // rt0
  double v12; // st6
  double v13; // st7
  double v14; // st6
  int v15; // esi
  double v16; // st7
  double v17; // st6
  double v18; // st4
  double v19; // st6
  double v20; // st5
  int *v21; // ecx
  _DWORD v23[3]; // [esp+8h] [ebp-4030h] BYREF
  int v24; // [esp+14h] [ebp-4024h]
  int v25; // [esp+18h] [ebp-4020h]
  int v26; // [esp+1Ch] [ebp-401Ch]
  int *v27; // [esp+20h] [ebp-4018h]
  int v28; // [esp+24h] [ebp-4014h]
  int v29; // [esp+28h] [ebp-4010h]
  int v30; // [esp+2Ch] [ebp-400Ch]
  int v31; // [esp+30h] [ebp-4008h]
  _BYTE Src[16384]; // [esp+34h] [ebp-4004h] BYREF

  v1 = sub_4465D0(dword_565D94);
  v2 = sub_44F020();
  v26 = v2;
  sub_4465F0(dword_565D94, v1);
  if ( v2 >= 0 )
  {
    v3 = 0;
    v4 = 0;
    v29 = 0;
    v25 = 0;
    if ( v24 > 0 )
    {
      v27 = (int *)Src;
      while ( 1 )
      {
        v5 = *(_DWORD **)(a1 + 4 * v2 + 20 + 4 * v3);
        if ( v5 )
        {
          sub_44DAC0(v23, v5);
          v6 = v23[2] - *(_DWORD *)(a1 + 133148);
          v7 = v23[0] - *(_DWORD *)(a1 + 133140);
          v30 = v23[1] - *(_DWORD *)(a1 + 133144);
          v8 = (double)v30;
          v28 = v6;
          v9 = (double)v6;
          v31 = v7;
          v10 = *(double *)(a1 + 133160) * v8 - v9 * *(double *)(a1 + 133152);
          v11 = v9;
          v12 = v8 * *(double *)(a1 + 133152);
          v30 = (int)v10;
          v28 = (int)(v11 * *(double *)(a1 + 133160) + v12);
          v13 = (double)v28;
          v14 = (double)v7;
          v15 = (int)(*(double *)(a1 + 133176) * v13 - *(double *)(a1 + 133168) * v14);
          v31 = (int)(v13 * *(double *)(a1 + 133168) + v14 * *(double *)(a1 + 133176));
          v16 = (double)v31;
          v17 = (double)(int)v10;
          v18 = *(double *)(a1 + 133184) * v17;
          v19 = v17 * *(double *)(a1 + 133192);
          v20 = *(double *)(a1 + 133184);
          v31 = (int)(v16 * *(double *)(a1 + 133192) - v18);
          v30 = (int)(v16 * v20 + v19);
          if ( v15 >= 0 )
          {
            ++v29;
            v21 = v27;
            v27 += 4;
            *v21 = v31;
            v21[1] = (int)(v16 * v20 + v19);
            v21[2] = v15;
            v21[3] = (int)v5;
          }
        }
        v3 = v25 + 1;
        v25 = v3;
        if ( v3 >= v24 )
          break;
        v2 = v26;
      }
      v4 = v29;
    }
    EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 136840));
    memcpy_0((void *)(*(_DWORD *)(a1 + 136832) + 16 * *(_DWORD *)(a1 + 136828)), Src, 16 * v4);
    *(_DWORD *)(a1 + 136828) += v4;
    LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 136840));
  }
  return v26 >= 0;
}

// ===== sub_44ED30 @ 0x0044ED30..0x0044ED3F =====
BOOL __cdecl sub_44ED30(int a1)
{
  return sub_44EAE0(a1);
}

// ===== sub_44ED40 @ 0x0044ED40..0x0044EFF0 =====
int __userpurge sub_44ED40@<eax>(int a1@<esi>, _DWORD *a2)
{
  int v2; // ebx
  int v3; // edi
  int v4; // ebx
  int v5; // edi
  int v6; // ecx
  int v7; // eax
  int v8; // ecx
  int v9; // edi
  int v10; // ebx
  int v11; // eax
  int v12; // edi
  _DWORD *i; // edx
  int v14; // eax
  _DWORD *v15; // eax
  int v16; // edx
  int v17; // ecx
  int v18; // edx
  int v19; // ecx
  int v21; // [esp-4h] [ebp-5Ch]
  int *v22; // [esp+Ch] [ebp-4Ch]
  int v23; // [esp+10h] [ebp-48h]
  int v24; // [esp+14h] [ebp-44h]
  int v25; // [esp+14h] [ebp-44h]
  int v26; // [esp+18h] [ebp-40h]
  _DWORD *Src; // [esp+1Ch] [ebp-3Ch]
  _DWORD *v28; // [esp+20h] [ebp-38h]
  int v29; // [esp+24h] [ebp-34h]
  unsigned int v30; // [esp+2Ch] [ebp-2Ch]
  int v31; // [esp+30h] [ebp-28h]
  int v32; // [esp+34h] [ebp-24h]
  int v33[4]; // [esp+38h] [ebp-20h] BYREF
  int v34[4]; // [esp+48h] [ebp-10h] BYREF

  v2 = sub_4465D0(dword_565D94);
  v3 = sub_44F070(a1);
  sub_4465F0(dword_565D94, v2);
  v26 = 0;
  v4 = sub_490AE0();
  v31 = v4;
  Src = operator new[](16 * ((v4 + *(_DWORD *)(a1 + 136828) - 1) / v4));
  sub_409190(v34, a2[2]);
  v29 = v3;
  if ( v3 < *(_DWORD *)(a1 + 136828) )
  {
    v5 = 16 * v3;
    v23 = v5;
    v32 = 16 * v4;
    v28 = Src;
    do
    {
      v22 = (int *)(*(int (__thiscall **)(_DWORD, int))(**(_DWORD **)(v5 + *(_DWORD *)(a1 + 136832) + 12) + 4))(
                     *(_DWORD *)(v5 + *(_DWORD *)(a1 + 136832) + 12),
                     32
                   - 32
                   * *(_DWORD *)(a1 + 133200)
                   / (*(_DWORD *)(a1 + 133200) + (*(int *)(v5 + *(_DWORD *)(a1 + 136832) + 8) >> 8)));
      if ( v22 )
      {
        v6 = *(_DWORD *)(a1 + 136832);
        v7 = *(_DWORD *)(v6 + v5);
        v8 = v5 + v6;
        v9 = *(_DWORD *)(a1 + 133200);
        v24 = *(_DWORD *)(v8 + 8) + (v9 << 8);
        v10 = a2[4] + v9 * v7 / v24 - ((unsigned int)v22[2] >> 1);
        v11 = v9 * *(_DWORD *)(v8 + 4) / v24;
        v25 = 0;
        v12 = a2[5] - v11 - ((unsigned int)v22[3] >> 1);
        v30 = a2[1];
        if ( v30 )
        {
          for ( i = (_DWORD *)a2[3]; *(_DWORD *)(v8 + 8) > *i; ++i )
          {
            if ( ++v25 >= v30 )
              goto LABEL_12;
          }
          v21 = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)(v8 + 12) + 12))(*(_DWORD *)(v8 + 12));
          v14 = sub_44DAF0(*(_DWORD *)(v23 + *(_DWORD *)(a1 + 136832) + 12));
          if ( !sub_40A530(v22, (_DWORD *)(a2[2] + 24 * v25), v12, v10, v14, v21) )
          {
            v15 = sub_409190(v33, (int)v22);
            sub_409170(v12, v10, v15);
            sub_409110(v33, v34);
            v16 = v33[1];
            ++v26;
            *v28 = v33[0];
            v17 = v33[2];
            v28[1] = v16;
            v18 = v33[3];
            v28[2] = v17;
            v28[3] = v18;
            v28 += 4;
          }
        }
LABEL_12:
        v19 = *(_DWORD *)(v23 + *(_DWORD *)(a1 + 136832) + 12);
        (*(void (__thiscall **)(int))(*(_DWORD *)v19 + 8))(v19);
        v5 = v23;
      }
      v5 += v32;
      v29 += v31;
      v23 = v5;
    }
    while ( v29 < *(_DWORD *)(a1 + 136828) );
  }
  EnterCriticalSection((LPCRITICAL_SECTION)(a1 + 136840));
  memcpy_0((void *)(a2[6] + 16 * *(_DWORD *)(a1 + 136836)), Src, 16 * v26);
  *(_DWORD *)(a1 + 136836) += v26;
  LeaveCriticalSection((LPCRITICAL_SECTION)(a1 + 136840));
  operator delete[](Src);
  return 0;
}

// ===== sub_44EFF0 @ 0x0044EFF0..0x0044F002 =====
int __cdecl sub_44EFF0(int *a1)
{
  return sub_44ED40(*a1, a1);
}

// ===== sub_44F010 @ 0x0044F010..0x0044F01B =====
int __usercall sub_44F010@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 136864) = 0;
  return result;
}

// ===== sub_44F020 @ 0x0044F020..0x0044F052 =====
int __usercall sub_44F020@<eax>(int a1@<edx>, int *a2@<esi>)
{
  int result; // eax
  int v3; // ecx

  result = *(_DWORD *)(a1 + 136864);
  if ( result >= 0x8000 )
    return -1;
  v3 = 0x8000 - result;
  if ( 0x8000 - result >= 1024 )
    v3 = 1024;
  *a2 = v3;
  *(_DWORD *)(a1 + 136864) = result + v3;
  return result;
}

// ===== sub_44F060 @ 0x0044F060..0x0044F06B =====
int __usercall sub_44F060@<eax>(int result@<eax>)
{
  *(_DWORD *)(result + 136868) = 0;
  return result;
}

// ===== sub_44F070 @ 0x0044F070..0x0044F080 =====
int __thiscall sub_44F070(_DWORD *this)
{
  int result; // eax

  result = this[34217];
  this[34217] = result + 1;
  return result;
}

// ===== sub_44F080 @ 0x0044F080..0x0044F098 =====
_DWORD *__usercall sub_44F080@<eax>(_DWORD *result@<eax>)
{
  *result = &DCParticleEnv::`vftable';
  result[1] = 0;
  result[2] = 0;
  result[3] = 0;
  result[4] = 0;
  result[5] = 0;
  return result;
}

// ===== sub_44F0A0 @ 0x0044F0A0..0x0044F0C1 =====
void *__thiscall sub_44F0A0(void *this, char a2)
{
  sub_44F0D0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44F0D0 @ 0x0044F0D0..0x0044F0D7 =====
void __thiscall sub_44F0D0(_DWORD *this)
{
  *this = &DCParticleEnv::`vftable';
}

// ===== sub_44F0E0 @ 0x0044F0E0..0x0044F0EA =====
int __usercall sub_44F0E0@<eax>(int a1@<eax>, int *a2@<ecx>)
{
  int v2; // edx

  v2 = *a2;
  a2[1] = a1;
  return (*(int (**)(void))(v2 + 4))();
}

// ===== sub_44F0F0 @ 0x0044F0F0..0x0044F119 =====
int __usercall sub_44F0F0@<eax>(_DWORD *a1@<eax>, _DWORD *a2@<edx>)
{
  if ( a2[2] )
  {
    *a1 = a2[3];
    a1[1] = a2[4];
    a1[2] = a2[5];
  }
  else
  {
    *a1 = 0;
    a1[1] = 0;
    a1[2] = 0;
  }
  return a2[2];
}

// ===== sub_44F120 @ 0x0044F120..0x0044F136 =====
_DWORD *__userpurge sub_44F120@<eax>(_DWORD *result@<eax>, int a2@<ecx>, int a3, int a4)
{
  result[3] = a2;
  result[4] = a3;
  result[5] = a4;
  return result;
}

// ===== sub_44F140 @ 0x0044F140..0x0044F18F =====
_DWORD *__stdcall sub_44F140(_DWORD *a1)
{
  _DWORD *result; // eax

  result = sub_44F080(a1);
  *result = &DCParticleEnvAir::`vftable';
  return result;
}

// ===== sub_44F190 @ 0x0044F190..0x0044F1B1 =====
void *__thiscall sub_44F190(void *this, char a2)
{
  sub_44F1C0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44F1C0 @ 0x0044F1C0..0x0044F207 =====
void __thiscall sub_44F1C0(_DWORD *this)
{
  *this = &DCParticleEnvAir::`vftable';
  sub_44F0D0(this);
}

// ===== sub_44F210 @ 0x0044F210..0x0044F24C =====
BOOL __thiscall sub_44F210(_DWORD *this)
{
  unsigned int v1; // esi

  v1 = this[1];
  if ( v1 )
  {
    this[16] = this[12] / v1;
    this[17] = this[13] / v1;
    this[18] = this[14] / v1;
    this[19] = this[15] / v1;
  }
  return v1 != 0;
}

// ===== sub_44F250 @ 0x0044F250..0x0044F2D8 =====
int __userpurge sub_44F250@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        _DWORD *a3@<esi>,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12)
{
  a3[6] = a5 >> 8;
  a3[11] = a1 >> 8;
  a3[7] = a7 >> 8;
  a3[10] = a8 >> 8;
  a3[8] = a2 >> 8;
  a3[12] = a9;
  a3[9] = a6 >> 8;
  a3[13] = a10;
  a3[14] = a11;
  a3[15] = a12;
  if ( a4 && (*(int (__thiscall **)(_DWORD *))(*a3 + 4))(a3) )
  {
    a3[2] = 1;
    return sub_44F3A0(1);
  }
  else
  {
    a3[2] = 0;
    return sub_44F3A0(1);
  }
}

// ===== sub_44F2E0 @ 0x0044F2E0..0x0044F3A0 =====
_DWORD *__thiscall sub_44F2E0(_DWORD *this)
{
  int v2; // eax
  int v3; // edi
  int v4; // ebx
  _DWORD *result; // eax
  int v6; // eax

  if ( !this[2] )
    return sub_44F120(this, 0, 0, 0);
  v2 = this[20];
  if ( v2 )
  {
    if ( v2 == 1 )
    {
      v3 = this[21];
      v4 = this[22];
      sub_44F120(
        this,
        ((v3 - v4) * this[23] + v4 * this[26]) / v3,
        ((v3 - v4) * this[24] + v4 * this[27]) / v3,
        ((v3 - v4) * this[25] + v4 * this[28]) / v3);
    }
  }
  else
  {
    sub_44F120(this, this[23], this[24], this[25]);
  }
  result = (_DWORD *)++this[22];
  if ( (unsigned int)result >= this[21] )
  {
    v6 = this[20];
    if ( v6 )
    {
      result = (_DWORD *)(v6 - 1);
      if ( !result )
        return (_DWORD *)sub_44F3A0(0);
    }
    else
    {
      sub_44DA90(this[19]);
      return (_DWORD *)sub_44F450(1);
    }
  }
  return result;
}

// ===== sub_44F3A0 @ 0x0044F3A0..0x0044F3FC =====
int __userpurge sub_44F3A0@<eax>(int *a1@<eax>, int a2)
{
  int v4; // edx
  int v5; // eax

  sub_44DA90(a1[17]);
  sub_44F450(0);
  if ( a2 )
  {
    sub_44F400(a1 + 23);
  }
  else
  {
    v4 = a1[27];
    v5 = a1[28];
    a1[23] = a1[26];
    a1[24] = v4;
    a1[25] = v5;
  }
  return sub_44F400(a1 + 26);
}

// ===== sub_44F400 @ 0x0044F400..0x0044F44C =====
int __userpurge sub_44F400@<eax>(_DWORD *a1@<esi>, _DWORD *a2)
{
  int result; // eax

  *a2 = a1[6] - a1[9] + sub_44DA90(2 * a1[9]);
  a2[1] = a1[7] - a1[10] + sub_44DA90(2 * a1[10]);
  result = a1[8] - a1[11] + sub_44DA90(2 * a1[11]);
  a2[2] = result;
  return result;
}

// ===== sub_44F450 @ 0x0044F450..0x0044F46E =====
_DWORD *__userpurge sub_44F450@<eax>(_DWORD *result@<eax>, int a2@<ecx>, int a3)
{
  result[20] = a3;
  result[22] = 0;
  if ( !a2 )
    a2 = 1;
  result[21] = a2;
  return result;
}

// ===== sub_44F470 @ 0x0044F470..0x0044F5DD =====
_DWORD *__fastcall sub_44F470(unsigned int a1, int a2, _DWORD *a3, int a4)
{
  int *v5; // ebx
  int v6; // eax
  _DWORD *v7; // edx
  _DWORD *v9; // [esp+28h] [ebp+Ch]

  sub_44D7E0(1, a1, a3, &byte_50FED8[140 * a1]);
  *a3 = &DCPFirefly::`vftable';
  if ( a1 < 0x40 )
  {
    v5 = &dword_50ECD8[18 * a1];
    if ( *v5 )
    {
      a3[16] = a4;
      v6 = sub_44DA90(v5[2]);
      v7 = (_DWORD *)a3[16];
      a3[17] = v5[1] + v6;
      a3[18] = 0;
      v9 = v7;
      a3[4] = *v7 - v5[3] + sub_44DA90(2 * v5[3]);
      a3[5] = v5[4] - *(_DWORD *)(a3[16] + 4) + sub_44DA90(2 * v9[1]);
      a3[6] = *(_DWORD *)(a3[16] + 8) - v5[5] + sub_44DA90(2 * v5[5]);
      a3[20] = v5[6] - v5[7] + sub_44DA90(2 * v5[7]);
      a3[22] = v5[8] - v5[9] + sub_44DA90(2 * v5[9]);
      a3[24] = v5[10] - v5[11] + sub_44DA90(2 * v5[11]);
      sub_44FE00(a3, 1);
      sub_44FC10();
      a3[7] = v5[14];
      a3[27] = v5[15];
      a3[28] = v5[16];
      sub_44DBA0((int)a3, v5[17]);
      a3[3] = 1;
    }
  }
  a3[29] = 0;
  return a3;
}

// ===== sub_44F5E0 @ 0x0044F5E0..0x0044F601 =====
void *__thiscall sub_44F5E0(void *this, char a2)
{
  sub_44F610();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_44F610 @ 0x0044F610..0x0044F669 =====
void __thiscall sub_44F610(_DWORD *this)
{
  *this = &DCPFirefly::`vftable';
  sub_44FB00();
  sub_44D860(this);
}

// ===== sub_44F670 @ 0x0044F670..0x0044F6F0 =====
int sub_44F670()
{
  _DWORD *v0; // esi
  int result; // eax

  v0 = &unk_50ECE0;
  memset(dword_5121D8, 0, 0x2300u);
  memset(dword_50EBD8, 0, 0x100u);
  memset(byte_50FED8, 0, 0x2300u);
  result = 64;
  do
  {
    *(v0 - 2) = 0;
    *(v0 - 1) = 0;
    *v0 = 0;
    v0[1] = 0;
    v0[2] = 0;
    v0[3] = 0;
    v0[4] = 0;
    v0[5] = 0;
    v0[6] = 0;
    v0[7] = 0;
    v0[8] = 0;
    v0[9] = 0;
    v0[10] = 0;
    v0[11] = 0;
    v0[13] = 0;
    v0[14] = 0;
    v0[15] = 32;
    v0[12] = 0;
    v0 += 18;
    --result;
  }
  while ( result );
  return result;
}

// ===== sub_44F6F0 @ 0x0044F6F0..0x0044F7DC =====
BOOL __usercall sub_44F6F0@<eax>(
        unsigned int a1@<eax>,
        int a2@<edx>,
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
        int *a15,
        int a16,
        int a17,
        int a18)
{
  int *v18; // ecx
  int v19; // eax
  int v20; // eax
  BOOL v22; // [esp+Ch] [ebp-4h]

  v22 = a1 < 0x40;
  if ( a1 >= 0x40 )
    return 0;
  dword_50ECDC[18 * a1] = a3;
  v18 = &dword_50ECD8[18 * a1];
  v18[2] = a2;
  v18[3] = (int)abs32(a4) >> 8;
  v18[4] = a5 >> 8;
  v18[5] = (int)abs32(a6) >> 8;
  v18[7] = (int)abs32(a8) >> 8;
  v18[9] = (int)abs32(a10) >> 8;
  v18[11] = (int)abs32(a12) >> 8;
  v18[12] = a13;
  v19 = a16;
  *v18 = 1;
  v18[6] = a7 >> 8;
  v18[8] = a9 >> 8;
  v18[10] = a11 >> 8;
  v18[13] = a14;
  if ( !a16 )
    v19 = 1;
  v18[15] = v19;
  v20 = a17;
  if ( !a17 )
    v20 = 1;
  v18[16] = v20;
  v18[17] = a18;
  v18[14] = *a15 >> 8;
  return v22;
}

// ===== sub_44F7E0 @ 0x0044F7E0..0x0044F811 =====
int __usercall sub_44F7E0@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<ecx>,
        unsigned int a3,
        _DWORD *a4,
        unsigned int a5)
{
  if ( a1 >= 0x40 )
    return -2147483647;
  else
    return sub_44D880((unsigned int *)&byte_50FED8[140 * a1], a3, a4, a5, a2);
}

// ===== sub_44F820 @ 0x0044F820..0x0044F85E =====
int __usercall sub_44F820@<eax>(
        unsigned int a1@<eax>,
        unsigned int a2@<ecx>,
        unsigned int a3@<esi>,
        unsigned int a4,
        _DWORD *a5,
        int a6)
{
  int result; // eax

  if ( a3 >= 0x40 )
    return -2147483647;
  result = sub_44D880((unsigned int *)&dword_5121D8[35 * a3], a4, a5, a2, a1);
  if ( !result )
    dword_50EBD8[a3] = a6;
  return result;
}

// ===== sub_44F860 @ 0x0044F860..0x0044F88F =====
int __usercall sub_44F860@<eax>(unsigned int a1@<edx>, int a2)
{
  int v2; // edx

  if ( a1 >= 0x40 )
    return -2147483647;
  if ( !sub_44F890(a1) )
    return -2147483643;
  dword_50EBD8[v2] = a2;
  return 0;
}

// ===== sub_44F890 @ 0x0044F890..0x0044F8A8 =====
BOOL __fastcall sub_44F890(unsigned int a1)
{
  BOOL result; // eax

  result = 0;
  if ( a1 < 0x40 )
    return dword_5121D8[35 * a1] != 0;
  return result;
}

// ===== sub_44F900 @ 0x0044F900..0x0044FAF8 =====
int *__thiscall sub_44F900(int *this, unsigned int a2)
{
  int v3; // ecx
  int v4; // eax
  int v5; // edx
  int v6; // eax
  int v7; // ecx
  int *v8; // edi
  int v9; // ecx
  int v10; // edx
  int v11; // eax
  unsigned int v12; // ecx
  int v13; // eax
  unsigned int v14; // ecx
  _DWORD *v15; // eax
  int v16; // ecx
  int v17; // edx
  int *v19; // [esp+Ch] [ebp-24h]
  int v20; // [esp+10h] [ebp-20h]
  void *v21[2]; // [esp+18h] [ebp-18h] BYREF
  unsigned int v22; // [esp+20h] [ebp-10h]
  unsigned int v23; // [esp+24h] [ebp-Ch]

  v3 = 0;
  if ( a2 > 0x1F )
    return (int *)v3;
  sub_42D560((int)this);
  v4 = sub_44DBC0((int)this);
  if ( *(_DWORD *)(dword_50FEDC[v5] + 24 * v4) == v3 )
    return (int *)v3;
  sub_42D560((int)this);
  v6 = sub_44DBC0((int)this);
  v8 = (int *)(dword_50FEDC[v7] + 24 * v6);
  if ( !dword_50EBD8[sub_42D560((int)this)] )
    return v8;
  (*(void (__thiscall **)(int *))(*this + 8))(this);
  v20 = sub_42D560((int)this);
  v10 = 3 * sub_44DBD0((int)this);
  v11 = dword_5121DC[35 * v9 + a2];
  v12 = v8[2];
  v13 = v11 + 8 * v10;
  v19 = (int *)v13;
  v22 = *(_DWORD *)(v13 + 8);
  if ( v12 >= v22 )
    v22 = v12;
  v14 = v8[3];
  v23 = *(_DWORD *)(v13 + 12);
  if ( v14 >= v23 )
    v23 = v14;
  v15 = operator new(0x18u);
  v16 = v22;
  v17 = v23;
  this[29] = (int)v15;
  sub_409030(v8[4], v17, v15, v16);
  sub_40A620(this[29], 0);
  sub_40A530(
    v8,
    (_DWORD *)this[29],
    (unsigned int)(*(_DWORD *)(this[29] + 12) - v8[3]) >> 1,
    (unsigned int)(*(_DWORD *)(this[29] + 8) - v8[2]) >> 1,
    128,
    0);
  if ( v22 == v19[2] && v23 == v19[3] )
  {
    sub_40C0F0(this[29], this[29], (int)v19, dword_50EBD8[v20], 1);
    return (int *)this[29];
  }
  else
  {
    sub_409030(v8[4], v23, v21, v22);
    sub_40A620((int)v21, 0);
    sub_40A530(v19, v21, (v23 - v19[3]) >> 1, (v22 - v19[2]) >> 1, 128, 0);
    sub_40C0F0(this[29], this[29], (int)v21, dword_50EBD8[v20], 1);
    operator delete[](v21[0]);
    return (int *)this[29];
  }
}

// ===== sub_44FB00 @ 0x0044FB00..0x0044FB2E =====
int __thiscall sub_44FB00(void **this)
{
  void **v2; // ecx
  int result; // eax

  v2 = (void **)this[29];
  result = 0;
  if ( v2 )
  {
    operator delete[](*v2);
    operator delete(this[29]);
    this[29] = 0;
    return 1;
  }
  return result;
}

// ===== sub_44FB30 @ 0x0044FB30..0x0044FB79 =====
int __thiscall sub_44FB30(_DWORD *this)
{
  int result; // eax
  unsigned int v2; // esi
  unsigned int v3; // edi
  unsigned int v4; // edx
  unsigned int v5; // ecx

  result = 0;
  if ( this[3] )
  {
    v2 = this[18];
    v3 = this[28];
    v4 = this[17] - v2;
    if ( v4 >= v3 )
    {
      v5 = this[27];
      if ( v2 < v5 )
        return 256 - (v2 << 8) / v5;
    }
    else
    {
      return 256 - (v4 << 8) / v3;
    }
  }
  return result;
}

// ===== sub_44FB80 @ 0x0044FB80..0x0044FC09 =====
int __thiscall sub_44FB80(_DWORD *this)
{
  _DWORD *v1; // ecx
  int result; // eax
  unsigned int v3; // eax
  unsigned int v4; // esi
  signed int v5; // edi

  sub_44DB40(this);
  result = 0;
  if ( v1[3] )
  {
    v3 = v1[18];
    v1[18] = v3 + 1;
    if ( v3 < v1[17] )
    {
      v4 = v1[26];
      v5 = v1[25];
      v1[4] += v1[19] + (int)(v4 * (v1[20] - v1[19])) / v5;
      v1[5] += v1[21] + (int)(v4 * (v1[22] - v1[21])) / v5;
      v1[6] += v1[23] + (int)(v4 * (v1[24] - v1[23])) / v5;
      if ( v4 >= v5 )
      {
        sub_44FC10();
        return 1;
      }
      else
      {
        result = 1;
        if ( *(_DWORD *)(v1[16] + 40) )
          v1[26] = v4 + 1;
      }
    }
    else
    {
      return 0;
    }
  }
  return result;
}

// ===== sub_44FC10 @ 0x0044FC10..0x0044FDCF =====
int __usercall sub_44FC10@<eax>(_DWORD *a1@<esi>)
{
  int v1; // ebx
  int v2; // eax
  int *v3; // ebx
  double v4; // st7
  long double v5; // st7
  long double v6; // st6
  long double v7; // st5
  long double v8; // st2
  long double v9; // st5
  long double v10; // st3
  long double v11; // st2
  long double v12; // st7
  long double v13; // st6
  long double v14; // st7
  long double v15; // st4
  int result; // eax
  int v17; // [esp+8h] [ebp-28h]
  long double v18; // [esp+8h] [ebp-28h]
  int v19; // [esp+10h] [ebp-20h]
  double v20; // [esp+10h] [ebp-20h]
  int v21; // [esp+18h] [ebp-18h]
  int v22; // [esp+18h] [ebp-18h]
  long double v23; // [esp+18h] [ebp-18h]
  double v24; // [esp+20h] [ebp-10h]
  double v25; // [esp+28h] [ebp-8h]

  v17 = a1[20];
  a1[19] = v17;
  v19 = a1[22];
  a1[21] = v19;
  v21 = a1[24];
  a1[23] = v21;
  v1 = 9 * sub_42D560((int)a1);
  v2 = a1[16];
  v3 = &dword_50ECD8[2 * v1];
  if ( *(_DWORD *)(v2 + 44) )
  {
    v25 = (double)v17;
    v20 = (double)v19;
    v4 = (double)v21;
    v22 = a1[16];
    v24 = v4;
    v23 = (double)(*(_DWORD *)(v22 + 48) - *(_DWORD *)(v22 + 52) + sub_44DA90(2 * *(_DWORD *)(v2 + 52)))
        * 3.141592653589793
        / 11796480.0;
    v18 = sin(v23);
    v5 = cos(v23);
    v6 = v5 * v20 - v18 * v24;
    v7 = v20 * v18 + v24 * v5;
    v8 = v7 * v18;
    v9 = v5 * v7 - v18 * v25;
    v10 = v25 * v5 + v8;
    v11 = v5;
    v12 = v5 * v10 - v18 * v6;
    v13 = v6 * v11 + v18 * v10;
    if ( v12 < 0.0 )
      v14 = v12 - 0.5;
    else
      v14 = v12 + 0.5;
    a1[20] = (int)v14;
    if ( v13 < 0.0 )
      v15 = v13 - 0.5;
    else
      v15 = v13 + 0.5;
    a1[22] = (int)v15;
    if ( v9 < 0.0 )
      a1[24] = (int)(v9 - 0.5);
    else
      a1[24] = (int)(v9 + 0.5);
  }
  else
  {
    a1[20] = v3[6] - v3[7] + sub_44DA90(2 * v3[7]);
    a1[22] = v3[8] - v3[9] + sub_44DA90(2 * v3[9]);
    a1[24] = v3[10] - v3[11] + sub_44DA90(2 * v3[11]);
    sub_44FE00(a1, 0);
  }
  result = v3[12] + sub_44DA90(v3[13]);
  a1[25] = result;
  a1[26] = 0;
  if ( !result )
    result = 1;
  a1[25] = result;
  return result;
}

// ===== sub_44FDD0 @ 0x0044FDD0..0x0044FDFD =====
int __usercall sub_44FDD0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v3; // edx
  int v4; // ecx

  if ( sub_42D560(a2) != a1 )
    return v3;
  sub_44DBE0(&dword_5121D8[35 * a1], v4);
  return 1;
}

// ===== sub_44FE00 @ 0x0044FE00..0x0044FF8A =====
int __stdcall sub_44FE00(_DWORD *a1, int a2)
{
  _DWORD *v2; // esi
  long double v3; // st7
  double v4; // st7
  long double v5; // st6
  int v6; // eax
  int v8; // [esp+Ch] [ebp-2Ch]
  double v9; // [esp+10h] [ebp-28h]
  double v10; // [esp+18h] [ebp-20h]
  double v11; // [esp+20h] [ebp-18h]
  double v12; // [esp+28h] [ebp-10h]
  long double v13; // [esp+30h] [ebp-8h]

  v2 = (_DWORD *)a1[16];
  if ( !v2[3] )
    return 1;
  v10 = (double)(int)a1[20];
  v11 = (double)(int)a1[22];
  v12 = (double)(int)a1[24];
  v3 = sqrt(v10 * v10 + v11 * v11 + v12 * v12);
  v9 = v3;
  if ( 0.0 == v3 )
  {
    v9 = 1.0;
    v3 = 1.0;
  }
  v13 = (1.0 - fabs(v12) / v3 * (double)(0x10000 - v2[8]) * 0.0000152587890625)
      * ((1.0 - fabs(v11) / v3 * (double)(0x10000 - v2[7]) * 0.0000152587890625)
       * (1.0 - fabs(v10) / v3 * (double)(0x10000 - v2[6]) * 0.0000152587890625));
  v4 = (double)(*(_DWORD *)(a1[16] + 16) + sub_44DA90(v2[5]));
  v5 = v13 * v4 / v9;
  a1[20] = (int)(v10 * v5);
  a1[22] = (int)(v11 * v5);
  a1[24] = (int)(v5 * v12);
  if ( !a2 )
    return 1;
  v6 = a1[16];
  if ( !*(_DWORD *)(v6 + 36) )
    return 1;
  v8 = *(_DWORD *)(v6 + 16);
  if ( v8 <= 0 )
    return 1;
  a1[17] = (__int64)((double)(unsigned int)a1[17] * (double)v8 / v4);
  return 1;
}

// ===== sub_44FF90 @ 0x0044FF90..0x0045009A =====
_DWORD *__stdcall sub_44FF90(_DWORD *a1, unsigned int a2)
{
  _DWORD *v2; // ebx
  int v3; // eax

  sub_44D7E0(0, a2, a1, (_DWORD *)&unk_5144D8 + 35 * a2);
  *a1 = &DCPSnow::`vftable';
  if ( a2 < 0x40 )
  {
    v2 = (_DWORD *)((char *)&unk_5167D8 + 44 * a2);
    if ( *v2 )
    {
      a1[4] = sub_44DA90(2 * v2[1]) - v2[1];
      a1[5] = v2[2];
      a1[6] = sub_44DA90(2 * v2[3]) - v2[3];
      a1[16] = v2[4] - v2[5] + sub_44DA90(2 * v2[5]);
      a1[17] = v2[6] - v2[7] + sub_44DA90(2 * v2[7]);
      a1[18] = v2[8] - v2[9] + sub_44DA90(2 * v2[9]);
      a1[7] = v2[10];
      v3 = sub_44DBA0((int)a1, 32);
      sub_44DBB0(v3);
      a1[3] = 1;
    }
  }
  return a1;
}
