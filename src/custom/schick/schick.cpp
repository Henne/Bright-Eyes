#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "schick.h"

// Is the game running?
static int schick = 0;
// Has the game called gen?
static int fromgame = 0;

//Segment relocation of the game
Bitu reloc_game;

//Datasegment
unsigned short datseg = 0;
Bit8u *p_datseg = NULL;
//Datesegment relocation from game if gen is called
unsigned short datseg_bak = 0;
Bit8u *p_datseg_bak = NULL;

static short schick_en = 0;

/**
	schick_is_en - returns 1 if the game language is english
*/
int schick_is_en() {
	return schick_en;
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

	while ((*dst++ = tolower(*p++)));
	*dst = '\0';
}

//Initializer - is startet if executed file is SCHICKM.EXE/BLADEM.EXE or GEN.EXE
//Returns true if the desired programm is started
bool init_schick(char *name, unsigned short reloc, unsigned short _cs, unsigned short ip)
{
	char fname[81];
	int ver;

	schick_get_fname(fname, name);

	if (strcmp(fname, "schickm.exe")
			&& strcmp(fname, "gen.exe")
			&& strcmp(fname, "bladem.exe")) return false;

	/* check for the game program */
	if (!strcmp(fname, "schickm.exe") || !strcmp(fname, "bladem.exe")) {

		/* Show CS:IP on the virtual machine and the pointer to 0:0 */
		D1_TRAC("\n\nCS:IP 0x%x:0x%x\tMemBase: %p\n", reloc, ip, MemBase);

		/* Read and show the Datasegment */
		datseg_bak = datseg;
		datseg = real_readw(reloc, ip+1);
		p_datseg_bak = p_datseg;
		p_datseg = MemBase + PhysMake(datseg, 0);
		D1_TRAC("Dseg: 0x%X\n", datseg);

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

		D1_INFO("DSA1 Generierung gestartet (wird emuliert)\n");

		if (!fromgame && schick) {
			if (schick_get_version((char*)p_datseg) == 302 && !schick_is_en()) {
				schick_status_disable();
				schick_timer_disable();
			}

			schick--;
			fromgame++;

			D1_INFO("Profiler pausiert\n");
		}
	}

	return true;
}


void exit_schick(unsigned char exit)
{
	if (fromgame) {
		fromgame--;
		schick++;

		if (schick_get_version((char*)p_datseg) == 302 && !schick_is_en()) {
			schick_status_enable();
			schick_timer_enable();
		}

		D1_INFO("Profiling geht weiter\n");

	} else if (schick) {
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

int schick_callf(unsigned selector, unsigned offs)
{
	if (selector == SegValue(ss))
		return 0;
	if (selector >= 0xa000)
		return 0;

	if (schick && !fromgame) {
		return schick_farcall_v302de(selector - reloc_game, offs);
	}

	return 0;
}

// Intercept near CALLs, 16-Bit
int schick_calln16(unsigned offs) {

	if (SegValue(cs) == SegValue(ss))
		return 0;
	if (SegValue(cs) >= 0xa000)
		return 0;

	int ret = 0;

	if (schick && !fromgame) {
		return schick_nearcall_v302de(offs);
	}

	return ret;
}
