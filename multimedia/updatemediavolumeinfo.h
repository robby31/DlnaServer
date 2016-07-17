#ifndef UPDATEMEDIAVOLUMEINFO_H
#define UPDATEMEDIAVOLUMEINFO_H

#include <QRunnable>
#include "medialibrary.h"
#include "logger.h"
#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include "dlnayoutubevideo.h"

class UpdateMediaVolumeInfo : public QRunnable
{

public:
    UpdateMediaVolumeInfo(Logger *log, QNetworkAccessManager *nam);
    ~UpdateMediaVolumeInfo();

    void run();

private:
    Logger *m_log;
    QNetworkAccessManager *m_nam;
};

#endif // UPDATEMEDIAVOLUMEINFO_H