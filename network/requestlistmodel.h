#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include "Models/listmodel.h"
#include "Http/httprequest.h"

class RequestListModel : public ListModel
{
    Q_OBJECT

public:
    explicit RequestListModel(QObject *parent = 0);
    virtual ~RequestListModel();


signals:

private slots:

};

#endif // REQUESTLISTMODEL_H
