#ifndef DLNAROOTFOLDER_H
#define DLNAROOTFOLDER_H

#include <QFileInfo>
#include "dlnaresource.h"
#include "dlnafolder.h"

class DlnaRootFolder : public DlnaResource
{
public:
    DlnaRootFolder(Logger* log, QString host, int port, QObject *parent = 0);
    ~DlnaRootFolder();

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName();

    virtual bool isFolder() const { return true; }

    // returns true if the folder is added to Root.
    bool addFolder(QString folder);

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt();

private:
    QString host;
    int port;
};

#endif // DLNAROOTFOLDER_H
