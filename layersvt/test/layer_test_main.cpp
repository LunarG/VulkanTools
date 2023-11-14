/*
 * Copyright (c) 2023 The Khronos Group Inc.
 * Copyright (c) 2023 Valve Corporation
 * Copyright (c) 2023 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Author: Christophe Ricico <christophe@lunarg.com>
 */

#include <gtest/gtest.h>

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(_WIN32) && !defined(NDEBUG)
#include <crtdbg.h>
#endif

int main(int argc, char **argv) {
    int result;

#if defined(_WIN32)
#if !defined(NDEBUG)
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
    // Avoid "Abort, Retry, Ignore" dialog boxes
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
    ::testing::InitGoogleTest(&argc, argv);

    result = RUN_ALL_TESTS();

    return result;
}
