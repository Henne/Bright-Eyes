#include "cpu.h"

#include <stdio.h>
#include <cstring>

/* Compiler Strings */
static const char borsig[] = "Borland C++ - Copyright 1991 Borland Intl.";
static const char tcsig[]  = "Turbo-C - Copyright (c) 1988 Borland Intl.";
static const char tcpsig[]  = "Turbo C++ - Copyright 1990 Borland Intl.";


/* Signature: strlen() BCC31:L BCC20:L */
static const unsigned char s_strlen[] =
	{0x55, 0x8b, 0xec, 0x57, 0xc4, 0x7e, 0x06, 0x33,
	 0xc0, 0x3b, 0x46, 0x08, 0x75, 0x04, 0x3b, 0xc7,
	 0x74, 0x0a, 0xfc, 0xb9, 0xff, 0xff, 0xf2, 0xae,
	 0x91, 0xf7, 0xd0, 0x48, 0x5f, 0x5d, 0xcb};

/* Signature: bioskey() BCC31:L */
static const unsigned char s_bioskey31[] =
	{0x55, 0x8b, 0xec, 0x32, 0xc0, 0x8a, 0x66, 0x06,
	 0xcd, 0x16, 0x74, 0x0f, 0xf6, 0x46, 0x06, 0x01,
	 0x74, 0x11, 0x0b, 0xc0, 0x75, 0x0d, 0xb8, 0xff,
	 0xff, 0xeb, 0x08, 0xf6, 0x46, 0x06, 0x01, 0x74,
	 0x02, 0x33, 0xc0, 0x5d, 0xcb};

/* Signature: bioskey() BCC20:L */
static const unsigned char s_bioskey20[] =
	{0x55, 0x8b, 0xec, 0x32, 0xc0, 0x8a, 0x66, 0x06,
	 0xcd, 0x16, 0x74, 0x0f, 0xf6, 0x46, 0x06, 0x01,
	 0x75, 0x11, 0x0b, 0xc0, 0x75, 0x0d, 0xb8, 0xff,
	 0xff, 0xeb, 0x08, 0xf6, 0x46, 0x06, 0x01, 0x74,
	 0x02, 0x33, 0xc0, 0x5d, 0xcb};

/* Signature: memcpy() BCC31:L BCC20:L */
static const unsigned char s_memcpy[] =
	{0x55, 0x8b, 0xec, 0x56, 0x57, 0x8c, 0xda, 0xc4,
	 0x7e, 0x06, 0xc5, 0x76, 0x0a, 0x8b, 0x4e, 0x0e,
	 0xd1, 0xe9, 0xfc, 0xf3, 0xa5, 0x73, 0x01, 0xa4,
	 0x8e, 0xda, 0x8b, 0x56, 0x08, 0x8b, 0x46, 0x06,
	 0x5f, 0x5e, 0x5d, 0xcb};

/* Signature: setmem() BCC31:L BCC20:L */
/* (internal use memset(), calloc()) */
static const unsigned char s_setmem[] =
	{0x55, 0x8b, 0xec, 0x57, 0xc4, 0x7e, 0x06, 0x8b,
	 0x4e, 0x0a, 0x8a, 0x46, 0x0c, 0x8a, 0xe0, 0xfc,
	 0xf7, 0xc7, 0x01, 0x00, 0x74, 0x04, 0xe3, 0x09,
	 0xaa, 0x49, 0xd1, 0xe9, 0xf3, 0xab, 0x73, 0x01,
	 0xaa, 0x5f, 0x5d, 0xcb};

/* Signature: memset() BCC31:L BCC20:L */
static const unsigned char s_memset[] =
	{0x55, 0x8b, 0xec, 0x8a, 0x46, 0x0a, 0x50, 0xff,
	 0x76, 0x0c, 0xff, 0x76, 0x08, 0xff, 0x76, 0x06,
	 0x0e, 0xe8, 0xc8, 0xff, 0x83, 0xc4, 0x08, 0x8b,
	 0x56, 0x08, 0x8b, 0x46, 0x06, 0x5d, 0xcb};

/* Signature: _rotl() BCC31:L BCC20:L */
static const unsigned char s__rotl[] =
	{0x55, 0x8b, 0xec, 0x8b, 0x46, 0x06, 0x8b, 0x4e,
	 0x08, 0xd3, 0xc0, 0x5d, 0xcb};

/* Signature: abs() BCC31:L BCC20:L */
static const unsigned char s_abs[] =
	{0x55, 0x8b, 0xec, 0x8b, 0x46, 0x06, 0x0b, 0xc0,
	 0x7d, 0x02, 0xf7, 0xd8, 0x5d, 0xcb};

void find_clib_signatures(Bit8u* p_cs, Bit8u* p_ds)
{
	const long len = p_ds - p_cs;	/* Size in bytes of the area between CS and DS */
	long i = 0;
	while (i < len) {
		if (memcmp(p_cs + i, s_strlen, sizeof(s_strlen)) == 0) {
			fprintf(stderr, "Found strlen() at CS:0x%04x\n", i);
			i += sizeof(s_strlen) - 1;
		}
		if (memcmp(p_cs + i, s_bioskey20, sizeof(s_bioskey20)) == 0) {
			fprintf(stderr, "Found bioskey() at CS:0x%04x\n", i);
			i += sizeof(s_bioskey20) - 1;
		}
		if (memcmp(p_cs + i, s_bioskey31, sizeof(s_bioskey31)) == 0) {
			fprintf(stderr, "Found bioskey() at CS:0x%04x\n", i);
			i += sizeof(s_bioskey31) - 1;
		}
		if (memcmp(p_cs + i, s_memcpy, sizeof(s_memcpy)) == 0) {
			fprintf(stderr, "Found memcpy() at CS:0x%04x\n", i);
			i += sizeof(s_memcpy) - 1;
		}
		if (memcmp(p_cs + i, s_setmem, sizeof(s_setmem)) == 0) {
			fprintf(stderr, "Found setmem() at CS:0x%04x\n", i);
			i += sizeof(s_setmem) - 1;
		}
		if (memcmp(p_cs + i, s_memset, sizeof(s_memset)) == 0) {
			fprintf(stderr, "Found memset() at CS:0x%04x\n", i);
			i += sizeof(s_memset) - 1;
		}
		if (memcmp(p_cs + i, s__rotl, sizeof(s__rotl)) == 0) {
			fprintf(stderr, "Found _rotl() at CS:0x%04x\n", i);
			i += sizeof(s__rotl) - 1;
		}
		if (memcmp(p_cs + i, s_abs, sizeof(s_abs)) == 0) {
			fprintf(stderr, "Found abs() at CS:0x%04x\n", i);
			i += sizeof(s_abs) - 1;
		}

		i++;
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
