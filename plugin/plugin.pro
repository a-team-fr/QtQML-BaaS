TEMPLATE = lib
QT += qml network
CONFIG += plugin c++11
TARGET = baasplugin


DESTDIR = $$[QT_INSTALL_QML]/BaaS

HEADERS += \
    src/baas.h \
    src/baasplugin.h \ 
    src/parse.h \
    src/baasmodel.h

SOURCES += \
    src/baasplugin.cpp \
    src/baas.cpp \ 
    src/parse.cpp \
    src/baasmodel.cpp

INCLUDEPATH += src

DISTFILES += \
    imports/BaaS/qmldir \
    imports/BaaS/baas.qmltypes

pluginfiles.files += \
    /Users/charby/hubiC/Developpement/Projets/Baas/plugin/imports/BaaS/qmldir
pluginfiles.path = $$[QT_INSTALL_QML]/BaaS

INSTALLS += pluginfiles

#FIXME : have qmldir copied to plugins folder
