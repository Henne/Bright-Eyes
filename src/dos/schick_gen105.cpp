#include <stdio.h>
#include <mem.h>

#include "schick.h"

// Hooks for tracing far calls for GEN.EXE(de/V1.05)
int schick_farcall_gen105(unsigned segm, unsigned offs, unsigned ss, unsigned sp)
{

	/* _decomp() */
	if (segm == 0xb39) {
		if (offs == 0xc)
		{
			D1_LOG("_decomp(%x:%x, %x:%x, 0x%lx)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readd(ss, sp+8));
			return 0;
		}
		return 0;
	}

	/* Rasterlib */
	if (segm == 0xb6b)
	{
		if (offs == 0x8) {
			D1_GFX("SetVideoMode(mode=0x%x);\n",
				real_readw(ss, sp));
				return 0;
		}
		if (offs == 0x1e) {
			D1_GFX("SetDisplayPage(page=0x%x);\n",
				real_readw(ss, sp));
				return 0;
		}
		if (offs == 0x34) {
			D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
				real_readw(ss, sp+2), real_readw(ss, sp));
				return 0;
		}
		if (offs == 0xde) {
			unsigned short seg=real_readw(ss, sp+2);
			unsigned short off=real_readw(ss, sp);

			D1_GFX("SetColor(rgb=0x%x:0x%x, color=0x%x);\n",
				seg, off, real_readw(ss, sp+4));
			D1_GFX("RGB=(0x%x, 0x%x, 0x%x);\n",
				real_readb(seg, off), real_readb(seg, off+1),
				real_readb(seg, off+2));
				return 0;
		}
		if (offs == 0x100) {
			unsigned short off=real_readw(ss, sp);
			unsigned short seg=real_readw(ss, sp+2);
			unsigned short first_color=real_readw(ss, sp+4);
			unsigned short colors=real_readw(ss, sp+6);
			unsigned short i;

			D1_GFX("SetPalette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
				seg, off, first_color, colors);
			for (i=0; i<colors; i++)
			{
				D1_LOG("\"\\%02d\\%02d\\%02d\"..\n",
					real_readb(seg, off+i*3),
					real_readb(seg, off+i*3+1),
					real_readb(seg, off+i*3+2));
			}
			return 0;
		}

		if (offs == 0x184) {
			unsigned short offset=real_readw(ss, sp);
			unsigned short n=real_readw(ss, sp+2);
			unsigned short color=real_readw(ss, sp+4);
			unsigned short space=real_readw(ss, sp+6);

			D1_GFX("DrawDots(offset=0x%x, n=0x%x, color=0x%x, space=%d);\t/*X=%d Y=%d*/\n",
				offset, n, color, space,
				offset%320, offset/320);
			return 0;
		}

		if (offs == 0x1f3)
		{
			D1_GFX("DrawPic(Dest=0x%x:0x%x, X=%d, Y=%d, ..., Breite=%d, Höhe=%d, Src=0x%x:0x%x, Mode=%d);\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+4), real_readw(ss, sp+6),
				real_readw(ss, sp+20), real_readw(ss, sp+22),
				real_readw(ss, sp+26), real_readw(ss, sp+24),
				real_readw(ss, sp+28));
			return 0;
		}
		if (offs == 0x2e3) {
			unsigned short off=real_readw(ss, sp+2);
			unsigned short seg=real_readw(ss, sp);

			D1_GFX("FillRect(segment=0x%x, offset=0x%x, color=0x%x, width=%d, height=%d);",
				seg, off, real_readw(ss, sp+4),
				real_readw(ss, sp+6), real_readw(ss, sp+8));

			if (seg == 0xa000)
				D1_GFX("\t/*X=%d Y=%d*/\n",
					off%320, off/320);
			else
				D1_GFX("\n");

			return 0;
		}

		if (offs == 0x39f) {
			D1_GFX("_39F(src=0x%x:0x%x, dst=0x%x:0x%x, a=%d, b=%d, c=%d);\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readw(ss, sp+8), real_readw(ss, sp+10),
				real_readw(ss, sp+12));
			return 0;
		}
		D1_GFX("Rasterlib:0x%x\n", offs);
		return 0;
	}


	if (segm == 0x0) {
		if (offs == 0x0438) {
			D1_TRAC("_dos_getvect(intnr=0x%x)\n", real_readw(ss,sp));
			return 0;
		}
		if (offs == 0x0447) {
			D1_TRAC("_dos_setvect(intnr=0x%x, *isr=0x%x:0x%x)\n",
				real_readw(ss, sp), real_readw(ss, sp+4),
				real_readw(ss, sp+2));
			return 0;
		}
		if (offs == 0x07c5) {
			D1_LOG(
			"__read(Handle=0x%x, Buffer=0x%x:0x%x, Length=%d)\n",
			real_readw(ss, sp), real_readw(ss, sp+4),
			real_readw(ss, sp+2), real_readw(ss, sp+6));
			return 0;
		}
		if (offs == 0x20bc) {
			D1_LOG("close(Handle=0x%x)\n", real_readw(ss, sp));
			return 0;
		}
		if (offs == 0x254e) {
			D1_LOG(
			"memcpy(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readw(ss, sp+8));
			return 0;
		}
		if (offs == 0x2596) {
			D1_LOG(
			"memset(__dest=0x%x:0x%x, __c=0x%x, __n=0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+4), real_readw(ss, sp+6));
			return 0;
		}
		if (offs == 0x2607) {
			D1_LOG(
			"memmove(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readw(ss, sp+8));
			return 0;
		}
		if (offs == 0x2655) {
			D1_LOG("open()\n");
			return 0;
		}
		if (offs == 0x2dd5) {
			D1_LOG(
			"strcpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4));
			return 0;
		}
		if (offs == 0x2e1d) {
			D1_LOG(
			"strncmp(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readw(ss, sp+8));
			return 0;
		}
		if (offs == 0x2e55) {
			D1_LOG(
			"strncpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(ss, sp+2), real_readw(ss, sp),
				real_readw(ss, sp+6), real_readw(ss, sp+4),
				real_readw(ss, sp+8));
			return 0;
		}
	}
	return 0;

}
