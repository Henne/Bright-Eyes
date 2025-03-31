#if !defined(__BORLANDC__)

#include "regs.h"
#include "paging.h"
#include "callback.h"

#include "../schick.h"

namespace G105de {

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

Bit16s AIL_register_driver(RealPt driver_base_addr)
{
	CPU_Push32(driver_base_addr);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xa24);
	CPU_Pop32();
	return reg_ax;
}

RealPt AIL_describe_driver(Bit16s driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xadc);
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

Bit16u AIL_detect_device(Bit16s driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
	CPU_Push16(DRQ);
	CPU_Push16(DMA);
	CPU_Push16(IRQ);
	CPU_Push16(IO_addr);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xb02);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax;
}

void AIL_init_driver(Bit16s driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
	CPU_Push16(DRQ);
	CPU_Push16(DMA);
	CPU_Push16(IRQ);
	CPU_Push16(IO_addr);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xb08);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_state_table_size(Bit16s driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc47);
	CPU_Pop16();
	return reg_ax;
}

Bit16s AIL_register_sequence(Bit16s driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table)
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

void AIL_release_sequence_handle(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc53);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_default_timbre_cache_size(Bit16s driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc59);
	CPU_Pop16();
	return reg_ax;
}

void AIL_define_timbre_cache(Bit16s driver, RealPt cache_addr, Bit16u cache_size)
{
	CPU_Push16(cache_size);
	CPU_Push32(cache_addr);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc5f);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
}

Bit16u AIL_timbre_request(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc65);
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax;
}

void AIL_install_timbre(Bit16s driver, Bit16s bank, Bit16s patch, RealPt src_addr)
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

void AIL_start_sequence(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc83);
	CPU_Pop16();
	CPU_Pop16();
}

void AIL_stop_sequence(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc89);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_sequence_status(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_gen + 0xbb2, 0xc95);
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax;
}

}
#endif
