#include "checknetworklinkitem.h"

CheckNetworkLinkItem::CheckNetworkLinkItem(QObject *parent):
    ListItem(parent),
    m_name(),
    m_message()
{
    m_roles[NameRole] = "name";
    m_roles[MessageRole] = "message";
}

CheckNetworkLinkItem::CheckNetworkLinkItem(const QString &name, const QString &message, QObject *parent):
    ListItem(parent),
    m_name(name),
    m_message(message)
{
    m_roles[NameRole] = "name";
    m_roles[MessageRole] = "message";
}

QVariant CheckNetworkLinkItem::data(int role) const
{
    switch (role) {
    case NameRole:
        return m_name;
    case MessageRole:
        return m_message;
    default:
        return QVariant::Invalid;
    }
}

bool CheckNetworkLinkItem::setData(const QVariant &value, const int &role)
{
    qWarning() << "unable to set data" << value << role;
    return false;
}
