#include "logger.h"

Logger::Logger(QObject *parent):
    QObject(parent),
    m_level(TRA)
{
    setLevel(TRA);
}

void Logger::Trace(const QString &message) const {
    if (isLevel(TRA))
        qDebug() << QString("TRACE - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Debug(const QString &message) const {
    if (isLevel(DEBG))
        qDebug() << QString("DEBUG - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Info(const QString &message) const {
    if (isLevel(INF))
        qDebug() << QString("INFO  - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Warning(const QString &message) const {
    if (isLevel(WAR))
        qWarning() << QString("WAR   - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

void Logger::Error(const QString &message) const {
    if (isLevel(ERR))
        qCritical() << QString("ERROR - %1 - %2").arg(date()).arg(message).toUtf8().constData();
}

