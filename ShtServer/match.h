#ifndef MATCH_H
#define MATCH_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "user.h"
#include "deck.h"
#include "card.h"

class Match : public QObject
{
    Q_OBJECT
public:
    explicit Match(QObject *parent = 0);
    ~Match();
    bool init(User* owner);
    bool start();
    bool pause();
    bool resume();
    void finish();
    void addPlayer(User* user);
    void leavePlayer(User* user);
    void matchMessage(QString sender, QString message, QString arg1, QString arg2);
    void playerMessage(QString sender, User* user, QString message, QString arg1, QString arg2);
    void distributeCards();
    void passCard(User* u, QString card);
    QJsonObject getJsonObject();

    const int MAXPLAYERS = 10;
    const int MINPLAYERS = 0; //TODO: SET TO 2 or 3
    QString uuid;

    enum states{INIT, STARTED, PAUSED, FINISHED};

    int state() const;

    QList<User *> players() const;

    User *getOwner() const;
    void setOwner(User *owner);

signals:
    void finished(const QString& uuid);

public slots:

private:
    void sendMatchInitialDataToPlayers();

    QList<User *> m_players;
    User *m_owner;
    int m_state;

    Deck *m_shitDeck;
    Deck *m_playersDeck;
};

#endif // MATCH_H
