#if !defined(__BORLANDC__)
namespace M302de {
#endif

void status_show(Bit16u);
void status_show_spell(Bit8u *, unsigned short, unsigned short, unsigned short,
				unsigned short, unsigned short);
void status_show_skill(Bit8u *, unsigned short, unsigned short, unsigned short,
				unsigned short, unsigned short);
void status_show_skills(Bit8u *);

#if !defined(__BORLANDC__)
}
#endif
