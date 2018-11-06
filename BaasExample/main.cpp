#include <QGuiApplication>
#include <QQmlApplicationEngine>

//TODO : gérer le cas de request concurrentes (id unique de request ?)
//TODO : pull up to refresh /listview
//TODO : social login
//TODO : push notification
//todo : affichage user extended
//todo : profile
//todo : data
//todo : dynamic backend loading
//todo : sauvegarder les dimensions de la fenetre
//todo : accueil

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("BaasExample");


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterSingletonType( QUrl("qrc:/qml/singleton/Assets.qml"),"BaaS.Example.Assets", 1, 0,"Assets");
    engine.addImportPath("qrc:///");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
