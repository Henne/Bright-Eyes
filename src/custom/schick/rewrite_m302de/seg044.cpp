/*
	Rewrite of DSA1 v3.02_de functions of seg044 (Fightsystem)
	Functions rewritten: 2/6
*/

#include "schick.h"

namespace M302de {

/**
 * copy_ani_seq() - copies an animation sequence
 * @dst:	where to copy the sequence
 * @ani:	the number of the animation sequence
 * @type:	type of animation (3 = WEAPANI.DAT else ANI.DAT)
 *
 * @return	the number of copied bytes
 */
Bit16s copy_ani_seq(Bit8u *dst, Bit16s ani, Bit16u type)
{
	Bit8u *p_start, *p_off;
	Bit16s nr_anis;
	Bit16u i;
	Bit8s length;

	/* get pointer from ANI.DAT */
	p_start = Real2Host(ds_readd(0xe378));

	/* check if we must use WEAPANI.DAT */
	if (type == 3)
		p_start = Real2Host(ds_readd(0xe374));

	/* get number of animation sequences */
	nr_anis = host_readw(p_start);

	/* sanity check */
	if (ani < 0 || ani > nr_anis)
		return 0;

	/* set p_off the the first data byte */
	p_off = p_start;
	p_off += nr_anis + 2;

	/* set p_off the first byte of the requested animation sequence */
	length = host_readb(p_start + 2);

	for (i = 1; i <= ani; i++) {
		p_off += length;
		length = host_readb(p_start + i + 2);
	}

	/* skip the first byte */
	p_off++;
	/* calc the length of the sequence */
	length -= 2;

	/* copy them */
	for (i = 0; i < length; p_off++, dst++, i++)
		host_writeb(dst, host_readb(p_off));

	return length;
}

/**
 * seg044_00ae() - return the first byte of a sequence from ANI.DAT
 * @ani:	the number of the animation sequence
 */
Bit8s seg044_00ae(Bit16s ani)
{
	Bit8u *p_start, *p_off;
	Bit8s length;
	Bit16s nr_anis;
	Bit16u i;

	/* get pointer from ANI.DAT */
	p_start = Real2Host(ds_readd(0xe378));

	/* get number of ani seqences in ANI.DAT */
	nr_anis = host_readw(p_start);

	if (ani < 0 || ani > nr_anis)
		return 0;

	p_off = p_start;
	p_off += nr_anis + 2;

	length = host_readb(p_start + 2);

	for (i = 1; i <= ani; i++) {
		p_off += length;
		length = host_readb(p_start + i + 2);
	}

#if 0
	D1_INFO("ani = 0x%x\tlength = %d\n", ani, length);
	for (i = 0; i < length; i++) {
		D1_INFO("%x ", host_readb(p_off + i));
	}
	D1_INFO("\n");
#endif

	return host_readb(p_off);
}

}
