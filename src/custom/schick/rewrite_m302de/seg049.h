#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void GRP_swap_heros(void);
/* 0x25 */
void GRP_move_hero(signed short);
/* 0x2a */
void GRP_hero_sleep(Bit8u*, signed short);

/* 0x2f */
void GRP_sort_heros(void);
/* 0x34 */
void GRP_split(void);
/* 0x39 */
void GRP_merge(void);
/* 0x3e */
void GRP_switch_to_next(signed short);
/* 0x43 */
int GRP_compare_heros(const void*, const void*);
/* 0x48 */
void GRP_save_pos(signed short);
#if !defined(__BORLANDC__)
}
#endif
