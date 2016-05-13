#!/usr/bin/env bash

./scripts/create_env.sh
rm -rf hrp2/sdk/bin
./scripts/make_all.sh app
./scripts/make_all.sh img
