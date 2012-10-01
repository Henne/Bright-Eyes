/*
 *	Rewrite of DSA2 c1.02_de functions seg043 (gfx_mode/palette)
 *	Functions rewritten 1/9
 *	Function unused 0/2
 */

#include "paging.h"
#include "../../ints/int10.h"

#include "schweif.h"

namespace C102de {

void set_video_mode(unsigned char mode)
{
	INT10_SetVideoMode(mode);
}

}
