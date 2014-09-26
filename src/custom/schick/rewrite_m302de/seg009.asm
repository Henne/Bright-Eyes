.186
.model tiny

pp20		segment	dword public 'CODE'
		assume cs:pp20
		assume es:nothing, ss:nothing, ds:@DATA

decomp_pp20	proc far

arg_0		= word ptr  6
arg_2		= word ptr  8
arg_4		= word ptr  0Ah
arg_6		= word ptr  0Ch
arg_8		= dword	ptr  0Eh
arg_C		= word ptr  12h
arg_E		= word ptr  14h

		enter	0, 0
		push	ds
		push	si
		push	di
		mov	ax, [bp+arg_0]
		mov	dx, [bp+arg_2]
		add	ax, [bp+arg_C]
		jnb	short loc_1FF31
		nop
		nop
		nop
		add	dx, 1000h

loc_1FF31:
		mov	bx, [bp+arg_E]
		shl	bx, 0Ch
		add	dx, bx
		mov	si, ax
		mov	ds, dx
		xor	dx, dx
		sub	si, 4
		jnb	short loc_1FF50
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_1FF50:
		mov	al, [si+3]
		mov	cl, [si]
		mov	bh, [si+1]
		mov	bl, [si+2]
		and	al, al
		jz	short loc_1FF85
		nop
		nop
		nop

loc_1FF62:
		shr	dx, 1
		jnz	short loc_1FF81
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_1FF7A
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_1FF7A:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_1FF81:
		dec	al
		jnz	short loc_1FF62

loc_1FF85:
		mov	ch, 0
		push	dx
		mov	ax, [bp+arg_4]
		mov	dx, [bp+arg_6]
		add	ax, bx
		jnb	short loc_1FF99
		nop
		nop
		nop
		add	dx, 1000h

loc_1FF99:
		shl	cx, 0Ch
		add	dx, cx
		mov	di, ax
		mov	es, dx
		mov	ax, [bp+arg_4]
		mov	cx, [bp+arg_6]
		mov	bl, ch
		shr	bl, 4
		mov	bh, 0
		shl	cx, 4
		add	ax, cx
		mov	cx, 0
		adc	cx, bx
		mov	[bp+arg_4], ax
		mov	[bp+arg_6], cx
		pop	dx

loc_1FFC0:
		shr	dx, 1
		jnz	short loc_1FFDF
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_1FFD8
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_1FFD8:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_1FFDF:
		jnb	short loc_1FFE4
		jmp	loc_20183

loc_1FFE4:
		mov	cx, 1

loc_1FFE7:
		xor	ax, ax
		shr	dx, 1
		jnz	short loc_20008
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20001
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20001:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20008:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_20029
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20022
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20022:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20029:
		rcl	al, 1
		add	cx, ax
		cmp	al, 3
		jz	short loc_1FFE7

loc_20031:
		mov	al, 0
		shr	dx, 1
		jnz	short loc_20052
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_2004B
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_2004B:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20052:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_20073
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_2006C
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_2006C:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20073:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_20094
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_2008D
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_2008D:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20094:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_200B5
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_200AE
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_200AE:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_200B5:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_200D6
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_200CF
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_200CF:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_200D6:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_200F7
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_200F0
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_200F0:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_200F7:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_20118
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20111
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20111:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20118:
		rcl	al, 1
		shr	dx, 1
		jnz	short loc_20139
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20132
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20132:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20139:
		rcl	al, 1
		sub	di, 1
		jnb	short loc_2014C
		nop
		nop
		nop
		push	ax
		mov	ax, es
		sub	ax, 1000h
		mov	es, ax
		pop	ax

loc_2014C:
		mov	es:[di], al

loc_2014F:
		loop	loc_20153
		jmp	short loc_20156

loc_20153:
		jmp	loc_20031

loc_20156:
		mov	ax, di
		mov	cx, es
		mov	bl, ch
		shr	bl, 4
		mov	bh, 0
		shl	cx, 4
		add	ax, cx
		mov	cx, 0
		adc	cx, bx
		cmp	cx, [bp+arg_6]
		ja	short loc_20183
		nop
		nop
		nop
		jnb	short loc_20178
		jmp	loc_20309

loc_20178:
		cmp	ax, [bp+arg_4]
		ja	short loc_20183
		nop
		nop
		nop
		jmp	loc_20309

loc_20183:
		mov	bl, 0
		shr	dx, 1
		jnz	short loc_201A4
		nop
		nop
		nop
		sub	si, 2

loc_2018F:
		jnb	short loc_2019D
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_2019D:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_201A4:
		rcl	bl, 1
		shr	dx, 1
		jnz	short loc_201C5
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_201BE
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_201BE:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_201C5:
		rcl	bl, 1
		mov	bh, 0
		push	es
		push	di
		les	di, [bp+arg_8]
		mov	cl, es:[bx+di]
		pop	di
		pop	es
		cmp	bl, 3
		jz	short loc_201DB
		jmp	loc_20298

loc_201DB:
		shr	dx, 1
		jnz	short loc_201FA
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_201F3
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_201F3:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_201FA:
		jb	short loc_20201
		nop
		nop
		nop
		mov	cl, 7

loc_20201:
		xor	ax, ax
		mov	ch, 0

loc_20205:
		shr	dx, 1
		jnz	short loc_20224
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_2021D
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_2021D:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_20224:
		rcl	ax, 1
		loop	loc_20205
		push	ax

loc_20229:
		xor	cx, cx
		shr	dx, 1
		jnz	short loc_2024A
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20243
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20243:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_2024A:
		rcl	cl, 1
		shr	dx, 1
		jnz	short loc_2026B
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20264
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20264:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_2026B:
		rcl	cl, 1
		shr	dx, 1
		jnz	short loc_2028C
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_20285
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_20285:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_2028C:
		rcl	cl, 1
		add	bx, cx
		cmp	cl, 7
		jz	short loc_20229
		jmp	short loc_202C0

		align 2

loc_20298:
		mov	ch, 0
		xor	ax, ax

loc_2029C:
		shr	dx, 1
		jnz	short loc_202BB
		nop
		nop
		nop
		sub	si, 2
		jnb	short loc_202B4
		nop
		nop
		nop
		push	ax
		mov	ax, ds
		sub	ax, 1000h
		mov	ds, ax
		pop	ax

loc_202B4:
		mov	dx, [si]
		xchg	dh, dl
		stc
		rcr	dx, 1

loc_202BB:
		rcl	ax, 1
		loop	loc_2029C
		push	ax

loc_202C0:
		add	bx, 2
		mov	cx, bx
		pop	bx
loc_202c6:

		mov	al, es:[bx+di]
		sub	di, 1
		jnb	short loc_202DA
		nop
		nop
		nop
		push	ax
		mov	ax, es
		sub	ax, 1000h
		mov	es, ax
		pop	ax

loc_202DA:
		mov	es:[di], al

loc_202DD:
		loop	loc_202C6
		mov	ax, di
		mov	cx, es
		mov	bl, ch
		shr	bl, 4
		mov	bh, 0
		shl	cx, 4
		add	ax, cx
		mov	cx, 0
		adc	cx, bx
		cmp	cx, [bp+arg_6]
		jbe	short loc_202FC
		jmp	loc_1FFC0

loc_202FC:
		jb	short loc_20309
		nop
		nop
		nop
		cmp	ax, [bp+arg_4]
		jbe	short loc_20309

loc_20306:
		jmp	loc_1FFC0

loc_20309:
		pop	di
		pop	si
		pop	ds
		leave
		retf

decomp_pp20	endp

pp20		ends

		end
