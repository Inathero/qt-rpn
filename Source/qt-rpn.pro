#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T20:35:20
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qt-rpn
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lncurses

SOURCES += main.cpp \
    qtrpncalc.cpp \
    consoleio.cpp \

HEADERS += \
    main.h \
    qtrpncalc.h \
    consoleio.h \
