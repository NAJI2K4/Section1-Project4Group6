/* Logger.cpp */
#include "Logger.h"
#include <QDateTime>

void Logger::log(const QString &message) {
    QFile file("client_log.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
        file.close();
    }
}
