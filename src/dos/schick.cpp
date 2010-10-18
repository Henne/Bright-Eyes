#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schick.h"
#include "cpu.h"

#include "schick_rewrite/seg002.h"
#include "schick_rewrite/seg003.h"
#include "schick_rewrite/seg007.h"

#define SCHICK_DAT(pos, name)	case pos: strcpy(file, name); break;

/* dice table */
static char dice_tab[4] = {6, 20, 3, 4};

// Is the profiler running?
static int running=0;
// Is the game running?
static int schick = 0;
// Is gen called from the game?
static int gen=0;
//Has the game called gen?
static int fromgame = 0;
// Is file schick.dat ?
static int dathandle = 0;
// Was an interesting function called?
static int call=0;
// Supress this many random()-calls
static int supress_rnd=0;
// Segment relocation
static unsigned short relocation;

// Debugging Mode (Bitfield): Bit 1=File Operations, Bit 2=Proben
static int dbg_mode=2;

//Datasegment
unsigned short datseg;

//Initializer - is startet if executed file is SCHICKM.EXE/BLADEM.EXE or GEN.EXE
void init_schick(char *name, unsigned short reloc, unsigned short _cs, unsigned short ip)
{

	char borsig[] = "Borland C++ - Copyright 1991 Borland Intl.";

	//This happens only if the game starts another program
	if (!fromgame && running && schick && !gen)
	{
		if (strcasestr(name, "gen.exe"))
		{
			schick--;
			fromgame++;
			gen++;
			D1_INFO("Gen gestartet\nProfiling angehalten\n");
		}
		return;
	}
	D1_INFO("executing %s\n", name);
	if (!strcasestr(name, "schickm.exe")
			&& !strcasestr(name, "bladem.exe")
			&& !strcasestr(name, "gen.exe")) return;

       /* Check CS:IP in the EXE-Header are 0:0
        * and the first executed instruction is mov dx,i16 */
       if (_cs != 0 || ip != 0 || real_readb(reloc+_cs, ip) != 0xba)
               return;

       /* Show CS:IP on the virtual machine and the pointer to 0:0 */
       D1_INFO("\n\nCS:IP 0x%x:0x%x\tMemBase: %p\n", reloc, ip, MemBase);

	/* Read and show the Datasegment */
	datseg = real_readw(reloc, ip+1);
	D1_INFO("Dseg: 0x%X\n", datseg=real_readw(reloc, ip+1));

       /* Check if the start of the Datasegment is Borland C++ */
       if (real_readd(datseg, 0) != 0 || strcmp((char*)MemBase+(datseg<<4)+4, borsig))
       {
               D1_ERR("Kein Borland C++ Kompilat!\n");
               return;
       }

	D1_INFO("DSA1 Schicksalsklinge gefunden\n");
	D1_INFO("Starte Profiler (reloc 0x%06x)\n", reloc);

	if (strcasestr(name, "schickm.exe") || strcasestr(name, "bladem.exe")) {
		schick++;
		schick_status_init();
	}

	if (strcasestr(name, "gen.exe"))
		gen++;

	relocation = reloc;
	running++;
}


void exit_schick(unsigned char exit)
{
	if (!running) return;

	if (fromgame)
	{
		gen--;
		fromgame--;
		schick++;
		D1_INFO("Gen beendet\nProfiling geht weiter\n");
		return;
	}

	if (gen)
		gen--;
	if (schick) {
		schick--;
		schick_status_exit();
	}
	running--;
	D1_INFO("DSA1 Fehlercode %d\nProfiler beendet\n", exit);
}

void schick_create(const char *name, unsigned char flags, unsigned int handle)
{

	if (!running || !schick || !(dbg_mode & 1) ) return;

	D1_ERR("Create File\tHandle %d\t%s\tFlags\n", handle, name, flags);
}

void schick_open(const char *name, unsigned char flags, unsigned int handle)
{

	if (!running || !schick) return;

	if (strstr(name, "SCHICK.DAT")) dathandle=handle;

	if (!(dbg_mode & 1)) return;
	D1_ERR("Open File\tHandle %d\t%s\tFlags\n", handle, name, flags);
}

void schick_close(unsigned handle)
{
	if (!running || !schick) return;

	if (handle == dathandle) dathandle=0;

	if (!(dbg_mode & 1)) return;
	D1_LOG("Close File\tHandle %d\n\n", handle);
}

void schick_read(unsigned handle, unsigned char *data, unsigned short len)
{
	if (!running || !schick) return;

	if ((len == 5744 || len == 5952) && dathandle == 0)
			schick_status_update(data, len);

	if (!(dbg_mode & 1)) return;

	D1_LOG("ReadFile\tHandle %d\tLen: %d\n", handle, len);
}

void schick_write(unsigned handle, unsigned char *data, unsigned short len)
{
	if (!running || !schick || !(dbg_mode & 1) ) return;

	D1_LOG("WriteFile\tHandle %d\tLen: %d\n", handle, len);
}

void schick_seek(unsigned handle, unsigned pos, unsigned type)
{
	char file[20];

	if (!running || !schick || !(dbg_mode & 1) ) return;

	if (handle != dathandle)
	{
		D1_LOG("Seek File\tHandle %x\tPos %ld\tType %x\n",
							handle, pos, type);
		return;
	}

	switch (pos) {
		SCHICK_DAT(1232, "PLAYM_UK")
		SCHICK_DAT(11904, "KDBACK.DAT")
		SCHICK_DAT(26552, "KCBACK.DAT")
		SCHICK_DAT(43952, "KLBACK.DAT")
		SCHICK_DAT(62996, "KSBACK.DAT")
		SCHICK_DAT(73876, "KARTE.DAT")
		SCHICK_DAT(137974, "BICONS")
		SCHICK_DAT(143158, "OBJECTS.NVF")
		SCHICK_DAT(145481, "LROUT.DAT")
		SCHICK_DAT(153013, "SROUT.DAT")
		SCHICK_DAT(158889, "HSROUT.DAT")
		SCHICK_DAT(162645, "FONT6")
		SCHICK_DAT(163237, "FONT8")
		SCHICK_DAT(163773, "GGSTS.NVF")
		SCHICK_DAT(180544, "TEXT.LTX")
		SCHICK_DAT(208675, "ICONS")
		SCHICK_DAT(241125, "MONSTER")
		SCHICK_DAT(728165, "MONSTER.TAB")
		SCHICK_DAT(728313, "COMPASS")
		SCHICK_DAT(730480, "MAPTEXT.LTX")
		SCHICK_DAT(745153, "ZUSTA_UK")
		SCHICK_DAT(750497, "FIGHTTXT.LTX")
		SCHICK_DAT(752411, "ANIS")
		SCHICK_DAT(1189085, "ANIS.TAB")
		SCHICK_DAT(1189233, "MFIGS")
		SCHICK_DAT(1745822, "MFIGS.TAB")
		SCHICK_DAT(1745998, "THORWAL.DAT")
		SCHICK_DAT(1747138, "SERSKE.DAT")
		SCHICK_DAT(1747524, "BREIDA.DAT")
		SCHICK_DAT(1747952, "PEILINEN.DAT")
		SCHICK_DAT(1748350, "ROVAMUND.DAT")
		SCHICK_DAT(1748784, "NORDVEST.DAT")
		SCHICK_DAT(1749200, "KRAVIK.DAT")
		SCHICK_DAT(1749568, "SKELELLE.DAT")
		SCHICK_DAT(1749984, "MERSKE.DAT")
		SCHICK_DAT(1750382, "EFFERDUN.DAT")
		SCHICK_DAT(1750840, "TJOILA.DAT")
		SCHICK_DAT(1751196, "RUKIAN.DAT")
		SCHICK_DAT(1751564, "ANGBODIRTAL.DAT")
		SCHICK_DAT(1751938, "AUPLOG.DAT")
		SCHICK_DAT(1752306, "VILNHEIM.DAT")
		SCHICK_DAT(1752758, "BODON.DAT")
		SCHICK_DAT(1753096, "OBERORKEN.DAT")
		SCHICK_DAT(1753620, "PHEXCAER.DAT")
		SCHICK_DAT(1754454, "GROENVEL.DAT")
		SCHICK_DAT(1754804, "FELSTEYN.DAT")
		SCHICK_DAT(1755196, "EINSIEDL.DAT")
		SCHICK_DAT(1755534, "ORKANGER.DAT")
		SCHICK_DAT(1755914, "CLANEGH.DAT")
		SCHICK_DAT(1756372, "LISKOR.DAT")
		SCHICK_DAT(1756794, "THOSS.DAT")
		SCHICK_DAT(1757174, "TJANSET.DAT")
		SCHICK_DAT(1757566, "ALA.DAT")
		SCHICK_DAT(1757916, "ORVIL.DAT")
		SCHICK_DAT(1758368, "OVERTHORN.DAT")
		SCHICK_DAT(1758802, "ROVIK.DAT")
		SCHICK_DAT(1759176, "HJALSING.DAT");
		SCHICK_DAT(1759592, "GUDDASUN.DAT");
		SCHICK_DAT(1760002, "KORD.DAT");
		SCHICK_DAT(1760442, "TREBAN.DAT");
		SCHICK_DAT(1760786, "ARYN.DAT");
		SCHICK_DAT(1761142, "RUNINSHA.DAT");
		SCHICK_DAT(1761516, "OTTARJE.DAT");
		SCHICK_DAT(1761920, "SKJAL.DAT");
		SCHICK_DAT(1762366, "PREM.DAT");
		SCHICK_DAT(1763140, "DASPOTA.DAT");
		SCHICK_DAT(1763592, "RYBON.DAT");
		SCHICK_DAT(1763948, "LJASDAHL.DAT");
		SCHICK_DAT(1764382, "VARNHEIM.DAT");
		SCHICK_DAT(1764828, "VAERMHAG.DAT");
		SCHICK_DAT(1765214, "TYLDON.DAT");
		SCHICK_DAT(1765612, "VIDSAND.DAT");
		SCHICK_DAT(1766040, "BRENDHIL.DAT");
		SCHICK_DAT(1766396, "MANRIN.DAT");
		SCHICK_DAT(1766836, "FTJOILA.DAT");
		SCHICK_DAT(1767186, "FANGBODI.DAT");
		SCHICK_DAT(1767536, "HJALLAND.DAT");
		SCHICK_DAT(1767868, "RUNIN.DAT");
		SCHICK_DAT(1768224, "THORWAL.LTX");
		SCHICK_DAT(1773637, "SERSKE.LTX");
		SCHICK_DAT(1773728, "BREIDA.LTX");
		SCHICK_DAT(1775116, "PEILINEN.LTX");
		SCHICK_DAT(1775790, "ROVAMUND.LTX");
		SCHICK_DAT(1776939, "NORDVEST.LTX");
		SCHICK_DAT(1777503, "KRAVIK.LTX");
		SCHICK_DAT(1778152, "SKELELLE.LTX");
		SCHICK_DAT(1779191, "MERSKE.LTX");
		SCHICK_DAT(1781290, "EFFERDUN.LTX");
		SCHICK_DAT(1782154, "TJOILA.LTX");
		SCHICK_DAT(1783562, "RUKIAN.LTX");
		SCHICK_DAT(1784666, "ANGBODIR.LTX");
		SCHICK_DAT(1785896, "AUPLOG.LTX");
		SCHICK_DAT(1787459, "VILNHEIM.LTX");
		SCHICK_DAT(1789593, "BODON.LTX");
		SCHICK_DAT(1791668, "OBERORKE.LTX");
		SCHICK_DAT(1793439, "PHEXCAER.LTX");
		SCHICK_DAT(1795875, "GROENVEL.LTX");
		SCHICK_DAT(1796145, "FELSTEYN.LTX");
		SCHICK_DAT(1797822, "EINSIEDL.LTX");
		SCHICK_DAT(1797900, "ORKANGER.LTX");
		SCHICK_DAT(1798999, "CLANEGH.LTX");
		SCHICK_DAT(1800486, "LISKOR.LTX");
		SCHICK_DAT(1802018, "THOSS.LTX");
		SCHICK_DAT(1803659, "TJANSET.LTX");
		SCHICK_DAT(1805062, "ALA.LTX");
		SCHICK_DAT(1806006, "ORVIL.LTX");
		SCHICK_DAT(1807533, "OVERTHOR.LTX");
		SCHICK_DAT(1809011, "ROVIK.LTX");
		SCHICK_DAT(1810301, "HJALSING.LTX");
		SCHICK_DAT(1811670, "GUDDASUN.LTX");
		SCHICK_DAT(1812360, "KORD.LTX");
		SCHICK_DAT(1813530, "TREBAN.LTX");
		SCHICK_DAT(1814467, "ARYN.LTX");
		SCHICK_DAT(1815308, "RUNINSHA.LTX");
		SCHICK_DAT(1816216, "OTTARJE.LTX");
		SCHICK_DAT(1818493, "SKJAL.LTX");
		SCHICK_DAT(1820159, "PREM.LTX");
		SCHICK_DAT(1821794, "DASPOTA.LTX");
		SCHICK_DAT(1822227, "RYBON.LTX");
		SCHICK_DAT(1822633, "LJASDAHL.LTX");
		SCHICK_DAT(1823441, "VARNHEIM.LTX");
		SCHICK_DAT(1825636, "VAERMHAG.LTX");
		SCHICK_DAT(1827155, "TYLDON.LTX");
		SCHICK_DAT(1827819, "VIDSAND.LTX");
		SCHICK_DAT(1829215, "BRENDHIL.LTX");
		SCHICK_DAT(1830397, "MANRIN.LTX");
		SCHICK_DAT(1831863, "F_TJOILA.LTX");
		SCHICK_DAT(1833274, "F_ANGBOD.LTX");
		SCHICK_DAT(1834455, "HJALLAND.LTX");
		SCHICK_DAT(1834553, "L_RUNIN.LTX");
		SCHICK_DAT(1834683, "TAVERN.TLK");
		SCHICK_DAT(1853952, "SPSTAR.NVF");
		SCHICK_DAT(1857024, "GUERTEL.NVF");
		SCHICK_DAT(1924411, "HAFEN.LTX");
		SCHICK_DAT(1927597, "FACE.NVF");
		SCHICK_DAT(1986713, "HYGBACK.NVF");
		SCHICK_DAT(2039756, "HYGGELIK.NVF");
		SCHICK_DAT(2092734, "SKULL.NVF");
		SCHICK_DAT(2156928, "OUTRO1.NVF");
		SCHICK_DAT(2170377, "OUTRO2.NVF");
		SCHICK_DAT(2182194, "OUTRO3.NVF");
		SCHICK_DAT(2188739, "TOWNPAL.DAT");
		SCHICK_DAT(2189027, "THORWAL.XMI");
		SCHICK_DAT(2204899, "SUMMARY.XMI");
		SCHICK_DAT(2213631, "TEMPLE.XMI");
		SCHICK_DAT(2222833, "TERMS.XMI");
		SCHICK_DAT(2238239, "SMITH.XMI");
		SCHICK_DAT(2252751, "INN.XMI");
		SCHICK_DAT(2268581, "HEALER.XMI");
		SCHICK_DAT(2271971, "CAMP.XMI");
		SCHICK_DAT(2282897, "VICTORY.XMI");
		SCHICK_DAT(2289587, "SAMPLE.AD");
		SCHICK_DAT(2293029, "POPUP.DAT");
		SCHICK_DAT(2293285, "F092.TLK");
		SCHICK_DAT(2301287, "HERBERG.TLK");
		SCHICK_DAT(2309878, "DASPOTA1.TLK");
		SCHICK_DAT(2319506, "DASPOTA2.TLK");
		SCHICK_DAT(2329021, "DASPOTA3.TLK");
		SCHICK_DAT(2333387, "INFO1.TLK");
		SCHICK_DAT(2341290, "INFO2.TLK");
		SCHICK_DAT(2352007, "INFO3.TLK");
		SCHICK_DAT(2361430, "INFO4.TLK");
		SCHICK_DAT(2371737, "INFO5.TLK");
		SCHICK_DAT(2382033, "INFO6.TLK");
		SCHICK_DAT(2391901, "INFO7.TLK");
		SCHICK_DAT(2398866, "SCHMIED.TLK");
		SCHICK_DAT(2411390, "GHANDEL.TLK");
		SCHICK_DAT(2424111, "KHANDEL.TLK");
		SCHICK_DAT(2429605, "WHANDEL.TLK");
		SCHICK_DAT(2444572, "F115.TLK");
		SCHICK_DAT(2456744, "EREMIT.TLK");
		SCHICK_DAT(2460770, "HYGGELIK.TLK");
		SCHICK_DAT(2464022, "WEAPONS.NVF");
		SCHICK_DAT(2470505, "BUCH.DAT");
		SCHICK_DAT(2486281, "SPELLOBJ.NVF");
		SCHICK_DAT(2490216, "IN_HEADS.NVF");
		SCHICK_DAT(2562920, "TEMPICON");
		SCHICK_DAT(2569119, "SHIPSL.NVF");
		SCHICK_DAT(2724864, "STONESL.NVF");
		SCHICK_DAT(2913163, "MARBLESL.NVF");
		SCHICK_DAT(3072303, "TFLOOR1.NVF");
		SCHICK_DAT(3075704, "TDIVERSE.NVF");
		SCHICK_DAT(3098953, "HOUSE1.NVF");
		SCHICK_DAT(3156393, "HOUSE2.NVF");
		SCHICK_DAT(3210497, "HOUSE3.NVF");
		SCHICK_DAT(3267600, "HOUSE4.NVF");
		SCHICK_DAT(3322982, "FEATURE.LTX");
		SCHICK_DAT(3326324, "FEATURE1.LTX");
		SCHICK_DAT(3334903, "FEATURE2.LTX");
		SCHICK_DAT(3344514, "FEATURE3.LTX");
		SCHICK_DAT(3354076, "FEATURE4.LTX");
		SCHICK_DAT(3364043, "FEATURE5.LTX");
		SCHICK_DAT(3372180, "FEATURE6.LTX");
		SCHICK_DAT(3382253, "FEATURE7.LTX");
		SCHICK_DAT(3391296, "FEATURE8.LTX");
		SCHICK_DAT(3399701, "FIGHTOBJ.NVF");
		SCHICK_DAT(3416620, "SCENARIO.LST");
		SCHICK_DAT(3572493, "KDLBACK.DAT");
		SCHICK_DAT(3583581, "KCLBACK.DAT");
		SCHICK_DAT(3597993, "KLLBACK.DAT");
		SCHICK_DAT(3612133, "KSLBACK.DAT");
		SCHICK_DAT(3623457, "FIGHT.LST");
		SCHICK_DAT(3679187, "SKARTE.NVF");
		SCHICK_DAT(3699980, "GAMES.NAM");
		SCHICK_DAT(3700025, "ITEMNAME");
		SCHICK_DAT(3703888, "MONNAMES");
		SCHICK_DAT(3704817, "THORWAL1.LTX");
		SCHICK_DAT(3715197, "THORWAL2.LTX");
		SCHICK_DAT(3723760, "DUNGEON.XMI");
		SCHICK_DAT(3727290, "COMBAT.XMI");
		SCHICK_DAT(3738486, "PLAYM_US");
		SCHICK_DAT(3749158, "ZUSTA_US");
		SCHICK_DAT(3754502, "ANI.DAT");
		SCHICK_DAT(3764353, "WEAPANI.DAT");
		SCHICK_DAT(3765723, "MONSTER.DAT");
		SCHICK_DAT(3769199, "BSKILLS.DAT");
		SCHICK_DAT(3770499, "ITEMS.DAT");
		SCHICK_DAT(3773559, "CHARTEXT.LTX");
		SCHICK_DAT(3778097, "SPELLTXT.LTX");
		SCHICK_DAT(3784746, "WONDER.LTX");
		SCHICK_DAT(3786503, "STRASSE.LTX");
		SCHICK_DAT(3799961, "NSC.LTX");
		SCHICK_DAT(3806604, "NARIELL.NPC");
		SCHICK_DAT(3808358, "HARIKA.NPC");
		SCHICK_DAT(3810112, "CURIAN.NPC");
		SCHICK_DAT(3811866, "ARDORA.NPC");
		SCHICK_DAT(3813620, "GARSVIK.NPC");
		SCHICK_DAT(3815374, "ERWO.NPC");
		SCHICK_DAT(3817128, "FINGER.NVF");
		SCHICK_DAT(3838673, "LTURM.NVF");
		SCHICK_DAT(3860206, "WFIGS");
		SCHICK_DAT(4416951, "WFIGS.TAB");
		SCHICK_DAT(4417127, "SPLASHES.DAT");
		SCHICK_DAT(4418080, "FEATURE9.LTX");
		SCHICK_DAT(4426163, "PHEX1.LTX");
		SCHICK_DAT(4434880, "PHEX2.LTX");
		SCHICK_DAT(4444297, "WILD.LTX");
		SCHICK_DAT(4447751, "WAFFINFO.LTX");
		SCHICK_DAT(4456803, "SHIP.DNG");
		SCHICK_DAT(4458723, "F046.DNG");
		SCHICK_DAT(4459363, "F051.DNG");
		SCHICK_DAT(4460003, "F061.DNG");
		SCHICK_DAT(4460323, "F076.DNG");
		SCHICK_DAT(4460643, "F094.DNG");
		SCHICK_DAT(4461283, "F100.DNG");
		SCHICK_DAT(4462243, "F108.DNG");
		SCHICK_DAT(4462563, "F126.DNG");
		SCHICK_DAT(4463523, "F129.DNG");
		SCHICK_DAT(4464483, "F131.DNG");
		SCHICK_DAT(4464803, "OBER.DNG");
		SCHICK_DAT(4465443, "PREM.DNG");
		SCHICK_DAT(4465763, "THORWAL.DNG");
		SCHICK_DAT(4467363, "FINAL.DNG");
		SCHICK_DAT(4468643, "SHIP.DDT");
		SCHICK_DAT(4469157, "F046.DDT");
		SCHICK_DAT(4469607, "F051.DDT");
		SCHICK_DAT(4470062, "F061.DDT");
		SCHICK_DAT(4470145, "F076.DDT");
		SCHICK_DAT(4470256, "F094.DDT");
		SCHICK_DAT(4470474, "F100.DDT");
		SCHICK_DAT(4470705, "F108.DDT");
		SCHICK_DAT(4470888, "F126.DDT");
		SCHICK_DAT(4471329, "F129.DDT");
		SCHICK_DAT(4471728, "F131.DDT");
		SCHICK_DAT(4471974, "OBER.DDT");
		SCHICK_DAT(4472153, "PREM.DDT");
		SCHICK_DAT(4472249, "THORWAL.DDT");
		SCHICK_DAT(4472811, "FINAL.DDT");
		SCHICK_DAT(4473143, "SHIP.DTX");
		SCHICK_DAT(4476641, "F046.DTX");
		SCHICK_DAT(4481353, "F051.DTX");
		SCHICK_DAT(4484123, "F061.DTX");
		SCHICK_DAT(4486413, "F076.DTX");
		SCHICK_DAT(4487905, "F094.DTX");
		SCHICK_DAT(4492006, "F100.DTX");
		SCHICK_DAT(4493573, "F108.DTX");
		SCHICK_DAT(4495843, "F126.DTX");
		SCHICK_DAT(4500897, "F129.DTX");
		SCHICK_DAT(4504782, "F131.DTX");
		SCHICK_DAT(4507318, "OBER.DTX");
		SCHICK_DAT(4510301, "PREM.DTX");
		SCHICK_DAT(4512992, "THORWAL.DTX");
		SCHICK_DAT(4521157, "FINAL.DTX");
		SCHICK_DAT(4526605, "TFLOOR2.NVF");
		SCHICK_DAT(4556370, "STORY.LTX");
		SCHICK_DAT(4560939, "FX1.VOC");
		SCHICK_DAT(4565093, "FX2.VOC");
		SCHICK_DAT(4572097, "FX3.VOC");
		SCHICK_DAT(4575544, "FX4.VOC");
		SCHICK_DAT(4577034, "FX5.VOC");
		SCHICK_DAT(4578524, "FX6.VOC");
		SCHICK_DAT(4580014, "FX7.VOC");
		SCHICK_DAT(4594811, "FX8.VOC");
		SCHICK_DAT(4601552, "FX9.VOC");
		SCHICK_DAT(4617450, "FX10.VOC");
		SCHICK_DAT(4637364, "FX11.VOC");
		SCHICK_DAT(4657064, "FX12.VOC");
		SCHICK_DAT(4662241, "FX13.VOC");
		SCHICK_DAT(4675202, "FX14.VOC");
		SCHICK_DAT(4693359, "FX15.VOC");
		SCHICK_DAT(4710771, "FX16.VOC");
		SCHICK_DAT(4729888, "FX17.VOC");
		SCHICK_DAT(4749663, "FX18.VOC");
		default:
			     strcpy(file,"");
	}
	D1_LOG("Seek File\tHandle %x\tPos %ld\tType %x\t%s\n",
			handle, pos, type, file);
}

const char* names_attrib[] = {	"MU", "KL", "CH", "FF", "GE", "IN", "KK",
				"AG", "HA", "RA", "GG", "TA", "NG", "JZ"};

const char* names_skill[] = {
    "Waffenlos", "Hiebwaffen", "Stichwaffen", "Schwerter", "Äxte", "Speere", "Zweihänder", "Schusswaffen", "Wurfwaffen",
    "Akrobatik", "Klettern", "Körperbeh.", "Reiten", "Schleichen", "Schwimmen", "Selbstbeh.", "Tanzen", "Verstecken", "Zechen",
    "Bekehren", "Betören", "Feilschen", "Gassenwissen", "Lügen", "Menschenkenntnis", "Schätzen",
    "Fährtensuchen", "Fesseln", "Orientierung", "Pflanzenkunde", "Tierkunde", "Wildnisleben",
    "Alchimie", "Alte Sprachen", "Geographie", "Geschichte", "Götter/Kulte", "Kriegskunst", "Lesen", "Magiekunde", "Sprachen",
    "Abrichten", "Fahrzeuge", "Falschspiel", "Heilen Gift", "Heilen Krankheit", "Heilen Wunden", "Musizieren", "Schlösser", "Taschendieb",
    "Gefahrensinn", "Sinnenschärfe"
};
static const char* arr_zaub[] = {
    "DUMMY",
    "Beherrschung brechen", "Destructibo", "Gardianum", "Illusionen zerstören", "Verwandlung beenden", // Antimagie
    "Band & Fessel", "Bannbaladin", "Böser Blick", "Große Gier", "Große Verwirrung", "Herr der Tiere", "Horriphobus", "Mag. Raub", "Respondami", "Sanftmut", "Somnigravis", "Zwingtanz", // Beherrschung
    "Furor Blut", "Geister bannen", "Geister rufen", "Heptagon", "Krähenruf", "Skelettarius", // Dämonologie
    "Elementar herbeirufen", "Nihilatio Gravitas", "Solidrid Farbenspiel", // Elementarmagie
    "Axxeleraus", "Foramen", "Motoricus", "Spurlos, Trittlos", "Transversalis", "Über Eis", // Bewegung
    "Balsam", "Hexenspeichel", "Klarum Purum", "Ruhe Körper", "Tiere heilen", // Heilung
    "Adleraug", "Analüs", "Eigenschaften", "Exposami", "Odem Arcanum", "Penetrizzel", "Sensibar", // Hellsicht
    "Chamaelioni", "Duplicatus", "Harmlos", "Hexenknoten", // Illusion
    "Blitz", "Ecliptifactus", "Eisenrost", "Fulminictus", "Ignifaxius", "Plumbumbarum", "Radau", "Saft, Kraft, Monstermacht", "Scharfes Auge", // Kampf
    "Hexenblick", "Nekropathia", // Verständigung
    "Adler, Wolf", "Arcano Psychostabilis", "Armatrutz", "CH steigern", "Feuerbann", "FF steigern", "GE steigern", "IN steigern", "KK steigern", "KL steigern", "MU steigern", "Mutabili", "Paralü", "Salander", "See & Fluss", "Visibili", // Verwandlung
    "Abvenenum", "Aeolitus", "Brenne", "Claudibus", "Dunkelheit", "Erstarre", "Flim Flam", "Schmelze", "Silentium", "Sturmgebrüll" // Veränderung
};

static inline Bit8u* schick_getCharname(unsigned p) {
    return MemBase+Real2Phys(p)+16;
}

static inline Bit8u* getString(unsigned p) {
	return MemBase+Real2Phys(p);
}

// Intercept far CALLs (both 32 and 16 bit)
int schick_farcall_v302(unsigned segm, unsigned offs, unsigned ss)
{
	/* There are no farcalls from the game to segments behind DS*/
	if (segm >= datseg - relocation) return 0;

	//this is for mouse handling and spams the log
	if (segm == 0xb2a)	{
		//D1_LOG("Segment 0xb2a:0x%04x\n", offs);
		return 0;
	}

	if (segm == 0xf18)	{
		if (offs == 0x8) {
			D1_GFX("SwapU16(val=0x%x);\n",
					real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x14) {
			D1_GFX("SetVideoMode(mode=0x%x);\n",
					real_readw(ss, reg_sp));
				return 0;
		}
		if (offs == 0x2a) {
			D1_GFX("SetDisplayPage(page=0x%x);\n",
				real_readw(ss, reg_sp));
				return 0;
		}
		if (offs == 0x40) {
			D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
				real_readw(ss, reg_sp+2), real_readw(ss, reg_sp));
				return 0;
		}
		if (offs == 0xea) {
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short off=real_readw(ss, reg_sp);

			D1_GFX("SetColor(rgb=0x%x:0x%x, color=0x%x);\n",
				seg, off, real_readw(ss, reg_sp+4));
			D1_GFX("RGB=(0x%x, 0x%x, 0x%x);\n",
				real_readb(seg, off), real_readb(seg, off+1),
				real_readb(seg, off+2));

				return 0;
		}
		if (offs == 0x119) {

			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short first_color=real_readw(ss, reg_sp+4);
			unsigned short colors=real_readw(ss, reg_sp+6);
			unsigned short i;

			D1_GFX("SetPalette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
				seg, off, first_color, colors);

			if (seg == datseg)
				D1_LOG("Palette at DS:0x%x\n", off);
			for (i=0; i<colors; i++)
				D1_GFX("\"\\%02d\\%02d\\%02d\"..\n",
					real_readb(seg, off+i*3),
					real_readb(seg, off+i*3+1),
					real_readb(seg, off+i*3+2));
			return 0;
		}
		if (offs == 0x14d) {
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short len=real_readw(ss, reg_sp+4);
			unsigned short color=real_readw(ss, reg_sp+6);

			if (seg == 0xa000)
				D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
					off%320, off/320, len, color);
			else
				D1_GFX("HLine(0x%04x:0x%04x,len=%u,color=0x%02x);\n",
					seg, off, len, color);
			return 0;
		}
		if (offs == 0x1af) {
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short cnt=real_readw(ss, reg_sp+4);
			unsigned char color=real_readb(ss, reg_sp+6);
			unsigned short space=real_readw(ss, reg_sp+8);

			if (seg == 0xa000)
				D1_GFX("HSpacedDots(X=%03d,Y=%03u,%03u,0x%02x,%u);\n",
					off%320, off/320, cnt, color, space);
			else
				D1_GFX("HSpacedDots(0x%04x:0x%04x,0x%04x,0x%02x);\n",
					seg, off, cnt, color, space);
			return 0;
		}
		if (offs == 0x219) {
			unsigned short off_dest=real_readw(ss, reg_sp);
			unsigned short seg_dest=real_readw(ss, reg_sp+2);
			unsigned short x1=real_readw(ss, reg_sp+4);
			unsigned short y1=real_readw(ss, reg_sp+6);
			unsigned short x2=real_readw(ss, reg_sp+8);
			unsigned short y2=real_readw(ss, reg_sp+10);

			unsigned short val12=real_readw(ss, reg_sp+12);
			unsigned short val14=real_readw(ss, reg_sp+14);
			unsigned short val16=real_readw(ss, reg_sp+16);
			unsigned short val18=real_readw(ss, reg_sp+18);

			unsigned short width=real_readw(ss, reg_sp+20);
			unsigned short height=real_readw(ss, reg_sp+22);
			unsigned short off_src=real_readw(ss, reg_sp+24);
			unsigned short seg_src=real_readw(ss, reg_sp+26);

			if (seg_dest == 0xa000)
				D1_GFX("PicCopy(X=%03u,Y=%03u,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x)\n",
					off_dest%320, off_dest/320,
					x1, y1,	x2, y2,
					val12, val14, val16, val18,
					width, height, seg_src, off_src);
			else
				D1_GFX("PicCopy(dest=0x%04x:0x%04x,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x)\n",
					seg_dest, off_dest,
					x1, y1,	x2, y2,
					val12, val14, val16, val18,
					width, height, seg_src, off_src);
			return 0;
		}
		if (offs == 0x655) {
			unsigned short seg_src=real_readw(ss, reg_sp);
			unsigned short off_src=real_readw(ss, reg_sp+2);
			unsigned short off_dst=real_readw(ss, reg_sp+4);
			unsigned short seg_dst=real_readw(ss, reg_sp+6);
			unsigned short width=real_readw(ss, reg_sp+8);
			unsigned short height=real_readw(ss, reg_sp+10);

			if (seg_src == 0xa000)
			D1_GFX("SaveScreen(X=%u,Y=%u,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
					off_src%320, off_src/320,
					seg_dst, off_dst,
					width, height);
			else
			D1_GFX("SaveScreen(src=0x%04x:0x%04x,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
					seg_src, off_src,
					seg_dst, off_dst,
					width, height);
			return 0;
		}
		if (offs == 0x68c) {
			unsigned short seg=real_readw(ss, reg_sp);
			unsigned short off=real_readw(ss, reg_sp+2);
			unsigned short color=real_readw(ss, reg_sp+4);
			unsigned short width=real_readw(ss, reg_sp+6);
			unsigned short height=real_readw(ss, reg_sp+8);

			if (seg == 0xa000)

				D1_GFX("FillRect(X=%u,Y=%u,color=%u,width=%u,height=%u)\n",
					off%320, off/320, color, width, height);
			else
				D1_GFX("FillRect(dest=0x%04x:0x%04x,color=%u,cnt=%u,%u)\n",
					seg, off, color, width, height);
			return 0;
		}
		if (offs == 0x6c5) {
			unsigned short off_dst=real_readw(ss, reg_sp);
			unsigned short seg_dst=real_readw(ss, reg_sp+2);
			unsigned short off_src=real_readw(ss, reg_sp+4);
			unsigned short seg_src=real_readw(ss, reg_sp+6);
			unsigned short val1=real_readw(ss, reg_sp+8);
			unsigned short val2=real_readw(ss, reg_sp+10);
			unsigned short val3=real_readw(ss, reg_sp+12);
			unsigned short val4=real_readw(ss, reg_sp+16);

			D1_GFX("0x6c5(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,%u,%u,%u,%u)\n",
					seg_dst, off_dst,
					seg_src, off_src,
					val1, val2, val3, val4);
			return 0;
		}
		/* used often in cities and dungeons */
		if (offs == 0x816) {
			unsigned short off_dst=real_readw(ss, reg_sp);
			unsigned short seg_dst=real_readw(ss, reg_sp+2);
			unsigned short off_src=real_readw(ss, reg_sp+4);
			unsigned short seg_src=real_readw(ss, reg_sp+6);
			unsigned short val1=real_readw(ss, reg_sp+8);
			unsigned short val2=real_readw(ss, reg_sp+10);
			unsigned short val3=real_readw(ss, reg_sp+12);
			unsigned short val4=real_readw(ss, reg_sp+14);

			D1_GFX("0x816(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,%u,%u,%u,%u)\n",
					seg_dst, off_dst,
					seg_src, off_src,
					val1, val2, val3, val4
					);
			return 0;
		}
		if (offs == 0x967){
			D1_GFX("RLE(width=%d, height=%d, src_pp=0x%x:0x%x, dst=0x%x:0x%x, src_rle=0x%x:0x%x, mode=%d)\n",
				real_readw(ss, reg_sp), real_readw(ss, reg_sp+2),
				real_readw(ss, reg_sp+6), real_readw(ss, reg_sp+4),
				real_readw(ss, reg_sp+10), real_readw(ss, reg_sp+8),
				real_readw(ss, reg_sp+14),real_readw(ss, reg_sp+12),
				real_readw(ss, reg_sp+16));
				return 0;
		}
		D1_GFX("Rasterlib:0x%x\n", offs);
		return 0;
	}

	if (segm == 0x4ac) return 0;

	//4 funcs of this sement are called every 0.18s and spam the log
	if (segm == 0x51e) {
		if (offs == 0x0017) return 0;
		if (offs == 0x0209) return 0;
		if (offs == 0x06fe) return 0;
		/* GUI Radio */
		if (offs == 0x0832) return 0;
		/* Betrunken */
		if (offs == 0x0856) return 0;
		if (offs == 0x0c0e) {
			short index = CPU_Pop16();
			CPU_Push16(index);

			unsigned int retval = get_readlength2(index);
			D1_LOG("get_readlength2(%d) = %d\n", index, retval);

			reg_ax = retval & 0xffff;
			reg_dx = (retval>>16) & 0xffff;

			return 1;
		}
		if (offs == 0x0c28) {
			unsigned short index=real_readw(ss, reg_sp);
			D1_LOG("ReadDatfile()\n");
			return 0;
		}
		if (offs == 0x0c72) return 0;
		if (offs == 0x0cb6) return 0;
		if (offs == 0x0d27) {
			unsigned short index=real_readw(ss, reg_sp);
			D1_LOG("OpenAndSeekDatfile(%u)\n", index);
			return 0;
		}
		if (offs == 0x0ed2) return 0;
		/* Leaf Function */
		if (offs == 0x1634) {
			unsigned short v1 = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			unsigned short v3 = CPU_Pop16();
			unsigned short v4 = CPU_Pop16();
			CPU_Push16(v4);
			CPU_Push16(v3);
			CPU_Push16(v2);
			CPU_Push16(v1);

			D1_INFO("???(%d, %d, %d, %d);\n", v1, v2, v3, v4);
			D1_INFO("ds:299c = %d\n", real_readw(ds, 0x299c));
			D1_INFO("ds:299e = %d\n", real_readw(ds, 0x299e));
			return 0;
		}
		if (offs == 0x16fd) return 0;
		if (offs == 0x1802) return 0;
		/* Leaf Function */
		if (offs == 0x18b3) return 0;
		if (offs == 0x1921) return 0;
		if (offs == 0x192b) return 0;
		if (offs == 0x1a34) return 0;
		if (offs == 0x1d67) return 0;
		if (offs == 0x1ecc) return 0;
		if (offs == 0x21ab) return 0;
		if (offs == 0x232a) return 0;
		if (offs == 0x25ce) return 0;
		if (offs == 0x274e) return 0;
		/* Wunder Rondra: Starker Schwertarm */
		if (offs == 0x2e26) return 0;
		/* Wunder Rondra: Starker Schwertarm  Leaf Function */
		if (offs == 0x2e69) return 0;
		/* Leaf Function - near&far */
		if (offs == 0x2f7a) return 0;
		if (offs == 0x3071) return 0;
		if (offs == 0x3230) return 0;
		if (offs == 0x37c4) return 0;
		if (offs == 0x3b4f) return 0;
		if (offs == 0x3ca6) return 0;
		/* Schiffsfahrt */
		if (offs == 0x3dbb) return 0;
		if (offs == 0x3ebb) return 0;
		/* Kopierschutzabfrage */
		if (offs == 0x4016) return 0;
		if (offs == 0x404f) return 0;
		if (offs == 0x40d1) return 0;
		if (offs == 0x41cd) return 0;
		if (offs == 0x43e7) {
			D1_LOG("set_to_ff()\n");
			set_to_ff();
			return 1;
		}
		if (offs == 0x43fd) return 0;
		if (offs == 0x4485) {
			short val = CPU_Pop16();
			CPU_Push16(val);

			reg_ax = mod_timer(val);

			D1_INFO("mod_timer(%d) = %d\n", val, reg_eax);

			return 1;
		}
		if (offs == 0x44aa) return 0;
		if (offs == 0x4559) {

			reg_ax = can_merge_group();
			D1_INFO("can_merge_group() = %d\n", (short)reg_ax);

			return 1;
		}
		if (offs == 0x45db) {
			short val = CPU_Pop16();
			CPU_Push16(val);

			D1_LOG("div16(%d)\n", val);

			reg_ax = div16(val);

			return 1;
		}
		/* Leaf Function - far only */
		if (offs == 0x45ea) return 0;
		/* Leaf Function - far only */
		if (offs == 0x4658) return 0;
		/* Leaf Function - far & near */
		if (offs == 0x4707) return 0;
		/* No Leaf - far only, calls only 0x4707 */
		if (offs == 0x472b) return 0;

		if (offs == 0x48b1) {
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = check_hero(MemBase + Real2Phys(hero));

			D1_LOG("check_hero(%s) = %d\n",
				schick_getCharname(hero), reg_ax);

			return 1;
		}
		if (offs == 0x49d8) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_INFO("istHeldBeiSinnenUndGruppe(%s)\n", schick_getCharname(ptr));
			return 0;
		}

		if (offs == 0x4a05) return 0;
		if (offs == 0x4a87) {
			RealPt hero = CPU_Pop32();
			short val = CPU_Pop16();
			CPU_Push16(val);
			CPU_Push32(hero);

			add_hero_ae(MemBase + Real2Phys(hero), val);

			D1_LOG("add_hero_ae(%s, %d)\n",
					schick_getCharname(hero), val);

			return 1;
		}
		if (offs == 0x4adc) return 0;
		/* Wunder TSA heilt ganze Gruppe 6x */
		if (offs == 0x4df3) return 0;
		if (offs == 0x4ff9) {
			/* Eigenschaftsprobe */
			RealPt hero = CPU_Pop32();
			unsigned attrib = CPU_Pop16();
			signed bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(attrib);
			CPU_Push32(hero);

			reg_ax = test_attrib(MemBase + Real2Phys(hero),
					attrib, bonus);

			return 1;
		}
		if (offs == 0x504e) { // Talent-/Zauber-Probe
			unsigned p0 = CPU_Pop32();
			unsigned p1 = CPU_Pop16();
			unsigned p2 = CPU_Pop16();
			unsigned p3 = CPU_Pop16();
			unsigned p4 = CPU_Pop16();
			CPU_Push16(p4);
			CPU_Push16(p3);
			CPU_Push16(p2);
			CPU_Push16(p1);
			CPU_Push32(p0);
			signed char p4_r = p4 & 0xFF;
			D1_INFO("->(%s/%s/%s) %+d:",
					names_attrib[p1], names_attrib[p2],
					names_attrib[p3], p4_r);
			supress_rnd=3;
			return 0;
		}
		if (offs == 0x515e) return 0;
		if (offs == 0x51c2) {
			unsigned int money;

			money = get_party_money();
			D1_LOG("get_party_money() = %d\n", money);

			reg_ax = money & 0xffff;
			reg_dx = (money>>16) & 0xffff;

			return 1;
		}
		if (offs == 0x5221) return 0;
		if (offs == 0x5331) return 0;
		if (offs == 0x5349) {
			RealPt hero = CPU_Pop32();
			int ap = CPU_Pop32();
			CPU_Push32(ap);
			CPU_Push32(hero);

			D1_INFO("%s erhält %d AP\n",
						schick_getCharname(hero), ap);
			add_hero_ap(MemBase+Real2Phys(hero), ap);

			return 1;
		}
		/* APs verteilen */
		if (offs == 0x535f) {
			int group_ap = CPU_Pop32();
			CPU_Push32(group_ap);

			D1_INFO("Gruppe erhält %d AP\n", group_ap);

			return 0;
		}
		/* Krakenangriff */
		if (offs == 0x53e8) return 0;
		/* Essen & Trinken */
		if (offs == 0x54e9) {
			/* unsigned short get_hero_index(hero_ptr *hero); */
			RealPt hero = CPU_Pop32();
			CPU_Push32(hero);

			reg_ax = get_hero_index(MemBase+Real2Phys(hero));
			D1_LOG("get_hero_index(%s) = (%d)\n",
					schick_getCharname(hero), reg_ax);
			return 1;
		}
		if (offs == 0x5520) {
			/* int get_item_pos(hero_ptr *hero, unsigned short item)*/
			RealPt hero = CPU_Pop32();
			unsigned short item = CPU_Pop16();
			CPU_Push16(item);
			CPU_Push32(hero);

			reg_ax = get_item_pos(MemBase + Real2Phys(hero), item);

			D1_LOG("get_item_pos(%s, 0x%04x) = %d\n",
						schick_getCharname(hero),
						item, (short)reg_ax);
			return 1;
		}
		/* Leaf Function - far only */
		if (offs == 0x554c) return 0;
		/* Leaf Function - near only */
		if (offs == 0x55b1) return 0;
		/* Krakenangriff */
		if (offs == 0x5615) return 0;
		if (offs == 0x5667) return 0;
		/* Tür einschlagen */
		if (offs == 0x56d6) return 0;
		if (offs == 0x5799) {

			reg_ax = count_heroes_available_in_group();
			D1_LOG("count_heroes_available_in_group() = %d\n",
				reg_ax);

			return 1;
		}
		if (offs == 0x5816) {
			unsigned short argc=real_readw(ss, reg_sp);
			D1_TRAC("main(argc=0x%04x, ...)\n", argc);
			return 0;
		}
		if (offs == 0x5a68) {
			D1_LOG("alloc_byte(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		D1_LOG("Segment 0x51e:0x%04x\n", offs);
		return 0;
	}
	if (segm == 0x0ae7) {
		if (offs == 0x000c) {
			unsigned short mod = CPU_Pop16();
			CPU_Push16(mod);

			D1_LOG("update_direction(%d)\n", mod);

			reg_ax = update_direction(mod);
			return 1;
		}
		return 0;
	}
	if (segm == 0x0c85) return 0;
	if (segm == 0x0e41) return 0;
	if (segm == 0x0ef8) {

		if (offs == 0x000b) {
			signed lo = CPU_Pop16();
			signed hi = CPU_Pop16();
			CPU_Push16(hi);
			CPU_Push16(lo);

			reg_ax = random_interval(lo, hi);

			D1_INFO("randomInterval %d - %d : %d\n",
				lo, hi, reg_ax);

			return 1;
		}
		if (offs == 0x002b) {
			unsigned p1 = CPU_Pop16();
			CPU_Push16(p1);

			if (supress_rnd == 0)
				D1_INFO("random(%d) =", p1);
			else
				supress_rnd--;
			call++;
			return 0;
		}
		if (offs == 0x007a) {
			unsigned n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			unsigned x = CPU_Pop16();
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			reg_ax = dice_roll(n, m, x);

			D1_INFO("wuerfel %dW%d%+d = %d\n",
				n, m, x, (short)reg_ax);

			return 1;
		}
		if (offs == 0x00c4) {
			unsigned val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_word_array(val, MemBase + Real2Phys(p));

			D1_LOG("is_in_word_array(0x%x, 0x%04x:0x%04x) = %d\n",
				val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00ef) {
			unsigned val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_byte_array((char)val, MemBase+Real2Phys(p));
			D1_LOG("is_in_byte_array(0x%x, 0x%04x:0x%04x) = %d\n",
				val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00a0) {
			signed n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			signed x = CPU_Pop16();
			RealPt pmin = CPU_Pop32();
			RealPt pmax = CPU_Pop32();
			CPU_Push32(pmax);
			CPU_Push32(pmin);
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			calc_damage_range(n, m, x, MemBase+Real2Phys(pmin),
				MemBase+Real2Phys(pmax));

			D1_LOG("calc_damage_range(%d, %d, %d)\n", n, m, x);

			return 1;
		}
		if (offs == 0x0119) {
		        unsigned val = CPU_Pop16();
		        CPU_Push16(val);

			unsigned short m = ((val & 0x0f00) >> 8) - 1;
			if (m > 3)
				m = 3;

			m = dice_tab[m];

			reg_ax = dice_template(val);

		        D1_INFO("Wuerfel %dW%d%+d = %d\n",
				(val & 0xf000) >> 12, m,
				(char)(val & 0xff), (short)reg_ax);

			return 1;
		}
		if (offs == 0x0186) {
			unsigned short val = CPU_Pop16();
			RealPt min = CPU_Pop32();
			RealPt max = CPU_Pop32();
			CPU_Push32(max);
			CPU_Push32(min);
			CPU_Push16(val);

			damage_range_template(val,MemBase + Real2Phys(min),
				MemBase + Real2Phys(max));

			D1_LOG("damage_range_template() Untested\n");

			return 1;
		}
		D1_ERR("Uncatched call to Segment seg007:0x%04x\n", offs);
		exit(1);
		return 0;
	}

	if (segm == 0x0ff1) return 0;
	if (segm == 0x1030) return 0;
	/* No overlay */
	if (segm == 0x1042) return 0;
	if (segm == 0x1112) return 0;

	if (segm == 0x12db) return 0;
	if (segm == 0x12de) return 0;

	/* All functions make an output here, but they aren't complete */
	if (segm == 0x12e5) {
		D1_LOG("Segment 0x12e5\t");
		if (offs == 0x0020) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0025) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x002a) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x002f) {
			D1_LOG("short ChooseLoadSavegame(void)\n");
			return 0;
		}
		if (offs == 0x0034) {
			D1_LOG("ip=0x%04X ChooseSaveSavegame()\n", offs);
			return 0;
		}
		if (offs == 0x0039) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x003e) {
			D1_LOG("ip=0x%4X ReleaseHero()\n", offs);
			return 0;
		}
		if (offs == 0x0043) {
			D1_LOG("ip=0x%4X ChooseFreeHero()\n", offs);
			return 0;
		}
		if (offs == 0x0048) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x004d) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0052) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0057) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x005c) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0061) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0066) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		return 0;
	}
	if (segm == 0x12ec) {
		if (offs = 0x0025) {
			short ani = CPU_Pop16();
			CPU_Push16(ani);

			D1_LOG("ANI %02d angefordert\n", ani);
			return 0;
		}
		return 0;
	}
	if (segm == 0x12f1) return 0;
	if (segm == 0x12f9) return 0;
	/* Spielstand und Zeit */
	if (segm == 0x12ff) return 0;
	if (segm == 0x1303) return 0;
	if (segm == 0x1309) return 0;
	if (segm == 0x130f) return 0;
	/* Kampf */
	if (segm == 0x1312) return 0;
	/* Kampf Loot + AP*/
	if (segm == 0x1316) return 0;
	/* Kampf */
	if (segm == 0x131a) return 0;
	/* Kampf */
	if (segm == 0x131f) return 0;
	if (segm == 0x1324) return 0;
	/* Kampf */
	if (segm == 0x1328) return 0;
	/* Kampf */
	if (segm == 0x132d) return 0;
	/* Werte Prüfen */
	if (segm == 0x1330) return 0;
	if (segm == 0x1335) return 0;
	/* Kampf */
	if (segm == 0x1338) return 0;
	/* Kampf */
	if (segm == 0x133b) return 0;
	/* Kampf */
	if (segm == 0x133f) return 0;
	/* Zustand */
	if (segm == 0x1344) return 0;
	/* Wächter aussuchen */
	if (segm == 0x1348) return 0;
	if (segm == 0x1350) return 0;
	if (segm == 0x1353) return 0;
	/* Steigern */
	if (segm == 0x1358) return 0;
	/*Vorräte auffüllen */
	if (segm == 0x135c) return 0;
	if (segm == 0x135f) return 0;
	/* Heiler stub053 */
	if (segm == 0x1362) {
		if (offs == 0x20) {
			unsigned short typi = real_readb(datseg, 0x4224);
			char v1 = real_readb(datseg, typi * 2 + 0x66ea);
			char v2 = real_readb(datseg, typi * 2 + 0x66ea + 1);
			D1_INFO("Heiler: 0x%02x Rabatt: %d%% Qualität: %d\n",
								typi, v1, v2);
			return 0;
		}
		return 0;
	}

	if (segm == 0x1365) return 0;
	if (segm == 0x1369) return 0;
	/* Waren kaufen */
	if (segm == 0x136d) return 0;
	/* Waren verkaufen */
	if (segm == 0x1370) return 0;
	/* Dialog mit SCHMIED */
	if (segm == 0x1373) return 0;
	if (segm == 0x1377) return 0;
	if (segm == 0x137b) return 0;
	/* Held Löschen*/
	if (segm == 0x137e) return 0;
	/* Wunder erbitten */
	if (segm == 0x1383) return 0;
	if (segm == 0x1386) return 0;
	/* Hafen */
	if (segm == 0x138a) return 0;
	if (segm == 0x1392) return 0;
	/* Zufallsnachrichten */
	if (segm == 0x139a) return 0;
	/* Magierakademie + Stoerrebrandt */
	if (segm == 0x13a1) return 0;
	if (segm == 0x13a8) return 0;
	if (segm == 0x13b4) return 0;
	/* */
	if (segm == 0x13b9)  {

		if (offs == 0x20) {
			char city=real_readb(datseg, 0x2d67);
			unsigned char ww=real_readb(datseg, 0x7c9d+city);
			D1_LOG("Merkwürdige Funktion\n");
			D1_LOG("Stadt: 0x%02x\t WW: 0x%02x\n", city, ww);
			return 0;
		}
		if (offs == 0x25) return 0;
		if (offs == 0x2a) return 0;
		if (offs == 0x2f) return 0;

		return 0;
	}
	/* Automap */
	if (segm == 0x13bd) return 0;
	/* Dungeon betreten */
	if (segm == 0x13c3) return 0;
	if (segm == 0x13cb) return 0;
	/* Kampf "Verfallene Herberge" */
	if (segm == 0x13d7) return 0;
	if (segm == 0x13e4) return 0;
	if (segm == 0x13e9) return 0;
	if (segm == 0x1401) return 0;
	if (segm == 0x1408) return 0;
	/* Kampf */
	if (segm == 0x140b) return 0;
	if (segm == 0x1417) return 0;
	if (segm == 0x1420) return 0;
	if (segm == 0x1429) return 0;
	if (segm == 0x142c) return 0;
	if (segm == 0x1432) return 0;
	if (segm == 0x1438) {
		if (offs == 0x0052) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("drawString(%s)\n", getString(ptr));
			return 0;
		}
		if (offs == 0x007f) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("drawLocName(%s)\n", getString(ptr));
			return 0;
		}
		return 0;
	}
	/* stub097 */
	if (segm == 0x1442) {
		if (offs == 0x0039) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("dialog_schmal(%s)\n", getString(ptr));
			return 0;
		}
		return 0;
	}
	if (segm == 0x1449) {
		if (offs == 0x0020) {
			D1_LOG("Menu: Magie Anwenden\n");
			return 0;
		}
		return 0;
	}
	if (segm == 0x145e) return 0;
	if (segm == 0x1467) return 0;
	/* Kampf Gegner zaubert */
	if (segm == 0x1472) return 0;
	/* Talent anwenden*/
	if (segm == 0x147b) {
		if (offs == 0x0020) { // Talentprobe
			unsigned p0 = CPU_Pop32();
			unsigned p1 = CPU_Pop16();
			unsigned p2 = CPU_Pop16();
			CPU_Push16(p2);
			CPU_Push16(p1);
			CPU_Push32(p0);

			signed char p2_r = p2 & 0xFF;
			D1_INFO("Talentprobe %s: %s %+d ",
						schick_getCharname(p0),
						names_skill[p1], p2_r);
			return 0;
		}
		if (offs == 0x0025) return 0;
		D1_INFO("\t\tTalent:0x%x\n", offs);
		return 0;
	}

	if (segm == 0x1480) return 0;
	if (segm == 0x1485) return 0;
	if (segm == 0x148c) return 0;
	if (segm == 0x1491) return 0;
	/* Essen */
	if (segm == 0x1498) return 0;
	if (segm == 0x149b) return 0;

	if (segm == 0x14d1) return 0;
	if (segm == 0x14d8) return 0;
	if (segm == 0x14e0) return 0;
	if (segm == 0x14e7) return 0;
	if (segm == 0x14ed) return 0;
	if (segm == 0x14f0) return 0;
	if (segm == 0x14f6) return 0;
	if (segm == 0x14f9) return 0;

	/* Borland C++ runtime */
	if (segm == 0x0) {
		/* nullsub */
		if (offs == 0x2c9) return 0;
		if (offs == 0x2f7) {
			/* int chdir(const char* __path) */
			RealPt path = CPU_Pop32();
			CPU_Push32(path);

			D1_LOG("chdir(%s)\n", (char*)MemBase+Real2Phys(path));

			return 0;
		}
		/* close() */
		if (offs == 0x31b) return 0;
		/* open() */
		if (offs == 0x61e) return 0;
		/* read() */
		if (offs == 0x654) return 0;
		if (offs == 0x678) return 0;

		if (offs == 0x6d0) {
			D1_LOG("C-Lib exit(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x6df) {//Not Called
			D1_LOG("_exit(%d)\n", real_readw(ss, reg_sp));
			return 0;	}

		if (offs == 0x70b){
			D1_LOG("Mul unsigned long\n");
			return 0;	}

		if (offs == 0x722) return 0;
		/* getcurdir() */
		if (offs == 0x73e) return 0;
		/* getdisk() */
		if (offs == 0x781) return 0;
		/* setdisk() */
		if (offs == 0x79b) return 0;
		/* dos_getdiskfree() */
		if (offs == 0x7ed) return 0;
		if (offs == 0x816) {
			D1_TRAC("_dos_getvect(int=0x%x)\n", real_readw(ss,reg_sp));
			return 0;
		}
		if (offs == 0x0825) {
			unsigned short interruptno = CPU_Pop16();
			RealPt isr = CPU_Pop32();
			CPU_Push32(isr);
			CPU_Push16(interruptno);

			D1_TRAC("_dos_setvect(int=0x%x, *isr=0x%x:0x%x)\n",
				interruptno, RealSeg(isr) - relocation,
				RealOff(isr));
			return 0;
		}
		if (offs == 0x839) return 0;
		if (offs == 0x840) {
			D1_LOG("Div unsigned long\n");
			return 0;
		}
		if (offs == 0x848) return 0;
		if (offs == 0x850) { //not called
			D1_LOG("Mod unsigned long\n");
			return 0;
		}
		if (offs == 0x8e7) return 0;
		/* rshift() */
		if (offs == 0x908) return 0;
		if (offs == 0x928) return 0;
		if (offs == 0x9b0) return 0;
		if (offs == 0xa10) return 0;
		if (offs == 0xb33) {
			/*seek()*/
			D1_LOG("__seek(Handle=0x%x, pos=%u, Mode=%d)\n",
			real_readw(ss, reg_sp), real_readw(ss, reg_sp+4)<<16+
			real_readw(ss, reg_sp+2), real_readw(ss, reg_sp+6));
			return 0;
		}
		/* mkdir() */
		if (offs == 0xb5c) return 0;
		if (offs == 0xbac) {
			unsigned short val=real_readw(ss, reg_sp);
			D1_TRAC("C-Lib srand(%d)\n", val);
			return 0;
		}
		if (offs == 0xbbd) {
			D1_LOG("rand()\n");
			return 0;
		}
		if (offs == 0x0be3) {
			/*read()*/
			D1_LOG("_read(fd=0x%x, buffer=0x%x:0x%x, len=%d)\n",
			real_readw(ss, reg_sp), real_readw(ss, reg_sp+4),
			real_readw(ss, reg_sp+2), real_readw(ss, reg_sp+6));
			return 0;
		}
		if (offs == 0x1123) {
			/* time(), user for randomize */
			unsigned short hi=real_readw(ss, reg_sp);
			unsigned short lo=real_readw(ss, reg_sp+2);
			D1_TRAC("C-Lib time(0x%04x)\n", hi<<16+lo);
			return 0;
		}
		if (offs == 0x117b) return 0;
		/* delete() */
		if (offs == 0x11a7) return 0;
		if (offs == 0x176d) return 0;
		if (offs == 0x1792) return 0;
		if (offs == 0x1e55) {
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			D1_LOG("free(0x%04x:0x%04x)\n", seg, off);
			return 0;		}
		if (offs == 0x1f69) {
			unsigned short lo=real_readw(ss, reg_sp);
			unsigned short hi=real_readw(ss, reg_sp+2);
			D1_LOG("farmalloc(%d)\n", hi<<16+lo);
			return 0;		}
		if (offs == 0x2287) {
			unsigned short nl=real_readw(ss, reg_sp);
			unsigned short nh=real_readw(ss, reg_sp+2);
			unsigned short lo=real_readw(ss, reg_sp+4);
			unsigned short hi=real_readw(ss, reg_sp+6);

			D1_LOG("calloc(%d, 0x%x)\n",
					(nh<<16)+nl, (hi<<16)+lo);
			return 0;
		}
		if (offs == 0x2315) return 0;
		if (offs == 0x2411) return 0;
		if (offs == 0x2596) return 0;
		if (offs == 0x2d82) {
			unsigned short i=4;
			unsigned short off=real_readw(ss, reg_sp+i);
			unsigned short seg=real_readw(ss, reg_sp+i+2);

			D1_LOG("C-Lib exec?(\"%s\", ",
					MemBase+(seg<<4)+off);
			/*
			do {
				i+=4;
				off=real_readw(ss, reg_sp+i);
				seg=real_readw(ss, reg_sp+i+2);
				if ((seg<<4)+off > 0)
						D1_LOG("\"%s\", ",
						MemBase+(seg<<4)+off);
					else
						D1_LOG("\"%s\")\n",
						MemBase+(seg<<4)+off);
			} while ((seg<<4)+off > 0);
			*/
			return 0;
		}
		if (offs == 0x2dff) {
			/* long atol(const char* s) */
			int val;
			RealPt s = CPU_Pop32();
			CPU_Push32(s);

			D1_LOG("atol(\"%s\") = ", getString(s));

			val = atoi((char*)getString(s));
			D1_LOG("%d\n", val);

			reg_ax = val & 0xffff;
			reg_dx = (val>>16) & 0xffff;

			return 1;
		}
		if (offs == 0x2eb2) {
			D1_LOG("C-Lib close(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x2eda) return 0;
		if (offs == 0x2f25) return 0;
		if (offs == 0x3040) return 0;
		if (offs == 0x3073) return 0;
		if (offs == 0x30a0) return 0;
		if (offs == 0x3350) {
			/* char* itoa(int __value, char* string, int radix);
			radix is everytime 10 in this game*/

			short value = CPU_Pop16();
			RealPt string = CPU_Pop32();
			short radix = CPU_Pop16();
			CPU_Push16(radix);
			CPU_Push32(string);
			CPU_Push16(value);

			D1_LOG("itoa(%d, 0x%04x:0x%04x, %d)\n",
					value, RealSeg(string),
					RealOff(string), radix);
			sprintf((char*)MemBase+Real2Phys(string), "%d", value);

			reg_ax = RealOff(string);
			reg_dx = RealSeg(string);
			return 1;
		}
		if (offs == 0x33c0) {
			/*void *memcpy(void *dest, const void *src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("memcpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			memcpy(MemBase + Real2Phys(dest), MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x3408) {
			/*void* memset(void *s, int c, size_t n)*/
			RealPt s = CPU_Pop32();
			short c = CPU_Pop16();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push16(c);
			CPU_Push32(s);

			D1_LOG("memset(0x%04x:0x%04x, 0x%02x, %u)\n",
					RealSeg(s), RealOff(s), c, n);
			// No bypass on graphic memory
			if (RealSeg(s) == 0xa000)
				return 0;

			memset(MemBase+Real2Phys(s), c, n);

			reg_ax = RealOff(s);
			reg_dx = RealSeg(s);
			return 1;
		}
		if (offs == 0x3479) {
			/* write(handle) */
			unsigned short handle=real_readw(ss, reg_sp);
			D1_LOG("write_0(%d)\n", handle);
			return 0;
		}
		if (offs == 0x34c7) {
			/*open()*/
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short mode=real_readw(ss, reg_sp+4);

			D1_LOG("open(\"%s\",\"%04x\")\n",
					MemBase+(seg<<4)+off, mode);
			return 0;
		}
		if (offs == 0x3636) {
			/* sortof open() */
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short mode=real_readw(ss, reg_sp+4);

			D1_LOG("C-Lib Unkn(\"%s\", 0x%04x)\n",
					MemBase+(seg<<4)+off, mode);
			return 0;
		}
		if (offs == 0x36dd) {
			/*printf()*/
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);

			D1_LOG("printf(\"%s\")\n", MemBase+(seg<<4)+off);
			return 0; }
		if (offs == 0x3d74) return 0;
			/* ret 0x000a */
		if (offs == 0x41d2) {
			unsigned short	o1=real_readw(ss, reg_sp);
			unsigned short	s1=real_readw(ss, reg_sp+2);
			unsigned short	o2=real_readw(ss, reg_sp+4);
			unsigned short	s2=real_readw(ss, reg_sp+6);
			unsigned short	o3=real_readw(ss, reg_sp+8);
			unsigned short	s3=real_readw(ss, reg_sp+10);
			unsigned short	o4=real_readw(ss, reg_sp+12);
			unsigned short	s4=real_readw(ss, reg_sp+14);
			D1_LOG("C-Lib sprintf(0x%04x:0x%04x, \"%s\", 0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					s1, o1, MemBase+(s2<<4)+o2,
					s3, o3, s4, o4);

			return 0;
		}
		if (offs == 0x4215) {
			/*char *strcat(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcat(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcat((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x4254) {
			/*int strcmp(const char* s1, const char* s2)*/
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("strcmp(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(s1), RealOff(s1),
					RealSeg(s2), RealSeg(s2));

			reg_ax = strcmp((char*)MemBase + Real2Phys(s1),
					(char*)MemBase + Real2Phys(s2));

			return 1;
		}
		if (offs == 0x4284) {
			/*char *strcpy(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcpy(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x42ad) {
			/*strlen()*/
			RealPt str = CPU_Pop32();
			CPU_Push32(str);

			D1_LOG("strlen(\"%s\")\n", getString(str));
			reg_ax = strlen((char*)getString(str));

			return 1;
		}
		if (offs == 0x42cc) {
			/*char *strncpy(char* dest, const char* src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strncpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			strncpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x462b) return 0;
		if (offs == 0x4a85) {
			/*write()*/
			unsigned short handle=real_readw(ss, reg_sp);
			unsigned short off=real_readw(ss, reg_sp+2);
			unsigned short seg=real_readw(ss, reg_sp+4);
			unsigned short val=real_readw(ss, reg_sp+6);
			D1_LOG("C-Lib __write(Handle=0x%x, Buffer=0x%x:0x%x, Len=%d)\n", handle, seg ,off, val);
			return 0;
		}
		if (offs == 0x4a88) return 0;

		D1_LOG("\t\tC-Lib:0x%x\n", offs);
		return 0;
	}
	D1_TRAC("Unfetched Segment: 0x%04x\n", segm);
	return 0;
}

int schick_callf(unsigned selector, unsigned offs, unsigned ss)
{
	if (!running || !(dbg_mode & 2)) return 0;

	unsigned short segm = selector - relocation;
	int ret = 0;

	if (schick && !fromgame) {
		ret = schick_farcall_v302(segm, offs, ss);
		return ret;
	}

	if (gen) {
		ret = schick_farcall_gen105(segm, offs, ss);
		return ret;
	}

	return ret;
}

// Intercept far JMPs (both 32 and 16 bit)
void schick_jmpf(unsigned selector,unsigned offs,unsigned oldeip) {

    if (!running || !schick || !(dbg_mode & 2) ) return;

    if (offs == 0x0E1F) { // Zauberprobe
	unsigned pIP= CPU_Pop32();
	unsigned p0 = CPU_Pop32();
	unsigned p1 = CPU_Pop16();
	unsigned p2 = CPU_Pop16();
	CPU_Push16(p2);
	CPU_Push16(p1);
	CPU_Push32(p0);
	CPU_Push32(pIP);
	signed char p2_r = p2 & 0xFF;
	D1_INFO("Zauberprobe %s: %s %+d ", schick_getCharname(p0), arr_zaub[p1], p2_r);
	return;
    }
}

// Intercept RETurn and print the return value. Simple hack.
// Works only correct for Routines containing no unintercepted CALLs
void schick_ret() {

    if (!running || !schick || !(dbg_mode & 2) || !call) return;

    if (supress_rnd) D1_INFO(" %d", reg_ax);
    else             D1_INFO(" %d\n", reg_ax);
    call--;
}

// Intercept near CALLs, 16-Bit
void schick_calln16(unsigned un1) {

    if (!running || !schick || !(dbg_mode & 2)) return;

    /* TODO: Das Segment kann hier seltsamerweise wechseln.
     * Für die Zauberprobe z.B. habe ich in der verfallenen Herberge einen Aufruf
     * im Segment 0x2572 (der kühle Raum) und einen von 0x272E (Zauber aus dem
     * Charakterbildschirm) gekriegt.
     */
    unsigned short segm = SegValue(cs)-relocation;
    unsigned short offs = reg_ip+(signed short)un1;

    if (offs == 0x040F) { // Talentprobe
	unsigned pIP= CPU_Pop32();
	unsigned p0 = CPU_Pop32();
	unsigned p1 = CPU_Pop16();
	unsigned p2 = CPU_Pop16();
	CPU_Push16(p2);
	CPU_Push16(p1);
	CPU_Push32(p0);
	CPU_Push32(pIP);
	signed char p2_r = p2 & 0xFF;
	D1_INFO("Talentprobe %s: %s %+d ",
	       schick_getCharname(p0), names_skill[p1], p2_r);
	return;
    }
    if (offs == 0x0E1F) { // Zauberprobe
	unsigned pIP= CPU_Pop32();
	unsigned p0 = CPU_Pop32();
	unsigned p1 = CPU_Pop16();
	unsigned p2 = CPU_Pop16();
	CPU_Push16(p2);
	CPU_Push16(p1);
	CPU_Push32(p0);
	CPU_Push32(pIP);
	signed char p2_r = p2 & 0xFF;
	D1_INFO("Zauberprobe %s: %s %+d ",
	       schick_getCharname(p0), arr_zaub[p1], p2_r);
	return;
    }
    if (offs == 0x0386) { // Unbekannte Probefunktion
	D1_INFO("?-Probe[0x%06x:0x%06x] ", segm, reg_ip);
	return;
    }
}
