/* ClientManager.h */
#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "NetworkHandler.h"
#include "Logger.h"

class ClientManager {
public:
    ClientManager();
    void login(const QString &username, const QString &password);
    void sendMessage(const QString &message);
    void uploadImage(const QString &filePath);

private:
    NetworkHandler networkHandler;
    Logger logger;
};

#endif // CLIENTMANAGER_H
