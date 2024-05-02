set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network QUIET)

if(Qt5_FOUND)
    file(GLOB FILES_SOURCE ./*.cpp)
    file(GLOB FILES_HEADER ./*.h)

    file(GLOB FILES_CONFIGURATIONS_2_2_2 ./configurations/2.2.2/*.json)
    source_group("Configurations 2.2.2 Files" FILES ${FILES_CONFIGURATIONS_2_2_2})

    file(GLOB FILES_LAYERS_130 ./layers/130/*.json)
    source_group("130 Files" FILES ${FILES_LAYERS_130})

    file(GLOB FILES_LAYERS_135 ./layers/135/*.json)
    source_group("135 Files" FILES ${FILES_LAYERS_135})

    file(GLOB FILES_LAYERS_141 ./layers/141/*.json)
    source_group("141 Files" FILES ${FILES_LAYERS_141})

    file(GLOB FILES_LAYERS_148 ./layers/148/*.json)
    source_group("148 Files" FILES ${FILES_LAYERS_148})

    file(GLOB FILES_LAYERS_154 ./layers/154/*.json)
    source_group("154 Files" FILES ${FILES_LAYERS_154})

    file(GLOB FILES_LAYERS_162 ./layers/162/*.json)
    source_group("162 Files" FILES ${FILES_LAYERS_162})

    file(GLOB FILES_LAYERS_170 ./layers/170/*.json)
    source_group("170 Files" FILES ${FILES_LAYERS_170})

    file(GLOB FILES_LAYERS_SCHEMA ./layers/layers_schema.json)
    source_group("Schema Files" FILES ${FILES_LAYERS_SCHEMA})

    set(FILES_RESOURCES 
        ${FILES_CONFIGURATIONS_2_2_2}
        ${FILES_LAYERS_130}
        ${FILES_LAYERS_135}
        ${FILES_LAYERS_141}
        ${FILES_LAYERS_148}
        ${FILES_LAYERS_154}
        ${FILES_LAYERS_162}
        ${FILES_LAYERS_170}
        ${FILES_LAYERS_SCHEMA})

    set(FILES_ALL ${FILES_SOURCE} ${FILES_HEADER} ${FILES_RESOURCES})

    add_library(vkconfig_core STATIC ${FILES_ALL})
    target_compile_options(vkconfig_core PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
    target_compile_definitions(vkconfig_core PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT)
    target_compile_definitions(vkconfig_core PRIVATE INSTALL_FULL_SYSCONFDIR="${CMAKE_INSTALL_FULL_SYSCONFDIR}")
    target_compile_definitions(vkconfig_core PRIVATE INSTALL_FULL_DATAROOTDIR="${CMAKE_INSTALL_FULL_DATAROOTDIR}")
    set_target_properties(vkconfig_core PROPERTIES FOLDER "vkconfig_core")

    if(WIN32)
        target_compile_definitions(vkconfig_core PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_link_libraries(vkconfig_core Cfgmgr32)
    endif()

    target_link_libraries(vkconfig_core Vulkan::Headers valijson Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)

    if(BUILD_TESTS)
        add_subdirectory(test)
    endif()
endif()
