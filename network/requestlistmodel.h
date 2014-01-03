#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include <QAbstractTableModel>
#include "request.h"

class RequestListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RequestListModel(QObject *parent = 0);
    ~RequestListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int,QByteArray> roleNames() const;

    // Create a new request and add it to the model
    Request* addRequest(Logger* log,
                        QTcpSocket* client, QString uuid,
                        QString servername, QString host, int port,
                        DlnaRootFolder *rootFolder);

    Q_INVOKABLE QVariant get(int index, int roleIndex);

    // remove all requests from the model
    void clearAll();

public slots:
    void requestChanged();

private:
    QList<Request *> mRecords;

    QHash<int, QByteArray> mRoles;
};

#endif // REQUESTLISTMODEL_H
