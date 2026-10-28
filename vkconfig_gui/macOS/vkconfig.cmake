# ~~~
# Copyright (c) 2018-2025 Valve Corporation
# Copyright (c) 2018-2025 LunarG, Inc.
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



add_executable(vkconfig-gui
    MACOSX_BUNDLE
    ${FILES_ALL}
    ${CMAKE_CURRENT_SOURCE_DIR}/macOS/Resources/VulkanIcon.icns
    )
#set_target_properties(vkconfig PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/macOS/Info.plist)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/macOS/vkconfig.sh PROPERTIES MACOSX_PACKAGE_LOCATION "MacOS")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/macOS/Resources/VulkanIcon.icns
                            PROPERTIES
                            MACOSX_PACKAGE_LOCATION
                            "Resources")

target_link_libraries(vkconfig-gui "-framework Cocoa -framework QuartzCore")

if(Qt6_FOUND)
    target_link_libraries(vkconfig-gui vkconfig-core Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
    get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
endif()

get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
find_program(MACDEPLOYQT_EXE macdeployqt HINTS "${QT_BIN_DIR}")

add_custom_command(TARGET vkconfig-gui POST_BUILD
    COMMAND "${MACDEPLOYQT_EXE}"
        "${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app/"
        -always-overwrite
        -executable="${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app/Contents/MacOS/vkconfig-gui"
        -verbose=1
    COMMENT "Running macdeployqt..."
    )

# Bundle the Vulkan Loader so vkconfig doesn't rely on a system wide installation
if(TARGET Vulkan::Loader)
    add_custom_command(TARGET vkconfig-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app/Contents/Frameworks"
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:Vulkan::Loader>"
        "${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app/Contents/Frameworks/libvulkan.1.dylib"
        COMMENT "Copying the Vulkan Loader to vkconfig-gui.app..."
    )
else()
    message(WARNING "Vulkan::Loader not found, vkconfig-gui.app will rely on the system Vulkan Loader")
endif()

add_custom_command(TARGET vkconfig-gui POST_BUILD
    COMMAND cp
    "${CMAKE_CURRENT_SOURCE_DIR}/macOS/Info.plist"
    "${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app/Contents/"
    COMMENT "Copying Info.plist to vkconfig-gui.app..."
)

# The bundle was modified after linking so it must be signed again or macOS will kill it at launch
add_custom_command(TARGET vkconfig-gui POST_BUILD
    COMMAND codesign --force --deep --sign -
    "${CMAKE_CURRENT_BINARY_DIR}/vkconfig-gui.app"
    COMMENT "Ad-hoc signing vkconfig-gui.app..."
)

set_target_properties(vkconfig-gui PROPERTIES SKIP_BUILD_RPATH FALSE)
set_target_properties(vkconfig-gui PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(vkconfig-gui PROPERTIES INSTALL_RPATH "")
set_target_properties(vkconfig-gui PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
install(TARGETS vkconfig-gui BUNDLE DESTINATION "bin")
