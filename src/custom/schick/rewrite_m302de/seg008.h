#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short swap_u16(unsigned short val);
void set_video_mode(signed short);
void set_video_page(signed short);
void save_display_stat(RealPt);
void set_color(Bit8u *ptr, unsigned char color);
void set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors);
void draw_h_line(RealPt, unsigned short, signed short);
void draw_h_spaced_dots(RealPt, signed short, signed short, signed short);

void pic_copy(RealPt dst, short x1, short y1, short x2, short y2,
        unsigned short val1, unsigned short val2,
        unsigned short val3, unsigned short val4,
        unsigned short width, unsigned short height,
        Bit8u *src, unsigned short mode);

void save_rect(RealPt src, RealPt dest, unsigned short width, unsigned short height);
void fill_rect(RealPt ptr, signed short, signed short, signed short);
void copy_solid_permuted(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void copy_solid(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void decomp_rle(unsigned short width, unsigned short height,
	Bit8u *dst, Bit8u *src, Bit8u *tmp_buffer, unsigned short mode);

#if !defined(__BORLANDC__)
}
#endif
