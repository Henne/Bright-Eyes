/*
	Rewrite of DSA1 v3.02_de functions of seg010 (EMS)
	Functions rewritten: 7/8
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

unsigned short EMS_get_num_pages_unalloced() {

	reg_ax = 0x4200;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_bx;

	return 0;
}

unsigned short EMS_alloc_pages(unsigned short pages) {

	reg_ax = 0x4300;
	reg_bx = pages;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_dx;

	return 0;
}

unsigned short EMS_free_pages(unsigned short handle) {

	reg_ax = 0x4500;
	reg_dx = handle;
	CALLBACK_RunRealInt(0x67);

	reg_ah = reg_al;
	return reg_ax;
}

unsigned short EMS_map_memory(unsigned short handle, unsigned short lpage, unsigned char ppage) {

	reg_ax = 0x4400;
	reg_al = ppage;
	reg_bx = lpage;
	reg_dx = handle;
	CALLBACK_RunRealInt(0x67);

	reg_ah = reg_al;
	return reg_ax;
}

unsigned short EMS_init() {

	if (EMS_installed()) {
		ds_writed(0x4baa, EMS_get_frame_ptr());
		return 1;
	}

	return 0;
}
