; Original EXE identified in sound_channels.asm. llvm-objdump, unmodified ranges.
  49a59c: 8d 95 e0 fc ff ff            	lea	edx, [ebp - 0x320]
  49a5a2: 8d 8d d4 f9 ff ff            	lea	ecx, [ebp - 0x62c]
  49a5a8: e8 93 79 fd ff               	call	0x471f40 <.text+0x70f40>
  49a5ad: 53                           	push	ebx
  49a5ae: 68 00 00 40 00               	push	0x400000
  49a5b3: 68 00 00 80 00               	push	0x800000
  49a5b8: 68 00 10 00 00               	push	0x1000
  49a5bd: 8d 95 e0 fc ff ff            	lea	edx, [ebp - 0x320]
  49a5c3: 8d 8d d4 f9 ff ff            	lea	ecx, [ebp - 0x62c]
  49a5c9: e8 92 06 00 00               	call	0x49ac60 <.text+0x99c60>
  49a5ce: 83 c4 10                     	add	esp, 0x10

  49ac60: 55                           	push	ebp
  49ac61: 8b ec                        	mov	ebp, esp
  49ac63: 81 ec 18 01 00 00            	sub	esp, 0x118
  49ac69: a1 f8 b2 52 00               	mov	eax, dword ptr [0x52b2f8]
  49ac6e: 33 c5                        	xor	eax, ebp
  49ac70: 89 45 fc                     	mov	dword ptr [ebp - 0x4], eax
  49ac73: 8b c1                        	mov	eax, ecx
  49ac75: 8b 4d 08                     	mov	ecx, dword ptr [ebp + 0x8]
  49ac78: 53                           	push	ebx
  49ac79: 56                           	push	esi
  49ac7a: 8b 75 14                     	mov	esi, dword ptr [ebp + 0x14]
  49ac7d: 33 db                        	xor	ebx, ebx
  49ac7f: 89 8d e8 fe ff ff            	mov	dword ptr [ebp - 0x118], ecx
  49ac85: 8b 4d 0c                     	mov	ecx, dword ptr [ebp + 0xc]
  49ac88: 57                           	push	edi
  49ac89: 89 8d ec fe ff ff            	mov	dword ptr [ebp - 0x114], ecx
  49ac8f: 8b fa                        	mov	edi, edx
  49ac91: 8b 4d 10                     	mov	ecx, dword ptr [ebp + 0x10]
  49ac94: 8b d0                        	mov	edx, eax
  49ac96: 89 8d f0 fe ff ff            	mov	dword ptr [ebp - 0x110], ecx
  49ac9c: 8d 8d f8 fe ff ff            	lea	ecx, [ebp - 0x108]
  49aca2: 53                           	push	ebx
  49aca3: 57                           	push	edi
  49aca4: 89 85 f4 fe ff ff            	mov	dword ptr [ebp - 0x10c], eax
  49acaa: 89 9d f8 fe ff ff            	mov	dword ptr [ebp - 0x108], ebx
  49acb0: e8 6b 7d fd ff               	call	0x472a20 <.text+0x71a20>
  49acb5: 83 c4 08                     	add	esp, 0x8
  49acb8: 85 c0                        	test	eax, eax
  49acba: 74 3b                        	je	0x49acf7 <.text+0x99cf7>
  49acbc: ff b5 f0 fe ff ff            	push	dword ptr [ebp - 0x110]
  49acc2: 8b ce                        	mov	ecx, esi
  49acc4: ff b5 ec fe ff ff            	push	dword ptr [ebp - 0x114]
  49acca: ff b5 e8 fe ff ff            	push	dword ptr [ebp - 0x118]
  49acd0: e8 5b 1b fb ff               	call	0x44c830 <.text+0x4b830>
  49acd5: 57                           	push	edi
  49acd6: ff b5 f8 fe ff ff            	push	dword ptr [ebp - 0x108]
  49acdc: 8b d8                        	mov	ebx, eax
  49acde: 8b cb                        	mov	ecx, ebx
  49ace0: e8 cb 1d fb ff               	call	0x44cab0 <.text+0x4bab0>

  44c830: 55                           	push	ebp
  44c831: 8b ec                        	mov	ebp, esp
  44c833: 6a ff                        	push	-0x1
  44c835: 68 22 12 4f 00               	push	0x4f1222
  44c83a: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0x0]
  44c840: 50                           	push	eax
  44c841: 51                           	push	ecx
  44c842: 56                           	push	esi
  44c843: a1 f8 b2 52 00               	mov	eax, dword ptr [0x52b2f8]
  44c848: 33 c5                        	xor	eax, ebp
  44c84a: 50                           	push	eax
  44c84b: 8d 45 f4                     	lea	eax, [ebp - 0xc]
  44c84e: 64 a3 00 00 00 00            	mov	dword ptr fs:[0x0], eax
  44c854: 8b f1                        	mov	esi, ecx
  44c856: 8b 46 0c                     	mov	eax, dword ptr [esi + 0xc]
  44c859: 85 c0                        	test	eax, eax
  44c85b: 74 0c                        	je	0x44c869 <.text+0x4b869>
  44c85d: 0f 1f 00                     	nop	dword ptr [eax]
  44c860: 8b f0                        	mov	esi, eax
  44c862: 8b 46 0c                     	mov	eax, dword ptr [esi + 0xc]
  44c865: 85 c0                        	test	eax, eax
  44c867: 75 f7                        	jne	0x44c860 <.text+0x4b860>
  44c869: 68 8c 00 00 00               	push	0x8c
  44c86e: e8 1a 13 08 00               	call	0x4cdb8d <??4SThreadParam@@QAEAAU0@ABU0@@Z+0x940d>
  44c873: 8b d0                        	mov	edx, eax
  44c875: 83 c4 04                     	add	esp, 0x4
  44c878: 89 55 f0                     	mov	dword ptr [ebp - 0x10], edx
  44c87b: c7 45 fc 00 00 00 00         	mov	dword ptr [ebp - 0x4], 0x0
  44c882: 8b ce                        	mov	ecx, esi
  44c884: 6a 00                        	push	0x0
  44c886: 6a 01                        	push	0x1
  44c888: ff 75 10                     	push	dword ptr [ebp + 0x10]
  44c88b: ff 75 0c                     	push	dword ptr [ebp + 0xc]
  44c88e: ff 75 08                     	push	dword ptr [ebp + 0x8]
  44c891: e8 8a ff ff ff               	call	0x44c820 <.text+0x4b820>
  44c896: 50                           	push	eax
  44c897: 8b ca                        	mov	ecx, edx
  44c899: e8 52 fc ff ff               	call	0x44c4f0 <.text+0x4b4f0>
  44c89e: c7 45 fc ff ff ff ff         	mov	dword ptr [ebp - 0x4], 0xffffffff
  44c8a5: 89 46 0c                     	mov	dword ptr [esi + 0xc], eax
  44c8a8: 8b 4d f4                     	mov	ecx, dword ptr [ebp - 0xc]
  44c8ab: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0x0], ecx
  44c8b2: 59                           	pop	ecx
  44c8b3: 5e                           	pop	esi
  44c8b4: 8b e5                        	mov	esp, ebp
  44c8b6: 5d                           	pop	ebp
  44c8b7: c2 0c 00                     	ret	0xc
  44c8ba: cc                           	int3
  44c8bb: cc                           	int3
  44c8bc: cc                           	int3
  44c8bd: cc                           	int3
  44c8be: cc                           	int3
  44c8bf: cc                           	int3
  44c8c0: 55                           	push	ebp
  44c8c1: 8b ec                        	mov	ebp, esp
  44c8c3: 8b 41 0c                     	mov	eax, dword ptr [ecx + 0xc]
  44c8c6: 8d 51 0c                     	lea	edx, [ecx + 0xc]
  44c8c9: 85 c0                        	test	eax, eax
  44c8cb: 74 10                        	je	0x44c8dd <.text+0x4b8dd>
  44c8cd: 8b 4d 08                     	mov	ecx, dword ptr [ebp + 0x8]
  44c8d0: 3b c1                        	cmp	eax, ecx
  44c8d2: 74 0f                        	je	0x44c8e3 <.text+0x4b8e3>
  44c8d4: 8d 50 0c                     	lea	edx, [eax + 0xc]
  44c8d7: 8b 02                        	mov	eax, dword ptr [edx]
  44c8d9: 85 c0                        	test	eax, eax
  44c8db: 75 f3                        	jne	0x44c8d0 <.text+0x4b8d0>
  44c8dd: 33 c0                        	xor	eax, eax
  44c8df: 5d                           	pop	ebp
  44c8e0: c2 04 00                     	ret	0x4
  44c8e3: e8 78 af fe ff               	call	0x437860 <.text+0x36860>
  44c8e8: 89 02                        	mov	dword ptr [edx], eax
  44c8ea: 85 c9                        	test	ecx, ecx
  44c8ec: 74 06                        	je	0x44c8f4 <.text+0x4b8f4>
  44c8ee: 8b 01                        	mov	eax, dword ptr [ecx]
  44c8f0: 6a 01                        	push	0x1
  44c8f2: ff 10                        	call	dword ptr [eax]
  44c8f4: b8 01 00 00 00               	mov	eax, 0x1
  44c8f9: 5d                           	pop	ebp
  44c8fa: c2 04 00                     	ret	0x4
  44c8fd: cc                           	int3
  44c8fe: cc                           	int3
  44c8ff: cc                           	int3
  44c900: 56                           	push	esi
  44c901: 8b f1                        	mov	esi, ecx
  44c903: 8b 4e 0c                     	mov	ecx, dword ptr [esi + 0xc]
  44c906: 85 c9                        	test	ecx, ecx
  44c908: 74 19                        	je	0x44c923 <.text+0x4b923>
  44c90a: e8 f1 ff ff ff               	call	0x44c900 <.text+0x4b900>
  44c90f: 8b 4e 0c                     	mov	ecx, dword ptr [esi + 0xc]
  44c912: 85 c9                        	test	ecx, ecx
  44c914: 74 06                        	je	0x44c91c <.text+0x4b91c>
  44c916: 8b 01                        	mov	eax, dword ptr [ecx]
  44c918: 6a 01                        	push	0x1
  44c91a: ff 10                        	call	dword ptr [eax]
  44c91c: c7 46 0c 00 00 00 00         	mov	dword ptr [esi + 0xc], 0x0
