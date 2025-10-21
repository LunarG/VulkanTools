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
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB FILES_SOURCE ./*.cpp)
file(GLOB FILES_HEADER ./*.h)
file(GLOB FILES_DOCS ./*.md)

source_group("Docs Files" FILES ${FILES_DOCS})

set(FILES_ALL ${FILES_UI} ${FILES_SOURCE} ${FILES_HEADER} ${FILES_DOCS} resources.qrc)

if(NOT APPLE)
    if (NOT DEFINED CMAKE_INSTALL_BINDIR)
        include(GNUInstallDirs)
    endif()

    if(WIN32)
        add_executable(vkconfig-cmd ${FILES_ALL})
        target_compile_definitions(vkconfig-cmd PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_options(vkconfig-cmd PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
        target_link_libraries(vkconfig-cmd Cfgmgr32)
    else()
        add_executable(vkconfig-cmd ${FILES_ALL} ${FILES_UI})
    endif()

    if(Qt6_FOUND)
        target_link_libraries(vkconfig-cmd Vulkan::Headers vkconfig-core Qt6::Core Qt6::Network)
    endif()

    target_compile_definitions(vkconfig-cmd PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT)
    set_target_properties(vkconfig-cmd PROPERTIES FOLDER "vkconfig")
    set_target_properties(vkconfig-cmd PROPERTIES OUTPUT_NAME "vkconfig")

    install(TARGETS vkconfig-cmd DESTINATION ${CMAKE_INSTALL_BINDIR})

    if(WIN32)
        if(Qt6_FOUND)
            get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
        endif()

        get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    endif()
else()
    include(macOS/vkconfig.cmake)
endif()
