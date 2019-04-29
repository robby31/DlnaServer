#include "debugitem.h"

DebugItem::DebugItem(QObject *parent):
    ListItem(parent),
    m_name()
{
    m_roles[NameRole] = "name";
    m_roles[CounterRole] = "counter";
}

DebugItem::DebugItem(const QString &name, QObject *parent):
    ListItem(parent),
    m_name(name)
{
    m_roles[NameRole] = "name";
    m_roles[CounterRole] = "counter";
}

QVariant DebugItem::counterValue() const
{
    if (m_name.trimmed() == "DlnaResource")
        return QVariant(DlnaResource::objectCounter);
    if (m_name.trimmed() == "Device")
        return QVariant(Device::objectCounter);
    if (m_name.trimmed() == "DlnaFolder")
        return QVariant(DlnaFolder::objectCounter);
    if (m_name.trimmed() == "DlnaMusicTrackFile")
        return QVariant(DlnaMusicTrackFile::objectCounter);
    if (m_name.trimmed() == "DlnaRootFolder")
        return QVariant(DlnaRootFolder::objectCounter);
    if (m_name.trimmed() == "DlnaVideoFile")
        return QVariant(DlnaVideoFile::objectCounter);
    if (m_name.trimmed() == "DlnaYoutubeVideo")
        return QVariant(DlnaYouTubeVideo::objectCounter);
    if (m_name.trimmed() == "DlnaNetworkVideo")
        return QVariant(DlnaNetworkVideo::objectCounter);
    if (m_name.trimmed() == "DlnaNetworkPlaylist")
        return QVariant(DlnaNetworkPlaylist::objectCounter);
    if (m_name.trimmed() == "DlnaStorageFolder")
        return QVariant(DlnaStorageFolder::objectCounter);
    if (m_name.trimmed() == "DlnaCachedFolder")
        return QVariant(DlnaCachedFolder::objectCounter);
    if (m_name.trimmed() == "DlnaCachedFolderMetaData")
        return QVariant(DlnaCachedFolderMetaData::objectCounter);
    if (m_name.trimmed() == "DlnaCachedGroupedFolderMetaData")
        return QVariant(DlnaCachedGroupedFolderMetaData::objectCounter);
    if (m_name.trimmed() == "DlnaCachedPlaylists")
        return QVariant(DlnaCachedPlaylists::objectCounter);
    if (m_name.trimmed() == "DlnaCachedMusicTrack")
        return QVariant(DlnaCachedMusicTrack::objectCounter);
    if (m_name.trimmed() == "DlnaCachedNetworkVideo")
        return QVariant(DlnaCachedNetworkVideo::objectCounter);

    return QVariant("invalid");
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
}

bool DebugItem::setData(const QVariant &value, const int &role)
{
    qWarning() << "unable to set data" << value << role;
    return false;
}
