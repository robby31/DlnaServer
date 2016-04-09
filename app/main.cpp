#include "myapplication.h"

int main(int argc, char** argv)
{
    qmlRegisterType<RequestListModel>("myTypes", 1, 0, "RequestListModel");
    qmlRegisterType<MediaRendererModel>("myTypes", 1, 0, "MediaRendererModel");

    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
