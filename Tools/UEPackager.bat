@echo off


for /d %%i in ("C:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set builder="%%i\Engine\Build\BatchFiles\RunUAT.bat"
        goto :main
    )
)
for /d %%i in ("D:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set builder="%%i\Engine\Build\BatchFiles\RunUAT.bat"
        goto :main
    )
)

color c
echo Unreal Engine Build.bat not found.
pause
exit


:main
md %~dp0..\GameBuild
set projExt=*.uproject

pushd "%~dp0.."
for %%a in (%projExt%) do set projName=%%a
popd


%builder% BuildCookRun ^
 -project="%~dp0..\%projName%" ^
 -noP4 ^
 -platform=Win64 ^
 -clientconfig=Shipping ^
 -cook -allmaps -build -stage -pak -archive ^
 -archivedirectory="%~dp0..\GameBuild" ^
 -verbose 

:: > GameBuild\build_log.txt 2>&1

pause