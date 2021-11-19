set tool_path_make_lib=%~1\opt\tc32\bin
goto :MAIN_MAKE_LIB



:MAIN_MAKE_LIB
REM make lib for B85
set file_path=%~2\project\tlsr_tc32\B85\%~3\drivers\lib\src
set lib_obj_name=libB85.a
set lib_save_path=%~2\chip\B85\drivers\lib\src
cd /d %~dp0
call read_cfg .\config\cfg.ini B85 lib
call :PROCESS_MAKE_LIB

REM make lib for B87
set file_path=%~2\project\tlsr_tc32\B87\%~3\drivers\lib\src
set lib_obj_name=libB87.a
set lib_save_path=%~2\chip\B87\drivers\lib\src
cd /d %~dp0
call read_cfg .\config\cfg.ini B87 lib
call :PROCESS_MAKE_LIB

REM make lib for B89
set file_path=%~2\project\tlsr_tc32\B89\%~3\drivers\lib\src
set lib_obj_name=libB89.a
set lib_save_path=%~2\chip\B89\drivers\lib\src
cd /d %~dp0
call read_cfg .\config\cfg.ini B89 lib
call :PROCESS_MAKE_LIB

goto :END


:PROCESS_MAKE_LIB
set mk_lib_temp= 
setlocal EnableDelayedExpansion
for /f %%i in (.\~cfgTmp.txt) do (
set mk_lib_temp=!mk_lib_temp!%file_path%\%%i.o 
)
REM echo !mk_lib_temp!
%tool_path_make_lib%\tc32-elf-ar.exe -r %file_path%\%lib_obj_name% !mk_lib_temp!
if exist %file_path%\%lib_obj_name% (
call %~dp0.\show_info "Make %lib_obj_name% successfully." 
copy %file_path%\%lib_obj_name% %lib_save_path%\%lib_obj_name%
copy %file_path%\%lib_obj_name% %~dp0.\config\%lib_obj_name%
) else (
call %~dp0.\show_info "Make %lib_obj_name% Failed." 
)
if exist %lib_save_path%\%lib_obj_name% (
for /f %%i in (%~dp0.\~cfgTmp.txt) do (
if exist %lib_save_path%\%%i.c del /s /q /f %lib_save_path%\%%i.c
)
)
cd /d %~dp0
setlocal DisableDelayedExpansion
goto :END


:END



