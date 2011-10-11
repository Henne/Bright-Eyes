/*
	Rewrite of DSA1 v3.02_de functions of seg028 (map / file loader)
	Functions rewritten: 2/19
*/

#include "schick.h"

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg009.h"

namespace M302de {

void load_npc(signed short index)
{
	Bit8u *npc_dst;
	unsigned short fd;

	npc_dst = get_hero(0) + 0x291c;
	/* load from temp directory */
	fd = load_archive_file(index | 0x8000);
	bc__read(fd, npc_dst, 0x6da);
	bc_close(fd);

	if (host_readb(npc_dst + 0x22) == 1) {
		/* female */
		/* set an unknown variable to typus + 11 */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21) + 11);
		if (host_readb(npc_dst + 0x9b) > 21)
			host_writeb(npc_dst + 0x9b, 21);
	} else {
		/* male */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21));
		if (host_readb(npc_dst + 0x9b) > 10)
			host_writeb(npc_dst + 0x9b, 10);
	}


}

void load_fightbg(signed short index)
{
	unsigned short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, Real2Host(ds_readd(0xd303)), 30000);
	decomp_pp20(Real2Host(ds_readd(0xd303)), Real2Host(ds_readd(0xc3a9)),
		Real2Host(ds_readd(0xd303)) + 4, get_readlength2(fd));
	bc_close(fd);

}

}
