#!/usr/bin/env bash

# This script is for making selected target directories

if [ $# -ne 2 ]; then
    echo "Illegal parameter" 1>&2
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 arguments." 1>&2
    echo "The meaning of 1st parameter is the path of build target." 1>&2
    echo "The meaning of 2nd parameter is build option." 1>&2
    exit 1
fi

# Remove that would be generated files and directories
SCRIPT_REMOVE=../../../scripts/remove_file.sh
${SCRIPT_REMOVE} ../OBJ
${SCRIPT_REMOVE} app
${SCRIPT_REMOVE} hrp2
${SCRIPT_REMOVE} uImage

# Execute make command to the selected project
TARGET_PROJECT=$1
echo "make $2=${TARGET_PROJECT}"
make $2=${TARGET_PROJECT} 1>build.log 2>&1

# Move the generated files and directories
TARGET_PATH=../bin/$2/$1
${SCRIPT_REMOVE} ${TARGET_PATH}
mkdir -p ${TARGET_PATH}

SCRIPT_MOVE=../../../scripts/move_file.sh
${SCRIPT_MOVE} build.log ${TARGET_PATH}/${TARGET_PROJECT}.$2.log
${SCRIPT_MOVE} app ${TARGET_PATH}/${TARGET_PROJECT}.$2
${SCRIPT_MOVE} hrp2 ${TARGET_PATH}
${SCRIPT_MOVE} uImage ${TARGET_PATH}
${SCRIPT_MOVE} ../OBJ ${TARGET_PATH}

# Check result
# If result of make command is success, this script outputs "    SUCCEEDED".
#   If parameter is "app", ${TARGET_PROJECT}.$2 exists under ${TARGET_PATH}.
#   And, if parameter is not "app", hrp2 and uImage exist under ${TARGET_PATH}.
# Otherwise, this script outputs "    FAILED".
if [ "$2" == "app" ]; then
    if [ ! -e ${TARGET_PATH}/${TARGET_PROJECT}.$2 ]; then
        echo "    FAILED"
        exit 2
    fi
else
    if [ ! -e ${TARGET_PATH}/hrp2 ]; then
        echo "    FAILED"
        exit 2
    fi

    if [ ! -e ${TARGET_PATH}/uImage ]; then
        echo "    FAILED"
        exit 2
    fi
fi

echo "    SUCCEEDED"
exit 0
