#ifndef CACHEDROOTFOLDERREADDIRECTORY_H
#define CACHEDROOTFOLDERREADDIRECTORY_H

#include <QRunnable>

#include "dlnacachedrootfolder.h"
#include "logger.h"

class CachedRootFolderReadDirectory : public QRunnable
{

public:
    CachedRootFolderReadDirectory(Logger *log, QDir folder);
    ~CachedRootFolderReadDirectory();

    void run();

private:
    Logger *m_log;
    QDir m_folder;
};

#endif // CACHEDROOTFOLDERREADDIRECTORY_H
