TEMPLATE = app

QT += qml quick quickcontrols2 websockets
CONFIG += c++11

SOURCES += main.cpp \
    shtclient.cpp \
    match.cpp \
    player.cpp

RESOURCES += qml.qrc \
    emojione.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    components/qmldir

HEADERS += \
    shtclient.h \
    match.h \
    player.h
