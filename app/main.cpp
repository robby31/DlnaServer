#include "myapplication.h"
#include "mysqldatabase.h"

int main(int argc, char** argv)
{
    QSqlDatabase database = CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE");
    database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    database.setConnectOptions("Pooling=True;Max Pool Size=100;");

    qmlRegisterType<MediaRendererModel>("myTypes", 1, 0, "MediaRendererModel");

    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    int ret = app.exec();

    qDebug() << QSqlDatabase::connectionNames();

    return ret;
}
