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

    enum Roles {
        methodRole = Qt::UserRole+1,
        argumentRole,
        hostRole,
        peerAddressRole,
        statusRole,
        headerRole,
        contentRole,
        durationRole,
        dateRole,
        answerRole,
        networkStatusRole,
        transcodeLogRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int,QByteArray> roleNames() const { return mRoles; }

    void createRequest(Logger* log,
                        QTcpSocket* client, const QString &uuid,
                        const QString &servername, const QString &host, const int &port,
                        DlnaRootFolder* rootFolder, MediaRendererModel *renderersModel);

    Q_INVOKABLE QVariant get(const int &index, const int &roleIndex) const;

    // remove all requests from the model
    void clearAll();

signals:
    void newRequest(Request *request);

public slots:
    void addRequestInModel(Request *request);
    void requestChanged(const QString &roleChanged);

private:
    QList<Request *> mRecords;

    QHash<int, QByteArray> mRoles;
};

#endif // REQUESTLISTMODEL_H
