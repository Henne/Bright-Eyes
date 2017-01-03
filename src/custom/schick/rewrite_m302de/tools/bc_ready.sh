#!/bin/bash

# This script compiles all files maked as finalized and makes sure the have not changed

DIR=${PWD}/temp
OBJDIR=${DIR}/OBJ
BINDIR=${DIR}/BIN
DISDIR=${DIR}/disasm

DISORIG=${DIR}/disasm_orig

DRIVE_C=${PWD}/../../drive_c

rm -rf $OBJDIR/* $BINDIR/* $DISDIR/*

mkdir -p $OBJDIR
mkdir -p $BINDIR
mkdir -p $DISDIR

USE_KVM="false"

#needs modprobe qemu-nbd ndisasm diff mount head

if [ "${USE_KVM}" = "true" ]; then

	MOUNT="/mnt/compile"
	DIR="${MOUNT}/BORLANDC/SCHICK"
	DEVICE="/dev/nbd4"

	# check nbd in loaded
	out=$(lsmod | grep ^nbd);
	if [ "${out}" = "" ]; then
		echo "Lade nbd modul"
		sudo modprobe nbd max_part=8
	fi

	# assume device 0 can be used
	sudo qemu-nbd --disconnect ${DEVICE}
	sudo qemu-nbd --connect=${DEVICE} ~/qemu/BE-compile/hda.qcow2
	sudo partprobe ${DEVICE}

	#mounten
	sudo mount ${DEVICE}p1 ${MOUNT} -t vfat -o rw,users

	#cpp files kopieren
	sudo rm -rf ${DIR}
	sudo mkdir -p ${DIR}
	sudo cp *.cpp *.h *.asm bc_ready.bat ${DIR}
	sudo cp -r AIL ${DIR}
	sync

	#unmounten
	sudo umount ${MOUNT}

	#kompilieren
	pushd ~/qemu/BE-compile/
	./start.sh
	popd
	sync


	#mounten
	sudo mount ${DEVICE}p1 ${MOUNT} -t vfat -o ro,users,sync
	#obj files zurueckkopieren
	sudo cp ${DIR}/*.OBJ $OBJDIR
	sync
	sudo umount ${MOUNT}
	#umounten
	sudo qemu-nbd --disconnect ${DEVICE}
else

	# copy all source files to DRIVE_C
	cp *.cpp *.h *.asm RESP ${DRIVE_C}/src
	cp -r AIL ${DRIVE_C}/src

	# copy c_ready.bat as compile.bat
	cp bc_ready.bat ${DRIVE_C}/src/compile.bat

	# run compile.bat in a DOSBox environment, needs an installes BCC.EXE there
	pushd ${DRIVE_C}
	dosbox -conf compile.conf
	popd

	# cleanup
	rm -rf ${DRIVE_C}/src/*.cpp
	rm -rf ${DRIVE_C}/src/*.h
	rm -rf ${DRIVE_C}/src/*.asm
	rm -rf ${DRIVE_C}/src/RESP
	rm -rf ${DRIVE_C}/src/compile.bat
	rm -rf ${DRIVE_C}/src/AIL

	# move all OBJ-files to OBJDIR
	mv ${DRIVE_C}/src/*.OBJ $OBJDIR 2>/dev/null

	# move all OBJ-files to OBJDIR
	mv ${DRIVE_C}/src/*.EXE $DIR 2>/dev/null
	mv ${DRIVE_C}/src/*.MAP $DIR 2>/dev/null
fi

N=0
GOOD=0
FAIL=0

# disassemble all *.OBJ files
for i in ${OBJDIR}/*.OBJ; do

	# extract the filename
	PREFIX=${i%\.OBJ}
	PREFIX=${PREFIX##*/}

	# extract instructions
	./tools/dump_obj $i >/dev/null
	# move the BIN-files to BINDIR
	mv ${OBJDIR}/${PREFIX}.BIN $BINDIR

	# diassemble BIN-file to DISDIR
	ndisasm -b16 ${BINDIR}/${PREFIX}.BIN >${DISDIR}/${PREFIX}.dis
#	ndisasm -b16 -e4 ${PREFIX}.BIN >${PREFIX}.dis

	# count lines of the original disassembly
	LINES=$(wc -l ${DISORIG}/${PREFIX}.dis |cut -d " " -f 1);

	# make the fresh file have the same length
	head -n $LINES ${DISDIR}/${PREFIX}.dis >${DISDIR}/${PREFIX}.tmp
	mv ${DISDIR}/${PREFIX}.tmp ${DISDIR}/${PREFIX}.dis


	RETVAL=0
	# count the lines containing '|' => difference
	case "${PREFIX}" in
		"SEG001")
			# exact 24 differing lines are allowed
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)

			if [ $LINES -ne 24 ]; then RETVAL=1; fi
			;;
		"SEG002")
			# exact 26 differing lines are allowed
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 26 ]; then RETVAL=1; fi
			;;
		"SEG004")
			# exact 1 differing lines are allowed
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 1 ]; then RETVAL=1; fi
			;;
		"SEG008")
			# exact 42 differing lines are allowed
			# adresses in unalinged codesegment
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 42 ]; then RETVAL=1; fi
			;;
		"SEG011")
			# AIL: dump_obj produces uncomparable files due to BSS
			;;
		"SEG013")
			# AIL: dump_obj produces uncomparable files due to BSS
			;;
		"SEG048")
			# exact 27 differing lines are allowed
			# other code in switch statements
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 27 ]; then RETVAL=1; fi
			;;
		"SEG049")
			# exact two differing lines are allowed
			# (function pointer argument)
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG050")
			# exact 1 differing lines are allowed
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 1 ]; then RETVAL=1; fi
			;;
		"SEG055")
			# exact 6 differing lines are allowed
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 6 ]; then RETVAL=1; fi
			;;
		"SEG092")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG106")
			# exact 57 differing lines are allowed
			# (function pointer argument)
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 57 ]; then RETVAL=1; fi
			;;
		"SEG113")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG120")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			LINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $LINES -ne 2 ]; then RETVAL=1; fi
			;;
		*)
			diff -q -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis
			RETVAL=$?
			;;
	esac

	if [ $RETVAL -eq 0 ]; then
		GOOD=$(($GOOD+1))
	else
		echo "Fehler: ${PREFIX} ist verschieden"
		FAIL=$(($FAIL+1))
	fi

	N=$(($N+1))
done

echo "REPORT ${N} Files: Good = ${GOOD} Fail = ${FAIL}"

# count the compile commands in bc_ready.bat, to make sure there is none missing
COMP=$(grep "\.\." bc_ready.bat | wc -l);

if [ ${COMP} -ne ${N} ]; then
	echo "Fehler: ${N} Dateien wurden geprueft, aber es sollten ${COMP} sein"
	exit 1;
fi

exit $FAIL
