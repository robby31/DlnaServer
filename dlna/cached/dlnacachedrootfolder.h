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
    DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent = 0);

    // returns true if the folder is added to Root.
    virtual bool addFolder(QString path);

    DlnaRootFolder* getRootFolder() { return &rootFolder; }

    void addResource(QFileInfo fileinfo);

    QSqlDatabase *getDatabase() const { return library.getDatabase(); }

private:
    MediaLibrary library;
    QMimeDatabase mimeDb;
    DlnaRootFolder rootFolder;

    void readDirectory(QDir folder);
};

#endif // DLNACACHEDROOTFOLDER_H
