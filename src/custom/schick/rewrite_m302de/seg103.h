#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
signed short test_skill(Bit8u *hero, unsigned short skill, signed char bonus);
//25
signed short bargain(Bit8u *hero, unsigned short nice, signed int price,
			signed short percent, signed char mod_init);
//39
//static
RealPt get_proper_hero(unsigned short skill);

#if !defined(__BORLANDC__)
}
#endif
