void AIL_startup();

void AIL_shutdown(RealPt signoff_msg);

Bit16u AIL_register_sequence(Bit16u driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table);

Bit16u AIL_timbre_request(Bit16u driver, Bit16u sequence);

void AIL_install_timbre(Bit16u driver, Bit16u bank, Bit16u patch, RealPt src_addr);

void AIL_start_sequence(Bit16u driver, Bit16u sequence);

void AIL_stop_sequence(Bit16u driver, Bit16u sequence);

Bit16u AIL_sequence_status(Bit16u driver, Bit16u sequence);

void AIL_release_sequence_handle(Bit16u driver, Bit16u sequence);
