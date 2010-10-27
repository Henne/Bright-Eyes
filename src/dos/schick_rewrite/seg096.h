//20
extern void GUI_write_char_to_screen(PhysPt dst, unsigned short v1, unsigned short v2);
//39
extern RealPt GUI_get_ptr(unsigned short v1, unsigned short v2);
//3e
extern RealPt GUI_get_ptr2(unsigned short v1, unsigned short v2);
//48
extern unsigned short GUI_print_char(char c, unsigned short x, unsigned short y);
//52
extern unsigned short GUI_count_lines(Bit8u *str);
//57
extern unsigned short GUI_lookup_char(char c, unsigned short *p);
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
