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
void draw_bar(unsigned short type, unsigned short hero, unsigned short pts_cur, unsigned short pts_max, unsigned short mode);
void restore_rect(PhysPt ptr1, Bit8u *ptr2, unsigned short x, unsigned short y, char n, char m);
void restore_rect_rle(PhysPt ptr1, Bit8u *ptr2, unsigned short x, unsigned short y, char n ,char m, unsigned short v1);
void draw_mouse_cursor();
void save_mouse_bg();
void restore_mouse_bg();
void load_objects_nvf(void);
void draw_wallclock(signed short, signed short);
void array_add(PhysPt dst, unsigned short len, unsigned char op, unsigned short flag);
void schick_set_video();
void schick_reset_video();
void clear_ani_pal();
void set_ani_pal(Bit8u*);
void do_h_line(PhysPt ptr,unsigned short x1, unsigned short x2, unsigned short y, char color);
void do_v_line(PhysPt ptr, unsigned short, unsigned short, unsigned short, char);
void do_border(PhysPt dst, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, char color);
void do_pic_copy(unsigned short mode);
void do_save_rect();
void do_fill_rect(RealPt dst, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color);
void wait_for_vsync();
void map_effect(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif
