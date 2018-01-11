@echo off

set errorCode=0
set BUILD_DIR=%~dp0
set BASE_DIR="%BUILD_DIR%external"
set REVISION_DIR="%BUILD_DIR%external_revisions"
set JSONCPP_DIR=%BASE_DIR%\jsoncpp

set /p JSONCPP_REVISION= < %REVISION_DIR%\jsoncpp_revision

echo JSONCPP_REVISION=%JSONCPP_REVISION%


:update_jsoncpp
   echo.
   echo Updating %JSONCPP_DIR%
   cd %JSONCPP_DIR%
   git fetch --all
   git checkout %JSONCPP_REVISION%

:build_jsoncpp
   echo.
   echo Building %JSONCPP_DIR%
   cd  %JSONCPP_DIR%
   python amalgamate.py

   if not exist %JSONCPP_DIR%\dist\json\json.h (
      echo.
      echo JsonCPP Amalgamation failed to generate %JSONCPP_DIR%\dist\json\json.h
      set errorCode=1
   )

