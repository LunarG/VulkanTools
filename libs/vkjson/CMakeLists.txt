# Copyright 2015 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if (NOT WIN32)
    set (CMAKE_CXX_FLAGS "-std=c++11")
endif()
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../loader
	${CMAKE_CURRENT_SOURCE_DIR}/../../include/vulkan
	)

add_library(vkjson STATIC vkjson.cc vkjson_instance.cc ../../loader/cJSON.c)

if(UNIX)
    add_executable(vkjson_unittest vkjson_unittest.cc)
    add_executable(vkjson_info vkjson_info.cc)
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
    add_executable(vkjson_unittest vkjson_unittest.cc)
    add_executable(vkjson_info vkjson_info.cc)
endif()

target_link_libraries(vkjson_unittest vkjson)

if(WIN32)
    target_link_libraries(vkjson_info vkjson vulkan-${MAJOR})
elseif(UNIX)
    target_link_libraries(vkjson_info vkjson vulkan)
else()
endif()
