#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include "user.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = 0);
    User* findUserByUuid(QString uuid);
    User *findUserById(int id);
    void saveUser(User* u);
    void addUser(User* u);

signals:

public slots:


private:
    QString m_dbpath;
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
