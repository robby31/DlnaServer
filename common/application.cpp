#include "application.h"
#include <QDebug>
#include <QtQuick/QQuickView>


Application::Application(QQmlApplicationEngine *engine, QObject *parent):
    QObject(parent),
    engine(engine),
    topLevel(engine->rootObjects().value(0))
{
    server = new HttpServer(&log, &requestsModel, &renderersModel, this);
    upnp = new UPNPHelper(&log, server, this);

    log.setLevel(INF);
}

int Application::load(QUrl url)
{
    engine->rootContext()->setContextProperty("requestsModel", &requestsModel);
    engine->rootContext()->setContextProperty("renderersModel", &renderersModel);

    engine->load(url);

    if (!topLevel)
    {
        topLevel = engine->rootObjects().value(0);

        QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
        if ( !window ) {
            log.ERROR("Your root item has to be a Window.");
            return -1;
        }
        window->show();
        connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(quit()));
    }

    return 0;
}

void Application::quit()
{
    log.TRACE("Quit Application.");
}
