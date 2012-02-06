namespace M302de {

//0x20
void load_pp20(Bit16u);
/*0x25*/
void load_ani(signed short);
//0x2f
void init_common_buffers();
//0x34
void load_scenario(signed short);
//0x39
unsigned short count_fight_enemies(signed short);
//0x3e
void read_fight_lst(signed short);
//0x43
void write_fight_lst(void);
}
