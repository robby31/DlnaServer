#include "mediarenderer.h"

MediaRenderer::MediaRenderer(QObject *parent) :
    ListItem(parent),
    m_roles(),
    ip(),
    port(),
    userAgent(),
    status("standby"),
    name("DefaultRenderer")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[userAgentRole] = "userAgent";
}

MediaRenderer::MediaRenderer(const QString &ip, const int &port, const QString &userAgent, QObject *parent) :
    ListItem(parent),
    m_roles(),
    ip(ip),
    port(port),
    userAgent(userAgent),
    status("standby"),
    name("DefaultRenderer")
{
    m_roles[statusRole] = "status";
    m_roles[nameRole] = "name";
    m_roles[networkAddressRole] = "networkAddress";
    m_roles[userAgentRole] = "userAgent";
}

QVariant MediaRenderer::data(int role) const
{
    switch (role) {
    case statusRole:
        return status;
    case nameRole:
        return name;
    case networkAddressRole:
        return ip;
    case userAgentRole:
        return userAgent;
    default:
        return QVariant::Invalid;
    }

    return QVariant::Invalid;
}

bool MediaRenderer::setData(const QVariant &value, const int &role)
{
    QVector<int> roles;
    roles << role;

    if (role == statusRole)
    {
        if (value != status)
        {
            status = value.toString();
            emit itemChanged(roles);
        }
        return true;
    }
    else if (role == nameRole)
    {
        if (value != name)
        {
            name = value.toString();
            emit itemChanged(roles);
        }
        return true;
    }
    else if (role == networkAddressRole)
    {
        if (value != ip)
        {
            ip = value.toString();
            emit itemChanged(roles);
        }
        return true;
    }
    else if (role == userAgentRole)
    {
        if (value != userAgent)
        {
            userAgent = value.toString();
            emit itemChanged(roles);
        }
        return true;
    }

    return false;
}
