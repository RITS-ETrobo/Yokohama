#!/usr/bin/env bash

# This script is for removing the selected file or directory.

if [ $# -ne 1 ]; then
    echo "Illegal parameter" 1>&2
    echo "For executing this script, need 1 argument." 1>&2
    echo "The meaning of parameter is the selected file or directory." 1>&2
    exit 1
fi

# Confirm that the selected one is existing.
if [ ! -e $1 ]; then
    exit 0
fi

# Remove file if the selected one is file.
if [ -f $1 ]; then
    rm $1
    exit 0
fi

# Remove directory if the selected one is directory.
if [ -d $1 ]; then
    rm -rf $1
    exit 0
fi

# Exception
echo "We could not remove $1."
exit 1
