#!/usr/bin/env bash

if [ $# -ne 1 ]; then
    echo "For executing this script, need 1 argument" 1>&2
    exit 1
fi

if [ ! -e $1 ]; then
    exit 0
fi

if [ -f $1 ]; then
    rm $1
    exit 0
fi

if [ -d $1 ]; then
    rm -rf $1
    exit 0
fi

echo "We could not remove $1"
exit 1
