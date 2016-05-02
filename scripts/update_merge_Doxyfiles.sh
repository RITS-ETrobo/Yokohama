#!/usr/bin/env bash
git fetch --prune origin
git config --global push.default upstream

# Remove `hackev/cpp/tasks/412` branch
./scripts/remove_branch.sh hackev/cpp/tasks/412

# Create `hackev/cpp/tasks/412` branch from `hackev/cpp/develop`
./scripts/renew_branch.sh hackev/cpp/tasks/412 hackev/cpp/develop

# Update docs
./scripts/updateDoxyfiles.sh Commit
git push origin hackev/cpp/tasks/412:hackev/cpp/tasks/412

# Merge branch into `hackev/cpp/release` from `hackev/cpp/tasks/412`
./scripts/merge_branch.sh hackev/cpp/release hackev/cpp/tasks/412

# Remove `hackev/cpp/tasks/412` branch
./scripts/remove_branch.sh hackev/cpp/tasks/412

# Merge all branches
./scripts/merge_all.sh

exit 0
