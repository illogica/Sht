#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(int matches READ matches WRITE setMatches NOTIFY matchesChanged)
    Q_PROPERTY(int kilos READ kilos WRITE setKilos NOTIFY kilosChanged)

public:
    explicit Player(QObject *parent = 0);

    int id() const;
    void setId(int id);

    int state() const;
    void setState(int state);

    QString name() const;
    void setName(const QString &name);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    int matches() const;
    void setMatches(int matches);

    int kilos() const;
    void setKilos(int kilos);

signals:
    void idChanged(int id);
    void stateChanged(int state);
    void nameChanged(const QString name);
    void avatarChanged(const QString avatar);
    void matchesChanged(int matches);
    void kilosChanged(int kilos);

public slots:

private:
    int m_id;
    int m_state;
    QString m_name;
    QString m_avatar;
    int m_matches;
    int m_kilos;
};

#endif // PLAYER_H
