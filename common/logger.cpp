#include "logger.h"

Logger::Logger(QObject *parent):
    QObject(parent)
{
    setLevel(TRA);
}

void Logger::Trace(QString message) const {
    if (isLevel(TRA))
        qDebug() << "TRACE -" << message;
}

void Logger::Debug(QString message) const {
    if (isLevel(DEBG))
        qDebug() << "DEBUG -" << message;
}

void Logger::Info(QString message) const {
    if (isLevel(INF))
        qDebug() << "INFO  -" << message;
}

void Logger::Warning(QString message) const {
    if (isLevel(WAR))
        qDebug() << "WAR   -" << message;
}

void Logger::Error(QString message) const {
    if (isLevel(ERR))
        qDebug() << "ERROR -" << message;
}

