#!/bin/bash

# Skrypt testujący
#
# Sposób użycia:
#
# ./testuj.sh RUBIK.c testy

function error() {
    echo "$1"
    exit 1
}

if [ -z "$1" ] ; then
    error "sposob uzycia: $0 PLIK_ZRODLOWY [ katalog z testami ]"
fi

TESTS_DIR="testy"
COMPILER_C_OPTIONS="-std=c17 -pedantic -Wall -O1 -Wextra -Werror -fstack-protector-strong -g -Wformat-security -Wduplicated-cond -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init -Wlogical-not-parentheses -Wnull-dereference -Wvla -fsanitize=undefined -fno-sanitize-recover -fno-omit-frame-pointer"
COMPILER_CPP_OPTIONS="-std=c++17 -pedantic -Wall -Wextra -Werror -fstack-protector-strong -g"

SOL="$1"
if [ -n "$2" -a -d "$2" ] ; then
    TESTS_DIR="$2"
fi
TESTS=`cd $TESTS_DIR ; ls -1 *.in | sort -n -k 3 -t t`
TEMP_DIR=`pwd`
EXE="$TEMP_DIR/a.e"

function compile() {
    EXT=`echo $SOL | cut -f 2 -d '.'`
    case $EXT in
    c)
        gcc $COMPILER_C_OPTIONS "$SOL" -o"$EXE" 
        retcode=$?
        ;;
    cpp|cc)
        g++ $COMPILER_CPP_OPTIONS "$SOL" -o"$EXE" 
        retcode=$?
        ;;
    *)
        error "Nieznane rozszerzenie $EXT"
    esac

    if [ $retcode -ne 0 ] ; then
        error "compile error"
    else
        echo "Compile OK :)"
    fi
}

function test_solution() {
    tname="$1"
    in_file="$TESTS_DIR/$1"
    out_file="${in_file%.in}.out"
    user_out_file="$TEMP_DIR/a.out"
    user_err_file="$TEMP_DIR/a.err"
    $EXE < $in_file > $user_out_file 2> $user_err_file
    retcode=$?
    echo -n "$tname .. "
    if [ $retcode -ne 0 ] ; then
	   echo "RUNTIME ERROR: $retcode"
       echo "STDERR":
       cat $user_err_file
       echo "STDOUT":
       cat $user_out_file
    elif [ ! -e "$out_file" ] ; then
       echo "MISSING $out_file"
    elif ! diff -q -b "$out_file" "$user_out_file" > /dev/null ; then
	   echo "WRONG ANSWER"
	   diff -b "$out_file" "$user_out_file" 
    elif ! diff -q "$out_file" "$user_out_file" > /dev/null ; then
	   echo "ALMOST OK"
	   diff "$out_file" "$user_out_file" 
    else
	   echo "OK"
    fi
    rm -f "${user_out_file}" "${user_err_file}"
}

compile
for t in 3 ; do
    test_solution "$t"
done
for t in 10 ; do
    test_solution "$t"
done
rm -f "$EXE"
