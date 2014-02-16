#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>
#include <QtQuick/QQuickView>
#include <QSettings>

#include "httpserver.h"
#include "upnphelper.h"
#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"

class Application : public QObject
{
    Q_OBJECT

public:
    Application(QQmlApplicationEngine *engine, QObject *parent = 0);

    // load the qml file describing the HMI
    int load(QUrl url);

    Q_INVOKABLE void addSharedFolder(QUrl folder);
    Q_INVOKABLE void removeFolder(int index);

public slots:

    // quit the application
    void quit();

private:
    QSettings* settings;
    QStringList sharedFolderModel;

    QQmlApplicationEngine *engine;

    // TopLevel window of the application
    QObject *topLevel;

    Logger log;

    HttpServer *server;

    UPNPHelper *upnp;

    // list of requests received by server
    RequestListModel requestsModel;

    // list of renderes connected to the server
    MediaRendererModel renderersModel;

    bool loadSettings();
    bool saveSettings();
};

#endif // APPLICATION_H
