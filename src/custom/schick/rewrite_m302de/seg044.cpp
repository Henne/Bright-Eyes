/*
 *	Rewrite of DSA1 v3.02_de functions of seg044 (Fightsystem)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg044.cpp
 */

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg036.h"
#include "seg038.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   copies an animation sequence
 *
 * \param   dst         where to copy the sequence
 * \param   ani         the number of the animation sequence
 * \param   type        type of animation (3 = WEAPANI.DAT else ANI.DAT)
 * \return              the number of copied bytes
 */
/* Borlandified and identical */
Bit16s copy_ani_seq(Bit8u *dst, Bit16s ani, Bit16u type)
{
	Bit8u *p_start, *p_seq;
	signed short nr_anis;
	signed short i;
	signed char length;

	/* get pointer from ANI.DAT */
	p_start = Real2Host(ds_readd(BUFFER_ANIDAT));

	/* check if we must use WEAPANI.DAT */
	if (type == 3)
		p_start = Real2Host(ds_readd(BUFFER_WEAPANIDAT));

	/* get number of animation sequences */
	nr_anis = host_readws(p_start);

	/* sanity check */
	if (ani < 0)
		return 0;

	if (ani > nr_anis)
		return 0;

	/* set p_seq to the first sequence */
	p_seq = p_start;
	p_seq += nr_anis + 2;

	/* set length to the length of the first sequence */
	length = host_readbs(p_start + 2);

	/* fast forward to the requestet sequence */
	for (i = 1; i <= ani; i++) {
		p_seq += length;
		length = host_readbs(p_start + i + 2);
	}

	/* skip the first byte */
	p_seq++;
	/* calc the length of the sequence */
	length -= 2;
	/* REMARK: the first an the last byte of the sequence are skipped */

	/* copy them */
	for (i = 0; i < length; p_seq++, dst++, i++)
		host_writeb(dst, host_readbs(p_seq));

	return length;
}

/**
 * \brief   TODO
 *
 * \param   ani         the number of the animation sequence
 * \return              the first byte of the sequence from ANI.DAT {0,1,2,3,4}
 */
/* Borlandified and identical */
Bit8s get_seq_header(Bit16s ani)
{
	Bit8u *p_start, *p_seq;
	Bit8s length;
	Bit16s nr_anis;
	Bit16s i;

	/* get pointer from ANI.DAT */
	p_start = Real2Host(ds_readd(BUFFER_ANIDAT));

	/* get number of ani seqences in ANI.DAT */
	nr_anis = host_readws(p_start);

	if (ani < 0) {
		return 0;
	}

	if (ani > nr_anis) {
		return 0;
	}

	p_seq = p_start;
	p_seq += nr_anis + 2;

	length = host_readbs(p_start + 2);

	for (i = 1; i <= ani; i++) {
		/* set pointer to the start of the next sequence */
		p_seq += length;
		/* set length to the length of the next sequence */
		length = host_readbs(p_start + i + 2);
	}

	return host_readbs(p_seq);
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 *
 * \param   a1          [0, 1]
 * \param   hero        pointer to hero
 * \param   weapon_type the type of weapon for the animation [-1, 5], 3,4,5 are range weapons
 * \param   action_type {2, 15, 100, 102, 103}
 */
/* Borlandified and identical */
void FIG_prepare_hero_fight_ani(signed short a1, Bit8u *hero, signed short weapon_type, signed short f_action, signed short fid_attacker, signed short fid_target, signed short a7)
{
	signed short l1;
	signed short attacker_x;
	signed short attacker_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	signed short l7;
	signed short l8;
	signed short l9;
	signed short l10;
	Bit8u *p1;
	Bit8u *p2;
	signed short weapon;
	Bit8u *p3;

	p3 = Real2Host(ds_readd(GFX_ANI_INDEX + host_readbs(hero + HERO_SPRITE_NO) * 4));
	weapon = host_readws(hero + HERO_ITEM_RIGHT);

	if ((signed char)fid_target != 0) {
		FIG_search_obj_on_cb((signed char)fid_target, &target_x, &target_y);
		FIG_search_obj_on_cb((signed char)fid_attacker, &attacker_x, &attacker_y);

#if !defined(__BORLANDC__)
		/* BE-fix */
		target_x = host_readws((Bit8u*)&target_x);
		target_y = host_readws((Bit8u*)&target_y);
		attacker_x = host_readws((Bit8u*)&attacker_x);
		attacker_y = host_readws((Bit8u*)&attacker_y);
#endif
		if (attacker_x == target_x) {
			if (target_y < attacker_y) {
				dir = 1;
			} else {
				dir = 3;
			}
		} else {
			if (target_x < attacker_x) {
				dir = 2;
			} else {
				dir = 0;
			}
		}
	} else {
		dir = host_readbs(hero + HERO_VIEWDIR);
	}

	if ((weapon_type == -1) || ((host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE) && (weapon == 0x85))) {

		l1 = (f_action == 2) ? 45 :			/* melee attack */
			(f_action == 102) ? 41 :		/* drink potion */
			(f_action == 103) ? 53 :		/* cast spell */
			49;

	} else {
		l1 = (f_action == 2) ?  21:			/* melee attack */
			(f_action == 102) ? 41 :		/* drink potion */
			(f_action == 103) ? 53 :		/* cast spell */
			(f_action != 15) ? 25 :
			(weapon_type == 3) ? 33 :
			(weapon_type == 4) ? 57 :
			61;
	}

	l1 += dir;
	p1 = p_datseg + (0xd8ce + 1) + 0xf3 * a1;
	p2 = p_datseg + (0xd8ce + 1 + 4*0xf3) + 0xf3 * a1;

	ds_writeb(0xd8ce + 0xf3 * a1, get_seq_header(host_readws(p3 + l1 * 2)));
	ds_writeb((0xd8ce + 242) + 0xf3 * a1, host_readbs(hero + HERO_SPRITE_NO));

	if (check_hero(hero) && (host_readbs(hero + HERO_VIEWDIR) != dir) &&

		((f_action == 2) || (f_action == 15) || (f_action == 103) ||
			((f_action == 100) && !ds_readbs((HERO_IS_TARGET-1) + (signed char)fid_attacker)) ||
			((ds_readws(ATTACKER_ATTACKS_AGAIN) != 0) && (a7 == 0)) ||
			((ds_readws(DEFENDER_ATTACKS) != 0) && (a7 == 1))))
	{

			ds_writeb(0xd8ce + a1 * 0xf3, 0);
			l8 = l7 = -1;
			l9 = host_readbs(hero + HERO_VIEWDIR);
			l8 = l9;
			l9++;

			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != dir) {
				l7 = l9;
				l9++;
				if (l9 == 4) {
					l9 = 0;
				}

				if (l9 != dir) {
					l8 = host_readbs(hero + HERO_VIEWDIR) + 4;
					l7 = -1;
				}
			}

			host_writeb(hero + HERO_VIEWDIR, (signed char)dir);

			if (l7 == -1) {
				for (l10 = 0; l10 < 2; l10++) {
					host_writeb(p1++, 0xfb);
					host_writeb(p1++, 0);
					host_writeb(p1++, 0);
				}
			}

			p1 += copy_ani_seq(p1, host_readws(p3 + l8 * 2), 2);

			if (l7 != -1) {
				p1 += copy_ani_seq(p1, host_readws(p3 + l7 * 2), 2);
			}

			host_writeb(p1++, 0xfc);
			host_writeb(p1++, get_seq_header(host_readws(p3 + l1 * 2)));
			host_writeb(p1++, 0);
	} else {
		for (l10 = 0; l10 < 5; l10++) {
			host_writeb(p1++, 0xfb);
			host_writeb(p1++, 0);
			host_writeb(p1++, 0);
		}
	}

	if ((check_hero(hero) && (f_action == 2)) ||
		((f_action == 15) || (f_action == 102) || (f_action == 103) ||
			((f_action == 100) && !ds_readbs((HERO_IS_TARGET-1) + (signed char)fid_attacker))))
	{
		p1 += copy_ani_seq(p1, host_readws(p3 + l1 *2), 2);

		if ((weapon_type != -1) && (weapon_type < 3) &&
			(host_readb(hero + HERO_TYPE) != HERO_TYPE_MAGE) &&
			(host_readb(hero + HERO_TYPE) != HERO_TYPE_DRUID))
		{
			for (l10 = 0; l10 < 5; l10++) {
				host_writeb(p2++, 0xfb);
				host_writeb(p2++, 0);
				host_writeb(p2++, 0);
			}

			p2 += copy_ani_seq(p2,
				ds_readw(WEAPONANI_TABLE +
				((ds_readbs(WEAPONANI_TYPES + host_readbs(hero + HERO_SPRITE_NO)) * 48 + weapon_type * 16) +
				((f_action == 2) ? 0 : 1) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2)), 3);
		}
	}

	if ((check_hero(hero) && ds_readw(ATTACKER_ATTACKS_AGAIN) != 0 && a7 == 0) ||
		((ds_readw(DEFENDER_ATTACKS) != 0) && (a7 == 1))) {

			p1 += copy_ani_seq(p1, host_readws(p3 + l1 * 2), 2);

			if ((weapon_type != -1) && (weapon_type < 3) &&
				(host_readb(hero + HERO_TYPE) != HERO_TYPE_MAGE) &&
				(host_readb(hero + HERO_TYPE) != HERO_TYPE_DRUID))
			{
				p2 += copy_ani_seq(p2,
					ds_readw(WEAPONANI_TABLE +
					((ds_readbs(WEAPONANI_TYPES + host_readbs(hero + HERO_SPRITE_NO)) * 48 + weapon_type * 16) +
					((f_action == 2) ? 0 : 1) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2)), 3);
			}
	}

	if ( ((ds_readw(ATTACKER_DEAD) != 0) && (a7 == 0)) ||
		((ds_readw(DEFENDER_DEAD) != 0) && (a7 == 1)))
	{
		host_writeb(p1++, 0xfc);
		host_writeb(p1++, get_seq_header(host_readws(p3 + 0x28)));
		host_writeb(p1++, 0);

		p1 += copy_ani_seq(p1, host_readws(p3 + 0x28), 2);
	}

	if (check_hero(hero) ||
		((ds_readw(ATTACKER_DEAD) != 0) && (a7 == 0)) ||
		((ds_readw(DEFENDER_DEAD) != 0) && (a7 == 1)))
	{
		FIG_set_0e(host_readb(hero + HERO_FIGHTER_ID), (signed char)a1);
		host_writebs(p1, -1);

		if ( (weapon_type != -1) && (weapon_type < 3) &&
			(host_readb(hero + HERO_TYPE) != HERO_TYPE_MAGE) &&
			(host_readb(hero + HERO_TYPE) != HERO_TYPE_DRUID))
		{
			FIG_set_0f(host_readb(hero + HERO_FIGHTER_ID), a1 + 4);
			host_writeb(p2, 0xff);
		}
	}

	host_writeb(p1, 0xff);
	if (f_action == 100) {
		ds_writeb((HERO_IS_TARGET-1) + (signed char)fid_attacker, 1);
	}
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 */
/* Borlandified and identical */
void FIG_prepare_enemy_fight_ani(signed short a1, Bit8u *enemy, signed short f_action, signed short fid_attacker, signed short fid_target, signed short a7)
{
	signed short l1;
	signed short attacker_x;
	signed short attacker_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	signed short l7;
	signed short l8;
	signed short l9;
	signed short i;
	Bit8u *p1;
	Bit8u *p2;
	Bit8u *p3;			/* only user for two sprited figures */
	Bit8u *p4;			/* read only */
	signed short weapon_type;

	/* initialize with bare hands */
	weapon_type = -1;

	/* every enemy with the gfx_id < 22 has a sword type weapon */
	if (host_readbs(enemy + ENEMY_SHEET_GFX_ID) < 22) {
		weapon_type = 2;
	}

	/* This byte is unknown atm */
	if (host_readbs(enemy + ENEMY_SHEET_BROKEN) != 0) {
		weapon_type = -1;
	}

	/* read a pointer from the datseg with the gfx_id as offset, read only */
	p4 = Real2Host(ds_readd(GFX_ANI_INDEX + host_readbs(enemy + ENEMY_SHEET_GFX_ID) * 4));

	/* find both actors on the chessboard */
	FIG_search_obj_on_cb((signed char)fid_target, &target_x, &target_y);
	FIG_search_obj_on_cb((signed char)fid_attacker, &attacker_x, &attacker_y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	target_x = host_readws((Bit8u*)&target_x);
	target_y = host_readws((Bit8u*)&target_y);
	attacker_x = host_readws((Bit8u*)&attacker_x);
	attacker_y = host_readws((Bit8u*)&attacker_y);
#endif
	/* find out which direction the action will have */
	if (attacker_x == target_x) {
		if (target_y < attacker_y) {
			dir = 1;
		} else {
			dir = 3;
		}
	} else {
		if (target_x < attacker_x) {
			dir = 2;
		} else {
			dir = 0;
		}
	}



	l1 = (f_action == 2) ? 21 :			/* melee attack */
		25;

	if ((host_readbs(enemy + ENEMY_SHEET_GFX_ID) == 8) ||
		(host_readbs(enemy + ENEMY_SHEET_GFX_ID) == 9) ||
		(host_readbs(enemy + ENEMY_SHEET_GFX_ID) == 19) ||
		(host_readbs(enemy + ENEMY_SHEET_GFX_ID) == 20))
	{
		weapon_type = -1;
		l1 = (f_action == 2) ? 45 : 49;
	}

	if (f_action == 15) {
		l1 = 33;
		weapon_type = -1;
	}

	l1 += dir;

	p1 = p_datseg + (0xd8ce + 1) + a1 * 0xf3;
	p2 = p_datseg + (0xd8ce + 1 + 4*0xf3) + a1 * 0xf3;


	ds_writeb(0xd8ce + 0xf3 * a1, get_seq_header(host_readws(p4 + l1 * 2)));
	ds_writeb((0xd8ce + 242) + 0xf3 * a1, host_readbs(enemy + ENEMY_SHEET_GFX_ID));

	/* first the enemy may turn */
	if ((host_readbs(enemy + ENEMY_SHEET_VIEWDIR) != dir) &&
		(	((f_action == 2) || (f_action == 15) ||
			((f_action == 100) && !ds_readbs(0xd82d + (signed char)fid_attacker))) ||
			((ds_readw(ATTACKER_ATTACKS_AGAIN) != 0) && (a7 == 0)) ||
			((ds_readw(DEFENDER_ATTACKS) != 0) && (a7 == 1))))
		{

		ds_writeb(0xd8ce + a1 * 0xf3, 0);

		/* find out the new direction */
		l8 = l7 = -1;

		/* try to turn right 90 degrees */
		l9 = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
		l8 = l9;
		l9++;

		if (l9 == 4) {
			l9 = 0;
		}

		if (l9 != dir) {
			l7 = l9;
			l9++;
			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != dir) {
				l8 = host_readbs(enemy + ENEMY_SHEET_VIEWDIR) + 4;
				l7 = -1;
			}
		}


		/* set the new direction in enemy sheet */
		host_writebs(enemy + ENEMY_SHEET_VIEWDIR, (signed char)dir);

		/* only if the turn is 90 degree */
		if (l7 == -1) {
			/* do not move for 2 frames */
			for (i = 0; i < 2; i++) {
				host_writeb(p1++, 0xfb);
				host_writeb(p1++, 0);
				host_writeb(p1++, 0);
			}
		}

		p1 += copy_ani_seq(p1, host_readws(p4 + l8 * 2), 1);

		if (l7 != -1) {
			p1 += copy_ani_seq(p1, host_readws(p4 + l7 * 2), 1);
		}

		host_writeb(p1++, 0xfc);
		host_writeb(p1++, get_seq_header(host_readws(p4 + l1 * 2)));
		host_writeb(p1++, 0);
	} else {
		/* do not move for 5 frames */
		for (i = 0; i < 5; i++) {
			host_writeb(p1++, 0xfb);
			host_writeb(p1++, 0);
			host_writeb(p1++, 0);
		}
	}

	if ((f_action == 2) || (f_action == 15) ||
		((f_action == 100) && !ds_readbs(0xd82d + (signed char)fid_attacker)))
	{
		p1 += copy_ani_seq(p1, host_readws(p4 + l1 *2), 1);

		if (weapon_type != -1) {

			/* do not move for 5 frames */
			for (i = 0; i < 5; i++) {
				host_writeb(p2++, 0xfb);
				host_writeb(p2++, 0);
				host_writeb(p2++, 0);
			}

			/* copy the weapon ani */
			p2 += copy_ani_seq(p2,
				ds_readws(WEAPONANI_TABLE +
					(	ds_readbs(WEAPONANI_TYPES + host_readbs(enemy + ENEMY_SHEET_GFX_ID)) * 48 +
						weapon_type * 16 +
						((f_action == 2) ? 0 : 1) * 8 +
						host_readbs(enemy + ENEMY_SHEET_VIEWDIR) * 2
					)
				), 3);
		}
	}

	if (((ds_readws(ATTACKER_ATTACKS_AGAIN) != 0) && (a7 == 0)) ||
		((ds_readws(DEFENDER_ATTACKS) != 0) && (a7 == 1))) {

			p1 += copy_ani_seq(p1, host_readws(p4 + l1 * 2), 1);

			if (weapon_type != -1) {

				/* copy the weapon ani */
				p2 += copy_ani_seq(p2,
					ds_readws(WEAPONANI_TABLE +
					((ds_readbs(WEAPONANI_TYPES + host_readbs(enemy + ENEMY_SHEET_GFX_ID)) * 48 + weapon_type * 16) +
					((f_action == 2) ? 0 : 1) * 8 + host_readbs(enemy + ENEMY_SHEET_VIEWDIR) * 2)), 3);
			}
	}

	if ( ((ds_readws(ATTACKER_DEAD) != 0) && (a7 == 0)) ||
		((ds_readws(DEFENDER_DEAD) != 0) && (a7 == 1)))
	{
		host_writeb(p1++, 0xfc);
		host_writeb(p1++, get_seq_header(host_readws(p4 + 0x28)));
		host_writeb(p1++, 0);

		p1 += copy_ani_seq(p1, host_readws(p4 + 0x28), 1);
	}

	FIG_set_0e(host_readbs(enemy + ENEMY_SHEET_FIGHTER_ID), (signed char)a1);
	/* terminate figure animation array */
	host_writebs(p1, -1);

	/* does this sprite need two fields */
	if (is_in_byte_array(host_readb(enemy + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID))	{

		memcpy(p_datseg + (0xd8ce + 2*0xf3) + a1 * 0xf3, p_datseg + 0xd8ce + a1 * 0xf3, 0xf3);

		p3 = Real2Host(FIG_get_ptr(host_readbs(enemy + ENEMY_SHEET_FIGHTER_ID)));

		FIG_set_0e(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(p3 + 0x13)), a1 + 2);
	}

	if (weapon_type != -1) {
		FIG_set_0f(host_readbs(enemy + ENEMY_SHEET_FIGHTER_ID), a1 + 4);
		/* terminate weapon animation array */
		host_writeb(p2, 0xff);
	}

	if (f_action == 100) {
		ds_writeb(0xd82d + (signed char)fid_attacker, 1);
	}
}


/**
 *
 * \param   v1          0 or 1
 * \param   hero        pointer to a hero
 * \param   v2          99 or 4
 * \param   obj1        * \param obj2	* \param v5	-1 or a var
 * \param   v6          0 or 1
 */

/* Borlandified and identical */
void seg044_002a(Bit16u v1, Bit8u *hero, Bit16u v2, Bit16s obj1, Bit16s obj2,
			Bit16u v5, Bit16u v6)
{
	signed short x_obj1, y_obj1;
	signed short x_obj2, y_obj2;
	signed short dir;
	signed short l2;
	signed short l3;
	Bit8u *lp1;
	Bit8u *lp2;

	signed short l_di;
	signed short l_si;


	/* get a pointer from an array where the Monster-ID serves as index */
	lp2 = Real2Host(ds_readd(GFX_ANI_INDEX + host_readbs(hero + HERO_SPRITE_NO) * 4));

	FIG_search_obj_on_cb((signed char)obj2, &x_obj2, &y_obj2);
	FIG_search_obj_on_cb((signed char)obj1, &x_obj1, &y_obj1);

#if !defined(__BORLANDC__)
	/* BE-fix */
	x_obj1 = host_readws((Bit8u*)&x_obj1);
	y_obj1 = host_readws((Bit8u*)&y_obj1);
	x_obj2 = host_readws((Bit8u*)&x_obj2);
	y_obj2 = host_readws((Bit8u*)&y_obj2);
#endif

	if (x_obj1 == x_obj2) {
		if (y_obj2 < y_obj1)
			dir = 1;
		else
			dir = 3;
	} else {
		if (x_obj2 < x_obj1)
			dir = 2;
		else
			dir = 0;
	}

	if ((signed char)obj2 == (signed char)obj1)
		dir = host_readbs(hero + HERO_VIEWDIR);


	l_di = (v2 == 4) ? ((v5 == 1) ? 37 : 29) : 16;

	l_di += (v2 == 4) ? dir : host_readbs(hero + HERO_VIEWDIR);

	lp1 = p_datseg + (0xd8ce + 1) + v1 * 0xf3;

	ds_writeb(0xd8ce + v1 * 0xf3, get_seq_header(host_readws(lp2 + l_di * 2)));

#if !defined(__BORLANDC__)
	ds_writeb((0xd8ce + 242) + v1 * 0xf3, host_readbs(hero + HERO_SPRITE_NO));
#else
	/* another ugly hack */
	asm {
		les bx, hero
		mov al, [es:bx+0x9b]
		mov bx, v1
		db 0x69, 0xdb, 0xf3, 0x00
		mov [bx + (0xd8ce + 242)], al
	}
#endif

	if ((host_readbs(hero + HERO_VIEWDIR) != dir) && (v2 == 4)) {

		ds_writeb(0xd8ce + v1 * 0xf3, 0);
		l3 = l2 = -1;
		l_si = host_readbs(hero + HERO_VIEWDIR);
		l3 = l_si;
		l_si++;
		if (l_si == 4)
			l_si = 0;

		if (l_si != dir) {
			l2 = l_si;
			l_si++;
			if (l_si == 4)
				l_si = 0;

			if (l_si != dir) {
				l3 = host_readbs(hero + HERO_VIEWDIR) + 4;
				l2 = -1;
			}
		}

		host_writebs(hero + HERO_VIEWDIR, (signed char)dir);
		lp1 += copy_ani_seq(lp1, host_readws(lp2 + l3 * 2), 2);

		if (l2 != -1)
			lp1 += copy_ani_seq(lp1, host_readws(lp2 + l2 * 2), 2);

		host_writeb(lp1, 0xfc);
		lp1++;

		host_writeb(lp1, get_seq_header(host_readws(lp2 + l_di * 2)));
		lp1++;

		host_writeb(lp1, 0x00);
		lp1++;
	}

	if ((v2 == 4) || check_hero(hero) ||
		((ds_readw(ATTACKER_DEAD) != 0) && (v6 == 0)) ||
		((ds_readw(DEFENDER_DEAD) != 0) && (v6 == 1))) {

		lp1 += copy_ani_seq(lp1, host_readws(lp2 + l_di * 2), 2);
	}

	if (((ds_readw(ATTACKER_DEAD) != 0) && (v6 == 0)) ||
		((ds_readw(DEFENDER_DEAD) != 0) && (v6 == 1))) {

		host_writeb(lp1, 0xfc);
		lp1++;

		host_writeb(lp1, get_seq_header(host_readws(lp2 + 0x28)));
		lp1++;

		host_writeb(lp1, 0x00);
		lp1++;

		lp1 += copy_ani_seq(lp1, host_readws(lp2 + 0x28), 2);
	}

	host_writeb(lp1, 0xff);
}


/**
 * \brief   prepares a spell animation
 *
 *          This is used for "Blitz", "Fulminictus", "Ignifaxius"
 *
 * \param   v1          0 or 1
 * \param   p           ????
 * \param   v2          4 of 99
 * \param   target      the id of the target
 * \param   caster      the id of the caster
 * \param   v5          0 or 1
 */
/* Borlandified and identical */
void seg044_002f(signed short v1, Bit8u *p, signed short v2, signed short target, signed short caster,
								signed short v5)
{
	signed short l1;
	signed short x_target, y_target;
	signed short x_caster, y_caster;
	signed short dir;
	signed short l2, l3;	/* indicees to lp2 */
	Bit8u *lp1;		/* mostly written */
	Bit8u *lp2;		/* read only */

	signed short dir2;


	/* get a pointer from an array where the Monster-ID serves as index */
	lp2 = Real2Host(ds_readd(GFX_ANI_INDEX + host_readbs(p + 1) * 4));

	FIG_search_obj_on_cb((signed char)caster, &x_caster, &y_caster);
	FIG_search_obj_on_cb((signed char)target, &x_target, &y_target);

#if !defined(__BORLANDC__)
	/* BE-fix */
	x_caster = host_readws((Bit8u*)&x_caster);
	y_caster = host_readws((Bit8u*)&y_caster);
	x_target = host_readws((Bit8u*)&x_target);
	x_target = host_readws((Bit8u*)&y_target);
#endif
	if (x_target == x_caster) {
		if (y_caster < y_target)
			dir = 1;
		else
			dir = 3;
	} else {
		if (x_caster < x_target)
			dir = 2;
		else
			dir = 0;
	}

	if ((signed char)caster == (signed char)target)
		dir = host_readbs(p + 0x27);

	/* this is true if a monster attacks a hero */
	l1 = (v2 == 4) ? 29 : 16;

	lp1 = p_datseg + (0xd8ce + 1) + v1 * 0xf3;

	/* this is true if a monster attacks a hero */
	l1 += (v2 == 4) ? dir : host_readbs(p + 0x27);

	ds_writeb(0xd8ce + v1 * 0xf3, get_seq_header(host_readws(lp2 + l1 * 2)));

	ds_writeb((0xd8ce + 242) + v1 * 0xf3, host_readbs(p + 1));

	if ((host_readbs(p + 0x27) != dir) && (v2 == 4)) {

		ds_writeb(0xd8ce + v1 * 0xf3, 0);
		l3 = l2 = -1;

		dir2 = host_readbs(p + 0x27);
		l3 = dir2;
		dir2++;
		if (dir2 == 4)
			dir2 = 0;

		if (dir2 != dir) {

			l2 = dir2;
			dir2++;
			if (dir2 == 4)
				dir2 = 0;
			if (dir2 != dir) {
				l3 = host_readbs(p + 0x27) + 4;
				l2 = -1;
			}
		}

		host_writebs(p + 0x27, (signed char)dir);

		lp1 += copy_ani_seq(lp1, host_readws(lp2 + l3 * 2), 1);

		if (l2 != -1)
			lp1 += copy_ani_seq(lp1, host_readws(lp2 + l2 * 2), 1);

		host_writeb(lp1, 0xfc);
		lp1++;

		host_writeb(lp1, get_seq_header(host_readws(lp2 + l1 * 2)));
		lp1++;

		host_writeb(lp1, 0);
		lp1++;
	}

	lp1 += copy_ani_seq(lp1, host_readws(lp2 + l1 * 2), 1);

	if (((ds_readw(ATTACKER_DEAD) != 0) && (v5 == 0)) ||
		((ds_readw(DEFENDER_DEAD) != 0) && (v5 == 1))) {

		host_writeb(lp1, 0xfc);
		lp1++;

		host_writeb(lp1, get_seq_header(host_readws(lp2 + 0x28)));
		lp1++;

		host_writeb(lp1, 0);
		lp1++;

		lp1 += copy_ani_seq(lp1, host_readws(lp2 + 0x28), 1);
	}

	host_writeb(lp1, 0xff);

	/* check if the moster sprite ID needs two fields */
	if (is_in_byte_array(host_readb(p + 1),	p_datseg + TWO_FIELDED_SPRITE_ID)) {
		memcpy(p_datseg + (0xd8ce + 2*0xf3) + v1 * 0xf3, p_datseg + 0xd8ce + v1 * 0xf3, 0xf3);
	}

}

#if !defined(__BORLANDC__)
}
#endif
