#ifndef BAASPLUGIN_H
#define BAASPLUGIN_H

#include <QQmlExtensionPlugin>

class BaasPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qt.playground.baas")

public:
    //void initializeEngine(QQmlEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // BAASPLUGIN_H
