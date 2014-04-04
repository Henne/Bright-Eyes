#include "schweif.h"

#if !defined(__BORLANDC__)
namespace C102de {
#endif

signed short get_item_pos(Bit8u *hero, signed short item_id)
{
	signed short i;

	for (i = 0; i < 31; i++) {

		if ((signed short)host_readw(hero + 0x1a8 + i * 0x1b) == item_id) {
			return i;
		}
	}

	return -1;
}

signed short hero_is_diseased(Bit8u* hero)
{
	signed short i;

	for (i = 1; i <= 7; i++) {
		if (host_readb(hero + 0xc0 + i * 5) == 0xff) {
			return i;
		}
	}

	return 0;
}

signed short hero_is_poisoned(Bit8u* hero)
{
	signed short i;

	for (i = 1; i <= 9; i++) {
		if (host_readb(hero + 0xe8 + i * 5) == 0xff) {
			return i;
		}
	}

	return 0;

}

#if !defined(__BORLANDC__)
}
#endif
