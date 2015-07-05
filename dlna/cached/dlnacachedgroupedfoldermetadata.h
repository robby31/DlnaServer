#ifndef DLNACACHEDGROUPEDFOLDERMETADATA_H
#define DLNACACHEDGROUPEDFOLDERMETADATA_H

#include "dlnastoragefolder.h"
#include "dlnacachedfoldermetadata.h"

#include "medialibrary.h"

class DlnaCachedGroupedFolderMetaData : public DlnaStorageFolder
{
    Q_OBJECT

public:
    DlnaCachedGroupedFolderMetaData(Logger* log, MediaLibrary* library, QString host, int port, QString name, QObject *parent = 0);

    virtual DlnaResource* getChild(int index, QObject *parent = 0);
    virtual int getChildrenSize() const { return children.size(); }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return name; }

    virtual QString getSystemName() const { return name; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return name; }

    void setNetworkAccessManager(QNetworkAccessManager *manager);

    void addFolder(QString stringQuery, QString stringQueryForChild, QString name);

private:
    MediaLibrary* library;
    QString name;
    QList<DlnaCachedFolderMetaData*> children;
};

#endif // DLNACACHEDGROUPEDFOLDERMETADATA_H
