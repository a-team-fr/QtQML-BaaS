#include <QGuiApplication>
#include <QQmlApplicationEngine>

//TODO : gérer le cas de request concurrentes (id unique de request ?)
//TODO : pull up to refresh /listview
//TODO : social login
//TODO : push notification
//TODO : affichage user extended
//TODO : profile
//TODO : data
//TODO : dynamic backend loading
//TODO : accueil
//TODO : detecter reseau
#include "./BaaS/baas_register.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("BaasExample");


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


//#if defined(Q_OS_ANDROID)
    //TODO : fix registering via registerFilterTypes() Q_COREAPP_STARTUP_FUNCTION (not called with Android?)
    //registerFilterTypes();
//#endif

    qmlRegisterSingletonType( QUrl("qrc:/qml/singleton/Assets.qml"),"BaaS.Example.Assets", 1, 0,"Assets");

#ifdef APPVERSION
    engine.rootContext()->setContextProperty( "appVersion", QString( APPVERSION ));
#endif

    engine.addImportPath("qrc:///");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
