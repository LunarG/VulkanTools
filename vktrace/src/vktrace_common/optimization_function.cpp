#include <stdio.h>
#include <string.h>
#include <ppl.h>
#include <stdlib.h>

using namespace concurrency;
using namespace std;


#define PARALLEL_INVOKE_NUM   10
#define SIZE_LIMIT_TO_USE_OPTIMIZATION 67100000
extern "C" void *opt_memcpy(void * destination, const void * source, size_t size)
{
    void *pRet=NULL;
		if (size < SIZE_LIMIT_TO_USE_OPTIMIZATION)
		{
			//memcpy(ptr_address, pBuffer, (size_t)size);
			pRet=memcpy(destination, source, (size_t)size);
		}
		else
		{
		    pRet=destination;
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
					size_parallel[i] = size - (PARALLEL_INVOKE_NUM-1)*stepsize;
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
	return pRet;
}
