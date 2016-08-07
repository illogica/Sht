#include "match.h"
#include <QUuid>

Match::Match(QObject *parent) : QObject(parent)
{
    uuid = QUuid::createUuid().toString();
    m_shitDeck = new Deck(this);
    m_playersDeck = new Deck(this);
}

bool Match::init(User* owner)
{
    this->m_owner = owner;

    m_state = INIT;
    return true;
}

bool Match::start()
{
    if(m_players.size() >= MINPLAYERS && m_players.size()<=MAXPLAYERS){
        delete(m_shitDeck);
        m_shitDeck = new Deck(this);
        delete(m_playersDeck);
        m_playersDeck = new Deck(m_players.size());
        distributeCards();
        matchMessage("match_server", "start", uuid, "");
        m_state = STARTED;
        return true;
    }
    return false;
}

bool Match::pause()
{
    m_state = PAUSED;
    return true;
}

bool Match::resume()
{
    m_state = STARTED;
    return true;
}

bool Match::finish()
{
    m_state = FINISHED;
    matchMessage("match_server", "finish", uuid, "");
    return true;
}

void Match::addPlayer(User *user)
{
    if(m_state==Match::INIT){
        matchMessage("match_server", "joined", uuid, user->name());
        playerMessage("match_server", user, "join", uuid, "ok");
        m_players.append(user);
        user->addPendingMatch(this);
        user->clearCards();
        sendMatchInitialDataToPlayers();
    } else {
        playerMessage("match_server", user, "join", uuid, "no");;
    }
}

void Match::leavePlayer(User *user)
{
    m_players.removeOne(user);
    user->removePendingMatch(this);

    if(m_state==Match::INIT){
        sendMatchInitialDataToPlayers();
        return;
    }

    if(m_state==Match::STARTED){
        //a user left, finish the game
        finish();
    }
}

void Match::matchMessage(QString sender, QString message, QString arg1, QString arg2)
{
    QJsonObject obj;
    obj["request"] = 100; // MATCH MESSAGE
    obj["sender"] = sender;
    obj["message"] = message;
    obj["sarg1"] = arg1;
    obj["sarg2"] = arg2;
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray msg = doc.toJson(QJsonDocument::Compact);
    for(User* u: m_players){
        u->socket()->sendTextMessage(msg);
    }
}

void Match::playerMessage(QString sender, User *user, QString message, QString arg1, QString arg2)
{
    user->sendMessage(sender, message, arg1, arg2);
}

void Match::distributeCards()
{
    for(User* u: m_players){
        for(int i=0; i<m_players.length(); i++){
            u->pushCard(m_playersDeck->popRandomCard());
        }
    }
}

void Match::passCard(User *u, QString card)
{
    //Remove the card from the current user deck
    Card* c = u->popCard(card);


    //Add the card to the following user deck
    int playersNumber = m_players.size();
    int playerId = m_players.indexOf(u);
    int nextPlayerId = playerId + 1;
    if(nextPlayerId == playersNumber)
        nextPlayerId = 0;
    m_players.at(nextPlayerId)->pushCard(c);
}

QJsonObject Match::getJsonObject()
{
    QJsonObject matchObj;
    matchObj["state"] = m_state;
    matchObj["uuid"] = uuid;
    matchObj["owner"] = m_owner->getJsonObject();
    QJsonArray a;
    for(int i=0; i<m_players.size(); i++){
        User* u = m_players.at(i);
        QJsonObject uObj = u->getJsonObject();
        uObj["order"] = i;
        a.append(uObj);
    }
    matchObj["players"] = a;
    return matchObj;
}

int Match::state() const
{
    return m_state;
}

QList<User *> Match::players() const
{
    return m_players;
}

void Match::sendMatchInitialDataToPlayers()
{
    QJsonDocument doc;
    doc.setObject(getJsonObject());
    matchMessage("match_server", "update_match", uuid, doc.toJson(QJsonDocument::Compact));
}

User *Match::getOwner() const
{
    return m_owner;
}

void Match::setOwner(User *owner)
{
    m_owner = owner;
}


