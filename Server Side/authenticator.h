// authenticator.h

#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QString>
#include <QSqlDatabase>

class Authenticator {
public:
    Authenticator();

    bool validateUser(const QString& username, const QString& password);
    bool addUser(const QString& username, const QString& password);

private:
    void initializeDatabase();
    QSqlDatabase db;
};

#endif // AUTHENTICATOR_H