#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    m_upnp(5080)
{
    setServersModel(new ServerModel(this));

    addController("homePageController", &m_controller);

    m_worker = new ApplicationWorker(&netManager);
    addWorker(&m_controller, m_worker);

    m_upnp.setNetworkManager(&netManager);
    connect(&m_upnp, &UpnpControlPoint::newRootDevice, m_serversModel, &ServerModel::addServer);

    m_upnp.startDiscover(UpnpRootDevice::UPNP_ROOTDEVICE);
}

ServerModel *MyApplication::serversModel() const
{
    return m_serversModel;
}

void MyApplication::setServersModel(ServerModel *model)
{
    if (m_serversModel)
        m_serversModel->deleteLater();

    m_serversModel = model;

    emit serversModelChanged();
}
