#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 arguments" 1>&2
    exit 1
fi

if [ ! -e $1 ]; then
    exit 0
fi

if [ -f $1 ]; then
    mv $1 $2
    exit 0
fi

if [ -d $1 ]; then
    mv $1 $2
    exit 0
fi

echo "We could not move $1 to $2."
exit 1
