#ifndef SHTCLIENT_H
#define SHTCLIENT_H

#include <QObject>
#include <QQmlContext>
#include <QSettings>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include "match.h"

class ShtClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
public:
    explicit ShtClient(QObject *parent = 0, QQmlContext *c = 0);
    void endMatch();

    QString uuid; //temporary!

signals:
    void connectedChanged(bool connected);
    void activeChanged(bool active);
    void qmlAddMatch(QString ownerName, int playersNumber, int matchState, QString uuid);
    void qmlResetMatches();
    void qmlOpenMatch(const QString &uuid);
    void qmlAddChatMessage(const QString &message);
    void qmlChatReset();
    void qmlCardsReset();
    void qmlHaveCard(const QString &card);
    void qmlAddPlayerToModel(QString name, int kilos, QString avatar);
    void qmlPlayersModelReset();
    void qmlMatchStart();

public slots:
    void onConnected();
    void onDisconnected();
    void parseServerMessage(const QString &message);
    void tryConnect(bool active);
    void sendGetMatches();
    void sendCreateMatch();
    void sendJoinMatch(const QString &uuid);
    void sendLeaveMatch();
    void sendPassCard(const QString &card);
    void sendStartMatch();

    //Properties stuff
    bool active();
    void setActive(bool active);

    bool connected();

private:
    void establish();
    void sendLogin();
    void fromJsonObject(QJsonObject obj, Player* p);
    void fromJsonObject(QJsonObject obj, Match* m);

    QList<QObject*> m_matches;

    QList<QString> currentCards;
    bool inMatch = false;
    Match currentMatch;

    QQmlContext* ctx;
    QSettings settings;
    QWebSocket socket;
    bool m_active;
    bool m_connected;

};

#endif // SHTCLIENT_H
