cmake_minimum_required(VERSION 3.10.2)
project(vkconfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets QUIET)

if(NOT Qt5_FOUND)
    message("WARNING: vkconfig will be excluded because Qt5 was not found. Please add Qt5 into the PATH environment variable")
else()
    set(LAYERMGR_SRCS
        dlgabout.ui
        dlgbetamessage.ui
        dlgcreateassociation.ui
        dlgcustompaths.ui
        dlghelp.ui
        dlghistory.ui
        dlglayeroutput.ui
        dlgprofileeditor.ui
        dlgvulkananalysis.ui
        dlgvulkaninfo.ui
        mainwindow.ui
        boolsettingwidget.cpp
        dlgabout.cpp
        dlgbetamessage.cpp
        dlgcreateassociation.cpp
        dlgcustompaths.cpp
        dlghelp.cpp
        dlglayeroutput.cpp
        dlgprofileeditor.cpp
        dlgvulkananalysis.cpp
        dlgvulkaninfo.cpp
        dlgvulkaninstallanalysis.cpp
        enumsettingwidget.cpp
        filenamesettingwidget.cpp
        foldersettingwidget.cpp
        khronossettingsadvanced.cpp
        layerfile.cpp
        main.cpp
        mainwindow.cpp
        multienumsetting.cpp
        profiledef.cpp
        settingstreemanager.cpp
        stringsettingwidget.cpp
        vulkanconfiguration.cpp
        resources.qrc
    )

    if(NOT APPLE)
        if (NOT DEFINED CMAKE_INSTALL_BINDIR)
            include(GNUInstallDirs)
        endif()

        if(WIN32)
            add_executable(vkconfig WIN32 ${LAYERMGR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/resourcefiles/vkconfig.rc)
            target_link_libraries(vkconfig Cfgmgr32)
        else()
            add_executable(vkconfig ${LAYERMGR_SRCS})
        endif()
        target_include_directories(vkconfig PRIVATE "${Vulkan_INCLUDE_DIR}")
        target_link_libraries(vkconfig Qt5::Core Qt5::Gui Qt5::Widgets)
        target_compile_definitions(vkconfig PRIVATE ${VKCONFIG_DEFINITIONS})

        install(TARGETS vkconfig DESTINATION ${CMAKE_INSTALL_BINDIR})

        if(WIN32)
            get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
            get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
            find_program(WINDEPLOYQT_EXE windeployqt HINTS "${QT_BIN_DIR}")

            message("Will run ${WINDEPLOYQT_EXE} at ${CMAKE_INSTALL_FULL_BINDIR}/vkconfig.exe")
            install(CODE "execute_process(COMMAND ${WINDEPLOYQT_EXE} ${CMAKE_INSTALL_FULL_BINDIR}/vkconfig.exe)")
        endif()

    else()
        include(macOS/vkconfig.cmake)
    endif()
endif()
