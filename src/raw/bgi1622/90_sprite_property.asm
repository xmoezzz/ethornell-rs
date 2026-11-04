
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

;==== 407B20 sub_407B20
407B20  mov     eax, ds:dword_4E41A0[eax*4]
407B27  retn

;==== 407F10 sub_407F10
407F10  xor     eax, eax
407F12  push    edi
407F13  test    ecx, ecx
407F15  js      loc_407F9D
407F1B  cmp     ecx, [edx+0Ch]
407F1E  jge     short loc_407F9D
407F20  mov     edx, [edx+8]
407F23  lea     ecx, [ecx+ecx*8]
407F26  cmp     [edx+ecx*8], eax
407F29  lea     edx, [edx+ecx*8]
407F2C  jz      short loc_407F9D
407F2E  mov     ecx, [edx+20h]
407F31  cmp     ecx, 0FFFFFFFFh
407F34  jz      short loc_407F75
407F36  call    sub_408E40
407F3B  test    eax, eax
407F3D  jz      short loc_407F75
407F3F  mov     ecx, [edx+4]
407F42  mov     [esi], ecx
407F44  mov     ecx, [edx+8]
407F47  mov     [esi+4], ecx
407F4A  mov     ecx, [edx+0Ch]
407F4D  mov     [esi+8], ecx
407F50  mov     ecx, [edx+10h]
407F53  mov     [esi+0Ch], ecx
407F56  mov     ecx, [edx+14h]
407F59  mov     [esi+10h], ecx
407F5C  mov     edx, [edx+18h]
407F5F  lea     ecx, [esi+4]
407F62  push    ecx
407F63  push    esi
407F64  mov     edi, eax
407F66  mov     [esi+14h], edx
407F69  call    sub_493E40
407F6E  mov     eax, 1
407F73  pop     edi
407F74  retn
407F75  mov     eax, [edx+4]
407F78  mov     [esi], eax
407F7A  mov     ecx, [edx+8]
407F7D  mov     [esi+4], ecx
407F80  mov     eax, [edx+0Ch]
407F83  mov     [esi+8], eax
407F86  mov     ecx, [edx+10h]
407F89  mov     [esi+0Ch], ecx
407F8C  mov     eax, [edx+14h]
407F8F  mov     [esi+10h], eax
407F92  mov     ecx, [edx+18h]
407F95  mov     [esi+14h], ecx
407F98  mov     eax, 1
407F9D  pop     edi
407F9E  retn

;==== 4082F0 sub_4082F0
4082F0  or      eax, 0FFFFFFFFh
4082F3  test    ecx, ecx
4082F5  js      short locret_40830E
4082F7  cmp     ecx, [edx+0Ch]
4082FA  jge     short locret_40830E
4082FC  mov     edx, [edx+8]
4082FF  lea     ecx, [ecx+ecx*8]
408302  cmp     dword ptr [edx+ecx*8], 0
408306  lea     ecx, [edx+ecx*8]
408309  jz      short locret_40830E
40830B  mov     eax, [ecx+1Ch]
40830E  retn

;==== 408E20 sub_408E20
408E20  mov     eax, dword_5076D8
408E25  test    eax, eax
408E27  jz      short locret_408E3B
408E29  lea     esp, [esp+0]
408E30  cmp     ecx, [eax]
408E32  jz      short locret_408E3B
408E34  mov     eax, [eax+0Ch]
408E37  test    eax, eax
408E39  jnz     short loc_408E30
408E3B  retn

;==== 408E40 sub_408E40
408E40  call    sub_408E20
408E45  test    eax, eax
408E47  jz      short loc_408E4D
408E49  mov     eax, [eax+4]
408E4C  retn
408E4D  xor     eax, eax
408E4F  retn

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

;==== 409FE0 sub_409FE0
409FE0  push    ebp
409FE1  mov     ebp, esp
409FE3  cmp     eax, 8
409FE6  jnb     short loc_40A048
409FE8  lea     eax, [eax+eax*2+3]
409FEC  cmp     dword ptr [ecx+eax*8], 0
409FF0  lea     ecx, [ecx+eax*8]
409FF3  jz      short loc_40A03F
409FF5  mov     edx, [ebp+arg_4]
409FF8  lea     eax, ds:0[edx*4]
409FFF  mov     edx, [ecx+4]
40A002  sub     edx, eax
40A004  cmp     edx, [ecx+8]
40A007  jge     short loc_40A020
40A009  push    esi
40A00A  mov     esi, [ecx+0Ch]
40A00D  push    edi
40A00E  mov     edi, [ecx+8]
40A011  mov     eax, edi
40A013  sub     eax, edx
40A015  xor     edx, edx
40A017  div     esi
40A019  sub     esi, edx
40A01B  lea     edx, [edi+esi]
40A01E  pop     edi
40A01F  pop     esi
40A020  mov     ecx, [ecx+10h]
40A023  sub     ecx, edx
40A025  mov     edx, [ebp+arg_8]
40A028  lea     eax, ds:0[edx*4]
40A02F  mov     edx, [ebp+arg_0]
40A032  cmp     ecx, eax
40A034  sbb     ecx, ecx
40A036  inc     ecx
40A037  mov     [edx], ecx
40A039  xor     eax, eax
40A03B  pop     ebp
40A03C  retn    0Ch
40A03F  mov     eax, 11h
40A044  pop     ebp
40A045  retn    0Ch
40A048  mov     eax, 10h
40A04D  pop     ebp
40A04E  retn    0Ch

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

;==== 40EAE0 sub_40EAE0
40EAE0  test    eax, eax
40EAE2  jns     short loc_40EAE8
40EAE4  add     eax, 4
40EAE7  retn
40EAE8  cmp     eax, 4
40EAEB  jl      short locret_40EAF0
40EAED  add     eax, 0FFFFFFFCh
40EAF0  retn

;==== 40EB00 sub_40EB00
40EB00  fld     qword ptr [ecx]
40EB02  fld     qword ptr [eax]
40EB04  fstp    qword ptr [ecx]
40EB06  fstp    qword ptr [eax]
40EB08  retn

;==== 40EB10 sub_40EB10
40EB10  push    ebp
40EB11  mov     ebp, esp
40EB13  sub     esp, 13Ch
40EB19  fld     ds:dbl_4EC9F0
40EB1F  push    ebx
40EB20  push    edi
40EB21  sub     esp, 10h
40EB24  fstp    [esp+154h+X]; double
40EB28  mov     ebx, esi
40EB2A  fld     dword ptr [esi+4]
40EB2D  fstp    [esp+154h+var_154]; double
40EB30  call    sub_4010B0
40EB35  fstp    [ebp+var_7C]
40EB38  fld     ds:dbl_4EC9F0
40EB3E  fstp    [esp+154h+X]; double
40EB42  fld     dword ptr [esi+4]
40EB45  fstp    [esp+154h+var_154]; double
40EB48  call    sub_4010B0
40EB4D  fstp    [ebp+var_3C]
40EB50  fld     ds:dbl_4EC9F0
40EB56  fstp    [esp+154h+X]; double
40EB5A  fld     dword ptr [esi]
40EB5C  fstp    [esp+154h+var_154]; double
40EB5F  call    sub_4010B0
40EB64  fstp    [ebp+var_64]
40EB67  fld     ds:dbl_4EC9F0
40EB6D  fstp    [esp+154h+X]; double
40EB71  fld     dword ptr [esi]
40EB73  fstp    [esp+154h+var_154]; double
40EB76  call    sub_4010B0
40EB7B  add     esp, 10h
40EB7E  fstp    [ebp+var_74]
40EB81  xor     edi, edi
40EB83  mov     [ebp+var_10], 1
40EB8A  jmp     short loc_40EB92
40EB90  fstp    st
40EB92  fld     ds:dbl_4EC9F0
40EB98  sub     esp, 10h
40EB9B  fstp    [esp+154h+X]; double
40EB9F  add     ebx, 14h
40EBA2  fld     dword ptr [ebx+4]
40EBA5  fstp    [esp+154h+var_154]; double
40EBA8  call    sub_4010B0
40EBAD  fcomp   [ebp+var_7C]
40EBB0  add     esp, 10h
40EBB3  fnstsw  ax
40EBB5  test    ah, 5
40EBB8  jp      short loc_40EBDB
40EBBA  fld     ds:dbl_4EC9F0
40EBC0  sub     esp, 10h
40EBC3  fstp    [esp+154h+X]; double
40EBC7  fld     dword ptr [ebx+4]
40EBCA  fstp    [esp+154h+var_154]; double
40EBCD  call    sub_4010B0
40EBD2  mov     edi, [ebp+var_10]
40EBD5  fstp    [ebp+var_7C]
40EBD8  add     esp, 10h
40EBDB  fld     ds:dbl_4EC9F0
40EBE1  sub     esp, 10h
40EBE4  fstp    [esp+154h+X]; double
40EBE8  fld     dword ptr [ebx+4]
40EBEB  fstp    [esp+154h+var_154]; double
40EBEE  call    sub_4010B0
40EBF3  fcomp   [ebp+var_3C]
40EBF6  add     esp, 10h
40EBF9  fnstsw  ax
40EBFB  test    ah, 41h
40EBFE  jnz     short loc_40EC1E
40EC00  fld     ds:dbl_4EC9F0
40EC06  sub     esp, 10h
40EC09  fstp    [esp+154h+X]; double
40EC0D  fld     dword ptr [ebx+4]
40EC10  fstp    [esp+154h+var_154]; double
40EC13  call    sub_4010B0
40EC18  fstp    [ebp+var_3C]
40EC1B  add     esp, 10h
40EC1E  fld     ds:dbl_4EC9F0
40EC24  sub     esp, 10h
40EC27  fstp    [esp+154h+X]; double
40EC2B  fld     dword ptr [ebx]
40EC2D  fstp    [esp+154h+var_154]; double
40EC30  call    sub_4010B0
40EC35  fcomp   [ebp+var_64]
40EC38  add     esp, 10h
40EC3B  fnstsw  ax
40EC3D  test    ah, 5
40EC40  jp      short loc_40EC5F
40EC42  fld     ds:dbl_4EC9F0
40EC48  sub     esp, 10h
40EC4B  fstp    [esp+154h+X]; double
40EC4F  fld     dword ptr [ebx]
40EC51  fstp    [esp+154h+var_154]; double
40EC54  call    sub_4010B0
40EC59  fstp    [ebp+var_64]
40EC5C  add     esp, 10h
40EC5F  fld     ds:dbl_4EC9F0
40EC65  sub     esp, 10h
40EC68  fstp    [esp+154h+X]; double
40EC6C  fld     dword ptr [ebx]
40EC6E  fstp    [esp+154h+var_154]; double
40EC71  call    sub_4010B0
40EC76  fld     [ebp+var_74]
40EC79  add     esp, 10h
40EC7C  fcom    st(1)
40EC7E  fnstsw  ax
40EC80  fstp    st(1)
40EC82  test    ah, 5
40EC85  jp      short loc_40ECA6
40EC87  fstp    st
40EC89  sub     esp, 10h
40EC8C  fld     ds:dbl_4EC9F0
40EC92  fstp    [esp+154h+X]; double
40EC96  fld     dword ptr [ebx]
40EC98  fstp    [esp+154h+var_154]; double
40EC9B  call    sub_4010B0
40ECA0  fst     [ebp+var_74]
40ECA3  add     esp, 10h
40ECA6  mov     eax, [ebp+var_10]
40ECA9  inc     eax
40ECAA  mov     [ebp+var_10], eax
40ECAD  cmp     eax, 4
40ECB0  jl      loc_40EB90
40ECB6  fld     [ebp+var_7C]
40ECB9  fld     st
40ECBB  fld     [ebp+var_3C]
40ECBE  fucompp
40ECC0  fnstsw  ax
40ECC2  test    ah, 44h
40ECC5  jnp     loc_40F6F1
40ECCB  fld     [ebp+var_64]
40ECCE  fucomp  st(2)
40ECD0  fnstsw  ax
40ECD2  fstp    st(1)
40ECD4  test    ah, 44h
40ECD7  jnp     loc_40F6F3
40ECDD  sub     esp, 8
40ECE0  fstp    [esp+14Ch+X]; X
40ECE3  call    _floor
40ECE8  call    __ftol2_sse
40ECED  fld     [ebp+var_3C]
40ECF0  mov     ebx, eax
40ECF2  fstp    [esp+14Ch+X]; X
40ECF5  mov     [ebp+var_10], ebx
40ECF8  call    _ceil
40ECFD  add     esp, 8
40ED00  call    __ftol2_sse
40ED05  mov     [ebp+var_18], eax
40ED08  test    eax, eax
40ED0A  js      loc_40F6F5
40ED10  fld     [ebp+var_64]
40ED13  sub     esp, 8
40ED16  fstp    [esp+14Ch+X]; X
40ED19  call    _floor
40ED1E  call    __ftol2_sse
40ED23  fld     [ebp+var_74]
40ED26  fstp    [esp+14Ch+X]; X
40ED29  mov     [ebp+var_E8], eax
40ED2F  call    _ceil
40ED34  add     esp, 8
40ED37  call    __ftol2_sse
40ED3C  fldz
40ED3E  fst     [ebp+var_84]
40ED44  mov     [ebp+var_98], edi
40ED4A  fst     [ebp+var_104]
40ED50  mov     [ebp+var_88], edi
40ED56  fst     [ebp+var_C4]
40ED5C  mov     edx, [ebp+arg_10]
40ED5F  fst     [ebp+var_F4]
40ED65  lea     edi, [edi+edi*4]
40ED68  fst     [ebp+var_6C]
40ED6B  add     edi, edi
40ED6D  fst     [ebp+var_10C]
40ED73  add     edi, edi
40ED75  fst     [ebp+var_B4]
40ED7B  xor     ecx, ecx
40ED7D  fstp    [ebp+var_3C]
40ED80  test    ebx, ebx
40ED82  fld     dword ptr [edi+esi]
40ED85  sets    cl
40ED88  fstp    [ebp+var_4C]
40ED8B  mov     [ebp+var_F8], eax
40ED91  fld     dword ptr [edi+esi+8]
40ED95  mov     eax, [ebp+arg_8]
40ED98  fstp    [ebp+var_114]
40ED9E  mov     [ebp+var_FC], edi
40EDA4  fld     dword ptr [edi+esi+0Ch]
40EDA8  mov     [ebp+var_B8], 7FFFFFFFh
40EDB2  fstp    [ebp+var_DC]
40EDB8  mov     [ebp+var_C8], 80000000h
40EDC2  fld     dword ptr [edi+esi+10h]
40EDC6  fstp    [ebp+var_D4]
40EDCC  dec     ecx
40EDCD  fld     dword ptr [edi+esi]
40EDD0  and     ecx, ebx
40EDD2  fstp    [ebp+var_2C]
40EDD5  mov     [ebp+var_8], ecx
40EDD8  fld     dword ptr [edi+esi+8]
40EDDC  fstp    [ebp+var_E4]
40EDE2  fld     dword ptr [edi+esi+0Ch]
40EDE6  fstp    [ebp+var_64]
40EDE9  fld     dword ptr [edi+esi+10h]
40EDED  mov     [eax], ecx
40EDEF  fstp    [ebp+var_34]
40EDF2  mov     eax, [ebp+var_18]
40EDF5  cmp     eax, edx
40EDF7  jl      short loc_40EDFC
40EDF9  lea     eax, [edx-1]
40EDFC  sub     eax, ecx
40EDFE  mov     ecx, [ebp+arg_4]
40EE01  inc     eax
40EE02  mov     [ecx], eax
40EE04  xor     ecx, ecx
40EE06  mov     edx, 20h ; ' '
40EE0B  mul     edx
40EE0D  seto    cl
40EE10  neg     ecx
40EE12  or      ecx, eax
40EE14  push    ecx; unsigned int
40EE15  call    ??_U@YAPAXI@Z; operator new[](uint)
40EE1A  mov     ecx, [ebp+arg_0]
40EE1D  mov     [ecx], eax
40EE1F  mov     ecx, [ebp+var_8]
40EE22  add     esp, 4
40EE25  mov     ebx, eax
40EE27  mov     [ebp+var_14], ecx
40EE2A  cmp     ecx, [ebp+var_18]
40EE2D  jg      loc_40F6B6
40EE33  fld     [ebp+var_6C]
40EE36  lea     edx, [ecx+1]
40EE39  fld     [ebp+var_DC]
40EE3F  mov     [ebp+var_9C], edx
40EE45  fld     [ebp+var_E4]
40EE4B  fld     [ebp+var_D4]
40EE51  fld     [ebp+var_114]
40EE57  fld     [ebp+var_64]
40EE5A  jmp     short loc_40EE68
40EE5C  fxch    st(2)
40EE5E  fxch    st(5)
40EE60  fxch    st(1)
40EE62  fxch    st(4)
40EE64  fxch    st(3)
40EE66  fxch    st(2)
40EE68  cmp     ecx, [ebp+arg_10]
40EE6B  jge     loc_40F6E5
40EE71  fild    [ebp+var_9C]
40EE77  lea     edx, [edi+esi]
40EE7A  fst     [ebp+var_24]
40EE7D  fld     dword ptr [edx+4]
40EE80  fcomp   st(1)
40EE82  fnstsw  ax
40EE84  test    ah, 5
40EE87  jp      loc_40EFF6
40EE8D  cmp     ecx, [ebp+var_18]
40EE90  jge     loc_40EFF6
40EE96  fstp    st
40EE98  cmp     ecx, [ebp+var_10]
40EE9B  jz      short loc_40EED3
40EE9D  fld     dword ptr [edx+4]
40EEA0  fisub   [ebp+var_14]
40EEA3  fst     qword ptr [ebp-0Ch]
40EEA6  fmul    [ebp+var_84]
40EEAC  fadd    [ebp+var_4C]
40EEAF  fstp    [ebp+var_4C]
40EEB2  fld     qword ptr [ebp-0Ch]
40EEB5  fmul    [ebp+var_104]
40EEBB  faddp   st(2), st
40EEBD  fld     qword ptr [ebp-0Ch]
40EEC0  fmul    [ebp+var_C4]
40EEC6  faddp   st(5), st
40EEC8  fld     qword ptr [ebp-0Ch]
40EECB  fmul    [ebp+var_F4]
40EED1  faddp   st(3), st
40EED3  fld     dword ptr [edx+4]
40EED6  xor     eax, eax
40EED8  fstp    [ebp+var_AC]
40EEDE  jmp     short loc_40EEE3
40EEE0  mov     eax, [ebp+var_8]
40EEE3  test    eax, eax
40EEE5  jz      short loc_40EF03
40EEE7  fstp    st(1)
40EEE9  fstp    st(3)
40EEEB  fstp    st
40EEED  fld     dword ptr [edx]
40EEEF  fstp    [ebp+var_4C]
40EEF2  fld     dword ptr [edx+8]
40EEF5  fld     dword ptr [edx+10h]
40EEF8  fld     dword ptr [edx+0Ch]
40EEFB  fxch    st(4)
40EEFD  fxch    st(1)
40EEFF  fxch    st(2)
40EF01  fxch    st(1)
40EF03  mov     eax, [ebp+var_98]
40EF09  mov     ecx, eax
40EF0B  inc     eax
40EF0C  call    sub_40EAE0
40EF11  mov     edi, eax
40EF13  lea     eax, [edi+edi*4]
40EF16  fld     dword ptr [esi+eax*4+4]
40EF1A  lea     edx, [esi+eax*4]
40EF1D  fcomp   [ebp+var_AC]
40EF23  mov     [ebp+var_98], edi
40EF29  mov     [ebp+var_8], 1
40EF30  fnstsw  ax
40EF32  test    ah, 1
40EF35  jnz     short loc_40EF44
40EF37  fld     dword ptr [edx+4]
40EF3A  fcomp   [ebp+var_24]
40EF3D  fnstsw  ax
40EF3F  test    ah, 5
40EF42  jnp     short loc_40EEE0
40EF44  lea     edi, [edi+edi*4]
40EF47  add     edi, edi
40EF49  add     edi, edi
40EF4B  fld     dword ptr [edi+esi+4]
40EF4F  lea     ecx, [ecx+ecx*4]
40EF52  fsub    dword ptr [esi+ecx*4+4]
40EF56  lea     eax, [esi+ecx*4]
40EF59  mov     ecx, [ebp+var_14]
40EF5C  fstp    qword ptr [ebp-0Ch]
40EF5F  fld     dword ptr [edi+esi]
40EF62  fsub    dword ptr [eax]
40EF64  fdiv    qword ptr [ebp-0Ch]
40EF67  fstp    [ebp+var_84]
40EF6D  fld     dword ptr [edi+esi+8]
40EF71  fsub    dword ptr [eax+8]
40EF74  fdiv    qword ptr [ebp-0Ch]
40EF77  fstp    [ebp+var_104]
40EF7D  fld     dword ptr [edi+esi+0Ch]
40EF81  fsub    dword ptr [eax+0Ch]
40EF84  fdiv    qword ptr [ebp-0Ch]
40EF87  fstp    [ebp+var_C4]
40EF8D  fld     dword ptr [edi+esi+10h]
40EF91  fsub    dword ptr [eax+10h]
40EF94  fdiv    qword ptr [ebp-0Ch]
40EF97  fstp    [ebp+var_F4]
40EF9D  fld     dword ptr [eax+4]
40EFA0  fsubr   [ebp+var_24]
40EFA3  fsub    ds:dbl_4E2830
40EFA9  fst     qword ptr [ebp-0Ch]
40EFAC  fmul    [ebp+var_84]
40EFB2  fadd    [ebp+var_4C]
40EFB5  fstp    [ebp+var_4C]
40EFB8  fld     qword ptr [ebp-0Ch]
40EFBB  fmul    [ebp+var_104]
40EFC1  faddp   st(2), st
40EFC3  fxch    st(1)
40EFC5  fst     [ebp+var_114]
40EFCB  fld     qword ptr [ebp-0Ch]
40EFCE  fmul    [ebp+var_C4]
40EFD4  faddp   st(5), st
40EFD6  fxch    st(4)
40EFD8  fst     [ebp+var_DC]
40EFDE  fld     qword ptr [ebp-0Ch]
40EFE1  fmul    [ebp+var_F4]
40EFE7  faddp   st(3), st
40EFE9  fxch    st(2)
40EFEB  fst     [ebp+var_D4]
40EFF1  fld     [ebp+var_24]
40EFF4  jmp     short loc_40F000
40EFF6  fxch    st(2)
40EFF8  fxch    st(5)
40EFFA  fxch    st(3)
40EFFC  fxch    st(1)
40EFFE  fxch    st(2)
40F000  mov     edx, [ebp+var_FC]
40F006  fld     dword ptr [edx+esi+4]
40F00A  add     edx, esi
40F00C  fcomp   st(1)
40F00E  fnstsw  ax
40F010  test    ah, 5
40F013  jp      loc_40F179
40F019  cmp     ecx, [ebp+var_18]
40F01C  jge     loc_40F179
40F022  cmp     ecx, [ebp+var_10]
40F025  jz      short loc_40F066
40F027  fstp    st
40F029  xor     eax, eax
40F02B  fld     dword ptr [edx+4]
40F02E  fisub   [ebp+var_14]
40F031  fld     st
40F033  fmulp   st(7), st
40F035  fxch    st(6)
40F037  fadd    [ebp+var_2C]
40F03A  fstp    [ebp+var_2C]
40F03D  fld     st(5)
40F03F  fmul    [ebp+var_10C]
40F045  faddp   st(4), st
40F047  fld     st(5)
40F049  fmul    [ebp+var_B4]
40F04F  faddp   st(2), st
40F051  fxch    st(5)
40F053  fmul    [ebp+var_3C]
40F056  fadd    [ebp+var_34]
40F059  fstp    [ebp+var_34]
40F05C  fld     [ebp+var_24]
40F05F  fxch    st(5)
40F061  fld     dword ptr [edx+4]
40F064  jmp     short loc_40F072
40F066  fstp    st(6)
40F068  xor     eax, eax
40F06A  fld     dword ptr [edx+4]
40F06D  jmp     short loc_40F072
40F06F  mov     eax, [ebp+var_8]
40F072  test    eax, eax
40F074  jz      short loc_40F095
40F076  fstp    st(4)
40F078  fstp    st(1)
40F07A  fld     dword ptr [edx]
40F07C  fstp    [ebp+var_2C]
40F07F  fld     dword ptr [edx+8]
40F082  fld     dword ptr [edx+0Ch]
40F085  fld     dword ptr [edx+10h]
40F088  fstp    [ebp+var_34]
40F08B  fxch    st(1)
40F08D  fxch    st(4)
40F08F  fxch    st(1)
40F091  fxch    st(2)
40F093  fxch    st(1)
40F095  mov     eax, [ebp+var_88]
40F09B  mov     dword ptr [ebp+var_54+4], eax
40F09E  dec     eax
40F09F  call    sub_40EAE0
40F0A4  mov     ecx, eax
40F0A6  lea     eax, [ecx+ecx*4]
40F0A9  fld     dword ptr [esi+eax*4+4]
40F0AD  lea     edx, [esi+eax*4]
40F0B0  fcomp   st(1)
40F0B2  mov     [ebp+var_88], ecx
40F0B8  mov     [ebp+var_8], 1
40F0BF  fnstsw  ax
40F0C1  test    ah, 1
40F0C4  jnz     short loc_40F0D2
40F0C6  fld     dword ptr [edx+4]
40F0C9  fcomp   st(7)
40F0CB  fnstsw  ax
40F0CD  test    ah, 5
40F0D0  jnp     short loc_40F06F
40F0D2  fstp    st
40F0D4  lea     eax, [ecx+ecx*4]
40F0D7  mov     ecx, dword ptr [ebp+var_54+4]
40F0DA  fstp    st(5)
40F0DC  add     eax, eax
40F0DE  add     eax, eax
40F0E0  fld     dword ptr [eax+esi+4]
40F0E4  lea     ecx, [ecx+ecx*4]
40F0E7  fsub    dword ptr [esi+ecx*4+4]
40F0EB  lea     ecx, [esi+ecx*4]
40F0EE  mov     [ebp+var_FC], eax
40F0F4  fst     qword ptr [ebp-0Ch]
40F0F7  fld     dword ptr [eax+esi]
40F0FA  fsub    dword ptr [ecx]
40F0FC  fdivrp  st(1), st
40F0FE  fst     [ebp+var_6C]
40F101  fld     dword ptr [eax+esi+8]
40F105  fsub    dword ptr [ecx+8]
40F108  fdiv    qword ptr [ebp-0Ch]
40F10B  fstp    [ebp+var_10C]
40F111  fld     dword ptr [eax+esi+0Ch]
40F115  fsub    dword ptr [ecx+0Ch]
40F118  fdiv    qword ptr [ebp-0Ch]
40F11B  fstp    [ebp+var_B4]
40F121  fld     dword ptr [eax+esi+10h]
40F125  fsub    dword ptr [ecx+10h]
40F128  fdiv    qword ptr [ebp-0Ch]
40F12B  fstp    [ebp+var_3C]
40F12E  fld     dword ptr [ecx+4]
40F131  fsubr   [ebp+var_24]
40F134  mov     ecx, [ebp+var_14]
40F137  fsub    ds:dbl_4E2830
40F13D  fst     qword ptr [ebp-0Ch]
40F140  fmul    st, st(1)
40F142  fadd    [ebp+var_2C]
40F145  fstp    [ebp+var_2C]
40F148  fld     qword ptr [ebp-0Ch]
40F14B  fmul    [ebp+var_10C]
40F151  faddp   st(4), st
40F153  fxch    st(3)
40F155  fst     [ebp+var_E4]
40F15B  fld     qword ptr [ebp-0Ch]
40F15E  fmul    [ebp+var_B4]
40F164  faddp   st(2), st
40F166  fxch    st(1)
40F168  fst     [ebp+var_64]
40F16B  fld     qword ptr [ebp-0Ch]
40F16E  fmul    [ebp+var_3C]
40F171  fadd    [ebp+var_34]
40F174  fstp    [ebp+var_34]
40F177  jmp     short loc_40F181
40F179  fstp    st
40F17B  fxch    st(5)
40F17D  fxch    st(3)
40F17F  fxch    st(1)
40F181  test    ecx, ecx
40F183  js      loc_40F625
40F189  fld     [ebp+var_4C]
40F18C  fst     [ebp+var_54]
40F18F  fxch    st(5)
40F191  fst     [ebp+var_11C]
40F197  fxch    st(3)
40F199  fst     [ebp+var_74]
40F19C  fxch    st(6)
40F19E  fst     [ebp+var_7C]
40F1A1  fld     [ebp+var_2C]
40F1A4  fstp    [ebp+var_24]
40F1A7  fxch    st(2)
40F1A9  fst     [ebp+var_94]
40F1AF  fxch    st(1)
40F1B1  fst     [ebp+var_44]
40F1B4  fld     [ebp+var_34]
40F1B7  fstp    [ebp+var_5C]
40F1BA  cmp     ecx, [ebp+var_10]
40F1BD  jnz     loc_40F2B6
40F1C3  fld     [ebp+var_2C]
40F1C6  fucomp  st(6)
40F1C8  fnstsw  ax
40F1CA  fstp    st(5)
40F1CC  test    ah, 44h
40F1CF  jp      loc_40F2B8
40F1D5  fld     [ebp+var_2C]
40F1D8  fadd    st, st(4)
40F1DA  fstp    [ebp+var_A4]
40F1E0  fld     [ebp+var_4C]
40F1E3  fadd    [ebp+var_84]
40F1E9  fst     [ebp+var_AC]
40F1EF  fsubr   [ebp+var_A4]
40F1F5  fst     qword ptr [ebp-0Ch]
40F1F8  fldz
40F1FA  fucompp
40F1FC  fnstsw  ax
40F1FE  test    ah, 44h
40F201  jnp     loc_40F2B8
40F207  fstp    st(3)
40F209  fld     st(2)
40F20B  fadd    [ebp+var_10C]
40F211  fld     st(2)
40F213  fadd    [ebp+var_104]
40F219  fsubp   st(1), st
40F21B  fdiv    qword ptr [ebp-0Ch]
40F21E  fstp    [ebp+var_124]
40F224  fld     st(3)
40F226  fadd    [ebp+var_B4]
40F22C  fld     st(5)
40F22E  fadd    [ebp+var_C4]
40F234  fsubp   st(1), st
40F236  fdiv    qword ptr [ebp-0Ch]
40F239  fstp    [ebp+var_5C]
40F23C  fld     [ebp+var_34]
40F23F  fadd    [ebp+var_3C]
40F242  fld     st(1)
40F244  fadd    [ebp+var_F4]
40F24A  fsubp   st(1), st
40F24C  fdiv    qword ptr [ebp-0Ch]
40F24F  fstp    qword ptr [ebp-0Ch]
40F252  fld     [ebp+var_AC]
40F258  fstp    [ebp+var_54]
40F25B  fld     [ebp+var_124]
40F261  fld     st
40F263  fld     [ebp+var_84]
40F269  fmul    st(1), st
40F26B  fxch    st(1)
40F26D  fadd    st, st(4)
40F26F  fstp    [ebp+var_11C]
40F275  fld     [ebp+var_5C]
40F278  fmul    st, st(1)
40F27A  fadd    st, st(7)
40F27C  fstp    [ebp+var_74]
40F27F  fmul    qword ptr [ebp-0Ch]
40F282  fadd    st, st(2)
40F284  fstp    [ebp+var_7C]
40F287  fld     [ebp+var_A4]
40F28D  fstp    [ebp+var_24]
40F290  fld     [ebp+var_6C]
40F293  fmul    st(1), st
40F295  fxch    st(1)
40F297  fadd    st, st(4)
40F299  fstp    [ebp+var_94]
40F29F  fld     [ebp+var_5C]
40F2A2  fmul    st, st(1)
40F2A4  fadd    st, st(5)
40F2A6  fstp    [ebp+var_44]
40F2A9  fld     qword ptr [ebp-0Ch]
40F2AC  fmul    st, st(1)
40F2AE  fadd    [ebp+var_34]
40F2B1  fstp    [ebp+var_5C]
40F2B4  jmp     short loc_40F2BA
40F2B6  fstp    st(5)
40F2B8  fxch    st(3)
40F2BA  fld     [ebp+var_24]
40F2BD  fcomp   [ebp+var_54]
40F2C0  fnstsw  ax
40F2C2  test    ah, 5
40F2C5  jp      short loc_40F330
40F2C7  fstp    st(2)
40F2C9  lea     eax, [ebp+var_24]
40F2CC  fstp    st(4)
40F2CE  lea     ecx, [ebp+var_54]
40F2D1  fstp    st(3)
40F2D3  fstp    st
40F2D5  fstp    st
40F2D7  fstp    st
40F2D9  call    sub_40EB00
40F2DE  lea     eax, [ebp+var_94]
40F2E4  lea     ecx, [ebp+var_11C]
40F2EA  call    sub_40EB00
40F2EF  lea     eax, [ebp+var_44]
40F2F2  lea     ecx, [ebp+var_74]
40F2F5  call    sub_40EB00
40F2FA  lea     eax, [ebp+var_5C]
40F2FD  lea     ecx, [ebp+var_7C]
40F300  call    sub_40EB00
40F305  fld     [ebp+var_6C]
40F308  fld     [ebp+var_DC]
40F30E  mov     ecx, [ebp+var_14]
40F311  fld     [ebp+var_E4]
40F317  fld     [ebp+var_D4]
40F31D  fld     [ebp+var_114]
40F323  fld     [ebp+var_64]
40F326  fxch    st(4)
40F328  fxch    st(5)
40F32A  fxch    st(1)
40F32C  fxch    st(2)
40F32E  fxch    st(1)
40F330  fld     [ebp+var_24]
40F333  fsub    [ebp+var_54]
40F336  fldz
40F338  fcomp   st(1)
40F33A  fnstsw  ax
40F33C  test    ah, 5
40F33F  jp      loc_40F5FD
40F345  fstp    st(3)
40F347  fstp    st(5)
40F349  fstp    st
40F34B  fstp    st(1)
40F34D  fstp    st(1)
40F34F  fstp    st(1)
40F351  cmp     ecx, [ebp+var_10]
40F354  jz      short loc_40F387
40F356  fld     ds:dbl_4E2838
40F35C  fcomp   st(1)
40F35E  fnstsw  ax
40F360  test    ah, 41h
40F363  jnp     short loc_40F387
40F365  fstp    st
40F367  fld     [ebp+var_134]
40F36D  fstp    [ebp+var_134]
40F373  fld     [ebp+var_12C]
40F379  fstp    [ebp+var_124]
40F37F  fld     [ebp+var_13C]
40F385  jmp     short loc_40F3B1
40F387  fld     [ebp+var_94]
40F38D  fsub    [ebp+var_11C]
40F393  fdiv    st, st(1)
40F395  fstp    [ebp+var_134]
40F39B  fld     [ebp+var_44]
40F39E  fsub    [ebp+var_74]
40F3A1  fdiv    st, st(1)
40F3A3  fstp    [ebp+var_124]
40F3A9  fld     [ebp+var_5C]
40F3AC  fsub    [ebp+var_7C]
40F3AF  fdivrp  st(1), st
40F3B1  fstp    [ebp+var_AC]
40F3B7  sub     esp, 10h
40F3BA  fld     ds:dbl_4EC9F0
40F3C0  fstp    [esp+154h+X]; double
40F3C4  fld     [ebp+var_54]
40F3C7  fstp    [esp+154h+var_154]; double
40F3CA  call    sub_4010B0
40F3CF  fstp    [ebp+var_12C]
40F3D5  fld     ds:dbl_4EC9F0
40F3DB  fstp    [esp+154h+X]; double
40F3DF  fld     [ebp+var_24]
40F3E2  fstp    [esp+154h+var_154]; double
40F3E5  call    sub_4010B0
40F3EA  fstp    [ebp+var_A4]
40F3F0  add     esp, 8
40F3F3  fld     [ebp+var_12C]
40F3F9  fstp    [esp+14Ch+X]; X
40F3FC  call    _floor
40F401  call    __ftol2_sse
40F406  fld     [ebp+var_A4]
40F40C  fstp    [esp+14Ch+X]; X
40F40F  mov     dword ptr [ebp+var_94+4], eax
40F415  call    _ceil
40F41A  add     esp, 8
40F41D  call    __ftol2_sse
40F422  mov     edx, dword ptr [ebp+var_94+4]
40F428  mov     dword ptr [ebp+var_54+4], eax
40F42B  mov     [ebp+var_8], eax
40F42E  mov     eax, [ebp+var_18]
40F431  mov     dword ptr [ebp+var_24+4], edx
40F434  cmp     [ebp+var_14], eax
40F437  jge     loc_40F4F3
40F43D  fld     [ebp+var_4C]
40F440  fadd    [ebp+var_84]
40F446  fst     [ebp+var_A4]
40F44C  fld     [ebp+var_2C]
40F44F  fld     [ebp+var_6C]
40F452  fadd    st(1), st
40F454  fxch    st(1)
40F456  fst     [ebp+var_5C]
40F459  fcom    st(2)
40F45B  fnstsw  ax
40F45D  test    ah, 5
40F460  jp      short loc_40F482
40F462  fstp    st(2)
40F464  lea     eax, [ebp+var_5C]
40F467  fstp    st(1)
40F469  lea     ecx, [ebp+var_A4]
40F46F  fstp    st
40F471  call    sub_40EB00
40F476  fld     [ebp+var_A4]
40F47C  fld     [ebp+var_6C]
40F47F  fld     [ebp+var_5C]
40F482  fsub    st, st(2)
40F484  fcomp   ds:dbl_4EC910
40F48A  fnstsw  ax
40F48C  test    ah, 41h
40F48F  jnz     short loc_40F4F8
40F491  fstp    st
40F493  sub     esp, 10h
40F496  fld     ds:dbl_4EC9F0
40F49C  fstp    [esp+154h+X]; double
40F4A0  fstp    [esp+154h+var_154]; double
40F4A3  call    sub_4010B0
40F4A8  add     esp, 8
40F4AB  fstp    [esp+14Ch+X]; X
40F4AE  call    _floor
40F4B3  call    __ftol2_sse
40F4B8  fld     ds:dbl_4EC9F0
40F4BE  fstp    [esp+14Ch+X]; double
40F4C1  sub     esp, 8
40F4C4  fld     [ebp+var_5C]
40F4C7  mov     dword ptr [ebp+var_24+4], eax
40F4CA  fstp    [esp+154h+var_154]; double
40F4CD  call    sub_4010B0
40F4D2  add     esp, 8
40F4D5  fstp    [esp+14Ch+X]; X
40F4D8  call    _ceil
40F4DD  add     esp, 8
40F4E0  call    __ftol2_sse
40F4E5  fld     [ebp+var_6C]
40F4E8  mov     edx, dword ptr [ebp+var_94+4]
40F4EE  mov     [ebp+var_8], eax
40F4F1  jmp     short loc_40F4FA
40F4F3  fld     [ebp+var_6C]
40F4F6  jmp     short loc_40F4FA
40F4F8  fstp    st(1)
40F4FA  mov     ecx, [ebp+var_B8]
40F500  mov     eax, dword ptr [ebp+var_54+4]
40F503  mov     dword ptr [ebp+var_44+4], edx
40F506  cmp     ecx, edx
40F508  jge     short loc_40F50F
40F50A  mov     dword ptr [ebp+var_44+4], ecx
40F50D  jmp     short loc_40F512
40F50F  mov     ecx, dword ptr [ebp+var_44+4]
40F512  mov     edx, dword ptr [ebp+var_24+4]
40F515  cmp     edx, ecx
40F517  jge     short loc_40F51E
40F519  mov     ecx, edx
40F51B  mov     dword ptr [ebp+var_44+4], ecx
40F51E  mov     edx, [ebp+var_E8]
40F524  cmp     ecx, edx
40F526  jge     short loc_40F52B
40F528  mov     dword ptr [ebp+var_44+4], edx
40F52B  mov     ecx, [ebp+var_C8]
40F531  mov     edx, dword ptr [ebp+var_54+4]
40F534  cmp     edx, ecx
40F536  jge     short loc_40F53A
40F538  mov     eax, ecx
40F53A  mov     ecx, [ebp+var_8]
40F53D  cmp     eax, ecx
40F53F  jge     short loc_40F543
40F541  mov     eax, ecx
40F543  mov     ecx, [ebp+var_F8]
40F549  cmp     ecx, eax
40F54B  jge     short loc_40F54F
40F54D  mov     eax, ecx
40F54F  fild    dword ptr [ebp+var_44+4]
40F552  mov     ecx, dword ptr [ebp+var_44+4]
40F555  mov     [ebx+4], eax
40F558  mov     eax, dword ptr [ebp+var_94+4]
40F55E  fsubr   [ebp+var_12C]
40F564  mov     [ebx], ecx
40F566  mov     ecx, [ebp+var_14]
40F569  mov     [ebp+var_B8], eax
40F56F  fld     st
40F571  mov     [ebp+var_C8], edx
40F577  fld     [ebp+var_134]
40F57D  add     ebx, 20h ; ' '
40F580  fmul    st(1), st
40F582  fld     [ebp+var_11C]
40F588  fsubrp  st(2), st
40F58A  fld     st(2)
40F58C  fld     [ebp+var_124]
40F592  fmul    st(1), st
40F594  fld     [ebp+var_74]
40F597  fsubrp  st(2), st
40F599  fld     [ebp+var_AC]
40F59F  fmul    st(5), st
40F5A1  fld     [ebp+var_7C]
40F5A4  fsubrp  st(6), st
40F5A6  fxch    st(4)
40F5A8  fstp    dword ptr [ebx-18h]
40F5AB  fxch    st(1)
40F5AD  fstp    dword ptr [ebx-14h]
40F5B0  fxch    st(3)
40F5B2  fstp    dword ptr [ebx-10h]
40F5B5  fst     dword ptr [ebx-0Ch]
40F5B8  fxch    st(2)
40F5BA  fst     dword ptr [ebx-8]
40F5BD  fxch    st(1)
40F5BF  fst     dword ptr [ebx-4]
40F5C2  fxch    st(2)
40F5C4  fstp    [ebp+var_134]
40F5CA  fstp    [ebp+var_12C]
40F5D0  fstp    [ebp+var_13C]
40F5D6  fld     [ebp+var_DC]
40F5DC  fld     [ebp+var_E4]
40F5E2  fld     [ebp+var_D4]
40F5E8  fld     [ebp+var_114]
40F5EE  fld     [ebp+var_64]
40F5F1  fxch    st(4)
40F5F3  fxch    st(5)
40F5F5  fxch    st(1)
40F5F7  fxch    st(2)
40F5F9  fxch    st(1)
40F5FB  jmp     short loc_40F637
40F5FD  xor     eax, eax
40F5FF  fstp    st
40F601  mov     [ebx+8], eax
40F604  mov     [ebx+0Ch], eax
40F607  mov     [ebx+10h], eax
40F60A  mov     [ebx+14h], eax
40F60D  mov     [ebx+18h], eax
40F610  mov     [ebx+1Ch], eax
40F613  mov     dword ptr [ebx], 7FFFFFFFh
40F619  mov     dword ptr [ebx+4], 80000000h
40F620  add     ebx, 20h ; ' '
40F623  jmp     short loc_40F637
40F625  fxch    st(2)
40F627  fxch    st(5)
40F629  fxch    st(2)
40F62B  fxch    st(4)
40F62D  fxch    st(1)
40F62F  fxch    st(3)
40F631  fxch    st(1)
40F633  fxch    st(2)
40F635  fxch    st(1)
40F637  fld     [ebp+var_4C]
40F63A  inc     [ebp+var_9C]
40F640  fadd    [ebp+var_84]
40F646  inc     ecx
40F647  mov     [ebp+var_14], ecx
40F64A  fstp    [ebp+var_4C]
40F64D  fxch    st(2)
40F64F  fadd    [ebp+var_104]
40F655  fst     [ebp+var_114]
40F65B  fxch    st(5)
40F65D  fadd    [ebp+var_C4]
40F663  fst     [ebp+var_DC]
40F669  fxch    st(1)
40F66B  fadd    [ebp+var_F4]
40F671  fst     [ebp+var_D4]
40F677  fld     [ebp+var_2C]
40F67A  fadd    st, st(3)
40F67C  fstp    [ebp+var_2C]
40F67F  fxch    st(3)
40F681  fadd    [ebp+var_10C]
40F687  fst     [ebp+var_E4]
40F68D  fxch    st(4)
40F68F  fadd    [ebp+var_B4]
40F695  fst     [ebp+var_64]
40F698  fld     [ebp+var_34]
40F69B  fadd    [ebp+var_3C]
40F69E  fstp    [ebp+var_34]
40F6A1  cmp     ecx, [ebp+var_18]
40F6A4  jle     loc_40EE5C
40F6AA  fstp    st(5)
40F6AC  fstp    st
40F6AE  fstp    st(1)
40F6B0  fstp    st(1)
40F6B2  fstp    st(1)
40F6B4  fstp    st
40F6B6  mov     eax, [ebp+arg_C]
40F6B9  test    eax, eax
40F6BB  jz      short loc_40F6DA
40F6BD  mov     ecx, [ebp+var_E8]
40F6C3  mov     edx, [ebp+var_F8]
40F6C9  mov     [eax], ecx
40F6CB  mov     ecx, [ebp+var_10]
40F6CE  mov     [eax+8], edx
40F6D1  mov     edx, [ebp+var_18]
40F6D4  mov     [eax+4], ecx
40F6D7  mov     [eax+0Ch], edx
40F6DA  mov     eax, 1
40F6DF  pop     edi
40F6E0  pop     ebx
40F6E1  mov     esp, ebp
40F6E3  pop     ebp
40F6E4  retn
40F6E5  fstp    st(1)
40F6E7  fstp    st(3)
40F6E9  fstp    st
40F6EB  fstp    st
40F6ED  fstp    st
40F6EF  jmp     short loc_40F6B4
40F6F1  fstp    st(1)
40F6F3  fstp    st
40F6F5  pop     edi
40F6F6  xor     eax, eax
40F6F8  pop     ebx
40F6F9  mov     esp, ebp
40F6FB  pop     ebp
40F6FC  retn

;==== 40F700 sub_40F700
40F700  push    ebp
40F701  mov     ebp, esp
40F703  mov     edx, [ebp+arg_10]
40F706  fild    [ebp+arg_10]
40F709  sub     esp, 7Ch
40F70C  push    ebx
40F70D  push    esi
40F70E  mov     esi, [ebp+arg_30]
40F711  push    edi
40F712  mov     edi, [ebp+arg_2C]
40F715  test    edx, edx
40F717  jns     short loc_40F71F
40F719  fadd    ds:dbl_4EC908
40F71F  fld     ds:dbl_4EC968
40F725  mov     edx, [ebp+arg_14]
40F728  fmul    st(1), st
40F72A  fild    [ebp+arg_8]
40F72D  fmul    st, st(1)
40F72F  fmul    st, st(2)
40F731  fild    [ebp+arg_14]
40F734  test    edx, edx
40F736  jns     short loc_40F73E
40F738  fadd    ds:dbl_4EC908
40F73E  mov     edx, [ecx+8]
40F741  fmul    st, st(2)
40F743  fild    [ebp+arg_C]
40F746  dec     edx
40F747  mov     [ebp+arg_30], edx
40F74A  fmul    st, st(3)
40F74C  fmul    st, st(1)
40F74E  fild    [ebp+arg_30]
40F751  jns     short loc_40F759
40F753  fadd    ds:dbl_4EC908
40F759  mov     edx, [ecx+0Ch]
40F75C  fmulp   st(5), st
40F75E  dec     edx
40F75F  mov     [ebp+arg_30], edx
40F762  fild    [ebp+arg_30]
40F765  jns     short loc_40F76D
40F767  fadd    ds:dbl_4EC908
40F76D  fmulp   st(2), st
40F76F  lea     ebx, [eax+8]
40F772  fld     st(2)
40F774  fchs
40F776  fstp    [ebp+arg_30]
40F779  fld     [ebp+arg_30]
40F77C  fst     dword ptr [eax]
40F77E  fld     st(1)
40F780  fchs
40F782  fstp    [ebp+arg_30]
40F785  fld     [ebp+arg_30]
40F788  fstp    dword ptr [eax+4]
40F78B  fldz
40F78D  fst     dword ptr [ebx]
40F78F  fst     dword ptr [eax+0Ch]
40F792  fst     dword ptr [eax+10h]
40F795  fxch    st(1)
40F797  fstp    dword ptr [eax+14h]
40F79A  fxch    st(2)
40F79C  fsubrp  st(1), st
40F79E  fstp    [ebp+arg_2C]
40F7A1  fld     [ebp+arg_2C]
40F7A4  fst     dword ptr [eax+18h]
40F7A7  fxch    st(1)
40F7A9  fst     dword ptr [eax+1Ch]
40F7AC  fst     dword ptr [eax+20h]
40F7AF  mov     edx, [ecx+0Ch]
40F7B2  dec     edx
40F7B3  mov     [ebp+arg_2C], edx
40F7B6  fild    [ebp+arg_2C]
40F7B9  jns     short loc_40F7C1
40F7BB  fadd    ds:flt_4EC9C0
40F7C1  fstp    dword ptr [eax+24h]
40F7C4  fxch    st(4)
40F7C6  fsubrp  st(2), st
40F7C8  fxch    st(1)
40F7CA  fstp    [ebp+arg_2C]
40F7CD  fld     [ebp+arg_2C]
40F7D0  fst     dword ptr [eax+28h]
40F7D3  fxch    st(1)
40F7D5  fstp    dword ptr [eax+2Ch]
40F7D8  fxch    st(2)
40F7DA  fst     dword ptr [eax+30h]
40F7DD  mov     edx, [ecx+8]
40F7E0  dec     edx
40F7E1  mov     [ebp+arg_2C], edx
40F7E4  fild    [ebp+arg_2C]
40F7E7  jns     short loc_40F7EF
40F7E9  fadd    ds:flt_4EC9C0
40F7EF  fstp    dword ptr [eax+34h]
40F7F2  mov     edx, [ecx+0Ch]
40F7F5  dec     edx
40F7F6  mov     [ebp+arg_2C], edx
40F7F9  fild    [ebp+arg_2C]
40F7FC  jns     short loc_40F804
40F7FE  fadd    ds:flt_4EC9C0
40F804  fstp    dword ptr [eax+38h]
40F807  fxch    st(2)
40F809  fstp    dword ptr [eax+3Ch]
40F80C  fld     [ebp+arg_30]
40F80F  fstp    dword ptr [eax+40h]
40F812  fxch    st(1)
40F814  fst     dword ptr [eax+44h]
40F817  mov     ecx, [ecx+8]
40F81A  dec     ecx
40F81B  mov     [ebp+arg_30], ecx
40F81E  fild    [ebp+arg_30]
40F821  jns     short loc_40F829
40F823  fadd    ds:flt_4EC9C0
40F829  fstp    dword ptr [eax+48h]
40F82C  mov     [ebp+var_78], offset aPhb; "phb"
40F833  mov     [ebp+var_74], offset aPbh; "pbh"
40F83A  fstp    dword ptr [eax+4Ch]
40F83D  mov     eax, 0AAAAAAABh
40F842  mul     esi
40F844  shr     edx, 2
40F847  lea     edx, [edx+edx*2]
40F84A  add     edx, edx
40F84C  sub     esi, edx
40F84E  mov     [ebp+var_70], offset aHpb; "hpb"
40F855  mov     [ebp+var_6C], offset aHbp; "hbp"
40F85C  mov     [ebp+var_68], offset aBph; "bph"
40F863  mov     [ebp+var_64], offset aBhp; "bhp"
40F86A  mov     eax, [ebp+esi*4+var_78]
40F86E  mov     [ebp+arg_2C], eax
40F871  mov     eax, [ebp+arg_24]
40F874  neg     eax
40F876  mov     [ebp+arg_30], eax
40F879  neg     edi
40F87B  mov     esi, ebx
40F87D  fild    [ebp+arg_30]
40F880  mov     [ebp+arg_30], edi
40F883  fld     ds:dbl_4EC960
40F889  mov     ebx, 4
40F88E  fmul    st(1), st
40F890  fld     ds:dbl_4EC978
40F896  fdiv    st(2), st
40F898  fxch    st(2)
40F89A  fstp    [ebp+var_20]
40F89D  fild    [ebp+arg_28]
40F8A0  fmul    st, st(1)
40F8A2  fdiv    st, st(2)
40F8A4  fstp    [ebp+var_28]
40F8A7  fild    [ebp+arg_30]
40F8AA  fmulp   st(1), st
40F8AC  fdivrp  st(1), st
40F8AE  fstp    [ebp+var_30]
40F8B1  fild    [ebp+arg_18]
40F8B4  fmul    st, st(1)
40F8B6  fstp    [ebp+var_40]
40F8B9  fild    [ebp+arg_1C]
40F8BC  fmul    st, st(1)
40F8BE  fstp    [ebp+var_48]
40F8C1  fild    [ebp+arg_20]
40F8C4  fmul    st, st(1)
40F8C6  fstp    [ebp+var_50]
40F8C9  fild    [ebp+arg_0]
40F8CC  fmul    st, st(1)
40F8CE  fstp    [ebp+var_58]
40F8D1  fimul   [ebp+arg_4]
40F8D4  fstp    [ebp+var_60]
40F8D7  fld     dword ptr [esi-8]
40F8DA  xor     edi, edi
40F8DC  fstp    [ebp+var_8]
40F8DF  fld     dword ptr [esi-4]
40F8E2  fstp    [ebp+var_10]
40F8E5  fld     dword ptr [esi]
40F8E7  fstp    [ebp+var_18]
40F8EA  mov     ecx, [ebp+arg_2C]
40F8ED  mov     al, [edi+ecx]
40F8F0  cmp     al, 62h ; 'b'
40F8F2  jz      loc_40F978
40F8F8  cmp     al, 68h ; 'h'
40F8FA  jz      short loc_40F93E
40F8FC  cmp     al, 70h ; 'p'
40F8FE  jnz     loc_40F9B0
40F904  fld     [ebp+var_20]
40F907  call    __CIcos
40F90C  fstp    [ebp+var_38]
40F90F  fld     [ebp+var_20]
40F912  call    __CIsin
40F917  fld     [ebp+var_38]
40F91A  fld     st
40F91C  fld     [ebp+var_10]
40F91F  fmul    st(1), st
40F921  fld     st(3)
40F923  fld     [ebp+var_18]
40F926  fmul    st(1), st
40F928  fxch    st(3)
40F92A  fsubrp  st(1), st
40F92C  fxch    st(4)
40F92E  fmulp   st(1), st
40F930  fxch    st(2)
40F932  fmulp   st(1), st
40F934  faddp   st(1), st
40F936  fstp    [ebp+var_18]
40F939  fstp    [ebp+var_10]
40F93C  jmp     short loc_40F9B0
40F93E  fld     [ebp+var_28]
40F941  call    __CIcos
40F946  fstp    [ebp+var_38]
40F949  fld     [ebp+var_28]
40F94C  call    __CIsin
40F951  fld     [ebp+var_38]
40F954  fld     st
40F956  fld     [ebp+var_18]
40F959  fmul    st(1), st
40F95B  fld     st(3)
40F95D  fld     [ebp+var_8]
40F960  fmul    st(1), st
40F962  fxch    st(3)
40F964  fsubrp  st(1), st
40F966  fxch    st(4)
40F968  fmulp   st(1), st
40F96A  fxch    st(2)
40F96C  fmulp   st(1), st
40F96E  faddp   st(1), st
40F970  fstp    [ebp+var_8]
40F973  fstp    [ebp+var_18]
40F976  jmp     short loc_40F9B0
40F978  fld     [ebp+var_30]
40F97B  call    __CIcos
40F980  fstp    [ebp+var_38]
40F983  fld     [ebp+var_30]
40F986  call    __CIsin
40F98B  fld     [ebp+var_38]
40F98E  fld     st
40F990  fld     [ebp+var_8]
40F993  fmul    st(1), st
40F995  fld     st(3)
40F997  fld     [ebp+var_10]
40F99A  fmul    st(1), st
40F99C  fxch    st(3)
40F99E  fsubrp  st(1), st
40F9A0  fxch    st(4)
40F9A2  fmulp   st(1), st
40F9A4  fxch    st(2)
40F9A6  fmulp   st(1), st
40F9A8  faddp   st(1), st
40F9AA  fstp    [ebp+var_10]
40F9AD  fstp    [ebp+var_8]
40F9B0  inc     edi
40F9B1  cmp     edi, 3
40F9B4  jl      loc_40F8EA
40F9BA  fld     [ebp+var_40]
40F9BD  fadd    [ebp+var_8]
40F9C0  fld     [ebp+var_48]
40F9C3  fadd    [ebp+var_10]
40F9C6  fld     [ebp+var_50]
40F9C9  fadd    [ebp+var_18]
40F9CC  fld     st
40F9CE  fldz
40F9D0  fld     st
40F9D2  fucomp  st(2)
40F9D4  fnstsw  ax
40F9D6  fstp    st(1)
40F9D8  test    ah, 44h
40F9DB  jnp     short loc_40F9FD
40F9DD  cmp     [ebp+arg_34], 0
40F9E1  jz      short loc_40F9FD
40F9E3  fcomp   st(1)
40F9E5  fnstsw  ax
40F9E7  fild    [ebp+arg_34]
40F9EA  fld     st
40F9EC  test    ah, 5
40F9EF  jp      short loc_40F9F7
40F9F1  faddp   st(2), st
40F9F3  fdivrp  st(1), st
40F9F5  jmp     short loc_40FA03
40F9F7  fsubrp  st(2), st
40F9F9  fdivp   st(1), st
40F9FB  jmp     short loc_40FA03
40F9FD  fstp    st(1)
40F9FF  fstp    st
40FA01  fld1
40FA03  fld     st
40FA05  add     esi, 14h
40FA08  dec     ebx
40FA09  fmulp   st(3), st
40FA0B  fxch    st(2)
40FA0D  fadd    [ebp+var_58]
40FA10  fstp    dword ptr [esi-1Ch]
40FA13  fmul    st, st(1)
40FA15  fadd    [ebp+var_60]
40FA18  fstp    dword ptr [esi-18h]
40FA1B  fst     dword ptr [esi-14h]
40FA1E  fld     dword ptr [esi-10h]
40FA21  fmul    st, st(1)
40FA23  fstp    dword ptr [esi-10h]
40FA26  fmul    dword ptr [esi-0Ch]
40FA29  fstp    dword ptr [esi-0Ch]
40FA2C  jnz     loc_40F8D7
40FA32  pop     edi
40FA33  pop     esi
40FA34  pop     ebx
40FA35  mov     esp, ebp
40FA37  pop     ebp
40FA38  retn

;==== 4186F0 sub_4186F0
4186F0  push    ebp
4186F1  mov     ebp, esp
4186F3  push    ecx
4186F4  mov     eax, [ecx+10h]
4186F7  cmp     [edx+10h], eax
4186FA  jnz     short loc_418717
4186FC  dec     eax
4186FD  cmp     eax, 1
418700  ja      short loc_418717
418702  mov     eax, [ebp+arg_8]
418705  push    eax
418706  mov     eax, [ebp+arg_4]
418709  push    eax
41870A  mov     eax, [ebp+arg_0]
41870D  push    eax
41870E  push    edx
41870F  call    sub_418720
418714  add     esp, 10h
418717  pop     ecx
418718  pop     ebp
418719  retn

;==== 418720 sub_418720
418720  push    ebp
418721  mov     ebp, esp
418723  sub     esp, 0BCh
418729  mov     eax, dword_4FB734
41872E  xor     eax, ebp
418730  mov     [ebp+var_4], eax
418733  mov     eax, [ebp+arg_0]
418736  push    ebx
418737  push    esi
418738  push    edi
418739  mov     edi, ecx
41873B  mov     ecx, [eax]
41873D  mov     edx, [edi]
41873F  mov     [ebp+var_AC], ecx
418745  mov     ecx, [eax+4]
418748  mov     [ebp+var_A4], ecx
41874E  mov     ecx, [eax+8]
418751  mov     eax, [eax+0Ch]
418754  mov     [ebp+var_98], eax
41875A  mov     eax, [edi+8]
41875D  mov     [ebp+var_A0], edx
418763  mov     edx, [edi+4]
418766  mov     [ebp+var_A8], eax
41876C  mov     eax, 10h
418771  mov     [ebp+var_90], ecx
418777  mov     [ebp+var_9C], ecx
41877D  mov     [ebp+var_B0], edx
418783  xor     ecx, ecx
418785  mov     [ebp+var_8C], eax
41878B  jmp     short loc_418790
418790  cdq
418791  mov     esi, eax
418793  mov     ebx, edx
418795  shld    ebx, esi, 10h
418799  shl     esi, 10h
41879C  or      esi, eax
41879E  or      ebx, edx
4187A0  shld    ebx, esi, 10h
4187A4  shl     esi, 10h
4187A7  or      esi, eax
4187A9  mov     eax, [ebp+var_8C]
4187AF  or      ebx, edx
4187B1  shld    ebx, esi, 18h
4187B5  shl     eax, 8
4187B8  cdq
4187B9  shl     esi, 18h
4187BC  or      esi, eax
4187BE  mov     eax, [ebp+var_8C]
4187C4  or      ebx, edx
4187C6  mov     [ebp+ecx*8+var_88], esi
4187CD  mov     [ebp+ecx*8+var_88+4], ebx
4187D4  inc     ecx
4187D5  dec     eax
4187D6  mov     [ebp+var_8C], eax
4187DC  cmp     ecx, 10h
4187DF  jl      short loc_418790
4187E1  mov     eax, [edi+8]
4187E4  sub     eax, [ebp+var_90]
4187EA  mov     ebx, [ebp+var_98]
4187F0  shl     eax, 0Fh
4187F3  mov     [ebp+var_90], eax
4187F9  xor     ecx, ecx
4187FB  mov     eax, ebx
4187FD  mov     edx, 4
418802  mul     edx
418804  seto    cl
418807  neg     ecx
418809  or      ecx, eax
41880B  push    ecx; unsigned int
41880C  call    ??_U@YAPAXI@Z; operator new[](uint)
418811  add     esp, 4
418814  xor     esi, esi
418816  mov     [ebp+var_8C], eax
41881C  test    ebx, ebx
41881E  jz      short loc_41889E
418820  mov     eax, [ebp+arg_4]
418823  fild    [ebp+arg_4]
418826  test    eax, eax
418828  jns     short loc_418830
41882A  fadd    ds:dbl_4EC908
418830  fstp    [ebp+var_B8]
418836  mov     ecx, [ebp+arg_8]
418839  lea     edx, [esi+ecx]
41883C  mov     [ebp+var_94], edx
418842  fild    [ebp+var_94]
418848  test    edx, edx
41884A  jns     short loc_418852
41884C  fadd    ds:dbl_4EC908
418852  fmul    ds:dbl_4EC9C8
418858  fdiv    [ebp+var_B8]
41885E  call    __CIsin
418863  mov     eax, [edi+8]
418866  imul    eax, [ebp+arg_C]
41886A  mov     [ebp+var_94], eax
418870  fild    [ebp+var_94]
418876  test    eax, eax
418878  jns     short loc_418880
41887A  fadd    ds:dbl_4EC908
418880  fmulp   st(1), st
418882  call    __ftol2_sse
418887  mov     ecx, [ebp+var_8C]
41888D  sar     eax, 1
41888F  add     eax, [ebp+var_90]
418895  inc     esi
418896  mov     [ecx+esi*4-4], eax
41889A  cmp     esi, ebx
41889C  jb      short loc_418836
41889E  cmp     dword_565B0C, 0
4188A5  jz      loc_4189BB
4188AB  mov     eax, [ebp+var_9C]
4188B1  cmp     eax, 8
4188B4  jb      loc_4189BB
4188BA  shr     eax, 1
4188BC  mov     [ebp+var_90], eax
4188C2  pxor    mm7, mm7
4188C5  mov     edx, [ebp+var_A8]
4188CB  mov     ecx, [ebp+var_98]
4188D1  mov     edi, [ebp+var_AC]
4188D7  mov     esi, [ebp+var_A0]
4188DD  mov     ebx, [ebp+var_8C]
4188E3  push    ecx
4188E4  push    edi
4188E5  push    esi
4188E6  push    ebx
4188E7  mov     ecx, [ebp+var_90]
4188ED  mov     ebx, [ebx]
4188EF  mov     eax, ebx
4188F1  sar     ebx, 10h
4188F4  and     eax, 0F000h
4188F9  shr     eax, 9
4188FC  movq    mm6, qword ptr [ebp+eax+var_88]
418904  pxor    mm0, mm0
418907  cmp     ebx, edx
418909  jnb     short loc_418914
41890B  movd    mm0, dword ptr [esi]
41890E  punpcklbw mm0, mm7
418911  add     esi, 4
418914  inc     ebx
418915  pxor    mm1, mm1
418918  cmp     ebx, edx
41891A  jnb     short loc_418925
41891C  movd    mm1, dword ptr [esi]
41891F  punpcklbw mm1, mm7
418922  add     esi, 4
418925  inc     ebx
418926  movq    mm2, mm1
418929  pxor    mm3, mm3
41892C  cmp     ebx, edx
41892E  jnb     short loc_418939
418930  movd    mm3, dword ptr [esi]
418933  punpcklbw mm3, mm7
418936  add     esi, 4
418939  inc     ebx
41893A  movq    mm4, mm0
41893D  movq    mm5, mm2
418940  psubw   mm4, mm1
418943  psubw   mm5, mm3
418946  psllw   mm4, 4
41894A  psllw   mm5, 4
41894E  pmulhw  mm4, mm6
418951  pmulhw  mm5, mm6
418954  paddw   mm4, mm1
418957  paddw   mm5, mm3
41895A  movq    mm0, mm3
41895D  packuswb mm4, mm5
418960  movntq  qword ptr [edi], mm4
418963  add     edi, 8
418966  dec     ecx
418967  jnz     short loc_418915
418969  test    [ebp+var_9C], 1
418973  jz      short loc_41899A
418975  inc     ebx
418976  pxor    mm1, mm1
418979  cmp     ebx, edx
41897B  jnb     short loc_418983
41897D  movd    mm1, dword ptr [esi]
418980  punpcklbw mm1, mm7
418983  inc     ebx
418984  movq    mm4, mm0
418987  psubw   mm4, mm1
41898A  psllw   mm4, 4
41898E  pmulhw  mm4, mm6
418991  paddw   mm4, mm1
418994  packuswb mm4, mm7
418997  movd    dword ptr [edi], mm4
41899A  pop     ebx
41899B  pop     esi
41899C  pop     edi
41899D  pop     ecx
41899E  add     ebx, 4
4189A1  add     esi, [ebp+var_B0]
4189A7  add     edi, [ebp+var_A4]
4189AD  dec     ecx
4189AE  jnz     loc_4188E3
4189B4  emms
4189B6  jmp     loc_418A56
4189BB  pxor    mm7, mm7
4189BE  mov     edx, [ebp+var_A8]
4189C4  mov     ecx, [ebp+var_98]
4189CA  mov     edi, [ebp+var_AC]
4189D0  mov     esi, [ebp+var_A0]
4189D6  mov     ebx, [ebp+var_8C]
4189DC  push    ecx
4189DD  push    edi
4189DE  push    esi
4189DF  push    ebx
4189E0  mov     ecx, [ebp+var_9C]
4189E6  mov     ebx, [ebx]
4189E8  mov     eax, ebx
4189EA  sar     ebx, 10h
4189ED  and     eax, 0F000h
4189F2  shr     eax, 9
4189F5  movq    mm6, qword ptr [ebp+eax+var_88]
4189FD  pxor    mm2, mm2
418A00  cmp     ebx, edx
418A02  jnb     short loc_418A0D
418A04  movd    mm2, dword ptr [esi]
418A07  punpcklbw mm2, mm7
418A0A  add     esi, 4
418A0D  inc     ebx
418A0E  pxor    mm3, mm3
418A11  cmp     ebx, edx
418A13  jnb     short loc_418A1E
418A15  movd    mm3, dword ptr [esi]
418A18  punpcklbw mm3, mm7
418A1B  add     esi, 4
418A1E  inc     ebx
418A1F  movq    mm0, mm2
418A22  psubw   mm0, mm3
418A25  psllw   mm0, 4
418A29  pmulhw  mm0, mm6
418A2C  paddw   mm0, mm3
418A2F  packuswb mm0, mm7
418A32  movq    mm2, mm3
418A35  movd    dword ptr [edi], mm0
418A38  add     edi, 4
418A3B  dec     ecx
418A3C  jnz     short loc_418A0E
418A3E  pop     ebx
418A3F  pop     esi
418A40  pop     edi
418A41  pop     ecx
418A42  add     ebx, 4
418A45  add     esi, [ebp+var_B0]
418A4B  add     edi, [ebp+var_A4]
418A51  dec     ecx
418A52  jnz     short loc_4189DC
418A54  emms
418A56  mov     edx, [ebp+var_8C]
418A5C  push    edx; void *
418A5D  call    ??_V@YAXPAX@Z; operator delete[](void *)
418A62  mov     ecx, [ebp+var_4]
418A65  add     esp, 4
418A68  pop     edi
418A69  pop     esi
418A6A  xor     ecx, ebp
418A6C  pop     ebx
418A6D  call    sub_4ABDD5
418A72  mov     esp, ebp
418A74  pop     ebp
418A75  retn

;==== 419220 sub_419220
419220  mov     edx, [eax+10h]
419223  cmp     [ecx+10h], edx
419226  jnz     short locret_419233
419228  dec     edx
419229  cmp     edx, 1
41922C  ja      short locret_419233
41922E  jmp     loc_419240
419233  retn
419240  push    ebp
419241  mov     ebp, esp
419243  sub     esp, 1Ch
419246  mov     edx, [ecx]
419248  mov     ecx, [ecx+4]
41924B  mov     [ebp-18h], edx
41924E  mov     edx, [eax]
419250  mov     [ebp-14h], edx
419253  mov     edx, [eax+4]
419256  mov     [ebp-4], ecx
419259  mov     ecx, [eax+8]
41925C  mov     eax, [eax+0Ch]
41925F  mov     [ebp-10h], edx
419262  mov     edx, ecx
419264  shr     edx, 1
419266  mov     [ebp-8], edx
419269  mov     edx, eax
41926B  and     eax, 1
41926E  push    ebx
41926F  shr     edx, 1
419271  add     eax, eax
419273  and     ecx, 1
419276  push    esi
419277  or      eax, ecx
419279  push    edi
41927A  mov     [ebp-1Ch], edx
41927D  mov     [ebp-0Ch], eax
419280  pxor    mm7, mm7
419283  mov     ecx, [ebp-1Ch]
419286  mov     edi, [ebp-18h]
419289  mov     esi, [ebp-14h]
41928C  mov     ebx, [ebp-10h]
41928F  mov     eax, [ebp-0Ch]
419292  push    edi
419293  push    esi
419294  mov     edx, [ebp-8]
419297  movq    mm0, qword ptr [esi]
41929A  movq    mm2, qword ptr [esi+ebx]
41929E  movq    mm1, mm0
4192A1  movq    mm3, mm2
4192A4  punpcklbw mm0, mm7
4192A7  punpcklbw mm2, mm7
4192AA  punpckhbw mm1, mm7
4192AD  punpckhbw mm3, mm7
4192B0  paddw   mm0, mm1
4192B3  paddw   mm2, mm3
4192B6  paddw   mm0, mm2
4192B9  add     esi, 8
4192BC  psrlw   mm0, 2
4192C0  packuswb mm0, mm7
4192C3  movd    dword ptr [edi], mm0
4192C6  add     edi, 4
4192C9  dec     edx
4192CA  jnz     short loc_419297
4192CC  test    eax, 1
4192D1  jz      short loc_4192ED
4192D3  movd    mm0, dword ptr [esi]
4192D6  movd    mm1, dword ptr [esi+ebx]
4192DA  punpcklbw mm0, mm7
4192DD  punpcklbw mm1, mm7
4192E0  paddw   mm0, mm1
4192E3  psrlw   mm0, 1
4192E7  packuswb mm0, mm7
4192EA  movd    dword ptr [edi], mm0
4192ED  pop     esi
4192EE  pop     edi
4192EF  lea     esi, [esi+ebx*2]
4192F2  add     edi, [ebp-4]
4192F5  dec     ecx
4192F6  jnz     short loc_419292
4192F8  test    eax, 2
4192FD  jz      short loc_41932F
4192FF  mov     edx, [ebp-8]
419302  movq    mm0, qword ptr [esi]
419305  movq    mm1, mm0
419308  punpcklbw mm0, mm7
41930B  punpckhbw mm1, mm7
41930E  add     esi, 8
419311  paddw   mm0, mm1
419314  psrlw   mm0, 1
419318  packuswb mm0, mm7
41931B  movd    dword ptr [edi], mm0
41931E  add     edi, 4
419321  dec     edx
419322  jnz     short loc_419302
419324  test    eax, 1
419329  jz      short loc_41932F
41932B  mov     eax, [esi]
41932D  mov     [edi], eax
41932F  emms
419331  pop     edi
419332  pop     esi
419333  pop     ebx
419334  mov     esp, ebp
419336  pop     ebp
419337  retn

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

;==== 41A9D0 sub_41A9D0
41A9D0  push    ebp
41A9D1  mov     ebp, esp
41A9D3  and     esp, 0FFFFFFF8h
41A9D6  mov     eax, [ebp+arg_0]
41A9D9  dec     ecx; switch 15 cases
41A9DA  sub     esp, 8
41A9DD  cmp     ecx, 0Eh
41A9E0  ja      def_41A9E6; jumptable 0041A9E6 default case
41A9E6  jmp     ds:jpt_41A9E6[ecx*4]; switch jump
41A9ED  push    0; jumptable 0041A9E6 case 1
41A9EF  cdq
41A9F0  push    0B4h
41A9F5  push    edx
41A9F6  push    eax
41A9F7  call    __allmul
41A9FC  push    0
41A9FE  push    100h
41AA03  push    edx
41AA04  push    eax
41AA05  call    __alldiv
41AA0A  mov     ecx, 0B40000h
41AA0F  sub     ecx, eax
41AA11  mov     dword ptr [esp+8+var_8], ecx
41AA14  fild    dword ptr [esp+8+var_8]
41AA17  fmul    ds:dbl_4EC960
41AA1D  fdiv    ds:dbl_4EC978
41AA23  call    __CIcos
41AA28  fadd    ds:dbl_4E2830
41AA2E  fmul    ds:dbl_4EC950
41AA34  call    __ftol2_sse
41AA39  mov     esp, ebp
41AA3B  pop     ebp
41AA3C  retn
41AA3D  push    0; jumptable 0041A9E6 case 2
41AA3F  cdq
41AA40  push    5Ah ; 'Z'
41AA42  push    edx
41AA43  push    eax
41AA44  call    __allmul
41AA49  push    0
41AA4B  push    100h
41AA50  push    edx
41AA51  push    eax
41AA52  call    __alldiv
41AA57  mov     dword ptr [esp+8+var_8], eax
41AA5A  fild    dword ptr [esp+8+var_8]
41AA5D  fmul    ds:dbl_4EC960
41AA63  fdiv    ds:dbl_4EC978
41AA69  call    __CIsin
41AA6E  fmul    ds:dbl_4EC930
41AA74  call    __ftol2_sse
41AA79  mov     esp, ebp
41AA7B  pop     ebp
41AA7C  retn
41AA7D  push    0; jumptable 0041A9E6 case 3
41AA7F  cdq
41AA80  push    5Ah ; 'Z'
41AA82  push    edx
41AA83  push    eax
41AA84  call    __allmul
41AA89  push    0
41AA8B  push    100h
41AA90  push    edx
41AA91  push    eax
41AA92  call    __alldiv
41AA97  mov     edx, 5A0000h
41AA9C  sub     edx, eax
41AA9E  mov     dword ptr [esp+8+var_8], edx
41AAA1  fild    dword ptr [esp+8+var_8]
41AAA4  fmul    ds:dbl_4EC960
41AAAA  fdiv    ds:dbl_4EC978
41AAB0  call    __CIsin
41AAB5  fld1
41AAB7  fsubrp  st(1), st
41AAB9  fmul    ds:dbl_4EC930
41AABF  call    __ftol2_sse
41AAC4  mov     esp, ebp
41AAC6  pop     ebp
41AAC7  retn
41AAC8  fild    [ebp+arg_0]; jumptable 0041A9E6 case 4
41AACB  fld     ds:dbl_4E2838
41AAD1  call    __CIpow
41AAD6  fmul    ds:dbl_4EC930
41AADC  fstp    [esp+8+var_8]
41AADF  fld     ds:dbl_4EC9B8
41AAE5  fld     ds:dbl_4E2838
41AAEB  call    __CIpow
41AAF0  fdivr   [esp+8+var_8]
41AAF3  call    __ftol2_sse
41AAF8  mov     esp, ebp
41AAFA  pop     ebp
41AAFB  retn
41AAFC  mov     ecx, 1000000h; jumptable 0041A9E6 case 5
41AB01  sub     ecx, eax
41AB03  mov     dword ptr [esp+8+var_8], ecx
41AB06  fild    dword ptr [esp+8+var_8]
41AB09  fld     ds:dbl_4E2838
41AB0F  call    __CIpow
41AB14  fstp    [esp+8+var_8]
41AB17  fld     ds:dbl_4EC9B8
41AB1D  fld     ds:dbl_4E2838
41AB23  call    __CIpow
41AB28  fdivr   [esp+8+var_8]
41AB2B  fld1
41AB2D  fsubrp  st(1), st
41AB2F  fmul    ds:dbl_4EC930
41AB35  call    __ftol2_sse
41AB3A  mov     esp, ebp
41AB3C  pop     ebp
41AB3D  retn
41AB3E  fild    [ebp+arg_0]; jumptable 0041A9E6 case 6
41AB41  fld     ds:dbl_4EC9B0
41AB47  call    __CIpow
41AB4C  fmul    ds:dbl_4EC930
41AB52  fstp    [esp+8+var_8]
41AB55  fld     ds:dbl_4EC9B8
41AB5B  fld     ds:dbl_4EC9B0
41AB61  call    __CIpow
41AB66  fdivr   [esp+8+var_8]
41AB69  call    __ftol2_sse
41AB6E  mov     esp, ebp
41AB70  pop     ebp
41AB71  retn
41AB72  mov     edx, 1000000h; jumptable 0041A9E6 case 7
41AB77  sub     edx, eax
41AB79  mov     dword ptr [esp+8+var_8], edx
41AB7C  fild    dword ptr [esp+8+var_8]
41AB7F  fld     ds:dbl_4EC9B0
41AB85  call    __CIpow
41AB8A  fstp    [esp+8+var_8]
41AB8D  fld     ds:dbl_4EC9B8
41AB93  fld     ds:dbl_4EC9B0
41AB99  call    __CIpow
41AB9E  fdivr   [esp+8+var_8]
41ABA1  fld1
41ABA3  fsubrp  st(1), st
41ABA5  fmul    ds:dbl_4EC930
41ABAB  call    __ftol2_sse
41ABB0  mov     esp, ebp
41ABB2  pop     ebp
41ABB3  retn
41ABB4  fild    [ebp+arg_0]; jumptable 0041A9E6 case 8
41ABB7  fld     ds:dbl_4EC918
41ABBD  call    __CIpow
41ABC2  fmul    ds:dbl_4EC930
41ABC8  fstp    [esp+8+var_8]
41ABCB  fld     ds:dbl_4EC9B8
41ABD1  fld     ds:dbl_4EC918
41ABD7  call    __CIpow
41ABDC  fdivr   [esp+8+var_8]
41ABDF  call    __ftol2_sse
41ABE4  mov     esp, ebp
41ABE6  pop     ebp
41ABE7  retn
41ABE8  mov     ecx, 1000000h; jumptable 0041A9E6 case 9
41ABED  sub     ecx, eax
41ABEF  mov     dword ptr [esp+8+var_8], ecx
41ABF2  fild    dword ptr [esp+8+var_8]
41ABF5  fld     ds:dbl_4EC918
41ABFB  call    __CIpow
41AC00  fstp    [esp+8+var_8]
41AC03  fld     ds:dbl_4EC9B8
41AC09  fld     ds:dbl_4EC918
41AC0F  call    __CIpow
41AC14  fdivr   [esp+8+var_8]
41AC17  fld1
41AC19  fsubrp  st(1), st
41AC1B  fmul    ds:dbl_4EC930
41AC21  call    __ftol2_sse
41AC26  mov     esp, ebp
41AC28  pop     ebp
41AC29  retn
41AC2A  fild    [ebp+arg_0]; jumptable 0041A9E6 case 10
41AC2D  fld     ds:dbl_4EC980
41AC33  call    __CIpow
41AC38  fmul    ds:dbl_4EC930
41AC3E  fstp    [esp+8+var_8]
41AC41  fld     ds:dbl_4EC9B8
41AC47  fld     ds:dbl_4EC980
41AC4D  call    __CIpow
41AC52  fdivr   [esp+8+var_8]
41AC55  call    __ftol2_sse
41AC5A  mov     esp, ebp
41AC5C  pop     ebp
41AC5D  retn
41AC5E  mov     edx, 1000000h; jumptable 0041A9E6 case 11
41AC63  sub     edx, eax
41AC65  mov     dword ptr [esp+8+var_8], edx
41AC68  fild    dword ptr [esp+8+var_8]
41AC6B  fld     ds:dbl_4EC980
41AC71  call    __CIpow
41AC76  fstp    [esp+8+var_8]
41AC79  fld     ds:dbl_4EC9B8
41AC7F  fld     ds:dbl_4EC980
41AC85  call    __CIpow
41AC8A  fdivr   [esp+8+var_8]
41AC8D  fld1
41AC8F  fsubrp  st(1), st
41AC91  fmul    ds:dbl_4EC930
41AC97  call    __ftol2_sse
41AC9C  mov     esp, ebp
41AC9E  pop     ebp
41AC9F  retn
41ACA0  fild    [ebp+arg_0]; jumptable 0041A9E6 case 12
41ACA3  fld     ds:dbl_4EC9A8
41ACA9  call    __CIpow
41ACAE  fmul    ds:dbl_4EC930
41ACB4  fstp    [esp+8+var_8]
41ACB7  fld     ds:dbl_4EC9B8
41ACBD  fld     ds:dbl_4EC9A8
41ACC3  call    __CIpow
41ACC8  fdivr   [esp+8+var_8]
41ACCB  call    __ftol2_sse
41ACD0  mov     esp, ebp
41ACD2  pop     ebp
41ACD3  retn
41ACD4  mov     ecx, 1000000h; jumptable 0041A9E6 case 13
41ACD9  sub     ecx, eax
41ACDB  mov     dword ptr [esp+8+var_8], ecx
41ACDE  fild    dword ptr [esp+8+var_8]
41ACE1  fld     ds:dbl_4EC9A8
41ACE7  call    __CIpow
41ACEC  fstp    [esp+8+var_8]
41ACEF  fld     ds:dbl_4EC9B8
41ACF5  fld     ds:dbl_4EC9A8
41ACFB  call    __CIpow
41AD00  fdivr   [esp+8+var_8]
41AD03  fld1
41AD05  fsubrp  st(1), st
41AD07  fmul    ds:dbl_4EC930
41AD0D  call    __ftol2_sse
41AD12  mov     esp, ebp
41AD14  pop     ebp
41AD15  retn
41AD16  fild    [ebp+arg_0]; jumptable 0041A9E6 case 14
41AD19  fld     ds:dbl_4EC9A0
41AD1F  call    __CIpow
41AD24  fmul    ds:dbl_4EC930
41AD2A  fstp    [esp+8+var_8]
41AD2D  fld     ds:dbl_4EC9B8
41AD33  fld     ds:dbl_4EC9A0
41AD39  call    __CIpow
41AD3E  fdivr   [esp+8+var_8]
41AD41  call    __ftol2_sse
41AD46  mov     esp, ebp
41AD48  pop     ebp
41AD49  retn
41AD4A  mov     edx, 1000000h; jumptable 0041A9E6 case 15
41AD4F  sub     edx, eax
41AD51  mov     dword ptr [esp+8+var_8], edx
41AD54  fild    dword ptr [esp+8+var_8]
41AD57  fld     ds:dbl_4EC9A0
41AD5D  call    __CIpow
41AD62  fstp    [esp+8+var_8]
41AD65  fld     ds:dbl_4EC9B8
41AD6B  fld     ds:dbl_4EC9A0
41AD71  call    __CIpow
41AD76  fdivr   [esp+8+var_8]
41AD79  fld1
41AD7B  fsubrp  st(1), st
41AD7D  fmul    ds:dbl_4EC930
41AD83  call    __ftol2_sse
41AD88  mov     esp, ebp
41AD8A  pop     ebp
41AD8B  retn
41AD8C  cdq; jumptable 0041A9E6 default case
41AD8D  and     edx, 0FFh
41AD93  add     eax, edx
41AD95  sar     eax, 8
41AD98  mov     esp, ebp
41AD9A  pop     ebp
41AD9B  retn

;==== 41ADE0 sub_41ADE0
41ADE0  push    ebp
41ADE1  mov     ebp, esp
41ADE3  push    ebx
41ADE4  push    edi
41ADE5  mov     edi, [ebp+arg_0]
41ADE8  test    eax, eax
41ADEA  jz      short loc_41AE38
41ADEC  test    esi, esi
41ADEE  jz      short loc_41AE38
41ADF0  xor     ecx, ecx
41ADF2  mov     edx, esi
41ADF4  test    eax, eax
41ADF6  js      short loc_41AE16
41ADF8  shld    ecx, edx, 10h
41ADFC  shl     edx, 10h
41ADFF  mov     ebx, edx
41AE01  cdq
41AE02  add     ebx, eax
41AE04  adc     ecx, edx
41AE06  push    ecx
41AE07  push    ebx
41AE08  push    esi
41AE09  push    0
41AE0B  call    __alldiv
41AE10  mov     [edi], eax
41AE12  pop     edi
41AE13  pop     ebx
41AE14  pop     ebp
41AE15  retn
41AE16  mov     edi, ecx
41AE18  shld    edi, edx, 10h
41AE1C  shl     edx, 10h
41AE1F  mov     ebx, edx
41AE21  cdq
41AE22  push    ecx
41AE23  sub     ebx, eax
41AE25  push    esi
41AE26  sbb     edi, edx
41AE28  push    edi
41AE29  push    ebx
41AE2A  call    __alldiv
41AE2F  mov     ecx, [ebp+arg_0]
41AE32  mov     [ecx], eax
41AE34  pop     edi
41AE35  pop     ebx
41AE36  pop     ebp
41AE37  retn
41AE38  mov     dword ptr [edi], 10000h
41AE3E  pop     edi
41AE3F  pop     ebx
41AE40  pop     ebp
41AE41  retn

;==== 41B060 sub_41B060
41B060  mov     eax, [eax+144h]
41B066  retn

;==== 41B0B0 sub_41B0B0
41B0B0  mov     [eax+14h], ecx
41B0B3  retn

;==== 41B100 sub_41B100
41B100  mov     [eax+54h], ecx
41B103  retn

;==== 41B290 sub_41B290
41B290  mov     eax, [esi+34h]
41B293  test    eax, eax
41B295  jz      short locret_41B2B0
41B297  test    ecx, ecx
41B299  jz      short locret_41B2B0
41B29B  mov     edx, [esi+38h]
41B29E  add     eax, eax
41B2A0  add     eax, eax
41B2A2  push    eax; Size
41B2A3  push    edx; Src
41B2A4  push    ecx; void *
41B2A5  call    _memcpy_0
41B2AA  mov     eax, [esi+34h]
41B2AD  add     esp, 0Ch
41B2B0  retn

;==== 41B7C0 sub_41B7C0
41B7C0  push    ebp
41B7C1  mov     ebp, esp
41B7C3  mov     edx, [ecx+60h]
41B7C6  mov     eax, [ebp+arg_0]
41B7C9  mov     [eax], edx
41B7CB  mov     edx, [ecx+64h]
41B7CE  mov     [eax+4], edx
41B7D1  mov     edx, [ecx+68h]
41B7D4  mov     [eax+8], edx
41B7D7  mov     ecx, [ecx+6Ch]
41B7DA  mov     [eax+0Ch], ecx
41B7DD  pop     ebp
41B7DE  retn    4

;==== 41B7F0 sub_41B7F0
41B7F0  push    ebx
41B7F1  mov     ebx, esp
41B7F3  sub     esp, 8
41B7F6  and     esp, 0FFFFFFF0h
41B7F9  add     esp, 4
41B7FC  push    ebp
41B7FD  mov     ebp, [ebx+4]
41B800  mov     [esp+0Ch+var_8], ebp
41B804  mov     ebp, esp
41B806  sub     esp, 10h
41B809  push    esi
41B80A  mov     ecx, edi
41B80C  call    sub_41B7C0
41B811  lea     eax, [ebp-10h]
41B814  mov     ecx, edi
41B816  call    sub_41B8A0
41B81B  mov     eax, [ebp-10h]
41B81E  mov     ecx, [ebp-0Ch]
41B821  add     [esi], eax
41B823  add     [esi+4], ecx
41B826  mov     edx, [ebp-8]
41B829  add     [esi+8], edx
41B82C  lea     eax, [ebp-10h]
41B82F  mov     ecx, edi
41B831  call    sub_41B930
41B836  mov     eax, [esi]
41B838  add     eax, [ebp-10h]
41B83B  mov     [esi], eax
41B83D  mov     eax, [esi+4]
41B840  add     eax, [ebp-0Ch]
41B843  mov     [esi+4], eax
41B846  mov     eax, [esi+8]
41B849  add     eax, [ebp-8]
41B84C  mov     [esi+8], eax
41B84F  mov     esp, ebp
41B851  pop     ebp
41B852  mov     esp, ebx
41B854  pop     ebx
41B855  retn

;==== 41B8A0 sub_41B8A0
41B8A0  mov     edx, [ecx+70h]
41B8A3  mov     [eax], edx
41B8A5  mov     edx, [ecx+74h]
41B8A8  mov     [eax+4], edx
41B8AB  mov     edx, [ecx+78h]
41B8AE  mov     ecx, [ecx+7Ch]
41B8B1  mov     [eax+8], edx
41B8B4  mov     [eax+0Ch], ecx
41B8B7  retn

;==== 41B930 sub_41B930
41B930  mov     edx, [ecx+80h]
41B936  mov     [eax], edx
41B938  mov     edx, [ecx+84h]
41B93E  mov     [eax+4], edx
41B941  mov     edx, [ecx+88h]
41B947  mov     ecx, [ecx+8Ch]
41B94D  mov     [eax+8], edx
41B950  mov     [eax+0Ch], ecx
41B953  retn

;==== 41B960 sub_41B960
41B960  mov     [eax+0BCh], ecx
41B966  retn

;==== 41BAB0 sub_41BAB0
41BAB0  dec     eax
41BAB1  jz      short loc_41BABB
41BAB3  movzx   eax, word ptr [ecx+0CEh]
41BABA  retn
41BABB  mov     eax, [ecx+0CCh]
41BAC1  retn

;==== 41BC40 sub_41BC40
41BC40  push    ebp
41BC41  mov     ebp, esp
41BC43  mov     eax, ecx
41BC45  mov     ecx, [ebp+arg_0]
41BC48  cmp     ecx, 0C4h
41BC4E  ja      short loc_41BCCE
41BC50  jz      short loc_41BCC0
41BC52  cmp     ecx, 0C1h; switch 194 cases
41BC58  ja      def_41BC65; jumptable 0041BC65 default case, cases 3-191
41BC5E  movzx   ecx, ds:byte_41BD74[ecx]
41BC65  jmp     ds:jpt_41BC65[ecx*4]; switch jump
41BC6C  mov     ecx, [ebp+arg_8]; jumptable 0041BC65 case 0
41BC6F  mov     edx, [eax]
41BC71  mov     edx, [edx+2Ch]
41BC74  push    ecx
41BC75  mov     ecx, [ebp+arg_4]
41BC78  push    ecx
41BC79  mov     ecx, eax
41BC7B  call    edx
41BC7D  xor     eax, eax
41BC7F  pop     ebp
41BC80  retn    0Ch
41BC83  mov     ecx, [ebp+arg_4]; jumptable 0041BC65 case 1
41BC86  call    sub_41B960
41BC8B  xor     eax, eax
41BC8D  pop     ebp
41BC8E  retn    0Ch
41BC91  mov     ecx, [ebp+arg_4]; jumptable 0041BC65 case 2
41BC94  mov     edx, [eax]
41BC96  mov     edx, [edx+48h]
41BC99  push    ecx
41BC9A  mov     ecx, eax
41BC9C  call    edx
41BC9E  xor     eax, eax
41BCA0  pop     ebp
41BCA1  retn    0Ch
41BCA4  mov     ecx, [ebp+arg_4]; jumptable 0041BC65 case 192
41BCA7  call    sub_41B0B0
41BCAC  xor     eax, eax
41BCAE  pop     ebp
41BCAF  retn    0Ch
41BCB2  mov     ecx, [ebp+arg_4]; jumptable 0041BC65 case 193
41BCB5  call    sub_49FA50
41BCBA  xor     eax, eax
41BCBC  pop     ebp
41BCBD  retn    0Ch
41BCC0  mov     ecx, [ebp+arg_4]
41BCC3  call    sub_41B100
41BCC8  xor     eax, eax
41BCCA  pop     ebp
41BCCB  retn    0Ch
41BCCE  cmp     ecx, 8100h
41BCD4  ja      short loc_41BD11
41BCD6  jz      short loc_41BD03
41BCD8  sub     ecx, 8000h
41BCDE  jz      short loc_41BCF1
41BCE0  dec     ecx
41BCE1  jnz     short def_41BC65; jumptable 0041BC65 default case, cases 3-191
41BCE3  mov     ecx, [ebp+arg_4]
41BCE6  call    sub_41C1E0
41BCEB  xor     eax, eax
41BCED  pop     ebp
41BCEE  retn    0Ch
41BCF1  mov     ecx, [ebp+arg_8]
41BCF4  push    ecx
41BCF5  mov     ecx, [ebp+arg_4]
41BCF8  call    sub_41C1A0
41BCFD  xor     eax, eax
41BCFF  pop     ebp
41BD00  retn    0Ch
41BD03  mov     ecx, [ebp+arg_4]
41BD06  call    sub_41C200
41BD0B  xor     eax, eax
41BD0D  pop     ebp
41BD0E  retn    0Ch
41BD11  cmp     ecx, 7FFF0000h
41BD17  jz      short loc_41BD4B
41BD19  cmp     ecx, 7FFFFFFFh
41BD1F  jz      short loc_41BD2A
41BD21  mov     eax, 0FFFF0001h; jumptable 0041BC65 default case, cases 3-191
41BD26  pop     ebp
41BD27  retn    0Ch
41BD2A  mov     ecx, [ebp+arg_4]
41BD2D  push    esi
41BD2E  mov     esi, [ebp+arg_8]
41BD31  mov     edx, eax
41BD33  call    sub_41C4C0
41BD38  neg     eax
41BD3A  sbb     eax, eax
41BD3C  and     eax, 0FFFEh
41BD41  add     eax, 0FFFF0002h
41BD46  pop     esi
41BD47  pop     ebp
41BD48  retn    0Ch
41BD4B  mov     ecx, [ebp+arg_4]
41BD4E  call    sub_41C4A0
41BD53  xor     eax, eax
41BD55  pop     ebp
41BD56  retn    0Ch

;==== 41C1A0 sub_41C1A0
41C1A0  push    ebp
41C1A1  mov     ebp, esp
41C1A3  mov     edx, [ebp+arg_0]
41C1A6  mov     [eax+94h], ecx
41C1AC  mov     [eax+98h], edx
41C1B2  pop     ebp
41C1B3  retn    4

;==== 41C1E0 sub_41C1E0
41C1E0  mov     [eax+9Ch], ecx
41C1E6  retn

;==== 41C1F0 sub_41C1F0
41C1F0  mov     eax, [eax+9Ch]
41C1F6  retn

;==== 41C200 sub_41C200
41C200  mov     [eax+30h], ecx
41C203  retn

;==== 41C390 sub_41C390
41C390  mov     ecx, dword_565B3C
41C396  jmp     sub_4431D0

;==== 41C3B0 sub_41C3B0
41C3B0  mov     eax, [eax+114h]
41C3B6  retn

;==== 41C3C0 sub_41C3C0
41C3C0  jmp     sub_443250

;==== 41C4A0 sub_41C4A0
41C4A0  mov     [eax+0D0h], ecx
41C4A6  retn

;==== 41C4C0 sub_41C4C0
41C4C0  cmp     ecx, 10h
41C4C3  sbb     eax, eax
41C4C5  neg     eax
41C4C7  jz      short locret_41C4D0
41C4C9  mov     [edx+ecx*4+0D4h], esi
41C4D0  retn

;==== 427670 sub_427670
427670  push    ebp
427671  mov     ebp, esp
427673  and     esp, 0FFFFFFF8h
427676  push    ecx
427677  mov     edx, [eax+150h]
42767D  push    edi
42767E  cmp     edx, 6; switch 7 cases
427681  ja      def_427687; jumptable 00427687 default case, cases 1,3,4
427687  jmp     ds:jpt_427687[edx*4]; switch jump
42768E  push    ecx; jumptable 00427687 case 0
42768F  mov     edi, eax
427691  call    sub_427790
427696  pop     edi
427697  mov     esp, ebp
427699  pop     ebp
42769A  retn
42769B  mov     edx, [eax+29Ch]; jumptable 00427687 case 2
4276A1  push    edx
4276A2  mov     edx, [eax+284h]
4276A8  push    edx
4276A9  mov     edx, [eax+280h]
4276AF  push    edx
4276B0  mov     edx, [eax+26Ch]
4276B6  push    edx
4276B7  movsx   edx, word ptr [eax+26Ah]
4276BE  push    edx
4276BF  movsx   edx, word ptr [eax+266h]
4276C6  push    edx
4276C7  push    ecx
4276C8  mov     edi, eax
4276CA  call    sub_427930
4276CF  pop     edi
4276D0  mov     esp, ebp
4276D2  pop     ebp
4276D3  retn
4276D4  mov     edx, [eax+29Ch]; jumptable 00427687 case 5
4276DA  push    edx
4276DB  mov     edx, [eax+298h]
4276E1  push    edx
4276E2  mov     edx, [eax+294h]
4276E8  push    edx
4276E9  mov     edx, [eax+26Ch]
4276EF  push    edx
4276F0  movsx   edx, word ptr [eax+26Ah]
4276F7  push    edx
4276F8  movsx   edx, word ptr [eax+266h]
4276FF  push    edx
427700  push    0FFFFFFFFh
427702  push    0
427704  push    0FFFFFFFFh
427706  push    ecx
427707  call    sub_427E20
42770C  pop     edi
42770D  mov     esp, ebp
42770F  pop     ebp
427710  retn
427711  mov     edx, [eax+29Ch]; jumptable 00427687 case 6
427717  push    edx
427718  mov     edx, [eax+298h]
42771E  push    edx
42771F  mov     edx, [eax+294h]
427725  push    edx
427726  mov     edx, [eax+27Ch]
42772C  push    edx
42772D  mov     edx, [eax+278h]
427733  push    edx
427734  mov     edx, [eax+274h]
42773A  push    edx
42773B  mov     edx, [eax+270h]
427741  push    edx
427742  movsx   edx, word ptr [eax+26Ah]
427749  push    edx
42774A  movsx   edx, word ptr [eax+266h]
427751  push    edx
427752  push    0FFFFFFFFh
427754  push    0
427756  push    0FFFFFFFFh
427758  push    ecx
427759  call    sub_428120
42775E  pop     edi
42775F  mov     esp, ebp
427761  pop     ebp
427762  retn
427763  xor     eax, eax; jumptable 00427687 default case, cases 1,3,4
427765  pop     edi
427766  mov     esp, ebp
427768  pop     ebp
427769  retn

;==== 427790 sub_427790
427790  push    ebp
427791  mov     ebp, esp
427793  mov     edx, dword_565B40
427799  sub     esp, 1Ch
42779C  push    ebx
42779D  mov     ebx, [ebp+arg_0]
4277A0  push    esi
4277A1  mov     ecx, ebx
4277A3  lea     esi, [ebp+var_1C]
4277A6  call    sub_407F10
4277AB  test    eax, eax
4277AD  jz      short loc_427812
4277AF  mov     esi, edi
4277B1  call    sub_429350
4277B6  call    sub_429490
4277BB  call    sub_4294E0
4277C0  call    sub_429500
4277C5  call    sub_429550
4277CA  mov     edx, dword_565B40
4277D0  mov     ecx, ebx
4277D2  mov     dword ptr [edi+150h], 0
4277DC  mov     [edi+16Ch], ebx
4277E2  call    sub_4082F0
4277E7  mov     ecx, [ebp+var_10]
4277EA  mov     edx, [ebp+var_14]
4277ED  mov     [edi+174h], eax
4277F3  mov     eax, [edi]
4277F5  mov     eax, [eax+74h]
4277F8  push    ecx
4277F9  push    edx
4277FA  mov     ecx, edi
4277FC  mov     dword ptr [edi+260h], 0FFFFFFFFh
427806  call    eax
427808  xor     eax, eax
42780A  pop     esi
42780B  pop     ebx
42780C  mov     esp, ebp
42780E  pop     ebp
42780F  retn    4
427812  pop     esi
427813  mov     eax, 80000001h
427818  pop     ebx
427819  mov     esp, ebp
42781B  pop     ebp
42781C  retn    4

;==== 427930 sub_427930
427930  push    ebp
427931  mov     ebp, esp
427933  and     esp, 0FFFFFFF8h
427936  mov     ecx, [ebp+arg_0]
427939  mov     edx, dword_565B40
42793F  sub     esp, 48h
427942  push    ebx
427943  mov     ebx, [ebp+arg_8]
427946  push    esi
427947  lea     esi, [esp+50h+var_18]
42794B  call    sub_407F10
427950  test    eax, eax
427952  jz      loc_427AFE
427958  mov     eax, [ebp+arg_4]
42795B  mov     ecx, [ebp+arg_10]
42795E  mov     edx, [ebp+arg_C]
427961  shl     eax, 10h
427964  shl     ebx, 10h
427967  push    ebx
427968  push    eax
427969  push    10000h
42796E  push    10000h
427973  mov     [esp+60h+var_28], eax
427977  mov     eax, [ebp+arg_14]
42797A  push    eax
42797B  push    ecx
42797C  push    edx
42797D  lea     eax, [esp+6Ch+var_3C]
427981  push    eax
427982  lea     ecx, [esp+70h+var_40]
427986  push    ecx
427987  lea     edx, [esp+74h+var_44]
42798B  push    edx
42798C  lea     eax, [esp+78h+var_30]
427990  push    eax
427991  mov     eax, edi
427993  mov     [esp+7Ch+var_24], ebx
427997  call    sub_429A60
42799C  mov     ecx, [esp+50h+var_2C]
4279A0  mov     edx, [esp+50h+var_30]
4279A4  mov     eax, [esp+50h+var_40]
4279A8  push    ecx
4279A9  mov     ecx, [esp+54h+var_44]
4279AD  push    edx
4279AE  mov     edx, [esp+58h+var_C]
4279B2  push    eax
4279B3  mov     eax, [esp+5Ch+var_10]
4279B7  push    ecx
4279B8  push    edx
4279B9  push    eax
4279BA  lea     ecx, [esp+68h+var_20]
4279BE  push    ecx
4279BF  lea     edx, [esp+6Ch+var_38]
4279C3  push    edx
4279C4  mov     edx, [esp+70h+var_3C]
4279C8  lea     eax, [esp+70h+var_34]
4279CC  push    eax
4279CD  call    sub_429580
4279D2  mov     ebx, [esp+50h+var_34]
4279D6  cmp     ebx, 2
4279D9  jb      loc_427AF1
4279DF  cmp     [esp+50h+var_38], 2
4279E4  jb      loc_427AF1
4279EA  mov     esi, edi
4279EC  call    sub_429350
4279F1  call    sub_429490
4279F6  call    sub_4294E0
4279FB  call    sub_429500
427A00  call    sub_429550
427A05  mov     ecx, [ebp+arg_0]
427A08  mov     edx, dword_565B40
427A0E  mov     dword ptr [edi+150h], 2
427A18  mov     [edi+16Ch], ecx
427A1E  call    sub_4082F0
427A23  mov     edx, [esp+50h+var_24]
427A27  mov     ecx, [esp+50h+var_28]
427A2B  mov     [edi+174h], eax
427A31  mov     eax, [ebp+arg_C]
427A34  mov     [edi+268h], edx
427A3A  mov     edx, [ebp+arg_14]
427A3D  mov     [edi+26Ch], eax
427A43  mov     [edi+264h], ecx
427A49  mov     ecx, [ebp+arg_10]
427A4C  mov     [edi+284h], edx
427A52  mov     edx, [esp+50h+var_2C]
427A56  mov     eax, 10000h
427A5B  mov     [edi+288h], eax
427A61  mov     [edi+28Ch], eax
427A67  mov     eax, [ebp+arg_18]
427A6A  mov     [edi+280h], ecx
427A70  mov     ecx, [esp+50h+var_30]
427A74  mov     [edi+2A4h], edx
427A7A  mov     edx, [esp+50h+var_3C]
427A7E  mov     [edi+29Ch], eax
427A84  mov     eax, [esp+50h+var_44]
427A88  mov     [edi+2A0h], ecx
427A8E  mov     ecx, [esp+50h+var_40]
427A92  mov     [edi+2BCh], edx
427A98  mov     edx, [esp+50h+var_1C]
427A9C  mov     [edi+2A8h], eax
427AA2  mov     eax, [esp+50h+var_38]
427AA6  mov     [edi+2B8h], ecx
427AAC  mov     ecx, [esp+50h+var_20]
427AB0  mov     [edi+2FCh], edx
427AB6  mov     edx, [edi]
427AB8  push    eax
427AB9  mov     [edi+2F4h], eax
427ABF  mov     eax, [edx+74h]
427AC2  mov     [edi+2F8h], ecx
427AC8  push    ebx
427AC9  mov     ecx, edi
427ACB  mov     dword ptr [edi+260h], 0FFFFFFFFh
427AD5  mov     dword ptr [edi+290h], 0
427ADF  mov     [edi+2F0h], ebx
427AE5  call    eax
427AE7  xor     eax, eax
427AE9  pop     esi
427AEA  pop     ebx
427AEB  mov     esp, ebp
427AED  pop     ebp
427AEE  retn    1Ch
427AF1  mov     eax, 80000004h
427AF6  pop     esi
427AF7  pop     ebx
427AF8  mov     esp, ebp
427AFA  pop     ebp
427AFB  retn    1Ch
427AFE  pop     esi
427AFF  mov     eax, 80000001h
427B04  pop     ebx
427B05  mov     esp, ebp
427B07  pop     ebp
427B08  retn    1Ch

;==== 427E20 sub_427E20
427E20  push    ebp
427E21  mov     ebp, esp
427E23  and     esp, 0FFFFFFF0h
427E26  mov     ecx, [ebp+arg_0]
427E29  mov     edx, dword_565B40
427E2F  sub     esp, 74h
427E32  push    ebx
427E33  mov     ebx, [ebp+arg_14]
427E36  push    esi
427E37  push    edi
427E38  lea     esi, [esp+80h+var_18]
427E3C  mov     edi, eax
427E3E  call    sub_407F10
427E43  test    eax, eax
427E45  jz      loc_428104
427E4B  mov     ecx, [ebp+arg_4]
427E4E  xor     eax, eax
427E50  cmp     ecx, 0FFFFFFFFh
427E53  setnz   al
427E56  mov     [esp+80h+var_54], eax
427E5A  test    eax, eax
427E5C  jz      short loc_427EAB
427E5E  mov     edx, dword_565B40
427E64  lea     esi, [esp+80h+var_40]
427E68  call    sub_407F10
427E6D  test    eax, eax
427E6F  jz      short loc_427E9D
427E71  mov     eax, [esp+80h+var_10]
427E75  cmp     eax, [esp+80h+var_38]
427E79  jnz     short loc_427E8F
427E7B  mov     ecx, [esp+80h+var_C]
427E7F  cmp     ecx, [esp+80h+var_34]
427E83  jnz     short loc_427E8F
427E85  mov     edx, [esp+80h+var_8]
427E89  cmp     edx, [esp+80h+var_30]
427E8D  jz      short loc_427EAB
427E8F  mov     eax, 80000003h
427E94  pop     edi
427E95  pop     esi
427E96  pop     ebx
427E97  mov     esp, ebp
427E99  pop     ebp
427E9A  retn    28h ; '('
427E9D  mov     eax, 80000002h
427EA2  pop     edi
427EA3  pop     esi
427EA4  pop     ebx
427EA5  mov     esp, ebp
427EA7  pop     ebp
427EA8  retn    28h ; '('
427EAB  mov     eax, [ebp+arg_10]
427EAE  shl     eax, 10h
427EB1  shl     ebx, 10h
427EB4  lea     esi, [esp+80h+var_40]
427EB8  mov     [esp+80h+var_50], eax
427EBC  mov     [esp+80h+var_4C], ebx
427EC0  call    sub_41B7F0
427EC5  mov     esi, [ebp+arg_1C]
427EC8  lea     eax, [esp+80h+var_58]
427ECC  push    eax
427ECD  mov     eax, [esp+84h+var_38]
427ED1  call    sub_41ADE0
427ED6  mov     ecx, [esp+84h+var_50]
427EDA  mov     edx, [ebp+arg_18]
427EDD  add     esp, 4
427EE0  push    ebx
427EE1  mov     ebx, [esp+84h+var_58]
427EE5  push    ecx
427EE6  push    10000h
427EEB  push    10000h
427EF0  push    ebx
427EF1  push    ebx
427EF2  push    edx
427EF3  lea     eax, [esp+9Ch+var_60]
427EF7  push    eax
427EF8  lea     ecx, [esp+0A0h+var_64]
427EFC  push    ecx
427EFD  lea     edx, [esp+0A4h+var_68]
427F01  push    edx
427F02  lea     eax, [esp+0A8h+var_48]
427F06  push    eax
427F07  mov     eax, edi
427F09  call    sub_429A60
427F0E  cmp     dword ptr [edi+31Ch], 0
427F15  jbe     short loc_427F1F
427F17  mov     eax, [edi+324h]
427F1D  jmp     short loc_427F21
427F1F  xor     eax, eax
427F21  mov     ecx, [esp+80h+var_44]
427F25  mov     edx, [esp+80h+var_48]
427F29  push    ecx
427F2A  mov     ecx, [esp+84h+var_3C]
427F2E  push    edx
427F2F  mov     edx, [esp+88h+var_40]
427F33  push    ecx
427F34  mov     ecx, [esp+8Ch+var_60]
427F38  push    edx
427F39  mov     edx, [esp+90h+var_64]
427F3D  push    ecx
427F3E  mov     ecx, [esp+94h+var_68]
427F42  push    edx
427F43  mov     edx, [esp+98h+var_C]
427F4A  push    ecx
427F4B  push    eax
427F4C  mov     eax, [esp+0A0h+var_10]
427F53  push    edx
427F54  push    eax
427F55  lea     ecx, [esp+0A8h+var_40]
427F59  push    ecx
427F5A  lea     edx, [esp+0ACh+var_58]
427F5E  lea     ecx, [esp+0ACh+var_5C]
427F62  call    sub_4295C0
427F67  mov     eax, 2
427F6C  cmp     [esp+80h+var_5C], eax
427F70  jb      loc_4280F6
427F76  cmp     [esp+80h+var_58], eax
427F7A  jb      loc_4280F6
427F80  mov     esi, edi
427F82  call    sub_429350
427F87  call    sub_429490
427F8C  call    sub_4294E0
427F91  call    sub_429500
427F96  call    sub_429550
427F9B  mov     edx, [ebp+arg_4]
427F9E  mov     ecx, [ebp+arg_0]
427FA1  mov     [edi+170h], edx
427FA7  mov     edx, dword_565B40
427FAD  mov     dword ptr [edi+150h], 5
427FB7  mov     [edi+16Ch], ecx
427FBD  call    sub_4082F0
427FC2  mov     esi, [esp+80h+var_54]
427FC6  mov     [edi+174h], eax
427FCC  test    esi, esi
427FCE  jz      short loc_427FE0
427FD0  mov     ecx, [ebp+arg_4]
427FD3  mov     edx, dword_565B40
427FD9  call    sub_4082F0
427FDE  jmp     short loc_427FE2
427FE0  xor     eax, eax
427FE2  mov     [edi+178h], eax
427FE8  mov     eax, esi
427FEA  neg     eax
427FEC  sbb     eax, eax
427FEE  and     eax, [ebp+arg_8]
427FF1  mov     [edi+25Ch], eax
427FF7  mov     [edi+254h], eax
427FFD  test    esi, esi
427FFF  jz      short loc_428006
428001  mov     eax, [ebp+arg_C]
428004  jmp     short loc_428009
428006  or      eax, 0FFFFFFFFh
428009  mov     ecx, [esp+80h+var_4C]
42800D  mov     edx, [ebp+arg_18]
428010  mov     esi, [esp+80h+var_58]
428014  mov     [edi+260h], eax
42801A  mov     eax, [esp+80h+var_50]
42801E  mov     [edi+264h], eax
428024  mov     eax, 10000h
428029  mov     [edi+288h], eax
42802F  mov     [edi+28Ch], eax
428035  mov     eax, [ebp+arg_1C]
428038  mov     [edi+268h], ecx
42803E  mov     ecx, [ebp+arg_20]
428041  mov     [edi+26Ch], edx
428047  mov     edx, [ebp+arg_24]
42804A  mov     [edi+294h], eax
428050  mov     eax, [esp+80h+var_48]
428054  mov     [edi+298h], ecx
42805A  mov     ecx, [esp+80h+var_44]
42805E  mov     [edi+29Ch], edx
428064  mov     edx, [esp+80h+var_68]
428068  mov     [edi+2A0h], eax
42806E  mov     eax, [esp+80h+var_64]
428072  mov     [edi+280h], ebx
428078  mov     [edi+284h], ebx
42807E  mov     ebx, [esp+80h+var_5C]
428082  mov     [edi+2A4h], ecx
428088  mov     ecx, [esp+80h+var_60]
42808C  mov     [edi+2A8h], edx
428092  mov     edx, [esp+80h+var_40]
428096  mov     [edi+2B8h], eax
42809C  mov     eax, [esp+80h+var_3C]
4280A0  push    edi
4280A1  mov     dword ptr [edi+290h], 0
4280AB  mov     [edi+2BCh], ecx
4280B1  mov     [edi+2F0h], ebx
4280B7  mov     [edi+2F4h], esi
4280BD  mov     [edi+2F8h], edx
4280C3  mov     [edi+2FCh], eax
4280C9  call    sub_42AA60
4280CE  push    edi
4280CF  call    sub_429EB0
4280D4  push    1
4280D6  call    sub_429210
4280DB  call    sub_4293A0
4280E0  mov     edx, [edi]
4280E2  mov     eax, [edx+74h]
4280E5  push    esi
4280E6  push    ebx
4280E7  mov     ecx, edi
4280E9  call    eax
4280EB  xor     eax, eax
4280ED  pop     edi
4280EE  pop     esi
4280EF  pop     ebx
4280F0  mov     esp, ebp
4280F2  pop     ebp
4280F3  retn    28h ; '('
4280F6  mov     eax, 80000004h
4280FB  pop     edi
4280FC  pop     esi
4280FD  pop     ebx
4280FE  mov     esp, ebp
428100  pop     ebp
428101  retn    28h ; '('
428104  pop     edi
428105  pop     esi
428106  mov     eax, 80000001h
42810B  pop     ebx
42810C  mov     esp, ebp
42810E  pop     ebp
42810F  retn    28h ; '('

;==== 428120 sub_428120
428120  push    ebp
428121  mov     ebp, esp
428123  and     esp, 0FFFFFFF8h
428126  mov     edx, dword_565B40
42812C  sub     esp, 3Ch
42812F  push    ebx
428130  mov     ebx, [ebp+arg_0]
428133  push    esi
428134  push    edi
428135  mov     ecx, ebx
428137  lea     esi, [esp+48h+var_30]
42813B  mov     edi, eax
42813D  call    sub_407F10
428142  test    eax, eax
428144  jz      loc_4282FB
42814A  mov     ecx, [ebp+arg_4]
42814D  xor     eax, eax
42814F  cmp     ecx, 0FFFFFFFFh
428152  setnz   al
428155  mov     [esp+48h+var_3C], eax
428159  test    eax, eax
42815B  jz      short loc_4281AA
42815D  mov     edx, dword_565B40
428163  lea     esi, [esp+48h+var_18]
428167  call    sub_407F10
42816C  test    eax, eax
42816E  jz      short loc_42819C
428170  mov     eax, [esp+48h+var_28]
428174  cmp     eax, [esp+48h+var_10]
428178  jnz     short loc_42818E
42817A  mov     ecx, [esp+48h+var_24]
42817E  cmp     ecx, [esp+48h+var_C]
428182  jnz     short loc_42818E
428184  mov     edx, [esp+48h+var_20]
428188  cmp     edx, [esp+48h+var_8]
42818C  jz      short loc_4281AA
42818E  mov     eax, 80000003h
428193  pop     edi
428194  pop     esi
428195  pop     ebx
428196  mov     esp, ebp
428198  pop     ebp
428199  retn    34h ; '4'
42819C  mov     eax, 80000002h
4281A1  pop     edi
4281A2  pop     esi
4281A3  pop     ebx
4281A4  mov     esp, ebp
4281A6  pop     ebp
4281A7  retn    34h ; '4'
4281AA  mov     esi, edi
4281AC  call    sub_429350
4281B1  call    sub_429490
4281B6  call    sub_4294E0
4281BB  call    sub_429500
4281C0  mov     eax, [ebp+arg_14]
4281C3  mov     esi, [ebp+arg_10]
4281C6  mov     edx, dword_565B40
4281CC  shl     eax, 10h
4281CF  mov     [esp+48h+var_34], eax
4281D3  mov     eax, [ebp+arg_4]
4281D6  shl     esi, 10h
4281D9  mov     ecx, ebx
4281DB  mov     [esp+48h+var_38], esi
4281DF  mov     dword ptr [edi+150h], 6
4281E9  mov     [edi+16Ch], ebx
4281EF  mov     [edi+170h], eax
4281F5  call    sub_4082F0
4281FA  mov     ebx, [esp+48h+var_3C]
4281FE  mov     [edi+174h], eax
428204  test    ebx, ebx
428206  jz      short loc_428218
428208  mov     ecx, [ebp+arg_4]
42820B  mov     edx, dword_565B40
428211  call    sub_4082F0
428216  jmp     short loc_42821A
428218  xor     eax, eax
42821A  mov     [edi+178h], eax
428220  mov     eax, ebx
428222  neg     eax
428224  sbb     eax, eax
428226  and     eax, [ebp+arg_8]
428229  mov     [edi+25Ch], eax
42822F  mov     [edi+254h], eax
428235  test    ebx, ebx
428237  jz      short loc_42823E
428239  mov     eax, [ebp+arg_C]
42823C  jmp     short loc_428241
42823E  or      eax, 0FFFFFFFFh
428241  mov     ecx, [esp+48h+var_34]
428245  mov     edx, [ebp+arg_18]
428248  mov     ebx, [ebp+arg_28]
42824B  mov     [edi+260h], eax
428251  mov     eax, [ebp+arg_1C]
428254  mov     [edi+268h], ecx
42825A  mov     ecx, [ebp+arg_20]
42825D  mov     [edi+264h], esi
428263  mov     esi, 10000h
428268  mov     [edi+270h], edx
42826E  mov     edx, [ebp+arg_24]
428271  mov     [edi+274h], eax
428277  mov     eax, [ebp+arg_2C]
42827A  mov     [edi+278h], ecx
428280  mov     ecx, [ebp+arg_30]
428283  push    edi
428284  mov     [edi+27Ch], edx
42828A  mov     [edi+280h], esi
428290  mov     [edi+284h], esi
428296  mov     [edi+288h], esi
42829C  mov     [edi+28Ch], esi
4282A2  mov     dword ptr [edi+290h], 0
4282AC  mov     [edi+294h], ebx
4282B2  mov     [edi+298h], eax
4282B8  mov     [edi+29Ch], ecx
4282BE  call    sub_42AA60
4282C3  push    1
4282C5  call    sub_429210
4282CA  call    sub_4293A0
4282CF  mov     edx, [ebp+arg_24]
4282D2  mov     eax, [ebp+arg_20]
4282D5  mov     ecx, [ebp+arg_1C]
4282D8  push    esi
4282D9  push    esi
4282DA  push    esi
4282DB  push    esi
4282DC  push    ebx
4282DD  push    edx
4282DE  mov     edx, [ebp+arg_18]
4282E1  push    eax
4282E2  push    ecx
4282E3  push    edx
4282E4  lea     eax, [esp+6Ch+var_38]
4282E8  push    eax
4282E9  mov     ecx, edi
4282EB  call    sub_42A4F0
4282F0  xor     eax, eax
4282F2  pop     edi
4282F3  pop     esi
4282F4  pop     ebx
4282F5  mov     esp, ebp
4282F7  pop     ebp
4282F8  retn    34h ; '4'
4282FB  pop     edi
4282FC  pop     esi
4282FD  mov     eax, 80000001h
428302  pop     ebx
428303  mov     esp, ebp
428305  pop     ebp
428306  retn    34h ; '4'

;==== 4283C0 sub_4283C0
4283C0  push    ebp
4283C1  mov     ebp, esp
4283C3  mov     ecx, [eax+150h]
4283C9  push    esi
4283CA  mov     esi, [ebp+arg_0]
4283CD  cmp     ecx, 2
4283D0  jz      short loc_4283DA
4283D2  add     ecx, 0FFFFFFFBh
4283D5  cmp     ecx, 1
4283D8  ja      short loc_4283EC
4283DA  shl     esi, 10h
4283DD  shl     edx, 10h
4283E0  mov     [eax+264h], esi
4283E6  mov     [eax+268h], edx
4283EC  pop     esi
4283ED  pop     ebp
4283EE  retn    4

;==== 428400 sub_428400
428400  push    ebp
428401  mov     ebp, esp
428403  and     esp, 0FFFFFFF8h
428406  push    ecx
428407  push    edi
428408  mov     edi, eax
42840A  mov     ecx, [edi+150h]
428410  xor     eax, eax
428412  sub     ecx, 2
428415  jz      short loc_428452
428417  sub     ecx, 3
42841A  jz      short loc_428434
42841C  dec     ecx
42841D  jnz     short loc_428462
42841F  mov     [edi+278h], edx
428425  call    sub_42A470
42842A  mov     eax, 1
42842F  pop     edi
428430  mov     esp, ebp
428432  pop     ebp
428433  retn
428434  mov     eax, edi
428436  mov     [edi+26Ch], edx
42843C  call    sub_429D40
428441  push    0
428443  call    sub_429210
428448  mov     eax, 1
42844D  pop     edi
42844E  mov     esp, ebp
428450  pop     ebp
428451  retn
428452  mov     [edi+26Ch], edx
428458  call    sub_429C20
42845D  mov     eax, 1
428462  pop     edi
428463  mov     esp, ebp
428465  pop     ebp
428466  retn

;==== 4284A0 sub_4284A0
4284A0  push    ebp
4284A1  mov     ebp, esp
4284A3  xor     eax, eax
4284A5  cmp     dword ptr [ecx+150h], 6
4284AC  jnz     short loc_4284C2
4284AE  mov     eax, [ebp+arg_0]
4284B1  mov     [ecx+270h], eax
4284B7  mov     [ecx+274h], edx
4284BD  mov     eax, 1
4284C2  pop     ebp
4284C3  retn    4

;==== 4284D0 sub_4284D0
4284D0  push    ebp
4284D1  mov     ebp, esp
4284D3  and     esp, 0FFFFFFF8h
4284D6  push    esi
4284D7  xor     eax, eax
4284D9  push    edi
4284DA  mov     edi, [ebp+arg_0]
4284DD  test    ecx, ecx
4284DF  jnz     short loc_4284E4
4284E1  lea     ecx, [eax+1]
4284E4  test    edx, edx
4284E6  jnz     short loc_4284F2
4284E8  mov     edx, ecx
4284EA  mov     [edi+290h], eax
4284F0  jmp     short loc_4284FC
4284F2  mov     dword ptr [edi+290h], 1
4284FC  mov     esi, [edi+150h]
428502  sub     esi, 2
428505  jz      short loc_428557
428507  sub     esi, 3
42850A  jz      short loc_42852B
42850C  dec     esi
42850D  jnz     short loc_42856D
42850F  mov     [edi+288h], ecx
428515  mov     [edi+28Ch], edx
42851B  call    sub_42A470
428520  lea     eax, [esi+1]
428523  pop     edi
428524  pop     esi
428525  mov     esp, ebp
428527  pop     ebp
428528  retn    4
42852B  mov     eax, edi
42852D  mov     [edi+288h], ecx
428533  mov     [edi+28Ch], edx
428539  call    sub_429D40
42853E  push    0
428540  call    sub_429210
428545  call    sub_4293A0
42854A  mov     eax, 1
42854F  pop     edi
428550  pop     esi
428551  mov     esp, ebp
428553  pop     ebp
428554  retn    4
428557  mov     [edi+288h], ecx
42855D  mov     [edi+28Ch], edx
428563  call    sub_429C20
428568  mov     eax, 1
42856D  pop     edi
42856E  pop     esi
42856F  mov     esp, ebp
428571  pop     ebp
428572  retn    4

;==== 4288E0 sub_4288E0
4288E0  push    ebp
4288E1  mov     ebp, esp
4288E3  and     esp, 0FFFFFFF8h
4288E6  push    ecx
4288E7  mov     edx, [ebp+arg_4]
4288EA  push    edi
4288EB  mov     edi, [ebp+arg_0]
4288EE  mov     [edi+31Ch], eax
4288F4  mov     eax, [edi+150h]
4288FA  sub     eax, 5
4288FD  mov     [edi+320h], ecx
428903  mov     [edi+324h], edx
428909  jz      short loc_42891F
42890B  dec     eax
42890C  jnz     short loc_42892B
42890E  call    sub_4293A0
428913  call    sub_42A470
428918  pop     edi
428919  mov     esp, ebp
42891B  pop     ebp
42891C  retn    8
42891F  call    sub_4293A0
428924  mov     eax, edi
428926  call    sub_429D40
42892B  pop     edi
42892C  mov     esp, ebp
42892E  pop     ebp
42892F  retn    8

;==== 428940 sub_428940
428940  push    ebp
428941  mov     ebp, esp
428943  push    esi
428944  mov     esi, ecx
428946  mov     ecx, [ebp+arg_0]
428949  lea     eax, [ecx-10h]; switch 241 cases
42894C  cmp     eax, 0F0h
428951  ja      def_42895E; jumptable 0042895E default case, cases 18-63,68-95,97-127,132-142,144-255
428957  movzx   eax, ds:byte_428B90[eax]
42895E  jmp     ds:jpt_42895E[eax*4]; switch jump
428965  mov     ecx, [ebp+arg_4]; jumptable 0042895E case 16
428968  mov     eax, esi
42896A  call    sub_427670
42896F  neg     eax
428971  sbb     eax, eax
428973  and     eax, 0FFFF0002h
428978  pop     esi
428979  pop     ebp
42897A  retn    0Ch
42897D  mov     eax, [ebp+arg_4]; jumptable 0042895E case 17
428980  mov     edx, [ebp+arg_8]
428983  mov     ecx, eax
428985  shr     ecx, 10h
428988  push    ecx
428989  mov     ecx, eax
42898B  shr     ecx, 8
42898E  push    edx
42898F  and     ecx, 0FFh
428995  push    ecx
428996  movzx   ecx, al
428999  mov     edx, esi
42899B  call    sub_42AF80
4289A0  neg     eax
4289A2  sbb     eax, eax
4289A4  and     eax, 0FFFF0002h
4289A9  pop     esi
4289AA  pop     ebp
4289AB  retn    0Ch
4289AE  mov     eax, [esi+150h]; jumptable 0042895E case 64
4289B4  cmp     eax, 2
4289B7  jz      short loc_4289C1
4289B9  add     eax, 0FFFFFFFBh
4289BC  cmp     eax, 1
4289BF  ja      short loc_4289CF
4289C1  mov     edx, [ebp+arg_4]
4289C4  push    edx
4289C5  mov     edx, [ebp+arg_8]
4289C8  mov     eax, esi
4289CA  call    sub_4283C0
4289CF  xor     eax, eax
4289D1  pop     esi
4289D2  pop     ebp
4289D3  retn    0Ch
4289D6  mov     eax, [esi+150h]; jumptable 0042895E case 65
4289DC  cmp     eax, 2
4289DF  jz      short loc_4289E9
4289E1  add     eax, 0FFFFFFFBh
4289E4  cmp     eax, 1
4289E7  ja      short loc_4289F3
4289E9  mov     edx, [ebp+arg_4]
4289EC  mov     eax, esi
4289EE  call    sub_428400
4289F3  xor     eax, eax
4289F5  pop     esi
4289F6  pop     ebp
4289F7  retn    0Ch
4289FA  cmp     dword ptr [esi+150h], 6; jumptable 0042895E case 67
428A01  jnz     short loc_428A11
428A03  mov     eax, [ebp+arg_4]
428A06  mov     edx, [ebp+arg_8]
428A09  push    eax
428A0A  mov     ecx, esi
428A0C  call    sub_4284A0
428A11  xor     eax, eax
428A13  pop     esi
428A14  pop     ebp
428A15  retn    0Ch
428A18  mov     eax, [esi+150h]; jumptable 0042895E case 66
428A1E  cmp     eax, 2
428A21  jz      short loc_428A2B
428A23  add     eax, 0FFFFFFFBh
428A26  cmp     eax, 1
428A29  ja      short loc_428A37
428A2B  mov     edx, [ebp+arg_8]
428A2E  mov     ecx, [ebp+arg_4]
428A31  push    esi
428A32  call    sub_4284D0
428A37  xor     eax, eax
428A39  pop     esi
428A3A  pop     ebp
428A3B  retn    0Ch
428A3E  mov     ecx, [ebp+arg_8]; jumptable 0042895E case 96
428A41  mov     eax, [ebp+arg_4]
428A44  push    ecx
428A45  movzx   ecx, ax
428A48  shr     eax, 10h
428A4B  push    esi
428A4C  call    sub_4288E0
428A51  xor     eax, eax
428A53  pop     esi
428A54  pop     ebp
428A55  retn    0Ch
428A58  mov     eax, [esi+150h]; jumptable 0042895E case 128
428A5E  cmp     eax, 2
428A61  jz      short loc_428A6B
428A63  add     eax, 0FFFFFFFBh
428A66  cmp     eax, 1
428A69  ja      short loc_428A7D
428A6B  mov     edx, [ebp+arg_4]
428A6E  mov     eax, [ebp+arg_8]
428A71  mov     [esi+2C8h], edx
428A77  mov     [esi+2CCh], eax
428A7D  xor     eax, eax
428A7F  pop     esi
428A80  pop     ebp
428A81  retn    0Ch
428A84  mov     eax, [esi+150h]; jumptable 0042895E case 129
428A8A  sub     eax, 2
428A8D  jz      short loc_428AAE
428A8F  sub     eax, 3
428A92  jz      short loc_428AAE
428A94  dec     eax
428A95  jnz     short loc_428AC0
428A97  mov     ecx, [ebp+arg_4]
428A9A  mov     edx, [ebp+arg_8]
428A9D  mov     [esi+2DCh], ecx
428AA3  mov     [esi+2E8h], edx
428AA9  pop     esi
428AAA  pop     ebp
428AAB  retn    0Ch
428AAE  mov     eax, [ebp+arg_4]
428AB1  mov     ecx, [ebp+arg_8]
428AB4  mov     [esi+2D0h], eax
428ABA  mov     [esi+2E8h], ecx
428AC0  xor     eax, eax
428AC2  pop     esi
428AC3  pop     ebp
428AC4  retn    0Ch
428AC7  mov     eax, [esi+150h]; jumptable 0042895E case 130
428ACD  cmp     eax, 2
428AD0  jz      short loc_428ADA
428AD2  add     eax, 0FFFFFFFBh
428AD5  cmp     eax, 1
428AD8  ja      short loc_428AEC
428ADA  mov     edx, [ebp+arg_4]
428ADD  mov     eax, [ebp+arg_8]
428AE0  mov     [esi+2E0h], edx
428AE6  mov     [esi+2E4h], eax
428AEC  xor     eax, eax
428AEE  pop     esi
428AEF  pop     ebp
428AF0  retn    0Ch
428AF3  cmp     dword ptr [esi+150h], 6; jumptable 0042895E case 131
428AFA  jnz     short loc_428B0E
428AFC  mov     ecx, [ebp+arg_4]
428AFF  mov     edx, [ebp+arg_8]
428B02  mov     [esi+2D4h], ecx
428B08  mov     [esi+2D8h], edx
428B0E  xor     eax, eax
428B10  pop     esi
428B11  pop     ebp
428B12  retn    0Ch
428B15  mov     eax, [ebp+arg_4]; jumptable 0042895E case 143
428B18  mov     [esi+2ECh], eax
428B1E  xor     eax, eax
428B20  pop     esi
428B21  pop     ebp
428B22  retn    0Ch
428B25  mov     ecx, [ebp+arg_8]; jumptable 0042895E case 256
428B28  push    edi
428B29  mov     edi, [ebp+arg_4]
428B2C  push    ecx
428B2D  call    sub_429180
428B32  neg     eax
428B34  sbb     eax, eax
428B36  pop     edi
428B37  and     eax, 0FFFF0002h
428B3C  pop     esi
428B3D  pop     ebp
428B3E  retn    0Ch
428B41  mov     edx, [ebp+arg_8]; jumptable 0042895E default case, cases 18-63,68-95,97-127,132-142,144-255
428B44  mov     eax, [ebp+arg_4]
428B47  push    edx
428B48  push    eax
428B49  push    ecx
428B4A  mov     ecx, esi
428B4C  call    sub_41BC40
428B51  pop     esi
428B52  pop     ebp
428B53  retn    0Ch

;==== 429180 sub_429180
429180  push    ebp
429181  mov     ebp, esp
429183  xor     eax, eax
429185  cmp     dword ptr [esi+150h], 4
42918C  push    ebx
42918D  mov     ebx, [ebp+arg_0]
429190  jnz     short loc_429202
429192  mov     eax, [esi+348h]
429198  push    eax
429199  mov     eax, [esi+350h]
42919F  push    edi
4291A0  lea     ecx, [ebp+arg_0]
4291A3  push    ecx
4291A4  mov     ecx, dword_565B40
4291AA  call    sub_409FE0
4291AF  test    eax, eax
4291B1  jnz     short loc_4291FD
4291B3  cmp     [ebp+arg_0], eax
4291B6  jz      short loc_4291F3
4291B8  mov     [esi+354h], edi
4291BE  cmp     ebx, 100h
4291C4  ja      short loc_4291D9
4291C6  mov     edx, [esi]
4291C8  mov     eax, ebx
4291CA  push    eax
4291CB  mov     eax, [edx+48h]
4291CE  mov     ecx, esi
4291D0  call    eax
4291D2  xor     eax, eax
4291D4  pop     ebx
4291D5  pop     ebp
4291D6  retn    4
4291D9  mov     edx, [esi]
4291DB  mov     eax, [edx+4Ch]
4291DE  mov     ecx, esi
4291E0  call    eax
4291E2  mov     edx, [esi]
4291E4  push    eax
4291E5  mov     eax, [edx+48h]
4291E8  mov     ecx, esi
4291EA  call    eax
4291EC  xor     eax, eax
4291EE  pop     ebx
4291EF  pop     ebp
4291F0  retn    4
4291F3  mov     eax, 80000009h
4291F8  pop     ebx
4291F9  pop     ebp
4291FA  retn    4
4291FD  mov     eax, 80000008h
429202  pop     ebx
429203  pop     ebp
429204  retn    4

;==== 429210 sub_429210
429210  push    ebp
429211  mov     ebp, esp
429213  mov     eax, [edi+150h]
429219  sub     esp, 38h
42921C  sub     eax, 5
42921F  push    esi
429220  jz      short loc_42922C
429222  dec     eax
429223  jz      short loc_42928E
429225  pop     esi
429226  mov     esp, ebp
429228  pop     ebp
429229  retn    4
42922C  cmp     [ebp+arg_0], 0
429230  jnz     short loc_42928E
429232  cmp     dword ptr [edi+2A8h], 0
429239  jnz     short loc_42927C
42923B  mov     eax, 10000h
429240  cmp     [edi+2B8h], eax
429246  jnz     short loc_42927C
429248  cmp     [edi+2BCh], eax
42924E  jnz     short loc_42927C
429250  cmp     dword ptr [edi+2C0h], 0
429257  jnz     short loc_42927C
429259  cmp     dword ptr [edi+2C4h], 0
429260  jnz     short loc_42927C
429262  cmp     dword ptr [edi+31Ch], 0
429269  jbe     loc_429343
42926F  cmp     dword ptr [edi+324h], 0
429276  jbe     loc_429343
42927C  mov     eax, [edi+254h]
429282  cmp     eax, [edi+25Ch]
429288  jz      loc_429343
42928E  mov     esi, edi
429290  call    sub_429350
429295  cmp     dword ptr [edi+170h], 0FFFFFFFFh
42929C  jz      loc_429343
4292A2  push    0
4292A4  lea     ecx, [ebp+arg_0]
4292A7  push    ecx
4292A8  lea     eax, [ebp+var_1C]
4292AB  mov     ecx, edi
4292AD  call    sub_42AED0
4292B2  test    eax, eax
4292B4  jz      loc_429343
4292BA  push    1
4292BC  push    0
4292BE  lea     eax, [ebp+var_34]
4292C1  mov     ecx, edi
4292C3  call    sub_42AED0
4292C8  test    eax, eax
4292CA  jz      short loc_429343
4292CC  mov     esi, dword_565B40
4292D2  mov     ecx, [edi+16Ch]
4292D8  mov     edx, esi
4292DA  call    sub_4082F0
4292DF  cmp     [edi+174h], eax
4292E5  jnz     short loc_429343
4292E7  mov     ecx, [edi+170h]
4292ED  mov     edx, esi
4292EF  call    sub_4082F0
4292F4  cmp     [edi+178h], eax
4292FA  jnz     short loc_429343
4292FC  mov     edx, [ebp+var_14]
4292FF  mov     eax, [ebp+var_C]
429302  push    edx
429303  mov     edx, [ebp+var_10]
429306  lea     esi, [edi+23Ch]
42930C  call    sub_409020
429311  mov     eax, [edi+25Ch]
429317  add     esp, 4
42931A  push    1
42931C  push    eax
42931D  lea     ecx, [ebp+var_34]
429320  push    ecx
429321  lea     edx, [ebp+var_1C]
429324  push    edx
429325  push    esi
429326  call    sub_40C4F0
42932B  mov     eax, [edi+25Ch]
429331  mov     ecx, [ebp+arg_0]
429334  add     esp, 14h
429337  mov     [edi+254h], eax
42933D  mov     [edi+258h], ecx
429343  pop     esi
429344  mov     esp, ebp
429346  pop     ebp
429347  retn    4

;==== 429350 sub_429350
429350  mov     eax, [esi+23Ch]
429356  xor     ecx, ecx
429358  test    eax, eax
42935A  setnz   cl
42935D  push    edi
42935E  mov     edi, ecx
429360  test    edi, edi
429362  jz      short loc_429393
429364  push    eax; void *
429365  call    ??_V@YAXPAX@Z; operator delete[](void *)
42936A  add     esp, 4
42936D  xor     eax, eax
42936F  mov     [esi+23Ch], eax
429375  mov     [esi+240h], eax
42937B  mov     [esi+244h], eax
429381  mov     [esi+248h], eax
429387  mov     [esi+24Ch], eax
42938D  mov     [esi+250h], eax
429393  mov     eax, edi
429395  pop     edi
429396  retn

;==== 4293A0 sub_4293A0
4293A0  push    ebp
4293A1  mov     ebp, esp
4293A3  mov     eax, [edi+150h]
4293A9  sub     esp, 20h
4293AC  add     eax, 0FFFFFFFBh
4293AF  push    ebx
4293B0  push    esi
4293B1  cmp     eax, 1
4293B4  ja      loc_429465
4293BA  mov     esi, edi
4293BC  call    sub_429490
4293C1  cmp     dword ptr [edi+31Ch], 0
4293C8  jbe     loc_429465
4293CE  cmp     dword ptr [edi+324h], 0
4293D5  jbe     loc_429465
4293DB  push    0
4293DD  lea     eax, [ebp+var_4]
4293E0  push    eax
4293E1  lea     eax, [ebp+var_1C]
4293E4  mov     ecx, edi
4293E6  call    sub_42AED0
4293EB  test    eax, eax
4293ED  jz      short loc_429465
4293EF  cmp     dword ptr [edi+170h], 0FFFFFFFFh
4293F6  jz      short loc_42946B
4293F8  cmp     dword ptr [edi+23Ch], 0
4293FF  lea     ebx, [edi+23Ch]
429405  jz      short loc_429465
429407  test    ebx, ebx
429409  jz      short loc_429465
42940B  mov     eax, [ebx+8]
42940E  mov     ecx, [edi+324h]
429414  mov     edx, [ebx+0Ch]
429417  add     ecx, 10000h
42941D  imul    ecx, eax
429420  shr     ecx, 10h
429423  sub     ecx, eax
429425  inc     ecx
429426  and     ecx, 0FFFFFFFEh
429429  add     ecx, eax
42942B  mov     eax, [ebx+10h]
42942E  lea     esi, [edi+328h]
429434  push    ecx
429435  call    sub_409020
42943A  mov     edx, [edi+324h]
429440  mov     ecx, [ebp+var_4]
429443  mov     eax, [edi+320h]
429449  add     esp, 4
42944C  push    edx
42944D  mov     edx, [edi+31Ch]
429453  shr     eax, cl
429455  shr     edx, cl
429457  mov     ecx, ebx
429459  push    eax
42945A  push    edx
42945B  mov     edx, esi
42945D  call    sub_4186F0
429462  add     esp, 0Ch
429465  pop     esi
429466  pop     ebx
429467  mov     esp, ebp
429469  pop     ebp
42946A  retn
42946B  mov     ecx, [edi+16Ch]
429471  mov     edx, dword_565B40
429477  call    sub_4082F0
42947C  cmp     [edi+174h], eax
429482  jnz     short loc_429465
429484  lea     ebx, [ebp+var_1C]
429487  jmp     short loc_42940B

;==== 429490 sub_429490
429490  mov     eax, [esi+328h]
429496  xor     ecx, ecx
429498  test    eax, eax
42949A  setnz   cl
42949D  push    edi
42949E  mov     edi, ecx
4294A0  test    edi, edi
4294A2  jz      short loc_4294D3
4294A4  push    eax; void *
4294A5  call    ??_V@YAXPAX@Z; operator delete[](void *)
4294AA  add     esp, 4
4294AD  xor     eax, eax
4294AF  mov     [esi+328h], eax
4294B5  mov     [esi+32Ch], eax
4294BB  mov     [esi+330h], eax
4294C1  mov     [esi+334h], eax
4294C7  mov     [esi+338h], eax
4294CD  mov     [esi+33Ch], eax
4294D3  mov     eax, edi
4294D5  pop     edi
4294D6  retn

;==== 4294E0 sub_4294E0
4294E0  mov     eax, [esi+34Ch]
4294E6  push    eax; void *
4294E7  call    ??_V@YAXPAX@Z; operator delete[](void *)
4294EC  add     esp, 4
4294EF  mov     dword ptr [esi+34Ch], 0
4294F9  retn

;==== 429500 sub_429500
429500  mov     eax, [esi+35Ch]
429506  xor     ecx, ecx
429508  test    eax, eax
42950A  setnz   cl
42950D  push    edi
42950E  mov     edi, ecx
429510  test    edi, edi
429512  jz      short loc_429543
429514  push    eax; void *
429515  call    ??_V@YAXPAX@Z; operator delete[](void *)
42951A  add     esp, 4
42951D  xor     eax, eax
42951F  mov     [esi+35Ch], eax
429525  mov     [esi+360h], eax
42952B  mov     [esi+364h], eax
429531  mov     [esi+368h], eax
429537  mov     [esi+36Ch], eax
42953D  mov     [esi+370h], eax
429543  mov     eax, edi
429545  pop     edi
429546  retn

;==== 429550 sub_429550
429550  mov     ecx, [esi+300h]
429556  xor     eax, eax
429558  test    ecx, ecx
42955A  jz      short locret_429574
42955C  push    ecx; void *
42955D  call    ??_V@YAXPAX@Z; operator delete[](void *)
429562  add     esp, 4
429565  mov     dword ptr [esi+300h], 0
42956F  mov     eax, 1
429574  retn

;==== 429580 sub_429580
429580  push    ebp
429581  mov     ebp, esp
429583  push    ecx
429584  mov     eax, [ebp+arg_20]
429587  mov     ecx, [ebp+arg_1C]
42958A  push    eax
42958B  mov     eax, [ebp+arg_18]
42958E  push    ecx
42958F  mov     ecx, [ebp+arg_14]
429592  push    0
429594  push    0
429596  push    edx
429597  mov     edx, [ebp+arg_10]
42959A  push    eax
42959B  mov     eax, [ebp+arg_C]
42959E  push    ecx
42959F  mov     ecx, [ebp+arg_8]
4295A2  push    0
4295A4  push    edx
4295A5  mov     edx, [ebp+arg_4]
4295A8  push    eax
4295A9  push    ecx
4295AA  mov     ecx, [ebp+arg_0]
4295AD  call    sub_4295C0
4295B2  pop     ecx
4295B3  pop     ebp
4295B4  retn    24h ; '$'

;==== 4295C0 sub_4295C0
4295C0  push    ebp
4295C1  mov     ebp, esp
4295C3  sub     esp, 0D0h
4295C9  mov     eax, dword_4FB734
4295CE  xor     eax, ebp
4295D0  mov     [ebp+var_8], eax
4295D3  mov     eax, [ebp+arg_4]
4295D6  mov     [ebp+var_98], edx
4295DC  mov     edx, [ebp+arg_C]
4295DF  add     edx, 10000h
4295E5  imul    edx, eax
4295E8  push    ebx
4295E9  push    esi
4295EA  mov     esi, [ebp+arg_18]
4295ED  mov     [ebp+var_A8], ecx
4295F3  mov     ecx, [ebp+arg_0]
4295F6  mov     dword ptr [ebp+var_94+4], edx
4295FC  push    edi
4295FD  mov     edi, [ebp+arg_14]
429600  fild    dword ptr [ebp+var_94+4]
429606  mov     [ebp+var_9C], ecx
42960C  test    edx, edx
42960E  jns     short loc_429616
429610  fadd    ds:dbl_4EC908
429616  fld     ds:dbl_4EC968
42961C  mov     ecx, [ebp+arg_8]
42961F  fmul    st(1), st
429621  fxch    st(1)
429623  fst     [ebp+var_BC]
429629  fild    [ebp+arg_8]
42962C  test    ecx, ecx
42962E  jns     short loc_429636
429630  fadd    ds:dbl_4EC908
429636  mov     dword ptr [ebp+var_94+4], eax
42963C  fstp    [ebp+var_C4]
429642  fild    dword ptr [ebp+var_94+4]
429648  test    eax, eax
42964A  jns     short loc_429652
42964C  fadd    ds:dbl_4EC908
429652  fsubp   st(1), st
429654  fmul    ds:dbl_4E1EE8
42965A  fild    [ebp+arg_24]
42965D  fmul    st, st(2)
42965F  faddp   st(1), st
429661  fstp    [ebp+var_CC]
429667  fimul   [ebp+arg_28]
42966A  fstp    [ebp+var_B4]
429670  fild    [ebp+arg_10]
429673  fmul    ds:dbl_4EC960
429679  fdiv    ds:dbl_4EC978
42967F  fst     qword ptr [ebp-94h]
429685  call    __CIcos
42968A  fstp    [ebp+var_A4]
429690  fld     qword ptr [ebp-94h]
429696  call    __CIsin
42969B  mov     edx, edi
42969D  mov     dword ptr [ebp+var_94+4], edx
4296A3  fild    dword ptr [ebp+var_94+4]
4296A9  test    edx, edx
4296AB  jns     short loc_4296B3
4296AD  fadd    ds:dbl_4EC908
4296B3  fld     ds:dbl_4EC968
4296B9  mov     eax, esi
4296BB  mov     dword ptr [ebp+var_94+4], eax
4296C1  fmul    st(1), st
4296C3  fild    dword ptr [ebp+var_94+4]
4296C9  test    eax, eax
4296CB  jns     short loc_4296D3
4296CD  fadd    ds:dbl_4EC908
4296D3  fmulp   st(1), st
4296D5  fld     [ebp+var_CC]
4296DB  fld     st
4296DD  fchs
4296DF  fld     st
4296E1  fld     [ebp+var_BC]
4296E7  fsubrp  st(3), st
4296E9  fld1
4296EB  fsub    st(3), st
4296ED  fxch    st(3)
4296EF  fst     [ebp+var_84]
4296F5  fxch    st(2)
4296F7  fstp    [ebp+var_7C]
4296FA  fld     [ebp+var_B4]
429700  fld     [ebp+var_C4]
429706  fsubr   st, st(1)
429708  faddp   st(4), st
42970A  fxch    st(3)
42970C  fst     [ebp+var_5C]
42970F  fstp    [ebp+var_54]
429712  fxch    st(1)
429714  fstp    [ebp+var_74]
429717  fmul    st, st(3)
429719  fld     st(2)
42971B  fmulp   st(2), st
42971D  fld     st(1)
42971F  fst     [ebp+var_6C]
429722  fld     st(1)
429724  fld     [ebp+var_A4]
42972A  fmul    st(1), st
42972C  fxch    st(2)
42972E  fmul    st, st(7)
429730  fsubp   st(1), st
429732  fstp    [ebp+var_4C]
429735  fxch    st(1)
429737  fmul    st, st(5)
429739  fld     [ebp+var_6C]
42973C  fmul    st, st(2)
42973E  faddp   st(1), st
429740  fstp    [ebp+var_2C]
429743  fld     [ebp+var_84]
429749  fmul    st, st(4)
42974B  fld     st
42974D  fmul    st, st(2)
42974F  fld     st(3)
429751  fmul    st, st(7)
429753  fsubp   st(1), st
429755  fstp    [ebp+var_44]
429758  fxch    st(2)
42975A  fmul    st, st(1)
42975C  fxch    st(2)
42975E  fmul    st, st(5)
429760  faddp   st(2), st
429762  fxch    st(1)
429764  fstp    [ebp+var_24]
429767  fld     [ebp+var_7C]
42976A  fmul    st, st(3)
42976C  fld     [ebp+var_5C]
42976F  fmul    st, st(3)
429771  fld     st(1)
429773  fmul    st, st(3)
429775  fld     st(1)
429777  fmul    st, st(7)
429779  fsubp   st(1), st
42977B  fxch    st(2)
42977D  fmul    st, st(6)
42977F  fxch    st(1)
429781  fmul    st, st(3)
429783  faddp   st(1), st
429785  fstp    [ebp+var_1C]
429788  fld     [ebp+var_74]
42978B  fmulp   st(4), st
42978D  fld     [ebp+var_54]
429790  fmulp   st(3), st
429792  fld     st(3)
429794  fmul    st, st(2)
429796  fld     st(3)
429798  fmul    st, st(6)
42979A  fsubp   st(1), st
42979C  fstp    [ebp+var_34]
42979F  fxch    st(3)
4297A1  fmulp   st(4), st
4297A3  fmulp   st(1), st
4297A5  faddp   st(2), st
4297A7  fxch    st(1)
4297A9  fstp    [ebp+var_14]
4297AC  fld     ds:dbl_4EC998
4297B2  fld     st
4297B4  fld     ds:dbl_4EC990
4297BA  fld     st
4297BC  fxch    st(1)
4297BE  fst     [ebp+var_A4]
4297C4  fld     st(3)
4297C6  fld     [ebp+var_4C]
4297C9  fcom    st(5)
4297CB  fnstsw  ax
4297CD  test    ah, 5
4297D0  jp      short loc_4297D8
4297D2  fstp    st(4)
4297D4  fld     st(3)
4297D6  fxch    st(4)
4297D8  fcom    st(2)
4297DA  fnstsw  ax
4297DC  test    ah, 41h
4297DF  jnz     short loc_4297E5
4297E1  fstp    st(3)
4297E3  jmp     short loc_4297E7
4297E5  fstp    st
4297E7  fld     [ebp+var_2C]
4297EA  fcom    st(2)
4297EC  fnstsw  ax
4297EE  fstp    st(2)
4297F0  test    ah, 41h
4297F3  jnz     short loc_4297F9
4297F5  fld     st(1)
4297F7  jmp     short loc_4297FF
4297F9  fld     [ebp+var_A4]
4297FF  fxch    st(5)
429801  fcomp   st(2)
429803  fnstsw  ax
429805  test    ah, 41h
429808  jnz     short loc_42980E
42980A  fstp    st
42980C  jmp     short loc_429810
42980E  fstp    st(1)
429810  fld     [ebp+var_44]
429813  fcom    st(3)
429815  fnstsw  ax
429817  test    ah, 5
42981A  jp      short loc_429822
42981C  fstp    st(3)
42981E  fld     st(2)
429820  fxch    st(3)
429822  fcom    st(2)
429824  fnstsw  ax
429826  test    ah, 41h
429829  jnz     short loc_42982F
42982B  fstp    st(2)
42982D  jmp     short loc_429831
42982F  fstp    st
429831  fld     [ebp+var_24]
429834  fcom    st(4)
429836  fnstsw  ax
429838  test    ah, 41h
42983B  jnz     short loc_429843
42983D  fstp    st(4)
42983F  fld     st(3)
429841  fxch    st(4)
429843  fcom    st(1)
429845  fnstsw  ax
429847  test    ah, 5
42984A  jp      short loc_429850
42984C  fstp    st(1)
42984E  jmp     short loc_429852
429850  fstp    st
429852  fxch    st(2)
429854  fcom    st(4)
429856  fnstsw  ax
429858  test    ah, 41h
42985B  jnz     short loc_429861
42985D  fstp    st
42985F  fld     st(3)
429861  fxch    st(1)
429863  fcom    st(4)
429865  fnstsw  ax
429867  test    ah, 5
42986A  jp      short loc_429870
42986C  fstp    st
42986E  jmp     short loc_429872
429870  fstp    st(4)
429872  fld     [ebp+var_1C]
429875  fcom    st(3)
429877  fnstsw  ax
429879  test    ah, 41h
42987C  jnz     short loc_429884
42987E  fstp    st(3)
429880  fld     st(2)
429882  fxch    st(3)
429884  fcom    st(2)
429886  fnstsw  ax
429888  test    ah, 5
42988B  jp      short loc_429891
42988D  fstp    st(2)
42988F  jmp     short loc_429893
429891  fstp    st
429893  fld     [ebp+var_34]
429896  fcom    st(1)
429898  fnstsw  ax
42989A  test    ah, 5
42989D  jp      short loc_4298A5
42989F  fstp    st(1)
4298A1  fld     st
4298A3  fxch    st(1)
4298A5  fcom    st(4)
4298A7  fnstsw  ax
4298A9  test    ah, 41h
4298AC  jnz     short loc_4298B2
4298AE  fstp    st(4)
4298B0  jmp     short loc_4298B4
4298B2  fstp    st
4298B4  fld     [ebp+var_14]
4298B7  fcom    st(3)
4298B9  fnstsw  ax
4298BB  test    ah, 41h
4298BE  jnz     short loc_4298C6
4298C0  fstp    st(3)
4298C2  fld     st(2)
4298C4  fxch    st(3)
4298C6  fcom    st(2)
4298C8  fnstsw  ax
4298CA  test    ah, 5
4298CD  jp      short loc_4298D3
4298CF  fstp    st(2)
4298D1  jmp     short loc_4298D5
4298D3  fstp    st
4298D5  mov     eax, [ebp+arg_1C]
4298D8  cdq
4298D9  push    edx
4298DA  push    eax
4298DB  push    0
4298DD  push    edi
4298DE  call    __allmul
4298E3  shrd    eax, edx, 10h
4298E7  and     eax, 0FFFFh
4298EC  mov     dword ptr [ebp+var_94], eax
4298F2  mov     eax, [ebp+arg_20]
4298F5  xor     edx, edx
4298F7  mov     dword ptr [ebp+var_94+4], edx
4298FD  cdq
4298FE  fild    [ebp+var_94]
429904  push    edx
429905  fld     ds:dbl_4EC968
42990B  push    eax
42990C  push    0
42990E  fmul    st(1), st
429910  push    esi
429911  fxch    st(1)
429913  faddp   st(5), st
429915  call    __allmul
42991A  shrd    eax, edx, 10h
42991E  and     eax, 0FFFFh
429923  xor     edx, edx
429925  mov     dword ptr [ebp+var_94], eax
42992B  mov     dword ptr [ebp+var_94+4], edx
429931  fild    [ebp+var_94]
429937  fmulp   st(1), st
429939  fsubp   st(2), st
42993B  fldz
42993D  fcom    st(1)
42993F  fnstsw  ax
429941  fld     ds:dbl_4EC930
429947  test    ah, 41h
42994A  jp      short loc_429957
42994C  fxch    st(2)
42994E  call    __ftol2_sse
429953  mov     edi, eax
429955  jmp     short loc_42997C
429957  fld     st(2)
429959  fmul    st, st(1)
42995B  call    __ftol2_sse
429960  fxch    st(2)
429962  mov     esi, eax
429964  and     esi, 0FFFFh
42996A  add     esi, 0FFFFh
429970  sar     esi, 10h
429973  call    __ftol2_sse
429978  mov     edi, eax
42997A  sub     edi, esi
42997C  fcom    st(4)
42997E  fnstsw  ax
429980  test    ah, 41h
429983  jp      short loc_429990
429985  fxch    st(4)
429987  call    __ftol2_sse
42998C  mov     esi, eax
42998E  jmp     short loc_4299B5
429990  fld     st(4)
429992  fmul    st, st(2)
429994  call    __ftol2_sse
429999  fxch    st(4)
42999B  mov     ebx, eax
42999D  and     ebx, 0FFFFh
4299A3  add     ebx, 0FFFFh
4299A9  sar     ebx, 10h
4299AC  call    __ftol2_sse
4299B1  mov     esi, eax
4299B3  sub     esi, ebx
4299B5  fxch    st(2)
4299B7  fcom    st(3)
4299B9  fnstsw  ax
4299BB  test    ah, 5
4299BE  jp      short loc_4299C9
4299C0  call    __ftol2_sse
4299C5  mov     ebx, eax
4299C7  jmp     short loc_4299EA
4299C9  fld     st
4299CB  fmul    st, st(3)
4299CD  call    __ftol2_sse
4299D2  mov     ebx, eax
4299D4  and     ebx, 0FFFFh
4299DA  add     ebx, 0FFFFh
4299E0  sar     ebx, 10h
4299E3  call    __ftol2_sse
4299E8  add     ebx, eax
4299EA  fcom    st(2)
4299EC  fnstsw  ax
4299EE  fstp    st(2)
4299F0  test    ah, 5
4299F3  jp      short loc_4299FE
4299F5  fstp    st
4299F7  call    __ftol2_sse
4299FC  jmp     short loc_429A27
4299FE  fmul    st, st(1)
429A00  call    __ftol2_sse
429A05  and     eax, 0FFFFh
429A0A  add     eax, 0FFFFh
429A0F  sar     eax, 10h
429A12  mov     dword ptr [ebp+var_94+4], eax
429A18  call    __ftol2_sse
429A1D  mov     ecx, dword ptr [ebp+var_94+4]
429A23  add     ecx, eax
429A25  mov     eax, ecx
429A27  mov     edx, [ebp+var_A8]
429A2D  sub     esi, edi
429A2F  inc     esi
429A30  mov     [edx], esi
429A32  mov     edx, [ebp+var_98]
429A38  mov     ecx, ebx
429A3A  sub     ecx, eax
429A3C  mov     eax, [ebp+var_9C]
429A42  inc     ecx
429A43  mov     [edx], ecx
429A45  mov     ecx, [ebp+var_8]
429A48  neg     edi
429A4A  mov     [eax], edi
429A4C  pop     edi
429A4D  pop     esi
429A4E  mov     [eax+4], ebx
429A51  xor     ecx, ebp
429A53  pop     ebx
429A54  call    sub_4ABDD5
429A59  mov     esp, ebp
429A5B  pop     ebp
429A5C  retn    2Ch ; ','

;==== 429A60 sub_429A60
429A60  push    ebp
429A61  mov     ebp, esp
429A63  sub     esp, 8
429A66  push    ebx
429A67  push    esi
429A68  mov     esi, eax
429A6A  push    edi
429A6B  mov     eax, 1
429A70  mov     ecx, esi
429A72  call    sub_41BAB0
429A77  mov     edi, eax
429A79  mov     eax, [esi+2C8h]
429A7F  xor     ebx, ebx
429A81  push    ebx
429A82  cdq
429A83  push    edi
429A84  push    edx
429A85  push    eax
429A86  mov     [ebp+var_8], edi
429A89  call    __allmul
429A8E  mov     ecx, [ebp+arg_0]
429A91  shrd    eax, edx, 18h
429A95  add     eax, dword ptr [ebp+arg_24]
429A98  push    ebx
429A99  mov     [ecx], eax
429A9B  mov     eax, [esi+2CCh]
429AA1  sar     edx, 18h
429AA4  cdq
429AA5  push    edi
429AA6  push    edx
429AA7  push    eax
429AA8  call    __allmul
429AAD  mov     ecx, [esi+2ECh]
429AB3  shrd    eax, edx, 18h
429AB7  add     eax, dword ptr [ebp+arg_24+4]
429ABA  sar     edx, 18h
429ABD  mov     edx, [ebp+arg_0]
429AC0  push    edi
429AC1  mov     [edx+4], eax
429AC4  call    sub_41A9D0
429AC9  imul    dword ptr [esi+2D0h]
429ACF  mov     ecx, [ebp+arg_4]
429AD2  shrd    eax, edx, 10h
429AD6  add     eax, [ebp+arg_10]
429AD9  add     esp, 4
429ADC  mov     [ecx], eax
429ADE  mov     eax, [esi+2E8h]
429AE4  sar     edx, 10h
429AE7  push    ebx
429AE8  cdq
429AE9  push    edi
429AEA  cmp     [esi+290h], ebx
429AF0  jz      loc_429B7C
429AF6  mov     ecx, eax
429AF8  mov     eax, [esi+2E0h]
429AFE  mov     dword ptr [ebp+arg_24+4], edx
429B01  cdq
429B02  add     eax, ecx
429B04  adc     edx, dword ptr [ebp+arg_24+4]
429B07  mov     dword ptr [ebp+arg_24], ecx
429B0A  push    edx
429B0B  push    eax
429B0C  call    __allmul
429B11  shrd    eax, edx, 18h
429B15  xor     ecx, ecx
429B17  sar     edx, 18h
429B1A  add     eax, [ebp+arg_1C]
429B1D  push    ecx
429B1E  adc     edx, ecx
429B20  mov     ecx, [ebp+arg_14]
429B23  push    ecx
429B24  push    edx
429B25  push    eax
429B26  call    __allmul
429B2B  shrd    eax, edx, 10h
429B2F  sar     edx, 10h
429B32  mov     edx, [ebp+arg_8]
429B35  mov     [edx], eax
429B37  mov     eax, [esi+2E4h]
429B3D  cdq
429B3E  add     eax, dword ptr [ebp+arg_24]
429B41  push    ebx
429B42  adc     edx, dword ptr [ebp+arg_24+4]
429B45  push    edi
429B46  push    edx
429B47  push    eax
429B48  call    __allmul
429B4D  shrd    eax, edx, 18h
429B51  xor     ecx, ecx
429B53  sar     edx, 18h
429B56  add     eax, [ebp+arg_20]
429B59  push    ecx
429B5A  adc     edx, ecx
429B5C  mov     ecx, [ebp+arg_18]
429B5F  push    ecx
429B60  push    edx
429B61  push    eax
429B62  call    __allmul
429B67  shrd    eax, edx, 10h
429B6B  sar     edx, 10h
429B6E  mov     edx, [ebp+arg_C]
429B71  mov     [edx], eax
429B73  pop     edi
429B74  pop     esi
429B75  pop     ebx
429B76  mov     esp, ebp
429B78  pop     ebp
429B79  retn    2Ch ; ','
429B7C  push    edx
429B7D  push    eax
429B7E  call    __allmul
429B83  shrd    eax, edx, 18h
429B87  mov     dword ptr [ebp+arg_24], eax
429B8A  mov     eax, [esi+2E0h]
429B90  sar     edx, 18h
429B93  push    ebx
429B94  mov     dword ptr [ebp+arg_24+4], edx
429B97  cdq
429B98  push    edi
429B99  push    edx
429B9A  push    eax
429B9B  call    __allmul
429BA0  mov     edi, dword ptr [ebp+arg_24+4]
429BA3  shrd    eax, edx, 18h
429BA7  mov     ecx, eax
429BA9  xor     eax, eax
429BAB  sar     edx, 18h
429BAE  add     ecx, [ebp+arg_14]
429BB1  adc     edx, eax
429BB3  mov     eax, [ebp+arg_1C]
429BB6  xor     ebx, ebx
429BB8  add     eax, dword ptr [ebp+arg_24]
429BBB  adc     ebx, edi
429BBD  push    ebx
429BBE  push    eax
429BBF  push    edx
429BC0  push    ecx
429BC1  call    __allmul
429BC6  mov     ecx, [ebp+arg_8]
429BC9  shrd    eax, edx, 10h
429BCD  mov     [ecx], eax
429BCF  mov     eax, [esi+2E4h]
429BD5  xor     ecx, ecx
429BD7  push    ecx
429BD8  mov     ecx, [ebp+var_8]
429BDB  sar     edx, 10h
429BDE  cdq
429BDF  push    ecx
429BE0  push    edx
429BE1  push    eax
429BE2  call    __allmul
429BE7  shrd    eax, edx, 18h
429BEB  mov     ecx, eax
429BED  xor     eax, eax
429BEF  sar     edx, 18h
429BF2  add     ecx, [ebp+arg_18]
429BF5  adc     edx, eax
429BF7  mov     eax, [ebp+arg_20]
429BFA  xor     esi, esi
429BFC  add     eax, dword ptr [ebp+arg_24]
429BFF  adc     esi, edi
429C01  push    esi
429C02  push    eax
429C03  push    edx
429C04  push    ecx
429C05  call    __allmul
429C0A  mov     ecx, [ebp+arg_C]
429C0D  shrd    eax, edx, 10h
429C11  pop     edi
429C12  pop     esi
429C13  sar     edx, 10h
429C16  mov     [ecx], eax
429C18  pop     ebx
429C19  mov     esp, ebp
429C1B  pop     ebp
429C1C  retn    2Ch ; ','

;==== 429C20 sub_429C20
429C20  push    ebp
429C21  mov     ebp, esp
429C23  mov     ecx, [edi+16Ch]
429C29  mov     edx, dword_565B40
429C2F  sub     esp, 38h
429C32  push    ebx
429C33  push    esi
429C34  lea     esi, [ebp+var_38]
429C37  call    sub_407F10
429C3C  test    eax, eax
429C3E  jz      loc_429D35
429C44  mov     ecx, [edi+16Ch]
429C4A  mov     edx, dword_565B40
429C50  call    sub_4082F0
429C55  cmp     [edi+174h], eax
429C5B  jnz     loc_429D35
429C61  mov     eax, [edi+268h]
429C67  mov     ecx, [edi+264h]
429C6D  mov     edx, [edi+28Ch]
429C73  push    eax
429C74  mov     eax, [edi+288h]
429C7A  push    ecx
429C7B  mov     ecx, [edi+284h]
429C81  push    edx
429C82  mov     edx, [edi+280h]
429C88  push    eax
429C89  mov     eax, [edi+26Ch]
429C8F  push    ecx
429C90  push    edx
429C91  push    eax
429C92  lea     ecx, [ebp+var_C]
429C95  push    ecx
429C96  lea     edx, [ebp+var_8]
429C99  push    edx
429C9A  lea     eax, [ebp+var_4]
429C9D  push    eax
429C9E  lea     ecx, [ebp+var_18]
429CA1  push    ecx
429CA2  mov     eax, edi
429CA4  call    sub_429A60
429CA9  mov     esi, [ebp+var_14]
429CAC  mov     ebx, [ebp+var_18]
429CAF  mov     edx, [ebp+var_8]
429CB2  mov     eax, [ebp+var_4]
429CB5  mov     ecx, [ebp+var_2C]
429CB8  push    esi
429CB9  push    ebx
429CBA  push    edx
429CBB  mov     edx, [ebp+var_30]
429CBE  push    eax
429CBF  push    ecx
429CC0  push    edx
429CC1  lea     eax, [ebp+var_20]
429CC4  push    eax
429CC5  lea     ecx, [ebp+var_14]
429CC8  push    ecx
429CC9  lea     edx, [ebp+var_10]
429CCC  push    edx
429CCD  mov     edx, [ebp+var_C]
429CD0  call    sub_429580
429CD5  mov     eax, [ebp+var_10]
429CD8  cmp     eax, 2
429CDB  jb      short loc_429D35
429CDD  mov     ecx, [ebp+var_14]
429CE0  cmp     ecx, 2
429CE3  jb      short loc_429D35
429CE5  mov     edx, [ebp+var_4]
429CE8  mov     [edi+2A8h], edx
429CEE  mov     edx, [ebp+var_8]
429CF1  mov     [edi+2B8h], edx
429CF7  mov     edx, [ebp+var_C]
429CFA  mov     [edi+2BCh], edx
429D00  mov     edx, [ebp+var_20]
429D03  mov     [edi+2F8h], edx
429D09  mov     edx, [ebp+var_1C]
429D0C  push    ecx
429D0D  mov     [edi+2FCh], edx
429D13  mov     edx, [edi]
429D15  mov     [edi+2F0h], eax
429D1B  mov     [edi+2F4h], ecx
429D21  push    eax
429D22  mov     eax, [edx+74h]
429D25  mov     ecx, edi
429D27  mov     [edi+2A0h], ebx
429D2D  mov     [edi+2A4h], esi
429D33  call    eax
429D35  pop     esi
429D36  pop     ebx
429D37  mov     esp, ebp
429D39  pop     ebp
429D3A  retn

;==== 429D40 sub_429D40
429D40  push    ebx
429D41  mov     ebx, esp
429D43  sub     esp, 8
429D46  and     esp, 0FFFFFFF0h
429D49  add     esp, 4
429D4C  push    ebp
429D4D  mov     ebp, [ebx+4]
429D50  mov     [esp+0Ch+var_8], ebp
429D54  mov     ebp, esp
429D56  mov     edx, dword_565B40
429D5C  sub     esp, 48h
429D5F  push    esi
429D60  push    edi
429D61  mov     edi, eax
429D63  mov     ecx, [edi+16Ch]
429D69  lea     esi, [ebp-48h]
429D6C  call    sub_407F10
429D71  test    eax, eax
429D73  jz      loc_429E9A
429D79  lea     esi, [ebp-30h]
429D7C  call    sub_41B7F0
429D81  mov     eax, edi
429D83  call    sub_41C1F0
429D88  test    eax, eax
429D8A  jz      short loc_429D90
429D8C  xor     esi, esi
429D8E  jmp     short loc_429D96
429D90  mov     esi, [edi+294h]
429D96  lea     eax, [ebp-10h]
429D99  push    eax
429D9A  mov     eax, [ebp-28h]
429D9D  call    sub_41ADE0
429DA2  mov     ecx, [edi+268h]
429DA8  mov     edx, [edi+264h]
429DAE  mov     eax, [edi+28Ch]
429DB4  add     esp, 4
429DB7  push    ecx
429DB8  mov     ecx, [edi+288h]
429DBE  push    edx
429DBF  mov     edx, [edi+26Ch]
429DC5  push    eax
429DC6  mov     eax, [ebp-10h]
429DC9  push    ecx
429DCA  push    eax
429DCB  push    eax
429DCC  push    edx
429DCD  lea     eax, [ebp-0Ch]
429DD0  push    eax
429DD1  lea     ecx, [ebp-8]
429DD4  push    ecx
429DD5  lea     edx, [ebp-4]
429DD8  push    edx
429DD9  lea     eax, [ebp-1Ch]
429DDC  push    eax
429DDD  mov     eax, edi
429DDF  call    sub_429A60
429DE4  cmp     dword ptr [edi+31Ch], 0
429DEB  jbe     short loc_429DF5
429DED  mov     eax, [edi+324h]
429DF3  jmp     short loc_429DF7
429DF5  xor     eax, eax
429DF7  mov     ecx, [ebp-18h]
429DFA  mov     edx, [ebp-1Ch]
429DFD  push    ecx
429DFE  mov     ecx, [ebp-2Ch]
429E01  push    edx
429E02  mov     edx, [ebp-30h]
429E05  push    ecx
429E06  mov     ecx, [ebp-0Ch]
429E09  push    edx
429E0A  mov     edx, [ebp-8]
429E0D  push    ecx
429E0E  mov     ecx, [ebp-4]
429E11  push    edx
429E12  mov     edx, [ebp-3Ch]
429E15  push    ecx
429E16  push    eax
429E17  mov     eax, [ebp-40h]
429E1A  push    edx
429E1B  push    eax
429E1C  lea     ecx, [ebp-28h]
429E1F  push    ecx
429E20  lea     edx, [ebp-10h]
429E23  lea     ecx, [ebp-14h]
429E26  call    sub_4295C0
429E2B  mov     esi, [ebp-14h]
429E2E  cmp     esi, 2
429E31  jb      short loc_429E9A
429E33  mov     eax, [ebp-10h]
429E36  cmp     eax, 2
429E39  jb      short loc_429E9A
429E3B  mov     edx, [ebp-1Ch]
429E3E  mov     ecx, [ebp-18h]
429E41  mov     [edi+2A0h], edx
429E47  mov     edx, [ebp-4]
429E4A  mov     [edi+2A4h], ecx
429E50  mov     ecx, [ebp-8]
429E53  mov     [edi+2A8h], edx
429E59  mov     edx, [ebp-0Ch]
429E5C  mov     [edi+2B8h], ecx
429E62  mov     ecx, [ebp-24h]
429E65  mov     [edi+2F4h], eax
429E6B  mov     eax, [ebp-28h]
429E6E  push    edi
429E6F  mov     [edi+2BCh], edx
429E75  mov     [edi+2F0h], esi
429E7B  mov     [edi+2F8h], eax
429E81  mov     [edi+2FCh], ecx
429E87  call    sub_429EB0
429E8C  mov     eax, [ebp-10h]
429E8F  mov     edx, [edi]
429E91  mov     edx, [edx+74h]
429E94  push    eax
429E95  push    esi
429E96  mov     ecx, edi
429E98  call    edx
429E9A  pop     edi
429E9B  pop     esi
429E9C  mov     esp, ebp
429E9E  pop     ebp
429E9F  mov     esp, ebx
429EA1  pop     ebx
429EA2  retn

;==== 429EB0 sub_429EB0
429EB0  push    ebx
429EB1  mov     ebx, esp
429EB3  sub     esp, 8
429EB6  and     esp, 0FFFFFFF0h
429EB9  add     esp, 4
429EBC  push    ebp
429EBD  mov     ebp, [ebx+4]
429EC0  mov     [esp+0Ch+var_8], ebp
429EC4  mov     ebp, esp
429EC6  sub     esp, 38h
429EC9  push    esi
429ECA  push    edi
429ECB  mov     edi, [ebx+8]
429ECE  lea     esi, [ebp-20h]
429ED1  call    sub_41B7F0
429ED6  lea     eax, [ebp-38h]
429ED9  call    sub_41C390
429EDE  mov     edx, [ebp-2Ch]
429EE1  mov     ecx, [ebx+8]
429EE4  mov     edi, [ebp-30h]
429EE7  shr     edx, 1
429EE9  mov     eax, ecx
429EEB  shr     edi, 1
429EED  mov     [ebp-4], edx
429EF0  call    sub_41C3B0
429EF5  test    eax, eax
429EF7  jz      short loc_429F18
429EF9  lea     esi, [ebp-0Ch]
429EFC  call    sub_41C3C0
429F01  mov     ecx, [ebx+8]
429F04  test    eax, eax
429F06  jz      short loc_429F15
429F08  mov     eax, [ebp-8]
429F0B  mov     edi, [ebp-0Ch]
429F0E  mov     [ebp-4], eax
429F11  mov     edx, eax
429F13  jmp     short loc_429F18
429F15  mov     edx, [ebp-4]
429F18  cmp     dword ptr [ecx+298h], 0
429F1F  jz      short loc_429F7D
429F21  mov     esi, [ecx+294h]
429F27  mov     eax, [ebp-18h]
429F2A  lea     edx, [ebp-8]
429F2D  push    edx
429F2E  call    sub_41ADE0
429F33  mov     ecx, [ebp-8]
429F36  mov     eax, [ebp-20h]
429F39  add     esp, 4
429F3C  xor     esi, esi
429F3E  push    esi
429F3F  cdq
429F40  push    ecx
429F41  push    edx
429F42  push    eax
429F43  call    __allmul
429F48  shrd    eax, edx, 10h
429F4C  xor     ecx, ecx
429F4E  push    ecx
429F4F  mov     ecx, [ebp-8]
429F52  shl     edi, 10h
429F55  lea     esi, [edi+eax]
429F58  mov     eax, [ebp-1Ch]
429F5B  sar     edx, 10h
429F5E  cdq
429F5F  push    ecx
429F60  push    edx
429F61  push    eax
429F62  call    __allmul
429F67  mov     ecx, [ebx+8]
429F6A  shrd    eax, edx, 10h
429F6E  mov     edi, eax
429F70  mov     eax, [ebp-4]
429F73  shl     eax, 10h
429F76  sar     edx, 10h
429F79  add     edi, eax
429F7B  jmp     short loc_429F8D
429F7D  shl     edi, 10h
429F80  add     edi, [ebp-20h]
429F83  shl     edx, 10h
429F86  add     edx, [ebp-1Ch]
429F89  mov     esi, edi
429F8B  mov     edi, edx
429F8D  mov     eax, [ecx]
429F8F  mov     eax, [eax+28h]
429F92  push    0
429F94  push    0
429F96  mov     edx, edi
429F98  sar     edx, 10h
429F9B  push    edx
429F9C  mov     edx, esi
429F9E  sar     edx, 10h
429FA1  push    edx
429FA2  call    eax
429FA4  mov     eax, [ebx+8]
429FA7  and     esi, 0FFFFh
429FAD  and     edi, 0FFFFh
429FB3  mov     [eax+2C4h], edi
429FB9  pop     edi
429FBA  mov     [eax+2C0h], esi
429FC0  pop     esi
429FC1  mov     esp, ebp
429FC3  pop     ebp
429FC4  mov     esp, ebx
429FC6  pop     ebx
429FC7  retn    4

;==== 429FD0 sub_429FD0
429FD0  push    ebp
429FD1  mov     ebp, esp
429FD3  sub     esp, 10h
429FD6  push    ebx
429FD7  push    esi
429FD8  mov     esi, eax
429FDA  push    edi
429FDB  mov     eax, 1
429FE0  mov     ecx, esi
429FE2  call    sub_41BAB0
429FE7  mov     edi, eax
429FE9  mov     eax, [esi+2C8h]
429FEF  xor     ebx, ebx
429FF1  push    ebx
429FF2  cdq
429FF3  push    edi
429FF4  push    edx
429FF5  push    eax
429FF6  mov     [ebp+var_10], edi
429FF9  call    __allmul
429FFE  mov     ecx, [ebp+arg_18]
42A001  shrd    eax, edx, 18h
42A005  add     eax, [ecx]
42A007  sar     edx, 18h
42A00A  mov     edx, [ebp+arg_0]
42A00D  mov     [edx], eax
42A00F  mov     eax, [esi+2CCh]
42A015  push    ebx
42A016  cdq
42A017  push    edi
42A018  push    edx
42A019  push    eax
42A01A  call    __allmul
42A01F  mov     ecx, [ebp+arg_18]
42A022  shrd    eax, edx, 18h
42A026  add     eax, [ecx+4]
42A029  mov     ecx, [esi+2ECh]
42A02F  sar     edx, 18h
42A032  mov     edx, [ebp+arg_0]
42A035  push    edi
42A036  mov     [edx+4], eax
42A039  call    sub_41A9D0
42A03E  cdq
42A03F  add     esp, 4
42A042  mov     ecx, edx
42A044  mov     dword ptr [ebp+var_8], eax
42A047  mov     eax, [esi+2D4h]
42A04D  push    ecx
42A04E  mov     dword ptr [ebp+var_8+4], ecx
42A051  mov     ecx, dword ptr [ebp+var_8]
42A054  cdq
42A055  push    ecx
42A056  push    edx
42A057  push    eax
42A058  call    __allmul
42A05D  mov     ecx, dword ptr [ebp+var_8+4]
42A060  shrd    eax, edx, 10h
42A064  add     eax, [ebp+arg_1C]
42A067  sar     edx, 10h
42A06A  mov     edx, [ebp+arg_4]
42A06D  push    ecx
42A06E  mov     ecx, dword ptr [ebp+var_8]
42A071  mov     [edx], eax
42A073  mov     eax, [esi+2D8h]
42A079  cdq
42A07A  push    ecx
42A07B  push    edx
42A07C  push    eax
42A07D  call    __allmul
42A082  mov     ecx, dword ptr [ebp+var_8+4]
42A085  shrd    eax, edx, 10h
42A089  add     eax, [ebp+arg_20]
42A08C  sar     edx, 10h
42A08F  mov     edx, [ebp+arg_8]
42A092  push    ecx
42A093  mov     ecx, dword ptr [ebp+var_8]
42A096  mov     [edx], eax
42A098  mov     eax, [esi+2DCh]
42A09E  cdq
42A09F  push    ecx
42A0A0  push    edx
42A0A1  push    eax
42A0A2  call    __allmul
42A0A7  shrd    eax, edx, 10h
42A0AB  add     eax, [ebp+arg_24]
42A0AE  sar     edx, 10h
42A0B1  mov     edx, [ebp+arg_C]
42A0B4  mov     [edx], eax
42A0B6  mov     eax, [esi+2E8h]
42A0BC  push    ebx
42A0BD  cdq
42A0BE  push    edi
42A0BF  cmp     [esi+290h], ebx
42A0C5  jz      loc_42A151
42A0CB  mov     ecx, eax
42A0CD  mov     eax, [esi+2E0h]
42A0D3  mov     dword ptr [ebp+var_8+4], edx
42A0D6  cdq
42A0D7  add     eax, ecx
42A0D9  adc     edx, dword ptr [ebp+var_8+4]
42A0DC  mov     dword ptr [ebp+var_8], ecx
42A0DF  push    edx
42A0E0  push    eax
42A0E1  call    __allmul
42A0E6  shrd    eax, edx, 18h
42A0EA  xor     ecx, ecx
42A0EC  sar     edx, 18h
42A0EF  add     eax, [ebp+arg_30]
42A0F2  push    ecx
42A0F3  adc     edx, ecx
42A0F5  mov     ecx, [ebp+arg_28]
42A0F8  push    ecx
42A0F9  push    edx
42A0FA  push    eax
42A0FB  call    __allmul
42A100  shrd    eax, edx, 10h
42A104  sar     edx, 10h
42A107  mov     edx, [ebp+arg_10]
42A10A  mov     [edx], eax
42A10C  mov     eax, [esi+2E4h]
42A112  cdq
42A113  add     eax, dword ptr [ebp+var_8]
42A116  push    ebx
42A117  adc     edx, dword ptr [ebp+var_8+4]
42A11A  push    edi
42A11B  push    edx
42A11C  push    eax
42A11D  call    __allmul
42A122  shrd    eax, edx, 18h
42A126  xor     ecx, ecx
42A128  sar     edx, 18h
42A12B  add     eax, [ebp+arg_34]
42A12E  push    ecx
42A12F  adc     edx, ecx
42A131  mov     ecx, [ebp+arg_2C]
42A134  push    ecx
42A135  push    edx
42A136  push    eax
42A137  call    __allmul
42A13C  shrd    eax, edx, 10h
42A140  sar     edx, 10h
42A143  mov     edx, [ebp+arg_14]
42A146  mov     [edx], eax
42A148  pop     edi
42A149  pop     esi
42A14A  pop     ebx
42A14B  mov     esp, ebp
42A14D  pop     ebp
42A14E  retn    38h ; '8'
42A151  push    edx
42A152  push    eax
42A153  call    __allmul
42A158  shrd    eax, edx, 18h
42A15C  mov     dword ptr [ebp+var_8], eax
42A15F  mov     eax, [esi+2E0h]
42A165  sar     edx, 18h
42A168  push    ebx
42A169  mov     dword ptr [ebp+var_8+4], edx
42A16C  cdq
42A16D  push    edi
42A16E  push    edx
42A16F  push    eax
42A170  call    __allmul
42A175  mov     edi, dword ptr [ebp+var_8+4]
42A178  shrd    eax, edx, 18h
42A17C  mov     ecx, eax
42A17E  xor     eax, eax
42A180  sar     edx, 18h
42A183  add     ecx, [ebp+arg_28]
42A186  adc     edx, eax
42A188  mov     eax, [ebp+arg_30]
42A18B  xor     ebx, ebx
42A18D  add     eax, dword ptr [ebp+var_8]
42A190  adc     ebx, edi
42A192  push    ebx
42A193  push    eax
42A194  push    edx
42A195  push    ecx
42A196  call    __allmul
42A19B  mov     ecx, [ebp+arg_10]
42A19E  shrd    eax, edx, 10h
42A1A2  mov     [ecx], eax
42A1A4  mov     eax, [esi+2E4h]
42A1AA  xor     ecx, ecx
42A1AC  push    ecx
42A1AD  mov     ecx, [ebp+var_10]
42A1B0  sar     edx, 10h
42A1B3  cdq
42A1B4  push    ecx
42A1B5  push    edx
42A1B6  push    eax
42A1B7  call    __allmul
42A1BC  shrd    eax, edx, 18h
42A1C0  mov     ecx, eax
42A1C2  xor     eax, eax
42A1C4  sar     edx, 18h
42A1C7  add     ecx, [ebp+arg_2C]
42A1CA  adc     edx, eax
42A1CC  mov     eax, [ebp+arg_34]
42A1CF  xor     esi, esi
42A1D1  add     eax, dword ptr [ebp+var_8]
42A1D4  adc     esi, edi
42A1D6  push    esi
42A1D7  push    eax
42A1D8  push    edx
42A1D9  push    ecx
42A1DA  call    __allmul
42A1DF  mov     ecx, [ebp+arg_14]
42A1E2  shrd    eax, edx, 10h
42A1E6  pop     edi
42A1E7  pop     esi
42A1E8  sar     edx, 10h
42A1EB  mov     [ecx], eax
42A1ED  pop     ebx
42A1EE  mov     esp, ebp
42A1F0  pop     ebp
42A1F1  retn    38h ; '8'

;==== 42A200 sub_42A200
42A200  push    ebp
42A201  mov     ebp, esp
42A203  sub     esp, 28h
42A206  push    ebx
42A207  push    esi
42A208  push    edi
42A209  lea     eax, [ebp+var_24]
42A20C  call    sub_41C390
42A211  mov     edi, [ebp+var_1C]
42A214  mov     ebx, [ebp+var_18]
42A217  mov     eax, [ebp+arg_0]
42A21A  shr     edi, 1
42A21C  shr     ebx, 1
42A21E  call    sub_41C3B0
42A223  test    eax, eax
42A225  jz      short loc_42A239
42A227  lea     esi, [ebp+var_C]
42A22A  call    sub_41C3C0
42A22F  test    eax, eax
42A231  jz      short loc_42A239
42A233  mov     edi, [ebp+var_C]
42A236  mov     ebx, [ebp+var_8]
42A239  shl     edi, 10h
42A23C  shl     ebx, 10h
42A23F  cmp     [ebp+arg_10], 0
42A243  jz      short loc_42A272
42A245  mov     eax, [ebp+arg_4]
42A248  mov     ecx, [ebp+arg_8]
42A24B  mov     [eax], edi
42A24D  mov     [eax+4], ebx
42A250  mov     eax, [ebp+arg_C]
42A253  mov     edx, [eax]
42A255  mov     [ecx], edx
42A257  mov     edx, [eax+4]
42A25A  mov     [ecx+4], edx
42A25D  mov     edx, [eax+8]
42A260  mov     eax, [eax+0Ch]
42A263  mov     [ecx+8], edx
42A266  mov     [ecx+0Ch], eax
42A269  pop     edi
42A26A  pop     esi
42A26B  pop     ebx
42A26C  mov     esp, ebp
42A26E  pop     ebp
42A26F  retn    14h
42A272  mov     eax, [ebp+arg_C]
42A275  add     edi, [eax]
42A277  mov     ecx, [ebp+arg_4]
42A27A  add     ebx, [eax+4]
42A27D  mov     edx, [eax+8]
42A280  mov     [ecx], edi
42A282  pop     edi
42A283  mov     [ecx+4], ebx
42A286  mov     ecx, [ebp+arg_8]
42A289  pop     esi
42A28A  mov     dword ptr [ecx], 0
42A290  mov     dword ptr [ecx+4], 0
42A297  mov     [ecx+8], edx
42A29A  pop     ebx
42A29B  mov     esp, ebp
42A29D  pop     ebp
42A29E  retn    14h

;==== 42A2B0 sub_42A2B0
42A2B0  push    ebp
42A2B1  mov     ebp, esp
42A2B3  sub     esp, 2Ch
42A2B6  push    ebx
42A2B7  mov     ebx, [ebp+arg_10]
42A2BA  push    esi
42A2BB  mov     esi, ecx
42A2BD  xor     ecx, ecx
42A2BF  mov     [ebp+arg_10], ecx
42A2C2  mov     dword ptr [ebp+var_8], ecx
42A2C5  cmp     eax, ecx
42A2C7  jz      short loc_42A2D3
42A2C9  mov     edx, [eax]
42A2CB  mov     eax, [eax+4]
42A2CE  mov     [ebp+var_10], edx
42A2D1  jmp     short loc_42A2D8
42A2D3  xor     eax, eax
42A2D5  mov     [ebp+var_10], eax
42A2D8  mov     [ebp+var_C], eax
42A2DB  cmp     [esi+31Ch], ecx
42A2E1  jz      loc_42A36B
42A2E7  cmp     [esi+324h], ecx
42A2ED  jz      short loc_42A36B
42A2EF  push    ecx
42A2F0  lea     ecx, [ebp+var_8]
42A2F3  push    ecx
42A2F4  lea     eax, [ebp+var_28]
42A2F7  mov     ecx, esi
42A2F9  call    sub_42AED0
42A2FE  test    eax, eax
42A300  jz      loc_42A45C
42A306  mov     edx, [esi+328h]
42A30C  mov     [edi], edx
42A30E  mov     eax, [esi+32Ch]
42A314  mov     [edi+4], eax
42A317  mov     ecx, [esi+330h]
42A31D  mov     [edi+8], ecx
42A320  mov     edx, [esi+334h]
42A326  mov     [edi+0Ch], edx
42A329  mov     eax, [esi+338h]
42A32F  mov     edx, [ebp+var_10]
42A332  mov     [edi+10h], eax
42A335  mov     ecx, [esi+33Ch]
42A33B  mov     eax, [ebp+arg_C]
42A33E  mov     [edi+14h], ecx
42A341  mov     ecx, dword ptr [ebp+var_8]
42A344  sar     [ebp+var_C], cl
42A347  sar     edx, cl
42A349  shl     eax, cl
42A34B  shl     ebx, cl
42A34D  mov     ecx, [esi+330h]
42A353  sub     ecx, [ebp+var_20]
42A356  shl     ecx, 0Fh
42A359  and     ecx, 0FFFF0000h
42A35F  add     edx, ecx
42A361  mov     [ebp+var_10], edx
42A364  mov     ecx, 1
42A369  jmp     short loc_42A3E7
42A36B  cmp     dword ptr [esi+170h], 0FFFFFFFFh
42A372  jnz     loc_42A415
42A378  push    ecx
42A379  lea     edx, [ebp+var_8]
42A37C  push    edx
42A37D  lea     eax, [ebp+var_28]
42A380  mov     ecx, esi
42A382  call    sub_42AED0
42A387  test    eax, eax
42A389  jz      short loc_42A3CE
42A38B  mov     ecx, [esi+16Ch]
42A391  mov     edx, dword_565B40
42A397  call    sub_4082F0
42A39C  cmp     [esi+174h], eax
42A3A2  jnz     short loc_42A3CE
42A3A4  mov     eax, [ebp+var_28]
42A3A7  mov     ecx, [ebp+var_24]
42A3AA  mov     edx, [ebp+var_20]
42A3AD  mov     [edi], eax
42A3AF  mov     eax, [ebp+var_1C]
42A3B2  mov     [edi+4], ecx
42A3B5  mov     ecx, [ebp+var_18]
42A3B8  mov     [edi+8], edx
42A3BB  mov     edx, [ebp+var_14]
42A3BE  mov     [edi+0Ch], eax
42A3C1  mov     [edi+10h], ecx
42A3C4  mov     [edi+14h], edx
42A3C7  mov     [ebp+arg_10], 1
42A3CE  mov     ecx, dword ptr [ebp+var_8]
42A3D1  mov     eax, [ebp+arg_C]
42A3D4  sar     [ebp+var_10], cl
42A3D7  sar     [ebp+var_C], cl
42A3DA  shl     eax, cl
42A3DC  shl     ebx, cl
42A3DE  cmp     [ebp+arg_10], 0
42A3E2  jz      short loc_42A45C
42A3E4  mov     ecx, [ebp+arg_10]
42A3E7  mov     edx, [ebp+arg_0]
42A3EA  test    edx, edx
42A3EC  jz      short loc_42A3F9
42A3EE  mov     esi, [ebp+var_10]
42A3F1  mov     [edx], esi
42A3F3  mov     esi, [ebp+var_C]
42A3F6  mov     [edx+4], esi
42A3F9  mov     edx, [ebp+arg_4]
42A3FC  test    edx, edx
42A3FE  jz      short loc_42A402
42A400  mov     [edx], eax
42A402  mov     eax, [ebp+arg_8]
42A405  test    eax, eax
42A407  jz      short loc_42A40B
42A409  mov     [eax], ebx
42A40B  mov     eax, ecx
42A40D  pop     esi
42A40E  pop     ebx
42A40F  mov     esp, ebp
42A411  pop     ebp
42A412  retn    14h
42A415  mov     eax, [esi+23Ch]
42A41B  mov     [edi], eax
42A41D  mov     ecx, [esi+240h]
42A423  mov     [edi+4], ecx
42A426  mov     edx, [esi+244h]
42A42C  mov     [edi+8], edx
42A42F  mov     eax, [esi+248h]
42A435  mov     [edi+0Ch], eax
42A438  mov     ecx, [esi+24Ch]
42A43E  mov     [edi+10h], ecx
42A441  mov     edx, [esi+250h]
42A447  mov     [edi+14h], edx
42A44A  mov     ecx, [esi+258h]
42A450  mov     [ebp+arg_10], 1
42A457  jmp     loc_42A3D1
42A45C  mov     eax, [ebp+arg_10]
42A45F  pop     esi
42A460  pop     ebx
42A461  mov     esp, ebp
42A463  pop     ebp
42A464  retn    14h

;==== 42A470 sub_42A470
42A470  push    ebp
42A471  mov     ebp, esp
42A473  mov     ecx, [edi+16Ch]
42A479  mov     edx, dword_565B40
42A47F  sub     esp, 1Ch
42A482  push    esi
42A483  lea     esi, [ebp+var_18]
42A486  call    sub_407F10
42A48B  test    eax, eax
42A48D  jz      short loc_42A4DC
42A48F  mov     eax, [edi+28Ch]
42A495  mov     ecx, [edi+288h]
42A49B  mov     edx, [edi+284h]
42A4A1  push    eax
42A4A2  mov     eax, [edi+280h]
42A4A8  push    ecx
42A4A9  mov     ecx, [edi+294h]
42A4AF  push    edx
42A4B0  mov     edx, [edi+27Ch]
42A4B6  push    eax
42A4B7  mov     eax, [edi+278h]
42A4BD  push    ecx
42A4BE  mov     ecx, [edi+274h]
42A4C4  push    edx
42A4C5  mov     edx, [edi+270h]
42A4CB  push    eax
42A4CC  push    ecx
42A4CD  push    edx
42A4CE  lea     eax, [edi+264h]
42A4D4  push    eax
42A4D5  mov     ecx, edi
42A4D7  call    sub_42A4F0
42A4DC  pop     esi
42A4DD  mov     esp, ebp
42A4DF  pop     ebp
42A4E0  retn

;==== 42A4F0 sub_42A4F0
42A4F0  push    ebx
42A4F1  mov     ebx, esp
42A4F3  sub     esp, 8
42A4F6  and     esp, 0FFFFFFF0h
42A4F9  add     esp, 4
42A4FC  push    ebp
42A4FD  mov     ebp, [ebx+4]
42A500  mov     [esp+0Ch+var_8], ebp
42A504  mov     ebp, esp
42A506  sub     esp, 0B8h
42A50C  mov     eax, dword_4FB734
42A511  xor     eax, ebp
42A513  mov     [ebp-4], eax
42A516  mov     eax, [ebx+8]
42A519  push    esi
42A51A  push    edi
42A51B  mov     edi, ecx
42A51D  mov     esi, edi
42A51F  mov     [ebp-64h], edi
42A522  mov     [ebp-5Ch], eax
42A525  call    sub_429550
42A52A  lea     esi, [ebp-0B0h]
42A530  call    sub_41B7F0
42A535  mov     ecx, [ebx+2Ch]
42A538  mov     edx, [ebx+28h]
42A53B  mov     eax, [ebx+24h]
42A53E  push    ecx
42A53F  mov     ecx, [ebx+20h]
42A542  push    edx
42A543  mov     edx, [ebx+14h]
42A546  push    eax
42A547  mov     eax, [ebx+10h]
42A54A  push    ecx
42A54B  mov     ecx, [ebx+0Ch]
42A54E  push    edx
42A54F  mov     edx, [ebp-5Ch]
42A552  push    eax
42A553  push    ecx
42A554  push    edx
42A555  lea     eax, [ebp-68h]
42A558  push    eax
42A559  lea     ecx, [ebp-5Ch]
42A55C  push    ecx
42A55D  lea     edx, [ebp-74h]
42A560  push    edx
42A561  lea     eax, [ebp-78h]
42A564  push    eax
42A565  lea     ecx, [ebp-60h]
42A568  push    ecx
42A569  lea     edx, [ebp-70h]
42A56C  mov     esi, edi
42A56E  push    edx
42A56F  mov     eax, esi
42A571  call    sub_429FD0
42A576  mov     eax, [esi+298h]
42A57C  push    eax
42A57D  lea     ecx, [ebp-0B0h]
42A583  push    ecx
42A584  lea     edx, [ebp-90h]
42A58A  push    edx
42A58B  lea     eax, [ebp-80h]
42A58E  push    eax
42A58F  push    esi
42A590  call    sub_42A200
42A595  mov     ecx, [ebp-68h]
42A598  mov     edx, [ebp-5Ch]
42A59B  push    ecx
42A59C  push    edx
42A59D  lea     eax, [ebp-68h]
42A5A0  push    eax
42A5A1  lea     ecx, [ebp-5Ch]
42A5A4  push    ecx
42A5A5  lea     edx, [ebp-70h]
42A5A8  push    edx
42A5A9  mov     eax, edx
42A5AB  lea     edi, [ebp-0B0h]
42A5B1  mov     ecx, esi
42A5B3  call    sub_42A2B0
42A5B8  mov     eax, [ebx+1Ch]
42A5BB  mov     ecx, [ebx+18h]
42A5BE  mov     edx, [ebp-74h]
42A5C1  push    eax
42A5C2  mov     eax, [ebp-78h]
42A5C5  push    ecx
42A5C6  mov     ecx, [ebp-60h]
42A5C9  push    edx
42A5CA  mov     edx, [ebp-88h]
42A5D0  push    eax
42A5D1  mov     eax, [ebp-8Ch]
42A5D7  push    ecx
42A5D8  mov     ecx, [ebp-90h]
42A5DE  push    edx
42A5DF  push    eax
42A5E0  push    ecx
42A5E1  mov     edx, [ebp-68h]
42A5E4  mov     eax, [ebp-5Ch]
42A5E7  mov     ecx, [ebp-6Ch]
42A5EA  push    edx
42A5EB  mov     edx, [ebp-70h]
42A5EE  push    eax
42A5EF  mov     eax, [ebp-7Ch]
42A5F2  push    ecx
42A5F3  mov     ecx, [ebp-80h]
42A5F6  push    edx
42A5F7  push    eax
42A5F8  push    ecx
42A5F9  mov     ecx, edi
42A5FB  lea     eax, [ebp-58h]
42A5FE  call    sub_40F700
42A603  add     esp, 38h
42A606  lea     eax, [ebp-0B0h]
42A60C  call    sub_41C390
42A611  mov     edx, [ebp-0A4h]
42A617  lea     eax, [esi+30Ch]
42A61D  push    edx
42A61E  push    eax
42A61F  mov     [ebp-60h], eax
42A622  lea     eax, [esi+308h]
42A628  lea     edi, [esi+300h]
42A62E  push    eax
42A62F  add     esi, 304h
42A635  push    esi
42A636  push    edi
42A637  lea     esi, [ebp-58h]
42A63A  call    sub_40EB10
42A63F  add     esp, 14h
42A642  cmp     dword ptr [edi], 0
42A645  jz      short loc_42A693
42A647  mov     esi, [ebp-64h]
42A64A  mov     ecx, [esi+318h]
42A650  sub     ecx, [esi+310h]
42A656  mov     eax, [esi+314h]
42A65C  mov     edi, [ebp-60h]
42A65F  sub     eax, [edi]
42A661  mov     edx, [esi]
42A663  inc     ecx
42A664  inc     eax
42A665  push    ecx
42A666  mov     [esi+2F0h], eax
42A66C  mov     [esi+2F4h], ecx
42A672  push    eax
42A673  mov     eax, [edx+74h]
42A676  mov     ecx, esi
42A678  call    eax
42A67A  mov     eax, [esi+310h]
42A680  mov     ecx, [edi]
42A682  mov     edx, [esi]
42A684  mov     edx, [edx+28h]
42A687  push    1
42A689  push    0
42A68B  push    eax
42A68C  push    ecx
42A68D  mov     ecx, esi
42A68F  call    edx
42A691  jmp     short loc_42A6A1
42A693  mov     ecx, [ebp-64h]
42A696  mov     eax, [ecx]
42A698  mov     edx, [eax+74h]
42A69B  push    1
42A69D  push    1
42A69F  call    edx
42A6A1  mov     ecx, [ebp-4]
42A6A4  pop     edi
42A6A5  xor     ecx, ebp
42A6A7  pop     esi
42A6A8  call    sub_4ABDD5
42A6AD  mov     esp, ebp
42A6AF  pop     ebp
42A6B0  mov     esp, ebx
42A6B2  pop     ebx
42A6B3  retn    28h ; '('

;==== 42AA60 sub_42AA60
42AA60  push    ebp
42AA61  mov     ebp, esp
42AA63  sub     esp, 38h
42AA66  push    ebx
42AA67  mov     ebx, [ebp+arg_0]
42AA6A  push    esi
42AA6B  push    edi
42AA6C  push    ebx
42AA6D  call    sub_42ADD0
42AA72  mov     ecx, [ebx+16Ch]
42AA78  mov     edx, dword_565B40
42AA7E  lea     esi, [ebp+var_20]
42AA81  call    sub_407F10
42AA86  test    eax, eax
42AA88  jnz     short loc_42AA9C
42AA8A  mov     [ebp+var_20], eax
42AA8D  mov     [ebp+var_1C], eax
42AA90  mov     [ebp+var_18], eax
42AA93  mov     [ebp+var_14], eax
42AA96  mov     [ebp+var_10], eax
42AA99  mov     [ebp+var_C], eax
42AA9C  mov     ecx, [ebx+170h]
42AAA2  mov     edx, dword_565B40
42AAA8  lea     esi, [ebp+var_38]
42AAAB  call    sub_407F10
42AAB0  test    eax, eax
42AAB2  jnz     short loc_42AAC6
42AAB4  mov     [ebp+var_38], eax
42AAB7  mov     [ebp+var_34], eax
42AABA  mov     [ebp+var_30], eax
42AABD  mov     [ebp+var_2C], eax
42AAC0  mov     [ebp+var_28], eax
42AAC3  mov     [ebp+var_24], eax
42AAC6  lea     esi, [ebp+var_38]
42AAC9  lea     edi, [ebp+var_20]
42AACC  mov     [ebp+arg_0], esi
42AACF  add     ebx, 1DCh
42AAD5  mov     [ebp+var_8], 4
42AADC  jmp     short loc_42AAE3
42AAE0  mov     esi, [ebp+arg_0]
42AAE3  cmp     dword ptr [edi], 0
42AAE6  jz      short loc_42AB1F
42AAE8  mov     ecx, [edi+8]
42AAEB  cmp     ecx, 2
42AAEE  jb      short loc_42AB1F
42AAF0  mov     eax, [edi+0Ch]
42AAF3  cmp     eax, 2
42AAF6  jb      short loc_42AB1F
42AAF8  inc     ecx
42AAF9  lea     edx, [eax+1]
42AAFC  mov     eax, [edi+10h]
42AAFF  shr     ecx, 1
42AB01  lea     esi, [ebx-60h]
42AB04  shr     edx, 1
42AB06  push    ecx
42AB07  call    sub_409020
42AB0C  add     esp, 4
42AB0F  test    eax, eax
42AB11  jz      short loc_42AB1C
42AB13  mov     eax, edi
42AB15  mov     ecx, esi
42AB17  call    sub_419220
42AB1C  mov     esi, [ebp+arg_0]
42AB1F  cmp     dword ptr [esi], 0
42AB22  jz      short loc_42AB58
42AB24  mov     ecx, [esi+8]
42AB27  cmp     ecx, 2
42AB2A  jb      short loc_42AB58
42AB2C  mov     eax, [esi+0Ch]
42AB2F  cmp     eax, 2
42AB32  jb      short loc_42AB58
42AB34  inc     ecx
42AB35  lea     edx, [eax+1]
42AB38  mov     eax, [esi+10h]
42AB3B  shr     ecx, 1
42AB3D  shr     edx, 1
42AB3F  push    ecx
42AB40  mov     esi, ebx
42AB42  call    sub_409020
42AB47  add     esp, 4
42AB4A  test    eax, eax
42AB4C  jz      short loc_42AB58
42AB4E  mov     eax, [ebp+arg_0]
42AB51  mov     ecx, ebx
42AB53  call    sub_419220
42AB58  lea     edi, [ebx-60h]
42AB5B  mov     [ebp+arg_0], ebx
42AB5E  add     ebx, 18h
42AB61  dec     [ebp+var_8]
42AB64  jnz     loc_42AAE0
42AB6A  pop     edi
42AB6B  pop     esi
42AB6C  pop     ebx
42AB6D  mov     esp, ebp
42AB6F  pop     ebp
42AB70  retn    4

;==== 42ADD0 sub_42ADD0
42ADD0  push    ebp
42ADD1  mov     ebp, esp
42ADD3  push    ebx
42ADD4  mov     ebx, [ebp+arg_0]
42ADD7  push    esi
42ADD8  add     ebx, 1DCh
42ADDE  push    edi
42ADDF  mov     esi, ebx
42ADE1  mov     edi, 4
42ADE6  mov     eax, [esi-60h]
42ADE9  push    eax; void *
42ADEA  call    ??_V@YAXPAX@Z; operator delete[](void *)
42ADEF  mov     ecx, [esi]
42ADF1  push    ecx; void *
42ADF2  call    ??_V@YAXPAX@Z; operator delete[](void *)
42ADF7  add     esp, 8
42ADFA  add     esi, 18h
42ADFD  dec     edi
42ADFE  jnz     short loc_42ADE6
42AE00  mov     edx, [ebp+arg_0]
42AE03  push    60h ; '`'; Size
42AE05  add     edx, 17Ch
42AE0B  push    edi; Val
42AE0C  push    edx; void *
42AE0D  call    _memset
42AE12  push    60h ; '`'; Size
42AE14  push    edi; Val
42AE15  push    ebx; void *
42AE16  call    _memset
42AE1B  add     esp, 18h
42AE1E  pop     edi
42AE1F  pop     esi
42AE20  pop     ebx
42AE21  pop     ebp
42AE22  retn    4

;==== 42AE30 sub_42AE30
42AE30  push    ebx
42AE31  mov     ebx, esp
42AE33  sub     esp, 8
42AE36  and     esp, 0FFFFFFF0h
42AE39  add     esp, 4
42AE3C  push    ebp
42AE3D  mov     ebp, [ebx+4]
42AE40  mov     [esp+0Ch+var_8], ebp
42AE44  mov     ebp, esp
42AE46  sub     esp, 28h
42AE49  push    esi
42AE4A  push    edi
42AE4B  mov     edi, eax
42AE4D  mov     edx, [edi+150h]
42AE53  xor     eax, eax
42AE55  sub     edx, 5
42AE58  mov     ecx, 8000h
42AE5D  mov     [ebp-4], eax
42AE60  mov     [ebp-8], ecx
42AE63  jz      short loc_42AEAB
42AE65  dec     edx
42AE66  jnz     short loc_42AEA2
42AE68  lea     esi, [ebp-20h]
42AE6B  call    sub_41B7F0
42AE70  mov     esi, [edi+294h]
42AE76  lea     eax, [ebp-0Ch]
42AE79  push    eax
42AE7A  mov     eax, [ebp-18h]
42AE7D  call    sub_41ADE0
42AE82  mov     ecx, [ebp-0Ch]
42AE85  add     esp, 4
42AE88  cmp     ecx, 8000h
42AE8E  ja      short loc_42AE9F
42AE90  mov     eax, [ebp-8]
42AE93  inc     dword ptr [ebp-4]
42AE96  shr     eax, 1
42AE98  mov     [ebp-8], eax
42AE9B  cmp     eax, ecx
42AE9D  jnb     short loc_42AE90
42AE9F  mov     eax, [ebp-4]
42AEA2  pop     edi
42AEA3  pop     esi
42AEA4  mov     esp, ebp
42AEA6  pop     ebp
42AEA7  mov     esp, ebx
42AEA9  pop     ebx
42AEAA  retn
42AEAB  mov     edx, [edi+2B8h]
42AEB1  cmp     edx, ecx
42AEB3  ja      short loc_42AEA2
42AEB5  shr     ecx, 1
42AEB7  inc     eax
42AEB8  cmp     ecx, edx
42AEBA  jnb     short loc_42AEB5
42AEBC  pop     edi
42AEBD  pop     esi
42AEBE  mov     esp, ebp
42AEC0  pop     ebp
42AEC1  mov     esp, ebx
42AEC3  pop     ebx
42AEC4  retn

;==== 42AED0 sub_42AED0
42AED0  push    ebp
42AED1  mov     ebp, esp
42AED3  push    ecx
42AED4  push    ebx
42AED5  mov     ebx, [ebp+arg_4]
42AED8  push    esi
42AED9  push    edi
42AEDA  mov     esi, eax
42AEDC  mov     edi, ecx
42AEDE  test    ebx, ebx
42AEE0  jnz     short loc_42AEEA
42AEE2  mov     ecx, [edi+16Ch]
42AEE8  jmp     short loc_42AEF0
42AEEA  mov     ecx, [edi+170h]
42AEF0  mov     edx, dword_565B40
42AEF6  call    sub_407F10
42AEFB  mov     [ebp+arg_4], eax
42AEFE  mov     eax, edi
42AF00  call    sub_42AE30
42AF05  cmp     [ebp+arg_4], 0
42AF09  jz      short loc_42AF63
42AF0B  test    eax, eax
42AF0D  jz      short loc_42AF63
42AF0F  lea     ecx, [edi+17Ch]
42AF15  test    ebx, ebx
42AF17  jz      short loc_42AF1F
42AF19  lea     ecx, [edi+1DCh]
42AF1F  cmp     eax, 4
42AF22  jbe     short loc_42AF29
42AF24  mov     eax, 4
42AF29  xor     edx, edx
42AF2B  test    eax, eax
42AF2D  jz      short loc_42AF63
42AF2F  nop
42AF30  cmp     dword ptr [ecx], 0
42AF33  jz      short loc_42AF61
42AF35  mov     edi, [ecx]
42AF37  mov     [esi], edi
42AF39  mov     edi, [ecx+4]
42AF3C  mov     [esi+4], edi
42AF3F  mov     edi, [ecx+8]
42AF42  mov     [esi+8], edi
42AF45  mov     edi, [ecx+0Ch]
42AF48  mov     [esi+0Ch], edi
42AF4B  mov     edi, [ecx+10h]
42AF4E  mov     [esi+10h], edi
42AF51  mov     edi, [ecx+14h]
42AF54  inc     edx
42AF55  add     ecx, 18h
42AF58  mov     [esi+14h], edi
42AF5B  cmp     edx, eax
42AF5D  jb      short loc_42AF30
42AF5F  jmp     short loc_42AF63
42AF61  mov     eax, edx
42AF63  mov     ecx, [ebp+arg_0]
42AF66  test    ecx, ecx
42AF68  jz      short loc_42AF6C
42AF6A  mov     [ecx], eax
42AF6C  mov     eax, [ebp+arg_4]
42AF6F  pop     edi
42AF70  pop     esi
42AF71  pop     ebx
42AF72  pop     ecx
42AF73  pop     ebp
42AF74  retn    8

;==== 42AF80 sub_42AF80
42AF80  push    ebp
42AF81  mov     ebp, esp
42AF83  push    esi
42AF84  mov     esi, [ebp+arg_0]
42AF87  mov     eax, 8000000Fh
42AF8C  cmp     ecx, 10h
42AF8F  jnb     short loc_42AFC4
42AF91  cmp     esi, 5
42AF94  ja      short loc_42AFBF
42AF96  lea     eax, [ecx+ecx*2]
42AF99  lea     eax, [edx+eax*4]
42AF9C  mov     [eax+374h], esi
42AFA2  mov     esi, [ebp+arg_4]
42AFA5  lea     ecx, [ecx+ecx*2+0DEh]
42AFAC  mov     [edx+ecx*4], esi
42AFAF  mov     edx, [ebp+arg_8]
42AFB2  mov     [eax+37Ch], edx
42AFB8  xor     eax, eax
42AFBA  pop     esi
42AFBB  pop     ebp
42AFBC  retn    0Ch
42AFBF  mov     eax, 80000010h
42AFC4  pop     esi
42AFC5  pop     ebp
42AFC6  retn    0Ch

;==== 430AB0 sub_430AB0
430AB0  push    ebp
430AB1  mov     ebp, esp
430AB3  push    ebx
430AB4  mov     ebx, [ebp+arg_0]
430AB7  push    esi
430AB8  mov     esi, eax
430ABA  mov     eax, [ebx]
430ABC  mov     edx, [eax+10h]
430ABF  mov     ecx, ebx
430AC1  call    edx
430AC3  test    eax, eax
430AC5  jz      short loc_430AD3
430AC7  push    ebx
430AC8  call    sub_431680
430ACD  pop     esi
430ACE  pop     ebx
430ACF  pop     ebp
430AD0  retn    4
430AD3  mov     eax, [ebx]
430AD5  mov     edx, [eax+1Ch]
430AD8  push    edi
430AD9  mov     ecx, ebx
430ADB  call    edx
430ADD  lea     edi, [esi+4]
430AE0  mov     esi, [esi+0Ch]
430AE3  mov     [ebp+arg_0], eax
430AE6  test    esi, esi
430AE8  jz      short loc_430AFD
430AEA  lea     ebx, [ebx+0]
430AF0  cmp     [esi], eax
430AF2  ja      short loc_430AFD
430AF4  mov     edi, esi
430AF6  mov     esi, [esi+8]
430AF9  test    esi, esi
430AFB  jnz     short loc_430AF0
430AFD  push    0Ch; Size
430AFF  call    ??2@YAPAXI@Z; operator new(uint)
430B04  mov     ecx, [ebp+arg_0]
430B07  mov     [edi+8], eax
430B0A  mov     [eax], ecx
430B0C  mov     edx, [edi+8]
430B0F  add     esp, 4
430B12  mov     [edx+4], ebx
430B15  mov     eax, [edi+8]
430B18  pop     edi
430B19  mov     [eax+8], esi
430B1C  pop     esi
430B1D  pop     ebx
430B1E  pop     ebp
430B1F  retn    4

;==== 430B30 sub_430B30
430B30  push    ebx
430B31  push    esi
430B32  push    edi
430B33  mov     edi, eax
430B35  xor     ebx, ebx
430B37  mov     esi, ecx
430B39  test    edi, edi
430B3B  jz      short loc_430B6E
430B3D  mov     eax, [edi]
430B3F  mov     edx, [eax+10h]
430B42  mov     ecx, edi
430B44  call    edx
430B46  test    eax, eax
430B48  jz      short loc_430B53
430B4A  call    sub_4316B0
430B4F  pop     edi
430B50  pop     esi
430B51  pop     ebx
430B52  retn
430B53  mov     eax, [esi+0Ch]
430B56  lea     ecx, [esi+4]
430B59  test    eax, eax
430B5B  jz      short loc_430B6E
430B5D  lea     ecx, [ecx+0]
430B60  cmp     [eax+4], edi
430B63  jz      short loc_430B74
430B65  mov     ecx, eax
430B67  mov     eax, [eax+8]
430B6A  test    eax, eax
430B6C  jnz     short loc_430B60
430B6E  pop     edi
430B6F  pop     esi
430B70  mov     eax, ebx
430B72  pop     ebx
430B73  retn
430B74  mov     edx, [eax+8]
430B77  push    eax; void *
430B78  mov     [ecx+8], edx
430B7B  call    ??3@YAXPAX@Z; operator delete(void *)
430B80  add     esp, 4
430B83  pop     edi
430B84  pop     esi
430B85  mov     eax, 1
430B8A  pop     ebx
430B8B  retn

;==== 430B90 sub_430B90
430B90  push    ebx
430B91  mov     eax, esi
430B93  mov     ecx, edi
430B95  xor     ebx, ebx
430B97  call    sub_430B30
430B9C  test    eax, eax
430B9E  jz      short loc_430BAD
430BA0  push    esi
430BA1  mov     eax, edi
430BA3  call    sub_430AB0
430BA8  lea     eax, [ebx+1]
430BAB  pop     ebx
430BAC  retn
430BAD  mov     eax, ebx
430BAF  pop     ebx
430BB0  retn

;==== 431680 sub_431680
431680  push    ebp
431681  mov     ebp, esp
431683  push    8; Size
431685  call    ??2@YAPAXI@Z; operator new(uint)
43168A  mov     ecx, [ebp+arg_0]
43168D  mov     [eax], ecx
43168F  mov     edx, [esi+14h]
431692  add     esp, 4
431695  mov     [eax+4], edx
431698  push    esi
431699  mov     [esi+14h], eax
43169C  call    sub_4316F0
4316A1  pop     ebp
4316A2  retn    4

;==== 4316B0 sub_4316B0
4316B0  mov     ecx, [esi+14h]
4316B3  xor     eax, eax
4316B5  lea     edx, [esi+10h]
4316B8  test    ecx, ecx
4316BA  jz      short locret_4316E8
4316BC  lea     esp, [esp+0]
4316C0  cmp     [ecx], edi
4316C2  jz      short loc_4316CE
4316C4  mov     edx, ecx
4316C6  mov     ecx, [ecx+4]
4316C9  test    ecx, ecx
4316CB  jnz     short loc_4316C0
4316CD  retn
4316CE  mov     eax, [ecx+4]
4316D1  push    ecx; void *
4316D2  mov     [edx+4], eax
4316D5  call    ??3@YAXPAX@Z; operator delete(void *)
4316DA  add     esp, 4
4316DD  push    esi
4316DE  call    sub_4316F0
4316E3  mov     eax, 1
4316E8  retn

;==== 4316F0 sub_4316F0
4316F0  push    ebp
4316F1  mov     ebp, esp
4316F3  sub     esp, 0Ch
4316F6  push    ebx
4316F7  mov     ebx, [ebp+arg_0]
4316FA  mov     eax, [ebx+24h]
4316FD  push    edi
4316FE  push    eax; void *
4316FF  call    ??_V@YAXPAX@Z; operator delete[](void *)
431704  xor     eax, eax
431706  mov     [ebx+24h], eax
431709  mov     [ebx+18h], eax
43170C  mov     [ebx+1Ch], eax
43170F  mov     [ebx+20h], eax
431712  mov     edi, [ebx+14h]
431715  add     esp, 4
431718  mov     [ebp+var_4], eax
43171B  cmp     edi, eax
43171D  jz      loc_43180C
431723  push    esi
431724  mov     esi, [edi]
431726  xor     ecx, ecx; void *
431728  call    sub_41B290
43172D  mov     edi, [edi+4]
431730  add     [ebp+var_4], eax
431733  test    edi, edi
431735  jnz     short loc_431724
431737  mov     esi, [ebp+var_4]
43173A  test    esi, esi
43173C  jz      loc_43180B
431742  xor     ecx, ecx
431744  mov     eax, esi
431746  mov     edx, 0Ch
43174B  mul     edx
43174D  seto    cl
431750  neg     ecx
431752  or      ecx, eax
431754  push    ecx; unsigned int
431755  call    ??_U@YAPAXI@Z; operator new[](uint)
43175A  mov     [ebx+24h], eax
43175D  mov     edi, eax
43175F  xor     ecx, ecx
431761  mov     eax, esi
431763  mov     edx, 4
431768  mul     edx
43176A  seto    cl
43176D  neg     ecx
43176F  or      ecx, eax
431771  push    ecx; unsigned int
431772  call    ??_U@YAPAXI@Z; operator new[](uint)
431777  mov     edx, [ebx+14h]
43177A  add     esp, 8
43177D  mov     [ebp+var_8], eax
431780  mov     [ebp+var_4], edx
431783  test    edx, edx
431785  jz      short loc_4317FF
431787  jmp     short loc_431796
431790  mov     edx, [ebp+var_4]
431793  mov     ebx, [ebp+arg_0]
431796  mov     ecx, [ebp+var_8]; void *
431799  mov     esi, [edx]
43179B  lea     eax, [ebx+18h]
43179E  mov     ebx, [ebx+20h]
4317A1  mov     [ebp+var_C], eax
4317A4  call    sub_41B290
4317A9  xor     ecx, ecx
4317AB  test    eax, eax
4317AD  jz      short loc_4317F2
4317AF  nop
4317B0  test    ebx, ebx
4317B2  jz      short loc_4317CE
4317B4  mov     edx, [ebp+var_8]
4317B7  mov     edx, [edx+ecx*4]
4317BA  lea     ebx, [ebx+0]
4317C0  cmp     [ebx], edx
4317C2  ja      short loc_4317CE
4317C4  mov     [ebp+var_C], ebx
4317C7  mov     ebx, [ebx+8]
4317CA  test    ebx, ebx
4317CC  jnz     short loc_4317C0
4317CE  mov     edx, [ebp+var_8]
4317D1  mov     edx, [edx+ecx*4]
4317D4  mov     [edi], edx
4317D6  mov     edx, [ebp+var_4]
4317D9  mov     edx, [edx]
4317DB  mov     [edi+4], edx
4317DE  mov     edx, [ebp+var_C]
4317E1  mov     [edi+8], ebx
4317E4  mov     [edx+8], edi
4317E7  mov     [ebp+var_C], edi
4317EA  inc     ecx
4317EB  add     edi, 0Ch
4317EE  cmp     ecx, eax
4317F0  jb      short loc_4317B0
4317F2  mov     eax, [ebp+var_4]
4317F5  mov     eax, [eax+4]
4317F8  mov     [ebp+var_4], eax
4317FB  test    eax, eax
4317FD  jnz     short loc_431790
4317FF  mov     ecx, [ebp+var_8]
431802  push    ecx; void *
431803  call    ??_V@YAXPAX@Z; operator delete[](void *)
431808  add     esp, 4
43180B  pop     esi
43180C  pop     edi
43180D  pop     ebx
43180E  mov     esp, ebp
431810  pop     ebp
431811  retn    4

;==== 43E990 sub_43E990
43E990  push    ebp
43E991  mov     ebp, esp
43E993  mov     edx, eax
43E995  and     edx, 0FF000000h
43E99B  xor     ecx, ecx
43E99D  cmp     edx, 80000000h
43E9A3  jnz     short loc_43E9BC
43E9A5  call    sub_443630
43E9AA  cmp     eax, 200h
43E9AF  jnb     short loc_43E9BC
43E9B1  mov     ecx, [ebp+arg_0]
43E9B4  mov     eax, [ecx+eax*4+5Ch]
43E9B8  pop     ebp
43E9B9  retn    4
43E9BC  mov     eax, ecx
43E9BE  pop     ebp
43E9BF  retn    4

;==== 43F060 sub_43F060
43F060  push    ebp
43F061  mov     ebp, esp
43F063  mov     eax, dword_566760
43F068  push    esi
43F069  push    eax
43F06A  mov     eax, [ebp+arg_0]
43F06D  call    sub_43E990
43F072  mov     esi, eax
43F074  test    esi, esi
43F076  jz      short loc_43F0C6
43F078  mov     edx, [esi]
43F07A  mov     eax, [edx+8]
43F07D  push    edi
43F07E  mov     ecx, esi
43F080  call    eax
43F082  mov     edi, eax
43F084  test    edi, edi
43F086  jz      short loc_43F091
43F088  mov     edx, [esi]
43F08A  mov     eax, [edx+0Ch]
43F08D  mov     ecx, esi
43F08F  call    eax
43F091  mov     ecx, [ebp+arg_4]
43F094  mov     eax, esi
43F096  call    sub_427670
43F09B  test    eax, eax
43F09D  jz      short loc_43F0B1
43F09F  cmp     eax, 80000001h
43F0A4  jnz     short loc_43F0D0
43F0A6  pop     edi
43F0A7  mov     eax, 1
43F0AC  pop     esi
43F0AD  pop     ebp
43F0AE  retn    8
43F0B1  test    edi, edi
43F0B3  jz      short loc_43F0BE
43F0B5  mov     edx, [esi]
43F0B7  mov     eax, [edx+0Ch]
43F0BA  mov     ecx, esi
43F0BC  call    eax
43F0BE  pop     edi
43F0BF  xor     eax, eax
43F0C1  pop     esi
43F0C2  pop     ebp
43F0C3  retn    8
43F0C6  mov     eax, 0FFh
43F0CB  pop     esi
43F0CC  pop     ebp
43F0CD  retn    8
43F0D0  mov     eax, [ebp+arg_0]
43F0D3  pop     edi
43F0D4  pop     esi
43F0D5  pop     ebp
43F0D6  retn    8

;==== 43F410 sub_43F410
43F410  push    ebp
43F411  mov     ebp, esp
43F413  mov     edx, eax
43F415  and     edx, 0FF000000h
43F41B  xor     ecx, ecx
43F41D  cmp     edx, 90000000h
43F423  jnz     short loc_43F43D
43F425  call    sub_443630
43F42A  cmp     eax, 8
43F42D  jnb     short loc_43F43D
43F42F  mov     ecx, [ebp+arg_0]
43F432  mov     eax, [ecx+eax*4+864h]
43F439  pop     ebp
43F43A  retn    4
43F43D  mov     eax, ecx
43F43F  pop     ebp
43F440  retn    4

;==== 43F710 sub_43F710
43F710  push    ebp
43F711  mov     ebp, esp
43F713  mov     edx, eax
43F715  and     edx, 0FF000000h
43F71B  xor     ecx, ecx
43F71D  cmp     edx, 91000000h
43F723  jnz     short loc_43F73D
43F725  call    sub_443630
43F72A  cmp     eax, 8
43F72D  jnb     short loc_43F73D
43F72F  mov     ecx, [ebp+arg_0]
43F732  mov     eax, [ecx+eax*4+88Ch]
43F739  pop     ebp
43F73A  retn    4
43F73D  mov     eax, ecx
43F73F  pop     ebp
43F740  retn    4

;==== 43FC70 sub_43FC70
43FC70  push    ebp
43FC71  mov     ebp, esp
43FC73  mov     edx, eax
43FC75  and     edx, 0FF000000h
43FC7B  xor     ecx, ecx
43FC7D  cmp     edx, 0A0000000h
43FC83  jnz     short loc_43FC9D
43FC85  call    sub_443630
43FC8A  cmp     eax, 8
43FC8D  jnb     short loc_43FC9D
43FC8F  mov     ecx, [ebp+arg_0]
43FC92  mov     eax, [ecx+eax*4+8B4h]
43FC99  pop     ebp
43FC9A  retn    4
43FC9D  mov     eax, ecx
43FC9F  pop     ebp
43FCA0  retn    4

;==== 4400F0 sub_4400F0
4400F0  push    ebp
4400F1  mov     ebp, esp
4400F3  mov     edx, eax
4400F5  and     edx, 0FF000000h
4400FB  xor     ecx, ecx
4400FD  cmp     edx, 0A1000000h
440103  jnz     short loc_44011D
440105  call    sub_443630
44010A  cmp     eax, 4
44010D  jnb     short loc_44011D
44010F  mov     ecx, [ebp+arg_0]
440112  mov     eax, [ecx+eax*4+8DCh]
440119  pop     ebp
44011A  retn    4
44011D  mov     eax, ecx
44011F  pop     ebp
440120  retn    4

;==== 440A80 sub_440A80
440A80  push    ebp
440A81  mov     ebp, esp
440A83  mov     edx, eax
440A85  and     edx, 0FF000000h
440A8B  xor     ecx, ecx
440A8D  cmp     edx, 0B0000000h
440A93  jnz     short loc_440AAD
440A95  call    sub_443630
440A9A  cmp     eax, 10h
440A9D  jnb     short loc_440AAD
440A9F  mov     ecx, [ebp+arg_0]
440AA2  mov     eax, [ecx+eax*4+8F4h]
440AA9  pop     ebp
440AAA  retn    4
440AAD  mov     eax, ecx
440AAF  pop     ebp
440AB0  retn    4

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

;==== 441D80 sub_441D80
441D80  push    ebp
441D81  mov     ebp, esp
441D83  mov     edx, eax
441D85  and     edx, 0FF000000h
441D8B  xor     ecx, ecx
441D8D  cmp     edx, 0C1000000h
441D93  jnz     short loc_441DAD
441D95  call    sub_443630
441D9A  cmp     eax, 8
441D9D  jnb     short loc_441DAD
441D9F  mov     ecx, [ebp+arg_0]
441DA2  mov     eax, [ecx+eax*4+964h]
441DA9  pop     ebp
441DAA  retn    4
441DAD  mov     eax, ecx
441DAF  pop     ebp
441DB0  retn    4

;==== 4425E0 sub_4425E0
4425E0  push    ebp
4425E1  mov     ebp, esp
4425E3  mov     edx, eax
4425E5  and     edx, 0FF000000h
4425EB  xor     ecx, ecx
4425ED  cmp     edx, 0F0000000h
4425F3  jnz     short loc_44260D
4425F5  call    sub_443630
4425FA  cmp     eax, 20h ; ' '
4425FD  jnb     short loc_44260D
4425FF  mov     ecx, [ebp+arg_0]
442602  mov     eax, [ecx+eax*4+98Ch]
442609  pop     ebp
44260A  retn    4
44260D  mov     eax, ecx
44260F  pop     ebp
442610  retn    4

;==== 4429D0 sub_4429D0
4429D0  push    ebp
4429D1  mov     ebp, esp
4429D3  mov     edx, eax
4429D5  and     edx, 0FF000000h
4429DB  xor     ecx, ecx
4429DD  cmp     edx, 0F1000000h
4429E3  jnz     short loc_4429FD
4429E5  call    sub_443630
4429EA  cmp     eax, 8
4429ED  jnb     short loc_4429FD
4429EF  mov     ecx, [ebp+arg_0]
4429F2  mov     eax, [ecx+eax*4+0A14h]
4429F9  pop     ebp
4429FA  retn    4
4429FD  mov     eax, ecx
4429FF  pop     ebp
442A00  retn    4

;==== 4431D0 sub_4431D0
4431D0  test    ecx, ecx
4431D2  jz      short loc_4431F8
4431D4  mov     edx, [ecx+18h]
4431D7  mov     [eax], edx
4431D9  mov     edx, [ecx+1Ch]
4431DC  mov     [eax+4], edx
4431DF  mov     edx, [ecx+20h]
4431E2  mov     [eax+8], edx
4431E5  mov     edx, [ecx+24h]
4431E8  mov     [eax+0Ch], edx
4431EB  mov     edx, [ecx+28h]
4431EE  mov     [eax+10h], edx
4431F1  mov     ecx, [ecx+2Ch]
4431F4  mov     [eax+14h], ecx
4431F7  retn
4431F8  xor     ecx, ecx
4431FA  mov     [eax], ecx
4431FC  mov     [eax+4], ecx
4431FF  mov     [eax+8], ecx
443202  mov     [eax+0Ch], ecx
443205  mov     [eax+10h], ecx
443208  mov     [eax+14h], ecx
44320B  retn

;==== 443250 sub_443250
443250  mov     ecx, dword_565B3C
443256  mov     edx, [ecx+40h]
443259  xor     eax, eax
44325B  test    edx, edx
44325D  js      short locret_44327F
44325F  cmp     edx, [ecx+20h]
443262  jge     short locret_44327F
443264  mov     edx, [ecx+44h]
443267  test    edx, edx
443269  js      short locret_44327F
44326B  cmp     edx, [ecx+24h]
44326E  jge     short locret_44327F
443270  mov     eax, [ecx+40h]
443273  mov     ecx, edx
443275  mov     [esi], eax
443277  mov     [esi+4], ecx
44327A  mov     eax, 1
44327F  retn

;==== 443630 sub_443630
443630  and     eax, 0FFFFFFh
443635  retn

;==== 443640 sub_443640
443640  mov     eax, [edi+50h]
443643  test    esi, esi
443645  jz      short locret_4436BB
443647  push    edi
443648  mov     eax, esi
44364A  call    sub_43E990
44364F  test    eax, eax
443651  jnz     short locret_4436BB
443653  push    edi
443654  mov     eax, esi
443656  call    sub_43F410
44365B  test    eax, eax
44365D  jnz     short locret_4436BB
44365F  push    edi
443660  mov     eax, esi
443662  call    sub_43F710
443667  test    eax, eax
443669  jnz     short locret_4436BB
44366B  push    edi
44366C  mov     eax, esi
44366E  call    sub_43FC70
443673  test    eax, eax
443675  jnz     short locret_4436BB
443677  push    edi
443678  mov     eax, esi
44367A  call    sub_4400F0
44367F  test    eax, eax
443681  jnz     short locret_4436BB
443683  push    edi
443684  mov     eax, esi
443686  call    sub_440A80
44368B  test    eax, eax
44368D  jnz     short locret_4436BB
44368F  push    edi
443690  mov     eax, esi
443692  call    sub_441600
443697  test    eax, eax
443699  jnz     short locret_4436BB
44369B  push    edi
44369C  mov     eax, esi
44369E  call    sub_441D80
4436A3  test    eax, eax
4436A5  jnz     short locret_4436BB
4436A7  push    edi
4436A8  mov     eax, esi
4436AA  call    sub_4425E0
4436AF  test    eax, eax
4436B1  jnz     short locret_4436BB
4436B3  push    edi
4436B4  mov     eax, esi
4436B6  call    sub_4429D0
4436BB  retn

;==== 4436D0 sub_4436D0
4436D0  push    edi
4436D1  mov     edi, [eax+14h]
4436D4  call    sub_430B90
4436D9  pop     edi
4436DA  retn

;==== 443C80 sub_443C80
443C80  push    ebp
443C81  mov     ebp, esp
443C83  push    esi
443C84  push    edi
443C85  mov     edi, dword_566760
443C8B  mov     esi, eax
443C8D  call    sub_443640
443C92  mov     esi, eax
443C94  test    esi, esi
443C96  jz      loc_443D22
443C9C  mov     eax, [esi]
443C9E  mov     edx, [eax+8]
443CA1  mov     ecx, esi
443CA3  call    edx
443CA5  test    eax, eax
443CA7  jz      short loc_443CB2
443CA9  mov     eax, [esi]
443CAB  mov     edx, [eax+0Ch]
443CAE  mov     ecx, esi
443CB0  call    edx
443CB2  mov     eax, [esi]
443CB4  mov     edx, [eax+1Ch]
443CB7  push    ebx
443CB8  mov     ecx, esi
443CBA  call    edx
443CBC  mov     ecx, [ebp+arg_8]
443CBF  mov     edx, [ebp+arg_4]
443CC2  push    ecx
443CC3  mov     ecx, [ebp+arg_0]
443CC6  push    edx
443CC7  mov     ebx, eax
443CC9  mov     eax, [esi]
443CCB  mov     edx, [eax+5Ch]
443CCE  push    ecx
443CCF  mov     ecx, esi
443CD1  call    edx
443CD3  test    eax, eax
443CD5  jz      short loc_443CEF
443CD7  sub     eax, 0FFFF0001h
443CDC  neg     eax
443CDE  sbb     eax, eax
443CE0  pop     ebx
443CE1  and     eax, 0F9h
443CE6  pop     edi
443CE7  add     eax, 5
443CEA  pop     esi
443CEB  pop     ebp
443CEC  retn    0Ch
443CEF  mov     eax, [esi]
443CF1  mov     edx, [eax+1Ch]
443CF4  mov     ecx, esi
443CF6  call    edx
443CF8  cmp     ebx, eax
443CFA  jz      short loc_443D03
443CFC  mov     eax, edi
443CFE  call    sub_4436D0
443D03  mov     eax, [esi]
443D05  mov     edx, [eax+8]
443D08  mov     ecx, esi
443D0A  call    edx
443D0C  test    eax, eax
443D0E  jz      short loc_443D19
443D10  mov     eax, [esi]
443D12  mov     edx, [eax+0Ch]
443D15  mov     ecx, esi
443D17  call    edx
443D19  pop     ebx
443D1A  pop     edi
443D1B  xor     eax, eax
443D1D  pop     esi
443D1E  pop     ebp
443D1F  retn    0Ch
443D22  pop     edi
443D23  mov     eax, 0FFh
443D28  pop     esi
443D29  pop     ebp
443D2A  retn    0Ch

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

;==== 454260 sub_454260
454260  mov     eax, [eax+4]
454263  retn

;==== 462570 sub_462570
462570  push    ebp
462571  mov     ebp, esp
462573  mov     edx, [ebp+arg_4]
462576  push    eax
462577  mov     eax, [ebp+arg_0]
46257A  push    ecx
46257B  push    edx
46257C  call    sub_443C80
462581  pop     ebp
462582  retn

;==== 462A70 sub_462A70
462A70  push    eax
462A71  push    ecx
462A72  call    sub_43F060
462A77  retn

;==== 47BE40 sub_47BE40
47BE40  push    ebp
47BE41  mov     ebp, esp
47BE43  sub     esp, 108h
47BE49  mov     eax, dword_4FB734
47BE4E  xor     eax, ebp
47BE50  mov     [ebp+var_4], eax
47BE53  push    ebx
47BE54  push    esi
47BE55  mov     esi, [ebp+arg_0]
47BE58  push    edi
47BE59  mov     ecx, esi
47BE5B  call    sub_445480
47BE60  mov     ecx, esi
47BE62  mov     edi, eax
47BE64  call    sub_445480
47BE69  mov     ecx, esi
47BE6B  mov     ebx, eax
47BE6D  call    sub_445480
47BE72  mov     edx, eax
47BE74  mov     ecx, esi
47BE76  mov     [ebp+var_108], edx
47BE7C  call    sub_445480
47BE81  push    edx
47BE82  push    eax
47BE83  mov     eax, edi
47BE85  mov     ecx, ebx
47BE87  call    sub_462570
47BE8C  add     esp, 8
47BE8F  sub     eax, 5
47BE92  jz      short loc_47BEE7
47BE94  sub     eax, 0F9h
47BE99  jz      short loc_47BEBC
47BE9B  dec     eax
47BE9C  jz      short loc_47BEB1
47BE9E  mov     ecx, [ebp+var_4]
47BEA1  pop     edi
47BEA2  pop     esi
47BEA3  xor     ecx, ebp
47BEA5  xor     eax, eax
47BEA7  pop     ebx
47BEA8  call    sub_4ABDD5
47BEAD  mov     esp, ebp
47BEAF  pop     ebp
47BEB0  retn
47BEB1  push    esi
47BEB2  mov     edi, offset dword_4E8C10
47BEB7  call    sub_464C50
47BEBC  mov     eax, [ebp+var_108]
47BEC2  push    edi
47BEC3  push    edi
47BEC4  push    ebx
47BEC5  push    ebx
47BEC6  push    eax
47BEC7  lea     ecx, [ebp+Buffer]
47BECD  push    offset dword_4E8D70; Format
47BED2  push    ecx; Buffer
47BED3  call    _sprintf
47BED8  add     esp, 1Ch
47BEDB  push    esi
47BEDC  lea     edi, [ebp+Buffer]
47BEE2  call    sub_464C50
47BEE7  mov     edx, [ebp+var_108]
47BEED  push    edx
47BEEE  lea     eax, [ebp+Buffer]
47BEF4  push    offset dword_4E8D30; Format
47BEF9  push    eax; Buffer
47BEFA  call    _sprintf
47BEFF  add     esp, 0Ch
47BF02  push    esi
47BF03  lea     edi, [ebp+Buffer]
47BF09  call    sub_464C50

;==== 47CEE0 sub_47CEE0
47CEE0  push    ebp
47CEE1  mov     ebp, esp
47CEE3  sub     esp, 104h
47CEE9  mov     eax, dword_4FB734
47CEEE  xor     eax, ebp
47CEF0  mov     [ebp+var_4], eax
47CEF3  push    ebx
47CEF4  push    esi
47CEF5  mov     esi, [ebp+arg_0]
47CEF8  push    edi
47CEF9  mov     ecx, esi
47CEFB  call    sub_445480
47CF00  mov     ecx, esi
47CF02  mov     edi, eax
47CF04  call    sub_445480
47CF09  push    edi
47CF0A  mov     ebx, eax
47CF0C  call    sub_4986D0
47CF11  add     esp, 4
47CF14  mov     eax, edi
47CF16  mov     ecx, ebx
47CF18  call    sub_462A70
47CF1D  dec     eax
47CF1E  jz      short loc_47CF45
47CF20  sub     eax, 0FEh
47CF25  jz      short loc_47CF3A
47CF27  mov     ecx, [ebp+var_4]
47CF2A  pop     edi
47CF2B  pop     esi
47CF2C  xor     ecx, ebp
47CF2E  xor     eax, eax
47CF30  pop     ebx
47CF31  call    sub_4ABDD5
47CF36  mov     esp, ebp
47CF38  pop     ebp
47CF39  retn
47CF3A  push    esi
47CF3B  mov     edi, offset dword_4E7FFC
47CF40  call    sub_464C50
47CF45  push    edi
47CF46  lea     eax, [ebp+Buffer]
47CF4C  push    offset dword_4E8E04; Format
47CF51  push    eax; Buffer
47CF52  call    _sprintf
47CF57  add     esp, 0Ch
47CF5A  push    esi
47CF5B  lea     edi, [ebp+Buffer]
47CF61  call    sub_464C50

;==== 493CE0 sub_493CE0
493CE0  mov     eax, [eax+148h]
493CE6  retn

;==== 493E40 sub_493E40
493E40  push    ebp
493E41  mov     ebp, esp
493E43  sub     esp, 1Ch
493E46  push    esi
493E47  mov     eax, edi
493E49  call    sub_41B060
493E4E  mov     esi, eax
493E50  test    esi, esi
493E52  jz      short loc_493E75
493E54  lea     eax, [ebp+pv]
493E57  push    eax; pv
493E58  push    18h; c
493E5A  push    esi; h
493E5B  call    ds:GetObjectA
493E61  mov     ecx, [ebp+var_C]
493E64  mov     edx, [ebp+arg_4]
493E67  mov     eax, edi
493E69  mov     [edx], ecx
493E6B  call    sub_493CE0
493E70  mov     ecx, [ebp+arg_0]
493E73  mov     [ecx], eax
493E75  xor     eax, eax
493E77  test    esi, esi
493E79  setnz   al
493E7C  pop     esi
493E7D  mov     esp, ebp
493E7F  pop     ebp
493E80  retn    8

;==== 4986D0 sub_4986D0
4986D0  push    ebp
4986D1  mov     ebp, esp
4986D3  sub     esp, 104h
4986D9  mov     eax, dword_4FB734
4986DE  xor     eax, ebp
4986E0  mov     [ebp+var_4], eax
4986E3  mov     eax, [ebp+arg_0]
4986E6  test    eax, eax
4986E8  js      short loc_4986F1
4986EA  cmp     eax, 4000h
4986EF  jl      short loc_498712
4986F1  push    eax
4986F2  lea     eax, [ebp+Buffer]
4986F8  push    offset byte_4E97F8; Format
4986FD  push    eax; Buffer
4986FE  call    _sprintf
498703  add     esp, 0Ch
498706  push    esi
498707  lea     edi, [ebp+Buffer]
49870D  call    sub_464C50
498712  mov     ecx, [ebp+var_4]
498715  xor     ecx, ebp
498717  call    sub_4ABDD5
49871C  mov     esp, ebp
49871E  pop     ebp
49871F  retn

;==== 49FA50 sub_49FA50
49FA50  mov     [eax+1Ch], ecx
49FA53  retn

;==== 4ABDD5 sub_4ABDD5
4ABDD5  cmp     ecx, dword_4FB734
4ABDDB  jnz     short loc_4ABDDF
4ABDDD  rep retn
4ABDDF  jmp     ___report_gsfailure

