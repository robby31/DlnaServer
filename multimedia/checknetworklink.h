#ifndef CHECKNETWORKLINK_H
#define CHECKNETWORKLINK_H

#include <QRunnable>
#include "logger.h"
#include "medialibrary.h"
#include "dlnayoutubevideo.h"

class CheckNetworkLink : public QRunnable
{
public:
    CheckNetworkLink(Logger *log);
    ~CheckNetworkLink();

    void run();

private:
    Logger *m_log;
};

#endif // CHECKNETWORKLINK_H
