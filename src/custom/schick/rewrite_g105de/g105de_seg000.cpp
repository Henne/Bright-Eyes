#include <stdlib.h>

#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schick.h"

#include "g105de_seg000.h"

namespace G105de {


Bit16u bc__dos_close(Bit16s fd)
{
	CPU_Push16(fd);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x02dc);
	CPU_Pop16();
	return reg_ax;
}

Bit16u bc__dos_open(char* pathP, Bit16u oflag, Bit8u* fd)
{
	RealPt PP = 0L;
	RealPt FP = 0L;

	PP = bc_farcalloc(1024, sizeof(char));
	FP = bc_farcalloc(16, sizeof(char));

	if (PP && FP) {
		// copy filename
		strncpy((char*)Real2Host(PP), pathP, 1023);

		// call _dos_open
		CPU_Push32(FP);
		CPU_Push16(oflag);
		CPU_Push32(PP);
		CALLBACK_RunRealFar(reloc_gen + 0x0, 0x02f2);
		CPU_Pop32();
		CPU_Pop16();
		CPU_Pop32();

		// copy file descriptor
		host_writew((Bit8u*)fd, (Bit16s)host_readw(Real2Host(FP)));

		bc_free(PP);
		bc_free(FP);
		return reg_ax;
	}
	return 0;
}

Bit16u bc__dos_read(Bit16s fd, Bit8u* buf, Bit16u len, Bit16u* nread)
{
	RealPt PP = 0L;
	RealPt FP = 0L;

	PP = bc_farcalloc(len + 16L, sizeof(char));
	FP = bc_farcalloc(16, sizeof(char));

	if (PP && FP) {
		// call _dos_read
		CPU_Push32(FP);
		CPU_Push16(len);
		CPU_Push32(PP);
		CPU_Push16(fd);
		CALLBACK_RunRealFar(reloc_gen + 0x0, 0x0328);
		CPU_Pop16();
		CPU_Pop32();
		CPU_Pop16();
		CPU_Pop32();

		// copy nread
		host_writew((HostPt)nread, host_readw(Real2Host(FP)));

		// copy buffer content
		memcpy((HostPt)buf, Real2Host(PP), len);

		bc_free(PP);
		bc_free(FP);
		return reg_ax;
	}
	return 0;
}

void bc_exit(Bit16u exitval)
{
	exit(exitval);
}

RealPt bc__dos_getvect(Bit16s intno)
{
	CPU_Push16(intno);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x0438);
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

void bc__dos_setvect(Bit16s intno, RealPt ptr)
{
	CPU_Push32(ptr);
	CPU_Push16(intno);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x0447);
	CPU_Pop16();
	CPU_Pop32();
}

RealPt bc_F_PADD(RealPt p, Bit32s v)
{
	CPU_Push16(reg_ax);
	CPU_Push16(reg_dx);
	CPU_Push16(reg_cx);
	CPU_Push16(reg_bx);

	reg_dx = RealSeg(p);
	reg_ax = RealOff(p);
	reg_cx = (v >> 16) & 0xffff;
	reg_bx = v & 0xffff;
		
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x05d2);

	RealPt r = RealMake(reg_dx, reg_ax);

	//D1_INFO("bc_F_PADD(0x%08x, %d) = 0x%08x\n", p, v, r);

	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();

	return r;
}

Bit32s bc_lseek(Bit16u handle, Bit32u offset, Bit16s whence)
{
	CPU_Push16(whence);
	CPU_Push32(offset);
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x072d);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
	
	return (Bit32s)(reg_dx << 16) + reg_ax;
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

RealPt bc_farcalloc(Bit32s nelem, Bit32s size)
{
	CPU_Push32(size);
	CPU_Push32(nelem);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x16c7);
	CPU_Pop32();
	CPU_Pop32();
	
	return RealMake(reg_dx, reg_ax);
}

void bc_harderr(RealPt handler)
{
	CPU_Push32(handler);
	CALLBACK_RunRealFar(reloc_gen + 0x0, 0x1801);
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
Bit16s bc__create(RealPt pathP, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(pathP);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x212f);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

Bit16s bc_flushall()
{
	CALLBACK_RunRealFar(reloc_gen + 0, 0x224a);
	return reg_ax;
}

RealPt bc_memcpy(RealPt dst, RealPt src, Bit16s n)
{
	CPU_Push16(n);
	CPU_Push32(src);
	CPU_Push32(dst);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x254e);
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

RealPt bc_memset(RealPt dst, Bit8s c, Bit16u n)
{
	CPU_Push16(n);
	CPU_Push16(c);
	CPU_Push32(dst);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x2596);
	CPU_Pop32();
	CPU_Pop16();
	CPU_Pop16();
	return dst;
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

RealPt bc_strcat(RealPt dst, RealPt src)
{
	CPU_Push32(src);
	CPU_Push32(dst);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x2d96);
	CPU_Pop32();
	CPU_Pop32();
	return RealMake(reg_dx, reg_ax);
}

RealPt bc_strcpy(RealPt dst, RealPt src)
{
	CPU_Push32(src);
	CPU_Push32(dst);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x2dd5);
	CPU_Pop32();
	CPU_Pop32();
	return RealMake(reg_dx, reg_ax);
}

RealPt bc_strncpy(RealPt dst, RealPt src, Bit16s n)
{
	CPU_Push16(n);
	CPU_Push32(src);
	CPU_Push32(dst);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x2e55);
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

Bit16s bc_write(Bit16s handle, RealPt buf, Bit16u n)
{
	CPU_Push16(n);
	CPU_Push32(buf);
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_gen + 0, 0x360e);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
	return (Bit16s)reg_ax;
}

/* functions that work different on host and emu */
Bit16s bc_open_host(char* fname, Bit16u attrib)
{
	RealPt p;
	Bit16s handle = -1;
	p = bc_farcalloc(100, sizeof(char));
	if (p) {
		strncpy((char*)Real2Host(p), fname, 100);
		handle = bc_open(p, attrib);
		bc_free(p);
		p = 0L;
	}
	return handle;
}

Bit16s bc__create_host(char* PathP, Bit16u attrib)
{
	RealPt p;
	Bit16s handle = -1;
	p = bc_farcalloc(100, sizeof(char));
	if (p) {
		strncpy((char*)Real2Host(p), PathP, 100);
		handle = bc__create(p, attrib);
		bc_free(p);
		p = 0L;
	}
	return handle;
}

}
