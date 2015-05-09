@echo off

rem Check "Graphviz installation".
set PATH_GRAPHVIZ="C:\Graphviz\bin\g*.exe"

if not exist %PATH_GRAPHVIZ% (
    echo "Graphviz was not installed in C:\Graphviz"
    exit 0
)

rem When this file was executed directly, set %1 to current path.
set PATH_CURRENT = %~1
if "%PATH_CURRENT%" EQU "" (
    set PATH_CURRENT = "."
)

rem Remove doxygen files.
set PATH_TARGET=html
md %PATH_TARGET%
cd %PATH_TARGET%
rd /s /q .
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
