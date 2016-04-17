#!/usr/bin/env bash

if [ $# -ne 1 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 1 argument" 1>&2
    exit 1
fi

echo "Entered argument is $1."
if [ "${buildOption}" = "app" ]; then
    buildOption="app"
else
    buildOption="mod"
fi

echo "buildOption is ${buildOption}"

set currentPath = pwd
cd hrp2/workspace
make ${buildOption}=ev3way-cpp
make ${buildOption}=gyrobody
make ${buildOption}=helloev3
make ${buildOption}=hwbrickbench
make ${buildOption}=linetrace
make ${buildOption}=loader
make ${buildOption}=sample_c4
make ${buildOption}=test-cpp
make ${buildOption}=test-cyc
make ${buildOption}=trike

cd $currentPath

