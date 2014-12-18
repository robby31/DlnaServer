#ifndef DLNAROOTFOLDER_H
#define DLNAROOTFOLDER_H

#include <QFileInfo>
#include "dlnastoragefolder.h"
#include "dlnafolder.h"

class DlnaRootFolder : public DlnaStorageFolder
{
    Q_OBJECT

public:
    explicit DlnaRootFolder(Logger* log, QString host, int port, QObject *parent = 0);
    virtual ~DlnaRootFolder();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return QString("root"); }

    virtual QString getSystemName() const { return getName(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return getName(); }

    virtual int getChildrenSize() const { return children.size(); }
    virtual DlnaResource* getChild(int index, QObject *parent = 0);

    /*
     * Adds a new DLNAResource to the child list.
     * Only useful if this object is of the container type.
     */
    void addChild(DlnaResource *child)  { emit addChildSignal(child); }
    void clearChildren()                { emit clearChildrenSignal(); }

    void addFolder(QString path) { emit addFolderSignal(path); }

signals:
    void addFolderSignal(const QString &path);
    void folderAddedSignal(const QString &folder);
    void error_addFolder(QString folder);

    void addChildSignal(DlnaResource *child);
    void clearChildrenSignal();

private slots:
    // returns true if the folder is added to Root.
    virtual bool addFolderSlot(QString folder);

    void addChildSlot(DlnaResource *child);
    void clearChildrenSlot() { children.clear(); }

private:
    QList<DlnaResource*> children;
};

#endif // DLNAROOTFOLDER_H
