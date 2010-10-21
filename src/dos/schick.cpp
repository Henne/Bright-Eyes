#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schick.h"
#include "cpu.h"

#define SCHICK_DAT(pos, name)	case pos: strcpy(file, name); break;

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
// Segment relocation
unsigned short relocation;

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
const char* names_spell[] = {
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

int schick_callf(unsigned selector, unsigned offs, unsigned ss)
{
	if (!running || !(dbg_mode & 2)) return 0;

	unsigned short segm = selector - relocation;
	int ret = 0;

	if (schick && !fromgame) {
		ret = schick_farcall_v302de(segm, offs, ss);
		return ret;
	}

	if (gen) {
		ret = schick_farcall_gen105(segm, offs, ss);
		return ret;
	}

	return ret;
}

// Intercept RETurn.
void schick_ret() {

	if (!running || !schick || !(dbg_mode & 2))
		return;
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
	unsigned pIP = CPU_Pop32();
	unsigned hero = CPU_Pop32();
	unsigned skill = CPU_Pop16();
	signed bonus = CPU_Pop16();
	CPU_Push16(bonus);
	CPU_Push16(skill);
	CPU_Push32(hero);
	CPU_Push32(pIP);

	D1_INFO("Talentprobe near : %s %+d ", names_skill[skill], (char)bonus);
	return;
    }
    if (offs == 0x0E1F) { // Zauberprobe
	unsigned pIP = CPU_Pop32();
	unsigned hero = CPU_Pop32();
	unsigned spell = CPU_Pop16();
	signed bonus = CPU_Pop16();
	CPU_Push16(bonus);
	CPU_Push16(spell);
	CPU_Push32(hero);
	CPU_Push32(pIP);

	D1_INFO("Zauberprobe near: %s %+d ", names_spell[spell], (char)bonus);
	return;
    }
    if (offs == 0x0386) { // Unbekannte Probefunktion
	D1_INFO("?-Probe[0x%06x:0x%06x] ", segm, reg_ip);
	return;
    }
}
