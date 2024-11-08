@REM CMAKE WITH NINJA SELECTS CLANG INSTEAD 
@REM OF MSVC, I HAVE NO CLUE HOW TO CHANGE THIS TRIED EVERYTHING
@REM NOT EVERYTHING, BUT ALOT

@ECHO ON
ECHO "========HERE 1"
REM Path to vswhere.exe
SET VsWherePath="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

IF NOT EXIST %VsWherePath% (
    ECHO Error: vswhere.exe not found at %VsWherePath%
    EXIT /B 1
)
ECHO "========HERE 2"

REM Use vswhere.exe to get the installation path of the latest Visual Studio with VC tools
FOR /F "usebackq tokens=*" %%i IN (`%VsWherePath% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) DO SET "InstallPath=%%i"

ECHO "========HERE 3"

IF NOT DEFINED InstallPath (
    ECHO Error: No suitable Visual Studio installation found.
    EXIT /B 1
)

ECHO "========HERE 4"

REM Construct the path to vcvars64.bat
SET VCVarsPath="%InstallPath%\VC\Auxiliary\Build\vcvars64.bat"

ECHO "========HERE 5"

IF EXIST %VCVarsPath% (
    CALL %VCVarsPath%
) ELSE (
    ECHO Error: Could not find vcvars64.bat at %VCVarsPath%
    EXIT /B 1
)

ECHO "========HERE 6"

REM Your subsequent commands go here


@rem "if CC not set, ninja will use clang if clang is installed on the machine"
set CC=cl
call cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE -GNinja --check-system-vars
call cmake --build build
call .\build\unit_tests.exe
call .\build\unit_tests.exe --list-test-cases