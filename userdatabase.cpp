#include "userdatabase.h"
#include <QDebug>
#include <QDir>

UserDatabase::UserDatabase(QObject *parent)
    : QObject(parent)
{
}

UserDatabase::~UserDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool UserDatabase::initialize()
{
    // Make sure the SQLite driver is loaded
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        m_lastError = "SQLite driver not available";
        qDebug() << "Available drivers:" << QSqlDatabase::drivers();
        return false;
    }

    // Set up the database
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/users.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        m_lastError = "Error opening database: " + db.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    return createTable();
}

bool UserDatabase::createTable()
{
    QSqlQuery query;
    QString createTableQuery = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(createTableQuery)) {
        m_lastError = "Error creating table: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    return true;
}

QString UserDatabase::hashPassword(const QString &password)
{
    // Use SHA-256 for password hashing
    QByteArray hashedPassword = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    return QString(hashedPassword.toHex());
}

bool UserDatabase::addUser(const QString &username, const QString &password)
{
    if (userExists(username)) {
        m_lastError = "Username already exists";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        m_lastError = "Error adding user: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    return true;
}

bool UserDatabase::validateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        m_lastError = "Error validating user: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    if (query.next()) {
        QString storedHash = query.value(0).toString();
        QString inputHash = hashPassword(password);
        return storedHash == inputHash;
    }

    m_lastError = "Invalid username or password";
    return false;
}

bool UserDatabase::userExists(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        m_lastError = "Error checking username: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

QString UserDatabase::lastError() const
{
    return m_lastError;
}
