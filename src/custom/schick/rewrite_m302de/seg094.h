#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void TM_func8(signed short);

#if defined(__BORLANDC__)
/* 0x2a */
void TM_func1(signed short, signed short);
#endif

/* 0x2f */
void prepare_map_marker(void);
/* 0x34 */
/* static */
void set_textbox_positions(signed short);
/* 0x39 */
signed short TM_unused1(RealPt, signed short);
/* 0x3e */
signed short TM_get_looking_direction(signed short);

/* 0x43 */
signed short TM_get_track_length(Bit8u*);
/* 0x48 */
signed short TM_enter_target_town(void);
/* 0x4d */
void TM_draw_track(signed short, signed short, signed short, signed short);
/* 0x52 */
void TM_unused2(void);

#if !defined(__BORLANDC__)
}
#endif
