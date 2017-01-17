#!/usr/bin/env bash

# This script is for testing hackEV project.

clear
./scripts/remove_file.sh "test/build"
mkdir -p test/build
cd test/

SOURCE_FILES="\
main.cpp test/hackEV/Test*.cpp test/hackEV/sensors/Test*.cpp \
../hrp2/sdk/workspace/common/user_function.cpp \
../hrp2/sdk/workspace/hackEV/EV3Position.cpp \
../hrp2/sdk/workspace/hackEV/SpeedCalculator.cpp \
../hrp2/sdk/workspace/hackEV/sensors/ColorSensorController.cpp"

INCLUDE_DIRECTORIES="\
-I./test \
-I./test/hackEV \
-I./test/hackEV/sensors \
-I./gtest/googletest/include \
-I../hrp2/sdk/workspace/common \
-I../hrp2/sdk/workspace/common/params \
-I../hrp2/sdk/workspace/hackEV \
-I../hrp2/sdk/workspace/hackEV/params \
-I../hrp2/sdk/workspace/hackEV/motors \
-I../hrp2/sdk/workspace/hackEV/sensors"

LIBRARIES="\
-L./gtest/googletest/lib \
-lgtest \
-lpthread"

g++ -o unittestEV3 -DEV3_UNITTEST ${INCLUDE_DIRECTORIES} ${SOURCE_FILES} ${LIBRARIES} 1>build/build.log 2>&1
RETURN_CODE=$?

if [ ${RETURN_CODE} = 0 ]; then
    mv unittestEV3 build/
    ./build/unittestEV3 1>build/test.log 2>&1
    cat build/test.log
else
    vim build/build.log
fi

cd ../
exit ${RETURN_CODE}
