#!/usr/bin/env bash

# This script is for removing the selected branch from local repogitory and copying from the remote repogitory.

if [ $# -eq 0 ]; then
    echo "Illegal parameter" 1>&2
    echo "Enter branch name" 1>&2
    exit 1
fi

# Remove local branch
./scripts/remove_local_branch.sh $1

# Create branch
if [ $# -eq 1 ]; then
    git checkout -b $1 origin/$1
    exit 0
fi

git checkout -b $1 origin/$2

exit 0
