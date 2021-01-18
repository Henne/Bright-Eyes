#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "soa.h"

// Is the game running?
static int soa_vgame_en;

unsigned short soa_relocation;

namespace soa_common {
//Datasegment
unsigned short datseg = 0;
Bit8u *p_datseg = NULL;
}

static short soa_en = 0;

/**
	soa_is_en - returns 1 if the game language is english
*/
int soa_is_en() {
	return soa_en;
}

/**
	soa_get_version - returns the version number of the game
	@p:	pointer to the start of the datasegment
*/
const char* soa_get_version(char *p) {
	/* ??? english floppy version */
	if (!strncmp(p + 0x4802, "OPITAR", 6) ||
		!strncmp(p + 0x530b, "OPITAR", 6)) {
		soa_en = 1;
		return (char*)"VGAME_EN";
	}

	return 0;
}

/**
	soa_get_fname - copies the filename from src to dst
	@src:	pathname to a file
	@dst:	string where the filename should be stored
*/
static void soa_get_fname(char *dst, char *src) {
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

//Initializer - is started if executed file is VGAME
//Returns true if the desired programm is started
bool soa_init(char *name, unsigned short reloc, unsigned short _cs, unsigned short ip)
{
	char fname[80];

	soa_get_fname(fname, name);
	if (strcmp(fname, "vgame.exe")) return false;

	/* Show CS:IP on the virtual machine and the pointer to 0:0 */
	SOA_ERR("\n\nCS:IP 0x%x:0x%x\tMemBase: %p\n", reloc, ip, MemBase);

	/* Read and show the Datasegment */
	datseg = real_readw(reloc, ip+1);
	p_datseg = MemBase + PhysMake(datseg, 0);
	soa_relocation = reloc;
	SOA_ERR("Dseg: 0x%X\n", datseg);

	/* check for the game program */
	if (!strcmp(fname, "vgame.exe")) {
		const char* ver = soa_get_version((char*)p_datseg);
		if (ver == 0) {
			SOA_ERR("Unbekannte Version von SOA\n");
			return false;
		}

		SOA_INFO("\nSOA gefunden Version %s\n", ver);

		/* enable profiler only on this version */
		if (ver == "VGAME_EN") {
			SOA_INFO("Starte Profiler\n");
			soa_vgame_en++;
		}
	}

	return true;
}


void soa_exit(unsigned char exit)
{
	if (soa_vgame_en)
		soa_vgame_en--;
	SOA_INFO("SOA Fehlercode %d\nProfiler beendet\n", exit);
}

namespace soa_common {
#if 0
const char* names_attrib[] = {	"MU", "KL", "CH", "FF", "GE", "IN", "KK",
				"AG", "HA", "RA", "GG", "TA", "NG", "JZ"};

#endif
}

int soa_callf(unsigned selector, unsigned offs)
{
	if (selector == SegValue(ss))
		return 0;
	if (selector >= 0xa000)
		return 0;


	unsigned short segm = selector - soa_relocation;

	if (soa_vgame_en)
		return soa_farcall_vgame_en(segm, offs);

	return 0;
}

// Intercept near CALLs, 16-Bit
int soa_calln(unsigned offs) {

	if (SegValue(cs) == SegValue(ss))
		return 0;
	if (SegValue(cs) >= 0xa000)
		return 0;

	if (soa_vgame_en)
		return soa_nearcall_vgame_en(offs);

	return 0;
}
