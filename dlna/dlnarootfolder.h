#ifndef DLNAROOTFOLDER_H
#define DLNAROOTFOLDER_H

#include <QFileInfo>
#include "dlnastoragefolder.h"
#include "dlnafolder.h"

class DlnaRootFolder : public DlnaStorageFolder
{

public:
    explicit DlnaRootFolder(Logger* log, QString host, int port, QObject *parent = 0);
    virtual ~DlnaRootFolder();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return QString("root"); }

    virtual QString getSystemName() const { return getName(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return getName(); }

    /*
     * Adds a new DLNAResource to the child list.
     * Only useful if this object is of the container type.
     */
    void addChild(DlnaResource *child);
    void clearChildren() { children.clear(); }

    virtual int getChildrenSize() { return children.size(); }
    virtual DlnaResource* getChild(int index, QObject *parent = 0) { Q_UNUSED(parent) return children.at(index); }

    // returns true if the folder is added to Root.
    virtual bool addFolder(QString folder);

private:
    QList<DlnaResource*> children;
};

#endif // DLNAROOTFOLDER_H
