QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17
CONFIG += sdk_no_version_check

INCLUDEPATH += ../external/Vulkan-Headers/include
INCLUDEPATH += ../Vulkan-Headers/include

# Ignore JSON validation
DEFINES += JSON_VALIDATION_OFF

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS NOMINMAX

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

Release: DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

linux: QMAKE_CXXFLAGS += -Wunused-variable

SOURCES += \
    ../vkconfig_core/alert.cpp \
    ../vkconfig_core/application.cpp \
    ../vkconfig_core/application_singleton.cpp \
    ../vkconfig_core/bitfield.cpp \
    ../vkconfig_core/command_line.cpp \
    ../vkconfig_core/configuration.cpp \
    ../vkconfig_core/configuration_info.cpp \
    ../vkconfig_core/configuration_manager.cpp \
    ../vkconfig_core/configurator.cpp \
    ../vkconfig_core/configurator_signal.cpp \
    ../vkconfig_core/date.cpp \
    ../vkconfig_core/doc.cpp \
    ../vkconfig_core/environment.cpp \
    ../vkconfig_core/header.cpp \
    ../vkconfig_core/help.cpp \
    ../vkconfig_core/json.cpp \
    ../vkconfig_core/json_validator.cpp \
    ../vkconfig_core/layer.cpp \
    ../vkconfig_core/layer_manager.cpp \
    ../vkconfig_core/layer_preset.cpp \
    ../vkconfig_core/parameter.cpp \
    ../vkconfig_core/path.cpp \
    ../vkconfig_core/registry.cpp \
    ../vkconfig_core/setting.cpp \
    ../vkconfig_core/setting_bool.cpp \
    ../vkconfig_core/setting_filesystem.cpp \
    ../vkconfig_core/setting_flags.cpp \
    ../vkconfig_core/setting_float.cpp \
    ../vkconfig_core/setting_frames.cpp \
    ../vkconfig_core/setting_group.cpp \
    ../vkconfig_core/setting_int.cpp \
    ../vkconfig_core/setting_list.cpp \
    ../vkconfig_core/setting_string.cpp \
    ../vkconfig_core/type_hide_message.cpp \
    ../vkconfig_core/type_layer_control.cpp \
    ../vkconfig_core/type_layer_type.cpp \
    ../vkconfig_core/type_layers_mode.cpp \
    ../vkconfig_core/type_log.cpp \
    ../vkconfig_core/type_platform.cpp \
    ../vkconfig_core/type_status.cpp \
    ../vkconfig_core/type_tab.cpp \
    ../vkconfig_core/type_update.cpp \
    ../vkconfig_core/util.cpp \
    ../vkconfig_core/version.cpp \
    ../vkconfig_core/vulkan_info.cpp \
    ../vkconfig_core/vulkan_util.cpp \
    configuration_layer_widget.cpp \
    widget_preset.cpp \
    widget_setting.cpp \
    widget_setting_bool.cpp \
    widget_setting_enum.cpp \
    widget_setting_filesystem.cpp \
    widget_setting_flags.cpp \
    widget_setting_float.cpp \
    widget_setting_int.cpp \
    widget_setting_frames.cpp \
    widget_setting_string.cpp \
    widget_setting_list_element.cpp \
    widget_setting_list.cpp \
    widget_tree_friendly_combobox.cpp \
    dialog_vulkan_analysis.cpp \
    dialog_vulkan_info.cpp \
    main.cpp \
    mainwindow.cpp \
    settings_tree.cpp

HEADERS += \
    ../vkconfig_core/alert.h \
    ../vkconfig_core/application.h \
    ../vkconfig_core/application_singleton.h \
    ../vkconfig_core/bitfield.h \
    ../vkconfig_core/command_line.h \
    ../vkconfig_core/configuration.h \
    ../vkconfig_core/configuration_info.h \
    ../vkconfig_core/configuration_manager.h \
    ../vkconfig_core/configurator.h \
    ../vkconfig_core/configurator_signal.h \
    ../vkconfig_core/date.h \
    ../vkconfig_core/doc.h \
    ../vkconfig_core/environment.h \
    ../vkconfig_core/header.h \
    ../vkconfig_core/help.h \
    ../vkconfig_core/json.h \
    ../vkconfig_core/json_validator.h \
    ../vkconfig_core/layer.h \
    ../vkconfig_core/layer_manager.h \
    ../vkconfig_core/layer_preset.h \
    ../vkconfig_core/parameter.h \
    ../vkconfig_core/path.h \
    ../vkconfig_core/registry.h \
    ../vkconfig_core/setting.h \
    ../vkconfig_core/setting_bool.h \
    ../vkconfig_core/setting_filesystem.h \
    ../vkconfig_core/setting_flags.h \
    ../vkconfig_core/setting_float.h \
    ../vkconfig_core/setting_frames.h \
    ../vkconfig_core/setting_group.h \
    ../vkconfig_core/setting_int.h \
    ../vkconfig_core/setting_list.h \
    ../vkconfig_core/setting_string.h \
    ../vkconfig_core/type_hide_message.h \
    ../vkconfig_core/type_layer_control.h \
    ../vkconfig_core/type_layer_type.h \
    ../vkconfig_core/type_layers_mode.h \
    ../vkconfig_core/type_log.h \
    ../vkconfig_core/type_platform.h \
    ../vkconfig_core/type_status.h \
    ../vkconfig_core/type_tab.h \
    ../vkconfig_core/type_update.h \
    ../vkconfig_core/util.h \
    ../vkconfig_core/version.h \
    ../vkconfig_core/vulkan_info.h \
    ../vkconfig_core/vulkan_util.h \
    configuration_layer_widget.h \
    widget_preset.h \
    widget_setting.h \
    widget_setting_bool.h \
    widget_setting_enum.h \
    widget_setting_filesystem.h \
    widget_setting_flags.h \
    widget_setting_float.h \
    widget_setting_int.h \
    widget_setting_frames.h \
    widget_setting_string.h \
    widget_setting_list_element.h \
    widget_setting_list.h \
    widget_tree_friendly_combobox.h \
    dialog_vulkan_analysis.h \
    dialog_vulkan_info.h \
    mainwindow.h \
    settings_tree.h

FORMS += \
    dialog_vulkan_analysis.ui \
    dialog_vulkan_info.ui \
    mainwindow.ui

TRANSLATIONS += \
    vkconfig_en_US.ts


win32: {
  LIBS += Advapi32.lib
  LIBS += Cfgmgr32.lib
  LIBS += User32.lib
  RC_ICONS = resourcefiles/vulkan.ico
}

macx: {
#CONFIG += file_copies
#COPIES += shellScript
#shellScript.files = $$files(macOS/vkconfig.sh)
#shellScript.path = $$OUT_PWD/VkConfig.app/Contents/MacOS

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

QMAKE_INFO_PLIST = macOS/Info.plist
RC_FILE = macOS/Resources/VulkanIcon.icns
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc