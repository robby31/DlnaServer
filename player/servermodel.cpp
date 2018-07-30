#include "servermodel.h"

ServerModel::ServerModel(QObject *parent) :
    ListModel(new ServerItem, parent)
{

}

void ServerModel::addServer(UpnpRootDevice *device)
{
    if (device && device->deviceType().startsWith("urn:schemas-upnp-org:device:MediaServer"))
    {
        ServerItem *server = qobject_cast<ServerItem*>(find(device->uuid()));
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
    ServerItem *server = qobject_cast<ServerItem*>(sender());

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
        return (ServerItem *)at(index);
    else
        return Q_NULLPTR;
}
