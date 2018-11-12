TEMPLATE = lib
QT += qml network
CONFIG += plugin c++11
TARGET = baasplugin


DESTDIR = $$[QT_INSTALL_QML]/BaaS

HEADERS += \
    src/baasplugin.h \ 
    src/parse.h \
    src/baasmodel.h \
    #src/couchdb.h
    src/restmanager.h \
    src/baasbase.h

SOURCES += \
    src/baasplugin.cpp \
    src/parse.cpp \
    src/baasmodel.cpp \
    #src/couchdb.cpp
    src/restmanager.cpp \
    src/baasbase.cpp

INCLUDEPATH += src


DISTFILES += \
    imports/BaaS/qmldir \
    imports/BaaS/baas.qmltypes \
    imports/BaaS/BaaSListView.qml

pluginfiles.files += \
    imports/BaaS/qmldir \
    imports/BaaS/BaaSListView.qml
pluginfiles.path = $$[QT_INSTALL_QML]/BaaS

INSTALLS += pluginfiles

FIXME : have qmldir copied to plugins folder
QMAKE_POST_LINK += $$QMAKE_COPY $$replace($$list($$quote($$PWD/imports/BaaS/qmldir) $$DESTDIR), /, $$QMAKE_DIR_SEP) && $$QMAKE_COPY $$replace($$list($$quote($$PWD/imports/BaaS/BaaSListView.qml) $$DESTDIR), /, $$QMAKE_DIR_SEP)
