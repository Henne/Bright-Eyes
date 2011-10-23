#include "dosbox.h"

#if DOSBOX_CUSTOM

#include <stdio.h>

#include "custom.h"
#include "custom_hooks.h"
static Bit8u custom_runs;

void custom_init_prog(char *name, Bit16u relocate, Bit16u init_cs, Bit16u init_ip)
{
/*
	int check;

	if (custom_runs)
		current->suspend();
*/

	/* run all detectors */
#ifdef DOSBOX_SCHICK_H
	if (schick_init(name, relocate, init_cs, init_ip))
		custom_runs++;
#endif /* DOSBOX_SCHICK_H */
#ifdef DOSBOX_SCHWEIF_H
	if (schweif_init(name, relocate, init_cs, init_ip))
		custom_runs++;
#endif /* DOSBOX_SCHWEIF_H */
#ifdef DOSBOX_RIVA_H
	if (riva_init(name, relocate, init_cs, init_ip))
		custom_runs++;
#endif /* DOSBOX_RIVA_H */
}

void custom_exit_prog(Bit8u exitcode)
{
	if (!custom_runs)
		return;

	custom_runs--;

#ifdef DOSBOX_SCHICK_H
	schick_exit(exitcode);
#endif /* DOSBOX_SCHICK_H */
#ifdef DOSBOX_SCHWEIF_H
	schweif_exit(exitcode);
#endif /* DOSBOX_SCHWEIF_H */
#ifdef DOSBOX_RIVA_H
	riva_exit(exitcode);
#endif /* DOSBOX_RIVA_H */
}

int custom_calln(Bit16u ip)
{
	if (!custom_runs)
		return 0;
	
#ifdef DOSBOX_SCHICK_H
	return schick_calln16(ip);
#endif /* DOSBOX_SCHICK_H */
#ifdef DOSBOX_SCHWEIF_H
	return schweif_calln(ip);
#endif /* DOSBOX_SCHWEIF_H */
#ifdef DOSBOX_RIVA_H
	return riva_calln(ip);
#endif /* DOSBOX_RIVA_H */
}

int custom_callf(Bitu cs, Bitu ip)
{
	if (!custom_runs)
		return 0;

#ifdef DOSBOX_SCHICK_H
	return schick_callf(cs, ip);
#endif /* DOSBOX_SCHICK_H */
#ifdef DOSBOX_SCHWEIF_H
	return schweif_callf(cs, ip);
#endif /* DOSBOX_SCHWEIF_H */
#ifdef DOSBOX_RIVA_H
	return riva_callf(cs, ip);
#endif /* DOSBOX_RIVA_H */
}

void custom_init(Section *sec)
{
}

#endif /* DOSBOX_CUSTOM */
