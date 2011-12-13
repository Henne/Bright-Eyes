#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "SDL.h"

#include "regs.h"

#include "schick.h"

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg024.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg032.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg044.h"
#include "seg046.h"
#include "seg047.h"
#include "seg050.h"
#include "seg063.h"
#include "seg068.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg100.h"
#include "seg101.h"
#include "seg102.h"
#include "seg103.h"
#include "seg105.h"
#include "seg120.h"

using namespace M302de;

/* dice table */
static char dice_tab[4] = {6, 20, 3, 4};

static SDL_TimerID irq_timer;
/* buffer for seed2 code */
static char irq_bak[17];

static Uint32 schick_irq_timer(Uint32 interval, void *param)
{
	/* inc seed2 */
	Bit16s seed2 = ds_readw(0xc3bf);
	ds_writew(0xc3bf, ++seed2);

	if (seed2 < 0) {
		D1_LOG("seed2 set to 0\n");
		ds_writew(0xc3bf, 0);
	}
	return interval;
}

void schick_timer_enable()
{
	/* get the adress of the seed2 code */
	Bit8u *loc = MemBase + PhysMake(reloc_game + 0xb2a, 0x261);

	/* copy seed2 code part */
	memcpy(irq_bak, loc, 17);
	/*replace it with NOPs */
	memset(loc, 0x90, sizeof(char));

	irq_timer = SDL_AddTimer(56, schick_irq_timer, NULL);

	if (irq_timer == NULL) {

		D1_ERR("Konnte den IRQ Timer nicht initialisieren\n");

		/* restore rand 2 code */
		memcpy(loc, irq_bak, 17);
	}

	D1_INFO("IRQ timer aktiviert\n");
}

void schick_timer_disable()
{
	SDL_RemoveTimer(irq_timer);
	irq_timer = NULL;

	/* restore rand 2 code */
	memcpy(MemBase + PhysMake(reloc_game + 0xb2a, 0x261), irq_bak, 17);
	memset(irq_bak, 0, 17);
	D1_INFO("IRQ timer deaktiviert\n");
}

/**
 *	get_ovrseg - returns segment of an overlay segment
 *	@stub_seg:	segment of the overlay stub
 *
 * Borland uses a technique called overlay to load code on demand.
 * At runtime you have a small stub segment where farcalls to this
 * segment are directed to. If the segment is not in memory an
 * interrupt 0x3f is generated, the code is loaded from the binarym
 * and the stub is ajusted with far jumps to the corrosponding funcs.
 */
static int get_ovrseg(unsigned short stub_seg) {
	Bit8u *p = MemBase + (reloc_game<<4) + (stub_seg<<4);

	if (host_readw(p) != 0x3fcd) {
		D1_ERR("Error: %x is not an overlay segment\n", stub_seg);
		return 0;
	}
	if (host_readw(p + 0x20) == 0x3fcd) {
	//	D1_ERR("Error: %x is not in memory\n", stub_seg);
		return 0;
	}
	if (host_readb(p + 0x20) != 0xea) {
		D1_ERR("No farjump in overlay segment\n");
		return 0;
	}
	return host_readw(p + 0x23);
}

static int is_ovrseg(unsigned short stub_seg) {
	return SegValue(cs) == get_ovrseg(stub_seg);
}



/* Borland C++ runtime */
static int seg000(unsigned short offs) {

	switch (offs) {
		/* nullsub */
		case 0x2c9: {
			return 0;
		}
		case 0x2f7: {
			/* int chdir(const char* __path) */
			RealPt path = CPU_Pop32();
			CPU_Push32(path);

			D1_LOG("chdir(%s)\n", (char*)MemBase+Real2Phys(path));

			return 0;
		}
		case 0x30f: {
			return 0;
		}
		/* close() */
		case 0x31b: {
			return 0;
		}
		/* open() */
		case 0x61e: {
			return 0;
		}
		/* read() */
		case 0x654: {
			return 0;
		}
		case 0x678: {
			return 0;
		}
		case 0x6d0: {
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);

			D1_LOG("C-Lib exit(%d)\n", val);
			return 0;
		}
		case 0x6df: {
			/* Not Called from far */
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);

			D1_LOG("_exit(%d)\n", val);
			return 0;
		}
		case 0x70b: {
			unsigned long retval;
			retval = (reg_dx << 16 | reg_ax) * (reg_cx << 16 | reg_bx);
			D1_LOG("Mul unsigned long %u * %u = %lu\n",
				reg_dx << 16 | reg_ax,
				reg_cx << 16 | reg_bx,
				retval);
			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;
		}
		/* struct copy*/
		case 0x722: {
			RealPt src = CPU_Pop32();
			RealPt dst = CPU_Pop32();
			CPU_Push32(dst);
			CPU_Push32(src);

			D1_LOG("F_SCOPY()");
			memcpy(Real2Host(dst), Real2Host(src), reg_cx);
			RET(8);
			return 1;
		}
		/* getcurdir() */
		case 0x73e: {
			 return 0;
		}
		/* getdisk() */
		case 0x781: {
			return 0;
		}
		/* setdisk() */
		case 0x79b: {
			return 0;
		}
		/* dos_getdiskfree() */
		case 0x7ed: {
			return 0;
		}
		case 0x816: {
			unsigned short interruptno = CPU_Pop16();
			CPU_Push16(interruptno);

			D1_LOG("_dos_getvect(int=0x%x)\n", interruptno);

			return 0;
		}
		case 0x825: {
			unsigned short interruptno = CPU_Pop16();
			RealPt isr = CPU_Pop32();
			CPU_Push32(isr);
			CPU_Push16(interruptno);

			D1_LOG("_dos_setvect(int=0x%x, *isr=0x%x:0x%x)\n",
				interruptno,
				(unsigned short)(RealSeg(isr) - reloc_game),
				RealOff(isr));
			return 0;
		}
		case 0x839: {
			return 0;
		}
		case 0x840: {
			D1_LOG("Div unsigned long\n");
			return 0;
		}
		case 0x848: {
			return 0;
		}
		case 0x850: { //not called
			D1_LOG("Mod unsigned long\n");
			return 0;
		}
		case 0x8e7: {
			return 0;
		}
		/* rshift() */
		case 0x908: {
			return 0;
		}
		case 0x928: {
			return 0;
		}
		case 0x9b0: {
			return 0;
		}
		case 0xa10: {
			return 0;
		}
		case 0xb33: {
			Bit16u handle = CPU_Pop16();
			Bit32u pos = CPU_Pop32();
			Bit16u whence = CPU_Pop16();
			CPU_Push16(whence);
			CPU_Push32(pos);
			CPU_Push16(handle);

			Bit32s retval = bc_lseek(handle, pos, whence);

			D1_LOG("C-Lib lseek(Handle=0x%x, pos=%u, whence=%d)\n",
				handle, pos, whence);

			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;
		}
		/* mkdir() */
		case 0xb5c: {
			return 0;
		}
		case 0xbac: {
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);

			D1_LOG("C-Lib srand(%d)\n", val);
			return 0;
		}
		case 0xbbd: {
			D1_LOG("rand()\n");
			return 0;
		}
		case 0x0be3: {
			Bit16u handle = CPU_Pop16();
			RealPt buf = CPU_Pop32();
			Bit16u count = CPU_Pop16();
			CPU_Push16(count);
			CPU_Push32(buf);
			CPU_Push16(handle);

			D1_LOG("C-Lib _read(handle=%d, buffer=0x%x, len=%d)\n",
				handle, buf, count);

			reg_ax = bc__read(handle, Real2Host(buf), count);

			return 1;
		}
		case 0x1123: {
			/* time(), user for randomize */
			unsigned int time = CPU_Pop32();
			CPU_Push32(time);

			D1_LOG("C-Lib time(0x%04x)\n", time);

			return 0;
		}
		case 0x117b: {
			signed short c = CPU_Pop16();
			CPU_Push16(c);
			reg_ax = toupper(c);
			D1_LOG("toupper(%c) == %c\n", c, reg_ax);
			return 1;
		}
		/* delete() */
		case 0x11a7: {
			return 0;
		}
		case 0x176d: {
			signed short cmd = CPU_Pop16();
			CPU_Push16(cmd);

			reg_ax = bioskey(cmd);
			D1_LOG("bioskey(%d);\n", (char) cmd);

			return 1;
		}
		case 0x1792: {
			return 0;
		}
		case 0x1e55: {
			RealPt ptr = CPU_Pop32();
			CPU_Push32(ptr);

			D1_LOG("free(0x%04x:0x%04x)\n",
				RealSeg(ptr), RealOff(ptr));
			return 0;
		}
		case 0x1f69: {
			unsigned int size = CPU_Pop32();
			CPU_Push32(size);

			D1_LOG("farmalloc(%d)\n", size);
			return 0;
		}
		case 0x2287: {
			Bit32u nmem = CPU_Pop32();
			Bit32u size = CPU_Pop32();
			RealPt p;

			p = bc_farcalloc(nmem, size);
			D1_LOG("calloc(%d, %d) = %08x\n", nmem, size, p);


			CPU_Push32(size);
			CPU_Push32(nmem);

			reg_ax = RealOff(p);
			reg_dx = RealSeg(p);

			return 1;
		}
		case 0x2315: {
			return 0;
		}
		case 0x2411: {
			return 0;
		}
		case 0x2596: {
			return 0;
		}
		case 0x2d82: {
			unsigned short i=4;
			unsigned short off=real_readw(SegValue(ss), reg_sp+i);
			unsigned short seg=real_readw(SegValue(ss), reg_sp+i+2);

			D1_LOG("C-Lib exec?(\"%s\", ",
					MemBase+(seg<<4)+off);
			/*
			do {
				i+=4;
				off=real_readw(SegValue(ss), reg_sp+i);
				seg=real_readw(SegValue(ss), reg_sp+i+2);
				if ((seg<<4)+off > 0)
						D1_LOG("\"%s\", ",
						MemBase+(seg<<4)+off);
					else
						D1_LOG("\"%s\")\n",
						MemBase+(seg<<4)+off);
			} while ((seg<<4)+off > 0);
			*/
			return 0;
		}
		case 0x2dff: {
			/* long atol(const char* s) */
			RealPt s = CPU_Pop32();
			CPU_Push32(s);

			int val;

			val = atoi(getString(s));
			D1_LOG("atol(\"%s\") = %d\n", getString(s), val);

			reg_ax = val & 0xffff;
			reg_dx = (val>>16) & 0xffff;

			return 1;
		}
		case 0x2eb2: {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = bc_close(handle);
			D1_LOG("C-Lib close(%d); = %d\n",
				handle, (signed short)reg_ax);

			return 1;
		}
		case 0x2eda: {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = bc__close(handle);
			D1_LOG("C-Lib _close(%d); = %d\n",
				handle, (signed short)reg_ax);

			return 1;
		}
		case 0x2f25: {
			RealPt name = CPU_Pop32();
			Bit16u attrib = CPU_Pop16();
			D1_INFO("C-Lib bc__creat(%s, %x);\n",
				(char*)Real2Host(name), attrib);
			reg_ax = bc__creat(name, attrib);
			CPU_Push16(attrib);
			CPU_Push32(name);
			return 1;
		}
		case 0x3040: {
			return 0;
		}
		case 0x3073: {
			return 0;
		}
		case 0x30a0: {
			return 0;
		}
		case 0x3350: {
			/* char* itoa(int __value, char* string, int radix);
			radix is everytime 10 in this game*/

			short value = CPU_Pop16();
			RealPt string = CPU_Pop32();
			short radix = CPU_Pop16();
			CPU_Push16(radix);
			CPU_Push32(string);
			CPU_Push16(value);

			D1_LOG("itoa(%d, 0x%04x:0x%04x, %d)\n",
					value, RealSeg(string),
					RealOff(string), radix);
			sprintf((char*)MemBase+Real2Phys(string), "%d", value);

			reg_ax = RealOff(string);
			reg_dx = RealSeg(string);
			return 1;
		}
		case 0x33c0: {
			/*void *memcpy(void *dest, const void *src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("memcpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			mem_memcpy(Real2Phys(dest), Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		case 0x3408: {
			/*void* memset(void *s, int c, size_t n)*/
			RealPt s = CPU_Pop32();
			short c = CPU_Pop16();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push16(c);
			CPU_Push32(s);

			D1_LOG("memset(0x%04x:0x%04x, 0x%02x, %u)\n",
					RealSeg(s), RealOff(s), c, n);
			// No bypass on graphic memory
			if (RealSeg(s) == 0xa000)
				return 0;

			memset(MemBase+Real2Phys(s), c, n);

			reg_ax = RealOff(s);
			reg_dx = RealSeg(s);
			return 1;
		}
		case 0x3479: {
			/* write(handle) */
			unsigned short handle = CPU_Pop16();
			CPU_Push16(handle);

			D1_LOG("write_0(%d)\n", handle);
			return 0;
		}
		case 0x34c7: {
			/*open()*/
			RealPt fname = CPU_Pop32();
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);
			CPU_Push32(fname);

			D1_LOG("open(\"%s\",\"%04x\")\n",
					MemBase + Real2Phys(fname), mode);
			return 0;
		}
		case 0x3636: {
			/* sortof open() */
			RealPt fname = CPU_Pop32();
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);
			CPU_Push32(fname);

			D1_LOG("C-Lib Unkn(\"%s\", 0x%04x)\n",
					MemBase + Real2Phys(fname), mode);
			return 0;
		}
		case 0x36dd: {
			/*printf()*/
			RealPt p = CPU_Pop32();
			CPU_Push32(p);

			D1_LOG("printf(\"%s\")\n", MemBase + Real2Phys(p));
			return 0;
		}
		case 0x3d74: {
			return 0;
		}
		/* ret 0x000a */
		case 0x41d2: {
			RealPt str = CPU_Pop32();
			RealPt format = CPU_Pop32();
			CPU_Push32(format);
			CPU_Push32(str);

			D1_LOG("C-Lib sprintf(0x%04x:0x%04x, \"%s\", ...)\n",
					RealSeg(str), RealOff(str),
					MemBase + Real2Phys(format));

			return 0;
		}
		case 0x4215: {
			/*char *strcat(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcat(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcat((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		case 0x4254: {
			/*int strcmp(const char* s1, const char* s2)*/
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("strcmp(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(s1), RealOff(s1),
					RealSeg(s2), RealSeg(s2));

			reg_ax = strcmp((char*)MemBase + Real2Phys(s1),
					(char*)MemBase + Real2Phys(s2));

			return 1;
		}
		case 0x4284: {
			/*char *strcpy(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcpy(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		case 0x42ad: {
			/*strlen()*/
			RealPt str = CPU_Pop32();
			CPU_Push32(str);

			D1_LOG("strlen(\"%s\")\n", getString(str));
			reg_ax = strlen(getString(str));

			return 1;
		}
		case 0x42cc: {
			/*char *strncpy(char* dest, const char* src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strncpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			strncpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		case 0x462b: {
			return 0;
		}
		case 0x4a85: {
			/*write()*/
			Bit16u handle = CPU_Pop16();
			RealPt buf = CPU_Pop32();
			Bit16u val = CPU_Pop16();

			Bit32s retval;

			retval = bc__write(handle, buf, val);

			CPU_Push16(val);
			CPU_Push32(buf);
			CPU_Push16(handle);

			D1_LOG("C-Lib __write(handle=0x%x, buffer=0x%x:0x%x, len=%d) = %d\n",
				handle, RealSeg(buf), RealOff(buf),
				val, retval);

			reg_dx = 0;
			reg_ax = retval & 0xffff;

			return 1;
		}
		case 0x4a88: {
			return 0;
		}
		default: {
			D1_TRAC("\t\tC-Lib:0x%x\n", offs);
			return 0;
		}
	}
}

static int seg001(unsigned short offs) {
	switch (offs) {
	case 0x2c4: {
		D1_LOG("%s:%x()\n", __func__, offs);
		seg001_02c4();
		return 1;
	}
	case 0x30e: {
		unsigned short val = CPU_Pop16();
		CPU_Push16(val);
		reg_ax = CD_bioskey(val);
		D1_LOG("CD_bioskey(%x) = %x\n", val, reg_ax);
		return 1;
	}
	case 0x34f: {
		D1_LOG("%s:%x()\n", __func__, offs);
		seg001_034f();
		return 1;
	}
	case 0x37a: {
		/* CDA off */
		CD_audio_pause();
		D1_LOG("CD_audio_pause()\n");
		return 1;
	}
	case 0x3d5: {
		/* CDA on */
		CD_audio_play();
		D1_LOG("CD_audio_play()\n");
		return 1;
	}
	case 0x4f4: {
		/* CDA set track */
		D1_LOG("%s:%x()\n", __func__, offs);
		return 0;
	}
	case 0x681: {
		/* check if DSA1 CD is in drive */
		D1_LOG("%s:%x()\n", __func__, offs);
		return 0;
	}
	case 0x6c6: {
		/* init CD drive */
		D1_LOG("%s:%x()\n", __func__, offs);
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		return 0;
	}
}

static int seg002(unsigned short offs) {
	switch (offs) {

	case 0x0017: {
		unsigned short track = CPU_Pop16();
		CPU_Push16(track);

		D1_LOG("set_cda_track(%d);\n", track);
		return 0;
	}
	case 0x0045:	/* wird bei Musikmenu aufgerufen */
	case 0x00e6:
	case 0x01e0:
	case 0x0209:
		return 0;
	case 0x06fe:
	case 0x079f:
		return 0;
	case 0x0832: {
		Bit16u index = CPU_Pop16();
		D1_LOG("play_voc(FX%d.VOC)\n", index - 288);
		play_voc(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0856:	/* Betrunken */
	case 0x0890:
		return 0;
	case 0x0c0e: {
		short index = CPU_Pop16();
		CPU_Push16(index);

		unsigned int retval = get_readlength2(index);
		D1_LOG("get_readlength2(%d) = %d\n", index, retval);

		reg_ax = retval & 0xffff;
		reg_dx = (retval>>16) & 0xffff;

		return 1;
	}
	case 0x0c28: {
		Bit16u handle = CPU_Pop16();
		RealPt buf = CPU_Pop32();
		Bit16u len = CPU_Pop16();
		CPU_Push16(len);
		CPU_Push32(buf);
		CPU_Push16(handle);

		D1_LOG("read_archive_file(%d, %x, %d);\n",
			handle, buf, len);
		reg_ax = read_archive_file(handle,
				Real2Host(buf), len);

		return 1;
	}
	case 0x0c72: {
		Bit16u handle = CPU_Pop16();
		Bit32u off = CPU_Pop32();
		CPU_Push32(off);
		CPU_Push16(handle);

		D1_LOG("seg002_0c72(%d, %d)\n", handle, off);
		seg002_0c72(handle, off);

		return 1;
	}
	case 0x0cb6: {
		Bit16u index = CPU_Pop16();

		reg_ax = load_regular_file(index);

		D1_LOG("load_regular_file(0x%x / %s) = %x\n",
			index, get_fname(index), reg_ax);

		CPU_Push16(index);

		return 1;
	}
	case 0x0d27: {
		Bit16u index = CPU_Pop16();

		reg_ax = load_archive_file(index);

		D1_LOG("load_archive_file(0x%x / %s) = %x\n",
			index, get_fname(index), reg_ax);

		CPU_Push16(index);

		return 1;
	}
	case 0x0ed2: {
			RealPt nvf = CPU_Pop32();
			CPU_Push32(nvf);

			signed int retval;

			D1_LOG("process_nvf(0x%04x:0x%04x);\n",
				RealSeg(nvf), RealOff(nvf));

			retval = process_nvf(Real2Host(nvf));

			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;

	}
	case 0x1634: {
		/* Leaf Function */
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		unsigned short v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);


		reg_ax = is_mouse_in_rect(v1, v2, v3, v4);

		D1_LOG("is_mouse_in_rect(%d, %d, %d, %d); = %d \n",
			v1, v2, v3, v4, reg_ax);

		return 1;
	}
	case 0x165e:
	case 0x16fd:
		return 0;
	case 0x1802: {
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		D1_LOG("mouse_move_cursor(x=%d, y=%d);\n", x, y);
		return 1;
	}
	case 0x18b3: {
		RealPt p = CPU_Pop32();
		CPU_Push32(p);

		D1_LOG("make_ggst_cursor(%x)\n", Real2Phys(p));
		make_ggst_cursor(Real2Host(p));
		return 1;
	}
	case 0x1921:
		D1_LOG("update_mouse_cursor();\n");
		update_mouse_cursor();
		return 1;
	case 0x192b:
		D1_LOG("refresh_screen_size();\n");
		refresh_screen_size();
		return 1;
	case 0x1a34:
		return 0;
	case 0x1cf2: {
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(y);
		CPU_Push16(x);

		reg_ax = get_mouse_action(x, y, Real2Host(p));

		D1_LOG("get_mouse_action(x=%d, y=%d, p=%x); = %x\n",
			x, y, p, reg_ax);

		return 1;
	}
	case 0x1d67: {
		D1_LOG("handle_input();\n");
		handle_input();
		return 1;
	}
	case 0x1ecc: {
		D1_LOG("wait_for_keyboard1()\n");
		wait_for_keyboard1();
		return 1;
	}
	case 0x21ab: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		D1_LOG("pal_fade(%x,%x);\n", p1, p2);
		pal_fade(Real2Phys(p1), Real2Phys(p2));
		return 1;
	}
	case 0x232a: {
		RealPt dst = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		unsigned short v3 = CPU_Pop16();
		unsigned short colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(v3);
		CPU_Push32(p2);
		CPU_Push32(dst);

		D1_LOG("pal_fade_in(%x,%x,%x,%x);\n", dst, p2, v3, colors);

		pal_fade_in(Real2Phys(dst), Real2Phys(p2), v3, colors);

		return 1;
	}
	case 0x25ce: {
		reg_ax = get_current_season();
		return 1;
		}
	case 0x274e:
		return 0;
	case 0x2bf6: {
		unsigned int val = CPU_Pop32();
		CPU_Push32(val);

		D1_LOG("far sub_ingame_timers(val = %u);\n", val);
		sub_ingame_timers(val);
		return 1;
	}
	case 0x2c5e: {
		unsigned int val = CPU_Pop32();
		CPU_Push32(val);

		D1_LOG("sub_mod_timers(%d);\n", val);
		sub_mod_timers(val);
		return 1;
	}
	case 0x2e26: {
		reg_ax = get_free_mod_slot();
		D1_LOG("get_free_mod_slot() = %d\n", reg_ax);
		return 1;
	}
	case 0x2e69: {
		unsigned short slot_nr = CPU_Pop16();
		unsigned int timer = CPU_Pop32();
		RealPt ptr = CPU_Pop32();
		signed short mod16 = CPU_Pop16();
		signed short who16 = CPU_Pop16();
		CPU_Push16(who16);
		CPU_Push16(mod16);
		CPU_Push32(ptr);
		CPU_Push32(timer);
		CPU_Push16(slot_nr);

		signed char mod = (signed char)(mod16 & 0xff);
		signed char who = (signed char)(who16 & 0xff);

		D1_LOG("set_mod_slot(%d, %d, 0x%x, %+d, %d)\n",
			slot_nr, timer, ptr, mod, who);
		set_mod_slot(slot_nr, timer, Real2Host(ptr), mod, who);

		return 1;
	}
	case 0x2f7a: {
		unsigned int v1 = CPU_Pop32();
		CPU_Push32(v1);

		D1_LOG("seg002_2f7a(v1=%d);\n", v1);
		seg002_2f7a(v1);

		return 1;
	}
	case 0x3071: {
		unsigned short quarter = CPU_Pop16();
		signed short v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(quarter);

		D1_LOG("sub_light_timers(quarter=%d, v2=%d);\n", quarter, v2);
		sub_light_timers(quarter, v2);

		return 1;
	}
	case 0x3230:
	case 0x37c4:
			return 0;

	case 0x3b4f:  {
		D1_LOG("set_and_spin_lock()\n");
		return 1;
	}
	case 0x3ca6:
	case 0x3dbb:	/* Schiffsfahrt */
		return 0;
	case 0x3ebb: {
		D1_LOG("dec_splash();\n");
		dec_splash();
		return 1;
	}
	case 0x4016: {
		D1_LOG("wait_for_keyboard2()\n");
		wait_for_keyboard2();
		return 1;
	}
	case 0x404f: {
		D1_LOG("seg002_404f()\n");
		wait_for_keypress();
		return 1;
	}
	/* Callers: 25 */
	case 0x40d1: {
		Bit16u time = CPU_Pop16();
		delay_or_keypress(time);
		CPU_Push16(time);
		D1_LOG("delay_or_keypress(%d);\n", time);
		return 1;
	}
	case 0x41cd: {
		unsigned int v = CPU_Pop32();
		CPU_Push32(v);

		unsigned int retval;

		retval = swap_u32(v);

		reg_ax = retval & 0xffff;
		reg_dx = (retval >> 16) & 0xffff;
		return 1;
	}
	/* 3 EMS functions (all disabled in v3.02_de) */
	case 0x4253: {
		unsigned int bytes = CPU_Pop32();
		CPU_Push32(bytes);

		reg_ax = alloc_EMS(bytes);
		D1_LOG("alloc_EMS(%d) = %d\n", bytes, reg_ax);

		return 1;
	}
	case 0x428b:
	case 0x4339:
		return 0;
	case 0x43e7: {
		D1_LOG("set_to_ff()\n");
		set_to_ff();
		return 1;
	}
	case 0x43fd:
		return 0;
	case 0x4485: {
		short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = mod_timer(val);
		D1_LOG("mod_timer(%d) = %d\n", val, reg_eax);
		return 1;
	}
	case 0x44aa: {
		draw_compass();
		return 1;
	}
	case 0x4559: {

		reg_ax = can_merge_group();
		D1_LOG("can_merge_group() = %d\n", (short)reg_ax);

		return 1;
	}
	case 0x45db: {
		Bit16u val = CPU_Pop16();
		CPU_Push16(val);

		D1_LOG("div16(%d)\n", val);

		reg_ax = div16((unsigned char)val);

		return 1;
	}
	case 0x45ea: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		D1_LOG("select_with_mouse(%x,%x);\n", p1, p2);
		select_with_mouse(Real2Host(p1), Real2Host(p2));

		return 1;

	}
	case 0x4658: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		D1_LOG("select_with_keyboard(%x,%x);\n", p1, p2);
		select_with_keyboard(Real2Host(p1), Real2Host(p2));

		return 1;
	}
	case 0x4707: {
		/* Leaf Function - far & near */
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		D1_LOG("set_map_tile(%d, %d)\n", x, y);
		set_automap_tile(x, y);
		return 1;
	}
	case 0x472b: {
		/* No Leaf - far only, calls only 0x4707 */
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		D1_LOG("set_automap_tiles(%d, %d)\n", x, y);
		set_automap_tiles(x, y);
		return 1;
	}
	case 0x48b1: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = check_hero(Real2Host(hero));

		D1_LOG("check_hero(%s) = %d\n",
			schick_getCharname(hero), reg_ax);

		return 1;
	}
	case 0x49d8: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = is_hero_available_in_group(Real2Host(hero));
		D1_LOG("is_hero_available_in_group(%s) = %d\n",
			schick_getCharname(hero), reg_ax);

		return 1;
	}
	case 0x4a05: {
		RealPt hero = CPU_Pop32();
		signed short type = CPU_Pop16();
		CPU_Push16(type);
		CPU_Push32(hero);

		D1_LOG("sub_ae_splash(%s, %d);\n",
			schick_getCharname(hero), type);

		sub_ae_splash(Real2Host(hero), type);

		return 1;
	}
	case 0x4a87: {
		RealPt hero = CPU_Pop32();
		short val = CPU_Pop16();
		CPU_Push16(val);
		CPU_Push32(hero);

		add_hero_ae(Real2Host(hero), val);

		D1_LOG("add_hero_ae(%s, %d)\n",
				schick_getCharname(hero), val);

		return 1;
	}
	case 0x4adc:
		return 0;
	case 0x4df3: {
		RealPt hero = CPU_Pop32();
		signed short le = CPU_Pop16();
		CPU_Push16(le);
		CPU_Push32(hero);

		D1_INFO("%s erhaelt %d LE\n", schick_getCharname(hero), le);
		add_hero_le(Real2Host(hero), le);

		return 1;
	}
	case 0x4efb: {
		signed short le = CPU_Pop16();
		CPU_Push16(le);

		D1_INFO("Jeder Held der Gruppe erhaelt %d LE\n", le);
		add_group_le(le);

		return 1;
	}
	case 0x4ff9: {
		/* Eigenschaftsprobe */
		RealPt hero = CPU_Pop32();
		unsigned attrib = CPU_Pop16();
		signed bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(attrib);
		CPU_Push32(hero);

		reg_ax = test_attrib(Real2Host(hero),
				attrib, bonus);

		return 1;
	}
	case 0x504e: {
		/* Talent-/Zauber-Probe */
		unsigned hero = CPU_Pop32();
		unsigned short attrib1 = CPU_Pop16();
		unsigned short attrib2 = CPU_Pop16();
		unsigned short attrib3 = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(attrib3);
		CPU_Push16(attrib2);
		CPU_Push16(attrib1);
		CPU_Push32(hero);

		reg_ax = test_attrib3(Real2Host(hero),
				attrib1, attrib2, attrib3, (signed char)bonus);

		return 1;
	}
	case 0x515e: {
		reg_ax = get_random_hero();

		D1_LOG("get_random_hero(); -> %s\n",
			schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da));
		return 1;
	}
	case 0x51c2: {
		unsigned int money;

		money = get_party_money();
		D1_INFO("Aktuelles Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);

		reg_ax = money & 0xffff;
		reg_dx = (money>>16) & 0xffff;

		return 1;
	}
	case 0x5221: {
		unsigned int money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Setze Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);
		set_party_money(money);

		return 1;
	}
	case 0x5331: {
		unsigned int money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Aendere Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);
		add_party_money(money);
		return 1;
	}
	case 0x5349: {
		RealPt hero = CPU_Pop32();
		int ap = CPU_Pop32();
		CPU_Push32(ap);
		CPU_Push32(hero);

		D1_INFO("%s erhaelt %d AP\n",
					schick_getCharname(hero), ap);
		add_hero_ap(MemBase+Real2Phys(hero), ap);

		return 1;
	}
	case 0x535f: {
		/* APs verteilen */
		signed int group_ap = CPU_Pop32();
		CPU_Push32(group_ap);

		D1_INFO("Gruppe erhaelt %d AP\n", group_ap);
		add_group_ap(group_ap);

		return 1;
	}
	case 0x53e8: {
		signed short ap = CPU_Pop16();
		CPU_Push16(ap);

		D1_INFO("Jeder Held erhaelt %+d AP\n", ap);
		add_hero_ap_all(ap);

		return 1;
	}
	case 0x5452: {
		signed short ap = CPU_Pop16();
		CPU_Push16(ap);

		D1_INFO("Jeder Held erhaelt %+d\n", ap);
		sub_hero_ap_all(ap);

		return 1;
	}
	case 0x54e9: {
		/* unsigned short get_hero_index(hero_ptr *hero); */
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = get_hero_index(MemBase+Real2Phys(hero));
		D1_LOG("get_hero_index(%s) = (%d)\n",
				schick_getCharname(hero), reg_ax);
		return 1;
	}
	case 0x5520: {
		/* int get_item_pos(hero_ptr *hero, unsigned short item)*/
		RealPt hero = CPU_Pop32();
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);
		CPU_Push32(hero);

		reg_ax = get_item_pos(Real2Host(hero), item);

		D1_LOG("get_item_pos(%s, %s = (0x%d)) = %d\n",
					schick_getCharname(hero),
					get_itemname(item),
					item, (short)reg_ax);
		return 1;
	}
	case 0x554c: {
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);

		reg_ax = get_first_hero_with_item(item);
		D1_LOG("get_first_hero_with_item(%s = (%d)) = %d\n",
			get_itemname(item), item, (signed short)reg_ax);

		return 1;
	}
	case 0x5615:	/* Krakenangriff */
			return 0;
	case 0x5667: {
		RealPt retval = get_first_hero_available_in_group();

		D1_LOG("get_first_hero_available_in_group() = %s\n",
			schick_getCharname(retval));

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	case 0x56d6: {
		RealPt retval = get_second_hero_available_in_group();

		D1_LOG("get_second_hero_available_in_group() = %s\n",
			schick_getCharname(retval));

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	case 0x573e: {
		reg_ax = count_heros_available();
		D1_LOG("count_heros_available() = %d\n", reg_ax);
		return 1;
	}
	case 0x5799: {
		reg_ax = count_heroes_available_in_group();
		D1_LOG("count_heroes_available_in_group() = %d\n", reg_ax);
		return 1;
	}
	case 0x5816: {
		unsigned short argc = CPU_Pop16();
		CPU_Push16(argc);
		D1_LOG("main(argc=0x%04x, ...)\n", argc);
		return 0;
	}
	case 0x5a68: {
		Bit32u size = CPU_Pop32();

		RealPt p = schick_alloc_emu(size);
		D1_LOG("schick_alloc_emu(%d)\n", size);

		CPU_Push32(size);

		reg_ax = RealOff(p);
		reg_dx = RealSeg(p);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		return 0;
	}
}

static int seg003(unsigned short offs) {
	switch (offs) {
		case  0x000c: {
			unsigned short mod = CPU_Pop16();
			CPU_Push16(mod);

			D1_LOG("update_direction(%d)\n", mod);

			reg_ax = update_direction((unsigned char)mod);
			return 1;
		}
		case  0x002c: {
			return 0;
		}
		case  0x01e1: {
			return 0;
		}
		case  0x031e: {
			return 0;
		}
		case  0x0424: {
			D1_LOG("no_way()\n");
			no_way();
			return 1;
		}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg004(unsigned short offs) {
	switch (offs) {
	case 0x55: {
		Bit16u v1 = CPU_Pop16();

		D1_LOG("init_ani(%d)\n", v1);
		init_ani(v1);

		CPU_Push16(v1);
		return 1;
	}
	case 0x134:
		D1_LOG("set_var_to_zero()\n");
		set_var_to_zero();
		return 1;
	case 0x15b:
		D1_LOG("clear_ani()\n");
		clear_ani();
		return 1;
	case 0x8e9: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		unsigned short v4 = CPU_Pop16();
		unsigned short v5 = CPU_Pop16();
		CPU_Push16(v5);
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("draw_bar(%d,%d,%d,%d,%d)\n", v1, v2, v3, v4, v5);
		draw_bar(v1, v2, v3, v4, v5);
		return 1;
	}
	case 0xa39: {
		RealPt ptr1 = CPU_Pop32();
		RealPt ptr2 = CPU_Pop32();
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		unsigned short n = CPU_Pop16();
		unsigned short m = CPU_Pop16();
		CPU_Push16(m);
		CPU_Push16(n);
		CPU_Push16(x);
		CPU_Push16(y);
		CPU_Push32(ptr2);
		CPU_Push32(ptr1);

		D1_LOG("restore_rect(%d %d %d %d)\n", x, y , n, m);
		restore_rect(Real2Phys(ptr1), Real2Host(ptr2), x, y,
			(char)n, (char)m);
		return 1;
	}
	case 0xaa4: {
		RealPt ptr1 = CPU_Pop32();
		RealPt ptr2 = CPU_Pop32();
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		unsigned short w = CPU_Pop16();
		unsigned short h = CPU_Pop16();
		unsigned short v1 = CPU_Pop16();
		CPU_Push16(v1);
		CPU_Push16(h);
		CPU_Push16(w);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(ptr2);
		CPU_Push32(ptr1);

		D1_LOG("restore_rect_rle(%d %d %d %d %d)\n", x, y , w, h, v1);

		restore_rect_rle(Real2Phys(ptr1), Real2Host(ptr2),
			x, y, (char)w, (char)h, v1);
		return 1;
	}
	case 0xb6b:
		D1_LOG("draw_mouse_cursor()\n");
		draw_mouse_cursor();
		return 1;
	case 0xc26:
		D1_LOG("save_mouse_bg()\n");
		save_mouse_bg();
		return 1;
	case 0xcb1:
		D1_LOG("restore_mouse_bg()\n");
		restore_mouse_bg();
		return 1;
	case 0x1147: {
		RealPt dst = CPU_Pop32();
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(dst);

		D1_LOG("array_add(0x%04x:0x%04x, len=%d, op=%d, flag=%d);\n",
			RealSeg(dst), RealOff(dst), v1, (char)v2, v3);
		array_add(Real2Phys(dst), v1, (char)v2, v3);
		return 1;
	}
	case 0x1190:
		D1_LOG("schick_set_video()\n");
		schick_set_video();
		return 1;
	case 0x11c1:
		D1_LOG("schick_reset_video()\n");
		schick_reset_video();
		return 1;
	case 0x11da: {
		D1_LOG("clear_ani_pal()\n");
		clear_ani_pal();
		return 1;
	}
	case 0x1209: {
		RealPt pal = CPU_Pop32();
		D1_LOG("set_ani_pal()\n");
		set_ani_pal(Real2Host(pal));
		CPU_Push32(pal);
		return 1;
	}
	case 0x12e8: {
		RealPt dst = CPU_Pop32();
		unsigned short x = CPU_Pop16();
		unsigned short y1 = CPU_Pop16();
		unsigned short y2 = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(y2);
		CPU_Push16(y1);
		CPU_Push16(x);
		CPU_Push32(dst);

		D1_LOG("do_v_line(0x%04x:0x%04x, %d, %d, %d, 0x%02x);\n",
			RealSeg(dst), RealOff(dst), x, y1, y2,
			(unsigned char)color);
		do_v_line(Real2Phys(dst), x, y1, y2, (unsigned char)color);
		return 1;
	}
	case 0x1342: {
		RealPt dst = CPU_Pop32();
		unsigned short x1 = CPU_Pop16();
		unsigned short x2 = CPU_Pop16();
		unsigned short y1 = CPU_Pop16();
		unsigned short y2 = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(y2);
		CPU_Push16(y1);
		CPU_Push16(x2);
		CPU_Push16(x1);
		CPU_Push32(dst);

		D1_LOG("do_border(0x%04x:0x%04x, %d, %d, %d, %d, 0x%02x);\n",
			RealSeg(dst), RealOff(dst), x1, x2, y1, y2,
			(unsigned char)color);
		do_border(Real2Phys(dst), x1, x2, y1, y2, (unsigned char)color);
		return 1;
	}
	case 0x13b7: {
		unsigned short mode = CPU_Pop16();
		CPU_Push16(mode);
		D1_LOG("do_pic_copy(%d);\n", mode);
		do_pic_copy(mode);
		return 1;
	}
	case 0x144c:
		D1_LOG("do_save_rect()\n");
		do_save_rect();
		return 1;
	case 0x14bd: {
		RealPt dst = CPU_Pop32();
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		unsigned short w = CPU_Pop16();
		unsigned short h = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(h);
		CPU_Push16(w);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(dst);

		D1_LOG("do_fill_rect(%d);\n", color);
		do_fill_rect(dst, x, y, w, h, color);
		return 1;
	}
	case 0x150d: {
		D1_LOG("wait_for_vsync()\n");
		wait_for_vsync();
		return 1;
	}

	default:
		return 0;
	}
}

static int seg005(unsigned short offs) {
	/* seg005 has only one func called by far */
	switch (offs) {
		case 0x598: {
			Bit16u val = CPU_Pop16();
			D1_LOG("seg005_0598(%d);\n", val);
			CPU_Push16(val);
//			seg005_0598(val);
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg006(unsigned short offs) {
	switch (offs) {
		case 0x00e: {
			unsigned short v1 = CPU_Pop16();
			CPU_Push16(v1);

			RealPt retval;
			retval = FIG_get_ptr((char)v1);
			D1_LOG("FIG_get_ptr(%d); = 0x%x\n", v1, retval);

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
		}
		case 0x07f:
			D1_LOG("seg006_07f()\n");
			return 0;
		case 0x236:
			D1_LOG("FIG_set_gfx()\n");
			FIG_set_gfx();
			return 1;
		case 0x29e:
			D1_LOG("FIG_do_draw_pic()\n");
			return 0;
		case 0x2a8:
			D1_LOG("FIG_draw_pic();\n");
//			FIG_draw_pic();
			return 0;
		case 0x2fa: {
			unsigned short v1 = CPU_Pop16();
			CPU_Push16(v1);

			RealPt retval;
			retval = FIG_get_hero_ptr((char)v1);
			D1_LOG("FIG_get_hero_ptr(%d); = 0x%x\n", v1, retval);

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
		}
		case 0x33c: {
			short v = CPU_Pop16();
			CPU_Push16(v);

			RealPt retval = seg006_033c(v);
			D1_LOG("seg006_33c(0x%x); = %x\n", v , retval);

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);

			return 1;
		}
		case 0x36c: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			FIG_set_0e((char)v1, (char)v2);
			D1_LOG("FIG_set_0e(%d, %d)\n", (char)v1, (char)v2);
			return 1;
		}
		case 0x3bb: {
			unsigned short id = CPU_Pop16();
			CPU_Push16(id);

			FIG_reset_12_13((char)id);
			D1_LOG("FIG_reset_12_13(%d)\n", (char)id);
			return 1;
		}
		case 0x443: {
			unsigned short id = CPU_Pop16();
			CPU_Push16(id);

			FIG_set_12_13((char)id);
			D1_LOG("FIG_set_12_13(%d)\n", (char)id);
			return 1;
		}
		case 0x4cb: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			FIG_set_0f((char)v1, (char)v2);
			D1_LOG("FIG_set_0f(%d, %d)\n", (char)v1, (char)v2);
			return 1;
		}
		case 0x512: {
			unsigned short id = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(id);

			D1_LOG("FIG_remove_from_list(%d, %d)\n",
				(char)id, (char)v2);
			FIG_remove_from_list((char)id, (char)v2);
			return 1;
		}
		case 0x637: {
			unsigned short v16 = CPU_Pop16();
			CPU_Push16(v16);

			signed char v = (signed char)(v16 & 0xff);

			D1_LOG("FIG_add_to_list(%d)\n", v);

			reg_ax = FIG_add_to_list(v);

			D1_LOG("returns (%d)\n", (signed char)reg_ax);

			return 1;
		}
		case 0x82b: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_LOG("FIG_draw_char_pic(%d, %d)\n", v1, v2);
			FIG_draw_char_pic(v1, v2);
			return 1;
		}
		case 0x99f: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_LOG("FIG_draw_enemy_pic(%d, %d)\n", v1, v2);
			//FIG_draw_enemy_pic(v1, v2);
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg007(unsigned short offs) {

		if (offs == 0x000b) {
			unsigned lo = CPU_Pop16();
			unsigned hi = CPU_Pop16();
			CPU_Push16(hi);
			CPU_Push16(lo);

			reg_ax = random_interval(lo, hi);

			D1_INFO("randomInterval %d - %d : %d\n",
				lo, hi, reg_ax);

			return 1;
		}
		if (offs == 0x002b) {
			signed short p1 = CPU_Pop16();
			CPU_Push16(p1);

			reg_ax = random_schick(p1);

			D1_INFO("random(%d) = %d\n", p1, reg_ax);

			return 1;
		}
		if (offs == 0x007a) {
			unsigned n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			signed short x = CPU_Pop16();
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			reg_ax = dice_roll(n, m, x);

			D1_INFO("wuerfel %dW%d%+d = %d\n",
				n, m, x, reg_ax);

			return 1;
		}
		if (offs == 0x00c4) {
			unsigned short val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_word_array(val, Real2Host(p));

			D1_LOG("is_in_word_array(0x%x, 0x%04x:0x%04x) = %d\n",
				val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00ef) {
			unsigned val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_byte_array((char)val, MemBase+Real2Phys(p));
			D1_LOG("is_in_byte_array(0x%x, 0x%04x:0x%04x) = %d\n",
				(char)val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00a0) {
			signed n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			signed x = CPU_Pop16();
			RealPt pmin = CPU_Pop32();
			RealPt pmax = CPU_Pop32();
			CPU_Push32(pmax);
			CPU_Push32(pmin);
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			calc_damage_range(n, m, x, MemBase+Real2Phys(pmin),
				MemBase+Real2Phys(pmax));

			D1_LOG("calc_damage_range(%d, %d, %d)\n", n, m, x);

			return 1;
		}
		if (offs == 0x0119) {
		        unsigned val = CPU_Pop16();
		        CPU_Push16(val);

			unsigned short m = ((val & 0x0f00) >> 8) - 1;
			if (m > 3)
				m = 3;

			m = dice_tab[m];

			reg_ax = dice_template(val);

		        D1_INFO("Wuerfel %dW%d%+d = %d\n",
				(val & 0xf000) >> 12, m,
				(signed char)(val & 0xff), (short)reg_ax);

			return 1;
		}
		if (offs == 0x0186) {
			unsigned short val = CPU_Pop16();
			RealPt min = CPU_Pop32();
			RealPt max = CPU_Pop32();
			CPU_Push32(max);
			CPU_Push32(min);
			CPU_Push16(val);

			damage_range_template(val, Real2Host(min),
				Real2Host(max));

			D1_LOG("damage_range_template() Untested\n");

			return 1;
		}
	D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
	exit(1);
	return 0;
}

static int seg008(unsigned short offs) {

	switch (offs) {

	case 0x8: {
		unsigned short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = swap_u16(val);
		D1_GFX("swap_u16(val=0x%04x); = 0x%04x\n", val, reg_ax);

		return 1;
	}
	case 0x14: {
		Bitu mode = CPU_Pop16();
		CPU_Push16(mode);

		D1_GFX("set_video_mode(mode=0x%x);\n", mode);
		set_video_mode((unsigned char)mode);

		return 1;
	}
	case 0x2a: {
		Bitu page = CPU_Pop16();
		CPU_Push16(page);

		D1_GFX("set_video_page(page=0x%x);\n", page);
		set_video_page((unsigned char)page);

		return 1;
	}
	case 0x40: {
		RealPt addr = CPU_Pop32();
		CPU_Push32(addr);

		D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
			RealSeg(addr), RealOff(addr));

		return 0;
	}
	case 0xea: {
		RealPt ptr = CPU_Pop32();
		Bitu color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push32(ptr);

		D1_GFX("set_color(rgb=0x%x:0x%x, color=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), (unsigned char)color);

		set_color(Real2Host(ptr), color);

		D1_GFX("RGB=(0x%x, 0x%x, 0x%x);\n",
			real_readb(RealSeg(ptr), RealOff(ptr)),
			real_readb(RealSeg(ptr), RealOff(ptr) + 1),
			real_readb(RealSeg(ptr), RealOff(ptr) + 2));

		return 1;
	}
	case 0x119: {
		RealPt ptr = CPU_Pop32();
		unsigned short first_color = CPU_Pop16();
		Bitu colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(first_color);
		CPU_Push32(ptr);

		unsigned short i;

		D1_GFX("set_palette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), first_color, colors);

		set_palette(Real2Host(ptr), (unsigned char)first_color, (unsigned char)colors);

		if (RealSeg(ptr) == datseg)
			D1_LOG("Palette at DS:0x%x\n", RealSeg(ptr));
		for (i=0; i<colors; i++)
			D1_GFX("\"\\%02d\\%02d\\%02d\"..\n",
				host_readb(Real2Host(ptr)+i*3),
				host_readb(Real2Host(ptr)+i*3+1),
				host_readb(Real2Host(ptr)+i*3+2));
		return 1;
	}
	case 0x14d: {
		RealPt rptr = CPU_Pop32();
		Bitu cnt = CPU_Pop16();
		Bitu color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push32(rptr);

		draw_h_line(Real2Phys(rptr), cnt, (unsigned char)color);

		if (RealSeg(rptr) == 0xa000)
			D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
				RealOff(rptr) % 320,
				RealOff(rptr) / 320, cnt, color);
		else
			D1_GFX("HLine(0x%04x:0x%04x,len=%u,color=0x%02x);\n",
				RealSeg(rptr), RealOff(rptr), cnt, color);
		return 1;
	}
	case 0x1af: {
		RealPt rptr = CPU_Pop32();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		unsigned short space = CPU_Pop16();
		CPU_Push16(space);
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push32(rptr);

		draw_h_spaced_dots(Real2Phys(rptr), cnt,
			(unsigned char)color, space);
		if (RealSeg(rptr) == 0xa000)
			D1_GFX("HSpacedDots(X=%03d,Y=%03u,%03u,0x%02x,%u);\n",
				RealOff(rptr) % 320,
				RealOff(rptr) / 320, cnt, color, space);
		else
			D1_GFX("HSpacedDots(0x%04x:0x%04x,0x%04x,0x%02x,0x%04x);\n",
				RealSeg(rptr), RealOff(rptr), cnt, color, space);
		return 1;
	}
	case 0x219: {
		RealPt dst = CPU_Pop32();
		short x1 = CPU_Pop16();
		short y1 = CPU_Pop16();
		short x2 = CPU_Pop16();
		short y2 = CPU_Pop16();
		unsigned short val12 = CPU_Pop16();
		unsigned short val14 = CPU_Pop16();
		unsigned short val16 = CPU_Pop16();
		unsigned short val18 = CPU_Pop16();
		unsigned short width = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		RealPt src = CPU_Pop32();
		unsigned short mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push32(src);
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push16(val18);
		CPU_Push16(val16);
		CPU_Push16(val14);
		CPU_Push16(val12);
		CPU_Push16(y2);
		CPU_Push16(x2);
		CPU_Push16(y1);
		CPU_Push16(x1);
		CPU_Push32(dst);

		if (RealSeg(dst) == 0xa000)
			D1_GFX("pic_copy(X=%03u,Y=%03u,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x, val5=%d)\n",
				RealOff(dst) %320, RealOff(dst) / 320,
				x1, y1,	x2, y2,
				val12, val14, val16, val18,
				width, height,
				RealSeg(src), RealOff(src), mode);
		else
			D1_GFX("pic_copy(dest=0x%04x:0x%04x,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x,val5=%d)\n",
				RealSeg(dst), RealOff(dst),
				x1, y1,	x2, y2,
				val12, val14, val16, val18,
				width, height, RealSeg(src), RealOff(src), mode);

		pic_copy(Real2Phys(dst), x1, y1, x2, y2, val12, val14,
			val16, val18, width, height,
			MemBase+Real2Phys(src), mode);

		return 1;
	}
	case 0x655: {
		RealPt rptr = CPU_Pop32();
		RealPt dst = CPU_Pop32();
		unsigned short width = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push32(dst);
		CPU_Push32(rptr);
		/* Seg and Off are swapped */

		rptr = (rptr >> 16) | (rptr << 16);

		save_rect(Real2Phys(rptr), Real2Phys(dst), width, height);

		if (RealSeg(rptr) == 0xa000)
			D1_GFX("save_rect(X=%u,Y=%u,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
				RealOff(rptr) % 320, RealOff(rptr) / 320,
				RealSeg(dst), RealOff(dst),
				width, height);
		else
			D1_GFX("save_rect(src=0x%04x:0x%04x,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
				RealSeg(rptr), RealOff(rptr),
				RealSeg(dst), RealOff(dst),
				width, height);
		return 1;
	}
	case 0x68c: {
		RealPt rptr = CPU_Pop32();
		unsigned short color = CPU_Pop16();
		unsigned short width = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push16(color);
		CPU_Push32(rptr);

		/* Seg and Off are swapped */
		rptr = (rptr >> 16) | (rptr << 16);

		fill_rect(Real2Phys(rptr), (unsigned char)color, width, height);

		if (RealSeg(rptr) == 0xa000)

			D1_GFX("fill_rect(X=%u,Y=%u,color=%u,width=%u,height=%u)\n",
				RealOff(rptr) % 320,
				RealOff(rptr) / 320,
				color, width, height);
		else
			D1_GFX("fill_rect(dest=0x%04x:0x%04x,color=%u,cnt=%u,%u)\n",
				RealSeg(rptr), RealOff(rptr),
				color, width, height);
		return 1;
	}
	case 0x6c5: {
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		unsigned short width_to_copy = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		unsigned short dst_width = CPU_Pop16();
		unsigned short src_width = CPU_Pop16();
		unsigned short solid = CPU_Pop16();
		CPU_Push16(solid);
		CPU_Push16(src_width);
		CPU_Push16(dst_width);
		CPU_Push16(height);
		CPU_Push16(width_to_copy);
		CPU_Push32(src);
		CPU_Push32(dst);

		D1_GFX("copy_solid_permuted(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,width_to_copy=%u,height=%u,dst_width=%u,src_width=%u,solid=%u)\n",
			RealSeg(dst), RealOff(dst), RealSeg(src), RealOff(src),
			width_to_copy, height, dst_width, src_width, solid);

		copy_solid_permuted(Real2Host(dst),
			Real2Host(src), width_to_copy,
			height,	dst_width, src_width, solid);

		return 1;
	}
	/* used often in cities and dungeons */
	case 0x816: {
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		unsigned short width_to_copy = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		unsigned short dst_width = CPU_Pop16();
		unsigned short src_width = CPU_Pop16();
		unsigned short solid = CPU_Pop16();
		CPU_Push16(solid);
		CPU_Push16(src_width);
		CPU_Push16(dst_width);
		CPU_Push16(height);
		CPU_Push16(width_to_copy);
		CPU_Push32(src);
		CPU_Push32(dst);

		D1_GFX("copy_solid(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,width_to_copy=%u,height=%u,dst_width=%u,src_width=%u,solid=%u)\n",
			RealSeg(dst), RealOff(dst), RealSeg(src), RealOff(src),
			width_to_copy, height, dst_width, src_width, solid);

		copy_solid(Real2Host(dst), Real2Host(src),
			width_to_copy, height, dst_width, src_width, solid);

		return 1;
	}
	case 0x967: {
		unsigned short width = CPU_Pop16();
		unsigned short height = CPU_Pop16();
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		RealPt tmp_buffer = CPU_Pop32();
		unsigned short mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push32(tmp_buffer);
		CPU_Push32(src);
		CPU_Push32(dst);
		CPU_Push16(height);
		CPU_Push16(width);

		D1_GFX("decomp_rle(width=%d, height=%d, dst=0x%x:0x%x, src=0x%x:0x%x, tmp_buffer=0x%x:0x%x, mode=%d)\n",
			width, height,	RealSeg(dst), RealOff(dst),
			RealSeg(src), RealOff(src), RealSeg(tmp_buffer),
			RealOff(tmp_buffer), mode);

		decomp_rle(width, height, Real2Host(dst),
			Real2Host(src),
			Real2Host(tmp_buffer), mode);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg009(unsigned short offs) {
	switch (offs) {
		case 0x8: {
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			RealPt p3 = CPU_Pop32();
			unsigned int len = CPU_Pop32();
			CPU_Push32(len);
			CPU_Push32(p3);
			CPU_Push32(p2);
			CPU_Push32(p1);

			D1_LOG("decomp_pp20(0x%04x:0x%04x, 0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
				RealSeg(p1), RealOff(p1),
				RealSeg(p2), RealOff(p2),
				RealSeg(p3), RealOff(p3), len);

			decomp_pp20(Real2Host(p1),
				Real2Host(p2),
				Real2Host(p3), len);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg010(unsigned short offs) {
	switch (offs) {
		case 0x55: {
			reg_ax = EMS_get_num_pages_unalloced();
			D1_INFO("EMS nichtallokierte Seiten %d\n",
				reg_ax);

			return 1;
		}
		case 0x71: {
			unsigned short pages = CPU_Pop16();
			CPU_Push16(pages);

			reg_ax = EMS_alloc_pages(pages);

			D1_INFO("EMS %d Seiten angefordert, Handle = %d\n",
				pages, reg_ax);

			return 1;
		}
		case 0x90: {
			unsigned short handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = EMS_free_pages(handle);

			D1_INFO("EMS Handle %d freigegeben, Fehlercode = %d\n",
				handle, reg_ax);

			return 1;
		}
		case 0xa9: {
			unsigned short handle = CPU_Pop16();
			unsigned short lpage = CPU_Pop16();
			unsigned short ppage = CPU_Pop16();
			CPU_Push16(ppage);
			CPU_Push16(lpage);
			CPU_Push16(handle);

			unsigned char p = ppage & 0xff;

			reg_ax = EMS_map_memory(handle, lpage, p);

			D1_INFO("EMS_map_memory(%d, %d, %d) = 0x%x\n",
				handle, lpage, p, reg_ax);

			return 1;
		}
		case 0xc8: {
			RealPt p = CPU_Pop32();
			CPU_Push32(p);

			RealPt retval = EMS_norm_ptr(p);

			D1_LOG("EMS_norm_ptr(0x%x) = 0x%x\n", p, retval);
			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);

			return 1;
		}
		case 0xfd: {
			reg_ax = EMS_init();
			if (reg_ax) {
				D1_LOG("EMS initialisiert Segment 0x%x\n",
					ds_readw(0x4bac));

				//ds_writeb(0x26ab, 1);
			} else
				D1_LOG("EMS nicht vorhanden\n");

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg012(unsigned short offs) {
	/*
		seg012 has only one func called by far directly.
		The fcalls to the func at 0x195 and 0xd29 must
		be done by a jump to a pointer value.
	 */
	switch (offs) {
		case 0x195:
			D1_LOG("seg12_195();\n");
			return 0;
		case 0x8ed: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			unsigned short v3 = CPU_Pop16();
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_LOG("seg12_8ed(%u, %u, %u);\n", v1, v2, v3);
			return 0;
		}
		case 0xd29:
			D1_LOG("seg12_d29();\n");
			return 0;
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg024(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		D1_LOG("diary_show();\n");
		diary_show();
		return 1;
	}
	case 0x2a: {
		D1_LOG("diary_new_entry();\n");
		diary_new_entry();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",
			__func__, offs);
		exit(1);
	}
}

static int seg026(unsigned short offs) {
	switch (offs) {

	case 0x20: {
		D1_LOG("ip=0x%04X unknown()\n", offs);
		return 0;
	}
	case 0x25: {
		RealPt src = CPU_Pop32();
		RealPt dst = CPU_Pop32();
		Bit32u len = CPU_Pop32();

		D1_LOG("split_textbuffer(%x, %x, %d)\n", src, dst, len);
		split_textbuffer(Real2Host(src), dst, len);
		CPU_Push32(len);
		CPU_Push32(dst);
		CPU_Push32(src);
		return 1;
	}
	case 0x2a: {
		D1_LOG("load_ggsts_nvf()\n");
		load_ggsts_nvf();
		return 1;
	}
	case 0x2f: {
		D1_LOG("short ChooseLoadSavegame(void)\n");
		return 0;
	}
	case 0x34: {
		D1_LOG("ip=0x%04X ChooseSaveSavegame()\n", offs);
		return 0;
	}
	case 0x39: {
		D1_LOG("ip=0x%4X unknown()\n", offs);
		return 0;
	}
	case 0x3e: {
		D1_LOG("ip=0x%4X ReleaseHero()\n", offs);
		return 0;
	}
	case 0x43: {
		D1_LOG("ip=0x%4X ChooseFreeHero()\n", offs);
		return 0;
	}
	case 0x48: {
		Bit16s head = CPU_Pop16();
		D1_LOG("load_in_head(%d)\n", head);
		load_in_head(head);
		CPU_Push16(head);
		return 1;
	}
	case 0x4d: {
		Bit16s index = CPU_Pop16();
		D1_LOG("load_city_ltx(%s)\n", get_fname(index));
		load_city_ltx(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x52: {
		Bit16s index = CPU_Pop16();
		D1_LOG("load_buffer_1(%s)\n", get_fname(index));
		load_buffer_1(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x57: {
		Bit16u index = CPU_Pop16();
		CPU_Push16(index);
		D1_LOG("load_ltx(%s)\n", get_fname(index));
		load_ltx(index);
		return 1;
	}
	case 0x5c: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);
		D1_LOG("prepare_chr_name(%x, %s);\n", p1, (char*)Real2Host(p2));
		prepare_chr_name((char*)Real2Host(p1), (char*)Real2Host(p2));
		return 1;
	}
	case 0x66: {
		D1_LOG("ip=0x%4X unknown()\n", offs);
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",
			__func__, offs);
		exit(1);
	}
}

static int seg027(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16u pic = CPU_Pop16();

			D1_LOG("load_pp20(%03x);\n", pic);
			load_pp20(pic);

			CPU_Push16(pic);
			return 1;
		}
		case 0x25: {
			short ani = CPU_Pop16();
			CPU_Push16(ani);

			D1_LOG("ANI %02d angefordert\n", ani);
			return 0;
		}
		case 0x2a: {
			return 0;
		}
		case 0x2f: {
			D1_LOG("init_common_buffers();\n");
			init_common_buffers();
			return 1;
		}
		case 0x34: {
			Bit16s nr = CPU_Pop16();

			D1_LOG("load_scenario(0x%02x);\n", nr);
			load_scenario(nr);

			CPU_Push16(nr);
			return 1;
		}
		case 0x39: {
			Bit16s nr = CPU_Pop16();
			CPU_Push16(nr);
			reg_ax = count_fight_enemies(nr);
			D1_LOG("count_fight_enemies(0x%x) = %d\n", nr, reg_ax);
			return 1;
		}
		case 0x3e: {
			Bit16s nr = CPU_Pop16();
			D1_LOG("read_fight_lst(0x%x)\n", nr);
			CPU_Push16(nr);
			read_fight_lst(nr);
			return 1;
		}
		case 0x43: {
			D1_LOG("write_fight_lst();\n");
			write_fight_lst();
			return 1;
		}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg028(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		return 0;
	}
	case 0x25: {
		return 0;
	}
	case 0x2a: {
		return 0;
	}
	case 0x2f: {
		return 0;
	}
	case 0x34: {
		return 0;
	}
	case 0x39: {
		return 0;
	}
	case 0x3e: {
		return 0;
	}
	case 0x43: {
		return 0;
	}
	case 0x48: {
		return 0;
	}
	case 0x4d: {
		return 0;
	}
	case 0x52: {
		return 0;
	}
	case 0x57: {
		return 0;
	}
	case 0x5c: {
		return 0;
	}
	case 0x61: {
		Bit16s index = CPU_Pop16();
		D1_LOG("load_npc(%s);\n", get_fname(index));
		load_npc(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x66: {
		Bit16s index = CPU_Pop16();
		D1_LOG("save_npc(%s);\n", get_fname(index));
		save_npc(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x6b: {
		return 0;
	}
	case 0x70: {
		return 0;
	}
	case 0x75: {
		Bit16s index = CPU_Pop16();
		D1_LOG("load_tlk(%s);\n", get_fname(index));
		load_tlk(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x7a: {
		Bit16s index = CPU_Pop16();
		D1_LOG("load_fightbg(%s);\n", get_fname(index));
		load_fightbg(index);
		CPU_Push16(index);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg029(unsigned short offs) {
	switch (offs) {
	case 0x25: {
		D1_LOG("draw_status_line();\n");
		draw_status_line();
		return 1;
	}
	case 0x2a: {
		D1_LOG("draw_icons();\n");
		draw_icons();
		return 1;
	}
	case 0x2f: {
		D1_LOG("draw_main_screen();\n");
		draw_main_screen();
		return 1;
	}
	case 0x39: {
		D1_LOG("clear_loc_line();\n");
		clear_loc_line();
		return 1;
	}
	case 0x3e: {
		unsigned short pos = CPU_Pop16();
		CPU_Push16(pos);

		select_hero_icon(pos);
		D1_LOG("select_hero_icon(%d);\n", pos);
		return 1;
	}
	case 0x43: {
		unsigned short pos = CPU_Pop16();
		CPU_Push16(pos);

		deselect_hero_icon(pos);
		D1_LOG("deselect_hero_icon(%d);\n", pos);
		return 1;
	}
	case 0x48: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		copy_forename(Real2Host(p1), Real2Host(p2));
		D1_LOG("copy_forename(%s, %s)\n",
			(char*)Real2Host(p1),
			(char*)Real2Host(p2));
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",
			__func__, offs);
		exit(1);
	}
}

static int seg032(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			unsigned short v1 = CPU_Pop16();
			CPU_Push16(v1);

			D1_LOG("Kampf 0x%02x\n", v1);
			return 0;
		}
		case 0x25: {
			signed short row = CPU_Pop16();
			signed short col = CPU_Pop16();
			signed short object = CPU_Pop16();
			CPU_Push16(object);
			CPU_Push16(col);
			CPU_Push16(row);

			signed char obj = object & 0xff;

			FIG_set_cb_field(row, col, obj);
			D1_LOG("FIG_set_cb_field(row=%d,col=%d,object=%d);\n",
				row, col, obj);
			return 1;
		}
		case 0x2a: {
			return 0;
		}
		case 0x39: {
			reg_ax = FIG_count_active_enemies();
			D1_LOG("FIG_count_active_enemies() = %d\n", reg_ax);
			return 1;
		}
		case 0x43: {
			reg_ax = FIG_get_first_active_hero();

			D1_LOG("FIG_get_first_active_hero(); = %s\n",
				reg_ax != -1 ? schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da) : "none");
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg038(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			signed short obj = CPU_Pop16();
			RealPt px = CPU_Pop32();
			RealPt py = CPU_Pop32();
			CPU_Push32(py);
			CPU_Push32(px);
			CPU_Push16(obj);

			reg_ax = FIG_search_obj_on_cb(obj,
					Real2Host(px),
					Real2Host(py));

			D1_LOG("far FIG_search_obj_on_cb(obj=%d, x=%d, y=%d) = %d\n",
				obj, mem_readw(Real2Phys(px)),
				mem_readw(Real2Phys(py)), reg_ax);

			return 1;
		}
		case 0x25: {
			signed short obj = CPU_Pop16();
			CPU_Push16(obj);

			FIG_init_list_elem(obj);
			D1_LOG("FIG_init_list_elem(%d);\n", obj);

			return 1;
		}
		case 0x39: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg039(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			unsigned short v3 = CPU_Pop16();
			unsigned short v4 = CPU_Pop16();
			CPU_Push16(v4);
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			reg_ax = seg039_0000(v1, v2, v3, v4);
			D1_LOG("seg039(%d,%d,%d,%d); = %d\n",
				v1, v2, v3, v4, reg_ax);

			return 1;
		}
		case 0x25: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = seg039_0023(Real2Host(hero));
			D1_LOG("seg039_0023(%s) = %d\n", schick_getCharname(hero), (signed short)reg_ax);

			return 1;
		}
		case 0x2a: {
			unsigned short sheet_nr = CPU_Pop16();
			unsigned short enemy_id_16 = CPU_Pop16();
			unsigned short round_16 = CPU_Pop16();
			CPU_Push16(round_16);
			CPU_Push16(enemy_id_16);
			CPU_Push16(sheet_nr);

			signed char enemy = (signed char)(enemy_id_16 & 0xff);
			signed char round = (signed char)(round_16 & 0xff);

			D1_LOG("fill_enemy_sheet(%d, %d, %d);\n",
				sheet_nr, enemy, round);

			fill_enemy_sheet(sheet_nr, enemy, round);

			return 1;
		}
		case 0x2f: {
			unsigned short x = CPU_Pop16();
			unsigned short y = CPU_Pop16();
			signed short object = CPU_Pop16();
			unsigned short v2_16 = CPU_Pop16();
			unsigned short dir_16 = CPU_Pop16();
			CPU_Push16(dir_16);
			CPU_Push16(v2_16);
			CPU_Push16(object);
			CPU_Push16(y);
			CPU_Push16(x);

			signed char v2 = (signed char)(v2_16 & 0xff);
			signed char dir = (signed char)(dir_16 & 0xff);

			reg_ax = place_obj_on_cb(x, y, object, v2, dir);
			D1_LOG("place_obj_on_cb(x=%d,y=%d,obj=%d,%d,dir=%d); = %d\n",
				x, y, object, v2, dir, reg_ax);

			return 1;
		}
		case 0x34: {
			return 0;
		}
		case 0x39: {
			return 0;
		}
		case 0x3e: {
			D1_LOG("FIG_init_heroes();\n");
			FIG_init_heroes();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg041(unsigned short offs) {
	switch(offs) {
	case 0x20: {
		return 0;
	}
	case 0x25: {
		RealPt str = CPU_Pop32();
		D1_LOG("FIG_output(%05s);\n", getString(str));
		FIG_output(Real2Host(str));
		CPU_Push32(str);
		return 1;
	}
	case 0x2a: {
		D1_LOG("FIG_clear_msgs();\n");
		FIG_clear_msgs();
		return 1;
	}
	case 0x2f: {
		unsigned short f_action = CPU_Pop16();
		unsigned short damage = CPU_Pop16();
		CPU_Push16(damage);
		CPU_Push16(f_action);
		D1_LOG("FIG_add_msg(f_action=0x%04x, damage=0x%04x);\n",
			f_action, damage);
		FIG_add_msg(f_action, damage);
		return 1;
	}
	case 0x34: {
		RealPt ptr = CPU_Pop32();
		Bit16u damage = CPU_Pop16();
		Bit16u v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(damage);
		CPU_Push32(ptr);

		D1_LOG("FIG_damage_enemy(%x, %x, %x);\n", ptr, damage, v2);
		FIG_damage_enemy(Real2Host(ptr), damage, v2 ? true : false);
		return 1;
	}
	case 0x39: {
		return 0;
	}
	case 0x3e: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = weapon_check(Real2Host(hero));

		D1_LOG("weapon_check(%s); = %d\n", schick_getCharname(hero),
			(signed short)reg_ax);

		return 1;
	}
	case 0x43: {
		RealPt enemy = CPU_Pop32();
		RealPt hero = CPU_Pop32();
		Bit16u flag = CPU_Pop16();

		reg_ax = FIG_get_enemy_attack_damage(Real2Host(enemy), Real2Host(hero), flag ? true : false);
		D1_LOG("FIG_get_enemy_attack_damage(%x, %x, %d); = %d\n",
				enemy, hero, flag, reg_ax);
		CPU_Push16(flag);
		CPU_Push32(hero);
		CPU_Push32(enemy);

		return 1;
	}
	case 0x48: {
		D1_LOG("seg041_8c8();\n");
		seg041_8c8();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg043(unsigned short offs) {

	switch (offs) {

	case 0x20: {
		return 0;
	}
	case 0x25: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg044(unsigned short offs) {

	switch (offs) {

	case 0x20: {
		D1_LOG("%s:0x%x\n", __func__, offs);
		return 0;
	}
	case 0x25: {
		D1_LOG("%s:0x%x\n", __func__, offs);
		return 0;
	}
	case 0x2a: {
		Bit16u v1 = CPU_Pop16();
		RealPt p = CPU_Pop32();
		Bit16u v2 = CPU_Pop16();
		Bit16s obj1 = CPU_Pop16();
		Bit16s obj2 = CPU_Pop16();
		Bit16u v5 = CPU_Pop16();
		Bit16u v6 = CPU_Pop16();

		D1_LOG("seg044_002a(%d, %s, %d, %d, %d, %d, %d)\n",
			v1, schick_getCharname(p), v2, obj1, obj2, v5, v6);
		seg044_002a(v1, Real2Host(p), v2, obj1, obj2, v5, v6);

		CPU_Push16(v6);
		CPU_Push16(v5);
		CPU_Push16(obj2);
		CPU_Push16(obj1);
		CPU_Push16(v2);
		CPU_Push32(p);
		CPU_Push16(v1);

		return 1;
	}
	case 0x2f: {
		Bit16u v1 = CPU_Pop16();
		RealPt p = CPU_Pop32();
		Bit16u v2 = CPU_Pop16();
		Bit16s obj1 = CPU_Pop16();
		Bit16s obj2 = CPU_Pop16();
		Bit16u v5 = CPU_Pop16();

		D1_LOG("seg044_002f(%d, 0x%x, %d, %d, %d, %d)\n",
			v1, p, v2, obj1, obj2, v5);

		seg044_002f(v1, Real2Host(p), v2, obj1, obj2, v5);

		CPU_Push16(v5);
		CPU_Push16(obj2);
		CPU_Push16(obj1);
		CPU_Push16(v2);
		CPU_Push32(p);
		CPU_Push16(v1);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg046(unsigned short offs) {

	switch (offs) {

	case 0x20: {
		Bit16u index = CPU_Pop16();

		D1_LOG("status_show(%d)\n", index);
		status_show(index);

		CPU_Push16(index);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg047(unsigned short offs) {
	switch(offs) {
	case 0x20: {
		RealPt s_ptr = CPU_Pop32();
		unsigned int money = CPU_Pop32();
		CPU_Push32(money);
		CPU_Push32(s_ptr);

		D1_LOG("make_valuta_str(%x, %d);\n", s_ptr, money);
		make_valuta_str((char*)MemBase + Real2Phys(s_ptr), money);

		return 1;
	}
	case 0x25: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		hero_get_sober(Real2Host(hero));

		return 1;
	}
	case 0x2a: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		D1_LOG("%s ist betrunken\n", schick_getCharname(hero));

		return 0;
	}
/*	not called
	case 0x2f: {
		signed short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = check_heros_KK(val);
		D1_LOG("check_heros_KK(%d); = %d\n", val, reg_ax);

		return 1;
	}
*/
	case 0x34: {
		signed short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = check_heros_KK(val);
		D1_LOG("check_heros_KK(%d); = %d\n", val, reg_ax);

		return 1;
	}
	case 0x39: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_is_diseased(Real2Host(hero));
		D1_LOG("hero_is_diseased(%s) = %d\n",
			schick_getCharname(hero), reg_ax);
		return 1;
	}
	case 0x3e: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_is_poisoned(Real2Host(hero));
		D1_LOG("hero_is_poisoned(%s) = %d\n",
			schick_getCharname(hero), reg_ax);
		return 1;
	}
	case 0x43: {
		return 0;
	}
	case 0x48: {
		RealPt hero = CPU_Pop32();
		D1_INFO("update_atpa(%s);\n", schick_getCharname(hero));
		update_atpa(Real2Host(hero));
		CPU_Push32(hero);
		return 1;
	}
	case 0x4d: {
		return 0;
	}
	case 0x52: {
		return 0;
	}
	case 0x57: {
		reg_ax = count_heroes_in_group();
		D1_LOG("count_heroes_in_group() = %d\n", reg_ax);
		return 1;
	}
	case 0x5c: {
		reg_ax = get_hero_CH_best();
		D1_LOG("get_hero_CH_best() = %d\n", reg_ax);
		return 1;
	}
	case 0x61: {
		reg_ax = get_hero_KK_best();
		D1_LOG("get_hero_KK_best() = %d\n", reg_ax);
		return 1;
	}
	case 0x66: {
		RealPt hero = CPU_Pop32();
		unsigned short poison = CPU_Pop16();
		CPU_Push16(poison);
		CPU_Push32(hero);

		hero_gets_poisoned(Real2Host(hero), poison);
		D1_INFO("%s wurde vergiftet %d\n",
			schick_getCharname(hero), poison);
		return 1;
	}
	case 0x6b: {
		RealPt hero = CPU_Pop32();
		unsigned short disease = CPU_Pop16();
		CPU_Push16(disease);
		CPU_Push32(hero);

		hero_gets_diseased(Real2Host(hero), disease);
		D1_INFO("%s ist erkrankt %d\n",
			schick_getCharname(hero), disease);
		return 1;
	}
	case 0x70: {
		RealPt hero = CPU_Pop32();
		unsigned short disease = CPU_Pop16();
		unsigned short probability = CPU_Pop16();
		CPU_Push16(probability);
		CPU_Push16(disease);
		CPU_Push32(hero);

		hero_disease_test(Real2Host(hero), disease, probability);
		D1_INFO("%s koennte zu %d%% an %d erkranken\n",
			schick_getCharname(hero), probability, disease);
		return 1;
	}
	case 0x75: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg050(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16u hero_nr = CPU_Pop16();

			D1_LOG("level_up(%s);\n", get_hero(hero_nr) + 0x10);

			CPU_Push16(hero_nr);
			return 0;
		}
		case 0x2f: {
			RealPt hero = CPU_Pop32();
			Bit16u skill = CPU_Pop16();

			D1_LOG("inc_skill_novice(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);
			inc_skill_novice(Real2Host(hero), skill);

			CPU_Push16(skill);
			CPU_Push32(hero);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg053(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			unsigned short typi = ds_readb(0x4224);
			signed char price = ds_readb(typi * 2 + 0x66ea);
			unsigned char qual = ds_readb(typi * 2 + 0x66ea + 1);
			D1_INFO("Heiler: 0x%02x Preis: %d%% Qualitaet: %d\n",
				typi, 100 + price, qual);
			return 0;
		}
		default:
			return 0;
	}
}

static int seg063(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("do_harbour();\n");
			return 0;
		}
		case 0x25: {
			D1_LOG("passages_init();\n");
			passages_init();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg068(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("thorwal_eisenhof();\n");
			return 0;
		}
		case 0x25: {
			D1_LOG("thorwal_imman();\n");
			thorwal_imman();
			return 1;
		}
		case 0x2a: {
			D1_LOG("thorwal_botschaft();\n");
			thorwal_botschaft();
			return 1;
		}
		case 0x2f: {
			D1_LOG("thorwal_bank();\n");
			return 0;
		}
		case 0x34: {
			D1_LOG("thorwal_zeughaus();\n");
			return 0;
		}
		case 0x39: {
			D1_LOG("thorwal_wache1();\n");
			return 0;
		}
		case 0x3e: {
			D1_LOG("thorwal_mueller();\n");
			thorwal_mueller();
			return 1;
		}
		case 0x43: {
			D1_LOG("thorwal_swafnir();\n");
			return 0;
		}
		case 0x4d: {
			D1_LOG("thorwal_zwingfeste();\n");
			return 0;
		}
		case 0x57: {
			D1_LOG("thorwal_akademie();\n");
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg069(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("thorwal_hetmann();\n");
			return 0;
		}
		case 0x25: {
			D1_LOG("thorwal_windzwinger();\n");
			return 0;
		}
		case 0x2a: {
			D1_LOG("thorwal_combi_tav_inn();\n");
			return 0;
		}
		case 0x2f: {
			D1_LOG("thorwal_sturm();\n");
			return 0;
		}
		case 0x34: {
			D1_LOG("thorwal_garaldssons();\n");
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg073(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			unsigned char city = ds_readb(0x2d67);
			unsigned char ww = ds_readb(0x7c9d + city);
			D1_LOG("Merkwuerdige Funktion\n");
			D1_LOG("Stadt: 0x%02x\t WW: 0x%02x\n", city, ww);
			return 0;
		}
		case 0x25:
		case 0x2a:
		case 0x2f:
			return 0;
		default:
			return 0;
	}
}

static int seg095(unsigned short offs) {
	switch (offs) {
		case 0x3e: {
			Bit16s head_index = CPU_Pop16();
			Bit16s days = CPU_Pop16();
			Bit16s index = CPU_Pop16();
			RealPt name = CPU_Pop32();
			RealPt text = CPU_Pop32();

			remove_npc(head_index, (signed char)days, index,
				Real2Host(name), Real2Host(text));

			D1_INFO("remove_npc(%x, %d, %x, %x, %x);\n",
				head_index, (signed char)days, index,
				name, text);

			CPU_Push32(text);
			CPU_Push32(name);
			CPU_Push16(index);
			CPU_Push16(days);
			CPU_Push16(head_index);
			return 1;
		}
		case 0x43: {
			Bit16s index = CPU_Pop16();
			D1_LOG("add_npc(%s);\n", get_fname(index));
			add_npc(index);
			CPU_Push16(index);
			return 1;
		}
		case 0x48: {
			Bit16u type = CPU_Pop16();
			D1_INFO("npcmeetings(%x);\n", type);
			reg_ax = npc_meetings(type);
			CPU_Push16(type);
			return 1;
		}
		case 0x4d: {
			D1_INFO("npc_farewell();\n");
			npc_farewell();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg096(unsigned short offs) {
	/*
		Handles Strings and Fonts
	*/
	switch (offs)	{
	case 0x25: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		RealPt retval;

		D1_LOG("GUI_names_grammar(%x,%x,%x)\n", v1 ,v2,v3);
		retval = GUI_names_grammar(v1, v2, v3);
		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 0;
	}
	case 0x2a: {
		unsigned short v1 = CPU_Pop16();
		RealPt s = CPU_Pop32();
		CPU_Push32(s);
		CPU_Push16(v1);

		RealPt retval;

		retval = GUI_name_plural(v1, MemBase + Real2Phys(s));
		D1_LOG("GUI_name_singular(%x, %s)\n",
			v1, getString(s));

		reg_dx = RealSeg(retval);
		reg_ax = RealOff(retval);

		return 1;
	}
	case 0x2f: {
		RealPt s = CPU_Pop32();
		CPU_Push32(s);
		RealPt retval;

		retval = GUI_name_singular(MemBase + Real2Phys(s));
		D1_LOG("GUI_name_singular(%x:%x)\n", RealSeg(s), RealOff(s));

		reg_dx = RealSeg(retval);
		reg_ax = RealOff(retval);

		return 1;
	}
	case 0x34: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		RealPt retval;

		retval = GUI_2f2(v1, v2, v3);
		D1_LOG("GUI_2f2(%d, %d, %d)\n", v1, v2, v3);
		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	case 0x39: {
			Bit16u v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			RealPt retval;

			retval = GUI_get_ptr(v1, v2);
			D1_LOG("GUI_get_ptr(%d,%d) = %s\n",
				v1, v2, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
	}
	case 0x3e: {
			Bit16u v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);

			RealPt retval;

			retval = GUI_get_ptr2(v1, v2);
			D1_LOG("GUI_get_ptr2(%d,%d) = %s", v1, v2,
				v1, v2, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
	}
	case 0x43: {
		RealPt s = CPU_Pop32();
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(s);

		D1_LOG("GUI_print_string(%s, %d, %d);\n", getString(s), x, y);
		GUI_print_string(MemBase + Real2Phys(s), x, y);
		return 1;
	}
	case 0x48: {
		unsigned short c = CPU_Pop16();
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push16(c);

		D1_LOG("GUI_print_char(%c, %d, %d);\n", c & 0xff, x, y);
		reg_ax = GUI_print_char(c & 0xff, x, y);
		return 1;
	}
	case 0x4d: {
		RealPt p = CPU_Pop32();
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = GUI_get_first_pos_centered(Real2Host(p), v1, v2, v3);
		D1_LOG("GUI_get_first_pos_centered(%s,%d,%d,%d) = %d\n", getString(p),
			v1, v2, v3, reg_ax);

		return 1;
	}
	case 0x52: {
		RealPt ptr = CPU_Pop32();
		CPU_Push32(ptr);

		reg_ax = GUI_count_lines(MemBase + Real2Phys(ptr));
		D1_LOG("GUI_count_lines(%s) = %d\n", getString(ptr),(unsigned short)reg_ax);

		return 1;
	}
	case 0x66: {
		GUI_blank_char();
		return 1;
	}
	case 0x6b: {
		RealPt p = CPU_Pop32();
		CPU_Push32(p);

		D1_LOG("GUI_font_to_buf(0x%x:0x%x)\n", RealSeg(p), RealOff(p));
		GUI_font_to_buf(Real2Host(p));

		return 1;
	}
	case 0x70: {
		unsigned short x = CPU_Pop16();
		unsigned short y = CPU_Pop16();
		unsigned short char_height = CPU_Pop16();
		unsigned short char_width = CPU_Pop16();
		CPU_Push16(char_width);
		CPU_Push16(char_height);
		CPU_Push16(y);
		CPU_Push16(x);

		D1_LOG("GUI_write_char_to_screen_xy(%d, %d, %d, %d);\n",
			x, y, char_height, char_width);
		GUI_write_char_to_screen_xy(x, y, char_height, char_width);

		return 1;
	}
	case 0x75: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("GUI_set_smth(%d, %d);\n", v1, v2);
		GUI_set_smth(v1, v2);

		return 1;
	}
	case 0x7a: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		D1_LOG("GUI_get_smth();\n");
		GUI_get_smth((short*)(Real2Host(p1)), (short*)(Real2Host(p2)));

		return 1;
	}
	case 0x7f: {
		RealPt ptr = CPU_Pop32();
		CPU_Push32(ptr);

		D1_LOG("GUI_print_loc_line(%s)\n", getString(ptr));
		GUI_print_loc_line(MemBase + Real2Phys(ptr));
		return 1;
	}
	case 0x84: {
		RealPt str = CPU_Pop32();
		CPU_Push32(str);

		D1_LOG("GUI_print_header(%s)\n", getString(str));
		GUI_print_header(MemBase + Real2Phys(str));
		return 1;
	}
	case 0x89: {
		RealPt p = CPU_Pop32();
		unsigned short v1 = CPU_Pop16();
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = GUI_get_space_for_string(MemBase + Real2Phys(p), (char)v1);
		D1_LOG("GUI_get_space_for_string(%s, %d) = %d\n",
			getString(p), v1, reg_ax);

		return 1;
	}
	case 0x8e: {
		unsigned short line = CPU_Pop16();
		unsigned short type = CPU_Pop16();
		CPU_Push16(type);
		CPU_Push16(line);

		D1_LOG("GUI_draw_popup_line(%d, %d)\n", line, type);
		GUI_draw_popup_line(line, type);
		return 1;
	}
	default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg097(unsigned short offs) {

	char buffer[20];

	switch (offs) {
	case 0x25: {
		unsigned short c = CPU_Pop16();
		RealPt p_height = CPU_Pop32();
		CPU_Push32(p_height);
		CPU_Push16(c);

		reg_ax = GUI_lookup_char_height(c & 0xff, (unsigned short*)Real2Host(p_height));
		D1_LOG("GUI_lookup_char_height() = %d\n", (char)reg_ax);
		return 1;
	}
	case 0x34: {
		RealPt str = CPU_Pop32();
		unsigned short num = CPU_Pop16();
		CPU_Push16(num);
		CPU_Push32(str);

		D1_LOG("GUI_input()\n");
		reg_ax = GUI_input(Real2Host(str), num);
		return 1;
	}
	case 0x39: {
		RealPt str = CPU_Pop32();
		CPU_Push32(str);

		D1_LOG("GUI_output()\n");
		GUI_output(Real2Host(str));
		return 1;
	}
	case 0x3e: {
		RealPt ptr = CPU_Pop32();
		CPU_Push32(ptr);

		strncpy(buffer, getString(ptr), 20);
		reg_ax = GUI_bool(Real2Host(ptr));
		D1_LOG("GUI_bool(%s...) = %d\n", buffer, reg_ax);

		return 1;
	}
	case 0x43: {
		RealPt text = CPU_Pop32();
		unsigned short options = CPU_Pop16();

		unsigned short i;

		strncpy(buffer, getString(text), 20);
		D1_LOG("GUI_RadioBox(%s..., %d", buffer, (char)options);

		for (i = 0; i < (char)options; i++) {
			strncpy(buffer, getString(real_readd(SegValue(ss), reg_sp + i *4)), 20);
			D1_LOG(", %s...", buffer);
		}

		D1_LOG(");\n");
		CPU_Push16(options);
		CPU_Push32(text);
		return 0;
	}
	case 0x5c: {
		RealPt pic = CPU_Pop32();
		RealPt name = CPU_Pop32();
		RealPt text = CPU_Pop32();
		unsigned short options = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(options);
		CPU_Push32(text);
		CPU_Push32(name);
		CPU_Push32(pic);

		if (name)
			strncpy(buffer, getString(name), 20);
		else
			strncpy(buffer, "(NULL)", 20);

		D1_LOG("GUI_dialogbox(pic=0x%x, %s,", pic, buffer);
		strncpy(buffer, getString(text), 20);
		D1_LOG("%s..., %d, %d)\n", buffer, (char)options, v2);
		return 0;
	}
	case 0x66: {
		unsigned short fight = CPU_Pop16();
		CPU_Push16(fight);

		D1_LOG("GUI_Dungeon(0x%x)\n", fight);
		return 0;
	}
	case 0x6b: {
		Bit16u head = CPU_Pop16();
		RealPt text = CPU_Pop32();
		CPU_Push32(text);
		CPU_Push16(head);

		D1_LOG("GUI_dialog_na(%d);\n", head);
		GUI_dialog_na(head, Real2Host(text));

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	return 0;
}

static int seg098(unsigned short offs) {
	switch (offs) {

	case 0x20: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		D1_LOG("Menu: Magie Anwenden %s\n", schick_getCharname(hero));

		return 0;
	}
	case 0x25: {
		/* Zauberprobe */
		RealPt hero = CPU_Pop32();
		unsigned short spell = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(spell);
		CPU_Push32(hero);

		D1_LOG("Zauberprobe : %s %+d ", names_spell[spell], (signed char)bonus);
		reg_ax = test_spell(Real2Host(hero), spell, (signed char)bonus);
		return 1;
	}
	case 0x2a: {
		/* Zauber auswaehlen */
		RealPt hero = CPU_Pop32();
		unsigned short a1 = CPU_Pop16();
		unsigned short a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(a1);
		CPU_Push32(hero);

		D1_LOG("Menu: Zauber auswaehlen(%s, %d, %d)\n",
			schick_getCharname(hero), a1, a2);

		return 0;
	}
	case 0x2f: {
		D1_TRAC("Seg098:0x%04x()\n", offs);
		return 0;
	}
	case 0x39: {
		D1_TRAC("Seg098:0x%04x()\n", offs);
		return 0;
	}
	case 0x3e: {
		/* Untested */
		D1_TRAC("seg098_3e()\n");
		/*reg_ax = seg098_3e(); */
		return 0;
	}
	case 0x48: {
		unsigned short spell = CPU_Pop16();
		unsigned short half_cost = CPU_Pop16();
		CPU_Push16(half_cost);
		CPU_Push16(spell);

		reg_ax = get_spell_cost(spell, half_cost);

		D1_LOG("get_spell_cost(%s, %d) = %d\n",
			names_spell[spell], half_cost, (short)reg_ax);

		return 1;
	}
	case 0x52: {
		unsigned short spell = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(spell);

		D1_LOG("Zauberprobe fuer alle\n");
		reg_ax = test_spell_group(spell, (signed char)bonus);

		return 1;
	}
	case 0x57: {
		D1_TRAC("Menu: Zauberer auswaehlen\n");
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	return 0;
}

static int seg099(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		return 0;
	}
	case 0x25: {
		D1_LOG("spell_destructibo();\n");
		spell_destructibo();
		return 1;
	}
	case 0x2a: {
		D1_LOG("spell_gardanium();\n");
		spell_gardanium();
		return 1;
	}
	case 0x2f: {
		return 0;
	}
	case 0x34: {
		return 0;
	}
	case 0x39: {
		return 0;
	}
	case 0x3e: {
		return 0;
	}
	case 0x43: {
		return 0;
	}
	case 0x48: {
		D1_LOG("spell_grosse_gier();\n");
		spell_grosse_gier();
		return 1;
	}
	case 0x4d: {
		D1_LOG("spell_grosse_ver();\n");
		spell_grosse_ver();
		return 1;
	}
	case 0x52: {
		return 0;
	}
	case 0x57: {
		return 0;
	}
	case 0x5c: {
		return 0;
	}
	case 0x61: {
		return 0;
	}
	case 0x66: {
		return 0;
	}
	case 0x6b: {
		return 0;
	}
	case 0x70: {
		return 0;
	}
	case 0x75: {
		return 0;
	}
	case 0x7a: {
		return 0;
	}
	case 0x7f: {
		return 0;
	}
	case 0x84: {
		return 0;
	}
	case 0x89: {
		return 0;
	}
	case 0x8e: {
		return 0;
	}
	case 0x93: {
		return 0;
	}
	case 0x98: {
		return 0;
	}
	case 0x9d: {
		return 0;
	}
	case 0xa2: {
		return 0;
	}
	case 0xa7: {
		return 0;
	}
	case 0xac: {
		return 0;
	}
	case 0xb1: {
		return 0;
	}
	case 0xb6: {
		return 0;
	}
	case 0xbb: {
		return 0;
	}
	case 0xc0: {
		return 0;
	}
	case 0xc5: {
		return 0;
	}
	case 0xca: {
		return 0;
	}
	case 0xcf: {
		return 0;
	}
	case 0xd4: {
		return 0;
	}
	case 0xd9: {
		return 0;
	}
	case 0xde: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg100(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		return 0;
	}
	case 0x25: {
		return 0;
	}
	case 0x2a: {
		return 0;
	}
	case 0x2f: {
		return 0;
	}
	case 0x34: {
		spell_sensibar();
		return 1;
	}
	case 0x39: {
		spell_chamaelioni();
		return 1;
	}
	case 0x3e: {
		spell_duplicatus();
		return 1;
	}
	case 0x43: {
		spell_harmlos();
		return 1;
	}
	case 0x48: {
		return 0;
	}
	case 0x4d: {
		spell_blitz();
		return 1;
	}
	case 0x52: {
		spell_ecliptifactus();
		return 1;
	}
	case 0x57: {
		return 0;
	}
	case 0x5c: {
		return 0;
	}
	case 0x61: {
		return 0;
	}
	case 0x66: {
		return 0;
	}
	case 0x6b: {
		return 0;
	}
	case 0x70: {
		return 0;
	}
	case 0x75: {
		return 0;
	}
	case 0x7a: {
		spell_hexenblick();
		return 1;
	}
	case 0x7f: {
		spell_necropathia();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg101(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		spell_adler();
		return 1;
	}
	case 0x25: {
		spell_arcano();
		return 1;
	}
	case 0x2a: {
		spell_armatrutz();
		return 1;
	}
	case 0x2f: {
		spell_inc_ch();
		return 1;
	}
	case 0x34: {
		spell_feuerbann();
		return 1;
	}
	case 0x39: {
		spell_inc_ff();
		return 1;
	}
	case 0x3e: {
		spell_inc_ge();
		return 1;
	}
	case 0x43: {
		spell_inc_in();
		return 1;
	}
	case 0x48: {
		spell_inc_kk();
		return 1;
	}
	case 0x4d: {
		spell_inc_kl();
		return 1;
	}
	case 0x52: {
		spell_inc_mu();
		return 1;
	}
	case 0x57: {
		spell_mutabili();
		return 1;
	}
	case 0x5c: {
		spell_paral();
		return 1;
	}
	case 0x61: {
		spell_salander();
		return 1;
	}
	case 0x66: {
		spell_see();
		return 1;
	}
	case 0x6b: {
		spell_visibili();
		return 1;
	}
	case 0x70: {
		spell_abvenenum();
		return 1;
	}
	case 0x75: {
		spell_aeolitus();
		return 1;
	}
	case 0x7a: {
		return 0;
	}
	case 0x7f: {
		spell_claudibus();
		return 1;
	}
	case 0x84: {
		spell_dunkelheit();
		return 1;
	}
	case 0x89: {
		spell_erstarre();
		return 1;
	}
	case 0x8e: {
		spell_flimflam();
		return 1;
	}
	case 0x93: {
		spell_schmelze();
		return 1;
	}
	case 0x98: {
		spell_silentium();
		return 1;
	}
	case 0x9d: {
		spell_sturmgebr();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg103(unsigned short offs) {
	switch (offs) {
		case 0x20: { // Talentprobe
			RealPt hero = CPU_Pop32();
			unsigned short skill = CPU_Pop16();
			signed short bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(skill);
			CPU_Push32(hero);

			D1_LOG("Talentprobe : %s %+d ",
				names_skill[skill], (signed char)bonus);

			reg_ax = test_skill(Real2Host(hero), skill,
				(signed char)bonus);
			return 1;
		}
		case 0x25: {
			RealPt hero = CPU_Pop32();
			unsigned short items = CPU_Pop16();
			signed int price = CPU_Pop32();
			signed short percent = CPU_Pop16();
			unsigned short mod_init = CPU_Pop16();
			CPU_Push16(mod_init);
			CPU_Push16(percent);
			CPU_Push32(price);
			CPU_Push32(items);
			CPU_Push32(hero);

			signed char m_init = (signed char) (mod_init & 0xff);

			reg_ax = bargain(Real2Host(hero), items,
					price, percent, m_init);

			D1_LOG("bargain(%s,%d,%d,%d,%d);\n",
				schick_getCharname(hero),
				items, price, percent, m_init);

			return 1;
		}
		case 0x2a:
		case 0x34:
		case 0x43:
			 return 0;
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg105(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);
			CPU_Push16(item);
			CPU_Push32(hero);

			D1_LOG("unequip(%s, %s=%d, %d);\n",
				schick_getCharname(hero),
				get_itemname(item), item, val);

			unequip(Real2Host(hero), item, val);

			return 1;
		}
		case 0x25: {
			return 0;
		}
		case 0x2f: {
			return 0;
		}
		case 0x34: {
			RealPt hero = CPU_Pop32();
			unsigned short pos = CPU_Pop16();
			signed short nr = CPU_Pop16();
			CPU_Push16(nr);
			CPU_Push16(pos);
			CPU_Push32(hero);

			reg_ax = drop_item(Real2Host(hero), pos, nr);
			D1_LOG("drop_item(%s, %d, %d); = %d\n",
				schick_getCharname(hero), pos, nr, reg_ax);

			return 1;
		}
		case 0x39: {
			return 0;
		}
		case 0x3e: {
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = can_hero_use_item(Real2Host(hero), item);
			D1_LOG("can_hero_use_item(%s, %s); = %d\n",
				schick_getCharname(hero),
				get_itemname(item), reg_ax);

			return 1;
		}
		case 0x43: {
			unsigned short item = CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(item);

			reg_ax = can_item_at_pos(item, pos);

			D1_LOG("can_item_at_pos(%s=%d, %d); = %d\n",
				get_itemname(item), item, pos, reg_ax);

			return 1;
		}
		case 0x52: {
			return 0;
		}
		case 0x57: {
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = hero_count_item(Real2Host(hero), item);
			D1_LOG("far hero_count_item(%s, %s) = %d\n",
				schick_getCharname(hero),
				get_itemname(item), reg_ax);

			return 1;
		}
		case 0x5c: {
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);

			reg_ax = group_count_item(item);
			D1_LOG("group_count_item(%s) = %d\n",
				get_itemname(item), reg_ax);

			return 1;
		}
		case 0x61: {
			 return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

static int seg109(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			unsigned short kampf = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(kampf);

			D1_LOG("Event? : Kampf = 0x%02x 0x%02x\n", kampf, v2);
			return 0;
		}
		case 0xa2: {
			unsigned short tevent = CPU_Pop16();
			CPU_Push16(tevent);

			D1_INFO("Reisebegegnung %d\n", tevent);
			return 0;
		}
		default:
			return 0;
	}
}

static int seg120(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			return 0;
		}
		case 0x25: {
			return 0;
		}
		case 0x2a: {
			return 0;
		}
		case 0x2f: {
			return 0;
		}
		case 0x34: {
			return 0;
		}
		case 0x3e: {
			D1_LOG("refresh_colors();\n");
			refresh_colors();
			return 1;
		}
		case 0x43: {
			return 0;
		}
		case 0x48: {
			return 0;
		}
		case 0x4d: {
			return 0;
		}
		case 0x52: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

/*
	In seg122 is only an empty function.
	We bypass it and avoid unneccesarry overlay magic.
*/
static inline int seg122(unsigned short offs) {
	return 1;
}

// Intercept far CALLs (both 32 and 16 bit)
int schick_farcall_v302de(unsigned segm, unsigned offs) {

	switch (segm) {
		case 0x0000:	return seg000(offs);
		case 0x04ac:	return seg001(offs);
		case 0x051e:	return seg002(offs);
		case 0x0ae7:	return seg003(offs);
		case 0x0b2a:	return seg004(offs);
		case 0x0c85:	return seg005(offs);
		case 0x0e41:	return seg006(offs);
		case 0x0ef8:	return seg007(offs);
		case 0x0f18:	return seg008(offs);
		case 0x0ff1:	return seg009(offs);
		case 0x1030:	return seg010(offs);
		case 0x1042:	return 0;
		case 0x1112:	return seg012(offs);
		case 0x12db:	return seg024(offs);
		case 0x12de:	return 0;
		case 0x12e5:	return seg026(offs);
		case 0x12ec:	return seg027(offs);
		case 0x12f1:	return seg028(offs);
		case 0x12f9:	return seg029(offs);
		case 0x12ff:	return 0;
		case 0x1303:	return 0;
		case 0x1309:	return seg032(offs);
		case 0x130f:	return 0;
		case 0x1312:	return 0;
		case 0x1316:	return 0;
		case 0x131a:	return 0;
		case 0x131f:	return 0;
		case 0x1324:	return seg038(offs);
		case 0x1328:	return seg039(offs);
		case 0x132d:	return 0;
		case 0x1330:	return seg041(offs);
		case 0x1335:	return 0;
		case 0x1338:	return seg043(offs);
		case 0x133b:	return seg044(offs);
		case 0x133f:	return 0;
		case 0x1344:	return seg046(offs);
		case 0x1348:	return seg047(offs);
		case 0x1350:	return 0;
		case 0x1353:	return 0;
		case 0x1358:	return seg050(offs);
		case 0x135c:	return 0;
		case 0x135f:	return 0;
		case 0x1362:	return seg053(offs);
		case 0x1365:	return 0;
		case 0x1369:	return 0;
		case 0x136d:	return 0;
		case 0x1370:	return 0;
		case 0x1373:	return 0;
		case 0x1377:	return 0;
		case 0x137b:	return 0;
		case 0x137e:	return 0;
		case 0x1383:	return 0;
		case 0x1386:	return seg063(offs);
		case 0x138a:	return 0;
		case 0x138e:	return 0;
		case 0x1392:	return 0;
		case 0x139a:	return 0;
		case 0x13a1:	return seg068(offs);
		case 0x13a8:	return seg069(offs);
		case 0x13b4:	return 0;
		case 0x13b9:	return seg073(offs);
		case 0x13bd:	return 0;
		case 0x13c3:	return 0;
		case 0x13cb:	return 0;
		case 0x13d7:	return 0;
		case 0x13e4:	return 0;
		case 0x13e9:	return 0;
		case 0x1401:	return 0;
		case 0x1408:	return 0;
		case 0x140b:	return 0;
		case 0x1417:	return 0;
		case 0x141b:	return 0;
		case 0x1420:	return 0;
		case 0x1429:	return 0;
		case 0x142c:	return 0;
		case 0x1432:	return seg095(offs);
		case 0x1438:	return seg096(offs);
		case 0x1442:	return seg097(offs);
		case 0x1449:	return seg098(offs);
		case 0x144f:	return seg099(offs);
		case 0x145e:	return seg100(offs);
		case 0x1467:	return seg101(offs);
		case 0x1472:	return 0;
		case 0x147b:	return seg103(offs);
		case 0x1480:	return 0;
		case 0x1485:	return seg105(offs);
		case 0x148c:	return 0;
		case 0x1491:	return 0;
		case 0x1498:	return 0;
		case 0x149b:	return seg109(offs);
		case 0x14b4:	return 0;
		case 0x14c2:	return 0;
		case 0x14cb:	return 0;
		case 0x14d1:	return 0;
		case 0x14d8:	return 0;
		case 0x14e0:	return 0;
		case 0x14e7:	return 0;
		case 0x14ed:	return 0;
		case 0x14f0:	return seg120(offs);
		case 0x14f6:	return 0;
		case 0x14f9:	return seg122(offs);
		default:
			D1_TRAC("Unfetched Segment: 0x%04x\n", segm);
			return 0;
	}
}

static int n_seg000(unsigned offs) {
	switch (offs) {
		case 0xb33: {
			CPU_Pop16();
			Bit16u handle = CPU_Pop16();
			Bit32u pos = CPU_Pop32();
			Bit16u whence = CPU_Pop16();
			CPU_Push16(whence);
			CPU_Push32(pos);
			CPU_Push16(handle);

			Bit32s retval = bc_lseek(handle, pos, whence);

			D1_LOG("C-Lib near lseek(Handle=0x%x, pos=%u, whence=%d)\n",
				handle, pos, whence);

			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;
		}
		default:
			return 0;
	}
}

static int n_seg050(unsigned short offs) {
	switch (offs) {
		case 0x0000: {
			Bit16u cs = CPU_Pop16();
			RealPt hero = CPU_Pop32();
			Bit16u spell = CPU_Pop16();

			D1_LOG("inc_spell_advanced(%s, %s);\n",
				schick_getCharname(hero), names_spell[spell]);

			CPU_Push16(spell);
			CPU_Push32(hero);
			CPU_Push16(cs);
			return 0;
		}
		case 0x01ec: {
			Bit16u cs = CPU_Pop16();
			RealPt hero = CPU_Pop32();
			Bit16u skill = CPU_Pop16();

			D1_LOG("inc_skill_advanced(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);

			CPU_Push16(skill);
			CPU_Push32(hero);
			CPU_Push16(cs);
			return 0;
		}
		case 0x03d2: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			Bit16u skill = CPU_Pop16();

			D1_LOG("inc_skill_novice(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);
			inc_skill_novice(Real2Host(hero), skill);

			CPU_Push16(skill);
			CPU_Push32(hero);
			return 1;
		}
		case 0x04b1: {
			Bit16u cs = CPU_Pop16();
			RealPt hero = CPU_Pop32();
			Bit16u spell = CPU_Pop16();

			D1_LOG("inc_spell_novice(%s, %s);\n",
				schick_getCharname(hero), names_spell[spell]);

			CPU_Push16(spell);
			CPU_Push32(hero);
			CPU_Push16(cs);
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				__func__, offs);
			exit(1);
	}
}

int schick_nearcall_v302de(unsigned offs) {

	unsigned short segm = SegValue(cs)-reloc_game;

	/* Borland C-Lib */
	if (segm == 0) return n_seg000(offs);
	else 
	/* seg001 - CD_library */
	if (segm == 0x4ac) {
		/* Callers: 1 */
		if (offs == 0x35) {
			CPU_Pop16();
			reg_ax = CD_set_drive_nr();
			D1_LOG("CD_set_drive_nr(); = %d:\n", reg_ax);
			return 1;
		}
		/* Callers: 6 */
		if (offs == 0x5c) {
			CPU_Pop16();
			RealPt req = CPU_Pop32();
			CPU_Push32(req);

			CD_driver_request(req);
			D1_LOG("CD_driver_request();\n");
			return 1;
		}
		/* Callers: 4 */
		if (offs == 0xb2) {
			CPU_Pop16();
			unsigned int retval;

			retval = CD_get_tod();

			D1_LOG("CD_get_tod(); = %d\n", retval);
			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;
			return 1;
		}
		/* Callers: 2 */
		if (offs == 0xc1) {
			CPU_Pop16();
			unsigned short track_nr = CPU_Pop16();
			CPU_Push16(track_nr);
			D1_LOG("seg001_00c1(track_nr = %d)\n", track_nr);
			seg001_00c1(track_nr);
			return 1;
		}
		/* Callers: 3 */
		if (offs == 0x322) {
			CPU_Pop16();
			D1_LOG("seg001_0322()\n");
			seg001_0322();
			return 1;
		}
		/* Callers: 1 */
		if (offs == 0x34f) {
			CPU_Pop16();
			D1_LOG("seg001_034f()\n");
			seg001_034f();
			return 1;
		}
		return 0;
	}

	/* seg002 - often used */
	if (segm == 0x51e) {

		switch (offs) {
		case 0x0832: {
			CPU_Pop16();
			Bit16u index = CPU_Pop16();
			D1_LOG("near play_voc(FX%d)\n", index - 288);
			play_voc(index);
			CPU_Push16(index);
			return 1;
		}
		case 0x0b7e: {
			CPU_Pop16();
			unsigned short fileindex = CPU_Pop16();
			CPU_Push16(fileindex);

			reg_ax = open_and_seek_dat(fileindex);

			D1_LOG("open_and_seek_dat(%s);\n",
				get_fname(fileindex));

			return 1;
		}
		case 0x0c28: {
			CPU_Pop16();
			Bit16u handle = CPU_Pop16();
			RealPt buf = CPU_Pop32();
			Bit16u len = CPU_Pop16();
			CPU_Push16(len);
			CPU_Push32(buf);
			CPU_Push16(handle);

			D1_LOG("near read_archive_file(%d, %x, %d);\n",
				handle, buf, len);
			reg_ax = read_archive_file(handle,
					Real2Host(buf), len);

			return 1;
		}
		case 0x0c72: {
			CPU_Pop16();
			Bit16u handle = CPU_Pop16();
			Bit32u off = CPU_Pop32();
			CPU_Push32(off);
			CPU_Push16(handle);

			D1_LOG("near seg002_0c72(%d, %d)\n", handle, off);
			seg002_0c72(handle, off);

			return 1;
		}
		case 0x1361: {
			CPU_Pop16();
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			RealPt p3 = CPU_Pop32();
			RealPt p4 = CPU_Pop32();
			RealPt p5 = CPU_Pop32();
			CPU_Push32(p5);
			CPU_Push32(p4);
			CPU_Push32(p3);
			CPU_Push32(p2);
			CPU_Push32(p1);

			D1_LOG("mouse_action()\n");

			mouse_action(Real2Host(p1),
				Real2Host(p2),
				Real2Host(p3),
				Real2Host(p4),
				Real2Host(p5));

			return 1;
		}
		/* Callers: 2 */
		case 0x1634: {
			CPU_Pop16();
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			unsigned short v3 = CPU_Pop16();
			unsigned short v4 = CPU_Pop16();
			CPU_Push16(v4);
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			reg_ax  = is_mouse_in_rect(v1, v2, v3, v4);
			D1_LOG("near is_mouse_in_rect(%d, %d, %d, %d); = %d \n",
				v1, v2, v3, v4, reg_ax);

			return 1;
		}
		/* Callers: 2 */
		case 0x19dc: {
			CPU_Pop16();
			D1_LOG("mouse_19dc()\n");
			mouse_19dc();
			return 1;
		}
		/* Callers: 2 */
		case 0x1cf2: {
			CPU_Pop16();
			unsigned short x = CPU_Pop16();
			unsigned short y = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(y);
			CPU_Push16(x);

			reg_ax = get_mouse_action(x, y, Real2Host(p));

			D1_LOG("near get_mouse_action(x=%d, y=%d, p=%x) = %x;\n",
				x, y, p, reg_ax);
			return 1;
		}
		case 0x1d67: {
			CPU_Pop16();
			D1_LOG("handle_input();\n");
			handle_input();
			return 1;
		}
		/* Callers: 1 */
		case 0x20bd: {
			CPU_Pop16();
			timers_daily();
			D1_LOG("timers_daily();\n");
			return 1;
		}
		/* Callers: 1 */
		case 0x2177: {
			CPU_Pop16();
			seg002_2177();
			D1_LOG("seg002_2177();\n");
			return 1;
		}
		/* Callers: 2 */
		case 0x21ab: {
			CPU_Pop16();
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			CPU_Push32(p2);
			CPU_Push32(p1);
			D1_LOG("pal_fade(%x,%x);\n", p1, p2);
			pal_fade(Real2Phys(p1), Real2Phys(p2));
			return 1;
		}
		/* Callers: 1 */
		case 0x25ce: {
			CPU_Pop16();
			reg_ax = get_current_season();
			return 1;
		}
		/* Callers: 1 */
		case 0x2628: {
			CPU_Pop16();
			D1_LOG("do_census();\n");
			do_census();
			return 1;
		}
		/* Callers: 4 */
		case 0x2bf6: {
			CPU_Pop16();
			unsigned int val = CPU_Pop32();
			CPU_Push32(val);
			D1_LOG("near sub_ingame_timers(val = %u);\n", val);
			sub_ingame_timers(val);
			return 1;
		}
		/* Callers: 4 */
		case 0x2c5e: {
			CPU_Pop16();
			unsigned int val = CPU_Pop32();
			CPU_Push32(val);

			D1_LOG("sub_mod_timers(%d);\n", val);
			sub_mod_timers(val);
			return 1;
		}
		/* Callers: 4 */
		case 0x2f7a: {
			CPU_Pop16();
			unsigned int val = CPU_Pop32();
			CPU_Push32(val);
			D1_LOG("near seg002_2f7a(fmin=%d);\n", val);
			seg002_2f7a(val);
			return 1;
		}
		/* Callers: 4 */
		case 0x3071: {
			CPU_Pop16();
			unsigned short quarter = CPU_Pop16();
			signed short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(quarter);

			D1_LOG("near sub_light_timers(quarter=%d, v2=%d);\n",
				quarter, v2);
			sub_light_timers(quarter, v2);

			return 1;
		}
		/* Callers: 1 */
		case 0x3b63: {
			CPU_Pop16();
			seg002_3b63();
			D1_LOG("seg002_3b63();\n");
			return 1;
		}
		/* Callers: 1 */
		case 0x3c63: {
			CPU_Pop16();
			seg002_3c63();
			D1_LOG("seg002_3c63();\n");
			return 1;
		}
		/* Callers: 2 */
		case 0x3f3e: {
			CPU_Pop16();
			unsigned short index = CPU_Pop16();
			unsigned short type = CPU_Pop16();
			CPU_Push16(type);
			CPU_Push16(index);

			D1_LOG("draw_splash(%d, %d);\n", index, type);
			draw_splash(index, type);
			return 1;
		}
		/* Callers: 1 */
		case 0x4016: {
			CPU_Pop16();
			D1_LOG("near wait_for_keyboard2()\n");
			wait_for_keyboard2();
			return 1;
		}
		/* Callers: 2 */
		case 0x47e2: {
			CPU_Pop16();
			seg002_47e2();
			D1_INFO("seg002_47e2();\n");
			return 1;
		}
		/* Callers: 2 */
		case 0x484f: {
			CPU_Pop16();
			seg002_484f();
			D1_INFO("seg002_484f();\n");
			return 1;
		}
		/* Callers: 2 */
		case 0x49d8: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = is_hero_available_in_group(Real2Host(hero));
			D1_LOG("is_hero_available_in_group(%s) = %d\n",
			schick_getCharname(hero), reg_ax);

			return 1;
		}
		/* Callers: 1 */
		case 0x4f49: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			Bit16u index = CPU_Pop16();
			Bit16u type = CPU_Pop16();

			D1_LOG("do_starve_damage(%s, %d, %d);\n",
				schick_getCharname(hero), index, type);

			do_starve_damage(Real2Host(hero), index, type);

			CPU_Push16(type);
			CPU_Push16(index);
			CPU_Push32(hero);

			return 1;
		}
		/* Callers: 1 */
		case 0x55b1: {
			CPU_Pop16();
			unsigned short item = CPU_Pop16();
			Bitu group = CPU_Pop16() & 0xff;
			CPU_Push16(group);
			CPU_Push16(item);

			reg_ax = get_first_hero_with_item_in_group(item, (signed char)group);
			D1_LOG("get_first_hero_with_item_in_group(%s = (%d), %d) = %d\n",
				get_itemname(item), item, group,
				(short)reg_ax);

			return 1;
		}
		/* Callers: 2 */
		case 0x573e: {
			CPU_Pop16();
			reg_ax = count_heros_available();
			D1_LOG("count_heros_available() = %d;\n", reg_ax);
			return 1;
		}
		/* Callers: 3 */
		case 0x5799: {
			CPU_Pop16();
			reg_ax = count_heroes_available_in_group();
			D1_LOG("count_heroes_available_in_group() = %d;\n",
				reg_ax);
			return 1;
		}
		/* Callers: 1 */
		case 0x5a81: {
			CPU_Pop16();
			reg_ax = 1;
			D1_LOG("Kopierschutzabfrage umgangen\n");
			return 1;
		}
		default:
			return 0;
		}
	}
	/* seg004 */
	if (segm == 0xb2a) {
		switch (offs) {
			case 0x55: {
				CPU_Pop16();
				Bit16u v1 = CPU_Pop16();

				D1_LOG("near init_ani(%d)\n", v1);
				init_ani(v1);

				CPU_Push16(v1);
				return 1;
			}
			case 0x11da: {
				CPU_Pop16();
				D1_LOG("clear_ani_pal()\n");
				clear_ani_pal();
				return 1;
			}
			case 0x1209: {
				CPU_Pop16();
				RealPt pal = CPU_Pop32();
				D1_LOG("set_ani_pal()\n");
				set_ani_pal(Real2Host(pal));
				CPU_Push32(pal);
				return 1;
			}
			case 0x150d: {
				CPU_Pop16();
				D1_LOG("wait_for_vsync()\n");
				wait_for_vsync();
				return 1;
			}
			default:
				return 0;
		}
	}
	/* seg005 */
	if (segm == 0xc85) {
		switch (offs) {
		case 0xb: {
			CPU_Pop16();
			RealPt p = CPU_Pop32();
			signed short x = CPU_Pop16();
			signed short y = CPU_Pop16();
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push32(p);

			reg_ax = FIG_obj_needs_refresh(Real2Host(p), x, y);
			D1_LOG("FIG_obj_needs_refresh(%x, x=%d, y=%d); = %d\n",
				p, x, y, reg_ax);

			return 1;
		}
		case 0x144: {
			CPU_Pop16();
			RealPt p = CPU_Pop32();
			unsigned short count = CPU_Pop16();
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);
			CPU_Push16(count);
			CPU_Push32(p);

			FIG_set_star_color(Real2Phys(p), count, val & 0xff);
			D1_LOG("FIG_set_star_color(%x,%d,%d)\n",
				p, count, val & 0xff);

			return 1;
		}
		case 0x181: {
			CPU_Pop16();
			unsigned short type = CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(type);

			RealPt retval = FIG_name_3rd_case(type, pos);
			D1_LOG("FIG_name_3rd_case(%d,%d) = %s\n",
				type, pos, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);

			return 1;
		}
		case 0x1b6: {
			CPU_Pop16();
			unsigned short type = CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(type);

			RealPt retval = FIG_name_4th_case(type, pos);
			D1_LOG("FIG_name_4th_case(%d,%d) = %s\n",
				type, pos, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);

			return 1;
		}
		case 0x1eb: {
			CPU_Pop16();
			unsigned short type = CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(type);

			RealPt retval = FIG_name_1st_case(type, pos);
			D1_LOG("FIG_name_1st_case(%d,%d) = %s\n",
				type, pos, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);

			return 1;
		}
		case 0x220: {
			CPU_Pop16();
			reg_ax = fight_printer();
			D1_LOG("fight_printer()\n");
			return 1;
		}
		case 0x1ba7: {
			CPU_Pop16();
			set_delay_timer();
			D1_LOG("set_delay_timer()\n");
			return 1;
		}
		case 0x1bb2: {
			CPU_Pop16();
			fight_delay();
			D1_LOG("fight_delay()\n");
			return 1;
		}
		default:
			return 0;
		}
	}
	/* Callers: 1 */
	if ((segm == 0xe41) && (offs == 0x5a)) {
		CPU_Pop16();

		reg_ax = FIG_set_array();

		D1_LOG("FIG_set_array(); = %d\n", (char)reg_ax);
		return 1;
	}
	/* seg024 */
	if (is_ovrseg(0x12db)) {
		switch (offs) {
		case 0x1d3: {
			CPU_Pop16();
			Bit16u line = CPU_Pop16();
			CPU_Push16(line);

			reg_ax = diary_print_entry(line);
			D1_LOG("diary_print_entry(%d); == %d\n", line, reg_ax);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg024", offs);
			exit(1);
		}
	}
	/* seg026 */
	if (is_ovrseg(0x12e5)) {
		switch (offs) {
		case 0x23e: {
			CPU_Pop16();
			RealPt src = CPU_Pop32();
			RealPt dst = CPU_Pop32();
			Bit32u len = CPU_Pop32();

			D1_LOG("split_textbuffer(%x, %x, %d)\n",
				src, dst, len);
			split_textbuffer(Real2Host(src), dst, len);
			CPU_Push32(len);
			CPU_Push32(dst);
			CPU_Push32(src);
			return 1;
		}
		case 0x2d3: {
			CPU_Pop16();
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			CPU_Push32(p2);
			CPU_Push32(p1);
			D1_LOG("prepare_chr_name(%x, %s);\n", p1,
				(char*)Real2Host(p2));
			prepare_chr_name((char*)Real2Host(p1),
				(char*)Real2Host(p2));
			return 1;
		}
		case 0x347: {
			CPU_Pop16();
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			CPU_Push32(p2);
			CPU_Push32(p1);
			D1_LOG("prepare_sg_name(%x, %s);\n", p1, (char*)Real2Host(p2));
			prepare_sg_name((char*)Real2Host(p1),
				(char*)Real2Host(p2));
			return 1;
		}
		case 0x1021: {
			return 0;
		}
		case 0x117f: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg024", offs);
			exit(1);
		}
	}

	/* seg029 */
	if (is_ovrseg(0x12f9)) {
		switch (offs) {
		case 0x000: {
			CPU_Pop16();
			D1_LOG("draw_playmask();\n");
			draw_playmask();
			return 1;
		}
		case 0x0e8: {
			CPU_Pop16();
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			CPU_Push32(p2);
			CPU_Push32(p1);

			copy_forename(Real2Host(p1),
				Real2Host(p2));
			D1_LOG("near copy_forename(%s, %s)\n",
				(char*)Real2Host(p1),
				(char*)Real2Host(p2));
			return 1;
		}
		case 0x127: {
			CPU_Pop16();
			D1_LOG("draw_status_line();\n");
			draw_status_line();
			return 1;
		}
		case 0x417: {
			CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);

			clear_hero_icon(pos);
			D1_LOG("clear_hero_icon(%d)\n", pos);
			return 1;
		}
		case 0x492: {
			CPU_Pop16();
			Bit16u index = CPU_Pop16();
			Bit16u icon = CPU_Pop16();
			Bit16u pos = CPU_Pop16();

			D1_LOG("load_icon(%x, %x, %x);\n",
				index, icon, pos);
			load_icon(index, icon, pos);

			CPU_Push16(pos);
			CPU_Push16(icon);
			CPU_Push16(index);

			return 1;
		}
		case 0x4fd: {
			CPU_Pop16();
			D1_LOG("draw_icons();\n");
			draw_icons();
			return 1;
		}
		case 0x5ff: {
			CPU_Pop16();
			D1_LOG("near clear_loc_line();\n");
			clear_loc_line();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg029", offs);
			exit(1);
		}
	}
	/* seg032 */
	if (is_ovrseg(0x1309)) {
		switch (offs) {
		/* Callers: 1 */
		case 0x25: {
			CPU_Pop16();
			signed short row = CPU_Pop16();
			signed short col = CPU_Pop16();
			signed short object = CPU_Pop16();
			CPU_Push16(object);
			CPU_Push16(col);
			CPU_Push16(row);

			signed char obj = object & 0xff;

			FIG_set_cb_field(row, col, obj);
			D1_LOG("FIG_set_cb_field(row=%d,col=%d,object=%d);\n",
				row, col, obj);
			return 1;
		}
		/* Callers: 1 */
		case 0xa8: {
			CPU_Pop16();
			reg_ax = FIG_choose_next_hero();
			D1_LOG("FIG_choose_next_hero() = %s\n",
				schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da));
			return 1;
		}
		/* Callers: 1 */
		case 0xfc: {
			CPU_Pop16();
			reg_ax = FIG_choose_next_enemy();
			D1_LOG("FIG_choose_next_enemy() = %d\n", reg_ax);
			return 1;
		}
		/* Callers: 1 */
		case 0x12c: {
			CPU_Pop16();
			reg_ax = FIG_count_active_enemies();
			D1_LOG("near FIG_count_active_enemies() = %d\n", reg_ax);
			return 1;
		}
		/* Callers: 1 */
		case 0x1ba: {
			CPU_Pop16();
			RealPt enemy = CPU_Pop32();
			CPU_Push32(enemy);

			reg_ax = FIG_is_enemy_active(Real2Host(enemy) );
			D1_LOG("near FIG_is_enemy_active(); = %d\n", reg_ax);
			return 1;
		}
		/* Callers: 2 */
		case 0x242: {
			CPU_Pop16();
			reg_ax = FIG_get_first_active_hero();
			D1_LOG("near FIG_get_first_active_hero() = %s\n",
				reg_ax != -1 ? schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da) : "none");
			return 1;
		}
		/* Callers: 1 */
		case 0x2db: {
			CPU_Pop16();
			reg_ax = seg032_02db();
			D1_LOG("near seg032_02db() = %d\n", reg_ax);
			return 1;
		}
		/* Callers: 1 */
		case 0x349: {
			CPU_Pop16();
			reg_ax = FIG_fight_continues();
			D1_LOG("FIG_fight_continues() = %d\n", reg_ax);
			return 1;
		}
		default:
			return 0;
		}
	}
	/* seg038 */
	if (is_ovrseg(0x1324)) {
		switch (offs) {
		case 0x000: {
			CPU_Pop16();
			signed short obj = CPU_Pop16();
			RealPt px = CPU_Pop32();
			RealPt py = CPU_Pop32();
			CPU_Push32(py);
			CPU_Push32(px);
			CPU_Push16(obj);

			reg_ax = FIG_search_obj_on_cb(obj,
					Real2Host(px),
					Real2Host(py));

			D1_LOG("near FIG_search_obj_on_cb(obj=%d, x=%d, y=%d) = %d\n",
				obj, mem_readw(Real2Phys(px)),
				mem_readw(Real2Phys(py)), reg_ax);

			return 1;
		}
		case 0x143: {
			return 0;
		}
		case 0x457: {
			CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);

			reg_ax = FIG_count_smth(Real2Host(p));
			D1_LOG("FIG_count_smth(%x) = %d\n", p, reg_ax);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg038", offs);
			exit(1);
		}
	}
	/* seg039 */
	if (is_ovrseg(0x1328)) {
		switch(offs) {
		case 0x23: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = seg039_0023(Real2Host(hero));
			D1_LOG("seg039_0023(%s) = %d\n",
				schick_getCharname(hero), (signed short)reg_ax);

			return 1;
		}
		case 0x97: {
			CPU_Pop16();
			unsigned short sheet_nr = CPU_Pop16();
			unsigned short enemy_id_16 = CPU_Pop16();
			unsigned short round_16 = CPU_Pop16();
			CPU_Push16(round_16);
			CPU_Push16(enemy_id_16);
			CPU_Push16(sheet_nr);

			signed char enemy = (signed char)(enemy_id_16 & 0xff);
			signed char round = (signed char)(round_16 & 0xff);

			D1_LOG("near fill_enemy_sheet(%d, %d, %d);\n",
				sheet_nr, enemy, round);

			fill_enemy_sheet(sheet_nr, enemy, round);

			return 1;
		}
		case 0x317: {
			CPU_Pop16();
			unsigned short x = CPU_Pop16();
			unsigned short y = CPU_Pop16();
			signed short object = CPU_Pop16();
			unsigned short v2_16 = CPU_Pop16();
			unsigned short dir_16 = CPU_Pop16();
			CPU_Push16(dir_16);
			CPU_Push16(v2_16);
			CPU_Push16(object);
			CPU_Push16(y);
			CPU_Push16(x);

			signed char v2 = (signed char)(v2_16 & 0xff);
			signed char dir = (signed char)(dir_16 & 0xff);

			reg_ax = place_obj_on_cb(x, y, object, v2, dir);
			D1_LOG("place_obj_on_cb(x=%d,y=%d,obj=%d,%d,dir=%d); = %d\n",
				x, y, object, v2, dir, reg_ax);

			return 1;
		}
		default:
			return 0;
		}
	}
	/* seg041 */
	if (is_ovrseg(0x1330)) {
		switch (offs) {
		case 0x8f1: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = weapon_check(Real2Host(hero));

			D1_LOG("near weapon_check(%s); = %d\n",
				schick_getCharname(hero), (signed short)reg_ax);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg041", offs);
			exit(1);
		}
	}
	/* seg044 */
	if (is_ovrseg(0x133b)) {
		switch (offs) {
		case 0x00: {
			CPU_Pop16();
			RealPt p = CPU_Pop32();
			Bit16s a2 = CPU_Pop16();
			Bit16u a3 = CPU_Pop16();

			reg_ax = copy_ani_seq(Real2Host(p), a2, a3);
			D1_LOG("copy_ani_seq(%x, %d, %d) = 0x%x\n",
				p, a2, a3, reg_ax);

			CPU_Push16(a3);
			CPU_Push16(a2);
			CPU_Push32(p);

			return 1;
		}
		case 0xae: {
			CPU_Pop16();

			Bit16s ani = CPU_Pop16();

			reg_ax = seg044_00ae(ani);
			D1_LOG("seg44_00ae(%d) = %d\n", ani, reg_ax);

			CPU_Push16(ani);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg044", offs);
			exit(1);
		}
	}
	/* seg046 */
	if (is_ovrseg(0x1344)) {
		switch (offs) {
		case 0x000: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short spell = CPU_Pop16();
			unsigned short fsig = CPU_Pop16();
			unsigned short x1 = CPU_Pop16();
			unsigned short x2 = CPU_Pop16();
			unsigned short yg = CPU_Pop16();
			CPU_Push16(yg);
			CPU_Push16(x2);
			CPU_Push16(x1);
			CPU_Push16(fsig);
			CPU_Push16(spell);
			CPU_Push32(hero);

			D1_LOG("status_show_spell(%s, %d,%d,%d,%d,%d);\n",
				schick_getCharname(hero), spell, fsig, x1, x2, yg);
			status_show_spell(Real2Host(hero), spell,
				fsig, x1, x2, yg);

			return 1;
		}
		case 0x08d: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short talent = CPU_Pop16();
			unsigned short ftig = CPU_Pop16();
			unsigned short x1 = CPU_Pop16();
			unsigned short x2 = CPU_Pop16();
			unsigned short yg = CPU_Pop16();
			CPU_Push16(yg);
			CPU_Push16(x2);
			CPU_Push16(x1);
			CPU_Push16(ftig);
			CPU_Push16(talent);
			CPU_Push32(hero);

			D1_LOG("status_show_spell(%s, %d,%d,%d,%d,%d);\n",
				schick_getCharname(hero), talent, ftig, x1, x2, yg);
			status_show_talent(Real2Host(hero), talent,
				ftig, x1, x2, yg);

			return 1;
		}
		case 0x11a: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			D1_LOG("status_show_talents(%s);\n",
				schick_getCharname(hero));

			status_show_talents(Real2Host(hero));

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg046", offs);
			exit(1);
		}
	}

	if (is_ovrseg(0x1358))
		return n_seg050(offs);
	/* seg095 */
	if (is_ovrseg(0x1432)) {
		switch (offs) {
			case 0x335: {
				CPU_Pop16();
				npc_nariell();
				return 1;
			}
			case 0x44d: {
				CPU_Pop16();
				npc_harika();
				return 1;
			}
			case 0x62d: {
				CPU_Pop16();
				npc_curian();
				return 1;
			}
			case 0x746: {
				CPU_Pop16();
				npc_ardora();
				return 1;
			}
			case 0x9a5: {
				CPU_Pop16();
				npc_garsvik();
				return 1;
			}
			case 0xad0: {
				CPU_Pop16();
				npc_erwo();
				return 1;
			}
			case 0xbfb: {
				CPU_Pop16();
				Bit16s head_index = CPU_Pop16();
				Bit16s days = CPU_Pop16();
				Bit16s index = CPU_Pop16();
				RealPt name = CPU_Pop32();
				RealPt text = CPU_Pop32();

				remove_npc(head_index, (signed char)days, index,
					Real2Host(name), Real2Host(text));

				D1_INFO("remove_npc(%x, %d, %x, %x, %x);\n",
					head_index, (signed char)days, index,
					name, text);

				CPU_Push32(text);
				CPU_Push32(name);
				CPU_Push16(index);
				CPU_Push16(days);
				CPU_Push16(head_index);
				return 1;
			}
			case 0xcb8: {
				CPU_Pop16();
				Bit16s index = CPU_Pop16();
				D1_LOG("add_npc(%s);\n", get_fname(index));
				add_npc(index);
				CPU_Push16(index);
				return 1;
			}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg046", offs);
			exit(1);
		}
	}
	/* seg097 */
	if (is_ovrseg(0x1442)) {
		switch (offs) {
#if 0
		/* Callers: 0 */
		case 0x000:
		/* Callers: 1 */
		case 0x129:
			return 0;
		/* Callers: 1 */
		case 0x15e: {
			CPU_Pop16();
			unsigned short c = CPU_Pop16();
			RealPt p_height = CPU_Pop32();
			CPU_Push32(p_height);
			CPU_Push16(c);

			reg_ax = GUI_lookup_char_height(c & 0xff, (unsigned short*)Real2Host(p_height));
			D1_LOG("GUI_lookup_char_height() = %d\n", (char)reg_ax);
			return 1;
		}
		/* Callers: 1 */
		case 0x1c2:
#endif
		/* Callers: 1 */
		case 0x1f8: {
			CPU_Pop16();
			RealPt dst = CPU_Pop32();
			unsigned short x = CPU_Pop16();
			unsigned short y = CPU_Pop16();
			unsigned short num = CPU_Pop16();
			unsigned short v4 = CPU_Pop16();
			CPU_Push16(v4);
			CPU_Push16(num);
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push32(dst);

			D1_LOG("GUI_1f8(0x%x, %d, %d, %d, %d);",
				dst, x, y, num, v4);

			reg_ax = GUI_enter_text(Real2Host(dst),
					x, y, num, v4);
			D1_LOG(" = 0x%x\n", reg_ax);

			return 1;
		}
		/* Callers: 3 */
		case 0x4ae: {
			CPU_Pop16();
			Bit16u v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			Bit16u v3 = CPU_Pop16();
			Bit16u v4 = CPU_Pop16();

			D1_LOG("GUI_draw_radio_bg(%d, %d, %d, %d);\n",
				v1, v2, v3, v4);
			GUI_draw_radio_bg(v1, v2, v3, v4);

			CPU_Push16(v4);
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			return 1;
		}
		/* Callers: 3 */
		case 0x564: {
			CPU_Pop16();
			unsigned short width = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			CPU_Push16(height);
			CPU_Push16(width);

			D1_LOG("GUI_copy_smth(%d, %d)\n", width, height);
			GUI_copy_smth(width, height);

			return 1;
		}
		/* Callers: 1 */
		case 0x59f: {
			CPU_Pop16();
			RealPt str = CPU_Pop32();
			CPU_Push32(str);

			D1_LOG("GUI_output()\n");
			GUI_output(Real2Host(str));
			return 1;
		}
		/* Callers: 1 */
		case 0x5b4: {
			CPU_Pop16();
			RealPt str = CPU_Pop32();
			unsigned short num = CPU_Pop16();
			CPU_Push16(num);
			CPU_Push32(str);

			D1_LOG("GUI_input()\n");
			reg_ax = GUI_input(Real2Host(str), num);

			return 1;
		}
		/* Callers: 1 */
		case 0x7f4: {
			CPU_Pop16();
			Bit16s v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			Bit16u v3 = CPU_Pop16();
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_LOG("GUI_fill_radio_button(%d, %d, %d);\n",
				v1, v2, v3);

			GUI_fill_radio_button(v1, v2, v3);

			return 1;

		}
		/* Callers: 1 */
		case 0x893:
			return 0;
		/* Callers: 2 */
		case 0xb43: {
			CPU_Pop16();
			Bit16u v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			Bit16u v3 = CPU_Pop16();
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_LOG("GUI_menu_input(%d, %d, %d);\n", v1, v2, v3);

			reg_ax = GUI_menu_input(v1, v2, v3);

			return 1;
		}
		/* Callers: 1 */
		case 0xd45:
			   return 0;
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg097", offs);
			exit(1);
		}
	}
	/* Callers: 2 */
	if (is_ovrseg(0x147b) && (offs == 0x040F)) {
		// Talentprobe
		CPU_Pop16();
		RealPt hero = CPU_Pop32();
		unsigned skill = CPU_Pop16();
		signed bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(skill);
		CPU_Push32(hero);

		D1_LOG("Talentprobe: %s %+d\n ",
			names_skill[skill], (char)bonus);

		reg_ax = test_skill(Real2Host(hero), skill, bonus);

		return 1;
	}
	/* seg098 */
	if (is_ovrseg(0x1449)) {
		switch (offs) {

		/* Callers: 1 */
		case 0x0000 : {
			return 0;
		}
		/* Callers: 4 */
		case 0x0339 : {
			CPU_Pop16();
			unsigned short spell = CPU_Pop16();
			unsigned short half_cost = CPU_Pop16();
			CPU_Push16(half_cost);
			CPU_Push16(spell);

			reg_ax = get_spell_cost(spell, half_cost);

			D1_LOG("get_spell_cost(%s, %d) = %d\n",
				names_spell[spell], half_cost, (short)reg_ax);

			return 1;
		}
		/* Callers: 1 */
		case 0x071d : {
			return 0;
		}
		/* Callers: 1 */
		case 0x0786 : {
			return 0;
		}
		/* Callers: 1 */
		case 0x0e1f : {
			// Zauberprobe
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned spell = CPU_Pop16();
			signed bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(spell);
			CPU_Push32(hero);

			D1_LOG("Zauberprobe: %s %+d ",
				names_spell[spell], (signed char)bonus);

			reg_ax = test_spell(Real2Host(hero),
				spell, (signed char)bonus);
			return 1;
		}
		/* Callers: 2 */
		case 0x1000 : {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg098", offs);
			exit(1);
		}
	}
	/* seg105 */
	if (is_ovrseg(0x1485)) {
		switch (offs) {
		case 0x000: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			unsigned short pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(item);
			CPU_Push32(hero);

			D1_LOG("unequip(%s, %s, %d);\n",
				schick_getCharname(hero),
				get_itemname(item), pos);
			unequip(Real2Host(hero), item, pos);

			return 1;
		}
		case 0x3aa: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = has_hero_stacked(Real2Host(hero), item);
			D1_LOG("has_hero_stacked(%s, %s) = %d\n",
				schick_getCharname(hero),
				get_itemname(item), (signed short)reg_ax)
;
			return 1;
		}
		case 0x3e8: {
			return 0;
		}
		case 0x675: {
			CPU_Pop16();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);

			reg_ax = item_pleasing_ingerimm(item);
			D1_LOG("item_pleasing_ingerimm(%s); = %d\n",
				get_itemname(item), reg_ax);

			return 1;
		}
		case 0x6d9: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short pos = CPU_Pop16();
			signed short nr = CPU_Pop16();
			CPU_Push16(nr);
			CPU_Push16(pos);
			CPU_Push32(hero);

			reg_ax = drop_item(Real2Host(hero), pos, nr);
			D1_LOG("drop_item(%s, %d, %d); = %d\n",
				schick_getCharname(hero), pos, nr, reg_ax);

			return 1;
		}
		case 0xada: {
			CPU_Pop16();
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = hero_count_item(Real2Host(hero), item);
			D1_INFO("hero_count_item(%s, %s) = %d\n",
				schick_getCharname(hero),
				get_itemname(item), reg_ax)
;
			return 1;
		}
		case 0xc10: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg105", offs);
			exit(1);
		}
	}
	/* seg120 */
	if (is_ovrseg(0x14f0)) {
		switch (offs) {
		case 0x578: {
			return 0;
		}
		case 0x99f: {
			CPU_Pop16();
			D1_LOG("refresh_colors();\n");
			refresh_colors();
			return 1;
		}
		case 0xd85: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",
				"seg120", offs);
			exit(1);
		}
	}

	return 0;
}
