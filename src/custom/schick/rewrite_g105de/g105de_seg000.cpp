#include <stdlib.h>

#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schick.h"

#include "g105de_seg000.h"

namespace G105de {

void bc_exit(Bit16u exitval)
{
	exit(exitval);
}

RealPt bc__dos_getvect(Bit16s intno)
{
	return host_readd(MemBase + intno * 4);
}

void bc__dos_setvect(Bit16s intno, RealPt ptr)
{
	host_writed(MemBase + intno * 4, ptr);
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

signed short bc_bioskey(signed short cmd)
{
	CPU_Push16(cmd);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x0f43);
	CPU_Pop16();
	return reg_ax;
}

void bc_clrscr()
{
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0xf68);
}

void bc_free(RealPt ptr)
{
	CPU_Push32(ptr);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x1295);
	CPU_Pop32();
}

Bit16s bc_close(Bit16u handle)
{
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x20bc);
	CPU_Pop16();

	return reg_ax;
}

Bit16s bc__close(Bit16u handle)
{
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x20e4);
	CPU_Pop16();

	return reg_ax;
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


Bit16s bc_open(RealPt fname, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x2655);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
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
