#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short TRV_found_camp_place(signed short);

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

/* 0x48 */
void TRV_barrier(signed short);

/* 0x4d */
void TRV_hunt_generic(signed short, signed short, signed short, signed short,
			signed short, signed short, signed short, signed short,
			signed short, signed short, signed short);

/* 0x52 */
void tevent_001(void);

/* 0x57 */
void tevent_002(void);

/* 0x5c */
void tevent_003(void);

/* 0x61 */
void tevent_004(void);

/* 0x66 */
void tevent_005(void);

/* 0x6b */
void tevent_006(void);

/* 0x70 */
void tevent_007(void);

/* 0x75 */
void tevent_008(void);

/* 0x7a */
void tevent_009(void);

/* 0x7f */
void tevent_010(void);

/* 0x84 */
void tevent_012(void);

/* 0x89 */
void tevent_018(void);

/* 0x8e */
void tevent_019(void);

/* 0x9d */
void TRV_load_textfile(signed short);

/* 0xa2 */
#if defined(__BORLANDC__)
void TRV_event(signed short);
#endif

/* 0xa7 */
signed short TRV_fight_event(signed short, signed short);

/* 0xac */
void TRV_found_inn(signed short, signed short);

/* 0xb1 */
signed short TRV_enter_hut_question(void);
#if !defined(__BORLANDC__)
}
#endif
