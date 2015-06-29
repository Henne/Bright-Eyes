/**
 *	Rewrite of DSA1 v3.02_de functions of seg067 (city)
 *	Functions rewritten: 1/13
 */

#include "v302de.h"

#include "seg007.h"
#include "seg026.h"
#include "seg067.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	select a random city event type
 */
/* Borlandified and identical */
void city_event_switch(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	/* load STRASSE.LTX */
	load_buffer_1(224);

	/* set city flag */
	ds_writeb(C_EVENT_ACTIVE, 1);

	switch (random_schick(9)) {
		case 1: city_event_1(); break;
		case 2: city_event_2(); break;
		case 3: city_event_3(); break;
		case 4: city_event_4(); break;
		case 5: city_event_5(); break;
		case 6: city_event_6(); break;
		case 7: city_event_7(); break;
		case 8: city_event_8(); break;
		case 9: city_event_9(); break;
	}

	/* reset city flag */
	ds_writeb(C_EVENT_ACTIVE, 0);

	/* load the LTX-file of the current town */
	load_buffer_1(ds_readbs(CURRENT_TOWN) + 77);

	/* update the current position / make the step */
	ds_writews(X_TARGET, ds_readws(0x2d83));
	ds_writews(Y_TARGET, ds_readws(0x2d85));
#endif
}

void city_event_1(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_2(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_3(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_4(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_5(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_6(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_7(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_8(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

void city_event_9(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

RealPt waffinfo_weapons(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

RealPt waffinfo_herbs(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}

RealPt waffinfo_general(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
}


#if !defined(__BORLANDC__)
}
#endif
