QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_gomodel.cpp \
    gomodel.cpp \
    godataaccess.cpp

HEADERS += \
    gomodel.h \
    godataaccess.h \
    player.h \
    godataaccessmock.h
