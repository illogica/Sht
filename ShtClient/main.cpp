#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "shtclient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Illogica1");
    QCoreApplication::setOrganizationDomain("Illogicasoftware.com1");
    QCoreApplication::setApplicationName("Sht Client");
    QQmlApplicationEngine engine;

    ShtClient *client = new ShtClient(0, engine.rootContext());
    engine.rootContext()->setContextProperty("Sht", client);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
