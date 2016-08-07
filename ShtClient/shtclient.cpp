#include "shtclient.h"
#include <QJsonArray>

ShtClient::ShtClient(QObject *parent, QQmlContext *ctx) : QObject(parent)
{
    this->ctx = ctx;

    connect(this, &ShtClient::activeChanged, this, &ShtClient::tryConnect);

    connect(&socket, &QWebSocket::connected, this, &ShtClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &ShtClient::onDisconnected);
    connect(&socket, &QWebSocket::textMessageReceived, this, &ShtClient::parseServerMessage);
}

void ShtClient::establish(){
    socket.open(QUrl(QStringLiteral("ws://192.168.1.232:8081")));
}

void ShtClient::onConnected()
{
    qWarning() << "onConnected()";
    m_connected = true;
    emit connectedChanged(true);
    //socket.sendTextMessage(makePing().toJson(QJsonDocument::Compact));
    sendLogin();
}

void ShtClient::onDisconnected()
{
    qWarning() << "onDisconnected()";
    m_connected = false;
    emit connectedChanged(false);
}

void ShtClient::parseServerMessage(const QString &message)
{
    qWarning() << "received: " << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if(!doc.isObject()){
        qWarning() << "Invalid json: " << message;
        return;
    }

    QJsonObject docObj = doc.object();
    int r = docObj["request"].toInt();
    switch(r){
    case 0: //LOGIN
    {
        //QString uuid = docObj["uuid"].toString(); //USE IN PRODUCTION
        uuid = docObj["uuid"].toString(); //USE IN TESTING
        QString name = docObj["name"].toString();
        int id = docObj["id"].toInt();
        int kilos = docObj["kilos"].toInt();
        int matches = docObj["matches"].toInt();
        QString avatar = docObj["avatar"].toString();

        //settings.setValue("uuid", uuid); //USE IN PRODUCTION
        settings.setValue("name", name);
        settings.setValue("kilos", kilos);
        settings.setValue("matches", matches);
        settings.setValue("id", id);
        settings.setValue("avatar", avatar);
        qWarning() << "new Uuid: " << uuid;
    } break;
    case 1: //GET MATCHES
    {
        QJsonArray matches = docObj["matchesList"].toArray();

        //Clear the list we already have
        while(!m_matches.isEmpty()){delete(m_matches.takeFirst());}

        //repopulate the list of matches
        QJsonArray::iterator it;
        for(it = matches.begin(); it!=matches.end(); it++){
            QJsonObject obj = (*it).toObject();
            Match* m = new Match(this);
            fromJsonObject(obj, m);
            m_matches.append(m);
            emit qmlAddMatch(m->owner()->name(), m->playersNumber(), m->matchState(), m->uuid());
        }
        //let's not do this and use Signal/slot connections instead
        //ctx->setContextProperty("matchesModel", QVariant::fromValue(m_matches));
    } break;
    case 100: //MATCH MESSAGE
    {
        QString sender = docObj["sender"].toString();
        QString message = docObj["message"].toString();
        QString sarg1 = docObj["sarg1"].toString();
        QString sarg2 = docObj["sarg2"].toString();

        if(sender == "match_server"){
            if(message=="joined"){
                QString s("<b>" + sarg2 + " si è aggiunto");
                emit qmlAddChatMessage(s);
            }
            else if (message=="left"){
                QString s("<b>" + sarg2 + " si è disconnesso");
                emit qmlAddChatMessage(s);
            }
            else if (message=="finish"){
                inMatch = false;
                currentMatch.setMatchState(Match::FINISHED);
                emit qmlAddChatMessage("La partita è finita");
            }
            else if (message=="start"){
                currentMatch.setMatchState(Match::STARTED);
                emit qmlAddChatMessage("La partita è iniziata!");
            }
        }
    } break;
    case 101: //PLAYER MESSAGE
    {
        QString sender = docObj["sender"].toString();
        QString message = docObj["message"].toString();
        QString sarg1 = docObj["sarg1"].toString();
        QString sarg2 = docObj["sarg2"].toString();

        if(sender == QStringLiteral("match_server")){
            if(message == "join"){
                if(sarg2 == "ok"){
                    inMatch = true;
                    currentCards.clear();
                    currentMatch.setUuid(sarg1);
                    currentMatch.setMatchState(Match::INIT);
                    emit qmlOpenMatch(uuid);
                } else {
                    qWarning() << "Join denied";
                }
            }
            else if(message == "have_card"){
                currentCards.append(sarg1);
                emit qmlHaveCard(sarg1);
            }
        }
    } break;

    default:
        qWarning() << "Unrecognized request: " << r;
    }
}

void ShtClient::tryConnect(bool active)
{
    if(active && !m_connected){
        establish();
    }
}

bool ShtClient::active()
{
    return m_active;
}

void ShtClient::setActive(bool active)
{
    if(m_active!=active){
        m_active = active;

        if(active==false){
            socket.close();
        } else {
            //establish();
        }

        emit activeChanged(active);
    }
}

bool ShtClient::connected()
{
    return m_connected;
}

void ShtClient::sendLogin()
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 0; // LOGIN
        obj["uuid"] = settings.value("uuid", "").toString();
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendGetMatches()
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 1; // GET MATCHES
        obj["id"] = settings.value("id", 0).toInt();
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendCreateMatch()
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 2; // CREATE MATCH
        obj["id"] = settings.value("id", 0).toInt(); //not actually used on the server
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendJoinMatch(const QString &uuid)
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 3; // JOIN MATCH
        obj["id"] = settings.value("id", 0).toInt(); //not actually used on the server
        obj["uuid"] = uuid;
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendLeaveMatch()
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 4; // LEAVE MATCH
        obj["id"] = settings.value("id", 0).toInt(); //not actually used on the server
        obj["uuid"] = currentMatch.uuid();
        currentMatch.setMatchState(Match::FINISHED);
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendPassCard(const QString &card)
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 5; // PASS CARD
        obj["id"] = settings.value("id", 0).toInt(); //not actually used on the server
        obj["uuid"] = currentMatch.uuid(); // the match uuid
        obj["card"] = card;
        QJsonDocument doc;
        doc.setObject(obj);
        currentCards.removeOne(card);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::sendStartMatch()
{
    if(m_connected){
        QJsonObject obj;
        obj["request"] = 6; // START MATCH
        obj["id"] = settings.value("id", 0).toInt(); //not actually used on the server
        obj["uuid"] = currentMatch.uuid();
        QJsonDocument doc;
        doc.setObject(obj);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void ShtClient::fromJsonObject(QJsonObject obj, Player *p)
{
    p->setId(obj["id"].toInt());
    p->setState(obj["state"].toInt());
    p->setName(obj["name"].toString());
    p->setAvatar(obj["avatar"].toString());
    p->setMatches(obj["matches"].toInt());
    p->setKilos(obj["matches"].toInt());
}

void ShtClient::fromJsonObject(QJsonObject obj, Match *m)
{
    m->setUuid(obj["uuid"].toString());
    m->setMatchState(obj["state"].toInt());
    Player* ownr = new Player(m);
    QJsonObject ownrObj = obj["owner"].toObject();
    fromJsonObject(ownrObj, ownr);
    m->setOwner(ownr);

    QJsonArray players = obj["players"].toArray();
    QJsonArray::iterator it;
    for(it = players.begin(); it!=players.end(); it++){
        QJsonObject playerObj = (*it).toObject();
        Player* p = new Player(m);
        fromJsonObject(playerObj, p);
        m->addPlayer(p);
    }
}
