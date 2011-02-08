#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schick.h"

#include "seg000.h"

signed short bioskey(signed short cmd) {
	reg_ah = cmd &  0xff;
	reg_al = 0;
	CALLBACK_RunRealInt(0x16);

	if (GETFLAG(ZF)) {
		if (!(cmd & 1))
			return reg_ax;

		return 0;
	} else {
		if (!(cmd & 1))
			return reg_ax;
		if (reg_ax)
			return reg_ax;
		return -1;
	}
}

Bit16s bc_close(Bit16u handle) {

	if (handle >= ds_readw(0xb786))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return bc__close(handle);

}

Bit16s bc__close(Bit16u handle) {

	if (!DOS_CloseFile(handle))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return 0;
}
