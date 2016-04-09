#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    settings("HOME", "QMS", this),
    m_sharedFolderModel(),
    m_controller(this),
    log(this),
    worker(this),
    server(0),
    m_requestsModel(0),
    m_renderersModel(0)
{
    connect(this, SIGNAL(mainQmlLoaded(QObject*)), this, SLOT(mainQmlLoaded(QObject*)));
    addController("homePageController", &m_controller);

    thread()->setObjectName("QML APPLICATION THREAD");
    worker.setObjectName("WORKER APPLICATION");
    log.setLevel(INF);

    server = new HttpServer(&log);
    server->moveToThread(&worker);
    connect(&worker, SIGNAL(finished()), server, SLOT(deleteLater()));

    setRenderersModel(new MediaRendererModel(this));

    setRequestsModel(new RequestListModel(&log, this));

    qRegisterMetaType<qintptr>("qintptr");

    connect(server, SIGNAL(serverStarted()), this, SLOT(serverStarted()));
    connect(server, SIGNAL(createRequest(qintptr,QString,QString,QString,int)), m_requestsModel, SLOT(createRequest(qintptr,QString,QString,QString,int)));
    connect(m_requestsModel, SIGNAL(newRequest(Request*)), server, SLOT(newRequest(Request*)));
    connect(this, SIGNAL(reloadLibrarySignal()), server, SLOT(reloadLibrary()));
    connect(this, SIGNAL(addFolder(QString)), server, SLOT(_addFolder(QString)));
    connect(server, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(server, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), server, SIGNAL(addNetworkLinkSignal(QString)));
    connect(server, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(server, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

    connect(this, SIGNAL(checkNetworkLink()), server, SIGNAL(checkNetworkLinkSignal()));

    worker.start();

    server->start();
}

MyApplication::~MyApplication()
{

}

void MyApplication::serverStarted()
{
    m_controller.popMessage("Server started");

    // load the settings
    loadSettings();
}

void MyApplication::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    if (server)
    {
        connect(server, SIGNAL(newRenderer(QString,int,QString)), m_renderersModel, SLOT(addRenderer(QString,int,QString)));
        connect(server, SIGNAL(servingRenderer(QString,QString)), m_renderersModel, SLOT(serving(QString,QString)));
        connect(server, SIGNAL(stopServingRenderer(QString)), m_renderersModel, SLOT(stopServing(QString)));
    }
    else
    {
        log.Error("Unable to connect signals/slots between server and renderersModel.");
    }

    emit renderersModelChanged();
}

void MyApplication::setRequestsModel(RequestListModel *model)
{
    if (m_requestsModel)
        m_requestsModel->deleteLater();

    m_requestsModel = model;

    if (server)
    {
        connect(server, SIGNAL(deleteRequest(Request*)), m_requestsModel, SLOT(requestDestroyed(Request*)));
    }
    else
    {
        log.Error("Unable to connect signals/slots between server and requestsModel.");
    }

    emit requestsModelChanged();
}

void MyApplication::mainQmlLoaded(QObject *obj)
{
    // HMI loaded
    connect(obj, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(quit()));
}

void MyApplication::removeFolder(const int &index) {
    if (index >=0 && index < m_sharedFolderModel.size()) {
        m_sharedFolderModel.removeAt(index);
        emit sharedFolderModelChanged();
    }
}

void MyApplication::quit() {
    log.Trace("Quit Application.");

    // save the settings
    saveSettings();

    worker.quit();
    if (!worker.wait(1000))
        log.Error("Unable to stop the worker of the application");
}

void MyApplication::folderAdded(const QString &folder)
{
    if (!m_sharedFolderModel.contains(folder))
    {
        m_sharedFolderModel.append(folder);
        emit sharedFolderModelChanged();
    }
}

void MyApplication::folderNotAdded(const QString &folder)
{
    m_controller.popMessage(QString("Folder not added %1").arg(folder), UiServices::POP_ERROR);
}

void MyApplication::linkAdded(const QString &url)
{
    m_controller.popMessage(QString("network link %1 added successfully.").arg(url));
}

void MyApplication::linkNotAdded(const QString &url)
{
    m_controller.popMessage(QString("network link %1 not added.").arg(url), UiServices::POP_ERROR);
}

bool MyApplication::loadSettings()
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

bool MyApplication::saveSettings()
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
