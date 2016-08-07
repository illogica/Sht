#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{

}

int Player::id() const
{
    return m_id;
}

void Player::setId(int id)
{
    if(m_id!=id){
        m_id = id;
        emit idChanged(id);
    }
}

int Player::state() const
{
    return m_state;
}

void Player::setState(int state)
{
    if(m_state != state){
        m_state = state;
        emit stateChanged(state);
    }
}

QString Player::name() const
{
    return m_name;
}

void Player::setName(const QString &name)
{
    if(m_name != name){
        m_name = name;
        emit nameChanged(name);
    }
}

QString Player::avatar() const
{
    return m_avatar;
}

void Player::setAvatar(const QString &avatar)
{
    if(m_avatar != avatar){
        m_avatar = avatar;
        emit avatarChanged(avatar);
    }
}

int Player::matches() const
{
    return m_matches;
}

void Player::setMatches(int matches)
{
    if(m_matches != matches){
        m_matches = matches;
        emit matchesChanged(matches);
    }
}

int Player::kilos() const
{
    return m_kilos;
}

void Player::setKilos(int kilos)
{
    if(m_kilos != kilos){
        m_kilos = kilos;
        emit kilosChanged(kilos);
    }
}

