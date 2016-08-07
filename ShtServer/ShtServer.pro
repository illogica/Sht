QT += core websockets sql
QT -= gui

CONFIG += c++11

TARGET = ShtServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    shtserver.cpp \
    dbmanager.cpp \
    user.cpp \
    card.cpp \
    deck.cpp \
    match.cpp

HEADERS += \
    shtserver.h \
    dbmanager.h \
    user.h \
    card.h \
    deck.h \
    match.h
