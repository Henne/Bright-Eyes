#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x25
void FIG_set_cb_field(signed short, signed short, signed short);
//0x2a
void draw_fight_screen_pal(signed short);
//0x2f
signed short FIG_choose_next_hero(void);
//0x34
signed short FIG_choose_next_enemy(void);
//0x39
unsigned short FIG_count_active_enemies();
//0x3e
unsigned short FIG_is_enemy_active(Bit8u *);
//0x43
signed short FIG_get_first_active_hero();
//0x48
unsigned short seg032_02db();
//0x4d
unsigned short FIG_fight_continues();

#if !defined(__BORLANDC__)
}
#endif
