#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/firebase.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/cloudant.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/bluemix.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/parse.qml")));

    return app.exec();
}
