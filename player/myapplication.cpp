#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    m_upnp(5080)
{
    setServersModel(new MediaRendererModel(this));

    addController("homePageController", &m_controller);

    m_worker = new ApplicationWorker(&netManager);
    addWorker(&m_controller, m_worker);

    m_upnp.setNetworkManager(&netManager);
    connect(&m_upnp, SIGNAL(newRootDevice(UpnpRootDevice*)), this, SLOT(newRootDevice(UpnpRootDevice*)));

    m_upnp.startDiscover(UpnpRootDevice::UPNP_ROOTDEVICE);
}

void MyApplication::newRootDevice(UpnpRootDevice *device)
{
    if (device->deviceType().startsWith("urn:schemas-upnp-org:device:MediaServer"))
        m_serversModel->addMediaRenderer(device);
}

MediaRendererModel *MyApplication::serversModel() const
{
    return m_serversModel;
}

void MyApplication::setServersModel(MediaRendererModel *model)
{
    if (m_serversModel)
        m_serversModel->deleteLater();

    m_serversModel = model;

    emit serversModelChanged();
}
