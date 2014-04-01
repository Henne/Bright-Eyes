#if !defined(__BORLANDC__)

#include "regs.h"
#include "callback.h"

#include "../schick.h"

#define AIL_SEGMENT (0x1042)

namespace M302de {

void AIL_startup(void)
{
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0x6d1);
}

void AIL_shutdown(RealPt signoff_msg)
{
	CPU_Push32(signoff_msg);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0x715);
	CPU_Pop32();
}

Bit16s AIL_register_driver(RealPt driver)
{
	CPU_Push32(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xa1e);
	CPU_Pop32();
	return reg_ax;
}

RealPt AIL_describe_driver(Bit16u driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xad6);
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

Bit16u AIL_detect_device(Bit16u driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
	CPU_Push16(DRQ);
	CPU_Push16(DMA);
	CPU_Push16(IRQ);
	CPU_Push16(IO_addr);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xad6);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax;
}

void AIL_init_driver(Bit16u driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
	CPU_Push16(DRQ);
	CPU_Push16(DMA);
	CPU_Push16(IRQ);
	CPU_Push16(IO_addr);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xb02);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
}

void AIL_play_VOC_file(Bit16u driver, RealPt VOC_file, Bit16s block_marker)
{
	CPU_Push16(block_marker);
	CPU_Push32(VOC_file);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xbff);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
}

Bit16u AIL_format_VOC_file(Bit16u driver, RealPt VOC_file, Bit16s block_marker)
{
	CPU_Push16(block_marker);
	CPU_Push32(VOC_file);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc05);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();
	return reg_ax;
}

Bit16u AIL_VOC_playback_status(Bit16u driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc0b);
	CPU_Pop16();
	return reg_ax;
}

void AIL_start_digital_playback(Bit16u driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc11);
	CPU_Pop16();
}

void AIL_stop_digital_playback(Bit16u driver)
{
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc17);
	CPU_Pop16();
}

void AIL_set_digital_playback_volume(Bit16u driver, Bit16u percent)
{
	CPU_Push16(percent);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc29);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_register_sequence(Bit16u driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table)
{
	CPU_Push32(controller_table);
	CPU_Push32(state_table);
	CPU_Push16(sequence_num);
	CPU_Push32(FORM_XMID);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc47);
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
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc4d);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_timbre_request(Bit16u driver, Bit16u sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc53);
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
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc59);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop32();
}

void AIL_start_sequence(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc7d);
	CPU_Pop16();
	CPU_Pop16();
}

void AIL_stop_sequence(Bit16s driver, Bit16s sequence)
{
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc83);
	CPU_Pop16();
	CPU_Pop16();
}

Bit16u AIL_sequence_status(Bit16u driver, Bit16u sequence)
{

	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc8f);
	CPU_Pop16();
	CPU_Pop16();

	return reg_ax;
}

void AIL_set_relative_volume(Bit16u driver, Bit16u sequence, Bit16u percent, Bit16u ms)
{

	CPU_Push16(ms);
	CPU_Push16(percent);
	CPU_Push16(sequence);
	CPU_Push16(driver);
	CALLBACK_RunRealFar(reloc_game + AIL_SEGMENT, 0xc8f);
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop16();
}

}
#else
#include "AIL/AIL.H"
#endif
