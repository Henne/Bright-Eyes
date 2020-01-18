#!/bin/sh

# Disassemble SCHICKM.EXE v3.02de into its code segments
# needs: nasm, python, head (coreutils)

# path to the game
GAMEDIR="./"
# filename of the SCHICKM.EXE
GAMEFILE="${GAMEDIR}/SCHICKM.EXE"
# output directory
OUTDIR=../temp/disasm_orig


# create the directory for the output
mkdir -p $OUTDIR

err=0
# check for the needed programs
for prog in ndisasm head python; do
	$prog --version >/dev/null 2>/dev/null
	if [ $? -eq 127 ]; then
		echo "ERROR: The program $prog is needed"
		err=1
	fi
done

if [ $err -eq 1 ]; then exit 1 ; fi

# check if the gamefile exists
if [ ! -e ${GAMEFILE} ]; then
	echo "ERROR: cant find SCHICKM.EXE in this directory"
	exit 1
fi

# generate a disassembly for each segment
ndisasm -b16 -e $((0x4ac0 + 0x2400)) ${GAMEFILE} | head -n 722 >${OUTDIR}/SEG001.dis
ndisasm -b16 -e $((0x51e2 + 0x2400)) ${GAMEFILE} | head -n 8807 >${OUTDIR}/SEG002.dis
ndisasm -b16 -e $((0xae7c + 0x2400)) ${GAMEFILE} | head -n 390 >${OUTDIR}/SEG003.dis
ndisasm -b16 -e $((0xb2a5 + 0x2400)) ${GAMEFILE} | head -n 2107 >${OUTDIR}/SEG004.dis
ndisasm -b16 -e $((0xc85b + 0x2400)) ${GAMEFILE} | head -n 2503 >${OUTDIR}/SEG005.dis
ndisasm -b16 -e $((0xe41e + 0x2400)) ${GAMEFILE} | head -n 991 >${OUTDIR}/SEG006.dis
ndisasm -b16 -e $((0xef8b + 0x2400)) ${GAMEFILE} | head -n 236 >${OUTDIR}/SEG007.dis
ndisasm -b16 -e $((0xf188 + 0x2400)) ${GAMEFILE} | head -n 1737 >${OUTDIR}/SEG008.dis
ndisasm -b16 -e $((0xff18 + 0x2400)) ${GAMEFILE} | head -n 576 >${OUTDIR}/SEG009.dis
ndisasm -b16 -e $((0x1030e + 0x2400)) ${GAMEFILE} | head -n 163 >${OUTDIR}/SEG010.dis
ndisasm -b16 -e $((0x10424 + 0x2400)) ${GAMEFILE} | head -n 1465 >${OUTDIR}/SEG011.dis

ndisasm -b16 -e $((0x24780 + 0x2400)) ${GAMEFILE} | head -n 350 >${OUTDIR}/SEG024.dis
ndisasm -b16 -e $((0x24be0 + 0x2400)) ${GAMEFILE} | head -n 1450 >${OUTDIR}/SEG025.dis
ndisasm -b16 -e $((0x25e90 + 0x2400)) ${GAMEFILE} | head -n 1772 >${OUTDIR}/SEG026.dis
ndisasm -b16 -e $((0x273f0 + 0x2400)) ${GAMEFILE} | head -n 1545 >${OUTDIR}/SEG027.dis
ndisasm -b16 -e $((0x28660 + 0x2400)) ${GAMEFILE} | head -n 1393 >${OUTDIR}/SEG028.dis
ndisasm -b16 -e $((0x29670 + 0x2400)) ${GAMEFILE} | head -n 718 >${OUTDIR}/SEG029.dis
ndisasm -b16 -e $((0x29ef0 + 0x2400)) ${GAMEFILE} | head -n 1823 >${OUTDIR}/SEG030.dis
ndisasm -b16 -e $((0x2b420 + 0x2400)) ${GAMEFILE} | head -n 713 >${OUTDIR}/SEG031.dis
ndisasm -b16 -e $((0x2bc50 + 0x2400)) ${GAMEFILE} | head -n 1750 >${OUTDIR}/SEG032.dis
ndisasm -b16 -e $((0x2d130 + 0x2400)) ${GAMEFILE} | head -n 1938 >${OUTDIR}/SEG033.dis
ndisasm -b16 -e $((0x2eae0 + 0x2400)) ${GAMEFILE} | head -n 1719 >${OUTDIR}/SEG034.dis
ndisasm -b16 -e $((0x2fec0 + 0x2400)) ${GAMEFILE} | head -n 690 >${OUTDIR}/SEG035.dis
ndisasm -b16 -e $((0x30680 + 0x2400)) ${GAMEFILE} | head -n 1767 >${OUTDIR}/SEG036.dis
ndisasm -b16 -e $((0x31a20 + 0x2400)) ${GAMEFILE} | head -n 1600 >${OUTDIR}/SEG037.dis
ndisasm -b16 -e $((0x32ba0 + 0x2400)) ${GAMEFILE} | head -n 1408 >${OUTDIR}/SEG038.dis
ndisasm -b16 -e $((0x33af0 + 0x2400)) ${GAMEFILE} | head -n 1115 >${OUTDIR}/SEG039.dis
ndisasm -b16 -e $((0x347a0 + 0x2400)) ${GAMEFILE} | head -n 552 >${OUTDIR}/SEG040.dis
ndisasm -b16 -e $((0x34ea0 + 0x2400)) ${GAMEFILE} | head -n 879 >${OUTDIR}/SEG041.dis
ndisasm -b16 -e $((0x358a0 + 0x2400)) ${GAMEFILE} | head -n 1697 >${OUTDIR}/SEG042.dis
ndisasm -b16 -e $((0x36d10 + 0x2400)) ${GAMEFILE} | head -n 1691 >${OUTDIR}/SEG043.dis
ndisasm -b16 -e $((0x38140 + 0x2400)) ${GAMEFILE} | head -n 1648 >${OUTDIR}/SEG044.dis
ndisasm -b16 -e $((0x39340 + 0x2400)) ${GAMEFILE} | head -n 432 >${OUTDIR}/SEG045.dis
ndisasm -b16 -e $((0x39800 + 0x2400)) ${GAMEFILE} | head -n 1847 >${OUTDIR}/SEG046.dis
ndisasm -b16 -e $((0x3ade0 + 0x2400)) ${GAMEFILE} | head -n 1054 >${OUTDIR}/SEG047.dis
ndisasm -b16 -e $((0x3b9c0 + 0x2400)) ${GAMEFILE} | head -n 1728 >${OUTDIR}/SEG048.dis
ndisasm -b16 -e $((0x3cf50 + 0x2400)) ${GAMEFILE} | head -n 1388 >${OUTDIR}/SEG049.dis
ndisasm -b16 -e $((0x3e0c0 + 0x2400)) ${GAMEFILE} | head -n 1671 >${OUTDIR}/SEG050.dis
ndisasm -b16 -e $((0x3f550 + 0x2400)) ${GAMEFILE} | head -n 1183 >${OUTDIR}/SEG051.dis
ndisasm -b16 -e $((0x404e0 + 0x2400)) ${GAMEFILE} | head -n 504 >${OUTDIR}/SEG052.dis
ndisasm -b16 -e $((0x40bd0 + 0x2400)) ${GAMEFILE} | head -n 733 >${OUTDIR}/SEG053.dis
ndisasm -b16 -e $((0x41560 + 0x2400)) ${GAMEFILE} | head -n 1088 >${OUTDIR}/SEG054.dis
ndisasm -b16 -e $((0x42390 + 0x2400)) ${GAMEFILE} | head -n 722 >${OUTDIR}/SEG055.dis
ndisasm -b16 -e $((0x42c40 + 0x2400)) ${GAMEFILE} | head -n 1614 >${OUTDIR}/SEG056.dis
ndisasm -b16 -e $((0x44050 + 0x2400)) ${GAMEFILE} | head -n 1595 >${OUTDIR}/SEG057.dis
ndisasm -b16 -e $((0x453c0 + 0x2400)) ${GAMEFILE} | head -n 1391 >${OUTDIR}/SEG058.dis
ndisasm -b16 -e $((0x46510 + 0x2400)) ${GAMEFILE} | head -n 531 >${OUTDIR}/SEG059.dis
ndisasm -b16 -e $((0x46c40 + 0x2400)) ${GAMEFILE} | head -n 1554 >${OUTDIR}/SEG060.dis
ndisasm -b16 -e $((0x47ee0 + 0x2400)) ${GAMEFILE} | head -n 1150 >${OUTDIR}/SEG061.dis
ndisasm -b16 -e $((0x48da0 + 0x2400)) ${GAMEFILE} | head -n 1242 >${OUTDIR}/SEG062.dis
ndisasm -b16 -e $((0x49e10 + 0x2400)) ${GAMEFILE} | head -n 1314 >${OUTDIR}/SEG063.dis
ndisasm -b16 -e $((0x4ae50 + 0x2400)) ${GAMEFILE} | head -n 485 >${OUTDIR}/SEG064.dis
ndisasm -b16 -e $((0x4b370 + 0x2400)) ${GAMEFILE} | head -n 1468 >${OUTDIR}/SEG065.dis
ndisasm -b16 -e $((0x4c590 + 0x2400)) ${GAMEFILE} | head -n 2274 >${OUTDIR}/SEG066.dis
ndisasm -b16 -e $((0x4def0 + 0x2400)) ${GAMEFILE} | head -n 1174 >${OUTDIR}/SEG067.dis
ndisasm -b16 -e $((0x4ed50 + 0x2400)) ${GAMEFILE} | head -n 1455 >${OUTDIR}/SEG068.dis
ndisasm -b16 -e $((0x50100 + 0x2400)) ${GAMEFILE} | head -n 622 >${OUTDIR}/SEG069.dis
ndisasm -b16 -e $((0x509c0 + 0x2400)) ${GAMEFILE} | head -n 1000 >${OUTDIR}/SEG070.dis
ndisasm -b16 -e $((0x51820 + 0x2400)) ${GAMEFILE} | head -n 1043 >${OUTDIR}/SEG071.dis
ndisasm -b16 -e $((0x52780 + 0x2400)) ${GAMEFILE} | head -n 1855 >${OUTDIR}/SEG072.dis
ndisasm -b16 -e $((0x53cc0 + 0x2400)) ${GAMEFILE} | head -n 807 >${OUTDIR}/SEG073.dis
ndisasm -b16 -e $((0x545f0 + 0x2400)) ${GAMEFILE} | head -n 1182 >${OUTDIR}/SEG074.dis
ndisasm -b16 -e $((0x55310 + 0x2400)) ${GAMEFILE} | head -n 2002 >${OUTDIR}/SEG075.dis
ndisasm -b16 -e $((0x56960 + 0x2400)) ${GAMEFILE} | head -n 1520 >${OUTDIR}/SEG076.dis
ndisasm -b16 -e $((0x57c80 + 0x2400)) ${GAMEFILE} | head -n 654 >${OUTDIR}/SEG077.dis
ndisasm -b16 -e $((0x58500 + 0x2400)) ${GAMEFILE} | head -n 1552 >${OUTDIR}/SEG078.dis
ndisasm -b16 -e $((0x59960 + 0x2400)) ${GAMEFILE} | head -n 1341 >${OUTDIR}/SEG079.dis
ndisasm -b16 -e $((0x5aaa0 + 0x2400)) ${GAMEFILE} | head -n 1061 >${OUTDIR}/SEG080.dis
ndisasm -b16 -e $((0x5b860 + 0x2400)) ${GAMEFILE} | head -n 1529 >${OUTDIR}/SEG081.dis
ndisasm -b16 -e $((0x5cc00 + 0x2400)) ${GAMEFILE} | head -n 680 >${OUTDIR}/SEG082.dis
ndisasm -b16 -e $((0x5d4f0 + 0x2400)) ${GAMEFILE} | head -n 1362 >${OUTDIR}/SEG083.dis
ndisasm -b16 -e $((0x5e670 + 0x2400)) ${GAMEFILE} | head -n 1456 >${OUTDIR}/SEG084.dis
ndisasm -b16 -e $((0x5f9c0 + 0x2400)) ${GAMEFILE} | head -n 1120 >${OUTDIR}/SEG085.dis
ndisasm -b16 -e $((0x608e0 + 0x2400)) ${GAMEFILE} | head -n 1029 >${OUTDIR}/SEG086.dis
ndisasm -b16 -e $((0x61660 + 0x2400)) ${GAMEFILE} | head -n 1734 >${OUTDIR}/SEG087.dis
ndisasm -b16 -e $((0x62db0 + 0x2400)) ${GAMEFILE} | head -n 461 >${OUTDIR}/SEG088.dis
ndisasm -b16 -e $((0x63370 + 0x2400)) ${GAMEFILE} | head -n 1656 >${OUTDIR}/SEG089.dis
ndisasm -b16 -e $((0x647a0 + 0x2400)) ${GAMEFILE} | head -n 910 >${OUTDIR}/SEG090.dis
ndisasm -b16 -e $((0x65390 + 0x2400)) ${GAMEFILE} | head -n 709 >${OUTDIR}/SEG091.dis
ndisasm -b16 -e $((0x65c40 + 0x2400)) ${GAMEFILE} | head -n 1221 >${OUTDIR}/SEG092.dis
ndisasm -b16 -e $((0x66a20 + 0x2400)) ${GAMEFILE} | head -n 539 >${OUTDIR}/SEG093.dis
ndisasm -b16 -e $((0x67110 + 0x2400)) ${GAMEFILE} | head -n 1495 >${OUTDIR}/SEG094.dis
ndisasm -b16 -e $((0x68260 + 0x2400)) ${GAMEFILE} | head -n 1041 >${OUTDIR}/SEG095.dis
ndisasm -b16 -e $((0x69020 + 0x2400)) ${GAMEFILE} | head -n 1156 >${OUTDIR}/SEG096.dis
ndisasm -b16 -e $((0x69bf0 + 0x2400)) ${GAMEFILE} | head -n 1450 >${OUTDIR}/SEG097.dis
ndisasm -b16 -e $((0x6acb0 + 0x2400)) ${GAMEFILE} | head -n 1628 >${OUTDIR}/SEG098.dis
ndisasm -b16 -e $((0x6c0e0 + 0x2400)) ${GAMEFILE} | head -n 1512 >${OUTDIR}/SEG099.dis
ndisasm -b16 -e $((0x6d3a0 + 0x2400)) ${GAMEFILE} | head -n 1472 >${OUTDIR}/SEG100.dis
ndisasm -b16 -e $((0x6e580 + 0x2400)) ${GAMEFILE} | head -n 1261 >${OUTDIR}/SEG101.dis
ndisasm -b16 -e $((0x6f5e0 + 0x2400)) ${GAMEFILE} | head -n 1157 >${OUTDIR}/SEG102.dis
ndisasm -b16 -e $((0x70340 + 0x2400)) ${GAMEFILE} | head -n 1606 >${OUTDIR}/SEG103.dis
ndisasm -b16 -e $((0x71720 + 0x2400)) ${GAMEFILE} | head -n 1231 >${OUTDIR}/SEG104.dis
ndisasm -b16 -e $((0x72620 + 0x2400)) ${GAMEFILE} | head -n 1324 >${OUTDIR}/SEG105.dis
ndisasm -b16 -e $((0x73550 + 0x2400)) ${GAMEFILE} | head -n 1899 >${OUTDIR}/SEG106.dis
ndisasm -b16 -e $((0x74ca0 + 0x2400)) ${GAMEFILE} | head -n 846 >${OUTDIR}/SEG107.dis
ndisasm -b16 -e $((0x757e0 + 0x2400)) ${GAMEFILE} | head -n 980 >${OUTDIR}/SEG108.dis
ndisasm -b16 -e $((0x76500 + 0x2400)) ${GAMEFILE} | head -n 1757 >${OUTDIR}/SEG109.dis
ndisasm -b16 -e $((0x779c0 + 0x2400)) ${GAMEFILE} | head -n 1680 >${OUTDIR}/SEG110.dis
ndisasm -b16 -e $((0x78df0 + 0x2400)) ${GAMEFILE} | head -n 1515 >${OUTDIR}/SEG111.dis
ndisasm -b16 -e $((0x7a1d0 + 0x2400)) ${GAMEFILE} | head -n 1495 >${OUTDIR}/SEG112.dis
ndisasm -b16 -e $((0x7b4e0 + 0x2400)) ${GAMEFILE} | head -n 1593 >${OUTDIR}/SEG113.dis
ndisasm -b16 -e $((0x7c9b0 + 0x2400)) ${GAMEFILE} | head -n 1635 >${OUTDIR}/SEG114.dis
ndisasm -b16 -e $((0x7df10 + 0x2400)) ${GAMEFILE} | head -n 1441 >${OUTDIR}/SEG115.dis
ndisasm -b16 -e $((0x7f210 + 0x2400)) ${GAMEFILE} | head -n 1640 >${OUTDIR}/SEG116.dis
ndisasm -b16 -e $((0x80710 + 0x2400)) ${GAMEFILE} | head -n 1747 >${OUTDIR}/SEG117.dis
ndisasm -b16 -e $((0x81c60 + 0x2400)) ${GAMEFILE} | head -n 1079 >${OUTDIR}/SEG118.dis
ndisasm -b16 -e $((0x82b00 + 0x2400)) ${GAMEFILE} | head -n 1244 >${OUTDIR}/SEG119.dis
ndisasm -b16 -e $((0x83b20 + 0x2400)) ${GAMEFILE} | head -n 1435 >${OUTDIR}/SEG120.dis
ndisasm -b16 -e $((0x84e20 + 0x2400)) ${GAMEFILE} | head -n 809 >${OUTDIR}/SEG121.dis
ndisasm -b16 -e $((0x858a0 + 0x2400)) ${GAMEFILE} | head -n 4 >${OUTDIR}/SEG122.dis

for i in ${OUTDIR}/*.dis; do
	#FNAME=${i##*/}
	#echo -n ${FNAME}

	# postprocess the disasm file
	python ./nc2fc.py $i
	mv ${i}.tmp ${i}
done
