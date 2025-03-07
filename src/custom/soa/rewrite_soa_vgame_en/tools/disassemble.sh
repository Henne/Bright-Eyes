#!/bin/sh

# Disassemble VGAME.EXE EN into its code segments
# needs: nasm, python, head (coreutils)

# path to the game
GAMEDIR="./"
# filename of the SCHICKM.EXE
GAMEFILE="${GAMEDIR}/VGAME.EXE"
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
ndisasm -b16 -e $((0x00000 + 0x3600)) ${GAMEFILE} | head -n 5692 >${OUTDIR}/SEG000.dis
ndisasm -b16 -e $((0x03435 + 0x3600)) ${GAMEFILE} | head -n 7519 >${OUTDIR}/SEG001.dis
ndisasm -b16 -e $((0x08164 + 0x3600)) ${GAMEFILE} | head -n 10381 >${OUTDIR}/SEG002.dis
ndisasm -b16 -e $((0x0ec2b + 0x3600)) ${GAMEFILE} | head -n 5068 >${OUTDIR}/SEG003.dis
ndisasm -b16 -e $((0x12603 + 0x3600)) ${GAMEFILE} | head -n 847 >${OUTDIR}/SEG004.dis
ndisasm -b16 -e $((0x12f33 + 0x3600)) ${GAMEFILE} | head -n 5329 >${OUTDIR}/SEG005.dis
ndisasm -b16 -e $((0x16828 + 0x3600)) ${GAMEFILE} | head -n 5945 >${OUTDIR}/SEG006.dis
ndisasm -b16 -e $((0x1a704 + 0x3600)) ${GAMEFILE} | head -n 549 >${OUTDIR}/SEG007.dis
ndisasm -b16 -e $((0x1ad0c + 0x3600)) ${GAMEFILE} | head -n 96 >${OUTDIR}/SEG008.dis
ndisasm -b16 -e $((0x1add0 + 0x3600)) ${GAMEFILE} | head -n 1745 >${OUTDIR}/SEG009.dis
ndisasm -b16 -e $((0x1bc19 + 0x3600)) ${GAMEFILE} | head -n 5832 >${OUTDIR}/SEG010.dis
ndisasm -b16 -e $((0x1faf1 + 0x3600)) ${GAMEFILE} | head -n 2674 >${OUTDIR}/SEG011.dis
ndisasm -b16 -e $((0x2181c + 0x3600)) ${GAMEFILE} | head -n 1024 >${OUTDIR}/SEG012.dis

for i in ${OUTDIR}/*.dis; do
	#FNAME=${i##*/}
	#echo -n ${FNAME}

	# postprocess the disasm file
	python3 ../../../tools/nc2fc.py $i
	mv ${i}.tmp ${i}
done
