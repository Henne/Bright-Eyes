#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_temple(void);
/* 0x25 */
void char_add(signed short);
/* 0x29 */
void char_letgo(signed short);
/* 0x2f */
signed short char_erase(void);
/* 0x34 */
void miracle_heal_hero(signed short, Bit8u*);
/* 0x39 */
void miracle_resurrect(Bit8u*);
/* 0x3e */
void miracle_modify(unsigned short, Bit32s, signed short);
/* 0x43 */
void miracle_weapon(Bit8u*, signed short);

#if !defined(__BORLANDC__)
}
#endif
