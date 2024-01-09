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

# COMPILATION with DOSBox

# copy all source files to DRIVE_C
cp *.cpp *.h *.asm TLINK.RES ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp bc_ready.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf compile.conf
popd

# cleanup
rm -rf ${DRIVE_C}/src/*.cpp
rm -rf ${DRIVE_C}/src/*.h
rm -rf ${DRIVE_C}/src/*.asm
rm -rf ${DRIVE_C}/src/TLINK.RES
rm -rf ${DRIVE_C}/src/compile.bat
rm -rf ${DRIVE_C}/src/AIL

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.OBJ $OBJDIR 2>/dev/null

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.EXE $DIR 2>/dev/null
mv ${DRIVE_C}/src/*.MAP $DIR 2>/dev/null

# VERIFICATION

N=0
GOOD=0
FAIL=0

# disassemble all *.OBJ files
for i in ${OBJDIR}/*.OBJ; do

	# extract the filename
	PREFIX=${i%\.OBJ}
	PREFIX=${PREFIX##*/}

	# TODO: SEG013 is not extracted properly
	if [ ${PREFIX} = "SEG013" ]; then
		GOOD=$(($GOOD+1))
		N=$(($N+1))
		continue;
	fi

	# extract instructions
	./tools/dump_obj $i >/dev/null
	# move the BIN-files to BINDIR
	mv ${OBJDIR}/${PREFIX}.BIN $BINDIR

	# diassemble BIN-file to DISDIR
	ndisasm -b16 ${BINDIR}/${PREFIX}.BIN >${DISDIR}/${PREFIX}.dis
#	ndisasm -b16 -e4 ${PREFIX}.BIN >${PREFIX}.dis

	# count lines of the original disassembly
	ORIGLINES=$(wc -l ${DISORIG}/${PREFIX}.dis | cut -d " " -f 1)

	# make the fresh file have the same length
	head -n $ORIGLINES ${DISDIR}/${PREFIX}.dis >${DISDIR}/${PREFIX}.tmp
	mv ${DISDIR}/${PREFIX}.tmp ${DISDIR}/${PREFIX}.dis


	RETVAL=0
	# count the lines containing '|' => difference
	case "${PREFIX}" in
		"SEG001")
			# exact 23 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)

			if [ $DIFFLINES -ne 23 ]; then RETVAL=1; fi
			;;
		"SEG002")
			# exact 26 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 26 ]; then RETVAL=1; fi
			;;
		"SEG004")
			# exact 1 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 1 ]; then RETVAL=1; fi
			;;
		"SEG008")
			# exact 42 differing lines are allowed
			# adresses in unalinged codesegment
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 42 ]; then RETVAL=1; fi
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
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 27 ]; then RETVAL=1; fi
			;;
		"SEG049")
			# exact two differing lines are allowed
			# (function pointer argument)
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG050")
			# exact 1 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 1 ]; then RETVAL=1; fi
			;;
		"SEG055")
			# exact 6 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 6 ]; then RETVAL=1; fi
			;;
		"SEG092")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG106")
			# exact 57 differing lines are allowed
			# (function pointer argument)
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 57 ]; then RETVAL=1; fi
			;;
		"SEG113")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 2 ]; then RETVAL=1; fi
			;;
		"SEG120")
			# exact 2 differing lines are allowed
			# (function pointer argument)
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 2 ]; then RETVAL=1; fi
			;;
		"DATSEG")
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
