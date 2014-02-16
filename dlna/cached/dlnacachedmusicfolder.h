#ifndef DLNACACHEDMUSICFOLDER_H
#define DLNACACHEDMUSICFOLDER_H

#include "dlnastoragefolder.h"
#include "dlnacachedfoldermetadata.h"

#include "medialibrary.h"

class DlnaCachedMusicFolder : public DlnaStorageFolder
{
public:
    DlnaCachedMusicFolder(Logger* log, MediaLibrary* library, QString host, int port, QObject *parent = 0);

    virtual bool discoverChildren();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return "Music"; }

    virtual QString getSystemName() const { return "Music"; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return "Music"; }

private:
    MediaLibrary* library;
};

#endif // DLNACACHEDMUSICFOLDER_H
