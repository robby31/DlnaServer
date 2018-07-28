#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include "Models/listmodel.h"
#include "serveritem.h"

class ServerModel : public ListModel
{
    Q_OBJECT

public:
    explicit ServerModel(QObject *parent = nullptr);

signals:

public slots:
    void addServer(UpnpRootDevice *device);
    void serverDestroyed(QObject *object);
};

#endif // SERVERMODEL_H
