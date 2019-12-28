/*
 * Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
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

#include "vktrace_lib_helpers.h"

// templates to convert API handles to uint64_t type and vice versa

template <>
uint64_t ToHandleId<uint64_t>(const uint64_t &handle) {
    return handle;
}

template <>
uint64_t FromHandleId<uint64_t>(uint64_t handle_id) {
    return handle_id;
}
