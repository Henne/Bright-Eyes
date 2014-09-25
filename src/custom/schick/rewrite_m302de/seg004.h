#if !defined(__BORLANDC__)
namespace M302de {
#endif


void save_and_set_timer(void);
void set_timer(void);
void reset_timer(void);
void init_ani(Bit16u);
void set_var_to_zero(void);
void init_ani_busy_loop(unsigned short);
void clear_ani(void);
#if defined(__BORLANDC__)
void interrupt timer_isr(void);
#endif
void update_status_bars(void);
void draw_bar(unsigned short, signed short, signed short, signed short, signed short);
void restore_rect(RealPt, Bit8u*, unsigned short, unsigned short, signed char, signed char);
void restore_rect_rle(RealPt, Bit8u*, unsigned short, unsigned short, signed char, signed char, unsigned short);
void draw_mouse_cursor(void);
void save_mouse_bg(void);
void restore_mouse_bg(void);
void load_objects_nvf(void);
void update_wallclock(void);
void draw_wallclock(signed short, signed short);
void array_add(Bit8u*, signed short, signed short, signed short);
void schick_set_video(void);
void schick_reset_video(void);
void clear_ani_pal(void);
void set_ani_pal(Bit8u*);
void do_h_line(RealPt, signed short, signed short, signed short, signed char);
void do_v_line(RealPt, signed short, signed short, signed short, signed char);
void do_border(RealPt, signed short, signed short, signed short, signed short, signed char);
void do_pic_copy(unsigned short mode);
void do_save_rect();
void do_fill_rect(RealPt, signed short, signed short, signed short, signed short, signed short);
void wait_for_vsync();
void map_effect(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif
