#ifndef DLNAFOLDER_H
#define DLNAFOLDER_H

#include "dlnastoragefolder.h"
#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include <QFileInfo>
#include <QDir>

class DlnaFolder : public DlnaStorageFolder
{
public:
    DlnaFolder(Logger* log, QString filename, QString host, int port, QObject *parent = 0);

    virtual bool discoverChildren();

    virtual int getChildrenSize() { return getChildrenFileInfo().size(); }

    // returns the file path of children
    QList<QFileInfo> getChildrenFileInfo();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return fileinfo.fileName(); }

    virtual QString getSystemName() const { return fileinfo.absoluteFilePath(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return fileinfo.completeBaseName(); }

private:
    QFileInfo fileinfo;
};

#endif // DLNAFOLDER_H
