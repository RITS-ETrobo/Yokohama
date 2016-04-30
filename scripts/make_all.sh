#!/usr/bin/env bash

if [ $# -ne 1 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 argument" 1>&2
    exit 1
fi

if [ "$1" == "app" ]; then
    buildOption="app"
else
    buildOption="img"
fi

set currentPath = pwd
WORKSPACE_PATH=hrp2/sdk/workspace
SCRIPT_PATH=../../../scripts/make_project.sh
cd ${WORKSPACE_PATH}

${SCRIPT_PATH} ev3way-cpp ${buildOption}
${SCRIPT_PATH} gyroboy ${buildOption}
${SCRIPT_PATH} helloev3 ${buildOption}
${SCRIPT_PATH} hwbrickbench ${buildOption}
${SCRIPT_PATH} linetrace ${buildOption}
${SCRIPT_PATH} sample_c4 ${buildOption}
${SCRIPT_PATH} test-cpp ${buildOption}
${SCRIPT_PATH} test-cyc ${buildOption}
${SCRIPT_PATH} trike ${buildOption}
${SCRIPT_PATH} "trike-old" ${buildOption}

cd $currentPath
exit 0
