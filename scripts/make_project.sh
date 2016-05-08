#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 argument" 1>&2
exit 1
fi

SCRIPT_REMOVE=../../../scripts/remove_file.sh
${SCRIPT_REMOVE} ../OBJ
${SCRIPT_REMOVE} app
${SCRIPT_REMOVE} hrp2
${SCRIPT_REMOVE} uImage

TARGET_PROJECT=$1
echo "make $2=${TARGET_PROJECT}"
make $2=${TARGET_PROJECT} 1>build.log 2>&1

TARGET_PATH=../bin/$2/$1
${SCRIPT_REMOVE} ${TARGET_PATH}
mkdir -p ${TARGET_PATH}

mv build.log ${TARGET_PATH}/${TARGET_PROJECT}.$2.log
mv app ${TARGET_PATH}/${TARGET_PROJECT}.$2
mv hrp2 ${TARGET_PATH}
mv uImage ${TARGET_PATH}
mv ../OBJ ${TARGET_PATH}

exit 0
