namespace M302de {
//0x20
//static
void draw_playmask();
//0x25
void draw_status_line();
//0x2a
void draw_icons();
//0x34
void clear_hero_icon(unsigned short);
//0x39
void clear_loc_line();
//0x3e
void select_hero_icon(unsigned short);
//0x43
void deselect_hero_icon(unsigned short);
//0x48
void copy_forename(Bit8u *, Bit8u *);
//0x4d
//static
void load_icon(Bit16u, Bit16s, Bit16s);

}
