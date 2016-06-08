#!/usr/bin/env bash

# This script is for making all target directories

if [ $# -ne 1 ]; then
    echo "Illegal parameter" 1>&2
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 argument" 1>&2
    echo "The meaning of parameter is build option." 1>&2
    exit 1
fi

# Determine build option
if [ "$1" == "app" ]; then
    buildOption="app"
else
    buildOption="img"
fi

projectRoot=`pwd`

WORKSPACE_PATH=hrp2/sdk/workspace
SCRIPT_PATH=../../../scripts/make_project.sh
cd ${WORKSPACE_PATH}

# Execute make command for all target directories
${SCRIPT_PATH} hackEV ${buildOption}

cd ${projectRoot}

# Cancel modifications the following *.a
git checkout -- ./hrp2/sdk/common/library/lib2/lib2-standalone.a
git checkout -- ./hrp2/sdk/common/library/libcpp-test/libcpp-test-standalone.a

exit 0
