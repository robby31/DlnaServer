#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include <QAbstractTableModel>
#include "request.h"
#include "logger.h"

class RequestListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RequestListModel(Logger *log, QObject *parent = 0);
    explicit RequestListModel(QObject *parent = 0);
    virtual ~RequestListModel();

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

    Q_INVOKABLE QVariant get(const int &index, const int &roleIndex) const;

    // remove all requests from the model
    void clearAll();

signals:
    void newRequest(Request *request);

private slots:
    void createRequest(qintptr socket, QString uuid, QString servername, QString host, int port);
    void requestChanged(const QString &roleChanged);
    void requestDestroyed(Request *request);

private:
    Logger *m_log;
    QList<Request *> mRecords;
    QHash<int, QByteArray> mRoles;
};

#endif // REQUESTLISTMODEL_H
