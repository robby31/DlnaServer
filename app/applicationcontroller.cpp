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
