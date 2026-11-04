;vtable 4DC394 const std::bad_alloc::`vftable'
;   +00 4441C0 sub_4441C0
;   +04 4ACE76 exception::what(void)
;vtable 4DC3B0 const std::exception::`vftable'
;   +00 4ACF48 sub_4ACF48
;   +04 4ACE76 exception::what(void)
;vtable 4E5B10 const CSpline::`vftable'
;   +00 4444E0 sub_4444E0
;vtable 4E5B78 const DCDstrbtdPrcssng::`vftable'
;   +00 446760 sub_446760
;vtable 4E6084 const DCSleep::`vftable'
;   +00 452BC0 sub_452BC0
;vtable 4E60B4 const DCTELgclFldMngr::`vftable'
;   +00 453220 sub_453220
;   +04 4ACE2F __purecall
;   +08 4ACE2F __purecall
;   +0C 4ACE2F __purecall
;   +10 4ACE2F __purecall
;   +14 4ACE2F __purecall
;   +18 454250 sub_454250
;   +1C 4ACE2F __purecall
;   +20 4ACE2F __purecall
;   +24 4ACE2F __purecall
;   +28 4ACE2F __purecall
;   +2C 4ACE2F __purecall
;   +30 4ACE2F __purecall
;   +34 4ACE2F __purecall
;   +38 4ACE2F __purecall
;vtable 4E60F4 const DCTELgclGrdFld::`vftable'
;   +00 454500 sub_454500
;   +04 454600 sub_454600
;   +08 454770 sub_454770
;   +0C 454880 sub_454880
;   +10 4548E0 sub_4548E0
;vtable 4E610C const DCTELgclGrdFldMngr::`vftable'
;   +00 454A80 sub_454A80
;   +04 454BC0 sub_454BC0
;   +08 489870 __RTC_NumErrors
;   +0C 454DE0 sub_454DE0
;   +10 454E40 sub_454E40
;   +14 454F00 sub_454F00
;   +18 454F50 sub_454F50
;   +1C 454F80 sub_454F80
;   +20 455110 sub_455110
;   +24 455180 sub_455180
;   +28 4551F0 sub_4551F0
;   +2C 455220 sub_455220
;   +30 455340 sub_455340
;   +34 455360 sub_455360
;   +38 4557B0 sub_4557B0
;vtable 4E614C const DCTELgclSpcMngr::`vftable'
;   +00 4559F0 sub_4559F0
;vtable 4E6154 const DCTELogicalField::`vftable'
;   +00 457A70 sub_457A70
;   +04 4ACE2F __purecall
;   +08 4ACE2F __purecall
;   +0C 4ACE2F __purecall
;   +10 4ACE2F __purecall
;vtable 4E616C const DCTESLGEvaluator::`vftable'
;   +00 4584C0 sub_4584C0

;==== 401000 sub_401000
401000  push    ebp
401001  mov     ebp, esp
401003  and     esp, 0FFFFFFF8h
401006  mov     ecx, [ebp+arg_0]
401009  mov     eax, [ebp+arg_4]
40100C  test    ecx, ecx
40100E  jnz     short loc_401030
401010  test    eax, eax
401012  jnz     short loc_401018
401014  mov     esp, ebp
401016  pop     ebp
401017  retn
401018  jle     short loc_401023
40101A  mov     eax, 5A0000h
40101F  mov     esp, ebp
401021  pop     ebp
401022  retn
401023  test    eax, eax
401025  jns     short loc_401079
401027  mov     eax, 10E0000h
40102C  mov     esp, ebp
40102E  pop     ebp
40102F  retn
401030  jle     short loc_401079
401032  fild    [ebp+arg_4]
401035  fidiv   [ebp+arg_0]
401038  test    eax, eax
40103A  js      short loc_401056
40103C  call    __CIatan
401041  fmul    ds:dbl_4EC978
401047  fdiv    ds:dbl_4EC960
40104D  call    __ftol2_sse
401052  mov     esp, ebp
401054  pop     ebp
401055  retn
401056  call    __CIatan
40105B  fmul    ds:dbl_4EC9E8
401061  fdiv    ds:dbl_4EC960
401067  call    __ftol2_sse
40106C  mov     ecx, 1680000h
401071  sub     ecx, eax
401073  mov     eax, ecx
401075  mov     esp, ebp
401077  pop     ebp
401078  retn
401079  fild    [ebp+arg_4]
40107C  fidiv   [ebp+arg_0]
40107F  call    __CIatan
401084  fmul    ds:dbl_4EC9E8
40108A  fdiv    ds:dbl_4EC960
401090  call    __ftol2_sse
401095  mov     ecx, 0B40000h
40109A  sub     ecx, eax
40109C  mov     eax, ecx
40109E  mov     esp, ebp
4010A0  pop     ebp
4010A1  retn

;==== 4010B0 sub_4010B0
4010B0  push    ebp
4010B1  mov     ebp, esp
4010B3  fld     [ebp+arg_0]
4010B6  sub     esp, 8
4010B9  fstp    [esp+8+X]; X
4010BC  call    _floor
4010C1  fld     [ebp+arg_0]
4010C4  add     esp, 8
4010C7  fld     st
4010C9  fsub    st, st(2)
4010CB  fld1
4010CD  fld     [ebp+arg_8]
4010D0  fsub    st(1), st
4010D2  fxch    st(1)
4010D4  fcomp   st(2)
4010D6  fnstsw  ax
4010D8  test    ah, 41h
4010DB  jp      short loc_4010F3
4010DD  fstp    st(1)
4010DF  sub     esp, 8
4010E2  fstp    st(2)
4010E4  fstp    st(1)
4010E6  fstp    [esp+8+X]; X
4010E9  call    _ceil
4010EE  add     esp, 8
4010F1  pop     ebp
4010F2  retn
4010F3  fcompp
4010F5  fnstsw  ax
4010F7  test    ah, 1
4010FA  jnz     short loc_401100
4010FC  fstp    st
4010FE  pop     ebp
4010FF  retn
401100  fstp    st(1)
401102  pop     ebp
401103  retn

;==== 4441C0 sub_4441C0
4441C0  push    ebp
4441C1  mov     ebp, esp
4441C3  push    0FFFFFFFFh
4441C5  push    offset SEH_444B40
4441CA  mov     eax, large fs:0
4441D0  push    eax
4441D1  push    ecx
4441D2  push    esi
4441D3  mov     eax, dword_4FB734
4441D8  xor     eax, ebp
4441DA  push    eax
4441DB  lea     eax, [ebp+var_C]
4441DE  mov     large fs:0, eax
4441E4  mov     esi, ecx
4441E6  mov     [ebp+var_10], esi
4441E9  mov     dword ptr [esi], offset ??_7bad_alloc@std@@6B@; const std::bad_alloc::`vftable'
4441EF  mov     [ebp+var_4], 0FFFFFFFFh
4441F6  call    sub_4ACF3D
4441FB  test    [ebp+arg_0], 1
4441FF  jz      short loc_44420A
444201  push    esi; void *
444202  call    ??3@YAXPAX@Z; operator delete(void *)
444207  add     esp, 4
44420A  mov     eax, esi
44420C  mov     ecx, [ebp+var_C]
44420F  mov     large fs:0, ecx
444216  pop     ecx
444217  pop     esi
444218  mov     esp, ebp
44421A  pop     ebp
44421B  retn    4
4D8CB0  mov     ecx, [ebp+var_10]
4D8CB3  jmp     sub_4ACF3D
4D8CB8  mov     edx, [esp-4+arg_4]
4D8CBC  lea     eax, [edx+0Ch]
4D8CBF  mov     ecx, [edx-0Ch]
4D8CC2  xor     ecx, eax
4D8CC4  call    sub_4ABDD5
4D8CC9  mov     eax, offset stru_4F64F4
4D8CCE  jmp     ___CxxFrameHandler3

;==== 444220 nullsub_1
444220  retn

;==== 444230 sub_444230
444230  push    ebp
444231  mov     ebp, esp
444233  sub     esp, 334h
444239  mov     eax, dword_4FB734
44423E  xor     eax, ebp
444240  mov     [ebp+var_4], eax
444243  push    ebx
444244  push    esi
444245  mov     esi, [edx+4]
444248  sub     esi, [edx]
44424A  xor     eax, eax
44424C  sar     esi, 2
44424F  dec     esi
444250  push    edi
444251  mov     [ecx], esi
444253  js      short loc_444268
444255  lea     esi, [ecx+8]
444258  mov     edi, [edx]
44425A  fild    dword ptr [edi+eax*4]
44425D  inc     eax
44425E  add     esi, 8
444261  fstp    qword ptr [esi-8]
444264  cmp     eax, [ecx]
444266  jle     short loc_444258
444268  fldz
44426A  mov     eax, [ecx]
44426C  fst     qword ptr [ecx+eax*8+658h]
444273  mov     edx, 1
444278  fst     qword ptr [ecx+658h]
44427E  fld     ds:dbl_4EC918
444284  cmp     [ecx], edx
444286  jle     short loc_4442AE
444288  fld     ds:dbl_4E2838
44428E  lea     eax, [ecx+18h]
444291  fld     qword ptr [eax-8]
444294  inc     edx
444295  fmul    st, st(1)
444297  add     eax, 8
44429A  fsubr   qword ptr [eax-18h]
44429D  fadd    qword ptr [eax-8]
4442A0  fmul    st, st(2)
4442A2  fstp    qword ptr [eax+640h]
4442A8  cmp     edx, [ecx]
4442AA  jl      short loc_444291
4442AC  fstp    st
4442AE  mov     eax, 1
4442B3  fxch    st(1)
4442B5  fst     [ebp+var_330]
4442BB  cmp     [ecx], eax
4442BD  jle     short loc_4442F6
4442BF  fld     ds:dbl_4EC980
4442C5  lea     edx, [ecx+660h]
4442CB  fld1
4442CD  fld     st(1)
4442CF  inc     eax
4442D0  fsub    [ebp+eax*8+var_340]
4442D7  add     edx, 8
4442DA  fld     qword ptr [edx-8]
4442DD  fsub    qword ptr [edx-10h]
4442E0  fdiv    st, st(1)
4442E2  fstp    qword ptr [edx-8]
4442E5  fdivr   st, st(1)
4442E7  fstp    [ebp+eax*8+var_338]
4442EE  cmp     eax, [ecx]
4442F0  jl      short loc_4442CD
4442F2  fstp    st(1)
4442F4  fstp    st
4442F6  mov     edx, [ecx]
4442F8  dec     edx
4442F9  cmp     edx, 4
4442FC  jl      short loc_44434A
4442FE  lea     edi, [edx-4]
444301  shr     edi, 2
444304  inc     edi
444305  mov     ebx, edi
444307  neg     ebx
444309  lea     esi, [ebp+edx*8+var_340]
444310  lea     eax, [ecx+edx*8+658h]
444317  lea     edx, [edx+ebx*4]
44431A  fld     qword ptr [eax+8]
44431D  sub     eax, 20h ; ' '
444320  fmul    qword ptr [esi+10h]
444323  sub     esi, 20h ; ' '
444326  dec     edi
444327  fsubr   qword ptr [eax+20h]
44432A  fst     qword ptr [eax+20h]
44432D  fmul    qword ptr [esi+28h]
444330  fsubr   qword ptr [eax+18h]
444333  fst     qword ptr [eax+18h]
444336  fmul    qword ptr [esi+20h]
444339  fsubr   qword ptr [eax+10h]
44433C  fst     qword ptr [eax+10h]
44433F  fmul    qword ptr [esi+18h]
444342  fsubr   qword ptr [eax+8]
444345  fstp    qword ptr [eax+8]
444348  jnz     short loc_44431A
44434A  test    edx, edx
44434C  jle     short loc_44436D
44434E  lea     eax, [ecx+edx*8+658h]
444355  fld     [ebp+edx*8+var_330]
44435C  dec     edx
44435D  fmul    qword ptr [eax+8]
444360  sub     eax, 8
444363  fsubr   qword ptr [eax+8]
444366  fstp    qword ptr [eax+8]
444369  test    edx, edx
44436B  jg      short loc_444355
44436D  mov     edx, [ecx]
44436F  fst     qword ptr [ecx+edx*8+980h]
444376  mov     eax, [ecx]
444378  xor     edx, edx
44437A  fstp    qword ptr [ecx+eax*8+330h]
444381  cmp     [ecx], edx
444383  jle     short loc_4443BE
444385  lea     eax, [ecx+980h]
44438B  fld     qword ptr [eax-320h]
444391  inc     edx
444392  fsub    qword ptr [eax-328h]
444398  add     eax, 8
44439B  fdiv    st, st(1)
44439D  fst     qword ptr [eax-8]
4443A0  fld     qword ptr [eax-978h]
4443A6  fsub    qword ptr [eax-980h]
4443AC  fsub    qword ptr [eax-330h]
4443B2  fsubrp  st(1), st
4443B4  fstp    qword ptr [eax-658h]
4443BA  cmp     edx, [ecx]
4443BC  jl      short loc_44438B
4443BE  mov     ecx, [ebp+var_4]
4443C1  fstp    st
4443C3  pop     edi
4443C4  pop     esi
4443C5  xor     ecx, ebp
4443C7  pop     ebx
4443C8  call    sub_4ABDD5
4443CD  mov     esp, ebp
4443CF  pop     ebp
4443D0  retn

;==== 4443E0 sub_4443E0
4443E0  push    ebp
4443E1  mov     ebp, esp
4443E3  push    ecx
4443E4  push    edi
4443E5  mov     edi, [esi]
4443E7  mov     [ebp+var_4], edi
4443EA  fild    [ebp+var_4]
4443ED  sub     esp, 8
4443F0  fmul    [ebp+arg_0]
4443F3  fst     [ebp+arg_0]
4443F6  fstp    [esp+10h+X]; X
4443F9  call    _floor
4443FE  add     esp, 8
444401  call    __ftol2_sse
444406  mov     [ebp+var_4], eax
444409  test    eax, eax
44440B  jns     short loc_444411
44440D  xor     eax, eax
44440F  jmp     short loc_444418
444411  cmp     eax, edi
444413  jl      short loc_44441B
444415  lea     eax, [edi-1]
444418  mov     [ebp+var_4], eax
44441B  fild    [ebp+var_4]
44441E  pop     edi
44441F  fsubr   [ebp+arg_0]
444422  fld     qword ptr [esi+eax*8+980h]
444429  fmul    st, st(1)
44442B  fadd    qword ptr [esi+eax*8+658h]
444432  fmul    st, st(1)
444434  fadd    qword ptr [esi+eax*8+330h]
44443B  fmulp   st(1), st
44443D  fadd    qword ptr [esi+eax*8+8]
444441  mov     esp, ebp
444443  pop     ebp
444444  retn    8

;==== 444450 sub_444450
444450  push    ebp
444451  mov     ebp, esp
444453  push    0FFFFFFFFh
444455  push    offset SEH_444450
44445A  mov     eax, large fs:0
444460  push    eax
444461  push    esi
444462  mov     eax, dword_4FB734
444467  xor     eax, ebp
444469  push    eax
44446A  lea     eax, [ebp+var_C]
44446D  mov     large fs:0, eax
444473  mov     esi, [ebp+arg_0]
444476  xor     eax, eax
444478  mov     dword ptr [esi], offset ??_7CSpline@@6B@; const CSpline::`vftable'
44447E  mov     [esi+8], eax
444481  mov     [esi+0Ch], eax
444484  mov     [esi+10h], eax
444487  mov     [ebp+var_4], eax
44448A  mov     [esi+18h], eax
44448D  mov     [esi+1Ch], eax
444490  mov     [esi+20h], eax
444493  mov     byte ptr [ebp+var_4], 1
444497  mov     [esi+28h], eax
44449A  mov     [esi+2Ch], eax
44449D  mov     [esi+30h], eax
4444A0  mov     byte ptr [ebp+var_4], 2
4444A4  mov     [esi+38h], eax
4444A7  mov     [esi+0CE0h], eax
4444AD  mov     [esi+1988h], eax
4444B3  call    sub_4445B0
4444B8  mov     [ebp+var_4], 0FFFFFFFFh
4444BF  mov     eax, esi
4444C1  mov     ecx, [ebp+var_C]
4444C4  mov     large fs:0, ecx
4444CB  pop     ecx
4444CC  pop     esi
4444CD  mov     esp, ebp
4444CF  pop     ebp
4444D0  retn    4
4448A0  mov     eax, [esi]
4448A2  test    eax, eax
4448A4  jz      short loc_4448AF
4448A6  push    eax; void *
4448A7  call    ??3@YAXPAX@Z; operator delete(void *)
4448AC  add     esp, 4
4448AF  mov     dword ptr [esi], 0
4448B5  mov     dword ptr [esi+4], 0
4448BC  mov     dword ptr [esi+8], 0
4448C3  retn
4D9270  mov     esi, [ebp+arg_0]
4D9273  add     esi, 8
4D9276  jmp     loc_4448A0
4D927B  mov     esi, [ebp+arg_0]
4D927E  add     esi, 18h
4D9281  jmp     loc_4448A0
4D9286  mov     esi, [ebp+arg_0]
4D9289  add     esi, 28h ; '('
4D928C  jmp     loc_4448A0
4D9291  mov     edx, [esp-4+arg_4]
4D9295  lea     eax, [edx+0Ch]
4D9298  mov     ecx, [edx-8]
4D929B  xor     ecx, eax
4D929D  call    sub_4ABDD5
4D92A2  mov     eax, offset stru_4F6ADC
4D92A7  jmp     ___CxxFrameHandler3

;==== 4444E0 sub_4444E0
4444E0  push    ebp
4444E1  mov     ebp, esp
4444E3  push    esi
4444E4  mov     esi, ecx
4444E6  call    sub_444510
4444EB  test    [ebp+arg_0], 1
4444EF  jz      short loc_4444FA
4444F1  push    esi; void *
4444F2  call    ??3@YAXPAX@Z; operator delete(void *)
4444F7  add     esp, 4
4444FA  mov     eax, esi
4444FC  pop     esi
4444FD  pop     ebp
4444FE  retn    4

;==== 444510 sub_444510
444510  push    ebp
444511  mov     ebp, esp
444513  push    0FFFFFFFFh
444515  push    offset SEH_444510
44451A  mov     eax, large fs:0
444520  push    eax
444521  push    ecx
444522  push    ebx
444523  push    esi
444524  mov     eax, dword_4FB734
444529  xor     eax, ebp
44452B  push    eax
44452C  lea     eax, [ebp+var_C]
44452F  mov     large fs:0, eax
444535  mov     esi, ecx
444537  mov     [ebp+var_10], esi
44453A  mov     dword ptr [esi], offset ??_7CSpline@@6B@; const CSpline::`vftable'
444540  mov     [ebp+var_4], 1
444547  mov     eax, [esi+28h]
44454A  xor     ebx, ebx
44454C  cmp     eax, ebx
44454E  jz      short loc_444559
444550  push    eax; void *
444551  call    ??3@YAXPAX@Z; operator delete(void *)
444556  add     esp, 4
444559  mov     [esi+28h], ebx
44455C  mov     [esi+2Ch], ebx
44455F  mov     [esi+30h], ebx
444562  mov     byte ptr [ebp+var_4], bl
444565  mov     eax, [esi+18h]
444568  cmp     eax, ebx
44456A  jz      short loc_444575
44456C  push    eax; void *
44456D  call    ??3@YAXPAX@Z; operator delete(void *)
444572  add     esp, 4
444575  mov     [esi+18h], ebx
444578  mov     [esi+1Ch], ebx
44457B  mov     [esi+20h], ebx
44457E  mov     [ebp+var_4], 0FFFFFFFFh
444585  mov     eax, [esi+8]
444588  cmp     eax, ebx
44458A  jz      short loc_444595
44458C  push    eax; void *
44458D  call    ??3@YAXPAX@Z; operator delete(void *)
444592  add     esp, 4
444595  mov     [esi+8], ebx
444598  mov     [esi+0Ch], ebx
44459B  mov     [esi+10h], ebx
44459E  mov     ecx, [ebp+var_C]
4445A1  mov     large fs:0, ecx
4445A8  pop     ecx
4445A9  pop     esi
4445AA  pop     ebx
4445AB  mov     esp, ebp
4445AD  pop     ebp
4445AE  retn
4448A0  mov     eax, [esi]
4448A2  test    eax, eax
4448A4  jz      short loc_4448AF
4448A6  push    eax; void *
4448A7  call    ??3@YAXPAX@Z; operator delete(void *)
4448AC  add     esp, 4
4448AF  mov     dword ptr [esi], 0
4448B5  mov     dword ptr [esi+4], 0
4448BC  mov     dword ptr [esi+8], 0
4448C3  retn
4D9230  mov     esi, [ebp+var_10]
4D9233  add     esi, 8
4D9236  jmp     loc_4448A0
4D923B  mov     esi, [ebp+var_10]
4D923E  add     esi, 18h
4D9241  jmp     loc_4448A0
4D9246  mov     esi, [ebp+var_10]
4D9249  add     esi, 28h ; '('
4D924C  jmp     loc_4448A0
4D9251  mov     edx, [esp-4+arg_4]
4D9255  lea     eax, [edx+0Ch]
4D9258  mov     ecx, [edx-10h]
4D925B  xor     ecx, eax
4D925D  call    sub_4ABDD5
4D9262  mov     eax, offset stru_4F6AA0
4D9267  jmp     ___CxxFrameHandler3

;==== 4445B0 sub_4445B0
4445B0  xor     eax, eax
4445B2  push    ebx
4445B3  mov     [esi+2634h], eax
4445B9  mov     [esi+2630h], eax
4445BF  mov     [esi+2638h], al
4445C5  mov     ecx, [esi+0Ch]
4445C8  push    edi
4445C9  mov     edi, [esi+8]
4445CC  cmp     edi, ecx
4445CE  jz      short loc_4445EE
4445D0  mov     eax, ecx
4445D2  sub     eax, ecx
4445D4  sar     eax, 2
4445D7  lea     ebx, ds:0[eax*4]
4445DE  push    ebx; Size
4445DF  push    ecx; Src
4445E0  push    edi; void *
4445E1  call    _memcpy
4445E6  add     esp, 0Ch
4445E9  add     ebx, edi
4445EB  mov     [esi+0Ch], ebx
4445EE  mov     ecx, [esi+1Ch]
4445F1  mov     edi, [esi+18h]
4445F4  cmp     edi, ecx
4445F6  jz      short loc_444616
4445F8  mov     eax, ecx
4445FA  sub     eax, ecx
4445FC  sar     eax, 2
4445FF  lea     ebx, ds:0[eax*4]
444606  push    ebx; Size
444607  push    ecx; Src
444608  push    edi; void *
444609  call    _memcpy
44460E  add     esp, 0Ch
444611  add     ebx, edi
444613  mov     [esi+1Ch], ebx
444616  mov     ecx, [esi+2Ch]
444619  mov     edi, [esi+28h]
44461C  cmp     edi, ecx
44461E  jz      short loc_44463E
444620  mov     eax, ecx
444622  sub     eax, ecx
444624  sar     eax, 2
444627  lea     ebx, ds:0[eax*4]
44462E  push    ebx; Size
44462F  push    ecx; Src
444630  push    edi; void *
444631  call    _memcpy
444636  add     esp, 0Ch
444639  add     ebx, edi
44463B  mov     [esi+2Ch], ebx
44463E  pop     edi
44463F  pop     ebx
444640  retn

;==== 444650 sub_444650
444650  push    ebp
444651  mov     ebp, esp
444653  mov     eax, [edi+0Ch]
444656  sub     eax, [edi+8]
444659  push    esi
44465A  lea     esi, [edi+8]
44465D  sar     eax, 2
444660  cmp     eax, 64h ; 'd'
444663  jb      short loc_44466C
444665  xor     al, al
444667  pop     esi
444668  pop     ebp
444669  retn    0Ch
44466C  lea     eax, [ebp+arg_0]
44466F  mov     byte ptr [edi+2638h], 0
444676  call    sub_4448D0
44467B  lea     esi, [edi+18h]
44467E  lea     eax, [ebp+arg_4]
444681  call    sub_4448D0
444686  lea     esi, [edi+28h]
444689  lea     eax, [ebp+arg_8]
44468C  call    sub_4448D0
444691  mov     al, 1
444693  pop     esi
444694  pop     ebp
444695  retn    0Ch

;==== 4446A0 sub_4446A0
4446A0  mov     dword ptr [eax+2630h], 0
4446AA  mov     [eax+2634h], ecx
4446B0  retn

;==== 4446C0 sub_4446C0
4446C0  push    ebp
4446C1  mov     ebp, esp
4446C3  and     esp, 0FFFFFFC0h
4446C6  sub     esp, 38h
4446C9  push    ebx
4446CA  push    edi
4446CB  mov     edi, eax
4446CD  xor     eax, eax
4446CF  mov     [esp+40h+var_5], 1
4446D4  cmp     esi, eax
4446D6  jnz     short loc_4446E0
4446D8  xor     al, al
4446DA  pop     edi
4446DB  pop     ebx
4446DC  mov     esp, ebp
4446DE  pop     ebp
4446DF  retn
4446E0  mov     [esi+8], eax
4446E3  mov     [esi+4], eax
4446E6  mov     [esi], eax
4446E8  mov     ecx, [edi+2634h]
4446EE  cmp     ecx, eax
4446F0  jz      short loc_4446D8
4446F2  mov     eax, [edi+0Ch]
4446F5  sub     eax, [edi+8]
4446F8  sar     eax, 2
4446FB  test    eax, eax
4446FD  jz      short loc_4446D8
4446FF  mov     ebx, [edi+2630h]
444705  add     ebx, ecx
444707  cmp     ebx, edx
444709  ja      short loc_444730
44470B  mov     ecx, [edi+8]
44470E  mov     ecx, [ecx+eax*4-4]
444712  mov     [esi], ecx
444714  mov     edx, [edi+18h]
444717  mov     ecx, [edx+eax*4-4]
44471B  mov     [esi+4], ecx
44471E  mov     edx, [edi+28h]
444721  mov     eax, [edx+eax*4-4]
444725  mov     [esi+8], eax
444728  mov     al, 1
44472A  pop     edi
44472B  pop     ebx
44472C  mov     esp, ebp
44472E  pop     ebp
44472F  retn
444730  sub     edx, [edi+2630h]
444736  mov     [esp+40h+var_4], edx
44473A  fild    [esp+40h+var_4]
44473E  jns     short loc_444746
444740  fadd    ds:dbl_4EC908
444746  mov     [esp+40h+var_4], ecx
44474A  fild    [esp+40h+var_4]
44474E  test    ecx, ecx
444750  jns     short loc_444758
444752  fadd    ds:dbl_4EC908
444758  fdivp   st(1), st
44475A  cmp     eax, 1
44475D  jnz     short loc_444782
44475F  mov     eax, [edi+8]
444762  fstp    st
444764  mov     ecx, [eax]
444766  mov     [esi], ecx
444768  mov     edx, [edi+18h]
44476B  mov     eax, [edx]
44476D  mov     [esi+4], eax
444770  mov     ecx, [edi+28h]
444773  mov     edx, [ecx]
444775  mov     al, [esp+40h+var_5]
444779  mov     [esi+8], edx
44477C  pop     edi
44477D  pop     ebx
44477E  mov     esp, ebp
444780  pop     ebp
444781  retn
444782  cmp     eax, 2
444785  jnz     short loc_4447EA
444787  mov     eax, [edi+8]
44478A  mov     ebx, [eax]
44478C  mov     eax, [eax+4]
44478F  sub     eax, ebx
444791  mov     [esp+40h+var_4], eax
444795  fild    [esp+40h+var_4]
444799  fmul    st, st(1)
44479B  call    __ftol2_sse
4447A0  add     eax, ebx
4447A2  mov     [esi], eax
4447A4  mov     eax, [edi+18h]
4447A7  mov     ebx, [eax]
4447A9  mov     ecx, [eax+4]
4447AC  sub     ecx, ebx
4447AE  mov     [esp+40h+var_4], ecx
4447B2  fild    [esp+40h+var_4]
4447B6  fmul    st, st(1)
4447B8  call    __ftol2_sse
4447BD  add     eax, ebx
4447BF  mov     [esi+4], eax
4447C2  mov     edi, [edi+28h]
4447C5  mov     ebx, [edi]
4447C7  mov     edx, [edi+4]
4447CA  sub     edx, ebx
4447CC  mov     [esp+40h+var_4], edx
4447D0  fild    [esp+40h+var_4]
4447D4  fmulp   st(1), st
4447D6  call    __ftol2_sse
4447DB  add     eax, ebx
4447DD  mov     [esi+8], eax
4447E0  mov     al, [esp+40h+var_5]
4447E4  pop     edi
4447E5  pop     ebx
4447E6  mov     esp, ebp
4447E8  pop     ebp
4447E9  retn
4447EA  sub     esp, 8
4447ED  fstp    [esp+48h+var_48]; double
4447F0  push    esi; int
4447F1  call    sub_444800
4447F6  pop     edi
4447F7  mov     [esp+3Ch+var_5], al
4447FB  pop     ebx
4447FC  mov     esp, ebp
4447FE  pop     ebp
4447FF  retn

;==== 444800 sub_444800
444800  push    ebp
444801  mov     ebp, esp
444803  push    ecx
444804  cmp     byte ptr [edi+2638h], 0
44480B  push    ebx
44480C  mov     ebx, [ebp+arg_0]
44480F  push    esi
444810  jnz     short loc_444840
444812  lea     edx, [edi+8]
444815  lea     ecx, [edi+38h]
444818  mov     byte ptr [edi+2638h], 1
44481F  call    sub_444230
444824  lea     edx, [edi+18h]
444827  lea     ecx, [edi+0CE0h]
44482D  call    sub_444230
444832  lea     edx, [edi+28h]
444835  lea     ecx, [edi+1988h]
44483B  call    sub_444230
444840  fld     [ebp+arg_4]
444843  sub     esp, 8
444846  lea     esi, [edi+38h]
444849  fstp    [esp+14h+var_14]; double
44484C  call    sub_4443E0
444851  call    __ftol2_sse
444856  fld     [ebp+arg_4]
444859  sub     esp, 8
44485C  lea     esi, [edi+0CE0h]
444862  fstp    [esp+14h+var_14]; double
444865  mov     [ebx], eax
444867  call    sub_4443E0
44486C  call    __ftol2_sse
444871  fld     [ebp+arg_4]
444874  sub     esp, 8
444877  lea     esi, [edi+1988h]
44487D  fstp    [esp+14h+var_14]; double
444880  mov     [ebx+4], eax
444883  call    sub_4443E0
444888  call    __ftol2_sse
44488D  mov     [ebx+8], eax
444890  pop     esi
444891  mov     al, 1
444893  pop     ebx
444894  pop     ecx
444895  pop     ebp
444896  retn    0Ch

;==== 4448D0 sub_4448D0
4448D0  push    edi
4448D1  mov     edi, eax
4448D3  mov     eax, [esi+4]
4448D6  cmp     edi, eax
4448D8  jnb     short loc_444908
4448DA  mov     ecx, [esi]
4448DC  cmp     ecx, edi
4448DE  ja      short loc_444908
4448E0  sub     edi, ecx
4448E2  sar     edi, 2
4448E5  cmp     eax, [esi+8]
4448E8  jnz     short loc_4448F1
4448EA  mov     ecx, esi
4448EC  call    sub_444930
4448F1  mov     eax, [esi]
4448F3  mov     edx, [esi+4]
4448F6  lea     ecx, [eax+edi*4]
4448F9  push    edx
4448FA  call    sub_444AF0
4448FF  add     esp, 4
444902  add     dword ptr [esi+4], 4
444906  pop     edi
444907  retn
444908  cmp     eax, [esi+8]
44490B  jnz     short loc_444914
44490D  mov     ecx, esi
44490F  call    sub_444930
444914  mov     eax, [esi+4]
444917  push    eax
444918  mov     ecx, edi
44491A  call    sub_444AF0
44491F  add     dword ptr [esi+4], 4
444923  add     esp, 4
444926  pop     edi
444927  retn

;==== 444930 sub_444930
444930  mov     eax, [ecx+4]
444933  push    esi
444934  mov     esi, [ecx]
444936  sub     eax, esi
444938  sar     eax, 2
44493B  cmp     eax, 3FFFFFFEh
444940  jbe     short loc_44494C
444942  push    offset aVectorTTooLong; "vector<T> too long"
444947  call    ?_Xlength_error@std@@YAXPBD@Z; std::_Xlength_error(char const *)
44494C  mov     edx, [ecx+8]
44494F  sub     edx, esi
444951  inc     eax
444952  sar     edx, 2
444955  cmp     eax, edx
444957  jbe     short loc_44497C
444959  push    edi
44495A  mov     esi, edx
44495C  shr     esi, 1
44495E  mov     edi, 3FFFFFFFh
444963  sub     edi, esi
444965  cmp     edi, edx
444967  pop     edi
444968  jnb     short loc_44496E
44496A  xor     edx, edx
44496C  jmp     short loc_444970
44496E  add     edx, esi
444970  cmp     edx, eax
444972  jnb     short loc_444976
444974  mov     edx, eax
444976  push    edx
444977  call    sub_444980
44497C  pop     esi
44497D  retn

;==== 444980 sub_444980
444980  push    ebp
444981  mov     ebp, esp
444983  push    0FFFFFFFFh
444985  push    offset SEH_444980
44498A  mov     eax, large fs:0
444990  push    eax
444991  sub     esp, 8
444994  push    ebx
444995  push    esi
444996  push    edi
444997  mov     eax, dword_4FB734
44499C  xor     eax, ebp
44499E  push    eax
44499F  lea     eax, [ebp+var_C]
4449A2  mov     large fs:0, eax
4449A8  mov     [ebp+var_10], esp
4449AB  mov     esi, ecx
4449AD  mov     ecx, [ebp+arg_0]
4449B0  cmp     ecx, 3FFFFFFFh
4449B6  jbe     short loc_4449C2
4449B8  push    offset aVectorTTooLong; "vector<T> too long"
4449BD  call    ?_Xlength_error@std@@YAXPBD@Z; std::_Xlength_error(char const *)
4449C2  mov     eax, [esi+8]
4449C5  sub     eax, [esi]
4449C7  sar     eax, 2
4449CA  cmp     eax, ecx
4449CC  jnb     short loc_444A2A
4449CE  call    sub_444A60
4449D3  mov     ebx, eax
4449D5  mov     [ebp+var_14], ebx
4449D8  mov     [ebp+var_4], 0
4449DF  mov     eax, [esi+4]
4449E2  mov     ecx, [esi]
4449E4  sub     eax, ecx
4449E6  sar     eax, 2
4449E9  lea     edx, ds:0[eax*4]
4449F0  push    edx; Size
4449F1  push    ecx; Src
4449F2  push    ebx; void *
4449F3  call    _memcpy
4449F8  add     esp, 0Ch
4449FB  mov     [ebp+var_4], 0FFFFFFFFh
444A02  mov     eax, [esi]
444A04  mov     edi, [esi+4]
444A07  sub     edi, eax
444A09  sar     edi, 2
444A0C  test    eax, eax
444A0E  jz      short loc_444A19
444A10  push    eax; void *
444A11  call    ??3@YAXPAX@Z; operator delete(void *)
444A16  add     esp, 4
444A19  mov     eax, [ebp+arg_0]
444A1C  lea     ecx, [ebx+eax*4]
444A1F  mov     [esi+8], ecx
444A22  lea     edx, [ebx+edi*4]
444A25  mov     [esi+4], edx
444A28  mov     [esi], ebx
444A2A  mov     ecx, [ebp+var_C]
444A2D  mov     large fs:0, ecx
444A34  pop     ecx
444A35  pop     edi
444A36  pop     esi
444A37  pop     ebx
444A38  mov     esp, ebp
444A3A  pop     ebp
444A3B  retn    4
444A3E  mov     eax, [ebp+var_14]
444A41  push    eax; void *
444A42  call    ??3@YAXPAX@Z; operator delete(void *)
444A47  add     esp, 4
444A4A  push    0; pThrowInfo
444A4C  push    0; pExceptionObject
444A4E  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D9210  mov     edx, [esp-4+arg_4]
4D9214  lea     eax, [edx+0Ch]
4D9217  mov     ecx, [edx-18h]
4D921A  xor     ecx, eax
4D921C  call    sub_4ABDD5
4D9221  mov     eax, offset stru_4F6A64
4D9226  jmp     ___CxxFrameHandler3

;==== 444A60 sub_444A60
444A60  push    ebp
444A61  mov     ebp, esp
444A63  push    0FFFFFFFFh
444A65  push    offset SEH_444A60
444A6A  mov     eax, large fs:0
444A70  push    eax
444A71  sub     esp, 10h
444A74  mov     eax, dword_4FB734
444A79  xor     eax, ebp
444A7B  push    eax
444A7C  lea     eax, [ebp+var_C]
444A7F  mov     large fs:0, eax
444A85  xor     eax, eax
444A87  test    ecx, ecx
444A89  jz      short loc_444ADD
444A8B  cmp     ecx, 3FFFFFFFh
444A91  ja      short loc_444AA7
444A93  lea     eax, ds:0[ecx*4]
444A9A  push    eax; Size
444A9B  call    ??2@YAPAXI@Z; operator new(uint)
444AA0  add     esp, 4
444AA3  test    eax, eax
444AA5  jnz     short loc_444ADD
444AA7  lea     ecx, [ebp+var_10]
444AAA  push    ecx; char **
444AAB  lea     ecx, [ebp+pExceptionObject]; this
444AAE  mov     [ebp+var_10], 0
444AB5  call    ??0exception@std@@QAE@ABQBD@Z; std::exception::exception(char const * const &)
444ABA  mov     [ebp+var_4], 0
444AC1  mov     [ebp+pExceptionObject], offset ??_7bad_alloc@std@@6B@; const std::bad_alloc::`vftable'
444AC8  mov     [ebp+var_4], 0FFFFFFFFh
444ACF  push    offset __TI2?AVbad_alloc@std@@; pThrowInfo
444AD4  lea     edx, [ebp+pExceptionObject]
444AD7  push    edx; pExceptionObject
444AD8  call    __CxxThrowException@8; _CxxThrowException(x,x)
444ADD  mov     ecx, [ebp+var_C]
444AE0  mov     large fs:0, ecx
444AE7  pop     ecx
444AE8  mov     esp, ebp
444AEA  pop     ebp
444AEB  retn
4D91E0  lea     ecx, [ebp+pExceptionObject]
4D91E3  jmp     sub_4ACF3D
4D91E8  mov     edx, [esp-4+arg_4]
4D91EC  lea     eax, [edx+0Ch]
4D91EF  mov     ecx, [edx-14h]
4D91F2  xor     ecx, eax
4D91F4  call    sub_4ABDD5
4D91F9  mov     eax, offset stru_4F6A0C
4D91FE  jmp     ___CxxFrameHandler3

;==== 444AF0 sub_444AF0
444AF0  push    ebp
444AF1  mov     ebp, esp
444AF3  push    0FFFFFFFFh
444AF5  push    offset SEH_444AF0
444AFA  mov     eax, large fs:0
444B00  push    eax
444B01  mov     eax, dword_4FB734
444B06  xor     eax, ebp
444B08  push    eax
444B09  lea     eax, [ebp+var_C]
444B0C  mov     large fs:0, eax
444B12  mov     eax, [ebp+arg_0]
444B15  mov     [ebp+var_4], 0
444B1C  test    eax, eax
444B1E  jz      short loc_444B24
444B20  mov     edx, [ecx]
444B22  mov     [eax], edx
444B24  mov     [ebp+var_4], 0FFFFFFFFh
444B2B  mov     ecx, [ebp+var_C]
444B2E  mov     large fs:0, ecx
444B35  pop     ecx
444B36  mov     esp, ebp
444B38  pop     ebp
444B39  retn
4D91C0  jmp     nullsub_1
4D91C5  mov     edx, [esp-4+arg_4]
4D91C9  lea     eax, [edx+0Ch]
4D91CC  mov     ecx, [edx-4]
4D91CF  xor     ecx, eax
4D91D1  call    sub_4ABDD5
4D91D6  mov     eax, offset stru_4F69E0
4D91DB  jmp     ___CxxFrameHandler3

;==== 4465B0 sub_4465B0
4465B0  push    ebp
4465B1  mov     ebp, esp
4465B3  sub     esp, 0Ch
4465B6  push    ebx
4465B7  push    esi
4465B8  mov     esi, eax
4465BA  push    edi
4465BB  mov     edi, [ebp+arg_0]
4465BE  xor     ebx, ebx
4465C0  xor     ecx, ecx
4465C2  mov     eax, edi
4465C4  mov     edx, 7Ch ; '|'
4465C9  mul     edx
4465CB  seto    cl
4465CE  mov     dword ptr [esi], offset ??_7DCDstrbtdPrcssng@@6B@; const DCDstrbtdPrcssng::`vftable'
4465D4  mov     [esi+4], edi
4465D7  mov     [esi+8], edi
4465DA  mov     dword ptr [esi+0Ch], 2
4465E1  mov     [esi+10h], ebx
4465E4  mov     [esi+14h], ebx
4465E7  neg     ecx
4465E9  or      ecx, eax
4465EB  push    ecx; Size
4465EC  mov     [esi+18h], ebx
4465EF  mov     dword ptr [esi+1Ch], 1
4465F6  mov     [esi+20h], ebx
4465F9  call    ??2@YAPAXI@Z; operator new(uint)
4465FE  mov     ecx, edi
446600  imul    ecx, 7Ch ; '|'
446603  push    ecx; Size
446604  push    ebx; Val
446605  push    eax; void *
446606  mov     [esi+24h], eax
446609  call    _memset
44660E  add     esp, 10h
446611  lea     edx, [esi+28h]
446614  push    edx; lpCriticalSection
446615  call    ds:InitializeCriticalSection
44661B  mov     eax, [esi+24h]
44661E  mov     dword ptr [eax], 1
446624  mov     ecx, [esi+24h]
446627  push    ebx; lpName
446628  push    ebx; bInitialState
446629  mov     [ecx+8], ebx
44662C  mov     edx, [esi+24h]
44662F  push    1; bManualReset
446631  push    ebx; lpEventAttributes
446632  mov     [edx+0Ch], ebx
446635  call    ds:CreateEventA
44663B  mov     ecx, [esi+24h]
44663E  mov     [ecx+10h], eax
446641  mov     edx, [esi+24h]
446644  mov     [edx+14h], ebx
446647  mov     eax, [esi+24h]
44664A  mov     ebx, 1
44664F  mov     [eax+18h], esi
446652  cmp     edi, ebx
446654  jbe     loc_446755
44665A  mov     edi, 7Ch ; '|'
44665F  nop
446660  mov     ecx, [esi+24h]
446663  push    0; lpName
446665  push    1; bInitialState
446667  mov     dword ptr [edi+ecx], 0
44666E  mov     edx, [esi+24h]
446671  push    1; bManualReset
446673  push    0; lpEventAttributes
446675  mov     [edi+edx+8], ebx
446679  call    ds:CreateEventA
44667F  mov     ecx, [esi+24h]
446682  push    0; lpName
446684  push    0; bInitialState
446686  push    1; bManualReset
446688  push    0; lpEventAttributes
44668A  mov     [edi+ecx+0Ch], eax
44668E  call    ds:CreateEventA
446694  mov     edx, [esi+24h]
446697  mov     [edi+edx+10h], eax
44669B  mov     eax, [esi+24h]
44669E  mov     dword ptr [edi+eax+14h], 0
4466A6  mov     ecx, [esi+24h]
4466A9  mov     [edi+ecx+18h], esi
4466AD  mov     [ebp+var_4], edi
4466B0  mov     [ebp+var_8], 4
4466B7  jmp     short loc_4466C0
4466C0  mov     edx, [esi+24h]
4466C3  mov     eax, [ebp+var_4]
4466C6  lea     ecx, [eax+edx+1Ch]
4466CA  push    ecx; lpCriticalSection
4466CB  call    ds:InitializeCriticalSection
4466D1  add     [ebp+var_4], 18h
4466D5  dec     [ebp+var_8]
4466D8  jnz     short loc_4466C0
4466DA  mov     edx, 2
4466DF  mov     ecx, ebx
4466E1  call    sub_446BB0
4466E6  mov     eax, [esi+24h]
4466E9  lea     edx, [ebp+ThrdAddr]
4466EC  push    edx; ThrdAddr
4466ED  push    0; InitFlag
4466EF  add     eax, edi
4466F1  push    eax; ArgList
4466F2  push    offset sub_446CF0; StartAddress
4466F7  push    0; StackSize
4466F9  push    0; Security
4466FB  call    __beginthreadex
446700  mov     ecx, [esi+24h]
446703  add     esp, 18h
446706  push    0; nPriority
446708  push    eax; hThread
446709  mov     [edi+ecx+4], eax
44670D  call    ds:SetThreadPriority
446713  inc     ebx
446714  add     edi, 7Ch ; '|'
446717  cmp     ebx, [ebp+arg_0]
44671A  jb      loc_446660
446720  mov     edi, [ebp+arg_0]
446723  cmp     edi, 1
446726  jbe     short loc_446755
446728  dec     edi
446729  mov     [ebp+arg_0], edi
44672C  mov     edi, ds:Sleep
446732  mov     ebx, 7Ch ; '|'
446737  mov     edx, [esi+24h]
44673A  cmp     dword ptr [ebx+edx], 0
44673E  jnz     short loc_44674D
446740  push    1; dwMilliseconds
446742  call    edi ; Sleep
446744  mov     eax, [esi+24h]
446747  cmp     dword ptr [ebx+eax], 0
44674B  jz      short loc_446740
44674D  add     ebx, 7Ch ; '|'
446750  dec     [ebp+arg_0]
446753  jnz     short loc_446737
446755  pop     edi
446756  mov     eax, esi
446758  pop     esi
446759  pop     ebx
44675A  mov     esp, ebp
44675C  pop     ebp
44675D  retn    4

;==== 446760 sub_446760
446760  push    ebp
446761  mov     ebp, esp
446763  push    esi
446764  mov     esi, ecx
446766  call    sub_446790
44676B  test    [ebp+arg_0], 1
44676F  jz      short loc_44677A
446771  push    esi; void *
446772  call    ??3@YAXPAX@Z; operator delete(void *)
446777  add     esp, 4
44677A  mov     eax, esi
44677C  pop     esi
44677D  pop     ebp
44677E  retn    4

;==== 446790 sub_446790
446790  push    ebp
446791  mov     ebp, esp
446793  sub     esp, 8
446796  push    ebx
446797  push    esi
446798  push    edi
446799  mov     esi, ecx
44679B  push    0
44679D  push    esi
44679E  mov     dword ptr [esi], offset ??_7DCDstrbtdPrcssng@@6B@; const DCDstrbtdPrcssng::`vftable'
4467A4  call    sub_446DB0
4467A9  mov     ebx, 1
4467AE  add     esp, 8
4467B1  mov     dword ptr [esi+1Ch], 0
4467B8  mov     edi, ebx
4467BA  cmp     [esi+4], ebx
4467BD  jbe     short loc_4467D3
4467BF  nop
4467C0  mov     edx, 2
4467C5  mov     ecx, edi
4467C7  call    sub_446BE0
4467CC  add     edi, ebx
4467CE  cmp     edi, [esi+4]
4467D1  jb      short loc_4467C0
4467D3  mov     eax, [esi+24h]
4467D6  mov     ecx, [eax+10h]
4467D9  push    ecx; hObject
4467DA  call    ds:CloseHandle
4467E0  mov     [ebp+var_8], ebx
4467E3  cmp     [esi+4], ebx
4467E6  jbe     short loc_44685F
4467E8  mov     ebx, 7Ch ; '|'
4467ED  lea     ecx, [ecx+0]
4467F0  mov     edx, [esi+24h]
4467F3  mov     eax, [edx+ebx+4]
4467F7  push    1; dwMilliseconds
4467F9  push    eax; hHandle
4467FA  call    ds:WaitForSingleObject
446800  cmp     eax, 102h
446805  jz      short loc_4467F0
446807  mov     ecx, [esi+24h]
44680A  mov     edx, [ecx+ebx+4]
44680E  push    edx; hObject
44680F  call    ds:CloseHandle
446815  mov     edi, ebx
446817  mov     [ebp+var_4], 4
44681E  mov     edi, edi
446820  mov     eax, [esi+24h]
446823  lea     ecx, [eax+edi+1Ch]
446827  push    ecx; lpCriticalSection
446828  call    ds:DeleteCriticalSection
44682E  add     edi, 18h
446831  dec     [ebp+var_4]
446834  jnz     short loc_446820
446836  mov     edx, [esi+24h]
446839  mov     eax, [edx+ebx+0Ch]
44683D  mov     edi, ds:CloseHandle
446843  push    eax; hObject
446844  call    edi ; CloseHandle
446846  mov     ecx, [esi+24h]
446849  mov     edx, [ecx+ebx+10h]
44684D  push    edx; hObject
44684E  call    edi ; CloseHandle
446850  mov     eax, [ebp+var_8]
446853  inc     eax
446854  add     ebx, 7Ch ; '|'
446857  mov     [ebp+var_8], eax
44685A  cmp     eax, [esi+4]
44685D  jb      short loc_4467F0
44685F  lea     eax, [esi+28h]
446862  push    eax; lpCriticalSection
446863  call    ds:DeleteCriticalSection
446869  mov     ecx, [esi+24h]
44686C  push    ecx; void *
44686D  call    ??3@YAXPAX@Z; operator delete(void *)
446872  add     esp, 4
446875  pop     edi
446876  pop     esi
446877  pop     ebx
446878  mov     esp, ebp
44687A  pop     ebp
44687B  retn

;==== 4468E0 sub_4468E0
4468E0  mov     eax, [eax+0Ch]
4468E3  retn

;==== 446900 sub_446900
446900  push    ebp
446901  mov     ebp, esp
446903  mov     edx, [ebp+arg_0]
446906  mov     [eax+14h], ecx
446909  mov     [eax+18h], edx
44690C  pop     ebp
44690D  retn    4

;==== 446910 sub_446910
446910  push    ebx
446911  push    esi
446912  mov     esi, ecx
446914  mov     ebx, 1
446919  push    edi
44691A  mov     [esi+20h], eax
44691D  cmp     [esi+4], ebx
446920  jbe     loc_4469F7
446926  test    eax, eax
446928  jz      loc_4469F7
44692E  push    ebx
44692F  push    esi
446930  mov     dword ptr [esi+40h], 0
446937  call    sub_446DB0
44693C  add     esp, 8
44693F  mov     edi, ebx
446941  cmp     [esi+4], ebx
446944  jbe     short loc_446962
446946  mov     edx, ebx
446948  mov     ecx, edi
44694A  call    sub_446BB0
44694F  mov     edx, 2
446954  mov     ecx, edi
446956  call    sub_446BE0
44695B  add     edi, ebx
44695D  cmp     edi, [esi+4]
446960  jb      short loc_446946
446962  mov     edi, ebx
446964  cmp     [esi+4], ebx
446967  jbe     short loc_446990
446969  lea     esp, [esp+0]
446970  mov     edx, 3
446975  mov     ecx, edi
446977  call    sub_446BB0
44697C  mov     edx, 3
446981  mov     ecx, edi
446983  call    sub_446BE0
446988  add     edi, ebx
44698A  cmp     edi, [esi+4]
44698D  jb      short loc_446970
44698F  nop
446990  xor     edi, edi
446992  call    sub_446C10
446997  test    eax, eax
446999  jnz     short loc_446990
44699B  push    eax
44699C  push    esi
44699D  call    sub_446DB0
4469A2  add     esp, 8
4469A5  mov     edi, ebx
4469A7  cmp     [esi+4], ebx
4469AA  jbe     short loc_4469CC
4469AC  lea     esp, [esp+0]
4469B0  mov     edx, 2
4469B5  mov     ecx, edi
4469B7  call    sub_446BB0
4469BC  mov     edx, ebx
4469BE  mov     ecx, edi
4469C0  call    sub_446BE0
4469C5  add     edi, ebx
4469C7  cmp     edi, [esi+4]
4469CA  jb      short loc_4469B0
4469CC  mov     edi, ebx
4469CE  cmp     [esi+4], ebx
4469D1  jbe     short loc_4469EC
4469D3  xor     edx, edx
4469D5  mov     ecx, edi
4469D7  call    sub_446BB0
4469DC  xor     edx, edx
4469DE  mov     ecx, edi
4469E0  call    sub_446BE0
4469E5  add     edi, ebx
4469E7  cmp     edi, [esi+4]
4469EA  jb      short loc_4469D3
4469EC  pop     edi
4469ED  mov     dword ptr [esi+20h], 0
4469F4  pop     esi
4469F5  pop     ebx
4469F6  retn
4469F7  xor     edi, edi
4469F9  call    sub_446C10
4469FE  test    eax, eax
446A00  jnz     short loc_4469F7
446A02  mov     [esi+20h], edi
446A05  pop     edi
446A06  pop     esi
446A07  pop     ebx
446A08  retn

;==== 446A10 sub_446A10
446A10  xor     eax, eax
446A12  cmp     dword ptr [ecx+4], 1
446A16  jbe     short locret_446A2C
446A18  cmp     [ecx+20h], eax
446A1B  jz      short locret_446A2C
446A1D  add     ecx, 28h ; '('
446A20  push    ecx; lpCriticalSection
446A21  call    ds:EnterCriticalSection
446A27  mov     eax, 1
446A2C  retn

;==== 446A30 sub_446A30
446A30  push    ebp
446A31  mov     ebp, esp
446A33  cmp     dword ptr [eax+4], 1
446A37  jbe     short loc_446A3F
446A39  cmp     dword ptr [eax+20h], 0
446A3D  jnz     short loc_446A45
446A3F  cmp     [ebp+arg_0], 0
446A43  jz      short loc_446A4F
446A45  add     eax, 28h ; '('
446A48  push    eax; lpCriticalSection
446A49  call    ds:LeaveCriticalSection
446A4F  pop     ebp
446A50  retn    4

;==== 446A60 sub_446A60
446A60  push    ebp
446A61  mov     ebp, esp
446A63  push    ecx
446A64  push    ebx
446A65  push    edi
446A66  mov     edi, eax
446A68  xor     ebx, ebx
446A6A  mov     [ebp+var_4], ebx
446A6D  cmp     edi, [esi+4]
446A70  jnb     loc_446B0C
446A76  mov     ecx, esi
446A78  call    sub_446A10
446A7D  cmp     [esi+40h], ebx
446A80  jnz     loc_446B03
446A86  mov     edx, [esi+4]
446A89  xor     eax, eax
446A8B  test    edx, edx
446A8D  jz      short loc_446B03
446A8F  xor     ecx, ecx
446A91  cmp     eax, edi
446A93  jz      short loc_446AA2
446A95  mov     ebx, [esi+24h]
446A98  cmp     dword ptr [ebx+ecx+14h], 0
446A9D  jz      short loc_446ABB
446A9F  mov     ebx, [ebp+var_4]
446AA2  inc     eax
446AA3  add     ecx, 7Ch ; '|'
446AA6  cmp     eax, edx
446AA8  jb      short loc_446A91
446AAA  push    1
446AAC  mov     eax, esi
446AAE  call    sub_446A30
446AB3  pop     edi
446AB4  mov     eax, ebx
446AB6  pop     ebx
446AB7  mov     esp, ebp
446AB9  pop     ebp
446ABA  retn
446ABB  imul    edi, 7Ch ; '|'
446ABE  mov     eax, ebx
446AC0  mov     ebx, 1
446AC5  mov     [eax+edi+14h], ebx
446AC9  push    ebx
446ACA  mov     eax, esi
446ACC  call    sub_446A30
446AD1  mov     ecx, [esi+24h]
446AD4  mov     edx, [ecx+edi+10h]
446AD8  push    0; bAlertable
446ADA  push    0FFFFFFFFh; dwMilliseconds
446ADC  push    edx; hHandle
446ADD  call    ds:WaitForSingleObjectEx
446AE3  mov     eax, [esi+24h]
446AE6  mov     ecx, [eax+edi+10h]
446AEA  push    ecx; hEvent
446AEB  call    ds:ResetEvent
446AF1  mov     ecx, esi
446AF3  call    sub_446A10
446AF8  mov     edx, [esi+24h]
446AFB  mov     dword ptr [edx+edi+14h], 0
446B03  push    1
446B05  mov     eax, esi
446B07  call    sub_446A30
446B0C  pop     edi
446B0D  mov     eax, ebx
446B0F  pop     ebx
446B10  mov     esp, ebp
446B12  pop     ebp
446B13  retn

;==== 446B20 sub_446B20
446B20  push    ebp
446B21  mov     ebp, esp
446B23  push    ebx
446B24  mov     ebx, [ebp+arg_0]
446B27  push    esi
446B28  mov     ecx, edi
446B2A  call    sub_446A10
446B2F  cmp     dword ptr [edi+40h], 0
446B33  jnz     short loc_446B3D
446B35  test    ebx, ebx
446B37  jz      short loc_446B3D
446B39  xor     eax, eax
446B3B  jmp     short loc_446B42
446B3D  mov     eax, 1
446B42  mov     [edi+40h], eax
446B45  cmp     ebx, 1
446B48  jb      short loc_446B4F
446B4A  cmp     ebx, [edi+4]
446B4D  jbe     short loc_446B52
446B4F  mov     ebx, [edi+4]
446B52  xor     esi, esi
446B54  test    ebx, ebx
446B56  jz      short loc_446B94
446B58  mov     [ebp+arg_0], ebx
446B5B  mov     ebx, ds:SetEvent
446B61  cmp     esi, [edi+4]
446B64  jnb     short loc_446B8F
446B66  mov     ecx, [edi+24h]
446B69  mov     eax, esi
446B6B  imul    eax, 7Ch ; '|'
446B6E  lea     eax, [eax+ecx+14h]
446B72  cmp     dword ptr [eax], 0
446B75  jnz     short loc_446B82
446B77  inc     esi
446B78  add     eax, 7Ch ; '|'
446B7B  cmp     esi, [edi+4]
446B7E  jb      short loc_446B72
446B80  jmp     short loc_446B8F
446B82  mov     edx, esi
446B84  imul    edx, 7Ch ; '|'
446B87  mov     eax, [edx+ecx+10h]
446B8B  push    eax; hEvent
446B8C  call    ebx ; SetEvent
446B8E  inc     esi
446B8F  dec     [ebp+arg_0]
446B92  jnz     short loc_446B61
446B94  push    1
446B96  mov     eax, edi
446B98  call    sub_446A30
446B9D  pop     esi
446B9E  pop     ebx
446B9F  pop     ebp
446BA0  retn    4

;==== 446BB0 sub_446BB0
446BB0  xor     eax, eax
446BB2  cmp     ecx, 1
446BB5  jb      short locret_446BD8
446BB7  cmp     ecx, [esi+4]
446BBA  jnb     short locret_446BD8
446BBC  imul    ecx, 7Ch ; '|'
446BBF  lea     eax, [edx+edx*2]
446BC2  mov     edx, [esi+24h]
446BC5  lea     ecx, [ecx+eax*8]
446BC8  lea     eax, [ecx+edx+1Ch]
446BCC  push    eax; lpCriticalSection
446BCD  call    ds:EnterCriticalSection
446BD3  mov     eax, 1
446BD8  retn

;==== 446BE0 sub_446BE0
446BE0  xor     eax, eax
446BE2  cmp     ecx, 1
446BE5  jb      short locret_446C08
446BE7  cmp     ecx, [esi+4]
446BEA  jnb     short locret_446C08
446BEC  imul    ecx, 7Ch ; '|'
446BEF  lea     eax, [edx+edx*2]
446BF2  mov     edx, [esi+24h]
446BF5  lea     ecx, [ecx+eax*8]
446BF8  lea     eax, [ecx+edx+1Ch]
446BFC  push    eax; lpCriticalSection
446BFD  call    ds:LeaveCriticalSection
446C03  mov     eax, 1
446C08  retn

;==== 446C10 sub_446C10
446C10  xor     eax, eax
446C12  cmp     [esi+10h], eax
446C15  jnz     short loc_446C1C
446C17  cmp     [esi+14h], eax
446C1A  jz      short locret_446C56
446C1C  cmp     edi, [esi+8]
446C1F  jb      short loc_446C38
446C21  mov     ecx, [esi+24h]
446C24  mov     eax, edi
446C26  imul    eax, 7Ch ; '|'
446C29  mov     edx, [eax+ecx+0Ch]
446C2D  push    0; bAlertable
446C2F  push    0FFFFFFFFh; dwMilliseconds
446C31  push    edx; hHandle
446C32  call    ds:WaitForSingleObjectEx
446C38  mov     eax, [esi+10h]
446C3B  test    eax, eax
446C3D  jz      short loc_446C49
446C3F  mov     ecx, [esi+18h]
446C42  push    ecx
446C43  call    eax
446C45  add     esp, 4
446C48  retn
446C49  mov     edx, [esi+18h]
446C4C  mov     eax, [esi+14h]
446C4F  push    edi
446C50  push    edx
446C51  call    eax
446C53  add     esp, 8
446C56  retn

;==== 446C60 sub_446C60
446C60  push    ebp
446C61  mov     ebp, esp
446C63  push    ebx
446C64  mov     ebx, [ebp+arg_0]
446C67  xor     eax, eax
446C69  test    ebx, ebx
446C6B  jnz     short loc_446C75
446C6D  mov     ecx, [esi+4]
446C70  mov     [ebp+arg_0], ecx
446C73  mov     ebx, ecx
446C75  cmp     ebx, 1
446C78  jb      short loc_446CE1
446C7A  cmp     ebx, [esi+4]
446C7D  ja      short loc_446CE1
446C7F  mov     eax, [esi+8]
446C82  push    edi
446C83  cmp     eax, ebx
446C85  jbe     short loc_446CB8
446C87  mov     edi, ebx
446C89  imul    edi, 7Ch ; '|'
446C8C  lea     esp, [esp+0]
446C90  mov     edx, [esi+24h]
446C93  mov     eax, [edi+edx+0Ch]
446C97  push    eax; hEvent
446C98  call    ds:ResetEvent
446C9E  inc     ebx
446C9F  add     edi, 7Ch ; '|'
446CA2  cmp     ebx, [esi+8]
446CA5  jb      short loc_446C90
446CA7  mov     ebx, [ebp+arg_0]
446CAA  pop     edi
446CAB  mov     [esi+8], ebx
446CAE  mov     eax, 1
446CB3  pop     ebx
446CB4  pop     ebp
446CB5  retn    4
446CB8  jnb     short loc_446CD8
446CBA  mov     edi, eax
446CBC  imul    edi, 7Ch ; '|'
446CBF  sub     ebx, eax
446CC1  mov     ecx, [esi+24h]
446CC4  mov     edx, [edi+ecx+0Ch]
446CC8  push    edx; hEvent
446CC9  call    ds:SetEvent
446CCF  add     edi, 7Ch ; '|'
446CD2  dec     ebx
446CD3  jnz     short loc_446CC1
446CD5  mov     ebx, [ebp+arg_0]
446CD8  mov     [esi+8], ebx
446CDB  mov     eax, 1
446CE0  pop     edi
446CE1  pop     ebx
446CE2  pop     ebp
446CE3  retn    4

;==== 446CF0 sub_446CF0
446CF0  push    ebp
446CF1  mov     ebp, esp
446CF3  push    ebx
446CF4  mov     ebx, [ebp+arg_0]
446CF7  push    esi
446CF8  mov     esi, [ebx+18h]
446CFB  push    edi
446CFC  mov     edi, [ebx+8]
446CFF  xor     edx, edx
446D01  mov     ecx, edi
446D03  call    sub_446BB0
446D08  mov     eax, esi
446D0A  call    sub_493C80
446D0F  test    eax, eax
446D11  jz      short loc_446D8A
446D13  call    sub_446C10
446D18  test    eax, eax
446D1A  jnz     short loc_446D7F
446D1C  lea     edx, [eax+1]
446D1F  mov     ecx, edi
446D21  call    sub_446BB0
446D26  mov     edx, 1
446D2B  mov     ecx, edi
446D2D  call    sub_446BE0
446D32  mov     edx, 3
446D37  mov     ecx, edi
446D39  call    sub_446BB0
446D3E  xor     edx, edx
446D40  mov     ecx, edi
446D42  call    sub_446BE0
446D47  cmp     dword ptr [ebx], 0
446D4A  jnz     short loc_446D52
446D4C  mov     dword ptr [ebx], 1
446D52  mov     edx, 2
446D57  mov     ecx, edi
446D59  call    sub_446BB0
446D5E  mov     edx, 2
446D63  mov     ecx, edi
446D65  call    sub_446BE0
446D6A  xor     edx, edx
446D6C  mov     ecx, edi
446D6E  call    sub_446BB0
446D73  mov     edx, 3
446D78  mov     ecx, edi
446D7A  call    sub_446BE0
446D7F  mov     eax, esi
446D81  call    sub_493C80
446D86  test    eax, eax
446D88  jnz     short loc_446D13
446D8A  xor     edx, edx
446D8C  mov     ecx, edi
446D8E  call    sub_446BE0
446D93  push    0; ReturnCode
446D95  call    __endthreadex

;==== 446DB0 sub_446DB0
446DB0  push    ebp
446DB1  mov     ebp, esp
446DB3  sub     esp, 34h
446DB6  push    ebx
446DB7  push    esi
446DB8  push    edi
446DB9  push    offset stru_50EA50; lpCriticalSection
446DBE  call    ds:EnterCriticalSection
446DC4  mov     edi, dword_565D7C
446DCA  mov     ebx, offset dword_565D7C
446DCF  mov     [ebp+var_4], 0
446DD6  test    edi, edi
446DD8  jz      short loc_446E23
446DDA  lea     ebx, [ebx+0]
446DE0  mov     esi, [edi]
446DE2  cmp     [ebp+arg_0], esi
446DE5  jnz     short loc_446E04
446DE7  cmp     [ebp+arg_4], 0
446DEB  jnz     short loc_446DF4
446DED  push    0
446DEF  call    sub_446C60
446DF4  mov     eax, [edi+4]
446DF7  push    edi; void *
446DF8  mov     [ebx], eax
446DFA  call    ??3@YAXPAX@Z; operator delete(void *)
446DFF  add     esp, 4
446E02  jmp     short loc_446E1D
446E04  mov     eax, esi
446E06  call    sub_454260
446E0B  mov     ecx, eax
446E0D  mov     eax, esi
446E0F  call    sub_4468E0
446E14  imul    ecx, eax
446E17  add     [ebp+var_4], ecx
446E1A  lea     ebx, [edi+4]
446E1D  mov     edi, [ebx]
446E1F  test    edi, edi
446E21  jnz     short loc_446DE0
446E23  cmp     [ebp+arg_4], 0
446E27  jz      short loc_446E5E
446E29  push    8; Size
446E2B  call    ??2@YAPAXI@Z; operator new(uint)
446E30  mov     ecx, [ebp+arg_0]
446E33  mov     edx, eax
446E35  mov     [edx], ecx
446E37  mov     eax, dword_565D7C
446E3C  mov     [edx+4], eax
446E3F  add     esp, 4
446E42  mov     eax, ecx
446E44  call    sub_454260
446E49  mov     esi, eax
446E4B  mov     eax, ecx
446E4D  call    sub_4468E0
446E52  imul    esi, eax
446E55  add     [ebp+var_4], esi
446E58  mov     dword_565D7C, edx
446E5E  cmp     [ebp+var_4], 1
446E62  jb      loc_446EF9
446E68  xor     esi, esi
446E6A  lea     ecx, [ebp+SystemInfo]
446E6D  push    ecx; lpSystemInfo
446E6E  mov     [ebp+arg_4], esi
446E71  mov     [ebp+var_8], esi
446E74  mov     [ebp+var_C], esi
446E77  call    ds:GetSystemInfo
446E7D  mov     ebx, dword_565D7C
446E83  test    ebx, ebx
446E85  jz      short loc_446EE2
446E87  mov     esi, [ebx]
446E89  mov     eax, esi
446E8B  call    sub_454260
446E90  mov     edx, eax
446E92  mov     eax, esi
446E94  call    sub_4468E0
446E99  imul    eax, edx
446E9C  imul    eax, [ebp+SystemInfo.dwNumberOfProcessors]
446EA0  xor     edx, edx
446EA2  div     [ebp+var_4]
446EA5  mov     edi, eax
446EA7  cmp     edi, 1
446EAA  jnb     short loc_446EB1
446EAC  mov     edi, 1
446EB1  push    edi
446EB2  call    sub_446C60
446EB7  mov     ecx, [ebx]
446EB9  add     [ebp+arg_4], edi
446EBC  mov     eax, ecx
446EBE  call    sub_4468E0
446EC3  cmp     [ebp+var_8], eax
446EC6  jnb     short loc_446ED8
446EC8  mov     eax, ecx
446ECA  call    sub_4468E0
446ECF  mov     [ebp+var_8], eax
446ED2  mov     [ebp+var_10], edi
446ED5  mov     [ebp+var_C], ecx
446ED8  mov     ebx, [ebx+4]
446EDB  test    ebx, ebx
446EDD  jnz     short loc_446E87
446EDF  mov     esi, [ebp+arg_4]
446EE2  mov     ecx, [ebp+SystemInfo.dwNumberOfProcessors]
446EE5  cmp     esi, ecx
446EE7  jnb     short loc_446EF9
446EE9  mov     eax, [ebp+var_10]
446EEC  sub     eax, esi
446EEE  mov     esi, [ebp+var_C]
446EF1  add     eax, ecx
446EF3  push    eax
446EF4  call    sub_446C60
446EF9  push    offset stru_50EA50; lpCriticalSection
446EFE  call    ds:LeaveCriticalSection
446F04  pop     edi
446F05  pop     esi
446F06  pop     ebx
446F07  mov     esp, ebp
446F09  pop     ebp
446F0A  retn

;==== 452B30 sub_452B30
452B30  push    ebp
452B31  mov     ebp, esp
452B33  sub     esp, 98h
452B39  mov     eax, dword_4FB734
452B3E  xor     eax, ebp
452B40  mov     [ebp+var_4], eax
452B43  push    94h; Size
452B48  lea     eax, [ebp+VersionInformation]
452B4E  push    0; Val
452B50  push    eax; void *
452B51  mov     dword ptr [esi], offset ??_7DCSleep@@6B@; const DCSleep::`vftable'
452B57  call    _memset
452B5C  add     esp, 0Ch
452B5F  lea     ecx, [ebp+VersionInformation]
452B65  push    ecx; lpVersionInformation
452B66  mov     [ebp+VersionInformation.dwOSVersionInfoSize], 94h
452B70  call    ds:GetVersionExA
452B76  cmp     [ebp+VersionInformation.dwPlatformId], 2
452B7D  jnz     short loc_452B9E
452B7F  push    0; lpTimerName
452B81  push    1; bManualReset
452B83  push    0; lpTimerAttributes
452B85  call    ds:CreateWaitableTimerA
452B8B  mov     [esi+4], eax
452B8E  mov     eax, esi
452B90  mov     ecx, [ebp+var_4]
452B93  xor     ecx, ebp
452B95  call    sub_4ABDD5
452B9A  mov     esp, ebp
452B9C  pop     ebp
452B9D  retn
452B9E  mov     ecx, [ebp+var_4]
452BA1  xor     ecx, ebp
452BA3  mov     dword ptr [esi+4], 0
452BAA  mov     eax, esi
452BAC  call    sub_4ABDD5
452BB1  mov     esp, ebp
452BB3  pop     ebp
452BB4  retn

;==== 452BC0 sub_452BC0
452BC0  push    ebp
452BC1  mov     ebp, esp
452BC3  push    esi
452BC4  mov     esi, ecx
452BC6  call    sub_452BF0
452BCB  test    [ebp+arg_0], 1
452BCF  jz      short loc_452BDA
452BD1  push    esi; void *
452BD2  call    ??3@YAXPAX@Z; operator delete(void *)
452BD7  add     esp, 4
452BDA  mov     eax, esi
452BDC  pop     esi
452BDD  pop     ebp
452BDE  retn    4

;==== 452BF0 sub_452BF0
452BF0  push    esi
452BF1  mov     esi, ecx
452BF3  mov     eax, [esi+4]
452BF6  mov     dword ptr [esi], offset ??_7DCSleep@@6B@; const DCSleep::`vftable'
452BFC  test    eax, eax
452BFE  jz      short loc_452C0E
452C00  push    eax; hObject
452C01  call    ds:CloseHandle
452C07  mov     dword ptr [esi+4], 0
452C0E  pop     esi
452C0F  retn

;==== 452C10 sub_452C10
452C10  push    ebp
452C11  mov     ebp, esp
452C13  and     esp, 0FFFFFFF8h
452C16  sub     esp, 0Ch
452C19  push    edi
452C1A  mov     edi, ds:SwitchToThread
452C20  cmp     dword ptr [esi+4], 0
452C24  jz      short loc_452C2A
452C26  test    eax, eax
452C28  jnz     short loc_452C37
452C2A  cmp     eax, 1
452C2D  jnb     short loc_452C20
452C2F  call    edi ; SwitchToThread
452C31  test    eax, eax
452C33  jnz     short loc_452C77
452C35  jmp     short loc_452C20
452C37  cmp     eax, 1
452C3A  jz      short loc_452C44
452C3C  imul    eax, 0FFFFD8F0h
452C42  jmp     short loc_452C49
452C44  mov     eax, 0FFFFEC78h
452C49  mov     ecx, [esi+4]
452C4C  push    0; fResume
452C4E  push    0; lpArgToCompletionRoutine
452C50  cdq
452C51  push    0; pfnCompletionRoutine
452C53  mov     dword ptr [esp+1Ch+DueTime], eax
452C57  push    0; lPeriod
452C59  lea     eax, [esp+20h+DueTime]
452C5D  push    eax; lpDueTime
452C5E  push    ecx; hTimer
452C5F  mov     dword ptr [esp+28h+DueTime+4], edx
452C63  call    ds:SetWaitableTimer
452C69  mov     edx, [esi+4]
452C6C  push    1; bAlertable
452C6E  push    0Ah; dwMilliseconds
452C70  push    edx; hHandle
452C71  call    ds:WaitForSingleObjectEx
452C77  pop     edi
452C78  mov     esp, ebp
452C7A  pop     ebp
452C7B  retn

;==== 4531E0 sub_4531E0
4531E0  push    edi
4531E1  mov     [esi+4], eax
4531E4  xor     eax, eax
4531E6  push    34h ; '4'; Size
4531E8  mov     dword ptr [esi], offset ??_7DCTELgclFldMngr@@6B@; const DCTELgclFldMngr::`vftable'
4531EE  mov     [esi+10h], eax
4531F1  mov     [esi+18h], eax
4531F4  push    eax; Val
4531F5  lea     edi, [esi+20h]
4531F8  mov     [esi+1Ch], eax
4531FB  push    edi; void *
4531FC  mov     dword ptr [esi+14h], 10000000h
453203  call    _memset
453208  add     esp, 0Ch
45320B  mov     dword ptr [edi], 20000000h
453211  mov     eax, esi
453213  pop     edi
453214  retn

;==== 453220 sub_453220
453220  push    ebp
453221  mov     ebp, esp
453223  push    esi
453224  mov     esi, ecx
453226  call    sub_453250
45322B  test    [ebp+arg_0], 1
45322F  jz      short loc_45323A
453231  push    esi; void *
453232  call    ??3@YAXPAX@Z; operator delete(void *)
453237  add     esp, 4
45323A  mov     eax, esi
45323C  pop     esi
45323D  pop     ebp
45323E  retn    4

;==== 453250 sub_453250
453250  push    edi
453251  mov     edi, ecx
453253  mov     dword ptr [ecx], offset ??_7DCTELgclFldMngr@@6B@; const DCTELgclFldMngr::`vftable'
453259  call    sub_4542C0
45325E  pop     edi
45325F  retn

;==== 453260 sub_453260
453260  cmp     dword ptr [esi+10h], 0
453264  jnz     short loc_453269
453266  xor     eax, eax
453268  retn
453269  mov     eax, [esi]
45326B  mov     edx, [eax+20h]
45326E  push    edi
45326F  mov     ecx, esi
453271  call    edx
453273  mov     ecx, [esi+1Ch]
453276  mov     edx, [esi+8]
453279  mov     edi, eax
45327B  mov     eax, [esi+50h]
45327E  push    eax; int
45327F  mov     eax, [esi+10h]
453282  push    ecx; Size
453283  mov     ecx, [esi+0Ch]; int
453286  push    edx; int
453287  push    edi; int
453288  call    sub_4532A0
45328D  mov     eax, edi
45328F  pop     edi
453290  retn

;==== 4532A0 sub_4532A0
4532A0  push    ebp
4532A1  mov     ebp, esp
4532A3  sub     esp, 0Ch
4532A6  push    esi
4532A7  mov     esi, [ebp+arg_0]
4532AA  push    edi
4532AB  mov     edi, [ebp+Size]
4532AE  push    eax; Src
4532AF  mov     eax, [ebp+arg_4]
4532B2  push    ecx; int
4532B3  mov     ecx, esi
4532B5  call    sub_453480
4532BA  mov     [ebp+var_8], eax
4532BD  test    eax, eax
4532BF  jnz     loc_453474
4532C5  push    ebx
4532C6  mov     ebx, [esi+0Ch]
4532C9  imul    ebx, [esi+8]
4532CD  lea     ecx, [esi+14h]
4532D0  mov     [ebp+arg_4], ebx
4532D3  mov     [ebp+Size], eax
4532D6  mov     [ebp+var_4], ecx
4532D9  test    edi, edi
4532DB  jz      short loc_453344
4532DD  lea     ecx, [ecx+0]
4532E0  push    0Ch; Size
4532E2  call    ??2@YAPAXI@Z; operator new(uint)
4532E7  mov     edx, [edi]
4532E9  mov     esi, eax
4532EB  mov     [esi], edx
4532ED  xor     ecx, ecx
4532EF  mov     eax, ebx
4532F1  mov     edx, 4
4532F6  mul     edx
4532F8  seto    cl
4532FB  neg     ecx
4532FD  or      ecx, eax
4532FF  push    ecx; Size
453300  call    ??2@YAPAXI@Z; operator new(uint)
453305  lea     ecx, ds:0[ebx*4]
45330C  push    ecx; Size
45330D  mov     [esi+4], eax
453310  mov     dword ptr [esi+8], 0
453317  mov     ecx, [edi+4]
45331A  push    ecx; Src
45331B  push    eax; void *
45331C  call    _memcpy_0
453321  mov     eax, [edi]
453323  add     esp, 14h
453326  cmp     [ebp+Size], eax
453329  jnb     short loc_45332E
45332B  mov     [ebp+Size], eax
45332E  mov     edx, [ebp+var_4]
453331  mov     [edx+8], esi
453334  mov     edi, [edi+8]
453337  mov     [ebp+var_4], esi
45333A  test    edi, edi
45333C  jnz     short loc_4532E0
45333E  mov     eax, [ebp+var_8]
453341  mov     esi, [ebp+arg_0]
453344  mov     ecx, [ebp+Size]
453347  inc     ecx
453348  mov     [esi+14h], ecx
45334B  mov     ecx, [ebp+arg_C]
45334E  add     esi, 20h ; ' '
453351  mov     [ebp+var_4], esi
453354  mov     [ebp+arg_0], ecx
453357  test    ecx, ecx
453359  jz      loc_453473
45335F  nop
453360  push    34h ; '4'; Size
453362  call    ??2@YAPAXI@Z; operator new(uint)
453367  mov     esi, [ebp+arg_0]
45336A  mov     ecx, 0Dh
45336F  mov     edi, eax
453371  rep movsd
453373  xor     edx, edx
453375  lea     ecx, [eax+2Ch]
453378  mov     [ecx], edx
45337A  mov     [ebp+arg_C], ecx
45337D  mov     ecx, [ebp+arg_0]
453380  mov     [eax+28h], edx
453383  mov     [eax+30h], edx
453386  mov     esi, [ecx+2Ch]
453389  add     esp, 4
45338C  mov     [ebp+var_C], eax
45338F  cmp     esi, edx
453391  jz      loc_453456
453397  lea     eax, ds:0[ebx*8]
45339E  sub     eax, ebx
4533A0  add     eax, eax
4533A2  add     eax, eax
4533A4  mov     [ebp+Size], eax
4533A7  push    20h ; ' '; Size
4533A9  call    ??2@YAPAXI@Z; operator new(uint)
4533AE  mov     edx, [esi]
4533B0  mov     edi, eax
4533B2  mov     [edi], edx
4533B4  mov     eax, [esi+4]
4533B7  mov     [edi+4], eax
4533BA  mov     ecx, [esi+8]
4533BD  mov     [edi+8], ecx
4533C0  mov     edx, [esi+0Ch]
4533C3  mov     [edi+0Ch], edx
4533C6  mov     eax, [esi+10h]
4533C9  mov     [edi+10h], eax
4533CC  xor     ecx, ecx
4533CE  mov     eax, ebx
4533D0  mov     edx, 1Ch
4533D5  mul     edx
4533D7  seto    cl
4533DA  neg     ecx
4533DC  or      ecx, eax
4533DE  push    ecx; Size
4533DF  call    ??2@YAPAXI@Z; operator new(uint)
4533E4  mov     ecx, [ebp+Size]
4533E7  lea     ebx, [edi+1Ch]
4533EA  mov     [edi+14h], eax
4533ED  mov     dword ptr [ebx], 0
4533F3  mov     edx, [esi+14h]
4533F6  push    ecx; Size
4533F7  push    edx; Src
4533F8  push    eax; void *
4533F9  call    _memcpy_0
4533FE  add     esp, 14h
453401  cmp     dword ptr [esi+18h], 0
453405  jz      short loc_453436
453407  mov     eax, [ebp+arg_4]
45340A  xor     ecx, ecx
45340C  mov     edx, 1Ch
453411  mul     edx
453413  seto    cl
453416  neg     ecx
453418  or      ecx, eax
45341A  push    ecx; Size
45341B  call    ??2@YAPAXI@Z; operator new(uint)
453420  mov     ecx, [ebp+Size]
453423  mov     [edi+18h], eax
453426  mov     edx, [esi+18h]
453429  push    ecx; Size
45342A  push    edx; Src
45342B  push    eax; void *
45342C  call    _memcpy_0
453431  add     esp, 10h
453434  jmp     short loc_45343D
453436  mov     dword ptr [edi+18h], 0
45343D  mov     eax, [ebp+arg_C]
453440  mov     [eax], edi
453442  mov     esi, [esi+1Ch]
453445  mov     [ebp+arg_C], ebx
453448  mov     ebx, [ebp+arg_4]
45344B  test    esi, esi
45344D  jnz     loc_4533A7
453453  mov     eax, [ebp+var_C]
453456  mov     ecx, [ebp+var_4]
453459  mov     edx, [ebp+arg_0]
45345C  mov     [ecx+30h], eax
45345F  mov     [ebp+var_4], eax
453462  mov     eax, [edx+30h]
453465  mov     [ebp+arg_0], eax
453468  test    eax, eax
45346A  jnz     loc_453360
453470  mov     eax, [ebp+var_8]
453473  pop     ebx
453474  pop     edi
453475  pop     esi
453476  mov     esp, ebp
453478  pop     ebp
453479  retn    10h

;==== 453480 sub_453480
453480  push    ebp
453481  mov     ebp, esp
453483  push    ebx
453484  mov     ebx, [ebp+arg_0]
453487  push    esi
453488  mov     esi, eax
45348A  push    edi
45348B  mov     edi, ecx
45348D  test    esi, esi
45348F  jz      short loc_4534EA
453491  test    ebx, ebx
453493  jz      short loc_4534EA
453495  call    sub_4542C0
45349A  mov     eax, [edi]
45349C  mov     edx, [eax+30h]
45349F  mov     ecx, edi
4534A1  call    edx
4534A3  mov     [edi+8], esi
4534A6  imul    esi, ebx
4534A9  xor     ecx, ecx
4534AB  mov     eax, esi
4534AD  mov     edx, 10h
4534B2  mul     edx
4534B4  seto    cl
4534B7  mov     [edi+0Ch], ebx
4534BA  neg     ecx
4534BC  or      ecx, eax
4534BE  push    ecx; Size
4534BF  call    ??2@YAPAXI@Z; operator new(uint)
4534C4  mov     ecx, [ebp+Src]
4534C7  shl     esi, 4
4534CA  push    esi; Size
4534CB  push    ecx; Src
4534CC  push    eax; void *
4534CD  mov     [edi+10h], eax
4534D0  call    _memcpy_0
4534D5  mov     edx, [edi]
4534D7  mov     eax, [edx+2Ch]
4534DA  add     esp, 10h
4534DD  mov     ecx, edi
4534DF  call    eax
4534E1  pop     edi
4534E2  pop     esi
4534E3  xor     eax, eax
4534E5  pop     ebx
4534E6  pop     ebp
4534E7  retn    8
4534EA  pop     edi
4534EB  pop     esi
4534EC  mov     eax, 80000001h
4534F1  pop     ebx
4534F2  pop     ebp
4534F3  retn    8

;==== 453500 sub_453500
453500  push    ebp
453501  mov     ebp, esp
453503  cmp     dword ptr [eax+10h], 0
453507  jnz     short loc_453512
453509  mov     eax, 80000002h
45350E  pop     ebp
45350F  retn    4
453512  mov     ecx, [eax+8]
453515  mov     eax, [eax+0Ch]
453518  mov     [edx], ecx
45351A  mov     ecx, [ebp+arg_0]
45351D  mov     [ecx], eax
45351F  xor     eax, eax
453521  pop     ebp
453522  retn    4

;==== 453530 sub_453530
453530  push    ebp
453531  mov     ebp, esp
453533  cmp     dword ptr [esi+10h], 0
453537  jnz     short loc_453542
453539  mov     eax, 80000002h
45353E  pop     ebp
45353F  retn    8
453542  push    edi
453543  push    0Ch; Size
453545  call    ??2@YAPAXI@Z; operator new(uint)
45354A  mov     edi, eax
45354C  mov     eax, [esi+14h]
45354F  mov     [edi], eax
453551  mov     eax, [esi+8]
453554  imul    eax, [esi+0Ch]
453558  xor     ecx, ecx
45355A  mov     edx, 4
45355F  mul     edx
453561  seto    cl
453564  neg     ecx
453566  or      ecx, eax
453568  push    ecx; Size
453569  call    ??2@YAPAXI@Z; operator new(uint)
45356E  mov     [edi+4], eax
453571  mov     ecx, [esi+1Ch]
453574  mov     [edi+8], ecx
453577  mov     edx, [esi+8]
45357A  imul    edx, [esi+0Ch]
45357E  mov     ecx, [ebp+Src]
453581  add     edx, edx
453583  add     edx, edx
453585  push    edx; Size
453586  push    ecx; Src
453587  push    eax; void *
453588  call    _memcpy_0
45358D  inc     dword ptr [esi+14h]
453590  mov     eax, [ebp+arg_0]
453593  mov     [esi+1Ch], edi
453596  mov     edx, [edi]
453598  add     esp, 14h
45359B  mov     [eax], edx
45359D  xor     eax, eax
45359F  pop     edi
4535A0  pop     ebp
4535A1  retn    8

;==== 4535B0 sub_4535B0
4535B0  push    ebp
4535B1  mov     ebp, esp
4535B3  cmp     dword ptr [edi+10h], 0
4535B7  jnz     short loc_4535C2
4535B9  mov     eax, 80000002h
4535BE  pop     ebp
4535BF  retn    4
4535C2  push    esi
4535C3  push    34h ; '4'; Size
4535C5  call    ??2@YAPAXI@Z; operator new(uint)
4535CA  push    34h ; '4'; Size
4535CC  mov     esi, eax
4535CE  push    0; Val
4535D0  push    esi; void *
4535D1  call    _memset
4535D6  mov     eax, [edi+20h]
4535D9  mov     [esi], eax
4535DB  or      eax, 0FFFFFFFFh
4535DE  mov     [esi+4], eax
4535E1  mov     [esi+8], eax
4535E4  mov     dword ptr [esi+0Ch], 0
4535EB  mov     dword ptr [esi+10h], 3
4535F2  mov     dword ptr [esi+14h], 80h
4535F9  mov     dword ptr [esi+20h], 0
453600  mov     eax, 1
453605  mov     [esi+24h], eax
453608  mov     dword ptr [esi+28h], 0
45360F  mov     ecx, [edi+50h]
453612  mov     [esi+30h], ecx
453615  add     [edi+20h], eax
453618  mov     eax, [ebp+arg_0]
45361B  mov     [edi+50h], esi
45361E  mov     edx, [esi]
453620  add     esp, 10h
453623  mov     [eax], edx
453625  xor     eax, eax
453627  pop     esi
453628  pop     ebp
453629  retn    4

;==== 453630 sub_453630
453630  push    esi
453631  mov     esi, [edx+50h]
453634  push    edi
453635  mov     eax, 80000003h
45363A  lea     edi, [edx+20h]
45363D  test    esi, esi
45363F  jz      short loc_453678
453641  cmp     ecx, [esi]
453643  jz      short loc_453651
453645  mov     edi, esi
453647  mov     esi, [esi+30h]
45364A  test    esi, esi
45364C  jnz     short loc_453641
45364E  pop     edi
45364F  pop     esi
453650  retn
453651  mov     eax, edx
453653  call    sub_454320
453658  mov     eax, [esi+30h]
45365B  mov     [edi+30h], eax
45365E  mov     ecx, [esi+28h]
453661  test    ecx, ecx
453663  jz      short loc_45366D
453665  mov     edx, [ecx]
453667  mov     eax, [edx]
453669  push    1
45366B  call    eax
45366D  push    esi; void *
45366E  call    ??3@YAXPAX@Z; operator delete(void *)
453673  add     esp, 4
453676  xor     eax, eax
453678  pop     edi
453679  pop     esi
45367A  retn

;==== 453680 sub_453680
453680  push    esi
453681  call    sub_4542A0
453686  mov     esi, eax
453688  test    esi, esi
45368A  jz      short loc_4536A6
45368C  mov     ecx, [esi+28h]
45368F  test    ecx, ecx
453691  jz      short loc_45369B
453693  mov     eax, [ecx]
453695  mov     edx, [eax]
453697  push    1
453699  call    edx
45369B  mov     dword ptr [esi+28h], 0
4536A2  xor     eax, eax
4536A4  pop     esi
4536A5  retn
4536A6  mov     eax, 80000003h
4536AB  pop     esi
4536AC  retn

;==== 4536B0 sub_4536B0
4536B0  mov     eax, edx
4536B2  call    sub_4542A0
4536B7  test    eax, eax
4536B9  jz      short loc_4536DC
4536BB  test    edi, edi
4536BD  js      short loc_4536D6
4536BF  cmp     edi, [edx+8]
4536C2  jge     short loc_4536D6
4536C4  test    esi, esi
4536C6  js      short loc_4536D6
4536C8  cmp     esi, [edx+0Ch]
4536CB  jge     short loc_4536D6
4536CD  mov     [eax+4], edi
4536D0  mov     [eax+8], esi
4536D3  xor     eax, eax
4536D5  retn
4536D6  mov     eax, 80000004h
4536DB  retn
4536DC  mov     eax, 80000003h
4536E1  retn

;==== 4536F0 sub_4536F0
4536F0  push    ebp
4536F1  mov     ebp, esp
4536F3  mov     eax, [ebp+arg_0]
4536F6  call    sub_4542A0
4536FB  test    eax, eax
4536FD  jz      short loc_453710
4536FF  mov     ecx, [eax+4]
453702  mov     [edx], ecx
453704  mov     eax, [eax+8]
453707  mov     [edx+4], eax
45370A  xor     eax, eax
45370C  pop     ebp
45370D  retn    4
453710  mov     eax, 80000003h
453715  pop     ebp
453716  retn    4

;==== 453720 sub_453720
453720  push    esi
453721  mov     eax, edx
453723  call    sub_4542A0
453728  mov     esi, eax
45372A  test    esi, esi
45372C  jz      short loc_45374A
45372E  mov     eax, [edx]
453730  mov     ecx, edx
453732  mov     edx, [eax+18h]
453735  push    edi
453736  call    edx
453738  test    eax, eax
45373A  jz      short loc_453743
45373C  mov     [esi+0Ch], edi
45373F  xor     eax, eax
453741  pop     esi
453742  retn
453743  mov     eax, 8000000Bh
453748  pop     esi
453749  retn
45374A  mov     eax, 80000003h
45374F  pop     esi
453750  retn

;==== 453760 sub_453760
453760  push    ebp
453761  mov     ebp, esp
453763  call    sub_4542A0
453768  test    eax, eax
45376A  jz      short loc_45377A
45376C  mov     eax, [eax+0Ch]
45376F  mov     ecx, [ebp+arg_0]
453772  mov     [ecx], eax
453774  xor     eax, eax
453776  pop     ebp
453777  retn    4
45377A  mov     eax, 80000003h
45377F  pop     ebp
453780  retn    4

;==== 453790 sub_453790
453790  push    ebp
453791  mov     ebp, esp
453793  mov     eax, [ebp+arg_0]
453796  call    sub_4542A0
45379B  test    eax, eax
45379D  jz      short loc_4537C5
45379F  test    esi, esi
4537A1  js      short loc_4537BC
4537A3  test    edx, edx
4537A5  js      short loc_4537B3
4537A7  mov     [eax+10h], esi
4537AA  mov     [eax+14h], edx
4537AD  xor     eax, eax
4537AF  pop     ebp
4537B0  retn    4
4537B3  mov     eax, 8000000Ah
4537B8  pop     ebp
4537B9  retn    4
4537BC  mov     eax, 80000009h
4537C1  pop     ebp
4537C2  retn    4
4537C5  mov     eax, 80000003h
4537CA  pop     ebp
4537CB  retn    4

;==== 4537D0 sub_4537D0
4537D0  push    ebp
4537D1  mov     ebp, esp
4537D3  mov     eax, [ebp+arg_0]
4537D6  call    sub_4542A0
4537DB  test    eax, eax
4537DD  jz      short loc_4537F0
4537DF  mov     ecx, [edx]
4537E1  mov     [eax+18h], ecx
4537E4  mov     edx, [edx+4]
4537E7  mov     [eax+1Ch], edx
4537EA  xor     eax, eax
4537EC  pop     ebp
4537ED  retn    4
4537F0  mov     eax, 80000003h
4537F5  pop     ebp
4537F6  retn    4

;==== 453800 sub_453800
453800  push    esi
453801  mov     eax, edi
453803  call    sub_4542A0
453808  mov     esi, eax
45380A  test    esi, esi
45380C  jz      short loc_453827
45380E  mov     ecx, edi
453810  call    sub_454370
453815  test    eax, eax
453817  jz      short loc_453820
453819  mov     [esi+20h], edx
45381C  xor     eax, eax
45381E  pop     esi
45381F  retn
453820  mov     eax, 80000005h
453825  pop     esi
453826  retn
453827  mov     eax, 80000003h
45382C  pop     esi
45382D  retn

;==== 453830 sub_453830
453830  push    ebp
453831  mov     ebp, esp
453833  push    ebx
453834  push    esi
453835  push    edi
453836  mov     esi, eax
453838  or      ebx, 0FFFFFFFFh
45383B  call    sub_4542A0
453840  mov     edi, eax
453842  test    edi, edi
453844  jz      short loc_45389E
453846  mov     ecx, [edi+28h]
453849  test    ecx, ecx
45384B  jz      short loc_453855
45384D  mov     eax, [ecx]
45384F  mov     edx, [eax]
453851  push    1
453853  call    edx
453855  mov     eax, [esi]
453857  mov     edx, [eax+24h]
45385A  mov     ecx, esi
45385C  call    edx
45385E  mov     ecx, [ebp+arg_10]
453861  mov     edx, [edi+20h]
453864  push    ecx
453865  push    edx
453866  push    eax
453867  mov     [edi+28h], eax
45386A  call    sub_454390
45386F  mov     eax, [ebp+arg_C]
453872  mov     ecx, [ebp+arg_8]
453875  mov     edx, [ebp+arg_4]
453878  push    eax
453879  mov     eax, [ebp+arg_0]
45387C  push    ecx
45387D  mov     ecx, [edi+8]
453880  push    edx
453881  mov     edx, [edi+4]
453884  push    eax
453885  push    ecx
453886  mov     ecx, [edi+28h]
453889  push    edx
45388A  lea     eax, [edi+18h]
45388D  push    ecx
45388E  call    sub_457B40
453893  test    eax, eax
453895  jnz     short loc_4538AA
453897  pop     edi
453898  pop     esi
453899  pop     ebx
45389A  pop     ebp
45389B  retn    14h
45389E  pop     edi
45389F  pop     esi
4538A0  mov     eax, 80000003h
4538A5  pop     ebx
4538A6  pop     ebp
4538A7  retn    14h
4538AA  pop     edi
4538AB  pop     esi
4538AC  mov     eax, ebx
4538AE  pop     ebx
4538AF  pop     ebp
4538B0  retn    14h

;==== 4538C0 sub_4538C0
4538C0  push    ebp
4538C1  mov     ebp, esp
4538C3  push    esi
4538C4  or      esi, 0FFFFFFFFh
4538C7  call    sub_4542A0
4538CC  test    eax, eax
4538CE  jz      short loc_45391A
4538D0  cmp     dword ptr [eax+28h], 0
4538D4  jz      short loc_453910
4538D6  mov     ecx, [ebp+arg_C]
4538D9  mov     eax, [eax+28h]
4538DC  mov     edx, [eax]
4538DE  mov     edx, [edx+4]
4538E1  push    1
4538E3  push    ecx
4538E4  mov     ecx, [ebp+arg_8]
4538E7  push    ecx
4538E8  mov     ecx, [ebp+arg_4]
4538EB  push    ecx
4538EC  mov     ecx, [ebp+arg_0]
4538EF  push    ecx
4538F0  mov     ecx, eax
4538F2  call    edx
4538F4  test    eax, eax
4538F6  jz      short loc_453909
4538F8  cmp     eax, 80000003h
4538FD  jz      short loc_453910
4538FF  cmp     eax, 0FFFFFFFEh
453902  jnz     short loc_453924
453904  pop     esi
453905  pop     ebp
453906  retn    10h
453909  xor     eax, eax
45390B  pop     esi
45390C  pop     ebp
45390D  retn    10h
453910  mov     eax, 80000006h
453915  pop     esi
453916  pop     ebp
453917  retn    10h
45391A  mov     eax, 80000003h
45391F  pop     esi
453920  pop     ebp
453921  retn    10h
453924  mov     eax, esi
453926  pop     esi
453927  pop     ebp
453928  retn    10h

;==== 453930 sub_453930
453930  push    ebp
453931  mov     ebp, esp
453933  push    edi
453934  or      edi, 0FFFFFFFFh
453937  call    sub_4542A0
45393C  test    eax, eax
45393E  jz      short loc_45399D
453940  push    esi
453941  mov     esi, [eax+28h]
453944  test    esi, esi
453946  jz      short loc_453974
453948  mov     eax, [ebp+arg_4]
45394B  mov     ecx, [ebp+arg_0]
45394E  mov     edx, [ebp+arg_8]
453951  push    eax
453952  mov     eax, [ebp+arg_C]
453955  push    ecx
453956  call    sub_457C10
45395B  cmp     eax, 80000005h
453960  ja      short loc_453992
453962  cmp     eax, 80000004h
453967  jnb     short loc_453987
453969  test    eax, eax
45396B  jz      short loc_45397F
45396D  cmp     eax, 80000003h
453972  jnz     short loc_4539A7
453974  pop     esi
453975  mov     eax, 80000006h
45397A  pop     edi
45397B  pop     ebp
45397C  retn    10h
45397F  pop     esi
453980  xor     eax, eax
453982  pop     edi
453983  pop     ebp
453984  retn    10h
453987  pop     esi
453988  mov     eax, 80000007h
45398D  pop     edi
45398E  pop     ebp
45398F  retn    10h
453992  cmp     eax, 0FFFFFFFEh
453995  jnz     short loc_4539A7
453997  pop     esi
453998  pop     edi
453999  pop     ebp
45399A  retn    10h
45399D  mov     eax, 80000003h
4539A2  pop     edi
4539A3  pop     ebp
4539A4  retn    10h
4539A7  pop     esi
4539A8  mov     eax, edi
4539AA  pop     edi
4539AB  pop     ebp
4539AC  retn    10h

;==== 4539B0 sub_4539B0
4539B0  push    ebp
4539B1  mov     ebp, esp
4539B3  mov     ecx, [ebp+arg_C]
4539B6  push    ebx
4539B7  mov     ebx, [ebp+arg_0]
4539BA  push    esi
4539BB  mov     esi, [ebp+arg_8]
4539BE  push    edi
4539BF  mov     edi, eax
4539C1  mov     eax, [ebp+arg_10]
4539C4  push    eax
4539C5  push    ecx
4539C6  push    esi
4539C7  push    0
4539C9  mov     ecx, edi
4539CB  mov     eax, ebx
4539CD  call    sub_453930
4539D2  mov     [ebp+arg_0], eax
4539D5  test    eax, eax
4539D7  jnz     short loc_453A42
4539D9  cmp     [ebp+arg_4], eax
4539DC  jz      short loc_453A42
4539DE  mov     edx, [esi]
4539E0  cmp     edx, 1
4539E3  jl      short loc_453A42
4539E5  mov     ecx, edi
4539E7  mov     eax, ebx
4539E9  call    sub_4542A0
4539EE  mov     edi, eax
4539F0  xor     ecx, ecx
4539F2  mov     eax, edx
4539F4  mov     edx, 4
4539F9  mul     edx
4539FB  seto    cl
4539FE  neg     ecx
453A00  or      ecx, eax
453A02  push    ecx; Size
453A03  call    ??2@YAPAXI@Z; operator new(uint)
453A08  mov     edx, [ebp+arg_C]
453A0B  add     esp, 4
453A0E  mov     ebx, eax
453A10  mov     eax, [ebp+arg_10]
453A13  push    esi
453A14  mov     esi, [edi+28h]
453A17  push    ebx
453A18  call    sub_457C10
453A1D  mov     eax, [ebp+arg_4]
453A20  mov     ecx, [edi+28h]
453A23  mov     edx, [edi+8]
453A26  push    ebx
453A27  push    eax
453A28  mov     eax, [ebp+arg_8]
453A2B  push    ecx
453A2C  mov     ecx, [edi+4]
453A2F  mov     edi, [eax]
453A31  call    sub_457D10
453A36  push    ebx; void *
453A37  call    ??3@YAXPAX@Z; operator delete(void *)
453A3C  mov     eax, [ebp+arg_0]
453A3F  add     esp, 4
453A42  pop     edi
453A43  pop     esi
453A44  pop     ebx
453A45  pop     ebp
453A46  retn    14h

;==== 453A50 sub_453A50
453A50  push    ebp
453A51  mov     ebp, esp
453A53  push    ebx
453A54  or      ebx, 0FFFFFFFFh
453A57  call    sub_4542A0
453A5C  test    eax, eax
453A5E  jz      short loc_453AA2
453A60  mov     eax, [eax+28h]
453A63  test    eax, eax
453A65  jz      short loc_453A98
453A67  mov     ecx, [ebp+arg_0]
453A6A  push    ecx
453A6B  mov     ecx, eax
453A6D  call    sub_457D70
453A72  cmp     eax, 80000003h
453A77  ja      short loc_453A84
453A79  jz      short loc_453A98
453A7B  test    eax, eax
453A7D  jnz     short loc_453AAC
453A7F  pop     ebx
453A80  pop     ebp
453A81  retn    4
453A84  add     eax, 7FFFFFFCh
453A89  cmp     eax, 1
453A8C  ja      short loc_453AAC
453A8E  mov     eax, 80000007h
453A93  pop     ebx
453A94  pop     ebp
453A95  retn    4
453A98  mov     eax, 80000006h
453A9D  pop     ebx
453A9E  pop     ebp
453A9F  retn    4
453AA2  mov     eax, 80000003h
453AA7  pop     ebx
453AA8  pop     ebp
453AA9  retn    4
453AAC  mov     eax, ebx
453AAE  pop     ebx
453AAF  pop     ebp
453AB0  retn    4

;==== 453AC0 sub_453AC0
453AC0  push    ebp
453AC1  mov     ebp, esp
453AC3  push    ebx
453AC4  or      ebx, 0FFFFFFFFh
453AC7  call    sub_4542A0
453ACC  test    eax, eax
453ACE  jz      short loc_453B17
453AD0  mov     eax, [eax+28h]
453AD3  test    eax, eax
453AD5  jz      short loc_453B0D
453AD7  mov     ecx, [ebp+arg_0]
453ADA  mov     edx, [ebp+arg_4]
453ADD  push    esi
453ADE  push    ecx
453ADF  mov     esi, eax
453AE1  call    sub_457DE0
453AE6  pop     esi
453AE7  cmp     eax, 80000003h
453AEC  ja      short loc_453AF9
453AEE  jz      short loc_453B0D
453AF0  test    eax, eax
453AF2  jnz     short loc_453B21
453AF4  pop     ebx
453AF5  pop     ebp
453AF6  retn    8
453AF9  add     eax, 7FFFFFFCh
453AFE  cmp     eax, 1
453B01  ja      short loc_453B21
453B03  mov     eax, 80000007h
453B08  pop     ebx
453B09  pop     ebp
453B0A  retn    8
453B0D  mov     eax, 80000006h
453B12  pop     ebx
453B13  pop     ebp
453B14  retn    8
453B17  mov     eax, 80000003h
453B1C  pop     ebx
453B1D  pop     ebp
453B1E  retn    8
453B21  mov     eax, ebx
453B23  pop     ebx
453B24  pop     ebp
453B25  retn    8

;==== 453B30 sub_453B30
453B30  push    ebp
453B31  mov     ebp, esp
453B33  push    esi
453B34  or      esi, 0FFFFFFFFh
453B37  call    sub_4542A0
453B3C  test    eax, eax
453B3E  jz      short loc_453B6E
453B40  mov     eax, [eax+28h]
453B43  test    eax, eax
453B45  jz      short loc_453B5D
453B47  mov     ecx, [ebp+arg_0]
453B4A  push    ecx; void *
453B4B  mov     ecx, eax
453B4D  call    sub_457E40
453B52  test    eax, eax
453B54  jz      short loc_453B67
453B56  cmp     eax, 80000003h
453B5B  jnz     short loc_453B78
453B5D  mov     eax, 80000006h
453B62  pop     esi
453B63  pop     ebp
453B64  retn    4
453B67  xor     eax, eax
453B69  pop     esi
453B6A  pop     ebp
453B6B  retn    4
453B6E  mov     eax, 80000003h
453B73  pop     esi
453B74  pop     ebp
453B75  retn    4
453B78  mov     eax, esi
453B7A  pop     esi
453B7B  pop     ebp
453B7C  retn    4

;==== 453B80 sub_453B80
453B80  push    ebp
453B81  mov     ebp, esp
453B83  push    esi
453B84  or      esi, 0FFFFFFFFh
453B87  call    sub_4542A0
453B8C  test    eax, eax
453B8E  jz      short loc_453BC4
453B90  mov     eax, [eax+28h]
453B93  test    eax, eax
453B95  jz      short loc_453BB3
453B97  mov     ecx, [ebp+arg_8]
453B9A  mov     edx, [ebp+arg_4]
453B9D  push    ecx
453B9E  mov     ecx, [ebp+arg_0]
453BA1  push    edx
453BA2  push    ecx
453BA3  call    sub_457E80
453BA8  test    eax, eax
453BAA  jz      short loc_453BBD
453BAC  cmp     eax, 80000003h
453BB1  jnz     short loc_453BCE
453BB3  mov     eax, 80000006h
453BB8  pop     esi
453BB9  pop     ebp
453BBA  retn    0Ch
453BBD  xor     eax, eax
453BBF  pop     esi
453BC0  pop     ebp
453BC1  retn    0Ch
453BC4  mov     eax, 80000003h
453BC9  pop     esi
453BCA  pop     ebp
453BCB  retn    0Ch
453BCE  mov     eax, esi
453BD0  pop     esi
453BD1  pop     ebp
453BD2  retn    0Ch

;==== 453BE0 sub_453BE0
453BE0  push    ebp
453BE1  mov     ebp, esp
453BE3  mov     eax, edx
453BE5  call    sub_4542A0
453BEA  test    eax, eax
453BEC  jz      short loc_453C15
453BEE  mov     ecx, [eax+0Ch]
453BF1  push    esi
453BF2  mov     esi, [edx]
453BF4  push    ecx
453BF5  mov     ecx, [eax+8]
453BF8  mov     eax, [eax+4]
453BFB  push    ecx
453BFC  mov     ecx, [ebp+arg_8]
453BFF  push    eax
453C00  mov     eax, [ebp+arg_4]
453C03  push    ecx
453C04  mov     ecx, [ebp+arg_0]
453C07  push    eax
453C08  push    ecx
453C09  mov     ecx, edx
453C0B  mov     edx, [esi+38h]
453C0E  call    edx
453C10  pop     esi
453C11  pop     ebp
453C12  retn    0Ch
453C15  mov     eax, 80000003h
453C1A  pop     ebp
453C1B  retn    0Ch

;==== 453C20 sub_453C20
453C20  push    ebp
453C21  mov     ebp, esp
453C23  sub     esp, 44h
453C26  push    ebx
453C27  mov     ebx, [ebp+arg_0]
453C2A  push    esi
453C2B  push    edi
453C2C  mov     edi, [ebp+arg_4]
453C2F  mov     ecx, edi
453C31  mov     eax, ebx
453C33  call    sub_4542A0
453C38  mov     esi, eax
453C3A  test    esi, esi
453C3C  jz      loc_454184
453C42  mov     eax, ebx
453C44  call    sub_454320
453C49  cmp     [ebp+arg_1C], 0
453C4D  mov     eax, [ebp+arg_C]
453C50  jz      short loc_453C54
453C52  add     eax, eax
453C54  mov     ecx, [ebp+arg_18]
453C57  mov     edx, [ebp+arg_8]
453C5A  push    ecx
453C5B  push    0FFFFFFFFh
453C5D  push    0FFFFFFFFh
453C5F  push    eax
453C60  push    edx
453C61  mov     ecx, edi
453C63  mov     eax, ebx
453C65  call    sub_453830
453C6A  mov     edi, [ebx+0Ch]
453C6D  imul    edi, [ebx+8]
453C71  xor     ecx, ecx
453C73  mov     eax, edi
453C75  mov     edx, 8
453C7A  mul     edx
453C7C  seto    cl
453C7F  mov     [ebp+arg_0], edi
453C82  neg     ecx
453C84  or      ecx, eax
453C86  push    ecx; Size
453C87  call    ??2@YAPAXI@Z; operator new(uint)
453C8C  mov     [ebp+arg_8], eax
453C8F  xor     ecx, ecx
453C91  mov     eax, edi
453C93  mov     edx, 8
453C98  mul     edx
453C9A  seto    cl
453C9D  neg     ecx
453C9F  or      ecx, eax
453CA1  push    ecx; Size
453CA2  call    ??2@YAPAXI@Z; operator new(uint)
453CA7  mov     [ebp+var_3C], eax
453CAA  xor     ecx, ecx
453CAC  mov     eax, edi
453CAE  mov     edx, 4
453CB3  mul     edx
453CB5  seto    cl
453CB8  neg     ecx
453CBA  or      ecx, eax
453CBC  push    ecx; Size
453CBD  call    ??2@YAPAXI@Z; operator new(uint)
453CC2  mov     [ebp+var_4], eax
453CC5  xor     ecx, ecx
453CC7  mov     eax, edi
453CC9  mov     edx, 4
453CCE  mul     edx
453CD0  seto    cl
453CD3  neg     ecx
453CD5  or      ecx, eax
453CD7  push    ecx; Size
453CD8  call    ??2@YAPAXI@Z; operator new(uint)
453CDD  mov     [ebp+var_C], eax
453CE0  xor     ecx, ecx
453CE2  mov     eax, edi
453CE4  mov     edx, 1Ch
453CE9  mul     edx
453CEB  seto    cl
453CEE  neg     ecx
453CF0  or      ecx, eax
453CF2  push    ecx; Size
453CF3  call    ??2@YAPAXI@Z; operator new(uint)
453CF8  mov     [ebp+Src], eax
453CFB  xor     ecx, ecx
453CFD  mov     eax, edi
453CFF  mov     edx, 1Ch
453D04  mul     edx
453D06  seto    cl
453D09  neg     ecx
453D0B  or      ecx, eax
453D0D  push    ecx; Size
453D0E  call    ??2@YAPAXI@Z; operator new(uint)
453D13  mov     edi, [ebp+arg_8]
453D16  add     esp, 18h
453D19  mov     [ebp+var_18], eax
453D1C  push    0
453D1E  lea     eax, [ebp+var_10]
453D21  lea     ecx, [edi+8]
453D24  push    eax
453D25  push    ecx
453D26  mov     ecx, [ebp+arg_4]
453D29  mov     eax, ebx
453D2B  call    sub_453B80
453D30  mov     edx, [esi+4]
453D33  inc     [ebp+var_10]
453D36  mov     [edi], edx
453D38  mov     eax, [esi+8]
453D3B  mov     [edi+4], eax
453D3E  mov     eax, [ebp+arg_10]
453D41  lea     ecx, [esi+2Ch]
453D44  mov     [ebp+var_40], ecx
453D47  mov     [ebp+var_34], ecx
453D4A  test    eax, eax
453D4C  jz      loc_454143
453D52  mov     esi, [ebp+arg_14]
453D55  mov     [ebp+arg_10], esi
453D58  mov     [ebp+var_38], eax
453D5B  jmp     short loc_453D63
453D60  mov     ecx, [ebp+var_40]
453D63  mov     ecx, [ecx]
453D65  test    ecx, ecx
453D67  jz      short loc_453DA8
453D69  lea     esp, [esp+0]
453D70  mov     edi, esi
453D72  sub     edi, ecx
453D74  mov     edx, 14h
453D79  mov     eax, ecx
453D7B  mov     [ebp+arg_14], edi
453D7E  mov     edi, edi
453D80  cmp     edx, 4
453D83  jb      short loc_453DA0
453D85  mov     edi, [eax+edi]
453D88  cmp     edi, [eax]
453D8A  jnz     short loc_453D97
453D8C  mov     edi, [ebp+arg_14]
453D8F  sub     edx, 4
453D92  add     eax, 4
453D95  jmp     short loc_453D80
453D97  mov     ecx, [ecx+1Ch]
453D9A  test    ecx, ecx
453D9C  jnz     short loc_453D70
453D9E  jmp     short loc_453DA8
453DA0  test    ecx, ecx
453DA2  jnz     loc_454131
453DA8  mov     eax, [ebp+arg_0]
453DAB  mov     ecx, [ebp+Src]
453DAE  lea     edi, ds:0[eax*8]
453DB5  sub     edi, eax
453DB7  add     edi, edi
453DB9  add     edi, edi
453DBB  push    edi; Size
453DBC  push    0; Val
453DBE  push    ecx; void *
453DBF  mov     [ebp+Size], edi
453DC2  call    _memset
453DC7  mov     edx, [ebp+var_18]
453DCA  push    edi; Size
453DCB  push    0; Val
453DCD  push    edx; void *
453DCE  call    _memset
453DD3  mov     edi, [ebp+arg_0]
453DD6  mov     eax, [ebp+var_4]
453DD9  add     edi, edi
453DDB  add     edi, edi
453DDD  push    edi; Size
453DDE  push    0; Val
453DE0  push    eax; void *
453DE1  call    _memset
453DE6  mov     ecx, [ebp+var_C]
453DE9  push    edi; Size
453DEA  push    0; Val
453DEC  push    ecx; void *
453DED  call    _memset
453DF2  xor     eax, eax
453DF4  add     esp, 30h
453DF7  mov     [ebp+var_8], eax
453DFA  cmp     [ebp+var_10], eax
453DFD  jbe     loc_45406B
453E03  jmp     short loc_453E08
453E05  mov     eax, [ebp+var_8]
453E08  mov     ecx, [ebp+arg_8]
453E0B  mov     edi, [ecx+eax*8+4]
453E0F  mov     eax, [ecx+eax*8]
453E12  mov     ecx, [ebp+arg_4]
453E15  push    eax
453E16  lea     edx, [ebp+var_44]
453E19  mov     [ebp+arg_14], eax
453E1C  push    edx
453E1D  mov     eax, ebx
453E1F  call    sub_453AC0
453E24  mov     eax, [ebp+var_44]
453E27  mov     ecx, [ebp+arg_C]
453E2A  cmp     eax, ecx
453E2C  jle     loc_453FD8
453E32  cmp     [ebp+arg_1C], 0
453E36  jz      loc_45405B
453E3C  cmp     dword ptr [esi], 0
453E3F  jnz     loc_45405B
453E45  add     ecx, ecx
453E47  cmp     eax, ecx
453E49  jg      loc_45405B
453E4F  mov     eax, [ebp+var_18]
453E52  mov     [ebp+var_20], eax
453E55  mov     ecx, [esi+0Ch]
453E58  xor     eax, eax
453E5A  cmp     [ebp+arg_18], eax
453E5D  mov     edx, [ebx]
453E5F  mov     edx, [edx+4]
453E62  push    1
453E64  setz    al
453E67  push    eax
453E68  mov     eax, [esi+8]
453E6B  push    0
453E6D  push    ecx
453E6E  mov     ecx, [esi+4]
453E71  push    eax
453E72  mov     eax, [ebp+arg_14]
453E75  push    ecx
453E76  push    edi
453E77  mov     edi, [ebp+var_3C]
453E7A  push    eax
453E7B  lea     ecx, [ebp+var_2C]
453E7E  push    ecx
453E7F  push    0
453E81  push    edi
453E82  mov     ecx, ebx
453E84  call    edx
453E86  cmp     [ebp+var_2C], 0
453E8A  mov     [ebp+var_28], 0
453E91  jbe     loc_45405B
453E97  mov     esi, edi
453E99  mov     [ebp+arg_14], edi
453E9C  lea     esp, [esp+0]
453EA0  mov     eax, [esi+4]
453EA3  mov     edi, [ebx+8]
453EA6  mov     edx, [ebp+arg_10]
453EA9  imul    edi, eax
453EAC  mov     ecx, [esi]
453EAE  mov     edx, [edx+10h]
453EB1  add     edi, ecx
453EB3  cmp     edx, 1
453EB6  jl      loc_453FF8
453EBC  mov     eax, [ebp+var_4]
453EBF  cmp     dword ptr [eax+edi*4], 0
453EC3  jnz     short loc_453F12
453EC5  mov     eax, [ebp+arg_0]
453EC8  xor     ecx, ecx
453ECA  mov     edx, 8
453ECF  mul     edx
453ED1  seto    cl
453ED4  neg     ecx
453ED6  or      ecx, eax
453ED8  push    ecx; Size
453ED9  call    ??2@YAPAXI@Z; operator new(uint)
453EDE  mov     ecx, [ebp+var_4]
453EE1  add     esp, 4
453EE4  push    1
453EE6  push    0
453EE8  push    0
453EEA  mov     [ecx+edi*4], eax
453EED  mov     ecx, [ebp+arg_10]
453EF0  mov     ecx, [ecx+10h]
453EF3  mov     edx, [ebx]
453EF5  mov     edx, [edx+4]
453EF8  push    0
453EFA  push    0
453EFC  push    ecx
453EFD  mov     ecx, [esi+4]
453F00  push    ecx
453F01  mov     ecx, [esi]
453F03  push    ecx
453F04  mov     ecx, [ebp+var_C]
453F07  lea     ecx, [ecx+edi*4]
453F0A  push    ecx
453F0B  push    0
453F0D  push    eax
453F0E  mov     ecx, ebx
453F10  call    edx
453F12  mov     ecx, [ebp+var_C]
453F15  cmp     dword ptr [ecx+edi*4], 0
453F19  mov     eax, [ebp+var_4]
453F1C  mov     eax, [eax+edi*4]
453F1F  mov     [ebp+var_24], 0
453F26  jbe     loc_454042
453F2C  mov     ecx, [ebp+arg_14]
453F2F  sub     ecx, eax
453F31  mov     esi, eax
453F33  mov     [ebp+var_1C], ecx
453F36  mov     ecx, 8
453F3B  mov     eax, esi
453F3D  lea     ecx, [ecx+0]
453F40  mov     edx, [ebp+var_1C]
453F43  mov     edx, [eax+edx]
453F46  cmp     edx, [eax]
453F48  jnz     loc_453FE3
453F4E  sub     ecx, 4
453F51  add     eax, 4
453F54  cmp     ecx, 4
453F57  jnb     short loc_453F40
453F59  mov     eax, [ebp+arg_14]
453F5C  mov     ecx, [eax+4]
453F5F  mov     eax, [eax]
453F61  push    ecx
453F62  mov     ecx, [ebp+arg_8]
453F65  push    eax
453F66  mov     eax, [ebp+var_8]
453F69  mov     ecx, [ecx+eax*8+4]
453F6D  push    ecx
453F6E  mov     ecx, [ebp+arg_8]
453F71  mov     eax, [ecx+eax*8]
453F74  mov     edx, [ebx]
453F76  mov     edx, [edx+0Ch]
453F79  push    eax
453F7A  mov     ecx, ebx
453F7C  call    edx
453F7E  mov     ecx, [esi+4]
453F81  imul    ecx, [ebx+8]
453F85  add     ecx, [esi]
453F87  sub     eax, 2
453F8A  lea     edx, ds:0[ecx*8]
453F91  sub     edx, ecx
453F93  mov     ecx, [ebp+var_20]
453F96  add     edx, eax
453F98  inc     dword ptr [ecx+edx*4+4]
453F9C  sub     [ebp+var_1C], 8
453FA0  lea     eax, [ecx+edx*4+4]
453FA4  mov     eax, [esi+4]
453FA7  imul    eax, [ebx+8]
453FAB  add     eax, [esi]
453FAD  add     esi, 8
453FB0  lea     edx, ds:0[eax*8]
453FB7  sub     edx, eax
453FB9  mov     eax, [ebp+var_24]
453FBC  mov     dword ptr [ecx+edx*4], 1
453FC3  mov     ecx, [ebp+var_C]
453FC6  inc     eax
453FC7  mov     [ebp+var_24], eax
453FCA  cmp     eax, [ecx+edi*4]
453FCD  jb      loc_453F36
453FD3  mov     esi, [ebp+arg_14]
453FD6  jmp     short loc_454042
453FD8  mov     edx, [ebp+Src]
453FDB  mov     [ebp+var_20], edx
453FDE  jmp     loc_453E55
453FE3  mov     eax, [esi+4]
453FE6  mov     ecx, [esi]
453FE8  push    eax
453FE9  mov     eax, [ebp+arg_14]
453FEC  push    ecx
453FED  mov     ecx, [eax+4]
453FF0  mov     eax, [eax]
453FF2  push    ecx
453FF3  jmp     loc_453F74
453FF8  test    edx, edx
453FFA  jnz     short loc_454042
453FFC  mov     edx, [ebx]
453FFE  mov     edx, [edx+0Ch]
454001  push    eax
454002  mov     eax, [ebp+var_8]
454005  push    ecx
454006  mov     ecx, [ebp+arg_8]
454009  mov     ecx, [ecx+eax*8+4]
45400D  push    ecx
45400E  mov     ecx, [ebp+arg_8]
454011  mov     eax, [ecx+eax*8]
454014  push    eax
454015  mov     ecx, ebx
454017  call    edx
454019  mov     ecx, [ebp+var_20]
45401C  sub     eax, 2
45401F  lea     edx, ds:0[edi*8]
454026  sub     edx, edi
454028  add     edx, eax
45402A  inc     dword ptr [ecx+edx*4+4]
45402E  lea     eax, [ecx+edx*4+4]
454032  lea     eax, ds:0[edi*8]
454039  sub     eax, edi
45403B  mov     dword ptr [ecx+eax*4], 1
454042  mov     eax, [ebp+var_28]
454045  inc     eax
454046  add     esi, 8
454049  mov     [ebp+var_28], eax
45404C  mov     [ebp+arg_14], esi
45404F  cmp     eax, [ebp+var_2C]
454052  jb      loc_453EA0
454058  mov     esi, [ebp+arg_10]
45405B  mov     eax, [ebp+var_8]
45405E  inc     eax
45405F  mov     [ebp+var_8], eax
454062  cmp     eax, [ebp+var_10]
454065  jb      loc_453E05
45406B  xor     edi, edi
45406D  cmp     [ebp+arg_0], edi
454070  jbe     short loc_454087
454072  mov     ecx, [ebp+var_4]
454075  mov     edx, [ecx+edi*4]
454078  push    edx; void *
454079  call    ??3@YAXPAX@Z; operator delete(void *)
45407E  inc     edi
45407F  add     esp, 4
454082  cmp     edi, [ebp+arg_0]
454085  jb      short loc_454072
454087  push    20h ; ' '; Size
454089  call    ??2@YAPAXI@Z; operator new(uint)
45408E  mov     edi, eax
454090  mov     eax, [esi]
454092  mov     [edi], eax
454094  mov     ecx, [esi+4]
454097  mov     [edi+4], ecx
45409A  mov     edx, [esi+8]
45409D  mov     [edi+8], edx
4540A0  mov     eax, [esi+0Ch]
4540A3  mov     [edi+0Ch], eax
4540A6  mov     ecx, [esi+10h]
4540A9  mov     eax, [ebp+arg_0]
4540AC  mov     [edi+10h], ecx
4540AF  xor     ecx, ecx
4540B1  mov     edx, 1Ch
4540B6  mul     edx
4540B8  seto    cl
4540BB  neg     ecx
4540BD  or      ecx, eax
4540BF  push    ecx; Size
4540C0  call    ??2@YAPAXI@Z; operator new(uint)
4540C5  mov     ecx, [ebp+Size]
4540C8  mov     edx, [ebp+Src]
4540CB  push    ecx; Size
4540CC  push    edx; Src
4540CD  lea     esi, [edi+1Ch]
4540D0  push    eax; void *
4540D1  mov     [edi+14h], eax
4540D4  mov     dword ptr [esi], 0
4540DA  call    _memcpy_0
4540DF  add     esp, 14h
4540E2  cmp     [ebp+arg_1C], 0
4540E6  jz      short loc_45411F
4540E8  mov     eax, [ebp+arg_10]
4540EB  cmp     dword ptr [eax], 0
4540EE  jnz     short loc_45411F
4540F0  mov     eax, [ebp+arg_0]
4540F3  xor     ecx, ecx
4540F5  mov     edx, 1Ch
4540FA  mul     edx
4540FC  seto    cl
4540FF  neg     ecx
454101  or      ecx, eax
454103  push    ecx; Size
454104  call    ??2@YAPAXI@Z; operator new(uint)
454109  mov     ecx, [ebp+Size]
45410C  mov     edx, [ebp+var_18]
45410F  push    ecx; Size
454110  push    edx; Src
454111  push    eax; void *
454112  mov     [edi+18h], eax
454115  call    _memcpy_0
45411A  add     esp, 10h
45411D  jmp     short loc_454126
45411F  mov     dword ptr [edi+18h], 0
454126  mov     eax, [ebp+var_34]
454129  mov     [ebp+var_34], esi
45412C  mov     esi, [ebp+arg_10]
45412F  mov     [eax], edi
454131  add     esi, 14h
454134  dec     [ebp+var_38]
454137  mov     [ebp+arg_10], esi
45413A  jnz     loc_453D60
454140  mov     edi, [ebp+arg_8]
454143  push    edi; void *
454144  call    ??3@YAXPAX@Z; operator delete(void *)
454149  mov     ecx, [ebp+var_3C]
45414C  push    ecx; void *
45414D  call    ??3@YAXPAX@Z; operator delete(void *)
454152  mov     edx, [ebp+var_4]
454155  push    edx; void *
454156  call    ??3@YAXPAX@Z; operator delete(void *)
45415B  mov     eax, [ebp+var_C]
45415E  push    eax; void *
45415F  call    ??3@YAXPAX@Z; operator delete(void *)
454164  mov     ecx, [ebp+Src]
454167  push    ecx; void *
454168  call    ??3@YAXPAX@Z; operator delete(void *)
45416D  mov     edx, [ebp+var_18]
454170  push    edx; void *
454171  call    ??3@YAXPAX@Z; operator delete(void *)
454176  add     esp, 18h
454179  pop     edi
45417A  pop     esi
45417B  xor     eax, eax
45417D  pop     ebx
45417E  mov     esp, ebp
454180  pop     ebp
454181  retn    20h ; ' '
454184  pop     edi
454185  pop     esi
454186  mov     eax, 80000003h
45418B  pop     ebx
45418C  mov     esp, ebp
45418E  pop     ebp
45418F  retn    20h ; ' '

;==== 4541A0 sub_4541A0
4541A0  push    ebp
4541A1  mov     ebp, esp
4541A3  mov     eax, [ebp+arg_0]
4541A6  call    sub_4542A0
4541AB  test    eax, eax
4541AD  jz      loc_454240
4541B3  mov     ecx, [eax+2Ch]
4541B6  push    ebx
4541B7  push    esi
4541B8  test    ecx, ecx
4541BA  jz      short loc_4541EB
4541BC  lea     esp, [esp+0]
4541C0  mov     esi, edi
4541C2  mov     edx, 14h
4541C7  mov     eax, ecx
4541C9  sub     esi, ecx
4541CB  jmp     short loc_4541D0
4541D0  cmp     edx, 4
4541D3  jb      short loc_4541F6
4541D5  mov     ebx, [esi+eax]
4541D8  cmp     ebx, [eax]
4541DA  jnz     short loc_4541E4
4541DC  sub     edx, 4
4541DF  add     eax, 4
4541E2  jmp     short loc_4541D0
4541E4  mov     ecx, [ecx+1Ch]
4541E7  test    ecx, ecx
4541E9  jnz     short loc_4541C0
4541EB  pop     esi
4541EC  mov     eax, 8000000Dh
4541F1  pop     ebx
4541F2  pop     ebp
4541F3  retn    0Ch
4541F6  cmp     [ebp+arg_8], 0
4541FA  jnz     short loc_454201
4541FC  mov     ecx, [ecx+14h]
4541FF  jmp     short loc_454204
454201  mov     ecx, [ecx+18h]
454204  test    ecx, ecx
454206  jz      short loc_454235
454208  mov     edx, [ebp+arg_0]
45420B  mov     eax, [edx+0Ch]
45420E  imul    eax, [edx+8]
454212  lea     edx, ds:0[eax*8]
454219  sub     edx, eax
45421B  mov     eax, [ebp+arg_4]
45421E  add     edx, edx
454220  add     edx, edx
454222  push    edx; Size
454223  push    ecx; Src
454224  push    eax; void *
454225  call    _memcpy_0
45422A  add     esp, 0Ch
45422D  pop     esi
45422E  xor     eax, eax
454230  pop     ebx
454231  pop     ebp
454232  retn    0Ch
454235  pop     esi
454236  mov     eax, 8000000Eh
45423B  pop     ebx
45423C  pop     ebp
45423D  retn    0Ch
454240  mov     eax, 80000003h
454245  pop     ebp
454246  retn    0Ch

;==== 454250 sub_454250
454250  push    ebp
454251  mov     ebp, esp
454253  xor     eax, eax
454255  cmp     [ebp+arg_0], eax
454258  setz    al
45425B  pop     ebp
45425C  retn    4

;==== 454260 sub_454260
454260  mov     eax, [eax+4]
454263  retn

;==== 454270 sub_454270
454270  push    ebp
454271  mov     ebp, esp
454273  mov     eax, [ebp+arg_0]
454276  mov     eax, [eax+50h]
454279  test    eax, eax
45427B  jz      short loc_454291
45427D  lea     ecx, [ecx+0]
454280  cmp     edx, [eax+4]
454283  jnz     short loc_45428A
454285  cmp     ecx, [eax+8]
454288  jz      short loc_454291
45428A  mov     eax, [eax+30h]
45428D  test    eax, eax
45428F  jnz     short loc_454280
454291  pop     ebp
454292  retn    4

;==== 4542A0 sub_4542A0
4542A0  mov     eax, [eax+50h]
4542A3  test    eax, eax
4542A5  jz      short locret_4542B2
4542A7  cmp     ecx, [eax]
4542A9  jz      short locret_4542B2
4542AB  mov     eax, [eax+30h]
4542AE  test    eax, eax
4542B0  jnz     short loc_4542A7
4542B2  retn

;==== 4542C0 sub_4542C0
4542C0  cmp     dword ptr [edi+50h], 0
4542C4  jz      short loc_4542D8
4542C6  mov     eax, [edi+50h]
4542C9  mov     ecx, [eax]
4542CB  mov     edx, edi
4542CD  call    sub_453630
4542D2  cmp     dword ptr [edi+50h], 0
4542D6  jnz     short loc_4542C6
4542D8  push    esi
4542D9  mov     esi, [edi+1Ch]
4542DC  test    esi, esi
4542DE  jz      short loc_4542FD
4542E0  push    ebx
4542E1  mov     ebx, esi
4542E3  mov     ecx, [ebx+4]
4542E6  mov     esi, [esi+8]
4542E9  push    ecx; void *
4542EA  call    ??3@YAXPAX@Z; operator delete(void *)
4542EF  push    ebx; void *
4542F0  call    ??3@YAXPAX@Z; operator delete(void *)
4542F5  add     esp, 8
4542F8  test    esi, esi
4542FA  jnz     short loc_4542E1
4542FC  pop     ebx
4542FD  mov     edx, [edi+10h]
454300  xor     esi, esi
454302  push    edx; void *
454303  mov     [edi+1Ch], esi
454306  call    ??3@YAXPAX@Z; operator delete(void *)
45430B  add     esp, 4
45430E  mov     [edi+10h], esi
454311  pop     esi
454312  retn

;==== 454320 sub_454320
454320  push    ebx
454321  call    sub_4542A0
454326  mov     ebx, eax
454328  test    ebx, ebx
45432A  jz      short loc_454362
45432C  push    esi
45432D  mov     esi, [ebx+2Ch]
454330  test    esi, esi
454332  jz      short loc_45435A
454334  push    edi
454335  mov     edi, esi
454337  mov     eax, [edi+14h]
45433A  mov     esi, [esi+1Ch]
45433D  push    eax; void *
45433E  call    ??3@YAXPAX@Z; operator delete(void *)
454343  mov     ecx, [edi+18h]
454346  push    ecx; void *
454347  call    ??3@YAXPAX@Z; operator delete(void *)
45434C  push    edi; void *
45434D  call    ??3@YAXPAX@Z; operator delete(void *)
454352  add     esp, 0Ch
454355  test    esi, esi
454357  jnz     short loc_454335
454359  pop     edi
45435A  xor     eax, eax
45435C  pop     esi
45435D  mov     [ebx+2Ch], eax
454360  pop     ebx
454361  retn
454362  mov     eax, 80000003h
454367  pop     ebx
454368  retn

;==== 454370 sub_454370
454370  mov     ecx, [ecx+1Ch]
454373  xor     eax, eax
454375  test    ecx, ecx
454377  jz      short locret_45438F
454379  lea     esp, [esp+0]
454380  cmp     edx, [ecx]
454382  jz      short loc_45438C
454384  mov     ecx, [ecx+8]
454387  test    ecx, ecx
454389  jnz     short loc_454380
45438B  retn
45438C  mov     eax, [ecx+4]
45438F  retn

;==== 454390 sub_454390
454390  push    ebp
454391  mov     ebp, esp
454393  push    ecx
454394  cmp     dword ptr [esi+10h], 0
454398  jnz     short loc_4543A5
45439A  mov     eax, 80000002h
45439F  mov     esp, ebp
4543A1  pop     ebp
4543A2  retn    0Ch
4543A5  mov     eax, [esi+8]
4543A8  imul    eax, [esi+0Ch]
4543AC  xor     ecx, ecx
4543AE  mov     edx, 10h
4543B3  mul     edx
4543B5  seto    cl
4543B8  push    ebx
4543B9  push    edi
4543BA  neg     ecx
4543BC  or      ecx, eax
4543BE  push    ecx; Size
4543BF  call    ??2@YAPAXI@Z; operator new(uint)
4543C4  mov     edx, [ebp+arg_4]
4543C7  mov     ebx, eax
4543C9  add     esp, 4
4543CC  mov     ecx, esi
4543CE  mov     [ebp+var_4], ebx
4543D1  call    sub_454370
4543D6  mov     ecx, [esi+10h]
4543D9  mov     edi, eax
4543DB  mov     eax, [esi+8]
4543DE  imul    eax, [esi+0Ch]
4543E2  shl     eax, 4
4543E5  push    eax; Size
4543E6  push    ecx; Src
4543E7  push    ebx; void *
4543E8  call    _memcpy_0
4543ED  add     esp, 0Ch
4543F0  test    edi, edi
4543F2  jz      short loc_45443C
4543F4  cmp     dword ptr [esi+0Ch], 0
4543F8  mov     [ebp+arg_4], 0
4543FF  jle     short loc_45443C
454401  mov     edx, [esi+8]
454404  xor     eax, eax
454406  test    edx, edx
454408  jle     short loc_454420
45440A  lea     ecx, [ebx+4]
45440D  lea     ecx, [ecx+0]
454410  mov     edx, [edi+eax*4]
454413  add     [ecx], edx
454415  mov     edx, [esi+8]
454418  inc     eax
454419  add     ecx, 10h
45441C  cmp     eax, edx
45441E  jl      short loc_454410
454420  mov     eax, [esi+8]
454423  mov     ecx, eax
454425  lea     edi, [edi+eax*4]
454428  mov     eax, [ebp+arg_4]
45442B  shl     ecx, 4
45442E  inc     eax
45442F  add     ebx, ecx
454431  mov     [ebp+arg_4], eax
454434  cmp     eax, [esi+0Ch]
454437  jl      short loc_454404
454439  mov     ebx, [ebp+var_4]
45443C  cmp     [ebp+arg_8], 0
454440  jz      short loc_454473
454442  mov     edi, [esi+50h]
454445  test    edi, edi
454447  jz      short loc_454473
454449  lea     esp, [esp+0]
454450  mov     eax, [edi+8]
454453  imul    eax, [esi+8]
454457  add     eax, [edi+4]
45445A  push    edi
45445B  add     eax, eax
45445D  or      dword ptr [ebx+eax*8+8], 1
454462  mov     edx, [esi]
454464  mov     eax, [edx+1Ch]
454467  push    ebx
454468  mov     ecx, esi
45446A  call    eax
45446C  mov     edi, [edi+30h]
45446F  test    edi, edi
454471  jnz     short loc_454450
454473  mov     ecx, [esi+0Ch]
454476  mov     eax, [esi+8]
454479  push    ebx; Src
45447A  push    ecx; int
45447B  mov     ecx, [ebp+arg_0]
45447E  call    sub_457AC0
454483  push    ebx; void *
454484  call    ??3@YAXPAX@Z; operator delete(void *)
454489  add     esp, 4
45448C  pop     edi
45448D  xor     eax, eax
45448F  pop     ebx
454490  mov     esp, ebp
454492  pop     ebp
454493  retn    0Ch

;==== 4544A0 sub_4544A0
4544A0  push    ebp
4544A1  mov     ebp, esp
4544A3  push    0FFFFFFFFh
4544A5  push    offset SEH_4544A0
4544AA  mov     eax, large fs:0
4544B0  push    eax
4544B1  mov     eax, dword_4FB734
4544B6  xor     eax, ebp
4544B8  push    eax
4544B9  lea     eax, [ebp+var_C]
4544BC  mov     large fs:0, eax
4544C2  mov     ecx, [ebp+arg_0]
4544C5  mov     eax, [ebp+arg_4]
4544C8  call    sub_457A30
4544CD  mov     [ebp+var_4], 0
4544D4  mov     dword ptr [ecx], offset ??_7DCTELgclGrdFld@@6B@; const DCTELgclGrdFld::`vftable'
4544DA  mov     [ebp+var_4], 0FFFFFFFFh
4544E1  mov     eax, ecx
4544E3  mov     ecx, [ebp+var_C]
4544E6  mov     large fs:0, ecx
4544ED  pop     ecx
4544EE  mov     esp, ebp
4544F0  pop     ebp
4544F1  retn    8
4D8C20  mov     ecx, [ebp+arg_0]
4D8C23  jmp     sub_457AA0
4D8C28  mov     edx, [esp-4+arg_4]
4D8C2C  lea     eax, [edx+0Ch]
4D8C2F  mov     ecx, [edx-4]
4D8C32  xor     ecx, eax
4D8C34  call    sub_4ABDD5
4D8C39  mov     eax, offset stru_4F6470
4D8C3E  jmp     ___CxxFrameHandler3

;==== 454500 sub_454500
454500  push    ebp
454501  mov     ebp, esp
454503  push    esi
454504  mov     esi, ecx
454506  call    sub_454530
45450B  test    [ebp+arg_0], 1
45450F  jz      short loc_45451A
454511  push    esi; void *
454512  call    ??3@YAXPAX@Z; operator delete(void *)
454517  add     esp, 4
45451A  mov     eax, esi
45451C  pop     esi
45451D  pop     ebp
45451E  retn    4

;==== 454530 sub_454530
454530  push    ebp
454531  mov     ebp, esp
454533  push    0FFFFFFFFh
454535  push    offset SEH_454530
45453A  mov     eax, large fs:0
454540  push    eax
454541  push    ecx
454542  mov     eax, dword_4FB734
454547  xor     eax, ebp
454549  push    eax
45454A  lea     eax, [ebp+var_C]
45454D  mov     large fs:0, eax
454553  mov     [ebp+var_10], ecx
454556  mov     dword ptr [ecx], offset ??_7DCTELgclGrdFld@@6B@; const DCTELgclGrdFld::`vftable'
45455C  mov     [ebp+var_4], 0FFFFFFFFh
454563  call    sub_457AA0
454568  mov     ecx, [ebp+var_C]
45456B  mov     large fs:0, ecx
454572  pop     ecx
454573  mov     esp, ebp
454575  pop     ebp
454576  retn
4D8BF0  mov     ecx, [ebp+var_10]
4D8BF3  jmp     sub_457AA0
4D8BF8  mov     edx, [esp-4+arg_4]
4D8BFC  lea     eax, [edx+0Ch]
4D8BFF  mov     ecx, [edx-8]
4D8C02  xor     ecx, eax
4D8C04  call    sub_4ABDD5
4D8C09  mov     eax, offset stru_4F6444
4D8C0E  jmp     ___CxxFrameHandler3

;==== 454580 sub_454580
454580  push    ebp
454581  mov     ebp, esp
454583  sub     eax, [ebp+arg_4]
454586  mov     ecx, [ebp+arg_0]
454589  sub     ecx, [ebp+arg_8]
45458C  imul    eax, eax
45458F  sub     edx, [ebp+arg_C]
454592  imul    ecx, ecx
454595  mov     [ebp+arg_4], edx
454598  add     ecx, eax
45459A  fild    [ebp+arg_4]
45459D  mov     [ebp+arg_4], ecx
4545A0  fidiv   [ebp+arg_10]
4545A3  fild    [ebp+arg_4]
4545A6  fld     st(1)
4545A8  fmulp   st(2), st
4545AA  faddp   st(1), st
4545AC  pop     ebp
4545AD  jmp     __CIsqrt

;==== 4545C0 sub_4545C0
4545C0  push    ebp
4545C1  mov     ebp, esp
4545C3  cmp     [ebp+arg_8], 0
4545C7  push    ebx
4545C8  mov     ebx, 80000008h
4545CD  jz      short loc_4545D2
4545CF  add     ecx, 2
4545D2  push    esi
4545D3  lea     edx, [ebp+arg_4]
4545D6  lea     esi, [ebp+arg_0]
4545D9  call    sub_4548A0
4545DE  pop     esi
4545DF  test    eax, eax
4545E1  jz      short loc_4545F3
4545E3  mov     eax, [ebp+arg_0]
4545E6  mov     ecx, [ebp+arg_4]
4545E9  mov     [edi], eax
4545EB  mov     [edi+4], ecx
4545EE  xor     eax, eax
4545F0  pop     ebx
4545F1  pop     ebp
4545F2  retn
4545F3  mov     eax, ebx
4545F5  pop     ebx
4545F6  pop     ebp
4545F7  retn

;==== 454600 sub_454600
454600  push    ebp
454601  mov     ebp, esp
454603  and     esp, 0FFFFFFF8h
454606  sub     esp, 3Ch
454609  mov     eax, dword_4FB734
45460E  xor     eax, esp
454610  mov     [esp+3Ch+var_4], eax
454614  mov     eax, [ebp+arg_0]
454617  mov     edx, [ebp+arg_4]
45461A  push    ebx
45461B  xor     ebx, ebx
45461D  push    esi
45461E  push    edi
45461F  mov     [esp+48h+var_3C], ecx
454623  mov     [esp+48h+var_30], eax
454627  cmp     [ecx+18h], ebx
45462A  jz      loc_454755
454630  or      eax, 0FFFFFFFFh
454633  mov     esi, 1
454638  mov     [esp+48h+var_20], eax
45463C  mov     [esp+48h+var_14], eax
454640  mov     [esp+48h+var_18], esi
454644  mov     [esp+48h+var_C], esi
454648  xor     esi, esi
45464A  lea     eax, [esp+48h+var_20]
45464E  sub     eax, edx
454650  mov     [esp+48h+var_24], ebx
454654  mov     [esp+48h+var_1C], ebx
454658  mov     [esp+48h+var_10], ebx
45465C  mov     [esp+48h+var_8], ebx
454660  mov     [esp+48h+var_38], esi
454664  mov     [esp+48h+var_34], edx
454668  mov     [esp+48h+var_28], eax
45466C  jmp     short loc_454676
454670  mov     eax, [esp+48h+var_28]
454674  xor     ebx, ebx
454676  mov     edi, [esp+48h+var_30]
45467A  mov     dword ptr [edi+esi*4], 0FFFFFFFFh
454681  cmp     esi, 4
454684  jnb     loc_454726
45468A  mov     esi, [eax+edx-4]
45468E  add     esi, [ebp+arg_8]
454691  mov     edi, [eax+edx]
454694  add     edi, [ebp+arg_C]
454697  cmp     esi, ebx
454699  jl      loc_454726
45469F  mov     eax, [ecx+0Ch]
4546A2  cmp     esi, eax
4546A4  jge     loc_454726
4546AA  cmp     edi, ebx
4546AC  jl      short loc_454726
4546AE  cmp     edi, [ecx+10h]
4546B1  jge     short loc_454726
4546B3  mov     ecx, [ecx+18h]
4546B6  imul    eax, edi
4546B9  add     eax, esi
4546BB  lea     ebx, [eax+eax*2]
4546BE  cmp     dword ptr [ecx+ebx*8+4], 0
4546C3  lea     ecx, [ecx+ebx*8]
4546C6  mov     [esp+48h+var_2C], ecx
4546CA  mov     ecx, [esp+48h+var_3C]
4546CE  jz      short loc_454726
4546D0  mov     ebx, [ecx+14h]
4546D3  add     eax, eax
4546D5  test    byte ptr [ebx+eax*8+8], 0Dh
4546DA  jnz     short loc_454726
4546DC  mov     edx, [ecx+0Ch]
4546DF  mov     eax, edx
4546E1  imul    edx, [ebp+arg_C]
4546E5  add     edx, [ebp+arg_8]
4546E8  imul    eax, edi
4546EB  add     eax, esi
4546ED  add     eax, eax
4546EF  mov     eax, [ebx+eax*8]
4546F2  add     edx, edx
4546F4  sub     eax, [ebx+edx*8]
4546F7  cdq
4546F8  xor     eax, edx
4546FA  sub     eax, edx
4546FC  cmp     eax, [ebp+arg_10]
4546FF  jg      short loc_454722
454701  mov     ecx, [esp+48h+var_2C]
454705  mov     edx, [ecx+8]
454708  mov     ecx, [esp+48h+var_30]
45470C  mov     eax, [esp+48h+var_38]
454710  mov     [ecx+eax*4], edx
454713  mov     edx, [esp+48h+var_34]
454717  mov     ecx, [esp+48h+var_3C]
45471B  mov     [edx], esi
45471D  mov     [edx+4], edi
454720  jmp     short loc_454726
454722  mov     edx, [esp+48h+var_34]
454726  mov     esi, [esp+48h+var_38]
45472A  inc     esi
45472B  add     edx, 8
45472E  mov     [esp+48h+var_38], esi
454732  mov     [esp+48h+var_34], edx
454736  cmp     esi, 6
454739  jb      loc_454670
45473F  xor     eax, eax
454741  pop     edi
454742  pop     esi
454743  pop     ebx
454744  mov     ecx, [esp+3Ch+var_4]
454748  xor     ecx, esp
45474A  call    sub_4ABDD5
45474F  mov     esp, ebp
454751  pop     ebp
454752  retn    14h
454755  mov     ecx, [esp+48h+var_4]
454759  pop     edi
45475A  pop     esi
45475B  pop     ebx
45475C  xor     ecx, esp
45475E  mov     eax, 80000003h
454763  call    sub_4ABDD5
454768  mov     esp, ebp
45476A  pop     ebp
45476B  retn    14h

;==== 454770 sub_454770
454770  push    ebp
454771  mov     ebp, esp
454773  sub     esp, 18h
454776  push    ebx
454777  push    esi
454778  mov     esi, [ebp+arg_0]
45477B  push    edi
45477C  mov     edi, [ebp+arg_4]
45477F  mov     ebx, ecx
454781  push    ebx
454782  lea     ecx, [ebp+var_18]
454785  call    sub_4580B0
45478A  mov     eax, [ebp+var_14]
45478D  cmp     eax, 2
454790  jz      short loc_4547C5
454792  mov     ecx, [ebp+arg_1C]
454795  xor     edx, edx
454797  cmp     eax, 3
45479A  mov     eax, [ebp+arg_14]
45479D  setnz   dl
4547A0  push    ecx
4547A1  mov     ecx, [ebp+arg_10]
4547A4  add     edx, [ebp+arg_18]
4547A7  push    edx
4547A8  mov     edx, [ebp+arg_C]
4547AB  push    eax
4547AC  mov     eax, [ebp+arg_8]
4547AF  push    ecx
4547B0  inc     edx
4547B1  push    edx
4547B2  push    eax
4547B3  mov     ecx, edi
4547B5  push    3
4547B7  dec     ecx
4547B8  mov     edi, esi
4547BA  push    ecx
4547BB  mov     esi, ebx
4547BD  call    sub_458120
4547C2  mov     eax, [ebp+var_14]
4547C5  cmp     eax, 3
4547C8  jz      short loc_4547FF
4547CA  mov     edx, [ebp+arg_1C]
4547CD  mov     edi, [ebp+arg_0]
4547D0  xor     ecx, ecx
4547D2  cmp     eax, 2
4547D5  mov     eax, [ebp+arg_10]
4547D8  setnz   cl
4547DB  push    edx
4547DC  mov     edx, [ebp+arg_14]
4547DF  mov     esi, ebx
4547E1  add     ecx, [ebp+arg_18]
4547E4  push    ecx
4547E5  mov     ecx, [ebp+arg_C]
4547E8  push    edx
4547E9  mov     edx, [ebp+arg_8]
4547EC  push    eax
4547ED  mov     eax, [ebp+arg_4]
4547F0  inc     ecx
4547F1  push    ecx
4547F2  push    edx
4547F3  push    2
4547F5  inc     eax
4547F6  push    eax
4547F7  call    sub_458120
4547FC  mov     eax, [ebp+var_14]
4547FF  cmp     eax, 4
454802  jz      short loc_454839
454804  mov     ecx, [ebp+arg_1C]
454807  mov     edi, [ebp+arg_0]
45480A  xor     edx, edx
45480C  cmp     eax, 5
45480F  mov     eax, [ebp+arg_14]
454812  setnz   dl
454815  push    ecx
454816  mov     ecx, [ebp+arg_10]
454819  dec     edi
45481A  mov     esi, ebx
45481C  add     edx, [ebp+arg_18]
45481F  push    edx
454820  mov     edx, [ebp+arg_C]
454823  push    eax
454824  mov     eax, [ebp+arg_8]
454827  push    ecx
454828  mov     ecx, [ebp+arg_4]
45482B  inc     edx
45482C  push    edx
45482D  push    eax
45482E  push    5
454830  push    ecx
454831  call    sub_458120
454836  mov     eax, [ebp+var_14]
454839  cmp     eax, 5
45483C  jz      short loc_454870
45483E  mov     edx, [ebp+arg_1C]
454841  mov     edi, [ebp+arg_0]
454844  xor     ecx, ecx
454846  cmp     eax, 4
454849  mov     eax, [ebp+arg_10]
45484C  setnz   cl
45484F  push    edx
454850  mov     edx, [ebp+arg_14]
454853  inc     edi
454854  mov     esi, ebx
454856  add     ecx, [ebp+arg_18]
454859  push    ecx
45485A  mov     ecx, [ebp+arg_C]
45485D  push    edx
45485E  mov     edx, [ebp+arg_8]
454861  push    eax
454862  mov     eax, [ebp+arg_4]
454865  inc     ecx
454866  push    ecx
454867  push    edx
454868  push    4
45486A  push    eax
45486B  call    sub_458120
454870  pop     edi
454871  pop     esi
454872  pop     ebx
454873  mov     esp, ebp
454875  pop     ebp
454876  retn    20h ; ' '

;==== 454880 sub_454880
454880  push    ebp
454881  mov     ebp, esp
454883  mov     ecx, [ebp+arg_8]
454886  mov     edx, [ebp+arg_4]
454889  push    esi
45488A  mov     esi, [ebp+arg_0]
45488D  call    sub_4548A0
454892  pop     esi
454893  pop     ebp
454894  retn    0Ch

;==== 4548A0 sub_4548A0
4548A0  add     ecx, 0FFFFFFFEh; switch 4 cases
4548A3  mov     eax, 1
4548A8  cmp     ecx, 3
4548AB  ja      short def_4548AD; jumptable 004548AD default case
4548AD  jmp     ds:jpt_4548AD[ecx*4]; switch jump
4548B4  dec     dword ptr [edx]; jumptable 004548AD case 2
4548B6  retn
4548B7  inc     dword ptr [edx]; jumptable 004548AD case 3
4548B9  retn
4548BA  dec     dword ptr [esi]; jumptable 004548AD case 4
4548BC  retn
4548BD  inc     dword ptr [esi]; jumptable 004548AD case 5
4548BF  retn
4548C0  xor     eax, eax; jumptable 004548AD default case
4548C2  retn

;==== 4548E0 sub_4548E0
4548E0  push    ebp
4548E1  mov     ebp, esp
4548E3  mov     edx, [ebp+arg_4]
4548E6  mov     eax, [ebp+arg_0]
4548E9  test    edx, edx
4548EB  jz      short loc_4548F0
4548ED  add     eax, 2
4548F0  lea     ecx, [eax-2]; switch 4 cases
4548F3  cmp     ecx, 3
4548F6  ja      short def_4548F8; jumptable 004548F8 default case
4548F8  jmp     ds:jpt_4548F8[ecx*4]; switch jump
4548FF  mov     eax, 3; jumptable 004548F8 case 2
454904  jmp     short def_4548F8; jumptable 004548F8 default case
454906  mov     eax, 2; jumptable 004548F8 case 3
45490B  jmp     short def_4548F8; jumptable 004548F8 default case
45490D  mov     eax, 5; jumptable 004548F8 case 4
454912  jmp     short def_4548F8; jumptable 004548F8 default case
454914  mov     eax, 4; jumptable 004548F8 case 5
454919  test    edx, edx; jumptable 004548F8 default case
45491B  jz      short loc_454920
45491D  sub     eax, 2
454920  pop     ebp
454921  retn    8

;==== 454940 sub_454940
454940  push    ebp
454941  mov     ebp, esp
454943  push    0FFFFFFFFh
454945  push    offset SEH_454940
45494A  mov     eax, large fs:0
454950  push    eax
454951  sub     esp, 4Ch
454954  push    ebx
454955  push    esi
454956  push    edi
454957  mov     eax, dword_4FB734
45495C  xor     eax, ebp
45495E  push    eax
45495F  lea     eax, [ebp+var_C]
454962  mov     large fs:0, eax
454968  mov     ebx, [ebp+arg_0]
45496B  mov     eax, [ebp+arg_4]
45496E  mov     esi, ebx
454970  call    sub_4531E0
454975  xor     eax, eax
454977  mov     [ebp+var_4], eax
45497A  mov     esi, 2000h
45497F  mov     ecx, 10000h
454984  mov     edx, 0E000h
454989  mov     [ebp+var_50], esi
45498C  mov     [ebp+var_4C], esi
45498F  mov     [ebp+var_40], ecx
454992  mov     [ebp+var_3C], esi
454995  mov     [ebp+var_30], esi
454998  mov     [ebp+var_2C], ecx
45499B  mov     [ebp+var_20], ecx
45499E  mov     [ebp+var_1C], ecx
4549A1  lea     edi, [ebx+54h]
4549A4  mov     ecx, 10h
4549A9  lea     esi, [ebp+var_58]
4549AC  mov     [ebp+var_58], eax
4549AF  mov     [ebp+var_54], eax
4549B2  mov     [ebp+var_48], edx
4549B5  mov     [ebp+var_44], eax
4549B8  mov     [ebp+var_38], eax
4549BB  mov     [ebp+var_34], edx
4549BE  mov     [ebp+var_28], edx
4549C1  mov     [ebp+var_24], edx
4549C4  rep movsd
4549C6  xor     ecx, ecx
4549C8  mov     dword ptr [ebx], offset ??_7DCTELgclGrdFldMngr@@6B@; const DCTELgclGrdFldMngr::`vftable'
4549CE  mov     [ebp+var_10], ecx
4549D1  mov     [ebp+var_14], 8
4549D8  lea     esi, [ebx+94h]
4549DE  jmp     short loc_4549E5
4549E0  mov     ecx, [ebp+var_10]
4549E3  xor     eax, eax
4549E5  mov     ebx, [ebp+var_14]
4549E8  cmp     ecx, 8
4549EB  jl      short loc_4549F0
4549ED  lea     ebx, [ecx-7]
4549F0  mov     [ebp+arg_4], eax
4549F3  imul    ebx, ebx
4549F6  mov     edi, 8
4549FB  jmp     short loc_454A03
454A00  mov     eax, [ebp+arg_4]
454A03  cmp     eax, 8
454A06  jge     short loc_454A0C
454A08  mov     eax, edi
454A0A  jmp     short loc_454A0F
454A0C  add     eax, 0FFFFFFF9h
454A0F  imul    eax, eax
454A12  add     eax, ebx
454A14  mov     [ebp+var_18], eax
454A17  fild    [ebp+var_18]
454A1A  call    __CIsqrt
454A1F  call    __ftol2_sse
454A24  inc     [ebp+arg_4]
454A27  shl     eax, 8
454A2A  cdq
454A2B  and     edx, 7
454A2E  add     eax, edx
454A30  sar     eax, 3
454A33  mov     [esi], eax
454A35  dec     edi
454A36  add     esi, 4
454A39  cmp     edi, 0FFFFFFF8h
454A3C  jg      short loc_454A00
454A3E  mov     eax, [ebp+var_14]
454A41  inc     [ebp+var_10]
454A44  dec     eax
454A45  mov     [ebp+var_14], eax
454A48  cmp     eax, 0FFFFFFF8h
454A4B  jg      short loc_4549E0
454A4D  mov     eax, [ebp+arg_0]
454A50  mov     dword ptr [eax+494h], 0
454A5A  mov     [ebp+var_4], 0FFFFFFFFh
454A61  mov     ecx, [ebp+var_C]
454A64  mov     large fs:0, ecx
454A6B  pop     ecx
454A6C  pop     edi
454A6D  pop     esi
454A6E  pop     ebx
454A6F  mov     esp, ebp
454A71  pop     ebp
454A72  retn    8
4D9F10  mov     ecx, [ebp+arg_0]
4D9F13  jmp     sub_453250
4D9F18  mov     edx, [esp-4+arg_4]
4D9F1C  lea     eax, [edx+0Ch]
4D9F1F  mov     ecx, [edx-5Ch]
4D9F22  xor     ecx, eax
4D9F24  call    sub_4ABDD5
4D9F29  mov     eax, offset stru_4F77B0
4D9F2E  jmp     ___CxxFrameHandler3

;==== 454A80 sub_454A80
454A80  push    ebp
454A81  mov     ebp, esp
454A83  push    esi
454A84  mov     esi, ecx
454A86  call    sub_454AB0
454A8B  test    [ebp+arg_0], 1
454A8F  jz      short loc_454A9A
454A91  push    esi; void *
454A92  call    ??3@YAXPAX@Z; operator delete(void *)
454A97  add     esp, 4
454A9A  mov     eax, esi
454A9C  pop     esi
454A9D  pop     ebp
454A9E  retn    4

;==== 454AB0 sub_454AB0
454AB0  push    ebp
454AB1  mov     ebp, esp
454AB3  push    0FFFFFFFFh
454AB5  push    offset SEH_454AB0
454ABA  mov     eax, large fs:0
454AC0  push    eax
454AC1  push    ecx
454AC2  push    esi
454AC3  mov     eax, dword_4FB734
454AC8  xor     eax, ebp
454ACA  push    eax
454ACB  lea     eax, [ebp+var_C]
454ACE  mov     large fs:0, eax
454AD4  mov     esi, ecx
454AD6  mov     [ebp+var_10], esi
454AD9  mov     dword ptr [esi], offset ??_7DCTELgclGrdFldMngr@@6B@; const DCTELgclGrdFldMngr::`vftable'
454ADF  mov     [ebp+var_4], 0
454AE6  call    sub_455340
454AEB  mov     [ebp+var_4], 0FFFFFFFFh
454AF2  mov     ecx, esi
454AF4  call    sub_453250
454AF9  mov     ecx, [ebp+var_C]
454AFC  mov     large fs:0, ecx
454B03  pop     ecx
454B04  pop     esi
454B05  mov     esp, ebp
454B07  pop     ebp
454B08  retn
4D9EE0  mov     ecx, [ebp+var_10]
4D9EE3  jmp     sub_453250
4D9EE8  mov     edx, [esp-4+arg_4]
4D9EEC  lea     eax, [edx+0Ch]
4D9EEF  mov     ecx, [edx-0Ch]
4D9EF2  xor     ecx, eax
4D9EF4  call    sub_4ABDD5
4D9EF9  mov     eax, offset stru_4F7784
4D9EFE  jmp     ___CxxFrameHandler3

;==== 454B90 sub_454B90
454B90  lea     edx, [ecx+0Ah]
454B93  cmp     edx, 14h
454B96  ja      short loc_454BB3
454B98  lea     edx, [eax+0Ah]
454B9B  cmp     edx, 14h
454B9E  ja      short loc_454BB3
454BA0  imul    eax, 15h
454BA3  add     eax, ecx
454BA5  mov     ecx, dword_565DB4
454BAB  mov     eax, [ecx+eax*4+370h]
454BB2  retn
454BB3  push    eax
454BB4  push    ecx
454BB5  call    sub_401000
454BBA  add     esp, 8
454BBD  retn

;==== 454BC0 sub_454BC0
454BC0  push    ebp
454BC1  mov     ebp, esp
454BC3  sub     esp, 0B4h
454BC9  mov     eax, dword_4FB734
454BCE  xor     eax, ebp
454BD0  mov     [ebp+var_4], eax
454BD3  mov     eax, [ebp+arg_0]
454BD6  mov     edx, [ebp+arg_8]
454BD9  push    ebx
454BDA  mov     ebx, [ebp+arg_10]
454BDD  push    esi
454BDE  mov     esi, [ebp+arg_C]
454BE1  mov     [ebp+var_B4], eax
454BE7  push    edi
454BE8  mov     edi, ecx
454BEA  mov     ecx, [ebp+arg_4]
454BED  lea     eax, [ebp+var_9C]
454BF3  push    eax
454BF4  mov     [ebp+var_AC], edx
454BFA  lea     edx, [ebp+var_A0]
454C00  mov     eax, edi
454C02  mov     [ebp+var_A8], ecx
454C08  mov     [ebp+var_94], esi
454C0E  mov     [ebp+var_8C], ebx
454C14  call    sub_453500
454C19  test    eax, eax
454C1B  jnz     loc_454DC1
454C21  mov     eax, [edi+8]
454C24  imul    eax, ebx
454C27  add     eax, esi
454C29  mov     esi, [ebp+arg_14]
454C2C  mov     edx, esi
454C2E  imul    edx, [ebp+arg_18]
454C32  shl     eax, 4
454C35  add     eax, [edi+10h]
454C38  shl     edx, 8
454C3B  mov     ecx, [eax]
454C3D  imul    ecx, edx
454C40  shl     esi, 10h
454C43  add     ecx, esi
454C45  mov     [ebp+var_98], eax
454C4B  xor     esi, esi
454C4D  lea     ecx, [ecx+0]
454C50  test    ecx, ecx
454C52  js      short loc_454C5C
454C54  lea     ebx, [ecx+4000h]
454C5A  jmp     short loc_454C5E
454C5C  xor     ebx, ebx
454C5E  mov     [ebp+esi*4+var_84], ebx
454C65  inc     esi
454C66  sub     ecx, edx
454C68  cmp     esi, 20h ; ' '
454C6B  jl      short loc_454C50
454C6D  xor     ecx, ecx
454C6F  mov     [ebp+var_90], ecx
454C75  mov     [ebp+var_88], ecx
454C7B  cmp     [ebp+var_9C], ecx
454C81  jle     loc_454DB1
454C87  jmp     short loc_454C90
454C90  xor     ebx, ebx
454C92  cmp     [ebp+var_A0], ebx
454C98  jle     loc_454D9E
454C9E  mov     edi, edi
454CA0  mov     esi, [edi+8]
454CA3  mov     eax, [eax]
454CA5  imul    esi, ecx
454CA8  mov     edx, [edi]
454CAA  mov     edx, [edx+28h]
454CAD  push    eax
454CAE  mov     eax, [ebp+var_8C]
454CB4  push    eax
454CB5  mov     eax, [ebp+var_94]
454CBB  add     esi, ebx
454CBD  shl     esi, 4
454CC0  add     esi, [edi+10h]
454CC3  push    eax
454CC4  mov     eax, [esi]
454CC6  push    eax
454CC7  push    ecx
454CC8  push    ebx
454CC9  mov     ecx, edi
454CCB  call    edx
454CCD  fmul    ds:dbl_4EC930
454CD3  call    __ftol2_sse
454CD8  mov     ecx, [esi]
454CDA  mov     [ebp+var_B0], eax
454CE0  cmp     eax, [ebp+ecx*4+var_84]
454CE7  jge     loc_454D85
454CED  cmp     [ebp+arg_20], 0
454CF1  mov     esi, [ebp+var_88]
454CF7  jz      short loc_454D07
454CF9  push    edi
454CFA  mov     ecx, esi
454CFC  mov     edx, ebx
454CFE  call    sub_454270
454D03  test    eax, eax
454D05  jz      short loc_454D85
454D07  mov     eax, [ebp+arg_24]
454D0A  mov     ecx, [ebp+arg_1C]
454D0D  mov     edx, [edi]
454D0F  mov     edx, [edx+34h]
454D12  push    eax
454D13  mov     eax, [ebp+var_8C]
454D19  push    ecx
454D1A  mov     ecx, [ebp+var_94]
454D20  push    eax
454D21  push    ecx
454D22  push    esi
454D23  push    ebx
454D24  lea     eax, [ebp+var_A4]
454D2A  push    eax
454D2B  mov     ecx, edi
454D2D  mov     [ebp+var_A4], 0
454D37  call    edx
454D39  cmp     [ebp+var_A4], 0
454D40  jnz     short loc_454D58
454D42  cmp     [ebp+arg_28], 0
454D46  jnz     short loc_454D85
454D48  cmp     ebx, [ebp+var_94]
454D4E  jnz     short loc_454D85
454D50  cmp     esi, [ebp+var_8C]
454D56  jnz     short loc_454D85
454D58  mov     ecx, [ebp+var_B4]
454D5E  mov     eax, [ebp+var_90]
454D64  mov     [ecx+eax*8], ebx
454D67  mov     [ecx+eax*8+4], esi
454D6B  mov     ecx, [ebp+var_A8]
454D71  test    ecx, ecx
454D73  jz      short loc_454D7E
454D75  mov     edx, [ebp+var_B0]
454D7B  mov     [ecx+eax*4], edx
454D7E  inc     eax
454D7F  mov     [ebp+var_90], eax
454D85  mov     eax, [ebp+var_98]
454D8B  mov     ecx, [ebp+var_88]
454D91  inc     ebx
454D92  cmp     ebx, [ebp+var_A0]
454D98  jl      loc_454CA0
454D9E  inc     ecx
454D9F  mov     [ebp+var_88], ecx
454DA5  cmp     ecx, [ebp+var_9C]
454DAB  jl      loc_454C90
454DB1  mov     eax, [ebp+var_AC]
454DB7  mov     ecx, [ebp+var_90]
454DBD  mov     [eax], ecx
454DBF  xor     eax, eax
454DC1  mov     ecx, [ebp+var_4]
454DC4  pop     edi
454DC5  pop     esi
454DC6  xor     ecx, ebp
454DC8  pop     ebx
454DC9  call    sub_4ABDD5
454DCE  mov     esp, ebp
454DD0  pop     ebp
454DD1  retn    2Ch ; ','

;==== 454DE0 sub_454DE0
454DE0  push    ebp
454DE1  mov     ebp, esp
454DE3  mov     eax, [ebp+arg_C]
454DE6  mov     ecx, [ebp+arg_0]
454DE9  sub     eax, [ebp+arg_4]
454DEC  sub     ecx, [ebp+arg_8]
454DEF  call    sub_454B90
454DF4  sar     eax, 10h
454DF7  lea     ecx, [eax-2Dh]
454DFA  cmp     ecx, 59h ; 'Y'
454DFD  ja      short loc_454E08
454DFF  mov     eax, 2
454E04  pop     ebp
454E05  retn    10h
454E08  lea     edx, [eax-0E1h]
454E0E  cmp     edx, 59h ; 'Y'
454E11  ja      short loc_454E1C
454E13  mov     eax, 3
454E18  pop     ebp
454E19  retn    10h
454E1C  add     eax, 0FFFFFF79h
454E21  mov     ecx, 59h ; 'Y'
454E26  cmp     ecx, eax
454E28  sbb     eax, eax
454E2A  neg     eax
454E2C  add     eax, 4
454E2F  pop     ebp
454E30  retn    10h

;==== 454E40 sub_454E40
454E40  push    ebp
454E41  mov     ebp, esp
454E43  push    esi
454E44  push    edi
454E45  mov     edi, [ebp+arg_8]
454E48  xor     eax, eax
454E4A  lea     esi, [eax+2]
454E4D  test    edi, edi
454E4F  jz      short loc_454E54
454E51  add     [ebp+arg_4], esi
454E54  mov     ecx, [ebp+arg_4]
454E57  add     ecx, 0FFFFFFFEh; switch 4 cases
454E5A  mov     edx, 3
454E5F  cmp     ecx, edx
454E61  ja      short def_454E63; jumptable 00454E63 default case
454E63  jmp     ds:jpt_454E63[ecx*4]; switch jump
454E6A  mov     ecx, [ebp+arg_0]; jumptable 00454E63 case 2
454E6D  mov     [ecx], edx
454E6F  mov     dword ptr [ecx+4], 4
454E76  mov     dword ptr [ecx+8], 5
454E7D  mov     [ecx+0Ch], esi
454E80  test    edi, edi
454E82  jz      short loc_454E94
454E84  mov     edx, 0FFFFFFFEh
454E89  add     [ecx], edx
454E8B  add     [ecx+4], edx
454E8E  add     [ecx+8], edx
454E91  add     [ecx+0Ch], edx
454E94  pop     edi
454E95  pop     esi
454E96  pop     ebp
454E97  retn    0Ch
454E9A  mov     ecx, [ebp+arg_0]; jumptable 00454E63 case 3
454E9D  mov     [ecx], esi
454E9F  mov     dword ptr [ecx+4], 5
454EA6  mov     dword ptr [ecx+8], 4
454EAD  mov     [ecx+0Ch], edx
454EB0  jmp     short loc_454E80
454EB2  mov     ecx, [ebp+arg_0]; jumptable 00454E63 case 4
454EB5  mov     dword ptr [ecx], 5
454EBB  mov     [ecx+4], edx
454EBE  mov     [ecx+8], esi
454EC1  mov     dword ptr [ecx+0Ch], 4
454EC8  jmp     short loc_454E80
454ECA  mov     ecx, [ebp+arg_0]; jumptable 00454E63 case 5
454ECD  mov     dword ptr [ecx], 4
454ED3  mov     [ecx+4], esi
454ED6  mov     [ecx+8], edx
454ED9  mov     dword ptr [ecx+0Ch], 5
454EE0  jmp     short loc_454E80
454EE2  pop     edi; jumptable 00454E63 default case
454EE3  mov     eax, 80000008h
454EE8  pop     esi
454EE9  pop     ebp
454EEA  retn    0Ch

;==== 454F00 sub_454F00
454F00  push    ebp
454F01  mov     ebp, esp
454F03  mov     eax, ecx
454F05  mov     ecx, [ebp+arg_4]
454F08  mov     edx, 80000003h
454F0D  call    sub_4542A0
454F12  test    eax, eax
454F14  jz      short loc_454F3F
454F16  mov     ecx, [ebp+arg_C]
454F19  mov     edx, [eax+8]
454F1C  mov     eax, [eax+4]
454F1F  push    edi
454F20  mov     edi, [ebp+arg_0]
454F23  push    ecx
454F24  mov     ecx, [ebp+arg_8]
454F27  push    edx
454F28  push    eax
454F29  call    sub_4545C0
454F2E  add     esp, 0Ch
454F31  neg     eax
454F33  sbb     eax, eax
454F35  and     eax, 8000000Bh
454F3A  pop     edi
454F3B  pop     ebp
454F3C  retn    10h
454F3F  mov     eax, edx
454F41  pop     ebp
454F42  retn    10h

;==== 454F50 sub_454F50
454F50  push    ebp
454F51  mov     ebp, esp
454F53  mov     eax, [ebp+arg_0]
454F56  cmp     eax, 2
454F59  jb      short loc_454F69
454F5B  cmp     eax, 5
454F5E  ja      short loc_454F69
454F60  mov     eax, 1
454F65  pop     ebp
454F66  retn    4
454F69  push    eax
454F6A  call    sub_454250
454F6F  pop     ebp
454F70  retn    4

;==== 454F80 sub_454F80
454F80  push    ebp
454F81  mov     ebp, esp
454F83  sub     esp, 14h
454F86  mov     eax, dword_4FB734
454F8B  xor     eax, ebp
454F8D  mov     [ebp+var_4], eax
454F90  push    ebx
454F91  push    esi
454F92  mov     esi, [ebp+arg_0]
454F95  mov     eax, ecx
454F97  mov     ecx, [ebp+arg_4]
454F9A  mov     edx, [ecx+8]
454F9D  push    edi
454F9E  mov     edi, [eax+8]
454FA1  mov     ebx, edi
454FA3  imul    ebx, edx
454FA6  mov     [ebp+var_8], eax
454FA9  mov     eax, [ecx+4]
454FAC  add     ebx, eax
454FAE  shl     ebx, 4
454FB1  mov     [ebp+var_14], esi
454FB4  add     esi, ebx
454FB6  mov     ebx, [esi]
454FB8  mov     [ebp+var_C], ebx
454FBB  movzx   ebx, byte ptr [ecx+1Ch]
454FBF  mov     [ebp+var_10], ebx
454FC2  test    eax, eax
454FC4  js      short loc_454FEA
454FC6  cmp     eax, edi
454FC8  jge     short loc_454FEA
454FCA  test    edx, edx
454FCC  js      short loc_454FEA
454FCE  mov     edi, [ebp+var_8]
454FD1  cmp     edx, [edi+0Ch]
454FD4  jge     short loc_454FED
454FD6  mov     eax, [ebp+var_C]
454FD9  sub     eax, [esi]
454FDB  cdq
454FDC  xor     eax, edx
454FDE  sub     eax, edx
454FE0  cmp     eax, [ecx+24h]
454FE3  jg      short loc_454FED
454FE5  or      [esi+0Ch], ebx
454FE8  jmp     short loc_454FED
454FEA  mov     edi, [ebp+var_8]
454FED  mov     eax, [ecx+8]
454FF0  mov     edx, [ecx+4]
454FF3  dec     eax
454FF4  test    edx, edx
454FF6  js      short loc_45502D
454FF8  mov     esi, [edi+8]
454FFB  cmp     edx, esi
454FFD  jge     short loc_45502D
454FFF  test    eax, eax
455001  js      short loc_45502D
455003  cmp     eax, [edi+0Ch]
455006  jge     short loc_45502D
455008  mov     ebx, [ebp+var_14]
45500B  imul    esi, eax
45500E  mov     eax, [ebp+var_C]
455011  add     esi, edx
455013  shl     esi, 4
455016  sub     eax, [esi+ebx]
455019  add     esi, ebx
45501B  cdq
45501C  xor     eax, edx
45501E  sub     eax, edx
455020  cmp     eax, [ecx+24h]
455023  jg      short loc_455030
455025  mov     eax, [ebp+var_10]
455028  or      [esi+0Ch], eax
45502B  jmp     short loc_455030
45502D  mov     ebx, [ebp+var_14]
455030  mov     eax, [ecx+8]
455033  mov     edx, [ecx+4]
455036  inc     eax
455037  test    edx, edx
455039  js      short loc_45506B
45503B  mov     esi, [edi+8]
45503E  cmp     edx, esi
455040  jge     short loc_45506B
455042  test    eax, eax
455044  js      short loc_45506B
455046  cmp     eax, [edi+0Ch]
455049  jge     short loc_45506B
45504B  imul    esi, eax
45504E  mov     eax, [ebp+var_C]
455051  add     esi, edx
455053  shl     esi, 4
455056  sub     eax, [esi+ebx]
455059  add     esi, ebx
45505B  cdq
45505C  xor     eax, edx
45505E  sub     eax, edx
455060  cmp     eax, [ecx+24h]
455063  jg      short loc_45506B
455065  mov     edx, [ebp+var_10]
455068  or      [esi+0Ch], edx
45506B  mov     esi, [ecx+4]
45506E  mov     edx, [ecx+8]
455071  lea     edi, [esi-1]
455074  test    edi, edi
455076  js      short loc_4550AE
455078  mov     eax, [ebp+var_8]
45507B  mov     eax, [eax+8]
45507E  cmp     edi, eax
455080  jge     short loc_4550AE
455082  test    edx, edx
455084  js      short loc_4550AE
455086  mov     edi, [ebp+var_8]
455089  cmp     edx, [edi+0Ch]
45508C  jge     short loc_4550AE
45508E  imul    eax, edx
455091  add     eax, esi
455093  add     eax, eax
455095  lea     esi, [ebx+eax*8-10h]
455099  mov     eax, [ebp+var_C]
45509C  sub     eax, [esi]
45509E  cdq
45509F  xor     eax, edx
4550A1  sub     eax, edx
4550A3  cmp     eax, [ecx+24h]
4550A6  jg      short loc_4550AE
4550A8  mov     edx, [ebp+var_10]
4550AB  or      [esi+0Ch], edx
4550AE  mov     esi, [ecx+4]
4550B1  mov     edx, [ecx+8]
4550B4  lea     edi, [esi+1]
4550B7  test    edi, edi
4550B9  js      short loc_4550F1
4550BB  mov     eax, [ebp+var_8]
4550BE  mov     eax, [eax+8]
4550C1  cmp     edi, eax
4550C3  jge     short loc_4550F1
4550C5  test    edx, edx
4550C7  js      short loc_4550F1
4550C9  mov     edi, [ebp+var_8]
4550CC  cmp     edx, [edi+0Ch]
4550CF  jge     short loc_4550F1
4550D1  imul    eax, edx
4550D4  lea     esi, [eax+esi+1]
4550D8  mov     eax, [ebp+var_C]
4550DB  add     esi, esi
4550DD  sub     eax, [ebx+esi*8]
4550E0  cdq
4550E1  xor     eax, edx
4550E3  sub     eax, edx
4550E5  cmp     eax, [ecx+24h]
4550E8  jg      short loc_4550F1
4550EA  mov     ecx, [ebp+var_10]
4550ED  or      [ebx+esi*8+0Ch], ecx
4550F1  mov     ecx, [ebp+var_4]
4550F4  pop     edi
4550F5  pop     esi
4550F6  xor     ecx, ebp
4550F8  pop     ebx
4550F9  call    sub_4ABDD5
4550FE  mov     esp, ebp
455100  pop     ebp
455101  retn    8

;==== 455110 sub_455110
455110  push    ebp
455111  mov     ebp, esp
455113  push    0FFFFFFFFh
455115  push    offset SEH_4ABD30
45511A  mov     eax, large fs:0
455120  push    eax
455121  push    ecx
455122  push    esi
455123  mov     eax, dword_4FB734
455128  xor     eax, ebp
45512A  push    eax
45512B  lea     eax, [ebp+var_C]
45512E  mov     large fs:0, eax
455134  mov     esi, ecx
455136  push    498h; Size
45513B  call    ??2@YAPAXI@Z; operator new(uint)
455140  mov     ecx, eax
455142  add     esp, 4
455145  mov     [ebp+var_10], ecx
455148  xor     eax, eax
45514A  mov     [ebp+var_4], eax
45514D  cmp     ecx, eax
45514F  jz      short loc_45515F
455151  mov     eax, esi
455153  call    sub_454260
455158  push    eax
455159  push    ecx
45515A  call    sub_454940
45515F  mov     [ebp+var_4], 0FFFFFFFFh
455166  mov     ecx, [ebp+var_C]
455169  mov     large fs:0, ecx
455170  pop     ecx
455171  pop     esi
455172  mov     esp, ebp
455174  pop     ebp
455175  retn
4DA280  mov     eax, [ebp+var_10]
4DA283  push    eax; void *
4DA284  call    ??3@YAXPAX@Z; operator delete(void *)
4DA289  pop     ecx
4DA28A  retn
4DA28B  mov     edx, [esp-4+arg_4]
4DA28F  lea     eax, [edx+0Ch]
4DA292  mov     ecx, [edx-0Ch]
4DA295  xor     ecx, eax
4DA297  call    sub_4ABDD5
4DA29C  mov     eax, offset stru_4F7B70
4DA2A1  jmp     ___CxxFrameHandler3

;==== 455180 sub_455180
455180  push    ebp
455181  mov     ebp, esp
455183  push    0FFFFFFFFh
455185  push    offset SEH_4ABD30
45518A  mov     eax, large fs:0
455190  push    eax
455191  push    ecx
455192  push    esi
455193  mov     eax, dword_4FB734
455198  xor     eax, ebp
45519A  push    eax
45519B  lea     eax, [ebp+var_C]
45519E  mov     large fs:0, eax
4551A4  mov     esi, ecx
4551A6  push    48h ; 'H'; Size
4551A8  call    ??2@YAPAXI@Z; operator new(uint)
4551AD  mov     ecx, eax
4551AF  add     esp, 4
4551B2  mov     [ebp+var_10], ecx
4551B5  xor     eax, eax
4551B7  mov     [ebp+var_4], eax
4551BA  cmp     ecx, eax
4551BC  jz      short loc_4551CC
4551BE  mov     eax, esi
4551C0  call    sub_454260
4551C5  push    eax
4551C6  push    ecx
4551C7  call    sub_4544A0
4551CC  mov     [ebp+var_4], 0FFFFFFFFh
4551D3  mov     ecx, [ebp+var_C]
4551D6  mov     large fs:0, ecx
4551DD  pop     ecx
4551DE  pop     esi
4551DF  mov     esp, ebp
4551E1  pop     ebp
4551E2  retn
4DA280  mov     eax, [ebp+var_10]
4DA283  push    eax; void *
4DA284  call    ??3@YAXPAX@Z; operator delete(void *)
4DA289  pop     ecx
4DA28A  retn
4DA28B  mov     edx, [esp-4+arg_4]
4DA28F  lea     eax, [edx+0Ch]
4DA292  mov     ecx, [edx-0Ch]
4DA295  xor     ecx, eax
4DA297  call    sub_4ABDD5
4DA29C  mov     eax, offset stru_4F7B70
4DA2A1  jmp     ___CxxFrameHandler3

;==== 4551F0 sub_4551F0
4551F0  push    ebp
4551F1  mov     ebp, esp
4551F3  mov     eax, ecx
4551F5  call    sub_454260
4551FA  mov     ecx, [ebp+arg_10]
4551FD  mov     edx, [ebp+arg_C]
455200  push    eax
455201  mov     eax, [ebp+arg_14]
455204  push    eax
455205  mov     eax, [ebp+arg_4]
455208  push    ecx
455209  push    edx
45520A  mov     edx, [ebp+arg_8]
45520D  push    eax
45520E  mov     eax, [ebp+arg_0]
455211  call    sub_454580
455216  add     esp, 14h
455219  pop     ebp
45521A  retn    18h

;==== 455220 sub_455220
455220  push    ebp
455221  mov     ebp, esp
455223  sub     esp, 10h
455226  push    ebx
455227  lea     eax, [ebp+var_8]
45522A  mov     ebx, ecx
45522C  push    eax
45522D  lea     edx, [ebp+var_10]
455230  mov     eax, ebx
455232  call    sub_453500
455237  test    eax, eax
455239  jnz     loc_455330
45523F  push    esi
455240  push    edi
455241  mov     edi, [ebp+var_10]
455244  mov     esi, edi
455246  imul    esi, [ebp+var_8]
45524A  xor     ecx, ecx
45524C  mov     eax, esi
45524E  mov     edx, 10h
455253  mul     edx
455255  seto    cl
455258  neg     ecx
45525A  or      ecx, eax
45525C  push    ecx; unsigned int
45525D  call    ??_U@YAPAXI@Z; operator new[](uint)
455262  shl     esi, 4
455265  push    esi; Size
455266  push    0; Val
455268  push    eax; void *
455269  mov     [ebp+var_10], eax
45526C  call    _memset
455271  mov     eax, 1
455276  add     esp, 10h
455279  mov     [ebp+var_4], eax
45527C  cmp     [ebp+var_8], eax
45527F  jle     loc_455323
455285  dec     edi
455286  mov     [ebp+var_C], edi
455289  lea     esp, [esp+0]
455290  xor     esi, esi
455292  cmp     [ebp+var_C], esi
455295  jle     short loc_455316
455297  mov     ecx, [ebx+8]
45529A  mov     edx, [ebx+10h]
45529D  mov     edi, ecx
45529F  imul    edi, eax
4552A2  dec     eax
4552A3  imul    eax, ecx
4552A6  add     eax, esi
4552A8  add     edi, esi
4552AA  add     eax, eax
4552AC  mov     ecx, [edx+eax*8+0Ch]
4552B0  shl     edi, 4
4552B3  add     edi, edx
4552B5  shr     ecx, 1Ch
4552B8  add     ecx, [edx+eax*8]
4552BB  mov     edx, [edi+1Ch]
4552BE  shr     edx, 1Ch
4552C1  add     edx, [edi+10h]
4552C4  mov     edi, [edi]
4552C6  cmp     edi, ecx
4552C8  jge     short loc_45530D
4552CA  cmp     edi, edx
4552CC  jge     short loc_45530D
4552CE  cmp     ecx, edx
4552D0  jl      short loc_4552D4
4552D2  mov     ecx, edx
4552D4  mov     edx, [ebp+var_10]
4552D7  mov     [edx+eax*8+0Ch], ecx
4552DB  mov     edi, [ebx+8]
4552DE  mov     eax, [ebp+var_4]
4552E1  dec     eax
4552E2  imul    edi, eax
4552E5  mov     eax, [ebp+var_4]
4552E8  add     edi, esi
4552EA  add     edi, edi
4552EC  mov     [edx+edi*8+18h], ecx
4552F0  mov     edi, [ebx+8]
4552F3  imul    edi, eax
4552F6  add     edi, esi
4552F8  add     edi, edi
4552FA  mov     [edx+edi*8+4], ecx
4552FE  mov     edi, [ebx+8]
455301  imul    edi, eax
455304  lea     eax, [edi+esi+1]
455308  add     eax, eax
45530A  mov     [edx+eax*8], ecx
45530D  mov     eax, [ebp+var_4]
455310  inc     esi
455311  cmp     esi, [ebp+var_C]
455314  jl      short loc_455297
455316  inc     eax
455317  mov     [ebp+var_4], eax
45531A  cmp     eax, [ebp+var_8]
45531D  jl      loc_455290
455323  mov     ecx, [ebp+var_10]
455326  pop     edi
455327  mov     [ebx+494h], ecx
45532D  xor     eax, eax
45532F  pop     esi
455330  pop     ebx
455331  mov     esp, ebp
455333  pop     ebp
455334  retn

;==== 455340 sub_455340
455340  push    esi
455341  mov     esi, ecx
455343  mov     eax, [esi+494h]
455349  push    eax; void *
45534A  call    ??_V@YAXPAX@Z; operator delete[](void *)
45534F  add     esp, 4
455352  mov     dword ptr [esi+494h], 0
45535C  pop     esi
45535D  retn

;==== 455360 sub_455360
455360  push    ebp
455361  mov     ebp, esp
455363  and     esp, 0FFFFFFF8h
455366  push    0FFFFFFFFh
455368  push    offset SEH_455360
45536D  mov     eax, large fs:0
455373  push    eax
455374  sub     esp, 48h
455377  push    ebx
455378  push    esi
455379  push    edi
45537A  mov     eax, dword_4FB734
45537F  xor     eax, esp
455381  push    eax
455382  lea     eax, [esp+64h+var_C]
455386  mov     large fs:0, eax
45538C  mov     esi, ecx
45538E  mov     dword ptr [esp+64h+var_40+4], esi
455392  cmp     dword ptr [esi+10h], 0
455396  jnz     short loc_4553B2
455398  mov     eax, 80000002h
45539D  mov     ecx, [esp+64h+var_C]
4553A1  mov     large fs:0, ecx
4553A8  pop     ecx
4553A9  pop     edi
4553AA  pop     esi
4553AB  pop     ebx
4553AC  mov     esp, ebp
4553AE  pop     ebp
4553AF  retn    1Ch
4553B2  mov     edx, [ebp+arg_4]
4553B5  test    edx, edx
4553B7  js      short loc_4553CC
4553B9  cmp     edx, [esi+8]
4553BC  jge     short loc_4553CC
4553BE  mov     eax, [ebp+arg_8]
4553C1  test    eax, eax
4553C3  js      short loc_4553CC
4553C5  mov     ebx, [esi+0Ch]
4553C8  cmp     eax, ebx
4553CA  jl      short loc_4553E6
4553CC  mov     eax, 80000004h
4553D1  mov     ecx, [esp+64h+var_C]
4553D5  mov     large fs:0, ecx
4553DC  pop     ecx
4553DD  pop     edi
4553DE  pop     esi
4553DF  pop     ebx
4553E0  mov     esp, ebp
4553E2  pop     ebp
4553E3  retn    1Ch
4553E6  mov     ecx, [ebp+arg_C]
4553E9  mov     edi, [ebp+arg_10]
4553EC  cmp     ecx, edx
4553EE  jnz     short loc_4553F4
4553F0  cmp     edi, eax
4553F2  jz      short loc_4553CC
4553F4  test    ecx, ecx
4553F6  js      short loc_455405
4553F8  cmp     ecx, [esi+8]
4553FB  jge     short loc_455405
4553FD  test    edi, edi
4553FF  js      short loc_455405
455401  cmp     edi, ebx
455403  jl      short loc_45541F
455405  mov     eax, 80000008h
45540A  mov     ecx, [esp+64h+var_C]
45540E  mov     large fs:0, ecx
455415  pop     ecx
455416  pop     edi
455417  pop     esi
455418  pop     ebx
455419  mov     esp, ebp
45541B  pop     ebp
45541C  retn    1Ch
45541F  push    2640h; Size
455424  call    ??2@YAPAXI@Z; operator new(uint)
455429  add     esp, 4
45542C  mov     [esp+64h+var_28], eax
455430  xor     ecx, ecx
455432  mov     [esp+64h+var_4], ecx
455436  cmp     eax, ecx
455438  jz      short loc_455446
45543A  push    eax
45543B  call    sub_444450
455440  mov     [esp+64h+var_4C], eax
455444  jmp     short loc_45544A
455446  mov     [esp+64h+var_4C], ecx
45544A  mov     [esp+64h+var_4], 0FFFFFFFFh
455452  mov     eax, [esi+8]
455455  mov     edx, [ebp+arg_C]
455458  mov     ecx, [esi+10h]
45545B  mov     ebx, eax
45545D  imul    eax, edi
455460  imul    ebx, [ebp+arg_8]
455464  add     ebx, [ebp+arg_4]
455467  add     eax, edx
455469  shl     eax, 4
45546C  add     eax, ecx
45546E  mov     dword ptr [esp+64h+var_48], eax
455472  mov     eax, [eax]
455474  push    eax
455475  mov     eax, [ebp+arg_8]
455478  push    edi
455479  push    edx
45547A  shl     ebx, 4
45547D  mov     edx, [ebx+ecx]
455480  add     ebx, ecx
455482  push    edx
455483  mov     edx, [ebp+arg_4]
455486  push    eax
455487  mov     eax, [esi]
455489  mov     eax, [eax+28h]
45548C  push    edx
45548D  mov     ecx, esi
45548F  call    eax
455491  mov     edx, [ebp+arg_C]
455494  fnstcw  word ptr [esp+64h+var_50]
455498  movzx   eax, word ptr [esp+64h+var_50]
45549D  fst     [esp+64h+var_30]
4554A1  fmul    ds:dbl_4EC890
4554A7  or      eax, 0C00h
4554AC  mov     dword ptr [esp+64h+var_40], eax
4554B0  push    esi
4554B1  fldcw   word ptr [esp+68h+var_40]
4554B5  fistp   qword ptr [esp+68h+var_28]
4554B9  mov     ecx, [esp+68h+var_28]
4554BD  mov     dword ptr [esp+68h+var_40], ecx
4554C1  mov     ecx, edi
4554C3  fldcw   word ptr [esp+68h+var_50]
4554C7  call    sub_454270
4554CC  test    eax, eax
4554CE  jz      short loc_4554D5
4554D0  mov     eax, [eax+10h]
4554D3  jmp     short loc_4554DA
4554D5  mov     eax, 3
4554DA  mov     ecx, [ebp+arg_8]
4554DD  mov     edx, [ebp+arg_4]
4554E0  shl     eax, 0Fh
4554E3  push    esi
4554E4  mov     [esp+68h+var_50], eax
4554E8  call    sub_454270
4554ED  test    eax, eax
4554EF  jz      short loc_4554F6
4554F1  mov     eax, [eax+10h]
4554F4  jmp     short loc_4554FB
4554F6  mov     eax, 3
4554FB  mov     esi, [esp+64h+var_4C]
4554FF  shl     eax, 0Fh
455502  mov     dword ptr [esp+64h+var_48+4], eax
455506  call    sub_4445B0
45550B  mov     edx, dword ptr [esp+64h+var_48]
45550F  mov     eax, [edx]
455511  mov     ecx, [ebp+arg_C]
455514  shl     eax, 10h
455517  add     eax, [esp+64h+var_50]
45551B  shl     edi, 10h
45551E  push    eax
45551F  add     edi, 8000h
455525  shl     ecx, 10h
455528  push    edi
455529  add     ecx, 8000h
45552F  push    ecx
455530  mov     edi, esi
455532  call    sub_444650
455537  cmp     [ebp+arg_14], 1
45553B  jl      short loc_455590
45553D  fild    [ebp+arg_14]
455540  fmul    [esp+64h+var_30]
455544  fmul    ds:dbl_4ECA20
45554A  fmul    ds:dbl_4EC930
455550  call    __ftol2_sse
455555  mov     edx, dword ptr [esp+64h+var_48]
455559  mov     ecx, [edx]
45555B  add     ecx, [ebx]
45555D  mov     edx, [ebp+arg_8]
455560  shl     ecx, 0Fh
455563  sub     ecx, eax
455565  mov     eax, dword ptr [esp+64h+var_48+4]
455569  add     eax, [esp+64h+var_50]
45556D  sar     eax, 1
45556F  add     ecx, eax
455571  push    ecx
455572  mov     ecx, [ebp+arg_10]
455575  lea     eax, [edx+ecx+1]
455579  mov     ecx, [ebp+arg_C]
45557C  mov     edx, [ebp+arg_4]
45557F  shl     eax, 0Fh
455582  push    eax
455583  lea     eax, [edx+ecx+1]
455587  shl     eax, 0Fh
45558A  push    eax
45558B  call    sub_444650
455590  mov     ecx, [ebx]
455592  mov     edx, [ebp+arg_8]
455595  mov     eax, [ebp+arg_4]
455598  shl     ecx, 10h
45559B  add     ecx, dword ptr [esp+64h+var_48+4]
45559F  shl     edx, 10h
4555A2  push    ecx
4555A3  add     edx, 8000h
4555A9  shl     eax, 10h
4555AC  push    edx
4555AD  add     eax, 8000h
4555B2  push    eax
4555B3  call    sub_444650
4555B8  mov     ecx, dword ptr [esp+64h+var_40]
4555BC  mov     eax, edi
4555BE  call    sub_4446A0
4555C3  xor     eax, eax
4555C5  mov     ebx, 1
4555CA  xor     edx, edx
4555CC  mov     dword ptr [esp+64h+var_48+4], ebx
4555D0  mov     [esp+64h+var_28], eax
4555D4  mov     dword ptr [esp+64h+var_48], edx
4555D8  cmp     edx, dword ptr [esp+64h+var_40]
4555DC  ja      loc_455754
4555E2  mov     [esp+64h+var_14], eax
4555E6  mov     [esp+64h+var_18], eax
4555EA  mov     [esp+64h+var_1C], eax
4555EE  mov     eax, [esp+64h+var_4C]
4555F2  lea     esi, [esp+64h+var_1C]
4555F6  call    sub_4446C0
4555FB  test    al, al
4555FD  jz      loc_455715
455603  mov     edi, dword ptr [esp+64h+var_40+4]
455607  mov     ecx, [esp+64h+var_18]
45560B  mov     eax, [edi+8]
45560E  mov     edx, [esp+64h+var_1C]
455612  mov     ebx, [esp+64h+var_14]
455616  mov     esi, [edi+10h]
455619  sar     ecx, 10h
45561C  imul    eax, ecx
45561F  sar     edx, 10h
455622  add     eax, edx
455624  sar     ebx, 10h
455627  shl     eax, 4
45562A  test    byte ptr [esi+eax+8], 2
45562F  mov     [esp+64h+var_50], ecx
455633  mov     dword ptr [esp+64h+var_30], ebx
455637  jnz     loc_455732
45563D  mov     ecx, [esi+eax]
455640  mov     esi, [esi+eax+0Ch]
455644  shr     esi, 1Ch
455647  add     esi, ecx
455649  mov     [esp+64h+var_34], ecx
45564D  cmp     esi, ebx
45564F  jg      loc_455732
455655  mov     esi, [edi+494h]
45565B  add     esi, eax
45565D  mov     [esp+64h+var_38], esi
455661  xor     ecx, ecx
455663  lea     eax, [edi+58h]
455666  jmp     short loc_455674
455668  jmp     short loc_455670
455670  mov     esi, [esp+64h+var_38]
455674  cmp     [esi+ecx*4], ebx
455677  jle     short loc_4556A4
455679  mov     esi, [esp+64h+var_1C]
45567D  and     esi, 0FFFFh
455683  cmp     [eax-4], esi
455686  jg      short loc_4556A4
455688  cmp     esi, [eax+4]
45568B  jge     short loc_4556A4
45568D  mov     esi, [esp+64h+var_18]
455691  and     esi, 0FFFFh
455697  cmp     [eax], esi
455699  jg      short loc_4556A4
45569B  cmp     esi, [eax+8]
45569E  jl      loc_45572A
4556A4  inc     ecx
4556A5  add     eax, 10h
4556A8  cmp     ecx, 4
4556AB  jb      short loc_455670
4556AD  mov     ebx, dword ptr [esp+64h+var_48+4]
4556B1  test    ebx, ebx
4556B3  jz      short loc_455715
4556B5  cmp     [ebp+arg_18], 0
4556B9  jnz     short loc_455715
4556BB  cmp     edx, [ebp+arg_C]
4556BE  jnz     short loc_4556C9
4556C0  mov     ecx, [esp+64h+var_50]
4556C4  cmp     ecx, [ebp+arg_10]
4556C7  jz      short loc_455715
4556C9  mov     ecx, [esp+64h+var_50]
4556CD  push    edi
4556CE  call    sub_454270
4556D3  test    eax, eax
4556D5  jz      short loc_455715
4556D7  mov     ecx, [eax+10h]
4556DA  add     ecx, [esp+64h+var_34]
4556DE  cmp     ecx, dword ptr [esp+64h+var_30]
4556E2  jle     short loc_455715
4556E4  mov     esi, [esp+64h+var_18]
4556E8  mov     ecx, [esp+64h+var_1C]
4556EC  mov     eax, [eax+14h]
4556EF  and     esi, 0FFFFh
4556F5  shl     esi, 4
4556F8  and     ecx, 0FFFFh
4556FE  sar     esi, 10h
455701  shl     ecx, 4
455704  shl     esi, 4
455707  sar     ecx, 10h
45570A  add     esi, ecx
45570C  cmp     eax, [edi+esi*4+94h]
455713  jge     short loc_455736
455715  mov     edx, dword ptr [esp+64h+var_48]
455719  inc     edx
45571A  xor     eax, eax
45571C  mov     dword ptr [esp+64h+var_48], edx
455720  cmp     ebx, eax
455722  jnz     loc_4555D8
455728  jmp     short loc_45577F
45572A  xor     ebx, ebx
45572C  mov     dword ptr [esp+64h+var_48+4], ebx
455730  jmp     short loc_455715
455732  xor     ebx, ebx
455734  jmp     short loc_45577F
455736  cmp     edx, [ebp+arg_4]
455739  jnz     short loc_45574D
45573B  mov     ecx, [esp+64h+var_50]
45573F  cmp     ecx, [ebp+arg_8]
455742  jnz     short loc_45574D
455744  mov     ebx, 1
455749  mov     eax, ebx
45574B  jmp     short loc_45575C
45574D  xor     ebx, ebx
45574F  lea     eax, [ebx+1]
455752  jmp     short loc_45575C
455754  mov     edi, dword ptr [esp+64h+var_40+4]
455758  mov     eax, [esp+64h+var_28]
45575C  test    ebx, ebx
45575E  jz      short loc_45577F
455760  test    eax, eax
455762  jnz     short loc_45577F
455764  mov     edx, [edi+8]
455767  imul    edx, [ebp+arg_8]
45576B  add     edx, [ebp+arg_4]
45576E  mov     eax, [edi+10h]
455771  add     edx, edx
455773  mov     ebx, [eax+edx*8+8]
455777  shr     ebx, 2
45577A  not     ebx
45577C  and     ebx, 1
45577F  mov     ecx, [ebp+arg_0]
455782  mov     [ecx], ebx
455784  mov     ecx, [esp+64h+var_4C]
455788  test    ecx, ecx
45578A  jz      short loc_455794
45578C  mov     edx, [ecx]
45578E  mov     eax, [edx]
455790  push    1
455792  call    eax
455794  xor     eax, eax
455796  mov     ecx, [esp+64h+var_C]
45579A  mov     large fs:0, ecx
4557A1  pop     ecx
4557A2  pop     edi
4557A3  pop     esi
4557A4  pop     ebx
4557A5  mov     esp, ebp
4557A7  pop     ebp
4557A8  retn    1Ch
4D9770  mov     eax, [ebp+var_28]
4D9773  push    eax; void *
4D9774  call    ??3@YAXPAX@Z; operator delete(void *)
4D9779  pop     ecx
4D977A  retn
4D977B  mov     edx, [esp-4+arg_4]
4D977F  lea     eax, [edx-54h]
4D9782  mov     ecx, [edx-58h]
4D9785  xor     ecx, eax
4D9787  call    sub_4ABDD5
4D978C  mov     eax, offset stru_4F7090
4D9791  jmp     ___CxxFrameHandler3

;==== 4557B0 sub_4557B0
4557B0  push    ebp
4557B1  mov     ebp, esp
4557B3  mov     eax, [ebp+arg_14]
4557B6  push    edi
4557B7  xor     edi, edi
4557B9  xor     ecx, ecx
4557BB  test    eax, eax
4557BD  jz      short loc_45581E
4557BF  push    esi
4557C0  add     eax, 0FFFFFFFEh; switch 4 cases
4557C3  xor     esi, esi
4557C5  cmp     eax, 3
4557C8  ja      short def_4557CA; jumptable 004557CA default case
4557CA  jmp     ds:jpt_4557CA[eax*4]; switch jump
4557D1  mov     esi, 5A0000h; jumptable 004557CA case 2
4557D6  jmp     short loc_4557EF
4557D8  mov     esi, 10E0000h; jumptable 004557CA case 3
4557DD  jmp     short loc_4557EF
4557DF  mov     esi, 0B40000h; jumptable 004557CA case 4
4557E4  jmp     short loc_4557EF
4557E6  xor     esi, esi; jumptable 004557CA case 5
4557E8  jmp     short loc_4557EF
4557EA  mov     edi, 8000000Bh; jumptable 004557CA default case
4557EF  mov     eax, [ebp+arg_10]
4557F2  mov     ecx, [ebp+arg_4]
4557F5  sub     eax, [ebp+arg_8]
4557F8  sub     ecx, [ebp+arg_C]
4557FB  call    sub_454B90
455800  sub     eax, esi
455802  cdq
455803  xor     eax, edx
455805  sub     eax, edx
455807  pop     esi
455808  cmp     eax, 0B40000h
45580D  jg      short loc_455813
45580F  mov     ecx, eax
455811  jmp     short loc_45581A
455813  mov     ecx, 1680000h
455818  sub     ecx, eax
45581A  test    edi, edi
45581C  jnz     short loc_455823
45581E  mov     eax, [ebp+arg_0]
455821  mov     [eax], ecx
455823  mov     eax, edi
455825  pop     edi
455826  pop     ebp
455827  retn    18h

;==== 455840 sub_455840
455840  push    ebp
455841  mov     ebp, esp
455843  push    ecx
455844  fld     [ebp+arg_0]
455847  sub     esp, 8
45584A  fstp    [esp+0Ch+X]; X
45584D  call    _ceil
455852  fstp    [ebp+arg_0]
455855  add     esp, 8
455858  fld     [ebp+arg_0]
45585B  pop     ecx
45585C  pop     ebp
45585D  retn

;==== 455860 sub_455860
455860  movaps  xmm0, xmmword ptr [eax]
455863  movss   xmm3, ds:dword_4EC974
45586B  mulps   xmm0, xmm0
45586E  movaps  xmm1, xmm0
455871  shufps  xmm1, xmm0, 0FEh
455875  movaps  xmm2, xmm0
455878  shufps  xmm2, xmm0, 0FDh
45587C  addss   xmm2, xmm1
455880  addss   xmm0, xmm2
455884  movaps  xmm1, xmm0
455887  rsqrtss xmm1, xmm1
45588B  movaps  xmm2, xmm0
45588E  mulss   xmm2, xmm1
455892  mulss   xmm2, xmm1
455896  mulss   xmm2, xmm1
45589A  mulss   xmm2, xmm3
45589E  movss   xmm3, ds:dword_4EC970
4558A6  mulss   xmm1, xmm3
4558AA  addss   xmm1, xmm2
4558AE  mulss   xmm0, xmm1
4558B2  retn

;==== 4558C0 sub_4558C0
4558C0  movaps  xmm3, xmmword ptr [eax]
4558C3  movaps  xmm1, xmm3
4558C6  mulps   xmm1, xmm3
4558C9  movaps  xmm2, xmm1
4558CC  shufps  xmm2, xmm1, 0FEh
4558D0  movaps  xmm4, xmm1
4558D3  shufps  xmm4, xmm1, 0FDh
4558D7  addss   xmm2, xmm4
4558DB  xorps   xmm0, xmm0
4558DE  addss   xmm2, xmm1
4558E2  comiss  xmm2, xmm0
4558E5  jbe     short locret_455935
4558E7  movss   xmm4, ds:dword_4EC974
4558EF  movaps  xmm0, xmm2
4558F2  rsqrtss xmm0, xmm0
4558F6  movaps  xmm1, xmm2
4558F9  mulss   xmm1, xmm0
4558FD  mulss   xmm1, xmm0
455901  mulss   xmm1, xmm0
455905  mulss   xmm1, xmm4
455909  movss   xmm4, ds:dword_4EC970
455911  mulss   xmm0, xmm4
455915  addss   xmm0, xmm1
455919  movaps  xmm1, xmm0
45591C  shufps  xmm1, xmm0, 0C0h
455920  mulps   xmm1, xmm3
455923  movaps  xmm3, xmm1
455926  mulss   xmm2, xmm0
45592A  shufps  xmm1, xmm2, 0Ah
45592E  shufps  xmm3, xmm1, 84h
455932  movaps  xmm0, xmm3
455935  retn

;==== 455940 sub_455940
455940  movaps  xmm0, ds:xmmword_4E41D0
455947  mulps   xmm0, xmmword ptr [eax]
45594A  movdqa  xmm2, ds:xmmword_4E4310
455952  cvtps2dq xmm3, xmm0
455956  movdqa  xmm0, ds:xmmword_4E4320
45595E  movdqa  xmm1, xmm3
455962  pand    xmm1, xmm0
455966  movdqa  xmm0, xmm3
45596A  psubd   xmm0, xmm1
45596E  pcmpgtd xmm2, xmm0
455972  movdqa  xmm4, xmm2
455976  pandn   xmm2, xmm3
45597A  movdqa  xmm3, ds:xmmword_4E4330
455982  pcmpgtd xmm0, xmm3
455986  pand    xmm4, xmm1
45598A  paddd   xmm1, ds:xmmword_4E4340
455992  movdqa  xmm3, xmm0
455996  por     xmm2, xmm4
45599A  pandn   xmm0, xmm2
45599E  pand    xmm3, xmm1
4559A2  por     xmm0, xmm3
4559A6  retn

;==== 4559B0 sub_4559B0
4559B0  push    10h; Alignment
4559B2  push    1800h; Size
4559B7  mov     dword ptr [esi], offset ??_7DCTELgclSpcMngr@@6B@; const DCTELgclSpcMngr::`vftable'
4559BD  mov     dword ptr [esi+10h], 40h ; '@'
4559C4  call    __aligned_malloc
4559C9  push    1800h; Size
4559CE  push    0; Val
4559D0  push    eax; void *
4559D1  mov     [esi+14h], eax
4559D4  call    _memset
4559D9  add     esp, 14h
4559DC  mov     dword ptr [esi+15Ch], 0
4559E6  mov     eax, esi
4559E8  retn

;==== 4559F0 sub_4559F0
4559F0  push    ebp
4559F1  mov     ebp, esp
4559F3  push    esi
4559F4  mov     esi, ecx
4559F6  call    sub_455A20
4559FB  test    [ebp+arg_0], 1
4559FF  jz      short loc_455A0A
455A01  push    esi; void *
455A02  call    ??3@YAXPAX@Z; operator delete(void *)
455A07  add     esp, 4
455A0A  mov     eax, esi
455A0C  pop     esi
455A0D  pop     ebp
455A0E  retn    4

;==== 455A20 sub_455A20
455A20  push    edi
455A21  mov     edi, ecx
455A23  mov     dword ptr [edi], offset ??_7DCTELgclSpcMngr@@6B@; const DCTELgclSpcMngr::`vftable'
455A29  call    sub_4573E0
455A2E  mov     eax, [edi+14h]
455A31  push    eax; Block
455A32  call    __aligned_free
455A37  add     esp, 4
455A3A  pop     edi
455A3B  retn

;==== 455A40 sub_455A40
455A40  push    ebp
455A41  mov     ebp, esp
455A43  and     esp, 0FFFFFFF0h
455A46  sub     esp, 38h
455A49  push    ebx
455A4A  mov     ebx, [ebp+arg_0]
455A4D  push    esi
455A4E  mov     eax, 0A0000001h
455A53  cmp     ebx, 1000h
455A59  jnb     loc_455C1F
455A5F  mov     esi, [edi+10h]
455A62  cmp     ebx, esi
455A64  jb      short loc_455A6C
455A66  add     esi, esi
455A68  cmp     ebx, esi
455A6A  jnb     short loc_455A66
455A6C  cmp     esi, [edi+10h]
455A6F  jbe     short loc_455AD3
455A71  lea     eax, [esi+esi*2]
455A74  shl     eax, 5
455A77  push    10h; Alignment
455A79  push    eax; Size
455A7A  call    __aligned_malloc
455A7F  mov     ecx, [edi+10h]
455A82  mov     edx, [edi+14h]
455A85  lea     ecx, [ecx+ecx*2]
455A88  shl     ecx, 5
455A8B  push    ecx; Size
455A8C  push    edx; Src
455A8D  push    eax; void *
455A8E  mov     [esp+54h+var_34], eax
455A92  call    _memcpy_0
455A97  mov     ecx, [edi+10h]
455A9A  mov     eax, esi
455A9C  sub     eax, ecx
455A9E  lea     eax, [eax+eax*2]
455AA1  lea     ecx, [ecx+ecx*2]
455AA4  add     esp, 14h
455AA7  shl     eax, 5
455AAA  push    eax; Size
455AAB  shl     ecx, 5
455AAE  add     ecx, [esp+44h+var_34]
455AB2  push    0; Val
455AB4  push    ecx; void *
455AB5  call    _memset
455ABA  mov     edx, [edi+14h]
455ABD  add     esp, 0Ch
455AC0  push    edx; Block
455AC1  call    __aligned_free
455AC6  mov     eax, [esp+44h+var_34]
455ACA  add     esp, 4
455ACD  mov     [edi+14h], eax
455AD0  mov     [edi+10h], esi
455AD3  fld1
455AD5  fld     [ebp+arg_1C]
455AD8  fcom    st(1)
455ADA  fnstsw  ax
455ADC  fstp    st(1)
455ADE  test    ah, 1
455AE1  jnz     loc_455C18
455AE7  mov     eax, [ebp+arg_28]
455AEA  test    al, al
455AEC  jz      loc_455C09
455AF2  mov     ecx, [ebp+arg_2C]
455AF5  lea     esi, [ebx+ebx*2]
455AF8  shl     esi, 5
455AFB  add     esi, [edi+14h]
455AFE  sub     esp, 10h
455B01  fstp    dword ptr [esi+4]
455B04  mov     [esi+14h], ecx
455B07  fld     [ebp+arg_20]
455B0A  mov     [esi+10h], eax
455B0D  fstp    dword ptr [esi+8]
455B10  mov     dword ptr [esi], 1
455B16  fld     [ebp+arg_24]
455B19  xor     eax, eax
455B1B  fstp    dword ptr [esi+0Ch]
455B1E  or      ecx, 0FFFFFFFFh
455B21  fldz
455B23  mov     [esi+18h], ecx
455B26  mov     [esi+20h], eax
455B29  mov     [esi+24h], eax
455B2C  mov     [esi+1Ch], ecx
455B2F  mov     [esi+28h], eax
455B32  mov     [esi+2Ch], eax
455B35  fstp    [esp+50h+var_34]
455B39  movss   xmm0, [esp+50h+var_34]
455B3F  fld     ds:dbl_4EC9F0
455B45  fstp    qword ptr [esp+50h+var_48]; double
455B49  fld     [ebp+arg_C]
455B4C  fstp    qword ptr [esp+50h+var_50]; double
455B4F  movaps  [esp+50h+var_20], xmm0
455B54  call    sub_4010B0
455B59  fstp    [esp+50h+var_34]
455B5D  fld     [esp+50h+var_34]
455B61  fstp    [esp+50h+var_34]
455B65  movss   xmm0, [esp+50h+var_34]
455B6B  fld     ds:dbl_4EC9F0
455B71  fstp    qword ptr [esp+50h+var_48]; double
455B75  fld     [ebp+arg_8]
455B78  fstp    qword ptr [esp+50h+var_50]; double
455B7B  movaps  [esp+50h+var_30], xmm0
455B80  call    sub_4010B0
455B85  fstp    [esp+50h+var_34]
455B89  fld     [esp+50h+var_34]
455B8D  fstp    [esp+50h+var_34]
455B91  movss   xmm0, [esp+50h+var_34]
455B97  fld     ds:dbl_4EC9F0
455B9D  fstp    qword ptr [esp+50h+var_48]; double
455BA1  fld     [ebp+arg_4]
455BA4  fstp    qword ptr [esp+50h+var_50]; double
455BA7  movaps  [esp+50h+var_10], xmm0
455BAC  call    sub_4010B0
455BB1  fstp    [esp+50h+var_34]
455BB5  fld     [esp+50h+var_34]
455BB9  movaps  xmm1, [esp+50h+var_30]
455BBE  movaps  xmm2, [esp+50h+var_10]
455BC3  fstp    [esp+50h+var_34]
455BC7  movss   xmm0, [esp+50h+var_34]
455BCD  fld     [ebp+arg_18]
455BD0  add     esp, 4
455BD3  fstp    [esp+4Ch+var_48+4]; float
455BD7  mov     ecx, ebx
455BD9  fld     [ebp+arg_14]
455BDC  mov     edx, edi
455BDE  fstp    [esp+4Ch+var_48]; float
455BE2  fld     [ebp+arg_10]
455BE5  unpcklps xmm0, xmm1
455BE8  fstp    [esp+4Ch+var_50+4]; float
455BEB  movaps  xmm1, [esp+4Ch+var_20]
455BF0  unpcklps xmm2, xmm1
455BF3  unpcklps xmm0, xmm2
455BF6  movaps  xmmword ptr [esi+30h], xmm0
455BFA  call    sub_455DF0
455BFF  xor     eax, eax
455C01  pop     esi
455C02  pop     ebx
455C03  mov     esp, ebp
455C05  pop     ebp
455C06  retn    30h ; '0'
455C09  mov     eax, 0A0000003h
455C0E  fstp    st
455C10  pop     esi
455C11  pop     ebx
455C12  mov     esp, ebp
455C14  pop     ebp
455C15  retn    30h ; '0'
455C18  mov     eax, 0A0000002h
455C1D  fstp    st
455C1F  pop     esi
455C20  pop     ebx
455C21  mov     esp, ebp
455C23  pop     ebp
455C24  retn    30h ; '0'

;==== 455C30 sub_455C30
455C30  push    ebp
455C31  mov     ebp, esp
455C33  push    ecx
455C34  mov     edx, [ebp+arg_4]
455C37  mov     ecx, [ebp+arg_0]
455C3A  mov     eax, 0A0000001h
455C3F  cmp     edx, [ecx+10h]
455C42  jnb     short loc_455CBB
455C44  push    esi
455C45  lea     esi, [edx+edx*2]
455C48  shl     esi, 5
455C4B  add     esi, [ecx+14h]
455C4E  cmp     dword ptr [esi], 0
455C51  mov     [ebp+var_4], esi
455C54  jz      short loc_455CBA
455C56  push    ebx
455C57  xor     ebx, ebx
455C59  push    edi
455C5A  add     esi, 20h ; ' '
455C5D  jmp     short loc_455C66
455C60  mov     ecx, [ebp+arg_0]
455C63  mov     edx, [ebp+arg_4]
455C66  push    0FFFFFFFFh
455C68  push    ebx
455C69  push    edx
455C6A  call    sub_455F90
455C6F  xor     edi, edi
455C71  cmp     [esi], edi
455C73  jbe     short loc_455C91
455C75  mov     eax, [esi+4]
455C78  mov     eax, [eax+edi*4]
455C7B  cmp     eax, 0FFFFFFFFh
455C7E  jz      short loc_455C8C
455C80  mov     ecx, [ebp+arg_0]
455C83  push    0FFFFFFFFh
455C85  push    ebx
455C86  push    eax
455C87  call    sub_455F90
455C8C  inc     edi
455C8D  cmp     edi, [esi]
455C8F  jb      short loc_455C75
455C91  mov     ecx, [esi+4]
455C94  push    ecx; void *
455C95  call    ??3@YAXPAX@Z; operator delete(void *)
455C9A  inc     ebx
455C9B  add     esp, 4
455C9E  add     esi, 8
455CA1  cmp     ebx, 2
455CA4  jb      short loc_455C60
455CA6  mov     edx, [ebp+var_4]
455CA9  push    60h ; '`'; Size
455CAB  push    0; Val
455CAD  push    edx; void *
455CAE  call    _memset
455CB3  add     esp, 0Ch
455CB6  pop     edi
455CB7  xor     eax, eax
455CB9  pop     ebx
455CBA  pop     esi
455CBB  mov     esp, ebp
455CBD  pop     ebp
455CBE  retn    8

;==== 455CD0 sub_455CD0
455CD0  push    ebp
455CD1  mov     ebp, esp
455CD3  and     esp, 0FFFFFFF0h
455CD6  sub     esp, 3Ch
455CD9  push    esi
455CDA  mov     eax, 0A0000001h
455CDF  cmp     ecx, [edx+10h]
455CE2  jnb     loc_455DAE
455CE8  lea     esi, [ecx+ecx*2]
455CEB  shl     esi, 5
455CEE  add     esi, [edx+14h]
455CF1  cmp     dword ptr [esi], 0
455CF4  jz      loc_455DAE
455CFA  fldz
455CFC  sub     esp, 10h
455CFF  fstp    [esp+50h+var_34]
455D03  fld     ds:dbl_4EC9F0
455D09  movss   xmm0, [esp+50h+var_34]
455D0F  fstp    [esp+50h+var_48]; double
455D13  fld     [ebp+arg_8]
455D16  fstp    [esp+50h+var_50]; double
455D19  movaps  [esp+50h+var_20], xmm0
455D1E  call    sub_4010B0
455D23  fstp    [esp+50h+var_34]
455D27  fld     [esp+50h+var_34]
455D2B  fstp    [esp+50h+var_34]
455D2F  movss   xmm0, [esp+50h+var_34]
455D35  fld     ds:dbl_4EC9F0
455D3B  fstp    [esp+50h+var_48]; double
455D3F  fld     [ebp+arg_4]
455D42  fstp    [esp+50h+var_50]; double
455D45  movaps  [esp+50h+var_30], xmm0
455D4A  call    sub_4010B0
455D4F  fstp    [esp+50h+var_34]
455D53  fld     [esp+50h+var_34]
455D57  fstp    [esp+50h+var_34]
455D5B  movss   xmm0, [esp+50h+var_34]
455D61  fld     ds:dbl_4EC9F0
455D67  fstp    [esp+50h+var_48]; double
455D6B  fld     [ebp+arg_0]
455D6E  fstp    [esp+50h+var_50]; double
455D71  movaps  [esp+50h+var_10], xmm0
455D76  call    sub_4010B0
455D7B  fstp    [esp+50h+var_34]
455D7F  fld     [esp+50h+var_34]
455D83  movaps  xmm1, [esp+50h+var_30]
455D88  movaps  xmm2, [esp+50h+var_10]
455D8D  fstp    [esp+50h+var_34]
455D91  movss   xmm0, [esp+50h+var_34]
455D97  add     esp, 10h
455D9A  xor     eax, eax
455D9C  unpcklps xmm0, xmm1
455D9F  movaps  xmm1, [esp+40h+var_20]
455DA4  unpcklps xmm2, xmm1
455DA7  unpcklps xmm0, xmm2
455DAA  movaps  xmmword ptr [esi+30h], xmm0
455DAE  pop     esi
455DAF  mov     esp, ebp
455DB1  pop     ebp
455DB2  retn    0Ch

;==== 455DC0 sub_455DC0
455DC0  push    ebp
455DC1  mov     ebp, esp
455DC3  mov     eax, 0A0000001h
455DC8  cmp     ecx, [edx+10h]
455DCB  jnb     short loc_455DE7
455DCD  lea     ecx, [ecx+ecx*2]
455DD0  shl     ecx, 5
455DD3  add     ecx, [edx+14h]
455DD6  cmp     dword ptr [ecx], 0
455DD9  jz      short loc_455DE7
455DDB  mov     eax, [ebp+arg_0]
455DDE  movaps  xmm0, xmmword ptr [ecx+30h]
455DE2  movups  xmmword ptr [eax], xmm0
455DE5  xor     eax, eax
455DE7  pop     ebp
455DE8  retn    4

;==== 455DF0 sub_455DF0
455DF0  push    ebp
455DF1  mov     ebp, esp
455DF3  and     esp, 0FFFFFFF8h
455DF6  sub     esp, 14h
455DF9  push    esi
455DFA  mov     eax, 0A0000001h
455DFF  cmp     ecx, [edx+10h]
455E02  jnb     loc_455F4A
455E08  lea     esi, [ecx+ecx*2]
455E0B  shl     esi, 5
455E0E  add     esi, [edx+14h]
455E11  cmp     dword ptr [esi], 0
455E14  jz      loc_455F4A
455E1A  fldz
455E1C  sub     esp, 10h
455E1F  fstp    [esp+28h+var_10]
455E23  fld     ds:dbl_4EC9F0
455E29  movss   xmm0, [esp+28h+var_10]
455E2F  fstp    [esp+28h+var_20]; double
455E33  fld     [ebp+arg_8]
455E36  movss   [esp+28h+var_8], xmm0
455E3C  fstp    [esp+28h+var_28]; double
455E3F  call    sub_4010B0
455E44  fstp    [esp+28h+var_10]
455E48  fld     [esp+28h+var_10]
455E4C  fstp    [esp+28h+var_10]
455E50  fld     ds:dbl_4EC9F0
455E56  movss   xmm0, [esp+28h+var_10]
455E5C  fstp    [esp+28h+var_20]; double
455E60  fld     [ebp+arg_4]
455E63  movss   [esp+28h+var_C], xmm0
455E69  fstp    [esp+28h+var_28]; double
455E6C  call    sub_4010B0
455E71  fstp    [esp+28h+var_10]
455E75  fld     [esp+28h+var_10]
455E79  fstp    [esp+28h+var_10]
455E7D  fld     ds:dbl_4EC9F0
455E83  movss   xmm0, [esp+28h+var_10]
455E89  fstp    [esp+28h+var_20]; double
455E8D  fld     [ebp+arg_0]
455E90  movss   [esp+28h+var_4], xmm0
455E96  fstp    [esp+28h+var_28]; double
455E99  call    sub_4010B0
455E9E  fstp    [esp+28h+var_10]
455EA2  fld     [esp+28h+var_10]
455EA6  movss   xmm0, [esp+28h+var_C]
455EAC  movss   xmm1, [esp+28h+var_4]
455EB2  fstp    [esp+28h+var_10]
455EB6  add     esp, 10h
455EB9  movss   xmm2, [esp+18h+var_10]
455EBF  unpcklps xmm2, xmm0
455EC2  movss   xmm0, [esp+18h+var_8]
455EC8  unpcklps xmm1, xmm0
455ECB  unpcklps xmm2, xmm1
455ECE  movaps  xmm1, xmm2
455ED1  mulps   xmm1, xmm2
455ED4  movaps  xmm0, xmm1
455ED7  shufps  xmm0, xmm1, 0FEh
455EDB  movaps  xmm3, xmm1
455EDE  shufps  xmm3, xmm1, 0FDh
455EE2  addss   xmm3, xmm0
455EE6  addss   xmm1, xmm3
455EEA  xorps   xmm0, xmm0
455EED  comiss  xmm1, xmm0
455EF0  jbe     short loc_455F45
455EF2  movss   xmm3, ds:dword_4EC974
455EFA  movaps  xmm0, xmm1
455EFD  rsqrtss xmm0, xmm0
455F01  mulss   xmm1, xmm0
455F05  mulss   xmm1, xmm0
455F09  mulss   xmm1, xmm0
455F0D  mulss   xmm1, xmm3
455F11  movss   xmm3, ds:dword_4EC970
455F19  mulss   xmm0, xmm3
455F1D  addss   xmm0, xmm1
455F21  movss   xmm1, dword ptr [esi+8]
455F26  shufps  xmm0, xmm0, 0C0h
455F2A  mulps   xmm0, xmm2
455F2D  shufps  xmm1, xmm1, 0
455F31  mulps   xmm1, xmm0
455F34  movaps  xmmword ptr [esi+40h], xmm0
455F38  movaps  xmmword ptr [esi+50h], xmm1
455F3C  xor     eax, eax
455F3E  pop     esi
455F3F  mov     esp, ebp
455F41  pop     ebp
455F42  retn    0Ch
455F45  mov     eax, 0A0000002h
455F4A  pop     esi
455F4B  mov     esp, ebp
455F4D  pop     ebp
455F4E  retn    0Ch

;==== 455F60 sub_455F60
455F60  push    ebp
455F61  mov     ebp, esp
455F63  mov     eax, 0A0000001h
455F68  cmp     ecx, [edx+10h]
455F6B  jnb     short loc_455F87
455F6D  lea     ecx, [ecx+ecx*2]
455F70  shl     ecx, 5
455F73  add     ecx, [edx+14h]
455F76  cmp     dword ptr [ecx], 0
455F79  jz      short loc_455F87
455F7B  mov     eax, [ebp+arg_0]
455F7E  movaps  xmm0, xmmword ptr [ecx+40h]
455F82  movups  xmmword ptr [eax], xmm0
455F85  xor     eax, eax
455F87  pop     ebp
455F88  retn    4

;==== 455F90 sub_455F90
455F90  push    ebp
455F91  mov     ebp, esp
455F93  sub     esp, 0Ch
455F96  push    ebx
455F97  push    esi
455F98  mov     esi, [ecx+10h]
455F9B  push    edi
455F9C  mov     edi, [ebp+arg_0]
455F9F  mov     ebx, 0A0000001h
455FA4  cmp     edi, esi
455FA6  jnb     loc_456150
455FAC  mov     edx, [ecx+14h]
455FAF  lea     eax, [edi+edi*2]
455FB2  shl     eax, 5
455FB5  add     eax, edx
455FB7  cmp     dword ptr [eax], 0
455FBA  mov     [ebp+var_C], eax
455FBD  jz      loc_456150
455FC3  mov     ebx, [ebp+arg_4]
455FC6  cmp     ebx, 2
455FC9  jnb     loc_456142
455FCF  cmp     [ebp+arg_8], esi
455FD2  jnb     loc_4560D0
455FD8  cmp     [ebp+arg_8], edi
455FDB  jz      loc_4560D0
455FE1  mov     esi, [ebp+arg_8]
455FE4  lea     esi, [esi+esi*2]
455FE7  shl     esi, 5
455FEA  add     esi, edx
455FEC  cmp     dword ptr [esi], 0
455FEF  jz      loc_456134
455FF5  push    0FFFFFFFFh
455FF7  push    ebx
455FF8  push    edi
455FF9  call    sub_455F90
455FFE  mov     edx, [esi+ebx*8+20h]
456002  xor     eax, eax
456004  test    edx, edx
456006  jz      short loc_456035
456008  lea     ecx, [esi+ebx*8+24h]
45600C  mov     [ebp+var_4], ecx
45600F  mov     ecx, [ecx]
456011  cmp     dword ptr [ecx], 0FFFFFFFFh
456014  jz      short loc_456020
456016  inc     eax
456017  add     ecx, 4
45601A  cmp     eax, edx
45601C  jb      short loc_456011
45601E  jmp     short loc_456029
456020  cmp     eax, 0FFFFFFFFh
456023  jnz     loc_4560B3
456029  test    edx, edx
45602B  jz      short loc_456035
45602D  lea     eax, [edx+edx]
456030  mov     [ebp+arg_4], eax
456033  jmp     short loc_45603C
456035  mov     [ebp+arg_4], 40h ; '@'
45603C  mov     eax, [ebp+arg_4]
45603F  xor     ecx, ecx
456041  mov     edx, 4
456046  mul     edx
456048  seto    cl
45604B  neg     ecx
45604D  or      ecx, eax
45604F  push    ecx; Size
456050  call    ??2@YAPAXI@Z; operator new(uint)
456055  lea     edi, [esi+ebx*8+24h]
456059  mov     ecx, eax
45605B  mov     eax, [edi]
45605D  add     esp, 4
456060  mov     [ebp+var_8], ecx
456063  mov     [ebp+var_4], edi
456066  test    eax, eax
456068  jz      short loc_456088
45606A  mov     edx, [esi+ebx*8+20h]
45606E  add     edx, edx
456070  add     edx, edx
456072  push    edx; Size
456073  push    eax; Src
456074  push    ecx; void *
456075  call    _memcpy_0
45607A  mov     eax, [edi]
45607C  push    eax; void *
45607D  call    ??3@YAXPAX@Z; operator delete(void *)
456082  mov     ecx, [ebp+var_8]
456085  add     esp, 10h
456088  mov     eax, [esi+ebx*8+20h]
45608C  mov     edx, [ebp+arg_4]
45608F  inc     eax
456090  cmp     eax, edx
456092  jnb     short loc_4560A6
456094  lea     edi, [ecx+eax*4]
456097  mov     ecx, edx
456099  sub     ecx, eax
45609B  or      eax, 0FFFFFFFFh
45609E  rep stosd
4560A0  mov     ecx, [ebp+var_8]
4560A3  mov     edi, [ebp+var_4]
4560A6  mov     eax, [esi+ebx*8+20h]
4560AA  mov     [edi], ecx
4560AC  mov     edi, [ebp+arg_0]
4560AF  mov     [esi+ebx*8+20h], edx
4560B3  mov     ecx, [ebp+var_4]
4560B6  mov     edx, [ecx]
4560B8  mov     ecx, [ebp+var_C]
4560BB  mov     [edx+eax*4], edi
4560BE  mov     eax, [ebp+arg_8]
4560C1  pop     edi
4560C2  mov     [ecx+ebx*4+18h], eax
4560C6  pop     esi
4560C7  xor     eax, eax
4560C9  pop     ebx
4560CA  mov     esp, ebp
4560CC  pop     ebp
4560CD  retn    0Ch
4560D0  cmp     [ebp+arg_8], 0FFFFFFFFh
4560D4  jnz     short loc_456134
4560D6  mov     eax, [eax+ebx*4+18h]
4560DA  cmp     eax, esi
4560DC  jnb     short loc_456129
4560DE  lea     eax, [eax+eax*2]
4560E1  lea     ecx, [ebx+eax*4]
4560E4  lea     edx, [edx+ecx*8+20h]
4560E8  mov     ecx, [edx]
4560EA  xor     eax, eax
4560EC  test    ecx, ecx
4560EE  jz      short loc_45611E
4560F0  mov     edx, [edx+4]
4560F3  mov     esi, edx
4560F5  cmp     [esi], edi
4560F7  jz      short loc_456117
4560F9  inc     eax
4560FA  add     esi, 4
4560FD  cmp     eax, ecx
4560FF  jb      short loc_4560F5
456101  mov     edx, [ebp+var_C]
456104  pop     edi
456105  pop     esi
456106  mov     dword ptr [edx+ebx*4+18h], 0FFFFFFFFh
45610E  xor     eax, eax
456110  pop     ebx
456111  mov     esp, ebp
456113  pop     ebp
456114  retn    0Ch
456117  mov     dword ptr [edx+eax*4], 0FFFFFFFFh
45611E  mov     edx, [ebp+var_C]
456121  mov     dword ptr [edx+ebx*4+18h], 0FFFFFFFFh
456129  pop     edi
45612A  pop     esi
45612B  xor     eax, eax
45612D  pop     ebx
45612E  mov     esp, ebp
456130  pop     ebp
456131  retn    0Ch
456134  pop     edi
456135  pop     esi
456136  mov     eax, 0A0000002h
45613B  pop     ebx
45613C  mov     esp, ebp
45613E  pop     ebp
45613F  retn    0Ch
456142  pop     edi
456143  pop     esi
456144  mov     eax, 0A0000005h
456149  pop     ebx
45614A  mov     esp, ebp
45614C  pop     ebp
45614D  retn    0Ch
456150  pop     edi
456151  pop     esi
456152  mov     eax, ebx
456154  pop     ebx
456155  mov     esp, ebp
456157  pop     ebp
456158  retn    0Ch

;==== 456160 sub_456160
456160  push    ebp
456161  mov     ebp, esp
456163  mov     eax, 0A0000001h
456168  cmp     ecx, [edx+10h]
45616B  jnb     short loc_456194
45616D  lea     ecx, [ecx+ecx*2]
456170  shl     ecx, 5
456173  add     ecx, [edx+14h]
456176  cmp     dword ptr [ecx], 0
456179  jz      short loc_456194
45617B  cmp     esi, 2
45617E  jnb     short loc_45618F
456180  mov     eax, [ecx+esi*4+18h]
456184  mov     ecx, [ebp+arg_0]
456187  mov     [ecx], eax
456189  xor     eax, eax
45618B  pop     ebp
45618C  retn    4
45618F  mov     eax, 0A0000005h
456194  pop     ebp
456195  retn    4

;==== 4561A0 sub_4561A0
4561A0  push    ebp
4561A1  mov     ebp, esp
4561A3  push    ebx
4561A4  mov     ebx, [ebp+arg_0]
4561A7  mov     eax, 0A0000001h
4561AC  cmp     ecx, [edx+10h]
4561AF  jnb     short loc_4561FD
4561B1  lea     ecx, [ecx+ecx*2]
4561B4  shl     ecx, 5
4561B7  add     ecx, [edx+14h]
4561BA  cmp     dword ptr [ecx], 0
4561BD  jz      short loc_4561FD
4561BF  cmp     edi, 2
4561C2  jnb     short loc_4561F8
4561C4  push    esi
4561C5  xor     edx, edx
4561C7  xor     esi, esi
4561C9  cmp     [ecx+edi*8+20h], edx
4561CD  jbe     short loc_4561EB
4561CF  nop
4561D0  mov     eax, [ecx+edi*8+24h]
4561D4  mov     eax, [eax+edx*4]
4561D7  cmp     eax, 0FFFFFFFFh
4561DA  jz      short loc_4561E4
4561DC  test    ebx, ebx
4561DE  jz      short loc_4561E3
4561E0  mov     [ebx+esi*4], eax
4561E3  inc     esi
4561E4  inc     edx
4561E5  cmp     edx, [ecx+edi*8+20h]
4561E9  jb      short loc_4561D0
4561EB  mov     ecx, [ebp+arg_4]
4561EE  mov     [ecx], esi
4561F0  pop     esi
4561F1  xor     eax, eax
4561F3  pop     ebx
4561F4  pop     ebp
4561F5  retn    8
4561F8  mov     eax, 0A0000005h
4561FD  pop     ebx
4561FE  pop     ebp
4561FF  retn    8

;==== 456210 sub_456210
456210  push    ebp
456211  mov     ebp, esp
456213  and     esp, 0FFFFFFF8h
456216  mov     eax, 0A0000001h
45621B  cmp     edx, [esi+10h]
45621E  jnb     short loc_45626F
456220  lea     ecx, [edx+edx*2]
456223  shl     ecx, 5
456226  add     ecx, [esi+14h]
456229  cmp     dword ptr [ecx], 0
45622C  jz      short loc_45626F
45622E  mov     eax, [ebp+arg_10]
456231  fld     dword ptr [ecx+4]
456234  movss   xmm0, ds:dword_4EC8A8
45623C  movss   xmm1, [ebp+arg_8]
456241  movss   xmm2, [ebp+arg_4]
456246  movss   xmm3, [ebp+arg_0]
45624B  push    eax
45624C  mov     eax, [ebp+arg_C]
45624F  push    eax
456250  push    edx
456251  mov     edx, [ecx+14h]
456254  push    edx
456255  push    ecx
456256  unpcklps xmm3, xmm1
456259  fstp    [esp+14h+var_14]
45625C  unpcklps xmm2, xmm0
45625F  movaps  xmm0, xmmword ptr [ecx+30h]
456263  unpcklps xmm3, xmm2
456266  push    esi
456267  movaps  xmm1, xmm3
45626A  call    sub_456D70
45626F  mov     esp, ebp
456271  pop     ebp
456272  retn    14h

;==== 456280 sub_456280
456280  push    ebp
456281  mov     ebp, esp
456283  and     esp, 0FFFFFFF0h
456286  sub     esp, 54h
456289  push    ebx
45628A  mov     ebx, [ebp+arg_C]
45628D  push    esi
45628E  mov     esi, eax
456290  mov     eax, dword_56670C
456295  mov     ecx, [esi+10h]
456298  push    edi
456299  mov     edi, [ebp+arg_8]
45629C  mov     [esp+60h+var_24], eax
4562A0  mov     eax, 0A0000001h
4562A5  cmp     edi, ecx
4562A7  jnb     loc_45694C
4562AD  mov     edx, [esi+14h]
4562B0  lea     edi, [edi+edi*2]
4562B3  shl     edi, 5
4562B6  add     edi, edx
4562B8  cmp     dword ptr [edi], 0
4562BB  mov     [esp+60h+var_2C], edi
4562BF  jz      loc_45694C
4562C5  mov     eax, 0A0000002h
4562CA  cmp     ebx, ecx
4562CC  jnb     loc_45694C
4562D2  lea     ebx, [ebx+ebx*2]
4562D5  shl     ebx, 5
4562D8  add     ebx, edx
4562DA  cmp     dword ptr [ebx], 0
4562DD  mov     [esp+60h+var_44], ebx
4562E1  jz      loc_45694C
4562E7  fld     [ebp+arg_14]
4562EA  stmxcsr [esp+60h+var_40]
4562EF  fldz
4562F1  mov     ecx, [esp+60h+var_40]
4562F5  fucompp
4562F7  and     ecx, 0F3FFh
4562FD  mov     [esp+60h+var_40], ecx
456301  ldmxcsr [esp+60h+var_40]
456306  mov     [esp+60h+var_4C], 0A0000006h
45630E  fnstsw  ax
456310  test    ah, 44h
456313  jp      short loc_45631A
456315  fld1
456317  fstp    [ebp+arg_14]
45631A  movss   xmm0, [ebp+arg_14]
45631F  shufps  xmm0, xmm0, 0
456323  rcpps   xmm1, xmm0
456326  movaps  xmm2, xmm1
456329  mulps   xmm2, xmm1
45632C  mulps   xmm2, xmm0
45632F  movaps  xmm0, xmm1
456332  addps   xmm0, xmm1
456335  movss   xmm1, [ebp+arg_10]
45633A  subps   xmm0, xmm2
45633D  mulss   xmm1, xmm0
456341  movss   [esp+60h+var_40], xmm1
456347  fld     [esp+60h+var_40]
45634B  sub     esp, 8
45634E  fstp    [esp+68h+var_40]
456352  movaps  [esp+68h+var_10], xmm0
456357  fld     [esp+68h+var_40]
45635B  fstp    [esp+68h+X]; X
45635E  call    _ceil
456363  fstp    [esp+68h+var_40]
456367  add     esp, 8
45636A  fld     [esp+60h+var_40]
45636E  call    __ftol2_sse
456373  cdq
456374  xor     eax, edx
456376  sub     eax, edx
456378  cmp     eax, 1
45637B  jge     short loc_456382
45637D  mov     eax, 1
456382  movaps  xmm1, xmmword ptr [ebx+30h]
456386  subps   xmm1, xmmword ptr [edi+30h]
45638A  movaps  xmm2, ds:xmmword_4E4260
456391  add     eax, eax
456393  lea     ecx, [edi+30h]
456396  mulps   xmm1, xmm2
456399  mulps   xmm1, [esp+60h+var_10]
45639E  movd    xmm0, eax
4563A2  pshufd  xmm0, xmm0, 0
4563A7  cvtps2dq xmm1, xmm1
4563AB  paddd   xmm0, xmm1
4563AF  lea     ebx, [eax+eax+1]
4563B3  movdqa  [esp+60h+var_20], xmm0
4563B9  mov     [esp+60h+var_28], eax
4563BD  mov     [esp+60h+var_40], ecx
4563C1  cmp     dword ptr [esp+60h+var_20], ebx
4563C5  jge     loc_456948
4563CB  cmp     dword ptr [esp+60h+var_20+4], ebx
4563CF  jge     loc_456948
4563D5  mov     [esi+18h], ebx
4563D8  mov     [esi+1Ch], ebx
4563DB  imul    ebx, ebx
4563DE  shl     ebx, 5
4563E1  push    10h; Alignment
4563E3  push    ebx; Size
4563E4  call    __aligned_malloc
4563E9  push    ebx; Size
4563EA  push    0; Val
4563EC  push    eax; void *
4563ED  mov     [esi+20h], eax
4563F0  call    _memset
4563F5  add     esp, 14h
4563F8  push    100h; Size
4563FD  lea     edx, [esi+30h]
456400  push    0; Val
456402  push    edx; void *
456403  call    _memset
456408  mov     eax, [esp+6Ch+var_44]
45640C  movss   xmm0, dword ptr [eax+4]
456411  movss   xmm1, dword ptr [edi+4]
456416  movss   xmm2, ds:dword_4ECA60
45641E  addss   xmm1, xmm0
456422  xorps   xmm0, xmm0
456425  movss   xmm0, xmm2
456429  mulss   xmm1, xmm0
45642D  mulss   xmm1, dword ptr [esp+6Ch+var_10]
456433  movss   [esp+6Ch+var_44], xmm1
456439  fld     [esp+6Ch+var_44]
45643D  add     esp, 8
456440  fstp    dword ptr [esp+64h+X+4]; float
456443  call    sub_455840
456448  add     esp, 4
45644B  call    __ftol2_sse
456450  cdq
456451  mov     ebx, eax
456453  xor     ebx, edx
456455  sub     ebx, edx
456457  mov     [esp+60h+var_48], ebx
45645B  cmp     ebx, 4
45645E  jl      short loc_45647B
456460  fild    [esp+60h+var_48]
456464  fadd    ds:dbl_4ECA58
45646A  fmul    ds:dbl_4ECA50
456470  call    __ftol2_sse
456475  mov     [esp+60h+var_44], eax
456479  jmp     short loc_456481
45647B  mov     eax, ebx
45647D  mov     [esp+60h+var_44], ebx
456481  mov     ecx, dword ptr [esp+60h+var_20+4]
456485  mov     edx, dword ptr [esp+60h+var_20]
456489  lea     edi, [eax+ecx]
45648C  sub     edx, eax
45648E  sub     eax, ebx
456490  mov     [esp+60h+var_38], eax
456494  mov     eax, dword ptr [esp+60h+var_20+4]
456498  sub     eax, dword ptr [esp+60h+var_20]
45649C  sub     ecx, ebx
45649E  mov     ebx, dword ptr [esp+60h+var_20]
4564A2  mov     [esp+60h+var_34], eax
4564A6  mov     eax, [esp+60h+var_48]
4564AA  sub     eax, [esp+60h+var_44]
4564AE  sub     ebx, dword ptr [esp+60h+var_20+4]
4564B2  mov     [esp+60h+var_50], ecx
4564B6  lea     ecx, [esi+34h]
4564B9  mov     [esp+60h+var_3C], eax
4564BD  mov     [esp+60h+var_44], ebx
4564C1  mov     [esp+60h+var_48], 2
4564C9  jmp     short loc_4564D4
4564CB  jmp     short loc_4564D0
4564D0  mov     ebx, [esp+60h+var_44]
4564D4  mov     eax, [esp+60h+var_3C]
4564D8  add     eax, edi
4564DA  add     ebx, eax
4564DC  mov     [ecx-4], ebx
4564DF  mov     ebx, dword ptr [esp+60h+var_20+4]
4564E3  mov     [ecx+20h], eax
4564E6  mov     eax, [esp+60h+var_38]
4564EA  mov     [ecx], ebx
4564EC  mov     ebx, [esp+60h+var_44]
4564F0  add     eax, edx
4564F2  add     ebx, edi
4564F4  mov     [ecx+3Ch], eax
4564F7  mov     eax, dword ptr [esp+60h+var_20+4]
4564FB  mov     [ecx+0Ch], ebx
4564FE  mov     [esp+60h+var_30], ebx
456502  mov     ebx, dword ptr [esp+60h+var_20]
456506  mov     [ecx+40h], eax
456509  mov     eax, [esp+60h+var_34]
45650D  mov     [ecx+1Ch], ebx
456510  add     eax, edx
456512  mov     [ecx+5Ch], ebx
456515  mov     ebx, [esp+60h+var_50]
456519  mov     [ecx+60h], ebx
45651C  mov     ebx, [esp+60h+var_30]
456520  mov     [ecx+50h], eax
456523  mov     [ecx+70h], eax
456526  mov     eax, 1
45652B  sub     [esp+60h+var_50], eax
45652F  mov     [ecx+10h], edi
456532  mov     [ecx+2Ch], edx
456535  mov     [ecx+30h], edi
456538  mov     [ecx+4Ch], edx
45653B  mov     [ecx+6Ch], ebx
45653E  add     edi, eax
456540  sub     edx, eax
456542  sub     ecx, 0FFFFFF80h
456545  sub     [esp+60h+var_48], eax
456549  jnz     short loc_4564D0
45654B  mov     eax, [esp+60h+var_2C]
45654F  fld     dword ptr [eax+4]
456552  mov     edx, [ebp+arg_8]
456555  fstp    dword ptr [esi+130h]
45655B  mov     ecx, [eax+14h]
45655E  mov     eax, [esp+60h+var_40]
456562  fld     [ebp+arg_10]
456565  mov     [esi+138h], edx
45656B  fstp    dword ptr [esi+13Ch]
456571  mov     edx, [ebp+arg_1C]
456574  fld     [ebp+arg_14]
456577  mov     [esi+134h], ecx
45657D  fstp    dword ptr [esi+140h]
456583  mov     ecx, [ebp+arg_18]
456586  mov     [esi+144h], eax
45658C  mov     eax, [ebp+arg_20]
45658F  mov     [esi+14Ch], edx
456595  mov     [esi+148h], ecx
45659B  mov     edx, 4
4565A0  mov     ecx, eax
4565A2  shl     edx, cl
4565A4  cmp     edx, 10h
4565A7  jle     short loc_4565AE
4565A9  mov     eax, 2
4565AE  push    10h; Alignment
4565B0  push    100h; Size
4565B5  mov     [esi+150h], eax
4565BB  call    __aligned_malloc
4565C0  push    100h; Size
4565C5  push    0; Val
4565C7  push    eax; void *
4565C8  mov     [esi+154h], eax
4565CE  call    _memset
4565D3  add     esp, 14h
4565D6  push    10h; Alignment
4565D8  push    100h; Size
4565DD  call    __aligned_malloc
4565E2  fld     [ebp+arg_14]
4565E5  fld     st
4565E7  mov     edi, 0FFFFFFFEh
4565EC  fchs
4565EE  mov     ebx, 2
4565F3  fstp    [esp+68h+var_2C]
4565F7  mov     [esi+158h], eax
4565FD  fld     ds:dbl_4E1EE8
456603  add     esp, 8
456606  fld     [esp+60h+var_2C]
45660A  mov     [esp+60h+var_48], edi
45660E  fldz
456610  mov     [esp+60h+var_44], ebx
456614  lea     eax, [edi+62h]
456617  mov     [esp+60h+var_50], 4
45661F  jmp     short loc_456627
456621  fxch    st(1)
456623  fxch    st(3)
456625  fxch    st(1)
456627  mov     ecx, [esi+154h]
45662D  fild    [esp+60h+var_48]
456631  mov     edx, edi
456633  and     edx, 0Fh
456636  fmul    st, st(4)
456638  add     edx, edx
45663A  mov     dword ptr [ecx+edx*8], 2
456641  mov     [ecx+edx*8+4], edi
456645  mov     [eax+ecx-40h], ebx
456649  fmul    st, st(3)
45664B  mov     dword ptr [eax+ecx-3Ch], 2
456653  mov     dword ptr [eax+ecx], 0FFFFFFFEh
45665A  mov     [eax+ecx+4], ebx
45665E  fstp    [esp+60h+var_30]
456662  mov     [eax+ecx+40h], edi
456666  mov     dword ptr [eax+ecx+44h], 0FFFFFFFEh
45666E  mov     ecx, [esi+158h]
456674  fst     [esp+60h+var_2C]
456678  movss   xmm0, [esp+60h+var_2C]
45667E  fst     [esp+60h+var_2C]
456682  movss   xmm1, [esp+60h+var_2C]
456688  fld     [esp+60h+var_30]
45668C  fst     [esp+60h+var_2C]
456690  add     eax, 10h
456693  movss   xmm2, [esp+60h+var_2C]
456699  fxch    st(4)
45669B  fst     [esp+60h+var_2C]
45669F  movss   xmm3, [esp+60h+var_2C]
4566A5  fild    [esp+60h+var_44]
4566A9  fmul    st, st(1)
4566AB  unpcklps xmm2, xmm0
4566AE  fmul    st, st(4)
4566B0  unpcklps xmm3, xmm1
4566B3  unpcklps xmm3, xmm2
4566B6  movaps  xmmword ptr [ecx+edx*8], xmm3
4566BA  fstp    [esp+60h+var_30]
4566BE  fxch    st(1)
4566C0  fst     [esp+60h+var_2C]
4566C4  movss   xmm0, [esp+60h+var_2C]
4566CA  fst     [esp+60h+var_2C]
4566CE  movss   xmm1, [esp+60h+var_2C]
4566D4  fxch    st(1)
4566D6  fst     [esp+60h+var_2C]
4566DA  movss   xmm2, [esp+60h+var_2C]
4566E0  fld     [esp+60h+var_30]
4566E4  fst     [esp+60h+var_2C]
4566E8  movss   xmm3, [esp+60h+var_2C]
4566EE  fxch    st(2)
4566F0  unpcklps xmm2, xmm0
4566F3  unpcklps xmm3, xmm1
4566F6  unpcklps xmm3, xmm2
4566F9  movaps  xmmword ptr [eax+ecx-50h], xmm3
4566FE  fst     [esp+60h+var_2C]
456702  movss   xmm0, [esp+60h+var_2C]
456708  fst     [esp+60h+var_2C]
45670C  movss   xmm1, [esp+60h+var_2C]
456712  fxch    st(2)
456714  fstp    [esp+60h+var_2C]
456718  movss   xmm2, [esp+60h+var_2C]
45671E  fxch    st(2)
456720  fst     [esp+60h+var_2C]
456724  movss   xmm3, [esp+60h+var_2C]
45672A  fxch    st(1)
45672C  unpcklps xmm2, xmm0
45672F  unpcklps xmm3, xmm1
456732  unpcklps xmm3, xmm2
456735  movaps  xmmword ptr [eax+ecx-10h], xmm3
45673A  fst     [esp+60h+var_2C]
45673E  movss   xmm0, [esp+60h+var_2C]
456744  fst     [esp+60h+var_2C]
456748  movss   xmm1, [esp+60h+var_2C]
45674E  fxch    st(1)
456750  fst     [esp+60h+var_2C]
456754  movss   xmm2, [esp+60h+var_2C]
45675A  fxch    st(4)
45675C  fstp    [esp+60h+var_2C]
456760  movss   xmm3, [esp+60h+var_2C]
456766  unpcklps xmm2, xmm0
456769  unpcklps xmm3, xmm1
45676C  unpcklps xmm3, xmm2
45676F  movaps  xmmword ptr [eax+ecx+30h], xmm3
456774  dec     ebx
456775  inc     edi
456776  dec     [esp+60h+var_50]
45677A  mov     [esp+60h+var_44], ebx
45677E  mov     [esp+60h+var_48], edi
456782  jnz     loc_456621
456788  mov     eax, [esp+60h+var_40]
45678C  fstp    st(1)
45678E  movaps  xmm0, xmmword ptr [eax]
456791  fstp    st(1)
456793  mov     eax, [esp+60h+var_28]
456797  fstp    st(1)
456799  sub     esp, 8
45679C  fst     dword ptr [esp+68h+X+4]
4567A0  movaps  xmm1, xmm0
4567A3  fstp    dword ptr [esp+68h+X]
4567A6  push    0FFFFFFFFh
4567A8  push    eax
4567A9  push    eax
4567AA  mov     eax, esi
4567AC  call    sub_457400
4567B1  mov     ebx, [ebp+arg_24]
4567B4  mov     edi, [esp+60h+var_24]
4567B8  mov     ecx, ebx
4567BA  neg     ecx
4567BC  sbb     ecx, ecx
4567BE  and     ecx, edi
4567C0  mov     [esi+160h], ecx
4567C6  push    esi
4567C7  mov     ecx, offset sub_457A10
4567CC  mov     eax, edi
4567CE  mov     dword ptr [esi+164h], 1
4567D8  mov     dword ptr [esi+168h], 0FFFFFFFFh
4567E2  call    sub_446900
4567E7  mov     eax, ebx
4567E9  mov     ecx, edi
4567EB  call    sub_446910
4567F0  xor     ebx, ebx
4567F2  push    ebx
4567F3  xor     ecx, ecx
4567F5  mov     eax, edi
4567F7  call    sub_446900
4567FC  lea     esp, [esp+0]
456800  push    ebx
456801  call    sub_457550
456806  test    eax, eax
456808  jnz     short loc_456800
45680A  mov     eax, [esi+168h]
456810  cmp     eax, ebx
456812  jl      loc_45690F
456818  add     eax, 3
45681B  shl     eax, 4
45681E  mov     edi, [eax+esi+8]
456822  mov     edx, [eax+esi+4]
456826  mov     ecx, [eax+esi]
456829  add     eax, esi
45682B  mov     eax, [eax+0Ch]
45682E  mov     dword ptr [esp+60h+var_20+8], edi
456832  mov     edi, [esi+18h]
456835  imul    edi, edx
456838  add     edi, ecx
45683A  shl     edi, 5
45683D  add     edi, [esi+20h]
456840  mov     [esp+60h+var_50], ebx
456844  cmp     dword ptr [edi], 0FFFFFFFFh
456847  mov     dword ptr [esp+60h+var_20], ecx
45684B  mov     dword ptr [esp+60h+var_20+4], edx
45684F  mov     dword ptr [esp+60h+var_20+0Ch], eax
456853  jz      short loc_4568C8
456855  push    20h ; ' '; Size
456857  call    ??2@YAPAXI@Z; operator new(uint)
45685C  mov     edx, 1
456861  add     esp, 4
456864  mov     [esp+60h+var_40], eax
456868  test    edx, edx
45686A  jz      short loc_456880
45686C  lea     eax, [edi+10h]
45686F  call    sub_455940
456874  mov     ecx, [esp+60h+var_40]
456878  movdqu  xmmword ptr [ecx], xmm0
45687C  mov     eax, ecx
45687E  jmp     short loc_456887
456880  movaps  xmm0, xmmword ptr [edi+10h]
456884  movups  xmmword ptr [eax], xmm0
456887  movdqa  xmm0, [esp+60h+var_20]
45688D  mov     [eax+10h], ebx
456890  mov     edx, [edi]
456892  mov     edi, [esi+18h]
456895  sub     edx, 8
456898  inc     [esp+60h+var_50]
45689C  and     edx, 0Fh
45689F  shl     edx, 4
4568A2  add     edx, [esi+154h]
4568A8  mov     ebx, eax
4568AA  paddd   xmm0, xmmword ptr [edx]
4568AE  movdqa  [esp+60h+var_20], xmm0
4568B4  imul    edi, dword ptr [esp+60h+var_20+4]
4568B9  add     edi, dword ptr [esp+60h+var_20]
4568BD  shl     edi, 5
4568C0  add     edi, [esi+20h]
4568C3  cmp     dword ptr [edi], 0FFFFFFFFh
4568C6  jnz     short loc_456855
4568C8  mov     eax, [esp+60h+var_50]
4568CC  test    eax, eax
4568CE  jz      short loc_456906
4568D0  mov     edi, [ebp+arg_0]
4568D3  mov     [esp+60h+var_40], eax
4568D7  mov     eax, [ebx]
4568D9  mov     [edi], eax
4568DB  mov     ecx, [ebx+4]
4568DE  mov     [edi+4], ecx
4568E1  mov     edx, [ebx+8]
4568E4  mov     [edi+8], edx
4568E7  mov     eax, [ebx+0Ch]
4568EA  mov     [edi+0Ch], eax
4568ED  push    ebx; void *
4568EE  mov     ebx, [ebx+10h]
4568F1  call    ??3@YAXPAX@Z; operator delete(void *)
4568F6  add     esp, 4
4568F9  add     edi, 10h
4568FC  dec     [esp+60h+var_40]
456900  jnz     short loc_4568D7
456902  mov     eax, [esp+60h+var_50]
456906  mov     ecx, [ebp+arg_4]
456909  mov     [ecx], eax
45690B  xor     edi, edi
45690D  jmp     short loc_456913
45690F  mov     edi, [esp+60h+var_4C]
456913  mov     edx, [esi+158h]
456919  push    edx; Block
45691A  call    __aligned_free
45691F  mov     eax, [esi+154h]
456925  add     esp, 4
456928  push    eax; Block
456929  call    __aligned_free
45692E  mov     ecx, [esi+20h]
456931  add     esp, 4
456934  push    ecx; Block
456935  call    __aligned_free
45693A  add     esp, 4
45693D  mov     eax, edi
45693F  pop     edi
456940  pop     esi
456941  pop     ebx
456942  mov     esp, ebp
456944  pop     ebp
456945  retn    28h ; '('
456948  mov     eax, [esp+60h+var_4C]
45694C  pop     edi
45694D  pop     esi
45694E  pop     ebx
45694F  mov     esp, ebp
456951  pop     ebp
456952  retn    28h ; '('

;==== 456960 sub_456960
456960  push    ebp
456961  mov     ebp, esp
456963  and     esp, 0FFFFFFF8h
456966  push    ebx
456967  mov     ebx, [ebp+arg_8]
45696A  push    edi
45696B  mov     edi, eax
45696D  mov     eax, 0A0000001h
456972  cmp     edx, [edi+10h]
456975  jnb     short loc_4569AE
456977  lea     ecx, [edx+edx*2]
45697A  shl     ecx, 5
45697D  add     ecx, [edi+14h]
456980  cmp     dword ptr [ecx], 0
456983  jz      short loc_4569AE
456985  mov     eax, ebx
456987  test    bl, bl
456989  jnz     short loc_456990
45698B  mov     eax, 0FFh
456990  fld     dword ptr [ecx+0Ch]
456993  movaps  xmm0, xmmword ptr [ecx+50h]
456997  addps   xmm0, xmmword ptr [ecx+30h]
45699B  push    eax
45699C  mov     eax, [ebp+arg_4]
45699F  push    edx
4569A0  push    ecx
4569A1  mov     ecx, [ebp+arg_0]
4569A4  fstp    [esp+14h+var_14]
4569A7  push    eax
4569A8  push    ecx
4569A9  call    sub_457340
4569AE  pop     edi
4569AF  pop     ebx
4569B0  mov     esp, ebp
4569B2  pop     ebp
4569B3  retn    0Ch

;==== 4569C0 sub_4569C0
4569C0  push    ebp
4569C1  mov     ebp, esp
4569C3  and     esp, 0FFFFFFF8h
4569C6  movss   xmm1, ds:dword_4EC8A8
4569CE  movss   xmm2, [ebp+arg_C]
4569D3  movss   xmm3, [ebp+arg_8]
4569D8  movss   xmm0, [ebp+arg_4]
4569DD  unpcklps xmm3, xmm1
4569E0  unpcklps xmm0, xmm2
4569E3  unpcklps xmm0, xmm3
4569E6  test    al, al
4569E8  jnz     short loc_4569EF
4569EA  mov     eax, 0FFh
4569EF  fld     [ebp+arg_10]
4569F2  mov     edx, [ebp+arg_0]
4569F5  push    eax
4569F6  mov     eax, [ebp+arg_14]
4569F9  push    eax
4569FA  push    ecx
4569FB  fstp    [esp+0Ch+var_C]
4569FE  push    ecx
4569FF  push    edx
456A00  call    sub_457340
456A05  mov     esp, ebp
456A07  pop     ebp
456A08  retn    18h

;==== 456A10 sub_456A10
456A10  push    ebp
456A11  mov     ebp, esp
456A13  and     esp, 0FFFFFFF0h
456A16  mov     edx, [ebp+arg_0]
456A19  mov     ecx, [ebp+arg_C]
456A1C  sub     esp, 44h
456A1F  push    ebx
456A20  push    esi
456A21  mov     esi, [edx+10h]
456A24  push    edi
456A25  mov     eax, 0A0000001h
456A2A  cmp     ecx, esi
456A2C  jnb     loc_456BF4
456A32  lea     ecx, [ecx+ecx*2]
456A35  shl     ecx, 5
456A38  add     ecx, [edx+14h]
456A3B  cmp     dword ptr [ecx], 0
456A3E  mov     [esp+50h+var_28], ecx
456A42  jz      loc_456BF4
456A48  cmp     byte ptr [ebp+arg_10], 0
456A4C  jnz     short loc_456A55
456A4E  mov     [ebp+arg_10], 0FFh
456A55  mov     eax, esi
456A57  shl     eax, 5
456A5A  push    10h; Alignment
456A5C  push    eax; Size
456A5D  call    __aligned_malloc
456A62  xor     ebx, ebx
456A64  xor     edx, edx
456A66  xor     edi, edi
456A68  add     esp, 8
456A6B  mov     ecx, eax
456A6D  mov     [esp+50h+Block], ecx
456A71  mov     [esp+50h+var_30], ebx
456A75  mov     [esp+50h+var_38], edx
456A79  mov     [esp+50h+var_34], edi
456A7D  test    esi, esi
456A7F  jz      loc_456B26
456A85  xor     eax, eax
456A87  mov     [esp+50h+var_2C], eax
456A8B  mov     esi, ecx
456A8D  lea     ecx, [ecx+0]
456A90  mov     ecx, [ebp+arg_0]
456A93  mov     ecx, [ecx+14h]
456A96  add     eax, ecx
456A98  cmp     dword ptr [eax], 0
456A9B  jz      short loc_456B0A
456A9D  cmp     edi, [ebp+arg_C]
456AA0  jz      short loc_456B0A
456AA2  mov     cl, byte ptr [ebp+arg_10]
456AA5  test    [eax+10h], cl
456AA8  jz      short loc_456B0A
456AAA  mov     edx, [esp+50h+var_28]
456AAE  movaps  xmm1, xmmword ptr [eax+30h]
456AB2  subps   xmm1, xmmword ptr [edx+30h]
456AB6  lea     eax, [esp+50h+var_20]
456ABA  mov     edi, esi
456ABC  movaps  [esp+50h+var_20], xmm1
456AC1  call    sub_4558C0
456AC6  mov     eax, [esp+50h+var_34]
456ACA  inc     [esp+50h+var_38]
456ACE  mov     [esi], eax
456AD0  movaps  xmmword ptr [esi+10h], xmm0
456AD4  movss   xmm1, dword ptr [edi+1Ch]
456AD9  add     esi, 20h ; ' '
456ADC  lea     ecx, [esp+50h+var_30]
456AE0  mov     eax, ebx
456AE2  test    ebx, ebx
456AE4  jz      short loc_456AF9
456AE6  movss   xmm0, dword ptr [eax+1Ch]
456AEB  comiss  xmm1, xmm0
456AEE  jb      short loc_456AF9
456AF0  lea     ecx, [eax+4]
456AF3  mov     eax, [ecx]
456AF5  test    eax, eax
456AF7  jnz     short loc_456AE6
456AF9  mov     edx, [esp+50h+var_38]
456AFD  mov     [edi+4], eax
456B00  mov     [ecx], edi
456B02  mov     ebx, [esp+50h+var_30]
456B06  mov     edi, [esp+50h+var_34]
456B0A  mov     eax, [esp+50h+var_2C]
456B0E  mov     ecx, [ebp+arg_0]
456B11  inc     edi
456B12  add     eax, 60h ; '`'
456B15  mov     [esp+50h+var_34], edi
456B19  mov     [esp+50h+var_2C], eax
456B1D  cmp     edi, [ecx+10h]
456B20  jb      loc_456A90
456B26  stmxcsr [esp+50h+var_28]
456B2B  mov     eax, [esp+50h+var_28]
456B2F  and     eax, 0F3FFh
456B34  mov     [esp+50h+var_28], eax
456B38  ldmxcsr [esp+50h+var_28]
456B3D  mov     eax, [esp+50h+var_30]
456B41  test    edx, edx
456B43  jz      loc_456BE0
456B49  mov     ecx, [ebp+arg_4]
456B4C  add     ecx, 10h
456B4F  mov     edi, edx
456B51  mov     esi, [eax]
456B53  mov     [ecx-10h], esi
456B56  mov     esi, 1
456B5B  test    esi, esi
456B5D  jz      short loc_456BCC
456B5F  movaps  xmm0, ds:xmmword_4E41D0
456B66  mulps   xmm0, xmmword ptr [eax+10h]
456B6A  movdqa  xmm1, ds:xmmword_4E4320
456B72  movdqa  xmm2, ds:xmmword_4E4310
456B7A  cvtps2dq xmm3, xmm0
456B7E  movdqa  xmm0, xmm3
456B82  pand    xmm0, xmm1
456B86  movdqa  xmm1, xmm3
456B8A  psubd   xmm1, xmm0
456B8E  pcmpgtd xmm2, xmm1
456B92  movdqa  xmm4, xmm2
456B96  pandn   xmm2, xmm3
456B9A  movdqa  xmm3, ds:xmmword_4E4330
456BA2  pcmpgtd xmm1, xmm3
456BA6  pand    xmm4, xmm0
456BAA  paddd   xmm0, ds:xmmword_4E4340
456BB2  movdqa  xmm3, xmm1
456BB6  por     xmm2, xmm4
456BBA  pandn   xmm3, xmm2
456BBE  pand    xmm1, xmm0
456BC2  por     xmm3, xmm1
456BC6  movdqu  xmmword ptr [ecx], xmm3
456BCA  jmp     short loc_456BD3
456BCC  movaps  xmm0, xmmword ptr [eax+10h]
456BD0  movups  xmmword ptr [ecx], xmm0
456BD3  mov     eax, [eax+4]
456BD6  add     ecx, 20h ; ' '
456BD9  dec     edi
456BDA  jnz     loc_456B51
456BE0  mov     ecx, [ebp+arg_8]
456BE3  mov     [ecx], edx
456BE5  mov     edx, [esp+50h+Block]
456BE9  push    edx; Block
456BEA  call    __aligned_free
456BEF  add     esp, 4
456BF2  xor     eax, eax
456BF4  pop     edi
456BF5  pop     esi
456BF6  pop     ebx
456BF7  mov     esp, ebp
456BF9  pop     ebp
456BFA  retn    14h

;==== 456C00 sub_456C00
456C00  push    ebp
456C01  mov     ebp, esp
456C03  and     esp, 0FFFFFFF0h
456C06  sub     esp, 24h
456C09  push    ebx
456C0A  mov     ebx, [ebp+arg_4]
456C0D  push    esi
456C0E  push    edi
456C0F  mov     edi, [ebp+arg_0]
456C12  mov     esi, [edi+10h]
456C15  mov     eax, 0A0000001h
456C1A  cmp     ecx, esi
456C1C  jnb     loc_456CAE
456C22  mov     edi, [edi+14h]
456C25  lea     ecx, [ecx+ecx*2]
456C28  shl     ecx, 5
456C2B  cmp     dword ptr [ecx+edi], 0
456C2F  jz      short loc_456CAE
456C31  cmp     edx, esi
456C33  jnb     short loc_456CA9
456C35  lea     eax, [edx+edx*2]
456C38  shl     eax, 5
456C3B  cmp     dword ptr [eax+edi], 0
456C3F  jz      short loc_456CA9
456C41  mov     [ebx], edx
456C43  stmxcsr [esp+30h+var_24]
456C48  mov     edx, [esp+30h+var_24]
456C4C  and     edx, 0F3FFh
456C52  mov     [esp+30h+var_24], edx
456C56  ldmxcsr [esp+30h+var_24]
456C5B  movaps  xmm0, xmmword ptr [eax+edi+30h]
456C60  subps   xmm0, xmmword ptr [ecx+edi+30h]
456C65  lea     eax, [esp+30h+var_20]
456C69  movaps  [esp+30h+var_20], xmm0
456C6E  call    sub_4558C0
456C73  mov     eax, 1
456C78  movaps  [esp+30h+var_10], xmm0
456C7D  test    eax, eax
456C7F  jz      short loc_456C9A
456C81  lea     eax, [esp+30h+var_10]
456C85  call    sub_455940
456C8A  movdqu  xmmword ptr [ebx+10h], xmm0
456C8F  xor     eax, eax
456C91  pop     edi
456C92  pop     esi
456C93  pop     ebx
456C94  mov     esp, ebp
456C96  pop     ebp
456C97  retn    8
456C9A  movups  xmmword ptr [ebx+10h], xmm0
456C9E  xor     eax, eax
456CA0  pop     edi
456CA1  pop     esi
456CA2  pop     ebx
456CA3  mov     esp, ebp
456CA5  pop     ebp
456CA6  retn    8
456CA9  mov     eax, 0A0000002h
456CAE  pop     edi
456CAF  pop     esi
456CB0  pop     ebx
456CB1  mov     esp, ebp
456CB3  pop     ebp
456CB4  retn    8

;==== 456CC0 sub_456CC0
456CC0  push    ebp
456CC1  mov     ebp, esp
456CC3  and     esp, 0FFFFFFF0h
456CC6  sub     esp, 28h
456CC9  push    esi
456CCA  mov     esi, [ebp+arg_0]
456CCD  push    edi
456CCE  mov     edi, 1
456CD3  mov     eax, 0A0000001h
456CD8  cmp     ecx, [edx+10h]
456CDB  jnb     loc_456D65
456CE1  lea     ecx, [ecx+ecx*2]
456CE4  shl     ecx, 5
456CE7  add     ecx, [edx+14h]
456CEA  cmp     dword ptr [ecx], 0
456CED  jz      short loc_456D65
456CEF  mov     dword ptr [esi], 0FFFFFFFFh
456CF5  stmxcsr [esp+30h+var_24]
456CFA  mov     eax, [esp+30h+var_24]
456CFE  and     eax, 0F3FFh
456D03  mov     [esp+30h+var_24], eax
456D07  ldmxcsr [esp+30h+var_24]
456D0C  movss   xmm0, ds:dword_4EC8A8
456D14  movss   xmm1, [ebp+arg_C]
456D19  movss   xmm2, [ebp+arg_8]
456D1E  movss   xmm3, [ebp+arg_4]
456D23  lea     eax, [esp+30h+var_20]
456D27  unpcklps xmm2, xmm0
456D2A  unpcklps xmm3, xmm1
456D2D  unpcklps xmm3, xmm2
456D30  subps   xmm3, xmmword ptr [ecx+30h]
456D34  movaps  [esp+30h+var_20], xmm3
456D39  call    sub_4558C0
456D3E  movaps  [esp+30h+var_10], xmm0
456D43  test    edi, edi
456D45  jz      short loc_456D5F
456D47  lea     eax, [esp+30h+var_10]
456D4B  call    sub_455940
456D50  movdqu  xmmword ptr [esi+10h], xmm0
456D55  xor     eax, eax
456D57  pop     edi
456D58  pop     esi
456D59  mov     esp, ebp
456D5B  pop     ebp
456D5C  retn    10h
456D5F  movups  xmmword ptr [esi+10h], xmm0
456D63  xor     eax, eax
456D65  pop     edi
456D66  pop     esi
456D67  mov     esp, ebp
456D69  pop     ebp
456D6A  retn    10h

;==== 456D70 sub_456D70
456D70  push    ebx
456D71  mov     ebx, esp
456D73  sub     esp, 8
456D76  and     esp, 0FFFFFFF0h
456D79  add     esp, 4
456D7C  push    ebp
456D7D  mov     ebp, [ebx+4]
456D80  mov     [esp+0Ch+var_8], ebp
456D84  mov     ebp, esp
456D86  sub     esp, 208h
456D8C  mov     eax, dword_4FB734
456D91  xor     eax, ebp
456D93  mov     [ebp-4], eax
456D96  movaps  xmm2, xmm0
456D99  cmpneqps xmm2, xmm1
456D9D  push    esi
456D9E  pmovmskb eax, xmm2
456DA2  push    edi
456DA3  movaps  xmmword ptr [ebp-1D0h], xmm1
456DAA  movaps  xmmword ptr [ebp-1C0h], xmm0
456DB1  test    eax, eax
456DB3  jz      loc_457198
456DB9  cmp     dword ptr [ebx+18h], 0
456DBD  jnz     short loc_456DC6
456DBF  mov     dword ptr [ebx+18h], 0FFh
456DC6  movss   xmm2, dword ptr [ebx+0Ch]
456DCB  movss   dword ptr [ebp-198h], xmm1
456DD3  fld     dword ptr [ebp-198h]
456DD9  movss   dword ptr [ebp-198h], xmm0
456DE1  fsub    dword ptr [ebp-198h]
456DE7  shufps  xmm2, xmm2, 0
456DEB  movaps  xmmword ptr [ebp-200h], xmm2
456DF2  fstp    dword ptr [ebp-198h]
456DF8  fld     dword ptr [ebp-1CCh]
456DFE  fsub    dword ptr [ebp-1BCh]
456E04  fstp    dword ptr [ebp-194h]
456E0A  fld     dword ptr [ebp-194h]
456E10  fld     dword ptr [ebp-198h]
456E16  call    sub_4D649A
456E1B  fstp    dword ptr [ebp-194h]
456E21  fld     dword ptr [ebp-194h]
456E27  fadd    ds:dbl_4EC988
456E2D  fstp    qword ptr [ebp-1E0h]
456E33  fld     dword ptr [ebp-1C8h]
456E39  fsub    dword ptr [ebp-1B8h]
456E3F  fstp    dword ptr [ebp-194h]
456E45  fld     dword ptr [ebp-194h]
456E4B  fld     dword ptr [ebp-198h]
456E51  call    sub_4D649A
456E56  fstp    dword ptr [ebp-194h]
456E5C  fld     dword ptr [ebp-194h]
456E62  lea     edi, [ebp-1F0h]
456E68  fadd    ds:dbl_4EC988
456E6E  xor     esi, esi
456E70  fstp    qword ptr [ebp-1D8h]
456E76  fldz
456E78  fstp    qword ptr [ebp-1F0h]
456E7E  fld     ds:dbl_4EC960
456E84  fstp    qword ptr [ebp-1E8h]
456E8A  lea     ebx, [ebx+0]
456E90  fldz
456E92  fld     qword ptr [ebp-1E0h]
456E98  fadd    qword ptr [edi]
456E9A  fst     qword ptr [ebp-1A8h]
456EA0  fxch    st(1)
456EA2  fst     dword ptr [ebp-194h]
456EA8  movss   xmm0, dword ptr [ebp-194h]
456EB0  fstp    dword ptr [ebp-194h]
456EB6  movss   dword ptr [ebp-1A0h], xmm0
456EBE  movss   xmm0, dword ptr [ebp-194h]
456EC6  movss   dword ptr [ebp-198h], xmm0
456ECE  call    __CIsin
456ED3  fstp    dword ptr [ebp-194h]
456ED9  fld     dword ptr [ebp-194h]
456EDF  fstp    dword ptr [ebp-194h]
456EE5  movss   xmm0, dword ptr [ebp-194h]
456EED  fld     qword ptr [ebp-1A8h]
456EF3  movss   dword ptr [ebp-19Ch], xmm0
456EFB  call    __CIcos
456F00  fstp    dword ptr [ebp-194h]
456F06  fld     dword ptr [ebp-194h]
456F0C  movss   xmm1, dword ptr [ebp-198h]
456F14  movss   xmm2, dword ptr [ebp-19Ch]
456F1C  fstp    dword ptr [ebp-194h]
456F22  movss   xmm0, dword ptr [ebp-194h]
456F2A  fld     qword ptr [ebp-1D8h]
456F30  fadd    qword ptr [edi]
456F32  fst     qword ptr [ebp-1A8h]
456F38  fldz
456F3A  unpcklps xmm0, xmm1
456F3D  movss   xmm1, dword ptr [ebp-1A0h]
456F45  unpcklps xmm2, xmm1
456F48  unpcklps xmm0, xmm2
456F4B  mulps   xmm0, xmmword ptr [ebp-200h]
456F52  movaps  xmm1, xmm0
456F55  addps   xmm1, xmmword ptr [ebp-1C0h]
456F5C  addps   xmm0, xmmword ptr [ebp-1D0h]
456F63  movaps  xmmword ptr [ebp+esi-190h], xmm0
456F6B  subps   xmm0, xmm1
456F6E  movaps  xmmword ptr [ebp+esi-0B0h], xmm1
456F76  movaps  xmmword ptr [ebp+esi-110h], xmm0
456F7E  mulps   xmm0, xmm0
456F81  movaps  xmm1, xmm0
456F84  shufps  xmm1, xmm0, 0FDh
456F88  addss   xmm0, xmm1
456F8C  movaps  xmmword ptr [ebp+esi-30h], xmm0
456F91  movaps  xmm0, xmmword ptr [ebp+esi-30h]
456F96  movaps  xmm1, xmm0
456F99  rcpss   xmm1, xmm1
456F9D  movaps  xmm2, xmm1
456FA0  mulss   xmm2, xmm1
456FA4  mulss   xmm0, xmm2
456FA8  addss   xmm1, xmm1
456FAC  subss   xmm1, xmm0
456FB0  movaps  xmmword ptr [ebp+esi-0D0h], xmm1
456FB8  fstp    dword ptr [ebp-19Ch]
456FBE  movss   xmm0, dword ptr [ebp-19Ch]
456FC6  movss   dword ptr [ebp-194h], xmm0
456FCE  call    __CIsin
456FD3  fstp    dword ptr [ebp-19Ch]
456FD9  fld     dword ptr [ebp-19Ch]
456FDF  fstp    dword ptr [ebp-19Ch]
456FE5  movss   xmm0, dword ptr [ebp-19Ch]
456FED  fldz
456FEF  fstp    dword ptr [ebp-19Ch]
456FF5  movss   dword ptr [ebp-1A0h], xmm0
456FFD  fld     qword ptr [ebp-1A8h]
457003  movss   xmm0, dword ptr [ebp-19Ch]
45700B  movss   dword ptr [ebp-198h], xmm0
457013  call    __CIcos
457018  fstp    dword ptr [ebp-19Ch]
45701E  fld     dword ptr [ebp-19Ch]
457024  movss   xmm1, dword ptr [ebp-1A0h]
45702C  movss   xmm2, dword ptr [ebp-198h]
457034  fstp    dword ptr [ebp-19Ch]
45703A  movss   xmm0, dword ptr [ebp-19Ch]
457042  movaps  xmm5, xmmword ptr [ebp-200h]
457049  unpcklps xmm0, xmm1
45704C  movss   xmm1, dword ptr [ebp-194h]
457054  unpcklps xmm2, xmm1
457057  unpcklps xmm0, xmm2
45705A  mulps   xmm0, xmm5
45705D  movaps  xmm1, xmm0
457060  addps   xmm1, xmmword ptr [ebp-1C0h]
457067  addps   xmm0, xmmword ptr [ebp-1D0h]
45706E  movaps  xmmword ptr [ebp+esi-170h], xmm0
457076  subps   xmm0, xmm1
457079  movaps  xmmword ptr [ebp+esi-0F0h], xmm1
457081  movaps  xmmword ptr [ebp+esi-150h], xmm0
457089  mulps   xmm0, xmm0
45708C  movaps  xmm1, xmm0
45708F  shufps  xmm1, xmm0, 0FEh
457093  addss   xmm0, xmm1
457097  movaps  xmmword ptr [ebp+esi-50h], xmm0
45709C  movaps  xmm0, xmmword ptr [ebp+esi-50h]
4570A1  movaps  xmm1, xmm0
4570A4  rcpss   xmm1, xmm1
4570A8  movaps  xmm2, xmm1
4570AB  mulss   xmm2, xmm1
4570AF  mulss   xmm0, xmm2
4570B3  addss   xmm1, xmm1
4570B7  subss   xmm1, xmm0
4570BB  movaps  xmmword ptr [ebp+esi-130h], xmm1
4570C3  add     esi, 10h
4570C6  add     edi, 8
4570C9  cmp     esi, 20h ; ' '
4570CC  jb      loc_456E90
4570D2  mov     ecx, [ebx+8]
4570D5  mov     esi, [ecx+10h]
4570D8  xor     eax, eax
4570DA  mov     [ebp-198h], eax
4570E0  mov     [ebp-19Ch], esi
4570E6  cmp     esi, eax
4570E8  jbe     loc_457189
4570EE  mov     edi, [ecx+14h]
4570F1  cmp     dword ptr [edi], 0
4570F4  jz      loc_457321
4570FA  cmp     eax, [ebx+14h]
4570FD  jz      loc_457321
457103  mov     cl, [ebx+18h]
457106  test    [edi+10h], cl
457109  jz      loc_457321
45710F  movaps  xmm4, xmmword ptr [edi+30h]
457113  movaps  xmm1, xmmword ptr [ebp-1C0h]
45711A  movaps  xmm0, xmmword ptr [ebp-1D0h]
457121  subps   xmm1, xmm4
457124  mulps   xmm1, xmm1
457127  movaps  xmm2, xmm1
45712A  shufps  xmm2, xmm1, 0FEh
45712E  movaps  xmm3, xmm1
457131  shufps  xmm3, xmm1, 0FDh
457135  addss   xmm3, xmm2
457139  subps   xmm0, xmm4
45713C  mulps   xmm0, xmm0
45713F  movaps  xmm6, xmm0
457142  movaps  xmm7, xmm0
457145  addss   xmm1, xmm3
457149  movss   xmm3, dword ptr [edi+4]
45714E  shufps  xmm6, xmm0, 0FEh
457152  shufps  xmm7, xmm0, 0FDh
457156  movaps  xmm2, xmm5
457159  addss   xmm2, xmm3
45715D  addss   xmm7, xmm6
457161  mulss   xmm2, xmm2
457165  addss   xmm0, xmm7
457169  comiss  xmm0, xmm2
45716C  jnb     short loc_4571AD
45716E  comiss  xmm1, xmm2
457171  jnb     short loc_45717F
457173  mov     edx, [ebx+10h]
457176  cmp     edx, [edi+14h]
457179  jg      loc_457321
45717F  mov     dword ptr [ebp-198h], 1
457189  mov     eax, [ebp-198h]
45718F  neg     eax
457191  sbb     eax, eax
457193  and     eax, 0A0000004h
457198  mov     ecx, [ebp-4]
45719B  pop     edi
45719C  xor     ecx, ebp
45719E  pop     esi
45719F  call    sub_4ABDD5
4571A4  mov     esp, ebp
4571A6  pop     ebp
4571A7  mov     esp, ebx
4571A9  pop     ebx
4571AA  retn    18h
4571AD  comiss  xmm1, xmm2
4571B0  jb      loc_45731B
4571B6  cmp     dword ptr [ebx+1Ch], 0
4571BA  jz      loc_45731B
4571C0  xor     edx, edx
4571C2  xor     esi, esi
4571C4  xor     ecx, ecx
4571C6  test    edx, edx
4571C8  jnz     short loc_45723A
4571CA  movaps  xmm1, xmmword ptr [ebp+ecx-110h]
4571D2  movaps  xmm0, xmm4
4571D5  subps   xmm0, xmmword ptr [ebp+ecx-0B0h]
4571DD  mulps   xmm1, xmm0
4571E0  movaps  xmm2, xmm1
4571E3  shufps  xmm2, xmm1, 0FDh
4571E7  addss   xmm1, xmm2
4571EB  xorps   xmm2, xmm2
4571EE  comiss  xmm1, xmm2
4571F1  jbe     short loc_45723A
4571F3  movaps  xmm2, xmmword ptr [ebp+ecx-30h]
4571F8  comiss  xmm2, xmm1
4571FB  jbe     short loc_45723A
4571FD  movaps  xmm6, xmm1
457200  mulss   xmm6, dword ptr [ebp+ecx-0D0h]
457209  mulss   xmm6, xmm1
45720D  mulps   xmm0, xmm0
457210  movaps  xmm1, xmm0
457213  shufps  xmm1, xmm0, 0FDh
457217  addss   xmm0, xmm1
45721B  movaps  xmm1, xmm3
45721E  subss   xmm0, xmm6
457222  mulss   xmm1, xmm3
457226  comiss  xmm0, xmm1
457229  jnb     short loc_457232
45722B  mov     edx, 1
457230  jmp     short loc_45723A
457232  movaps  xmmword ptr [ebp+ecx-90h], xmm0
45723A  test    esi, esi
45723C  jnz     short loc_4572AB
45723E  movaps  xmm1, xmmword ptr [ebp+ecx-150h]
457246  movaps  xmm0, xmm4
457249  subps   xmm0, xmmword ptr [ebp+ecx-0F0h]
457251  mulps   xmm1, xmm0
457254  movaps  xmm2, xmm1
457257  shufps  xmm2, xmm1, 0FEh
45725B  addss   xmm1, xmm2
45725F  xorps   xmm2, xmm2
457262  comiss  xmm1, xmm2
457265  jbe     short loc_4572AB
457267  movaps  xmm2, xmmword ptr [ebp+ecx-50h]
45726C  comiss  xmm2, xmm1
45726F  jbe     short loc_4572AB
457271  movaps  xmm6, xmm1
457274  mulss   xmm6, dword ptr [ebp+ecx-130h]
45727D  mulss   xmm6, xmm1
457281  mulps   xmm0, xmm0
457284  movaps  xmm1, xmm0
457287  shufps  xmm1, xmm0, 0FEh
45728B  addss   xmm0, xmm1
45728F  movaps  xmm1, xmm3
457292  subss   xmm0, xmm6
457296  mulss   xmm1, xmm3
45729A  comiss  xmm0, xmm1
45729D  jnb     short loc_4572A6
45729F  mov     esi, 1
4572A4  jmp     short loc_4572AB
4572A6  movaps  xmmword ptr [ebp+ecx-70h], xmm0
4572AB  test    edx, edx
4572AD  jz      short loc_4572B7
4572AF  test    esi, esi
4572B1  jnz     loc_45717F
4572B7  add     ecx, 10h
4572BA  cmp     ecx, 20h ; ' '
4572BD  jb      loc_4571C6
4572C3  movaps  xmm0, xmm5
4572C6  addss   xmm0, xmm5
4572CA  mulss   xmm0, xmm0
4572CE  test    edx, edx
4572D0  jnz     short loc_4572F0
4572D2  movaps  xmm1, xmmword ptr [ebp-90h]
4572D9  comiss  xmm1, xmm0
4572DC  jnb     short loc_4572EE
4572DE  movaps  xmm1, xmmword ptr [ebp-80h]
4572E2  comiss  xmm1, xmm0
4572E5  jnb     short loc_4572EE
4572E7  mov     edx, 1
4572EC  jmp     short loc_4572F0
4572EE  xor     edx, edx
4572F0  test    esi, esi
4572F2  jnz     short loc_45730F
4572F4  movaps  xmm1, xmmword ptr [ebp-70h]
4572F8  comiss  xmm1, xmm0
4572FB  jnb     short loc_45730D
4572FD  movaps  xmm1, xmmword ptr [ebp-60h]
457301  comiss  xmm1, xmm0
457304  jnb     short loc_45730D
457306  mov     esi, 1
45730B  jmp     short loc_45730F
45730D  xor     esi, esi
45730F  test    edx, edx
457311  jz      short loc_45731B
457313  test    esi, esi
457315  jnz     loc_45717F
45731B  mov     esi, [ebp-19Ch]
457321  inc     eax
457322  add     edi, 60h ; '`'
457325  cmp     eax, esi
457327  jb      loc_4570F1
45732D  jmp     loc_457189

;==== 457340 sub_457340
457340  push    ebp
457341  mov     ebp, esp
457343  movss   xmm3, [ebp+arg_8]
457348  xor     eax, eax
45734A  xor     ecx, ecx
45734C  push    esi
45734D  shufps  xmm3, xmm3, 0
457351  cmp     [edi+10h], eax
457354  jbe     short loc_4573CD
457356  mov     [ebp+arg_8], eax
457359  lea     esp, [esp+0]
457360  mov     esi, [ebp+arg_8]
457363  add     esi, [edi+14h]
457366  cmp     dword ptr [esi], 0
457369  jz      short loc_4573B7
45736B  cmp     eax, [ebp+arg_C]
45736E  jz      short loc_4573B7
457370  mov     dl, [ebp+arg_10]
457373  test    [esi+10h], dl
457376  jz      short loc_4573B7
457378  movss   xmm2, dword ptr [esi+4]
45737D  movaps  xmm4, xmm2
457380  movaps  xmm1, xmm0
457383  subps   xmm1, xmmword ptr [esi+30h]
457387  mulps   xmm1, xmm1
45738A  movaps  xmm2, xmm3
45738D  addss   xmm2, xmm4
457391  movaps  xmm4, xmm1
457394  movaps  xmm5, xmm1
457397  shufps  xmm4, xmm1, 0FEh
45739B  shufps  xmm5, xmm1, 0FDh
45739F  addss   xmm5, xmm4
4573A3  addss   xmm1, xmm5
4573A7  mulss   xmm2, xmm2
4573AB  comiss  xmm1, xmm2
4573AE  jnb     short loc_4573B7
4573B0  mov     edx, [ebp+arg_0]
4573B3  mov     [edx+ecx*4], eax
4573B6  inc     ecx
4573B7  add     [ebp+arg_8], 60h ; '`'
4573BB  inc     eax
4573BC  cmp     eax, [edi+10h]
4573BF  jb      short loc_457360
4573C1  mov     eax, [ebp+arg_4]
4573C4  mov     [eax], ecx
4573C6  xor     eax, eax
4573C8  pop     esi
4573C9  pop     ebp
4573CA  retn    14h
4573CD  mov     edx, [ebp+arg_4]
4573D0  mov     [edx], ecx
4573D2  xor     eax, eax
4573D4  pop     esi
4573D5  pop     ebp
4573D6  retn    14h

;==== 4573E0 sub_4573E0
4573E0  push    esi
4573E1  xor     esi, esi
4573E3  cmp     [edi+10h], esi
4573E6  jbe     short loc_4573F5
4573E8  push    esi
4573E9  push    edi
4573EA  call    sub_455C30
4573EF  inc     esi
4573F0  cmp     esi, [edi+10h]
4573F3  jb      short loc_4573E8
4573F5  pop     esi
4573F6  retn

;==== 457400 sub_457400
457400  push    ebx
457401  mov     ebx, esp
457403  sub     esp, 8
457406  and     esp, 0FFFFFFF0h
457409  add     esp, 4
45740C  push    ebp
45740D  mov     ebp, [ebx+4]
457410  mov     [esp+0Ch+var_8], ebp
457414  mov     ebp, esp
457416  sub     esp, 38h
457419  mov     ecx, [ebx+8]
45741C  push    esi
45741D  push    edi
45741E  mov     edi, eax
457420  xor     eax, eax
457422  movaps  xmmword ptr [ebp-20h], xmm1
457426  movaps  xmmword ptr [ebp-30h], xmm0
45742A  test    ecx, ecx
45742C  js      loc_45753C
457432  mov     esi, [edi+18h]
457435  cmp     ecx, esi
457437  jge     loc_45753C
45743D  mov     ecx, [ebx+0Ch]
457440  test    ecx, ecx
457442  js      loc_45753C
457448  cmp     ecx, [edi+1Ch]
45744B  jge     loc_45753C
457451  mov     eax, [edi+144h]
457457  subps   xmm1, xmmword ptr [eax]
45745A  lea     eax, [ebp-10h]
45745D  movaps  xmmword ptr [ebp-10h], xmm1
457461  call    sub_455860
457466  movss   xmm1, dword ptr [edi+13Ch]
45746E  comiss  xmm0, xmm1
457471  ja      loc_45753A
457477  imul    esi, [ebx+0Ch]
45747B  add     esi, [ebx+8]
45747E  shl     esi, 5
457481  add     esi, [edi+20h]
457484  fld     dword ptr [esi+4]
457487  fldz
457489  fucompp
45748B  fnstsw  ax
45748D  test    ah, 44h
457490  jnp     short loc_4574AE
457492  fld     dword ptr [ebx+14h]
457495  fadd    dword ptr [ebx+18h]
457498  fadd    dword ptr [edi+140h]
45749E  fld     dword ptr [esi+4]
4574A1  fcompp
4574A3  fnstsw  ax
4574A5  test    ah, 1
4574A8  jnz     loc_45753A
4574AE  mov     eax, [edi+15Ch]
4574B4  add     edi, 15Ch
4574BA  test    eax, eax
4574BC  jz      short loc_4574C8
4574BE  mov     edi, edi
4574C0  mov     edi, eax
4574C2  mov     eax, [eax]
4574C4  test    eax, eax
4574C6  jnz     short loc_4574C0
4574C8  push    10h; Alignment
4574CA  push    50h ; 'P'; Size
4574CC  call    __aligned_malloc
4574D1  push    50h ; 'P'; Size
4574D3  mov     esi, eax
4574D5  push    0; Val
4574D7  push    esi; void *
4574D8  call    _memset
4574DD  fld     dword ptr [ebx+14h]
4574E0  fadd    dword ptr [ebx+18h]
4574E3  mov     ecx, [ebx+10h]
4574E6  movaps  xmm0, xmmword ptr [ebp-20h]
4574EA  mov     eax, [ebx+0Ch]
4574ED  mov     edx, [ebx+8]
4574F0  fstp    dword ptr [esi+14h]
4574F3  mov     dword ptr [esi], 0
4574F9  mov     dword ptr [esi+4], 0
457500  mov     [esi+10h], ecx
457503  movss   xmm2, dword ptr [ebx+18h]
457508  movaps  xmm1, xmm0
45750B  shufps  xmm0, xmm2, 0Ah
45750F  shufps  xmm1, xmm0, 84h
457513  movaps  xmm0, xmmword ptr [ebp-30h]
457517  mov     [esi+44h], eax
45751A  movaps  xmmword ptr [esi+20h], xmm1
45751E  movaps  xmmword ptr [esi+30h], xmm0
457522  mov     [esi+40h], edx
457525  add     esp, 14h
457528  mov     [edi], esi
45752A  mov     eax, 1
45752F  pop     edi
457530  pop     esi
457531  mov     esp, ebp
457533  pop     ebp
457534  mov     esp, ebx
457536  pop     ebx
457537  retn    14h
45753A  xor     eax, eax
45753C  pop     edi
45753D  pop     esi
45753E  mov     esp, ebp
457540  pop     ebp
457541  mov     esp, ebx
457543  pop     ebx
457544  retn    14h

;==== 457550 sub_457550
457550  push    ebp
457551  mov     ebp, esp
457553  mov     ecx, [esi+160h]
457559  push    ebx
45755A  mov     ebx, [ebp+arg_0]
45755D  push    edi
45755E  xor     edi, edi
457560  test    ecx, ecx
457562  jz      short loc_45756C
457564  call    sub_446A10
457569  mov     [ebp+arg_0], eax
45756C  mov     eax, [esi+15Ch]
457572  test    eax, eax
457574  jz      short loc_4575A0
457576  mov     ecx, [eax]
457578  mov     [esi+15Ch], ecx
45757E  test    ebx, ebx
457580  jz      short loc_457592
457582  cmp     [eax+4], edi
457585  jnz     short loc_45758E
457587  mov     dword ptr [eax+4], 4
45758E  mov     [ebx], eax
457590  jmp     short loc_45759B
457592  push    eax; Block
457593  call    __aligned_free
457598  add     esp, 4
45759B  mov     edi, 1
4575A0  mov     eax, [esi+160h]
4575A6  test    eax, eax
4575A8  jz      short loc_4575B3
4575AA  mov     edx, [ebp+arg_0]
4575AD  push    edx
4575AE  call    sub_446A30
4575B3  mov     eax, edi
4575B5  pop     edi
4575B6  pop     ebx
4575B7  pop     ebp
4575B8  retn    4

;==== 4575C0 sub_4575C0
4575C0  push    ebp
4575C1  mov     ebp, esp
4575C3  mov     ecx, [edi+160h]
4575C9  push    esi
4575CA  xor     esi, esi
4575CC  test    ecx, ecx
4575CE  jz      short loc_4575D9
4575D0  call    sub_446A10
4575D5  mov     edx, eax
4575D7  jmp     short loc_4575DC
4575D9  mov     edx, [ebp+arg_0]
4575DC  mov     ecx, [edi+15Ch]
4575E2  test    ecx, ecx
4575E4  jz      short loc_457600
4575E6  cmp     [ecx+4], esi
4575E9  jz      short loc_4575F3
4575EB  mov     ecx, [ecx]
4575ED  test    ecx, ecx
4575EF  jnz     short loc_4575E6
4575F1  jmp     short loc_457600
4575F3  mov     eax, [ebp+arg_0]
4575F6  mov     esi, 1
4575FB  mov     [ecx+4], esi
4575FE  mov     [eax], ecx
457600  mov     eax, [edi+160h]
457606  test    eax, eax
457608  jz      short loc_457610
45760A  push    edx
45760B  call    sub_446A30
457610  mov     eax, esi
457612  pop     esi
457613  pop     ebp
457614  retn    4

;==== 457620 sub_457620
457620  push    ebx
457621  mov     ebx, esp
457623  sub     esp, 8
457626  and     esp, 0FFFFFFF0h
457629  add     esp, 4
45762C  push    ebp
45762D  mov     ebp, [ebx+4]
457630  mov     [esp+0Ch+var_8], ebp
457634  mov     ebp, esp
457636  sub     esp, 48h
457639  push    esi
45763A  mov     esi, [ebx+0Ch]
45763D  mov     eax, [esi+4]
457640  push    edi
457641  mov     dword ptr [ebp-4], 0FFFFFFFFh
457648  cmp     eax, 4
45764B  jnz     short loc_457692
45764D  mov     edi, [ebx+8]
457650  mov     eax, [edi+14Ch]
457656  fld     dword ptr [edi+130h]
45765C  mov     ecx, [edi+148h]
457662  mov     edx, [edi+138h]
457668  movaps  xmm1, xmmword ptr [esi+20h]
45766C  movaps  xmm0, xmmword ptr [esi+30h]
457670  push    eax
457671  mov     eax, [edi+134h]
457677  push    ecx
457678  push    edx
457679  push    eax
45767A  push    ecx
45767B  fstp    [esp+70h+var_70]
45767E  push    edi
45767F  call    sub_456D70
457684  neg     eax
457686  sbb     eax, eax
457688  neg     eax
45768A  add     eax, 2
45768D  mov     [esi+4], eax
457690  jmp     short loc_4576F8
457692  cmp     eax, 1
457695  jnz     short loc_4576F5
457697  mov     edi, [ebx+8]
45769A  lea     ecx, [ebp-10h]
45769D  push    ecx
45769E  call    sub_4575C0
4576A3  test    eax, eax
4576A5  jz      short loc_4576ED
4576A7  mov     edx, [edi+14Ch]
4576AD  fld     dword ptr [edi+130h]
4576B3  mov     eax, [edi+148h]
4576B9  mov     ecx, [edi+138h]
4576BF  push    edx
4576C0  mov     edx, [edi+134h]
4576C6  push    eax
4576C7  mov     eax, [ebp-10h]
4576CA  movaps  xmm1, xmmword ptr [eax+20h]
4576CE  movaps  xmm0, xmmword ptr [eax+30h]
4576D2  push    ecx
4576D3  push    edx
4576D4  push    ecx
4576D5  fstp    [esp+70h+var_70]
4576D8  push    edi
4576D9  call    sub_456D70
4576DE  mov     ecx, [ebp-10h]
4576E1  neg     eax
4576E3  sbb     eax, eax
4576E5  neg     eax
4576E7  add     eax, 2
4576EA  mov     [ecx+4], eax
4576ED  cmp     dword ptr [esi+4], 1
4576F1  jz      short loc_457697
4576F3  jmp     short loc_4576F8
4576F5  mov     edi, [ebx+8]
4576F8  cmp     dword ptr [esi+4], 2
4576FC  jnz     loc_4578D9
457702  mov     edx, [edi+18h]
457705  imul    edx, [esi+44h]
457709  add     edx, [esi+40h]
45770C  shl     edx, 5
45770F  add     edx, [edi+20h]
457712  fld     dword ptr [edx+4]
457715  fldz
457717  fucompp
457719  fnstsw  ax
45771B  test    ah, 44h
45771E  jnp     short loc_457739
457720  fld     dword ptr [edi+140h]
457726  fadd    dword ptr [esi+14h]
457729  fld     dword ptr [edx+4]
45772C  fcompp
45772E  fnstsw  ax
457730  test    ah, 1
457733  jnz     loc_4578D9
457739  add     esi, 10h
45773C  mov     ecx, 8
457741  mov     edi, edx
457743  rep movsd
457745  mov     esi, [ebx+8]
457748  mov     edi, [ebx+0Ch]
45774B  mov     dword ptr [ebp-8], 0
457752  lea     eax, [esi+34h]
457755  mov     ecx, [edi+40h]
457758  cmp     ecx, [eax-4]
45775B  jnz     short loc_457764
45775D  mov     ecx, [edi+44h]
457760  cmp     ecx, [eax]
457762  jz      short loc_457795
457764  mov     ecx, [ebp-8]
457767  inc     ecx
457768  add     eax, 10h
45776B  mov     [ebp-8], ecx
45776E  cmp     ecx, 10h
457771  jb      short loc_457755
457773  mov     edx, [edx]
457775  lea     eax, [edx+1]
457778  neg     eax
45777A  sbb     eax, eax
45777C  and     eax, 0FFFFFFF9h
45777F  add     eax, 10h
457782  mov     [ebp-18h], eax
457785  cmp     edx, 0FFFFFFFFh
457788  jz      short loc_4577A3
45778A  add     edx, 0FFFFFFFCh
45778D  and     edx, 0Fh
457790  mov     [ebp-14h], edx
457793  jmp     short loc_4577AA
457795  mov     eax, [ebp-8]
457798  pop     edi
457799  pop     esi
45779A  mov     esp, ebp
45779C  pop     ebp
45779D  mov     esp, ebx
45779F  pop     ebx
4577A0  retn    8
4577A3  mov     dword ptr [ebp-14h], 0
4577AA  mov     ecx, [esi+160h]
4577B0  mov     dword ptr [ebp-0Ch], 0
4577B7  test    ecx, ecx
4577B9  jz      short loc_4577C6
4577BB  call    sub_446A10
4577C0  mov     [ebp-20h], eax
4577C3  mov     eax, [ebp-18h]
4577C6  mov     dword ptr [ebp-8], 0
4577CD  test    eax, eax
4577CF  jz      loc_4578B2
4577D5  movss   xmm0, ds:dword_4EC974
4577DD  movaps  xmmword ptr [ebp-30h], xmm0
4577E1  movss   xmm0, ds:dword_4EC970
4577E9  movaps  xmmword ptr [ebp-40h], xmm0
4577ED  mov     eax, [ebp-14h]
4577F0  add     eax, [ebp-8]
4577F3  mov     edx, [esi+158h]
4577F9  movaps  xmm3, xmmword ptr [edi+20h]
4577FD  and     eax, 0Fh
457800  mov     ecx, eax
457802  add     ecx, ecx
457804  movaps  xmm0, xmmword ptr [edx+ecx*8]
457808  mulps   xmm0, xmm0
45780B  movaps  xmm1, xmm0
45780E  shufps  xmm1, xmm0, 0FEh
457812  movaps  xmm2, xmm0
457815  shufps  xmm2, xmm0, 0FDh
457819  addss   xmm2, xmm1
45781D  addss   xmm0, xmm2
457821  movaps  xmm1, xmm0
457824  rsqrtss xmm1, xmm1
457828  movaps  xmm2, xmm1
45782B  movaps  xmm4, xmm0
45782E  mulss   xmm4, xmm2
457832  mulss   xmm4, xmm2
457836  mulss   xmm4, xmm2
45783A  mulss   xmm4, dword ptr [ebp-30h]
45783F  mulss   xmm2, dword ptr [ebp-40h]
457844  addss   xmm2, xmm4
457848  mulss   xmm0, xmm2
45784C  sub     esp, 8
45784F  movss   dword ptr [ebp-1Ch], xmm0
457854  fld     dword ptr [ebp-1Ch]
457857  movaps  xmm1, xmm3
45785A  addps   xmm1, xmmword ptr [edx+ecx*8]
45785E  fstp    [esp+64h+var_60]
457862  fld     dword ptr [edi+14h]
457865  mov     edx, [esi+154h]
45786B  fstp    [esp+64h+var_64]
45786E  push    eax
45786F  mov     eax, [edx+ecx*8+4]
457873  add     eax, [edi+44h]
457876  mov     ecx, [edx+ecx*8]
457879  add     ecx, [edi+40h]
45787C  push    eax
45787D  push    ecx
45787E  mov     eax, esi
457880  movaps  xmm0, xmm3
457883  call    sub_457400
457888  mov     ecx, [esi+150h]
45788E  neg     eax
457890  sbb     eax, eax
457892  neg     eax
457894  add     [ebp-0Ch], eax
457897  mov     eax, [ebp-8]
45789A  add     ecx, 2
45789D  mov     edx, 10h
4578A2  sar     edx, cl
4578A4  add     eax, edx
4578A6  mov     [ebp-8], eax
4578A9  cmp     eax, [ebp-18h]
4578AC  jb      loc_4577ED
4578B2  mov     eax, [esi+160h]
4578B8  test    eax, eax
4578BA  jz      short loc_4578D9
4578BC  mov     ecx, [ebp-20h]
4578BF  push    ecx
4578C0  call    sub_446A30
4578C5  mov     eax, [ebp-0Ch]
4578C8  cmp     eax, 1
4578CB  jb      short loc_4578D9
4578CD  mov     edi, [esi+160h]
4578D3  push    eax
4578D4  call    sub_446B20
4578D9  mov     eax, [ebp-4]
4578DC  pop     edi
4578DD  pop     esi
4578DE  mov     esp, ebp
4578E0  pop     ebp
4578E1  mov     esp, ebx
4578E3  pop     ebx
4578E4  retn    8

;==== 4578F0 sub_4578F0
4578F0  push    ebp
4578F1  mov     ebp, esp
4578F3  and     esp, 0FFFFFFF8h
4578F6  push    ecx
4578F7  push    ebx
4578F8  mov     ebx, [ebp+arg_0]
4578FB  push    esi
4578FC  push    edi
4578FD  mov     edi, eax
4578FF  test    ebx, ebx
457901  jnz     short loc_45797D
457903  cmp     [edi+164h], ebx
457909  jz      short loc_45795D
45790B  jmp     short loc_457910
457910  lea     eax, [esp+10h+Block]
457914  push    eax
457915  mov     esi, edi
457917  call    sub_457550
45791C  test    eax, eax
45791E  jz      short loc_45794A
457920  mov     esi, [esp+10h+Block]
457924  push    esi
457925  push    edi
457926  call    sub_457620
45792B  test    eax, eax
45792D  js      short loc_45793F
45792F  mov     [edi+168h], eax
457935  mov     dword ptr [edi+164h], 0
45793F  push    esi; Block
457940  call    __aligned_free
457945  add     esp, 4
457948  jmp     short loc_457954
45794A  mov     dword ptr [edi+164h], 0
457954  cmp     dword ptr [edi+164h], 0
45795B  jnz     short loc_457910
45795D  mov     edi, [edi+160h]
457963  test    edi, edi
457965  jz      loc_4579FA
45796B  push    0
45796D  call    sub_446B20
457972  xor     eax, eax
457974  pop     edi
457975  pop     esi
457976  pop     ebx
457977  mov     esp, ebp
457979  pop     ebp
45797A  retn    4
45797D  cmp     dword ptr [edi+160h], 0
457984  jz      short loc_4579FA
457986  cmp     dword ptr [edi+164h], 0
45798D  jz      short loc_4579FA
45798F  nop
457990  lea     ecx, [esp+10h+Block]
457994  push    ecx
457995  call    sub_4575C0
45799A  test    eax, eax
45799C  jz      short loc_4579E4
45799E  mov     edx, [edi+14Ch]
4579A4  fld     dword ptr [edi+130h]
4579AA  mov     eax, [edi+148h]
4579B0  mov     ecx, [edi+138h]
4579B6  mov     esi, [esp+10h+Block]
4579BA  movaps  xmm1, xmmword ptr [esi+20h]
4579BE  movaps  xmm0, xmmword ptr [esi+30h]
4579C2  push    edx
4579C3  mov     edx, [edi+134h]
4579C9  push    eax
4579CA  push    ecx
4579CB  push    edx
4579CC  push    ecx
4579CD  fstp    [esp+24h+var_24]
4579D0  push    edi
4579D1  call    sub_456D70
4579D6  neg     eax
4579D8  sbb     eax, eax
4579DA  neg     eax
4579DC  add     eax, 2
4579DF  mov     [esi+4], eax
4579E2  jmp     short loc_4579F1
4579E4  mov     esi, [edi+160h]
4579EA  mov     eax, ebx
4579EC  call    sub_446A60
4579F1  cmp     dword ptr [edi+164h], 0
4579F8  jnz     short loc_457990
4579FA  pop     edi
4579FB  pop     esi
4579FC  xor     eax, eax
4579FE  pop     ebx
4579FF  mov     esp, ebp
457A01  pop     ebp
457A02  retn    4

;==== 457A10 sub_457A10
457A10  push    ebp
457A11  mov     ebp, esp
457A13  mov     eax, [ebp+arg_4]
457A16  push    eax
457A17  mov     eax, [ebp+arg_0]
457A1A  call    sub_4578F0
457A1F  pop     ebp
457A20  retn

;==== 457A30 sub_457A30
457A30  push    esi
457A31  push    edi
457A32  mov     esi, eax
457A34  xor     edi, edi
457A36  mov     dword ptr [ecx], offset ??_7DCTELogicalField@@6B@; const DCTELogicalField::`vftable'
457A3C  mov     [ecx+4], edi
457A3F  cmp     esi, 1
457A42  jnb     short loc_457A49
457A44  mov     esi, 1
457A49  mov     eax, 10000h
457A4E  xor     edx, edx
457A50  div     esi
457A52  mov     [ecx+14h], edi
457A55  mov     [ecx+18h], edi
457A58  mov     [ecx+40h], edi
457A5B  pop     edi
457A5C  pop     esi
457A5D  mov     [ecx+8], eax
457A60  lea     eax, [ecx+1Ch]
457A63  mov     [ecx+44h], eax
457A66  mov     eax, ecx
457A68  retn

;==== 457A70 sub_457A70
457A70  push    ebp
457A71  mov     ebp, esp
457A73  push    esi
457A74  mov     esi, ecx
457A76  call    sub_457AA0
457A7B  test    [ebp+arg_0], 1
457A7F  jz      short loc_457A8A
457A81  push    esi; void *
457A82  call    ??3@YAXPAX@Z; operator delete(void *)
457A87  add     esp, 4
457A8A  mov     eax, esi
457A8C  pop     esi
457A8D  pop     ebp
457A8E  retn    4

;==== 457AA0 sub_457AA0
457AA0  push    esi
457AA1  mov     esi, ecx
457AA3  push    edi
457AA4  mov     dword ptr [esi], offset ??_7DCTELogicalField@@6B@; const DCTELogicalField::`vftable'
457AAA  call    sub_458020
457AAF  mov     edi, esi
457AB1  call    sub_4581E0
457AB6  pop     edi
457AB7  pop     esi
457AB8  retn

;==== 457AC0 sub_457AC0
457AC0  push    ebp
457AC1  mov     ebp, esp
457AC3  push    ebx
457AC4  mov     ebx, [ebp+arg_0]
457AC7  push    esi
457AC8  mov     esi, ecx
457ACA  cmp     dword ptr [esi+4], 0
457ACE  push    edi
457ACF  mov     edi, eax
457AD1  jz      short loc_457ADF
457AD3  pop     edi
457AD4  pop     esi
457AD5  mov     eax, 0FFFFFFFDh
457ADA  pop     ebx
457ADB  pop     ebp
457ADC  retn    8
457ADF  test    edi, edi
457AE1  jz      short loc_457B2A
457AE3  test    ebx, ebx
457AE5  jz      short loc_457B2A
457AE7  call    sub_458020
457AEC  mov     [esi+0Ch], edi
457AEF  imul    edi, ebx
457AF2  xor     ecx, ecx
457AF4  mov     eax, edi
457AF6  mov     edx, 10h
457AFB  mul     edx
457AFD  seto    cl
457B00  mov     [esi+10h], ebx
457B03  neg     ecx
457B05  or      ecx, eax
457B07  push    ecx; Size
457B08  call    ??2@YAPAXI@Z; operator new(uint)
457B0D  mov     ecx, [ebp+Src]
457B10  shl     edi, 4
457B13  push    edi; Size
457B14  push    ecx; Src
457B15  push    eax; void *
457B16  mov     [esi+14h], eax
457B19  call    _memcpy_0
457B1E  add     esp, 10h
457B21  pop     edi
457B22  pop     esi
457B23  xor     eax, eax
457B25  pop     ebx
457B26  pop     ebp
457B27  retn    8
457B2A  pop     edi
457B2B  pop     esi
457B2C  mov     eax, 80000001h
457B31  pop     ebx
457B32  pop     ebp
457B33  retn    8

;==== 457B40 sub_457B40
457B40  push    ebp
457B41  mov     ebp, esp
457B43  sub     esp, 8
457B46  push    ebx
457B47  mov     ebx, [ebp+arg_0]
457B4A  push    esi
457B4B  push    edi
457B4C  mov     esi, ebx
457B4E  mov     edi, eax
457B50  call    sub_458050
457B55  test    eax, eax
457B57  jz      loc_457C02
457B5D  mov     eax, [edi]
457B5F  mov     ecx, eax
457B61  and     ecx, 1
457B64  mov     [ebp+var_8], ecx
457B67  mov     ecx, eax
457B69  and     ecx, 4
457B6C  mov     [ebp+var_4], ecx
457B6F  test    al, 2
457B71  jz      short loc_457B7C
457B73  mov     [ebp+arg_0], 0
457B7A  jmp     short loc_457B89
457B7C  mov     eax, [edi+4]
457B7F  not     eax
457B81  and     eax, 0FFh
457B86  mov     [ebp+arg_0], eax
457B89  mov     edi, ebx
457B8B  call    sub_4581E0
457B90  mov     eax, [ebp+arg_10]
457B93  cmp     eax, 1
457B96  jge     short loc_457B9D
457B98  mov     eax, 7FFFFFFFh
457B9D  mov     edi, [ebp+arg_4]
457BA0  push    eax
457BA1  mov     eax, [ebp+arg_8]
457BA4  push    0
457BA6  push    0
457BA8  push    0
457BAA  push    0
457BAC  push    0
457BAE  push    1
457BB0  push    eax
457BB1  call    sub_458120
457BB6  mov     esi, [ebx+40h]
457BB9  test    esi, esi
457BBB  jz      short loc_457BF0
457BBD  lea     ecx, [ecx+0]
457BC0  mov     ecx, [ebp+arg_0]
457BC3  mov     edx, [ebp+var_4]
457BC6  mov     eax, [ebp+var_8]
457BC9  push    ecx
457BCA  mov     ecx, [ebp+arg_C]
457BCD  push    edx
457BCE  push    eax
457BCF  push    ecx
457BD0  push    ebx
457BD1  call    sub_458210
457BD6  test    eax, eax
457BD8  jz      short loc_457BE9
457BDA  mov     edx, [ebp+arg_14]
457BDD  cmp     edx, [esi]
457BDF  jnz     short loc_457BE9
457BE1  mov     eax, [ebp+arg_18]
457BE4  cmp     eax, [esi+4]
457BE7  jz      short loc_457BF0
457BE9  mov     esi, [esi+24h]
457BEC  test    esi, esi
457BEE  jnz     short loc_457BC0
457BF0  mov     edi, ebx
457BF2  call    sub_4581E0
457BF7  pop     edi
457BF8  pop     esi
457BF9  xor     eax, eax
457BFB  pop     ebx
457BFC  mov     esp, ebp
457BFE  pop     ebp
457BFF  retn    1Ch
457C02  pop     edi
457C03  pop     esi
457C04  mov     eax, 80000002h
457C09  pop     ebx
457C0A  mov     esp, ebp
457C0C  pop     ebp
457C0D  retn    1Ch

;==== 457C10 sub_457C10
457C10  push    ebp
457C11  mov     ebp, esp
457C13  push    ecx
457C14  mov     ecx, [esi+18h]
457C17  push    edi
457C18  mov     edi, eax
457C1A  test    ecx, ecx
457C1C  jz      loc_457CF5
457C22  test    edx, edx
457C24  js      loc_457CE9
457C2A  mov     eax, [esi+0Ch]
457C2D  cmp     edx, eax
457C2F  jge     loc_457CE9
457C35  test    edi, edi
457C37  js      loc_457CE9
457C3D  cmp     edi, [esi+10h]
457C40  jge     loc_457CE9
457C46  imul    eax, edi
457C49  add     eax, edx
457C4B  lea     eax, [eax+eax*2]
457C4E  cmp     dword ptr [ecx+eax*8+4], 0
457C53  lea     eax, [ecx+eax*8]
457C56  jz      loc_457CDD
457C5C  mov     ecx, [ebp+arg_4]
457C5F  push    ebx
457C60  mov     ebx, [eax+8]
457C63  xor     eax, eax
457C65  mov     [ecx], ebx
457C67  cmp     ebx, 1
457C6A  jl      short loc_457CC8
457C6C  cmp     [ebp+arg_0], eax
457C6F  jz      short loc_457CC8
457C71  mov     [ebp+var_4], edx
457C74  mov     [ebp+arg_4], edi
457C77  test    ebx, ebx
457C79  jz      short loc_457CC8
457C7B  jmp     short loc_457C80
457C80  mov     eax, [esi+0Ch]
457C83  imul    eax, [ebp+arg_4]
457C87  add     eax, [ebp+var_4]
457C8A  push    0
457C8C  lea     edx, [eax+eax*2]
457C8F  mov     eax, [esi+18h]
457C92  lea     edi, [eax+edx*8]
457C95  mov     edx, [esi]
457C97  mov     eax, [edi+4]
457C9A  mov     edx, [edx+10h]
457C9D  push    eax
457C9E  mov     ecx, esi
457CA0  dec     ebx
457CA1  call    edx
457CA3  mov     ecx, [ebp+arg_0]
457CA6  mov     [ecx+ebx*4], eax
457CA9  mov     eax, [edi+4]
457CAC  mov     edx, [esi]
457CAE  mov     edx, [edx+0Ch]
457CB1  push    eax
457CB2  lea     ecx, [ebp+arg_4]
457CB5  push    ecx
457CB6  lea     eax, [ebp+var_4]
457CB9  push    eax
457CBA  mov     ecx, esi
457CBC  call    edx
457CBE  test    eax, eax
457CC0  jz      short loc_457CD0
457CC2  test    ebx, ebx
457CC4  jnz     short loc_457C80
457CC6  xor     eax, eax
457CC8  pop     ebx
457CC9  pop     edi
457CCA  mov     esp, ebp
457CCC  pop     ebp
457CCD  retn    8
457CD0  pop     ebx
457CD1  mov     eax, 0FFFFFFFEh
457CD6  pop     edi
457CD7  mov     esp, ebp
457CD9  pop     ebp
457CDA  retn    8
457CDD  mov     eax, 80000004h
457CE2  pop     edi
457CE3  mov     esp, ebp
457CE5  pop     ebp
457CE6  retn    8
457CE9  mov     eax, 80000005h
457CEE  pop     edi
457CEF  mov     esp, ebp
457CF1  pop     ebp
457CF2  retn    8
457CF5  mov     eax, 80000003h
457CFA  pop     edi
457CFB  mov     esp, ebp
457CFD  pop     ebp
457CFE  retn    8

;==== 457D10 sub_457D10
457D10  push    ebp
457D11  mov     ebp, esp
457D13  push    ecx
457D14  push    ebx
457D15  mov     ebx, [ebp+arg_4]
457D18  push    esi
457D19  xor     eax, eax
457D1B  xor     esi, esi
457D1D  mov     [ebp+arg_4], ecx
457D20  mov     [ebp+var_4], edx
457D23  test    edi, edi
457D25  jle     short loc_457D65
457D27  mov     edx, [ebp+arg_8]
457D2A  mov     edx, [edx+esi*4]
457D2D  mov     ecx, [ebp+arg_0]
457D30  mov     eax, [ecx]
457D32  mov     eax, [eax+0Ch]
457D35  push    edx
457D36  lea     edx, [ebp+var_4]
457D39  push    edx
457D3A  lea     edx, [ebp+arg_4]
457D3D  push    edx
457D3E  call    eax
457D40  test    eax, eax
457D42  jz      short loc_457D60
457D44  mov     ecx, [ebp+arg_4]
457D47  mov     edx, [ebp+var_4]
457D4A  mov     [ebx+esi*8], ecx
457D4D  mov     [ebx+esi*8+4], edx
457D51  inc     esi
457D52  cmp     esi, edi
457D54  jl      short loc_457D27
457D56  pop     esi
457D57  xor     eax, eax
457D59  pop     ebx
457D5A  mov     esp, ebp
457D5C  pop     ebp
457D5D  retn    0Ch
457D60  mov     eax, 80000007h
457D65  pop     esi
457D66  pop     ebx
457D67  mov     esp, ebp
457D69  pop     ebp
457D6A  retn    0Ch

;==== 457D70 sub_457D70
457D70  push    ebp
457D71  mov     ebp, esp
457D73  mov     edx, [ecx+18h]
457D76  test    edx, edx
457D78  jz      short loc_457DCA
457D7A  test    esi, esi
457D7C  js      short loc_457DC1
457D7E  mov     eax, [ecx+0Ch]
457D81  cmp     esi, eax
457D83  jge     short loc_457DC1
457D85  test    edi, edi
457D87  js      short loc_457DC1
457D89  cmp     edi, [ecx+10h]
457D8C  jge     short loc_457DC1
457D8E  imul    eax, edi
457D91  add     eax, esi
457D93  lea     eax, [eax+eax*2]
457D96  cmp     dword ptr [edx+eax*8+4], 0
457D9B  lea     eax, [edx+eax*8]
457D9E  jz      short loc_457DB8
457DA0  mov     edx, [ecx]
457DA2  mov     eax, [eax+4]
457DA5  mov     edx, [edx+10h]
457DA8  push    0
457DAA  push    eax
457DAB  call    edx
457DAD  mov     ecx, [ebp+arg_0]
457DB0  mov     [ecx], eax
457DB2  xor     eax, eax
457DB4  pop     ebp
457DB5  retn    4
457DB8  mov     eax, 80000004h
457DBD  pop     ebp
457DBE  retn    4
457DC1  mov     eax, 80000005h
457DC6  pop     ebp
457DC7  retn    4
457DCA  mov     eax, 80000003h
457DCF  pop     ebp
457DD0  retn    4

;==== 457DE0 sub_457DE0
457DE0  push    ebp
457DE1  mov     ebp, esp
457DE3  mov     ecx, [esi+18h]
457DE6  test    ecx, ecx
457DE8  jz      short loc_457E30
457DEA  test    edx, edx
457DEC  js      short loc_457E27
457DEE  mov     eax, [esi+0Ch]
457DF1  cmp     edx, eax
457DF3  jge     short loc_457E27
457DF5  test    edi, edi
457DF7  js      short loc_457E27
457DF9  cmp     edi, [esi+10h]
457DFC  jge     short loc_457E27
457DFE  imul    eax, edi
457E01  add     eax, edx
457E03  lea     eax, [eax+eax*2]
457E06  cmp     dword ptr [ecx+eax*8+4], 0
457E0B  lea     eax, [ecx+eax*8]
457E0E  jz      short loc_457E1E
457E10  mov     ecx, [eax+0Ch]
457E13  mov     edx, [ebp+arg_0]
457E16  mov     [edx], ecx
457E18  xor     eax, eax
457E1A  pop     ebp
457E1B  retn    4
457E1E  mov     eax, 80000004h
457E23  pop     ebp
457E24  retn    4
457E27  mov     eax, 80000005h
457E2C  pop     ebp
457E2D  retn    4
457E30  mov     eax, 80000003h
457E35  pop     ebp
457E36  retn    4

;==== 457E40 sub_457E40
457E40  push    ebp
457E41  mov     ebp, esp
457E43  mov     edx, [ecx+18h]
457E46  test    edx, edx
457E48  jz      short loc_457E6E
457E4A  mov     eax, [ecx+10h]
457E4D  imul    eax, [ecx+0Ch]
457E51  mov     ecx, [ebp+arg_0]
457E54  lea     eax, [eax+eax*2]
457E57  add     eax, eax
457E59  add     eax, eax
457E5B  add     eax, eax
457E5D  push    eax; Size
457E5E  push    edx; Src
457E5F  push    ecx; void *
457E60  call    _memcpy_0
457E65  add     esp, 0Ch
457E68  xor     eax, eax
457E6A  pop     ebp
457E6B  retn    4
457E6E  mov     eax, 80000003h
457E73  pop     ebp
457E74  retn    4

;==== 457E80 sub_457E80
457E80  push    ebp
457E81  mov     ebp, esp
457E83  sub     esp, 10h
457E86  push    esi
457E87  mov     esi, eax
457E89  or      eax, 0FFFFFFFFh
457E8C  cmp     dword ptr [esi+18h], 0
457E90  jz      loc_45800A
457E96  mov     eax, [esi+0Ch]
457E99  imul    eax, [esi+10h]
457E9D  xor     ecx, ecx
457E9F  mov     edx, 20h ; ' '
457EA4  mul     edx
457EA6  seto    cl
457EA9  push    ebx
457EAA  push    edi
457EAB  neg     ecx
457EAD  or      ecx, eax
457EAF  push    ecx; Size
457EB0  call    ??2@YAPAXI@Z; operator new(uint)
457EB5  mov     [ebp+var_10], eax
457EB8  mov     eax, [esi+0Ch]
457EBB  imul    eax, [esi+10h]
457EBF  xor     ecx, ecx
457EC1  mov     edx, 4
457EC6  mul     edx
457EC8  seto    cl
457ECB  neg     ecx
457ECD  or      ecx, eax
457ECF  push    ecx; Size
457ED0  call    ??2@YAPAXI@Z; operator new(uint)
457ED5  xor     ebx, ebx
457ED7  xor     edi, edi
457ED9  add     esp, 8
457EDC  mov     [ebp+Base], eax
457EDF  mov     [ebp+var_4], edi
457EE2  mov     [ebp+var_8], ebx
457EE5  cmp     [esi+10h], ebx
457EE8  jle     loc_457FAC
457EEE  mov     edi, edi
457EF0  mov     ecx, [esi+0Ch]
457EF3  xor     edx, edx
457EF5  test    ecx, ecx
457EF7  jle     loc_457F9F
457EFD  mov     eax, edi
457EFF  shl     eax, 5
457F02  add     eax, [ebp+var_10]
457F05  imul    ecx, ebx
457F08  mov     ebx, [esi+18h]
457F0B  add     ecx, edx
457F0D  lea     edi, [ecx+ecx*2]
457F10  cmp     dword ptr [ebx+edi*8+0Ch], 0
457F15  jle     short loc_457F8D
457F17  mov     edi, [esi+14h]
457F1A  add     ecx, ecx
457F1C  test    byte ptr [edi+ecx*8+8], 0Dh
457F21  jnz     short loc_457F8D
457F23  cmp     [ebp+arg_8], 0
457F27  mov     ebx, [ebp+var_8]
457F2A  jz      short loc_457F77
457F2C  mov     [eax], edx
457F2E  mov     [eax+4], ebx
457F31  mov     ecx, [esi+0Ch]
457F34  mov     edi, [esi+18h]
457F37  imul    ecx, ebx
457F3A  add     ecx, edx
457F3C  lea     ecx, [ecx+ecx*2]
457F3F  lea     ecx, [edi+ecx*8]
457F42  mov     edi, [ecx]
457F44  mov     [eax+8], edi
457F47  mov     edi, [ecx+4]
457F4A  mov     [eax+0Ch], edi
457F4D  mov     edi, [ecx+8]
457F50  mov     [eax+10h], edi
457F53  mov     edi, [ecx+0Ch]
457F56  mov     [eax+14h], edi
457F59  mov     edi, [ecx+10h]
457F5C  mov     [eax+18h], edi
457F5F  mov     ecx, [ecx+14h]
457F62  mov     edi, [ebp+var_4]
457F65  mov     [eax+1Ch], ecx
457F68  mov     ecx, [ebp+Base]
457F6B  mov     [ecx+edi*4], eax
457F6E  inc     edi
457F6F  mov     [ebp+var_4], edi
457F72  add     eax, 20h ; ' '
457F75  jmp     short loc_457F93
457F77  mov     edi, [ebp+var_4]
457F7A  mov     ecx, [ebp+arg_0]
457F7D  mov     [ecx+edi*8], edx
457F80  mov     [ecx+edi*8+4], ebx
457F84  inc     edi
457F85  mov     [ebp+var_4], edi
457F88  add     eax, 20h ; ' '
457F8B  jmp     short loc_457F93
457F8D  mov     ebx, [ebp+var_8]
457F90  mov     edi, [ebp+var_4]
457F93  mov     ecx, [esi+0Ch]
457F96  inc     edx
457F97  cmp     edx, ecx
457F99  jl      loc_457F05
457F9F  inc     ebx
457FA0  mov     [ebp+var_8], ebx
457FA3  cmp     ebx, [esi+10h]
457FA6  jl      loc_457EF0
457FAC  cmp     [ebp+arg_8], 0
457FB0  jz      short loc_457FEC
457FB2  cmp     edi, 1
457FB5  jb      short loc_457FEC
457FB7  mov     edx, [ebp+Base]
457FBA  push    offset sub_4583C0; CompareFunction
457FBF  push    4; SizeOfElements
457FC1  push    edi; NumOfElements
457FC2  push    edx; Base
457FC3  call    _qsort
457FC8  add     esp, 10h
457FCB  xor     eax, eax
457FCD  test    edi, edi
457FCF  jz      short loc_457FEC
457FD1  mov     ecx, [ebp+arg_0]
457FD4  mov     edx, [ebp+Base]
457FD7  mov     edx, [edx+eax*4]
457FDA  mov     esi, [edx]
457FDC  mov     [ecx], esi
457FDE  mov     edx, [edx+4]
457FE1  mov     [ecx+4], edx
457FE4  inc     eax
457FE5  add     ecx, 8
457FE8  cmp     eax, edi
457FEA  jb      short loc_457FD4
457FEC  mov     ecx, [ebp+var_10]
457FEF  mov     eax, [ebp+arg_4]
457FF2  push    ecx; void *
457FF3  mov     [eax], edi
457FF5  call    ??3@YAXPAX@Z; operator delete(void *)
457FFA  mov     edx, [ebp+Base]
457FFD  push    edx; void *
457FFE  call    ??3@YAXPAX@Z; operator delete(void *)
458003  add     esp, 8
458006  pop     edi
458007  xor     eax, eax
458009  pop     ebx
45800A  pop     esi
45800B  mov     esp, ebp
45800D  pop     ebp
45800E  retn    0Ch

;==== 458020 sub_458020
458020  cmp     dword ptr [esi+4], 0
458024  jnz     short loc_458039
458026  mov     eax, [esi+14h]
458029  push    eax; void *
45802A  call    ??3@YAXPAX@Z; operator delete(void *)
45802F  add     esp, 4
458032  mov     dword ptr [esi+14h], 0
458039  mov     ecx, [esi+18h]
45803C  push    ecx; void *
45803D  call    ??3@YAXPAX@Z; operator delete(void *)
458042  add     esp, 4
458045  mov     dword ptr [esi+18h], 0
45804C  retn

;==== 458050 sub_458050
458050  xor     eax, eax
458052  cmp     [esi+14h], eax
458055  jz      short locret_4580A3
458057  cmp     [esi+18h], eax
45805A  jnz     short loc_45807F
45805C  mov     eax, [esi+10h]
45805F  imul    eax, [esi+0Ch]
458063  xor     ecx, ecx
458065  mov     edx, 18h
45806A  mul     edx
45806C  seto    cl
45806F  neg     ecx
458071  or      ecx, eax
458073  push    ecx; Size
458074  call    ??2@YAPAXI@Z; operator new(uint)
458079  add     esp, 4
45807C  mov     [esi+18h], eax
45807F  mov     eax, [esi+10h]
458082  imul    eax, [esi+0Ch]
458086  mov     ecx, [esi+18h]
458089  lea     eax, [eax+eax*2]
45808C  add     eax, eax
45808E  add     eax, eax
458090  add     eax, eax
458092  push    eax; Size
458093  push    0; Val
458095  push    ecx; void *
458096  call    _memset
45809B  add     esp, 0Ch
45809E  mov     eax, 1
4580A3  retn

;==== 4580B0 sub_4580B0
4580B0  push    ebp
4580B1  mov     ebp, esp
4580B3  push    ebx
4580B4  mov     ebx, [ebp+arg_0]
4580B7  mov     edx, [ebx+18h]
4580BA  test    edx, edx
4580BC  jz      short loc_458110
4580BE  test    esi, esi
4580C0  js      short loc_458106
4580C2  mov     eax, [ebx+0Ch]
4580C5  cmp     esi, eax
4580C7  jge     short loc_458106
4580C9  test    edi, edi
4580CB  js      short loc_458106
4580CD  cmp     edi, [ebx+10h]
4580D0  jge     short loc_458106
4580D2  imul    eax, edi
4580D5  add     eax, esi
4580D7  lea     eax, [eax+eax*2]
4580DA  lea     eax, [edx+eax*8]
4580DD  mov     edx, [eax]
4580DF  mov     [ecx], edx
4580E1  mov     edx, [eax+4]
4580E4  mov     [ecx+4], edx
4580E7  mov     edx, [eax+8]
4580EA  mov     [ecx+8], edx
4580ED  mov     edx, [eax+0Ch]
4580F0  mov     [ecx+0Ch], edx
4580F3  mov     edx, [eax+10h]
4580F6  mov     eax, [eax+14h]
4580F9  mov     [ecx+10h], edx
4580FC  mov     [ecx+14h], eax
4580FF  xor     eax, eax
458101  pop     ebx
458102  pop     ebp
458103  retn    4
458106  mov     eax, 80000005h
45810B  pop     ebx
45810C  pop     ebp
45810D  retn    4
458110  mov     eax, 80000003h
458115  pop     ebx
458116  pop     ebp
458117  retn    4

;==== 458120 sub_458120
458120  push    ebp
458121  mov     ebp, esp
458123  push    ebx
458124  mov     ebx, [ebp+arg_0]
458127  test    edi, edi
458129  js      loc_4581D3
45812F  mov     ecx, [esi+0Ch]
458132  cmp     ecx, edi
458134  jle     loc_4581D3
45813A  test    ebx, ebx
45813C  js      loc_4581D3
458142  cmp     [esi+10h], ebx
458145  jle     loc_4581D3
45814B  imul    ecx, ebx
45814E  add     ecx, edi
458150  lea     eax, [ecx+ecx*2]
458153  mov     ecx, [esi+18h]
458156  cmp     dword ptr [ecx+eax*8], 0
45815A  lea     ecx, [ecx+eax*8]
45815D  mov     [ebp+arg_0], 80000006h
458164  jz      short loc_45817B
458166  mov     eax, [ebp+arg_10]
458169  mov     edx, [ecx+0Ch]
45816C  inc     eax
45816D  cmp     eax, edx
45816F  jl      short loc_45817B
458171  jnz     short loc_4581CB
458173  mov     edx, [ebp+arg_14]
458176  cmp     edx, [ecx+10h]
458179  jge     short loc_4581CB
45817B  push    28h ; '('; Size
45817D  call    ??2@YAPAXI@Z; operator new(uint)
458182  mov     ecx, [ebp+arg_4]
458185  mov     edx, [ebp+arg_8]
458188  mov     [eax+8], ecx
45818B  mov     ecx, [ebp+arg_C]
45818E  mov     [eax+0Ch], edx
458191  mov     edx, [ebp+arg_10]
458194  mov     [eax+10h], ecx
458197  mov     ecx, [ebp+arg_14]
45819A  mov     [eax+14h], edx
45819D  mov     edx, [ebp+arg_18]
4581A0  mov     [eax+18h], ecx
4581A3  mov     ecx, [ebp+arg_1C]
4581A6  mov     [eax+4], ebx
4581A9  mov     [eax], edi
4581AB  mov     [eax+1Ch], edx
4581AE  mov     [eax+20h], ecx
4581B1  mov     dword ptr [eax+24h], 0
4581B8  mov     edx, [esi+44h]
4581BB  add     esp, 4
4581BE  mov     [edx+24h], eax
4581C1  mov     [esi+44h], eax
4581C4  xor     eax, eax
4581C6  pop     ebx
4581C7  pop     ebp
4581C8  retn    20h ; ' '
4581CB  mov     eax, [ebp+arg_0]
4581CE  pop     ebx
4581CF  pop     ebp
4581D0  retn    20h ; ' '
4581D3  mov     eax, 80000005h
4581D8  pop     ebx
4581D9  pop     ebp
4581DA  retn    20h ; ' '

;==== 4581E0 sub_4581E0
4581E0  push    esi
4581E1  mov     esi, [edi+40h]
4581E4  test    esi, esi
4581E6  jz      short loc_458200
4581E8  jmp     short loc_4581F0
4581F0  push    esi; void *
4581F1  mov     esi, [esi+24h]
4581F4  call    ??3@YAXPAX@Z; operator delete(void *)
4581F9  add     esp, 4
4581FC  test    esi, esi
4581FE  jnz     short loc_4581F0
458200  lea     eax, [edi+1Ch]
458203  mov     dword ptr [edi+40h], 0
45820A  mov     [edi+44h], eax
45820D  pop     esi
45820E  retn

;==== 458210 sub_458210
458210  push    ebp
458211  mov     ebp, esp
458213  sub     esp, 10h
458216  mov     edx, [ebp+arg_0]
458219  mov     eax, [edx+0Ch]
45821C  imul    eax, [esi+4]
458220  add     eax, [esi]
458222  push    ebx
458223  push    edi
458224  mov     edi, [edx+18h]
458227  lea     ecx, [eax+eax*2]
45822A  cmp     dword ptr [edi+ecx*8], 0
45822E  lea     edi, [edi+ecx*8]
458231  mov     [ebp+var_C], 0
458238  jz      short loc_458267
45823A  mov     ecx, [esi+14h]
45823D  mov     ebx, [edi+0Ch]
458240  inc     ecx
458241  cmp     ecx, ebx
458243  jl      short loc_458267
458245  jnz     loc_4583B1
45824B  mov     ecx, [esi+18h]
45824E  mov     ebx, [edi+10h]
458251  cmp     ecx, ebx
458253  jl      short loc_458267
458255  jnz     loc_4583B1
45825B  mov     ecx, [esi+1Ch]
45825E  cmp     ecx, [edi+14h]
458261  jge     loc_4583B1
458267  shl     eax, 4
45826A  add     eax, [edx+14h]
45826D  xor     ebx, ebx
45826F  cmp     dword ptr [esi+8], 1
458273  mov     ecx, eax
458275  mov     eax, [ecx+8]
458278  setz    bl
45827B  mov     [ebp+var_10], ecx
45827E  mov     [ebp+var_8], ebx
458281  test    al, 2
458283  jnz     loc_4583AB
458289  cmp     [ebp+arg_8], 0
45828D  jnz     short loc_4582A3
45828F  test    al, 0Ch
458291  jnz     loc_4583AB
458297  test    ebx, ebx
458299  jnz     short loc_4582A3
45829B  test    al, 1
45829D  jnz     loc_4583AB
4582A3  mov     eax, [ecx+0Ch]
4582A6  shr     eax, 1Ch
4582A9  add     eax, [ecx]
4582AB  mov     edx, eax
4582AD  test    ebx, ebx
4582AF  jnz     short loc_4582B4
4582B1  mov     edx, [esi+0Ch]
4582B4  cmp     [ebp+arg_8], 0
4582B8  jz      short loc_4582C5
4582BA  cmp     edx, eax
4582BC  jl      short loc_4582C5
4582BE  mov     eax, edx
4582C0  mov     [ebp+var_4], edx
4582C3  jmp     short loc_4582C8
4582C5  mov     [ebp+var_4], eax
4582C8  sub     eax, edx
4582CA  cdq
4582CB  xor     eax, edx
4582CD  sub     eax, edx
4582CF  cmp     [ebp+arg_4], eax
4582D2  jge     short loc_4582DC
4582D4  test    ebx, ebx
4582D6  jz      loc_4583B1
4582DC  xor     ebx, ebx
4582DE  mov     [ebp+arg_4], ebx
4582E1  cmp     [ebp+var_8], ebx
4582E4  jnz     short loc_458310
4582E6  mov     ebx, [ecx+4]
4582E9  inc     ebx
4582EA  cmp     [ebp+arg_C], 0
4582EE  mov     [ebp+arg_4], ebx
4582F1  jnz     short loc_458310
4582F3  cmp     [ebp+arg_8], 0
4582F7  jz      short loc_458304
4582F9  cmp     eax, 4
4582FC  jl      short loc_458310
4582FE  lea     ebx, [ebx+eax-3]
458302  jmp     short loc_45830D
458304  cmp     eax, 2
458307  jl      short loc_458310
458309  lea     ebx, [ebx+eax-1]
45830D  mov     [ebp+arg_4], ebx
458310  mov     edx, [esi+14h]
458313  add     edx, ebx
458315  mov     ebx, [esi+18h]
458318  add     ebx, eax
45831A  cmp     dword ptr [edi], 0
45831D  jz      short loc_45833D
45831F  mov     eax, [edi+0Ch]
458322  cmp     edx, eax
458324  jl      short loc_45833D
458326  jnz     loc_4583B1
45832C  mov     eax, [edi+10h]
45832F  cmp     ebx, eax
458331  jl      short loc_45833D
458333  jnz     short loc_4583B1
458335  mov     eax, [esi+1Ch]
458338  cmp     eax, [edi+14h]
45833B  jge     short loc_4583B1
45833D  mov     eax, [esi+20h]
458340  sub     eax, [ebp+arg_4]
458343  js      short loc_4583B1
458345  mov     dword ptr [edi], 1
45834B  mov     ecx, [esi+8]
45834E  mov     [edi+4], ecx
458351  mov     ecx, [esi+10h]
458354  mov     [edi+8], ecx
458357  mov     [edi+0Ch], edx
45835A  mov     [edi+10h], ebx
45835D  mov     ecx, [esi+1Ch]
458360  mov     [edi+14h], ecx
458363  cmp     eax, 1
458366  jl      short loc_458373
458368  mov     ecx, [ebp+arg_10]
45836B  mov     edi, [ebp+var_10]
45836E  test    [edi+0Ch], ecx
458371  jz      short loc_458379
458373  cmp     [ebp+var_8], 0
458377  jz      short loc_458399
458379  mov     ecx, [ebp+arg_0]
45837C  mov     edi, [ecx]
45837E  push    eax
45837F  mov     eax, [esi+1Ch]
458382  push    eax
458383  mov     eax, [ebp+var_4]
458386  push    ebx
458387  push    edx
458388  mov     edx, [esi+10h]
45838B  push    edx
45838C  mov     edx, [esi+4]
45838F  push    eax
458390  mov     eax, [esi]
458392  push    edx
458393  mov     edx, [edi+8]
458396  push    eax
458397  call    edx
458399  mov     [ebp+var_C], 1
4583A0  mov     eax, [ebp+var_C]
4583A3  pop     edi
4583A4  pop     ebx
4583A5  mov     esp, ebp
4583A7  pop     ebp
4583A8  retn    14h
4583AB  mov     dword ptr [edi], 1
4583B1  mov     eax, [ebp+var_C]
4583B4  pop     edi
4583B5  pop     ebx
4583B6  mov     esp, ebp
4583B8  pop     ebp
4583B9  retn    14h

;==== 4583C0 sub_4583C0
4583C0  push    ebp
4583C1  mov     ebp, esp
4583C3  mov     eax, [ebp+arg_0]
4583C6  mov     ecx, [eax]
4583C8  mov     edx, [ebp+arg_4]
4583CB  mov     eax, [edx]
4583CD  mov     ecx, [ecx+14h]
4583D0  sub     ecx, [eax+14h]
4583D3  mov     eax, ecx
4583D5  pop     ebp
4583D6  retn

;==== 4583E0 sub_4583E0
4583E0  push    ebp
4583E1  mov     ebp, esp
4583E3  push    0FFFFFFFFh
4583E5  push    offset SEH_4583E0
4583EA  mov     eax, large fs:0
4583F0  push    eax
4583F1  sub     esp, 10h
4583F4  push    ebx
4583F5  mov     eax, dword_4FB734
4583FA  xor     eax, ebp
4583FC  push    eax
4583FD  lea     eax, [ebp+var_C]
458400  mov     large fs:0, eax
458406  push    44h ; 'D'; Size
458408  mov     dword ptr [esi], offset ??_7DCTESLGEvaluator@@6B@; const DCTESLGEvaluator::`vftable'
45840E  mov     [esi+8], edi
458411  call    ??2@YAPAXI@Z; operator new(uint)
458416  add     esp, 4
458419  mov     [ebp+var_10], eax
45841C  xor     ebx, ebx
45841E  mov     [ebp+var_4], ebx
458421  cmp     eax, ebx
458423  jz      short loc_45842D
458425  push    edi
458426  call    sub_4465B0
45842B  jmp     short loc_45842F
45842D  xor     eax, eax
45842F  mov     [ebp+var_4], 0FFFFFFFFh
458436  push    esi
458437  mov     ecx, offset sub_45BB20
45843C  mov     [esi+0Ch], eax
45843F  call    sub_446900
458444  mov     eax, [esi+8]
458447  xor     ecx, ecx
458449  mov     edx, 4
45844E  mul     edx
458450  seto    cl
458453  neg     ecx
458455  or      ecx, eax
458457  push    ecx; Size
458458  call    ??2@YAPAXI@Z; operator new(uint)
45845D  add     esp, 4
458460  push    ebx
458461  push    ebx
458462  push    ebx
458463  mov     ecx, esi
458465  mov     [esi+14h], eax
458468  mov     [esi+10h], ebx
45846B  mov     [esi+24h], ebx
45846E  mov     [esi+28h], ebx
458471  call    sub_458550
458476  lea     eax, [ebp+var_1C]
458479  mov     ecx, esi
45847B  mov     [ebp+var_1C], 18000h
458482  mov     [ebp+var_18], 10000h
458489  mov     [ebp+var_14], 0AB85h
458490  call    sub_4597A0
458495  mov     eax, esi
458497  call    sub_459820
45849C  mov     dword ptr [esi+154h], 32h ; '2'
4584A6  mov     dword ptr [esi+158h], 43h ; 'C'
4584B0  mov     ecx, [ebp+var_C]
4584B3  mov     large fs:0, ecx
4584BA  pop     ecx
4584BB  pop     ebx
4584BC  mov     esp, ebp
4584BE  pop     ebp
4584BF  retn
4DA320  mov     eax, [ebp+var_10]
4DA323  push    eax; void *
4DA324  call    ??3@YAXPAX@Z; operator delete(void *)
4DA329  pop     ecx
4DA32A  retn
4DA32B  mov     edx, [esp-4+arg_4]
4DA32F  lea     eax, [edx+0Ch]
4DA332  mov     ecx, [edx-18h]
4DA335  xor     ecx, eax
4DA337  call    sub_4ABDD5
4DA33C  mov     eax, offset stru_4F7C30
4DA341  jmp     ___CxxFrameHandler3

;==== 4584C0 sub_4584C0
4584C0  push    ebp
4584C1  mov     ebp, esp
4584C3  push    esi
4584C4  mov     esi, ecx
4584C6  call    sub_4584F0
4584CB  test    [ebp+arg_0], 1
4584CF  jz      short loc_4584DA
4584D1  push    esi; void *
4584D2  call    ??3@YAXPAX@Z; operator delete(void *)
4584D7  add     esp, 4
4584DA  mov     eax, esi
4584DC  pop     esi
4584DD  pop     ebp
4584DE  retn    4

;==== 4584F0 sub_4584F0
4584F0  push    esi
4584F1  push    0
4584F3  push    0
4584F5  mov     esi, ecx
4584F7  push    0
4584F9  mov     dword ptr [esi], offset ??_7DCTESLGEvaluator@@6B@; const DCTESLGEvaluator::`vftable'
4584FF  call    sub_458550
458504  mov     eax, [esi+0Ch]
458507  push    0
458509  xor     ecx, ecx
45850B  call    sub_446900
458510  mov     ecx, [esi+0Ch]
458513  test    ecx, ecx
458515  jz      short loc_45851F
458517  mov     eax, [ecx]
458519  mov     edx, [eax]
45851B  push    1
45851D  call    edx
45851F  mov     eax, [esi+14h]
458522  push    eax; void *
458523  call    ??3@YAXPAX@Z; operator delete(void *)
458528  add     esp, 4
45852B  pop     esi
45852C  retn

;==== 458530 sub_458530
458530  push    ebp
458531  mov     ebp, esp
458533  mov     eax, 90000002h
458538  cmp     ecx, 1
45853B  jl      short loc_458545
45853D  mov     eax, [ebp+arg_0]
458540  mov     [eax+20h], ecx
458543  xor     eax, eax
458545  pop     ebp
458546  retn    4

;==== 458550 sub_458550
458550  push    ebp
458551  mov     ebp, esp
458553  push    ebx
458554  push    esi
458555  mov     esi, [ebp+arg_0]
458558  push    edi
458559  mov     edi, ecx
45855B  test    esi, esi
45855D  jz      loc_4585EF
458563  mov     ebx, [ebp+arg_4]
458566  lea     eax, [ebx-1]
458569  cmp     eax, 3Fh ; '?'
45856C  ja      short loc_4585E3
45856E  push    0
458570  push    0
458572  push    0
458574  call    sub_458550
458579  mov     eax, esi
45857B  call    sub_4586B0
458580  xor     ecx, ecx
458582  mov     eax, ebx
458584  mov     edx, 8DCh
458589  mul     edx
45858B  seto    cl
45858E  mov     [edi+24h], ebx
458591  neg     ecx
458593  or      ecx, eax
458595  push    ecx; Size
458596  call    ??2@YAPAXI@Z; operator new(uint)
45859B  mov     ecx, ebx
45859D  imul    ecx, 8DCh
4585A3  push    ecx; Size
4585A4  push    0; Val
4585A6  push    eax; void *
4585A7  mov     [edi+28h], eax
4585AA  call    _memset
4585AF  add     esp, 10h
4585B2  xor     esi, esi
4585B4  test    ebx, ebx
4585B6  jz      short loc_4585DA
4585B8  mov     edx, [ebp+arg_8]
4585BB  mov     [ebp+arg_0], edx
4585BE  mov     edi, edi
4585C0  mov     eax, [ebp+arg_0]
4585C3  push    1
4585C5  push    eax
4585C6  push    edi
4585C7  mov     ecx, esi
4585C9  call    sub_4586E0
4585CE  add     [ebp+arg_0], 834h
4585D5  inc     esi
4585D6  cmp     esi, ebx
4585D8  jb      short loc_4585C0
4585DA  pop     edi
4585DB  pop     esi
4585DC  xor     eax, eax
4585DE  pop     ebx
4585DF  pop     ebp
4585E0  retn    0Ch
4585E3  pop     edi
4585E4  pop     esi
4585E5  mov     eax, 90000002h
4585EA  pop     ebx
4585EB  pop     ebp
4585EC  retn    0Ch
4585EF  mov     ecx, [edi+10h]
4585F2  test    ecx, ecx
4585F4  jz      short loc_4585FE
4585F6  mov     edx, [ecx]
4585F8  mov     eax, [edx]
4585FA  push    1
4585FC  call    eax
4585FE  xor     ebx, ebx
458600  mov     [edi+10h], ebx
458603  mov     [ebp+arg_0], ebx
458606  cmp     [edi+24h], ebx
458609  jbe     short loc_458686
45860B  jmp     short loc_458610
458610  mov     ecx, [edi+28h]
458613  mov     edx, [ebx+ecx+854h]
45861A  push    edx; void *
45861B  call    ??3@YAXPAX@Z; operator delete(void *)
458620  mov     eax, [edi+28h]
458623  mov     ecx, [ebx+eax+858h]
45862A  push    ecx; void *
45862B  call    ??3@YAXPAX@Z; operator delete(void *)
458630  add     esp, 8
458633  xor     esi, esi
458635  jmp     short loc_458640
458640  mov     eax, [edi+28h]
458643  lea     edx, [ebx+esi]
458646  mov     ecx, [edx+eax+85Ch]
45864D  push    ecx; void *
45864E  call    ??3@YAXPAX@Z; operator delete(void *)
458653  mov     eax, [edi+28h]
458656  lea     edx, [ebx+esi]
458659  mov     ecx, [edx+eax+860h]
458660  push    ecx; void *
458661  call    ??3@YAXPAX@Z; operator delete(void *)
458666  add     esi, 8
458669  add     esp, 8
45866C  cmp     esi, 80h
458672  jb      short loc_458640
458674  mov     eax, [ebp+arg_0]
458677  inc     eax
458678  add     ebx, 8DCh
45867E  mov     [ebp+arg_0], eax
458681  cmp     eax, [edi+24h]
458684  jb      short loc_458610
458686  mov     edx, [edi+28h]
458689  push    edx; void *
45868A  mov     dword ptr [edi+24h], 0
458691  call    ??3@YAXPAX@Z; operator delete(void *)
458696  add     esp, 4
458699  xor     eax, eax
45869B  mov     [edi+28h], eax
45869E  pop     edi
45869F  pop     esi
4586A0  pop     ebx
4586A1  pop     ebp
4586A2  retn    0Ch

;==== 4586B0 sub_4586B0
4586B0  mov     ecx, [edi+10h]
4586B3  push    esi
4586B4  mov     esi, eax
4586B6  test    ecx, ecx
4586B8  jz      short loc_4586C2
4586BA  mov     eax, [ecx]
4586BC  mov     edx, [eax]
4586BE  push    1
4586C0  call    edx
4586C2  test    esi, esi
4586C4  jz      short loc_4586D2
4586C6  call    sub_453260
4586CB  mov     [edi+10h], eax
4586CE  xor     eax, eax
4586D0  pop     esi
4586D1  retn
4586D2  xor     eax, eax
4586D4  mov     [edi+10h], eax
4586D7  pop     esi
4586D8  retn

;==== 4586E0 sub_4586E0
4586E0  push    ebp
4586E1  mov     ebp, esp
4586E3  sub     esp, 180h
4586E9  mov     eax, dword_4FB734
4586EE  xor     eax, ebp
4586F0  mov     [ebp+var_4], eax
4586F3  mov     eax, [ebp+arg_0]
4586F6  push    ebx
4586F7  mov     ebx, [ebp+arg_4]
4586FA  mov     [ebp+var_17C], eax
458700  mov     edx, 90000003h
458705  cmp     ecx, [eax+24h]
458708  jnb     loc_458A29
45870E  imul    ecx, 8DCh
458714  add     ecx, [eax+28h]
458717  push    esi
458718  mov     edx, ecx
45871A  push    edi
45871B  mov     ecx, 20Dh
458720  mov     esi, ebx
458722  mov     edi, edx
458724  rep movsd
458726  mov     ecx, [ebx]
458728  mov     [ebp+var_170], edx
45872E  test    ecx, ecx
458730  jz      loc_458A05
458736  mov     eax, [eax+10h]
458739  add     edx, 834h
45873F  push    eax
458740  call    sub_4536F0
458745  mov     esi, [ebp+var_17C]
45874B  mov     eax, [esi+10h]
45874E  lea     ecx, [ebp+var_178]
458754  push    ecx
458755  mov     ecx, [ebx]
458757  call    sub_453760
45875C  mov     edx, [ebp+var_178]
458762  mov     eax, [ebp+var_170]
458768  add     edx, 0FFFFFFFEh
45876B  cmp     [ebp+arg_8], 0
45876F  mov     [eax+83Ch], edx
458775  jz      loc_458A14
45877B  mov     eax, [esi+10h]
45877E  lea     ecx, [ebp+var_180]
458784  push    ecx
458785  lea     edx, [ebp+var_178]
45878B  call    sub_453500
458790  xor     edx, edx
458792  mov     esi, ebx
458794  lea     ecx, [ebp+var_158]
45879A  mov     [ebp+var_174], 1
4587A4  call    sub_459880
4587A9  xor     eax, eax
4587AB  lea     ecx, [ebp+var_144]
4587B1  lea     edi, [eax+1]
4587B4  mov     edx, edi
4587B6  mov     esi, ebx
4587B8  call    sub_459880
4587BD  test    eax, eax
4587BF  jnz     short loc_4587CA
4587C1  inc     [ebp+var_174]
4587C7  add     ecx, 14h
4587CA  mov     eax, edi
4587CC  cmp     eax, 10h
4587CF  jb      short loc_4587B1
4587D1  mov     eax, [ebp+var_174]
4587D7  mov     ecx, [ebx+238h]
4587DD  push    1; int
4587DF  push    0; int
4587E1  lea     edx, [ebp+var_158]
4587E7  push    edx; int
4587E8  mov     edx, [ebx+234h]
4587EE  push    eax; int
4587EF  mov     eax, [ebx]
4587F1  push    ecx; int
4587F2  mov     ecx, [ebp+var_17C]
4587F8  push    edx; void *
4587F9  mov     edx, [ecx+10h]
4587FC  push    eax; int
4587FD  push    edx; int
4587FE  call    sub_453C20
458803  mov     eax, [ebp+var_170]
458809  mov     ecx, [eax+854h]
45880F  push    ecx; void *
458810  call    ??3@YAXPAX@Z; operator delete(void *)
458815  mov     esi, [ebp+var_178]
45881B  imul    esi, [ebp+var_180]
458822  xor     ecx, ecx
458824  mov     eax, esi
458826  mov     edx, 1Ch
45882B  mul     edx
45882D  seto    cl
458830  mov     [ebp+var_174], esi
458836  neg     ecx
458838  or      ecx, eax
45883A  push    ecx; Size
45883B  call    ??2@YAPAXI@Z; operator new(uint)
458840  mov     esi, [ebp+var_170]
458846  mov     [esi+854h], eax
45884C  mov     eax, [esi+858h]
458852  push    eax; void *
458853  call    ??3@YAXPAX@Z; operator delete(void *)
458858  mov     ecx, [esi+854h]
45885E  mov     edx, [ebp+var_17C]
458864  add     esp, 0Ch
458867  push    0; int
458869  mov     dword ptr [esi+858h], 0
458873  mov     eax, [edx+10h]
458876  push    ecx; void *
458877  mov     ecx, [ebx]
458879  push    eax; int
45887A  lea     edi, [ebp+var_158]
458880  call    sub_4541A0
458885  test    eax, eax
458887  jnz     short loc_4588C6
458889  mov     eax, [ebp+var_174]
45888F  xor     ecx, ecx
458891  mov     edx, 1Ch
458896  mul     edx
458898  seto    cl
45889B  neg     ecx
45889D  or      ecx, eax
45889F  push    ecx; Size
4588A0  call    ??2@YAPAXI@Z; operator new(uint)
4588A5  mov     edx, [ebp+var_17C]
4588AB  add     esp, 4
4588AE  push    1; int
4588B0  mov     ecx, esi
4588B2  mov     [ecx+858h], eax
4588B8  mov     ecx, [ebx]
4588BA  push    eax; void *
4588BB  mov     eax, [edx+10h]
4588BE  push    eax; int
4588BF  call    sub_4541A0
4588C4  jmp     short loc_4588DF
4588C6  mov     ecx, [esi+854h]
4588CC  push    ecx; void *
4588CD  call    ??3@YAXPAX@Z; operator delete(void *)
4588D2  add     esp, 4
4588D5  mov     dword ptr [esi+854h], 0
4588DF  lea     edi, [esi+860h]
4588E5  mov     [ebp+var_178], 0
4588EF  mov     [ebp+var_170], edi
4588F5  mov     edx, [edi-4]
4588F8  push    edx; void *
4588F9  call    ??3@YAXPAX@Z; operator delete(void *)
4588FE  mov     eax, [edi]
458900  push    eax; void *
458901  mov     dword ptr [edi-4], 0
458908  call    ??3@YAXPAX@Z; operator delete(void *)
45890D  mov     edx, [ebp+var_178]
458913  inc     edx
458914  add     esp, 8
458917  mov     esi, ebx
458919  lea     ecx, [ebp+var_16C]
45891F  mov     dword ptr [edi], 0
458925  mov     [ebp+var_180], edx
45892B  call    sub_459880
458930  test    eax, eax
458932  jnz     loc_4589D2
458938  mov     eax, [ebp+var_174]
45893E  xor     ecx, ecx
458940  mov     edx, 1Ch
458945  mul     edx
458947  seto    cl
45894A  neg     ecx
45894C  or      ecx, eax
45894E  push    ecx; Size
45894F  call    ??2@YAPAXI@Z; operator new(uint)
458954  add     esp, 4
458957  mov     esi, eax
458959  mov     eax, [ebp+var_17C]
45895F  mov     ecx, [eax+10h]
458962  push    0; int
458964  push    esi; void *
458965  push    ecx; int
458966  mov     ecx, [ebx]
458968  lea     edi, [ebp+var_16C]
45896E  call    sub_4541A0
458973  test    eax, eax
458975  jnz     short loc_4589C3
458977  mov     edx, [ebp+var_170]
45897D  mov     eax, [ebp+var_174]
458983  mov     [edx-4], esi
458986  xor     ecx, ecx
458988  mov     edx, 1Ch
45898D  mul     edx
45898F  seto    cl
458992  neg     ecx
458994  or      ecx, eax
458996  push    ecx; Size
458997  call    ??2@YAPAXI@Z; operator new(uint)
45899C  add     esp, 4
45899F  mov     esi, eax
4589A1  mov     eax, [ebp+var_17C]
4589A7  mov     ecx, [eax+10h]
4589AA  push    1; int
4589AC  push    esi; void *
4589AD  push    ecx; int
4589AE  mov     ecx, [ebx]
4589B0  call    sub_4541A0
4589B5  test    eax, eax
4589B7  jnz     short loc_4589C3
4589B9  mov     edx, [ebp+var_170]
4589BF  mov     [edx], esi
4589C1  jmp     short loc_4589CC
4589C3  push    esi; void *
4589C4  call    ??3@YAXPAX@Z; operator delete(void *)
4589C9  add     esp, 4
4589CC  mov     edi, [ebp+var_170]
4589D2  mov     eax, [ebp+var_180]
4589D8  add     edi, 8
4589DB  mov     [ebp+var_178], eax
4589E1  mov     [ebp+var_170], edi
4589E7  cmp     eax, 10h
4589EA  jb      loc_4588F5
4589F0  pop     edi
4589F1  pop     esi
4589F2  xor     eax, eax
4589F4  pop     ebx
4589F5  mov     ecx, [ebp+var_4]
4589F8  xor     ecx, ebp
4589FA  call    sub_4ABDD5
4589FF  mov     esp, ebp
458A01  pop     ebp
458A02  retn    0Ch
458A05  or      eax, 0FFFFFFFFh
458A08  mov     [edx+834h], eax
458A0E  mov     [edx+838h], eax
458A14  pop     edi
458A15  pop     esi
458A16  xor     eax, eax
458A18  pop     ebx
458A19  mov     ecx, [ebp+var_4]
458A1C  xor     ecx, ebp
458A1E  call    sub_4ABDD5
458A23  mov     esp, ebp
458A25  pop     ebp
458A26  retn    0Ch
458A29  mov     ecx, [ebp+var_4]
458A2C  xor     ecx, ebp
458A2E  mov     eax, edx
458A30  pop     ebx
458A31  call    sub_4ABDD5
458A36  mov     esp, ebp
458A38  pop     ebp
458A39  retn    0Ch

;==== 458A40 sub_458A40
458A40  push    ebp
458A41  mov     ebp, esp
458A43  push    esi
458A44  mov     esi, eax
458A46  mov     eax, 90000003h
458A4B  cmp     esi, [ecx+24h]
458A4E  jnb     short loc_458A67
458A50  imul    esi, 8DCh
458A56  add     esi, [ecx+28h]
458A59  push    edi
458A5A  mov     edi, [ebp+arg_0]
458A5D  mov     ecx, 20Dh
458A62  rep movsd
458A64  xor     eax, eax
458A66  pop     edi
458A67  pop     esi
458A68  pop     ebp
458A69  retn    4

;==== 458A70 sub_458A70
458A70  push    ebp
458A71  mov     ebp, esp
458A73  sub     esp, 31Ch
458A79  mov     eax, dword_4FB734
458A7E  xor     eax, ebp
458A80  mov     [ebp+var_4], eax
458A83  mov     eax, [ebp+arg_4]
458A86  push    ebx
458A87  mov     ebx, [ebp+arg_0]
458A8A  mov     [ebp+var_2B8], eax
458A90  mov     eax, [ebx+24h]
458A93  mov     [ebp+var_2C4], ecx
458A99  mov     ecx, [ebp+arg_8]
458A9C  push    esi
458A9D  push    edi
458A9E  mov     [ebp+var_284], ecx
458AA4  cmp     ecx, eax
458AA6  jb      short loc_458AC0
458AA8  mov     eax, 90000003h
458AAD  pop     edi
458AAE  pop     esi
458AAF  pop     ebx
458AB0  mov     ecx, [ebp+var_4]
458AB3  xor     ecx, ebp
458AB5  call    sub_4ABDD5
458ABA  mov     esp, ebp
458ABC  pop     ebp
458ABD  retn    10h
458AC0  xor     esi, esi
458AC2  mov     [ebx+2Ch], ecx
458AC5  test    eax, eax
458AC7  jz      short loc_458ADE
458AC9  lea     esp, [esp+0]
458AD0  push    ebx
458AD1  mov     ecx, esi
458AD3  call    sub_459CF0
458AD8  inc     esi
458AD9  cmp     esi, [ebx+24h]
458ADC  jb      short loc_458AD0
458ADE  mov     eax, [ebx+10h]
458AE1  lea     edx, [ebp+var_298]
458AE7  push    edx
458AE8  lea     edx, [ebp+var_270]
458AEE  call    sub_453500
458AF3  mov     eax, [ebp+var_270]
458AF9  mov     ecx, [ebp+var_298]
458AFF  mov     [ebx+18h], eax
458B02  imul    eax, ecx
458B05  mov     [ebx+1Ch], ecx
458B08  xor     ecx, ecx
458B0A  mov     esi, eax
458B0C  mov     edx, 8
458B11  mul     edx
458B13  seto    cl
458B16  neg     ecx
458B18  or      ecx, eax
458B1A  push    ecx; Size
458B1B  call    ??2@YAPAXI@Z; operator new(uint)
458B20  mov     [ebp+var_278], eax
458B26  xor     ecx, ecx
458B28  add     esp, 4
458B2B  mov     eax, esi
458B2D  mov     edx, 8
458B32  mul     edx
458B34  seto    cl
458B37  neg     ecx
458B39  or      ecx, eax
458B3B  push    ecx; Size
458B3C  call    ??2@YAPAXI@Z; operator new(uint)
458B41  mov     [ebp+var_274], eax
458B47  mov     eax, [ebx+24h]
458B4A  xor     ecx, ecx
458B4C  add     esp, 4
458B4F  mov     edx, 8DCh
458B54  mul     edx
458B56  seto    cl
458B59  neg     ecx
458B5B  or      ecx, eax
458B5D  push    ecx; Size
458B5E  call    ??2@YAPAXI@Z; operator new(uint)
458B63  mov     ecx, [ebx+28h]
458B66  mov     edi, eax
458B68  mov     eax, [ebx+24h]
458B6B  imul    eax, 8DCh
458B71  add     esp, 4
458B74  push    eax; Size
458B75  push    ecx; Src
458B76  push    edi; void *
458B77  mov     [ebp+var_28C], edi
458B7D  call    _memcpy_0
458B82  mov     esi, [ebp+var_284]
458B88  imul    esi, 8DCh
458B8E  xor     eax, eax
458B90  add     esi, edi
458B92  add     esp, 0Ch
458B95  mov     [ebp+var_26C], esi
458B9B  mov     [ebp+var_2A0], eax
458BA1  mov     [ebp+var_29C], 0FFFFFFFFh
458BAB  mov     [ebp+var_2AC], eax
458BB1  cmp     [esi+0Ch], eax
458BB4  jl      loc_458C67
458BBA  mov     edx, [esi+234h]
458BC0  mov     ecx, [esi]
458BC2  push    eax
458BC3  push    0FFFFFFFFh
458BC5  push    0FFFFFFFFh
458BC7  push    eax
458BC8  mov     eax, [ebx+10h]
458BCB  push    edx
458BCC  call    sub_453830
458BD1  mov     eax, [esi+844h]
458BD7  mov     ecx, [esi+840h]
458BDD  push    eax
458BDE  mov     eax, [ebx+10h]
458BE1  push    ecx
458BE2  mov     ecx, [esi]
458BE4  lea     edx, [ebp+var_29C]
458BEA  push    edx
458BEB  push    0
458BED  call    sub_453930
458BF2  test    eax, eax
458BF4  jnz     short loc_458C3C
458BF6  mov     eax, [ebp+var_29C]
458BFC  xor     ecx, ecx
458BFE  mov     edx, 8
458C03  mul     edx
458C05  seto    cl
458C08  neg     ecx
458C0A  or      ecx, eax
458C0C  push    ecx; Size
458C0D  call    ??2@YAPAXI@Z; operator new(uint)
458C12  mov     ecx, [esi+844h]
458C18  mov     edx, [esi+840h]
458C1E  add     esp, 4
458C21  push    ecx
458C22  push    edx
458C23  mov     edx, [ebx+10h]
458C26  lea     ecx, [ebp+var_29C]
458C2C  push    ecx
458C2D  push    eax
458C2E  mov     [ebp+var_2A0], eax
458C34  mov     eax, [esi]
458C36  push    edx
458C37  call    sub_4539B0
458C3C  mov     eax, [esi+0Ch]
458C3F  imul    eax, 8DCh
458C45  mov     ecx, [eax+edi+234h]
458C4C  push    0
458C4E  push    0FFFFFFFFh
458C50  add     eax, edi
458C52  push    0FFFFFFFFh
458C54  push    0
458C56  push    ecx
458C57  mov     ecx, [eax]
458C59  mov     [ebp+var_2AC], eax
458C5F  mov     eax, [ebx+10h]
458C62  call    sub_453830
458C67  mov     edx, [esi+238h]
458C6D  mov     eax, [esi+234h]
458C73  mov     ecx, [esi]
458C75  push    1
458C77  push    0FFFFFFFFh
458C79  push    0FFFFFFFFh
458C7B  push    edx
458C7C  push    eax
458C7D  mov     eax, [ebx+10h]
458C80  call    sub_453830
458C85  mov     edx, [ebp+var_278]
458C8B  mov     eax, [ebx+10h]
458C8E  push    1
458C90  lea     ecx, [ebp+var_270]
458C96  push    ecx
458C97  mov     ecx, [esi]
458C99  push    edx
458C9A  call    sub_453B80
458C9F  mov     ecx, [ebx+10h]
458CA2  mov     eax, [ebp+var_278]
458CA8  mov     edi, [ebp+var_270]
458CAE  push    ecx
458CAF  mov     ecx, [esi]
458CB1  lea     edx, [eax+edi*8]
458CB4  call    sub_4536F0
458CB9  xor     ecx, ecx
458CBB  mov     eax, 1000h
458CC0  mov     [ebp+var_290], eax
458CC6  mov     edx, 4Ch ; 'L'
458CCB  mul     edx
458CCD  seto    cl
458CD0  mov     [ebp+var_25C], 0
458CDA  neg     ecx
458CDC  or      ecx, eax
458CDE  push    ecx; Size
458CDF  call    ??2@YAPAXI@Z; operator new(uint)
458CE4  mov     [ebp+Src], eax
458CEA  or      eax, 0FFFFFFFFh
458CED  mov     [ebp+var_24C], eax
458CF3  mov     [ebp+var_248], eax
458CF9  mov     [ebp+var_244], eax
458CFF  mov     [ebp+var_23C], eax
458D05  mov     eax, 80000000h
458D0A  mov     [ebp+var_224], eax
458D10  mov     [ebp+var_220], eax
458D16  mov     [ebp+var_21C], eax
458D1C  mov     [ebp+var_218], eax
458D22  mov     [ebp+var_214], eax
458D28  mov     [ebp+var_210], eax
458D2E  mov     eax, [ebp+var_278]
458D34  mov     [ebp+var_27C], eax
458D3A  mov     eax, 0FFFFFFFCh
458D3F  sub     eax, [ebp+var_2A0]
458D45  add     esp, 4
458D48  inc     edi
458D49  mov     [ebp+var_240], 80000000h
458D53  mov     [ebp+var_230], 0
458D5D  mov     [ebp+var_2B4], 6
458D67  mov     [ebp+var_2BC], eax
458D6D  mov     [ebp+var_270], edi
458D73  jmp     short loc_458D80
458D80  mov     eax, [ebp+var_27C]
458D86  mov     ecx, [ebp+var_26C]
458D8C  mov     esi, [eax+4]
458D8F  mov     edi, [eax]
458D91  mov     ecx, [ecx]
458D93  mov     edx, [ebx+10h]
458D96  call    sub_4536B0
458D9B  mov     edx, [ebp+var_27C]
458DA1  mov     ecx, [edx]
458DA3  mov     eax, [ebp+var_26C]
458DA9  mov     [eax+834h], ecx
458DAF  mov     ecx, [edx+4]
458DB2  mov     [eax+838h], ecx
458DB8  mov     ecx, [ebp+var_25C]
458DBE  mov     eax, ecx
458DC0  imul    eax, 4Ch ; 'L'
458DC3  add     eax, [ebp+Src]
458DC9  inc     ecx
458DCA  mov     [ebp+var_25C], ecx
458DD0  mov     edi, eax
458DD2  mov     ecx, 13h
458DD7  lea     esi, [ebp+var_258]
458DDD  rep movsd
458DDF  mov     ecx, [edx]
458DE1  mov     [eax], ecx
458DE3  mov     ecx, [edx+4]
458DE6  mov     [eax+4], ecx
458DE9  mov     ecx, [ebp+var_26C]
458DEF  mov     dword ptr [eax+8], 0
458DF6  mov     edi, [edx+4]
458DF9  mov     esi, [edx]
458DFB  mov     ecx, [ecx]
458DFD  mov     [ebp+var_260], eax
458E03  lea     eax, [ebp+var_2B4]
458E09  push    eax
458E0A  mov     eax, [ebx+10h]
458E0D  call    sub_453A50
458E12  mov     edx, [ebp+var_2B4]
458E18  mov     eax, [ebp+var_260]
458E1E  add     edx, 0FFFFFFFEh
458E21  mov     [eax+20h], edx
458E24  mov     edx, [ebp+var_26C]
458E2A  lea     esi, [eax+24h]
458E2D  mov     eax, [ebp+var_27C]
458E33  mov     ecx, [eax]
458E35  mov     edi, [eax+4]
458E38  mov     eax, [ebx+10h]
458E3B  push    ecx
458E3C  mov     ecx, [edx]
458E3E  push    esi
458E3F  call    sub_453AC0
458E44  cmp     [ebp+var_2AC], 0
458E4B  jz      short loc_458E84
458E4D  mov     eax, [ebp+var_27C]
458E53  mov     edx, [eax]
458E55  mov     ecx, [ebp+var_260]
458E5B  mov     edi, [eax+4]
458E5E  mov     eax, [ebp+var_2AC]
458E64  add     ecx, 28h ; '('
458E67  push    edx
458E68  push    ecx
458E69  mov     ecx, [eax]
458E6B  mov     eax, [ebx+10h]
458E6E  call    sub_453AC0
458E73  test    eax, eax
458E75  jz      short loc_458E84
458E77  mov     eax, [ebp+var_260]
458E7D  mov     dword ptr [eax+28h], 0FFFFFFFFh
458E84  mov     esi, [esi]
458E86  test    esi, esi
458E88  jz      short loc_458EA1
458E8A  mov     ecx, [ebp+var_26C]
458E90  mov     eax, [ecx+240h]
458E96  imul    eax, esi
458E99  add     eax, [ecx+23Ch]
458E9F  jmp     short loc_458EA3
458EA1  xor     eax, eax
458EA3  mov     ecx, [ebp+var_260]
458EA9  mov     [ecx+2Ch], eax
458EAC  mov     eax, [ebp+var_29C]
458EB2  mov     dword ptr [ecx+30h], 0
458EB9  test    eax, eax
458EBB  jle     short loc_458F14
458EBD  mov     ecx, [ebp+var_2BC]
458EC3  mov     edx, [ebp+var_27C]
458EC9  mov     esi, [ebp+var_2A0]
458ECF  lea     edx, [ecx+edx+4]
458ED3  mov     [ebp+Base], eax
458ED9  lea     esp, [esp+0]
458EE0  mov     ecx, 8
458EE5  mov     eax, esi
458EE7  mov     edi, [edx+eax]
458EEA  cmp     edi, [eax]
458EEC  jnz     short loc_458F06
458EEE  sub     ecx, 4
458EF1  add     eax, 4
458EF4  cmp     ecx, 4
458EF7  jnb     short loc_458EE7
458EF9  mov     eax, [ebp+var_260]
458EFF  mov     dword ptr [eax+30h], 1
458F06  sub     edx, 8
458F09  add     esi, 8
458F0C  dec     [ebp+Base]
458F12  jnz     short loc_458EE0
458F14  mov     eax, [ebp+var_26C]
458F1A  mov     esi, [ebp+var_260]
458F20  push    1
458F22  push    0
458F24  push    1
458F26  mov     ecx, 13h
458F2B  lea     edi, [ebp+var_318]
458F31  rep movsd
458F33  mov     esi, [eax+25Ch]
458F39  mov     ecx, [ebx+10h]
458F3C  mov     edx, [ecx]
458F3E  mov     edx, [edx+4]
458F41  push    esi
458F42  mov     esi, [eax+258h]
458F48  mov     eax, [eax+254h]
458F4E  push    esi
458F4F  push    eax
458F50  mov     eax, [ebp+var_27C]
458F56  mov     esi, [eax+4]
458F59  mov     eax, [eax]
458F5B  push    esi
458F5C  push    eax
458F5D  lea     eax, [ebp+var_294]
458F63  push    eax
458F64  mov     eax, [ebp+var_274]
458F6A  push    0
458F6C  push    eax
458F6D  mov     [ebp+var_310], 6
458F77  call    edx
458F79  cmp     [ebp+var_294], 0
458F80  mov     [ebp+Base], 0
458F8A  jbe     loc_459092
458F90  mov     eax, [ebp+var_25C]
458F96  mov     ecx, [ebp+var_274]
458F9C  imul    eax, 4Ch ; 'L'
458F9F  mov     [ebp+Size], eax
458FA5  mov     [ebp+var_260], ecx
458FAB  mov     eax, [ebp+var_28C]
458FB1  push    eax
458FB2  mov     edx, ebx
458FB4  call    sub_45A420
458FB9  test    eax, eax
458FBB  jz      loc_45906A
458FC1  mov     ecx, [eax+4]
458FC4  mov     edx, [ebp+var_26C]
458FCA  test    [edx+4], ecx
458FCD  jnz     loc_45906A
458FD3  mov     esi, [ebp+var_290]
458FD9  cmp     [ebp+var_25C], esi
458FDF  jb      short loc_45902C
458FE1  add     esi, esi
458FE3  xor     ecx, ecx
458FE5  mov     eax, esi
458FE7  mov     edx, 4Ch ; 'L'
458FEC  mul     edx
458FEE  seto    cl
458FF1  mov     [ebp+var_290], esi
458FF7  neg     ecx
458FF9  or      ecx, eax
458FFB  push    ecx; Size
458FFC  call    ??2@YAPAXI@Z; operator new(uint)
459001  mov     edi, [ebp+Src]
459007  mov     esi, eax
459009  mov     eax, [ebp+Size]
45900F  add     esp, 4
459012  push    eax; Size
459013  push    edi; Src
459014  push    esi; void *
459015  call    _memcpy_0
45901A  add     esp, 0Ch
45901D  push    edi; void *
45901E  call    ??3@YAXPAX@Z; operator delete(void *)
459023  add     esp, 4
459026  mov     [ebp+Src], esi
45902C  mov     ecx, [ebp+Size]
459032  mov     edx, [ebp+Src]
459038  inc     [ebp+var_25C]
45903E  lea     eax, [ecx+edx]
459041  add     ecx, 4Ch ; 'L'
459044  mov     [ebp+Size], ecx
45904A  mov     ecx, 13h
45904F  lea     esi, [ebp+var_318]
459055  mov     edi, eax
459057  rep movsd
459059  mov     ecx, [ebp+var_260]
45905F  mov     edx, [ecx]
459061  mov     [eax+10h], edx
459064  mov     ecx, [ecx+4]
459067  mov     [eax+14h], ecx
45906A  mov     eax, [ebp+Base]
459070  mov     ecx, [ebp+var_260]
459076  inc     eax
459077  add     ecx, 8
45907A  mov     [ebp+Base], eax
459080  mov     [ebp+var_260], ecx
459086  cmp     eax, [ebp+var_294]
45908C  jb      loc_458FAB
459092  mov     eax, [ebp+var_26C]
459098  add     eax, 2E8h
45909D  mov     [ebp+var_310], 7
4590A7  mov     [ebp+Base], 0
4590B1  mov     [ebp+var_260], eax
4590B7  jmp     short loc_4590C0
4590C0  mov     edx, [ebp+var_260]
4590C6  cmp     dword ptr [edx-34h], 0
4590CA  jz      loc_4593D2
4590D0  mov     eax, [ebp+Base]
4590D6  mov     [ebp+var_30C], eax
4590DC  mov     eax, edx
4590DE  mov     edi, [eax]
4590E0  test    edi, edi
4590E2  js      loc_459351
4590E8  xor     ecx, ecx
4590EA  mov     esi, [ebx+10h]
4590ED  mov     edx, [esi]
4590EF  push    0
4590F1  push    0
4590F3  mov     edx, [edx+4]
4590F6  test    edi, edi
4590F8  setz    cl
4590FB  push    ecx
4590FC  mov     ecx, [eax-4]
4590FF  push    ecx
459100  mov     ecx, [eax-8]
459103  mov     eax, [eax-0Ch]
459106  push    ecx
459107  push    eax
459108  mov     eax, [ebp+var_27C]
45910E  mov     ecx, [eax+4]
459111  mov     eax, [eax]
459113  push    ecx
459114  push    eax
459115  mov     eax, [ebp+var_274]
45911B  lea     ecx, [ebp+var_294]
459121  push    ecx
459122  push    0
459124  push    eax
459125  mov     ecx, esi
459127  call    edx
459129  cmp     [ebp+var_294], 0
459130  mov     [ebp+var_298], 0
45913A  jbe     loc_4593D2
459140  mov     eax, [ebp+var_25C]
459146  imul    eax, 4Ch ; 'L'
459149  mov     [ebp+NumOfElements], eax
45914F  mov     eax, [ebp+var_274]
459155  mov     [ebp+Size], eax
45915B  jmp     short loc_459160
459160  mov     esi, [ebp+var_260]
459166  mov     eax, [esi]
459168  test    eax, eax
45916A  jnz     short loc_4591BB
45916C  mov     ecx, [ebp+var_28C]
459172  push    ecx
459173  mov     ecx, [ebp+Size]
459179  mov     edx, ebx
45917B  call    sub_45A420
459180  mov     ecx, [esi-30h]
459183  cmp     ecx, 4; switch 5 cases
459186  ja      def_45918C; jumptable 0045918C default case
45918C  jmp     ds:jpt_45918C[ecx*4]; switch jump
459193  mov     edx, [eax+4]; jumptable 0045918C cases 0,4
459196  mov     eax, [ebp+var_26C]
45919C  test    [eax+4], edx
45919F  jnz     loc_45932C
4591A5  jmp     def_45918C; jumptable 0045918C default case
4591AA  mov     ecx, [eax+4]; jumptable 0045918C cases 1-3
4591AD  mov     edx, [ebp+var_26C]
4591B3  test    [edx+4], ecx
4591B6  jmp     loc_45928F
4591BB  cmp     eax, 1
4591BE  jl      def_45918C; jumptable 0045918C default case
4591C4  mov     ecx, [ebx+10h]
4591C7  mov     edx, [ecx]
4591C9  push    0
4591CB  mov     edx, [edx+4]
4591CE  push    0
4591D0  push    1
4591D2  push    0
4591D4  push    0
4591D6  push    eax
4591D7  mov     eax, [ebp+Size]
4591DD  mov     esi, [eax+4]
4591E0  mov     eax, [eax]
4591E2  push    esi
4591E3  push    eax
4591E4  lea     eax, [ebp+var_2C0]
4591EA  push    eax
4591EB  push    0
4591ED  lea     eax, [ebp+var_208]
4591F3  push    eax
4591F4  call    edx
4591F6  mov     edi, [ebp+var_2C0]
4591FC  test    edi, edi
4591FE  jz      loc_45932C
459204  xor     eax, eax
459206  mov     [ebp+var_2B0], eax
45920C  mov     [ebp+var_2A4], eax
459212  cmp     edi, eax
459214  jbe     short loc_45926A
459216  mov     eax, [ebp+var_26C]
45921C  mov     ecx, [eax+4]
45921F  mov     [ebp+var_2A8], ecx
459225  lea     esi, [ebp+var_208]
45922B  jmp     short loc_459230
459230  mov     edx, [ebp+var_28C]
459236  push    edx
459237  mov     ecx, esi
459239  mov     edx, ebx
45923B  call    sub_45A420
459240  mov     eax, [eax+4]
459243  and     eax, [ebp+var_2A8]
459249  mov     ecx, 0
45924E  setnz   cl
459251  xor     edx, edx
459253  add     [ebp+var_2B0], ecx
459259  test    eax, eax
45925B  setz    dl
45925E  add     esi, 8
459261  add     [ebp+var_2A4], edx
459267  dec     edi
459268  jnz     short loc_459230
45926A  mov     eax, [ebp+var_260]
459270  mov     eax, [eax-30h]
459273  cmp     eax, 4; switch 5 cases
459276  ja      short def_45918C; jumptable 0045918C default case
459278  jmp     ds:jpt_459278[eax*4]; switch jump
45927F  cmp     [ebp+var_2A4], 0; jumptable 00459278 cases 0,4
459286  jmp     short loc_45928F
459288  cmp     [ebp+var_2B0], 0; jumptable 00459278 cases 1-3
45928F  jz      loc_45932C
459295  mov     esi, [ebp+var_290]; jumptable 0045918C default case
45929B  cmp     [ebp+var_25C], esi
4592A1  jb      short loc_4592EE
4592A3  add     esi, esi
4592A5  xor     ecx, ecx
4592A7  mov     eax, esi
4592A9  mov     edx, 4Ch ; 'L'
4592AE  mul     edx
4592B0  seto    cl
4592B3  mov     [ebp+var_290], esi
4592B9  neg     ecx
4592BB  or      ecx, eax
4592BD  push    ecx; Size
4592BE  call    ??2@YAPAXI@Z; operator new(uint)
4592C3  mov     edi, [ebp+Src]
4592C9  mov     esi, eax
4592CB  mov     eax, [ebp+NumOfElements]
4592D1  add     esp, 4
4592D4  push    eax; Size
4592D5  push    edi; Src
4592D6  push    esi; void *
4592D7  call    _memcpy_0
4592DC  add     esp, 0Ch
4592DF  push    edi; void *
4592E0  call    ??3@YAXPAX@Z; operator delete(void *)
4592E5  add     esp, 4
4592E8  mov     [ebp+Src], esi
4592EE  mov     eax, [ebp+NumOfElements]
4592F4  mov     ecx, [ebp+Src]
4592FA  inc     [ebp+var_25C]
459300  lea     edx, [eax+ecx]
459303  add     eax, 4Ch ; 'L'
459306  mov     [ebp+NumOfElements], eax
45930C  mov     eax, [ebp+Size]
459312  mov     ecx, 13h
459317  lea     esi, [ebp+var_318]
45931D  mov     edi, edx
45931F  rep movsd
459321  mov     ecx, [eax]
459323  mov     [edx+10h], ecx
459326  mov     eax, [eax+4]
459329  mov     [edx+14h], eax
45932C  mov     eax, [ebp+var_298]
459332  add     [ebp+Size], 8
459339  inc     eax
45933A  mov     [ebp+var_298], eax
459340  cmp     eax, [ebp+var_294]
459346  jb      loc_459160
45934C  jmp     loc_4593D2
459351  mov     esi, [ebp+var_290]
459357  cmp     [ebp+var_25C], esi
45935D  jb      short loc_4593AD
45935F  add     esi, esi
459361  xor     ecx, ecx
459363  mov     eax, esi
459365  mov     edx, 4Ch ; 'L'
45936A  mul     edx
45936C  seto    cl
45936F  mov     [ebp+var_290], esi
459375  neg     ecx
459377  or      ecx, eax
459379  push    ecx; Size
45937A  call    ??2@YAPAXI@Z; operator new(uint)
45937F  mov     edi, [ebp+Src]
459385  mov     esi, eax
459387  mov     eax, [ebp+var_25C]
45938D  imul    eax, 4Ch ; 'L'
459390  add     esp, 4
459393  push    eax; Size
459394  push    edi; Src
459395  push    esi; void *
459396  call    _memcpy_0
45939B  add     esp, 0Ch
45939E  push    edi; void *
45939F  call    ??3@YAXPAX@Z; operator delete(void *)
4593A4  add     esp, 4
4593A7  mov     [ebp+Src], esi
4593AD  mov     eax, [ebp+var_25C]
4593B3  mov     edi, eax
4593B5  imul    edi, 4Ch ; 'L'
4593B8  add     edi, [ebp+Src]
4593BE  mov     ecx, 13h
4593C3  lea     esi, [ebp+var_318]
4593C9  inc     eax
4593CA  rep movsd
4593CC  mov     [ebp+var_25C], eax
4593D2  mov     eax, [ebp+Base]
4593D8  add     [ebp+var_260], 58h ; 'X'
4593DF  inc     eax
4593E0  mov     [ebp+Base], eax
4593E6  cmp     eax, 10h
4593E9  jb      loc_4590C0
4593EF  add     [ebp+var_27C], 8
4593F6  dec     [ebp+var_270]
4593FC  jnz     loc_458D80
459402  mov     ecx, [ebp+var_278]
459408  push    ecx; void *
459409  call    ??3@YAXPAX@Z; operator delete(void *)
45940E  mov     edx, [ebp+var_274]
459414  add     esp, 4
459417  push    edx; void *
459418  call    ??3@YAXPAX@Z; operator delete(void *)
45941D  mov     esi, [ebx+24h]
459420  mov     eax, [ebp+var_25C]
459426  mov     ecx, [ebp+Src]
45942C  add     esp, 4
45942F  lea     edx, ds:0[esi*4]
459436  push    edx; Size
459437  xor     edi, edi
459439  mov     [ebx+148h], eax
45943F  lea     eax, [ebp+var_108]
459445  push    edi; Val
459446  push    eax; void *
459447  mov     [ebx+14Ch], edi
45944D  mov     [ebx+150h], ecx
459453  call    _memset
459458  mov     ecx, [ebp+var_284]
45945E  add     esp, 0Ch
459461  mov     [ebp+ecx*4+var_108], 1
45946C  mov     [ebp+var_270], edi
459472  cmp     esi, edi
459474  jbe     loc_45951D
45947A  lea     edx, [ebx+30h]
45947D  mov     [ebp+Base], edx
459483  xor     eax, eax
459485  or      esi, 0FFFFFFFFh
459488  mov     [ebp+var_284], 7FFFFFFFh
459492  cmp     [ebx+24h], eax
459495  jbe     short loc_4594E9
459497  mov     edx, [ebp+var_28C]
45949D  mov     edi, [ebx+24h]
4594A0  add     edx, 12Ch
4594A6  cmp     [ebp+eax*4+var_108], 0
4594AE  jnz     short loc_4594DE
4594B0  cmp     dword ptr [edx-12Ch], 0
4594B7  jz      short loc_4594D3
4594B9  mov     ecx, [edx+84h]
4594BF  add     ecx, [edx]
4594C1  cmp     ecx, [ebp+var_284]
4594C7  jge     short loc_4594DE
4594C9  mov     esi, eax
4594CB  mov     [ebp+var_284], ecx
4594D1  jmp     short loc_4594DE
4594D3  mov     [ebp+eax*4+var_108], 1
4594DE  inc     eax
4594DF  add     edx, 8DCh
4594E5  cmp     eax, edi
4594E7  jb      short loc_4594A6
4594E9  mov     eax, [ebp+Base]
4594EF  mov     [eax], esi
4594F1  test    esi, esi
4594F3  js      short loc_45951D
4594F5  mov     eax, [ebp+var_270]
4594FB  add     [ebp+Base], 4
459502  inc     eax
459503  mov     [ebp+esi*4+var_108], 1
45950E  mov     [ebp+var_270], eax
459514  cmp     eax, [ebx+24h]
459517  jb      loc_459483
45951D  xor     edi, edi
45951F  cmp     [ebx+8], edi
459522  jbe     short loc_459538
459524  mov     esi, [ebx+10h]
459527  call    sub_453260
45952C  mov     ecx, [ebx+14h]
45952F  mov     [ecx+edi*4], eax
459532  inc     edi
459533  cmp     edi, [ebx+8]
459536  jb      short loc_459524
459538  mov     ecx, [ebx+0Ch]
45953B  mov     eax, 1
459540  call    sub_446910
459545  xor     esi, esi
459547  cmp     [ebx+8], esi
45954A  jbe     short loc_459568
45954C  lea     esp, [esp+0]
459550  mov     edx, [ebx+14h]
459553  mov     ecx, [edx+esi*4]
459556  test    ecx, ecx
459558  jz      short loc_459562
45955A  mov     eax, [ecx]
45955C  mov     edx, [eax]
45955E  push    1
459560  call    edx
459562  inc     esi
459563  cmp     esi, [ebx+8]
459566  jb      short loc_459550
459568  xor     esi, esi
45956A  cmp     [ebp+var_25C], esi
459570  jbe     loc_45971F
459576  mov     edi, [ebp+Src]
45957C  mov     edx, [ebp+var_25C]
459582  add     edi, 34h ; '4'
459585  mov     eax, edi
459587  mov     ecx, 6
45958C  lea     esp, [esp+0]
459590  xor     ebx, ebx
459592  cmp     dword ptr [eax], 80000000h
459598  setnz   bl
45959B  add     eax, 4
45959E  add     esi, ebx
4595A0  dec     ecx
4595A1  jnz     short loc_459590
4595A3  add     edi, 4Ch ; 'L'
4595A6  dec     edx
4595A7  jnz     short loc_459585
4595A9  mov     [ebp+NumOfElements], esi
4595AF  cmp     esi, 1
4595B2  jb      loc_45971F
4595B8  mov     eax, esi
4595BA  mov     edx, 1Ch
4595BF  mul     edx
4595C1  seto    cl
4595C4  neg     ecx
4595C6  or      ecx, eax
4595C8  push    ecx; Size
4595C9  call    ??2@YAPAXI@Z; operator new(uint)
4595CE  mov     [ebp+var_2A8], eax
4595D4  mov     ebx, eax
4595D6  xor     ecx, ecx
4595D8  add     esp, 4
4595DB  mov     eax, esi
4595DD  mov     edx, 4
4595E2  mul     edx
4595E4  seto    cl
4595E7  neg     ecx
4595E9  or      ecx, eax
4595EB  push    ecx; Size
4595EC  call    ??2@YAPAXI@Z; operator new(uint)
4595F1  mov     ecx, [ebp+var_25C]
4595F7  mov     [ebp+Base], eax
4595FD  mov     [ebp+var_284], eax
459603  mov     eax, [ebp+Src]
459609  add     esp, 4
45960C  add     eax, 1Ch
45960F  mov     [ebp+var_274], eax
459615  mov     [ebp+var_270], ecx
45961B  jmp     short loc_459620
459620  mov     eax, [ebp+var_274]
459626  xor     edx, edx
459628  add     eax, 18h
45962B  mov     [ebp+var_278], eax
459631  mov     ecx, [ebp+var_278]
459637  cmp     dword ptr [ecx], 80000000h
45963D  jz      short loc_459695
45963F  mov     esi, [ebp+var_274]
459645  add     esi, 0FFFFFFE4h
459648  mov     ecx, 7
45964D  mov     edi, ebx
45964F  xor     eax, eax
459651  rep movsd
459653  cmp     [ebx+8], eax
459656  jnz     short loc_459674
459658  mov     ecx, [ebp+var_274]
45965E  mov     [ebx+8], edx
459661  cmp     [ecx], edx
459663  jnz     short loc_45966A
459665  mov     eax, 2
45966A  cmp     [ecx+4], edx
45966D  jnz     short loc_459674
45966F  mov     eax, 1
459674  mov     ecx, [ebp+var_278]
45967A  mov     ecx, [ecx]
45967C  add     ecx, eax
45967E  mov     eax, [ebp+var_284]
459684  mov     [ebx+18h], ecx
459687  mov     [eax], ebx
459689  add     ebx, 1Ch
45968C  add     eax, 4
45968F  mov     [ebp+var_284], eax
459695  add     [ebp+var_278], 4
45969C  inc     edx
45969D  cmp     edx, 6
4596A0  jb      short loc_459631
4596A2  add     [ebp+var_274], 4Ch ; 'L'
4596A9  dec     [ebp+var_270]
4596AF  jnz     loc_459620
4596B5  mov     esi, [ebp+NumOfElements]
4596BB  mov     edx, [ebp+Base]
4596C1  push    offset sub_45BB40; CompareFunction
4596C6  push    4; SizeOfElements
4596C8  push    esi; NumOfElements
4596C9  push    edx; Base
4596CA  call    _qsort
4596CF  mov     ebx, [ebp+Base]
4596D5  add     esp, 10h
4596D8  xor     eax, eax
4596DA  test    esi, esi
4596DC  jz      short loc_459701
4596DE  mov     edx, [ebp+var_2B8]
4596E4  cmp     eax, [ebp+arg_C]
4596E7  jnb     short loc_459701
4596E9  mov     esi, [ebx+eax*4]
4596EC  mov     edi, edx
4596EE  inc     eax
4596EF  mov     ecx, 7
4596F4  add     edx, 1Ch
4596F7  rep movsd
4596F9  cmp     eax, [ebp+NumOfElements]
4596FF  jb      short loc_4596E4
459701  mov     eax, [ebp+var_2A8]
459707  push    eax; void *
459708  call    ??3@YAXPAX@Z; operator delete(void *)
45970D  add     esp, 4
459710  push    ebx; void *
459711  call    ??3@YAXPAX@Z; operator delete(void *)
459716  mov     esi, [ebp+NumOfElements]
45971C  add     esp, 4
45971F  mov     ecx, [ebp+var_28C]
459725  push    ecx; void *
459726  call    ??3@YAXPAX@Z; operator delete(void *)
45972B  mov     edx, [ebp+Src]
459731  add     esp, 4
459734  push    edx; void *
459735  call    ??3@YAXPAX@Z; operator delete(void *)
45973A  mov     eax, [ebp+var_2A0]
459740  add     esp, 4
459743  push    eax; void *
459744  call    ??3@YAXPAX@Z; operator delete(void *)
459749  mov     eax, [ebp+arg_C]
45974C  add     esp, 4
45974F  cmp     eax, esi
459751  ja      short loc_459755
459753  mov     esi, eax
459755  mov     ecx, [ebp+var_2C4]
45975B  mov     [ecx], esi
45975D  mov     ecx, [ebp+var_4]
459760  pop     edi
459761  pop     esi
459762  xor     ecx, ebp
459764  xor     eax, eax
459766  pop     ebx
459767  call    sub_4ABDD5
45976C  mov     esp, ebp
45976E  pop     ebp
45976F  retn    10h

;==== 4597A0 sub_4597A0
4597A0  mov     edx, [eax]
4597A2  mov     [ecx+130h], edx
4597A8  mov     edx, [eax+4]
4597AB  mov     [ecx+134h], edx
4597B1  mov     eax, [eax+8]
4597B4  mov     [ecx+138h], eax
4597BA  retn

;==== 4597C0 sub_4597C0
4597C0  push    ebp
4597C1  mov     ebp, esp
4597C3  xor     eax, eax
4597C5  push    ebx
4597C6  mov     ebx, [ebp+arg_0]
4597C9  test    ecx, ecx
4597CB  js      short loc_459811
4597CD  push    esi
4597CE  mov     esi, [ebx+20h]
4597D1  cmp     ecx, esi
4597D3  jge     short loc_459810
4597D5  test    edi, edi
4597D7  js      short loc_459810
4597D9  cmp     edi, esi
4597DB  jge     short loc_459810
4597DD  lea     eax, [edi+1]
4597E0  cdq
4597E1  idiv    esi
4597E3  cmp     ecx, edx
4597E5  jnz     short loc_4597F3
4597E7  mov     eax, [ebx+130h]
4597ED  pop     esi
4597EE  pop     ebx
4597EF  pop     ebp
4597F0  retn    4
4597F3  lea     eax, [esi+edi-1]
4597F7  cdq
4597F8  idiv    esi
4597FA  cmp     ecx, edx
4597FC  jnz     short loc_45980A
4597FE  mov     eax, [ebx+138h]
459804  pop     esi
459805  pop     ebx
459806  pop     ebp
459807  retn    4
45980A  mov     eax, [ebx+134h]
459810  pop     esi
459811  pop     ebx
459812  pop     ebp
459813  retn    4

;==== 459820 sub_459820
459820  fld     ds:dbl_4E2838
459826  fstp    qword ptr [eax+140h]
45982C  retn

;==== 459830 sub_459830
459830  push    ebp
459831  mov     ebp, esp
459833  fld     ds:dbl_4EC9F0
459839  sub     esp, 8
45983C  fstp    [esp+8+var_8]; double
45983F  fild    [ebp+arg_4]
459842  fdiv    ds:dbl_4EC978
459848  fmul    ds:dbl_4EC988
45984E  call    __CIcos
459853  sub     esp, 8
459856  fstp    [esp+10h+var_10]; double
459859  call    sub_4010B0
45985E  fld1
459860  mov     eax, [ebp+arg_0]
459863  fsub    st, st(1)
459865  add     esp, 10h
459868  fmul    qword ptr [eax+140h]
45986E  faddp   st(1), st
459870  fmul    ds:dbl_4EC930
459876  call    __ftol2_sse
45987B  pop     ebp
45987C  retn    8

;==== 459880 sub_459880
459880  mov     eax, 90000004h
459885  test    edx, edx
459887  jnz     short loc_4598B0
459889  mov     [ecx], edx
45988B  mov     eax, [esi+254h]
459891  mov     [ecx+4], eax
459894  mov     edx, [esi+258h]
45989A  mov     [ecx+8], edx
45989D  mov     eax, [esi+25Ch]
4598A3  mov     [ecx+0Ch], eax
4598A6  mov     dword ptr [ecx+10h], 0
4598AD  xor     eax, eax
4598AF  retn
4598B0  push    edi
4598B1  lea     edi, [edx-1]
4598B4  cmp     edi, 0Fh
4598B7  pop     edi
4598B8  ja      short locret_4598ED
4598BA  imul    edx, 58h ; 'X'
4598BD  cmp     dword ptr [edx+esi+25Ch], 0
4598C5  lea     edx, [edx+esi+25Ch]
4598CC  jz      short locret_4598ED
4598CE  mov     eax, [edx+4]
4598D1  mov     [ecx], eax
4598D3  mov     eax, [edx+28h]
4598D6  mov     [ecx+4], eax
4598D9  mov     eax, [edx+2Ch]
4598DC  mov     [ecx+8], eax
4598DF  mov     eax, [edx+30h]
4598E2  mov     [ecx+0Ch], eax
4598E5  mov     edx, [edx+34h]
4598E8  mov     [ecx+10h], edx
4598EB  xor     eax, eax
4598ED  retn

;==== 4598F0 sub_4598F0
4598F0  push    ebp
4598F1  mov     ebp, esp
4598F3  sub     esp, 18h
4598F6  cdq
4598F7  mov     dword ptr [ebp+var_18], eax
4598FA  mov     eax, [ebp+arg_14]
4598FD  mov     dword ptr [ebp+var_18+4], edx
459900  cdq
459901  mov     dword ptr [ebp+var_10], eax
459904  mov     eax, [ebp+arg_4]
459907  sub     eax, 0
45990A  push    ebx
45990B  mov     ebx, [ebp+arg_8]
45990E  push    esi
45990F  mov     esi, [ebp+arg_C]
459912  mov     dword ptr [ebp+var_10+4], edx
459915  jz      loc_459A6F
45991B  dec     eax
45991C  jz      short loc_45998B
45991E  dec     eax
45991F  jz      short loc_459935
459921  mov     eax, [ebp+arg_0]
459924  mov     dword ptr [eax], 0
45992A  mov     eax, 90000005h
45992F  pop     esi
459930  pop     ebx
459931  mov     esp, ebp
459933  pop     ebp
459934  retn
459935  mov     eax, [ecx+74h]
459938  add     eax, [ebx+0Ch]
45993B  add     eax, edi
45993D  cdq
45993E  mov     esi, eax
459940  mov     eax, [ecx+64h]
459943  add     eax, [ebx+8]
459946  mov     [ebp+arg_14], edx
459949  add     eax, edi
45994B  cdq
45994C  add     esi, eax
45994E  mov     eax, [ebp+arg_14]
459951  adc     eax, edx
459953  mov     [ebp+arg_14], eax
459956  mov     eax, [ecx+54h]
459959  mov     edx, ebx
45995B  add     eax, [edx+4]
45995E  add     eax, edi
459960  cdq
459961  add     esi, eax
459963  mov     eax, [ebp+arg_14]
459966  adc     eax, edx
459968  mov     [ebp+arg_14], eax
45996B  mov     eax, [ecx+44h]
45996E  mov     ecx, ebx
459970  add     eax, [ecx]
459972  mov     ecx, dword ptr [ebp+var_10]
459975  add     eax, edi
459977  cdq
459978  add     esi, eax
45997A  mov     eax, [ebp+arg_14]
45997D  adc     eax, edx
45997F  mov     edx, dword ptr [ebp+var_10+4]
459982  push    edx
459983  push    ecx
459984  push    eax
459985  push    esi
459986  jmp     loc_459B74
45998B  mov     eax, [esi+48h]
45998E  mov     [ebp+arg_14], eax
459991  cmp     eax, 1
459994  jge     short loc_45999D
459996  mov     [ebp+arg_14], 1
45999D  mov     eax, [ecx+44h]
4599A0  add     eax, [ebx]
4599A2  xor     ebx, ebx
4599A4  add     eax, edi
4599A6  imul    eax, [ecx+40h]
4599AA  cdq
4599AB  idiv    [ebp+arg_14]
4599AE  sub     eax, [esi+4Ch]
4599B1  sub     eax, [ebp+arg_10]
4599B4  cdq
4599B5  cmp     edx, ebx
4599B7  jl      short loc_4599C8
4599B9  jg      short loc_4599C0
4599BB  cmp     eax, 1
4599BE  jb      short loc_4599C8
4599C0  mov     [ebp+var_8], eax
4599C3  mov     [ebp+var_4], edx
4599C6  jmp     short loc_4599CE
4599C8  mov     [ebp+var_8], ebx
4599CB  mov     [ebp+var_4], ebx
4599CE  mov     ebx, [esi+58h]
4599D1  cmp     ebx, 1
4599D4  jge     short loc_4599DB
4599D6  mov     ebx, 1
4599DB  mov     edx, [ebp+arg_8]
4599DE  mov     eax, [ecx+54h]
4599E1  add     eax, [edx+4]
4599E4  add     eax, edi
4599E6  imul    eax, [ecx+50h]
4599EA  cdq
4599EB  idiv    ebx
4599ED  sub     eax, [esi+5Ch]
4599F0  sub     eax, [ebp+arg_10]
4599F3  cdq
4599F4  test    edx, edx
4599F6  jl      short loc_4599FF
4599F8  jg      short loc_459A03
4599FA  cmp     eax, 1
4599FD  jnb     short loc_459A03
4599FF  xor     eax, eax
459A01  xor     edx, edx
459A03  add     [ebp+var_8], eax
459A06  mov     eax, [esi+68h]
459A09  mov     ebx, eax
459A0B  adc     [ebp+var_4], edx
459A0E  cmp     eax, 1
459A11  jge     short loc_459A18
459A13  mov     ebx, 1
459A18  mov     edx, [ebp+arg_8]
459A1B  mov     eax, [ecx+64h]
459A1E  add     eax, [edx+8]
459A21  add     eax, edi
459A23  imul    eax, [ecx+60h]
459A27  cdq
459A28  idiv    ebx
459A2A  sub     eax, [esi+6Ch]
459A2D  sub     eax, [ebp+arg_10]
459A30  cdq
459A31  test    edx, edx
459A33  jl      short loc_459A3C
459A35  jg      short loc_459A40
459A37  cmp     eax, 1
459A3A  jnb     short loc_459A40
459A3C  xor     eax, eax
459A3E  xor     edx, edx
459A40  add     [ebp+var_8], eax
459A43  mov     eax, [esi+78h]
459A46  mov     ebx, eax
459A48  adc     [ebp+var_4], edx
459A4B  cmp     eax, 1
459A4E  jge     short loc_459A55
459A50  mov     ebx, 1
459A55  mov     edx, [ebp+arg_8]
459A58  mov     eax, [ecx+74h]
459A5B  add     eax, [edx+0Ch]
459A5E  add     eax, edi
459A60  imul    eax, [ecx+70h]
459A64  cdq
459A65  idiv    ebx
459A67  sub     eax, [esi+7Ch]
459A6A  jmp     loc_459B4D
459A6F  mov     eax, [esi+8]
459A72  mov     [ebp+arg_14], eax
459A75  cmp     eax, 1
459A78  jge     short loc_459A81
459A7A  mov     [ebp+arg_14], 1
459A81  mov     eax, [ecx+4]
459A84  add     eax, [ebx]
459A86  xor     ebx, ebx
459A88  add     eax, edi
459A8A  imul    eax, [ecx]
459A8D  cdq
459A8E  idiv    [ebp+arg_14]
459A91  sub     eax, [esi+0Ch]
459A94  sub     eax, [ebp+arg_10]
459A97  cdq
459A98  cmp     edx, ebx
459A9A  jl      short loc_459AAB
459A9C  jg      short loc_459AA3
459A9E  cmp     eax, 1
459AA1  jb      short loc_459AAB
459AA3  mov     [ebp+var_8], eax
459AA6  mov     [ebp+var_4], edx
459AA9  jmp     short loc_459AB1
459AAB  mov     [ebp+var_8], ebx
459AAE  mov     [ebp+var_4], ebx
459AB1  mov     ebx, [esi+18h]
459AB4  cmp     ebx, 1
459AB7  jge     short loc_459ABE
459AB9  mov     ebx, 1
459ABE  mov     edx, [ebp+arg_8]
459AC1  mov     eax, [ecx+14h]
459AC4  add     eax, [edx+4]
459AC7  add     eax, edi
459AC9  imul    eax, [ecx+10h]
459ACD  cdq
459ACE  idiv    ebx
459AD0  sub     eax, [esi+1Ch]
459AD3  sub     eax, [ebp+arg_10]
459AD6  cdq
459AD7  test    edx, edx
459AD9  jl      short loc_459AE2
459ADB  jg      short loc_459AE6
459ADD  cmp     eax, 1
459AE0  jnb     short loc_459AE6
459AE2  xor     eax, eax
459AE4  xor     edx, edx
459AE6  add     [ebp+var_8], eax
459AE9  mov     eax, [esi+28h]
459AEC  mov     ebx, eax
459AEE  adc     [ebp+var_4], edx
459AF1  cmp     eax, 1
459AF4  jge     short loc_459AFB
459AF6  mov     ebx, 1
459AFB  mov     edx, [ebp+arg_8]
459AFE  mov     eax, [ecx+24h]
459B01  add     eax, [edx+8]
459B04  add     eax, edi
459B06  imul    eax, [ecx+20h]
459B0A  cdq
459B0B  idiv    ebx
459B0D  sub     eax, [esi+2Ch]
459B10  sub     eax, [ebp+arg_10]
459B13  cdq
459B14  test    edx, edx
459B16  jl      short loc_459B1F
459B18  jg      short loc_459B23
459B1A  cmp     eax, 1
459B1D  jnb     short loc_459B23
459B1F  xor     eax, eax
459B21  xor     edx, edx
459B23  add     [ebp+var_8], eax
459B26  mov     eax, [esi+38h]
459B29  mov     ebx, eax
459B2B  adc     [ebp+var_4], edx
459B2E  cmp     eax, 1
459B31  jge     short loc_459B38
459B33  mov     ebx, 1
459B38  mov     edx, [ebp+arg_8]
459B3B  mov     eax, [ecx+34h]
459B3E  add     eax, [edx+0Ch]
459B41  add     eax, edi
459B43  imul    eax, [ecx+30h]
459B47  cdq
459B48  idiv    ebx
459B4A  sub     eax, [esi+3Ch]
459B4D  sub     eax, [ebp+arg_10]
459B50  cdq
459B51  test    edx, edx
459B53  jl      short loc_459B60
459B55  jg      short loc_459B5C
459B57  cmp     eax, 1
459B5A  jb      short loc_459B60
459B5C  mov     ecx, edx
459B5E  jmp     short loc_459B64
459B60  xor     eax, eax
459B62  xor     ecx, ecx
459B64  mov     edx, dword ptr [ebp+var_10+4]
459B67  add     eax, [ebp+var_8]
459B6A  push    edx
459B6B  mov     edx, dword ptr [ebp+var_10]
459B6E  adc     ecx, [ebp+var_4]
459B71  push    edx
459B72  push    ecx
459B73  push    eax
459B74  call    __allmul
459B79  mov     ecx, dword ptr [ebp+var_18+4]
459B7C  push    ecx
459B7D  mov     ecx, dword ptr [ebp+var_18]
459B80  push    ecx
459B81  push    edx
459B82  push    eax
459B83  call    __allmul
459B88  mov     ecx, [ebp+arg_0]
459B8B  mov     eax, edx
459B8D  pop     esi
459B8E  sar     eax, 1Fh
459B91  mov     [ecx], edx
459B93  xor     eax, eax
459B95  pop     ebx
459B96  mov     esp, ebp
459B98  pop     ebp
459B99  retn

;==== 459BA0 sub_459BA0
459BA0  mov     eax, 90000005h
459BA5  cmp     edx, 2
459BA8  ja      loc_459C9A
459BAE  jz      loc_459C71
459BB4  sub     edx, 0
459BB7  jz      short loc_459C1D
459BB9  dec     edx
459BBA  jnz     loc_459CA2
459BC0  cmp     [ecx+144h], edx
459BC6  jle     short loc_459BD0
459BC8  mov     eax, [ecx+1C4h]
459BCE  jmp     short loc_459BD2
459BD0  xor     eax, eax
459BD2  cmp     dword ptr [ecx+148h], 0
459BD9  jle     short loc_459BE3
459BDB  mov     ecx, [ecx+1C8h]
459BE1  jmp     short loc_459BE5
459BE3  xor     ecx, ecx
459BE5  sub     eax, ecx
459BE7  cmp     dword ptr [esi+14Ch], 0
459BEE  mov     [edi], eax
459BF0  jle     short loc_459BFA
459BF2  mov     eax, [esi+1CCh]
459BF8  jmp     short loc_459BFC
459BFA  xor     eax, eax
459BFC  cmp     dword ptr [esi+150h], 0
459C03  jle     short loc_459C13
459C05  mov     ecx, [esi+1D0h]
459C0B  sub     eax, ecx
459C0D  mov     [edi+4], eax
459C10  xor     eax, eax
459C12  retn
459C13  xor     ecx, ecx
459C15  sub     eax, ecx
459C17  mov     [edi+4], eax
459C1A  xor     eax, eax
459C1C  retn
459C1D  cmp     dword ptr [ecx+134h], 0
459C24  jle     short loc_459C2E
459C26  mov     eax, [ecx+1B4h]
459C2C  jmp     short loc_459C30
459C2E  xor     eax, eax
459C30  cmp     dword ptr [ecx+138h], 0
459C37  jle     short loc_459C41
459C39  mov     ecx, [ecx+1B8h]
459C3F  jmp     short loc_459C43
459C41  xor     ecx, ecx
459C43  sub     eax, ecx
459C45  cmp     dword ptr [esi+13Ch], 0
459C4C  mov     [edi], eax
459C4E  jle     short loc_459C58
459C50  mov     eax, [esi+1BCh]
459C56  jmp     short loc_459C5A
459C58  xor     eax, eax
459C5A  cmp     dword ptr [esi+140h], 0
459C61  jle     short loc_459C13
459C63  mov     ecx, [esi+1C0h]
459C69  sub     eax, ecx
459C6B  mov     [edi+4], eax
459C6E  xor     eax, eax
459C70  retn
459C71  cmp     dword ptr [ecx+144h], 0
459C78  jle     short loc_459C82
459C7A  mov     eax, [ecx+1C4h]
459C80  jmp     short loc_459C84
459C82  xor     eax, eax
459C84  cmp     dword ptr [ecx+148h], 0
459C8B  jle     short loc_459CD9
459C8D  mov     ecx, [ecx+1C8h]
459C93  sub     eax, ecx
459C95  mov     [edi], eax
459C97  xor     eax, eax
459C99  retn
459C9A  cmp     edx, 100h
459CA0  jz      short loc_459CB0
459CA2  mov     dword ptr [edi], 0
459CA8  mov     dword ptr [edi+4], 0
459CAF  retn
459CB0  cmp     dword ptr [ecx+154h], 0
459CB7  jle     short loc_459CC1
459CB9  mov     eax, [ecx+1D4h]
459CBF  jmp     short loc_459CC3
459CC1  xor     eax, eax
459CC3  cmp     dword ptr [ecx+158h], 0
459CCA  jle     short loc_459CD9
459CCC  mov     ecx, [ecx+1D8h]
459CD2  sub     eax, ecx
459CD4  mov     [edi], eax
459CD6  xor     eax, eax
459CD8  retn
459CD9  xor     ecx, ecx
459CDB  sub     eax, ecx
459CDD  mov     [edi], eax
459CDF  xor     eax, eax
459CE1  retn

;==== 459CF0 sub_459CF0
459CF0  push    ebp
459CF1  mov     ebp, esp
459CF3  sub     esp, 588h
459CF9  mov     eax, dword_4FB734
459CFE  xor     eax, ebp
459D00  mov     [ebp+var_8], eax
459D03  push    ebx
459D04  mov     ebx, [ebp+arg_0]
459D07  push    esi
459D08  push    edi
459D09  mov     eax, 90000003h
459D0E  cmp     ecx, [ebx+24h]
459D11  jnb     loc_45A40C
459D17  imul    ecx, 8DCh
459D1D  add     ecx, [ebx+28h]
459D20  mov     edi, ecx
459D22  mov     ecx, [edi]
459D24  mov     [ebp+var_564], edi
459D2A  test    ecx, ecx
459D2C  jz      loc_45A40C
459D32  mov     eax, [edi+234h]
459D38  push    0
459D3A  push    0FFFFFFFFh
459D3C  push    0FFFFFFFFh
459D3E  push    0
459D40  push    eax
459D41  mov     eax, [ebx+10h]
459D44  call    sub_453830
459D49  cmp     dword ptr [edi+254h], 1
459D50  jnz     loc_459ED0
459D56  cmp     dword ptr [ebx+24h], 0
459D5A  mov     [ebp+var_558], 0
459D64  jbe     loc_45A0E7
459D6A  lea     ecx, [ebp+var_354]
459D70  mov     [ebp+var_560], 0
459D7A  mov     [ebp+var_568], ecx
459D80  mov     eax, [ebx+28h]
459D83  add     eax, [ebp+var_560]
459D89  mov     edi, [ebp+var_564]
459D8F  mov     edx, [eax+4]
459D92  test    [edi+4], edx
459D95  jnz     loc_459E93
459D9B  mov     eax, [eax]
459D9D  test    eax, eax
459D9F  jz      loc_459E93
459DA5  mov     esi, [ebx+10h]
459DA8  push    esi
459DA9  mov     ecx, eax
459DAB  lea     edx, [ebp+var_574]
459DB1  mov     [ebp+var_578], 7FFFFFFFh
459DBB  call    sub_4536F0
459DC0  mov     eax, [ebp+var_570]
459DC6  mov     ecx, [ebp+var_574]
459DCC  push    eax
459DCD  push    ecx
459DCE  mov     ecx, [edi]
459DD0  lea     edx, [ebp+var_154]
459DD6  push    edx
459DD7  lea     eax, [ebp+var_20]
459DDA  push    eax
459DDB  mov     eax, esi
459DDD  call    sub_4538C0
459DE2  xor     eax, eax
459DE4  mov     [ebp+var_55C], eax
459DEA  lea     esi, [ebp+var_154]
459DF0  cmp     [ebp+eax*4+var_20], 0
459DF5  jl      short loc_459E38
459DF7  mov     ecx, [esi]
459DF9  mov     eax, [ebp+var_564]
459DFF  mov     edi, [esi+4]
459E02  push    ecx
459E03  mov     ecx, [eax]
459E05  mov     eax, [ebx+10h]
459E08  lea     edx, [ebp+var_57C]
459E0E  push    edx
459E0F  call    sub_453AC0
459E14  mov     eax, [ebp+var_57C]
459E1A  cmp     eax, [ebp+var_578]
459E20  jge     short loc_459E38
459E22  mov     ecx, [esi]
459E24  mov     edx, edi
459E26  mov     [ebp+var_578], eax
459E2C  mov     [ebp+var_574], ecx
459E32  mov     [ebp+var_570], edx
459E38  mov     eax, [ebp+var_55C]
459E3E  inc     eax
459E3F  add     esi, 8
459E42  mov     [ebp+var_55C], eax
459E48  cmp     eax, 6
459E4B  jb      short loc_459DF0
459E4D  mov     eax, [ebp+var_578]
459E53  cmp     eax, 7FFFFFFFh
459E58  jge     short loc_459E80
459E5A  mov     ecx, [ebp+var_558]
459E60  mov     edx, [ebp+var_574]
459E66  mov     [ebp+ecx*4+var_124], eax
459E6D  mov     eax, [ebp+var_568]
459E73  mov     ecx, [ebp+var_570]
459E79  mov     [eax], edx
459E7B  mov     [eax+4], ecx
459E7E  jmp     short loc_459EA4
459E80  mov     edx, [ebp+var_558]
459E86  mov     [ebp+edx*4+var_124], 0FFFFFFFFh
459E91  jmp     short loc_459EA4
459E93  mov     eax, [ebp+var_558]
459E99  mov     [ebp+eax*4+var_124], 0
459EA4  mov     eax, [ebp+var_558]
459EAA  add     [ebp+var_560], 8DCh
459EB4  add     [ebp+var_568], 8
459EBB  inc     eax
459EBC  mov     [ebp+var_558], eax
459EC2  cmp     eax, [ebx+24h]
459EC5  jb      loc_459D80
459ECB  jmp     loc_45A0E7
459ED0  xor     esi, esi
459ED2  cmp     [ebx+24h], esi
459ED5  jbe     short loc_459F2A
459ED7  mov     ecx, [edi+4]
459EDA  mov     edi, [ebx+28h]
459EDD  mov     [ebp+var_570], ecx
459EE3  lea     edx, [ebp+var_554]
459EE9  mov     eax, [ebp+var_570]
459EEF  test    [edi+4], eax
459EF2  jnz     short loc_459F10
459EF4  mov     ecx, [edi]
459EF6  test    ecx, ecx
459EF8  jz      short loc_459F10
459EFA  mov     eax, [ebx+10h]
459EFD  push    eax
459EFE  mov     [ebp+esi*4+var_124], 0FFFFFFFFh
459F09  call    sub_4536F0
459F0E  jmp     short loc_459F1B
459F10  mov     [ebp+esi*4+var_124], 0
459F1B  inc     esi
459F1C  add     edi, 8DCh
459F22  add     edx, 8
459F25  cmp     esi, [ebx+24h]
459F28  jb      short loc_459EE9
459F2A  mov     eax, [ebx+10h]
459F2D  lea     ecx, [ebp+var_56C]
459F33  push    ecx
459F34  lea     edx, [ebp+var_570]
459F3A  call    sub_453500
459F3F  mov     eax, [ebp+var_570]
459F45  imul    eax, [ebp+var_56C]
459F4C  xor     ecx, ecx
459F4E  mov     edx, 8
459F53  mul     edx
459F55  seto    cl
459F58  neg     ecx
459F5A  or      ecx, eax
459F5C  push    ecx; Size
459F5D  call    ??2@YAPAXI@Z; operator new(uint)
459F62  mov     esi, eax
459F64  push    200h; Size
459F69  mov     [ebp+var_56C], esi
459F6F  call    ??2@YAPAXI@Z; operator new(uint)
459F74  mov     ecx, [ebp+var_564]
459F7A  mov     ecx, [ecx]
459F7C  add     esp, 8
459F7F  mov     [ebp+var_560], eax
459F85  push    1
459F87  lea     eax, [ebp+var_570]
459F8D  push    eax
459F8E  mov     eax, [ebx+10h]
459F91  push    esi
459F92  call    sub_453B80
459F97  mov     [ebp+var_568], 0
459FA1  mov     [ebp+var_558], esi
459FA7  mov     edx, [ebp+var_570]
459FAD  cmp     [ebp+var_568], edx
459FB3  jnb     loc_45A0CC
459FB9  mov     eax, [ebp+var_558]
459FBF  mov     edi, [eax+4]
459FC2  mov     eax, [eax]
459FC4  mov     esi, [ebp+var_564]
459FCA  mov     ecx, [esi]
459FCC  push    eax
459FCD  lea     eax, [ebp+var_57C]
459FD3  push    eax
459FD4  mov     eax, [ebx+10h]
459FD7  call    sub_453AC0
459FDC  mov     eax, [esi+25Ch]
459FE2  mov     ecx, [ebx+10h]
459FE5  mov     edx, [ecx]
459FE7  push    1
459FE9  mov     edx, [edx+4]
459FEC  push    1
459FEE  push    1
459FF0  push    eax
459FF1  mov     eax, [esi+258h]
459FF7  push    eax
459FF8  mov     eax, [esi+254h]
459FFE  push    eax
459FFF  mov     eax, [ebp+var_558]
45A005  mov     eax, [eax]
45A007  push    edi
45A008  push    eax
45A009  lea     eax, [ebp+var_55C]
45A00F  push    eax
45A010  mov     eax, [ebp+var_560]
45A016  push    0
45A018  push    eax
45A019  call    edx
45A01B  mov     esi, [ebp+var_560]
45A021  xor     ecx, ecx
45A023  cmp     [ebp+var_55C], ecx
45A029  jbe     short loc_45A093
45A02B  mov     edx, [ebx+24h]
45A02E  xor     eax, eax
45A030  test    edx, edx
45A032  jz      short loc_45A08A
45A034  cmp     [ebp+eax*4+var_124], 0
45A03C  jge     short loc_45A057
45A03E  mov     edi, [ebp+eax*8+var_554]
45A045  cmp     edi, [esi+ecx*8]
45A048  jnz     short loc_45A057
45A04A  mov     edi, [ebp+eax*8+var_550]
45A051  cmp     edi, [esi+ecx*8+4]
45A055  jz      short loc_45A05E
45A057  inc     eax
45A058  cmp     eax, edx
45A05A  jb      short loc_45A034
45A05C  jmp     short loc_45A08A
45A05E  mov     edi, [ebp+var_57C]
45A064  mov     [ebp+eax*4+var_124], edi
45A06B  mov     edi, [ebp+var_558]
45A071  mov     edi, [edi]
45A073  mov     [ebp+eax*8+var_354], edi
45A07A  mov     edi, [ebp+var_558]
45A080  mov     edi, [edi+4]
45A083  mov     [ebp+eax*8+var_350], edi
45A08A  inc     ecx
45A08B  cmp     ecx, [ebp+var_55C]
45A091  jb      short loc_45A02E
45A093  mov     ecx, [ebx+24h]
45A096  xor     edx, edx
45A098  xor     eax, eax
45A09A  test    ecx, ecx
45A09C  jz      short loc_45A0B5
45A09E  mov     edi, edi
45A0A0  cmp     [ebp+eax*4+var_124], edx
45A0A7  jl      short loc_45A0B0
45A0A9  inc     eax
45A0AA  cmp     eax, ecx
45A0AC  jb      short loc_45A0A0
45A0AE  jmp     short loc_45A0B5
45A0B0  mov     edx, 1
45A0B5  inc     [ebp+var_568]
45A0BB  add     [ebp+var_558], 8
45A0C2  test    edx, edx
45A0C4  jnz     loc_459FA7
45A0CA  jmp     short loc_45A0D2
45A0CC  mov     esi, [ebp+var_560]
45A0D2  mov     eax, [ebp+var_56C]
45A0D8  push    eax; void *
45A0D9  call    ??3@YAXPAX@Z; operator delete(void *)
45A0DE  push    esi; void *
45A0DF  call    ??3@YAXPAX@Z; operator delete(void *)
45A0E4  add     esp, 8
45A0E7  mov     ecx, [ebp+var_564]
45A0ED  mov     eax, [ecx+0Ch]
45A0F0  xor     edx, edx
45A0F2  mov     [ebp+var_558], edx
45A0F8  mov     [ebp+var_560], 0FFFFFFFFh
45A102  cmp     eax, edx
45A104  jl      short loc_45A12B
45A106  mov     ecx, [ebp+eax*4+var_124]
45A10D  cmp     ecx, 1
45A110  jl      short loc_45A12B
45A112  imul    eax, 8DCh
45A118  mov     [ebp+var_558], ecx
45A11E  mov     ecx, [ebx+28h]
45A121  mov     eax, [eax+ecx+8]
45A125  mov     [ebp+var_560], eax
45A12B  mov     [ebp+var_578], edx
45A131  cmp     [ebx+24h], edx
45A134  jbe     loc_45A2F6
45A13A  lea     ecx, [ebp+var_354]
45A140  mov     [ebp+var_588], edx
45A146  mov     [ebp+var_55C], ecx
45A14C  lea     esp, [esp+0]
45A150  mov     esi, [ebx+28h]
45A153  mov     edx, [ebp+var_578]
45A159  add     esi, [ebp+var_588]
45A15F  cmp     [ebp+edx*4+var_124], 1
45A167  jl      loc_45A2CF
45A16D  mov     eax, [ebp+var_564]
45A173  mov     ecx, [eax+8]
45A176  lea     eax, [ecx+1]
45A179  cdq
45A17A  idiv    dword ptr [ebx+20h]
45A17D  mov     esi, [esi+8]
45A180  xor     eax, eax
45A182  mov     [ebp+var_570], esi
45A188  mov     edi, edx
45A18A  cmp     esi, edi
45A18C  setz    al
45A18F  mov     [ebp+var_584], eax
45A195  test    eax, eax
45A197  jnz     short loc_45A1B5
45A199  mov     esi, [ebx+20h]
45A19C  lea     eax, [esi+ecx-1]
45A1A0  cdq
45A1A1  idiv    esi
45A1A3  mov     [ebp+var_568], 1
45A1AD  cmp     [ebp+var_570], edx
45A1B3  jnz     short loc_45A1BF
45A1B5  mov     [ebp+var_568], 0
45A1BF  mov     esi, [ebx+20h]
45A1C2  xor     edx, edx
45A1C4  cmp     [ebp+var_560], edi
45A1CA  lea     eax, [esi+ecx-1]
45A1CE  setz    dl
45A1D1  mov     edi, edx
45A1D3  cdq
45A1D4  idiv    esi
45A1D6  xor     eax, eax
45A1D8  cmp     [ebp+var_560], edx
45A1DE  setz    al
45A1E1  mov     [ebp+var_56C], eax
45A1E7  test    edi, edi
45A1E9  jnz     short loc_45A1F9
45A1EB  mov     [ebp+var_580], 1
45A1F5  test    eax, eax
45A1F7  jz      short loc_45A203
45A1F9  mov     [ebp+var_580], 0
45A203  mov     esi, [ebp+var_578]
45A209  mov     eax, [ebp+esi*4+var_124]
45A210  mov     ecx, [ebp+var_558]
45A216  cmp     eax, ecx
45A218  jge     loc_45A29C
45A21E  xor     edx, edx
45A220  lea     ecx, [eax+eax]
45A223  cmp     ecx, [ebp+var_558]
45A229  setle   dl
45A22C  cmp     [ebp+var_584], 0
45A233  mov     ecx, edx
45A235  jz      short loc_45A23F
45A237  test    edi, edi
45A239  jz      short loc_45A268
45A23B  test    ecx, ecx
45A23D  jnz     short loc_45A268
45A23F  cmp     [ebp+var_568], 0
45A246  jz      short loc_45A25E
45A248  cmp     [ebp+var_56C], 0
45A24F  jnz     short loc_45A268
45A251  cmp     [ebp+var_580], 0
45A258  jz      short loc_45A25E
45A25A  test    ecx, ecx
45A25C  jnz     short loc_45A268
45A25E  mov     ecx, [ebp+var_558]
45A264  test    ecx, ecx
45A266  jnz     short loc_45A2D5
45A268  mov     ecx, [ebp+var_55C]
45A26E  mov     edx, [ecx]
45A270  mov     ecx, [ecx+4]
45A273  mov     [ebp+var_558], eax
45A279  mov     eax, [ebp+var_570]
45A27F  mov     [ebp+var_560], eax
45A285  mov     eax, [ebp+var_564]
45A28B  mov     [eax+0Ch], esi
45A28E  mov     [eax+840h], edx
45A294  mov     [eax+844h], ecx
45A29A  jmp     short loc_45A2D5
45A29C  mov     edx, [ebp+var_564]
45A2A2  mov     edx, [edx+238h]
45A2A8  add     edx, ecx
45A2AA  cmp     eax, edx
45A2AC  jge     short loc_45A264
45A2AE  cmp     [ebp+var_584], 0
45A2B5  jz      short loc_45A2BB
45A2B7  test    edi, edi
45A2B9  jz      short loc_45A268
45A2BB  cmp     [ebp+var_568], 0
45A2C2  jz      short loc_45A264
45A2C4  cmp     [ebp+var_56C], 0
45A2CB  jnz     short loc_45A268
45A2CD  jmp     short loc_45A264
45A2CF  mov     esi, [ebp+var_578]
45A2D5  add     [ebp+var_588], 8DCh
45A2DF  add     [ebp+var_55C], 8
45A2E6  inc     esi
45A2E7  mov     [ebp+var_578], esi
45A2ED  cmp     esi, [ebx+24h]
45A2F0  jb      loc_45A150
45A2F6  mov     eax, [ebp+var_564]
45A2FC  cmp     dword ptr [eax+0Ch], 0
45A300  jl      loc_45A40A
45A306  mov     edx, [eax+844h]
45A30C  mov     ecx, [eax+840h]
45A312  push    edx
45A313  push    ecx
45A314  mov     ecx, [eax]
45A316  mov     eax, [ebx+10h]
45A319  lea     edx, [ebp+var_55C]
45A31F  push    edx
45A320  push    0
45A322  call    sub_453930
45A327  test    eax, eax
45A329  jnz     loc_45A40A
45A32F  mov     eax, [ebp+var_55C]
45A335  xor     ecx, ecx
45A337  mov     edx, 8
45A33C  mul     edx
45A33E  seto    cl
45A341  neg     ecx
45A343  or      ecx, eax
45A345  push    ecx; Size
45A346  call    ??2@YAPAXI@Z; operator new(uint)
45A34B  mov     ecx, [ebp+var_564]
45A351  mov     edx, [ecx+844h]
45A357  add     esp, 4
45A35A  push    edx
45A35B  mov     edx, [ecx+840h]
45A361  push    edx
45A362  lea     edx, [ebp+var_55C]
45A368  push    edx
45A369  push    eax
45A36A  mov     [ebp+var_56C], eax
45A370  mov     eax, [ebx+10h]
45A373  push    eax
45A374  mov     eax, [ecx]
45A376  call    sub_4539B0
45A37B  mov     ecx, [ebp+var_55C]
45A381  test    ecx, ecx
45A383  jle     short loc_45A3FB
45A385  mov     eax, [ebp+var_564]
45A38B  mov     edx, [eax]
45A38D  mov     eax, [eax+238h]
45A393  mov     esi, [ebp+var_56C]
45A399  mov     [ebp+var_584], edx
45A39F  mov     [ebp+var_580], eax
45A3A5  mov     [ebp+var_570], ecx
45A3AB  jmp     short loc_45A3B0
45A3B0  mov     ecx, [esi]
45A3B2  mov     edi, [esi+4]
45A3B5  mov     eax, [ebx+10h]
45A3B8  push    ecx
45A3B9  mov     ecx, [ebp+var_584]
45A3BF  lea     edx, [ebp+var_57C]
45A3C5  push    edx
45A3C6  call    sub_453AC0
45A3CB  mov     eax, [ebp+var_580]
45A3D1  cmp     [ebp+var_57C], eax
45A3D7  jg      short loc_45A3F0
45A3D9  mov     ecx, [esi]
45A3DB  mov     eax, [ebp+var_564]
45A3E1  mov     [eax+848h], ecx
45A3E7  mov     edx, [esi+4]
45A3EA  mov     [eax+84Ch], edx
45A3F0  add     esi, 8
45A3F3  dec     [ebp+var_570]
45A3F9  jnz     short loc_45A3B0
45A3FB  mov     eax, [ebp+var_56C]
45A401  push    eax; void *
45A402  call    ??3@YAXPAX@Z; operator delete(void *)
45A407  add     esp, 4
45A40A  xor     eax, eax
45A40C  mov     ecx, [ebp+var_8]
45A40F  pop     edi
45A410  pop     esi
45A411  xor     ecx, ebp
45A413  pop     ebx
45A414  call    sub_4ABDD5
45A419  mov     esp, ebp
45A41B  pop     ebp
45A41C  retn    4

;==== 45A420 sub_45A420
45A420  push    ebp
45A421  mov     ebp, esp
45A423  push    ecx
45A424  push    esi
45A425  push    edi
45A426  mov     edi, [ebp+arg_0]
45A429  xor     eax, eax
45A42B  test    edi, edi
45A42D  jnz     short loc_45A437
45A42F  mov     esi, [edx+28h]
45A432  mov     [ebp+arg_0], esi
45A435  mov     edi, esi
45A437  mov     edx, [edx+24h]
45A43A  xor     esi, esi
45A43C  mov     [ebp+var_4], edx
45A43F  test    edx, edx
45A441  jz      short loc_45A49D
45A443  push    ebx
45A444  sub     ecx, edi
45A446  lea     ebx, [edi+834h]
45A44C  lea     edi, [ecx-834h]
45A452  mov     edx, 8
45A457  mov     ecx, ebx
45A459  lea     esp, [esp+0]
45A460  cmp     edx, 4
45A463  jb      short loc_45A491
45A465  mov     eax, [edi+ecx]
45A468  cmp     eax, [ecx]
45A46A  jnz     short loc_45A474
45A46C  sub     edx, 4
45A46F  add     ecx, 4
45A472  jmp     short loc_45A460
45A474  inc     esi
45A475  sub     edi, 8DCh
45A47B  add     ebx, 8DCh
45A481  cmp     esi, [ebp+var_4]
45A484  jb      short loc_45A452
45A486  pop     ebx
45A487  pop     edi
45A488  xor     eax, eax
45A48A  pop     esi
45A48B  mov     esp, ebp
45A48D  pop     ebp
45A48E  retn    4
45A491  imul    esi, 8DCh
45A497  add     esi, [ebp+arg_0]
45A49A  pop     ebx
45A49B  mov     eax, esi
45A49D  pop     edi
45A49E  pop     esi
45A49F  mov     esp, ebp
45A4A1  pop     ebp
45A4A2  retn    4

;==== 45A4B0 sub_45A4B0
45A4B0  push    ebp
45A4B1  mov     ebp, esp
45A4B3  sub     esp, 8
45A4B6  mov     edx, [ebp+arg_4]
45A4B9  mov     [ebp+var_4], ecx
45A4BC  push    edx
45A4BD  mov     edx, [ebp+arg_0]
45A4C0  lea     ecx, [ebp+var_8]
45A4C3  mov     [ebp+var_8], eax
45A4C6  call    sub_45A420
45A4CB  mov     esp, ebp
45A4CD  pop     ebp
45A4CE  retn    8

;==== 45A4E0 sub_45A4E0
45A4E0  push    ebp
45A4E1  mov     ebp, esp
45A4E3  push    ebx
45A4E4  mov     ebx, [ebp+arg_0]
45A4E7  push    esi
45A4E8  mov     esi, eax
45A4EA  or      eax, 0FFFFFFFFh
45A4ED  push    edi
45A4EE  test    esi, esi
45A4F0  jnz     short loc_45A4F5
45A4F2  mov     esi, [edx+28h]
45A4F5  mov     edi, [edx+24h]
45A4F8  xor     ecx, ecx
45A4FA  test    edi, edi
45A4FC  jz      short loc_45A51E
45A4FE  mov     edi, edi
45A500  mov     edx, [esi]
45A502  test    edx, edx
45A504  jz      short loc_45A50A
45A506  cmp     [ebx], edx
45A508  jz      short loc_45A51C
45A50A  inc     ecx
45A50B  add     esi, 8DCh
45A511  cmp     ecx, edi
45A513  jb      short loc_45A500
45A515  pop     edi
45A516  pop     esi
45A517  pop     ebx
45A518  pop     ebp
45A519  retn    4
45A51C  mov     eax, ecx
45A51E  pop     edi
45A51F  pop     esi
45A520  pop     ebx
45A521  pop     ebp
45A522  retn    4

;==== 45A530 sub_45A530
45A530  push    ebp
45A531  mov     ebp, esp
45A533  and     esp, 0FFFFFFF8h
45A536  sub     esp, 754h
45A53C  mov     eax, dword_4FB734
45A541  xor     eax, esp
45A543  mov     [esp+754h+var_4], eax
45A54A  push    ebx
45A54B  mov     ebx, [ebp+arg_0]
45A54E  push    esi
45A54F  push    edi
45A550  mov     edi, ecx
45A552  mov     ecx, [edi+0Ch]
45A555  xor     esi, esi
45A557  mov     [esp+760h+var_73C], edi
45A55B  mov     [esp+760h+var_724], esi
45A55F  call    sub_446A10
45A564  mov     ecx, [edi+14Ch]
45A56A  cmp     ecx, [edi+148h]
45A570  jnb     short loc_45A588
45A572  mov     esi, ecx
45A574  imul    esi, 4Ch ; 'L'
45A577  add     esi, [edi+150h]
45A57D  inc     ecx
45A57E  mov     [esp+760h+var_724], esi
45A582  mov     [edi+14Ch], ecx
45A588  push    eax
45A589  mov     eax, [edi+0Ch]
45A58C  call    sub_446A30
45A591  test    esi, esi
45A593  jz      loc_45BAE6
45A599  mov     eax, [edi+14h]
45A59C  mov     ecx, [eax+ebx*4]
45A59F  mov     eax, [edi+24h]
45A5A2  mov     [esp+760h+var_72C], ecx
45A5A6  xor     ecx, ecx
45A5A8  mov     edx, 8DCh
45A5AD  mul     edx
45A5AF  seto    cl
45A5B2  neg     ecx
45A5B4  or      ecx, eax
45A5B6  push    ecx; Size
45A5B7  call    ??2@YAPAXI@Z; operator new(uint)
45A5BC  mov     ecx, [edi+28h]
45A5BF  mov     ebx, eax
45A5C1  mov     eax, [edi+24h]
45A5C4  imul    eax, 8DCh
45A5CA  add     esp, 4
45A5CD  push    eax; Size
45A5CE  push    ecx; Src
45A5CF  push    ebx; void *
45A5D0  mov     [esp+76Ch+var_6FC], ebx
45A5D4  call    _memcpy_0
45A5D9  mov     edx, [edi+2Ch]
45A5DC  mov     ecx, [esp+76Ch+var_6FC]
45A5E0  imul    edx, 8DCh
45A5E6  mov     eax, [ebx+edx+0Ch]
45A5EA  add     ebx, edx
45A5EC  add     esp, 0Ch
45A5EF  test    eax, eax
45A5F1  js      short loc_45A601
45A5F3  imul    eax, 8DCh
45A5F9  add     eax, ecx
45A5FB  mov     [esp+760h+var_70C], eax
45A5FF  jmp     short loc_45A609
45A601  mov     [esp+760h+var_70C], 0
45A609  mov     eax, [esi]
45A60B  mov     [ebx+834h], eax
45A611  mov     edx, [edi+2Ch]
45A614  mov     eax, [esi+4]
45A617  imul    edx, 8DCh
45A61D  mov     [edx+ecx+838h], eax
45A624  cmp     dword ptr [edi+24h], 0
45A628  mov     [esp+760h+var_71C], 0
45A630  jbe     short loc_45A679
45A632  mov     [esp+760h+var_730], ecx
45A636  mov     ecx, [ecx]
45A638  test    ecx, ecx
45A63A  jz      short loc_45A65D
45A63C  mov     eax, [esp+760h+var_730]
45A640  mov     esi, [eax+838h]
45A646  mov     edi, [eax+834h]
45A64C  mov     edx, [esp+760h+var_72C]
45A650  call    sub_4536B0
45A655  mov     esi, [esp+760h+var_724]
45A659  mov     edi, [esp+760h+var_73C]
45A65D  mov     eax, [esp+760h+var_71C]
45A661  mov     ecx, [esp+760h+var_730]
45A665  inc     eax
45A666  add     ecx, 8DCh
45A66C  mov     [esp+760h+var_71C], eax
45A670  mov     [esp+760h+var_730], ecx
45A674  cmp     eax, [edi+24h]
45A677  jb      short loc_45A636
45A679  mov     ecx, [edi+24h]
45A67C  add     ecx, ecx
45A67E  add     ecx, ecx
45A680  push    ecx; Size
45A681  lea     edx, [edi+30h]
45A684  push    edx; Src
45A685  lea     eax, [esp+768h+Src]
45A68C  push    eax; void *
45A68D  call    _memcpy_0
45A692  mov     eax, [esi+8]
45A695  mov     ecx, [esi+2Ch]
45A698  add     esp, 0Ch
45A69B  mov     [esp+760h+var_748], 0
45A6A3  mov     [esp+760h+var_700], ecx
45A6A7  mov     [esp+760h+var_708], 0FFFFFFFFh
45A6AF  test    eax, eax
45A6B1  jnz     short loc_45A6CC
45A6B3  mov     edx, [ebx+250h]
45A6B9  mov     eax, [ebx+24Ch]
45A6BF  add     [esp+760h+var_700], eax
45A6C3  mov     [esp+760h+var_748], edx
45A6C7  jmp     loc_45AC9C
45A6CC  cmp     eax, 6
45A6CF  jnz     loc_45A7E3
45A6D5  mov     ecx, [esp+760h+var_6FC]
45A6D9  mov     eax, [esp+760h+var_724]
45A6DD  mov     edx, [esp+760h+var_73C]
45A6E1  push    ecx
45A6E2  mov     ecx, [eax+14h]
45A6E5  mov     eax, [eax+10h]
45A6E8  push    edx
45A6E9  call    sub_45A4B0
45A6EE  mov     edx, [ebx+260h]
45A6F4  mov     esi, eax
45A6F6  mov     ecx, ebx
45A6F8  lea     edi, [esp+760h+var_714]
45A6FC  call    sub_459BA0
45A701  mov     eax, [esp+760h+var_724]
45A705  mov     ecx, [eax+4]
45A708  mov     edx, [eax]
45A70A  push    ecx
45A70B  mov     ecx, [esi]
45A70D  push    edx
45A70E  mov     edx, [esp+768h+var_72C]
45A712  lea     eax, [esp+768h+var_718]
45A716  push    eax
45A717  call    sub_453BE0
45A71C  mov     ecx, [esp+760h+var_718]
45A720  mov     edi, [esp+760h+var_73C]
45A724  push    ecx
45A725  push    edi
45A726  call    sub_459830
45A72B  mov     ecx, [esi+8]
45A72E  push    eax
45A72F  push    edi
45A730  mov     edi, [ebx+8]
45A733  call    sub_4597C0
45A738  mov     edx, [esp+764h+var_710]
45A73C  mov     edi, [esp+764h+var_714]
45A740  push    edx
45A741  lea     ecx, [esi+2Ch]
45A744  push    ecx
45A745  lea     edx, [ebx+264h]
45A74B  push    edx
45A74C  mov     edx, [ebx+260h]
45A752  push    edx
45A753  lea     edx, [esp+774h+var_738]
45A757  lea     ecx, [ebx+2Ch]
45A75A  push    edx
45A75B  call    sub_4598F0
45A760  mov     eax, [esi+1Ch]
45A763  mov     ecx, [esp+778h+var_738]
45A767  add     esp, 18h
45A76A  cmp     ecx, eax
45A76C  jle     short loc_45A774
45A76E  mov     ecx, eax
45A770  mov     [esp+760h+var_738], ecx
45A774  sub     [esi+1Ch], ecx
45A777  cmp     dword ptr [esi+1Ch], 0
45A77B  jg      short loc_45A785
45A77D  mov     edx, [ebx+284h]
45A783  jmp     short loc_45A787
45A785  xor     edx, edx
45A787  mov     eax, [ebx+280h]
45A78D  imul    eax, ecx
45A790  add     eax, [ebx+27Ch]
45A796  add     eax, edx
45A798  mov     [esp+760h+var_748], eax
45A79C  mov     eax, [esp+760h+var_724]
45A7A0  cmp     dword ptr [eax+24h], 0
45A7A4  jz      short loc_45A7AE
45A7A6  mov     ecx, [ebx+278h]
45A7AC  jmp     short loc_45A7B0
45A7AE  xor     ecx, ecx
45A7B0  mov     esi, [eax+4]
45A7B3  mov     edx, [ebx+274h]
45A7B9  push    esi
45A7BA  mov     esi, [eax]
45A7BC  add     edx, ecx
45A7BE  add     [esp+764h+var_700], edx
45A7C2  mov     ecx, [esp+764h+var_72C]
45A7C6  mov     edx, [ecx]
45A7C8  mov     edx, [edx+0Ch]
45A7CB  push    esi
45A7CC  mov     esi, [eax+14h]
45A7CF  mov     eax, [eax+10h]
45A7D2  push    esi
45A7D3  push    eax
45A7D4  call    edx
45A7D6  mov     esi, [esp+760h+var_724]
45A7DA  mov     edi, [esp+760h+var_73C]
45A7DE  jmp     loc_45AC95
45A7E3  cmp     eax, 7
45A7E6  jnz     loc_45AC9C
45A7EC  mov     eax, [esi+0Ch]
45A7EF  imul    eax, 58h ; 'X'
45A7F2  lea     eax, [eax+ebx+2B4h]
45A7F9  mov     [esp+760h+var_750], eax
45A7FD  mov     eax, [eax+34h]
45A800  mov     [esp+760h+Size], eax
45A804  test    eax, eax
45A806  js      loc_45AC53
45A80C  jnz     short loc_45A82C
45A80E  mov     ecx, [esi+10h]
45A811  mov     edx, [esi+14h]
45A814  mov     [esp+760h+var_408], ecx
45A81B  mov     [esp+760h+var_404], edx
45A822  mov     [esp+760h+var_708], 1
45A82A  jmp     short loc_45A874
45A82C  mov     eax, [esi+10h]
45A82F  cmp     eax, [esi]
45A831  jnz     short loc_45A83F
45A833  mov     eax, [esi+14h]
45A836  cmp     eax, [esi+4]
45A839  jnz     short loc_45A83F
45A83B  xor     eax, eax
45A83D  jmp     short loc_45A844
45A83F  mov     eax, 1
45A844  push    eax
45A845  mov     eax, [esp+764h+Size]
45A849  mov     ecx, [esp+764h+var_72C]
45A84D  mov     edx, [ecx]
45A84F  mov     edx, [edx+4]
45A852  push    0
45A854  push    1
45A856  push    0
45A858  push    0
45A85A  push    eax
45A85B  mov     eax, [esi+14h]
45A85E  push    eax
45A85F  mov     eax, [esi+10h]
45A862  push    eax
45A863  lea     eax, [esp+780h+var_708]
45A867  push    eax
45A868  push    0
45A86A  lea     eax, [esp+788h+var_408]
45A871  push    eax
45A872  call    edx
45A874  cmp     [esp+760h+var_708], 0
45A879  mov     [esp+760h+var_720], 0
45A881  jbe     loc_45AC5D
45A887  jmp     short loc_45A890
45A890  mov     edx, [esp+760h+var_6FC]
45A894  mov     eax, [esp+760h+var_720]
45A898  mov     ecx, [esp+eax*8+760h+var_404]
45A89F  mov     eax, [esp+eax*8+760h+var_408]
45A8A6  push    edx
45A8A7  mov     edx, [esp+764h+var_73C]
45A8AB  push    edx
45A8AC  call    sub_45A4B0
45A8B1  mov     [esp+760h+var_744], eax
45A8B5  mov     esi, eax
45A8B7  mov     eax, [esp+760h+var_750]
45A8BB  mov     edx, [eax+0Ch]
45A8BE  mov     ecx, ebx
45A8C0  lea     edi, [esp+760h+var_714]
45A8C4  call    sub_459BA0
45A8C9  mov     ecx, [esp+760h+var_750]
45A8CD  cmp     dword ptr [ecx+4], 0
45A8D1  mov     eax, 10000h
45A8D6  jnz     short loc_45A930
45A8D8  mov     eax, [esp+760h+var_724]
45A8DC  mov     ecx, [eax+10h]
45A8DF  mov     edx, [esp+760h+var_720]
45A8E3  cmp     [esp+edx*8+760h+var_408], ecx
45A8EA  jnz     short loc_45A906
45A8EC  mov     edx, [esp+edx*8+760h+var_404]
45A8F3  cmp     edx, [eax+14h]
45A8F6  jnz     short loc_45A906
45A8F8  mov     edx, [eax+4]
45A8FB  mov     eax, [eax]
45A8FD  push    edx
45A8FE  push    eax
45A8FF  lea     ecx, [esp+768h+var_718]
45A903  push    ecx
45A904  jmp     short loc_45A910
45A906  mov     edx, [eax+14h]
45A909  push    edx
45A90A  push    ecx
45A90B  lea     eax, [esp+768h+var_718]
45A90F  push    eax
45A910  mov     esi, [esp+76Ch+var_744]
45A914  mov     ecx, [esi]
45A916  mov     edx, [esp+76Ch+var_72C]
45A91A  call    sub_453BE0
45A91F  mov     ecx, [esp+760h+var_718]
45A923  mov     edx, [esp+760h+var_73C]
45A927  push    ecx
45A928  push    edx
45A929  call    sub_459830
45A92E  jmp     short loc_45A934
45A930  mov     esi, [esp+760h+var_744]
45A934  mov     ecx, [esi+8]
45A937  mov     edi, [ebx+8]
45A93A  push    eax
45A93B  mov     eax, [esp+764h+var_73C]
45A93F  push    eax
45A940  call    sub_4597C0
45A945  mov     ecx, [esp+764h+var_710]
45A949  mov     edi, [esp+764h+var_714]
45A94D  push    ecx
45A94E  lea     edx, [esi+2Ch]
45A951  push    edx
45A952  mov     edx, [esp+76Ch+var_750]
45A956  lea     ecx, [edx+10h]
45A959  push    ecx
45A95A  mov     ecx, [edx+0Ch]
45A95D  push    ecx
45A95E  lea     edx, [esp+774h+var_738]
45A962  push    edx
45A963  lea     ecx, [ebx+2Ch]
45A966  call    sub_4598F0
45A96B  mov     eax, [esi+4]
45A96E  add     esp, 18h
45A971  test    [ebx+4], eax
45A974  mov     eax, [esp+760h+var_750]
45A978  jnz     short loc_45A97F
45A97A  add     eax, 4Ch ; 'L'
45A97D  jmp     short loc_45A982
45A97F  add     eax, 40h ; '@'
45A982  mov     ecx, [esp+760h+var_750]
45A986  mov     ecx, [ecx+4]
45A989  cmp     ecx, 4; switch 5 cases
45A98C  ja      def_45A992; jumptable 0045A992 default case
45A992  jmp     ds:jpt_45A992[ecx*4]; switch jump
45A999  mov     edx, [esp+760h+var_744]; jumptable 0045A992 case 0
45A99D  mov     esi, [edx+1Ch]
45A9A0  mov     ecx, [esp+760h+var_738]
45A9A4  cmp     ecx, esi
45A9A6  jle     short loc_45A9AE
45A9A8  mov     ecx, esi
45A9AA  mov     [esp+760h+var_738], ecx
45A9AE  sub     [edx+1Ch], ecx
45A9B1  cmp     dword ptr [edx+1Ch], 0
45A9B5  jg      short loc_45A9CD
45A9B7  mov     esi, [eax+4]
45A9BA  mov     edx, [eax+8]
45A9BD  imul    esi, ecx
45A9C0  add     esi, edx
45A9C2  add     esi, [eax]
45A9C4  add     [esp+760h+var_748], esi
45A9C8  jmp     def_45A992; jumptable 0045A992 default case
45A9CD  mov     esi, [eax+4]
45A9D0  imul    esi, ecx
45A9D3  xor     edx, edx
45A9D5  add     esi, edx
45A9D7  add     esi, [eax]
45A9D9  add     [esp+760h+var_748], esi
45A9DD  jmp     def_45A992; jumptable 0045A992 default case
45A9E2  mov     esi, [esp+760h+var_744]; jumptable 0045A992 case 1
45A9E6  mov     edx, [esi+20h]
45A9E9  sub     edx, [esi+1Ch]
45A9EC  mov     ecx, [esp+760h+var_738]
45A9F0  cmp     ecx, edx
45A9F2  jle     short loc_45A9FA
45A9F4  mov     ecx, edx
45A9F6  mov     [esp+760h+var_738], ecx
45A9FA  add     [esi+1Ch], ecx
45A9FD  mov     edx, [eax+4]
45AA00  imul    edx, ecx
45AA03  add     edx, [eax]
45AA05  add     [esp+760h+var_748], edx
45AA09  jmp     def_45A992; jumptable 0045A992 default case
45AA0E  mov     esi, [esp+760h+var_750]; jumptable 0045A992 case 2
45AA12  mov     ecx, [esi+8]
45AA15  mov     edi, [esp+760h+var_744]
45AA19  mov     ecx, [edi+ecx*4+134h]
45AA20  mov     edx, [esp+760h+var_738]
45AA24  cmp     edx, ecx
45AA26  jle     short loc_45AA2E
45AA28  mov     edx, ecx
45AA2A  mov     [esp+760h+var_738], edx
45AA2E  mov     ecx, [esi+8]
45AA31  sub     [edi+ecx*4+134h], edx
45AA38  lea     ecx, [edi+ecx*4+134h]
45AA3F  mov     ecx, [eax+4]
45AA42  imul    ecx, edx
45AA45  add     ecx, [eax]
45AA47  mov     eax, [esi+8]
45AA4A  add     [esp+760h+var_748], ecx
45AA4E  cmp     eax, 8
45AA51  jl      def_45A992; jumptable 0045A992 default case
45AA57  cmp     eax, 9
45AA5A  jle     short loc_45AAAA
45AA5C  cmp     eax, 1Fh
45AA5F  jmp     short loc_45AAA4
45AA61  mov     ecx, [esp+760h+var_750]; jumptable 0045A992 cases 3,4
45AA65  mov     edx, [ecx+8]
45AA68  mov     edi, [ecx+20h]
45AA6B  mov     esi, [esp+760h+var_744]
45AA6F  mov     [esi+edx*4+134h], edi
45AA76  mov     edi, [ecx+8]
45AA79  mov     edx, [esp+760h+var_738]
45AA7D  mov     [esi+edi*4+1B4h], edx
45AA84  mov     esi, [eax+4]
45AA87  mov     ecx, [ecx+8]
45AA8A  imul    esi, edx
45AA8D  add     esi, [eax]
45AA8F  add     [esp+760h+var_748], esi
45AA93  cmp     ecx, 8
45AA96  jl      def_45A992; jumptable 0045A992 default case
45AA9C  cmp     ecx, 9
45AA9F  jle     short loc_45AAAA
45AAA1  cmp     ecx, 1Fh
45AAA4  jnz     def_45A992; jumptable 0045A992 default case
45AAAA  mov     eax, [esp+760h+var_744]
45AAAE  mov     ecx, [eax]
45AAB0  cmp     ecx, [ebx]
45AAB2  jz      def_45A992; jumptable 0045A992 default case
45AAB8  xor     esi, esi
45AABA  mov     ecx, eax
45AABC  mov     edx, 100h
45AAC1  lea     edi, [esp+760h+var_714]
45AAC5  call    sub_459BA0
45AACA  mov     eax, [esp+760h+var_744]
45AACE  mov     esi, [eax+12Ch]
45AAD4  mov     eax, [eax+1B0h]
45AADA  sub     eax, [esp+760h+var_714]
45AADE  mov     edx, 0
45AAE3  add     eax, esi
45AAE5  sets    dl
45AAE8  dec     edx
45AAE9  and     eax, edx
45AAEB  mov     [esp+760h+Size], eax
45AAEF  cmp     eax, esi
45AAF1  jz      def_45A992; jumptable 0045A992 default case
45AAF7  mov     eax, [esp+760h+var_744]
45AAFB  mov     edx, [esp+760h+var_73C]
45AAFF  push    eax
45AB00  mov     eax, [esp+764h+var_6FC]
45AB04  call    sub_45A4E0
45AB09  mov     [esp+760h+var_71C], eax
45AB0D  or      esi, 0FFFFFFFFh
45AB10  xor     edi, edi
45AB12  xor     eax, eax
45AB14  mov     [esp+760h+var_734], esi
45AB18  mov     [esp+760h+var_730], edi
45AB1C  cmp     dword ptr [esp+760h+Src], eax
45AB23  jl      def_45A992; jumptable 0045A992 default case
45AB29  mov     edx, [esp+760h+Size]
45AB2D  lea     ecx, [esp+760h+Src]
45AB34  mov     ecx, [ecx]
45AB36  cmp     [esp+760h+var_71C], ecx
45AB3A  jnz     short loc_45AB4C
45AB3C  test    esi, esi
45AB3E  jns     short loc_45AB8C
45AB40  or      edi, 0FFFFFFFFh
45AB43  lea     esi, [eax+1]
45AB46  mov     [esp+760h+var_730], edi
45AB4A  jmp     short loc_45AB78
45AB4C  test    edi, edi
45AB4E  jg      short loc_45AB78
45AB50  mov     edi, [esp+760h+var_6FC]
45AB54  imul    ecx, 8DCh
45AB5A  cmp     edx, [ecx+edi+12Ch]
45AB61  jge     short loc_45AB74
45AB63  test    esi, esi
45AB65  jns     short loc_45AB95
45AB67  mov     edi, 1
45AB6C  mov     esi, eax
45AB6E  mov     [esp+760h+var_730], edi
45AB72  jmp     short loc_45AB78
45AB74  mov     edi, [esp+760h+var_730]
45AB78  inc     eax
45AB79  cmp     dword ptr [esp+eax*4+760h+Src], 0
45AB81  lea     ecx, [esp+eax*4+760h+Src]
45AB88  jge     short loc_45AB34
45AB8A  jmp     short loc_45ABA0
45AB8C  lea     ecx, [eax-1]
45AB8F  mov     [esp+760h+var_734], ecx
45AB93  jmp     short loc_45ABA0
45AB95  mov     edi, [esp+760h+var_730]
45AB99  lea     edx, [eax-1]
45AB9C  mov     [esp+760h+var_734], edx
45ABA0  mov     ecx, [esp+760h+var_734]
45ABA4  cmp     esi, ecx
45ABA6  jz      def_45A992; jumptable 0045A992 default case
45ABAC  test    ecx, ecx
45ABAE  jns     short loc_45ABB7
45ABB0  lea     ecx, [eax-1]
45ABB3  mov     [esp+760h+var_734], ecx
45ABB7  lea     eax, ds:4[eax*4]
45ABBE  push    eax; Size
45ABBF  lea     edx, [esp+764h+Src]
45ABC6  mov     [esp+764h+Size], eax
45ABCA  push    edx; Src
45ABCB  lea     eax, [esp+768h+var_208]
45ABD2  push    eax; void *
45ABD3  call    _memcpy_0
45ABD8  add     esp, 0Ch
45ABDB  mov     eax, esi
45ABDD  cmp     edi, 1
45ABE0  jz      short loc_45ABE6
45ABE2  mov     eax, [esp+760h+var_734]
45ABE6  mov     ecx, [esp+760h+var_71C]
45ABEA  mov     [esp+eax*4+760h+var_208], ecx
45ABF1  mov     eax, [esp+760h+var_734]
45ABF5  sub     eax, esi
45ABF7  lea     edx, ds:4[eax*4]
45ABFE  push    edx; Size
45ABFF  lea     eax, [esp+esi*4+764h+Src]
45AC06  add     edi, esi
45AC08  push    eax; Src
45AC09  lea     ecx, [esp+edi*4+768h+var_208]
45AC10  push    ecx; void *
45AC11  call    _memcpy_0
45AC16  mov     edx, [esp+76Ch+Size]
45AC1A  add     esp, 0Ch
45AC1D  push    edx; Size
45AC1E  lea     eax, [esp+764h+var_208]
45AC25  push    eax; Src
45AC26  lea     ecx, [esp+768h+Src]
45AC2D  push    ecx; void *
45AC2E  call    _memcpy_0
45AC33  add     esp, 0Ch
45AC36  mov     eax, [esp+760h+var_720]; jumptable 0045A992 default case
45AC3A  inc     eax
45AC3B  mov     [esp+760h+var_720], eax
45AC3F  cmp     eax, [esp+760h+var_708]
45AC43  jb      loc_45A890
45AC49  mov     esi, [esp+760h+var_724]
45AC4D  mov     edi, [esp+760h+var_73C]
45AC51  jmp     short loc_45AC5D
45AC53  mov     edx, [ebx+27Ch]
45AC59  mov     [esp+760h+var_748], edx
45AC5D  cmp     dword ptr [esi+24h], 0
45AC61  jz      short loc_45AC6C
45AC63  mov     eax, [esp+760h+var_750]
45AC67  mov     eax, [eax+3Ch]
45AC6A  jmp     short loc_45AC6E
45AC6C  xor     eax, eax
45AC6E  mov     ecx, [esp+760h+var_750]
45AC72  mov     edx, [ecx+38h]
45AC75  mov     ecx, [esp+760h+var_72C]
45AC79  add     edx, eax
45AC7B  add     [esp+760h+var_700], edx
45AC7F  mov     edx, [esi+4]
45AC82  mov     eax, [ecx]
45AC84  mov     eax, [eax+0Ch]
45AC87  push    edx
45AC88  mov     edx, [esi]
45AC8A  push    edx
45AC8B  mov     edx, [esi+14h]
45AC8E  push    edx
45AC8F  mov     edx, [esi+10h]
45AC92  push    edx
45AC93  call    eax
45AC95  sub     eax, 2
45AC98  mov     [esp+760h+var_708], eax
45AC9C  mov     ecx, [esi+24h]
45AC9F  mov     edx, [ebx+248h]
45ACA5  mov     eax, [esi+28h]
45ACA8  imul    edx, ecx
45ACAB  add     [esp+760h+var_748], edx
45ACAF  mov     edx, 1
45ACB4  cmp     eax, edx
45ACB6  jl      short loc_45ACC7
45ACB8  mov     edi, [ebx+2B0h]
45ACBE  imul    edi, eax
45ACC1  add     [esp+760h+var_748], edi
45ACC5  jmp     short loc_45ACDC
45ACC7  test    eax, eax
45ACC9  jns     short loc_45ACDC
45ACCB  mov     eax, [edi+1Ch]
45ACCE  add     eax, [edi+18h]
45ACD1  imul    eax, [ebx+2B0h]
45ACD8  add     [esp+760h+var_748], eax
45ACDC  xor     eax, eax
45ACDE  mov     [esp+760h+var_71C], eax
45ACE2  cmp     [esi+30h], eax
45ACE5  jz      short loc_45AD0C
45ACE7  mov     edi, [ebx+14h]
45ACEA  imul    edi, ecx
45ACED  imul    edi, ecx
45ACF0  add     edi, [ebx+10h]
45ACF3  add     [esp+760h+var_748], edi
45ACF7  cmp     [esi+8], eax
45ACFA  jnz     short loc_45AD08
45ACFC  mov     [esp+760h+var_71C], edx
45AD00  cmp     [ebx+254h], edx
45AD06  jz      short loc_45AD0C
45AD08  mov     [esp+760h+var_71C], eax
45AD0C  mov     ecx, [esp+760h+var_700]
45AD10  mov     [ebx+12Ch], ecx
45AD16  xor     esi, esi
45AD18  mov     ecx, ebx
45AD1A  mov     edx, 100h
45AD1F  lea     edi, [esp+760h+var_714]
45AD23  call    sub_459BA0
45AD28  mov     edx, [ebx+1B0h]
45AD2E  sub     edx, [esp+760h+var_714]
45AD32  mov     ecx, [esp+760h+var_72C]
45AD36  mov     eax, [ecx]
45AD38  add     [esp+760h+var_700], edx
45AD3C  mov     edx, [eax+8]
45AD3F  call    edx
45AD41  mov     ecx, [esp+760h+var_73C]
45AD45  mov     edx, [ecx+18h]
45AD48  mov     esi, [esp+760h+var_724]
45AD4C  mov     ecx, [esi+4]
45AD4F  imul    ecx, edx
45AD52  add     ecx, [esi]
45AD54  mov     [esp+760h+var_744], eax
45AD58  mov     [esp+760h+var_6D8], ecx
45AD5F  xor     ecx, ecx
45AD61  mov     [esp+760h+var_6A8], ecx
45AD68  mov     [esp+760h+var_6A4], ecx
45AD6F  mov     [esp+760h+var_6A0], ecx
45AD76  mov     [esp+760h+var_69C], ecx
45AD7D  mov     [esp+760h+var_698], ecx
45AD84  mov     [esp+760h+var_694], ecx
45AD8B  test    eax, eax
45AD8D  jz      short loc_45AD9D
45AD8F  mov     ecx, eax
45AD91  mov     eax, [ebx+1Ch]
45AD94  lea     edi, [esp+760h+var_678]
45AD9B  rep stosd
45AD9D  mov     eax, [esp+760h+var_73C]
45ADA1  mov     eax, [eax+1Ch]
45ADA4  imul    eax, edx
45ADA7  xor     ecx, ecx
45ADA9  mov     edx, 1Ch
45ADAE  mul     edx
45ADB0  seto    cl
45ADB3  neg     ecx
45ADB5  or      ecx, eax
45ADB7  push    ecx; Size
45ADB8  call    ??2@YAPAXI@Z; operator new(uint)
45ADBD  add     esp, 4
45ADC0  cmp     dword ptr [esp+760h+Src], 0
45ADC8  mov     [esp+760h+var_6F8], eax
45ADCC  mov     [esp+760h+var_6DC], 0
45ADD7  jl      loc_45B8F0
45ADDD  lea     eax, [esp+760h+Src]
45ADE4  mov     [esp+760h+var_6E0], eax
45ADEB  mov     ecx, [eax]
45ADED  mov     eax, ecx
45ADEF  imul    eax, 8DCh
45ADF5  add     eax, [esp+760h+var_6FC]
45ADF9  cmp     dword ptr [eax+1Ch], 0
45ADFD  mov     [esp+760h+var_74C], eax
45AE01  jle     loc_45B8C5
45AE07  mov     edx, [esp+760h+var_700]
45AE0B  mov     esi, eax
45AE0D  mov     eax, [esi+12Ch]
45AE13  cmp     eax, edx
45AE15  jg      loc_45B8F0
45AE1B  jnz     short loc_45AE2A
45AE1D  mov     edx, [esp+760h+var_73C]
45AE21  cmp     [edx+2Ch], ecx
45AE24  jl      loc_45B8F0
45AE2A  mov     eax, [esi+4]
45AE2D  test    [ebx+4], eax
45AE30  jnz     loc_45B6FF
45AE36  mov     edx, [esp+760h+var_6D8]
45AE3D  xor     eax, eax
45AE3F  cmp     [esi+850h], eax
45AE45  lea     ecx, ds:0[edx*8]
45AE4C  setnz   al
45AE4F  sub     ecx, edx
45AE51  add     ecx, ecx
45AE53  add     ecx, ecx
45AE55  mov     [esp+760h+var_6E4], 0
45AE5D  mov     [esp+760h+var_734], ecx
45AE61  mov     edx, [esi+eax*4+854h]
45AE68  cmp     dword ptr [ecx+edx], 0
45AE6C  mov     [esp+760h+Size], eax
45AE70  jz      loc_45AEF9
45AE76  mov     esi, [esp+760h+var_74C]
45AE7A  xor     edx, edx
45AE7C  lea     ecx, [esp+760h+var_6D4]
45AE83  call    sub_459880
45AE88  mov     edx, [esi+234h]
45AE8E  push    0; int
45AE90  push    1; int
45AE92  mov     eax, ecx
45AE94  mov     ecx, [esi+238h]
45AE9A  push    eax; int
45AE9B  mov     eax, [esi]
45AE9D  mov     esi, [esp+76Ch+var_72C]
45AEA1  push    1; int
45AEA3  push    ecx; int
45AEA4  push    edx; void *
45AEA5  push    eax; int
45AEA6  push    esi; int
45AEA7  call    sub_453C20
45AEAC  mov     ecx, [esp+760h+var_6F8]
45AEB0  mov     edx, [esp+760h+var_74C]
45AEB4  push    0; int
45AEB6  push    ecx; void *
45AEB7  mov     ecx, [edx]
45AEB9  push    esi; int
45AEBA  lea     edi, [esp+76Ch+var_6D4]
45AEC1  call    sub_4541A0
45AEC6  mov     eax, [esp+760h+var_708]
45AECA  mov     ecx, [esp+760h+var_734]
45AECE  add     ecx, [esp+760h+var_6F8]
45AED2  mov     edi, [esp+760h+var_744]
45AED6  push    eax
45AED7  mov     eax, [esp+764h+var_73C]
45AEDB  push    ecx
45AEDC  push    eax
45AEDD  lea     ecx, [esp+76Ch+var_690]
45AEE4  call    sub_45BB60
45AEE9  mov     esi, [esp+760h+var_74C]
45AEED  mov     ecx, [esp+760h+var_734]
45AEF1  mov     [esp+760h+var_6E4], eax
45AEF5  mov     eax, [esp+760h+Size]
45AEF9  mov     edx, [esp+760h+var_6E4]
45AEFD  lea     edi, [esp+760h+var_408]
45AF04  mov     [esp+760h+var_730], edx
45AF08  mov     [esp+760h+var_750], edi
45AF0C  xor     edx, edx
45AF0E  lea     edi, [esi+2B8h]
45AF14  lea     eax, [esi+eax*4+85Ch]
45AF1B  mov     [esp+760h+var_740], edx
45AF1F  mov     [esp+760h+var_720], edi
45AF23  mov     [esp+760h+var_704], eax
45AF27  mov     [esp+edx*4+760h+var_648], 0
45AF32  mov     edx, [esp+760h+var_720]
45AF36  cmp     dword ptr [edx-4], 0
45AF3A  jz      loc_45B01C
45AF40  mov     eax, [eax]
45AF42  cmp     dword ptr [ecx+eax], 0
45AF46  jz      loc_45B01C
45AF4C  mov     edx, [esp+760h+var_740]
45AF50  mov     esi, [esp+760h+var_74C]
45AF54  inc     edx
45AF55  lea     ecx, [esp+760h+var_6D4]
45AF5C  call    sub_459880
45AF61  mov     edx, [esi+238h]
45AF67  mov     eax, [esi+234h]
45AF6D  push    0; int
45AF6F  push    1; int
45AF71  push    ecx; int
45AF72  mov     ecx, [esi]
45AF74  mov     esi, [esp+76Ch+var_72C]
45AF78  push    1; int
45AF7A  push    edx; int
45AF7B  push    eax; void *
45AF7C  push    ecx; int
45AF7D  push    esi; int
45AF7E  call    sub_453C20
45AF83  mov     edx, [esp+760h+var_6F8]
45AF87  mov     eax, [esp+760h+var_74C]
45AF8B  mov     ecx, [eax]
45AF8D  push    0; int
45AF8F  push    edx; void *
45AF90  push    esi; int
45AF91  lea     edi, [esp+76Ch+var_6D4]
45AF98  call    sub_4541A0
45AF9D  mov     ecx, [esp+760h+var_720]
45AFA1  mov     eax, [ecx]
45AFA3  test    eax, eax
45AFA5  jnz     short loc_45AFD4
45AFA7  mov     edx, [esp+760h+var_708]
45AFAB  mov     eax, [esp+760h+var_734]
45AFAF  add     eax, [esp+760h+var_6F8]
45AFB3  mov     edi, [esp+760h+var_744]
45AFB7  mov     ecx, [esp+760h+var_750]
45AFBB  push    edx
45AFBC  mov     edx, [esp+764h+var_73C]
45AFC0  push    eax
45AFC1  push    edx
45AFC2  call    sub_45BB60
45AFC7  mov     ecx, [esp+760h+var_740]
45AFCB  mov     [esp+ecx*4+760h+var_648], eax
45AFD2  jmp     short loc_45AFEF
45AFD4  cmp     eax, 4
45AFD7  jnz     short loc_45AFEF
45AFD9  mov     edx, [esp+760h+var_734]
45AFDD  mov     eax, [esp+760h+var_6F8]
45AFE1  mov     ecx, [edx+eax]
45AFE4  mov     edx, [esp+760h+var_740]
45AFE8  mov     [esp+edx*4+760h+var_648], ecx
45AFEF  cmp     [esp+760h+var_730], 0
45AFF4  jnz     short loc_45B00C
45AFF6  mov     eax, [esp+760h+var_740]
45AFFA  cmp     [esp+eax*4+760h+var_648], 0
45B002  mov     [esp+760h+var_730], 0
45B00A  jz      short loc_45B014
45B00C  mov     [esp+760h+var_730], 1
45B014  mov     ecx, [esp+760h+var_734]
45B018  mov     esi, [esp+760h+var_74C]
45B01C  mov     edx, [esp+760h+var_740]
45B020  mov     eax, [esp+760h+var_704]
45B024  add     [esp+760h+var_720], 58h ; 'X'
45B029  add     [esp+760h+var_750], 18h
45B02E  inc     edx
45B02F  add     eax, 8
45B032  mov     [esp+760h+var_740], edx
45B036  mov     [esp+760h+var_704], eax
45B03A  cmp     edx, 10h
45B03D  jb      loc_45AF27
45B043  cmp     dword ptr [esi+850h], 0
45B04A  jz      short loc_45B06C
45B04C  mov     eax, [ebx+2ACh]
45B052  test    eax, eax
45B054  jnz     short loc_45B066
45B056  mov     ecx, [esp+760h+var_73C]
45B05A  mov     edx, [ecx+154h]
45B060  mov     [esp+760h+var_720], edx
45B064  jmp     short loc_45B074
45B066  mov     [esp+760h+var_720], eax
45B06A  jmp     short loc_45B074
45B06C  mov     [esp+760h+var_720], 64h ; 'd'
45B074  cmp     [esp+760h+var_744], 0
45B079  mov     [esp+760h+var_750], 0
45B081  jbe     loc_45B3E5
45B087  mov     eax, [esp+760h+var_708]
45B08B  test    eax, eax
45B08D  js      short loc_45B099
45B08F  cmp     eax, [esp+760h+var_750]
45B093  jnz     loc_45B3D2
45B099  mov     edi, [esp+760h+var_750]
45B09D  mov     ecx, [ebx]
45B09F  mov     edx, [esp+760h+var_72C]
45B0A3  add     edi, 2
45B0A6  call    sub_453720
45B0AB  cmp     [esp+760h+var_6E4], 0
45B0B0  jz      loc_45B1D0
45B0B6  mov     eax, [esp+760h+var_750]
45B0BA  mov     eax, [esp+eax*4+760h+var_678]
45B0C1  xor     ecx, ecx
45B0C3  cmp     eax, 1
45B0C6  setnl   cl
45B0C9  mov     esi, ebx
45B0CB  lea     edi, [esp+760h+var_714]
45B0CF  mov     [esp+760h+var_6F0], ecx
45B0D3  mov     ecx, [esp+760h+var_74C]
45B0D7  mov     edx, [ecx+260h]
45B0DD  call    sub_459BA0
45B0E2  mov     eax, [esp+760h+var_750]
45B0E6  mov     ecx, [esp+eax*4+760h+var_690]
45B0ED  mov     eax, [ebx]
45B0EF  mov     esi, [esp+760h+var_72C]
45B0F3  mov     edx, [esi]
45B0F5  mov     edx, [edx+14h]
45B0F8  push    1
45B0FA  push    ecx
45B0FB  push    eax
45B0FC  lea     ecx, [esp+76Ch+var_6EC]
45B103  push    ecx
45B104  mov     ecx, esi
45B106  call    edx
45B108  mov     eax, [esp+760h+var_6E8]
45B10C  mov     ecx, [esp+760h+var_6EC]
45B110  push    eax
45B111  push    ecx
45B112  mov     ecx, [ebx]
45B114  lea     edx, [esp+768h+var_718]
45B118  push    edx
45B119  mov     edx, esi
45B11B  call    sub_453BE0
45B120  mov     eax, [esp+760h+var_718]
45B124  mov     esi, [esp+760h+var_73C]
45B128  push    eax
45B129  push    esi
45B12A  call    sub_459830
45B12F  mov     ecx, [ebx+8]
45B132  push    eax
45B133  push    esi
45B134  mov     esi, [esp+768h+var_74C]
45B138  mov     edi, [esi+8]
45B13B  call    sub_4597C0
45B140  mov     ecx, [esp+764h+var_710]
45B144  mov     edi, [esp+764h+var_714]
45B148  push    ecx
45B149  lea     edx, [ebx+2Ch]
45B14C  push    edx
45B14D  mov     edx, [esi+260h]
45B153  lea     ecx, [esi+264h]
45B159  push    ecx
45B15A  push    edx
45B15B  lea     edx, [esp+774h+var_738]
45B15F  lea     ecx, [esi+2Ch]
45B162  push    edx
45B163  call    sub_4598F0
45B168  mov     ecx, [esp+778h+var_750]
45B16C  mov     eax, [esp+ecx*4+778h+var_678]
45B173  mov     edx, [esp+778h+var_738]
45B177  sub     eax, edx
45B179  add     esp, 18h
45B17C  mov     [esp+ecx*4+760h+var_678], eax
45B183  test    eax, eax
45B185  jg      short loc_45B196
45B187  cmp     [esp+760h+var_6F0], 0
45B18C  jz      short loc_45B196
45B18E  mov     eax, [ebx+290h]
45B194  jmp     short loc_45B198
45B196  xor     eax, eax
45B198  mov     ecx, [ebx+28Ch]
45B19E  imul    ecx, edx
45B1A1  add     ecx, [ebx+288h]
45B1A7  mov     esi, [esp+760h+var_750]
45B1AB  add     ecx, eax
45B1AD  imul    ecx, [esp+760h+var_720]
45B1B2  mov     eax, 51EB851Fh
45B1B7  imul    ecx
45B1B9  sar     edx, 5
45B1BC  mov     eax, edx
45B1BE  shr     eax, 1Fh
45B1C1  lea     esi, [esp+esi*4+760h+var_6A8]
45B1C8  add     eax, edx
45B1CA  add     [esi], eax
45B1CC  mov     esi, [esp+760h+var_74C]
45B1D0  mov     edx, [esp+760h+var_750]
45B1D4  lea     ecx, [esi+2C0h]
45B1DA  mov     [esp+760h+var_740], ecx
45B1DE  xor     eax, eax
45B1E0  lea     ecx, [esp+edx*4+760h+var_408]
45B1E7  mov     [esp+760h+var_6F0], eax
45B1EB  mov     [esp+760h+var_704], ecx
45B1EF  cmp     [esp+eax*4+760h+var_648], 0
45B1F7  jz      loc_45B3B6
45B1FD  mov     eax, [esp+760h+var_740]
45B201  mov     eax, [eax-8]
45B204  test    eax, eax
45B206  jnz     loc_45B302
45B20C  mov     edx, [esp+760h+var_750]
45B210  xor     ecx, ecx
45B212  cmp     [esp+edx*4+760h+var_678], 1
45B21A  mov     edx, [esp+760h+var_740]
45B21E  mov     edx, [edx]
45B220  setnl   cl
45B223  mov     esi, ebx
45B225  lea     edi, [esp+760h+var_714]
45B229  mov     [esp+760h+var_6F4], ecx
45B22D  mov     ecx, [esp+760h+var_74C]
45B231  call    sub_459BA0
45B236  mov     ecx, [esp+760h+var_704]
45B23A  mov     edx, [ecx]
45B23C  mov     ecx, [ebx]
45B23E  mov     esi, [esp+760h+var_72C]
45B242  mov     eax, [esi]
45B244  mov     eax, [eax+14h]
45B247  push    1
45B249  push    edx
45B24A  push    ecx
45B24B  lea     edx, [esp+76Ch+var_6EC]
45B252  push    edx
45B253  mov     ecx, esi
45B255  call    eax
45B257  mov     ecx, [esp+760h+var_6E8]
45B25B  mov     edx, [esp+760h+var_6EC]
45B25F  push    ecx
45B260  mov     ecx, [ebx]
45B262  push    edx
45B263  lea     eax, [esp+768h+var_718]
45B267  push    eax
45B268  mov     edx, esi
45B26A  call    sub_453BE0
45B26F  mov     ecx, [esp+760h+var_718]
45B273  mov     esi, [esp+760h+var_73C]
45B277  push    ecx
45B278  push    esi
45B279  call    sub_459830
45B27E  mov     ecx, [ebx+8]
45B281  push    eax
45B282  push    esi
45B283  mov     esi, [esp+768h+var_74C]
45B287  mov     edi, [esi+8]
45B28A  call    sub_4597C0
45B28F  mov     edx, [esp+764h+var_710]
45B293  mov     edi, [esp+764h+var_714]
45B297  push    edx
45B298  mov     edx, [esp+768h+var_740]
45B29C  mov     edx, [edx]
45B29E  lea     ecx, [ebx+2Ch]
45B2A1  push    ecx
45B2A2  mov     ecx, [esp+76Ch+var_740]
45B2A6  add     ecx, 4
45B2A9  push    ecx
45B2AA  push    edx
45B2AB  lea     edx, [esp+774h+var_738]
45B2AF  lea     ecx, [esi+2Ch]
45B2B2  push    edx
45B2B3  call    sub_4598F0
45B2B8  mov     eax, [esp+778h+var_750]
45B2BC  mov     eax, [esp+eax*4+778h+var_678]
45B2C3  mov     edx, [esp+778h+var_738]
45B2C7  mov     ecx, [esp+778h+var_750]
45B2CB  sub     eax, edx
45B2CD  add     esp, 18h
45B2D0  mov     [esp+ecx*4+760h+var_678], eax
45B2D7  test    eax, eax
45B2D9  jg      short loc_45B2EA
45B2DB  cmp     [esp+760h+var_6F4], 0
45B2E0  jz      short loc_45B2EA
45B2E2  mov     eax, [ebx+29Ch]
45B2E8  jmp     short loc_45B2EC
45B2EA  xor     eax, eax
45B2EC  mov     ecx, [ebx+298h]
45B2F2  imul    ecx, edx
45B2F5  add     ecx, [ebx+294h]
45B2FB  add     ecx, eax
45B2FD  jmp     loc_45B38F
45B302  cmp     eax, 4
45B305  jnz     loc_45B3B6
45B30B  mov     ecx, [esp+760h+var_740]
45B30F  mov     edx, [ecx]
45B311  mov     ecx, [esp+760h+var_74C]
45B315  mov     esi, ebx
45B317  lea     edi, [esp+760h+var_714]
45B31B  call    sub_459BA0
45B320  mov     edx, [esp+760h+var_73C]
45B324  mov     esi, [esp+760h+var_74C]
45B328  mov     ecx, [ebx+8]
45B32B  mov     edi, [esi+8]
45B32E  push    10000h
45B333  push    edx
45B334  call    sub_4597C0
45B339  mov     ecx, [esp+764h+var_710]
45B33D  mov     edi, [esp+764h+var_714]
45B341  push    ecx
45B342  lea     edx, [ebx+2Ch]
45B345  push    edx
45B346  mov     edx, [esp+76Ch+var_740]
45B34A  lea     ecx, [edx+4]
45B34D  mov     edx, [edx]
45B34F  push    ecx
45B350  push    edx
45B351  lea     edx, [esp+774h+var_738]
45B355  lea     ecx, [esi+2Ch]
45B358  push    edx
45B359  call    sub_4598F0
45B35E  mov     eax, [esp+778h+var_740]
45B362  mov     ecx, [eax-4]
45B365  mov     edx, [eax+14h]
45B368  mov     [ebx+ecx*4+134h], edx
45B36F  mov     ecx, [eax-4]
45B372  mov     eax, [esp+778h+var_738]
45B376  mov     [ebx+ecx*4+1B4h], eax
45B37D  mov     ecx, [ebx+2A4h]
45B383  imul    ecx, eax
45B386  add     esp, 18h
45B389  add     ecx, [ebx+2A0h]
45B38F  imul    ecx, [esp+760h+var_720]
45B394  mov     esi, [esp+760h+var_750]
45B398  mov     eax, 51EB851Fh
45B39D  imul    ecx
45B39F  sar     edx, 5
45B3A2  mov     eax, edx
45B3A4  shr     eax, 1Fh
45B3A7  lea     esi, [esp+esi*4+760h+var_6A8]
45B3AE  add     eax, edx
45B3B0  add     [esi], eax
45B3B2  mov     esi, [esp+760h+var_74C]
45B3B6  mov     eax, [esp+760h+var_6F0]
45B3BA  add     [esp+760h+var_740], 58h ; 'X'
45B3BF  add     [esp+760h+var_704], 18h
45B3C4  inc     eax
45B3C5  mov     [esp+760h+var_6F0], eax
45B3C9  cmp     eax, 10h
45B3CC  jb      loc_45B1EF
45B3D2  mov     eax, [esp+760h+var_750]
45B3D6  inc     eax
45B3D7  mov     [esp+760h+var_750], eax
45B3DB  cmp     eax, [esp+760h+var_744]
45B3DF  jb      loc_45B087
45B3E5  cmp     [esp+760h+var_730], 0
45B3EA  jnz     loc_45B4CB
45B3F0  cmp     dword ptr [esi+850h], 0
45B3F7  jnz     loc_45B4CB
45B3FD  mov     eax, [esi+238h]
45B403  mov     edx, [esi+12Ch]
45B409  lea     ecx, [eax+eax*2]
45B40C  mov     eax, [esp+760h+var_6DC]
45B413  sar     ecx, 2
45B416  imul    ecx, [esi+240h]
45B41D  add     ecx, [esi+23Ch]
45B423  inc     eax
45B424  add     edx, ecx
45B426  xor     ecx, ecx
45B428  mov     [esi+12Ch], edx
45B42E  cmp     dword ptr [esp+eax*4+760h+Src], ecx
45B435  jl      short loc_45B46F
45B437  mov     edi, [esp+760h+var_6FC]
45B43B  jmp     short loc_45B440
45B440  test    ecx, ecx
45B442  jnz     short loc_45B460
45B444  mov     esi, dword ptr [esp+eax*4+760h+Src]
45B44B  imul    esi, 8DCh
45B451  cmp     edx, [esi+edi+12Ch]
45B458  mov     esi, [esp+760h+var_74C]
45B45C  jge     short loc_45B460
45B45E  mov     ecx, eax
45B460  inc     eax
45B461  cmp     dword ptr [esp+eax*4+760h+Src], 0
45B469  jge     short loc_45B440
45B46B  test    ecx, ecx
45B46D  jnz     short loc_45B473
45B46F  mov     ecx, eax
45B471  jmp     short loc_45B480
45B473  cmp     eax, ecx
45B475  jb      short loc_45B493
45B477  jmp     short loc_45B480
45B480  mov     edx, dword ptr [esp+eax*4+760h+Src]
45B487  mov     [esp+eax*4+760h+var_604], edx
45B48E  dec     eax
45B48F  cmp     eax, ecx
45B491  jnb     short loc_45B480
45B493  cmp     dword ptr [esi+0Ch], 0
45B497  mov     eax, [esp+760h+var_6E0]
45B49E  mov     edx, [eax]
45B4A0  mov     dword ptr [esp+ecx*4+760h+Src], edx
45B4A7  mov     dword ptr [esi+850h], 1
45B4B1  jl      short loc_45B4CB
45B4B3  mov     eax, [esi+848h]
45B4B9  mov     ecx, [esi+84Ch]
45B4BF  mov     [esi+834h], eax
45B4C5  mov     [esi+838h], ecx
45B4CB  mov     edx, [esp+760h+var_724]
45B4CF  cmp     dword ptr [edx+8], 0
45B4D3  jnz     loc_45B8C5
45B4D9  cmp     [esp+760h+var_730], 0
45B4DE  jz      loc_45B8C5
45B4E4  mov     eax, edx
45B4E6  cmp     dword ptr [eax+1Ch], 0
45B4EA  jge     loc_45B8C5
45B4F0  xor     eax, eax
45B4F2  xor     esi, esi
45B4F4  mov     [esp+760h+var_6C0], eax
45B4FB  mov     [esp+760h+var_6BC], eax
45B502  mov     [esp+760h+var_6B8], eax
45B509  mov     [esp+760h+var_6B4], eax
45B510  mov     [esp+760h+var_6B0], eax
45B517  mov     [esp+760h+var_6AC], eax
45B51E  mov     [esp+760h+var_740], esi
45B522  cmp     [esp+760h+var_744], eax
45B526  jbe     loc_45B64A
45B52C  mov     eax, [esp+760h+Size]
45B530  mov     edx, [esp+760h+var_74C]
45B534  lea     ecx, [edx+eax*4+864h]
45B53B  mov     eax, [edx+eax*4+854h]
45B542  mov     [esp+760h+var_6F4], ecx
45B546  mov     ecx, [esp+760h+var_734]
45B54A  mov     [esp+760h+var_734], ecx
45B54E  lea     ecx, [ecx+eax+4]
45B552  mov     [esp+760h+Size], ecx
45B556  jmp     short loc_45B564
45B558  jmp     short loc_45B560
45B560  mov     edx, [esp+760h+var_74C]
45B564  mov     ecx, [ecx]
45B566  add     [esp+esi*4+760h+var_6C0], ecx
45B56D  mov     ecx, [esp+760h+var_6F4]
45B571  xor     eax, eax
45B573  add     edx, 310h
45B579  cmp     [esp+eax*4+760h+var_648], 0
45B581  jz      short loc_45B59F
45B583  cmp     dword ptr [edx-58h], 0
45B587  jnz     short loc_45B59F
45B589  mov     esi, [ecx-8]
45B58C  mov     edi, [esp+760h+var_734]
45B590  mov     edi, [esi+edi+4]
45B594  mov     esi, [esp+760h+var_740]
45B598  add     [esp+esi*4+760h+var_6C0], edi
45B59F  cmp     [esp+eax*4+760h+var_644], 0
45B5A7  jz      short loc_45B5C3
45B5A9  cmp     dword ptr [edx], 0
45B5AC  jnz     short loc_45B5C3
45B5AE  mov     esi, [ecx]
45B5B0  mov     edi, [esp+760h+var_734]
45B5B4  mov     edi, [edi+esi+4]
45B5B8  mov     esi, [esp+760h+var_740]
45B5BC  add     [esp+esi*4+760h+var_6C0], edi
45B5C3  cmp     [esp+eax*4+760h+var_640], 0
45B5CB  jz      short loc_45B5E9
45B5CD  cmp     dword ptr [edx+58h], 0
45B5D1  jnz     short loc_45B5E9
45B5D3  mov     esi, [ecx+8]
45B5D6  mov     edi, [esp+760h+var_734]
45B5DA  mov     edi, [esi+edi+4]
45B5DE  mov     esi, [esp+760h+var_740]
45B5E2  add     [esp+esi*4+760h+var_6C0], edi
45B5E9  cmp     [esp+eax*4+760h+var_63C], 0
45B5F1  jz      short loc_45B612
45B5F3  cmp     dword ptr [edx+0B0h], 0
45B5FA  jnz     short loc_45B612
45B5FC  mov     esi, [ecx+10h]
45B5FF  mov     edi, [esp+760h+var_734]
45B603  mov     edi, [esi+edi+4]
45B607  mov     esi, [esp+760h+var_740]
45B60B  add     [esp+esi*4+760h+var_6C0], edi
45B612  add     eax, 4
45B615  add     edx, 160h
45B61B  add     ecx, 20h ; ' '
45B61E  cmp     eax, 10h
45B621  jb      loc_45B579
45B627  mov     esi, [esp+760h+var_740]
45B62B  mov     ecx, [esp+760h+Size]
45B62F  add     [esp+760h+var_734], 4
45B634  inc     esi
45B635  add     ecx, 4
45B638  mov     [esp+760h+var_740], esi
45B63C  mov     [esp+760h+Size], ecx
45B640  cmp     esi, [esp+760h+var_744]
45B644  jb      loc_45B560
45B64A  mov     edi, [esp+760h+var_744]
45B64E  mov     esi, [esp+760h+var_6C0]
45B655  mov     eax, 1
45B65A  cmp     edi, eax
45B65C  jbe     short loc_45B672
45B65E  mov     edi, edi
45B660  mov     ecx, [esp+eax*4+760h+var_6C0]
45B667  cmp     esi, ecx
45B669  jnb     short loc_45B66D
45B66B  mov     esi, ecx
45B66D  inc     eax
45B66E  cmp     eax, edi
45B670  jb      short loc_45B660
45B672  xor     eax, eax
45B674  xor     edx, edx
45B676  mov     [esp+760h+var_704], eax
45B67A  cmp     edi, eax
45B67C  jbe     short loc_45B6B6
45B67E  mov     edi, edi
45B680  xor     ecx, ecx
45B682  cmp     esi, [esp+eax*4+760h+var_6C0]
45B689  setz    cl
45B68C  mov     [esp+eax*4+760h+var_690], ecx
45B693  test    ecx, ecx
45B695  jz      short loc_45B69C
45B697  inc     edx
45B698  mov     [esp+760h+var_704], eax
45B69C  inc     eax
45B69D  cmp     eax, edi
45B69F  jb      short loc_45B680
45B6A1  cmp     edx, 1
45B6A4  jnz     short loc_45B6B6
45B6A6  mov     edx, [esp+760h+var_724]
45B6AA  mov     eax, [esp+760h+var_704]
45B6AE  mov     [edx+1Ch], eax
45B6B1  jmp     loc_45B8C5
45B6B6  mov     esi, [esp+760h+var_724]
45B6BA  mov     eax, [esi+4]
45B6BD  mov     ecx, [esp+760h+var_72C]
45B6C1  mov     edx, [ecx]
45B6C3  mov     edx, [edx+0Ch]
45B6C6  push    eax
45B6C7  mov     eax, [esi]
45B6C9  push    eax
45B6CA  mov     eax, [esp+768h+var_74C]
45B6CE  mov     edi, [eax+838h]
45B6D4  mov     eax, [eax+834h]
45B6DA  push    edi
45B6DB  push    eax
45B6DC  call    edx
45B6DE  sub     eax, 2
45B6E1  cmp     [esp+eax*4+760h+var_690], 0
45B6E9  jz      short loc_45B6F3
45B6EB  mov     [esi+1Ch], eax
45B6EE  jmp     loc_45B8C5
45B6F3  mov     eax, [esp+760h+var_704]
45B6F7  mov     [esi+1Ch], eax
45B6FA  jmp     loc_45B8C5
45B6FF  cmp     [esp+760h+var_70C], 0
45B704  jz      loc_45B8C5
45B70A  mov     ecx, [esp+760h+var_70C]
45B70E  mov     eax, [ecx+838h]
45B714  mov     edx, [esp+760h+var_73C]
45B718  imul    eax, [edx+18h]
45B71C  add     eax, [ecx+834h]
45B722  lea     ecx, ds:0[eax*8]
45B729  sub     ecx, eax
45B72B  mov     eax, [esp+760h+var_74C]
45B72F  mov     edx, [eax+854h]
45B735  add     ecx, ecx
45B737  add     ecx, ecx
45B739  cmp     dword ptr [ecx+edx], 0
45B73D  mov     [esp+760h+var_6F4], ecx
45B741  jz      loc_45B8C5
45B747  xor     edx, edx
45B749  mov     esi, eax
45B74B  lea     ecx, [esp+760h+var_6D4]
45B752  call    sub_459880
45B757  mov     edx, [esi+234h]
45B75D  push    0; int
45B75F  push    1; int
45B761  mov     eax, ecx
45B763  mov     ecx, [esi+238h]
45B769  push    eax; int
45B76A  mov     eax, [esi]
45B76C  mov     esi, [esp+76Ch+var_72C]
45B770  push    1; int
45B772  push    ecx; int
45B773  push    edx; void *
45B774  push    eax; int
45B775  push    esi; int
45B776  call    sub_453C20
45B77B  mov     ecx, [esp+760h+var_6F8]
45B77F  mov     edx, [esp+760h+var_74C]
45B783  push    0; int
45B785  push    ecx; void *
45B786  mov     ecx, [edx]
45B788  push    esi; int
45B789  lea     edi, [esp+76Ch+var_6D4]
45B790  call    sub_4541A0
45B795  mov     eax, [esp+760h+var_70C]
45B799  mov     ecx, [eax+83Ch]
45B79F  mov     edx, [esp+760h+var_6F4]
45B7A3  add     edx, [esp+760h+var_6F8]
45B7A7  mov     edi, [esp+760h+var_744]
45B7AB  push    ecx
45B7AC  mov     ecx, [esp+764h+var_73C]
45B7B0  push    edx
45B7B1  push    ecx
45B7B2  lea     ecx, [esp+76Ch+var_690]
45B7B9  call    sub_45BB60
45B7BE  test    eax, eax
45B7C0  jz      loc_45B8C5
45B7C6  mov     esi, [esp+760h+var_70C]
45B7CA  mov     ecx, [esp+760h+var_74C]
45B7CE  xor     edx, edx
45B7D0  cmp     dword ptr [esi+1Ch], 1
45B7D4  lea     edi, [esp+760h+var_714]
45B7D8  setnl   dl
45B7DB  mov     [esp+760h+Size], edx
45B7DF  mov     edx, [ecx+260h]
45B7E5  call    sub_459BA0
45B7EA  mov     ecx, [esi+83Ch]
45B7F0  mov     edx, [esp+ecx*4+760h+var_690]
45B7F7  mov     ecx, [esi]
45B7F9  mov     edi, [esp+760h+var_72C]
45B7FD  mov     eax, [edi]
45B7FF  mov     eax, [eax+14h]
45B802  push    1
45B804  push    edx
45B805  push    ecx
45B806  lea     edx, [esp+76Ch+var_6EC]
45B80D  push    edx
45B80E  mov     ecx, edi
45B810  call    eax
45B812  mov     ecx, [esp+760h+var_6E8]
45B816  mov     edx, [esp+760h+var_6EC]
45B81A  push    ecx
45B81B  mov     ecx, [esi]
45B81D  push    edx
45B81E  lea     eax, [esp+768h+var_718]
45B822  push    eax
45B823  mov     edx, edi
45B825  call    sub_453BE0
45B82A  mov     ecx, [esp+760h+var_718]
45B82E  mov     edi, [esp+760h+var_73C]
45B832  push    ecx
45B833  push    edi
45B834  call    sub_459830
45B839  mov     edx, [esp+760h+var_74C]
45B83D  mov     ecx, [esi+8]
45B840  push    eax
45B841  push    edi
45B842  mov     edi, [edx+8]
45B845  call    sub_4597C0
45B84A  mov     ecx, [esp+764h+var_710]
45B84E  mov     edi, [esp+764h+var_714]
45B852  push    ecx
45B853  lea     edx, [esi+2Ch]
45B856  push    edx
45B857  mov     edx, [esp+76Ch+var_74C]
45B85B  lea     ecx, [edx+264h]
45B861  push    ecx
45B862  lea     ecx, [edx+2Ch]
45B865  mov     edx, [edx+260h]
45B86B  push    edx
45B86C  lea     edx, [esp+774h+var_738]
45B870  push    edx
45B871  call    sub_4598F0
45B876  mov     esi, [esi+1Ch]
45B879  mov     eax, [esp+778h+var_738]
45B87D  add     esp, 18h
45B880  cmp     eax, esi
45B882  jle     short loc_45B892
45B884  mov     eax, [esp+760h+Size]
45B888  neg     eax
45B88A  sbb     eax, eax
45B88C  and     eax, esi
45B88E  mov     [esp+760h+var_738], eax
45B892  mov     ecx, [esp+760h+var_70C]
45B896  sub     [ecx+1Ch], eax
45B899  cmp     dword ptr [ecx+1Ch], 0
45B89D  jg      short loc_45B8AE
45B89F  cmp     [esp+760h+Size], 0
45B8A4  jz      short loc_45B8AE
45B8A6  mov     ecx, [ebx+284h]
45B8AC  jmp     short loc_45B8B0
45B8AE  xor     ecx, ecx
45B8B0  mov     edx, [ebx+280h]
45B8B6  imul    edx, eax
45B8B9  add     edx, [ebx+27Ch]
45B8BF  add     edx, ecx
45B8C1  add     [esp+760h+var_748], edx
45B8C5  mov     eax, [esp+760h+var_6DC]
45B8CC  inc     eax
45B8CD  cmp     dword ptr [esp+eax*4+760h+Src], 0
45B8D5  mov     [esp+760h+var_6DC], eax
45B8DC  lea     eax, [esp+eax*4+760h+Src]
45B8E3  mov     [esp+760h+var_6E0], eax
45B8EA  jge     loc_45ADEB
45B8F0  mov     eax, [esp+760h+var_6F8]
45B8F4  push    eax; void *
45B8F5  call    ??3@YAXPAX@Z; operator delete(void *)
45B8FA  add     esp, 4
45B8FD  cmp     [esp+760h+var_71C], 0
45B902  jz      loc_45BAA1
45B908  mov     ecx, [ebx+238h]
45B90E  mov     edx, [ebx+234h]
45B914  mov     esi, [esp+760h+var_72C]
45B918  push    1
45B91A  push    0FFFFFFFFh
45B91C  push    0FFFFFFFFh
45B91E  push    ecx
45B91F  mov     ecx, [ebx]
45B921  push    edx
45B922  mov     eax, esi
45B924  call    sub_453830
45B929  mov     eax, [esp+760h+var_70C]
45B92D  mov     ecx, [eax+838h]
45B933  mov     edx, [eax+834h]
45B939  push    ecx
45B93A  push    edx
45B93B  lea     eax, [esp+768h+var_678]
45B942  push    eax
45B943  lea     ecx, [esp+76Ch+var_690]
45B94A  push    ecx
45B94B  mov     ecx, [ebx]
45B94D  mov     eax, esi
45B94F  call    sub_4538C0
45B954  cmp     [esp+760h+var_744], 0
45B959  mov     [esp+760h+var_71C], 80000000h
45B961  mov     [esp+760h+var_730], 0
45B969  jbe     loc_45BAA1
45B96F  nop
45B970  mov     edx, [esp+760h+var_730]
45B974  cmp     [esp+edx*4+760h+var_690], 0
45B97C  jl      loc_45BA61
45B982  mov     esi, [esp+760h+var_70C]
45B986  mov     edx, [ebx+260h]
45B98C  xor     eax, eax
45B98E  cmp     dword ptr [esi+1Ch], 1
45B992  mov     ecx, ebx
45B994  setnl   al
45B997  lea     edi, [esp+760h+var_714]
45B99B  mov     [esp+760h+Size], eax
45B99F  call    sub_459BA0
45B9A4  mov     eax, [esp+760h+var_730]
45B9A8  mov     ecx, [esp+eax*8+760h+var_674]
45B9AF  mov     edx, [esp+eax*8+760h+var_678]
45B9B6  push    ecx
45B9B7  mov     ecx, [esi]
45B9B9  push    edx
45B9BA  mov     edx, [esp+768h+var_72C]
45B9BE  lea     eax, [esp+768h+var_718]
45B9C2  push    eax
45B9C3  call    sub_453BE0
45B9C8  mov     ecx, [esp+760h+var_718]
45B9CC  mov     edi, [esp+760h+var_73C]
45B9D0  push    ecx
45B9D1  push    edi
45B9D2  call    sub_459830
45B9D7  mov     ecx, [esi+8]
45B9DA  push    eax
45B9DB  push    edi
45B9DC  mov     edi, [ebx+8]
45B9DF  call    sub_4597C0
45B9E4  mov     edx, [esp+764h+var_710]
45B9E8  mov     edi, [esp+764h+var_714]
45B9EC  push    edx
45B9ED  lea     ecx, [esi+2Ch]
45B9F0  push    ecx
45B9F1  lea     edx, [ebx+264h]
45B9F7  push    edx
45B9F8  mov     edx, [ebx+260h]
45B9FE  push    edx
45B9FF  lea     edx, [esp+774h+var_738]
45BA03  lea     ecx, [ebx+2Ch]
45BA06  push    edx
45BA07  call    sub_4598F0
45BA0C  mov     esi, [esi+1Ch]
45BA0F  mov     ecx, [esp+778h+var_738]
45BA13  add     esp, 18h
45BA16  cmp     ecx, esi
45BA18  jle     short loc_45BA28
45BA1A  mov     ecx, [esp+760h+Size]
45BA1E  neg     ecx
45BA20  sbb     ecx, ecx
45BA22  and     ecx, esi
45BA24  mov     [esp+760h+var_738], ecx
45BA28  mov     eax, [esp+760h+var_70C]
45BA2C  sub     [eax+1Ch], ecx
45BA2F  cmp     dword ptr [eax+1Ch], 0
45BA33  jg      short loc_45BA44
45BA35  cmp     [esp+760h+Size], 0
45BA3A  jz      short loc_45BA44
45BA3C  mov     edx, [ebx+284h]
45BA42  jmp     short loc_45BA46
45BA44  xor     edx, edx
45BA46  mov     eax, [ebx+280h]
45BA4C  imul    eax, ecx
45BA4F  add     eax, [ebx+27Ch]
45BA55  add     eax, edx
45BA57  cmp     [esp+760h+var_71C], eax
45BA5B  jge     short loc_45BA61
45BA5D  mov     [esp+760h+var_71C], eax
45BA61  mov     eax, [esp+760h+var_730]
45BA65  inc     eax
45BA66  mov     [esp+760h+var_730], eax
45BA6A  cmp     eax, [esp+760h+var_744]
45BA6E  jb      loc_45B970
45BA74  mov     eax, [esp+760h+var_71C]
45BA78  cmp     eax, 80000000h
45BA7D  jz      short loc_45BAA1
45BA7F  mov     ecx, [esp+760h+var_73C]
45BA83  mov     ecx, [ecx+158h]
45BA89  imul    ecx, eax
45BA8C  mov     eax, 51EB851Fh
45BA91  imul    ecx
45BA93  sar     edx, 5
45BA96  mov     eax, edx
45BA98  shr     eax, 1Fh
45BA9B  add     eax, edx
45BA9D  add     [esp+760h+var_748], eax
45BAA1  mov     edx, [esp+760h+var_744]
45BAA5  xor     eax, eax
45BAA7  test    edx, edx
45BAA9  jz      short loc_45BAD5
45BAAB  mov     ecx, [esp+760h+var_724]
45BAAF  mov     esi, [esp+760h+var_708]
45BAB3  mov     edi, [esp+760h+var_748]
45BAB7  add     ecx, 34h ; '4'
45BABA  test    esi, esi
45BABC  js      short loc_45BAC2
45BABE  cmp     esi, eax
45BAC0  jnz     short loc_45BACD
45BAC2  mov     ebx, [esp+eax*4+760h+var_6A8]
45BAC9  add     ebx, edi
45BACB  mov     [ecx], ebx
45BACD  inc     eax
45BACE  add     ecx, 4
45BAD1  cmp     eax, edx
45BAD3  jb      short loc_45BABA
45BAD5  mov     ecx, [esp+760h+var_6FC]
45BAD9  push    ecx; void *
45BADA  call    ??3@YAXPAX@Z; operator delete(void *)
45BADF  mov     esi, [esp+764h+var_724]
45BAE3  add     esp, 4
45BAE6  mov     ecx, [esp+760h+var_4]
45BAED  xor     eax, eax
45BAEF  test    esi, esi
45BAF1  pop     edi
45BAF2  pop     esi
45BAF3  setnz   al
45BAF6  pop     ebx
45BAF7  xor     ecx, esp
45BAF9  call    sub_4ABDD5
45BAFE  mov     esp, ebp
45BB00  pop     ebp
45BB01  retn    4

;==== 45BB20 sub_45BB20
45BB20  push    ebp
45BB21  mov     ebp, esp
45BB23  mov     eax, [ebp+arg_4]
45BB26  mov     ecx, [ebp+arg_0]
45BB29  push    eax
45BB2A  call    sub_45A530
45BB2F  pop     ebp
45BB30  retn

;==== 45BB40 sub_45BB40
45BB40  push    ebp
45BB41  mov     ebp, esp
45BB43  mov     eax, [ebp+arg_0]
45BB46  mov     ecx, [eax]
45BB48  mov     edx, [ebp+arg_4]
45BB4B  mov     eax, [edx]
45BB4D  mov     eax, [eax+18h]
45BB50  sub     eax, [ecx+18h]
45BB53  pop     ebp
45BB54  retn

;==== 45BB60 sub_45BB60
45BB60  push    ebp
45BB61  mov     ebp, esp
45BB63  sub     esp, 24h
45BB66  mov     eax, dword_4FB734
45BB6B  xor     eax, ebp
45BB6D  mov     [ebp+var_4], eax
45BB70  mov     eax, [ebp+arg_0]
45BB73  mov     edx, [ebp+arg_4]
45BB76  mov     [ebp+var_24], eax
45BB79  xor     eax, eax
45BB7B  mov     [ebp+var_20], ecx
45BB7E  cmp     [edx], eax
45BB80  jz      short loc_45BBD7
45BB82  push    ebx
45BB83  push    esi
45BB84  xor     esi, esi
45BB86  lea     ebx, [eax+1]
45BB89  test    edi, edi
45BB8B  jz      short loc_45BBD3
45BB8D  lea     ecx, [ecx+0]
45BB90  mov     eax, [ebp+arg_8]
45BB93  test    eax, eax
45BB95  js      short loc_45BB9B
45BB97  cmp     eax, esi
45BB99  jnz     short loc_45BBCD
45BB9B  mov     eax, [ebp+var_24]
45BB9E  mov     ecx, [eax+10h]
45BBA1  mov     edx, [ecx]
45BBA3  mov     edx, [edx+10h]
45BBA6  push    ebx
45BBA7  push    esi
45BBA8  lea     eax, [ebp+var_1C]
45BBAB  push    eax
45BBAC  call    edx
45BBAE  xor     eax, eax
45BBB0  mov     ecx, [ebp+eax*4+var_1C]
45BBB4  mov     edx, [ebp+arg_4]
45BBB7  cmp     [edx+ecx*4+4], ebx
45BBBB  jnb     short loc_45BBC5
45BBBD  add     eax, ebx
45BBBF  cmp     eax, edi
45BBC1  jb      short loc_45BBB0
45BBC3  jmp     short loc_45BBCD
45BBC5  mov     eax, ecx
45BBC7  mov     ecx, [ebp+var_20]
45BBCA  mov     [ecx+esi*4], eax
45BBCD  add     esi, ebx
45BBCF  cmp     esi, edi
45BBD1  jb      short loc_45BB90
45BBD3  pop     esi
45BBD4  mov     eax, ebx
45BBD6  pop     ebx
45BBD7  mov     ecx, [ebp+var_4]
45BBDA  xor     ecx, ebp
45BBDC  call    sub_4ABDD5
45BBE1  mov     esp, ebp
45BBE3  pop     ebp
45BBE4  retn    0Ch

;==== 477910 sub_477910
477910  push    ebp
477911  mov     ebp, esp
477913  push    ebx
477914  push    esi
477915  push    edi
477916  mov     edi, [ebp+arg_0]
477919  mov     ecx, edi
47791B  call    sub_445480
477920  mov     ecx, edi
477922  mov     esi, eax
477924  call    sub_445480
477929  mov     ecx, edi
47792B  mov     ebx, eax
47792D  call    sub_48EA40
477932  push    esi; int
477933  push    ebx; void *
477934  push    eax; int
477935  call    sub_497260
47793A  mov     esi, eax
47793C  add     esp, 0Ch
47793F  mov     eax, edi
477941  call    sub_4454A0
477946  pop     edi
477947  pop     esi
477948  xor     eax, eax
47794A  pop     ebx
47794B  pop     ebp
47794C  retn

;==== 477950 sub_477950
477950  push    ebp
477951  mov     ebp, esp
477953  push    esi
477954  push    edi
477955  mov     edi, [ebp+arg_0]
477958  mov     ecx, edi
47795A  call    sub_445480
47795F  mov     edx, eax
477961  call    sub_497330
477966  mov     esi, eax
477968  mov     eax, edi
47796A  call    sub_4454A0
47796F  pop     edi
477970  xor     eax, eax
477972  pop     esi
477973  pop     ebp
477974  retn

;==== 477980 sub_477980
477980  push    ebp
477981  mov     ebp, esp
477983  push    ebx
477984  push    esi
477985  push    edi
477986  mov     edi, [ebp+arg_0]
477989  mov     ecx, edi
47798B  call    sub_48EA40
477990  mov     ecx, edi
477992  mov     esi, eax
477994  call    sub_445480
477999  mov     ecx, edi
47799B  mov     ebx, eax
47799D  call    sub_445480
4779A2  mov     ecx, edi
4779A4  mov     edx, eax
4779A6  call    sub_445480
4779AB  push    esi; Src
4779AC  push    ebx; int
4779AD  push    edx; int
4779AE  mov     edx, eax
4779B0  call    sub_4973B0
4779B5  mov     esi, eax
4779B7  add     esp, 0Ch
4779BA  mov     eax, edi
4779BC  call    sub_4454A0
4779C1  pop     edi
4779C2  pop     esi
4779C3  xor     eax, eax
4779C5  pop     ebx
4779C6  pop     ebp
4779C7  retn

;==== 4779D0 sub_4779D0
4779D0  push    ebp
4779D1  mov     ebp, esp
4779D3  push    ebx
4779D4  push    esi
4779D5  push    edi
4779D6  mov     edi, [ebp+arg_0]
4779D9  mov     ecx, edi
4779DB  call    sub_48EA40
4779E0  mov     ecx, edi
4779E2  mov     esi, eax
4779E4  call    sub_445480
4779E9  mov     ecx, edi
4779EB  mov     ebx, eax
4779ED  call    sub_48EA40
4779F2  push    esi; Src
4779F3  push    eax; int
4779F4  mov     edx, ebx
4779F6  call    sub_4973E0
4779FB  mov     esi, eax
4779FD  add     esp, 8
477A00  mov     eax, edi
477A02  call    sub_4454A0
477A07  pop     edi
477A08  pop     esi
477A09  xor     eax, eax
477A0B  pop     ebx
477A0C  pop     ebp
477A0D  retn

;==== 477A10 sub_477A10
477A10  push    ebp
477A11  mov     ebp, esp
477A13  push    esi
477A14  push    edi
477A15  mov     edi, [ebp+arg_0]
477A18  mov     ecx, edi
477A1A  call    sub_445480
477A1F  mov     ecx, edi
477A21  mov     esi, eax
477A23  call    sub_48EA40
477A28  push    eax
477A29  mov     edx, esi
477A2B  call    sub_497410
477A30  mov     esi, eax
477A32  add     esp, 4
477A35  mov     eax, edi
477A37  call    sub_4454A0
477A3C  pop     edi
477A3D  xor     eax, eax
477A3F  pop     esi
477A40  pop     ebp
477A41  retn

;==== 477A50 sub_477A50
477A50  push    ebp
477A51  mov     ebp, esp
477A53  push    esi
477A54  push    edi
477A55  mov     edi, [ebp+arg_0]
477A58  mov     ecx, edi
477A5A  call    sub_445480
477A5F  mov     ecx, edi
477A61  mov     esi, eax
477A63  call    sub_445480
477A68  push    esi
477A69  mov     edx, eax
477A6B  call    sub_497440
477A70  mov     esi, eax
477A72  add     esp, 4
477A75  mov     eax, edi
477A77  call    sub_4454A0
477A7C  pop     edi
477A7D  xor     eax, eax
477A7F  pop     esi
477A80  pop     ebp
477A81  retn

;==== 477A90 sub_477A90
477A90  push    ebp
477A91  mov     ebp, esp
477A93  push    esi
477A94  push    edi
477A95  mov     edi, [ebp+arg_0]
477A98  mov     ecx, edi
477A9A  call    sub_445480
477A9F  mov     ecx, edi
477AA1  mov     esi, eax
477AA3  call    sub_445480
477AA8  push    esi
477AA9  mov     edx, eax
477AAB  call    sub_497470
477AB0  mov     esi, eax
477AB2  add     esp, 4
477AB5  mov     eax, edi
477AB7  call    sub_4454A0
477ABC  pop     edi
477ABD  xor     eax, eax
477ABF  pop     esi
477AC0  pop     ebp
477AC1  retn

;==== 477AD0 sub_477AD0
477AD0  push    ebp
477AD1  mov     ebp, esp
477AD3  push    ebx
477AD4  mov     ebx, [ebp+arg_0]
477AD7  push    esi
477AD8  push    edi
477AD9  mov     ecx, ebx
477ADB  call    sub_445480
477AE0  mov     ecx, ebx
477AE2  mov     esi, eax
477AE4  call    sub_445480
477AE9  mov     ecx, ebx
477AEB  mov     edi, eax
477AED  call    sub_445480
477AF2  mov     ecx, ebx
477AF4  mov     edx, eax
477AF6  call    sub_445480
477AFB  push    edx
477AFC  mov     edx, eax
477AFE  call    sub_4974A0
477B03  mov     esi, eax
477B05  add     esp, 4
477B08  mov     eax, ebx
477B0A  call    sub_4454A0
477B0F  pop     edi
477B10  pop     esi
477B11  xor     eax, eax
477B13  pop     ebx
477B14  pop     ebp
477B15  retn

;==== 477B20 sub_477B20
477B20  push    ebp
477B21  mov     ebp, esp
477B23  push    ebx
477B24  push    esi
477B25  push    edi
477B26  mov     edi, [ebp+arg_0]
477B29  mov     ecx, edi
477B2B  call    sub_445480
477B30  mov     ecx, edi
477B32  mov     ebx, eax
477B34  call    sub_445480
477B39  mov     ecx, edi
477B3B  mov     esi, eax
477B3D  call    sub_445480
477B42  mov     ecx, edi
477B44  mov     edx, eax
477B46  call    sub_445480
477B4B  push    ebx
477B4C  push    edx
477B4D  mov     edx, eax
477B4F  call    sub_497500
477B54  mov     esi, eax
477B56  add     esp, 8
477B59  mov     eax, edi
477B5B  call    sub_4454A0
477B60  pop     edi
477B61  pop     esi
477B62  xor     eax, eax
477B64  pop     ebx
477B65  pop     ebp
477B66  retn

;==== 477B70 sub_477B70
477B70  push    ebp
477B71  mov     ebp, esp
477B73  push    ebx
477B74  push    esi
477B75  push    edi
477B76  mov     edi, [ebp+arg_0]
477B79  mov     ecx, edi
477B7B  call    sub_48EA40
477B80  mov     ecx, edi
477B82  mov     esi, eax
477B84  call    sub_445480
477B89  mov     ecx, edi
477B8B  mov     ebx, eax
477B8D  call    sub_445480
477B92  push    esi
477B93  push    ebx
477B94  mov     edx, eax
477B96  call    sub_497530
477B9B  mov     esi, eax
477B9D  add     esp, 8
477BA0  mov     eax, edi
477BA2  call    sub_4454A0
477BA7  pop     edi
477BA8  pop     esi
477BA9  xor     eax, eax
477BAB  pop     ebx
477BAC  pop     ebp
477BAD  retn

;==== 477BB0 sub_477BB0
477BB0  push    ebp
477BB1  mov     ebp, esp
477BB3  push    ebx
477BB4  push    esi
477BB5  push    edi
477BB6  mov     edi, [ebp+arg_0]
477BB9  mov     ecx, edi
477BBB  call    sub_445480
477BC0  mov     ecx, edi
477BC2  mov     esi, eax
477BC4  call    sub_445480
477BC9  mov     ecx, edi
477BCB  mov     ebx, eax
477BCD  call    sub_445480
477BD2  push    esi
477BD3  push    ebx
477BD4  mov     edx, eax
477BD6  call    sub_497560
477BDB  mov     esi, eax
477BDD  add     esp, 8
477BE0  mov     eax, edi
477BE2  call    sub_4454A0
477BE7  pop     edi
477BE8  pop     esi
477BE9  xor     eax, eax
477BEB  pop     ebx
477BEC  pop     ebp
477BED  retn

;==== 477BF0 sub_477BF0
477BF0  push    ebp
477BF1  mov     ebp, esp
477BF3  push    ebx
477BF4  mov     ebx, [ebp+arg_0]
477BF7  push    esi
477BF8  push    edi
477BF9  mov     ecx, ebx
477BFB  call    sub_445480
477C00  mov     ecx, ebx
477C02  mov     edi, eax
477C04  call    sub_445480
477C09  mov     ecx, ebx
477C0B  mov     esi, eax
477C0D  call    sub_445480
477C12  push    esi
477C13  mov     edx, eax
477C15  call    sub_4974D0
477C1A  mov     esi, eax
477C1C  add     esp, 4
477C1F  mov     eax, ebx
477C21  call    sub_4454A0
477C26  pop     edi
477C27  pop     esi
477C28  xor     eax, eax
477C2A  pop     ebx
477C2B  pop     ebp
477C2C  retn

;==== 477C30 sub_477C30
477C30  push    ebp
477C31  mov     ebp, esp
477C33  sub     esp, 8
477C36  push    ebx
477C37  push    esi
477C38  push    edi
477C39  mov     edi, [ebp+arg_0]
477C3C  mov     ecx, edi
477C3E  call    sub_445480
477C43  mov     ecx, edi
477C45  mov     esi, eax
477C47  call    sub_445480
477C4C  mov     ecx, edi
477C4E  mov     ebx, eax
477C50  call    sub_445480
477C55  mov     ecx, edi
477C57  mov     edx, eax
477C59  call    sub_445480
477C5E  mov     ecx, edi
477C60  mov     [ebp+var_4], eax
477C63  call    sub_445480
477C68  mov     ecx, edi
477C6A  mov     [ebp+var_8], eax
477C6D  call    sub_445480
477C72  mov     ecx, [ebp+var_4]
477C75  push    esi
477C76  push    ebx
477C77  push    edx
477C78  mov     edx, [ebp+var_8]
477C7B  push    ecx
477C7C  push    edx
477C7D  mov     edx, eax
477C7F  call    sub_497590
477C84  mov     esi, eax
477C86  add     esp, 14h
477C89  mov     eax, edi
477C8B  call    sub_4454A0
477C90  pop     edi
477C91  pop     esi
477C92  xor     eax, eax
477C94  pop     ebx
477C95  mov     esp, ebp
477C97  pop     ebp
477C98  retn

;==== 477CA0 sub_477CA0
477CA0  push    ebp
477CA1  mov     ebp, esp
477CA3  sub     esp, 0Ch
477CA6  push    ebx
477CA7  push    esi
477CA8  push    edi
477CA9  mov     edi, [ebp+arg_0]
477CAC  mov     ecx, edi
477CAE  call    sub_445480
477CB3  mov     ecx, edi
477CB5  mov     esi, eax
477CB7  call    sub_445480
477CBC  mov     ecx, edi
477CBE  mov     ebx, eax
477CC0  call    sub_445480
477CC5  mov     ecx, edi
477CC7  mov     [ebp+var_4], eax
477CCA  call    sub_445480
477CCF  mov     ecx, edi
477CD1  mov     [ebp+var_C], eax
477CD4  call    sub_48EA40
477CD9  mov     ecx, edi
477CDB  mov     [ebp+var_8], eax
477CDE  call    sub_48EA40
477CE3  mov     ecx, [ebp+var_4]
477CE6  mov     edx, [ebp+var_8]
477CE9  push    esi
477CEA  push    ebx
477CEB  push    ecx
477CEC  push    edx
477CED  mov     edx, [ebp+var_C]
477CF0  push    eax
477CF1  call    sub_4975D0
477CF6  mov     esi, eax
477CF8  add     esp, 14h
477CFB  mov     eax, edi
477CFD  call    sub_4454A0
477D02  pop     edi
477D03  pop     esi
477D04  xor     eax, eax
477D06  pop     ebx
477D07  mov     esp, ebp
477D09  pop     ebp
477D0A  retn

;==== 477D10 sub_477D10
477D10  push    ebp
477D11  mov     ebp, esp
477D13  push    ebx
477D14  push    esi
477D15  push    edi
477D16  mov     edi, [ebp+arg_0]
477D19  mov     ecx, edi
477D1B  call    sub_445480
477D20  mov     ecx, edi
477D22  mov     esi, eax
477D24  call    sub_445480
477D29  mov     ecx, edi
477D2B  mov     ebx, eax
477D2D  call    sub_48EA40
477D32  push    esi; int
477D33  push    eax; void *
477D34  mov     edx, ebx
477D36  call    sub_497610
477D3B  mov     esi, eax
477D3D  add     esp, 8
477D40  mov     eax, edi
477D42  call    sub_4454A0
477D47  pop     edi
477D48  pop     esi
477D49  xor     eax, eax
477D4B  pop     ebx
477D4C  pop     ebp
477D4D  retn

;==== 477D50 sub_477D50
477D50  push    ebp
477D51  mov     ebp, esp
477D53  push    ecx
477D54  push    ebx
477D55  push    esi
477D56  push    edi
477D57  mov     edi, [ebp+arg_0]
477D5A  mov     ecx, edi
477D5C  call    sub_445480
477D61  mov     ecx, edi
477D63  mov     esi, eax
477D65  call    sub_445480
477D6A  mov     ecx, edi
477D6C  mov     ebx, eax
477D6E  call    sub_48EA40
477D73  mov     ecx, edi
477D75  mov     [ebp+var_4], eax
477D78  call    sub_48EA40
477D7D  mov     ecx, [ebp+var_4]
477D80  push    esi
477D81  push    ecx
477D82  push    eax
477D83  mov     edx, ebx
477D85  call    sub_497640
477D8A  mov     esi, eax
477D8C  add     esp, 0Ch
477D8F  mov     eax, edi
477D91  call    sub_4454A0
477D96  pop     edi
477D97  pop     esi
477D98  xor     eax, eax
477D9A  pop     ebx
477D9B  mov     esp, ebp
477D9D  pop     ebp
477D9E  retn

;==== 477DA0 sub_477DA0
477DA0  push    ebp
477DA1  mov     ebp, esp
477DA3  sub     esp, 20h
477DA6  push    ebx
477DA7  push    esi
477DA8  push    edi
477DA9  mov     edi, [ebp+arg_0]
477DAC  mov     ecx, edi
477DAE  call    sub_445480
477DB3  mov     ecx, edi
477DB5  mov     esi, eax
477DB7  call    sub_445480
477DBC  mov     ecx, edi
477DBE  mov     ebx, eax
477DC0  call    sub_445480
477DC5  mov     ecx, edi
477DC7  mov     [ebp+var_4], eax
477DCA  call    sub_445480
477DCF  mov     ecx, edi
477DD1  mov     [ebp+var_8], eax
477DD4  call    sub_445480
477DD9  mov     ecx, edi
477DDB  mov     [ebp+var_C], eax
477DDE  call    sub_445480
477DE3  mov     ecx, edi
477DE5  mov     [ebp+var_10], eax
477DE8  call    sub_445480
477DED  mov     ecx, edi
477DEF  mov     [ebp+var_14], eax
477DF2  call    sub_445480
477DF7  mov     ecx, edi
477DF9  mov     [ebp+var_20], eax
477DFC  call    sub_48EA40
477E01  mov     ecx, edi
477E03  mov     [ebp+var_18], eax
477E06  call    sub_48EA40
477E0B  mov     ecx, edi
477E0D  mov     [ebp+var_1C], eax
477E10  call    sub_48EA40
477E15  mov     ecx, [ebp+var_4]
477E18  mov     edx, [ebp+var_8]
477E1B  push    esi
477E1C  push    ebx
477E1D  push    ecx
477E1E  mov     ecx, [ebp+var_C]
477E21  push    edx
477E22  mov     edx, [ebp+var_10]
477E25  push    ecx
477E26  mov     ecx, [ebp+var_14]
477E29  push    edx
477E2A  mov     edx, [ebp+var_18]
477E2D  push    ecx
477E2E  mov     ecx, [ebp+var_1C]
477E31  push    edx
477E32  mov     edx, [ebp+var_20]
477E35  push    ecx
477E36  push    eax
477E37  call    sub_497670
477E3C  mov     esi, eax
477E3E  add     esp, 28h
477E41  mov     eax, edi
477E43  call    sub_4454A0
477E48  pop     edi
477E49  pop     esi
477E4A  xor     eax, eax
477E4C  pop     ebx
477E4D  mov     esp, ebp
477E4F  pop     ebp
477E50  retn

;==== 477E60 sub_477E60
477E60  push    ebp
477E61  mov     ebp, esp
477E63  sub     esp, 8
477E66  push    ebx
477E67  push    esi
477E68  push    edi
477E69  mov     edi, [ebp+arg_0]
477E6C  mov     ecx, edi
477E6E  call    sub_445480
477E73  mov     ecx, edi
477E75  mov     esi, eax
477E77  call    sub_445480
477E7C  mov     ecx, edi
477E7E  mov     ebx, eax
477E80  call    sub_445480
477E85  mov     ecx, edi
477E87  mov     [ebp+var_4], eax
477E8A  call    sub_445480
477E8F  mov     ecx, edi
477E91  mov     [ebp+var_8], eax
477E94  call    sub_48EA40
477E99  mov     ecx, [ebp+var_4]
477E9C  mov     edx, [ebp+var_8]
477E9F  push    esi
477EA0  push    ecx
477EA1  push    edx
477EA2  push    eax
477EA3  mov     edx, ebx
477EA5  call    sub_4976C0
477EAA  mov     esi, eax
477EAC  add     esp, 10h
477EAF  mov     eax, edi
477EB1  call    sub_4454A0
477EB6  pop     edi
477EB7  pop     esi
477EB8  xor     eax, eax
477EBA  pop     ebx
477EBB  mov     esp, ebp
477EBD  pop     ebp
477EBE  retn

;==== 477EC0 sub_477EC0
477EC0  push    ebp
477EC1  mov     ebp, esp
477EC3  sub     esp, 0Ch
477EC6  push    ebx
477EC7  push    esi
477EC8  push    edi
477EC9  mov     edi, [ebp+arg_0]
477ECC  mov     ecx, edi
477ECE  call    sub_445480
477ED3  mov     ecx, edi
477ED5  mov     esi, eax
477ED7  call    sub_445480
477EDC  mov     ecx, edi
477EDE  mov     ebx, eax
477EE0  call    sub_445480
477EE5  mov     ecx, edi
477EE7  mov     [ebp+var_4], eax
477EEA  call    sub_445480
477EEF  mov     ecx, edi
477EF1  mov     [ebp+var_8], eax
477EF4  call    sub_445480
477EF9  mov     ecx, edi
477EFB  mov     [ebp+var_C], eax
477EFE  call    sub_48EA40
477F03  mov     ecx, [ebp+var_4]
477F06  mov     edx, [ebp+var_8]
477F09  push    esi
477F0A  push    ebx
477F0B  push    ecx
477F0C  push    edx
477F0D  mov     edx, [ebp+var_C]
477F10  push    eax
477F11  call    sub_497700
477F16  mov     esi, eax
477F18  add     esp, 14h
477F1B  mov     eax, edi
477F1D  call    sub_4454A0
477F22  pop     edi
477F23  pop     esi
477F24  xor     eax, eax
477F26  pop     ebx
477F27  mov     esp, ebp
477F29  pop     ebp
477F2A  retn

;==== 477F30 sub_477F30
477F30  push    ebp
477F31  mov     ebp, esp
477F33  push    esi
477F34  push    edi
477F35  mov     edi, [ebp+arg_0]
477F38  mov     ecx, edi
477F3A  call    sub_48EA40
477F3F  push    eax
477F40  call    sub_497740
477F45  mov     esi, eax
477F47  add     esp, 4
477F4A  mov     eax, edi
477F4C  call    sub_4454A0
477F51  pop     edi
477F52  xor     eax, eax
477F54  pop     esi
477F55  pop     ebp
477F56  retn

;==== 477F60 sub_477F60
477F60  push    ebp
477F61  mov     ebp, esp
477F63  push    esi
477F64  push    edi
477F65  mov     edi, [ebp+arg_0]
477F68  mov     ecx, edi
477F6A  call    sub_445480
477F6F  mov     edx, eax
477F71  call    sub_4977E0
477F76  mov     esi, eax
477F78  mov     eax, edi
477F7A  call    sub_4454A0
477F7F  pop     edi
477F80  xor     eax, eax
477F82  pop     esi
477F83  pop     ebp
477F84  retn

;==== 477F90 sub_477F90
477F90  push    ebp
477F91  mov     ebp, esp
477F93  sub     esp, 24h
477F96  push    ebx
477F97  push    esi
477F98  push    edi
477F99  mov     edi, [ebp+arg_0]
477F9C  mov     ecx, edi
477F9E  call    sub_445480
477FA3  mov     ecx, edi
477FA5  mov     esi, eax
477FA7  call    sub_445480
477FAC  mov     ecx, edi
477FAE  mov     ebx, eax
477FB0  call    sub_445480
477FB5  mov     ecx, edi
477FB7  mov     [ebp+var_4], eax
477FBA  call    sub_445480
477FBF  mov     ecx, edi
477FC1  mov     [ebp+var_8], eax
477FC4  call    sub_445480
477FC9  mov     ecx, edi
477FCB  mov     [ebp+var_C], eax
477FCE  call    sub_445480
477FD3  mov     ecx, edi
477FD5  mov     [ebp+var_10], eax
477FD8  call    sub_445480
477FDD  mov     ecx, edi
477FDF  mov     [ebp+var_14], eax
477FE2  call    sub_445480
477FE7  mov     ecx, edi
477FE9  mov     [ebp+var_18], eax
477FEC  call    sub_445480
477FF1  mov     ecx, edi
477FF3  mov     [ebp+var_1C], eax
477FF6  call    sub_445480
477FFB  mov     ecx, edi
477FFD  mov     [ebp+var_20], eax
478000  call    sub_445480
478005  mov     ecx, edi
478007  mov     [ebp+var_24], eax
47800A  call    sub_445480
47800F  mov     ecx, edi
478011  mov     edx, eax
478013  call    sub_445480
478018  fild    [ebp+var_4]
47801B  fld     ds:dbl_4EC968
478021  push    esi; int
478022  push    ebx; int
478023  fmul    st(1), st
478025  sub     esp, 24h
478028  fxch    st(1)
47802A  fstp    [ebp+var_4]
47802D  fld     [ebp+var_4]
478030  fstp    [esp+5Ch+var_3C]; float
478034  fild    [ebp+var_8]
478037  fmul    st, st(1)
478039  fstp    [ebp+var_8]
47803C  fld     [ebp+var_8]
47803F  fstp    [esp+5Ch+var_40]; float
478043  fild    [ebp+var_C]
478046  fmul    st, st(1)
478048  fstp    [ebp+var_C]
47804B  fld     [ebp+var_C]
47804E  fstp    [esp+5Ch+var_44]; float
478052  fild    [ebp+var_10]
478055  fmul    st, st(1)
478057  fstp    [ebp+var_10]
47805A  fld     [ebp+var_10]
47805D  fstp    [esp+5Ch+var_48]; float
478061  fild    [ebp+var_14]
478064  fmul    st, st(1)
478066  fstp    [ebp+var_14]
478069  fld     [ebp+var_14]
47806C  fstp    [esp+5Ch+var_4C]; float
478070  fild    [ebp+var_18]
478073  fmul    st, st(1)
478075  fstp    [ebp+var_18]
478078  fld     [ebp+var_18]
47807B  fstp    [esp+5Ch+var_50]; float
47807F  fild    [ebp+var_1C]
478082  fmul    st, st(1)
478084  fstp    [ebp+var_1C]
478087  fld     [ebp+var_1C]
47808A  fstp    [esp+5Ch+var_54]; float
47808E  fild    [ebp+var_20]
478091  fmul    st, st(1)
478093  fstp    [ebp+var_20]
478096  fld     [ebp+var_20]
478099  fstp    [esp+5Ch+var_58]; float
47809D  fimul   [ebp+var_24]
4780A0  fstp    [ebp+var_24]
4780A3  fld     [ebp+var_24]
4780A6  fstp    [esp+5Ch+var_5C]; float
4780A9  push    edx; int
4780AA  mov     edx, eax
4780AC  call    sub_497880
4780B1  mov     esi, eax
4780B3  add     esp, 30h
4780B6  mov     eax, edi
4780B8  call    sub_4454A0
4780BD  pop     edi
4780BE  pop     esi
4780BF  xor     eax, eax
4780C1  pop     ebx
4780C2  mov     esp, ebp
4780C4  pop     ebp
4780C5  retn

;==== 4780D0 sub_4780D0
4780D0  push    ebp
4780D1  mov     ebp, esp
4780D3  push    esi
4780D4  push    edi
4780D5  mov     edi, [ebp+arg_0]
4780D8  mov     ecx, edi
4780DA  call    sub_445480
4780DF  mov     ecx, edi
4780E1  mov     esi, eax
4780E3  call    sub_445480
4780E8  push    esi
4780E9  mov     edx, eax
4780EB  call    sub_497900
4780F0  mov     esi, eax
4780F2  add     esp, 4
4780F5  mov     eax, edi
4780F7  call    sub_4454A0
4780FC  pop     edi
4780FD  xor     eax, eax
4780FF  pop     esi
478100  pop     ebp
478101  retn

;==== 478110 sub_478110
478110  push    ebp
478111  mov     ebp, esp
478113  sub     esp, 0Ch
478116  push    esi
478117  push    edi
478118  mov     edi, [ebp+arg_0]
47811B  mov     ecx, edi
47811D  call    sub_445480
478122  mov     ecx, edi
478124  mov     [ebp+var_4], eax
478127  call    sub_445480
47812C  mov     ecx, edi
47812E  mov     [ebp+var_8], eax
478131  call    sub_445480
478136  mov     ecx, edi
478138  mov     [ebp+var_C], eax
47813B  call    sub_445480
478140  mov     ecx, edi
478142  mov     esi, eax
478144  call    sub_445480
478149  fild    [ebp+var_4]
47814C  fld     ds:dbl_4EC968
478152  sub     esp, 0Ch
478155  mov     edx, eax
478157  fmul    st(1), st
478159  fxch    st(1)
47815B  fstp    [ebp+var_4]
47815E  fld     [ebp+var_4]
478161  fstp    [esp+20h+var_18]; float
478165  fild    [ebp+var_8]
478168  fmul    st, st(1)
47816A  fstp    [ebp+var_8]
47816D  fld     [ebp+var_8]
478170  fstp    [esp+20h+var_1C]; float
478174  fimul   [ebp+var_C]
478177  fstp    [ebp+var_C]
47817A  fld     [ebp+var_C]
47817D  fstp    [esp+20h+var_20]; float
478180  push    esi; int
478181  call    sub_497930
478186  mov     esi, eax
478188  add     esp, 10h
47818B  mov     eax, edi
47818D  call    sub_4454A0
478192  pop     edi
478193  xor     eax, eax
478195  pop     esi
478196  mov     esp, ebp
478198  pop     ebp
478199  retn

;==== 4781A0 sub_4781A0
4781A0  push    ebp
4781A1  mov     ebp, esp
4781A3  and     esp, 0FFFFFFF0h
4781A6  sub     esp, 24h
4781A9  push    ebx
4781AA  push    esi
4781AB  push    edi
4781AC  mov     edi, [ebp+arg_0]
4781AF  mov     ecx, edi
4781B1  call    sub_445480
4781B6  mov     ecx, edi
4781B8  mov     esi, eax
4781BA  call    sub_445480
4781BF  mov     ecx, edi
4781C1  mov     ebx, eax
4781C3  call    sub_48EA40
4781C8  mov     [esp+30h+var_14], eax
4781CC  lea     eax, [esp+30h+var_10]
4781D0  push    esi
4781D1  push    eax
4781D2  mov     edx, ebx
4781D4  call    sub_497970
4781D9  mov     esi, eax
4781DB  add     esp, 8
4781DE  test    esi, esi
4781E0  jnz     short loc_478221
4781E2  stmxcsr [esp+30h+var_18]
4781E7  mov     ecx, [esp+30h+var_18]
4781EB  and     ecx, 0F3FFh
4781F1  mov     [esp+30h+var_18], ecx
4781F5  ldmxcsr [esp+30h+var_18]
4781FA  lea     eax, [esp+30h+var_10]
4781FE  call    sub_455940
478203  mov     eax, [esp+30h+var_14]
478207  movdqa  [esp+30h+var_10], xmm0
47820D  mov     edx, dword ptr [esp+30h+var_10]
478211  mov     ecx, dword ptr [esp+30h+var_10+4]
478215  mov     [eax], edx
478217  mov     edx, dword ptr [esp+30h+var_10+8]
47821B  mov     [eax+4], ecx
47821E  mov     [eax+8], edx
478221  mov     eax, edi
478223  call    sub_4454A0
478228  pop     edi
478229  pop     esi
47822A  xor     eax, eax
47822C  pop     ebx
47822D  mov     esp, ebp
47822F  pop     ebp
478230  retn

;==== 478240 sub_478240
478240  push    ebp
478241  mov     ebp, esp
478243  sub     esp, 0Ch
478246  push    esi
478247  push    edi
478248  mov     edi, [ebp+arg_0]
47824B  mov     ecx, edi
47824D  call    sub_445480
478252  mov     ecx, edi
478254  mov     [ebp+var_4], eax
478257  call    sub_445480
47825C  mov     ecx, edi
47825E  mov     [ebp+var_8], eax
478261  call    sub_445480
478266  mov     ecx, edi
478268  mov     [ebp+var_C], eax
47826B  call    sub_445480
478270  mov     ecx, edi
478272  mov     esi, eax
478274  call    sub_445480
478279  fild    [ebp+var_4]
47827C  fld     ds:dbl_4EC968
478282  sub     esp, 0Ch
478285  mov     edx, eax
478287  fmul    st(1), st
478289  fxch    st(1)
47828B  fstp    [ebp+var_4]
47828E  fld     [ebp+var_4]
478291  fstp    [esp+20h+var_18]; float
478295  fild    [ebp+var_8]
478298  fmul    st, st(1)
47829A  fstp    [ebp+var_8]
47829D  fld     [ebp+var_8]
4782A0  fstp    [esp+20h+var_1C]; float
4782A4  fimul   [ebp+var_C]
4782A7  fstp    [ebp+var_C]
4782AA  fld     [ebp+var_C]
4782AD  fstp    [esp+20h+var_20]; float
4782B0  push    esi; int
4782B1  call    sub_4979A0
4782B6  mov     esi, eax
4782B8  add     esp, 10h
4782BB  mov     eax, edi
4782BD  call    sub_4454A0
4782C2  pop     edi
4782C3  xor     eax, eax
4782C5  pop     esi
4782C6  mov     esp, ebp
4782C8  pop     ebp
4782C9  retn

;==== 4782D0 sub_4782D0
4782D0  push    ebp
4782D1  mov     ebp, esp
4782D3  and     esp, 0FFFFFFF0h
4782D6  sub     esp, 24h
4782D9  push    ebx
4782DA  push    esi
4782DB  push    edi
4782DC  mov     edi, [ebp+arg_0]
4782DF  mov     ecx, edi
4782E1  call    sub_445480
4782E6  mov     ecx, edi
4782E8  mov     esi, eax
4782EA  call    sub_445480
4782EF  mov     ecx, edi
4782F1  mov     ebx, eax
4782F3  call    sub_48EA40
4782F8  mov     [esp+30h+var_14], eax
4782FC  lea     eax, [esp+30h+var_10]
478300  push    esi
478301  push    eax
478302  mov     edx, ebx
478304  call    sub_4979E0
478309  mov     esi, eax
47830B  add     esp, 8
47830E  test    esi, esi
478310  jnz     short loc_478351
478312  stmxcsr [esp+30h+var_18]
478317  mov     ecx, [esp+30h+var_18]
47831B  and     ecx, 0F3FFh
478321  mov     [esp+30h+var_18], ecx
478325  ldmxcsr [esp+30h+var_18]
47832A  lea     eax, [esp+30h+var_10]
47832E  call    sub_455940
478333  mov     eax, [esp+30h+var_14]
478337  movdqa  [esp+30h+var_10], xmm0
47833D  mov     edx, dword ptr [esp+30h+var_10]
478341  mov     ecx, dword ptr [esp+30h+var_10+4]
478345  mov     [eax], edx
478347  mov     edx, dword ptr [esp+30h+var_10+8]
47834B  mov     [eax+4], ecx
47834E  mov     [eax+8], edx
478351  mov     eax, edi
478353  call    sub_4454A0
478358  pop     edi
478359  pop     esi
47835A  xor     eax, eax
47835C  pop     ebx
47835D  mov     esp, ebp
47835F  pop     ebp
478360  retn

;==== 478370 sub_478370
478370  push    ebp
478371  mov     ebp, esp
478373  push    ebx
478374  push    esi
478375  push    edi
478376  mov     edi, [ebp+arg_0]
478379  mov     ecx, edi
47837B  call    sub_445480
478380  mov     ecx, edi
478382  mov     esi, eax
478384  call    sub_445480
478389  mov     ecx, edi
47838B  mov     ebx, eax
47838D  call    sub_445480
478392  mov     ecx, edi
478394  mov     edx, eax
478396  call    sub_445480
47839B  push    esi
47839C  push    ebx
47839D  push    edx
47839E  mov     edx, eax
4783A0  call    sub_497A10
4783A5  mov     esi, eax
4783A7  add     esp, 0Ch
4783AA  mov     eax, edi
4783AC  call    sub_4454A0
4783B1  pop     edi
4783B2  pop     esi
4783B3  xor     eax, eax
4783B5  pop     ebx
4783B6  pop     ebp
4783B7  retn

;==== 4783C0 sub_4783C0
4783C0  push    ebp
4783C1  mov     ebp, esp
4783C3  push    ecx
4783C4  push    ebx
4783C5  push    esi
4783C6  push    edi
4783C7  mov     edi, [ebp+arg_0]
4783CA  mov     ecx, edi
4783CC  call    sub_445480
4783D1  mov     ecx, edi
4783D3  mov     esi, eax
4783D5  call    sub_445480
4783DA  mov     ecx, edi
4783DC  mov     ebx, eax
4783DE  call    sub_445480
4783E3  mov     ecx, edi
4783E5  mov     [ebp+var_4], eax
4783E8  call    sub_48EA40
4783ED  mov     edx, [ebp+var_4]
4783F0  push    esi
4783F1  push    ebx
4783F2  push    eax
4783F3  call    sub_497A40
4783F8  mov     esi, eax
4783FA  add     esp, 0Ch
4783FD  mov     eax, edi
4783FF  call    sub_4454A0
478404  pop     edi
478405  pop     esi
478406  xor     eax, eax
478408  pop     ebx
478409  mov     esp, ebp
47840B  pop     ebp
47840C  retn

;==== 478410 sub_478410
478410  push    ebp
478411  mov     ebp, esp
478413  sub     esp, 8
478416  push    ebx
478417  mov     ebx, [ebp+arg_0]
47841A  push    esi
47841B  push    edi
47841C  mov     ecx, ebx
47841E  call    sub_445480
478423  mov     ecx, ebx
478425  mov     edi, eax
478427  call    sub_445480
47842C  mov     ecx, ebx
47842E  mov     esi, eax
478430  call    sub_445480
478435  mov     ecx, ebx
478437  mov     [ebp+var_8], eax
47843A  call    sub_48EA40
47843F  mov     ecx, ebx
478441  mov     [ebp+var_4], eax
478444  call    sub_48EA40
478449  mov     ecx, [ebp+var_4]
47844C  mov     edx, [ebp+var_8]
47844F  push    esi
478450  push    ecx
478451  push    eax
478452  call    sub_497A70
478457  mov     esi, eax
478459  add     esp, 0Ch
47845C  mov     eax, ebx
47845E  call    sub_4454A0
478463  pop     edi
478464  pop     esi
478465  xor     eax, eax
478467  pop     ebx
478468  mov     esp, ebp
47846A  pop     ebp
47846B  retn

;==== 478470 sub_478470
478470  push    ebp
478471  mov     ebp, esp
478473  sub     esp, 0Ch
478476  push    ebx
478477  push    esi
478478  push    edi
478479  mov     edi, [ebp+arg_0]
47847C  mov     ecx, edi
47847E  call    sub_445480
478483  mov     ecx, edi
478485  mov     esi, eax
478487  call    sub_445480
47848C  mov     ecx, edi
47848E  mov     ebx, eax
478490  call    sub_445480
478495  mov     ecx, edi
478497  mov     [ebp+var_4], eax
47849A  call    sub_445480
47849F  mov     ecx, edi
4784A1  mov     [ebp+var_8], eax
4784A4  call    sub_445480
4784A9  mov     ecx, edi
4784AB  mov     [ebp+var_C], eax
4784AE  call    sub_445480
4784B3  mov     ecx, edi
4784B5  mov     edx, eax
4784B7  call    sub_445480
4784BC  fild    [ebp+var_4]
4784BF  fld     ds:dbl_4EC968
4784C5  push    esi; int
4784C6  push    ebx; int
4784C7  fmul    st(1), st
4784C9  sub     esp, 0Ch
4784CC  fxch    st(1)
4784CE  fstp    [ebp+var_4]
4784D1  fld     [ebp+var_4]
4784D4  fstp    [esp+2Ch+var_24]; float
4784D8  fild    [ebp+var_8]
4784DB  fmul    st, st(1)
4784DD  fstp    [ebp+var_8]
4784E0  fld     [ebp+var_8]
4784E3  fstp    [esp+2Ch+var_28]; float
4784E7  fimul   [ebp+var_C]
4784EA  fstp    [ebp+var_C]
4784ED  fld     [ebp+var_C]
4784F0  fstp    [esp+2Ch+var_2C]; float
4784F3  push    edx; int
4784F4  mov     edx, eax
4784F6  call    sub_497AA0
4784FB  mov     esi, eax
4784FD  add     esp, 18h
478500  mov     eax, edi
478502  call    sub_4454A0
478507  pop     edi
478508  pop     esi
478509  xor     eax, eax
47850B  pop     ebx
47850C  mov     esp, ebp
47850E  pop     ebp
47850F  retn

;==== 478510 sub_478510
478510  push    ebp
478511  mov     ebp, esp
478513  sub     esp, 20h
478516  push    ebx
478517  push    esi
478518  push    edi
478519  mov     edi, [ebp+arg_0]
47851C  mov     ecx, edi
47851E  call    sub_445480
478523  mov     ecx, edi
478525  mov     esi, eax
478527  call    sub_445480
47852C  mov     ecx, edi
47852E  mov     ebx, eax
478530  call    sub_445480
478535  mov     ecx, edi
478537  mov     [ebp+var_4], eax
47853A  call    sub_445480
47853F  mov     ecx, edi
478541  mov     [ebp+var_8], eax
478544  call    sub_445480
478549  mov     ecx, edi
47854B  mov     [ebp+var_C], eax
47854E  call    sub_445480
478553  mov     ecx, edi
478555  mov     [ebp+var_10], eax
478558  call    sub_445480
47855D  mov     ecx, edi
47855F  mov     [ebp+var_14], eax
478562  call    sub_445480
478567  mov     ecx, edi
478569  mov     [ebp+var_18], eax
47856C  call    sub_445480
478571  mov     ecx, edi
478573  mov     [ebp+var_20], eax
478576  call    sub_48EA40
47857B  mov     ecx, edi
47857D  mov     [ebp+var_1C], eax
478580  call    sub_48EA40
478585  fild    [ebp+var_C]
478588  fld     ds:dbl_4EC968
47858E  mov     ecx, [ebp+var_4]
478591  mov     edx, [ebp+var_8]
478594  push    esi; int
478595  fmul    st(1), st
478597  push    ebx; int
478598  fxch    st(1)
47859A  push    ecx; int
47859B  mov     ecx, [ebp+var_14]
47859E  fstp    [ebp+var_C]
4785A1  fld     [ebp+var_C]
4785A4  push    edx; int
4785A5  mov     edx, [ebp+var_18]
4785A8  sub     esp, 8
4785AB  fstp    [esp+44h+var_40]; float
4785AF  fimul   [ebp+var_10]
4785B2  fstp    [ebp+var_10]
4785B5  fld     [ebp+var_10]
4785B8  fstp    [esp+44h+var_44]; float
4785BB  push    ecx; int
4785BC  mov     ecx, [ebp+var_1C]
4785BF  push    edx; int
4785C0  mov     edx, [ebp+var_20]
4785C3  push    ecx; int
4785C4  push    eax; int
4785C5  call    sub_497AF0
4785CA  mov     esi, eax
4785CC  add     esp, 28h
4785CF  mov     eax, edi
4785D1  call    sub_4454A0
4785D6  pop     edi
4785D7  pop     esi
4785D8  xor     eax, eax
4785DA  pop     ebx
4785DB  mov     esp, ebp
4785DD  pop     ebp
4785DE  retn

;==== 4785E0 sub_4785E0
4785E0  push    ebp
4785E1  mov     ebp, esp
4785E3  sub     esp, 8
4785E6  push    ebx
4785E7  push    esi
4785E8  push    edi
4785E9  mov     edi, [ebp+arg_0]
4785EC  mov     ecx, edi
4785EE  call    sub_445480
4785F3  mov     ecx, edi
4785F5  mov     esi, eax
4785F7  call    sub_445480
4785FC  mov     ecx, edi
4785FE  mov     ebx, eax
478600  call    sub_445480
478605  mov     ecx, edi
478607  mov     [ebp+var_8], eax
47860A  call    sub_48EA40
47860F  mov     ecx, edi
478611  mov     [ebp+var_4], eax
478614  call    sub_48EA40
478619  mov     ecx, [ebp+var_4]
47861C  mov     edx, [ebp+var_8]
47861F  push    esi
478620  push    ebx
478621  push    ecx
478622  push    eax
478623  call    sub_497B50
478628  mov     esi, eax
47862A  add     esp, 10h
47862D  mov     eax, edi
47862F  call    sub_4454A0
478634  pop     edi
478635  pop     esi
478636  xor     eax, eax
478638  pop     ebx
478639  mov     esp, ebp
47863B  pop     ebp
47863C  retn

;==== 478640 sub_478640
478640  push    ebp
478641  mov     ebp, esp
478643  sub     esp, 18h
478646  push    ebx
478647  push    esi
478648  push    edi
478649  mov     edi, [ebp+arg_0]
47864C  mov     ecx, edi
47864E  call    sub_445480
478653  mov     ecx, edi
478655  mov     esi, eax
478657  call    sub_445480
47865C  mov     ecx, edi
47865E  mov     ebx, eax
478660  call    sub_445480
478665  mov     ecx, edi
478667  mov     [ebp+var_4], eax
47866A  call    sub_445480
47866F  mov     ecx, edi
478671  mov     [ebp+var_8], eax
478674  call    sub_445480
478679  mov     ecx, edi
47867B  mov     [ebp+var_C], eax
47867E  call    sub_445480
478683  mov     ecx, edi
478685  mov     [ebp+var_10], eax
478688  call    sub_445480
47868D  mov     ecx, edi
47868F  mov     [ebp+var_18], eax
478692  call    sub_48EA40
478697  mov     ecx, edi
478699  mov     [ebp+var_14], eax
47869C  call    sub_48EA40
4786A1  fild    [ebp+var_4]
4786A4  fld     ds:dbl_4EC968
4786AA  push    esi; int
4786AB  push    ebx; int
4786AC  fmul    st(1), st
4786AE  sub     esp, 10h
4786B1  fxch    st(1)
4786B3  mov     ecx, [ebp+var_14]
4786B6  mov     edx, [ebp+var_18]
4786B9  fstp    [ebp+var_4]
4786BC  fld     [ebp+var_4]
4786BF  fstp    [esp+3Ch+var_30]; float
4786C3  fild    [ebp+var_8]
4786C6  fmul    st, st(1)
4786C8  fstp    [ebp+var_8]
4786CB  fld     [ebp+var_8]
4786CE  fstp    [esp+3Ch+var_34]; float
4786D2  fild    [ebp+var_C]
4786D5  fmul    st, st(1)
4786D7  fstp    [ebp+var_C]
4786DA  fld     [ebp+var_C]
4786DD  fstp    [esp+3Ch+var_38]; float
4786E1  fimul   [ebp+var_10]
4786E4  fstp    [ebp+var_10]
4786E7  fld     [ebp+var_10]
4786EA  fstp    [esp+3Ch+var_3C]; float
4786ED  push    ecx; int
4786EE  push    eax; int
4786EF  call    sub_497B90
4786F4  mov     esi, eax
4786F6  add     esp, 20h
4786F9  mov     eax, edi
4786FB  call    sub_4454A0
478700  pop     edi
478701  pop     esi
478702  xor     eax, eax
478704  pop     ebx
478705  mov     esp, ebp
478707  pop     ebp
478708  retn

;==== 478710 sub_478710
478710  push    ebp
478711  mov     ebp, esp
478713  sub     esp, 8
478716  push    ebx
478717  push    esi
478718  push    edi
478719  mov     edi, [ebp+arg_0]
47871C  mov     ecx, edi
47871E  call    sub_445480
478723  mov     ecx, edi
478725  mov     esi, eax
478727  call    sub_445480
47872C  mov     ecx, edi
47872E  mov     ebx, eax
478730  call    sub_445480
478735  mov     ecx, edi
478737  mov     [ebp+var_8], eax
47873A  call    sub_48EA40
47873F  mov     ecx, edi
478741  mov     [ebp+var_4], eax
478744  call    sub_48EA40
478749  mov     ecx, [ebp+var_4]
47874C  mov     edx, [ebp+var_8]
47874F  push    esi
478750  push    ebx
478751  push    ecx
478752  push    eax
478753  call    sub_497BF0
478758  mov     esi, eax
47875A  add     esp, 10h
47875D  mov     eax, edi
47875F  call    sub_4454A0
478764  pop     edi
478765  pop     esi
478766  xor     eax, eax
478768  pop     ebx
478769  mov     esp, ebp
47876B  pop     ebp
47876C  retn

;==== 478770 sub_478770
478770  push    ebp
478771  mov     ebp, esp
478773  push    ecx
478774  push    ebx
478775  push    esi
478776  push    edi
478777  mov     edi, [ebp+arg_0]
47877A  mov     ecx, edi
47877C  call    sub_445480
478781  mov     ecx, edi
478783  mov     esi, eax
478785  call    sub_445480
47878A  mov     ecx, edi
47878C  mov     ebx, eax
47878E  call    sub_445480
478793  mov     ecx, edi
478795  mov     [ebp+var_4], eax
478798  call    sub_48EA40
47879D  mov     edx, [ebp+var_4]
4787A0  push    esi
4787A1  push    ebx
4787A2  push    eax
4787A3  call    sub_497C30
4787A8  mov     esi, eax
4787AA  add     esp, 0Ch
4787AD  mov     eax, edi
4787AF  call    sub_4454A0
4787B4  pop     edi
4787B5  pop     esi
4787B6  xor     eax, eax
4787B8  pop     ebx
4787B9  mov     esp, ebp
4787BB  pop     ebp
4787BC  retn

;==== 4787C0 sub_4787C0
4787C0  push    ebp
4787C1  mov     ebp, esp
4787C3  sub     esp, 0Ch
4787C6  push    ebx
4787C7  push    esi
4787C8  push    edi
4787C9  mov     edi, [ebp+arg_0]
4787CC  mov     ecx, edi
4787CE  call    sub_445480
4787D3  mov     ecx, edi
4787D5  mov     [ebp+var_4], eax
4787D8  call    sub_445480
4787DD  mov     ecx, edi
4787DF  mov     [ebp+var_8], eax
4787E2  call    sub_445480
4787E7  mov     ecx, edi
4787E9  mov     [ebp+var_C], eax
4787EC  call    sub_445480
4787F1  mov     ecx, edi
4787F3  mov     esi, eax
4787F5  call    sub_445480
4787FA  mov     ecx, edi
4787FC  mov     ebx, eax
4787FE  call    sub_48EA40
478803  fild    [ebp+var_4]
478806  fld     ds:dbl_4EC968
47880C  sub     esp, 0Ch
47880F  mov     edx, ebx
478811  fmul    st(1), st
478813  fxch    st(1)
478815  fstp    [ebp+var_4]
478818  fld     [ebp+var_4]
47881B  fstp    [esp+24h+var_1C]; float
47881F  fild    [ebp+var_8]
478822  fmul    st, st(1)
478824  fstp    [ebp+var_8]
478827  fld     [ebp+var_8]
47882A  fstp    [esp+24h+var_20]; float
47882E  fimul   [ebp+var_C]
478831  fstp    [ebp+var_C]
478834  fld     [ebp+var_C]
478837  fstp    [esp+24h+var_24]; float
47883A  push    esi; int
47883B  push    eax; int
47883C  call    sub_497C60
478841  mov     esi, eax
478843  add     esp, 14h
478846  mov     eax, edi
478848  call    sub_4454A0
47884D  pop     edi
47884E  pop     esi
47884F  xor     eax, eax
478851  pop     ebx
478852  mov     esp, ebp
478854  pop     ebp
478855  retn

;==== 478860 sub_478860
478860  push    ebp
478861  mov     ebp, esp
478863  mov     ecx, [ebp+arg_0]
478866  call    sub_48EA40
47886B  push    eax
47886C  call    sub_497DC0
478871  add     esp, 4
478874  xor     eax, eax
478876  pop     ebp
478877  retn

;==== 478880 sub_478880
478880  push    ebp
478881  mov     ebp, esp
478883  push    esi
478884  push    edi
478885  mov     edi, [ebp+arg_0]
478888  mov     ecx, edi
47888A  call    sub_445480
47888F  mov     ecx, eax
478891  call    sub_497EE0
478896  mov     esi, eax
478898  mov     eax, edi
47889A  call    sub_4454A0
47889F  pop     edi
4788A0  xor     eax, eax
4788A2  pop     esi
4788A3  pop     ebp
4788A4  retn

;==== 4788B0 sub_4788B0
4788B0  push    ebp
4788B1  mov     ebp, esp
4788B3  push    esi
4788B4  push    edi
4788B5  mov     edi, [ebp+arg_0]
4788B8  mov     ecx, edi
4788BA  call    sub_445480
4788BF  mov     ecx, edi
4788C1  mov     edx, eax
4788C3  call    sub_445480
4788C8  push    edx
4788C9  mov     ecx, eax
4788CB  call    sub_4980B0
4788D0  mov     esi, eax
4788D2  add     esp, 4
4788D5  mov     eax, edi
4788D7  call    sub_4454A0
4788DC  pop     edi
4788DD  xor     eax, eax
4788DF  pop     esi
4788E0  pop     ebp
4788E1  retn

;==== 4788F0 sub_4788F0
4788F0  push    ebp
4788F1  mov     ebp, esp
4788F3  push    esi
4788F4  push    edi
4788F5  mov     edi, [ebp+arg_0]
4788F8  mov     ecx, edi
4788FA  call    sub_445480
4788FF  mov     ecx, edi
478901  mov     esi, eax
478903  call    sub_48EA40
478908  push    eax
478909  mov     ecx, esi
47890B  call    sub_498050
478910  mov     esi, eax
478912  add     esp, 4
478915  mov     eax, edi
478917  call    sub_4454A0
47891C  pop     edi
47891D  xor     eax, eax
47891F  pop     esi
478920  pop     ebp
478921  retn

;==== 478930 sub_478930
478930  push    ebp
478931  mov     ebp, esp
478933  push    ecx
478934  push    ebx
478935  mov     ebx, [ebp+arg_0]
478938  push    esi
478939  push    edi
47893A  mov     ecx, ebx
47893C  call    sub_445480
478941  mov     ecx, ebx
478943  mov     esi, eax
478945  call    sub_48EA40
47894A  mov     ecx, ebx
47894C  mov     edi, eax
47894E  call    sub_445480
478953  mov     ecx, ebx
478955  mov     edx, eax
478957  call    sub_445480
47895C  mov     ecx, ebx
47895E  mov     [ebp+var_4], eax
478961  call    sub_445480
478966  push    esi; int
478967  push    edi; Src
478968  mov     edi, edx
47896A  mov     edx, [ebp+var_4]
47896D  mov     ecx, eax
47896F  call    sub_498110
478974  mov     esi, eax
478976  add     esp, 8
478979  mov     eax, ebx
47897B  call    sub_4454A0
478980  pop     edi
478981  pop     esi
478982  xor     eax, eax
478984  pop     ebx
478985  mov     esp, ebp
478987  pop     ebp
478988  retn

;==== 478990 sub_478990
478990  push    ebp
478991  mov     ebp, esp
478993  sub     esp, 8
478996  push    ebx
478997  push    esi
478998  push    edi
478999  mov     edi, [ebp+arg_0]
47899C  mov     ecx, edi
47899E  call    sub_445480
4789A3  mov     ecx, edi
4789A5  mov     esi, eax
4789A7  call    sub_445480
4789AC  mov     ecx, edi
4789AE  mov     ebx, eax
4789B0  call    sub_445480
4789B5  mov     ecx, edi
4789B7  mov     [ebp+var_8], eax
4789BA  call    sub_48EA40
4789BF  mov     ecx, edi
4789C1  mov     [ebp+var_4], eax
4789C4  call    sub_48EA40
4789C9  mov     ecx, [ebp+var_4]
4789CC  push    esi
4789CD  push    ebx
4789CE  push    ecx
4789CF  mov     ecx, [ebp+var_8]
4789D2  push    eax
4789D3  call    sub_4983B0
4789D8  mov     esi, eax
4789DA  add     esp, 10h
4789DD  mov     eax, edi
4789DF  call    sub_4454A0
4789E4  pop     edi
4789E5  pop     esi
4789E6  xor     eax, eax
4789E8  pop     ebx
4789E9  mov     esp, ebp
4789EB  pop     ebp
4789EC  retn

;==== 4789F0 sub_4789F0
4789F0  push    ebp
4789F1  mov     ebp, esp
4789F3  push    esi
4789F4  push    edi
4789F5  mov     edi, [ebp+arg_0]
4789F8  mov     ecx, edi
4789FA  call    sub_445480
4789FF  mov     ecx, edi
478A01  mov     edx, eax
478A03  call    sub_445480
478A08  mov     ecx, eax
478A0A  call    sub_498210
478A0F  mov     esi, eax
478A11  mov     eax, edi
478A13  call    sub_4454A0
478A18  pop     edi
478A19  xor     eax, eax
478A1B  pop     esi
478A1C  pop     ebp
478A1D  retn

;==== 478A20 sub_478A20
478A20  push    ebp
478A21  mov     ebp, esp
478A23  push    ecx
478A24  push    ebx
478A25  push    esi
478A26  push    edi
478A27  mov     edi, [ebp+arg_0]
478A2A  mov     ecx, edi
478A2C  call    sub_445480
478A31  mov     ecx, edi
478A33  mov     esi, eax
478A35  call    sub_445480
478A3A  mov     ecx, edi
478A3C  mov     ebx, eax
478A3E  call    sub_48EA40
478A43  mov     ecx, edi
478A45  mov     edx, eax
478A47  call    sub_445480
478A4C  mov     ecx, edi
478A4E  mov     [ebp+var_4], eax
478A51  call    sub_445480
478A56  mov     ecx, eax
478A58  mov     eax, [ebp+var_4]
478A5B  push    esi
478A5C  push    ebx
478A5D  push    eax
478A5E  mov     eax, edx
478A60  call    sub_498280
478A65  mov     esi, eax
478A67  add     esp, 0Ch
478A6A  mov     eax, edi
478A6C  call    sub_4454A0
478A71  pop     edi
478A72  pop     esi
478A73  xor     eax, eax
478A75  pop     ebx
478A76  mov     esp, ebp
478A78  pop     ebp
478A79  retn

;==== 478A80 sub_478A80
478A80  push    ebp
478A81  mov     ebp, esp
478A83  push    ebx
478A84  push    esi
478A85  push    edi
478A86  mov     edi, [ebp+arg_0]
478A89  mov     ecx, edi
478A8B  call    sub_445480
478A90  mov     ecx, edi
478A92  mov     esi, eax
478A94  call    sub_445480
478A99  mov     ecx, edi
478A9B  mov     ebx, eax
478A9D  call    sub_48EA40
478AA2  push    esi
478AA3  push    eax
478AA4  mov     ecx, ebx
478AA6  call    sub_498350
478AAB  mov     esi, eax
478AAD  add     esp, 8
478AB0  mov     eax, edi
478AB2  call    sub_4454A0
478AB7  pop     edi
478AB8  pop     esi
478AB9  xor     eax, eax
478ABB  pop     ebx
478ABC  pop     ebp
478ABD  retn

;==== 493C80 sub_493C80
493C80  mov     eax, [eax+1Ch]
493C83  retn

;==== 497120 sub_497120
497120  cmp     eax, 8000000Ah
497125  ja      loc_4971BE
49712B  jz      loc_4971B8
497131  cmp     eax, 80000004h
497136  ja      short loc_497185
497138  jz      short loc_49717F
49713A  cmp     eax, 80000001h
49713F  ja      short loc_497161
497141  jz      short loc_49715B
497143  test    eax, eax
497145  jz      short loc_497158
497147  cmp     eax, 80000000h
49714C  jnz     def_497193; jumptable 00497193 default case
497152  mov     eax, 1
497157  retn
497158  xor     eax, eax
49715A  retn
49715B  mov     eax, 2
497160  retn
497161  cmp     eax, 80000002h
497166  jz      short loc_497179
497168  cmp     eax, 80000003h
49716D  jnz     def_497193; jumptable 00497193 default case
497173  mov     eax, 4
497178  retn
497179  mov     eax, 3
49717E  retn
49717F  mov     eax, 5
497184  retn
497185  add     eax, 7FFFFFFBh; switch 5 cases
49718A  cmp     eax, 4
49718D  ja      def_497193; jumptable 00497193 default case
497193  jmp     ds:jpt_497193[eax*4]; switch jump
49719A  mov     eax, 6; jumptable 00497193 case -2147483643
49719F  retn
4971A0  mov     eax, 7; jumptable 00497193 case -2147483642
4971A5  retn
4971A6  mov     eax, 8; jumptable 00497193 case -2147483641
4971AB  retn
4971AC  mov     eax, 9; jumptable 00497193 case -2147483640
4971B1  retn
4971B2  mov     eax, 0Ah; jumptable 00497193 case -2147483639
4971B7  retn
4971B8  mov     eax, 0Bh
4971BD  retn
4971BE  cmp     eax, 0A0000000h
4971C3  ja      short loc_4971EE
4971C5  jz      short loc_497152
4971C7  cmp     eax, 8000000Bh
4971CC  jz      short loc_4971E8
4971CE  cmp     eax, 90000002h
4971D3  jz      short loc_4971E2
4971D5  cmp     eax, 90000003h
4971DA  jnz     short def_497193; jumptable 00497193 default case
4971DC  mov     eax, 11h
4971E1  retn
4971E2  mov     eax, 10h
4971E7  retn
4971E8  mov     eax, 0Ch
4971ED  retn
4971EE  cmp     eax, 0FFFFFFFEh
4971F1  ja      short def_497193; jumptable 00497193 default case
4971F3  jz      short loc_497224
4971F5  add     eax, 5FFFFFFFh; switch 6 cases
4971FA  cmp     eax, 5
4971FD  ja      short def_497193; jumptable 00497193 default case
4971FF  jmp     ds:jpt_4971FF[eax*4]; switch jump
497206  mov     eax, 12h; jumptable 004971FF case -1610612735
49720B  retn
49720C  mov     eax, 13h; jumptable 004971FF case -1610612734
497211  retn
497212  mov     eax, 14h; jumptable 004971FF case -1610612733
497217  retn
497218  mov     eax, 15h; jumptable 004971FF case -1610612731
49721D  retn
49721E  mov     eax, 16h; jumptable 004971FF case -1610612730
497223  retn
497224  mov     eax, 0FFFFFFFEh
497229  retn
49722A  or      eax, 0FFFFFFFFh; jumptable 00497193 default case
49722D  retn

;==== 497260 sub_497260
497260  push    ebp
497261  mov     ebp, esp
497263  push    0FFFFFFFFh
497265  push    offset SEH_497260
49726A  mov     eax, large fs:0
497270  push    eax
497271  push    esi
497272  mov     eax, dword_4FB734
497277  xor     eax, ebp
497279  push    eax
49727A  lea     eax, [ebp+var_C]
49727D  mov     large fs:0, eax
497283  xor     esi, esi
497285  cmp     [ebp+arg_4], esi
497288  jnz     short loc_4972B8
49728A  push    498h; Size
49728F  call    ??2@YAPAXI@Z; operator new(uint)
497294  add     esp, 4
497297  mov     [ebp+arg_4], eax
49729A  mov     [ebp+var_4], esi
49729D  test    eax, eax
49729F  jz      short loc_4972AD
4972A1  mov     ecx, [ebp+arg_8]
4972A4  push    ecx
4972A5  push    eax
4972A6  call    sub_454940
4972AB  jmp     short loc_4972AF
4972AD  xor     eax, eax
4972AF  mov     [ebp+var_4], 0FFFFFFFFh
4972B6  mov     esi, eax
4972B8  xor     eax, eax
4972BA  test    esi, esi
4972BC  jz      short loc_4972F3
4972BE  push    0Ch; Size
4972C0  call    ??2@YAPAXI@Z; operator new(uint)
4972C5  mov     edx, dword_5669E4
4972CB  mov     [eax], edx
4972CD  add     esp, 4
4972D0  inc     dword_5669E4
4972D6  mov     [eax+4], esi
4972D9  mov     ecx, dword_5669EC
4972DF  mov     [eax+8], ecx
4972E2  mov     dword_5669EC, eax
4972E7  mov     edx, [eax]
4972E9  mov     eax, [ebp+arg_0]
4972EC  mov     [eax], edx
4972EE  mov     eax, 1
4972F3  mov     ecx, [ebp+var_C]
4972F6  mov     large fs:0, ecx
4972FD  pop     ecx
4972FE  pop     esi
4972FF  mov     esp, ebp
497301  pop     ebp
497302  retn
4DA100  mov     eax, [ebp+arg_4]
4DA103  push    eax; void *
4DA104  call    ??3@YAXPAX@Z; operator delete(void *)
4DA109  pop     ecx
4DA10A  retn
4DA10B  mov     edx, [esp-4+arg_4]
4DA10F  lea     eax, [edx+0Ch]
4DA112  mov     ecx, [edx-8]
4DA115  xor     ecx, eax
4DA117  call    sub_4ABDD5
4DA11C  mov     eax, offset stru_4F79D8
4DA121  jmp     ___CxxFrameHandler3

;==== 497310 sub_497310
497310  mov     ecx, dword_5669EC
497316  xor     eax, eax
497318  test    ecx, ecx
49731A  jz      short locret_49732F
49731C  lea     esp, [esp+0]
497320  cmp     edx, [ecx]
497322  jz      short loc_49732C
497324  mov     ecx, [ecx+8]
497327  test    ecx, ecx
497329  jnz     short loc_497320
49732B  retn
49732C  mov     eax, [ecx+4]
49732F  retn

;==== 497330 sub_497330
497330  push    esi
497331  mov     esi, dword_5669EC
497337  xor     eax, eax
497339  mov     ecx, offset dword_5669E4
49733E  test    esi, esi
497340  jz      short loc_497374
497342  cmp     edx, [esi]
497344  jz      short loc_497351
497346  mov     ecx, esi
497348  mov     esi, [esi+8]
49734B  test    esi, esi
49734D  jnz     short loc_497342
49734F  pop     esi
497350  retn
497351  mov     eax, [esi+8]
497354  mov     [ecx+8], eax
497357  mov     ecx, [esi+4]
49735A  test    ecx, ecx
49735C  jz      short loc_497366
49735E  mov     edx, [ecx]
497360  mov     eax, [edx]
497362  push    1
497364  call    eax
497366  push    esi; void *
497367  call    ??3@YAXPAX@Z; operator delete(void *)
49736C  add     esp, 4
49736F  mov     eax, 1
497374  pop     esi
497375  retn

;==== 4973B0 sub_4973B0
4973B0  push    ebp
4973B1  mov     ebp, esp
4973B3  call    sub_497310
4973B8  mov     ecx, eax
4973BA  test    ecx, ecx
4973BC  jz      short loc_4973D4
4973BE  mov     eax, [ebp+Src]
4973C1  mov     edx, [ebp+arg_4]
4973C4  push    eax; Src
4973C5  mov     eax, [ebp+arg_0]
4973C8  push    edx; int
4973C9  call    sub_453480
4973CE  pop     ebp
4973CF  jmp     sub_497120
4973D4  mov     eax, 80000000h
4973D9  pop     ebp
4973DA  jmp     sub_497120

;==== 4973E0 sub_4973E0
4973E0  push    ebp
4973E1  mov     ebp, esp
4973E3  call    sub_497310
4973E8  test    eax, eax
4973EA  jz      short loc_497403
4973EC  mov     ecx, [ebp+Src]
4973EF  mov     edx, [ebp+arg_0]
4973F2  push    esi
4973F3  push    ecx; Src
4973F4  push    edx; int
4973F5  mov     esi, eax
4973F7  call    sub_453530
4973FC  pop     esi
4973FD  pop     ebp
4973FE  jmp     sub_497120
497403  mov     eax, 80000000h
497408  pop     ebp
497409  jmp     sub_497120

;==== 497410 sub_497410
497410  push    ebp
497411  mov     ebp, esp
497413  call    sub_497310
497418  test    eax, eax
49741A  jz      short loc_49742F
49741C  mov     ecx, [ebp+arg_0]
49741F  push    edi
497420  push    ecx
497421  mov     edi, eax
497423  call    sub_4535B0
497428  pop     edi
497429  pop     ebp
49742A  jmp     sub_497120
49742F  mov     eax, 80000000h
497434  pop     ebp
497435  jmp     sub_497120

;==== 497440 sub_497440
497440  push    ebp
497441  mov     ebp, esp
497443  call    sub_497310
497448  test    eax, eax
49744A  jz      short loc_49745C
49744C  mov     ecx, [ebp+arg_0]
49744F  mov     edx, eax
497451  call    sub_453630
497456  pop     ebp
497457  jmp     sub_497120
49745C  mov     eax, 80000000h
497461  pop     ebp
497462  jmp     sub_497120

;==== 497470 sub_497470
497470  push    ebp
497471  mov     ebp, esp
497473  call    sub_497310
497478  test    eax, eax
49747A  jz      short loc_49748A
49747C  mov     ecx, [ebp+arg_0]
49747F  call    sub_453680
497484  pop     ebp
497485  jmp     sub_497120
49748A  mov     eax, 80000000h
49748F  pop     ebp
497490  jmp     sub_497120

;==== 4974A0 sub_4974A0
4974A0  push    ebp
4974A1  mov     ebp, esp
4974A3  call    sub_497310
4974A8  test    eax, eax
4974AA  jz      short loc_4974BC
4974AC  mov     ecx, [ebp+arg_0]
4974AF  mov     edx, eax
4974B1  call    sub_4536B0
4974B6  pop     ebp
4974B7  jmp     sub_497120
4974BC  mov     eax, 80000000h
4974C1  pop     ebp
4974C2  jmp     sub_497120

;==== 4974D0 sub_4974D0
4974D0  push    ebp
4974D1  mov     ebp, esp
4974D3  call    sub_497310
4974D8  test    eax, eax
4974DA  jz      short loc_4974EC
4974DC  mov     ecx, [ebp+arg_0]
4974DF  mov     edx, eax
4974E1  call    sub_453720
4974E6  pop     ebp
4974E7  jmp     sub_497120
4974EC  mov     eax, 80000000h
4974F1  pop     ebp
4974F2  jmp     sub_497120

;==== 497500 sub_497500
497500  push    ebp
497501  mov     ebp, esp
497503  call    sub_497310
497508  test    eax, eax
49750A  jz      short loc_49751E
49750C  mov     edx, [ebp+arg_4]
49750F  mov     ecx, [ebp+arg_0]
497512  push    eax
497513  call    sub_453790
497518  pop     ebp
497519  jmp     sub_497120
49751E  mov     eax, 80000000h
497523  pop     ebp
497524  jmp     sub_497120

;==== 497530 sub_497530
497530  push    ebp
497531  mov     ebp, esp
497533  call    sub_497310
497538  test    eax, eax
49753A  jz      short loc_49754E
49753C  mov     edx, [ebp+arg_4]
49753F  mov     ecx, [ebp+arg_0]
497542  push    eax
497543  call    sub_4537D0
497548  pop     ebp
497549  jmp     sub_497120
49754E  mov     eax, 80000000h
497553  pop     ebp
497554  jmp     sub_497120

;==== 497560 sub_497560
497560  push    ebp
497561  mov     ebp, esp
497563  call    sub_497310
497568  test    eax, eax
49756A  jz      short loc_497581
49756C  mov     edx, [ebp+arg_4]
49756F  mov     ecx, [ebp+arg_0]
497572  push    edi
497573  mov     edi, eax
497575  call    sub_453800
49757A  pop     edi
49757B  pop     ebp
49757C  jmp     sub_497120
497581  mov     eax, 80000000h
497586  pop     ebp
497587  jmp     sub_497120

;==== 497590 sub_497590
497590  push    ebp
497591  mov     ebp, esp
497593  call    sub_497310
497598  test    eax, eax
49759A  jz      short loc_4975BC
49759C  mov     ecx, [ebp+arg_10]
49759F  mov     edx, [ebp+arg_C]
4975A2  push    1
4975A4  push    ecx
4975A5  mov     ecx, [ebp+arg_8]
4975A8  push    edx
4975A9  mov     edx, [ebp+arg_4]
4975AC  push    ecx
4975AD  mov     ecx, [ebp+arg_0]
4975B0  push    edx
4975B1  call    sub_453830
4975B6  pop     ebp
4975B7  jmp     sub_497120
4975BC  mov     eax, 80000000h
4975C1  pop     ebp
4975C2  jmp     sub_497120

;==== 4975D0 sub_4975D0
4975D0  push    ebp
4975D1  mov     ebp, esp
4975D3  call    sub_497310
4975D8  test    eax, eax
4975DA  jz      short loc_4975FA
4975DC  mov     ecx, [ebp+arg_10]
4975DF  mov     edx, [ebp+arg_C]
4975E2  push    ecx
4975E3  mov     ecx, [ebp+arg_4]
4975E6  push    edx
4975E7  mov     edx, [ebp+arg_0]
4975EA  push    ecx
4975EB  mov     ecx, [ebp+arg_8]
4975EE  push    edx
4975EF  call    sub_453930
4975F4  pop     ebp
4975F5  jmp     sub_497120
4975FA  mov     eax, 80000000h
4975FF  pop     ebp
497600  jmp     sub_497120

;==== 497610 sub_497610
497610  push    ebp
497611  mov     ebp, esp
497613  call    sub_497310
497618  test    eax, eax
49761A  jz      short loc_49762E
49761C  mov     ecx, [ebp+arg_0]
49761F  push    ecx; void *
497620  mov     ecx, [ebp+arg_4]
497623  call    sub_453B30
497628  pop     ebp
497629  jmp     sub_497120
49762E  mov     eax, 80000000h
497633  pop     ebp
497634  jmp     sub_497120

;==== 497640 sub_497640
497640  push    ebp
497641  mov     ebp, esp
497643  call    sub_497310
497648  test    eax, eax
49764A  jz      short loc_497664
49764C  mov     ecx, [ebp+arg_4]
49764F  mov     edx, [ebp+arg_0]
497652  push    0
497654  push    ecx
497655  mov     ecx, [ebp+arg_8]
497658  push    edx
497659  call    sub_453B80
49765E  pop     ebp
49765F  jmp     sub_497120
497664  mov     eax, 80000000h
497669  pop     ebp
49766A  jmp     sub_497120

;==== 497670 sub_497670
497670  push    ebp
497671  mov     ebp, esp
497673  call    sub_497310
497678  test    eax, eax
49767A  jz      short loc_4976B5
49767C  mov     ecx, [ebp+arg_24]
49767F  push    1
497681  push    ecx
497682  mov     ecx, [ebp+arg_20]
497685  push    ecx
497686  mov     ecx, [ebp+arg_1C]
497689  push    ecx
49768A  mov     ecx, [ebp+arg_18]
49768D  mov     edx, [eax]
49768F  push    ecx
497690  mov     ecx, [ebp+arg_14]
497693  mov     edx, [edx+4]
497696  push    ecx
497697  mov     ecx, [ebp+arg_10]
49769A  push    ecx
49769B  mov     ecx, [ebp+arg_C]
49769E  push    ecx
49769F  mov     ecx, [ebp+arg_8]
4976A2  push    ecx
4976A3  mov     ecx, [ebp+arg_4]
4976A6  push    ecx
4976A7  mov     ecx, [ebp+arg_0]
4976AA  push    ecx
4976AB  mov     ecx, eax
4976AD  call    edx
4976AF  pop     ebp
4976B0  jmp     sub_497120
4976B5  mov     eax, 80000000h
4976BA  pop     ebp
4976BB  jmp     sub_497120

;==== 4976C0 sub_4976C0
4976C0  push    ebp
4976C1  mov     ebp, esp
4976C3  call    sub_497310
4976C8  test    eax, eax
4976CA  jz      short loc_4976E8
4976CC  mov     ecx, [ebp+arg_8]
4976CF  mov     edx, [ebp+arg_4]
4976D2  push    ecx
4976D3  mov     ecx, [ebp+arg_0]
4976D6  push    edx
4976D7  push    ecx
4976D8  mov     ecx, [ebp+arg_C]
4976DB  mov     edx, eax
4976DD  call    sub_453BE0
4976E2  pop     ebp
4976E3  jmp     sub_497120
4976E8  mov     eax, 80000000h
4976ED  pop     ebp
4976EE  jmp     sub_497120

;==== 497700 sub_497700
497700  push    ebp
497701  mov     ebp, esp
497703  call    sub_497310
497708  test    eax, eax
49770A  jz      short loc_497732
49770C  mov     ecx, [ebp+arg_10]
49770F  mov     edx, [eax]
497711  mov     edx, [edx+0Ch]
497714  push    ecx
497715  mov     ecx, [ebp+arg_C]
497718  push    ecx
497719  mov     ecx, [ebp+arg_8]
49771C  push    ecx
49771D  mov     ecx, [ebp+arg_4]
497720  push    ecx
497721  mov     ecx, eax
497723  call    edx
497725  mov     ecx, [ebp+arg_0]
497728  mov     [ecx], eax
49772A  xor     eax, eax
49772C  pop     ebp
49772D  jmp     sub_497120
497732  mov     eax, 80000000h
497737  pop     ebp
497738  jmp     sub_497120

;==== 497740 sub_497740
497740  push    ebp
497741  mov     ebp, esp
497743  push    0FFFFFFFFh
497745  push    offset SEH_466DE0
49774A  mov     eax, large fs:0
497750  push    eax
497751  push    ecx
497752  push    esi
497753  push    edi
497754  mov     eax, dword_4FB734
497759  xor     eax, ebp
49775B  push    eax
49775C  lea     eax, [ebp+var_C]
49775F  mov     large fs:0, eax
497765  push    0Ch; Size
497767  call    ??2@YAPAXI@Z; operator new(uint)
49776C  mov     edi, eax
49776E  mov     eax, dword_5669F0
497773  inc     eax
497774  mov     dword_5669F0, eax
497779  push    170h; Size
49777E  mov     [edi], eax
497780  call    ??2@YAPAXI@Z; operator new(uint)
497785  add     esp, 8
497788  mov     [ebp+var_10], eax
49778B  mov     [ebp+var_4], 0
497792  test    eax, eax
497794  jz      short loc_49779F
497796  mov     esi, eax
497798  call    sub_4559B0
49779D  jmp     short loc_4977A1
49779F  xor     eax, eax
4977A1  mov     [ebp+var_4], 0FFFFFFFFh
4977A8  mov     edx, [ebp+arg_0]
4977AB  mov     [edi+4], eax
4977AE  mov     eax, dword_5669F8
4977B3  mov     [edi+8], eax
4977B6  mov     dword_5669F8, edi
4977BC  mov     ecx, [edi]
4977BE  mov     [edx], ecx
4977C0  xor     eax, eax
4977C2  mov     ecx, [ebp+var_C]
4977C5  mov     large fs:0, ecx
4977CC  pop     ecx
4977CD  pop     edi
4977CE  pop     esi
4977CF  mov     esp, ebp
4977D1  pop     ebp
4977D2  retn
4D99C0  mov     eax, [ebp+var_10]
4D99C3  push    eax; void *
4D99C4  call    ??3@YAXPAX@Z; operator delete(void *)
4D99C9  pop     ecx
4D99CA  retn
4D99CB  mov     edx, [esp-4+arg_4]
4D99CF  lea     eax, [edx+0Ch]
4D99D2  mov     ecx, [edx-10h]
4D99D5  xor     ecx, eax
4D99D7  call    sub_4ABDD5
4D99DC  mov     eax, offset stru_4F72B0
4D99E1  jmp     ___CxxFrameHandler3

;==== 4977E0 sub_4977E0
4977E0  push    esi
4977E1  mov     esi, dword_5669F8
4977E7  mov     eax, 1
4977EC  mov     ecx, offset dword_5669F0
4977F1  test    esi, esi
4977F3  jz      short loc_497824
4977F5  cmp     edx, [esi]
4977F7  jz      short loc_497804
4977F9  mov     ecx, esi
4977FB  mov     esi, [esi+8]
4977FE  test    esi, esi
497800  jnz     short loc_4977F5
497802  pop     esi
497803  retn
497804  mov     eax, [esi+8]
497807  mov     [ecx+8], eax
49780A  mov     ecx, [esi+4]
49780D  test    ecx, ecx
49780F  jz      short loc_497819
497811  mov     edx, [ecx]
497813  mov     eax, [edx]
497815  push    1
497817  call    eax
497819  push    esi; void *
49781A  call    ??3@YAXPAX@Z; operator delete(void *)
49781F  add     esp, 4
497822  xor     eax, eax
497824  pop     esi
497825  retn

;==== 497860 sub_497860
497860  mov     ecx, dword_5669F8
497866  xor     eax, eax
497868  test    ecx, ecx
49786A  jz      short locret_49787F
49786C  lea     esp, [esp+0]
497870  cmp     edx, [ecx]
497872  jz      short loc_49787C
497874  mov     ecx, [ecx+8]
497877  test    ecx, ecx
497879  jnz     short loc_497870
49787B  retn
49787C  mov     eax, [ecx+4]
49787F  retn

;==== 497880 sub_497880
497880  push    ebp
497881  mov     ebp, esp
497883  push    esi
497884  mov     esi, 0A0000000h
497889  call    sub_497860
49788E  test    eax, eax
497890  jz      short loc_4978EA
497892  mov     ecx, [ebp+arg_2C]
497895  fld     [ebp+arg_24]
497898  mov     edx, [ebp+arg_28]
49789B  push    edi
49789C  push    ecx; int
49789D  mov     ecx, [ebp+arg_0]
4978A0  push    edx; int
4978A1  sub     esp, 24h
4978A4  fstp    [esp+34h+var_14]; float
4978A8  mov     edi, eax
4978AA  fld     [ebp+arg_20]
4978AD  fstp    [esp+34h+var_18]; float
4978B1  fld     [ebp+arg_1C]
4978B4  fstp    [esp+34h+var_1C]; float
4978B8  fld     [ebp+arg_18]
4978BB  fstp    [esp+34h+var_20]; float
4978BF  fld     [ebp+arg_14]
4978C2  fstp    [esp+34h+var_24]; float
4978C6  fld     [ebp+arg_10]
4978C9  fstp    [esp+34h+var_28]; float
4978CD  fld     [ebp+arg_C]
4978D0  fstp    [esp+34h+var_2C]; float
4978D4  fld     [ebp+arg_8]
4978D7  fstp    [esp+34h+var_30]; float
4978DB  fld     [ebp+arg_4]
4978DE  fstp    [esp+34h+var_34]; float
4978E1  push    ecx; int
4978E2  call    sub_455A40
4978E7  mov     esi, eax
4978E9  pop     edi
4978EA  mov     eax, esi
4978EC  pop     esi
4978ED  pop     ebp
4978EE  jmp     sub_497120

;==== 497900 sub_497900
497900  push    ebp
497901  mov     ebp, esp
497903  push    esi
497904  mov     esi, 0A0000000h
497909  call    sub_497860
49790E  test    eax, eax
497910  jz      short loc_49791E
497912  mov     ecx, [ebp+arg_0]
497915  push    ecx
497916  push    eax
497917  call    sub_455C30
49791C  mov     esi, eax
49791E  mov     eax, esi
497920  pop     esi
497921  pop     ebp
497922  jmp     sub_497120

;==== 497930 sub_497930
497930  push    ebp
497931  mov     ebp, esp
497933  push    esi
497934  mov     esi, 0A0000000h
497939  call    sub_497860
49793E  test    eax, eax
497940  jz      short loc_497965
497942  fld     [ebp+arg_C]
497945  mov     ecx, [ebp+arg_0]
497948  sub     esp, 0Ch
49794B  fstp    [esp+10h+var_8]; float
49794F  mov     edx, eax
497951  fld     [ebp+arg_8]
497954  fstp    [esp+10h+var_C]; float
497958  fld     [ebp+arg_4]
49795B  fstp    [esp+10h+var_10]; float
49795E  call    sub_455CD0
497963  mov     esi, eax
497965  mov     eax, esi
497967  pop     esi
497968  pop     ebp
497969  jmp     sub_497120

;==== 497970 sub_497970
497970  push    ebp
497971  mov     ebp, esp
497973  push    esi
497974  mov     esi, 0A0000000h
497979  call    sub_497860
49797E  test    eax, eax
497980  jz      short loc_497992
497982  mov     ecx, [ebp+arg_0]
497985  push    ecx
497986  mov     ecx, [ebp+arg_4]
497989  mov     edx, eax
49798B  call    sub_455DC0
497990  mov     esi, eax
497992  mov     eax, esi
497994  pop     esi
497995  pop     ebp
497996  jmp     sub_497120

;==== 4979A0 sub_4979A0
4979A0  push    ebp
4979A1  mov     ebp, esp
4979A3  push    esi
4979A4  mov     esi, 0A0000000h
4979A9  call    sub_497860
4979AE  test    eax, eax
4979B0  jz      short loc_4979D5
4979B2  fld     [ebp+arg_C]
4979B5  mov     ecx, [ebp+arg_0]
4979B8  sub     esp, 0Ch
4979BB  fstp    [esp+10h+var_8]; float
4979BF  mov     edx, eax
4979C1  fld     [ebp+arg_8]
4979C4  fstp    [esp+10h+var_C]; float
4979C8  fld     [ebp+arg_4]
4979CB  fstp    [esp+10h+var_10]; float
4979CE  call    sub_455DF0
4979D3  mov     esi, eax
4979D5  mov     eax, esi
4979D7  pop     esi
4979D8  pop     ebp
4979D9  jmp     sub_497120

;==== 4979E0 sub_4979E0
4979E0  push    ebp
4979E1  mov     ebp, esp
4979E3  push    esi
4979E4  mov     esi, 0A0000000h
4979E9  call    sub_497860
4979EE  test    eax, eax
4979F0  jz      short loc_497A02
4979F2  mov     ecx, [ebp+arg_0]
4979F5  push    ecx
4979F6  mov     ecx, [ebp+arg_4]
4979F9  mov     edx, eax
4979FB  call    sub_455F60
497A00  mov     esi, eax
497A02  mov     eax, esi
497A04  pop     esi
497A05  pop     ebp
497A06  jmp     sub_497120

;==== 497A10 sub_497A10
497A10  push    ebp
497A11  mov     ebp, esp
497A13  push    esi
497A14  mov     esi, 0A0000000h
497A19  call    sub_497860
497A1E  test    eax, eax
497A20  jz      short loc_497A37
497A22  mov     ecx, [ebp+arg_8]
497A25  mov     edx, [ebp+arg_4]
497A28  push    ecx
497A29  mov     ecx, [ebp+arg_0]
497A2C  push    edx
497A2D  push    ecx
497A2E  mov     ecx, eax
497A30  call    sub_455F90
497A35  mov     esi, eax
497A37  mov     eax, esi
497A39  pop     esi
497A3A  pop     ebp
497A3B  jmp     sub_497120

;==== 497A40 sub_497A40
497A40  push    ebp
497A41  mov     ebp, esp
497A43  push    esi
497A44  mov     esi, 0A0000000h
497A49  call    sub_497860
497A4E  test    eax, eax
497A50  jz      short loc_497A65
497A52  mov     ecx, [ebp+arg_0]
497A55  mov     esi, [ebp+arg_8]
497A58  push    ecx
497A59  mov     ecx, [ebp+arg_4]
497A5C  mov     edx, eax
497A5E  call    sub_456160
497A63  mov     esi, eax
497A65  mov     eax, esi
497A67  pop     esi
497A68  pop     ebp
497A69  jmp     sub_497120

;==== 497A70 sub_497A70
497A70  push    ebp
497A71  mov     ebp, esp
497A73  push    esi
497A74  mov     esi, 0A0000000h
497A79  call    sub_497860
497A7E  test    eax, eax
497A80  jz      short loc_497A96
497A82  mov     ecx, [ebp+arg_4]
497A85  mov     edx, [ebp+arg_0]
497A88  push    ecx
497A89  mov     ecx, [ebp+arg_8]
497A8C  push    edx
497A8D  mov     edx, eax
497A8F  call    sub_4561A0
497A94  mov     esi, eax
497A96  mov     eax, esi
497A98  pop     esi
497A99  pop     ebp
497A9A  jmp     sub_497120

;==== 497AA0 sub_497AA0
497AA0  push    ebp
497AA1  mov     ebp, esp
497AA3  push    esi
497AA4  mov     esi, 0A0000000h
497AA9  call    sub_497860
497AAE  test    eax, eax
497AB0  jz      short loc_497ADD
497AB2  mov     ecx, [ebp+arg_14]
497AB5  fld     [ebp+arg_C]
497AB8  mov     edx, [ebp+arg_10]
497ABB  push    ecx
497ABC  push    edx
497ABD  mov     edx, [ebp+arg_0]
497AC0  sub     esp, 0Ch
497AC3  fstp    [esp+18h+var_10]
497AC7  mov     esi, eax
497AC9  fld     [ebp+arg_8]
497ACC  fstp    [esp+18h+var_14]
497AD0  fld     [ebp+arg_4]
497AD3  fstp    [esp+18h+var_18]
497AD6  call    sub_456210
497ADB  mov     esi, eax
497ADD  mov     eax, esi
497ADF  pop     esi
497AE0  pop     ebp
497AE1  jmp     sub_497120

;==== 497AF0 sub_497AF0
497AF0  push    ebp
497AF1  mov     ebp, esp
497AF3  push    esi
497AF4  mov     esi, 0A0000000h
497AF9  call    sub_497860
497AFE  test    eax, eax
497B00  jz      short loc_497B39
497B02  mov     ecx, [ebp+arg_24]
497B05  fld     [ebp+arg_14]
497B08  mov     edx, [ebp+arg_20]
497B0B  push    ecx; int
497B0C  mov     ecx, [ebp+arg_1C]
497B0F  push    edx; int
497B10  mov     edx, [ebp+arg_18]
497B13  push    ecx; int
497B14  mov     ecx, [ebp+arg_C]
497B17  push    edx; int
497B18  mov     edx, [ebp+arg_8]
497B1B  sub     esp, 8
497B1E  fstp    [esp+1Ch+var_18]; float
497B22  fld     [ebp+arg_10]
497B25  fstp    [esp+1Ch+var_1C]; int
497B28  push    ecx; int
497B29  mov     ecx, [ebp+arg_4]
497B2C  push    edx; int
497B2D  mov     edx, [ebp+arg_0]
497B30  push    ecx; int
497B31  push    edx; int
497B32  call    sub_456280
497B37  mov     esi, eax
497B39  mov     eax, esi
497B3B  pop     esi
497B3C  pop     ebp
497B3D  jmp     sub_497120

;==== 497B50 sub_497B50
497B50  push    ebp
497B51  mov     ebp, esp
497B53  push    esi
497B54  mov     esi, 0A0000000h
497B59  call    sub_497860
497B5E  test    eax, eax
497B60  jz      short loc_497B78
497B62  mov     ecx, [ebp+arg_C]
497B65  mov     edx, [ebp+arg_4]
497B68  push    ecx
497B69  mov     ecx, [ebp+arg_0]
497B6C  push    edx
497B6D  mov     edx, [ebp+arg_8]
497B70  push    ecx
497B71  call    sub_456960
497B76  mov     esi, eax
497B78  mov     eax, esi
497B7A  pop     esi
497B7B  pop     ebp
497B7C  jmp     sub_497120

;==== 497B90 sub_497B90
497B90  push    ebp
497B91  mov     ebp, esp
497B93  push    esi
497B94  push    edi
497B95  mov     esi, 0A0000000h
497B9A  call    sub_497860
497B9F  mov     edi, eax
497BA1  test    edi, edi
497BA3  jz      short loc_497BD8
497BA5  mov     eax, [ebp+arg_18]
497BA8  fld     [ebp+arg_14]
497BAB  mov     ecx, [ebp+arg_0]
497BAE  push    eax; int
497BAF  mov     eax, [ebp+arg_1C]
497BB2  sub     esp, 10h
497BB5  fstp    [esp+1Ch+var_10]; float
497BB9  fld     [ebp+arg_10]
497BBC  fstp    [esp+1Ch+var_14]; int
497BC0  fld     [ebp+arg_C]
497BC3  fstp    [esp+1Ch+var_18]; int
497BC7  fld     [ebp+arg_8]
497BCA  fstp    [esp+1Ch+var_1C]; int
497BCD  push    ecx; int
497BCE  mov     ecx, [ebp+arg_4]
497BD1  call    sub_4569C0
497BD6  mov     esi, eax
497BD8  pop     edi
497BD9  mov     eax, esi
497BDB  pop     esi
497BDC  pop     ebp
497BDD  jmp     sub_497120

;==== 497BF0 sub_497BF0
497BF0  push    ebp
497BF1  mov     ebp, esp
497BF3  push    esi
497BF4  mov     esi, 0A0000000h
497BF9  call    sub_497860
497BFE  test    eax, eax
497C00  jz      short loc_497C1A
497C02  mov     ecx, [ebp+arg_C]
497C05  mov     edx, [ebp+arg_8]
497C08  push    ecx
497C09  mov     ecx, [ebp+arg_4]
497C0C  push    edx
497C0D  mov     edx, [ebp+arg_0]
497C10  push    ecx
497C11  push    edx
497C12  push    eax
497C13  call    sub_456A10
497C18  mov     esi, eax
497C1A  mov     eax, esi
497C1C  pop     esi
497C1D  pop     ebp
497C1E  jmp     sub_497120

;==== 497C30 sub_497C30
497C30  push    ebp
497C31  mov     ebp, esp
497C33  push    esi
497C34  mov     esi, 0A0000000h
497C39  call    sub_497860
497C3E  test    eax, eax
497C40  jz      short loc_497C54
497C42  mov     ecx, [ebp+arg_0]
497C45  mov     edx, [ebp+arg_8]
497C48  push    ecx
497C49  mov     ecx, [ebp+arg_4]
497C4C  push    eax
497C4D  call    sub_456C00
497C52  mov     esi, eax
497C54  mov     eax, esi
497C56  pop     esi
497C57  pop     ebp
497C58  jmp     sub_497120

;==== 497C60 sub_497C60
497C60  push    ebp
497C61  mov     ebp, esp
497C63  push    esi
497C64  mov     esi, 0A0000000h
497C69  call    sub_497860
497C6E  test    eax, eax
497C70  jz      short loc_497C99
497C72  fld     [ebp+arg_10]
497C75  mov     ecx, [ebp+arg_0]
497C78  sub     esp, 0Ch
497C7B  fstp    [esp+10h+var_8]
497C7F  mov     edx, eax
497C81  fld     [ebp+arg_C]
497C84  fstp    [esp+10h+var_C]
497C88  fld     [ebp+arg_8]
497C8B  fstp    [esp+10h+var_10]
497C8E  push    ecx
497C8F  mov     ecx, [ebp+arg_4]
497C92  call    sub_456CC0
497C97  mov     esi, eax
497C99  mov     eax, esi
497C9B  pop     esi
497C9C  pop     ebp
497C9D  jmp     sub_497120

;==== 497CB0 sub_497CB0
497CB0  push    ebp
497CB1  mov     ebp, esp
497CB3  and     esp, 0FFFFFFF8h
497CB6  push    ecx
497CB7  push    ebx
497CB8  push    esi
497CB9  mov     esi, [ebp+arg_0]
497CBC  cmp     dword ptr [esi+20h], 0
497CC0  push    edi
497CC1  jnz     loc_497DA4
497CC7  mov     ebx, ds:LeaveCriticalSection
497CCD  lea     edi, [esi+8]
497CD0  push    edi; lpCriticalSection
497CD1  call    ds:EnterCriticalSection
497CD7  cmp     dword ptr [esi+24h], 1
497CDB  jnz     loc_497D81
497CE1  push    edi; lpCriticalSection
497CE2  mov     dword ptr [esi+24h], 2
497CE9  call    ebx ; LeaveCriticalSection
497CEB  mov     eax, [esi+28h]
497CEE  sub     eax, 0
497CF1  jz      short loc_497D4B
497CF3  dec     eax
497CF4  jz      short loc_497D24
497CF6  dec     eax
497CF7  jz      short loc_497D02
497CF9  mov     dword ptr [esi+68h], 0Fh
497D00  jmp     short loc_497D73
497D02  mov     eax, [esi+38h]
497D05  mov     ecx, [esi+34h]
497D08  mov     edx, [esi+2Ch]
497D0B  push    eax
497D0C  mov     eax, [esi+70h]
497D0F  push    ecx
497D10  mov     ecx, [esi+30h]
497D13  push    edx
497D14  push    eax
497D15  call    sub_458A70
497D1A  call    sub_497120
497D1F  mov     [esi+68h], eax
497D22  jmp     short loc_497D73
497D24  mov     ecx, [esi+34h]
497D27  mov     edx, [esi+30h]
497D2A  mov     eax, [esi+70h]
497D2D  push    ecx
497D2E  mov     ecx, [esi+2Ch]
497D31  push    edx
497D32  push    eax
497D33  call    sub_4586E0
497D38  call    sub_497120
497D3D  mov     ecx, [esi+30h]
497D40  push    ecx; void *
497D41  mov     [esi+68h], eax
497D44  call    ??3@YAXPAX@Z; operator delete(void *)
497D49  jmp     short loc_497D70
497D4B  mov     edx, [esi+34h]
497D4E  mov     eax, [esi+30h]
497D51  mov     ecx, [esi+2Ch]
497D54  push    edx
497D55  push    eax
497D56  push    ecx
497D57  mov     ecx, [esi+70h]
497D5A  call    sub_458550
497D5F  call    sub_497120
497D64  mov     edx, [esi+34h]
497D67  push    edx; void *
497D68  mov     [esi+68h], eax
497D6B  call    ??_V@YAXPAX@Z; operator delete[](void *)
497D70  add     esp, 4
497D73  push    edi; lpCriticalSection
497D74  call    ds:EnterCriticalSection
497D7A  mov     dword ptr [esi+24h], 0
497D81  push    edi; lpCriticalSection
497D82  call    ebx ; LeaveCriticalSection
497D84  mov     eax, [esi+4]
497D87  push    eax; hEvent
497D88  call    ds:ResetEvent
497D8E  mov     ecx, [esi+4]
497D91  push    0FFFFFFFFh; dwMilliseconds
497D93  push    ecx; hHandle
497D94  call    ds:WaitForSingleObject
497D9A  cmp     dword ptr [esi+20h], 0
497D9E  jz      loc_497CD0
497DA4  push    0; ReturnCode
497DA6  call    __endthreadex

;==== 497DC0 sub_497DC0
497DC0  push    ebp
497DC1  mov     ebp, esp
497DC3  push    0FFFFFFFFh
497DC5  push    offset SEH_497DC0
497DCA  mov     eax, large fs:0
497DD0  push    eax
497DD1  sub     esp, 8
497DD4  push    ebx
497DD5  push    esi
497DD6  push    edi
497DD7  mov     eax, dword_4FB734
497DDC  xor     eax, ebp
497DDE  push    eax
497DDF  lea     eax, [ebp+var_C]
497DE2  mov     large fs:0, eax
497DE8  push    78h ; 'x'; Size
497DEA  call    ??2@YAPAXI@Z; operator new(uint)
497DEF  push    78h ; 'x'; Size
497DF1  xor     edi, edi
497DF3  mov     ebx, eax
497DF5  push    edi; Val
497DF6  push    ebx; void *
497DF7  call    _memset
497DFC  mov     eax, dword_5669FC
497E01  add     esp, 10h
497E04  push    edi; lpName
497E05  push    1; bInitialState
497E07  inc     eax
497E08  push    1; bManualReset
497E0A  push    edi; lpEventAttributes
497E0B  mov     dword_5669FC, eax
497E10  mov     [ebx], eax
497E12  call    ds:CreateEventA
497E18  lea     esi, [ebx+8]
497E1B  push    esi; lpCriticalSection
497E1C  mov     [ebx+4], eax
497E1F  call    ds:InitializeCriticalSection
497E25  lea     eax, [ebp+ThrdAddr]
497E28  push    eax; ThrdAddr
497E29  push    edi; InitFlag
497E2A  push    ebx; ArgList
497E2B  push    offset sub_497CB0; StartAddress
497E30  push    edi; StackSize
497E31  push    edi; Security
497E32  mov     [ebx+20h], edi
497E35  mov     [ebx+24h], edi
497E38  call    __beginthreadex
497E3D  add     esp, 18h
497E40  cmp     eax, edi
497E42  jz      short loc_497EAB
497E44  push    160h; Size
497E49  mov     [ebx+6Ch], eax
497E4C  call    ??2@YAPAXI@Z; operator new(uint)
497E51  mov     esi, eax
497E53  add     esp, 4
497E56  mov     [ebp+var_14], esi
497E59  mov     [ebp+var_4], edi
497E5C  cmp     esi, edi
497E5E  jz      short loc_497E75
497E60  mov     eax, dword_56670C
497E65  call    sub_454260
497E6A  mov     edi, eax
497E6C  call    sub_4583E0
497E71  xor     edi, edi
497E73  jmp     short loc_497E77
497E75  xor     eax, eax
497E77  mov     [ebp+var_4], 0FFFFFFFFh
497E7E  mov     [ebx+70h], eax
497E81  mov     ecx, dword_566A00
497E87  mov     eax, [ebp+arg_0]
497E8A  mov     [ebx+74h], ecx
497E8D  mov     dword_566A00, ebx
497E93  mov     edx, [ebx]
497E95  mov     [eax], edx
497E97  xor     eax, eax
497E99  mov     ecx, [ebp+var_C]
497E9C  mov     large fs:0, ecx
497EA3  pop     ecx
497EA4  pop     edi
497EA5  pop     esi
497EA6  pop     ebx
497EA7  mov     esp, ebp
497EA9  pop     ebp
497EAA  retn
497EAB  push    esi; lpCriticalSection
497EAC  call    ds:DeleteCriticalSection
497EB2  mov     ecx, [ebx+4]
497EB5  push    ecx; hObject
497EB6  call    ds:CloseHandle
497EBC  push    ebx; void *
497EBD  call    ??3@YAXPAX@Z; operator delete(void *)
497EC2  add     esp, 4
497EC5  mov     eax, 0Dh
497ECA  mov     ecx, [ebp+var_C]
497ECD  mov     large fs:0, ecx
497ED4  pop     ecx
497ED5  pop     edi
497ED6  pop     esi
497ED7  pop     ebx
497ED8  mov     esp, ebp
497EDA  pop     ebp
497EDB  retn
4DA590  mov     eax, [ebp+var_14]
4DA593  push    eax; void *
4DA594  call    ??3@YAXPAX@Z; operator delete(void *)
4DA599  pop     ecx
4DA59A  retn
4DA59B  mov     edx, [esp-4+arg_4]
4DA59F  lea     eax, [edx+0Ch]
4DA5A2  mov     ecx, [edx-18h]
4DA5A5  xor     ecx, eax
4DA5A7  call    sub_4ABDD5
4DA5AC  mov     eax, offset stru_4F7E68
4DA5B1  jmp     ___CxxFrameHandler3

;==== 497EE0 sub_497EE0
497EE0  push    ebp
497EE1  mov     ebp, esp
497EE3  and     esp, 0FFFFFFF8h
497EE6  push    0FFFFFFFFh
497EE8  push    offset SEH_497EE0
497EED  mov     eax, large fs:0
497EF3  push    eax
497EF4  sub     esp, 10h
497EF7  push    ebx
497EF8  push    esi
497EF9  push    edi
497EFA  mov     eax, dword_4FB734
497EFF  xor     eax, esp
497F01  push    eax
497F02  lea     eax, [esp+2Ch+var_C]
497F06  mov     large fs:0, eax
497F0C  mov     edi, dword_566A00
497F12  mov     eax, 1
497F17  mov     ebx, offset dword_566A00
497F1C  test    edi, edi
497F1E  jz      loc_497FE1
497F24  cmp     ecx, [edi]
497F26  jz      short loc_497F44
497F28  lea     ebx, [edi+74h]
497F2B  mov     edi, [ebx]
497F2D  test    edi, edi
497F2F  jnz     short loc_497F24
497F31  mov     ecx, [esp+2Ch+var_C]
497F35  mov     large fs:0, ecx
497F3C  pop     ecx
497F3D  pop     edi
497F3E  pop     esi
497F3F  pop     ebx
497F40  mov     esp, ebp
497F42  pop     ebp
497F43  retn
497F44  lea     esi, [esp+2Ch+var_18]
497F48  mov     [edi+20h], eax
497F4B  call    sub_452B30
497F50  mov     [esp+2Ch+var_4], 0
497F58  cmp     dword ptr [edi+24h], 0
497F5C  jz      short loc_497F74
497F5E  mov     edi, edi
497F60  mov     eax, 1
497F65  lea     esi, [esp+2Ch+var_18]
497F69  call    sub_452C10
497F6E  cmp     dword ptr [edi+24h], 0
497F72  jnz     short loc_497F60
497F74  mov     eax, [edi+4]
497F77  push    eax; hEvent
497F78  call    ds:SetEvent
497F7E  mov     esi, ds:WaitForSingleObject
497F84  mov     ecx, [edi+6Ch]
497F87  push    1; dwMilliseconds
497F89  push    ecx; hHandle
497F8A  call    esi ; WaitForSingleObject
497F8C  cmp     eax, 102h
497F91  jz      short loc_497F84
497F93  mov     ecx, [edi+70h]
497F96  test    ecx, ecx
497F98  jz      short loc_497FA2
497F9A  mov     edx, [ecx]
497F9C  mov     eax, [edx]
497F9E  push    1
497FA0  call    eax
497FA2  mov     ecx, [edi+6Ch]
497FA5  mov     esi, ds:CloseHandle
497FAB  push    ecx; hObject
497FAC  call    esi ; CloseHandle
497FAE  lea     edx, [edi+8]
497FB1  push    edx; lpCriticalSection
497FB2  call    ds:DeleteCriticalSection
497FB8  mov     eax, [edi+4]
497FBB  push    eax; hObject
497FBC  call    esi ; CloseHandle
497FBE  mov     ecx, [edi+74h]
497FC1  push    edi; void *
497FC2  mov     [ebx], ecx
497FC4  call    ??3@YAXPAX@Z; operator delete(void *)
497FC9  add     esp, 4
497FCC  xor     esi, esi
497FCE  mov     [esp+2Ch+var_4], 0FFFFFFFFh
497FD6  lea     ecx, [esp+2Ch+var_18]
497FDA  call    sub_452BF0
497FDF  mov     eax, esi
497FE1  mov     ecx, [esp+2Ch+var_C]
497FE5  mov     large fs:0, ecx
497FEC  pop     ecx
497FED  pop     edi
497FEE  pop     esi
497FEF  pop     ebx
497FF0  mov     esp, ebp
497FF2  pop     ebp
497FF3  retn
4D8DB0  lea     ecx, [ebp+var_18]
4D8DB3  jmp     sub_452BF0
4D8DB8  mov     edx, [esp-4+arg_4]
4D8DBC  lea     eax, [edx-1Ch]
4D8DBF  mov     ecx, [edx-20h]
4D8DC2  xor     ecx, eax
4D8DC4  call    sub_4ABDD5
4D8DC9  mov     eax, offset stru_4F6664
4D8DCE  jmp     ___CxxFrameHandler3

;==== 498030 sub_498030
498030  mov     eax, dword_566A00
498035  test    eax, eax
498037  jz      short locret_49804B
498039  lea     esp, [esp+0]
498040  cmp     ecx, [eax]
498042  jz      short locret_49804B
498044  mov     eax, [eax+74h]
498047  test    eax, eax
498049  jnz     short loc_498040
49804B  retn

;==== 498050 sub_498050
498050  push    ebp
498051  mov     ebp, esp
498053  push    esi
498054  mov     edx, 1
498059  call    sub_498030
49805E  mov     esi, eax
498060  test    esi, esi
498062  jz      short loc_4980A5
498064  push    edi
498065  lea     edi, [esi+8]
498068  push    edi; lpCriticalSection
498069  call    ds:EnterCriticalSection
49806F  cmp     dword ptr [esi+24h], 0
498073  jnz     short loc_498093
498075  mov     eax, [esi+68h]
498078  mov     ecx, [ebp+arg_0]
49807B  mov     [ecx], eax
49807D  mov     dword ptr [esi+68h], 0FFFF0000h
498084  push    edi; lpCriticalSection
498085  xor     esi, esi
498087  call    ds:LeaveCriticalSection
49808D  pop     edi
49808E  mov     eax, esi
498090  pop     esi
498091  pop     ebp
498092  retn
498093  push    edi; lpCriticalSection
498094  mov     esi, 0Eh
498099  call    ds:LeaveCriticalSection
49809F  pop     edi
4980A0  mov     eax, esi
4980A2  pop     esi
4980A3  pop     ebp
4980A4  retn
4980A5  mov     eax, edx
4980A7  pop     esi
4980A8  pop     ebp
4980A9  retn

;==== 4980B0 sub_4980B0
4980B0  push    ebp
4980B1  mov     ebp, esp
4980B3  push    esi
4980B4  mov     edx, 1
4980B9  call    sub_498030
4980BE  mov     esi, eax
4980C0  test    esi, esi
4980C2  jz      short loc_498107
4980C4  push    edi
4980C5  lea     edi, [esi+8]
4980C8  push    edi; lpCriticalSection
4980C9  call    ds:EnterCriticalSection
4980CF  cmp     dword ptr [esi+24h], 0
4980D3  jnz     short loc_4980F5
4980D5  mov     eax, [esi+70h]
4980D8  mov     ecx, [ebp+arg_0]
4980DB  push    eax
4980DC  call    sub_458530
4980E1  call    sub_497120
4980E6  push    edi; lpCriticalSection
4980E7  mov     esi, eax
4980E9  call    ds:LeaveCriticalSection
4980EF  pop     edi
4980F0  mov     eax, esi
4980F2  pop     esi
4980F3  pop     ebp
4980F4  retn
4980F5  push    edi; lpCriticalSection
4980F6  mov     esi, 0Eh
4980FB  call    ds:LeaveCriticalSection
498101  pop     edi
498102  mov     eax, esi
498104  pop     esi
498105  pop     ebp
498106  retn
498107  mov     eax, edx
498109  pop     esi
49810A  pop     ebp
49810B  retn

;==== 498110 sub_498110
498110  push    ebp
498111  mov     ebp, esp
498113  sub     esp, 8
498116  push    ebx
498117  push    esi
498118  mov     ebx, 1
49811D  call    sub_498030
498122  mov     esi, eax
498124  test    esi, esi
498126  jz      loc_4981FC
49812C  call    sub_497310
498131  mov     [ebp+var_4], eax
498134  test    eax, eax
498136  jz      loc_4981FC
49813C  lea     ebx, [esi+8]
49813F  push    ebx; lpCriticalSection
498140  mov     [ebp+lpCriticalSection], ebx
498143  call    ds:EnterCriticalSection
498149  cmp     dword ptr [esi+24h], 0
49814D  jnz     loc_4981E8
498153  cmp     [ebp+arg_4], 0
498157  jz      short loc_4981C1
498159  xor     ecx, ecx
49815B  mov     eax, edi
49815D  mov     edx, 834h
498162  mul     edx
498164  seto    cl
498167  neg     ecx
498169  or      ecx, eax
49816B  push    ecx; unsigned int
49816C  call    ??_U@YAPAXI@Z; operator new[](uint)
498171  mov     ecx, [ebp+Src]
498174  mov     ebx, eax
498176  mov     eax, edi
498178  imul    eax, 834h
49817E  push    eax; Size
49817F  push    ecx; Src
498180  push    ebx; void *
498181  call    _memcpy_0
498186  mov     eax, [esi+4]
498189  mov     edx, [ebp+var_4]
49818C  add     esp, 10h
49818F  push    eax; hEvent
498190  mov     dword ptr [esi+24h], 1
498197  mov     dword ptr [esi+28h], 0
49819E  mov     [esi+2Ch], edx
4981A1  mov     [esi+30h], edi
4981A4  mov     [esi+34h], ebx
4981A7  call    ds:SetEvent
4981AD  mov     ebx, [ebp+lpCriticalSection]
4981B0  push    ebx; lpCriticalSection
4981B1  xor     esi, esi
4981B3  call    ds:LeaveCriticalSection
4981B9  mov     eax, esi
4981BB  pop     esi
4981BC  pop     ebx
4981BD  mov     esp, ebp
4981BF  pop     ebp
4981C0  retn
4981C1  mov     ecx, [ebp+Src]
4981C4  mov     edx, [ebp+var_4]
4981C7  push    ecx
4981C8  mov     ecx, [esi+70h]
4981CB  push    edi
4981CC  push    edx
4981CD  call    sub_458550
4981D2  call    sub_497120
4981D7  push    ebx; lpCriticalSection
4981D8  mov     esi, eax
4981DA  call    ds:LeaveCriticalSection
4981E0  mov     eax, esi
4981E2  pop     esi
4981E3  pop     ebx
4981E4  mov     esp, ebp
4981E6  pop     ebp
4981E7  retn
4981E8  push    ebx; lpCriticalSection
4981E9  mov     esi, 0Eh
4981EE  call    ds:LeaveCriticalSection
4981F4  mov     eax, esi
4981F6  pop     esi
4981F7  pop     ebx
4981F8  mov     esp, ebp
4981FA  pop     ebp
4981FB  retn
4981FC  pop     esi
4981FD  mov     eax, ebx
4981FF  pop     ebx
498200  mov     esp, ebp
498202  pop     ebp
498203  retn

;==== 498210 sub_498210
498210  push    ebx
498211  push    esi
498212  mov     ebx, 1
498217  call    sub_498030
49821C  mov     esi, eax
49821E  test    esi, esi
498220  jz      short loc_498274
498222  push    edi
498223  call    sub_497310
498228  mov     edi, eax
49822A  test    edi, edi
49822C  jz      short loc_49826E
49822E  lea     ebx, [esi+8]
498231  push    ebx; lpCriticalSection
498232  call    ds:EnterCriticalSection
498238  cmp     dword ptr [esi+24h], 0
49823C  jnz     short loc_49825C
49823E  mov     eax, edi
498240  mov     edi, [esi+70h]
498243  call    sub_4586B0
498248  call    sub_497120
49824D  push    ebx; lpCriticalSection
49824E  mov     esi, eax
498250  call    ds:LeaveCriticalSection
498256  pop     edi
498257  mov     eax, esi
498259  pop     esi
49825A  pop     ebx
49825B  retn
49825C  push    ebx; lpCriticalSection
49825D  mov     esi, 0Eh
498262  call    ds:LeaveCriticalSection
498268  pop     edi
498269  mov     eax, esi
49826B  pop     esi
49826C  pop     ebx
49826D  retn
49826E  pop     edi
49826F  pop     esi
498270  mov     eax, ebx
498272  pop     ebx
498273  retn
498274  pop     esi
498275  mov     eax, ebx
498277  pop     ebx
498278  retn

;==== 498280 sub_498280
498280  push    ebp
498281  mov     ebp, esp
498283  push    ecx
498284  push    ebx
498285  push    esi
498286  mov     esi, eax
498288  mov     edx, 1
49828D  call    sub_498030
498292  mov     ebx, eax
498294  test    ebx, ebx
498296  jz      loc_498342
49829C  push    edi
49829D  lea     edi, [ebx+8]
4982A0  push    edi; lpCriticalSection
4982A1  mov     [ebp+lpCriticalSection], edi
4982A4  call    ds:EnterCriticalSection
4982AA  cmp     dword ptr [ebx+24h], 0
4982AE  jnz     short loc_49832D
4982B0  cmp     [ebp+arg_8], 0
4982B4  jz      short loc_498305
4982B6  push    834h; Size
4982BB  call    ??2@YAPAXI@Z; operator new(uint)
4982C0  mov     edx, [ebp+arg_4]
4982C3  mov     edi, eax
4982C5  mov     ecx, 20Dh
4982CA  rep movsd
4982CC  mov     ecx, 1
4982D1  mov     [ebx+30h], eax
4982D4  mov     eax, [ebx+4]
4982D7  add     esp, 4
4982DA  mov     [ebx+24h], ecx
4982DD  mov     [ebx+28h], ecx
4982E0  mov     ecx, [ebp+arg_0]
4982E3  push    eax; hEvent
4982E4  mov     [ebx+2Ch], ecx
4982E7  mov     [ebx+34h], edx
4982EA  call    ds:SetEvent
4982F0  mov     edi, [ebp+lpCriticalSection]
4982F3  push    edi; lpCriticalSection
4982F4  xor     esi, esi
4982F6  call    ds:LeaveCriticalSection
4982FC  pop     edi
4982FD  mov     eax, esi
4982FF  pop     esi
498300  pop     ebx
498301  mov     esp, ebp
498303  pop     ebp
498304  retn
498305  mov     ecx, [ebp+arg_4]
498308  mov     edx, [ebx+70h]
49830B  push    ecx
49830C  mov     ecx, [ebp+arg_0]
49830F  push    esi
498310  push    edx
498311  call    sub_4586E0
498316  call    sub_497120
49831B  push    edi; lpCriticalSection
49831C  mov     esi, eax
49831E  call    ds:LeaveCriticalSection
498324  pop     edi
498325  mov     eax, esi
498327  pop     esi
498328  pop     ebx
498329  mov     esp, ebp
49832B  pop     ebp
49832C  retn
49832D  push    edi; lpCriticalSection
49832E  mov     esi, 0Eh
498333  call    ds:LeaveCriticalSection
498339  pop     edi
49833A  mov     eax, esi
49833C  pop     esi
49833D  pop     ebx
49833E  mov     esp, ebp
498340  pop     ebp
498341  retn
498342  pop     esi
498343  mov     eax, edx
498345  pop     ebx
498346  mov     esp, ebp
498348  pop     ebp
498349  retn

;==== 498350 sub_498350
498350  push    ebp
498351  mov     ebp, esp
498353  push    esi
498354  mov     edx, 1
498359  call    sub_498030
49835E  mov     esi, eax
498360  test    esi, esi
498362  jz      short loc_4983AA
498364  push    edi
498365  lea     edi, [esi+8]
498368  push    edi; lpCriticalSection
498369  call    ds:EnterCriticalSection
49836F  cmp     dword ptr [esi+24h], 0
498373  jnz     short loc_498398
498375  mov     eax, [ebp+arg_0]
498378  mov     ecx, [esi+70h]
49837B  push    eax
49837C  mov     eax, [ebp+arg_4]
49837F  call    sub_458A40
498384  call    sub_497120
498389  push    edi; lpCriticalSection
49838A  mov     esi, eax
49838C  call    ds:LeaveCriticalSection
498392  pop     edi
498393  mov     eax, esi
498395  pop     esi
498396  pop     ebp
498397  retn
498398  push    edi; lpCriticalSection
498399  mov     esi, 0Eh
49839E  call    ds:LeaveCriticalSection
4983A4  pop     edi
4983A5  mov     eax, esi
4983A7  pop     esi
4983A8  pop     ebp
4983A9  retn
4983AA  mov     eax, edx
4983AC  pop     esi
4983AD  pop     ebp
4983AE  retn

;==== 4983B0 sub_4983B0
4983B0  push    ebp
4983B1  mov     ebp, esp
4983B3  push    esi
4983B4  mov     edx, 1
4983B9  call    sub_498030
4983BE  mov     esi, eax
4983C0  test    esi, esi
4983C2  jz      short loc_498426
4983C4  push    edi
4983C5  lea     edi, [esi+8]
4983C8  push    edi; lpCriticalSection
4983C9  call    ds:EnterCriticalSection
4983CF  cmp     dword ptr [esi+24h], 0
4983D3  jnz     short loc_498414
4983D5  mov     ecx, [ebp+arg_4]
4983D8  mov     eax, [ebp+arg_0]
4983DB  mov     edx, [ebp+arg_8]
4983DE  mov     [esi+30h], ecx
4983E1  mov     ecx, [esi+4]
4983E4  mov     [esi+2Ch], eax
4983E7  mov     eax, [ebp+arg_C]
4983EA  push    ecx; hEvent
4983EB  mov     dword ptr [esi+24h], 1
4983F2  mov     dword ptr [esi+28h], 2
4983F9  mov     [esi+34h], edx
4983FC  mov     [esi+38h], eax
4983FF  call    ds:SetEvent
498405  push    edi; lpCriticalSection
498406  xor     esi, esi
498408  call    ds:LeaveCriticalSection
49840E  pop     edi
49840F  mov     eax, esi
498411  pop     esi
498412  pop     ebp
498413  retn
498414  push    edi; lpCriticalSection
498415  mov     esi, 0Eh
49841A  call    ds:LeaveCriticalSection
498420  pop     edi
498421  mov     eax, esi
498423  pop     esi
498424  pop     ebp
498425  retn
498426  mov     eax, edx
498428  pop     esi
498429  pop     ebp
49842A  retn

;==== 4ABDD5 sub_4ABDD5
4ABDD5  cmp     ecx, dword_4FB734
4ABDDB  jnz     short loc_4ABDDF
4ABDDD  rep retn
4ABDDF  jmp     ___report_gsfailure

;==== 4ACF3D sub_4ACF3D
4ACF3D  mov     dword ptr [ecx], offset ??_7exception@std@@6B@; const std::exception::`vftable'
4ACF43  jmp     ?_Tidy@exception@std@@AAEXXZ; std::exception::_Tidy(void)

;==== 4ACF48 sub_4ACF48
4ACF48  mov     edi, edi
4ACF4A  push    ebp
4ACF4B  mov     ebp, esp
4ACF4D  push    esi
4ACF4E  mov     esi, ecx
4ACF50  mov     dword ptr [esi], offset ??_7exception@std@@6B@; const std::exception::`vftable'
4ACF56  call    ?_Tidy@exception@std@@AAEXXZ; std::exception::_Tidy(void)
4ACF5B  test    [ebp+arg_0], 1
4ACF5F  jz      short loc_4ACF68
4ACF61  push    esi; void *
4ACF62  call    ??3@YAXPAX@Z; operator delete(void *)
4ACF67  pop     ecx
4ACF68  mov     eax, esi
4ACF6A  pop     esi
4ACF6B  pop     ebp
4ACF6C  retn    4

;==== 4D649A sub_4D649A
4D649A  mov     edx, offset dword_4EDBC0
4D649F  jmp     __cintrindisp2

