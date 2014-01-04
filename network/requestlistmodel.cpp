#include "requestlistmodel.h"


RequestListModel::RequestListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mRoles[Qt::UserRole] = "method";
    mRoles[Qt::UserRole+1] = "argument";
    mRoles[Qt::UserRole+2] = "host";
    mRoles[Qt::UserRole+3] = "peerAddress";
    mRoles[Qt::UserRole+4] = "status";
    mRoles[Qt::UserRole+5] = "header";
    mRoles[Qt::UserRole+6] = "content";
    mRoles[Qt::UserRole+7] = "duration";
    mRoles[Qt::UserRole+8] = "date";
    mRoles[Qt::UserRole+9] = "answer";
    mRoles[Qt::UserRole+10] = "network_status";
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

    if ( role - Qt::UserRole >= 0)
    {
        if (role == Qt::UserRole)
            return mRecords.at(index.row())->getMethod();
        else if (role == Qt::UserRole+1)
            return mRecords.at(index.row())->getArgument();
        else if (role == Qt::UserRole+2)
            return mRecords.at(index.row())->getHost();
        else if (role == Qt::UserRole+3)
            return mRecords.at(index.row())->getpeerAddress();
        else if (role == Qt::UserRole+4)
            return mRecords.at(index.row())->getStatus();
        else if (role == Qt::UserRole+5)
            return mRecords.at(index.row())->getTextHeader();
        else if (role == Qt::UserRole+6)
            return mRecords.at(index.row())->getTextContent();
        else if (role == Qt::UserRole+7)
            return mRecords.at(index.row())->getDuration();
        else if (role == Qt::UserRole+8)
            return mRecords.at(index.row())->getDate();
        else if (role == Qt::UserRole+9)
            return mRecords.at(index.row())->getTextAnswer();
        else if (role == Qt::UserRole+10)
            return mRecords.at(index.row())->getNetworkStatus();
    }

    return QVariant();
}

QHash<int, QByteArray> RequestListModel::roleNames() const
{
    return mRoles;
}

Request* RequestListModel::addRequest(Logger *log, QTcpSocket *client, QString uuid, QString servername, QString host, int port, DlnaRootFolder *rootFolder)
{
    Request* request = 0;

    if (client != 0) {
        request = new Request(log,
                              client,
                              uuid, servername,
                              host, port,
                              rootFolder);

        beginInsertRows(QModelIndex(), mRecords.length(), mRecords.length());
        mRecords.append(request);
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
    return data(this->index(index, 0), Qt::UserRole+roleIndex);
}
