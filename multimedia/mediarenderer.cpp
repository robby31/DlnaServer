#include "mediarenderer.h"

MediaRenderer::MediaRenderer(QObject *parent) :
    ListItem(parent),
    m_roles(),
    m_device(0),
    status("standby")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[iconUrlRole] = "iconurl";
    m_roles[availableRole] = "available";
}

MediaRenderer::MediaRenderer(UpnpRootDevice *device, QObject *parent) :
    ListItem(parent),
    m_roles(),
    m_device(device),
    status("standby")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[iconUrlRole] = "iconurl";
    m_roles[availableRole] = "available";

    connect(m_device, SIGNAL(itemChanged(QVector<int>)), this, SLOT(deviceItemChanged(QVector<int>)));
}

QString MediaRenderer::id() const
{
    if (m_device)
        return m_device->uuid();
    else
        return ListItem::id();
}

QVariant MediaRenderer::data(int role) const
{
    switch (role) {
    case statusRole:
        return status;
    case nameRole:
        if (m_device)
            return m_device->friendlyName();
        else
            return QString();
    case networkAddressRole:
        if (m_device)
            return m_device->host().toString();
        else
            return QString();
    case iconUrlRole:
        if (m_device)
            return m_device->iconUrl();
        else
            return QString();
    case availableRole:
        if (m_device)
            return m_device->available();
        else
            return false;
    default:
        return QVariant::Invalid;
    }

    return QVariant::Invalid;
}

bool MediaRenderer::setData(const QVariant &value, const int &role)
{
    QVector<int> roles;
    roles << role;

    switch(role)
    {
    case statusRole:
        if (value != status)
        {
            status = value.toString();
            emit itemChanged(roles);
        }
        return true;

    default:
        qWarning() << "unable to set data" << value << role;
        return false;
    }
}

void MediaRenderer::deviceItemChanged(QVector<int> roles)
{
    emit itemChanged();

    if (roles.contains(UpnpRootDevice::AvailableRole) && m_device->available() == false)
        emit removeRenderer();
}
