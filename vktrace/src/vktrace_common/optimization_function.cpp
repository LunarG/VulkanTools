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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <ppl.h>
using namespace concurrency;
#define PARALLEL_INVOKE_NUM   10
#define SIZE_LIMIT_TO_USE_OPTIMIZATION 67100000
#endif // WIN32

using namespace std;

extern "C" void *opt_memcpy(void * destination, const void * source, size_t size)
{
    void *pRet = NULL;
#ifndef WIN32
    pRet = memcpy(destination, source, (size_t)size);
#else
    if (size < SIZE_LIMIT_TO_USE_OPTIMIZATION)
    {
        pRet = memcpy(destination, source, (size_t)size);
    }
    else
    {
        pRet = destination;
        void* ptr_parallel[PARALLEL_INVOKE_NUM];
        void* pBuffer_parallel[PARALLEL_INVOKE_NUM];
        size_t size_parallel[PARALLEL_INVOKE_NUM];

        int stepsize = size / PARALLEL_INVOKE_NUM;
        for (int i = 0; i < PARALLEL_INVOKE_NUM; i++)
        {
            ptr_parallel[i] = (char *)destination + i*stepsize;
            pBuffer_parallel[i] = (char *)source + i*stepsize;
            if ((i + 1) == PARALLEL_INVOKE_NUM)
            {
                size_parallel[i] = size - (PARALLEL_INVOKE_NUM - 1)*stepsize;
            }
            else
            {
                size_parallel[i] = stepsize;
            }
        }
        parallel_invoke(
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[9], pBuffer_parallel[9], (size_t)size_parallel[9]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[8], pBuffer_parallel[8], (size_t)size_parallel[8]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[7], pBuffer_parallel[7], (size_t)size_parallel[7]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[6], pBuffer_parallel[6], (size_t)size_parallel[6]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[5], pBuffer_parallel[5], (size_t)size_parallel[5]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[4], pBuffer_parallel[4], (size_t)size_parallel[4]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[3], pBuffer_parallel[3], (size_t)size_parallel[3]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[2], pBuffer_parallel[2], (size_t)size_parallel[2]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[1], pBuffer_parallel[1], (size_t)size_parallel[1]); },
            [&ptr_parallel, &pBuffer_parallel, &size_parallel] { memcpy(ptr_parallel[0], pBuffer_parallel[0], (size_t)size_parallel[0]); }
        );
    }
#endif // WIN32
    return pRet;
}
