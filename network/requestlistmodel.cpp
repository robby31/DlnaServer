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

void RequestListModel::createRequest(HttpClient *client, QString uuid, QString servername, QString host, int port)
{
    Request *request = new Request(m_log, client, uuid, servername, host, port, this);

    insertRow(0, request);

    emit newRequest(request);
}

void RequestListModel::requestDestroyed(Request *request)
{
    QModelIndex requestIndex = indexFromItem(request);

    if (requestIndex.isValid())
        removeRow(requestIndex.row());
}
