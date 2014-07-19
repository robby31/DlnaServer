#ifndef DLNACACHEDFOLDERMETADATA_H
#define DLNACACHEDFOLDERMETADATA_H

#include "dlnastoragefolder.h"
#include "dlnacachedfolder.h"

#include "medialibrary.h"

class DlnaCachedFolderMetaData : public DlnaStorageFolder
{

public:
    explicit DlnaCachedFolderMetaData(Logger* log, MediaLibrary* library, int typeMedia, QString metaData, QString name, QString host, int port, QObject *parent = 0);

    virtual DlnaResource* getChild(int index, QObject *parent = 0);
    virtual int getChildrenSize() const { return nbChildren; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return name; }

    virtual QString getSystemName() const { return name; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return name; }

private:
    MediaLibrary* library;
    QString metaData;
    QString name;
    int typeMedia;
    QSqlQuery query;
    int nbChildren;
};

#endif // DLNACACHEDFOLDERMETADATA_H
