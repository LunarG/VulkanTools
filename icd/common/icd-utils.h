/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
 * Author: Chia-I Wu <olvaffe@gmail.com>
 * Author: Chia-I Wu <olv@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 *
 */

#ifndef ICD_UTILS_H
#define ICD_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#if defined(PLATFORM_LINUX)
#include <strings.h> /* for ffs() */
#endif
#include "icd.h"

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#if defined(__GNUC__)
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define u_popcount(u) __builtin_popcount(u)
#define u_popcountll(u) __builtin_popcountll(u)

#define STATIC_ASSERT(expr) do {            \
    (void) sizeof(char[1 - 2 * !(expr)]);   \
} while (0)

#define U_CLAMP(val, min, max) \
    ((val) < (min) ? (min) : (val) > (max)? (max) : (val))

/**
 * Return true if val is power of two, or zero.
 */
static inline bool u_is_pow2(unsigned int val)
{
    return ((val & (val - 1)) == 0);
}

static inline int u_ffs(int val)
{
#if defined(PLATFORM_LINUX)
	return ffs(val);
#else
	return __lzcnt(val) + 1;
#endif
}

static inline unsigned int u_align(unsigned int val, unsigned alignment)
{
    assert(alignment && u_is_pow2(alignment));
    return (val + alignment - 1) & ~(alignment - 1);
}

static inline unsigned int u_minify(unsigned int val, unsigned level)
{
    return (val >> level) ? val >> level : 1;
}

static inline uint32_t u_fui(float f)
{
    union {
        float f;
        uint32_t ui;
    } u = { .f = f };

    return u.ui;
}

static inline float u_uif(uint32_t ui)
{
    union {
        float f;
        uint32_t ui;
    } u = { .ui = ui };

    return u.f;
}

static inline int u_iround(float f)
{
    if (f >= 0.0f)
        return (int) (f + 0.5f);
    else
        return (int) (f - 0.5f);
}

uint16_t u_float_to_half(float f);

#endif /* ICD_UTILS_H */
