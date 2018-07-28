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
            connect(server, &ServerItem::destroyed, this, &ServerModel::serverDestroyed);
            appendRow(server);
        }
    }
}

void ServerModel::serverDestroyed(QObject *object)
{
    ServerItem *server = qobject_cast<ServerItem*>(object);

    QModelIndex index = indexFromItem(server);

    if (index.isValid())
        removeRow(index.row());
}
