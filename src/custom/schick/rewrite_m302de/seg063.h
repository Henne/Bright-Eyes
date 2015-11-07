#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* 0x20 */
void do_harbour(void);
#endif

/* 0x25 */
void passages_init(void);
/* 0x2a */
void sea_travel(signed short, signed short);
/* 0x2f */
void mod_clock_pos(signed short);
/* 0x34 */
signed short get_srout_len(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif
