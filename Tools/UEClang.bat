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

%unrealBuilder% -mode=GenerateClangDatabase -project="%~dp0..\%projName%" MysteryBoxEditor Development Win64

copy /Y "D:\Program Files\Epic Games\UE_5.6\compile_commands.json" "%~dp0compile_commands.json"


for %%i in (code.cmd) do for %%j in ("%%~dp$PATH:i\..") do set "VSCODE_PARENT=%%~fj"
start "" "%VSCODE_PARENT%\Code.exe" ..

exit

