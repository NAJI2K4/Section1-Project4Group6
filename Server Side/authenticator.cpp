#include "authenticator.h"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Authenticator::Authenticator() {
    initializeDatabase();
}

void Authenticator::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT)")) {
        qDebug() << "Failed to create table:" << query.lastError().text();
    }
    else {
        qDebug() << "Users table ready";
    }
}

bool Authenticator::validateUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        return false; // User does not exist
    }

    QString storedPassword = query.value(0).toString();
    return storedPassword == QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool Authenticator::addUser(const QString& username, const QString& password) {
    if (validateUser(username, password)) {
        return false; // User already exists
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()));

    if (!query.exec()) {
        qDebug() << "Failed to add user:" << query.lastError().text();
        return false;
    }

    return true;
}