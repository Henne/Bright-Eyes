#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x2f */
void TRV_found_herb_place(signed short);

/* 0x34 */
void TRV_inside_herb_place(void);

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

/* 0xb1 */
signed short enter_hut_question(void);
#if !defined(__BORLANDC__)
}
#endif
