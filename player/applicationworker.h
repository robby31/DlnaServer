#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include "Worker/worker.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QObject *parent = 0);

};

#endif // APPLICATIONWORKER_H
