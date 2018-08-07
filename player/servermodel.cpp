#include "servermodel.h"

ServerModel::ServerModel(QObject *parent) :
    ListModel(new ServerItem, parent)
{

}

void ServerModel::addServer(UpnpRootDevice *device)
{
    if (device && device->deviceType().startsWith("urn:schemas-upnp-org:device:MediaServer"))
    {
        auto server = qobject_cast<ServerItem*>(find(device->uuid()));
        if (!server)
        {
            server = new ServerItem(device, this);
            connect(server, &ServerItem::removeItem, this, &ServerModel::removeServer);
            appendRow(server);
        }
    }
}

void ServerModel::removeServer()
{
    auto server = qobject_cast<ServerItem*>(sender());

    if (server)
    {
        QModelIndex index = indexFromItem(server);

        if (index.isValid())
            removeRow(index.row());
    }
}

ServerItem *ServerModel::getServer(const int &index)
{
    if (index>=0 && index<rowCount())
        return qobject_cast<ServerItem *>(at(index));

    return Q_NULLPTR;
}
