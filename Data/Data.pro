TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    datadecoder.cpp \
    datacontainer.cpp \
    datagenerator.cpp

HEADERS += \
    datadecoder.h \
    datacontainer.h \
    datagenerator.h