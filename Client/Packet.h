#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>

class Packet {
public:
    enum PacketType { AUTH, MESSAGE, IMAGE };

    Packet(PacketType type, const QByteArray &data);
    QByteArray serialize() const;
    static QByteArray createAuthPacket(const QString &username, const QString &password);
    static QByteArray createMessagePacket(const QString &message);
    static QByteArray createImagePacket(const QString &filePath);
    PacketType getType() const;

private:
    PacketType type;
    QByteArray data;
};

#endif // PACKET_H
