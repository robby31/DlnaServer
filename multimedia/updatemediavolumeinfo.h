#ifndef UPDATEMEDIAVOLUMEINFO_H
#define UPDATEMEDIAVOLUMEINFO_H

#include <QRunnable>
#include "medialibrary.h"
#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include "dlnayoutubevideo.h"

class UpdateMediaVolumeInfo : public QRunnable
{

public:
    UpdateMediaVolumeInfo(QNetworkAccessManager *nam);
    ~UpdateMediaVolumeInfo();

    void run();

private:
    QNetworkAccessManager *m_nam;
};

#endif // UPDATEMEDIAVOLUMEINFO_H
