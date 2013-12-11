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
}

int RequestListModel::rowCount(const QModelIndex &parent) const
{
    return mRecords.count();
}

int RequestListModel::columnCount(const QModelIndex &parent) const
{
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
    }

    return QVariant();
}

QHash<int, QByteArray> RequestListModel::roleNames() const
{
    return mRoles;
}

void RequestListModel::addRequest(Request *request)
{
    beginInsertRows(QModelIndex(), mRecords.length(), mRecords.length());
    mRecords.append(request);
    endInsertRows();
}

QVariant RequestListModel::get(int index, int roleIndex)
{
    return data(this->index(index, 0), Qt::UserRole+roleIndex);
}
