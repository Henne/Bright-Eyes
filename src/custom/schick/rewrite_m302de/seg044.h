#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void FIG_prepare_hero_fight_ani(signed short, Bit8u*, signed short, signed short, signed char, signed char, signed short);

/* 0x25 */
void FIG_prepare_enemy_fight_ani(signed short, Bit8u*, signed short, signed char, signed char, signed short);

Bit16s copy_ani_seq(Bit8u*, Bit16s, Bit16u);
Bit8s seg044_00ae(Bit16s);

void seg044_002a(Bit16u, Bit8u*, Bit16u, Bit8s, Bit8s, Bit16u, Bit16u);
void seg044_002f(signed short, Bit8u*, signed short, signed char, signed char, signed short);

#if !defined(__BORLANDC__)
}
#endif
