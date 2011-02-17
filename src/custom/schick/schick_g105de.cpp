#include <stdlib.h>

#include "regs.h"

#include "schick.h"

#include "rewrite_g105de/g105de_seg002.h"
#include "rewrite_g105de/g105de_seg003.h"

#include "rewrite_m302de/seg008.h"
#include "rewrite_m302de/seg009.h"

static int seg003(unsigned short offs) {

	switch (offs) {
	case 0xb: {
		Bit16u lo = CPU_Pop16();
		Bit16u hi = CPU_Pop16();
		CPU_Push16(hi);
		CPU_Push16(lo);

		reg_ax = random_interval_gen(lo, hi);
		D1_LOG("random_interval(%d, %d); = %d\n", lo, hi, reg_ax);

		return 1;
	}
	case 0x29: {
		Bit16u val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = random_gen(val);
		D1_LOG("random_gen(%d); = %d\n", val, reg_ax);

		return 1;
	}
	case 0x7d: {
		Bit16u val = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(val);

		reg_ax = is_in_word_array(val, MemBase + Real2Phys(p));
		D1_LOG("is_in_word_array(%x, %x); = %d\n", val, p, reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg004(unsigned short offs) {

	switch (offs) {
	case 0xc: {
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		unsigned int len = CPU_Pop32();
		CPU_Push32(len);
		CPU_Push32(src);
		CPU_Push32(dst);

		D1_LOG("decomp_pp20(dst=0x%04x:0x%04x, src=0x%04x:0x%04x, %u)\n",
			RealSeg(dst), RealOff(dst),
			RealSeg(src), RealOff(src), len);

		decomp_pp20(MemBase + Real2Phys(src),
			MemBase + Real2Phys(dst),
			MemBase + Real2Phys(src) + 8, len);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

/* Rasterlib */
static int seg005(unsigned short offs) {

	switch (offs) {
	case 0x34: {
		RealPt addr = CPU_Pop32();
		CPU_Push32(addr);

		D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
			RealSeg(addr), RealOff(addr));

		return 0;
        }
	case 0x100: {
		RealPt ptr = CPU_Pop32();
		unsigned short first_color = CPU_Pop16();
		unsigned short colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(first_color);
		CPU_Push32(ptr);

		unsigned short i;

		D1_GFX("set_palette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), first_color, colors);

		set_palette(MemBase + Real2Phys(ptr), first_color, colors);

		return 1;
	}
	case 0x127: {
		unsigned short ptr = CPU_Pop16();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push16(ptr);

		draw_h_line(PhysMake(0xa000, ptr), cnt, color);

			D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
				ptr % 320, ptr / 320, cnt, color);
		return 1;
        }
	case 0x184: {
		unsigned short ptr = CPU_Pop16();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		unsigned short space = CPU_Pop16();
		CPU_Push16(space);
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push16(ptr);

		draw_h_spaced_dots(PhysMake(0xa000, ptr), cnt, color, space);

		D1_GFX("HSpacedDots(X=%03d,Y=%03u,%03u,0x%02x,%u);\n",
			ptr % 320, ptr / 320, cnt, color, space);

		return 1;
	}
	case 0x386: {
		unsigned short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = swap_u16(val);
		D1_GFX("swap_u16(val=0x%04x); = 0x%04x\n", val, reg_ax);

		return 1;
	}

	}

	if (offs == 0x1f3)
	{
		D1_GFX("DrawPic(Dest=0x%x:0x%x, X=%d, Y=%d, ..., Breite=%d, Höhe=%d, Src=0x%x:0x%x, Mode=%d);\n",
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
			real_readw(SegValue(ss), reg_sp+4), real_readw(SegValue(ss), reg_sp+6),
			real_readw(SegValue(ss), reg_sp+20), real_readw(SegValue(ss), reg_sp+22),
			real_readw(SegValue(ss), reg_sp+26), real_readw(SegValue(ss), reg_sp+24),
			real_readw(SegValue(ss), reg_sp+28));
		return 0;
	}
	if (offs == 0x2e3) {
		unsigned short off=real_readw(SegValue(ss), reg_sp+2);
		unsigned short seg=real_readw(SegValue(ss), reg_sp);

		D1_GFX("FillRect(segment=0x%x, offset=0x%x, color=0x%x, width=%d, height=%d);",
			seg, off, real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+8));

		if (seg == 0xa000)
			D1_GFX("\t/*X=%d Y=%d*/\n",
				off%320, off/320);
		else
			D1_GFX("\n");

		return 0;
	}

	if (offs == 0x39f) {
		D1_GFX("_39F(src=0x%x:0x%x, dst=0x%x:0x%x, a=%d, b=%d, c=%d);\n",
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
			real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+8), real_readw(SegValue(ss), reg_sp+10),
			real_readw(SegValue(ss), reg_sp+12));
		return 0;
	}
	D1_GFX("Rasterlib:0x%x\n", offs);
	return 0;
}

// Hooks for tracing far calls for GEN.EXE(de/V1.05)
int schick_farcall_gen105(unsigned segm, unsigned offs)
{
	/* seg002 main */
	if (segm == 0x3c6) {
		if (offs == 0x1dbe) {
			exit_video();
			return 1;
		}
	}
	/* seg003 random */
	if (segm == 0xb2d)
		return seg003(offs);
	/* _decomp() */
	if (segm == 0xb39)
		return seg004(offs);
	if (segm == 0xb6b)
		return seg005(offs);


	if (segm == 0x0) {
		if (offs == 0x0438) {
			D1_TRAC("_dos_getvect(intnr=0x%x)\n", real_readw(SegValue(ss),reg_sp));
			return 0;
		}
		if (offs == 0x0447) {
			D1_TRAC("_dos_setvect(intnr=0x%x, *isr=0x%x:0x%x)\n",
				real_readw(SegValue(ss), reg_sp), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+2));
			return 0;
		}
		if (offs == 0x07c5) {
			D1_LOG(
			"__read(Handle=0x%x, Buffer=0x%x:0x%x, Length=%d)\n",
			real_readw(SegValue(ss), reg_sp), real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp+6));
			return 0;
		}
		if (offs == 0x20bc) {
			D1_LOG("close(Handle=0x%x)\n", real_readw(SegValue(ss), reg_sp));
			return 0;
		}
		if (offs == 0x254e) {
			D1_LOG(
			"memcpy(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2596) {
			D1_LOG(
			"memset(__dest=0x%x:0x%x, __c=0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+4), real_readw(SegValue(ss), reg_sp+6));
			return 0;
		}
		if (offs == 0x2607) {
			D1_LOG(
			"memmove(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2655) {
			D1_LOG("open()\n");
			return 0;
		}
		if (offs == 0x2dd5) {
			D1_LOG(
			"strcpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4));
			return 0;
		}
		if (offs == 0x2e1d) {
			D1_LOG(
			"strncmp(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2e55) {
			D1_LOG(
			"strncpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
	}
	return 0;
}

int schick_nearcall_gen105(unsigned offs) {

	unsigned short segm = SegValue(cs)-relocation;

	switch (segm) {
		/* C-Lib */
		case  0x000: {
			return 0;
		}
		/* CD */
		case  0x364: {
			return 0;
		}
		/* Main */
		case  0x3c6: {
			switch (offs) {
				case 0x1bb2: {
					CPU_Pop16();
					RealPt name = CPU_Pop32();
					RealPt table = CPU_Pop32();
					CPU_Push32(table);
					CPU_Push32(name);

					Bit32s retval;

					retval = get_archive_offset(MemBase + Real2Phys(name), MemBase + Real2Phys(table));

					D1_LOG("get_archive_offset(%s, %s); = %d\n",
						getString(name),
						getString(table), retval);

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;

					return 1;
				}
				case 0x1c6f: {
					CPU_Pop16();

					Bit32s retval;
					retval = get_filelength();

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;

					D1_LOG("get_filelength() = %d\n",
						retval);
					return 1;
				}
				case 0x1c7b: {
					CPU_Pop16();

					reg_ax = ret_zero1();
					D1_LOG("ret_zero1();\n");

					return 1;
				}
				case 0x1d8f: {
					CPU_Pop16();

					D1_LOG("init_video();\n");
					init_video();

					return 1;
				}
				case 0x1dbe: {
					CPU_Pop16();

					D1_INFO("exit_video();\n");
					exit_video();

					return 1;
				}
				case 0x1e86: {
					CPU_Pop16();
					Bit16u v1 = CPU_Pop16();
					Bit16u v2 = CPU_Pop16();
					Bit16u v3 = CPU_Pop16();
					Bit16u v4 = CPU_Pop16();
					CPU_Push16(v4);
					CPU_Push16(v3);
					CPU_Push16(v2);
					CPU_Push16(v1);

					draw_v_line(v1, v2, v3, v4);

					D1_LOG("draw_v_line(%d,%d,%d,%x);\n",
						v1, v2, v3, v4);

					return 1;
				}
				case 0x2346: {
					CPU_Pop16();
					Bit16u c = CPU_Pop16();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					CPU_Push16(y);
					CPU_Push16(x);
					CPU_Push16(c);

					reg_ax = print_chr(c, x, y);

					D1_LOG("print_chr(%c,%d,%d); = %d\n",
						c, x, y, reg_ax);

					return 1;
				}
				case 0x2375: {
					CPU_Pop16();
					Bit16u c = CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);
					CPU_Push16(c);

					reg_ax = get_chr_info(c,
						MemBase + Real2Phys(ptr));

					D1_LOG("get_chr_info(%c,%x); = %d\n",
						c & 0xff, ptr, reg_ax);

					return 1;
				}
				case 0x23d7: {
					CPU_Pop16();
					Bit16u v1 = CPU_Pop16();
					Bit16u v2 = CPU_Pop16();
					Bit16u v3 = CPU_Pop16();
					Bit16u v4 = CPU_Pop16();
					CPU_Push16(v4);
					CPU_Push16(v3);
					CPU_Push16(v2);
					CPU_Push16(v1);

					D1_LOG("call_them_all(%d,%d,%d,%d);\n",
						v1, v2, v3, v4);
					call_them_all(v1, v2, v3, v4);

					return 1;
				}
				case 0x2523: {
					CPU_Pop16();
					Bit16u v1 = CPU_Pop16();
					Bit16u v2 = CPU_Pop16();
					CPU_Push16(v2);
					CPU_Push16(v1);

					set_vals(v1, v2);
					D1_LOG("set_vals(%d, %d);\n", v1, v2);

					return 1;
				}
				case 0x2534: {
					CPU_Pop16();
					RealPt p1 = CPU_Pop32();
					RealPt p2 = CPU_Pop32();
					CPU_Push32(p2);
					CPU_Push32(p1);

					get_vals(MemBase + Real2Phys(p1),
						MemBase + Real2Phys(p2));
					D1_LOG("get_vals(%x, %x);\n", p1, p2);

					return 1;
				}
				case 0x256c: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					reg_ax = get_str_width((char*)getString(ptr));

					D1_LOG("get_str_width(%s); = %d\n",
						(char*)getString(ptr), reg_ax);

					return 1;
				}
				case 0x259c: {
					CPU_Pop16();
					RealPt str = CPU_Pop32();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					CPU_Push16(y);
					CPU_Push16(x);
					CPU_Push32(str);

					reg_ax = get_line_start_c((char*)getString(str), x, y);

					D1_LOG("get_line_start_c(%s,%d,%d); = %d\n",
						getString(str), x, y, reg_ax);

					return 1;
				}
				case 0x38ae: {
					CPU_Pop16();

					D1_LOG("clear_hero();\n");
					clear_hero();

					return 1;
				}
				case 0x75c1: {
					CPU_Pop16();

					D1_LOG("init_colors();\n");
					init_colors();

					return 1;
				}
				case 0x7635: {
					CPU_Pop16();

					D1_LOG("init_stuff();\n");
					init_stuff();

					return 1;
				}
				default:
					return 0;
			}
		}
		default:
			return 0;
	}
}
