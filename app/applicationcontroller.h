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

signals:
    void checkNetworkLinkSignal();
    void scanVolumeInfoSignal();

public slots:
};

#endif // APPLICATIONCONTROLLER_H
