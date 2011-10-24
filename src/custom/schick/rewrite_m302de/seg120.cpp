/**
 *	Rewrite of DSA1 v3.02_de functions of seg120 (misc)
 *	Functions rewritten: 1/11
 */

#include "schick.h"

#include "seg008.h"

namespace M302de {

void refresh_colors()
{
	set_color(p_datseg + 0xb22d, 0);
	set_color(p_datseg + 0xb22a, 0xff);
	set_palette(p_datseg + 0xb230, 0xd8, 8);

	set_palette(p_datseg + 0xb248, 0xc8, 3);
	set_palette(p_datseg + 0xb251, 0x40, 0x20);
	set_palette(p_datseg + 0xb2b1, 0x20, 0x20);
	set_palette(p_datseg + 0x2723, 0x60, 0x20);

}

}
