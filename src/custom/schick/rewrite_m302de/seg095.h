#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
void npc_nariell();
//0x25
void npc_harika();
//0x2a
void npc_curian();
//0x2f
void npc_ardora();
//0x34
void npc_garsvik();
//0x39
void npc_erwo();
//0x3e
void remove_npc(signed short, signed char, signed short, Bit8u*, Bit8u*);
//0x43
void add_npc(signed short);
//0x48
unsigned short npc_meetings(unsigned short);
//0x4d
void npc_farewell();

#if !defined(__BORLANDC__)
}
#endif
