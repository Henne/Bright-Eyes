struct mouse_action {
	signed short x1;
	signed short y1;
	signed short x2;
	signed short y2;
	signed short action;
};

struct nvf_desc {
#if !defined(__BORLANDC__)
	RealPt dst;
	RealPt src;
#else
	RealPt far dst;
	RealPt far src;
#endif
	signed short no;
	signed char type;
	signed short *width;
	signed short *height;
};

#if !defined(__BORLANDC__)
namespace G105de {
#endif

	void start_music(Bit16u);
	void read_soundcfg();
	void init_music(unsigned long);
	void stop_music();
	RealPt load_snd_driver(RealPt);
	void unload_snd_driver();
	unsigned short load_seq(Bit16s);
	unsigned short play_sequence(Bit16s);
	RealPt get_timbre(Bit16s, Bit16s);
	unsigned short call_load_file(Bit16s);
	unsigned short load_file(Bit16s);
	void restart_midi();
	unsigned short load_driver(RealPt, Bit16s, Bit16s);
	void play_midi(Bit16u);
	void stop_sequence();

	void do_mouse_action(Bit8u*, Bit8u*, Bit8u*, Bit8u*, Bit8u*);
#if defined(__BORLANDC__)
	void interrupt mouse_isr(void);
#endif
	void mouse_enable();
	void mouse_disable();
	void mouse_do_enable(Bit16u, RealPt);
	void mouse_do_disable();
	void update_mouse_cursor();
	void call_mouse();
	void update_mouse_cursor1();
	void mouse();
	void mouse_compare();
	void handle_input();
	Bit16u get_mouse_action(Bit16s, Bit16s, struct mouse_action*);
	void decomp_rle(Bit8u*, Bit8u*, Bit16s, Bit16s, Bit16s, Bit16s, Bit16u);
	void draw_mouse_cursor();
	void save_mouse_bg();
	void restore_mouse_bg();
	void load_font_and_text();
	void split_textbuffer(Bit8u*, RealPt, Bit32u);
	void load_font_and_text_host();
	void load_page(Bit16s);
	void load_typus(Bit16u);
	void save_chr();
	void read_common_files();
	Bit32s process_nvf(struct nvf_desc*);
	Bit16s open_datfile(Bit16u);
	Bit32s get_archive_offset(const char*, Bit8u*);
	Bit16s read_datfile(Bit16u, Bit8u*, Bit16u);
	Bit32s get_filelength(Bit16s);
	Bit16u ret_zero1();
	void wait_for_keypress();
	void error_msg(Bit8u*);
	void vsync_or_key(Bit16s);
	Bit32u swap_u32(Bit32u);
	void init_video(Bit16s);
	void exit_video();
	void draw_v_line(Bit16s, Bit16s, Bit16s, Bit16u);
	void do_draw_pic(Bit16u);
	void call_fill_rect_gen(PhysPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u);
	void wait_for_vsync();
	void blit_smth3(RealPt, Bit16s, Bit16s);
	Bit16u str_splitter(char *);
	Bit16u print_line(char*);
	void print_str(char *, Bit16s, Bit16s);
	Bit16s print_chr(unsigned char, Bit16s, Bit16s);
	Bit16s get_chr_info(unsigned char, Bit16s*);
	void call_them_all(Bit16s, Bit16s, Bit16s, Bit16s);
	void fill_smth();
	void fill_smth2(Bit8u*);
	RealPt get_gfx_ptr(Bit16s, Bit16s, Bit16s*);
	Bit16s ret_zero(Bit16s, Bit16s);
	void call_blit_smth3(RealPt, Bit16s, Bit16s, Bit16s, Bit16s);
	void set_textcolor(Bit16s, Bit16s);
	void get_textcolor(Bit16s*, Bit16s*);
	Bit16s get_str_width(char*);
	Bit16s get_line_start_c(char*, Bit16s, Bit16s);
	Bit16s enter_string(char*, Bit16s, Bit16s, Bit16s, Bit16s);
	void draw_popup_line(Bit16s, Bit16s);
	Bit16s infobox(char*, Bit16s);
	Bit16s gui_bool(Bit8u*);
	void fill_radio_button(Bit16s, Bit16s, Bit16s);
	Bit16s gui_radio(Bit8u *header, Bit8s pos, ...);
	void enter_name();
	void change_head();
	void change_sex();
	void do_gen();
	void calc_at_pa();
	void fill_values();
	void refresh_screen();
	void clear_hero();
	void new_values();
	void skill_inc_novice(Bit16s);
	void spell_inc_novice(Bit16s);
	void select_typus();
	Bit16s can_change_attribs();
	void change_attribs();
	void save_picbuf();
	void restore_picbuf(RealPt);
	void print_attribs();
	void print_values();
	void make_valuta_str(char *, Bit32s);
	void inc_skill(Bit16s, Bit16s, char*);
	void select_skill();
	void inc_spell(Bit16s);
	void select_spell();
	void choose_atpa();
	void choose_typus();
	void pal_fade_out(Bit8u*, Bit8u*, Bit16s);
	void pal_fade_in(Bit8u*, Bit8u*, Bit16s, Bit16s);
	void intro();
#if defined(__BORLANDC__)
	void interrupt timer_isr(void);
#endif
	void set_timer_isr();
	void restore_timer_isr();
	int main_gen(int argc, char **argv);
	void alloc_buffers_emu();
	void alloc_buffers();
	void init_colors();
	void init_stuff();
	RealPt gen_alloc(Bit32u);

#if !defined(__BORLANDC__)
}
#endif
