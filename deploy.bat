@echo off

set OUTPUT_DIR=deploy
set EXE_PATH=release\hyne.exe
set LIB_DIR=%QTDIR%\bin
set LIBS=Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll Qt5WinExtras.dll icudt53.dll icuin53.dll icuuc53.dll
set MINGW_LIBS=libgcc_s_dw2-1.dll libstdc++-6.dll libwinpthread-1.dll

set QT_TR_DIR=%QTDIR%\translations
set LANGUAGES=fr ja

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

for %%l in (%LIBS%) do (
    xcopy /y %LIB_DIR%\%%l %OUTPUT_DIR%
)

for %%l in (%MINGW_LIBS%) do (
    xcopy /y %LIB_DIR%\%%l %OUTPUT_DIR%
)

for %%l in (%LANGUAGES%) do (
    xcopy /y %QT_TR_DIR%\qt_%%l.qm %OUTPUT_DIR%
)

xcopy /y %EXE_PATH% %OUTPUT_DIR%

exit 0

upx %OUTPUT_DIR%\*.dll %OUTPUT_DIR%\*.exe

exit 0
