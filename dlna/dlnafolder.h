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
    explicit DlnaFolder(Logger* log, QString filename, QString host, int port, QObject *parent = 0);
    virtual ~DlnaFolder();

    virtual DlnaResource* getChild(int index, QObject *parent = 0);
    virtual int getChildrenSize() const { return children.size(); }

    // returns the file path of children
    QList<QFileInfo> getChildrenFileInfo() const { return children; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return fileinfo.fileName(); }

    virtual QString getSystemName() const { return fileinfo.absoluteFilePath(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return fileinfo.completeBaseName(); }

private:
    QFileInfo fileinfo;
    QList<QFileInfo> children;
};

#endif // DLNAFOLDER_H
