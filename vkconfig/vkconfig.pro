QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11
CONFIG += sdk_no_version_check

INCLUDEPATH += $$(VULKAN_SDK)/include


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

Release: DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

linux: QMAKE_CXXFLAGS += -Wunused-variable

SOURCES += \
    ../vkconfig_core/application.cpp \
    ../vkconfig_core/application_singleton.cpp \
    ../vkconfig_core/command_line.cpp \
    ../vkconfig_core/configuration.cpp \
    ../vkconfig_core/environment.cpp \
    ../vkconfig_core/layer.cpp \
    ../vkconfig_core/layer_setting.cpp \
    ../vkconfig_core/layer_type.cpp \
    ../vkconfig_core/parameter.cpp \
    ../vkconfig_core/path_manager.cpp \
    ../vkconfig_core/util.cpp \
    ../vkconfig_core/version.cpp \
    vulkan.cpp \
    alert.cpp \
    widget_bool_setting.cpp \
    widget_enum_setting.cpp \
    widget_multi_enum_setting.cpp \
    widget_filesystem_setting.cpp \
    widget_string_setting.cpp \
    widget_mute_message.cpp \
    widget_tree_friendly_combobox.cpp \
    widget_vuid_search.cpp \
    dialog_vulkan_analysis.cpp \
    dialog_vulkan_info.cpp \
    dlgabout.cpp \
    dlgcreateassociation.cpp \
    dlgcustompaths.cpp \
    dlgprofileeditor.cpp \
    main.cpp \
    mainwindow.cpp \
    preferences.cpp \
    settingstreemanager.cpp \
    khronossettingsadvanced.cpp \
    configurator.cpp

HEADERS += \
    ../vkconfig_core/application.h \
    ../vkconfig_core/application_singleton.h \
    ../vkconfig_core/command_line.h \
    ../vkconfig_core/configuration.h \
    ../vkconfig_core/environment.h \
    ../vkconfig_core/layer.h \
    ../vkconfig_core/layer_setting.h \
    ../vkconfig_core/layer_type.h \
    ../vkconfig_core/parameter.h \
    ../vkconfig_core/path_manager.h \
    ../vkconfig_core/util.h \
    ../vkconfig_core/version.h \
    vulkan.h \
    alert.h \
    widget_bool_setting.h \
    widget_enum_setting.h \
    widget_multi_enum_setting.h \
    widget_filesystem_setting.h \
    widget_string_setting.h \
    widget_mute_message.h \
    widget_tree_friendly_combobox.h \
    widget_vuid_search.h \
    dialog_vulkan_analysis.h \
    dialog_vulkan_info.h \
    dlgabout.h \
    dlgcreateassociation.h \
    dlgcustompaths.h \
    dlgprofileeditor.h \
    khronossettingsadvanced.h \
    mainwindow.h \
    preferences.h \
    settingstreemanager.h \
    configurator.h

FORMS += \
    dlgabout.ui \
    dlgcreateassociation.ui \
    dlgcustompaths.ui \
    dlgprofileeditor.ui \
    dialog_vulkan_analysis.ui \
    dialog_vulkan_info.ui \
    mainwindow.ui

TRANSLATIONS += \
    vkconfig_en_US.ts


win32: {
  LIBS += Advapi32.lib
  LIBS += Cfgmgr32.lib
  RC_ICONS = resourcefiles/vulkan.ico
}

macx: {
#CONFIG += file_copies
#COPIES += shellScript
#shellScript.files = $$files(macOS/vkconfig.sh)
#shellScript.path = $$OUT_PWD/VkConfig.app/Contents/MacOS

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

QMAKE_INFO_PLIST = macOS/Info.plist
RC_FILE = macOS/Resources/LunarGIcon.icns
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
