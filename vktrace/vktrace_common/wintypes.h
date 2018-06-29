/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Jon Ashburn <jon@lunarg.com>
 **************************************************************************/
#pragma once

#if defined(__linux__) || defined(__APPLE__)
#include <stdint.h>
#include <stddef.h>
typedef void *LPVOID;
typedef void *PVOID;
typedef void VOID;
typedef char CHAR;
typedef char TCHAR;
typedef long LONG;
typedef unsigned long ULONG;
typedef int BOOL;
typedef size_t SIZE_T;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned char *PBYTE;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned short WORD;
typedef DWORD *DWORD_PTR;
typedef DWORD *PDWORD;
typedef DWORD_PTR *PDWORD_PTR;
typedef int32_t INT32;
typedef int64_t LONG64;
typedef uint64_t ULONG64;
typedef const char *PCSTR;
typedef const wchar_t *PCWSTR;
#if !defined(MAX_PATH)
#include <limits.h>
#if !defined(PATH_MAX)
#define MAX_PATH 4096
#else
#define MAX_PATH PATH_MAX
#endif
#endif
#if !defined(TRUE)
#define TRUE 1
#endif
#if !defined(FALSE)
#define FALSE 0
#endif

#elif WIN32
#include <windows.h>
#endif
