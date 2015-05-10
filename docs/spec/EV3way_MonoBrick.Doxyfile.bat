@echo off

rem Check "Graphviz installation".
set PATH_GRAPHVIZ="C:\Graphviz\bin\g*.exe"

if not exist %PATH_GRAPHVIZ% (
    echo "Graphviz was not installed in C:\Graphviz"
    exit 0
)

rem When this file was executed directly, set \docs\spec to current path.
set PATH_CURRENT=%~dp0
cd %PATH_CURRENT%

rem Remove doxygen files.
set PATH_TARGET=html
md %PATH_TARGET%
cd %PATH_TARGET%
del /s /q *
cd ..

rem Generate doxygen files.
doxygen EV3way_MonoBrick.Doxyfile

cd %PATH_TARGET%
if not exist "index.html" (
  echo "Generated files not exist."

  rem Revert all changes.
  git checkout .
  exit 0
)

git add *
