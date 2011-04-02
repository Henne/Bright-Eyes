//#ifdef DOSBOX_CUSTOM

#include <stdio.h>

#include "dosbox.h"
#include "custom.h"
#include "custom_hooks.h"

static Bit8u custom_runs;

//static class custom_prog running_progs[2];

//static class custom_prog *current;

void custom_init_prog(char *name, Bit16u relocate, Bit16u init_cs, Bit16u init_ip)
{
/*
	int check;

	if (custom_runs)
		current->suspend();
*/

	/* run all detectors */
	if (init_schick(name, relocate, init_cs, init_ip))
		custom_runs++;
}

void custom_exit_prog(Bit8u exitcode)
{
	if (!custom_runs)
		return;

	custom_runs--;
	exit_schick(exitcode);
}

int custom_calln(Bit16u ip)
{
	if (!custom_runs)
		return 0;

	return schick_calln16(ip);
}

int custom_callf(Bitu cs, Bitu ip)
{
	if (!custom_runs)
		return 0;

	return schick_callf(cs, ip);
}

void custom_init(Section *sec)
{
//	custom_prog *p_current = new custom_prog;
}

//#endif /* DOSBOX_CUSTOM */
