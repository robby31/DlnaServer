#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "UIController/controller.h"

class ApplicationController : public Controller
{
    Q_OBJECT

public:
    explicit ApplicationController(QObject *parent = Q_NULLPTR);
};

#endif // APPLICATIONCONTROLLER_H
