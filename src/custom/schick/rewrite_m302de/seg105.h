namespace M302de {

//20
void unequip(Bit8u *hero, unsigned short item, unsigned short pos);
//34
unsigned short drop_item(Bit8u *, unsigned short, signed short);
//3e
unsigned short can_hero_use_item(Bit8u *, unsigned short);
//43
unsigned short can_item_at_pos(unsigned short item, unsigned short pos);
//48
signed short has_hero_stacked(Bit8u *, unsigned short);
//4d
//static
unsigned short item_pleasing_ingerimm(unsigned short);
//57
unsigned short hero_count_item(Bit8u *, unsigned short);
//5c
unsigned short group_count_item(unsigned short);

}
