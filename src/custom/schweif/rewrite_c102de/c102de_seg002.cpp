#include "schweif.h"

#if !defined(__BORLANDC__)
namespace C102de {
#endif

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
