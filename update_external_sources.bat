@echo off

set errorCode=0
set BUILD_DIR=%~dp0

set BASE_DIR=%BUILD_DIR%submodules
set JSONCPP_DIR=%BASE_DIR%\jsoncpp
set V_LVL_DIR=%BASE_DIR%\Vulkan-LoaderAndValidationLayers


git submodule update --init --recursive

:build_jsoncpp
   echo.
   echo Building %JSONCPP_DIR%
   cd  "%JSONCPP_DIR%"
   python amalgamate.py

   if not exist %JSONCPP_DIR%\dist\json\json.h (
      echo.
      echo JsonCPP Amalgamation failed to generate %JSONCPP_DIR%\dist\json\json.h
      set errorCode=1
   )
