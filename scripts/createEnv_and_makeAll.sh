#!/usr/bin/env bash

# Create environment
./scripts/create_env.sh

# Execute the make command
./scripts/make_all.sh app
./scripts/make_all.sh img
