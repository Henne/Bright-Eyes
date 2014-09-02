#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* 0x20 */
void show_automap(void);
#endif

/* 0x25 */
signed short select_teleport_dest(void);

/* 0x2f */
/* should be static */
void seg074_305(signed short);

/* 0x2a */
/* should be static */
unsigned short is_discovered(signed short, signed short);

/* 0x34 */
/* should be static */
void draw_automap_square(signed short, signed short, signed short, signed short);

/* 0x39 */
/* should be static */
void draw_automap_entrance(signed short, signed short, signed short);

/* 0x3e */
/* should be static */
void draw_automap_to_screen(void);

/* 0x43 */
/* should be static */
unsigned short get_mapval_small(signed short, signed short);

/* 0x48 */
/* should be static */
unsigned short get_mapval_large(signed short, signed short);

/* 0x4d */
/* should be static */
signed short is_group_in_prison(signed short);

/* 0x52 */
/* should be static */
signed short seg074_bbb(signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
