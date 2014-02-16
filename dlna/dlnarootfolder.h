#ifndef DLNAROOTFOLDER_H
#define DLNAROOTFOLDER_H

#include <QFileInfo>
#include "dlnastoragefolder.h"
#include "dlnafolder.h"

class DlnaRootFolder : public DlnaStorageFolder
{
public:
    DlnaRootFolder(Logger* log, QString host, int port, QObject *parent = 0);

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return QString("root"); }

    virtual QString getSystemName() const { return getName(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return getName(); }

    // returns true if the folder is added to Root.
    virtual bool addFolder(QString folder);
};

#endif // DLNAROOTFOLDER_H
