#!/usr/bin/env bash
git fetch --prune origin
git config --global push.default upstream

# Merge all branches
./scripts/merge_branch.sh hackev/cpp/release
./scripts/merge_branch.sh hackev/cpp/develop hackev/cpp/release
./scripts/merge_branch.sh hackev/cpp/master hackev/cpp/release

# Finalize
git checkout hackev/cpp/develop
./scripts/remove_local_branch.sh hackev/cpp/release
./scripts/remove_local_branch.sh hackev/cpp/master

exit 0
