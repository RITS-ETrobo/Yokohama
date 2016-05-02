#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Enter branch name" 1>&2
    exit 1
fi

git branch -D $1

exit 0
