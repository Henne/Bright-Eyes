extern unsigned short swap_u16(unsigned short val);
extern void set_video_mode(unsigned char mode);
extern void set_video_page(unsigned char mode);
extern void set_color(Bit8u *ptr, unsigned char color);
extern void set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors);
extern void draw_h_line(PhysPt ptr, unsigned short count, unsigned char color);
extern void draw_h_spaced_dots(PhysPt ptr, unsigned short count, unsigned char color, unsigned short space);

extern void pic_copy(PhysPt dst, short x1, short y1, short x2, short y2,
        unsigned short val1, unsigned short val2,
        unsigned short val3, unsigned short val4,
        unsigned short width, unsigned short height,
        Bit8u *src, unsigned short mode);

extern void save_rect(PhysPt src, PhysPt dest, unsigned short width, unsigned short height);
extern void fill_rect(PhysPt ptr, unsigned char color, unsigned short width, unsigned short height);
extern void copy_solid_permuted(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
extern void copy_solid(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
extern void decomp_rle(unsigned short width, unsigned short height,
	Bit8u *dst, Bit8u *src, Bit8u *tmp_buffer, unsigned short mode);
