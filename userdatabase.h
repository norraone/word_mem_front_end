#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QCryptographicHash>

class UserDatabase : public QObject
{
    Q_OBJECT

public:
    explicit UserDatabase(QObject *parent = nullptr);
    ~UserDatabase();

    bool initialize();
    bool addUser(const QString &username, const QString &password);
    bool validateUser(const QString &username, const QString &password);
    bool userExists(const QString &username);
    QString lastError() const;

private:
    QSqlDatabase db;
    QString hashPassword(const QString &password);
    bool createTable();
    QString m_lastError;
};

#endif // USERDATABASE_H
