#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
void npc_nariell(void);
//0x25
void npc_harika(void);
//0x2a
void npc_curian(void);
//0x2f
void npc_ardora(void);
//0x34
void npc_garsvik(void);
//0x39
void npc_erwo(void);
//0x3e
void remove_npc(signed short, signed char, signed short, Bit8u*, Bit8u*);
//0x43
void add_npc(signed short);
//0x48
unsigned short npc_meetings(unsigned short);
//0x4d
void npc_farewell(void);

#if !defined(__BORLANDC__)
}
#endif
