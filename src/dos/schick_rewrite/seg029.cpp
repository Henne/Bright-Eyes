#include "schick.h"

#include "seg002.h"
#include "seg004.h"

void clear_loc_line() {
	update_mouse_cursor();
	do_fill_rect(ds_readd(0xd2ff), 3, 140, 316, 153, 0);
	refresh_screen_size();
}
