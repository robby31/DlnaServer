#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    settings("HOME", "QMS", this),
    m_sharedFolderModel(),
    m_controller(this),
    log(this),
    netManager(this),
    server(&log, backendThread(), &netManager),
    m_requestsModel(0),
    m_renderersModel(0),
    m_debugModel(0)
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(quit()));

    QFfmpegProcess::setDirPath("/opt/local/bin");
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    addController("homePageController", &m_controller);

    thread()->setObjectName("QML APPLICATION THREAD");
    log.setLevel(INF);

    setRenderersModel(new MediaRendererModel(this));

    setRequestsModel(new RequestListModel(&log, this));

    m_debugModel = new ListModel(new DebugItem, this);

    DebugItem *item;
    item = new DebugItem("DlnaResource", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaMusicTrackFile", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaRootFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaVideoFile", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaYoutubeVideo", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("HttpClient", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("Request", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("Reply", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("Device", m_debugModel);
    m_debugModel->appendRow(item);



    qRegisterMetaType<qintptr>("qintptr");

    connect(&server, SIGNAL(serverStarted()), this, SLOT(serverStarted()));
    connect(&server, SIGNAL(createRequest(qintptr,QString,QString,QString,int)), m_requestsModel, SLOT(createRequest(qintptr,QString,QString,QString,int)));
    connect(m_requestsModel, SIGNAL(newRequest(Request*)), &server, SLOT(newRequest(Request*)));
    connect(this, SIGNAL(reloadLibrarySignal()), &server, SLOT(reloadLibrary()));
    connect(this, SIGNAL(addFolder(QString)), &server, SLOT(_addFolder(QString)));
    connect(&server, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(&server, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), &server, SIGNAL(addNetworkLinkSignal(QString)));
    connect(&server, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(&server, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

    server.start();
}

void MyApplication::serverStarted()
{
    m_controller.popMessage("Server started");

    // load the settings
    loadSettings();

    // update volume informations
    UpdateMediaVolumeInfo *volumeInfoWorker = new UpdateMediaVolumeInfo(&log, &netManager);
    QThreadPool::globalInstance()->start(volumeInfoWorker);
}

void MyApplication::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    connect(&server, SIGNAL(newRenderer(QString,int,QString)), m_renderersModel, SLOT(addRenderer(QString,int,QString)));
    connect(&server, SIGNAL(servingRenderer(QString,QString)), m_renderersModel, SLOT(serving(QString,QString)));
    connect(&server, SIGNAL(stopServingRenderer(QString)), m_renderersModel, SLOT(stopServing(QString)));

    emit renderersModelChanged();
}

void MyApplication::setRequestsModel(RequestListModel *model)
{
    if (m_requestsModel)
        m_requestsModel->deleteLater();

    m_requestsModel = model;

    connect(&server, SIGNAL(deleteRequest(Request*)), m_requestsModel, SLOT(requestDestroyed(Request*)));

    emit requestsModelChanged();
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

void MyApplication::startCheckNetworkLink()
{
    // check all network links
    CheckNetworkLink *checknetworklinkWorker = new CheckNetworkLink(&log, &netManager);
    QThreadPool::globalInstance()->start(checknetworklinkWorker);
}
