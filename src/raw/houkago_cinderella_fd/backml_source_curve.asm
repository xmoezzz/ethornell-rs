; Original EXE identified by sound_channels.asm. llvm-objdump, unmodified ranges.
; BackML vtable at 0x511A84; +0x5C -> 0x423C60.
; Switch index table 0x423D50, base property 0x41; target table 0x423D34.
; 0x423D34 dwords: 0x423c82, 0x423cb1, 0x423cca, 0x423ce0, 0x423cf9, 0x423d0f, 0x423d28
; 0x423D50 bytes: 00 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 06 01 02 03 06 06 06 06 06 06 06 06 06 06 06 04 05
  423a80: 55                           	push	ebp
  423a81: 8b ec                        	mov	ebp, esp
  423a83: 8b 45 08                     	mov	eax, dword ptr [ebp + 0x8]
  423a86: 83 f8 08                     	cmp	eax, 0x8
  423a89: 73 13                        	jae	0x423a9e <.text+0x22a9e>
  423a8b: 6b d0 58                     	imul	edx, eax, 0x58
  423a8e: 8b 45 0c                     	mov	eax, dword ptr [ebp + 0xc]
  423a91: 89 84 0a 84 01 00 00         	mov	dword ptr [edx + ecx + 0x184], eax
  423a98: 33 c0                        	xor	eax, eax
  423a9a: 5d                           	pop	ebp
  423a9b: c2 08 00                     	ret	0x8
  423a9e: b8 01 00 00 80               	mov	eax, 0x80000001
  423aa3: 5d                           	pop	ebp
  423aa4: c2 08 00                     	ret	0x8

  423c60: 55                           	push	ebp
  423c61: 8b ec                        	mov	ebp, esp
  423c63: 8b d1                        	mov	edx, ecx
  423c65: 8b 4d 08                     	mov	ecx, dword ptr [ebp + 0x8]
  423c68: 8d 41 bf                     	lea	eax, [ecx - 0x41]
  423c6b: 83 f8 4e                     	cmp	eax, 0x4e
  423c6e: 0f 87 b4 00 00 00            	ja	0x423d28 <.text+0x22d28>
  423c74: 0f b6 80 50 3d 42 00         	movzx	eax, byte ptr [eax + 0x423d50]
  423c7b: ff 24 85 34 3d 42 00         	jmp	dword ptr [4*eax + 0x423d34]
  423c82: 8b 8a 4c 01 00 00            	mov	ecx, dword ptr [edx + 0x14c]
  423c88: 6b c1 58                     	imul	eax, ecx, 0x58
  423c8b: ff b4 10 a4 01 00 00         	push	dword ptr [eax + edx + 0x1a4]
  423c92: ff b4 10 80 01 00 00         	push	dword ptr [eax + edx + 0x180]
  423c99: ff b4 10 7c 01 00 00         	push	dword ptr [eax + edx + 0x17c]
  423ca0: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423ca3: 51                           	push	ecx
  423ca4: 8b ca                        	mov	ecx, edx
  423ca6: e8 55 fd ff ff               	call	0x423a00 <.text+0x22a00>
  423cab: 33 c0                        	xor	eax, eax
  423cad: 5d                           	pop	ebp
  423cae: c2 0c 00                     	ret	0xc
  423cb1: ff 75 10                     	push	dword ptr [ebp + 0x10]
  423cb4: 8b ca                        	mov	ecx, edx
  423cb6: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423cb9: ff b2 4c 01 00 00            	push	dword ptr [edx + 0x14c]
  423cbf: e8 2c fe ff ff               	call	0x423af0 <.text+0x22af0>
  423cc4: 33 c0                        	xor	eax, eax
  423cc6: 5d                           	pop	ebp
  423cc7: c2 0c 00                     	ret	0xc
  423cca: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423ccd: 8b ca                        	mov	ecx, edx
  423ccf: ff b2 4c 01 00 00            	push	dword ptr [edx + 0x14c]
  423cd5: e8 56 fe ff ff               	call	0x423b30 <.text+0x22b30>
  423cda: 33 c0                        	xor	eax, eax
  423cdc: 5d                           	pop	ebp
  423cdd: c2 0c 00                     	ret	0xc
  423ce0: ff 75 10                     	push	dword ptr [ebp + 0x10]
  423ce3: 8b ca                        	mov	ecx, edx
  423ce5: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423ce8: ff b2 4c 01 00 00            	push	dword ptr [edx + 0x14c]
  423cee: e8 6d fe ff ff               	call	0x423b60 <.text+0x22b60>
  423cf3: 33 c0                        	xor	eax, eax
  423cf5: 5d                           	pop	ebp
  423cf6: c2 0c 00                     	ret	0xc
  423cf9: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423cfc: 8b ca                        	mov	ecx, edx
  423cfe: ff b2 4c 01 00 00            	push	dword ptr [edx + 0x14c]
  423d04: e8 77 fd ff ff               	call	0x423a80 <.text+0x22a80>
  423d09: 33 c0                        	xor	eax, eax
  423d0b: 5d                           	pop	ebp
  423d0c: c2 0c 00                     	ret	0xc
  423d0f: ff 75 10                     	push	dword ptr [ebp + 0x10]
  423d12: 8b ca                        	mov	ecx, edx
  423d14: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  423d17: ff b2 4c 01 00 00            	push	dword ptr [edx + 0x14c]
  423d1d: e8 8e fd ff ff               	call	0x423ab0 <.text+0x22ab0>
  423d22: 33 c0                        	xor	eax, eax
  423d24: 5d                           	pop	ebp
  423d25: c2 0c 00                     	ret	0xc
  423d28: 89 4d 08                     	mov	dword ptr [ebp + 0x8], ecx
  423d2b: 8b ca                        	mov	ecx, edx
  423d2d: 5d                           	pop	ebp
  423d2e: e9 7d d6 ff ff               	jmp	0x4213b0 <.text+0x203b0>

  423eb1: 8b 94 24 84 00 00 00         	mov	edx, dword ptr [esp + 0x84]
  423eb8: e8 53 c1 ff ff               	call	0x420010 <.text+0x1f010>
  423ebd: 99                           	cdq
  423ebe: 8b f8                        	mov	edi, eax
  423ec0: 8b f2                        	mov	esi, edx
  423ec2: 8b 84 24 90 00 00 00         	mov	eax, dword ptr [esp + 0x90]
  423ec9: 56                           	push	esi
  423eca: 99                           	cdq
  423ecb: 57                           	push	edi
  423ecc: 52                           	push	edx
  423ecd: 50                           	push	eax
  423ece: e8 ad 8a 0c 00               	call	0x4ec980 <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x28200>
  423ed3: 0f ac d0 10                  	shrd	eax, edx, 0x10
  423ed7: 56                           	push	esi
  423ed8: 03 44 24 74                  	add	eax, dword ptr [esp + 0x74]
  423edc: 89 44 24 10                  	mov	dword ptr [esp + 0x10], eax
  423ee0: 8b 84 24 98 00 00 00         	mov	eax, dword ptr [esp + 0x98]
  423ee7: 99                           	cdq
  423ee8: 57                           	push	edi
  423ee9: 52                           	push	edx
  423eea: 50                           	push	eax
  423eeb: e8 90 8a 0c 00               	call	0x4ec980 <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x28200>
  423ef0: 8b 4c 24 14                  	mov	ecx, dword ptr [esp + 0x14]
  423ef4: 0f ac d0 10                  	shrd	eax, edx, 0x10
  423ef8: 8b 94 24 88 00 00 00         	mov	edx, dword ptr [esp + 0x88]
  423eff: 03 44 24 74                  	add	eax, dword ptr [esp + 0x74]
  423f03: 89 44 24 10                  	mov	dword ptr [esp + 0x10], eax
  423f07: e8 04 c1 ff ff               	call	0x420010 <.text+0x1f010>
  423f0c: 8b c8                        	mov	ecx, eax
  423f0e: 8b 84 24 98 00 00 00         	mov	eax, dword ptr [esp + 0x98]
  423f15: f7 e9                        	imul	ecx
  423f17: 8b 4c 24 14                  	mov	ecx, dword ptr [esp + 0x14]
  423f1b: 0f ac d0 10                  	shrd	eax, edx, 0x10
  423f1f: 8b 94 24 8c 00 00 00         	mov	edx, dword ptr [esp + 0x8c]
  423f26: 03 44 24 78                  	add	eax, dword ptr [esp + 0x78]
  423f2a: 89 44 24 08                  	mov	dword ptr [esp + 0x8], eax
  423f2e: e8 dd c0 ff ff               	call	0x420010 <.text+0x1f010>
  423f33: 8b 4c 24 7c                  	mov	ecx, dword ptr [esp + 0x7c]

  420463: 8b c1                        	mov	eax, ecx
  420465: 99                           	cdq
  420466: 81 e2 ff 00 00 00            	and	edx, 0xff
  42046c: 03 c2                        	add	eax, edx
  42046e: c1 f8 08                     	sar	eax, 0x8
  420471: 8b e5                        	mov	esp, ebp
  420473: 5d                           	pop	ebp
  420474: c3                           	ret
