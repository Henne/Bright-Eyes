#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
#if defined(__BORLANDC__)
signed short TRV_found_camp_place(signed short);
#endif

/* 0x25 */
void TRV_found_replenish_place(signed short);

/* 0x2a */
signed short TRV_follow_trail_question(void);

/* 0x2f */
void TRV_found_herb_place(signed short);

/* 0x34 */
void TRV_inside_herb_place(void);

/* 0x39 */
#if defined(__BORLANDC__)
signed short TRV_cross_a_ford(Bit8u*, signed short, signed short);
#endif

void tevent_002(void);

/* 0x3e */
void seg109_067e(signed short, signed short);

/* 0x9d */
void TRV_load_textfile(signed short);

/* 0xa2 */
#if defined(__BORLANDC__)
void TRV_event(signed short);
#endif

/* 0xa7 */
signed short TRV_fight_event(signed short, signed short);

/* 0xac */
#if defined(__BORLANDC__)
void TRV_found_inn(signed short, signed short);
#endif

/* 0xb1 */
signed short TRV_enter_hut_question(void);
#if !defined(__BORLANDC__)
}
#endif
