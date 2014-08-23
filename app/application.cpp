#include "application.h"

Application::Application(QQmlApplicationEngine *engine, QObject *parent):
    QObject(parent),
    settings(0),
    m_sharedFolderModel(),
    engine(engine),
    topLevel(engine != 0 ? engine->rootObjects().value(0) : 0),
    log(this),
    worker(this),
    server(0),
    upnp(0),
    m_requestsModel(0),
    m_renderersModel(0)
{
    log.setLevel(INF);

    setRequestsModel(new RequestListModel(this));
    setRenderersModel(new MediaRendererModel(this));

    settings = new QSettings("HOME", "QMS", this);

    server = new HttpServer(&log, m_requestsModel, m_renderersModel);
    server->moveToThread(&worker);
    connect(&worker, SIGNAL(finished()), server, SLOT(deleteLater()));

    upnp = new UPNPHelper(&log, server);
    upnp->moveToThread(&worker);
    connect(&worker, SIGNAL(finished()), upnp, SLOT(deleteLater()));

    connect(this, SIGNAL(addFolder(QString)), server, SLOT(addFolder(QString)));
    connect(server, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(server, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), server, SLOT(addNetworkLink(QString)));
    connect(server, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(server, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

    worker.start();

    // load the settings
    loadSettings();

//    if (!reloadLibrary())
//        loadSettings();
}

int Application::load(const QUrl &url) {
    if (engine != 0) {
        engine->rootContext()->setContextProperty("_app",  this);

        engine->load(url);

        if (!topLevel)
        {
            topLevel = engine->rootObjects().value(0);

            QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
            if ( !window ) {
                log.Error("Your root item has to be a Window.");
                return -1;
            } else {
                window->show();
                connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(quit()));
                connect(server, SIGNAL(progressUpdate(int)), window, SIGNAL(progressUpdate(int)));
            }
        }

        return 0;
    } else {
        return -2;
    }
}

void Application::addSharedFolder(const QUrl &folder) {
    if (folder.isLocalFile())
        emit addFolder(folder.toLocalFile());
}

void Application::removeFolder(const int &index) {
    if (index >=0 && index < m_sharedFolderModel.size()) {
        m_sharedFolderModel.removeAt(index);

        emit sharedFolderModelChanged();
    }
}

void Application::addNetworkLink(const QString &url)
{
    emit addLink(url);
}

void Application::quit() {
    log.Trace("Quit Application.");

    // save the settings
    saveSettings();

    worker.quit();
    if (!worker.wait(1000))
        log.Error("Unable to stop the worker of the application");
}

void Application::folderAdded(const QString &folder)
{
    m_sharedFolderModel.append(folder);
    emit sharedFolderModelChanged();
}

void Application::folderNotAdded(const QString &folder)
{
    log.Error(QString("Folder not added %1").arg(folder));
}

void Application::linkAdded(const QString &url)
{
    log.Info(QString("network link %1 added successfully.").arg(url));
}

void Application::linkNotAdded(const QString &url)
{
    log.Error(QString("network link %1 not added.").arg(url));
}

bool Application::loadSettings() {
    if (settings and server) {
        // read the settings
        int size = settings->beginReadArray("sharedFolder");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            QString folder = settings->value("folder").toString();
            emit addFolder(folder);
        }
        settings->endArray();

        return true;
    }

    return false;
}

bool Application::saveSettings() {
    if (settings) {
        // save the settings
        settings->beginWriteArray("sharedFolder");
        int i = 0;
        foreach(QString folder, m_sharedFolderModel) {
            settings->setArrayIndex(i++);
            settings->setValue("folder", folder);
        }
        settings->endArray();

        return true;
    }

    return false;
}

bool Application::reloadLibrary()
{
    // save network media
    QList<QString> networkMedia;
    QSqlQuery query;
    if (query.exec("SELECT filename from media WHERE filename like 'http%'")) {
        while (query.next())
            networkMedia.append(query.value("filename").toString());
    } else {
        log.Error(QString("Unable to load network media: %1").arg(query.lastError().text()));
    }

    if (server->resetLibrary()) {
       bool res = true;

        // load network media
        foreach (const QString &url, networkMedia)
            if (!server->addNetworkLink(url))
                res = false;

        return loadSettings() && res;
    } else {
        log.Error("Unable to reload library");
        return false;
    }
}
