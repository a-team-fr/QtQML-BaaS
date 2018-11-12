CONFIG += c++11
QT += network

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/parse.h \
    $$PWD/baasmodel.h \
    $$PWD/restmanager.h \
    $$PWD/baasbase.h \
    $$PWD/baas_register.h

SOURCES += \
    $$PWD/parse.cpp \
    $$PWD/baasmodel.cpp \
    $$PWD/restmanager.cpp \
    $$PWD/baasbase.cpp
