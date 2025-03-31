#if !defined(__BORLANDC__)
namespace G105de {

void AIL_startup();

void AIL_shutdown(RealPt signoff_msg);

Bit16s AIL_register_driver(RealPt driver_base_addr);

RealPt AIL_describe_driver(Bit16s driver);

Bit16u AIL_detect_device(Bit16s driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ);

void AIL_init_driver(Bit16s driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ);

Bit16u AIL_state_table_size(Bit16s driver);

Bit16s AIL_register_sequence(Bit16s driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table);

void AIL_release_sequence_handle(Bit16s driver, Bit16s sequence);

Bit16u AIL_default_timbre_cache_size(Bit16s driver);

void AIL_define_timbre_cache(Bit16s driver, RealPt cache_addr, Bit16u cache_size);

Bit16u AIL_timbre_request(Bit16s driver, Bit16s sequence);

void AIL_install_timbre(Bit16s driver, Bit16s bank, Bit16s patch, RealPt src_addr);

void AIL_start_sequence(Bit16s driver, Bit16s sequence);

void AIL_stop_sequence(Bit16s driver, Bit16s sequence);

Bit16u AIL_sequence_status(Bit16s driver, Bit16s sequence);

}
#else
#include "AIL.H"
#endif
