/*
 *	Rewrite of DSA1 v3.02_de functions of seg000 (Borland C-Lib)
 *	Functions rewritten: 0/0
*/
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"
#include "paging.h"

#include "schick.h"

#include "seg000.h"

namespace M302de {

Bit16s bc_chdir(char *path)
{
	Bit32u esp_bak = reg_esp;

	reg_esp -= 100;

	char *str = (char*)Real2Host(RealMake(SegValue(ss), reg_esp));
	strcpy(str, path);

	CPU_Push32(RealMake(SegValue(ss), reg_esp));
	CALLBACK_RunRealFar(reloc_game, 0x2f7);
	CPU_Pop32();

	if (strlen(str) > 40) {
		D1_ERR("Error: Verzeichnis zu tief. maximal 40 Zeichen\n");
	}

	reg_esp = esp_bak;

	return reg_ax;
}

Bit16s bc_mkdir(char *path)
{
	Bit32u esp_bak = reg_esp;

	reg_esp -= 100;

	char *str = (char*)Real2Host(RealMake(SegValue(ss), reg_esp));
	strcpy(str, path);

	CPU_Push32(RealMake(SegValue(ss), reg_esp));
	CALLBACK_RunRealFar(reloc_game, 0xb5c);
	CPU_Pop32();

	if (strlen(str) > 40) {
		D1_ERR("Error: Verzeichnis zu tief. maximal 40 Zeichen\n");
	}

	reg_esp = esp_bak;

	return reg_ax;
}

Bit16s bc_getcurdir(Bit16s drive, char *dir)
{
	Bit32u esp_bak = reg_esp;

	CPU_Push32(RealMake(SegValue(ss), reg_sp - 100));
	CPU_Push16(drive);
	CALLBACK_RunRealFar(reloc_game, 0x73e);
	CPU_Pop16();
	CPU_Pop32();

	reg_esp -= 100;

	char *str = (char*)Real2Host(RealMake(SegValue(ss), reg_esp));


	if (strlen(str) > 40) {
		D1_ERR("Error: Verzeichnis zu tief. maximal 40 Zeichen\n");
	}

	strcpy(dir, str);

	reg_esp = esp_bak;
	return reg_ax;
}

Bit16s bc_getdisk(void)
{
	CALLBACK_RunRealFar(reloc_game, 0x781);
	return reg_ax;
}

Bit16s bc_setdisk(Bit16s __drive)
{
	CPU_Push16(__drive);
	CALLBACK_RunRealFar(reloc_game, 0x79b);
	CPU_Pop16();
	return reg_ax;
}

void bc_dos_getdiskfree(Bit16u drive, Bit8u *p)
{
	Bit32u esp_bak = reg_esp;

	CPU_Push32(RealMake(SegValue(ss), reg_sp - 32));
	CPU_Push16(drive);
	CALLBACK_RunRealFar(reloc_game, 0x7ed);
	CPU_Pop16();
	CPU_Pop32();

	reg_esp -= 32;

	host_writew(p + 0, CPU_Pop16());
	host_writew(p + 2, CPU_Pop16());
	host_writew(p + 4, CPU_Pop16());
	host_writew(p + 6, CPU_Pop16());

	reg_esp = esp_bak;
}

void F_PADA(RealPt p, Bit32s off)
{
	reg_dx = RealSeg(p);
	reg_ax = RealOff(p);
	reg_cx = off >> 16;
	reg_bx = off & 0xffff;
	CALLBACK_RunRealFar(reloc_game, 0x928);
}

RealPt F_PADD(RealPt p, Bit32s off)
{
	reg_dx = RealSeg(p);
	reg_ax = RealOff(p);
	reg_cx = off >> 16;
	reg_bx = off & 0xffff;
	CALLBACK_RunRealFar(reloc_game, 0x9b0);
	return RealMake(reg_dx, reg_ax);
}

Bit32s F_PSUB(RealPt p1, RealPt p2)
{
	reg_dx = RealSeg(p1);
	reg_ax = RealOff(p1);

	reg_cx = RealSeg(p2);
	reg_bx = RealOff(p2);

	CALLBACK_RunRealFar(reloc_game, 0xa10);
	return ((reg_dx << 16) | reg_ax);
}

void bc_exit(Bit16s status)
{
	CPU_Push16(status);
	CALLBACK_RunRealFar(reloc_game, 0x6df);
	CPU_Pop16();
}

RealPt bc__dos_getvect(Bit8u __interruptno)
{
	CPU_Push16(__interruptno);
	CALLBACK_RunRealFar(reloc_game + 0, 0x816);
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

void bc__dos_setvect(Bit8u __interruptno, RealPt handler)
{
	CPU_Push32(handler);
	CPU_Push16(__interruptno);
	CALLBACK_RunRealFar(reloc_game + 0, 0x825);
	CPU_Pop16();
	CPU_Pop32();
}

Bit32s bc_lseek(Bit16u handle, Bit32u offset, Bit16s whence) {

	ds_writew(0xb788 + handle * 2, ds_readw(0xb788 + handle * 2) & 0xfdff);

	if (!DOS_SeekFile(handle, &offset, whence))
		return -1;

	return offset;

}

void bc_srand(Bit16u seed)
{
	CPU_Push16(seed);
	CALLBACK_RunRealFar(reloc_game + 0, 0xbac);
	CPU_Pop16();
}

Bit16s bc__read(Bit16u handle, Bit8u *buf, Bit16u count) {

	if ((ds_readw(0xb788 + handle * 2) & 2))
		return -1;

	if (!DOS_ReadFile(handle, buf, &count))
		return -1;

	if (count == 5952)
		schick_status_update(buf, count);

	return (Bit16s)count;
}


Bit32s bc_time_dosbox(RealPt p)
{
	CPU_Push32(p);
	CALLBACK_RunRealFar(reloc_game + 0, 0x1123);
	CPU_Pop32();

	return reg_ax;
}

signed short bc_unlink(RealPt fname)
{
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_game + 0, 0x11a7);
	CPU_Pop32();
	return reg_ax;
}

signed short bc_bioskey(signed short cmd)
{
	CPU_Push16(cmd);
	CALLBACK_RunRealFar(reloc_game + 0, 0x176d);
	CPU_Pop16();
	return reg_ax;
}

void bc_clrscr(void)
{
	CALLBACK_RunRealFar(reloc_game + 0, 0x1792);
}

void bc_farfree(RealPt ptr)
{
	CPU_Push32(ptr);
	CALLBACK_RunRealFar(reloc_game + 0, 0x1e55);
	CPU_Pop32();
}

Bit32u bc_farcoreleft(void)
{
	CALLBACK_RunRealFar(reloc_game + 0, 0x2315);
	return (reg_dx << 16) + reg_ax;
}

RealPt bc_farcalloc(Bit32u nmemb, Bit32u size)
{
	CPU_Push32(size);
	CPU_Push32(nmemb);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2287);
	CPU_Pop32();
	CPU_Pop32();

	return RealMake(reg_dx, reg_ax);
}

void bc_harderr(RealPt p)
{
	CPU_Push32(p);
	CALLBACK_RunRealFar(reloc_game, 0x2411);
	CPU_Pop32();
}

void bc_hardresume(Bit16s __axret)
{
	CPU_Push16(__axret);
	CALLBACK_RunRealFar(reloc_game, 0x2423);
	CPU_Pop16();
}

Bit16s bc_spawnl(Bit16s mode, RealPt a2, RealPt a3, RealPt a4, RealPt a5, RealPt a6, RealPt a7)
{
	CPU_Push32(a7);
	CPU_Push32(a6);
	CPU_Push32(a5);
	CPU_Push32(a4);
	CPU_Push32(a3);
	CPU_Push32(a2);
	CPU_Push16(mode);
	CALLBACK_RunRealFar(reloc_game, 0x2d82);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();

	return reg_ax;
}

signed short bc_findfirst_dosbox(RealPt path, RealPt __ffblk, signed short __attrib)
{
	CPU_Push16(__attrib);
	CPU_Push32(__ffblk);
	CPU_Push32(path);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3040);
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop16();
	return reg_ax;
}

signed short bc_findfirst(RealPt path, struct ffblk *__ffblk, signed short __attrib)
{
	Bit32u reg_esp_bak = reg_esp;
	reg_esp -= 0x50;

	Bit16s retval = bc_findfirst_dosbox(path, RealMake(SegValue(ss), reg_sp), __attrib);
	memcpy(__ffblk, Real2Host(RealMake(SegValue(ss), reg_sp)), 43);

	reg_esp = reg_esp_bak;
	return retval;
}

signed short bc_findnext_dosbox(RealPt __ffblk)
{
	CPU_Push32(__ffblk);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3073);
	CPU_Pop32();
	return reg_ax;
}

signed short bc_findnext(struct ffblk *__ffblk)
{
	/* allocate 0x50 bytes on the stack */
	Bit32u reg_esp_bak = reg_esp;
	reg_esp -= 0x50;

	/* make a pointer to the location on the stack */
	RealPt ptr = RealMake(SegValue(ss), reg_sp);

	/* copy the old __ffblk to the stack */
	memcpy(Real2Host(ptr), __ffblk, 43);
	/* call findnext() */
	Bit16s retval = bc_findnext_dosbox(ptr);
	/* copy the result to host memory */
	memcpy(__ffblk, Real2Host(ptr), 43);

	/* free memory on the stack */
	reg_esp = reg_esp_bak;

	return retval;
}

Bit16s bc_flushall(void)
{
	CALLBACK_RunRealFar(reloc_game + 0, 0x30a0);
	return reg_ax;
}

Bit16s bc_close(Bit16s handle)
{
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2eb2);
	CPU_Pop16();

	return reg_ax;
}

Bit16s bc__close(Bit16s handle)
{
	CPU_Push16(handle);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2eda);
	CPU_Pop16();

	return reg_ax;
}

RealPt bc_memset(RealPt dst, Bit8s val, Bit16u count)
{
	PhysPt d;

	d = Real2Phys(dst);

	while (count--) {
		mem_writeb_inline(d++, val);
	}

	return dst;
}

RealPt bc_memmove(RealPt dst, RealPt src, Bit16u len)
{
	PhysPt s, d;

	s = Real2Phys(src);
	d = Real2Phys(dst);

	while (len--) {
		mem_writeb_inline(d++, mem_readb_inline(s++));
	}

	return dst;
}

RealPt bc_memcpy(RealPt dst, RealPt src, Bit16u len)
{
	return bc_memmove(dst, src, len);
}

Bit16s bc__creat(RealPt name, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(name);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2f25);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

void bc_perror(RealPt __s)
{
	CPU_Push32(__s);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3685);
	CPU_Pop32();
}

Bit16s bc__open(RealPt fname, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_game + 0, 0x34c7);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

void bc_qsort(RealPt base, Bit16u nmemb, Bit16u size, RealPt fcmp)
{
	CPU_Push32(fcmp);
	CPU_Push16(size);
	CPU_Push16(nmemb);
	CPU_Push32(base);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3d74);
	CPU_Pop32();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop32();
}

Bit32s bc__write(Bit16u fd, RealPt buf, Bit16u len)
{
	CPU_Push16(len);
	CPU_Push32(buf);
	CPU_Push16(fd);
	CALLBACK_RunRealFar(reloc_game + 0, 0x4a85);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

Bit16s bc_OvrInitEms(Bit16u __emsHandle, Bit16u __emsFirst, Bit16u __emsPages)
{
	CPU_Push16(__emsPages);
	CPU_Push16(__emsFirst);
	CPU_Push16(__emsHandle);
	CALLBACK_RunRealFar(reloc_game + 0x1112, 0x8ed);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();

	return reg_ax;
}

}
