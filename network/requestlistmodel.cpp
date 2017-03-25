#include "requestlistmodel.h"

RequestListModel::RequestListModel(QObject *parent) :
    ListModel(new HttpRequest, parent)
{
}


RequestListModel::~RequestListModel()
{
    qWarning() << "Remove all requests, size =" << rowCount();
}
