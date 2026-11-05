; Original Houkago Cinderella FD executable, image base 0x400000.
; SHA256: f9206815f5239837c8c1089026bc8d730bbaafff66fa31942cadd1f083360782
; llvm-objdump -d --x86-asm-syntax=intel; selected function ranges, unmodified.
; A0 table VA 0x502018, A0:09 -> 0x494900.
  494900: 55                           	push	ebp
  494901: 8b ec                        	mov	ebp, esp
  494903: 53                           	push	ebx
  494904: 56                           	push	esi
  494905: 57                           	push	edi
  494906: 8b 7d 08                     	mov	edi, dword ptr [ebp + 0x8]
  494909: 8b cf                        	mov	ecx, edi
  49490b: e8 90 85 fb ff               	call	0x44cea0 <.text+0x4bea0>
  494910: 8b d8                        	mov	ebx, eax
  494912: e8 89 85 fb ff               	call	0x44cea0 <.text+0x4bea0>
  494917: 8b d7                        	mov	edx, edi
  494919: 8b cb                        	mov	ecx, ebx
  49491b: 8b f0                        	mov	esi, eax
  49491d: e8 be 23 01 00               	call	0x4a6ce0 <.text+0xa5ce0>
  494922: 8b d7                        	mov	edx, edi
  494924: 8b ce                        	mov	ecx, esi
  494926: e8 05 23 01 00               	call	0x4a6c30 <.text+0xa5c30>
  49492b: 8b d3                        	mov	edx, ebx
  49492d: 8b ce                        	mov	ecx, esi
  49492f: e8 bc c8 00 00               	call	0x4a11f0 <.text+0xa01f0>
  494934: 5f                           	pop	edi
  494935: 5e                           	pop	esi
  494936: 33 c0                        	xor	eax, eax
  494938: 5b                           	pop	ebx
  494939: 5d                           	pop	ebp
  49493a: c3                           	ret

  4a6c30: 55                           	push	ebp
  4a6c31: 8b ec                        	mov	ebp, esp
  4a6c33: 81 ec 04 01 00 00            	sub	esp, 0x104
  4a6c39: a1 f8 b2 52 00               	mov	eax, dword ptr [0x52b2f8]
  4a6c3e: 33 c5                        	xor	eax, ebp
  4a6c40: 89 45 fc                     	mov	dword ptr [ebp - 0x4], eax
  4a6c43: 56                           	push	esi
  4a6c44: 8b f2                        	mov	esi, edx
  4a6c46: 81 f9 80 00 00 00            	cmp	ecx, 0x80
  4a6c4c: 73 0f                        	jae	0x4a6c5d <.text+0xa5c5d>
  4a6c4e: 8b 4d fc                     	mov	ecx, dword ptr [ebp - 0x4]
  4a6c51: 33 cd                        	xor	ecx, ebp
  4a6c53: 5e                           	pop	esi
  4a6c54: e8 1e 6f 02 00               	call	0x4cdb77 <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x93f7>
  4a6c59: 8b e5                        	mov	esp, ebp
  4a6c5b: 5d                           	pop	ebp
  4a6c5c: c3                           	ret
  4a6c5d: 51                           	push	ecx
  4a6c5e: 8d 85 fc fe ff ff            	lea	eax, [ebp - 0x104]
  4a6c64: 68 84 9d 51 00               	push	0x519d84
  4a6c69: 50                           	push	eax
  4a6c6a: e8 a1 b3 f5 ff               	call	0x402010 <.text+0x1010>
  4a6c6f: 83 c4 0c                     	add	esp, 0xc
  4a6c72: 8d 8d fc fe ff ff            	lea	ecx, [ebp - 0x104]
  4a6c78: 8b d6                        	mov	edx, esi
  4a6c7a: e8 f1 a6 fc ff               	call	0x471370 <.text+0x70370>
  4a6c7f: cc                           	int3

  4a11f0: 81 f9 80 00 00 00            	cmp	ecx, 0x80
  4a11f6: 73 28                        	jae	0x4a1220 <.text+0xa0220>
  4a11f8: 81 fa 80 00 00 00            	cmp	edx, 0x80
  4a11fe: 77 20                        	ja	0x4a1220 <.text+0xa0220>
  4a1200: 83 3d b0 c5 58 00 00         	cmp	dword ptr [0x58c5b0], 0x0
  4a1207: 89 14 8d 70 c3 58 00         	mov	dword ptr [4*ecx + 0x58c370], edx
  4a120e: 75 0a                        	jne	0x4a121a <.text+0xa021a>
  4a1210: 52                           	push	edx
  4a1211: 51                           	push	ecx
  4a1212: e8 f9 58 02 00               	call	0x4c6b10 <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x2390>
  4a1217: 83 c4 08                     	add	esp, 0x8
  4a121a: b8 01 00 00 00               	mov	eax, 0x1
  4a121f: c3                           	ret
  4a1220: 33 c0                        	xor	eax, eax
  4a1222: c3                           	ret

  4a6d40: 55                           	push	ebp
  4a6d41: 8b ec                        	mov	ebp, esp
  4a6d43: 81 ec 04 01 00 00            	sub	esp, 0x104
  4a6d49: a1 f8 b2 52 00               	mov	eax, dword ptr [0x52b2f8]
  4a6d4e: 33 c5                        	xor	eax, ebp
  4a6d50: 89 45 fc                     	mov	dword ptr [ebp - 0x4], eax
  4a6d53: 56                           	push	esi
  4a6d54: 8b f2                        	mov	esi, edx
  4a6d56: 83 f9 10                     	cmp	ecx, 0x10
  4a6d59: 73 0f                        	jae	0x4a6d6a <.text+0xa5d6a>
  4a6d5b: 8b 4d fc                     	mov	ecx, dword ptr [ebp - 0x4]
  4a6d5e: 33 cd                        	xor	ecx, ebp
  4a6d60: 5e                           	pop	esi
  4a6d61: e8 11 6e 02 00               	call	0x4cdb77 <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x93f7>
  4a6d66: 8b e5                        	mov	esp, ebp
  4a6d68: 5d                           	pop	ebp
  4a6d69: c3                           	ret
  4a6d6a: 51                           	push	ecx
  4a6d6b: 8d 85 fc fe ff ff            	lea	eax, [ebp - 0x104]
  4a6d71: 68 38 9c 51 00               	push	0x519c38
  4a6d76: 50                           	push	eax
  4a6d77: e8 94 b2 f5 ff               	call	0x402010 <.text+0x1010>
  4a6d7c: 83 c4 0c                     	add	esp, 0xc
  4a6d7f: 8d 8d fc fe ff ff            	lea	ecx, [ebp - 0x104]
  4a6d85: 8b d6                        	mov	edx, esi
  4a6d87: e8 e4 a5 fc ff               	call	0x471370 <.text+0x70370>
  4a6d8c: cc                           	int3
  4a6d8d: cc                           	int3
  4a6d8e: cc                           	int3
  4a6d8f: cc                           	int3
