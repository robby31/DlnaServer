#include "logobject.h"

LogObject::LogObject(Logger *log, QObject *parent):
    QObject(parent),
    m_log(log)
{
    connect(m_log, SIGNAL(destroyed()), this, SLOT(_logDestroyed()));

    if (!m_log)
        qWarning() << "log is not available for" << this;
}
