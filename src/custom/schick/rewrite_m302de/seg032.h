#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
signed short do_fight(signed short);
//0x25
void FIG_set_cb_field(signed short, signed short, signed short);
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
//0x57
void FIG_load_ship_sprites(void);

#if !defined(__BORLANDC__)
}
#endif
