#ifndef USER_H
#define USER_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include "card.h"
//#include "match.h"

class Match;
class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent =0);

    QJsonObject getJsonObject();
    void addPendingMatch(Match* m);
    void removePendingMatch(Match* m);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString uuid() const;
    void setUuid(const QString &uuid);

    QString email() const;
    void setEmail(const QString &email);

    QWebSocket *socket() const;
    void setSocket(QWebSocket *socket);

    void sendMessage(QString sender, QString message, QString arg1, QString arg2);
    void clearCards();
    void pushCard(Card *c);
    Card* popCard(QString card);

    int matches() const;
    void setMatches(int matches);

    int kilos() const;
    void setKilos(int kilos);

    enum states{OFFLINE, AVAILABLE, PLAYING};

    int state() const;
    void setState(int value);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    Match *getPendingMatch() const;
    void setPendingMatch(Match *pendingMatch);

signals:

public slots:

private:
    int m_id;
    QString m_name;
    QString m_uuid;
    QString m_email;
    int m_matches;
    int m_kilos;
    int m_state;
    QString m_avatar;

    Match* m_pendingMatch; //matches the user is playing

    QWebSocket* m_socket;
    QList<Card *> m_matchCards; // match uuid and cards

};

#endif // USER_H
