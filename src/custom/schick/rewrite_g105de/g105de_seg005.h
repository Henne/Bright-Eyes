#if !defined(__BORLANDC__)
namespace G105de {
#else
extern "C" {
#endif

void set_video_mode(Bit16u mode);
void set_video_page(Bit16u mode);
void save_display_stat(RealPt);
void set_color(RealPt ptr, unsigned char color);
void set_palette(RealPt ptr, unsigned char first_color, unsigned short colors);
void draw_h_line(Bit16u, Bit16s, Bit16u);
void draw_h_spaced_dots(Bit16u, Bit16u, Bit16s, Bit16u);
void pic_copy(RealPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, RealPt, Bit16u);
void save_rect(Bit16u, Bit16u, RealPt, Bit16s, Bit16s);
void fill_rect(Bit16u, Bit16u, Bit16s, Bit16s, Bit16s);
unsigned short swap_u16(unsigned short val);
void copy_to_screen(RealPt, RealPt, Bit16s, Bit16s, Bit16s);

RealPt normalize_ptr(RealPt);

#if !defined(__BORLANDC__)
}
#else
}
#endif
