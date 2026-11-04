;==== 489D90
489D90  push    ebp
489D91  mov     ebp, esp
489D93  mov     edx, [ebp+arg_0]
489D96  mov     ecx, edx
489D98  call    sub_445480
489D9D  neg     eax
489D9F  sbb     eax, eax
489DA1  and     eax, edx
489DA3  call    sub_48DC60
489DA8  xor     eax, eax
489DAA  pop     ebp
489DAB  retn
;==== 48D860
48D860  push    ebp
48D861  mov     ebp, esp
48D863  push    0FFFFFFFFh
48D865  push    offset SEH_48D860
48D86A  mov     eax, large fs:0
48D870  push    eax
48D871  sub     esp, 12Ch
48D877  mov     eax, dword_4FB734
48D87C  xor     eax, ebp
48D87E  mov     [ebp+var_14], eax
48D881  push    ebx
48D882  push    esi
48D883  push    edi
48D884  push    eax
48D885  lea     eax, [ebp+var_C]
48D888  mov     large fs:0, eax
48D88E  mov     [ebp+var_10], esp
48D891  mov     eax, [ebp+arg_0]
48D894  mov     [ebp+var_120], eax
48D89A  xor     ebx, ebx
48D89C  mov     [ebp+var_118], ebx
48D8A2  mov     [ebp+var_130], ebx
48D8A8  call    sub_4468E0
48D8AD  mov     esi, eax
48D8AF  mov     [ebp+var_11C], esi
48D8B5  mov     [ebp+var_4], ebx
48D8B8  cmp     esi, ebx
48D8BA  jz      loc_48DA2B
48D8C0  mov     edi, 1
48D8C5  mov     [ebp+var_124], edi
48D8CB  mov     [ebp+var_12C], ebx
48D8D1  cmp     dword_5668AC, ebx
48D8D7  jz      short loc_48D8E5
48D8D9  cmp     esi, dword_5668B0
48D8DF  jnz     loc_48D9FE
48D8E5  mov     [ebp+var_128], edi
48D8EB  mov     eax, esi
48D8ED  call    sub_445370
48D8F2  test    eax, eax
48D8F4  jns     short loc_48D912
48D8F6  mov     eax, esi
48D8F8  call    sub_4458D0
48D8FD  test    eax, eax
48D8FF  jnz     short loc_48D907
48D901  mov     [ebp+var_12C], esi
48D907  mov     [ebp+var_128], ebx
48D90D  jmp     loc_48D9FE
48D912  test    al, 1
48D914  jz      short loc_48D935
48D916  call    sub_4455C0
48D91B  cmp     eax, ebx
48D91D  jz      short loc_48D92A
48D91F  cmp     eax, 0FFFFFFFFh
48D922  jnz     short loc_48D935
48D924  mov     [ebp+var_118], edi
48D92A  mov     [ebp+var_128], ebx
48D930  jmp     loc_48D9FE
48D935  cmp     [ebp+var_118], ebx
48D93B  jnz     loc_48D9FE
48D941  xor     eax, eax
48D943  xor     edi, edi
48D945  mov     [ebp+var_134], edi
48D94B  cmp     eax, ebx
48D94D  jnz     short loc_48D996
48D94F  cmp     edi, 100000h
48D955  jnb     short loc_48D99B
48D957  mov     eax, esi
48D959  call    sub_4453E0
48D95E  movzx   ecx, al
48D961  mov     eax, funcs_48D98E[ecx*4]
48D968  cmp     eax, ebx
48D96A  jnz     short loc_48D98D
48D96C  push    ecx
48D96D  push    offset byte_4EBF60; Format
48D972  lea     eax, [ebp+Buffer]
48D978  push    eax; Buffer
48D979  call    _sprintf
48D97E  add     esp, 0Ch
48D981  push    esi
48D982  lea     edi, [ebp+Buffer]
48D988  call    sub_464C50
48D98D  push    esi
48D98E  call    eax ; sub_473AF0
48D990  add     esp, 4
48D993  inc     edi
48D994  jmp     short loc_48D945
48D996  cmp     eax, 6; switch 7 cases
48D999  ja      short def_48D99B; jumptable 0048D99B default case, cases 0,1
48D99B  jmp     ds:jpt_48D99B[eax*4]; switch jump
48D9A2  mov     [ebp+var_124], ebx; jumptable 0048D99B case 2
48D9A8  jmp     short loc_48DA0D
48D9AA  mov     ecx, dword_5668A8; jumptable 0048D99B case 3
48D9B0  push    ecx
48D9B1  mov     ecx, [ebp+var_120]
48D9B7  call    sub_444F60
48D9BC  mov     esi, eax
48D9BE  mov     [ebp+var_11C], esi
48D9C4  mov     [ebp+var_124], ebx
48D9CA  jmp     short loc_48DA0D
48D9CC  mov     ecx, 80000000h; jumptable 0048D99B case 4
48D9D1  mov     eax, esi
48D9D3  call    sub_445390
48D9D8  mov     [ebp+var_124], ebx
48D9DE  jmp     short loc_48DA0D
48D9E0  mov     [ebp+var_130], 1; jumptable 0048D99B case 5
48D9EA  jmp     short def_48D99B; jumptable 0048D99B default case, cases 0,1
48D9EC  mov     [ebp+var_118], 1; jumptable 0048D99B case 6
48D9F6  cmp     [ebp+var_124], ebx; jumptable 0048D99B default case, cases 0,1
48D9FC  jz      short loc_48DA0D
48D9FE  mov     eax, esi
48DA00  call    sub_4468E0
48DA05  mov     esi, eax
48DA07  mov     [ebp+var_11C], esi
48DA0D  mov     ecx, [ebp+var_12C]
48DA13  cmp     ecx, ebx
48DA15  jz      loc_48D8B8
48DA1B  mov     edx, [ebp+var_120]
48DA21  call    sub_444EF0
48DA26  jmp     loc_48D8B8
48DA2B  mov     [ebp+var_4], 0FFFFFFFFh
48DA32  jmp     short loc_48DA83
48DA34  call    sub_4649B0
48DA39  test    eax, eax
48DA3B  jz      short loc_48DA6A
48DA3D  cmp     [ebp+var_138], 7FFFFFFFh
48DA47  jnz     short loc_48DA6A
48DA49  xor     ebx, ebx
48DA4B  push    ebx
48DA4C  push    ebx
48DA4D  push    0FFFFFFFFh
48DA4F  call    sub_497060
48DA54  add     esp, 0Ch
48DA57  mov     ecx, [ebp+var_11C]
48DA5D  mov     edx, [ebp+var_120]
48DA63  call    sub_444EF0
48DA68  jmp     short loc_48DA74
48DA6A  mov     [ebp+var_118], 1
48DA74  mov     [ebp+var_4], 0FFFFFFFFh
48DA7B  mov     eax, offset loc_48DA81
48DA80  retn
48DA81  xor     ebx, ebx
48DA83  cmp     [ebp+var_118], ebx
48DA89  jz      loc_48DB1D
48DA8F  mov     eax, 1
48DA94  jmp     loc_48DB2A
48DA99  push    10000h; unsigned int
48DA9E  call    ??_U@YAPAXI@Z; operator new[](uint)
48DAA3  mov     esi, eax
48DAA5  push    offset dword_4EBF88; int
48DAAA  push    esi; Buffer
48DAAB  mov     edi, [ebp+var_11C]
48DAB1  mov     eax, edi
48DAB3  call    sub_464AD0
48DAB8  push    1040h
48DABD  push    esi
48DABE  mov     edx, hWndParent
48DAC4  push    edx
48DAC5  mov     eax, offset loc_4EBFA4
48DACA  call    sub_46C1E0
48DACF  add     esp, 18h
48DAD2  call    sub_4649B0
48DAD7  test    eax, eax
48DAD9  jz      short loc_48DAFD
48DADB  call    sub_464A20
48DAE0  xor     ebx, ebx
48DAE2  push    ebx
48DAE3  push    ebx
48DAE4  push    0FFFFFFFFh
48DAE6  call    sub_497060
48DAEB  add     esp, 0Ch
48DAEE  mov     ecx, edi
48DAF0  mov     edx, [ebp+var_120]
48DAF6  call    sub_444EF0
48DAFB  jmp     short loc_48DB07
48DAFD  mov     [ebp+var_118], 1
48DB07  push    esi; void *
48DB08  call    ??_V@YAXPAX@Z; operator delete[](void *)
48DB0D  add     esp, 4
48DB10  mov     [ebp+var_4], 0FFFFFFFFh
48DB17  mov     eax, offset loc_48DA81
48DB1C  retn
48DB1D  mov     eax, [ebp+var_130]
48DB23  neg     eax
48DB25  sbb     eax, eax
48DB27  and     eax, 2
48DB2A  mov     ecx, [ebp+var_C]
48DB2D  mov     large fs:0, ecx
48DB34  pop     ecx
48DB35  pop     edi
48DB36  pop     esi
48DB37  pop     ebx
48DB38  mov     ecx, [ebp+var_14]
48DB3B  xor     ecx, ebp
48DB3D  call    sub_4ABDD5
48DB42  mov     esp, ebp
48DB44  pop     ebp
48DB45  retn
4DA2B0  mov     edx, [esp-4+arg_4]
4DA2B4  lea     eax, [edx+0Ch]
4DA2B7  mov     ecx, [edx-13Ch]
4DA2BD  xor     ecx, eax
4DA2BF  call    sub_4ABDD5
4DA2C4  mov     ecx, [edx-8]
4DA2C7  xor     ecx, eax
4DA2C9  call    sub_4ABDD5
4DA2CE  mov     eax, offset stru_4F7BD8
4DA2D3  jmp     ___CxxFrameHandler3
4EBFA4  add     dword ptr [edi-7Dh], 0FFFFFF89h
4EBFA8  sbb     dword ptr [ebx+0], 0
;==== 48DC60
48DC60  xor     ecx, ecx
48DC62  test    eax, eax
48DC64  setnz   cl
48DC67  mov     dword_5668B0, eax
48DC6C  mov     dword_5668AC, ecx
48DC72  retn
