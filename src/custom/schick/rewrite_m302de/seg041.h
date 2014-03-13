#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x25
void FIG_output(Bit8u*);
//0x2a
void FIG_clear_msgs();
//0x2f
void FIG_add_msg(unsigned short, unsigned short);
//0x34
void FIG_damage_enemy(Bit8u*, Bit16s, signed short);
//0x3e
signed short weapon_check(Bit8u *);
//0x43
signed short FIG_get_enemy_attack_damage(Bit8u*, Bit8u*, signed short);
//0x48
void seg041_8c8();

#if !defined(__BORLANDC__)
}
#endif
