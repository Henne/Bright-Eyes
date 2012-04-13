#include <stdlib.h>
#include <string.h>

#include "regs.h"

#include "schick.h"

#include "rewrite_g105de/g105de_seg000.h"
#include "rewrite_g105de/g105de_seg001.h"
#include "rewrite_g105de/g105de_seg002.h"
#include "rewrite_g105de/g105de_seg003.h"
#include "rewrite_g105de/g105de_seg004.h"
#include "rewrite_g105de/g105de_seg005.h"
#include "rewrite_g105de/g105de_seg006.h"

using namespace G105de;

static int seg000(Bitu offs) {
	switch (offs) {
		case 0x3a4: {
			Bit16u ev = CPU_Pop16();
			CPU_Push16(ev);

			D1_INFO("Bye Bye! We're closing DOSBox\n");
			bc_exit(ev);
			return 1;
		}
		case 0x0438: {
			Bit16u intnr = CPU_Pop16();
			CPU_Push16(intnr);

			RealPt addr  = bc__dos_getvect(intnr);
			D1_LOG("_dos_getvect(intnr=0x%x) = %x\n", intnr, addr);

			reg_ax = RealOff(addr);
			reg_dx = RealSeg(addr);

			return 1;
		}
		case 0x0447: {
			Bit16u intnr = CPU_Pop16();
			RealPt addr = CPU_Pop32();
			CPU_Push32(addr);
			CPU_Push16(intnr);

			D1_LOG("_dos_setvect(intnr=0x%x, *isr=0x%x:0x%x)\n",
				intnr, RealSeg(addr), RealOff(addr));

			bc__dos_setvect(intnr, addr);

			return 1;
		}
		case 0x072d: {
			Bit16u handle = CPU_Pop16();
			Bit32s off = CPU_Pop32();
			Bit16u kind = CPU_Pop16();
			CPU_Push16(kind);
			CPU_Push32(off);
			CPU_Push16(handle);

			Bit32s retval;

			retval = bc_lseek(handle, off, kind);
			D1_LOG("lseek(Handle=0x%x, off=%d, kind=%d) = %d\n",
				handle, off, kind, retval);

			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;
		}
		case 0x07c5: {
			Bit16u handle = CPU_Pop16();
			RealPt buf = CPU_Pop32();
			Bit16u len = CPU_Pop16();
			CPU_Push16(len);
			CPU_Push32(buf);
			CPU_Push16(handle);

			reg_ax = bc__read(handle, MemBase + Real2Phys(buf), len);
			D1_LOG(
			"_read(Handle=0x%x, Buffer=0x%x:0x%x, Length=%d) = %d\n",
				handle, RealSeg(buf), RealOff(buf),
				len, reg_ax);

			if (reg_ax != len)
				D1_ERR("Error while reading\n");

			return 1;
		}
		case 0x0f43: {
			Bit16u v = CPU_Pop16();
			CPU_Push16(v);

			reg_ax = bioskey(v);

			D1_LOG("bioskey(%d); = %x\n", v, reg_ax);

			return 1;
		}
		case 0xf68: {
			bc_clrscr();
			D1_LOG("clrscr();\n");
			return 1;
		}
		case 0x1295: {
			RealPt p = CPU_Pop32();
			D1_LOG("bc_free(%x);\n", p);
			bc_free(p);
			CPU_Push32(p);
			return 1;
		}
		case 0x20bc: {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = bc_close(handle);
			D1_LOG("close(Handle=0x%x) = 0x%x\n", handle, reg_ax);

			return 1;
		}
		case 0x20e4: {
			Bit16u handle = CPU_Pop16();
			CPU_Push16(handle);

			reg_ax = bc__close(handle);
			D1_LOG("_close(Handle=0x%x) = 0x%x\n", handle, reg_ax);

			return 1;
		}
		case 0x212f: {
			RealPt pathP = CPU_Pop32();
			Bit16u attr = CPU_Pop16();
			CPU_Push16(attr);
			CPU_Push32(pathP);

			reg_ax = bc__create(MemBase + Real2Phys(pathP), attr);
			D1_LOG("bc__create(%s, %x) = %d\n",
				(char*)MemBase + Real2Phys(pathP),
				attr, reg_ax);

			return 1;
		}
		case 0x254e: {
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dst);

			D1_LOG(
			"memcpy(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				RealSeg(dst), RealOff(dst),
				RealSeg(src), RealOff(src), n);

			memcpy(MemBase + Real2Phys(dst), MemBase + Real2Phys(src), n);
			reg_ax = RealSeg(dst);
			reg_ax = RealOff(src);

			return 1;
		}
		case 0x2596: {
			RealPt dst = CPU_Pop32();
			Bit16u c = CPU_Pop16();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push16(c);
			CPU_Push32(dst);

			memset(MemBase + Real2Phys(dst), c, n);

			D1_LOG("memset(__dest=0x%x:0x%x, __c=0x%x, __n=0x%x)\n",
				RealSeg(dst), RealOff(dst), c, n);

			reg_ax = RealOff(dst);
			reg_dx = RealSeg(dst);

			return 1;
		}
		case 0x2607: {
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dst);

			D1_LOG(
			"memmove(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				RealSeg(dst), RealOff(dst),
				RealSeg(src), RealOff(src), n);

			memmove(MemBase + Real2Phys(dst),
				MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dst);
			reg_dx = RealSeg(dst);

			return 1;
		}
		case 0x2655: {
			RealPt pathP = CPU_Pop32();
			Bit16u oflag = CPU_Pop16();
			CPU_Push16(oflag);
			CPU_Push32(pathP);

			D1_LOG("bc_open(%s, %x)\n",
				(char*)MemBase + Real2Phys(pathP), oflag);

			return 0;
		}
		case 0x2dd5: {
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("strcpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x %s)\n",
				RealSeg(s1), RealOff(s1),
				RealSeg(s2), RealOff(s2), getString(s2));
			strcpy(getString(s1), getString(s2));

			return 1;
		}
		case 0x2e1d: {
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG(
			"strncmp(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				RealSeg(s1), RealOff(s1),
				RealSeg(s2), RealOff(s2), n);

			return 1;
		}
		case 0x2e55: {
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			Bit16u n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("bc_strncpy(s1=0x%x, \"%s\",  maxlen=%d)\n",
				s1, getString(s2), n);

			strncpy(getString(s1), getString(s2), n);

			return 1;
		}
		case 0x360e: {
			Bit16u handle = CPU_Pop16();
			RealPt buf = CPU_Pop32();
			Bit16u len = CPU_Pop16();
			CPU_Push16(len);
			CPU_Push32(buf);
			CPU_Push16(handle);

			reg_ax = bc_write(handle, MemBase + Real2Phys(buf), len);
			D1_LOG(
			"bc_write(Handle=0x%x, Buffer=0x%x:0x%x, Length=%d) = %d\n",
				handle, RealSeg(buf), RealOff(buf),
				len, reg_ax);

			if (reg_ax != len)
				D1_ERR("Error while writing\n");

			return 1;
		}
		default:
			return 0;
	}
}

static int seg001(Bitu offs) {
	switch (offs) {
		case 0x0300: {
			Bit16u cmd = CPU_Pop16();
			CPU_Push16(cmd);
			reg_ax = CD_bioskey(cmd);
			return 1;
		}
		case 0x033b: {
			seg001_033b();
			return 1;
		}
		case 0x0465: {
			D1_LOG("seg001_0465()\n");
			seg001_0465();
			return 1;
		}
		case 0x0600: {
			D1_LOG("seg001_0600();\n");
			reg_ax = seg001_0600();
			return 1;
		}
	default:
		return 0;
	}
}

static int seg002(Bitu offs)
{
	switch (offs) {
		case 0x1dbe: {
			exit_video();
			return 1;
		}
		case 0x1fe0: {
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
		case 0x730b: {
			D1_LOG("restore_mouse_isr();\n");
			restore_mouse_isr();

			return 1;
		}
		case 0x7322: {
			Bit16u argc = CPU_Pop16();
			RealPt p = CPU_Pop32();
			Bit8u *b_argv = Real2Host(p);
			char *argv[4];
			Bit16u i;

			for (i = 0; i < argc; i++)
				argv[i] = getString(host_readd(b_argv + i * 4));

			D1_LOG("main_gen(%d, %p);\n", argc, (void*)argv);
			reg_ax = main_gen(argc, argv);

			CPU_Push32(p);
			CPU_Push16(argc);
			return 1;
		}
		default:
			return 0;
	}
}

static int seg003(Bitu offs) {

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
		D1_ERR("Uncatched call to Segment %s:0x%04lx\n",
			__func__, offs);
		exit(1);
	}
}

static int seg004(Bitu offs) {

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
		D1_ERR("Uncatched call to Segment %s:0x%04lx\n",
			__func__, offs);
		exit(1);
	}
}

/* Rasterlib */
static int seg005(Bitu offs) {

	switch (offs) {
	case 0x34: {
		RealPt addr = CPU_Pop32();
		D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
			RealSeg(addr), RealOff(addr));
		save_display_stat(addr);
		CPU_Push32(addr);
		return 1;
        }
	case 0x100: {
		RealPt ptr = CPU_Pop32();
		unsigned short first_color = CPU_Pop16();
		unsigned short colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(first_color);
		CPU_Push32(ptr);

		D1_GFX("set_palette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), first_color, colors);

		set_palette(MemBase + Real2Phys(ptr),
			(unsigned char)first_color, colors);

		return 1;
	}
	case 0x127: {
		unsigned short ptr = CPU_Pop16();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push16(ptr);

		draw_h_line(PhysMake(0xa000, ptr), cnt, (unsigned char)color);

			D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
				ptr % 320, ptr / 320, cnt, color);
		return 1;
        }
	case 0x1f3: {
		RealPt dst = CPU_Pop32();
		Bit16u x = CPU_Pop16();
		Bit16u y = CPU_Pop16();
		Bit16u d1 = CPU_Pop16();
		Bit16u d2 = CPU_Pop16();
		Bit16u width = CPU_Pop16();
		Bit16u height = CPU_Pop16();
		Bit16u d3 = CPU_Pop32();
		Bit16u d4 = CPU_Pop16();
		Bit16u v1 = CPU_Pop16();
		Bit16u v2 = CPU_Pop16();
		RealPt src = CPU_Pop32();
		Bit16u mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push32(src);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push16(d4);
		CPU_Push16(d3);
		CPU_Push16(height);
		CPU_Push16(width);
		CPU_Push16(d2);
		CPU_Push16(d1);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(dst);

		D1_GFX("DrawPic(dst=0x%x, x=%d, y=%d, ..., v1=%d, v2=%d, w=%d, h=%d, src=0x%x, mode=%d);\n",
			dst, x, y, width, height, v1, v2, src, mode);

		draw_pic(Real2Phys(dst), x, y, d1, d2, width, height,
				d3, d4, v1, v2, Real2Phys(src), mode);
		return 1;
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
	case 0x445: {
		return 0;
	}
	default:
		D1_ERR("Rasterlib:0x%lx\n", offs);
		exit(0);
	}
}

static int seg006(Bitu offs) {

	switch (offs) {
		case 0xa24: {
			return 0;
		}
		case 0xadc: {
			return 0;
		}
		case 0xb02: {
			return 0;
		}
		case 0xb08: {
			return 0;
		}
		case 0xc47: {
			return 0;
		}
		case 0xc4d: {
			Bit16u driver = CPU_Pop16();
			RealPt form = CPU_Pop32();
			Bit16u sequence = CPU_Pop16();
			RealPt state = CPU_Pop32();
			RealPt controller = CPU_Pop32();

			D1_LOG("AIL_register_sequence()\n");
			reg_ax = AIL_register_sequence(driver, form,
					sequence, state, controller);
			CPU_Push32(controller);
			CPU_Push32(state);
			CPU_Push16(sequence);
			CPU_Push32(form);
			CPU_Push16(driver);
			return 1;
		}
		case 0xc59: {
			return 0;
		}
		case 0xc5f: {
			return 0;
		}
		case 0xc65: {
			Bit16u driver = CPU_Pop16();
			Bit16u sequence = CPU_Pop16();
			D1_LOG("AIL_timbre_request()\n");
			reg_ax = AIL_timbre_request(driver, sequence);

			CPU_Push16(sequence);
			CPU_Push16(driver);
			return 1;
		}
		case 0xc6b: {
			Bit16u driver = CPU_Pop16();
			Bit16u bank = CPU_Pop16();
			Bit16u patch = CPU_Pop16();
			RealPt src = CPU_Pop32();
			D1_LOG("AIL_install_timbre()\n");
			AIL_install_timbre(driver, bank, patch, src);
			CPU_Push32(src);
			CPU_Push16(patch);
			CPU_Push16(bank);
			CPU_Push16(driver);

			return 1;
		}
	}
	return 0;
}

// Hooks for tracing far calls for GEN.EXE(de/V1.05)
int schick_farcall_gen105(unsigned segm, unsigned offs)
{
	switch (segm) {
		/* seg000 Borland C-Lib */
		case 0x0:	return seg000(offs);
		/* seg001 CD */
		case 0x364:	return seg001(offs);
		/* seg002 main */
		case 0x3c6:	return seg002(offs);
		/* seg003 random */
		case 0xb2d:	return seg003(offs);
		/* seg004 _decomp() */
		case 0xb39:	return seg004(offs);
		/* seg005 Rasterlib */
		case 0xb6b:	return seg005(offs);
		/* seg006 AIL */
		case 0xbb2:	return seg006(offs);
		default:
				return 0;
	}
}

int schick_nearcall_gen105(unsigned offs) {

	unsigned short segm = SegValue(cs)-reloc_gen;

	switch (segm) {
		/* C-Lib */
		case  0x000: {
			switch (offs) {
				case 0x2572: {
				CPU_Pop16();
				RealPt dst = CPU_Pop32();
				Bit16u n = CPU_Pop16();
				Bit16u c = CPU_Pop16();
				CPU_Push16(c);
				CPU_Push16(n);
				CPU_Push32(dst);

				memset(MemBase + Real2Phys(dst), (char)c, n);

				D1_LOG("setmem(__dest=0x%x:0x%x, __n=0x%x, __c=0x%x)\n",
					RealSeg(dst), RealOff(dst), n, c);


				return 1;
			}
			default:
				return 0;
			}
		}
		/* CD */
		case  0x364: {
			switch(offs) {
				case 0x0038: {
					CPU_Pop16();
					reg_ax = CD_set_drive_nr();
					return 1;
				}
				case 0x005d: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);
					CD_driver_request(ptr);
					return 1;
				}
				case 0x00ae: {
					unsigned int retval;
					CPU_Pop16();
					retval = CD_get_tod();

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;
					return 1;
				}
				case 0x00bb: {
					CPU_Pop16();
					Bit16u track = CPU_Pop16();
					CPU_Push16(track);
					seg001_00bb(track);
					return 1;
				}
				case 0x0312b: {
					CPU_Pop16();
					seg001_0312();
					return 1;
				}
				case 0x033b: {
					CPU_Pop16();
					seg001_033b();
					return 1;
				}
				case 0x03a8: {
					CPU_Pop16();
					D1_LOG("0x3a8()\n");
					seg001_03a8();
					return 1;
				}
				case 0x0487: {
					/* Skip CD-Check for Jowood version */
					CPU_Pop16();
					reg_ax = 1;
					return 1;
				}
				default:
					return 0;
			}
		}
		/* Main */
		case  0x3c6: {
			switch (offs) {
				case 0x000b: {
					CPU_Pop16();
					Bit16u index = CPU_Pop16();
					CPU_Push16(index);

					D1_LOG("start_music(%d);\n", index);
					start_music(index);

					return 1;
				}
				case 0x0034: {
					CPU_Pop16();
					D1_LOG("read_soundcfg();\n");
					read_soundcfg();
					return 1;
				}
				case 0x0083: {
					CPU_Pop16();
					Bit32u size = CPU_Pop32();
					CPU_Push32(size);
					D1_LOG("init_music(%d);\n", size);
					init_music(size);
					return 1;
				}
				case 0x00ac: {
					CPU_Pop16();
					D1_LOG("stop_music();\n");
					stop_music();
					return 1;
				}
				case 0x389: {
					CPU_Pop16();
					Bit16u index = CPU_Pop16();
					CPU_Push16(index);

					D1_LOG("call_load_file(%d);\n", index);
					reg_ax = load_file(index);

					return 1;
				}
				case 0x397: {
					CPU_Pop16();
					Bit16u index = CPU_Pop16();
					CPU_Push16(index);

					D1_LOG("load_file(%d);\n", index);
					reg_ax = load_file(index);

					return 1;
				}
				case 0x0565: {
					CPU_Pop16();
					D1_LOG("restart_midi();\n");
					restart_midi();
					return 1;
				}
				case 0x05a1: {
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

					do_mouse_action(MemBase + Real2Phys(p1),
						MemBase + Real2Phys(p2),
						MemBase + Real2Phys(p3),
						MemBase + Real2Phys(p4),
						MemBase + Real2Phys(p5));

					D1_LOG("mouse_action();\n");

					return 1;
				}
				case 0x078c: {
					CPU_Pop16();
					mouse_enable();
					D1_LOG("mouse_enable();\n");
					return 1;
				}
				case 0x0829: {
					CPU_Pop16();

					mouse_disable();
					D1_LOG("mouse_disable();\n");

					return 1;
				}
				case 0x0874: {
					CPU_Pop16();
					Bit16u val = CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);
					CPU_Push16(val);

					mouse_do_enable(val, ptr);
					D1_LOG("mouse_do_enable();\n");

					return 1;
				}
				case 0x08d4: {
					CPU_Pop16();

					mouse_do_disable();
					D1_LOG("mouse_do_disable();\n");

					return 1;
				}
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
				case 0x0a8c: {
					CPU_Pop16();

					D1_LOG("mouse_compare();\n");
					mouse_compare();

					return 1;
				}
				case 0x0ae4: {
					CPU_Pop16();

					D1_LOG("handle_input();\n");
					handle_input();

					return 1;
				}
				case 0x0cf2: {
					CPU_Pop16();
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

					decomp_rle(MemBase + Real2Phys(dst),
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

					reg_ax = get_mouse_action(x, y,
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

					split_textbuffer(MemBase + Real2Phys(p1), p2, p3);

					return 1;
				}
				case 0x0f7c: {
					CPU_Pop16();

					D1_LOG("load_font_and_text();\n");
					load_font_and_text();

					return 1;
				}
				case 0x1059: {
					CPU_Pop16();
					Bit16u val = CPU_Pop16();
					CPU_Push16(val);

					D1_LOG("load_page(%d);\n", val);
					load_page(val);

					return 1;
				}
				case 0x11e5: {
					CPU_Pop16();
					Bit16u val = CPU_Pop16();
					CPU_Push16(val);

					D1_LOG("load_typus(%d);\n", val);
					load_typus(val);

					return 1;
				}
				case 0x12fa: {
					CPU_Pop16();
					D1_LOG("save_chr();\n");
					save_chr();
					return 1;
				}
				case 0x14f6: {
					CPU_Pop16();

					D1_LOG("read_common_files();\n");
					read_common_files();

					return 1;
				}
				case 0x15df: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					D1_LOG("process_nvf(%x);\n", ptr);
					process_nvf((struct nvf_desc*)MemBase + Real2Phys(ptr));

					return 1;
				}
				case 0x1bb2: {
					CPU_Pop16();
					RealPt name = CPU_Pop32();
					RealPt table = CPU_Pop32();
					CPU_Push32(table);
					CPU_Push32(name);

					Bit32s retval;

					retval = get_archive_offset(getString(name), MemBase + Real2Phys(table));

					D1_LOG("get_archive_offset(%s, %s); = %d\n",
						getString(name),
						getString(table), retval);

					reg_ax = retval & 0xffff;
					reg_dx = (retval>>16) & 0xffff;

					return 1;
				}
				case 0x1c34: {
					CPU_Pop16();
					Bit16u handle = CPU_Pop16();
					Bit32s buf = CPU_Pop32();
					Bit16u len = CPU_Pop16();
					CPU_Push16(len);
					CPU_Push32(buf);
					CPU_Push16(handle);


					reg_ax = read_datfile(handle,
						MemBase + Real2Phys(buf), len);

					D1_LOG("read_datfile(Handle=%x, off=%x, len=%d)\n",
						handle, buf, len);


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
					wait_for_keypress();
					D1_LOG("wait_for_keypress()\n");
					return 1;
				}
				case 0x1c9d: {
					CPU_Pop16();
					RealPt msg = CPU_Pop32();
					CPU_Push32(msg);

					D1_LOG("error_msg(%s);\n", MemBase + Real2Phys(msg));
					error_msg(MemBase + Real2Phys(msg));

					return 1;
				}
				case 0x1cc8: {
					CPU_Pop16();
					Bit16u val = CPU_Pop16();
					CPU_Push16(val);

					D1_LOG("vsync_or_key(%d);\n", val);
					vsync_or_key(val);

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
				case 0x1ecc: {
					CPU_Pop16();
					Bit16u val = CPU_Pop16();
					CPU_Push16(val);

					D1_LOG("0x1ecc(%d);\n", val);
					do_draw_pic(val);

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
						getString(ptr), reg_ax);

					return 1;
				}
				case 0x21ea: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					reg_ax = print_line((char*)MemBase + Real2Phys(ptr));
					D1_LOG("print_line(%s); = %d\n",
						getString(ptr), reg_ax);

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

					reg_ax = print_chr((char)c, x, y);

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

					reg_ax = get_chr_info((char)c,
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
				case 0x2024: {
					CPU_Pop16();

					D1_LOG("WAIT_FOR_VSYNC();\n");
					wait_for_vsync();

					return 1;
				}
				case 0x2523: {
					CPU_Pop16();
					Bit16u v1 = CPU_Pop16();
					Bit16u v2 = CPU_Pop16();
					CPU_Push16(v2);
					CPU_Push16(v1);

					set_textcolor(v1, v2);
					D1_LOG("set_textcolor(%d, %d);\n", v1, v2);

					return 1;
				}
				case 0x2534: {
					CPU_Pop16();
					RealPt p1 = CPU_Pop32();
					RealPt p2 = CPU_Pop32();
					CPU_Push32(p2);
					CPU_Push32(p1);

					get_textcolor(Real2Host(p1),
						Real2Host(p2));
					D1_LOG("get_textcolor(%x, %x);\n", p1, p2);

					return 1;
				}
				case 0x256c: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					reg_ax = get_str_width(getString(ptr));

					D1_LOG("get_str_width(%s); = %d\n",
						getString(ptr), reg_ax);

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

					reg_ax = get_line_start_c(getString(str), x, y);

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
				case 0x288a: {
					CPU_Pop16();
					Bit16u line = CPU_Pop16();
					Bit16u type = CPU_Pop16();
					CPU_Push16(type);
					CPU_Push16(line);

					D1_LOG("draw_popup_line(%d, %d);\n",
						line, type);
					draw_popup_line(line, type);

					return 1;
				}
				case 0x2993: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					Bit16u c = CPU_Pop16();
					CPU_Push16(c);
					CPU_Push32(ptr);


					reg_ax = infobox((char*)MemBase + Real2Phys(ptr), c);
					D1_LOG("infobox(%s,%x); = %d\n",
						MemBase + Real2Phys(ptr), c , reg_ax);

					return 1;
				}
				case 0x2bcb: {
					CPU_Pop16();
					RealPt msg = CPU_Pop32();
					CPU_Push32(msg);

					reg_ax = gui_bool(MemBase + Real2Phys(msg));
					D1_LOG("gui_bool(%p) = %d\n",
						MemBase + Real2Phys(msg),
						reg_ax);

					return 1;
				}
				case 0x2c0d: {
					CPU_Pop16();
					Bit16u old_pos = CPU_Pop16();
					Bit16u new_pos = CPU_Pop16();
					Bit16u offset = CPU_Pop16();
					CPU_Push16(offset);
					CPU_Push16(new_pos);
					CPU_Push16(old_pos);

					D1_LOG("fill_radio_button(%x,%x,%x);\n",
						old_pos, new_pos, offset);

					fill_radio_button(old_pos,
						new_pos, offset);

					return 1;

				}
				case 0x3064: {
					CPU_Pop16();

					D1_LOG("enter_name();\n");
					enter_name();

					return 1;
				}
				case 0x30df: {
					CPU_Pop16();
					change_head();
					D1_LOG("change_head()\n");
					return 1;
				}
				case 0x3171: {
					CPU_Pop16();

					D1_LOG("change_sex();\n");
					change_sex();

					return 1;
				}
				case 0x3217: {
					CPU_Pop16();
					D1_LOG("do_gen();\n");
					do_gen();
					return 1;
				}
				case 0x35d0: {
					CPU_Pop16();

					D1_LOG("refresh_screen();\n");
					refresh_screen();

					return 1;
				}
				case 0x3917: {
					CPU_Pop16();

					D1_LOG("new_values();\n");
					new_values();

					return 1;
				}
				case 0x3bdd: {
					CPU_Pop16();

					calc_at_pa();
					D1_LOG("calc_at_pa();\n");

					return 1;
				}
				case 0x3caf: {
					CPU_Pop16();

					D1_LOG("fill_values();\n");
					fill_values();

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
				case 0x435b: {
					CPU_Pop16();
					D1_LOG("select_typus();\n");
					select_typus();
					return 1;
				}
				case 0x4632: {
					CPU_Pop16();
					reg_ax = can_change_attribs();
					D1_LOG("can_change_attribs() = %d;\n",
						reg_ax);
					return 1;
				}
				case 0x4718: {
					CPU_Pop16();
					D1_LOG("change_attribs();\n");
					change_attribs();
					return 1;
				}
				case 0x4b1b: {
					CPU_Pop16();

					D1_LOG("save_picbuf();\n");

					save_picbuf();

					return 1;
				}
				case 0x4cb4: {
					CPU_Pop16();
					RealPt ptr = CPU_Pop32();
					CPU_Push32(ptr);

					D1_LOG("restore_picbuf(%x);\n", ptr);

					restore_picbuf(Real2Phys(ptr));

					return 1;
				}
				case 0x4e47: {
					CPU_Pop16();
					D1_LOG("print_attribs();\n");
					print_attribs();
					return 1;
				}
				case 0x4ea0: {
					CPU_Pop16();
					D1_LOG("print_values();\n");
					print_values();
					return 1;
				}
				case 0x5abd: {
					CPU_Pop16();
					RealPt dst = CPU_Pop32();
					Bit32u money = CPU_Pop32();
					CPU_Push32(money);
					CPU_Push32(dst);

					D1_LOG("make_valuta_str(%d);\n", money);

					make_valuta_str(
						(char*)MemBase + Real2Phys(dst),						money);

					return 1;
				}
				case 0x5b21: {
					CPU_Pop16();
					Bit16u skill = CPU_Pop16();
					Bit16u max = CPU_Pop16();
					RealPt msg = CPU_Pop32();
					CPU_Push32(msg);
					CPU_Push16(max);
					CPU_Push16(skill);

					D1_LOG("inc_skill(%d, %d, %p)\n",
						skill, max, (char*)MemBase + Real2Phys(msg));
					inc_skill(skill, max, (char*)MemBase + Real2Phys(msg));
					return 1;
				}
				case 0x5bdd: {
					CPU_Pop16();
					D1_LOG("select_skill();\n");
					select_skill();
					return 1;
				}
				case 0x5fb5: {
					CPU_Pop16();
					Bit16u spell = CPU_Pop16();
					CPU_Push16(spell);

					D1_LOG("inc_spell(%d);\n", spell);
					inc_spell(spell);

					return 1;
				}
				case 0x60db: {
					CPU_Pop16();
					D1_LOG("select_spell();\n");
					select_spell();
					return 1;
				}
				case 0x6716: {
					CPU_Pop16();
					D1_LOG("choose_atpa();\n");
					choose_atpa();
					return 1;
				}
				case 0x6821: {
					CPU_Pop16();
					D1_LOG("choose_typus();\n");
					choose_typus();
					return 1;
				}
				case 0x6b05: {
					CPU_Pop16();

					RealPt dst = CPU_Pop32();
					RealPt src = CPU_Pop32();
					Bit16u n = CPU_Pop16();
					CPU_Push16(n);
					CPU_Push32(src);
					CPU_Push32(dst);

					D1_LOG("pal_fade_out(%x, %x, %d);\n",
						dst, src, n);

					pal_fade_out(MemBase + Real2Phys(dst),
						MemBase + Real2Phys(src), n);

					return 1;
				}
				case 0x6c20: {
					CPU_Pop16();

					RealPt dst = CPU_Pop32();
					RealPt src = CPU_Pop32();
					Bit16u col = CPU_Pop16();
					Bit16u n = CPU_Pop16();
					CPU_Push16(n);
					CPU_Push16(col);
					CPU_Push32(src);
					CPU_Push32(dst);

					D1_LOG("pal_fade_in(%x, %x, %d, %d);\n",
						dst, src, col, n);

					pal_fade_in(MemBase + Real2Phys(dst),
						MemBase + Real2Phys(src),
						col, n);

					return 1;
				}
				case 0x6cf6: {
					CPU_Pop16();

					D1_LOG("intro();\n");
					intro();

					return 1;
				}
				case 0x72e7: {
					CPU_Pop16();

					D1_LOG("set_mouse_isr();\n");
					set_mouse_isr();

					return 1;
				}
				case 0x730b: {
					CPU_Pop16();

					D1_LOG("restore_mouse_isr();\n");
					restore_mouse_isr();

					return 1;
				}
				case 0x7446: {
					/*
					 * We allocate these buffers also on the
					 * host and start using the host memory.
					 */
					CPU_Pop16();
					D1_INFO("alloc_buffers();\n");
					alloc_buffers();
					alloc_buffers_emu();

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
				case 0x7664: {
					RealPt ret;
					CPU_Pop16();
					Bit32u  nelem = CPU_Pop32();

					ret = emu_gen_alloc(nelem);
					D1_LOG("emu_gen_alloc(%d) = %x\n",
						nelem, ret);
					reg_ax = RealOff(ret);
					reg_dx = RealSeg(ret);


					CPU_Push32(nelem);

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
