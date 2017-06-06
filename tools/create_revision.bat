@ECHO off

set REVFILE=../src/version_rev.h
set VERSION_REV=0
for /f "eol=; tokens=1,2,3,4" %%x in ('git -C ../ rev-list --count master') do (
  set VERSION_REV=%%x
)

set STR=#define VCS_REVISION %VERSION_REV%
set IS_FIND=1
for /f "delims=" %%a in ('find /i "%STR%" ^< %REVFILE%') do (
  set str2=%%a 
  set IS_FIND=0
)

if %IS_FIND% equ 1 (
  echo %STR%>%REVFILE%
)
