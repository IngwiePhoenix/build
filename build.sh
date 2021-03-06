#!/bin/bash

BUSRC="stack.cpp string.cpp hash.cpp list.cpp trace.cpp stream.cpp formatter.cpp util.cpp sharedcore.cpp exceptionbase.cpp heap.cpp archivebase.cpp archive.cpp queue.cpp archival.cpp sio.cpp stdstream.cpp process.cpp plugger.cpp optparser.cpp signals.cpp array.cpp membuf.cpp file.cpp variant.cpp random.cpp randombasic.cpp randombase.cpp streamstack.cpp"
BUHDR="stack.h string.h hash.h list.h trace.h stream.h formatter.h util.h sharedcore.h exceptionbase.h heap.h archivebase.h archive.h queue.h archival.h sio.h stdstream.h process.h plugger.h singleton.h optparser.h array.h membuf.h file.h variant.h fmt.h extratypes.h random.h randombasic.h randombase.h streamstack.h"
BUEXPSRC="regex.cpp"
BUEXPHDR="regex.h"
BUCOMPAT="config.h compat/linux.h compat/win32.h compat/osx.h"

if [ -z "${CXX}" ]; then
    CXX="g++"
fi

function bld()
{
	OUTFILE="$1"
	if [ ! -e "$OUTFILE" ]; then
		return 1
	fi
	shift
	for INFILE in $*; do
		if [ "$INFILE" -nt "$OUTFILE" ]; then
			return 1
		fi
	done
	return 0
}

function cmd()
{
	printf "%8s $2\n" $1
	shift 2
	$* || exit
}

function gpp()
{
	bld "$1" "$2" || cmd CXX "$1" ${CXX} -ggdb -fPIC -W -Wall -Iminibu -c -o "$1" "$2"
}

function presetup()
{
    for dir in minibu/src minibu/bu minibu/bu/compat; do
        cmd MKDIR ${dir} mkdir -p ${dir}
    done
    cmd FAKE minibu/bu/autoconfig.h touch minibu/bu/autoconfig.h
    for file in $(cd bootstrap; ls); do
        cmd BOOTSTRAP minibu/bu/${file} cp bootstrap/${file} minibu/bu
    done
    for F in $BUSRC; do
        bld minibu/src/$F || cmd WGET minibu/src/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/stable/$F -O minibu/src/$F
    done
    for F in $BUHDR; do
        bld minibu/bu/$F || cmd WGET minibu/bu/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/stable/$F -O minibu/bu/$F
    done
    for F in $BUEXPSRC; do
        bld minibu/src/$F || cmd WGET minibu/src/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/experimental/$F -O minibu/src/$F
    done
    for F in $BUEXPHDR; do
        bld minibu/bu/$F || cmd WGET minibu/bu/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/experimental/$F -O minibu/bu/$F
    done
    for F in $BUCOMPAT; do
        bld minibu/bu/$F || cmd WGET minibu/bu/$F wget -q http://svn.xagasoft.com/libbu++/trunk/src/$F -O minibu/bu/$F
    done

    bld src/build.tab.c src/build.y || cmd BISON src/build.tab.c bison -bsrc/build src/build.y
    bld src/build.yy.c src/build.l || cmd FLEX src/build.yy.c flex src/build.l
}

if [ ! -z "$1" ]; then
	if [ "$1" == "clean" -o "$1" == "-c" ]; then
		echo "Cleaning up, deleting all object code and downloaded source code..."
		echo 
		rm -Rf minibu src/*.o
		exit
    elif [ "$1" == "setup" -o "$1" == "-s" ]; then
        echo "Only preforming download / setup"
        echo
        presetup
        exit
	else
		echo "Without parameters build.sh will download extra components and"
        echo "compile build."
        echo
        echo "Parameters:"
        echo "  clean | -c  Delete all opbject code and downloaded source."
        echo "  setup | -s  Download and perform initial setup, but do not"
        echo "              compile."
        echo
        echo "Using ${CXX} to complie code."
        echo
		exit
	fi
fi

presetup

for F in $BUSRC $BUEXPSRC; do
	OUTPUT=${F%.*}.o
	bld minibu/src/$OUTPUT || gpp minibu/src/$OUTPUT minibu/src/$F
done

for F in src/*.c src/*.cpp; do
	OUTPUT=${F%.*}.o
	gpp "$OUTPUT" "$F"
done
bld build src/*.o minibu/src/*.o || cmd LINK build ${CXX} -fPIC -rdynamic -Wl,-export-dynamic -o build src/*.o minibu/src/*.o -ldl
bld build~ build || cmd CP build~ cp build build~
