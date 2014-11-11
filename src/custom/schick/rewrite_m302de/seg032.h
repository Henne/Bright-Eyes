#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x25
#if !defined(__BORLANDC__)
void FIG_set_cb_field(signed short, signed short, signed char);
#else
void FIG_set_cb_field(signed short, signed short, signed short);
#endif

//0x2a
void draw_fight_screen_pal(signed short);
//0x2f
signed short FIG_choose_next_hero(void);
//0x34
signed short FIG_choose_next_enemy(void);
//0x39
signed short FIG_count_active_enemies(void);
//0x3e
signed short FIG_is_enemy_active(Bit8u*);
//0x43
signed short FIG_get_first_active_hero(void);
//0x48
unsigned short seg032_02db(void);
//0x4d
unsigned short FIG_fight_continues(void);
//0x52
void FIG_do_round(void);

#if !defined(__BORLANDC__)
}
#endif
