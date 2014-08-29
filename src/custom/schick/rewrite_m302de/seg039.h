#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short calc_beeline(signed short, signed short, signed short, signed short);
/* 0x25 */
signed short FIG_get_range_weapon_type(Bit8u *);
/* 0x2a */
void fill_enemy_sheet(unsigned short, signed char, unsigned char);


/* 0x2f */
#if !defined(__BORLANDC__)
unsigned short place_obj_on_cb(signed short, signed short, signed short, signed char, signed char);
#else
unsigned short place_obj_on_cb(signed short, signed short, signed short, signed short, signed short);
#endif

/* 0x34 */
void FIG_load_enemy_sprites(Bit8u *, signed short, signed short);

/* 0x39 */
void FIG_init_enemies(void);

/* 0x3e */
void FIG_init_heroes(void);
#if !defined(__BORLANDC__)
}
#endif
