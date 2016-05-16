#!/bin/sh

# This script is for updating result of Doxygen.

# Remove old files and directories
OUTPUT_PATH=hrp2/sdk/html/
rm -rf ${OUTPUT_PATH}

# Execute doxygen
doxygen ./scripts/workspace.Doxyfile

echo "$1"

if [ "${1}" = "Commit" ]
then
    # Commit files
    git add ${OUTPUT_PATH}
    git commit -am "#412 Update doxyfiles"
fi

exit 0
