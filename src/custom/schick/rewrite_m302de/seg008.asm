; Segment type:	Pure code
;
; There are 32 assembler-written, low-level VGA-routines.
; Only 14 of them are used in the game, and get meaningful names
; The other 18 are called unused_func01 .. unused_func18


.186
.model large

Rasterlib	segment	byte public 'CODE'
		assume cs:Rasterlib
		;org 8
		assume es:nothing, ss:nothing, ds:@DATA


swap_u16	proc far

		push	bp
		mov	bp, sp

		mov	ax, [bp+6]
		xchg	ah, al

		mov	sp, bp
		pop	bp
		retf

swap_u16	endp


set_video_mode	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ah, 0
		mov	al, [bp+6]
		int	10h

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

set_video_mode	endp


set_video_page	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	al, [bp+6]
		mov	ah, 5
		int	10h

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

set_video_page	endp


save_display_stat proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		push	bp

		les	di, [bp+6]
		mov	ah, 0Fh
		int	10h
		mov	dx, ax
		xor	ax, ax
		mov	al, bh
		stosw
		mov	al, dl
		stosw
		mov	al, dh
		stosw
		push	es
		mov	ax, 1130h
		mov	bh, 2
		int	10h
		xor	dh, dh
		inc	dx
		pop	es
		mov	es:[di], dx

		pop	bp
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

save_display_stat endp


unused_func01	proc	far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ah, 2
		mov	bh, [bp+6]
		mov	dh, [bp+8]
		mov	dl, [bp+0Ah]

					; - VIDEO - SET	CURSOR POSITION
		int	10h		; DH,DL	= row, column (0,0 = upper left)
					; BH = page number

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func01	endp


unused_func02	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ah, 3
		mov	bh, [bp+6]
					; - VIDEO - READ CURSOR	POSITION
		int	10h		; BH = page number
					; Return: DH,DL	= row,column, CH = cursor start	line, CL = cursor end line

		mov	al, dh
		mov	ah, dl

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func02 endp


unused_func03	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ch, [bp+6]
		mov	cl, [bp+8]
		and	cx, 1FFFh
		mov	ah, 1

					; - VIDEO - SET	CURSOR CHARACTERISTICS
		int	10h		; CH bits 0-4 =	start line for cursor in character cell
					; bits 5-6 = blink attribute
					; CL bits 0-4 =	end line for cursor in character cell

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func03 endp


unused_func04	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		lds	si, [bp+6]
		mov	cx, 10h
		xor	bl, bl

u_f04_l1:
		mov	bh, [si]
		mov	ax, 1000h
					; - VIDEO - SET	PALETTE	REGISTER (Jr, PS, TANDY	1000, EGA, VGA)
		int	10h		; BL = palette register	to set
					; BH = color value to store
		inc	si
		inc	bl
		loop	u_f04_l1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
unused_func04 endp

set_color	proc far

;ptr		= dword	ptr  6
;color		= byte ptr  0Ah

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		cli
		mov	dx, 3C8h
		mov	al, [bp+0ah]
		out	dx, al
		lds	si, [bp+6]
		mov	dx, 3C9h
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		mov	dx, 3DAh

set_color_loop:
		in	al, dx		; Video	status bits:
					; 0: retrace.  1=display is in vert or horiz retrace.
					; 1: 1=light pen is triggered; 0=armed
					; 2: 1=light pen switch	is open; 0=closed
					; 3: 1=vertical	sync pulse is occurring.
		and	al, 1

		jz	set_color_loop

		mov	dx, 3C9h
		lodsb
		out	dx, al

		sti

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
set_color	endp


set_palette	proc far

;ptr		= dword	ptr  6
;first_color	= byte ptr  0Ah
;colors		= word ptr  0Ch

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		cli

		mov	dx, 3C8h
		mov	al, [bp+0ah]	;# of the first color
		out	dx, al
		lds	si, [bp+6]	;ds:si == pointer to color data
		mov	dx, 3C9h
		mov	cx, [bp+0ch]	;# of colors

set_palette_loop1:
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		mov	dx, 3DAh

set_palette_loop2:
		in	al, dx		; Video	status bits:
					; 0: retrace.  1=display is in vert or horiz retrace.
					; 1: 1=light pen is triggered; 0=armed
					; 2: 1=light pen switch	is open; 0=closed
					; 3: 1=vertical	sync pulse is occurring.
		and	al, 1
		jz	set_palette_loop2

		mov	dx, 3C9h
		lodsb
		out	dx, al
		loop	set_palette_loop1

		sti

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
set_palette	endp


draw_h_line	proc far

;ptr		= dword	ptr  6
;cnt		= word ptr  0Ah
;color		= byte ptr  0Ch

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		les	di, [bp+6]
		mov	cx, [bp+0ah]
		mov	al, [bp+0ch]
		mov	ah, al
		shr	cx, 1
		jnb	draw_h_line_label1
		nop
		nop
		nop
		stosb

draw_h_line_label1:
		rep stosw

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

draw_h_line	endp


unused_func05	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	cx, 0A0h ; ' '
		mov	ax, 0A000h
		mov	ds, ax
		assume ds:nothing
		mov	si, [bp+6]
		les	di, [bp+8]
		rep movsw
		pop	di
		pop	si
		pop	es
		pop	ds
		assume ds:@DATA
		mov	sp, bp
		pop	bp
		retf

unused_func05	endp


unused_func06	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	cx, 0A0h
		mov	ax, 0A000h
		mov	es, ax
		assume es:nothing
		mov	di, [bp+6]
		lds	si, [bp+8]
		rep movsw

		pop	di
		pop	si
		pop	es
		assume es:nothing
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func06	endp


draw_h_spaced_dots proc	far

;ptr		= dword	ptr  6
;count		= word ptr  0Ah
;color		= byte ptr  0Ch
;space		= word ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		les	di, [bp+6]
		mov	cx, [bp+0ah]
		mov	al, [bp+0ch]

draw_h_spaced_dots_loop1:
		mov	es:[di], al
		add	di, [bp+0eh]
		loop	draw_h_spaced_dots_loop1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

draw_h_spaced_dots endp


unused_func07	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		cld
		mov	ax, 0A000h
		mov	ds, ax
		assume ds:nothing
		mov	si, [bp+6]
		les	di, [bp+8]
		mov	cx, 200

u_f07_loop1:
		movsb
		add	si, 320 - 1
		loop	u_f07_loop1
		pop	di
		pop	si
		pop	es
		pop	ds
		assume ds:@DATA
		mov	sp, bp
		pop	bp
		retf

unused_func07	endp


unused_func08	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		cld
		mov	ax, 0A000h
		mov	es, ax
		assume es:nothing
		mov	di, [bp+6]
		lds	si, [bp+8]
		mov	cx, 200

u_f08_loop1:
		movsb
		add	di, 320 - 1
		loop	u_f08_loop1

		pop	di
		pop	si
		pop	es
		assume es:nothing
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func08	endp


pic_copy	proc far

lv5		= word ptr -1Ah
lv4		= word ptr -18h
lv3		= word ptr -16h
lv2		= word ptr -14h
lv1		= word ptr -12h
cur_height	= word ptr -10h
cur_width	= word ptr -0Eh
ds_4		= word ptr -0Ch
ds_2		= word ptr -0Ah
ds_1		= word ptr -8
ds_3		= word ptr -6
dst		= dword	ptr  6
x1		= word ptr  0Ah
y1		= word ptr  0Ch
x2		= word ptr  0Eh
y2		= word ptr  10h
val1		= word ptr  12h
val2		= word ptr  14h
src_width	= word ptr  1Ah
src_height	= word ptr  1Ch
src		= dword	ptr  1Eh
mode		= word ptr  22h

		push	bp
		mov	bp, sp
		sub	sp, 1Ah
		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		lds	si, [bp+src]
		mov	ax, [bp+src_width]
		mov	[bp+cur_width],	ax
		mov	ax, [bp+src_height]
		mov	[bp+cur_height], ax
		mov	[bp+lv1], 0
		mov	[bp+lv2], 0
		mov	[bp+lv3], 0
		mov	[bp+lv4], 0
		mov	ax, cs:87D0h
		mov	[bp+ds_1], ax
		mov	ax, cs:87D2h
		mov	[bp+ds_2], ax
		mov	ax, cs:87D4h
		mov	[bp+ds_3], ax
		mov	ax, cs:87D6h
		mov	[bp+ds_4], ax
		mov	ax, [bp+y1]
		cmp	ax, [bp+ds_1]
		jge	short loc_1F403

loc_1F3ED:
		nop
		nop
		nop

loc_1F3F0:
		mov	ax, [bp+ds_1]

loc_1F3F3:
		sub	ax, [bp+y1]

loc_1F3F6:
		mov	[bp+lv3], ax

loc_1F3F9:
		sub	[bp+cur_height], ax
		mov	cx, [bp+src_width]

loc_1F3FF:
		mul	cx
		add	si, ax

loc_1F403:				; CODE XREF: pic_copy:loc_1F3EBj
		mov	ax, [bp+y2]

loc_1F406:
		cmp	ax, [bp+ds_3]
		jle	short loc_1F417

loc_1F40B:
		nop
		nop
		nop

loc_1F40E:
		sub	ax, [bp+ds_3]

loc_1F411:
		mov	[bp+lv4], ax
		sub	[bp+cur_height], ax

loc_1F417:				; CODE XREF: pic_copy+70j
		mov	ax, [bp+x1]

loc_1F41A:
		cmp	ax, [bp+ds_2]
		jge	short loc_1F430
		nop
		nop
		nop

loc_1F422:
		mov	ax, [bp+ds_2]

loc_1F425:
		sub	ax, [bp+x1]

loc_1F428:
		mov	[bp+lv1], ax
		sub	[bp+cur_width],	ax
		add	si, ax

loc_1F430:				; CODE XREF: pic_copy+84j
		mov	ax, [bp+x2]
		cmp	ax, [bp+ds_4]
		jle	short loc_1F444

loc_1F438:
		nop
		nop
		nop
		sub	ax, [bp+ds_4]

loc_1F43E:
		mov	[bp+lv2], ax
		sub	[bp+cur_width],	ax

loc_1F444:				; CODE XREF: pic_copy+9Dj
		cmp	[bp+cur_height], 0

loc_1F448:
		jg	short loc_1F450
		nop
		nop
		nop

loc_1F44D:
		jmp	@@leave
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F450:				; CODE XREF: pic_copy:loc_1F448j
		cmp	[bp+cur_width],	0

loc_1F454:
		jg	short loc_1F45C
		nop
		nop
		nop

loc_1F459:
		jmp	@@leave
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F45C:				; CODE XREF: pic_copy:loc_1F454j
		mov	ax, [bp+ds_1]

loc_1F45F:
		mov	cx, 320
		mul	cx
		add	di, ax

loc_1F466:
		add	di, [bp+ds_2]

loc_1F469:
		mov	ax, [bp+y1]

loc_1F46C:
		cmp	ax, [bp+ds_1]
		jle	short loc_1F47E
		nop
		nop
		nop
		sub	ax, [bp+ds_1]
		mov	cx, 320
		mul	cx
		add	di, ax

loc_1F47E:				; CODE XREF: pic_copy+D6j
		mov	ax, [bp+x1]

loc_1F481:
		cmp	ax, [bp+ds_2]
		jle	short loc_1F48E
		nop
		nop

loc_1F488:
		nop
		sub	ax, [bp+ds_2]

loc_1F48C:
		add	di, ax

loc_1F48E:				; CODE XREF: pic_copy+EBj
		mov	[bp+lv5], 0

loc_1F493:
		mov	ax, [bp+src_width]

loc_1F496:
		sub	ax, [bp+cur_width]

loc_1F499:
		mov	[bp+lv5], ax

loc_1F49C:
		cmp	[bp+mode], 1

loc_1F4A0:
		jnz	short val5_neq_1
		nop
		nop
		nop
		jmp	short val5_eq_1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

val5_neq_1:				; CODE XREF: pic_copy:loc_1F4A0j
		cmp	[bp+mode], 2

loc_1F4AC:
		jnz	short val5_neq_2
		nop
		nop
		nop

loc_1F4B1:
		jmp	val5_eq_2
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

val5_neq_2:				; CODE XREF: pic_copy:loc_1F4ACj
		cmp	[bp+mode], 3

loc_1F4B8:
		jnz	short val5_neq_3
		jmp	val5_eq_3
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

val5_neq_3:				; CODE XREF: pic_copy:loc_1F4B8j
		jmp	val5_default
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

val5_eq_1:				; CODE XREF: pic_copy+10Cj
		mov	dx, [bp+cur_height]
		mov	bx, 320

loc_1F4C6:
		sub	bx, [bp+cur_width]

mode1_loop:				; CODE XREF: pic_copy+2B7j
		mov	cx, [bp+cur_width]
		cmp	word ptr cs:0A8D2h, 0

loc_1F4D2:
		jz	short loc_1F509

loc_1F4D4:
		nop
		nop
		nop

loc_1F4D7:				; CODE XREF: pic_copy:loc_1F504j
		mov	al, es:[di]

loc_1F4DA:
		cmp	dx, 40

loc_1F4DD:
		jnb	short check_col

loc_1F4DF:
		nop

loc_1F4E0:
		nop
		nop

loc_1F4E2:
		cmp	cx, 75
		jle	short check_col
		nop
		nop
		nop

loc_1F4EA:
		cmp	cx, 150
		jnb	short check_col
		nop
		nop
		nop
		jmp	short write
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

check_col:				; CODE XREF: pic_copy:loc_1F4DDj
					; pic_copy+14Cj ...
		cmp	al, 0C8h
		jnb	short continue
		nop
		nop
		nop

write:					; CODE XREF: pic_copy+15Aj
		mov	al, [si]
		mov	es:[di], al

continue:				; CODE XREF: pic_copy+15Fj
		inc	si
		inc	di

loc_1F504:
		loop	loc_1F4D7
		jmp	loc_1F648
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F509:				; CODE XREF: pic_copy:loc_1F4D2j
		cmp	cx, 10h
		jnb	short loc_1F511

loc_1F50E:
		jmp	loc_1F62F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F511:				; CODE XREF: pic_copy+173j
					; pic_copy:loc_1F62Cj
		cmp	byte ptr es:[di], 0C8h ; 'È'
		jnb	short loc_1F51F
		nop
		nop
		nop

loc_1F51A:
		mov	al, [si]

loc_1F51C:
		mov	es:[di], al

loc_1F51F:				; CODE XREF: pic_copy+17Cj
		cmp	byte ptr es:[di+1], 0C8h ; 'È'
		jnb	short loc_1F530
		nop
		nop
		nop

loc_1F529:
		mov	al, [si+1]

loc_1F52C:
		mov	es:[di+1], al

loc_1F530:				; CODE XREF: pic_copy+18Bj
		cmp	byte ptr es:[di+2], 0C8h ; 'È'
		jnb	short loc_1F541
		nop
		nop
		nop
		mov	al, [si+2]
		mov	es:[di+2], al

loc_1F541:				; CODE XREF: pic_copy+19Cj
		cmp	byte ptr es:[di+3], 0C8h ; 'È'
		jnb	short loc_1F552
		nop
		nop
		nop
		mov	al, [si+3]

loc_1F54E:
		mov	es:[di+3], al

loc_1F552:				; CODE XREF: pic_copy+1ADj
		cmp	byte ptr es:[di+4], 0C8h ; 'È'

loc_1F557:
		jnb	short loc_1F563
		nop

loc_1F55A:
		nop

loc_1F55B:
		nop

loc_1F55C:
		mov	al, [si+4]
		mov	es:[di+4], al

loc_1F563:				; CODE XREF: pic_copy:loc_1F557j
		cmp	byte ptr es:[di+5], 0C8h ; 'È'

loc_1F568:
		jnb	short loc_1F574

loc_1F56A:
		nop
		nop

loc_1F56C:
		nop

loc_1F56D:
		mov	al, [si+5]

loc_1F570:
		mov	es:[di+5], al

loc_1F574:				; CODE XREF: pic_copy:loc_1F568j
		cmp	byte ptr es:[di+6], 0C8h ; 'È'
		jnb	short loc_1F585
		nop
		nop
		nop
		mov	al, [si+6]

loc_1F581:
		mov	es:[di+6], al

loc_1F585:				; CODE XREF: pic_copy+1E0j
		cmp	byte ptr es:[di+7], 0C8h ; 'È'
		jnb	short loc_1F596
		nop

loc_1F58D:
		nop

loc_1F58E:
		nop
		mov	al, [si+7]
		mov	es:[di+7], al

loc_1F596:				; CODE XREF: pic_copy+1F1j
		cmp	byte ptr es:[di+8], 0C8h ; 'È'
		jnb	short loc_1F5A7
		nop
		nop
		nop

loc_1F5A0:
		mov	al, [si+8]

loc_1F5A3:
		mov	es:[di+8], al

loc_1F5A7:				; CODE XREF: pic_copy+202j
		cmp	byte ptr es:[di+9], 0C8h ; 'È'
		jnb	short loc_1F5B8
		nop
		nop
		nop

loc_1F5B1:
		mov	al, [si+9]

loc_1F5B4:
		mov	es:[di+9], al

loc_1F5B8:				; CODE XREF: pic_copy+213j
		cmp	byte ptr es:[di+0Ah], 0C8h ; 'È'
		jnb	short loc_1F5C9

loc_1F5BF:
		nop
		nop
		nop
		mov	al, [si+0Ah]

loc_1F5C5:
		mov	es:[di+0Ah], al

loc_1F5C9:				; CODE XREF: pic_copy+224j
		cmp	byte ptr es:[di+0Bh], 0C8h ; 'È'
		jnb	short loc_1F5DA
		nop

loc_1F5D1:
		nop
		nop
		mov	al, [si+0Bh]

loc_1F5D6:
		mov	es:[di+0Bh], al

loc_1F5DA:				; CODE XREF: pic_copy+235j
		cmp	byte ptr es:[di+0Ch], 0C8h ; 'È'
		jnb	short loc_1F5EB
		nop
		nop

loc_1F5E3:
		nop

loc_1F5E4:
		mov	al, [si+0Ch]

loc_1F5E7:
		mov	es:[di+0Ch], al

loc_1F5EB:				; CODE XREF: pic_copy+246j
		cmp	byte ptr es:[di+0Dh], 0C8h ; 'È'

loc_1F5F0:
		jnb	short loc_1F5FC
		nop
		nop

loc_1F5F4:
		nop

loc_1F5F5:
		mov	al, [si+0Dh]

loc_1F5F8:
		mov	es:[di+0Dh], al

loc_1F5FC:				; CODE XREF: pic_copy:loc_1F5F0j
		cmp	byte ptr es:[di+0Eh], 0C8h ; 'È'

loc_1F601:
		jnb	short loc_1F60D
		nop
		nop

loc_1F605:
		nop
		mov	al, [si+0Eh]
		mov	es:[di+0Eh], al

loc_1F60D:				; CODE XREF: pic_copy:loc_1F601j
		cmp	byte ptr es:[di+0Fh], 0C8h ; 'È'
		jnb	short loc_1F61E

loc_1F614:
		nop
		nop

loc_1F616:
		nop
		mov	al, [si+0Fh]

loc_1F61A:
		mov	es:[di+0Fh], al

loc_1F61E:				; CODE XREF: pic_copy+279j
		add	si, 10h

loc_1F621:
		add	di, 10h
		sub	cx, 10h
		cmp	cx, 10h

loc_1F62A:
		jb	short loc_1F62F

loc_1F62C:
		jmp	loc_1F511
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F62F:				; CODE XREF: pic_copy:loc_1F50Ej
					; pic_copy:loc_1F62Aj
		jcxz	short loc_1F648
		nop
		nop

loc_1F633:
		nop
		nop
		nop

loc_1F636:				; CODE XREF: pic_copy:loc_1F646j
		cmp	byte ptr es:[di], 0C8h ; 'È'
		jnb	short loc_1F644
		nop
		nop
		nop
		mov	al, [si]
		mov	es:[di], al

loc_1F644:				; CODE XREF: pic_copy+2A1j
		inc	si
		inc	di

loc_1F646:
		loop	loc_1F636

loc_1F648:				; CODE XREF: pic_copy+16Dj
					; pic_copy:loc_1F62Fj
		add	di, bx

loc_1F64A:
		add	si, [bp+lv5]

loc_1F64D:
		dec	dx

loc_1F64E:
		jle	short _leave_eq_1
		jmp	mode1_loop
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

_leave_eq_1:				; CODE XREF: pic_copy:loc_1F64Ej
		jmp	@@leave
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

val5_eq_2:				; CODE XREF: pic_copy:loc_1F4B1j
		mov	dx, [bp+cur_height]

loc_1F659:
		mov	bx, 320

loc_1F65C:
		sub	bx, [bp+cur_width]

loc_1F65F:				; CODE XREF: pic_copy+3DFj
		mov	cx, [bp+cur_width]
		cmp	cx, 10h
		jnb	short loc_1F66A
		jmp	loc_1F760
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_1F66A:				; CODE XREF: pic_copy+2CCj
					; pic_copy:loc_1F756j
		mov	al, [si]
		or	al, al
		jz	short loc_1F676
		nop
		nop
		nop
		mov	es:[di], al

loc_1F676:				; CODE XREF: pic_copy+2D5j
		mov	al, [si+1]
		or	al, al

loc_1F67B:
		jz	short loc_1F684
		nop

loc_1F67E:
		nop
		nop

loc_1F680:
		mov	es:[di+1], al

loc_1F684:				; CODE XREF: pic_copy:loc_1F67Bj
		mov	al, [si+2]

loc_1F687:
		or	al, al
		jz	short loc_1F692
		nop
		nop

loc_1F68D:
		nop
		mov	es:[di+2], al

loc_1F692:				; CODE XREF: pic_copy+2F0j
		mov	al, [si+3]
		or	al, al
		jz	short loc_1F6A0
		nop
		nop

loc_1F69B:
		nop
		mov	es:[di+3], al

loc_1F6A0:				; CODE XREF: pic_copy+2FEj
		mov	al, [si+4]
		or	al, al
		jz	short loc_1F6AE
		nop

loc_1F6A8:
		nop
		nop
		mov	es:[di+4], al

loc_1F6AE:				; CODE XREF: pic_copy+30Cj
		mov	al, [si+5]
		or	al, al
		jz	short loc_1F6BC
		nop
		nop
		nop

loc_1F6B8:
		mov	es:[di+5], al

loc_1F6BC:				; CODE XREF: pic_copy+31Aj
		mov	al, [si+6]
		or	al, al
		jz	short loc_1F6CA
		nop
		nop
		nop

loc_1F6C6:
		mov	es:[di+6], al

loc_1F6CA:				; CODE XREF: pic_copy+328j
		mov	al, [si+7]

loc_1F6CD:
		or	al, al

loc_1F6CF:
		jz	short loc_1F6D8
		nop
		nop
		nop
		mov	es:[di+7], al

loc_1F6D8:				; CODE XREF: pic_copy:loc_1F6CFj
		mov	al, [si+8]
		or	al, al
		jz	short loc_1F6E6
		nop

loc_1F6E0:
		nop
		nop
		mov	es:[di+8], al

loc_1F6E6:				; CODE XREF: pic_copy+344j
		mov	al, [si+9]
		or	al, al
		jz	short loc_1F6F4
		nop
		nop
		nop
		mov	es:[di+9], al

loc_1F6F4:				; CODE XREF: pic_copy+352j
		mov	al, [si+0Ah]
		or	al, al
		jz	short loc_1F702
		nop

loc_1F6FC:
		nop
		nop

loc_1F6FE:
		mov	es:[di+0Ah], al

loc_1F702:				; CODE XREF: pic_copy+360j
		mov	al, [si+0Bh]
		or	al, al
		jz	short loc_1F710
		nop
		nop
		nop
		mov	es:[di+0Bh], al

loc_1F710:				; CODE XREF: pic_copy+36Ej
		mov	al, [si+0Ch]

loc_1F713:
		or	al, al
		jz	short loc_1F71E
		nop
		nop
		nop
		mov	es:[di+0Ch], al

loc_1F71E:				; CODE XREF: pic_copy+37Cj
		mov	al, [si+0Dh]
		or	al, al
		jz	short loc_1F72C
		nop
		nop
		nop
		mov	es:[di+0Dh], al

loc_1F72C:				; CODE XREF: pic_copy+38Aj
		mov	al, [si+0Eh]

loc_1F72F:
		or	al, al
		jz	short loc_1F73A
		nop
		nop
		nop
		mov	es:[di+0Eh], al

loc_1F73A:				; CODE XREF: pic_copy+398j
		mov	al, [si+0Fh]
		or	al, al

loc_1F73F:
		jz	short loc_1F748
		nop
		nop
		nop
		mov	es:[di+0Fh], al

loc_1F748:				; CODE XREF: pic_copy:loc_1F73Fj
		add	si, 10h

loc_1F74B:
		add	di, 10h

loc_1F74E:
		sub	cx, 10h
		cmp	cx, 10h
		jb	short loc_1F759

loc_1F756:
		jmp	loc_1F66A

loc_1F759:				; CODE XREF: pic_copy+3BBj
		jcxz	short loc_1F770
		nop

loc_1F75C:
		nop
		nop
		nop
		nop

loc_1F760:				; CODE XREF: pic_copy+2CEj
					; pic_copy+3D5j
		mov	al, [si]
		cmp	al, 0
		jz	short loc_1F76C
		nop
		nop
		nop
		mov	es:[di], al

loc_1F76C:				; CODE XREF: pic_copy+3CBj
		inc	si
		inc	di
		loop	loc_1F760

loc_1F770:				; CODE XREF: pic_copy:loc_1F759j
		add	di, bx

loc_1F772:
		add	si, [bp+lv5]
		dec	dx
		jle	short loc_1F77B
		jmp	loc_1F65F

loc_1F77B:				; CODE XREF: pic_copy+3DDj
		jmp	short @@leave
		align 2

val5_eq_3:				; CODE XREF: pic_copy+121j
		mov	ax, [bp+val2]
		mov	cx, 140h
		mul	cx
		add	ax, [bp+val1]
		add	si, ax
		mov	dx, [bp+cur_height]
		mov	bx, 140h
		sub	bx, [bp+cur_width]
		mov	ax, [bp+cur_width]

loc_1F797:				; CODE XREF: pic_copy+40Fj
		mov	cx, ax

loc_1F799:
		shr	cx, 1
		jnb	short loc_1F7A1
		nop
		nop
		nop
		movsb

loc_1F7A1:				; CODE XREF: pic_copy+402j
		rep movsw
		add	di, bx
		add	si, bx
		dec	dx
		jg	short loc_1F797
		jmp	short @@leave
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		db 90h
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

val5_default:				; CODE XREF: pic_copy:val5_neq_3j
		mov	dx, [bp+cur_height]
		mov	bx, 140h

loc_1F7B3:
		sub	bx, [bp+cur_width]

loc_1F7B6:
		mov	ax, [bp+cur_width]

@@do_loop:				; CODE XREF: pic_copy+432j
		mov	cx, ax
		shr	cx, 1
		jnb	short loc_1F7C3
		nop
		nop
		nop
		movsb

loc_1F7C3:				; CODE XREF: pic_copy+424j
		rep movsw
		add	di, bx
		add	si, [bp+lv5]
		dec	dx
		jg	short @@do_loop

@@leave:				; CODE XREF: pic_copy:loc_1F44Dj
					; pic_copy:loc_1F459j ...
		pop	si
		pop	ds
		pop	di
		pop	es
		mov	sp, bp
		pop	bp
		retf
pic_copy	endp


save_rect	proc far

src_seg		= word ptr  6
src_off		= word ptr  8
dst		= dword	ptr  0Ah
r_width		= word ptr  0Eh
height		= word ptr  10h

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, [bp+src_seg]
		mov	ds, ax
		mov	si, [bp+src_off]
		les	di, [bp+dst]
		mov	dx, [bp+height]
		mov	bx, 320
		sub	bx, [bp+r_width]
		mov	ax, [bp+r_width]

save_rect_loop1:
		mov	cx, ax
		shr	cx, 1
		jnb	save_rect_label2
		nop
		nop
		nop
		movsb

save_rect_label2:
		rep movsw
		add	si, bx
		dec	dx
		jg	save_rect_loop1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

save_rect	endp


fill_rect	proc far

segm		= word ptr  6
off		= word ptr  8
color		= byte ptr  0Ah
p_width		= word ptr  0Ch
height		= word ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, [bp+segm]
		mov	es, ax
		mov	di, [bp+off]
		mov	dx, [bp+height]
		mov	bx, 320
		sub	bx, [bp+p_width]
		mov	cx, [bp+p_width]
		mov	al, [bp+color]
		mov	ah, al

loc_1F82C:				; CODE XREF: fill_rect+2Fj
		push	cx
		shr	cx, 1
		jnb	short loc_1F835
		nop
		nop
		nop
		stosb

loc_1F835:				; CODE XREF: fill_rect:loc_1F82Fj
		rep stosw
		pop	cx
		add	di, bx
		dec	dx
		jg	short loc_1F82C

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
fill_rect	endp


csp_copy	macro off
		local csp_skip

		mov	al, [si - off]
		cmp	ax, [bp+solid]
		jz	csp_skip
		nop
		nop
		nop
		mov	es:[di + off], al
csp_skip:

		endm

copy_solid_permuted proc far

dst		= dword	ptr  6
src		= dword	ptr  0Ah
width_to_copy	= word ptr  0Eh
height		= word ptr  10h
dst_width	= word ptr  12h
src_width	= word ptr  14h
solid		= word ptr  16h

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		les	di, [bp+dst]
		lds	si, [bp+src]
		xor	ax, ax
		xor	dx, dx
		jmp	csp_loop_condition

csp_loop_start:
		push	si
		push	di
		mov	cx, [bp+width_to_copy]
		cmp	cx, 10h
		jnb	short loc_1F866
		jmp	loc_1F965

		;the # of pixels to copy are >= 16
		;

loc_1F866:				; CODE XREF: copy_solid_permuted+1Cj

							; copy_solid_permuted+11Dj
		mov	al, [si]
		cmp	ax, [bp+solid]
		jz	short loc_1F873
		nop
		nop
		nop
		mov	es:[di], al

loc_1F873:				; CODE XREF: copy_solid_permuted+26j
		mov	al, [si-1]
		cmp	ax, [bp+solid]
		jz	short loc_1F882
		nop
		nop
		nop
		mov	es:[di+1], al

loc_1F882:				; CODE XREF: copy_solid_permuted+34j
		mov	al, [si-2]
		cmp	ax, [bp+solid]
		jz	short loc_1F891
		nop
		nop
		nop
		mov	es:[di+2], al

loc_1F891:				; CODE XREF: copy_solid_permuted+43j
		mov	al, [si-3]
		cmp	ax, [bp+solid]
		jz	short loc_1F8A0
		nop
		nop
		nop
		mov	es:[di+3], al

loc_1F8A0:				; CODE XREF: copy_solid_permuted+52j
		mov	al, [si-4]
		cmp	ax, [bp+solid]
		jz	short loc_1F8AF
		nop
		nop
		nop
		mov	es:[di+4], al

loc_1F8AF:				; CODE XREF: copy_solid_permuted+61j
		mov	al, [si-5]
		cmp	ax, [bp+solid]
		jz	short loc_1F8BE
		nop
		nop
		nop
		mov	es:[di+5], al

loc_1F8BE:				; CODE XREF: copy_solid_permuted+70j
		mov	al, [si-6]
		cmp	ax, [bp+solid]
		jz	short loc_1F8CD
		nop
		nop
		nop
		mov	es:[di+6], al

loc_1F8CD:				; CODE XREF: copy_solid_permuted+7Fj
		mov	al, [si-7]
		cmp	ax, [bp+solid]
		jz	short loc_1F8DC
		nop
		nop
		nop
		mov	es:[di+7], al

loc_1F8DC:				; CODE XREF: copy_solid_permuted+8Ej
		mov	al, [si-8]
		cmp	ax, [bp+solid]
		jz	short loc_1F8EB
		nop
		nop
		nop
		mov	es:[di+8], al

loc_1F8EB:				; CODE XREF: copy_solid_permuted:loc_1F8E2j
		mov	al, [si-9]
		cmp	ax, [bp+solid]
		jz	short loc_1F8FA
		nop
		nop
		nop
		mov	es:[di+9], al

loc_1F8FA:				; CODE XREF: copy_solid_permuted+ACj
		mov	al, [si-0Ah]
		cmp	ax, [bp+solid]
		jz	short loc_1F909
		nop
		nop
		nop
		mov	es:[di+0Ah], al

loc_1F909:				; CODE XREF: copy_solid_permuted+BBj
		mov	al, [si-0Bh]
		cmp	ax, [bp+solid]
		jz	short loc_1F918
		nop
		nop
		nop
		mov	es:[di+0Bh], al

loc_1F918:				; CODE XREF: copy_solid_permuted:loc_1F90Fj
		mov	al, [si-0Ch]
		cmp	ax, [bp+solid]
		jz	short loc_1F927
		nop
		nop
		nop
		mov	es:[di+0Ch], al

loc_1F927:				; CODE XREF: copy_solid_permuted+D9j
		mov	al, [si-0Dh]
		cmp	ax, [bp+solid]
		jz	short loc_1F936
		nop
		nop
		nop
		mov	es:[di+0Dh], al

loc_1F936:				; CODE XREF: copy_solid_permuted+E8j
		mov	al, [si-0Eh]
		cmp	ax, [bp+solid]
		jz	short loc_1F945
		nop
		nop
		nop
		mov	es:[di+0Eh], al

loc_1F945:				; CODE XREF: copy_solid_permuted+F7j
		mov	al, [si-0Fh]
		cmp	ax, [bp+solid]
		jz	short loc_1F954
		nop
		nop
		nop
		mov	es:[di+0Fh], al

loc_1F954:				; CODE XREF: copy_solid_permuted+106j
		add	di, 10h
		sub	si, 10h
		sub	cx, 10h
		cmp	cx, 10h
		jb	short loc_1F965
		jmp	loc_1F866

loc_1F965:				; CODE XREF: copy_solid_permuted:loc_1F863j
					; copy_solid_permuted:loc_1F960j
		jcxz	short loc_1F97D
		nop
		nop
		nop
		nop
		nop

loc_1F96C:				; CODE XREF: copy_solid_permuted+136j
		mov	al, [si]
		cmp	ax, [bp+solid]
		jz	short loc_1F979
		nop
		nop
		nop
		mov	es:[di], al

loc_1F979:				; CODE XREF: copy_solid_permuted:loc_1F971j
		inc	di
		dec	si
		loop	loc_1F96C

loc_1F97D:				; CODE XREF: copy_solid_permuted:loc_1F965j
		pop	di
		pop	si
		add	si, [bp+src_width]
		add	di, [bp+dst_width]
		inc	dx

csp_loop_condition:
		cmp	dx, [bp+height]
		jge	short csp_loop_leave
		jmp	csp_loop_start

csp_loop_leave:
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
copy_solid_permuted endp


copy_solid	proc far

dst		= dword	ptr  6
src		= dword	ptr  0Ah
width_to_copy	= word ptr  0Eh
height		= word ptr  10h
dst_width	= word ptr  12h
src_width	= word ptr  14h
solid		= word ptr  16h

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		les	di, [bp+dst]
		lds	si, [bp+src]
		xor	ax, ax
		xor	dx, dx
		jmp	cs_loop_condition

@@loop_start:				; CODE XREF: copy_solid+146j
		push	si
		push	di
		mov	cx, [bp+width_to_copy]
		cmp	cx, 10h
		jnb	short loc_1F9B7
		jmp	@@lt_16

loc_1F9B7:				; CODE XREF: copy_solid:loc_1F9B2j
					; copy_solid:loc_1FAB3j
		mov	al, [si]
		cmp	ax, [bp+solid]
		jz	short loc_1F9C4
		nop
		nop
		nop
		mov	es:[di], al

loc_1F9C4:				; CODE XREF: copy_solid+26j
		mov	al, [si+1]
		cmp	ax, [bp+solid]
		jz	short loc_1F9D3
		nop
		nop
		nop
		mov	es:[di+1], al

loc_1F9D3:				; CODE XREF: copy_solid+34j
		mov	al, [si+2]
		cmp	ax, [bp+solid]
		jz	short loc_1F9E2
		nop
		nop
		nop
		mov	es:[di+2], al

loc_1F9E2:				; CODE XREF: copy_solid:loc_1F9D9j
		mov	al, [si+3]
		cmp	ax, [bp+solid]
		jz	short loc_1F9F1
		nop
		nop
		nop
		mov	es:[di+3], al

loc_1F9F1:				; CODE XREF: copy_solid+52j
		mov	al, [si+4]
		cmp	ax, [bp+solid]
		jz	short loc_1FA00
		nop
		nop
		nop
		mov	es:[di+4], al

loc_1FA00:				; CODE XREF: copy_solid:loc_1F9F7j
		mov	al, [si+5]
		cmp	ax, [bp+solid]
		jz	short loc_1FA0F
		nop
		nop
		nop
		mov	es:[di+5], al

loc_1FA0F:				; CODE XREF: copy_solid+70j
		mov	al, [si+6]
		cmp	ax, [bp+solid]
		jz	short loc_1FA1E
		nop
		nop
		nop
		mov	es:[di+6], al

loc_1FA1E:				; CODE XREF: copy_solid+7Fj
		mov	al, [si+7]
		cmp	ax, [bp+solid]
		jz	short loc_1FA2D
		nop
		nop
		nop
		mov	es:[di+7], al

loc_1FA2D:				; CODE XREF: copy_solid+8Ej
		mov	al, [si+8]
		cmp	ax, [bp+solid]
		jz	short loc_1FA3C
		nop
		nop
		nop
		mov	es:[di+8], al

loc_1FA3C:				; CODE XREF: copy_solid:loc_1FA33j
		mov	al, [si+9]
		cmp	ax, [bp+solid]
		jz	short loc_1FA4B
		nop
		nop
		nop
		mov	es:[di+9], al

loc_1FA4B:				; CODE XREF: copy_solid+ACj
		mov	al, [si+0Ah]
		cmp	ax, [bp+solid]
		jz	short loc_1FA5A
		nop
		nop
		nop
		mov	es:[di+0Ah], al

loc_1FA5A:				; CODE XREF: copy_solid+BBj
		mov	al, [si+0Bh]
		cmp	ax, [bp+solid]
		jz	short loc_1FA69
		nop
		nop
		nop
		mov	es:[di+0Bh], al

loc_1FA69:				; CODE XREF: copy_solid+CAj
		mov	al, [si+0Ch]
		cmp	ax, [bp+solid]
		jz	short loc_1FA78
		nop
		nop
		nop
		mov	es:[di+0Ch], al

loc_1FA78:				; CODE XREF: copy_solid:loc_1FA6Fj
		mov	al, [si+0Dh]
		cmp	ax, [bp+solid]
		jz	short loc_1FA87
		nop
		nop
		nop
		mov	es:[di+0Dh], al

loc_1FA87:				; CODE XREF: copy_solid+E8j
		mov	al, [si+0Eh]
		cmp	ax, [bp+solid]
		jz	short loc_1FA96
		nop
		nop
		nop
		mov	es:[di+0Eh], al

loc_1FA96:				; CODE XREF: copy_solid+F7j
		mov	al, [si+0Fh]
		cmp	ax, [bp+solid]
		jz	short loc_1FAA5
		nop
		nop
		nop
		mov	es:[di+0Fh], al

loc_1FAA5:				; CODE XREF: copy_solid+106j
		add	si, 10h
		add	di, 10h
		sub	cx, 10h
		cmp	cx, 10h
		jb	short @@lt_16

loc_1FAB3:
		jmp	loc_1F9B7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

@@lt_16:				; CODE XREF: copy_solid:loc_1F9B4j
					; copy_solid+11Bj
		jcxz	short @@continue
		nop
		nop
		nop
		nop
		nop

loc_1FABD:				; CODE XREF: copy_solid+136j
		mov	al, [si]

loc_1FABF:
		cmp	ax, [bp+solid]
		jz	short loc_1FACA
		nop
		nop
		nop
		mov	es:[di], al

loc_1FACA:				; CODE XREF: copy_solid+12Cj
		inc	di
		inc	si
		loop	loc_1FABD

@@continue:				; CODE XREF: copy_solid:@@lt_16j
		pop	di
		pop	si
		add	si, [bp+src_width]
		add	di, [bp+dst_width]
		inc	dx

cs_loop_condition:			; CODE XREF: copy_solid+11j
		cmp	dx, [bp+height]
		jge	short cs_leave
		jmp	@@loop_start

cs_leave:				; CODE XREF: copy_solid+144j
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

copy_solid	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: library function bp-based	frame

decomp_rle	proc far		; CODE XREF: process_nvf+42FP

p_width		= word ptr  6
height		= word ptr  8
dst		= dword	ptr  0Ah
src		= dword	ptr  0Eh
tmp_buffer	= dword	ptr  12h
mode		= word ptr  16h

		push	bp
		mov	bp, sp

loc_1FAEA:
		push	ds
		push	es
		push	si
		push	di
		mov	dx, [bp+height]

@@y_loop_start:				; CODE XREF: decomp_rle:loc_1FC31j
		les	di, [bp+src]

loc_1FAF4:
		cmp	[bp+mode], 5

loc_1FAF8:
		jz	short mode45
		nop
		nop

loc_1FAFC:
		nop

loc_1FAFD:
		cmp	[bp+mode], 4
		jnz	short mode23
		nop
		nop
		nop

mode45:					; CODE XREF: decomp_rle:loc_1FAF8j
		lds	si, [bp+tmp_buffer]
		jmp	short @@do_it
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

mode23:					; CODE XREF: decomp_rle+1Aj
		lds	si, [bp+dst]

@@do_it:				; CODE XREF: decomp_rle+22j
		push	dx
		mov	dx, [bp+p_width]

@@x_loop_start:				; CODE XREF: decomp_rle:loc_1FB3Dj
		mov	al, es:[di]
		inc	di

loc_1FB17:
		cmp	al, 7Fh	; ''
		jnz	short @@no_id_byte
		nop

loc_1FB1C:
		nop

loc_1FB1D:
		nop
		xor	bx, bx
		xor	cx, cx

loc_1FB22:
		mov	bl, es:[di]

loc_1FB25:
		inc	di
		mov	cl, bl

loc_1FB28:
		mov	al, es:[di]
		inc	di

@@repeat_byte:				; CODE XREF: decomp_rle+48j
		mov	[si], al
		inc	si
		loop	@@repeat_byte
		sub	dx, bx

loc_1FB33:				; __linkproc__ line_condition
		jmp	short @@line_condition
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

@@no_id_byte:				; CODE XREF: decomp_rle+32j
		mov	[si], al
		inc	si
		dec	dx

; __linkproc__ line_condition
@@line_condition:			; CODE XREF: decomp_rle:loc_1FB33j
		cmp	dx, 0

loc_1FB3D:
		jg	short @@x_loop_start
		pop	dx
		mov	word ptr [bp+src], di
		mov	word ptr [bp+src+2], es
		les	di, [bp+dst]
		cmp	[bp+mode], 5
		jz	short @@mode45
		nop
		nop
		nop

loc_1FB52:
		cmp	[bp+mode], 4
		jz	short @@mode45

loc_1FB58:				; __linkproc__ mode23
		jmp	@@mode23
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

@@mode45:				; CODE XREF: decomp_rle+66j
					; decomp_rle+6Fj
		dec	si
		mov	cx, [bp+p_width]
		cmp	cx, 10h

loc_1FB62:
		jnb	short @@width_lt_16
		jmp	@@mode_45_end	; __linkproc__ mode_45_end
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

@@width_lt_16:				; CODE XREF: decomp_rle:loc_1FB62j
					; decomp_rle:loc_1FBE1j
		mov	al, [si]
		mov	es:[di], al
		mov	al, [si-1]
		mov	es:[di+1], al
		mov	al, [si-2]
		mov	es:[di+2], al
		mov	al, [si-3]
		mov	es:[di+3], al
		mov	al, [si-4]
		mov	es:[di+4], al
		mov	al, [si-5]
		mov	es:[di+5], al
		mov	al, [si-6]
		mov	es:[di+6], al
		mov	al, [si-7]
		mov	es:[di+7], al
		mov	al, [si-8]
		mov	es:[di+8], al
		mov	al, [si-9]
		mov	es:[di+9], al
		mov	al, [si-0Ah]
		mov	es:[di+0Ah], al
		mov	al, [si-0Bh]
		mov	es:[di+0Bh], al
		mov	al, [si-0Ch]
		mov	es:[di+0Ch], al
		mov	al, [si-0Dh]
		mov	es:[di+0Dh], al
		mov	al, [si-0Eh]
		mov	es:[di+0Eh], al
		mov	al, [si-0Fh]
		mov	es:[di+0Fh], al
		add	di, 10h
		sub	si, 10h
		sub	cx, 10h
		cmp	cx, 10h
		jnb	short @@width_lt_16
		jcxz	short @@mode23	; __linkproc__ mode23
		nop
		nop
		nop
		nop
		nop

; __linkproc__ mode_45_end
@@mode_45_end:				; CODE XREF: decomp_rle+7Dj
					; decomp_rle+10Aj
		mov	al, [si]
		mov	es:[di], al
		inc	di
		dec	si
		loop	@@mode_45_end	; __linkproc__ mode_45_end

; __linkproc__ mode23
@@mode23:				; CODE XREF: decomp_rle:loc_1FB58j
					; decomp_rle+FCj
		cmp	[bp+mode], 2
		jz	short @@mode2
		nop
		nop
		nop
		cmp	[bp+mode], 4
		jz	short @@mode4	; __linkproc__ mode4
		nop
		nop
		nop
		cmp	[bp+mode], 3
		jnz	short @@proceed
		nop
		nop
		nop
		add	di, [bp+p_width]
		jmp	short @@proceed
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

@@mode2:				; CODE XREF: decomp_rle+110j
		add	di, 320

loc_1FC18:
		jmp	short @@proceed
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		db 90h
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

; __linkproc__ mode4
@@mode4:				; CODE XREF: decomp_rle+119j
		add	di, 320
		sub	di, [bp+p_width]
		jmp	short @@proceed
		db 90h
@@proceed:				; CODE XREF: decomp_rle+122j
					; decomp_rle+12Aj ...
		mov	word ptr [bp+dst], di
		mov	word ptr [bp+dst+2], es
		dec	dx
		jle	short rle_leave
		nop
		nop
		nop
		jmp	@@y_loop_start
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

rle_leave:				; CODE XREF: decomp_rle+145j
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
decomp_rle	endp

; OPL3 - code

byte_1FC3C	db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
dword_1FC4A	dd 0			; DATA XREF: sub_1FD30:loc_1FDA4r
					; sub_1FD30:loc_1FE1Cr	...
dword_1FC4E	dd 0			; DATA XREF: sub_1FD30+49r
					; sub_1FD30:loc_1FE2Cw	...
byte_1FC52	db 0, 0, 0, 0		; DATA XREF: sub_1FD30+ACw
					; sub_1FD30+C0r ...
word_1FC56	dw 0			; DATA XREF: sub_1FD30+79r
					; sub_1FD30+81w ...
byte_1FC58	db 0			; DATA XREF: sub_1FD30:loc_1FD5Cr
					; sub_1FD30:loc_1FDFFw	...
byte_1FC59	db 0			; DATA XREF: sub_1FD30+36r
					; sub_1FD30+9Cw ...
dummy6		db 0


unused_func09	proc far

		push	cx
		mov	dx, 388h
		out	dx, al
		mov	cx, word ptr cs:byte_1FC3C
		cli

loc_1FC66:				; CODE XREF: sub_1FC5A+Fj
					; sub_1FC5A+11j
		in	al, dx
		and	al, 0
		jnz	short loc_1FC66
		loop	loc_1FC66
		sti
		mov	al, ah
		inc	dx
		out	dx, al
		dec	dx
		mov	cx, word ptr cs:byte_1FC3C+2
		cli
		nop

loc_1FC7A:				; CODE XREF: sub_1FC5A+23j
					; sub_1FC5A+25j
		in	al, dx
		and	al, 0
		jnz	short loc_1FC7A
		loop	loc_1FC7A
		sti

		pop	cx
		retf
unused_func09	endp


unused_func10	proc far
		mov	ah, 80h
		mov	al, 4
		call	unused_func09

		mov	ah, 80h
		mov	al, 2
		call	unused_func09

		mov	dx, 388h
		mov	al, 2
		out	dx, al
		mov	cx, word ptr cs:byte_1FC3C
		cli

loc_1FCA0:				; CODE XREF: sub_1FC84+1Fj
					; sub_1FC84+21j
		in	al, dx
		and	al, 0
		jnz	short loc_1FCA0
		loop	loc_1FCA0
		sti
		xor	cx, cx
		mov	al, 1
		inc	dx
		out	dx, al
		dec	dx
		cli

loc_1FCB0:				; CODE XREF: sub_1FC84+34j
		in	al, dx
		and	al, 40h
		jnz	short loc_1FCBD
		nop
		nop
		nop
		loop	loc_1FCB0
		mov	cx, 0E4A8h

loc_1FCBD:				; CODE XREF: sub_1FC84+2Fj
		sti
		neg	cx
		mov	ax, cx
		xor	dx, dx
		mov	bx, 0C1Fh
		div	bx
		inc	ax
		mov	word ptr cs:byte_1FC3C,	ax
		xor	dx, dx
		mov	ax, cx
		mov	bx, 1BDh
		div	bx
		inc	ax
		mov	word ptr cs:byte_1FC3C+2, ax
		mov	ah, 80h
		mov	al, 4
		call	unused_func09
		retf

unused_func10	endp


unused_func11	proc far		; CODE XREF: sub_1FE8F:loc_1FEA8p
					; sub_1FEBF:loc_1FECAp
		mov	ah, 20h	; ' '
		mov	al, 1
		call	unused_func09

		mov	ah, 80h
		mov	al, 4
		call	unused_func09

		mov	ah, 0
		mov	al, 8
		call	unused_func09

		mov	ah, 20h	; ' '
		mov	al, 0BDh
		call	unused_func09

		xor	bl, bl

loc_1FD07:				; CODE XREF: sub_1FCE5:loc_1FD18j
		mov	al, 0B0h
		add	al, bl
		mov	ah, 0
		mov	al, al
		call	unused_func09

		inc	bl
		cmp	bl, 8
		jbe	short loc_1FD07
		xor	bl, bl

loc_1FD1C:				; CODE XREF: sub_1FCE5+48j
		mov	al, 40h	; '@'
		add	al, bl
		mov	ah, 3Fh	; '?'
		mov	al, al
		call	unused_func09
		inc	bl
		cmp	bl, 15h
		jbe	short loc_1FD1C
		retf

unused_func11	endp


unused_func12	proc far

		push	ax
		push	bx
		mov	bx, word ptr cs:byte_1FC3C+0Ch
		dec	bl
		jnz	short loc_1FD49
		nop
		nop
		nop
		mov	bl, 4
		pushf
		call	dword ptr cs:byte_1FC3C+4
		jmp	short loc_1FD4D
		nop

loc_1FD49:				; CODE XREF: sub_1FD30+9j
		mov	al, 20h	; ' '
		out	20h, al		; Interrupt controller,	8259A.

loc_1FD4D:				; CODE XREF: sub_1FD30+16j
		dec	bh
		jz	short loc_1FD5C
		nop
		nop
		nop
		mov	word ptr cs:byte_1FC3C+0Ch, bx
		jmp	loc_1FE3A

loc_1FD5C:				; CODE XREF: sub_1FD30+1Fj
		mov	bh, cs:byte_1FC58
		mov	word ptr cs:byte_1FC3C+0Ch, bx
		cmp	cs:byte_1FC59, 1
		jnz	short loc_1FD74
		nop
		nop

loc_1FD70:
		nop
		jmp	loc_1FE3A

loc_1FD74:				; CODE XREF: sub_1FD30+3Cj
		push	cx
		push	dx
		push	si
		push	ds
		cld
		lds	si, cs:dword_1FC4E

loc_1FD7E:				; CODE XREF: sub_1FD30+C5j
					; sub_1FD30+D3j ...
		lodsb
		cmp	al, 1
		jnz	short loc_1FDBB
		nop
		nop
		nop

loc_1FD86:				; CODE XREF: sub_1FD30+62j
		lodsw
		cmp	al, 0FFh
		jz	short loc_1FD94
		nop
		nop
		nop
		call	unused_func09
		jmp	short loc_1FD86

loc_1FD94:				; CODE XREF: sub_1FD30+59j
		cmp	ah, 0FFh
		jnz	short loc_1FD9C
		jmp	loc_1FE2C

loc_1FD9C:				; CODE XREF: sub_1FD30+67j
		cmp	ah, 0FEh ; 'ş'
		jz	short loc_1FDA4
		jmp	loc_1FE2C

loc_1FDA4:				; CODE XREF: sub_1FD30+6Fj
		lds	si, cs:dword_1FC4A
		mov	bx, cs:word_1FC56
		add	bx, 2
		mov	cs:word_1FC56, bx
		add	si, [bx+si]
		jmp	short loc_1FE2C
		nop

loc_1FDBB:				; CODE XREF: sub_1FD30+51j
		cmp	al, 0
		jnz	short loc_1FDC5
		nop
		nop
		nop
		jmp	short loc_1FE2C
		nop

loc_1FDC5:				; CODE XREF: sub_1FD30+8Dj
		cmp	al, 2
		jnz	short loc_1FDD5
		nop
		nop
		nop
		mov	cs:byte_1FC59, 1
		jmp	short loc_1FE2C
		nop

loc_1FDD5:				; CODE XREF: sub_1FD30+97j
		cmp	al, 3
		jnz	short loc_1FDE9
		nop
		nop
		nop
		mov	word ptr cs:byte_1FC52,	si
		mov	word ptr cs:byte_1FC52+2, ds
		jmp	short loc_1FE2C
		nop

loc_1FDE9:				; CODE XREF: sub_1FD30+A7j
		cmp	al, 4
		jnz	short loc_1FDF7
		nop
		nop
		nop
		lds	si, dword ptr cs:byte_1FC52
		jmp	short loc_1FD7E

loc_1FDF7:				; CODE XREF: sub_1FD30+BBj
		cmp	al, 5
		jnz	short loc_1FE06
		nop
		nop
		nop
		lodsb
		mov	cs:byte_1FC58, al
		jmp	loc_1FD7E

loc_1FE06:				; CODE XREF: sub_1FD30:loc_1FDF9j
		cmp	al, 6
		jnz	short loc_1FE26
		nop
		nop
		nop
		lodsb
		xor	ah, ah
		shl	ax, 1
		mov	cs:word_1FC56, ax
		mov	bx, ax
		mov	cs:word_1FC56, ax
		lds	si, cs:dword_1FC4A
		add	si, [bx+si]
		jmp	loc_1FD7E

loc_1FE26:				; CODE XREF: sub_1FD30:loc_1FE08j
		mov	cs:byte_1FC59, 1

loc_1FE2C:				; CODE XREF: sub_1FD30:loc_1FD99j
					; sub_1FD30+71j ...
		mov	word ptr cs:dword_1FC4E, si
		mov	word ptr cs:dword_1FC4E+2, ds
		pop	ds
		pop	si
		pop	dx
		pop	cx
loc_1FE3A:
		pop	bx
		pop	ax
		iret
unused_func12	endp


unused_func13	proc far
		push	ds
		push	es
		cli
		mov	al, 36h
		out	43h, al
		xor	ax, ax
		out	40h, al
		out	40h, al
		mov	es, ax
		assume es:nothing
		lds	dx, dword ptr cs:byte_1FC3C+4
		mov	es:20h,	dx
		mov	word ptr es:22h, ds
		sti
		pop	es
		assume es:nothing
		pop	ds
		retf

unused_func13	endp

unused_func14	proc far
		push	es
		push	ds
		mov	dx, 0BB0h
		xor	ax, ax
		mov	ds, ax
		assume ds:nothing
		cli
		les	bx, ds:20h
		mov	ds:20h,	dx
		mov	word ptr ds:22h, cs
		mov	word ptr cs:byte_1FC3C+4, bx
		mov	word ptr cs:byte_1FC3C+6, es
		mov	al, 36h
		out	43h, al
		xor	al, al
		out	40h, al
		mov	al, 40h
		out	40h, al
		sti
		pop	ds
		assume ds:@DATA
		pop	es
		retf

unused_func14	endp


unused_func15	proc far

		push	ax
		push	bx
		push	cx
		push	dx

		mov	word ptr cs:byte_1FC3C,	2000
		mov	word ptr cs:byte_1FC3C+2, 7000
		mov	cs:byte_1FC59, 1
		call	unused_func11

		call	unused_func10

		mov	ax, 2308
		mov	word ptr cs:byte_1FC3C+0Ch, ax
		call	unused_func14

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		retf

unused_func15	endp


unused_func16	proc far

		push	ax
		push	bx
		push	cx
		push	dx

		mov	cs:byte_1FC59, 1
		call	unused_func11

		call	unused_func13

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		retf

unused_func16	endp

unused_func17	proc far

		mov	cs:byte_1FC59, 1
		retf

unused_func17	endp


unused_func18	proc far

arg_0		= dword	ptr  6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	cs:byte_1FC58, 9
		lds	si, [bp+arg_0]
		mov	word ptr cs:dword_1FC4A, si
		mov	word ptr cs:dword_1FC4A+2, ds
		mov	cs:word_1FC56, 0
		add	si, [si]
		mov	word ptr cs:dword_1FC4E, si
		mov	word ptr cs:dword_1FC4E+2, ds
		mov	cs:byte_1FC59, 0
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

unused_func18	endp

Rasterlib	ends
		end
