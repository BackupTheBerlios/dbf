@echo off
:: compile.bat
:: Compiles dbf on Microsoft Windows System
:: You need nmake and a working Visual Studio installation
:: Copyright (c) 2003, Bjoern Berg, clergyman@gmx.de

echo ====== Changing directory
@cd src

echo ====== Building dbf.exe
if exist Debug goto COMPILEDYET
@nmake /f dbf.mak

echo ====== Going back to root
@cd ..

echo ====== Compilation complete, please check src/Debug
goto ENDBATCH

:COMPILEDYET
echo ERROR: ** dbf was compiled before
echo Please remove the Directory src/Debug
echo Then run compile.bat again
@cd ..
goto ENDBATCH

:NO_NMAKE
echo ERROR: ** nmake not found
goto ENDBATCH

:NO_LINKER
echo ERROR: ** linker not found
goto ENDBATCH

:ENDBATCH
echo ====== Terminating process
:@exit 