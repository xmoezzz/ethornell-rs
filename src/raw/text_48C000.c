#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_48C0D0 @ 0x0048C0D0..0x0048C10B =====
int __cdecl sub_48C0D0(_DWORD *a1)
{
  int v1; // esi
  unsigned int v2; // eax
  int v3; // edx
  int v4; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_461040(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48C110 @ 0x0048C110..0x0048C128 =====
int __cdecl sub_48C110(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_45E880();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C130 @ 0x0048C130..0x0048C153 =====
int __cdecl sub_48C130(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax
  _DWORD *v3; // edx

  v1 = sub_4450B0(a1);
  v2 = sub_45E830(v1);
  sub_4450D0(v3, v2);
  return 0;
}

// ===== sub_48C160 @ 0x0048C160..0x0048C183 =====
int __cdecl sub_48C160(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax
  _DWORD *v3; // edx

  v1 = sub_4450B0(a1);
  v2 = sub_45E860(v1);
  sub_4450D0(v3, v2);
  return 0;
}

// ===== sub_48C190 @ 0x0048C190..0x0048C1B3 =====
int __cdecl sub_48C190(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_45E770(v3, v1);
  return 0;
}

// ===== sub_48C1C0 @ 0x0048C1C0..0x0048C1D4 =====
int __cdecl sub_48C1C0(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_49A250();
  return 0;
}

// ===== sub_48C1E0 @ 0x0048C1E0..0x0048C203 =====
int __cdecl sub_48C1E0(_DWORD *a1)
{
  int v1; // eax
  __int64 v2; // rax

  v1 = sub_4450B0(a1);
  v2 = sub_498800(v1);
  sub_4450D0((_DWORD *)HIDWORD(v2), v2);
  return 0;
}

// ===== sub_48C210 @ 0x0048C210..0x0048C228 =====
int __cdecl sub_48C210(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_49A2D0(v1);
  return 0;
}

// ===== sub_48C230 @ 0x0048C230..0x0048C244 =====
int __cdecl sub_48C230(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_464440(v1);
  return 0;
}

// ===== sub_48C250 @ 0x0048C250..0x0048C273 =====
int __cdecl sub_48C250(_DWORD *a1)
{
  int v1; // eax
  unsigned int v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_464480(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48C280 @ 0x0048C280..0x0048C29D =====
int __cdecl sub_48C280(_DWORD *a1)
{
  unsigned __int16 v1; // ax

  v1 = sub_460590();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C2A0 @ 0x0048C2A0..0x0048C2B8 =====
int __cdecl sub_48C2A0(_DWORD *a1)
{
  BOOL v1; // eax

  v1 = __uncaught_exception();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C2C0 @ 0x0048C2C0..0x0048C2E3 =====
int __cdecl sub_48C2C0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_460550(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48C2F0 @ 0x0048C2F0..0x0048C320 =====
int __cdecl sub_48C2F0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_495C50(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48C320 @ 0x0048C320..0x0048C350 =====
int __cdecl sub_48C320(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  sub_48DF50(a1);
  v1 = sub_495BE0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C350 @ 0x0048C350..0x0048C381 =====
int __cdecl sub_48C350(_DWORD *a1)
{
  unsigned int v1; // esi
  _DWORD *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_46A8B0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48C390 @ 0x0048C390..0x0048C3B5 =====
int __cdecl sub_48C390(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_46A960(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48C3C0 @ 0x0048C3C0..0x0048C41E =====
int __cdecl sub_48C3C0(_DWORD *a1)
{
  void *v1; // ebx
  int *v2; // eax
  unsigned int v3; // eax
  unsigned int v5; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  v1 = (void *)sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = (int *)sub_48DF50(a1);
  v3 = sub_46A9D0(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48C420 @ 0x0048C420..0x0048C452 =====
int __cdecl sub_48C420(_DWORD *a1)
{
  unsigned int v1; // edx
  unsigned int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_46AA30(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48C460 @ 0x0048C460..0x0048C4AE =====
int __cdecl sub_48C460(_DWORD *a1)
{
  unsigned int v1; // esi
  void *v2; // ebx
  void *v3; // eax
  unsigned int v4; // eax
  _DWORD *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v6 = (_DWORD *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_46AA60(v2, v1, v3, v6);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48C4B0 @ 0x0048C4B0..0x0048C4EE =====
int __cdecl sub_48C4B0(_DWORD *a1)
{
  int *v1; // ebx
  void *v2; // eax
  int v3; // eax

  sub_4450B0(a1);
  v1 = (int *)sub_48DF50(a1);
  v2 = (void *)sub_48DF50(a1);
  v3 = sub_46AAA0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48C4F0 @ 0x0048C4F0..0x0048C570 =====
int __cdecl sub_48C4F0(_DWORD *a1)
{
  int v1; // esi
  const CHAR *v2; // ebx
  DWORD *v3; // eax
  int v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-10h]
  const char *v7; // [esp+10h] [ebp-Ch]
  const char *v8; // [esp+14h] [ebp-8h]
  int v9; // [esp+18h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v9 = sub_4450B0(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v8 = (const char *)sub_48DF50(a1);
  v7 = (const char *)sub_48DF50(a1);
  v3 = (DWORD *)sub_48DF50(a1);
  v4 = sub_4724B0(v6, v2, v3, v7, v8, v9, v1, 1, 1, 0);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48C570 @ 0x0048C570..0x0048C5A2 =====
int __cdecl sub_48C570(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // edi
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  sub_401900(v3, v2, v1);
  return 0;
}

// ===== sub_48C5B0 @ 0x0048C5B0..0x0048C5E5 =====
int __cdecl sub_48C5B0(_DWORD *a1)
{
  size_t v1; // edi
  void *v2; // ebx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v3 = sub_48DF50(a1);
  sub_4A1440(v1, v3, v2);
  return 0;
}

// ===== sub_48C5F0 @ 0x0048C5F0..0x0048C617 =====
int __cdecl sub_48C5F0(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  v1 = sub_48F970();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C620 @ 0x0048C620..0x0048C645 =====
int __cdecl sub_48C620(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_48FA00();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48C650 @ 0x0048C650..0x0048C8CC =====
int __cdecl sub_48C650(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // ebx
  int v3; // eax
  int v4; // ebx
  int v5; // ebx
  void *v6; // ebx
  void *v7; // eax
  unsigned int v8; // eax
  int v9; // esi
  const char *v11; // [esp+10h] [ebp-40h]
  const void *v12; // [esp+14h] [ebp-3Ch]
  int v13; // [esp+18h] [ebp-38h]
  char *v14; // [esp+1Ch] [ebp-34h]
  char *v15; // [esp+20h] [ebp-30h]
  char *v16; // [esp+24h] [ebp-2Ch]
  char *v17; // [esp+28h] [ebp-28h]
  char *v18; // [esp+2Ch] [ebp-24h]
  void *v19; // [esp+34h] [ebp-1Ch]
  _DWORD *v20; // [esp+34h] [ebp-1Ch]
  void *v21; // [esp+38h] [ebp-18h]
  void *v22; // [esp+3Ch] [ebp-14h]
  _DWORD *v23; // [esp+40h] [ebp-10h]
  void (__thiscall ***v24)(_DWORD, int); // [esp+40h] [ebp-10h]

  v18 = (char *)sub_48DF50(a1);
  v17 = (char *)sub_48DF50(a1);
  v16 = (char *)sub_48DF50(a1);
  v15 = (char *)sub_48DF50(a1);
  v14 = (char *)sub_48DF50(a1);
  v13 = sub_4450B0(a1);
  sub_48DF50(a1);
  sub_48DF50(a1);
  v12 = (const void *)sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v23 = (_DWORD *)sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v11 = (const char *)sub_48DF50(a1);
  v22 = 0;
  if ( v2 )
  {
    v3 = 0;
    if ( *v2 )
    {
      do
        ++v3;
      while ( v2[v3] );
    }
    v19 = (void *)(v3 + 1);
    v22 = operator new[](4 * (v3 + 1));
    sub_48DF70(v22, a1, v19);
  }
  v4 = 0;
  if ( *v23 )
  {
    do
      ++v4;
    while ( v23[v4] );
  }
  v5 = v4 + 1;
  v20 = operator new[](4 * v5);
  sub_48DF70(v20, a1, v5);
  v6 = operator new[](4 * v1);
  sub_48DF70(v6, a1, v1);
  v21 = operator new[](4 * v1);
  sub_48DF70(v21, a1, v1);
  v7 = operator new(0x10A0u);
  if ( v7 )
    v24 = (void (__thiscall ***)(_DWORD, int))sub_451480(a1, (int)v7);
  else
    v24 = 0;
  v8 = sub_451680(v11, v12, (int)v24, (int)v22, v20, v1, (int)v6, (int)v21, v13, v14, v15, v16, v17, v18);
  if ( v8 > 0x80000001 )
  {
    if ( v8 == -2147483646 )
    {
      v9 = 3;
      goto LABEL_20;
    }
    goto LABEL_18;
  }
  if ( v8 == -2147483647 )
  {
    v9 = 2;
    goto LABEL_20;
  }
  if ( !v8 )
  {
    sub_4451C0((int)a1, (int)v24);
    v9 = 0;
    goto LABEL_23;
  }
  if ( v8 != 0x80000000 )
  {
LABEL_18:
    v9 = -1;
    goto LABEL_20;
  }
  v9 = 1;
LABEL_20:
  if ( v24 )
    (**v24)(v24, 1);
  sub_4450D0(a1, v9);
LABEL_23:
  operator delete[](v21);
  operator delete[](v6);
  operator delete[](v20);
  if ( v22 )
    operator delete[](v22);
  return v9 != 0 ? 0 : 2;
}

// ===== sub_48C8D0 @ 0x0048C8D0..0x0048C920 =====
int __cdecl sub_48C8D0(_DWORD *a1)
{
  int v1; // esi
  const char *v2; // eax
  BOOL v3; // eax
  const char *v5; // [esp+Ch] [ebp-4h]

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v5 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_4713F0(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48C920 @ 0x0048C920..0x0048C982 =====
int __cdecl sub_48C920(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(_DWORD *); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = (int (__cdecl *)(_DWORD *))dword_503F00[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EBEC8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2(a1);
}

// ===== sub_48C990 @ 0x0048C990..0x0048CD68 =====
int sub_48C990()
{
  _DWORD *v0; // eax
  int v1; // eax
  _DWORD *v2; // eax
  int v3; // edx
  _DWORD *v4; // eax
  _DWORD *v5; // ebx
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  void *v11; // ecx
  int v12; // edi
  void *v13; // ecx
  void *v14; // ecx
  int v15; // eax
  int v16; // eax
  int v17; // eax
  int v18; // esi
  int v19; // eax
  int v20; // eax
  int v21; // eax
  _DWORD v23[5]; // [esp+0h] [ebp-648h] BYREF
  int v24; // [esp+14h] [ebp-634h]
  void *v25; // [esp+18h] [ebp-630h]
  char v26[780]; // [esp+1Ch] [ebp-62Ch] BYREF
  _BYTE v27[780]; // [esp+328h] [ebp-320h] BYREF
  _DWORD *v28; // [esp+638h] [ebp-10h]
  int v29; // [esp+644h] [ebp-4h]

  v28 = v23;
  v0 = operator new(8u);
  v25 = v0;
  v29 = 0;
  if ( v0 )
    Concurrency::details::SchedulerBase::CheckStaticConstruction(v0);
  else
    v1 = 0;
  v29 = -1;
  dword_566770 = v1;
  v2 = operator new(0x88u);
  v25 = v2;
  v29 = 1;
  if ( v2 )
    v4 = sub_4447C0(0, v3, v2, 0, 0, 0, 0);
  else
    v4 = 0;
  v29 = -1;
  v5 = v4;
  v25 = v4;
  v6 = 1;
LABEL_8:
  while ( v6 )
  {
    v24 = 0;
    sub_42D280(dword_566770);
    sub_49A0D0();
    sub_461740(0, 0);
    v7 = sub_461D00(1);
    sub_461D20(v7 + 59);
    sub_401B70();
    sub_401DF0();
    sub_493AF0();
    sub_4942E0();
    sub_48DBB0();
    sub_498970();
    sub_463650();
    sub_463850();
    sub_463830(1);
    sub_496270();
    sub_46D640();
    sub_49A030();
    sub_496500();
    sub_46D9D0();
    sub_48EE50(0);
    sub_461030();
    sub_405DB0();
    sub_461E20(0);
    v8 = sub_46B420(1);
    v9 = sub_46D970(v8);
    sub_46D980(v9);
    v10 = sub_46D990(0);
    sub_46BD60(v10);
    sub_46C1A0(v11);
    v12 = 0;
    sub_4954A0();
    sub_496070();
    sub_46A880(v13);
    sub_46C540(v14);
    sub_490B20();
    sub_465240(1);
    sub_465250();
    v15 = sub_4633F0(0);
    sub_463A80(v15);
    v16 = sub_4319B0(1);
    sub_4319E0(v16, 0);
    v29 = 2;
    sub_4650C0((int)v27, v26);
    v17 = sub_48D080(v27, v26, 4096, v5);
    v23[4] = v17 != 0;
    v29 = -1;
    if ( v17 && sub_444B50((int)v5) )
    {
      while ( 1 )
      {
        if ( dword_5668A4 )
        {
          if ( dword_5668A8 <= (unsigned int)sub_498720() )
            sub_48D1E0();
          goto LABEL_22;
        }
        v18 = 0;
        if ( dword_503EFC )
        {
          sub_407B90(dword_5666F8);
          v18 = 1;
        }
        v19 = sub_48CD70(v5, v23[0]) - 1;
        if ( v19 )
        {
          if ( v19 != 1 )
            goto LABEL_18;
          v24 = 1;
        }
        v12 = 1;
LABEL_18:
        if ( v18 )
          sub_407B90(0);
LABEL_22:
        sub_4017F0();
        sub_461C80();
        sub_490B10();
        sub_490B50();
        sub_4920F0();
        sub_492100();
        sub_401830(0);
        v20 = sub_46C490();
        if ( v20 )
        {
          if ( v20 == 1 )
            sub_46C5B0();
        }
        else
        {
          sub_46C570();
        }
        sub_496430();
        sub_48E930();
        sub_48EC80();
        sub_46DF00(1u, (_DWORD *)1);
        v21 = sub_461BA0();
        if ( v21 <= 0 && (!v21 || sub_48D1C0() || !sub_460330()) )
          sub_493AE0();
        sub_460A80();
        if ( sub_49A060() < 0 )
          v12 = 1;
        sub_461450();
        sub_498720();
        sub_49A260();
        sub_4639C0();
        if ( sub_46C490() == 1 )
          sub_46C570();
        if ( v12 )
        {
          if ( !sub_43D1C0() )
          {
            sub_401150();
            sub_444B60(v5);
          }
        }
        if ( !sub_444B50((int)v5) )
        {
          v6 = v24;
          goto LABEL_8;
        }
      }
    }
    v6 = v24;
  }
  if ( v5 )
    (*(void (__thiscall **)(_DWORD *, int))*v5)(v5, 1);
  if ( dword_566770 )
    (**(void (__thiscall ***)(int, int))dword_566770)(dword_566770, 1);
  return 0;
}

// ===== sub_48CD70 @ 0x0048CD70..0x0048D056 =====
int __cdecl sub_48CD70(void *a1)
{
  _DWORD *v1; // esi
  int v2; // eax
  int v3; // eax
  int v4; // eax
  unsigned int i; // edi
  int v6; // ecx
  int (__cdecl *v7)(int); // eax
  _DWORD v9[6]; // [esp+0h] [ebp-148h] BYREF
  int v10; // [esp+18h] [ebp-130h]
  _DWORD *v11; // [esp+1Ch] [ebp-12Ch]
  int v12; // [esp+20h] [ebp-128h]
  int v13; // [esp+24h] [ebp-124h]
  void *v14; // [esp+28h] [ebp-120h]
  _DWORD *v15; // [esp+2Ch] [ebp-11Ch]
  int v16; // [esp+30h] [ebp-118h]
  char Buffer[256]; // [esp+34h] [ebp-114h] BYREF
  _DWORD *v18; // [esp+138h] [ebp-10h]
  int v19; // [esp+144h] [ebp-4h]

  v18 = v9;
  v14 = a1;
  v16 = 0;
  v10 = 0;
  v1 = (_DWORD *)sub_444B50((int)a1);
  v15 = v1;
  v19 = 0;
  while ( v1 )
  {
    v13 = 1;
    v11 = 0;
    if ( dword_566898 && v1 != (_DWORD *)dword_56689C )
      goto LABEL_28;
    v12 = 1;
    v2 = sub_444FA0((int)v1);
    if ( v2 < 0 )
    {
      if ( !sub_445500((int)v1) )
        v11 = v1;
      v12 = 0;
      goto LABEL_28;
    }
    if ( (v2 & 1) != 0 )
    {
      v3 = sub_4451F0((int)v1);
      if ( !v3 )
        goto LABEL_13;
      if ( v3 == -1 )
      {
        v16 = 1;
LABEL_13:
        v12 = 0;
        goto LABEL_28;
      }
    }
    if ( !v16 )
    {
      v4 = 0;
      for ( i = 0; ; ++i )
      {
        v9[5] = i;
        if ( v4 || i >= 0x100000 )
          break;
        v6 = (unsigned __int8)sub_445010(v1);
        v7 = funcs_48CE9E[v6];
        if ( !v7 )
        {
          sprintf(Buffer, &byte_4EBEF8, v6);
          sub_4646F0(Buffer, (int)v1);
        }
        v4 = v7((int)v1);
      }
      switch ( v4 )
      {
        case 2:
          v13 = 0;
          goto LABEL_29;
        case 3:
          v1 = (_DWORD *)sub_444B90(v14, dword_566894);
          v15 = v1;
          v13 = 0;
          goto LABEL_29;
        case 4:
          sub_444FC0((int)v1, 0x80000000);
          v13 = 0;
          goto LABEL_29;
        case 5:
          v10 = 1;
          goto LABEL_27;
        case 6:
          v16 = 1;
          goto LABEL_27;
        default:
LABEL_27:
          if ( v13 )
            break;
          goto LABEL_29;
      }
    }
LABEL_28:
    v1 = (_DWORD *)sub_444B50((int)v1);
    v15 = v1;
LABEL_29:
    if ( v11 )
      sub_444B10((int)v11, (int)v14);
  }
  v19 = -1;
  if ( v16 )
    return 1;
  else
    return v10 != 0 ? 2 : 0;
}

// ===== sub_48D080 @ 0x0048D080..0x0048D167 =====
int __fastcall sub_48D080(size_t a1, size_t a2, const char *a3, char *a4, int a5, _DWORD *a6)
{
  _DWORD *v6; // esi
  _DWORD *v7; // edi
  int v8; // edi
  char Buffer[256]; // [esp+1Ch] [ebp-104h] BYREF

  v6 = operator new[](0x20000u);
  if ( !sub_465AB0(a4, v6, a3) )
  {
    sprintf(Buffer, &byte_4EB960, a3, a4);
LABEL_4:
    sub_4646F0(Buffer, (int)a6);
  }
  v7 = sub_444A70(a6, a5, a1, a2);
  if ( sub_444CE0(v7, v6, a4) == 0x80000000 )
  {
    sprintf(Buffer, &byte_4EBF48, a3, a4);
    goto LABEL_4;
  }
  v8 = sub_42D560((int)v7);
  operator delete[](v6);
  return v8;
}

// ===== sub_48D170 @ 0x0048D170..0x0048D183 =====
int __usercall sub_48D170@<eax>(int result@<eax>)
{
  dword_56689C = result;
  dword_566898 = result != 0;
  return result;
}

// ===== sub_48D190 @ 0x0048D190..0x0048D196 =====
int __usercall sub_48D190@<eax>(int result@<eax>)
{
  dword_5668A0 = result;
  return result;
}

// ===== sub_48D1A0 @ 0x0048D1A0..0x0048D1A6 =====
int sub_48D1A0()
{
  return dword_5668A0;
}

// ===== sub_48D1B0 @ 0x0048D1B0..0x0048D1B6 =====
int __usercall sub_48D1B0@<eax>(int result@<eax>)
{
  dword_503EF8 = result;
  return result;
}

// ===== sub_48D1C0 @ 0x0048D1C0..0x0048D1C6 =====
int sub_48D1C0()
{
  return dword_503EF8;
}

// ===== sub_48D1D0 @ 0x0048D1D0..0x0048D1DB =====
void sub_48D1D0()
{
  dword_5668A4 = 1;
}

// ===== sub_48D1E0 @ 0x0048D1E0..0x0048D206 =====
int __usercall sub_48D1E0@<eax>(int a1@<esi>)
{
  int result; // eax

  result = 0;
  if ( dword_5668A4 )
  {
    if ( a1 )
    {
      result = a1 + sub_498720();
      dword_5668A8 = result;
    }
    else
    {
      dword_5668A4 = 0;
      dword_5668A8 = 0;
    }
  }
  return result;
}

// ===== sub_48D210 @ 0x0048D210..0x0048D216 =====
int __usercall sub_48D210@<eax>(int result@<eax>)
{
  dword_503EFC = result;
  return result;
}

// ===== sub_48D220 @ 0x0048D220..0x0048D242 =====
int sub_48D220()
{
  int v0; // edi
  int v1; // esi

  v0 = rand() << 10;
  v1 = (v0 ^ rand()) << 10;
  return v1 ^ rand();
}

// ===== sub_48D250 @ 0x0048D250..0x0048D260 =====
int __usercall sub_48D250@<eax>(char *Buffer@<ecx>, int a2@<eax>)
{
  return sprintf(Buffer, "FMO%.8xForBGI", a2);
}

// ===== sub_48D260 @ 0x0048D260..0x0048D366 =====
int __usercall sub_48D260@<eax>(const void *a1@<ecx>, DWORD a2@<esi>, HWND hWnd)
{
  HANDLE FileMappingA; // eax
  void *v4; // ebx
  void *v5; // edi
  int wParam; // [esp+8h] [ebp-314h]
  CHAR Name[780]; // [esp+Ch] [ebp-310h] BYREF

  if ( !IsWindow(hWnd) )
    return -2147483647;
  if ( !a2 )
    return -2147483646;
  wParam = sub_48D220();
  sub_48D250(Name, wParam);
  FileMappingA = CreateFileMappingA((HANDLE)0xFFFFFFFF, 0, 4u, 0, a2, Name);
  v4 = FileMappingA;
  if ( !FileMappingA )
    return -2147483645;
  v5 = MapViewOfFile(FileMappingA, 0xF001Fu, 0, 0, 0);
  memcpy_0(v5, a1, a2);
  UnmapViewOfFile(v5);
  SendMessageA(hWnd, 0x9000u, wParam, a2);
  CloseHandle(v4);
  return 0;
}

// ===== sub_48D370 @ 0x0048D370..0x0048D43B =====
int __cdecl sub_48D370(void *a1, int a2)
{
  size_t Size; // ecx
  DWORD v3; // esi
  HANDLE FileMappingA; // eax
  void *v5; // ebx
  const void *v6; // eax
  const void *v7; // edi
  CHAR Name[780]; // [esp+Ch] [ebp-310h] BYREF

  v3 = Size;
  sub_48D250(Name, a2);
  FileMappingA = CreateFileMappingA((HANDLE)0xFFFFFFFF, 0, 4u, 0, v3, Name);
  v5 = FileMappingA;
  if ( !FileMappingA )
    return -2147483644;
  v6 = MapViewOfFile(FileMappingA, 0xF001Fu, 0, 0, 0);
  v7 = v6;
  if ( v6 )
  {
    memcpy_0(a1, v6, v3);
    UnmapViewOfFile(v7);
    CloseHandle(v5);
    return 0;
  }
  else
  {
    CloseHandle(v5);
    return -2147483644;
  }
}

// ===== _WinMain@16 @ 0x0048D440..0x0048D63F =====
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  HWND WindowA; // eax
  HWND v5; // esi
  HWND DefaultIMEWnd; // esi
  HIMC hInstancea; // [esp+Ch] [ebp+8h]

  hInst = hInstance;
  sub_4316F0(lpCmdLine);
  if ( dword_5666E0
    || (hMutex = CreateMutexA(0, 1, "Buriko General Interpreter for Tayutama2TV is executing."), GetLastError() != 183) )
  {
    if ( !sub_46F2B0(0, 0xFFFFFFFF) )
    {
      MessageBoxA(0, &Text, "Error!!", 0x1010u);
      return 0;
    }
    if ( sub_46E970() && sub_46E990() )
    {
      sub_46F910();
      if ( CoInitialize(0) >= 0 && sub_46F710() )
      {
        sub_464A80();
        sub_48EBA0();
        sub_472490((const char *)&dword_4EBD9C);
        if ( sub_46D480() )
        {
          DefaultIMEWnd = ImmGetDefaultIMEWnd(hWndParent);
          SendMessageA(DefaultIMEWnd, 0x283u, 0x21u, 0);
          hInstancea = ImmAssociateContext(hWndParent, 0);
          sub_48C990();
          ImmAssociateContext(hWndParent, hInstancea);
          SendMessageA(DefaultIMEWnd, 0x283u, 0x22u, 0);
        }
        sub_492450();
        CoUninitialize();
        if ( !dword_5666E0 )
        {
          ReleaseMutex(hMutex);
          CloseHandle(hMutex);
        }
        if ( dword_566778 )
        {
          sub_4724B0(
            0,
            (const CHAR *)dword_56677C,
            0,
            (const char *)dword_566774,
            (const char *)dword_566778,
            1,
            0,
            0,
            0,
            0);
          operator delete[](dword_566778);
          if ( dword_566774 )
            operator delete[](dword_566774);
          if ( dword_56677C )
          {
            operator delete[](dword_56677C);
            return 0;
          }
        }
      }
    }
    else
    {
      MessageBoxA(0, &byte_4EC038, "Error!!", 0x1010u);
    }
  }
  else
  {
    WindowA = FindWindowA(aBgiMainWindow, 0);
    v5 = WindowA;
    if ( WindowA )
    {
      BringWindowToTop(WindowA);
      SetForegroundWindow(v5);
      sub_431820(v5);
      return 0;
    }
  }
  return 0;
}

// ===== sub_48D640 @ 0x0048D640..0x0048D7BA =====
int sub_48D640()
{
  int result; // eax
  int v1; // edi
  char v2; // cl
  int v3; // eax
  CHAR v4; // cl
  DWORD_PTR dwParam2; // [esp+4h] [ebp-330h] BYREF
  MCIDEVICEID mciId; // [esp+8h] [ebp-32Ch]
  const char *v7; // [esp+Ch] [ebp-328h]
  _BYTE *v8; // [esp+10h] [ebp-324h]
  int v9; // [esp+14h] [ebp-320h]
  DWORD_PTR v10[3]; // [esp+18h] [ebp-31Ch] BYREF
  _BYTE v11[780]; // [esp+24h] [ebp-310h] BYREF

  result = ::mciId != 0;
  v1 = result;
  if ( !::mciId )
  {
    do
    {
      v2 = *(&Buffer + result);
      v11[result++] = v2;
    }
    while ( v2 );
    sub_42EA80(0, v11);
    v3 = dword_5180A4[v11[0]] == 5;
    if ( dword_5180A4[v11[0]] != 5 && byte_517C08[0] != (_BYTE)v3 )
    {
      do
      {
        v4 = byte_517C08[v3];
        v11[v3++] = v4;
      }
      while ( v4 );
      sub_42EA80(0, v11);
      v3 = dword_5180A4[v11[0]] == 5;
    }
    dwParam2 = 0;
    mciId = 0;
    v9 = 0;
    v11[2] = 0;
    v7 = "cdaudio";
    v8 = v11;
    if ( !mciSendCommandA(0, 0x803u, (v3 != 0 ? 0x200 : 0) | 0x2000, (DWORD_PTR)&dwParam2) )
    {
      v10[0] = 0;
      v10[2] = 0;
      v10[1] = 10;
      if ( !mciSendCommandA(mciId, 0x80Du, 0x400u, (DWORD_PTR)v10) )
      {
        ::mciId = mciId;
        return 1;
      }
      sub_48D7C0();
    }
    return v1;
  }
  return result;
}

// ===== sub_48D7C0 @ 0x0048D7C0..0x0048D7EA =====
int sub_48D7C0()
{
  int result; // eax

  result = 0;
  if ( mciId )
  {
    mciSendCommandA(mciId, 0x804u, 0, 0);
    mciId = 0;
    return 1;
  }
  return result;
}

// ===== sub_48D7F0 @ 0x0048D7F0..0x0048D84F =====
int sub_48D7F0()
{
  bool v0; // zf
  int result; // eax
  DWORD_PTR dwParam2; // [esp+4h] [ebp-14h] BYREF
  int v3; // [esp+8h] [ebp-10h]
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]

  if ( !mciId )
    return 0;
  dwParam2 = 0;
  v3 = 0;
  v5 = 0;
  v4 = 7;
  v0 = mciSendCommandA(mciId, 0x814u, 0x100u, (DWORD_PTR)&dwParam2) == 0;
  result = v3;
  if ( !v0 )
    return 0;
  return result;
}

// ===== sub_48D850 @ 0x0048D850..0x0048D8CA =====
int __cdecl sub_48D850(int a1, int a2)
{
  int result; // eax
  DWORD_PTR dwParam2[3]; // [esp+8h] [ebp-10h] BYREF

  result = sub_48D7F0();
  if ( result )
  {
    dwParam2[2] = (unsigned __int8)(a1 + 1);
    dwParam2[1] = (unsigned __int8)a1;
    dwParam2[0] = (DWORD_PTR)hWndParent;
    result = mciSendCommandA(mciId, 0x806u, (a2 != 0) | 0xC, (DWORD_PTR)dwParam2) == 0;
    dword_5668B0 = a1;
  }
  return result;
}

// ===== sub_48D8D0 @ 0x0048D8D0..0x0048D8E3 =====
int sub_48D8D0()
{
  return sub_48D850(dword_5668B0, 1);
}

// ===== sub_48D8F0 @ 0x0048D8F0..0x0048D910 =====
int sub_48D8F0()
{
  int result; // eax

  result = 0;
  if ( mciId )
  {
    mciSendCommandA(mciId, 0x808u, 0, 0);
    return 1;
  }
  return result;
}

// ===== sub_48D910 @ 0x0048D910..0x0048DA58 =====
int __usercall sub_48D910@<eax>(_DWORD *a1@<esi>)
{
  int result; // eax
  DWORD_PTR dwParam2; // [esp+4h] [ebp-14h] BYREF
  int v3; // [esp+8h] [ebp-10h]
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]

  if ( !mciId )
    return 0;
  dwParam2 = 0;
  v3 = 0;
  v5 = 0;
  v4 = 4;
  if ( mciSendCommandA(mciId, 0x814u, 0x100u, (DWORD_PTR)&dwParam2) )
    return 0;
  switch ( v3 )
  {
    case 524:
      *a1 = 0;
      result = 1;
      break;
    case 525:
      *a1 = 3;
      result = 1;
      break;
    case 526:
      *a1 = 2;
      result = 1;
      break;
    case 527:
      *a1 = 6;
      result = 1;
      break;
    case 528:
      *a1 = 1;
      result = 1;
      break;
    case 529:
      *a1 = 4;
      result = 1;
      break;
    case 530:
      *a1 = 5;
      result = 1;
      break;
    default:
      *a1 = -1;
      result = 1;
      break;
  }
  return result;
}

// ===== sub_48DA80 @ 0x0048DA80..0x0048DBA3 =====
int sub_48DA80()
{
  int result; // eax

  result = 0;
  memset(&unk_51A998, 0, 0x40000u);
  memset(&unk_55AE18, 0, 0x4000u);
  memset(&unk_55A998, 0, 0x400u);
  dword_51A958 = 0;
  dword_51A95C = 0;
  dword_51A960 = 0;
  dword_51A964 = 0;
  dword_51A968 = 0;
  dword_51A96C = 0;
  dword_51A970 = 0;
  dword_51A974 = 0;
  dword_51A978 = 0;
  dword_51A97C = 0;
  dword_51A980 = 0;
  dword_51A984 = 0;
  dword_51A988 = 0;
  dword_51A98C = 0;
  dword_51A990 = 0;
  dword_51A994 = 0;
  dword_55AD98 = 0;
  dword_55AD9C = 0;
  dword_55ADA0 = 0;
  dword_55ADA4 = 0;
  dword_55ADA8 = 0;
  dword_55ADAC = 0;
  dword_55ADB0 = 0;
  dword_55ADB4 = 0;
  dword_55ADB8 = 0;
  dword_55ADBC = 0;
  dword_55ADC0 = 0;
  dword_55ADC4 = 0;
  dword_55ADC8 = 0;
  dword_55ADCC = 0;
  dword_55ADD0 = 0;
  dword_55ADD4 = 0;
  dword_55ADD8 = 0;
  dword_55ADDC = 0;
  dword_55ADE0 = 0;
  dword_55ADE4 = 0;
  dword_55ADE8 = 0;
  dword_55ADEC = 0;
  dword_55ADF0 = 0;
  dword_55ADF4 = 0;
  dword_55ADF8 = 0;
  dword_55ADFC = 0;
  dword_55AE00 = 0;
  dword_55AE04 = 0;
  dword_55AE08 = 0;
  dword_55AE0C = 0;
  dword_55AE10 = 0;
  dword_55AE14 = 0;
  dword_5668B4 = 1;
  return result;
}

// ===== sub_48DBB0 @ 0x0048DBB0..0x0048DC65 =====
void sub_48DBB0()
{
  void **v0; // esi
  void **v1; // esi
  void **v2; // esi
  void **v3; // esi
  void **v4; // esi

  if ( dword_5668B4 )
  {
    v0 = (void **)&unk_51A998;
    do
    {
      operator delete[](*v0);
      *v0++ = 0;
    }
    while ( (int)v0 < (int)&unk_55A998 );
    v1 = (void **)&unk_55AE18;
    do
    {
      operator delete[](*v1);
      *v1++ = 0;
    }
    while ( (int)v1 < (int)&dword_55EE18 );
    v2 = (void **)&unk_55A998;
    do
    {
      operator delete[](*v2);
      *v2++ = 0;
    }
    while ( (int)v2 < (int)&dword_55AD98 );
    v3 = (void **)&dword_51A958;
    do
    {
      operator delete[](*v3);
      *v3++ = 0;
    }
    while ( (int)v3 < (int)&unk_51A998 );
    v4 = (void **)&dword_55AD98;
    do
    {
      operator delete[](*v4);
      *v4++ = 0;
    }
    while ( (int)v4 < (int)&unk_55AE18 );
  }
}

// ===== sub_48DC70 @ 0x0048DC70..0x0048DD02 =====
int __cdecl sub_48DC70(unsigned int a1)
{
  int result; // eax
  unsigned int v2; // ebx
  int v3; // edi
  int v4; // edx
  int v5; // esi
  _DWORD *v6; // ecx

  result = 0;
  v2 = a1;
  if ( off_503E78 )
  {
    v3 = 0;
    do
    {
      if ( result )
        break;
      if ( v2 <= dword_503ECC[v3] )
      {
        v4 = dword_503E90[v3];
        v5 = 0;
        if ( v4 > 0 )
        {
          v6 = *(_UNKNOWN **)((char *)&off_503E78 + v3 * 4);
          while ( *v6 )
          {
            ++v5;
            ++v6;
            if ( v5 >= v4 )
              goto LABEL_11;
          }
          (*(_DWORD **)((char *)&off_503E78 + v3 * 4))[v5] = operator new[](v2);
          result = ((v5 & ((1 << (26 - dword_503EA4[v3])) - 1)) << dword_503EA4[v3]) | ((dword_503EE0[v3]
                                                                                       + (v5 >> (26 - dword_503EA4[v3]))) << 26);
          v2 = a1;
        }
      }
LABEL_11:
      ++v3;
    }
    while ( *(_UNKNOWN **)((char *)&off_503E78 + v3 * 4) );
  }
  return result;
}

// ===== sub_48DD10 @ 0x0048DD10..0x0048DDCA =====
int __usercall sub_48DD10@<eax>(unsigned int a1@<eax>)
{
  unsigned int v1; // esi
  int v2; // ebx
  int v3; // edx
  unsigned int v4; // ecx

  v1 = a1 >> 26;
  v2 = 0;
  if ( !off_503E78 )
    return 0;
  v3 = 0;
  while ( 1 )
  {
    v4 = dword_503EE0[v3];
    if ( v1 >= v4 && v1 < dword_503EE4[v3] && (a1 & dword_503EB8[v3]) == 0 )
    {
      v1 = ((a1 & 0x3FFFFFF) >> dword_503EA4[v3]) | ((v1 - v4) << (26 - dword_503EA4[v3]));
      if ( (*(_DWORD **)((char *)&off_503E78 + v3 * 4))[v1] )
        break;
    }
    v3 = ++v2;
    if ( !*(&off_503E78 + v2) )
      return 0;
  }
  operator delete[](*((void **)*(&off_503E78 + v2) + v1));
  *((_DWORD *)*(&off_503E78 + v2) + v1) = 0;
  return 1;
}

// ===== sub_48DDD0 @ 0x0048DDD0..0x0048DF31 =====
char *__usercall sub_48DDD0@<eax>(unsigned int a1@<edx>, int a2)
{
  char *result; // eax
  unsigned int v3; // eax
  int v4; // eax
  int v5; // edx
  int v6; // eax
  int v7; // edx
  int v8; // esi
  int v9; // ecx
  int v10; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  result = 0;
  if ( a1 )
  {
    v3 = a1 >> 26;
    switch ( a1 >> 26 )
    {
      case 0u:
        return (char *)dword_566758 + (a1 & 0x3FFFFFF);
      case 1u:
        v4 = sub_444C60(a2);
        return (char *)((v5 & 0x3FFFFFF) + v4);
      case 2u:
        v6 = sub_444C70(a2);
        return (char *)((v7 & 0x3FFFFFF) + v6);
      case 3u:
        return (char *)(*(int (__thiscall **)(int, unsigned int))(*(_DWORD *)a2 + 12))(a2, a1 & 0x3FFFFFF);
      default:
        v8 = 0;
        if ( !off_503E78 )
          goto LABEL_15;
        v9 = 0;
        break;
    }
    while ( v3 < dword_503EE0[v9] || v3 >= dword_503EE4[v9] )
    {
      v9 = ++v8;
      if ( !*(&off_503E78 + v8) )
        goto LABEL_15;
    }
    v10 = *((_DWORD *)*(&off_503E78 + v8)
          + (((a1 & 0x3FFFFFF) >> dword_503EA4[v8]) | ((v3 - dword_503EE0[v8]) << (26 - dword_503EA4[v8]))));
    if ( !v10 || (result = (char *)((a1 & dword_503EB8[v8]) + v10)) == 0 )
    {
LABEL_15:
      sprintf(Buffer, &byte_4EC09C, a1);
      sub_4646F0(Buffer, a2);
    }
  }
  return result;
}

// ===== sub_48DF50 @ 0x0048DF50..0x0048DF61 =====
char *__thiscall sub_48DF50(_DWORD *this)
{
  unsigned int v1; // eax

  v1 = sub_4450B0(this);
  return sub_48DDD0(v1, (int)this);
}

// ===== sub_48DF70 @ 0x0048DF70..0x0048DFA2 =====
char *__usercall sub_48DF70@<eax>(char *result@<eax>, int a2, int a3, int a4)
{
  int v4; // esi
  unsigned int *i; // edi

  v4 = 0;
  for ( i = (unsigned int *)result; v4 < a4; ++i )
  {
    result = sub_48DDD0(*i, a3);
    *(_DWORD *)(a2 + 4 * v4++) = result;
  }
  return result;
}

// ===== sub_48DFB0 @ 0x0048DFB0..0x0048DFDF =====
void sub_48DFB0()
{
  void *v0; // esi
  void *v1; // [esp-4h] [ebp-8h]

  v0 = dword_5668B8;
  while ( v0 )
  {
    v1 = v0;
    v0 = (void *)*((_DWORD *)v0 + 67);
    operator delete(v1);
  }
  dword_5668B8 = 0;
}

// ===== sub_48DFE0 @ 0x0048DFE0..0x0048E03D =====
BOOL __usercall sub_48DFE0@<eax>(const char *a1@<edi>, int a2, int a3, int a4)
{
  int v4; // eax
  void *v5; // ecx

  if ( a4 )
  {
    operator new(0x110u);
    v4 = sub_42D560(a2);
    *(_DWORD *)v5 = v4;
    *((_DWORD *)v5 + 1) = a3;
    *((_DWORD *)v5 + 2) = a4;
    strcpy((char *)v5 + 12, a1);
    *((_DWORD *)v5 + 67) = dword_5668B8;
    dword_5668B8 = v5;
  }
  return a4 != 0;
}

// ===== sub_48E040 @ 0x0048E040..0x0048E135 =====
void __usercall sub_48E040(int a1@<edx>, unsigned int a2, int a3)
{
  _DWORD *v3; // esi
  unsigned int v4; // eax
  char *v5; // edi
  char Buffer[1024]; // [esp+Ch] [ebp-404h] BYREF

  v3 = dword_5668B8;
  if ( dword_5668B8 )
  {
    while ( 1 )
    {
      v4 = v3[1];
      if ( v4 <= a2 + a3 - 1 && a2 < v4 + v3[2] )
        break;
      v3 = (_DWORD *)v3[67];
      if ( !v3 )
        return;
    }
    if ( (a2 >> 26) - 1 > 2 || *v3 == sub_42D560(a1) )
    {
      v5 = (char *)operator new[](0x10000u);
      sprintf(Buffer, aS_7, &unk_4EC15C, v3 + 3, v3[1], v3[2], a2, a3);
      sub_464570(a1, v5, Buffer);
      sub_46BC80(byte_4EC138, hWndParent, v5, 0x1040u);
      operator delete[](v5);
    }
  }
}

// ===== sub_48E140 @ 0x0048E140..0x0048E18E =====
char *__usercall sub_48E140@<eax>(int a1@<edi>, unsigned int a2@<esi>, int a3)
{
  _DWORD v4[3]; // [esp+0h] [ebp-10h]

  v4[0] = 1;
  v4[1] = 2;
  v4[2] = 4;
  sub_48E040(a1, a2, v4[a3]);
  return sub_48DDD0(a2, a1);
}

// ===== sub_48E190 @ 0x0048E190..0x0048E1DB =====
int __cdecl sub_48E190(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // esi
  char *v3; // eax
  int v5; // [esp-4h] [ebp-10h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = (unsigned __int8)sub_445030(a1);
  v3 = sub_48E140((int)a1, v2, v5);
  sub_4736F0(v1, v3, v5);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48E1E0 @ 0x0048E1E0..0x0048E222 =====
int __cdecl sub_48E1E0(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // ebx
  char *v3; // eax
  int v5; // [esp-4h] [ebp-10h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v5 = (unsigned __int8)sub_445030(a1);
  v3 = sub_48E140((int)a1, v1, v5);
  sub_4736F0(v2, v3, v5);
  return 0;
}

// ===== sub_48E230 @ 0x0048E230..0x0048E307 =====
int __cdecl sub_48E230(_DWORD *a1)
{
  _DWORD *v1; // ecx
  unsigned __int8 v2; // al
  int *v3; // ebx
  int v4; // esi
  unsigned int v5; // esi
  int v6; // edx
  int v7; // eax
  char *v8; // eax
  int v10; // [esp-4h] [ebp-42Ch]
  int v11; // [esp+Ch] [ebp-41Ch]
  int v12; // [esp+10h] [ebp-418h]
  int v13; // [esp+14h] [ebp-414h]
  char v14; // [esp+18h] [ebp-410h] BYREF
  _DWORD v15[3]; // [esp+418h] [ebp-10h]

  v12 = (unsigned __int8)sub_445030(a1);
  v2 = sub_445030(v1);
  v3 = (int *)&v14;
  v4 = v2;
  if ( v2 )
  {
    do
    {
      *v3++ = sub_4450B0(a1);
      --v4;
    }
    while ( v4 );
  }
  v5 = sub_4450B0(a1);
  v15[0] = 1;
  v15[1] = 2;
  v15[2] = 4;
  v13 = v6;
  if ( v6 )
  {
    v7 = v12;
    v11 = v15[v12];
    while ( 1 )
    {
      --v13;
      v10 = v7;
      --v3;
      v8 = sub_48E140((int)a1, v5, v7);
      sub_4736F0(*v3, v8, v10);
      v5 += v11;
      if ( !v13 )
        break;
      v7 = v12;
    }
  }
  return 0;
}

// ===== sub_48E310 @ 0x0048E310..0x0048E325 =====
char *__usercall sub_48E310@<eax>(unsigned int a1@<edi>, int a2@<esi>)
{
  unsigned int v3; // [esp+0h] [ebp-8h]
  int v4; // [esp+4h] [ebp-4h]

  sub_48E040(a2, v3, v4);
  return sub_48DDD0(a1, a2);
}

// ===== sub_48E330 @ 0x0048E330..0x0048E374 =====
int __cdecl sub_48E330(_DWORD *a1)
{
  size_t v1; // ebx
  unsigned int v2; // eax
  char *v3; // eax
  char *Src; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  Src = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_48E310(v2, (int)a1);
  memcpy_0(v3, Src, v1);
  return 0;
}

// ===== sub_48E380 @ 0x0048E380..0x0048E3B3 =====
int __cdecl sub_48E380(_DWORD *a1)
{
  unsigned int v1; // eax
  size_t v2; // edx
  char *v3; // eax
  size_t v5; // [esp-4h] [ebp-Ch]

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v5 = v2;
  v3 = sub_48E310(v1, (int)a1);
  memset(v3, 0, v5);
  return 0;
}

// ===== sub_48E3C0 @ 0x0048E3C0..0x0048E3FD =====
int __cdecl sub_48E3C0(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // eax
  size_t v3; // edx
  char *v4; // eax
  size_t v6; // [esp-4h] [ebp-10h]

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = v3;
  v4 = sub_48E310(v2, (int)a1);
  memset(v4, v1, v6);
  return 0;
}

// ===== sub_48E400 @ 0x0048E400..0x0048E442 =====
int __cdecl sub_48E400(_DWORD *a1)
{
  char *v1; // ebx
  unsigned int v2; // edi
  char *v3; // eax
  char v4; // cl

  v1 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  strlen(v1);
  v3 = (char *)(sub_48E310(v2, (int)a1) - v1);
  do
  {
    v4 = *v1;
    v1[(_DWORD)v3] = *v1;
    ++v1;
  }
  while ( v4 );
  return 0;
}

// ===== sub_48E450 @ 0x0048E450..0x0048E4AF =====
int __cdecl sub_48E450(_DWORD *a1)
{
  unsigned int v1; // ebx
  char *v2; // edi
  char *v4; // [esp+Ch] [ebp-4h]

  v4 = sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_48DDD0(v1, (int)a1);
  sub_474720(a1, v4, v2);
  sub_48E040((int)a1, v1, strlen(v2) + 1);
  return 0;
}

// ===== sub_48E4B0 @ 0x0048E4B0..0x0048E55D =====
void __cdecl sub_48E4B0(int a1)
{
  if ( a1 )
  {
    off_506324[0] = (int (__cdecl *)(int))sub_48E190;
    off_506328[0] = (int (__cdecl *)(int))sub_48E1E0;
    off_506330 = (int (__cdecl *)(int))sub_48E230;
    off_506480[0] = (int (__cdecl *)(int))sub_48E330;
    off_506484[0] = (int (__cdecl *)(int))sub_48E380;
    off_506488[0] = (int (__cdecl *)(int))sub_48E3C0;
    off_5064A8[0] = (int (__cdecl *)(int))sub_48E400;
    off_5064BC[0] = (int (__cdecl *)(int))sub_48E450;
  }
  else
  {
    off_506324[0] = (int (__cdecl *)(int))sub_473710;
    off_506328[0] = (int (__cdecl *)(int))sub_473750;
    off_506330 = (int (__cdecl *)(int))sub_4737C0;
    off_506480[0] = (int (__cdecl *)(int))sub_474260;
    off_506484[0] = (int (__cdecl *)(int))sub_4742A0;
    off_506488[0] = (int (__cdecl *)(int))sub_4742D0;
    off_5064A8[0] = sub_474600;
    off_5064BC[0] = (int (__cdecl *)(int))sub_474E40;
  }
}

// ===== sub_48E560 @ 0x0048E560..0x0048E58A =====
int __fastcall sub_48E560(unsigned int a1, int a2, int a3)
{
  int result; // eax

  result = 0;
  if ( a1 < 5 )
  {
    sub_45E8D0(&dword_503E40[2 * a1], a2, a3, 1);
    return 1;
  }
  return result;
}

// ===== sub_48E590 @ 0x0048E590..0x0048E5B0 =====
int __fastcall sub_48E590(unsigned int a1, _DWORD *a2)
{
  int result; // eax

  result = 0;
  if ( a1 < 5 )
  {
    *a2 = dword_503E40[2 * a1];
    a2[1] = dword_503E44[2 * a1];
    return 1;
  }
  return result;
}

// ===== sub_48E5B0 @ 0x0048E5B0..0x0048E63C =====
void __usercall sub_48E5B0(_DWORD *a1@<edi>, _DWORD *a2@<esi>)
{
  int v2; // eax
  int SystemMetrics; // [esp+8h] [ebp-18h]
  struct tagRECT Rect; // [esp+Ch] [ebp-14h] BYREF

  if ( dword_5666F0 )
  {
    GetWindowRect(hWndParent, &Rect);
    if ( sub_45F640() )
    {
      v2 = Rect.top + a1[1];
      *a2 = Rect.left + *a1;
      a2[1] = v2;
    }
    else
    {
      *a2 = Rect.left + *a1 + GetSystemMetrics(7);
      SystemMetrics = GetSystemMetrics(8);
      a2[1] = Rect.top + a1[1] + GetSystemMetrics(4) + SystemMetrics;
    }
  }
}

// ===== sub_48E640 @ 0x0048E640..0x0048E680 =====
BOOL __usercall sub_48E640@<eax>(BOOL result@<eax>, int a2@<ecx>)
{
  int v2[2]; // [esp+8h] [ebp-10h] BYREF
  int X[2]; // [esp+10h] [ebp-8h] BYREF

  if ( dword_5666F0 )
  {
    sub_45E8D0(v2, a2, result, 0);
    sub_48E5B0(v2, X);
    return SetCursorPos(X[0], X[1]);
  }
  return result;
}

// ===== sub_48E680 @ 0x0048E680..0x0048E773 =====
void __cdecl sub_48E680(int *a1)
{
  int v1; // ecx
  int v2; // esi
  int SystemMetrics; // eax
  struct tagPOINT Point; // [esp+Ch] [ebp-1Ch] BYREF
  struct tagRECT Rect; // [esp+14h] [ebp-14h] BYREF

  if ( dword_5666F0 )
  {
    if ( !sub_46B190(v1, a1) )
    {
      GetWindowRect(hWndParent, &Rect);
      GetCursorPos(&Point);
      if ( sub_45F640() )
      {
        sub_45E8D0(a1, Point.x - Rect.left, Point.y - Rect.top, 1);
      }
      else
      {
        Point.x -= Rect.left + GetSystemMetrics(7);
        v2 = Point.y - Rect.top - GetSystemMetrics(4);
        SystemMetrics = GetSystemMetrics(8);
        sub_45E8D0(a1, Point.x, v2 - SystemMetrics, 1);
      }
    }
  }
  else
  {
    *a1 = 0;
    a1[1] = 0;
  }
}

// ===== sub_48E780 @ 0x0048E780..0x0048E845 =====
BOOL __usercall sub_48E780@<eax>(unsigned int a1@<edi>, int a2, int a3, int a4, int a5, int a6)
{
  BOOL v6; // eax
  BOOL v7; // esi

  v6 = IsIconic(hWndParent);
  v7 = !v6;
  if ( !v6 )
  {
    sub_48E680(&dword_5668C0);
    dword_5668C8 = a2 - dword_5668C0;
    dword_5668D4 = dword_5668C4;
    dword_5668D0 = dword_5668C0;
    dword_5668CC = a3 - dword_5668C4;
    dword_5668D8 = a4;
    dword_5668DC = 0;
    dword_5668E4 = a1;
    dword_5668E0 = (a5 * a1 / 0x3E8 == 0) + a5 * a1 / 0x3E8;
    dword_5668E8 = sub_498720();
    dword_5668BC = 1;
    dword_5668EC = dword_5668E8 + a1 / dword_5668E0;
    dword_5668F0 = a6;
  }
  return v7;
}

// ===== sub_48E850 @ 0x0048E850..0x0048E8C2 =====
void __usercall sub_48E850(int a1@<esi>)
{
  if ( dword_5668F4 )
  {
    if ( a1 )
    {
      dword_5668FC = a1;
      dword_566900 = a1 + sub_498720();
    }
    else
    {
      dword_5668F4 = 0;
      if ( !dword_5668F8 )
        sub_48ED90();
    }
  }
  else if ( a1 )
  {
    dword_5668F4 = 1;
    dword_5668F8 = 1;
    dword_5668FC = a1;
    dword_566900 = a1 + sub_498720();
    dword_566904 = 0x80000000;
    dword_566908 = 0x80000000;
  }
}

// ===== sub_48E8D0 @ 0x0048E8D0..0x0048E92B =====
int __usercall sub_48E8D0@<eax>(int a1@<eax>, int a2@<ecx>, unsigned int a3@<esi>, int a4)
{
  if ( a2 == 1 )
    return (a4 * (int)((cos((double)(int)(46080 - 46080 * a1 / a3) * 3.141592653589793 / 46080.0) + 1.0) * 32768.0)) >> 16;
  else
    return (int)(a4 * ((a1 << 16) / a3)) >> 16;
}

// ===== sub_48E930 @ 0x0048E930..0x0048EB99 =====
void sub_48E930()
{
  unsigned int v0; // ebx
  signed int v1; // esi
  int v2; // edi
  signed int v3; // ecx
  unsigned int v4; // esi
  unsigned int v5; // ebx
  int v6; // edi
  int v7; // ecx
  unsigned int v8; // ecx
  int v9; // edx
  unsigned int v10; // eax
  unsigned int v11; // eax
  unsigned int v12; // [esp+Ch] [ebp-14h]
  int v13; // [esp+10h] [ebp-10h] BYREF
  int v14; // [esp+14h] [ebp-Ch]
  _DWORD v15[2]; // [esp+18h] [ebp-8h] BYREF

  if ( dword_5668BC )
  {
    if ( sub_49A230() )
    {
      v0 = sub_498720();
      v12 = v0;
      while ( dword_5668EC <= v0 )
      {
        if ( !dword_5668BC )
          break;
        if ( ((sub_48E680(&v13),
               v1 = v13 - dword_5668D0,
               v2 = v14 - dword_5668D4,
               sub_45F650(v15),
               v3 = (unsigned int)(v15[1] + 0xFFFF) >> 16,
               v1 < (signed int)-((unsigned int)(v15[0] + 0xFFFF) >> 16))
           || v1 > (int)((unsigned int)(v15[0] + 0xFFFF) >> 16)
           || v2 < -v3
           || v2 > v3)
          && dword_5668F0 )
        {
          dword_5668BC = 0;
        }
        else
        {
          v4 = dword_5668E0;
          v5 = dword_5668DC + 1;
          dword_5668DC = v5;
          if ( v5 >= dword_5668E0 )
          {
            v6 = dword_5668C0 + dword_5668C8;
            v7 = dword_5668C4 + dword_5668CC;
            dword_5668D0 = dword_5668C0 + dword_5668C8;
            dword_5668BC = 0;
          }
          else
          {
            v6 = dword_5668C0 + sub_48E8D0(v5, dword_5668D8, dword_5668E0, dword_5668C8);
            dword_5668D0 = v6;
            v7 = dword_5668C4 + sub_48E8D0(v5, dword_5668D8, v4, dword_5668CC);
            dword_5668EC = dword_5668E8 + dword_5668E4 * (v5 + 1) / v4;
          }
          dword_5668D4 = v7;
          sub_48E640(v7, v6);
          v0 = v12;
        }
      }
    }
    else
    {
      dword_5668BC = 0;
    }
  }
  if ( dword_5668F4 )
  {
    if ( !sub_49A230() )
      goto LABEL_32;
    sub_48E680(&v13);
    v8 = v13;
    v9 = v14;
    if ( v13 != dword_566904
      || v14 != dword_566908
      || v13 < 0
      || (v10 = sub_4610A0(), v8 >= v10)
      || v9 < 0
      || (v11 = sub_4610C0(), v9 >= v11) )
    {
      dword_566904 = v8;
      dword_566908 = v9;
      if ( dword_5668F8 )
        goto LABEL_34;
      goto LABEL_33;
    }
    if ( IsIconic(hWndParent) )
    {
LABEL_32:
      if ( dword_5668F8 )
        return;
LABEL_33:
      sub_48ED90();
      dword_5668F8 = 1;
LABEL_34:
      dword_566900 = dword_5668FC + sub_498720();
      return;
    }
    if ( dword_5668F8 )
    {
      if ( dword_566900 <= (unsigned int)sub_498720() )
      {
        sub_48ED90();
        dword_5668F8 = 0;
      }
    }
  }
}

// ===== sub_48EBA0 @ 0x0048EBA0..0x0048EBB5 =====
BOOL sub_48EBA0()
{
  dword_56690C = SwapMouseButton(0);
  return SwapMouseButton(dword_56690C);
}

// ===== sub_48EBC0 @ 0x0048EBC0..0x0048EBC6 =====
int sub_48EBC0()
{
  return dword_56690C;
}

// ===== sub_48EBD0 @ 0x0048EBD0..0x0048EC7B =====
int __usercall sub_48EBD0@<eax>(int a1@<esi>, int a2, int a3)
{
  int v4; // ecx

  if ( a1 )
  {
    if ( sub_461F10(1, a1) )
    {
      sub_48ED40(0);
      if ( dword_5668F4 )
        sub_461F20(dword_5668F8, a1);
      dword_566914 = a2;
      dword_566910 = a1;
      dword_566918 = a3;
      dword_503E68 = 0x7FFFFFFF;
      dword_503E6C = 0x7FFFFFFF;
      sub_48EC80();
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if ( dword_566910 )
    {
      sub_461F10(0, dword_566910);
      dword_566910 = 0;
      sub_461D80();
      v4 = dword_5668F8;
      if ( !dword_5668F4 )
        v4 = 1;
      sub_48ED40(v4);
    }
    return 0;
  }
}

// ===== sub_48EC80 @ 0x0048EC80..0x0048ED34 =====
void sub_48EC80()
{
  int v0; // esi
  int v1; // edi
  BOOL v2; // ecx
  int v3; // [esp+8h] [ebp-10h] BYREF
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]
  int v6; // [esp+14h] [ebp-4h]

  if ( dword_566910 )
  {
    sub_48E680(&v3);
    v0 = v3;
    v1 = v4;
    if ( v3 != dword_503E68 || v4 != dword_503E6C )
    {
      dword_503E68 = v3;
      dword_503E6C = v4;
      sub_442E50(&v3);
      v2 = v0 < v3 || v0 > v5 || v1 < v4 || v1 > v6;
      sub_48ED40(v2);
      if ( !sub_461F50(dword_566918 + v1, dword_566914 + v0, dword_566910) )
      {
        dword_566910 = 0;
        sub_48ED40(1);
      }
      sub_461D80();
    }
  }
}

// ===== sub_48ED40 @ 0x0048ED40..0x0048ED81 =====
int __thiscall sub_48ED40(void *this)
{
  int result; // eax
  int v2; // edi

  result = dword_503E70;
  v2 = dword_503E70;
  if ( this )
  {
    if ( dword_503E70 )
      return result;
  }
  else if ( !dword_503E70 )
  {
    return result;
  }
  dword_503E70 = (int)this;
  if ( this )
  {
    while ( ShowCursor(1) < 0 )
      ;
    return v2;
  }
  else
  {
    while ( ShowCursor(0) >= 0 )
      ;
    return v2;
  }
}

// ===== sub_48ED90 @ 0x0048ED90..0x0048EDD5 =====
int __usercall sub_48ED90@<eax>(void *a1@<eax>)
{
  int v1; // esi

  v1 = dword_503E74;
  if ( a1 )
  {
    if ( dword_503E74 )
      return dword_503E74;
  }
  else if ( !dword_503E74 )
  {
    return v1;
  }
  dword_503E74 = (int)a1;
  if ( dword_566910 )
  {
    sub_461F20((int)a1, dword_566910);
    sub_461D80();
    return v1;
  }
  sub_48ED40(a1);
  return v1;
}

// ===== sub_48EDE0 @ 0x0048EDE0..0x0048EE16 =====
BOOL sub_48EDE0()
{
  tagCURSORINFO pci; // [esp+0h] [ebp-14h] BYREF

  memset(&pci.flags, 0, 16);
  pci.cbSize = 20;
  GetCursorInfo(&pci);
  return (pci.flags & 2) == 0;
}

// ===== sub_48EE20 @ 0x0048EE20..0x0048EE43 =====
int sub_48EE20()
{
  int result; // eax
  int v1; // esi

  result = dword_503E74;
  v1 = dword_503E74;
  if ( !dword_566910 )
  {
    if ( dword_503E74 )
    {
      dword_503E74 = sub_48EDE0();
      return v1;
    }
  }
  return result;
}

// ===== sub_48EE50 @ 0x0048EE50..0x0048EE63 =====
int __fastcall sub_48EE50(unsigned int a1)
{
  int result; // eax

  result = 0;
  if ( a1 <= 1 )
  {
    dword_56691C = a1;
    return 1;
  }
  return result;
}

// ===== sub_48EE70 @ 0x0048EE70..0x0048EE76 =====
int sub_48EE70()
{
  return dword_56691C;
}

// ===== sub_48EE80 @ 0x0048EE80..0x0048EF25 =====
int sub_48EE80()
{
  int v0; // esi
  DWORD Type; // [esp+4h] [ebp-110h] BYREF
  DWORD cbData; // [esp+8h] [ebp-10Ch] BYREF
  HKEY phkResult; // [esp+Ch] [ebp-108h] BYREF
  BYTE Data[256]; // [esp+10h] [ebp-104h] BYREF

  v0 = 0;
  if ( !RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, 0x20019u, &phkResult) )
  {
    cbData = 256;
    if ( !RegQueryValueExA(phkResult, "MouseTrails", 0, &Type, Data, &cbData) && Type == 1 )
      v0 = atoi((const char *)Data);
    RegCloseKey(phkResult);
  }
  return v0;
}

// ===== sub_48EF30 @ 0x0048EF30..0x0048EF80 =====
void __cdecl sub_48EF30(int a1)
{
  if ( a1 )
  {
    if ( !dword_566920 )
    {
      if ( sub_48EE80() )
      {
        sub_45FFB0(1);
        dword_566920 = 1;
      }
    }
  }
  else if ( dword_566920 )
  {
    sub_45FFB0(0);
    dword_566920 = 0;
  }
}

// ===== sub_48EF80 @ 0x0048EF80..0x0048F0DA =====
int sub_48EF80()
{
  int result; // eax
  _DWORD *v1; // eax
  _DWORD *v2; // eax
  int v3; // eax
  int v4; // [esp+Ch] [ebp-10h]

  result = 0;
  if ( !dword_566924 )
  {
    if ( CoCreateInstance(&rclsid, 0, 1u, &riid, &ppv) >= 0
      && ((v1 = operator new(0x180u)) == 0 ? (v2 = 0) : (v2 = sub_45B690(v1)),
          !v2 ? (v3 = 0) : (v3 = (int)(v2 + 3)),
          (dword_566934 = v3, v4 >= 0)
       && v3
       && (*(int (__stdcall **)(LPVOID, int, const wchar_t *))(*(_DWORD *)ppv + 12))(ppv, v3, L"TEXTURERENDERER_BURIKO") >= 0
       && (**(int (__stdcall ***)(LPVOID, void *, int *))ppv)(ppv, &unk_4DB8C4, &dword_566948) >= 0
       && (**(int (__stdcall ***)(LPVOID, void *, int *))ppv)(ppv, &unk_4DB8D4, &dword_56694C) >= 0
       && (**(int (__stdcall ***)(LPVOID, void *, int *))ppv)(ppv, &unk_4DB9B4, &dword_566950) >= 0
       && (**(int (__stdcall ***)(LPVOID, void *, int *))ppv)(ppv, &unk_4DB8F4, &dword_566954) >= 0) )
    {
      dword_566924 = 1;
      return 1;
    }
    else
    {
      return 0;
    }
  }
  return result;
}

// ===== sub_48F0E0 @ 0x0048F0E0..0x0048F25D =====
int sub_48F0E0()
{
  int result; // eax
  int v1; // edi
  bool v2; // zf
  int v3; // [esp+8h] [ebp-4h] BYREF

  result = dword_566924;
  v1 = dword_566924;
  if ( dword_566924 )
  {
    if ( sub_45F640() == 1 )
      dword_566964 = 1;
    if ( sub_48F680() )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566948 + 36))(dword_566948);
      do
        (*(void (__stdcall **)(int, _DWORD, int *))(*(_DWORD *)dword_566948 + 40))(dword_566948, 0, &v3);
      while ( v3 );
      dword_566960 = 0;
    }
    if ( dword_566954 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566954 + 8))(dword_566954);
      dword_566954 = 0;
    }
    if ( dword_566950 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566950 + 8))(dword_566950);
      dword_566950 = 0;
    }
    if ( dword_56694C )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_56694C + 8))(dword_56694C);
      dword_56694C = 0;
    }
    if ( dword_566948 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566948 + 8))(dword_566948);
      dword_566948 = 0;
    }
    if ( dword_56693C )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_56693C + 8))(dword_56693C);
      dword_56693C = 0;
    }
    if ( dword_566940 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566940 + 8))(dword_566940);
      dword_566940 = 0;
    }
    if ( dword_566944 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566944 + 8))(dword_566944);
      dword_566944 = 0;
    }
    if ( dword_566938 )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)dword_566938 + 8))(dword_566938);
      dword_566938 = 0;
    }
    if ( ppv )
    {
      (*(void (__stdcall **)(LPVOID))(*(_DWORD *)ppv + 8))(ppv);
      ppv = 0;
    }
    if ( dword_56695C )
    {
      (*(void (__stdcall **)(int))(*(_DWORD *)(dword_56695C + 12) + 8))(dword_56695C + 12);
      dword_56695C = 0;
    }
    if ( dword_566958 )
    {
      (**(void (__thiscall ***)(int, int))dword_566958)(dword_566958, 1);
      dword_566958 = 0;
    }
    dword_566924 = 0;
    v2 = sub_45F640() == 1;
    result = v1;
    if ( v2 )
      dword_566964 = 0;
  }
  return result;
}

// ===== sub_48F260 @ 0x0048F260..0x0048F266 =====
int sub_48F260()
{
  return dword_566964;
}

// ===== sub_48F270 @ 0x0048F270..0x0048F67B =====
int __cdecl sub_48F270(_DWORD *a1, const char *a2)
{
  const char *v2; // ecx
  const char *v3; // esi
  int v4; // eax
  int v5; // eax
  _DWORD *v6; // eax
  _DWORD *v7; // eax
  int v8; // ecx
  int v9; // eax
  int v10; // ecx
  void (__stdcall *v11)(int); // edx
  int v13; // [esp+7Ch] [ebp-D84h] BYREF
  void *v14; // [esp+80h] [ebp-D80h]
  _DWORD *v15; // [esp+84h] [ebp-D7Ch]
  _DWORD v16[2]; // [esp+88h] [ebp-D78h] BYREF
  __int64 v17; // [esp+90h] [ebp-D70h] BYREF
  _DWORD v18[4]; // [esp+98h] [ebp-D68h] BYREF
  _DWORD v19[18]; // [esp+A8h] [ebp-D58h] BYREF
  _BYTE v20[16]; // [esp+F0h] [ebp-D10h] BYREF
  _BYTE v21[96]; // [esp+100h] [ebp-D00h] BYREF
  LONG lDistanceToMove; // [esp+160h] [ebp-CA0h]
  int v23; // [esp+164h] [ebp-C9Ch]
  char v24[784]; // [esp+180h] [ebp-C80h] BYREF
  CHAR FileName[784]; // [esp+490h] [ebp-970h] BYREF
  WCHAR WideCharStr[782]; // [esp+7A0h] [ebp-660h] BYREF
  int v27; // [esp+DFCh] [ebp-4h]

  v15 = a1;
  v3 = v2;
  sub_48F0E0();
  sub_48EF80();
  if ( sub_4666C0(v24, v3) || sub_466440(v24, &Buffer) )
    MultiByteToWideChar(0, 0, v24, -1, WideCharStr, 780);
  else
    MultiByteToWideChar(0, 0, MultiByteStr, -1, WideCharStr, 780);
  v4 = (*(int (__stdcall **)(LPVOID, WCHAR *, _DWORD))(*(_DWORD *)ppv + 52))(ppv, WideCharStr, 0);
  v13 = v4;
  if ( v4 < 0 )
  {
    if ( v4 != -2147220970 || !a2 )
      return 0;
    if ( sub_467BB0((int)v3, (int)v21, FileName, a2) )
    {
      v14 = operator new(0x158u);
      v27 = 0;
      if ( v14 )
        v5 = sub_4C5C10(-1);
      else
        v5 = 0;
      v27 = -1;
      dword_566958 = v5;
      if ( sub_4C5980(FileName, v23, 0, lDistanceToMove, 0) )
        return 0;
      sub_4C0AB0(v19);
      v27 = 1;
      v19[0] = -466162813;
      v19[1] = 298734159;
      v19[2] = 536892319;
      v19[3] = 1889995695;
      v19[4] = -466162812;
      v19[5] = 298734159;
      v19[6] = 536892319;
      v19[7] = 1889995695;
      v13 = 0;
      v6 = operator new(0x1E8u);
      v14 = v6;
      LOBYTE(v27) = 2;
      if ( v6 )
        v7 = sub_44C860((void *)dword_566958, v6, (int)v19, (int)&v13);
      else
        v7 = 0;
      LOBYTE(v27) = 1;
      dword_56695C = (int)v7;
      if ( v13 < 0
        || !v7
        || (((*(void (__stdcall **)(_DWORD *))(v7[3] + 4))(v7 + 3), !dword_56695C) ? (v8 = 0) : (v8 = dword_56695C + 12),
            (v13 = (*(int (__stdcall **)(LPVOID, int, _DWORD))(*(_DWORD *)ppv + 12))(ppv, v8, 0), v13 < 0)
         || ((v9 = (*(int (__thiscall **)(int, _DWORD))(*(_DWORD *)dword_56695C + 28))(dword_56695C, 0)) == 0
           ? (v10 = 0)
           : (v10 = v9 + 12),
             v13 = (*(int (__stdcall **)(LPVOID, int))(*(_DWORD *)ppv + 48))(ppv, v10),
             v13 < 0)) )
      {
        v27 = -1;
        sub_4C0AA0(v19);
        return 0;
      }
      v27 = -1;
      sub_4C0AA0(v19);
    }
  }
  (*(void (__stdcall **)(int, void *, void *))(*(_DWORD *)dword_566950 + 64))(dword_566950, &unk_503E30, &unk_503E38);
  (*(void (__stdcall **)(int, _DWORD, _DWORD))(*(_DWORD *)dword_566950 + 68))(
    dword_566950,
    COERCE_UNSIGNED_INT64(1.0),
    HIDWORD(COERCE_UNSIGNED_INT64(1.0)));
  (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_566954 + 28))(dword_566954, dword_566928);
  if ( (*(int (__stdcall **)(int, HWND, int, _DWORD))(*(_DWORD *)dword_56694C + 52))(
         dword_56694C,
         hWndParent,
         0x8000,
         0) >= 0 )
  {
    v11 = *(void (__stdcall **)(int))(*(_DWORD *)dword_566948 + 28);
    dword_566960 = 1;
    v11(dword_566948);
    v18[0] = 2071483764;
    v18[2] = -1442837316;
    v18[3] = -160125952;
    v18[1] = 298814594;
    (*(void (__stdcall **)(int, _DWORD *))(*(_DWORD *)dword_566950 + 44))(dword_566950, v16);
    (*(void (__stdcall **)(int, _BYTE *))(*(_DWORD *)dword_566950 + 28))(dword_566950, v20);
    (*(void (__stdcall **)(int, __int64 *, _DWORD *, _DWORD, _DWORD, _BYTE *))(*(_DWORD *)dword_566950 + 52))(
      dword_566950,
      &v17,
      v18,
      v16[0],
      v16[1],
      v20);
    *v15 = v17 / 10000;
    return 1;
  }
  return 0;
}

// ===== sub_48F680 @ 0x0048F680..0x0048F686 =====
int sub_48F680()
{
  return dword_566960;
}

// ===== sub_48F690 @ 0x0048F690..0x0048F6E1 =====
BOOL sub_48F690()
{
  int v0; // ecx
  __int64 v2; // [esp+0h] [ebp-10h] BYREF
  __int64 v3; // [esp+8h] [ebp-8h] BYREF

  if ( !sub_48F680() || !dword_566950 )
    return v0;
  (*(void (__stdcall **)(int, __int64 *, __int64 *))(*(_DWORD *)dword_566950 + 60))(dword_566950, &v3, &v2);
  return v3 < v2;
}

// ===== sub_48F6F0 @ 0x0048F6F0..0x0048F75C =====
BOOL __usercall sub_48F6F0@<eax>(unsigned int a1@<eax>)
{
  BOOL v1; // esi
  int v2; // edi

  v1 = a1 <= 0x80;
  if ( a1 <= 0x80 )
  {
    if ( a1 )
      v2 = (int)-((double)(100 * (128 - a1)) / 2.6666666666);
    else
      v2 = -10000;
    if ( !dword_56692C && dword_566954 )
      (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_566954 + 28))(dword_566954, v2);
    dword_566928 = v2;
  }
  return v1;
}

// ===== sub_48F760 @ 0x0048F760..0x0048F7A1 =====
int __usercall sub_48F760@<eax>(int a1@<esi>)
{
  int result; // eax
  int v2; // edi
  int v3; // eax

  result = dword_56692C;
  v2 = dword_56692C;
  if ( a1 )
  {
    if ( dword_56692C )
      return result;
  }
  else if ( !dword_56692C )
  {
    return result;
  }
  if ( dword_566954 )
  {
    v3 = -10000;
    if ( !a1 )
      v3 = dword_566928;
    (*(void (__stdcall **)(int, int))(*(_DWORD *)dword_566954 + 28))(dword_566954, v3);
  }
  dword_56692C = a1;
  return v2;
}

// ===== sub_48F7B0 @ 0x0048F7B0..0x0048F8A2 =====
int __fastcall sub_48F7B0(const char *a1, int a2, int a3, int a4, int a5)
{
  int v8; // [esp+Ch] [ebp-6ACh]
  _BYTE v9[96]; // [esp+10h] [ebp-6A8h] BYREF
  LONG v10; // [esp+70h] [ebp-648h]
  int v11; // [esp+74h] [ebp-644h]
  char v12[784]; // [esp+90h] [ebp-628h] BYREF
  char v13[788]; // [esp+3A0h] [ebp-318h] BYREF

  if ( sub_466440(v12, &Buffer) || sub_466440(v12, byte_517C08) )
    return sub_4083F0(a5, a4, a3, (int)v12, 0, 0);
  if ( a1 && sub_467BB0(a2, (int)v9, v13, a1) )
    return sub_4083F0(a5, a4, a3, (int)v13, v10, v11);
  return v8;
}

// ===== sub_48F8B0 @ 0x0048F8B0..0x0048F8B8 =====
int __usercall sub_48F8B0@<eax>(int a1@<eax>, int a2@<ecx>)
{
  return sub_408430(a2, a1);
}

// ===== sub_48F8C0 @ 0x0048F8C0..0x0048F8CE =====
int __usercall sub_48F8C0@<eax>(int a1@<eax>)
{
  return sub_4084B0((int)dword_566750, a1);
}

// ===== sub_48F8D0 @ 0x0048F8D0..0x0048F8D5 =====
// attributes: thunk
int __fastcall sub_48F8D0(int a1)
{
  return sub_408550(a1);
}

// ===== sub_48F8E0 @ 0x0048F8E0..0x0048F8E7 =====
unsigned int __usercall sub_48F8E0@<eax>(int a1@<eax>, int a2@<edx>, int a3@<ecx>)
{
  return sub_4085A0(a3, a2, a1);
}

// ===== sub_48F8F0 @ 0x0048F8F0..0x0048F8F5 =====
// attributes: thunk
unsigned int __fastcall sub_48F8F0(int a1)
{
  return sub_408600(a1);
}

// ===== sub_48F900 @ 0x0048F900..0x0048F907 =====
unsigned int __usercall sub_48F900@<eax>(int a1@<eax>, int a2@<edx>, int a3@<ecx>)
{
  return sub_408650(a3, a2, a1);
}

// ===== sub_48F910 @ 0x0048F910..0x0048F926 =====
int __usercall sub_48F910@<eax>(int yBottom@<ecx>, int a2@<eax>, int a3, int xRight, int a5)
{
  return sub_408AD0(a5, a3, xRight, yBottom, a2);
}

// ===== sub_48F930 @ 0x0048F930..0x0048F935 =====
// attributes: thunk
int __usercall sub_48F930@<eax>(int a1@<esi>)
{
  return sub_408B80(a1);
}

// ===== sub_48F940 @ 0x0048F940..0x0048F953 =====
int __cdecl sub_48F940(int a1)
{
  return sub_408BE0(a1, (int)dword_566750);
}

// ===== sub_48F960 @ 0x0048F960..0x0048F965 =====
// attributes: thunk
BOOL __usercall sub_48F960@<eax>(int a1@<eax>, _DWORD *a2@<edi>)
{
  return sub_407FB0(a1, a2);
}

// ===== sub_48F970 @ 0x0048F970..0x0048F9FC =====
int __usercall sub_48F970@<eax>(const char *a1@<edi>)
{
  HANDLE MutexA; // ebx
  _DWORD *v2; // esi
  _BYTE *v3; // eax
  const char *v4; // ecx
  _BYTE *v5; // edx
  char v6; // al
  const CHAR *v8; // [esp+0h] [ebp-Ch]

  MutexA = CreateMutexA(0, 1, v8);
  if ( !MutexA )
    return 0;
  if ( GetLastError() )
  {
    CloseHandle(MutexA);
    return 0;
  }
  v2 = operator new(0x10u);
  *v2 = ++dword_566968;
  v3 = operator new[](strlen(a1) + 1);
  v2[1] = v3;
  v4 = a1;
  v5 = v3;
  do
  {
    v6 = *v4;
    *v5++ = *v4++;
  }
  while ( v6 );
  v2[2] = MutexA;
  v2[3] = dword_566974;
  dword_566974 = v2;
  return *v2;
}

// ===== sub_48FA00 @ 0x0048FA00..0x0048FA57 =====
int __fastcall sub_48FA00(int a1, int a2)
{
  void *v2; // esi
  int result; // eax
  int *v4; // ecx

  v2 = dword_566974;
  result = 0;
  v4 = &dword_566968;
  if ( dword_566974 )
  {
    while ( a2 != *(_DWORD *)v2 )
    {
      v4 = (int *)v2;
      v2 = (void *)*((_DWORD *)v2 + 3);
      if ( !v2 )
        return result;
    }
    v4[3] = *((_DWORD *)v2 + 3);
    operator delete[](*((void **)v2 + 1));
    ReleaseMutex(*((HANDLE *)v2 + 2));
    CloseHandle(*((HANDLE *)v2 + 2));
    operator delete(v2);
    return 1;
  }
  return result;
}

// ===== sub_48FA60 @ 0x0048FA60..0x0048FA81 =====
int *__thiscall sub_48FA60(void *this)
{
  int *result; // eax

  for ( result = (int *)dword_566974; dword_566974; result = (int *)dword_566974 )
    sub_48FA00((int)this, *result);
  return result;
}

// ===== sub_48FA90 @ 0x0048FA90..0x0048FAAF =====
_DWORD *__usercall sub_48FA90@<eax>(_DWORD *result@<eax>)
{
  *result = &NCPainter32::`vftable';
  result[7] = 0;
  result[5] = 0;
  result[6] = 0;
  result[8] = 0;
  result[9] = 0;
  result[10] = -16777216;
  return result;
}

// ===== sub_48FAB0 @ 0x0048FAB0..0x0048FAD1 =====
void *__thiscall sub_48FAB0(void *this, char a2)
{
  sub_48FAE0();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_48FAE0 @ 0x0048FAE0..0x0048FAE7 =====
void __thiscall sub_48FAE0(_DWORD *this)
{
  *this = &NCPainter32::`vftable';
}

// ===== sub_48FAF0 @ 0x0048FAF0..0x0048FB56 =====
int __userpurge sub_48FAF0@<eax>(int a1@<edx>, int a2@<ecx>, unsigned int a3@<edi>, unsigned int a4, int a5)
{
  void (__stdcall *v6)(_BYTE *, _DWORD, _DWORD, int, int); // edx
  _BYTE v7[16]; // [esp+4h] [ebp-14h] BYREF

  if ( !a1 || a4 > a3 )
    return 0;
  *(_DWORD *)(a2 + 24) = a5;
  *(_DWORD *)(a2 + 20) = a4;
  *(_DWORD *)(a2 + 28) = a1;
  v6 = **(void (__stdcall ***)(_BYTE *, _DWORD, _DWORD, int, int))a2;
  *(_DWORD *)(a2 + 32) = a3;
  v6(v7, 0, 0, a4 - 1, a5 - 1);
  return 1;
}

// ===== sub_48FB60 @ 0x0048FB60..0x0048FB72 =====
_DWORD *__userpurge sub_48FB60@<eax>(_DWORD *result@<eax>, int a2@<ecx>, int a3)
{
  *result = *(_DWORD *)(a2 + 36);
  *(_DWORD *)(a2 + 36) = a3;
  return result;
}

// ===== sub_48FB80 @ 0x0048FB80..0x0048FB92 =====
_DWORD *__userpurge sub_48FB80@<eax>(_DWORD *result@<eax>, int a2@<ecx>, int a3)
{
  *result = *(_DWORD *)(a2 + 40);
  *(_DWORD *)(a2 + 40) = a3;
  return result;
}

// ===== sub_48FBA0 @ 0x0048FBA0..0x0048FC0A =====
_DWORD *__thiscall sub_48FBA0(_DWORD *this, _DWORD *a2, int xLeft, int yTop, int xRight, int yBottom)
{
  struct tagRECT *v6; // eax
  int v7; // edx
  int v8; // ebx
  int v9; // edi
  int v10; // ecx
  int v11; // edx

  v6 = (struct tagRECT *)(this + 1);
  *a2 = this[1];
  a2[1] = this[2];
  a2[2] = this[3];
  a2[3] = this[4];
  if ( xLeft < 0 )
    xLeft = 0;
  v7 = this[5];
  v8 = xRight;
  if ( xRight >= v7 )
    v8 = v7 - 1;
  v9 = yTop;
  if ( yTop < 0 )
    v9 = 0;
  v10 = this[6];
  v11 = yBottom;
  if ( yBottom >= v10 )
    v11 = v10 - 1;
  SetRect(v6, xLeft, v9, v8, v11);
  return a2;
}

// ===== sub_48FC10 @ 0x0048FC10..0x0048FC5D =====
int __thiscall sub_48FC10(_DWORD *this)
{
  int result; // eax
  int v2; // ebx
  int v3; // edi
  int v4; // esi
  _DWORD *i; // edx

  result = this[2];
  v2 = result;
  v3 = this[7] + 4 * (this[1] + result * this[8]);
  if ( result <= this[4] )
  {
    v4 = this[3];
    do
    {
      result = this[1];
      for ( i = (_DWORD *)v3; result <= v4; ++i )
      {
        *i = this[10];
        v4 = this[3];
        ++result;
      }
      v3 += this[8];
      ++v2;
    }
    while ( v2 <= this[4] );
  }
  return result;
}

// ===== sub_48FC60 @ 0x0048FC60..0x0048FF4E =====
int __thiscall sub_48FC60(_DWORD *this, int a2, int a3, int a4, int a5)
{
  int result; // eax
  int v6; // edx
  int v7; // edi
  int v8; // ebx
  int v9; // esi
  int v10; // ebx
  int v11; // ebx
  unsigned int v12; // eax
  unsigned int v13; // ebx
  int v14; // esi
  int v15; // edx
  int v16; // edi
  int n; // edi
  int v18; // ebx
  _DWORD *ii; // edi
  int v20; // ebx
  int v21; // edi
  int m; // eax
  int *v23; // edi
  int v24; // eax
  int v25; // edx
  int v26; // esi
  int v27; // eax
  int v28; // esi
  int v29; // ebx
  int v30; // esi
  int i; // eax
  _DWORD *j; // edi
  int v33; // [esp+4h] [ebp-4h]
  int v34; // [esp+10h] [ebp+8h]
  int v35; // [esp+14h] [ebp+Ch]
  int v36; // [esp+14h] [ebp+Ch]
  int v37; // [esp+18h] [ebp+10h]
  int v38; // [esp+18h] [ebp+10h]
  int k; // [esp+18h] [ebp+10h]
  int v40; // [esp+18h] [ebp+10h]
  int v41; // [esp+1Ch] [ebp+14h]

  result = this[1];
  v6 = a4;
  v7 = a2;
  if ( a2 >= result || a4 >= result )
  {
    result = this[3];
    if ( a2 <= result || a4 <= result )
    {
      result = a5;
      v8 = this[2];
      v9 = a3;
      if ( a3 >= v8 || a5 >= v8 )
      {
        v10 = this[4];
        if ( a3 <= v10 || a5 <= v10 )
        {
          if ( a3 > a5 )
          {
            v7 = a4;
            v6 = a2;
            v9 = a5;
            result = a3;
            a4 = a2;
            a3 = a5;
            a5 = result;
          }
          if ( v7 > v6 )
            v11 = v7 - v6;
          else
            v11 = v6 - v7;
          v12 = result - v9 + 1;
          v13 = v11 + 1;
          if ( v13 <= v12 )
          {
            v24 = (v13 << 12) / v12;
            v25 = this[2];
            v26 = v7 << 12;
            v34 = v24;
            if ( a3 >= v25 )
            {
              v25 = a3;
              v33 = 0;
            }
            else
            {
              v33 = -(a3 * v24);
            }
            v27 = this[4];
            if ( a5 <= v27 )
              v36 = a5 + 1;
            else
              v36 = v27 + 1;
            if ( v7 > a4 )
            {
              v30 = v26 - v33;
              for ( i = this[3]; v30 >> 12 > i; ++v25 )
                v30 -= v34;
              result = v30 >> 12;
              v40 = v30 >> 12;
              if ( v30 >> 12 > -1 )
              {
                for ( j = (_DWORD *)(this[7] + v25 * this[8] + 4 * result); v25 < v36; v40 = v30 >> 12 )
                {
                  v30 -= v34;
                  *j = this[9];
                  result = v30 >> 12;
                  if ( v40 != v30 >> 12 )
                  {
                    --j;
                    if ( result <= -1 )
                      break;
                  }
                  j = (_DWORD *)((char *)j + this[8]);
                  ++v25;
                }
              }
            }
            else
            {
              v28 = v33 + v26;
              result = this[1];
              for ( k = this[3] + 1; v28 < result; ++v25 )
                v28 += v34;
              v29 = v28 >> 12;
              if ( v28 >> 12 < this[3] + 1 )
              {
                for ( result = this[7] + v25 * this[8] + 4 * v29; v25 < v36; v29 = v28 >> 12 )
                {
                  v28 += v34;
                  *(_DWORD *)result = this[9];
                  if ( v29 != v28 >> 12 )
                  {
                    result += 4;
                    if ( v28 >> 12 >= k )
                      break;
                  }
                  result += this[8];
                  ++v25;
                }
              }
            }
          }
          else
          {
            v14 = v9 << 12;
            v15 = (v12 << 12) / v13;
            v35 = v15;
            if ( v7 > a4 )
            {
              if ( v7 <= this[3] )
              {
                v20 = v7;
              }
              else
              {
                v20 = this[3];
                v14 += v15 * (v7 - v20);
              }
              v21 = this[1];
              if ( a4 >= v21 )
                v38 = a4 - 1;
              else
                v38 = v21 - 1;
              for ( m = this[2]; v14 >> 12 < m; --v20 )
                v14 += v15;
              result = v14 >> 12;
              v41 = v14 >> 12;
              if ( v14 >> 12 < this[4] )
              {
                v23 = (int *)(this[7] + result * this[8] + 4 * v20);
                if ( v20 > v38 )
                {
                  while ( 1 )
                  {
                    result = this[9];
                    v14 += v15;
                    *v23 = result;
                    if ( v41 != v14 >> 12 )
                    {
                      v23 = (int *)((char *)v23 + this[8]);
                      if ( v14 >> 12 > this[4] )
                        break;
                    }
                    --v20;
                    --v23;
                    v41 = v14 >> 12;
                    if ( v20 <= v38 )
                      break;
                    v15 = v35;
                  }
                }
              }
            }
            else
            {
              result = this[1];
              if ( v7 >= result )
                result = v7;
              else
                v14 -= v7 * v15;
              v16 = this[3];
              if ( a4 <= v16 )
                v37 = a4 + 1;
              else
                v37 = v16 + 1;
              for ( n = this[2]; v14 >> 12 < n; ++result )
                v14 += v15;
              v18 = v14 >> 12;
              if ( v14 >> 12 < this[4] )
              {
                for ( ii = (_DWORD *)(this[7] + v18 * this[8] + 4 * result); result < v37; v18 = v14 >> 12 )
                {
                  v14 += v15;
                  *ii = this[9];
                  if ( v18 != v14 >> 12 )
                  {
                    ii = (_DWORD *)((char *)ii + this[8]);
                    if ( v14 >> 12 > this[4] )
                      break;
                  }
                  ++result;
                  ++ii;
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}

// ===== sub_48FF50 @ 0x0048FF50..0x00490091 =====
_DWORD *__stdcall sub_48FF50(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  long double v4; // [esp+10h] [ebp-1Ch]

  sub_4907E0();
  a1[7] = -100;
  a1[9] = -100;
  a1[11] = -100;
  a1[8] = 100;
  a1[10] = 100;
  a1[12] = 100;
  a1[13] = 0;
  a1[14] = -1;
  a1[15] = 0;
  a1[16] = 15360;
  a1[17] = 89600;
  a1[23] = 0;
  a1[24] = -60;
  a1[25] = 0;
  a1[26] = 0;
  a1[27] = -350;
  a1[28] = 0;
  a1[18] = -1;
  a1[19] = 0;
  a1[20] = 100;
  a1[21] = 20;
  a1[22] = 1;
  a1[29] = 0;
  a1[30] = 0;
  a1[31] = 0;
  a1[32] = 0;
  a1[33] = 0;
  a1[34] = 0;
  a1[35] = 10;
  v4 = 0.01745329251944444 * 0.0 / 10.0;
  v1 = (int)(sin(v4) * 256.0);
  a1[36] = v1;
  v2 = (int)(cos(v4) * 256.0);
  a1[37] = v2;
  a1[38] = v1;
  a1[39] = v2;
  a1[40] = v1;
  a1[41] = v2;
  a1[5] = 0;
  return a1;
}
