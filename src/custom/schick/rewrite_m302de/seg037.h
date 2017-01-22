#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
signed short copy_ani_stuff(Bit8u*, signed short, signed short);
#endif

/* 0x20 */
/* should be static */
void seg037_00ae(Bit8u*, signed short);

/* 0x25 */
/* should be static */
unsigned short test_foe_melee_attack(signed short, signed short, signed short, signed short, signed short);

/* 0x2a */
/* should be static */
signed short test_foe_range_attack(signed short, signed short, const signed short, signed short);

/* 0x2f */
/* should be static */
signed short get_foe_attack_mode(signed short, signed short);

/* 0x34 */
/* should be static */
signed short seg037_0791(Bit8u*, signed short, signed short, signed short, signed short);

/* 0x39 */
/* should be static */
signed short seg037_0b3e(Bit8u*, signed short, signed short, signed short, signed short);

/* 0x3e */
void enemy_turn(Bit8u *, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
