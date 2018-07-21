#include "myapplication.h"
#include "mysqldatabase.h"

int main(int argc, char** argv)
{
    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    int ret = app.exec();

    qDebug() << QSqlDatabase::connectionNames();

    return ret;
}
