#!/usr/bin/env bash

# This script is for updating result of Doxygen and merging all branches.

git fetch --prune origin
git config --global push.default upstream

# Remove `hackev/cpp/tasks/412` branch
./scripts/remove_branch.sh issues/412

# Create `issues/412` branch from `hackev/cpp/develop`
./scripts/renew_branch.sh issues/412 hackev/cpp/develop

# Update docs
./scripts/updateDoxyfiles.sh Commit
if [ $? -ne 0 ]; then
    exit $2
fi

git push origin issues/412:issues/412

# Merge branch into `hackev/cpp/develop` from `issues/412`
./scripts/merge_branch.sh hackev/cpp/develop issues/412

# Remove `issues/412` branch
./scripts/remove_branch.sh issues/412

exit 0
