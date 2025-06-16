@echo off

:: Path to your correct Qt installation
set QT_ROOT=C:\msys64\mingw64

:: Prepend Qt bin folder to PATH
set PATH=%QT_ROOT%\bin;%PATH%

:: Run your app from the current folder
SpectraForge.exe
