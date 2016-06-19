#include "debugitem.h"

DebugItem::DebugItem(QObject *parent):
    ListItem(parent),
    m_roles(),
    m_name()
{
    m_roles[NameRole] = "name";
    m_roles[CounterRole] = "counter";
}

DebugItem::DebugItem(const QString &name, QObject *parent):
    ListItem(parent),
    m_roles(),
    m_name(name)
{
    m_roles[NameRole] = "name";
    m_roles[CounterRole] = "counter";
}

QVariant DebugItem::counterValue() const
{
    if (m_name == "DlnaResource")
        return QVariant(DlnaResource::objectCounter);
    else if (m_name == "HttpClient")
        return QVariant(HttpClient::objectCounter);
    else if (m_name == "Request")
        return QVariant(Request::objectCounter);
    else if (m_name == "Reply")
        return QVariant(Reply::objectCounter);
    else if (m_name == "Device")
        return QVariant(Device::objectCounter);
    else if (m_name == "DlnaFolder")
        return QVariant(DlnaFolder::objectCounter);
    else if (m_name == "DlnaMusicTrackFile")
        return QVariant(DlnaMusicTrackFile::objectCounter);
    else if (m_name == "DlnaRootFolder")
        return QVariant(DlnaRootFolder::objectCounter);
    else if (m_name == "DlnaVideoFile")
        return QVariant(DlnaVideoFile::objectCounter);
    else if (m_name == "DlnaYoutubeVideo")
        return QVariant(DlnaYouTubeVideo::objectCounter);

    return QVariant("");
}

QVariant DebugItem::data(int role) const
{
    switch (role) {
    case NameRole:
        return m_name;
    case CounterRole:
        return counterValue();
    default:
        return QVariant::Invalid;
    }

    return QVariant::Invalid;
}

bool DebugItem::setData(const QVariant &value, const int &role)
{
    qWarning() << "unable to set data" << value << role;
    return false;
}
