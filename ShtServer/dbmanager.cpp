#include "dbmanager.h"
#include <QDir>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

DbManager::DbManager(QObject *parent) : QObject(parent)
{
    m_dbpath = QStringLiteral("/home/loris/sht.sqlite");
    QFile dbFile(m_dbpath);
    if(!dbFile.exists()){
        QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(m_dbpath);
        if(!m_db.open()){
            qWarning() << "Failed opening database";
        } else {
            QSqlQuery query(m_db);
            query.exec("CREATE TABLE user "
                       "(id INTEGER PRIMARY KEY, "
                       "name VARCHAR(20), "
                       "uuid VARCHAR(40), "
                       "email VARCHAR(255),"
                       "matches INTEGER,"
                       "kilos INTEGER,"
                       "state INTEGER,"
                       "avatar VARCHAR(20))");
            qWarning() << query.lastError().text();
            m_db.close();
            QSqlDatabase::removeDatabase(m_dbpath);
            qWarning() << "Database created";
        }
    }

    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbpath);
    if(!m_db.open()){
        qWarning() << "Failed opening database";
    }
    qWarning() << "Database ready";
}

User *DbManager::findUserByUuid(QString uuid)
{
    if(uuid.isEmpty()) return nullptr;

    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec())
    {
       if (query.next())
       {
          User* u = new User();
          u->setId(query.value(0).toInt());
          u->setName(query.value(1).toString());
          u->setUuid(query.value(2).toString());
          u->setEmail(query.value(3).toString());
          u->setMatches(query.value(4).toInt());
          u->setKilos(query.value(5).toInt());
          u->setState(query.value(6).toInt());
          u->setAvatar(query.value(7).toString());
          return u;
       }
    }
    return nullptr;
}

User *DbManager::findUserById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec())
    {
       if (query.next())
       {
          User* u = new User();
          u->setId(query.value(0).toInt());
          u->setName(query.value(1).toString());
          u->setUuid(query.value(2).toString());
          u->setEmail(query.value(3).toString());
          u->setMatches(query.value(4).toInt());
          u->setKilos(query.value(5).toInt());
          u->setState(query.value(6).toInt());
          u->setAvatar(query.value(7).toString());
          return u;
       }
    }
    return nullptr;
}

void DbManager::saveUser(User *u)
{
    QSqlQuery query;
    query.prepare("UPDATE user SET name= :name, uuid= :uuid, email= :email, matches= :matches, kilos= :kilos, state= :state, avatar= :avatar WHERE id= :id");
    query.bindValue(":name", u->name());
    query.bindValue(":uuid", u->uuid());
    query.bindValue(":email", u->email());
    query.bindValue(":matches", u->matches());
    query.bindValue(":kilos", u->kilos());
    query.bindValue(":state", u->state());
    query.bindValue(":avatar", u->avatar());

    query.bindValue(":id", u->id());

    if (!query.exec())
    {
       qWarning() << "Error doing UPDATE: " << query.lastError().text();
    }
}

void DbManager::addUser(User *u)
{
    QSqlQuery query;
    query.prepare("INSERT INTO user(name, uuid, email, matches, kilos, state, avatar) VALUES(:name, :uuid, :email, :matches, :kilos, :state, :avatar)");
    query.bindValue(":name", u->name());
    query.bindValue(":uuid", u->uuid());
    query.bindValue(":email", u->email());
    query.bindValue(":matches", u->matches());
    query.bindValue(":kilos", u->kilos());
    query.bindValue(":state", u->state());
    query.bindValue(":avatar", u->avatar());

    if (!query.exec())
    {
       qWarning() << "Error doing INSERT: " << query.lastError().text();
    }
    u->setId(query.lastInsertId().toInt());
}
