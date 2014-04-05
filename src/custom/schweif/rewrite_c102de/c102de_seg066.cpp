
#include "schweif.h"

#include "c102de_seg002.h"

#if !defined(__BORLANDC__)
namespace C102de {
#endif

signed char get_skill_value(Bit8u *hero, signed short talent)
{

	/*	Mit dem magischen Schwert aus den Blutzinnen im Gepaeck
		wir der Wert von Koerperbeherrschung um Eins reduziert */
	if ((talent == 13) && (get_item_pos(hero, 0x154) != -1) ) {

		return (signed char)host_readb(hero + 0x11a + talent) - 1;
	}

	return (signed char)host_readb(hero + 0x11a + talent);
}

#if !defined(__BORLANDC__)
}
#endif
