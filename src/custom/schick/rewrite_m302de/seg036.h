#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void seg036_00ae(Bit8u*, signed short);

/* 0x25 */
/* should be static */
signed short KI_change_hero_weapon(Bit8u*);

/* 0x2a */
/* should be static */
signed short KI_can_attack_neighbour(signed short, signed short, signed short,
					signed short, signed short);
/* 0x2f */
/* should be static */
signed short KI_search_spell_target(signed short, signed short, signed short, signed short);

/* 0x34 */
/* should be static */
signed short KI_select_spell_target(Bit8u*, signed short, signed short, signed short, signed short);

/* 0x39 */
/* should be static */
signed short KI_get_spell(signed short, signed short);

/* 0x34 */
/* should be static */
signed short seg036_8cf(Bit8u*, signed short, signed short, signed short, signed short);

/* 0x43 */
/* should be static */
signed short KI_count_heroes(signed short);

/* 0x48 */
void KI_hero(Bit8u*, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
