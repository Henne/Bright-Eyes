#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_dungeon(void);
/* 0x25 */
void DNG_fallpit_test(signed short);
/* 0x2a */
void DNG_see_stairs(void);
/* 0x2f */
void DNG_door(signed short);
/* 0x34 */
void DNG_see_door(void);
/* 0x39 */
void DNG_see_chest(void);
/* 0x3e */
void DNG_fight(void);

#if defined(__BORLANDC__)
/* 0x43 */
void DNG_func6(void);
#endif

/* 0x48 */
void print_msg_with_first_hero(Bit8u*);

#if defined(__BORLANDC__)
/* 0x4d */
signed short DNG_step(void);
#endif

/* 0x52 */
void DNG_waterbarrel(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif
