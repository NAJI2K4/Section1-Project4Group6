/* NetworkHandler.cpp */
#include "NetworkHandler.h"
#include <QDebug>

NetworkHandler::NetworkHandler() {
    connect(&socket, &QTcpSocket::readyRead, this, &NetworkHandler::receiveData);
}

void NetworkHandler::connectToServer(const QString &address, int port) {
    qDebug() << "Connecting to server at" << address << "on port" << port;
    socket.connectToHost(address, port);
}

void NetworkHandler::sendPacket(const QByteArray &packet) {
    qDebug() << "Sending packet...";
    socket.write(packet);
}

void NetworkHandler::receiveData() {
    QByteArray data = socket.readAll();
    qDebug() << "Received data from server:" << data;
}
