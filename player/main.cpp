#include "myapplication.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
