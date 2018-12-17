#include "myapplication.h"
#include "serveritem.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<ServerItem>("Type", 1, 0, "ServerItem");

    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    return MyApplication::exec();
}
