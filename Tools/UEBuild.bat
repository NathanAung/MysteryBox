@echo off

for /d %%i in ("C:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set compiler="%%i\Engine\Build\BatchFiles\Build.bat"
        goto :main
    )
)
for /d %%i in ("D:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set compiler="%%i\Engine\Build\BatchFiles\Build.bat"
        goto :main
    )
)

color c
echo Unreal Engine Build.bat not found.
pause
exit

:main
set projExt=*.uproject

pushd "%~dp0.."
for %%a in (%projExt%) do set projName=%%a
popd

call %compiler% %projName:~0,-9%Editor Win64 Development "%~dp0..\%projName%" -waitmutex

pause