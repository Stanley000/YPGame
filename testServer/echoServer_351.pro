QT += core gui network widgets sql

TEMPLATE = app
TARGET = server

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serverstuff.cpp

HEADERS += \
    mainwindow.h \
    serverstuff.h

FORMS += \
    mainwindow.ui
