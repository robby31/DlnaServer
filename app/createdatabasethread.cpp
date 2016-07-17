#include "createdatabasethread.h"

CreateDatabaseThread::CreateDatabaseThread(QObject *parent):
    QObject(parent)
{
    connect(this, SIGNAL(runSignal()), this, SLOT(runSlot()));
}

CreateDatabaseThread::~CreateDatabaseThread()
{
    REMOVE_DATABASE("MEDIA_DATABASE");
}

void CreateDatabaseThread::runSlot()
{
    QSqlDatabase database = CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE");
    database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
}
