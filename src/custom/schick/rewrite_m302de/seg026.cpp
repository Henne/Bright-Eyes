#include "schick.h"
#include "schick_common.h"
#include "v302de.h"

#include "seg000.h"
#include "seg002.h"

namespace M302de {

/**
 *	load_in_head() - loads a head icon from IN_HEADS.NVF
 *	@head:	index of the desired head
 */
void load_in_head(Bit16s head)
{
	Bit16u fd;

	if (head < 0)
		return;

	fd = load_archive_file(0xb3);

	seg002_0c72(fd, head * 1024);

	read_archive_file(fd, Real2Host(ds_readd(0xd2f3)), 1024);

	bc_close(fd);

	ds_writew(0x515c, head);

}

}
