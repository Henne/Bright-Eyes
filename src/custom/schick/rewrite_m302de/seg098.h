#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* 0x20 */
signed short use_magic(Bit8u*);
#endif

/* 0x25 */
signed short test_spell(Bit8u*, signed short, signed char);

#if defined(__BORLANDC__)
/* 0x2a */
signed short use_spell(Bit8u*, signed short, signed char);
#endif

/* 0x2f */
signed short select_spell(Bit8u*, signed short);

/* 0x34 */
/* static */
void magic_heal_ani(Bit8u*);
/* 0x39 */
void FIG_do_spell_damage(signed short);
/* 0x3e */
signed short get_attackee_parade(void);
/* 0x48 */
signed short get_spell_cost(signed short, signed short);
/* 0x4d */
/* can be static */
signed short can_use_spellclass(Bit8u*, signed short);
/* 0x52 */
signed short test_spell_group(signed short, signed char);
#if defined(__BORLANDC__)
/* 0x57 */
signed short select_magic_user(void);
#endif

#if !defined(__BORLANDC__)
}
#endif
