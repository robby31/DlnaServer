#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include <QNetworkAccessManager>
#include "Worker/worker.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QNetworkAccessManager *nam, QObject *parent = 0);

private:
    QNetworkAccessManager *m_nam;
};

#endif // APPLICATIONWORKER_H
