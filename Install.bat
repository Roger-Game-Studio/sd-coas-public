:: Hokkins: файлик для автоматизации процесса установки игровых скриптов с гитхаба =)

@echo off
chcp 65001  >nul

::  пути до папок -->
set "dir_modules=%~dp0\modules\"
set "dir_program=%~dp0\PROGRAM\"
set "dir_resINI=%~dp0\RESOURCE\INI\"
set "dir_save=%~dp0\SAVE\"


:: Hokkins: проверяем папки, если есть удаляем -->
IF EXIST "%dir_modules%" (
    RD /S/Q "%dir_modules%"
) ELSE (
    echo folder "modules" missing
)
IF EXIST "%dir_program%" (
    RD /S/Q "%dir_program%"
) ELSE (
    echo folder "PROGRAM" missing
)
IF EXIST "%dir_resINI%" (
    RD /S/Q "%dir_resINI%"
) ELSE (
    echo folder "RESOURCE\INI" missing
)
IF EXIST "%dir_save%" (
    RD /S/Q "%dir_save%"
) ELSE (
    echo folder "SAVE" missing
)


:: Hokkins: проверяем файлы, если есть удаляем -->
IF EXIST "%~dp0\*.txt" (
    erase "%~dp0\*.txt"
) ELSE (
    echo ".txt" files are missing
)
IF EXIST "%~dp0\*.ini" (
    erase "%~dp0\*.ini"
) ELSE (
    echo ".ini" files are missing
)
IF EXIST "%~dp0\*.exe" (
    erase "%~dp0\*.exe"
) ELSE (
    echo ".exe" files are missing
)
IF EXIST "%~dp0\*.dll" (
    erase "%~dp0\*.dll"
) ELSE (
    echo ".dll" files are missing
)
IF EXIST "%~dp0\*.dat" (
    erase "%~dp0\*.dat"
) ELSE (
    echo ".dat" files are missing
)
IF EXIST "%~dp0\*." (
    erase "%~dp0\*."
) ELSE (
    echo other files are missing
)
IF EXIST "%~dp0\RESOURCE\animation\*.ani" (
    erase "%~dp0\RESOURCE\animation\*.ani"
) ELSE (
    echo "RESOURCE\animation\.ani" files are missing
)
IF EXIST "%~dp0\RESOURCE\foam\islands\*.ini" (
    erase "%~dp0\RESOURCE\islands\*.ini"
) ELSE (
    echo "RESOURCE\foam\islands\.ini" files are missing
)
IF EXIST "%~dp0\RESOURCE\foam\locations\*.ini" (
    erase "%~dp0\RESOURCE\locations\*.ini"
) ELSE (
    echo "RESOURCE\foam\locations\.ini" files are missing
)
:: 18.06.22 Hokkins: чистим текстуры UI и HUD -->
IF EXIST "%~dp0\RESOURCE\Textures\BATTLE_INTERFACE\*.tx" (
    erase "%~dp0\RESOURCE\Textures\BATTLE_INTERFACE\*.tx"
) ELSE (
    echo "RESOURCE\Textures\BATTLE_INTERFACE\*.tx" files are missing
)
IF EXIST "%~dp0\RESOURCE\Textures\INTERFACES\*.tx" (
    erase "%~dp0\RESOURCE\Textures\INTERFACES\*.tx"
) ELSE (
    echo "RESOURCE\Textures\INTERFACES\*.tx" files are missing
)
:: <--


:: Hokkins: начинаем процесс установки скриптов и конфигурационных файлов -->
echo D | xcopy /S/Q/Y "%~dp0\sd-coas-public" "%~dp0\"
RD /S/Q "%~dp0\sd-coas-public"
erase "%~dp0\Install.bat"

pause