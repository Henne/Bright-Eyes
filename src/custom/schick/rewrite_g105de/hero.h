#ifdef _MSC_VER
#pragma pack (1)
#endif

struct struct_attribs {
	signed char normal;
	signed char current;
	signed char mod;
}
#if __GNUC__
__attribute__ ((packed))
#endif
;

struct struct_hero {
	/* Offset 0x00 */
	unsigned char name[16];
	/* Offset 0x10 */
	unsigned char alias[16];
	/* Offset 0x20 */
	unsigned char items;
	unsigned char typus;
	unsigned char sex;
	unsigned char height;
	unsigned short weight;
	unsigned char god;
	unsigned char level;
	signed int ap;
	signed int money;
	/* Offset 0x30 */
	signed char rs1;
	signed char rs2;
	signed char unkn1;
	unsigned char bp_left;
	struct struct_attribs attribs[14];
	/* Offset 0x5e */
	signed short le;
	/* Offset 0x60 */
	signed short le_max;
	signed short ae1;
	signed short ae2;
	signed char mr;

	signed char atpa;
	signed char at[7];
	signed char pa[7];
	signed char w_at_mod;
	signed char w_pa_mod;
	signed char w_type;
	signed char atpa_mod;
	unsigned char le_malus;

	signed char unkn2[4];
	signed char hunger;
	/* Offset 0x80 */
	signed char thirst;
	signed char unkn3;
	signed char direction;
	signed char actions_left;
	signed char unkn4[3];
	signed char group;
	signed char unkn5[2];
	signed char position;
	/* Offset 0x8b */
	signed int unkn6;
	signed int unkn7;
	signed char unkn8[3];
	signed char rounds_blinded;
	signed char rounds_eclipt;
	signed char unkn9[8];
	/* Offset 0xa0 */
	signed char axxel;
	signed char drunk;
	signed char unkn10[8];
	signed char status;
	signed char status2;
	signed short unkn11;
	/* Not figured out yet, but reserve space */
	signed char sick[8][5];
	signed char poison[10][5];

	/* Offset 0x108 */
	signed char skills[52];
	signed char skill_incs;
	/* Offset 9x13d */
	signed char spells[86];
	signed char spell_incs;
	signed char school;
	signed char staff_level;

	/* Offset 0x196 */
	/* Not figured out yet, but reserve space */
	signed char inventory[23][14];
	signed short load;

	unsigned char pic[1024];
}
#if __GNUC__
__attribute__ ((packed))
#endif
;

#ifdef _MSC_VER
#pragma pack ()
#endif
