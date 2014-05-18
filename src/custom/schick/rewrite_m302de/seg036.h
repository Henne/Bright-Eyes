#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* static */
signed short KI_copy_ani_sequence(Bit8u*, signed short, signed short);

/* 0x20 */
void seg036_00ae(Bit8u*, signed short);

/* 0x25 */
signed short KI_change_hero_weapon(Bit8u*);

/* 0x2a */
signed short KI_can_attack_neighbour(signed short, signed short, signed short,
					signed short, signed short);
/* 0x39 */
signed short KI_get_spell(signed short, signed short);

/* 0x43 */
signed short KI_count_heros(signed short);

#if !defined(__BORLANDC__)
}
#endif
