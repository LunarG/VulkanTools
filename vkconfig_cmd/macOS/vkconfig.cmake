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



add_executable(vkconfig-cmd
    ${FILES_ALL}
    )


target_link_libraries(vkconfig-cmd vkconfig-core Qt6::Core Qt6::Network)
target_link_libraries(vkconfig-cmd "-framework Cocoa -framework QuartzCore")

get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)

set_target_properties(vkconfig-cmd PROPERTIES OUTPUT_NAME "vkconfig")
set_target_properties(vkconfig-cmd PROPERTIES SKIP_BUILD_RPATH FALSE)
set_target_properties(vkconfig-cmd PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(vkconfig-cmd PROPERTIES INSTALL_RPATH "")
set_target_properties(vkconfig-cmd PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
install(TARGETS vkconfig-cmd)
