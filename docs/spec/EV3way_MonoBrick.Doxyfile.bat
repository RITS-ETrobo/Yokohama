if "%1" EQU "" (
    %1 = "."
)

cd %~1
set TARGET_PATH=html
rmdir /s /q %TARGET_PATH%
doxygen EV3way_MonoBrick.Doxyfile
cd %TARGET_PATH%
git add .
