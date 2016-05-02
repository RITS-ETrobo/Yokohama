#!/usr/bin/env bash
git fetch --prune origin
git config --global push.default upstream

git checkout -b hackev/cpp/tasks/412 origin/hackev/develop
git checkout hackev/cpp/tasks/412
git pull

./scripts/updateDoxyfiles.sh Commit
git push

./scripts/merge_branch.sh hackev/cpp/release hackev/cpp/tasks/412

./scripts/merge_all.sh
