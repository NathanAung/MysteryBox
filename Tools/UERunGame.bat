@echo off


for /d %%i in ("C:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set unreal="%%i\Engine\Binaries\Win64\UnrealEditor.exe"
        goto :main
    )
)
for /d %%i in ("D:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set unreal="%%i\Engine\Binaries\Win64\UnrealEditor.exe"
        goto :main
    )
)

color c
echo UnrealEditor not found.
pause
exit


:main
set projExt=*.uproject

pushd "%~dp0.."
for %%a in (%projExt%) do set projName=%%a
popd


%unreal% "%~dp0..\%projName%" -ExecCmds="Scalability 3" -log -game -windowed -resx=1280 -resy=720