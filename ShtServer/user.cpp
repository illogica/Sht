#include "user.h"
#include <QJsonObject>
#include <QJsonDocument>

User::User(QObject *parent) : QObject(parent)
{
}

QJsonObject User::getJsonObject()
{
    QJsonObject obj;
    obj["request"] = 0;
    obj["name"] = m_name;
    obj["matches"] = m_matches;
    obj["kilos"] = m_kilos;
    obj["uuid"] = m_uuid;
    obj["id"] = m_id;
    obj["avatar"] = m_avatar;
    return obj;
}

void User::addPendingMatch(Match *m)
{
    m_pendingMatches.append(m);
}

void User::removePendingMatch(Match *m)
{
    m_pendingMatches.removeOne(m);
}

int User::id() const
{
    return m_id;
}

void User::setId(int id)
{
    m_id = id;
}

QString User::name() const
{
    return m_name;
}

void User::setName(const QString &name)
{
    m_name = name;
}

QString User::uuid() const
{
    return m_uuid;
}

void User::setUuid(const QString &uuid)
{
    m_uuid = uuid;
}

QString User::email() const
{
    return m_email;
}

void User::setEmail(const QString &email)
{
    m_email = email;
}

QWebSocket *User::socket() const
{
    return m_socket;
}

void User::setSocket(QWebSocket *socket)
{
    m_socket = socket;
}

int User::matches() const
{
    return m_matches;
}

void User::setMatches(int matches)
{
    m_matches = matches;
}

int User::kilos() const
{
    return m_kilos;
}

void User::setKilos(int kilos)
{
    m_kilos = kilos;
}

int User::state() const
{
    return m_state;
}

void User::setState(int value)
{
    m_state = value;
}

QString User::avatar() const
{
    return m_avatar;
}

void User::setAvatar(const QString &avatar)
{
    m_avatar = avatar;
}

QList<Match *> User::getPendingMatches() const
{
    return m_pendingMatches;
}

void User::setPendingMatches(const QList<Match *> &pendingMatches)
{
    m_pendingMatches = pendingMatches;
}

void User::sendMessage(QString sender, QString message, QString arg1, QString arg2)
{
    QJsonObject obj;
    obj["request"] = 101; // PLAYER MESSAGE
    obj["sender"] = sender;
    obj["message"] = message;
    obj["sarg1"] = arg1;
    obj["sarg2"] = arg2;
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray msg = doc.toJson(QJsonDocument::Compact);
    if(!m_socket){
        qWarning() << "m_socket is null!";
    } else {
        m_socket->sendTextMessage(msg);
    }
}

void User::clearCards()
{
    while(!m_matchCards.isEmpty()) delete m_matchCards.takeFirst();
}

void User::pushCard(Card *c)
{
    m_matchCards.append(c);
    sendMessage("match_server", "have_card", c->toString(), "");
}

Card *User::popCard(QString card)
{
    for(Card* c : m_matchCards){
        if(c->name() == card){
            m_matchCards.removeOne(c);
            return c;
        }
    }
    return nullptr;
}
