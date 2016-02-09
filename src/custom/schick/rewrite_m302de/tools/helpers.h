/* accessors in Little Endian */
static inline unsigned char read_u8(const unsigned char *b)
{
	return *(unsigned char*)(b);
}

static inline unsigned short read_u16(const unsigned char *b)
{
	return (unsigned short)(read_u8(b) + (read_u8(b + 1) << 8));
}

static inline unsigned int read_u32(const unsigned char *b)
{
	return (unsigned int)(read_u16(b) + (read_u16(b + 2) << 16));
}
