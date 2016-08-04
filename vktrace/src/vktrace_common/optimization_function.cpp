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
#include "vktrace_platform.h"

#define OPTIMIZATION_FUNCTION_IMPLEMENTATION


#if defined(WIN32)

  #if defined(OPT_MEMCPY_USE_PPL_LIB) 

    #include <ppl.h>
    using namespace concurrency;

  #endif
#else
  #include <semaphore.h>
  #include <pthread.h>
#endif

using namespace std;

static const size_t SIZE_LIMIT_TO_USE_OPTIMIZATION = 1048576; //turn off optimization of memcpy if size < this limit

#if defined(OPT_MEMCPY_USE_PPL_LIB) 

#if defined(WIN32) 
#define PARALLEL_INVOKE_NUM   10
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
#else//defined(OPT_MEMCPY_USE_PPL_LIB), Linux
extern "C" void *opt_memcpy(void * destination, const void * source, size_t size)
{
    return memcpy(destination, source, (size_t)size);
}
#endif

#else //!defined(OPT_MEMCPY_USE_PPL_LIB), use cross-platform memcpy multithread which exclude PPL

#if defined(WIN32) 
typedef HANDLE OPT_thread_id;
typedef HANDLE OPT_sem_id;
#else
typedef pthread_t OPT_thread_id;
typedef sem_t* OPT_sem_id;
#endif

typedef void(*OPT_ptr_task_unit_function)(void *pTaskUnitParaInput);

typedef struct
{
    void * src, *dest;
    size_t size;
} OPT_task_unit_parameters;

typedef struct
{
    int index;
    OPT_task_unit_parameters *ptask_units;
    int amount;
    OPT_sem_id sem_id_access;
} OPT_task_queue;

typedef struct
{
    int index;
    OPT_thread_id thread_id;
    OPT_sem_id sem_id_task_start;
    OPT_sem_id sem_id_task_end;
    //OPT_task_unit_parameters *ptask_para;
} OPT_task_control_block;


bool OPT_sem_create(OPT_sem_id *sem_id, uint32_t initvalue)
{
    bool sem_create_ok = false;
#if defined(WIN32)
    static const uint32_t maxvalue=0x40000;//no need for this windows value
    HANDLE sid = CreateSemaphore(NULL, initvalue, maxvalue, NULL);
    if (sid != INVALID_HANDLE_VALUE)
    {
        *sem_id = sid;
        sem_create_ok = true;
    }
#else
    sem_t *sem = new sem_t;
    if (sem_init(sem, 0, initvalue) == 0)
    {
        sem_create_ok = true;
        *sem_id = sem;
    }
#endif
    return sem_create_ok;
}

void OPT_sem_delete(OPT_sem_id sid)
{
#if defined(WIN32)
    CloseHandle(sid);
#else
    sem_close(sid);
    delete (sem_t *)sid;
#endif
}

void OPT_sem_wait(OPT_sem_id sid)
{
#if defined(WIN32)
    WaitForSingleObject(sid, INFINITE);
#else
    sem_wait(sid);
#endif
}

void OPT_sem_post(OPT_sem_id sid)
{
#if defined(WIN32)
    ReleaseSemaphore(sid, 1, NULL);
#else
    sem_post(sid);
#endif
}

#if defined(WIN32)
  typedef uint32_t(*OPT_thread_function_ptr)(void *parameters);
#else
  typedef void *(*OPT_thread_function_ptr)(void *parameters);
#endif
bool OPT_create_thread(OPT_thread_id *ptid, OPT_thread_function_ptr pfunc, OPT_task_control_block *ptaskpara)
{
    bool create_thread_ok = false;
#if defined(WIN32) 
    DWORD dwThreadID;
    HANDLE thread_handle;
    thread_handle = CreateThread(NULL, 0,
        (LPTHREAD_START_ROUTINE)pfunc, ptaskpara,
        0, &dwThreadID);

    if (thread_handle != INVALID_HANDLE_VALUE)
    {
        *ptid = thread_handle;
        create_thread_ok = true;
    }

#else
    pthread_t thread;
    int state = PTHREAD_CANCEL_ENABLE, oldstate, oldtype;
    state = PTHREAD_CANCEL_ASYNCHRONOUS;
    pthread_setcanceltype(state, &oldtype);
    if (pthread_create(&thread, NULL, pfunc, (void *)ptaskpara) == 0)
    {
        *ptid = thread;
        create_thread_ok = true;
    }
    pthread_setcanceltype(oldtype, &state);
#endif
    return create_thread_ok;
}


void OPT_join_thread(OPT_thread_id tid)
{
#if defined(WIN32) 
    WaitForSingleObject((HANDLE)tid, INFINITE);
#else
    pthread_join((pthread_t)tid, NULL);
#endif
}

void OPT_delete_thread(OPT_thread_id tid)
{
#if defined(WIN32) 
    DWORD  dwExitCode=0;
    TerminateThread((HANDLE)tid, dwExitCode);
#else
    pthread_cancel((pthread_t)tid);
#endif
}

int OPT_get_cpu_core_count()
{
    int iret = 4;
#if defined(WIN32) 
    SYSTEM_INFO sSysInfo;
    GetSystemInfo(&sSysInfo);
    iret = sSysInfo.dwNumberOfProcessors;
#else
    iret=sysconf(_SC_NPROCESSORS_ONLN);
#endif
    return iret;
}

OPT_task_queue *OPT_get_task_queue()
{
    static OPT_task_queue *pOPT_task_queue_multi_thread_memcpy = nullptr;
    if (!pOPT_task_queue_multi_thread_memcpy)
    {
        pOPT_task_queue_multi_thread_memcpy = new OPT_task_queue;
        memset( reinterpret_cast<void *>(pOPT_task_queue_multi_thread_memcpy), 0, sizeof(OPT_task_queue));
        OPT_sem_create(&pOPT_task_queue_multi_thread_memcpy->sem_id_access, 1);
    }
    return pOPT_task_queue_multi_thread_memcpy;
}

OPT_task_unit_parameters *OPT_get_task_unit_parameters()
{
    OPT_task_unit_parameters *pret = nullptr;
    OPT_task_queue *ptaskqueue = OPT_get_task_queue();
    OPT_sem_wait(ptaskqueue->sem_id_access);
    if (ptaskqueue->index<ptaskqueue->amount)
    {
        pret = &ptaskqueue->ptask_units[ptaskqueue->index];
        ptaskqueue->index++;
    }
    OPT_sem_post(ptaskqueue->sem_id_access);
    return pret;
}

OPT_task_control_block *OPT_get_task_control_block()
{
    static OPT_task_control_block *ptask_control_block = nullptr;
    if (!ptask_control_block)
    {
        int thread_number = OPT_get_cpu_core_count();
        ptask_control_block = new OPT_task_control_block[thread_number];
        memset((void *)ptask_control_block, 0, thread_number*sizeof(OPT_task_control_block));
    }
    return ptask_control_block;
}

uint32_t OPT_thread_function(void *ptcbpara)
{
    OPT_task_control_block * ptasktcb = reinterpret_cast<OPT_task_control_block *>(ptcbpara);
    OPT_task_unit_parameters *parameters;
    bool stop_loop = false;
    while (1)
    {
        OPT_sem_wait(ptasktcb->sem_id_task_start);
        stop_loop = false;
        while (!stop_loop)
        {
            parameters = OPT_get_task_unit_parameters();
            if (parameters != nullptr)
            {
                memcpy(parameters->dest, parameters->src, parameters->size);
            }
            else
            {
                stop_loop = true;
            }
        }
        OPT_sem_post(ptasktcb->sem_id_task_end);
    }
}

bool OPT_init_multi_threads_memcpy_custom(OPT_thread_function_ptr pfunc)
{
    bool init_multi_threads_memcpy_custom_ok = false, success_sem_start = false, success_sem_end = false, success_thread = false;
    OPT_task_control_block *ptcb = OPT_get_task_control_block();
    int thread_number = OPT_get_cpu_core_count();
    for (int i = 0; i < thread_number; i++)
    {
        success_sem_start = OPT_sem_create(&ptcb[i].sem_id_task_start,0);
        success_sem_end = OPT_sem_create(&ptcb[i].sem_id_task_end,0);
        ptcb[i].index = i;
        success_thread = OPT_create_thread(&ptcb[i].thread_id, (OPT_thread_function_ptr)pfunc, &ptcb[i]);
        if (success_sem_start&&success_sem_end &&success_thread)
        {
            init_multi_threads_memcpy_custom_ok = true;
        }
        else
        {
            init_multi_threads_memcpy_custom_ok = false;
            break;
        }
    }
    return init_multi_threads_memcpy_custom_ok;
}

static OPT_sem_id glocal_sem_id;
static bool glocal_sem_id_create_success= OPT_sem_create(&glocal_sem_id, 1);

int OPT_ref_count(bool release)
{
  static int ref_count = 0;
  int curr_ref_count;
  if (!release)
  {
      OPT_sem_wait(glocal_sem_id);
      curr_ref_count = ref_count;
      ++ref_count;
      OPT_sem_post(glocal_sem_id);
      return curr_ref_count;
  }
  else
  {
     OPT_sem_wait(glocal_sem_id);
     --ref_count;
     curr_ref_count = ref_count;
     OPT_sem_post(glocal_sem_id);
     return curr_ref_count;
  }
}

extern "C" BOOL OPT_init_multi_threads_memcpy()
{
    int refnum = OPT_ref_count(false);
    BOOL init_multi_threads_memcpy_ok = FALSE;
    OPT_thread_function_ptr pfunc = (OPT_thread_function_ptr)OPT_thread_function;
    if (!refnum)
    {
        if (OPT_init_multi_threads_memcpy_custom(pfunc))
        {
            init_multi_threads_memcpy_ok = TRUE;
        }
    }
    else
    {
        init_multi_threads_memcpy_ok = TRUE;
    }
    return init_multi_threads_memcpy_ok;
}

extern "C" void OPT_done_multi_threads_memcpy()
{
    int refnum = OPT_ref_count(true);
    if (!refnum)
    {
        OPT_task_control_block *ptcb = OPT_get_task_control_block();
        int thread_number = OPT_get_cpu_core_count();

        for (int i = 0; i < thread_number; i++)
        {
            OPT_sem_delete(ptcb[i].sem_id_task_start);
            OPT_sem_delete(ptcb[i].sem_id_task_end);
            OPT_delete_thread(ptcb[i].thread_id);
        }
        delete[] OPT_get_task_control_block();
        OPT_sem_delete(OPT_get_task_queue()->sem_id_access);
        delete OPT_get_task_queue();
    }
}

//must keep units until OPT_multi_threads_memcpy_run finished
void OPT_set_task_queue(OPT_task_unit_parameters *units, int unitamount)
{
    OPT_task_queue *pqueue = OPT_get_task_queue();
    pqueue->amount = unitamount;
    pqueue->ptask_units = units;
    pqueue->index = 0;
}

void OPT_clear_task_queue()
{
    OPT_task_queue *pqueue = OPT_get_task_queue();
    pqueue->amount = 0;
    pqueue->ptask_units = nullptr;
    pqueue->index = 0;
}

//The steps for using multithreading copy:
//<1>init_multi_threads_memcpy
//   it should be put at beginning of the app

//<2>OPT_set_task_queue
//<3>OPT_multi_threads_memcpy_run

//<4>done_multi_threads_memcpy()
//   it should be putted at end of the app
void OPT_multi_threads_memcpy_run()
{
    OPT_task_control_block *ptcb = OPT_get_task_control_block();
    int thread_number = OPT_get_cpu_core_count();

    for (int i = 0; i < thread_number; i++)
    {
        OPT_sem_post(ptcb[i].sem_id_task_start);
    }

    for (int i = 0; i < thread_number; i++)
    {
        OPT_sem_wait(ptcb[i].sem_id_task_end);
    }

}

#define OPT_MEMCPY_MULTITHREAD_UNIT_SIZE 0x10000
void OPT_memcpy_multithread(void *dest, const void *src, size_t n)
{
    int thread_number = OPT_get_cpu_core_count();

    //taskunitamount should be >=thread_number, but should not >= a value which make the unit too small and the cost of switch thread > memcpy that unit, on the other side, too small is also not best if consider last task will determine the memcpy speed.
    int taskunitamount = n / OPT_MEMCPY_MULTITHREAD_UNIT_SIZE;
    if (taskunitamount < thread_number)
    {
        taskunitamount = thread_number;
    }
    size_t size_per_unit = n / taskunitamount, size_left = n%taskunitamount, size;
    OPT_task_unit_parameters *units = new OPT_task_unit_parameters[taskunitamount];

    //OPT_set_task_queue(OPT_task_unit_parameters *units,int unitamount)
    for (int i = 0; i < taskunitamount; i++)
    {
        size = size_per_unit;
        if((i + 1) == taskunitamount)
        {
            size += size_left;
        }
        units[i].src = (void *)((uint8_t *)src + i*size_per_unit);
        units[i].dest = (void *)((uint8_t *)dest + i*size_per_unit);
        units[i].size = size;
    }
    OPT_set_task_queue(units, taskunitamount);
    OPT_multi_threads_memcpy_run();
    delete[] units;
    OPT_clear_task_queue();
}

extern "C" void *opt_memcpy(void * destination, const void * source, size_t size)
{
    void *pRet = NULL;
    if (size < SIZE_LIMIT_TO_USE_OPTIMIZATION)
    {
        pRet = memcpy(destination, source, (size_t)size);
    }
    else
    {
        pRet = destination;
        OPT_memcpy_multithread(destination, source, (size_t)size);
    }
    return pRet;
}

#endif
