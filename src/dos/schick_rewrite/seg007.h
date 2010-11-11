unsigned short random_interval(unsigned short lo, unsigned short hi);
unsigned short random_schick(short val);
short dice_roll(unsigned short n, unsigned short m, short x);
void calc_damage_range(unsigned short n, unsigned short m, short x,
	Bit8u *min, Bit8u *max);
unsigned short is_in_word_array(unsigned short val, Bit8u *p);
unsigned short is_in_byte_array(char val, Bit8u *p);
short dice_template(unsigned short val);
void damage_range_template(unsigned short val, Bit8u *min, Bit8u *max);
