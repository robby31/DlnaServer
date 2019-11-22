#include "myapplication.h"
#include "mysqldatabase.h"
#include "mediamodel.h"
#include "debuginfo.h"

int main(int argc, char** argv)
{
    qmlRegisterType<MediaModel>("Model", 1, 0, "MediaModel");

    int ret;

    {
        MyApplication app(argc, argv);

        // load the qml file describing the application
        app.loadMainQml(QUrl("qrc:/qml/main.qml"));

        ret = MyApplication::exec();
    }

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << QSqlDatabase::connectionNames();
    #endif

    DecryptYoutubeSignature::print_cache();

    DebugInfo::display_alive_objects();

    return ret;
}
