
;==== 487B30 sub_487B30
487B30  push    ebp
487B31  mov     ebp, esp
487B33  push    ebx
487B34  push    esi
487B35  mov     esi, [ebp+arg_0]
487B38  push    edi
487B39  mov     ecx, esi
487B3B  call    sub_445480
487B40  mov     ecx, esi
487B42  mov     edi, eax
487B44  call    sub_445480
487B49  push    edi
487B4A  mov     ebx, eax
487B4C  call    sub_498560
487B51  push    ebx
487B52  call    sub_4985B0
487B57  add     esp, 8
487B5A  mov     edx, edi
487B5C  mov     ecx, ebx
487B5E  call    sub_494650
487B63  pop     edi
487B64  pop     esi
487B65  xor     eax, eax
487B67  pop     ebx
487B68  pop     ebp
487B69  retn

;==== 487B70 sub_487B70
487B70  push    ebp
487B71  mov     ebp, esp
487B73  push    ebx
487B74  push    esi
487B75  mov     esi, [ebp+arg_0]
487B78  push    edi
487B79  mov     ecx, esi
487B7B  call    sub_445480
487B80  mov     ecx, esi
487B82  mov     edi, eax
487B84  call    sub_445480
487B89  push    edi
487B8A  mov     ebx, eax
487B8C  call    sub_498560
487B91  push    ebx
487B92  call    sub_4984C0
487B97  add     esp, 8
487B9A  mov     edx, edi
487B9C  mov     ecx, ebx
487B9E  call    sub_494680
487BA3  pop     edi
487BA4  pop     esi
487BA5  xor     eax, eax
487BA7  pop     ebx
487BA8  pop     ebp
487BA9  retn

;==== 487EC0 sub_487EC0
487EC0  push    ebp
487EC1  mov     ebp, esp
487EC3  push    ebx
487EC4  push    esi
487EC5  mov     esi, [ebp+arg_0]
487EC8  push    edi
487EC9  mov     ecx, esi
487ECB  call    sub_445480
487ED0  mov     ecx, esi
487ED2  mov     ebx, eax
487ED4  call    sub_445480
487ED9  mov     edi, eax
487EDB  push    edi
487EDC  call    sub_4985B0
487EE1  xor     eax, eax
487EE3  test    ebx, ebx
487EE5  setz    al
487EE8  push    eax
487EE9  push    edi
487EEA  call    sub_4A3CC0
487EEF  add     esp, 0Ch
487EF2  pop     edi
487EF3  pop     esi
487EF4  xor     eax, eax
487EF6  pop     ebx
487EF7  pop     ebp
487EF8  retn

;==== 487F50 sub_487F50
487F50  push    ebp
487F51  mov     ebp, esp
487F53  push    ecx
487F54  push    ebx
487F55  push    esi
487F56  mov     esi, [ebp+arg_0]
487F59  push    edi
487F5A  mov     ecx, esi
487F5C  call    sub_445480
487F61  mov     ecx, esi
487F63  mov     [ebp+var_4], eax
487F66  call    sub_445480
487F6B  mov     ecx, esi
487F6D  mov     edi, eax
487F6F  call    sub_445480
487F74  push    edi
487F75  mov     ebx, eax
487F77  call    sub_498560
487F7C  push    ebx
487F7D  call    sub_4985B0
487F82  mov     eax, [ebp+var_4]
487F85  push    eax
487F86  push    edi
487F87  push    ebx
487F88  call    sub_4A3D50
487F8D  add     esp, 14h
487F90  pop     edi
487F91  pop     esi
487F92  xor     eax, eax
487F94  pop     ebx
487F95  mov     esp, ebp
487F97  pop     ebp
487F98  retn

;==== 487FA0 sub_487FA0
487FA0  push    ebp
487FA1  mov     ebp, esp
487FA3  push    ebx
487FA4  push    esi
487FA5  mov     esi, [ebp+arg_0]
487FA8  push    edi
487FA9  mov     ecx, esi
487FAB  call    sub_445480
487FB0  mov     ecx, esi
487FB2  mov     edi, eax
487FB4  call    sub_445480
487FB9  push    edi
487FBA  mov     ebx, eax
487FBC  call    sub_498510
487FC1  push    ebx
487FC2  call    sub_4985B0
487FC7  push    edi
487FC8  push    ebx
487FC9  call    sub_4A3F00
487FCE  add     esp, 10h
487FD1  pop     edi
487FD2  pop     esi
487FD3  xor     eax, eax
487FD5  pop     ebx
487FD6  pop     ebp
487FD7  retn

;==== 487FE0 sub_487FE0
487FE0  push    ebp
487FE1  mov     ebp, esp
487FE3  push    ebx
487FE4  push    esi
487FE5  mov     esi, [ebp+arg_0]
487FE8  push    edi
487FE9  mov     ecx, esi
487FEB  call    sub_445480
487FF0  mov     ecx, esi
487FF2  mov     ebx, eax
487FF4  call    sub_445480
487FF9  mov     edi, eax
487FFB  push    edi
487FFC  call    sub_4985B0
488001  push    ebx
488002  push    edi
488003  call    sub_4A3BA0
488008  add     esp, 0Ch
48800B  pop     edi
48800C  pop     esi
48800D  xor     eax, eax
48800F  pop     ebx
488010  pop     ebp
488011  retn

;==== 488020 sub_488020
488020  push    ebp
488021  mov     ebp, esp
488023  push    ebx
488024  push    esi
488025  mov     esi, [ebp+arg_0]
488028  push    edi
488029  mov     ecx, esi
48802B  call    sub_445480
488030  mov     ecx, esi
488032  mov     ebx, eax
488034  call    sub_445480
488039  mov     edi, eax
48803B  push    edi
48803C  call    sub_4985B0
488041  push    ebx
488042  push    edi
488043  call    sub_4A3B10
488048  add     esp, 0Ch
48804B  pop     edi
48804C  pop     esi
48804D  xor     eax, eax
48804F  pop     ebx
488050  pop     ebp
488051  retn

;==== 488060 sub_488060
488060  push    ebp
488061  mov     ebp, esp
488063  push    ebx
488064  push    esi
488065  mov     esi, [ebp+arg_0]
488068  push    edi
488069  mov     ecx, esi
48806B  call    sub_445480
488070  mov     ecx, esi
488072  mov     edi, eax
488074  call    sub_445480
488079  push    edi
48807A  mov     ebx, eax
48807C  call    sub_498560
488081  push    ebx
488082  call    sub_4985B0
488087  push    edi
488088  push    ebx
488089  call    sub_4A3E70
48808E  add     esp, 10h
488091  pop     edi
488092  pop     esi
488093  xor     eax, eax
488095  pop     ebx
488096  pop     ebp
488097  retn

;==== 488240 sub_488240
488240  push    ebp
488241  mov     ebp, esp
488243  push    esi
488244  mov     esi, [ebp+arg_0]
488247  push    edi
488248  mov     ecx, esi
48824A  call    sub_445480
48824F  mov     edi, eax
488251  push    edi
488252  call    sub_4984C0
488257  add     esp, 4
48825A  mov     esi, edi
48825C  call    sub_494E70
488261  pop     edi
488262  xor     eax, eax
488264  pop     esi
488265  pop     ebp
488266  retn

;==== 488360 sub_488360
488360  push    ebp
488361  mov     ebp, esp
488363  sub     esp, 8
488366  push    ebx
488367  push    esi
488368  push    edi
488369  mov     edi, [ebp+arg_0]
48836C  mov     ecx, edi
48836E  call    sub_445480
488373  mov     edx, eax
488375  mov     ecx, edi
488377  mov     [ebp+var_8], edx
48837A  call    sub_445480
48837F  mov     ecx, edi
488381  mov     [ebp+var_4], eax
488384  call    sub_445480
488389  push    edx
48838A  mov     esi, edi
48838C  mov     ebx, eax
48838E  call    sub_498510
488393  mov     eax, [ebp+var_4]
488396  push    eax
488397  call    sub_498560
48839C  push    ebx
48839D  call    sub_4984C0
4883A2  mov     eax, [ebp+var_8]
4883A5  mov     ecx, [ebp+var_4]
4883A8  push    ebx
4883A9  call    sub_494E90
4883AE  add     esp, 10h
4883B1  test    eax, eax
4883B3  jz      short loc_4883CE
4883B5  cmp     eax, 14h
4883B8  jz      short loc_4883CE
4883BA  xor     eax, eax
4883BC  mov     esi, eax
4883BE  mov     eax, edi
4883C0  call    sub_4454A0
4883C5  pop     edi
4883C6  pop     esi
4883C7  xor     eax, eax
4883C9  pop     ebx
4883CA  mov     esp, ebp
4883CC  pop     ebp
4883CD  retn
4883CE  mov     ecx, ebx
4883D0  call    sub_494ED0
4883D5  mov     esi, eax
4883D7  mov     eax, edi
4883D9  call    sub_4454A0
4883DE  pop     edi
4883DF  pop     esi
4883E0  xor     eax, eax
4883E2  pop     ebx
4883E3  mov     esp, ebp
4883E5  pop     ebp
4883E6  retn

;==== 4883F0 sub_4883F0
4883F0  push    ebp
4883F1  mov     ebp, esp
4883F3  push    esi
4883F4  mov     esi, [ebp+arg_0]
4883F7  push    edi
4883F8  mov     ecx, esi
4883FA  call    sub_445480
4883FF  mov     edi, eax
488401  push    edi
488402  call    sub_4984C0
488407  add     esp, 4
48840A  mov     eax, edi
48840C  call    sub_494EB0
488411  pop     edi
488412  xor     eax, eax
488414  pop     esi
488415  pop     ebp
488416  retn

;==== 488420 sub_488420
488420  push    ebp
488421  mov     ebp, esp
488423  push    ebx
488424  push    esi
488425  mov     esi, [ebp+arg_0]
488428  push    edi
488429  mov     ecx, esi
48842B  call    sub_445480
488430  mov     ecx, esi
488432  mov     ebx, eax
488434  call    sub_445480
488439  mov     edi, eax
48843B  push    edi
48843C  call    sub_4984C0
488441  add     esp, 4
488444  mov     eax, ebx
488446  mov     ecx, edi
488448  call    sub_494EC0
48844D  pop     edi
48844E  pop     esi
48844F  xor     eax, eax
488451  pop     ebx
488452  pop     ebp
488453  retn

;==== 488660 sub_488660
488660  push    ebp
488661  mov     ebp, esp
488663  push    ebx
488664  push    esi
488665  mov     esi, [ebp+arg_0]
488668  push    edi
488669  mov     ecx, esi
48866B  call    sub_445480
488670  mov     ecx, esi
488672  mov     edi, eax
488674  call    sub_445480
488679  push    edi
48867A  mov     ebx, eax
48867C  call    sub_498560
488681  push    ebx
488682  call    sub_4984C0
488687  push    edi
488688  push    ebx
488689  call    sub_4A39F0
48868E  add     esp, 10h
488691  pop     edi
488692  pop     esi
488693  xor     eax, eax
488695  pop     ebx
488696  pop     ebp
488697  retn

;==== 4886A0 sub_4886A0
4886A0  push    ebp
4886A1  mov     ebp, esp
4886A3  push    ebx
4886A4  push    esi
4886A5  push    edi
4886A6  mov     edi, [ebp+arg_0]
4886A9  mov     ecx, edi
4886AB  call    sub_445480
4886B0  mov     ebx, eax
4886B2  push    ebx
4886B3  mov     esi, edi
4886B5  call    sub_4984C0
4886BA  add     esp, 4
4886BD  mov     ecx, ebx
4886BF  call    sub_494ED0
4886C4  mov     esi, eax
4886C6  mov     eax, edi
4886C8  call    sub_4454A0
4886CD  pop     edi
4886CE  pop     esi
4886CF  xor     eax, eax
4886D1  pop     ebx
4886D2  pop     ebp
4886D3  retn

;==== 494650 sub_494650
494650  xor     eax, eax
494652  cmp     ecx, 10h
494655  jnb     short locret_49467D
494657  cmp     edx, 80h
49465D  ja      short locret_49467D
49465F  mov     dword_55FF28[ecx*4], edx
494666  cmp     dword_5669A8, eax
49466C  jnz     short loc_494678
49466E  push    edx
49466F  push    ecx
494670  call    sub_4A3DE0
494675  add     esp, 8
494678  mov     eax, 1
49467D  retn

;==== 494680 sub_494680
494680  xor     eax, eax
494682  cmp     ecx, 40h ; '@'
494685  jnb     short locret_4946AD
494687  cmp     edx, 80h
49468D  ja      short locret_4946AD
49468F  mov     dword_55EE28[ecx*4], edx
494696  cmp     dword_5669A8, eax
49469C  jnz     short loc_4946A8
49469E  push    edx
49469F  push    ecx
4946A0  call    sub_4A3960
4946A5  add     esp, 8
4946A8  mov     eax, 1
4946AD  retn

;==== 494E70 sub_494E70
494E70  mov     eax, esi
494E72  push    40h ; '@'; Size
494E74  shl     eax, 6
494E77  add     eax, offset dword_55EF28
494E7C  push    0; Val
494E7E  push    eax; void *
494E7F  call    _memset
494E84  push    esi
494E85  call    sub_4A3280
494E8A  add     esp, 10h
494E8D  retn

;==== 494E90 sub_494E90
494E90  push    ebp
494E91  mov     ebp, esp
494E93  mov     edx, [ebp+arg_0]
494E96  push    eax
494E97  push    ecx
494E98  push    edx
494E99  call    sub_4A4920
494E9E  add     esp, 0Ch
494EA1  pop     ebp
494EA2  retn

;==== 494EB0 sub_494EB0
494EB0  push    eax
494EB1  call    sub_4A3A80
494EB6  add     esp, 4
494EB9  retn

;==== 494EC0 sub_494EC0
494EC0  push    eax
494EC1  push    ecx
494EC2  call    sub_4A3C30
494EC7  add     esp, 8
494ECA  retn

;==== 494ED0 sub_494ED0
494ED0  push    ebp
494ED1  mov     ebp, esp
494ED3  shl     ecx, 6
494ED6  mov     edx, dword_55EF38[ecx]
494EDC  sub     esp, 0Ch
494EDF  xor     eax, eax
494EE1  test    edx, edx
494EE3  jz      short loc_494F49
494EE5  mov     eax, dword_55EF34[ecx]
494EEB  fild    dword_55EF34[ecx]
494EF1  test    eax, eax
494EF3  jns     short loc_494EFB
494EF5  fadd    ds:dbl_4EC908
494EFB  fmul    ds:dbl_4EC928
494F01  mov     [ebp+var_8], edx
494F04  fild    [ebp+var_8]
494F07  test    edx, edx
494F09  jns     short loc_494F11
494F0B  fadd    ds:dbl_4EC908
494F11  mov     ecx, dword_55EF64[ecx]
494F17  fdivp   st(1), st
494F19  mov     [ebp+var_8], ecx
494F1C  fild    [ebp+var_8]
494F1F  test    ecx, ecx
494F21  jns     short loc_494F29
494F23  fadd    ds:dbl_4EC908
494F29  fnstcw  [ebp+var_2]
494F2C  movzx   eax, [ebp+var_2]
494F30  fmulp   st(1), st
494F32  or      eax, 0C00h
494F37  mov     [ebp+var_8], eax
494F3A  fldcw   word ptr [ebp+var_8]
494F3D  fistp   qword ptr [ebp-0Ch]
494F40  mov     eax, [ebp+var_C]
494F43  shr     eax, 10h
494F46  fldcw   [ebp+var_2]
494F49  mov     esp, ebp
494F4B  pop     ebp
494F4C  retn

;==== 4984C0 sub_4984C0
4984C0  push    ebp
4984C1  mov     ebp, esp
4984C3  sub     esp, 104h
4984C9  mov     eax, dword_4FB734
4984CE  xor     eax, ebp
4984D0  mov     [ebp+var_4], eax
4984D3  mov     eax, [ebp+arg_0]
4984D6  cmp     eax, 40h ; '@'
4984D9  jb      short loc_4984FC
4984DB  push    eax
4984DC  lea     eax, [ebp+Buffer]
4984E2  push    offset byte_4EC3E4; Format
4984E7  push    eax; Buffer
4984E8  call    _sprintf
4984ED  add     esp, 0Ch
4984F0  push    esi
4984F1  lea     edi, [ebp+Buffer]
4984F7  call    sub_464C50
4984FC  mov     ecx, [ebp+var_4]
4984FF  xor     ecx, ebp
498501  call    sub_4ABDD5
498506  mov     esp, ebp
498508  pop     ebp
498509  retn

;==== 498510 sub_498510
498510  push    ebp
498511  mov     ebp, esp
498513  sub     esp, 104h
498519  mov     eax, dword_4FB734
49851E  xor     eax, ebp
498520  mov     [ebp+var_4], eax
498523  mov     eax, [ebp+arg_0]
498526  test    eax, eax
498528  js      short loc_498531
49852A  cmp     eax, 80h
49852F  jle     short loc_498552
498531  push    eax
498532  lea     eax, [ebp+Buffer]
498538  push    offset byte_4EC410; Format
49853D  push    eax; Buffer
49853E  call    _sprintf
498543  add     esp, 0Ch
498546  push    esi
498547  lea     edi, [ebp+Buffer]
49854D  call    sub_464C50
498552  mov     ecx, [ebp+var_4]
498555  xor     ecx, ebp
498557  call    sub_4ABDD5
49855C  mov     esp, ebp
49855E  pop     ebp
49855F  retn

;==== 498560 sub_498560
498560  push    ebp
498561  mov     ebp, esp
498563  sub     esp, 104h
498569  mov     eax, dword_4FB734
49856E  xor     eax, ebp
498570  mov     [ebp+var_4], eax
498573  mov     eax, [ebp+arg_0]
498576  test    eax, eax
498578  js      short loc_498581
49857A  cmp     eax, 80h
49857F  jle     short loc_4985A2
498581  push    eax
498582  lea     eax, [ebp+Buffer]
498588  push    offset byte_4EC444; Format
49858D  push    eax; Buffer
49858E  call    _sprintf
498593  add     esp, 0Ch
498596  push    esi
498597  lea     edi, [ebp+Buffer]
49859D  call    sub_464C50
4985A2  mov     ecx, [ebp+var_4]
4985A5  xor     ecx, ebp
4985A7  call    sub_4ABDD5
4985AC  mov     esp, ebp
4985AE  pop     ebp
4985AF  retn

;==== 4985B0 sub_4985B0
4985B0  push    ebp
4985B1  mov     ebp, esp
4985B3  sub     esp, 104h
4985B9  mov     eax, dword_4FB734
4985BE  xor     eax, ebp
4985C0  mov     [ebp+var_4], eax
4985C3  mov     eax, [ebp+arg_0]
4985C6  cmp     eax, 10h
4985C9  jb      short loc_4985EC
4985CB  push    eax
4985CC  lea     eax, [ebp+Buffer]
4985D2  push    offset byte_4EC478; Format
4985D7  push    eax; Buffer
4985D8  call    _sprintf
4985DD  add     esp, 0Ch
4985E0  push    esi
4985E1  lea     edi, [ebp+Buffer]
4985E7  call    sub_464C50
4985EC  mov     ecx, [ebp+var_4]
4985EF  xor     ecx, ebp
4985F1  call    sub_4ABDD5
4985F6  mov     esp, ebp
4985F8  pop     ebp
4985F9  retn

;==== 4A2B50 sub_4A2B50
4A2B50  push    ebp
4A2B51  mov     ebp, esp
4A2B53  mov     eax, dword_5085A4
4A2B58  and     eax, 3
4A2B5B  cmp     al, 3
4A2B5D  jz      short loc_4A2B66
4A2B5F  mov     eax, 14h
4A2B64  pop     ebp
4A2B65  retn
4A2B66  mov     eax, [ebp+arg_0]
4A2B69  cmp     eax, 40h ; '@'
4A2B6C  jb      short loc_4A2B75
4A2B6E  mov     eax, 15h
4A2B73  pop     ebp
4A2B74  retn
4A2B75  push    esi
4A2B76  mov     esi, eax
4A2B78  shl     esi, 4
4A2B7B  add     esi, eax
4A2B7D  add     esi, esi
4A2B7F  mov     ecx, dword_508A0C[esi+esi]
4A2B86  mov     edx, [ecx]
4A2B88  mov     eax, [edx+1Ch]
4A2B8B  add     esi, esi
4A2B8D  call    eax
4A2B8F  test    eax, eax
4A2B91  jz      short loc_4A2B9D
4A2B93  mov     edx, [eax]
4A2B95  mov     ecx, eax
4A2B97  mov     eax, [edx]
4A2B99  push    1
4A2B9B  call    eax
4A2B9D  mov     dword_508A10[esi], 0
4A2BA7  mov     dword_508A08[esi], 0
4A2BB1  xor     eax, eax
4A2BB3  pop     esi
4A2BB4  pop     ebp
4A2BB5  retn

;==== 4A2CB0 sub_4A2CB0
4A2CB0  push    ebp
4A2CB1  mov     ebp, esp
4A2CB3  mov     eax, dword_5085A4
4A2CB8  and     eax, 3
4A2CBB  cmp     al, 3
4A2CBD  jz      short loc_4A2CC6
4A2CBF  mov     eax, 14h
4A2CC4  pop     ebp
4A2CC5  retn
4A2CC6  mov     eax, [ebp+arg_0]
4A2CC9  cmp     eax, 40h ; '@'
4A2CCC  jb      short loc_4A2CD5
4A2CCE  mov     eax, 15h
4A2CD3  pop     ebp
4A2CD4  retn
4A2CD5  mov     ecx, eax
4A2CD7  shl     ecx, 4
4A2CDA  add     ecx, eax
4A2CDC  mov     eax, dword_508A08[ecx*4]
4A2CE3  neg     eax
4A2CE5  sbb     eax, eax
4A2CE7  and     eax, 0FFFFFFEDh
4A2CEA  add     eax, 13h
4A2CED  pop     ebp
4A2CEE  retn

;==== 4A2CF0 sub_4A2CF0
4A2CF0  push    ebp
4A2CF1  mov     ebp, esp
4A2CF3  mov     eax, dword_5085A4
4A2CF8  and     eax, 3
4A2CFB  cmp     al, 3
4A2CFD  jz      short loc_4A2D06
4A2CFF  mov     eax, 14h
4A2D04  pop     ebp
4A2D05  retn
4A2D06  mov     eax, [ebp+arg_0]
4A2D09  cmp     eax, 10h
4A2D0C  jb      short loc_4A2D15
4A2D0E  mov     eax, 15h
4A2D13  pop     ebp
4A2D14  retn
4A2D15  mov     ecx, eax
4A2D17  shl     ecx, 4
4A2D1A  add     ecx, eax
4A2D1C  mov     eax, dword_5085A8[ecx*4]
4A2D23  neg     eax
4A2D25  sbb     eax, eax
4A2D27  and     eax, 0FFFFFFEDh
4A2D2A  add     eax, 13h
4A2D2D  pop     ebp
4A2D2E  retn

;==== 4A2D30 sub_4A2D30
4A2D30  push    ebp
4A2D31  mov     ebp, esp
4A2D33  push    ecx
4A2D34  mov     eax, [ebp+arg_0]
4A2D37  mov     ecx, [eax+40h]
4A2D3A  push    ebx
4A2D3B  mov     ebx, [eax+28h]
4A2D3E  push    esi
4A2D3F  mov     esi, [eax+0Ch]
4A2D42  push    edi
4A2D43  mov     edi, [eax+10h]
4A2D46  mov     eax, 200h
4A2D4B  sub     eax, esi
4A2D4D  sub     eax, edi
4A2D4F  sub     eax, ebx
4A2D51  sub     eax, ecx
4A2D53  mov     [ebp+arg_0], eax
4A2D56  fild    [ebp+arg_0]
4A2D59  mov     [ebp+var_4], ecx
4A2D5C  fld     ds:dbl_4DBA20
4A2D62  fdiv    st(1), st
4A2D64  fxch    st(1)
4A2D66  call    __ftol2_sse
4A2D6B  mov     edx, 80h
4A2D70  sub     edx, esi
4A2D72  mov     [ebp+arg_0], edx
4A2D75  fild    [ebp+arg_0]
4A2D78  mov     ecx, eax
4A2D7A  fdiv    st, st(1)
4A2D7C  fld     ds:dbl_4DBA18
4A2D82  fcom    st(1)
4A2D84  fnstsw  ax
4A2D86  fstp    st(1)
4A2D88  test    ah, 41h
4A2D8B  jnp     short loc_4A2DDF
4A2D8D  mov     eax, 80h
4A2D92  sub     eax, edi
4A2D94  mov     [ebp+arg_0], eax
4A2D97  fild    [ebp+arg_0]
4A2D9A  fdiv    st, st(2)
4A2D9C  fcomp   st(1)
4A2D9E  fnstsw  ax
4A2DA0  test    ah, 1
4A2DA3  jz      short loc_4A2DDF
4A2DA5  mov     edx, 80h
4A2DAA  sub     edx, ebx
4A2DAC  mov     [ebp+arg_0], edx
4A2DAF  fild    [ebp+arg_0]
4A2DB2  fdiv    st, st(2)
4A2DB4  fcomp   st(1)
4A2DB6  fnstsw  ax
4A2DB8  test    ah, 1
4A2DBB  jz      short loc_4A2DDF
4A2DBD  mov     eax, 80h
4A2DC2  sub     eax, [ebp+var_4]
4A2DC5  mov     [ebp+arg_0], eax
4A2DC8  fild    [ebp+arg_0]
4A2DCB  fdivrp  st(2), st
4A2DCD  fcompp
4A2DCF  fnstsw  ax
4A2DD1  test    ah, 41h
4A2DD4  jnp     short loc_4A2DE3
4A2DD6  pop     edi
4A2DD7  pop     esi
4A2DD8  mov     eax, ecx
4A2DDA  pop     ebx
4A2DDB  mov     esp, ebp
4A2DDD  pop     ebp
4A2DDE  retn
4A2DDF  fstp    st
4A2DE1  fstp    st
4A2DE3  pop     edi
4A2DE4  pop     esi
4A2DE5  mov     eax, 80h
4A2DEA  pop     ebx
4A2DEB  mov     esp, ebp
4A2DED  pop     ebp
4A2DEE  retn

;==== 4A2DF0 sub_4A2DF0
4A2DF0  push    ebp
4A2DF1  mov     ebp, esp
4A2DF3  sub     esp, 0Ch
4A2DF6  fld1
4A2DF8  push    esi
4A2DF9  mov     esi, [ebp+arg_0]
4A2DFC  mov     ecx, [esi+4]
4A2DFF  mov     eax, [esi+8]
4A2E02  mov     edx, [esi+0Ch]
4A2E05  push    edi
4A2E06  mov     edi, [esi+10h]
4A2E09  sub     eax, ecx
4A2E0B  sub     edi, edx
4A2E0D  mov     [ebp+var_4], eax
4A2E10  mov     [ebp+var_C], edx
4A2E13  mov     [ebp+var_8], edi
4A2E16  test    eax, eax
4A2E18  jle     short loc_4A2E33
4A2E1A  mov     eax, [ebp+arg_4]
4A2E1D  sub     eax, ecx
4A2E1F  mov     [ebp+arg_0], eax
4A2E22  fild    [ebp+arg_0]
4A2E25  fidiv   [ebp+var_4]
4A2E28  fcom    st(1)
4A2E2A  fnstsw  ax
4A2E2C  test    ah, 1
4A2E2F  jnz     short loc_4A2E4D
4A2E31  fstp    st
4A2E33  fimul   [ebp+var_8]
4A2E36  mov     dword ptr [esi], 0
4A2E3C  fiadd   [ebp+var_C]
4A2E3F  call    __ftol2_sse
4A2E44  pop     edi
4A2E45  mov     [esi+14h], eax
4A2E48  pop     esi
4A2E49  mov     esp, ebp
4A2E4B  pop     ebp
4A2E4C  retn
4A2E4D  fstp    st(1)
4A2E4F  fimul   [ebp+var_8]
4A2E52  fiadd   [ebp+var_C]
4A2E55  call    __ftol2_sse
4A2E5A  pop     edi
4A2E5B  mov     [esi+14h], eax
4A2E5E  pop     esi
4A2E5F  mov     esp, ebp
4A2E61  pop     ebp
4A2E62  retn

;==== 4A2E70 sub_4A2E70
4A2E70  push    ebp
4A2E71  mov     ebp, esp
4A2E73  push    ecx
4A2E74  mov     eax, [ebp+arg_4]
4A2E77  test    eax, eax
4A2E79  jle     short loc_4A2EEA
4A2E7B  push    ebx
4A2E7C  push    esi
4A2E7D  mov     esi, [ebp+arg_0]
4A2E80  push    edi
4A2E81  mov     edi, [ebp+arg_8]
4A2E84  add     esi, 2Ch ; ','
4A2E87  mov     [ebp+arg_4], eax
4A2E8A  lea     ebx, [ebx+0]
4A2E90  xor     ecx, ecx
4A2E92  cmp     [esi-2Ch], ecx
4A2E95  lea     ebx, [esi-2Ch]
4A2E98  jz      short loc_4A2EDF
4A2E9A  cmp     [esi-18h], ecx
4A2E9D  lea     eax, [esi-18h]
4A2EA0  jz      short loc_4A2EB1
4A2EA2  push    edi
4A2EA3  push    eax
4A2EA4  call    sub_4A2DF0
4A2EA9  add     esp, 8
4A2EAC  mov     ecx, 1
4A2EB1  cmp     dword ptr [esi], 0
4A2EB4  jz      short loc_4A2EC2
4A2EB6  push    edi
4A2EB7  push    esi
4A2EB8  call    sub_4A2DF0
4A2EBD  add     esp, 8
4A2EC0  jmp     short loc_4A2EC6
4A2EC2  test    ecx, ecx
4A2EC4  jz      short loc_4A2EDF
4A2EC6  mov     edi, [esi-28h]
4A2EC9  push    ebx
4A2ECA  call    sub_4A2D30
4A2ECF  add     esp, 4
4A2ED2  push    eax
4A2ED3  mov     eax, [edi]
4A2ED5  mov     eax, [eax+18h]
4A2ED8  mov     ecx, edi
4A2EDA  call    eax
4A2EDC  mov     edi, [ebp+arg_8]
4A2EDF  add     esi, 44h ; 'D'
4A2EE2  dec     [ebp+arg_4]
4A2EE5  jnz     short loc_4A2E90
4A2EE7  pop     edi
4A2EE8  pop     esi
4A2EE9  pop     ebx
4A2EEA  mov     esp, ebp
4A2EEC  pop     ebp
4A2EED  retn

;==== 4A3280 sub_4A3280
4A3280  push    ebp
4A3281  mov     ebp, esp
4A3283  push    0FFFFFFFFh
4A3285  push    offset SEH_4A3280
4A328A  mov     eax, large fs:0
4A3290  push    eax
4A3291  sub     esp, 8
4A3294  push    ebx
4A3295  push    esi
4A3296  push    edi
4A3297  mov     eax, dword_4FB734
4A329C  xor     eax, ebp
4A329E  push    eax
4A329F  lea     eax, [ebp+var_C]
4A32A2  mov     large fs:0, eax
4A32A8  mov     [ebp+var_10], esp
4A32AB  mov     ecx, offset dword_5089E8
4A32B0  call    sub_4A55F0
4A32B5  mov     [ebp+var_4], 0
4A32BC  mov     eax, [ebp+arg_0]
4A32BF  push    eax
4A32C0  call    sub_4A2B50
4A32C5  add     esp, 4
4A32C8  mov     esi, eax
4A32CA  mov     [ebp+var_14], esi
4A32CD  mov     ecx, offset dword_5089E8
4A32D2  call    sub_4A5600
4A32D7  mov     [ebp+var_4], 0FFFFFFFFh
4A32DE  mov     eax, esi
4A32E0  mov     ecx, [ebp+var_C]
4A32E3  mov     large fs:0, ecx
4A32EA  pop     ecx
4A32EB  pop     edi
4A32EC  pop     esi
4A32ED  pop     ebx
4A32EE  mov     esp, ebp
4A32F0  pop     ebp
4A32F1  retn
4A32F2  mov     ecx, offset dword_5089E8
4A32F7  call    sub_4A5600
4A32FC  push    0; pThrowInfo
4A32FE  push    0; pExceptionObject
4A3300  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7560  mov     edx, [esp-4+arg_4]
4D7564  lea     eax, [edx+0Ch]
4D7567  mov     ecx, [edx-18h]
4D756A  xor     ecx, eax
4D756C  call    sub_4ABDD5
4D7571  mov     eax, offset stru_4F3FA8
4D7576  jmp     ___CxxFrameHandler3

;==== 4A3390 sub_4A3390
4A3390  push    ebp
4A3391  mov     ebp, esp
4A3393  mov     eax, dword_5085A4
4A3398  and     eax, 3
4A339B  cmp     al, 3
4A339D  jz      short loc_4A33A6
4A339F  mov     eax, 14h
4A33A4  pop     ebp
4A33A5  retn
4A33A6  mov     edx, [ebp+arg_0]
4A33A9  cmp     edx, 40h ; '@'
4A33AC  jb      short loc_4A33B5
4A33AE  mov     eax, 15h
4A33B3  pop     ebp
4A33B4  retn
4A33B5  mov     ecx, [ebp+arg_4]
4A33B8  cmp     ecx, 80h
4A33BE  jbe     short loc_4A33C5
4A33C0  mov     ecx, 80h
4A33C5  mov     eax, edx
4A33C7  shl     eax, 4
4A33CA  add     eax, edx
4A33CC  add     eax, eax
4A33CE  add     eax, eax
4A33D0  push    esi
4A33D1  mov     esi, dword_508A0C[eax]
4A33D7  mov     dword_508A14[eax], ecx
4A33DD  push    edi
4A33DE  mov     edi, [esi]
4A33E0  lea     ecx, dword_508A08[eax]
4A33E6  push    ecx
4A33E7  call    sub_4A2D30
4A33EC  mov     edx, [edi+18h]
4A33EF  add     esp, 4
4A33F2  push    eax
4A33F3  mov     ecx, esi
4A33F5  call    edx
4A33F7  pop     edi
4A33F8  xor     eax, eax
4A33FA  pop     esi
4A33FB  pop     ebp
4A33FC  retn

;==== 4A3400 sub_4A3400
4A3400  push    ebp
4A3401  mov     ebp, esp
4A3403  mov     eax, dword_5085A4
4A3408  and     eax, 3
4A340B  cmp     al, 3
4A340D  jz      short loc_4A3416
4A340F  mov     eax, 14h
4A3414  pop     ebp
4A3415  retn
4A3416  mov     edx, [ebp+arg_0]
4A3419  cmp     edx, 40h ; '@'
4A341C  jb      short loc_4A3425
4A341E  mov     eax, 15h
4A3423  pop     ebp
4A3424  retn
4A3425  mov     ecx, [ebp+arg_4]
4A3428  cmp     ecx, 80h
4A342E  jbe     short loc_4A3435
4A3430  mov     ecx, 80h
4A3435  mov     eax, edx
4A3437  shl     eax, 4
4A343A  add     eax, edx
4A343C  add     eax, eax
4A343E  add     eax, eax
4A3440  push    esi
4A3441  mov     esi, dword_508A0C[eax]
4A3447  mov     dword_508A18[eax], ecx
4A344D  push    edi
4A344E  mov     edi, [esi]
4A3450  lea     ecx, dword_508A08[eax]
4A3456  push    ecx
4A3457  call    sub_4A2D30
4A345C  mov     edx, [edi+18h]
4A345F  add     esp, 4
4A3462  push    eax
4A3463  mov     ecx, esi
4A3465  call    edx
4A3467  pop     edi
4A3468  xor     eax, eax
4A346A  pop     esi
4A346B  pop     ebp
4A346C  retn

;==== 4A3470 sub_4A3470
4A3470  push    ebp
4A3471  mov     ebp, esp
4A3473  push    ecx
4A3474  mov     edx, [ebp+arg_0]
4A3477  push    edx
4A3478  call    sub_4A2CB0
4A347D  add     esp, 4
4A3480  test    eax, eax
4A3482  jnz     short loc_4A34BF
4A3484  mov     ecx, [ebp+arg_4]
4A3487  add     ecx, 0FFFFFFC0h
4A348A  mov     [ebp+var_4], ecx
4A348D  fild    [ebp+var_4]
4A3490  mov     eax, edx
4A3492  shl     eax, 4
4A3495  add     eax, edx
4A3497  fmul    ds:dbl_4DBB48
4A349D  push    esi
4A349E  mov     esi, dword_508A0C[eax*4]
4A34A5  push    edi
4A34A6  fmul    ds:dbl_4EC8E8
4A34AC  mov     edi, [esi]
4A34AE  call    __ftol2_sse
4A34B3  mov     edx, [edi+14h]
4A34B6  push    eax
4A34B7  mov     ecx, esi
4A34B9  call    edx
4A34BB  pop     edi
4A34BC  xor     eax, eax
4A34BE  pop     esi
4A34BF  mov     esp, ebp
4A34C1  pop     ebp
4A34C2  retn

;==== 4A34D0 sub_4A34D0
4A34D0  push    ebp
4A34D1  mov     ebp, esp
4A34D3  mov     edx, [ebp+arg_0]
4A34D6  push    edx
4A34D7  call    sub_4A2CB0
4A34DC  add     esp, 4
4A34DF  test    eax, eax
4A34E1  jnz     short loc_4A34FA
4A34E3  mov     eax, edx
4A34E5  shl     eax, 4
4A34E8  add     eax, edx
4A34EA  mov     ecx, dword_508A0C[eax*4]
4A34F1  mov     edx, [ecx]
4A34F3  mov     eax, [edx+10h]
4A34F6  call    eax
4A34F8  xor     eax, eax
4A34FA  pop     ebp
4A34FB  retn

;==== 4A3500 sub_4A3500
4A3500  push    ebp
4A3501  mov     ebp, esp
4A3503  push    esi
4A3504  mov     esi, [ebp+arg_0]
4A3507  push    esi
4A3508  call    sub_4A2CF0
4A350D  add     esp, 4
4A3510  test    eax, eax
4A3512  jnz     short loc_4A3556
4A3514  call    ds:GetTickCount
4A351A  mov     ecx, esi
4A351C  shl     ecx, 4
4A351F  add     ecx, esi
4A3521  add     ecx, ecx
4A3523  add     ecx, ecx
4A3525  mov     dword_5085D8[ecx], eax
4A352B  add     eax, [ebp+arg_4]
4A352E  mov     dword_5085E4[ecx], 0
4A3538  mov     dword_5085DC[ecx], eax
4A353E  mov     eax, dword_5085E8[ecx]
4A3544  mov     dword_5085E0[ecx], eax
4A354A  mov     dword_5085D4[ecx], 1
4A3554  xor     eax, eax
4A3556  pop     esi
4A3557  pop     ebp
4A3558  retn

;==== 4A3560 sub_4A3560
4A3560  push    ebp
4A3561  mov     ebp, esp
4A3563  push    esi
4A3564  mov     esi, [ebp+arg_0]
4A3567  push    esi
4A3568  call    sub_4A2CF0
4A356D  add     esp, 4
4A3570  test    eax, eax
4A3572  jnz     short loc_4A35B6
4A3574  call    ds:GetTickCount
4A357A  mov     ecx, esi
4A357C  shl     ecx, 4
4A357F  add     ecx, esi
4A3581  add     ecx, ecx
4A3583  add     ecx, ecx
4A3585  mov     dword_5085D8[ecx], eax
4A358B  add     eax, [ebp+arg_4]
4A358E  mov     dword_5085E4[ecx], 80h
4A3598  mov     dword_5085DC[ecx], eax
4A359E  mov     eax, dword_5085E8[ecx]
4A35A4  mov     dword_5085E0[ecx], eax
4A35AA  mov     dword_5085D4[ecx], 1
4A35B4  xor     eax, eax
4A35B6  pop     esi
4A35B7  pop     ebp
4A35B8  retn

;==== 4A35C0 sub_4A35C0
4A35C0  push    ebp
4A35C1  mov     ebp, esp
4A35C3  push    esi
4A35C4  mov     esi, [ebp+arg_0]
4A35C7  push    esi
4A35C8  call    sub_4A2CB0
4A35CD  add     esp, 4
4A35D0  test    eax, eax
4A35D2  jnz     short loc_4A3616
4A35D4  call    ds:GetTickCount
4A35DA  mov     ecx, esi
4A35DC  shl     ecx, 4
4A35DF  add     ecx, esi
4A35E1  add     ecx, ecx
4A35E3  add     ecx, ecx
4A35E5  mov     dword_508A38[ecx], eax
4A35EB  add     eax, [ebp+arg_4]
4A35EE  mov     dword_508A44[ecx], 0
4A35F8  mov     dword_508A3C[ecx], eax
4A35FE  mov     eax, dword_508A48[ecx]
4A3604  mov     dword_508A40[ecx], eax
4A360A  mov     dword_508A34[ecx], 1
4A3614  xor     eax, eax
4A3616  pop     esi
4A3617  pop     ebp
4A3618  retn

;==== 4A3620 sub_4A3620
4A3620  push    ebp
4A3621  mov     ebp, esp
4A3623  mov     edx, [ebp+arg_0]
4A3626  push    edx
4A3627  call    sub_4A2CF0
4A362C  add     esp, 4
4A362F  test    eax, eax
4A3631  jnz     short loc_4A364E
4A3633  mov     eax, edx
4A3635  shl     eax, 4
4A3638  add     eax, edx
4A363A  mov     ecx, dword_5085AC[eax*4]
4A3641  mov     edx, [ecx]
4A3643  mov     eax, [ebp+arg_4]
4A3646  mov     edx, [edx+0Ch]
4A3649  push    eax
4A364A  call    edx
4A364C  xor     eax, eax
4A364E  pop     ebp
4A364F  retn

;==== 4A3650 sub_4A3650
4A3650  push    ebp
4A3651  mov     ebp, esp
4A3653  push    esi
4A3654  mov     esi, [ebp+arg_0]
4A3657  push    esi
4A3658  call    sub_4A2CF0
4A365D  add     esp, 4
4A3660  test    eax, eax
4A3662  jnz     short loc_4A36B4
4A3664  push    edi
4A3665  mov     edi, [ebp+arg_4]
4A3668  cmp     edi, 80h
4A366E  jbe     short loc_4A3675
4A3670  mov     edi, 80h
4A3675  call    ds:GetTickCount
4A367B  mov     ecx, esi
4A367D  shl     ecx, 4
4A3680  add     ecx, esi
4A3682  add     ecx, ecx
4A3684  add     ecx, ecx
4A3686  mov     dword_5085C0[ecx], eax
4A368C  add     eax, [ebp+arg_8]
4A368F  mov     dword_5085CC[ecx], edi
4A3695  mov     dword_5085C4[ecx], eax
4A369B  mov     eax, dword_5085D0[ecx]
4A36A1  mov     dword_5085C8[ecx], eax
4A36A7  mov     dword_5085BC[ecx], 1
4A36B1  xor     eax, eax
4A36B3  pop     edi
4A36B4  pop     esi
4A36B5  pop     ebp
4A36B6  retn

;==== 4A36C0 sub_4A36C0
4A36C0  push    ebp
4A36C1  mov     ebp, esp
4A36C3  mov     eax, dword_5085A4
4A36C8  and     eax, 3
4A36CB  cmp     al, 3
4A36CD  jz      short loc_4A36D6
4A36CF  mov     eax, 14h
4A36D4  pop     ebp
4A36D5  retn
4A36D6  mov     ecx, [ebp+arg_0]
4A36D9  cmp     ecx, 10h
4A36DC  jb      short loc_4A36E5
4A36DE  mov     eax, 15h
4A36E3  pop     ebp
4A36E4  retn
4A36E5  mov     eax, ecx
4A36E7  shl     eax, 4
4A36EA  add     eax, ecx
4A36EC  mov     ecx, [ebp+arg_4]
4A36EF  add     eax, eax
4A36F1  add     eax, eax
4A36F3  push    esi
4A36F4  mov     esi, dword_5085AC[eax]
4A36FA  push    edi
4A36FB  lea     edx, dword_5085A8[eax]
4A3701  mov     dword_5085B4[eax], ecx
4A3707  mov     edi, [esi]
4A3709  push    edx
4A370A  call    sub_4A2D30
4A370F  add     esp, 4
4A3712  push    eax
4A3713  mov     eax, [edi+18h]
4A3716  mov     ecx, esi
4A3718  call    eax
4A371A  pop     edi
4A371B  xor     eax, eax
4A371D  pop     esi
4A371E  pop     ebp
4A371F  retn

;==== 4A3720 sub_4A3720
4A3720  push    ebp
4A3721  mov     ebp, esp
4A3723  mov     eax, dword_5085A4
4A3728  and     eax, 3
4A372B  cmp     al, 3
4A372D  jz      short loc_4A3736
4A372F  mov     eax, 14h
4A3734  pop     ebp
4A3735  retn
4A3736  mov     edx, [ebp+arg_0]
4A3739  cmp     edx, 10h
4A373C  jb      short loc_4A3745
4A373E  mov     eax, 15h
4A3743  pop     ebp
4A3744  retn
4A3745  mov     ecx, [ebp+arg_4]
4A3748  cmp     ecx, 80h
4A374E  jbe     short loc_4A3755
4A3750  mov     ecx, 80h
4A3755  mov     eax, edx
4A3757  shl     eax, 4
4A375A  add     eax, edx
4A375C  add     eax, eax
4A375E  add     eax, eax
4A3760  push    esi
4A3761  mov     esi, dword_5085AC[eax]
4A3767  mov     dword_5085B8[eax], ecx
4A376D  push    edi
4A376E  mov     edi, [esi]
4A3770  lea     ecx, dword_5085A8[eax]
4A3776  push    ecx
4A3777  call    sub_4A2D30
4A377C  mov     edx, [edi+18h]
4A377F  add     esp, 4
4A3782  push    eax
4A3783  mov     ecx, esi
4A3785  call    edx
4A3787  pop     edi
4A3788  xor     eax, eax
4A378A  pop     esi
4A378B  pop     ebp
4A378C  retn

;==== 4A3790 sub_4A3790
4A3790  push    ebp
4A3791  mov     ebp, esp
4A3793  push    ecx
4A3794  mov     edx, [ebp+arg_0]
4A3797  push    edx
4A3798  call    sub_4A2CF0
4A379D  add     esp, 4
4A37A0  test    eax, eax
4A37A2  jnz     short loc_4A37F3
4A37A4  mov     eax, [ebp+arg_4]
4A37A7  cmp     eax, 80h
4A37AC  jle     short loc_4A37B5
4A37AE  mov     eax, 80h
4A37B3  jmp     short loc_4A37BB
4A37B5  test    eax, eax
4A37B7  jns     short loc_4A37BB
4A37B9  xor     eax, eax
4A37BB  add     eax, 0FFFFFFC0h
4A37BE  mov     [ebp+var_4], eax
4A37C1  fild    [ebp+var_4]
4A37C4  mov     ecx, edx
4A37C6  shl     ecx, 4
4A37C9  add     ecx, edx
4A37CB  fmul    ds:dbl_4DBB48
4A37D1  push    esi
4A37D2  mov     esi, dword_5085AC[ecx*4]
4A37D9  push    edi
4A37DA  fmul    ds:dbl_4EC8E8
4A37E0  mov     edi, [esi]
4A37E2  call    __ftol2_sse
4A37E7  mov     edx, [edi+14h]
4A37EA  push    eax
4A37EB  mov     ecx, esi
4A37ED  call    edx
4A37EF  pop     edi
4A37F0  xor     eax, eax
4A37F2  pop     esi
4A37F3  mov     esp, ebp
4A37F5  pop     ebp
4A37F6  retn

;==== 4A3960 sub_4A3960
4A3960  push    ebp
4A3961  mov     ebp, esp
4A3963  push    0FFFFFFFFh
4A3965  push    offset SEH_4A3960
4A396A  mov     eax, large fs:0
4A3970  push    eax
4A3971  sub     esp, 8
4A3974  push    ebx
4A3975  push    esi
4A3976  push    edi
4A3977  mov     eax, dword_4FB734
4A397C  xor     eax, ebp
4A397E  push    eax
4A397F  lea     eax, [ebp+var_C]
4A3982  mov     large fs:0, eax
4A3988  mov     [ebp+var_10], esp
4A398B  mov     ecx, offset dword_5089E8
4A3990  call    sub_4A55F0
4A3995  mov     [ebp+var_4], 0
4A399C  mov     eax, [ebp+arg_4]
4A399F  push    eax
4A39A0  mov     ecx, [ebp+arg_0]
4A39A3  push    ecx
4A39A4  call    sub_4A3390
4A39A9  add     esp, 8
4A39AC  mov     esi, eax
4A39AE  mov     [ebp+var_14], esi
4A39B1  mov     ecx, offset dword_5089E8
4A39B6  call    sub_4A5600
4A39BB  mov     [ebp+var_4], 0FFFFFFFFh
4A39C2  mov     eax, esi
4A39C4  mov     ecx, [ebp+var_C]
4A39C7  mov     large fs:0, ecx
4A39CE  pop     ecx
4A39CF  pop     edi
4A39D0  pop     esi
4A39D1  pop     ebx
4A39D2  mov     esp, ebp
4A39D4  pop     ebp
4A39D5  retn
4A39D6  mov     ecx, offset dword_5089E8
4A39DB  call    sub_4A5600
4A39E0  push    0; pThrowInfo
4A39E2  push    0; pExceptionObject
4A39E4  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D75C0  mov     edx, [esp-4+arg_4]
4D75C4  lea     eax, [edx+0Ch]
4D75C7  mov     ecx, [edx-18h]
4D75CA  xor     ecx, eax
4D75CC  call    sub_4ABDD5
4D75D1  mov     eax, offset stru_4F40B0
4D75D6  jmp     ___CxxFrameHandler3

;==== 4A39F0 sub_4A39F0
4A39F0  push    ebp
4A39F1  mov     ebp, esp
4A39F3  push    0FFFFFFFFh
4A39F5  push    offset SEH_4A39F0
4A39FA  mov     eax, large fs:0
4A3A00  push    eax
4A3A01  sub     esp, 8
4A3A04  push    ebx
4A3A05  push    esi
4A3A06  push    edi
4A3A07  mov     eax, dword_4FB734
4A3A0C  xor     eax, ebp
4A3A0E  push    eax
4A3A0F  lea     eax, [ebp+var_C]
4A3A12  mov     large fs:0, eax
4A3A18  mov     [ebp+var_10], esp
4A3A1B  mov     ecx, offset dword_5089E8
4A3A20  call    sub_4A55F0
4A3A25  mov     [ebp+var_4], 0
4A3A2C  mov     eax, [ebp+arg_4]
4A3A2F  push    eax
4A3A30  mov     ecx, [ebp+arg_0]
4A3A33  push    ecx
4A3A34  call    sub_4A3400
4A3A39  add     esp, 8
4A3A3C  mov     esi, eax
4A3A3E  mov     [ebp+var_14], esi
4A3A41  mov     ecx, offset dword_5089E8
4A3A46  call    sub_4A5600
4A3A4B  mov     [ebp+var_4], 0FFFFFFFFh
4A3A52  mov     eax, esi
4A3A54  mov     ecx, [ebp+var_C]
4A3A57  mov     large fs:0, ecx
4A3A5E  pop     ecx
4A3A5F  pop     edi
4A3A60  pop     esi
4A3A61  pop     ebx
4A3A62  mov     esp, ebp
4A3A64  pop     ebp
4A3A65  retn
4A3A66  mov     ecx, offset dword_5089E8
4A3A6B  call    sub_4A5600
4A3A70  push    0; pThrowInfo
4A3A72  push    0; pExceptionObject
4A3A74  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D75E0  mov     edx, [esp-4+arg_4]
4D75E4  lea     eax, [edx+0Ch]
4D75E7  mov     ecx, [edx-18h]
4D75EA  xor     ecx, eax
4D75EC  call    sub_4ABDD5
4D75F1  mov     eax, offset stru_4F4108
4D75F6  jmp     ___CxxFrameHandler3

;==== 4A3A80 sub_4A3A80
4A3A80  push    ebp
4A3A81  mov     ebp, esp
4A3A83  push    0FFFFFFFFh
4A3A85  push    offset SEH_4A3A80
4A3A8A  mov     eax, large fs:0
4A3A90  push    eax
4A3A91  sub     esp, 8
4A3A94  push    ebx
4A3A95  push    esi
4A3A96  push    edi
4A3A97  mov     eax, dword_4FB734
4A3A9C  xor     eax, ebp
4A3A9E  push    eax
4A3A9F  lea     eax, [ebp+var_C]
4A3AA2  mov     large fs:0, eax
4A3AA8  mov     [ebp+var_10], esp
4A3AAB  mov     ecx, offset dword_5089E8
4A3AB0  call    sub_4A55F0
4A3AB5  mov     [ebp+var_4], 0
4A3ABC  mov     eax, [ebp+arg_0]
4A3ABF  push    eax
4A3AC0  call    sub_4A34D0
4A3AC5  add     esp, 4
4A3AC8  mov     esi, eax
4A3ACA  mov     [ebp+var_14], esi
4A3ACD  mov     ecx, offset dword_5089E8
4A3AD2  call    sub_4A5600
4A3AD7  mov     [ebp+var_4], 0FFFFFFFFh
4A3ADE  mov     eax, esi
4A3AE0  mov     ecx, [ebp+var_C]
4A3AE3  mov     large fs:0, ecx
4A3AEA  pop     ecx
4A3AEB  pop     edi
4A3AEC  pop     esi
4A3AED  pop     ebx
4A3AEE  mov     esp, ebp
4A3AF0  pop     ebp
4A3AF1  retn
4A3AF2  mov     ecx, offset dword_5089E8
4A3AF7  call    sub_4A5600
4A3AFC  push    0; pThrowInfo
4A3AFE  push    0; pExceptionObject
4A3B00  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7600  mov     edx, [esp-4+arg_4]
4D7604  lea     eax, [edx+0Ch]
4D7607  mov     ecx, [edx-18h]
4D760A  xor     ecx, eax
4D760C  call    sub_4ABDD5
4D7611  mov     eax, offset stru_4F4160
4D7616  jmp     ___CxxFrameHandler3

;==== 4A3B10 sub_4A3B10
4A3B10  push    ebp
4A3B11  mov     ebp, esp
4A3B13  push    0FFFFFFFFh
4A3B15  push    offset SEH_4A3B10
4A3B1A  mov     eax, large fs:0
4A3B20  push    eax
4A3B21  sub     esp, 8
4A3B24  push    ebx
4A3B25  push    esi
4A3B26  push    edi
4A3B27  mov     eax, dword_4FB734
4A3B2C  xor     eax, ebp
4A3B2E  push    eax
4A3B2F  lea     eax, [ebp+var_C]
4A3B32  mov     large fs:0, eax
4A3B38  mov     [ebp+var_10], esp
4A3B3B  mov     ecx, offset dword_5089E8
4A3B40  call    sub_4A55F0
4A3B45  mov     [ebp+var_4], 0
4A3B4C  mov     eax, [ebp+arg_4]
4A3B4F  push    eax
4A3B50  mov     ecx, [ebp+arg_0]
4A3B53  push    ecx
4A3B54  call    sub_4A3500
4A3B59  add     esp, 8
4A3B5C  mov     esi, eax
4A3B5E  mov     [ebp+var_14], esi
4A3B61  mov     ecx, offset dword_5089E8
4A3B66  call    sub_4A5600
4A3B6B  mov     [ebp+var_4], 0FFFFFFFFh
4A3B72  mov     eax, esi
4A3B74  mov     ecx, [ebp+var_C]
4A3B77  mov     large fs:0, ecx
4A3B7E  pop     ecx
4A3B7F  pop     edi
4A3B80  pop     esi
4A3B81  pop     ebx
4A3B82  mov     esp, ebp
4A3B84  pop     ebp
4A3B85  retn
4A3B86  mov     ecx, offset dword_5089E8
4A3B8B  call    sub_4A5600
4A3B90  push    0; pThrowInfo
4A3B92  push    0; pExceptionObject
4A3B94  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7620  mov     edx, [esp-4+arg_4]
4D7624  lea     eax, [edx+0Ch]
4D7627  mov     ecx, [edx-18h]
4D762A  xor     ecx, eax
4D762C  call    sub_4ABDD5
4D7631  mov     eax, offset stru_4F41B8
4D7636  jmp     ___CxxFrameHandler3

;==== 4A3BA0 sub_4A3BA0
4A3BA0  push    ebp
4A3BA1  mov     ebp, esp
4A3BA3  push    0FFFFFFFFh
4A3BA5  push    offset SEH_4A3BA0
4A3BAA  mov     eax, large fs:0
4A3BB0  push    eax
4A3BB1  sub     esp, 8
4A3BB4  push    ebx
4A3BB5  push    esi
4A3BB6  push    edi
4A3BB7  mov     eax, dword_4FB734
4A3BBC  xor     eax, ebp
4A3BBE  push    eax
4A3BBF  lea     eax, [ebp+var_C]
4A3BC2  mov     large fs:0, eax
4A3BC8  mov     [ebp+var_10], esp
4A3BCB  mov     ecx, offset dword_5089E8
4A3BD0  call    sub_4A55F0
4A3BD5  mov     [ebp+var_4], 0
4A3BDC  mov     eax, [ebp+arg_4]
4A3BDF  push    eax
4A3BE0  mov     ecx, [ebp+arg_0]
4A3BE3  push    ecx
4A3BE4  call    sub_4A3560
4A3BE9  add     esp, 8
4A3BEC  mov     esi, eax
4A3BEE  mov     [ebp+var_14], esi
4A3BF1  mov     ecx, offset dword_5089E8
4A3BF6  call    sub_4A5600
4A3BFB  mov     [ebp+var_4], 0FFFFFFFFh
4A3C02  mov     eax, esi
4A3C04  mov     ecx, [ebp+var_C]
4A3C07  mov     large fs:0, ecx
4A3C0E  pop     ecx
4A3C0F  pop     edi
4A3C10  pop     esi
4A3C11  pop     ebx
4A3C12  mov     esp, ebp
4A3C14  pop     ebp
4A3C15  retn
4A3C16  mov     ecx, offset dword_5089E8
4A3C1B  call    sub_4A5600
4A3C20  push    0; pThrowInfo
4A3C22  push    0; pExceptionObject
4A3C24  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7640  mov     edx, [esp-4+arg_4]
4D7644  lea     eax, [edx+0Ch]
4D7647  mov     ecx, [edx-18h]
4D764A  xor     ecx, eax
4D764C  call    sub_4ABDD5
4D7651  mov     eax, offset stru_4F4210
4D7656  jmp     ___CxxFrameHandler3

;==== 4A3C30 sub_4A3C30
4A3C30  push    ebp
4A3C31  mov     ebp, esp
4A3C33  push    0FFFFFFFFh
4A3C35  push    offset SEH_4A3C30
4A3C3A  mov     eax, large fs:0
4A3C40  push    eax
4A3C41  sub     esp, 8
4A3C44  push    ebx
4A3C45  push    esi
4A3C46  push    edi
4A3C47  mov     eax, dword_4FB734
4A3C4C  xor     eax, ebp
4A3C4E  push    eax
4A3C4F  lea     eax, [ebp+var_C]
4A3C52  mov     large fs:0, eax
4A3C58  mov     [ebp+var_10], esp
4A3C5B  mov     ecx, offset dword_5089E8
4A3C60  call    sub_4A55F0
4A3C65  mov     [ebp+var_4], 0
4A3C6C  mov     eax, [ebp+arg_4]
4A3C6F  push    eax
4A3C70  mov     ecx, [ebp+arg_0]
4A3C73  push    ecx
4A3C74  call    sub_4A35C0
4A3C79  add     esp, 8
4A3C7C  mov     esi, eax
4A3C7E  mov     [ebp+var_14], esi
4A3C81  mov     ecx, offset dword_5089E8
4A3C86  call    sub_4A5600
4A3C8B  mov     [ebp+var_4], 0FFFFFFFFh
4A3C92  mov     eax, esi
4A3C94  mov     ecx, [ebp+var_C]
4A3C97  mov     large fs:0, ecx
4A3C9E  pop     ecx
4A3C9F  pop     edi
4A3CA0  pop     esi
4A3CA1  pop     ebx
4A3CA2  mov     esp, ebp
4A3CA4  pop     ebp
4A3CA5  retn
4A3CA6  mov     ecx, offset dword_5089E8
4A3CAB  call    sub_4A5600
4A3CB0  push    0; pThrowInfo
4A3CB2  push    0; pExceptionObject
4A3CB4  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7660  mov     edx, [esp-4+arg_4]
4D7664  lea     eax, [edx+0Ch]
4D7667  mov     ecx, [edx-18h]
4D766A  xor     ecx, eax
4D766C  call    sub_4ABDD5
4D7671  mov     eax, offset stru_4F4268
4D7676  jmp     ___CxxFrameHandler3

;==== 4A3CC0 sub_4A3CC0
4A3CC0  push    ebp
4A3CC1  mov     ebp, esp
4A3CC3  push    0FFFFFFFFh
4A3CC5  push    offset SEH_4A3CC0
4A3CCA  mov     eax, large fs:0
4A3CD0  push    eax
4A3CD1  sub     esp, 8
4A3CD4  push    ebx
4A3CD5  push    esi
4A3CD6  push    edi
4A3CD7  mov     eax, dword_4FB734
4A3CDC  xor     eax, ebp
4A3CDE  push    eax
4A3CDF  lea     eax, [ebp+var_C]
4A3CE2  mov     large fs:0, eax
4A3CE8  mov     [ebp+var_10], esp
4A3CEB  mov     ecx, offset dword_5089E8
4A3CF0  call    sub_4A55F0
4A3CF5  mov     [ebp+var_4], 0
4A3CFC  mov     eax, [ebp+arg_4]
4A3CFF  push    eax
4A3D00  mov     ecx, [ebp+arg_0]
4A3D03  push    ecx
4A3D04  call    sub_4A3620
4A3D09  add     esp, 8
4A3D0C  mov     esi, eax
4A3D0E  mov     [ebp+var_14], esi
4A3D11  mov     ecx, offset dword_5089E8
4A3D16  call    sub_4A5600
4A3D1B  mov     [ebp+var_4], 0FFFFFFFFh
4A3D22  mov     eax, esi
4A3D24  mov     ecx, [ebp+var_C]
4A3D27  mov     large fs:0, ecx
4A3D2E  pop     ecx
4A3D2F  pop     edi
4A3D30  pop     esi
4A3D31  pop     ebx
4A3D32  mov     esp, ebp
4A3D34  pop     ebp
4A3D35  retn
4A3D36  mov     ecx, offset dword_5089E8
4A3D3B  call    sub_4A5600
4A3D40  push    0; pThrowInfo
4A3D42  push    0; pExceptionObject
4A3D44  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7680  mov     edx, [esp-4+arg_4]
4D7684  lea     eax, [edx+0Ch]
4D7687  mov     ecx, [edx-18h]
4D768A  xor     ecx, eax
4D768C  call    sub_4ABDD5
4D7691  mov     eax, offset stru_4F42C0
4D7696  jmp     ___CxxFrameHandler3

;==== 4A3D50 sub_4A3D50
4A3D50  push    ebp
4A3D51  mov     ebp, esp
4A3D53  push    0FFFFFFFFh
4A3D55  push    offset SEH_4A3D50
4A3D5A  mov     eax, large fs:0
4A3D60  push    eax
4A3D61  sub     esp, 8
4A3D64  push    ebx
4A3D65  push    esi
4A3D66  push    edi
4A3D67  mov     eax, dword_4FB734
4A3D6C  xor     eax, ebp
4A3D6E  push    eax
4A3D6F  lea     eax, [ebp+var_C]
4A3D72  mov     large fs:0, eax
4A3D78  mov     [ebp+var_10], esp
4A3D7B  mov     ecx, offset dword_5089E8
4A3D80  call    sub_4A55F0
4A3D85  mov     [ebp+var_4], 0
4A3D8C  mov     eax, [ebp+arg_8]
4A3D8F  push    eax
4A3D90  mov     ecx, [ebp+arg_4]
4A3D93  push    ecx
4A3D94  mov     edx, [ebp+arg_0]
4A3D97  push    edx
4A3D98  call    sub_4A3650
4A3D9D  add     esp, 0Ch
4A3DA0  mov     esi, eax
4A3DA2  mov     [ebp+var_14], esi
4A3DA5  mov     ecx, offset dword_5089E8
4A3DAA  call    sub_4A5600
4A3DAF  mov     [ebp+var_4], 0FFFFFFFFh
4A3DB6  mov     eax, esi
4A3DB8  mov     ecx, [ebp+var_C]
4A3DBB  mov     large fs:0, ecx
4A3DC2  pop     ecx
4A3DC3  pop     edi
4A3DC4  pop     esi
4A3DC5  pop     ebx
4A3DC6  mov     esp, ebp
4A3DC8  pop     ebp
4A3DC9  retn
4A3DCA  mov     ecx, offset dword_5089E8
4A3DCF  call    sub_4A5600
4A3DD4  push    0; pThrowInfo
4A3DD6  push    0; pExceptionObject
4A3DD8  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D76A0  mov     edx, [esp-4+arg_4]
4D76A4  lea     eax, [edx+0Ch]
4D76A7  mov     ecx, [edx-18h]
4D76AA  xor     ecx, eax
4D76AC  call    sub_4ABDD5
4D76B1  mov     eax, offset stru_4F4318
4D76B6  jmp     ___CxxFrameHandler3

;==== 4A3DE0 sub_4A3DE0
4A3DE0  push    ebp
4A3DE1  mov     ebp, esp
4A3DE3  push    0FFFFFFFFh
4A3DE5  push    offset SEH_4A3DE0
4A3DEA  mov     eax, large fs:0
4A3DF0  push    eax
4A3DF1  sub     esp, 8
4A3DF4  push    ebx
4A3DF5  push    esi
4A3DF6  push    edi
4A3DF7  mov     eax, dword_4FB734
4A3DFC  xor     eax, ebp
4A3DFE  push    eax
4A3DFF  lea     eax, [ebp+var_C]
4A3E02  mov     large fs:0, eax
4A3E08  mov     [ebp+var_10], esp
4A3E0B  mov     ecx, offset dword_5089E8
4A3E10  call    sub_4A55F0
4A3E15  mov     [ebp+var_4], 0
4A3E1C  mov     eax, [ebp+arg_4]
4A3E1F  push    eax
4A3E20  mov     ecx, [ebp+arg_0]
4A3E23  push    ecx
4A3E24  call    sub_4A36C0
4A3E29  add     esp, 8
4A3E2C  mov     esi, eax
4A3E2E  mov     [ebp+var_14], esi
4A3E31  mov     ecx, offset dword_5089E8
4A3E36  call    sub_4A5600
4A3E3B  mov     [ebp+var_4], 0FFFFFFFFh
4A3E42  mov     eax, esi
4A3E44  mov     ecx, [ebp+var_C]
4A3E47  mov     large fs:0, ecx
4A3E4E  pop     ecx
4A3E4F  pop     edi
4A3E50  pop     esi
4A3E51  pop     ebx
4A3E52  mov     esp, ebp
4A3E54  pop     ebp
4A3E55  retn
4A3E56  mov     ecx, offset dword_5089E8
4A3E5B  call    sub_4A5600
4A3E60  push    0; pThrowInfo
4A3E62  push    0; pExceptionObject
4A3E64  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D76C0  mov     edx, [esp-4+arg_4]
4D76C4  lea     eax, [edx+0Ch]
4D76C7  mov     ecx, [edx-18h]
4D76CA  xor     ecx, eax
4D76CC  call    sub_4ABDD5
4D76D1  mov     eax, offset stru_4F4370
4D76D6  jmp     ___CxxFrameHandler3

;==== 4A3E70 sub_4A3E70
4A3E70  push    ebp
4A3E71  mov     ebp, esp
4A3E73  push    0FFFFFFFFh
4A3E75  push    offset SEH_4A3E70
4A3E7A  mov     eax, large fs:0
4A3E80  push    eax
4A3E81  sub     esp, 8
4A3E84  push    ebx
4A3E85  push    esi
4A3E86  push    edi
4A3E87  mov     eax, dword_4FB734
4A3E8C  xor     eax, ebp
4A3E8E  push    eax
4A3E8F  lea     eax, [ebp+var_C]
4A3E92  mov     large fs:0, eax
4A3E98  mov     [ebp+var_10], esp
4A3E9B  mov     ecx, offset dword_5089E8
4A3EA0  call    sub_4A55F0
4A3EA5  mov     [ebp+var_4], 0
4A3EAC  mov     eax, [ebp+arg_4]
4A3EAF  push    eax
4A3EB0  mov     ecx, [ebp+arg_0]
4A3EB3  push    ecx
4A3EB4  call    sub_4A3720
4A3EB9  add     esp, 8
4A3EBC  mov     esi, eax
4A3EBE  mov     [ebp+var_14], esi
4A3EC1  mov     ecx, offset dword_5089E8
4A3EC6  call    sub_4A5600
4A3ECB  mov     [ebp+var_4], 0FFFFFFFFh
4A3ED2  mov     eax, esi
4A3ED4  mov     ecx, [ebp+var_C]
4A3ED7  mov     large fs:0, ecx
4A3EDE  pop     ecx
4A3EDF  pop     edi
4A3EE0  pop     esi
4A3EE1  pop     ebx
4A3EE2  mov     esp, ebp
4A3EE4  pop     ebp
4A3EE5  retn
4A3EE6  mov     ecx, offset dword_5089E8
4A3EEB  call    sub_4A5600
4A3EF0  push    0; pThrowInfo
4A3EF2  push    0; pExceptionObject
4A3EF4  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D76E0  mov     edx, [esp-4+arg_4]
4D76E4  lea     eax, [edx+0Ch]
4D76E7  mov     ecx, [edx-18h]
4D76EA  xor     ecx, eax
4D76EC  call    sub_4ABDD5
4D76F1  mov     eax, offset stru_4F43C8
4D76F6  jmp     ___CxxFrameHandler3

;==== 4A3F00 sub_4A3F00
4A3F00  push    ebp
4A3F01  mov     ebp, esp
4A3F03  push    0FFFFFFFFh
4A3F05  push    offset SEH_4A3F00
4A3F0A  mov     eax, large fs:0
4A3F10  push    eax
4A3F11  sub     esp, 8
4A3F14  push    ebx
4A3F15  push    esi
4A3F16  push    edi
4A3F17  mov     eax, dword_4FB734
4A3F1C  xor     eax, ebp
4A3F1E  push    eax
4A3F1F  lea     eax, [ebp+var_C]
4A3F22  mov     large fs:0, eax
4A3F28  mov     [ebp+var_10], esp
4A3F2B  mov     ecx, offset dword_5089E8
4A3F30  call    sub_4A55F0
4A3F35  mov     [ebp+var_4], 0
4A3F3C  mov     eax, [ebp+arg_4]
4A3F3F  push    eax
4A3F40  mov     ecx, [ebp+arg_0]
4A3F43  push    ecx
4A3F44  call    sub_4A3790
4A3F49  add     esp, 8
4A3F4C  mov     esi, eax
4A3F4E  mov     [ebp+var_14], esi
4A3F51  mov     ecx, offset dword_5089E8
4A3F56  call    sub_4A5600
4A3F5B  mov     [ebp+var_4], 0FFFFFFFFh
4A3F62  mov     eax, esi
4A3F64  mov     ecx, [ebp+var_C]
4A3F67  mov     large fs:0, ecx
4A3F6E  pop     ecx
4A3F6F  pop     edi
4A3F70  pop     esi
4A3F71  pop     ebx
4A3F72  mov     esp, ebp
4A3F74  pop     ebp
4A3F75  retn
4A3F76  mov     ecx, offset dword_5089E8
4A3F7B  call    sub_4A5600
4A3F80  push    0; pThrowInfo
4A3F82  push    0; pExceptionObject
4A3F84  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7700  mov     edx, [esp-4+arg_4]
4D7704  lea     eax, [edx+0Ch]
4D7707  mov     ecx, [edx-18h]
4D770A  xor     ecx, eax
4D770C  call    sub_4ABDD5
4D7711  mov     eax, offset stru_4F4420
4D7716  jmp     ___CxxFrameHandler3

;==== 4A4050 sub_4A4050
4A4050  push    ebp
4A4051  mov     ebp, esp
4A4053  push    ecx
4A4054  push    ebx
4A4055  push    edi
4A4056  mov     edi, [ebp+arg_0]
4A4059  push    edi
4A405A  call    sub_4A2CB0
4A405F  mov     ebx, eax
4A4061  add     esp, 4
4A4064  test    ebx, ebx
4A4066  jnz     short loc_4A40D2
4A4068  mov     eax, [ebp+arg_8]
4A406B  push    esi
4A406C  push    eax
4A406D  push    edi
4A406E  call    sub_4A3470
4A4073  mov     ecx, [ebp+arg_4]
4A4076  mov     esi, edi
4A4078  shl     esi, 4
4A407B  add     esi, edi
4A407D  add     esi, esi
4A407F  add     esi, esi
4A4081  mov     dword_508A30[esi], ecx
4A4087  mov     ecx, dword_508A0C[esi]
4A408D  mov     dword_508A34[esi], ebx
4A4093  mov     dword_508A48[esi], 80h
4A409D  mov     edx, [ecx]
4A409F  mov     eax, [edx+10h]
4A40A2  add     esp, 8
4A40A5  call    eax
4A40A7  mov     edi, dword_508A0C[esi]
4A40AD  lea     ecx, dword_508A08[esi]
4A40B3  push    ecx
4A40B4  call    sub_4A2D30
4A40B9  add     esp, 4
4A40BC  push    eax
4A40BD  mov     eax, [edi]
4A40BF  mov     edx, [eax+8]
4A40C2  mov     ecx, edi
4A40C4  call    edx
4A40C6  pop     esi
4A40C7  test    eax, eax
4A40C9  jz      short loc_4A40D0
4A40CB  mov     ebx, 17h
4A40D0  mov     eax, ebx
4A40D2  pop     edi
4A40D3  pop     ebx
4A40D4  mov     esp, ebp
4A40D6  pop     ebp
4A40D7  retn

;==== 4A4920 sub_4A4920
4A4920  push    ebp
4A4921  mov     ebp, esp
4A4923  push    0FFFFFFFFh
4A4925  push    offset SEH_4A4920
4A492A  mov     eax, large fs:0
4A4930  push    eax
4A4931  sub     esp, 8
4A4934  push    ebx
4A4935  push    esi
4A4936  push    edi
4A4937  mov     eax, dword_4FB734
4A493C  xor     eax, ebp
4A493E  push    eax
4A493F  lea     eax, [ebp+var_C]
4A4942  mov     large fs:0, eax
4A4948  mov     [ebp+var_10], esp
4A494B  mov     ecx, offset dword_5089E8
4A4950  call    sub_4A55F0
4A4955  mov     [ebp+var_4], 0
4A495C  mov     eax, [ebp+arg_8]
4A495F  push    eax
4A4960  mov     ecx, [ebp+arg_4]
4A4963  push    ecx
4A4964  mov     edx, [ebp+arg_0]
4A4967  push    edx
4A4968  call    sub_4A4050
4A496D  add     esp, 0Ch
4A4970  mov     esi, eax
4A4972  mov     [ebp+var_14], esi
4A4975  mov     ecx, offset dword_5089E8
4A497A  call    sub_4A5600
4A497F  mov     [ebp+var_4], 0FFFFFFFFh
4A4986  mov     eax, esi
4A4988  mov     ecx, [ebp+var_C]
4A498B  mov     large fs:0, ecx
4A4992  pop     ecx
4A4993  pop     edi
4A4994  pop     esi
4A4995  pop     ebx
4A4996  mov     esp, ebp
4A4998  pop     ebp
4A4999  retn
4A499A  mov     ecx, offset dword_5089E8
4A499F  call    sub_4A5600
4A49A4  push    0; pThrowInfo
4A49A6  push    0; pExceptionObject
4A49A8  call    __CxxThrowException@8; _CxxThrowException(x,x)
4D7850  mov     edx, [esp-4+arg_4]
4D7854  lea     eax, [edx+0Ch]
4D7857  mov     ecx, [edx-18h]
4D785A  xor     ecx, eax
4D785C  call    sub_4ABDD5
4D7861  mov     eax, offset stru_4F4644
4D7866  jmp     ___CxxFrameHandler3

;==== 4A55F0 sub_4A55F0
4A55F0  add     ecx, 4
4A55F3  push    ecx; lpCriticalSection
4A55F4  call    ds:EnterCriticalSection
4A55FA  xor     eax, eax
4A55FC  retn

;==== 4A5600 sub_4A5600
4A5600  add     ecx, 4
4A5603  push    ecx; lpCriticalSection
4A5604  call    ds:LeaveCriticalSection
4A560A  xor     eax, eax
4A560C  retn

;==== 4ABDD5 sub_4ABDD5
4ABDD5  cmp     ecx, dword_4FB734
4ABDDB  jnz     short loc_4ABDDF
4ABDDD  rep retn
4ABDDF  jmp     ___report_gsfailure

