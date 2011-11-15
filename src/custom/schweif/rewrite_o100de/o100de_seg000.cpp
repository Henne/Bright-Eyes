#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"

#include "schweif.h"

#include "o100de_seg000.h"

namespace schweif_o100de {

Bit32s bc_lseek(Bit16u handle, Bit32u offset, Bit16s whence) {

	ds_writew(0x895C + handle * 2, ds_readw(0x895C + handle * 2) & 0xfdff);

	if (!DOS_SeekFile(handle, &offset, whence))
		return -1;

	return offset;

}

Bit16s bc__read(Bit16u handle, Bit8u *buf, Bit16u count) {

	if ((ds_readw(0x895C + handle * 2) & 2))
		return -1;

	if (!DOS_ReadFile(handle, buf, &count))
		return -1;

	return (Bit16s)count;
}

signed short bioskey(signed short cmd) {
	reg_ah = cmd &  0xff;
	reg_al = 0;
	CALLBACK_RunRealInt(0x16);

	if (GETFLAG(ZF)) {
		if (!(cmd & 1))
			return reg_ax;

		return 0;
	} else {
		if (!(cmd & 1))
			return reg_ax;
		if (reg_ax)
			return reg_ax;
		return -1;
	}
}

Bit16s bc_close(Bit16u handle) {

	if (handle >= ds_readw(0xb786))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return bc__close(handle);

}

Bit16s bc__close(Bit16u handle) {

	if (!DOS_CloseFile(handle))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return 0;
}


int seg000(unsigned short offs) {
    switch(offs) {
    case 0x1E91: { // sub_11E91
	return 0;
    }
    case 0x1EB4: { // sub_11EB4
	return 0;
    }
    case 0x1EBE: { // sub_11EBE
	return 0;
    }
    case 0x1F3B: { // sub_11F3B
	return 0;
    }
    case 0x1FB7: { // sub_11FB7
	return 0;
    }
    case 0x2022: { // sub_12022
	return 0;
    }
    case 0x202B: { // sub_1202B
	return 0;
    }
    case 0x209C: { // sub_1209C
	return 0;
    }
    case 0x2112: { // sub_12112
	return 0;
    }
    case 0x2151: { // sub_12151
	return 0;
    }
    case 0x21DC: { // sub_121DC
	return 0;
    }
    case 0x226A: { // sub_1226A
	return 0;
    }
    case 0x22BA: { // sub_122BA
	return 0;
    }
    case 0x24DC: { // sub_124DC
	return 0;
    }
    case 0x25B1: { // sub_125B1
	return 0;
    }
    case 0x25FA: { // sub_125FA
	return 0;
    }
    case 0x2650: { // sub_12650
	return 0;
    }
    case 0x26D7: { // sub_126D7
	return 0;
    }
    case 0x2770: { // sub_12770
	return 0;
    }
    case 0x279E: { // sub_1279E
	return 0;
    }
    case 0x2839: { // sub_12839
	return 0;
    }
    case 0x28D6: { // sub_128D6
	return 0;
    }
    case 0x28ED: { // sub_128ED
	return 0;
    }
    case 0x28F1: { // sub_128F1
	return 0;
    }
    case 0x2929: { // sub_12929
	return 0;
    }
    case 0x295D: { // sub_1295D
	return 0;
    }
    case 0x2A27: { // sub_12A27
	return 0;
    }
    case 0x0FBF: { // sub_10FBF
	return 0;
    }
    case 0x1257: { // sub_11257
	return 0;
    }
    case 0x125E: { // sub_1125E
	return 0;
    }
    case 0x1267: { // sub_11267
	return 0;
    }
    case 0x126F: { // sub_1126F
	return 0;
    }
    case 0x1289: { // sub_11289
	return 0;
    }
    case 0x1296: { // sub_11296
	return 0;
    }
    case 0x129F: { // sub_1129F
	return 0;
    }
    case 0x1732: { // sub_11732
	return 0;
    }
    case 0x1757: { // sub_11757
	return 0;
    }
    case 0x1824: { // sub_11824
	return 0;
    }
    case 0x184D: { // sub_1184D
	return 0;
    }
    case 0x1899: { // sub_11899
	return 0;
    }
    case 0x19CB: { // sub_119CB
	return 0;
    }
    case 0x19E7: { // sub_119E7
	return 0;
    }
    case 0x1A09: { // sub_11A09
	return 0;
    }
    case 0x1A36: { // sub_11A36
	return 0;
    }
    case 0x1AEC: { // sub_11AEC
	return 0;
    }
    case 0x1BAE: { // sub_11BAE
	return 0;
    }
    case 0x1BCE: { // sub_11BCE
	return 0;
    }
    case 0x1C09: { // sub_11C09
	return 0;
    }
    case 0x1C76: { // sub_11C76
	return 0;
    }
    case 0x1CD9: { // sub_11CD9
	return 0;
    }
    case 0x1D4A: { // sub_11D4A
	return 0;
    }
    case 0x1D73: { // sub_11D73
	return 0;
    }
    case 0x1DAA: { // sub_11DAA
	return 0;
    }
    case 0x1DD3: { // sub_11DD3
	return 0;
    }
    case 0x1E37: { // sub_11E37
	return 0;
    }
    case 0x394F: { // sub_1394F
	return 0;
    }
    case 0x3B52: { // sub_13B52
	return 0;
    }
    case 0x3BAA: { // sub_13BAA
	return 0;
    }
    case 0x3BD8: { // sub_13BD8
	return 0;
    }
    case 0x3E5E: { // sub_13E5E
	return 0;
    }
    case 0x3E86: { // sub_13E86
	return 0;
    }
    case 0x3F57: { // sub_13F57
	return 0;
    }
    case 0x3F5A: { // sub_13F5A
	return 0;
    }
    case 0x40B7: { // sprintf
	RealPt dst_p = CPU_Pop32();
	Bit16u format_ofs = CPU_Pop16();
	Bit16u format_seg = CPU_Pop16();
	Bit16u args_ofs   = CPU_Pop16();
	Bit16u args_seg   = CPU_Pop16();
	CPU_Push16(args_seg);
	CPU_Push16(args_ofs);
	CPU_Push16(format_seg);
	CPU_Push16(format_ofs);
	CPU_Push32(dst_p);
	char* dest   = (char*)(MemBase + Real2Phys(dst_p));
	char* format = (char*)(MemBase + PhysMake(format_seg, format_ofs));
	char* args_p = (char*)(MemBase + PhysMake(  args_seg,   args_ofs));
	
	// TODO: Find out how Borland-sprintf saves the variadric argument list.
	//       Transform this so the host version of sprintf can be used.
	//D2_LOG("sprintf: %s with '%s'\n", format, args_p);
	return 0;
    }
    case 0x40FA: { // strcat
	RealPt dest   = CPU_Pop32();
	RealPt source = CPU_Pop32();
	CPU_Push32(source);
	CPU_Push32(dest);
	//D2_LOG("strcat(%08x, %s)\n", dest, MemBase+Real2Phys(source));
	strcat((char*)(MemBase + Real2Phys(dest)),  (char*)(MemBase + Real2Phys(source)));
	return 1;
    }
    case 0x4139: { // sub_14139
	return 0;
    }
    case 0x4176: { // sub_14176
	return 0;
    }
    case 0x41A6: { // sub_141A6
	return 0;
    }
    case 0x41CF: { // sub_141CF
	return 0;
    }
    case 0x41EE: { // sub_141EE
	return 0;
    }
    case 0x4226: { // sub_14226
	return 0;
    }
    case 0x4257: { // sub_14257
	return 0;
    }
    case 0x429B: { // sub_1429B
	return 0;
    }
    case 0x4309: { // sub_14309
	return 0;
    }
    case 0x4402: { // sub_14402
	return 0;
    }
    case 0x44E6: { // sub_144E6
	return 0;
    }
    case 0x450A: { // sub_1450A
	return 0;
    }
    case 0x4634: { // sub_14634
	return 0;
    }
    case 0x4838: { // sub_14838
	return 0;
    }
    case 0x4A26: { // sub_14A26
	return 0;
    }
    case 0x4B05: { // sub_14B05
	return 0;
    }
    case 0x4C92: { // sub_14C92
	return 0;
    }
    case 0x2A63: { // sub_12A63
	return 0;
    }
    case 0x2AB8: { // sub_12AB8
	return 0;
    }
    case 0x2ADA: { // sub_12ADA
	return 0;
    }
    case 0x2CDB: { // sub_12CDB
	return 0;
    }
    case 0x2D71: { // sub_12D71
	return 0;
    }
    case 0x2DE5: { // sub_12DE5
	return 0;
    }
    case 0x2E11: { // sub_12E11
	return 0;
    }
    case 0x2E18: { // sub_12E18
	return 0;
    }
    case 0x2E44: { // sub_12E44
	return 0;
    }
    case 0x2EE7: { // sub_12EE7
	return 0;
    }
    case 0x2F48: { // sub_12F48
	return 0;
    }
    case 0x2FBD: { // sub_12FBD
	return 0;
    }
    case 0x2FEA: { // sub_12FEA
	return 0;
    }
    case 0x3038: { // sub_13038
	return 0;
    }
    case 0x31A6: { // sub_131A6
	return 0;
    }
    case 0x31E8: { // sub_131E8
	return 0;
    }
    case 0x326A: { // sub_1326A
	return 0;
    }
    case 0x33CA: { // sub_133CA
	return 0;
    }
    case 0x343A: { // sub_1343A
	return 0;
    }
    case 0x34AA: { // sub_134AA
	return 0;
    }
    case 0x34CE: { // sub_134CE
	return 0;
    }
    case 0x34F2: { // sub_134F2
	return 0;
    }
    case 0x3511: { // sub_13511
	return 0;
    }
    case 0x3563: { // sub_13563
	return 0;
    }
    case 0x35B1: { // sub_135B1
	return 0;
    }
    case 0x376F: { // sub_1376F
	return 0;
    }
    case 0x378A: { // sub_1378A
	return 0;
    }
    case 0x37A6: { // sub_137A6
	return 0;
    }
    case 0x04E9: { // DOS_GetFAT
	return 0;
    }
    case 0x049B: { // DOS_GetDrive2
	return 0;
    }
    case 0x035E: { // div_int
	return 0;
    }
    case 0x036A: { // DOS_GetDrive
	return 0;
    }
    case 0x0507: { // DOS_GetDiskSpace
	return 0;
    }
    case 0x07A1: { // DOS_GetDeviceInfo
	return 0;
    }
    case 0x0488: { // DOS_GetCurrentTime
	return 0;
    }
    case 0x0458: { // DOS_GetCurrentDir
	return 0;
    }
    case 0x0475: { // DOS_GetCurrentDate
	return 0;
    }
    case 0x3138: { // DOS_FindNext
	return 0;
    }
    case 0x3105: { // DOS_FindFirst
	return 0;
    }
    case 0x3165: { // DOS_FileLength
	return 0;
    }
    case 0x2ECA: { // DOS_FileAttributes
	return 0;
    }
    case 0x016B: { // DOS_Exit
	return 0;
    }
    case 0x0D2A: { // DOS_DeleteFile
	return 0;
    }
    case 0x3584: { // DOS_CreateFile2
	return 0;
    }
    case 0x2F2D: { // DOS_CreateFile
	return 0;
    }
    case 0x2F0F: { // DOS_CloseFile
	return 0;
    }
    case 0x02F7: { // DOS_Chdir
	return 0;
    }
    case 0x037C: { // DOS_ChangeDrive
	return 0;
    }
    case 0x053F: { // DOS_LoadIntVector
	return 0;
    }
    case 0x0899: { // DOS_Mkdir
	return 0;
    }
    case 0x3720: { // DOS_OpenFile
	return 0;
    }
    case 0x0920: { // DOS_ReadFile
	Bit16u handle = CPU_Pop16();
	RealPt buf = CPU_Pop32();
	Bit16u count = CPU_Pop16();
	CPU_Push16(count);
	CPU_Push32(buf);
	CPU_Push16(handle);
	
/*	D2_LOG("C-Lib _read(handle=%d, buffer=0x%x, len=%d)\n",
	       handle, buf, count);
*/
	reg_ax = bc__read(handle, MemBase + Real2Phys(buf), count);
	
	return 1;
    }
    case 0x0870: { // DOS_Seek
	Bit16u handle = CPU_Pop16();
	Bit32u pos = CPU_Pop32();
	Bit16u whence = CPU_Pop16();
	CPU_Push16(whence);
	CPU_Push32(pos);
	CPU_Push16(handle);
	
	Bit32s retval = bc_lseek(handle, pos, whence);

/*	D2_LOG("C-Lib lseek(Handle=0x%x, pos=%u, whence=%d, apos=%x)\n",
	       handle, pos, whence, ((handle*2)-0x76A4)&0xFDFF);
*/	
	reg_ax = retval & 0xffff;
	reg_dx = (retval >> 16) & 0xffff;
	
	return 1;
	return 0;
    }
    case 0x0CB8: { // DOS_SeekEOF
	return 0;
    }
    case 0x0B3C: { // DOS_SetCurrentDate
	return 0;
    }
    case 0x0B51: { // DOS_SetCurrentTime
	return 0;
    }
    case 0x4C56: { // DOS_WriteFile
	return 0;
    }
    case 0x359F: { // DOS_WriteFile_0?
	return 0;
    }
    case 0x2553: { // DOS_getch
	return 0;
    }
    case 0x17EB: { // INT_16h_keyb
	return 0;
    }
    case 0x1810: { // INT_17h_printer
	return 0;
    }
    case 0x01ED: { // Initialize
	return 0;
    }
    case 0x01C0: { // LoadIntVectors
	return 0;
    }
    case 0x017D: { // SaveIntVectors
	return 0;
    }
    case 0x1A44: { // _VideoInt
	return 0;
    }
    case 0x0338: { // DOS_LoadInt23h
	return 0;
    }
    case 0x27FE: { // DOS_INT21_IOCTL
	return 0;
    }
    case 0x0622: { // crunchmode2_helper
	return 0;
    }
    case 0x0530: { // DOS_GetIntVector
	return 0;
    }
    case 0x04B5: { // sub_104B5
	return 0;
    }
    case 0x0550: { // sub_10550
	return 0;
    }
    case 0x055F: { // sub_1055F
	return 0;
    }
    case 0x05FE: { // sub_105FE
	return 0;
    }
    case 0x0601: { // sub_10601
	return 0;
    }
    case 0x061F: { // sub_1061F
	return 0;
    }
    case 0x06A7: { // sub_106A7
	return 0;
    }
    case 0x06AA: { // subtract
	Bit32s a = (reg_dx << 16) | (reg_ax);
	Bit32s b = (reg_cx << 16) | (reg_bx);
	Bit32s c = a-b;
	//reg_dx = (c >> 16) & 0xFFFF;
	//reg_ax =  c        & 0xFFFF;
	//SETFLAGBIT(SF, (c<0)?1:0);
	//SETFLAGBIT(CF, 0);
	//D2_LOG("%08x - %08x = %08x\n", a, b, c);
	return 0;
    }
    case 0x0707: { // sub_10707
	return 0;
    }
    case 0x0735: { // sub_10735
	return 0;
    }
    case 0x0756: { // sub_10756
	return 0;
    }
    case 0x07B2: { // sub_107B2
	return 0;
    }
    case 0x0854: { // sub_10854
	return 0;
    }
    case 0x08B1: { // sub_108B1
	return 0;
    }
    case 0x08C8: { // sub_108C8
	return 0;
    }
    case 0x08E9: { // sub_108E9
	return 0;
    }
    case 0x08FA: { // sub_108FA
	return 0;
    }
    case 0x094F: { // sub_1094F
	return 0;
    }
    case 0x097A: { // sub_1097A
	return 0;
    }
    case 0x0B1C: { // sub_10B1C
	return 0;
    }
    case 0x0C60: { // sub_10C60
	return 0;
    }
    case 0x0CD2: { // sub_10CD2
	return 0;
    }
    case 0x0CFE: { // sub_10CFE
	return 0;
    }
    case 0x0D42: { // sub_10D42
	return 0;
    }
    case 0x0000: { // start
	return 0;
    }
    case 0x0157: { // sub_10157
	return 0;
    }
    case 0x0231: { // sub_10231
	return 0;
    }
    case 0x043C: { // sub_1043C
	return 0;
    }
    case 0x0425: { // sub_10425
	return 0;
    }
    case 0x03F9: { // sub_103F9
	return 0;
    }
    case 0x03EA: { // sub_103EA
	return 0;
    }
    case 0x0393: { // sub_10393
	return 0;
    }
    case 0x02CA: { // sub_102CA
	return 0;
    }
    case 0x0272: { // sub_10272
	return 0;
    }
    default: return 0;
    }
}

}
