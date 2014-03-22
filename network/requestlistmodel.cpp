#include "requestlistmodel.h"


RequestListModel::RequestListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mRoles[methodRole] = "method";
    mRoles[argumentRole] = "argument";
    mRoles[hostRole] = "host";
    mRoles[peerAddressRole] = "peerAddress";
    mRoles[statusRole] = "status";
    mRoles[headerRole] = "header";
    mRoles[contentRole] = "content";
    mRoles[durationRole] = "duration";
    mRoles[dateRole] = "date";
    mRoles[answerRole] = "answer";
    mRoles[networkStatusRole] = "network_status";
    mRoles[transcodeLogRole] = "transcode_log";
}

RequestListModel::~RequestListModel() {
    // clear the model
    clearAll();
}

void RequestListModel::clearAll() {
    // remove all requests
    while (!mRecords.isEmpty()) {
        Request* request = mRecords.takeFirst();
        delete request;
    }
}

int RequestListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mRecords.count();
}

int RequestListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mRoles.keys().length();
}

QVariant RequestListModel::data(const QModelIndex &index, int role) const
{
    if ( mRecords.count() <= 0)
        return QVariant();

    switch (role) {
    case methodRole:
        return mRecords.at(index.row())->getMethod();
    case argumentRole:
        return mRecords.at(index.row())->getArgument();
    case hostRole:
        return QString("%1 (%2)").arg(mRecords.at(index.row())->getHost()).arg(mRecords.at(index.row())->socketDescriptor());
    case peerAddressRole:
        return mRecords.at(index.row())->getpeerAddress();
    case statusRole:
        return mRecords.at(index.row())->getStatus();
    case headerRole:
        return mRecords.at(index.row())->getTextHeader();
    case contentRole:
        return mRecords.at(index.row())->getTextContent();
    case durationRole:
        return mRecords.at(index.row())->getDuration();
    case dateRole:
        return mRecords.at(index.row())->getDate();
    case answerRole:
        return mRecords.at(index.row())->getTextAnswer();
    case networkStatusRole:
        return mRecords.at(index.row())->getNetworkStatus();
    case transcodeLogRole:
        return mRecords.at(index.row())->getTranscodeLog();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> RequestListModel::roleNames() const
{
    return mRoles;
}

Request* RequestListModel::addRequest(Logger *log, QTcpSocket *client, QString uuid, QString servername, QString host, int port, DlnaRootFolder *rootFolder, MediaRendererModel *renderersModel)
{
    Request* request = 0;

    if (client != 0) {
        request = new Request(log,
                              client,
                              uuid, servername,
                              host, port,
                              rootFolder, renderersModel,
                              this);

        beginInsertRows(QModelIndex(), 0, 0);
        mRecords.prepend(request);
        endInsertRows();

        connect(request, SIGNAL(dataChanged()), this, SLOT(requestChanged()));
    }

    return request;
}

void RequestListModel::requestChanged() {
    int requestIndex = mRecords.indexOf(static_cast<Request*>(sender()));
    emit dataChanged(index(requestIndex, 0), index(requestIndex, columnCount()-1));
}

QVariant RequestListModel::get(int index, int roleIndex)
{
    return data(this->index(index, 0), Qt::UserRole+1+roleIndex);
}
