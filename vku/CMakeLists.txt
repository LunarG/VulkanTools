#
# Copyright (c) 2021-2021 Valve Corporation
# Copyright (c) 2021-2021 LunarG, Inc.
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
#

file(GLOB FILES_SOURCE ./*.cpp)
file(GLOB FILES_HEADER ./*.h)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(vku STATIC ${FILES_SOURCE} ${FILES_HEADER})

if(WIN32)
	target_compile_definitions(vku PUBLIC _CRT_SECURE_NO_WARNINGS)
else()
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
install(TARGETS vku DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(vku PROPERTIES LINKER_LANGUAGE CXX)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/generated
                    ${VulkanHeaders_INCLUDE_DIRS})

