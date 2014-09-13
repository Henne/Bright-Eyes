#if !defined(__BORLANDC__)
namespace M302de {
#endif

RealPt FIG_get_ptr(signed char);

/* static */
signed char FIG_set_array(void);

void FIG_draw_figures(void);

void FIG_set_gfx(void);

void FIG_call_draw_pic(void);

void FIG_draw_pic(void);

RealPt FIG_get_hero_ptr(signed short);

RealPt seg006_033c(signed short);

void FIG_set_0e(signed char, signed char);

void FIG_reset_12_13(signed char);

void FIG_set_12_13(signed short);

void FIG_set_0f(signed char, signed char);

void FIG_remove_from_list(signed char, signed char);

signed char FIG_add_to_list(signed char);

void FIG_draw_char_pic(signed short, signed short);

void FIG_draw_enemy_pic(signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
