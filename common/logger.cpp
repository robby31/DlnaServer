#include "logger.h"

Logger::Logger(QObject *parent):
    QObject(parent)
{
    level = TRA;
}


void Logger::setLevel(LogLevel level) {
    this->level = level;
}

void Logger::TRACE(QString message) const {
    if (level <= TRA)
    {
        qDebug() << "TRACE -" << message;
    }
}

void Logger::DEBUG(QString message) const {
    if (level <= DEBG)
    {
        qDebug() << "DEBUG -" << message;
    }
}

void Logger::INFO(QString message) const {
    if (level <= INF)
    {
        qDebug() << "INFO  -" << message;
    }
}

void Logger::WARNING(QString message) const {
    if (level <= WAR)
    {
        qDebug() << "WAR   -" << message;
    }
}

void Logger::ERROR(QString message) const {
    if (level <= ERR)
    {
        qDebug() << "ERROR -" << message;
    }
}

