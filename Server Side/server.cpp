// server.cpp
#include "server.h"
#include <QHostAddress>
#include <QDebug>
#include <QTimer>

Server::Server(QObject* parent) : QTcpServer(parent) {}

Server::~Server() {
    stopServer();
}

void Server::startServer(quint16 port) {
    if (listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
        emit logMessage("Server started on port " + QString::number(port));
        connect(this, &QTcpServer::newConnection, this, &Server::newClientConnection);
    }
    else {
        qDebug() << "Failed to start server!";
        emit logMessage("Failed to start server!");
    }
}

void Server::stopServer() {
    for (QTcpSocket* client : clients) {
        if (client && client->isOpen()) {
            client->disconnectFromHost();
        }
    }
    clients.clear();
    clientUsernames.clear();
    close();
    emit logMessage("Server stopped");
}

void Server::newClientConnection() {
    QTcpSocket* clientSocket = nextPendingConnection();
    if (!clientSocket) return;

    clients.append(clientSocket);
    qDebug() << "New client connected:" << clientSocket->peerAddress().toString();

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::receiveMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
}

void Server::receiveMessage() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket || !clientSocket->isOpen()) return;

    QByteArray data = clientSocket->readAll();
    if (!data.isEmpty()) {
        processPacket(clientSocket, data);
    }
}

void Server::clientDisconnected() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QString username = clientUsernames.value(clientSocket, "Unknown");
    clients.removeAll(clientSocket);
    clientUsernames.remove(clientSocket);
    clientSocket->deleteLater();

    emit logMessage(username + " disconnected.");
    emit clientListUpdated(clientUsernames.values());
}

void Server::processPacket(QTcpSocket* client, const QByteArray& data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    int packetType;
    stream >> packetType;

    switch (packetType) {
    case Packet::AUTH: {
        QString username, password;
        stream >> username >> password;

        qDebug() << "Auth attempt from:" << username;
        bool isValid = auth.validateUser(username, password);
        bool added = false;
        if (!isValid) {
            added = auth.addUser(username, password);
            isValid = added;
        }

        qDebug() << "Validation result:" << isValid;
        qDebug() << "Registration result:" << added;

        if (isValid && client && client->isOpen()) {
            clientUsernames[client] = username;
            client->write("Authentication Successful\n");
            emit logMessage(username + " authenticated.");
            emit clientListUpdated(clientUsernames.values());

            // Send connected users list
            QStringList otherUsers;
            for (QTcpSocket* sock : clientUsernames.keys()) {
                if (sock != client) {
                    otherUsers << clientUsernames[sock];
                }
            }
            QString userListMsg = "[CLIENT_LIST] " + otherUsers.join(",");
            QByteArray listPacket = Packet::createMessagePacket(userListMsg);
            client->write(listPacket);
        }
        else {
            client->write("Authentication Failed\n");
            QTimer::singleShot(100, client, &QTcpSocket::disconnectFromHost);
            emit logMessage("Authentication failed for: " + username);
        }
        break;
    }

    case Packet::MESSAGE: {
        QString rawMsg;
        stream >> rawMsg;
        qDebug() << "Raw message received:" << rawMsg;

        QString fromUser = clientUsernames.value(client);
        QString target;
        QString payload;

        if (rawMsg.startsWith("[SENDTO:")) {
            target = rawMsg.section("]", 0, 0).mid(8);
            payload = rawMsg.section("]", 1).trimmed();
        }
        else if (rawMsg.startsWith("[ANNOTTO:")) {
            target = rawMsg.section("]", 0, 0).mid(9);
            payload = "[Annotation] " + rawMsg.section("]", 1).trimmed();
        }
        else {
            // broadcast to all clients (fallback)
            emit logMessage("Broadcast message: " + rawMsg);
            for (QTcpSocket* other : clients) {
                if (other != client) {
                    QByteArray msg = Packet::createMessagePacket(fromUser + ": " + rawMsg);
                    other->write(msg);
                }
            }
            break;
        }

        // Send private message
        QTcpSocket* targetSock = clientUsernames.key(target);
        if (targetSock && targetSock->isOpen()) {
            QByteArray msg = Packet::createMessagePacket(fromUser + " ➜ you: " + payload);
            targetSock->write(msg);
            emit logMessage(fromUser + " ➜ " + target + ": " + payload);
        }
        else {
            client->write("User not available\n");
        }
        break;
    }

    case Packet::IMAGE: {
        QByteArray imgData = data.mid(sizeof(int)); // remove packetType prefix
        QString fromUser = clientUsernames.value(client);

        // Fallback: look for [IMGTO:username] prefix in image
        QString imageString = QString::fromUtf8(imgData.left(100)); // only peek
        QString target;
        if (imageString.startsWith("[IMGTO:")) {
            target = imageString.section("]", 0, 0).mid(7);
        }

        QTcpSocket* targetSock = clientUsernames.key(target);
        if (!target.isEmpty() && targetSock && targetSock->isOpen()) {
            targetSock->write(data); // send raw image
            emit logMessage(fromUser + " sent an image to " + target);
        }
        else {
            emit logMessage("Image received (no valid target): " + QString::number(imgData.size()));
            client->write("Image ignored: no recipient\n");
        }

        break;
    }

    default:
        emit logMessage("Unknown packet type received.");
        if (client && client->isOpen()) {
            client->write("Invalid Packet\n");
        }
        break;
    }
}