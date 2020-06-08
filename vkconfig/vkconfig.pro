QT       += core gui \
    quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += sdk_no_version_check


macx: INCLUDEPATH += /Users/rwright/SDKs/vulkansdk/MoltenVK/include

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


linux: QMAKE_CXXFLAGS += -Wunused-variable

SOURCES += \
    dlgabout.cpp \
    dlgbetamessage.cpp \
    dlgcreateassociation.cpp \
    dlgcustompaths.cpp \
    dlghelp.cpp \
    dlghistory.cpp \
    dlglayeroutput.cpp \
    dlgprofileeditor.cpp \
    dlgvulkananalysis.cpp \
    dlgvulkaninfo.cpp \
    khronossettings.cpp \
    khronossettingsadvanced.cpp \
    layerfile.cpp \
    main.cpp \
    mainwindow.cpp \
    profiledef.cpp \
    settingseditor.cpp \
    settingstreemanager.cpp \
    vulkanconfiguration.cpp

HEADERS += \
    dlgabout.h \
    dlgbetamessage.h \
    dlgcreateassociation.h \
    dlgcustompaths.h \
    dlghelp.h \
    dlghistory.h \
    dlglayeroutput.h \
    dlgprofileeditor.h \
    dlgvulkananalysis.h \
    dlgvulkaninfo.h \
    khronossettings.h \
    khronossettingsadvanced.h \
    layerfile.h \
    mainwindow.h \
    profiledef.h \
    settingseditor.h \
    settingstreemanager.h \
    vulkanconfiguration.h

FORMS += \
    dlgabout.ui \
    dlgbetamessage.ui \
    dlgcreateassociation.ui \
    dlgcustompaths.ui \
    dlghelp.ui \
    dlghistory.ui \
    dlglayeroutput.ui \
    dlgprofileeditor.ui \
    dlgvulkananalysis.ui \
    dlgvulkaninfo.ui \
    khronossettings.ui \
    khronossettingsadvanced.ui \
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
