# ~~~
# Copyright (c) 2018 Valve Corporation
# Copyright (c) 2018 LunarG, Inc.
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

add_executable(vkconfig MACOSX_BUNDLE ${LAYERMGR_SRCS})
target_link_libraries(vkconfig Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(vkconfig "-framework Cocoa -framework QuartzCore")

set_target_properties(vkconfig PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

set(prefix "bin/vkconfig.app/Contents")
set(INSTALL_RUNTIME_DIR "${prefix}/MacOS")
set(INSTALL_CMAKE_DIR "${prefix}/Resources")

macro(install_qt5_plugin QT_PLUGIN_NAME QT_PLUGINS_VAR _prefix)
    get_target_property(QT_PLUGIN_PATH "${QT_PLUGIN_NAME}" LOCATION)
    if(EXISTS "${QT_PLUGIN_PATH}")
        get_filename_component(QT_PLUGIN_FILE "${QT_PLUGIN_PATH}" NAME)
        get_filename_component(QT_PLUGIN_TYPE "${QT_PLUGIN_PATH}" PATH)
        get_filename_component(QT_PLUGIN_TYPE "${QT_PLUGIN_TYPE}" NAME)
        set(QT_PLUGIN_DEST "${_prefix}/PlugIns/${QT_PLUGIN_TYPE}")
        file(MAKE_DIRECTORY "${QT_PLUGIN_DEST}")
        file(COPY "${QT_PLUGIN_PATH}" DESTINATION "${QT_PLUGIN_DEST}")
        install(FILES "${QT_PLUGIN_PATH}"
            DESTINATION "${QT_PLUGIN_DEST}")
        set(${QT_PLUGINS_VAR}
            "${${QT_PLUGINS_VAR}};${CMAKE_INSTALL_PREFIX}/${QT_PLUGIN_DEST}/${QT_PLUGIN_FILE}")
    else()
        message(FATAL_ERROR "Qt plugin ${QT_PLUGIN_NAME} not found")
    endif()
endmacro()

install_qt5_plugin("Qt5::QCocoaIntegrationPlugin" QT_PLUGINS ${prefix})

install(TARGETS vkconfig BUNDLE DESTINATION "bin")

set(DIRS "${CMAKE_BINARY_DIR}")

if(CMAKE_PREFIX_PATH)
    foreach(dir ${CMAKE_PREFIX_PATH})
        list(APPEND ${DIRS} "${dir}/bin" "${dir}/lib")
    endforeach()
endif()

list(APPEND ${DIRS} "${Qt5Widgets_DIR}/../..")

include(InstallRequiredSystemLibraries)

message(STATUS "QT_PLUGINS: ${QT_PLUGINS}")
message(STATUS "DIRS: ${DIRS}")

# Fix up the library references to be self-contained within the bundle.
install(CODE "
    include(BundleUtilities)
    fixup_bundle(\${CMAKE_INSTALL_PREFIX}/bin/vkconfig.app \"${QT_PLUGINS}\" \"${DIRS}\")
    verify_app(\${CMAKE_INSTALL_PREFIX}/bin/vkconfig.app)
    ")
