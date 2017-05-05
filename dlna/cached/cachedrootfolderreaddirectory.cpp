#include "cachedrootfolderreaddirectory.h"

CachedRootFolderReadDirectory::CachedRootFolderReadDirectory(QDir folder):
    QRunnable(),
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
        database.setConnectOptions("Pooling=True;Max Pool Size=100;");

        if (database.isValid())
        {
            DlnaCachedRootFolder root("HOST", 80);
            root.readDirectory(m_folder);
        }
        else
        {
            qCritical() << "unable to initialize database connection.";
        }
    }

    REMOVE_DATABASE("MEDIA_DATABASE");
}
