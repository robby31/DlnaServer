#ifndef SERVICECONNECTIONMANAGER_H
#define SERVICECONNECTIONMANAGER_H

#include <QObject>
#include "Http/httprequest.h"
#include "soapaction.h"
#include "soapactionresponse.h"

class ServiceConnectionManager : public QObject
{
    Q_OBJECT

public:
    explicit ServiceConnectionManager(QObject *parent = 0);

    void reply(HttpRequest *request);

signals:

public slots:
};

#endif // SERVICECONNECTIONMANAGER_H
