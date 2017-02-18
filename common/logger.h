#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QDateTime>

enum LogLevel {TRA, DEBG, INF, WAR, ERR};

class Logger: public QObject
{
    Q_OBJECT

public:
    explicit Logger(QObject *parent = 0);

    void setLevel(const LogLevel &level) { m_level = level; }
    bool isLevel(const LogLevel &level) const { return m_level <= level; }

    void Trace(const QString &message) const;
    void Debug(const QString &message) const;
    void Info(const QString &message) const;
    void Error(const QString &message) const;
    void Warning(const QString &message) const;

private:
    QString date() const { return QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"); }

    // LOG level:
    //    0: TRACE
    //    1: DEBUG
    //    2: INFO
    //    3: WARNING
    //    4: ERROR
    LogLevel m_level;
};

#endif // LOGGER_H
