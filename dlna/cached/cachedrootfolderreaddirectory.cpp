#include "cachedrootfolderreaddirectory.h"

CachedRootFolderReadDirectory::CachedRootFolderReadDirectory(Logger *log, QDir folder):
    m_log(log),
    m_folder(folder)
{

}

CachedRootFolderReadDirectory::~CachedRootFolderReadDirectory()
{
    qDebug() << "Read directory" << m_folder << "finished.";
}

void CachedRootFolderReadDirectory::run()
{
    {
        // initialize database in current Thread
        QSqlDatabase database = CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE");
        database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");

        if (database.isValid())
        {
            DlnaCachedRootFolder root(m_log, "HOST", 80);
            root.readDirectory(m_folder);
        }
        else
        {
            qCritical() << "unable to initialize database connection.";
        }
    }

    REMOVE_DATABASE("MEDIA_DATABASE");
}
