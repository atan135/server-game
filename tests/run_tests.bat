@echo off
echo Running Logger Tests...
echo.

cd ..\build\bin

if exist LoggerTest.exe (
    echo Found LoggerTest.exe, running tests...
    LoggerTest.exe
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo All tests passed! ✅
    ) else (
        echo.
        echo Some tests failed! ❌
    )
) else (
    echo LoggerTest.exe not found. Please build the project first.
    echo Run: cmake --build .. from the build directory
)

echo.
pause

