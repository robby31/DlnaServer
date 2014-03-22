#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

enum LogLevel {TRA, DEBG, INF, WAR, ERR};

class Logger: public QObject
{
    Q_OBJECT

public:
    Logger(QObject *parent = 0);

    void setLevel(LogLevel level) { m_level = level; }
    bool isLevel(LogLevel level) const { return m_level <= level; }

    void Trace(QString message) const;
    void Debug(QString message) const;
    void Info(QString message) const;
    void Error(QString message) const;
    void Warning(QString message) const;

private:
    // LOG level:
    //    0: TRACE
    //    1: DEBUG
    //    2: INFO
    //    3: WARNING
    //    4: ERROR
    LogLevel m_level;
};

#endif // LOGGER_H
