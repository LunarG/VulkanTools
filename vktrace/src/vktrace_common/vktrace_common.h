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

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "vktrace_platform.h"

#include "vktrace_memory.h"
#include "vktrace_tracelog.h"

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#endif

#if defined(WIN32)

#define VKTRACER_EXPORT __declspec(dllexport)
#define VKTRACER_STDCALL __stdcall
#define VKTRACER_CDECL __cdecl
#define VKTRACER_EXIT void __cdecl
#define VKTRACER_ENTRY void
#define VKTRACER_LEAVE void

#elif defined(PLATFORM_LINUX)

#define VKTRACER_EXPORT __attribute__ ((visibility ("default")))
#define VKTRACER_STDCALL
#define VKTRACER_CDECL
#define VKTRACER_EXIT void
#define VKTRACER_ENTRY void __attribute__ ((constructor))
#define VKTRACER_LEAVE void __attribute__ ((destructor))

#endif
