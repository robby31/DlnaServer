#include "myapplication.h"
#include "mysqldatabase.h"
#include "mediamodel.h"

int main(int argc, char** argv)
{
    qmlRegisterType<MediaModel>("Model", 1, 0, "MediaModel");

    MyApplication app(argc, argv);

    // load the qml file describing the application
    app.loadMainQml(QUrl("qrc:/qml/main.qml"));

    int ret = MyApplication::exec();

    qDebug() << QSqlDatabase::connectionNames();

    DecryptYoutubeSignature::print_cache();

    return ret;
}
