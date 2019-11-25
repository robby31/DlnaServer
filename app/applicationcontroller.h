#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "UIController/controller.h"

class ApplicationController : public Controller
{
    Q_OBJECT

public:
    explicit ApplicationController(QObject *parent = Q_NULLPTR);

    void checkNetworkLink();
    void scanVolumeInfo();

    void export_playlist(const QUrl &url);
    void export_media(const QUrl &url);

signals:
    void checkNetworkLinkSignal();
    void scanVolumeInfoSignal();
    void exportPlaylistSignal(const QUrl &url);
    void exportMediaSignal(const QUrl &url);
};

#endif // APPLICATIONCONTROLLER_H
