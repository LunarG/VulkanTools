set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB FILES_SOURCE ./*.cpp)
file(GLOB FILES_HEADER ./*.h)
file(GLOB FILES_DOCS ./*.md)

source_group("Docs Files" FILES ${FILES_DOCS})

set(FILES_ALL ${FILES_UI} ${FILES_SOURCE} ${FILES_HEADER} ${FILES_DOCS})

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

    target_link_libraries(vkconfig-cmd Vulkan::Headers vkconfig-core Qt6::Core Qt6::Network)
    target_compile_definitions(vkconfig-cmd PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT)
    set_target_properties(vkconfig-cmd PROPERTIES FOLDER "vkconfig")
    set_target_properties(vkconfig-cmd PROPERTIES OUTPUT_NAME "vkconfig")

    install(TARGETS vkconfig-cmd DESTINATION ${CMAKE_INSTALL_BINDIR})

    if(WIN32)
        get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    endif()
else()
    include(macOS/vkconfig.cmake)
endif()
