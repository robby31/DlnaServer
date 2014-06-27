#include "logger.h"

Logger::Logger(QObject *parent):
    QObject(parent),
    m_level(TRA)
{
    setLevel(TRA);
}

void Logger::Trace(QString message) const {
    if (isLevel(TRA))
        qDebug() << QString("TRACE - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Debug(QString message) const {
    if (isLevel(DEBG))
        qDebug() << QString("DEBUG - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Info(QString message) const {
    if (isLevel(INF))
        qDebug() << QString("INFO  - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Warning(QString message) const {
    if (isLevel(WAR))
        qDebug() << QString("WAR   - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Error(QString message) const {
    if (isLevel(ERR))
        qDebug() << QString("ERROR - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

