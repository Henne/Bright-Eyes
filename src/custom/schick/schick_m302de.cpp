#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "SDL.h"

#include "regs.h"

#include "schick.h"

#include "common.h"
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
#include "seg011.h"
#include "seg024.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg031.h"
#include "seg032.h"
#include "seg033.h"
#include "seg034.h"
#include "seg035.h"
#include "seg036.h"
#include "seg037.h"
#include "seg038.h"
#include "seg039.h"
#include "seg040.h"
#include "seg041.h"
#include "seg042.h"
#include "seg043.h"
#include "seg044.h"
#include "seg045.h"
#include "seg046.h"
#include "seg047.h"
#include "seg048.h"
#include "seg049.h"
#include "seg050.h"
#include "seg051.h"
#include "seg052.h"
#include "seg053.h"
#include "seg054.h"
#include "seg055.h"
#include "seg056.h"
#include "seg057.h"
#include "seg058.h"
#include "seg059.h"
#include "seg060.h"
#include "seg061.h"
#include "seg062.h"
#include "seg063.h"
#include "seg064.h"
#include "seg065.h"
#include "seg066.h"
#include "seg067.h"
#include "seg068.h"
#include "seg069.h"
#include "seg070.h"
#include "seg071.h"
#include "seg072.h"
#include "seg073.h"
#include "seg074.h"
#include "seg075.h"
#include "seg076.h"
#include "seg077.h"
#include "seg078.h"
#include "seg079.h"
#include "seg080.h"
#include "seg081.h"
#include "seg082.h"
#include "seg083.h"
#include "seg084.h"
#include "seg085.h"
#include "seg086.h"
#include "seg087.h"
#include "seg088.h"
#include "seg089.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"
#include "seg093.h"
#include "seg094.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg100.h"
#include "seg101.h"
#include "seg102.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"
#include "seg106.h"
#include "seg107.h"
#include "seg108.h"
#include "seg109.h"
#include "seg110.h"
#include "seg111.h"
#include "seg112.h"
#include "seg113.h"
#include "seg114.h"
#include "seg115.h"
#include "seg116.h"
#include "seg117.h"
#include "seg118.h"
#include "seg119.h"
#include "seg120.h"
#include "seg121.h"
#include "seg122.h"

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


void schick_timer_disable()
{
	SDL_RemoveTimer(irq_timer);
	irq_timer = NULL;

	/* restore rand 2 code */
	memcpy(Real2Host(RealMake(reloc_game + 0xb2a, 0x261)),
		irq_bak, 17 * sizeof(char));
	memset(irq_bak, 0, 17 * sizeof(char));
	D1_INFO("IRQ timer deaktiviert\n");
}

void schick_timer_enable()
{
	/* get the adress of the seed2 code */
	Bit8u *loc = Real2Host(RealMake(reloc_game + 0xb2a, 0x261));

	/* copy seed2 code part */
	memcpy(irq_bak, loc, 17 * sizeof(char));
	/*replace it with NOPs */
	memset(loc, 0x90, 17 * sizeof(char));

	irq_timer = SDL_AddTimer(56, schick_irq_timer, NULL);

	if (irq_timer == NULL) {

		D1_ERR("Konnte den IRQ Timer nicht initialisieren\n");

		/* restore rand 2 code */
		memcpy(loc, irq_bak, 17 * sizeof(char));
		return;
	}

	D1_INFO("IRQ timer aktiviert\n");
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

static int n_seg000(unsigned offs) {
	switch (offs) {
		case 0xb33: {
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

static int n_seg001(unsigned offs)
{
	switch (offs) {
	/* Callers: 1 */
	case 0x35: {
		reg_ax = CD_set_drive_nr();
		D1_LOG("CD_set_drive_nr(); = %d:\n", reg_ax);
		return 1;
	}
	/* Callers: 6 */
	case 0x5c: {
		RealPt req = CPU_Pop32();
		CPU_Push32(req);

		CD_driver_request(req);
		D1_LOG("CD_driver_request();\n");
		return 1;
	}
	/* Callers: 4 */
	case 0xb2: {
		unsigned int retval;

		retval = CD_get_tod();

		D1_LOG("CD_get_tod(); = %d\n", retval);
		reg_ax = retval & 0xffff;
		reg_dx = (retval >> 16) & 0xffff;
		return 1;
	}
	/* Callers: 2 */
	case 0xc1: {
		unsigned short track_nr = CPU_Pop16();
		CPU_Push16(track_nr);
		D1_LOG("seg001_00c1(track_nr = %d)\n", track_nr);
		seg001_00c1(track_nr);
		return 1;
	}
	/* Callers: 3 */
	case 0x322: {
		D1_LOG("CD_audio_stop_hsg()\n");
		CD_audio_stop_hsg();
		return 1;
	}
	/* Callers: 1 */
	case 0x34f: {
		D1_LOG("CD_audio_stop()\n");
		CD_audio_stop();
		return 1;
	}
	case 0x0432: {
		return 0;
	}
	case 0x056b: {
		return 0;
	}
	case 0x0681: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg002(unsigned short offs)
{

	switch (offs) {
	case 0x0002: {
		Bit16s index = CPU_Pop16();
		D1_LOG("play_music_file(%d)\n", index);
		play_music_file(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0045: {
		D1_LOG("sound_menu()\n");
		sound_menu();
		return 1;
	}
	case 0x00e6: {
		D1_LOG("near read_sound_cfg()\n");
		read_sound_cfg();
		return 1;
	}
	case 0x01e0: {
		Bit32u size = CPU_Pop32();
		D1_LOG("near init_AIL(%d)\n", size);
		init_AIL(size);
		CPU_Push32(size);
		return 1;
	}
	case 0x0209: {
		D1_LOG("near exit_AIL()\n");
		exit_AIL();
		return 1;
	}
	case 0x02aa: {
		RealPt fname = CPU_Pop32();
		RealPt ret = read_music_driver(fname);
		D1_LOG("near read_music_driver(%s); = %x\n",
				Real2Host(fname), ret);
		CPU_Push32(fname);

		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	case 0x0349: {
		Bit16s seq = CPU_Pop16();
		reg_ax = prepare_midi_playback(seq);
		D1_LOG("prepare_midi_playback(%d)\n", seq);
		CPU_Push16(seq);
		return 1;
	}
	case 0x0413: {
		Bit16s seq = CPU_Pop16();
		reg_ax = start_midi_playback(seq);
		D1_LOG("start_midi_playback(%d)\n", seq);
		CPU_Push16(seq);
		return 1;
	}
	case 0x043d: {
		Bit16s a1 = CPU_Pop16();
		Bit16s patch = CPU_Pop16();
		RealPt p = prepare_timbre(a1, patch);
		D1_LOG("prepare_timbre(%d, %d) = %x\n", a1, patch, p);
		CPU_Push16(patch);
		CPU_Push16(a1);
		reg_ax = RealOff(p);
		reg_dx = RealSeg(p);
		return 1;
	}
	case 0x04f2: {
		Bit16s index = CPU_Pop16();
		reg_ax = load_midi_file(index);
		D1_LOG("load_midi_file(%d)\n", index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0502: {
		Bit16s index = CPU_Pop16();
		reg_ax = do_load_midi_file(index);
		D1_LOG("do_load_midi_file(%d)\n", index);
		CPU_Push16(index);
		return 1;
	}
	case 0x053d: {
		RealPt fname = CPU_Pop32();
		Bit16s type = CPU_Pop16();
		Bit16s port = CPU_Pop16();
		reg_ax = load_music_driver(fname, type, port);
		D1_LOG("load_music_driver(%s, %, 0x%x) = %d\n",
			Real2Host(fname), type, port, (signed short)reg_ax);
		CPU_Push16(port);
		CPU_Push16(type);
		CPU_Push32(fname);
		return 1;
	}
	case 0x069c: {
		Bit16s index = CPU_Pop16();
		do_play_music_file(index);
		D1_LOG("do_play_music_file(%d)\n", index);
		CPU_Push16(index);
		return 1;
	}
	case 0x06c7: {
		D1_LOG("stop_midi_playback();\n");
		stop_midi_playback();
		return 1;
	}
	case 0x079f: {
		reg_ax = have_mem_for_sound();
		D1_LOG("have_mem_for_sound() = %d\n", reg_ax);
		return 1;
	}
	case 0x0832: {
		Bit16u index = CPU_Pop16();
		D1_LOG("near play_voc(FX%d)\n", index - 288);
		play_voc(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0890: {
		Bit32s size = CPU_Pop32();
		CPU_Push32(size);
		D1_LOG("alloc_voc_buffer(%d)\n", size);
		alloc_voc_buffer(size);
		return 1;
	}
	case 0x08b5: {
		D1_LOG("near free_voc_buffer();\n");
		free_voc_buffer();
		return 1;
	}
	case 0x09ff: {
		RealPt fname = CPU_Pop32();
		Bit16s type = CPU_Pop16();
		Bit16s io = CPU_Pop16();
		Bit16s irq = CPU_Pop16();
		reg_ax = load_digi_driver(fname, type, io, irq);
		D1_LOG("load_digi_driver(%s, %, 0x%x, %d) = %d\n",
			Real2Host(fname), type, io, irq, (signed short)reg_ax);
		CPU_Push16(irq);
		CPU_Push16(type);
		CPU_Push32(fname);
		return 1;
	}
	case 0x0adf: {
		RealPt fname = CPU_Pop32();
		RealPt ret = read_digi_driver(fname);
		D1_LOG("near read_digi_driver(%s); = %x\n",
				Real2Host(fname), ret);
		CPU_Push32(fname);

		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	case 0x0b7e: {
		unsigned short fileindex = CPU_Pop16();
		CPU_Push16(fileindex);

		reg_ax = open_and_seek_dat(fileindex);

		D1_LOG("open_and_seek_dat(%s);\n", get_fname(fileindex));

		return 1;
	}
	case 0x0c28: {
		Bit16u handle = CPU_Pop16();
		RealPt buf = CPU_Pop32();
		Bit16u len = CPU_Pop16();
		CPU_Push16(len);
		CPU_Push32(buf);
		CPU_Push16(handle);

		D1_LOG("near read_archive_file(%d, %x, %d);\n",
			handle, buf, len);
		reg_ax = read_archive_file(handle, Real2Host(buf), len);

		return 1;
	}
	case 0x0c72: {
		Bit16u handle = CPU_Pop16();
		Bit32u off = CPU_Pop32();
		Bit16u dummy = CPU_Pop16();
		CPU_Push16(dummy);
		CPU_Push32(off);
		CPU_Push16(handle);

		D1_LOG("near seg002_0c72(%d, %d, %d)\n", handle, off, dummy);
		seg002_0c72(handle, off, dummy);

		return 1;
	}
	case 0x1361: {
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
	case 0x1454: {
		return 0;
	}
	/* Callers: 2 */
	case 0x1634: {
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
	case 0x165e: {
		D1_LOG("mouse_init()\n");
		mouse_init();
		return 1;
	}
	case 0x1742: {
		return 0;
	}
	case 0x174c: {
		return 0;
	}
	/* Callers: 1 */
	case 0x17ae: {
		D1_LOG("mouse_reset_ehandler()\n");
		mouse_reset_ehandler();
		return 1;
	}
	case 0x192b: {
		refresh_screen_size();
		return 1;
	}
	case 0x195d: {
		refresh_screen_size1();
		return 1;
	}
	/* Callers: 2 */
	case 0x19dc: {
		D1_LOG("mouse_19dc()\n");
		mouse_19dc();
		return 1;
	}
	/* Callers: 2 */
	case 0x1cf2: {
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
		D1_LOG("handle_input();\n");
		handle_input();
		return 1;
	}
	case 0x1ee7: {
		D1_LOG("main_loop();\n");
		return 0;
	}
	/* Callers: 1 */
	case 0x20bd: {
		timers_daily();
		D1_LOG("timers_daily();\n");
		return 1;
	}
	/* Callers: 1 */
	case 0x2177: {
		seg002_2177();
		D1_LOG("seg002_2177();\n");
		return 1;
	}
	/* Callers: 2 */
	case 0x21ab: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);
		D1_LOG("pal_fade(%x,%x);\n", p1, p2);
		pal_fade(Real2Host(p1), Real2Host(p2));
		return 1;
	}
	/* Callers: 1 */
	case 0x2400: {
		dawning();
		D1_LOG("dawning()\n");
		return 1;
	}
	/* Callers: 1 */
	case 0x24f0: {
		D1_LOG("nightfall()\n");
		nightfall();
		return 1;
	}
	/* Callers: 1 */
	case 0x25ce: {
		reg_ax = get_current_season();
		return 1;
	}
	/* Callers: 1 */
	case 0x2628: {
		D1_LOG("do_census();\n");
		do_census();
		return 1;
	}
	case 0x274e: {
		D1_LOG("near do_timers();\n");
		do_timers();
		return 1;
	}
	/* Callers: 4 */
	case 0x2bf6: {
		unsigned int val = CPU_Pop32();
		CPU_Push32(val);
		D1_LOG("near sub_ingame_timers(val = %u);\n", val);
		sub_ingame_timers(val);
		return 1;
	}
	/* Callers: 4 */
	case 0x2c5e: {
		unsigned int val = CPU_Pop32();
		CPU_Push32(val);

		D1_LOG("sub_mod_timers(%d);\n", val);
		sub_mod_timers(val);
		return 1;
	}
	/* Callers: 4 */
	case 0x2f7a: {
		Bit32s val = CPU_Pop32();
		CPU_Push32(val);
		D1_LOG("near seg002_2f7a(fmin=%d);\n", val);
		seg002_2f7a(val);
		return 1;
	}
	/* Callers: 4 */
	case 0x3071: {
		Bit32s quarter = CPU_Pop32();
		CPU_Push32(quarter);

		D1_LOG("near sub_light_timers(quarter=%d);\n", quarter);
		sub_light_timers(quarter);

		return 1;
	}
	case 0x31a2: {
		D1_LOG("magical_chainmail_damage()\n");
		magical_chainmail_damage();
		return 1;
	}
	case 0x3230: {
		/* input routines are called faster now,
		   so heros would starve earlier if the cycles are to high */
		if (reg_eip == 0x1d7b || reg_eip == 0x1a4b)
			return 0;

		D1_LOG("near herokeeping();\n");
		herokeeping();

		return 1;
	}
	case 0x373a: {
		D1_LOG("check_level_up();\n");
		check_level_up();
		return 1;
	}
	/* Callers: 1 */
	case 0x3b63: {
		passages_recalc();
		D1_LOG("passages_recalc();\n");
		return 1;
	}
	/* Callers: 1 */
	case 0x3c63: {
		passages_reset();
		D1_LOG("passages_reset();\n");
		return 1;
	}
	/* Callers: 2 */
	case 0x3f3e: {
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
		D1_LOG("near wait_for_keyboard2()\n");
		wait_for_keyboard2();
		return 1;
	}
	/* Callers: 2 */
	case 0x47e2: {
		seg002_47e2();
		D1_LOG("seg002_47e2();\n");
		return 1;
	}
	/* Callers: 2 */
	case 0x484f: {
		seg002_484f();
		D1_LOG("seg002_484f();\n");
		return 1;
	}
	/* Callers: 2 */
	case 0x49d8: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = is_hero_available_in_group(Real2Host(hero));
		D1_LOG("is_hero_available_in_group(%s) = %d\n",
		schick_getCharname(hero), reg_ax);

		return 1;
	}
	case 0x4adc: {
		RealPt hero = CPU_Pop32();
		Bit16s le = CPU_Pop16();
		CPU_Push16(le);
		CPU_Push32(hero);

		D1_LOG("near sub_hero_le(%s, %d);\n",
			(char*)Real2Host(hero) + 0x10, le);

		sub_hero_le(Real2Host(hero), le);

		return 1;
	}
	/* Callers: 1 */
	case 0x4f49: {
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
		Bit16s item = CPU_Pop16();
		Bit16s group = CPU_Pop16();
		CPU_Push16(group);
		CPU_Push16(item);

		reg_ax = get_first_hero_with_item_in_group(item, group);
		D1_LOG("get_first_hero_with_item_in_group(%s = (%d), %d) = %d\n",
			get_itemname(item), item, group, (Bit16s)reg_ax);

		return 1;
	}
	/* Callers: 2 */
	case 0x573e: {
		reg_ax = count_heros_available();
		D1_LOG("count_heros_available() = %d;\n", reg_ax);
		return 1;
	}
	/* Callers: 3 */
	case 0x5799: {
		reg_ax = count_heroes_available_in_group();
		D1_LOG("count_heroes_available_in_group() = %d;\n",
			reg_ax);
		return 1;
	}
	/* Callers: 1 */
	case 0x5a81: {
#if 1
		reg_ax = 1;
		D1_LOG("Kopierschutzabfrage umgangen\n");
#else
		copy_protection();
#endif
		return 1;
	}
	default:
		return 0;
	}
}

static int n_seg004(unsigned short offs)
{
	switch (offs) {
		case 0x55: {
			Bit16u v1 = CPU_Pop16();
			D1_LOG("near init_ani(%d)\n", v1);
			init_ani(v1);
			CPU_Push16(v1);
			return 1;
		}
		case 0x54b: {
			D1_LOG("near update_status_bars()\n");
			update_status_bars();
			return 1;
		}
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

			D1_LOG("near draw_bar(%d,%d,%d,%d,%d)\n", v1, v2, v3, v4, v5);
			draw_bar(v1, v2, v3, v4, v5);
			return 1;
		}
		case 0xe31: {
			D1_LOG("update_wallclock()\n");
			update_wallclock();
			return 1;
		}
		case 0xf54: {
			Bit16s pos = CPU_Pop16();
			Bit16s night = CPU_Pop16();
			D1_LOG("near wallclock(%d, %d)\n", pos, night);
			draw_wallclock(pos, night);
			CPU_Push16(night);
			CPU_Push16(pos);
			return 1;
		}
		case 0x1147: {
			RealPt dst = CPU_Pop32();
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			unsigned short v3 = CPU_Pop16();
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);
			CPU_Push32(dst);

			D1_LOG("near array_add(0x%04x:0x%04x, len=%d, op=%d, flag=%d);\n",
				RealSeg(dst), RealOff(dst), v1, (char)v2, v3);
			array_add(Real2Host(dst), v1, (char)v2, v3);
			return 1;
		}
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
		case 0x1291: {
			return 0;
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

			D1_LOG("near do_v_line(0x%04x:0x%04x, %d, %d, %d, 0x%02x);\n",
				RealSeg(dst), RealOff(dst), x, y1, y2,
				(unsigned char)color);
			do_v_line(dst, x, y1, y2, (unsigned char)color);
			return 1;
		}
		case 0x13b7: {
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);
			D1_LOG("near do_pic_copy(%d);\n", mode);
			do_pic_copy(mode);
			return 1;
		}
		case 0x150d: {
			D1_LOG("wait_for_vsync()\n");
			wait_for_vsync();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int n_seg005(unsigned short offs)
{
	switch (offs) {
	case 0xb: {
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
		RealPt p = CPU_Pop32();
		unsigned short count = CPU_Pop16();
		unsigned short val = CPU_Pop16();
		CPU_Push16(val);
		CPU_Push16(count);
		CPU_Push32(p);

		FIG_set_star_color(Real2Host(p), count, (Bit8u)val);
		D1_LOG("FIG_set_star_color(%x,%d,%d)\n",
			p, count, (Bit8u)val);

		return 1;
	}
	case 0x181: {
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
		reg_ax = fight_printer();
		D1_LOG("fight_printer()\n");
		return 1;
	}
	case 0x1ba7: {
		set_delay_timer();
		D1_LOG("set_delay_timer()\n");
		return 1;
	}
	case 0x1bb2: {
		fight_delay();
		D1_LOG("fight_delay()\n");
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg006(unsigned offs)
{
	switch (offs) {
	/* Callers: 1 */
	case 0x5a: {
		reg_ax = FIG_set_array();
		D1_LOG("FIG_set_array(); = %d\n", (char)reg_ax);
		return 1;
	}
	case 0x82b: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("n FIG_draw_char_pic(%d, %d)\n", v1, v2);
		FIG_draw_char_pic(v1, v2);
		return 1;
	}
	case 0x99f: {
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("n FIG_draw_enemy_pic(%d, %d)\n", v1, v2);
		FIG_draw_enemy_pic(v1, v2);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg024(unsigned short offs)
{
	switch (offs) {
	case 0x1d3: {
		Bit16u line = CPU_Pop16();
		CPU_Push16(line);
		reg_ax = diary_print_entry(line);
		D1_LOG("diary_print_entry(%d); == %d\n", line, reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg025(unsigned short offs)
{
	switch (offs) {
	case 0x4a2: {
		D1_LOG("show_treasure_map();\n");
		show_treasure_map();
		return 1;
	}
	case 0xca8: {
		Bit16s id = CPU_Pop16();
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push16(id);
		D1_LOG("draw_icon(%d,%d,%d);\n", id, x, y);
		draw_icon(id, x, y);
		return 1;
	}
	case 0xd54: {
		D1_LOG("show_storytext();\n");
		reg_ax = show_storytext();
		return 1;
	}
	case 0xea9: {
		D1_LOG("near turnaround();\n");
		turnaround();
		return 1;
	}
	case 0x114a: {
		D1_LOG("copy_palette();\n");
		copy_palette();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg026(unsigned short offs)
{
	switch (offs) {
	case 0x23e: {
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
		RealPt str = CPU_Pop32();
		Bit16s hero_pos = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(hero_pos);
		CPU_Push32(str);

		reg_ax = read_chr_temp(str, hero_pos, a2);
		D1_LOG("read_chr_temp(%s, %d, %d) = %d\n",
			(char*)Real2Host(str),
			hero_pos,
			a2, reg_ax);
		return 1;
	}
	case 0x117f: {
		Bit16u hero = CPU_Pop16();
		CPU_Push16(hero);
		D1_LOG("write_chr_temp(%d)\n", hero);
		write_chr_temp(hero);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg028(unsigned offs) {
	switch (offs) {
	case 0x000: {
		D1_LOG("prepare_dungeon_area()\n");
		prepare_dungeon_area();
		return 1;
	}
	case 0x19e: {
		D1_LOG("load_dungeon_ddt()\n");
		load_dungeon_ddt();
		return 1;
	}
	case 0x224: {
		D1_LOG("seg028_0224()\n");
		seg028_0224();
		return 1;
	}
	case 0x444: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		Bit16s a4 = CPU_Pop16();
		CPU_Push16(a4);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);

		RealPt retval = seg028_0444(a1, a2, a3, a4);
		D1_LOG("seg028_0444(%d, %d, %d, %d) = %x\n",
			a1, a2, a3 ,a4, retval);

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);
		return 1;
	}
	case 0x56a: {
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("load_area_description(%d);\n", arg);
		load_area_description(arg);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg029(unsigned offs) {
	switch (offs) {
	case 0x000: {
		D1_LOG("draw_playmask();\n");
		draw_playmask();
		return 1;
	}
	case 0x0e8: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		copy_forename(Real2Host(p1), Real2Host(p2));
		D1_LOG("near copy_forename(%s, %s)\n",
			(char*)Real2Host(p1), (char*)Real2Host(p2));
		return 1;
	}
	case 0x127: {
		D1_LOG("draw_status_line();\n");
		draw_status_line();
		return 1;
	}
	case 0x417: {
		unsigned short pos = CPU_Pop16();
		CPU_Push16(pos);

		clear_hero_icon(pos);
		D1_LOG("clear_hero_icon(%d)\n", pos);
		return 1;
	}
	case 0x492: {
		Bit16u index = CPU_Pop16();
		Bit16u icon = CPU_Pop16();
		Bit16u pos = CPU_Pop16();

		D1_LOG("load_icon(%x, %x, %x);\n", index, icon, pos);
		load_icon(index, icon, pos);

		CPU_Push16(pos);
		CPU_Push16(icon);
		CPU_Push16(index);

		return 1;
	}
	case 0x4fd: {
		D1_LOG("draw_icons();\n");
		draw_icons();
		return 1;
	}
	case 0x5ff: {
		D1_LOG("near clear_loc_line();\n");
		clear_loc_line();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg030(unsigned offs) {
	switch (offs) {
	case 0x000: {
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("seg030_0000(%d);\n", arg);
		RealPt retval = seg030_0000(arg);
		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);
		return 1;
	}
	case 0x8d: {
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("seg030_008d(%d);\n", arg);
		RealPt retval = seg030_008d(arg);

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);
		return 1;
	}
	case 0x14f: {
		return 0;
	}
	case 0xfd5: {
		D1_LOG("talk_switch();\n");

		/* TODO: all TLK_ID [0,19] */
		Bit16s id = ds_readws(TLK_ID);

		if (id == 1 || (id >= 3 && id <= 10) ||
			(id >= 12 && id <= 16))
		{
			talk_switch();
			return 1;
		}

		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg031(unsigned offs) {
	switch (offs) {
	case 0x56c: {
		reg_ax = get_town_lookup_entry();
		D1_LOG("get_town_lookup_entry(); = %d\n", reg_ax);
		return 1;
	}
	case 0x63b: {
		RealPt retval;

		retval = load_current_town_gossip();
		D1_LOG("near load_current_town_gossip() = 0x%x\n", retval);

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg032(unsigned offs)
{
	switch (offs) {
	/* Callers: 1 */
	case 0x0000: {
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
	case 0x0032: {
		Bit16s mode = CPU_Pop16();
		D1_LOG("draw_fight_screen_pal(%d)\n", mode);
		draw_fight_screen_pal(mode);
		CPU_Push16(mode);
		return 1;
	}
	/* Callers: 1 */
	case 0xa8: {
		reg_ax = FIG_choose_next_hero();
		D1_LOG("FIG_choose_next_hero() = %s\n",
		schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da));
		return 1;
	}
	/* Callers: 1 */
	case 0xfc: {
		reg_ax = FIG_choose_next_enemy();
		D1_LOG("FIG_choose_next_enemy() = %d\n", reg_ax);
		return 1;
	}
	/* Callers: 1 */
	case 0x12c: {
		reg_ax = FIG_count_active_enemies();
		D1_LOG("near FIG_count_active_enemies() = %d\n", reg_ax);
		return 1;
	}
	/* Callers: 1 */
	case 0x1ba: {
		RealPt enemy = CPU_Pop32();
		CPU_Push32(enemy);

		reg_ax = FIG_is_enemy_active(Real2Host(enemy) );
		D1_LOG("near FIG_is_enemy_active(); = %d\n", reg_ax);
		return 1;
	}
	/* Callers: 2 */
	case 0x242: {
		reg_ax = FIG_get_first_active_hero();
		D1_LOG("near FIG_get_first_active_hero() = %s\n",
			reg_ax != -1 ? schick_getCharname(ds_readd(0xbd34) + reg_ax * 0x6da) : "none");
		return 1;
	}
	/* Callers: 1 */
	case 0x2db: {
		reg_ax = seg032_02db();
		D1_LOG("near seg032_02db() = %d\n", reg_ax);
		return 1;
	}
	/* Callers: 1 */
	case 0x349: {
		reg_ax = FIG_fight_continues();
		D1_LOG("FIG_fight_continues() = %d\n", reg_ax);
		return 1;
	}
	case 0x380: {
		D1_LOG("FIG_do_round()\n");
		FIG_do_round();
		return 1;
	}
	case 0xa38: {
		D1_LOG("FIG_load_ship_sprites()\n");
		FIG_load_ship_sprites();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg034(unsigned offs)
{
	switch (offs) {
	case 0x000: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		Bit16s a4 = CPU_Pop16();
		Bit16s a5 = CPU_Pop16();
		Bit16s a6 = CPU_Pop16();
		Bit16s a7 = CPU_Pop16();
		CPU_Push16(a7);
		CPU_Push16(a6);
		CPU_Push16(a5);
		CPU_Push16(a4);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);

		reg_ax = seg034_000(a1, a2, a3, a4, a5, a6, a7);
		D1_LOG("near seg034_000(%d, %d, %d, %d, %d, %d, %d) = %d\n",
			a1, a2, a3, a4, a5, a6, a7, (Bit16s)reg_ax);
		return 1;
	}
	case 0x718: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		RealPt a3 = CPU_Pop32();
		RealPt a4 = CPU_Pop32();
		Bit16s a5 = CPU_Pop16();
		Bit16s a6 = CPU_Pop16();
		CPU_Push16(a6);
		CPU_Push16(a5);
		CPU_Push32(a4);
		CPU_Push32(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);

		seg034_718(a1, a2, Real2Host(a3), Real2Host(a4), a5, a6);

		return 1;
	}
	case 0xaec: {
		reg_ax = seg034_aec();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}


static int n_seg036(unsigned offs)
{
	switch (offs) {
	case 0x000: {
		RealPt p = CPU_Pop32();
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = KI_copy_ani_sequence(Real2Host(p), v1, v2);
		D1_LOG("KI_copy_ani_sequence(%x, %d, %d); = %d\n",
			p, v1, v2, (signed char)reg_ax);

		return 1;
	}
	case 0x0ae: {
		RealPt in_ptr = CPU_Pop32();
		Bit16s a2 = CPU_Pop16();
		D1_LOG("near seg036_00ae(%x, %d)\n", in_ptr, a2);
		seg036_00ae(Real2Host(in_ptr), a2);
		CPU_Push16(a2);
		CPU_Push32(in_ptr);
		return 1;
	}
	case 0x27f: {
		RealPt hero = CPU_Pop32();
		reg_ax = KI_change_hero_weapon(Real2Host(hero));
		D1_LOG("KI_change_hero_weapon(%s) = %d\n",
			Real2Host(hero) + 0x10, reg_ax);
		CPU_Push32(hero);
		return 1;
	}
	case 0x39b: {
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		Bit16s v5 = CPU_Pop16();
		CPU_Push16(v5);
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		reg_ax = KI_can_attack_neighbour(v1, v2, v3, v4, v5);

		D1_LOG("KI_can_attack_neighbour(%d,%d,%d,%d,%d); = %d\n",
			v1, v2, v3, v4, v5, reg_ax);
		return 1;
	}
	case 0x4cf: {
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		reg_ax = KI_search_spell_target(v1, v2, v3, v4);

		D1_LOG("KI_get_spell_targed_id(%d,%d,%d,%d); = %d\n",
			v1, v2, v3, v4, reg_ax);
		return 1;
	}
	case 0x6f7: {
		RealPt p = CPU_Pop32();
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = KI_select_spell_target(Real2Host(p), v1, v2, v3, v4);
		D1_LOG("KI_select_spell_target(%s, %d, %d, %d, %d) = %d\n",
			schick_getCharname(p), v1, v2, v3, v4, reg_ax);

		return 1;
	}
	case 0x863: {
		Bit16s spell = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(spell);
		reg_ax = KI_get_spell(spell, v2);
		D1_LOG("KI_get_spell(%s, %d); = %d\n",
			names_spell[spell], v2, (signed short)reg_ax);
		return 1;
	}
	case 0x8cf: {
		RealPt p = CPU_Pop32();
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = seg036_8cf(Real2Host(p), v1, v2, v3, v4);
		D1_LOG("seg036_8cf(%s, %d, %d, %d, %d) = %d\n",
			schick_getCharname(p), v1, v2, v3, v4, reg_ax);

		return 1;
	}
	case 0xc39: {
		Bit16u v = CPU_Pop16();
		CPU_Push16(v);

		reg_ax = KI_count_heros(v);
		D1_LOG("KI_count_heros(%d); = %d\n", v, reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg037(unsigned offs) {
	switch (offs) {
	case 0x000: {
		RealPt p = CPU_Pop32();
		Bit16u v1 = CPU_Pop16();
		Bit16u v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = copy_ani_stuff(Real2Host(p), v1, v2);
		D1_LOG("copy_ani_stuff(%x, %d, %d); = %x\n", p, v1, v2, reg_ax);
		return 1;
	}
	case 0x00ae: {
		RealPt p = CPU_Pop32();
		Bit16s v1 = CPU_Pop16();
		CPU_Push16(v1);
		CPU_Push32(p);

		D1_LOG("seg037_00ae(%x, %d);\n", p, v1);
		seg037_00ae(Real2Host(p), v1);
		return 1;
	}
	case 0x2e3: {
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		Bit16s v5 = CPU_Pop16();
		CPU_Push16(v5);
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		reg_ax = test_foe_melee_attack(v1, v2, v3, v4, v5);
		D1_LOG("test_foe_melee_attack(%d, %d, %d, %d, %d); = %x\n",
				v1, v2, v3, v4, v5, reg_ax);
		return 1;
	}
	case 0x417: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		Bit16s dir = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		reg_ax = test_foe_range_attack(x, y, dir, v4);
		D1_LOG("test_foe_range_attack(%d, %d, %d, %d); = %d attacker = %d\n",
			x, y, dir, v4, reg_ax, get_cb_val(x, y));
		CPU_Push16(v4);
		CPU_Push16(dir);
		CPU_Push16(y);
		CPU_Push16(x);
		return 1;
	}
	case 0x725: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		reg_ax = get_foe_attack_mode(a1, a2);
		D1_LOG("get_foe_attack_mode(%d, %d); = %d\n",
			a1, a2, reg_ax);
		CPU_Push16(a2);
		CPU_Push16(a1);
		return 1;
	}
	case 0x791: {
		RealPt p = CPU_Pop32();
		Bit16u a2 = CPU_Pop16();
		Bit16u a3 = CPU_Pop16();
		Bit16u x = CPU_Pop16();
		Bit16u y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push32(p);

		reg_ax = seg037_0791(Real2Host(p), a2, a3, x, y);
		D1_LOG("seg037_0791(%x, %d, %d, %d, %d); = %x\n", p, a2, a3, x, y, reg_ax);
		return 1;
	}
	case 0xb3e: {
		D1_LOG("%s:%x\n", __func__, offs);
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg038(unsigned offs)
{
	switch (offs) {
	case 0x000: {
		signed short obj = CPU_Pop16();
		RealPt px = CPU_Pop32();
		RealPt py = CPU_Pop32();
		CPU_Push32(py);
		CPU_Push32(px);
		CPU_Push16(obj);

		reg_ax = FIG_search_obj_on_cb(obj,
			(signed short*)Real2Host(px),
			(signed short*)Real2Host(py));

		D1_LOG("near FIG_search_obj_on_cb(obj=%d, x=%d, y=%d) = %d\n",
			obj, mem_readw(Real2Phys(px)),
			mem_readw(Real2Phys(py)), reg_ax);

		host_writew(Real2Host(px),
			*((signed short*)Real2Host(px)));
		host_writew(Real2Host(py),
			*((signed short*)Real2Host(py)));

		return 1;
	}
	case 0x143: {
		RealPt in_ptr = CPU_Pop32();
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		Bit16s arg4 = CPU_Pop16();
		Bit16s arg5 = CPU_Pop16();
		Bit16s arg6 = CPU_Pop16();
		Bit16s arg7 = CPU_Pop16();
		Bit16s arg8 = CPU_Pop16();

		D1_LOG("FIG_backtrack(%x, %d, %d, %d, %d, %d, %d, %d)\n",
			in_ptr, x, y, arg4, arg5, arg6, arg7, arg8);

		FIG_backtrack(Real2Host(in_ptr), x, y, arg4, (Bit8s)arg5,
							arg6, arg7, arg8);

		CPU_Push16(arg8);
		CPU_Push16(arg7);
		CPU_Push16(arg6);
		CPU_Push16(arg5);
		CPU_Push16(arg4);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(in_ptr);

		return 1;
	}
	case 0x457: {
		RealPt p = CPU_Pop32();
		CPU_Push32(p);

		reg_ax = FIG_count_smth((signed char*)Real2Host(p));
		D1_LOG("FIG_count_smth(%x) = %d\n", p, reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg039(unsigned offs)
{

	switch(offs) {

	case 0x23: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = FIG_get_range_weapon_type(Real2Host(hero));
		D1_LOG("FIG_get_range_weapon_type(%s) = %d\n",
			schick_getCharname(hero), (signed short)reg_ax);

		return 1;
	}
	case 0x97: {
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
	case 0x546: {
		RealPt p = CPU_Pop32();
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		CPU_Push16(v2);
		CPU_Push16(v1);
		CPU_Push32(p);

		D1_LOG("FIG_load_enemy_sprites(%x, %d, %d);\n", p, v1, v2);
		FIG_load_enemy_sprites(Real2Host(p),
			(signed char)v1, (signed char)v2);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg041(unsigned offs)
{
	switch (offs) {
	case 0x8f1: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = weapon_check(Real2Host(hero));

		D1_LOG("near weapon_check(%s); = %d\n",
			schick_getCharname(hero), (signed short)reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg044(unsigned short offs)
{
	switch (offs) {
	case 0x00: {
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
		Bit16s ani = CPU_Pop16();
		CPU_Push16(ani);

		reg_ax = (Bit16s)get_seq_header(ani);
		D1_LOG("get_seq_header(%d) = %d\n", ani, (Bit16s)reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg045(unsigned short offs)
{
	switch (offs) {
	case 0x0000: {
		Bit16s fight_id = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		D1_LOG("seg045_0000(%d, %d, %d);", fight_id, a2, a3);
		seg045_0000(fight_id, a2, a3);

		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(fight_id);
		return 1;
	}
	case 0x014f: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		Bit16s v = CPU_Pop16();

		CPU_Push16(v);
		CPU_Push32(p2);
		CPU_Push32(p1);
		reg_ax = FIG_copy_it(Real2Host(p1), Real2Host(p2), (signed char)v);
		D1_LOG("FIG_copy_it(%x, %x, %x) = %d\n",
			p1, p2, (signed char)v, reg_ax);
		return 1;
	}
	case 0x0273: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		D1_LOG("seg045_0273(%d, %d, %d);\n", a1, a2, a3);
		seg045_0273((signed char)a1, (signed char)a2, a3);

		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg046(unsigned short offs)
{
	switch (offs) {
	case 0x000: {
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
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		D1_LOG("status_show_talents(%s);\n",
			schick_getCharname(hero));

		status_show_talents(Real2Host(hero));

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg048(unsigned short offs)
{
	switch(offs) {
	case 0x00: {
		D1_LOG("reset_item_selector()\n");
		reset_item_selector();
		return 1;
	}
	case 0xd0: {
		Bit16s hero_pos = CPU_Pop16();
		CPU_Push16(hero_pos);
		D1_LOG("status_menu(%d);\n", hero_pos);
		status_menu(hero_pos);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg049(unsigned short offs)
{
	switch (offs) {
	case 0x01da: {
		GRP_sort_heros();
		D1_LOG("near GRP_sort_heros()\n");
		return 1;
	}
	case 0x0224: {
		Bit16s group = CPU_Pop16();
		D1_LOG("near GRP_save_pos(%d)\n", group);
		GRP_save_pos(group);
		CPU_Push16(group);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg050(unsigned short offs) {
	switch (offs) {
		case 0x0000: {
			RealPt hero = CPU_Pop32();
			Bit16s spell = CPU_Pop16();

			D1_LOG("inc_spell_advanced(%s, %s);\n",
				schick_getCharname(hero), names_spell[spell]);

			inc_spell_advanced(Real2Host(hero), spell);

			CPU_Push16(spell);
			CPU_Push32(hero);
			return 1;
		}
		case 0x01ec: {
			RealPt hero = CPU_Pop32();
			Bit16s skill = CPU_Pop16();

			D1_LOG("inc_skill_advanced(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);

			inc_skill_advanced(Real2Host(hero), skill);

			CPU_Push16(skill);
			CPU_Push32(hero);
			return 1;
		}
		case 0x03d2: {
			RealPt hero = CPU_Pop32();
			Bit16s skill = CPU_Pop16();

			D1_LOG("inc_skill_novice(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);
			inc_skill_novice(Real2Host(hero), skill);

			CPU_Push16(skill);
			CPU_Push32(hero);
			return 1;
		}
		case 0x04b1: {
			RealPt hero = CPU_Pop32();
			Bit16s spell = CPU_Pop16();

			D1_LOG("inc_spell_novice(%s, %s);\n",
				schick_getCharname(hero), names_spell[spell]);

			inc_spell_novice(Real2Host(hero), spell);
			CPU_Push16(spell);
			CPU_Push32(hero);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg053(unsigned short offs) {
	switch (offs) {
		case 0x0000: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = is_hero_healable(Real2Host(hero));

			D1_LOG("near is_hero_healable(%s); = %d\n",
				Real2Host(hero) + 0x10, reg_ax);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg054(unsigned short offs)
{
	switch (offs) {
		case 0x0000: {
			D1_LOG("talk_inn()\n");
			return 0;
		}
		case 0x0011: {
			RealPt hero = get_first_busy_hero();
			D1_LOG("get_first_busy_hero() = %s\n",
				hero != NULL ? schick_getCharname(hero) : "NULL");

			reg_ax = RealOff(hero);
			reg_dx = RealSeg(hero);
			return 1;
		}
		case 0x007c: {
			D1_LOG("do_inn()\n");
			return 0;
		}
		case 0x0ca2: {
			Bit16s state = CPU_Pop16();
			CPU_Push16(state);
			D1_LOG("TLK_herberg(%d)\n", state);
			TLK_herberg(state);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg055(unsigned short offs)
{
	switch (offs) {
		case 0x0000: {
			RealPt shop_ptr = CPU_Pop32();
			Bit16s item_id = CPU_Pop16();
			Bit16s pos = CPU_Pop16();
			CPU_Push16(pos);
			CPU_Push16(item_id);
			CPU_Push32(shop_ptr);

			D1_LOG("add_item_to_shop(%x, %s, %d)\n", shop_ptr, get_itemname(item_id), pos);
			add_item_to_shop(Real2Host(shop_ptr), item_id, pos);
			return 1;
		}
		case 0x007a: {
			const Bit8u typi = ds_readb(0x4224);
			const Bit8s price = ds_readbs(typi * 9 + 0x6870);
			const Bit8s h_type = ds_readb(typi * 9 + 0x6870 + 1);
			const Bit8s sortiment = ds_readb(typi * 9 + 0x6870 + 2);

			const char h_types[][10] = { "UNGUELTIG", "Waffen", "Kraeuter", "Kraemer" };
			const char *h_string = (h_type >= 1 && h_type <= 3) ? h_types[h_type] : h_types[0];

			D1_INFO("Haendler-Nr: %d / Haendlertyp: %s\n", typi, h_string);
			D1_INFO("\tPreise: %3d%% [70, 180]\n", 100 + price);
			D1_INFO("\tAuswahl: %2d [0, 18] (je kleiner der Wert, desto groesser die Auswahl)\n", sortiment);

			return 0;
		}
		case 0x0622: {
			return 0;
		}
		case 0x0660: {
			Bit16s state = CPU_Pop16();
			CPU_Push16(state);

			D1_LOG("TLK_ghandel(%d)\n", state);
			TLK_ghandel(state);
			return 1;
		}
		case 0x06e1: {
			Bit16s state = CPU_Pop16();
			CPU_Push16(state);

			D1_LOG("TLK_khandel(%d)\n", state);
			TLK_khandel(state);
			return 1;
		}
		case 0x078e: {
			Bit16s state = CPU_Pop16();
			CPU_Push16(state);

			D1_LOG("TLK_whandel(%d)\n", state);
			TLK_whandel(state);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg058(const unsigned short offs)
{
	switch (offs) {
		case 0x0000: {
			RealPt smith_ptr = CPU_Pop32();
			RealPt hero = CPU_Pop32();
			Bit16s item_pos = CPU_Pop16();
			Bit16s smith_pos = CPU_Pop16();
			CPU_Push16(smith_pos);
			CPU_Push16(item_pos);
			CPU_Push32(hero);
			CPU_Push32(smith_ptr);

			D1_LOG("add_item_to_smith(%x, %s, %d, %d);\n",
				smith_ptr,
				schick_getCharname(hero),
				item_pos,
				smith_pos);
			add_item_to_smith(Real2Host(smith_ptr), Real2Host(hero), item_pos, smith_pos);
			return 1;
		}
		case 0x01f9: {
			RealPt smith_ptr = CPU_Pop32();
			Bit16s a1 = CPU_Pop16();
			CPU_Push16(a1);
			CPU_Push32(smith_ptr);

			D1_LOG("repair_screen(%x, %d)\n", smith_ptr, a1);
			repair_screen(Real2Host(smith_ptr), a1);

			return 1;
		}
		case 0x0dc3: {
			return 0;
		}
		case 0x0f96: {
			return 0;
		}
		case 0x0fa7: {
			Bit16s state = CPU_Pop16();
			CPU_Push16(state);

			D1_LOG("TLK_schmied(%d)\n", state);
			TLK_schmied(state);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg061(unsigned offs) {
	switch (offs) {
		case 0x57b: {
			Bit16s temple_id = CPU_Pop16();
			CPU_Push16(temple_id);

			D1_LOG("char_add(%d)\n", temple_id);
			char_add(temple_id);
			return 1;
		}
		case 0x74a: {
			Bit16s temple_id = CPU_Pop16();
			CPU_Push16(temple_id);

			D1_LOG("char_letgo(%d)\n", temple_id);
			char_letgo(temple_id);
			return 1;
		}
		case 0x89d: {
			reg_ax = char_erase();
			D1_LOG("char_erase(); = %d\n", reg_ax);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int n_seg063(unsigned offs) {
	switch (offs) {
	case 0x999: {
		return 0;
	}
	case 0xa0e: {
		return 0;
	}
	case 0xf6f: {
		RealPt ptr = CPU_Pop32();
		CPU_Push32(ptr);
		reg_ax = get_srout_len(Real2Host(ptr));
		D1_LOG("get_srout_len(0x%x) = %d;\n", ptr, reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg064(unsigned offs) {
	switch (offs) {
	case 0x000: {
		Bit16s arg1 = CPU_Pop16();
		Bit16s arg2 = CPU_Pop16();
		CPU_Push16(arg2);
		CPU_Push16(arg1);
		RealPt tmp = get_ship_name((signed char)arg1, arg2);
		D1_LOG("get_ship_name(%d, %d); = 0x%x\n",
			(signed char)arg1, arg2, tmp);

		reg_ax = RealOff(tmp);
		reg_dx = RealSeg(tmp);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg066(unsigned offs)
{
	switch (offs) {
	case 0x000: {
		return 0;
	}
	case 0x0dd: {
		return 0;
	}
	case 0x5fc: {
		return 0;
	}
	case 0x692: {
		return 0;
	}
	case 0x6c1: {
		return 0;
	}
	case 0xb73: {
		Bit16s v1 = CPU_Pop16();
		CPU_Push16(v1);
		reg_ax = get_border_index((unsigned char)v1);
		D1_LOG("get_border_index(%d) = %d\n", v1, reg_ax);
		return 1;
	}
	case 0xbad: {
		return 0;
	}
	case 0xc50: {
		return 0;
	}
	case 0xd1d: {
		return 0;
	}
	case 0xf62: {
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		load_city_texture(v1, v2, v3, v4);

		D1_LOG("load_city_texture(%d, %d, %x, %d)\n",
			v1, v2, v3, v4);
		return 1;
	}
	case 0x10c8: {
		return 0;
	}
	case 0x10e9: {
		return 0;
	}
	case 0x14dd: {
		return 0;
	}
	case 0x159b: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg067(unsigned offs)
{
	switch (offs) {
	case 0x000: {
		D1_LOG("city_event_switch()\n");
		return 0;
	}
	case 0x009a: {
		D1_LOG("city_event_1()\n");
		city_event_1();
		return 1;
	}
	case 0x02eb: {
		D1_LOG("city_event_2()\n");
		city_event_2();
		return 1;
	}
	case 0x04a6: {
		D1_LOG("city_event_3()\n");
		city_event_3();
		return 1;
	}
	case 0x05aa: {
		D1_LOG("city_event_4()\n");
		city_event_4();
		return 1;
	}
	case 0x0707: {
		D1_LOG("city_event_5()\n");
		city_event_5();
		return 1;
	}
	case 0x07b2: {
		D1_LOG("city_event_6()\n");
#if 0
		city_event_6();
		return 1;
#else
		return 0;
#endif
	}
	case 0x090a: {
		D1_LOG("city_event_7()\n");
		city_event_7();
		return 1;
	}
	case 0x0b2d: {
		D1_LOG("city_event_8()\n");
		city_event_8();
		return 1;
	}
	case 0x0c1c: {
		D1_LOG("city_event_9()\n");
		city_event_9();
		return 1;
	}
	case 0x0ca8: {
		D1_LOG("waffinfo_weapons()\n");
		RealPt ret = waffinfo_weapons();
		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	case 0x0ce6: {
		D1_LOG("waffinfo_herbs()\n");
		RealPt ret = waffinfo_herbs();
		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	case 0x0d27: {
		D1_LOG("waffinfo_general()\n");
		RealPt ret = waffinfo_general();
		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg072(unsigned short offs)
{
	switch (offs) {
	case 0x146a: {
		D1_LOG("count_map_parts()\n");
		reg_ax = count_map_parts();
		return 1;
	}
	case 0x148b: {
		D1_LOG("has_intro_letter()\n");
		reg_ax = has_intro_letter();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg074(unsigned short offs)
{
	switch (offs) {
	case 0x24a: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		reg_ax = is_discovered(x, y);
		D1_LOG("is_discovered(%d, %d) = %d\n", x, y, reg_ax);
		return 1;
	}
	case 0x270: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		reg_ax = get_mapval_small(x, y);
		D1_LOG("get_mapval_small(%d, %d) = %d\n", x, y, reg_ax);
		return 1;
	}
	case 0x295: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		reg_ax = get_mapval_large(x, y);
		D1_LOG("get_mapval_large(%d, %d) = %d\n", x, y, reg_ax);
		return 1;
	}
	case 0x2ba: {
		Bit16s group_nr = CPU_Pop16();
		CPU_Push16(group_nr);

		reg_ax = is_group_in_prison(group_nr);
		D1_LOG("is_group_in_prison(%d) = %d\n", group_nr, reg_ax);
		return 1;
	}
	case 0x305: {
		Bit16s x_off = CPU_Pop16();
		CPU_Push16(x_off);

		seg074_305(x_off);
		D1_LOG("seg074_305(%d)\n", x_off);
		return 1;
	}
	case 0x5f9: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		Bit16s dir = CPU_Pop16();
		CPU_Push16(dir);
		CPU_Push16(a3);
		CPU_Push16(y);
		CPU_Push16(x);

		draw_automap_square(x, y, a3, dir);
		D1_LOG("draw_automap_square(%d, %d, %d, %d)\n", x, y, a3, dir);
		return 1;
	}
	case 0x72b: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		Bit16s dir = CPU_Pop16();
		CPU_Push16(dir);
		CPU_Push16(y);
		CPU_Push16(x);

		draw_automap_entrance(x, y, dir);
		D1_LOG("draw_automap_entrance(%d, %d, %d)\n", x, y, dir);
		return 1;
	}
	case 0x7b2: {
		draw_automap_to_screen();
		D1_LOG("draw_automap_to_screen()\n");
		return 1;
	}
	case 0xbbb: {
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);

		reg_ax = seg074_bbb(x, y);
		D1_LOG("seg074_bbb(%d, %d) = %d\n", x, y, reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg075(unsigned short offs)
{
	switch (offs) {
	case 0x000: {
		D1_LOG("DNG_floor_ceil();\n");
		DNG_floor_ceil();
		return 1;
	}
	case 0x0b9: {
		D1_LOG("DNG_turn()\n");
		DNG_turn();
		return 1;
	}
	case 0x56b: {
		D1_LOG("DNG_stub1();\n");
		DNG_stub1();
		return 1;
	}
	case 0x591: {
		D1_LOG("DNG_stub2()\n");
		DNG_stub2();
		return 1;
	}
	case 0x5e5: {
		D1_LOG("DNG_stub3()\n");
		DNG_stub3();
		return 1;
	}
	case 0x693: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		D1_LOG("DNG_draw_walls(%d, %d, %d)\n", a1, a2, a3);
		DNG_draw_walls(a1, a2, a3);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);
		return 1;
	}
	case 0x82e: {
		D1_LOG("DNG_stub4();\n");
		DNG_stub4();
		return 1;
	}
	case 0x9ef: {
		D1_LOG("DNG_stub5();\n");
		DNG_stub5();
		return 1;
	}
	case 0xa46: {
		reg_ax = is_staff_lvl2_in_group();
		D1_LOG("is_staff_lvl2_in_group() %d\n", reg_ax);
		return 1;
	}
	case 0xaaa: {
		D1_LOG("DNG_lights()\n");
		DNG_lights();
		return 1;
	}
	case 0xc6d: {
		D1_LOG("near DNG_update_pos()\n");
		DNG_update_pos();
		return 1;
	}
	case 0xc8e: {
		D1_LOG("DNG_inc_level()\n");
		DNG_inc_level();
		return 1;
	}
	case 0x10de: {
		return 0;
	}
	case 0x1168: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg076(unsigned short offs)
{
	switch (offs) {
	case 0x000: {
		return 0;
	}
	case 0x576: {
		RealPt msg = CPU_Pop32();
		CPU_Push32(msg);

		D1_LOG("near print_msg_with_first_hero()\n");
		print_msg_with_first_hero(Real2Host(msg));
		return 1;
	}
	case 0x71d: {
		return 0;
	}
	case 0xc73: {
		return 0;
	}
	case 0xd28: {
		return 0;
	}
	case 0xdaa: {
		return 0;
	}
	case 0xe89: {
		return 0;
	}
	case 0x11e7: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg090(unsigned short offs)
{
	switch (offs) {
	case 0xa94: {
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		D1_LOG("DNG_clear_corridor()\n");
		DNG_clear_corridor(Real2Host(p));
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg091(unsigned short offs)
{
	switch (offs) {
	case 0x44f: {
		return 0;
	}
	case 0x613: {
		return 0;
	}
	case 0x721: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg092(unsigned short offs)
{
	switch (offs) {
	case 0x4d6: {
		RealPt chest = CPU_Pop32();
		Bit16s item_nr = CPU_Pop16();
		CPU_Push16(item_nr);
		CPU_Push32(chest);

		D1_LOG("delete_chest_item(%d)\n", item_nr);
		delete_chest_item(Real2Host(chest), item_nr);

		return 1;
	}
	case 0x66d: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_has_lockpicks(Real2Host(hero));
		D1_LOG("near hero_has_lockpicks(%s)= %d\n",
			schick_getCharname(hero), (Bit16s)reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg094(unsigned short offs)
{
	switch (offs) {
	case 0x0092: {
		Bit16s town_id = CPU_Pop16();
		CPU_Push16(town_id);
		D1_LOG("set_textbox_positions(%d)\n", town_id);
		set_textbox_positions(town_id);
		return 1;
	}
	case 0x0125: {
		return 0;
	}
	case 0x0de2: {
		return 0;
	}
	case 0x0f0e: {
		return 0;
	}
	case 0x0f58: {
		return 0;
	}
	case 0x105d: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}


static int n_seg095(unsigned short offs)
{
	switch (offs) {
	case 0x335: {
		npc_nariell();
		return 1;
	}
	case 0x44d: {
		npc_harika();
		return 1;
	}
	case 0x62d: {
		npc_curian();
		return 1;
	}
	case 0x746: {
		npc_ardora();
		return 1;
	}
	case 0x9a5: {
		npc_garsvik();
		return 1;
	}
	case 0xad0: {
		npc_erwo();
		return 1;
	}
	case 0xbfb: {
		Bit16s head_index = CPU_Pop16();
		Bit16s days = CPU_Pop16();
		Bit16s index = CPU_Pop16();
		RealPt name = CPU_Pop32();
		RealPt text = CPU_Pop32();

		remove_npc(head_index, (signed char)days, index,
			Real2Host(name), Real2Host(text));

		D1_LOG("remove_npc(%x, %d, %x, %x, %x);\n",
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
		Bit16s index = CPU_Pop16();
		D1_LOG("add_npc(%s);\n", get_fname(index));
		add_npc(index);
		CPU_Push16(index);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg097(unsigned short offs)
{

	switch (offs) {
#if 0
	/* Callers: 0 */
	case 0x000:
	/* Callers: 1 */
	case 0x129:
		return 0;
	/* Callers: 1 */
	case 0x15e: {
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
		RealPt dst = CPU_Pop32();
		Bit16s x = CPU_Pop16();
		Bit16s y = CPU_Pop16();
		Bit16s num = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(num);
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(dst);


		reg_ax = GUI_enter_text(Real2Host(dst),	x, y, num, v4);
		D1_LOG("GUI_enter_text(0x%x, %d, %d, %d, %d); = 0x%x\n",
			dst, x, y, num, v4, reg_ax);

		return 1;
	}
	/* Callers: 3 */
	case 0x4ae: {
		Bit16s v1 = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s v3 = CPU_Pop16();
		Bit16s v4 = CPU_Pop16();

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
		RealPt str = CPU_Pop32();
		CPU_Push32(str);

		D1_LOG("GUI_output()\n");
		GUI_output(Real2Host(str));
		return 1;
	}
	/* Callers: 1 */
	case 0x5b4: {
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
		Bit16s v1 = CPU_Pop16();
		Bit16u v2 = CPU_Pop16();
		Bit16u v3 = CPU_Pop16();
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("GUI_fill_radio_button(%d, %d, %d);\n", v1, v2, v3);

		GUI_fill_radio_button(v1, v2, v3);

		return 1;

	}
	/* Callers: 1 */
	case 0x893:
		return 0;
	/* Callers: 2 */
	case 0xb43: {
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
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg098(unsigned short offs)
{
	switch (offs) {

	/* Callers: 1 */
	case 0x0000 : {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		magic_heal_ani(Real2Host(hero));
		D1_LOG("magic_heal_ani(%s)\n", schick_getCharname(hero));

		return 1;
	}
	/* Callers: 4 */
	case 0x0339 : {
		unsigned short spell = CPU_Pop16();
		unsigned short half_cost = CPU_Pop16();
		CPU_Push16(half_cost);
		CPU_Push16(spell);

		reg_ax = get_spell_cost(spell, half_cost);

		D1_LOG("get_spell_cost(%s, %d) = %d\n",
			names_spell[spell], half_cost, (short)reg_ax);

		return 1;
	}
	case 0x071d : {
		RealPt hero = CPU_Pop32();
		Bit16s spellclass_nr = CPU_Pop16();
		CPU_Push16(spellclass_nr);
		CPU_Push32(hero);

		reg_ax = can_use_spellclass(Real2Host(hero), spellclass_nr);
		D1_LOG("can_use_spellclass(%s, %d) = %d\n",
			schick_getCharname(hero), spellclass_nr, reg_ax);

		return 1;
	}
	/* Callers: 1 */
	case 0x0786 : {
		RealPt hero = CPU_Pop32();
		Bit16s show_vals = CPU_Pop16();
		CPU_Push16(show_vals);
		CPU_Push32(hero);

		reg_ax = select_spell(Real2Host(hero), show_vals);

		D1_LOG("select_spell(%s, %d) = %d\n",
			schick_getCharname(hero), show_vals, reg_ax);
		return 1;
	}
	/* Callers: 1 */
	case 0x0e1f : {
		// Zauberprobe
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
		RealPt hero = CPU_Pop32();
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(a1);
		CPU_Push32(hero);

		reg_ax = use_spell(hero, a1, (Bit8s)a2);
		D1_LOG("use_spell(%s, %d, %d) = %d\n",
			schick_getCharname(hero), a1, a2, (Bit16s)reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg102(unsigned short offs)
{
	switch (offs) {
	case 0x000: {
		Bit16s damage = CPU_Pop16();
		CPU_Push16(damage);

		D1_INFO("MON_do_damage(%d)\n", damage);
		MON_do_damage(damage);

		return 1;
	}
	case 0x18e: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(a1);

		D1_INFO("MON_get_spell_cost(%d, %d) = %d\n", a1, a2, reg_ax);
		reg_ax = MON_get_spell_cost(a1, a2);

		return 1;
	}
	case 0x1cf: {
		RealPt mon = CPU_Pop32();
		Bit16s t1 = CPU_Pop16();
		Bit16s t2 = CPU_Pop16();
		Bit16s t3 = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(t3);
		CPU_Push16(t2);
		CPU_Push16(t1);
		CPU_Push32(mon);

		reg_ax = MON_test_attrib3(Real2Host(mon), t1, t2, t3, (Bit8s)bonus);
		D1_INFO("MON_test_attrib3(%d, %d, %d, %d) = %d\n", t1, t2, t3, bonus, reg_ax);

		return 1;
	}
	case 0x22a: {
		RealPt mon = CPU_Pop32();
		Bit16s mspell_nr = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(mspell_nr);
		CPU_Push32(mon);

		reg_ax = MON_test_skill(Real2Host(mon), mspell_nr, (Bit8s)bonus);
		D1_INFO("MON_test_skill(%d, %d) = %d\n", mspell_nr, bonus, reg_ax);

		return 1;
	}
	case 0x2c2: {
		RealPt mon = CPU_Pop32();
		Bit16s ae = CPU_Pop16();
		CPU_Push16(ae);
		CPU_Push32(mon);

		MON_sub_ae(Real2Host(mon), ae);
		D1_INFO("MON_test_skill(%d)\n", ae);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}


static int n_seg103(unsigned short offs)
{
	switch (offs) {
	case 0x040f: {
		// Talentprobe
		RealPt hero = CPU_Pop32();
		Bit16s skill = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(skill);
		CPU_Push32(hero);

		D1_LOG("Talentprobe: %s %+d\n ",
			names_skill[skill], (signed char)bonus);

		reg_ax = test_skill(Real2Host(hero), skill, (signed char)bonus);

		return 1;
	}
	case 0x0537: {
		reg_ax = select_talent();
		D1_LOG("select_talent() = %s\n", names_skill[reg_ax]);
		return 1;
	}
	case 0x06bf: {
		Bit16s hero = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		Bit16s skill = CPU_Pop16();
		CPU_Push16(skill);
		CPU_Push16(bonus);
		CPU_Push16(hero);

		reg_ax = use_talent(hero, (signed char)bonus, skill);

		D1_LOG("use_talent(): %s %+d\n",
			names_skill[skill], (signed char)bonus);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg104(unsigned short offs)
{
	D1_LOG("%s:0x%x\n", __func__, offs);
	switch (offs) {
		case 0x0000: {
			RealPt hero = CPU_Pop32();
			Bit16s receipe_index = CPU_Pop16();
			reg_ax = hero_has_ingrendients(Real2Host(hero), (Bit8s)receipe_index);
			D1_LOG("hero_has_ingrendients(%s, %d) = %d\n",
				Real2Host(hero) + 0x10, (Bit8s)receipe_index, (Bit16s)reg_ax);
			CPU_Push16(receipe_index);
			CPU_Push32(hero);
			return 1;
		}
		case 0x00b2: {
			RealPt hero = CPU_Pop32();
			Bit16s receipe_index = CPU_Pop16();
			hero_use_ingrendients(Real2Host(hero), (Bit8s)receipe_index);
			D1_LOG("hero_use_ingrendients(%s, %d)\n",
				Real2Host(hero) + 0x10, (Bit8s)receipe_index);
			CPU_Push16(receipe_index);
			CPU_Push32(hero);
			return 1;
		}
		case 0x016d: {
			RealPt hero = CPU_Pop32();
			Bit16s receipe_index = CPU_Pop16();
			Bit16s flag = CPU_Pop16();

			reg_ax = do_alchemy(Real2Host(hero), (Bit8s)receipe_index, flag);
			D1_LOG("do_alchemy(%s, %d, %d) = %d\n",
				Real2Host(hero) + 0x10, (Bit8s)receipe_index, flag, (Bit16s)reg_ax);
			CPU_Push16(flag);
			CPU_Push16(receipe_index);
			CPU_Push32(hero);
			return 1;
		}
		case 0x07e6: {
			RealPt hero = CPU_Pop32();
			Bit16s disease = CPU_Pop16();
			CPU_Push16(disease);
			CPU_Push32(hero);

			reg_ax = has_herb_for_disease(Real2Host(hero), disease);
			D1_LOG("has_herb_for_disease(%s, disease) = %d\n",
				schick_getCharname(hero), disease, reg_ax);
			return 1;
		}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int n_seg105(unsigned offs) {
	switch (offs) {
	case 0x000: {
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
		RealPt hero = CPU_Pop32();
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);
		CPU_Push32(hero);

		reg_ax = has_hero_stacked(Real2Host(hero), item);
		D1_LOG("has_hero_stacked(%s, %s) = %d\n",
			schick_getCharname(hero),
			get_itemname(item), (signed short)reg_ax);

		return 1;
	}
	case 0x3e8: {
		RealPt hero = CPU_Pop32();
		Bit16s item = CPU_Pop16();
		Bit16s v2 = CPU_Pop16();
		Bit16s nr = CPU_Pop16();
		CPU_Push16(nr);
		CPU_Push16(v2);
		CPU_Push16(item);
		CPU_Push32(hero);

		reg_ax = give_hero_new_item(Real2Host(hero), item, v2, nr);
		D1_LOG("near give_hero_new_item(%s, %s, %d, %d); = %d\n",
			(char*)Real2Host(hero) + 0x10,
			get_itemname(item), v2, nr, (signed short)reg_ax);
		return 1;
	}
	case 0x675: {
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);

		reg_ax = item_pleasing_ingerimm(item);
		D1_LOG("item_pleasing_ingerimm(%s); = %d\n",
			get_itemname(item), reg_ax);

		return 1;
	}
	case 0x6d9: {
		RealPt hero = CPU_Pop32();
		signed short pos = CPU_Pop16();
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
		RealPt hero = CPU_Pop32();
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);
		CPU_Push32(hero);

		reg_ax = hero_count_item(Real2Host(hero), item);
		D1_LOG("hero_count_item(%s, %s) = %d\n",
			schick_getCharname(hero),
			get_itemname(item), reg_ax);

		return 1;
	}
	case 0xc10: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);
		reg_ax = select_item_to_drop(Real2Host(hero));
		D1_LOG("near select_item_to_drop(%s) = %d\n",
			schick_getCharname(hero), reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg106(unsigned offs) {
	switch (offs) {
	case 0x00: {
		RealPt hero = CPU_Pop32();
		Bit16s item = CPU_Pop16();
		Bit16s pos = CPU_Pop16();
		CPU_Push16(pos);
		CPU_Push16(item);
		CPU_Push32(hero);
		reg_ax = two_hand_collision(Real2Host(hero), item, pos);

		D1_LOG("two_hand_collision(%s, 0x%x, %d); == %d\n",
			(char*)Real2Host(hero) + 0x10, item, pos, reg_ax);

		return 1;
	}
	case 0x9c: {
		Bit16s pos1 = CPU_Pop16();
		Bit16s pos2 = CPU_Pop16();
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);
		CPU_Push16(pos2);
		CPU_Push16(pos1);

		D1_LOG("near move_item(%d, %d, %s);\n",
			pos1, pos2, (char*)Real2Host(hero) + 0x10);

		move_item(pos1, pos2, Real2Host(hero));

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg113(unsigned offs) {
	switch (offs) {
	case 0x900: {
		RealPt hero = CPU_Pop32();
		Bit16s idx = CPU_Pop16();
		Bit16s arg2 = CPU_Pop16();
		CPU_Push16(arg2);
		CPU_Push16(idx);
		CPU_Push32(hero);
		D1_LOG("hero_disappear(%s, %d, %d);\n",
			Real2Host(hero) + 0x10, idx, (signed char)arg2);

		hero_disappear(Real2Host(hero), idx, (signed char)arg2);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg117(unsigned short offs)
{
	switch (offs) {
	case 0x0000: {
		Bit16s ani_nr = CPU_Pop16();
		CPU_Push16(ani_nr);

		D1_LOG("pause_traveling(%d);\n", ani_nr);
		pause_traveling(ani_nr);

		return 1;
	}
	case 0x006a: {
		D1_LOG("resume_traveling();\n");
		resume_traveling();
		return 1;
	}
	case 0x00a0: {
		D1_LOG("hunt_karen();\n");
		hunt_karen();
		return 1;
	}
	case 0x02a3: {
		D1_LOG("hunt_wildboar();\n");
		hunt_wildboar();
		return 1;
	}
	case 0x047f: {
		D1_LOG("hunt_cavebear();\n");
		hunt_cavebear();
		return 1;
	}

	case 0x060c: {
		D1_LOG("hunt_viper();\n");
		hunt_viper();
		return 1;
	}
	case 0x0981: {
		D1_INFO("do_bison_hunt();\n");
		return 0;
	}
	case 0x0a8c: {
		D1_INFO("do_rhino_attack();\n");
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg120(unsigned short offs)
{
	switch (offs) {
	case 0x578: {
		D1_LOG("init_global_buffer()\n");
		init_global_buffer();
		return 1;
	}
	case 0x99f: {
		D1_LOG("refresh_colors();\n");
		refresh_colors();
		return 1;
	}
	case 0xd85: {
		D1_LOG("near cleanup_game();\n");
		cleanup_game();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
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

			D1_LOG("chdir(%s)\n", (char*)Real2Host(path));

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
			Bit16s val = CPU_Pop16();
			CPU_Push16(val);

			D1_LOG("C-Lib exit(%d)\n", val);
			bc_exit(val);
			return 1;
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
			Bit16s drive = CPU_Pop16();
			RealPt dir = CPU_Pop32();
			CPU_Push32(dir);
			CPU_Push16(drive);

			reg_ax = bc_getcurdir(drive, (char*)Real2Host(dir));
			D1_LOG("getcurdir(%d, %s) = %d\n",
				drive, Real2Host(dir), (Bit16s)reg_ax);
			return 1;
		}
		/* getdisk() */
		case 0x781: {
			reg_ax = bc_getdisk();
			D1_LOG("Drive %c\n", reg_ax + 'A');
			return 1;
		}
		/* setdisk() */
		case 0x79b: {
			Bit16s drive = CPU_Pop16();
			CPU_Push16(drive);
			reg_ax = bc_setdisk(drive);
			D1_LOG("setdisk(%c) = %d\n", drive + 'A', (Bit16s)reg_ax);
			return 1;
		}
		/* dos_getdiskfree() */
		case 0x7ed: {

			Bit16u drive = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(drive);

			bc_dos_getdiskfree(drive, Real2Host(p));

			D1_LOG("disk = %d, %d, %d, %d\n",
				host_readw(Real2Host(p) + 0),
				host_readw(Real2Host(p) + 2),
				host_readw(Real2Host(p) + 4),
				host_readw(Real2Host(p) + 6));

			return 1;
		}
		case 0x816: {
			Bit16u interruptno = CPU_Pop16();

			RealPt p = bc__dos_getvect((Bit8u)interruptno);
			D1_LOG("_dos_getvect(int=0x%x) = %x\n",
				interruptno, p);

			CPU_Push16(interruptno);

			reg_ax = RealOff(p);
			reg_dx = RealSeg(p);

			return 1;
		}
		case 0x825: {
			Bit16u interruptno = CPU_Pop16();
			RealPt isr = CPU_Pop32();

			D1_LOG("_dos_setvect(int=0x%x, *isr=0x%x:0x%x)\n",
				interruptno,
				(unsigned short)(RealSeg(isr) - reloc_game),
				RealOff(isr));

			bc__dos_setvect((Bit8u)interruptno, isr);

			CPU_Push32(isr);
			CPU_Push16(interruptno);
			return 1;
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
			Bit16u seed = CPU_Pop16();
			CPU_Push16(seed);

			D1_LOG("srand(%d)\n", seed);
			bc_srand(seed);
			return 1;
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
			RealPt tp = CPU_Pop32();
			CPU_Push32(tp);

			Bit32s retval = bc_time(tp);

			D1_LOG("C-Lib time(0x%04x) = %d\n", tp, retval);

			reg_ax = (Bit16s)retval;
			reg_dx = (Bit16s)(retval >> 16);

			return 1;
		}
		case 0x117b: {
			signed short c = CPU_Pop16();
			CPU_Push16(c);
			reg_ax = toupper(c);
			D1_LOG("toupper(%c) == %c\n", c, reg_ax);
			return 1;
		}
		/* unlink() or remove() */
		case 0x11a7: {
			RealPt fname = CPU_Pop32();
			reg_ax = bc_unlink(fname);
			D1_LOG("unlink(%s) = %d\n",
				Real2Host(fname), reg_ax);
			CPU_Push32(fname);
			return 1;
		}
		case 0x176d: {
			Bit16s cmd = CPU_Pop16();

			reg_ax = bc_bioskey(cmd);
			D1_LOG("bioskey(%d);\n", (char) cmd);
			CPU_Push16(cmd);

			return 1;
		}
		case 0x1792: {
			D1_LOG("clrscr();\n");
			bc_clrscr();
			return 1;
		}
		case 0x1e55: {
			RealPt ptr = CPU_Pop32();

			D1_LOG("farfree(0x%08x)\n", ptr);
			bc_farfree(ptr);
			CPU_Push32(ptr);

			return 1;
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
			Bit32u mem = bc_farcoreleft();
			D1_LOG("farcoreleft() = %u\n", mem);

			reg_ax = mem & 0xffff;
			reg_dx = mem >> 16;
			return 1;
		}
		case 0x2411: {
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			D1_LOG("harderr(%x)\n", p);
			bc_harderr(p);
			return 1;
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
			D1_LOG("C-Lib bc__creat(%s, %x);\n",
				(char*)Real2Host(name), attrib);
			reg_ax = bc__creat(name, attrib);
			CPU_Push16(attrib);
			CPU_Push32(name);
			return 1;
		}
		case 0x3040: {
			RealPt path = CPU_Pop32();
			RealPt ffblk = CPU_Pop32();
			Bit16s attrib = CPU_Pop16();
			reg_ax = bc_findfirst_dosbox(path, ffblk, attrib);
			D1_LOG("findfirst(%s, %x, %d) = %d\n",
				(char*)Real2Host(path), ffblk, attrib, reg_ax);
			CPU_Push16(attrib);
			CPU_Push32(ffblk);
			CPU_Push32(path);
			return 1;
		}
		case 0x3073: {
			RealPt ffblk = CPU_Pop32();
			reg_ax = bc_findnext_dosbox(ffblk);
			D1_LOG("findnext(%x) = %d\n", ffblk, reg_ax);
			CPU_Push32(ffblk);
			return 1;
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
			sprintf((char*)Real2Host(string), "%d", value);

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

			memset(Real2Host(s), c, n);

			reg_ax = RealOff(s);
			reg_dx = RealSeg(s);
			return 1;
		}
		case 0x3479: {
			/*  memmove() */
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			Bit16u len = CPU_Pop16();
			CPU_Push16(len);
			CPU_Push32(src);
			CPU_Push32(dst);

			D1_LOG("bc_memmove(0x%x, 0x%x, %d)\n", dst, src, len);
			bc_memmove(dst, src, len);
			reg_ax = RealOff(dst);
			reg_dx = RealSeg(dst);
			return 1;
		}
		case 0x34c7: {
			/*open()*/
			RealPt fname = CPU_Pop32();
			Bit16u mode = CPU_Pop16();

			reg_ax = bc__open(fname, mode);
			D1_LOG("_open(\"%s\",\"%04x\") = %d\n",
					(char*)Real2Host(fname),
					mode,
					(Bit16s)reg_ax);

			CPU_Push16(mode);
			CPU_Push32(fname);
			return 1;
		}
		case 0x3636: {
			/* sortof open() */
			RealPt fname = CPU_Pop32();
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);
			CPU_Push32(fname);

			D1_LOG("C-Lib Unkn(\"%s\", 0x%04x)\n",
					(char*)Real2Host(fname), mode);
			return 0;
		}
		case 0x36dd: {
			/*printf()*/
			RealPt p = CPU_Pop32();
			CPU_Push32(p);

			D1_LOG("printf(\"%s\")\n", (char*)Real2Host(p));
			return 0;
		}
		case 0x3d74: {
			return 0;
			RealPt base = CPU_Pop32();
			Bit16u nmemb = CPU_Pop16();
			Bit16u size = CPU_Pop16();
			RealPt compar = CPU_Pop32();

			D1_LOG("qsort(0x%x, 0x%x, 0x%x, 0x%x)\n",
				base, nmemb, size, compar);
			bc_qsort(base, nmemb, size, compar);
			CPU_Push32(compar);
			CPU_Push16(size);
			CPU_Push16(nmemb);
			CPU_Push32(base);
			return 1;
		}
		/* ret 0x000a */
		case 0x41d2: {
			RealPt str = CPU_Pop32();
			RealPt format = CPU_Pop32();
			CPU_Push32(format);
			CPU_Push32(str);

			D1_LOG("C-Lib sprintf(0x%04x:0x%04x, \"%s\", ...)\n",
					RealSeg(str), RealOff(str),
					(char*)Real2Host(format));

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

			strcat((char*)Real2Host(dest), (char*)Real2Host(src));

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

			reg_ax = strcmp((char*)Real2Host(s1),
					(char*)Real2Host(s2));

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

			strcpy((char*)Real2Host(dest), (char*)Real2Host(src));

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

			strncpy((char*)Real2Host(dest),
				(char*)Real2Host(src), n);

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
		D1_LOG("CD_audio_stop()\n");
		CD_audio_stop();
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
		unsigned short index = CPU_Pop16();
		CPU_Push16(index);

		D1_LOG("set_audio_track(%d);\n", index);
		set_audio_track(index);
		return 1;
	}
	case 0x0045: {
		D1_LOG("sound_menu()\n");
		sound_menu();
		return 1;
	}
	case 0x00e6: {
		D1_LOG("read_sound_cfg()\n");
		read_sound_cfg();
		return 1;
	}
	case 0x01e0: {
		Bit32u size = CPU_Pop32();
		D1_LOG("far init_AIL(%d)\n", size);
		init_AIL(size);
		CPU_Push32(size);
		return 1;
	}
	case 0x0209: {
		D1_LOG("far exit_AIL()\n");
		exit_AIL();
		return 1;
	}
	case 0x06fe: {
		D1_LOG("start_midi_playback_IRQ()");
		start_midi_playback_IRQ();
		return 1;
	}
	case 0x079f: {
		reg_ax = have_mem_for_sound();
		D1_LOG("have_mem_for_sound() = %d\n", reg_ax);
		return 1;
	}
	case 0x0832: {
		Bit16u index = CPU_Pop16();
		D1_LOG("play_voc(FX%d.VOC)\n", index - 288);
		play_voc(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0856: {
		Bit16u index = CPU_Pop16();
		D1_LOG("play_voc_delay(FX%d.VOC)\n", index - 288);
		play_voc_delay(index);
		CPU_Push16(index);
		return 1;
	}
	case 0x0890: {
		Bit32u size = CPU_Pop32();
		alloc_voc_buffer(size);
		D1_LOG("alloc_voc_buffer(%d)\n", size);
		CPU_Push32(size);
		return 1;
	}
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
		Bit16u dummy = CPU_Pop16();
		CPU_Push16(dummy);
		CPU_Push32(off);
		CPU_Push16(handle);

		D1_LOG("seg002_0c72(%d, %d, %d)\n", handle, off, dummy);
		seg002_0c72(handle, off, dummy);

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
	case 0x0e57: {
		RealPt src_file = CPU_Pop32();
		RealPt fname = CPU_Pop32();

		D1_LOG("copy_file_to_temp(%s, %s);\n",
			Real2Host(src_file), Real2Host(fname));
		copy_file_to_temp(src_file, fname);
		CPU_Push32(fname);
		CPU_Push32(src_file);
		return 1;
	}
	case 0x0ed2: {
			RealPt nvf_rp = CPU_Pop32();
			CPU_Push32(nvf_rp);

			Bit32s retval;

			Bit8u *nvf_p = Real2Host(nvf_rp);

			struct nvf_desc nvf;

			nvf.dst = Real2Host(host_readd(nvf_p + 0));
			nvf.src = Real2Host(host_readd(nvf_p + 4));
			nvf.nr = host_readw(nvf_p + 8);
			nvf.type = host_readb(nvf_p + 10);
			nvf.width = Real2Host(host_readd(nvf_p + 11));
			nvf.height = Real2Host(host_readd(nvf_p + 15));

			D1_LOG("process_nvf(0x%04x:0x%04x);\n",
				RealSeg(nvf_rp), RealOff(nvf_rp));

			retval = process_nvf(&nvf);

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
	case 0x165e: {
		return 0;
	}
	case 0x16fd: {
		D1_LOG("disable_mouse()\n");
		disable_mouse();
		return 1;
	}
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
	case 0x1a34: {
		D1_LOG("handle_gui_input();\n");
		handle_gui_input();
		return 1;
	}
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
		pal_fade(Real2Host(p1), Real2Host(p2));
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

		pal_fade_in(Real2Host(dst), Real2Host(p2), v3, colors);

		return 1;
	}
	case 0x25ce: {
		reg_ax = get_current_season();
		return 1;
		}
	case 0x274e: {
		D1_LOG("far do_timers();\n");
		do_timers();
		return 1;
	}
	case 0x2bf6: {
		Bit32s val = CPU_Pop32();
		CPU_Push32(val);

		D1_LOG("far sub_ingame_timers(val = %u);\n", val);
		sub_ingame_timers(val);
		return 1;
	}
	case 0x2c5e: {
		Bit32s val = CPU_Pop32();
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
		Bit32s v1 = CPU_Pop32();
		CPU_Push32(v1);

		D1_LOG("seg002_2f7a(v1=%d);\n", v1);
		seg002_2f7a(v1);

		return 1;
	}
	case 0x3071: {
		Bit32s quarter = CPU_Pop32();
		CPU_Push32(quarter);

		D1_LOG("sub_light_timers(quarter=%d);\n", quarter);
		sub_light_timers(quarter);

		return 1;
	}
	case 0x3230: {
		D1_LOG("herokeeping();\n");
		herokeeping();
		return 1;
	}
	case 0x37c4: {
		D1_LOG("seg002_37c4();\n");
		seg002_37c4();
		return 1;
	}
	case 0x3b4f:  {
		D1_LOG("set_and_spin_lock()\n");
		set_and_spin_lock();
		return 1;
	}
	case 0x3ca6: {
		Bit32s time = CPU_Pop32();
		CPU_Push32(time);
		D1_LOG("timewarp(0x%x);\n", time);
		timewarp(time);
		return 1;
	}
	case 0x3dbb: {
		Bit32s time = CPU_Pop32();
		CPU_Push32(time);
		D1_LOG("timewarp_until(0x%x);\n", time);
		timewarp_until(time);
		return 1;
	}
	case 0x3ebb: {
		D1_LOG("dec_splash();\n");
		dec_splash();
		return 1;
	}
	case 0x3f9a: {
		D1_LOG("timewarp_until_midnight();\n");
		timewarp_until_midnight();
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

		Bit32u retval;

		retval = swap_u32(v);

		reg_ax = retval & 0xffff;
		reg_dx = (retval >> 16) & 0xffff;
		return 1;
	}
	/* 3 EMS functions (all disabled in v3.02_de) */
	case 0x4253: {
		Bit32u bytes = CPU_Pop32();
		CPU_Push32(bytes);

		reg_ax = alloc_EMS(bytes);
		D1_LOG("alloc_EMS(%d) = %d\n", bytes, reg_ax);

		return 1;
	}
	case 0x428b: {
		RealPt dst = CPU_Pop32();
		Bit16u handle = CPU_Pop16();
		Bit32u bytes = CPU_Pop32();
		CPU_Push32(bytes);
		CPU_Push16(handle);
		CPU_Push32(dst);

		D1_LOG("from_EMS(0x%x, 0x%x, 0x%x);\n", dst, handle, bytes);
		from_EMS(dst, handle, bytes);
		return 1;
	}
	case 0x4339: {
		Bit16u handle = CPU_Pop16();
		RealPt src = CPU_Pop32();
		Bit32u bytes = CPU_Pop32();
		CPU_Push32(bytes);
		CPU_Push32(src);
		CPU_Push16(handle);

		D1_LOG("to_EMS(0x%x, 0x%x, 0x%x);\n", handle, src, bytes);
		to_EMS(handle, src, bytes);
		return 1;
	}
	case 0x43e7: {
		D1_LOG("set_to_ff()\n");
		set_to_ff();
		return 1;
	}
	case 0x43fd: {
		// va_arg function are not bypassed this way
		D1_LOG("draw_loc_icons(...);\n");
		//draw_loc_icons(9, 10, 11, 21, 31, 41, 51, 61, 71, 81);
		return 0;
	}
	case 0x4485: {
		Bit16s val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = mod_day_timer(val);
		D1_LOG("mod_day_timer(%d) = %d\n", val, reg_eax);
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
	case 0x4adc: {
		RealPt hero = CPU_Pop32();
		Bit16s le = CPU_Pop16();
		CPU_Push16(le);
		CPU_Push32(hero);

		D1_LOG("far sub_hero_le(%s, %d);\n",
			(char*)Real2Host(hero) + 0x10, le);

		sub_hero_le(Real2Host(hero), le);

		return 1;
	}
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
		Bit32s money;

		money = get_party_money();
		D1_INFO("Aktuelles Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);

		reg_ax = money & 0xffff;
		reg_dx = (money>>16) & 0xffff;

		return 1;
	}
	case 0x5221: {
		Bit32s money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Setze Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);
		set_party_money(money);

		return 1;
	}
	case 0x5331: {
		Bit32s money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Aendere Gruppenvermoegen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);
		add_party_money(money);
		return 1;
	}
	case 0x5349: {
		RealPt hero = CPU_Pop32();
		Bit32s ap = CPU_Pop32();
		CPU_Push32(ap);
		CPU_Push32(hero);

		D1_INFO("%s erhaelt %d AP\n",
					schick_getCharname(hero), ap);
		add_hero_ap(Real2Host(hero), ap);

		return 1;
	}
	case 0x535f: {
		/* APs verteilen */
		Bit32s group_ap = CPU_Pop32();
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

		reg_ax = get_hero_index(Real2Host(hero));
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
		Bit16s item = CPU_Pop16();
		CPU_Push16(item);

		reg_ax = get_first_hero_with_item(item);
		D1_LOG("get_first_hero_with_item(%s = (%d)) = %d\n",
			get_itemname(item), item, (signed short)reg_ax);

		return 1;
	}
	case 0x5615: {
		Bit16s le = CPU_Pop16();
		CPU_Push16(le);

		D1_LOG("sub_group_le(%d);\n", le);
		sub_group_le(le);

		return 1;
	}
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
	case 0x57f1: {
		D1_LOG("seg002_57f1()\n");
		seg002_57f1();
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
			D1_LOG("move()\n");
			move();
			return 1;
		}
		case  0x01e1: {
			Bit16s nr = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			Bit16s y = CPU_Pop16();
			Bit16s frame = CPU_Pop16();

			D1_LOG("door_frame(%d, %d, %d, %d);\n",
				nr, x, y, frame);
			door_frame(nr, x, y, frame);

			CPU_Push16(frame);
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push16(nr);
			return 1;
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
	case 0x05: {
		D1_LOG("save_and_set_timer()\n");
		save_and_set_timer();
		return 1;
	}
	case 0x29: {
		D1_LOG("set_timer()\n");
		set_timer();
		return 1;
	}
	case 0x3e: {
		D1_LOG("reset_timer()\n");
		reset_timer();
		return 1;
	}
	case 0x55: {
		Bit16u v1 = CPU_Pop16();

		D1_LOG("init_ani(%d)\n", v1);
		init_ani(v1);

		CPU_Push16(v1);
		return 1;
	}
	case 0x134: {
		D1_LOG("set_var_to_zero()\n");
		set_var_to_zero();
		return 1;
	}
	case 0x13f: {
		Bit16u v1 = CPU_Pop16();
		CPU_Push16(v1);
		D1_LOG("init_ani_busy_loop(%d)\n", v1);
		init_ani_busy_loop(v1);
		return 1;
	}
	case 0x15b: {
		D1_LOG("clear_ani()\n");
		clear_ani();
		return 1;
	}
	case 0x244: {
		return 0;
	}
	case 0x54b: {
		D1_LOG("far update_status_bars()\n");
		update_status_bars();
		return 1;
	}
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
		restore_rect(ptr1, Real2Host(ptr2), x, y,
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

		restore_rect_rle(ptr1, Real2Host(ptr2),
			x, y, (char)w, (char)h, v1);
		return 1;
	}
	case 0xb6b: {
		D1_LOG("draw_mouse_cursor()\n");
		draw_mouse_cursor();
		return 1;
	}
	case 0xc26: {
		D1_LOG("save_mouse_bg()\n");
		save_mouse_bg();
		return 1;
	}
	case 0xcb1: {
		D1_LOG("restore_mouse_bg()\n");
		restore_mouse_bg();
		return 1;
	}
	case 0xd3c: {
		D1_LOG("load_objects_nvf()\n");
		load_objects_nvf();
		return 1;
	}
	case 0xf54: {
		return 0;
	}
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
		array_add(Real2Host(dst), v1, (char)v2, v3);
		return 1;
	}
	case 0x1190: {
		D1_LOG("schick_set_video()\n");
		schick_set_video();
		return 1;
	}
	case 0x11c1: {
		D1_LOG("schick_reset_video()\n");
		schick_reset_video();
		return 1;
	}
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
	case 0x1291: {
		return 0;
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
		do_v_line(dst, x, y1, y2, (unsigned char)color);
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
		do_border((RealPt)dst, x1, x2, y1, y2, (unsigned char)color);
		return 1;
	}
	case 0x13b7: {
		unsigned short mode = CPU_Pop16();
		CPU_Push16(mode);
		D1_LOG("do_pic_copy(%d);\n", mode);
		do_pic_copy(mode);
		return 1;
	}
	case 0x144c: {
		D1_LOG("do_save_rect()\n");
		do_save_rect();
		return 1;
	}
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
	case 0x1557: {
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		D1_LOG("map_effect(%x)\n", p);
		map_effect(Real2Host(p));
		return 1;
	}

	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg005(unsigned short offs) {
	/* seg005 has only one func called by far */
	switch (offs) {
		case 0x598: {

			Bit16u val = CPU_Pop16();
			D1_LOG("draw_fight_screen(%d);\n", val);
			CPU_Push16(val);

			draw_fight_screen(val);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
		case 0x07f: {
			D1_LOG("FIG_draw_figures()\n");
			FIG_draw_figures();
			return 1;
		}
		case 0x236:
			D1_LOG("FIG_set_gfx()\n");
			FIG_set_gfx();
			return 1;
		case 0x29e: {
			D1_LOG("FIG_call_draw_pic()\n");
			FIG_call_draw_pic();
			return 1;
		}
		case 0x2a8: {
			D1_LOG("FIG_draw_pic();\n");
			FIG_draw_pic();
			return 1;
		}
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
			FIG_draw_enemy_pic(v1, v2);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg007(unsigned short offs)
{
	switch (offs) {

	case 0x000b: {
		Bit32s lo = CPU_Pop16();
		Bit32s hi = CPU_Pop16();
		CPU_Push16(hi);
		CPU_Push16(lo);

		reg_ax = random_interval(lo, hi);

		D1_INFO("randomInterval %d - %d : %d\n", lo, hi, (Bit16u)reg_ax);

		return 1;
	}
	case 0x002b: {
		Bit32s p1 = CPU_Pop16();
		CPU_Push16(p1);

		reg_ax = random_schick(p1);

		D1_INFO("random(%d) = %d\n", p1, reg_ax);

		return 1;
	}
	case 0x007a: {
		Bit32s n = CPU_Pop16();
		Bit32s m = CPU_Pop16();
		Bit32s x = CPU_Pop16();
		CPU_Push16(x);
		CPU_Push16(m);
		CPU_Push16(n);

		reg_ax = dice_roll(n, m, x);

		D1_INFO("wuerfel %dW%d%+d = %d\n", n, m, x, reg_ax);

		return 1;
	}
	case 0x00c4: {
		unsigned short val = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(val);

		reg_ax = is_in_word_array(val, (signed short*)Real2Host(p));

		D1_LOG("is_in_word_array(0x%x, 0x%04x:0x%04x) = %d\n",
			val, RealSeg(p), RealOff(p), reg_ax);

		return 1;
	}
	case 0x00ef: {
		Bit32s val = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(val);

		reg_ax = is_in_byte_array((char)val, Real2Host(p));
		D1_LOG("is_in_byte_array(0x%x, 0x%04x:0x%04x) = %d\n",
			(signed char)val, RealSeg(p), RealOff(p), reg_ax);

		return 1;
	}
	case 0x00a0: {
		Bit32s n = CPU_Pop16();
		Bit32s m = CPU_Pop16();
		Bit32s x = CPU_Pop16();
		RealPt pmin = CPU_Pop32();
		RealPt pmax = CPU_Pop32();
		CPU_Push32(pmax);
		CPU_Push32(pmin);
		CPU_Push16(x);
		CPU_Push16(m);
		CPU_Push16(n);

		calc_damage_range(n, m, x, Real2Host(pmin), Real2Host(pmax));

		D1_LOG("calc_damage_range(%d, %d, %d)\n", n, m, x);

		return 1;
	}
	case 0x0119: {
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
	case 0x0186: {
		unsigned short val = CPU_Pop16();
		RealPt min = CPU_Pop32();
		RealPt max = CPU_Pop32();
		CPU_Push32(max);
		CPU_Push32(min);
		CPU_Push16(val);

		damage_range_template(val, Real2Host(min), Real2Host(max));

		D1_LOG("damage_range_template() Untested\n");

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
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

		D1_GFX("save_display_stat(dstat=0x%x:0x%x);\n",
			RealSeg(addr), RealOff(addr));

		save_display_stat(addr);

		return 1;
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

		draw_h_line(rptr, cnt, (unsigned char)color);

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

		draw_h_spaced_dots(rptr, cnt,
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

		pic_copy(dst, x1, y1, x2, y2, val12, val14,
			val16, val18, width, height,
			Real2Host(src), mode);

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

		save_rect(RealSeg(rptr), RealOff(rptr), dst, width, height);

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

		fill_rect(RealSeg(rptr), RealOff(rptr), color, width, height);

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

		D1_GFX("decomp_rle(width=%u, height=%u, dst=0x%x:0x%x, src=0x%x:0x%x, tmp_buffer=0x%x:0x%x, mode=%u)\n",
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg011(unsigned short offs) {
	switch (offs) {
	case 0x6d1: {
		D1_LOG("AIL_startup();\n");
		AIL_startup();
		return 1;
	}
	case 0x715: {
		RealPt signoff_msg = CPU_Pop32();
		D1_LOG("AIL_shutdown();\n");
		AIL_shutdown(signoff_msg);
		CPU_Push32(signoff_msg);
		return 1;
	}
	case 0xa1e: {
		RealPt ptr = CPU_Pop32();
		reg_ax = AIL_register_driver(ptr);
		D1_LOG("AIL_register_driver(%x); = %d\n", ptr, (signed short)reg_ax);
		CPU_Push32(ptr);
		return 1;
	}
	case 0xad6: {
		Bit16u driver = CPU_Pop16();
		RealPt ret = AIL_describe_driver(driver);
		D1_LOG("AIL_describe_driver(); = %x\n", ret);
		CPU_Push16(driver);
		reg_ax = RealOff(ret);
		reg_dx = RealSeg(ret);
		return 1;
	}
	case 0xafc: {
		Bit16u driver = CPU_Pop16();
		Bit16u io = CPU_Pop16();
		Bit16u irq = CPU_Pop16();
		Bit16u dma = CPU_Pop16();
		Bit16u drq = CPU_Pop16();
		reg_ax  = AIL_detect_device(driver, io, irq, dma, drq);
		D1_LOG("AIL_detect_device(%d, 0x%x, 0x%x, %x, %x); = %x\n",
			driver, io, irq, dma, drq, reg_ax);
		CPU_Push16(drq);
		CPU_Push16(dma);
		CPU_Push16(irq);
		CPU_Push16(io);
		CPU_Push16(driver);
		return 1;
	}
	case 0xb02: {
		Bit16u driver = CPU_Pop16();
		Bit16u io = CPU_Pop16();
		Bit16u irq = CPU_Pop16();
		Bit16u dma = CPU_Pop16();
		Bit16u drq = CPU_Pop16();
		AIL_init_driver(driver, io, irq, dma, drq);
		D1_LOG("AIL_init_driver(%d, 0x%x, 0x%x, %x, %x); = %x\n",
			driver, io, irq, dma, drq, reg_ax);
		CPU_Push16(drq);
		CPU_Push16(dma);
		CPU_Push16(irq);
		CPU_Push16(io);
		CPU_Push16(driver);
		return 1;
	}
	case 0xbff: {
		Bit16u driver = CPU_Pop16();
		RealPt VOC_file = CPU_Pop32();
		Bit16s block_marker = CPU_Pop16();
		AIL_play_VOC_file(driver, VOC_file, block_marker);
		D1_LOG("AIL_play_VOC_file(%d, 0x%x, %d);\n",
			driver, VOC_file, block_marker);
		CPU_Push16(block_marker);
		CPU_Push32(VOC_file);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc05: {
		Bit16u driver = CPU_Pop16();
		RealPt VOC_file = CPU_Pop32();
		Bit16s block_marker = CPU_Pop16();
		reg_ax = AIL_format_VOC_file(driver, VOC_file, block_marker);
		D1_LOG("AIL_format_VOC_file(%d, 0x%x, %d); = %d\n",
			driver, VOC_file, block_marker, (signed short)reg_ax);
		CPU_Push16(block_marker);
		CPU_Push32(VOC_file);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc0b: {
		Bit16u driver = CPU_Pop16();
		reg_ax = AIL_VOC_playback_status(driver);
		D1_LOG("AIL_VOC_playback_status(); = %d\n", reg_ax);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc11: {
		Bit16u driver = CPU_Pop16();
		D1_LOG("AIL_start_digital_playback();\n");
		AIL_start_digital_playback(driver);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc17: {
		Bit16u driver = CPU_Pop16();
		D1_LOG("AIL_stop_digital_playback();\n");
		AIL_stop_digital_playback(driver);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc29: {
		Bit16u driver = CPU_Pop16();
		Bit16u percent = CPU_Pop16();
		D1_LOG("AIL_set_digital_playback_volume(%d, %d);\n",
			driver, percent);
		AIL_set_digital_playback_volume(driver, percent);
		CPU_Push16(percent);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc41: {
		Bit16u driver = CPU_Pop16();
		reg_ax = AIL_state_table_size(driver);
		D1_LOG("AIL_state_table_size(%d); = %d\n",
			driver, reg_ax);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc53: {
		Bit16u driver = CPU_Pop16();
		reg_ax = AIL_default_timbre_cache_size(driver);
		D1_LOG("AIL_default_timbre_cache_size(%d); = %d\n",
			driver, reg_ax);
		CPU_Push16(driver);
		return 1;
	}
	case 0xc7d: {
		Bit16u sequence = CPU_Pop16();
		Bit16u driver = CPU_Pop16();
		D1_INFO("AIL_start_sequence(0x%x, 0x%x);\n",
			driver, sequence);
		AIL_start_sequence(driver, sequence);
		CPU_Push16(driver);
		CPU_Push16(sequence);
		return 1;
	}
	case 0xc83: {
		Bit16u sequence = CPU_Pop16();
		Bit16u driver = CPU_Pop16();
		D1_INFO("AIL_stop_sequence(0x%x, 0x%x);\n",
			driver, sequence);
		AIL_stop_sequence(driver, sequence);
		CPU_Push16(driver);
		CPU_Push16(sequence);
		return 1;
	}
	case 0xc8f: {
		Bit16u sequence = CPU_Pop16();
		Bit16u driver = CPU_Pop16();
		reg_ax = AIL_sequence_status(driver, sequence);
		D1_INFO("AIL_sequence_status(0x%x, 0x%x) = %d;\n",
			driver, sequence, reg_ax);
		CPU_Push16(driver);
		CPU_Push16(sequence);
		return 1;
	}
	case 0xca1: {
		Bit16u ms = CPU_Pop16();
		Bit16u percent = CPU_Pop16();
		Bit16u sequence = CPU_Pop16();
		Bit16u driver = CPU_Pop16();
		AIL_set_relative_volume(driver, sequence, percent, ms);
		D1_INFO("AIL_set_relative_volume(0x%x, 0x%x, %d, %d);\n",
			driver, sequence, percent, ms);
		CPU_Push16(driver);
		CPU_Push16(sequence);
		CPU_Push16(percent);
		CPU_Push16(ms);
		return 1;
	}
	default:
		return 0;
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
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
		case 0x195: {
			D1_LOG("seg12_195();\n");
			return 0;
		}
		case 0x8ed: {
			Bit16u v1 = CPU_Pop16();
			Bit16u v2 = CPU_Pop16();
			Bit16u v3 = CPU_Pop16();
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			reg_ax = bc_OvrInitEms(v1, v2, v3);
			D1_LOG("_OvrInitEms(%u, %u, %u) = %d;\n",
				v1, v2, v3, (Bit16s)reg_ax);

			return 1;
		}
		case 0xd29: {
			D1_LOG("seg12_d29();\n");
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
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
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg025(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		return 0;
	}
	case 0x25: {
		D1_LOG("turnaround();\n");
		turnaround();
		return 1;
	}
	case 0x2a: {
		D1_LOG("game_options();\n");
		reg_ax = game_options();
		return 1;
	}
	case 0x2f: {
		D1_LOG("far copy_palette();\n");
		copy_palette();
		return 1;
	}
	case 0x39: {
		D1_LOG("show_entrance()\n");
		show_entrance();
		return 1;
	}
	case 0x3e: {
		D1_LOG("show_citizen();\n");
		show_citizen();
		return 1;
	}
	case 0x48: {
		D1_LOG("do_house();\n");
		do_house();
		return 1;
	}
	case 0x4d: {
		return 0;
	}
	case 0x52: {
		D1_LOG("enter_map();\n");
		enter_map();
		return 1;
	}
	case 0x57: {
		D1_LOG("show_treasure_map();\n");
		show_treasure_map();
		return 1;
	}
	case 0x5c: {
		D1_LOG("leave_dungeon()\n");
		leave_dungeon();
		return 1;
	}
	case 0x61: {
		D1_LOG("tumult();\n");
		tumult();
		return 1;
	}
	case 0x66: {
		D1_LOG("fade_into();\n");
		fade_into();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg026(unsigned short offs) {
	switch (offs) {

	case 0x20: {
		D1_LOG("init_text()\n");
		init_text();
		return 1;
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
		reg_ax = load_game_state();
		D1_LOG("load_game_state() = %d\n", (Bit16s)reg_ax);
		return 1;
	}
	case 0x34: {
		reg_ax = save_game_state();
		D1_LOG("save_game_state() = %d\n", (Bit16s)reg_ax);
		return 1;
	}
	case 0x39: {
		RealPt str = CPU_Pop32();
		Bit16s hero_pos = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(hero_pos);
		CPU_Push32(str);

		reg_ax = read_chr_temp(str, hero_pos, a2);
		D1_LOG("read_chr_temp(%s, %d, %d) = %d\n",
			(char*)Real2Host(str),
			hero_pos,
			a2, reg_ax);
		return 1;
	}
	case 0x3e: {
		Bit16u hero = CPU_Pop16();
		CPU_Push16(hero);
		D1_LOG("write_chr_temp(%d)\n", hero);
		write_chr_temp(hero);
		return 1;
	}
	case 0x43: {
		RealPt ptr = CPU_Pop32();
		Bit16s flag = CPU_Pop16();
		CPU_Push16(flag);
		CPU_Push32(ptr);

		reg_ax = copy_chr_names(Real2Host(ptr), flag);
		D1_LOG("copy_chr_names(0x%x, %d) = %d\n", ptr, flag, (Bit16s)reg_ax);
		return 1;
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
		Bit16u nr = CPU_Pop16();
		CPU_Push16(nr);
		D1_LOG("load_tempicon(%d);\n", nr);
		load_tempicon(nr);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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

			D1_LOG("load_ani(0x%02x);\n", ani);
			load_ani(ani);
			return 1;
		}
		case 0x2a: {
			RealPt ret;
			short fig = CPU_Pop16();
			CPU_Push16(fig);

			ret = load_fight_figs(fig);
			D1_LOG("load_fight_figs(%d) = 0x%x\n", fig, ret);
			reg_ax = RealOff(ret);
			reg_dx = RealSeg(ret);
			return 1;
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
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("load_area_description(%d);\n", arg);
		load_area_description(arg);
		return 1;
	}
	case 0x2f: {
		return 0;
	}
	case 0x34: {
		return 0;
	}
	case 0x39: {
		D1_LOG("load_map();\n");
		load_map();
		return 1;
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
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("load_special_textures(%x);\n", arg);
		load_special_textures(arg);
		return 1;
	}
	case 0x52: {
		D1_LOG("call_load_buffer();\n");
		call_load_buffer();
		return 1;
	}
	case 0x57: {
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("seg028_0555(%d);\n", arg);
		seg028_0555(arg);
		return 1;
	}
	case 0x5c: {
		Bit16s arg = CPU_Pop16();
		CPU_Push16(arg);
		D1_LOG("call_load_area(%d);\n", arg);
		call_load_area(arg);
		return 1;
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
		D1_LOG("load_splashes();\n");
		load_splashes();
		return 1;
	}
	case 0x70: {
		Bit16s index = CPU_Pop16();
		CPU_Push16(index);
		D1_LOG("load_informer_tlk(%0x);\n", index);
		load_informer_tlk(index);
		return 1;
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
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int n_seg030(unsigned);

static int seg030(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			return 0;
		}
		case 0x2a: {
			D1_LOG("prepare_date_str();\n");
			prepare_date_str();
			return 1;
		}
		case 0x2f: return n_seg030(0xfd5); break;
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg031(unsigned short offs) {
	switch (offs) {
	case 0x20: {

		RealPt retval;

		retval = load_current_town_gossip();
		D1_LOG("far load_current_town_gossip() = 0x%x\n", retval);

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
		case 0x25: {
			return 0;
		}
	case 0x2f: {
		reg_ax = get_town_lookup_entry();
		D1_LOG("far get_town_lookup_entry(); = %d\n", reg_ax);
		return 1;
	}
	case 0x34: {

		RealPt retval;

		retval = get_informer_hint();
		D1_LOG("get_informer_hint() = 0x%x\n", retval);

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	case 0x39: {

		RealPt retval;

		retval = get_informer_name();
		D1_LOG("get_informer_name() = %s\n", Real2Host(retval));

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
	case 0x3e: {
		RealPt retval;

		retval = get_informer_name2();
		D1_LOG("get_informer_name2() = %s\n", Real2Host(retval));

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);

		return 1;
	}
		case 0x43: {
			return 0;
		}
	case 0x48: {
		Bit16s amount = CPU_Pop16();
		CPU_Push16(amount);

		D1_LOG("drink_while_drinking(%d);\n", amount);
		drink_while_drinking(amount);
		return 1;
	}
	case 0x4d: {
		Bit16s amount = CPU_Pop16();
		CPU_Push16(amount);

		D1_LOG("eat_while_drinking(%d);\n", amount);
		eat_while_drinking(amount);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg032(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("Kampf 0x%02x\n", fight_id);
			reg_ax = do_fight(fight_id);
			return 1;
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
			Bit16s mode = CPU_Pop16();
			D1_LOG("draw_fight_screen_pal(%d)\n", mode);
			draw_fight_screen_pal(mode);
			CPU_Push16(mode);
			return 1;
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg033(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			RealPt hero = CPU_Pop32();
			Bit16s hero_pos = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			Bit16s y = CPU_Pop16();
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push16(hero_pos);
			CPU_Push32(hero);

			D1_LOG("FIG_menu()\n");
			FIG_menu(Real2Host(hero), hero_pos, x, y) ;

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg034(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		RealPt hero = CPU_Pop32();
		Bit16s hero_pos = CPU_Pop16();
		RealPt px = CPU_Pop32();
		RealPt py = CPU_Pop32();
		CPU_Push32(py);
		CPU_Push32(px);
		CPU_Push16(hero_pos);
		CPU_Push32(hero);

		D1_LOG("FIG_move_hero(%s, %d)\n", schick_getCharname(hero), hero_pos);
		FIG_move_hero(Real2Host(hero), hero_pos, Real2Host(px), Real2Host(py));

		return 1;
	}
	case 0x25: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		Bit16s a4 = CPU_Pop16();
		Bit16s a5 = CPU_Pop16();
		Bit16s a6 = CPU_Pop16();
		Bit16s a7 = CPU_Pop16();
		CPU_Push16(a7);
		CPU_Push16(a6);
		CPU_Push16(a5);
		CPU_Push16(a4);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);

		reg_ax = seg034_000(a1, a2, a3, a4, a5, a6, a7);
		D1_LOG("seg034_000(%d, %d, %d, %d, %d, %d, %d) = %d\n",
			a1, a2, a3, a4, a5, a6, a7, (Bit16s)reg_ax);
		return 1;
	}
	case 0x2a: {
		RealPt a1 = CPU_Pop32();
		RealPt a2 = CPU_Pop32();
		Bit16s a3 = CPU_Pop16();
		CPU_Push16(a3);
		CPU_Push32(a2);
		CPU_Push32(a1);

		reg_ax = (Bit16s)seg034_2e3(Real2Host(a1), Real2Host(a2), a3);

		D1_LOG("seg034_2e3(*a1 = %d, *a2 = %d, a3 = %d) = %d\n",
			host_readws(Real2Host(a1)),
			host_readws(Real2Host(a2)),
			a3, (Bit16s)reg_ax);

		return 1;
	}
	case 0x34: {
		D1_LOG("FIG_latecomers()\n");
		FIG_latecomers();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg035(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		D1_LOG("FIG_tidy_monsters()\n");
		FIG_tidy_monsters();
		return 1;
	}
	case 0x25: {
		D1_LOG("FIG_loot_monsters()\n");
		FIG_loot_monsters();
		return 1;
	}
	case 0x2a: {
		D1_LOG("FIG_split_ap()\n");
		FIG_split_ap();
		return 1;
	}
	case 0x2f: {
		RealPt hero = CPU_Pop32();
		Bit16s hero_pos = CPU_Pop16();
		CPU_Push16(hero_pos);
		CPU_Push32(hero);

		reg_ax = check_hero_range_attack(Real2Host(hero), hero_pos);

		D1_LOG("check_hero_range_attack(%s, %d) = %d\n",
			schick_getCharname(hero), hero_pos, reg_ax);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg036(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			RealPt in_ptr = CPU_Pop32();
			Bit16s a2 = CPU_Pop16();
			D1_LOG("far seg036_00ae(%x, %d)\n", in_ptr, a2);
			seg036_00ae(Real2Host(in_ptr), a2);
			CPU_Push16(a2);
			CPU_Push32(in_ptr);
			return 1;
		}
		case 0x48: {
			RealPt hero = CPU_Pop32();
			Bit16s index = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			Bit16s y = CPU_Pop16();
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push16(index);
			CPU_Push32(hero);

			D1_LOG("KI_hero(%s, %d, %d, %d)\n",
					Real2Host(hero) + 0x10, index, x, y);
			KI_hero(Real2Host(hero), index, x, y);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}


static int seg037(unsigned short offs) {
	switch (offs) {
		case 0x3e: {
			RealPt p = CPU_Pop32();
			Bit16s active = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			Bit16s y = CPU_Pop16();
			CPU_Push16(y);
			CPU_Push16(x);
			CPU_Push16(active);
			CPU_Push32(p);

			D1_LOG("enemy_turn(%x, %x, %d, %d);\n",
				p, active, x, y);
			enemy_turn(Real2Host(p), active, x, y);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
					(signed short*)Real2Host(px),
					(signed short*)Real2Host(py));

			D1_LOG("far FIG_search_obj_on_cb(obj=%d, x=%d, y=%d) = %d\n",
				obj, mem_readw(Real2Phys(px)),
				mem_readw(Real2Phys(py)), reg_ax);

			host_writew(Real2Host(px),
				*((signed short*)Real2Host(px)));
			host_writew(Real2Host(py),
				*((signed short*)Real2Host(py)));

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
			RealPt in_ptr = CPU_Pop32();
			Bit16s a1 = CPU_Pop16();
			Bit16s x_in = CPU_Pop16();
			Bit16s y_in = CPU_Pop16();
			Bit16s a4 = CPU_Pop16();

			reg_ax = seg038(Real2Host(in_ptr), a1, x_in, y_in, (Bit8s)a4);
			D1_LOG("seg038(%x, %d, %d, %d, %d) = %d\n",
				in_ptr, a1, x_in, y_in, (Bit8s)a4, (Bit16s)reg_ax);

			CPU_Push16(a4);
			CPU_Push16(y_in);
			CPU_Push16(x_in);
			CPU_Push16(a1);
			CPU_Push32(in_ptr);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg039(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			Bit16s v3 = CPU_Pop16();
			Bit16s v4 = CPU_Pop16();
			CPU_Push16(v4);
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			reg_ax = calc_beeline(v1, v2, v3, v4);
			D1_LOG("calc_beeline(%d,%d,%d,%d); = %d\n",
				v1, v2, v3, v4, reg_ax);

			return 1;
		}
		case 0x25: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = FIG_get_range_weapon_type(Real2Host(hero));
			D1_LOG("FIG_get_range_weapon_type(%s) = %d\n", schick_getCharname(hero), (signed short)reg_ax);

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
			RealPt p = CPU_Pop32();
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			CPU_Push32(p);

			D1_LOG("FIG_load_enemy_sprites(%x, %d, %d);\n", p, v1, v2);
			FIG_load_enemy_sprites(Real2Host(p),
				(signed char)v1, (signed char)v2);

			return 1;
		}
		case 0x39: {
			D1_LOG("FIG_init_enemies();\n");
			FIG_init_enemies();
			return 1;
		}
		case 0x3e: {
			D1_LOG("FIG_init_heroes();\n");
			FIG_init_heroes();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg040(unsigned short offs) {
	switch(offs) {
	case 0x20: {
		D1_LOG("FIG_chessboard_init()\n");
		FIG_chessboard_init();
		return 1;
	}
	case 0x25: {
		D1_LOG("FIG_preload_gfx()\n");
		FIG_preload_gfx();
		return 1;
	}
	case 0x2a: {
		D1_LOG("FIG_draw_scenario()\n");
		FIG_draw_scenario();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg041(unsigned short offs) {
	switch(offs) {
	case 0x20: {
		RealPt hero = CPU_Pop32();
		Bit16u arg = CPU_Pop16();
		reg_ax = seg041_0020(Real2Host(hero), (signed short)arg);
		D1_LOG("seg041_0020(%s, %d); = %d\n",
			(char*)Real2Host(hero) + 0x10, arg, reg_ax);
		CPU_Push16(arg);
		CPU_Push32(hero);
		return 1;
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
		RealPt hero = CPU_Pop32();
		RealPt target = CPU_Pop32();
		Bit16u flag = CPU_Pop16();

		reg_ax = FIG_get_hero_melee_attack_damage(Real2Host(hero), Real2Host(target), flag);
		D1_LOG("FIG_get_hero_melee_attack_damage(%s, %s); = %d\n",
			(char*)Real2Host(hero) + 0x10,
			flag != 0 ? (char*)Real2Host(target) + 0x10 : "enemy",
			reg_ax);

		CPU_Push16(flag);
		CPU_Push32(target);
		CPU_Push32(hero);

		return 1;
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

static int seg042(unsigned short offs) {

	switch (offs) {

	case 0x20: {
		RealPt hero = CPU_Pop32();
		Bit16s target = CPU_Pop16();
		CPU_Push16(target);
		CPU_Push32(hero);

		D1_LOG("FIG_do_hero_action(%s, %d)\n", schick_getCharname(hero), target);
		FIG_do_hero_action(hero, target);

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
		RealPt monster = CPU_Pop32();
		Bit16s target = CPU_Pop16();
		CPU_Push16(target);
		CPU_Push32(monster);

		D1_LOG("FIG_do_monster_action(%x, %d)\n", monster, target);
		FIG_do_monster_action(monster, target);

		return 1;
	}
	case 0x25: {
		RealPt hero = CPU_Pop32();
		RealPt target_monster = CPU_Pop32();
		RealPt target_hero = CPU_Pop32();
		Bit16s flag = CPU_Pop16();
		Bit16s hero_pos = CPU_Pop16();
		CPU_Push16(hero_pos);
		CPU_Push16(flag);
		CPU_Push32(target_hero);
		CPU_Push32(target_monster);
		CPU_Push32(hero);

		D1_LOG("FIG_use_item(%s, %s, %s, %d, %d);\n",
			schick_getCharname(hero),
			get_monname(host_readbs(Real2Host(target_monster))),
			schick_getCharname(target_hero),
			flag, hero_pos);

		FIG_use_item(Real2Host(hero), Real2Host(target_monster), Real2Host(target_hero), flag, hero_pos);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg044(unsigned short offs) {

	switch (offs) {

	case 0x20: {
		Bit16s a1 = CPU_Pop16();
		RealPt hero = CPU_Pop32();
		Bit16s weapon_type = CPU_Pop16();
		Bit16s a4 = CPU_Pop16();
		Bit8s a5 = (Bit8s)CPU_Pop16();
		Bit8u a6 = (Bit8u)CPU_Pop16();
		Bit16s a7 = CPU_Pop16();

		D1_LOG("FIG_prepare_hero_fight_ani(%d, %s, w_type = %d, %d, %d, %u, %d)\n",
			a1, schick_getCharname(hero), weapon_type, a4, a5, a6, a7);
		FIG_prepare_hero_fight_ani(a1, Real2Host(hero), weapon_type, a4, a5, a6, a7);

		CPU_Push16(a7);
		CPU_Push16(a6);
		CPU_Push16(a4);
		CPU_Push16(a5);
		CPU_Push16(weapon_type);
		CPU_Push32(hero);
		CPU_Push16(a1);
		return 1;
	}
	case 0x25: {
	//	return 0;
		Bit16s a1 = CPU_Pop16();
		RealPt mon = CPU_Pop32();
		Bit16s a3 = CPU_Pop16();
		Bit8s a4 = (Bit8s)CPU_Pop16();
		Bit8u a5 = (Bit8u)CPU_Pop16();
		Bit16s a6 = CPU_Pop16();

		D1_LOG("FIG_prepare_enemy_fight_ani(%d, %s, %d, %d, %u, %d)\n",
			a1, get_monname(host_readb(Real2Host(mon))), a3, a4, a5, a6);
		FIG_prepare_enemy_fight_ani(a1, Real2Host(mon), a3, a4, a5, a6);

		CPU_Push16(a6);
		CPU_Push16(a5);
		CPU_Push16(a4);
		CPU_Push16(a3);
		CPU_Push32(mon);
		CPU_Push16(a1);
		return 1;
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
			v1, schick_getCharname(p), v2,
			(Bit8s)obj1, (Bit8s)obj2, v5, v6);

		seg044_002a(v1, Real2Host(p), v2,
			(Bit8s)obj1, (Bit8s)obj2, v5, v6);

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

		seg044_002f(v1, Real2Host(p), v2, (Bit8s)obj1, (Bit8s)obj2, v5);

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

static int seg045(unsigned short offs) {
	switch (offs) {

	case 0x25: {
		D1_LOG("FIG_remove_smth();\n");
		FIG_remove_smth();
		return 1;
	}
	case 0x2f: {
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		Bit16s a3 = CPU_Pop16();
		Bit16s a4 = CPU_Pop16();
		Bit16s a5 = CPU_Pop16();
		reg_ax = seg045_01a0(a1, a2, a3, a4, a5);
		D1_LOG("seg045_01a0(%d, %d, %d, %d, %d); = %d\n",
			a1, a2, a3, a4, a5, (Bit16s)reg_ax);
		CPU_Push16(a5);
		CPU_Push16(a4);
		CPU_Push16(a3);
		CPU_Push16(a2);
		CPU_Push16(a1);
		return 0;
	}
	case 0x39: {
		D1_LOG("FIG_remove_smth2();\n");
		FIG_remove_smth2();
		return 1;
	}
	case 0x3e: {
		Bit16s a1 = CPU_Pop16();
		RealPt hero = CPU_Pop32();
		Bit16s a3 = CPU_Pop16();

		D1_LOG("seg045_0394(%d, %s, %d);\n",
			a1, schick_getCharname(hero), a3);
		seg045_0394(a1, Real2Host(hero), a3);

		CPU_Push16(a3);
		CPU_Push32(hero);
		CPU_Push16(a1);
		return 1;
	}
	case 0x43: {
		Bit16s a1 = CPU_Pop16();
		RealPt enemy = CPU_Pop32();
		Bit16s a3 = CPU_Pop16();

		D1_LOG("seg045_0394(%d, enemy, %d);\n",
			a1, a3);
		seg045_0394(a1, Real2Host(enemy), a3);

		CPU_Push16(a3);
		CPU_Push32(enemy);
		CPU_Push16(a1);
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
		Bit32s money = CPU_Pop32();
		CPU_Push32(money);
		CPU_Push32(s_ptr);

		D1_LOG("make_valuta_str(%x, %d);\n", s_ptr, money);
		make_valuta_str((char*)Real2Host(s_ptr), money);

		return 1;
	}
	case 0x25: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		hero_get_sober(Real2Host(hero));
		D1_LOG("%s ist wieder nuechtern\n", schick_getCharname(hero));

		return 1;
	}
	case 0x2a: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		hero_get_drunken(Real2Host(hero));
		D1_LOG("%s ist betrunken\n", schick_getCharname(hero));

		return 1;
	}
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
		RealPt ptr = CPU_Pop32();
		Bit16s entries = CPU_Pop16();
		Bit16s mode = CPU_Pop16();
		CPU_Push16(mode);
		CPU_Push16(entries);
		CPU_Push32(ptr);

		reg_ax = menu_enter_delete(ptr, entries, mode);
		D1_LOG("Called menu_enter_delete(%x, %d, %d) = %d\n",
			ptr, entries, mode, reg_ax);
		return 1;
	}
	case 0x48: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);
		D1_LOG("update_atpa(%s);\n", schick_getCharname(hero));
		update_atpa(Real2Host(hero));
		return 1;
	}
	case 0x4d: {
		RealPt title = CPU_Pop32();
		CPU_Push32(title);
		reg_ax = select_hero_from_group(Real2Host(title));
		D1_LOG("select_hero_from_group(%s) = %d;\n",
				Real2Host(title), (signed short)reg_ax);
		return 1;
	}
	case 0x52: {
		RealPt title = CPU_Pop32();
		CPU_Push32(title);
		reg_ax = select_hero_ok(Real2Host(title));
		D1_LOG("select_hero_ok(%s) = %d;\n",
				Real2Host(title), (signed short)reg_ax);
		return 1;
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
		if (host_readb(Real2Host(hero) + 0x21) != 0)
			D1_INFO("%s ist erkrankt %d\n",
				schick_getCharname(hero), disease);
		return 1;
	}
	case 0x70: {
		RealPt hero = CPU_Pop32();
		unsigned short disease = CPU_Pop16();
		signed short probability = CPU_Pop16();
		CPU_Push16(probability);
		CPU_Push16(disease);
		CPU_Push32(hero);

		hero_disease_test(Real2Host(hero), disease, probability);

		if (host_readb(Real2Host(hero) + 0x21) != 0)
			D1_INFO("%s koennte zu %d%% an %d erkranken\n",
				schick_getCharname(hero), probability, disease);
		return 1;
	}
	case 0x75: {
		RealPt title = CPU_Pop32();
		CPU_Push32(title);
		reg_ax = select_hero_ok_forced(Real2Host(title));
		D1_LOG("select_hero_ok_forced(%s) = %d;\n",
				Real2Host(title), (signed short)reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg048(unsigned short offs)
{
	switch(offs) {
	case 0x20: {
		D1_LOG("status_select_hero()\n");
		status_select_hero();
		return 1;
	}
	case 0x25: {
		Bit16s hero_pos = CPU_Pop16();
		CPU_Push16(hero_pos);
		D1_LOG("status_menu(%d);\n", hero_pos);
		status_menu(hero_pos);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg049(unsigned short offs)
{
	switch(offs) {
	case 0x20: {
		D1_LOG("GRP_swap_heros()\n");
		GRP_swap_heros();
		return 1;
	}
	case 0x25: {
		Bit16s pos = CPU_Pop16();
		D1_LOG("GRP_move_hero(%d)\n", pos);
		//GRP_move_hero(pos);
		CPU_Push16(pos);
		return 0;
	}
	case 0x2a: {
		RealPt hero = CPU_Pop32();
		Bit16s mod = CPU_Pop16();
		D1_LOG("GRP_hero_sleep(%s, %d)\n",
			Real2Host(hero) + 0x10, mod);
		GRP_hero_sleep(Real2Host(hero), mod);
		CPU_Push16(mod);
		CPU_Push32(hero);
		return 1;
	}
	case 0x34: {
		D1_LOG("GRP_split()\n");
		GRP_split();
		return 1;
	}
	case 0x39: {
		D1_LOG("GRP_merge()\n");
		GRP_merge();
		return 1;
	}
	case 0x3e: {
		Bit16s mode = CPU_Pop16();
		D1_LOG("GRP_switch_to_next(%d)\n", mode);
		GRP_switch_to_next(mode);
		CPU_Push16(mode);
		return 1;
	}
	case 0x43: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		reg_ax = GRP_compare_heros((void*)Real2Host(p1), (void*)Real2Host(p2));
		D1_LOG("GRP_compare_heros() = %d\n", (signed short)reg_ax);
		CPU_Push32(p2);
		CPU_Push32(p1);
		return 1;
	}
	case 0x48: {
		Bit16s group = CPU_Pop16();
		D1_LOG("GRP_save_pos(%d)\n", group);
		GRP_save_pos(group);
		CPU_Push16(group);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg050(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16u hero_pos = CPU_Pop16();

			D1_LOG("level_up(%s);\n", get_hero(hero_pos) + 0x10);
			level_up(hero_pos);

			CPU_Push16(hero_pos);
			return 1;
		}
		case 0x2f: {
			RealPt hero = CPU_Pop32();
			Bit16s skill = CPU_Pop16();

			D1_LOG("inc_skill_novice(%s, %s);\n",
				schick_getCharname(hero), names_skill[skill]);
			inc_skill_novice(Real2Host(hero), skill);

			CPU_Push16(skill);
			CPU_Push32(hero);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg052(const unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("Citycamp();\n");
			do_citycamp();
			return 1;
		}
		default: {
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
		}
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

			do_healer();

			return 1;
		}
		case 0x25: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = is_hero_healable(Real2Host(hero));

			D1_LOG("is_hero_healable(%s) = %d\n",
				Real2Host(hero) + 0x10, reg_ax);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg054(unsigned short offs)
{
	switch (offs) {
		case 0x20: return n_seg054(0x007c);
		case 0x2f: return n_seg054(0x0ca2);
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg055(unsigned short offs)
{
	switch (offs) {
		case 0x20: return n_seg055(0x007a);
		case 0x25: return n_seg055(0x0622);
		case 0x2f: return n_seg055(0x0660);
		case 0x34: return n_seg055(0x06e1);
		case 0x39: return n_seg055(0x078e);
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg056(const unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			RealPt p1 = CPU_Pop32();
			RealPt p2 = CPU_Pop32();
			CPU_Push32(p2);
			CPU_Push32(p1);
#if 0
			Bit32s retval;
			retval = shop_compar(Real2Host(p1), Real2Host(p2));
			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			D1_INFO("shop_compar() = %d\n", (Bit16s)reg_ax);
			return 1;
#else
			return 0;
#endif
		}
		case 0x25: {
			D1_LOG("buy_screen();\n");
			buy_screen();
			return 1;
		}
		case 0x2a: {
			RealPt ptr = CPU_Pop32();
			RealPt hero = CPU_Pop32();
			Bit16s item_pos = CPU_Pop16();
			Bit16s shop_pos = CPU_Pop16();
			CPU_Push16(shop_pos);
			CPU_Push16(item_pos);
			CPU_Push32(hero);
			CPU_Push32(ptr);


			D1_LOG("insert_sell_items(0x%x, %s, %d, %d)\n",
				ptr, schick_getCharname(hero), item_pos, shop_pos);

			insert_sell_items(Real2Host(ptr), Real2Host(hero), item_pos, shop_pos);

			return 1;
		}
		default: {
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
		}
	}
}

static int seg057(const unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			RealPt shop_ptr = CPU_Pop32();
			CPU_Push32(shop_ptr);
			D1_LOG("sell_screen(%x)\n", shop_ptr);
			sell_screen(Real2Host(shop_ptr));
			return 1;
		}
		default: {
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
		}
	}
}

static int seg058(const unsigned short offs)
{
	switch (offs) {
		case 0x20: return n_seg058(0xdc3);
		case 0x34: return n_seg058(0xfa7);
		default: {
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
		}
	}
}

static int seg061(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("do_temple();\n");
			do_temple();
			return 1;
		}
		case 0x2f: {
			reg_ax = char_erase();
			D1_LOG("char_erase(); = %d\n", reg_ax);
			return 1;
		}
		case 0x34: {
			Bit16s le_in = CPU_Pop16();
			RealPt str = CPU_Pop32();
			CPU_Push32(str);
			CPU_Push16(le_in);

			D1_LOG("miracle_heal_hero(%d,%p);\n", le_in, str);
			miracle_heal_hero(le_in, Real2Host(str));

			return 1;
		}
		case 0x39: {
			RealPt str = CPU_Pop32();
			CPU_Push32(str);

			D1_LOG("miracle_resurrect(%p);\n", str);
			miracle_resurrect(Real2Host(str));
			return 1;
		}
		case 0x3e: {
			Bit16u offset = CPU_Pop16();
			Bit32s timer_value = CPU_Pop32();
			Bit16s mod = CPU_Pop16();
			CPU_Push16(mod);
			CPU_Push32(timer_value);
			CPU_Push16(offset);

			D1_LOG("miracle_modify(%d,0x%x,%d);\n", offset, timer_value, mod);
			miracle_modify(offset, timer_value, mod);
			return 1;
		}
		case 0x43: {
			RealPt str = CPU_Pop32();
			Bit16s mode = CPU_Pop16();
			CPU_Push16(mode);
			CPU_Push32(str);

			D1_LOG("miracle_weapon(%p,%d);\n", str, mode);
			miracle_weapon(Real2Host(str), mode);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg062(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("ask_miracle();\n");
			ask_miracle();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg064(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		RealPt tmp;
		Bit16s price = CPU_Pop16();
		RealPt entry = CPU_Pop32();
		CPU_Push32(entry);
		CPU_Push16(price);
		tmp = print_passage_price(price, Real2Host(entry));
		D1_LOG("print_passage_price(%d, %x)\n", price, entry);
		reg_ax = RealOff(tmp);
		reg_dx = RealSeg(tmp);
		return 1;
	}
	case 0x2a: {
		reg_ax = prepare_passages();
		D1_LOG("prepare_passages(); = %d\n", reg_ax);
		return 1;
	}
	case 0x2f: {
		Bit16s arg1 = CPU_Pop16();
		Bit16s arg2 = CPU_Pop16();
		CPU_Push16(arg2);
		CPU_Push16(arg1);
		reg_ax = get_passage_travel_hours(arg1, arg2);
		D1_LOG("get_passage_travel_hours(%d, %d) = %d\n",
			arg1, arg2, reg_ax);
		return 1;
	}
	case 0x34: {
		Bit16u type = CPU_Pop16();
		CPU_Push16(type);
		reg_ax = get_next_passages(type);
		D1_LOG("get_next_passages(%d) = %d\n", type, reg_ax);
		return 1;
	}
	case 0x39: {
		D1_LOG("passage_arrival()\n");
		reg_ax = passage_arrival();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg066(unsigned short offs)
{
	switch (offs) {
	case 0x2a: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x2f: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x34: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x39: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x61: {
		D1_LOG("seg066_172b()\n");
		seg066_172b();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg067(unsigned short offs)
{
	switch (offs) {
	case 0x4d: return n_seg067(0x0000);
	case 0x52: return n_seg067(0x0ca8);
	case 0x57: return n_seg067(0x0ce6);
	case 0x5c: return n_seg067(0x0d27);
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg070(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			D1_LOG("PHX_fuhrhaus()\n");
			PHX_fuhrhaus();
			return 1;
		}
		case 0x25: {
			D1_LOG("PHX_stadthaus()\n");
			PHX_stadthaus();
			return 1;
		}
		case 0x2a: {
			D1_LOG("PHX_spielhaus()\n");
			PHX_spielhaus();
			return 1;
		}
		case 0x2f: {
			D1_LOG("PHX_villa_gremob()\n");
			PHX_villa_gremob();
			return 1;
		}
		case 0x34: {
			D1_LOG("PHX_bordel()\n");
			PHX_bordell();
			return 1;
		}
		case 0x39: {
			D1_LOG("PHX_apotheke()\n");
			PHX_apotheke();
			return 1;
		}
		case 0x3e: {
#if 0
			D1_LOG("PHX_healer()\n");
			PHX_healer();
			return 1;
#else
			return 0;
#endif
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg071(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			D1_LOG("PHX_drogentempel()\n");
			PHX_drogentempel();
			return 1;
		}
		case 0x25: {
			D1_LOG("PHX_phextempel()\n");
			PHX_phextempel();
			return 1;
		}
		case 0x2a: {
			D1_LOG("PHX_alrik_derondan()\n");
			PHX_alrik_derondan();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}


static int seg072(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Jurge" : "Hjore", v2);
			INF_jurge_hjore(v1, v2);
			return 1;
		}
		case 0x2f: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Yasma" : (v1 == 1 ? "Umbrik" : "Isleif"), v2);
			INF_yasma_umbrik_isleif(v1, v2);
			return 1;
		}
		case 0x34: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Ragna" : (v1 == 1 ? "Beorn" : "Algrid"), v2);
			INF_ragna_beorn_algrid(v1, v2);
			return 1;
		}
		case 0x39: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Eliane" : "Tiomar", v2);
			INF_eliane_tiomar(v1, v2);
			return 1;
		}
		case 0x3e: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Olvir" : "Asgrimm", v2);
			INF_olvir_asgrimm(v1, v2);
			return 1;
		}
		case 0x43: {
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 || v1 == 1 ? "Treborn" : "Einhorn", v2);
#if 0
			INF_treborn_unicorn(v1, v2);
			return 1;
#else
			return 0;
#endif
		}
		case 0x48: {
			/* Informer: Swafnild */
			Bit16s v1 = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(v1);
			D1_INFO("Informant %s Zustand %d\n", v1 == 0 ? "Swafnild" : "Einhorn", v2);
			INF_swafnild_unicorn(v1, v2);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
			exit(1);
	}
}

static int seg073(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			reg_ax = get_tavern_gossip();
			D1_LOG("tavern_gossip() = %d\n", reg_ax);
			return 1;
		}
		case 0x25: {
			RealPt ret;
			D1_LOG("get_drinkmate();\n");
			ret = get_drinkmate();

			reg_ax = RealOff(ret);
			reg_dx = RealSeg(ret);
			return 1;
		}
		case 0x2a: {
			reg_ax = tavern_quest_infos();
			D1_LOG("tavern_quest_infos() = %d\n", reg_ax);
			return 1;
		}
		case 0x2f: {
#if 0
			D1_LOG("tavern_follow_informer()\n");
			tavern_follow_informer();
			return 1;
#else
			return 0;
#endif
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg074(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("show_automap()\n");
			show_automap();
			return 1;
		}
		case 0x25: {
			D1_LOG("select_teleport_dest();\n");
			reg_ax = select_teleport_dest();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg075(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			D1_LOG("DNG_update_pos()\n");
			DNG_update_pos();
			return 1;
		}
		case 0x25: {
			D1_LOG("DNG_inc_level()\n");
			DNG_inc_level();
			return 1;
		}
		case 0x2a: {
			D1_LOG("DNG_dec_level()\n");
			DNG_dec_level();
			return 1;
		}
		case 0x43: {
			Bit16s a1 = CPU_Pop16();
			D1_LOG("DNG_timestep(%d);\n", a1);
			DNG_timestep(a1);
			CPU_Push16(a1);
			return 1;
		}
		case 0x57: {
			D1_LOG("DNG_open_door()\n");
			DNG_open_door();
			return 1;
		}
		case 0x5c: {
			D1_LOG("DNG_close_door()\n");
			DNG_close_door();
			return 1;
		}
		case 0x61: {
			D1_LOG("DNG_lights()\n");
			DNG_lights();
			return 1;
		}
		case 0x66: {
			Bit16s a1 = CPU_Pop16();
			reg_ax = DNG_fallpit(a1);
			D1_LOG("DNG_fallpit(%d) = %d\n", a1, reg_ax);
			CPU_Push16(a1);
			return 1;
		}
		case 0x70: {
			DNG_stub6();
			D1_LOG("DNG_stub6()\n");
			return 1;
		}
		case 0x75: {
			reg_ax = DNG_check_climb_tools();
			D1_LOG("DNG_check_climb_tools() = %d\n", reg_ax);
			return 1;
		}
		case 0x7a: {
			Bit16s dungeon_id = CPU_Pop16();
			D1_LOG("DNG_enter_dungeon(%d)\n", dungeon_id);
			DNG_enter_dungeon(dungeon_id);
			CPU_Push16(dungeon_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg076(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			D1_LOG("do_Dungeon()\n");
			return 0;
		}
		case 0x25: {
			return 0;
		}
		case 0x48: {
			RealPt msg = CPU_Pop32();
			CPU_Push32(msg);

			D1_LOG("print_msg_with_first_hero()\n");
			print_msg_with_first_hero(Real2Host(msg));
			return 1;
		}
		case 0x52: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}


static int seg077(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
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
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg078(unsigned short offs)
{
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
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG2_fight_intro(%d)\n", fight_id);
			DNG2_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg080(unsigned short offs)
{
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
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG5_fight_intro(%d)\n", fight_id);
			DNG5_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg081(unsigned short offs)
{
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
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG6_fight_intro(%d)\n", fight_id);
			DNG6_fight_intro(fight_id);
			return 1;
		}
		case 0x34: {
			return 0;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg084(unsigned short offs)
{
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
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG9_fight_intro(%d)\n", fight_id);
			DNG9_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg086(unsigned short offs)
{
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
			return 0;
		}
		case 0x66: {
			return 0;
		}
		case 0x6b: {
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG11_fight_intro(%d)\n", fight_id);
			DNG11_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg088(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			return 0;
		}
		case 0x25: {
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG14_fight_intro(%d)\n", fight_id);
			DNG14_fight_intro(fight_id);
			return 1;
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
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg090(unsigned short offs)
{
	switch (offs) {
		case 0x25: {
			RealPt chest = CPU_Pop32();
			CPU_Push32(chest);

			D1_LOG("DNG_oberorken_chest()\n");
			DNG_oberorken_chest(Real2Host(chest));
			return 1;
		}
		case 0x2a: {
			D1_LOG("DNG_handler_oberorken()\n");
			reg_ax = DNG_handler_oberorken();
			return 1;
		}
		case 0x2f: {
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG12_fight_intro(%d)\n", fight_id);
			DNG12_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg091(unsigned short offs)
{
	switch (offs) {
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
			Bit16s fight_id = CPU_Pop16();
			CPU_Push16(fight_id);

			D1_LOG("DNG13_fight_intro(%d)\n", fight_id);
			DNG13_fight_intro(fight_id);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg092(unsigned short offs) {
	switch (offs) {
	case 0x25: {
		D1_LOG("chest_protected_normal()\n");
		chest_protected_normal();
		return 1;
	}
	case 0x2a: {
		D1_LOG("chest_closed()\n");
		chest_closed();
		return 1;
	}
	case 0x2f: {
		D1_LOG("chest_protected_heavy()\n");
		chest_protected_heavy();
		return 1;
	}
	case 0x34: {
		D1_LOG("chest_cursed()\n");
		chest_cursed();
		return 1;
	}
	case 0x39: {
		D1_LOG("chest_fulminictus()\n");
		chest_fulminictus();
		return 1;
	}
	case 0x3e: {
		RealPt chest = CPU_Pop32();
		CPU_Push32(chest);

		D1_LOG("loot_simple_chest()\n");
		loot_simple_chest(Real2Host(chest));
		return 1;
	}
	case 0x43: {
		RealPt chest = CPU_Pop32();
		RealPt text1 = CPU_Pop32();
		RealPt text2 = CPU_Pop32();
		CPU_Push32(text2);
		CPU_Push32(text1);
		CPU_Push32(chest);

		D1_LOG("loot_chest()\n");
		loot_chest(Real2Host(chest), Real2Host(text1), Real2Host(text2));
		return 1;
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
		D1_LOG("chest_poisoned1()\n");
		chest_poisoned1();
		return 1;
	}
	case 0x61: {
		D1_LOG("chest_poisoned2()\n");
		chest_poisoned2();
		return 1;
	}
	case 0x66: {
		D1_LOG("chest_poisoned3()\n");
		chest_poisoned3();
		return 1;
	}
	case 0x6b: {
		D1_LOG("chest_protected_brutal()\n");
		chest_protected_brutal();
		return 1;
	}
	case 0x70: {
		D1_LOG("chest_stoned()\n");
		chest_stoned();
		return 1;
	}
	case 0x75: {
		D1_LOG("chest_ignifax_normal()\n");
		chest_ignifax_normal();
		return 1;
	}
	case 0x7a: {
		D1_LOG("chest_ignifax_brutal()\n");
		chest_ignifax_brutal();
		return 1;
	}
	case 0x7f: {
		D1_LOG("chest_ignifax_heavy()\n");
		chest_ignifax_heavy();
		return 1;
	}
	case 0x84: {
		D1_LOG("chest_crossbow_bolts()\n");
		chest_crossbow_bolts();
		return 1;
	}
	case 0x89: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_has_lockpicks(Real2Host(hero));
		D1_LOG("hero_has_lockpicks(%s)= %d\n",
			schick_getCharname(hero), (Bit16s)reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg094(unsigned short offs) {
	switch (offs) {
	case 0x25: {
		return 0;
	}
	case 0x2a: {
		return 0;
	}
	case 0x2f: {
		D1_LOG("prepare_map_marker()\n");
		prepare_map_marker();
		return 1;
	}
	case 0x48: {
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
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

			D1_LOG("remove_npc(%x, %d, %x, %x, %x);\n",
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
			D1_LOG("npcmeetings(%x);\n", type);
			reg_ax = npc_meetings(type);
			CPU_Push16(type);
			return 1;
		}
		case 0x4d: {
			D1_LOG("npc_farewell();\n");
			npc_farewell();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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

		retval = GUI_name_plural(v1, Real2Host(s));
		D1_LOG("GUI_name_plural(%x, %s)\n",
			v1, getString(s));

		reg_dx = RealSeg(retval);
		reg_ax = RealOff(retval);

		return 1;
	}
	case 0x2f: {
		RealPt s = CPU_Pop32();
		CPU_Push32(s);
		RealPt retval;

		retval = GUI_name_singular(Real2Host(s));
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
			D1_LOG("GUI_get_ptr2(%d,%d) = %s",
				v1, v2, getString(retval));

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
	}
	case 0x43: {
		RealPt s = CPU_Pop32();
		signed short x = CPU_Pop16();
		signed short y = CPU_Pop16();
		CPU_Push16(y);
		CPU_Push16(x);
		CPU_Push32(s);

		D1_LOG("GUI_print_string(%s, %d, %d);\n", getString(s), x, y);
		GUI_print_string(Real2Host(s), x, y);
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

		reg_ax = GUI_count_lines(Real2Host(ptr));
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

		D1_LOG("set_textcolor(%d, %d);\n", v1, v2);
		set_textcolor(v1, v2);

		return 1;
	}
	case 0x7a: {
		RealPt p1 = CPU_Pop32();
		RealPt p2 = CPU_Pop32();
		CPU_Push32(p2);
		CPU_Push32(p1);

		D1_LOG("get_textcolor();\n");
		get_textcolor((signed short*)Real2Host(p1),
			(signed short*)Real2Host(p2));

		return 1;
	}
	case 0x7f: {
		RealPt ptr = CPU_Pop32();
		CPU_Push32(ptr);

		D1_LOG("GUI_print_loc_line(%s)\n", getString(ptr));
		GUI_print_loc_line(Real2Host(ptr));
		return 1;
	}
	case 0x84: {
		RealPt str = CPU_Pop32();
		CPU_Push32(str);

		D1_LOG("GUI_print_header(%s)\n", getString(str));
		GUI_print_header(Real2Host(str));
		return 1;
	}
	case 0x89: {
		RealPt p = CPU_Pop32();
		unsigned short v1 = CPU_Pop16();
		CPU_Push16(v1);
		CPU_Push32(p);

		reg_ax = GUI_get_space_for_string(Real2Host(p), (char)v1);
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
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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

		reg_ax = GUI_lookup_char_height(c & 0xff, (signed short*)Real2Host(p_height));
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
		buffer[19] = '\0';
		reg_ax = GUI_bool(Real2Host(ptr));
		D1_LOG("GUI_bool(%s...) = %d\n", buffer, reg_ax);

		return 1;
	}
	case 0x43: {
		RealPt text = CPU_Pop32();
		unsigned short options = CPU_Pop16();

		unsigned short i;

		strncpy(buffer, getString(text), 20);
		buffer[19] = '\0';
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
		Bit16s fight_id = CPU_Pop16();
		CPU_Push16(fight_id);

		D1_LOG("GUI_print_fight_intro_msg(%d)\n", fight_id);
		GUI_print_fight_intro_msg(fight_id);
		return 1;
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

		reg_ax = use_magic(hero);
		D1_LOG("use_magic(%s) = %d\n",
			schick_getCharname(hero), (Bit16s)reg_ax);

		return 1;
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
		RealPt hero = CPU_Pop32();
		Bit16s a1 = CPU_Pop16();
		Bit16s a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(a1);
		CPU_Push32(hero);

		reg_ax = use_spell(hero, a1, (Bit8s)a2);
		D1_LOG("use_spell(%s, %d, %d) = %d\n",
			schick_getCharname(hero), a1, a2, (Bit16s)reg_ax);

		return 1;
	}
	case 0x2f: {
		RealPt hero = CPU_Pop32();
		Bit16s show_vals = CPU_Pop16();
		CPU_Push16(show_vals);
		CPU_Push32(hero);

		reg_ax = select_spell(Real2Host(hero), show_vals);

		D1_LOG("select_spell(%s, %d) = %d\n",
			schick_getCharname(hero), show_vals, reg_ax);
		return 1;
	}
	case 0x39: {
		Bit16s le = CPU_Pop16();
		CPU_Push16(le);

		D1_LOG("FIG_do_spell_damage(%d)\n", le);
		FIG_do_spell_damage(le);
		return 1;
	}
	case 0x3e: {
		D1_LOG("get_attackee_parade();\n");
		reg_ax = get_attackee_parade();
		return 1;
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
		reg_ax = select_magic_user();
		D1_LOG("select_magic_user() = %d\n", (Bit16s)reg_ax);
		return 1;
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
		spell_beherrschung();
		return 1;
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
		spell_illusionen();
		return 1;
	}
	case 0x34: {
		spell_verwandlung();
		return 1;
	}
	case 0x39: {
		spell_band();
		return 1;
	}
	case 0x3e: {
		spell_bannbaladin();
		return 1;
	}
	case 0x43: {
		spell_boeser_blick();
		return 1;
	}
	case 0x48: {
		spell_grosse_gier();
		return 1;
	}
	case 0x4d: {
		spell_grosse_ver();
		return 1;
	}
	case 0x52: {
		spell_herrdertiere();
		return 1;
	}
	case 0x57: {
		spell_horriphobus();
		return 1;
	}
	case 0x5c: {
		spell_magischerraub();
		return 1;
	}
	case 0x61: {
		spell_respondami();
		return 1;
	}
	case 0x66: {
		spell_sanftmut();
		return 1;
	}
	case 0x6b: {
		spell_somnigravis();
		return 1;
	}
	case 0x70: {
		spell_zwingtanz();
		return 1;
	}
	case 0x75: {
		spell_furor_blut();
		return 1;
	}
	case 0x7a: {
		spell_geister_bannen();
		return 1;
	}
	case 0x7f: {
		spell_geister_rufen();
		return 1;
	}
	case 0x84: {
		spell_heptagon();
		return 1;
	}
	case 0x89: {
		spell_kraehenruf();
		return 1;
	}
	case 0x8e: {
		spell_skelettarius();
		return 1;
	}
	case 0x93: {
		spell_elementare();
		return 1;
	}
	case 0x98: {
		spell_nihilatio();
		return 1;
	}
	case 0x9d: {
		spell_solidirid();
		return 1;
	}
	case 0xa2: {
		spell_axxeleratus();
		return 1;
	}
	case 0xa7: {
		spell_foramen();
		return 1;
	}
	case 0xac: {
		spell_motoricus();
		return 1;
	}
	case 0xb1: {
		spell_spurlos();
		return 1;
	}
	case 0xb6: {
		spell_transversalis();
		return 1;
	}
	case 0xbb: {
		spell_ueber_eis();
		return 1;
	}
	case 0xc0: {
		spell_balsam();
		return 1;
	}
	case 0xc5: {
		spell_hexenspeichel();
		return 1;
	}
	case 0xca: {
		spell_klarum_purum();
		return 1;
	}
	case 0xcf: {
		spell_ruhe_koerper();
		return 1;
	}
	case 0xd4: {
		spell_tiere_heilen();
		return 1;
	}
	case 0xd9: {
		spell_adleraug();
		return 1;
	}
	case 0xde: {
		RealPt retval;
		retval = spell_analues();

		reg_ax = RealOff(retval);
		reg_dx = RealSeg(retval);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

static int seg100(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		spell_eigenschaften();
		return 1;
	}
	case 0x25: {
		spell_exposami();
		return 1;
	}
	case 0x2a: {
		spell_odem_arcanum();
		return 1;
	}
	case 0x2f: {
		spell_penetrizzel();
		return 1;
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
		spell_hexenknoten();
		return 1;
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
		spell_eisenrost();
		return 1;
	}
	case 0x5c: {
		spell_fulminictus();
		return 1;
	}
	case 0x61: {
		spell_ignifaxius();
		return 1;
	}
	case 0x66: {
		spell_plumbumbarum();
		return 1;
	}
	case 0x6b: {
		spell_radau();
		return 1;
	}
	case 0x70: {
		spell_saft_kraft();
		return 1;
	}
	case 0x75: {
		spell_scharfes_auge();
		return 1;
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
		spell_brenne();
		return 1;
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

static int seg102(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		RealPt mon = CPU_Pop32();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push32(mon);

		reg_ax = MON_cast_spell(mon, (Bit8s)bonus);
		D1_LOG("MON_cast_spell(..., %d) = %d\n", bonus, (Bit16s)reg_ax);
		return 1;
	}
	case 0x25: {
		mspell_verwandlung();
		return 1;
	}
	case 0x2a: {
		mspell_bannbaladin();
		return 1;
	}
	case 0x2f: {
		mspell_boeser_blick();
		return 1;
	}
	case 0x34: {
		mspell_horriphobus();
		return 1;
	}
	case 0x39: {
		mspell_axxeleratus();
		return 1;
	}
	case 0x3e: {
		mspell_balsam();
		return 1;
	}
	case 0x43: {
		mspell_blitz();
		return 1;
	}
	case 0x48: {
		mspell_eisenrost();
		return 1;
	}
	case 0x4d: {
		mspell_fulminictus();
		return 1;
	}
	case 0x52: {
		mspell_ignifaxius();
		return 1;
	}
	case 0x57: {
		mspell_plumbumbarum();
		return 1;
	}
	case 0x5c: {
		mspell_saft_kraft();
		return 1;
	}
	case 0x61: {
		mspell_armatrutz();
		return 1;
	}
	case 0x66: {
		mspell_paral();
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
			Bit16s skill = CPU_Pop16();
			Bit16s bonus = CPU_Pop16();
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
			Bit16s items = CPU_Pop16();
			Bit32s price = CPU_Pop32();
			Bit16s percent = CPU_Pop16();
			Bit16s mod_init = CPU_Pop16();
			CPU_Push16(mod_init);
			CPU_Push16(percent);
			CPU_Push32(price);
			CPU_Push16(items);
			CPU_Push32(hero);

			reg_ax = bargain(Real2Host(hero), items,
					price, percent, (Bit8s)mod_init);

			D1_LOG("bargain(%s,%d,%d,%d,%d);\n",
				schick_getCharname(hero),
				items, price, percent, mod_init);

			return 1;
		}
		case 0x2a: {
			Bit16s hero_pos = CPU_Pop16();
			Bit16s bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(hero_pos);

			reg_ax = GUI_use_talent(hero_pos, (Bit8s)bonus);
			D1_LOG("GUI_use_talent(%d, %d) = %d\n",
				hero_pos, (Bit8s)bonus, (Bit16s)reg_ax);
			return 1;
		}
		case 0x34: {
			RealPt hero = CPU_Pop32();
			Bit16s flag = CPU_Pop16();
			CPU_Push16(flag);
			CPU_Push32(hero);

			reg_ax = LVL_select_talent(Real2Host(hero), flag);

			D1_LOG("LVL_select_talent(%s, %d) = %d\n",
					schick_getCharname(hero), flag, reg_ax);
			return 1;
		}
		case 0x43: {
			Bit16s bonus = CPU_Pop16();
			RealPt msg = CPU_Pop32();
			CPU_Push32(msg);
			CPU_Push16(bonus);

			reg_ax = GUI_use_talent2((Bit8s)bonus, Real2Host(msg));
			D1_LOG("GUI_use_talent2(%d, %s) = %d\n", bonus, Real2Host(msg), reg_ax);
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg104(unsigned short offs)
{
	D1_LOG("%s:0x%x\n", __func__, offs);
	switch (offs) {
		case 0x20: {
			RealPt hero = CPU_Pop32();
			Bit16s receipe_index = CPU_Pop16();
			Bit16s flag = CPU_Pop16();

			reg_ax = do_alchemy(Real2Host(hero), (Bit8s)receipe_index, flag);
			D1_LOG("do_alchemy(%s, %d, %d) = %d\n",
				Real2Host(hero) + 0x10, (Bit8s)receipe_index, flag, (Bit16s)reg_ax);
			CPU_Push16(flag);
			CPU_Push16(receipe_index);
			CPU_Push32(hero);
			return 1;
		}
		case 0x2f: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);
			reg_ax = plan_alchemy(Real2Host(hero));
			D1_LOG("plan_alchemy(%s) = %d\n", schick_getCharname(hero), reg_ax);
			return 1;
		}
		case 0x39: {
			RealPt healer = CPU_Pop32();
			RealPt patient = CPU_Pop32();
			Bit16s handycap = CPU_Pop16();
			Bit16s flag = CPU_Pop16();
			CPU_Push16(flag);
			CPU_Push16(handycap);
			CPU_Push32(patient);
			CPU_Push32(healer);

			reg_ax = talent_cure_disease(Real2Host(healer),
							Real2Host(patient),
							handycap, flag);

			D1_LOG("talent_cure_disease(%s, %s, %d, %d) = %d\n",
				schick_getCharname(healer),
				schick_getCharname(patient),
				handycap, flag, reg_ax);

			return 1;
		}
		case 0x3e: {
			RealPt retval = get_heaviest_hero();
			D1_LOG("get_heaviest_hero()\n");

			reg_ax = RealOff(retval);
			reg_dx = RealSeg(retval);
			return 1;
		}
		case 0x43: {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = get_hero_weight(Real2Host(hero));
			D1_LOG("get_hero_weight(%s)\n", Real2Host(hero) + 0x10);

			return 1;
		}
		case 0x48: {
			Bit16s skill = CPU_Pop16();
			CPU_Push16(skill);

			reg_ax = get_skilled_hero_pos(skill);
			D1_LOG("get_skilled_hero_index(%s) = %d\n",
				names_skill[skill], reg_ax);

			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
	return 0;
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

			RealPt owner = CPU_Pop32();
			RealPt equipper = CPU_Pop32();
			Bit16u item = CPU_Pop16();
			Bit16s pos_i = CPU_Pop16();
			Bit16s pos_b = CPU_Pop16();
			CPU_Push16(pos_b);
			CPU_Push16(pos_i);
			CPU_Push16(item);
			CPU_Push32(equipper);
			CPU_Push32(owner);

			D1_LOG("add_equip_boni(%s, %s, %x, %x, %x)\n",
				schick_getCharname(owner),
				schick_getCharname(equipper),
				item, pos_i, pos_b);

			add_equip_boni(Real2Host(owner), Real2Host(equipper),
				item, pos_i, pos_b);

			return 1;
		}
		case 0x2f: {
			RealPt hero = CPU_Pop32();
			Bit16s item = CPU_Pop16();
			Bit16s v2 = CPU_Pop16();
			Bit16s nr = CPU_Pop16();
			CPU_Push16(nr);
			CPU_Push16(v2);
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = give_hero_new_item(Real2Host(hero), item, v2, nr);
			D1_LOG("far new(%s, %s, %d, %d); = %d \n",
				(char*)Real2Host(hero) + 0x10,
				get_itemname(item),
				v2, nr, (signed short)reg_ax);
			return 1;
		}
		case 0x34: {
			RealPt hero = CPU_Pop32();
			signed short pos = CPU_Pop16();
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
			signed short id = CPU_Pop16();
			signed short unused = CPU_Pop16();
			signed short nr = CPU_Pop16();
			CPU_Push16(nr);
			CPU_Push16(unused);
			CPU_Push16(id);

			reg_ax = get_item(id, unused, nr);
			D1_LOG("get_item(%d, %d, %d) = %d;\n",
				id, unused, nr, reg_ax);

			return 1;
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
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);
			reg_ax = select_item_to_drop(Real2Host(hero));
			D1_LOG("select_item_to_drop(%s) = %d\n",
				schick_getCharname(hero), reg_ax);
			return 1;
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
			RealPt hero = CPU_Pop32();
			Bit16s per = CPU_Pop16();
			RealPt text = CPU_Pop32();
			CPU_Push32(text);
			CPU_Push16(per);
			CPU_Push32(hero);
			D1_LOG("loose_random_item()\n");
			loose_random_item(Real2Host(hero), per, Real2Host(text));
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg106(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		Bit16s pos1 = CPU_Pop16();
		Bit16s pos2 = CPU_Pop16();
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);
		CPU_Push16(pos2);
		CPU_Push16(pos1);

		D1_LOG("move_item(%d, %d, %s);\n",
			pos1, pos2, (char*)Real2Host(hero) + 0x10);

		move_item(pos1, pos2, Real2Host(hero));

		return 1;
	}
	case 0x25: {
		RealPt hero = CPU_Pop32();
		Bit16s pos = CPU_Pop16();
		CPU_Push16(pos);
		CPU_Push32(hero);
		D1_LOG("print_item_description(%s, %d);\n",
				(char*)Real2Host(hero) + 0x10,
				pos);
		print_item_description(Real2Host(hero), pos);
		return 1;
	}
	case 0x2a: {
		RealPt hero1 = CPU_Pop32();
		Bit16s pos1 = CPU_Pop16();
		RealPt hero2 = CPU_Pop32();
		Bit16s pos2 = CPU_Pop16();
		D1_INFO("pass_item(%s, %d, %s, %d);\n",
			schick_getCharname(hero1), pos1,
			schick_getCharname(hero2), pos2);
		pass_item(Real2Host(hero1), pos1, Real2Host(hero2), pos2);
		CPU_Push16(pos2);
		CPU_Push32(hero2);
		CPU_Push16(pos1);
		CPU_Push32(hero1);
		return 1;
	}
	case 0x2f: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		D1_LOG("startup_equipment(%s);\n", schick_getCharname(hero));
		startup_equipment(Real2Host(hero));
		return 1;
	}
	case 0x39: {
		reg_ax = get_max_light_time();
		D1_LOG("get_max_light_time(); == %d\n", (signed short)reg_ax);
		return 1;
	}
	case 0x3e: {
		D1_LOG("equip_belt_ani();\n");
		equip_belt_ani();
		return 1;
	}
	case 0x43: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);
		reg_ax = get_full_waterskin_pos(Real2Host(hero));
		D1_LOG("get_full_waterskin_pos(%s); = %d\n",
			(char*)Real2Host(hero) + 0x10, (signed short)reg_ax);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg107(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		Bit16s item_pos = CPU_Pop16();
		Bit16s hero_pos = CPU_Pop16();
		CPU_Push16(hero_pos);
		CPU_Push16(item_pos);
		D1_LOG("use_item(%d, %d)\n", item_pos, hero_pos);

		use_item(item_pos, hero_pos);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg108(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		RealPt owner = CPU_Pop32();
		RealPt consumer = CPU_Pop32();
		Bit16s pos = CPU_Pop16();
		CPU_Push16(pos);
		CPU_Push32(consumer);
		CPU_Push32(owner);
		D1_LOG("consume(%s, %s, %d);\n",
			(char*)Real2Host(owner) + 0x10,
			(char*)Real2Host(consumer) + 0x10,
			pos);
		consume(Real2Host(owner), Real2Host(consumer), pos);
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
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
		case 0x57: {
			tevent_002();
			return 1;
		}
		case 0xa2: {
			unsigned short tevent = CPU_Pop16();
			CPU_Push16(tevent);

			D1_INFO("Reisebegegnung %d\n", tevent);
			return 0;
		}
		case 0xb1: {
			reg_ax = enter_hut_question();
			D1_LOG("enter_hut_question(); = %d\n",
				(signed short)reg_ax);
			return 1;
		}
		default:
			return 0;
	}
}

static int seg111(unsigned short offs)
{
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
		tevent_062();
		return 1;
	}
	case 0x34: {
		tevent_063();
		return 1;
	}
	case 0x39: {
		return 0;
	}
	case 0x3e: {
		tevent_065();
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
		tevent_055();
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
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
		return 0;
	}
}

static int seg113(unsigned short offs) {
	switch (offs) {
	case 0x20: {
		RealPt hero = CPU_Pop32();
		Bit16s idx = CPU_Pop16();
		Bit16s arg2 = CPU_Pop16();
		CPU_Push16(arg2);
		CPU_Push16(idx);
		CPU_Push32(hero);
		D1_LOG("hero_disappear(%s, %d, %d);\n",
			Real2Host(hero) + 0x10, idx, (signed char)arg2);

		hero_disappear(Real2Host(hero), idx, (signed char)arg2);
		return 1;
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
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}


static int seg117(unsigned short offs)
{
	switch (offs) {
	case 0x34: {
		D1_INFO("do_octopus_attack();\n");
		return 0;
	}
	case 0x43: {
		D1_INFO("do_pirate_attack();\n");
		return 0;
	}
	case 0x48: {
		D1_INFO("wilderness_events();\n");
		return 0;
	}
	case 0x4d: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x52: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x57: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x5c: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	case 0x61: {
		D1_LOG("%s:0x%x()\n", __func__, offs);
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg119(unsigned short offs)
{
	switch (offs) {
	case 0x20: {
		D1_LOG("disease_effect();\n");
		disease_effect();
		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
		exit(1);
	}
}

static int seg120(unsigned short offs) {
	switch (offs) {
		case 0x20: {
			reg_ax = init_memory();
			D1_LOG("init_memory() = %d\n", reg_ax);
			return 1;
		}
		case 0x25: {
			D1_LOG("init_game_state()\n");
			init_game_state();
			return 1;
		}
		case 0x2a: {
			D1_LOG("prepare_dirs()\n");
			prepare_dirs();
			return 1;
		}
		case 0x2f: {
			D1_LOG("cleanup_game()\n");
			cleanup_game();
			return 1;
		}
		case 0x34: {
			RealPt hero = CPU_Pop32();
			Bit16s hero_pos = CPU_Pop16();
			CPU_Push16(hero_pos);
			CPU_Push32(hero);

			rabies(hero, hero_pos);

			return 1;
		}
		case 0x3e: {
			D1_LOG("refresh_colors();\n");
			refresh_colors();
			return 1;
		}
		case 0x43: {
			Bit32s retval = get_diskspace();
			D1_LOG("get_diskspace() = %d\n", retval);
			reg_ax = (Bit16s)retval;
			reg_dx = (Bit16s)(retval >> 16);
			return 1;
		}
		case 0x48: {
			return 0;
		}
		case 0x4d: {
			D1_LOG("game_over_screen()\n");
			game_over_screen();
			return 1;
		}
		case 0x52: {
			D1_LOG("call_gen()\n");
			call_gen();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

static int seg121(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("poison_effect()\n");
			poison_effect();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

/*
	In seg122 is only an empty function.
	We bypass it and avoid unneccesarry overlay magic.
*/
static int seg122(unsigned short offs)
{
	switch (offs) {
		case 0x20: {
			D1_LOG("do_location1()\n");
			do_location1();
			return 1;
		}
		default:
			D1_ERR("Uncatched call to Segment %s:0x%04x\n",	__func__, offs);
			exit(1);
	}
}

// Intercept far CALLs (both 32 and 16 bit)
int schick_farcall_v302de(unsigned segm, unsigned offs)
{
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
		case 0x1042:	return seg011(offs);
		case 0x1112:	return seg012(offs);
		case 0x12db:	return seg024(offs);
		case 0x12de:	return seg025(offs);
		case 0x12e5:	return seg026(offs);
		case 0x12ec:	return seg027(offs);
		case 0x12f1:	return seg028(offs);
		case 0x12f9:	return seg029(offs);
		case 0x12ff:	return seg030(offs);
		case 0x1303:	return seg031(offs);
		case 0x1309:	return seg032(offs);
		case 0x130f:	return seg033(offs);
		case 0x1312:	return seg034(offs);
		case 0x1316:	return seg035(offs);
		case 0x131a:	return seg036(offs);
		case 0x131f:	return seg037(offs);
		case 0x1324:	return seg038(offs);
		case 0x1328:	return seg039(offs);
		case 0x132d:	return seg040(offs);
		case 0x1330:	return seg041(offs);
		case 0x1335:	return seg042(offs);
		case 0x1338:	return seg043(offs);
		case 0x133b:	return seg044(offs);
		case 0x133f:	return seg045(offs);
		case 0x1344:	return seg046(offs);
		case 0x1348:	return seg047(offs);
		case 0x1350:	return seg048(offs);
		case 0x1353:	return seg049(offs);
		case 0x1358:	return seg050(offs);
		case 0x135c:	return 0;
		case 0x135f:	return seg052(offs);
		case 0x1362:	return seg053(offs);
		case 0x1365:	return seg054(offs);
		case 0x1369:	return seg055(offs);
		case 0x136d:	return seg056(offs);
		case 0x1370:	return seg057(offs);
		case 0x1373:	return seg058(offs);
		case 0x1377:	return 0;
		case 0x137b:	return 0;
		case 0x137e:	return seg061(offs);
		case 0x1383:	return seg062(offs);
		case 0x1386:	return seg063(offs);
		case 0x138a:	return seg064(offs);
		case 0x138e:	return 0;
		case 0x1392:	return seg066(offs);
		case 0x139a:	return seg067(offs);
		case 0x13a1:	return seg068(offs);
		case 0x13a8:	return seg069(offs);
		case 0x13ac:	return seg070(offs);
		case 0x13b1:	return seg071(offs);
		case 0x13b4:	return seg072(offs);
		case 0x13b9:	return seg073(offs);
		case 0x13bd:	return seg074(offs);
		case 0x13c3:	return seg075(offs);
		case 0x13cb:	return seg076(offs);
		case 0x13d1:	return seg077(offs);
		case 0x13d7:	return seg078(offs);
		case 0x13dd:	return 0;
		case 0x13e4:	return seg080(offs);
		case 0x13e9:	return seg081(offs);
		case 0x13ed:	return 0;
		case 0x13f0:	return 0;
		case 0x13f6:	return seg084(offs);
		case 0x13fc:	return 0;
		case 0x1401:	return seg086(offs);
		case 0x1408:	return 0;
		case 0x140b:	return seg088(offs);
		case 0x1411:	return 0;
		case 0x1417:	return seg090(offs);
		case 0x141b:	return seg091(offs);
		case 0x1420:	return seg092(offs);
		case 0x1429:	return 0;
		case 0x142c:	return seg094(offs);
		case 0x1432:	return seg095(offs);
		case 0x1438:	return seg096(offs);
		case 0x1442:	return seg097(offs);
		case 0x1449:	return seg098(offs);
		case 0x144f:	return seg099(offs);
		case 0x145e:	return seg100(offs);
		case 0x1467:	return seg101(offs);
		case 0x1472:	return seg102(offs);
		case 0x147b:	return seg103(offs);
		case 0x1480:	return seg104(offs);
		case 0x1485:	return seg105(offs);
		case 0x148c:	return seg106(offs);
		case 0x1491:	return seg107(offs);
		case 0x1498:	return seg108(offs);
		case 0x149b:	return seg109(offs);
		case 0x14a7:	return 0;
		case 0x14b4:	return seg111(offs);
		case 0x14bb:	return 0;
		case 0x14c2:	return seg113(offs);
		case 0x14cb:	return 0;
		case 0x14d1:	return 0;
		case 0x14d8:	return 0;
		case 0x14e0:	return seg117(offs);
		case 0x14e7:	return 0;
		case 0x14ed:	return seg119(offs);
		case 0x14f0:	return seg120(offs);
		case 0x14f6:	return seg121(offs);
		case 0x14f9:	return seg122(offs);
		default:
			D1_TRAC("Unfetched Segment: 0x%04x\n", segm);
			return 0;
	}
}

int schick_nearcall_v302de(unsigned offs)
{

	Bit16u segm = SegValue(cs) - reloc_game;

	int retval = 0;

	Bit16u CS = CPU_Pop16();

	/* Borland C-Lib */
	if (segm == 0) retval = n_seg000(offs);
	else if (segm == 0x4ac) retval = n_seg001(offs);
	else if (segm == 0x51e) retval = n_seg002(offs);
	else if (segm == 0xb2a) retval = n_seg004(offs);
	else if (segm == 0xc85) retval = n_seg005(offs);
	else if (segm == 0xe41) retval = n_seg006(offs);
	else if (is_ovrseg(0x12db)) retval = n_seg024(offs);
	else if (is_ovrseg(0x12de)) retval = n_seg025(offs);
	else if (is_ovrseg(0x12e5)) retval = n_seg026(offs);
	else if (is_ovrseg(0x12f1)) retval = n_seg028(offs);
	else if (is_ovrseg(0x12f9)) retval = n_seg029(offs);
	else if (is_ovrseg(0x12ff)) retval = n_seg030(offs);
	else if (is_ovrseg(0x1303)) retval = n_seg031(offs);
	else if (is_ovrseg(0x1309)) retval = n_seg032(offs);
	else if (is_ovrseg(0x1312)) retval = n_seg034(offs);
	else if (is_ovrseg(0x131a)) retval = n_seg036(offs);
	else if (is_ovrseg(0x131f)) retval = n_seg037(offs);
	else if (is_ovrseg(0x1324)) retval = n_seg038(offs);
	else if (is_ovrseg(0x1328)) retval = n_seg039(offs);
	else if (is_ovrseg(0x1330)) retval = n_seg041(offs);
	else if (is_ovrseg(0x133b)) retval = n_seg044(offs);
	else if (is_ovrseg(0x133f)) retval = n_seg045(offs);
	else if (is_ovrseg(0x1344)) retval = n_seg046(offs);
	else if (is_ovrseg(0x1350)) retval = n_seg048(offs);
	else if (is_ovrseg(0x1353)) retval = n_seg049(offs);
	else if (is_ovrseg(0x1358)) retval = n_seg050(offs);
	else if (is_ovrseg(0x1362)) retval = n_seg053(offs);
	else if (is_ovrseg(0x1365)) retval = n_seg054(offs);
	else if (is_ovrseg(0x1369)) retval = n_seg055(offs);
	else if (is_ovrseg(0x1373)) retval = n_seg058(offs);
	else if (is_ovrseg(0x137e)) retval = n_seg061(offs);
	else if (is_ovrseg(0x1386)) retval = n_seg063(offs);
	else if (is_ovrseg(0x138a)) retval = n_seg064(offs);
	else if (is_ovrseg(0x1392)) retval = n_seg066(offs);
	else if (is_ovrseg(0x139a)) retval = n_seg067(offs);
	else if (is_ovrseg(0x13b4)) retval = n_seg072(offs);
	else if (is_ovrseg(0x13bd)) retval = n_seg074(offs);
	else if (is_ovrseg(0x13c3)) retval = n_seg075(offs);
	else if (is_ovrseg(0x13cb)) retval = n_seg076(offs);
	else if (is_ovrseg(0x1417)) retval = n_seg090(offs);
	else if (is_ovrseg(0x141b)) retval = n_seg091(offs);
	else if (is_ovrseg(0x1420)) retval = n_seg092(offs);
	else if (is_ovrseg(0x142c)) retval = n_seg094(offs);
	else if (is_ovrseg(0x1432)) retval = n_seg095(offs);
	else if (is_ovrseg(0x1442)) retval = n_seg097(offs);
	else if (is_ovrseg(0x1449)) retval = n_seg098(offs);
	else if (is_ovrseg(0x1472)) retval = n_seg102(offs);
	else if (is_ovrseg(0x147b)) retval = n_seg103(offs);
	else if (is_ovrseg(0x1480)) retval = n_seg104(offs);
	else if (is_ovrseg(0x1485)) retval = n_seg105(offs);
	else if (is_ovrseg(0x148c)) retval = n_seg106(offs);
	else if (is_ovrseg(0x14c2)) retval = n_seg113(offs);
	else if (is_ovrseg(0x14e0)) retval = n_seg117(offs);
	else if (is_ovrseg(0x14f0)) retval = n_seg120(offs);

	if (retval == 0) CPU_Push16(CS);

	return retval;
}
