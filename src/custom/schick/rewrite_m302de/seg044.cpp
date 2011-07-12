/*
	Rewrite of DSA1 v3.02_de functions of seg044 (Fightsystem)
	Functions rewritten: 2/6
*/

#include "schick.h"

namespace M302de {

Bit16s seg044_0000(Bit8u *p, Bit16s ani, Bit16u a3)
{
	Bit8u *p_start, *p_off;
	Bit16s nr_anis;
	Bit16u i;
	Bit8s l3;

	/* get pointer from ANI.DAT */
	p_start = Real2Host(ds_readd(0xe378));

	if (a3 == 3)
		p_start = Real2Host(ds_readd(0xe374));

	/* get number of animation sequences */
	nr_anis = host_readw(p_start);

	if (ani < 0 || ani > nr_anis)
		return 0;

	p_off = p_start;
	p_off += nr_anis + 2;

	l3 = host_readb(p_start + 2);

	for (i = 1; i <= ani; i++) {
		p_off += l3;
		l3 = host_readb(p_start + i + 2);
	}

	p_off++;

	l3 -= 2;

	for (i = 0; l3 > i; p_off++, p++, i++)
		host_writeb(p, host_readb(p_off));

	return l3;
}

Bit8s seg044_00ae(Bit16s ani)
{
	Bit8u *p_start, *p_off;
	Bit8s l3;
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

	l3 = host_readb(p_start + 2);

	for (i = 1; i <= ani; i++) {
		p_off += l3;
		l3 = host_readb(p_start + i + 2);
	}

	return host_readb(p_off);
}

}
