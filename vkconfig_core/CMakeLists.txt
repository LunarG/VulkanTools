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

target_link_libraries(vkconfig-core Vulkan::Headers valijson Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

