# - FindPthreadStubs
#
# Copyright (C) 2015-2016 Valve Corporation
# Copyright (C) 2015-2016 LunarG, Inc.

find_package(PkgConfig)

pkg_check_modules(PC_PTHREADSTUBS QUIET pthread-stubs)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PthreadStubs DEFAULT_MSG
    PC_PTHREADSTUBS_FOUND)

set(PTHREADSTUBS_INCLUDE_DIRS "")
set(PTHREADSTUBS_LIBRARIES "")
