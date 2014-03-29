#include <stdlib.h>
#include <string.h>

#include "dosbox.h"

#include "custom.h"
#include "schweif.h"

#include "c102de_seg000.h"
#include "c102de_seg002.h"
#include "c102de_seg013.h"
#include "c102de_seg024.h"
#include "c102de_seg029.h"
#include "c102de_seg033.h"
#include "c102de_seg034.h"
#include "c102de_seg037.h"
//#include "c102de_seg041.h"
#include "c102de_seg043.h"
#include "c102de_seg136.h"

using namespace C102de;

static int seg002(unsigned short offs)
{
	switch (offs) {
	case 0xf9: {
		RealPt pal = CPU_Pop32();
		Bit16s first_col = CPU_Pop16();
		Bit16s colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(first_col);
		CPU_Push32(pal);
		D2_TRAC("set_palette(%x, %x, %x)\n", pal, first_col, colors);
		return 0;
	}
	case 0x18f: {
		RealPt pal = CPU_Pop32();
		Bit16s first_col = CPU_Pop16();
		CPU_Push16(first_col);
		CPU_Push32(pal);
		D2_TRAC("set_color(%x, %x)\n", pal, first_col);
		return 0;
	}
	case 0x46f: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_is_diseased(Real2Host(hero));
		D2_TRAC("hero_is_diseased(%s) = %d\n",
			schweif_getCharname(hero), reg_ax);

		return 1;
	}
	case 0x49b: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = hero_is_poisoned(Real2Host(hero));
		D2_TRAC("hero_is_poisoned(%s) = %d\n",
			schweif_getCharname(hero), reg_ax);

		return 1;
	}
	default:
		return 0;
	}
	return 0;
}

static int seg004(unsigned short offs)
{
	switch(offs) {
	case 0x012d: {
		return 0;
	}
	case 0x06da: {
		return 0;
	}
	case 0x0760: {
		return 0;
	}
	case 0x0b70: {
		return 0;
	}
	default:
		//D2_LOG("%s:0x%04x();\n", __func__, offs);
		return 0;
	}
}

static int seg007(unsigned short offs)
{
	switch(offs) {
	default: return 0;
	}
}

static int seg012(unsigned short offs) {
	switch(offs) {
	case 0x0006: {
		return 0;
	}
	case 0x004C: {
		return 0;
	}
	case 0x027C: {
		return 0;
	}
	case 0x069d: { // draw 3d scene
		return 0;
	}
	case 0x0F0D: {
		return 0;
	}
	case 0x105E: {
		return 0;
	}
	case 0x1117: {
		return 0;
	}
	case 0x1131: {
		return 0;
	}
	case 0x1174: {
		return 0;
	}
	case 0x11D2: {
		return 0;
	}
	case 0x121D: {
		return 0;
	}
	case 0x1331: {
		return 0;
	}
	case 0x1396: {
		return 0;
	}
	case 0x13B7: {
		return 0;
	}
	case 0x13FE: {
		return 0;
	}
	case 0x1428: {
		return 0;
	}
	case 0x1467: {
		return 0;
	}
	default: {
		D2_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg013(unsigned short offs)
{
	switch (offs) {
	case 0x006: {
		return 0;
	}
	case 0x1bf: {
		return 0;
	}
	case 0x7af: {
		return 0;
	}
	case 0x91d: {
		Bit16s a = CPU_Pop16();
		Bit16s b = CPU_Pop16();
		CPU_Push16(b);
		CPU_Push16(a);
		D2_GFX("seg013_91d(%d, %d);\n", a, b);
		seg013_91d(a, b);
		return 1;
	}
	case 0x95f: {
		return 0;
	}
	case 0xb10: {
		return 0;
	}
	case 0xc7d: {
		return 1;
	}
	case 0xf8e: {
		return 0;
	}
	default: {
		D2_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
	return 0;
}

#if 1
static int seg016(unsigned short offs) {
	switch(offs) {
	case 0x000B:{ // decode_image(...)
		D2_LOG("decoding image");
		return 0;
	}
	default: return 0;
	}
}

#endif

static int seg018(unsigned short offs) {
	switch(offs) {
	case 0x057D:{ // keypress_handler: alles außer Cursortasten
		return 0;
	}
	default: return 0;
	}
}
static int firstc=0;
static int seg024(unsigned short offs)
{
	switch (offs) {
	case 0x010: {
		RealPt arg0 = CPU_Pop32(); // dest?
		unsigned short arg4 = CPU_Pop16();
		RealPt arg6 = CPU_Pop32();
		unsigned short argA = CPU_Pop16();
		RealPt argC = CPU_Pop32();  // Mapping-irgendwas.
		RealPt arg10 = CPU_Pop32(); // Mapping-Funktion/Matrix/Whatever.
		unsigned short arg14 = CPU_Pop16();
		CPU_Push16(arg14);
		CPU_Push32(arg10);
		CPU_Push32(argC);
		CPU_Push16(argA);
		CPU_Push32(arg6);
		CPU_Push16(arg4);
		CPU_Push32(arg0);
		Bit8u* arg0p = Real2Host(arg0);
		Bit8u* arg6p = Real2Host(arg6);
		Bit8u* argCp = Real2Host(argC);
		Bit8u* arg10p= Real2Host(arg10);
		//D2_GFX("bresenham?0: %08x, %04x, %08x, %04x, %08x, %08x, %04x\n", *arg0p, arg4, *arg6p, argA, *argCp, *arg10p, arg14);
		return 0;
	}
	case 0x25A: {
		// Malt scheinbar eine Linie mit irgendwelchen Daten in spezielle Puffer,
		// um dem Texturierer anzuzeigen, wo er ausmalen soll.
		RealPt arg0 = CPU_Pop32(); // y0?
		unsigned short arg4 = CPU_Pop16();
		RealPt arg6 = CPU_Pop32(); // y1?
		unsigned short argA = CPU_Pop16();
		RealPt argC = CPU_Pop32(); // x0?
		RealPt arg10 = CPU_Pop32(); // x1?
		unsigned short arg14 = CPU_Pop16();
		CPU_Push16(arg14);
		CPU_Push32(arg10);
		CPU_Push32(argC);
		CPU_Push16(argA);
		CPU_Push32(arg6);
		CPU_Push16(arg4);
		CPU_Push32(arg0);
		Bit8u* arg0p = Real2Host(arg0);
		Bit8u* arg0p2 = Real2Host(arg0+2);
		Bit8u* arg6p = Real2Host(arg6);
		Bit8u* arg6p2 = Real2Host(arg6+2);
		Bit8u* argCp = Real2Host(argC);
		Bit8u* arg10p= Real2Host(arg10);
		/*Bit8u* buffer1 = MemBase+PhysMake(datseg, 0x78DA);
		memset(buffer1, *arg0p2, 0x1A0);
		u.s.w.*/
		return 0;
	}
	case 0x4f3: {
		// Texturiere 3D-Bild: 
		unsigned short arg0 = CPU_Pop16(); // flags?
		RealPt arg2 = CPU_Pop32();
		RealPt arg6 = CPU_Pop32();
		RealPt argA = CPU_Pop32(); // source?
		unsigned short horizontal = CPU_Pop16();
		CPU_Push16(horizontal);
		CPU_Push32(argA);
		CPU_Push32(arg6);
		CPU_Push32(arg2);
		CPU_Push16(arg0);
		//D2_GFX("fillOtherFloor: %d, %04x, %04x, %04x, %d\n", arg0, arg2, arg6, argA, horizontal);
		return 0;
	}
	case 0x560: {
		// Texturiere 3D-Bild: Kästchen, auf dem die Party gerade steht
		unsigned short arg0 = CPU_Pop16(); // flags?
		RealPt arg2 = CPU_Pop32(); // source?
		RealPt dest = CPU_Pop32();
		RealPt argA = CPU_Pop32(); // source?
		unsigned short horizontal = CPU_Pop16();
		CPU_Push16(horizontal);
		CPU_Push32(argA);
		CPU_Push32(dest);
		CPU_Push32(arg2);
		CPU_Push16(arg0);
		firstc = 0;
		/*
		RealPt si = arg2;
		arg0 *= 2;
		unsigned short bx = 0;
		do {
		    unsigned short di = bx;
		    bx += 2;
		    if (bx == arg0) bx = 0;
		    si = bx;
		    texturize3d((dest+di*8), (dest+arg2*8+4),
				(dest+si*8), (dest+bx+si*8+4),
				argA+di*2, argA+si*2, horizontal);
		} while (reg_bx != 0);
		*/
		
		//D2_GFX("fillCurrentFloor: %d, %04x, %04x, %04x, %d\n", arg0, arg2, dest, argA, horizontal);
		//D2_GFX("tex(%0x, %0x, %0x, %0x, ...);\n", deref32(argA), deref32(argA+4), deref32(argA+8), deref32(argA+12));
		//memcpy(Real2Host(dest), Real2Host(argA), 100);
		return 0;
	}
	case 0x5cd: {
		fill_smth();
		return 1;
	}
	case 0x5f6: {
		Bit16u val = CPU_Pop16();
		CPU_Push16(val);
		fill_scene((unsigned char)val);
		return 1;
	}
	case 0x627: {
		copy_3d_to_screen();
		return 1;
	}
	case 0x668: {
		// Texturiert Häuser.
		return 0;
	}
	case 0x198a: {
		// Texturiert Bäume, Sträucher u.ä.
		return 0;
	}
	case 0x2fce: {
		// Texturiert den Boden.
		unsigned short arg0 = CPU_Pop16();
		CPU_Push16(arg0);
		Bit8u* sceneptr = Real2Host(ds_readd(0xB128));
		//Bit8u* texture  = MemBase+PhysMake(datseg, 0x7A7A);//Real2Host(0x85DA);
		//unsigned i = 0;
		//while(ds_readw(0x7A7A + 2*i) == 0x8001) { i++; }
		//D2_GFX("texBoden(%04x): cp from %x\n", arg0, i);
		//Bit8u* texture  = Real2Host(ds_readd(0x7A7A + 2*(i+1)));
		//memcpy(sceneptr, texture, 268*134);
		return 0;
	}
	case 0x4b29: {
		// Texturiert u.a. Wurzelwerk von Bäumen.
		return 0;
	}
	case 0x6c35: {
		// Keine Ahnung, was diese Funktion macht.
		return 0;
	}
	default: {
		D2_ERR("uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
	}
	}
}

static int seg029(unsigned short offs)
{
	switch (offs) {
	case 0x0e: {
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		Bit32u length = CPU_Pop32();
		CPU_Push32(length);
		CPU_Push32(src);
		CPU_Push32(dst);
		decomp_pp20(Real2Host(dst), Real2Host(src), length);
		if (custom_oldIP != 0x0F51) return 1;
		else return 0;
	}
	default: return 0;
	}
}

static int seg033(unsigned short offs)
{
	switch (offs) {
	case 0x0e: {
		RealPt src = CPU_Pop32();
		RealPt dst = CPU_Pop32();
		RealPt src_data = CPU_Pop32();
		Bit32u length = CPU_Pop32();
		CPU_Push32(length);
		CPU_Push32(src_data);
		CPU_Push32(dst);
		CPU_Push32(src);
		//D2_LOG("ppDecrunch(%x, %x, %x, %x)\n",
		//	src, dst, src_data, length);
		return 0;
	}
	default: return 0;
	}
}

static int seg034(unsigned short offs)
{
	switch (offs) {
	case 0x08: { // fill_line
		RealPt dst = CPU_Pop32();
		Bit16u count = CPU_Pop16();
		Bit16s color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(count);
		CPU_Push32(dst);
		//D2_GFX("fill_line(0x%x, %d, 0x%x);\n", dst, count, (signed char)color);
		fill_line(Real2Phys(dst), count, (signed char)color);
		return 1;
	}
	case 0x22: { // copyVideoLine2Buffer
		Bit16u length = CPU_Pop16();
		RealPt src = CPU_Pop32();
		CPU_Push32(src);
		CPU_Push16(length);
		D2_GFX("seg034_22(%x, %x)\n", length, src);
		return 0;
	}
	case 0x41: { // copyBuffer2VideoLine
		D2_GFX("copyBuffer2VideoLine\n");
		return 0;
	}
	case 0x60: { // copyBuffer2VideoMem
		D2_GFX("copyBuffer2VideoMem\n");
		return 0;
	}
	case 0x7f: { // fill_v_line
		RealPt dst = CPU_Pop32();
		Bit16u height = CPU_Pop16();
		Bit16s color = CPU_Pop16();
		Bit16u disp = CPU_Pop16();
		CPU_Push16(disp);
		CPU_Push16(color);
		CPU_Push16(height);
		CPU_Push32(dst);
		//D2_GFX("fill_v_line(0x%08x, %d, 0x%08x, %d);\n", dst, height, color, disp);
		fill_v_line(Real2Phys(dst), height, (signed char)color, disp);
		return 1;
	}
	case 0x9f: { // copyVideoLineVert2Buf
		D2_GFX("copyVideoLineVert2Buf\n");
		return 0;
	}
	case 0xc4: { // copyBuffer2VideoLineVert
		D2_GFX("copyBuffer2VideoLineVert\n");
		return 0;
	}
	case 0xe9: { // copyBuffer2VideoMemVert
		D2_GFX("copyBuffer2VideoMemVert\n");
		return 0;
	}
	default: {
		D2_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg037(unsigned short offs)
{
	switch(offs) {
	case 0x05: {
		Bit16u lowerBound = CPU_Pop16();
		Bit16u upperBound = CPU_Pop16();
		CPU_Push16(upperBound);
		CPU_Push16(lowerBound);
		reg_ax = random_interval(lowerBound, upperBound);
		D2_INFO("randomInterval(%d, %d) = %d\n",
		       lowerBound, upperBound, reg_ax);
		return 1;
	}
	case 0x25: {
		Bit16u range = CPU_Pop16();
		CPU_Push16(range);
		reg_ax = random_schweif(range);
		D2_INFO("random(%d) = %d\n", range & 0xFF, reg_ax);
		return 1;
	}
	case 0x73: {
		Bit16u n = CPU_Pop16();
		Bit16u m = CPU_Pop16();
		Bit16s x = CPU_Pop16();
		CPU_Push16(x);
		CPU_Push16(m);
		CPU_Push16(n);
		reg_ax = dice_roll(n, m, x);
		D2_INFO("wuerfel %dW%d%+d = %d\n", n, m, x, reg_ax);
		return 1;
	}
	case 0xa2: {
		Bit16u n = CPU_Pop16();
		Bit16u m = CPU_Pop16();
		Bit16s x = CPU_Pop16();
		RealPt min = CPU_Pop32();
		RealPt max = CPU_Pop32();
		CPU_Push32(max);
		CPU_Push32(min);
		CPU_Push16(x);
		CPU_Push16(m);
		CPU_Push16(n);
		calc_damage_range(n, m, x, Real2Host(min),
				  Real2Host(max));
		return 1;
	}
	}
	return 0;
}

/* XMS stuff */
static int seg039(unsigned short offs)
{
	return 0;
}

static int seg041(unsigned short offs) {
    switch(offs) {
    case 0x46: { // still unknown.
	return 0;
    }
    case 0xD0: {
	//CPU_Pop16();
	RealPt ptr = CPU_Pop32();
	CPU_Push32(ptr);
	//process_nvf((struct nvf_desc*)MemBase + Real2Phys(ptr));
	return 0;
    }
    case 0x715: { // process_nvf2
	return 0;
    }
    }
    return 0;
}

static int seg043(unsigned short offs)
{
	switch (offs) {
	case 0x0c: {
		Bit16u mode = CPU_Pop16();
		CPU_Push16(mode);
		D2_LOG("set_video_mode(0x%x);\n", mode);
		set_video_mode((unsigned char)mode);
		return 1;
	}
	case 0x24: {
		Bit16u page = CPU_Pop16();
		CPU_Push16(page);
		D2_LOG("set_display_page(0x%x);\n", page);
		set_display_page((unsigned char)page);
		return 1;
	}
	case 0x3c: {
		return 0;
	}
	case 0x72: {
		return 0;
	}
	case 0xf2: {
		return 0;
	}
	case 0x111: {
		return 0;
	}
	case 0x130: {
		return 0;
	}
	default:
		D2_ERR("Uncatched call to %s:0x%x()\n", __func__, offs);
		exit(1);
	}

	return 0;

}

/* Graphics drawing stuff */
static int seg046(unsigned short offs)
{
	switch(offs) {
	case 0x000C: return 0;
	case 0x01BD: return 0;//redraw stuff under cursor
	case 0x023E: return 0;//subfunction of 000c
	default:     return 0;
	}
}

static int seg136(unsigned short offs)
{
	switch(offs) {
#if 0
	case 0x4D:{
		D2_LOG("Probe-Basis\n");
		return 0;
	}
#endif
	case 0x48: {
		RealPt hero = CPU_Pop32();
		Bit16u attrib = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(attrib);
		CPU_Push32(hero);
		reg_ax = test_attrib(Real2Host(hero), attrib, bonus);
		return 1;
	}
	case 0x61:{
		RealPt hero = CPU_Pop32();
		Bit16u skill = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(skill);
		CPU_Push32(hero);
		D2_INFO("Talentprobe %s auf %s %+d\n",
		       schweif_getCharname(hero),
		       schweif_common::names_skill[skill],
		       (signed char)bonus);
		return 0;
	}
	default:
		return 0;
	}
}

static int seg134(unsigned short offs)
{
	switch (offs) {
	case 0x3e: {
		RealPt hero = CPU_Pop32();
		Bit16s poison = CPU_Pop16();

		D2_INFO("%s wurde mit Gift %d vergiftet\n",
			schweif_getCharname(hero), poison);

		CPU_Push16(poison);
		CPU_Push32(hero);
		return 0;
	}
	case 0x43: {
		RealPt hero = CPU_Pop32();
		Bit16s disease = CPU_Pop16();

		D2_INFO("%s erkrankt an Krankheit %d\n",
			schweif_getCharname(hero), disease);

		CPU_Push16(disease);
		CPU_Push32(hero);
		return 0;
	}
	case 0x48: {
		RealPt hero = CPU_Pop32();
		Bit16s disease = CPU_Pop16();
		Bit16s chance = CPU_Pop16();

		D2_INFO("%s erkrankt mit Wahrscheinlichkeit von %d%% an Krankheit %d\n",
			schweif_getCharname(hero), disease, chance);

		CPU_Push16(chance);
		CPU_Push16(disease);
		CPU_Push32(hero);

		return 0;
	}
	default:
		return 0;
	}
}

static int seg151(unsigned short offs)
{
	switch (offs) {
	case 0x2a: {
		RealPt hero = CPU_Pop32();
		Bit16u spell = CPU_Pop16();
		Bit16s bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(spell);
		CPU_Push32(hero);
		D2_INFO("Zauberprobe %s auf %s %+d\n",
		       schweif_getCharname(hero),
		       schweif_common::names_spell[spell],
		       (signed char)bonus);
		return 0;
	}
	default: return 0;
	}
}

static int ovr249(unsigned short offs)
{
	switch(offs) {
	case 0xB7D: {
		RealPt arg00 = CPU_Pop32();
		RealPt arg04 = CPU_Pop32();
		RealPt arg08 = CPU_Pop32();
		RealPt dialog_text = CPU_Pop32();
		RealPt option_count_ptr = CPU_Pop32();
		RealPt option_ptr = CPU_Pop32();
		CPU_Push32(option_ptr);
		CPU_Push32(option_count_ptr);
		CPU_Push32(dialog_text);
		CPU_Push32(arg08);
		CPU_Push32(arg04);
		CPU_Push32(arg00);

		RealPt* options = (RealPt*)Real2Host(option_ptr);
		char* option_count = (char*)Real2Host(option_count_ptr);
		D2_TRAC("Dialog mit %d Optionen: %s, Parameter: %x, %x, %x\n",
		       *option_count,
		       (char*)Real2Host(dialog_text),
		       Real2Host(arg00)[0], arg04, arg08);
		for (int i=0;  i < *option_count;  i++)
		    D2_TRAC("- %s\n", Real2Host(options[i]));
		return 0;
	}
	default: return 0;
	}
}

static int ovr265(unsigned short offs)
{
	switch(offs) {
	case 0x0F3D: {
		RealPt dialog_args = CPU_Pop32();
		Bit16u textfield_size = CPU_Pop16();
		RealPt dialog_string = CPU_Pop32();
		CPU_Push32(dialog_string);
		CPU_Push16(textfield_size);
		CPU_Push32(dialog_args);
		unsigned short* dargs = (unsigned short*)Real2Host(dialog_args);
		D2_TRAC("Dialog mit Textfeld(len:%d): %x, %x, %x : %s\n",
		       textfield_size,
		       dargs[0], dargs[1], dargs[2],
		       (char*)Real2Host(dialog_string));
		return 0;
	}
	case 0x0DDB: {
		RealPt zeilen = CPU_Pop32();
		RealPt titel  = CPU_Pop32();
		Bit16u arg8   = CPU_Pop16();
		Bit16u argA   = CPU_Pop16();
		CPU_Push16(argA);
		CPU_Push16(arg8);
		CPU_Push32(titel);
		CPU_Push32(zeilen);
		D2_TRAC("SubDialog: %d Zeilen, Titel: %s, Optionen %d, %d\n",
		       Real2Host(zeilen)[0], (char*)Real2Host(titel), arg8, argA);
		return 0;
	}
	default: return 0;
	}
}

static int stub249(unsigned short offs)
{
	switch(offs) {
	case 0x0020: return ovr249(0x0B6A);
	case 0x0025: return ovr249(0x033A);
	case 0x002A: return ovr249(0x0449);
	case 0x002F: return ovr249(0x0000);
	case 0x0034: return ovr249(0x00C4);
	case 0x0039: return ovr249(0x0269);
	case 0x003E: return ovr249(0x0B7D);
	case 0x0043: return ovr249(0x103B);
	case 0x0048: return ovr249(0x119F);
	case 0x004D: return ovr249(0x149A);
	default:
		return 0;
	}
}

static int stub250(unsigned short offs) // Todo
{
	switch(offs) {
	case 0x0070: {
		return 0;
	}
	default: return 0;
	}
}

static int stub265(unsigned short offs)
{
	switch(offs) {
	case 0x0020: return ovr265(0x01C5);
	case 0x0025: return ovr265(0x0000);
	case 0x002A: return ovr265(0x0087);
	case 0x002F: return ovr265(0x04F4);
	case 0x0034: return ovr265(0x0559);
	case 0x0039: return ovr265(0x06DA);
	case 0x003E: return ovr265(0x05C3);
	case 0x0043: return ovr265(0x0634);
	case 0x0048: return ovr265(0x0D35);
	case 0x004D: return ovr265(0x0DDB);
	case 0x0052: return ovr265(0x0270);
	case 0x0057: return ovr265(0x0F3D);
	case 0x005C: return ovr265(0x11DD);
	case 0x0061: return ovr265(0x0A37);
	case 0x0066: return ovr265(0x0A4D);
	case 0x006B: return ovr265(0x0D27);
	default:
		return 0;
	}
}


static int segUnk(unsigned segm, unsigned short offs)
{
	switch(offs) {
		//case 0x003e: printf("call at %04x:%04x\n", segm, offs);
	default: return 0;
	}
}

static int segUnk_near(unsigned segm, unsigned short offs)
{
	switch(offs) {
		//case 0x0DDB: printf("call at %04x:%04x\n", segm, offs);
	default: return 0;
	}
}

// Intercept far CALLs (both 32 and 16 bit)
int schweif_farcall_c102de(unsigned segm, unsigned offs)
{
	switch (segm) {
	case 0x0000: return seg000(offs);
	case 0x053a: return seg002(offs);
	case 0x06bd: return seg004(offs);
	case 0x0a32: return seg007(offs);
	case 0x0b97: return seg012(offs);
	case 0x0ce1: return seg013(offs);
	case 0x0ec6: return seg018(offs);
	case 0x1288: return seg024(offs);
	case 0x1a8a: return seg029(offs);
	case 0x1aa4: return seg033(offs);
	case 0x1ae4: return seg034(offs);
	case 0x1b27: return seg037(offs);
	case 0x1b42: return seg039(offs);
	case 0x1c8a: return seg043(offs);
	case 0x1cce: return seg046(offs);
	case 0x20ad: return seg134(offs);
	case 0x20be: return seg136(offs);
	case 0x2119: return seg151(offs);
	case 0x2313: return stub249(offs);
	case 0x2319: return stub250(offs);
	case 0x2383: return stub265(offs);
		//case 0x33a3: return ovr265(offs); break;
	default:
		return segUnk(segm, offs);
	}
}

int schweif_nearcall_c102de(unsigned offs)
{
	unsigned short segm = SegValue(cs)-relocation;
	int ret = 0;
	unsigned short temp = CPU_Pop16();

	switch (segm) {
	case 0x0000: ret = seg000(offs); break;
	case 0x06bd: ret = seg004(offs); break;
	case 0x0a32: ret = seg007(offs); break;
	case 0x0ce1: ret = seg013(offs); break;
	case 0x0ec6: ret = seg018(offs); break;
	case 0x1288: ret = seg024(offs); break;
	case 0x1a8a: ret = seg029(offs); break;
	case 0x1aa4: ret = seg033(offs); break;
	case 0x1ae4: ret = seg034(offs); break;
	case 0x1b27: ret = seg037(offs); break;
	case 0x1b42: ret = seg039(offs); break;
	case 0x1cce: ret = seg046(offs); break;
	case 0x20be: ret = seg136(offs); break;
	case 0x2119: ret = seg151(offs); break;
		//case 0x33a3: ret = ovr265(offs); break;
	default:     ret = segUnk_near(segm, offs); break;
	}

	if (!ret) CPU_Push16(temp);
	
	return ret;
}
