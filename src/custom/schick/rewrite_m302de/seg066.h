#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
/* static */
void refresh_floor_and_sky(void);
#endif

/* 0x20 */
signed short enter_location(signed short);
/* 0x25 */
signed short enter_location_daspota(void);

/* 0x2a */
#if !defined(__BORLANDC__)
signed short get_border_index(unsigned short);
#endif

/* 0x2f */
void do_special_buildings(void);
/* 0x34 */
void TLK_eremit(signed short);
/* 0x39 */
void do_town(void);

#if defined(__BORLANDC__)
/* 0x3e */
void seg066_0692(void);
#endif

#if defined(__BORLANDC__)
/* 0x43 */
void seg066_0d1d(void);
#endif

/* 0x4d */
void load_city_texture(signed short, signed short, signed short, signed short);

#if defined(__BORLANDC__)
/* 0x52 */
void seg066_0c50(void);
#endif

#if defined(__BORLANDC__)
/* 0x5c */
void seg066_159b(void);
#endif

/* 0x61 */
void seg066_172b(void);

#if defined(__BORLANDC__)
/* 0x66 */
void seg066_0bad(void);
#endif

/* 0x6b */
void seg066_06c1(void);

/* 0x70 */
#if defined(__BORLANDC__)
void city_step(void);
#endif

#if !defined(__BORLANDC__)
}
#endif
