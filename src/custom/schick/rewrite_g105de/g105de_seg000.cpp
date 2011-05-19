#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schick.h"

#include "g105de_seg000.h"

namespace G105de {

RealPt bc__dos_getvect(Bit16s intnr)
{
	return host_readd(MemBase + intnr * 4);
}

void bc__dos_setvect(Bit16s intnr, RealPt ptr)
{
	host_writed(MemBase + intnr * 4, ptr);
}

Bit32s bc_lseek(Bit16u handle, Bit32u offset, Bit16s whence) {

	ds_writew(0x2298 + handle * 2, ds_readw(0x2298 + handle * 2) & 0xfdff);

	if (!DOS_SeekFile(handle, &offset, whence))
		return -1;

	return offset;

}

Bit16s bc__read(Bit16u handle, Bit8u *buf, Bit16u count) {

	if ((ds_readw(0x2298 + handle * 2) & 2))
		return -1;

	if (!DOS_ReadFile(handle, buf, &count))
		return -1;

	return (Bit16s)count;
}

signed short bioskey(signed short cmd) {
	reg_ah = cmd &  0xff;
	reg_al = 0;
	CALLBACK_RunRealInt(0x16);

	if (GETFLAG(ZF)) {
		if (!(cmd & 1))
			return reg_ax;

		return 0;
	} else {
		if (!(cmd & 1))
			return reg_ax;
		if (reg_ax)
			return reg_ax;
		return -1;
	}
}

void bc_free(RealPt ptr)
{
	CPU_Push32(ptr);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x1295);
	CPU_Pop32();
}

Bit16s bc_close(Bit16u handle) {

	if (handle >= ds_readw(0x2296))
		return -1;

	ds_writew(0x2298 + handle * 2, 0);

	return bc__close(handle);

}

Bit16s bc__close(Bit16u handle) {

	if (!DOS_CloseFile(handle))
		return -1;

	ds_writew(0x2298 + handle * 2, 0);

	return 0;
}

/* This create function is just a hack and gets replaced later by fopen() */
Bit16s bc__create(Bit8u *pathP, Bit16u attr)
{
	Bit16u handle;

	if (!DOS_CreateFile((char*)pathP, attr, &handle))
		return -1;

	ds_writew(0x2298 + handle * 2, handle);

	return handle;
}


/* This write function is just a hackand gets replaced later by fwrite() */
Bit16s bc_write(Bit16u handle, Bit8u *buf, Bit16u count)
{

	if ((ds_readw(0x2298 + handle * 2) & 2))
		return -1;

	if (!DOS_WriteFile(handle, buf, &count))
		return -1;

	return (Bit16s)count;
}

}
