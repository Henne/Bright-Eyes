#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short update_direction(unsigned char mod);

void move(void);

void door_frame(signed short, signed short, signed short, signed short);

void loot_corpse(RealPt, Bit8u*, Bit8u*);

void no_way();

#if !defined(__BORLANDC__)
}
#endif
