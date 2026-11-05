; Houkago FD sprite GetProperty 0x10.
; 42E330 dispatches; 42D8E0 returns object+0x160 for every sprite mode.
  42d8e0: 8b 81 60 01 00 00            	mov	eax, dword ptr [ecx + 0x160]
  42d8e6: c3                           	ret
  42e330: 55                           	push	ebp
  42e331: 8b ec                        	mov	ebp, esp
  42e333: 83 e4 f8                     	and	esp, -0x8
  42e336: 8b 45 0c                     	mov	eax, dword ptr [ebp + 0xc]
  42e339: 83 ec 34                     	sub	esp, 0x34
  42e33c: 56                           	push	esi
  42e33d: 8b f1                        	mov	esi, ecx
  42e33f: 3d 00 00 00 10               	cmp	eax, 0x10000000
  42e344: 0f 87 27 01 00 00            	ja	0x42e471 <.text+0x2d471>
  42e34a: 74 3e                        	je	0x42e38a <.text+0x2d38a>
  42e34c: 83 f8 10                     	cmp	eax, 0x10
  42e34f: 74 26                        	je	0x42e377 <.text+0x2d377>
  42e351: 83 f8 41                     	cmp	eax, 0x41
  42e354: 0f 85 1e 01 00 00            	jne	0x42e478 <.text+0x2d478>
  42e35a: ff 75 08                     	push	dword ptr [ebp + 0x8]
  42e35d: e8 ee f6 ff ff               	call	0x42da50 <.text+0x2ca50>
  42e362: f7 d8                        	neg	eax
  42e364: 1b c0                        	sbb	eax, eax
  42e366: 25 ff ff 00 00               	and	eax, 0xffff
  42e36b: 05 01 00 ff ff               	add	eax, 0xffff0001
  42e370: 5e                           	pop	esi
  42e371: 8b e5                        	mov	esp, ebp
  42e373: 5d                           	pop	ebp
  42e374: c2 08 00                     	ret	0x8
  42e377: e8 64 f5 ff ff               	call	0x42d8e0 <.text+0x2c8e0>
  42e37c: 8b 4d 08                     	mov	ecx, dword ptr [ebp + 0x8]
  42e37f: 89 01                        	mov	dword ptr [ecx], eax
  42e381: 33 c0                        	xor	eax, eax
  42e383: 5e                           	pop	esi
  42e384: 8b e5                        	mov	esp, ebp
  42e386: 5d                           	pop	ebp
  42e387: c2 08 00                     	ret	0x8
