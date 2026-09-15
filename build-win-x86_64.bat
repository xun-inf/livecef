@echo off
rem Qt toolchain path: use QTDIR environment variable if set, otherwise fall back to the default below
if not defined QTDIR set "QTDIR=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64"

cmake -S . ^
-B .build/windows.x86_64 ^
-A x64 ^
-DPROJECT_ARCH=x86_64 ^
-DBUILD_DEMO=ON ^
-DQT_SDK_DIR:PATH="%QTDIR%" ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86_64" ^
%*

cmake --build .build/windows.x86_64 --config Release