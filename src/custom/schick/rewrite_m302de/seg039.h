#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
signed short seg039_0000(unsigned short, unsigned short, unsigned short, unsigned short);
//0x25
signed short seg039_0023(Bit8u *);
//0x2a
void fill_enemy_sheet(unsigned short, signed char, unsigned char);
//0x2f
unsigned short place_obj_on_cb(unsigned short, unsigned short, signed short, signed char, signed char);

//0x3e
void FIG_init_heroes();
#if !defined(__BORLANDC__)
}
#endif
