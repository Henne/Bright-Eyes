void draw_mouse_ptr_wrapper();
void call_mouse();
void draw_mouse_ptr();
void mouse();
namespace G105de {
	void mouse_compare();
	Bit16u get_mouse_action(Bit16u, Bit16u, Bit8u*);
	void decomp_rle(Bit8u*, Bit8u*, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u);
}
void update_mouse_ptr();
void save_mouse_ptr();
void do_draw_mouse_ptr();
namespace G105de {
	void split_textbuffer(Bit8u*, RealPt, Bit32u);
}
Bit32s get_archive_offset(Bit8u*, Bit8u*);
namespace G105de {
	Bit16u read_datfile(Bit16u, Bit8u*, Bit16u);
}
Bit32s get_filelength();
Bit16u ret_zero1();
namespace G105de {
	void wait_for_keypress();
}
Bit32u swap32(Bit16u, Bit16u);
void init_video();
void exit_video();
void draw_v_line(Bit16u, Bit16u, Bit16u, Bit16u);
namespace G105de {
	void do_draw_pic(Bit16u);
	void draw_popup_line(Bit16u, Bit16u);
}
void call_fill_rect_gen(PhysPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u);
void blit_smth3(PhysPt, Bit16u, Bit16u);
Bit16u print_line(char*);
void print_str(char *, Bit16u, Bit16u);
Bit16u str_splitter(char *);
Bit16u print_chr(unsigned char, Bit16u, Bit16u);
Bit8u get_chr_info(unsigned char, Bit8u*);
void call_them_all(Bit16u, Bit16u, Bit16u, Bit16u);
void fill_smth();
void fill_smth2(Bit8u*);
RealPt get_gfx_ptr(Bit16u, Bit16u);
Bit16u ret_zero();
void call_blit_smth3(PhysPt, Bit16u, Bit16u, Bit16u, Bit16u);
void set_vals(Bit16u, Bit16u);
void get_vals(Bit8u*, Bit8u*);
Bit16u get_str_width(char*);
Bit16u get_line_start_c(char*, Bit16u, Bit16u);
Bit16u enter_string(char*, Bit16u, Bit16u, Bit16u, Bit16u);
namespace G105de {
	void fill_radio_button(Bit16s, Bit16u, Bit16u);
}
void enter_name();
namespace G105de {
	void change_head();
	void change_sex();
}
void calc_at_pa();
void clear_hero();
void skill_inc_novice(Bit16u);
void spell_inc_novice(Bit16u);
namespace G105de {
	void save_picbuf();
	void restore_picbuf(PhysPt);
	void print_attribs();
	void make_valuta_str(char *, unsigned int);
	void pal_fade_out(Bit8u*, Bit8u*, Bit16u);
	void pal_fade_in(Bit8u*, Bit8u*, Bit16u, Bit16u);
	void set_mouse_isr();
	void restore_mouse_isr();
}
void init_colors();
void init_stuff();
