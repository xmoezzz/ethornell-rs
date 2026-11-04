;vtable 4E5D3C const DCParticle::`vftable'
;   +00 44DC90 sub_44DC90
;   +04 44CCC0 sub_44CCC0
;   +08 41B220 sub_41B220
;   +0C 44DF60 sub_44DF60
;   +10 44DFA0 sub_44DFA0
;vtable 4E5D7C const DCPFirefly::`vftable'
;   +00 44FA30 sub_44FA30
;   +04 44FD50 sub_44FD50
;   +08 44FF50 sub_44FF50
;   +0C 44FF80 sub_44FF80
;   +10 44FFD0 sub_44FFD0
;vtable 4E5D94 const DCPSnow::`vftable'
;   +00 4504F0 sub_4504F0
;   +04 4506D0 sub_4506D0
;   +08 41B220 sub_41B220
;   +0C 44DF60 sub_44DF60
;   +10 450740 sub_450740

;==== 407AD0 sub_407AD0
407AD0  cmp     dword_565B18, 0
407AD7  jnz     short loc_407AE8
407AD9  cmp     dword_565B1C, 0Fh
407AE0  jz      short loc_407AE8
407AE2  mov     eax, 1
407AE7  retn
407AE8  xor     eax, eax
407AEA  retn

;==== 407B20 sub_407B20
407B20  mov     eax, ds:dword_4E41A0[eax*4]
407B27  retn

;==== 409020 sub_409020
409020  push    ebp
409021  mov     ebp, esp
409023  mov     ecx, [ebp+arg_0]
409026  mov     [esi+8], ecx
409029  mov     [esi+0Ch], edx
40902C  mov     [esi+10h], eax
40902F  call    sub_407B20
409034  mov     [esi+14h], eax
409037  imul    eax, ecx
40903A  mov     [esi+4], eax
40903D  test    ecx, ecx
40903F  jz      short loc_40905C
409041  test    edx, edx
409043  jz      short loc_40905C
409045  imul    eax, edx
409048  push    eax; unsigned int
409049  call    ??_U@YAPAXI@Z; operator new[](uint)
40904E  mov     [esi], eax
409050  xor     eax, eax
409052  add     esp, 4
409055  cmp     [esi], eax
409057  setnz   al
40905A  pop     ebp
40905B  retn
40905C  xor     eax, eax
40905E  mov     dword ptr [esi], 0
409064  cmp     [esi], eax
409066  setnz   al
409069  pop     ebp
40906A  retn

;==== 4090A0 sub_4090A0
4090A0  mov     edx, [eax]
4090A2  cmp     edx, [ecx]
4090A4  jg      short loc_4090C4
4090A6  mov     edx, [ecx+8]
4090A9  cmp     edx, [eax+8]
4090AC  jg      short loc_4090C4
4090AE  mov     edx, [eax+4]
4090B1  cmp     edx, [ecx+4]
4090B4  jg      short loc_4090C4
4090B6  mov     ecx, [ecx+0Ch]
4090B9  cmp     ecx, [eax+0Ch]
4090BC  jg      short loc_4090C4
4090BE  mov     eax, 1
4090C3  retn
4090C4  xor     eax, eax
4090C6  retn

;==== 4090D0 sub_4090D0
4090D0  mov     edx, [ecx]
4090D2  cmp     edx, [eax+8]
4090D5  jg      short loc_4090F4
4090D7  mov     edx, [eax]
4090D9  cmp     edx, [ecx+8]
4090DC  jg      short loc_4090F4
4090DE  mov     edx, [ecx+4]
4090E1  cmp     edx, [eax+0Ch]
4090E4  jg      short loc_4090F4
4090E6  mov     eax, [eax+4]
4090E9  cmp     eax, [ecx+0Ch]
4090EC  jg      short loc_4090F4
4090EE  mov     eax, 1
4090F3  retn
4090F4  xor     eax, eax
4090F6  retn

;==== 409100 sub_409100
409100  mov     edx, [eax]
409102  push    ebx
409103  push    esi
409104  mov     esi, [ecx]
409106  push    edi
409107  mov     ebx, esi
409109  cmp     edx, esi
40910B  jl      short loc_40910F
40910D  mov     ebx, edx
40910F  mov     edx, [eax+4]
409112  mov     [eax], ebx
409114  mov     esi, [ecx+4]
409117  mov     edi, esi
409119  cmp     edx, esi
40911B  jl      short loc_40911F
40911D  mov     edi, edx
40911F  mov     edx, [eax+8]
409122  mov     [eax+4], edi
409125  mov     esi, [ecx+8]
409128  cmp     edx, esi
40912A  jge     short loc_40912E
40912C  mov     esi, edx
40912E  mov     edx, [eax+0Ch]
409131  mov     [eax+8], esi
409134  mov     ecx, [ecx+0Ch]
409137  cmp     edx, ecx
409139  jge     short loc_40913D
40913B  mov     ecx, edx
40913D  mov     [eax+0Ch], ecx
409140  cmp     ebx, esi
409142  jg      short loc_409151
409144  cmp     edi, ecx
409146  jg      short loc_409151
409148  pop     edi
409149  pop     esi
40914A  mov     eax, 1
40914F  pop     ebx
409150  retn
409151  pop     edi
409152  pop     esi
409153  xor     eax, eax
409155  pop     ebx
409156  retn

;==== 409160 sub_409160
409160  push    ebp
409161  mov     ebp, esp
409163  mov     eax, [ebp+arg_0]
409166  add     [eax], edx
409168  add     [eax+8], edx
40916B  add     [eax+4], ecx
40916E  add     [eax+0Ch], ecx
409171  pop     ebp
409172  retn

;==== 409180 sub_409180
409180  mov     dword ptr [eax], 0
409186  mov     dword ptr [eax+4], 0
40918D  mov     edx, [ecx+8]
409190  dec     edx
409191  mov     [eax+8], edx
409194  mov     ecx, [ecx+0Ch]
409197  dec     ecx
409198  mov     [eax+0Ch], ecx
40919B  retn

;==== 4091A0 sub_4091A0
4091A0  push    ebp
4091A1  mov     ebp, esp
4091A3  sub     esp, 18h
4091A6  push    edi
4091A7  mov     ecx, esi
4091A9  lea     eax, [ebp+var_14]
4091AC  xor     edi, edi
4091AE  call    sub_409180
4091B3  mov     ecx, [ebp+arg_0]
4091B6  call    sub_409100
4091BB  test    eax, eax
4091BD  jz      short loc_4091EF
4091BF  mov     eax, [ebp+var_14]
4091C2  mov     ecx, [ebp+var_C]
4091C5  mov     edx, [ebp+var_8]
4091C8  sub     ecx, eax
4091CA  inc     ecx
4091CB  mov     [esi+8], ecx
4091CE  mov     ecx, [ebp+var_10]
4091D1  sub     edx, ecx
4091D3  inc     edx
4091D4  mov     [esi+0Ch], edx
4091D7  mov     edx, [esi+4]
4091DA  imul    edx, ecx
4091DD  mov     ecx, [esi+14h]
4091E0  imul    ecx, eax
4091E3  add     edx, ecx
4091E5  add     [esi], edx
4091E7  lea     eax, [edi+1]
4091EA  pop     edi
4091EB  mov     esp, ebp
4091ED  pop     ebp
4091EE  retn
4091EF  mov     eax, edi
4091F1  pop     edi
4091F2  mov     esp, ebp
4091F4  pop     ebp
4091F5  retn

;==== 40A520 sub_40A520
40A520  push    ebp
40A521  mov     ebp, esp
40A523  sub     esp, 54h
40A526  mov     edx, [ecx]
40A528  mov     [ebp+var_54], edx
40A52B  mov     edx, [ecx+4]
40A52E  mov     [ebp+var_50], edx
40A531  mov     edx, [ecx+8]
40A534  mov     [ebp+var_4C], edx
40A537  mov     edx, [ecx+0Ch]
40A53A  mov     [ebp+var_48], edx
40A53D  mov     edx, [ecx+10h]
40A540  mov     ecx, [ecx+14h]
40A543  mov     [ebp+var_44], edx
40A546  mov     edx, [eax]
40A548  mov     [ebp+var_40], ecx
40A54B  mov     ecx, [eax+4]
40A54E  mov     [ebp+var_3C], edx
40A551  mov     edx, [eax+8]
40A554  mov     [ebp+var_38], ecx
40A557  mov     ecx, [eax+0Ch]
40A55A  mov     [ebp+var_34], edx
40A55D  mov     edx, [eax+10h]
40A560  mov     eax, [eax+14h]
40A563  push    ebx
40A564  mov     ebx, [ebp+arg_0]
40A567  mov     [ebp+var_30], ecx
40A56A  mov     [ebp+var_28], eax
40A56D  push    esi
40A56E  lea     ecx, [ebp+var_54]
40A571  lea     eax, [ebp+var_24]
40A574  mov     [ebp+var_2C], edx
40A577  call    sub_409180
40A57C  lea     ecx, [ebp+var_3C]
40A57F  lea     eax, [ebp+var_14]
40A582  call    sub_409180
40A587  lea     eax, [ebp+var_24]
40A58A  mov     ecx, edi
40A58C  mov     edx, ebx
40A58E  neg     ecx
40A590  neg     edx
40A592  push    eax
40A593  call    sub_409160
40A598  add     esp, 4
40A59B  lea     ecx, [ebp+var_24]
40A59E  lea     eax, [ebp+var_14]
40A5A1  call    sub_409100
40A5A6  test    eax, eax
40A5A8  jz      short loc_40A5F7
40A5AA  lea     ecx, [ebp+var_14]
40A5AD  push    ecx
40A5AE  lea     esi, [ebp+var_3C]
40A5B1  call    sub_4091A0
40A5B6  lea     edx, [ebp+var_14]
40A5B9  add     esp, 4
40A5BC  push    edx
40A5BD  mov     ecx, edi
40A5BF  mov     edx, ebx
40A5C1  call    sub_409160
40A5C6  lea     eax, [ebp+var_14]
40A5C9  add     esp, 4
40A5CC  push    eax
40A5CD  lea     esi, [ebp+var_54]
40A5D0  call    sub_4091A0
40A5D5  mov     ecx, [ebp+arg_8]
40A5D8  mov     edx, [ebp+arg_4]
40A5DB  add     esp, 4
40A5DE  push    1
40A5E0  push    ecx
40A5E1  push    edx
40A5E2  lea     eax, [ebp+var_3C]
40A5E5  push    eax
40A5E6  mov     ecx, esi
40A5E8  push    ecx
40A5E9  call    sub_40AA10
40A5EE  add     esp, 14h
40A5F1  pop     esi
40A5F2  pop     ebx
40A5F3  mov     esp, ebp
40A5F5  pop     ebp
40A5F6  retn
40A5F7  pop     esi
40A5F8  mov     eax, 4
40A5FD  pop     ebx
40A5FE  mov     esp, ebp
40A600  pop     ebp
40A601  retn

;==== 40A610 sub_40A610
40A610  push    ebp
40A611  mov     ebp, esp
40A613  and     esp, 0FFFFFFF8h
40A616  sub     esp, 1Ch
40A619  mov     edx, [eax+4]
40A61C  push    ebx
40A61D  mov     ebx, [eax+10h]
40A620  push    esi
40A621  mov     esi, [eax]
40A623  push    edi
40A624  mov     edi, [eax+8]
40A627  mov     [esp+28h+var_10], edi
40A62B  mov     edi, [eax+0Ch]
40A62E  mov     eax, [eax+14h]
40A631  mov     [esp+28h+var_18], esi
40A635  mov     [esp+28h+var_14], edx
40A639  mov     [esp+28h+var_C], edi
40A63D  mov     [esp+28h+var_8], ebx
40A641  mov     [esp+28h+var_4], eax
40A645  test    ecx, ecx
40A647  jz      short loc_40A666
40A649  push    ecx
40A64A  lea     esi, [esp+2Ch+var_18]
40A64E  call    sub_4091A0
40A653  mov     eax, [esp+2Ch+var_4]
40A657  mov     edi, [esp+2Ch+var_C]
40A65B  mov     esi, [esp+2Ch+var_18]
40A65F  mov     edx, [esp+2Ch+var_14]
40A663  add     esp, 4
40A666  imul    eax, [esp+28h+var_10]
40A66B  mov     ebx, eax
40A66D  test    bl, 7
40A670  jnz     short loc_40A6DB
40A672  or      eax, edx
40A674  or      eax, esi
40A676  test    al, 0Fh
40A678  jnz     short loc_40A6A6
40A67A  shr     ebx, 4
40A67D  xorps   xmm0, xmm0
40A680  test    edi, edi
40A682  jz      short loc_40A6F2
40A684  dec     edi
40A685  mov     ecx, esi
40A687  mov     eax, ebx
40A689  test    ebx, ebx
40A68B  jz      short loc_40A699
40A68D  lea     ecx, [ecx+0]
40A690  movntps xmmword ptr [ecx], xmm0
40A693  add     ecx, 10h
40A696  dec     eax
40A697  jnz     short loc_40A690
40A699  add     esi, edx
40A69B  test    edi, edi
40A69D  jnz     short loc_40A684
40A69F  pop     edi
40A6A0  pop     esi
40A6A1  pop     ebx
40A6A2  mov     esp, ebp
40A6A4  pop     ebp
40A6A5  retn
40A6A6  shr     ebx, 3
40A6A9  pxor    mm0, mm0
40A6AC  test    edi, edi
40A6AE  jz      short loc_40A6D2
40A6B0  dec     edi
40A6B1  mov     ecx, esi
40A6B3  mov     eax, ebx
40A6B5  test    ebx, ebx
40A6B7  jz      short loc_40A6CC
40A6B9  lea     esp, [esp+0]
40A6C0  movq    mm1, mm0
40A6C3  movntq  qword ptr [ecx], mm1
40A6C6  add     ecx, 8
40A6C9  dec     eax
40A6CA  jnz     short loc_40A6C0
40A6CC  add     esi, edx
40A6CE  test    edi, edi
40A6D0  jnz     short loc_40A6B0
40A6D2  emms
40A6D4  pop     edi
40A6D5  pop     esi
40A6D6  pop     ebx
40A6D7  mov     esp, ebp
40A6D9  pop     ebp
40A6DA  retn
40A6DB  test    edi, edi
40A6DD  jz      short loc_40A6F2
40A6DF  push    ebx; Size
40A6E0  push    0; Val
40A6E2  push    esi; void *
40A6E3  call    _memset
40A6E8  add     esi, [esp+34h+var_14]
40A6EC  add     esp, 0Ch
40A6EF  dec     edi
40A6F0  jnz     short loc_40A6DF
40A6F2  pop     edi
40A6F3  pop     esi
40A6F4  pop     ebx
40A6F5  mov     esp, ebp
40A6F7  pop     ebp
40A6F8  retn

;==== 40A990 sub_40A990
40A990  push    ebp
40A991  mov     ebp, esp
40A993  sub     esp, 24h
40A996  mov     ecx, esi
40A998  lea     eax, [ebp+var_14]
40A99B  call    sub_409180
40A9A0  mov     ecx, [ebp+arg_0]
40A9A3  lea     eax, [ebp+var_24]
40A9A6  call    sub_409180
40A9AB  mov     ecx, eax
40A9AD  lea     eax, [ebp+var_14]
40A9B0  call    sub_409100
40A9B5  test    eax, eax
40A9B7  jz      short loc_40A9C3
40A9B9  lea     ecx, [ebp+var_14]
40A9BC  mov     eax, esi
40A9BE  call    sub_40A610
40A9C3  mov     esp, ebp
40A9C5  pop     ebp
40A9C6  retn

;==== 40A9D0 sub_40A9D0
40A9D0  xor     eax, eax
40A9D2  cmp     ecx, edx
40A9D4  setz    al
40A9D7  test    eax, eax
40A9D9  jnz     short locret_40A9F7
40A9DB  mov     eax, ecx
40A9DD  dec     eax
40A9DE  jz      short loc_40A9EF
40A9E0  dec     eax
40A9E1  jz      short loc_40A9E6
40A9E3  xor     eax, eax
40A9E5  retn
40A9E6  xor     eax, eax
40A9E8  cmp     edx, 1
40A9EB  setz    al
40A9EE  retn
40A9EF  xor     eax, eax
40A9F1  cmp     edx, 2
40A9F4  setz    al
40A9F7  retn

;==== 40AA10 sub_40AA10
40AA10  push    ebp
40AA11  mov     ebp, esp
40AA13  and     esp, 0FFFFFFF8h
40AA16  sub     esp, 14h
40AA19  push    ebx
40AA1A  mov     ebx, [ebp+arg_0]
40AA1D  mov     ecx, [ebx+10h]
40AA20  push    esi
40AA21  push    edi
40AA22  mov     edi, [ebp+arg_4]
40AA25  mov     edx, [edi+10h]
40AA28  mov     [esp+20h+var_14], 0
40AA30  call    sub_40A9D0
40AA35  test    eax, eax
40AA37  jz      loc_40ACA4
40AA3D  mov     esi, [ebp+arg_C]
40AA40  cmp     esi, 100h
40AA46  ja      loc_40AC91
40AA4C  cmp     [ebp+arg_10], 0
40AA50  jz      short loc_40AA8C
40AA52  mov     eax, [ebp+arg_8]
40AA55  lea     ecx, [esp+20h+var_10]
40AA59  push    ecx
40AA5A  push    0
40AA5C  push    0
40AA5E  push    2
40AA60  lea     edx, [esp+30h+var_8]
40AA64  push    edx
40AA65  push    offset sub_41A380
40AA6A  mov     edx, edi
40AA6C  mov     [esp+38h+var_8], ebx
40AA70  mov     [esp+38h+var_4], edi
40AA74  mov     [esp+38h+var_10], eax
40AA78  mov     [esp+38h+var_C], esi
40AA7C  call    sub_41A120
40AA81  add     esp, 18h
40AA84  test    eax, eax
40AA86  jnz     loc_40ACAC
40AA8C  mov     eax, [ebp+arg_8]
40AA8F  cmp     eax, 0FFh
40AA94  ja      def_40AAB2; jumptable 0040AAB2 default case, cases 10-31,40-63,66-127,129-191,194-239
40AA9A  jz      loc_40AC66
40AAA0  cmp     eax, 0F0h; switch 241 cases
40AAA5  ja      def_40AAB2; jumptable 0040AAB2 default case, cases 10-31,40-63,66-127,129-191,194-239
40AAAB  movzx   eax, ds:byte_40AD14[eax]
40AAB2  jmp     ds:jpt_40AAB2[eax*4]; switch jump
40AAB9  mov     eax, edi; jumptable 0040AAB2 case 0
40AABB  mov     ecx, ebx
40AABD  call    sub_40B100
40AAC2  mov     eax, [esp+20h+var_14]
40AAC6  pop     edi
40AAC7  pop     esi
40AAC8  pop     ebx
40AAC9  mov     esp, ebp
40AACB  pop     ebp
40AACC  retn
40AACD  push    ebx; jumptable 0040AAB2 cases 1,32
40AACE  mov     ecx, esi
40AAD0  mov     eax, edi
40AAD2  call    sub_40B560
40AAD7  mov     eax, [esp+24h+var_14]
40AADB  add     esp, 4
40AADE  pop     edi
40AADF  pop     esi
40AAE0  pop     ebx
40AAE1  mov     esp, ebp
40AAE3  pop     ebp
40AAE4  retn
40AAE5  push    esi; jumptable 0040AAB2 case 2
40AAE6  mov     edx, edi
40AAE8  mov     ecx, ebx
40AAEA  call    sub_40CD70
40AAEF  mov     eax, [esp+24h+var_14]
40AAF3  add     esp, 4
40AAF6  pop     edi
40AAF7  pop     esi
40AAF8  pop     ebx
40AAF9  mov     esp, ebp
40AAFB  pop     ebp
40AAFC  retn
40AAFD  mov     edx, esi; jumptable 0040AAB2 case 3
40AAFF  mov     esi, edi
40AB01  mov     ecx, ebx
40AB03  call    sub_40D5B0
40AB08  mov     eax, [esp+20h+var_14]
40AB0C  pop     edi
40AB0D  pop     esi
40AB0E  pop     ebx
40AB0F  mov     esp, ebp
40AB11  pop     ebp
40AB12  retn
40AB13  mov     ecx, 100h; jumptable 0040AAB2 case 33
40AB18  sub     ecx, esi
40AB1A  push    ecx
40AB1B  mov     edx, edi
40AB1D  mov     ecx, ebx
40AB1F  call    sub_40CD70
40AB24  mov     eax, [esp+24h+var_14]
40AB28  add     esp, 4
40AB2B  pop     edi
40AB2C  pop     esi
40AB2D  pop     ebx
40AB2E  mov     esp, ebp
40AB30  pop     ebp
40AB31  retn
40AB32  mov     edx, 100h; jumptable 0040AAB2 case 34
40AB37  sub     edx, esi
40AB39  mov     esi, edi
40AB3B  mov     ecx, ebx
40AB3D  call    sub_40D5B0
40AB42  mov     eax, [esp+20h+var_14]
40AB46  pop     edi
40AB47  pop     esi
40AB48  pop     ebx
40AB49  mov     esp, ebp
40AB4B  pop     ebp
40AB4C  retn
40AB4D  mov     esi, 100h; jumptable 0040AAB2 case 35
40AB52  sub     esi, [ebp+arg_C]
40AB55  mov     ecx, edi; jumptable 0040AAB2 case 4
40AB57  mov     eax, ebx
40AB59  call    sub_40D850
40AB5E  mov     eax, [esp+20h+var_14]
40AB62  pop     edi
40AB63  pop     esi
40AB64  pop     ebx
40AB65  mov     esp, ebp
40AB67  pop     ebp
40AB68  retn
40AB69  mov     esi, 100h; jumptable 0040AAB2 case 36
40AB6E  sub     esi, [ebp+arg_C]
40AB71  mov     eax, edi; jumptable 0040AAB2 case 6
40AB73  mov     ecx, ebx
40AB75  call    sub_4154B0
40AB7A  mov     eax, [esp+20h+var_14]
40AB7E  pop     edi
40AB7F  pop     esi
40AB80  pop     ebx
40AB81  mov     esp, ebp
40AB83  pop     ebp
40AB84  retn
40AB85  mov     esi, 100h; jumptable 0040AAB2 case 37
40AB8A  sub     esi, [ebp+arg_C]
40AB8D  mov     eax, edi; jumptable 0040AAB2 case 7
40AB8F  mov     ecx, ebx
40AB91  call    sub_416150
40AB96  mov     eax, [esp+20h+var_14]
40AB9A  pop     edi
40AB9B  pop     esi
40AB9C  pop     ebx
40AB9D  mov     esp, ebp
40AB9F  pop     ebp
40ABA0  retn
40ABA1  mov     esi, 100h; jumptable 0040AAB2 case 38
40ABA6  sub     esi, [ebp+arg_C]
40ABA9  mov     eax, edi; jumptable 0040AAB2 case 8
40ABAB  mov     ecx, ebx
40ABAD  call    sub_415790
40ABB2  mov     eax, [esp+20h+var_14]
40ABB6  pop     edi
40ABB7  pop     esi
40ABB8  pop     ebx
40ABB9  mov     esp, ebp
40ABBB  pop     ebp
40ABBC  retn
40ABBD  mov     esi, 100h; jumptable 0040AAB2 case 39
40ABC2  sub     esi, [ebp+arg_C]
40ABC5  mov     eax, edi; jumptable 0040AAB2 case 9
40ABC7  mov     ecx, ebx
40ABC9  call    sub_415AE0
40ABCE  mov     eax, [esp+20h+var_14]
40ABD2  pop     edi
40ABD3  pop     esi
40ABD4  pop     ebx
40ABD5  mov     esp, ebp
40ABD7  pop     ebp
40ABD8  retn
40ABD9  push    edi; jumptable 0040AAB2 case 64
40ABDA  mov     edx, esi
40ABDC  mov     ecx, ebx
40ABDE  call    sub_40E340
40ABE3  mov     eax, [esp+24h+var_14]
40ABE7  add     esp, 4
40ABEA  pop     edi
40ABEB  pop     esi
40ABEC  pop     ebx
40ABED  mov     esp, ebp
40ABEF  pop     ebp
40ABF0  retn
40ABF1  push    edi; jumptable 0040AAB2 case 65
40ABF2  mov     esi, ebx
40ABF4  call    sub_40A990
40ABF9  mov     eax, [esp+24h+var_14]
40ABFD  add     esp, 4
40AC00  pop     edi
40AC01  pop     esi
40AC02  pop     ebx
40AC03  mov     esp, ebp
40AC05  pop     ebp
40AC06  retn
40AC07  mov     eax, edi; jumptable 0040AAB2 case 128
40AC09  mov     ecx, ebx
40AC0B  call    sub_40AF70
40AC10  mov     eax, [esp+20h+var_14]
40AC14  pop     edi
40AC15  pop     esi
40AC16  pop     ebx
40AC17  mov     esp, ebp
40AC19  pop     ebp
40AC1A  retn
40AC1B  push    ebx; jumptable 0040AAB2 cases 5,192
40AC1C  mov     ecx, esi
40AC1E  mov     eax, edi
40AC20  call    sub_40DE80
40AC25  mov     eax, [esp+24h+var_14]
40AC29  add     esp, 4
40AC2C  pop     edi
40AC2D  pop     esi
40AC2E  pop     ebx
40AC2F  mov     esp, ebp
40AC31  pop     ebp
40AC32  retn
40AC33  mov     ecx, esi; jumptable 0040AAB2 case 193
40AC35  push    ebx
40AC36  mov     esi, 0FFFFFFh
40AC3B  mov     eax, edi
40AC3D  call    sub_4115B0
40AC42  mov     eax, [esp+24h+var_14]
40AC46  add     esp, 4
40AC49  pop     edi
40AC4A  pop     esi
40AC4B  pop     ebx
40AC4C  mov     esp, ebp
40AC4E  pop     ebp
40AC4F  retn
40AC50  mov     ecx, esi; jumptable 0040AAB2 case 240
40AC52  mov     eax, edi
40AC54  mov     esi, ebx
40AC56  call    sub_40BF60
40AC5B  mov     eax, [esp+20h+var_14]
40AC5F  pop     edi
40AC60  pop     esi
40AC61  pop     ebx
40AC62  mov     esp, ebp
40AC64  pop     ebp
40AC65  retn
40AC66  push    ebx
40AC67  mov     edx, esi
40AC69  mov     eax, edi
40AC6B  call    sub_414190
40AC70  mov     eax, [esp+24h+var_14]
40AC74  add     esp, 4
40AC77  pop     edi
40AC78  pop     esi
40AC79  pop     ebx
40AC7A  mov     esp, ebp
40AC7C  pop     ebp
40AC7D  retn
40AC7E  mov     [esp+20h+var_14], 2; jumptable 0040AAB2 default case, cases 10-31,40-63,66-127,129-191,194-239
40AC86  mov     eax, [esp+20h+var_14]
40AC8A  pop     edi
40AC8B  pop     esi
40AC8C  pop     ebx
40AC8D  mov     esp, ebp
40AC8F  pop     ebp
40AC90  retn
40AC91  mov     [esp+20h+var_14], 3
40AC99  mov     eax, [esp+20h+var_14]
40AC9D  pop     edi
40AC9E  pop     esi
40AC9F  pop     ebx
40ACA0  mov     esp, ebp
40ACA2  pop     ebp
40ACA3  retn
40ACA4  mov     [esp+20h+var_14], 1
40ACAC  mov     eax, [esp+20h+var_14]
40ACB0  pop     edi
40ACB1  pop     esi
40ACB2  pop     ebx
40ACB3  mov     esp, ebp
40ACB5  pop     ebp
40ACB6  retn

;==== 40AE10 sub_40AE10
40AE10  push    ebp
40AE11  mov     ebp, esp
40AE13  sub     esp, 14h
40AE16  mov     ecx, [edx+4]
40AE19  push    esi
40AE1A  mov     esi, [edx]
40AE1C  push    edi
40AE1D  mov     edi, [eax]
40AE1F  mov     eax, [eax+4]
40AE22  mov     [ebp+var_8], eax
40AE25  mov     eax, [edx+14h]
40AE28  imul    eax, [edx+8]
40AE2C  mov     [ebp+var_C], ecx
40AE2F  mov     ecx, [edx+0Ch]
40AE32  mov     [ebp+var_10], edi
40AE35  mov     [ebp+var_14], eax
40AE38  test    al, 0Fh
40AE3A  jnz     loc_40AEEE
40AE40  mov     edx, [ebp+var_C]
40AE43  or      edx, [ebp+var_8]
40AE46  shr     eax, 4
40AE49  or      edx, esi
40AE4B  or      edx, edi
40AE4D  mov     [ebp+var_4], eax
40AE50  test    dl, 0Fh
40AE53  jnz     short loc_40AE9A
40AE55  mov     eax, ecx
40AE57  test    ecx, ecx
40AE59  jz      loc_40AF62
40AE5F  mov     ecx, [ebp+var_4]
40AE62  dec     eax
40AE63  mov     [ebp+var_10], eax
40AE66  mov     eax, esi
40AE68  test    ecx, ecx
40AE6A  jz      short loc_40AE87
40AE6C  mov     edx, edi
40AE6E  sub     edx, esi
40AE70  prefetchnta byte ptr [eax+200h]
40AE77  movaps  xmm0, xmmword ptr [eax]
40AE7A  movntps xmmword ptr [edx+eax], xmm0
40AE7E  add     eax, 10h
40AE81  dec     ecx
40AE82  jnz     short loc_40AE70
40AE84  mov     ecx, [ebp+var_4]
40AE87  mov     eax, [ebp+var_10]
40AE8A  add     edi, [ebp+var_8]
40AE8D  add     esi, [ebp+var_C]
40AE90  test    eax, eax
40AE92  jnz     short loc_40AE62
40AE94  pop     edi
40AE95  pop     esi
40AE96  mov     esp, ebp
40AE98  pop     ebp
40AE99  retn
40AE9A  test    ecx, ecx
40AE9C  jz      short loc_40AEE6
40AE9E  mov     edx, eax
40AEA0  dec     ecx
40AEA1  mov     [ebp+var_14], ecx
40AEA4  test    edx, edx
40AEA6  jz      short loc_40AED9
40AEA8  mov     eax, edi
40AEAA  mov     edi, esi
40AEAC  sub     edi, [ebp+var_10]
40AEAF  lea     ecx, [esi+8]
40AEB2  movq    mm0, qword ptr [edi+eax]
40AEB6  prefetchnta byte ptr [ecx+1F8h]
40AEBD  movntq  qword ptr [eax], mm0
40AEC0  movq    mm0, qword ptr [ecx]
40AEC3  movntq  qword ptr [eax+8], mm0
40AEC7  add     eax, 10h
40AECA  add     ecx, 10h
40AECD  dec     edx
40AECE  jnz     short loc_40AEB2
40AED0  mov     edi, [ebp+var_10]
40AED3  mov     ecx, [ebp+var_14]
40AED6  mov     edx, [ebp+var_4]
40AED9  add     edi, [ebp+var_8]
40AEDC  add     esi, [ebp+var_C]
40AEDF  mov     [ebp+var_10], edi
40AEE2  test    ecx, ecx
40AEE4  jnz     short loc_40AEA0
40AEE6  emms
40AEE8  pop     edi
40AEE9  pop     esi
40AEEA  mov     esp, ebp
40AEEC  pop     ebp
40AEED  retn
40AEEE  test    al, 7
40AEF0  jnz     short loc_40AF3C
40AEF2  shr     eax, 3
40AEF5  mov     edx, eax
40AEF7  mov     [ebp+var_4], edx
40AEFA  mov     eax, ecx
40AEFC  test    ecx, ecx
40AEFE  jz      short loc_40AF34
40AF00  dec     eax
40AF01  mov     [ebp+var_14], eax
40AF04  mov     eax, esi
40AF06  mov     ecx, edx
40AF08  test    edx, edx
40AF0A  jz      short loc_40AF27
40AF0C  mov     edx, edi
40AF0E  sub     edx, esi
40AF10  prefetchnta byte ptr [eax+200h]
40AF17  movq    mm0, qword ptr [eax]
40AF1A  movntq  qword ptr [edx+eax], mm0
40AF1E  add     eax, 8
40AF21  dec     ecx
40AF22  jnz     short loc_40AF10
40AF24  mov     edx, [ebp+var_4]
40AF27  mov     eax, [ebp+var_14]
40AF2A  add     edi, [ebp+var_8]
40AF2D  add     esi, [ebp+var_C]
40AF30  test    eax, eax
40AF32  jnz     short loc_40AF00
40AF34  emms
40AF36  pop     edi
40AF37  pop     esi
40AF38  mov     esp, ebp
40AF3A  pop     ebp
40AF3B  retn
40AF3C  mov     [ebp+var_10], ecx
40AF3F  test    ecx, ecx
40AF41  jz      short loc_40AF62
40AF43  jmp     short loc_40AF48
40AF45  mov     eax, [ebp+var_14]
40AF48  dec     [ebp+var_10]
40AF4B  push    eax; Size
40AF4C  push    esi; Src
40AF4D  push    edi; void *
40AF4E  call    _memcpy_0
40AF53  add     edi, [ebp+var_8]
40AF56  add     esi, [ebp+var_C]
40AF59  add     esp, 0Ch
40AF5C  cmp     [ebp+var_10], 0
40AF60  jnz     short loc_40AF45
40AF62  pop     edi
40AF63  pop     esi
40AF64  mov     esp, ebp
40AF66  pop     ebp
40AF67  retn

;==== 40AF70 sub_40AF70
40AF70  push    esi
40AF71  mov     esi, [eax+10h]
40AF74  cmp     [ecx+10h], esi
40AF77  jnz     short loc_40AF83
40AF79  mov     edx, eax
40AF7B  mov     eax, ecx
40AF7D  pop     esi
40AF7E  jmp     sub_40AE10
40AF83  dec     esi
40AF84  jz      short loc_40AF8F
40AF86  dec     esi
40AF87  jnz     short loc_40AF95
40AF89  pop     esi
40AF8A  jmp     loc_40B010
40AF8F  pop     esi
40AF90  jmp     loc_40AFA0
40AF95  pop     esi
40AF96  retn
40AFA0  push    ebp
40AFA1  mov     ebp, esp
40AFA3  sub     esp, 10h
40AFA6  mov     edx, [eax]
40AFA8  push    esi
40AFA9  mov     esi, [ecx]
40AFAB  mov     ecx, [ecx+4]
40AFAE  mov     [ebp-0Ch], ecx
40AFB1  mov     ecx, [eax+4]
40AFB4  push    edi
40AFB5  mov     edi, [eax+0Ch]
40AFB8  mov     [ebp-10h], ecx
40AFBB  mov     ecx, [eax+8]
40AFBE  mov     [ebp-4], edx
40AFC1  mov     [ebp-8], ecx
40AFC4  test    edi, edi
40AFC6  jz      short loc_40B005
40AFC8  push    ebx
40AFC9  lea     esp, [esp+0]
40AFD0  dec     edi
40AFD1  mov     eax, esi
40AFD3  test    ecx, ecx
40AFD5  jz      short loc_40AFF4
40AFD7  sub     edx, esi
40AFD9  lea     esp, [esp+0]
40AFE0  mov     ebx, [edx+eax]
40AFE3  or      ebx, 0FF000000h
40AFE9  mov     [eax], ebx
40AFEB  add     eax, 4
40AFEE  dec     ecx
40AFEF  jnz     short loc_40AFE0
40AFF1  mov     ecx, [ebp-8]
40AFF4  mov     edx, [ebp-4]
40AFF7  add     edx, [ebp-10h]
40AFFA  add     esi, [ebp-0Ch]
40AFFD  mov     [ebp-4], edx
40B000  test    edi, edi
40B002  jnz     short loc_40AFD0
40B004  pop     ebx
40B005  pop     edi
40B006  pop     esi
40B007  mov     esp, ebp
40B009  pop     ebp
40B00A  retn
40B010  push    ebp
40B011  mov     ebp, esp
40B013  sub     esp, 18h
40B016  mov     edx, [eax]
40B018  push    esi
40B019  mov     esi, [eax+0Ch]
40B01C  push    edi
40B01D  mov     edi, [ecx]
40B01F  mov     ecx, [ecx+4]
40B022  mov     [ebp-14h], ecx
40B025  mov     ecx, [eax+4]
40B028  mov     [ebp-18h], ecx
40B02B  mov     ecx, [eax+8]
40B02E  mov     [ebp-4], edi
40B031  mov     [ebp-8], edx
40B034  pxor    xmm1, xmm1
40B038  test    esi, esi
40B03A  jz      loc_40B0F4
40B040  mov     eax, ecx
40B042  shr     eax, 1
40B044  and     ecx, 1
40B047  mov     [ebp-0Ch], eax
40B04A  mov     [ebp-10h], ecx
40B04D  dec     esi
40B04E  mov     ecx, edi
40B050  test    eax, eax
40B052  jz      short loc_40B0A7
40B054  movq    xmm0, qword ptr [edx]
40B058  punpcklbw xmm0, xmm1
40B05C  pextrw  edi, xmm0, 3
40B061  sar     edi, 1
40B063  shl     edi, 4
40B066  movq    xmm3, qword_50B0F0[edi]
40B06E  pextrw  edi, xmm0, 7
40B073  sar     edi, 1
40B075  shl     edi, 4
40B078  add     edi, offset qword_50B0F0
40B07E  movdqa  xmm2, xmm0
40B082  movdqa  xmm0, xmmword ptr [edi]
40B086  punpcklqdq xmm3, xmm0
40B08A  pmullw  xmm2, xmm3
40B08E  psrlw   xmm2, 7
40B093  packuswb xmm2, xmm1
40B097  movq    qword ptr [ecx], xmm2
40B09B  add     ecx, 8
40B09E  add     edx, 8
40B0A1  dec     eax
40B0A2  jnz     short loc_40B054
40B0A4  mov     eax, [ebp-0Ch]
40B0A7  cmp     dword ptr [ebp-10h], 0
40B0AB  jz      short loc_40B0DA
40B0AD  mov     edx, [edx]
40B0AF  movd    xmm0, edx
40B0B3  shr     edx, 19h
40B0B6  shl     edx, 4
40B0B9  add     edx, offset qword_50B0F0
40B0BF  movdqa  xmm2, xmmword ptr [edx]
40B0C3  punpcklbw xmm0, xmm1
40B0C7  pmullw  xmm0, xmm2
40B0CB  psrlw   xmm0, 7
40B0D0  packuswb xmm0, xmm1
40B0D4  movd    edx, xmm0
40B0D8  mov     [ecx], edx
40B0DA  mov     edi, [ebp-4]
40B0DD  mov     edx, [ebp-8]
40B0E0  add     edi, [ebp-14h]
40B0E3  add     edx, [ebp-18h]
40B0E6  mov     [ebp-4], edi
40B0E9  mov     [ebp-8], edx
40B0EC  test    esi, esi
40B0EE  jnz     loc_40B04D
40B0F4  pop     edi
40B0F5  pop     esi
40B0F6  mov     esp, ebp
40B0F8  pop     ebp
40B0F9  retn

;==== 40B100 sub_40B100
40B100  push    esi
40B101  mov     esi, [eax+10h]
40B104  sub     esi, 0
40B107  jz      short loc_40B13D
40B109  dec     esi
40B10A  jz      short loc_40B124
40B10C  dec     esi
40B10D  jnz     short loc_40B147
40B10F  mov     esi, [ecx+10h]
40B112  dec     esi
40B113  jz      short loc_40B11E
40B115  dec     esi
40B116  jnz     short loc_40B147
40B118  pop     esi
40B119  jmp     sub_40B330
40B11E  pop     esi
40B11F  jmp     sub_40B1B0
40B124  mov     esi, [ecx+10h]
40B127  dec     esi
40B128  jz      short loc_40B133
40B12A  dec     esi
40B12B  jnz     short loc_40B147
40B12D  pop     esi
40B12E  jmp     loc_40AFA0
40B133  mov     edx, eax
40B135  mov     eax, ecx
40B137  pop     esi
40B138  jmp     sub_40AE10
40B13D  push    eax
40B13E  push    ecx
40B13F  call    sub_40B150
40B144  add     esp, 8
40B147  pop     esi
40B148  retn

;==== 40B150 sub_40B150
40B150  push    ebp
40B151  mov     ebp, esp
40B153  push    ecx
40B154  mov     eax, [ebp+arg_0]
40B157  mov     ecx, [ebp+arg_4]
40B15A  push    ebx
40B15B  mov     ebx, [eax]
40B15D  mov     eax, [ecx+0Ch]
40B160  push    esi
40B161  mov     esi, [ecx]
40B163  test    eax, eax
40B165  jz      short loc_40B1A9
40B167  push    edi
40B168  mov     edx, [ecx+8]
40B16B  dec     eax
40B16C  mov     [ebp+var_4], eax
40B16F  test    edx, edx
40B171  jz      short loc_40B19B
40B173  mov     edi, ebx
40B175  lea     eax, [esi+edx*2]
40B178  sub     edi, esi
40B17A  lea     ebx, [ebx+0]
40B180  movzx   ecx, word ptr [eax-2]
40B184  sub     eax, 2
40B187  dec     edx
40B188  test    cx, cx
40B18B  jz      short loc_40B191
40B18D  mov     [edi+eax], cx
40B191  test    edx, edx
40B193  jnz     short loc_40B180
40B195  mov     eax, [ebp+var_4]
40B198  mov     ecx, [ebp+arg_4]
40B19B  mov     edx, [ebp+arg_0]
40B19E  add     ebx, [edx+4]
40B1A1  add     esi, [ecx+4]
40B1A4  test    eax, eax
40B1A6  jnz     short loc_40B168
40B1A8  pop     edi
40B1A9  pop     esi
40B1AA  pop     ebx
40B1AB  mov     esp, ebp
40B1AD  pop     ebp
40B1AE  retn

;==== 40B1B0 sub_40B1B0
40B1B0  push    ebp
40B1B1  mov     ebp, esp
40B1B3  sub     esp, 18h
40B1B6  mov     edx, [ecx]
40B1B8  mov     ecx, [ecx+4]
40B1BB  movdqa  xmm3, ds:xmmword_4E4300
40B1C3  mov     [ebp+var_4], edx
40B1C6  mov     edx, [eax]
40B1C8  mov     [ebp+var_8], edx
40B1CB  mov     edx, [eax+4]
40B1CE  mov     [ebp+var_18], edx
40B1D1  mov     edx, [eax+0Ch]
40B1D4  push    esi
40B1D5  mov     [ebp+var_14], ecx
40B1D8  mov     ecx, [eax+8]
40B1DB  push    edi
40B1DC  pxor    xmm2, xmm2
40B1E0  test    edx, edx
40B1E2  jz      loc_40B325
40B1E8  mov     eax, ecx
40B1EA  shr     eax, 1
40B1EC  and     ecx, 1
40B1EF  mov     [ebp+var_C], eax
40B1F2  mov     [ebp+var_10], ecx
40B1F5  jmp     short loc_40B1FA
40B1F7  mov     eax, [ebp+var_C]
40B1FA  mov     esi, [ebp+var_4]
40B1FD  mov     edi, [ebp+var_8]
40B200  dec     edx
40B201  test    eax, eax
40B203  jz      loc_40B2AF
40B209  movq    xmm0, qword ptr [edi]
40B20D  movdqa  xmm1, xmm0
40B211  pcmpeqb xmm1, xmm2
40B215  pmovmskb ecx, xmm1
40B219  and     ecx, 88h
40B21F  dec     eax
40B220  cmp     cl, 88h
40B223  jz      short loc_40B2A1
40B225  movdqa  xmm1, xmm3
40B229  pand    xmm1, xmm0
40B22D  pcmpeqb xmm1, xmm3
40B231  pmovmskb ecx, xmm1
40B235  and     ecx, 88h
40B23B  cmp     cl, 88h
40B23E  jnz     short loc_40B246
40B240  movq    qword ptr [esi], xmm0
40B244  jmp     short loc_40B2A1
40B246  punpcklbw xmm0, xmm2
40B24A  movdqa  xmm1, xmm0
40B24E  movq    xmm0, qword ptr [esi]
40B252  pextrw  ecx, xmm1, 3
40B257  sar     ecx, 1
40B259  shl     ecx, 4
40B25C  punpcklbw xmm0, xmm2
40B260  movdqa  xmm4, xmm0
40B264  movdqa  xmm5, xmm1
40B268  psubw   xmm5, xmm0
40B26C  movq    xmm0, qword_50B0F0[ecx]
40B274  pextrw  ecx, xmm1, 7
40B279  sar     ecx, 1
40B27B  shl     ecx, 4
40B27E  add     ecx, offset qword_50B0F0
40B284  movdqa  xmm1, xmmword ptr [ecx]
40B288  punpcklqdq xmm0, xmm1
40B28C  pmullw  xmm5, xmm0
40B290  psraw   xmm5, 7
40B295  paddw   xmm4, xmm5
40B299  packuswb xmm4, xmm2
40B29D  movq    qword ptr [esi], xmm4
40B2A1  add     esi, 8
40B2A4  add     edi, 8
40B2A7  test    eax, eax
40B2A9  jnz     loc_40B209
40B2AF  cmp     [ebp+var_10], 0
40B2B3  jz      short loc_40B311
40B2B5  mov     ecx, [edi]
40B2B7  test    ecx, 0FE000000h
40B2BD  jz      short loc_40B311
40B2BF  movzx   eax, byte ptr [edi+3]
40B2C3  shr     eax, 1
40B2C5  cmp     eax, 7Fh
40B2C8  jnz     short loc_40B2D4
40B2CA  and     ecx, 0FFFFFFh
40B2D0  mov     [esi], ecx
40B2D2  jmp     short loc_40B311
40B2D4  mov     edi, [esi]
40B2D6  shl     eax, 4
40B2D9  movd    xmm0, edi
40B2DD  punpcklbw xmm0, xmm2
40B2E1  movd    xmm4, ecx
40B2E5  punpcklbw xmm4, xmm2
40B2E9  psubw   xmm4, xmm0
40B2ED  add     eax, offset qword_50B0F0
40B2F2  movdqa  xmm1, xmm0
40B2F6  movdqa  xmm0, xmmword ptr [eax]
40B2FA  pmullw  xmm4, xmm0
40B2FE  psraw   xmm4, 7
40B303  paddw   xmm1, xmm4
40B307  packuswb xmm1, xmm2
40B30B  movd    eax, xmm1
40B30F  mov     [esi], eax
40B311  mov     ecx, [ebp+var_14]
40B314  mov     eax, [ebp+var_18]
40B317  add     [ebp+var_4], ecx
40B31A  add     [ebp+var_8], eax
40B31D  test    edx, edx
40B31F  jnz     loc_40B1F7
40B325  pop     edi
40B326  pop     esi
40B327  mov     esp, ebp
40B329  pop     ebp
40B32A  retn

;==== 40B330 sub_40B330
40B330  push    ebp
40B331  mov     ebp, esp
40B333  sub     esp, 20h
40B336  mov     edx, [ecx]
40B338  mov     ecx, [ecx+4]
40B33B  mov     [ebp+var_4], edx
40B33E  mov     edx, [eax]
40B340  mov     [ebp+var_8], edx
40B343  mov     edx, [eax+4]
40B346  mov     [ebp+var_20], edx
40B349  mov     edx, [eax+0Ch]
40B34C  push    esi
40B34D  mov     [ebp+var_1C], ecx
40B350  mov     ecx, [eax+8]
40B353  push    edi
40B354  pxor    xmm5, xmm5
40B358  test    edx, edx
40B35A  jz      loc_40B552
40B360  mov     eax, ecx
40B362  shr     eax, 1
40B364  and     ecx, 1
40B367  mov     [ebp+var_C], eax
40B36A  mov     [ebp+var_10], ecx
40B36D  jmp     short loc_40B373
40B370  mov     eax, [ebp+var_C]
40B373  mov     esi, [ebp+var_4]
40B376  mov     edi, [ebp+var_8]
40B379  dec     edx
40B37A  mov     [ebp+var_18], edx
40B37D  test    eax, eax
40B37F  jz      loc_40B49E
40B385  movq    xmm0, qword ptr [edi]
40B389  movdqa  xmm1, xmm0
40B38D  pcmpeqb xmm1, xmm5
40B391  pmovmskb ecx, xmm1
40B395  and     ecx, 88h
40B39B  dec     eax
40B39C  cmp     cl, 88h
40B39F  jz      loc_40B490
40B3A5  movdqa  xmm1, ds:xmmword_4E42F0
40B3AD  movdqa  xmm2, xmm0
40B3B1  pcmpeqb xmm2, xmm1
40B3B5  pmovmskb ecx, xmm2
40B3B9  and     ecx, 88h
40B3BF  cmp     cl, 88h
40B3C2  jz      loc_40B48C
40B3C8  movq    xmm1, qword ptr [esi]
40B3CC  movaps  xmm4, ds:xmmword_4E42A0
40B3D3  punpcklbw xmm0, xmm5
40B3D7  movdqa  xmm2, xmm0
40B3DB  punpcklbw xmm1, xmm5
40B3DF  movdqa  xmm3, xmm1
40B3E3  psrlq   xmm3, 30h ; '0'
40B3E8  cvtdq2ps xmm3, xmm3
40B3EB  mulps   xmm3, ds:xmmword_4E42B0
40B3F2  psrlq   xmm2, 30h ; '0'
40B3F7  cvtdq2ps xmm2, xmm2
40B3FA  subps   xmm4, xmm2
40B3FD  mulps   xmm3, xmm4
40B400  movaps  xmm4, xmm3
40B403  addps   xmm4, xmm2
40B406  shufps  xmm2, xmm3, 88h
40B40A  rcpps   xmm6, xmm4
40B40D  shufps  xmm6, xmm6, 88h
40B411  mulps   xmm6, xmm2
40B414  mulps   xmm6, ds:xmmword_4E42A0
40B41B  cvttps2dq xmm2, xmm6
40B41F  movd    ecx, xmm2
40B423  shl     ecx, 4
40B426  movq    xmm3, qword_50B900[ecx]
40B42E  pextrw  ecx, xmm2, 2
40B433  shl     ecx, 4
40B436  add     ecx, offset qword_50B900
40B43C  movdqa  xmm6, xmmword ptr [ecx]
40B440  punpcklqdq xmm3, xmm6
40B444  pmullw  xmm0, xmm3
40B448  pextrw  ecx, xmm2, 4
40B44D  shl     ecx, 4
40B450  movq    xmm3, qword_50B900[ecx]
40B458  pextrw  ecx, xmm2, 6
40B45D  shl     ecx, 4
40B460  add     ecx, offset qword_50B900
40B466  movdqa  xmm2, xmmword ptr [ecx]
40B46A  punpcklqdq xmm3, xmm2
40B46E  pmullw  xmm1, xmm3
40B472  paddw   xmm0, xmm1
40B476  psrlw   xmm0, 8
40B47B  cvttps2dq xmm1, xmm4
40B47F  psllq   xmm1, 30h ; '0'
40B484  por     xmm0, xmm1
40B488  packuswb xmm0, xmm5
40B48C  movq    qword ptr [esi], xmm0
40B490  add     esi, 8
40B493  add     edi, 8
40B496  test    eax, eax
40B498  jnz     loc_40B385
40B49E  cmp     [ebp+var_10], 0
40B4A2  jz      loc_40B53E
40B4A8  mov     eax, [edi]
40B4AA  mov     [ebp+var_14], eax
40B4AD  test    eax, 0FF000000h
40B4B2  jz      loc_40B53E
40B4B8  mov     edi, eax
40B4BA  shr     edi, 18h
40B4BD  cmp     edi, 0FFh
40B4C3  jz      short loc_40B53C
40B4C5  movzx   eax, byte ptr [esi+3]
40B4C9  mov     edx, 100h
40B4CE  sub     edx, edi
40B4D0  imul    eax, edx
40B4D3  mov     edx, [esi]
40B4D5  movd    xmm0, edx
40B4D9  mov     ecx, edi
40B4DB  shl     ecx, 8
40B4DE  add     ecx, eax
40B4E0  shl     eax, 8
40B4E3  xor     edx, edx
40B4E5  div     ecx
40B4E7  xor     edx, edx
40B4E9  punpcklbw xmm0, xmm5
40B4ED  shl     eax, 4
40B4F0  add     eax, offset qword_50B900
40B4F5  movdqa  xmm1, xmmword ptr [eax]
40B4F9  mov     eax, edi
40B4FB  shl     eax, 10h
40B4FE  div     ecx
40B500  mov     edx, [ebp+var_18]
40B503  pmullw  xmm0, xmm1
40B507  movd    xmm1, [ebp+var_14]
40B50C  punpcklbw xmm1, xmm5
40B510  and     ecx, 0FFFFFF00h
40B516  shl     ecx, 10h
40B519  shl     eax, 4
40B51C  add     eax, offset qword_50B900
40B521  movdqa  xmm2, xmmword ptr [eax]
40B525  pmullw  xmm1, xmm2
40B529  paddw   xmm0, xmm1
40B52D  psrlw   xmm0, 8
40B532  packuswb xmm0, xmm5
40B536  movd    eax, xmm0
40B53A  or      eax, ecx
40B53C  mov     [esi], eax
40B53E  mov     ecx, [ebp+var_1C]
40B541  mov     eax, [ebp+var_20]
40B544  add     [ebp+var_4], ecx
40B547  add     [ebp+var_8], eax
40B54A  test    edx, edx
40B54C  jnz     loc_40B370
40B552  pop     edi
40B553  pop     esi
40B554  mov     esp, ebp
40B556  pop     ebp
40B557  retn

;==== 40B560 sub_40B560
40B560  push    ebp
40B561  mov     ebp, esp
40B563  mov     edx, [ebp+arg_0]
40B566  push    esi
40B567  test    ecx, ecx
40B569  jz      short loc_40B5E0
40B56B  cmp     ecx, 100h
40B571  jnb     short loc_40B595
40B573  mov     esi, [eax+10h]
40B576  sub     esi, 0
40B579  jz      short loc_40B5CC
40B57B  dec     esi
40B57C  jz      short loc_40B5A7
40B57E  dec     esi
40B57F  jnz     short loc_40B595
40B581  mov     esi, [edx+10h]
40B584  dec     esi
40B585  jz      short loc_40B598
40B587  dec     esi
40B588  jnz     short loc_40B595
40B58A  push    ecx
40B58B  mov     ecx, edx
40B58D  call    sub_40BD00
40B592  add     esp, 4
40B595  pop     esi
40B596  pop     ebp
40B597  retn
40B598  push    ecx
40B599  push    eax
40B59A  mov     ecx, edx
40B59C  call    sub_40B900
40B5A1  add     esp, 8
40B5A4  pop     esi
40B5A5  pop     ebp
40B5A6  retn
40B5A7  mov     esi, [edx+10h]
40B5AA  dec     esi
40B5AB  jz      short loc_40B5C0
40B5AD  dec     esi
40B5AE  jnz     short loc_40B595
40B5B0  push    ecx
40B5B1  mov     ecx, eax
40B5B3  mov     eax, edx
40B5B5  call    sub_40B7E0
40B5BA  add     esp, 4
40B5BD  pop     esi
40B5BE  pop     ebp
40B5BF  retn
40B5C0  push    edx
40B5C1  call    sub_40B6F0
40B5C6  add     esp, 4
40B5C9  pop     esi
40B5CA  pop     ebp
40B5CB  retn
40B5CC  cmp     dword ptr [edx+10h], 0
40B5D0  jnz     short loc_40B595
40B5D2  push    ecx
40B5D3  push    eax
40B5D4  push    edx
40B5D5  call    sub_40B5F0
40B5DA  add     esp, 0Ch
40B5DD  pop     esi
40B5DE  pop     ebp
40B5DF  retn
40B5E0  mov     ecx, edx
40B5E2  pop     esi
40B5E3  pop     ebp
40B5E4  jmp     sub_40B100

;==== 40B5F0 sub_40B5F0
40B5F0  push    ebp
40B5F1  mov     ebp, esp
40B5F3  sub     esp, 18h
40B5F6  mov     eax, [ebp+arg_0]
40B5F9  mov     edx, [eax]
40B5FB  push    ebx
40B5FC  mov     ebx, [ebp+arg_4]
40B5FF  mov     ecx, [ebx+0Ch]
40B602  mov     eax, [ebx]
40B604  push    esi
40B605  push    edi
40B606  mov     edi, [ebp+arg_8]
40B609  mov     esi, 100h
40B60E  sub     esi, edi
40B610  mov     [ebp+var_4], edx
40B613  mov     [ebp+var_10], eax
40B616  mov     [ebp+var_C], esi
40B619  mov     [ebp+var_8], ecx
40B61C  test    ecx, ecx
40B61E  jz      loc_40B6E7
40B624  mov     ecx, [ebx+8]
40B627  dec     [ebp+var_8]
40B62A  test    ecx, ecx
40B62C  jz      loc_40B6CE
40B632  sub     eax, [ebp+var_4]
40B635  lea     edx, [edx+ecx*2]
40B638  mov     [ebp+var_18], eax
40B63B  jmp     short loc_40B640
40B640  dec     ecx
40B641  sub     edx, 2
40B644  cmp     word ptr [eax+edx], 0
40B649  mov     [ebp+var_14], ecx
40B64C  jbe     short loc_40B6C0
40B64E  movzx   eax, word ptr [eax+edx]
40B652  movzx   ecx, word ptr [edx]
40B655  mov     ebx, eax
40B657  and     ebx, 3E0h
40B65D  imul    ebx, esi
40B660  mov     esi, ecx
40B662  and     esi, 3E0h
40B668  imul    esi, edi
40B66B  add     ebx, esi
40B66D  mov     esi, eax
40B66F  and     esi, 7C00h
40B675  imul    esi, [ebp+var_C]
40B679  mov     edi, ecx
40B67B  and     edi, 7C00h
40B681  imul    edi, [ebp+arg_8]
40B685  add     esi, edi
40B687  mov     edi, [ebp+arg_8]
40B68A  shr     ebx, 8
40B68D  shr     esi, 8
40B690  and     ebx, 3E0h
40B696  and     esi, 7C00h
40B69C  and     eax, 1Fh
40B69F  add     ebx, esi
40B6A1  mov     esi, [ebp+var_C]
40B6A4  and     ecx, 1Fh
40B6A7  imul    eax, esi
40B6AA  imul    ecx, edi
40B6AD  add     eax, ecx
40B6AF  mov     ecx, [ebp+var_14]
40B6B2  shr     eax, 8
40B6B5  add     ebx, eax
40B6B7  mov     eax, [ebp+var_18]
40B6BA  mov     [edx], bx
40B6BD  mov     ebx, [ebp+arg_4]
40B6C0  test    ecx, ecx
40B6C2  jnz     loc_40B640
40B6C8  mov     eax, [ebp+var_10]
40B6CB  mov     edx, [ebp+var_4]
40B6CE  mov     ecx, [ebp+arg_0]
40B6D1  add     edx, [ecx+4]
40B6D4  add     eax, [ebx+4]
40B6D7  cmp     [ebp+var_8], 0
40B6DB  mov     [ebp+var_4], edx
40B6DE  mov     [ebp+var_10], eax
40B6E1  jnz     loc_40B624
40B6E7  pop     edi
40B6E8  pop     esi
40B6E9  pop     ebx
40B6EA  mov     esp, ebp
40B6EC  pop     ebp
40B6ED  retn

;==== 40B6F0 sub_40B6F0
40B6F0  push    ebp
40B6F1  mov     ebp, esp
40B6F3  sub     esp, 10h
40B6F6  mov     edx, [ebp+arg_0]
40B6F9  push    esi
40B6FA  mov     esi, [eax]
40B6FC  shr     ecx, 1
40B6FE  push    edi
40B6FF  mov     edi, [edx]
40B701  mov     edx, [edx+4]
40B704  movsx   ecx, cx
40B707  mov     [ebp+var_C], edx
40B70A  mov     edx, [eax+4]
40B70D  mov     [ebp+arg_0], esi
40B710  mov     esi, [eax+0Ch]
40B713  movd    xmm0, ecx
40B717  mov     [ebp+var_10], edx
40B71A  mov     edx, [eax+8]
40B71D  punpcklwd xmm0, xmm0
40B721  pxor    xmm1, xmm1
40B725  pshufd  xmm2, xmm0, 0
40B72A  test    esi, esi
40B72C  jz      loc_40B7D0
40B732  mov     ecx, edx
40B734  shr     ecx, 1
40B736  and     edx, 1
40B739  mov     [ebp+var_4], ecx
40B73C  mov     [ebp+var_8], edx
40B73F  jmp     short loc_40B744
40B741  mov     ecx, [ebp+var_4]
40B744  mov     edx, [ebp+arg_0]
40B747  dec     esi
40B748  mov     eax, edi
40B74A  test    ecx, ecx
40B74C  jz      short loc_40B786
40B74E  mov     edi, edi
40B750  movq    xmm0, qword ptr [edx]
40B754  movq    xmm4, qword ptr [eax]
40B758  punpcklbw xmm4, xmm1
40B75C  punpcklbw xmm0, xmm1
40B760  psubw   xmm4, xmm0
40B764  pmullw  xmm4, xmm2
40B768  movdqa  xmm3, xmm0
40B76C  psraw   xmm4, 7
40B771  paddw   xmm3, xmm4
40B775  packuswb xmm3, xmm1
40B779  movq    qword ptr [eax], xmm3
40B77D  add     eax, 8
40B780  add     edx, 8
40B783  dec     ecx
40B784  jnz     short loc_40B750
40B786  cmp     [ebp+var_8], 0
40B78A  jz      short loc_40B7BF
40B78C  mov     edx, [edx]
40B78E  mov     ecx, [eax]
40B790  movd    xmm0, edx
40B794  punpcklbw xmm0, xmm1
40B798  movd    xmm4, ecx
40B79C  punpcklbw xmm4, xmm1
40B7A0  psubw   xmm4, xmm0
40B7A4  pmullw  xmm4, xmm2
40B7A8  movdqa  xmm3, xmm0
40B7AC  psraw   xmm4, 7
40B7B1  paddw   xmm3, xmm4
40B7B5  packuswb xmm3, xmm1
40B7B9  movd    edx, xmm3
40B7BD  mov     [eax], edx
40B7BF  mov     eax, [ebp+var_10]
40B7C2  add     edi, [ebp+var_C]
40B7C5  add     [ebp+arg_0], eax
40B7C8  test    esi, esi
40B7CA  jnz     loc_40B741
40B7D0  pop     edi
40B7D1  pop     esi
40B7D2  mov     esp, ebp
40B7D4  pop     ebp
40B7D5  retn

;==== 40B7E0 sub_40B7E0
40B7E0  push    ebp
40B7E1  mov     ebp, esp
40B7E3  sub     esp, 28h
40B7E6  push    esi
40B7E7  mov     esi, ecx
40B7E9  mov     ecx, [eax]
40B7EB  mov     eax, [eax+4]
40B7EE  mov     edx, [esi]
40B7F0  mov     [ebp+var_24], eax
40B7F3  mov     eax, [esi+4]
40B7F6  mov     [ebp+var_28], eax
40B7F9  push    edi
40B7FA  mov     edi, [esi+8]
40B7FD  mov     esi, [esi+0Ch]
40B800  mov     eax, 100h
40B805  sub     eax, [ebp+arg_0]
40B808  mov     [ebp+var_8], ecx
40B80B  mov     [ebp+var_C], edx
40B80E  mov     [ebp+var_20], edi
40B811  mov     [ebp+var_1C], eax
40B814  test    esi, esi
40B816  jz      loc_40B8F9
40B81C  push    ebx
40B81D  lea     ecx, [ecx+0]
40B820  dec     esi
40B821  mov     [ebp+var_18], esi
40B824  mov     [ebp+var_4], edi
40B827  test    edi, edi
40B829  jz      loc_40B8E4
40B82F  imul    eax, 0FFh
40B835  lea     esi, [ecx+3]
40B838  sub     ecx, edx
40B83A  mov     [ebp+var_10], eax
40B83D  lea     edi, [edx+2]
40B840  mov     [ebp+var_14], ecx
40B843  jmp     short loc_40B848
40B845  mov     eax, [ebp+var_10]
40B848  movzx   edx, byte ptr [esi]
40B84B  mov     ecx, 10000h
40B850  sub     ecx, eax
40B852  imul    ecx, edx
40B855  shr     ecx, 8
40B858  lea     edx, [ecx+eax]
40B85B  mov     [ebp+arg_0], edx
40B85E  xor     edx, edx
40B860  shl     eax, 10h
40B863  div     [ebp+arg_0]
40B866  xor     edx, edx
40B868  dec     [ebp+var_4]
40B86B  add     edi, 4
40B86E  add     esi, 4
40B871  mov     ebx, eax
40B873  mov     eax, ecx
40B875  shl     eax, 10h
40B878  div     [ebp+arg_0]
40B87B  movzx   ecx, byte ptr [edi-6]
40B87F  movzx   edx, byte ptr [esi-7]
40B883  imul    ecx, ebx
40B886  imul    edx, eax
40B889  add     ecx, edx
40B88B  movzx   edx, byte ptr [esi-6]
40B88F  shr     ecx, 10h
40B892  imul    edx, eax
40B895  mov     [esi-7], cl
40B898  movzx   ecx, byte ptr [edi-5]
40B89C  imul    ecx, ebx
40B89F  add     ecx, edx
40B8A1  shr     ecx, 10h
40B8A4  mov     [esi-6], cl
40B8A7  mov     ecx, [ebp+var_14]
40B8AA  movzx   edx, byte ptr [ecx+edi-4]
40B8AF  imul    edx, eax
40B8B2  movzx   eax, byte ptr [edi-4]
40B8B6  imul    eax, ebx
40B8B9  add     edx, eax
40B8BB  shr     edx, 10h
40B8BE  mov     [ecx+edi-4], dl
40B8C2  mov     ecx, [ebp+arg_0]
40B8C5  shr     ecx, 8
40B8C8  cmp     [ebp+var_4], 0
40B8CC  mov     [esi-4], cl
40B8CF  jnz     loc_40B845
40B8D5  mov     esi, [ebp+var_18]
40B8D8  mov     eax, [ebp+var_1C]
40B8DB  mov     edi, [ebp+var_20]
40B8DE  mov     ecx, [ebp+var_8]
40B8E1  mov     edx, [ebp+var_C]
40B8E4  add     ecx, [ebp+var_24]
40B8E7  add     edx, [ebp+var_28]
40B8EA  mov     [ebp+var_8], ecx
40B8ED  mov     [ebp+var_C], edx
40B8F0  test    esi, esi
40B8F2  jnz     loc_40B820
40B8F8  pop     ebx
40B8F9  pop     edi
40B8FA  pop     esi
40B8FB  mov     esp, ebp
40B8FD  pop     ebp
40B8FE  retn

;==== 40B900 sub_40B900
40B900  push    ebx
40B901  mov     ebx, esp
40B903  sub     esp, 8
40B906  and     esp, 0FFFFFFF0h
40B909  add     esp, 4
40B90C  push    ebp
40B90D  mov     ebp, [ebx+4]
40B910  mov     [esp+0Ch+var_8], ebp
40B914  mov     ebp, esp
40B916  sub     esp, 858h
40B91C  mov     eax, dword_4FB734
40B921  xor     eax, ebp
40B923  mov     [ebp-4], eax
40B926  push    esi
40B927  push    edi
40B928  mov     edx, [ecx]
40B92A  mov     eax, [ebx+8]
40B92D  mov     ecx, [ecx+4]
40B930  mov     [ebp-824h], edx
40B936  mov     edx, [eax]
40B938  mov     [ebp-818h], edx
40B93E  mov     edx, [eax+4]
40B941  mov     [ebp-834h], ecx
40B947  mov     ecx, [eax+0Ch]
40B94A  mov     [ebp-828h], edx
40B950  mov     edx, [eax+8]
40B953  mov     eax, 100h
40B958  sub     eax, [ebx+0Ch]
40B95B  mov     [ebp-838h], ecx
40B961  mov     [ebp-820h], eax
40B967  xor     eax, eax
40B969  lea     ecx, [ebp-810h]
40B96F  pxor    xmm0, xmm0
40B973  mov     [ebp-82Ch], edx
40B979  movdqa  xmmword ptr [ebp-850h], xmm0
40B981  mov     [ebp-814h], eax
40B987  mov     [ebp-81Ch], ecx
40B98D  mov     dword ptr [ebp-830h], 80h
40B997  jmp     short loc_40B9A0
40B9A0  shr     eax, 8
40B9A3  mov     [ebp-83Ch], eax
40B9A9  xor     eax, eax
40B9AB  movzx   eax, ax
40B9AE  xor     edx, edx
40B9B0  movd    xmm1, eax
40B9B4  movzx   edx, dx
40B9B7  xor     ecx, ecx
40B9B9  xor     edi, edi
40B9BB  xor     esi, esi
40B9BD  movd    xmm0, edx
40B9C1  movzx   ecx, cx
40B9C4  movzx   eax, di
40B9C7  movd    xmm4, eax
40B9CB  mov     eax, [ebp-83Ch]
40B9D1  movzx   edx, si
40B9D4  movd    xmm2, ecx
40B9D8  movzx   ecx, ax
40B9DB  movd    xmm3, edx
40B9DF  movzx   edx, ax
40B9E2  movzx   eax, ax
40B9E5  movd    xmm5, ecx
40B9E9  mov     ecx, [ebp-81Ch]
40B9EF  movd    xmm7, eax
40B9F3  mov     eax, [ebp-814h]
40B9F9  add     eax, [ebp-820h]
40B9FF  movd    xmm6, edx
40BA03  punpcklwd xmm4, xmm0
40BA07  add     ecx, 10h
40BA0A  dec     dword ptr [ebp-830h]
40BA10  punpcklwd xmm7, xmm3
40BA14  punpcklwd xmm5, xmm1
40BA18  punpcklwd xmm6, xmm2
40BA1C  punpcklwd xmm7, xmm5
40BA20  punpcklwd xmm6, xmm4
40BA24  punpcklwd xmm7, xmm6
40BA28  movdqa  xmmword ptr [ecx-10h], xmm7
40BA2D  mov     [ebp-814h], eax
40BA33  mov     [ebp-81Ch], ecx
40BA39  jnz     loc_40B9A0
40BA3F  call    sub_407AD0
40BA44  test    eax, eax
40BA46  mov     eax, [ebp-838h]
40BA4C  mov     edx, eax
40BA4E  jz      loc_40BBB2
40BA54  test    eax, eax
40BA56  jz      loc_40BCEB
40BA5C  mov     eax, [ebp-82Ch]
40BA62  movdqa  xmm1, xmmword ptr [ebp-850h]
40BA6A  mov     ecx, eax
40BA6C  shr     ecx, 1
40BA6E  and     eax, 1
40BA71  mov     [ebp-81Ch], ecx
40BA77  mov     ecx, [ebp-824h]
40BA7D  mov     [ebp-814h], eax
40BA83  mov     eax, [ebp-81Ch]
40BA89  mov     edi, [ebp-818h]
40BA8F  dec     edx
40BA90  mov     [ebp-820h], edx
40BA96  mov     esi, ecx
40BA98  test    eax, eax
40BA9A  jz      loc_40BB2C
40BAA0  movq    xmm2, qword ptr [edi]
40BAA4  movdqa  xmm0, xmm2
40BAA8  pcmpeqb xmm0, xmm1
40BAAC  pmovmskb edx, xmm0
40BAB0  and     edx, 88h
40BAB6  dec     eax
40BAB7  cmp     dl, 88h
40BABA  jz      short loc_40BB18
40BABC  movq    xmm0, qword ptr [esi]
40BAC0  prefetchnta byte ptr [esi+80h]
40BAC7  punpcklbw xmm2, xmm1
40BACB  pextrw  edx, xmm2, 3
40BAD0  sar     edx, 1
40BAD2  punpcklbw xmm0, xmm1
40BAD6  add     edx, edx
40BAD8  movdqa  xmm3, xmm0
40BADC  movdqa  xmm4, xmm2
40BAE0  psubw   xmm4, xmm0
40BAE4  movq    xmm0, qword ptr [ebp+edx*8-810h]
40BAED  pextrw  edx, xmm2, 7
40BAF2  sar     edx, 1
40BAF4  add     edx, edx
40BAF6  movdqa  xmm2, xmmword ptr [ebp+edx*8-810h]
40BAFF  punpcklqdq xmm0, xmm2
40BB03  pmullw  xmm4, xmm0
40BB07  psraw   xmm4, 7
40BB0C  paddw   xmm3, xmm4
40BB10  packuswb xmm3, xmm1
40BB14  movq    qword ptr [esi], xmm3
40BB18  add     esi, 8
40BB1B  add     edi, 8
40BB1E  test    eax, eax
40BB20  jnz     loc_40BAA0
40BB26  mov     edx, [ebp-820h]
40BB2C  cmp     dword ptr [ebp-814h], 0
40BB33  jz      short loc_40BB85
40BB35  mov     edi, [edi]
40BB37  test    edi, 0FE000000h
40BB3D  jz      short loc_40BB85
40BB3F  mov     eax, [esi]
40BB41  prefetchnta byte ptr [esi+80h]
40BB48  movd    xmm3, edi
40BB4C  movd    xmm0, eax
40BB50  punpcklbw xmm0, xmm1
40BB54  punpcklbw xmm3, xmm1
40BB58  shr     edi, 19h
40BB5B  psubw   xmm3, xmm0
40BB5F  movdqa  xmm2, xmm0
40BB63  add     edi, edi
40BB65  movdqa  xmm0, xmmword ptr [ebp+edi*8-810h]
40BB6E  pmullw  xmm3, xmm0
40BB72  psraw   xmm3, 7
40BB77  paddw   xmm2, xmm3
40BB7B  packuswb xmm2, xmm1
40BB7F  movd    eax, xmm2
40BB83  mov     [esi], eax
40BB85  mov     eax, [ebp-828h]
40BB8B  add     ecx, [ebp-834h]
40BB91  add     [ebp-818h], eax
40BB97  test    edx, edx
40BB99  jnz     loc_40BA83
40BB9F  pop     edi
40BBA0  pop     esi
40BBA1  mov     ecx, [ebp-4]
40BBA4  xor     ecx, ebp
40BBA6  call    sub_4ABDD5
40BBAB  mov     esp, ebp
40BBAD  pop     ebp
40BBAE  mov     esp, ebx
40BBB0  pop     ebx
40BBB1  retn
40BBB2  test    eax, eax
40BBB4  jz      loc_40BCEB
40BBBA  mov     eax, [ebp-82Ch]
40BBC0  movdqa  xmm1, xmmword ptr [ebp-850h]
40BBC8  mov     ecx, eax
40BBCA  shr     ecx, 1
40BBCC  and     eax, 1
40BBCF  mov     [ebp-81Ch], ecx
40BBD5  mov     ecx, [ebp-824h]
40BBDB  mov     [ebp-814h], eax
40BBE1  mov     eax, [ebp-81Ch]
40BBE7  mov     esi, [ebp-818h]
40BBED  dec     edx
40BBEE  mov     [ebp-820h], edx
40BBF4  mov     edi, ecx
40BBF6  test    eax, eax
40BBF8  jz      loc_40BC7F
40BBFE  movq    xmm2, qword ptr [esi]
40BC02  movdqa  xmm0, xmm2
40BC06  pcmpeqb xmm0, xmm1
40BC0A  pmovmskb edx, xmm0
40BC0E  and     edx, 88h
40BC14  dec     eax
40BC15  cmp     dl, 88h
40BC18  jz      short loc_40BC6F
40BC1A  movq    xmm0, qword ptr [edi]
40BC1E  punpcklbw xmm2, xmm1
40BC22  pextrw  edx, xmm2, 3
40BC27  sar     edx, 1
40BC29  punpcklbw xmm0, xmm1
40BC2D  add     edx, edx
40BC2F  movdqa  xmm3, xmm0
40BC33  movdqa  xmm4, xmm2
40BC37  psubw   xmm4, xmm0
40BC3B  movq    xmm0, qword ptr [ebp+edx*8-810h]
40BC44  pextrw  edx, xmm2, 7
40BC49  sar     edx, 1
40BC4B  add     edx, edx
40BC4D  movdqa  xmm2, xmmword ptr [ebp+edx*8-810h]
40BC56  punpcklqdq xmm0, xmm2
40BC5A  pmullw  xmm4, xmm0
40BC5E  psraw   xmm4, 7
40BC63  paddw   xmm3, xmm4
40BC67  packuswb xmm3, xmm1
40BC6B  movq    qword ptr [edi], xmm3
40BC6F  add     edi, 8
40BC72  add     esi, 8
40BC75  test    eax, eax
40BC77  jnz     short loc_40BBFE
40BC79  mov     edx, [ebp-820h]
40BC7F  cmp     dword ptr [ebp-814h], 0
40BC86  jz      short loc_40BCD1
40BC88  mov     esi, [esi]
40BC8A  test    esi, 0FE000000h
40BC90  jz      short loc_40BCD1
40BC92  mov     eax, [edi]
40BC94  movd    xmm3, esi
40BC98  movd    xmm0, eax
40BC9C  punpcklbw xmm0, xmm1
40BCA0  punpcklbw xmm3, xmm1
40BCA4  shr     esi, 19h
40BCA7  psubw   xmm3, xmm0
40BCAB  movdqa  xmm2, xmm0
40BCAF  add     esi, esi
40BCB1  movdqa  xmm0, xmmword ptr [ebp+esi*8-810h]
40BCBA  pmullw  xmm3, xmm0
40BCBE  psraw   xmm3, 7
40BCC3  paddw   xmm2, xmm3
40BCC7  packuswb xmm2, xmm1
40BCCB  movd    eax, xmm2
40BCCF  mov     [edi], eax
40BCD1  mov     eax, [ebp-828h]
40BCD7  add     ecx, [ebp-834h]
40BCDD  add     [ebp-818h], eax
40BCE3  test    edx, edx
40BCE5  jnz     loc_40BBE1
40BCEB  mov     ecx, [ebp-4]
40BCEE  pop     edi
40BCEF  xor     ecx, ebp
40BCF1  pop     esi
40BCF2  call    sub_4ABDD5
40BCF7  mov     esp, ebp
40BCF9  pop     ebp
40BCFA  mov     esp, ebx
40BCFC  pop     ebx
40BCFD  retn

;==== 40BD00 sub_40BD00
40BD00  push    ebx
40BD01  mov     ebx, esp
40BD03  sub     esp, 8
40BD06  and     esp, 0FFFFFFF0h
40BD09  add     esp, 4
40BD0C  push    ebp
40BD0D  mov     ebp, [ebx+4]
40BD10  mov     [esp+0Ch+var_8], ebp
40BD14  mov     ebp, esp
40BD16  sub     esp, 48h
40BD19  mov     edx, [ecx]
40BD1B  mov     ecx, [ecx+4]
40BD1E  movaps  xmm3, ds:xmmword_4E42E0
40BD25  mov     [ebp-4], edx
40BD28  mov     edx, [eax]
40BD2A  mov     [ebp-20h], ecx
40BD2D  mov     ecx, 100h
40BD32  sub     ecx, [ebx+8]
40BD35  mov     [ebp-8], edx
40BD38  mov     edx, [eax+4]
40BD3B  push    esi
40BD3C  mov     [ebp-18h], ecx
40BD3F  fild    dword ptr [ebp-18h]
40BD42  mov     [ebp-24h], edx
40BD45  mov     edx, [eax+8]
40BD48  push    edi
40BD49  mov     [ebp-1Ch], ecx
40BD4C  pxor    xmm5, xmm5
40BD50  jns     short loc_40BD58
40BD52  fadd    ds:flt_4EC9C0
40BD58  fstp    dword ptr [ebp-0Ch]
40BD5B  movss   xmm0, dword ptr [ebp-0Ch]
40BD60  mov     eax, [eax+0Ch]
40BD63  shufps  xmm0, xmm0, 0
40BD67  mulps   xmm3, xmm0
40BD6A  movaps  xmmword ptr [ebp-40h], xmm3
40BD6E  mov     [ebp-0Ch], eax
40BD71  test    eax, eax
40BD73  jz      loc_40BF4D
40BD79  mov     esi, edx
40BD7B  shr     esi, 1
40BD7D  and     edx, 1
40BD80  mov     [ebp-10h], esi
40BD83  mov     [ebp-14h], edx
40BD86  jmp     short loc_40BD93
40BD88  jmp     short loc_40BD90
40BD90  mov     esi, [ebp-10h]
40BD93  dec     dword ptr [ebp-0Ch]
40BD96  mov     edi, [ebp-4]
40BD99  mov     eax, [ebp-8]
40BD9C  test    esi, esi
40BD9E  jz      loc_40BEA1
40BDA4  movq    xmm0, qword ptr [eax]
40BDA8  movdqa  xmm1, xmm0
40BDAC  pcmpeqb xmm1, xmm5
40BDB0  pmovmskb edx, xmm1
40BDB4  and     edx, 88h
40BDBA  dec     esi
40BDBB  cmp     dl, 88h
40BDBE  jz      loc_40BE93
40BDC4  movq    xmm2, qword ptr [edi]
40BDC8  movaps  xmm4, ds:xmmword_4E42A0
40BDCF  punpcklbw xmm0, xmm5
40BDD3  movdqa  xmm1, xmm0
40BDD7  psrlq   xmm1, 30h ; '0'
40BDDC  cvtdq2ps xmm1, xmm1
40BDDF  mulps   xmm1, xmm3
40BDE2  subps   xmm4, xmm1
40BDE5  punpcklbw xmm2, xmm5
40BDE9  movdqa  xmm3, xmm2
40BDED  psrlq   xmm3, 30h ; '0'
40BDF2  cvtdq2ps xmm3, xmm3
40BDF5  mulps   xmm3, ds:xmmword_4E42B0
40BDFC  mulps   xmm3, xmm4
40BDFF  movaps  xmm4, xmm3
40BE02  addps   xmm4, xmm1
40BE05  shufps  xmm1, xmm3, 88h
40BE09  rcpps   xmm6, xmm4
40BE0C  shufps  xmm6, xmm6, 88h
40BE10  mulps   xmm6, xmm1
40BE13  mulps   xmm6, ds:xmmword_4E42A0
40BE1A  cvttps2dq xmm1, xmm6
40BE1E  movd    edx, xmm1
40BE22  shl     edx, 4
40BE25  movq    xmm3, qword_50B900[edx]
40BE2D  pextrw  edx, xmm1, 2
40BE32  shl     edx, 4
40BE35  add     edx, offset qword_50B900
40BE3B  movdqa  xmm6, xmmword ptr [edx]
40BE3F  pextrw  edx, xmm1, 4
40BE44  punpcklqdq xmm3, xmm6
40BE48  pmullw  xmm0, xmm3
40BE4C  shl     edx, 4
40BE4F  movq    xmm3, qword_50B900[edx]
40BE57  pextrw  edx, xmm1, 6
40BE5C  shl     edx, 4
40BE5F  add     edx, offset qword_50B900
40BE65  movdqa  xmm1, xmmword ptr [edx]
40BE69  punpcklqdq xmm3, xmm1
40BE6D  pmullw  xmm2, xmm3
40BE71  movaps  xmm3, xmmword ptr [ebp-40h]
40BE75  paddw   xmm0, xmm2
40BE79  psrlw   xmm0, 8
40BE7E  cvttps2dq xmm1, xmm4
40BE82  psllq   xmm1, 30h ; '0'
40BE87  por     xmm0, xmm1
40BE8B  packuswb xmm0, xmm5
40BE8F  movq    qword ptr [edi], xmm0
40BE93  add     edi, 8
40BE96  add     eax, 8
40BE99  test    esi, esi
40BE9B  jnz     loc_40BDA4
40BEA1  cmp     dword ptr [ebp-14h], 0
40BEA5  jz      loc_40BF37
40BEAB  mov     edx, [eax]
40BEAD  mov     [ebp-18h], edx
40BEB0  test    edx, 0FF000000h
40BEB6  jz      short loc_40BF37
40BEB8  movzx   esi, byte ptr [eax+3]
40BEBC  movzx   eax, byte ptr [edi+3]
40BEC0  imul    esi, ecx
40BEC3  mov     edx, [edi]
40BEC5  mov     ecx, 10000h
40BECA  sub     ecx, esi
40BECC  imul    eax, ecx
40BECF  shr     eax, 8
40BED2  lea     ecx, [eax+esi]
40BED5  shl     eax, 8
40BED8  movd    xmm0, edx
40BEDC  xor     edx, edx
40BEDE  div     ecx
40BEE0  xor     edx, edx
40BEE2  punpcklbw xmm0, xmm5
40BEE6  shl     eax, 4
40BEE9  add     eax, offset qword_50B900
40BEEE  movdqa  xmm1, xmmword ptr [eax]
40BEF2  mov     eax, esi
40BEF4  shl     eax, 8
40BEF7  div     ecx
40BEF9  pmullw  xmm0, xmm1
40BEFD  movd    xmm1, dword ptr [ebp-18h]
40BF02  punpcklbw xmm1, xmm5
40BF06  and     ecx, 0FFFFFF00h
40BF0C  shl     ecx, 10h
40BF0F  shl     eax, 4
40BF12  add     eax, offset qword_50B900
40BF17  movdqa  xmm2, xmmword ptr [eax]
40BF1B  pmullw  xmm1, xmm2
40BF1F  paddw   xmm0, xmm1
40BF23  psrlw   xmm0, 8
40BF28  packuswb xmm0, xmm5
40BF2C  movd    eax, xmm0
40BF30  or      eax, ecx
40BF32  mov     ecx, [ebp-1Ch]
40BF35  mov     [edi], eax
40BF37  mov     edx, [ebp-20h]
40BF3A  mov     eax, [ebp-24h]
40BF3D  add     [ebp-4], edx
40BF40  add     [ebp-8], eax
40BF43  cmp     dword ptr [ebp-0Ch], 0
40BF47  jnz     loc_40BD90
40BF4D  pop     edi
40BF4E  pop     esi
40BF4F  mov     esp, ebp
40BF51  pop     ebp
40BF52  mov     esp, ebx
40BF54  pop     ebx
40BF55  retn

;==== 40BF60 sub_40BF60
40BF60  push    ecx
40BF61  test    ecx, ecx
40BF63  jz      short loc_40BF92
40BF65  cmp     ecx, 100h
40BF6B  jnb     short loc_40BF99
40BF6D  mov     edx, [eax+10h]
40BF70  test    edx, edx
40BF72  jz      short loc_40BF85
40BF74  dec     edx
40BF75  cmp     edx, 1
40BF78  ja      short loc_40BF99
40BF7A  push    esi
40BF7B  call    sub_40B6F0
40BF80  add     esp, 4
40BF83  pop     ecx
40BF84  retn
40BF85  push    ecx
40BF86  push    eax
40BF87  push    esi
40BF88  call    sub_40BFA0
40BF8D  add     esp, 0Ch
40BF90  pop     ecx
40BF91  retn
40BF92  mov     ecx, esi
40BF94  call    sub_40AF70
40BF99  pop     ecx
40BF9A  retn

;==== 40BFA0 sub_40BFA0
40BFA0  push    ebp
40BFA1  mov     ebp, esp
40BFA3  sub     esp, 18h
40BFA6  mov     ecx, [ebp+arg_4]
40BFA9  mov     edx, [ecx]
40BFAB  mov     eax, [ebp+arg_0]
40BFAE  push    ebx
40BFAF  mov     ebx, [eax]
40BFB1  push    esi
40BFB2  push    edi
40BFB3  mov     edi, [ebp+arg_8]
40BFB6  mov     [ebp+var_4], edx
40BFB9  mov     edx, [ecx+0Ch]
40BFBC  mov     esi, 100h
40BFC1  sub     esi, edi
40BFC3  mov     [ebp+var_C], ebx
40BFC6  mov     [ebp+var_8], esi
40BFC9  test    edx, edx
40BFCB  jz      loc_40C08C
40BFD1  mov     eax, [ecx+8]
40BFD4  dec     edx
40BFD5  mov     [ebp+var_18], edx
40BFD8  test    eax, eax
40BFDA  jz      loc_40C075
40BFE0  mov     ecx, [ebp+var_4]
40BFE3  sub     ecx, ebx
40BFE5  lea     edx, [ebx+eax*2]
40BFE8  mov     [ebp+var_10], ecx
40BFEB  jmp     short loc_40BFF3
40BFF0  mov     ecx, [ebp+var_10]
40BFF3  dec     eax
40BFF4  mov     [ebp+var_14], eax
40BFF7  movzx   eax, word ptr [ecx+edx-2]
40BFFC  movzx   ecx, word ptr [edx-2]
40C000  sub     edx, 2
40C003  mov     ebx, eax
40C005  and     ebx, 3E0h
40C00B  imul    ebx, esi
40C00E  mov     esi, ecx
40C010  and     esi, 3E0h
40C016  imul    esi, edi
40C019  add     ebx, esi
40C01B  mov     esi, eax
40C01D  and     esi, 7C00h
40C023  imul    esi, [ebp+var_8]
40C027  mov     edi, ecx
40C029  and     edi, 7C00h
40C02F  imul    edi, [ebp+arg_8]
40C033  add     esi, edi
40C035  mov     edi, [ebp+arg_8]
40C038  shr     ebx, 8
40C03B  shr     esi, 8
40C03E  and     ebx, 3E0h
40C044  and     esi, 7C00h
40C04A  and     eax, 1Fh
40C04D  add     ebx, esi
40C04F  mov     esi, [ebp+var_8]
40C052  imul    eax, esi
40C055  and     ecx, 1Fh
40C058  imul    ecx, edi
40C05B  add     eax, ecx
40C05D  shr     eax, 8
40C060  add     ebx, eax
40C062  mov     eax, [ebp+var_14]
40C065  mov     [edx], bx
40C068  test    eax, eax
40C06A  jnz     short loc_40BFF0
40C06C  mov     edx, [ebp+var_18]
40C06F  mov     ebx, [ebp+var_C]
40C072  mov     ecx, [ebp+arg_4]
40C075  mov     eax, [ebp+arg_0]
40C078  add     ebx, [eax+4]
40C07B  mov     eax, [ecx+4]
40C07E  add     [ebp+var_4], eax
40C081  mov     [ebp+var_C], ebx
40C084  test    edx, edx
40C086  jnz     loc_40BFD1
40C08C  pop     edi
40C08D  pop     esi
40C08E  pop     ebx
40C08F  mov     esp, ebp
40C091  pop     ebp
40C092  retn

;==== 40C4F0 sub_40C4F0
40C4F0  push    ebp
40C4F1  mov     ebp, esp
40C4F3  and     esp, 0FFFFFFF8h
40C4F6  sub     esp, 14h
40C4F9  push    ebx
40C4FA  push    esi
40C4FB  push    edi
40C4FC  mov     edi, [ebp+arg_0]
40C4FF  mov     ecx, [edi+10h]
40C502  cmp     ecx, 1
40C505  jz      short loc_40C518
40C507  cmp     ecx, 2
40C50A  jz      short loc_40C518
40C50C  mov     eax, 0Ah
40C511  pop     edi
40C512  pop     esi
40C513  pop     ebx
40C514  mov     esp, ebp
40C516  pop     ebp
40C517  retn
40C518  mov     esi, [ebp+arg_4]
40C51B  mov     eax, [esi+10h]
40C51E  mov     ebx, [ebp+arg_8]
40C521  cmp     eax, [ebx+10h]
40C524  jnz     short loc_40C5A0
40C526  cmp     ecx, eax
40C528  jnz     short loc_40C5A0
40C52A  cmp     [ebp+arg_10], 0
40C52E  jz      short loc_40C566
40C530  mov     eax, [ebp+arg_C]
40C533  lea     ecx, [esp+20h+var_10]
40C537  push    ecx
40C538  push    0
40C53A  push    0
40C53C  push    3
40C53E  lea     edx, [esp+30h+var_C]
40C542  push    edx
40C543  push    offset sub_41A3C0
40C548  mov     edx, esi
40C54A  mov     [esp+38h+var_C], edi
40C54E  mov     [esp+38h+var_8], esi
40C552  mov     [esp+38h+var_4], ebx
40C556  mov     [esp+38h+var_10], eax
40C55A  call    sub_41A120
40C55F  add     esp, 18h
40C562  test    eax, eax
40C564  jnz     short loc_40C597
40C566  cmp     dword ptr [edi+10h], 1
40C56A  jnz     short loc_40C586
40C56C  mov     eax, [ebp+arg_C]
40C56F  push    eax
40C570  push    edi
40C571  mov     eax, ebx
40C573  mov     ecx, esi
40C575  call    sub_40C5B0
40C57A  add     esp, 8
40C57D  xor     eax, eax
40C57F  pop     edi
40C580  pop     esi
40C581  pop     ebx
40C582  mov     esp, ebp
40C584  pop     ebp
40C585  retn
40C586  mov     ecx, [ebp+arg_C]
40C589  push    ecx
40C58A  push    esi
40C58B  mov     ecx, ebx
40C58D  mov     edx, edi
40C58F  call    sub_40C790
40C594  add     esp, 8
40C597  xor     eax, eax
40C599  pop     edi
40C59A  pop     esi
40C59B  pop     ebx
40C59C  mov     esp, ebp
40C59E  pop     ebp
40C59F  retn
40C5A0  pop     edi
40C5A1  pop     esi
40C5A2  mov     eax, 9
40C5A7  pop     ebx
40C5A8  mov     esp, ebp
40C5AA  pop     ebp
40C5AB  retn

;==== 40C5B0 sub_40C5B0
40C5B0  push    ebp
40C5B1  mov     ebp, esp
40C5B3  sub     esp, 18h
40C5B6  mov     edx, [ebp+arg_0]
40C5B9  push    esi
40C5BA  mov     esi, [ecx]
40C5BC  push    edi
40C5BD  mov     edi, [edx]
40C5BF  mov     edx, [edx+4]
40C5C2  mov     [ebp+var_10], edx
40C5C5  mov     edx, [ecx+4]
40C5C8  mov     [ebp+var_4], esi
40C5CB  mov     esi, [eax]
40C5CD  mov     [ebp+var_18], edx
40C5D0  mov     edx, [eax+4]
40C5D3  mov     [ebp+arg_0], esi
40C5D6  mov     esi, [eax+0Ch]
40C5D9  mov     [ebp+var_14], edx
40C5DC  mov     edx, [ecx+0Ch]
40C5DF  cmp     edx, esi
40C5E1  ja      short loc_40C5E5
40C5E3  mov     esi, edx
40C5E5  mov     ecx, [ecx+8]
40C5E8  mov     eax, [eax+8]
40C5EB  mov     edx, ecx
40C5ED  cmp     ecx, eax
40C5EF  jbe     short loc_40C5F3
40C5F1  mov     edx, eax
40C5F3  mov     eax, [ebp+arg_4]
40C5F6  shl     eax, 4
40C5F9  cwde
40C5FA  movd    xmm1, eax
40C5FE  punpcklwd xmm1, xmm1
40C602  pxor    xmm0, xmm0
40C606  pshufd  xmm2, xmm1, 0
40C60B  mov     [ebp+arg_4], esi
40C60E  cmp     edi, [ebp+var_4]
40C611  jz      loc_40C6D9
40C617  test    esi, esi
40C619  jz      loc_40C6D1
40C61F  mov     eax, edx
40C621  shr     eax, 1
40C623  and     edx, 1
40C626  mov     [ebp+var_8], eax
40C629  mov     [ebp+var_C], edx
40C62C  jmp     short loc_40C633
40C630  mov     eax, [ebp+var_8]
40C633  dec     [ebp+arg_4]
40C636  mov     edx, [ebp+var_4]
40C639  mov     esi, [ebp+arg_0]
40C63C  mov     ecx, edi
40C63E  test    eax, eax
40C640  jz      short loc_40C67E
40C642  movq    xmm1, qword ptr [edx]
40C646  movq    xmm4, qword ptr [esi]
40C64A  punpcklbw xmm4, xmm0
40C64E  punpcklbw xmm1, xmm0
40C652  psubw   xmm4, xmm1
40C656  psllw   xmm4, 4
40C65B  movdqa  xmm3, xmm1
40C65F  pmulhw  xmm4, xmm2
40C663  paddw   xmm3, xmm4
40C667  packuswb xmm3, xmm0
40C66B  movdq2q mm0, xmm3
40C66F  movntq  qword ptr [ecx], mm0
40C672  add     ecx, 8
40C675  add     edx, 8
40C678  add     esi, 8
40C67B  dec     eax
40C67C  jnz     short loc_40C642
40C67E  cmp     [ebp+var_C], 0
40C682  jz      short loc_40C6B8
40C684  mov     edx, [edx]
40C686  mov     eax, [esi]
40C688  movd    xmm1, edx
40C68C  punpcklbw xmm1, xmm0
40C690  movd    xmm4, eax
40C694  punpcklbw xmm4, xmm0
40C698  psubw   xmm4, xmm1
40C69C  psllw   xmm4, 4
40C6A1  movdqa  xmm3, xmm1
40C6A5  pmulhw  xmm4, xmm2
40C6A9  paddw   xmm3, xmm4
40C6AD  packuswb xmm3, xmm0
40C6B1  movd    edx, xmm3
40C6B5  movnti  dword ptr [ecx], edx
40C6B8  mov     eax, [ebp+var_18]
40C6BB  mov     ecx, [ebp+var_14]
40C6BE  add     edi, [ebp+var_10]
40C6C1  add     [ebp+var_4], eax
40C6C4  add     [ebp+arg_0], ecx
40C6C7  cmp     [ebp+arg_4], 0
40C6CB  jnz     loc_40C630
40C6D1  emms
40C6D3  pop     edi
40C6D4  pop     esi
40C6D5  mov     esp, ebp
40C6D7  pop     ebp
40C6D8  retn
40C6D9  test    esi, esi
40C6DB  jz      loc_40C783
40C6E1  mov     ecx, edx
40C6E3  shr     ecx, 1
40C6E5  and     edx, 1
40C6E8  mov     [ebp+var_8], ecx
40C6EB  mov     [ebp+var_C], edx
40C6EE  jmp     short loc_40C6F3
40C6F0  mov     ecx, [ebp+var_8]
40C6F3  dec     [ebp+arg_4]
40C6F6  mov     esi, [ebp+arg_0]
40C6F9  mov     eax, edi
40C6FB  mov     edx, esi
40C6FD  test    ecx, ecx
40C6FF  jz      short loc_40C737
40C701  movq    xmm1, qword ptr [eax]
40C705  movq    xmm4, qword ptr [edx]
40C709  punpcklbw xmm4, xmm0
40C70D  punpcklbw xmm1, xmm0
40C711  psubw   xmm4, xmm1
40C715  psllw   xmm4, 4
40C71A  movdqa  xmm3, xmm1
40C71E  pmulhw  xmm4, xmm2
40C722  paddw   xmm3, xmm4
40C726  packuswb xmm3, xmm0
40C72A  movq    qword ptr [eax], xmm3
40C72E  add     eax, 8
40C731  add     edx, 8
40C734  dec     ecx
40C735  jnz     short loc_40C701
40C737  cmp     [ebp+var_C], 0
40C73B  jz      short loc_40C770
40C73D  mov     ecx, [eax]
40C73F  mov     edx, [edx]
40C741  movd    xmm1, ecx
40C745  punpcklbw xmm1, xmm0
40C749  movd    xmm4, edx
40C74D  punpcklbw xmm4, xmm0
40C751  psubw   xmm4, xmm1
40C755  psllw   xmm4, 4
40C75A  movdqa  xmm3, xmm1
40C75E  pmulhw  xmm4, xmm2
40C762  paddw   xmm3, xmm4
40C766  packuswb xmm3, xmm0
40C76A  movd    ecx, xmm3
40C76E  mov     [eax], ecx
40C770  add     esi, [ebp+var_14]
40C773  add     edi, [ebp+var_10]
40C776  cmp     [ebp+arg_4], 0
40C77A  mov     [ebp+arg_0], esi
40C77D  jnz     loc_40C6F0
40C783  pop     edi
40C784  pop     esi
40C785  mov     esp, ebp
40C787  pop     ebp
40C788  retn

;==== 40C790 sub_40C790
40C790  push    ebx
40C791  mov     ebx, esp
40C793  sub     esp, 8
40C796  and     esp, 0FFFFFFF0h
40C799  add     esp, 4
40C79C  push    ebp
40C79D  mov     ebp, [ebx+4]
40C7A0  mov     [esp+0Ch+var_8], ebp
40C7A4  mov     ebp, esp
40C7A6  sub     esp, 898h
40C7AC  mov     eax, dword_4FB734
40C7B1  xor     eax, ebp
40C7B3  mov     [ebp-4], eax
40C7B6  push    esi
40C7B7  push    edi
40C7B8  mov     eax, [ebx+8]
40C7BB  mov     esi, [edx]
40C7BD  mov     edx, [edx+4]
40C7C0  mov     [ebp-840h], esi
40C7C6  mov     esi, [eax]
40C7C8  mov     [ebp-84Ch], edx
40C7CE  mov     edx, [eax+4]
40C7D1  mov     [ebp-834h], esi
40C7D7  mov     esi, [ecx]
40C7D9  mov     [ebp-844h], edx
40C7DF  mov     edx, [ecx+4]
40C7E2  mov     [ebp-82Ch], esi
40C7E8  mov     esi, [ecx+0Ch]
40C7EB  mov     [ebp-848h], edx
40C7F1  mov     edx, [eax+0Ch]
40C7F4  mov     [ebp-828h], edx
40C7FA  cmp     edx, esi
40C7FC  jbe     short loc_40C804
40C7FE  mov     [ebp-828h], esi
40C804  mov     eax, [eax+8]
40C807  mov     ecx, [ecx+8]
40C80A  mov     [ebp-824h], eax
40C810  cmp     eax, ecx
40C812  jbe     short loc_40C81A
40C814  mov     [ebp-824h], ecx
40C81A  mov     eax, 100h
40C81F  sub     eax, [ebx+0Ch]
40C822  xor     ecx, ecx
40C824  mov     [ebp-83Ch], eax
40C82A  movzx   eax, ax
40C82D  movd    xmm1, eax
40C831  mov     eax, ecx
40C833  movzx   eax, ax
40C836  movd    xmm2, eax
40C83A  mov     eax, ecx
40C83C  movzx   eax, ax
40C83F  movd    xmm3, eax
40C843  movzx   eax, word ptr [ebp-83Ch]
40C84A  pxor    xmm0, xmm0
40C84E  xor     edi, edi
40C850  movzx   ecx, cx
40C853  movdqa  xmmword ptr [ebp-880h], xmm0
40C85B  movdqa  xmm0, ds:xmmword_4E42D0
40C863  movd    xmm5, eax
40C867  xor     edx, edx
40C869  movd    xmm4, ecx
40C86D  movzx   eax, di
40C870  movzx   edi, word ptr [ebx+0Ch]
40C874  movdqa  xmmword ptr [ebp-890h], xmm0
40C87C  movzx   ecx, dx
40C87F  movd    xmm0, eax
40C883  xor     eax, eax
40C885  movd    xmm6, ecx
40C889  punpcklwd xmm5, xmm1
40C88D  movd    xmm1, edi
40C891  mov     edi, eax
40C893  xor     esi, esi
40C895  movzx   edi, di
40C898  punpcklwd xmm6, xmm2
40C89C  movzx   edx, si
40C89F  movd    xmm2, edi
40C8A3  mov     edi, eax
40C8A5  movd    xmm7, edx
40C8A9  punpcklwd xmm0, xmm4
40C8AD  movzx   eax, ax
40C8B0  movd    xmm4, eax
40C8B4  movzx   eax, word ptr [ebx+0Ch]
40C8B8  punpcklwd xmm7, xmm3
40C8BC  punpcklwd xmm7, xmm5
40C8C0  punpcklwd xmm0, xmm6
40C8C4  punpcklwd xmm0, xmm7
40C8C8  xor     edx, edx
40C8CA  xor     ecx, ecx
40C8CC  movd    xmm5, eax
40C8D0  movzx   ecx, cx
40C8D3  movzx   edx, dx
40C8D6  movdqa  xmmword ptr [ebp-870h], xmm0
40C8DE  movzx   edi, di
40C8E1  movzx   eax, si
40C8E4  movd    xmm6, ecx
40C8E8  movd    xmm7, edx
40C8EC  movd    xmm3, edi
40C8F0  movd    xmm0, eax
40C8F4  punpcklwd xmm5, xmm1
40C8F8  xor     ecx, ecx
40C8FA  lea     edx, [ebp-820h]
40C900  punpcklwd xmm6, xmm2
40C904  punpcklwd xmm0, xmm4
40C908  punpcklwd xmm7, xmm3
40C90C  punpcklwd xmm0, xmm6
40C910  punpcklwd xmm7, xmm5
40C914  punpcklwd xmm0, xmm7
40C918  movdqa  xmmword ptr [ebp-860h], xmm0
40C920  xor     esi, esi
40C922  movzx   esi, si
40C925  movzx   eax, cx
40C928  movd    xmm0, esi
40C92C  mov     esi, eax
40C92E  xor     edi, edi
40C930  movd    xmm1, esi
40C934  movd    xmm2, esi
40C938  movd    xmm3, esi
40C93C  movzx   esi, di
40C93F  movd    xmm4, esi
40C943  mov     esi, eax
40C945  movd    xmm5, esi
40C949  movd    xmm6, esi
40C94D  movd    xmm7, eax
40C951  inc     ecx
40C952  add     edx, 10h
40C955  punpcklwd xmm4, xmm0
40C959  punpcklwd xmm5, xmm1
40C95D  punpcklwd xmm7, xmm3
40C961  punpcklwd xmm6, xmm2
40C965  punpcklwd xmm7, xmm5
40C969  punpcklwd xmm6, xmm4
40C96D  punpcklwd xmm7, xmm6
40C971  movdqa  xmmword ptr [edx-10h], xmm7
40C976  cmp     ecx, 81h
40C97C  jl      short loc_40C920
40C97E  mov     edi, [ebp-840h]
40C984  cmp     edi, [ebp-834h]
40C98A  jz      loc_40CB93
40C990  mov     eax, [ebp-828h]
40C996  mov     [ebp-828h], eax
40C99C  test    eax, eax
40C99E  jz      loc_40CB7E
40C9A4  mov     ecx, [ebp-824h]
40C9AA  movdqa  xmm4, xmmword ptr [ebp-880h]
40C9B2  movdqa  xmm5, xmmword ptr [ebp-890h]
40C9BA  movdqa  xmm6, xmmword ptr [ebp-860h]
40C9C2  mov     eax, ecx
40C9C4  shr     eax, 1
40C9C6  and     ecx, 1
40C9C9  mov     [ebp-824h], eax
40C9CF  mov     [ebp-830h], ecx
40C9D5  jmp     short loc_40C9E6
40C9D7  jmp     short loc_40C9E0
40C9E0  mov     eax, [ebp-824h]
40C9E6  dec     dword ptr [ebp-828h]
40C9EC  mov     edx, [ebp-834h]
40C9F2  mov     esi, [ebp-82Ch]
40C9F8  mov     ecx, edi
40C9FA  test    eax, eax
40C9FC  jz      loc_40CAAE
40CA02  movq    xmm0, qword ptr [edx]
40CA06  movq    xmm1, qword ptr [esi]
40CA0A  punpcklbw xmm0, xmm4
40CA0E  movdqa  xmm2, xmm0
40CA12  pmaddwd xmm2, xmmword ptr [ebp-870h]
40CA1A  movdqa  xmm3, xmm2
40CA1E  punpcklbw xmm1, xmm4
40CA22  movdqa  xmm7, xmm1
40CA26  pmaddwd xmm7, xmm6
40CA2A  paddd   xmm3, xmm7
40CA2E  pslld   xmm2, 7
40CA33  cvtdq2ps xmm7, xmm3
40CA36  rcpps   xmm7, xmm7
40CA39  cvtdq2ps xmm2, xmm2
40CA3C  mulps   xmm2, xmm7
40CA3F  cvttps2dq xmm2, xmm2
40CA43  pslld   xmm3, 8
40CA48  pextrw  edi, xmm2, 2
40CA4D  add     edi, edi
40CA4F  movdqa  xmm7, xmm5
40CA53  pandn   xmm7, xmm3
40CA57  movq    xmm3, qword ptr [ebp+edi*8-820h]
40CA60  pextrw  edi, xmm2, 6
40CA65  add     edi, edi
40CA67  movdqa  xmm2, xmmword ptr [ebp+edi*8-820h]
40CA70  psubw   xmm0, xmm1
40CA74  punpcklqdq xmm3, xmm2
40CA78  pmullw  xmm0, xmm3
40CA7C  psraw   xmm0, 7
40CA81  pand    xmm1, xmm5
40CA85  paddw   xmm0, xmm1
40CA89  por     xmm7, xmm0
40CA8D  packuswb xmm7, xmm4
40CA91  movdq2q mm0, xmm7
40CA95  movntq  qword ptr [ecx], mm0
40CA98  add     ecx, 8
40CA9B  add     edx, 8
40CA9E  add     esi, 8
40CAA1  dec     eax
40CAA2  jnz     loc_40CA02
40CAA8  mov     edi, [ebp-840h]
40CAAE  cmp     dword ptr [ebp-830h], 0
40CAB5  jz      loc_40CB4D
40CABB  mov     edx, [edx]
40CABD  mov     eax, [esi]
40CABF  movd    xmm0, edx
40CAC3  punpcklbw xmm0, xmm4
40CAC7  movdqa  xmm2, xmm0
40CACB  pmaddwd xmm2, xmmword ptr [ebp-870h]
40CAD3  movdqa  xmm3, xmm2
40CAD7  movd    xmm1, eax
40CADB  punpcklbw xmm1, xmm4
40CADF  movdqa  xmm7, xmm1
40CAE3  pmaddwd xmm7, xmm6
40CAE7  paddd   xmm3, xmm7
40CAEB  movdqa  xmm7, xmm3
40CAEF  pslld   xmm2, 7
40CAF4  cvtdq2ps xmm2, xmm2
40CAF7  cvtdq2ps xmm3, xmm3
40CAFA  rcpps   xmm3, xmm3
40CAFD  mulps   xmm2, xmm3
40CB00  cvttps2dq xmm2, xmm2
40CB04  pextrw  edx, xmm2, 2
40CB09  psubw   xmm0, xmm1
40CB0D  add     edx, edx
40CB0F  movdqa  xmm2, xmmword ptr [ebp+edx*8-820h]
40CB18  pmullw  xmm0, xmm2
40CB1C  movdqa  xmm6, xmm5
40CB20  pslld   xmm7, 8
40CB25  pandn   xmm6, xmm7
40CB29  psraw   xmm0, 7
40CB2E  pand    xmm1, xmm5
40CB32  paddw   xmm0, xmm1
40CB36  por     xmm6, xmm0
40CB3A  packuswb xmm6, xmm4
40CB3E  movd    eax, xmm6
40CB42  movdqa  xmm6, xmmword ptr [ebp-860h]
40CB4A  movnti  dword ptr [ecx], eax
40CB4D  add     edi, [ebp-84Ch]
40CB53  mov     ecx, [ebp-844h]
40CB59  mov     edx, [ebp-848h]
40CB5F  add     [ebp-834h], ecx
40CB65  add     [ebp-82Ch], edx
40CB6B  cmp     dword ptr [ebp-828h], 0
40CB72  mov     [ebp-840h], edi
40CB78  jnz     loc_40C9E0
40CB7E  emms
40CB80  pop     edi
40CB81  pop     esi
40CB82  mov     ecx, [ebp-4]
40CB85  xor     ecx, ebp
40CB87  call    sub_4ABDD5
40CB8C  mov     esp, ebp
40CB8E  pop     ebp
40CB8F  mov     esp, ebx
40CB91  pop     ebx
40CB92  retn
40CB93  mov     esi, [ebp-828h]
40CB99  test    esi, esi
40CB9B  jz      loc_40CD5B
40CBA1  mov     eax, [ebp-824h]
40CBA7  movdqa  xmm4, xmmword ptr [ebp-880h]
40CBAF  movdqa  xmm5, xmmword ptr [ebp-890h]
40CBB7  movdqa  xmm6, xmmword ptr [ebp-860h]
40CBBF  mov     ecx, eax
40CBC1  shr     ecx, 1
40CBC3  and     eax, 1
40CBC6  mov     [ebp-824h], ecx
40CBCC  mov     [ebp-830h], eax
40CBD2  jmp     short loc_40CBE6
40CBD4  jmp     short loc_40CBE0
40CBE0  mov     ecx, [ebp-824h]
40CBE6  mov     edx, [ebp-82Ch]
40CBEC  dec     esi
40CBED  mov     [ebp-844h], esi
40CBF3  mov     eax, edi
40CBF5  test    ecx, ecx
40CBF7  jz      loc_40CCA3
40CBFD  movq    xmm0, qword ptr [eax]
40CC01  movq    xmm1, qword ptr [edx]
40CC05  punpcklbw xmm0, xmm4
40CC09  movdqa  xmm2, xmm0
40CC0D  pmaddwd xmm2, xmmword ptr [ebp-870h]
40CC15  movdqa  xmm3, xmm2
40CC19  punpcklbw xmm1, xmm4
40CC1D  movdqa  xmm7, xmm1
40CC21  pmaddwd xmm7, xmm6
40CC25  paddd   xmm3, xmm7
40CC29  pslld   xmm2, 7
40CC2E  cvtdq2ps xmm7, xmm3
40CC31  rcpps   xmm7, xmm7
40CC34  cvtdq2ps xmm2, xmm2
40CC37  mulps   xmm2, xmm7
40CC3A  cvttps2dq xmm2, xmm2
40CC3E  pslld   xmm3, 8
40CC43  pextrw  esi, xmm2, 2
40CC48  add     esi, esi
40CC4A  movdqa  xmm7, xmm5
40CC4E  pandn   xmm7, xmm3
40CC52  movq    xmm3, qword ptr [ebp+esi*8-820h]
40CC5B  pextrw  esi, xmm2, 6
40CC60  add     esi, esi
40CC62  movdqa  xmm2, xmmword ptr [ebp+esi*8-820h]
40CC6B  psubw   xmm0, xmm1
40CC6F  punpcklqdq xmm3, xmm2
40CC73  pmullw  xmm0, xmm3
40CC77  psraw   xmm0, 7
40CC7C  pand    xmm1, xmm5
40CC80  paddw   xmm0, xmm1
40CC84  por     xmm7, xmm0
40CC88  packuswb xmm7, xmm4
40CC8C  movq    qword ptr [eax], xmm7
40CC90  add     eax, 8
40CC93  add     edx, 8
40CC96  dec     ecx
40CC97  jnz     loc_40CBFD
40CC9D  mov     esi, [ebp-844h]
40CCA3  cmp     dword ptr [ebp-830h], 0
40CCAA  jz      loc_40CD41
40CCB0  mov     ecx, [eax]
40CCB2  mov     edx, [edx]
40CCB4  movd    xmm0, ecx
40CCB8  punpcklbw xmm0, xmm4
40CCBC  movdqa  xmm2, xmm0
40CCC0  pmaddwd xmm2, xmmword ptr [ebp-870h]
40CCC8  movdqa  xmm3, xmm2
40CCCC  movd    xmm1, edx
40CCD0  punpcklbw xmm1, xmm4
40CCD4  movdqa  xmm7, xmm1
40CCD8  pmaddwd xmm7, xmm6
40CCDC  paddd   xmm3, xmm7
40CCE0  movdqa  xmm7, xmm3
40CCE4  pslld   xmm2, 7
40CCE9  cvtdq2ps xmm2, xmm2
40CCEC  cvtdq2ps xmm3, xmm3
40CCEF  rcpps   xmm3, xmm3
40CCF2  mulps   xmm2, xmm3
40CCF5  cvttps2dq xmm2, xmm2
40CCF9  pextrw  ecx, xmm2, 2
40CCFE  psubw   xmm0, xmm1
40CD02  add     ecx, ecx
40CD04  movdqa  xmm2, xmmword ptr [ebp+ecx*8-820h]
40CD0D  pmullw  xmm0, xmm2
40CD11  movdqa  xmm6, xmm5
40CD15  pslld   xmm7, 8
40CD1A  pandn   xmm6, xmm7
40CD1E  psraw   xmm0, 7
40CD23  pand    xmm1, xmm5
40CD27  paddw   xmm0, xmm1
40CD2B  por     xmm6, xmm0
40CD2F  packuswb xmm6, xmm4
40CD33  movd    edx, xmm6
40CD37  movdqa  xmm6, xmmword ptr [ebp-860h]
40CD3F  mov     [eax], edx
40CD41  mov     eax, [ebp-848h]
40CD47  add     edi, [ebp-84Ch]
40CD4D  add     [ebp-82Ch], eax
40CD53  test    esi, esi
40CD55  jnz     loc_40CBE0
40CD5B  mov     ecx, [ebp-4]
40CD5E  pop     edi
40CD5F  xor     ecx, ebp
40CD61  pop     esi
40CD62  call    sub_4ABDD5
40CD67  mov     esp, ebp
40CD69  pop     ebp
40CD6A  mov     esp, ebx
40CD6C  pop     ebx
40CD6D  retn

;==== 40CD70 sub_40CD70
40CD70  push    ebp
40CD71  mov     ebp, esp
40CD73  push    edi
40CD74  mov     edi, [ebp+arg_0]
40CD77  test    edi, edi
40CD79  jz      short loc_40CDDB
40CD7B  mov     eax, [edx+10h]
40CD7E  sub     eax, 0
40CD81  jz      short loc_40CDD0
40CD83  dec     eax
40CD84  jz      short loc_40CDAC
40CD86  dec     eax
40CD87  jnz     short loc_40CDDB
40CD89  mov     eax, [ecx+10h]
40CD8C  dec     eax
40CD8D  jz      short loc_40CD9F
40CD8F  dec     eax
40CD90  jnz     short loc_40CDDB
40CD92  push    edi
40CD93  push    edx
40CD94  call    sub_40D350
40CD99  add     esp, 8
40CD9C  pop     edi
40CD9D  pop     ebp
40CD9E  retn
40CD9F  push    edi
40CDA0  push    edx
40CDA1  call    sub_40D0D0
40CDA6  add     esp, 8
40CDA9  pop     edi
40CDAA  pop     ebp
40CDAB  retn
40CDAC  mov     eax, [ecx+10h]
40CDAF  dec     eax
40CDB0  jz      short loc_40CDC2
40CDB2  dec     eax
40CDB3  jnz     short loc_40CDDB
40CDB5  push    edx
40CDB6  push    ecx
40CDB7  call    sub_40CFD0
40CDBC  add     esp, 8
40CDBF  pop     edi
40CDC0  pop     ebp
40CDC1  retn
40CDC2  push    edi
40CDC3  push    edx
40CDC4  push    ecx
40CDC5  call    sub_40CEF0
40CDCA  add     esp, 0Ch
40CDCD  pop     edi
40CDCE  pop     ebp
40CDCF  retn
40CDD0  push    edi
40CDD1  push    edx
40CDD2  push    ecx
40CDD3  call    sub_40CDE0
40CDD8  add     esp, 0Ch
40CDDB  pop     edi
40CDDC  pop     ebp
40CDDD  retn

;==== 40CDE0 sub_40CDE0
40CDE0  push    ebp
40CDE1  mov     ebp, esp
40CDE3  sub     esp, 14h
40CDE6  mov     eax, [ebp+arg_0]
40CDE9  push    ebx
40CDEA  mov     ebx, [ebp+arg_4]
40CDED  mov     edx, [ebx]
40CDEF  push    esi
40CDF0  mov     esi, [eax]
40CDF2  mov     eax, [ebx+0Ch]
40CDF5  mov     [ebp+var_4], esi
40CDF8  mov     [ebp+var_8], edx
40CDFB  test    eax, eax
40CDFD  jz      loc_40CEE4
40CE03  push    edi
40CE04  mov     ecx, [ebx+8]
40CE07  dec     eax
40CE08  mov     [ebp+var_14], eax
40CE0B  test    ecx, ecx
40CE0D  jz      loc_40CECC
40CE13  sub     edx, esi
40CE15  lea     edi, [esi+ecx*2]
40CE18  mov     [ebp+var_10], edx
40CE1B  jmp     short loc_40CE20
40CE20  movzx   eax, word ptr [edx+edi-2]
40CE25  sub     edi, 2
40CE28  dec     ecx
40CE29  mov     [ebp+var_C], ecx
40CE2C  test    ax, ax
40CE2F  jz      loc_40CEBE
40CE35  mov     ebx, [ebp+arg_8]
40CE38  movzx   esi, word ptr [edi]
40CE3B  mov     ecx, eax
40CE3D  and     ecx, 1Fh
40CE40  imul    ecx, ebx
40CE43  mov     edx, esi
40CE45  shr     ecx, 8
40CE48  and     edx, 1Fh
40CE4B  add     ecx, edx
40CE4D  cmp     ecx, 20h ; ' '
40CE50  jb      short loc_40CE57
40CE52  mov     ecx, 1Fh
40CE57  mov     edx, eax
40CE59  and     edx, 3E0h
40CE5F  imul    edx, ebx
40CE62  mov     ebx, esi
40CE64  shr     edx, 8
40CE67  and     ebx, 3E0h
40CE6D  add     edx, ebx
40CE6F  cmp     edx, 400h
40CE75  jnb     short loc_40CE7F
40CE77  and     edx, 3E0h
40CE7D  jmp     short loc_40CE84
40CE7F  mov     edx, 3E0h
40CE84  and     eax, 7C00h
40CE89  imul    eax, [ebp+arg_8]
40CE8D  shr     eax, 8
40CE90  and     esi, 7C00h
40CE96  add     eax, esi
40CE98  cmp     eax, 8000h
40CE9D  jnb     short loc_40CEA6
40CE9F  and     eax, 7C00h
40CEA4  jmp     short loc_40CEAB
40CEA6  mov     eax, 7C00h
40CEAB  mov     ebx, [ebp+arg_4]
40CEAE  mov     esi, [ebp+var_4]
40CEB1  add     ecx, edx
40CEB3  mov     edx, [ebp+var_10]
40CEB6  add     ecx, eax
40CEB8  mov     [edi], cx
40CEBB  mov     ecx, [ebp+var_C]
40CEBE  test    ecx, ecx
40CEC0  jnz     loc_40CE20
40CEC6  mov     eax, [ebp+var_14]
40CEC9  mov     edx, [ebp+var_8]
40CECC  mov     ecx, [ebp+arg_0]
40CECF  add     esi, [ecx+4]
40CED2  add     edx, [ebx+4]
40CED5  mov     [ebp+var_4], esi
40CED8  mov     [ebp+var_8], edx
40CEDB  test    eax, eax
40CEDD  jnz     loc_40CE04
40CEE3  pop     edi
40CEE4  pop     esi
40CEE5  pop     ebx
40CEE6  mov     esp, ebp
40CEE8  pop     ebp
40CEE9  retn

;==== 40CEF0 sub_40CEF0
40CEF0  push    ebp
40CEF1  mov     ebp, esp
40CEF3  sub     esp, 0Ch
40CEF6  mov     eax, [ebp+arg_4]
40CEF9  mov     edx, [eax]
40CEFB  push    ebx
40CEFC  mov     ebx, [ebp+arg_0]
40CEFF  mov     ecx, [ebx]
40CF01  push    esi
40CF02  mov     esi, [eax+0Ch]
40CF05  mov     [ebp+var_4], ecx
40CF08  mov     [ebp+var_8], edx
40CF0B  test    esi, esi
40CF0D  jz      loc_40CFC6
40CF13  push    edi
40CF14  mov     edi, [eax+8]
40CF17  dec     esi
40CF18  mov     [ebp+var_C], esi
40CF1B  test    edi, edi
40CF1D  jz      loc_40CFB1
40CF23  movzx   eax, byte ptr [edx]
40CF26  movzx   esi, byte ptr [edx+2]
40CF2A  movzx   ebx, byte ptr [edx+1]
40CF2E  add     esi, eax
40CF30  add     ebx, esi
40CF32  dec     edi
40CF33  test    ebx, ebx
40CF35  jle     short loc_40CF94
40CF37  mov     ebx, [ebp+arg_8]
40CF3A  movzx   esi, byte ptr [ecx]
40CF3D  imul    eax, ebx
40CF40  shr     eax, 8
40CF43  add     esi, eax
40CF45  mov     eax, esi
40CF47  cmp     esi, 100h
40CF4D  jb      short loc_40CF54
40CF4F  mov     eax, 0FFh
40CF54  movzx   esi, byte ptr [ecx+1]
40CF58  mov     [ecx], al
40CF5A  movzx   eax, byte ptr [edx+1]
40CF5E  imul    eax, ebx
40CF61  shr     eax, 8
40CF64  add     eax, esi
40CF66  cmp     eax, 100h
40CF6B  jb      short loc_40CF72
40CF6D  mov     eax, 0FFh
40CF72  movzx   esi, byte ptr [ecx+2]
40CF76  mov     [ecx+1], al
40CF79  movzx   eax, byte ptr [edx+2]
40CF7D  imul    eax, ebx
40CF80  shr     eax, 8
40CF83  add     eax, esi
40CF85  cmp     eax, 100h
40CF8A  jb      short loc_40CF91
40CF8C  mov     eax, 0FFh
40CF91  mov     [ecx+2], al
40CF94  mov     ebx, [ebp+arg_0]
40CF97  mov     eax, [ebp+arg_4]
40CF9A  add     ecx, [ebx+14h]
40CF9D  add     edx, [eax+14h]
40CFA0  test    edi, edi
40CFA2  jnz     loc_40CF23
40CFA8  mov     edx, [ebp+var_8]
40CFAB  mov     ecx, [ebp+var_4]
40CFAE  mov     esi, [ebp+var_C]
40CFB1  add     ecx, [ebx+4]
40CFB4  add     edx, [eax+4]
40CFB7  mov     [ebp+var_4], ecx
40CFBA  mov     [ebp+var_8], edx
40CFBD  test    esi, esi
40CFBF  jnz     loc_40CF14
40CFC5  pop     edi
40CFC6  pop     esi
40CFC7  pop     ebx
40CFC8  mov     esp, ebp
40CFCA  pop     ebp
40CFCB  retn

;==== 40CFD0 sub_40CFD0
40CFD0  push    ebp
40CFD1  mov     ebp, esp
40CFD3  sub     esp, 10h
40CFD6  mov     eax, [ebp+arg_0]
40CFD9  mov     eax, [eax]
40CFDB  push    esi
40CFDC  mov     esi, [ebp+arg_4]
40CFDF  mov     ecx, [esi+0Ch]
40CFE2  mov     edx, [esi]
40CFE4  mov     [ebp+var_8], eax
40CFE7  mov     [ebp+var_C], edx
40CFEA  test    ecx, ecx
40CFEC  jz      loc_40D0C9
40CFF2  push    ebx
40CFF3  mov     ebx, [esi+8]
40CFF6  dec     ecx
40CFF7  mov     [ebp+var_10], ecx
40CFFA  mov     [ebp+var_4], ebx
40CFFD  test    ebx, ebx
40CFFF  jz      loc_40D0B1
40D005  movzx   ecx, byte ptr [edx]
40D008  movzx   esi, byte ptr [edx+2]
40D00C  movzx   ebx, byte ptr [edx+1]
40D010  dec     [ebp+var_4]
40D013  add     esi, ecx
40D015  add     ebx, esi
40D017  test    ebx, ebx
40D019  jle     short loc_40D08F
40D01B  movzx   esi, byte ptr [eax]
40D01E  imul    ecx, edi
40D021  shr     ecx, 8
40D024  add     esi, ecx
40D026  mov     ecx, esi
40D028  cmp     esi, 100h
40D02E  jb      short loc_40D035
40D030  mov     ecx, 0FFh
40D035  movzx   esi, byte ptr [eax+1]
40D039  mov     [eax], cl
40D03B  movzx   ecx, byte ptr [edx+1]
40D03F  imul    ecx, edi
40D042  shr     ecx, 8
40D045  add     ecx, esi
40D047  cmp     ecx, 100h
40D04D  jb      short loc_40D054
40D04F  mov     ecx, 0FFh
40D054  movzx   esi, byte ptr [eax+2]
40D058  mov     [eax+1], cl
40D05B  movzx   ecx, byte ptr [edx+2]
40D05F  imul    ecx, edi
40D062  shr     ecx, 8
40D065  add     ecx, esi
40D067  cmp     ecx, 100h
40D06D  jb      short loc_40D074
40D06F  mov     ecx, 0FFh
40D074  movzx   esi, byte ptr [eax+3]
40D078  add     esi, edi
40D07A  mov     [eax+2], cl
40D07D  mov     ecx, esi
40D07F  cmp     esi, 100h
40D085  jb      short loc_40D08C
40D087  mov     ecx, 0FFh
40D08C  mov     [eax+3], cl
40D08F  mov     ecx, [ebp+arg_0]
40D092  add     eax, [ecx+14h]
40D095  mov     ecx, [ebp+arg_4]
40D098  add     edx, [ecx+14h]
40D09B  cmp     [ebp+var_4], 0
40D09F  jnz     loc_40D005
40D0A5  mov     ecx, [ebp+var_10]
40D0A8  mov     edx, [ebp+var_C]
40D0AB  mov     eax, [ebp+var_8]
40D0AE  mov     esi, [ebp+arg_4]
40D0B1  mov     ebx, [ebp+arg_0]
40D0B4  add     eax, [ebx+4]
40D0B7  add     edx, [esi+4]
40D0BA  mov     [ebp+var_8], eax
40D0BD  mov     [ebp+var_C], edx
40D0C0  test    ecx, ecx
40D0C2  jnz     loc_40CFF3
40D0C8  pop     ebx
40D0C9  pop     esi
40D0CA  mov     esp, ebp
40D0CC  pop     ebp
40D0CD  retn

;==== 40D0D0 sub_40D0D0
40D0D0  push    ebx
40D0D1  mov     ebx, esp
40D0D3  sub     esp, 8
40D0D6  and     esp, 0FFFFFFF0h
40D0D9  add     esp, 4
40D0DC  push    ebp
40D0DD  mov     ebp, [ebx+4]
40D0E0  mov     [esp+0Ch+var_8], ebp
40D0E4  mov     ebp, esp
40D0E6  sub     esp, 858h
40D0EC  mov     eax, dword_4FB734
40D0F1  xor     eax, ebp
40D0F3  mov     [ebp-4], eax
40D0F6  cmp     dword ptr [ebx+0Ch], 100h
40D0FD  mov     edx, [ecx]
40D0FF  mov     eax, [ebx+8]
40D102  mov     ecx, [ecx+4]
40D105  mov     [ebp-81Ch], edx
40D10B  mov     edx, [eax]
40D10D  mov     [ebp-818h], edx
40D113  mov     edx, [eax+4]
40D116  mov     [ebp-834h], ecx
40D11C  mov     ecx, [eax+8]
40D11F  mov     eax, [eax+0Ch]
40D122  push    esi
40D123  pxor    xmm1, xmm1
40D127  push    edi
40D128  mov     [ebp-838h], edx
40D12E  mov     [ebp-824h], ecx
40D134  mov     [ebp-820h], eax
40D13A  movdqa  xmmword ptr [ebp-850h], xmm1
40D142  jnb     loc_40D228
40D148  xor     eax, eax
40D14A  lea     ecx, [ebp-810h]
40D150  mov     [ebp-814h], eax
40D156  mov     [ebp-828h], ecx
40D15C  mov     dword ptr [ebp-82Ch], 80h
40D166  jmp     short loc_40D170
40D170  shr     eax, 8
40D173  mov     [ebp-830h], eax
40D179  xor     eax, eax
40D17B  movzx   eax, ax
40D17E  xor     edx, edx
40D180  movd    xmm1, eax
40D184  movzx   edx, dx
40D187  xor     ecx, ecx
40D189  xor     edi, edi
40D18B  xor     esi, esi
40D18D  movd    xmm0, edx
40D191  movzx   ecx, cx
40D194  movzx   eax, di
40D197  movd    xmm4, eax
40D19B  mov     eax, [ebp-830h]
40D1A1  movzx   edx, si
40D1A4  movd    xmm2, ecx
40D1A8  movzx   ecx, ax
40D1AB  movd    xmm3, edx
40D1AF  movzx   edx, ax
40D1B2  movzx   eax, ax
40D1B5  movd    xmm5, ecx
40D1B9  mov     ecx, [ebp-828h]
40D1BF  movd    xmm7, eax
40D1C3  mov     eax, [ebp-814h]
40D1C9  add     eax, [ebx+0Ch]
40D1CC  movd    xmm6, edx
40D1D0  punpcklwd xmm4, xmm0
40D1D4  add     ecx, 10h
40D1D7  dec     dword ptr [ebp-82Ch]
40D1DD  punpcklwd xmm7, xmm3
40D1E1  punpcklwd xmm5, xmm1
40D1E5  punpcklwd xmm6, xmm2
40D1E9  punpcklwd xmm7, xmm5
40D1ED  punpcklwd xmm6, xmm4
40D1F1  punpcklwd xmm7, xmm6
40D1F5  movdqa  xmmword ptr [ecx-10h], xmm7
40D1FA  mov     [ebp-814h], eax
40D200  mov     [ebp-828h], ecx
40D206  jnz     loc_40D170
40D20C  movdqa  xmm1, xmmword ptr [ebp-850h]
40D214  mov     ecx, [ebp-824h]
40D21A  mov     eax, [ebp-820h]
40D220  lea     edx, [ebp-810h]
40D226  jmp     short loc_40D22D
40D228  mov     edx, offset qword_50B900
40D22D  mov     [ebp-814h], eax
40D233  test    eax, eax
40D235  jz      loc_40D33B
40D23B  mov     eax, ecx
40D23D  shr     eax, 1
40D23F  and     ecx, 1
40D242  mov     [ebp-820h], eax
40D248  mov     [ebp-824h], ecx
40D24E  jmp     short loc_40D256
40D250  mov     eax, [ebp-820h]
40D256  dec     dword ptr [ebp-814h]
40D25C  mov     edi, [ebp-81Ch]
40D262  mov     esi, [ebp-818h]
40D268  test    eax, eax
40D26A  jz      short loc_40D2D9
40D26C  movq    xmm0, qword ptr [esi]
40D270  movdqa  xmm2, xmm0
40D274  pcmpeqb xmm2, xmm1
40D278  pmovmskb ecx, xmm2
40D27C  and     ecx, 88h
40D282  dec     eax
40D283  cmp     cl, 88h
40D286  jz      short loc_40D2C9
40D288  movq    xmm2, qword ptr [edi]
40D28C  punpcklbw xmm0, xmm1
40D290  pextrw  ecx, xmm0, 3
40D295  sar     ecx, 1
40D297  add     ecx, ecx
40D299  movq    xmm4, qword ptr [edx+ecx*8]
40D29E  pextrw  ecx, xmm0, 7
40D2A3  sar     ecx, 1
40D2A5  movdqa  xmm3, xmm0
40D2A9  add     ecx, ecx
40D2AB  movdqa  xmm0, xmmword ptr [edx+ecx*8]
40D2B0  punpcklqdq xmm4, xmm0
40D2B4  pmullw  xmm3, xmm4
40D2B8  psraw   xmm3, 7
40D2BD  packuswb xmm3, xmm1
40D2C1  paddusb xmm2, xmm3
40D2C5  movq    qword ptr [edi], xmm2
40D2C9  add     edi, 8
40D2CC  add     esi, 8
40D2CF  test    eax, eax
40D2D1  jnz     short loc_40D26C
40D2D3  mov     ecx, [ebp-824h]
40D2D9  test    ecx, ecx
40D2DB  jz      short loc_40D316
40D2DD  mov     esi, [esi]
40D2DF  test    esi, 0FE000000h
40D2E5  jz      short loc_40D316
40D2E7  mov     eax, [edi]
40D2E9  movd    xmm2, esi
40D2ED  shr     esi, 19h
40D2F0  punpcklbw xmm2, xmm1
40D2F4  add     esi, esi
40D2F6  movdqa  xmm3, xmmword ptr [edx+esi*8]
40D2FB  pmullw  xmm2, xmm3
40D2FF  movd    xmm0, eax
40D303  psraw   xmm2, 7
40D308  packuswb xmm2, xmm1
40D30C  paddusb xmm0, xmm2
40D310  movd    eax, xmm0
40D314  mov     [edi], eax
40D316  mov     eax, [ebp-834h]
40D31C  add     [ebp-81Ch], eax
40D322  mov     eax, [ebp-838h]
40D328  add     [ebp-818h], eax
40D32E  cmp     dword ptr [ebp-814h], 0
40D335  jnz     loc_40D250
40D33B  mov     ecx, [ebp-4]
40D33E  pop     edi
40D33F  xor     ecx, ebp
40D341  pop     esi
40D342  call    sub_4ABDD5
40D347  mov     esp, ebp
40D349  pop     ebp
40D34A  mov     esp, ebx
40D34C  pop     ebx
40D34D  retn

;==== 40D350 sub_40D350
40D350  push    ebx
40D351  mov     ebx, esp
40D353  sub     esp, 8
40D356  and     esp, 0FFFFFFF0h
40D359  add     esp, 4
40D35C  push    ebp
40D35D  mov     ebp, [ebx+4]
40D360  mov     [esp+0Ch+var_8], ebp
40D364  mov     ebp, esp
40D366  sub     esp, 858h
40D36C  mov     eax, dword_4FB734
40D371  xor     eax, ebp
40D373  mov     [ebp-4], eax
40D376  push    esi
40D377  push    edi
40D378  mov     edx, [ecx]
40D37A  mov     eax, [ebx+8]
40D37D  mov     ecx, [ecx+4]
40D380  mov     [ebp-81Ch], edx
40D386  mov     edx, [eax]
40D388  mov     [ebp-820h], edx
40D38E  mov     edx, [eax+4]
40D391  mov     [ebp-830h], ecx
40D397  mov     ecx, [eax+0Ch]
40D39A  mov     [ebp-838h], edx
40D3A0  mov     edx, [eax+8]
40D3A3  mov     eax, [ebx+0Ch]
40D3A6  shr     eax, 1
40D3A8  mov     [ebp-814h], ecx
40D3AE  mov     [ebp-834h], eax
40D3B4  xor     eax, eax
40D3B6  lea     ecx, [ebp-810h]
40D3BC  pxor    xmm0, xmm0
40D3C0  mov     [ebp-818h], edx
40D3C6  movdqa  xmmword ptr [ebp-850h], xmm0
40D3CE  mov     [ebp-828h], eax
40D3D4  mov     [ebp-82Ch], ecx
40D3DA  mov     dword ptr [ebp-824h], 80h
40D3E4  xor     edx, edx
40D3E6  movzx   edx, dx
40D3E9  xor     ecx, ecx
40D3EB  movd    xmm0, edx
40D3EF  movzx   ecx, cx
40D3F2  xor     esi, esi
40D3F4  movzx   edx, si
40D3F7  movd    xmm1, ecx
40D3FB  xor     edi, edi
40D3FD  movd    xmm2, edx
40D401  movzx   edx, word ptr [ebp-834h]
40D408  shr     eax, 8
40D40B  movzx   ecx, di
40D40E  movd    xmm3, ecx
40D412  movzx   ecx, ax
40D415  movd    xmm4, edx
40D419  movzx   edx, ax
40D41C  movzx   eax, ax
40D41F  movd    xmm5, ecx
40D423  mov     ecx, [ebp-82Ch]
40D429  movd    xmm7, eax
40D42D  mov     eax, [ebp-828h]
40D433  add     eax, [ebx+0Ch]
40D436  movd    xmm6, edx
40D43A  punpcklwd xmm4, xmm0
40D43E  add     ecx, 10h
40D441  dec     dword ptr [ebp-824h]
40D447  punpcklwd xmm7, xmm3
40D44B  punpcklwd xmm5, xmm1
40D44F  punpcklwd xmm6, xmm2
40D453  punpcklwd xmm7, xmm5
40D457  punpcklwd xmm6, xmm4
40D45B  punpcklwd xmm7, xmm6
40D45F  movdqa  xmmword ptr [ecx-10h], xmm7
40D464  mov     [ebp-828h], eax
40D46A  mov     [ebp-82Ch], ecx
40D470  jnz     loc_40D3E4
40D476  mov     edx, [ebp-814h]
40D47C  test    edx, edx
40D47E  jz      loc_40D599
40D484  mov     eax, [ebp-818h]
40D48A  movdqa  xmm1, xmmword ptr [ebp-850h]
40D492  mov     ecx, eax
40D494  shr     ecx, 1
40D496  and     eax, 1
40D499  mov     [ebp-818h], ecx
40D49F  mov     ecx, [ebp-81Ch]
40D4A5  mov     [ebp-814h], eax
40D4AB  mov     eax, [ebp-818h]
40D4B1  mov     esi, [ebp-820h]
40D4B7  dec     edx
40D4B8  mov     [ebp-81Ch], edx
40D4BE  mov     edi, ecx
40D4C0  test    eax, eax
40D4C2  jz      short loc_40D539
40D4C4  movq    xmm0, qword ptr [esi]
40D4C8  movdqa  xmm2, xmm0
40D4CC  pcmpeqb xmm2, xmm1
40D4D0  pmovmskb edx, xmm2
40D4D4  and     edx, 88h
40D4DA  dec     eax
40D4DB  cmp     dl, 88h
40D4DE  jz      short loc_40D529
40D4E0  movq    xmm2, qword ptr [edi]
40D4E4  punpcklbw xmm0, xmm1
40D4E8  pextrw  edx, xmm0, 3
40D4ED  sar     edx, 1
40D4EF  add     edx, edx
40D4F1  movq    xmm4, qword ptr [ebp+edx*8-810h]
40D4FA  pextrw  edx, xmm0, 7
40D4FF  sar     edx, 1
40D501  movdqa  xmm3, xmm0
40D505  add     edx, edx
40D507  movdqa  xmm0, xmmword ptr [ebp+edx*8-810h]
40D510  punpcklqdq xmm4, xmm0
40D514  pmullw  xmm3, xmm4
40D518  psraw   xmm3, 7
40D51D  packuswb xmm3, xmm1
40D521  paddusb xmm2, xmm3
40D525  movq    qword ptr [edi], xmm2
40D529  add     edi, 8
40D52C  add     esi, 8
40D52F  test    eax, eax
40D531  jnz     short loc_40D4C4
40D533  mov     edx, [ebp-81Ch]
40D539  cmp     dword ptr [ebp-814h], 0
40D540  jz      short loc_40D57F
40D542  mov     esi, [esi]
40D544  test    esi, 0FE000000h
40D54A  jz      short loc_40D57F
40D54C  mov     eax, [edi]
40D54E  movd    xmm2, esi
40D552  shr     esi, 19h
40D555  punpcklbw xmm2, xmm1
40D559  add     esi, esi
40D55B  movdqa  xmm3, xmmword ptr [ebp+esi*8-810h]
40D564  pmullw  xmm2, xmm3
40D568  movd    xmm0, eax
40D56C  psraw   xmm2, 7
40D571  packuswb xmm2, xmm1
40D575  paddusb xmm0, xmm2
40D579  movd    eax, xmm0
40D57D  mov     [edi], eax
40D57F  mov     eax, [ebp-838h]
40D585  add     ecx, [ebp-830h]
40D58B  add     [ebp-820h], eax
40D591  test    edx, edx
40D593  jnz     loc_40D4AB
40D599  mov     ecx, [ebp-4]
40D59C  pop     edi
40D59D  xor     ecx, ebp
40D59F  pop     esi
40D5A0  call    sub_4ABDD5
40D5A5  mov     esp, ebp
40D5A7  pop     ebp
40D5A8  mov     esp, ebx
40D5AA  pop     ebx
40D5AB  retn

;==== 40D5B0 sub_40D5B0
40D5B0  push    ecx
40D5B1  test    edx, edx
40D5B3  jz      short loc_40D5CD
40D5B5  mov     eax, [esi+10h]
40D5B8  sub     eax, 2
40D5BB  jnz     short loc_40D5CD
40D5BD  mov     eax, [ecx+10h]
40D5C0  dec     eax
40D5C1  jnz     short loc_40D5CD
40D5C3  push    edx
40D5C4  push    esi
40D5C5  call    sub_40D5D0
40D5CA  add     esp, 8
40D5CD  pop     ecx
40D5CE  retn

;==== 40D5D0 sub_40D5D0
40D5D0  push    ebx
40D5D1  mov     ebx, esp
40D5D3  sub     esp, 8
40D5D6  and     esp, 0FFFFFFF0h
40D5D9  add     esp, 4
40D5DC  push    ebp
40D5DD  mov     ebp, [ebx+4]
40D5E0  mov     [esp+0Ch+var_8], ebp
40D5E4  mov     ebp, esp
40D5E6  sub     esp, 858h
40D5EC  mov     eax, dword_4FB734
40D5F1  xor     eax, ebp
40D5F3  mov     [ebp-4], eax
40D5F6  cmp     dword ptr [ebx+0Ch], 100h
40D5FD  mov     edx, [ecx]
40D5FF  mov     eax, [ebx+8]
40D602  mov     ecx, [ecx+4]
40D605  mov     [ebp-81Ch], edx
40D60B  mov     edx, [eax]
40D60D  mov     [ebp-818h], edx
40D613  mov     edx, [eax+4]
40D616  mov     [ebp-834h], ecx
40D61C  mov     ecx, [eax+8]
40D61F  mov     eax, [eax+0Ch]
40D622  push    esi
40D623  pxor    xmm1, xmm1
40D627  push    edi
40D628  mov     [ebp-838h], edx
40D62E  mov     [ebp-824h], ecx
40D634  mov     [ebp-820h], eax
40D63A  movdqa  xmmword ptr [ebp-850h], xmm1
40D642  jnb     loc_40D728
40D648  xor     eax, eax
40D64A  lea     ecx, [ebp-810h]
40D650  mov     [ebp-814h], eax
40D656  mov     [ebp-828h], ecx
40D65C  mov     dword ptr [ebp-82Ch], 80h
40D666  jmp     short loc_40D670
40D670  shr     eax, 8
40D673  mov     [ebp-830h], eax
40D679  xor     eax, eax
40D67B  movzx   eax, ax
40D67E  xor     edx, edx
40D680  movd    xmm1, eax
40D684  movzx   edx, dx
40D687  xor     ecx, ecx
40D689  xor     edi, edi
40D68B  xor     esi, esi
40D68D  movd    xmm0, edx
40D691  movzx   ecx, cx
40D694  movzx   eax, di
40D697  movd    xmm4, eax
40D69B  mov     eax, [ebp-830h]
40D6A1  movzx   edx, si
40D6A4  movd    xmm2, ecx
40D6A8  movzx   ecx, ax
40D6AB  movd    xmm3, edx
40D6AF  movzx   edx, ax
40D6B2  movzx   eax, ax
40D6B5  movd    xmm5, ecx
40D6B9  mov     ecx, [ebp-828h]
40D6BF  movd    xmm7, eax
40D6C3  mov     eax, [ebp-814h]
40D6C9  add     eax, [ebx+0Ch]
40D6CC  movd    xmm6, edx
40D6D0  punpcklwd xmm4, xmm0
40D6D4  add     ecx, 10h
40D6D7  dec     dword ptr [ebp-82Ch]
40D6DD  punpcklwd xmm7, xmm3
40D6E1  punpcklwd xmm5, xmm1
40D6E5  punpcklwd xmm6, xmm2
40D6E9  punpcklwd xmm7, xmm5
40D6ED  punpcklwd xmm6, xmm4
40D6F1  punpcklwd xmm7, xmm6
40D6F5  movdqa  xmmword ptr [ecx-10h], xmm7
40D6FA  mov     [ebp-814h], eax
40D700  mov     [ebp-828h], ecx
40D706  jnz     loc_40D670
40D70C  movdqa  xmm1, xmmword ptr [ebp-850h]
40D714  mov     ecx, [ebp-824h]
40D71A  mov     eax, [ebp-820h]
40D720  lea     edx, [ebp-810h]
40D726  jmp     short loc_40D72D
40D728  mov     edx, offset qword_50B900
40D72D  mov     [ebp-814h], eax
40D733  test    eax, eax
40D735  jz      loc_40D83B
40D73B  mov     eax, ecx
40D73D  shr     eax, 1
40D73F  and     ecx, 1
40D742  mov     [ebp-820h], eax
40D748  mov     [ebp-824h], ecx
40D74E  jmp     short loc_40D756
40D750  mov     eax, [ebp-820h]
40D756  dec     dword ptr [ebp-814h]
40D75C  mov     edi, [ebp-81Ch]
40D762  mov     esi, [ebp-818h]
40D768  test    eax, eax
40D76A  jz      short loc_40D7D9
40D76C  movq    xmm0, qword ptr [esi]
40D770  movdqa  xmm2, xmm0
40D774  pcmpeqb xmm2, xmm1
40D778  pmovmskb ecx, xmm2
40D77C  and     ecx, 88h
40D782  dec     eax
40D783  cmp     cl, 88h
40D786  jz      short loc_40D7C9
40D788  movq    xmm2, qword ptr [edi]
40D78C  punpcklbw xmm0, xmm1
40D790  pextrw  ecx, xmm0, 3
40D795  sar     ecx, 1
40D797  add     ecx, ecx
40D799  movq    xmm4, qword ptr [edx+ecx*8]
40D79E  pextrw  ecx, xmm0, 7
40D7A3  sar     ecx, 1
40D7A5  movdqa  xmm3, xmm0
40D7A9  add     ecx, ecx
40D7AB  movdqa  xmm0, xmmword ptr [edx+ecx*8]
40D7B0  punpcklqdq xmm4, xmm0
40D7B4  pmullw  xmm3, xmm4
40D7B8  psraw   xmm3, 7
40D7BD  packuswb xmm3, xmm1
40D7C1  psubusb xmm2, xmm3
40D7C5  movq    qword ptr [edi], xmm2
40D7C9  add     edi, 8
40D7CC  add     esi, 8
40D7CF  test    eax, eax
40D7D1  jnz     short loc_40D76C
40D7D3  mov     ecx, [ebp-824h]
40D7D9  test    ecx, ecx
40D7DB  jz      short loc_40D816
40D7DD  mov     esi, [esi]
40D7DF  test    esi, 0FE000000h
40D7E5  jz      short loc_40D816
40D7E7  mov     eax, [edi]
40D7E9  movd    xmm2, esi
40D7ED  shr     esi, 19h
40D7F0  punpcklbw xmm2, xmm1
40D7F4  add     esi, esi
40D7F6  movdqa  xmm3, xmmword ptr [edx+esi*8]
40D7FB  pmullw  xmm2, xmm3
40D7FF  movd    xmm0, eax
40D803  psraw   xmm2, 7
40D808  packuswb xmm2, xmm1
40D80C  psubusb xmm0, xmm2
40D810  movd    eax, xmm0
40D814  mov     [edi], eax
40D816  mov     eax, [ebp-834h]
40D81C  add     [ebp-81Ch], eax
40D822  mov     eax, [ebp-838h]
40D828  add     [ebp-818h], eax
40D82E  cmp     dword ptr [ebp-814h], 0
40D835  jnz     loc_40D750
40D83B  mov     ecx, [ebp-4]
40D83E  pop     edi
40D83F  xor     ecx, ebp
40D841  pop     esi
40D842  call    sub_4ABDD5
40D847  mov     esp, ebp
40D849  pop     ebp
40D84A  mov     esp, ebx
40D84C  pop     ebx
40D84D  retn

;==== 40D850 sub_40D850
40D850  push    ecx
40D851  test    esi, esi
40D853  jz      short loc_40D8A5
40D855  mov     edx, [ecx+10h]
40D858  dec     edx
40D859  jz      short loc_40D888
40D85B  dec     edx
40D85C  jz      short loc_40D874
40D85E  dec     edx
40D85F  jnz     short loc_40D8A5
40D861  cmp     dword ptr [eax+10h], 3
40D865  jnz     short loc_40D8A5
40D867  push    esi
40D868  mov     edx, eax
40D86A  call    sub_40DDF0
40D86F  add     esp, 4
40D872  pop     ecx
40D873  retn
40D874  mov     edx, [eax+10h]
40D877  dec     edx
40D878  jnz     short loc_40D8A5
40D87A  push    esi
40D87B  push    ecx
40D87C  mov     ecx, eax
40D87E  call    sub_40DB90
40D883  add     esp, 8
40D886  pop     ecx
40D887  retn
40D888  mov     edx, [eax+10h]
40D88B  dec     edx
40D88C  jz      short loc_40D89C
40D88E  dec     edx
40D88F  jnz     short loc_40D8A5
40D891  push    esi
40D892  call    sub_40DA20
40D897  add     esp, 4
40D89A  pop     ecx
40D89B  retn
40D89C  push    esi
40D89D  call    sub_40D8B0
40D8A2  add     esp, 4
40D8A5  pop     ecx
40D8A6  retn

;==== 40D8B0 sub_40D8B0
40D8B0  push    ebx
40D8B1  mov     ebx, esp
40D8B3  sub     esp, 8
40D8B6  and     esp, 0FFFFFFF0h
40D8B9  add     esp, 4
40D8BC  push    ebp
40D8BD  mov     ebp, [ebx+4]
40D8C0  mov     [esp+0Ch+var_8], ebp
40D8C4  mov     ebp, esp
40D8C6  sub     esp, 38h
40D8C9  mov     edx, [ecx]
40D8CB  mov     [ebp-4], edx
40D8CE  mov     edx, [ecx+4]
40D8D1  push    esi
40D8D2  mov     [ebp-14h], edx
40D8D5  xor     edx, edx
40D8D7  push    edi
40D8D8  mov     edi, [eax]
40D8DA  mov     eax, [eax+4]
40D8DD  mov     [ebp-10h], eax
40D8E0  mov     eax, [ecx+8]
40D8E3  movzx   edx, dx
40D8E6  mov     [ebp-8], eax
40D8E9  mov     eax, [ebx+8]
40D8EC  shr     eax, 1
40D8EE  pxor    xmm0, xmm0
40D8F2  movdqa  xmmword ptr [ebp-30h], xmm0
40D8F7  movd    xmm0, edx
40D8FB  movzx   edx, ax
40D8FE  movd    xmm1, edx
40D902  movzx   edx, ax
40D905  movd    xmm3, edx
40D909  movzx   edx, ax
40D90C  xor     esi, esi
40D90E  movd    xmm4, edx
40D912  movzx   edx, si
40D915  mov     esi, [ecx+0Ch]
40D918  movd    xmm5, edx
40D91C  movzx   edx, ax
40D91F  movd    xmm6, edx
40D923  movzx   edx, ax
40D926  movzx   eax, ax
40D929  movd    xmm7, edx
40D92D  movd    xmm2, eax
40D931  punpcklwd xmm6, xmm1
40D935  punpcklwd xmm5, xmm0
40D939  punpcklwd xmm2, xmm4
40D93D  punpcklwd xmm7, xmm3
40D941  movdqa  xmm3, ds:xmmword_4E42C0
40D949  punpcklwd xmm2, xmm6
40D94D  punpcklwd xmm7, xmm5
40D951  punpcklwd xmm2, xmm7
40D955  test    esi, esi
40D957  jz      loc_40DA09
40D95D  mov     eax, [ebp-8]
40D960  movdqa  xmm1, xmmword ptr [ebp-30h]
40D965  mov     ecx, eax
40D967  shr     ecx, 1
40D969  and     eax, 1
40D96C  mov     [ebp-8], ecx
40D96F  mov     [ebp-0Ch], eax
40D972  jmp     short loc_40D977
40D974  mov     ecx, [ebp-8]
40D977  mov     edx, [ebp-4]
40D97A  dec     esi
40D97B  mov     eax, edi
40D97D  test    ecx, ecx
40D97F  jz      short loc_40D9BB
40D981  movq    xmm0, qword ptr [eax]
40D985  movq    xmm4, qword ptr [edx]
40D989  punpcklbw xmm4, xmm1
40D98D  psubw   xmm4, xmm3
40D991  punpcklbw xmm0, xmm1
40D995  pmullw  xmm4, xmm2
40D999  movdqa  xmm5, xmm0
40D99D  psllw   xmm5, 1
40D9A2  pmulhw  xmm4, xmm5
40D9A6  paddw   xmm4, xmm0
40D9AA  packuswb xmm4, xmm1
40D9AE  movq    qword ptr [eax], xmm4
40D9B2  add     eax, 8
40D9B5  add     edx, 8
40D9B8  dec     ecx
40D9B9  jnz     short loc_40D981
40D9BB  cmp     dword ptr [ebp-0Ch], 0
40D9BF  jz      short loc_40D9F8
40D9C1  mov     ecx, [eax]
40D9C3  mov     edx, [edx]
40D9C5  movd    xmm0, ecx
40D9C9  punpcklbw xmm0, xmm1
40D9CD  movd    xmm4, edx
40D9D1  punpcklbw xmm4, xmm1
40D9D5  psubw   xmm4, xmm3
40D9D9  pmullw  xmm4, xmm2
40D9DD  movdqa  xmm5, xmm0
40D9E1  psllw   xmm5, 1
40D9E6  pmulhw  xmm4, xmm5
40D9EA  paddw   xmm4, xmm0
40D9EE  packuswb xmm4, xmm1
40D9F2  movd    ecx, xmm4
40D9F6  mov     [eax], ecx
40D9F8  mov     edx, [ebp-14h]
40D9FB  add     edi, [ebp-10h]
40D9FE  add     [ebp-4], edx
40DA01  test    esi, esi
40DA03  jnz     loc_40D974
40DA09  pop     edi
40DA0A  pop     esi
40DA0B  mov     esp, ebp
40DA0D  pop     ebp
40DA0E  mov     esp, ebx
40DA10  pop     ebx
40DA11  retn

;==== 40DA20 sub_40DA20
40DA20  push    ebx
40DA21  mov     ebx, esp
40DA23  sub     esp, 8
40DA26  and     esp, 0FFFFFFF0h
40DA29  add     esp, 4
40DA2C  push    ebp
40DA2D  mov     ebp, [ebx+4]
40DA30  mov     [esp+0Ch+var_8], ebp
40DA34  mov     ebp, esp
40DA36  sub     esp, 38h
40DA39  mov     edx, [ecx]
40DA3B  mov     [ebp-4], edx
40DA3E  mov     edx, [ecx+4]
40DA41  push    esi
40DA42  mov     [ebp-14h], edx
40DA45  xor     edx, edx
40DA47  push    edi
40DA48  mov     edi, [eax]
40DA4A  mov     eax, [eax+4]
40DA4D  mov     [ebp-10h], eax
40DA50  mov     eax, [ecx+8]
40DA53  movzx   edx, dx
40DA56  mov     [ebp-8], eax
40DA59  mov     eax, [ebx+8]
40DA5C  shr     eax, 1
40DA5E  pxor    xmm0, xmm0
40DA62  movdqa  xmmword ptr [ebp-30h], xmm0
40DA67  movd    xmm0, edx
40DA6B  movzx   edx, ax
40DA6E  movd    xmm1, edx
40DA72  movzx   edx, ax
40DA75  movd    xmm3, edx
40DA79  movzx   edx, ax
40DA7C  xor     esi, esi
40DA7E  movd    xmm4, edx
40DA82  movzx   edx, si
40DA85  mov     esi, [ecx+0Ch]
40DA88  movd    xmm5, edx
40DA8C  movzx   edx, ax
40DA8F  movd    xmm6, edx
40DA93  movzx   edx, ax
40DA96  movzx   eax, ax
40DA99  movd    xmm7, edx
40DA9D  movd    xmm2, eax
40DAA1  punpcklwd xmm6, xmm1
40DAA5  punpcklwd xmm5, xmm0
40DAA9  punpcklwd xmm2, xmm4
40DAAD  punpcklwd xmm7, xmm3
40DAB1  punpcklwd xmm2, xmm6
40DAB5  punpcklwd xmm7, xmm5
40DAB9  punpcklwd xmm2, xmm7
40DABD  test    esi, esi
40DABF  jz      loc_40DB82
40DAC5  mov     eax, [ebp-8]
40DAC8  movdqa  xmm1, xmmword ptr [ebp-30h]
40DACD  mov     ecx, eax
40DACF  shr     ecx, 1
40DAD1  and     eax, 1
40DAD4  mov     [ebp-8], ecx
40DAD7  mov     [ebp-0Ch], eax
40DADA  jmp     short loc_40DAE3
40DAE0  mov     ecx, [ebp-8]
40DAE3  mov     edx, [ebp-4]
40DAE6  dec     esi
40DAE7  mov     eax, edi
40DAE9  test    ecx, ecx
40DAEB  jz      short loc_40DB2F
40DAED  lea     ecx, [ecx+0]
40DAF0  movq    xmm0, qword ptr [eax]
40DAF4  movq    xmm4, qword ptr [edx]
40DAF8  punpcklbw xmm0, xmm1
40DAFC  movdqa  xmm3, xmm0
40DB00  punpcklbw xmm4, xmm1
40DB04  pmullw  xmm3, xmm4
40DB08  psrlw   xmm3, 8
40DB0D  psubw   xmm3, xmm0
40DB11  pmullw  xmm3, xmm2
40DB15  psraw   xmm3, 7
40DB1A  paddw   xmm3, xmm0
40DB1E  packuswb xmm3, xmm1
40DB22  movq    qword ptr [eax], xmm3
40DB26  add     eax, 8
40DB29  add     edx, 8
40DB2C  dec     ecx
40DB2D  jnz     short loc_40DAF0
40DB2F  cmp     dword ptr [ebp-0Ch], 0
40DB33  jz      short loc_40DB71
40DB35  mov     ecx, [eax]
40DB37  mov     edx, [edx]
40DB39  movd    xmm0, ecx
40DB3D  punpcklbw xmm0, xmm1
40DB41  movd    xmm4, edx
40DB45  movdqa  xmm3, xmm0
40DB49  punpcklbw xmm4, xmm1
40DB4D  pmullw  xmm3, xmm4
40DB51  psrlw   xmm3, 8
40DB56  psubw   xmm3, xmm0
40DB5A  pmullw  xmm3, xmm2
40DB5E  psraw   xmm3, 7
40DB63  paddw   xmm3, xmm0
40DB67  packuswb xmm3, xmm1
40DB6B  movd    ecx, xmm3
40DB6F  mov     [eax], ecx
40DB71  mov     edx, [ebp-14h]
40DB74  add     edi, [ebp-10h]
40DB77  add     [ebp-4], edx
40DB7A  test    esi, esi
40DB7C  jnz     loc_40DAE0
40DB82  pop     edi
40DB83  pop     esi
40DB84  mov     esp, ebp
40DB86  pop     ebp
40DB87  mov     esp, ebx
40DB89  pop     ebx
40DB8A  retn

;==== 40DB90 sub_40DB90
40DB90  push    ebx
40DB91  mov     ebx, esp
40DB93  sub     esp, 8
40DB96  and     esp, 0FFFFFFF0h
40DB99  add     esp, 4
40DB9C  push    ebp
40DB9D  mov     ebp, [ebx+4]
40DBA0  mov     [esp+0Ch+var_8], ebp
40DBA4  mov     ebp, esp
40DBA6  sub     esp, 848h
40DBAC  mov     eax, dword_4FB734
40DBB1  xor     eax, ebp
40DBB3  mov     [ebp-4], eax
40DBB6  mov     edx, [ecx]
40DBB8  mov     eax, [ebx+8]
40DBBB  mov     ecx, [ecx+4]
40DBBE  mov     [ebp-814h], edx
40DBC4  mov     edx, [eax]
40DBC6  mov     [ebp-81Ch], edx
40DBCC  mov     edx, [eax+4]
40DBCF  mov     [ebp-82Ch], edx
40DBD5  mov     edx, [eax+0Ch]
40DBD8  push    esi
40DBD9  mov     [ebp-828h], ecx
40DBDF  mov     ecx, [eax+8]
40DBE2  mov     [ebp-820h], edx
40DBE8  pxor    xmm0, xmm0
40DBEC  push    edi
40DBED  mov     [ebp-818h], ecx
40DBF3  movdqa  xmmword ptr [ebp-840h], xmm0
40DBFB  xor     edx, edx
40DBFD  lea     esi, [ebp-810h]
40DC03  mov     dword ptr [ebp-824h], 80h
40DC0D  lea     ecx, [ecx+0]
40DC10  xor     ecx, ecx
40DC12  movzx   ecx, cx
40DC15  movd    xmm0, ecx
40DC19  mov     eax, edx
40DC1B  shr     eax, 8
40DC1E  movzx   ecx, ax
40DC21  movd    xmm1, ecx
40DC25  movzx   ecx, ax
40DC28  movd    xmm2, ecx
40DC2C  movzx   ecx, ax
40DC2F  add     edx, [ebx+0Ch]
40DC32  movd    xmm3, ecx
40DC36  xor     edi, edi
40DC38  movzx   ecx, di
40DC3B  movd    xmm4, ecx
40DC3F  movzx   ecx, ax
40DC42  movd    xmm5, ecx
40DC46  movzx   ecx, ax
40DC49  movzx   eax, ax
40DC4C  movd    xmm6, ecx
40DC50  movd    xmm7, eax
40DC54  punpcklwd xmm5, xmm1
40DC58  punpcklwd xmm4, xmm0
40DC5C  add     esi, 10h
40DC5F  dec     dword ptr [ebp-824h]
40DC65  punpcklwd xmm7, xmm3
40DC69  punpcklwd xmm6, xmm2
40DC6D  punpcklwd xmm7, xmm5
40DC71  punpcklwd xmm6, xmm4
40DC75  punpcklwd xmm7, xmm6
40DC79  movdqa  xmmword ptr [esi-10h], xmm7
40DC7E  jnz     short loc_40DC10
40DC80  mov     edx, [ebp-820h]
40DC86  test    edx, edx
40DC88  jz      loc_40DDD5
40DC8E  mov     eax, [ebp-818h]
40DC94  movdqa  xmm1, xmmword ptr [ebp-840h]
40DC9C  mov     ecx, eax
40DC9E  shr     ecx, 1
40DCA0  and     eax, 1
40DCA3  mov     [ebp-818h], ecx
40DCA9  mov     ecx, [ebp-814h]
40DCAF  mov     [ebp-820h], eax
40DCB5  mov     eax, [ebp-818h]
40DCBB  mov     esi, [ebp-81Ch]
40DCC1  dec     edx
40DCC2  mov     [ebp-814h], edx
40DCC8  mov     edi, ecx
40DCCA  test    eax, eax
40DCCC  jz      loc_40DD60
40DCD2  movq    xmm2, qword ptr [esi]
40DCD6  movdqa  xmm0, xmm2
40DCDA  pcmpeqb xmm0, xmm1
40DCDE  pmovmskb edx, xmm0
40DCE2  and     edx, 88h
40DCE8  dec     eax
40DCE9  cmp     dl, 88h
40DCEC  jz      short loc_40DD4C
40DCEE  movq    xmm0, qword ptr [edi]
40DCF2  punpcklbw xmm2, xmm1
40DCF6  pextrw  edx, xmm2, 3
40DCFB  sar     edx, 1
40DCFD  punpcklbw xmm0, xmm1
40DD01  add     edx, edx
40DD03  movdqa  xmm4, xmm2
40DD07  psllw   xmm4, 8
40DD0C  movdqa  xmm3, xmm0
40DD10  pmulhuw xmm3, xmm4
40DD14  movq    xmm4, qword ptr [ebp+edx*8-810h]
40DD1D  pextrw  edx, xmm2, 7
40DD22  sar     edx, 1
40DD24  psubw   xmm3, xmm0
40DD28  add     edx, edx
40DD2A  movdqa  xmm2, xmmword ptr [ebp+edx*8-810h]
40DD33  punpcklqdq xmm4, xmm2
40DD37  pmullw  xmm3, xmm4
40DD3B  psraw   xmm3, 7
40DD40  paddw   xmm3, xmm0
40DD44  packuswb xmm3, xmm1
40DD48  movq    qword ptr [edi], xmm3
40DD4C  add     edi, 8
40DD4F  add     esi, 8
40DD52  test    eax, eax
40DD54  jnz     loc_40DCD2
40DD5A  mov     edx, [ebp-814h]
40DD60  cmp     dword ptr [ebp-820h], 0
40DD67  jz      short loc_40DDBB
40DD69  mov     esi, [esi]
40DD6B  test    esi, 0FE000000h
40DD71  jz      short loc_40DDBB
40DD73  mov     eax, [edi]
40DD75  movd    xmm3, esi
40DD79  movd    xmm0, eax
40DD7D  punpcklbw xmm0, xmm1
40DD81  punpcklbw xmm3, xmm1
40DD85  shr     esi, 19h
40DD88  psllw   xmm3, 8
40DD8D  movdqa  xmm2, xmm0
40DD91  pmulhuw xmm2, xmm3
40DD95  psubw   xmm2, xmm0
40DD99  add     esi, esi
40DD9B  movdqa  xmm3, xmmword ptr [ebp+esi*8-810h]
40DDA4  pmullw  xmm2, xmm3
40DDA8  psraw   xmm2, 7
40DDAD  paddw   xmm2, xmm0
40DDB1  packuswb xmm2, xmm1
40DDB5  movd    eax, xmm2
40DDB9  mov     [edi], eax
40DDBB  mov     eax, [ebp-82Ch]
40DDC1  add     ecx, [ebp-828h]
40DDC7  add     [ebp-81Ch], eax
40DDCD  test    edx, edx
40DDCF  jnz     loc_40DCB5
40DDD5  mov     ecx, [ebp-4]
40DDD8  pop     edi
40DDD9  xor     ecx, ebp
40DDDB  pop     esi
40DDDC  call    sub_4ABDD5
40DDE1  mov     esp, ebp
40DDE3  pop     ebp
40DDE4  mov     esp, ebx
40DDE6  pop     ebx
40DDE7  retn

;==== 40DDF0 sub_40DDF0
40DDF0  push    ebp
40DDF1  mov     ebp, esp
40DDF3  sub     esp, 18h
40DDF6  mov     eax, [edx]
40DDF8  mov     edx, [edx+4]
40DDFB  mov     [ebp+var_14], edx
40DDFE  mov     edx, [ecx+4]
40DE01  push    edi
40DE02  mov     edi, [ecx]
40DE04  mov     [ebp+var_18], edx
40DE07  mov     edx, [ecx+8]
40DE0A  mov     ecx, [ecx+0Ch]
40DE0D  mov     [ebp+var_4], eax
40DE10  mov     [ebp+var_8], edi
40DE13  mov     [ebp+var_C], edx
40DE16  test    ecx, ecx
40DE18  jz      short loc_40DE75
40DE1A  push    ebx
40DE1B  push    esi
40DE1C  lea     esp, [esp+0]
40DE20  dec     ecx
40DE21  mov     [ebp+var_10], ecx
40DE24  test    edx, edx
40DE26  jz      short loc_40DE60
40DE28  sub     edi, eax
40DE2A  lea     ebx, [ebx+0]
40DE30  mov     cl, [eax]
40DE32  movzx   ebx, byte ptr [edi+eax]
40DE36  movzx   esi, cl
40DE39  imul    ebx, esi
40DE3C  inc     ebx
40DE3D  imul    ebx, 101h
40DE43  sar     ebx, 10h
40DE46  sub     ebx, esi
40DE48  imul    ebx, [ebp+arg_0]
40DE4C  shr     ebx, 8
40DE4F  add     bl, cl
40DE51  mov     [eax], bl
40DE53  inc     eax
40DE54  dec     edx
40DE55  jnz     short loc_40DE30
40DE57  mov     eax, [ebp+var_4]
40DE5A  mov     edx, [ebp+var_C]
40DE5D  mov     ecx, [ebp+var_10]
40DE60  mov     edi, [ebp+var_8]
40DE63  add     eax, [ebp+var_14]
40DE66  add     edi, [ebp+var_18]
40DE69  mov     [ebp+var_4], eax
40DE6C  mov     [ebp+var_8], edi
40DE6F  test    ecx, ecx
40DE71  jnz     short loc_40DE20
40DE73  pop     esi
40DE74  pop     ebx
40DE75  pop     edi
40DE76  mov     esp, ebp
40DE78  pop     ebp
40DE79  retn

;==== 40DE80 sub_40DE80
40DE80  push    ebp
40DE81  mov     ebp, esp
40DE83  mov     edx, [ebp+arg_0]
40DE86  push    esi
40DE87  test    ecx, ecx
40DE89  jz      short loc_40DEFE
40DE8B  mov     esi, [eax+10h]
40DE8E  sub     esi, 0
40DE91  jz      short loc_40DEE9
40DE93  dec     esi
40DE94  jz      short loc_40DEC0
40DE96  dec     esi
40DE97  jnz     short loc_40DEBD
40DE99  mov     esi, [edx+10h]
40DE9C  dec     esi
40DE9D  jz      short loc_40DEB0
40DE9F  dec     esi
40DEA0  jnz     short loc_40DEBD
40DEA2  push    ecx
40DEA3  push    eax
40DEA4  push    edx
40DEA5  call    sub_40E220
40DEAA  add     esp, 0Ch
40DEAD  pop     esi
40DEAE  pop     ebp
40DEAF  retn
40DEB0  push    ecx
40DEB1  mov     ecx, eax
40DEB3  mov     eax, edx
40DEB5  call    sub_40E040
40DEBA  add     esp, 4
40DEBD  pop     esi
40DEBE  pop     ebp
40DEBF  retn
40DEC0  mov     esi, [edx+10h]
40DEC3  dec     esi
40DEC4  jnz     short loc_40DEBD
40DEC6  cmp     ecx, 100h
40DECC  jnb     short loc_40DEDE
40DECE  push    ecx
40DECF  mov     ecx, eax
40DED1  mov     eax, edx
40DED3  call    sub_40DF10
40DED8  add     esp, 4
40DEDB  pop     esi
40DEDC  pop     ebp
40DEDD  retn
40DEDE  xor     ecx, ecx
40DEE0  mov     eax, edx
40DEE2  pop     esi
40DEE3  pop     ebp
40DEE4  jmp     sub_40A610
40DEE9  cmp     dword ptr [edx+10h], 0
40DEED  jnz     short loc_40DEBD
40DEEF  push    eax
40DEF0  push    edx
40DEF1  xor     eax, eax
40DEF3  call    sub_411600
40DEF8  add     esp, 8
40DEFB  pop     esi
40DEFC  pop     ebp
40DEFD  retn
40DEFE  mov     ecx, edx
40DF00  pop     esi
40DF01  pop     ebp
40DF02  jmp     sub_40AF70

;==== 40DF10 sub_40DF10
40DF10  push    ebx
40DF11  mov     ebx, esp
40DF13  sub     esp, 8
40DF16  and     esp, 0FFFFFFF0h
40DF19  add     esp, 4
40DF1C  push    ebp
40DF1D  mov     ebp, [ebx+4]
40DF20  mov     [esp+0Ch+var_8], ebp
40DF24  mov     ebp, esp
40DF26  sub     esp, 38h
40DF29  mov     edx, [ecx]
40DF2B  mov     [ebp-4], edx
40DF2E  mov     edx, [ecx+4]
40DF31  push    esi
40DF32  mov     [ebp-14h], edx
40DF35  xor     edx, edx
40DF37  push    edi
40DF38  mov     edi, [eax]
40DF3A  mov     eax, [eax+4]
40DF3D  mov     [ebp-10h], eax
40DF40  mov     eax, [ecx+8]
40DF43  movzx   edx, dx
40DF46  mov     [ebp-8], eax
40DF49  mov     eax, 100h
40DF4E  sub     eax, [ebx+8]
40DF51  pxor    xmm0, xmm0
40DF55  movdqa  xmmword ptr [ebp-30h], xmm0
40DF5A  movd    xmm0, edx
40DF5E  movzx   edx, ax
40DF61  movd    xmm2, edx
40DF65  movzx   edx, ax
40DF68  movd    xmm3, edx
40DF6C  movzx   edx, ax
40DF6F  xor     esi, esi
40DF71  movd    xmm4, edx
40DF75  movzx   edx, si
40DF78  mov     esi, [ecx+0Ch]
40DF7B  movd    xmm5, edx
40DF7F  movzx   edx, ax
40DF82  movd    xmm6, edx
40DF86  movzx   edx, ax
40DF89  movzx   eax, ax
40DF8C  movd    xmm7, edx
40DF90  movd    xmm1, eax
40DF94  punpcklwd xmm6, xmm2
40DF98  punpcklwd xmm5, xmm0
40DF9C  punpcklwd xmm1, xmm4
40DFA0  punpcklwd xmm7, xmm3
40DFA4  punpcklwd xmm1, xmm6
40DFA8  punpcklwd xmm7, xmm5
40DFAC  punpcklwd xmm1, xmm7
40DFB0  test    esi, esi
40DFB2  jz      short loc_40E032
40DFB4  mov     ecx, [ebp-8]
40DFB7  movdqa  xmm0, xmmword ptr [ebp-30h]
40DFBC  mov     eax, ecx
40DFBE  shr     eax, 1
40DFC0  and     ecx, 1
40DFC3  mov     [ebp-8], eax
40DFC6  mov     [ebp-0Ch], ecx
40DFC9  jmp     short loc_40DFD3
40DFCB  jmp     short loc_40DFD0
40DFD0  mov     eax, [ebp-8]
40DFD3  mov     edx, [ebp-4]
40DFD6  dec     esi
40DFD7  mov     ecx, edi
40DFD9  test    eax, eax
40DFDB  jz      short loc_40E002
40DFDD  lea     ecx, [ecx+0]
40DFE0  movq    xmm2, qword ptr [edx]
40DFE4  punpcklbw xmm2, xmm0
40DFE8  pmullw  xmm2, xmm1
40DFEC  psrlw   xmm2, 8
40DFF1  packuswb xmm2, xmm0
40DFF5  movq    qword ptr [ecx], xmm2
40DFF9  add     ecx, 8
40DFFC  add     edx, 8
40DFFF  dec     eax
40E000  jnz     short loc_40DFE0
40E002  cmp     dword ptr [ebp-0Ch], 0
40E006  jz      short loc_40E025
40E008  mov     edx, [edx]
40E00A  movd    xmm2, edx
40E00E  punpcklbw xmm2, xmm0
40E012  pmullw  xmm2, xmm1
40E016  psrlw   xmm2, 8
40E01B  packuswb xmm2, xmm0
40E01F  movd    eax, xmm2
40E023  mov     [ecx], eax
40E025  mov     ecx, [ebp-14h]
40E028  add     edi, [ebp-10h]
40E02B  add     [ebp-4], ecx
40E02E  test    esi, esi
40E030  jnz     short loc_40DFD0
40E032  pop     edi
40E033  pop     esi
40E034  mov     esp, ebp
40E036  pop     ebp
40E037  mov     esp, ebx
40E039  pop     ebx
40E03A  retn

;==== 40E040 sub_40E040
40E040  push    ebx
40E041  mov     ebx, esp
40E043  sub     esp, 8
40E046  and     esp, 0FFFFFFF0h
40E049  add     esp, 4
40E04C  push    ebp
40E04D  mov     ebp, [ebx+4]
40E050  mov     [esp+0Ch+var_8], ebp
40E054  mov     ebp, esp
40E056  sub     esp, 38h
40E059  push    esi
40E05A  push    edi
40E05B  mov     edx, [eax]
40E05D  mov     eax, [eax+4]
40E060  mov     [ebp-4], edx
40E063  mov     edx, [ecx]
40E065  mov     [ebp-8], edx
40E068  mov     edx, [ecx+4]
40E06B  mov     [ebp-18h], edx
40E06E  xor     edx, edx
40E070  movzx   edx, dx
40E073  mov     [ebp-14h], eax
40E076  mov     eax, 100h
40E07B  sub     eax, [ebx+8]
40E07E  pxor    xmm0, xmm0
40E082  movdqa  xmmword ptr [ebp-30h], xmm0
40E087  movd    xmm0, edx
40E08B  movzx   edx, ax
40E08E  movd    xmm1, edx
40E092  movzx   edx, ax
40E095  movd    xmm2, edx
40E099  movzx   edx, ax
40E09C  mov     esi, [ecx+8]
40E09F  mov     ecx, [ecx+0Ch]
40E0A2  movd    xmm4, edx
40E0A6  xor     edi, edi
40E0A8  movzx   edx, di
40E0AB  movd    xmm5, edx
40E0AF  movzx   edx, ax
40E0B2  movd    xmm6, edx
40E0B6  movzx   edx, ax
40E0B9  movzx   eax, ax
40E0BC  movd    xmm7, edx
40E0C0  movd    xmm3, eax
40E0C4  punpcklwd xmm6, xmm1
40E0C8  punpcklwd xmm5, xmm0
40E0CC  punpcklwd xmm3, xmm4
40E0D0  punpcklwd xmm7, xmm2
40E0D4  punpcklwd xmm3, xmm6
40E0D8  punpcklwd xmm7, xmm5
40E0DC  punpcklwd xmm3, xmm7
40E0E0  test    ecx, ecx
40E0E2  jz      loc_40E20A
40E0E8  movdqa  xmm1, xmmword ptr [ebp-30h]
40E0ED  mov     edi, [ebp-4]
40E0F0  mov     eax, esi
40E0F2  shr     eax, 1
40E0F4  and     esi, 1
40E0F7  mov     [ebp-10h], esi
40E0FA  mov     esi, [ebp-8]
40E0FD  mov     [ebp-0Ch], eax
40E100  dec     ecx
40E101  test    eax, eax
40E103  jz      loc_40E196
40E109  movq    xmm2, qword ptr [esi]
40E10D  movdqa  xmm0, xmm2
40E111  pcmpeqb xmm0, xmm1
40E115  pmovmskb edx, xmm0
40E119  and     edx, 88h
40E11F  dec     eax
40E120  cmp     dl, 88h
40E123  jz      short loc_40E185
40E125  movq    xmm0, qword ptr [edi]
40E129  punpcklbw xmm2, xmm1
40E12D  pextrw  edx, xmm2, 3
40E132  sar     edx, 1
40E134  shl     edx, 4
40E137  punpcklbw xmm0, xmm1
40E13B  movdqa  xmm5, xmm2
40E13F  pmullw  xmm5, xmm3
40E143  psrlw   xmm5, 8
40E148  psubw   xmm5, xmm0
40E14C  movdqa  xmm4, xmm0
40E150  movq    xmm0, qword_50B0F0[edx]
40E158  pextrw  edx, xmm2, 7
40E15D  sar     edx, 1
40E15F  shl     edx, 4
40E162  add     edx, offset qword_50B0F0
40E168  movdqa  xmm2, xmmword ptr [edx]
40E16C  punpcklqdq xmm0, xmm2
40E170  pmullw  xmm5, xmm0
40E174  psraw   xmm5, 7
40E179  paddw   xmm4, xmm5
40E17D  packuswb xmm4, xmm1
40E181  movq    qword ptr [edi], xmm4
40E185  add     edi, 8
40E188  add     esi, 8
40E18B  test    eax, eax
40E18D  jnz     loc_40E109
40E193  mov     eax, [ebp-0Ch]
40E196  cmp     dword ptr [ebp-10h], 0
40E19A  jz      short loc_40E1F0
40E19C  mov     esi, [esi]
40E19E  test    esi, 0FE000000h
40E1A4  jz      short loc_40E1F0
40E1A6  mov     edx, [edi]
40E1A8  movd    xmm4, esi
40E1AC  punpcklbw xmm4, xmm1
40E1B0  shr     esi, 19h
40E1B3  pmullw  xmm4, xmm3
40E1B7  movd    xmm0, edx
40E1BB  punpcklbw xmm0, xmm1
40E1BF  psrlw   xmm4, 8
40E1C4  shl     esi, 4
40E1C7  psubw   xmm4, xmm0
40E1CB  movdqa  xmm2, xmm0
40E1CF  add     esi, offset qword_50B0F0
40E1D5  movdqa  xmm0, xmmword ptr [esi]
40E1D9  pmullw  xmm4, xmm0
40E1DD  psraw   xmm4, 7
40E1E2  paddw   xmm2, xmm4
40E1E6  packuswb xmm2, xmm1
40E1EA  movd    edx, xmm2
40E1EE  mov     [edi], edx
40E1F0  mov     edi, [ebp-4]
40E1F3  mov     esi, [ebp-8]
40E1F6  add     edi, [ebp-14h]
40E1F9  add     esi, [ebp-18h]
40E1FC  mov     [ebp-4], edi
40E1FF  mov     [ebp-8], esi
40E202  test    ecx, ecx
40E204  jnz     loc_40E100
40E20A  pop     edi
40E20B  pop     esi
40E20C  mov     esp, ebp
40E20E  pop     ebp
40E20F  mov     esp, ebx
40E211  pop     ebx
40E212  retn

;==== 40E220 sub_40E220
40E220  push    ebp
40E221  mov     ebp, esp
40E223  sub     esp, 18h
40E226  mov     eax, [ebp+arg_0]
40E229  mov     ecx, [eax]
40E22B  push    esi
40E22C  mov     esi, [ebp+arg_4]
40E22F  mov     eax, [esi+0Ch]
40E232  mov     edx, 100h
40E237  sub     edx, [ebp+arg_8]
40E23A  push    edi
40E23B  mov     edi, [esi]
40E23D  mov     [ebp+arg_8], edx
40E240  mov     [ebp+var_8], ecx
40E243  mov     [ebp+var_C], edi
40E246  test    eax, eax
40E248  jz      loc_40E32B
40E24E  push    ebx
40E24F  nop
40E250  dec     eax
40E251  mov     [ebp+var_18], eax
40E254  mov     eax, [esi+8]
40E257  test    eax, eax
40E259  jz      loc_40E30D
40E25F  nop
40E260  dec     eax
40E261  test    dword ptr [edi], 0FF000000h
40E267  mov     [ebp+var_14], eax
40E26A  jz      loc_40E2F9
40E270  movzx   eax, byte ptr [edi+3]
40E274  movzx   esi, byte ptr [ecx+3]
40E278  mov     ebx, 100h
40E27D  sub     ebx, eax
40E27F  imul    ebx, esi
40E282  mov     esi, eax
40E284  imul    eax, edx
40E287  xor     edx, edx
40E289  shl     eax, 10h
40E28C  shl     esi, 8
40E28F  add     esi, ebx
40E291  div     esi
40E293  xor     edx, edx
40E295  mov     [ebp+var_4], eax
40E298  mov     eax, ebx
40E29A  shl     eax, 10h
40E29D  div     esi
40E29F  movzx   edx, byte ptr [ecx]
40E2A2  movzx   ebx, byte ptr [edi]
40E2A5  imul    edx, eax
40E2A8  mov     [ebp+var_10], eax
40E2AB  mov     eax, [ebp+var_4]
40E2AE  imul    ebx, eax
40E2B1  add     edx, ebx
40E2B3  movzx   ebx, byte ptr [ecx+1]
40E2B7  shr     edx, 10h
40E2BA  mov     [ecx], dl
40E2BC  movzx   edx, byte ptr [edi+1]
40E2C0  imul    edx, eax
40E2C3  mov     eax, [ebp+var_10]
40E2C6  imul    ebx, eax
40E2C9  add     edx, ebx
40E2CB  movzx   ebx, byte ptr [ecx+2]
40E2CF  shr     edx, 10h
40E2D2  imul    ebx, eax
40E2D5  mov     [ecx+1], dl
40E2D8  movzx   edx, byte ptr [edi+2]
40E2DC  imul    edx, [ebp+var_4]
40E2E0  add     edx, ebx
40E2E2  shr     edx, 10h
40E2E5  mov     eax, esi
40E2E7  mov     esi, [ebp+arg_4]
40E2EA  shr     eax, 8
40E2ED  mov     [ecx+2], dl
40E2F0  mov     edx, [ebp+arg_8]
40E2F3  mov     [ecx+3], al
40E2F6  mov     eax, [ebp+var_14]
40E2F9  mov     ebx, [ebp+arg_0]
40E2FC  add     ecx, [ebx+14h]
40E2FF  add     edi, [esi+14h]
40E302  test    eax, eax
40E304  jnz     loc_40E260
40E30A  mov     ecx, [ebp+var_8]
40E30D  mov     eax, [ebp+arg_0]
40E310  add     ecx, [eax+4]
40E313  mov     edi, [ebp+var_C]
40E316  add     edi, [esi+4]
40E319  mov     eax, [ebp+var_18]
40E31C  mov     [ebp+var_8], ecx
40E31F  mov     [ebp+var_C], edi
40E322  test    eax, eax
40E324  jnz     loc_40E250
40E32A  pop     ebx
40E32B  pop     edi
40E32C  pop     esi
40E32D  mov     esp, ebp
40E32F  pop     ebp
40E330  retn

;==== 40E340 sub_40E340
40E340  push    ebp
40E341  mov     ebp, esp
40E343  push    esi
40E344  mov     esi, [ebp+arg_0]
40E347  mov     eax, [esi+10h]
40E34A  sub     eax, 0
40E34D  jz      short loc_40E3A0
40E34F  dec     eax
40E350  jz      short loc_40E37F
40E352  dec     eax
40E353  jnz     short loc_40E36C
40E355  mov     eax, [ecx+10h]
40E358  dec     eax
40E359  push    edi
40E35A  jz      short loc_40E36F
40E35C  dec     eax
40E35D  jnz     short loc_40E36B
40E35F  push    edx
40E360  push    ecx
40E361  mov     edi, esi
40E363  call    sub_40E490
40E368  add     esp, 8
40E36B  pop     edi
40E36C  pop     esi
40E36D  pop     ebp
40E36E  retn
40E36F  push    edx
40E370  push    ecx
40E371  mov     edi, esi
40E373  call    sub_40E490
40E378  add     esp, 8
40E37B  pop     edi
40E37C  pop     esi
40E37D  pop     ebp
40E37E  retn
40E37F  mov     eax, [ecx+10h]
40E382  dec     eax
40E383  jz      short loc_40E394
40E385  dec     eax
40E386  jnz     short loc_40E36C
40E388  push    ecx
40E389  call    sub_40E410
40E38E  add     esp, 4
40E391  pop     esi
40E392  pop     ebp
40E393  retn
40E394  push    ecx
40E395  call    sub_40E410
40E39A  add     esp, 4
40E39D  pop     esi
40E39E  pop     ebp
40E39F  retn
40E3A0  push    esi
40E3A1  push    ecx
40E3A2  call    sub_40E3B0
40E3A7  add     esp, 8
40E3AA  pop     esi
40E3AB  pop     ebp
40E3AC  retn

;==== 40E3B0 sub_40E3B0
40E3B0  push    ebp
40E3B1  mov     ebp, esp
40E3B3  push    ecx
40E3B4  mov     eax, [ebp+arg_0]
40E3B7  push    ebx
40E3B8  mov     ebx, [ebp+arg_4]
40E3BB  mov     ecx, [ebx+0Ch]
40E3BE  push    esi
40E3BF  mov     esi, [eax]
40E3C1  push    edi
40E3C2  mov     edi, [ebx]
40E3C4  test    ecx, ecx
40E3C6  jz      short loc_40E407
40E3C8  mov     eax, [ebx+8]
40E3CB  dec     ecx
40E3CC  mov     [ebp+var_4], ecx
40E3CF  test    eax, eax
40E3D1  jz      short loc_40E3FA
40E3D3  mov     edx, edi
40E3D5  lea     ecx, [esi+eax*2]
40E3D8  sub     edx, esi
40E3DA  lea     ebx, [ebx+0]
40E3E0  sub     ecx, 2
40E3E3  dec     eax
40E3E4  cmp     word ptr [edx+ecx], 0
40E3E9  jbe     short loc_40E3F3
40E3EB  xor     ebx, ebx
40E3ED  mov     [ecx], bx
40E3F0  mov     ebx, [ebp+arg_4]
40E3F3  test    eax, eax
40E3F5  jnz     short loc_40E3E0
40E3F7  mov     ecx, [ebp+var_4]
40E3FA  mov     edx, [ebp+arg_0]
40E3FD  add     esi, [edx+4]
40E400  add     edi, [ebx+4]
40E403  test    ecx, ecx
40E405  jnz     short loc_40E3C8
40E407  pop     edi
40E408  pop     esi
40E409  pop     ebx
40E40A  mov     esp, ebp
40E40C  pop     ebp
40E40D  retn

;==== 40E410 sub_40E410
40E410  push    ebp
40E411  mov     ebp, esp
40E413  sub     esp, 0Ch
40E416  mov     eax, [esi]
40E418  push    ebx
40E419  mov     ebx, [ebp+arg_0]
40E41C  mov     ecx, [ebx]
40E41E  push    edi
40E41F  mov     edi, [esi+0Ch]
40E422  mov     [ebp+var_4], ecx
40E425  mov     [ebp+var_8], eax
40E428  test    edi, edi
40E42A  jz      short loc_40E480
40E42C  lea     esp, [esp+0]
40E430  mov     edx, [esi+8]
40E433  dec     edi
40E434  mov     [ebp+var_C], edi
40E437  test    edx, edx
40E439  jz      short loc_40E470
40E43B  jmp     short loc_40E440
40E440  movzx   ebx, byte ptr [eax+1]
40E444  movzx   edi, byte ptr [eax+2]
40E448  add     edi, ebx
40E44A  movzx   ebx, byte ptr [eax]
40E44D  add     edi, ebx
40E44F  dec     edx
40E450  test    edi, edi
40E452  jle     short loc_40E45A
40E454  mov     dword ptr [ecx], 0
40E45A  mov     ebx, [ebp+arg_0]
40E45D  add     ecx, [ebx+14h]
40E460  add     eax, [esi+14h]
40E463  test    edx, edx
40E465  jnz     short loc_40E440
40E467  mov     eax, [ebp+var_8]
40E46A  mov     ecx, [ebp+var_4]
40E46D  mov     edi, [ebp+var_C]
40E470  add     ecx, [ebx+4]
40E473  add     eax, [esi+4]
40E476  mov     [ebp+var_4], ecx
40E479  mov     [ebp+var_8], eax
40E47C  test    edi, edi
40E47E  jnz     short loc_40E430
40E480  pop     edi
40E481  pop     ebx
40E482  mov     esp, ebp
40E484  pop     ebp
40E485  retn

;==== 40E490 sub_40E490
40E490  push    ebp
40E491  mov     ebp, esp
40E493  sub     esp, 8
40E496  mov     eax, [ebp+arg_0]
40E499  mov     ecx, [edi+0Ch]
40E49C  push    ebx
40E49D  mov     ebx, [eax]
40E49F  mov     eax, [edi]
40E4A1  mov     [ebp+var_4], eax
40E4A4  test    ecx, ecx
40E4A6  jz      short loc_40E508
40E4A8  push    esi
40E4A9  lea     esp, [esp+0]
40E4B0  mov     edx, [edi+8]
40E4B3  dec     ecx
40E4B4  mov     [ebp+var_8], ecx
40E4B7  mov     esi, ebx
40E4B9  test    edx, edx
40E4BB  jz      short loc_40E4F4
40E4BD  lea     ecx, [ecx+0]
40E4C0  dec     edx
40E4C1  cmp     [ebp+arg_4], 0
40E4C5  jz      short loc_40E4D1
40E4C7  mov     ecx, [eax]
40E4C9  and     ecx, 0FF000000h
40E4CF  jmp     short loc_40E4DA
40E4D1  xor     ecx, ecx
40E4D3  cmp     byte ptr [eax+3], 0FFh
40E4D7  setz    cl
40E4DA  test    ecx, ecx
40E4DC  jz      short loc_40E4E4
40E4DE  mov     dword ptr [esi], 0
40E4E4  mov     ecx, [ebp+arg_0]
40E4E7  add     esi, [ecx+14h]
40E4EA  add     eax, [edi+14h]
40E4ED  test    edx, edx
40E4EF  jnz     short loc_40E4C0
40E4F1  mov     eax, [ebp+var_4]
40E4F4  mov     edx, [ebp+arg_0]
40E4F7  add     eax, [edi+4]
40E4FA  mov     ecx, [ebp+var_8]
40E4FD  add     ebx, [edx+4]
40E500  mov     [ebp+var_4], eax
40E503  test    ecx, ecx
40E505  jnz     short loc_40E4B0
40E507  pop     esi
40E508  pop     ebx
40E509  mov     esp, ebp
40E50B  pop     ebp
40E50C  retn

;==== 4115B0 sub_4115B0
4115B0  push    ebp
4115B1  mov     ebp, esp
4115B3  mov     edx, [ebp+arg_0]
4115B6  push    edi
4115B7  test    esi, esi
4115B9  jnz     short loc_4115C7
4115BB  push    edx
4115BC  call    sub_40DE80
4115C1  add     esp, 4
4115C4  pop     edi
4115C5  pop     ebp
4115C6  retn
4115C7  mov     edi, [eax+10h]
4115CA  cmp     [edx+10h], edi
4115CD  jnz     short loc_4115F4
4115CF  test    edi, edi
4115D1  jz      short loc_4115E8
4115D3  dec     edi
4115D4  cmp     edi, 1
4115D7  ja      short loc_4115F4
4115D9  push    ecx
4115DA  push    esi
4115DB  mov     ecx, edx
4115DD  call    sub_4116F0
4115E2  add     esp, 8
4115E5  pop     edi
4115E6  pop     ebp
4115E7  retn
4115E8  push    eax
4115E9  push    edx
4115EA  mov     eax, esi
4115EC  call    sub_411600
4115F1  add     esp, 8
4115F4  pop     edi
4115F5  pop     ebp
4115F6  retn

;==== 411600 sub_411600
411600  push    ebp
411601  mov     ebp, esp
411603  sub     esp, 1Ch
411606  push    ebx
411607  push    esi
411608  mov     esi, ecx
41160A  mov     ecx, [ebp+arg_0]
41160D  mov     edx, [ecx]
41160F  mov     ecx, [ebp+arg_4]
411612  mov     ebx, [ecx]
411614  mov     [ebp+var_4], edx
411617  mov     edx, eax
411619  push    edi
41161A  shr     edx, 9
41161D  mov     edi, eax
41161F  shr     eax, 3
411622  and     edx, 7C00h
411628  imul    edx, esi
41162B  and     eax, 1Fh
41162E  shr     edi, 6
411631  imul    eax, esi
411634  and     edi, 3E0h
41163A  imul    edi, esi
41163D  mov     [ebp+var_10], edx
411640  mov     edx, [ebp+arg_4]
411643  mov     ecx, 100h
411648  mov     [ebp+var_14], eax
41164B  mov     eax, [edx+0Ch]
41164E  sub     ecx, esi
411650  mov     [ebp+var_C], ebx
411653  mov     [ebp+var_8], edi
411656  test    eax, eax
411658  jz      loc_4116E7
41165E  mov     edi, edi
411660  mov     esi, [edx+8]
411663  dec     eax
411664  mov     [ebp+var_1C], eax
411667  test    esi, esi
411669  jz      short loc_4116CD
41166B  mov     eax, [ebp+var_4]
41166E  sub     eax, ebx
411670  lea     edx, [ebx+esi*2]
411673  mov     [ebp+var_18], eax
411676  movzx   eax, word ptr [edx-2]
41167A  sub     edx, 2
41167D  mov     ebx, eax
41167F  and     ebx, 3E0h
411685  imul    ebx, ecx
411688  add     ebx, edi
41168A  mov     edi, eax
41168C  and     edi, 7C00h
411692  imul    edi, ecx
411695  add     edi, [ebp+var_10]
411698  and     eax, 1Fh
41169B  imul    eax, ecx
41169E  add     eax, [ebp+var_14]
4116A1  shr     ebx, 8
4116A4  shr     edi, 8
4116A7  and     ebx, 3E0h
4116AD  and     edi, 7C00h
4116B3  add     ebx, edi
4116B5  mov     edi, [ebp+var_8]
4116B8  shr     eax, 8
4116BB  add     ebx, eax
4116BD  dec     esi
4116BE  mov     eax, [ebp+var_18]
4116C1  mov     [eax+edx], bx
4116C5  jnz     short loc_411676
4116C7  mov     eax, [ebp+var_1C]
4116CA  mov     ebx, [ebp+var_C]
4116CD  mov     edx, [ebp+arg_0]
4116D0  mov     edx, [edx+4]
4116D3  add     [ebp+var_4], edx
4116D6  mov     edx, [ebp+arg_4]
4116D9  add     ebx, [edx+4]
4116DC  mov     [ebp+var_C], ebx
4116DF  test    eax, eax
4116E1  jnz     loc_411660
4116E7  pop     edi
4116E8  pop     esi
4116E9  pop     ebx
4116EA  mov     esp, ebp
4116EC  pop     ebp
4116ED  retn

;==== 4116F0 sub_4116F0
4116F0  push    ebx
4116F1  mov     ebx, esp
4116F3  sub     esp, 8
4116F6  and     esp, 0FFFFFFF0h
4116F9  add     esp, 4
4116FC  push    ebp
4116FD  mov     ebp, [ebx+4]
411700  mov     [esp+0Ch+var_8], ebp
411704  mov     ebp, esp
411706  sub     esp, 48h
411709  mov     edx, [ecx]
41170B  mov     [ebp-10h], edx
41170E  mov     edx, [eax]
411710  mov     [ebp-4], edx
411713  mov     edx, [ecx+4]
411716  mov     [ebp-14h], edx
411719  mov     edx, [eax+4]
41171C  push    esi
41171D  mov     esi, [ebx+0Ch]
411720  push    edi
411721  mov     edi, [eax+0Ch]
411724  mov     [ebp-18h], edx
411727  mov     edx, [ecx+0Ch]
41172A  mov     [ebp-8], edx
41172D  cmp     edx, edi
41172F  jbe     short loc_411734
411731  mov     [ebp-8], edi
411734  mov     ecx, [ecx+8]
411737  mov     eax, [eax+8]
41173A  mov     [ebp-0Ch], ecx
41173D  cmp     ecx, eax
41173F  jbe     short loc_411744
411741  mov     [ebp-0Ch], eax
411744  mov     ecx, 100h
411749  mov     edx, ecx
41174B  movzx   ecx, cx
41174E  movd    xmm1, ecx
411752  mov     eax, 100h
411757  sub     eax, esi
411759  movzx   ecx, ax
41175C  movd    xmm2, ecx
411760  movzx   ecx, ax
411763  movd    xmm3, ecx
411767  movzx   ecx, ax
41176A  movzx   edx, dx
41176D  movd    xmm4, ecx
411771  movd    xmm5, edx
411775  movzx   edx, ax
411778  movzx   ecx, ax
41177B  movzx   eax, ax
41177E  pxor    xmm0, xmm0
411782  movd    xmm7, edx
411786  mov     edx, [ebx+8]
411789  movdqa  xmmword ptr [ebp-30h], xmm0
41178E  movd    xmm0, eax
411792  movd    xmm6, ecx
411796  mov     eax, edx
411798  and     eax, 0FF0000h
41179D  imul    eax, esi
4117A0  mov     ecx, edx
4117A2  and     ecx, 0FF00h
4117A8  imul    ecx, esi
4117AB  and     edx, 0FFh
4117B1  imul    edx, esi
4117B4  xor     esi, esi
4117B6  punpcklwd xmm5, xmm1
4117BA  movzx   esi, si
4117BD  punpcklwd xmm7, xmm3
4117C1  punpcklwd xmm7, xmm5
4117C5  punpcklwd xmm0, xmm4
4117C9  punpcklwd xmm6, xmm2
4117CD  punpcklwd xmm0, xmm6
4117D1  punpcklwd xmm0, xmm7
4117D5  shr     eax, 10h
4117D8  movdqa  xmmword ptr [ebp-40h], xmm0
4117DD  movd    xmm0, esi
4117E1  shr     ecx, 8
4117E4  mov     esi, eax
4117E6  movd    xmm2, esi
4117EA  movzx   esi, cx
4117ED  movd    xmm3, esi
4117F1  movzx   esi, dx
4117F4  xor     edi, edi
4117F6  movd    xmm4, esi
4117FA  movzx   esi, di
4117FD  movzx   ecx, cx
411800  movzx   edx, dx
411803  movd    xmm5, esi
411807  mov     esi, [ebp-8]
41180A  movd    xmm6, eax
41180E  movd    xmm7, ecx
411812  movd    xmm1, edx
411816  punpcklwd xmm5, xmm0
41181A  punpcklwd xmm6, xmm2
41181E  punpcklwd xmm1, xmm4
411822  punpcklwd xmm7, xmm3
411826  punpcklwd xmm1, xmm6
41182A  punpcklwd xmm7, xmm5
41182E  punpcklwd xmm1, xmm7
411832  test    esi, esi
411834  jz      loc_4118CA
41183A  mov     ecx, [ebp-0Ch]
41183D  movdqa  xmm0, xmmword ptr [ebp-30h]
411842  movdqa  xmm2, xmmword ptr [ebp-40h]
411847  mov     edi, [ebp-10h]
41184A  mov     eax, ecx
41184C  shr     eax, 1
41184E  and     ecx, 1
411851  mov     [ebp-0Ch], eax
411854  mov     [ebp-8], ecx
411857  jmp     short loc_411863
411860  mov     eax, [ebp-0Ch]
411863  mov     edx, [ebp-4]
411866  dec     esi
411867  mov     ecx, edi
411869  test    eax, eax
41186B  jz      short loc_411896
41186D  lea     ecx, [ecx+0]
411870  movq    xmm3, qword ptr [edx]
411874  punpcklbw xmm3, xmm0
411878  pmullw  xmm3, xmm2
41187C  paddusw xmm3, xmm1
411880  psrlw   xmm3, 8
411885  packuswb xmm3, xmm0
411889  movq    qword ptr [ecx], xmm3
41188D  add     ecx, 8
411890  add     edx, 8
411893  dec     eax
411894  jnz     short loc_411870
411896  cmp     dword ptr [ebp-8], 0
41189A  jz      short loc_4118BD
41189C  mov     eax, [edx]
41189E  movd    xmm3, eax
4118A2  punpcklbw xmm3, xmm0
4118A6  pmullw  xmm3, xmm2
4118AA  paddusw xmm3, xmm1
4118AE  psrlw   xmm3, 8
4118B3  packuswb xmm3, xmm0
4118B7  movd    edx, xmm3
4118BB  mov     [ecx], edx
4118BD  mov     eax, [ebp-18h]
4118C0  add     edi, [ebp-14h]
4118C3  add     [ebp-4], eax
4118C6  test    esi, esi
4118C8  jnz     short loc_411860
4118CA  pop     edi
4118CB  pop     esi
4118CC  mov     esp, ebp
4118CE  pop     ebp
4118CF  mov     esp, ebx
4118D1  pop     ebx
4118D2  retn

;==== 414190 sub_414190
414190  push    ebp
414191  mov     ebp, esp
414193  mov     ecx, [ebp+arg_0]
414196  push    esi
414197  cmp     edx, 4
41419A  jnb     short loc_4141B4
41419C  mov     esi, [eax+10h]
41419F  dec     esi
4141A0  cmp     esi, 1
4141A3  ja      short loc_4141B1
4141A5  push    edx
4141A6  push    ecx
4141A7  mov     esi, eax
4141A9  call    sub_4141C0
4141AE  add     esp, 8
4141B1  pop     esi
4141B2  pop     ebp
4141B3  retn
4141B4  pop     esi
4141B5  pop     ebp
4141B6  jmp     sub_40B100

;==== 4141C0 sub_4141C0
4141C0  push    ebp
4141C1  mov     ebp, esp
4141C3  sub     esp, 0Ch
4141C6  cmp     dword ptr [esi+0Ch], 0
4141CA  mov     eax, [ebp+arg_0]
4141CD  mov     ecx, [eax]
4141CF  mov     edx, [esi]
4141D1  push    ebx
4141D2  mov     ebx, [ebp+arg_4]
4141D5  mov     [ebp+var_4], ecx
4141D8  mov     [ebp+var_8], edx
4141DB  mov     [ebp+var_C], 0
4141E2  jbe     loc_414285
4141E8  push    edi
4141E9  lea     esp, [esp+0]
4141F0  xor     edi, edi
4141F2  cmp     [esi+8], edi
4141F5  jbe     short loc_414265
4141F7  mov     eax, ecx
4141F9  sub     edx, ecx
4141FB  jmp     short loc_414200
414200  cmp     ebx, 3; switch 4 cases
414203  ja      short def_414205; jumptable 00414205 default case
414205  jmp     ds:jpt_414205[ebx*4]; switch jump
41420C  movzx   ebx, byte ptr [edx+eax]; jumptable 00414205 case 0
414210  or      ebx, 0FF000000h
414216  jmp     short loc_414254
414218  mov     ebx, [edx+eax]; jumptable 00414205 case 1
41421B  and     ebx, 0FF00h
414221  or      ebx, 0FF000000h
414227  jmp     short loc_414254
414229  mov     ebx, [edx+eax]; jumptable 00414205 case 2
41422C  and     ebx, 0FFFF0000h
414232  or      ebx, 0FF000000h
414238  jmp     short loc_414254
41423A  movzx   ecx, byte ptr [edx+eax+3]; jumptable 00414205 case 3
41423F  mov     ebx, ecx
414241  or      ebx, 0FFFFFF00h
414247  shl     ebx, 8
41424A  or      ebx, ecx
41424C  shl     ebx, 8
41424F  or      ebx, ecx
414251  mov     ecx, [ebp+var_4]
414254  mov     [eax], ebx
414256  mov     ebx, [ebp+arg_4]; jumptable 00414205 default case
414259  inc     edi
41425A  add     eax, 4
41425D  cmp     edi, [esi+8]
414260  jb      short loc_414200
414262  mov     edx, [ebp+var_8]
414265  mov     eax, [ebp+arg_0]
414268  add     ecx, [eax+4]
41426B  mov     eax, [ebp+var_C]
41426E  add     edx, [esi+4]
414271  inc     eax
414272  mov     [ebp+var_4], ecx
414275  mov     [ebp+var_8], edx
414278  mov     [ebp+var_C], eax
41427B  cmp     eax, [esi+0Ch]
41427E  jb      loc_4141F0
414284  pop     edi
414285  pop     ebx
414286  mov     esp, ebp
414288  pop     ebp
414289  retn

;==== 4154B0 sub_4154B0
4154B0  push    ecx
4154B1  test    esi, esi
4154B3  jz      short loc_4154DF
4154B5  mov     edx, [eax+10h]
4154B8  dec     edx
4154B9  jz      short loc_4154D0
4154BB  dec     edx
4154BC  jnz     short loc_4154DF
4154BE  mov     edx, [ecx+10h]
4154C1  dec     edx
4154C2  jnz     short loc_4154DF
4154C4  push    esi
4154C5  push    eax
4154C6  call    sub_4155A0
4154CB  add     esp, 8
4154CE  pop     ecx
4154CF  retn
4154D0  cmp     dword ptr [ecx+10h], 1
4154D4  jnz     short loc_4154DF
4154D6  push    esi
4154D7  call    sub_4154F0
4154DC  add     esp, 4
4154DF  pop     ecx
4154E0  retn

;==== 4154F0 sub_4154F0
4154F0  push    ebp
4154F1  mov     ebp, esp
4154F3  sub     esp, 20h
4154F6  mov     edx, [ecx]
4154F8  mov     ecx, [ecx+4]
4154FB  push    esi
4154FC  mov     [ebp+var_14], edx
4154FF  mov     edx, [eax]
415501  push    edi
415502  mov     [ebp+var_10], edx
415505  mov     edx, [eax+4]
415508  push    100010h
41550D  push    100010h
415512  mov     [ebp+var_8], ecx
415515  mov     ecx, [eax+0Ch]
415518  mov     [ebp+var_4], edx
41551B  mov     edx, [eax+8]
41551E  mov     eax, [ebp+arg_0]
415521  push    0
415523  push    eax
415524  mov     [ebp+var_18], ecx
415527  mov     [ebp+var_C], edx
41552A  call    __allmul
41552F  mov     dword ptr [ebp+var_20], eax
415532  mov     dword ptr [ebp+var_20+4], edx
415535  movq    mm6, [ebp+var_20]
415539  pxor    mm7, mm7
41553C  mov     edx, [ebp+var_18]
41553F  mov     edi, [ebp+var_14]
415542  mov     esi, [ebp+var_10]
415545  mov     ecx, [ebp+var_C]
415548  xor     eax, eax
41554A  movd    mm1, dword ptr [esi+eax]
41554E  movd    mm0, dword ptr [edi+eax]
415552  punpcklbw mm1, mm7
415555  punpcklbw mm0, mm7
415558  psllw   mm1, 4
41555C  movq    mm2, mm0
41555F  pmulhw  mm1, mm6
415562  psllw   mm0, 4
415566  movq    mm3, mm1
415569  psllw   mm1, 4
41556D  paddw   mm2, mm3
415570  pmulhw  mm0, mm1
415573  psubsw  mm2, mm0
415576  packuswb mm2, mm7
415579  movd    dword ptr [edi+eax], mm2
41557D  add     eax, 4
415580  dec     ecx
415581  jnz     short loc_41554A
415583  add     edi, [ebp+var_8]
415586  add     esi, [ebp+var_4]
415589  dec     edx
41558A  jnz     short loc_415545
41558C  emms
41558E  pop     edi
41558F  pop     esi
415590  mov     esp, ebp
415592  pop     ebp
415593  retn

;==== 4155A0 sub_4155A0
4155A0  push    ebp
4155A1  mov     ebp, esp
4155A3  sub     esp, 424h
4155A9  mov     eax, dword_4FB734
4155AE  xor     eax, ebp
4155B0  mov     [ebp+var_4], eax
4155B3  mov     edx, [ecx]
4155B5  mov     eax, [ebp+arg_0]
4155B8  mov     ecx, [ecx+4]
4155BB  mov     [ebp+var_414], edx
4155C1  mov     edx, [eax]
4155C3  mov     [ebp+var_420], edx
4155C9  mov     edx, [eax+4]
4155CC  push    ebx
4155CD  mov     ebx, [ebp+arg_4]
4155D0  mov     [ebp+var_424], ecx
4155D6  mov     ecx, [eax+8]
4155D9  mov     [ebp+var_418], edx
4155DF  mov     edx, [eax+0Ch]
4155E2  push    esi
4155E3  push    edi
4155E4  mov     [ebp+var_410], ecx
4155EA  mov     [ebp+var_41C], edx
4155F0  cmp     ebx, 100h
4155F6  jnb     short loc_41563B
4155F8  xor     esi, esi
4155FA  xor     edi, edi
4155FC  lea     esp, [esp+0]
415600  mov     eax, edi
415602  push    1
415604  shr     eax, 8
415607  cdq
415608  push    10001h
41560D  push    edx
41560E  push    eax
41560F  call    __allmul
415614  mov     [ebp+esi*8+var_408], eax
41561B  mov     [ebp+esi*8+var_404], edx
415622  inc     esi
415623  add     edi, ebx
415625  cmp     esi, 80h
41562B  jl      short loc_415600
41562D  lea     eax, [ebp+var_408]
415633  mov     [ebp+var_40C], eax
415639  jmp     short loc_415645
41563B  mov     [ebp+var_40C], offset dword_50A8D0
415645  cmp     dword_565B0C, 0
41564C  jz      loc_4156F7
415652  call    sub_407AD0
415657  test    eax, eax
415659  jz      loc_4156F7
41565F  pxor    mm7, mm7
415662  mov     ecx, [ebp+var_41C]
415668  mov     edi, [ebp+var_414]
41566E  mov     esi, [ebp+var_420]
415674  mov     ebx, [ebp+var_40C]
41567A  push    esi
41567B  push    edi
41567C  mov     edx, [ebp+var_410]
415682  prefetchnta byte ptr [esi+100h]
415689  mov     eax, [esi]
41568B  test    eax, 0FE000000h
415690  jz      short loc_4156CA
415692  prefetchnta byte ptr [edi+80h]
415699  movd    mm1, eax
41569C  shr     eax, 19h
41569F  punpcklbw mm1, mm7
4156A2  movd    mm0, dword ptr [edi]
4156A5  pmullw  mm1, qword ptr [ebx+eax*8]
4156A9  punpcklbw mm0, mm7
4156AC  psraw   mm1, 7
4156B0  movq    mm2, mm0
4156B3  psllw   mm0, 4
4156B7  paddw   mm2, mm1
4156BA  psllw   mm1, 4
4156BE  pmulhw  mm0, mm1
4156C1  psubsw  mm2, mm0
4156C4  packuswb mm2, mm7
4156C7  movd    dword ptr [edi], mm2
4156CA  add     edi, 4
4156CD  add     esi, 4
4156D0  dec     edx
4156D1  jnz     short loc_415682
4156D3  pop     edi
4156D4  pop     esi
4156D5  add     edi, [ebp+var_424]
4156DB  add     esi, [ebp+var_418]
4156E1  dec     ecx
4156E2  jnz     short loc_41567A
4156E4  emms
4156E6  pop     edi
4156E7  pop     esi
4156E8  pop     ebx
4156E9  mov     ecx, [ebp+var_4]
4156EC  xor     ecx, ebp
4156EE  call    sub_4ABDD5
4156F3  mov     esp, ebp
4156F5  pop     ebp
4156F6  retn
4156F7  pxor    mm7, mm7
4156FA  mov     ecx, [ebp+var_41C]
415700  mov     edi, [ebp+var_414]
415706  mov     esi, [ebp+var_420]
41570C  mov     ebx, [ebp+var_40C]
415712  push    esi
415713  push    edi
415714  mov     edx, [ebp+var_410]
41571A  mov     eax, [esi]
41571C  test    eax, 0FE000000h
415721  jz      short loc_415754
415723  movd    mm1, eax
415726  shr     eax, 19h
415729  punpcklbw mm1, mm7
41572C  movd    mm0, dword ptr [edi]
41572F  pmullw  mm1, qword ptr [ebx+eax*8]
415733  punpcklbw mm0, mm7
415736  psraw   mm1, 7
41573A  movq    mm2, mm0
41573D  psllw   mm0, 4
415741  paddw   mm2, mm1
415744  psllw   mm1, 4
415748  pmulhw  mm0, mm1
41574B  psubsw  mm2, mm0
41574E  packuswb mm2, mm7
415751  movd    dword ptr [edi], mm2
415754  add     edi, 4
415757  add     esi, 4
41575A  dec     edx
41575B  jnz     short loc_41571A
41575D  pop     edi
41575E  pop     esi
41575F  add     edi, [ebp+var_424]
415765  add     esi, [ebp+var_418]
41576B  dec     ecx
41576C  jnz     short loc_415712
41576E  emms
415770  mov     ecx, [ebp+var_4]
415773  pop     edi
415774  pop     esi
415775  xor     ecx, ebp
415777  pop     ebx
415778  call    sub_4ABDD5
41577D  mov     esp, ebp
41577F  pop     ebp
415780  retn

;==== 415790 sub_415790
415790  push    ecx
415791  test    esi, esi
415793  jz      short loc_4157D0
415795  mov     edx, [eax+10h]
415798  dec     edx
415799  jz      short loc_4157B0
41579B  dec     edx
41579C  jnz     short loc_4157D0
41579E  mov     edx, [ecx+10h]
4157A1  dec     edx
4157A2  jnz     short loc_4157D0
4157A4  push    esi
4157A5  push    eax
4157A6  call    sub_415970
4157AB  add     esp, 8
4157AE  pop     ecx
4157AF  retn
4157B0  cmp     dword ptr [ecx+10h], 1
4157B4  jnz     short loc_4157D0
4157B6  cmp     esi, 100h
4157BC  jnb     short loc_4157CB
4157BE  push    ecx
4157BF  mov     ecx, esi
4157C1  call    sub_415890
4157C6  add     esp, 4
4157C9  pop     ecx
4157CA  retn
4157CB  call    sub_4157E0
4157D0  pop     ecx
4157D1  retn

;==== 4157E0 sub_4157E0
4157E0  push    ebp
4157E1  mov     ebp, esp
4157E3  sub     esp, 18h
4157E6  mov     edx, [ecx]
4157E8  mov     ecx, [ecx+4]
4157EB  mov     [ebp+var_14], edx
4157EE  mov     edx, [eax]
4157F0  mov     [ebp+var_10], edx
4157F3  mov     edx, [eax+4]
4157F6  mov     [ebp+var_8], ecx
4157F9  mov     ecx, [eax+8]
4157FC  mov     [ebp+var_4], edx
4157FF  mov     edx, [eax+0Ch]
415802  push    esi
415803  push    edi
415804  mov     [ebp+var_C], ecx
415807  mov     [ebp+var_18], edx
41580A  pcmpeqw mm6, mm6
41580D  psrlw   mm6, 9
415811  pxor    mm7, mm7
415814  mov     ecx, [ebp+var_18]
415817  mov     edi, [ebp+var_14]
41581A  mov     esi, [ebp+var_10]
41581D  push    esi
41581E  push    edi
41581F  mov     edx, [ebp+var_C]
415822  movd    mm0, dword ptr [esi]
415825  movd    mm1, dword ptr [edi]
415828  punpcklbw mm0, mm7
41582B  punpcklbw mm1, mm7
41582E  movq    mm4, mm1
415831  pcmpgtw mm4, mm6
415834  movq    mm2, mm4
415837  psrlw   mm2, 0Fh
41583B  psllw   mm2, 8
41583F  pxor    mm0, mm4
415842  pxor    mm1, mm4
415845  psubw   mm0, mm4
415848  psubw   mm1, mm4
41584B  paddw   mm0, mm2
41584E  paddw   mm1, mm2
415851  paddw   mm2, mm4
415854  psllw   mm0, 5
415858  psllw   mm1, 4
41585C  pmulhw  mm0, mm1
41585F  pxor    mm0, mm4
415862  psubw   mm0, mm4
415865  paddw   mm0, mm2
415868  packuswb mm0, mm7
41586B  movd    dword ptr [edi], mm0
41586E  add     edi, 4
415871  add     esi, 4
415874  dec     edx
415875  jnz     short loc_415822
415877  pop     edi
415878  pop     esi
415879  add     edi, [ebp+var_8]
41587C  add     esi, [ebp+var_4]
41587F  dec     ecx
415880  jnz     short loc_41581D
415882  emms
415884  pop     edi
415885  pop     esi
415886  mov     esp, ebp
415888  pop     ebp
415889  retn

;==== 415890 sub_415890
415890  push    ebp
415891  mov     ebp, esp
415893  sub     esp, 20h
415896  mov     edx, [ebp+arg_0]
415899  push    esi
41589A  mov     esi, [edx]
41589C  mov     edx, [edx+4]
41589F  mov     [ebp+var_4], edx
4158A2  mov     edx, [eax+4]
4158A5  mov     [ebp+arg_0], edx
4158A8  mov     edx, [eax+8]
4158AB  mov     [ebp+var_10], esi
4158AE  mov     esi, [eax]
4158B0  mov     eax, [eax+0Ch]
4158B3  mov     [ebp+var_14], eax
4158B6  mov     [ebp+var_8], edx
4158B9  mov     eax, ecx
4158BB  and     eax, 1FEh
4158C0  mov     edx, 80008000h
4158C5  mul     edx
4158C7  shr     ecx, 1
4158C9  mov     [ebp+var_C], esi
4158CC  xor     esi, esi
4158CE  add     eax, ecx
4158D0  adc     edx, esi
4158D2  push    edi
4158D3  mov     dword ptr [ebp+var_20], eax
4158D6  mov     dword ptr [ebp+var_20+4], edx
4158D9  movq    mm5, [ebp+var_20]
4158DD  pcmpeqw mm6, mm6
4158E0  psrlw   mm6, 9
4158E4  pxor    mm7, mm7
4158E7  mov     ecx, [ebp+var_14]
4158EA  mov     edi, [ebp+var_10]
4158ED  mov     esi, [ebp+var_C]
4158F0  push    esi
4158F1  push    edi
4158F2  mov     edx, [ebp+var_8]
4158F5  movd    mm0, dword ptr [esi]
4158F8  movd    mm1, dword ptr [edi]
4158FB  punpcklbw mm0, mm7
4158FE  punpcklbw mm1, mm7
415901  movq    mm4, mm1
415904  movq    mm3, mm1
415907  pcmpgtw mm4, mm6
41590A  movq    mm2, mm4
41590D  psrlw   mm2, 0Fh
415911  psllw   mm2, 8
415915  pxor    mm0, mm4
415918  pxor    mm1, mm4
41591B  psubw   mm0, mm4
41591E  psubw   mm1, mm4
415921  paddw   mm0, mm2
415924  paddw   mm1, mm2
415927  paddw   mm2, mm4
41592A  psllw   mm0, 5
41592E  psllw   mm1, 4
415932  pmulhw  mm0, mm1
415935  pxor    mm0, mm4
415938  psubw   mm0, mm4
41593B  paddw   mm0, mm2
41593E  psubw   mm0, mm3
415941  pmullw  mm0, mm5
415944  psraw   mm0, 7
415948  paddw   mm0, mm3
41594B  packuswb mm0, mm7
41594E  movd    dword ptr [edi], mm0
415951  add     edi, 4
415954  add     esi, 4
415957  dec     edx
415958  jnz     short loc_4158F5
41595A  pop     edi
41595B  pop     esi
41595C  add     edi, [ebp+var_4]
41595F  add     esi, [ebp+arg_0]
415962  dec     ecx
415963  jnz     short loc_4158F0
415965  emms
415967  pop     edi
415968  pop     esi
415969  mov     esp, ebp
41596B  pop     ebp
41596C  retn

;==== 415970 sub_415970
415970  push    ebp
415971  mov     ebp, esp
415973  sub     esp, 424h
415979  mov     eax, dword_4FB734
41597E  xor     eax, ebp
415980  mov     [ebp+var_4], eax
415983  mov     edx, [ecx]
415985  mov     eax, [ebp+arg_0]
415988  mov     ecx, [ecx+4]
41598B  mov     [ebp+var_414], edx
415991  mov     edx, [eax]
415993  mov     [ebp+var_420], edx
415999  mov     edx, [eax+4]
41599C  push    ebx
41599D  mov     ebx, [ebp+arg_4]
4159A0  mov     [ebp+var_424], ecx
4159A6  mov     ecx, [eax+8]
4159A9  mov     [ebp+var_418], edx
4159AF  mov     edx, [eax+0Ch]
4159B2  push    esi
4159B3  push    edi
4159B4  mov     [ebp+var_410], ecx
4159BA  mov     [ebp+var_41C], edx
4159C0  cmp     ebx, 100h
4159C6  jnb     short loc_415A0B
4159C8  xor     esi, esi
4159CA  xor     edi, edi
4159CC  lea     esp, [esp+0]
4159D0  mov     eax, edi
4159D2  push    1
4159D4  shr     eax, 8
4159D7  cdq
4159D8  push    10001h
4159DD  push    edx
4159DE  push    eax
4159DF  call    __allmul
4159E4  mov     [ebp+esi*8+var_408], eax
4159EB  mov     [ebp+esi*8+var_404], edx
4159F2  inc     esi
4159F3  add     edi, ebx
4159F5  cmp     esi, 80h
4159FB  jl      short loc_4159D0
4159FD  lea     eax, [ebp+var_408]
415A03  mov     [ebp+var_40C], eax
415A09  jmp     short loc_415A15
415A0B  mov     [ebp+var_40C], offset dword_50A8D0
415A15  pcmpeqw mm6, mm6
415A18  psrlw   mm6, 9
415A1C  pxor    mm7, mm7
415A1F  mov     ecx, [ebp+var_41C]
415A25  mov     edi, [ebp+var_414]
415A2B  mov     esi, [ebp+var_420]
415A31  mov     ebx, [ebp+var_40C]
415A37  push    esi
415A38  push    edi
415A39  mov     edx, [ebp+var_410]
415A3F  mov     eax, [esi]
415A41  test    eax, 0FE000000h
415A46  jz      short loc_415AA8
415A48  movd    mm0, eax
415A4B  movd    mm1, dword ptr [edi]
415A4E  punpcklbw mm0, mm7
415A51  punpcklbw mm1, mm7
415A54  movq    mm4, mm1
415A57  movq    mm3, mm1
415A5A  pcmpgtw mm4, mm6
415A5D  movq    mm2, mm4
415A60  psrlw   mm2, 0Fh
415A64  psllw   mm2, 8
415A68  shr     eax, 19h
415A6B  pxor    mm0, mm4
415A6E  pxor    mm1, mm4
415A71  psubw   mm0, mm4
415A74  psubw   mm1, mm4
415A77  paddw   mm0, mm2
415A7A  paddw   mm1, mm2
415A7D  paddw   mm2, mm4
415A80  psllw   mm0, 5
415A84  psllw   mm1, 4
415A88  pmulhw  mm0, mm1
415A8B  pxor    mm0, mm4
415A8E  psubw   mm0, mm4
415A91  paddw   mm0, mm2
415A94  psubw   mm0, mm3
415A97  pmullw  mm0, qword ptr [ebx+eax*8]
415A9B  psraw   mm0, 7
415A9F  paddw   mm0, mm3
415AA2  packuswb mm0, mm7
415AA5  movd    dword ptr [edi], mm0
415AA8  add     edi, 4
415AAB  add     esi, 4
415AAE  dec     edx
415AAF  jnz     short loc_415A3F
415AB1  pop     edi
415AB2  pop     esi
415AB3  add     edi, [ebp+var_424]
415AB9  add     esi, [ebp+var_418]
415ABF  dec     ecx
415AC0  jnz     loc_415A37
415AC6  emms
415AC8  mov     ecx, [ebp+var_4]
415ACB  pop     edi
415ACC  pop     esi
415ACD  xor     ecx, ebp
415ACF  pop     ebx
415AD0  call    sub_4ABDD5
415AD5  mov     esp, ebp
415AD7  pop     ebp
415AD8  retn

;==== 415AE0 sub_415AE0
415AE0  push    ecx
415AE1  test    esi, esi
415AE3  jz      short loc_415B20
415AE5  mov     edx, [eax+10h]
415AE8  dec     edx
415AE9  jz      short loc_415B00
415AEB  dec     edx
415AEC  jnz     short loc_415B20
415AEE  mov     edx, [ecx+10h]
415AF1  dec     edx
415AF2  jnz     short loc_415B20
415AF4  push    esi
415AF5  push    eax
415AF6  call    sub_415CC0
415AFB  add     esp, 8
415AFE  pop     ecx
415AFF  retn
415B00  cmp     dword ptr [ecx+10h], 1
415B04  jnz     short loc_415B20
415B06  cmp     esi, 100h
415B0C  jnb     short loc_415B1B
415B0E  push    ecx
415B0F  mov     ecx, esi
415B11  call    sub_415BE0
415B16  add     esp, 4
415B19  pop     ecx
415B1A  retn
415B1B  call    sub_415B30
415B20  pop     ecx
415B21  retn

;==== 415B30 sub_415B30
415B30  push    ebp
415B31  mov     ebp, esp
415B33  sub     esp, 18h
415B36  mov     edx, [ecx]
415B38  mov     ecx, [ecx+4]
415B3B  mov     [ebp+var_14], edx
415B3E  mov     edx, [eax]
415B40  mov     [ebp+var_10], edx
415B43  mov     edx, [eax+4]
415B46  mov     [ebp+var_8], ecx
415B49  mov     ecx, [eax+8]
415B4C  mov     [ebp+var_4], edx
415B4F  mov     edx, [eax+0Ch]
415B52  push    esi
415B53  push    edi
415B54  mov     [ebp+var_C], ecx
415B57  mov     [ebp+var_18], edx
415B5A  pcmpeqw mm6, mm6
415B5D  psrlw   mm6, 9
415B61  pxor    mm7, mm7
415B64  mov     ecx, [ebp+var_18]
415B67  mov     edi, [ebp+var_14]
415B6A  mov     esi, [ebp+var_10]
415B6D  push    esi
415B6E  push    edi
415B6F  mov     edx, [ebp+var_C]
415B72  movd    mm0, dword ptr [esi]
415B75  movd    mm1, dword ptr [edi]
415B78  punpcklbw mm0, mm7
415B7B  punpcklbw mm1, mm7
415B7E  movq    mm4, mm0
415B81  pcmpgtw mm4, mm6
415B84  movq    mm2, mm4
415B87  psrlw   mm2, 0Fh
415B8B  psllw   mm2, 8
415B8F  pxor    mm0, mm4
415B92  pxor    mm1, mm4
415B95  psubw   mm0, mm4
415B98  psubw   mm1, mm4
415B9B  paddw   mm0, mm2
415B9E  paddw   mm1, mm2
415BA1  paddw   mm2, mm4
415BA4  psllw   mm0, 5
415BA8  psllw   mm1, 4
415BAC  pmulhw  mm0, mm1
415BAF  pxor    mm0, mm4
415BB2  psubw   mm0, mm4
415BB5  paddw   mm0, mm2
415BB8  packuswb mm0, mm7
415BBB  movd    dword ptr [edi], mm0
415BBE  add     edi, 4
415BC1  add     esi, 4
415BC4  dec     edx
415BC5  jnz     short loc_415B72
415BC7  pop     edi
415BC8  pop     esi
415BC9  add     edi, [ebp+var_8]
415BCC  add     esi, [ebp+var_4]
415BCF  dec     ecx
415BD0  jnz     short loc_415B6D
415BD2  emms
415BD4  pop     edi
415BD5  pop     esi
415BD6  mov     esp, ebp
415BD8  pop     ebp
415BD9  retn

;==== 415BE0 sub_415BE0
415BE0  push    ebp
415BE1  mov     ebp, esp
415BE3  sub     esp, 20h
415BE6  mov     edx, [ebp+arg_0]
415BE9  push    esi
415BEA  mov     esi, [edx]
415BEC  mov     edx, [edx+4]
415BEF  mov     [ebp+var_4], edx
415BF2  mov     edx, [eax+4]
415BF5  mov     [ebp+arg_0], edx
415BF8  mov     edx, [eax+8]
415BFB  mov     [ebp+var_10], esi
415BFE  mov     esi, [eax]
415C00  mov     eax, [eax+0Ch]
415C03  mov     [ebp+var_14], eax
415C06  mov     [ebp+var_8], edx
415C09  mov     eax, ecx
415C0B  and     eax, 1FEh
415C10  mov     edx, 80008000h
415C15  mul     edx
415C17  shr     ecx, 1
415C19  mov     [ebp+var_C], esi
415C1C  xor     esi, esi
415C1E  add     eax, ecx
415C20  adc     edx, esi
415C22  push    edi
415C23  mov     dword ptr [ebp+var_20], eax
415C26  mov     dword ptr [ebp+var_20+4], edx
415C29  movq    mm5, [ebp+var_20]
415C2D  pcmpeqw mm6, mm6
415C30  psrlw   mm6, 9
415C34  pxor    mm7, mm7
415C37  mov     ecx, [ebp+var_14]
415C3A  mov     edi, [ebp+var_10]
415C3D  mov     esi, [ebp+var_C]
415C40  push    esi
415C41  push    edi
415C42  mov     edx, [ebp+var_8]
415C45  movd    mm0, dword ptr [esi]
415C48  movd    mm1, dword ptr [edi]
415C4B  punpcklbw mm0, mm7
415C4E  punpcklbw mm1, mm7
415C51  movq    mm4, mm0
415C54  movq    mm3, mm1
415C57  pcmpgtw mm4, mm6
415C5A  movq    mm2, mm4
415C5D  psrlw   mm2, 0Fh
415C61  psllw   mm2, 8
415C65  pxor    mm0, mm4
415C68  pxor    mm1, mm4
415C6B  psubw   mm0, mm4
415C6E  psubw   mm1, mm4
415C71  paddw   mm0, mm2
415C74  paddw   mm1, mm2
415C77  paddw   mm2, mm4
415C7A  psllw   mm0, 5
415C7E  psllw   mm1, 4
415C82  pmulhw  mm0, mm1
415C85  pxor    mm0, mm4
415C88  psubw   mm0, mm4
415C8B  paddw   mm0, mm2
415C8E  psubw   mm0, mm3
415C91  pmullw  mm0, mm5
415C94  psraw   mm0, 7
415C98  paddw   mm0, mm3
415C9B  packuswb mm0, mm7
415C9E  movd    dword ptr [edi], mm0
415CA1  add     edi, 4
415CA4  add     esi, 4
415CA7  dec     edx
415CA8  jnz     short loc_415C45
415CAA  pop     edi
415CAB  pop     esi
415CAC  add     edi, [ebp+var_4]
415CAF  add     esi, [ebp+arg_0]
415CB2  dec     ecx
415CB3  jnz     short loc_415C40
415CB5  emms
415CB7  pop     edi
415CB8  pop     esi
415CB9  mov     esp, ebp
415CBB  pop     ebp
415CBC  retn

;==== 415CC0 sub_415CC0
415CC0  push    ebp
415CC1  mov     ebp, esp
415CC3  sub     esp, 424h
415CC9  mov     eax, dword_4FB734
415CCE  xor     eax, ebp
415CD0  mov     [ebp+var_4], eax
415CD3  mov     edx, [ecx]
415CD5  mov     eax, [ebp+arg_0]
415CD8  mov     ecx, [ecx+4]
415CDB  mov     [ebp+var_414], edx
415CE1  mov     edx, [eax]
415CE3  mov     [ebp+var_420], edx
415CE9  mov     edx, [eax+4]
415CEC  push    ebx
415CED  mov     ebx, [ebp+arg_4]
415CF0  mov     [ebp+var_424], ecx
415CF6  mov     ecx, [eax+8]
415CF9  mov     [ebp+var_418], edx
415CFF  mov     edx, [eax+0Ch]
415D02  push    esi
415D03  push    edi
415D04  mov     [ebp+var_410], ecx
415D0A  mov     [ebp+var_41C], edx
415D10  cmp     ebx, 100h
415D16  jnb     short loc_415D5B
415D18  xor     esi, esi
415D1A  xor     edi, edi
415D1C  lea     esp, [esp+0]
415D20  mov     eax, edi
415D22  push    1
415D24  shr     eax, 8
415D27  cdq
415D28  push    10001h
415D2D  push    edx
415D2E  push    eax
415D2F  call    __allmul
415D34  mov     [ebp+esi*8+var_408], eax
415D3B  mov     [ebp+esi*8+var_404], edx
415D42  inc     esi
415D43  add     edi, ebx
415D45  cmp     esi, 80h
415D4B  jl      short loc_415D20
415D4D  lea     eax, [ebp+var_408]
415D53  mov     [ebp+var_40C], eax
415D59  jmp     short loc_415D65
415D5B  mov     [ebp+var_40C], offset dword_50A8D0
415D65  pcmpeqw mm6, mm6
415D68  psrlw   mm6, 9
415D6C  pxor    mm7, mm7
415D6F  mov     ecx, [ebp+var_41C]
415D75  mov     edi, [ebp+var_414]
415D7B  mov     esi, [ebp+var_420]
415D81  mov     ebx, [ebp+var_40C]
415D87  push    esi
415D88  push    edi
415D89  mov     edx, [ebp+var_410]
415D8F  mov     eax, [esi]
415D91  test    eax, 0FE000000h
415D96  jz      short loc_415DF8
415D98  movd    mm0, eax
415D9B  movd    mm1, dword ptr [edi]
415D9E  punpcklbw mm0, mm7
415DA1  punpcklbw mm1, mm7
415DA4  movq    mm4, mm0
415DA7  movq    mm3, mm1
415DAA  pcmpgtw mm4, mm6
415DAD  movq    mm2, mm4
415DB0  psrlw   mm2, 0Fh
415DB4  psllw   mm2, 8
415DB8  shr     eax, 19h
415DBB  pxor    mm0, mm4
415DBE  pxor    mm1, mm4
415DC1  psubw   mm0, mm4
415DC4  psubw   mm1, mm4
415DC7  paddw   mm0, mm2
415DCA  paddw   mm1, mm2
415DCD  paddw   mm2, mm4
415DD0  psllw   mm0, 5
415DD4  psllw   mm1, 4
415DD8  pmulhw  mm0, mm1
415DDB  pxor    mm0, mm4
415DDE  psubw   mm0, mm4
415DE1  paddw   mm0, mm2
415DE4  psubw   mm0, mm3
415DE7  pmullw  mm0, qword ptr [ebx+eax*8]
415DEB  psraw   mm0, 7
415DEF  paddw   mm0, mm3
415DF2  packuswb mm0, mm7
415DF5  movd    dword ptr [edi], mm0
415DF8  add     edi, 4
415DFB  add     esi, 4
415DFE  dec     edx
415DFF  jnz     short loc_415D8F
415E01  pop     edi
415E02  pop     esi
415E03  add     edi, [ebp+var_424]
415E09  add     esi, [ebp+var_418]
415E0F  dec     ecx
415E10  jnz     loc_415D87
415E16  emms
415E18  mov     ecx, [ebp+var_4]
415E1B  pop     edi
415E1C  pop     esi
415E1D  xor     ecx, ebp
415E1F  pop     ebx
415E20  call    sub_4ABDD5
415E25  mov     esp, ebp
415E27  pop     ebp
415E28  retn

;==== 416150 sub_416150
416150  push    ecx
416151  test    esi, esi
416153  jz      short loc_41618A
416155  mov     edx, [eax+10h]
416158  sub     edx, 2
41615B  jnz     short loc_41618A
41615D  mov     edx, [ecx+10h]
416160  dec     edx
416161  jz      short loc_416180
416163  dec     edx
416164  jnz     short loc_41618A
416166  cmp     esi, 100h
41616C  jnb     short loc_416179
41616E  push    esi
41616F  call    sub_4162C0
416174  add     esp, 4
416177  pop     ecx
416178  retn
416179  call    sub_416340
41617E  pop     ecx
41617F  retn
416180  push    esi
416181  push    eax
416182  call    sub_416190
416187  add     esp, 8
41618A  pop     ecx
41618B  retn

;==== 416190 sub_416190
416190  push    ebp
416191  mov     ebp, esp
416193  sub     esp, 824h
416199  mov     eax, dword_4FB734
41619E  xor     eax, ebp
4161A0  mov     [ebp+var_4], eax
4161A3  mov     eax, [ebp+arg_0]
4161A6  mov     edx, [ecx]
4161A8  mov     ecx, [ecx+4]
4161AB  mov     [ebp+var_820], edx
4161B1  mov     edx, [eax]
4161B3  mov     [ebp+var_810], edx
4161B9  mov     edx, [eax+4]
4161BC  push    ebx
4161BD  push    esi
4161BE  mov     [ebp+var_824], ecx
4161C4  mov     ecx, [eax+8]
4161C7  mov     [ebp+var_818], edx
4161CD  mov     edx, [eax+0Ch]
4161D0  xor     esi, esi
4161D2  xor     eax, eax
4161D4  push    edi
4161D5  mov     [ebp+var_81C], ecx
4161DB  mov     [ebp+var_814], edx
4161E1  mov     [ebp+var_80C], eax
4161E7  jmp     short loc_4161F0
4161F0  shr     eax, 8
4161F3  mov     ecx, 100h
4161F8  sub     ecx, eax
4161FA  mov     eax, ecx
4161FC  shl     eax, 4
4161FF  cdq
416200  mov     edi, eax
416202  push    10h
416204  mov     eax, ecx
416206  mov     ebx, edx
416208  cdq
416209  push    100000h
41620E  push    edx
41620F  push    eax
416210  call    __allmul
416215  add     edi, eax
416217  mov     eax, [ebp+var_80C]
41621D  adc     ebx, edx
41621F  add     eax, [ebp+arg_4]
416222  mov     dword ptr [ebp+esi*8+var_808], edi
416229  mov     dword ptr [ebp+esi*8+var_808+4], ebx
416230  inc     esi
416231  mov     [ebp+var_80C], eax
416237  cmp     esi, 100h
41623D  jl      short loc_4161F0
41623F  pxor    mm6, mm6
416242  movd    mm7, [ebp+var_81C]
416249  mov     edx, [ebp+var_814]
41624F  mov     edi, [ebp+var_820]
416255  mov     esi, [ebp+var_810]
41625B  xor     ebx, ebx
41625D  movd    ecx, mm7
416260  mov     eax, [esi+ebx]
416263  test    eax, 0FF000000h
416268  jz      short loc_41628A
41626A  shr     eax, 18h
41626D  movd    mm0, dword ptr [edi+ebx]
416271  movq    mm1, [ebp+eax*8+var_808]
416279  punpcklbw mm0, mm6
41627C  psllw   mm0, 4
416280  pmulhw  mm0, mm1
416283  packuswb mm0, mm6
416286  movd    dword ptr [edi+ebx], mm0
41628A  add     ebx, 4
41628D  dec     ecx
41628E  jnz     short loc_416260
416290  add     edi, [ebp+var_824]
416296  add     esi, [ebp+var_818]
41629C  dec     edx
41629D  jnz     short loc_41625B
41629F  emms
4162A1  mov     ecx, [ebp+var_4]
4162A4  pop     edi
4162A5  pop     esi
4162A6  xor     ecx, ebp
4162A8  pop     ebx
4162A9  call    sub_4ABDD5
4162AE  mov     esp, ebp
4162B0  pop     ebp
4162B1  retn

;==== 4162C0 sub_4162C0
4162C0  push    ebp
4162C1  mov     ebp, esp
4162C3  sub     esp, 18h
4162C6  mov     edx, [ecx]
4162C8  mov     ecx, [ecx+4]
4162CB  mov     [ebp+var_14], edx
4162CE  mov     edx, [eax]
4162D0  mov     [ebp+var_10], edx
4162D3  mov     edx, [eax+4]
4162D6  push    ebx
4162D7  mov     [ebp+var_4], ecx
4162DA  mov     ecx, [eax+8]
4162DD  mov     [ebp+var_8], edx
4162E0  mov     edx, [eax+0Ch]
4162E3  push    esi
4162E4  push    edi
4162E5  mov     [ebp+var_C], ecx
4162E8  mov     [ebp+var_18], edx
4162EB  mov     ecx, [ebp+var_18]
4162EE  mov     edi, [ebp+var_14]
4162F1  mov     esi, [ebp+var_10]
4162F4  push    ecx
4162F5  push    edi
4162F6  push    esi
4162F7  mov     ecx, [ebp+var_C]
4162FA  xor     edx, edx
4162FC  xor     ebx, ebx
4162FE  mov     dl, [esi+3]
416301  add     esi, 4
416304  and     edx, edx
416306  mov     eax, 100h
41630B  jz      short loc_416322
41630D  imul    edx, [ebp+arg_0]
416311  mov     bl, [edi+3]
416314  shr     edx, 8
416317  sub     eax, edx
416319  imul    ebx, eax
41631C  shr     ebx, 8
41631F  mov     [edi+3], bl
416322  add     edi, 4
416325  dec     ecx
416326  jnz     short loc_4162FA
416328  pop     esi
416329  pop     edi
41632A  add     esi, [ebp+var_8]
41632D  pop     ecx
41632E  add     edi, [ebp+var_4]
416331  dec     ecx
416332  jnz     short loc_4162F4
416334  pop     edi
416335  pop     esi
416336  pop     ebx
416337  mov     esp, ebp
416339  pop     ebp
41633A  retn

;==== 416340 sub_416340
416340  push    ebp
416341  mov     ebp, esp
416343  sub     esp, 18h
416346  mov     edx, [ecx]
416348  mov     ecx, [ecx+4]
41634B  mov     [ebp+var_14], edx
41634E  mov     edx, [eax]
416350  mov     [ebp+var_10], edx
416353  mov     edx, [eax+4]
416356  push    ebx
416357  mov     [ebp+var_4], ecx
41635A  mov     ecx, [eax+8]
41635D  mov     [ebp+var_8], edx
416360  mov     edx, [eax+0Ch]
416363  push    esi
416364  push    edi
416365  mov     [ebp+var_C], ecx
416368  mov     [ebp+var_18], edx
41636B  mov     ecx, [ebp+var_18]
41636E  mov     edi, [ebp+var_14]
416371  mov     esi, [ebp+var_10]
416374  push    ecx
416375  push    edi
416376  push    esi
416377  mov     ecx, [ebp+var_C]
41637A  xor     edx, edx
41637C  xor     ebx, ebx
41637E  mov     dl, [esi+3]
416381  add     esi, 4
416384  and     edx, edx
416386  mov     eax, 100h
41638B  jz      short loc_41639B
41638D  sub     eax, edx
41638F  mov     bl, [edi+3]
416392  imul    ebx, eax
416395  shr     ebx, 8
416398  mov     [edi+3], bl
41639B  add     edi, 4
41639E  dec     ecx
41639F  jnz     short loc_41637A
4163A1  pop     esi
4163A2  pop     edi
4163A3  add     esi, [ebp+var_8]
4163A6  pop     ecx
4163A7  add     edi, [ebp+var_4]
4163AA  dec     ecx
4163AB  jnz     short loc_416374
4163AD  pop     edi
4163AE  pop     esi
4163AF  pop     ebx
4163B0  mov     esp, ebp
4163B2  pop     ebp
4163B3  retn

;==== 419E50 sub_419E50
419E50  push    ebp
419E51  mov     ebp, esp
419E53  mov     eax, dword_565B2C
419E58  push    esi
419E59  xor     esi, esi
419E5B  test    eax, eax
419E5D  jz      short loc_419ED6
419E5F  mov     ecx, [edx+0Ch]
419E62  push    ebx
419E63  mov     ebx, [edx+8]
419E66  imul    ecx, ebx
419E69  cmp     ecx, 0A00h
419E6F  jb      short loc_419ED0
419E71  call    sub_454260
419E76  mov     ecx, eax
419E78  cmp     ecx, 2
419E7B  jb      short loc_419ED0
419E7D  mov     esi, [edx+0Ch]
419E80  xor     edx, edx
419E82  mov     eax, esi
419E84  shl     eax, 10h
419E87  div     ecx
419E89  push    edi
419E8A  xor     edx, edx
419E8C  mov     edi, eax
419E8E  mov     eax, esi
419E90  div     ecx
419E92  imul    eax, ebx
419E95  cmp     eax, 0A000h
419E9A  jbe     short loc_419EAD
419E9C  lea     esp, [esp+0]
419EA0  shr     eax, 1
419EA2  add     ecx, ecx
419EA4  shr     edi, 1
419EA6  cmp     eax, 0A000h
419EAB  ja      short loc_419EA0
419EAD  cmp     edi, 10000h
419EB3  jb      short loc_419EC9
419EB5  mov     eax, [ebp+arg_4]
419EB8  mov     edx, [ebp+arg_0]
419EBB  mov     [eax], edi
419EBD  pop     edi
419EBE  pop     ebx
419EBF  mov     [edx], ecx
419EC1  mov     eax, 1
419EC6  pop     esi
419EC7  pop     ebp
419EC8  retn
419EC9  pop     edi
419ECA  pop     ebx
419ECB  xor     eax, eax
419ECD  pop     esi
419ECE  pop     ebp
419ECF  retn
419ED0  pop     ebx
419ED1  mov     eax, esi
419ED3  pop     esi
419ED4  pop     ebp
419ED5  retn
419ED6  mov     eax, esi
419ED8  pop     esi
419ED9  pop     ebp
419EDA  retn

;==== 419EE0 sub_419EE0
419EE0  push    ebp
419EE1  mov     ebp, esp
419EE3  push    ecx
419EE4  push    ebx
419EE5  push    esi
419EE6  mov     esi, [ebp+arg_8]
419EE9  push    edi
419EEA  xor     edi, edi
419EEC  xor     ebx, ebx
419EEE  mov     [ebp+var_4], ecx
419EF1  test    esi, esi
419EF3  jz      loc_419F88
419EF9  lea     esp, [esp+0]
419F00  add     ebx, [ebp+arg_C]
419F03  xor     edx, edx
419F05  dec     esi
419F06  mov     [ebp+arg_8], ebx
419F09  cmp     [ebp+arg_4], edx
419F0C  jbe     short loc_419F5F
419F0E  mov     edi, edi
419F10  mov     ecx, [ebp+arg_0]
419F13  mov     ecx, [ecx+edx*4]
419F16  mov     ebx, [ecx]
419F18  mov     [eax], ebx
419F1A  mov     ebx, [ecx+4]
419F1D  mov     [eax+4], ebx
419F20  mov     ebx, [ecx+8]
419F23  mov     [eax+8], ebx
419F26  mov     ebx, [ecx+0Ch]
419F29  mov     [eax+0Ch], ebx
419F2C  mov     ebx, [ecx+10h]
419F2F  mov     [eax+10h], ebx
419F32  mov     ecx, [ecx+14h]
419F35  mov     ebx, [ebp+arg_8]
419F38  mov     [eax+14h], ecx
419F3B  mov     ecx, [eax+4]
419F3E  imul    ecx, edi
419F41  add     [eax], ecx
419F43  test    esi, esi
419F45  jz      short loc_419F4E
419F47  mov     ecx, ebx
419F49  shr     ecx, 10h
419F4C  jmp     short loc_419F53
419F4E  mov     ecx, [eax+0Ch]
419F51  sub     ecx, edi
419F53  mov     [eax+0Ch], ecx
419F56  inc     edx
419F57  add     eax, 18h
419F5A  cmp     edx, [ebp+arg_4]
419F5D  jb      short loc_419F10
419F5F  mov     ecx, [ebp+var_4]
419F62  test    ecx, ecx
419F64  jz      short loc_419F73
419F66  mov     edx, ebx
419F68  shr     edx, 10h
419F6B  mov     [ecx], edx
419F6D  add     ecx, 4
419F70  mov     [ebp+var_4], ecx
419F73  mov     ecx, ebx
419F75  shr     ecx, 10h
419F78  add     edi, ecx
419F7A  and     ebx, 0FFFFh
419F80  test    esi, esi
419F82  jnz     loc_419F00
419F88  pop     edi
419F89  pop     esi
419F8A  pop     ebx
419F8B  mov     esp, ebp
419F8D  pop     ebp
419F8E  retn

;==== 419F90 sub_419F90
419F90  push    ebp
419F91  mov     ebp, esp
419F93  sub     esp, 18h
419F96  mov     ecx, [eax]
419F98  mov     edx, [eax+4]
419F9B  push    ebx
419F9C  mov     ebx, [ebp+arg_4]
419F9F  push    esi
419FA0  push    edi
419FA1  mov     [ebp+var_14], ecx
419FA4  mov     ecx, [eax+8]
419FA7  mov     [ebp+var_10], edx
419FAA  mov     edx, [eax+0Ch]
419FAD  xor     edi, edi
419FAF  mov     [ebp+var_C], ecx
419FB2  mov     [ebp+var_8], edx
419FB5  test    ebx, ebx
419FB7  jz      short loc_419FF7
419FB9  mov     esi, [ebp+arg_0]
419FBC  mov     eax, [ebp+var_14]
419FBF  mov     ecx, [ebp+var_10]
419FC2  mov     edx, [ebp+var_C]
419FC5  add     edi, [ebp+arg_8]
419FC8  mov     [esi], eax
419FCA  mov     eax, [ebp+var_8]
419FCD  mov     [esi+4], ecx
419FD0  mov     [esi+8], edx
419FD3  mov     ecx, edi
419FD5  lea     edx, [ebp+var_14]
419FD8  sar     ecx, 10h
419FDB  push    edx
419FDC  neg     ecx
419FDE  xor     edx, edx
419FE0  mov     [esi+0Ch], eax
419FE3  call    sub_409160
419FE8  add     esp, 4
419FEB  and     edi, 0FFFFh
419FF1  add     esi, 10h
419FF4  dec     ebx
419FF5  jnz     short loc_419FBC
419FF7  pop     edi
419FF8  pop     esi
419FF9  pop     ebx
419FFA  mov     esp, ebp
419FFC  pop     ebp
419FFD  retn

;==== 41A000 sub_41A000
41A000  push    ebp
41A001  mov     ebp, esp
41A003  mov     edx, [ecx+4]
41A006  push    ebx
41A007  push    esi
41A008  mov     esi, [ecx]
41A00A  push    edi
41A00B  mov     edi, eax
41A00D  xor     eax, eax
41A00F  test    edi, edi
41A011  jz      short loc_41A033
41A013  mov     ecx, [ebp+arg_0]
41A016  add     eax, [ebp+arg_4]
41A019  mov     [ecx+4], edx
41A01C  mov     ebx, eax
41A01E  and     ebx, 0FFFF0000h
41A024  mov     [ecx], esi
41A026  sub     edx, ebx
41A028  and     eax, 0FFFFh
41A02D  add     ecx, 8
41A030  dec     edi
41A031  jnz     short loc_41A016
41A033  pop     edi
41A034  pop     esi
41A035  pop     ebx
41A036  pop     ebp
41A037  retn

;==== 41A040 sub_41A040
41A040  push    ebp
41A041  mov     ebp, esp
41A043  sub     esp, 0Ch
41A046  mov     edx, [ecx]
41A048  mov     ecx, [ecx+4]
41A04B  push    ebx
41A04C  push    esi
41A04D  mov     esi, eax
41A04F  xor     eax, eax
41A051  mov     [ebp+var_8], ecx
41A054  test    esi, esi
41A056  jz      short loc_41A08D
41A058  mov     ecx, [ebp+arg_0]
41A05B  mov     ebx, [ebp+var_8]
41A05E  fld     [ebp+var_8]
41A061  add     eax, edi
41A063  mov     [ecx+4], ebx
41A066  mov     ebx, eax
41A068  shr     ebx, 10h
41A06B  mov     [ebp+var_4], ebx
41A06E  fild    [ebp+var_4]
41A071  mov     [ecx], edx
41A073  test    ebx, ebx
41A075  jns     short loc_41A07D
41A077  fadd    ds:flt_4EC9C0
41A07D  fsubp   st(1), st
41A07F  and     eax, 0FFFFh
41A084  add     ecx, 8
41A087  dec     esi
41A088  fstp    [ebp+var_8]
41A08B  jnz     short loc_41A05B
41A08D  pop     esi
41A08E  pop     ebx
41A08F  mov     esp, ebp
41A091  pop     ebp
41A092  retn

;==== 41A0A0 sub_41A0A0
41A0A0  push    ebp
41A0A1  mov     ebp, esp
41A0A3  sub     esp, 0Ch
41A0A6  mov     ecx, [eax]
41A0A8  mov     edx, [eax+8]
41A0AB  push    ebx
41A0AC  mov     ebx, [eax+4]
41A0AF  push    esi
41A0B0  xor     esi, esi
41A0B2  mov     [ebp+var_4], ecx
41A0B5  mov     [ebp+var_C], ebx
41A0B8  cmp     [ebp+arg_4], esi
41A0BB  jbe     short loc_41A118
41A0BD  mov     ecx, [ebp+arg_0]
41A0C0  push    edi
41A0C1  mov     eax, [ebp+arg_8]
41A0C4  mov     eax, [eax+esi*4]
41A0C7  cmp     eax, edx
41A0C9  jg      short loc_41A0D2
41A0CB  mov     edi, eax
41A0CD  mov     [ebp+var_8], edi
41A0D0  jmp     short loc_41A0D7
41A0D2  mov     edi, edx
41A0D4  mov     [ebp+var_8], edx
41A0D7  sub     eax, edi
41A0D9  cmp     eax, ebx
41A0DB  jle     short loc_41A0DF
41A0DD  mov     eax, ebx
41A0DF  cmp     eax, 1
41A0E2  jl      short loc_41A0FB
41A0E4  mov     edi, [ebp+var_4]
41A0E7  mov     ebx, eax
41A0E9  shl     ebx, 5
41A0EC  mov     [ecx], edi
41A0EE  add     edi, ebx
41A0F0  mov     ebx, [ebp+var_C]
41A0F3  mov     [ebp+var_4], edi
41A0F6  mov     edi, [ebp+var_8]
41A0F9  jmp     short loc_41A101
41A0FB  mov     dword ptr [ecx], 0
41A101  mov     [ecx+8], edx
41A104  mov     [ecx+4], eax
41A107  sub     ebx, eax
41A109  inc     esi
41A10A  sub     edx, edi
41A10C  add     ecx, 0Ch
41A10F  mov     [ebp+var_C], ebx
41A112  cmp     esi, [ebp+arg_4]
41A115  jb      short loc_41A0C1
41A117  pop     edi
41A118  pop     esi
41A119  pop     ebx
41A11A  mov     esp, ebp
41A11C  pop     ebp
41A11D  retn

;==== 41A120 sub_41A120
41A120  push    ebp
41A121  mov     ebp, esp
41A123  sub     esp, 24h
41A126  push    ebx
41A127  push    esi
41A128  push    edi
41A129  lea     eax, [ebp+var_C]
41A12C  push    eax
41A12D  lea     ecx, [ebp+var_8]
41A130  push    ecx
41A131  xor     esi, esi
41A133  call    sub_419E50
41A138  add     esp, 8
41A13B  test    eax, eax
41A13D  jz      loc_41A2F2
41A143  mov     ebx, [ebp+var_8]
41A146  mov     eax, ebx
41A148  imul    eax, [ebp+arg_8]
41A14C  xor     ecx, ecx
41A14E  mov     edx, 18h
41A153  mul     edx
41A155  seto    cl
41A158  neg     ecx
41A15A  or      ecx, eax
41A15C  push    ecx; unsigned int
41A15D  call    ??_U@YAPAXI@Z; operator new[](uint)
41A162  add     esp, 4
41A165  cmp     [ebp+arg_C], 4
41A169  mov     [ebp+var_8], eax
41A16C  jnz     short loc_41A18E
41A16E  xor     ecx, ecx
41A170  mov     eax, ebx
41A172  mov     edx, 4
41A177  mul     edx
41A179  seto    cl
41A17C  neg     ecx
41A17E  or      ecx, eax
41A180  push    ecx; unsigned int
41A181  call    ??_U@YAPAXI@Z; operator new[](uint)
41A186  add     esp, 4
41A189  mov     [ebp+var_4], eax
41A18C  jmp     short loc_41A191
41A18E  mov     [ebp+var_4], esi
41A191  mov     edi, [ebp+var_C]
41A194  mov     eax, [ebp+arg_8]
41A197  mov     ecx, [ebp+arg_4]
41A19A  push    edi
41A19B  push    ebx
41A19C  push    eax
41A19D  mov     eax, [ebp+var_8]
41A1A0  push    ecx
41A1A1  mov     ecx, [ebp+var_4]
41A1A4  xor     esi, esi
41A1A6  call    sub_419EE0
41A1AB  mov     eax, [ebp+arg_C]
41A1AE  dec     eax; switch 4 cases
41A1AF  add     esp, 10h
41A1B2  cmp     eax, 3
41A1B5  ja      def_41A1BB; jumptable 0041A1BB default case
41A1BB  jmp     ds:jpt_41A1BB[eax*4]; switch jump
41A1C2  xor     ecx, ecx; jumptable 0041A1BB case 1
41A1C4  mov     eax, ebx
41A1C6  mov     edx, 10h
41A1CB  mul     edx
41A1CD  seto    cl
41A1D0  neg     ecx
41A1D2  or      ecx, eax
41A1D4  push    ecx; unsigned int
41A1D5  call    ??_U@YAPAXI@Z; operator new[](uint)
41A1DA  add     esp, 4
41A1DD  push    edi
41A1DE  mov     esi, eax
41A1E0  mov     eax, [ebp+arg_10]
41A1E3  push    ebx
41A1E4  push    esi
41A1E5  call    sub_419F90
41A1EA  jmp     loc_41A275
41A1EF  xor     ecx, ecx; jumptable 0041A1BB case 2
41A1F1  mov     eax, ebx
41A1F3  mov     edx, 8
41A1F8  mul     edx
41A1FA  seto    cl
41A1FD  neg     ecx
41A1FF  or      ecx, eax
41A201  push    ecx; unsigned int
41A202  call    ??_U@YAPAXI@Z; operator new[](uint)
41A207  mov     ecx, [ebp+arg_10]
41A20A  add     esp, 4
41A20D  mov     esi, eax
41A20F  push    edi
41A210  push    esi
41A211  mov     eax, ebx
41A213  call    sub_41A000
41A218  add     esp, 8
41A21B  jmp     short def_41A1BB; jumptable 0041A1BB default case
41A21D  xor     ecx, ecx; jumptable 0041A1BB case 3
41A21F  mov     eax, ebx
41A221  mov     edx, 8
41A226  mul     edx
41A228  seto    cl
41A22B  neg     ecx
41A22D  or      ecx, eax
41A22F  push    ecx; unsigned int
41A230  call    ??_U@YAPAXI@Z; operator new[](uint)
41A235  mov     ecx, [ebp+arg_10]
41A238  mov     esi, eax
41A23A  add     esp, 4
41A23D  push    esi
41A23E  mov     eax, ebx
41A240  call    sub_41A040
41A245  add     esp, 4
41A248  jmp     short def_41A1BB; jumptable 0041A1BB default case
41A24A  xor     ecx, ecx; jumptable 0041A1BB case 4
41A24C  mov     eax, ebx
41A24E  mov     edx, 0Ch
41A253  mul     edx
41A255  seto    cl
41A258  neg     ecx
41A25A  or      ecx, eax
41A25C  push    ecx; unsigned int
41A25D  call    ??_U@YAPAXI@Z; operator new[](uint)
41A262  mov     esi, eax
41A264  mov     eax, [ebp+var_4]
41A267  add     esp, 4
41A26A  push    eax
41A26B  mov     eax, [ebp+arg_10]
41A26E  push    ebx
41A26F  push    esi
41A270  call    sub_41A0A0
41A275  add     esp, 0Ch
41A278  mov     ecx, [ebp+var_4]; jumptable 0041A1BB default case
41A27B  push    ecx; void *
41A27C  call    ??_V@YAXPAX@Z; operator delete[](void *)
41A281  mov     edx, [ebp+var_8]
41A284  mov     eax, [ebp+arg_8]
41A287  mov     ecx, [ebp+arg_14]
41A28A  mov     edi, dword_565B2C
41A290  mov     [ebp+var_1C], edx
41A293  add     esp, 4
41A296  lea     edx, [ebp+var_24]
41A299  mov     [ebp+var_18], eax
41A29C  mov     [ebp+var_10], ecx
41A29F  mov     ecx, [ebp+arg_0]
41A2A2  push    edx
41A2A3  mov     eax, edi
41A2A5  mov     [ebp+var_24], 0
41A2AC  mov     [ebp+var_20], ebx
41A2AF  mov     [ebp+var_14], esi
41A2B2  call    sub_4468F0
41A2B7  mov     eax, 1
41A2BC  mov     ecx, edi
41A2BE  call    sub_446910
41A2C3  mov     eax, dword_565B2C
41A2C8  push    0
41A2CA  xor     ecx, ecx
41A2CC  call    sub_4468F0
41A2D1  mov     eax, [ebp+var_8]
41A2D4  push    eax; void *
41A2D5  call    ??_V@YAXPAX@Z; operator delete[](void *)
41A2DA  add     esp, 4
41A2DD  push    esi; void *
41A2DE  call    ??_V@YAXPAX@Z; operator delete[](void *)
41A2E3  add     esp, 4
41A2E6  mov     eax, 1
41A2EB  pop     edi
41A2EC  pop     esi
41A2ED  pop     ebx
41A2EE  mov     esp, ebp
41A2F0  pop     ebp
41A2F1  retn
41A2F2  pop     edi
41A2F3  mov     eax, esi
41A2F5  pop     esi
41A2F6  pop     ebx
41A2F7  mov     esp, ebp
41A2F9  pop     ebp
41A2FA  retn

;==== 41A310 sub_41A310
41A310  push    ebp
41A311  mov     ebp, esp
41A313  push    ecx
41A314  mov     ecx, dword_565B2C
41A31A  push    ebx
41A31B  mov     ebx, [ebp+Size]
41A31E  push    edi
41A31F  xor     edi, edi
41A321  call    sub_446A10
41A326  mov     [ebp+var_4], eax
41A329  mov     eax, [esi]
41A32B  cmp     eax, [esi+4]
41A32E  jnb     short loc_41A35C
41A330  mov     ecx, [ebp+arg_0]
41A333  test    ecx, ecx
41A335  jz      short loc_41A348
41A337  imul    eax, ebx
41A33A  add     eax, [esi+10h]
41A33D  push    ebx; Size
41A33E  push    eax; Src
41A33F  push    ecx; void *
41A340  call    _memcpy_0
41A345  add     esp, 0Ch
41A348  mov     ecx, [esi]
41A34A  mov     eax, [esi+0Ch]
41A34D  mov     edx, [esi+8]
41A350  imul    eax, ecx
41A353  lea     eax, [eax+eax*2]
41A356  inc     ecx
41A357  lea     edi, [edx+eax*8]
41A35A  mov     [esi], ecx
41A35C  mov     eax, [ebp+var_4]
41A35F  push    eax
41A360  mov     eax, dword_565B2C
41A365  call    sub_446A30
41A36A  mov     eax, edi
41A36C  pop     edi
41A36D  pop     ebx
41A36E  mov     esp, ebp
41A370  pop     ebp
41A371  retn

;==== 41A380 sub_41A380
41A380  push    ebp
41A381  mov     ebp, esp
41A383  push    esi
41A384  mov     esi, [ebp+arg_0]
41A387  push    edi
41A388  xor     edi, edi
41A38A  push    edi; Size
41A38B  push    edi; void *
41A38C  call    sub_41A310
41A391  add     esp, 8
41A394  test    eax, eax
41A396  jz      short loc_41A3B7
41A398  mov     ecx, [esi+14h]
41A39B  mov     edx, [ecx+4]
41A39E  mov     ecx, [ecx]
41A3A0  push    edi
41A3A1  push    edx
41A3A2  push    ecx
41A3A3  lea     edx, [eax+18h]
41A3A6  push    edx
41A3A7  push    eax
41A3A8  call    sub_40AA10
41A3AD  add     esp, 14h
41A3B0  lea     eax, [edi+1]
41A3B3  pop     edi
41A3B4  pop     esi
41A3B5  pop     ebp
41A3B6  retn
41A3B7  mov     eax, edi
41A3B9  pop     edi
41A3BA  pop     esi
41A3BB  pop     ebp
41A3BC  retn

;==== 41A3C0 sub_41A3C0
41A3C0  push    ebp
41A3C1  mov     ebp, esp
41A3C3  push    esi
41A3C4  mov     esi, [ebp+arg_0]
41A3C7  push    edi
41A3C8  xor     edi, edi
41A3CA  push    edi; Size
41A3CB  push    edi; void *
41A3CC  call    sub_41A310
41A3D1  add     esp, 8
41A3D4  test    eax, eax
41A3D6  jz      short loc_41A3F7
41A3D8  mov     ecx, [esi+14h]
41A3DB  mov     edx, [ecx]
41A3DD  push    edi
41A3DE  push    edx
41A3DF  lea     ecx, [eax+30h]
41A3E2  push    ecx
41A3E3  lea     edx, [eax+18h]
41A3E6  push    edx
41A3E7  push    eax
41A3E8  call    sub_40C4F0
41A3ED  add     esp, 14h
41A3F0  lea     eax, [edi+1]
41A3F3  pop     edi
41A3F4  pop     esi
41A3F5  pop     ebp
41A3F6  retn
41A3F7  mov     eax, edi
41A3F9  pop     edi
41A3FA  pop     esi
41A3FB  pop     ebp
41A3FC  retn

;==== 41B220 sub_41B220
41B220  xor     eax, eax
41B222  retn

;==== 424870 sub_424870
424870  mov     ecx, [eax+150h]
424876  push    ecx
424877  call    sub_44E710
42487C  retn

;==== 424C10 sub_424C10
424C10  push    ebp
424C11  mov     ebp, esp
424C13  and     esp, 0FFFFFFF8h
424C16  mov     eax, [edi+158h]
424C1C  sub     esp, 28h
424C1F  push    ebx
424C20  push    esi
424C21  test    eax, eax
424C23  jz      loc_424D81
424C29  cmp     dword ptr [eax], 0
424C2C  jz      loc_424D81
424C32  xor     esi, esi
424C34  cmp     [edi+154h], esi
424C3A  jbe     short loc_424C5B
424C3C  xor     ebx, ebx
424C3E  mov     edi, edi
424C40  mov     eax, [edi+158h]
424C46  add     eax, ebx
424C48  xor     ecx, ecx
424C4A  call    sub_40A610
424C4F  inc     esi
424C50  add     ebx, 18h
424C53  cmp     esi, [edi+154h]
424C59  jb      short loc_424C40
424C5B  mov     edx, [edi+16Ch]
424C61  mov     ecx, [edi+174h]
424C67  mov     esi, [edi+150h]
424C6D  push    edx
424C6E  mov     edx, [edi+168h]
424C74  push    edx
424C75  mov     edx, [edi+15Ch]
424C7B  push    edx
424C7C  mov     edx, [edi+158h]
424C82  mov     eax, ecx
424C84  push    edx
424C85  mov     edx, [edi+154h]
424C8B  shl     eax, 13h
424C8E  lea     eax, [eax+edi+180h]
424C95  lea     ecx, [edi+ecx*4+178h]
424C9C  push    edx
424C9D  call    sub_44E680
424CA2  mov     eax, [edi+17Ch]
424CA8  add     eax, [edi+178h]
424CAE  mov     edx, [edi]
424CB0  lea     esi, [edi+178h]
424CB6  mov     ecx, edi
424CB8  cmp     eax, [edi+170h]
424CBE  jge     loc_424D75
424CC4  mov     eax, [edx+1Ch]
424CC7  call    eax
424CC9  mov     edx, [edi]
424CCB  mov     edx, [edx+34h]
424CCE  mov     [esp+30h+var_1C], eax
424CD2  lea     eax, [esp+30h+var_18]
424CD6  push    eax
424CD7  mov     ecx, edi
424CD9  call    edx
424CDB  lea     eax, [edi+180h]
424CE1  mov     [esp+30h+var_24], eax
424CE5  mov     [esp+30h+var_28], esi
424CE9  mov     [esp+30h+var_20], 2
424CF1  xor     ebx, ebx
424CF3  cmp     [esi], ebx
424CF5  jle     short loc_424D4F
424CF7  mov     esi, [esp+30h+var_24]
424CFB  jmp     short loc_424D00
424D00  mov     ecx, [esi]
424D02  mov     edx, [esi+4]
424D05  mov     eax, [esi+8]
424D08  mov     [esp+30h+var_10], ecx
424D0C  mov     ecx, [esi+0Ch]
424D0F  mov     [esp+30h+var_C], edx
424D13  lea     edx, [esp+30h+var_10]
424D17  mov     [esp+30h+var_4], ecx
424D1B  mov     ecx, [esp+30h+var_14]
424D1F  push    edx
424D20  mov     edx, [esp+34h+var_18]
424D24  mov     [esp+34h+var_8], eax
424D28  call    sub_409160
424D2D  mov     eax, dword_565B3C
424D32  mov     ecx, [esp+34h+var_1C]
424D36  add     esp, 4
424D39  push    eax
424D3A  lea     eax, [esp+34h+var_10]
424D3E  call    sub_443610
424D43  mov     ecx, [esp+30h+var_28]
424D47  inc     ebx
424D48  add     esi, 10h
424D4B  cmp     ebx, [ecx]
424D4D  jl      short loc_424D00
424D4F  mov     esi, [esp+30h+var_28]
424D53  add     [esp+30h+var_24], 80000h
424D5B  add     esi, 4
424D5E  dec     [esp+30h+var_20]
424D62  mov     [esp+30h+var_28], esi
424D66  jnz     short loc_424CF1
424D68  xor     dword ptr [edi+174h], 1
424D6F  pop     esi
424D70  pop     ebx
424D71  mov     esp, ebp
424D73  pop     ebp
424D74  retn
424D75  mov     eax, [edx+0Ch]
424D78  call    eax
424D7A  xor     dword ptr [edi+174h], 1
424D81  pop     esi
424D82  pop     ebx
424D83  mov     esp, ebp
424D85  pop     ebp
424D86  retn

;==== 430BF0 sub_430BF0
430BF0  push    ebp
430BF1  mov     ebp, esp
430BF3  and     esp, 0FFFFFFF8h
430BF6  sub     esp, 4Ch
430BF9  mov     eax, dword_4FB734
430BFE  xor     eax, esp
430C00  mov     [esp+4Ch+var_4], eax
430C04  mov     eax, ecx
430C06  mov     ecx, [eax+40h]
430C09  push    ebx
430C0A  push    esi
430C0B  mov     esi, [ebp+arg_4]
430C0E  push    edi
430C0F  mov     [esp+58h+var_4C], eax
430C13  cmp     ecx, [eax+44h]
430C16  jge     loc_431070
430C1C  mov     ecx, [eax+54h]
430C1F  add     ecx, 18h
430C22  mov     eax, esi
430C24  call    sub_409100
430C29  test    eax, eax
430C2B  jz      loc_431075
430C31  mov     eax, [esi+0Ch]
430C34  mov     ebx, [esi]
430C36  mov     edi, [esi+4]
430C39  mov     edx, [esi+8]
430C3C  mov     [esp+58h+var_28], eax
430C40  mov     eax, [esp+58h+var_4C]
430C44  mov     esi, [eax+3Ch]
430C47  lea     ecx, [eax+28h]
430C4A  mov     [esp+58h+var_34], ebx
430C4E  mov     [esp+58h+var_30], edi
430C52  mov     [esp+58h+var_2C], edx
430C56  mov     [esp+58h+var_48], ecx
430C5A  test    esi, esi
430C5C  jz      loc_430EA6
430C62  mov     eax, esi
430C64  lea     ecx, [esp+58h+var_34]
430C68  call    sub_4090D0
430C6D  test    eax, eax
430C6F  jz      loc_430E13
430C75  mov     eax, esi
430C77  call    sub_4090A0
430C7C  test    eax, eax
430C7E  jnz     loc_431075
430C84  mov     eax, [esi]
430C86  cmp     ebx, eax
430C88  jle     short loc_430C8C
430C8A  mov     ebx, eax
430C8C  mov     ecx, [esp+58h+var_2C]
430C90  mov     [esp+58h+var_24], ebx
430C94  mov     eax, [esi+8]
430C97  cmp     ecx, eax
430C99  jge     short loc_430C9D
430C9B  mov     ecx, eax
430C9D  mov     [esp+58h+var_1C], ecx
430CA1  mov     edx, [esi+4]
430CA4  cmp     edi, edx
430CA6  jg      short loc_430CAA
430CA8  mov     edx, edi
430CAA  mov     [esp+58h+var_20], edx
430CAE  mov     eax, [esi+0Ch]
430CB1  cmp     [esp+58h+var_28], eax
430CB5  jl      short loc_430CBB
430CB7  mov     eax, [esp+58h+var_28]
430CBB  sub     ecx, ebx
430CBD  mov     [esp+58h+var_18], eax
430CC1  mov     ebx, [esi]
430CC3  sub     eax, edx
430CC5  mov     edx, [esi+4]
430CC8  inc     ecx
430CC9  inc     eax
430CCA  imul    eax, ecx
430CCD  mov     ecx, [esi+8]
430CD0  mov     [esp+58h+var_38], eax
430CD4  mov     eax, [esi+0Ch]
430CD7  mov     [esp+58h+var_3C], ecx
430CDB  sub     ecx, ebx
430CDD  inc     ecx
430CDE  sub     eax, edx
430CE0  mov     [esp+58h+var_40], ebx
430CE4  mov     ebx, [esp+58h+var_2C]
430CE8  sub     ebx, [esp+58h+var_34]
430CEC  inc     eax
430CED  imul    eax, ecx
430CF0  mov     ecx, [esp+58h+var_28]
430CF4  sub     ecx, edi
430CF6  inc     ecx
430CF7  inc     ebx
430CF8  imul    ecx, ebx
430CFB  cmp     eax, ecx
430CFD  ja      short loc_430D03
430CFF  shr     eax, 1
430D01  jmp     short loc_430D05
430D03  shr     ecx, 1
430D05  add     eax, ecx
430D07  cmp     [esp+58h+var_38], eax
430D0B  jbe     loc_431008
430D11  mov     eax, [esi+0Ch]
430D14  mov     ecx, [esp+58h+var_28]
430D18  cmp     edi, edx
430D1A  jnz     short loc_430D26
430D1C  cmp     eax, ecx
430D1E  jz      loc_431008
430D24  cmp     edi, edx
430D26  jl      loc_430DAE
430D2C  cmp     ecx, eax
430D2E  jg      short loc_430DAA
430D30  mov     eax, [esp+58h+var_40]
430D34  cmp     [esp+58h+var_34], eax
430D38  jle     short loc_430D3E
430D3A  mov     [esp+58h+var_34], eax
430D3E  mov     eax, [esi+8]
430D41  cmp     [esp+58h+var_2C], eax
430D45  jge     short loc_430D4B
430D47  mov     [esp+58h+var_2C], eax
430D4B  mov     eax, [esi+4]
430D4E  cmp     eax, edi
430D50  jge     short loc_430D8A
430D52  cmp     ecx, [esi+0Ch]
430D55  jge     short loc_430D88
430D57  mov     edx, [esi]
430D59  mov     [esp+58h+var_24], edx
430D5D  mov     eax, [esi+8]
430D60  inc     ecx
430D61  mov     [esp+58h+var_20], ecx
430D65  mov     [esp+58h+var_1C], eax
430D69  mov     ecx, [esi+0Ch]
430D6C  mov     [esp+58h+var_18], ecx
430D70  mov     eax, [esi+10h]
430D73  mov     ecx, [esp+58h+var_4C]
430D77  lea     edx, [esp+58h+var_24]
430D7B  push    edx
430D7C  dec     edi
430D7D  push    eax
430D7E  mov     [esi+0Ch], edi
430D81  call    sub_430BF0
430D86  jmp     short loc_430D96
430D88  cmp     eax, edi
430D8A  jnz     short loc_430D92
430D8C  inc     ecx
430D8D  mov     [esi+4], ecx
430D90  jmp     short loc_430D96
430D92  dec     edi
430D93  mov     [esi+0Ch], edi
430D96  mov     eax, [esi+10h]
430D99  cmp     [ebp+arg_0], eax
430D9C  jbe     loc_430E8F
430DA2  mov     [ebp+arg_0], eax
430DA5  jmp     loc_430E8F
430DAA  cmp     edi, edx
430DAC  jg      short loc_430DB6
430DAE  cmp     eax, ecx
430DB0  jle     loc_430EF5
430DB6  mov     ebx, [esp+58h+var_34]
430DBA  cmp     ebx, [esp+58h+var_40]
430DBE  jl      short loc_430DFB
430DC0  mov     eax, [esp+58h+var_2C]
430DC4  cmp     eax, [esp+58h+var_3C]
430DC8  jg      short loc_430DF1
430DCA  cmp     edi, edx
430DCC  jge     short loc_430DD5
430DCE  dec     edx
430DCF  mov     [esp+58h+var_28], edx
430DD3  jmp     short loc_430DDD
430DD5  mov     edi, [esi+0Ch]
430DD8  inc     edi
430DD9  mov     [esp+58h+var_30], edi
430DDD  mov     eax, [esi+10h]
430DE0  cmp     [ebp+arg_0], eax
430DE3  jbe     loc_430E97
430DE9  mov     [ebp+arg_0], eax
430DEC  jmp     loc_430E97
430DF1  cmp     ebx, [esp+58h+var_40]
430DF5  jg      loc_430F9D
430DFB  mov     eax, [esp+58h+var_3C]
430DFF  cmp     eax, [esp+58h+var_2C]
430E03  jg      loc_430F9D
430E09  cmp     edi, edx
430E0B  jge     short loc_430D92
430E0D  inc     ecx
430E0E  mov     [esi+4], ecx
430E11  jmp     short loc_430D96
430E13  mov     eax, [esi]
430E15  mov     edx, [esp+58h+var_2C]
430E19  mov     ecx, [esp+58h+var_28]
430E1D  mov     [esp+58h+var_14], ebx
430E21  mov     [esp+58h+var_10], edi
430E25  mov     [esp+58h+var_C], edx
430E29  mov     [esp+58h+var_8], ecx
430E2D  cmp     ebx, eax
430E2F  jl      short loc_430E35
430E31  mov     [esp+58h+var_14], eax
430E35  mov     ebx, [esi+4]
430E38  cmp     edi, ebx
430E3A  jl      short loc_430E40
430E3C  mov     [esp+58h+var_10], ebx
430E40  mov     edi, [esi+8]
430E43  cmp     edx, edi
430E45  jge     short loc_430E4D
430E47  mov     edx, edi
430E49  mov     [esp+58h+var_C], edx
430E4D  mov     eax, [esi+0Ch]
430E50  cmp     ecx, eax
430E52  jge     short loc_430E5A
430E54  mov     ecx, eax
430E56  mov     [esp+58h+var_8], ecx
430E5A  sub     edi, [esi]
430E5C  sub     ecx, [esp+58h+var_10]
430E60  sub     edx, [esp+58h+var_14]
430E64  sub     eax, ebx
430E66  mov     ebx, [esp+58h+var_2C]
430E6A  sub     ebx, [esp+58h+var_34]
430E6E  inc     edi
430E6F  inc     eax
430E70  imul    eax, edi
430E73  mov     edi, [esp+58h+var_28]
430E77  sub     edi, [esp+58h+var_30]
430E7B  inc     ebx
430E7C  inc     edi
430E7D  imul    edi, ebx
430E80  inc     ecx
430E81  inc     edx
430E82  imul    ecx, edx
430E85  add     eax, edi
430E87  cmp     eax, ecx
430E89  jz      loc_43104D
430E8F  mov     edi, [esp+58h+var_30]
430E93  mov     ebx, [esp+58h+var_34]
430E97  mov     [esp+58h+var_48], esi
430E9B  mov     esi, [esi+14h]
430E9E  test    esi, esi
430EA0  jnz     loc_430C62
430EA6  push    18h; Size
430EA8  call    ??2@YAPAXI@Z; operator new(uint)
430EAD  mov     ecx, [esp+5Ch+var_34]
430EB1  mov     [eax], ecx
430EB3  mov     edx, [esp+5Ch+var_30]
430EB7  mov     [eax+4], edx
430EBA  mov     ecx, [esp+5Ch+var_2C]
430EBE  mov     [eax+8], ecx
430EC1  mov     edx, [esp+5Ch+var_28]
430EC5  mov     ecx, [ebp+arg_0]
430EC8  mov     [eax+10h], ecx
430ECB  mov     ecx, [esp+5Ch+var_4C]
430ECF  mov     [eax+0Ch], edx
430ED2  mov     edx, [ecx+3Ch]
430ED5  mov     [eax+14h], edx
430ED8  add     esp, 4
430EDB  inc     dword ptr [ecx+40h]
430EDE  mov     [ecx+3Ch], eax
430EE1  pop     edi
430EE2  pop     esi
430EE3  pop     ebx
430EE4  mov     ecx, [esp+4Ch+var_4]
430EE8  xor     ecx, esp
430EEA  call    sub_4ABDD5
430EEF  mov     esp, ebp
430EF1  pop     ebp
430EF2  retn    8
430EF5  mov     ecx, [esi+14h]
430EF8  mov     edx, [esp+58h+var_48]
430EFC  mov     edi, [esp+58h+var_4C]
430F00  mov     ebx, [ebp+arg_0]
430F03  mov     [edx+14h], ecx
430F06  dec     dword ptr [edi+40h]
430F09  mov     eax, [esi+4]
430F0C  mov     [esp+58h+var_20], eax
430F10  mov     ecx, [esi+0Ch]
430F13  mov     [esp+58h+var_18], ecx
430F17  mov     eax, [esi+10h]
430F1A  cmp     ebx, eax
430F1C  ja      short loc_430F20
430F1E  mov     eax, ebx
430F20  lea     edx, [esp+58h+var_24]
430F24  push    edx
430F25  push    eax
430F26  mov     ecx, edi
430F28  call    sub_430BF0
430F2D  mov     eax, [esp+58h+var_34]
430F31  mov     ecx, [esp+58h+var_2C]
430F35  mov     [esp+58h+var_24], eax
430F39  mov     eax, [esp+58h+var_30]
430F3D  mov     [esp+58h+var_1C], ecx
430F41  cmp     eax, [esi+4]
430F44  jge     short loc_430F5F
430F46  mov     [esp+58h+var_20], eax
430F4A  mov     edx, [esi+4]
430F4D  lea     eax, [esp+58h+var_24]
430F51  push    eax
430F52  dec     edx
430F53  push    ebx
430F54  mov     ecx, edi
430F56  mov     [esp+60h+var_18], edx
430F5A  call    sub_430BF0
430F5F  mov     eax, [esi+0Ch]
430F62  mov     ecx, [esp+58h+var_28]
430F66  cmp     eax, ecx
430F68  jge     short loc_430F80
430F6A  mov     [esp+58h+var_18], ecx
430F6E  lea     ecx, [esp+58h+var_24]
430F72  push    ecx
430F73  inc     eax
430F74  push    ebx
430F75  mov     ecx, edi
430F77  mov     [esp+60h+var_20], eax
430F7B  call    sub_430BF0
430F80  push    esi; void *
430F81  call    ??3@YAXPAX@Z; operator delete(void *)
430F86  add     esp, 4
430F89  pop     edi
430F8A  pop     esi
430F8B  pop     ebx
430F8C  mov     ecx, [esp+4Ch+var_4]
430F90  xor     ecx, esp
430F92  call    sub_4ABDD5
430F97  mov     esp, ebp
430F99  pop     ebp
430F9A  retn    8
430F9D  mov     eax, edx
430F9F  cmp     edi, eax
430FA1  jge     short loc_430FB6
430FA3  mov     [esp+58h+var_20], eax
430FA7  mov     [esp+58h+var_18], ecx
430FAB  dec     eax
430FAC  inc     ecx
430FAD  mov     [esp+58h+var_28], eax
430FB1  mov     [esi+4], ecx
430FB4  jmp     short loc_430FCA
430FB6  mov     [esp+58h+var_20], edi
430FBA  mov     eax, [esi+0Ch]
430FBD  mov     [esp+58h+var_18], eax
430FC1  inc     eax
430FC2  dec     edi
430FC3  mov     [esp+58h+var_30], eax
430FC7  mov     [esi+0Ch], edi
430FCA  mov     edi, [ebp+arg_0]
430FCD  mov     ebx, [esp+58h+var_4C]
430FD1  lea     edx, [esp+58h+var_34]
430FD5  push    edx
430FD6  push    edi
430FD7  mov     ecx, ebx
430FD9  call    sub_430BF0
430FDE  mov     esi, [esi+10h]
430FE1  cmp     edi, esi
430FE3  ja      short loc_430FE7
430FE5  mov     esi, edi
430FE7  lea     eax, [esp+58h+var_24]
430FEB  push    eax
430FEC  push    esi
430FED  mov     ecx, ebx
430FEF  call    sub_430BF0
430FF4  pop     edi
430FF5  pop     esi
430FF6  pop     ebx
430FF7  mov     ecx, [esp+4Ch+var_4]
430FFB  xor     ecx, esp
430FFD  call    sub_4ABDD5
431002  mov     esp, ebp
431004  pop     ebp
431005  retn    8
431008  mov     ecx, [esi+14h]
43100B  mov     edx, [esp+58h+var_48]
43100F  mov     [edx+14h], ecx
431012  mov     ecx, [esp+58h+var_4C]
431016  dec     dword ptr [ecx+40h]
431019  mov     eax, [esi+10h]
43101C  mov     edx, [ebp+arg_0]
43101F  cmp     edx, eax
431021  ja      short loc_431025
431023  mov     eax, edx
431025  lea     edx, [esp+58h+var_24]
431029  push    edx
43102A  push    eax
43102B  call    sub_430BF0
431030  push    esi; void *
431031  call    ??3@YAXPAX@Z; operator delete(void *)
431036  add     esp, 4
431039  pop     edi
43103A  pop     esi
43103B  pop     ebx
43103C  mov     ecx, [esp+4Ch+var_4]
431040  xor     ecx, esp
431042  call    sub_4ABDD5
431047  mov     esp, ebp
431049  pop     ebp
43104A  retn    8
43104D  mov     eax, [esi+14h]
431050  mov     ecx, [esp+58h+var_48]
431054  mov     edx, [ebp+arg_0]
431057  mov     [ecx+14h], eax
43105A  mov     ecx, [esp+58h+var_4C]
43105E  dec     dword ptr [ecx+40h]
431061  mov     eax, [esi+10h]
431064  cmp     edx, eax
431066  ja      short loc_43106A
431068  mov     eax, edx
43106A  lea     edx, [esp+58h+var_14]
43106E  jmp     short loc_431029
431070  call    sub_4310D0
431075  mov     ecx, [esp+58h+var_4]
431079  pop     edi
43107A  pop     esi
43107B  pop     ebx
43107C  xor     ecx, esp
43107E  call    sub_4ABDD5
431083  mov     esp, ebp
431085  pop     ebp
431086  retn    8

;==== 4310D0 sub_4310D0
4310D0  mov     dword ptr [eax+4Ch], 1
4310D7  retn

;==== 4310F0 sub_4310F0
4310F0  mov     eax, [eax+48h]
4310F3  retn

;==== 4339A0 sub_4339A0
4339A0  mov     [eax+38h], ecx
4339A3  retn

;==== 441470 sub_441470
441470  push    esi
441471  mov     esi, dword_566760
441477  push    edi
441478  add     esi, 93Ch
44147E  mov     edi, 8
441483  mov     eax, [esi]
441485  test    eax, eax
441487  jz      short loc_44148E
441489  call    sub_424870
44148E  add     esi, 4
441491  dec     edi
441492  jnz     short loc_441483
441494  pop     edi
441495  pop     esi
441496  retn

;==== 441600 sub_441600
441600  push    ebp
441601  mov     ebp, esp
441603  mov     edx, eax
441605  and     edx, 0FF000000h
44160B  xor     ecx, ecx
44160D  cmp     edx, 0C0000000h
441613  jnz     short loc_44162D
441615  call    sub_443630
44161A  cmp     eax, 8
44161D  jnb     short loc_44162D
44161F  mov     ecx, [ebp+arg_0]
441622  mov     eax, [ecx+eax*4+93Ch]
441629  pop     ebp
44162A  retn    4
44162D  mov     eax, ecx
44162F  pop     ebp
441630  retn    4

;==== 441840 sub_441840
441840  push    ebp
441841  mov     ebp, esp
441843  mov     eax, dword_566760
441848  push    edi
441849  push    eax
44184A  mov     eax, [ebp+arg_0]
44184D  call    sub_441600
441852  mov     edi, eax
441854  test    edi, edi
441856  jz      short loc_44185D
441858  call    sub_424C10
44185D  xor     eax, eax
44185F  test    edi, edi
441861  setnz   al
441864  pop     edi
441865  pop     ebp
441866  retn    4

;==== 443530 sub_443530
443530  mov     eax, [eax+14h]
443533  jmp     sub_4310F0

;==== 443610 sub_443610
443610  push    ebp
443611  mov     ebp, esp
443613  mov     edx, [ebp+arg_0]
443616  push    eax
443617  push    ecx
443618  mov     ecx, [edx+14h]
44361B  call    sub_430BF0
443620  pop     ebp
443621  retn    4

;==== 443630 sub_443630
443630  and     eax, 0FFFFFFh
443635  retn

;==== 4468E0 sub_4468E0
4468E0  mov     eax, [eax+0Ch]
4468E3  retn

;==== 4468F0 sub_4468F0
4468F0  push    ebp
4468F1  mov     ebp, esp
4468F3  mov     edx, [ebp+arg_0]
4468F6  mov     [eax+10h], ecx
4468F9  mov     [eax+18h], edx
4468FC  pop     ebp
4468FD  retn    4

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

;==== 44CCC0 sub_44CCC0
44CCC0  xor     eax, eax
44CCC2  retn    4

;==== 44DC40 sub_44DC40
44DC40  push    ebp
44DC41  mov     ebp, esp
44DC43  mov     [esi+4], eax
44DC46  xor     eax, eax
44DC48  push    ebx
44DC49  mov     ebx, [ebp+arg_0]
44DC4C  push    edi
44DC4D  mov     dword ptr [esi], offset ??_7DCParticle@@6B@; const DCParticle::`vftable'
44DC53  mov     [esi+8], ecx
44DC56  mov     [esi+20h], eax
44DC59  mov     [esi+24h], eax
44DC5C  mov     edi, [ebx+88h]
44DC62  call    sub_44DEF0
44DC67  add     eax, [ebx+84h]
44DC6D  pop     edi
44DC6E  mov     [esi+28h], eax
44DC71  mov     [esi+2Ch], eax
44DC74  mov     eax, [ebx]
44DC76  mov     [esi+30h], eax
44DC79  mov     [esi+34h], ebx
44DC7C  mov     dword ptr [esi+0Ch], 0
44DC83  mov     eax, esi
44DC85  pop     ebx
44DC86  pop     ebp
44DC87  retn    4

;==== 44DC90 sub_44DC90
44DC90  push    ebp
44DC91  mov     ebp, esp
44DC93  push    esi
44DC94  mov     esi, ecx
44DC96  call    sub_44DCC0
44DC9B  test    [ebp+arg_0], 1
44DC9F  jz      short loc_44DCAA
44DCA1  push    esi; void *
44DCA2  call    ??3@YAXPAX@Z; operator delete(void *)
44DCA7  add     esp, 4
44DCAA  mov     eax, esi
44DCAC  pop     esi
44DCAD  pop     ebp
44DCAE  retn    4

;==== 44DCC0 sub_44DCC0
44DCC0  mov     dword ptr [ecx], offset ??_7DCParticle@@6B@; const DCParticle::`vftable'
44DCC6  retn

;==== 44DEF0 sub_44DEF0
44DEF0  push    esi
44DEF1  call    _rand
44DEF6  mov     esi, eax
44DEF8  shl     esi, 0Fh
44DEFB  call    _rand
44DF00  mov     ecx, eax
44DF02  or      ecx, esi
44DF04  lea     eax, [edi+1]
44DF07  cdq
44DF08  mov     esi, eax
44DF0A  xor     esi, edx
44DF0C  sub     esi, edx
44DF0E  mov     eax, ecx
44DF10  cdq
44DF11  idiv    esi
44DF13  pop     esi
44DF14  mov     eax, edx
44DF16  test    edi, edi
44DF18  jns     short locret_44DF1C
44DF1A  neg     eax
44DF1C  retn

;==== 44DF20 sub_44DF20
44DF20  mov     eax, [eax+8]
44DF23  retn

;==== 44DF30 sub_44DF30
44DF30  mov     edx, [ecx+10h]
44DF33  mov     [eax], edx
44DF35  mov     edx, [ecx+14h]
44DF38  mov     ecx, [ecx+18h]
44DF3B  mov     [eax+4], edx
44DF3E  mov     [eax+8], ecx
44DF41  retn

;==== 44DF50 sub_44DF50
44DF50  mov     eax, [eax+38h]
44DF53  retn

;==== 44DF60 sub_44DF60
44DF60  mov     eax, [ecx+3Ch]
44DF63  retn

;==== 44DF70 sub_44DF70
44DF70  mov     eax, [ecx+0Ch]
44DF73  test    eax, eax
44DF75  jz      short locret_44DF9F
44DF77  push    esi
44DF78  mov     esi, [edx]
44DF7A  imul    esi, [ecx+1Ch]
44DF7E  sar     esi, 8
44DF81  add     [ecx+10h], esi
44DF84  mov     esi, [edx+4]
44DF87  imul    esi, [ecx+1Ch]
44DF8B  sar     esi, 8
44DF8E  add     [ecx+14h], esi
44DF91  mov     edx, [edx+8]
44DF94  imul    edx, [ecx+1Ch]
44DF98  sar     edx, 8
44DF9B  add     [ecx+18h], edx
44DF9E  pop     esi
44DF9F  retn

;==== 44DFA0 sub_44DFA0
44DFA0  mov     eax, [ecx+28h]
44DFA3  add     [ecx+20h], eax
44DFA6  mov     eax, [ecx+2Ch]
44DFA9  add     [ecx+24h], eax
44DFAC  mov     edx, [ecx+20h]
44DFAF  mov     eax, [ecx+24h]
44DFB2  push    esi
44DFB3  mov     esi, [ecx+34h]
44DFB6  push    edi
44DFB7  mov     edi, [esi]
44DFB9  shl     edi, 10h
44DFBC  cmp     edx, edi
44DFBE  jb      short loc_44DFD3
44DFC0  mov     edi, [esi]
44DFC2  shl     edi, 10h
44DFC5  sub     edx, edi
44DFC7  mov     [ecx+20h], edx
44DFCA  mov     edi, [esi]
44DFCC  shl     edi, 10h
44DFCF  cmp     edx, edi
44DFD1  jnb     short loc_44DFC0
44DFD3  mov     edx, [ecx+30h]
44DFD6  shl     edx, 10h
44DFD9  cmp     eax, edx
44DFDB  jb      short loc_44DFEF
44DFDD  lea     ecx, [ecx+0]
44DFE0  sub     eax, edx
44DFE2  mov     [ecx+24h], eax
44DFE5  mov     esi, [ecx+30h]
44DFE8  shl     esi, 10h
44DFEB  cmp     eax, esi
44DFED  jnb     short loc_44DFE0
44DFEF  mov     eax, [ecx+0Ch]
44DFF2  pop     edi
44DFF3  pop     esi
44DFF4  retn

;==== 44E000 sub_44E000
44E000  mov     dword ptr [eax+3Ch], 0
44E007  retn

;==== 44E010 sub_44E010
44E010  movzx   eax, word ptr [eax+22h]
44E014  retn

;==== 44E020 sub_44E020
44E020  movzx   eax, word ptr [eax+26h]
44E024  retn

;==== 44E030 sub_44E030
44E030  push    ebp
44E031  mov     ebp, esp
44E033  push    ebx
44E034  mov     ebx, [ebp+arg_0]
44E037  push    edi
44E038  mov     edi, [esi+88h]
44E03E  call    sub_44DEF0
44E043  add     eax, [esi+84h]
44E049  pop     edi
44E04A  mov     [ebx+2Ch], eax
44E04D  mov     eax, [esi]
44E04F  mov     [ebx+30h], eax
44E052  pop     ebx
44E053  pop     ebp
44E054  retn    4

;==== 44E680 sub_44E680
44E680  push    ebp
44E681  mov     ebp, esp
44E683  mov     edx, [ebp+arg_10]
44E686  push    eax
44E687  mov     eax, [ebp+arg_C]
44E68A  push    ecx
44E68B  mov     ecx, [ebp+arg_8]
44E68E  push    edx
44E68F  mov     edx, [ebp+arg_4]
44E692  push    eax
44E693  mov     eax, [ebp+arg_0]
44E696  push    ecx
44E697  push    edx
44E698  push    eax
44E699  call    sub_44EE30
44E69E  pop     ebp
44E69F  retn    14h

;==== 44E710 sub_44E710
44E710  push    ebp
44E711  mov     ebp, esp
44E713  call    sub_499290
44E718  push    eax
44E719  mov     eax, [ebp+arg_0]
44E71C  push    eax
44E71D  call    sub_44E840
44E722  pop     ebp
44E723  retn    4

;==== 44E840 sub_44E840
44E840  push    ebp
44E841  mov     ebp, esp
44E843  push    ecx
44E844  mov     eax, [ebp+arg_4]
44E847  push    esi
44E848  mov     esi, [ebp+arg_0]
44E84B  push    eax
44E84C  lea     ecx, [ebp+var_4]
44E84F  push    ecx
44E850  call    sub_44E890
44E855  test    eax, eax
44E857  jz      short loc_44E884
44E859  push    ebx
44E85A  xor     ebx, ebx
44E85C  xor     esi, esi
44E85E  cmp     [ebp+var_4], ebx
44E861  push    edi
44E862  setz    bl
44E865  xor     edi, edi
44E867  mov     edx, [ebp+arg_4]
44E86A  mov     ecx, [ebp+arg_0]
44E86D  push    ebx; void *
44E86E  push    edx; int
44E86F  push    edi; int
44E870  push    esi; int
44E871  call    sub_44EA70
44E876  inc     edi
44E877  cmp     edi, 40h ; '@'
44E87A  jb      short loc_44E867
44E87C  inc     esi
44E87D  cmp     esi, 2
44E880  jb      short loc_44E865
44E882  pop     edi
44E883  pop     ebx
44E884  pop     esi
44E885  mov     esp, ebp
44E887  pop     ebp
44E888  retn    8

;==== 44E890 sub_44E890
44E890  push    ebp
44E891  mov     ebp, esp
44E893  xor     eax, eax
44E895  cmp     eax, [esi+8]
44E898  push    ebx
44E899  mov     ebx, [ebp+arg_4]
44E89C  sbb     ecx, ecx
44E89E  neg     ecx
44E8A0  mov     [ebp+arg_4], ecx
44E8A3  jz      loc_44E936
44E8A9  mov     edx, [esi+0Ch]
44E8AC  add     edx, 1F4h
44E8B2  cmp     edx, ebx
44E8B4  mov     edx, [ebp+arg_0]
44E8B7  sbb     eax, eax
44E8B9  inc     eax
44E8BA  mov     [edx], eax
44E8BC  jnz     short loc_44E8C1
44E8BE  mov     [esi+0Ch], ebx
44E8C1  cmp     [esi+0Ch], ebx
44E8C4  ja      short loc_44E936
44E8C6  push    edi
44E8C7  jmp     short loc_44E8D0
44E8D0  mov     ecx, [esi+21654h]
44E8D6  mov     eax, [ecx]
44E8D8  mov     edx, [eax+8]
44E8DB  call    edx
44E8DD  mov     edx, [esi+21654h]
44E8E3  lea     eax, [esi+21670h]
44E8E9  call    sub_44F540
44E8EE  mov     eax, esi
44E8F0  call    sub_44F460
44E8F5  mov     edi, dword_565DA8
44E8FB  push    esi
44E8FC  mov     ecx, offset sub_44EA60
44E901  mov     eax, edi
44E903  call    sub_4468F0
44E908  mov     eax, 1
44E90D  mov     ecx, edi
44E90F  call    sub_446910
44E914  mov     eax, dword_565DA8
44E919  push    0
44E91B  xor     ecx, ecx
44E91D  call    sub_4468F0
44E922  mov     eax, [esi+8]
44E925  add     [esi+0Ch], eax
44E928  cmp     [esi+0Ch], ebx
44E92B  jbe     short loc_44E8D0
44E92D  mov     eax, [ebp+arg_4]
44E930  pop     edi
44E931  pop     ebx
44E932  pop     ebp
44E933  retn    8
44E936  mov     eax, ecx
44E938  pop     ebx
44E939  pop     ebp
44E93A  retn    8

;==== 44E940 sub_44E940
44E940  push    ebp
44E941  mov     ebp, esp
44E943  sub     esp, 18h
44E946  mov     ecx, dword_565DA8
44E94C  push    ebx
44E94D  push    esi
44E94E  push    edi
44E94F  call    sub_446A10
44E954  mov     ebx, [ebp+arg_0]
44E957  lea     esi, [ebp+var_4]
44E95A  mov     edx, ebx
44E95C  mov     edi, eax
44E95E  call    sub_44F470
44E963  mov     esi, eax
44E965  mov     eax, dword_565DA8
44E96A  push    edi
44E96B  mov     [ebp+var_8], esi
44E96E  call    sub_446A30
44E973  test    esi, esi
44E975  js      loc_44EA4E
44E97B  lea     ebx, [ebx+esi*4+14h]
44E97F  xor     esi, esi
44E981  cmp     [ebp+var_4], esi
44E984  jle     loc_44EA4B
44E98A  lea     ebx, [ebx+0]
44E990  mov     ecx, [ebx+esi*4]
44E993  test    ecx, ecx
44E995  jz      loc_44EA41
44E99B  mov     edx, [ebp+arg_0]
44E99E  add     edx, 21670h
44E9A4  call    sub_44DF70
44E9A9  mov     ecx, [ebx+esi*4]
44E9AC  mov     eax, [ecx]
44E9AE  mov     edx, [eax+10h]
44E9B1  call    edx
44E9B3  mov     edi, [ebx+esi*4]
44E9B6  mov     [ebp+var_C], eax
44E9B9  lea     eax, [ebp+var_18]
44E9BC  mov     ecx, edi
44E9BE  call    sub_44DF30
44E9C3  mov     eax, [ebp+var_10]
44E9C6  sar     [ebp+var_18], 8
44E9CA  sar     [ebp+var_14], 8
44E9CE  sar     eax, 8
44E9D1  cmp     [ebp+var_C], 0
44E9D5  mov     [ebp+var_10], eax
44E9D8  jz      short loc_44EA02
44E9DA  mov     ecx, [ebp+var_18]
44E9DD  add     ecx, 3E80h
44E9E3  cmp     ecx, 7D00h
44E9E9  ja      short loc_44EA02
44E9EB  cmp     [ebp+var_14], 3E80h
44E9F2  ja      short loc_44EA02
44E9F4  cmp     eax, 1F40h
44E9F9  jg      short loc_44EA02
44E9FB  cmp     eax, 0FFFFE0C0h
44EA00  jge     short loc_44EA41
44EA02  mov     eax, edi
44EA04  call    sub_454260
44EA09  mov     edx, eax
44EA0B  mov     eax, edi
44EA0D  shl     edx, 6
44EA10  call    sub_44DF20
44EA15  mov     edi, [ebp+arg_0]
44EA18  add     edx, eax
44EA1A  dec     dword ptr [edi+edx*4+20214h]
44EA21  mov     ecx, [ebx+esi*4]
44EA24  lea     eax, [edi+edx*4+20214h]
44EA2B  test    ecx, ecx
44EA2D  jz      short loc_44EA37
44EA2F  mov     eax, [ecx]
44EA31  mov     edx, [eax]
44EA33  push    1
44EA35  call    edx
44EA37  mov     dword ptr [ebx+esi*4], 0
44EA3E  dec     dword ptr [edi+10h]
44EA41  inc     esi
44EA42  cmp     esi, [ebp+var_4]
44EA45  jl      loc_44E990
44EA4B  mov     esi, [ebp+var_8]
44EA4E  xor     eax, eax
44EA50  pop     edi
44EA51  test    esi, esi
44EA53  pop     esi
44EA54  setns   al
44EA57  pop     ebx
44EA58  mov     esp, ebp
44EA5A  pop     ebp
44EA5B  retn    4

;==== 44EA60 sub_44EA60
44EA60  push    ebp
44EA61  mov     ebp, esp
44EA63  mov     eax, [ebp+arg_0]
44EA66  push    eax
44EA67  call    sub_44E940
44EA6C  pop     ebp
44EA6D  retn

;==== 44EA70 sub_44EA70
44EA70  push    ebp
44EA71  mov     ebp, esp
44EA73  push    0FFFFFFFFh
44EA75  push    offset SEH_44EA70
44EA7A  mov     eax, large fs:0
44EA80  push    eax
44EA81  push    ecx
44EA82  push    ebx
44EA83  push    esi
44EA84  push    edi
44EA85  mov     eax, dword_4FB734
44EA8A  xor     eax, ebp
44EA8C  push    eax
44EA8D  lea     eax, [ebp+var_C]
44EA90  mov     large fs:0, eax
44EA96  mov     esi, ecx
44EA98  mov     ecx, [ebp+arg_0]
44EA9B  xor     eax, eax
44EA9D  mov     [ebp+var_10], eax
44EAA0  cmp     ecx, 2
44EAA3  jnb     loc_44EBF9
44EAA9  mov     edx, [ebp+arg_4]
44EAAC  cmp     edx, 40h ; '@'
44EAAF  jnb     loc_44EBF9
44EAB5  cmp     [ebp+arg_C], eax
44EAB8  jz      short loc_44EACB
44EABA  mov     ebx, [ebp+arg_8]
44EABD  mov     edi, ecx
44EABF  shl     edi, 6
44EAC2  add     edi, edx
44EAC4  mov     [esi+edi*4+20614h], ebx
44EACB  mov     edi, ecx
44EACD  shl     edi, 6
44EAD0  add     edi, edx
44EAD2  mov     edx, [ebp+arg_8]
44EAD5  cmp     [esi+edi*4+20614h], edx
44EADC  ja      loc_44EBF9
44EAE2  jmp     short loc_44EAE7
44EAE4  mov     ecx, [ebp+arg_0]
44EAE7  mov     edx, [esi+edi*4+20214h]
44EAEE  cmp     edx, [esi+edi*4+20014h]
44EAF5  jge     loc_44EBEE
44EAFB  xor     ebx, ebx
44EAFD  mov     eax, ecx
44EAFF  sub     eax, ebx
44EB01  jz      short loc_44EB61
44EB03  dec     eax
44EB04  jnz     loc_44EB90
44EB0A  push    78h ; 'x'; Size
44EB0C  call    ??2@YAPAXI@Z; operator new(uint)
44EB11  add     esp, 4
44EB14  mov     [ebp+arg_C], eax
44EB17  mov     [ebp+var_4], 1
44EB1E  test    eax, eax
44EB20  jz      short loc_44EB40
44EB22  mov     ecx, [ebp+arg_4]
44EB25  lea     edx, ds:0[ecx*8]
44EB2C  sub     edx, ecx
44EB2E  lea     edx, [esi+edx*8+20854h]
44EB35  push    edx
44EB36  push    eax
44EB37  call    sub_44F8C0
44EB3C  mov     ebx, eax
44EB3E  jmp     short loc_44EB42
44EB40  xor     ebx, ebx
44EB42  mov     [ebp+var_4], 0FFFFFFFFh
44EB49  mov     ecx, [ebp+arg_4]
44EB4C  call    sub_44FCE0
44EB51  test    eax, eax
44EB53  jz      short loc_44EB90
44EB55  mov     eax, [ebp+arg_4]
44EB58  mov     ecx, ebx
44EB5A  call    sub_450220
44EB5F  jmp     short loc_44EB90
44EB61  push    4Ch ; 'L'; Size
44EB63  call    ??2@YAPAXI@Z; operator new(uint)
44EB68  add     esp, 4
44EB6B  mov     [ebp+arg_C], eax
44EB6E  mov     [ebp+var_4], 0
44EB75  test    eax, eax
44EB77  jz      short loc_44EB85
44EB79  mov     ecx, [ebp+arg_4]
44EB7C  push    ecx
44EB7D  push    eax
44EB7E  call    sub_4503E0
44EB83  jmp     short loc_44EB87
44EB85  xor     eax, eax
44EB87  mov     [ebp+var_4], 0FFFFFFFFh
44EB8E  mov     ebx, eax
44EB90  push    ebx
44EB91  call    sub_44EC10
44EB96  mov     eax, 1
44EB9B  add     [esi+edi*4+20214h], eax
44EBA2  cmp     dword ptr [esi+edi*4+20614h], 0
44EBAA  jnz     short loc_44EBB6
44EBAC  mov     edx, [ebp+arg_8]
44EBAF  mov     [esi+edi*4+20614h], edx
44EBB6  mov     ecx, [esi+edi*4+20414h]
44EBBD  add     [ebp+var_10], eax
44EBC0  mov     edx, [ebp+arg_8]
44EBC3  add     [esi+edi*4+20614h], ecx
44EBCA  mov     eax, [ebp+var_10]
44EBCD  cmp     [esi+edi*4+20614h], edx
44EBD4  jbe     loc_44EAE4
44EBDA  mov     ecx, [ebp+var_C]
44EBDD  mov     large fs:0, ecx
44EBE4  pop     ecx
44EBE5  pop     edi
44EBE6  pop     esi
44EBE7  pop     ebx
44EBE8  mov     esp, ebp
44EBEA  pop     ebp
44EBEB  retn    10h
44EBEE  mov     dword ptr [esi+edi*4+20614h], 0
44EBF9  mov     ecx, [ebp+var_C]
44EBFC  mov     large fs:0, ecx
44EC03  pop     ecx
44EC04  pop     edi
44EC05  pop     esi
44EC06  pop     ebx
44EC07  mov     esp, ebp
44EC09  pop     ebp
44EC0A  retn    10h
4D98F0  mov     eax, [ebp+arg_C]
4D98F3  push    eax; void *
4D98F4  call    ??3@YAXPAX@Z; operator delete(void *)
4D98F9  pop     ecx
4D98FA  retn
4D98FB  mov     eax, [ebp+arg_C]
4D98FE  push    eax; void *
4D98FF  call    ??3@YAXPAX@Z; operator delete(void *)
4D9904  pop     ecx
4D9905  retn
4D9906  mov     edx, [esp-4+arg_4]
4D990A  lea     eax, [edx+0Ch]
4D990D  mov     ecx, [edx-14h]
4D9910  xor     ecx, eax
4D9912  call    sub_4ABDD5
4D9917  mov     eax, offset stru_4F71F8
4D991C  jmp     ___CxxFrameHandler3

;==== 44EC10 sub_44EC10
44EC10  push    ebp
44EC11  mov     ebp, esp
44EC13  mov     ecx, [esi+10h]
44EC16  inc     ecx
44EC17  xor     eax, eax
44EC19  cmp     ecx, 8000h
44EC1F  jg      short loc_44EC49
44EC21  xor     ecx, ecx
44EC23  lea     edx, [esi+14h]
44EC26  cmp     [edx], eax
44EC28  jz      short loc_44EC3A
44EC2A  inc     ecx
44EC2B  add     edx, 4
44EC2E  cmp     ecx, 8000h
44EC34  jl      short loc_44EC26
44EC36  pop     ebp
44EC37  retn    4
44EC3A  mov     edx, [ebp+arg_0]
44EC3D  mov     [esi+ecx*4+14h], edx
44EC41  inc     dword ptr [esi+10h]
44EC44  mov     eax, 1
44EC49  pop     ebp
44EC4A  retn    4

;==== 44ED30 sub_44ED30
44ED30  push    ebp
44ED31  mov     ebp, esp
44ED33  sub     esp, 14h
44ED36  cmp     [ebp+arg_4], 2
44ED3A  mov     [ebp+var_4], ecx
44ED3D  jb      loc_44EE1E
44ED43  push    ebx
44ED44  mov     ebx, [ebp+arg_0]
44ED47  push    esi
44ED48  push    edi
44ED49  lea     esp, [esp+0]
44ED50  mov     edx, [ebp+arg_4]
44ED53  mov     eax, edx
44ED55  shr     eax, 1
44ED57  add     eax, eax
44ED59  mov     edi, [ebx+eax*8+8]
44ED5D  xor     esi, esi
44ED5F  mov     ecx, esi
44ED61  add     ecx, ecx
44ED63  lea     eax, [ebx+ecx*8+8]
44ED67  dec     edx
44ED68  cmp     [eax], edi
44ED6A  jle     short loc_44ED78
44ED6C  lea     esp, [esp+0]
44ED70  add     eax, 10h
44ED73  inc     esi
44ED74  cmp     [eax], edi
44ED76  jg      short loc_44ED70
44ED78  mov     eax, edx
44ED7A  add     eax, eax
44ED7C  cmp     [ebx+eax*8+8], edi
44ED80  lea     eax, [ebx+eax*8+8]
44ED84  jge     short loc_44ED8E
44ED86  sub     eax, 10h
44ED89  dec     edx
44ED8A  cmp     [eax], edi
44ED8C  jl      short loc_44ED86
44ED8E  cmp     esi, edx
44ED90  jnb     short loc_44EDEE
44ED92  mov     eax, esi
44ED94  shl     eax, 4
44ED97  mov     ecx, [eax+ebx]
44ED9A  add     eax, ebx
44ED9C  mov     [ebp+var_14], ecx
44ED9F  mov     ecx, [eax+4]
44EDA2  mov     [ebp+var_10], ecx
44EDA5  mov     ecx, [eax+8]
44EDA8  mov     [ebp+var_C], ecx
44EDAB  mov     ecx, [eax+0Ch]
44EDAE  mov     [ebp+var_8], ecx
44EDB1  mov     ecx, edx
44EDB3  shl     ecx, 4
44EDB6  add     ecx, ebx
44EDB8  mov     ebx, [ecx]
44EDBA  mov     [eax], ebx
44EDBC  mov     ebx, [ecx+4]
44EDBF  mov     [eax+4], ebx
44EDC2  mov     ebx, [ecx+8]
44EDC5  mov     [eax+8], ebx
44EDC8  mov     ebx, [ecx+0Ch]
44EDCB  mov     [eax+0Ch], ebx
44EDCE  mov     eax, [ebp+var_14]
44EDD1  mov     ebx, [ebp+arg_0]
44EDD4  mov     [ecx], eax
44EDD6  mov     eax, [ebp+var_10]
44EDD9  mov     [ecx+4], eax
44EDDC  mov     eax, [ebp+var_C]
44EDDF  mov     [ecx+8], eax
44EDE2  mov     eax, [ebp+var_8]
44EDE5  mov     [ecx+0Ch], eax
44EDE8  inc     esi
44EDE9  jmp     loc_44ED5F
44EDEE  cmp     esi, 2
44EDF1  jb      short loc_44EDFD
44EDF3  mov     ecx, [ebp+var_4]
44EDF6  push    esi
44EDF7  push    ebx
44EDF8  call    sub_44ED30
44EDFD  mov     eax, [ebp+arg_4]
44EE00  sub     eax, esi
44EE02  cmp     eax, 2
44EE05  jb      short loc_44EE1B
44EE07  shl     esi, 4
44EE0A  add     ebx, esi
44EE0C  mov     [ebp+arg_4], eax
44EE0F  mov     [ebp+arg_0], ebx
44EE12  cmp     eax, 2
44EE15  jnb     loc_44ED50
44EE1B  pop     edi
44EE1C  pop     esi
44EE1D  pop     ebx
44EE1E  mov     esp, ebp
44EE20  pop     ebp
44EE21  retn    8

;==== 44EE30 sub_44EE30
44EE30  push    ebp
44EE31  mov     ebp, esp
44EE33  sub     esp, 1Ch
44EE36  push    edi
44EE37  push    80000h; unsigned int
44EE3C  mov     dword ptr [esi+2167Ch], 0
44EE46  call    ??_U@YAPAXI@Z; operator new[](uint)
44EE4B  mov     [esi+21680h], eax
44EE51  add     esp, 4
44EE54  mov     eax, esi
44EE56  call    sub_44F460
44EE5B  mov     edi, dword_565DA8
44EE61  push    esi
44EE62  mov     ecx, offset sub_44F180
44EE67  mov     eax, edi
44EE69  call    sub_4468F0
44EE6E  mov     eax, 1
44EE73  mov     ecx, edi
44EE75  call    sub_446910
44EE7A  mov     edi, dword_565DA8
44EE80  push    0
44EE82  xor     ecx, ecx
44EE84  mov     eax, edi
44EE86  call    sub_4468F0
44EE8B  mov     eax, [esi+2167Ch]
44EE91  mov     ecx, [esi+21680h]
44EE97  push    eax
44EE98  push    ecx
44EE99  mov     ecx, esi
44EE9B  call    sub_44ED30
44EEA0  mov     eax, [ebp+arg_4]
44EEA3  mov     edx, [ebp+arg_0]
44EEA6  mov     ecx, [ebp+arg_8]
44EEA9  mov     [ebp+var_14], eax
44EEAC  mov     eax, [ebp+arg_10]
44EEAF  mov     [ebp+var_18], edx
44EEB2  mov     edx, [ebp+arg_C]
44EEB5  mov     [ebp+var_10], ecx
44EEB8  mov     ecx, [ebp+arg_18]
44EEBB  mov     [ebp+var_8], eax
44EEBE  mov     eax, esi
44EEC0  mov     dword ptr [esi+21684h], 0
44EECA  mov     [ebp+var_1C], esi
44EECD  mov     [ebp+var_C], edx
44EED0  mov     [ebp+var_4], ecx
44EED3  call    sub_44F4B0
44EED8  lea     edx, [ebp+var_1C]
44EEDB  push    edx
44EEDC  mov     ecx, offset sub_44F440
44EEE1  mov     eax, edi
44EEE3  call    sub_4468F0
44EEE8  mov     eax, 1
44EEED  mov     ecx, edi
44EEEF  call    sub_446910
44EEF4  mov     eax, dword_565DA8
44EEF9  push    0
44EEFB  xor     ecx, ecx
44EEFD  call    sub_4468F0
44EF02  mov     eax, [esi+21684h]
44EF08  mov     ecx, [ebp+arg_14]
44EF0B  mov     [ecx], eax
44EF0D  mov     edx, [esi+21680h]
44EF13  push    edx; void *
44EF14  call    ??_V@YAXPAX@Z; operator delete[](void *)
44EF19  add     esp, 4
44EF1C  pop     edi
44EF1D  mov     esp, ebp
44EF1F  pop     ebp
44EF20  retn    1Ch

;==== 44EF30 sub_44EF30
44EF30  push    ebp
44EF31  mov     ebp, esp
44EF33  mov     eax, 4030h
44EF38  call    __alloca_probe
44EF3D  mov     eax, dword_4FB734
44EF42  xor     eax, ebp
44EF44  mov     [ebp+var_4], eax
44EF47  mov     ecx, dword_565DA8
44EF4D  push    ebx
44EF4E  push    esi
44EF4F  call    sub_446A10
44EF54  lea     esi, [ebp+var_4024]
44EF5A  mov     edx, edi
44EF5C  mov     ebx, eax
44EF5E  call    sub_44F470
44EF63  mov     esi, eax
44EF65  mov     eax, dword_565DA8
44EF6A  push    ebx
44EF6B  mov     [ebp+var_401C], esi
44EF71  call    sub_446A30
44EF76  test    esi, esi
44EF78  js      loc_44F15C
44EF7E  xor     ecx, ecx
44EF80  xor     ebx, ebx
44EF82  mov     [ebp+var_4010], ebx
44EF88  mov     [ebp+var_4020], ecx
44EF8E  cmp     [ebp+var_4024], ecx
44EF94  jle     loc_44F11D
44EF9A  lea     eax, [ebp+Src]
44EFA0  mov     [ebp+var_4018], eax
44EFA6  jmp     short loc_44EFB6
44EFA8  jmp     short loc_44EFB0
44EFB0  mov     esi, [ebp+var_401C]
44EFB6  lea     eax, [edi+esi*4+14h]
44EFBA  mov     ebx, [eax+ecx*4]
44EFBD  test    ebx, ebx
44EFBF  jz      loc_44F0FE
44EFC5  lea     eax, [ebp+var_4030]
44EFCB  mov     ecx, ebx
44EFCD  call    sub_44DF30
44EFD2  mov     edx, [ebp+var_402C]
44EFD8  sub     edx, [edi+20818h]
44EFDE  mov     eax, [ebp+var_4028]
44EFE4  sub     eax, [edi+2081Ch]
44EFEA  mov     ecx, [ebp+var_4030]
44EFF0  sub     ecx, [edi+20814h]
44EFF6  mov     [ebp+var_400C], edx
44EFFC  fild    [ebp+var_400C]
44F002  mov     [ebp+var_4014], eax
44F008  fild    [ebp+var_4014]
44F00E  mov     [ebp+var_4008], ecx
44F014  fld     qword ptr [edi+20828h]
44F01A  fmul    st, st(2)
44F01C  fld     st(1)
44F01E  fmul    qword ptr [edi+20820h]
44F024  fsubp   st(1), st
44F026  call    __ftol2_sse
44F02B  fxch    st(1)
44F02D  fmul    qword ptr [edi+20820h]
44F033  mov     [ebp+var_400C], eax
44F039  fld     qword ptr [edi+20828h]
44F03F  fmulp   st(2), st
44F041  faddp   st(1), st
44F043  call    __ftol2_sse
44F048  mov     [ebp+var_4014], eax
44F04E  fild    [ebp+var_4014]
44F054  fild    [ebp+var_4008]
44F05A  fld     qword ptr [edi+20838h]
44F060  fmul    st, st(2)
44F062  fld     qword ptr [edi+20830h]
44F068  fmul    st, st(2)
44F06A  fsubp   st(1), st
44F06C  call    __ftol2_sse
44F071  fld     qword ptr [edi+20830h]
44F077  fmulp   st(2), st
44F079  mov     esi, eax
44F07B  fmul    qword ptr [edi+20838h]
44F081  faddp   st(1), st
44F083  call    __ftol2_sse
44F088  mov     [ebp+var_4008], eax
44F08E  fild    [ebp+var_4008]
44F094  fild    [ebp+var_400C]
44F09A  fld     st(1)
44F09C  fmul    qword ptr [edi+20848h]
44F0A2  fld     qword ptr [edi+20840h]
44F0A8  fmul    st, st(2)
44F0AA  fsubp   st(1), st
44F0AC  call    __ftol2_sse
44F0B1  fmul    qword ptr [edi+20848h]
44F0B7  fld     qword ptr [edi+20840h]
44F0BD  mov     [ebp+var_4008], eax
44F0C3  fmulp   st(2), st
44F0C5  faddp   st(1), st
44F0C7  call    __ftol2_sse
44F0CC  mov     [ebp+var_400C], eax
44F0D2  test    esi, esi
44F0D4  js      short loc_44F0FE
44F0D6  mov     edx, [ebp+var_4018]
44F0DC  inc     [ebp+var_4010]
44F0E2  mov     ecx, edx
44F0E4  add     edx, 10h
44F0E7  mov     [ebp+var_4018], edx
44F0ED  mov     edx, [ebp+var_4008]
44F0F3  mov     [ecx], edx
44F0F5  mov     [ecx+4], eax
44F0F8  mov     [ecx+8], esi
44F0FB  mov     [ecx+0Ch], ebx
44F0FE  mov     ecx, [ebp+var_4020]
44F104  inc     ecx
44F105  mov     [ebp+var_4020], ecx
44F10B  cmp     ecx, [ebp+var_4024]
44F111  jl      loc_44EFB0
44F117  mov     ebx, [ebp+var_4010]
44F11D  lea     esi, [edi+21688h]
44F123  push    esi; lpCriticalSection
44F124  call    ds:EnterCriticalSection
44F12A  mov     eax, [edi+2167Ch]
44F130  mov     ecx, ebx
44F132  shl     ecx, 4
44F135  push    ecx; Size
44F136  shl     eax, 4
44F139  add     eax, [edi+21680h]
44F13F  lea     edx, [ebp+Src]
44F145  push    edx; Src
44F146  push    eax; void *
44F147  call    _memcpy_0
44F14C  add     [edi+2167Ch], ebx
44F152  add     esp, 0Ch
44F155  push    esi; lpCriticalSection
44F156  call    ds:LeaveCriticalSection
44F15C  mov     ecx, [ebp+var_4]
44F15F  xor     eax, eax
44F161  cmp     [ebp+var_401C], eax
44F167  pop     esi
44F168  setnl   al
44F16B  xor     ecx, ebp
44F16D  pop     ebx
44F16E  call    sub_4ABDD5
44F173  mov     esp, ebp
44F175  pop     ebp
44F176  retn

;==== 44F180 sub_44F180
44F180  push    ebp
44F181  mov     ebp, esp
44F183  push    edi
44F184  mov     edi, [ebp+arg_0]
44F187  call    sub_44EF30
44F18C  pop     edi
44F18D  pop     ebp
44F18E  retn

;==== 44F190 sub_44F190
44F190  push    ebp
44F191  mov     ebp, esp
44F193  and     esp, 0FFFFFFF8h
44F196  sub     esp, 50h
44F199  mov     ecx, dword_565DA8
44F19F  push    ebx
44F1A0  push    edi
44F1A1  call    sub_446A10
44F1A6  mov     ecx, esi
44F1A8  mov     ebx, eax
44F1AA  call    sub_44F4C0
44F1AF  mov     edi, eax
44F1B1  mov     eax, dword_565DA8
44F1B6  push    ebx
44F1B7  call    sub_446A30
44F1BC  mov     eax, dword_565DA8
44F1C1  mov     [esp+58h+var_40], 0
44F1C9  call    sub_454260
44F1CE  mov     ebx, eax
44F1D0  mov     eax, [esi+2167Ch]
44F1D6  lea     eax, [ebx+eax-1]
44F1DA  cdq
44F1DB  idiv    ebx
44F1DD  xor     ecx, ecx
44F1DF  mov     edx, 10h
44F1E4  mov     [esp+58h+var_28], ebx
44F1E8  mul     edx
44F1EA  seto    cl
44F1ED  neg     ecx
44F1EF  or      ecx, eax
44F1F1  push    ecx; unsigned int
44F1F2  call    ??_U@YAPAXI@Z; operator new[](uint)
44F1F7  mov     [esp+5Ch+Src], eax
44F1FB  mov     eax, [ebp+arg_0]
44F1FE  mov     ecx, [eax+8]
44F201  add     esp, 4
44F204  lea     eax, [esp+58h+var_10]
44F208  call    sub_409180
44F20D  mov     [esp+58h+var_34], edi
44F211  cmp     edi, [esi+2167Ch]
44F217  jge     loc_44F3E8
44F21D  mov     ecx, [esp+58h+Src]
44F221  shl     edi, 4
44F224  shl     ebx, 4
44F227  mov     [esp+58h+var_48], edi
44F22B  mov     [esp+58h+var_24], ebx
44F22F  mov     [esp+58h+var_38], ecx
44F233  jmp     short loc_44F240
44F240  mov     edx, [esi+21680h]
44F246  mov     eax, [esi+20850h]
44F24C  lea     ecx, [edi+edx]
44F24F  mov     edx, [ecx+0Ch]
44F252  mov     ecx, [ecx+8]
44F255  mov     ebx, [edx]
44F257  sar     ecx, 8
44F25A  add     ecx, eax
44F25C  mov     [esp+58h+var_44], edx
44F260  shl     eax, 5
44F263  cdq
44F264  idiv    ecx
44F266  mov     ecx, [esp+58h+var_44]
44F26A  mov     edx, 20h ; ' '
44F26F  sub     edx, eax
44F271  mov     eax, [ebx+4]
44F274  push    edx
44F275  call    eax
44F277  mov     [esp+58h+var_4C], eax
44F27B  test    eax, eax
44F27D  jz      loc_44F3C8
44F283  mov     ecx, [esi+21680h]
44F289  mov     eax, [ecx+edi]
44F28C  add     ecx, edi
44F28E  mov     edi, [esi+20850h]
44F294  imul    eax, edi
44F297  cdq
44F298  mov     ebx, edi
44F29A  shl     ebx, 8
44F29D  add     ebx, [ecx+8]
44F2A0  idiv    ebx
44F2A2  mov     edx, [esp+58h+var_4C]
44F2A6  mov     [esp+58h+var_44], ebx
44F2AA  mov     ebx, eax
44F2AC  mov     eax, [edx+8]
44F2AF  mov     edx, [ebp+arg_0]
44F2B2  shr     eax, 1
44F2B4  sub     ebx, eax
44F2B6  mov     eax, [ecx+4]
44F2B9  add     ebx, [edx+10h]
44F2BC  imul    eax, edi
44F2BF  cdq
44F2C0  idiv    [esp+58h+var_44]
44F2C4  mov     edx, [ebp+arg_0]
44F2C7  mov     edi, [edx+14h]
44F2CA  mov     [esp+58h+var_44], 0
44F2D2  sub     edi, eax
44F2D4  mov     eax, [esp+58h+var_4C]
44F2D8  mov     edx, [eax+0Ch]
44F2DB  shr     edx, 1
44F2DD  sub     edi, edx
44F2DF  mov     edx, [ebp+arg_0]
44F2E2  mov     eax, [edx+4]
44F2E5  mov     [esp+58h+var_2C], eax
44F2E9  test    eax, eax
44F2EB  jz      loc_44F3AF
44F2F1  mov     edx, [edx+0Ch]
44F2F4  jmp     short loc_44F2FA
44F2F6  mov     edx, [esp+58h+var_30]
44F2FA  mov     eax, [ecx+8]
44F2FD  cmp     eax, [edx]
44F2FF  jle     short loc_44F31C
44F301  mov     eax, [esp+58h+var_44]
44F305  inc     eax
44F306  add     edx, 4
44F309  mov     [esp+58h+var_44], eax
44F30D  mov     [esp+58h+var_30], edx
44F311  cmp     eax, [esp+58h+var_2C]
44F315  jb      short loc_44F2F6
44F317  jmp     loc_44F3AF
44F31C  mov     ecx, [ecx+0Ch]
44F31F  mov     eax, [ecx]
44F321  mov     edx, [eax+0Ch]
44F324  call    edx
44F326  mov     ecx, [esp+58h+var_48]
44F32A  push    eax
44F32B  mov     eax, [esi+21680h]
44F331  mov     eax, [ecx+eax+0Ch]
44F335  call    sub_44DF50
44F33A  push    eax
44F33B  mov     eax, [esp+60h+var_44]
44F33F  lea     edx, [eax+eax*2]
44F342  mov     eax, [ebp+arg_0]
44F345  mov     ecx, [eax+8]
44F348  mov     eax, [esp+60h+var_4C]
44F34C  push    ebx
44F34D  lea     ecx, [ecx+edx*8]
44F350  call    sub_40A520
44F355  add     esp, 0Ch
44F358  test    eax, eax
44F35A  jnz     short loc_44F3AF
44F35C  mov     ecx, [esp+58h+var_4C]
44F360  lea     eax, [esp+58h+var_20]
44F364  call    sub_409180
44F369  mov     edx, eax
44F36B  push    edx
44F36C  mov     ecx, edi
44F36E  mov     edx, ebx
44F370  call    sub_409160
44F375  add     esp, 4
44F378  lea     ecx, [esp+58h+var_10]
44F37C  lea     eax, [esp+58h+var_20]
44F380  call    sub_409100
44F385  mov     eax, [esp+58h+var_38]
44F389  mov     ecx, [esp+58h+var_20]
44F38D  mov     edx, [esp+58h+var_1C]
44F391  inc     [esp+58h+var_40]
44F395  mov     [eax], ecx
44F397  mov     ecx, [esp+58h+var_18]
44F39B  mov     [eax+4], edx
44F39E  mov     edx, dword ptr [esp+58h+var_14]
44F3A2  mov     [eax+8], ecx
44F3A5  mov     [eax+0Ch], edx
44F3A8  add     eax, 10h
44F3AB  mov     [esp+58h+var_38], eax
44F3AF  mov     eax, [esi+21680h]
44F3B5  mov     ecx, [esp+58h+var_48]
44F3B9  mov     ecx, [ecx+eax+0Ch]
44F3BD  mov     edx, [ecx]
44F3BF  mov     eax, [edx+8]
44F3C2  call    eax
44F3C4  mov     edi, [esp+58h+var_48]
44F3C8  mov     eax, [esp+58h+var_34]
44F3CC  add     eax, [esp+58h+var_28]
44F3D0  add     edi, [esp+58h+var_24]
44F3D4  mov     [esp+58h+var_34], eax
44F3D8  mov     [esp+58h+var_48], edi
44F3DC  cmp     eax, [esi+2167Ch]
44F3E2  jl      loc_44F240
44F3E8  lea     edi, [esi+21688h]
44F3EE  push    edi; lpCriticalSection
44F3EF  call    ds:EnterCriticalSection
44F3F5  mov     ebx, [esp+58h+var_40]
44F3F9  mov     eax, [esi+21684h]
44F3FF  mov     edx, [esp+58h+Src]
44F403  mov     ecx, ebx
44F405  shl     ecx, 4
44F408  push    ecx; Size
44F409  mov     ecx, [ebp+arg_0]
44F40C  shl     eax, 4
44F40F  add     eax, [ecx+18h]
44F412  push    edx; Src
44F413  push    eax; void *
44F414  call    _memcpy_0
44F419  add     [esi+21684h], ebx
44F41F  add     esp, 0Ch
44F422  push    edi; lpCriticalSection
44F423  call    ds:LeaveCriticalSection
44F429  mov     edx, [esp+58h+Src]
44F42D  push    edx; void *
44F42E  call    ??_V@YAXPAX@Z; operator delete[](void *)
44F433  add     esp, 4
44F436  pop     edi
44F437  xor     eax, eax
44F439  pop     ebx
44F43A  mov     esp, ebp
44F43C  pop     ebp
44F43D  retn    4

;==== 44F440 sub_44F440
44F440  push    ebp
44F441  mov     ebp, esp
44F443  mov     eax, [ebp+arg_0]
44F446  push    esi
44F447  mov     esi, [eax]
44F449  push    eax
44F44A  call    sub_44F190
44F44F  pop     esi
44F450  pop     ebp
44F451  retn

;==== 44F460 sub_44F460
44F460  mov     dword ptr [eax+216A0h], 0
44F46A  retn

;==== 44F470 sub_44F470
44F470  mov     eax, [edx+216A0h]
44F476  or      ecx, 0FFFFFFFFh
44F479  cmp     eax, 8000h
44F47E  jge     short loc_44F49F
44F480  mov     ecx, 8000h
44F485  sub     ecx, eax
44F487  cmp     ecx, 400h
44F48D  jl      short loc_44F494
44F48F  mov     ecx, 400h
44F494  mov     [esi], ecx
44F496  add     ecx, eax
44F498  mov     [edx+216A0h], ecx
44F49E  retn
44F49F  mov     eax, ecx
44F4A1  retn

;==== 44F4B0 sub_44F4B0
44F4B0  mov     dword ptr [eax+216A4h], 0
44F4BA  retn

;==== 44F4C0 sub_44F4C0
44F4C0  mov     eax, [ecx+216A4h]
44F4C6  lea     edx, [eax+1]
44F4C9  mov     [ecx+216A4h], edx
44F4CF  retn

;==== 44F540 sub_44F540
44F540  cmp     dword ptr [edx+8], 0
44F544  jz      short loc_44F55B
44F546  mov     ecx, [edx+0Ch]
44F549  mov     [eax], ecx
44F54B  mov     ecx, [edx+10h]
44F54E  mov     [eax+4], ecx
44F551  mov     ecx, [edx+14h]
44F554  mov     [eax+8], ecx
44F557  mov     eax, [edx+8]
44F55A  retn
44F55B  xor     ecx, ecx
44F55D  mov     [eax], ecx
44F55F  mov     [eax+4], ecx
44F562  mov     [eax+8], ecx
44F565  mov     eax, [edx+8]
44F568  retn

;==== 44F8C0 sub_44F8C0
44F8C0  push    ebp
44F8C1  mov     ebp, esp
44F8C3  push    0FFFFFFFFh
44F8C5  push    offset SEH_44F8C0
44F8CA  mov     eax, large fs:0
44F8D0  push    eax
44F8D1  push    ebx
44F8D2  push    esi
44F8D3  push    edi
44F8D4  mov     eax, dword_4FB734
44F8D9  xor     eax, ebp
44F8DB  push    eax
44F8DC  lea     eax, [ebp+var_C]
44F8DF  mov     large fs:0, eax
44F8E5  mov     edi, ecx
44F8E7  mov     esi, [ebp+arg_0]
44F8EA  mov     eax, edi
44F8EC  imul    eax, 8Ch
44F8F2  add     eax, offset dword_50FEE8
44F8F7  push    eax
44F8F8  mov     eax, 1
44F8FD  call    sub_44DC40
44F902  mov     [ebp+var_4], 0
44F909  mov     dword ptr [esi], offset ??_7DCPFirefly@@6B@; const DCPFirefly::`vftable'
44F90F  cmp     edi, 40h ; '@'
44F912  jnb     loc_44FA09
44F918  lea     ebx, [edi+edi*8]
44F91B  cmp     dword_50ECE8[ebx*8], 0
44F923  lea     ebx, dword_50ECE8[ebx*8]
44F92A  jz      loc_44FA09
44F930  mov     ecx, [ebp+arg_4]
44F933  mov     [esi+40h], ecx
44F936  mov     edi, [ebx+8]
44F939  call    sub_44DEF0
44F93E  add     eax, [ebx+4]
44F941  mov     edx, [esi+40h]
44F944  mov     [esi+44h], eax
44F947  mov     dword ptr [esi+48h], 0
44F94E  mov     edi, [ebx+0Ch]
44F951  add     edi, edi
44F953  mov     [ebp+arg_4], edx
44F956  call    sub_44DEF0
44F95B  mov     ecx, eax
44F95D  mov     eax, [ebp+arg_4]
44F960  mov     edx, [eax]
44F962  sub     edx, [ebx+0Ch]
44F965  add     ecx, edx
44F967  mov     [esi+10h], ecx
44F96A  mov     edi, [eax+4]
44F96D  add     edi, edi
44F96F  call    sub_44DEF0
44F974  mov     edx, [ebx+10h]
44F977  mov     ecx, [esi+40h]
44F97A  sub     edx, [ecx+4]
44F97D  add     eax, edx
44F97F  mov     [esi+14h], eax
44F982  mov     edi, [ebx+14h]
44F985  add     edi, edi
44F987  call    sub_44DEF0
44F98C  mov     ecx, [esi+40h]
44F98F  mov     edx, [ecx+8]
44F992  sub     edx, [ebx+14h]
44F995  add     eax, edx
44F997  mov     [esi+18h], eax
44F99A  mov     edi, [ebx+1Ch]
44F99D  add     edi, edi
44F99F  call    sub_44DEF0
44F9A4  mov     ecx, [ebx+18h]
44F9A7  sub     ecx, [ebx+1Ch]
44F9AA  add     eax, ecx
44F9AC  mov     [esi+50h], eax
44F9AF  mov     edi, [ebx+24h]
44F9B2  add     edi, edi
44F9B4  call    sub_44DEF0
44F9B9  mov     edx, [ebx+20h]
44F9BC  sub     edx, [ebx+24h]
44F9BF  add     eax, edx
44F9C1  mov     [esi+58h], eax
44F9C4  mov     edi, [ebx+2Ch]
44F9C7  add     edi, edi
44F9C9  call    sub_44DEF0
44F9CE  mov     ecx, [ebx+28h]
44F9D1  sub     ecx, [ebx+2Ch]
44F9D4  push    1
44F9D6  add     eax, ecx
44F9D8  push    esi
44F9D9  mov     [esi+60h], eax
44F9DC  call    sub_450250
44F9E1  call    sub_450060
44F9E6  mov     edx, [ebx+38h]
44F9E9  mov     [esi+1Ch], edx
44F9EC  mov     eax, [ebx+3Ch]
44F9EF  mov     [esi+6Ch], eax
44F9F2  mov     ecx, [ebx+40h]
44F9F5  mov     [esi+70h], ecx
44F9F8  mov     ecx, [ebx+44h]
44F9FB  mov     eax, esi
44F9FD  call    sub_4339A0
44FA02  mov     dword ptr [esi+0Ch], 1
44FA09  mov     dword ptr [esi+74h], 0
44FA10  mov     [ebp+var_4], 0FFFFFFFFh
44FA17  mov     eax, esi
44FA19  mov     ecx, [ebp+var_C]
44FA1C  mov     large fs:0, ecx
44FA23  pop     ecx
44FA24  pop     edi
44FA25  pop     esi
44FA26  pop     ebx
44FA27  mov     esp, ebp
44FA29  pop     ebp
44FA2A  retn    8
4D8FF0  mov     ecx, [ebp+arg_0]
4D8FF3  jmp     sub_44DCC0
4D8FF8  mov     edx, [esp-4+arg_4]
4D8FFC  lea     eax, [edx+0Ch]
4D8FFF  mov     ecx, [edx-10h]
4D9002  xor     ecx, eax
4D9004  call    sub_4ABDD5
4D9009  mov     eax, offset stru_4F6848
4D900E  jmp     ___CxxFrameHandler3

;==== 44FA30 sub_44FA30
44FA30  push    ebp
44FA31  mov     ebp, esp
44FA33  push    esi
44FA34  mov     esi, ecx
44FA36  call    sub_44FA60
44FA3B  test    [ebp+arg_0], 1
44FA3F  jz      short loc_44FA4A
44FA41  push    esi; void *
44FA42  call    ??3@YAXPAX@Z; operator delete(void *)
44FA47  add     esp, 4
44FA4A  mov     eax, esi
44FA4C  pop     esi
44FA4D  pop     ebp
44FA4E  retn    4

;==== 44FA60 sub_44FA60
44FA60  push    ebp
44FA61  mov     ebp, esp
44FA63  push    0FFFFFFFFh
44FA65  push    offset SEH_44FA60
44FA6A  mov     eax, large fs:0
44FA70  push    eax
44FA71  push    ecx
44FA72  push    esi
44FA73  mov     eax, dword_4FB734
44FA78  xor     eax, ebp
44FA7A  push    eax
44FA7B  lea     eax, [ebp+var_C]
44FA7E  mov     large fs:0, eax
44FA84  mov     esi, ecx
44FA86  mov     [ebp+var_10], esi
44FA89  mov     dword ptr [esi], offset ??_7DCPFirefly@@6B@; const DCPFirefly::`vftable'
44FA8F  mov     [ebp+var_4], 0
44FA96  call    sub_44FF50
44FA9B  mov     [ebp+var_4], 0FFFFFFFFh
44FAA2  mov     ecx, esi
44FAA4  call    sub_44DCC0
44FAA9  mov     ecx, [ebp+var_C]
44FAAC  mov     large fs:0, ecx
44FAB3  pop     ecx
44FAB4  pop     esi
44FAB5  mov     esp, ebp
44FAB7  pop     ebp
44FAB8  retn
4D9020  mov     ecx, [ebp+var_10]
4D9023  jmp     sub_44DCC0
4D9028  mov     edx, [esp-4+arg_4]
4D902C  lea     eax, [edx+0Ch]
4D902F  mov     ecx, [edx-0Ch]
4D9032  xor     ecx, eax
4D9034  call    sub_4ABDD5
4D9039  mov     eax, offset stru_4F6874
4D903E  jmp     ___CxxFrameHandler3

;==== 44FCE0 sub_44FCE0
44FCE0  xor     eax, eax
44FCE2  cmp     ecx, 40h ; '@'
44FCE5  jnb     short locret_44FCF7
44FCE7  imul    ecx, 8Ch
44FCED  cmp     eax, dword_5121E8[ecx]
44FCF3  sbb     eax, eax
44FCF5  neg     eax
44FCF7  retn

;==== 44FD50 sub_44FD50
44FD50  push    ebp
44FD51  mov     ebp, esp
44FD53  and     esp, 0FFFFFFF8h
44FD56  sub     esp, 24h
44FD59  push    ebx
44FD5A  push    esi
44FD5B  mov     esi, [ebp+arg_0]
44FD5E  mov     ebx, ecx
44FD60  xor     ecx, ecx
44FD62  push    edi
44FD63  cmp     esi, 1Fh
44FD66  ja      loc_44FF3D
44FD6C  mov     eax, ebx
44FD6E  call    sub_44DF20
44FD73  mov     edx, eax
44FD75  imul    edx, 23h ; '#'
44FD78  mov     eax, ebx
44FD7A  add     edx, esi
44FD7C  call    sub_44E010
44FD81  mov     edx, dword_50FEEC[edx*4]
44FD88  lea     eax, [eax+eax*2]
44FD8B  cmp     [edx+eax*8], ecx
44FD8E  jz      loc_44FF3D
44FD94  mov     eax, ebx
44FD96  call    sub_44DF20
44FD9B  mov     ecx, eax
44FD9D  imul    ecx, 23h ; '#'
44FDA0  mov     eax, ebx
44FDA2  add     ecx, esi
44FDA4  call    sub_44E010
44FDA9  lea     edx, [eax+eax*2]
44FDAC  mov     eax, dword_50FEEC[ecx*4]
44FDB3  lea     edi, [eax+edx*8]
44FDB6  mov     eax, ebx
44FDB8  mov     [esp+30h+var_1C], edi
44FDBC  call    sub_44DF20
44FDC1  cmp     dword_50EBE8[eax*4], 0
44FDC9  jbe     loc_44FF32
44FDCF  mov     edx, [ebx]
44FDD1  mov     eax, [edx+8]
44FDD4  mov     ecx, ebx
44FDD6  call    eax
44FDD8  mov     eax, ebx
44FDDA  call    sub_44DF20
44FDDF  mov     ecx, eax
44FDE1  mov     eax, ebx
44FDE3  mov     [esp+30h+var_20], ecx
44FDE7  call    sub_44E020
44FDEC  imul    ecx, 23h ; '#'
44FDEF  add     ecx, esi
44FDF1  lea     edx, [eax+eax*2]
44FDF4  mov     eax, dword_5121EC[ecx*4]
44FDFB  mov     ecx, [edi+8]
44FDFE  lea     eax, [eax+edx*8]
44FE01  mov     edx, [eax+8]
44FE04  mov     [esp+30h+var_24], eax
44FE08  mov     [esp+30h+var_10], edx
44FE0C  cmp     ecx, edx
44FE0E  jb      short loc_44FE14
44FE10  mov     [esp+30h+var_10], ecx
44FE14  mov     eax, [eax+0Ch]
44FE17  mov     ecx, [edi+0Ch]
44FE1A  mov     [esp+30h+var_C], eax
44FE1E  cmp     ecx, eax
44FE20  jb      short loc_44FE26
44FE22  mov     [esp+30h+var_C], ecx
44FE26  push    18h; Size
44FE28  call    ??2@YAPAXI@Z; operator new(uint)
44FE2D  mov     ecx, [esp+34h+var_10]
44FE31  mov     edx, [esp+34h+var_C]
44FE35  mov     esi, eax
44FE37  add     esp, 4
44FE3A  mov     [ebx+74h], esi
44FE3D  mov     eax, [edi+10h]
44FE40  push    ecx
44FE41  call    sub_409020
44FE46  mov     eax, [ebx+74h]
44FE49  add     esp, 4
44FE4C  xor     ecx, ecx
44FE4E  call    sub_40A610
44FE53  mov     ecx, [ebx+74h]
44FE56  mov     esi, [esp+30h+var_1C]
44FE5A  mov     edx, [ecx+8]
44FE5D  mov     edi, [ecx+0Ch]
44FE60  sub     edx, [esi+8]
44FE63  sub     edi, [esi+0Ch]
44FE66  push    0
44FE68  shr     edx, 1
44FE6A  push    80h
44FE6F  shr     edi, 1
44FE71  push    edx
44FE72  mov     eax, esi
44FE74  call    sub_40A520
44FE79  mov     eax, [esp+3Ch+var_10]
44FE7D  mov     ecx, [esp+3Ch+var_24]
44FE81  mov     edx, [esp+3Ch+var_C]
44FE85  add     esp, 0Ch
44FE88  cmp     eax, [ecx+8]
44FE8B  jnz     short loc_44FEBA
44FE8D  cmp     edx, [ecx+0Ch]
44FE90  jnz     short loc_44FEBA
44FE92  mov     edx, [esp+30h+var_20]
44FE96  mov     edx, dword_50EBE8[edx*4]
44FE9D  mov     eax, [ebx+74h]
44FEA0  push    1
44FEA2  push    edx
44FEA3  push    ecx
44FEA4  push    eax
44FEA5  push    eax
44FEA6  call    sub_40C4F0
44FEAB  mov     eax, [ebx+74h]
44FEAE  add     esp, 14h
44FEB1  pop     edi
44FEB2  pop     esi
44FEB3  pop     ebx
44FEB4  mov     esp, ebp
44FEB6  pop     ebp
44FEB7  retn    4
44FEBA  push    eax
44FEBB  mov     eax, [esi+10h]
44FEBE  lea     esi, [esp+34h+var_18]
44FEC2  call    sub_409020
44FEC7  add     esp, 4
44FECA  xor     ecx, ecx
44FECC  mov     eax, esi
44FECE  call    sub_40A610
44FED3  mov     eax, [esp+30h+var_24]
44FED7  mov     ecx, [esp+30h+var_10]
44FEDB  sub     ecx, [eax+8]
44FEDE  mov     edi, [esp+30h+var_C]
44FEE2  sub     edi, [eax+0Ch]
44FEE5  push    0
44FEE7  shr     ecx, 1
44FEE9  push    80h
44FEEE  push    ecx
44FEEF  shr     edi, 1
44FEF1  mov     ecx, esi
44FEF3  call    sub_40A520
44FEF8  mov     edx, [esp+3Ch+var_20]
44FEFC  mov     ecx, dword_50EBE8[edx*4]
44FF03  mov     eax, [ebx+74h]
44FF06  add     esp, 0Ch
44FF09  push    1
44FF0B  push    ecx
44FF0C  mov     edx, esi
44FF0E  push    edx
44FF0F  push    eax
44FF10  push    eax
44FF11  call    sub_40C4F0
44FF16  mov     eax, [esp+44h+var_18]
44FF1A  add     esp, 14h
44FF1D  push    eax; void *
44FF1E  call    ??_V@YAXPAX@Z; operator delete[](void *)
44FF23  mov     eax, [ebx+74h]
44FF26  add     esp, 4
44FF29  pop     edi
44FF2A  pop     esi
44FF2B  pop     ebx
44FF2C  mov     esp, ebp
44FF2E  pop     ebp
44FF2F  retn    4
44FF32  mov     eax, edi
44FF34  pop     edi
44FF35  pop     esi
44FF36  pop     ebx
44FF37  mov     esp, ebp
44FF39  pop     ebp
44FF3A  retn    4
44FF3D  pop     edi
44FF3E  pop     esi
44FF3F  mov     eax, ecx
44FF41  pop     ebx
44FF42  mov     esp, ebp
44FF44  pop     ebp
44FF45  retn    4

;==== 44FF50 sub_44FF50
44FF50  push    esi
44FF51  mov     esi, ecx
44FF53  mov     ecx, [esi+74h]
44FF56  xor     eax, eax
44FF58  test    ecx, ecx
44FF5A  jz      short loc_44FF7C
44FF5C  mov     eax, [ecx]
44FF5E  push    eax; void *
44FF5F  call    ??_V@YAXPAX@Z; operator delete[](void *)
44FF64  mov     ecx, [esi+74h]
44FF67  push    ecx; void *
44FF68  call    ??3@YAXPAX@Z; operator delete(void *)
44FF6D  add     esp, 8
44FF70  mov     dword ptr [esi+74h], 0
44FF77  mov     eax, 1
44FF7C  pop     esi
44FF7D  retn

;==== 44FF80 sub_44FF80
44FF80  xor     eax, eax
44FF82  cmp     [ecx+0Ch], eax
44FF85  jz      short locret_44FFC8
44FF87  mov     edx, [ecx+44h]
44FF8A  push    esi
44FF8B  mov     esi, [ecx+48h]
44FF8E  push    edi
44FF8F  mov     edi, [ecx+70h]
44FF92  sub     edx, esi
44FF94  cmp     edx, edi
44FF96  jnb     short loc_44FFAD
44FF98  mov     eax, edx
44FF9A  shl     eax, 8
44FF9D  xor     edx, edx
44FF9F  div     edi
44FFA1  mov     ecx, 100h
44FFA6  pop     edi
44FFA7  pop     esi
44FFA8  sub     ecx, eax
44FFAA  mov     eax, ecx
44FFAC  retn
44FFAD  mov     ecx, [ecx+6Ch]
44FFB0  cmp     esi, ecx
44FFB2  jnb     short loc_44FFC6
44FFB4  mov     eax, esi
44FFB6  shl     eax, 8
44FFB9  xor     edx, edx
44FFBB  div     ecx
44FFBD  mov     ecx, 100h
44FFC2  sub     ecx, eax
44FFC4  mov     eax, ecx
44FFC6  pop     edi
44FFC7  pop     esi
44FFC8  retn

;==== 44FFD0 sub_44FFD0
44FFD0  call    sub_44DFA0
44FFD5  xor     eax, eax
44FFD7  cmp     [ecx+0Ch], eax
44FFDA  jz      short locret_450058
44FFDC  mov     eax, [ecx+48h]
44FFDF  lea     edx, [eax+1]
44FFE2  mov     [ecx+48h], edx
44FFE5  cmp     eax, [ecx+44h]
44FFE8  jb      short loc_44FFED
44FFEA  xor     eax, eax
44FFEC  retn
44FFED  mov     eax, [ecx+50h]
44FFF0  push    ebx
44FFF1  mov     ebx, [ecx+4Ch]
44FFF4  sub     eax, ebx
44FFF6  push    esi
44FFF7  mov     esi, [ecx+68h]
44FFFA  imul    eax, esi
44FFFD  cdq
44FFFE  push    edi
44FFFF  mov     edi, [ecx+64h]
450002  idiv    edi
450004  add     eax, ebx
450006  add     [ecx+10h], eax
450009  mov     ebx, [ecx+54h]
45000C  mov     eax, [ecx+58h]
45000F  sub     eax, ebx
450011  imul    eax, esi
450014  cdq
450015  idiv    edi
450017  add     eax, ebx
450019  add     [ecx+14h], eax
45001C  mov     ebx, [ecx+5Ch]
45001F  mov     eax, [ecx+60h]
450022  sub     eax, ebx
450024  imul    eax, esi
450027  cdq
450028  idiv    edi
45002A  add     eax, ebx
45002C  add     [ecx+18h], eax
45002F  cmp     esi, edi
450031  jnb     short loc_450049
450033  mov     eax, [ecx+40h]
450036  cmp     dword ptr [eax+28h], 0
45003A  mov     eax, 1
45003F  jz      short loc_450055
450041  inc     esi
450042  pop     edi
450043  mov     [ecx+68h], esi
450046  pop     esi
450047  pop     ebx
450048  retn
450049  mov     esi, ecx
45004B  call    sub_450060
450050  mov     eax, 1
450055  pop     edi
450056  pop     esi
450057  pop     ebx
450058  retn

;==== 450060 sub_450060
450060  push    ebp
450061  mov     ebp, esp
450063  and     esp, 0FFFFFFF8h
450066  sub     esp, 28h
450069  mov     eax, [esi+50h]
45006C  mov     dword ptr [esp+28h+var_28], eax
45006F  mov     [esi+4Ch], eax
450072  mov     eax, [esi+58h]
450075  mov     dword ptr [esp+28h+var_20], eax
450079  mov     [esi+54h], eax
45007C  mov     eax, [esi+60h]
45007F  push    ebx
450080  mov     dword ptr [esp+2Ch+var_18], eax
450084  mov     [esi+5Ch], eax
450087  push    edi
450088  mov     eax, esi
45008A  call    sub_44DF20
45008F  lea     ebx, [eax+eax*8]
450092  mov     eax, [esi+40h]
450095  cmp     dword ptr [eax+2Ch], 0
450099  lea     ebx, dword_50ECE8[ebx*8]
4500A0  jz      loc_4501B3
4500A6  fild    dword ptr [esp+30h+var_28]
4500AA  mov     edi, [eax+34h]
4500AD  add     edi, edi
4500AF  fstp    [esp+30h+var_8]
4500B3  fild    dword ptr [esp+30h+var_20]
4500B7  fstp    [esp+30h+var_20]
4500BB  fild    dword ptr [esp+30h+var_18]
4500BF  mov     dword ptr [esp+30h+var_18], eax
4500C3  fstp    [esp+30h+var_10]
4500C7  call    sub_44DEF0
4500CC  mov     ecx, eax
4500CE  mov     eax, dword ptr [esp+30h+var_18]
4500D2  mov     edx, [eax+30h]
4500D5  sub     edx, [eax+34h]
4500D8  add     ecx, edx
4500DA  mov     dword ptr [esp+30h+var_18], ecx
4500DE  fild    dword ptr [esp+30h+var_18]
4500E2  fmul    ds:dbl_4EC960
4500E8  fdiv    ds:dbl_4EC978
4500EE  fst     [esp+30h+var_18]
4500F2  call    __CIsin
4500F7  fstp    [esp+30h+var_28]
4500FB  fld     [esp+30h+var_18]
4500FF  call    __CIcos
450104  fld     st
450106  fld     [esp+30h+var_20]
45010A  fmul    st(1), st
45010C  fld     [esp+30h+var_28]
450110  fld     st
450112  fld     [esp+30h+var_10]
450116  fmul    st(1), st
450118  fxch    st(4)
45011A  fsubrp  st(1), st
45011C  fld     st(4)
45011E  fmulp   st(4), st
450120  fld     st(1)
450122  fmulp   st(3), st
450124  fxch    st(3)
450126  faddp   st(2), st
450128  fld     st(3)
45012A  fmul    st, st(2)
45012C  fld     st(1)
45012E  fld     [esp+30h+var_8]
450132  fmul    st(1), st
450134  fxch    st(2)
450136  fsubrp  st(1), st
450138  fld     st(2)
45013A  fmulp   st(4), st
45013C  fld     st(5)
45013E  fmulp   st(2), st
450140  fxch    st(3)
450142  faddp   st(1), st
450144  fld     st(4)
450146  fmul    st, st(1)
450148  fld     st(2)
45014A  fmul    st, st(5)
45014C  fsubp   st(1), st
45014E  fxch    st(5)
450150  fmulp   st(4), st
450152  fmulp   st(1), st
450154  faddp   st(2), st
450156  fldz
450158  fcom    st(3)
45015A  fnstsw  ax
45015C  fld     ds:dbl_4E1EE8
450162  test    ah, 41h
450165  jp      short loc_45016B
450167  fadd    st(4), st
450169  jmp     short loc_45016D
45016B  fsub    st(4), st
45016D  fxch    st(4)
45016F  call    __ftol2_sse
450174  fcom    st(2)
450176  mov     [esi+50h], eax
450179  fnstsw  ax
45017B  fxch    st(2)
45017D  test    ah, 41h
450180  jp      short loc_450186
450182  fadd    st, st(3)
450184  jmp     short loc_450188
450186  fsub    st, st(3)
450188  call    __ftol2_sse
45018D  fcom    st(1)
45018F  mov     [esi+58h], eax
450192  fnstsw  ax
450194  fstp    st(1)
450196  test    ah, 1
450199  jnz     short loc_4501A7
45019B  faddp   st(1), st
45019D  call    __ftol2_sse
4501A2  mov     [esi+60h], eax
4501A5  jmp     short loc_4501FA
4501A7  fsubrp  st(1), st
4501A9  call    __ftol2_sse
4501AE  mov     [esi+60h], eax
4501B1  jmp     short loc_4501FA
4501B3  mov     edi, [ebx+1Ch]
4501B6  add     edi, edi
4501B8  call    sub_44DEF0
4501BD  mov     ecx, [ebx+18h]
4501C0  sub     ecx, [ebx+1Ch]
4501C3  add     eax, ecx
4501C5  mov     [esi+50h], eax
4501C8  mov     edi, [ebx+24h]
4501CB  add     edi, edi
4501CD  call    sub_44DEF0
4501D2  mov     edx, [ebx+20h]
4501D5  sub     edx, [ebx+24h]
4501D8  add     eax, edx
4501DA  mov     [esi+58h], eax
4501DD  mov     edi, [ebx+2Ch]
4501E0  add     edi, edi
4501E2  call    sub_44DEF0
4501E7  mov     ecx, [ebx+28h]
4501EA  sub     ecx, [ebx+2Ch]
4501ED  push    0
4501EF  add     eax, ecx
4501F1  push    esi
4501F2  mov     [esi+60h], eax
4501F5  call    sub_450250
4501FA  mov     edi, [ebx+34h]
4501FD  call    sub_44DEF0
450202  add     eax, [ebx+30h]
450205  xor     ecx, ecx
450207  mov     [esi+64h], eax
45020A  mov     [esi+68h], ecx
45020D  cmp     eax, ecx
45020F  jnz     short loc_450216
450211  mov     eax, 1
450216  pop     edi
450217  mov     [esi+64h], eax
45021A  pop     ebx
45021B  mov     esp, ebp
45021D  pop     ebp
45021E  retn

;==== 450220 sub_450220
450220  push    esi
450221  mov     esi, eax
450223  mov     eax, ecx
450225  xor     edx, edx
450227  call    sub_44DF20
45022C  cmp     eax, esi
45022E  jnz     short loc_450249
450230  imul    esi, 8Ch
450236  add     esi, offset dword_5121E8
45023C  push    ecx
45023D  call    sub_44E030
450242  mov     eax, 1
450247  pop     esi
450248  retn
450249  mov     eax, edx
45024B  pop     esi
45024C  retn

;==== 450250 sub_450250
450250  push    ebp
450251  mov     ebp, esp
450253  and     esp, 0FFFFFFF8h
450256  sub     esp, 2Ch
450259  push    ebx
45025A  mov     ebx, [ebp+arg_0]
45025D  push    esi
45025E  mov     esi, [ebx+40h]
450261  cmp     dword ptr [esi+0Ch], 0
450265  push    edi
450266  jz      loc_4503CC
45026C  fild    dword ptr [ebx+50h]
45026F  fst     [esp+38h+var_20]
450273  fild    dword ptr [ebx+58h]
450276  fst     [esp+38h+var_18]
45027A  fild    dword ptr [ebx+60h]
45027D  fst     [esp+38h+var_10]
450281  fld     st(1)
450283  fmulp   st(2), st
450285  fld     st(2)
450287  fmulp   st(3), st
450289  fxch    st(1)
45028B  faddp   st(2), st
45028D  fmul    st, st
45028F  faddp   st(1), st
450291  call    __CIsqrt
450296  fst     [esp+38h+var_28]
45029A  fld     st
45029C  fldz
45029E  fucompp
4502A0  fnstsw  ax
4502A2  fld1
4502A4  test    ah, 44h
4502A7  jp      short loc_4502B3
4502A9  fstp    st(1)
4502AB  fld     st
4502AD  fst     [esp+38h+var_28]
4502B1  fxch    st(1)
4502B3  fld     [esp+38h+var_18]
4502B7  mov     eax, 10000h
4502BC  fabs
4502BE  sub     eax, [esi+1Ch]
4502C1  fdiv    st, st(2)
4502C3  mov     ecx, 10000h
4502C8  mov     [esp+38h+var_2C], eax
4502CC  sub     ecx, [esi+18h]
4502CF  mov     edi, [esi+14h]
4502D2  mov     edx, 10000h
4502D7  sub     edx, [esi+20h]
4502DA  fild    [esp+38h+var_2C]
4502DE  mov     [esp+38h+var_2C], ecx
4502E2  fmulp   st(1), st
4502E4  fld     ds:dbl_4EC968
4502EA  fmul    st(1), st
4502EC  fld     st(2)
4502EE  fsubrp  st(2), st
4502F0  fld     [esp+38h+var_20]
4502F4  fabs
4502F6  fdiv    st, st(4)
4502F8  fild    [esp+38h+var_2C]
4502FC  mov     [esp+38h+var_2C], edx
450300  fmulp   st(1), st
450302  fmul    st, st(1)
450304  fsubr   st, st(3)
450306  fmulp   st(2), st
450308  fld     [esp+38h+var_10]
45030C  fabs
45030E  fdivrp  st(4), st
450310  fild    [esp+38h+var_2C]
450314  fmulp   st(4), st
450316  fmulp   st(3), st
450318  fxch    st(1)
45031A  fsubrp  st(2), st
45031C  fmulp   st(1), st
45031E  fstp    [esp+38h+var_8]
450322  call    sub_44DEF0
450327  mov     ecx, [ebx+40h]
45032A  add     eax, [ecx+10h]
45032D  mov     [esp+38h+var_2C], eax
450331  fild    [esp+38h+var_2C]
450335  fld     [esp+38h+var_8]
450339  fmul    st, st(1)
45033B  fdiv    [esp+38h+var_28]
45033F  fld     [esp+38h+var_20]
450343  fmul    st, st(1)
450345  call    __ftol2_sse
45034A  fld     [esp+38h+var_18]
45034E  fmul    st, st(1)
450350  mov     [ebx+50h], eax
450353  call    __ftol2_sse
450358  fmul    [esp+38h+var_10]
45035C  mov     [ebx+58h], eax
45035F  call    __ftol2_sse
450364  cmp     [ebp+arg_4], 0
450368  mov     [ebx+60h], eax
45036B  jz      short loc_4503CA
45036D  mov     eax, [ebx+40h]
450370  cmp     dword ptr [eax+24h], 0
450374  jz      short loc_4503CA
450376  mov     eax, [eax+10h]
450379  mov     [esp+38h+var_2C], eax
45037D  test    eax, eax
45037F  jle     short loc_4503CA
450381  mov     edx, [ebx+44h]
450384  fild    dword ptr [ebx+44h]
450387  test    edx, edx
450389  jns     short loc_450391
45038B  fadd    ds:dbl_4EC908
450391  fimul   [esp+38h+var_2C]
450395  fnstcw  word ptr [esp+38h+var_2C]
450399  movzx   eax, word ptr [esp+38h+var_2C]
45039E  or      eax, 0C00h
4503A3  fdivrp  st(1), st
4503A5  mov     dword ptr [esp+38h+var_28], eax
4503A9  fldcw   word ptr [esp+38h+var_28]
4503AD  fistp   [esp+38h+var_8]
4503B1  mov     eax, dword ptr [esp+38h+var_8]
4503B5  mov     [ebx+44h], eax
4503B8  mov     eax, 1
4503BD  fldcw   word ptr [esp+38h+var_2C]
4503C1  pop     edi
4503C2  pop     esi
4503C3  pop     ebx
4503C4  mov     esp, ebp
4503C6  pop     ebp
4503C7  retn    8
4503CA  fstp    st
4503CC  pop     edi
4503CD  pop     esi
4503CE  mov     eax, 1
4503D3  pop     ebx
4503D4  mov     esp, ebp
4503D6  pop     ebp
4503D7  retn    8

;==== 4503E0 sub_4503E0
4503E0  push    ebp
4503E1  mov     ebp, esp
4503E3  push    0FFFFFFFFh
4503E5  push    offset SEH_44F8C0
4503EA  mov     eax, large fs:0
4503F0  push    eax
4503F1  push    ebx
4503F2  push    esi
4503F3  push    edi
4503F4  mov     eax, dword_4FB734
4503F9  xor     eax, ebp
4503FB  push    eax
4503FC  lea     eax, [ebp+var_C]
4503FF  mov     large fs:0, eax
450405  mov     ebx, [ebp+arg_4]
450408  mov     esi, [ebp+arg_0]
45040B  mov     eax, ebx
45040D  imul    eax, 8Ch
450413  add     eax, offset dword_5144E8
450418  push    eax
450419  mov     ecx, ebx
45041B  xor     eax, eax
45041D  call    sub_44DC40
450422  mov     [ebp+var_4], 0
450429  mov     dword ptr [esi], offset ??_7DCPSnow@@6B@; const DCPSnow::`vftable'
45042F  cmp     ebx, 40h ; '@'
450432  jnb     loc_4504CD
450438  imul    ebx, 2Ch ; ','
45043B  add     ebx, offset dword_5167E8
450441  cmp     dword ptr [ebx], 0
450444  jz      loc_4504CD
45044A  mov     edi, [ebx+4]
45044D  add     edi, edi
45044F  call    sub_44DEF0
450454  sub     eax, [ebx+4]
450457  mov     [esi+10h], eax
45045A  mov     ecx, [ebx+8]
45045D  mov     [esi+14h], ecx
450460  mov     edi, [ebx+0Ch]
450463  add     edi, edi
450465  call    sub_44DEF0
45046A  sub     eax, [ebx+0Ch]
45046D  mov     [esi+18h], eax
450470  mov     edi, [ebx+14h]
450473  add     edi, edi
450475  call    sub_44DEF0
45047A  mov     edx, [ebx+10h]
45047D  sub     edx, [ebx+14h]
450480  add     eax, edx
450482  mov     [esi+40h], eax
450485  mov     edi, [ebx+1Ch]
450488  add     edi, edi
45048A  call    sub_44DEF0
45048F  mov     ecx, [ebx+18h]
450492  sub     ecx, [ebx+1Ch]
450495  add     eax, ecx
450497  mov     [esi+44h], eax
45049A  mov     edi, [ebx+24h]
45049D  add     edi, edi
45049F  call    sub_44DEF0
4504A4  mov     edx, [ebx+20h]
4504A7  sub     edx, [ebx+24h]
4504AA  mov     ecx, 20h ; ' '
4504AF  add     eax, edx
4504B1  mov     [esi+48h], eax
4504B4  mov     eax, [ebx+28h]
4504B7  mov     [esi+1Ch], eax
4504BA  mov     eax, esi
4504BC  call    sub_4339A0
4504C1  call    sub_44E000
4504C6  mov     dword ptr [esi+0Ch], 1
4504CD  mov     [ebp+var_4], 0FFFFFFFFh
4504D4  mov     eax, esi
4504D6  mov     ecx, [ebp+var_C]
4504D9  mov     large fs:0, ecx
4504E0  pop     ecx
4504E1  pop     edi
4504E2  pop     esi
4504E3  pop     ebx
4504E4  mov     esp, ebp
4504E6  pop     ebp
4504E7  retn    8
4D8FF0  mov     ecx, [ebp+arg_0]
4D8FF3  jmp     sub_44DCC0
4D8FF8  mov     edx, [esp-4+arg_4]
4D8FFC  lea     eax, [edx+0Ch]
4D8FFF  mov     ecx, [edx-10h]
4D9002  xor     ecx, eax
4D9004  call    sub_4ABDD5
4D9009  mov     eax, offset stru_4F6848
4D900E  jmp     ___CxxFrameHandler3

;==== 4504F0 sub_4504F0
4504F0  push    ebp
4504F1  mov     ebp, esp
4504F3  push    esi
4504F4  mov     esi, ecx
4504F6  call    sub_450520
4504FB  test    [ebp+arg_0], 1
4504FF  jz      short loc_45050A
450501  push    esi; void *
450502  call    ??3@YAXPAX@Z; operator delete(void *)
450507  add     esp, 4
45050A  mov     eax, esi
45050C  pop     esi
45050D  pop     ebp
45050E  retn    4

;==== 450520 sub_450520
450520  push    ebp
450521  mov     ebp, esp
450523  push    0FFFFFFFFh
450525  push    offset SEH_450520
45052A  mov     eax, large fs:0
450530  push    eax
450531  push    ecx
450532  mov     eax, dword_4FB734
450537  xor     eax, ebp
450539  push    eax
45053A  lea     eax, [ebp+var_C]
45053D  mov     large fs:0, eax
450543  mov     [ebp+var_10], ecx
450546  mov     dword ptr [ecx], offset ??_7DCPSnow@@6B@; const DCPSnow::`vftable'
45054C  mov     [ebp+var_4], 0FFFFFFFFh
450553  call    sub_44DCC0
450558  mov     ecx, [ebp+var_C]
45055B  mov     large fs:0, ecx
450562  pop     ecx
450563  mov     esp, ebp
450565  pop     ebp
450566  retn
4D8FC0  mov     ecx, [ebp+var_10]
4D8FC3  jmp     sub_44DCC0
4D8FC8  mov     edx, [esp-4+arg_4]
4D8FCC  lea     eax, [edx+0Ch]
4D8FCF  mov     ecx, [edx-8]
4D8FD2  xor     ecx, eax
4D8FD4  call    sub_4ABDD5
4D8FD9  mov     eax, offset stru_4F681C
4D8FDE  jmp     ___CxxFrameHandler3

;==== 4506D0 sub_4506D0
4506D0  push    ebp
4506D1  mov     ebp, esp
4506D3  mov     edx, [ebp+arg_0]
4506D6  push    esi
4506D7  xor     esi, esi
4506D9  cmp     edx, 1Fh
4506DC  ja      short loc_45072B
4506DE  push    edi
4506DF  mov     eax, ecx
4506E1  call    sub_44DF20
4506E6  mov     edi, eax
4506E8  imul    edi, 23h ; '#'
4506EB  mov     eax, ecx
4506ED  add     edi, edx
4506EF  call    sub_44E010
4506F4  mov     edi, dword_5144EC[edi*4]
4506FB  lea     eax, [eax+eax*2]
4506FE  cmp     [edi+eax*8], esi
450701  pop     edi
450702  jz      short loc_45072B
450704  mov     eax, ecx
450706  call    sub_44DF20
45070B  mov     esi, eax
45070D  imul    esi, 23h ; '#'
450710  mov     eax, ecx
450712  add     esi, edx
450714  call    sub_44E010
450719  mov     edx, dword_5144EC[esi*4]
450720  lea     ecx, [eax+eax*2]
450723  lea     eax, [edx+ecx*8]
450726  pop     esi
450727  pop     ebp
450728  retn    4
45072B  mov     eax, esi
45072D  pop     esi
45072E  pop     ebp
45072F  retn    4

;==== 450740 sub_450740
450740  call    sub_44DFA0
450745  mov     eax, [ecx+0Ch]
450748  test    eax, eax
45074A  jz      short locret_45075E
45074C  mov     edx, [ecx+40h]
45074F  add     [ecx+10h], edx
450752  mov     edx, [ecx+44h]
450755  add     [ecx+14h], edx
450758  mov     edx, [ecx+48h]
45075B  add     [ecx+18h], edx
45075E  retn

;==== 454260 sub_454260
454260  mov     eax, [eax+4]
454263  retn

;==== 4622E0 sub_4622E0
4622E0  xor     eax, eax
4622E2  jmp     loc_4622B0
4622B0  cmp     dword_566714, 0
4622B7  jz      short loc_4622C0
4622B9  or      dword_506BD0, eax
4622BF  retn
4622C0  mov     dword_566714, 1
4622CA  mov     dword_506BD0, eax
4622CF  retn

;==== 48BC40 sub_48BC40
48BC40  push    ebp
48BC41  mov     ebp, esp
48BC43  push    esi
48BC44  call    sub_4C1B50
48BC49  mov     esi, eax
48BC4B  mov     eax, [ebp+arg_0]
48BC4E  call    sub_4454A0
48BC53  xor     eax, eax
48BC55  pop     esi
48BC56  pop     ebp
48BC57  retn

;==== 491610 sub_491610
491610  jmp     sub_441470

;==== 491650 sub_491650
491650  push    ebp
491651  mov     ebp, esp
491653  sub     esp, 0Ch
491656  push    ebx
491657  push    esi
491658  call    sub_499290
49165D  mov     [ebp+var_4], eax
491660  mov     eax, dword_566760
491665  call    sub_443530
49166A  mov     esi, dword_566998
491670  mov     [ebp+var_C], eax
491673  mov     [ebp+var_8], 0
49167A  mov     ebx, offset dword_56698C
49167F  test    esi, esi
491681  jz      loc_49171C
491687  push    edi
491688  mov     eax, [ebp+var_4]
49168B  cmp     [esi+8], eax
49168E  ja      short loc_491704
491690  mov     ecx, dword_566760
491696  mov     eax, [esi]
491698  push    ecx
491699  call    sub_441600
49169E  mov     edi, eax
4916A0  test    edi, edi
4916A2  jz      short loc_4916F3
4916A4  mov     edx, [edi]
4916A6  mov     eax, [edx+8]
4916A9  mov     ecx, edi
4916AB  call    eax
4916AD  test    eax, eax
4916AF  jz      short loc_4916CD
4916B1  mov     edx, [edi]
4916B3  mov     eax, [edx+1Ch]
4916B6  mov     ecx, edi
4916B8  call    eax
4916BA  cmp     [ebp+var_C], eax
4916BD  ja      short loc_4916CD
4916BF  mov     eax, [esi]
4916C1  call    sub_491840
4916C6  mov     [ebp+var_8], 1
4916CD  cmp     dword ptr [esi+8], 0
4916D1  jnz     short loc_4916D9
4916D3  mov     ecx, [ebp+var_4]
4916D6  mov     [esi+8], ecx
4916D9  mov     ecx, [esi+8]
4916DC  mov     eax, [ebp+var_4]
4916DF  mov     edi, [esi+4]
4916E2  sub     eax, ecx
4916E4  xor     edx, edx
4916E6  div     edi
4916E8  inc     eax
4916E9  imul    eax, edi
4916EC  add     eax, ecx
4916EE  mov     [esi+8], eax
4916F1  jmp     short loc_491704
4916F3  mov     edx, [esi+0Ch]
4916F6  push    esi; void *
4916F7  mov     [ebx+0Ch], edx
4916FA  call    ??3@YAXPAX@Z; operator delete(void *)
4916FF  add     esp, 4
491702  mov     esi, ebx
491704  mov     ebx, esi
491706  mov     esi, [esi+0Ch]
491709  test    esi, esi
49170B  jnz     loc_491688
491711  pop     edi
491712  cmp     [ebp+var_8], esi
491715  jz      short loc_49171C
491717  call    sub_4622E0
49171C  pop     esi
49171D  pop     ebx
49171E  mov     esp, ebp
491720  pop     ebp
491721  retn

;==== 491840 sub_491840
491840  push    eax
491841  call    sub_441840
491846  retn

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

;==== 4ABDD5 sub_4ABDD5
4ABDD5  cmp     ecx, dword_4FB734
4ABDDB  jnz     short loc_4ABDDF
4ABDDD  rep retn
4ABDDF  jmp     ___report_gsfailure

;==== 4C1B50 sub_4C1B50
4C1B50  mov     eax, 1
4C1B55  retn

