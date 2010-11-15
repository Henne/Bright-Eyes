#include <string.h>

#include "../schick.h"

void seg041_1fd() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
}

