#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include <QAbstractTableModel>
#include "request.h"

class RequestListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RequestListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int,QByteArray> roleNames() const;

    void addRequest(Request *request);

    Q_INVOKABLE QVariant get(int index, int roleIndex);

private:
    QList<Request *> mRecords;

    QHash<int, QByteArray> mRoles;
};

#endif // REQUESTLISTMODEL_H
