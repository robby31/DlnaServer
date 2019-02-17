#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    m_upnp(5080)
{
    addImageProvider("upnpclass", new UpnpClassImageProvider);

    setServersModel(new ServerModel(this));

    addController("homePageController", &m_controller);

    m_worker = new ApplicationWorker();
    addWorker(&m_controller, m_worker);

    connect(&m_upnp, &UpnpControlPoint::newRootDevice, m_serversModel, &ServerModel::addServer);

    m_renderer = new MediaRenderer(m_upnp.macAddress(), m_upnp.host().toString(), 7000, this);
    m_upnp.addLocalRootDevice(m_renderer);
    m_renderer->startServer();

    m_upnp.startDiscover();
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
