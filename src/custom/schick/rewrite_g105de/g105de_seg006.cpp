#include "regs.h"
#include "paging.h"
#include "callback.h"

#include "../schick.h"

void AIL_startup()
{
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0x6d7);
}

void AIL_shutdown(RealPt signoff_msg)
{
	CPU_Push32(signoff_msg);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0x71b);
	CPU_Pop32();
}

Bit16u AIL_register_sequence(Bit16u driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table)
{
	CPU_Push32(controller_table);
	CPU_Push32(state_table);
	CPU_Push16(sequence_num);
	CPU_Push32(FORM_XMID);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc4d);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop32();
	return reg_ax;
}

Bit16u AIL_timbre_request(Bit16u driver, Bit16u sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc65);
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax;
}

void AIL_install_timbre(Bit16u driver, Bit16u bank, Bit16u patch, RealPt src_addr)
{
	CPU_Push32(src_addr);
	CPU_Push16(patch);
	CPU_Push16(bank);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc6b);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop32();
}

void AIL_start_sequence(Bit16u driver, Bit16u sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc83);
	CPU_Pop16();
	CPU_Pop16();
}

void AIL_stop_sequence(Bit16u driver, Bit16u sequence)
{

	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc89);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_sequence_status(Bit16u driver, Bit16u sequence)
{

	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc95);
	CPU_Pop16();
	CPU_Pop16();

	return reg_ax;
}

void AIL_release_sequence_handle(Bit16u driver, Bit16u sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc53);
	CPU_Pop16();
	CPU_Pop16();
}
