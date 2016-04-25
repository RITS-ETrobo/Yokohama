#!/usr/bin/env bash
git fetch --prune origin

./merge_branch.sh hackev/cpp/release
./merge_branch.sh hackev/cpp/develop hackev/cpp/release
./merge_branch.sh hackev/cpp/master hackev/cpp/release
