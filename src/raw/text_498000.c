#include "../include/ida_prelude.h"
#include "../include/globals.h"

// ===== ?__uncaught_exception@@YA_NXZ_2 @ 0x004980F0..0x004980FC =====
BOOL __cdecl __uncaught_exception()
{
  return dword_566A1C != 0;
}

// ===== sub_498100 @ 0x00498100..0x00498170 =====
void __usercall sub_498100(_DWORD *a1@<edi>, int a2, int a3, int a4, double a5, double a6)
{
  double *v6; // esi
  _DWORD *v7; // eax
  _DWORD *v8; // ecx

  *a1 = -1;
  v6 = (double *)operator new(0x28u);
  v6[2] = a5;
  v6[3] = a6;
  *(_DWORD *)v6 = a1;
  *((_DWORD *)v6 + 1) = a2;
  *((_DWORD *)v6 + 2) = a3;
  *((_DWORD *)v6 + 3) = a4;
  *((_DWORD *)v6 + 8) = 0;
  EnterCriticalSection(&stru_560FA0);
  v7 = dword_566A40;
  v8 = &unk_566A20;
  if ( dword_566A40 )
  {
    do
    {
      v8 = v7;
      v7 = (_DWORD *)v7[8];
    }
    while ( v7 );
  }
  v8[8] = v6;
  LeaveCriticalSection(&stru_560FA0);
}

// ===== sub_498170 @ 0x00498170..0x00498199 =====
BOOL sub_498170()
{
  BOOL v0; // esi
  void *v2; // [esp-4h] [ebp-8h]

  v0 = dword_566A40 != 0;
  if ( dword_566A40 )
  {
    v2 = dword_566A40;
    dword_566A40 = (void *)*((_DWORD *)dword_566A40 + 8);
    operator delete(v2);
  }
  return v0;
}

// ===== sub_4981A0 @ 0x004981A0..0x004982C8 =====
BOOL sub_4981A0()
{
  void *v0; // esi
  _DWORD v2[7]; // [esp+10h] [ebp-28h] BYREF
  int v3; // [esp+34h] [ebp-4h]

  v2[6] = v2;
  EnterCriticalSection(&stru_560FA0);
  v0 = dword_566A40;
  v2[5] = dword_566A40;
  LeaveCriticalSection(&stru_560FA0);
  if ( v0 )
  {
    v3 = 0;
    switch ( sub_494300(
               *((_DWORD *)v0 + 1),
               *((const void **)v0 + 2),
               *((_DWORD *)v0 + 3),
               *((double *)v0 + 2),
               *((double *)v0 + 3)) )
    {
      case 0:
      case 20:
        **(_DWORD **)v0 = 0;
        v3 = -1;
        break;
      case 14:
        **(_DWORD **)v0 = -2147483647;
        v3 = -1;
        break;
      case 18:
        **(_DWORD **)v0 = -2147483646;
        v3 = -1;
        break;
      default:
        **(_DWORD **)v0 = -1879048193;
        v3 = -1;
        break;
    }
    EnterCriticalSection(&stru_560FA0);
    sub_498170();
    LeaveCriticalSection(&stru_560FA0);
  }
  return v0 != 0;
}

// ===== ?__uncaught_exception@@YA_NXZ_3 @ 0x004982F0..0x004982FC =====
BOOL __cdecl __uncaught_exception()
{
  return dword_566A40 != 0;
}

// ===== sub_498300 @ 0x00498300..0x0049834A =====
void __cdecl __noreturn sub_498300()
{
  while ( dword_5669F8 )
  {
    if ( !sub_497FD0() && !sub_4981A0() )
    {
      if ( sub_4014D0() )
        sub_4527C0(1u, dword_5669F4);
    }
  }
  _endthread();
}

// ===== sub_498350 @ 0x00498350..0x004983F7 =====
void *sub_498350()
{
  _DWORD *v0; // eax
  _DWORD *v1; // eax
  void *result; // eax

  v0 = operator new(8u);
  if ( v0 )
    v1 = sub_4526E0(v0);
  else
    v1 = 0;
  dword_5669F4 = (int)v1;
  sub_401110();
  dword_5669F8 = 1;
  InitializeCriticalSection(&stru_560FA0);
  dword_566A00 = 1;
  result = (void *)_beginthread((_beginthread_proc_type)sub_498300, 0, 0);
  if ( result != (void *)-1 )
  {
    hThread = result;
    return (void *)SetThreadPriority(result, 1);
  }
  return result;
}

// ===== sub_498400 @ 0x00498400..0x004984AB =====
void sub_498400()
{
  DWORD ExitCode; // [esp+Ch] [ebp-4h] BYREF

  if ( dword_566A00 )
  {
    dword_566A00 = 0;
    dword_5669F8 = 0;
    while ( GetExitCodeThread(hThread, &ExitCode) )
      sub_4527C0(0x14u, dword_5669F4);
    EnterCriticalSection(&stru_560FA0);
    LeaveCriticalSection(&stru_560FA0);
    DeleteCriticalSection(&stru_560FA0);
    while ( sub_497F80() )
      ;
    while ( sub_498170() )
      ;
    if ( dword_5669F4 )
      (**(void (__thiscall ***)(int, int))dword_5669F4)(dword_5669F4, 1);
    dword_5669F4 = 0;
  }
}

// ===== sub_4984B0 @ 0x004984B0..0x00498531 =====
void __cdecl __noreturn sub_4984B0(void *a1)
{
  _DWORD v1[5]; // [esp+0h] [ebp-20h] BYREF
  int v2; // [esp+1Ch] [ebp-4h]

  v1[4] = v1;
  v2 = 0;
  *((_DWORD *)a1 + 3) = sub_493980(*((_DWORD *)a1 + 2), *((_DWORD *)a1 + 5), *((void **)a1 + 4));
  v2 = -1;
  *((_DWORD *)a1 + 1) = 1;
  _endthread();
}

// ===== sub_498550 @ 0x00498550..0x004985B0 =====
_DWORD *__cdecl sub_498550(int a1, int a2, int a3)
{
  _DWORD *v3; // esi
  void *v4; // eax

  v3 = operator new(0x18u);
  v3[1] = 0;
  v3[2] = a1;
  v3[3] = 0;
  v3[4] = a2;
  v3[5] = a3;
  v4 = (void *)_beginthread((_beginthread_proc_type)sub_4984B0, 0, v3);
  if ( v4 == (void *)-1 )
  {
    operator delete(v3);
    return 0;
  }
  else
  {
    *v3 = v4;
    SetThreadPriority(v4, 2);
    return v3;
  }
}

// ===== sub_4985B0 @ 0x004985B0..0x00498651 =====
void __cdecl __noreturn sub_4985B0(void *a1)
{
  const char *v1; // edi
  _DWORD v2[5]; // [esp+0h] [ebp-20h] BYREF
  int v3; // [esp+1Ch] [ebp-4h]

  v2[4] = v2;
  v3 = 0;
  v1 = (const char *)*((_DWORD *)a1 + 4);
  if ( sub_493940(v1) )
    *((_DWORD *)a1 + 3) = sub_4938F0((int)v1, *((_BYTE **)a1 + 2));
  else
    sub_465320(*((void **)a1 + 2), (size_t *)a1 + 3, *((_DWORD *)a1 + 5), 0LL);
  v3 = -1;
  *((_DWORD *)a1 + 1) = 1;
  _endthread();
}

// ===== sub_498670 @ 0x00498670..0x004986D0 =====
_DWORD *__cdecl sub_498670(int a1, int a2, int a3)
{
  _DWORD *v3; // esi
  void *v4; // eax

  v3 = operator new(0x18u);
  v3[1] = 0;
  v3[2] = a1;
  v3[3] = 0;
  v3[4] = a2;
  v3[5] = a3;
  v4 = (void *)_beginthread((_beginthread_proc_type)sub_4985B0, 0, v3);
  if ( v4 == (void *)-1 )
  {
    operator delete(v3);
    return 0;
  }
  else
  {
    *v3 = v4;
    SetThreadPriority(v4, 2);
    return v3;
  }
}

// ===== sub_4986D0 @ 0x004986D0..0x004986F9 =====
MMRESULT sub_4986D0()
{
  MMRESULT result; // eax

  timeGetDevCaps((LPTIMECAPS)&uPeriod, 8u);
  result = uPeriod;
  if ( uPeriod < 0xA )
  {
    result = timeBeginPeriod(uPeriod);
    dword_566A48 = 1;
  }
  return result;
}

// ===== sub_498700 @ 0x00498700..0x00498720 =====
MMRESULT sub_498700()
{
  MMRESULT result; // eax

  if ( dword_566A48 )
  {
    result = timeEndPeriod(uPeriod);
    dword_566A48 = 0;
  }
  return result;
}

// ===== sub_498720 @ 0x00498720..0x00498770 =====
int sub_498720()
{
  DWORD Time; // eax

  if ( dword_566A48 )
    Time = timeGetTime();
  else
    Time = GetTickCount();
  if ( Time < dword_566A50 || dword_566A50 + dword_503DE8 < Time )
    dword_566A58 += Time - dword_566A50;
  dword_566A50 = Time;
  if ( dword_566A54 )
    return dword_566A54;
  else
    return Time - dword_566A58;
}

// ===== sub_498770 @ 0x00498770..0x004987B8 =====
int __cdecl sub_498770(int a1)
{
  int v1; // ecx

  v1 = 0;
  if ( dword_5666EC && !dword_566A5C )
  {
    if ( (sub_498810() || a1 != v1) && dword_566A54 == v1 )
    {
      dword_566A54 = sub_498720();
      v1 = 1;
    }
    dword_566A5C = 1;
  }
  return v1;
}

// ===== sub_4987C0 @ 0x004987C0..0x00498800 =====
int sub_4987C0()
{
  int result; // eax
  int v1; // esi

  result = 0;
  if ( dword_5666EC && dword_566A5C )
  {
    v1 = dword_566A54;
    if ( dword_566A54 )
    {
      dword_566A54 = 0;
      dword_566A58 += sub_498720() - v1;
      result = 1;
    }
    dword_566A5C = 0;
  }
  return result;
}

// ===== sub_498800 @ 0x00498800..0x0049880C =====
int __thiscall sub_498800(void *this)
{
  int result; // eax

  result = dword_566A4C;
  dword_566A4C = (int)this;
  return result;
}

// ===== sub_498810 @ 0x00498810..0x00498816 =====
int sub_498810()
{
  return dword_566A4C;
}

// ===== sub_498820 @ 0x00498820..0x00498839 =====
int __thiscall sub_498820(char *this)
{
  int result; // eax

  result = 0;
  if ( (unsigned int)(this - 50) <= 0xEA2E )
  {
    dword_503DE8 = (int)this;
    return 1;
  }
  return result;
}

// ===== sub_498840 @ 0x00498840..0x00498855 =====
BOOL __usercall sub_498840@<eax>(LARGE_INTEGER *a1@<esi>)
{
  BOOL result; // eax

  result = QueryPerformanceFrequency(a1);
  if ( !result )
  {
    a1->LowPart = 1000;
    a1->HighPart = 0;
  }
  return result;
}

// ===== sub_498860 @ 0x00498860..0x0049887A =====
LARGE_INTEGER sub_498860()
{
  LARGE_INTEGER v1; // [esp+4h] [ebp-8h] BYREF

  sub_498840(&v1);
  return v1;
}

// ===== sub_498880 @ 0x00498880..0x0049889A =====
DWORD __usercall sub_498880@<eax>(LARGE_INTEGER *a1@<esi>)
{
  DWORD result; // eax

  result = QueryPerformanceCounter(a1);
  if ( !result )
  {
    result = sub_498720();
    a1->LowPart = result;
    a1->HighPart = 0;
  }
  return result;
}

// ===== sub_4988A0 @ 0x004988A0..0x004988BA =====
LARGE_INTEGER sub_4988A0()
{
  LARGE_INTEGER v1; // [esp+8h] [ebp-8h] BYREF

  sub_498880(&v1);
  return v1;
}

// ===== sub_4988C0 @ 0x004988C0..0x004988F9 =====
int __usercall sub_4988C0@<eax>(_QWORD *a1@<edi>)
{
  LARGE_INTEGER v2; // [esp+4h] [ebp-10h] BYREF
  LARGE_INTEGER v3; // [esp+Ch] [ebp-8h] BYREF

  sub_498840(&v2);
  sub_498880(&v3);
  *a1 = (unsigned __int64)((double)v3.QuadPart / (double)v2.QuadPart * 1000000000.0);
  return 1;
}

// ===== sub_498900 @ 0x00498900..0x00498911 =====
int sub_498900()
{
  int result; // eax

  result = 0;
  memset(&dword_560FC0, 0, 0x3C0u);
  return result;
}

// ===== sub_498920 @ 0x00498920..0x0049896C =====
BOOL __usercall sub_498920@<eax>(int a1@<esi>)
{
  void **v1; // eax
  BOOL v2; // edi

  v1 = (void **)*(&dword_560FC0 + a1);
  v2 = v1 != 0;
  if ( v1 )
  {
    operator delete[](*v1);
    operator delete[](*((void **)*(&dword_560FC0 + a1) + 1));
    operator delete(*(&dword_560FC0 + a1));
    *(&dword_560FC0 + a1) = 0;
  }
  return v2;
}

// ===== sub_498970 @ 0x00498970..0x00498983 =====
BOOL sub_498970()
{
  int i; // esi
  BOOL result; // eax

  for ( i = 0; i < 240; ++i )
    result = sub_498920(i);
  return result;
}

// ===== sub_498990 @ 0x00498990..0x00498A57 =====
BOOL __usercall sub_498990@<eax>(int a1@<edi>, const char *a2, char *a3)
{
  size_t v3; // esi
  _BYTE *v4; // edx
  char *v5; // ecx
  char v6; // al
  _DWORD *Src; // [esp+Ch] [ebp-4h]

  sub_498920(a1);
  Src = operator new[](0x20000u);
  v3 = sub_465AB0(a3, Src, a2);
  if ( v3 )
  {
    *(&dword_560FC0 + a1) = operator new(8u);
    *(_DWORD *)*(&dword_560FC0 + a1) = operator new[](strlen(a3) + 1);
    v4 = *(_BYTE **)*(&dword_560FC0 + a1);
    v5 = a3;
    do
    {
      v6 = *v5;
      *v4++ = *v5++;
    }
    while ( v6 );
    *((_DWORD *)*(&dword_560FC0 + a1) + 1) = operator new[](v3);
    memcpy_0(*((void **)*(&dword_560FC0 + a1) + 1), Src, v3);
  }
  operator delete[](Src);
  return v3 != 0;
}

// ===== sub_498A60 @ 0x00498A60..0x00498B06 =====
int __usercall sub_498A60@<eax>(_DWORD *a1@<esi>)
{
  char *v1; // ebx
  unsigned int v2; // eax
  const char *v3; // edx
  char *v5; // [esp+8h] [ebp-108h]
  char Buffer[256]; // [esp+Ch] [ebp-104h] BYREF

  v1 = sub_48DF50(a1);
  v5 = sub_48DF50(a1);
  v2 = sub_4450B0(a1);
  if ( v2 >= 0xF0 )
  {
    sprintf(Buffer, &byte_4EC61C, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( !sub_498990(v2, v3, v1) )
  {
    sprintf(Buffer, &byte_4EB960, v5, v1);
    sub_4646F0(Buffer, (int)a1);
  }
  return 0;
}

// ===== sub_498B10 @ 0x00498B10..0x00498B6B =====
int __thiscall sub_498B10(_DWORD *this)
{
  unsigned int v2; // eax
  char Buffer[256]; // [esp+4h] [ebp-104h] BYREF

  v2 = sub_4450B0(this);
  if ( v2 >= 0xF0 )
  {
    sprintf(Buffer, &byte_4EC61C, v2);
    sub_4646F0(Buffer, (int)this);
  }
  sub_498920(v2);
  return 0;
}

// ===== sub_498B70 @ 0x00498B70..0x00498B8F =====
int __usercall sub_498B70@<eax>(_DWORD *a1@<eax>)
{
  int v2; // eax

  sub_444D80(a1);
  sub_444D80(a1);
  v2 = sub_4450F0((int)a1);
  sub_444FE0(v2, (int)a1);
  return 0;
}

// ===== sub_498B90 @ 0x00498B90..0x00498D30 =====
int __cdecl sub_498B90(_DWORD *a1)
{
  _DWORD *v1; // ecx
  unsigned int v2; // esi
  unsigned int v3; // eax
  unsigned int v4; // edx
  int v5; // eax
  const char *v7; // [esp-Ch] [ebp-13Ch]
  int v8; // [esp+8h] [ebp-128h]
  char Buffer[256]; // [esp+Ch] [ebp-124h] BYREF
  _DWORD v10[8]; // [esp+10Ch] [ebp-24h] BYREF

  v2 = (unsigned __int8)sub_445030(a1);
  switch ( v2 )
  {
    case 0xF0u:
      return sub_498A60(a1);
    case 0xF1u:
      return sub_498B10(v1);
    case 0xF8u:
      return sub_498B70(a1);
  }
  if ( v2 >= 0xF0 )
  {
    sprintf(Buffer, v7, &unk_4EC68C);
LABEL_7:
    sub_4646F0(Buffer, (int)a1);
  }
  if ( !*(&dword_560FC0 + v2) )
  {
    sprintf(Buffer, &byte_4EC65C, v2);
    goto LABEL_7;
  }
  v10[0] = 16;
  v10[1] = 16;
  v10[2] = 0;
  v10[3] = 0;
  v10[4] = 369102854;
  v10[5] = 63743;
  v10[6] = 0;
  v10[7] = 0;
  v8 = sub_444CE0(a1, v10, "Mediation program");
  if ( v8 == 0x80000000 )
  {
    sprintf(Buffer, &byte_4EC6E0, v2);
    sub_4646F0(Buffer, (int)a1);
  }
  if ( sub_444CE0(a1, *((_DWORD **)*(&dword_560FC0 + v2) + 1), *(const char **)*(&dword_560FC0 + v2)) == 0x80000000 )
  {
    sprintf(Buffer, &byte_4EC6E0, v2);
    goto LABEL_7;
  }
  sub_444FF0((int)a1);
  v3 = sub_444C40(a1);
  if ( v4 >= v3 )
    sub_4646F0(byte_4E7738, (int)a1);
  v5 = sub_444FD0((int)a1);
  sub_445110((int)a1, v5 + 2);
  sub_444FE0(v8, (int)a1);
  return 0;
}

// ===== sub_498D30 @ 0x00498D30..0x00498DBB =====
BOOL __cdecl sub_498D30(PVOID pvParam, int a2, int a3)
{
  const BYTE *v3; // eax
  const BYTE *v4; // eax
  DWORD dwDisposition; // [esp+0h] [ebp-8h] BYREF
  HKEY phkResult; // [esp+4h] [ebp-4h] BYREF

  RegCreateKeyExA(HKEY_CURRENT_USER, "control panel\\desktop", 0, 0, 0, 0x20006u, 0, &phkResult, &dwDisposition);
  v3 = "2";
  if ( !a2 )
    v3 = (const BYTE *)"0";
  RegSetValueExA(phkResult, "WallpaperStyle", 0, 1u, v3, 2u);
  v4 = "1";
  if ( !a3 )
    v4 = (const BYTE *)"0";
  RegSetValueExA(phkResult, "TileWallpaper", 0, 1u, v4, 2u);
  return SystemParametersInfoA(0x14u, 0, pvParam, 3u);
}

// ===== sub_498DC0 @ 0x00498DC0..0x00499CCA =====
int __stdcall sub_498DC0(HWND a1, UINT Msg, signed int wParam, unsigned int lParam)
{
  char v4; // edx^2
  unsigned int v5; // ecx
  int result; // eax
  HMENU SystemMenu; // eax
  HMENU v8; // eax
  LONG top; // edx
  LONG bottom; // ecx
  HDC DC; // esi
  HBRUSH StockObject; // eax
  int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // eax
  _DWORD *v17; // edi
  int v18; // eax
  _DWORD *v19; // eax
  int v20; // eax
  WPARAM v21; // esi
  COLORREF v22; // eax
  int v23; // [esp+Ch] [ebp-3A0h]
  int SystemMetrics; // [esp+Ch] [ebp-3A0h]
  unsigned int v25; // [esp+2Ch] [ebp-380h] BYREF
  int v26; // [esp+30h] [ebp-37Ch] BYREF
  struct tagRECT Rect; // [esp+34h] [ebp-378h] BYREF
  RECT rc; // [esp+44h] [ebp-368h] BYREF
  struct tagPAINTSTRUCT Paint; // [esp+54h] [ebp-358h] BYREF
  char v30[788]; // [esp+94h] [ebp-318h] BYREF

  v25 = lParam;
  sub_499F70(lParam, wParam);
  if ( Msg <= 0x100 )
  {
    if ( Msg != 256 )
    {
      switch ( Msg )
      {
        case 1u:
          dword_5666F0 = 1;
          return 0;
        case 2u:
          dword_5666F0 = 0;
          hWndParent = 0;
          PostQuitMessage(0);
          return 0;
        case 5u:
          if ( wParam )
          {
            if ( wParam == 1 )
            {
              if ( dword_5666B8 )
              {
                sub_494460();
                sub_48F760(1);
                sub_498770(0);
              }
              if ( !dword_506A84 )
              {
                SystemMenu = GetSystemMenu(hWndParent, 0);
                EnableMenuItem(SystemMenu, 0xF060u, 1u);
              }
              dword_5666EC = 0;
            }
          }
          else
          {
            if ( dword_5666B8 )
            {
              sub_4944B0();
              sub_48F760(0);
              if ( sub_49A220() )
              {
                GetWindowRect(hWndParent, &Rect);
                if ( !sub_46F930(Rect.left, Rect.top) )
                  sub_461690();
              }
            }
            v8 = GetSystemMenu(hWndParent, 0);
            EnableMenuItem(v8, 0xF060u, 0);
            dword_5666EC = 1;
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 6u:
          dword_5666E8 = (_WORD)wParam != 0;
          if ( (_WORD)wParam )
          {
            if ( dword_5666B8 )
            {
              sub_4987C0();
              sub_461CA0();
            }
            if ( dword_5666F4 && !IsIconic(a1) )
              dword_5666F4 = 0;
          }
          else if ( dword_5666B8 )
          {
            sub_498770(0);
            sub_461C90();
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0xFu:
          BeginPaint(a1, &Paint);
          EndPaint(a1, &Paint);
          if ( !dword_506BD4 || !dword_506BD8 )
            return 0;
          if ( dword_5666B8 )
          {
            if ( !sub_48F680() && (!sub_45F640() || sub_45E850() == 1 || sub_460030()) )
            {
              sub_461A70(0, 0, 1, 0);
              return 0;
            }
          }
          else
          {
            GetClientRect(a1, &rc);
            DC = GetDC(hWndParent);
            StockObject = (HBRUSH)GetStockObject(4);
            FillRect(DC, &rc, StockObject);
            ReleaseDC(hWndParent, DC);
          }
          return 0;
        case 0x10u:
          sub_46E5B0();
          if ( !dword_506A84 )
          {
            sub_496540(2, 0, 0);
            return 0;
          }
          sub_464190();
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x13u:
          if ( !dword_566A68 )
            return DefWindowProcA(a1, Msg, wParam, lParam);
          dword_566A68 = 0;
          return 0;
        case 0x1Au:
          if ( !lParam
            || strcmp((const char *)lParam, "UserInteractionMode")
            && wcscmp((const unsigned __int16 *)lParam, L"UserInteractionMode") )
          {
            return DefWindowProcA(a1, Msg, wParam, lParam);
          }
          goto LABEL_77;
        case 0x20u:
          if ( *(&hCursor + dword_5666D8) )
            SetCursor(*(&hCursor + dword_5666D8));
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x46u:
          if ( dword_5666B8 && (*(_BYTE *)(lParam + 24) & 0x11) == 0 && !sub_45F640() )
          {
            if ( dword_566A60 || dword_566A64 )
            {
              sub_4615E0(&Rect);
              top = Rect.top;
              *(_DWORD *)(lParam + 8) = Rect.left;
              *(_DWORD *)(lParam + 12) = top;
              dword_566A64 = 0;
            }
            sub_4616E0(&Rect);
            bottom = Rect.bottom;
            *(_DWORD *)(lParam + 16) = Rect.right;
            *(_DWORD *)(lParam + 20) = bottom;
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x7Eu:
LABEL_77:
          sub_461410();
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x7Fu:
          switch ( wParam )
          {
            case 0:
              goto LABEL_59;
            case 1:
              SystemMetrics = GetSystemMetrics(12);
              v14 = GetSystemMetrics(11);
              result = (int)LoadImageA(hInst, (LPCSTR)0x65, 1u, v14, SystemMetrics, 0x8000u);
              if ( !result )
                return DefWindowProcA(a1, Msg, wParam, lParam);
              break;
            case 2:
LABEL_59:
              v23 = GetSystemMetrics(50);
              v13 = GetSystemMetrics(49);
              result = (int)LoadImageA(hInst, (LPCSTR)0x65, 1u, v13, v23, 0x8000u);
              if ( !result )
                return DefWindowProcA(a1, Msg, wParam, lParam);
              break;
            default:
              return DefWindowProcA(a1, Msg, wParam, lParam);
          }
          break;
        case 0x85u:
          if ( dword_5666B8 )
            return DefWindowProcA(a1, Msg, wParam, lParam);
          return 0;
        case 0x86u:
          dword_5666E8 = wParam;
          if ( !dword_5666B8 || wParam || !sub_48F260() )
            return DefWindowProcA(a1, Msg, wParam, lParam);
          return 0;
        case 0xA1u:
          v15 = wParam - 2;
          if ( wParam != 2 )
            goto LABEL_71;
          if ( !sub_49A230() )
            SetActiveWindow(a1);
          if ( sub_49A220() && !sub_45F640() )
          {
            GetWindowRect(hWndParent, &Rect);
            dword_566A74 = 1;
            dword_561388 = (__int16)lParam - Rect.left;
            dword_56138C = SHIWORD(v25) - Rect.top;
          }
          return 0;
        case 0xA2u:
          v15 = wParam - 2;
          if ( wParam == 2 )
            return 0;
LABEL_71:
          v16 = v15 - 6;
          if ( !v16 || v16 == 12 )
            return DefWindowProcA(a1, Msg, wParam, lParam);
          return 0;
        case 0xA4u:
        case 0xA5u:
          return 0;
        default:
          return DefWindowProcA(a1, Msg, wParam, lParam);
      }
      return result;
    }
    if ( sub_46DA60(wParam) )
      sub_46DB00(wParam);
    if ( sub_46DA80(wParam) )
    {
      sub_46E5B0();
      sub_496540(3, wParam, 0);
      if ( sub_4617B0(wParam) )
        sub_461440();
    }
    return DefWindowProcA(a1, Msg, wParam, lParam);
  }
  if ( Msg > 0x120 )
  {
    if ( Msg > 0x233 )
    {
      if ( Msg > 0x8000 )
      {
        if ( Msg == 32769 )
        {
          sub_408820(lParam);
        }
        else if ( Msg == 36864 )
        {
          sub_496540(16, 0, 0);
          sub_48D370(v30, wParam);
          sub_463B30(v30);
          return 0;
        }
      }
      else
      {
        switch ( Msg )
        {
          case 0x8000u:
            if ( dword_56694C )
            {
              while ( (*(int (__stdcall **)(int, unsigned int *, struct tagRECT *, int *, _DWORD))(*(_DWORD *)dword_56694C
                                                                                                 + 32))(
                        dword_56694C,
                        &v25,
                        &Rect,
                        &v26,
                        0) >= 0 )
              {
                (*(void (__stdcall **)(int, unsigned int, LONG, int))(*(_DWORD *)dword_56694C + 48))(
                  dword_56694C,
                  v25,
                  Rect.left,
                  v26);
                if ( v25 == 1 )
                {
                  dword_566A68 = IsIconic(a1);
                  dword_566960 = 0;
                }
              }
            }
            if ( !sub_48F680() )
            {
              sub_48F0E0();
              sub_461D70();
            }
            break;
          case 0x240u:
            if ( sub_46AF90((unsigned __int16)wParam, (HTOUCHINPUT)lParam) )
              return 0;
            break;
          case 0x312u:
            if ( HIWORD(lParam) == 44 )
              return 0;
            break;
          default:
            if ( Msg == 953 && wParam == 1 )
              sub_48D8D0();
            break;
        }
      }
    }
    else
    {
      if ( Msg == 563 )
      {
        sub_496540(16, 0, 0);
        sub_463AB0((HDROP)wParam);
        return 0;
      }
      switch ( Msg )
      {
        case 0x133u:
          SetBkMode((HDC)wParam, 1);
          v22 = sub_4642C0();
          SetTextColor((HDC)wParam, v22);
          return (int)GetStockObject(5);
        case 0x201u:
          goto LABEL_119;
        case 0x202u:
          if ( dword_566764 )
            goto LABEL_121;
          sub_46DB00(1);
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x203u:
          sub_496540(128, 0, 0);
LABEL_119:
          sub_48E560(0, (unsigned __int16)lParam, HIWORD(lParam));
          sub_46E5B0();
          sub_496540(3, 1, 0);
          SetFocus(a1);
          v17 = sub_463780();
          if ( v17 )
          {
            sub_48E680(&Rect.left);
            sub_4637C0(v17, Rect.left, Rect.top);
            sub_46DB20(1);
            dword_566764 = 1;
          }
          else
          {
            sub_46DA80(1);
LABEL_121:
            dword_566764 = 0;
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x204u:
          goto LABEL_126;
        case 0x205u:
          v20 = sub_48EE70();
          if ( v20 )
          {
            if ( v20 == 1 )
              SendMessageA(a1, 0x202u, wParam, lParam);
          }
          else
          {
            if ( dword_566768 )
              goto LABEL_131;
            sub_46DB00(2);
          }
          break;
        case 0x206u:
          sub_496540(129, 0, 0);
LABEL_126:
          v18 = sub_48EE70();
          if ( v18 )
          {
            if ( v18 == 1 )
              SendMessageA(a1, 0x201u, wParam, lParam);
          }
          else
          {
            sub_48E560(1u, (unsigned __int16)lParam, HIWORD(lParam));
            sub_46E5B0();
            sub_496540(3, 2, 0);
            SetFocus(a1);
            v19 = sub_463780();
            if ( v19 )
            {
              v19[3] = 1;
              sub_46DB20(2);
              dword_566768 = 1;
            }
            else
            {
              sub_46DA80(2);
LABEL_131:
              dword_566768 = 0;
            }
          }
          break;
        case 0x207u:
          sub_48E560(2u, (unsigned __int16)lParam, HIWORD(v5));
          sub_46E5B0();
          sub_496540(3, 4, 0);
          SetFocus(a1);
          sub_46DA80(4);
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x208u:
          sub_46DB00(4);
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x20Au:
          if ( HIWORD(wParam) )
          {
            sub_46E5B0();
            sub_496540(3, (wParam < 0) + 14, 0);
            if ( sub_463840() && sub_463880() )
            {
              sub_463920(wParam < 0);
            }
            else if ( wParam >= 0 )
            {
              sub_46DA80(14);
              sub_46DB00(14);
            }
            else
            {
              sub_46DA80(15);
              sub_46DB00(15);
            }
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x20Bu:
          v21 = HIWORD(wParam) & 1;
          sub_48E560(((_WORD)v21 == 0) + 3, (unsigned __int16)lParam, HIWORD(lParam));
          sub_46E5B0();
          SetFocus(a1);
          if ( (_WORD)v21 )
          {
            sub_496540(3, 5, 0);
            sub_46DA80(5);
          }
          else
          {
            sub_496540(3, 6, 0);
            sub_46DA80(6);
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x20Cu:
          if ( (v4 & 1) != 0 )
            sub_46DB00(5);
          else
            sub_46DB00(6);
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x20Eu:
          if ( HIWORD(wParam) )
          {
            sub_46E5B0();
            sub_496540(3, (wParam >= 0) + 142, 0);
            if ( wParam >= 0 )
            {
              sub_46DA80(143);
              sub_46DB00(143);
            }
            else
            {
              sub_46DA80(142);
              sub_46DB00(142);
            }
          }
          return DefWindowProcA(a1, Msg, wParam, lParam);
        case 0x218u:
          switch ( wParam )
          {
            case 0:
            case 9:
              sub_48D1D0();
              while ( __uncaught_exception() || __uncaught_exception() )
                Sleep(1u);
              sub_48D1E0(5000);
              result = 1;
              break;
            case 2:
              sub_48D1E0(0);
              result = 1;
              break;
            case 4:
              sub_48D1E0(1000);
              result = 1;
              break;
            case 6:
            case 7:
            case 18:
              sub_48D1E0(500);
              result = 1;
              break;
            default:
              return DefWindowProcA(a1, Msg, wParam, lParam);
          }
          return result;
        case 0x219u:
          sub_4609A0();
          return DefWindowProcA(a1, Msg, wParam, lParam);
        default:
          return DefWindowProcA(a1, Msg, wParam, lParam);
      }
    }
    return DefWindowProcA(a1, Msg, wParam, lParam);
  }
  if ( Msg == 288 )
    return 0x10000;
  switch ( Msg )
  {
    case 0x101u:
      sub_46DB00(wParam);
      return DefWindowProcA(a1, Msg, wParam, lParam);
    case 0x104u:
      switch ( wParam )
      {
        case 13:
          if ( dword_5666EC && (lParam & 0x40000000) == 0 )
            sub_461440();
          break;
        case 115:
          PostMessageA(hWndParent, 0x10u, 0, 0);
          return 0;
        case 121:
          if ( sub_46DA80(121) )
          {
            sub_46E5B0();
            sub_496540(3, 121, 0);
            if ( sub_4617B0(121) )
            {
              sub_461440();
              return 0;
            }
          }
          break;
      }
      return 0;
    case 0x105u:
      if ( wParam == 18 )
      {
        if ( sub_46DA80(18) )
        {
          sub_46E5B0();
          sub_496540(3, 18, 0);
          if ( sub_4617B0(18) )
            sub_461440();
        }
      }
      else if ( wParam != 121 )
      {
        return 0;
      }
      sub_46DB00(wParam);
      return 0;
    case 0x112u:
      if ( wParam != 61472 )
      {
        if ( (wParam == 61760 || wParam == 61808) && sub_49A230() )
          return 0;
        return DefWindowProcA(a1, Msg, wParam, lParam);
      }
      result = sub_49A220();
      if ( result )
        return DefWindowProcA(a1, Msg, wParam, lParam);
      break;
    case 0x119u:
      if ( !sub_46AC20((HGESTUREINFO)lParam) )
        return DefWindowProcA(a1, Msg, wParam, lParam);
      return 0;
    default:
      return DefWindowProcA(a1, Msg, wParam, lParam);
  }
  return result;
}

// ===== sub_499F40 @ 0x00499F40..0x00499F6F =====
_DWORD *__cdecl sub_499F40(int a1, int a2)
{
  _DWORD *result; // eax

  result = operator new(0x18u);
  *result = a1;
  result[1] = a2;
  result[4] = 0;
  result[5] = dword_566A90;
  dword_566A90 = result;
  return result;
}

// ===== sub_499F70 @ 0x00499F70..0x00499F9A =====
_DWORD *__usercall sub_499F70@<eax>(int a1@<edx>, int a2@<ecx>, int a3@<edi>)
{
  _DWORD *result; // eax

  result = dword_566A90;
  if ( dword_566A90 )
  {
    do
    {
      if ( result[1] == a3 )
      {
        result[4] = 1;
        result[2] = a1;
        result[3] = a2;
      }
      result = (_DWORD *)result[5];
    }
    while ( result );
  }
  return result;
}

// ===== sub_499FA0 @ 0x00499FA0..0x00499FD1 =====
void __usercall sub_499FA0(int a1@<edx>, int a2@<esi>)
{
  _DWORD *v2; // eax
  _DWORD *v3; // ecx

  v2 = dword_566A90;
  v3 = &unk_566A7C;
  if ( dword_566A90 )
  {
    while ( *v2 != a2 || v2[1] != a1 )
    {
      v3 = v2;
      v2 = (_DWORD *)v2[5];
      if ( !v2 )
        return;
    }
    v3[5] = v2[5];
    operator delete(v2);
  }
}

// ===== sub_499FE0 @ 0x00499FE0..0x0049A02B =====
int __usercall sub_499FE0@<eax>(int a1@<eax>, _DWORD *a2@<edx>, int a3@<edi>)
{
  _DWORD *v3; // ecx
  int result; // eax

  v3 = dword_566A90;
  result = 0;
  if ( dword_566A90 )
  {
    while ( *v3 != a3 || v3[1] != a1 )
    {
      v3 = (_DWORD *)v3[5];
      if ( !v3 )
        return result;
    }
    *a2 = *v3;
    a2[1] = v3[1];
    a2[2] = v3[2];
    a2[3] = v3[3];
    a2[4] = v3[4];
    a2[5] = 0;
    v3[4] = 0;
    return 1;
  }
  return result;
}

// ===== sub_49A030 @ 0x0049A030..0x0049A05C =====
void sub_49A030()
{
  void *v0; // esi
  void *v1; // [esp-4h] [ebp-8h]

  v0 = dword_566A90;
  while ( v0 )
  {
    v1 = v0;
    v0 = (void *)*((_DWORD *)v0 + 5);
    operator delete(v1);
  }
  dword_566A90 = 0;
}

// ===== sub_49A060 @ 0x0049A060..0x0049A0CA =====
int sub_49A060()
{
  int v0; // esi
  struct tagMSG Msg; // [esp+8h] [ebp-1Ch] BYREF

  v0 = 0;
  if ( !PeekMessageA(&Msg, 0, 0, 0, 1u) )
    return 0;
  while ( Msg.message != 18 )
  {
    TranslateMessage(&Msg);
    DispatchMessageA(&Msg);
    ++v0;
    if ( !PeekMessageA(&Msg, 0, 0, 0, 1u) )
      return v0;
  }
  return -1;
}

// ===== sub_49A0D0 @ 0x0049A0D0..0x0049A102 =====
BOOL __usercall sub_49A0D0@<eax>(BOOL result@<eax>)
{
  HMENU SystemMenu; // eax
  UINT v2; // [esp-4h] [ebp-4h]

  dword_506A84 = result;
  if ( !dword_5666EC )
  {
    v2 = !result;
    SystemMenu = GetSystemMenu(hWndParent, 0);
    return EnableMenuItem(SystemMenu, 0xF060u, v2);
  }
  return result;
}

// ===== sub_49A110 @ 0x0049A110..0x0049A14B =====
int *__usercall sub_49A110@<eax>(int *result@<eax>)
{
  if ( result )
  {
    X = *result;
    Y = result[1];
  }
  else
  {
    result = (int *)sub_4615E0(&X);
  }
  dword_566A6C = 1;
  return result;
}

// ===== sub_49A150 @ 0x0049A150..0x0049A168 =====
int *__usercall sub_49A150@<eax>(int a1@<eax>, int a2@<ecx>)
{
  int v3[2]; // [esp+0h] [ebp-8h] BYREF

  v3[0] = a1;
  v3[1] = a2;
  return sub_49A110(v3);
}

// ===== sub_49A170 @ 0x0049A170..0x0049A215 =====
int __usercall sub_49A170@<eax>(int a1@<esi>)
{
  int result; // eax
  bool v2; // zf

  result = dword_5666F0;
  if ( dword_5666F0 )
  {
    if ( a1 )
    {
      if ( !sub_45F640() )
      {
        if ( dword_566A6C )
        {
          SetWindowPos(hWndParent, 0, X, Y, 0, 0, 0x25u);
          sub_4610D0(Y);
        }
        ShowWindow(hWndParent, 1);
        UpdateWindow(hWndParent);
      }
      dword_566A6C = 0;
    }
    else
    {
      ShowWindow(hWndParent, 0);
    }
    dword_566A70 = a1;
    v2 = GetForegroundWindow() == hWndParent;
    result = dword_5666F0;
    dword_5666E8 = v2;
  }
  return result;
}

// ===== sub_49A220 @ 0x0049A220..0x0049A226 =====
int sub_49A220()
{
  return dword_566A70;
}

// ===== sub_49A230 @ 0x0049A230..0x0049A236 =====
int sub_49A230()
{
  return dword_5666E8;
}

// ===== sub_49A240 @ 0x0049A240..0x0049A246 =====
int sub_49A240()
{
  return dword_5666F4;
}

// ===== sub_49A250 @ 0x0049A250..0x0049A256 =====
int __usercall sub_49A250@<eax>(int result@<eax>)
{
  dword_566A60 = result;
  return result;
}

// ===== sub_49A260 @ 0x0049A260..0x0049A2C8 =====
void sub_49A260()
{
  struct tagPOINT Point; // [esp+0h] [ebp-8h] BYREF

  if ( dword_566A74 )
  {
    if ( sub_46D560(1) >= 0 )
    {
      dword_566A74 = 0;
    }
    else
    {
      GetCursorPos(&Point);
      SetWindowPos(hWndParent, 0, Point.x - dword_561388, Point.y - dword_56138C, 0, 0, 0x25u);
    }
  }
}

// ===== sub_49A2D0 @ 0x0049A2D0..0x0049A345 =====
BOOL __cdecl sub_49A2D0(int a1)
{
  unsigned int i; // esi
  BOOL result; // eax
  unsigned int j; // esi

  if ( a1 )
  {
    if ( !dword_566A78 )
    {
      for ( i = 0; i < 0x10; ++i )
        result = RegisterHotKey(hWndParent, i, i & 0xF, 0x2Cu);
      dword_566A78 = 1;
    }
  }
  else if ( dword_566A78 )
  {
    for ( j = 0; j < 0x10; ++j )
      result = UnregisterHotKey(hWndParent, j);
    dword_566A78 = 0;
  }
  return result;
}

// ===== sub_49A350 @ 0x0049A350..0x0049A360 =====
int __userpurge sub_49A350@<eax>(int a1@<eax>, int a2)
{
  return sub_49A360(a1);
}

// ===== sub_49A360 @ 0x0049A360..0x0049A3EF =====
int __userpurge sub_49A360@<eax>(int a1@<eax>, _DWORD *a2@<esi>, unsigned int a3)
{
  if ( strcmp((const char *)a1, "BF_Movie_______") || a3 >= *(_DWORD *)(a1 + 40) || !*a2 )
    return 0;
  if ( !a3 )
  {
    sub_49A3F0(a2, a1);
    return 1;
  }
  if ( a2[2] != *(_DWORD *)(a1 + 20) || a2[3] != *(_DWORD *)(a1 + 24) || a2[4] != *(_DWORD *)(a1 + 32) )
    return 0;
  sub_49A4C0((int)a2, (void *)(a1 + *(_DWORD *)(a1 + 4 * a3 + 64)));
  return 1;
}

// ===== sub_49A3F0 @ 0x0049A3F0..0x0049A4B1 =====
int __userpurge sub_49A3F0@<eax>(int a1@<eax>, int a2, _DWORD *a3)
{
  unsigned int v3; // ebx
  unsigned int *v4; // esi
  unsigned int *v6; // edi
  int v7; // eax
  unsigned int *v8; // [esp-4h] [ebp-14h]
  unsigned int v9; // [esp+Ch] [ebp-4h] BYREF

  v3 = sub_49ACC0(a1, &v9);
  v4 = (unsigned int *)operator new[](v3);
  if ( !sub_49A5A0(v4, v3) )
  {
    v8 = v4;
LABEL_3:
    operator delete[](v8);
    return 0;
  }
  v9 = *v4;
  v6 = (unsigned int *)operator new[](v9);
  if ( sub_49A7D0(v6, v9, v4 + 1, v3 - 4) )
  {
    operator delete[](v4);
    v7 = sub_49A860(a3[5], a3[6], a3[7]);
    v8 = v6;
    if ( !v7 )
      goto LABEL_3;
    operator delete[](v6);
    return 1;
  }
  else
  {
    operator delete[](v6);
    operator delete[](v4);
    return 0;
  }
}

// ===== sub_49A4C0 @ 0x0049A4C0..0x0049A59A =====
int __userpurge sub_49A4C0@<eax>(int *a1@<edi>, void **a2, void *a3)
{
  unsigned int v3; // ebx
  int *v4; // esi
  int *v6; // eax
  int v7; // ebx
  int v8; // eax
  int *v9; // [esp-4h] [ebp-10h]
  unsigned int v10; // [esp-4h] [ebp-10h]
  int v11; // [esp+8h] [ebp-4h] BYREF
  int *v12; // [esp+18h] [ebp+Ch]

  v3 = sub_49ACC0(a3, &v11);
  v4 = (int *)operator new[](v3);
  if ( !sub_49A5A0(v4, v3) )
  {
    v9 = v4;
LABEL_3:
    operator delete[](v9);
    return 0;
  }
  v11 = *v4;
  v6 = (int *)operator new[](v11 + 2);
  v10 = v3 - 4;
  v7 = v11;
  v12 = v6;
  if ( sub_49A7D0(v6, v11, v4 + 1, v10) )
  {
    operator delete[](v4);
    v8 = sub_49AB10(*a2, 4 * a1[6] * a1[5], (int)v12, v7, a1[5], a1[6], a1[7]);
    v9 = v12;
    if ( !v8 )
      goto LABEL_3;
    operator delete[](v12);
    return 1;
  }
  else
  {
    operator delete[](v12);
    operator delete[](v4);
    return 0;
  }
}

// ===== sub_49A5A0 @ 0x0049A5A0..0x0049A7C3 =====
int __thiscall sub_49A5A0(unsigned __int8 *this, int a2, unsigned int a3)
{
  unsigned int i; // esi
  int v5; // eax
  unsigned int v6; // ecx
  char *v7; // eax
  int v8; // edx
  char *v9; // eax
  int v10; // ecx
  int *v11; // esi
  unsigned int v12; // edx
  _DWORD *v13; // eax
  int v14; // edi
  int v15; // edx
  int v16; // eax
  int v17; // ecx
  _DWORD *v18; // esi
  unsigned int v19; // edi
  unsigned __int8 v20; // dl
  unsigned __int8 v21; // bl
  _BYTE *v22; // eax
  _BYTE *j; // ecx
  _BYTE *v24; // esi
  int v26; // [esp+Ch] [ebp-2C10h] BYREF
  int v27; // [esp+10h] [ebp-2C0Ch]
  int v28; // [esp+14h] [ebp-2C08h]
  int v29; // [esp+18h] [ebp-2C04h]
  unsigned int v30; // [esp+1Ch] [ebp-2C00h]
  int v31; // [esp+20h] [ebp-2BFCh]
  _BYTE *v32; // [esp+24h] [ebp-2BF8h]
  unsigned __int8 *v33; // [esp+28h] [ebp-2BF4h] BYREF
  _BYTE v34[4]; // [esp+2Ch] [ebp-2BF0h] BYREF
  char v35; // [esp+30h] [ebp-2BECh] BYREF
  char v36; // [esp+1430h] [ebp-17ECh] BYREF
  _BYTE v37[1036]; // [esp+280Ch] [ebp-410h] BYREF

  v28 = a2;
  v31 = 0;
  for ( i = 0; i < 0x100; ++i )
  {
    v5 = sub_49ACC0(this, &v33);
    v31 += v5;
    this = &this[(_DWORD)v33];
    *(_DWORD *)&v37[4 * i + 12] = v5;
  }
  v33 = this;
  v6 = 0;
  v7 = &v35;
  do
  {
    v8 = *(_DWORD *)&v37[4 * v6 + 12];
    *v7 = v6;
    *((_DWORD *)v7 - 1) = v8 != 0;
    *((_DWORD *)v7 + 1) = v8;
    *((_DWORD *)v7 + 2) = 0;
    *((_DWORD *)v7 + 3) = 0;
    ++v6;
    v7 += 20;
  }
  while ( v6 < 0x100 );
  v9 = &v36;
  v10 = 255;
  do
  {
    *((_DWORD *)v9 - 1) = 0;
    *v9 = 0;
    *((_DWORD *)v9 + 1) = 0;
    *((_DWORD *)v9 + 2) = 0;
    *((_DWORD *)v9 + 3) = 0;
    v9 += 20;
    --v10;
  }
  while ( v10 );
  v30 = 510;
  v32 = v37;
  do
  {
    v11 = &v26;
    v29 = 2;
    do
    {
      v12 = -1;
      *v11 = 0;
      v13 = v34;
      v14 = 511;
      do
      {
        if ( *v13 && v13[2] < v12 )
        {
          v12 = v13[2];
          *v11 = (int)v13;
        }
        v13 += 5;
        --v14;
      }
      while ( v14 );
      if ( *v11 )
        *(_DWORD *)*v11 = 0;
      ++v11;
      --v29;
    }
    while ( v29 );
    v15 = v26;
    v16 = 0;
    if ( v26 )
      v16 = *(_DWORD *)(v26 + 8);
    v17 = v27;
    if ( v27 )
      v16 += *(_DWORD *)(v27 + 8);
    v18 = v32;
    *((_DWORD *)v32 - 2) = v16 != 0;
    *v18 = v16;
    v18[1] = v17;
    v18[2] = v15;
    if ( v16 == v31 )
      break;
    --v30;
    v32 = v18 - 5;
  }
  while ( v30 >= 0x100 );
  v19 = 0;
  v20 = 0;
  v21 = 0;
  if ( a3 )
  {
    v32 = &v34[20 * v30];
    do
    {
      v22 = v32;
      for ( j = (_BYTE *)*((_DWORD *)v32 + 3); j; v21 >>= 1 )
      {
        v24 = (_BYTE *)*((_DWORD *)v22 + 4);
        if ( !v24 )
          break;
        if ( !v21 )
        {
          v20 = *v33++;
          v21 = 0x80;
        }
        v22 = v24;
        if ( (v20 & 1) == 0 )
          v22 = j;
        j = (_BYTE *)*((_DWORD *)v22 + 3);
        v20 >>= 1;
      }
      *(_BYTE *)(v28 + v19++) = v22[4];
    }
    while ( v19 < a3 );
  }
  return 1;
}

// ===== sub_49A7D0 @ 0x0049A7D0..0x0049A855 =====
int __stdcall sub_49A7D0(int a1, size_t a2, int a3, size_t a4)
{
  size_t v4; // esi
  size_t i; // edi
  size_t v6; // eax
  size_t v7; // esi
  size_t v8; // eax
  int v9; // esi
  size_t v10; // ebx
  size_t v11; // edi
  size_t Size; // [esp+8h] [ebp-8h]
  int v14; // [esp+Ch] [ebp-4h] BYREF

  v4 = 0;
  for ( i = 0; v4 < a4; i = v10 + v11 )
  {
    if ( i >= a2 )
      break;
    v6 = sub_49ACC0(v4 + a3, &v14);
    v7 = v14 + v4;
    Size = v6;
    v8 = sub_49ACC0(v7 + a3, &v14);
    v9 = v14 + v7;
    v10 = v8;
    memset((void *)(i + a1), 128, Size);
    v11 = Size + i;
    memcpy_0((void *)(v11 + a1), (const void *)(v9 + a3), v10);
    v4 = v10 + v9;
  }
  return 1;
}

// ===== sub_49A860 @ 0x0049A860..0x0049AB08 =====
int __userpurge sub_49A860@<eax>(int a1@<edi>, unsigned int a2, unsigned int a3, unsigned int a4)
{
  int v4; // esi
  unsigned int v5; // ecx
  __int64 v6; // rax
  unsigned int v7; // ebx
  int v8; // ebx
  int v9; // ebx
  unsigned int v11; // ebx
  int v12; // ebx
  int v13; // ebx
  char v14; // bl
  char v15; // bl
  int v16; // [esp+8h] [ebp-10h]
  unsigned int v17; // [esp+Ch] [ebp-Ch]
  unsigned int v18; // [esp+10h] [ebp-8h]
  unsigned int v19; // [esp+10h] [ebp-8h]
  unsigned __int8 *v20; // [esp+14h] [ebp-4h]
  unsigned int v21; // [esp+28h] [ebp+10h]
  unsigned int v22; // [esp+28h] [ebp+10h]

  v4 = 0;
  v17 = a4 >> 3;
  v6 = sub_49AD10(a2);
  v16 = v6 - a2 * (a4 >> 3);
  LODWORD(v6) = HIDWORD(v6) - 4 * a2;
  v20 = (unsigned __int8 *)v6;
  if ( a4 == 24 )
  {
    v22 = v5;
    if ( a3 <= v5 )
      return 1;
    while ( 1 )
    {
      v11 = 0;
      v19 = 0;
      if ( a2 )
        break;
LABEL_37:
      v4 += v16;
      if ( ++v22 >= a3 )
        return 1;
      LODWORD(v6) = v20;
    }
    while ( v11 )
    {
      v12 = *(unsigned __int8 *)(v5 + HIDWORD(v6) - 4);
      if ( !v22 )
      {
        *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)(v4 + a1) + v12;
        *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v4 + a1 + 1) + *(_BYTE *)(v5 + HIDWORD(v6) - 3);
        v14 = *(_BYTE *)(v4 + a1 + 2) + *(_BYTE *)(v5 + HIDWORD(v6) - 2);
LABEL_34:
        *(_BYTE *)(v5 + HIDWORD(v6) + 2) = v14;
        goto LABEL_35;
      }
      v13 = *(unsigned __int8 *)v6 + v12;
      LODWORD(v6) = *(unsigned __int8 *)(v5 + HIDWORD(v6) - 3);
      *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)(v4 + a1) + (v13 >> 1);
      *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v4 + a1 + 1) + ((v20[1] + (int)v6) >> 1);
      *(_BYTE *)(v5 + HIDWORD(v6) + 2) = *(_BYTE *)(v4 + a1 + 2)
                                       + ((v20[2] + *(unsigned __int8 *)(v5 + HIDWORD(v6) - 2)) >> 1);
      LODWORD(v6) = v20;
LABEL_35:
      *(_BYTE *)(v5 + HIDWORD(v6) + 3) = 0;
LABEL_36:
      v4 += v17;
      v11 = v19 + 1;
      v5 += 4;
      v20 = (unsigned __int8 *)(v6 + 4);
      v19 = v11;
      if ( v11 >= a2 )
        goto LABEL_37;
      LODWORD(v6) = v6 + 4;
    }
    if ( v19 )
      goto LABEL_36;
    v15 = *(_BYTE *)(v4 + a1);
    if ( v22 )
    {
      *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)v6 + v15;
      *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v6 + 1) + *(_BYTE *)(v4 + a1 + 1);
      v14 = *(_BYTE *)(v6 + 2) + *(_BYTE *)(v4 + a1 + 2);
    }
    else
    {
      *(_BYTE *)(v5 + HIDWORD(v6)) = v15;
      *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v4 + a1 + 1);
      v14 = *(_BYTE *)(v4 + a1 + 2);
    }
    goto LABEL_34;
  }
  if ( a4 != 32 )
    return 1;
  v21 = v5;
  if ( a3 <= v5 )
    return 1;
  while ( 1 )
  {
    v7 = 0;
    v18 = 0;
    if ( a2 )
    {
      while ( 1 )
      {
        if ( v7 )
        {
          v8 = *(unsigned __int8 *)(v5 + HIDWORD(v6) - 4);
          if ( v21 )
          {
            v9 = *(unsigned __int8 *)v6 + v8;
            LODWORD(v6) = *(unsigned __int8 *)(v5 + HIDWORD(v6) - 3);
            *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)(v4 + a1) + (v9 >> 1);
            *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v4 + a1 + 1) + ((v20[1] + (int)v6) >> 1);
            *(_BYTE *)(v5 + HIDWORD(v6) + 2) = *(_BYTE *)(v4 + a1 + 2)
                                             + ((v20[2] + *(unsigned __int8 *)(v5 + HIDWORD(v6) - 2)) >> 1);
            *(_BYTE *)(v5 + HIDWORD(v6) + 3) = *(_BYTE *)(v4 + a1 + 3)
                                             + ((v20[3] + *(unsigned __int8 *)(v5 + HIDWORD(v6) - 1)) >> 1);
            LODWORD(v6) = v20;
          }
          else
          {
            *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)(v4 + a1) + v8;
            *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v4 + a1 + 1) + *(_BYTE *)(v5 + HIDWORD(v6) - 3);
            *(_BYTE *)(v5 + HIDWORD(v6) + 2) = *(_BYTE *)(v4 + a1 + 2) + *(_BYTE *)(v5 + HIDWORD(v6) - 2);
            *(_BYTE *)(v5 + HIDWORD(v6) + 3) = *(_BYTE *)(v4 + a1 + 3) + *(_BYTE *)(v5 + HIDWORD(v6) - 1);
          }
        }
        else if ( !v18 )
        {
          if ( v21 )
          {
            *(_BYTE *)(v5 + HIDWORD(v6)) = *(_BYTE *)v6 + *(_BYTE *)(v4 + a1);
            *(_BYTE *)(v5 + HIDWORD(v6) + 1) = *(_BYTE *)(v6 + 1) + *(_BYTE *)(v4 + a1 + 1);
            *(_BYTE *)(v5 + HIDWORD(v6) + 2) = *(_BYTE *)(v6 + 2) + *(_BYTE *)(v4 + a1 + 2);
            *(_BYTE *)(v5 + HIDWORD(v6) + 3) = *(_BYTE *)(v6 + 3) + *(_BYTE *)(v4 + a1 + 3);
          }
          else
          {
            *(_DWORD *)(v5 + HIDWORD(v6)) = *(_DWORD *)(v4 + a1);
          }
        }
        v4 += v17;
        v7 = v18 + 1;
        v5 += 4;
        v20 = (unsigned __int8 *)(v6 + 4);
        v18 = v7;
        if ( v7 >= a2 )
          break;
        LODWORD(v6) = v6 + 4;
      }
    }
    if ( ++v21 >= a3 )
      break;
    LODWORD(v6) = v20;
  }
  return 1;
}

// ===== sub_49AB10 @ 0x0049AB10..0x0049ACC0 =====
int __stdcall sub_49AB10(
        char *Src,
        size_t Size,
        int a3,
        unsigned int a4,
        unsigned int a5,
        unsigned int a6,
        unsigned int a7)
{
  unsigned int v7; // esi
  unsigned int v8; // ebx
  unsigned int v10; // edx
  int v11; // eax
  unsigned int v12; // edi
  int v13; // esi
  char *v14; // esi
  int v15; // edi
  int v16; // eax
  int v18; // [esp+Ch] [ebp-44h]
  int v19; // [esp+10h] [ebp-40h]
  int v20; // [esp+14h] [ebp-3Ch]
  _DWORD v21[2]; // [esp+18h] [ebp-38h] BYREF
  int v22; // [esp+20h] [ebp-30h]
  void *v23; // [esp+24h] [ebp-2Ch]
  unsigned int v24; // [esp+28h] [ebp-28h]
  unsigned int i; // [esp+2Ch] [ebp-24h]
  unsigned int v26; // [esp+30h] [ebp-20h]
  int v27; // [esp+34h] [ebp-1Ch]
  int v28; // [esp+38h] [ebp-18h]
  unsigned int v29; // [esp+3Ch] [ebp-14h]
  int v30; // [esp+40h] [ebp-10h]
  int v31; // [esp+44h] [ebp-Ch]
  size_t v32; // [esp+48h] [ebp-8h]
  unsigned __int8 v33; // [esp+4Fh] [ebp-1h]
  unsigned __int8 v34; // [esp+73h] [ebp+23h]

  v24 = (a6 + 7) >> 3;
  v7 = (((a5 + 7) >> 3) * v24 + 7) >> 3;
  v26 = (a5 + 7) >> 3;
  v22 = v7 + a3;
  v8 = v7 + 8 * sub_49AE00();
  v23 = operator new[](Size);
  sub_49AD30(v23, Src, Size);
  v10 = a5;
  v11 = 0;
  v12 = a7 >> 3;
  v32 = 0;
  v28 = 0;
  v31 = 0;
  v21[1] = a7 >> 3;
  v34 = 0;
  v33 = 0;
  if ( a7 == 24 )
    v12 = 4;
  v19 = 8 * v12;
  for ( i = 0; i < v24; v11 = v13 )
  {
    if ( v8 >= a4 || v32 >= Size )
      break;
    v13 = v11 + 8;
    v18 = v11 + 8;
    if ( v11 + 8 <= a6 )
      v27 = 8;
    else
      v27 = a6 - v11;
    v29 = 0;
    v30 = 0;
    if ( v26 )
    {
      v14 = &Src[v32];
      v15 = (_BYTE *)v23 - Src;
      v20 = (_BYTE *)v23 - Src;
      do
      {
        if ( v8 >= a4 )
          break;
        if ( !v33 )
        {
          v34 = *(_BYTE *)(v28 + a3);
          ++v28;
          v33 = 0x80;
        }
        if ( (v34 & 1) != 0 )
        {
          if ( v30 + 8 <= v10 )
            v16 = 8;
          else
            v16 = v10 - v30;
          sub_49AE50(&v14[v15], v22 + v31, v16, v27, 4 * v10, v21);
          v8 += v21[0];
          v31 += 8;
          v10 = a5;
          v15 = v20;
        }
        ++v29;
        v14 += v19;
        v34 >>= 1;
        v33 >>= 1;
        v30 += 8;
      }
      while ( v29 < v26 );
      v13 = v18;
    }
    v32 += 32 * v10;
    ++i;
  }
  operator delete[](v23);
  return 1;
}

// ===== sub_49ACC0 @ 0x0049ACC0..0x0049AD07 =====
int __stdcall sub_49ACC0(char *a1, int *a2)
{
  int v2; // ecx
  int result; // eax
  int v4; // esi
  char v5; // dl

  v2 = 0;
  result = *a1 & 0x7F;
  v4 = 1;
  if ( *a1 < 0 )
  {
    do
    {
      v5 = a1[v4];
      v2 += 7;
      ++v4;
      result |= (v5 & 0x7F) << v2;
    }
    while ( v5 < 0 );
  }
  if ( a2 )
    *a2 = v4;
  return result;
}

// ===== sub_49AD10 @ 0x0049AD10..0x0049AD24 =====
unsigned int __userpurge sub_49AD10@<eax>(unsigned int a1@<eax>, int a2)
{
  return (a2 * (a1 >> 3) + 3) & 0xFFFFFFFC;
}

// ===== sub_49AD30 @ 0x0049AD30..0x0049ADF1 =====
void *__stdcall sub_49AD30(__m64 *a1, __m64 *Src, size_t Size)
{
  void *result; // eax
  size_t v4; // ecx
  float *v5; // edi
  __m128 *v6; // esi
  size_t v7; // ecx
  __m64 *v8; // edi
  __m64 *v9; // esi
  size_t v10; // ecx
  __m64 *v11; // edi
  __m64 *v12; // esi
  size_t Sizea; // [esp+10h] [ebp+10h]

  result = (void *)Size;
  if ( Size )
  {
    if ( (Size & 7) != 0 )
    {
      return memcpy_0(a1, Src, Size);
    }
    else if ( !dword_566A98
           || (Size & 0xF) != 0
           || ((unsigned __int8)a1 & 0xF) != 0
           || ((unsigned __int8)Src & 0xF) != 0 )
    {
      result = (void *)(Size >> 3);
      Sizea = Size >> 3;
      if ( dword_566A94 )
      {
        v7 = Sizea;
        v8 = a1;
        v9 = Src;
        do
        {
          _mm_prefetch((const char *)&v9[64], 0);
          _mm_stream_pi(v8, (__m64)v9->m64_u64);
          ++v9;
          ++v8;
          --v7;
        }
        while ( v7 );
      }
      else
      {
        v10 = Sizea;
        v11 = a1;
        v12 = Src;
        do
        {
          v11->m64_u64 = v12->m64_u64;
          ++v12;
          ++v11;
          --v10;
        }
        while ( v10 );
      }
      _m_empty();
    }
    else
    {
      result = (void *)(Size >> 4);
      v4 = Size >> 4;
      v5 = (float *)a1;
      v6 = (__m128 *)Src;
      do
      {
        _mm_prefetch((const char *)&v6[32], 0);
        _mm_stream_ps(v5, *v6++);
        v5 += 4;
        --v4;
      }
      while ( v4 );
    }
  }
  return result;
}

// ===== sub_49AE00 @ 0x0049AE00..0x0049AE4D =====
int __usercall sub_49AE00@<eax>(int a1@<edi>, unsigned int a2@<esi>)
{
  __int64 v2; // rax
  unsigned __int8 v3; // cl
  unsigned __int8 v4; // cl
  unsigned __int8 v5; // cl
  unsigned __int8 v6; // cl
  unsigned __int8 v7; // cl
  unsigned __int8 v8; // cl
  char v9; // cl

  v2 = 0LL;
  if ( a2 )
  {
    do
    {
      v3 = *(_BYTE *)(HIDWORD(v2) + a1);
      if ( (v3 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v4 = v3 >> 1;
      if ( (v4 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v5 = v4 >> 1;
      if ( (v5 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v6 = v5 >> 1;
      if ( (v6 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v7 = v6 >> 1;
      if ( (v7 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v8 = v7 >> 1;
      if ( (v8 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      v9 = v8 >> 1;
      if ( (v9 & 1) != 0 )
        LODWORD(v2) = v2 + 1;
      if ( (v9 & 2) != 0 )
        LODWORD(v2) = v2 + 1;
      ++HIDWORD(v2);
    }
    while ( HIDWORD(v2) < a2 );
  }
  return v2;
}

// ===== sub_49AE50 @ 0x0049AE50..0x0049B013 =====
int __userpurge sub_49AE50@<eax>(
        int a1@<ecx>,
        int a2@<edi>,
        int a3@<esi>,
        int a4,
        int a5,
        int a6,
        unsigned int a7,
        int a8,
        _DWORD *a9)
{
  int v9; // edx
  int v10; // eax
  int v11; // ecx
  unsigned int v12; // ecx
  int v13; // ecx
  char v14; // bl
  unsigned int v16; // ecx
  int v17; // ecx
  char v18; // bl
  int v19; // [esp+4h] [ebp-14h]
  int v20; // [esp+4h] [ebp-14h]
  int v21; // [esp+8h] [ebp-10h]
  int v22; // [esp+8h] [ebp-10h]
  unsigned int v23; // [esp+Ch] [ebp-Ch]
  unsigned int i; // [esp+Ch] [ebp-Ch]
  int v25; // [esp+10h] [ebp-8h]
  unsigned __int8 v26; // [esp+17h] [ebp-1h]
  unsigned __int8 v27; // [esp+17h] [ebp-1h]

  v9 = a4;
  v10 = 0;
  v11 = a1 - 3;
  v25 = 0;
  if ( !v11 )
  {
    v16 = 0;
    for ( i = 0; v16 < a7; i = v16 )
    {
      v27 = *(_BYTE *)(v16 + a5);
      v17 = v25;
      if ( a6 )
      {
        v22 = v9 - a3;
        v20 = a6;
        do
        {
          if ( (v27 & 1) != 0 )
          {
            *(_BYTE *)(v17 + a3) = *(_BYTE *)(v10 + a2) + *(_BYTE *)(v22 + v17 + a3);
            v9 = a4;
            *(_BYTE *)(v17 + a3 + 1) = *(_BYTE *)(v10 + a2 + 1) + *(_BYTE *)(v17 + a4 + 1);
            v18 = *(_BYTE *)(v10 + a2 + 2) + *(_BYTE *)(v17 + a4 + 2);
            *(_BYTE *)(v17 + a3 + 3) = 0;
            *(_BYTE *)(v17 + a3 + 2) = v18;
            v10 += 3;
          }
          else if ( *(_BYTE *)(v10 + a2) == 0x80 )
          {
            *(_DWORD *)(v17 + a3) = 0;
            ++v10;
          }
          else
          {
            v9 = a4;
            *(_DWORD *)(v17 + a3) = *(_DWORD *)(v17 + a8 * *(char *)(v10 + a2 + 1) + 4 * *(char *)(v10 + a2) + a4);
            v10 += 2;
          }
          v27 >>= 1;
          v17 += 4;
          --v20;
        }
        while ( v20 );
      }
      v25 += a8;
      v16 = i + 1;
    }
    goto LABEL_24;
  }
  v12 = v11 - 1;
  if ( v12 || (v23 = 0, !a7) )
  {
LABEL_24:
    *a9 = v10;
    return 1;
  }
  do
  {
    v26 = *(_BYTE *)(v12 + a5);
    v13 = v25;
    if ( a6 )
    {
      v21 = v9 - a3;
      v19 = a6;
      do
      {
        if ( (v26 & 1) != 0 )
        {
          *(_BYTE *)(v13 + a3) = *(_BYTE *)(v10 + a2) + *(_BYTE *)(v21 + v13 + a3);
          v9 = a4;
          *(_BYTE *)(v13 + a3 + 1) = *(_BYTE *)(v10 + a2 + 1) + *(_BYTE *)(v13 + a4 + 1);
          *(_BYTE *)(v13 + a3 + 2) = *(_BYTE *)(v10 + a2 + 2) + *(_BYTE *)(v13 + a4 + 2);
          v14 = *(_BYTE *)(v10 + a2 + 3) + *(_BYTE *)(v13 + a4 + 3);
          v10 += 4;
          *(_BYTE *)(v13 + a3 + 3) = v14;
        }
        else if ( *(_BYTE *)(v10 + a2) == 0x80 )
        {
          *(_DWORD *)(v13 + a3) = 0;
          ++v10;
        }
        else
        {
          v9 = a4;
          *(_DWORD *)(v13 + a3) = *(_DWORD *)(v13 + a8 * *(char *)(v10 + a2 + 1) + 4 * *(char *)(v10 + a2) + a4);
          v10 += 2;
        }
        v26 >>= 1;
        v13 += 4;
        --v19;
      }
      while ( v19 );
    }
    v25 += a8;
    v12 = v23 + 1;
    v23 = v12;
  }
  while ( v12 < a7 );
  *a9 = v10;
  return 1;
}

// ===== sub_49B020 @ 0x0049B020..0x0049B026 =====
int __usercall sub_49B020@<eax>(int result@<eax>)
{
  dword_566A94 = result;
  return result;
}

// ===== sub_49B030 @ 0x0049B030..0x0049B036 =====
int __usercall sub_49B030@<eax>(int result@<eax>)
{
  dword_566A98 = result;
  return result;
}

// ===== sub_49B040 @ 0x0049B040..0x0049B1CA =====
_DWORD *__usercall sub_49B040@<eax>(_DWORD *result@<eax>, int a2@<ecx>)
{
  bool v2; // zf
  int v3; // ecx
  double v4; // st2
  int v5; // ecx
  int v6; // edx
  double v7; // st2
  int i; // ecx
  int v9; // [esp+0h] [ebp-4h]
  int v10; // [esp+0h] [ebp-4h]

  v2 = dword_566AA0 == 0;
  *result = &YCBFMovie2::`vftable';
  result[4] = a2;
  if ( v2 )
  {
    v3 = 0;
    v9 = 0;
    do
    {
      v4 = (double)v9;
      v9 = ++v3;
      flt_56268C[v3] = v4 * 0.299;
      flt_562A8C[v3] = v4 * 0.587;
      flt_562E8C[v3] = v4 * 0.114 + 0.5;
      flt_56328C[v3] = -0.16874 * v4;
      flt_56368C[v3] = -0.33126 * v4;
      flt_563A8C[v3] = v4 * 0.5 + 128.0 + 0.5;
      flt_563E8C[v3] = v4 * 0.5;
      flt_56428C[v3] = -0.41869 * v4;
      flt_56468C[v3] = 128.0 - v4 * 0.08130999999999999 + 0.5;
    }
    while ( v3 < 256 );
    v10 = -128;
    v5 = 0;
    v6 = 256;
    do
    {
      v7 = (double)v10++;
      ++v5;
      --v6;
      *(float *)&dword_56138C[v5] = v7 * 1.402 + 0.5;
      flt_56178C[v5] = v7 * -0.34414;
      flt_561B8C[v5] = 0.5 - v7 * 0.71414;
      flt_561F8C[v5] = v7 * 1.772 + 0.5;
      flt_564A8C[v5] = 0.0;
      flt_564E8C[v5] = 0.0;
      flt_56528C[v5] = 0.0;
      flt_56568C[v5] = 0.0;
    }
    while ( v6 );
    for ( i = 0; i < 256; ++i )
    {
      byte_562390[i] = 0;
      byte_562490[i] = i;
      byte_562590[i] = -1;
    }
    dword_566AA0 = 1;
  }
  return result;
}

// ===== sub_49B1D0 @ 0x0049B1D0..0x0049B1F1 =====
void *__thiscall sub_49B1D0(void *this, char a2)
{
  sub_49B200();
  if ( (a2 & 1) != 0 )
    operator delete(this);
  return this;
}

// ===== sub_49B200 @ 0x0049B200..0x0049B207 =====
void __thiscall sub_49B200(_DWORD *this)
{
  *this = &YCBFMovie2::`vftable';
}

// ===== sub_49B210 @ 0x0049B210..0x0049B269 =====
int __usercall sub_49B210@<eax>(int a1@<esi>)
{
  int result; // eax
  unsigned int v2; // ecx

  result = 0;
  if ( strcmp((const char *)a1, "BF_Movie_______") )
    return 8;
  v2 = *(_DWORD *)(a1 + 16);
  if ( v2 < 0x10000 || v2 > 0x10001 )
    return 9;
  return result;
}

// ===== sub_49B270 @ 0x0049B270..0x0049B276 =====
int __usercall sub_49B270@<eax>(int result@<eax>)
{
  dword_566A9C = result;
  return result;
}

// ===== sub_49B280 @ 0x0049B280..0x0049B286 =====
int sub_49B280()
{
  return dword_566A9C;
}

// ===== sub_49B290 @ 0x0049B290..0x0049B465 =====
_BYTE *__usercall sub_49B290@<eax>(int a1@<eax>, int a2@<esi>)
{
  _BYTE *v2; // ecx
  float *v3; // edi
  double v4; // st7
  int v5; // ebx
  int v6; // edx
  char v7; // al
  char v8; // al
  double v9; // st4
  _BYTE *result; // eax
  unsigned int v11; // ecx
  int v12; // edx
  double v13; // st7
  unsigned __int8 v14; // al
  int v15; // [esp+8h] [ebp-8h]
  int v16; // [esp+8h] [ebp-8h]

  if ( a1 < 50 )
  {
    v11 = 0;
    v12 = a2 + 432;
    v13 = (double)(2 * a1) / 100.0;
    do
    {
      v16 = 255 - (unsigned __int8)byte_503C28[v11++];
      v12 += 4;
      v14 = (int)(255.0 - (double)v16 * v13);
      *(_BYTE *)(a2 + v11 + 31) = v14;
      *(float *)(v12 - 260) = 1.0 / ((double)v14 * flt_503CA4[v11] * 8.0);
      LOBYTE(result) = (int)(255.0 - (double)(255 - (unsigned __int8)byte_503C67[v11]) * v13);
      *(_BYTE *)(a2 + v11 + 95) = (_BYTE)result;
      result = (_BYTE *)(unsigned __int8)result;
      *(float *)(v12 - 4) = 1.0 / ((double)(unsigned __int8)result * flt_503CA4[v11] * 8.0);
    }
    while ( v11 < 0x40 );
  }
  else
  {
    v2 = (_BYTE *)(a2 + 96);
    v3 = (float *)&unk_503CA8;
    v4 = (double)(2 * (100 - a1)) / 100.0;
    v5 = a2 + 432;
    v6 = -96 - a2;
    do
    {
      v7 = (int)((double)(unsigned __int8)byte_503C28[(_DWORD)&v2[v6]] * v4);
      *(v2 - 64) = v7;
      if ( !v7 )
        *(v2 - 64) = 1;
      *(float *)(v5 - 256) = 1.0 / ((double)(unsigned __int8)*(v2 - 64) * *v3 * 8.0);
      v8 = (int)((double)(unsigned __int8)byte_503C68[(_DWORD)&v2[v6]] * v4);
      *v2 = v8;
      if ( !v8 )
        *v2 = 1;
      v9 = *v3;
      v15 = (unsigned __int8)*v2++;
      result = &v2[v6];
      ++v3;
      v5 += 4;
      *(float *)(v5 - 4) = 1.0 / (v9 * (double)v15 * 8.0);
    }
    while ( (unsigned int)&v2[v6] < 0x40 );
  }
  return result;
}

// ===== sub_49B470 @ 0x0049B470..0x0049B534 =====
int __usercall sub_49B470@<eax>(int a1@<eax>, int a2@<edx>)
{
  int result; // eax
  float *v3; // ecx
  int v4; // edx
  int v5; // esi
  double v6; // st7
  int v7; // [esp+8h] [ebp-4h]

  result = a1 + 1;
  v3 = (float *)&unk_503CAC;
  v4 = a2 + 180;
  v5 = 8;
  do
  {
    v6 = *(v3 - 1);
    v7 = *(unsigned __int8 *)(result - 1);
    result += 8;
    v3 += 8;
    v4 += 32;
    --v5;
    *(float *)(v4 - 36) = v6 * (double)v7;
    *(float *)(v4 - 32) = (double)*(unsigned __int8 *)(result - 8) * *(v3 - 8);
    *(float *)(v4 - 28) = *(v3 - 7) * (double)*(unsigned __int8 *)(result - 7);
    *(float *)(v4 - 24) = *(v3 - 6) * (double)*(unsigned __int8 *)(result - 6);
    *(float *)(v4 - 20) = *(v3 - 5) * (double)*(unsigned __int8 *)(result - 5);
    *(float *)(v4 - 16) = *(v3 - 4) * (double)*(unsigned __int8 *)(result - 4);
    *(float *)(v4 - 12) = *(v3 - 3) * (double)*(unsigned __int8 *)(result - 3);
    *(float *)(v4 - 8) = *(v3 - 2) * (double)*(unsigned __int8 *)(result - 2);
  }
  while ( v5 );
  return result;
}

// ===== sub_49B540 @ 0x0049B540..0x0049B544 =====
int __usercall sub_49B540@<eax>(int a1@<eax>)
{
  return a1 + 32;
}

// ===== sub_49B550 @ 0x0049B550..0x0049B614 =====
int __usercall sub_49B550@<eax>(int a1@<eax>, int a2@<edx>)
{
  int result; // eax
  float *v3; // ecx
  int v4; // edx
  int v5; // esi
  double v6; // st7
  int v7; // [esp+8h] [ebp-4h]

  result = a1 + 1;
  v3 = (float *)&unk_503CAC;
  v4 = a2 + 436;
  v5 = 8;
  do
  {
    v6 = *(v3 - 1);
    v7 = *(unsigned __int8 *)(result - 1);
    result += 8;
    v3 += 8;
    v4 += 32;
    --v5;
    *(float *)(v4 - 36) = v6 * (double)v7;
    *(float *)(v4 - 32) = (double)*(unsigned __int8 *)(result - 8) * *(v3 - 8);
    *(float *)(v4 - 28) = *(v3 - 7) * (double)*(unsigned __int8 *)(result - 7);
    *(float *)(v4 - 24) = *(v3 - 6) * (double)*(unsigned __int8 *)(result - 6);
    *(float *)(v4 - 20) = *(v3 - 5) * (double)*(unsigned __int8 *)(result - 5);
    *(float *)(v4 - 16) = *(v3 - 4) * (double)*(unsigned __int8 *)(result - 4);
    *(float *)(v4 - 12) = *(v3 - 3) * (double)*(unsigned __int8 *)(result - 3);
    *(float *)(v4 - 8) = *(v3 - 2) * (double)*(unsigned __int8 *)(result - 2);
  }
  while ( v5 );
  return result;
}

// ===== sub_49B620 @ 0x0049B620..0x0049B624 =====
int __usercall sub_49B620@<eax>(int a1@<eax>)
{
  return a1 + 96;
}

// ===== sub_49B630 @ 0x0049B630..0x0049B63B =====
int __usercall sub_49B630@<eax>(int a1@<eax>)
{
  return 4 * *(_DWORD *)(a1 + 40) + 128;
}

// ===== sub_49B640 @ 0x0049B640..0x0049B683 =====
int __usercall sub_49B640@<eax>(int a1@<eax>, unsigned int a2@<ecx>, int a3@<esi>, _DWORD *a4, int a5)
{
  if ( a2 >= *(_DWORD *)(a3 + 40) )
    return 0;
  *a4 = *(_DWORD *)(a5 + 4 * a2 + 128);
  if ( a2 + 1 < *(_DWORD *)(a3 + 40) )
    a1 = *(_DWORD *)(a5 + 4 * a2 + 132);
  a4[1] = a1 - *(_DWORD *)(a5 + 4 * a2 + 128);
  return 1;
}

// ===== sub_49B690 @ 0x0049B690..0x0049C7AF =====
int __fastcall sub_49B690(unsigned int *a1, _DWORD *a2, int a3, int a4)
{
  int v5; // ecx
  unsigned int v6; // esi
  char *v7; // eax
  char *v8; // esi
  unsigned int v9; // edi
  int v10; // edi
  int v11; // esi
  unsigned int v12; // edi
  char *v13; // esi
  _DWORD *v14; // eax
  _DWORD *v15; // esi
  int v16; // eax
  int v17; // eax
  int v18; // ecx
  unsigned int v19; // edx
  int v20; // edx
  int v21; // ecx
  int j; // eax
  char *v23; // edi
  char *v24; // edx
  int v25; // eax
  int v26; // eax
  unsigned int v27; // ecx
  int v28; // ecx
  unsigned int v29; // edi
  unsigned int v30; // eax
  unsigned int v31; // eax
  size_t v32; // edi
  unsigned int v33; // esi
  unsigned int v34; // esi
  size_t v35; // edi
  int v36; // ecx
  unsigned int v37; // edi
  int v38; // edx
  int v39; // eax
  _DWORD *v40; // edi
  unsigned int v41; // eax
  char *v42; // edx
  unsigned int v43; // ecx
  size_t v44; // edi
  _BYTE *v45; // eax
  _BYTE *v46; // esi
  int v47; // ecx
  _DWORD *v48; // eax
  size_t v49; // eax
  int v51; // [esp+0h] [ebp-37A4h] BYREF
  int pExceptionObject; // [esp+14h] [ebp-3790h] BYREF
  int v53; // [esp+18h] [ebp-378Ch] BYREF
  int v54; // [esp+1Ch] [ebp-3788h] BYREF
  int v55; // [esp+20h] [ebp-3784h] BYREF
  void *v56; // [esp+24h] [ebp-3780h]
  size_t v57; // [esp+28h] [ebp-377Ch]
  void *v58; // [esp+2Ch] [ebp-3778h]
  void *v59; // [esp+30h] [ebp-3774h]
  unsigned int v60; // [esp+34h] [ebp-3770h]
  int v61; // [esp+38h] [ebp-376Ch]
  void *v62; // [esp+3Ch] [ebp-3768h]
  unsigned int *v63; // [esp+40h] [ebp-3764h] BYREF
  void *v64; // [esp+44h] [ebp-3760h]
  int v65; // [esp+48h] [ebp-375Ch]
  int v66; // [esp+4Ch] [ebp-3758h]
  size_t Size; // [esp+50h] [ebp-3754h]
  void *v68; // [esp+54h] [ebp-3750h]
  void *v69; // [esp+58h] [ebp-374Ch]
  void *v70; // [esp+5Ch] [ebp-3748h]
  void *v71; // [esp+60h] [ebp-3744h]
  _DWORD *v72; // [esp+64h] [ebp-3740h]
  unsigned int v73; // [esp+68h] [ebp-373Ch]
  int v74; // [esp+6Ch] [ebp-3738h]
  unsigned int i; // [esp+70h] [ebp-3734h]
  unsigned int v76; // [esp+74h] [ebp-3730h]
  void *Block; // [esp+78h] [ebp-372Ch]
  unsigned int v78; // [esp+7Ch] [ebp-3728h]
  int v79; // [esp+80h] [ebp-3724h]
  void *v80; // [esp+84h] [ebp-3720h]
  unsigned int v81; // [esp+88h] [ebp-371Ch]
  int v82; // [esp+8Ch] [ebp-3718h]
  size_t v83; // [esp+90h] [ebp-3714h] BYREF
  int v84; // [esp+94h] [ebp-3710h] BYREF
  unsigned int v85; // [esp+98h] [ebp-370Ch]
  int v86; // [esp+9Ch] [ebp-3708h]
  char *v87; // [esp+A0h] [ebp-3704h]
  void *Src; // [esp+A4h] [ebp-3700h]
  _DWORD *v89; // [esp+A8h] [ebp-36FCh]
  unsigned int v90; // [esp+ACh] [ebp-36F8h]
  unsigned int v91; // [esp+B0h] [ebp-36F4h]
  unsigned int v92; // [esp+B4h] [ebp-36F0h]
  int v93; // [esp+B8h] [ebp-36ECh]
  int k; // [esp+BCh] [ebp-36E8h]
  _BYTE v95[8424]; // [esp+C0h] [ebp-36E4h] BYREF
  _BYTE v96[1024]; // [esp+21A8h] [ebp-15FCh] BYREF
  _BYTE v97[1024]; // [esp+25A8h] [ebp-11FCh] BYREF
  _BYTE v98[1024]; // [esp+29A8h] [ebp-DFCh] BYREF
  _BYTE v99[1024]; // [esp+2DA8h] [ebp-9FCh] BYREF
  _BYTE v100[744]; // [esp+31A8h] [ebp-5FCh] BYREF
  _DWORD v101[176]; // [esp+3490h] [ebp-314h] BYREF
  _DWORD v102[18]; // [esp+3750h] [ebp-54h] BYREF
  int v103; // [esp+37A0h] [ebp-4h]

  v102[17] = &v51;
  v61 = a3;
  v79 = a4;
  v63 = a1;
  v72 = a2;
  v68 = 0;
  v59 = 0;
  v58 = 0;
  v56 = 0;
  v71 = 0;
  v69 = 0;
  v80 = 0;
  v70 = 0;
  Block = 0;
  v62 = 0;
  v64 = 0;
  v103 = 0;
  v82 = 0;
  v5 = a2[5];
  v74 = 3;
  if ( v5 != 4 )
    v74 = v5;
  v6 = (a2[2] + 7) & 0xFFFFFFF8;
  v81 = v6;
  v76 = (a2[3] + 7) & 0xFFFFFFF8;
  v86 = v6 * v5;
  Size = v76 * v6 * v5;
  v84 = v6 * v76;
  v85 = v6 >> 3;
  v90 = v76 >> 3;
  v7 = (char *)operator new[](Size);
  v68 = v7;
  if ( a2[2] == v6 && a2[3] == v76 )
  {
    memcpy_0(v7, (const void *)*a2, Size);
  }
  else
  {
    v8 = v7;
    v89 = v7;
    Src = (void *)*a2;
    for ( i = 0; i < a2[3]; ++i )
    {
      memcpy_0(v8, Src, a2[1]);
      memset(&v8[a2[1]], 0, v86 - a2[1]);
      v8 += v86;
      v89 = v8;
      Src = (char *)Src + a2[1];
    }
    v9 = a2[3];
    if ( v9 < v76 )
      memset(v8, 0, v86 * (v76 - v9));
  }
  v57 = (v85 + 7) >> 3;
  v83 = (v57 + 3) & 0xFFFFFFFC;
  v78 = v90 * v83;
  v10 = v84;
  v11 = v74 * v84;
  v59 = operator new[](4 * v74 * v84);
  v12 = 2 * v10;
  v84 = (int)operator new[](v12);
  v58 = (void *)v84;
  v78 = (unsigned int)operator new[](v78);
  v56 = (void *)v78;
  v13 = (char *)operator new[](2 * v11);
  v69 = v13;
  v80 = operator new[](4 * v90);
  *(_DWORD *)(a3 + 24) = v90;
  if ( v72[5] == 4 )
  {
    v82 = 1;
    ++*(_DWORD *)(a3 + 24);
  }
  v14 = operator new[](132 * *(_DWORD *)(a3 + 24));
  *(_DWORD *)(a3 + 20) = v14;
  v89 = v68;
  v87 = v13;
  v15 = v72;
  Src = (void *)*v72;
  v66 = 0;
  v60 = v78;
  if ( v72[5] == 4 )
  {
    *v14 = 2;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 8) = Size;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 16) = v12;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 4) = v68;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 12) = v84;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 20) = v78;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 24) = Src;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 28) = v66;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 32) = v71;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 36) = v62;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 40) = v64;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 48) = v12;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 52) = v15[2];
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 56) = v15[3];
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 60) = v15[1];
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 64) = v81;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 68) = v76;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 72) = v86;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 76) = v15[5];
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 80) = v85;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + 84) = v90;
  }
  v16 = v82;
  k = v82;
  v93 = 0;
  while ( v16 < *(_DWORD *)(a3 + 24) )
  {
    v17 = 132 * v16;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20)) = 0;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 8) = 8 * v86;
    v18 = v74 * v81;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 16) = 8 * v74 * v81;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 4) = v89;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 12) = v87;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 20) = v60;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 24) = Src;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 28) = v66;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 52) = v15[2];
    v19 = v15[3];
    if ( 8 * v93 + 8 <= v19 )
      v20 = 8;
    else
      v20 = v19 - 8 * v93;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 56) = v20;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 60) = v15[1];
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 64) = v81;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 68) = 8;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 72) = v86;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 76) = v15[5];
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 80) = v85;
    *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 84) = 1;
    v89 = (_DWORD *)((char *)v89 + *(_DWORD *)(v17 + *(_DWORD *)(a3 + 20) + 8));
    v87 += 16 * v18;
    Src = (char *)Src + 8 * v15[1];
    if ( v66 )
      v66 += 8 * MEMORY[4];
    v60 += v83;
    v16 = ++k;
    ++v93;
  }
  sub_49EEE0(0);
  sub_4464C0(*(_DWORD *)(a3 + 16), (int)sub_49CBC0, a3);
  sub_4464E0(1, *(_DWORD *)(a3 + 16));
  sub_4464C0(*(_DWORD *)(a3 + 16), 0, 0);
  for ( j = v82; ; ++j )
  {
    k = j;
    v93 = v21;
    if ( j >= *(_DWORD *)(a3 + 24) )
      break;
    *((_DWORD *)v80 + v21++) = *(_DWORD *)(132 * j + *(_DWORD *)(a3 + 20) + 16);
  }
  v23 = (char *)v69;
  v87 = (char *)v69;
  memset(v102, 0, 0x40u);
  memset(v101, 0, sizeof(v101));
  for ( k = v82; k < *(_DWORD *)(a3 + 24); ++k )
  {
    v83 = *(_DWORD *)(132 * k + *(_DWORD *)(a3 + 20) + 16);
    sub_4A0110(v102, v83);
    sub_4A0150(v101, v23, v83);
    v23 += 16 * v74 * v81;
    v87 = v23;
  }
  v78 = sub_4A0360(v100);
  v84 = sub_4A0360(v95);
  v24 = (char *)v69;
  v87 = (char *)v69;
  v89 = v59;
  v25 = v82;
  k = v82;
  v93 = 0;
  while ( v25 < *(_DWORD *)(a3 + 24) )
  {
    v26 = 132 * v25;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20)) = 1;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 8) = *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 16);
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 16) = 4 * *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 8);
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 4) = v24;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 12) = v89;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 24) = 0;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 28) = 0;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 52) = v15[2];
    v27 = v15[3];
    if ( 8 * v93 + 8 <= v27 )
      v28 = 8;
    else
      v28 = v27 - 8 * v93;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 56) = v28;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 60) = v15[1];
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 64) = v81;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 68) = 8;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 72) = v86;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 76) = v15[5];
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 80) = v85;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 84) = 1;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 92) = v78;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 88) = v100;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 104) = *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 16);
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 112) = v84;
    *(_DWORD *)(v26 + *(_DWORD *)(a3 + 20) + 108) = v95;
    *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 124) = *(_DWORD *)(*(_DWORD *)(a3 + 20) + v26 + 16);
    v24 += 16 * v74 * v81;
    v87 = v24;
    v89 += 8 * v74 * v81;
    v25 = ++k;
    ++v93;
  }
  sub_49EEE0(v82);
  sub_4464C0(*(_DWORD *)(a3 + 16), (int)sub_49CBC0, a3);
  sub_4464E0(1, *(_DWORD *)(a3 + 16));
  sub_4464C0(*(_DWORD *)(a3 + 16), 0, 0);
  v92 = 0;
  v91 = 0;
  while ( v91 < 0x10 )
  {
    sub_49FA10();
    v92 += v90;
    ++v91;
    v15 = v72;
  }
  v91 = 0;
  while ( v91 < 0xB0 )
  {
    sub_49FA10();
    v92 += v90;
    ++v91;
    v15 = v72;
  }
  v73 = v92;
  v29 = v92 + 4 * *(_DWORD *)(a3 + 24);
  v92 = v29;
  v85 = v29;
  k = v82;
  v93 = 0;
  while ( k < *(_DWORD *)(a3 + 24) )
  {
    v30 = v73;
    *(_DWORD *)(v79 + v73) = v85;
    v73 = v30 + 4;
    sub_49FA10();
    v85 += v57 + v90 + *(_DWORD *)(*(_DWORD *)(a3 + 20) + 132 * k++ + 16);
    ++v93;
    v29 = v92;
    v15 = v72;
  }
  if ( v15[5] == 4 )
  {
    v31 = v73;
    *(_DWORD *)(v79 + v73) = v85;
    v73 = v31 + 4;
  }
  k = v82;
  v93 = 0;
  while ( k < *(_DWORD *)(a3 + 24) )
  {
    v83 = 132 * k;
    v32 = v57;
    v33 = v92;
    memcpy_0((void *)(v92 + v79), *(const void **)(132 * k + *(_DWORD *)(a3 + 20) + 20), v57);
    v92 = v32 + v33;
    sub_49FA10();
    v34 = v90 + v92;
    v92 = v34;
    v35 = v83;
    memcpy_0(
      (void *)(v34 + v79),
      *(const void **)(v83 + *(_DWORD *)(a3 + 20) + 12),
      *(_DWORD *)(v83 + *(_DWORD *)(a3 + 20) + 16));
    v92 = *(_DWORD *)(v35 + *(_DWORD *)(a3 + 20) + 16) + v34;
    ++k;
    ++v93;
    v29 = v92;
    v15 = v72;
  }
  if ( v15[5] == 4 )
  {
    v83 = 0;
    v36 = v79;
    *(_DWORD *)(v79 + v29) = *(_DWORD *)(*(_DWORD *)(a3 + 20) + 128);
    v37 = v29 + 4;
    v92 = v37;
    memcpy_0((void *)(v37 + v36), *(const void **)(*(_DWORD *)(a3 + 20) + 12), *(_DWORD *)(*(_DWORD *)(a3 + 20) + 16));
    v29 = *(_DWORD *)(v83 + *(_DWORD *)(a3 + 20) + 16) + v37;
    v92 = v29;
  }
  *v63 = v29;
  Block = _aligned_malloc(16 * v74 * v81, 0x10u);
  if ( !Block )
  {
    pExceptionObject = 11;
    _CxxThrowException(&pExceptionObject, (_ThrowInfo *)&_TI1K);
  }
  v87 = (char *)v69;
  sub_4A01E0(a3, v97, v96, v100);
  sub_4A01E0(a3, v99, v98, v95);
  k = v82;
  v93 = 0;
  while ( k < *(_DWORD *)(a3 + 24) )
  {
    v38 = *((_DWORD *)v80 + v93);
    if ( v38 )
    {
      v39 = *(_DWORD *)(a3 + 20) + 132 * k;
      v84 = *(_DWORD *)(v39 + 16);
      v63 = (unsigned int *)v84;
      v40 = *(_DWORD **)(v39 + 12);
      v89 = v40;
      sub_49FB40(Block, v38, v40, &v84, v100, v97, v96);
      sub_49FD70(Block, *((_DWORD *)v80 + v93), (char *)v40 + v84, &v63, v95, v99, v98);
      v41 = 0;
      v91 = 0;
      v42 = v87;
      while ( v41 < *((_DWORD *)v80 + v93) )
      {
        v43 = 0;
        v73 = 0;
        while ( v43 < 0x40 )
        {
          if ( *((_WORD *)Block + v43 + v41) != *(_WORD *)&v87[2 * v43 + 2 * v41] )
          {
            v53 = 3;
            _CxxThrowException(&v53, (_ThrowInfo *)&_TI1K);
          }
          v73 = ++v43;
          v41 = v91;
        }
        v41 += 64;
        v91 = v41;
      }
    }
    else
    {
      v42 = v87;
    }
    v87 = &v42[16 * v74 * v81];
    ++k;
    ++v93;
  }
  _aligned_free(Block);
  Block = 0;
  if ( v15[5] == 4 )
  {
    v44 = Size;
    v45 = operator new[](Size);
    v46 = v45;
    v70 = v45;
    v47 = *(_DWORD *)(a3 + 20);
    if ( *(_DWORD *)(v47 + 128) == 1 )
    {
      sub_4A0930(v45, v44, *(_DWORD *)(v47 + 12), v47 + 16, v86);
    }
    else
    {
      if ( *(_DWORD *)(v47 + 128) != 2 )
      {
        v55 = 3;
        _CxxThrowException(&v55, (_ThrowInfo *)&_TI1K);
      }
      v48 = *(_DWORD **)(a3 + 20);
      v89 = (_DWORD *)v48[3];
      v90 = (unsigned int)(v48[20] * v48[21] + 7) >> 3;
      v83 = v48[12] - v90;
      sub_4A0B90(a3, *v89, v48 + 4);
      sub_4A0650(v70, v71, &v83, *(_DWORD *)(*(_DWORD *)(a3 + 20) + 72), *(_DWORD *)(*(_DWORD *)(a3 + 20) + 80));
      v46 = v70;
    }
    v49 = 3;
    v91 = 3;
    while ( v49 < Size )
    {
      if ( v46[v49] != *((_BYTE *)v68 + v49) )
      {
        v54 = 3;
        _CxxThrowException(&v54, (_ThrowInfo *)&_TI1K);
      }
      v49 += 4;
      v91 = v49;
    }
    operator delete[](v46);
    v70 = 0;
  }
  v65 = 0;
  v103 = -1;
  operator delete[](*(void **)(a3 + 20));
  operator delete[](v68);
  operator delete[](v59);
  operator delete[](v58);
  operator delete[](v56);
  operator delete[](v71);
  operator delete[](v62);
  operator delete[](v64);
  operator delete[](v69);
  operator delete[](v80);
  operator delete[](v70);
  _aligned_free(Block);
  return v65;
}
