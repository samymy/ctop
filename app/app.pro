QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../ctop
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/ctop.cpp

INCLUDEPATH += include ../view/include ../model/include

HEADERS += include/imodel.h \
    include/iview.h \
    include/process.h

LIBS += -L../model -lmodel
LIBS += -L../view -lview
