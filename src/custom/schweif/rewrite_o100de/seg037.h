namespace schweif_o100de {

unsigned short random_interval(unsigned short lo, unsigned short hi);
unsigned short random_schweif(short val);
short dice_roll(unsigned short num, unsigned short faces, short plus);
void calc_damage_range(unsigned short num, unsigned short faces, short plus,
	Bit8u *min, Bit8u *max);

}
