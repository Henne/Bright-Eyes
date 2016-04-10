#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_dungeon(void);
/* 0x25 */
void DNG_fallpit_test(signed short);
/* 0x2a */
void DNG_stairs(void);

/* 0x2f */
void DNG_door(signed short);
/* 0x34 */
void DNG_func2(void);

#if defined(__BORLANDC__)
/* 0x39 */
void DNG_func3(void);
#endif

#if defined(__BORLANDC__)
/* 0x3e */
void DNG_func4(void);
#endif

#if defined(__BORLANDC__)
/* 0x43 */
void DNG_func6(void);
#endif

/* 0x48 */
void print_msg_with_first_hero(Bit8u*);

#if defined(__BORLANDC__)
/* 0x4d */
signed short DNG_step(void);
#endif

#if defined(__BORLANDC__)
/* 0x52 */
void DNG_func5(Bit8u*);
#endif

#if !defined(__BORLANDC__)
}
#endif
