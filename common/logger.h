#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

enum LogLevel {TRA, DEBG, INF, WAR, ERR};

class Logger: public QObject
{
    Q_OBJECT

public:
    Logger(QObject *parent = 0);

    void setLevel(LogLevel level);

    void TRACE(QString message) const;
    void DEBUG(QString message) const;
    void INFO(QString message) const;
    void ERROR(QString message) const;
    void WARNING(QString message) const;

private:
    // LOG level:
    //    0: TRACE
    //    1: DEBUG
    //    2: INFO
    //    3: WARNING
    //    4: ERROR
    LogLevel level;
};

#endif // LOGGER_H
