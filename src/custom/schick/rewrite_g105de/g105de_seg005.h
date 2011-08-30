namespace G105de {
void set_video_mode(unsigned char mode);
void set_video_page(unsigned char mode);
void save_display_stat(RealPt);
void set_color(Bit8u *ptr, unsigned char color);
void set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors);
void draw_h_line(PhysPt ptr, unsigned short count, unsigned char color);
void draw_h_spaced_dots(PhysPt ptr, unsigned short count, unsigned char color, unsigned short space);
void draw_pic(PhysPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, PhysPt, Bit16u);
void fill_rect(PhysPt ptr, unsigned char color, unsigned short width, unsigned short height);
unsigned short swap_u16(unsigned short val);
void copy_to_screen(PhysPt, PhysPt, Bit16s, Bit16s, Bit16u);
}
