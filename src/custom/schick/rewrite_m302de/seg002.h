#if !defined(__BORLANDC__)
namespace M302de {
#endif

void play_music_file(signed short);
void set_audio_track(Bit16u);
void sound_menu(void);
/* static */
void read_sound_cfg(void);
void init_AIL(Bit32u);
void exit_AIL(void);
/* static */
RealPt read_music_driver(RealPt);
/* static */
signed short prepare_midi_playback(signed short);
/* static */
signed short start_midi_playback(signed short);
/* static */
RealPt prepare_timbre(signed short, signed short);
/* static */
signed short load_midi_file(signed short);
/* static */
signed short do_load_midi_file(signed short);
/* static */
signed short load_music_driver(RealPt, signed short, signed short);
/* static */
void do_play_music_file(signed short);
/* static */
void stop_midi_playback(void);
void start_midi_playback_IRQ(void);
signed short have_mem_for_sound(void);
void play_voc(signed short);
void play_voc_delay(signed short);
void alloc_voc_buffer(Bit32u);
/* static */
void free_voc_buffer(void);
/* static */
signed short read_new_voc_file(signed short);
/* static */
signed short read_voc_file(signed short);

/* static */
void SND_play_voc(signed short);
/* static */
void SND_stop_digi(void);
/* static */
void SND_set_volume(unsigned short);
/* static */
signed short load_digi_driver(RealPt, signed short, signed short, signed short);
/* static */
RealPt read_digi_driver(RealPt);
//static
signed short open_and_seek_dat(unsigned short);
Bit32u get_readlength2(signed short);
unsigned short read_archive_file(Bit16u, Bit8u *, Bit16u);
void seg002_0c72(Bit16u, Bit32u, Bit16u);
signed short load_regular_file(Bit16u);
signed short load_archive_file(Bit16u);
signed short open_temp_file(unsigned short);
void copy_from_archive_to_temp(unsigned short, RealPt);
void copy_file_to_temp(RealPt, RealPt);
signed int process_nvf(struct nvf_desc *);
//static
void mouse_action(Bit8u *, Bit8u *, Bit8u *, Bit8u *, Bit8u *);
short is_mouse_in_rect(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void disable_mouse(void);
void mouse_reset_ehandler(void);
void mouse_move_cursor(unsigned short x, unsigned short y);
void make_ggst_cursor(Bit8u *icon);
void update_mouse_cursor();
void refresh_screen_size();
void handle_gui_input(void);
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
void set_mod_slot(unsigned short, Bit32u, Bit8u *, signed char, signed char);
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
void timewarp(Bit32s);
void timewarp_until(Bit32s);
void dec_splash();
//static
void draw_splash(unsigned short index, unsigned short type);
void wait_for_keyboard2();
void wait_for_keypress();
void delay_or_keypress(Bit16u);
unsigned int swap_u32(unsigned int v);
unsigned short alloc_EMS(Bit32s);
void from_EMS(RealPt, unsigned short, Bit32s);
void to_EMS(unsigned short, RealPt, Bit32s);
void set_to_ff();
void draw_loc_icons(signed short, ...);
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
Bit32s get_party_money();
void set_party_money(Bit32s);
void add_party_money(Bit32s);
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

#if !defined(__BORLANDC__)
}
#endif
