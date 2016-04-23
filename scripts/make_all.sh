#!/usr/bin/env bash

if [ $# -ne 1 ]; then
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 argument" 1>&2
    exit 1
fi

echo "Entered argument is $1."
if [ "$1" == "app" ]; then
    buildOption="app"
else
    buildOption="mod"
fi

echo "buildOption is ${buildOption}"

set currentPath = pwd
cd hrp2/workspace

../../scripts/make_project.sh ev3way-cpp ${buildOption}
../../scripts/make_project.sh gyrobody ${buildOption}
../../scripts/make_project.sh helloev3 ${buildOption}
../../scripts/make_project.sh hwbrickbench ${buildOption}
../../scripts/make_project.sh linetrace ${buildOption}
../../scripts/make_project.sh loader ${buildOption}
../../scripts/make_project.sh sample_c4 ${buildOption}
../../scripts/make_project.sh test-cpp ${buildOption}
../../scripts/make_project.sh test-cyc ${buildOption}
../../scripts/make_project.sh trike ${buildOption}

cd $currentPath
exit 0
