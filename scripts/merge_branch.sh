#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Enter branch name" 1>&2
    exit 1
fi

# update branch to up-to-date.
./scripts/renew_branch.sh $1

if [ $# -ne 2 ]; then
    exit 0
fi

git merge $2
git push origin $1:$1

exit 0
