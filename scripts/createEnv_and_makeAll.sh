#!/usr/bin/env bash

# Create environment
./scripts/create_env.sh

# Execute the make command
./scripts/make_all.sh app
./scripts/make_all.sh img

# Revert the modified files
git checkout hrp2/sdk/common/library/libcpp-ev3/libcpp-ev3-standalone.a
git checkout hrp2/sdk/workspace/hrp2
git checkout hrp2/sdk/workspace/uImage
