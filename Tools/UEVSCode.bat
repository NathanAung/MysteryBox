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

%unrealBuilder% -ProjectFiles -Project="%~dp0..\%projName%" -Game -Engine -vscode


for %%i in (code.cmd) do for %%j in ("%%~dp$PATH:i\..") do set "VSCODE_PARENT=%%~fj"
start "" "%VSCODE_PARENT%\Code.exe" ..

exit

