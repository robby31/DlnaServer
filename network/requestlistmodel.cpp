#include "requestlistmodel.h"

RequestListModel::RequestListModel(Logger *log, QObject *parent) :
    ListModel(new Request, parent),
    m_log(log)
{
}

RequestListModel::RequestListModel(QObject *parent) :
    ListModel(new Request, parent),
    m_log(0)
{
}


RequestListModel::~RequestListModel()
{
    qWarning() << "Remove all requests, size =" << rowCount();
}

void RequestListModel::createRequest(qintptr socket, QString uuid, QString servername, QString host, int port)
{
    Request *request = new Request(m_log, socket, uuid, servername, host, port, this);

    insertRow(0, request);

    emit newRequest(request);
}

void RequestListModel::requestDestroyed(Request *request)
{
    QModelIndex requestIndex = indexFromItem(request);
    qWarning() << "request destroyed" << request << requestIndex;

    if (requestIndex.isValid())
        removeRow(requestIndex.row());

    request->deleteLater();
}
