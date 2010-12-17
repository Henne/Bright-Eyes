#include "dosbox.h"
#include "regs.h"
#include "callback.h"

#include "schick.h"

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
