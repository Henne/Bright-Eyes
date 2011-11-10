#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "schweif.h"

// Is the game running?
static int schweif = 0;

// Segment relocation
unsigned short relocation;

namespace schweif_o100de {
//Datasegment
unsigned short datseg = 0;
Bit8u *p_datseg = NULL;
}

static short schweif_en = 0;

/**
	schweif_is_en - returns 1 if the game language is english
*/
int schweif_is_en() {
	return schweif_en;
}

/**
	schweif_get_version - returns the version number of the game
	@p:	pointer to the start of the datasegment
*/
const char* schweif_get_version(char *p) {
	/* German 1CD-Version 01.00. Yes, that's an upcase 'o' in front. */
	if (!strncmp(p + 0x2219, "O1.00", 4))
		return (char*)"O100de";
	/* C2.00 english floppy version */
	if (!strncmp(p + 0x2195, "C2.00", 4)) {
		schweif_en = 1;
		return (char*)"C2.00";
	}

	return 0;
}

/**
	schweif_get_fname - copies the filename from src to dst
	@src:	pathname to a file
	@dst:	string where the filename should be stored
*/
static void schweif_get_fname(char *dst, char *src) {
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

//Initializer - is started if executed file is STAR.EXE/SCHWEIF.EXE
//Returns true if the desired programm is started
bool schweif_init(char *name, unsigned short reloc, unsigned short _cs, unsigned short ip)
{
	char borsig[] = "Borland C++ - Copyright 1991 Borland Intl.";
	char fname[13];

	schweif_get_fname(fname, name);
	if (strcmp(fname, "schweif.exe")
	    && strcmp(fname, "star.exe")) return false;

	/* Check CS:IP in the EXE-Header are 0:0
	 * and the first executed instruction is mov dx,i16 */
	if (_cs != 0 || ip != 0 || real_readb(reloc+_cs, ip) != 0xba)
		return false;
	
	/* Show CS:IP on the virtual machine and the pointer to 0:0 */
	D2_TRAC("\n\nCS:IP 0x%x:0x%x\tMemBase: %p\n", reloc, ip, MemBase);

	/* Read and show the Datasegment */
	datseg = real_readw(reloc, ip+1);
	p_datseg = MemBase + PhysMake(datseg, 0);
	relocation = reloc;
	D2_TRAC("Dseg: 0x%X\n", datseg);

	/* Check if the start of the Datasegment is Borland C++ */
	if (host_readd(p_datseg) != 0 ||
		strcmp((char*)MemBase+PhysMake(datseg, 4), borsig)) {

		D2_ERR("Kein Borland C++ Kompilat!\n");
		return false;
	}

	/* check for the game program */
	if (!strcmp(fname, "schweif.exe") || !strcmp(fname, "star.exe")) {
		const char* ver = schweif_get_version((char*)p_datseg);
		if (ver == 0) {
			D2_ERR("Unbekannte Version von DSA2\n");
			return false;
		}

		D2_INFO("\nDSA2 Sternenschweif gefunden Version %s\n", ver);

		/* enable profiler only on this version */
		if (ver == "O100de") {
			D2_INFO("Starte Profiler\n");
			schweif++;
		}
	}

	return true;
}


void schweif_exit(unsigned char exit)
{
	if (schweif) {
		schweif--;
	}
	D2_INFO("DSA2 Fehlercode %d\nProfiler beendet\n", exit);
}

namespace schweif_o100 {
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
 * interrupt 0x3f is generated, the code is loaded from the binary
 * and the stub is ajusted with far jumps to the corrosponding funcs.
 */
int schweif_get_ovrseg(unsigned short stub_seg) {
    Bit8u *p = MemBase + (relocation<<4) + (stub_seg<<4);
    
    if (host_readw(p) != 0x3fcd) {
	D2_ERR("Error: %x is not an overlay segment\n", stub_seg);
	return 0;
    }
    if (host_readw(p + 0x20) == 0x3fcd) {
	//	D2_ERR("Error: %x is not in memory\n", stub_seg);
	return 0;
    }
    if (host_readb(p + 0x20) != 0xea) {
	D2_ERR("No farjump in overlay segment\n");
	return 0;
    }
    return host_readw(p + 0x23);
}

}

int schweif_callf(unsigned selector, unsigned offs)
{
	if (selector == SegValue(ss))
		return 0;
	if (selector >= 0xa000)
		return 0;

	unsigned short segm = selector - relocation;
	int ret = 0;

	if (schweif) {
		ret = schweif_farcall_v100de(segm, offs);
		return ret;
	}
	return ret;
}

// Intercept near CALLs, 16-Bit
int schweif_calln(unsigned offs) {

	if (SegValue(cs) == SegValue(ss))
		return 0;
	if (SegValue(cs) >= 0xa000)
		return 0;

	int ret = 0;

	if (schweif) {
		ret = schweif_nearcall_v100de(offs);
		return ret;
	}

	return ret;
}

