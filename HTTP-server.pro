QT += core \
    network \
    testlib
QT -= gui

CONFIG += c++11

TARGET = HTTP-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    mysocket.cpp \
    httprequest.cpp \
    unittests/testhttprequest.cpp \
    unittests/testmysocket.cpp \
    mythread.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    server.h \
    mysocket.h \
    httprequest.h \
    unittests/testhttprequest.h \
    unittests/testmysocket.h \
    mythread.h

DISTFILES += \
    unittests/requestline

