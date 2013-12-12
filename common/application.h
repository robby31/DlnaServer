#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>

#include "httpserver.h"
#include "upnphelper.h"
#include "logger.h"
#include "requestlistmodel.h"

class Application : public QObject
{
    Q_OBJECT

public:
    Application(QQmlApplicationEngine *engine);

    // load the qml file describing the HMI
    int load(QUrl url);

public slots:

    // quit the application
    void quit();

private:

    QQmlApplicationEngine *engine;

    // TopLevel window of the application
    QObject *topLevel;

    Logger log;

    HttpServer *server;

    UPNPHelper *upnp;

    // list of requests received by server
    RequestListModel requestsModel;

};

#endif // APPLICATION_H
