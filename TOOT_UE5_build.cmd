@echo off
setlocal

::  Change this UE ROOT folder according to your environment
SET UE_FOLDER=C:\UnrealEngine


echo "++++++++=============================++++++++++++++"
echo  Current Dir:  %~dp0
echo  UE_FOLDER Dir:  %UE_FOLDER%
echo "++++++++=============================++++++++++++++"

:: echo %PROJECT%

 :: echo "++++++++=====================  For loop find *.pdb ========++++++++++++++"

 :: FOR    %%F in (Binaries\Win64\UnrealEditor-*.patch_*.pdb) do echo "%%~F"

 :: echo "++++++++=====================  For loop find *.pdb done ========++++++++++++++"


FOR /R %%F in (.\*.uproject) do SET PRJNAME=%%~nF
FOR /R %%F in (*.uproject) do SET PRJNAMEPATH=%%~F

echo %PRJNAME%
echo %PRJNAMEPATH%

echo "++++++++========== UE build UnrealEditor Dlls for:  %PRJNAME%.uproject    +===++++++++++++++"


call %UE_FOLDER%/Engine/Build/BatchFiles/Build.bat -Target="%PRJNAME%Editor Win64 Development" -project="%PRJNAMEPATH%"  -WaitMutex -FromMsBuild -architecture=x64


if %ERRORLEVEL% NEQ 0   pause

