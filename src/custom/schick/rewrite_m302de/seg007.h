#if !defined(__BORLANDC__)
namespace M302de {
#endif

int random_interval(const int, const int);
int random_schick(const int);
int dice_roll(const int, const int, const int);
void calc_damage_range(const int, const int, const int, Bit8u*, Bit8u*);
int is_in_word_array(const int, signed short*);
int is_in_byte_array(const signed char, Bit8u*);
int dice_template(const unsigned short);
void damage_range_template(unsigned short val, Bit8u *min, Bit8u *max);

#if !defined(__BORLANDC__)
}
#endif
