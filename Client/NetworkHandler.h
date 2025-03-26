/* NetworkHandler.h */
#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include "Packet.h"

class NetworkHandler : public QObject {
    Q_OBJECT

public:
    NetworkHandler();
    void connectToServer(const QString &address, int port);
    void sendPacket(const QByteArray &packet);
    void receiveData();

private:
    QTcpSocket socket;
};

#endif // NETWORKHANDLER_H
