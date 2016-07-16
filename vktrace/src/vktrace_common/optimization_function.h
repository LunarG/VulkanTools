#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef _cplusplus
extern "C" void *opt_memcpy(void * destination, const void * source, size_t num);
#else
void *opt_memcpy(void * destination, const void * source, size_t num);
#endif

typedef struct __OPTChangedBlockInfo
{
    DWORD offset;
    DWORD length;
    DWORD reserve0;
    DWORD reserve1;
} OPTChangedBlockInfo, *pOPTChangedBlockInfo;

#define USE_OPT_SPEEDUP

#define OPT_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES 0X00000001
