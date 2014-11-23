#!/usr/bin/env python
# coding=ascii

# Aendert die Arten der Funktionsaufrufe in NASM-Disassembler Listings
#
#
# Beim Kompilieren mit dem Borland Compiler 3.1 unterscheidet sich der
# Binaercode in Objekt- und EXE-Dateien.
#
# Der Grund dafuer ist, dass der Compiler bei der Codeerzeugung noch nicht
# ueber alle Funktionen der zu kompilierenden Quelldatei informiert ist.
# Wird ein Funktionsaufruf zu einer noch unbekannten Funktion
# erkannt, wird der Code fuer einen Farcall eingefuegt.
#
# Wenn die aufgerufene Funktion doch noch innerhalb dieser Objektdatei
# ist ersetzt der Linker, aus Performanzgruenden, diese Farcalls durch
# eine gleichgrosse Codesequenz mit Nearcalls.
# Diese Ersetzungen koennen daran erkannt werden, dass die Funktion
# innerhalb des Segments in aufseteigender Adressrichtung liegt.

# Compiler:	call absadr
# Linker:	nop ; push cs; call word reladr

import sys
import string

# Anderungen in der Ausgabedatei
nc = 0
fc = 0

name = sys.argv[1]

fname = string.rsplit(name, "/", 1)[1]

infile = open(name, 'r')
outfile = open(name + ".tmp", 'w')

line = infile.readline()
while line is not None and line != "":

	nopline = string.split(line, None, 2)
	opcode = int(nopline[1][:2], 16)

	# far call
	if opcode == 0x9a:
		o = nopline[0]
		o = o + "  "
		o = o + "9A00000000"
		o = o + "        "
		o = o + "call word 0x0:0x0\n"
		outfile.write(o)
		fc = fc + 1

	# check for a line with nop instruction
	elif opcode == 0x90:
		lines1 = line
		lines2 = infile.readline()
		if lines2 is None:
			outfile.write(lines1)
			continue

		pushline = string.split(lines2, None, 2)
		opcode = int(pushline[1][:2], 16)
		if opcode == 0x0e:
			lines3 = infile.readline()
			if lines3 is None:
				outfile.write(lines1)
				outfile.write(lines2)
				continue

			callline = string.split(lines3)

			instruction = callline[1]
			opcode = int(instruction[:2], 16)
			if opcode == 0xe8:
				#get the old relative address
				address_lo = int(instruction[2:4], 16)
				address_hi = int(instruction[4:6], 16)
				address = address_hi * 256 + address_lo

				# forward call
				if address < 0x8000:

					#calculate the absolute address
					new_add = address + int(callline[0], 16) + 3

					lo = new_add % 256
					hi = new_add / 256

					#create one output line

					#create offset
					o = nopline[0]
					o = o + "  "

					#create opcode
					o = o + "9A"
					o = o + string.upper(hex(lo)[2:4].zfill(2))
					o = o + string.upper(hex(hi)[2:4].zfill(2))
					o = o + "0000        "

					#create assembly
					o = o + "call word 0x0:"
					o = o + hex(new_add) + "\n"
					outfile.write(o)

					nc = nc + 1
				else:
					outfile.write(lines1)
					outfile.write(lines2)
					outfile.write(lines3)
			else:
				outfile.write(lines1)
				outfile.write(lines2)
				outfile.write(lines3)
		else:
			outfile.write(lines1)
			outfile.write(lines2)
	else:
		outfile.write(line)

	#read next line
	line = infile.readline()

infile.close()
outfile.close()

if nc > 0 or fc > 0:
	print fname, "near = ", nc, " far =", fc, " changed"
else:
	print fname
