#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mem.h>
#include <SDL.h>

#include "schick.h"
#include "v302de.h"

static const char god[][9] = {"Neu-Gott", "Praios", "Rondra", "Efferd",
				"Travia", "Boron", "Hesinde", "Firun", "Tsa",
				"Phex", "Peraine", "Ingerimm", "Rhaja",
				"Swafnir", "Ifirn"};

static const char dungeon[][25] = {	"",
				"Das Totenschiff",
				"Eine verfallene Herberge",
				"Die Spinnenhoehle",
				"Der Wolfsbau",
				"Eine Hoehle",
				"Eine Hoehle",
				"Eine Ruine",
				"Eine Hoehle",
				"Ein Gewoelbe",
				"Eine Hoehle",
				"Eine Piratenhoehle",
				"Ein Dungeon",
				"Eine verlassene Miene",
				"Die alte Zwingfeste",
				"Eine Burgruine" };

static const char location[][15] = {	"",
					"Automap",
					"Tempel",
					"Taverne",
					"Heiler",
					"Haendler",
					"Wildniscamp",
					"Herberge",
					"Schmied",
					"Marktplatz",
					"Dorfbewohner",
					"Hafen",
					"Landkarte",
					"Informant",
					"Dungeoneingang",
					"",
					"Haus",
					"Dialog",
					"Stadtcamp",
};

static const char dir[][5] = { "Nord", "Ost", "Sued", "West" };

static const char week[][10] = {	"Rohalstag", "Feuertag",
					"Wassertag", "Windstag",
					"Erdstag", "Markttag", "Praiostag" };

static const char informer[][30] = {	"Jurge Torfinnson",
					"Hjore Ahrensson",
					"Yasma Thinmarsdotter",
					"Umbrik Siebenstein",
					"Isleif Olgardsson",
					"Ragna Firunjasdotter",
					"Beorn Hjallasson",
					"Asgrimm Thurboldsson",
					"Eliane Windenbeck",
					"Olvir Gundridsson",
					"Swafnild Egilsdotter",
					"Haendler Kolberg",
					"Einhorn",
					"Algrid Trondesdotter",
					"Tiomar Swafnildsson"};

static const char map[][25] = {		"oben links", "oben mitte",
					"oben rechts", "mitte links",
					"mitte", "mitte rechts", "unten links",
					"unten mitte", "unten rechts",
					"unten links (Faelschung)" };
#define NR_TIMER (26)
static char timer_flags[NR_TIMER] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0};

static const char timer_desc[NR_TIMER][40] = {	"Wunder Rondra: Schutz vor Magie",
					"Wunder Efferd: Wasser finden",
					"Wunder Efferd: Schutz auf Wasser",
					"Wunder Firun/Ifirn: Glueck bei der Jagd",
					"Wunder Travia: Schutz in der Nacht",
					"Unbekannt1",
					"Akrobatik",
					"Musizieren",
					"Flim Flam",
					"Dunkelheit",
					"Unbekannt2",
					"Unbekannt3",
					"Wunder Rondra: Geschickter Schwertarm",
					"Wunder Rondra: Magische Waffe",
					"Unbekannt4",
					"Unbekannt5",
					"Unbekannt6",
					"Unbekannt7",
					"Wunder Ifirn: Kein Hunger&Durst",
					"Wunder Phex: Diebesherzen",
					"Wunder Phex: Kraemerseele",
					"Wunder Phex: Zauber auf die Finger",
					"Unbekannt9",
					"Unbekannt10",
					"Unbekannt11",
					"Unbekannt12"};

//Status comparator
static SDL_TimerID cmp_status_timer = NULL;

//Pointer to game status
static unsigned char *status_ingame = NULL;
static unsigned char *status_copy = NULL;
static unsigned int status_len = 0;
static unsigned short status_offset = 0;

static void schick_log_timer32(const char *text, char *flag,
						unsigned long offset) {
	if (!text) {
		D1_ERR("%s argument text is null\n", __func__);
		return;
	}
	if (!flag) {
		D1_ERR("%s argument flag is null\n", __func__);
		return;
	}

	unsigned int oldval = host_readd(status_copy+offset);
	unsigned int newval = host_readd(status_ingame+offset);

	if (*flag) {
		if (newval != 0 && newval < oldval) {
			host_writed(status_copy+offset, newval);
			return;
		}
		if (newval == 0x00) {
			*flag=*flag-1;
			D1_INFO("Timer %s beendet\n", text);
			host_writed(status_copy+offset, newval);
			return;
		}
		if (newval > oldval) {
			D1_INFO("Timer %s reaktiviert\n", text);
			D1_INFO("0x%08x -> 0x%08x\n", oldval, newval);
			host_writed(status_copy+offset, newval);
			return;
		}
	}
	if (oldval == 0x00 && !*flag) {
		*flag=*flag+1;
		D1_INFO("Timer %s: 0x%08x\n", text, newval);
		host_writed(status_copy+offset, newval);
		return;
	}
	D1_ERR("Strange flag == %u oldval == %u newval == %u\n",
						*flag, oldval, newval);
	host_writed(status_copy+offset, newval);
}
static void schick_status_dng_thorwal(unsigned long i) {

	D1_INFO("Dungeon (%s): ", dungeon[14]);
	switch (i) {

	case 0x1282:
		D1_INFO("Phexschrein gepluendert 0x0204\n");
		break;
	case 0x1283:
		D1_INFO("Alarm ausgeloest 0x0503\n");
		break;
	case 0x1284:
	case 0x1285:
	case 0x1286:
	case 0x1287:
	case 0x1288:
		D1_INFO("Geheimtuer ");
		switch (i) {
		case 0x1284:
			D1_INFO("0x040b ");
			break;
		case 0x1285:
			D1_INFO("0x0b07 ");
			break;
		case 0x1286:
			D1_INFO("0x1109 ");
			break;
		case 0x1287:
			D1_INFO("0x170a ");
			break;
		case 0x1288:
			D1_INFO("0x2e05 ");
			break;
		}
		switch (status_ingame[i]) {
		case 0:
			D1_INFO("versteckt?\n");
			break;
		case 1:
			D1_INFO("gefunden\n");
			break;
		case 2:
			D1_INFO("geoeffnet\n");
			break;
		}
		break;
	case 0x1289:
		switch (status_ingame[i]) {
		case 1:
			D1_INFO("Giftfalle 0x0d07 aktiviert\n");
			break;
		case 0:
		case 2:
			D1_INFO("Giftfalle 0x0d07 deaktiviert\n");
			break;
		}
		break;
	case 0x128a:
		D1_INFO("Hauptkampf erledigt\n");
		break;
	default:
		D1_INFO("(0x%04lx) von %x auf %x\n", i, status_copy[i],
			status_ingame[i]);
	}

	status_copy[i]=status_ingame[i];
}

static void schick_cmp_heros()
{

	Bit8u *hero;
	unsigned long i, j;
	unsigned char items;

	/* check for non NULL */
	if (ds_readd(HEROS) == 0) {
		D1_ERR("Null ptr\n");
		return;
	}

	hero = get_hero(0);

	for (i = 0; i < 7; i++, hero += 0x6da) {

		if (host_readb(hero + 0x21) == 0)
			continue;

		/* check for invalid item counter */
		items = 0;
		for (j = 0; j < 23; j++) {
			if (host_readb(hero + 0x196 + j * 14))
				items++;
		}

		if ((signed char)host_readb(hero + 0x20) == items)
			continue;

		D1_ERR("Original-Bug: %s hat einen ungueltigen Gegenstandszaehler.\n",
			(char*)(hero + 0x10));
		D1_ERR("\tKorrigiere den Wert von %d -> %d\n",
			host_readb(hero + 0x20), items);

		host_writeb(hero + 0x20, items);
	}

	hero = Real2Host(ds_readd(0xbd34));
	for (i = 0; i < 7; i++, hero += 0x6da) {
		/* check for invalid skill_attempts */
		if ((signed char)host_readb(hero + 0x13c) < 0) {
			D1_ERR("Original-Bug: %s hat negative Talentsteigerungen\n", (char*)(hero + 0x10));
			host_writeb(hero + 0x13c, 0);
		}

		/* check for invalid spell_attempts */
		if ((signed char)host_readb(hero + 0x193) < 0) {
			D1_ERR("Original-Bug: %s hat negative Zaubersteigerungen\n", (char*)(hero + 0x10));
			host_writeb(hero + 0x193, 0);
		}
	}
}

static Uint32 schick_cmp_status(Uint32 interval, void *param)
{

//	if (!running || !schick) return (interval);
	if (!status_offset) return (interval);

	unsigned long i = 0;

	schick_cmp_heros();

	while (i < status_len)
	{
		if (status_ingame[i] == status_copy[i])	{
			i++;
			continue;
		}

		/* Palette */
		if (i >= 0x111e && i <= 0x123e) {
			memcpy(status_copy+0x111e, status_ingame+0x111e, 0x120);
			i = 0x123f;
			continue;
		}

		/* frequently often changed timer timer++ */
		/* TODO: shows two timer values after 12.00 */
		if (i >= 0x87 && i <= 0x8a)
		{
			unsigned int timer = host_readd(status_ingame+0x87);
//			D1_INFO("Timer1 %08x\n", timer);
			host_writed(status_copy+0x87, timer);
			i = 0x8b;
			continue;
		}
		if (i == 0x8b || i == 0x8c) {
			memcpy(status_copy+0x8b, status_ingame+0x8b, 5);
			i = 0x90;
			/* Der Monat 0 ist nicht gueltig */
			if (!status_ingame[0x8d])
				continue;
			if (status_ingame[0x8c] > 0)
				D1_LOG("Es ist %s der %d.%s im Jahre %d Hal\n",
						week[status_ingame[0x8b]],
						status_ingame[0x8c],
						god[status_ingame[0x8d]],
						status_ingame[0x8e]);
			/* Nameloser Tag
			else {};
			*/
			if (status_ingame[0x8f])
				D1_LOG("Besonderer Tag\n");
			continue;
		}
		/* frequently often changed timer timer-= 256 */
		if (i >= 0x126d && i <= 0x1270)
		{
			unsigned int timer = host_readd(status_ingame+0x126d);
			host_writed(status_copy+0x126d, timer);
			i = 0x1271;
			continue;
		}
		/* frequently often changed timer timer traveltimer*/
		if (i == 0x14f4 || i == 0x14f5)
		{
			unsigned int timer = host_readw(status_ingame+0x14f4);
			host_writew(status_copy+0x14f4, timer);
			i = 0x14f6;
			continue;
		}
		/* Timers */
		if (i >= 0x90 && i < 0xf8) {
			unsigned long idx = (i - 0x90) / 4;
			schick_log_timer32(timer_desc[idx], &timer_flags[idx],
					idx * 4 + 0x90);
			i = 0x90 + idx * 4;
			continue;
		}

		if (i >= 0xf8 && i < 0x418) {
			//modification slots 100x8 byte
			unsigned short s_nr=((unsigned short)i-0xf8)/8;
			unsigned short s_of=status_offset+s_nr*8+0xf8;

			unsigned int cnt = ds_readd(s_of);
			unsigned short off = ds_readw(s_of + 4);
			unsigned char tar = ds_readb(s_of + 6);
			signed char mod = ds_readb(s_of + 7);

			/* align i to the mod slot */
			i = 0xf8 + s_nr*8;

			/* deactivate mod slot */
			if (cnt == 0 && host_readd(status_copy+s_nr*8+0xf8))
			{
				D1_INFO("Mod Timer %d beendet\n", s_nr);
				memcpy(status_copy+s_nr*8+0xf8,
					status_ingame+s_nr*8+0xf8, 8);
				i += 8;
				continue;
			}

			/* only subtract the timer */
			if (off == host_readw(status_copy+s_nr*8+0xf8+4) ||
				tar == host_readb(status_copy+s_nr*8+0xf8+6) ||
				mod == host_readb(status_copy+s_nr*8+0xf8+7))
			{
				host_writed(status_copy+s_nr*8+0xf8, cnt);
				i += 8;
				continue;
			}

			/* activate a new slot */
			if (off != 0 && !host_readw(status_copy+s_nr*8+0xf8+4)) {
				D1_INFO("Mod Timer in Slot %d aktiviert\n",
						s_nr);
				D1_INFO("Offset: 0x%x\tBonus %d auf %d\n",
						off, mod, tar);
				memcpy(status_copy+s_nr*8+0xf8,
					status_ingame+s_nr*8+0xf8, 8);
				i += 8;
				continue;
			}

			D1_ERR("Broken Mod Timer logic\n");
			i += 8;
			continue;
		}
		/* check gods estimation */
		if (i >= 0x041a && i < 0x0456) {
			unsigned long idx=(i-0x041a)/4;
			int oldval=host_readd(status_copy+idx*4+0x41a);
			int newval=host_readd(status_ingame+idx*4+0x41a);
			D1_INFO("Ansehen %s: von %d auf %d\n", god[idx],
					oldval, newval);
			host_writed(status_copy+idx*4+0x041a, newval);
			i = 0x41a + idx*4;
			continue;
		}

		if (i >= 0x456 && i <= 0x4ad) {
			D1_INFO("Beim Falschspiel in Taverne (0x%02lx) erwischt\n",
					i-0x456);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x2c) {
			if (status_ingame[i])
				D1_INFO("Location betreten: %s\n",
					location[status_ingame[i]]);
			else
				D1_INFO("Location verlassen: %s\n",
					location[status_copy[i]]);

			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x14ee || i == 0x14ef) {
			unsigned short val=host_readw(status_ingame+0x14ee);
			D1_INFO("Stadtindex: 0x%04x\n", val);
			host_writew(status_copy+0x14ee, val);
			i = 0x14f0;
			continue;
		}
		if (i == 0x14f0 || i == 0x14f1) {
			unsigned short val=host_readw(status_ingame+0x14f0);
			D1_INFO("Typindex: 0x%04x\n", val);
			host_writew(status_copy+0x14f0, val);
			i = 0x14f2;
			continue;
		}
		if (i == 0x3a) {
			if (status_ingame[i] != 0)
				D1_INFO("Dungeon \"%s\" betreten\n", dungeon[status_ingame[i]]);
			else
				D1_INFO("Dungeon \"%s\" verlassen\n", dungeon[status_copy[i]]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x8df) {
			D1_LOG("Dungeonbeleuchtung %s\n",
					status_ingame[i]?"aus":"an");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x8e0) {
			if (status_ingame[i] != 0)
				D1_LOG("Reisemodus aktiviert\n");
			else
				D1_LOG("Reisemodus deaktiviert\n");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		/* Dialogpartner */
		if (i >= 0x8e4 && i < 0xa1a) {
			unsigned long s_nr = (i - 0x8e4)/38;
			memcpy(status_copy+0x8e4+s_nr*38,
					status_ingame+0x8e4+s_nr*38, 38);
			D1_LOG("Dialogpartner Slot %ld geaendert\n", s_nr);
			i = 0x8e4 + s_nr * 38;
			continue;

		}
		/* Dialoglayout */
		if (i >= 0xa60 && i < 0xf60) {
			unsigned long s_nr = (i - 0xa60)/8;
			memcpy(status_copy+0xa60+s_nr*8,
					status_ingame+0xa60+s_nr*8, 8);
			D1_LOG("Dialoglayout Slot %ld geaendert\n", s_nr);
			i = 0xa60 + s_nr * 8;
			continue;
		}
		/*
		if (i >= 0x8e4 && i < 0x8e8) {
			int oldval=host_readd(status_copy+0x8e4);
			int newval=host_readd(status_ingame+0x8e4);
			D1_INFO("Pointer 0x%04x:0x%04x -> 0x%04x:0x%04x\n",
				(oldval>>16) & 0xffff, oldval & 0xffff,
				(newval>>16) & 0xffff, newval & 0xffff);

			host_writed(status_copy+0x08e4, newval);
			i = 0x8e8;
			continue;
		}
		if (i >= 0x8ea && i < 0x908) {
			D1_INFO("Gespraechspartner: %s\n", status_ingame+0x8ea);
			memcpy(status_copy+0x8ea, status_ingame+0x8ea, 0x1e);
			i = 0x908;
			continue;
		}
		if (i == 0x908 || i == 0x909) {
			unsigned short val=host_readw(status_ingame+0x908);
			D1_INFO("Gespraechspartner: Bild 0x%02x\n", val);
			host_writew(status_copy+0x908, val);
			i = 0x90a;
			continue;
		}
		*/
		if (i == 0x33) {
			if (status_ingame[i] != 0)
				D1_LOG("Stadtmodus aktiviert (0x%02x)\n",
						status_ingame[i]);
			else
				D1_LOG("Stadtmodus deaktiviert\n");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		/* Gruppenmanagement */
		if (i == 0x01) {
			D1_INFO("Gruppe %u ausgewaehlt\n", status_ingame[i]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x09) {
			D1_LOG("Blickrichtung %s\n", dir[status_ingame[i]]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		if (i == 0x10 || i == 0x12) {
			D1_LOG("Koordinaten Ziel (%u/%u)\n",
					status_ingame[0x10],
					status_ingame[0x12]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x48) {
			D1_LOG("alte Blickrichtung %s\n",
					dir[status_ingame[i]]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x4f || i == 0x51) {
			D1_LOG("Koordinaten (%u/%u)\n",
					status_ingame[0x4f],
					status_ingame[0x51]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x5da || i == 0x5db) {
			/* Ist Word */
			D1_LOG("Dungeonkoordinaten (0x%x/0x%x)\n",
					status_ingame[0x5db],
					status_ingame[0x5da]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x72) {
			D1_LOG("Stadt betreten: %s (0x%02x)\n", "",
					status_ingame[i]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i >= 0x14de && i <= 0x14e4) {
			switch (status_ingame[i]) {
				case 0:	break;
				case 1: D1_INFO("Held %lu wird ohnmaechtig\n",
							i - 0x14de + 1);
					break;
				default:
					D1_INFO("Held %lu wird 0x%02x\n",
							i - 0x14de + 1,
							status_ingame[i]);
			}
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		/* Informer */
		if (i >= 0x630 && i <= 0x63e) {
			D1_INFO("Informant %s ist %s\n",
					informer[i-0x630],
					status_ingame[i]==2? "abgeschlossen":"jetzt bekannt");
			/* Original Bugfix : Set Umbrik to know if he is done */
			if (i == 0x633 && status_ingame[i] == 1 && status_copy[i] == 2) {
				status_ingame[i] = 2;
				D1_INFO("Original-Bug (fixed): Umbrik Siebenstein\n");
}
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		/* Maps */
		if (i >= 0x5fe && i <= 0x607) {
			D1_INFO("Kartenteil %s erhalten\n", map[i-0x5fe]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		if (i >= 0x161d && i<= 0x166f) {
			D1_INFO("Gegner (0x%02lx) zum ersten mal besiegt\n",
					i - 0x161d);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x622) {
			short newval = host_readw(status_ingame + i);
			D1_INFO("Quest: Zwingfeste Thorwal ");
			switch (newval) {
				case 1:
					D1_INFO("Auftrag angenommen\n");
					break;
				case 3:
					D1_INFO("Auftrag ausgefuehrt\n");
					break;
				case 4:
					D1_INFO("Quest abgeschlossen\n");
					break;
				default:
					D1_ERR("Fehler\n");
					exit(1);
			}
			host_writew(status_copy + i, newval);
			i += 2;
			continue;
		}
		if (i >= 0x6cc && i <= 0x715) {
			D1_INFO("Ihr habt Herberge (0x%02lx) beleidigt\n",
					i - 0x6cc);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		if (i >= 0x1282 && i <= 0x1292) {
			schick_status_dng_thorwal(i);
			i = 0x1293;
			continue;
		}
		/* Extendet-Status */
		/* Texte aus STORY.LTX */
		if (i >= 0x1672 && i <= 0x1682) {
			D1_LOG("Story Hinweis (0x%02lx) erhalten\n", i-0x1672);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		/* Ingame Diary */
		if (i >= 0x1686) {
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		if (i >= 0x1670)
			D1_LOG("Extendet-");

		D1_LOG("Status[0x%lx] von %x -> %x\n",
				i, status_copy[i], status_ingame[i]);

		status_copy[i]=status_ingame[i];
		i++;
	}
	return (interval);
}

/* Updates the status when loading a savegame */
void schick_status_update(unsigned char *data, unsigned short len)
{
	unsigned i;

	if (status_len == 0)
		status_len = len;

	if (!status_copy)
	{
		status_copy = (unsigned char*)calloc(status_len, 1);
		if (status_copy == NULL) {
			D1_ERR("Kein Speicher um eine Kopie des Statusbereiches zu erstellen\n");
			exit(1);
		}
	}


	memcpy(status_copy, data, status_len);
	D1_INFO("Status kopiert!\n");

	for (i=0; i<NR_TIMER; i++)
		if (host_readd(status_copy+4*i+0x90))
			timer_flags[i]=1;
		else
			timer_flags[i]=0;

	if (cmp_status_timer == NULL) {
		cmp_status_timer=SDL_AddTimer(1000, schick_cmp_status, NULL);
		if (cmp_status_timer == NULL)
			D1_ERR("Konnte den Status Timer nicht initialisieren\n");
	}
}

/**
 *	schick_status_enable -  enable status timer
 *
 * Enable the timer for checking changes in the game state
 */
void schick_status_enable()
{
	if (cmp_status_timer) {
		D1_ERR("Tried to enable an already enabled timer\n");
		return;
	}

	cmp_status_timer = SDL_AddTimer(1000, schick_cmp_status, NULL);

	if (cmp_status_timer == NULL) {
		D1_ERR("Konnte den Status Timer nicht initialisieren\n");
		return;
	}

	D1_INFO("Statusvergleich aktiviert\n");
}

/**
 *	schick_status_disable -  disable status timer
 *
 * Disable the timer for checking changes in the game state
 */
void schick_status_disable()
{
	if (cmp_status_timer == NULL) {
		D1_ERR("Tried to disable a already disabled timer\n");
		return;
	}

	SDL_RemoveTimer(cmp_status_timer);
	cmp_status_timer = NULL;

	D1_INFO("Statusvergleich deaktiviert\n");
}


/* Initialize a timer for checking game_state changes */
void schick_status_init()
{
	int ver;

	ver = schick_get_version((char*)MemBase + (datseg<<4));

	status_ingame = NULL;
	status_copy = NULL;
	status_len = 0;
	status_offset = 0;

	/* Disable delay and set status manually (german CD-version) */
	if (ver == 302 && !schick_is_en())
	{
#if 0
		/*disable delay */
		ds_writew(0x4b66, 0x0000);
		D1_INFO("Verzoegerungsfaktor ausgeschalten\n");
#endif

		/*set status manually */
		status_len = 0x1740;
		status_offset = 0x2d34;
	}

	/* set status manually (V1.0x german floppy versions) */
	if (ver < 300 && !schick_is_en())
	{
		/*set status manually */
		status_len = 0x1670;
		switch (ver) {

			case 100:
					status_offset = 0x2b56;
					break;
			case 104:
					status_offset = 0x2b5a;
					break;
			case 107:
					status_offset = 0x2b56;
					break;
			default:
					D1_ERR("Status kann nicht gesetzt werden\n");
					D1_ERR("Unbekannte Floppy Version\n");

					return;
		}
	}

	/* Disable delay and set status manually (english version) */
	if (schick_is_en()) {
#if 0
		/*disable delay */
		ds_writew(0x4c5a, 0x0000);
		D1_INFO("Verzoegerungsfaktor ausgeschalten\n");
#endif

		/*set status manually */
		status_len = 0x1740;
		status_offset = 0x2b5c;
	}

	status_ingame = GetMemBase() + PhysMake(datseg, status_offset);

	if (!status_copy) {
		status_copy = (unsigned char*)calloc(status_len, 1);
		if (status_copy == NULL) {
			D1_ERR("Kein Speicher um eine Kopie des Statusbereiches zu erstellen\n");
			exit(1);
		}
	}

	memcpy(status_copy, status_ingame, status_len);
	D1_TRAC("Status manuell gesetzt DS:0x%04x\n", status_offset);

	schick_status_enable();
}


/* Exit the timer for checking game_state changes */
void schick_status_exit()
{
	schick_status_disable();

	status_ingame = NULL;
	status_len = 0;
	status_offset = 0;

	if (status_copy)
	{
		free(status_copy);
		status_copy = NULL;
	}
}
