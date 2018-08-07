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

    connect(m_device, &UpnpRootDevice::itemChanged, this, &ServerItem::deviceItemChanged);
    connect(m_device, &UpnpRootDevice::destroyed, this, &ServerItem::deviceDestroyed);

    initializeContentDirectory();
}

QHash<int, QByteArray> ServerItem::roleNames() const
{
    return m_roles;
}

QString ServerItem::id() const
{
    if (m_device)
        return m_device->uuid();

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

        return QString();
    }
    case networkAddressRole:
    {
        if (m_device)
            return m_device->host().toString();

        return QString();
    }
    case iconUrlRole:
    {
        if (m_device)
            return m_device->iconUrl();

        return QString();
    }
    case availableRole:
    {
        if (m_device)
            return m_device->available();

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

void ServerItem::deviceItemChanged(const QVector<int>& roles)
{
    emit itemChanged();

    if (roles.contains(UpnpRootDevice::AvailableRole) && !m_device->available())
        emit removeItem();
}

void ServerItem::deviceDestroyed(QObject *obj)
{
    if (obj == m_device)
    {
        m_device = Q_NULLPTR;
        emit removeItem();
        emit nameChanged();
        emit iconurlChanged();
    }
}

UpnpRootDevice *ServerItem::device() const
{
    return m_device;
}

void ServerItem::initializeContentDirectory()
{
    m_contentDirectory = qobject_cast<UpnpService*>(m_device->getService("urn:upnp-org:serviceId:ContentDirectory"));
    if (m_contentDirectory)
    {
        setContentModel(new ContentModel(m_contentDirectory, "0", this));
    }
}

ContentModel *ServerItem::contentModel() const
{
    return m_contentModel;
}

void ServerItem::setContentModel(ContentModel *model)
{
    m_contentModel = model;
    emit contentModelChanged();
}

QString ServerItem::name() const
{
    return data(nameRole).toString();
}

QUrl ServerItem::iconurl() const
{
    return data(iconUrlRole).toUrl();
}
