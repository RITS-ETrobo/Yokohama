#!/usr/bin/env bash

# Create environment
./scripts/create_env.sh

# Remove binary files
rm -rf hrp2/sdk/bin

# Execute the make command
./scripts/make_all.sh app
./scripts/make_all.sh img
