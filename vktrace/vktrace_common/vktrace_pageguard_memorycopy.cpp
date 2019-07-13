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
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vktrace_pageguard_memorycopy.h"

#define OPTIMIZATION_FUNCTION_IMPLEMENTATION

static const size_t SIZE_LIMIT_TO_USE_OPTIMIZATION = 1 * 1024 * 1024;  // turn off optimization of memcpy if size < this limit.
// for multithread memcopy, there is system cost on multiple threads include switch control from different threads,
// synchronization and communication like semaphore wait and post and other process which system don't need to handle
// in single thread memcpy, if these cost is greater than benefit of using multithread,we should directly call memcpy.
// here set the value with 1M base on roughly estimation of the cost.

bool vktrace_sem_create(vktrace_sem_id *sem_id, uint32_t initvalue) {
    bool sem_create_ok = false;
#if defined(USE_PAGEGUARD_SPEEDUP)
#if defined(WIN32)
    static const uint32_t maxValue = 0x40000;  // Posix doesn't have this value in its sem_create interface, but windows have it.
                                               // here we also don't need this value, so give it a value that's has no limit for
                                               // this case.
    HANDLE sid = CreateSemaphore(NULL, initvalue, maxValue, NULL);
    if (sid != INVALID_HANDLE_VALUE) {
        *sem_id = sid;
        sem_create_ok = true;
    }
#else
    sem_t *sem = new sem_t;
    if (sem != nullptr) {
        if (sem_init(sem, 0, initvalue) == 0) {
            sem_create_ok = true;
            *sem_id = sem;
        }
    }
#endif
#endif  // USE_PAGEGUARD_SPEEDUP
    return sem_create_ok;
}

void vktrace_sem_delete(vktrace_sem_id sid) {
#if defined(USE_PAGEGUARD_SPEEDUP)
#if defined(WIN32)
    CloseHandle(sid);
#else
    sem_close(sid);
    delete (sem_t *)sid;
#endif
#endif  // USE_PAGEGUARD_SPEEDUP
}

void vktrace_sem_wait(vktrace_sem_id sid) {
#if defined(USE_PAGEGUARD_SPEEDUP)
#if defined(WIN32)
    WaitForSingleObject(sid, INFINITE);
#else
    sem_wait(sid);
#endif
#endif  // USE_PAGEGUARD_SPEEDUP
}

void vktrace_sem_post(vktrace_sem_id sid) {
#if defined(USE_PAGEGUARD_SPEEDUP)
#if defined(WIN32)
    ReleaseSemaphore(sid, 1, NULL);
#else
    sem_post(sid);
#endif
#endif  // USE_PAGEGUARD_SPEEDUP
}

#if defined(PAGEGUARD_MEMCPY_USE_PPL_LIB)

#if defined(WIN32)
#define PARALLEL_INVOKE_NUM \
    10  // this is the maximum task number that  parallel_invoke can use, how many threads are actually used to finish these task
        // depand on system concurrency algorithm.
extern "C" void *vktrace_pageguard_memcpy(void *destination, const void *source, uint64_t size) {
    void *pRet = NULL;
    if (size < SIZE_LIMIT_TO_USE_OPTIMIZATION) {
        pRet = memcpy(destination, source, (size_t)size);
    } else {
        pRet = destination;
        void *ptr_parallel[PARALLEL_INVOKE_NUM];
        void *pBuffer_parallel[PARALLEL_INVOKE_NUM];
        size_t size_parallel[PARALLEL_INVOKE_NUM];

        int stepsize = size / PARALLEL_INVOKE_NUM;
        for (int i = 0; i < PARALLEL_INVOKE_NUM; i++) {
            ptr_parallel[i] = (char *)destination + i * stepsize;
            pBuffer_parallel[i] = (char *)source + i * stepsize;
            if ((i + 1) == PARALLEL_INVOKE_NUM) {
                size_parallel[i] = size - (PARALLEL_INVOKE_NUM - 1) * stepsize;
            } else {
                size_parallel[i] = stepsize;
            }
        }
        parallel_invoke([&ptr_parallel, &pBuffer_parallel,
                         &size_parallel] { memcpy(ptr_parallel[9], pBuffer_parallel[9], (size_t)size_parallel[9]); },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[8], pBuffer_parallel[8], (size_t)size_parallel[8]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[7], pBuffer_parallel[7], (size_t)size_parallel[7]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[6], pBuffer_parallel[6], (size_t)size_parallel[6]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[5], pBuffer_parallel[5], (size_t)size_parallel[5]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[4], pBuffer_parallel[4], (size_t)size_parallel[4]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[3], pBuffer_parallel[3], (size_t)size_parallel[3]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[2], pBuffer_parallel[2], (size_t)size_parallel[2]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[1], pBuffer_parallel[1], (size_t)size_parallel[1]);
                        },
                        [&ptr_parallel, &pBuffer_parallel, &size_parallel] {
                            memcpy(ptr_parallel[0], pBuffer_parallel[0], (size_t)size_parallel[0]);
                        });
    }
    return pRet;
}
#else  // defined(PAGEGUARD_MEMCPY_USE_PPL_LIB), Linux
extern "C" void *vktrace_pageguard_memcpy(void *destination, const void *source, uint64_t size) {
    return memcpy(destination, source, (size_t)size);
}
#endif

#else  //! defined(PAGEGUARD_MEMCPY_USE_PPL_LIB), use cross-platform memcpy multithread which exclude PPL

typedef void (*vktrace_pageguard_ptr_task_unit_function)(void *pTaskUnitParaInput);

typedef struct {
    void *src, *dest;
    size_t size;
} vktrace_pageguard_task_unit_parameters;

typedef struct {
    size_t index;
    vktrace_pageguard_task_unit_parameters *ptask_units;
    size_t amount;
    vktrace_sem_id sem_id_access;
} vktrace_pageguard_task_queue;

typedef struct {
    size_t index;
    vktrace_pageguard_thread_id thread_id;
    vktrace_sem_id sem_id_task_start;
    vktrace_sem_id sem_id_task_end;
    // vktrace_pageguard_task_unit_parameters *ptask_para;
} vktrace_pageguard_task_control_block;

#if defined(WIN32)
typedef void (*vktrace_pageguard_thread_function_ptr)(void *parameters);
#else
typedef void *(*vktrace_pageguard_thread_function_ptr)(void *parameters);
#endif
bool vktrace_pageguard_create_thread(vktrace_pageguard_thread_id *ptid, vktrace_pageguard_thread_function_ptr pfunc,
                                     vktrace_pageguard_task_control_block *ptaskpara) {
    bool create_thread_ok = false;
#if defined(WIN32)
    DWORD dwThreadID;
    HANDLE thread_handle;
    thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pfunc, ptaskpara, 0, &dwThreadID);

    if (thread_handle != INVALID_HANDLE_VALUE) {
        *ptid = thread_handle;
        create_thread_ok = true;
    }

#else
    pthread_t thread;
    int state = PTHREAD_CANCEL_ENABLE, oldtype;
    state = PTHREAD_CANCEL_ASYNCHRONOUS;
    pthread_setcanceltype(state, &oldtype);
    if (pthread_create(&thread, NULL, pfunc, (void *)ptaskpara) == 0) {
        *ptid = thread;
        create_thread_ok = true;
    }
    pthread_setcanceltype(oldtype, &state);
#endif
    return create_thread_ok;
}

void vktrace_pageguard_delete_thread(vktrace_pageguard_thread_id tid) {
#if defined(WIN32)
    DWORD dwExitCode = 0;
    TerminateThread((HANDLE)tid, dwExitCode);
#else
    pthread_cancel((pthread_t)tid);
    pthread_join((pthread_t)tid, NULL);
#endif
}

uint32_t vktrace_pageguard_get_cpu_core_count() {
    uint32_t iret = 4;
#if defined(WIN32)
    SYSTEM_INFO sSysInfo;
    GetSystemInfo(&sSysInfo);
    iret = sSysInfo.dwNumberOfProcessors;
#else
    iret = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    return iret;
}

vktrace_pageguard_task_queue *vktrace_pageguard_get_task_queue() {
    static vktrace_pageguard_task_queue *pvktrace_pageguard_task_queue_multi_thread_memcpy = nullptr;
    if (!pvktrace_pageguard_task_queue_multi_thread_memcpy) {
        pvktrace_pageguard_task_queue_multi_thread_memcpy = new vktrace_pageguard_task_queue;
        memset(reinterpret_cast<void *>(pvktrace_pageguard_task_queue_multi_thread_memcpy), 0,
               sizeof(vktrace_pageguard_task_queue));
        vktrace_sem_create(&pvktrace_pageguard_task_queue_multi_thread_memcpy->sem_id_access, 1);
    }
    return pvktrace_pageguard_task_queue_multi_thread_memcpy;
}

vktrace_pageguard_task_unit_parameters *vktrace_pageguard_get_task_unit_parameters() {
    vktrace_pageguard_task_unit_parameters *pret = nullptr;
    vktrace_pageguard_task_queue *ptaskqueue = vktrace_pageguard_get_task_queue();
    vktrace_sem_wait(ptaskqueue->sem_id_access);
    if (ptaskqueue->index < ptaskqueue->amount) {
        pret = &ptaskqueue->ptask_units[ptaskqueue->index];
        ptaskqueue->index++;
    }
    vktrace_sem_post(ptaskqueue->sem_id_access);
    return pret;
}

vktrace_pageguard_task_control_block *vktrace_pageguard_get_task_control_block() {
    static vktrace_pageguard_task_control_block *ptask_control_block = nullptr;
    if (!ptask_control_block) {
        int thread_number = vktrace_pageguard_get_cpu_core_count();
        ptask_control_block = reinterpret_cast<vktrace_pageguard_task_control_block *>(
            new uint8_t[thread_number * sizeof(vktrace_pageguard_task_control_block)]);
        memset((void *)ptask_control_block, 0, thread_number * sizeof(vktrace_pageguard_task_control_block));
    }
    return ptask_control_block;
}

void vktrace_pageguard_thread_function(void *ptcbpara) {
    vktrace_pageguard_task_control_block *ptasktcb = reinterpret_cast<vktrace_pageguard_task_control_block *>(ptcbpara);
    vktrace_pageguard_task_unit_parameters *parameters;
    bool stop_loop;
    while (1) {
        vktrace_sem_wait(ptasktcb->sem_id_task_start);
        stop_loop = false;
        while (!stop_loop) {
            parameters = vktrace_pageguard_get_task_unit_parameters();
            if (parameters != nullptr) {
                memcpy(parameters->dest, parameters->src, parameters->size);
            } else {
                stop_loop = true;
            }
        }
        vktrace_sem_post(ptasktcb->sem_id_task_end);
    }
}

bool vktrace_pageguard_init_multi_threads_memcpy_custom(vktrace_pageguard_thread_function_ptr pfunc) {
    bool init_multi_threads_memcpy_custom_ok = false, success_sem_start = false, success_sem_end = false, success_thread = false;
    vktrace_pageguard_task_control_block *ptcb = vktrace_pageguard_get_task_control_block();
    int thread_number = vktrace_pageguard_get_cpu_core_count();
    for (int i = 0; i < thread_number; i++) {
        success_sem_start = vktrace_sem_create(&ptcb[i].sem_id_task_start, 0);
        success_sem_end = vktrace_sem_create(&ptcb[i].sem_id_task_end, 0);
        ptcb[i].index = i;
        success_thread =
            vktrace_pageguard_create_thread(&ptcb[i].thread_id, (vktrace_pageguard_thread_function_ptr)pfunc, &ptcb[i]);
        if (success_sem_start && success_sem_end && success_thread) {
            init_multi_threads_memcpy_custom_ok = true;
        } else {
            init_multi_threads_memcpy_custom_ok = false;
            break;
        }
    }
    return init_multi_threads_memcpy_custom_ok;
}

static vktrace_sem_id glocal_sem_id;
#if defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)
static bool glocal_sem_id_create_success __attribute__((unused)) = vktrace_sem_create(&glocal_sem_id, 1);
#else
static bool glocal_sem_id_create_success = vktrace_sem_create(&glocal_sem_id, 1);
#endif

int vktrace_pageguard_ref_count(bool release) {
    static int ref_count = 0;
    int curr_ref_count;
    if (!release) {
        vktrace_sem_wait(glocal_sem_id);
        curr_ref_count = ref_count;
        ++ref_count;
        vktrace_sem_post(glocal_sem_id);
        return curr_ref_count;
    } else {
        vktrace_sem_wait(glocal_sem_id);
        --ref_count;
        curr_ref_count = ref_count;
        vktrace_sem_post(glocal_sem_id);
        return curr_ref_count;
    }
}

extern "C" BOOL vktrace_pageguard_init_multi_threads_memcpy() {
    int refnum = vktrace_pageguard_ref_count(false);
    BOOL init_multi_threads_memcpy_ok = TRUE;
    vktrace_pageguard_thread_function_ptr pfunc = (vktrace_pageguard_thread_function_ptr)vktrace_pageguard_thread_function;
    if (!refnum) {
        init_multi_threads_memcpy_ok = vktrace_pageguard_init_multi_threads_memcpy_custom(pfunc);
    }
    return init_multi_threads_memcpy_ok;
}

void vktrace_pageguard_delete_task_control_block() { delete[] vktrace_pageguard_get_task_control_block(); }

void vktrace_pageguard_delete_task_queue() { delete vktrace_pageguard_get_task_queue(); }

extern "C" void vktrace_pageguard_done_multi_threads_memcpy() {
    int refnum = vktrace_pageguard_ref_count(true);
    if (!refnum) {
        vktrace_pageguard_task_control_block *task_control_block = vktrace_pageguard_get_task_control_block();
        if (task_control_block != nullptr) {
            int thread_number = vktrace_pageguard_get_cpu_core_count();

            for (int i = 0; i < thread_number; i++) {
                vktrace_pageguard_delete_thread(task_control_block[i].thread_id);
                vktrace_sem_delete(task_control_block[i].sem_id_task_start);
                vktrace_sem_delete(task_control_block[i].sem_id_task_end);
            }
            vktrace_pageguard_delete_task_control_block();
            vktrace_sem_delete(vktrace_pageguard_get_task_queue()->sem_id_access);
            vktrace_pageguard_delete_task_queue();
        }
    }
}

// must keep units until vktrace_pageguard_multi_threads_memcpy_run finished
void vktrace_pageguard_set_task_queue(vktrace_pageguard_task_unit_parameters *units, size_t unitamount) {
    vktrace_sem_wait(glocal_sem_id);
    vktrace_pageguard_task_queue *pqueue = vktrace_pageguard_get_task_queue();
    pqueue->amount = unitamount;
    pqueue->ptask_units = units;
    pqueue->index = 0;
}

void vktrace_pageguard_clear_task_queue() {
    vktrace_pageguard_task_queue *pqueue = vktrace_pageguard_get_task_queue();
    pqueue->amount = 0;
    pqueue->ptask_units = nullptr;
    pqueue->index = 0;
    vktrace_sem_post(glocal_sem_id);
}

// The steps for using multithreading copy:
//<1>init_multi_threads_memcpy
//   it should be put at beginning of the app

//<2>vktrace_pageguard_set_task_queue
//<3>vktrace_pageguard_multi_threads_memcpy_run

//<4>done_multi_threads_memcpy()
//   it should be putted at end of the app
void vktrace_pageguard_multi_threads_memcpy_run() {
    vktrace_pageguard_task_control_block *ptcb = vktrace_pageguard_get_task_control_block();
    int thread_number = vktrace_pageguard_get_cpu_core_count();

    for (int i = 0; i < thread_number; i++) {
        vktrace_sem_post(ptcb[i].sem_id_task_start);
    }

    for (int i = 0; i < thread_number; i++) {
        vktrace_sem_wait(ptcb[i].sem_id_task_end);
    }
}

void vktrace_pageguard_memcpy_multithread(void *dest, const void *src, size_t n) {
    static const size_t PAGEGUARD_MEMCPY_MULTITHREAD_UNIT_SIZE = 0x10000;
    uint32_t thread_number = vktrace_pageguard_get_cpu_core_count();

    // taskunitamount should be >=thread_number, but should not >= a value which make the unit too small and the cost of switch
    // thread > memcpy that unit, on the other side, too small is also not best if consider last task will determine the memcpy
    // speed.
    size_t taskunitamount = n / PAGEGUARD_MEMCPY_MULTITHREAD_UNIT_SIZE;
    if (taskunitamount < thread_number) {
        taskunitamount = thread_number;
    }
    size_t size_per_unit = n / taskunitamount, size_left = n % taskunitamount, size;
    vktrace_pageguard_task_unit_parameters *units = reinterpret_cast<vktrace_pageguard_task_unit_parameters *>(
        new uint8_t[taskunitamount * sizeof(vktrace_pageguard_task_unit_parameters)]);
    assert(units);
    for (uint32_t i = 0; i < taskunitamount; i++) {
        size = size_per_unit;
        if ((i + 1) == taskunitamount) {
            size += size_left;
        }
        units[i].src = (void *)((uint8_t *)src + i * size_per_unit);
        units[i].dest = (void *)((uint8_t *)dest + i * size_per_unit);
        units[i].size = size;
    }
    vktrace_pageguard_set_task_queue(units, taskunitamount);
    vktrace_pageguard_multi_threads_memcpy_run();
    delete[] units;
    vktrace_pageguard_clear_task_queue();
}

extern "C" void *vktrace_pageguard_memcpy(void *destination, const void *source, uint64_t size) {
    void *pRet = NULL;
    if (size < SIZE_LIMIT_TO_USE_OPTIMIZATION) {
        pRet = memcpy(destination, source, (size_t)size);
    } else {
        pRet = destination;
        vktrace_pageguard_memcpy_multithread(destination, source, (size_t)size);
    }
    return pRet;
}
#endif
