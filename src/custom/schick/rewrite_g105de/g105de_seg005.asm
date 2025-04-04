; Segment type:	Pure code
;
; There are 32 assembler-written, low-level VGA-routines.
; Only 14 of them are used in the game, and get meaningful names
; The other 18 are called unused_func01 .. unused_func18

.186
.model large
.code

	public _set_video_mode
	public _set_video_page
	public _save_display_stat
	public _set_color
	public _set_palette
	public _draw_h_line
	public _draw_h_spaced_dots
	public _pic_copy
	public _save_rect
	public _fill_rect
	public _copy_solid_permuted
	public _copy_solid
	public _swap_u16
	public _copy_to_screen
	public unused_func13
	public _normalize_ptr

	extrn F_LXLSH@:far

	assume cs:@code


_set_video_mode	proc far

mode		= byte ptr 6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ah, 0
		mov	al, [bp+mode]
		int	10h

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_set_video_mode	endp


_set_video_page	proc far

page_no		= byte ptr 6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	al, [bp+page_no]
		mov	ah, 5
		int	10h

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_set_video_page	endp


_save_display_stat proc far

pointer		= dword ptr 6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		push	bp

		les	di, [bp+pointer]
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

_save_display_stat endp


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

_set_color	proc far

pnt		= dword	ptr  6
color		= byte ptr  0Ah

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	dx, 3C8h
		mov	al, [bp+color]
		out	dx, al
		lds	si, [bp+pnt]
		mov	dx, 3C9h
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		lodsb
		out	dx, al

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_set_color	endp


_set_palette	proc far

p_palette	= dword	ptr  6
first_color	= byte ptr  0Ah
colors		= word ptr  0Ch

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	dx, 3C8h
		mov	al, [bp+first_color]	;# of the first color
		out	dx, al
		lds	si, [bp+p_palette]	;ds:si == pointer to color data
		mov	dx, 3C9h
		mov	cx, [bp+colors]		;# of colors

set_palette_loop1:
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		loop set_palette_loop1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_set_palette	endp


_draw_h_line	proc far

pnt		= word	ptr  6
cnt		= word ptr  08h
color		= byte ptr  0ah

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, 0A000h
		mov	es, ax
		mov	di, [bp+pnt]
		mov	cx, [bp+cnt]
		mov	al, [bp+color]
		rep stosb

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_draw_h_line	endp


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


_draw_h_spaced_dots proc	far

pptr		= word ptr  6
count		= word ptr  08h
color		= byte ptr  0ah
space		= word ptr  0ch

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, 0A000h
		mov	es, ax
		mov	di, [bp+pptr]
		mov	cx, [bp+count]
		mov	al, [bp+color]

draw_h_spaced_dots_loop1:
		mov	es:[di], al
		add	di, [bp+space]
		loop	draw_h_spaced_dots_loop1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_draw_h_spaced_dots endp


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


_pic_copy	proc far

dst		= dword	ptr  6
x		= word ptr  0Ah
y		= word ptr  0Ch
dummy1		= dword ptr 0Eh
v1		= word ptr  12h
v2		= word ptr  14h
dummy2		= dword ptr 16h
w		= word ptr  1Ah
h		= word ptr  1Ch
src		= dword	ptr  1Eh
mode		= word ptr  22h

		push	bp
		mov	bp, sp
		sub	sp, 8h
		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		mov	ax, [bp+y]
		mov	cx, 320
		mul	cx
		add	ax, [bp+x]
		add	di, ax
		lds	si, [bp+src]

		cmp	[bp+mode], 1
		jnz	case2
		jmp	mode_1
case2:
		cmp	[bp+mode], 2
		jnz	case3
		jmp	mode_2
case3:
		cmp	[bp+mode], 3
		jnz	default
		jmp	mode_3
mode_1:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
m1_h_loop:
		mov	cx, [bp+w]
m1_w_loop:
		mov	al, es:[di]
		cmp	al, 200
		jnb	m1_w_skip
		mov	al, [si]
		mov	es:[di], al
m1_w_skip:
		inc	si
		inc	di
		loop	m1_w_loop
		add	di, bx
		dec	dx
		jg	m1_h_loop
		jmp	l_leave

mode_2:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
m2_h_loop:
		mov	cx, [bp+w]
m2_w_loop:
		mov	al, [si]
		cmp	al, 0
		jz	m2_w_skip
		mov	es:[di], al
m2_w_skip:
		inc	si
		inc	di
		loop	m2_w_loop
		add	di, bx
		dec	dx
		jg	m2_h_loop
		jmp	l_leave

mode_3:
		mov	ax, [bp+v2]
		mov	cx, 320
		mul	cx
		add	ax, [bp+v1]
		add	si, ax
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
		mov	ax, [bp+w]
m3_loop:
		mov	cx, ax
		rep	movsb
		add	di, bx
		add	si, bx
		dec	dx
		jg	m3_loop
		jmp	l_leave

default:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
		mov	ax, [bp+w]
def_loop:
		mov	cx, ax
		rep	movsb
		add	di, bx
		dec	dx
		jg	def_loop

l_leave:
		pop	si
		pop	ds
		pop	di
		pop	es
		mov	sp, bp
		pop	bp
		retf
_pic_copy	endp


_save_rect	proc far

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
		rep 	movsb
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

_save_rect	endp


_fill_rect	proc far

pptr		= dword ptr  6
color		= byte ptr  0Ah
p_width		= word ptr  0Ch
height		= word ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, word ptr [bp+pptr]
		mov	es, ax
		mov	di, word ptr [bp+pptr+2]
		mov	dx, [bp+height]
		mov	bx, 320
		sub	bx, [bp+p_width]
		mov	cx, [bp+p_width]
		mov	al, [bp+color]

save_loop:
		push	cx
		rep	stosb
		pop	cx
		add	di, bx
		dec	dx
		jg	save_loop

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_fill_rect	endp

_copy_solid_permuted proc far

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
		jmp	csp_loop1_condition

csp_loop1_body:
		push	si
		push	di
		mov	cx, [bp+width_to_copy]

csp_loop2_body:
		mov	al, [si]
		cmp	ax, [bp+solid]
		jz	csp_loop2_condition
		mov	es:[di], al

csp_loop2_condition:
		inc	di
		dec	si
		loop	csp_loop2_body
		pop	di
		pop	si
		add	si, [bp+src_width]
		add	di, [bp+dst_width]
		inc	dx

csp_loop1_condition:
		cmp	dx, [bp+height]
		jl	csp_loop1_body

csp_loop_leave:
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_copy_solid_permuted endp


_copy_solid	proc far

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
		jmp	cs_loop1_condition

cs_loop1_body:
		push	si
		push	di
		mov	cx, [bp+width_to_copy]

cs_loop2_body:
		mov	al, [si]
		cmp	ax, [bp+solid]
		jz	cs_loop2_condition
		mov	es:[di], al

cs_loop2_condition:
		inc	di
		inc	si
		loop	cs_loop2_body

		pop	di
		pop	si
		add	si, [bp+src_width]
		add	di, [bp+dst_width]
		inc	dx

cs_loop1_condition:
		cmp	dx, [bp+height]
		jl	cs_loop1_body

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_copy_solid	endp


_swap_u16	proc far

val		= word	ptr  6

		push	bp
		mov	bp, sp

		push	bx
		mov	bx, [bp+val]
		mov	ax, 0
		mov	al, bh
		mov	ah, bl
		pop	bx

		mov	sp, bp
		pop	bp
		retf

_swap_u16	endp


_copy_to_screen		proc far

v2		= word ptr -4
v1		= word ptr -2
src		= dword	ptr 6
dst		= dword	ptr 0Ah
p_width		= word ptr  0Eh
height		= word ptr  10h
mode		= word ptr  12h

		push	bp
		mov	bp, sp
		sub	sp, 4

		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		lds	si, [bp+src]
		mov	dx, [bp+height]
		mov	ax, 320
		sub	ax, [bp+p_width]
		mov	[bp+v1], ax
		mov	[bp+v2], ax

		mov	ax, [bp+mode]
		and	ax, 07fh
		cmp	ax, 0
		jnz	cts_if_1
		mov	[bp+v2], 0
		jmp	cts_if_exit
cts_if_1:
		cmp	ax, 2
		jnz	cts_if_exit
		mov	[bp+v1], 0
cts_if_exit:

		mov	ax, [bp+mode]
		and	ax, 80h
		jz	cts_loop1_condition
cts_loop1_body:
		mov	cx, [bp+p_width]
cts_loop2_body:
		mov	al, [si]
		cmp	al, 0
		jz	cts_loop2_condition
		mov	es:[di], al

cts_loop2_condition:
		inc	di
		inc	si
		loop	cts_loop2_body
		add	di, [bp+v1]
		add	si, [bp+v2]
		dec	dx
		jg	cts_loop1_body
		jmp	cts_leave

cts_loop1_condition:
		mov	cx, [bp+p_width]
		rep	movsb
		add	di, [bp+v1]
		add	si, [bp+v2]
		dec	dx
		jg	cts_loop1_condition
cts_leave:
		pop	si
		pop	ds
		pop	di
		pop	es

		mov	sp, bp
		pop	bp
		retf
_copy_to_screen	endp


unused_func13	proc far

v1		= word ptr -2
dst		= dword	ptr 6
a_width		= word	ptr 0Ah
height		= word ptr  0Ch
color		= byte ptr  0Eh

		push bp
		mov bp, sp
		sub sp, 2
		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		mov	dx, [bp+height]
		mov	ax, 320
		sub	ax, [bp+a_width]
		mov	[bp+v1], ax
		cld

u13_loop:
		mov	al, [bp+color]
		mov	cx, [bp+a_width]
		rep	stosb
		add	di, [bp+v1]
		dec	dx
		jg	u13_loop

		pop	si
		pop	ds
		pop	di
		pop	es

		mov	sp, bp
		pop	bp
		retf

unused_func13	endp


_normalize_ptr	proc far

pnt		= dword ptr 6

		push bp
		mov bp, sp
		push ds
		push es
		push si
		push di

		xor	dx, dx
		mov	ax, word ptr [bp+pnt+2]
		mov	cx, 4
		call	far ptr F_LXLSH@
		mov 	word ptr [bp+pnt+2], 0
		add	ax, word ptr [bp+pnt]
		adc	dx, word ptr [bp+pnt+2]
		mov	cx, 0ch
		call	far ptr F_LXLSH@
		mov	ax, word ptr [bp+pnt]
		and	ax, 0fh

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_normalize_ptr	endp

		end
