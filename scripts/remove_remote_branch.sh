#!/usr/bin/env bash

# This script is for removing the selected branch from remote repogitories.

if [ $# -eq 0 ]; then
    echo "Illegal parameter" 1>&2
    echo "Enter branch name" 1>&2
    exit 1
fi

# Remove the selected branch from remote repository.
git push origin :$1

exit 0
