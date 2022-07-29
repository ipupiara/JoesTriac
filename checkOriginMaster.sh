#! /bin/sh
git diff --name-status master origin/master
echo "--->"
echo "git local HEAD commit ID to be checked with github.com below from SSH connection"
echo " "
git rev-parse --short HEAD
git show-ref --head
git show --shortstat HEAD
echo "ATTENTION current Head on remote origin, check with above local head:"
echo " "
git ls-remote origin
