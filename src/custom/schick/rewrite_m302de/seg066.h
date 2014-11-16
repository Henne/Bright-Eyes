#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x2a */
#if !defined(__BORLANDC__)
signed short get_border_index(unsigned char);
#else
signed short get_border_index(unsigned short);
#endif

/* 0x39 */
void do_town(void);

/* 0x4d */
void load_city_texture(signed short, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
