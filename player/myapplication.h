#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "application.h"
#include "applicationcontroller.h"
#include "applicationworker.h"
#include "upnpcontrolpoint.h"
#include "Devices/mediarenderermodel.h"

class MyApplication : public Application
{
    Q_OBJECT

    Q_PROPERTY(MediaRendererModel *serversModel READ serversModel WRITE setServersModel NOTIFY serversModelChanged)

public:
    explicit MyApplication(int &argc, char **argv);

    MediaRendererModel *serversModel() const;
    void setServersModel(MediaRendererModel *model);

signals:
    void serversModelChanged();

private slots:
    void newRootDevice(UpnpRootDevice *device);

private:
    ApplicationController m_controller;
    ApplicationWorker *m_worker = Q_NULLPTR;

    QNetworkAccessManager netManager;
    UpnpControlPoint m_upnp;

    // list of servers
    MediaRendererModel *m_serversModel = Q_NULLPTR;
};

#endif // MYAPPLICATION_H
