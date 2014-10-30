#ifndef LOGOBJECT_H
#define LOGOBJECT_H

#include <QDebug>
#include "logger.h"

class LogObject : public QObject
{
    Q_OBJECT

public:
    explicit LogObject(Logger *log, QObject *parent = 0);

    Logger *log() const { return m_log; }

protected:    
    void logError(const QString &message)   const { if (m_log) m_log->Error(message);   }
    void logDebug(const QString &message)   const { if (m_log) m_log->Debug(message);   }
    void logInfo(const QString &message)    const { if (m_log) m_log->Info(message);    }
    void logWarning(const QString &message) const { if (m_log) m_log->Warning(message); }
    void logTrace(const QString &message)   const { if (m_log) m_log->Trace(message);   }

    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }

private slots:
    void _logDestroyed() { m_log = 0; }

private:
    Logger *m_log;
};

#endif // LOGOBJECT_H
