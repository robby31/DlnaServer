#include "myapplication.h"

int main(int argc, char** argv)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    qmlRegisterType<RequestListModel>("myTypes", 1, 0, "RequestListModel");
    qmlRegisterType<MediaRendererModel>("myTypes", 1, 0, "MediaRendererModel");
    qmlRegisterType<ListModel>("myTypes", 1, 0, "CListModel");

    MyApplication app(argc, argv, &database);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
