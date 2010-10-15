extern unsigned short random_interval(unsigned short lo, unsigned short hi);
extern unsigned short random_schick(short val);
extern short dice_roll(unsigned short n, unsigned short m, short x);
extern void calc_damage_range(unsigned short n, unsigned short m, short x,
	Bit8u *min, Bit8u *max);
