#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short init_memory(void);

/* 0x25 */
void init_game_state(void);

/* 0x2a */
void prepare_dirs(void);

/* 0x2f */
void cleanup_game(void);

/* 0x34 */
void rabies(RealPt, signed short);

/* 0x39 */
/* should be static */
void init_global_buffer(void);

/* 0x3e */
void refresh_colors(void);

/* 0x43 */
Bit32s get_diskspace(void);

/* 0x4d */
void game_over_screen(void);

/* 0x52 */
void call_gen(void);

#if !defined(__BORLANDC__)
}
#endif
