TARGET = model
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/model.cpp \
    src/processesinfo.cpp

INCLUDEPATH += include ../app/include

HEADERS += include/model.h \
    include/processesinfo.h \
    ../app/include/imodel.h \
    ../app/include/iview.h \
    ../app/include/process.h
