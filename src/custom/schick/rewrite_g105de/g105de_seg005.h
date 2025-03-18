#if !defined(__BORLANDC__)
namespace G105de {
#endif
void set_video_mode(Bit16u mode);
void set_video_page(Bit16u mode);
void save_display_stat(RealPt);
void set_color(Bit8u *ptr, unsigned char color);
void set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors);
void draw_h_line(Bit16u, Bit16s, Bit16u);
void draw_h_spaced_dots(Bit16u, Bit16u, Bit16s, Bit16u);
void pic_copy(RealPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, RealPt, Bit16u);
void save_rect(RealPt, RealPt, Bit16s, Bit16s);
void fill_rect(PhysPt ptr, unsigned char color, unsigned short width, unsigned short height);
unsigned short swap_u16(unsigned short val);
void copy_to_screen(PhysPt, PhysPt, Bit16s, Bit16s, Bit16u);

#if defined(__BORLANDC__)
RealPt _normalize_ptr(Bit32u);
#else
static inline RealPt _normalize_ptr(Bit32u) { }
#endif

#if !defined(__BORLANDC__)
}
#endif
