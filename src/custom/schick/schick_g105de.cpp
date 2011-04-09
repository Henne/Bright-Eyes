#include <stdlib.h>
#include <string.h>

#include "regs.h"

#include "schick.h"

#include "rewrite_g105de/g105de_seg000.h"
#include "rewrite_g105de/g105de_seg001.h"
#include "rewrite_g105de/g105de_seg002.h"
#include "rewrite_g105de/g105de_seg003.h"
#include "rewrite_g105de/g105de_seg005.h"

#include "rewrite_m302de/seg000.h"
#include "rewrite_m302de/seg008.h"
#include "rewrite_m302de/seg009.h"

static int seg001(unsigned short offs) {
	switch (offs) {
		case 0x0300: {
			Bit16u cmd = CPU_Pop16();
			CPU_Push16(cmd);
			reg_ax = G105de::CD_bioskey(cmd);
			return 1;
		}
		case 0x033b: {
			G105de::seg001_033b();
			return 1;
		}
	default:
		return 0;
	}
}

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
	case 0x1f3: {
		RealPt dst = CPU_Pop32();
		Bit16u x = CPU_Pop16();
		Bit16u y = CPU_Pop16();
		Bit32u dummy1 = CPU_Pop32();
		Bit16u width = CPU_Pop16();
		Bit16u height = CPU_Pop16();
		Bit32u dummy2 = CPU_Pop32();
		Bit16u v1 = CPU_Pop16();
		Bit16u v2 = CPU_Pop16();
		RealPt src = CPU_Pop32();
		Bit16u mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push32(src);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(dummy2);
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push32(dummy1);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(dst);

		D1_GFX("DrawPic(dst=0x%x, x=%d, y=%d, ..., w=%d, h=%d, src=0x%x, mode=%d);\n",
			dst, x, y, width, height, src, mode);
		return 0;
	}
	case 0x39f: {
		RealPt src = CPU_Pop32();
		RealPt dst = CPU_Pop32();
		Bit16u width = CPU_Pop16();
		Bit16u height = CPU_Pop16();
		Bit16u mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push32(dst);
		CPU_Push32(src);

		copy_to_screen(Real2Phys(src), Real2Phys(dst), width,
			height, mode);

		D1_LOG("copy_to_screen(src=0x%x, dst=0x%x, w=%d, h=%d, m=%d);\n",
			src, dst, width, height, mode);

		return 1;
	}
	default:
		D1_ERR("Rasterlib:0x%x\n", offs);
		exit(0);
	}
}

// Hooks for tracing far calls for GEN.EXE(de/V1.05)
int schick_farcall_gen105(unsigned segm, unsigned offs)
{
	/* seg001 CD */
	if (segm == 0x364)
		return seg001(offs);

	/* seg002 main */
	if (segm == 0x3c6) {
		if (offs == 0x1dbe) {
			exit_video();
			return 1;
		} else
		if (offs == 0x1fe0) {
			RealPt ptr = CPU_Pop32();
			Bit16u x1 = CPU_Pop16();
			Bit16u y1 = CPU_Pop16();
			Bit16u x2 = CPU_Pop16();
			Bit16u y2 = CPU_Pop16();
			Bit16u color = CPU_Pop16();
			CPU_Push16(color);
			CPU_Push16(y2);
			CPU_Push16(x2);
			CPU_Push16(y1);
			CPU_Push16(x1);
			CPU_Push32(ptr);

			call_fill_rect_gen(Real2Phys(ptr),
				x1, y1, x2, y2, color);

			D1_LOG("call_fill_rect_gen(%x,%d,%d,%d,%d,%x);\n",
				ptr, x1, y1, x2, y2, color);

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
		if (offs == 0x0f43) {
			Bit16u v = CPU_Pop16();
			CPU_Push16(v);

			reg_ax = G105de::bioskey(v);

			D1_LOG("bioskey(%d); = %x\n", v, reg_ax);

			return 1;
		}
		if (offs == 0x20bc) {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = G105de::bc_close(handle);
			D1_LOG("close(Handle=0x%x) = 0x%x\n", handle, reg_ax);

			return 1;
		}
		if (offs == 0x20e4) {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = G105de::bc__close(handle);
			D1_LOG("_close(Handle=0x%x) = 0x%x\n", handle, reg_ax);

			return 1;
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
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("strcpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x %s)\n",
				RealSeg(s1), RealOff(s1),
				RealSeg(s2), RealOff(s2), getString(s2));
			strcpy((char*)MemBase + Real2Phys(s1),
				(char*)MemBase + Real2Phys(s2));

			return 1;
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
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("bc_strncpy(s1=0x%x, \"%s\",  maxlen=%d)\n",
				s1, getString(s2), n);

			strncpy((char*)MemBase + Real2Phys(s1),
				(char*)MemBase + Real2Phys(s2), n);

			return 1;
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
			switch(offs) {
				case 0x0038: {
					CPU_Pop16();
					reg_ax = G105de::CD_set_drive_nr();
					return 1;
				}
				case 0x005d: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);
					G105de::CD_driver_request(ptr);
					return 1;
				}
				case 0x00ae: {
					unsigned int retval;
					CPU_Pop16();
					retval = G105de::CD_get_tod();

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;
					return 1;
				}
				case 0x00bb: {
					CPU_Pop16();
					Bit16u track = CPU_Pop16();
					CPU_Push16(track);
					G105de::seg001_00bb(track);
					return 1;
				}
				case 0x0312b: {
					CPU_Pop16();
					G105de::seg001_0312();
					return 1;
				}
				case 0x033b: {
					CPU_Pop16();
					G105de::seg001_033b();
					return 1;
				}
				default:
					return 0;
			}
		}
		/* Main */
		case  0x3c6: {
			switch (offs) {
				case 0x09d1: {
					CPU_Pop16();

					draw_mouse_ptr_wrapper();
					D1_LOG("draw_mouse_ptr_wrapper();\n");

					return 1;
				}
				case 0x09db: {
					CPU_Pop16();

					D1_LOG("call_mouse();\n");
					call_mouse();

					return 1;
				}
				case 0x09e5: {
					CPU_Pop16();

					draw_mouse_ptr();
					D1_LOG("draw_mouse_ptr();\n");

					return 1;
				}
				case 0x0a0d: {
					CPU_Pop16();

					D1_LOG("mouse();\n");
					mouse();

					return 1;
				}
				case 0x0cf2: {
					Bit16u pIP = CPU_Pop16();
					RealPt dst = CPU_Pop32();
					RealPt src = CPU_Pop32();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					Bit16u w = CPU_Pop16();
					Bit16u h = CPU_Pop16();
					Bit16u m = CPU_Pop16();
					CPU_Push16(m);
					CPU_Push16(h);
					CPU_Push16(w);
					CPU_Push16(y);
					CPU_Push16(x);
					CPU_Push32(src);
					CPU_Push32(dst);

					D1_LOG("decomp_rle(%x, %x,...);\n",
						dst, src);

					G105de::decomp_rle(MemBase + Real2Phys(dst),
						MemBase + Real2Phys(src),
						x, y, w, h, m);

					return 1;
				}


				case 0x0db1: {
					CPU_Pop16();

					D1_LOG("update_mouse_ptr();\n");
					update_mouse_ptr();

					return 1;
				}
				case 0x0e6a: {
					CPU_Pop16();

					D1_LOG("save_mouse_ptr();\n");
					save_mouse_ptr();

					return 1;
				}
				case 0x0c1c: {
					CPU_Pop16();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);
					CPU_Push16(y);
					CPU_Push16(x);

					reg_ax = G105de::get_mouse_action(x, y,
						MemBase + Real2Phys(ptr));

					D1_LOG("get_mouse_action(%d,%d,%x); = %d\n",
						x, y, ptr, reg_ax);

					return 1;
				}
				case 0x0fef: {
					CPU_Pop16();
					RealPt p1 = CPU_Pop32();
					RealPt p2 = CPU_Pop32();
					RealPt p3 = CPU_Pop32();
					CPU_Push32(p3);
					CPU_Push32(p2);
					CPU_Push32(p1);

					D1_LOG("split_textbuffer(0x%x,0x%x,%d);\n",
						p1, p2, p3);

					G105de::split_textbuffer(MemBase + Real2Phys(p1), p2, p3);

					return 1;
				}
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
				case 0x1c82: {
					CPU_Pop16();
					G105de::wait_for_keypress();
					D1_LOG("wait_for_keypress()\n");
					return 1;
				}
				case 0x1d14: {
					CPU_Pop16();
					Bit16u v1 = CPU_Pop16();
					Bit16u v2 = CPU_Pop16();
					CPU_Push16(v2);
					CPU_Push16(v1);

					Bit32u retval;
					retval = swap32(v1, v2);

					D1_INFO("swap32(%x, %x);\n", v1, v2);

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;

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

					D1_LOG("exit_video();\n");
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
				case 0x1fe0: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					Bit16u x1 = CPU_Pop16();
					Bit16u y1 = CPU_Pop16();
					Bit16u x2 = CPU_Pop16();
					Bit16u y2 = CPU_Pop16();
					Bit16u color = CPU_Pop16();
					CPU_Push16(color);
					CPU_Push16(y2);
					CPU_Push16(x2);
					CPU_Push16(y1);
					CPU_Push16(x1);
					CPU_Push32(ptr);

					call_fill_rect_gen(Real2Phys(ptr),
						x1, y1, x2, y2, color);

					D1_LOG("call_fill_rect_gen(%x,%d,%d,%d,%d,%x);\n",
						ptr, x1, y1, x2, y2, color);

					return 1;
				}
				case 0x20aa: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					reg_ax = str_splitter((char*)MemBase + Real2Phys(ptr));
					D1_LOG("str_splitter(%s); = %d\n",
						(char*)getString(ptr), reg_ax);

					return 1;
				}
				case 0x21ea: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					reg_ax = print_line((char*)MemBase + Real2Phys(ptr));
					D1_LOG("print_line(%s); = %d\n",
						(char*)getString(ptr), reg_ax);

					return 1;
				}
				case 0x2223: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					CPU_Push16(y);
					CPU_Push16(x);
					CPU_Push32(ptr);

					D1_LOG("print_str(%s)\n", getString(ptr));
					print_str((char*)MemBase + Real2Phys(ptr),
						x, y);

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
				case 0x25e9: {
					CPU_Pop16();
					RealPt str = CPU_Pop32();
					Bit16u x = CPU_Pop16();
					Bit16u y = CPU_Pop16();
					Bit16u len = CPU_Pop16();
					Bit16u zero = CPU_Pop16();
					CPU_Push16(zero);
					CPU_Push16(len);
					CPU_Push16(y);
					CPU_Push16(x);
					CPU_Push32(str);

					D1_LOG("enter_string(%x,%d,%d,%d,%d);\n",
						getString(str), x, y, len, zero);

					reg_ax = enter_string(
						(char*)MemBase + Real2Phys(str),
						x, y, len, zero);

					return 1;
				}
				case 0x2c0d: {
					Bit16u pIP = CPU_Pop16();
					Bit16u old_pos = CPU_Pop16();
					Bit16u new_pos = CPU_Pop16();
					Bit16u offset = CPU_Pop16();
					CPU_Push16(offset);
					CPU_Push16(new_pos);
					CPU_Push16(old_pos);

					D1_LOG("fill_radio_button(%x,%x,%x);\n",
						old_pos, new_pos, offset);

					G105de::fill_radio_button(old_pos,
						new_pos, offset);

					return 1;

				}
				case 0x3064: {
					CPU_Pop16();

					D1_LOG("enter_name();\n");
					enter_name();

					return 1;
				}
				case 0x3171: {
					CPU_Pop16();

					D1_LOG("change_sex();\n");
					G105de::change_sex();

					return 1;
				}
				case 0x3bdd: {
					CPU_Pop16();

					calc_at_pa();
					D1_LOG("calc_at_pa();\n");

					return 1;
				}
				case 0x38ae: {
					CPU_Pop16();

					D1_LOG("clear_hero();\n");
					clear_hero();

					return 1;
				}
				case 0x4291: {
					CPU_Pop16();
					Bit16u skill = CPU_Pop16();
					CPU_Push16(skill);

					D1_LOG("skill_inc_novice(%d);\n",
						skill);
					skill_inc_novice(skill);

					return 1;
				}
				case 0x4305: {
					CPU_Pop16();
					Bit16u spell = CPU_Pop16();
					CPU_Push16(spell);

					D1_LOG("spell_inc_novice(%d);\n",
						spell);
					spell_inc_novice(spell);

					return 1;
				}
				case 0x4b1b: {
					CPU_Pop16();

					D1_LOG("save_picbuf();\n");

					G105de::save_picbuf();

					return 1;
				}
				case 0x4cb4: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					D1_LOG("restore_picbuf(%x);\n", ptr);

					G105de::restore_picbuf(Real2Phys(ptr));

					return 1;
				}
				case 0x5abd: {
					CPU_Pop16();
					RealPt dst = CPU_Pop32();
					Bit32u money = CPU_Pop32();
					CPU_Push32(money);
					CPU_Push32(dst);

					D1_LOG("make_valuta_str(%d);\n", money);

					G105de::make_valuta_str(
						(char*)MemBase + Real2Phys(dst),						money);

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
