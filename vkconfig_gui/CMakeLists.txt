set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network QUIET)

if(NOT Qt5_FOUND)
    message("WARNING: vkconfig3 will be excluded because Qt5 was not found. Please add Qt5 into the PATH environment variable")
elseif(Qt5_VERSION VERSION_LESS 5.5)
    message("WARNING: vkconfig3 will be excluded because the found Qt version is too old. vkconfig requires version 5.5.")
else()
    file(GLOB FILES_UI ./*.ui)

    source_group("UI Files" FILES ${FILES_UI})

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
            add_executable(vkconfig-gui WIN32 ${FILES_ALL} ${CMAKE_CURRENT_SOURCE_DIR}/resourcefiles/vkconfig.rc)
            target_compile_definitions(vkconfig-gui PRIVATE _CRT_SECURE_NO_WARNINGS)
            target_compile_options(vkconfig-gui PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
            target_link_libraries(vkconfig-gui Cfgmgr32)
        else()
            add_executable(vkconfig-gui ${FILES_ALL} ${FILES_UI})
        endif()

        target_link_libraries(vkconfig-gui Vulkan::Headers vkconfig_core Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
        target_compile_definitions(vkconfig-gui PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT)
        set_target_properties(vkconfig-gui PROPERTIES FOLDER "vkconfig")

        install(TARGETS vkconfig-gui DESTINATION ${CMAKE_INSTALL_BINDIR})

        if(WIN32)
            get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
            get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
            find_program(WINDEPLOYQT_EXE windeployqt HINTS "${QT_BIN_DIR}")

            message("Will run ${WINDEPLOYQT_EXE} at ${CMAKE_INSTALL_FULL_BINDIR}/vkconfig3.exe")
            install(CODE "execute_process(COMMAND ${WINDEPLOYQT_EXE} ${CMAKE_INSTALL_FULL_BINDIR}/vkconfig3.exe)")
        endif()
    else()
        include(macOS/vkconfig.cmake)
    endif()
endif()
