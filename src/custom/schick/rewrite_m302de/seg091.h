#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void DNG13_unblock_passage(Bit8u*, Bit8u*);

#if defined(__BORLANDC__)
/* 0x25 */
void DNG13_func3(void);
#endif

#if defined(__BORLANDC__)
/* 0x2a */
void DNG13_func4(void);
#endif

/* 0x2f */
void DNG13_corpse1(RealPt);
/* 0x34 */
void DNG13_corpse0(RealPt);
/* 0x39 */
void DNG13_chest1(RealPt);
/* 0x3e */
void DNG13_chest0(RealPt);

#if defined(__BORLANDC__)
/* 0x43 */
signed short DNG13_handler(void);
#endif

/* 0x48 */
void DNG13_fight_intro(signed short);

#if !defined(__BORLANDC__)
}
#endif
