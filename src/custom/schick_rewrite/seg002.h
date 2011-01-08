unsigned int get_readlength2(signed short index);
signed int process_nvf(Bit8u *p);
//static
void mouse_action(Bit8u *, Bit8u *, Bit8u *, Bit8u *, Bit8u *);
short is_mouse_in_rect(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void mouse_move_cursor(unsigned short x, unsigned short y);
void make_ggst_cursor(Bit8u *icon);
void update_mouse_cursor();
void refresh_screen_size();
void update_mouse_cursor1();
void refresh_screen_size1();
//static
void mouse_19dc();
unsigned short get_mouse_action(unsigned short x, unsigned short y, Bit8u *p);
void wait_for_keyboard1();
//static
void timers_daily();
//static
void seg002_2177();
void pal_fade(PhysPt dst, PhysPt p2);
void pal_fade_in(PhysPt dst, PhysPt p2, unsigned short v3, unsigned short colors);
unsigned short get_current_season();
void sub_ingame_timers(unsigned int);
void sub_mod_timers(unsigned int);
void seg002_2f7a(unsigned int fmin);
void sub_light_timers(unsigned short quarter, signed short v2);
void set_and_spin_lock();
//static
void seg002_3b63();
//static
void seg002_3c63();
void dec_splash();
//static
void draw_splash(unsigned short index, unsigned short type);
void wait_for_keyboard2();
void wait_for_keypress();
unsigned int swap_u32(unsigned int v);
unsigned short alloc_EMS(unsigned int bytes);
void set_to_ff();
unsigned short mod_timer(short val);
void draw_compass();
short can_merge_group();
unsigned short div16(unsigned char val);
void select_with_mouse(Bit8u *p1, Bit8u *p2);
void select_with_keyboard(Bit8u *p1, Bit8u *p2);
void set_automap_tile(unsigned short x, unsigned short y);
void set_automap_tiles(unsigned short x, unsigned short y);
void seg002_47e2();
void seg002_484f();
unsigned short check_hero(Bit8u* hero);
unsigned short is_hero_available_in_group(Bit8u* hero);
void sub_ae_splash(Bit8u *hero, signed short type);
void add_hero_ae(Bit8u* hero, short ae);
void add_hero_le(Bit8u *hero, signed short le);
void add_group_le(signed short le);
short test_attrib(Bit8u* hero, unsigned short attrib, short bonus);
short test_attrib3(Bit8u* hero, unsigned short attrib1, unsigned short attrib2, unsigned short attrib3, signed char bonus);
unsigned short get_random_hero();
unsigned int get_party_money();
void set_party_money(signed int money);
void add_party_money(signed int money);
void add_hero_ap(Bit8u* hero, int ap);
void add_group_ap(signed int ap);
void add_hero_ap_all(short ap);
void sub_hero_ap_all(short ap);
unsigned short get_hero_index(Bit8u* hero);
int get_item_pos(Bit8u* hero, unsigned short item);
short get_first_hero_with_item(unsigned short item);
signed short get_first_hero_with_item_in_group(unsigned short item, signed char group);
RealPt get_first_hero_available_in_group();
RealPt get_second_hero_available_in_group();
unsigned short count_heros_available();
unsigned short count_heroes_available_in_group();
