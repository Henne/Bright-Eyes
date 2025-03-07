#!/bin/bash

# replaces addresses from the DS with symbolic names from symbols.h

if [ ! -e symbols.h ]; then
	echo "ERROR: symbols.h does not exist"
	exit 1
fi

IFS=$'\n'
for line in $(egrep "^#define " symbols.h | egrep "\(0x[0-9a-f]{4}\)"); do

	# extract the name and the position
	NAME=$(echo ${line} | cut -d " " -f 2 | cut -d " " -f 1)
	POS=$(echo ${line} | cut -d "(" -f 2 | cut -d ")" -f 1)

#	echo "$line"
	echo "replace $POS with $NAME"

	for file in *.cpp; do

		# look in the cpp-file for position
		grep ${POS} $file

		if [ $? -eq 0 ]; then
			# found => replace position with name
			sed -i -e "s/${POS}/${NAME}/" $file
		fi
	done
done
