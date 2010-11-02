//20
extern void GUI_write_char_to_screen(PhysPt dst, unsigned short v1, unsigned short v2);
//25
extern RealPt GUI_names_grammar(unsigned short v1, unsigned short v2, unsigned short v3);
//2a
extern RealPt GUI_name_plural(unsigned short v1, Bit8u *s);
//2f
extern RealPt GUI_name_singular(Bit8u *s);
//34
extern RealPt GUI_2f2(unsigned short v1, unsigned short v2, unsigned short v3);
//39
extern RealPt GUI_get_ptr(unsigned short v1, unsigned short v2);
//3e
extern RealPt GUI_get_ptr2(unsigned short v1, unsigned short v2);
//43
extern void GUI_print_string(Bit8u *str, unsigned short x, unsigned short y);
//48
extern unsigned short GUI_print_char(unsigned char c, unsigned short x, unsigned short y);
//4d
extern unsigned short GUI_get_first_pos_centered(Bit8u *p, unsigned short v1, unsigned short v2, unsigned short v3);
//52
extern unsigned short GUI_count_lines(Bit8u *str);
//57
extern unsigned short GUI_lookup_char_width(unsigned char c, unsigned short *p);
//5c
extern unsigned short GUI_unused(Bit8u *str);
//61
extern void GUI_write_fonti_to_screen(unsigned short fi, unsigned short char_width, unsigned short x, unsigned short y);
//66
extern void GUI_blank_char();
//6B
extern void GUI_font_to_buf(Bit8u *p);
//70
extern void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width);
//75
extern void GUI_set_smth(unsigned short v1, unsigned short v2);
extern void GUI_get_smth(Bit8u *p1, Bit8u *p2);
//84
extern unsigned short GUI_print_header(Bit8u *str);
//89
extern unsigned short GUI_get_space_for_string(Bit8u *p, unsigned short dir);
//8e
extern void GUI_draw_popup_line(unsigned short line, unsigned short type);
