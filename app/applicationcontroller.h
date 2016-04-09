#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "UIController/controller.h"

class ApplicationController : public Controller
{
    Q_OBJECT
public:
    explicit ApplicationController(QObject *parent = 0);

signals:

public slots:
};

#endif // APPLICATIONCONTROLLER_H
