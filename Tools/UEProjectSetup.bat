@echo off

for /d %%i in ("C:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set ueTempPath="%%i\Engine\Templates"
        goto :main
    )
)
for /d %%i in ("D:\Program Files\Epic Games\UE_5.*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set ueTempPath="%%i\Engine\Templates"
        goto :main
    )
)

color c
echo Unreal Templates not found.
pause
exit

:main

set true=1 EQU 1
set false=0 EQU 1
echo Settingup heavy stuffs...

:: Starting Contents
if %false% robocopy "%ueTempPath%\..\Samples\StarterContent\Content" "%~dp0..\Content" /E /NFL /NDL /NJH /NJS /NC /NS

:: Characters
if %false% robocopy "%ueTempPath%\TemplateResources\High\Characters\Content" "%~dp0..\Content\Characters" /E /NFL /NDL /NJH /NJS /NC /NS

:: LevelPrototyping
if %false% robocopy "%ueTempPath%\TemplateResources\High\LevelPrototyping\Content" "%~dp0..\Content\LevelPrototyping" /E /NFL /NDL /NJH /NJS /NC /NS

:: Plugins
if not exist "%~dp0..\Plugins\pluginInstalled.txt" powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0PluginsSetup.ps1"


pause
