#include "application.h"

int main(int argc, char** argv)
{
    qmlRegisterType<RequestListModel>("myTypes", 1, 0, "RequestListModel");
    qmlRegisterType<MediaRendererModel>("myTypes", 1, 0, "MediaRendererModel");

    Application app(argc, argv);

    // load the qml file describing the application
    if (app.load(QUrl("qrc:/qml/main.qml")) == -1)
        return -1;

    return app.exec();
}
