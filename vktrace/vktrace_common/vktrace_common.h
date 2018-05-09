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

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)

#define VKTRACER_EXPORT __attribute__((visibility("default")))
#define VKTRACER_STDCALL
#define VKTRACER_CDECL
#define VKTRACER_EXIT void
#define VKTRACER_ENTRY void __attribute__((constructor))
#define VKTRACER_LEAVE void __attribute__((destructor))

#endif

#define ROUNDUP_TO_2(_len) ((((_len) + 1) >> 1) << 1)
#define ROUNDUP_TO_4(_len) ((((_len) + 3) >> 2) << 2)
#define ROUNDUP_TO_8(_len) ((((_len) + 7) >> 3) << 3)
#define ROUNDUP_TO_16(_len) ((((_len) + 15) >> 4) << 4)

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

static const uint32_t  INVALID_BINDING_INDEX = UINT32_MAX;
// Windows needs 64 bit versions of fseek and ftell
#if defined(WIN32)
#define Ftell _ftelli64
#define Fseek _fseeki64
#else
#define Ftell ftell
#define Fseek fseek
#endif

// Enviroment variables used by vktrace/replay

// VKTRACE_PMB_ENABLE env var enables tracking of PMB if the value is 1.
// Other values disable PMB tracking. If this env var is undefined, PMB
// tracking is enabled. The env var is set by the vktrace program to
// communicate the --PMB arg value to the trace layer.
#define VKTRACE_PMB_ENABLE_ENV "VKTRACE_PMB_ENABLE"

// _VKTRACE_PMB_TARGET_RANGE_SIZE env var specifies the minimum size of
// memory objects tracked by pmb. vktrace only tracks changes to memory
// objects of whose size > this value. If this env var is not defined,
// pmb tracks all mapped memory - this is the default option. There is
// typically no need for a user to specify this env variable, it's
// primary use is for debugging the trace layer.
#define _VKTRACE_PMB_TARGET_RANGE_SIZE_ENV "_VKTRACE_PMB_TARGET_RANGE_SIZE"

// VKTRACE_PAGEGUARD_ENABLE_READ_PMB env var enables read PMB support.
// It is only supported on Windows. If PMB data changes comes from the
// GPU side, PMB tracking does not usually capture those changes. This
// env var is used to enable capture of such GPU initiated PMB data
// changes.
#define VKTRACE_PAGEGUARD_ENABLE_READ_PMB_ENV "VKTRACE_PAGEGUARD_ENABLE_READ_PMB"

// VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS env var enables post
// processing in read PMB support. It is only supported on Windows.
// PMB processing will miss writes following reads if writes occur
// on the same page as a read. This env var is used to enable post
// processing to fix missed pmb writes.
#define VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS_ENV "VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS"

// VKTRACE_PAGEGUARD_ENABLE_LAZY_COPY_ENV env var enables lazy copy in PMB
// support. It is only supported on Windows.
//
// The lazy copy method is first used for some title to improve the capture
// speed, the target title heavily use memory map/unmap to transfer data,
// not like other titles rely on read/write persistently mapped memory. It
// cause the title capture speed extremely slow, the reason is it's needed
// to sync the mapped memory to page guard shadow memory when calling
// vkMapMemory even the target title only change small portion of whole
// memory range.
//
// The lazy copy method delay the sync memcopy and only do it for some of
// pages when target title really read/write to them. At that time, page
// guard handler capture the target title read/write the data and only
// transfer related page, this way fix the capture speed problem for that
// target title.
//
// So far, most vulkan titles like this commit targeted use memory as
// persistent mapped buffer, once map, the data transfer rely on read/write
// the persistent mapped memory, not rely on the pair of map/unmap. For such
// titles, lazy copy affect the capture speed not much. And from the current
// target title, some corruption can be found at some location during
// capture/playback when enable lazy copy, but no this problem if disable it
//, the reason is not verified but there's possibility it relate to page guard
// multithreading limitation. For such titles, lazy copy move the title data
// loading process from the loading splash to other location and spread the
// process in game playing.
//
// So here, the env variable give user the choice to enable/disable the lazy
// copy process. If the variable is not defined, the lazy copy will be
// disabled.
#define VKTRACE_PAGEGUARD_ENABLE_LAZY_COPY_ENV "VKTRACE_PAGEGUARD_ENABLE_LAZY_COPY"

// VKTRACE_TRIM_TRIGGER env var is set by the vktrace program to
// communicate the --TraceTrigger command line argument to the
// trace layer.
#define VKTRACE_TRIM_TRIGGER_ENV "VKTRACE_TRIM_TRIGGER"

// _VKTRACE_VERBOSITY env var is set by the vktrace program to
// communicate verbosity level to the trace layer. It is set to
// one of "quiet", "errors", "warnings", "full", or "debug".
#define _VKTRACE_VERBOSITY_ENV "_VKTRACE_VERBOSITY"
