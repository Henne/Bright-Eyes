#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* NOT EXPORTED */
signed short copy_ani_stuff(Bit8u*, signed short, signed short);

/* 0x20 */
/* static */
void seg037_00ae(Bit8u*, signed short);

/* 0x25 */
/* static */
unsigned short test_foe_melee_attack(signed short, signed short, signed short, signed short, signed short);

/* 0x2a */
/* static */
signed short test_foe_range_attack(signed short, signed short, const signed short, signed short);

/* 0x2f */
/* static */
signed short get_foe_attack_mode(signed short, signed short);

/* 0x34 */
/* static */
signed short seg037_0791(Bit8u*, signed short, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
