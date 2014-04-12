#include "requestlistmodel.h"

RequestListModel::RequestListModel(QObject *parent) :
    QAbstractTableModel(parent),
    mRecords(),
    mRoles()
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
    qDeleteAll(mRecords);
    mRecords.clear();
}

int RequestListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mRecords.count();
}

int RequestListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mRoles.keys().length();
}

QVariant RequestListModel::data(const QModelIndex &index, int role) const
{
    Request *item = 0;

    if ( index.row() >= 0 and index.row() < mRecords.size() )
        item = mRecords.at(index.row());

    if (item != 0) {
        switch (role) {
        case methodRole:
            return item->getMethod();
        case argumentRole:
            return item->getArgument();
        case hostRole:
            return QString("%1 (%2)").arg(item->getHost()).arg(item->socketDescriptor());
        case peerAddressRole:
            return item->getpeerAddress();
        case statusRole:
            return item->getStatus();
        case headerRole:
            return item->getTextHeader();
        case contentRole:
            return item->getTextContent();
        case durationRole:
            return item->getDuration();
        case dateRole:
            return item->getDate();
        case answerRole:
            return item->getTextAnswer();
        case networkStatusRole:
            return item->getNetworkStatus();
        case transcodeLogRole:
            return item->getTranscodeLog();
        default:
            return QVariant::Invalid;

        }
    }

    return QVariant::Invalid;
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
