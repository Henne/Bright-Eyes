#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
void unused_store(signed short);
//0x2a
void load_area_description(unsigned short);
//0x34
void prepare_dungeon_area(void);
//0x39
void load_map(void);
//0x3e
void load_dungeon_ddt(void);
//0x4d
void load_special_textures(signed short);
//0x52
void call_load_buffer(void);
//0x5c
void call_load_area(unsigned short);
//0x61
void load_npc(signed short);
//0x66
void save_npc(signed short);
/* 0x6b */
void load_splashes(void);
/* 0x70 */
void load_informer_tlk(signed short);
//0x75
void load_tlk(signed short);
//0x7a
void load_fightbg(signed short index);

#if !defined(__BORLANDC__)
}
#endif
