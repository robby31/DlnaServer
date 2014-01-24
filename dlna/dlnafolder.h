#ifndef DLNAFOLDER_H
#define DLNAFOLDER_H

#include "dlnaresource.h"
#include "dlnamusictrack.h"
#include "dlnavideoitem.h"
#include <QFileInfo>
#include <QDir>

class DlnaFolder : public DlnaResource
{
public:
    DlnaFolder(Logger* log, QString filename, QString host, int port, QObject *parent = 0);
    ~DlnaFolder();

    virtual bool discoverChildren();

    // returns the file path of children
    QList<QFileInfo> getChildrenFileInfo();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName();

    virtual bool isFolder() const { return true; }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt();

private:
    QFileInfo fileinfo;
    QString host;
    int port;
};

#endif // DLNAFOLDER_H
