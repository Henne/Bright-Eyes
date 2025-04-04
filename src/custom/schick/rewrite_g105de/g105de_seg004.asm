.186
.model large
.data
dword_1EC48	dd 0
word_1EC4C	dw 0
word_1EC4E	dw 0
dword_1EC50	dd 0
dword_1EC54	dd 0
word_1EC58	dw 0
word_1EC5A	dw 0
word_1EC5C	dw 0
word_1EC5E	dw 0
word_1EC60	dw 0
word_1EC62	dw 0
word_1EC64	dw 0

.code

	public _decomp_pp20


_decomp_pp20	proc far


arg_0		= dword	ptr  6
arg_4		= dword	ptr  0Ah
arg_8		= dword	ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, word ptr [bp+arg_0+2]
		mov	bx, word ptr [bp+arg_0]
		mov	word ptr dword_1EC50+2,	ax
		mov	word ptr dword_1EC50, bx
		mov	ax, word ptr [bp+arg_4+2]
		mov	bx, word ptr [bp+arg_4]
		mov	di, bx
		xor	dx, dx
		add	bx, word ptr [bp+arg_8]
		adc	dx, word ptr [bp+arg_8+2]
		mov	cx, 0Ch
		shl	dx, cl
		add	ax, dx
		mov	word ptr dword_1EC48+2,	ax
		mov	word ptr dword_1EC48, bx
		add	di, 4
		mov	word ptr dword_1EC54+2,	ax
		mov	word ptr dword_1EC54, di
		call	sub_1B3E3
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_decomp_pp20	endp

sub_1B3E3	proc near
		mov	word_1EC62, 1
		mov	word_1EC64, 0
		mov	ax, word ptr dword_1EC50
		mov	word_1EC4C, ax
		mov	ax, word ptr dword_1EC50+2
		mov	word_1EC4E, ax
		xor	ax, ax
		sub	word ptr dword_1EC48, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dword_1EC48+2,	ax
		les	di, dword_1EC48
		mov	bh, es:[di]
		mov	bl, es:[di+1]
		mov	ah, es:[di+2]
		mov	al, es:[di+3]
		mov	word_1EC5A, ax
		mov	word_1EC5C, bx
		cmp	al, 0
		jz	short loc_1B448
		call	sub_1B589
		dec	byte ptr word_1EC5A
		mov	ax, word_1EC62
		mov	dx, word_1EC64
		mov	cx, word_1EC5A
		call	sub_1B679
		mov	word_1EC62, ax
		mov	word_1EC64, dx

loc_1B448:
		mov	cl, 8
		mov	ax, word_1EC5A
		mov	dx, word_1EC5C
		call	sub_1B679
		mov	word_1EC5A, ax
		mov	word_1EC5C, dx
		mov	ax, word_1EC5A
		xor	dx, dx
		add	word ptr dword_1EC50, ax
		adc	dx, word_1EC5C
		mov	cx, 0Ch
		shl	dx, cl
		add	word ptr dword_1EC50+2,	dx

loc_1B471:
		call	sub_1B589
		jb	short loc_1B4D2
		mov	word_1EC5E, 0

loc_1B47C:
		mov	word_1EC58, 1
		call	sub_1B5F0
		mov	ax, word_1EC5A
		add	word_1EC5E, ax
		cmp	ax, 3
		jz	short loc_1B47C

loc_1B491:
		mov	word_1EC58, 7
		call	sub_1B5F0
		xor	ax, ax
		sub	word ptr dword_1EC50, 1
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dword_1EC50+2,	ax
		mov	ax, word_1EC5A
		les	di, dword_1EC50
		mov	es:[di], al
		dec	word_1EC5E
		jge	short loc_1B491
		mov	cx, word ptr dword_1EC50+2
		mov	bx, word ptr dword_1EC50
		mov	dx, word_1EC4E
		mov	ax, word_1EC4C
		call	sub_1B697
		jb	short loc_1B4D2
		retn

loc_1B4D2:
		mov	word_1EC58, 1
		call	sub_1B5F0
		mov	word_1EC58, 0
		les	di, dword_1EC54
		add	di, word_1EC5A
		mov	al, es:[di]
		mov	byte ptr word_1EC58, al
		mov	ax, word_1EC5A
		mov	word_1EC5E, ax
		cmp	ax, 3
		jnz	short loc_1B528
		call	sub_1B589
		jb	short loc_1B505
		mov	word_1EC58, 7

loc_1B505:
		call	sub_1B5EC
		mov	ax, word_1EC5A
		mov	word_1EC60, ax

loc_1B50E:
		mov	word_1EC58, 2
		call	sub_1B5F0
		mov	ax, word_1EC5A
		add	word_1EC5E, ax
		cmp	word_1EC5A, 7
		jz	short loc_1B50E
		jmp	short loc_1B531
;REMARK: may become an obstacle
		nop
;

loc_1B528:
		call	sub_1B5EC
		mov	ax, word_1EC5A
		mov	word_1EC60, ax

loc_1B531:
		inc	word_1EC5E

loc_1B535:
		push	ds
		les	di, dword_1EC50
		xor	ax, ax
		add	di, word_1EC60
		adc	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		mov	bx, es
		add	bx, ax
		mov	es, bx
		xor	ax, ax
		sub	word ptr dword_1EC50, 1
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dword_1EC50+2,	ax
		lds	si, dword_1EC50
		mov	al, es:[di]
		mov	[si], al
		pop	ds
		dec	word_1EC5E
		jge	short loc_1B535
		mov	cx, word ptr dword_1EC50+2
		mov	bx, word ptr dword_1EC50
		mov	dx, word_1EC4E
		mov	ax, word_1EC4C
		call	sub_1B697
		jnb	short locret_1B588
		jmp	loc_1B471

locret_1B588:
		retn
sub_1B3E3	endp


sub_1B589	proc near
		mov	bx, word_1EC62
		mov	ax, word_1EC64
		mov	dx, 0
		shr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B59C
		mov	dx, 1

loc_1B59C:
		cmp	ax, 0
		jnz	short loc_1B5DE
		cmp	bx, 0
		jnz	short loc_1B5DE
		xor	ax, ax
		sub	word ptr dword_1EC48, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dword_1EC48+2,	ax
		les	di, dword_1EC48
		mov	ah, es:[di]
		mov	al, es:[di+1]
		mov	bh, es:[di+2]
		mov	bl, es:[di+3]
		mov	cx, dx
		clc
		jcxz	short loc_1B5D2
		stc

loc_1B5D2:
		mov	dx, 0
		rcr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B5DE
		mov	dx, 1

loc_1B5DE:

		mov	word_1EC62, bx
		mov	word_1EC64, ax
		mov	cx, dx
		clc
		jcxz	short locret_1B5EB
		stc

locret_1B5EB:
		retn
sub_1B589	endp

sub_1B5EC	proc near
		dec	word_1EC58
sub_1B5EC	endp

sub_1B5F0	proc near

		mov	word_1EC5A, 0
		mov	word_1EC5C, 0

loc_1B5FC:
		mov	bx, word_1EC62
		mov	ax, word_1EC64
		mov	dx, 0
		shr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B60F
		mov	dx, 1

loc_1B60F:
		cmp	ax, 0
		jnz	short loc_1B651
		cmp	bx, 0
		jnz	short loc_1B651
		xor	ax, ax
		sub	word ptr dword_1EC48, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dword_1EC48+2,	ax
		les	di, dword_1EC48
		mov	ah, es:[di]
		mov	al, es:[di+1]
		mov	bh, es:[di+2]
		mov	bl, es:[di+3]
		mov	cx, dx
		clc
		jcxz	short loc_1B645
		stc

loc_1B645:
		mov	dx, 0
		rcr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B651
		mov	dx, 1

loc_1B651:

		mov	word_1EC62, bx
		mov	word_1EC64, ax
		mov	bx, word_1EC5A
		mov	ax, word_1EC5C
		mov	cx, dx
		clc
		jcxz	short loc_1B665
		stc

loc_1B665:
		rcl	bx, 1
		rcl	ax, 1
		mov	word_1EC5A, bx
		mov	word_1EC5C, ax
		dec	word_1EC58
		jl	short locret_1B678
		jmp	short loc_1B5FC

locret_1B678:
		retn
sub_1B5F0	endp


sub_1B679	proc near

		cmp	cl, 10h
		jnb	short loc_1B68E
		mov	bx, dx
		shr	ax, cl
		shr	dx, cl
		neg	cl
		add	cl, 10h
		shl	bx, cl
		or	ax, bx
		retn

loc_1B68E:
		sub	cl, 10h
		xchg	ax, dx
		xor	dx, dx
		shr	ax, cl
		retn
sub_1B679	endp

sub_1B697	proc near

		push	cx
		mov	ch, al
		mov	cl, 4
		shr	ax, cl
		add	dx, ax
		mov	al, ch
		mov	ah, bl
		shr	bx, cl
		pop	cx
		add	cx, bx
		mov	bl, ah
		and	ax, 0Fh
		and	bx, 0Fh
		cmp	dx, cx
		jnz	short locret_1B6B7
		cmp	ax, bx

locret_1B6B7:
		retn
sub_1B697	endp

		end
