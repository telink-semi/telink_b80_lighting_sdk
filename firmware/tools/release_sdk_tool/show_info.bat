@echo off
if index%~2 equ index1 (
echo [%time:~0,-3%]telink@ ################################################ 
echo [%time:~0,-3%]telink@ ## %~1
echo [%time:~0,-3%]telink@ ################################################
) else (
if index%~2 equ index2 (
echo [%time:~0,-3%]telink@ ## [ %~1 ] ##
echo [%time:~0,-3%]telink@
) else (
if index%~2 equ index3 (
echo [%time:~0,-3%]telink@
echo [%time:~0,-3%]telink@ ## %~1 
) else (
if index%~2 equ index4 (
echo [%time:~0,-3%]telink@ %~1 
echo [%time:~0,-3%]telink@
) else (
echo [%time:~0,-3%]telink@ %~1
)
)
)
)