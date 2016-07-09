#include "myapplication.h"

int main(int argc, char** argv)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", "MEDIA_DATABASE");
    database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");

    qmlRegisterType<RequestListModel>("myTypes", 1, 0, "RequestListModel");
    qmlRegisterType<MediaRendererModel>("myTypes", 1, 0, "MediaRendererModel");
    qmlRegisterType<ListModel>("myTypes", 1, 0, "CListModel");

    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    int ret = app.exec();

    database.removeDatabase("MEDIA_DATABASE");

    return ret;
}
