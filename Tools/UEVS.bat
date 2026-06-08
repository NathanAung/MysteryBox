@echo off

for /d %%i in ("C:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set unrealBuilder="%%i\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
        goto :main
    )
)
for /d %%i in ("D:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set unrealBuilder="%%i\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
        goto :main
    )
)

color c
echo UnrealBuildTool not found.
pause
exit

:main
set projExt=*.uproject

pushd "%~dp0.."
for %%a in (%projExt%) do set projName=%%a
popd

:: 1. Changed -vscode to -projectfiles to generate a .sln file
%unrealBuilder% -ProjectFiles -Project="%~dp0..\%projName%" -Game -Engine

:: 2. Launch the newly generated Visual Studio solution file
pushd "%~dp0.."
for %%s in (*.sln) do start "" "%%s"
popd

exit
