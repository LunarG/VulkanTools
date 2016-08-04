/*
* Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

//use PPL parallel_invoke call(on windows for now, but PPL also has a PPLx for Linux), or use cross-platform memcpy multithread which exclude PPL
//#define OPT_MEMCPY_USE_PPL_LIB


#ifdef WIN32
typedef struct __OPTChangedBlockInfo
{
    uint32_t offset;
    uint32_t length;
    uint32_t reserve0;
    uint32_t reserve1;
} OPTChangedBlockInfo, *pOPTChangedBlockInfo;
#endif // WIN32

#define USE_OPT_SPEEDUP

#define OPT_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES 0X00000001
