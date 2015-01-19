#include "requestlistmodel.h"

RequestListModel::RequestListModel(Logger *log, QObject *parent) :
    QAbstractTableModel(parent),
    m_log(log),
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

RequestListModel::RequestListModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_log(0),
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
    qWarning() << "Remove all requests, size =" << mRecords.size();
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
            return item->getLog();
        default:
            return QVariant::Invalid;

        }
    }

    return QVariant::Invalid;
}

void RequestListModel::createRequest(qintptr socket, QString uuid, QString servername, QString host, int port)
{
    Request *request = new Request(m_log, socket, uuid, servername, host, port, this);

    beginInsertRows(QModelIndex(), 0, 0);
    mRecords.prepend(request);
    endInsertRows();

    connect(request, SIGNAL(dataChanged(QString)), this, SLOT(requestChanged(QString)));

    emit newRequest(request);
}

void RequestListModel::requestDestroyed(Request *request)
{
    int requestIndex = mRecords.indexOf(request);
    qWarning() << "request destroyed" << request << requestIndex;
    if (requestIndex>=0 && requestIndex<mRecords.size())
    {
        beginRemoveRows(QModelIndex(), requestIndex, requestIndex);
        mRecords.removeAt(requestIndex);
        endRemoveRows();
    }

    request->deleteLater();
}

void RequestListModel::requestChanged(const QString &roleChanged) {
    int roleChangedIndex = mRoles.key(QByteArray(roleChanged.toUtf8().constData()));
    if (mRoles.contains(roleChangedIndex)) {
        int columnIndex = roleChangedIndex-Qt::UserRole-1;
        int requestIndex = mRecords.indexOf(qobject_cast<Request*>(sender()));
        if (requestIndex!=-1) {
            QVector<int> rolesChanged;
            rolesChanged.append(roleChangedIndex);
            emit dataChanged(index(requestIndex, columnIndex), index(requestIndex, columnIndex), rolesChanged);
        } else {
            qWarning() << "requestChanged ERROR, sender" << sender() << "is unknown" << roleChanged;
        }
    } else {
        qWarning() << "requestChanged ERROR, role" << roleChanged << "is unknown";
    }
}

QVariant RequestListModel::get(const int &index, const int &roleIndex) const
{
    return data(this->index(index, 0), Qt::UserRole+1+roleIndex);
}
