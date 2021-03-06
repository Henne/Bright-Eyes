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
/* 0x3e */
void seg066_0692(void);
/* 0x43 */
void city_building_textures(void);
/* 0x48 */
void seg066_10c8(void);
/* 0x4d */
void load_city_texture(signed short, signed short, signed short, signed short);
/* 0x52 */
void city_water_and_grass(void);
/* 0x57 */
void seg066_14dd(signed short);
/* 0x5c */
void city_fade_and_colors(void);
/* 0x61 */
void seg066_172b(void);
/* 0x66 */
void seg066_0bad(void);
/* 0x6b */
void seg066_06c1(void);
/* 0x70 */
signed short city_step(void);

#if !defined(__BORLANDC__)
}
#endif
