#include "logger.h"

Logger::Logger()
{
    level = 0;
}


void Logger::setLevel(int level) {
    if ((level>=0) && (level<=4)) {
        this->level = level;
    }
}

void Logger::TRACE(QString message) const {
    if (level <= 0)
    {
        qDebug() << "TRACE -" << message;
    }
}

void Logger::DEBUG(QString message) const {
    if (level <= 1)
    {
        qDebug() << "DEBUG -" << message;
    }
}

void Logger::INFO(QString message) const {
    if (level <= 2)
    {
        qDebug() << "INFO  -" << message;
    }
}

void Logger::WARNING(QString message) const {
    if (level <= 3)
    {
        qDebug() << "WAR   -" << message;
    }
}

void Logger::ERROR(QString message) const {
    if (level <= 4)
    {
        qDebug() << "ERROR -" << message;
    }
}

