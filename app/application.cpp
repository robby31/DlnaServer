#include "application.h"

Application::Application(int &argc, char **argv):
    QApplication(argc, argv),
    settings("HOME", "QMS", this),
    m_sharedFolderModel(),
    engine(this),
    log(this),
    worker(this),
    server(0),
    m_requestsModel(0),
    m_renderersModel(0)
{
    thread()->setObjectName("QML APPLICATION THREAD");
    worker.setObjectName("WORKER APPLICATION");
    log.setLevel(INF);

    server = new HttpServer(&log);
    server->moveToThread(&worker);
    connect(&worker, SIGNAL(finished()), server, SLOT(deleteLater()));

    setRenderersModel(new MediaRendererModel(this));

    setRequestsModel(new RequestListModel(this));

    connect(server, SIGNAL(serverStarted()), this, SLOT(serverStarted()));
    connect(this, SIGNAL(addFolder(QString)), server, SLOT(_addFolder(QString)));
    connect(server, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(server, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), server, SLOT(addNetworkLink(QString)));
    connect(server, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(server, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

    connect(this, SIGNAL(checkNetworkLink()), server, SLOT(_checkNetworkLink()));

    worker.start();

    server->start();
}

void Application::serverStarted()
{
    // load the settings
    loadSettings();
//    emit checkNetworkLink();

//    if (!reloadLibrary())
//        loadSettings();
}

void Application::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    if (server)
    {
        connect(server, SIGNAL(newRenderer(MediaRenderer*)), m_renderersModel, SLOT(addRendererInModel(MediaRenderer*)));
        connect(server, SIGNAL(servingRenderer(QString,QString)), m_renderersModel, SLOT(serving(QString,QString)));
        connect(server, SIGNAL(stopServingRenderer(QString)), m_renderersModel, SLOT(stopServing(QString)));
    }
    else
    {
        log.Error("Unable to connect signals/slots between server and renderersModel.");
    }

    emit renderersModelChanged();
}

void Application::setRequestsModel(RequestListModel *model)
{
    if (m_requestsModel)
        m_requestsModel->deleteLater();

    m_requestsModel = model;

    if (server)
    {
        connect(server, SIGNAL(newRequest(Request*)), m_requestsModel, SLOT(addRequestInModel(Request*)));
    }
    else
    {
        log.Error("Unable to connect signals/slots between server and requestsModel.");
    }

    emit requestsModelChanged();
}

int Application::load(const QUrl &url) {
    engine.rootContext()->setContextProperty("_app",  this);

    engine.load(url);

    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
    if (!window)
    {
        log.Error("Your root item has to be a Window.");
        return -1;
    }
    else
    {
        window->show();
        connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(quit()));
        connect(server, SIGNAL(progressUpdate(int)), window, SIGNAL(progressUpdate(int)));
    }

    return 0;
}

void Application::removeFolder(const int &index) {
    if (index >=0 && index < m_sharedFolderModel.size()) {
        m_sharedFolderModel.removeAt(index);
        emit sharedFolderModelChanged();
    }
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
    if (!m_sharedFolderModel.contains(folder))
    {
        m_sharedFolderModel.append(folder);
        emit sharedFolderModelChanged();
    }
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

bool Application::loadSettings()
{
    // read the settings
    int size = settings.beginReadArray("sharedFolder");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString folder = settings.value("folder").toString();
        emit addFolder(folder);
    }
    settings.endArray();

    return true;
}

bool Application::saveSettings()
{
    // save the settings
    settings.beginWriteArray("sharedFolder");
    int i = 0;
    foreach(QString folder, m_sharedFolderModel) {
        settings.setArrayIndex(i++);
        settings.setValue("folder", folder);
    }
    settings.endArray();

    return true;
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
