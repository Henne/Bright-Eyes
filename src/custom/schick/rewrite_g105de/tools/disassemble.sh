#!/bin/sh

# Disassemble GEN.EXE v1.05_de into its code segments
# needs: nasm, python, head (coreutils)

# path to the game
GAMEDIR="./"
# filename of the GEN.EXE
GAMEFILE="${GAMEDIR}/GEN.EXE"
# output directory
OUTDIR=../temp/disasm_orig


# create the directory for the output
mkdir -p $OUTDIR

err=0
# check for the needed programs
for prog in ndisasm head python3; do
	$prog --version >/dev/null 2>/dev/null
	if [ $? -eq 127 ]; then
		echo "ERROR: The program $prog is needed"
		err=1
	fi
done

if [ $err -eq 1 ]; then exit 1 ; fi

# check if the gamefile exists
if [ ! -e ${GAMEFILE} ]; then
	echo "ERROR: cant find VGAME.EXE in this directory"
	exit 1
fi

# generate a disassembly for each segment
ndisasm -b16 -e $((0x00000 + 0x0c00)) ${GAMEFILE} | head -n 6088 >${OUTDIR}/SEG000.dis
ndisasm -b16 -e $((0x03649 + 0x0c00)) ${GAMEFILE} | head -n 618 >${OUTDIR}/SEG001.dis
ndisasm -b16 -e $((0x03c60 + 0x0c00)) ${GAMEFILE} | head -n 11430 >${OUTDIR}/SEG002.dis
ndisasm -b16 -e $((0x0b2db + 0x0c00)) ${GAMEFILE} | head -n 91 >${OUTDIR}/SEG003.dis
ndisasm -b16 -e $((0x0b39c + 0x0c00)) ${GAMEFILE} | head -n 282 >${OUTDIR}/SEG004.dis
ndisasm -b16 -e $((0x0b6b8 + 0x0c00)) ${GAMEFILE} | head -n 646 >${OUTDIR}/SEG005.dis
#AIL-quirk: this object has 946 bytes of BCC data upfront
#ndisasm -b16 -e $((0x0bb2a + 0x0c00)) ${GAMEFILE} | head -n 1465 >${OUTDIR}/SEG006.dis
ndisasm -b16 -e $((0x0bb2a + 0x0c00 + 0x3b2)) ${GAMEFILE} | head -n 922 >${OUTDIR}/SEG006.dis

for i in ${OUTDIR}/*.dis; do
	#FNAME=${i##*/}
	#echo -n ${FNAME}

	# postprocess the disasm file
	python3 ../../../tools/nc2fc.py $i
	mv ${i}.tmp ${i}
done
