#include "mem.h"

#include "../schick.h"

#include "seg007.h"

/*
	 16/140 Functions complete
*/

unsigned int get_readlength2(signed short index) {
	return index == -1 ? 0 : real_readd(datseg, 0xbce7);
}

void set_to_ff() {
	unsigned i;

	for (i = 0; i < 9; i++)
		real_writeb(datseg, 0xbd38+i, 0xff);
}

unsigned short mod_timer(short val) {
	if (real_readd(datseg, 0x2dbb) % val == 0)
		return 1;

	return 0;
}

short can_merge_group() {
	short retval = -1;
	char cur_heroes;
	short i;

	cur_heroes = real_readb(datseg, (short)real_readb(datseg, 0x2d35) + 0x2d36);
	if (cur_heroes == real_readb(datseg, 0x2d3c))
		return retval;

	for (i = 0; i < 6; i++) {
		if (i == (char)real_readb(datseg, 0x2d35))
			continue;
		if (0 == real_readb(datseg, i + 0x2d36))
			continue;
		/* check XTarget */
		if (real_readw(datseg, i * 2 + 0x2d48) != real_readw(datseg, 0x2d44))
			continue;
		/* check YTarget */
		if (real_readw(datseg, i * 2 + 0x2d54) != real_readw(datseg, 0x2d46))
			continue;
		/* check Location */
		if (real_readb(datseg, 0x2d61) != real_readb(datseg, 0x2d60))
			continue;
		/* check currentTown */
		if (real_readb(datseg, 0x2d68) != real_readb(datseg, 0x2d67))
			continue;
		/* check DungeonIndex */
		if (real_readb(datseg, 0x2d6f) != real_readb(datseg, 0x2d6e))
			continue;
		/* check DungeonLevel */
		if (real_readb(datseg, 0x2d76) != real_readb(datseg, 0x2d75))
			continue;

		retval = i;
	}

	return retval;
}

unsigned short div16(unsigned char val) {
	return val >> 4;
}

/**
	check_hero - returns true if heros not dead, stoned or unconscious
	should be static
*/
unsigned short check_hero(Bit8u *hero) {

	/* Check if hero has a class */
	if (host_readb(hero+0x21) == 0)
		return 0;
	/* Check if sleeping */
	if ((host_readb(hero+0xaa) >> 1) & 1)
		return 0;
	/* Check if dead */
	if (host_readb(hero+0xaa) & 1 )
		return 0;
	/* Check if stoned */
	if ((host_readb(hero+0xaa) >> 2) & 1)
		return 0;
	/* Check if unconscious */
	if ((host_readb(hero+0xaa) >> 6) & 1)
		return 0;
	/* Check if ??? */
	if ((host_readb(hero+0xaa) >> 5) & 1)
		return 0;
	/* Check if ??? */
	if (host_readb(hero+0x84) == 0x10)
		return 0;

	return 1;
}

/**
	check_hero_no2 - returns true if heros not dead, stoned or unconscious
	should be static
*/
unsigned short check_hero_no2(Bit8u *hero) {

	/* Check if hero has a class */
	if (host_readb(hero+0x21) == 0)
		return 0;
	/* Check if dead */
	if (host_readb(hero+0xaa) & 1 )
		return 0;
	/* Check if stoned */
	if ((host_readb(hero+0xaa) >> 2) & 1)
		return 0;
	/* Check if unconscious */
	if ((host_readb(hero+0xaa) >> 6) & 1)
		return 0;
	/* Check if ??? */
	if ((host_readb(hero+0xaa) >> 5) & 1)
		return 0;

	return 1;
}

/**
	check_hero_no3 - returns true if heros not dead, stoned or unconscious
	should be static
*/
unsigned short check_hero_no3(Bit8u *hero) {

	/* Check if hero has a class */
	if (host_readb(hero+0x21) == 0)
		return 0;
	/* Check if dead */
	if (host_readb(hero+0xaa) & 1 )
		return 0;
	/* Check if stoned */
	if ((host_readb(hero+0xaa) >> 2) & 1)
		return 0;
	/* Check if unconscious */
	if ((host_readb(hero+0xaa) >> 6) & 1)
		return 0;

	return 1;
}

/**
	add_hero_ae - add AE points to heros current AE
*/
void add_hero_ae(Bit8u* hero, short ae) {

	short tmp;

	/* dont add AE if hero is dead or ae = 0 */
	if ( (*(hero+0xaa) & 1) || ae == 0)
		return;

	tmp = real_readw(datseg, 0xc3cb);
	real_writew(datseg, 0xc3cb, 0);

	/* add AE to heros current AE */
	host_writew(hero+0x64, host_readw(hero+0x64) + ae);

	/* if current AE is greater than AE maximum
		set current AE to AE maximum */
	if (host_readw(hero+0x64) > host_readw(hero+0x62))
		host_writew(hero+0x64, host_readw(hero+0x62));

	real_writew(datseg, 0xc3cb, tmp);
}

/**
	test_attrib - make an attribute test

	A test is positive if the return value is greater than zero.
*/
short test_attrib(Bit8u* hero, unsigned short attrib, short bonus) {

	short si = random_schick(20);
	short tmp;

	D1_INFO("Eigenschaftsprobe %s auf %s %+d: W20 = %d",
		(char*)(hero+0x10), names_attrib[attrib], bonus, si);

	if (si == 20)
		return -99;

	si += bonus;

	tmp = host_readb(hero + attrib*3 + 0x35);
	tmp += host_readb(hero + attrib*3 + 0x36);
	tmp -= si + 1;

	D1_INFO(" -> %s mit %d\n",
		tmp > 0 ? "bestanden" : "nicht bestanden", tmp);

	return tmp;
}

/**
	test_attrib3 - make three attribute tests

	A test is positive if the return value is greater than zero.
*/
short test_attrib3(Bit8u* hero, unsigned short attrib1, unsigned short attrib2, unsigned short attrib3, short bonus) {

	short si = 0;
	unsigned short zw = 0;
	unsigned short i;
	short tmp;

	D1_INFO("%s -> (%s/%s/%s) %+d: ",
		(char*)(hero+0x10), names_attrib[attrib1],
		names_attrib[attrib2], names_attrib[attrib3], bonus);

	for (i = 0; i < 3; i++) {
		tmp = random_schick(20);

		D1_INFO("%d ", tmp);

		if (tmp == 20) {
			zw++;
			if (zw == 2) {
				D1_INFO(" -> UNGLÜCKLICH! nicht bestanden\n");
				return -99;
			}
		}
		si += tmp;
	}

	si += bonus;

	tmp = (char)host_readb(hero + attrib1*3 + 0x35);
	tmp += (char)host_readb(hero + attrib1*3 + 0x36);
	tmp += (char)host_readb(hero + attrib2*3 + 0x35);
	tmp += (char)host_readb(hero + attrib2*3 + 0x36);
	tmp += (char)host_readb(hero + attrib3*3 + 0x35);
	tmp += (char)host_readb(hero + attrib3*3 + 0x36);

	tmp -= si + 1;

	D1_INFO(" -> %s mit %d\n",
		tmp > 0 ? "bestanden" : "nicht bestanden", tmp);

	return tmp;
}

/**
	get_party_money - summs up the money of the current party
*/
unsigned int get_party_money() {
	Bit8u* hero;
	unsigned int sum, i;

	sum = 0;
	hero = MemBase + Real2Phys(real_readd(datseg, 0xbd34));

	for (i=0; i < 6; i++, hero+=0x6da) {
		/* Check if hero has a class */
		if (host_readb(hero+0x21) == 0)
						continue;
		/* Check if hero is in current party */
		if (host_readb(hero+0x87) != real_readb(datseg, 0x2d35))
								continue;
		sum += host_readd(hero+0x2c);
	}

	return sum;
}

/**
	add_hero_ap - add AP
*/
void add_hero_ap(Bit8u *hero, int ap) {
	host_writed(hero+0x28, host_readd(hero+0x28) + ap);
}

/**
	get_hero_index - gets hero index
*/

unsigned short get_hero_index(Bit8u *hero) {
	Bit8u *first_hero = MemBase + Real2Phys(real_readd(datseg, 0xbd34));
	int i = 0;

	while (hero != first_hero + i*0x6da)
		i++;
	return i;

}

/**
	get_item_pos - gets item position
*/
int get_item_pos(Bit8u *hero, unsigned short item) {
	int i;

	for (i = 0; i < 0x17; i++) {
		if (item == host_readw(hero + i*14 + 0x196))
			return i;
	}
	return -1;
}
/**
	count_heroes_available_in_group
*/

unsigned short count_heroes_available_in_group() {
	Bit8u *hero_i = MemBase + Real2Phys(real_readd(datseg, 0xbd34));
	char i, heroes = 0;

	for (i=0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if (host_readb(hero_i+0x21) == 0)
			continue;
		/* Check if in current group */
		if (host_readb(hero_i+0x87) != real_readb(datseg, 0x2d35))
			continue;
		/* Check if hero is available */
		if (check_hero_no2(hero_i) == 0)
			continue;

		heroes++;
	}

	return heroes;
}
