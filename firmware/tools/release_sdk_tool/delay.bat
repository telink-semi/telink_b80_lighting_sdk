goto :MAIN

:delayUnit
for /l %%i in (1,1,250) do (
if %%i equ 1001 echo %%i
)
goto :END


:MAIN
for /l %%i in (1,1,%~1) do (
call :delayUnit
)
:END