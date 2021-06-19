QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_unittest.cpp \
    E:/qt/YPGame/CCPS/functions.cpp

HEADERS += \
    E:/qt/YPGame/CCPS/functions.h
