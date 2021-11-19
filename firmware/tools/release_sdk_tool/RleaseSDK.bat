@echo off
set TOOL_PATH_RLEASE_SDK=C:\TelinkSDK
set Project_Path_Rlease_SDK=..\..\
set make_lib_compile_option=UART_Demo

set Project_Path_B85=%Project_Path_Rlease_SDK%\project\tlsr_tc32\B85
set Project_Path_B87=%Project_Path_Rlease_SDK%\project\tlsr_tc32\B87
set Project_Path_B89=%Project_Path_Rlease_SDK%\project\tlsr_tc32\B89

goto :MAIN


:TEST_COMPILE
call %~dp0.\show_info "Compile Test B85" 1
set Project_Path_Compile=%Project_Path_B85%
set Test_Compile_CompileOption=B85_Driver_Demo
call :COMPILE_PROCESS
call %~dp0.\show_info "Compile Test B85 Completed!" 4

call %~dp0.\show_info "Compile Test B87" 1
set Project_Path_Compile=%Project_Path_B87%
set Test_Compile_CompileOption=B87_Driver_Demo
call :COMPILE_PROCESS
call %~dp0.\show_info "Compile Test B87 Completed!" 4

call %~dp0.\show_info "Compile Test B89" 1
set Project_Path_Compile=%Project_Path_B89%
set Test_Compile_CompileOption=B89_Driver_Demo
call :COMPILE_PROCESS
call %~dp0.\show_info "Compile Test B89 Completed!" 4

goto :END

:COMPILE_PROCESS
call %~dp0.\tool rd_options %Project_Path_Compile% .cproject %~dp0.\~cfgTmp.txt
for /f %%i in (%~dp0.\~cfgTmp.txt) do (
call %~dp0.\show_info "Compile %%i." 3
if exist %Project_Path_Compile%\%%i (rmdir /s /q %Project_Path_Compile%\%%i)
call compile %TOOL_PATH_RLEASE_SDK% %Project_Path_Compile% %Test_Compile_CompileOption% %%i
if exist %Project_Path_Compile%\%%i\*.bin (
call %~dp0.\show_info "PASS" 2
) else (
call %~dp0.\show_info "FAIL" 2
pause
)
if exist %Project_Path_Compile%\%%i (rmdir /s /q %Project_Path_Compile%\%%i)
)
goto :END

:MAKE_LIB
call %~dp0.\show_info "Make library" 1
copy %~dp0.\config\libB85.a %Project_Path_Rlease_SDK%\chip\B85\drivers\libB85.a
copy %~dp0.\config\libB87.a %Project_Path_Rlease_SDK%\chip\B87\drivers\libB87.a
copy %~dp0.\config\libB89.a %Project_Path_Rlease_SDK%\chip\B89\drivers\libB89.a
set build_prj_Rlease_SDK=%make_lib_compile_option%

call %~dp0.\show_info "Compile B85 %build_prj_Rlease_SDK%." 3
set Project_Path_MakeLib_Complie=%Project_Path_B85%
set MakeLib_CompileOption=B85_Driver_Demo
call %~dp0.\compile %TOOL_PATH_RLEASE_SDK% %Project_Path_MakeLib_Complie% %MakeLib_CompileOption% %build_prj_Rlease_SDK%
if exist %Project_Path_MakeLib_Complie%\%build_prj_Rlease_SDK%\*.bin (
call %~dp0.\show_info "PASS" 2
) else (
goto :MAKE_LIB_ERROR
)

call %~dp0.\show_info "Compile B87 %build_prj_Rlease_SDK%." 3
set Project_Path_MakeLib_Complie=%Project_Path_B87%
set MakeLib_CompileOption=B87_Driver_Demo
call %~dp0.\compile %TOOL_PATH_RLEASE_SDK% %Project_Path_MakeLib_Complie% %MakeLib_CompileOption% %build_prj_Rlease_SDK%
if exist %Project_Path_MakeLib_Complie%\%build_prj_Rlease_SDK%\*.bin (
call %~dp0.\show_info "PASS" 2
) else (
goto :MAKE_LIB_ERROR
)

call %~dp0.\show_info "Compile B89 %build_prj_Rlease_SDK%." 3
set Project_Path_MakeLib_Complie=%Project_Path_B89%
set MakeLib_CompileOption=B89_Driver_Demo
call %~dp0.\compile %TOOL_PATH_RLEASE_SDK% %Project_Path_MakeLib_Complie% %MakeLib_CompileOption% %build_prj_Rlease_SDK%
if exist %Project_Path_MakeLib_Complie%\%build_prj_Rlease_SDK%\*.bin (
call %~dp0.\show_info "PASS" 2
) else (
goto :MAKE_LIB_ERROR
)

call %~dp0.\make_lib %TOOL_PATH_RLEASE_SDK% %Project_Path_Rlease_SDK% %build_prj_Rlease_SDK%
call %~dp0.\show_info "Make library Completed!" 4
goto :END

:MAKE_LIB_ERROR
call %~dp0.\show_info "Make library Failed!" 4
call %~dp0.\show_info "FAIL" 2
pause
exit
goto :END


:REMOVE_COMPILE_OPTIONS

call %~dp0.\show_info "Remove Compile Options" 1

call %~dp0.\show_info "Remove B85 Compile Options" 3
call %~dp0.\tool rm_options %Project_Path_B85% %~dp0.\config\cfg.ini B85 .\.cproject
call %~dp0.\show_info "Remove B85 Compile Options Completed!" 4

call %~dp0.\show_info "Remove B87 Compile Options" 3
call %~dp0.\tool rm_options %Project_Path_B87% %~dp0.\config\cfg.ini B87 .\.cproject
call %~dp0.\show_info "Remove B87 Compile Options Completed!" 4

call %~dp0.\show_info "Remove B89 Compile Options" 3
call %~dp0.\tool rm_options %Project_Path_B89% %~dp0.\config\cfg.ini B89 .\.cproject
call %~dp0.\show_info "Remove B89 Compile Options Completed!" 4
goto :END

:REMOVE_FILES
call %~dp0.\show_info "Remove Files" 1
call %~dp0.\tool rm_files %Project_Path_Rlease_SDK% %~dp0.\config\cfg.ini B85
call %~dp0.\tool rm_files %Project_Path_Rlease_SDK% %~dp0.\config\cfg.ini B87
call %~dp0.\tool rm_files %Project_Path_Rlease_SDK% %~dp0.\config\cfg.ini B89

call %~dp0.\show_info "Remove Files Completed!" 4
goto :END

:REMOVE_TEMP_FILE
if exist %~dp0.\~workspace (rmdir /s /q %~dp0.\~workspace)
if exist %~dp0.\customize_chip_profile (rmdir /s /q %~dp0.\customize_chip_profile)
if exist %~dp0.\~cfgTmp.txt (del /s /q /f %~dp0.\~cfgTmp.txt)
goto :END

:MAIN
call :REMOVE_TEMP_FILE
echo ------BEGIN(%date%  %time%)--------
call :MAKE_LIB
call :REMOVE_COMPILE_OPTIONS
call :REMOVE_FILES
call :TEST_COMPILE
echo ------END(%date%  %time%)---------
call :REMOVE_TEMP_FILE
pause

:END