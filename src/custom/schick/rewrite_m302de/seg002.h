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
void seg002_0c72(Bit16u, Bit32s, Bit16u);
signed short load_regular_file(Bit16u);
signed short load_archive_file(Bit16u);
signed short open_temp_file(unsigned short);
void copy_from_archive_to_temp(unsigned short, RealPt);
void copy_file_to_temp(RealPt, RealPt);
Bit32s process_nvf(struct nvf_desc *);
#if defined(__BORLANDC__)
void interrupt mouse_isr(void);
#endif
//static
void mouse_action(Bit8u *, Bit8u *, Bit8u *, Bit8u *, Bit8u *);
signed short is_mouse_in_rect(signed short, signed short, signed short, signed short);
#if defined(__BORLANDC__)
void mouse_init(void);
#endif
void disable_mouse(void);
void seg002_170e(void);
#if defined(__BORLANDC__)
void call_mouse_isr(void);
void mouse_irq_init(signed short, void interrupt *);
#endif
void mouse_reset_ehandler(void);
void mouse_move_cursor(signed short, signed short);
void seg002_1838(signed short, signed short, signed short, signed short);
void seg002_1880(signed short);
void make_ggst_cursor(Bit8u *icon);
void update_mouse_cursor(void);
void refresh_screen_size(void);
void update_mouse_cursor1(void);
void refresh_screen_size1(void);
//static
void mouse_19dc(void);
void handle_gui_input(void);
signed short get_mouse_action(signed short, signed short, Bit8u*);
void handle_input(void);
void wait_for_keyboard1(void);
/* static */
void game_loop(void);
//static
void timers_daily(void);
/* static */
void seg002_2177(void);
void pal_fade(Bit8u*, Bit8u*);
void pal_fade_in(Bit8u*, Bit8u*, signed short, signed short);
void dawning(void);
void nightfall(void);
signed short get_current_season(void);
/* static */
void do_census(void);
void do_timers(void);
void sub_ingame_timers(Bit32s);
void sub_mod_timers(Bit32s);
signed short get_free_mod_slot(void);
void set_mod_slot(signed short, Bit32s, Bit8u*, signed char, signed char);
void seg002_2f7a(Bit32s);
void sub_light_timers(Bit32s);
//static
void magical_chainmail_damage(void);
void herokeeping(void);
#if defined(__BORLANDC__)
void check_level_up(void);
#endif
void seg002_37c4(void);
void set_and_spin_lock(void);
/* static */
void passages_recalc(void);
//static
void passages_reset();
void timewarp(Bit32s);
void timewarp_until(Bit32s);
void dec_splash(void);
//static
void draw_splash(signed short, signed short);
void timewarp_until_midnight(void);
void wait_for_keyboard2(void);
/* static */
void seg002_4031(void);
void wait_for_keypress(void);
void delay_or_keypress(signed short);
void unused_delay(signed short);
void unused_spinlock(void);
Bit32u swap_u32(Bit32u);
Bit32u swap_u32_unused(Bit32u);
signed short alloc_EMS(Bit32s);
void from_EMS(RealPt, signed short, Bit32s);
void to_EMS(signed short, RealPt, Bit32s);
void set_to_ff(void);
void draw_loc_icons(signed short, ...);
signed short mod_day_timer(signed short);
void draw_compass(void);
signed short can_merge_group(void);
signed short div16(signed short);
void select_with_mouse(Bit8u*, Bit8u*);
void select_with_keyboard(Bit8u*, Bit8u*);
void set_automap_tile(signed short, signed short);
void set_automap_tiles(signed short, signed short);
void seg002_47e2(void);
void seg002_484f(void);
signed short check_hero(Bit8u*);
signed short check_hero_no2(Bit8u*);
signed short check_hero_no3(Bit8u*);
signed short is_hero_available_in_group(Bit8u*);
void sub_ae_splash(Bit8u*, signed short);
void add_hero_ae(Bit8u*, signed short);
void sub_hero_le(Bit8u*, signed short);
void add_hero_le(Bit8u*, signed short);
void add_group_le(signed short);
/* static */
void do_starve_damage(Bit8u*, signed short, signed short);
signed short compare_name(Bit8u*);
signed short test_attrib(Bit8u*, signed short, signed short);
signed short test_attrib3(Bit8u*, signed short, signed short, signed short, signed char);
signed short unused_cruft(void);
signed short get_random_hero(void);
Bit32s get_party_money(void);
void set_party_money(Bit32s);
void add_party_money(Bit32s);
void add_hero_ap(Bit8u*, Bit32s);
void add_group_ap(Bit32s);
void add_hero_ap_all(signed short);
void sub_hero_ap_all(signed short);
signed short get_hero_index(Bit8u*);
signed short get_item_pos(Bit8u*, signed short);
signed short get_first_hero_with_item(signed short);
signed short get_first_hero_with_item_in_group(signed short, signed short);
void sub_group_le(signed short);
RealPt get_first_hero_available_in_group(void);
RealPt get_second_hero_available_in_group();
signed short count_heros_available(void);
signed short count_heroes_available_in_group(void);
RealPt schick_alloc_emu(Bit32u);

#if !defined(__BORLANDC__)
}
#endif
