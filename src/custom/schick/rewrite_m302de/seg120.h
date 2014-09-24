#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short init_memory(void);

/* 0x25 */
void init_game_state(void);

/* 0x2f */
void cleanup_game(void);

/* 0x34 */
void rabies(RealPt, signed short);

/* 0x39 */
/* should be static */
void init_global_buffer(void);

/* 0x3e */
void refresh_colors(void);

#if !defined(__BORLANDC__)
}
#endif
