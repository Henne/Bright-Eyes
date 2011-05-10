#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "regs.h"
#include "paging.h"
#include "../../../dos/drives.h"

#include "../schick.h"

#include "g105de_seg000.h"
#include "g105de_seg001.h"
#include "g105de_seg002.h"
#include "g105de_seg003.h"
#include "g105de_seg005.h"

#include "../rewrite_m302de/seg002.h"
#include "../rewrite_m302de/seg008.h"


static FILE * fd_open_datfile(Bit16u);
static Bit16u fd_read_datfile(FILE * fd, Bit8u *buf, Bit16u len);

bool G105de::load_file(Bit16u index)
{
	FILE *fd;

	fd = fd_open_datfile(index);

	if (fd == NULL)
		return false;

	fread(MemBase + Real2Phys(ds_readd(0x3f46)), 1, 32767, fd);
	fclose(fd);

	return true;
}

/* static */
void draw_mouse_ptr_wrapper() {

	draw_mouse_ptr();
}

/* static */
void call_mouse()
{
	mouse();
}

/* static */
void draw_mouse_ptr()
{
	if (ds_readw(0x1248))
		return;

	if (ds_readw(0x124a) == 0) {
		ds_writew(0x1248, 1);
		do_draw_mouse_ptr();
		ds_writew(0x1248, 0);
	}
	ds_writew(0x124a, ds_readw(0x124a) - 1);
}

/* static */
void mouse()
{
	if (ds_readw(0x1248))
		return;

	ds_writew(0x124a, ds_readw(0x124a) + 1);

	if (ds_readw(0x124a))
		return;

	ds_writew(0x1248, 1);

	if (ds_readw(0x124c) < ds_readw(0x1256))
		ds_writew(0x124c, ds_readw(0x1256));

	if (ds_readw(0x124c) > 315)
		ds_writew(0x124c, 315);

	if (ds_readw(0x124e) < ds_readw(0x1258))
		ds_writew(0x124e, ds_readw(0x1258));

	if (ds_readw(0x124e) > 195)
		ds_writew(0x124e, 195);

	save_mouse_ptr();

	ds_writew(0x1250, ds_readw(0x124c));
	ds_writew(0x1252, ds_readw(0x124e));
	ds_writew(0x125a, ds_readw(0x1256));
	ds_writew(0x125c, ds_readw(0x1258));

	update_mouse_ptr();

	ds_writew(0x1248, 0);
}

/* static */
void G105de::mouse_compare()
{
	/* these pointers never differ in gen */
	if (ds_readw(0x1254) || ds_readd(0x4621) != ds_readd(0x4625)) {

		/* copy a pointer */
		ds_writed(0x4621, ds_readd(0x4625));

		if (PhysMake(datseg, 0x1200) == Real2Phys(ds_readd(0x4625))) {
			ds_writew(0x1258, 0);
			ds_writew(0x1256, 0);
		} else {
			ds_writew(0x1258, 8);
			ds_writew(0x1256, 8);
		}
		ds_writew(0x1254, 0);
		draw_mouse_ptr();
		mouse();
	}
}

/* static */
Bit16u G105de::get_mouse_action(Bit16u x, Bit16u y, Bit8u *ptr)
{

	Bit16u i;

	for (i = 0; host_readw(ptr + i * 10) != 0xffff; i++) {

		if (host_readw(ptr + i * 10 ) > x)
			continue;
		if (host_readw(ptr + i * 10 + 4) < x)
			continue;
		if (host_readw(ptr + i * 10 + 2) > y)
			continue;
		if (host_readw(ptr + i * 10 + 6) < y)
			continue;

		return host_readw(ptr + i * 10 + 8);
	}

	return 0;
}

/**
 * decomp_rle() - decompress a RLE compressed picture
 * @dst:	destination
 * @src:	source
 * @y:		y - Coordinate to start
 * @x:		x - Coordinate to start
 * @width:	width of the picture
 * @height:	height of the picture
 * @mode:	if 2 copy pixels with the value 0
 *
*/

void G105de::decomp_rle(Bit8u *dst, Bit8u *src, Bit16u y, Bit16u x,
				Bit16u width, Bit16u height, Bit16u mode)
{
	Bit8u *dst_loc;
	Bit16u i, j, n, k;
	unsigned char val, pix;

	/*
	 * In Gen V1.05_de this function can an will be tweaked,
	 * since the only argument which differs is src.
	 *
	 * To help the compiler optimizing, we set the arguments to the values,
	 * we know they will have, so they can be threaten as constants.
	 *
	 * With GCC 4.4.5 and the default DOSBox settings (-O2),
	 * the code of this function shrinked:
	 * - on my x86_64 machine from 253 to 156 byte (62%).
	 * - on my x86_32 machine from 276 to 180 byte (65%).
	 */

	y = x = 0;
	width = 320;
	height = 200;
	mode = 0;

	/* End of the tweaker section. */

	dst_loc = dst + 320 * y + x;
	draw_mouse_ptr_wrapper();

	for (i = 0; i < height; dst_loc += 320, i++) {

		j = 0;

		while (j < width) {

			val = host_readb(src++);

			if (val == 0x7f) {
				n = host_readb(src++);
				pix = host_readb(src++);

				if (pix != 0 || mode != 2)
					for (k = 0; k <= n; k++)
						host_writeb(dst_loc + j + k, pix);
				j += n;
			} else {
				if (val != 0 || mode != 2)
					host_writeb(dst_loc + j, val);
				j++;
			}
		}
	}

	call_mouse();
}

/* static */
void update_mouse_ptr()
{
	PhysPt p1, p2;
	Bit16u v1, v2, v3, si, di;
	Bit8s i, j;

	p1 = Real2Phys(ds_readd(0x47cb));

	p2 = Real2Phys(ds_readd(0x4625)) + 32;

	di = ds_readw(0x124c) - ds_readw(0x1256);

	v1 = ds_readw(0x124e) - ds_readw(0x1258);

	v2 = v3 = 16;

	if (di > 304)
		v2 = 320 - di;

	if (v1 > 184)
		v3 = 200 - v1;

	p1 += v1 * 320 + di;

	for (i = 0; i < v3; p1 += 320, i++) {

		si = mem_readw_inline(p2);
		p2 += 2;

		for (j = 0; j < v2; j++)
			if ((0x8000 >> j) & si)
				mem_writeb_inline(p1 + j, 0xff);

	}
}

/* static */
void save_mouse_ptr()
{
	PhysPt p;
	Bit16u v6, v8, va, si, di, cx;

	p = Real2Phys(ds_readd(0x47cb));

	/* X - Coordinate */
	di = ds_readw(0x124c) - ds_readw(0x1256);
	/* Y - Coordinate */
	v6 = ds_readw(0x124e) - ds_readw(0x1258);

	va = v8 = 16;

	if (di > 304)
		v8 = 320 - di;

	if (v6 > 184)
		va = 200 - v6;

	p += v6 *320 + di;

	for (si = 0; si < va; p += 320, si++)
		for (cx = 0; cx < v8; cx++)
			ds_writeb(0x4669 + si * 16 + cx,
				mem_readb_inline(p + cx));
}

/* static */
void do_draw_mouse_ptr()
{
	PhysPt ptr;
	Bit16u pos_x, pos_y;
	Bit16u d_x, d_y;
	Bit16u i, j;

	ptr = Real2Phys(ds_readd(0x47cb));

	pos_x = ds_readw(0x1250) - ds_readw(0x125a);
	pos_y = ds_readw(0x1252) - ds_readw(0x125c);
	d_x = d_y = 16;

	if (pos_x > 304)
		d_x = 320 - pos_x;
	if (pos_y > 184)
		d_y = 200 - pos_y;

	ptr += pos_y * 320 + pos_x;

	for (i = 0; i < d_y; ptr += 320, i++)
		for (j = 0; j < d_x; j++)
			mem_writeb_inline(ptr + j,
				ds_readb(0x4669 + i * 16 + j));
}

void G105de::load_font_and_text()
{
	FILE *fd;
	Bit32u len;

	fd = fd_open_datfile(0x0e);
	fd_read_datfile(fd, MemBase + Real2Phys(ds_readd(0x4779)), 1000);
	fclose(fd);

	fd = fd_open_datfile(0x0f);
	len = fd_read_datfile(fd, MemBase + Real2Phys(ds_readd(0x4775)), 64000);
	fclose(fd);

	G105de::split_textbuffer(MemBase + PhysMake(datseg, 0x40d9),
			ds_readd(0x4775), len);

}

void G105de::split_textbuffer(Bit8u *dst, RealPt src, Bit32u len)
{
	Bit32u i = 0;

	host_writed(dst, src);
	dst += 4;

	for (; i != len; src++, i++) {
		/* continue if not the ned of the string */
		if (mem_readb(Real2Phys(src)) != 0)
			continue;
		/* return if "\0\0" (never happens) */
		if (mem_readb(Real2Phys(src) + 1) == 0)
			return;
		/* write the adress of the next string */
		host_writed(dst, src + 1);
		dst += 4;
	}
}

static void prepare_path(char *p)
{
	while (*p) {
#ifdef __WIN32__
		if (*p == '/')
			*p = '\\';
#else
		if (*p == '\\')
			*p = '/';
#endif
		p++;
	}
}

static FILE * fd_open_datfile(Bit16u index)
{
	FILE *fd;
	char *pwd;
	signed long offset;
	Bit8u buf[800];
	char fname[800];

	/* build the path to DSAGEN.DAT */
	Bit8u drive = DOS_GetDefaultDrive();

	localDrive *dr = dynamic_cast<localDrive*>(Drives[drive]);

	dr->GetSystemFilename((char*)buf, "");
	sprintf(fname, "%s/%s/DSAGEN.DAT", (char*)buf,	Drives[drive]->curdir);
	prepare_path(fname);

	fd = fopen(fname, "rb");

	if (fd == NULL) {
		D1_ERR("%s(): failed to open datafile at %s\n",
			__func__, fname);
		return NULL;
	}

	if (fread(buf, 1, 800, fd) != 800) {
		D1_ERR("%s(): failed to read datafile\n");
		fclose(fd);
		return NULL;
	}


	offset = get_archive_offset(MemBase + Real2Phys(ds_readd(0x1ad1 + index * 4)), buf);
	ds_writew(0x3f36, offset);


	if (offset == -1) {
		D1_ERR("%s(): file not found in archive\n", __func__);
		fclose(fd);
		return NULL;
	}

	fseek(fd, offset, SEEK_SET);

	return fd;

}

static Bit16u fd_read_datfile(FILE * fd, Bit8u *buf, Bit16u len)
{

	if (len > ds_readd(0x3f2e))
		len = ds_readw(0x3f2e);

	len = fread(buf, 1, len, fd);

	ds_writed(0x3f2e, ds_readd(0x3f2e) - len);

	return len;
}

/* static */
Bit32s get_archive_offset(Bit8u *name, Bit8u *table) {

	Bit32s offset, length;
	Bit16u i;

	for (i = 0; i < 50; i++) {

		/* check the filename */
		if (strncmp((char*)name, (char*)table + i * 16, 12))
			continue;

		/* calculate offset and length */
		offset = host_readd(table + i * 16 + 0x0c);
		length = host_readd(table + (i + 1) * 16 + 0x0c) - offset;

		/* save length in 2 variables */
		ds_writed(0x3f2a, length);
		ds_writed(0x3f2e, length);

		return offset;
	}

	return -1;
}

namespace G105de {

Bit16u read_datfile(Bit16u handle, Bit8u *buf, Bit16u len)
{

	if (len > ds_readd(0x3f2e))
		len = ds_readw(0x3f2e);

	len = bc__read(handle, buf, len);

	ds_writed(0x3f2e, ds_readd(0x3f2e) - len);

	return len;
}

}

Bit32s get_filelength() {

	return ds_readd(0x3f2a);
}

Bit16u ret_zero1() {
	return 0;
}

void G105de::wait_for_keypress()
{
	while (G105de::CD_bioskey(1)) {
		G105de::CD_bioskey(0);
	}
}

Bit32u swap32(Bit16u v1, Bit16u v2) {
	return (swap_u16(v2) << 16 | swap_u16(v1));
}

void init_video()
{
	/* set the video mode to 320x200 8bit */
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x1b79), 0xff);
}

void exit_video()
{
	/* restore old mode */
	set_video_mode(ds_readw(0x47dd));
	/* restore old page */
	set_video_page(ds_readw(0x47db));
}

void draw_v_line(Bit16u x, Bit16u y1, Bit16u y2, Bit16u color)
{
	Bit16u tmp, len, off;

	if (y1 > y2) {
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	len = y2 - y1 + 1;
	off = y1 * 320 + x;

	draw_h_spaced_dots(PhysMake(0xa000, off), len, color, 320);
}

void G105de::do_draw_pic(Bit16u mode)
{
	Bit16u x, y, d1, d2, v1, v2;
	Bit16u d3, d4, w, h;
	PhysPt dst, src;

	x = ds_readw(0x40c5);
	y = ds_readw(0x40c7);
	d1 = ds_readw(0x40c9);
	d2 = ds_readw(0x40cb);
	v1 = ds_readw(0x40d1);
	v2 = ds_readw(0x40d3);
	d3 = ds_readw(0x40d5);
	d4 = ds_readw(0x40d7);

	w = d1 - x + 1;
	h = d2 - y + 1;

	src = Real2Phys(ds_readd(0x40cd));
	dst = Real2Phys(ds_readd(0x40c1));

	draw_mouse_ptr_wrapper();

	draw_pic(dst, x, y, d1, d2, v1, v2, d3, d4, w, h, src, mode);

	call_mouse();


}

void call_fill_rect_gen(PhysPt ptr, Bit16u x1, Bit16u y1, Bit16u x2, Bit16u y2, Bit16u color)
{
	fill_rect(ptr + y1 * 320 + x1, color, x2 - x1 + 1, y2 - y1 + 1);
}

/* static */
void blit_smth3(PhysPt ptr, Bit16u v1, Bit16u v2) {

	Bit8u *src;
	Bit16u i, j;

	src = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < v1; src += 8 - v2, ptr += 320, i++)
		for (j = 0; j < v2; src++, j++)
			mem_writeb_inline(ptr + j, host_readb(src));
}

/* static */
Bit16u print_line(char *str)
{
	Bit16u lines = 1;

	draw_mouse_ptr_wrapper();

	lines = str_splitter(str);

	print_str(str, ds_readw(0x4791), ds_readw(0x478f));

	call_mouse();

	return lines;
}

/* static */
void print_str(char *str, Bit16u x, Bit16u y)
{
	Bit16u i, x_bak;
	unsigned char c;

	i = 0;

	draw_mouse_ptr_wrapper();

	if (ds_readw(0x4789) == 1)
		x = get_line_start_c(str, x, ds_readw(0x478d));

	x_bak = x;

	while (c = str[i++]) {
		switch (c) {
			case 0x0d: case 0x40: {
				/* newline */
				y += 7;

				if (ds_readw(0x4789) == 1)
					x = get_line_start_c(str + i,
						ds_readw(0x4791),
						ds_readw(0x478d));
				else
					x = x_bak;
				break;
			}
			case 0x7e: {
				/* CRUFT */
				if (x < ds_readw(0x47e3)) {
					x = ds_readw(0x47e3);
					continue;
				}
				if (x < ds_readw(0x47e5)) {
					x = ds_readw(0x47e5);
					continue;
				}
				if (x < ds_readw(0x47e7)) {
					x = ds_readw(0x47e7);
					continue;
				}
				if (x < ds_readw(0x47e9)) {
					x = ds_readw(0x47e9);
					continue;
				}
				if (x < ds_readw(0x47eb)) {
					x = ds_readw(0x47eb);
					continue;
				}
				if (x < ds_readw(0x47ed)) {
					x = ds_readw(0x47ed);
					continue;
				}
				if (x < ds_readw(0x47ef)) {
					x = ds_readw(0x47ef);
					continue;
				}
				break;
		}
		case 0xf0: case 0xf1: case 0xf2: case 0xf3: {
			/* change text color */
			ds_writew(0x477d, c - 0xf0);
			break;
		}
		default:
			/* print normal */
			x += print_chr(c, x, y);
		}
	}

	call_mouse();
}

/**
 * str_splitter() - sets the line breaks for a string
 * @s:	string
 *
 * Returns the number of lines the string needs.
 */
/* static */
Bit16u str_splitter(char *s) {

	char *tp;
	Bit16u last_space, lines, c_width, l_width, l_width_max;
	Bit16s i;

	if (s == NULL || s == (char*)MemBase)
		return 0;

	lines = 1;
	l_width_max = ds_readw(0x478d);


	/* replace all CR and LF with spaces */
	for (tp = s; *tp; tp++)
		if (*tp == 0x0d || *tp == 0x0a)
			*tp = 0x20;

	tp = s;

	l_width = i = last_space = 0;

	while (tp[i]) {
		get_chr_info(tp[i], (Bit8u*)&c_width);
		l_width += c_width;

		if (l_width >= l_width_max) {

			if (last_space) {
				tp[last_space] = 0xd;
				tp += last_space;
			} else {
				tp[i] = 0xd;
				tp += i + 1;
			}

			lines++;
			l_width = last_space = i = 0;
		}

		/* remember the last i in last_space */
		if (tp[i] == 0x20)
			last_space = i;

		if (tp[i] == 0x40) {
			tp += i + 1;
			i = -1;
			l_width = last_space = 0;
			lines++;
		}
		i++;
	}

	if (l_width >= l_width_max)
		if (last_space) {
			tp[last_space] = 0xd;
			lines++;
		} else
			tp[i - 1] = 0;

	return lines;
}

Bit16u print_chr(unsigned char c, Bit16u x, Bit16u y) {

	Bit16u width, idx;

	idx = get_chr_info(c, (Bit8u*)&width);

	call_them_all(idx, width, x, y);

	return width;
}

/**
 * get_chr_info() - gets font information of a character
 * @c:		the character
 * @width:	pointer to save width
 *
 * Returns the font index.
 */
Bit8u get_chr_info(unsigned char c, Bit8u *ptr) {

	Bit16u i;

	for (i = 0; i != 222; i += 3) {
		/* search for the character */
		if (ds_readb(0x1b85 + i) != c)
			continue;

		host_writew(ptr, ds_readb(0x1b85 + 2 + i));
		return ds_readb(0x1b85 + 1 + i);
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew(ptr, 0);
		return 0;
	} else {
		host_writew(ptr, 6);
		return 0;
	}
}

/* static */
void call_them_all(Bit16u v1, Bit16u v2, Bit16u x, Bit16u y) {

	PhysPt gfx_ptr;
	Bit32u bogus;

	fill_smth();
	fill_smth2(MemBase + Real2Phys(ds_readd(0x4779) + v1 * 8));

	gfx_ptr = get_gfx_ptr(x, y);
	bogus = ret_zero();

	call_blit_smth3(Real2Phys(gfx_ptr), 7, 0, 0, v2);
}

/* static */
void fill_smth() {

	Bit8u *ptr;
	Bit16u i, j;

	if (ds_readb(0x109a))
		ptr = MemBase + PhysMake(datseg, 0x45a1);
	else
		ptr = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, ptr += 8)
		for (j = 0; j < 8; j++)
			host_writeb(ptr + j, ds_readb(0x477f));
}

/* static */
void fill_smth2(Bit8u* ptr) {

	Bit8u *lp;
	Bit16u i, j;
	Bit8u lv;

	if (ds_readb(0x109a))
		lp = MemBase + PhysMake(datseg, 0x45a1);
	else
		lp = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, lp += 8) {
		lv = host_readb(ptr);
		ptr++;
		for (j = 0; j < 8; j++) {
			if (!((0x80 >> j) & lv))
				continue;

			host_writeb(lp + j,
				ds_readb(0x4781 + ds_readw(0x477d) * 2));
		}
	}
}

/* static */
RealPt get_gfx_ptr(Bit16u x, Bit16u y) {

	RealPt start;

	start = ds_readd(0x47c7) + 320 * y + x;

	return start;
}

/* static */
Bit16u ret_zero() {
	return 0;
}

/* static */
void call_blit_smth3(PhysPt dst, Bit16u v1, Bit16u v2, Bit16u v3, Bit16u v4) {
	blit_smth3(dst, v1, v4);
}

/* static */
void set_vals(Bit16u v1, Bit16u v2) {
	ds_writew(0x4781, v1);
	ds_writew(0x477f, v2);
}

/* static */
void get_vals(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, ds_readw(0x4781));
	host_writew(p2, ds_readw(0x477f));
}

Bit16u get_str_width(char *str) {

	Bit16u width, sum = 0;

	while (*str) {
		get_chr_info(*str++, (Bit8u*)&width);
		sum += width;
	}

	return sum;
}

/**
 * get_line_start_c() - calculates the start positon for a centered line
 * @str:	the string
 * @x:		start position of the string
 * @x_max:	end position of the string
 *
 * Returns the X coordinate where the strin must start.
 */
Bit16u get_line_start_c(char *str, Bit16u x, Bit16u x_max) {

	Bit16u width, sum = 0;

	while (*str && *str != 0x40 && *str != 0x0d) {
		get_chr_info(*str++, (Bit8u*)&width);
		sum += width;
	}
	return (x_max - sum) / 2 + x ;
}

Bit16u enter_string(char *dst, Bit16u x, Bit16u y, Bit16u num, Bit16u zero)
{
	Bit16u pos, l3, di, si;
	Bit16s c;

	draw_mouse_ptr_wrapper();
	di = x;
	pos = 0;

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			print_chr(0x20, di, y);
			print_chr(0x5f, di, y);
			di += 6;
		}
		di = x;
	} else {
		print_chr(0x20, di, y);
		print_chr(0x5f, di, y);
	}
	G105de::wait_for_keypress();
	ds_writew(0x4597, 0);

	c = 0;
	while (c != 0xd || pos == 0) {
		do {
			do {} while (G105de::CD_bioskey(1) == 0 &&
				ds_readw(0x4597) == 0);

			if (ds_readw(0x4597)) {
				ds_writew(0x459d, 0xd);
				ds_writew(0x459b, 0);
				ds_writew(0x4597, 0);
			} else {
				ds_writew(0x459d, G105de::CD_bioskey(0));
				ds_writew(0x459f, ds_readw(0x459d) >> 8);
				ds_writew(0x459d, ds_readw(0x459d) & 0xff);
			}
		} while (ds_readw(0x459f) == 0 && ds_readw(0x459d) == 0);

		c = ds_readw(0x459d);

		if (c == 0xd)
			continue;

		if (ds_readw(0x459f) == 1) {
			*dst = 0;
			call_mouse();
			ds_writew(0x459f, 0);
			return 1;
		}
		if (c == 8) {
			if (pos <= 0)
				continue;

			if (zero == 1 && pos != num)
				print_chr(0x20, di, y);
			pos--;
			dst--;
			get_chr_info(*dst, (Bit8u*)&l3);

			if (zero == 0)
				di -= 6;
			else
				di -= l3;
		} else {
			if (!(ds_readb(0x1ff9 + c) & 0x0e) &&
				(c != 0x84 & 0xff) && (c != 0x94 & 0xff) &&
				(c != 0x81 & 0xff) && (c != 0x8e & 0xff) &&
				(c != 0x99 & 0xff) && (c != 0x9a & 0xff) &&
				(c != 0x20) && (c != 0x2e))
					continue;

			/* is_alpha(c) */
			if (ds_readb(0x1ff9 + c) & 0xc)
				c = toupper(c);

			/* ae */
			if (c == 0x84)
				c = 0xff8e;
			/* oe */
			if (c == 0x94)
				c = 0xff99;
			/* ue */
			if (c == 0x81)
				c = 0xff9a;

			/* are we at the end of the input field */
			if (pos == num) {
				dst--;
				get_chr_info(*dst, (Bit8u*)&l3);

				if (zero != 0)
					di -= l3;
				else
					di -= 6;

				pos--;
			}

			*dst++ = c & 0xff;
			print_chr(0x20, di, y);
			print_chr(c & 0xff, di, y);
			get_chr_info(c & 0xff, (Bit8u*)&l3);

			if (zero != 0)
				di += l3;
			else
				di += 6;

			pos++;

			if (zero != 1 || pos == num)
				continue;
		}
		print_chr(0x20, di, y);
		print_chr(0x5f, di, y);
	}

	if (zero == 0)
		while (pos < num) {
			print_chr(0x20, di, y);
			di += 6;
			pos++;
		}

	*dst = 0;
	call_mouse();
	return 0;
}

namespace G105de {

void draw_popup_line(Bit16u line, Bit16u type)
{
	PhysPt dst, src;
	Bit16u i, popup_right, popup_left, popup_middle;

	/* This is a bit bogus */
	dst = Real2Phys(ds_readd(0x47cb));

	/* (line * 8 + y) * 320  + x */
	dst = ((line * 8) + ds_readw(0x40bd)) * 320 + Real2Phys(ds_readd(0x47cb)) + ds_readw(0x40bb);

	switch (type) {
		case 0: {
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		}
		case 1: {
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 2: {
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 3: {
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
		}
	}

	src = Real2Phys(ds_readd(0x476d)) + popup_left;
	copy_to_screen(src, dst, 16, 8, 0);

	src = Real2Phys(ds_readd(0x476d)) + popup_middle;
	dst += 16;
	for (i = 0; i < ds_readw(0x40b9); dst += 32, i++)
		copy_to_screen(src, dst, 32, 8, 0);

	src = Real2Phys(ds_readd(0x476d)) + popup_right;
	copy_to_screen(src, dst, 16, 8, 0);
}

}

/**
 * fill_radio_button() - marks the active radio button
 * @old_pos:	the position of the last active button (or -1)
 * @new_pos:	the position of the current active button
 * @offset:	the offset of the first radio line
 *
 */
void G105de::fill_radio_button(Bit16s old_pos, Bit16u new_pos, Bit16u offset)
{
	Bit16u i, x, y;

	draw_mouse_ptr_wrapper();

	/* unmark the old radio button, if any */
	if (old_pos != -1) {
		y = ds_readw(0x40bb) + 6;

		x = (offset + old_pos) * 8 + ds_readw(0x40bd) + 2;

		for (i = 0; i < 4; i++)
			draw_v_line(y + i, x, x + 3, 0xd8);
	}

	/* mark the new radio button */
	y = ds_readw(0x40bb) + 6;

	x = (offset + new_pos) * 8 + ds_readw(0x40bd) + 2;

	for (i = 0; i < 4; i++)
		draw_v_line(y + i, x, x + 3, 0xd9);

	call_mouse();
}

/**
 * enter_name() - enter the name of a hero
 */
void enter_name()
{
	PhysPt dst;

	dst = Real2Phys(ds_readd(0x47cb) + 12 * 320 + 176);

	draw_mouse_ptr_wrapper();
	G105de::copy_to_screen(Real2Phys(ds_readd(0x479f)), dst, 94, 8, 0);
	enter_string((char*)MemBase + PhysMake(datseg, 0x132c), 180, 12, 15, 1);
	G105de::copy_to_screen(Real2Phys(ds_readd(0x479f)), dst, 94, 8, 0);
	call_mouse();
	print_str((char*)MemBase + PhysMake(datseg, 0x132c), 180, 12);
}


void G105de::change_head()
{
	char nvf[19];
	Bit8u *n = (Bit8u*)nvf;

	host_writed(n + 0, ds_readd(0x47a3));

	host_writed(n + 4, ds_readd(0x4771));

	host_writew(n + 8, ds_readb(0x40b6));

	host_writew(n + 10, 0);

	host_writed(n + 11, RealMake(SegValue(ss), reg_sp - 8));
	host_writed(n + 15, RealMake(SegValue(ss), reg_sp - 10));

	process_nvf(n);

	ds_writed(0x40cd, ds_readd(0x47a3));

	ds_writew(0x40c5, 272);
	ds_writew(0x40c9, 303);

	if (ds_readw(0x1324) == 0) {
		ds_writew(0x40c7, 8);
		ds_writew(0x40cb, 39);
		do_draw_pic(0);
	} else if (ds_readw(0x1324) > 4) {
		ds_writew(0x40c7, 4);
		ds_writew(0x40cb, 35);
		ds_writew(0x40c7, 8);
		ds_writew(0x40cb, 39);
		do_draw_pic(0);
	}
}

/**
 * change_sex() - changes the sex of the hero
 *
 */
void G105de::change_sex()
{
	PhysPt dst, src;
	signed char tmp;

	/* change sex of the hero */
	ds_writeb(0x134e, ds_readb(0x134e) ^ 1);

	/* hero has a typus */
	if (ds_readb(0x134d)) {
		if (ds_readb(0x134e) != 0) {
			/* To female */
			tmp = ds_readb(0x1054 + (signed char)ds_readb(0x40b7));

			ds_writeb(0x40b6, tmp);
			ds_writeb(0x40b5, tmp);
			ds_writeb(0x40b4, ds_readb(0x1049 + (signed char)ds_readb(0x40b7)) - 1);
		} else {
			/* To male */
			tmp = ds_readb(0x1048 + (signed char)ds_readb(0x40b7));
			ds_writeb(0x40b6, tmp);
			ds_writeb(0x40b5, tmp);
			ds_writeb(0x40b4, ds_readb(0x1054 + (signed char)ds_readb(0x40b7)) -1);
		}
		ds_writew(0x11fe, 1);
		return;
	} else {
		dst = Real2Phys(ds_readd(0x47cb)) + 7 * 320 + 305;
		src = Real2Phys(ds_readd(0x4769)) + ds_readb(0x134e) * 256;
		draw_mouse_ptr_wrapper();
		G105de::copy_to_screen(src, dst, 16, 16, 0);
		call_mouse();
	}
}

/**
 * calc_at_pa() - calculate AT and PA values
 */
/* static */
void calc_at_pa() {
	Bit16u i;
	Bit16s tmp, base;

	/* base = (GE + IN + KK) / 5 */
	tmp = (signed char)ds_readb(0x136f);
	tmp += (signed char)ds_readb(0x1372);
	tmp += (signed char)ds_readb(0x136c);

	base = tmp / 5;

	/* round up if neccessary */
	if ((tmp % 5) >= 3)
		base++;

	/* save AT/PA base value */
	ds_writeb(0x1393, base);

	for (i = 0; i < 7; i++) {
		/* set the weapon values to base */
		ds_writeb(0x139b + i, base);
		ds_writeb(0x1394 + i, base);

		if ((signed char)ds_readb(0x1434 + i) < 0) {
			/* calculate ATPA for negative weapon skill */
			tmp = abs((signed char)ds_readb(0x1434 + i)) / 2;

			/* sub skill / 2 from AT */
			ds_writeb(0x1394 + i, ds_readb(0x1394 + i) - tmp);

			/* sub skill / 2 from PA */
			ds_writeb(0x139b + i, ds_readb(0x139b + i) - tmp);

			/* if skill % 2, then decrement PA */
			if (abs((signed char)ds_readb(0x1434 + i)) != tmp * 2)
				ds_writeb(0x139b + i, ds_readb(0x139b + i) - 1);
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = abs((signed char)ds_readb(0x1434 + i)) / 2;

			/* add skill / 2 to AT */
			ds_writeb(0x1394 + i, ds_readb(0x1394 + i) + tmp);

			/* add skill / 2 to PA */
			ds_writeb(0x139b + i, ds_readb(0x139b + i) + tmp);

			/* if skill % 2, then increment AT */
			if (ds_readb(0x1434 + i) != tmp * 2)
				ds_writeb(0x1394 + i, ds_readb(0x1394 + i) + 1);
		}

	}
}

/* static */
void clear_hero() {

	Bit16u i;

	ds_writew(0x2780, 0);
	ds_writew(0x2782, 0);

	ds_writeb(0x40b6, 0);
	ds_writeb(0x40b4, 0);
	ds_writeb(0x40b5, 0);
	ds_writeb(0x40b7, 0);

	for (i = 0; i < 14; i++)
		ds_writeb(0x4076 + i, 0);

	for (i = 0; i < 86; i++) {
		ds_writeb(0x3f63 + i * 2, 0);
		ds_writeb(0x3f62 + i * 2, 0);
	}
	for (i = 0; i < 52; i++) {
		ds_writeb(0x400f + i * 2, 0);
		ds_writeb(0x400e + i * 2, 0);
	}

	ds_writeb(0x1353, 1);
};

/**
 * skill_inc_novice() - tries to increment a skill in novice mode
 * @skill:	the skill which should be incremented
 *
 *
 */
void skill_inc_novice(Bit16u skill)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readw(0x400e + skill * 2) == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* decrement counter for skill increments */
		ds_writeb(0x1468, ds_readb(0x1468) - 1);

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > (signed char)ds_readb(0x1434 + skill)) {
			/* increment skill */
			ds_writeb(0x1434 + skill, ds_readb(0x1434 + skill) + 1);

			/* set inc tries for this skill to zero */
			ds_writew(0x400e + skill * 2, 0);

			/* set the flag to leave this loop */
			done++;

			if (skill > 6)
				continue;

			/* set increment the lower AT/PA value */
			if (ds_readb(0x1394 + skill) > ds_readb(0x139b + skill))
				ds_writeb(0x139b + skill, ds_readb(0x139b + skill) + 1);
			else
				ds_writeb(0x1394 + skill, ds_readb(0x1394 + skill) + 1);
		} else
			/* inc tries for that skill */
			ds_writew(0x400e + skill * 2, ds_readw(0x400e + skill * 2) + 1);
	}
}

/**
 * spell_inc_novice() - tries to increment a spell in novice mode
 * @spell:	the spell which should be incremented
 *
 *
 */
void spell_inc_novice(Bit16u spell)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readw(0x3f62 + spell * 2) == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* decrement counter for spell increments */
		ds_writeb(0x14bf, ds_readb(0x14bf) - 1);

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > (signed char)ds_readb(0x1469 + spell)) {
			/* increment spell */
			ds_writeb(0x1469 + spell, ds_readb(0x1469 + spell) + 1);

			/* set inc tries for this spell to zero */
			ds_writew(0x3f62 + spell * 2, 0);

			/* set the flag to leave this loop */
			done++;
		} else
			ds_writeb(0x3f62 + spell * 2, ds_readb(0x3f62 + spell * 2) + 1);
	}
}

#define INC (1)
#define DEC (2)
/**
 * can_change_attribs() - checks if attribute changes are possible
 *
 */
Bit16u G105de::can_change_attribs()
{
	Bit8u *p;
	Bit16u i;
	Bit16s pa_inc, pa_dec, na_inc, na_dec;

	na_dec = na_inc = pa_dec = pa_inc = 0;

	for (i = 0; i < 14; i++)
		D1_LOG("%d ", ds_readb(i + 0x4076));
	D1_LOG("\n");


	for (i = 0; i < 7; i++) {
		p = p_datseg + 0x1360 + i * 3;

		if (ds_readb(i + 0x4076) != INC && host_readb(p) > 8)
			pa_dec += 8 - (signed char)host_readb(p);
		if (ds_readb(i + 0x4076) != DEC && host_readb(p) < 13)
			pa_inc += 13 - (signed char)host_readb(p);
	}

	for (i = 7; i < 14; i++) {
		p = p_datseg + 0x1360 + i * 3;

		if (ds_readb(i + 0x4076) != INC && host_readb(p) > 2)
			na_dec += 2 - (signed char)host_readb(p);
		if (ds_readb(i + 0x4076) != DEC && host_readb(p) < 8)
			na_inc += 8 - (signed char)host_readb(p);
	}

	D1_LOG("%d %d %d %d\n", pa_inc, pa_dec, na_inc, na_dec);

	/* no values from positive attributes left */
	if (pa_inc == 0 && pa_dec == 0)
		return 0;

	if (pa_inc == 0 && na_dec < 2)
		return 0;

	if (na_inc < 2 && pa_dec == 0)
		return 0;

	if (na_dec >= 2)
		return 1;

	if (na_inc >= 2)
		return 1;

	return 0;
}

#undef INC
#undef DEC

void G105de::save_picbuf()
{
	PhysPt p;
	Bit16u x_1, x_2, x_3;
	Bit16u y_1, y_2, y_3;
	Bit16u w_1, w_2, w_3;
	Bit16u h_1, h_2, h_3;

	x_1 = 0;

	/* check on which page we are */
	switch (ds_readw(0x1324)) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = Real2Phys(ds_readd(0x47d3)) + y_1 * 320 + x_1;
		G105de::copy_to_screen(p, Real2Phys(ds_readd(0x479f)),
			w_1, h_1, 2);
	}

	p = Real2Phys(ds_readd(0x47d3)) + y_2 * 320 + x_2;
	G105de::copy_to_screen(p, Real2Phys(ds_readd(0x479b)), w_2, h_2, 2);

	p = Real2Phys(ds_readd(0x47d3)) + y_3 * 320 + x_3;
	G105de::copy_to_screen(p, Real2Phys(ds_readd(0x4797)), w_3, h_3, 2);
}

void G105de::restore_picbuf(PhysPt ptr)
{
	PhysPt p;
	Bit16u x_1, x_2, x_3;
	Bit16u y_1, y_2, y_3;
	Bit16u w_1, w_2, w_3;
	Bit16u h_1, h_2, h_3;

	x_1 = 0;

	/* check on which page we are */
	switch (ds_readw(0x1324)) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = ptr + y_1 * 320 + x_1;
		G105de::copy_to_screen(Real2Phys(ds_readd(0x479f)),
			p, w_1, h_1, 0);
	}

	p = ptr + y_2 * 320 + x_2;
	G105de::copy_to_screen(Real2Phys(ds_readd(0x479b)), p, w_2, h_2, 0);

	p = ptr + y_3 * 320 + x_3;
	G105de::copy_to_screen(Real2Phys(ds_readd(0x4797)), p, w_3, h_3, 0);
}

/**
 * print_attribs() -	print the attribute values
 *
 */
void G105de::print_attribs()
{
	Bit8u *p;
	char buf[10];
	Bit16u i;

	p = p_datseg + 0x1360;

	for (i = 0; i < 14; p += 3, i++) {
		/* don't print 0s */
		if (host_readb(p) == 0)
			continue;

		/* convert value to string with itoa() */
		sprintf(buf, "%d", (signed char)host_readb(p));

		/* print it */
		print_str(buf, ds_readw(i * 4 + 0x105f),
			ds_readw(i * 4 + 0x1061));
	}
}

/**
 * print_values() - print the values of the character
 *
 */
void G105de::print_values()
{
	char tmp[4];
	const Bit16u align_left = 222;
	const Bit16u align_right = 302;
	Bit16u width;
	Bit16s i, pos;


	switch (ds_readw(0x1324)) {

		case 0: {
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print name */
			print_str((char*)MemBase + PhysMake(datseg, 0x132c),
				180, 12);

			/* print attributes */
			print_attribs();

			/* break if no typus */
			if (ds_readb(0x134d) == 0)
				break;

			/* print height */
			sprintf((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				(char*)MemBase + Real2Phys(ds_readd(0x41f1)),
				ds_readb(0x134f));

			print_str((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				205, 25);

			/* print weight */
			sprintf((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				(char*)MemBase + Real2Phys(ds_readd(0x41f5)),
				ds_readw(0x1350));

			print_str((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				205, 37);

			/* print god name */
			print_str((char*)MemBase + Real2Phys(ds_readd(0x41b9 + 4 * ds_readb(0x1352))),
				205, 49);

			/* print money */
			make_valuta_str((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				ds_readd(0x1358));
			print_str((char*)MemBase + Real2Phys(ds_readd(0x47bf)),
				205, 61);

			/* print LE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", ds_readw(0x138c));
			print_str(tmp, 172, 164);

			/* print AE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", ds_readw(0x1390));
			print_str(tmp, 221, 164);

			/* print Endurance */
			/* originally it was itoa() */
			sprintf(tmp, "%d",
				ds_readw(0x138c) + (signed char)ds_readb(0x1373));
			print_str(tmp, 296, 164);

			/* print MR */
			/* originally it was itoa() */
			sprintf(tmp, "%d", (signed char)ds_readb(0x1392));
			print_str(tmp, 232, 184);

			break;
		}
		case 1: {
			/* SKILLS Page 1/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print fight skills */
			for (i = 0; i < 9; i++) {
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (i & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(i) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(i) / 2) * 12 + 42);
			}

			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 119);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 119);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", (signed char)ds_readb(0x1468));
			print_str(tmp, 271, 184);

			break;
		}
		case 2: {
			/* SKILLS Page 2/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 113);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 113);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", (signed char)ds_readb(0x1468));
			print_str(tmp, 271, 184);

			break;
		}
		case 3: {
			/* SKILLS Page 3/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {
				pos = i - 41;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print nature skills */
			for (i = 26; i < 32; i++) {
				pos = i - 26;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 119);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 119);
			}

			/* print intuition skills */
			for (i = 50; i < 52; i++) {
				pos = i - 50;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 170);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 170);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", (signed char)ds_readb(0x1468));
			print_str(tmp, 271, 184);

			break;
		}
		case 4: {
			/* ATPA Page */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* Print base value  2x the same */
			sprintf(tmp, "%d", (signed char)ds_readb(0x1393));
			print_str(tmp, 231, 30);
			sprintf(tmp, "%d", (signed char)ds_readb(0x1393));
			print_str(tmp, 268, 30);

			for (i = 0; i < 7; i++) {
				/* print AT value */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1394 + i));

				print_str(tmp, 237 - get_str_width(tmp),
					i * 12 + 48);

				/* print PA value */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x139b + i));

				print_str(tmp, 274 - get_str_width(tmp),
					i * 12 + 48);

				/* print skill value */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1434 + i));

				print_str(tmp, 315 - get_str_width(tmp),
					i * 12 + 48);
			}

			/* calc range base value (KL+GE+KK)/4 */
			pos = (signed char)ds_readb(0x1363) +
				(signed char)ds_readb(0x136c) +
				(signed char)ds_readb(0x1372);
			pos /= 4;

			/* print missle weapon value */
			sprintf(tmp, "%d",
				(signed char)ds_readb(0x143b) + pos);

			print_str(tmp, 231, 144);

			/* print thrown weapon value */
			sprintf(tmp, "%d",
				(signed char)ds_readb(0x143c) + pos);
			print_str(tmp, 231, 156);

			break;
		}

		case 5: {
			/* Spells Page 1/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 1; i < 6; i++) {
				pos = i - 1;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}
			for (i = 33; i < 38; i++) {
				pos = i - 33;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {
				pos = i - 6;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 146);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 146);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 12; i <= 17; i++) {
				pos = i - 12;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {
				pos = i - 18;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}
			for (i = 24; i < 27; i++) {
				pos = i - 24;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 146);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 146);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 27; i < 33; i++) {
				pos = i - 27;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			for (i = 38; i < 45; i++) {
				pos = i - 38;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}

			for (i = 45; i <= 46; i++) {
				pos = i - 45;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 160);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 160);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 47; i <= 48; i++) {
				pos = i - 47;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			for (i = 49; i < 58; i++) {
				pos = i - 49;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 71);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 71);
			}

			for (i = 58; i < 60; i++) {
				pos = i - 58;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 148);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 148);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 60; i < 76; i++) {
				pos = i - 60;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 76; i < 86; i++) {
				pos = i - 76;
				/* originally it was itoa() */
				sprintf(tmp, "%d",
					(signed char)ds_readb(0x1469 + i));
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", (signed char)ds_readb(0x14bf));
			print_str(tmp, 217, 184);

			break;
		}

	}
}

/**
 *	make_valuta_str	-	makes a valuta string
 *	@dst:	the destination
 *	@money:	the money in Heller
 *
 *	This funcion is buggy.
 */
void G105de::make_valuta_str(char *dst, unsigned int money)
{
	/* Orig-BUG: d can overflow  on D > 65536*/
	unsigned short d = 0;
	unsigned short s = 0;

	/*	These loops are not very performant.
		They take longer the more money you have.
		Here is a much better solution.
	*/

	/*
	d = money / 100;
	money -= d * 100;

	s = money / 10;
	money -= s * 10;
	*/
	while (money / 100) {
		d++;
		money -= 100;
	}

	while (money / 10) {
		s++;
		money -= 10;
	}

	sprintf(dst, (char*)MemBase + Real2Phys(ds_readd(0x41ed)), d, s, money);
}

void G105de::pal_fade_out(Bit8u *dst, Bit8u *src, Bit16u n)
{
	Bit16u i;

	for (i = 0; i < n; i++) {
		if (host_readb(src + 3 * i) < host_readb(dst + 3 * i)) {
			host_writeb(dst + 3 * i, host_readb(dst + 3 * i) - 1);
		} else {
			if (host_readb(src + i * 3) > host_readb(dst + i * 3))
				host_writeb(dst + 3 * i,
					host_readb(dst + 3 * i) + 1);
		}

		if (host_readb(src + 3 * i + 1) < host_readb(dst + 3 * i + 1)) {
			host_writeb(dst + 3 * i + 1,
				host_readb(dst + 3 * i + 1) - 1);
		} else {
			if (host_readb(src + i * 3 + 1) > host_readb(dst + i * 3 + 1))
				host_writeb(dst + 3 * i + 1,
					host_readb(dst + 3 * i + 1) + 1);
		}

		if (host_readb(src + 3 * i + 2) < host_readb(dst + 3 * i + 2)) {
			host_writeb(dst + 3 * i + 2,
				host_readb(dst + 3 * i + 2) - 1);
		} else {
			if (host_readb(src + i * 3 + 2) > host_readb(dst + i * 3 + 2))
				host_writeb(dst + 3 * i + 2,
					host_readb(dst + 3 * i + 2) + 1);
		}
	}
}

void G105de::pal_fade_in(Bit8u *dst, Bit8u *src, Bit16u col, Bit16u n)
{
	Bit16u i;
	Bit16u si;

	si = 0x40 - col;

	for (i = 0; i < n; i++) {
		if (host_readb(src + 3 * i) >= si) {
			if (host_readb(src + i * 3) > host_readb(dst + i * 3))
				host_writeb(dst + 3 * i,
					host_readb(dst + 3 * i) + 1);
		}

		if (host_readb(src + 3 * i + 1) >= si) {
			if (host_readb(src + i * 3 + 1) > host_readb(dst + i * 3 + 1))
				host_writeb(dst + 3 * i + 1,
					host_readb(dst + 3 * i + 1) + 1);
		}

		if (host_readb(src + 3 * i + 2) >= si) {
			if (host_readb(src + i * 3 + 2) > host_readb(dst + i * 3 + 2))
				host_writeb(dst + 3 * i + 2,
					host_readb(dst + 3 * i + 2) + 1);
		}
	}
}

void G105de::set_mouse_isr()
{
	/* save adress of the old ISR */
	ds_writed(0x247c, bc__dos_getvect(0x1c));
	/* set a the new one */
	bc__dos_setvect(0x1c, RealMake(relocation + 0x3c6, 0x72b3));
}

void G105de::restore_mouse_isr()
{
	bc__dos_setvect(0x1c, ds_readd(0x247c));
}

void init_colors()
{
	set_palette(MemBase + PhysMake(datseg, 0x1d4c), 0x00, 1);
	set_palette(MemBase + PhysMake(datseg, 0x1d49), 0xff, 1);
	set_palette(MemBase + PhysMake(datseg, 0x1d4f), 0xd8, 8);
	set_palette(MemBase + PhysMake(datseg, 0x1d67), 0xc8, 3);
	set_palette(MemBase + PhysMake(datseg, 0x119b), 0x40, 0x20);
	set_palette(MemBase + PhysMake(datseg, 0x1d70), 0x20, 0x20);
	set_vals(0xff, 0x0);
}

void init_stuff()
{
	init_colors();

	/* these 3 variables are bogus */
	ds_writew(0x4783, 0xc8);
	ds_writew(0x4785, 0xc9);
	ds_writew(0x4787, 0xca);

	/* number of menu tiles width */
	ds_writew(0x40b9, 3);

	ds_writed(0x40c1, ds_readd(0x47cb));
}
