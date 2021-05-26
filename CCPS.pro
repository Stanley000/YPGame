QT += core gui network widgets

TEMPLATE = app
TARGET = client

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

CONFIG += c++11

SOURCES += \
    clientstuff.cpp \
    functions.cpp \
    main.cpp \
    mainwindow.cpp \
    registrwindow.cpp \
    work.cpp

HEADERS += \
    clientstuff.h \
    functions.h \
    mainwindow.h \
    registrwindow.h \
    work.h

FORMS += \
    mainwindow.ui \
    registrwindow.ui \
    work.ui

RESOURCES += \
    resources.qrc
