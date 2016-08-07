#include <QCoreApplication>
#include "shtserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ShtServer *srv = new ShtServer();

    //Q_UNUSED(srv);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, srv, &ShtServer::onQuit);

    return a.exec();
}
