#if !defined(__BORLANDC__)
namespace G105de {
#endif

#include "g105de_seg007.h"

#include "port.h"

//static
unsigned short CD_set_drive_no();

//static
#if defined(__BORLANDC__)
//void CD_driver_request(driver_request far*);
#else
//void CD_driver_request(RealPt);
#endif

//static
Bit32s CD_get_tod();

	//static
	void seg001_00bb(Bit16s);

//static
signed short CD_bioskey(signed short);

//static
void seg001_0312();

void seg001_033b();

void seg001_03a8();

void seg001_0465(unsigned short);
//static
Bit16s CD_check_file(char*);

signed short seg001_0600();

#if !defined(__BORLANDC__)
}
#endif
