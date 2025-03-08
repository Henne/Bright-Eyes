#include "cpu.h"

#include <stdio.h>
#include <cstring>

/* Compiler Strings */
static const char borsig[] = "Borland C++ - Copyright 1991 Borland Intl.";
static const char tcsig[]  = "Turbo-C - Copyright (c) 1988 Borland Intl.";
static const char tcpsig[]  = "Turbo C++ - Copyright 1990 Borland Intl.";


/* Signature: strlen() BCC31:L */
static const char s_strlen[] = {0x55, 0x8b, 0xec, 0x57, 0xc4, 0x7e, 0x06, 0x33,
				0xc0, 0x3b, 0x46, 0x08, 0x75, 0x04, 0x3b, 0xc7,
				0x74, 0x0a, 0xfc, 0xb9, 0xff, 0xff, 0xf2, 0xae,
				0x91, 0xf7, 0xd0, 0x48, 0x5f, 0x5d, 0xcb};


void find_clib_signatures(Bit8u* p_cs, Bit8u* p_ds)
{
	int i;
	for (i = 0; i < 0xffff; i++) {
		if (memcmp(p_cs + i, s_strlen, strlen(s_strlen)) == 0) {
			fprintf(stderr, "Found strlen() at CS:0x%04x\n", i);
		}
	}

}

bool check_bcc(char *name, unsigned short reloc, unsigned short _cs, unsigned short _ip)
{
	Bit8u* p_datseg = NULL;
	Bit8u *p_cseg = NULL;
	Bit16u datseg;
	const char *sig = NULL;

	fprintf(stderr, "Executable: %s is Checked!\n", name);

	p_cseg = MemBase + PhysMake(reloc + _cs, _ip);

	/* Check CS:IP in the EXE-Header are 0:0
	 * and the first executed instruction is mov dx, i16 */
	if (_cs != 0 || _ip != 0 || *p_cseg != 0xba)
		return false;

	//fprintf(stderr, "Executable: %s passed 1!\n", name);

	/* next two bytes are the segment address of the datasegment */
	datseg = real_readw(reloc, _ip + 1);
	p_datseg = MemBase + PhysMake(datseg, 0);

	/* Check if the start of the Datasegment is Borland C++ */
	if (host_readd(p_datseg) == 0) {
		if (!strcmp((char*)(p_datseg + 4), borsig)) sig = borsig;
		else if (!strcmp((char*)(p_datseg + 4), tcsig)) sig = tcsig;
		else if (!strcmp((char*)(p_datseg + 4), tcpsig)) sig = tcpsig;
	}

	if (sig == NULL) {
		fprintf(stderr, "Executable: %s no known signature found!\n", name);
		return false;
	}

	//fprintf(stderr, "Executable: %s passed 2!\n", name);

	/* Now: more info */
	Bit8u* pt = MemBase + PhysMake(reloc, 0x25); /* pointer to call SaveVectors */
	int version = -1;
	char model = '?';

	/* Borland C++ */
	if ((sig == borsig) && (*pt++ == 0xe8)) {
		//fprintf(stderr, "Executable: %s passed 3!\n", name);
		     if ((pt[0] == 0x43) && (pt[1] == 0x01) && (p_cseg[0x6] == 0x3c) ) {	version = 20; model = 'M'; }
		else if ((pt[0] == 0x14) && (pt[1] == 0x01)) {	version = 20; model = 'C'; }
		else if ((pt[0] == 0x1a) && (pt[1] == 0x01)) {	version = 20; model = 'L'; }
		else if ((pt[0] == 0x08) && (pt[1] == 0x01)) {	version = 20; model = 'H'; }

		else if ((pt[0] == 0x85) && (pt[1] == 0x01)) {	version = 30; model = 'M'; }
		else if ((pt[0] == 0x4e) && (pt[1] == 0x01)) {	version = 30; model = 'C'; }
		else if ((pt[0] == 0x52) && (pt[1] == 0x01)) {	version = 30; model = 'L'; }
		else if ((pt[0] == 0x40) && (pt[1] == 0x01)) {	version = 30; model = 'H'; }

		else if ((pt[0] == 0x88) && (pt[1] == 0x01)) {	version = 31; model = 'M'; }
		else if ((pt[0] == 0x51) && (pt[1] == 0x01)) {	version = 31; model = 'C'; }
		else if ((pt[0] == 0x55) && (pt[1] == 0x01)) {	version = 31; model = 'L'; }
		else if ((pt[0] == 0x43) && (pt[1] == 0x01) && (p_cseg[0x6] == 0x84)) {	version = 31; model = 'H'; }
	}

	fprintf(stderr, "Executable: %s, Compiler: %s\n", name, sig);
	fprintf(stderr, "Executable: %s, Version: %d.%d Model %c!\n", name, version / 10, version % 10, model);

	find_clib_signatures(p_cseg, p_datseg);

	return true;
}
