
;==== 45EAE0 sub_45EAE0
45EAE0  push    ebp
45EAE1  mov     ebp, esp
45EAE3  cmp     [ebp+arg_0], 0
45EAE7  jz      short loc_45EB35
45EAE9  mov     ecx, dword_5177C0
45EAEF  xor     edx, edx
45EAF1  lea     eax, ds:0[ecx*4]
45EAF8  div     dword_5177C4
45EAFE  cmp     eax, 0Ah
45EB01  jb      short loc_45EB05
45EB03  shr     ecx, 1
45EB05  mov     [edi], ecx
45EB07  mov     ecx, dword_5177C4
45EB0D  push    esi
45EB0E  mov     esi, dword_5177C0
45EB14  xor     edx, edx
45EB16  mov     eax, ecx
45EB18  div     esi
45EB1A  cmp     eax, 1
45EB1D  jb      short loc_45EB2F
45EB1F  mov     eax, ecx
45EB21  imul    eax, 64h ; 'd'
45EB24  xor     edx, edx
45EB26  div     esi
45EB28  cmp     eax, 7Dh ; '}'
45EB2B  jnb     short loc_45EB2F
45EB2D  shr     ecx, 1
45EB2F  pop     esi
45EB30  mov     [edi+4], ecx
45EB33  pop     ebp
45EB34  retn
45EB35  mov     eax, dword_5177C0
45EB3A  mov     [edi], eax
45EB3C  mov     ecx, dword_5177C4
45EB42  mov     [edi+4], ecx
45EB45  pop     ebp
45EB46  retn

;==== 45EB60 sub_45EB60
45EB60  call    sub_461600
45EB65  mov     [ecx], eax
45EB67  call    sub_461620
45EB6C  mov     [ecx+4], eax
45EB6F  retn

;==== 45ECB0 sub_45ECB0
45ECB0  mov     ecx, dword_51773C
45ECB6  mov     edx, dword_517740
45ECBC  mov     [eax], ecx
45ECBE  mov     [eax+4], edx
45ECC1  retn

;==== 45EDB0 sub_45EDB0
45EDB0  mov     eax, dword_50721C
45EDB5  retn

;==== 45EDE0 sub_45EDE0
45EDE0  push    ebp
45EDE1  mov     ebp, esp
45EDE3  sub     esp, 10h
45EDE6  push    esi
45EDE7  mov     esi, dword_565EAC
45EDED  push    edi
45EDEE  cmp     esi, 2
45EDF1  jnz     short loc_45EE20
45EDF3  lea     ecx, [ebp+var_10]
45EDF6  call    sub_45EB60
45EDFB  push    1
45EDFD  lea     edi, [ebp+var_8]
45EE00  call    sub_45EAE0
45EE05  mov     eax, [ebp+var_8]
45EE08  add     esp, 4
45EE0B  cmp     eax, [ebp+var_10]
45EE0E  jb      short loc_45EE18
45EE10  mov     ecx, [ebp+var_4]
45EE13  cmp     ecx, [ebp+var_C]
45EE16  jnb     short loc_45EE20
45EE18  xor     eax, eax
45EE1A  pop     edi
45EE1B  pop     esi
45EE1C  mov     esp, ebp
45EE1E  pop     ebp
45EE1F  retn
45EE20  pop     edi
45EE21  mov     eax, esi
45EE23  pop     esi
45EE24  mov     esp, ebp
45EE26  pop     ebp
45EE27  retn

;==== 45EE30 sub_45EE30
45EE30  push    ebp
45EE31  mov     ebp, esp
45EE33  sub     esp, 14h
45EE36  push    ebx
45EE37  mov     ebx, [ebp+arg_4]
45EE3A  push    esi
45EE3B  push    edi
45EE3C  lea     ecx, [ebp+var_14]
45EE3F  call    sub_45EB60
45EE44  call    sub_45FBA0
45EE49  test    eax, eax
45EE4B  jnz     short loc_45EEB4
45EE4D  lea     eax, [ebp+var_C]
45EE50  call    sub_45ECB0
45EE55  mov     eax, [ebp+arg_C]
45EE58  sub     eax, 0
45EE5B  jz      loc_45EF36
45EE61  dec     eax
45EE62  jnz     loc_45F0A5
45EE68  mov     eax, [ebp+var_14]
45EE6B  fild    [ebp+var_14]
45EE6E  test    eax, eax
45EE70  jns     short loc_45EE78
45EE72  fadd    ds:dbl_4EC908
45EE78  mov     ecx, [ebp+var_C]
45EE7B  fimul   [ebp+arg_4]
45EE7E  fild    [ebp+var_C]
45EE81  test    ecx, ecx
45EE83  jns     short loc_45EE8B
45EE85  fadd    ds:dbl_4EC908
45EE8B  fdivp   st(1), st
45EE8D  call    __ftol2_sse
45EE92  fild    [ebp+var_10]
45EE95  mov     edx, [ebp+var_10]
45EE98  mov     ebx, eax
45EE9A  test    edx, edx
45EE9C  jns     short loc_45EEA4
45EE9E  fadd    ds:dbl_4EC908
45EEA4  mov     eax, [ebp+var_8]
45EEA7  fimul   [ebp+arg_8]
45EEAA  test    eax, eax
45EEAC  fild    [ebp+var_8]
45EEAF  jmp     loc_45EF7D
45EEB4  push    1
45EEB6  lea     edi, [ebp+var_C]
45EEB9  call    sub_45EAE0
45EEBE  add     esp, 4
45EEC1  call    sub_45EDE0
45EEC6  sub     eax, 0
45EEC9  jz      loc_45EFA1
45EECF  dec     eax
45EED0  jz      short loc_45EE55
45EED2  dec     eax
45EED3  jnz     loc_45F0A5
45EED9  mov     eax, [ebp+arg_C]
45EEDC  sub     eax, 0
45EEDF  jz      short loc_45EF0F
45EEE1  dec     eax
45EEE2  jnz     loc_45F0A5
45EEE8  mov     edx, [ebp+var_C]
45EEEB  sub     edx, [ebp+var_14]
45EEEE  mov     eax, [ebp+var_8]
45EEF1  sub     eax, [ebp+var_10]
45EEF4  shr     edx, 1
45EEF6  shr     eax, 1
45EEF8  sub     ebx, edx
45EEFA  sub     [ebp+arg_8], eax
45EEFD  mov     eax, [ebp+arg_0]
45EF00  mov     ecx, [ebp+arg_8]
45EF03  mov     [eax], ebx
45EF05  mov     [eax+4], ecx
45EF08  pop     edi
45EF09  pop     esi
45EF0A  pop     ebx
45EF0B  mov     esp, ebp
45EF0D  pop     ebp
45EF0E  retn
45EF0F  mov     ecx, [ebp+var_C]
45EF12  sub     ecx, [ebp+var_14]
45EF15  mov     edx, [ebp+var_8]
45EF18  sub     edx, [ebp+var_10]
45EF1B  mov     eax, [ebp+arg_0]
45EF1E  shr     ecx, 1
45EF20  add     ebx, ecx
45EF22  shr     edx, 1
45EF24  add     [ebp+arg_8], edx
45EF27  mov     ecx, [ebp+arg_8]
45EF2A  mov     [eax], ebx
45EF2C  mov     [eax+4], ecx
45EF2F  pop     edi
45EF30  pop     esi
45EF31  pop     ebx
45EF32  mov     esp, ebp
45EF34  pop     ebp
45EF35  retn
45EF36  mov     ecx, [ebp+var_C]
45EF39  fild    [ebp+var_C]
45EF3C  test    ecx, ecx
45EF3E  jns     short loc_45EF46
45EF40  fadd    ds:dbl_4EC908
45EF46  mov     edx, [ebp+var_14]
45EF49  fimul   [ebp+arg_4]
45EF4C  fild    [ebp+var_14]
45EF4F  test    edx, edx
45EF51  jns     short loc_45EF59
45EF53  fadd    ds:dbl_4EC908
45EF59  fdivp   st(1), st
45EF5B  call    __ftol2_sse
45EF60  fild    [ebp+var_8]
45EF63  mov     ebx, eax
45EF65  mov     eax, [ebp+var_8]
45EF68  test    eax, eax
45EF6A  jns     short loc_45EF72
45EF6C  fadd    ds:dbl_4EC908
45EF72  mov     ecx, [ebp+var_10]
45EF75  fimul   [ebp+arg_8]
45EF78  test    ecx, ecx
45EF7A  fild    [ebp+var_10]
45EF7D  jns     short loc_45EF85
45EF7F  fadd    ds:dbl_4EC908
45EF85  fdivp   st(1), st
45EF87  call    __ftol2_sse
45EF8C  mov     [ebp+arg_8], eax
45EF8F  mov     eax, [ebp+arg_0]
45EF92  mov     ecx, [ebp+arg_8]
45EF95  mov     [eax], ebx
45EF97  mov     [eax+4], ecx
45EF9A  pop     edi
45EF9B  pop     esi
45EF9C  pop     ebx
45EF9D  mov     esp, ebp
45EF9F  pop     ebp
45EFA0  retn
45EFA1  mov     edx, [ebp+var_14]
45EFA4  fild    [ebp+var_14]
45EFA7  test    edx, edx
45EFA9  jns     short loc_45EFB1
45EFAB  fadd    ds:dbl_4EC908
45EFB1  mov     esi, [ebp+var_C]
45EFB4  mov     eax, esi
45EFB6  mov     [ebp+var_4], eax
45EFB9  fild    [ebp+var_4]
45EFBC  test    eax, eax
45EFBE  jns     short loc_45EFC6
45EFC0  fadd    ds:dbl_4EC908
45EFC6  mov     ecx, [ebp+var_10]
45EFC9  fdiv    st, st(1)
45EFCB  fild    [ebp+var_10]
45EFCE  test    ecx, ecx
45EFD0  jns     short loc_45EFD8
45EFD2  fadd    ds:dbl_4EC908
45EFD8  mov     edi, [ebp+var_8]
45EFDB  mov     edx, edi
45EFDD  mov     [ebp+var_4], edx
45EFE0  fild    [ebp+var_4]
45EFE3  test    edx, edx
45EFE5  jns     short loc_45EFED
45EFE7  fadd    ds:dbl_4EC908
45EFED  fdiv    st, st(1)
45EFEF  fcom    st(2)
45EFF1  fnstsw  ax
45EFF3  test    ah, 1
45EFF6  jnz     short loc_45EFFC
45EFF8  fstp    st
45EFFA  jmp     short loc_45EFFE
45EFFC  fstp    st(2)
45EFFE  mov     eax, [ebp+arg_C]
45F001  sub     eax, 0
45F004  jz      short loc_45F05F
45F006  dec     eax
45F007  jnz     loc_45F09F
45F00D  fxch    st(2)
45F00F  fmul    st, st(1)
45F011  call    __ftol2_sse
45F016  sub     esi, eax
45F018  sar     esi, 1
45F01A  sub     ebx, esi
45F01C  mov     [ebp+arg_4], ebx
45F01F  fild    [ebp+arg_4]
45F022  fdiv    st, st(1)
45F024  call    __ftol2_sse
45F029  fmul    st(1), st
45F02B  fxch    st(1)
45F02D  mov     ebx, eax
45F02F  call    __ftol2_sse
45F034  sub     edi, eax
45F036  mov     eax, [ebp+arg_8]
45F039  sar     edi, 1
45F03B  sub     eax, edi
45F03D  mov     [ebp+arg_8], eax
45F040  fild    [ebp+arg_8]
45F043  fdivrp  st(1), st
45F045  call    __ftol2_sse
45F04A  mov     [ebp+arg_8], eax
45F04D  mov     eax, [ebp+arg_0]
45F050  mov     ecx, [ebp+arg_8]
45F053  mov     [eax], ebx
45F055  mov     [eax+4], ecx
45F058  pop     edi
45F059  pop     esi
45F05A  pop     ebx
45F05B  mov     esp, ebp
45F05D  pop     ebp
45F05E  retn
45F05F  fxch    st(2)
45F061  fmul    st, st(1)
45F063  call    __ftol2_sse
45F068  sub     esi, eax
45F06A  sar     esi, 1
45F06C  mov     [ebp+var_4], esi
45F06F  fild    [ebp+var_4]
45F072  fild    [ebp+arg_4]
45F075  fmul    st, st(2)
45F077  faddp   st(1), st
45F079  call    __ftol2_sse
45F07E  fmul    st(1), st
45F080  fxch    st(1)
45F082  mov     ebx, eax
45F084  call    __ftol2_sse
45F089  sub     edi, eax
45F08B  sar     edi, 1
45F08D  mov     [ebp+arg_4], edi
45F090  fild    [ebp+arg_4]
45F093  fild    [ebp+arg_8]
45F096  fmulp   st(2), st
45F098  faddp   st(1), st
45F09A  jmp     loc_45EF87
45F09F  fstp    st(2)
45F0A1  fstp    st
45F0A3  fstp    st
45F0A5  mov     eax, [ebp+arg_0]
45F0A8  mov     ecx, [ebp+arg_8]
45F0AB  pop     edi
45F0AC  pop     esi
45F0AD  mov     [eax], ebx
45F0AF  mov     [eax+4], ecx
45F0B2  pop     ebx
45F0B3  mov     esp, ebp
45F0B5  pop     ebp
45F0B6  retn

;==== 45FBA0 sub_45FBA0
45FBA0  mov     eax, dword_565EA8
45FBA5  retn

;==== 460510 sub_460510
460510  push    ebp
460511  mov     ebp, esp
460513  mov     ecx, dword_565EB4
460519  xor     eax, eax
46051B  test    ecx, ecx
46051D  jz      short loc_46057F
46051F  cmp     [ebp+arg_0], eax
460522  jz      short loc_460556
460524  cmp     dword_565ED4, eax
46052A  jnz     short loc_460549
46052C  call    sub_45FBA0
460531  cmp     eax, 1
460534  jnz     short loc_460549
460536  call    sub_45EDB0
46053B  test    eax, eax
46053D  jnz     short loc_460549
46053F  mov     eax, [ecx]
460541  push    1
460543  push    ecx
460544  mov     ecx, [eax+50h]
460547  call    ecx
460549  inc     dword_565ED4
46054F  mov     eax, 1
460554  pop     ebp
460555  retn
460556  dec     dword_565ED4
46055C  jnz     short loc_46057A
46055E  call    sub_45FBA0
460563  cmp     eax, 1
460566  jnz     short loc_46057A
460568  call    sub_45EDB0
46056D  test    eax, eax
46056F  jnz     short loc_46057A
460571  mov     edx, [ecx]
460573  push    eax
460574  mov     eax, [edx+50h]
460577  push    ecx
460578  call    eax
46057A  mov     eax, 1
46057F  pop     ebp
460580  retn

;==== 4615F0 sub_4615F0
4615F0  mov     eax, dword_506B8C[eax*4]
4615F7  retn

;==== 461600 sub_461600
461600  mov     eax, dword_506B88
461605  jmp     sub_4615F0

;==== 461610 sub_461610
461610  mov     eax, dword_506BAC[eax*4]
461617  retn

;==== 461620 sub_461620
461620  mov     eax, dword_506B88
461625  jmp     sub_461610

;==== 4649B0 sub_4649B0
4649B0  mov     eax, dword_566014
4649B5  retn

;==== 4649E0 sub_4649E0
4649E0  mov     ecx, dword_566018
4649E6  push    esi
4649E7  mov     esi, eax
4649E9  xor     eax, eax
4649EB  test    ecx, ecx
4649ED  jz      short loc_464A10
4649EF  mov     eax, ecx
4649F1  push    edi
4649F2  lea     edi, [eax+1]
4649F5  mov     dl, [eax]
4649F7  inc     eax
4649F8  test    dl, dl
4649FA  jnz     short loc_4649F5
4649FC  sub     eax, edi
4649FE  inc     eax
4649FF  pop     edi
464A00  test    esi, esi
464A02  jz      short loc_464A10
464A04  sub     esi, ecx
464A06  mov     dl, [ecx]
464A08  mov     [esi+ecx], dl
464A0B  inc     ecx
464A0C  test    dl, dl
464A0E  jnz     short loc_464A06
464A10  pop     esi
464A11  retn

;==== 464A20 sub_464A20
464A20  xor     eax, eax
464A22  cmp     dword_566018, eax
464A28  jnz     short locret_464A5F
464A2A  mov     eax, esi
464A2C  lea     edx, [eax+1]
464A2F  nop
464A30  mov     cl, [eax]
464A32  inc     eax
464A33  test    cl, cl
464A35  jnz     short loc_464A30
464A37  sub     eax, edx
464A39  inc     eax
464A3A  push    eax; unsigned int
464A3B  call    ??_U@YAPAXI@Z; operator new[](uint)
464A40  mov     edx, eax
464A42  add     esp, 4
464A45  mov     dword_566018, eax
464A4A  mov     ecx, esi
464A4C  sub     edx, esi
464A4E  mov     edi, edi
464A50  mov     al, [ecx]
464A52  mov     [edx+ecx], al
464A55  inc     ecx
464A56  test    al, al
464A58  jnz     short loc_464A50
464A5A  mov     eax, 1
464A5F  retn

;==== 464A60 sub_464A60
464A60  mov     ecx, hWndParent
464A66  push    1010h
464A6B  push    eax
464A6C  push    ecx
464A6D  mov     eax, offset Caption; "Error!!"
464A72  call    sub_46C1E0
464A77  add     esp, 0Ch
464A7A  retn

;==== 464A80 sub_464A80
464A80  push    ebp
464A81  mov     ebp, esp
464A83  push    esi
464A84  mov     esi, eax
464A86  xor     eax, eax
464A88  call    sub_4649E0
464A8D  test    eax, eax
464A8F  jnz     short loc_464A98
464A91  mov     eax, esi
464A93  call    sub_464A60
464A98  call    sub_4649B0
464A9D  test    eax, eax
464A9F  jz      short loc_464AA6
464AA1  call    sub_464A20
464AA6  cmp     [ebp+pExceptionObject], 0
464AAA  jz      short loc_464AB5
464AAC  push    esi; void *
464AAD  call    ??_V@YAXPAX@Z; operator delete[](void *)
464AB2  add     esp, 4
464AB5  push    offset __TI1H; pThrowInfo
464ABA  lea     eax, [ebp+pExceptionObject]
464ABD  push    eax; pExceptionObject
464ABE  mov     [ebp+pExceptionObject], 7FFFFFFFh
464AC5  call    __CxxThrowException@8; _CxxThrowException(x,x)

;==== 46B6F0 sub_46B6F0
46B6F0  mov     ecx, dword_5666B0
46B6F6  xor     eax, eax
46B6F8  test    ecx, ecx
46B6FA  jz      short locret_46B712
46B6FC  mov     eax, [ecx+8]
46B6FF  mov     [edx], eax
46B701  mov     ecx, dword_5666B0
46B707  mov     eax, [ecx+0Ch]
46B70A  mov     [edx+4], eax
46B70D  mov     eax, 1
46B712  retn

;==== 46C1E0 sub_46C1E0
46C1E0  push    ebp
46C1E1  mov     ebp, esp
46C1E3  push    esi
46C1E4  push    edi
46C1E5  push    1
46C1E7  mov     esi, eax
46C1E9  call    sub_460510
46C1EE  push    1
46C1F0  call    sub_4992E0
46C1F5  add     esp, 8
46C1F8  mov     ecx, 1
46C1FD  call    sub_48F840
46C202  mov     edi, eax
46C204  test    esi, esi
46C206  jz      short loc_46C20C
46C208  mov     eax, esi
46C20A  jmp     short loc_46C21A
46C20C  mov     eax, dword_5666F8
46C211  test    eax, eax
46C213  jnz     short loc_46C21A
46C215  mov     eax, offset aEthornellBurik_0; "Ethornell - BURIKO General Interpreter "...
46C21A  mov     ecx, [ebp+uType]
46C21D  mov     edx, [ebp+lpText]
46C220  push    ecx; uType
46C221  push    eax; lpCaption
46C222  mov     eax, [ebp+hWnd]
46C225  push    edx; lpText
46C226  push    eax; hWnd
46C227  call    ds:MessageBoxA
46C22D  mov     ecx, edi
46C22F  mov     esi, eax
46C231  call    sub_48F840
46C236  call    sub_46DF80
46C23B  call    sub_499330
46C240  push    0
46C242  call    sub_460510
46C247  add     esp, 4
46C24A  pop     edi
46C24B  mov     eax, esi
46C24D  pop     esi
46C24E  pop     ebp
46C24F  retn

;==== 46DAB0 sub_46DAB0
46DAB0  mov     eax, dword_566800
46DAB5  retn

;==== 46DAC0 sub_46DAC0
46DAC0  push    ebx
46DAC1  push    esi
46DAC2  mov     eax, ecx
46DAC4  xor     esi, esi
46DAC6  dec     eax
46DAC7  push    edi
46DAC8  jz      short loc_46DADB
46DACA  dec     eax
46DACB  jnz     short loc_46DAE8
46DACD  call    sub_48F970
46DAD2  cmp     eax, 1
46DAD5  jnz     short loc_46DAE8
46DAD7  xor     ecx, ecx
46DAD9  jmp     short loc_46DAE8
46DADB  call    sub_48F970
46DAE0  cmp     eax, 1
46DAE3  jnz     short loc_46DAE8
46DAE5  lea     esi, [eax+1]
46DAE8  call    sub_48F6C0
46DAED  test    eax, eax
46DAEF  jz      short loc_46DB0C
46DAF1  mov     eax, ecx
46DAF3  dec     eax
46DAF4  jz      short loc_46DAFE
46DAF6  dec     eax
46DAF7  jnz     short loc_46DB0C
46DAF9  lea     ecx, [eax+1]
46DAFC  jmp     short loc_46DB0C
46DAFE  xor     eax, eax
46DB00  test    esi, esi
46DB02  setnz   al
46DB05  mov     ecx, 2
46DB0A  mov     esi, eax
46DB0C  mov     ebx, ds:GetAsyncKeyState
46DB12  push    ecx; vKey
46DB13  call    ebx ; GetAsyncKeyState
46DB15  movsx   edi, ax
46DB18  test    esi, esi
46DB1A  jz      short loc_46DB24
46DB1C  push    esi; vKey
46DB1D  call    ebx ; GetAsyncKeyState
46DB1F  movsx   ecx, ax
46DB22  or      edi, ecx
46DB24  call    sub_49ADA0
46DB29  test    eax, eax
46DB2B  jnz     short loc_46DB3A
46DB2D  call    sub_46DAB0
46DB32  test    eax, eax
46DB34  jnz     short loc_46DB3A
46DB36  pop     edi
46DB37  pop     esi
46DB38  pop     ebx
46DB39  retn
46DB3A  mov     ax, di
46DB3D  pop     edi
46DB3E  pop     esi
46DB3F  pop     ebx
46DB40  retn

;==== 46DBC0 sub_46DBC0
46DBC0  push    ebp
46DBC1  mov     ebp, esp
46DBC3  push    ebx
46DBC4  mov     ebx, [ebp+arg_0]
46DBC7  push    1Ch; Size
46DBC9  call    ??2@YAPAXI@Z; operator new(uint)
46DBCE  mov     [eax], ebx
46DBD0  mov     ecx, [esi]
46DBD2  mov     [eax+4], ecx
46DBD5  mov     edx, [esi+4]
46DBD8  mov     [eax+8], edx
46DBDB  mov     ecx, [esi+8]
46DBDE  mov     [eax+0Ch], ecx
46DBE1  mov     edx, [esi+0Ch]
46DBE4  mov     ecx, [ebp+arg_4]
46DBE7  mov     [eax+10h], edx
46DBEA  mov     [eax+14h], ecx
46DBED  mov     ecx, [edi+18h]
46DBF0  add     esp, 4
46DBF3  mov     edx, edi
46DBF5  test    ecx, ecx
46DBF7  jz      short loc_46DC0D
46DBF9  lea     esp, [esp+0]
46DC00  cmp     [ecx], ebx
46DC02  jbe     short loc_46DC0D
46DC04  mov     edx, ecx
46DC06  mov     ecx, [ecx+18h]
46DC09  test    ecx, ecx
46DC0B  jnz     short loc_46DC00
46DC0D  mov     [eax+18h], ecx
46DC10  mov     [edx+18h], eax
46DC13  pop     ebx
46DC14  pop     ebp
46DC15  retn

;==== 46DC20 sub_46DC20
46DC20  push    esi
46DC21  push    edi
46DC22  push    0
46DC24  push    eax
46DC25  mov     esi, offset dword_506A4C
46DC2A  mov     edi, offset dword_566804
46DC2F  call    sub_46DBC0
46DC34  add     esp, 8
46DC37  pop     edi
46DC38  pop     esi
46DC39  retn

;==== 46DC80 sub_46DC80
46DC80  push    ebp
46DC81  mov     ebp, esp
46DC83  sub     esp, 14h
46DC86  xor     eax, eax
46DC88  push    esi
46DC89  push    edi
46DC8A  push    eax
46DC8B  mov     [ebp+var_14], eax
46DC8E  mov     [ebp+var_10], eax
46DC91  mov     [ebp+var_C], eax
46DC94  mov     [ebp+var_8], eax
46DC97  mov     eax, [ebp+arg_0]
46DC9A  push    eax
46DC9B  lea     esi, [ebp+var_14]
46DC9E  mov     edi, offset dword_566820
46DCA3  call    sub_46DBC0
46DCA8  add     esp, 8
46DCAB  pop     edi
46DCAC  pop     esi
46DCAD  mov     esp, ebp
46DCAF  pop     ebp
46DCB0  retn

;==== 46DD00 sub_46DD00
46DD00  mov     edx, offset dword_566804
46DD05  jmp     loc_46DCC0
46DCC0  mov     ecx, [edx+18h]
46DCC3  xor     eax, eax
46DCC5  test    ecx, ecx
46DCC7  jz      short locret_46DCF2
46DCC9  lea     esp, [esp+0]
46DCD0  cmp     [ecx], esi
46DCD2  jz      short loc_46DCDE
46DCD4  mov     edx, ecx
46DCD6  mov     ecx, [ecx+18h]
46DCD9  test    ecx, ecx
46DCDB  jnz     short loc_46DCD0
46DCDD  retn
46DCDE  mov     eax, [ecx+18h]
46DCE1  push    ecx; void *
46DCE2  mov     [edx+18h], eax
46DCE5  call    ??3@YAXPAX@Z; operator delete(void *)
46DCEA  add     esp, 4
46DCED  mov     eax, 1
46DCF2  retn

;==== 46DD10 sub_46DD10
46DD10  mov     edx, offset dword_566820
46DD15  jmp     loc_46DCC0
46DCC0  mov     ecx, [edx+18h]
46DCC3  xor     eax, eax
46DCC5  test    ecx, ecx
46DCC7  jz      short locret_46DCF2
46DCC9  lea     esp, [esp+0]
46DCD0  cmp     [ecx], esi
46DCD2  jz      short loc_46DCDE
46DCD4  mov     edx, ecx
46DCD6  mov     ecx, [ecx+18h]
46DCD9  test    ecx, ecx
46DCDB  jnz     short loc_46DCD0
46DCDD  retn
46DCDE  mov     eax, [ecx+18h]
46DCE1  push    ecx; void *
46DCE2  mov     [edx+18h], eax
46DCE5  call    ??3@YAXPAX@Z; operator delete(void *)
46DCEA  add     esp, 4
46DCED  mov     eax, 1
46DCF2  retn

;==== 46DD70 sub_46DD70
46DD70  push    ebp
46DD71  mov     ebp, esp
46DD73  mov     eax, dword_506A44
46DD78  test    eax, eax
46DD7A  jz      short loc_46DD8E
46DD7C  mov     ecx, dword_566838
46DD82  test    ecx, ecx
46DD84  jz      short loc_46DD8E
46DD86  mov     eax, [ebp+arg_0]
46DD89  cmp     eax, [ecx]
46DD8B  sbb     eax, eax
46DD8D  inc     eax
46DD8E  pop     ebp
46DD8F  retn

;==== 46DD90 sub_46DD90
46DD90  push    ebp
46DD91  mov     ebp, esp
46DD93  and     esp, 0FFFFFFF8h
46DD96  sub     esp, 0Ch
46DD99  push    ebx
46DD9A  xor     eax, eax
46DD9C  push    esi
46DD9D  push    edi
46DD9E  cmp     dword_506A44, eax
46DDA4  jz      loc_46DE64
46DDAA  lea     eax, [esp+18h+var_8]
46DDAE  push    eax
46DDAF  call    sub_48F180
46DDB4  mov     edi, [esp+1Ch+var_8]
46DDB8  mov     ebx, [esp+1Ch+var_4]
46DDBC  add     esp, 4
46DDBF  test    edi, edi
46DDC1  js      short loc_46DDE1
46DDC3  call    sub_461600
46DDC8  cmp     edi, eax
46DDCA  jge     short loc_46DDE1
46DDCC  test    ebx, ebx
46DDCE  js      short loc_46DDE1
46DDD0  call    sub_461620
46DDD5  mov     [esp+18h+var_C], 1
46DDDD  cmp     ebx, eax
46DDDF  jl      short loc_46DDE9
46DDE1  mov     [esp+18h+var_C], 0
46DDE9  mov     esi, dword_56681C
46DDEF  test    esi, esi
46DDF1  jz      short loc_46DE62
46DDF3  cmp     dword ptr [esi+14h], 0
46DDF7  jz      short loc_46DE24
46DDF9  cmp     [esp+18h+var_C], 0
46DDFE  jz      loc_46DEA6
46DE04  mov     ecx, [esi+14h]
46DE07  mov     edx, [ecx]
46DE09  mov     eax, [edx+8]
46DE0C  call    eax
46DE0E  test    eax, eax
46DE10  jz      loc_46DEA6
46DE16  mov     ecx, [esi+14h]
46DE19  mov     edx, [ecx]
46DE1B  mov     edx, [edx+24h]
46DE1E  lea     eax, [esi+4]
46DE21  push    eax
46DE22  call    edx
46DE24  mov     eax, [esi]
46DE26  mov     ecx, [ebp+arg_0]
46DE29  cmp     eax, ecx
46DE2B  jbe     short loc_46DE6B
46DE2D  cmp     [esi+4], edi
46DE30  jg      short loc_46DEA6
46DE32  cmp     edi, [esi+0Ch]
46DE35  jg      short loc_46DEA6
46DE37  mov     eax, [esi+8]
46DE3A  cmp     eax, ebx
46DE3C  jg      short loc_46DEA6
46DE3E  cmp     ebx, [esi+10h]
46DE41  jg      short loc_46DEA6
46DE43  cmp     dword ptr [esi+14h], 0
46DE47  jz      short loc_46DE62
46DE49  mov     ecx, [esi+14h]
46DE4C  mov     edx, [ecx]
46DE4E  mov     edx, [edx+64h]
46DE51  sub     ebx, eax
46DE53  push    1
46DE55  mov     eax, edi
46DE57  sub     eax, [esi+4]
46DE5A  push    ebx
46DE5B  push    eax
46DE5C  call    edx
46DE5E  test    eax, eax
46DE60  jz      short loc_46DEA2
46DE62  xor     eax, eax
46DE64  pop     edi
46DE65  pop     esi
46DE66  pop     ebx
46DE67  mov     esp, ebp
46DE69  pop     ebp
46DE6A  retn
46DE6B  jnz     short loc_46DE62
46DE6D  cmp     [esi+4], edi
46DE70  jg      short loc_46DEA6
46DE72  cmp     edi, [esi+0Ch]
46DE75  jg      short loc_46DEA6
46DE77  mov     eax, [esi+8]
46DE7A  cmp     eax, ebx
46DE7C  jg      short loc_46DEA6
46DE7E  cmp     ebx, [esi+10h]
46DE81  jg      short loc_46DEA6
46DE83  cmp     dword ptr [esi+14h], 0
46DE87  jz      short loc_46DEBA
46DE89  mov     ecx, [esi+14h]
46DE8C  mov     edx, [ecx]
46DE8E  mov     edx, [edx+64h]
46DE91  sub     ebx, eax
46DE93  push    1
46DE95  mov     eax, edi
46DE97  sub     eax, [esi+4]
46DE9A  push    ebx
46DE9B  push    eax
46DE9C  call    edx
46DE9E  test    eax, eax
46DEA0  jnz     short loc_46DEBA
46DEA2  mov     ebx, [esp+18h+var_4]
46DEA6  mov     esi, [esi+18h]
46DEA9  test    esi, esi
46DEAB  jnz     loc_46DDF3
46DEB1  xor     eax, eax
46DEB3  pop     edi
46DEB4  pop     esi
46DEB5  pop     ebx
46DEB6  mov     esp, ebp
46DEB8  pop     ebp
46DEB9  retn
46DEBA  pop     edi
46DEBB  pop     esi
46DEBC  mov     eax, 1
46DEC1  pop     ebx
46DEC2  mov     esp, ebp
46DEC4  pop     ebp
46DEC5  retn

;==== 46DF80 sub_46DF80
46DF80  mov     eax, offset dword_518CAC
46DF85  xor     ecx, ecx
46DF87  mov     [eax-4], ecx
46DF8A  mov     [eax], ecx
46DF8C  mov     [eax+4], ecx
46DF8F  mov     [eax+0Ch], ecx
46DF92  add     eax, 18h
46DF95  cmp     eax, offset VersionInformation.dwMajorVersion
46DF9A  jl      short loc_46DF87
46DF9C  retn

;==== 46E060 sub_46E060
46E060  mov     eax, esi
46E062  call    sub_4986B0
46E067  lea     eax, [esi+esi*2]
46E06A  add     eax, eax
46E06C  add     eax, eax
46E06E  add     eax, eax
46E070  xor     ecx, ecx
46E072  mov     dword_518CA8[eax], ecx
46E078  mov     dword_518CAC[eax], ecx
46E07E  retn

;==== 46E0A0 sub_46E0A0
46E0A0  push    ebp
46E0A1  mov     ebp, esp
46E0A3  push    ecx
46E0A4  push    ebx
46E0A5  push    esi
46E0A6  mov     esi, [ebp+arg_0]
46E0A9  push    edi
46E0AA  mov     eax, esi
46E0AC  call    sub_4986B0
46E0B1  mov     ecx, esi
46E0B3  call    sub_46DAC0
46E0B8  lea     edi, [esi+esi*2]
46E0BB  add     edi, edi
46E0BD  add     edi, edi
46E0BF  add     edi, edi
46E0C1  xor     ebx, ebx
46E0C3  shr     eax, 0Fh
46E0C6  and     eax, 1
46E0C9  cmp     dword_518CA8[edi], ebx
46E0CF  jz      short loc_46E0DA
46E0D1  cmp     eax, ebx
46E0D3  jnz     short loc_46E0DA
46E0D5  call    sub_46E060
46E0DA  xor     ecx, ecx
46E0DC  lea     eax, [esi-0C1h]; switch 4 cases
46E0E2  mov     [ebp+arg_0], ecx
46E0E5  cmp     eax, 16h
46E0E8  ja      short loc_46E159
46E0EA  mov     [ebp+var_4], ebx
46E0ED  lea     edi, [ebp+var_4]
46E0F0  cmp     eax, 3
46E0F3  ja      short def_46E0F5; jumptable 0046E0F5 default case
46E0F5  jmp     ds:jpt_46E0F5[eax*4]; switch jump
46E0FC  mov     edi, offset dword_5067C0; jumptable 0046E0F5 case 193
46E101  jmp     short def_46E0F5; jumptable 0046E0F5 default case
46E103  mov     edi, offset dword_506800; jumptable 0046E0F5 case 194
46E108  jmp     short def_46E0F5; jumptable 0046E0F5 default case
46E10A  mov     edi, offset dword_5069AC; jumptable 0046E0F5 case 195
46E10F  jmp     short def_46E0F5; jumptable 0046E0F5 default case
46E111  mov     edi, offset dword_506A38; jumptable 0046E0F5 case 196
46E116  xor     esi, esi; jumptable 0046E0F5 default case
46E118  cmp     [edi], ecx
46E11A  jz      short loc_46E14E
46E11C  mov     eax, edi
46E11E  mov     edi, edi
46E120  mov     eax, [eax]
46E122  push    eax
46E123  call    sub_46E0A0
46E128  add     esp, 4
46E12B  test    eax, eax
46E12D  jz      short loc_46E141
46E12F  mov     ecx, eax
46E131  and     ecx, 80000000h
46E137  or      [ebp+arg_0], ecx
46E13A  and     eax, 7FFFFFFFh
46E13F  add     ebx, eax
46E141  inc     esi
46E142  cmp     dword ptr [edi+esi*4], 0
46E146  lea     eax, [edi+esi*4]
46E149  jnz     short loc_46E120
46E14B  mov     ecx, [ebp+arg_0]
46E14E  pop     edi
46E14F  or      ecx, ebx
46E151  pop     esi
46E152  mov     eax, ecx
46E154  pop     ebx
46E155  mov     esp, ebp
46E157  pop     ebp
46E158  retn
46E159  mov     eax, dword_518CB0[edi]
46E15F  cmp     dword_518CA8[edi], ebx
46E165  jz      short loc_46E17E
46E167  cmp     dword_518CAC[edi], ebx
46E16D  jnz     short loc_46E17E
46E16F  or      eax, 80000000h
46E174  mov     dword_518CAC[edi], 1
46E17E  mov     dword_518CB0[edi], ebx
46E184  pop     edi
46E185  pop     esi
46E186  pop     ebx
46E187  mov     esp, ebp
46E189  pop     ebp
46E18A  retn

;==== 46E1E0 sub_46E1E0
46E1E0  push    ebp
46E1E1  mov     ebp, esp
46E1E3  sub     esp, 0A8h
46E1E9  mov     eax, dword_4FB734
46E1EE  xor     eax, ebp
46E1F0  mov     [ebp+var_4], eax
46E1F3  push    ebx
46E1F4  push    esi
46E1F5  push    edi
46E1F6  xor     edi, edi
46E1F8  mov     esi, offset dword_506740
46E1FD  mov     [ebp+var_A8], esi
46E203  mov     [ebp+var_A4], offset dword_506780
46E20D  mov     [ebp+var_A0], offset dword_5067C0
46E217  mov     [ebp+var_9C], offset dword_506800
46E221  mov     [ebp+var_98], offset dword_506840
46E22B  mov     [ebp+var_94], offset dword_506880
46E235  mov     [ebp+var_90], offset dword_5068C0
46E23F  mov     [ebp+var_8C], offset dword_506900
46E249  mov     [ebp+var_88], offset dword_506734
46E253  mov     [ebp+var_84], offset dword_506940
46E25D  mov     [ebp+var_80], offset dword_50694C
46E264  mov     [ebp+var_7C], offset dword_506958
46E26B  mov     [ebp+var_78], offset dword_506964
46E272  mov     [ebp+var_74], offset dword_506970
46E279  mov     [ebp+var_70], offset dword_50697C
46E280  mov     [ebp+var_6C], offset dword_506988
46E287  mov     [ebp+var_68], offset dword_506994
46E28E  mov     [ebp+var_64], offset dword_5069A0
46E295  mov     [ebp+var_60], offset dword_5069B8
46E29C  mov     [ebp+var_5C], edi
46E29F  mov     [ebp+var_50], 40h ; '@'
46E2A6  mov     [ebp+var_4C], 80h
46E2AD  mov     [ebp+var_48], 100h
46E2B4  mov     [ebp+var_44], 200h
46E2BB  mov     [ebp+var_40], 1000h
46E2C2  mov     [ebp+var_3C], 2000h
46E2C9  mov     [ebp+var_38], 4000h
46E2D0  mov     [ebp+var_34], 8000h
46E2D7  mov     [ebp+var_30], 10000h
46E2DE  mov     [ebp+var_2C], 20000h
46E2E5  mov     [ebp+var_28], 40000h
46E2EC  mov     [ebp+var_24], 80000h
46E2F3  mov     [ebp+var_20], 100000h
46E2FA  mov     [ebp+var_1C], 200000h
46E301  mov     [ebp+var_18], 400000h
46E308  mov     [ebp+var_14], 800000h
46E30F  mov     [ebp+var_10], 1000000h
46E316  mov     [ebp+var_C], 2000000h
46E31D  mov     [ebp+var_8], 40000000h
46E324  mov     [ebp+var_54], edi
46E327  mov     [ebp+var_58], edi
46E32A  xor     ebx, ebx
46E32C  lea     esp, [esp+0]
46E330  cmp     [esi], edi
46E332  jz      short loc_46E358
46E334  mov     eax, esi
46E336  mov     eax, [eax]
46E338  push    eax
46E339  call    sub_46E0A0
46E33E  add     esp, 4
46E341  test    eax, eax
46E343  jnz     short loc_46E351
46E345  inc     edi
46E346  cmp     dword ptr [esi+edi*4], 0
46E34A  lea     eax, [esi+edi*4]
46E34D  jnz     short loc_46E336
46E34F  jmp     short loc_46E358
46E351  mov     ecx, [ebp+ebx+var_50]
46E355  or      [ebp+var_54], ecx
46E358  mov     eax, [ebp+var_58]
46E35B  inc     eax
46E35C  lea     ebx, ds:0[eax*4]
46E363  mov     esi, [ebp+ebx+var_A8]
46E36A  xor     edi, edi
46E36C  mov     [ebp+var_58], eax
46E36F  cmp     esi, edi
46E371  jnz     short loc_46E330
46E373  mov     ecx, [ebp+var_4]
46E376  mov     eax, [ebp+var_54]
46E379  pop     edi
46E37A  pop     esi
46E37B  xor     ecx, ebp
46E37D  pop     ebx
46E37E  call    sub_4ABDD5
46E383  mov     esp, ebp
46E385  pop     ebp
46E386  retn

;==== 46E390 sub_46E390
46E390  push    ebp
46E391  mov     ebp, esp
46E393  push    ecx
46E394  mov     eax, dword_56683C
46E399  push    ebx
46E39A  push    esi
46E39B  push    edi
46E39C  mov     [ebp+var_4], eax
46E39F  xor     edi, edi
46E3A1  call    sub_49ADA0
46E3A6  test    eax, eax
46E3A8  jz      short loc_46E423
46E3AA  cmp     dword_506A44, edi
46E3B0  jz      short loc_46E423
46E3B2  xor     ebx, ebx
46E3B4  cmp     dword_5069F8, ebx
46E3BA  jz      short loc_46E423
46E3BC  mov     esi, offset dword_5069F8
46E3C1  mov     ecx, [esi]
46E3C3  call    sub_46DAC0
46E3C8  and     eax, 8000h
46E3CD  test    edi, edi
46E3CF  jnz     short loc_46E3D5
46E3D1  test    eax, eax
46E3D3  jz      short loc_46E3DA
46E3D5  mov     edi, 1
46E3DA  cmp     dword_566840, 0
46E3E1  jz      short loc_46E404
46E3E3  mov     ecx, [esi]
46E3E5  push    ecx
46E3E6  call    sub_46E0A0
46E3EB  add     esp, 4
46E3EE  test    eax, eax
46E3F0  jz      short loc_46E411
46E3F2  cmp     dword_566700, 0
46E3F9  jz      short loc_46E411
46E3FB  mov     [ebp+var_4], 1
46E402  jmp     short loc_46E411
46E404  test    eax, eax
46E406  jz      short loc_46E411
46E408  cmp     dword_566700, 0
46E40F  jnz     short loc_46E43C
46E411  inc     ebx
46E412  cmp     dword_5069F8[ebx*4], 0
46E41A  lea     esi, dword_5069F8[ebx*4]
46E421  jnz     short loc_46E3C1
46E423  cmp     dword_566840, 0
46E42A  jz      short loc_46E436
46E42C  test    edi, edi
46E42E  jnz     short loc_46E436
46E430  mov     dword_566840, edi
46E436  cmp     [ebp+var_4], 0
46E43A  jz      short loc_46E451
46E43C  cmp     dword_506A48, 0
46E443  jz      short loc_46E451
46E445  pop     edi
46E446  pop     esi
46E447  mov     eax, 1
46E44C  pop     ebx
46E44D  mov     esp, ebp
46E44F  pop     ebp
46E450  retn
46E451  pop     edi
46E452  pop     esi
46E453  xor     eax, eax
46E455  pop     ebx
46E456  mov     esp, ebp
46E458  pop     ebp
46E459  retn

;==== 46E460 sub_46E460
46E460  push    ebp
46E461  mov     ebp, esp
46E463  push    esi
46E464  push    eax
46E465  xor     esi, esi
46E467  call    sub_46DD70
46E46C  add     esp, 4
46E46F  test    eax, eax
46E471  jz      short loc_46E489
46E473  call    sub_46E1E0
46E478  mov     esi, eax
46E47A  call    sub_46E390
46E47F  test    eax, eax
46E481  jz      short loc_46E489
46E483  or      esi, 80000000h
46E489  mov     ecx, [ebp+arg_0]
46E48C  push    ecx
46E48D  call    sub_46DD90
46E492  add     esp, 4
46E495  test    eax, eax
46E497  jz      short loc_46E4EE
46E499  push    1
46E49B  call    sub_46E0A0
46E4A0  add     esp, 4
46E4A3  test    eax, eax
46E4A5  jz      short loc_46E4AA
46E4A7  or      esi, 1
46E4AA  push    2
46E4AC  call    sub_46E0A0
46E4B1  add     esp, 4
46E4B4  test    eax, eax
46E4B6  jz      short loc_46E4BB
46E4B8  or      esi, 2
46E4BB  push    4
46E4BD  call    sub_46E0A0
46E4C2  add     esp, 4
46E4C5  test    eax, eax
46E4C7  jz      short loc_46E4CC
46E4C9  or      esi, 4
46E4CC  push    5
46E4CE  call    sub_46E0A0
46E4D3  add     esp, 4
46E4D6  test    eax, eax
46E4D8  jz      short loc_46E4DD
46E4DA  or      esi, 10h
46E4DD  push    6
46E4DF  call    sub_46E0A0
46E4E4  add     esp, 4
46E4E7  test    eax, eax
46E4E9  jz      short loc_46E4EE
46E4EB  or      esi, 20h
46E4EE  mov     eax, esi
46E4F0  pop     esi
46E4F1  pop     ebp
46E4F2  retn

;==== 488C10 sub_488C10
488C10  push    ebp
488C11  mov     ebp, esp
488C13  mov     ecx, [ebp+arg_0]
488C16  push    esi
488C17  call    sub_445480
488C1C  mov     esi, eax
488C1E  shl     esi, 10h
488C21  or      esi, 0FFFFh
488C27  mov     eax, esi
488C29  call    sub_46DC20
488C2E  push    esi
488C2F  call    sub_46DC80
488C34  push    esi
488C35  mov     eax, esi
488C37  call    sub_46E460
488C3C  add     esp, 8
488C3F  xor     eax, eax
488C41  pop     esi
488C42  pop     ebp
488C43  retn

;==== 488C50 sub_488C50
488C50  push    ebp
488C51  mov     ebp, esp
488C53  mov     ecx, [ebp+arg_0]
488C56  push    esi
488C57  call    sub_445480
488C5C  mov     esi, eax
488C5E  shl     esi, 10h
488C61  or      esi, 0FFFFh
488C67  push    esi
488C68  mov     eax, esi
488C6A  call    sub_46E460
488C6F  add     esp, 4
488C72  call    sub_46DD00
488C77  call    sub_46DD10
488C7C  xor     eax, eax
488C7E  pop     esi
488C7F  pop     ebp
488C80  retn

;==== 488C90 sub_488C90
488C90  push    ebp
488C91  mov     ebp, esp
488C93  push    esi
488C94  push    edi
488C95  mov     edi, [ebp+arg_0]
488C98  mov     ecx, edi
488C9A  call    sub_445480
488C9F  shl     eax, 10h
488CA2  or      eax, 0FFFFh
488CA7  push    eax
488CA8  call    sub_46E460
488CAD  mov     esi, eax
488CAF  add     esp, 4
488CB2  mov     eax, edi
488CB4  call    sub_4454A0
488CB9  pop     edi
488CBA  xor     eax, eax
488CBC  pop     esi
488CBD  pop     ebp
488CBE  retn

;==== 488D90 sub_488D90
488D90  push    ebp
488D91  mov     ebp, esp
488D93  push    ebx
488D94  mov     ebx, [ebp+arg_0]
488D97  push    esi
488D98  push    edi
488D99  mov     ecx, ebx
488D9B  call    sub_445480
488DA0  mov     ecx, ebx
488DA2  mov     edi, eax
488DA4  call    sub_445480
488DA9  shl     eax, 10h
488DAC  or      eax, 0FFFFh
488DB1  xor     esi, esi
488DB3  cmp     edi, 1
488DB6  jz      short loc_488DC5
488DB8  cmp     edi, 2
488DBB  jz      short loc_488DC5
488DBD  push    eax
488DBE  call    sub_46DD70
488DC3  jmp     short loc_488DCB
488DC5  push    eax
488DC6  call    sub_46DD90
488DCB  add     esp, 4
488DCE  test    eax, eax
488DD0  jz      short loc_488DDD
488DD2  push    edi
488DD3  call    sub_46E0A0
488DD8  mov     esi, eax
488DDA  add     esp, 4
488DDD  mov     eax, ebx
488DDF  call    sub_4454A0
488DE4  pop     edi
488DE5  pop     esi
488DE6  xor     eax, eax
488DE8  pop     ebx
488DE9  pop     ebp
488DEA  retn

;==== 48F180 sub_48F180
48F180  push    ebp
48F181  mov     ebp, esp
48F183  and     esp, 0FFFFFFF8h
48F186  sub     esp, 1Ch
48F189  mov     eax, dword_4FB734
48F18E  xor     eax, esp
48F190  mov     [esp+1Ch+var_4], eax
48F194  cmp     dword_566704, 0
48F19B  push    ebx
48F19C  mov     ebx, [ebp+arg_0]
48F19F  push    esi
48F1A0  push    edi
48F1A1  jz      loc_48F254
48F1A7  mov     edx, ebx
48F1A9  call    sub_46B6F0
48F1AE  test    eax, eax
48F1B0  jnz     loc_48F261
48F1B6  mov     ecx, hWndParent
48F1BC  lea     eax, [esp+28h+Rect]
48F1C0  push    eax; lpRect
48F1C1  push    ecx; hWnd
48F1C2  call    ds:GetWindowRect
48F1C8  lea     edx, [esp+28h+Point]
48F1CC  push    edx; lpPoint
48F1CD  call    ds:GetCursorPos
48F1D3  call    sub_45FBA0
48F1D8  test    eax, eax
48F1DA  jnz     short loc_48F225
48F1DC  mov     edi, ds:GetSystemMetrics
48F1E2  push    7; nIndex
48F1E4  call    edi ; GetSystemMetrics
48F1E6  add     eax, [esp+28h+Rect.left]
48F1EA  push    4; nIndex
48F1EC  sub     [esp+2Ch+Point.x], eax
48F1F0  call    edi ; GetSystemMetrics
48F1F2  mov     esi, [esp+28h+Point.y]
48F1F6  sub     esi, [esp+28h+Rect.top]
48F1FA  push    8; nIndex
48F1FC  sub     esi, eax
48F1FE  call    edi ; GetSystemMetrics
48F200  sub     esi, eax
48F202  mov     eax, [esp+28h+Point.x]
48F206  push    1
48F208  push    esi
48F209  push    eax
48F20A  push    ebx
48F20B  call    sub_45EE30
48F210  add     esp, 10h
48F213  pop     edi
48F214  pop     esi
48F215  pop     ebx
48F216  mov     ecx, [esp+1Ch+var_4]
48F21A  xor     ecx, esp
48F21C  call    sub_4ABDD5
48F221  mov     esp, ebp
48F223  pop     ebp
48F224  retn
48F225  mov     esi, [esp+28h+Point.y]
48F229  mov     eax, [esp+28h+Point.x]
48F22D  sub     esi, [esp+28h+Rect.top]
48F231  sub     eax, [esp+28h+Rect.left]
48F235  push    1
48F237  push    esi
48F238  push    eax
48F239  push    ebx
48F23A  call    sub_45EE30
48F23F  add     esp, 10h
48F242  pop     edi
48F243  pop     esi
48F244  pop     ebx
48F245  mov     ecx, [esp+1Ch+var_4]
48F249  xor     ecx, esp
48F24B  call    sub_4ABDD5
48F250  mov     esp, ebp
48F252  pop     ebp
48F253  retn
48F254  mov     dword ptr [ebx], 0
48F25A  mov     dword ptr [ebx+4], 0
48F261  mov     ecx, [esp+28h+var_4]
48F265  pop     edi
48F266  pop     esi
48F267  pop     ebx
48F268  xor     ecx, esp
48F26A  call    sub_4ABDD5
48F26F  mov     esp, ebp
48F271  pop     ebp
48F272  retn

;==== 48F6C0 sub_48F6C0
48F6C0  mov     eax, dword_566920
48F6C5  retn

;==== 48F840 sub_48F840
48F840  mov     eax, dword_503E70
48F845  push    edi
48F846  mov     edi, eax
48F848  test    ecx, ecx
48F84A  jz      short loc_48F852
48F84C  test    eax, eax
48F84E  jz      short loc_48F856
48F850  pop     edi
48F851  retn
48F852  test    eax, eax
48F854  jz      short loc_48F87F
48F856  push    esi
48F857  mov     esi, ds:ShowCursor
48F85D  mov     dword_503E70, ecx
48F863  test    ecx, ecx
48F865  jz      short loc_48F874
48F867  push    1; bShow
48F869  call    esi ; ShowCursor
48F86B  test    eax, eax
48F86D  js      short loc_48F867
48F86F  pop     esi
48F870  mov     eax, edi
48F872  pop     edi
48F873  retn
48F874  push    0; bShow
48F876  call    esi ; ShowCursor
48F878  test    eax, eax
48F87A  jns     short loc_48F874
48F87C  mov     eax, edi
48F87E  pop     esi
48F87F  pop     edi
48F880  retn

;==== 48F970 sub_48F970
48F970  mov     eax, dword_566930
48F975  retn

;==== 4986B0 sub_4986B0
4986B0  cmp     eax, 0FFh
4986B5  jg      short loc_4986BB
4986B7  test    eax, eax
4986B9  jns     short locret_4986C7
4986BB  push    0
4986BD  mov     eax, offset dword_4EC4D4
4986C2  call    sub_464A80
4986C7  retn

;==== 499290 sub_499290
499290  cmp     dword_566A34, 0
499297  jz      short loc_4992A1
499299  call    ds:timeGetTime
49929F  jmp     short loc_4992A7
4992A1  call    ds:GetTickCount
4992A7  mov     ecx, dword_566A64
4992AD  cmp     eax, ecx
4992AF  jb      short loc_4992BD
4992B1  mov     edx, dword_503DE8
4992B7  add     edx, ecx
4992B9  cmp     edx, eax
4992BB  jnb     short loc_4992C7
4992BD  mov     edx, eax
4992BF  sub     edx, ecx
4992C1  add     dword_566A6C, edx
4992C7  mov     ecx, dword_566A68
4992CD  mov     dword_566A64, eax
4992D2  test    ecx, ecx
4992D4  jz      short loc_4992D9
4992D6  mov     eax, ecx
4992D8  retn
4992D9  sub     eax, dword_566A6C
4992DF  retn

;==== 4992E0 sub_4992E0
4992E0  push    ebp
4992E1  mov     ebp, esp
4992E3  xor     ecx, ecx
4992E5  cmp     dword_566700, ecx
4992EB  jz      short loc_499324
4992ED  cmp     dword_566A70, ecx
4992F3  jnz     short loc_499324
4992F5  call    sub_499380
4992FA  test    eax, eax
4992FC  jnz     short loc_499303
4992FE  cmp     [ebp+arg_0], ecx
499301  jz      short loc_49931A
499303  cmp     dword_566A68, ecx
499309  jnz     short loc_49931A
49930B  call    sub_499290
499310  mov     dword_566A68, eax
499315  mov     ecx, 1
49931A  mov     dword_566A70, 1
499324  mov     eax, ecx
499326  pop     ebp
499327  retn

;==== 499330 sub_499330
499330  xor     eax, eax
499332  cmp     dword_566700, eax
499338  jz      short locret_49936F
49933A  cmp     dword_566A70, eax
499340  jz      short locret_49936F
499342  push    esi
499343  mov     esi, dword_566A68
499349  test    esi, esi
49934B  jz      short loc_499364
49934D  mov     dword_566A68, eax
499352  call    sub_499290
499357  sub     eax, esi
499359  add     dword_566A6C, eax
49935F  mov     eax, 1
499364  mov     dword_566A70, 0
49936E  pop     esi
49936F  retn

;==== 499380 sub_499380
499380  mov     eax, dword_566A60
499385  retn

;==== 49ADA0 sub_49ADA0
49ADA0  mov     eax, dword_5666FC
49ADA5  retn

;==== 4ABDD5 sub_4ABDD5
4ABDD5  cmp     ecx, dword_4FB734
4ABDDB  jnz     short loc_4ABDDF
4ABDDD  rep retn
4ABDDF  jmp     ___report_gsfailure

