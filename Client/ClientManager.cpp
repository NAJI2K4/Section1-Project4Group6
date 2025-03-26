/* ClientManager.cpp */
#include "ClientManager.h"
#include <QDebug>

ClientManager::ClientManager() {}

void ClientManager::login(const QString &username, const QString &password) {
    qDebug() << "Logging in with username:" << username;
    networkHandler.sendPacket(Packet::createAuthPacket(username, password));
}

void ClientManager::sendMessage(const QString &message) {
    qDebug() << "Sending message:" << message;
    networkHandler.sendPacket(Packet::createMessagePacket(message));
}

void ClientManager::uploadImage(const QString &filePath) {
    qDebug() << "Uploading image from:" << filePath;
    networkHandler.sendPacket(Packet::createImagePacket(filePath));
}
