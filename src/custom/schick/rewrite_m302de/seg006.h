#if !defined(__BORLANDC__)
namespace M302de {
#endif

RealPt FIG_get_ptr(signed char v1);
void FIG_draw_figures(void);
signed char FIG_set_array();
void FIG_set_gfx();
void FIG_call_draw_pic();
void FIG_draw_pic(void);
RealPt FIG_get_hero_ptr(unsigned short v1);
RealPt seg006_033c(short v);
void FIG_set_0e(signed char id, signed char val);
void FIG_reset_12_13(signed char id);
void FIG_set_12_13(signed short);
void FIG_set_0f(signed char id, signed char val);
void FIG_remove_from_list(signed char id, signed char v2);
signed char FIG_add_to_list(signed char);
void FIG_draw_char_pic(unsigned short pos, unsigned short hero_nr);
void FIG_draw_enemy_pic(unsigned short pos, unsigned short enemy_nr);

#if !defined(__BORLANDC__)
}
#endif
