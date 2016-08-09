#include "shtserver.h"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include "deck.h"

ShtServer::ShtServer(QObject *parent) : QObject(parent)
{
    dbManager = new DbManager(this);
    socketServer = new QWebSocketServer(QStringLiteral("Hca Server"), QWebSocketServer::NonSecureMode, this);
    if(socketServer->listen(QHostAddress::Any, PORT)){
        connect(socketServer, &QWebSocketServer::newConnection, this, &ShtServer::onNewConnection);
        connect(socketServer, &QWebSocketServer::closed, this, &ShtServer::onConnectionClosed);
        qWarning() << "Server listening on port " << PORT;
    }
}

void ShtServer::onNewConnection()
{
    QWebSocket *socket = socketServer->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &ShtServer::onTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &ShtServer::onSocketDisconnected);
    //onlineSockets << socket;
    qWarning() << "Added socket " << socket->peerName() << ":" << socket->peerPort();
}

void ShtServer::onConnectionClosed()
{
    qWarning() << "Main connection closed.";
}

void ShtServer::onTextMessage(QString msg)
{
    QElapsedTimer totalTime;
    totalTime.start();

    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qWarning() << "received: " << msg << "from " << socket->peerName();

    //Json
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if(!doc.isObject()){
        socket->sendTextMessage("json or gtfo");
        return;
    }

    QJsonObject docObj = doc.object();
    int r = docObj["request"].toInt();
    switch(r){
    case 0:     //LOGIN
    {
        QString uuid = docObj["uuid"].toString();
        User* u = dbManager->findUserByUuid(uuid);
        if(!u){
            //DEFAULTS FOR NEW USERS
           u = new User();
           u->setUuid(QUuid::createUuid().toString());
           u->setName("anonimo");
           u->setEmail("");
           u->setMatches(0);
           u->setKilos(0);
           u->setState(User::AVAILABLE);
           u->setAvatar("263a");
           dbManager->addUser(u);

           u->setSocket(socket);
        }
        usersBySocket.insert(socket, u);

        QJsonObject obj = u->getJsonObject();
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray msg = doc.toJson(QJsonDocument::Compact);
        socket->sendTextMessage(msg);
    }break;

    case 1:     //GET MATCHES
    {
        User *u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "WARNING: getMatches from unknown user";
            break;
        }

        QJsonArray matchesList;
        for(Match* m: matches){
            QJsonObject matchObj = m->getJsonObject();
            matchesList.append(matchObj);
        }

        QJsonObject obj;
        obj["request"] = 1;
        obj["matchesList"] = matchesList;
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray msg = doc.toJson(QJsonDocument::Compact);
        socket->sendTextMessage(msg);
    }break;
    case 2:     //CREATE MATCH
    {
        User* u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "ERROR: createMatch() user not found";
            break;
        }

        if(u->getPendingMatch() != NULL){
            qWarning() << "SKIPPING: user already has a match pending";
            break;
        }

        Match *m = new Match(this);
        m->init(u);
        m->addPlayer(u);
        matches.append(m);
        u->setPendingMatch(m);

        QJsonObject obj;
        obj["request"] = 2;
        obj["result"] = "ok";
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray msg = doc.toJson(QJsonDocument::Compact);
        socket->sendTextMessage(msg);
        connect(m, &Match::finished, this, &ShtServer::onMatchFinished);
    }break;
    case 3:     //JOIN MATCH
    {
        User* u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "ERROR: joinMatch() user not found";
            break;
        }

        Match* m = findMatch(docObj["uuid"].toString());
        if(!m){
            qWarning() << "ERROR: match not found";
            break;
        }
        m->addPlayer(u);
        u->setPendingMatch(m);
    }break;
    case 4:     //LEAVE MATCH
    {
        User* u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "ERROR: leaveMatch() user not found";
            break;
        }

        Match* m = findMatch(docObj["uuid"].toString());
        if(!m){
            qWarning() << "ERROR: match not found";
            break;
        }
        m->leavePlayer(u);
    }break;
    case 5:     //PASS CARD
    {
        User* u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "ERROR: passCard user not found";
            break;
        }

        Match* m = findMatch(docObj["uuid"].toString());
        if(!m){
            qWarning() << "ERROR: match not found";
            break;
        }
        m->passCard(u, docObj["card"].toString());
    }break;
    case 6:     //START MATCH
    {
        User* u = usersBySocket.value(socket);
        if(!u){
            qWarning() << "ERROR: matchStart user not found";
            break;
        }

        Match* m = findMatch(docObj["uuid"].toString());
        if(!m){
            qWarning() << "ERROR: match not found";
            break;
        }

        //Only match creator can start the match
        if(m->state()==Match::INIT && m->getOwner() == u){
            m->start();
        } else {
            u->sendMessage("match_server", "start_game", "denied", "");
        }

    }break;

    default:
        break;
    }
    qWarning() << "Request " << r << " evaded in " << totalTime.elapsed() << "ms";
}

void ShtServer::onSocketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    User* u = usersBySocket.value(socket);
    if(u->getPendingMatch() != NULL){
        u->getPendingMatch()->leavePlayer(u);
    }
    u->setState(User::OFFLINE);
    dbManager->saveUser(u);
    usersBySocket.remove(socket);
    u->deleteLater();
    qWarning() << "onSocketDisconnected";
}

void ShtServer::onQuit()
{
    qDebug() << "onQuit";
}

void ShtServer::onMatchFinished(const QString &uuid)
{
    qWarning() << "OnMatchFinished, uuid: " << uuid;
    Match *m = findMatch(uuid);
    matches.removeOne(m);
    m->deleteLater();
}

Match *ShtServer::findMatch(QString uuid)
{
    for(Match *m: matches){
        if(m->uuid == uuid)
            return m;
    }
    return NULL;
}

User *ShtServer::findLoggedUser(int id)
{
    for(User* u : usersBySocket.values()){
        if(u->id() == id){
            return u;
        }
    }
    return NULL;
}


