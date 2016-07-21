/*
* Copyright (C) 2016 Advanced Micro Device, Inc.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef _cplusplus
extern "C" void *opt_memcpy(void * destination, const void * source, size_t num);
#else
void *opt_memcpy(void * destination, const void * source, size_t num);
#endif
