#include "mediarenderermodel.h"

MediaRendererModel::MediaRendererModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mRoles[statusRole] = "status";
    mRoles[nameRole] = "name";
    mRoles[networkAddressRole] = "networkAddress";
    mRoles[userAgentRole] = "userAgent";
}

void MediaRendererModel::clearAll() {
    // remove all renderers
    while (!mRecords.isEmpty()) {
        MediaRenderer* renderer = mRecords.takeFirst();
        delete renderer;
    }
}

int MediaRendererModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mRecords.count();
}

int MediaRendererModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mRoles.keys().length();
}

QVariant MediaRendererModel::data(const QModelIndex &index, int role) const
{
    if ( mRecords.count() <= 0)
        return QVariant();

    switch (role) {
    case statusRole:
        return mRecords.at(index.row())->getStatus();
    case nameRole:
        return mRecords.at(index.row())->getName();
    case networkAddressRole:
        return mRecords.at(index.row())->getNetworkAddress();
    case userAgentRole:
        return mRecords.at(index.row())->getUserAgent();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MediaRendererModel::roleNames() const
{
    return mRoles;
}

MediaRenderer* MediaRendererModel::addRenderer(Logger *log, QString ip, int port, QString userAgent)
{
    MediaRenderer* renderer = 0;

    renderer = new MediaRenderer(log, ip, port, userAgent, this);

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
    renderer->setStatus(QString("Serving %1").arg(mediaName));
}

void MediaRendererModel::stopServing(QString ip) {
    MediaRenderer* renderer = getFromIp(ip);
    renderer->setStatus("standby");
}
