QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = view
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/view.cpp \

INCLUDEPATH += include ../app/include

HEADERS += include/view.h \
    ../app/include/imodel.h \
    ../app/include/iview.h \
    ../app/include/process.h

FORMS    += forms/view.ui
