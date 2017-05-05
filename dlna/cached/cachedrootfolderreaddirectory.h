#ifndef CACHEDROOTFOLDERREADDIRECTORY_H
#define CACHEDROOTFOLDERREADDIRECTORY_H

#include <QRunnable>

#include "dlnacachedrootfolder.h"

class CachedRootFolderReadDirectory : public QRunnable
{

public:
    CachedRootFolderReadDirectory(QDir folder);
    ~CachedRootFolderReadDirectory();

    void run();

private:
    QDir m_folder;
};

#endif // CACHEDROOTFOLDERREADDIRECTORY_H
