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
    appsingleton.cpp \
    boolsettingwidget.cpp \
    dlgabout.cpp \
    dlgcreateassociation.cpp \
    dlgcustompaths.cpp \
    dlgprofileeditor.cpp \
    dlgvulkananalysis.cpp \
    dlgvulkaninfo.cpp \
    enumsettingwidget.cpp \
    filenamesettingwidget.cpp \
    foldersettingwidget.cpp \
    khronossettingsadvanced.cpp \
    layerfile.cpp \
    main.cpp \
    mainwindow.cpp \
    multienumsetting.cpp \
    mutemessagewidget.cpp \
    configuration.cpp \
    preferences.cpp \
    settingstreemanager.cpp \
    stringsettingwidget.cpp \
    treefriendlycombobox.cpp \
    vuidsearchwidget.cpp \
    configurator.cpp \
    command_line.cpp \
    vku.cpp \
    test.cpp

HEADERS += \
    appsingleton.h \
    boolsettingwidget.h \
    dlgabout.h \
    dlgcreateassociation.h \
    dlgcustompaths.h \
    dlgprofileeditor.h \
    dlgvulkananalysis.h \
    dlgvulkaninfo.h \
    enumsettingwidget.h \
    filenamesettingwidget.h \
    foldersettingwidget.h \
    khronossettingsadvanced.h \
    layerfile.h \
    mainwindow.h \
    multienumsetting.h \
    mutemessagewidget.h \
    configuration.h \
    preferences.h \
    settingstreemanager.h \
    stringsettingwidget.h \
    treefriendlycombobox.h \
    vuidsearchwidget.h \
    configurator.h \
    command_line.h \
    vku.h \
    test.h

FORMS += \
    dlgabout.ui \
    dlgcreateassociation.ui \
    dlgcustompaths.ui \
    dlgprofileeditor.ui \
    dlgvulkananalysis.ui \
    dlgvulkaninfo.ui \
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
