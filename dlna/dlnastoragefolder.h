#ifndef DLNASTORAGEFOLDER_H
#define DLNASTORAGEFOLDER_H

#include "dlnaresource.h"

class DlnaStorageFolder : public DlnaResource
{
public:
    DlnaStorageFolder(Logger* log, QString host, int port, QObject *parent = 0);

    // Return upnp class
    virtual QString getUpnpClass() const { return QString("object.container.storageFolder"); }

    virtual bool isFolder() const { return true; }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() { return QImage(); }

protected:
    QString host;
    int port;
};

#endif // DLNASTORAGEFOLDER_H
