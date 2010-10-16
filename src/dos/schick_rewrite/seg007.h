extern unsigned short random_interval(unsigned short lo, unsigned short hi);
extern unsigned short random_schick(short val);
extern short dice_roll(unsigned short n, unsigned short m, short x);
extern void calc_damage_range(unsigned short n, unsigned short m, short x,
	Bit8u *min, Bit8u *max);
extern unsigned short is_in_word_array(unsigned short val, Bit8u *p);
extern unsigned short is_in_byte_array(char val, Bit8u *p);
extern short dice_template(unsigned short val);
extern void damage_range_template(unsigned short val, Bit8u *min, Bit8u *max);
