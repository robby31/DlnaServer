#include "serveritem.h"

ServerItem::ServerItem(QObject *parent) :
    ListItem(parent),
    status("standby")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[iconUrlRole] = "iconurl";
    m_roles[availableRole] = "available";
}

ServerItem::ServerItem(UpnpRootDevice *device, QObject *parent) :
    ListItem(parent),
    m_device(device),
    status("standby")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[iconUrlRole] = "iconurl";
    m_roles[availableRole] = "available";

    connect(m_device, SIGNAL(itemChanged(QVector<int>)), this, SLOT(deviceItemChanged(QVector<int>)));
    connect(m_device, SIGNAL(destroyed(QObject*)), this, SLOT(deviceDestroyed(QObject*)));
}

QHash<int, QByteArray> ServerItem::roleNames() const
{
    return m_roles;
}

QString ServerItem::id() const
{
    if (m_device)
        return m_device->uuid();
    else
        return ListItem::id();
}

QVariant ServerItem::data(int role) const
{
    switch (role) {
    case statusRole:
    {
        return status;
    }
    case nameRole:
    {
        if (m_device)
            return m_device->friendlyName();
        else
            return QString();
    }
    case networkAddressRole:
    {
        if (m_device)
            return m_device->host().toString();
        else
            return QString();
    }
    case iconUrlRole:
    {
        if (m_device)
            return m_device->iconUrl();
        else
            return QString();
    }
    case availableRole:
    {
        if (m_device)
            return m_device->available();
        else
            return false;
    }
    default:
    {
        return QVariant::Invalid;
    }
    }

    return QVariant::Invalid;
}

bool ServerItem::setData(const QVariant &value, const int &role)
{
    QVector<int> roles;
    roles << role;

    switch(role)
    {
    case statusRole:
    {
        if (value != status)
        {
            status = value.toString();
            emit itemChanged(roles);
        }
        return true;
    }

    default:
    {
        qWarning() << "unable to set data" << value << role;
        return false;
    }
    }
}

void ServerItem::deviceItemChanged(QVector<int> roles)
{
    emit itemChanged();

    if (roles.contains(UpnpRootDevice::AvailableRole) && m_device->available() == false)
        deleteLater();
}

void ServerItem::deviceDestroyed(QObject *obj)
{
    if (obj == m_device)
    {
        m_device = Q_NULLPTR;
        deleteLater();
    }
}

