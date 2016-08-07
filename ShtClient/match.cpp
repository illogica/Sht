#include "match.h"

Match::Match(QObject *parent) : QObject(parent)
{

}

void Match::addPlayer(Player *p)
{
    m_players.append(p);
    emit playersNumberChanged(m_players.size());
}

QString Match::uuid() const
{
    return m_uuid;
}

void Match::setUuid(const QString &uuid)
{
    if(m_uuid!=uuid){
        m_uuid = uuid;
        emit uuidChanged(uuid);
    }
}

QString Match::ownerName() const
{
    return m_owner->name();
}

int Match::playersNumber() const
{
    return m_players.size();
}

int Match::matchState() const
{
    return m_matchState;
}

void Match::setMatchState(int state)
{
    if(m_matchState!=state){
        m_matchState = state;
        emit matchStateChanged(state);
    }
}

Player *Match::owner() const
{
    return m_owner;
}

void Match::setOwner(Player *owner)
{
    m_owner = owner;
    emit ownerNameChanged(m_owner->name());
}

QList<QObject *> Match::players() const
{
    return m_players;
}

