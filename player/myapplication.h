#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "application.h"
#include "applicationcontroller.h"
#include "applicationworker.h"
#include "upnpcontrolpoint.h"
#include "servermodel.h"
#include "upnpclassimageprovider.h"
#include "mediarenderer.h"

class MyApplication : public Application
{
    Q_OBJECT

    Q_PROPERTY(ServerModel *serversModel READ serversModel WRITE setServersModel NOTIFY serversModelChanged)

public:
    explicit MyApplication(int &argc, char **argv);

    ServerModel *serversModel() const;
    void setServersModel(ServerModel *model);

signals:
    void serversModelChanged();

private slots:

private:
    ApplicationController m_controller;
    ApplicationWorker *m_worker = Q_NULLPTR;

    UpnpControlPoint m_upnp;

    // Upnp MediaServer listmodel
    ServerModel *m_serversModel = Q_NULLPTR;

    // local MediaRenderer
    MediaRenderer *m_renderer = Q_NULLPTR;
};

#endif // MYAPPLICATION_H
