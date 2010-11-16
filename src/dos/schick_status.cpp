#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>

#include <mem.h>
#include <SDL.h>

#include "schick.h"

static const char god[][9] = {"Neu-Gott", "Praios", "Rondra", "Efferd",
				"Travia", "Boron", "Hesinde", "Firun", "Tsa",
				"Phex", "Peraine", "Ingerimm", "Rhaja",
				"Swafnir", "Ifirn"};

static const char dungeon[][25] = {	"",
				"Das Totenschiff",
				"Eine verfallene Herberge",
				"Die Spinnenhöhle",
				"Der Wolfsbau",
				"Eine Höhle",
				"Eine Höhle",
				"Eine Ruine",
				"Eine Höhle",
				"Ein Gewölbe",
				"Eine Höhle",
				"Eine Piratenhöhle",
				"Ein Dungeon",
				"Eine verlassene Miene",
				"Die alte Zwingfeste",
				"Eine Burgruine" };

static const char location[][15] = {	"",
					"Automap",
					"Tempel",
					"Taverne",
					"Heiler",
					"Händler",
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

static const char dir[][5] = { "Nord", "Ost", "Süd", "West" };

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
					"Händler Kolberg",
					"Einhorn",
					"Algrid Trondesdotter",
					"Tiomar Swafnildsson"};

static const char map[][25] = {		"oben links", "oben mitte",
					"oben rechts", "mitte links",
					"mitte", "mitte rechts", "unten links",
					"unten mitte", "unten rechts",
					"unten links (Fälschung)" };
#define NR_TIMER (26)
static char timer_flags[NR_TIMER] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0};

static const char timer_desc[NR_TIMER][40] = {	"Wunder Rondra: Schutz vor Magie",
					"Wunder Efferd: Wasser finden",
					"Wunder Efferd: Schutz auf Wasser",
					"Wunder Firun/Ifirn: Glück bei der Jagd",
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
					"Wunder Phex: Krämerseele",
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

static char slots[12] = { 0,0,0,0,0,0,0,0,0,0,0,0};

static void schick_log_timer32(const char *text, char *flag,
						const unsigned short offset) {
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
		D1_INFO("Phexschrein geplündert 0x0204\n");
		break;
	case 0x1283:
		D1_INFO("Alarm ausgelöst 0x0503\n");
		break;
	case 0x1284:
	case 0x1285:
	case 0x1286:
	case 0x1287:
	case 0x1288:
		D1_INFO("Geheimtür ");
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
			D1_INFO("geöffnet\n");
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
	case 0x128a:
		D1_INFO("Hauptkampf erledigt\n");
		break;
	default:
		D1_INFO("(0x%04lx) von %x auf %x\n", i, status_copy[i],
			status_ingame[i]);
	}

	status_copy[i]=status_ingame[i];
}

static Uint32 schick_cmp_status(Uint32 interval, void *param)
{

//	if (!running || !schick) return (interval);
	if (!status_offset) return (interval);

	unsigned long i = 0;

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
			/* Der Monat 0 ist nicht gültig */
			if (!status_ingame[0x8d])
				continue;
			if (status_ingame[0x8c] > 0)
				D1_INFO("Es ist %s der %d.%s im Jahre %d Hal\n",
						week[status_ingame[0x8b]],
						status_ingame[0x8c],
						god[status_ingame[0x8d]],
						status_ingame[0x8e]);
			/* Nameloser Tag
			else {};
			*/
			if (status_ingame[0x8f])
				D1_INFO("Besonderer Tag\n");
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
			unsigned long idx=(i-0x90)/4;
			schick_log_timer32(timer_desc[idx], &timer_flags[idx],
					0x90+idx*4);
			i = 0x90 + idx*4;
			continue;
		}

		if (i >= 0xf8 && i <= 0x187) //Zauberslots
		{
			unsigned long s_nr=(i-0xf8)/8;
			unsigned long s_of=status_offset+s_nr*8+0xf8;

			unsigned int cnt=real_readd(datseg, s_of);
			unsigned short sp=real_readw(datseg, s_of+4);
			unsigned char tar=real_readb(datseg, s_of+6);
			signed char bon=real_readb(datseg, s_of+7);

			i = 0xf8 + s_nr*8;
			/* Zauber beendet */
			if (sp != 0 && cnt == 0 && host_readd(status_copy+s_nr*8+0xf8) != 0)
			{
				D1_INFO("Zauber %ld beendet\n", s_nr+1);
				memcpy(status_copy+s_nr*8+0xf8, status_ingame+s_nr*8+0xf8, 8);
				continue;
			}

			/* Alles ausser der Counter bleiben gleich */
			if (sp == host_readw(status_copy+s_nr*8+0xf8+4) ||
				tar == host_readb(status_copy+s_nr*8+0xf8+6) ||
				bon == host_readb(status_copy+s_nr*8+0xf8+7))
			{
				host_writed(status_copy+s_nr*8+0xf8, cnt);
				continue;
			}

			if ( sp != 0 && !host_readw(status_copy+s_nr*8+0xf8+4)) {
				D1_INFO("Zauber in Slot %ld aktiviert\n",
						s_nr+1);
				D1_INFO("Zauber: 0x%x\tBonus %d auf %d\n",
						sp, bon, tar);
				memcpy(status_copy+s_nr*8+0xf8, status_ingame+s_nr*8+0xf8, 8);
			}
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
			D1_INFO("Dungeonbeleuchtung %s\n",
					status_ingame[i]?"aus":"an");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x8e0) {
			if (status_ingame[i] != 0)
				D1_INFO("Reisemodus aktiviert (0x%02x)\n",
						status_ingame[i]);
			else
				D1_INFO("Reisemodus deaktiviert\n");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		/* Dialogpartner */
		if (i >= 0x8e4 && i < 0xa1a) {
			unsigned long s_nr = (i - 0x8e4)/38;
			memcpy(status_copy+0x8e4+s_nr*38,
					status_ingame+0x8e4+s_nr*38, 38);
			D1_INFO("Dialogpartner Slot %ld geändert\n", s_nr);
			i = 0x8e4 + s_nr * 38;
			continue;

		}
		/* Dialoglayout */
		if (i >= 0xa60 && i < 0xf60) {
			unsigned long s_nr = (i - 0xa60)/8;
			memcpy(status_copy+0xa60+s_nr*8,
					status_ingame+0xa60+s_nr*8, 8);
			D1_INFO("Dialoglayout Slot %ld geändert\n", s_nr);
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
			D1_INFO("Gesprächspartner: %s\n", status_ingame+0x8ea);
			memcpy(status_copy+0x8ea, status_ingame+0x8ea, 0x1e);
			i = 0x908;
			continue;
		}
		if (i == 0x908 || i == 0x909) {
			unsigned short val=host_readw(status_ingame+0x908);
			D1_INFO("Gesprächspartner: Bild 0x%02x\n", val);
			host_writew(status_copy+0x908, val);
			i = 0x90a;
			continue;
		}
		*/
		if (i == 0x33) {
			if (status_ingame[i] != 0)
				D1_INFO("Stadtmodus aktiviert (0x%02x)\n",
						status_ingame[i]);
			else
				D1_INFO("Stadtmodus deaktiviert\n");
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}

		/* Gruppenmanagement */
		if (i == 0x01) {
			D1_INFO("Gruppe %u ausgewählt\n", status_ingame[i]);
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
			D1_INFO("Dungeonkoordinaten (0x%x/0x%x)\n",
					status_ingame[0x5db],
					status_ingame[0x5da]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i == 0x72) {
			D1_INFO("Stadt betreten: %s (0x%02x)\n", "",
					status_ingame[i]);
			status_copy[i]=status_ingame[i];
			i++;
			continue;
		}
		if (i >= 0x14de && i <= 0x14e4) {
			switch (status_ingame[i]) {
				case 0:	break;
				case 1: D1_INFO("Held %lu wird ohnmächtig\n",
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
					D1_INFO("Auftrag ausgeführt\n");
					break;
				case 4:
					D1_INFO("Quest abgeschlossen\n");
				default:
					D1_INFO("Fehler\n");
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
			D1_INFO("Story Hinweis (0x%02lx) erhalten\n", i-0x1672);
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
			D1_INFO("Extendet-");

		D1_INFO("Status[0x%lx] von %x -> %x\n",
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
		if (status_copy == NULL)
			error(1, ENOMEM, "");
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

/* Enable the timer for checking game_state changes */
void schick_status_init()
{

	status_ingame = NULL;
	status_copy = NULL;
	status_len = 0;
	status_offset = 0;

	/* Disable delay and set status manually (german CD-version) */
	if (real_readw(datseg, 0x4b66) == 0x4)
	{
		/*disable delay */
		real_writew(datseg, 0x4b66, 0x0000);
		D1_INFO("Verzögerungsfaktor ausgeschalten de_V3.02\n");

		/*set status manually */
		status_len = 0x1740;
		status_offset = 0x2d34;
		status_ingame = GetMemBase() + PhysMake(datseg, status_offset);

		if (!status_copy)
		{
			status_copy = (unsigned char*)calloc(status_len, 1);
			if (status_copy == NULL)
				error(1, ENOMEM, "");
		}
		memcpy(status_copy, status_ingame, status_len);
		D1_INFO("Status manuell gesetzt DS:0x%04x\n", status_offset);
		cmp_status_timer=SDL_AddTimer(1000, schick_cmp_status, NULL);
		if (cmp_status_timer == NULL)
			D1_ERR("Konnte den Status Timer nicht initialisieren\n");
	}
	/* Disable delay and set status manually (english version) */
	if (real_readw(datseg, 0x4c5a) == 0x4)
	{
		/*disable delay */
		real_writew(datseg, 0x4c5a, 0x0000);
		D1_INFO("Verzögerungsfaktor ausgeschalten en_V3.0x\n");

		/*set status manually */
		status_len = 0x1740;
		status_offset = 0x2b5c;
		status_ingame = GetMemBase() + PhysMake(datseg, status_offset);

		if (!status_copy)
		{
			status_copy = (unsigned char*)calloc(status_len, 1);
			if (status_copy == NULL)
				error(1, ENOMEM, "");
		}
		memcpy(status_copy, status_ingame, status_len);
		D1_INFO("Status manuell gesetzt DS:0x04x\n", status_offset);
		cmp_status_timer=SDL_AddTimer(1000, schick_cmp_status, NULL);
		if (cmp_status_timer == NULL)
			D1_ERR("Konnte den Status Timer nicht initialisieren\n");
	}
}
/* Disable the timer for checking game_state changes */
void schick_status_exit()
{
	if (cmp_status_timer) {
		SDL_RemoveTimer(cmp_status_timer);
		cmp_status_timer = NULL;
	}

	status_ingame = NULL;
	status_len = 0;
	status_offset = 0;

	if (status_copy)
	{
		free(status_copy);
		status_copy = NULL;
	}
}
