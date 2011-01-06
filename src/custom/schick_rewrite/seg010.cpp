/*
	Rewrite of DSA1 v3.02_de functions of seg010 (EMS)
	Functions rewritten: 3/8
*/
#include <string.h>

#include "regs.h"
#include "callback.h"

#include "schick.h"

static unsigned short EMS_installed() {

	RealPt IRQ_67;
	char *sig;

	/* get interrupt vector of interrupt 0x67 */
	IRQ_67 = host_readd(MemBase + 0x67 * 4);

	/* Make a pointer to where "EMMXXXX0" stands if EMS is enabled */
	sig = (char*)MemBase + PhysMake(RealSeg(IRQ_67), 0x0a);

	/* return 1 if signature is found */
	if (!strncmp(sig, "EMMXXXX0", 8))
		return 1;

	return 0;
}

static RealPt EMS_get_frame_ptr() {

	reg_ax = 0x4100;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return RealMake(reg_bx, 0);

	return RealMake(0, 0);
}

unsigned short EMS_init() {

	if (EMS_installed()) {
		ds_writed(0x4baa, EMS_get_frame_ptr());
		return 1;
	}

	return 0;
}
