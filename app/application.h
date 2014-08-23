#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>
#include <QtQuick/QQuickView>
#include <QSettings>
#include <QThread>

#include "httpserver.h"
#include "upnphelper.h"
#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"

class Application : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList sharedFolderModel READ sharedFolderModel WRITE setsharedFolderModel NOTIFY sharedFolderModelChanged)
    Q_PROPERTY(RequestListModel *requestsModel READ requestsModel WRITE setRequestsModel NOTIFY requestsModelChanged)
    Q_PROPERTY(MediaRendererModel *renderersModel READ renderersModel WRITE setRenderersModel NOTIFY renderersModelChanged)

public:
    Application(QQmlApplicationEngine *engine, QObject *parent = 0);

    // load the qml file describing the HMI
    int load(const QUrl &url);

    Q_INVOKABLE void addSharedFolder(const QUrl &folder);
    Q_INVOKABLE void removeFolder(const int &index);
    Q_INVOKABLE void addNetworkLink(const QString &url);

    QStringList sharedFolderModel() const { return m_sharedFolderModel; }
    void setsharedFolderModel(const QStringList &model) { m_sharedFolderModel = model; emit sharedFolderModelChanged(); }

    RequestListModel *requestsModel() const { return m_requestsModel; }
    void setRequestsModel(RequestListModel *model) { m_requestsModel = model; emit requestsModelChanged(); }

    MediaRendererModel *renderersModel() const { return m_renderersModel; }
    void setRenderersModel(MediaRendererModel *model) { m_renderersModel = model; emit renderersModelChanged(); }

private:
    bool loadSettings();
    bool saveSettings();

    bool reloadLibrary();

public slots:
    void folderAdded(const QString &folder);
    void folderNotAdded(const QString &folder);

    void linkAdded(const QString &url);
    void linkNotAdded(const QString &url);

    // quit the application
    void quit();

signals:
    void sharedFolderModelChanged();
    void requestsModelChanged();
    void renderersModelChanged();

    void addFolder(QString folder);
    void addLink(QString url);

private:
    QSettings* settings;
    QStringList m_sharedFolderModel;

    QQmlApplicationEngine *engine;

    // TopLevel window of the application
    QObject *topLevel;

    Logger log;

    QThread worker;

    HttpServer *server;

    UPNPHelper *upnp;

    // list of requests received by server
    RequestListModel *m_requestsModel;

    // list of renderes connected to the server
    MediaRendererModel *m_renderersModel;
};

#endif // APPLICATION_H
