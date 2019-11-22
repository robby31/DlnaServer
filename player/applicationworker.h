#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include "Worker/worker.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QObject *parent = Q_NULLPTR);

};

#endif // APPLICATIONWORKER_H
