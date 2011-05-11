#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "schick.h"

// Is the game running?
static int schick = 0;
// Is gen called from the game?
static int gen=0;
//Has the game called gen?
static int fromgame = 0;
// Segment relocation
unsigned short relocation;

//Segment relocation of the game
Bitu reloc_game;
//Segment relocation of the generator
Bitu reloc_gen;

//Datasegment
unsigned short datseg = 0;
Bit8u *p_datseg = NULL;
//Datesegment relocation from game if gen is called
unsigned short datseg_bak = 0;
Bit8u *p_datseg_bak = NULL;

static short schick_en = 0;
static short gen_en = 0;

/**
	schick_is_en - returns 1 if the game language is english
*/
int schick_is_en() {
	return schick_en;
}
/**
	schick_gen_is_en - returns 1 if the language of the character generator is english
*/
int schick_gen_is_en() {
	return gen_en;
}

/**
	schick_get_version - returns the version number of the game
	@p:	pointer to the start of the datasegment
*/
int schick_get_version(char *p) {

	/* V3.02_de, the common CD-version */
	if (!strncmp(p + 0x46ec, "V3.02", 6))
		return 302;

	/* V1.00_de, initial floppy version */
	if (!strncmp(p + 0x4529, "V1.00", 6))
		return 100;
	/* V1.04_de, a floppy version */
	if (!strncmp(p + 0x452d, "V1.04", 6))
		return 104;
	/* V1.07_de, last german floppy version taken from PC-Joker 01/2001  */
	if (!strncmp(p + 0x4439, "V1.07", 6))
		return 107;

	/* V3.0x english floppy versions */
	if (!strncmp(p + 0x4512, "V3.00", 6)) {
		schick_en = 1;
		return 300;
	}
	if (!strncmp(p + 0x4512, "V3.02", 6)) {
		schick_en = 1;
		return 302;
	}
	if (!strncmp(p + 0x4512, "V3.09", 6)) {
		schick_en = 1;
		return 309;
	}

	return 0;
}

/**
	schick_gen_get_version - returns the version number of the character generator
	@p:	pointer to the start of the datasegment
*/
int schick_gen_get_version(char *p) {

	/* V1.05_de, the common CD-version */
	if (!strncmp(p + 0x1cb3, "V1.05", 6))
		return 105;

	/* V1.01_de, a german floppy version */
	if (!strncmp(p + 0x1e7e, "V1.01", 6))
		return 101;
	/* V1.03_de, a german floppy version */
	if (!strncmp(p + 0x1e7e, "V1.03", 6))
		return 103;
	/* V1.04_de, a german floppy version */
	if (!strncmp(p + 0x1e80, "V1.04", 6))
		return 104;
	/* V3.00_en, an english floppy version */
	if (!strncmp(p + 0x1bf1, "V3.00", 6)) {
		gen_en = 1;
		return 300;
	}
	return 0;
}

/**
	schick_get_fname - copies the filename from src to dst
	@src:	pathname to a file
	@dst:	string where the filename shoukld be stored
*/
void schick_get_fname(char *dst, char *src) {
	char *p = NULL;
	char *c = src;

	while (*c != '\0') {
		if (*c == '\\')
			p = c + 1;
		c++;
	}

	/* No backslash in src*/
	if (p == NULL)
		p = src;

	while (*dst++ = tolower(*p++));
	*dst = '\0';
}

//Initializer - is startet if executed file is SCHICKM.EXE/BLADEM.EXE or GEN.EXE
//Returns true if the desired programm is started
bool init_schick(char *name, unsigned short reloc, unsigned short _cs, unsigned short ip)
{

	char borsig[] = "Borland C++ - Copyright 1991 Borland Intl.";
	char fname[13];
	int ver;

	schick_get_fname(fname, name);

	if (strcmp(fname, "schickm.exe")
			&& strcmp(fname, "bladem.exe")
			&& strcmp(fname, "gen.exe")) return false;

	/* Check CS:IP in the EXE-Header are 0:0
	 * and the first executed instruction is mov dx,i16 */
	if (_cs != 0 || ip != 0 || real_readb(reloc+_cs, ip) != 0xba)
		return false;

	/* Show CS:IP on the virtual machine and the pointer to 0:0 */
	D1_TRAC("\n\nCS:IP 0x%x:0x%x\tMemBase: %p\n", reloc, ip, MemBase);

	/* Read and show the Datasegment */
	datseg_bak = datseg;
	datseg = real_readw(reloc, ip+1);
	p_datseg_bak = p_datseg;
	p_datseg = MemBase + PhysMake(datseg, 0);
	relocation = reloc;
	D1_TRAC("Dseg: 0x%X\n", datseg);

	/* Check if the start of the Datasegment is Borland C++ */
	if (host_readd(p_datseg) != 0 ||
		strcmp((char*)MemBase+PhysMake(datseg, 4), borsig)) {

		D1_ERR("Kein Borland C++ Kompilat!\n");
		return false;
	}

	/* check for the game program */
	if (!strcmp(fname, "schickm.exe") || !strcmp(fname, "bladem.exe")) {

		reloc_game = reloc;

		ver = schick_get_version((char*)p_datseg);

		if (ver == 0) {
			D1_ERR("Unbekannte Version von DSA1\n");
			return false;
		}

		D1_INFO("\nDSA1 Schicksalsklinge gefunden V%d.%02d_%s\n",
			ver / 100, ver % 100, schick_is_en() ? "en": "de");

		/* enable profiler only on this version */
		if (ver == 302 && !schick_is_en()) {
			D1_INFO("Starte Profiler\n");

			/* enable status comperator */
			schick_status_init();

			schick_timer_enable();

			schick++;
		}
	}

	/* check for the character generation program */
	if (!strcmp(fname, "gen.exe")) {
		reloc_gen = reloc;
		ver = schick_gen_get_version((char*)p_datseg);

		if (ver == 0) {
			D1_ERR("Unbekannte Version von DSA1 Generierung\n");
			return false;
		}

		D1_INFO("DSA1 Generierung gefunden V%d.%02d_%s\n",
			ver / 100, ver % 100, schick_gen_is_en() ? "en": "de");

		/* This happens only gen is started from the game.
		   We have to save some values. */

		if (!fromgame && schick && !gen) {
			if (schick_get_version((char*)p_datseg_bak) == 302 && !schick_is_en()) {
				schick_status_disable();
				schick_timer_disable();
		}
			schick--;
			fromgame++;

			D1_INFO("Gen gestartet\nreloc (0x%x)\n", relocation);
		}

		/* enable profiler only on this version */
		if (ver == 105 && !schick_is_en()) {
			D1_INFO("Starte Profiler\n");
			gen++;
		}
	}

	return true;
}


void exit_schick(unsigned char exit)
{
	if (fromgame)
	{
		gen--;
		fromgame--;
		schick++;
		relocation = reloc_game;
		datseg = datseg_bak;
		datseg_bak = 0;
		p_datseg = p_datseg_bak;
		p_datseg_bak = NULL;
		reloc_gen = 0;
		D1_INFO("Gen beendet\nProfiling geht weiter\n");

		if (schick_get_version((char*)p_datseg) == 302 && !schick_is_en()) {
			schick_status_enable();
			schick_timer_enable();
		}

		return;
	}

	if (gen) {
		G105de::BE_cleanup();
		gen--;
		reloc_gen = 0;
	}
	if (schick) {
		schick--;

		if (schick_get_version((char*)p_datseg) == 302 && !schick_is_en()) {
			schick_status_exit();
			schick_timer_disable();
			reloc_game = 0;
		}
	}
	D1_INFO("DSA1 Fehlercode %d\nProfiler beendet\n", exit);
}

const char* names_attrib[] = {	"MU", "KL", "CH", "FF", "GE", "IN", "KK",
				"AG", "HA", "RA", "GG", "TA", "NG", "JZ"};

const char* names_skill[] = {
    "Waffenlos", "Hiebwaffen", "Stichwaffen", "Schwerter", "Äxte", "Speere", "Zweihänder", "Schusswaffen", "Wurfwaffen",
    "Akrobatik", "Klettern", "Körperbeh.", "Reiten", "Schleichen", "Schwimmen", "Selbstbeh.", "Tanzen", "Verstecken", "Zechen",
    "Bekehren", "Betören", "Feilschen", "Gassenwissen", "Lügen", "Menschenkenntnis", "Schätzen",
    "Fährtensuchen", "Fesseln", "Orientierung", "Pflanzenkunde", "Tierkunde", "Wildnisleben",
    "Alchimie", "Alte Sprachen", "Geographie", "Geschichte", "Götter/Kulte", "Kriegskunst", "Lesen", "Magiekunde", "Sprachen",
    "Abrichten", "Fahrzeuge", "Falschspiel", "Heilen Gift", "Heilen Krankheit", "Heilen Wunden", "Musizieren", "Schlösser", "Taschendieb",
    "Gefahrensinn", "Sinnenschärfe"
};
const char* names_spell[] = {
    "DUMMY",
    "Beherrschung brechen", "Destructibo", "Gardianum", "Illusionen zerstören", "Verwandlung beenden", // Antimagie
    "Band & Fessel", "Bannbaladin", "Böser Blick", "Große Gier", "Große Verwirrung", "Herr der Tiere", "Horriphobus", "Mag. Raub", "Respondami", "Sanftmut", "Somnigravis", "Zwingtanz", // Beherrschung
    "Furor Blut", "Geister bannen", "Geister rufen", "Heptagon", "Krähenruf", "Skelettarius", // Dämonologie
    "Elementar herbeirufen", "Nihilatio Gravitas", "Solidrid Farbenspiel", // Elementarmagie
    "Axxeleraus", "Foramen", "Motoricus", "Spurlos, Trittlos", "Transversalis", "Über Eis", // Bewegung
    "Balsam", "Hexenspeichel", "Klarum Purum", "Ruhe Körper", "Tiere heilen", // Heilung
    "Adleraug", "Analüs", "Eigenschaften", "Exposami", "Odem Arcanum", "Penetrizzel", "Sensibar", // Hellsicht
    "Chamaelioni", "Duplicatus", "Harmlos", "Hexenknoten", // Illusion
    "Blitz", "Ecliptifactus", "Eisenrost", "Fulminictus", "Ignifaxius", "Plumbumbarum", "Radau", "Saft, Kraft, Monstermacht", "Scharfes Auge", // Kampf
    "Hexenblick", "Nekropathia", // Verständigung
    "Adler, Wolf", "Arcano Psychostabilis", "Armatrutz", "CH steigern", "Feuerbann", "FF steigern", "GE steigern", "IN steigern", "KK steigern", "KL steigern", "MU steigern", "Mutabili", "Paralü", "Salander", "See & Fluss", "Visibili", // Verwandlung
    "Abvenenum", "Aeolitus", "Brenne", "Claudibus", "Dunkelheit", "Erstarre", "Flim Flam", "Schmelze", "Silentium", "Sturmgebrüll" // Veränderung
};

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
int get_ovrseg(unsigned short stub_seg) {
	Bit8u *p = MemBase + (relocation<<4) + (stub_seg<<4);

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

int schick_callf(unsigned selector, unsigned offs)
{
	if (selector == SegValue(ss))
		return 0;
	if (selector >= 0xa000)
		return 0;

	unsigned short segm = selector - relocation;
	int ret = 0;

	if (schick && !fromgame) {
		ret = schick_farcall_v302de(segm, offs);
		return ret;
	}

	if (gen) {
		ret = schick_farcall_gen105(segm, offs);
		return ret;
	}

	return ret;
}

// Intercept near CALLs, 16-Bit
int schick_calln16(unsigned offs) {

	if (SegValue(cs) == SegValue(ss))
		return 0;
	if (SegValue(cs) >= 0xa000)
		return 0;

	int ret = 0;

	if (schick && !fromgame) {
		ret = schick_nearcall_v302de(offs);
		return ret;
	}

	if (gen) {
		ret = schick_nearcall_gen105(offs);
		return ret;
	}

	return ret;
}
