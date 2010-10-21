/*
        Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
        Functions rewritten: 1/14
*/

unsigned short swap_u16(unsigned short val) {
	return (val << 8) | (val >> 8);
}
