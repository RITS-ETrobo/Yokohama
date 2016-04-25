#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Enter branch name" 1>&2
    exit 1
fi

# update branch to up-to-date.
git checkout -b $1 origin/$1
git checkout $1
git pull

if [ $# -ne 2 ]; then
    exit 0
fi

git merge $2
git push origin $1:$1
exit 0
