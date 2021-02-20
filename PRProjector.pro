#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T03:40:16
#
#-------------------------------------------------

QT += core gui widgets multimedia multimediawidgets qml quick

TARGET = PRProjector
TEMPLATE = app

CONFIG += sdk_no_version_check
CONFIG += c++11

SOURCES = $$files(code/*.cpp, true)
HEADERS = $$files(code/*.hpp, true)
FORMS = $$files(code/*.ui, true)

RESOURCES = code/Resources.qrc

OTHER_FILES += code/resources/qml/ProjectorScreen.qml

win32 {
    RC_FILE = code/App.rc
}

macx {
    # compilation
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_OBJECTIVE_CFLAGS_RELEASE =  $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

    # platform
    #CONFIG += x86_64
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    # debug dSYM
    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2

    # plist
    QMAKE_INFO_PLIST = $${PWD}/code/resources/macos/Info.plist

    # icon
    ICON = $${PWD}/code/resources/icons/app.icns
}
