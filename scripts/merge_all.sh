#!/usr/bin/env bash
git fetch --prune origin
git config --global push.default upstream

./scripts/merge_branch.sh hackev/cpp/release
./scripts/merge_branch.sh hackev/cpp/develop hackev/cpp/release
./scripts/merge_branch.sh hackev/cpp/master hackev/cpp/release
