/* Logger.h */
#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>

class Logger {
public:
    static void log(const QString &message);
};

#endif // LOGGER_H
