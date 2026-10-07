#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_460030 @ 0x00460030..0x00460036 =====
int sub_460030()
{
  return dword_565EC0;
}

// ===== sub_460040 @ 0x00460040..0x004602BA =====
int sub_460040()
{
  IDirect3D9 *v0; // eax
  int (__stdcall *v2)(int, _DWORD, int, int, int, int); // eax
  HMODULE LibraryA; // eax
  _BYTE v4[8]; // [esp+24h] [ebp-138h] BYREF
  unsigned int v5; // [esp+2Ch] [ebp-130h]
  int v6; // [esp+30h] [ebp-12Ch]
  int v7; // [esp+F0h] [ebp-6Ch]

  v0 = Direct3DCreate9(0x20u);
  dword_565E9C = (int)v0;
  if ( v0 && v0->lpVtbl->GetAdapterDisplayMode(v0, 0, (D3DDISPLAYMODE *)&dword_5177B0) >= 0 )
  {
    if ( (*(int (__stdcall **)(int, _DWORD, _DWORD, void *))(*(_DWORD *)dword_565E9C + 20))(
           dword_565E9C,
           0,
           0,
           &unk_5172E0) < 0 )
      memset(&unk_5172E0, 0, 0x44Cu);
    if ( (*(int (__stdcall **)(int, _DWORD, int, _BYTE *))(*(_DWORD *)dword_565E9C + 56))(dword_565E9C, 0, 1, v4) >= 0
      && (v6 & 0x20000000) != 0
      && (v2 = *(int (__stdcall **)(int, _DWORD, int, int, int, int))(*(_DWORD *)dword_565E9C + 36),
          HIDWORD(xmmword_50B0E0[0]) = (v5 >> 17) & 1,
          v2(dword_565E9C, 0, 1, 22, 22, 1) >= 0)
      && (*(int (__stdcall **)(int, _DWORD, int, int, int, _DWORD))(*(_DWORD *)dword_565E9C + 36))(
           dword_565E9C,
           0,
           1,
           22,
           22,
           0) >= 0
      && (*(int (__stdcall **)(int, _DWORD, int, int, int, int, int))(*(_DWORD *)dword_565E9C + 40))(
           dword_565E9C,
           0,
           1,
           22,
           512,
           3,
           22) >= 0 )
    {
      dword_5172D8 = v7;
      LibraryA = LoadLibraryA("d3dx9_43.dll");
      hLibModule = LibraryA;
      if ( LibraryA )
        D3DXCreateEffect = (int)GetProcAddress(LibraryA, "D3DXCreateEffect");
      return sub_461110(2, 1, 0);
    }
    else
    {
      sub_46F710();
      sub_464500();
      return 0;
    }
  }
  else
  {
    sub_46F710();
    sub_464500();
    return 0;
  }
}

// ===== sub_4602C0 @ 0x004602C0..0x00460314 =====
HMODULE __cdecl sub_4602C0(int a1)
{
  HMODULE result; // eax

  sub_460400();
  sub_45F2B0();
  result = (HMODULE)dword_565E9C;
  if ( dword_565E9C )
  {
    result = (HMODULE)(*(int (__stdcall **)(int))(*(_DWORD *)dword_565E9C + 8))(dword_565E9C);
    dword_565E9C = 0;
  }
  if ( a1 )
  {
    result = hLibModule;
    if ( hLibModule )
    {
      result = (HMODULE)FreeLibrary(hLibModule);
      hLibModule = 0;
      D3DXCreateEffect = 0;
    }
  }
  return result;
}

// ===== sub_460320 @ 0x00460320..0x00460326 =====
int sub_460320()
{
  return HIDWORD(xmmword_50B0E0[0]);
}

// ===== sub_460330 @ 0x00460330..0x004603FD =====
int sub_460330()
{
  int v0; // ecx
  int v1; // ebx
  unsigned int v2; // ebx
  unsigned int v3; // edi
  unsigned int v4; // esi
  bool v5; // cf
  unsigned int v7; // [esp+Ch] [ebp-4h] BYREF

  if ( !sub_460320() || !sub_45F700() )
    return v0;
  v1 = -1 - sub_45F670();
  v2 = sub_45F680() + v1;
  v3 = 0;
  if ( !sub_45F690(&v7) )
  {
    while ( 1 )
    {
      v4 = v7;
      v5 = v7 < v3;
      if ( v7 < v3 )
        break;
      if ( v7 >= v2 )
      {
        v5 = v7 < v3;
        break;
      }
      sub_493AE0();
      v3 = v4;
      if ( sub_45F690(&v7) )
        return 1;
    }
    if ( !v5 && !sub_45F690(&v7) )
    {
      do
        sub_493AE0();
      while ( !sub_45F690(&v7) );
    }
  }
  return 1;
}

// ===== sub_460400 @ 0x00460400..0x0046041C =====
int sub_460400()
{
  int result; // eax

  result = dword_565ECC;
  if ( dword_565ECC )
  {
    result = (*(int (__stdcall **)(int))(*(_DWORD *)dword_565ECC + 8))(dword_565ECC);
    dword_565ECC = 0;
  }
  return result;
}

// ===== sub_460420 @ 0x00460420..0x0046054E =====
int sub_460420()
{
  int v0; // edi
  int v1; // eax
  HRSRC ResourceA; // eax
  HRSRC v3; // esi
  HGLOBAL Resource; // eax
  DWORD v5; // edi
  void *v6; // esi
  void *v7; // ebx
  void *v8; // eax
  int v10; // [esp+14h] [ebp-Ch] BYREF
  void *Src; // [esp+18h] [ebp-8h] BYREF
  int v12; // [esp+1Ch] [ebp-4h] BYREF

  v0 = -2;
  if ( (unsigned int)sub_460590() < 0xFFFF0300 )
    return 15;
  LOBYTE(v1) = __uncaught_exception();
  if ( !v1 )
    return 15;
  ResourceA = FindResourceA(hInst, (LPCSTR)0x81, "SHADER");
  v3 = ResourceA;
  if ( ResourceA )
  {
    Resource = LoadResource(hInst, ResourceA);
    Src = LockResource(Resource);
    v5 = SizeofResource(hInst, v3);
    v6 = operator new[](v5);
    v7 = operator new[](0x10000u);
    memcpy_0(v6, Src, v5);
    if ( sub_493940() )
    {
      v8 = (void *)sub_4938F0(v7);
    }
    else
    {
      sub_465320(v7, (int)&Src, v5, 0, 0);
      v8 = Src;
    }
    v0 = 0;
    v12 = 0;
    v10 = 0;
    if ( D3DXCreateEffect(dword_565EA0, v7, v8, 0, 0, 2, 0, &v12, &v10) )
    {
      v0 = -2;
    }
    else
    {
      sub_460400();
      dword_565ECC = v12;
    }
    operator delete[](v6);
    operator delete[](v7);
  }
  return v0;
}

// ===== sub_460550 @ 0x00460550..0x00460588 =====
int __usercall sub_460550@<eax>(int a1@<eax>)
{
  if ( a1 )
  {
    if ( a1 != 1 || sub_460420() )
    {
      return 0;
    }
    else
    {
      dword_565EC8 = 1;
      return 1;
    }
  }
  else
  {
    sub_460400();
    dword_565EC8 = 0;
    return 1;
  }
}

// ===== sub_460590 @ 0x00460590..0x00460596 =====
int sub_460590()
{
  return dword_5172D8;
}

// ===== ?__uncaught_exception@@YA_NXZ_0 @ 0x004605A0..0x004605AC =====
BOOL __cdecl __uncaught_exception()
{
  return D3DXCreateEffect != 0;
}

// ===== sub_4605B0 @ 0x004605B0..0x00460611 =====
int __fastcall sub_4605B0(int a1, int a2)
{
  void *v2; // esi
  int result; // eax
  int *v4; // ecx
  int v5; // eax

  v2 = dword_565F04;
  result = 0;
  v4 = &dword_565EE0;
  if ( dword_565F04 )
  {
    while ( a2 != *(_DWORD *)v2 )
    {
      v4 = (int *)v2;
      v2 = (void *)*((_DWORD *)v2 + 9);
      if ( !v2 )
        return result;
    }
    v4[9] = *((_DWORD *)v2 + 9);
    (*(void (__stdcall **)(_DWORD))(**((_DWORD **)v2 + 5) + 32))(*((_DWORD *)v2 + 5));
    v5 = *((_DWORD *)v2 + 5);
    if ( v5 )
    {
      (*(void (__stdcall **)(_DWORD))(*(_DWORD *)v5 + 8))(*((_DWORD *)v2 + 5));
      *((_DWORD *)v2 + 5) = 0;
    }
    operator delete(*((void **)v2 + 6));
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_460620 @ 0x00460620..0x0046065F =====
LPVOID __thiscall sub_460620(void *this)
{
  int *v1; // eax
  LPVOID result; // eax

  v1 = (int *)dword_565F04;
  for ( dword_565ED8 = 0; dword_565F04; v1 = (int *)dword_565F04 )
    sub_4605B0((int)this, *v1);
  result = ppvOut;
  if ( ppvOut )
  {
    result = (LPVOID)(*(int (__stdcall **)(LPVOID))(*(_DWORD *)ppvOut + 8))(ppvOut);
    ppvOut = 0;
  }
  return result;
}

// ===== sub_460660 @ 0x00460660..0x004606D8 =====
int __cdecl sub_460660(int a1, int a2, int a3, int a4)
{
  _DWORD *v4; // esi
  _DWORD v6[4]; // [esp+8h] [ebp-14h] BYREF

  v4 = dword_565F04;
  v6[0] = a1;
  v6[1] = a2;
  v6[2] = a3;
  v6[3] = a4;
  if ( !dword_565F04 )
    return 0;
  while ( !sub_445510(v6, v4 + 1) )
  {
    v4 = (_DWORD *)v4[9];
    if ( !v4 )
      return 0;
  }
  return 1;
}

// ===== sub_4606E0 @ 0x004606E0..0x00460819 =====
int __stdcall sub_4606E0(int *a1, int a2)
{
  _DWORD *v2; // esi
  _DWORD *v3; // ebx
  void *v4; // eax
  int v6; // [esp+Ch] [ebp-34h] BYREF
  _DWORD v7[11]; // [esp+10h] [ebp-30h] BYREF

  v2 = a1 + 1;
  if ( !sub_460660(a1[1], a1[2], a1[3], a1[4]) )
  {
    if ( (*(int (__stdcall **)(LPVOID, _DWORD *, int *, _DWORD))(*(_DWORD *)ppvOut + 12))(ppvOut, v2, &v6, 0) >= 0
      && (memset(v7, 0, sizeof(v7)), v7[0] = 44, (*(int (__stdcall **)(int, _DWORD *))(*(_DWORD *)v6 + 12))(v6, v7) >= 0)
      && (LOBYTE(v7[2]) == 21 || LOBYTE(v7[2]) == 20)
      && v7[3] >= 2u
      && v7[4] )
    {
      v3 = operator new(0x28u);
      *v3 = ++dword_565EE0;
      v3[1] = *v2;
      v3[2] = a1[2];
      v3[3] = a1[3];
      v3[4] = a1[4];
      v3[5] = v6;
      v4 = operator new(0x2Cu);
      v3[6] = v4;
      qmemcpy(v4, v7, 0x2Cu);
      v3[7] = 0;
      v3[8] = 0;
      v3[9] = dword_565F04;
      dword_565F04 = v3;
    }
    else if ( v6 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)v6 + 8))(v6);
    }
  }
  return 1;
}

// ===== sub_460820 @ 0x00460820..0x0046089A =====
BOOL __stdcall sub_460820(int a1, int *a2)
{
  int v2; // edx
  int (__stdcall *v3)(int *, int, int *); // ecx
  int v5; // [esp+0h] [ebp-1Ch] BYREF
  int v6; // [esp+4h] [ebp-18h]
  int v7; // [esp+8h] [ebp-14h]
  int v8; // [esp+Ch] [ebp-10h]
  int v9; // [esp+10h] [ebp-Ch]
  int v10; // [esp+14h] [ebp-8h]

  v5 = 0;
  v6 = 0;
  v7 = 0;
  v8 = 0;
  v9 = 0;
  v10 = 0;
  v2 = *a2;
  v7 = *(_DWORD *)(a1 + 24);
  v3 = *(int (__stdcall **)(int *, int, int *))(v2 + 24);
  v5 = 24;
  v6 = 16;
  v8 = 2;
  v9 = -1024;
  v10 = 1024;
  return v3(a2, 4, &v5) >= 0;
}

// ===== sub_4608A0 @ 0x004608A0..0x00460998 =====
int __usercall sub_4608A0@<eax>(_DWORD *a1@<edi>)
{
  int v1; // esi
  int v2; // eax
  _DWORD v4[4]; // [esp+4h] [ebp-18h] BYREF
  int v5; // [esp+14h] [ebp-8h]

  for ( ; !a1[8]; a1[7] = v2 == 0 )
  {
    v1 = a1[5];
    if ( (*(int (__stdcall **)(int, void *))(*(_DWORD *)v1 + 44))(v1, &unk_4DB7AC) < 0 )
      break;
    if ( (*(int (__stdcall **)(int, HWND, int))(*(_DWORD *)v1 + 52))(v1, hWndParent, 10) < 0 )
      break;
    v4[0] = 20;
    v4[1] = 16;
    v4[2] = 0;
    v4[3] = 0;
    v5 = 0;
    if ( (*(int (__stdcall **)(int, int, _DWORD *))(*(_DWORD *)v1 + 24))(v1, 2, v4) < 0 )
      break;
    (*(void (__stdcall **)(int, BOOL (__stdcall *)(int, int *), int, int))(*(_DWORD *)v1 + 16))(v1, sub_460820, v1, 3);
    v5 = 4096;
    if ( (*(int (__stdcall **)(int, int, _DWORD *))(*(_DWORD *)v1 + 24))(v1, 1, v4) < 0 )
      break;
    if ( (*(int (__stdcall **)(int))(*(_DWORD *)v1 + 28))(v1) < 0 )
      break;
    v2 = (*(int (__stdcall **)(int))(*(_DWORD *)v1 + 100))(v1);
    if ( v2 < 0 )
      break;
    a1[8] = 1;
  }
  return a1[8];
}

// ===== sub_4609A0 @ 0x004609A0..0x004609F5 =====
int sub_4609A0()
{
  int *v0; // edi
  int v1; // esi
  int v2; // eax
  int v3; // ecx

  if ( !ppvOut
    || (*(int (__stdcall **)(LPVOID, int, int (__stdcall *)(int *, int), _DWORD, int))(*(_DWORD *)ppvOut + 16))(
         ppvOut,
         4,
         sub_4606E0,
         0,
         1) < 0 )
  {
    return 0;
  }
  v0 = (int *)dword_565F04;
  if ( dword_565F04 )
  {
    do
    {
      v1 = *v0;
      v2 = sub_4608A0(v0);
      v0 = (int *)v0[9];
      if ( !v2 )
        sub_4605B0(v3, v1);
    }
    while ( v0 );
  }
  return 1;
}

// ===== sub_460A00 @ 0x00460A00..0x00460A46 =====
int __thiscall sub_460A00(void *this)
{
  void *v1; // ecx
  int result; // eax

  sub_460620(this);
  if ( DirectInput8Create(hInst, 0x800u, &riidltf, &ppvOut, 0) >= 0 && sub_4609A0() )
  {
    result = 1;
    dword_565ED8 = 1;
  }
  else
  {
    sub_460620(v1);
    return 0;
  }
  return result;
}

// ===== sub_460A50 @ 0x00460A50..0x00460A80 =====
int __usercall sub_460A50@<eax>(char a1@<al>, __int16 a2@<cx>, __int16 a3)
{
  return sub_496540(257, a3 & 0xFFF | ((a2 & 0xFFF | ((a1 & 3) << 12)) << 12));
}

// ===== sub_460A80 @ 0x00460A80..0x00460DED =====
int sub_460A80()
{
  int v0; // esi
  int *v1; // edi
  int v2; // ebx
  int (__stdcall *v3)(int, int, int *, int *, _DWORD); // edx
  int v4; // eax
  unsigned int v5; // edx
  int v6; // esi
  int *v8; // [esp+4h] [ebp-34h]
  int v9; // [esp+8h] [ebp-30h]
  int v10; // [esp+Ch] [ebp-2Ch] BYREF
  BOOL v11; // [esp+10h] [ebp-28h]
  int v12; // [esp+14h] [ebp-24h]
  int v13; // [esp+18h] [ebp-20h]
  int v14; // [esp+1Ch] [ebp-1Ch]
  int v15; // [esp+20h] [ebp-18h] BYREF
  unsigned int v16; // [esp+24h] [ebp-14h]

  v0 = 0;
  if ( !dword_565ED8 )
    return 1;
  v1 = (int *)dword_565F04;
  if ( dword_565F04 )
  {
    do
    {
      v2 = v1[5];
      v8 = v1;
      v14 = 0;
      v12 = 0;
      v9 = 0;
      v11 = 0;
LABEL_4:
      v13 = v0;
LABEL_5:
      while ( !v1[7] || (*(int (__stdcall **)(int))(*(_DWORD *)v2 + 100))(v2) >= 0 )
      {
        v3 = *(int (__stdcall **)(int, int, int *, int *, _DWORD))(*(_DWORD *)v2 + 40);
        v0 = 1;
        v10 = 1;
        v4 = v3(v2, 20, &v15, &v10, 0);
        if ( v4 >= 0 && v10 )
        {
          switch ( v15 )
          {
            case 0:
              dword_566ABC = v16;
              goto LABEL_4;
            case 4:
              dword_566AC0 = v16;
              v14 = 1;
              continue;
            case 8:
              dword_566AC4 = v16;
              v12 = 1;
              continue;
            case 20:
              dword_566AC8 = v16;
              v9 = 1;
              continue;
            case 32:
            case 36:
            case 40:
            case 44:
              if ( v16 >= 0x8CA0 )
                LOWORD(v5) = -1;
              else
                v5 = v16 / 0x1194;
              sub_496540(258, (unsigned __int16)v5 | ((v15 - 32) << 16));
              if ( v16 >= 0x2328 )
              {
                if ( v16 >= 0x4650 )
                {
                  if ( v16 >= 0x6978 )
                  {
                    if ( v16 >= 0x8CA0 || !dword_565F90 )
                      continue;
                  }
                  else if ( !dword_565F8C )
                  {
                    continue;
                  }
                }
                else if ( !dword_565F94 )
                {
                  continue;
                }
              }
              else if ( !dword_565F88 )
              {
                continue;
              }
              goto LABEL_35;
            default:
              v6 = 0;
              break;
          }
          while ( v15 != dword_4E6AC8[v6] )
          {
            if ( (unsigned int)++v6 >= 0x20 )
              goto LABEL_5;
          }
          sub_496540(256, v6 & 0x7F | ((unsigned __int8)v16 != 0 ? 0x80 : 0));
          if ( (_BYTE)v16 && dword_565F08[v6] )
          {
LABEL_35:
            sub_46DA80();
            sub_46DB00();
          }
        }
        else
        {
          if ( v4 != -2147024866 )
          {
            v11 = v4 >= 0;
            break;
          }
          (*(void (__stdcall **)(int))(*(_DWORD *)v2 + 28))(v2);
        }
      }
      if ( !v13 && !v14 )
        goto LABEL_55;
      sub_460A50(0, dword_566AC0, dword_566ABC);
      if ( v13 )
      {
        if ( dword_566ABC == 1024 )
        {
          if ( !dword_565F94 )
            goto LABEL_48;
LABEL_47:
          sub_46DA80();
          sub_46DB00();
          goto LABEL_48;
        }
        if ( dword_566ABC == -1024 && dword_565F90 )
          goto LABEL_47;
      }
LABEL_48:
      if ( !v14 )
        goto LABEL_55;
      if ( dword_566AC0 == 1024 )
      {
        if ( !dword_565F8C )
          goto LABEL_55;
      }
      else if ( dword_566AC0 != -1024 || !dword_565F88 )
      {
        goto LABEL_55;
      }
      sub_46DA80();
      sub_46DB00();
LABEL_55:
      if ( v12 || v9 )
        sub_460A50(1, dword_566AC8, dword_566AC4);
      v1 = (int *)v1[9];
      v0 = 0;
      if ( !v11 )
        sub_4605B0((int)v8, *v8);
    }
    while ( v1 );
  }
  return 0;
}

// ===== sub_460E40 @ 0x00460E40..0x00460E5B =====
int __cdecl sub_460E40(int a1)
{
  unsigned int v1; // ecx
  int result; // eax

  result = -2147483646;
  if ( v1 < 0x24 )
  {
    dword_565F08[v1] = a1;
    return 0;
  }
  return result;
}

// ===== sub_460E60 @ 0x00460E60..0x00461029 =====
int __cdecl sub_460E60(_DWORD *a1)
{
  int *v1; // ecx
  _DWORD **v2; // edi
  int v3; // ebx
  int v4; // esi
  unsigned int v5; // edx
  unsigned int v6; // ecx
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // eax
  int v14; // eax
  int v16; // [esp+10h] [ebp-70h]
  int v17; // [esp+14h] [ebp-6Ch]
  int v18; // [esp+18h] [ebp-68h]
  int v19; // [esp+1Ch] [ebp-64h]
  int v20; // [esp+20h] [ebp-60h]
  int *v21; // [esp+24h] [ebp-5Ch]
  _DWORD v22[8]; // [esp+28h] [ebp-58h] BYREF
  unsigned int v23; // [esp+48h] [ebp-38h]
  _BYTE v24[36]; // [esp+58h] [ebp-28h]

  v2 = (_DWORD **)dword_565F04;
  v3 = 0;
  v21 = v1;
  v4 = -2147483647;
  v16 = 0;
  v17 = 0;
  v18 = 0;
  v19 = -1;
  v20 = 0;
  if ( dword_565F04 )
  {
    do
    {
      if ( a1 == *v2 || !a1 )
      {
        if ( (*(int (__stdcall **)(_DWORD *, int, _DWORD *))(*v2[5] + 36))(v2[5], 80, v22) >= 0 )
        {
          v16 += v22[1];
          v3 += v22[0];
          v17 += v22[2];
          v18 += v22[5];
          if ( v23 < 0x8CA0 )
          {
            v5 = v23 / 0x1194;
            if ( v19 != -1 )
              v5 = dword_4E6B48[8 * v19 + v5];
            v19 = v5;
          }
          v6 = 0;
          v7 = 1;
          do
          {
            if ( v24[v6] )
              v20 |= v7;
            v8 = __ROL4__(v7, 1);
            if ( v24[v6 + 1] )
              v20 |= v8;
            v9 = __ROL4__(v8, 1);
            if ( v24[v6 + 2] )
              v20 |= v9;
            v10 = __ROL4__(v9, 1);
            if ( v24[v6 + 3] )
              v20 |= v10;
            v6 += 4;
            v7 = __ROL4__(v10, 1);
          }
          while ( v6 < 0x20 );
          v4 = 0;
        }
        v1 = v21;
        if ( a1 )
          break;
      }
      v2 = (_DWORD **)v2[9];
    }
    while ( v2 );
    if ( !v4 )
    {
      if ( v3 >= -1024 )
      {
        v11 = 1024;
        if ( v3 <= 1024 )
          v11 = v3;
      }
      else
      {
        v11 = -1024;
      }
      *v1 = v11;
      v12 = v16;
      if ( v16 >= -1024 )
      {
        if ( v16 > 1024 )
          v12 = 1024;
      }
      else
      {
        v12 = -1024;
      }
      v1[1] = v12;
      v13 = v17;
      if ( v17 >= -1024 )
      {
        if ( v17 > 1024 )
          v13 = 1024;
      }
      else
      {
        v13 = -1024;
      }
      v1[2] = v13;
      v14 = v18;
      if ( v18 >= -1024 )
      {
        if ( v18 > 1024 )
          v14 = 1024;
      }
      else
      {
        v14 = -1024;
      }
      v1[3] = v14;
      v1[4] = v19;
      v1[5] = v20;
    }
  }
  return v4;
}

// ===== sub_461030 @ 0x00461030..0x0046103A =====
int sub_461030()
{
  _DWORD *v0; // edi
  int v1; // esi
  int result; // eax

  result = sub_442CA0();
  v0 = dword_566750;
  v1 = 0;
  if ( *((int *)dword_566750 + 3) > 0 )
  {
    do
      result = sub_407CF0(v1++, (int)v0);
    while ( v1 < v0[3] );
  }
  return result;
}

// ===== sub_461040 @ 0x00461040..0x00461073 =====
int __fastcall sub_461040(int a1, int a2, unsigned int a3)
{
  if ( a3 >= 8 )
    return 1;
  if ( !a2 || !a1 )
    return 2;
  dword_506B8C[a3] = a2;
  dword_506BAC[a3] = a1;
  return 0;
}

// ===== sub_461080 @ 0x00461080..0x00461086 =====
int __usercall sub_461080@<eax>(int result@<eax>)
{
  dword_507218 = result;
  return result;
}

// ===== sub_461090 @ 0x00461090..0x00461098 =====
int __usercall sub_461090@<eax>(int a1@<eax>)
{
  return dword_506B8C[a1];
}

// ===== sub_4610A0 @ 0x004610A0..0x004610AA =====
int sub_4610A0()
{
  return sub_461090(dword_506B88);
}

// ===== sub_4610B0 @ 0x004610B0..0x004610B8 =====
int __usercall sub_4610B0@<eax>(int a1@<eax>)
{
  return dword_506BAC[a1];
}

// ===== sub_4610C0 @ 0x004610C0..0x004610CA =====
int sub_4610C0()
{
  return sub_4610B0(dword_506B88);
}

// ===== sub_4610D0 @ 0x004610D0..0x004610E0 =====
int __usercall sub_4610D0@<eax>(int result@<eax>)
{
  dword_507210 = result;
  dword_565F98 = 1;
  return result;
}

// ===== sub_4610E0 @ 0x004610E0..0x004610F0 =====
BOOL sub_4610E0()
{
  return dword_565F98 != 0;
}

// ===== sub_4610F0 @ 0x004610F0..0x00461104 =====
BOOL __usercall sub_4610F0@<eax>(struct tagMONITORINFO *a1@<eax>)
{
  HMONITOR v1; // eax

  a1->cbSize = 40;
  v1 = sub_45E4D0();
  return GetMonitorInfoA(v1, a1);
}

// ===== sub_461110 @ 0x00461110..0x004613EF =====
int __cdecl sub_461110(int a1, int a2, int a3)
{
  int v3; // edi
  HDC DC; // esi
  HBRUSH StockObject; // eax
  HDC v6; // edi
  HBRUSH v7; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int nWidth; // [esp+10h] [ebp-68h] BYREF
  int nHeight; // [esp+14h] [ebp-64h]
  int v14; // [esp+18h] [ebp-60h]
  int v15; // [esp+1Ch] [ebp-5Ch]
  _DWORD v16[2]; // [esp+20h] [ebp-58h] BYREF
  int v17; // [esp+28h] [ebp-50h]
  RECT rc; // [esp+2Ch] [ebp-4Ch] BYREF
  struct tagRECT Rect; // [esp+3Ch] [ebp-3Ch] BYREF
  struct tagMONITORINFO v20; // [esp+4Ch] [ebp-2Ch] BYREF

  v14 = 1;
  if ( !dword_565F9C )
  {
    dword_565F9C = 1;
    v3 = sub_461090(a1);
    v16[0] = v3;
    v15 = sub_4610B0(a1);
    nWidth = v3 + dword_517F14;
    v17 = v15 + dword_517B04;
    GetClientRect(hWndParent, &Rect);
    DC = GetDC(hWndParent);
    StockObject = (HBRUSH)GetStockObject(4);
    FillRect(DC, &Rect, StockObject);
    ReleaseDC(hWndParent, DC);
    dword_506B88 = a1;
    dword_518604 = a2;
    if ( a3 )
    {
      sub_45E550(0);
      sub_4610F0(&v20);
      sub_45E580((unsigned int *)&nWidth, 0);
      sub_48EF30(1);
      SetWindowLongA(hWndParent, -16, -1879048192);
      MoveWindow(hWndParent, v20.rcMonitor.left, v20.rcMonitor.top, nWidth, nHeight, 0);
      v6 = GetDC(hWndParent);
      rc.left = 0;
      rc.top = 0;
      rc.right = nWidth;
      rc.bottom = nHeight;
      v7 = (HBRUSH)GetStockObject(4);
      FillRect(v6, &rc, v7);
      ReleaseDC(hWndParent, v6);
      if ( sub_45F340(1) )
      {
        dword_565F9C = 0;
        return sub_461110(a1, a2, 0);
      }
      v3 = v16[0];
    }
    else
    {
      sub_48EF30(0);
      SetWindowLongA(hWndParent, -16, -1865809920);
      if ( dword_507218 )
      {
        sub_45E740(v3, v15);
        SetWindowPos(hWndParent, (HWND)0xFFFFFFFE, 0, 0, nWidth, v17, 0x10Au);
      }
      else
      {
        sub_45E750(v16);
        SetWindowPos(hWndParent, (HWND)0xFFFFFFFE, 0, 0, dword_517F14 + v16[0], dword_517B04 + v16[1], 0x10Au);
      }
      v9 = sub_45F340(0);
      if ( v9 )
      {
        switch ( v9 )
        {
          case 3:
            sub_464500();
            goto LABEL_11;
          case 5:
          case 7:
          case 11:
          case 13:
LABEL_11:
            sub_464500();
            break;
          default:
            break;
        }
        v14 = 0;
      }
      if ( sub_49A220() )
      {
        sub_461690();
        InvalidateRect(0, 0, 1);
      }
      else
      {
        sub_49A110();
      }
    }
    v10 = sub_46F5D0();
    sub_442D40(v10, a2, v3, v15);
    v11 = sub_45F010();
    sub_41ADF0((int)dword_56674C, v11);
    sub_461D70();
    dword_565F9C = 0;
  }
  return v14;
}

// ===== sub_461410 @ 0x00461410..0x0046143F =====
DWORD sub_461410()
{
  unsigned int v0; // esi
  DWORD result; // eax

  dword_565FA0 = 1;
  v0 = 0x1770u / sub_45E480();
  result = GetTickCount();
  dword_565FA8 = result + v0 + 1;
  return result;
}

// ===== sub_461440 @ 0x00461440..0x0046144B =====
void sub_461440()
{
  dword_565FA4 = 1;
}

// ===== sub_461450 @ 0x00461450..0x0046154D =====
void sub_461450()
{
  int v0; // eax
  unsigned int v1; // esi
  int v2; // eax
  int v3; // eax
  int v4; // eax

  if ( dword_565FA0 && dword_565FA8 <= GetTickCount() )
  {
    if ( sub_45F640() == 1 && sub_45E7C0() )
    {
      v0 = sub_45F640();
      sub_461110(dword_506B88, dword_518604, v0);
      v1 = 0x3E8u / sub_45E480();
      dword_565FAC = GetTickCount() + v1 + 1;
    }
    dword_565FA0 = 0;
    dword_565FA8 = 0;
  }
  if ( dword_565FA4 )
  {
    if ( dword_5666FC )
    {
      if ( !sub_48F680() )
      {
        LOBYTE(v2) = __uncaught_exception();
        if ( !v2 )
        {
          v3 = sub_45F640();
          if ( sub_461110(dword_506B88, dword_518604, v3 == 0) )
          {
            v4 = sub_45F640();
            sub_496540(1, v4);
          }
        }
      }
    }
    dword_565FA4 = 0;
  }
  if ( dword_565FAC )
  {
    if ( dword_565FAC <= GetTickCount() )
    {
      sub_461D70();
      dword_565FAC = 0;
    }
  }
}

// ===== sub_461550 @ 0x00461550..0x004615D8 =====
BOOL sub_461550()
{
  unsigned int v1[5]; // [esp+8h] [ebp-14h] BYREF

  sub_45E580(v1, 1);
  sub_45E600();
  return (int)(10000.0 * ((double)v1[0] / (double)v1[1])) != (int)((double)v1[2] / (double)v1[3] * 10000.0);
}

// ===== sub_4615E0 @ 0x004615E0..0x00461688 =====
int __cdecl sub_4615E0(_DWORD *a1)
{
  int result; // eax
  int v2; // esi
  unsigned int v3[2]; // [esp+Ch] [ebp-40h] BYREF
  _DWORD v4[2]; // [esp+14h] [ebp-38h] BYREF
  struct tagMONITORINFO v5; // [esp+1Ch] [ebp-30h] BYREF

  result = sub_45F640();
  if ( result )
  {
    *a1 = 0;
    a1[1] = 0;
  }
  else
  {
    sub_45E580(v3, 1);
    sub_45E750(v4);
    sub_4610F0(&v5);
    *a1 = v5.rcMonitor.left + (signed int)(v3[0] - dword_517F14 - v4[0]) / 2;
    v2 = v5.rcMonitor.top + (signed int)(v3[1] - v4[1]) / 2 - GetSystemMetrics(4);
    result = GetSystemMetrics(8);
    a1[1] = v2 - result;
  }
  return result;
}

// ===== sub_461690 @ 0x00461690..0x004616DD =====
int sub_461690()
{
  int v0; // ecx
  int X[2]; // [esp+0h] [ebp-8h] BYREF

  if ( sub_45F640() )
    return v0;
  sub_4615E0(X);
  SetWindowPos(hWndParent, 0, X[0], X[1], 0, 0, 0x25u);
  return 1;
}

// ===== sub_4616E0 @ 0x004616E0..0x0046173F =====
unsigned int __usercall sub_4616E0@<eax>(_DWORD *a1@<esi>)
{
  unsigned int result; // eax
  unsigned int v2; // ecx
  unsigned int v3; // [esp+8h] [ebp-8h] BYREF
  int v4; // [esp+Ch] [ebp-4h]

  *a1 = 0;
  a1[1] = 0;
  if ( sub_45F640() )
  {
    result = sub_45E580(&v3, 1);
    v2 = v3;
    a1[3] = v4;
    a1[2] = v2;
  }
  else
  {
    sub_45E750(&v3);
    result = v4 + dword_517B04;
    a1[2] = v3 + dword_517F14;
    a1[3] = result;
  }
  return result;
}

// ===== sub_461740 @ 0x00461740..0x004617A2 =====
int __usercall sub_461740@<eax>(char *a1@<eax>, int a2@<ecx>)
{
  int result; // eax
  int v4; // edx
  int v5; // edx
  int v6; // ecx
  int v7; // esi

  result = 0;
  dword_5666FC = a2;
  if ( !a2 )
    return 1;
  if ( !a1 )
  {
    dword_566700[0] = 0;
    return 1;
  }
  v4 = 0;
  if ( !*(_DWORD *)a1 )
    goto LABEL_6;
  do
    ++v4;
  while ( *(_DWORD *)&a1[4 * v4] );
  if ( v4 < 16 )
  {
LABEL_6:
    v5 = v4 + 1;
    v6 = 0;
    if ( v5 > 0 )
    {
      v7 = a1 - (char *)dword_566700;
      do
      {
        dword_566700[v6] = *(int *)((char *)&dword_566700[v6] + v7);
        ++v6;
      }
      while ( v6 < v5 );
      return 1;
    }
    return 1;
  }
  return result;
}

// ===== sub_4617B0 @ 0x004617B0..0x004617D5 =====
BOOL __usercall sub_4617B0@<eax>(int a1@<esi>)
{
  BOOL result; // eax
  int *v2; // edx
  int v3; // ecx

  result = 0;
  if ( dword_5666FC )
  {
    v2 = dword_566700;
    do
    {
      v3 = *v2;
      if ( !*v2 )
        break;
      result = a1 == v3;
      ++v2;
    }
    while ( a1 != v3 );
  }
  return result;
}

// ===== sub_4617E0 @ 0x004617E0..0x0046197D =====
int __cdecl sub_4617E0(HDC a1, int a2, int a3, int a4, int a5)
{
  LONG v5; // edx
  int v6; // esi
  int v7; // eax
  WORD v8; // ax
  int v9; // edx
  int v10; // ecx
  int v11; // edi
  int v12; // eax
  int v13; // edi
  HDC v14; // esi
  int result; // eax
  int mode; // [esp+14h] [ebp-64h] BYREF
  int v17; // [esp+18h] [ebp-60h]
  int xDest; // [esp+1Ch] [ebp-5Ch]
  HGDIOBJ ho; // [esp+20h] [ebp-58h]
  int DestWidth[2]; // [esp+24h] [ebp-54h] BYREF
  int DestHeight; // [esp+2Ch] [ebp-4Ch]
  struct tagPOINT pt; // [esp+30h] [ebp-48h] BYREF
  BITMAPINFO bmi; // [esp+38h] [ebp-40h] BYREF
  RECT rect; // [esp+64h] [ebp-14h] BYREF

  v5 = *(_DWORD *)(a4 + 8);
  v6 = *(_DWORD *)(a4 + 12);
  bmi.bmiHeader.biPlanes = 1;
  v7 = *(_DWORD *)(a4 + 16);
  bmi.bmiHeader.biSize = 40;
  bmi.bmiHeader.biWidth = v5;
  bmi.bmiHeader.biHeight = -v6;
  v8 = sub_407B40(v7);
  v10 = v9;
  v11 = v9 * *(_DWORD *)(a4 + 20);
  bmi.bmiHeader.biBitCount = v8;
  v12 = v6;
  bmi.bmiHeader.biSizeImage = v6 * v11;
  memset(&bmi.bmiHeader.biXPelsPerMeter, 0, 16);
  ho = 0;
  xDest = a2;
  v13 = a3;
  bmi.bmiHeader.biCompression = 0;
  if ( a5 )
  {
    mode = v9;
    v17 = v6;
    sub_45E8D0(DestWidth, a2, a3, 0);
    sub_45ECF0(&mode, (unsigned int *)&mode);
    v13 = DestWidth[1];
    xDest = DestWidth[0];
    DestWidth[0] = mode;
    DestHeight = v17;
    sub_45EBB0(&rect);
    v14 = a1;
    ho = CreateRectRgnIndirect(&rect);
    SelectClipRgn(a1, (HRGN)ho);
    mode = SetStretchBltMode(a1, 4);
    SetBrushOrgEx(a1, 0, 0, &pt);
    v12 = DestHeight;
    v10 = DestWidth[0];
  }
  else
  {
    v14 = a1;
  }
  result = StretchDIBits(
             v14,
             xDest,
             v13,
             v10,
             v12,
             0,
             0,
             *(_DWORD *)(a4 + 8),
             *(_DWORD *)(a4 + 12),
             *(const void **)a4,
             &bmi,
             0,
             0xCC0020u);
  if ( a5 )
  {
    SetStretchBltMode(v14, mode);
    SetBrushOrgEx(v14, pt.x, pt.y, 0);
    SelectClipRgn(v14, 0);
    return DeleteObject(ho);
  }
  return result;
}

// ===== sub_461980 @ 0x00461980..0x004619BA =====
int __cdecl sub_461980(int a1, int a2, int a3)
{
  HDC DC; // esi

  DC = GetDC(hWndParent);
  sub_4617E0(DC, a1, a2, a3, 1);
  return ReleaseDC(hWndParent, DC);
}

// ===== sub_4619C0 @ 0x004619C0..0x00461A14 =====
int __cdecl sub_4619C0(int a1, int a2)
{
  int v2; // ecx
  _DWORD v4[6]; // [esp+8h] [ebp-18h] BYREF

  if ( !dword_5666F0 )
    return 1;
  if ( !sub_407F20((int)dword_566750, v2, v4) )
    return 2;
  sub_461980(a1, a2, (int)v4);
  return 0;
}

// ===== sub_461A20 @ 0x00461A20..0x00461A62 =====
char sub_461A20()
{
  int v0; // eax
  HWND v1; // esi

  LOBYTE(v0) = __uncaught_exception();
  if ( v0 )
  {
    v0 = sub_464280();
    if ( v0 )
    {
      dword_565FB0 = 1;
      v1 = (HWND)sub_463F10();
      InvalidateRect(v1, 0, 0);
      LOBYTE(v0) = UpdateWindow(v1);
      dword_565FB0 = 0;
    }
  }
  return v0;
}

// ===== sub_461A70 @ 0x00461A70..0x00461AA5 =====
int __usercall sub_461A70@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  int v5; // [esp+0h] [ebp-4h] BYREF

  sub_45F730(a3, a4, a2, a1);
  v5 = 0;
  sub_45FF00(&v5);
  sub_461A20();
  sub_4301A0();
  return v5;
}

// ===== sub_461AB0 @ 0x00461AB0..0x00461AEA =====
int sub_461AB0()
{
  int v0; // esi

  sub_4017F0();
  v0 = sub_4430C0();
  sub_401830(1);
  if ( !v0 )
    return 0;
  sub_4086E0();
  return sub_461A70(0, 0, 1, 0);
}

// ===== sub_461AF0 @ 0x00461AF0..0x00461B9A =====
int sub_461AF0()
{
  int v0; // esi
  int v2; // [esp+8h] [ebp-4008h] BYREF
  _DWORD v3[4096]; // [esp+Ch] [ebp-4004h] BYREF

  sub_4017F0();
  v0 = sub_4430F0(&v2, v3);
  sub_401830(1);
  if ( !v0 )
    return 0;
  sub_4086E0();
  if ( !v2 )
    return 0;
  if ( v2 == -1 )
    return sub_461A70(0, 0, 1, 0);
  return sub_461A70(0, 0, v2, (int)v3);
}

// ===== sub_461BA0 @ 0x00461BA0..0x00461C77 =====
int sub_461BA0()
{
  int v0; // edi
  unsigned int v1; // esi
  int v2; // eax
  unsigned int v4; // esi

  v0 = -1;
  if ( dword_566740 )
  {
    if ( dword_506BD4 )
    {
      if ( dword_506BD8 )
      {
        v1 = sub_498720();
        if ( dword_566744 <= v1 )
        {
          if ( dword_506BD0 )
            v2 = sub_461AB0();
          else
            v2 = sub_461AF0();
          v0 = v2;
          ++dword_566748;
          dword_566740 = 0;
          dword_566744 += dword_506BCC * ((v1 - dword_566744) / dword_506BCC + 1);
        }
      }
    }
  }
  if ( sub_48F680() )
  {
    if ( sub_45F710() )
    {
      sub_48F0E0();
      return v0;
    }
  }
  else if ( sub_45F590() == -2147483647 && dword_506BD4 )
  {
    if ( dword_506BD8 )
    {
      v4 = 0xBB8u / sub_45E480();
      dword_565FAC = GetTickCount() + v4 + 1;
    }
  }
  return v0;
}

// ===== sub_461C80 @ 0x00461C80..0x00461C85 =====
// attributes: thunk
int sub_461C80()
{
  return sub_4086B0();
}

// ===== sub_461C90 @ 0x00461C90..0x00461C9F =====
void sub_461C90()
{
  _DWORD *v0; // edi

  if ( sub_498810() )
  {
    sub_408870();
    v0 = dword_5076C8;
    if ( dword_5076C8 )
    {
      do
      {
        sub_44CFF0((_DWORD *)v0[1]);
        v0 = (_DWORD *)v0[5];
      }
      while ( v0 );
    }
    sub_408880();
  }
}

// ===== sub_461CA0 @ 0x00461CA0..0x00461CA5 =====
// attributes: thunk
int sub_461CA0()
{
  return sub_4087F0();
}

// ===== sub_461CB0 @ 0x00461CB0..0x00461CF2 =====
int sub_461CB0()
{
  int result; // eax
  int v1[4]; // [esp+0h] [ebp-10h] BYREF

  result = 0;
  if ( dword_5666F0 )
  {
    if ( !dword_565FB0 )
    {
      sub_4642D0();
      sub_443240(v1, 0, (int)dword_56674C);
      sub_461D80();
      return 1;
    }
  }
  return result;
}

// ===== sub_461D00 @ 0x00461D00..0x00461D06 =====
int __usercall sub_461D00@<eax>(int result@<eax>)
{
  dword_506BD4 = result;
  return result;
}

// ===== sub_461D10 @ 0x00461D10..0x00461D16 =====
int __usercall sub_461D10@<eax>(int result@<eax>)
{
  dword_506BD8 = result;
  return result;
}

// ===== sub_461D20 @ 0x00461D20..0x00461D4B =====
unsigned int __fastcall sub_461D20(unsigned int a1)
{
  unsigned int result; // eax

  if ( !a1 || (result = 0x3E8 / a1, (dword_506BCC = 0x3E8 / a1) == 0) )
    dword_506BCC = 1;
  dword_566744 = 0;
  return result;
}

// ===== sub_461D70 @ 0x00461D70..0x00461D7A =====
int sub_461D70()
{
  int result; // eax

  result = 1;
  if ( dword_566740 )
  {
    dword_506BD0 |= 1u;
  }
  else
  {
    dword_566740 = 1;
    dword_506BD0 = 1;
  }
  return result;
}

// ===== sub_461D80 @ 0x00461D80..0x00461D87 =====
int sub_461D80()
{
  int result; // eax

  result = 0;
  if ( !dword_566740 )
  {
    dword_566740 = 1;
    dword_506BD0 = 0;
  }
  return result;
}

// ===== sub_461D90 @ 0x00461D90..0x00461DA2 =====
int __usercall sub_461D90@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_407BC0(a4, a3, a2, a1);
}

// ===== sub_461DB0 @ 0x00461DB0..0x00461DDD =====
int __cdecl sub_461DB0(int a1, int a2, int a3, int a4, int a5)
{
  const CHAR *v5; // eax

  v5 = (const CHAR *)sub_468BB0();
  if ( v5 )
    return sub_407BF0(a4, a5, v5, a1, a2, a3);
  else
    return -2147483644;
}

// ===== sub_461DE0 @ 0x00461DE0..0x00461E0C =====
int __cdecl sub_461DE0(int a1, int a2, int a3, int a4)
{
  if ( sub_468BB0() )
    return sub_407C20(a4, a3, a1, a2);
  else
    return -2147483644;
}

// ===== sub_461E10 @ 0x00461E10..0x00461E15 =====
// attributes: thunk
int sub_461E10()
{
  return sub_4092D0();
}

// ===== sub_461E20 @ 0x00461E20..0x00461E25 =====
// attributes: thunk
int __usercall sub_461E20@<eax>(int result@<eax>)
{
  return sub_407B50(result);
}

// ===== sub_461E30 @ 0x00461E30..0x00461E3C =====
int __usercall sub_461E30@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_442E70((int *)dword_56674C, a2, a1);
}

// ===== sub_461E40 @ 0x00461E40..0x00461E53 =====
int __cdecl sub_461E40(int a1)
{
  int v1; // ecx

  return sub_442EC0(a1, v1, (int)dword_56674C);
}

// ===== sub_461E60 @ 0x00461E60..0x00461E65 =====
// attributes: thunk
int __usercall sub_461E60@<eax>(int a1@<eax>)
{
  return sub_442EE0(a1);
}

// ===== sub_461E70 @ 0x00461E70..0x00461E7E =====
int __usercall sub_461E70@<eax>(int a1@<eax>, int a2)
{
  return sub_442F80(a2, a1);
}

// ===== sub_461E80 @ 0x00461E80..0x00461E93 =====
int __cdecl sub_461E80(int a1)
{
  return sub_443130(a1, (int)dword_56674C);
}

// ===== sub_461EA0 @ 0x00461EA0..0x00461EAB =====
int __usercall sub_461EA0@<eax>(int a1@<eax>)
{
  int v1; // edx

  sub_430D20(a1, *((_DWORD *)dword_56674C + 5));
  return sub_430D10(*(_DWORD *)(v1 + 20));
}

// ===== sub_461EB0 @ 0x00461EB0..0x00461EB5 =====
// attributes: thunk
int sub_461EB0()
{
  return sub_443170();
}

// ===== sub_461EC0 @ 0x00461EC0..0x00461EC5 =====
// attributes: thunk
int __fastcall sub_461EC0(int a1, int a2)
{
  return sub_443180(a1, a2);
}

// ===== sub_461ED0 @ 0x00461ED0..0x00461EE7 =====
int __usercall sub_461ED0@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3, int a4, unsigned int a5)
{
  return sub_409AF0(a3, a4, a5, a2, a1);
}

// ===== sub_461EF0 @ 0x00461EF0..0x00461F0F =====
int __usercall sub_461EF0@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3, int a4, unsigned int a5, int a6, int a7)
{
  return sub_409C60(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_461F10 @ 0x00461F10..0x00461F1E =====
BOOL __usercall sub_461F10@<eax>(int a1@<eax>, int a2)
{
  return sub_443310(a2, a1);
}

// ===== sub_461F20 @ 0x00461F20..0x00461F2E =====
BOOL __usercall sub_461F20@<eax>(int a1@<eax>, int a2)
{
  return sub_443380(a2, a1);
}

// ===== sub_461F30 @ 0x00461F30..0x00461F3E =====
BOOL __usercall sub_461F30@<eax>(int a1@<eax>, int a2)
{
  return sub_4433F0(a2, a1);
}

// ===== sub_461F40 @ 0x00461F40..0x00461F4E =====
BOOL __usercall sub_461F40@<eax>(int a1@<eax>, int a2)
{
  return sub_443460(a2, a1);
}

// ===== sub_461F50 @ 0x00461F50..0x00461F5F =====
BOOL __usercall sub_461F50@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_4434C0(a3, a2, a1);
}

// ===== sub_461F60 @ 0x00461F60..0x00461F73 =====
BOOL __usercall sub_461F60@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_443520(a3, a4, a2, a1);
}

// ===== sub_461F80 @ 0x00461F80..0x00461F8E =====
BOOL __usercall sub_461F80@<eax>(int a1@<eax>, int a2)
{
  return sub_4435B0(a2, a1);
}

// ===== sub_461F90 @ 0x00461F90..0x00461F9E =====
BOOL __usercall sub_461F90@<eax>(int a1@<eax>, int a2)
{
  return sub_443610(a2, a1);
}

// ===== sub_461FA0 @ 0x00461FA0..0x00461FAE =====
BOOL __usercall sub_461FA0@<eax>(int a1@<eax>, int a2)
{
  return sub_443670(a2, a1);
}

// ===== sub_461FB0 @ 0x00461FB0..0x00461FBF =====
BOOL __usercall sub_461FB0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_4436D0(a3, a2, a1);
}

// ===== sub_461FC0 @ 0x00461FC0..0x00461FCF =====
BOOL __usercall sub_461FC0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_443730(a3, a2, a1);
}

// ===== sub_461FD0 @ 0x00461FD0..0x00461FE3 =====
BOOL __usercall sub_461FD0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_443790(a3, a4, a2, a1);
}

// ===== sub_461FF0 @ 0x00461FF0..0x00462003 =====
BOOL __usercall sub_461FF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_443820(a3, a4, a2, a1);
}

// ===== sub_462010 @ 0x00462010..0x00462023 =====
int __usercall sub_462010@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_4438B0(a3, a4, a2, a1);
}

// ===== sub_462030 @ 0x00462030..0x00462038 =====
int __usercall sub_462030@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<esi>)
{
  return sub_443960(a3, a2, a1);
}

// ===== sub_462040 @ 0x00462040..0x0046204E =====
int __usercall sub_462040@<eax>(int a1@<eax>, int a2)
{
  return sub_4439B0(a2, a1);
}

// ===== sub_462050 @ 0x00462050..0x00462057 =====
int __usercall sub_462050@<eax>(int a1@<eax>, void *a2@<ecx>)
{
  return sub_443A40(a2, a1);
}

// ===== sub_462060 @ 0x00462060..0x00462065 =====
// attributes: thunk
BOOL __usercall sub_462060@<eax>(int a1@<esi>)
{
  return sub_443BD0(a1);
}

// ===== sub_462070 @ 0x00462070..0x004620D7 =====
BOOL __usercall sub_462070@<eax>(int a1@<eax>, _DWORD *a2)
{
  int v3; // eax
  _DWORD v5[2]; // [esp+10h] [ebp-10h] BYREF
  _DWORD v6[2]; // [esp+18h] [ebp-8h] BYREF

  v3 = sub_443270((int)dword_56674C, a1);
  if ( !v3 )
    return 0;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v3 + 52))(v3, v6);
  sub_48E680(v5);
  return sub_443B90(a1, a2, v5[0] - v6[0], v5[1] - v6[1]);
}

// ===== sub_4620E0 @ 0x004620E0..0x004620E5 =====
// attributes: thunk
int __usercall sub_4620E0@<eax>(int a1@<esi>)
{
  return sub_443C00(a1);
}

// ===== sub_4620F0 @ 0x004620F0..0x004620F7 =====
int __usercall sub_4620F0@<eax>(int a1@<eax>, int a2@<esi>)
{
  return sub_443C50(a2, a1);
}

// ===== sub_462100 @ 0x00462100..0x00462113 =====
int __usercall sub_462100@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_443C90(a3, a4, a2, a1);
}

// ===== sub_462120 @ 0x00462120..0x0046212E =====
int __usercall sub_462120@<eax>(int a1@<eax>, int a2)
{
  return sub_443D40(a2, a1);
}

// ===== sub_462130 @ 0x00462130..0x00462137 =====
int __usercall sub_462130@<eax>(int a1@<eax>)
{
  return sub_43D680(a1);
}

// ===== sub_462140 @ 0x00462140..0x00462150 =====
int __usercall sub_462140@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_43D6B0(a3, a2, a1);
}

// ===== sub_462150 @ 0x00462150..0x0046216C =====
int __usercall sub_462150@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_43D6F0(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462170 @ 0x00462170..0x00462198 =====
int __usercall sub_462170@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  return sub_43D750(a3, a4, a5, a6, a7, a8, a9, a2, a1);
}

// ===== sub_4621A0 @ 0x004621A0..0x004621B0 =====
int __usercall sub_4621A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_43D840(a3, a2, a1);
}

// ===== sub_4621B0 @ 0x004621B0..0x004621C8 =====
int __usercall sub_4621B0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5)
{
  return sub_43D8B0(a3, a4, a5, a2, a1);
}

// ===== sub_4621D0 @ 0x004621D0..0x004621E0 =====
int __usercall sub_4621D0@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3)
{
  return sub_43D980(a3, a2, a1);
}

// ===== sub_4621E0 @ 0x004621E0..0x004621F8 =====
int __usercall sub_4621E0@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4, int a5)
{
  return sub_43DA10(a3, a4, a5, a2, a1);
}

// ===== sub_462200 @ 0x00462200..0x00462218 =====
int __usercall sub_462200@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4, int a5)
{
  return sub_43DB00(a3, a4, a5, a2, a1);
}

// ===== sub_462220 @ 0x00462220..0x00462230 =====
int __usercall sub_462220@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_43DB90(a3, a2, a1);
}

// ===== sub_462230 @ 0x00462230..0x00462248 =====
int __usercall sub_462230@<eax>(unsigned int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5)
{
  return sub_43DC00(a3, a4, a5, a2, a1);
}

// ===== sub_462250 @ 0x00462250..0x00462278 =====
int __usercall sub_462250@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  return sub_43DCB0(a3, a4, a5, a6, a7, a8, a9, a2, a1);
}

// ===== sub_462280 @ 0x00462280..0x00462287 =====
int __usercall sub_462280@<eax>(unsigned int a1@<eax>)
{
  return sub_43DD60(a1);
}

// ===== sub_462290 @ 0x00462290..0x00462298 =====
int __usercall sub_462290@<eax>(int a1@<eax>, unsigned int a2@<ecx>)
{
  return sub_43DDB0(a2, a1);
}

// ===== sub_4622A0 @ 0x004622A0..0x004622A8 =====
int __usercall sub_4622A0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, unsigned int a3@<edi>)
{
  return sub_43DE10(a3, a2, a1);
}

// ===== sub_4622B0 @ 0x004622B0..0x004622B7 =====
int __usercall sub_4622B0@<eax>(int a1@<eax>, unsigned int a2@<esi>)
{
  return sub_43DE90(a2, a1);
}

// ===== sub_4622C0 @ 0x004622C0..0x004622C7 =====
int __usercall sub_4622C0@<eax>(int a1@<eax>, unsigned int a2@<esi>)
{
  return sub_43DF00(a2, a1);
}

// ===== sub_4622D0 @ 0x004622D0..0x004622DE =====
int __usercall sub_4622D0@<eax>(int a1@<eax>, unsigned int a2@<edi>)
{
  return sub_43DF70(a2, a1);
}

// ===== sub_4622E0 @ 0x004622E0..0x004622F2 =====
int __usercall sub_4622E0@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3@<esi>)
{
  return sub_43E000(a3, a2, a1);
}

// ===== sub_462300 @ 0x00462300..0x0046230F =====
int __usercall sub_462300@<eax>(int a1@<eax>, unsigned int a2@<ecx>)
{
  return sub_43E090(a2, a1);
}

// ===== sub_462310 @ 0x00462310..0x0046231F =====
int __usercall sub_462310@<eax>(int a1@<eax>, unsigned int a2@<ecx>)
{
  return sub_43E0E0(a2, a1);
}

// ===== sub_462320 @ 0x00462320..0x00462330 =====
int __usercall sub_462320@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3@<esi>, int a4)
{
  return sub_43E130(a3, a4, a2, a1);
}

// ===== sub_462330 @ 0x00462330..0x0046233E =====
int __usercall sub_462330@<eax>(int a1@<eax>, int a2@<edi>)
{
  return sub_43E490(a1, a2, (int *)dword_56674C);
}

// ===== sub_462340 @ 0x00462340..0x00462345 =====
// attributes: thunk
int sub_462340()
{
  return sub_43E4C0();
}

// ===== sub_462350 @ 0x00462350..0x00462355 =====
// attributes: thunk
int sub_462350()
{
  return sub_43E510();
}

// ===== sub_462360 @ 0x00462360..0x00462367 =====
BOOL __usercall sub_462360@<eax>(int a1@<eax>)
{
  return sub_43E610(a1);
}

// ===== sub_462370 @ 0x00462370..0x0046238F =====
int __usercall sub_462370@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  return sub_43E690(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_462390 @ 0x00462390..0x004623B7 =====
int __usercall sub_462390@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  return sub_43E730(a3, a4, a5, a6, a7, a8, a9, a2, a1);
}

// ===== sub_4623C0 @ 0x004623C0..0x004623FD =====
int __cdecl sub_4623C0(
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
  return sub_43E7F0(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

// ===== sub_462400 @ 0x00462400..0x0046242B =====
int __usercall sub_462400@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  return sub_43E8C0(a3, a4, a5, a6, a7, a8, a9, a10, a2, a1);
}

// ===== sub_462430 @ 0x00462430..0x0046245B =====
int __usercall sub_462430@<eax>(
        int a1@<eax>,
        int a2@<ecx>,
        int a3,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10)
{
  return sub_43E990(a3, a4, a5, a6, a7, a8, a9, a10, a2, a1);
}

// ===== sub_462460 @ 0x00462460..0x004624AD =====
int __cdecl sub_462460(
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
  return sub_43EAB0(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
}

// ===== sub_4624B0 @ 0x004624B0..0x00462509 =====
int __cdecl sub_4624B0(
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
        int a18,
        int a19,
        int a20)
{
  return sub_43EBA0(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
}

// ===== sub_462510 @ 0x00462510..0x00462518 =====
int __usercall sub_462510@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43ECA0(a2, a1);
}

// ===== sub_462520 @ 0x00462520..0x00462528 =====
int __usercall sub_462520@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43ED20(a2, a1);
}

// ===== sub_462530 @ 0x00462530..0x0046253E =====
int __usercall sub_462530@<eax>(int a1@<eax>, int a2)
{
  return sub_43EDA0(a2, a1);
}

// ===== sub_462540 @ 0x00462540..0x00462548 =====
BOOL __usercall sub_462540@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43EE70(a2, a1);
}

// ===== sub_462550 @ 0x00462550..0x00462560 =====
int __usercall sub_462550@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_43EEE0(a3, a2, a1);
}

// ===== sub_462560 @ 0x00462560..0x00462565 =====
// attributes: thunk
int sub_462560()
{
  return sub_43EF90();
}

// ===== sub_462570 @ 0x00462570..0x00462577 =====
BOOL __usercall sub_462570@<eax>(int a1@<eax>)
{
  return sub_43F090(a1);
}

// ===== sub_462580 @ 0x00462580..0x0046259F =====
int __usercall sub_462580@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  return sub_43F110(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_4625A0 @ 0x004625A0..0x004625A8 =====
BOOL __usercall sub_4625A0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43F1D0(a2, a1);
}

// ===== sub_4625B0 @ 0x004625B0..0x004625B5 =====
// attributes: thunk
int sub_4625B0()
{
  return sub_43F290();
}

// ===== sub_4625C0 @ 0x004625C0..0x004625C7 =====
BOOL __usercall sub_4625C0@<eax>(int a1@<eax>)
{
  return sub_43F390(a1);
}

// ===== sub_4625D0 @ 0x004625D0..0x004625EB =====
int __usercall sub_4625D0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_43F410(a3, a4, a5, a6, a2, a1);
}

// ===== sub_4625F0 @ 0x004625F0..0x00462603 =====
int __usercall sub_4625F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, unsigned int a4)
{
  return sub_43F4E0(a3, a4, a2, a1);
}

// ===== sub_462610 @ 0x00462610..0x0046262B =====
int __usercall sub_462610@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, unsigned int a6)
{
  return sub_43F560(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462630 @ 0x00462630..0x00462657 =====
int __usercall sub_462630@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  return sub_43F640(a3, a4, a5, a6, a7, a8, a9, a2, a1);
}

// ===== sub_462660 @ 0x00462660..0x0046266F =====
int __usercall sub_462660@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_43F6D0(a3, a2, a1);
}

// ===== sub_462670 @ 0x00462670..0x0046267E =====
BOOL __usercall sub_462670@<eax>(int a1@<eax>, int a2)
{
  return sub_43F730(a2, a1);
}

// ===== sub_462680 @ 0x00462680..0x00462685 =====
// attributes: thunk
int sub_462680()
{
  return sub_43F7F0();
}

// ===== sub_462690 @ 0x00462690..0x00462697 =====
BOOL __usercall sub_462690@<eax>(int a1@<eax>)
{
  return sub_43F8F0(a1);
}

// ===== sub_4626A0 @ 0x004626A0..0x004626A8 =====
BOOL __usercall sub_4626A0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43F970(a2, a1);
}

// ===== sub_4626B0 @ 0x004626B0..0x004626CF =====
int __usercall sub_4626B0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  return sub_43F9E0(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_4626D0 @ 0x004626D0..0x004626E8 =====
int __usercall sub_4626D0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, unsigned int a4, unsigned int a5)
{
  return sub_43FA70(a3, a4, a5, a2, a1);
}

// ===== sub_4626F0 @ 0x004626F0..0x00462704 =====
int __usercall sub_4626F0@<eax>(int a1@<ecx>, void *a2@<eax>, int a3, int a4)
{
  return sub_43FB10(a3, a4, a1, a2);
}

// ===== sub_462710 @ 0x00462710..0x0046272C =====
int __usercall sub_462710@<eax>(
        int a1@<eax>,
        unsigned int a2@<ecx>,
        int a3,
        unsigned int a4,
        unsigned int a5,
        unsigned int a6)
{
  return sub_43FB50(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462730 @ 0x00462730..0x00462738 =====
int __usercall sub_462730@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43FBC0(a2, a1);
}

// ===== sub_462740 @ 0x00462740..0x0046275C =====
int __usercall sub_462740@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3, int a4, int a5, int a6)
{
  return sub_43FC50(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462760 @ 0x00462760..0x00462767 =====
BOOL __usercall sub_462760@<eax>(int a1@<eax>)
{
  return sub_43FD70(a1);
}

// ===== sub_462770 @ 0x00462770..0x004627D7 =====
int __usercall sub_462770@<eax>(int a1@<esi>, int *a2, int a3)
{
  int v3; // eax
  _DWORD v5[2]; // [esp+8h] [ebp-10h] BYREF
  _DWORD v6[2]; // [esp+10h] [ebp-8h] BYREF

  v3 = sub_43FD30(a1, (int)dword_56674C);
  if ( !v3 )
    return 255;
  (*(void (__thiscall **)(int, _DWORD *))(*(_DWORD *)v3 + 52))(v3, v6);
  sub_48E680(v5);
  return sub_43FDF0(a2, a1, v5[0] - v6[0], v5[1] - v6[1], a3);
}

// ===== sub_4627E0 @ 0x004627E0..0x004627E8 =====
BOOL __usercall sub_4627E0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_43FE60(a2, a1);
}

// ===== sub_4627F0 @ 0x004627F0..0x0046280B =====
int __usercall sub_4627F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_43FED0(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462810 @ 0x00462810..0x0046282C =====
int __usercall sub_462810@<eax>(
        int a1@<eax>,
        unsigned int a2@<ecx>,
        int a3,
        unsigned int a4,
        unsigned int a5,
        unsigned int a6)
{
  return sub_43FF50(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462830 @ 0x00462830..0x0046284F =====
int __usercall sub_462830@<eax>(
        unsigned int *a1@<eax>,
        unsigned int a2@<ecx>,
        int a3,
        int a4,
        unsigned int a5,
        int a6,
        int a7)
{
  return sub_440000(a3, a4, a5, a6, a7, a2, a1);
}

// ===== sub_462850 @ 0x00462850..0x00462863 =====
int __usercall sub_462850@<eax>(_DWORD *a1@<eax>, unsigned int a2@<ecx>, int a3, unsigned int a4)
{
  return sub_4400C0(a3, a4, a2, a1);
}

// ===== sub_462870 @ 0x00462870..0x00462884 =====
int __usercall sub_462870@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4)
{
  return sub_440170(a3, a4, a2, a1);
}

// ===== sub_462890 @ 0x00462890..0x004628AC =====
int __usercall sub_462890@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3, unsigned int a4, int a5, unsigned int a6)
{
  return sub_4401D0(a3, a4, a5, a6, a2, a1);
}

// ===== sub_4628B0 @ 0x004628B0..0x004628C0 =====
int __usercall sub_4628B0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3)
{
  return sub_4402D0(a3, a2, a1);
}

// ===== sub_4628C0 @ 0x004628C0..0x004628D3 =====
int __usercall sub_4628C0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, unsigned int a4)
{
  return sub_440360(a3, a4, a2, a1);
}

// ===== sub_4628E0 @ 0x004628E0..0x004628F4 =====
unsigned int __usercall sub_4628E0@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, _DWORD *a3, int a4)
{
  return sub_440400(a3, a4, a2, a1);
}

// ===== sub_462900 @ 0x00462900..0x00462914 =====
unsigned int __usercall sub_462900@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, int a3, int a4)
{
  return sub_440480(a3, a4, a2, a1);
}

// ===== sub_462920 @ 0x00462920..0x00462934 =====
int __usercall sub_462920@<eax>(int a1@<eax>, int a2)
{
  return sub_440570(a2, a1);
}

// ===== sub_462940 @ 0x00462940..0x0046297B =====
int __usercall sub_462940@<eax>(unsigned int a1@<eax>, unsigned int a2@<ecx>, _DWORD *a3)
{
  int v3; // eax
  int v4; // eax

  v3 = sub_4405B0(a3, a2, a1);
  if ( !v3 )
    return 0;
  v4 = v3 - 9;
  if ( !v4 )
    return 1;
  if ( v4 == 1 )
    return 2;
  return a2;
}

// ===== sub_462980 @ 0x00462980..0x00462987 =====
BOOL __usercall sub_462980@<eax>(int a1@<eax>)
{
  return sub_440700(a1);
}

// ===== sub_462990 @ 0x00462990..0x004629D2 =====
int __usercall sub_462990@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // esi
  int result; // eax

  v2 = 0;
  switch ( sub_440780(a2, a1) )
  {
    case 1:
      result = 1;
      break;
    case 2:
      result = 2;
      break;
    case 3:
      result = 3;
      break;
    case 255:
      v2 = -1;
      goto LABEL_6;
    default:
LABEL_6:
      result = v2;
      break;
  }
  return result;
}

// ===== sub_462AF0 @ 0x00462AF0..0x00462B0B =====
BOOL __usercall sub_462AF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_440830(a3, a4, a5, a6, a2, a1);
}

// ===== sub_462B10 @ 0x00462B10..0x00462B18 =====
BOOL __usercall sub_462B10@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4408A0(a2, a1);
}

// ===== sub_462B20 @ 0x00462B20..0x00462B58 =====
int __usercall sub_462B20@<eax>(int a1@<eax>, int a2@<ecx>, int a3, unsigned int a4, unsigned int a5)
{
  int v5; // eax

  v5 = sub_4408D0(a3, a4, a5, a2, a1);
  if ( v5 == 4 )
    return 1;
  if ( v5 == 255 )
    return -1;
  return 0;
}

// ===== sub_462B60 @ 0x00462B60..0x00462B8A =====
int __usercall sub_462B60@<eax>(int a1@<eax>, _DWORD *a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440910(a2, a1);
  if ( !v2 )
    return 0;
  if ( v2 == 255 )
    return -1;
  return (int)a2;
}

// ===== sub_462B90 @ 0x00462B90..0x00462BB6 =====
int __usercall sub_462B90@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440940(a2, a1);
  if ( v2 == 18 )
    return 1;
  if ( v2 == 255 )
    return -1;
  return 0;
}

// ===== sub_462BC0 @ 0x00462BC0..0x00462BE6 =====
int __usercall sub_462BC0@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440980(a2, a1);
  if ( v2 == 19 )
    return 1;
  if ( v2 == 255 )
    return -1;
  return 0;
}

// ===== sub_462BF0 @ 0x00462BF0..0x00462C5F =====
int __usercall sub_462BF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  const CHAR *v7; // eax
  unsigned int v8; // eax

  v7 = (const CHAR *)sub_468BB0();
  v8 = sub_4409C0(a3, v7, a5, a6, a7, a2, a1);
  if ( v8 > 6 )
  {
    if ( v8 == 7 )
      return 3;
    if ( v8 == 255 )
      return -1;
  }
  else
  {
    switch ( v8 )
    {
      case 6u:
        return 2;
      case 0u:
        return 0;
      case 5u:
        return 1;
    }
  }
  return a2;
}

// ===== sub_462C60 @ 0x00462C60..0x00462C98 =====
int __usercall sub_462C60@<eax>(unsigned int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440A50(a2, a1);
  switch ( v2 )
  {
    case 0:
      return 0;
    case 8:
      return 1;
    case 255:
      return -1;
  }
  return a2;
}

// ===== sub_462CA0 @ 0x00462CA0..0x00462CA8 =====
BOOL __usercall sub_462CA0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_440A90(a2, a1);
}

// ===== sub_462CB0 @ 0x00462CB0..0x00462CB8 =====
BOOL __usercall sub_462CB0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_440B00(a2, a1);
}

// ===== sub_462CC0 @ 0x00462CC0..0x00462CEA =====
int __usercall sub_462CC0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440B50(a2, a1);
  if ( !v2 )
    return 0;
  if ( v2 == 255 )
    return -1;
  return a2;
}

// ===== sub_462CF0 @ 0x00462CF0..0x00462D74 =====
int __usercall sub_462CF0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  unsigned int v6; // eax
  int result; // eax

  v6 = sub_440B80(a3, a4, a5, a6, a2, a1);
  if ( v6 > 0xC )
  {
    switch ( v6 )
    {
      case 0xDu:
        result = 3;
        break;
      case 0xEu:
        result = 4;
        break;
      case 0xFu:
        result = 5;
        break;
      case 0xFFu:
        result = -1;
        break;
      default:
        return a2;
    }
  }
  else if ( v6 == 12 )
  {
    return 2;
  }
  else if ( v6 )
  {
    if ( v6 == 2 )
      return 1;
    else
      return a2;
  }
  else
  {
    return 0;
  }
  return result;
}

// ===== sub_462E80 @ 0x00462E80..0x00462EB8 =====
int __usercall sub_462E80@<eax>(int a1@<eax>, void *a2@<ecx>)
{
  int v2; // eax
  int v3; // eax

  v2 = sub_440C80(a2, a1);
  if ( !v2 )
    return 0;
  v3 = v2 - 2;
  if ( !v3 )
    return 1;
  if ( v3 == 253 )
    return -1;
  return (int)a2;
}

// ===== sub_462EC0 @ 0x00462EC0..0x00462EF8 =====
int __usercall sub_462EC0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v2; // eax

  v2 = sub_440D60(a2, a1);
  switch ( v2 )
  {
    case 0:
      return 0;
    case 20:
      return 1;
    case 255:
      return -1;
  }
  return a2;
}

// ===== sub_462F00 @ 0x00462F00..0x00462F08 =====
BOOL __usercall sub_462F00@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_440DC0(a2, a1);
}

// ===== sub_462F10 @ 0x00462F10..0x00462F94 =====
int __usercall sub_462F10@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, void *a6)
{
  unsigned int v6; // eax
  int result; // eax

  v6 = sub_440E10(a3, a4, a5, a6, a2, a1);
  if ( v6 > 0xC )
  {
    switch ( v6 )
    {
      case 0xDu:
        result = 3;
        break;
      case 0xEu:
        result = 4;
        break;
      case 0xFu:
        result = 5;
        break;
      case 0xFFu:
        result = -1;
        break;
      default:
        return a2;
    }
  }
  else if ( v6 == 12 )
  {
    return 2;
  }
  else if ( v6 )
  {
    if ( v6 == 2 )
      return 1;
    else
      return a2;
  }
  else
  {
    return 0;
  }
  return result;
}

// ===== sub_4630A0 @ 0x004630A0..0x004630EC =====
int __usercall sub_4630A0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, const char *a4, int a5, int a6, int a7)
{
  int v7; // eax

  v7 = sub_440F10(a3, a4, a5, a6, a7, a2, a1);
  switch ( v7 )
  {
    case 0:
      return 0;
    case 17:
      return 1;
    case 255:
      return -1;
  }
  return a2;
}

// ===== sub_4630F0 @ 0x004630F0..0x004630F7 =====
BOOL __usercall sub_4630F0@<eax>(int a1@<eax>)
{
  return sub_440F80(a1);
}

// ===== sub_463100 @ 0x00463100..0x00463110 =====
BOOL __usercall sub_463100@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_440FE0(a3, a2, a1);
}

// ===== sub_463110 @ 0x00463110..0x00463118 =====
BOOL __usercall sub_463110@<eax>(int a1@<eax>, _DWORD *a2@<ecx>)
{
  return sub_441010(a2, a1);
}

// ===== sub_463120 @ 0x00463120..0x0046314A =====
int __usercall sub_463120@<eax>(int a1@<eax>, BOOL *a2@<ecx>)
{
  int v2; // eax

  v2 = sub_441040(a2, a1);
  if ( !v2 )
    return 0;
  if ( v2 == 255 )
    return -1;
  return (int)a2;
}

// ===== sub_463150 @ 0x00463150..0x00463155 =====
// attributes: thunk
int __usercall sub_463150@<eax>(int result@<eax>)
{
  return sub_4334C0(result);
}

// ===== sub_463160 @ 0x00463160..0x00463165 =====
// attributes: thunk
BOOL __usercall sub_463160@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4334D0(a1, a2);
}

// ===== sub_463170 @ 0x00463170..0x00463175 =====
// attributes: thunk
BOOL __usercall sub_463170@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4334F0(a1, a2);
}

// ===== sub_463180 @ 0x00463180..0x00463185 =====
// attributes: thunk
int __usercall sub_463180@<eax>(int result@<eax>, int a2@<ecx>)
{
  return sub_433510(result, a2);
}

// ===== sub_463190 @ 0x00463190..0x004631AD =====
int __usercall sub_463190@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_432E40(a4, a3, a5, a6, a2, a1);
}

// ===== sub_4631B0 @ 0x004631B0..0x004631BB =====
int __usercall sub_4631B0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_432F50(a2, a1);
}

// ===== sub_4631C0 @ 0x004631C0..0x004631D3 =====
int __usercall sub_4631C0@<eax>(_DWORD *a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_433300(a3, a2, a1);
}

// ===== sub_4631E0 @ 0x004631E0..0x004631E5 =====
// attributes: thunk
int __usercall sub_4631E0@<eax>(int result@<eax>)
{
  return sub_433480(result);
}

// ===== sub_4631F0 @ 0x004631F0..0x00463201 =====
int __usercall sub_4631F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_433490(a3, a2, a1);
}

// ===== sub_463210 @ 0x00463210..0x00463215 =====
// attributes: thunk
int __usercall sub_463210@<eax>(int result@<eax>, int a2@<ecx>)
{
  return sub_4334B0(result, a2);
}

// ===== sub_463220 @ 0x00463220..0x00463225 =====
// attributes: thunk
int __usercall sub_463220@<eax>(int result@<eax>)
{
  return sub_433520(result);
}

// ===== sub_463230 @ 0x00463230..0x00463235 =====
// attributes: thunk
int __usercall sub_463230@<eax>(unsigned int a1@<edx>, unsigned int a2@<ecx>, unsigned int a3@<esi>)
{
  return sub_433530(a1, a2, a3);
}

// ===== sub_463240 @ 0x00463240..0x00463245 =====
// attributes: thunk
int __usercall sub_463240@<eax>(int result@<eax>)
{
  return sub_432D70(result);
}

// ===== sub_463250 @ 0x00463250..0x00463255 =====
// attributes: thunk
int __usercall sub_463250@<eax>(int a1@<edx>, unsigned int a2@<esi>)
{
  return sub_432D80(a1, a2);
}

// ===== sub_463260 @ 0x00463260..0x00463265 =====
// attributes: thunk
int __usercall sub_463260@<eax>(int result@<eax>)
{
  return sub_432DC0(result);
}

// ===== sub_463270 @ 0x00463270..0x00463298 =====
int __usercall sub_463270@<eax>(int a1@<eax>, const char *a2@<ecx>)
{
  if ( a2 )
  {
    if ( a1 )
    {
      sub_4344E0();
      return 1;
    }
    else
    {
      return sub_434730(a2, (int)&unk_565BB4, 0);
    }
  }
  else
  {
    sub_434800();
    return 1;
  }
}

// ===== sub_4632A0 @ 0x004632A0..0x004632AA =====
int __usercall sub_4632A0@<eax>(const char *a1@<eax>)
{
  return sub_434920(a1);
}

// ===== sub_4632B0 @ 0x004632B0..0x004632B5 =====
// attributes: thunk
int __thiscall sub_4632B0(void *this)
{
  return sub_4344F0(this);
}

// ===== sub_4632C0 @ 0x004632C0..0x004632E7 =====
int __usercall sub_4632C0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6, int a7)
{
  const char *v7; // eax

  v7 = (const char *)sub_468BB0();
  return sub_434440(v7, a4, a5, a6, a7, a2, a1);
}

// ===== sub_4632F0 @ 0x004632F0..0x0046330D =====
int __usercall sub_4632F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4, int a5, int a6)
{
  return sub_434340(a5, a6, a3, a4, a2, a1);
}

// ===== sub_463310 @ 0x00463310..0x00463315 =====
// attributes: thunk
int __fastcall sub_463310(int a1, unsigned int a2)
{
  return sub_4343B0(a1, a2);
}

// ===== sub_463320 @ 0x00463320..0x0046332A =====
int __usercall sub_463320@<eax>(int a1@<eax>)
{
  return sub_434410(a1);
}

// ===== sub_463330 @ 0x00463330..0x00463335 =====
// attributes: thunk
int __thiscall sub_463330(void *this)
{
  return sub_437EB0(this);
}

// ===== sub_463340 @ 0x00463340..0x0046334B =====
int __usercall sub_463340@<eax>(char *a1@<ecx>, char *a2@<eax>)
{
  return sub_437EE0(a1, a2);
}

// ===== sub_463350 @ 0x00463350..0x0046336F =====
unsigned int __usercall sub_463350@<eax>(DWORD a1@<eax>, int a2@<ecx>, int a3, int a4, int a5)
{
  const char *v5; // eax

  v5 = (const char *)sub_468BB0(a2, a3);
  return sub_437FA0(a4, v5, a5, a2, a1);
}

// ===== sub_463370 @ 0x00463370..0x00463375 =====
// attributes: thunk
int __usercall sub_463370@<eax>(int result@<eax>)
{
  return sub_438050(result);
}

// ===== sub_463380 @ 0x00463380..0x00463385 =====
// attributes: thunk
int __usercall sub_463380@<eax>(const char *Src@<ecx>, _BYTE *a2@<eax>)
{
  return sub_438070(Src, a2);
}

// ===== sub_463390 @ 0x00463390..0x00463395 =====
// attributes: thunk
int __fastcall sub_463390(unsigned int a1)
{
  return sub_407B70(a1);
}

// ===== sub_4633A0 @ 0x004633A0..0x004633A5 =====
// attributes: thunk
int __usercall sub_4633A0@<eax>(int result@<eax>, int a2@<ecx>)
{
  return sub_43B5A0(result, a2);
}

// ===== sub_4633B0 @ 0x004633B0..0x004633B5 =====
// attributes: thunk
int __usercall sub_4633B0@<eax>(int result@<eax>)
{
  return sub_43B590(result);
}

// ===== sub_4633C0 @ 0x004633C0..0x004633D5 =====
int __usercall sub_4633C0@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_43B5B0(a3, a4, a2, a1);
}

// ===== sub_4633E0 @ 0x004633E0..0x004633E8 =====
BOOL __usercall sub_4633E0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_441080(a2, a1);
}

// ===== sub_4633F0 @ 0x004633F0..0x004633FF =====
int __usercall sub_4633F0@<eax>(int a1@<eax>)
{
  int v1; // eax
  int result; // eax

  v1 = sub_43A360(a1);
  result = sub_43B560(v1);
  dword_565D84 = result;
  return result;
}

// ===== sub_463400 @ 0x00463400..0x0046343E =====
int __usercall sub_463400@<eax>(_DWORD *a1@<eax>, int a2@<ecx>)
{
  int result; // eax

  switch ( sub_442100(a1) )
  {
    case 0:
      result = 0;
      break;
    case 1:
      result = 1;
      break;
    case 2:
      result = 2;
      break;
    case 3:
      result = 3;
      break;
    default:
      result = a2;
      break;
  }
  return result;
}

// ===== sub_463450 @ 0x00463450..0x00463455 =====
// attributes: thunk
BOOL __usercall sub_463450@<eax>(int a1@<eax>)
{
  return sub_442260(a1);
}

// ===== sub_463460 @ 0x00463460..0x00463468 =====
BOOL __usercall sub_463460@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4422B0(a2, a1);
}

// ===== sub_463470 @ 0x00463470..0x00463480 =====
BOOL __usercall sub_463470@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<edi>, int a4)
{
  return sub_442320(a3, a4, a2, a1);
}

// ===== sub_463480 @ 0x00463480..0x00463490 =====
BOOL __usercall sub_463480@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  return sub_442400(a3, a2, a1);
}

// ===== sub_463490 @ 0x00463490..0x00463498 =====
BOOL __usercall sub_463490@<eax>(_DWORD *a1@<eax>, int a2@<ecx>)
{
  return sub_442460(a2, a1);
}

// ===== sub_4634A0 @ 0x004634A0..0x004634A8 =====
BOOL __usercall sub_4634A0@<eax>(_DWORD *a1@<eax>, int a2@<ecx>)
{
  return sub_442490(a2, a1);
}

// ===== sub_4634B0 @ 0x004634B0..0x004634EC =====
int __usercall sub_4634B0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  int v3; // eax

  v3 = sub_442380(a3, a2, a1);
  switch ( v3 )
  {
    case 0:
      return 0;
    case 4:
      return 4;
    case 255:
      return -1;
  }
  return a2;
}

// ===== sub_4634F0 @ 0x004634F0..0x0046352C =====
int __usercall sub_4634F0@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  int v3; // eax

  v3 = sub_4423C0(a3, a2, a1);
  switch ( v3 )
  {
    case 0:
      return 0;
    case 5:
      return 5;
    case 255:
      return -1;
  }
  return a2;
}

// ===== sub_463530 @ 0x00463530..0x00463538 =====
BOOL __usercall sub_463530@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4424E0(a2, a1);
}

// ===== sub_463540 @ 0x00463540..0x00463545 =====
// attributes: thunk
int sub_463540()
{
  return sub_442560();
}

// ===== sub_463550 @ 0x00463550..0x00463555 =====
// attributes: thunk
BOOL __usercall sub_463550@<eax>(int a1@<eax>)
{
  return sub_442650(a1);
}

// ===== sub_463560 @ 0x00463560..0x00463568 =====
BOOL __usercall sub_463560@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_4426A0(a2, a1);
}

// ===== sub_463570 @ 0x00463570..0x00463584 =====
BOOL __usercall sub_463570@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  return sub_442710(a3, a4, a2, a1);
}

// ===== sub_463590 @ 0x00463590..0x004635E6 =====
int __usercall sub_463590@<eax>(int a1@<eax>, int a2@<ecx>, int a3, int a4)
{
  unsigned int v4; // eax
  int result; // eax

  v4 = sub_442780(a3, a4, a2, a1);
  if ( v4 > 0xFF )
    return a2;
  if ( v4 == 255 )
    return -1;
  switch ( v4 )
  {
    case 0u:
      result = 0;
      break;
    case 1u:
      result = 1;
      break;
    case 3u:
      result = 3;
      break;
    case 4u:
      result = 4;
      break;
    default:
      return a2;
  }
  return result;
}

// ===== sub_463600 @ 0x00463600..0x00463649 =====
int __usercall sub_463600@<eax>(int a1@<eax>, int a2@<ecx>, int a3)
{
  unsigned int v3; // eax

  v3 = sub_4427F0(a3, a1);
  if ( v3 > 2 )
  {
    if ( v3 == 255 )
      return -1;
    else
      return a2;
  }
  else if ( v3 == 2 )
  {
    return 2;
  }
  else
  {
    return v3 != 0;
  }
}

// ===== sub_463650 @ 0x00463650..0x00463696 =====
int sub_463650()
{
  void *v0; // edi
  void *v1; // ebx

  v0 = dword_565FC4;
  while ( v0 )
  {
    v1 = v0;
    v0 = (void *)*((_DWORD *)v0 + 4);
    sub_46D800();
    operator delete(v1);
  }
  dword_565FC4 = 0;
  return sub_4637C0(0, 0);
}

// ===== sub_4636A0 @ 0x004636A0..0x0046371E =====
BOOL __cdecl sub_4636A0(int a1)
{
  int v1; // eax
  int v2; // edi
  unsigned int v3; // eax
  _DWORD *v4; // esi
  _DWORD *v5; // ebx
  int *v6; // eax
  unsigned int v8; // [esp+4h] [ebp-4h]

  v1 = sub_442220(a1, (int)dword_56674C);
  v2 = v1;
  if ( v1 )
  {
    v3 = (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 28))(v1);
    v4 = dword_565FC4;
    v8 = v3;
    v5 = &unk_565FB4;
    if ( dword_565FC4 )
    {
      do
      {
        if ( v3 >= v4[2] )
          break;
        v5 = v4;
        v4 = (_DWORD *)v4[4];
      }
      while ( v4 );
    }
    v6 = (int *)operator new(0x14u);
    *v6 = a1;
    v6[1] = v2;
    v6[2] = v8;
    v6[3] = 0;
    v6[4] = (int)v4;
    v5[4] = v6;
    sub_46D6E0(v2);
  }
  return v2 != 0;
}

// ===== sub_463720 @ 0x00463720..0x0046377F =====
int __thiscall sub_463720(void *this)
{
  void **v1; // ebx
  int result; // eax
  void **v3; // esi
  int v4; // eax

  v1 = (void **)dword_565FC4;
  result = 0;
  v3 = (void **)&unk_565FB4;
  if ( dword_565FC4 )
  {
    while ( this != *v1 )
    {
      v3 = v1;
      v1 = (void **)v1[4];
      if ( !v1 )
        return result;
    }
    v4 = sub_463820();
    if ( v4 )
    {
      if ( v1 == (void **)v4 )
        sub_4637C0(0, 0);
    }
    v3[4] = v1[4];
    sub_46D800();
    operator delete(v1);
    return 1;
  }
  return result;
}

// ===== sub_463780 @ 0x00463780..0x004637B2 =====
_DWORD *sub_463780()
{
  _DWORD *v0; // esi
  int v1; // eax

  v0 = dword_565FC4;
  if ( dword_565FC4 )
  {
    do
    {
      v1 = (*(int (__thiscall **)(_DWORD))(*(_DWORD *)v0[1] + 28))(v0[1]);
      if ( sub_46D830(v1) )
        break;
      v0 = (_DWORD *)v0[4];
    }
    while ( v0 );
  }
  return v0;
}

// ===== sub_4637C0 @ 0x004637C0..0x00463812 =====
int __usercall sub_4637C0@<eax>(_DWORD *a1@<edi>, int a2, int a3)
{
  int result; // eax

  if ( a1 )
  {
    sub_4212A0(a3, a1[1], a2);
    result = sub_496540(4096, *a1);
    dword_50720C = a3;
    dword_565FC8 = (int)a1;
    dword_507208 = a2;
  }
  else
  {
    dword_565FC8 = 0;
  }
  return result;
}

// ===== sub_463820 @ 0x00463820..0x00463826 =====
int sub_463820()
{
  return dword_565FC8;
}

// ===== sub_463830 @ 0x00463830..0x00463836 =====
int __usercall sub_463830@<eax>(int result@<eax>)
{
  dword_507204 = result;
  return result;
}

// ===== sub_463840 @ 0x00463840..0x00463846 =====
int sub_463840()
{
  return dword_507204;
}

// ===== sub_463850 @ 0x00463850..0x0046387C =====
void sub_463850()
{
  void *v0; // esi
  void *v1; // [esp-4h] [ebp-8h]

  v0 = dword_565FD0;
  while ( v0 )
  {
    v1 = v0;
    v0 = (void *)*((_DWORD *)v0 + 1);
    operator delete(v1);
  }
  dword_565FD0 = 0;
}

// ===== sub_463880 @ 0x00463880..0x0046388F =====
int sub_463880()
{
  if ( dword_565FD0 )
    return *(_DWORD *)dword_565FD0;
  else
    return 0;
}

// ===== sub_463890 @ 0x00463890..0x004638E0 =====
int __cdecl sub_463890(int a1)
{
  int v1; // eax
  _DWORD *v2; // ecx
  _DWORD *v3; // edx

  v1 = sub_442220(a1, (int)dword_56674C);
  if ( !v1 )
    return 0;
  v2 = dword_565FD0;
  v3 = &unk_565FCC;
  if ( !dword_565FD0 )
    return 0;
  while ( v1 != *v2 )
  {
    v3 = v2;
    v2 = (_DWORD *)v2[1];
    if ( !v2 )
      return 0;
  }
  v3[1] = v2[1];
  operator delete(v2);
  return 1;
}

// ===== sub_4638E0 @ 0x004638E0..0x0046391C =====
BOOL __cdecl sub_4638E0(int a1)
{
  int v1; // esi
  int *v2; // eax

  v1 = sub_442220(a1, (int)dword_56674C);
  if ( v1 )
  {
    v2 = (int *)operator new(8u);
    *v2 = v1;
    v2[1] = (int)dword_565FD0;
    dword_565FD0 = v2;
  }
  return v1 != 0;
}

// ===== sub_463920 @ 0x00463920..0x00463966 =====
BOOL __cdecl sub_463920(int a1)
{
  _DWORD *v1; // ebx

  v1 = dword_565FD0;
  if ( dword_565FD0 )
  {
    (*(void (__thiscall **)(_DWORD))(**(_DWORD **)dword_565FD0 + 12))(*(_DWORD *)dword_565FD0);
    sub_421520(2 * (a1 != 0) - 1, (_DWORD *)*v1);
    (*(void (__thiscall **)(_DWORD))(*(_DWORD *)*v1 + 12))(*v1);
    sub_461D80();
  }
  return v1 != 0;
}

// ===== sub_463970 @ 0x00463970..0x00463992 =====
int sub_463970()
{
  int *v0; // ecx
  int result; // eax

  v0 = (int *)dword_565FC4;
  result = 0;
  if ( dword_565FC4 )
  {
    do
    {
      if ( v0[3] )
      {
        result = *v0;
        v0[3] = 0;
      }
      v0 = (int *)v0[4];
    }
    while ( v0 );
  }
  return result;
}

// ===== sub_4639A0 @ 0x004639A0..0x004639B1 =====
int sub_4639A0()
{
  int v0; // eax
  int v1; // ecx

  v0 = sub_463820();
  if ( v0 )
    return *(_DWORD *)v0;
  else
    return v1;
}

// ===== sub_4639C0 @ 0x004639C0..0x00463A7E =====
int sub_4639C0()
{
  int result; // eax
  int v1; // edi
  int v2; // ebx
  int v3; // [esp+10h] [ebp-8h] BYREF
  int v4; // [esp+14h] [ebp-4h]

  result = sub_463820();
  v1 = result;
  if ( result )
  {
    if ( (sub_46D560(1) & 0x8000u) == 0 )
    {
      sub_496540(4097, *(_DWORD *)v1);
      return sub_4637C0(0, 0, 0);
    }
    else
    {
      sub_48E680(&v3);
      v2 = v3;
      if ( v3 != dword_507208 || (result = v4, v4 != dword_50720C) )
      {
        (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(v1 + 4) + 12))(*(_DWORD *)(v1 + 4));
        if ( sub_421300(v2, *(_DWORD **)(v1 + 4), v4) )
          (*(void (__thiscall **)(_DWORD))(**(_DWORD **)(v1 + 4) + 12))(*(_DWORD *)(v1 + 4));
        result = sub_461D80();
        dword_507208 = v2;
        dword_50720C = v4;
      }
    }
  }
  return result;
}

// ===== sub_463A80 @ 0x00463A80..0x00463AA7 =====
void *__usercall sub_463A80@<eax>(BOOL a1@<eax>)
{
  dword_565FD4 = a1;
  DragAcceptFiles(hWndParent, a1);
  return memset(szFile, 0, sizeof(szFile));
}

// ===== sub_463AB0 @ 0x00463AB0..0x00463ADC =====
void __usercall sub_463AB0(HDROP a1@<esi>)
{
  if ( DragQueryFileA(a1, 0xFFFFFFFF, 0, 0) )
    DragQueryFileA(a1, 0, szFile, 0x30Cu);
  DragFinish(a1);
}

// ===== sub_463AE0 @ 0x00463AE0..0x00463B23 =====
int __cdecl sub_463AE0(char *a1)
{
  int result; // eax

  result = 0;
  if ( dword_565FD4 )
  {
    if ( strlen(szFile) )
    {
      strcpy(a1, szFile);
      return 1;
    }
  }
  return result;
}

// ===== sub_463B30 @ 0x00463B30..0x00463B5D =====
BOOL __fastcall sub_463B30(const char *a1)
{
  unsigned int v1; // kr00_4
  BOOL result; // eax
  CHAR *v3; // esi
  char v4; // dl

  v1 = strlen(a1);
  result = v1 < 0x30C;
  if ( v1 < 0x30C )
  {
    v3 = (CHAR *)(szFile - a1);
    do
    {
      v4 = *a1;
      a1[(_DWORD)v3] = *a1;
      ++a1;
    }
    while ( v4 );
  }
  return result;
}

// ===== sub_463B60 @ 0x00463B60..0x00463D84 =====
int __cdecl sub_463B60(int a1, int a2)
{
  int v2; // edi
  int i; // esi
  unsigned __int8 v4; // cl
  int v5; // ecx
  int v6; // ebx
  unsigned int *v7; // edx
  int j; // esi
  unsigned int v9; // eax
  unsigned int v10; // ecx
  int v11; // edx
  _DWORD *v12; // esi
  int *v13; // eax
  int v14; // ecx
  int *v15; // edi
  int v16; // ecx
  int v17; // ebx
  int v18; // ecx
  int v19; // esi
  bool v20; // zf
  int v22; // [esp+Ch] [ebp-1830h]
  int *v23; // [esp+10h] [ebp-182Ch]
  int v24; // [esp+1Ch] [ebp-1820h]
  _DWORD *v25; // [esp+20h] [ebp-181Ch]
  int v26; // [esp+24h] [ebp-1818h]
  int v27; // [esp+28h] [ebp-1814h]
  int v28; // [esp+28h] [ebp-1814h]
  int v29; // [esp+2Ch] [ebp-1810h]
  int v30; // [esp+2Ch] [ebp-1810h]
  int v31; // [esp+30h] [ebp-180Ch]
  int v32; // [esp+30h] [ebp-180Ch]
  _DWORD v33[1024]; // [esp+34h] [ebp-1808h] BYREF
  _DWORD v34[513]; // [esp+1034h] [ebp-808h] BYREF

  sub_46BB70();
  sub_4922C0();
  v2 = 0;
  for ( i = 0; i < 512; ++i )
  {
    v4 = *(_BYTE *)(i + a2 + 32) - sub_4922D0();
    if ( v4 )
      v34[v2++] = i + (v4 << 16);
  }
  v5 = 0;
  v24 = v2;
  v34[v2] = 0;
  if ( v2 - 1 > 0 )
  {
    v6 = 1;
    v7 = v34;
    v29 = v2 - 1;
    do
    {
      for ( j = v6; j < v2; ++j )
      {
        v9 = v34[j];
        v10 = *v7;
        if ( *v7 > v9 )
        {
          *v7 = v9;
          v34[j] = v10;
        }
      }
      ++v6;
      ++v7;
      --v29;
    }
    while ( v29 );
    v5 = 0;
  }
  v11 = 1;
  v31 = 1;
  v26 = 0;
  v33[0] = 0;
  v30 = 0;
  if ( v2 > 0 )
  {
    v12 = v34;
    v13 = v33;
    v25 = v34;
    while ( 1 )
    {
      v14 = v5 ^ 0x200;
      v15 = &v33[v14];
      v22 = v14;
      v23 = v15;
      v27 = 0;
      if ( v26 == *((unsigned __int16 *)v12 + 1) )
      {
        do
        {
          ++v27;
          v16 = 2 * *v13;
          *(_DWORD *)(a1 + 8 * v16 + 4) = *v12 & 0x1FF;
          *(_DWORD *)(a1 + 8 * v16) = 0;
          v12 = &v34[++v30];
          ++v13;
        }
        while ( v26 == *((unsigned __int16 *)v12 + 1) );
        v25 = v12;
      }
      v17 = v27;
      v28 = v31 - v27;
      if ( v17 < v31 )
      {
        v32 = v28;
        do
        {
          v18 = *v13;
          *(_DWORD *)(a1 + 16 * *v13) = 1;
          v18 *= 4;
          v19 = v11 + 1;
          *v15 = v11;
          *(_DWORD *)(a1 + 4 * v18 + 8) = v11;
          v15[1] = v11 + 1;
          ++v13;
          v11 += 2;
          v15 += 2;
          v20 = v32-- == 1;
          *(_DWORD *)(a1 + 4 * v18 + 12) = v19;
        }
        while ( !v20 );
        v12 = v25;
      }
      ++v26;
      v31 = 2 * v28;
      if ( v30 >= v24 )
        break;
      v5 = v22;
      v13 = v23;
    }
  }
  return sub_46BB80();
}

// ===== sub_463D90 @ 0x00463D90..0x00463EAB =====
int __cdecl sub_463D90(_BYTE *a1, int a2, int a3)
{
  int v3; // eax
  unsigned __int8 *v4; // edi
  int v5; // edx
  unsigned __int8 v6; // bl
  int v7; // ecx
  unsigned int v8; // ebx
  unsigned int v9; // ecx
  unsigned int v10; // esi
  int v11; // ecx
  _BYTE *v12; // esi
  int v15; // [esp+Ch] [ebp-Ch]
  __int16 v16; // [esp+10h] [ebp-8h]
  int v17; // [esp+14h] [ebp-4h]
  unsigned __int8 *v18; // [esp+24h] [ebp+Ch]

  v3 = a3;
  v4 = (unsigned __int8 *)(a2 + 544);
  v5 = 0;
  v17 = 0;
  v15 = 0;
  if ( *(int *)(a2 + 24) <= 0 )
    return 0;
  v6 = HIBYTE(a2);
  do
  {
    v7 = 0;
    do
    {
      if ( !v5 )
      {
        v6 = *v4++;
        v5 = 8;
      }
      v7 = *(_DWORD *)(v3 + 4 * ((v6 >> 7) + 4 * v7) + 8);
      v6 *= 2;
      --v5;
    }
    while ( *(_DWORD *)(v3 + 16 * v7) );
    v16 = *(_WORD *)(v3 + 16 * v7 + 4);
    v18 = v4;
    if ( HIBYTE(v16) == 1 )
    {
      v8 = v6 >> (8 - v5);
      if ( v5 < 12 )
      {
        v9 = ((unsigned int)(11 - v5) >> 3) + 1;
        v5 += 8 * v9;
        do
        {
          v8 = *v4++ + (v8 << 8);
          --v9;
        }
        while ( v9 );
        v18 = v4;
      }
      v5 -= 12;
      v10 = v8 >> v5;
      v6 = (_BYTE)v8 << (8 - v5);
      v11 = (unsigned __int8)v16 + 2;
      v17 += v11;
      v12 = &a1[-(unsigned __int16)v10 - 2];
      if ( (unsigned __int8)v16 != -2 )
      {
        do
        {
          *a1 = *v12++;
          --v11;
          ++a1;
        }
        while ( v11 );
        v4 = v18;
      }
      v3 = a3;
    }
    else
    {
      *a1 = v16;
      ++v17;
      ++a1;
    }
    ++v15;
  }
  while ( v15 < *(_DWORD *)(a2 + 24) );
  return v17;
}

// ===== sub_463EB0 @ 0x00463EB0..0x00463EF3 =====
int __usercall sub_463EB0@<eax>(_BYTE *a1@<edi>, int a2@<esi>)
{
  int v3; // [esp+0h] [ebp-3FF8h]
  _BYTE v4[16368]; // [esp+4h] [ebp-3FF4h] BYREF

  sub_463B60((int)v4, v3);
  return sub_463D90(a1, a2, (int)v4);
}

// ===== ?__uncaught_exception@@YA_NXZ_1 @ 0x00463F00..0x00463F0C =====
BOOL __cdecl __uncaught_exception()
{
  return dword_565FD8 != 0;
}

// ===== sub_463F10 @ 0x00463F10..0x00463F16 =====
HWND sub_463F10()
{
  return dword_565FD8;
}

// ===== sub_463F20 @ 0x00463F20..0x00464183 =====
int __usercall sub_463F20@<eax>(
        unsigned int a1@<eax>,
        int a2,
        int a3,
        unsigned int a4,
        unsigned int a5,
        WPARAM wParam,
        int a7)
{
  unsigned int v8; // eax
  unsigned int v9; // ecx
  int v10; // edx
  int v11; // ecx
  int v13; // eax
  HWND v14; // eax
  const CHAR *pszFaceName; // [esp+Ch] [ebp-24h]
  int X; // [esp+10h] [ebp-20h] BYREF
  int Y; // [esp+14h] [ebp-1Ch]
  int v18; // [esp+18h] [ebp-18h]
  int v19; // [esp+1Ch] [ebp-14h]
  int v20; // [esp+20h] [ebp-10h] BYREF
  int v21; // [esp+24h] [ebp-Ch]
  int v22; // [esp+28h] [ebp-8h]
  int v23; // [esp+2Ch] [ebp-4h]

  if ( a4 < 8 )
    return 1;
  v8 = sub_4610A0();
  if ( v9 > v8 || a1 < 8 || a1 > sub_4610C0() )
    return 1;
  pszFaceName = (const CHAR *)sub_468BB0(v11, v10);
  if ( !pszFaceName )
    return 2;
  if ( a5 - 8 > 0x38 )
    return 3;
  if ( wParam - 1 > 0xFF )
    return 4;
  sub_464190();
  sub_45FFB0(1);
  dword_565FD8 = CreateWindowExA(
                   0,
                   "EDIT",
                   &WindowName,
                   (a1 / a5 > 1 ? 4 : 128) | 0x40000100,
                   0,
                   0,
                   1,
                   1,
                   hWndParent,
                   0,
                   hInst,
                   0);
  dword_565FDC = SetWindowLongA(dword_565FD8, -4, (LONG)sub_464370);
  sub_464210();
  v20 = a2;
  v22 = a4 + a2;
  v21 = a3;
  v23 = a1 + a3;
  sub_45EB60(&X, &v20);
  MoveWindow(dword_565FD8, X, Y, v18 - X, v19 - Y, 1);
  X = (int)(a5 * dword_506EF4) / 100;
  Y = a5;
  sub_45ECF0(&X, (unsigned int *)&X);
  v13 = sub_42DA70((LPARAM)pszFaceName);
  ho = CreateFontA(Y, (unsigned int)X >> 1, 0, 0, 100, 0, 0, 0, v13 != 0 ? 0x80 : 0, 4u, 0, 2u, 1u, pszFaceName);
  SendMessageA(dword_565FD8, 0x30u, (WPARAM)ho, 1);
  SendMessageA(dword_565FD8, 0xC5u, wParam, 0);
  SetWindowTextA(dword_565FD8, String);
  SendMessageA(dword_565FD8, 0xB1u, 0, -1);
  v14 = dword_565FD8;
  if ( !a7 )
    v14 = hWndParent;
  SetFocus(v14);
  dword_565FE8 = v20;
  dword_565FF0 = v22 - 1;
  dword_565FEC = v21;
  dword_565FF4 = v23 - 1;
  return 0;
}
