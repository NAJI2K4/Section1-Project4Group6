#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QHash>
#include "authenticator.h"
#include "Packet.h"

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    ~Server();

    void startServer(quint16 port);
    void stopServer();

    void processPacket(QTcpSocket* client, const QByteArray& data);

signals:
    void logMessage(const QString& message);
    void clientListUpdated(const QStringList& usernames);  // Emits connected usernames

private slots:
    void newClientConnection();
    void receiveMessage();
    void clientDisconnected();

private:
    QList<QTcpSocket*> clients;
    QHash<QTcpSocket*, QString> clientUsernames;  // Maps socket to username
    Authenticator auth;
};

#endif // SERVER_H