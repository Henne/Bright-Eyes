#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void move_item(signed short, signed short, Bit8u*);
/* 0x25 */
void print_item_description(Bit8u*, signed short);
/* 0x2a */
void pass_item(Bit8u*, signed short, Bit8u*, signed short);
/* 0x2f */
void startup_equipment(Bit8u*);
/* 0x34 */
signed short two_hand_collision(Bit8u*, signed short, signed short);
/* 0x39 */
signed short get_max_light_time(void);
/* 0x3e */
void equip_belt_ani(void);
/* 0x43 */
signed short get_full_waterskin_pos(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif
