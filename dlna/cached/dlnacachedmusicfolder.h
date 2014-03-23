#ifndef DLNACACHEDMUSICFOLDER_H
#define DLNACACHEDMUSICFOLDER_H

#include "dlnastoragefolder.h"
#include "dlnacachedfoldermetadata.h"

#include "medialibrary.h"

class DlnaCachedMusicFolder : public DlnaStorageFolder
{

public:
    explicit DlnaCachedMusicFolder(Logger* log, MediaLibrary* library, QString host, int port, int idType, QObject *parent = 0);

    virtual DlnaResource* getChild(int index, QObject *parent = 0) { Q_UNUSED(parent) return children.at(index); }
    virtual int getChildrenSize() { return children.size(); }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return "Music"; }

    virtual QString getSystemName() const { return "Music"; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return "Music"; }

private:
    QList<DlnaResource*> children;
};

#endif // DLNACACHEDMUSICFOLDER_H
