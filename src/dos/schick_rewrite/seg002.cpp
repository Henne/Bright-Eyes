#include <mem.h>
#include "../schick.h"

unsigned int get_readlength2(signed short index) {
	return index == -1 ? 0 : real_readd(datseg, 0xbce7);
}

unsigned short div16(unsigned char val) {
	return val >> 4;
}
