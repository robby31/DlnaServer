#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include "Models/listmodel.h"
#include "serveritem.h"

class ServerModel : public ListModel
{
    Q_OBJECT

public:
    explicit ServerModel(QObject *parent = nullptr);

    Q_INVOKABLE ServerItem *getServer(const int &index);

signals:

public slots:
    void addServer(UpnpRootDevice *device);

private slots:
    void removeServer();
};

#endif // SERVERMODEL_H
