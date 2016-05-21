#!/usr/bin/env bash

# For details, please refer to http://dev.toppers.jp/trac_user/ev3pf/wiki/DevEnvLinux

projectRoot=`pwd`

# Create development environment
cd hrp2/cfg
make

cd ${projectRoot}

exit 0
