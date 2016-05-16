#!/usr/bin/env bash

# This script is for moving the selected file or directory.

if [ $# -ne 2 ]; then
    echo "Illegal parameter" 1>&2
    echo "Counts of entered argument is $#." 1>&2
    echo "For executing this script, need 2 arguments." 1>&2
    echo "The meaning of 1st parameter is the target file or directory." 1>&2
    echo "The meaning of 2nd parameter is the source path." 1>&2
    exit 1
fi

# Confirm that the selected one is existing.
if [ ! -e $1 ]; then
    exit 0
fi

# Remove file if the selected one is file.
if [ -f $1 ]; then
    mv $1 $2
    exit 0
fi

# Remove directory if the selected one is directory.
if [ -d $1 ]; then
    mv $1 $2
    exit 0
fi

# Exception
echo "We could not move $1 to $2."
exit 1
