#ifndef SHTSERVER_H
#define SHTSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "dbmanager.h"
#include "match.h"

class ShtServer : public QObject
{
    Q_OBJECT
public:
    explicit ShtServer(QObject *parent = 0);
    const int PORT = 8081;

signals:
    void sendTextMessage(const QString &message);

public slots:
    void onNewConnection();
    void onConnectionClosed();
    void onTextMessage(QString msg);
    void onSocketDisconnected();
    void onQuit();
    void onMatchFinished(const QString& uuid);

private:

    Match* findMatch(QString uuid);
    User* findLoggedUser(int id);

    QWebSocketServer *socketServer;
    DbManager *dbManager;

    QHash<QWebSocket*, User*> usersBySocket;
    QList<Match *> matches;
};

#endif // SHTSERVER_H
