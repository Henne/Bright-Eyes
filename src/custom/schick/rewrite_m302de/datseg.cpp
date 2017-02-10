/**
 *	Rewrite of DSA1 v3.02_de datasegment
 *	No code is allowed here, only the global data
 */

#include "datseg.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short cd_init_successful = 0;

#if defined(__BORLANDC__)
char ds[DS_SIZE];
#endif

#if !defined(__BORLANDC__)
}
#endif
