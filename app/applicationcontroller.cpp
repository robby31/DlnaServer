#include "applicationcontroller.h"

ApplicationController::ApplicationController(QObject *parent) :
    Controller(parent)
{
}

void ApplicationController::checkNetworkLink()
{
    if (setActivity("Check Network Links"))
        emit checkNetworkLinkSignal();
}

void ApplicationController::scanVolumeInfo()
{
    if (setActivity("Scan Volume"))
        emit scanVolumeInfoSignal();
}

void ApplicationController::export_playlist(const QUrl &url)
{
    if (setActivity("Export Playlist"))
        emit exportPlaylistSignal(url);
}

void ApplicationController::export_media(const QUrl &url)
{
    if (setActivity("Export Media"))
        emit exportMediaSignal(url);
}
