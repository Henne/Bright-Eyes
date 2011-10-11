/*
	Rewrite of DSA1 v3.02_de functions of seg028 (map / file loader)
	Functions rewritten: 1/19
*/

#include "schick.h"

#include "seg000.h"
#include "seg002.h"
#include "seg009.h"

namespace M302de {

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
