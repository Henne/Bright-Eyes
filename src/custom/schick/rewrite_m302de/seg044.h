#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
/* is static */
Bit16s copy_ani_seq(Bit8u*, Bit16s, Bit16u);
/* is static */
Bit8s get_seq_header(Bit16s);
#endif

/* 0x20 */
void FIG_prepare_hero_fight_ani(signed short, Bit8u*, signed short, signed short, signed char, signed char, signed short);

/* 0x25 */
void FIG_prepare_enemy_fight_ani(signed short, Bit8u*, signed short, signed char, signed char, signed short);

/* 0x2a */
void seg044_002a(Bit16u, Bit8u*, Bit16u, Bit8s, Bit8s, Bit16u, Bit16u);

/* 0x2f */
void seg044_002f(signed short, Bit8u*, signed short, signed char, signed char, signed short);

#if !defined(__BORLANDC__)
}
#endif
