#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== sub_488010 @ 0x00488010..0x0048803C =====
int __cdecl sub_488010(_DWORD *a1)
{
  int v1; // eax
  unsigned __int16 v2; // ax

  v1 = sub_4450B0(a1);
  v2 = sub_46D560(v1);
  sub_4450D0(a1, (v2 >> 15) & 1);
  return 0;
}

// ===== sub_488040 @ 0x00488040..0x0048807D =====
int __cdecl sub_488040(_DWORD *a1)
{
  int v1; // eax
  int i; // esi
  int v3; // ecx

  v1 = *(_DWORD *)sub_48DF50(a1);
  for ( i = 0; v1; v1 = *(_DWORD *)(v3 + 4) )
    i += sub_46DC70(v1);
  sub_4450D0(a1, i);
  return 0;
}

// ===== sub_488080 @ 0x00488080..0x00488098 =====
int __cdecl sub_488080(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_46E5C0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4880A0 @ 0x004880A0..0x004880B4 =====
int __cdecl sub_4880A0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46D980(v1);
  return 0;
}

// ===== sub_4880C0 @ 0x004880C0..0x004880D4 =====
int __cdecl sub_4880C0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46D990(v1);
  return 0;
}

// ===== sub_4880E0 @ 0x004880E0..0x004880E8 =====
int sub_4880E0()
{
  sub_46D9A0();
  return 0;
}

// ===== sub_4880F0 @ 0x004880F0..0x00488108 =====
int __cdecl sub_4880F0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_46DE30();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_488110 @ 0x00488110..0x00488144 =====
int __cdecl sub_488110(_DWORD *a1)
{
  _DWORD *v1; // esi

  v1 = (_DWORD *)((sub_4450B0(a1) << 16) | 0xFFFF);
  sub_46D6C0((unsigned int)v1);
  sub_46D720((unsigned int)v1);
  sub_46DF00((unsigned int)v1, v1);
  return 0;
}

// ===== sub_488150 @ 0x00488150..0x00488181 =====
int __cdecl sub_488150(_DWORD *a1)
{
  _DWORD *v1; // esi

  v1 = (_DWORD *)((sub_4450B0(a1) << 16) | 0xFFFF);
  sub_46DF00((unsigned int)v1, v1);
  sub_46D7A0((int)v1);
  sub_46D7B0((int)v1);
  return 0;
}

// ===== sub_488190 @ 0x00488190..0x004881BF =====
int __cdecl sub_488190(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46DF00((v1 << 16) | 0xFFFF, (_DWORD *)((v1 << 16) | 0xFFFF));
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4881C0 @ 0x004881C0..0x00488254 =====
int __cdecl sub_4881C0(_DWORD *a1)
{
  unsigned int v1; // edi
  char *v2; // edx
  int v3; // eax
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_46DFA0(v1, v2);
  if ( v3 == -2147483647 )
  {
    sprintf(Buffer, &byte_4EB80C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v3 == -2147483646 )
  {
    sprintf(Buffer, &byte_4EB840, 15);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_488260 @ 0x00488260..0x00488287 =====
int __cdecl sub_488260(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46E070(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488290 @ 0x00488290..0x004882EB =====
int __cdecl sub_488290(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // eax
  int v3; // esi
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)((sub_4450B0(a1) << 16) | 0xFFFF);
  v3 = 0;
  if ( v1 == 1 || v1 == 2 )
    v4 = sub_46D830(v2);
  else
    v4 = sub_46D810((unsigned int)v2);
  if ( v4 )
    v3 = sub_46DB40(v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4882F0 @ 0x004882F0..0x00488313 =====
int __cdecl sub_4882F0(_DWORD *a1)
{
  int v1; // eax
  __int64 v2; // rax

  v1 = sub_4450B0(a1);
  v2 = sub_48EE50(v1);
  sub_4450D0((_DWORD *)HIDWORD(v2), v2);
  return 0;
}

// ===== sub_488320 @ 0x00488320..0x0048837E =====
int __cdecl sub_488320(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // ebx
  _DWORD *v4; // edx
  _DWORD *v5; // edx
  _DWORD *v6; // edx
  _DWORD *v7; // edx
  int v8; // eax
  int v10; // [esp+Ch] [ebp-8h]
  int v11; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  sub_4450B0(v4);
  v11 = sub_4450B0(v5);
  v10 = sub_4450B0(v6);
  v8 = sub_4450B0(v7);
  sub_48E780(v8, v10, v11, v3, v1);
  return 0;
}

// ===== sub_488380 @ 0x00488380..0x004883C2 =====
int __cdecl sub_488380(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4978F0(v1);
  v2 = sub_48DC70(v1);
  if ( !v2 )
    sub_4646F0(byte_4EB89C, (int)a1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4883D0 @ 0x004883D0..0x00488443 =====
int __cdecl sub_4883D0(_DWORD *a1)
{
  int v1; // edi
  int v2; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = 1;
  if ( v1 )
  {
    v2 = sub_48DD10();
    if ( !v2 )
    {
      sprintf(Buffer, &byte_4EB8C0, v1);
      sub_4646F0(Buffer, (int)a1);
    }
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488450 @ 0x00488450..0x0048848E =====
int __cdecl sub_488450(_DWORD *a1)
{
  int v1; // esi
  char *v2; // eax
  unsigned int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (char *)sub_48DF50(a1);
  v3 = sub_466BB0(0, 0, 0, v2, v1, 0, 0x7FFFFFFFu);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488490 @ 0x00488490..0x00488501 =====
int __cdecl sub_488490(_DWORD *a1)
{
  unsigned int v1; // ebx
  char *v2; // esi
  int v3; // eax
  int v5; // [esp+Ch] [ebp-Ch] BYREF
  LPCSTR lpFileName; // [esp+10h] [ebp-8h]
  int v7; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  lpFileName = (LPCSTR)sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  v2 = (char *)sub_48DF50(a1);
  v3 = sub_466BB0(v2, &v5, v5, (char *)lpFileName, v7, 0, v1);
  if ( !v2 )
    v3 = v5;
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488510 @ 0x00488510..0x0048855F =====
int __cdecl sub_488510(_DWORD *a1)
{
  unsigned int v1; // esi
  const CHAR *v2; // ebx
  unsigned int v3; // eax
  unsigned int v5; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v5 = sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_466F00(v5, v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488560 @ 0x00488560..0x0048858F =====
int __cdecl sub_488560(_DWORD *a1)
{
  const CHAR *v1; // esi
  const CHAR *v2; // eax
  BOOL v3; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v3 = MoveFileA(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488590 @ 0x00488590..0x004885B7 =====
int __cdecl sub_488590(_DWORD *a1)
{
  const CHAR *v1; // eax
  BOOL DirectoryA; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  DirectoryA = CreateDirectoryA(v1, 0);
  sub_4450D0(a1, DirectoryA);
  return 0;
}

// ===== sub_4885C0 @ 0x004885C0..0x004885E5 =====
int __cdecl sub_4885C0(_DWORD *a1)
{
  const CHAR *v1; // eax
  BOOL v2; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = RemoveDirectoryA(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4885F0 @ 0x004885F0..0x00488622 =====
int __cdecl sub_4885F0(_DWORD *a1)
{
  const CHAR *v1; // eax
  int v2; // esi
  DWORD FileAttributesA; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = 0;
  FileAttributesA = GetFileAttributesA(v1);
  if ( FileAttributesA != -1 )
    v2 = (FileAttributesA >> 4) & 1;
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488630 @ 0x00488630..0x0048869A =====
int __cdecl sub_488630(_DWORD *a1)
{
  const CHAR *v1; // edi
  CHAR *v2; // esi
  CHAR *v3; // eax
  int v4; // ecx
  CHAR *v6; // [esp+Ch] [ebp-8h]
  CHAR *v7; // [esp+10h] [ebp-4h]

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (CHAR *)sub_48DF50(a1);
  v7 = (CHAR *)sub_48DF50(a1);
  v6 = (CHAR *)sub_48DF50(a1);
  v3 = (CHAR *)sub_48DF50(a1);
  v4 = 0;
  if ( v1 )
  {
    sub_42D410(v2, v6, v3, v7, v1);
    v4 = 1;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4886A0 @ 0x004886A0..0x004886C5 =====
int __cdecl sub_4886A0(_DWORD *a1)
{
  const CHAR *v1; // eax
  DWORD FileAttributesA; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  FileAttributesA = GetFileAttributesA(v1);
  sub_4450D0(a1, FileAttributesA);
  return 0;
}

// ===== sub_4886D0 @ 0x004886D0..0x004886FF =====
int __cdecl sub_4886D0(_DWORD *a1)
{
  DWORD v1; // esi
  const CHAR *v2; // eax
  BOOL v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v3 = SetFileAttributesA(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488700 @ 0x00488700..0x0048874C =====
int __cdecl sub_488700(_DWORD *a1)
{
  const CHAR *v1; // ebx
  const CHAR *v2; // esi
  DWORD FileAttributesA; // eax
  BOOL v4; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  FileAttributesA = GetFileAttributesA(v2);
  if ( FileAttributesA != -1 )
    SetFileAttributesA(v2, FileAttributesA & 0xFFFFFFFE);
  v4 = CopyFileA(v1, v2, 0);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_488750 @ 0x00488750..0x0048878F =====
int __cdecl sub_488750(_DWORD *a1)
{
  _BYTE *v1; // ebx
  const char *v2; // esi
  _DWORD *v3; // eax
  size_t v4; // eax

  v1 = (_BYTE *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_465AB0(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_488790 @ 0x00488790..0x004887EE =====
int __cdecl sub_488790(_DWORD *a1)
{
  int v1; // ebx
  int v2; // esi
  void *v3; // eax
  int v4; // eax
  _BYTE *v6; // [esp+Ch] [ebp-8h]
  const char *v7; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = (_BYTE *)sub_48DF50(a1);
  v7 = (const char *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_465C30(v6, v3, v7, __SPAIR64__(v1, v2));
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_4887F0 @ 0x004887F0..0x00488835 =====
int __cdecl sub_4887F0(_DWORD *a1)
{
  DWORD v1; // esi
  const void *v2; // ebx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const void *)sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = sub_465E30(v2, v1);
  sub_4450D0(a1, v3 == v1);
  return 0;
}

// ===== sub_488840 @ 0x00488840..0x004888BD =====
int __cdecl sub_488840(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  BOOL v3; // eax
  char Buffer[780]; // [esp+8h] [ebp-310h] BYREF

  v1 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  if ( v2 )
    sprintf(Buffer, "%s\\%s", v2, v1);
  else
    sprintf(Buffer, "%s%s", &::Buffer, v1);
  v3 = DeleteFileA(Buffer);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_4888C0 @ 0x004888C0..0x004888F2 =====
int __cdecl sub_4888C0(_DWORD *a1)
{
  const char *v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_4665C0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488900 @ 0x00488900..0x00488932 =====
int __cdecl sub_488900(_DWORD *a1)
{
  _BYTE *v1; // esi
  size_t v2; // eax

  v1 = (_BYTE *)sub_48DF50(a1);
  sub_48DF50(a1);
  v2 = sub_4662E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488940 @ 0x00488940..0x00488954 =====
int __cdecl sub_488940(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_465240(v1);
  return 0;
}

// ===== sub_488960 @ 0x00488960..0x00488978 =====
int __cdecl sub_488960(int a1)
{
  const char *v1; // eax

  v1 = (const char *)sub_48DF50(a1);
  sub_465290(v1);
  return 0;
}

// ===== sub_488980 @ 0x00488980..0x004889FA =====
int __cdecl sub_488980(_DWORD *a1)
{
  _DWORD *v1; // edi
  int v2; // esi
  int v3; // esi
  const char **v4; // ebx
  int v5; // eax
  const char *v7; // [esp+Ch] [ebp-4h]

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = 0;
  v7 = (const char *)sub_48DF50(a1);
  if ( *v1 )
  {
    do
      ++v2;
    while ( v1[v2] );
  }
  v3 = v2 + 1;
  v4 = (const char **)operator new[](4 * v3);
  sub_48DF70(v4, a1, v3);
  v5 = sub_466880(v7, v4);
  sub_4450D0(a1, v5);
  operator delete[](v4);
  return 0;
}

// ===== sub_488A00 @ 0x00488A00..0x00488A25 =====
int __cdecl sub_488A00(_DWORD *a1)
{
  const char *v1; // eax
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_46B390(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488A30 @ 0x00488A30..0x00488A62 =====
int __cdecl sub_488A30(_DWORD *a1)
{
  BYTE *v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = (BYTE *)sub_48DF50(a1);
  v2 = sub_467570(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488A70 @ 0x00488A70..0x00488ADD =====
int __cdecl sub_488A70(_DWORD *a1)
{
  int v1; // esi
  const CHAR *v2; // edi
  void *v3; // eax
  int v4; // eax
  const char *v6; // [esp+Ch] [ebp-Ch]
  const char *v7; // [esp+10h] [ebp-8h]
  const CHAR *v8; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v8 = (const CHAR *)sub_48DF50(a1);
  v7 = (const char *)sub_48DF50(a1);
  v6 = (const char *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_466B10(v2, v6, v3, v7, v8, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_488AE0 @ 0x00488AE0..0x00488B1C =====
int __cdecl sub_488AE0(_DWORD *a1)
{
  int v1; // ebx
  const char *v2; // eax
  BOOL v3; // eax

  v1 = sub_48DF50(a1);
  sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_4667A0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488B20 @ 0x00488B20..0x00488B4E =====
int __cdecl sub_488B20(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_4649A0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_488B50 @ 0x00488B50..0x00488BA8 =====
int __cdecl sub_488B50(_DWORD *a1)
{
  const CHAR *v1; // edi
  DWORD FileAttributesA; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  FileAttributesA = GetFileAttributesA(v1);
  if ( FileAttributesA == -1 || (FileAttributesA & 0x10) == 0 )
  {
    sub_4450D0(a1, 0);
    return 0;
  }
  else
  {
    sprintf(&Buffer, "%s\\", v1);
    sub_4450D0(a1, 1);
    return 0;
  }
}

// ===== sub_488BB0 @ 0x00488BB0..0x00488C00 =====
int __cdecl sub_488BB0(_DWORD *a1)
{
  int v1; // esi
  const char *v2; // ebx
  const char *v3; // eax
  int v4; // eax
  const char *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v6 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_464F40(v3, v2, v6, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_488C00 @ 0x00488C00..0x00488CC3 =====
int __cdecl sub_488C00(_DWORD *a1)
{
  char *v1; // ebx
  _DWORD *v2; // esi
  int v3; // eax
  const char *v5; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = (char *)sub_48DF50(a1);
  v5 = (const char *)sub_48DF50(a1);
  v2 = operator new[](0x20000u);
  if ( !sub_465AB0(v1, v2, v5) )
  {
    sprintf(Buffer, &byte_4EB960, v5, v1);
LABEL_4:
    sub_4646F0(Buffer, (int)a1);
  }
  v3 = sub_444CE0(a1, v2, v1);
  if ( v3 == 0x80000000 )
  {
    sprintf(Buffer, &byte_4EB908, v5, v1);
    goto LABEL_4;
  }
  sub_4450D0(a1, v3);
  operator delete[](v2);
  return 0;
}

// ===== sub_488CD0 @ 0x00488CD0..0x00488CFD =====
int __cdecl sub_488CD0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_444D80(a1);
  if ( !v1 )
    sub_4646F0(byte_4EB990, (int)a1);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_488D00 @ 0x00488D00..0x00488D61 =====
int __cdecl sub_488D00(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax
  int v4; // [esp+Ch] [ebp-8h]
  int v5; // [esp+10h] [ebp-4h]

  sub_4450B0(a1);
  sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v4 = sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_48D080(v1, v4, v5, a1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== __RTC_NumErrors @ 0x00488D70..0x00488D76 =====
int __cdecl _RTC_NumErrors()
{
  return 4;
}

// ===== sub_488D80 @ 0x00488D80..0x00488D9C =====
int __cdecl sub_488D80(int a1)
{
  int v1; // eax
  _DWORD *v2; // ecx

  v1 = sub_42D560(a1);
  sub_4450D0(v2, v1);
  return 0;
}

// ===== sub_488DA0 @ 0x00488DA0..0x00488DD1 =====
int __cdecl sub_488DA0(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v2; // eax
  int v4; // [esp-4h] [ebp-Ch]

  v4 = sub_4450B0(a1);
  v1 = sub_444A60(a1);
  v2 = sub_444B90(v1, v4);
  sub_4450D0(a1, v2 != 0);
  return 0;
}

// ===== sub_488DE0 @ 0x00488DE0..0x00488E22 =====
int __cdecl sub_488DE0(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // eax
  int v3; // eax
  int v5; // [esp-4h] [ebp-Ch]

  v1 = sub_4450B0(a1);
  v5 = sub_4450B0(a1);
  v2 = sub_444A60(a1);
  v3 = sub_444B90(v2, v5);
  if ( !v3 )
    sub_4646F0(byte_4EB9BC, (int)a1);
  sub_4452C0(v3, v1);
  return 0;
}

// ===== sub_488E30 @ 0x00488E30..0x00488E59 =====
int __cdecl sub_488E30(_DWORD *a1)
{
  _DWORD *v1; // eax
  BOOL v2; // eax

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_445300((int)a1, v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_488E60 @ 0x00488E60..0x00488F0B =====
int __cdecl sub_488E60(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // eax
  int v3; // ebx
  int i; // esi
  int v6; // [esp-4h] [ebp-118h]
  int v7; // [esp+Ch] [ebp-108h]
  char Buffer[256]; // [esp+10h] [ebp-104h] BYREF

  v7 = sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_444A60(a1);
  v3 = sub_444B90(v2, v6);
  if ( !v3 )
    sub_4646F0(byte_4EB9BC, (int)a1);
  if ( v1 < 1 )
  {
    sprintf(Buffer, &byte_4EB9E4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  for ( i = 0; i < v1; ++i )
    sub_4452C0(v3, *(_DWORD *)(v7 + 4 * i));
  return 0;
}

// ===== sub_488F10 @ 0x00488F10..0x00488FC0 =====
int __cdecl sub_488F10(_DWORD *a1)
{
  _DWORD *v1; // edi
  int v2; // eax
  int v3; // ebx
  int v4; // esi
  BOOL v5; // eax
  int i; // [esp+10h] [ebp-108h]
  char Buffer[256]; // [esp+14h] [ebp-104h] BYREF

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v3 = v2;
  if ( v2 < 1 )
  {
    sprintf(Buffer, &byte_4EB9E4, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  v4 = 0;
  v5 = 1;
  for ( i = 0; v4 < v3; i += v5 )
  {
    if ( !v5 )
      break;
    v5 = sub_445300((int)a1, v1);
    ++v4;
    ++v1;
  }
  sub_4450D0(a1, i);
  return 0;
}

// ===== sub_488FC0 @ 0x00488FC0..0x00489027 =====
int __cdecl sub_488FC0(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  int v3; // edi
  BOOL v4; // eax
  int v6; // [esp-4h] [ebp-18h]
  int v7; // [esp+Ch] [ebp-8h]
  int v8; // [esp+10h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v7 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v2 = sub_444A60(a1);
  v3 = sub_444B90(v2, v6);
  v4 = 0;
  if ( v3 )
    v4 = sub_445230(v1, v7, v3, v8);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_489030 @ 0x00489030..0x00489047 =====
int __cdecl sub_489030(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_4319B0(v1);
  return 1;
}

// ===== sub_489050 @ 0x00489050..0x00489064 =====
int __cdecl sub_489050(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_48D1B0();
  return 0;
}

// ===== sub_489070 @ 0x00489070..0x0048907D =====
int sub_489070()
{
  sub_48D190();
  return 0;
}

// ===== sub_489080 @ 0x00489080..0x004890F2 =====
int __cdecl sub_489080(_DWORD *a1)
{
  _DWORD *v1; // eax
  _DWORD *v2; // edx
  void *v4; // [esp+8h] [ebp-10h]

  v4 = operator new(0x24u);
  v1 = 0;
  if ( v4 )
  {
    sub_4450B0(a1);
    v1 = sub_43D4D0(a1, v2);
  }
  sub_4451C0((int)a1, (int)v1);
  return 2;
}

// ===== sub_489100 @ 0x00489100..0x00489118 =====
int __cdecl sub_489100(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx

  v1 = sub_4450B0(a1);
  sub_445290(v2, v1);
  return 0;
}

// ===== sub_489120 @ 0x00489120..0x00489153 =====
int __cdecl sub_489120(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4452B0((int)a1, v1);
  v2 = sub_445260((int)a1);
  sub_4450D0(a1, v2 != 0);
  return 0;
}

// ===== sub_489160 @ 0x00489160..0x004891EF =====
int __cdecl sub_489160(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // eax
  _DWORD *v3; // eax

  v1 = sub_445260((int)a1);
  if ( v1 )
  {
    v2 = operator new(0x20u);
    if ( v2 )
      v3 = sub_43D1D0(a1, v2, v1);
    else
      v3 = 0;
    sub_4451C0((int)a1, (int)v3);
  }
  sub_4450D0(a1, v1 != 0);
  return 2;
}

// ===== sub_4891F0 @ 0x004891F0..0x00489286 =====
int __cdecl sub_4891F0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  int v6; // [esp+14h] [ebp-10h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v3 = operator new(0x2Cu);
  if ( v3 )
    v4 = sub_43D2E0((int)a1, v1, v3, v6, v2);
  else
    v4 = 0;
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_489290 @ 0x00489290..0x004892AC =====
int __cdecl sub_489290(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_48D170();
  return 0;
}

// ===== sub_4892B0 @ 0x004892B0..0x004892C7 =====
int __cdecl sub_4892B0(_DWORD *a1)
{
  dword_566894 = sub_4450B0(a1);
  return 3;
}

// ===== sub_4892D0 @ 0x004892D0..0x004892D6 =====
int sub_4892D0()
{
  return 1;
}

// ===== sub_4892E0 @ 0x004892E0..0x00489332 =====
int __cdecl sub_4892E0(_DWORD *a1)
{
  int v1; // edi
  _DWORD *v2; // edx
  unsigned int v3; // esi
  _DWORD *v4; // edx
  unsigned int v5; // eax
  int v6; // edx

  v1 = sub_4450B0(a1);
  v3 = sub_4450B0(v2);
  v5 = sub_4450B0(v4);
  if ( v3 >= 2 )
    sub_4646F0(byte_4EBA18, v6);
  if ( v5 >= 8 )
    sub_4646F0(byte_4EBA40, v6);
  sub_461110(v5, v3, v1);
  return 0;
}

// ===== sub_489340 @ 0x00489340..0x00489358 =====
int __cdecl sub_489340(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_45F640();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_489360 @ 0x00489360..0x00489394 =====
int __cdecl sub_489360(_DWORD *a1)
{
  int v1; // eax
  char *v2; // edx

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  if ( !sub_461740(v2, v1) )
    sub_4646F0(byte_4EBA68, (int)a1);
  return 0;
}

// ===== sub_4893A0 @ 0x004893A0..0x004893BB =====
int __cdecl sub_4893A0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_45E860(v1 == 0);
  return 0;
}

// ===== sub_4893C0 @ 0x004893C0..0x004893DD =====
int __cdecl sub_4893C0(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_49A170();
  sub_46DA20();
  return 0;
}

// ===== sub_4893E0 @ 0x004893E0..0x004893FE =====
int sub_4893E0()
{
  CloseWindow(hWndParent);
  sub_46DA20();
  dword_5666F4 = 1;
  return 0;
}

// ===== sub_489400 @ 0x00489400..0x0048942B =====
int __cdecl sub_489400(int a1)
{
  char *v1; // esi

  v1 = (char *)sub_48DF50(a1);
  if ( SetWindowTextA(hWndParent, v1) )
    sub_46BBE0(v1);
  return 0;
}

// ===== sub_489430 @ 0x00489430..0x0048948A =====
int __cdecl sub_489430(_DWORD *a1)
{
  unsigned int v1; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  if ( v1 > 4 )
  {
    sprintf(Buffer, &byte_4EBAB4, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  dword_5666D8 = v1;
  return 0;
}

// ===== sub_489490 @ 0x00489490..0x004894A4 =====
int __cdecl sub_489490(_DWORD *a1)
{
  sub_4450B0(a1);
  sub_49A0D0();
  return 0;
}

// ===== sub_4894B0 @ 0x004894B0..0x004894C5 =====
int sub_4894B0()
{
  PostMessageA(hWndParent, 0x10u, 0, 0);
  return 0;
}

// ===== sub_4894D0 @ 0x004894D0..0x004894D6 =====
int sub_4894D0()
{
  return 6;
}

// ===== sub_4894E0 @ 0x004894E0..0x0048950C =====
int __cdecl sub_4894E0(int a1)
{
  char *v1; // edi

  v1 = (char *)sub_48DF50(a1);
  sub_48DF50(a1);
  sub_4650F0(v1);
  return 5;
}

// ===== sub_489510 @ 0x00489510..0x00489524 =====
int __cdecl sub_489510(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_463A80(v1);
  return 0;
}

// ===== sub_489530 @ 0x00489530..0x00489557 =====
int __cdecl sub_489530(_DWORD *a1)
{
  char *v1; // eax
  int v2; // eax

  v1 = (char *)sub_48DF50(a1);
  v2 = sub_463AE0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_489560 @ 0x00489560..0x00489574 =====
int __cdecl sub_489560(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_45F6F0(v1);
  return 0;
}

// ===== sub_489580 @ 0x00489580..0x00489598 =====
int __cdecl sub_489580(_DWORD *a1)
{
  BOOL v1; // eax

  v1 = sub_461550();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_4895A0 @ 0x004895A0..0x0048960A =====
int __cdecl sub_4895A0(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_46BB90(v1);
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EBAE8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_489610 @ 0x00489610..0x0048962A =====
int sub_489610()
{
  memset(dword_566758, 0, Size);
  return 0;
}

// ===== sub_489630 @ 0x00489630..0x00489644 =====
int __cdecl sub_489630(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46B420(v1);
  return 0;
}

// ===== sub_489650 @ 0x00489650..0x004896CE =====
int __cdecl sub_489650(_DWORD *a1)
{
  const char *v1; // edi
  int v2; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  if ( strlen(v1) >= 0x28 )
  {
    sprintf(Buffer, &byte_4EBB18, 39);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_465F90(v2);
  return 0;
}

// ===== sub_4896D0 @ 0x004896D0..0x00489725 =====
int __cdecl sub_4896D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax
  _BYTE v4[1024]; // [esp+8h] [ebp-400h] BYREF

  qmemcpy(v4, dword_566758, sizeof(v4));
  v1 = sub_4450B0(a1);
  v2 = sub_4661A0(v1);
  sub_4450D0(a1, v2);
  qmemcpy(dword_566758, v4, 0x400u);
  return 0;
}

// ===== sub_489730 @ 0x00489730..0x00489761 =====
int __cdecl sub_489730(_DWORD *a1)
{
  int v1; // esi
  void *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_48DF50(a1);
  v3 = sub_466240(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_489770 @ 0x00489770..0x00489797 =====
int __cdecl sub_489770(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_466200(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4897A0 @ 0x004897A0..0x00489802 =====
int __cdecl sub_4897A0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edi
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  int v6[2]; // [esp+8h] [ebp-8h] BYREF

  v1 = sub_46B680(v6);
  v2 = v1;
  if ( v1 )
  {
    if ( v1 == -2147483647 )
    {
      v2 = 1;
    }
    else if ( v1 == -2147483646 )
    {
      v2 = 2;
    }
  }
  else
  {
    v2 = 0;
  }
  v3 = sub_4450D0(a1, v6[0]);
  v4 = sub_4450D0(v3, v6[1]);
  sub_4450D0(v4, v2);
  return 0;
}

// ===== sub_489810 @ 0x00489810..0x00489828 =====
int __cdecl sub_489810(_DWORD *a1)
{
  BOOL v1; // eax

  v1 = sub_46B4C0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_489830 @ 0x00489830..0x004898E7 =====
int __cdecl sub_489830(_DWORD *a1)
{
  size_t v1; // edi
  unsigned int v2; // eax
  const void *v3; // edx
  char Buffer[256]; // [esp+8h] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  if ( v2 >= 0x100000 )
  {
    sprintf(Buffer, &byte_4EBB5C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 + v1 > 0x100000 || !v1 || v1 > 0x100000 )
  {
    sprintf(Buffer, &byte_4EBB88, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  memcpy_0((char *)dword_566760 + v2, v3, v1);
  return 0;
}

// ===== sub_4898F0 @ 0x004898F0..0x004899AA =====
int __cdecl sub_4898F0(_DWORD *a1)
{
  size_t v1; // ebx
  unsigned int v2; // edi
  void *v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = (void *)sub_48DF50(a1);
  if ( v2 >= 0x100000 )
  {
    sprintf(Buffer, &byte_4EBB5C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v2 + v1 > 0x100000 || !v1 || v1 > 0x100000 )
  {
    sprintf(Buffer, &byte_4EBB88, v2, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  memcpy_0(v3, (char *)dword_566760 + v2, v1);
  return 0;
}

// ===== sub_4899B0 @ 0x004899B0..0x004899D3 =====
int __cdecl sub_4899B0(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  v1 = (void *)sub_48DF50(a1);
  v2 = sub_46B430(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_4899E0 @ 0x004899E0..0x00489A03 =====
int __cdecl sub_4899E0(_DWORD *a1)
{
  int v1; // eax
  BOOL v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_46B450(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_489A10 @ 0x00489A10..0x00489A40 =====
int __cdecl sub_489A10(_DWORD *a1)
{
  int v1; // esi
  const char *v2; // eax
  BOOL v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_46B460(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_489A40 @ 0x00489A40..0x00489ABB =====
int __cdecl sub_489A40(_DWORD *a1)
{
  int v1; // ebx
  unsigned int v2; // esi
  int v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_46B480(v1, v2, v3);
  if ( v4 )
  {
    if ( v4 == -2147483646 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v4 == -2147483645 )
    {
      sub_4450D0(a1, 2);
      return 0;
    }
  }
  else
  {
    v4 = 0;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_489AC0 @ 0x00489AC0..0x00489B72 =====
int __cdecl sub_489AC0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax
  unsigned int v4; // eax
  unsigned int v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = sub_4450B0(a1);
  v3 = sub_48DF50(a1);
  v4 = sub_46B490(v1, v2, v3, v6);
  if ( v4 > 0x80000003 )
  {
    if ( v4 == -2147483644 )
      v4 = 3;
  }
  else
  {
    switch ( v4 )
    {
      case 0x80000003:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000002:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_489B80 @ 0x00489B80..0x00489BF9 =====
int __cdecl sub_489B80(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // ebx
  _DWORD *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_46B4B0(v3, v1, v2);
  if ( v4 )
  {
    if ( v4 == -2147483646 )
    {
      sub_4450D0(a1, 1);
      return 0;
    }
    if ( v4 == -2147483645 )
    {
      sub_4450D0(a1, 2);
      return 0;
    }
  }
  else
  {
    v4 = 0;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_489C00 @ 0x00489C00..0x00489C18 =====
int __cdecl sub_489C00(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46BD60(v1);
  return 0;
}

// ===== sub_489C20 @ 0x00489C20..0x00489C38 =====
int __cdecl sub_489C20(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_46C090();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_489C40 @ 0x00489C40..0x00489DD4 =====
int __cdecl sub_489C40(_DWORD *a1)
{
  const char *v1; // edi
  const char *v2; // ebx
  int v3; // eax
  const char *v4; // edx
  int v6; // [esp+Ch] [ebp-128h]
  const char *v7; // [esp+10h] [ebp-124h]
  int v8; // [esp+14h] [ebp-120h]
  int v9; // [esp+18h] [ebp-11Ch]
  int v10; // [esp+1Ch] [ebp-118h]
  int v11; // [esp+20h] [ebp-114h]
  int v12; // [esp+24h] [ebp-110h]
  int v13; // [esp+28h] [ebp-10Ch]
  int v14; // [esp+2Ch] [ebp-108h]
  char Buffer[256]; // [esp+30h] [ebp-104h] BYREF

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v7 = (const char *)sub_48DF50(a1);
  sub_48DF50(a1);
  v6 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v11 = sub_4450B0(a1);
  v14 = sub_4450B0(a1);
  v13 = sub_4450B0(a1);
  v12 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v8 = sub_4450B0(a1);
  v3 = sub_4450B0(a1);
  switch ( sub_46BDB0(v4, v3, v8, v10, v12, v13, v14, v11, v9, v6, v7, v2, v1, 0) )
  {
    case -2147483647:
      sprintf(Buffer, &byte_4EBBCC, 31);
      break;
    case -2147483646:
      sprintf(Buffer, &byte_4EBBFC, 31);
      sub_4646F0(Buffer, (int)a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EBC28, 31);
      sub_4646F0(Buffer, (int)a1);
    case -2147483644:
      sprintf(Buffer, byte_4EBC50, 255);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, (int)a1);
}

// ===== sub_489DF0 @ 0x00489DF0..0x00489E68 =====
int __cdecl sub_489DF0(_DWORD *a1)
{
  unsigned int v1; // esi
  _DWORD *v2; // edi
  BOOL v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_46C0A0(v1, v2, 0);
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EBC7C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_489E70 @ 0x00489E70..0x00489F40 =====
int __cdecl sub_489E70(int a1)
{
  int v1; // eax
  const char *v3; // [esp-8h] [ebp-110h]
  int v4; // [esp-4h] [ebp-10Ch]
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_48DF50(a1);
  switch ( sub_46BFD0(v1) )
  {
    case -2147483647:
      v4 = 31;
      v3 = &byte_4EBCB0;
      goto LABEL_3;
    case -2147483646:
      sprintf(Buffer, &byte_4EBCD4, 31);
      sub_4646F0(Buffer, a1);
    case -2147483645:
      sprintf(Buffer, &byte_4EBCF8, 31);
      sub_4646F0(Buffer, a1);
    case -2147483644:
      v4 = 255;
      v3 = (const char *)&unk_4EBD14;
LABEL_3:
      sprintf(Buffer, v3, v4);
      break;
    case -2147483643:
      sprintf(Buffer, byte_4EBD38, 511);
      break;
    default:
      return 0;
  }
  sub_4646F0(Buffer, a1);
}

// ===== sub_489F60 @ 0x00489F60..0x00489FD8 =====
int __cdecl sub_489F60(_DWORD *a1)
{
  unsigned int v1; // esi
  _DWORD *v2; // edi
  BOOL v3; // eax
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_46C0A0(v1, v2, 1);
  if ( !v3 )
  {
    sprintf(Buffer, &byte_4EBC7C, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_489FE0 @ 0x00489FE0..0x0048A01B =====
int __cdecl sub_489FE0(_DWORD *a1)
{
  int v1; // edi
  int v2; // esi
  _DWORD *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_46C1D0(v1, v2, v3);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48A020 @ 0x0048A020..0x0048A045 =====
int __cdecl sub_48A020(_DWORD *a1)
{
  int v1; // eax
  int v2; // ecx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v3 = sub_46C220(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A050 @ 0x0048A050..0x0048A082 =====
int __cdecl sub_48A050(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_46C2A0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A090 @ 0x0048A090..0x0048A0C2 =====
int __cdecl sub_48A090(_DWORD *a1)
{
  void *v1; // edx
  int v2; // eax

  sub_48DF50(a1);
  sub_4450B0(a1);
  v2 = sub_46C2E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A0D0 @ 0x0048A0D0..0x0048A10D =====
int __cdecl sub_48A0D0(_DWORD *a1)
{
  int v1; // edi
  void *v2; // esi
  void *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_46C380(v2, v1, v3);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48A110 @ 0x0048A110..0x0048A14B =====
int __cdecl sub_48A110(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // eax

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  sub_4450B0(a1);
  v2 = sub_46C3E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A150 @ 0x0048A150..0x0048A177 =====
int __cdecl sub_48A150(_DWORD *a1)
{
  int v1; // eax

  sub_48DF50(a1);
  v1 = sub_496580();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48A180 @ 0x0048A180..0x0048A1A8 =====
int __cdecl sub_48A180(_DWORD *a1)
{
  _DWORD *v1; // edx
  int v2; // eax

  sub_4450B0(a1);
  v2 = sub_4450B0(v1);
  sub_496540(0, v2);
  return 0;
}

// ===== sub_48A1B0 @ 0x0048A1B0..0x0048A1F2 =====
int __cdecl sub_48A1B0(_DWORD *a1)
{
  int v1; // ebx
  int v2; // eax
  int v3; // ecx
  int v4; // eax
  BOOL v5; // esi

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_46C4D0(v3, v2);
  v5 = v4 != 0;
  if ( v4 )
    sub_41ADE0(v4, v1);
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_48A200 @ 0x0048A200..0x0048A242 =====
int __cdecl sub_48A200(_DWORD *a1)
{
  int *v1; // ebx
  int v2; // eax
  int v3; // ecx
  int v4; // eax
  BOOL v5; // esi

  v1 = (int *)sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_46C4D0(v3, v2);
  v5 = v4 != 0;
  if ( v4 )
    *v1 = sub_4477B0(v4);
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_48A250 @ 0x0048A250..0x0048A2A3 =====
int __cdecl sub_48A250(_DWORD *a1)
{
  void *v1; // edi
  int v2; // eax
  int v3; // ecx
  int v4; // eax
  BOOL v5; // esi
  int v7; // [esp+Ch] [ebp-4h]

  v1 = (void *)sub_48DF50(a1);
  v7 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v4 = sub_46C4D0(v3, v2);
  v5 = v4 != 0;
  if ( v4 )
    sub_4477E0(v4, v7, v1);
  sub_4450D0(a1, v5);
  return 0;
}

// ===== sub_48A2B0 @ 0x0048A2B0..0x0048A2D3 =====
int __cdecl sub_48A2B0(_DWORD *a1)
{
  unsigned int v1; // eax
  int v2; // eax
  _DWORD *v3; // edx

  v1 = sub_4450B0(a1);
  v2 = sub_46C470(v1);
  sub_4450D0(v3, v2);
  return 0;
}

// ===== sub_48A2E0 @ 0x0048A2E0..0x0048A30E =====
int __cdecl sub_48A2E0(_DWORD *a1)
{
  int v1; // esi
  char *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (char *)sub_48DF50(a1);
  v3 = sub_42D100(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A310 @ 0x0048A310..0x0048A333 =====
int __cdecl sub_48A310(_DWORD *a1)
{
  const char *v1; // eax
  int v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_42D1E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A340 @ 0x0048A340..0x0048A3C6 =====
int __cdecl sub_48A340(_DWORD *a1)
{
  unsigned int v1; // ebx
  const char *v2; // edi
  void *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = operator new(0x128u);
  if ( v3 )
    v4 = sub_439510(v2, (int)v3, (int)a1, v1);
  else
    v4 = 0;
  sub_4451C0((int)a1, v4);
  return 2;
}

// ===== sub_48A3D0 @ 0x0048A3D0..0x0048A3FF =====
int __cdecl sub_48A3D0(_DWORD *a1)
{
  int v1; // eax
  const char *v2; // ecx
  int v3; // eax

  sub_48DF50(a1);
  v1 = sub_42D560((int)a1);
  v3 = sub_42D330(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A400 @ 0x0048A400..0x0048A42D =====
int __cdecl sub_48A400(_DWORD *a1)
{
  unsigned int v1; // esi
  const char *v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_42D370(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A430 @ 0x0048A430..0x0048A4C4 =====
int __cdecl sub_48A430(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = operator new(0x24u);
  if ( v3 )
    v4 = sub_439280(v3, v2, v1);
  else
    v4 = 0;
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_48A4D0 @ 0x0048A4D0..0x0048A502 =====
int __cdecl sub_48A4D0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_4938F0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A510 @ 0x0048A510..0x0048A5B2 =====
int __cdecl sub_48A510(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax
  int v6; // [esp+18h] [ebp-10h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  sub_48DF50(a1);
  v6 = sub_48DF50(a1);
  v3 = operator new(0x24u);
  if ( v3 )
    v4 = sub_4393B0(v3, v6, v2, v1);
  else
    v4 = 0;
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_48A5C0 @ 0x0048A5C0..0x0048A631 =====
int __cdecl sub_48A5C0(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  char *v3; // esi
  char *v5; // [esp+Ch] [ebp-4h]

  v1 = sub_48DF50(a1);
  v5 = (char *)sub_48DF50(a1);
  v2 = 0;
  if ( sub_493940() )
  {
    v3 = (char *)operator new[](*(_DWORD *)(v1 + 24));
    sub_4938F0(v3);
    if ( sub_447330(v3, v5) )
      v2 = 0;
    else
      v2 = *((_DWORD *)v3 + 5);
    operator delete[](v3);
  }
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A640 @ 0x0048A640..0x0048A6D4 =====
int __cdecl sub_48A640(_DWORD *a1)
{
  int v1; // edi
  int v2; // ebx
  _DWORD *v3; // eax
  _DWORD *v4; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = operator new(0x28u);
  if ( v3 )
    v4 = sub_450910(a1, v3, v2, v1);
  else
    v4 = 0;
  sub_4451C0((int)a1, (int)v4);
  return 2;
}

// ===== sub_48A6E0 @ 0x0048A6E0..0x0048A712 =====
int __cdecl sub_48A6E0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_4960A0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A720 @ 0x0048A720..0x0048A745 =====
int __cdecl sub_48A720(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_496150();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48A750 @ 0x0048A750..0x0048A78B =====
int __cdecl sub_48A750(_DWORD *a1)
{
  void *v1; // esi
  int v2; // eax

  v1 = (void *)sub_48DF50(a1);
  sub_48DF50(a1);
  sub_4450B0(a1);
  v2 = sub_4961C0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A790 @ 0x0048A790..0x0048A7C0 =====
int __cdecl sub_48A790(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v2 = sub_4961F0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A7C0 @ 0x0048A7C0..0x0048A80E =====
int __cdecl sub_48A7C0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  sub_4450B0(a1);
  sub_48DF50(a1);
  v3 = sub_496220(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A810 @ 0x0048A810..0x0048A81F =====
int sub_48A810()
{
  sub_4954A0();
  return 0;
}

// ===== sub_48A820 @ 0x0048A820..0x0048A845 =====
int __cdecl sub_48A820(_DWORD *a1)
{
  int v1; // eax

  sub_4450B0(a1);
  v1 = sub_4956C0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48A850 @ 0x0048A850..0x0048A88C =====
int __cdecl sub_48A850(_DWORD *a1)
{
  int v1; // eax
  void *v2; // edx
  int v3; // eax

  sub_48DF50(a1);
  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_495550(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48A890 @ 0x0048A890..0x0048A8C2 =====
int __cdecl sub_48A890(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  v1 = (void *)sub_48DF50(a1);
  v2 = sub_495640(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A8D0 @ 0x0048A8D0..0x0048A900 =====
int __cdecl sub_48A8D0(_DWORD *a1)
{
  void *v1; // edx
  int v2; // eax

  sub_48DF50(a1);
  sub_4450B0(a1);
  v2 = sub_4956E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A900 @ 0x0048A900..0x0048A940 =====
int __cdecl sub_48A900(_DWORD *a1)
{
  void *v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = (void *)sub_48DF50(a1);
  v2 = sub_495850(v1, 0);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A940 @ 0x0048A940..0x0048A97F =====
int __cdecl sub_48A940(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  sub_4450B0(a1);
  sub_4450B0(a1);
  v1 = sub_48DF50(a1);
  v2 = sub_495850(0, v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48A980 @ 0x0048A980..0x0048A9DB =====
int __cdecl sub_48A980(_DWORD *a1)
{
  int v1; // esi
  const CHAR *v2; // ebx
  const char *v3; // eax
  int v4; // eax
  const char *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v6 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_4724B0(0, v2, 0, v3, v6, 1, v1, 1, 1, 0);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48A9E0 @ 0x0048A9E0..0x0048AADE =====
int __cdecl sub_48A9E0(int a1)
{
  const char *v1; // edi
  const char *v2; // eax
  const char *v3; // esi
  const char *v4; // ecx
  int v5; // edx
  char v6; // al
  const char *v7; // ecx
  int v8; // edx
  char v9; // al
  const char *v10; // ecx
  int v11; // edx
  char v12; // al
  const char *v14; // [esp+Ch] [ebp-4h]

  v14 = (const char *)sub_48DF50(a1);
  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = v2;
  if ( v2 )
  {
    dword_566774 = operator new[](strlen(v2) + 1);
    v4 = v3;
    v5 = (_BYTE *)dword_566774 - v3;
    do
    {
      v6 = *v4;
      v4[v5] = *v4;
      ++v4;
    }
    while ( v6 );
  }
  else
  {
    dword_566774 = 0;
  }
  if ( !v1 )
    sub_4646F0(byte_4EBD60, a1);
  dword_566778 = operator new[](strlen(v1) + 1);
  v7 = v1;
  v8 = (_BYTE *)dword_566778 - v1;
  do
  {
    v9 = *v7;
    v7[v8] = *v7;
    ++v7;
  }
  while ( v9 );
  if ( v14 )
  {
    dword_56677C = operator new[](strlen(v14) + 1);
    v10 = v14;
    v11 = (_BYTE *)dword_56677C - v14;
    do
    {
      v12 = *v10;
      v10[v11] = *v10;
      ++v10;
    }
    while ( v12 );
  }
  else
  {
    dword_56677C = 0;
  }
  DestroyWindow(hWndParent);
  return 6;
}

// ===== sub_48AAE0 @ 0x0048AAE0..0x0048AB2C =====
int __cdecl sub_48AAE0(_DWORD *a1)
{
  const CHAR *v1; // esi
  const char *v2; // ebx
  const char *v3; // eax
  int v4; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_4724B0(0, v1, 0, v3, v2, 1, 1, 1, 0, 1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48AB30 @ 0x0048AB30..0x0048AB53 =====
int __cdecl sub_48AB30(_DWORD *a1)
{
  const CHAR *v1; // eax
  BOOL v2; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = sub_472880(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48AB60 @ 0x0048AB60..0x0048AB89 =====
int __cdecl sub_48AB60(int a1)
{
  _DWORD *v1; // eax

  v1 = (_DWORD *)sub_48DF50(a1);
  *v1 = 1970889044;
  v1[1] = 1634558324;
  v1[2] = &unk_565432;
  return 0;
}

// ===== sub_48AB90 @ 0x0048AB90..0x0048AC22 =====
int __cdecl sub_48AB90(_DWORD *a1)
{
  char *v1; // esi
  _DWORD *v2; // ebx
  int v3; // eax
  char Buffer[780]; // [esp+Ch] [ebp-310h] BYREF

  v1 = (char *)sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  if ( sub_464980(v1) )
    strcpy(Buffer, v1);
  else
    sprintf(Buffer, "%s%s", &::Buffer, v1);
  v3 = sub_401980(v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48AC30 @ 0x0048AC30..0x0048AC44 =====
int __cdecl sub_48AC30(int a1)
{
  const char *v1; // eax

  v1 = (const char *)sub_48DF50(a1);
  sub_472490(v1);
  return 0;
}

// ===== sub_48AC50 @ 0x0048AC50..0x0048ACD7 =====
int __cdecl sub_48AC50(_DWORD *a1)
{
  const CHAR *v1; // esi
  int v2; // ebx
  char *v3; // eax
  INT_PTR v4; // eax
  char *v6; // [esp+Ch] [ebp-14h]
  _DWORD *v7; // [esp+10h] [ebp-10h]
  _DWORD *v8; // [esp+14h] [ebp-Ch]
  WPARAM v9; // [esp+18h] [ebp-8h]
  WPARAM v10; // [esp+1Ch] [ebp-4h]

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v10 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v6 = (char *)sub_48DF50(a1);
  v8 = (_DWORD *)sub_48DF50(a1);
  v7 = (_DWORD *)sub_48DF50(a1);
  v3 = (char *)sub_48DF50(a1);
  v4 = sub_471FB0(v6, v3, v7, v8, v9, v10, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48ACE0 @ 0x0048ACE0..0x0048AD4C =====
int __cdecl sub_48ACE0(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  const CHAR *v3; // eax
  INT_PTR v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-Ch]
  const CHAR *v7; // [esp+10h] [ebp-8h]
  const CHAR *v8; // [esp+14h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v8 = (const CHAR *)sub_48DF50(a1);
  v7 = (const CHAR *)sub_48DF50(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v3 = (const CHAR *)sub_48DF50(a1);
  v4 = sub_472050(v6, v3, v7, v8, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48AD50 @ 0x0048AD50..0x0048AF26 =====
int __cdecl sub_48AD50(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // ebx
  int v3; // eax
  int v4; // eax
  _DWORD *v5; // ebx
  int v6; // eax
  const char *v8; // [esp+Ch] [ebp-38h]
  char *v9; // [esp+10h] [ebp-34h]
  const char *v10; // [esp+14h] [ebp-30h]
  int v11; // [esp+18h] [ebp-2Ch]
  int v12; // [esp+1Ch] [ebp-28h]
  char *v13; // [esp+20h] [ebp-24h]
  const char *v14; // [esp+24h] [ebp-20h]
  const CHAR *v15; // [esp+28h] [ebp-1Ch]
  int v16; // [esp+2Ch] [ebp-18h]
  void *v17; // [esp+38h] [ebp-Ch]
  void *v18; // [esp+38h] [ebp-Ch]
  void *v19; // [esp+38h] [ebp-Ch]
  _DWORD *v20; // [esp+3Ch] [ebp-8h]
  void *v21; // [esp+3Ch] [ebp-8h]
  const char **v22; // [esp+40h] [ebp-4h]

  v16 = sub_4450B0(a1);
  v15 = (const CHAR *)sub_48DF50(a1);
  v14 = (const char *)sub_48DF50(a1);
  v13 = (char *)sub_48DF50(a1);
  v9 = (char *)sub_48DF50(a1);
  v8 = (const char *)sub_48DF50(a1);
  v12 = sub_4450B0(a1);
  sub_48DF50(a1);
  sub_48DF50(a1);
  v11 = sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v20 = (_DWORD *)sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v10 = (const char *)sub_48DF50(a1);
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
    v17 = (void *)(v3 + 1);
    v22 = (const char **)operator new[](4 * (v3 + 1));
    sub_48DF70(v22, a1, v17);
  }
  v4 = 0;
  if ( *v20 )
  {
    do
      ++v4;
    while ( v20[v4] );
  }
  v18 = (void *)(v4 + 1);
  v5 = operator new[](4 * (v4 + 1));
  sub_48DF70(v5, a1, v18);
  v19 = operator new[](4 * v1);
  sub_48DF70(v19, a1, v1);
  v21 = operator new[](4 * v1);
  sub_48DF70(v21, a1, v1);
  v6 = sub_472150(v8, v9, v10, v22, v5, v1, v11, (int)v19, (int)v21, v12, v13, v14, v15, v16);
  sub_4450D0(a1, v6);
  operator delete[](v21);
  operator delete[](v19);
  operator delete[](v5);
  if ( v22 )
    operator delete[](v22);
  return 0;
}

// ===== sub_48AF30 @ 0x0048AF30..0x0048AFB6 =====
int __cdecl sub_48AF30(_DWORD *a1)
{
  int v1; // esi
  int v2; // ebx
  const char *v3; // eax
  int v4; // eax
  const char *v6; // [esp+Ch] [ebp-14h]
  const char *v7; // [esp+10h] [ebp-10h]
  const char *v8; // [esp+14h] [ebp-Ch]
  const char *v9; // [esp+18h] [ebp-8h]
  const char *v10; // [esp+1Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v7 = (const char *)sub_48DF50(a1);
  v6 = (const char *)sub_48DF50(a1);
  v10 = (const char *)sub_48DF50(a1);
  v9 = (const char *)sub_48DF50(a1);
  v8 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_471530(v6, v7, v3, v8, v9, v10, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48AFC0 @ 0x0048AFC0..0x0048B03B =====
int __cdecl sub_48AFC0(_DWORD *a1)
{
  _DWORD *v1; // edi
  int v2; // esi
  int v3; // esi
  const char **v4; // ebx
  int v5; // eax
  const char *v7; // [esp+Ch] [ebp-4h]

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = 0;
  v7 = (const char *)sub_48DF50(a1);
  if ( *v1 )
  {
    do
      ++v2;
    while ( v1[v2] );
  }
  v3 = v2 + 1;
  v4 = (const char **)operator new[](4 * v3);
  sub_48DF70(v4, a1, v3);
  v5 = sub_471C10(v7, v4);
  sub_4450D0(a1, v5);
  operator delete[](v4);
  return 0;
}

// ===== sub_48B040 @ 0x0048B040..0x0048B0BB =====
int __cdecl sub_48B040(_DWORD *a1)
{
  _DWORD *v1; // edi
  int v2; // esi
  int v3; // esi
  const char **v4; // ebx
  BOOL v5; // eax
  int v7; // [esp+Ch] [ebp-4h]

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = 0;
  v7 = sub_48DF50(a1);
  if ( *v1 )
  {
    do
      ++v2;
    while ( v1[v2] );
  }
  v3 = v2 + 1;
  v4 = (const char **)operator new[](4 * v3);
  sub_48DF70(v4, a1, v3);
  v5 = sub_471E10(v7, v4);
  sub_4450D0(a1, v5);
  operator delete[](v4);
  return 0;
}

// ===== sub_48B0C0 @ 0x0048B0C0..0x0048B105 =====
int __cdecl sub_48B0C0(_DWORD *a1)
{
  int v1; // ebx
  const char *v2; // esi
  const char *v3; // eax
  const char *v5; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = (const char *)sub_48DF50(a1);
  v5 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  sub_4718D0(v3, v2, v5, v1);
  return 0;
}

// ===== sub_48B110 @ 0x0048B110..0x0048B150 =====
int __cdecl sub_48B110(_DWORD *a1)
{
  int v1; // ebx
  const char *v2; // esi
  const char *v3; // eax
  BOOL v4; // eax

  v1 = sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_4713F0(v3, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48B150 @ 0x0048B150..0x0048B168 =====
int __cdecl sub_48B150(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4892D0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48B170 @ 0x0048B170..0x0048B1AF =====
int __cdecl sub_48B170(_DWORD *a1)
{
  const char *v1; // esi
  const char *v2; // edi
  BYTE *v3; // eax
  int v4; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = (BYTE *)sub_48DF50(a1);
  v4 = sub_4719D0(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48B1B0 @ 0x0048B1B0..0x0048B1E0 =====
int __cdecl sub_48B1B0(_DWORD *a1)
{
  const char *v1; // esi
  const char *v2; // eax
  BOOL v3; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (const char *)sub_48DF50(a1);
  v3 = sub_471BA0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B1E0 @ 0x0048B1E0..0x0048B212 =====
int __cdecl sub_48B1E0(_DWORD *a1)
{
  const char *v1; // esi
  char *v2; // eax
  int v3; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (char *)sub_48DF50(a1);
  v3 = sub_471AA0(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B220 @ 0x0048B220..0x0048B23A =====
int __cdecl sub_48B220(int a1)
{
  BYTE *v1; // eax

  v1 = (BYTE *)sub_48DF50(a1);
  sub_467570(v1);
  return 0;
}

// ===== sub_48B240 @ 0x0048B240..0x0048B2A0 =====
int __cdecl sub_48B240(_DWORD *a1)
{
  const BYTE *v1; // esi
  const BYTE *v2; // ebx
  const char *v3; // eax
  int v4; // eax
  BYTE *lpData; // [esp+Ch] [ebp-8h]
  BYTE *v7; // [esp+10h] [ebp-4h]

  v1 = (const BYTE *)sub_48DF50(a1);
  v2 = (const BYTE *)sub_48DF50(a1);
  v7 = (BYTE *)sub_48DF50(a1);
  lpData = (BYTE *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_4728B0(v2, v1, v3, lpData, v7);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48B2A0 @ 0x0048B2A0..0x0048B2B8 =====
int __cdecl sub_48B2A0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4728A0();
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48B2C0 @ 0x0048B2C0..0x0048B322 =====
int __cdecl sub_48B2C0(_DWORD *a1)
{
  unsigned __int8 v1; // al
  int (__cdecl *v2)(int); // ecx
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v1 = sub_445030(a1);
  v2 = funcs_48B30E[v1];
  if ( !v2 )
  {
    sprintf(Buffer, &byte_4EBDA8, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return v2((int)a1);
}

// ===== sub_48B330 @ 0x0048B330..0x0048B355 =====
int __cdecl sub_48B330(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_498820(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48B360 @ 0x0048B360..0x0048B390 =====
int __cdecl sub_48B360(_DWORD *a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_48E590(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B390 @ 0x0048B390..0x0048B3B4 =====
int __cdecl sub_48B390(int a1)
{
  CHAR *v1; // eax
  DWORD pcbBuffer; // [esp+0h] [ebp-4h] BYREF

  v1 = (CHAR *)sub_48DF50(a1);
  pcbBuffer = 257;
  GetUserNameA(v1, &pcbBuffer);
  return 0;
}

// ===== sub_48B3C0 @ 0x0048B3C0..0x0048B3E4 =====
int __cdecl sub_48B3C0(int a1)
{
  CHAR *v1; // eax
  DWORD nSize; // [esp+0h] [ebp-4h] BYREF

  v1 = (CHAR *)sub_48DF50(a1);
  nSize = 16;
  GetComputerNameA(v1, &nSize);
  return 0;
}

// ===== sub_48B3F0 @ 0x0048B3F0..0x0048B415 =====
int __cdecl sub_48B3F0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_48DF50(a1);
  v2 = sub_46F720(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48B420 @ 0x0048B420..0x0048B44A =====
int __cdecl sub_48B420(int a1)
{
  _DWORD *v1; // esi
  int v2; // eax

  sub_48DF50(a1);
  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  sub_45E490(v2, v1);
  return 0;
}

// ===== sub_48B450 @ 0x0048B450..0x0048B4DA =====
int __cdecl sub_48B450(int a1)
{
  char *v1; // edi
  DWORD *v2; // esi
  DWORD dwBuildNumber; // eax
  DWORD dwMinorVersion; // edx
  DWORD dwPlatformId; // ecx
  struct _OSVERSIONINFOA v7; // [esp+8h] [ebp-98h] BYREF

  v1 = (char *)sub_48DF50(a1);
  v2 = (DWORD *)sub_48DF50(a1);
  sub_46F800(&v7);
  dwBuildNumber = v7.dwBuildNumber;
  dwMinorVersion = v7.dwMinorVersion;
  *v2 = v7.dwMajorVersion;
  dwPlatformId = v7.dwPlatformId;
  v2[1] = dwMinorVersion;
  v2[2] = dwBuildNumber;
  v2[3] = dwPlatformId;
  strcpy(v1, v7.szCSDVersion);
  return 0;
}

// ===== sub_48B4E0 @ 0x0048B4E0..0x0048B564 =====
int __cdecl sub_48B4E0(int a1)
{
  _DWORD *v1; // edi
  _DWORD *v2; // esi
  _MEMORYSTATUSEX Buffer; // [esp+8h] [ebp-48h] BYREF

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  memset(&Buffer, 0, sizeof(Buffer));
  Buffer.dwLength = 64;
  GlobalMemoryStatusEx(&Buffer);
  *v2 = Buffer.ullTotalPhys >> 20;
  *v1 = Buffer.ullAvailPhys >> 20;
  return 0;
}

// ===== sub_48B570 @ 0x0048B570..0x0048B58D =====
int __cdecl sub_48B570(int a1)
{
  unsigned int *v1; // eax

  v1 = (unsigned int *)sub_48DF50(a1);
  sub_45E580(v1, 1);
  return 0;
}

// ===== sub_48B590 @ 0x0048B590..0x0048B5AF =====
int __cdecl sub_48B590(_DWORD *a1)
{
  BOOL v1; // eax

  v1 = IsIconic(hWndParent);
  sub_4450D0(a1, v1);
  return 0;
}

// ===== sub_48B5B0 @ 0x0048B5B0..0x0048B5E2 =====
int __cdecl sub_48B5B0(_DWORD *a1)
{
  int v1; // eax
  int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_46DA40(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B5F0 @ 0x0048B5F0..0x0048B606 =====
int __cdecl sub_48B5F0(int a1)
{
  BYTE *v1; // eax

  v1 = (BYTE *)sub_48DF50(a1);
  GetKeyboardState(v1);
  return 0;
}

// ===== sub_48B610 @ 0x0048B610..0x0048B624 =====
int __cdecl sub_48B610(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46D540(v1);
  return 0;
}

// ===== sub_48B630 @ 0x0048B630..0x0048B662 =====
int __cdecl sub_48B630(_DWORD *a1)
{
  unsigned int v1; // eax
  unsigned int v2; // edx
  int v3; // eax

  sub_4450B0(a1);
  v1 = sub_4450B0(a1);
  v3 = sub_46B1C0(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B670 @ 0x0048B670..0x0048B6BF =====
int __cdecl sub_48B670(_DWORD *a1)
{
  unsigned int v1; // esi
  int v2; // eax
  unsigned int v3; // eax
  int v5; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  sub_4450B0(a1);
  v5 = sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_46B300(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B6C0 @ 0x0048B6C0..0x0048B6E7 =====
int __cdecl sub_48B6C0(_DWORD *a1)
{
  int v1; // eax
  BOOL v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46B100(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48B6F0 @ 0x0048B6F0..0x0048B715 =====
int __cdecl sub_48B6F0(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v2; // eax

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_46B140(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48B720 @ 0x0048B720..0x0048B755 =====
int __cdecl sub_48B720(_DWORD *a1)
{
  int v1; // esi
  _DWORD *v2; // edx
  int v3; // eax
  _DWORD *v4; // edx

  v1 = sub_4450B0(a1);
  sub_4450B0(v2);
  v3 = sub_460E40(v1);
  sub_4450D0(v4, v3 == 0);
  return 0;
}

// ===== sub_48B760 @ 0x0048B760..0x0048B797 =====
int __cdecl sub_48B760(_DWORD *a1)
{
  _DWORD *v1; // esi
  int v2; // eax

  v1 = (_DWORD *)sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = sub_460E60(v1);
  sub_4450D0(a1, v2 == 0);
  return 0;
}

// ===== sub_48B7A0 @ 0x0048B7A0..0x0048B7C7 =====
int __cdecl sub_48B7A0(_DWORD *a1)
{
  int v1; // eax
  int v2; // eax

  v1 = sub_4450B0(a1);
  v2 = sub_46E5D0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48B7D0 @ 0x0048B7D0..0x0048B7E4 =====
int __cdecl sub_48B7D0(_DWORD *a1)
{
  int v1; // eax

  v1 = sub_4450B0(a1);
  sub_46E5A0(v1);
  return 0;
}

// ===== sub_48B7F0 @ 0x0048B7F0..0x0048B88A =====
int __cdecl sub_48B7F0(_DWORD *a1)
{
  unsigned int v1; // esi
  _DWORD *v2; // eax
  unsigned int v3; // eax

  v1 = sub_4450B0(a1);
  sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_4011B0(v2, v1);
  if ( v3 > 0x80000002 )
  {
    if ( v3 == -2147483645 )
      v3 = 3;
  }
  else
  {
    switch ( v3 )
    {
      case 0x80000002:
        sub_4450D0(a1, 2);
        return 0;
      case 0u:
        sub_4450D0(a1, 0);
        return 0;
      case 0x80000001:
        sub_4450D0(a1, 1);
        return 0;
    }
  }
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B890 @ 0x0048B890..0x0048B8E4 =====
int __cdecl sub_48B890(_DWORD *a1)
{
  void *v1; // esi
  _DWORD *v2; // eax
  int v3; // eax

  v1 = (void *)sub_4450B0(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_401490(v1, v2);
  if ( v3 )
  {
    if ( v3 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v3 = 0;
  }
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48B8F0 @ 0x0048B8F0..0x0048B967 =====
int __cdecl sub_48B8F0(_DWORD *a1)
{
  int v1; // esi
  int v2; // edi
  _DWORD *v3; // eax
  int v4; // eax
  void *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_48DF50(a1);
  v6 = (void *)sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_4014A0(v1, v2, v3, v6);
  if ( v4 )
  {
    if ( v4 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v4 = 0;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48B970 @ 0x0048B970..0x0048B9D1 =====
int __cdecl sub_48B970(_DWORD *a1)
{
  int v1; // esi
  void *v2; // edi
  _DWORD *v3; // eax
  int v4; // eax

  v1 = sub_4450B0(a1);
  v2 = (void *)sub_4450B0(a1);
  v3 = (_DWORD *)sub_48DF50(a1);
  v4 = sub_4014C0(v1, v2, v3);
  if ( v4 )
  {
    if ( v4 == -2147483644 )
    {
      sub_4450D0(a1, 4);
      return 0;
    }
  }
  else
  {
    v4 = 0;
  }
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48B9E0 @ 0x0048B9E0..0x0048BA2E =====
int __cdecl sub_48B9E0(_DWORD *a1)
{
  const CHAR *v1; // esi
  struct _SYSTEMTIME *v2; // ebx
  struct _SYSTEMTIME *v3; // eax
  int v4; // eax
  struct _SYSTEMTIME *v6; // [esp+Ch] [ebp-4h]

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (struct _SYSTEMTIME *)sub_48DF50(a1);
  v6 = (struct _SYSTEMTIME *)sub_48DF50(a1);
  v3 = (struct _SYSTEMTIME *)sub_48DF50(a1);
  v4 = sub_4687A0(v3, v6, v2, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48BA30 @ 0x0048BA30..0x0048BA7F =====
int __cdecl sub_48BA30(_DWORD *a1)
{
  SYSTEMTIME *v1; // esi
  SYSTEMTIME *v2; // ebx
  BOOL v3; // eax
  SYSTEMTIME *lpSystemTime; // [esp+Ch] [ebp-4h]

  v1 = (SYSTEMTIME *)sub_48DF50(a1);
  v2 = (SYSTEMTIME *)sub_48DF50(a1);
  lpSystemTime = (SYSTEMTIME *)sub_48DF50(a1);
  sub_48DF50(a1);
  v3 = sub_468850(lpSystemTime, v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48BA80 @ 0x0048BA80..0x0048BAA5 =====
int __cdecl sub_48BA80(_DWORD *a1)
{
  const CHAR *v1; // eax
  int v2; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = sub_4677E0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48BAB0 @ 0x0048BAB0..0x0048BB9F =====
int __cdecl sub_48BAB0(_DWORD *a1)
{
  DWORD v1; // edi
  int v2; // esi
  void *v3; // ecx
  _DWORD *v4; // eax
  _DWORD *v5; // eax
  int v6; // eax
  int v8; // [esp+14h] [ebp-1Ch]
  int v9; // [esp+18h] [ebp-18h]
  const char *v10; // [esp+1Ch] [ebp-14h]
  CHAR *v11; // [esp+20h] [ebp-10h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v11 = (CHAR *)sub_48DF50(a1);
  v10 = (const char *)sub_48DF50(a1);
  v9 = sub_48DF50(a1);
  v8 = sub_48D1A0(v9);
  if ( v8 )
  {
    v4 = operator new(0x654u);
    if ( v4 )
      v5 = sub_4523F0((int)v10, v2, v4, (int)a1, v9, (int)v11);
    else
      v5 = 0;
    sub_4451C0((int)a1, (int)v5);
    sub_48D190();
  }
  else
  {
    v6 = sub_467F50(v10, 0, v3, v11, v2, v1);
    sub_4450D0(a1, v6);
  }
  return v8 != 0 ? 2 : 0;
}

// ===== sub_48BBA0 @ 0x0048BBA0..0x0048BBEF =====
int __cdecl sub_48BBA0(_DWORD *a1)
{
  DWORD v1; // esi
  LONG v2; // ebx
  DWORD *v3; // eax
  int v4; // eax
  const CHAR *v6; // [esp+Ch] [ebp-4h]

  v1 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v6 = (const CHAR *)sub_48DF50(a1);
  v3 = (DWORD *)sub_48DF50(a1);
  v4 = sub_468420(v2, v3, v6, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48BBF0 @ 0x0048BBF0..0x0048BC3E =====
int __cdecl sub_48BBF0(_DWORD *a1)
{
  DWORD *v1; // esi
  CHAR *v2; // ebx
  void *v3; // eax
  int v4; // eax
  size_t *v6; // [esp+Ch] [ebp-4h]

  v1 = (DWORD *)sub_4450B0(a1);
  v2 = (CHAR *)sub_48DF50(a1);
  v6 = (size_t *)sub_48DF50(a1);
  v3 = (void *)sub_48DF50(a1);
  v4 = sub_468140(v6, v2, v3, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48BC40 @ 0x0048BC40..0x0048BC72 =====
int __cdecl sub_48BC40(_DWORD *a1)
{
  const char *v1; // eax
  unsigned int v2; // eax

  sub_48DF50(a1);
  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_468310(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48BC80 @ 0x0048BC80..0x0048BCA3 =====
int __cdecl sub_48BC80(_DWORD *a1)
{
  _DWORD *v1; // eax
  int v2; // eax

  v1 = (_DWORD *)sub_48DF50(a1);
  v2 = sub_464B00(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48BCB0 @ 0x0048BCB0..0x0048BCE4 =====
int __cdecl sub_48BCB0(_DWORD *a1)
{
  const char *v1; // esi
  _DWORD *v2; // eax
  int v3; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = (_DWORD *)sub_48DF50(a1);
  v3 = sub_4686D0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48BCF0 @ 0x0048BCF0..0x0048BDCF =====
int __cdecl sub_48BCF0(_DWORD *a1)
{
  int v1; // esi
  const char **v2; // ebx
  int v3; // eax
  const CHAR *v5; // [esp+Ch] [ebp-1Ch]
  void *v6; // [esp+10h] [ebp-18h]
  const CHAR *v7; // [esp+14h] [ebp-14h]
  int v8; // [esp+18h] [ebp-10h]
  void *v9; // [esp+24h] [ebp-4h]

  v8 = sub_4450B0(a1);
  v7 = (const CHAR *)sub_48DF50(a1);
  v5 = (const CHAR *)sub_48DF50(a1);
  sub_48DF50(a1);
  sub_48DF50(a1);
  v1 = sub_4450B0(a1);
  v6 = (void *)sub_48DF50(a1);
  v2 = (const char **)operator new[](4 * v1);
  v9 = operator new[](4 * v1);
  sub_48DF70(v2, a1, v1);
  sub_48DF70(v9, a1, v1);
  v3 = sub_466920((int)v9, v5, v6, v1, v2, v7, v8);
  sub_4450D0(a1, v3);
  operator delete[](v2);
  operator delete[](v9);
  return 0;
}

// ===== sub_48BDD0 @ 0x0048BDD0..0x0048BE0F =====
int __cdecl sub_48BDD0(_DWORD *a1)
{
  _BYTE *v1; // esi
  unsigned int *v2; // ebx
  const char *v3; // eax
  int v4; // eax

  v1 = (_BYTE *)sub_48DF50(a1);
  v2 = (unsigned int *)sub_48DF50(a1);
  v3 = (const char *)sub_48DF50(a1);
  v4 = sub_467A60(v1, v3, v2);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48BE10 @ 0x0048BE10..0x0048BE54 =====
int __cdecl sub_48BE10(_DWORD *a1)
{
  LPARAM v1; // esi
  const CHAR *v2; // ebx
  CHAR *v3; // eax
  BOOL v4; // eax

  v1 = sub_48DF50(a1);
  v2 = (const CHAR *)sub_48DF50(a1);
  v3 = (CHAR *)sub_48DF50(a1);
  v4 = sub_46F9C0(v2, hWndParent, v3, v1);
  sub_4450D0(a1, v4);
  return 0;
}

// ===== sub_48BE60 @ 0x0048BE60..0x0048BEAF =====
int __cdecl sub_48BE60(_DWORD *a1)
{
  const CHAR *v1; // ebx
  LPARAM v2; // eax
  int v3; // eax
  const CHAR *v5; // [esp+Ch] [ebp-4h]

  sub_48DF50(a1);
  v1 = (const CHAR *)sub_48DF50(a1);
  v5 = (const CHAR *)sub_48DF50(a1);
  v2 = sub_48DF50(a1);
  v3 = sub_467920(v2, v5, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48BEB0 @ 0x0048BEB0..0x0048BED7 =====
int __cdecl sub_48BEB0(_DWORD *a1)
{
  const char *v1; // eax
  BOOL v2; // eax

  v1 = (const char *)sub_48DF50(a1);
  v2 = sub_4685D0(v1);
  sub_4450D0(a1, v2);
  return 0;
}

// ===== sub_48BEE0 @ 0x0048BEE0..0x0048BF12 =====
int __cdecl sub_48BEE0(_DWORD *a1)
{
  char *v1; // esi
  CHAR *v2; // eax
  BOOL v3; // eax

  v1 = (char *)sub_48DF50(a1);
  v2 = (CHAR *)sub_48DF50(a1);
  v3 = sub_468670(v1, v2);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48BF20 @ 0x0048BF20..0x0048BF51 =====
int __cdecl sub_48BF20(_DWORD *a1)
{
  const CHAR *v1; // esi
  BOOL *v2; // eax
  int v3; // eax

  v1 = (const CHAR *)sub_48DF50(a1);
  v2 = (BOOL *)sub_48DF50(a1);
  v3 = sub_4648F0(v2, v1);
  sub_4450D0(a1, v3);
  return 0;
}

// ===== sub_48BF60 @ 0x0048BF60..0x0048C0C8 =====
int __cdecl sub_48BF60(_DWORD *a1)
{
  unsigned int v1; // ebx
  int v2; // esi
  _DWORD *v3; // eax
  _DWORD *v4; // edx
  _DWORD *v5; // esi
  int v6; // eax
  int v7; // eax
  int v9; // [esp+14h] [ebp-118h]
  int v10; // [esp+18h] [ebp-114h]
  char Buffer[256]; // [esp+1Ch] [ebp-110h] BYREF
  int v12; // [esp+128h] [ebp-4h]

  v1 = sub_4450B0(a1);
  v9 = sub_4450B0(a1);
  v10 = sub_4450B0(a1);
  v2 = sub_4450B0(a1);
  v12 = 0;
  if ( operator new(0x90u) )
  {
    v3 = sub_444A60(a1);
    v5 = sub_452B10(v2, (int)v4, v4, (int)v3);
  }
  else
  {
    v5 = 0;
  }
  v12 = -1;
  v6 = sub_452C20((int)a1, v5, v10, v9, v1);
  if ( v6 == -2147483646 )
  {
    sprintf(Buffer, &byte_4EBDD8, v10);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v6 == -2147483645 )
  {
    sprintf(Buffer, &byte_4EBE28, v9);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( v1 >= sub_444C30(v5) )
  {
    sprintf(Buffer, &byte_4EBE78, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  v7 = sub_42D560((int)v5);
  sub_4450D0(a1, v7);
  return 0;
}
