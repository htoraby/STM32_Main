@ECHO off

set REVFILE=../src/version_rev.h
set VERSION_REV=0
for /f "eol=; tokens=1,2,3,4" %%x in ('git -C ../ rev-list --count master') do (
  set VERSION_REV=%%x
)

set str=#define VCS_REVISION %VERSION_REV%
echo %str% > %REVFILE%
