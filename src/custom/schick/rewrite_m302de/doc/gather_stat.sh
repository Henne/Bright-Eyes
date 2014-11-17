#!/bin/sh
#
#	Gather development status information of Bright-Eyes

#outfput format csv, html
OUTFMT="html"

DIR=$PWD

cd ..

let F_ALL=0
let F_DONE=0
let F_BC=0
let FILES=0

case "$OUTFMT" in
	"csv")
		echo "$OUTFMT"
		OUTFILE="${DIR}/details.csv"
		rm -rf "$OUTFILE"
		;;
	"html")
		OUTFILE="${DIR}/details.html"
		rm -rf "$OUTFILE"
		echo "<?xml version=\"1.0\"?><html>" >$OUTFILE
		echo "<head><title>Bright-Eyes development status</title>" >>$OUTFILE
		echo "<style type=\"text/css\">tr.done {background-color: green;} tr.new {background-color: gray} tr.inwork {background-color:white}</style></head>" >>$OUTFILE
		echo "<body>" >>$OUTFILE
		echo "<table border="1">" >>$OUTFILE
		echo "<thead><tr><th>Object</th><th>Content</th><th>functions</th><th>done</th><th>identical</th></tr></thead><tbody>" >>$OUTFILE
		;;
esac

for FILE in seg*.cpp; do

	#get sement name
	NAME=${FILE%%.cpp}

	#get content of the segment
	CONTENT=$(grep -m1 "Rewrite of DSA1 v3.02_de functions" ${FILE} | cut -d '(' -f 2 |cut -d ')' -f 1)

	# read function counters
	LINE=$(grep -m1 "Functions rewritten:" ${FILE}| cut -d ':' -f 2 | cut -d '(' -f 1)

	if [ "$LINE " != " " ]; then

		let ALL=$(echo ${LINE} |cut -d '/' -f 2);
		let DONE=$(echo ${LINE} |cut -d '/' -f 1);

		grep -P -m1 "^ \*\tBorlandified and identical$" ${FILE} 2>/dev/null >/dev/null

		if [ $? -eq 0 ]; then
			#found => segment is complete
			BC=$DONE
		else
			#not found => some work todo
			BC=$(grep -c "/\* Borlandified and identical \*/" ${FILE})
		fi

		let F_BC=$(expr ${F_BC} + ${BC} )
		let F_DONE=$(expr ${F_DONE} + ${DONE} )
		let F_ALL=$(expr ${F_ALL} + ${ALL} )
		let FILES=$(expr $FILES + 1 )
	fi

	# put out a line
	case "$OUTFMT" in
		"csv")
			echo "$NAME; $CONTENT; $ALL; $DONE; $BC" >>$OUTFILE;
			;;
		"html")
			if [ $ALL -eq $BC ]; then
				CLASS="done";
			else
				if [ $DONE -eq 0 ]; then
					CLASS="new"
				else
					CLASS="inwork"
				fi
			fi

			echo "<tr class=\"$CLASS\"><td>$NAME</td><td>$CONTENT</td><td>$ALL</td><td>$DONE</td><td>$BC</tr>" >>$OUTFILE;
			;;
	esac

done

case "$OUTFMT" in
	"csv")
		;;
	"html")
		echo "</tbody></table></body></html>" >>$OUTFILE
		;;
esac

echo "$F_DONE / $F_ALL in $FILES files are done, $F_BC are identical"

cd $DIR
