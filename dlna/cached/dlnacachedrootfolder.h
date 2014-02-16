#ifndef DLNACACHEDROOTFOLDER_H
#define DLNACACHEDROOTFOLDER_H

#include "dlnarootfolder.h"

#include <qmimedatabase.h>
#include "medialibrary.h"

#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include "dlnacachedfolder.h"
#include "dlnacachedmusicfolder.h"
#include "dlnarootfolder.h"

class DlnaCachedRootFolder : public DlnaRootFolder
{
public:
    DlnaCachedRootFolder(Logger* log, QString host, int port, QObject *parent = 0);

    // returns true if the folder is added to Root.
    virtual bool addFolder(QString path);

    // Parse children and return true if done
    virtual bool discoverChildren();

private:
    void readDirectory(QDir folder);

    void addResource(QFileInfo fileinfo);

    MediaLibrary library;
    QMimeDatabase mimeDb;
    DlnaRootFolder rootFolder;
};

#endif // DLNACACHEDROOTFOLDER_H
