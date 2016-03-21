#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* 0x20 */
void TM_func8(signed short);

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

#if defined(__BORLANDC__)
/* 0x43 */
signed short TM_get_track_length(Bit8u*);
#endif


#if !defined(__BORLANDC__)
}
#endif
