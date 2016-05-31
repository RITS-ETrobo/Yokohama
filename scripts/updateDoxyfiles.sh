#!/bin/sh

# This script is for updating result of Doxygen.

# Remove old files and directories
OUTPUT_PATH=hrp2/sdk/html
rm -rf ${OUTPUT_PATH}

# Execute doxygen
echo "START: Updating the documents by Doxygen and Graphviz."
echo ""

doxygen ./scripts/workspace.Doxyfile 1>doxygen.log 2>&1
if [ ! -e doxygen.log ]; then
    echo "ERROR: Perhaps, Doxygen and/or Graphviz are not installed."
    exit 2
fi

./scripts/move_file.sh doxygen.log ${OUTPUT_PATH}/..

echo "END  : Updated the documents."
echo "       Please open \"hrp2/sdk/html/index.html\" in Web browser."
echo ""

if [ "${1}" = "Commit" ]
then
    echo "$1"

    # Commit files
    git add ${OUTPUT_PATH}
    git commit -am "#412 Update doxyfiles"
fi

exit 0
