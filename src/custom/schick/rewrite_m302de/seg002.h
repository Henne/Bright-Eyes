namespace M302de {

void play_voc(Bit16u);
void play_voc_delay(Bit16u);
//static
Bit16u open_and_seek_dat(unsigned short);
unsigned int get_readlength2(signed short index);
unsigned short read_archive_file(Bit16u, Bit8u *, Bit16u);
void seg002_0c72(Bit16u, Bit32u);
Bit16u load_regular_file(Bit16u);
Bit16u load_archive_file(Bit16u);
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
void handle_input();
void wait_for_keyboard1();
//static
void timers_daily();
//static
void seg002_2177();
void pal_fade(Bit8u*, Bit8u*);
void pal_fade_in(Bit8u*, Bit8u*, unsigned short, unsigned short);
void dawning(void);
void nightfall(void);
unsigned short get_current_season();
void do_timers(void);
//static
void do_census();
void sub_ingame_timers(unsigned int);
void sub_mod_timers(unsigned int);
unsigned short get_free_mod_slot();
void set_mod_slot(unsigned short, unsigned int, Bit8u *, signed char, signed char);
void seg002_2f7a(unsigned int fmin);
void sub_light_timers(signed int);
//static
void magical_chainmail_damage(void);
void herokeeping(void);
void set_and_spin_lock();
//static
void passages_recalc();
//static
void passages_reset();
void timewarp(unsigned int);
void timewarp_until(unsigned int);
void dec_splash();
//static
void draw_splash(unsigned short index, unsigned short type);
void wait_for_keyboard2();
void wait_for_keypress();
void delay_or_keypress(Bit16u);
unsigned int swap_u32(unsigned int v);
unsigned short alloc_EMS(unsigned int bytes);
void from_EMS(RealPt, unsigned short, unsigned int);
void to_EMS(unsigned short, RealPt, unsigned int bytes);
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
unsigned short check_hero_no2(Bit8u* hero);
unsigned short check_hero_no3(Bit8u* hero);
unsigned short is_hero_available_in_group(Bit8u* hero);
/* static */
void do_starve_damage(Bit8u*, Bit16u, Bit16u);
void sub_ae_splash(Bit8u *hero, signed short type);
void add_hero_ae(Bit8u* hero, short ae);
void sub_hero_le(Bit8u*, short);
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
void sub_group_le(signed short);
RealPt get_first_hero_available_in_group();
RealPt get_second_hero_available_in_group();
unsigned short count_heros_available();
unsigned short count_heroes_available_in_group();
RealPt schick_alloc_emu(Bit32u);

}
