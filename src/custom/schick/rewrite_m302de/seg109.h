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
signed short TRV_cross_a_ford(Bit8u*, signed short, signed short);

/* 0x3e */
void TRV_ford_test(signed short, signed short);

/* 0x43 */
signed short TRV_ferry(Bit8u*, signed short);

/* 0x4d */
#if defined(__BORLANDC__)
void TRV_hunt_generic(signed short, signed short, signed short, signed short,
			signed short, signed short, signed short, signed short,
			signed short, signed short, signed short);
#endif

/* 0x52 */
#if defined(__BORLANDC__)
void tevent_001(void);
#endif

/* 0x57 */
void tevent_002(void);

/* 0x5c */
void tevent_003(void);

/* 0x61 */
#if defined(__BORLANDC__)
void tevent_004(void);
#endif

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
