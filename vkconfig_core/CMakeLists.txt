# ~~~
# Copyright (c) 2020-2025 LunarG, Inc.
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
# ~~~

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB FILES_SOURCE ./*.cpp)
file(GLOB FILES_HEADER ./*.h)

file(GLOB FILES_CONFIGURATIONS ./configurations/3.0.0/*.json)
source_group("Configurations Files" FILES ${FILES_CONFIGURATIONS})

file(GLOB FILES_LAYERS_SCHEMA ./layers/layers_schema.json)
source_group("Schema Files" FILES ${FILES_LAYERS_SCHEMA})

set(FILES_RESOURCES 
    ${FILES_CONFIGURATIONS}
    ${FILES_LAYERS_SCHEMA})

set(FILES_ALL ${FILES_SOURCE} ${FILES_HEADER} ${FILES_RESOURCES})

if(WIN32)
    # Use the static runtime library? This is for special builds of Vulkan Configurator only
    if(USE_STATIC_RT_VKCONFIG)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        message(STATUS "vkconfig-core will link against static runtime")
    endif()    
endif()

add_library(vkconfig-core STATIC ${FILES_ALL})
target_compile_options(vkconfig-core PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
target_compile_definitions(vkconfig-core PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT)
target_compile_definitions(vkconfig-core PRIVATE INSTALL_FULL_SYSCONFDIR="${CMAKE_INSTALL_FULL_SYSCONFDIR}")
target_compile_definitions(vkconfig-core PRIVATE INSTALL_FULL_DATAROOTDIR="${CMAKE_INSTALL_FULL_DATAROOTDIR}")
set_target_properties(vkconfig-core PROPERTIES FOLDER "vkconfig")

if(WIN32)
    target_compile_definitions(vkconfig-core PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(vkconfig-core Cfgmgr32)
endif()

if(Qt6_FOUND)
    target_link_libraries(vkconfig-core Vulkan::Headers valijson Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
elseif(Qt5_FOUND)
    target_link_libraries(vkconfig-core Vulkan::Headers valijson Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

