#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
signed short seg039_0000(signed short, signed short, signed short, signed short);
/* 0x25 */
signed short FIG_get_range_weapon_type(Bit8u *);
//0x2a
void fill_enemy_sheet(unsigned short, signed char, unsigned char);
//0x2f
unsigned short place_obj_on_cb(unsigned short, unsigned short, signed short, signed char, signed char);

/* 0x34 */
void FIG_load_enemy_sprites(Bit8u *, signed char, signed char);

/* 0x39 */
void FIG_init_enemies(void);

//0x3e
void FIG_init_heroes();
#if !defined(__BORLANDC__)
}
#endif
