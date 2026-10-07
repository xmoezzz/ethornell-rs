// Types and macros Hex-Rays output relies on. The sources under src/ are
// reading material for the Rust port; they are written to be syntactically
// close to C but are not expected to link.
#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint8_t  _BYTE;
typedef uint16_t _WORD;
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;
typedef int BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef char CHAR;
typedef void *PVOID;

#define __cdecl
#define __stdcall
#define __fastcall
#define __thiscall
#define __usercall
#define __userpurge
#define __noreturn
#define LOBYTE(x)  (*((_BYTE *)&(x)))
#define HIBYTE(x)  (*((_BYTE *)&(x) + 1))
#define LOWORD(x)  (*((_WORD *)&(x)))
#define HIWORD(x)  (*((_WORD *)&(x) + 1))
#define BYREF
