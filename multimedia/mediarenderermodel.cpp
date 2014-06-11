#include "mediarenderermodel.h"

MediaRendererModel::MediaRendererModel(QObject *parent) :
    QAbstractTableModel(parent),
    mRecords(),
    mRoles()
{
    mRoles[statusRole] = "status";
    mRoles[nameRole] = "name";
    mRoles[networkAddressRole] = "networkAddress";
    mRoles[userAgentRole] = "userAgent";
}

MediaRendererModel::~MediaRendererModel()
{
    clearAll();
}

void MediaRendererModel::clearAll() {
    // remove all renderers
    qDeleteAll(mRecords);
    mRecords.clear();
}

int MediaRendererModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mRecords.count();
}

int MediaRendererModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mRoles.keys().length();
}

QVariant MediaRendererModel::data(const QModelIndex &index, int role) const
{
    MediaRenderer *item = 0;

    if ( index.row() >= 0 and index.row() < mRecords.size() )
        item = mRecords.at(index.row());

    if (item != 0) {
        switch (role) {
        case statusRole:
            return item->getStatus();
        case nameRole:
            return item->getName();
        case networkAddressRole:
            return item->getNetworkAddress();
        case userAgentRole:
            return item->getUserAgent();
        default:
            return QVariant::Invalid;
        }
    }

    return QVariant::Invalid;
}

MediaRenderer* MediaRendererModel::addRenderer(Logger *log, QString ip, int port, QString userAgent)
{
    MediaRenderer* renderer = 0;

    renderer = new MediaRenderer(log, ip, port, userAgent);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mRecords.append(renderer);
    endInsertRows();

    connect(renderer, SIGNAL(dataChanged()), this, SLOT(rendererChanged()));

    return renderer;
}

void MediaRendererModel::rendererChanged() {
    int rendererIndex = mRecords.indexOf(static_cast<MediaRenderer*>(sender()));
    emit dataChanged(index(rendererIndex, 0), index(rendererIndex, columnCount()-1));
}

QVariant MediaRendererModel::get(int index, int roleIndex)
{
    return data(this->index(index, 0), Qt::UserRole+1+roleIndex);
}

MediaRenderer* MediaRendererModel::getFromIp(QString ip) {
    foreach(MediaRenderer* renderer, mRecords) {
        if (renderer->getNetworkAddress() == ip) {
            return renderer;
        }
    }

    return 0;
}

void MediaRendererModel::serving(QString ip, QString mediaName) {
    MediaRenderer* renderer = getFromIp(ip);
    if (renderer != 0)
        renderer->setStatus(QString("Serving %1").arg(mediaName));
}

void MediaRendererModel::stopServing(QString ip) {
    MediaRenderer* renderer = getFromIp(ip);
    if (renderer != 0)
        renderer->setStatus("standby");
}
