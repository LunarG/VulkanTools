QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += sdk_no_version_check


macx: INCLUDEPATH += /Developer/vulkansdk/MoltenVK/include

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    dlgabout.cpp \
    dlgcreateassociation.cpp \
    dlgcreateprofile.cpp \
    dlgcustompaths.cpp \
    dlghistory.cpp \
    dlglayeroutput.cpp \
    dlgprofileeditor.cpp \
    dlgvulkananalysis.cpp \
    dlgvulkaninfo.cpp \
    layerfile.cpp \
    main.cpp \
    mainwindow.cpp \
    profiledef.cpp \
    settingseditor.cpp \
    vulkanconfiguration.cpp

HEADERS += \
    dlgabout.h \
    dlgcreateassociation.h \
    dlgcreateprofile.h \
    dlgcustompaths.h \
    dlghistory.h \
    dlglayeroutput.h \
    dlgprofileeditor.h \
    dlgvulkananalysis.h \
    dlgvulkaninfo.h \
    layerfile.h \
    mainwindow.h \
    profiledef.h \
    settingseditor.h \
    vulkanconfiguration.h

FORMS += \
    dlgabout.ui \
    dlgcreateassociation.ui \
    dlgcreateprofile.ui \
    dlgcustompaths.ui \
    dlghistory.ui \
    dlglayeroutput.ui \
    dlgprofileeditor.ui \
    dlgvulkananalysis.ui \
    dlgvulkaninfo.ui \
    mainwindow.ui

TRANSLATIONS += \
    vkconfig2_en_US.ts


win32: {
  LIBS += Advapi32.lib
  }

macx: {
CONFIG += file_copies
COPIES += shellScript
shellScript.files = $$files(macOS/vkconfig2.sh)
shellScript.path = $$OUT_PWD/VkConfig2.app/Contents/MacOS

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
