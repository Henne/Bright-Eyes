/*
 *	Rewrite of DSA2 c1.02_de functions seg041 (nvf graphics format decompression)
 *	Functions rewritten 2/5
 *	Function unused 0/4
 */

#include "schweif.h"
#include "c102de_seg041.h"
#include "c102de_seg029.cpp" // decomp_pp20
#include "string.h" // memcpy

namespace C102de {

/**
 * decomp_rle() - decompress a RLE compressed picture
 * @width:	width of the picture
 * @height:	height of the picture
 * @dst:	destination (cf. @tmp_buffer)
 * @src:	source
 * @tmp_buffer: alternative destination
 * @mode:	nvf mode/type
 * helper function (is part of process_nvf in original code)
 *
 */
void decomp_rle(unsigned short width, unsigned short height, Bit8u *dst,
	Bit8u *src, Bit8u *tmp_buffer, unsigned short mode) {

	Bit8u *my_dst;
	unsigned short i,x;
	unsigned char tmp;
	unsigned char cnt;

	/* select destination buffer */
	if (mode == 5 || mode == 4)
		my_dst = tmp_buffer;
	else
		my_dst = dst;

	do {
		/* decode one line */
		x = width;
		do {
			tmp = *src++;
			if (tmp == 0x7f) {
				cnt = *src++;
				tmp = *src++;
				for (i = 0; i < cnt; i++)
					*my_dst++ = tmp;
				x -= cnt;
			} else {
				*my_dst++ = tmp;
				x--;
			}
		} while (x);

		/* reverse line */
		if (mode == 5 || mode == 4) {
			my_dst--;
			for (i = width; i; i--)
				*dst++ = *my_dst--;
		}

		/* set destination to next line */
		switch (mode) {
		case	5:	break;
		case	3:	dst += width;
				break;
		case	2:	dst += 320;
				break;
		case	4:	dst += 320 - width;
				break;
		}

	} while (--height);
}

    static inline unsigned int swap_u32(unsigned int v) {
	return ((v >> 24) & 0xff) | ((v >> 16) & 0xff) << 8 |
	    ((v >> 8) & 0xff) << 16 | (v&0xff) << 24;

    }


    signed int process_nvf(struct nvf_desc *nvf) {

	signed char nvf_type;
	Bit8u *src, *dst;
	int p_size;
	int offs;
	signed int retval;
	short va;
	short height;
	short pics;
	short width;
	short i;

	Bit8u *p = nvf->src;
	dst = nvf->dst;
	D2_LOG("nvf struct: dst=%p, src=%p, nr=%d, type=%d, w=%d, h=%d\n",
	       nvf_desc->dst, nvf_desc->src, nvf_desc->nr, nvf_desc->type, nvf_desc->width, nvf_desc->height);
	nvf_type = host_readb(p);
	va = nvf_type & 0x80;
	nvf_type &= 0x7f;

	pics = host_readw(p + 1);

	if (nvf->nr < 0)
	    nvf->nr = 0;

	if (nvf->nr > pics - 1)
	    nvf->nr = pics - 1;

	switch (nvf_type) {

	case 0x00:
	    width = host_readw(p + 3);
	    height = host_readw(p + 5);
	    p_size = height * width;
	    src =  p + nvf->nr * p_size + 7;
	    break;

	case 0x01:
	    offs = pics * 4 + 3;
	    for (i = 0; i < nvf->nr; i++) {
		width = host_readw(p + i * 4 + 3);
		height = host_readw(p + i * 4 + 5);
		offs += width * height;
	    }

	    width = host_readw(p + nvf->nr * 4 + 3);
	    height = host_readw(p + nvf->nr * 4 + 5);
	    p_size = width * height;
	    src = p + offs;
	    break;

	case 0x02: case 0x04:
	    width = host_readw(p + 3);
	    height = host_readw(p + 5);
	    offs = pics * 4 + 7;
	    for (i = 0; i < nvf->nr; i++)
		offs += host_readd(p  + (i * 4) + 7);

	    p_size = host_readd(p + nvf->nr * 4 + 7);
	    src = p + offs;
	    break;

	case 0x03: case 0x05:
	    offs = pics * 8 + 3;
	    for (i = 0; i < nvf->nr; i++)
		offs += host_readd(p  + (i * 8) + 7);

	    width = host_readw(p + nvf->nr * 8 + 3);
	    height = host_readw(p + nvf->nr * 8 + 5);
	    p_size = host_readd(p + i * 8 + 7);
	    src = p + offs;
	    break;
	}

	switch (nvf->type) {

	case 0:
	    /* PP20 decompression */

	    if (va != 0) {

		/* get size from unpacked picture */
		retval = host_readd(src);
		retval = host_readd(src + (retval - 4));
		retval = swap_u32(retval) >> 8;

	    } else
		retval = width * height;

	    decomp_pp20(dst, src, p_size);
	    break;

	case 2: case 3: case 4: case 5:
	    /* RLE decompression */
	    decomp_rle(width, height, dst, src,
		       Real2Host(ds_readd(0xd2eb)), nvf->type);
	    /* retval was originally neither set nor used here.
	       VC++2008 complains about an uninitialized variable
	       on a Debug build, so we fix this for debuggings sake */
	    /* Orig-Fix */
	    retval = p_size;
	    break;

	default:
	    /* No decompression, just copy */
	    memcpy(nvf->dst, src, p_size);
	    retval = p_size;
	}

	host_writew(nvf->width, width);
	host_writew(nvf->height, height);

	return retval;
    }
}
