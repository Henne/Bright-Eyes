#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short test_skill(Bit8u *hero, signed short, signed char);
//25
signed short bargain(Bit8u *hero, unsigned short nice, signed int price,
			signed short percent, signed char mod_init);

/* 0x2f */
signed short select_talent(void);

/* 0x34 */
signed short LVL_select_talent(Bit8u *, signed short);

/* 0x39 */
/* can be static */
RealPt get_proper_hero(signed short);

#if !defined(__BORLANDC__)
}
#endif
