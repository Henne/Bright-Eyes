#include "dosbox.h"

#ifdef DOSBOX_CUSTOM

#include <stdio.h>

#include "setup.h"
#include "custom.h"
#include "custom_hooks.h"
static Bit8u custom_runs;

static Bit8u schick_runs;
static Bit8u schweif_runs;

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
	if (init_schick(name, relocate, init_cs, init_ip)) {
		custom_runs++;
		schick_runs++;
	}
	if (schweif_init(name, relocate, init_cs, init_ip)) {
		custom_runs++;
		schweif_runs++;
	}
}

void custom_exit_prog(Bit8u exitcode)
{
	if (!custom_runs)
		return;

	custom_runs--;
	if (schick_runs) {
		exit_schick(exitcode);
		schick_runs--;
	}
	if (schweif_runs) {
		schweif_exit(exitcode);
		schweif_runs--;
	}
}

int custom_calln(Bit16u ip)
{
	if (!custom_runs)
		return 0;

	if (schick_runs)
		return schick_calln16(ip);

	if (schweif_runs)
		return schweif_calln(ip);

	return 0;
}

int custom_callf(Bitu cs, Bitu ip)
{
	if (!custom_runs)
		return 0;

	if (schick_runs)
		return schick_callf(cs, ip);
	if (schweif_runs)
		return schweif_callf(cs, ip);

	return 0;
}

static void custom_exit(Section *sec)
{
	custom_exit_prog(0);
}

void custom_init(Section *sec)
{
//	custom_prog *p_current = new custom_prog;
	sec->AddDestroyFunction(&custom_exit);
}

#endif /* DOSBOX_CUSTOM */
