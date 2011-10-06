namespace M302de {

//25
char GUI_lookup_char_height(char c, unsigned short *p);
//2a
void GUI_1c2(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4);
//2f
signed short GUI_enter_text(Bit8u *, unsigned short, unsigned short, unsigned short, unsigned short);
//0x34
signed short GUI_input(Bit8u*, unsigned short);
//0x39
void GUI_output(Bit8u*);
//0x43
signed short GUI_radio(Bit8u*, signed char, ...);
//4d
void GUI_draw_radio_bg(Bit16u, Bit16u, Bit16u, Bit16u);
// 52
//static
void GUI_copy_smth(unsigned short width, unsigned short height);
//0x57
void GUI_fill_radio_button(signed short, unsigned short, unsigned short);
//0x5c
signed short GUI_dialogbox(RealPt, Bit8u*, Bit8u*, signed char, ...);
//0x61
signed short GUI_menu_input(unsigned short, unsigned short, unsigned short);
//0x6b
void GUI_dialog_na(unsigned short, Bit8u*);

}
