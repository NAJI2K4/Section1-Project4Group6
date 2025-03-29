#include "Packet.h"
#include <QDataStream>
#include <QFile>
#include <QIODevice>

Packet::Packet(PacketType type, const QByteArray& data) : type(type), data(data) {}

QByteArray Packet::serialize() const {
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream << static_cast<int>(type) << data;
    return packet;
}

QByteArray Packet::createAuthPacket(const QString& username, const QString& password) {
    Packet packet(AUTH, QByteArray());
    QDataStream stream(&packet.data, QIODevice::WriteOnly);
    stream << username << password;
    return packet.serialize();
}

QByteArray Packet::createMessagePacket(const QString& message) {
    Packet packet(MESSAGE, message.toUtf8());
    return packet.serialize();
}

QByteArray Packet::createImagePacket(const QString& filePath) {
    QByteArray imageData;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        imageData = file.readAll();
        file.close();
    }
    Packet packet(IMAGE, imageData);
    return packet.serialize();
}

Packet::PacketType Packet::getType() const {
    return type;
}
