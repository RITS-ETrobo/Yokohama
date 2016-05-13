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

set currentPath = pwd
WORKSPACE_PATH=hrp2/sdk/workspace_samples
SCRIPT_PATH=../../../scripts/make_project.sh
cd ${WORKSPACE_PATH}

# Execute make command for all target directories
${SCRIPT_PATH} sample_c4 ${buildOption}

cd $currentPath
exit 0
