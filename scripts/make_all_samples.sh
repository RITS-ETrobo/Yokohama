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

WORKSPACE_PATH=hrp2/sdk/workspace_samples
SCRIPT_PATH=../../../scripts/make_project.sh
cd ${WORKSPACE_PATH}

# Execute the make command for all target directories
${SCRIPT_PATH} sample_c4 ${buildOption}
${SCRIPT_PATH} etrobo_tr ${buildOption}
${SCRIPT_PATH} etrobo_tr_ex1 ${buildOption}
${SCRIPT_PATH} etrobo_tr_ex2 ${buildOption}
${SCRIPT_PATH} etrobo_tr_ex3 ${buildOption}
${SCRIPT_PATH} sample_cpp4 ${buildOption}
${SCRIPT_PATH} sample00 ${buildOption}
${SCRIPT_PATH} sample01 ${buildOption}
${SCRIPT_PATH} sample02 ${buildOption}
${SCRIPT_PATH} sample03 ${buildOption}
${SCRIPT_PATH} sample04 ${buildOption}
${SCRIPT_PATH} sample05 ${buildOption}

cd ${projectRoot}

# Cancel modifications the following *.a
git checkout -- ./hrp2/sdk/common/library/lib2/lib2-standalone.a
git checkout -- ./hrp2/sdk/common/library/libcpp-test/libcpp-test-standalone.a

exit 0
