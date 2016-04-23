#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 argument" 1>&2
    exit 1
fi

TARGET_PROJECT=$1
make $2=${TARGET_PROJECT}
TARGET_PATH=bin/$2/$1
rm -rf ${TARGET_PATH}
mkdir -p ${TARGET_PATH}
mv app ${TARGET_PATH}/${TARGET_PROJECT}.$2
mv OBJ/ ${TARGET_PATH}

exit 0
