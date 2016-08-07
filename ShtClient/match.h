#ifndef MATCH_H
#define MATCH_H

#include <QObject>
#include "player.h"

class Match : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(int matchState READ matchState WRITE setMatchState NOTIFY matchStateChanged)
    Q_PROPERTY(QString ownerName READ ownerName NOTIFY ownerNameChanged)
    Q_PROPERTY(int playersNumber READ playersNumber NOTIFY playersNumberChanged)
public:
    explicit Match(QObject *parent = 0);
    void addPlayer(Player* p);

    QString uuid() const;
    void setUuid(const QString &uuid);

    QString ownerName() const;
    int playersNumber() const;

    int matchState() const;
    void setMatchState(int matchState);

    Player *owner() const;
    void setOwner(Player *owner);

    QList<QObject *> players() const;

    enum states{INIT, STARTED, PAUSED, FINISHED};

signals:
    void uuidChanged(const QString uuid);
    void matchStateChanged(int matchState);
    void ownerNameChanged(const QString name);
    void playersNumberChanged(int matchState);

public slots:

private:
    QString m_uuid;
    int m_matchState;
    Player* m_owner;
    QList<QObject*> m_players; //the dataModel for the players in the match
};

#endif // MATCH_H
