namespace M302de {

/* 0x39 */
void FIG_do_spell_damage(signed short);
/* 0x3e */
signed short get_attackee_parade(void);
short get_spell_cost(unsigned short spell, unsigned short half_cost);
short test_spell(Bit8u *hero, unsigned short spell, signed char bonus);
unsigned short test_spell_group(unsigned short spell, signed char bonus);

}
