#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x25 */
short test_spell(Bit8u *hero, unsigned short spell, signed char bonus);

/* 0x34 */
/* static */
void magic_heal_ani(Bit8u*);
/* 0x39 */
void FIG_do_spell_damage(signed short);
/* 0x3e */
signed short get_attackee_parade(void);
/* 0x48 */
signed short get_spell_cost(signed short, signed short);

/* 0x52 */
unsigned short test_spell_group(unsigned short spell, signed char bonus);

#if !defined(__BORLANDC__)
}
#endif
