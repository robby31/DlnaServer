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
    m_debugModel(0),
    m_checkNetworkLinkModel(0),
    m_checkInProgress(-1)
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(quit()));

    // create database in backend Thread
    CreateDatabaseThread *dbBackend = new CreateDatabaseThread();
    dbBackend->moveToThread(backendThread());
    connect(backendThread(), SIGNAL(finished()), dbBackend, SLOT(deleteLater()));
    dbBackend->run();

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
    connect(&server, SIGNAL(createRequest(HttpClient*,QString,QString,QString,int)), m_requestsModel, SLOT(createRequest(HttpClient*,QString,QString,QString,int)));
    connect(m_requestsModel, SIGNAL(newRequest(Request*)), &server, SLOT(newRequest(Request*)));
    connect(this, SIGNAL(reloadLibrarySignal()), &server, SLOT(reloadLibrary()));
    connect(this, SIGNAL(addFolder(QString)), &server, SLOT(_addFolder(QString)));
    connect(&server, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(&server, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), &server, SIGNAL(addNetworkLinkSignal(QString)));
    connect(&server, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(&server, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

    connect(this, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)), &server, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)));

    server.start();
}

void MyApplication::serverStarted()
{
    m_controller.popMessage("Server started");

    // load the settings
    loadSettings();

    // update volume informations
//    UpdateMediaVolumeInfo *volumeInfoWorker = new UpdateMediaVolumeInfo(&netManager);
//    QThreadPool::globalInstance()->start(volumeInfoWorker);
}

void MyApplication::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    connect(&server, SIGNAL(newMediaRenderer(UpnpRootDevice*)), m_renderersModel, SLOT(addMediaRenderer(UpnpRootDevice*)));
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

void MyApplication::refreshFolder(const int &index)
{
    if (index >=0 && index < m_sharedFolderModel.size())
    {
        QString path = m_sharedFolderModel.at(index);

        // scan the folder in background
        CachedRootFolderReadDirectory *readDirectoryWorker = new CachedRootFolderReadDirectory(QDir(path));
        QThreadPool::globalInstance()->start(readDirectoryWorker);
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
    if (m_checkNetworkLinkModel)
    {
        qCritical() << "check network link already in progress.";
    }
    else
    {
        // check all network links
        CheckNetworkLink *checknetworklinkWorker = new CheckNetworkLink(&log, &netManager);
        connect(checknetworklinkWorker, SIGNAL(addMessage(QString,QString)), this, SLOT(checkNetworkLinkMessage(QString, QString)));
        connect(checknetworklinkWorker, SIGNAL(progress(int)), this, SLOT(checkNetworkLinkProgress(int)));
        connect(this, SIGNAL(abortCheckNetworkLink()), checknetworklinkWorker, SLOT(abort()));
        connect(this, SIGNAL(aboutToQuit()), checknetworklinkWorker, SLOT(abort()));

        setNetworkLinkModel(new ListModel(new CheckNetworkLinkItem, this));

        QThreadPool::globalInstance()->start(checknetworklinkWorker);
    }
}

void MyApplication::removeMedia(const int &id)
{
    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");

    db.transaction();

    QSqlQuery query(db);

    if (query.prepare("DELETE FROM param_value WHERE media=:id"))
    {
        query.bindValue(":id", id);
        if (!query.exec())
        {
            log.Error(QString("unable to remove param values for media(%1) : %2.").arg(id).arg(query.lastError().text()));
            if (!db.rollback())
                qCritical() << "unable to rollback" << db.lastError().text();
        }
        else
        {
            // remove media
            if (query.prepare("DELETE FROM media WHERE id=:id"))
            {
                query.bindValue(":id", id);
                if (!query.exec())
                {
                    log.Error(QString("unable to remove media(%1) : %2.").arg(id).arg(query.lastError().text()));
                    if (!db.rollback())
                        qCritical() << "unable to rollback" << db.lastError().text();
                }
                else
                {
                    if (!db.commit())
                        qCritical() << "unable to commit" << db.lastError().text();
                }
            }
            else
            {
                if (!db.rollback())
                    qCritical() << "unable to rollback" << db.lastError().text();
            }
        }
    }
    else
    {
        if (!db.rollback())
            qCritical() << "unable to rollback" << db.lastError().text();
    }
}

void MyApplication::checkNetworkLinkMessage(QString name, QString message)
{
    if (m_checkNetworkLinkModel)
    {
        CheckNetworkLinkItem *item = new CheckNetworkLinkItem(name, message, m_checkNetworkLinkModel);
        m_checkNetworkLinkModel->appendRow(item);
    }
    else
    {
        qCritical() << "ERROR in checkNetworkLinkMessage, model is not initialized.";
    }
}

void MyApplication::checkNetworkLinkProgress(const int &value)
{
    setcheckInProgress(value);
}

void MyApplication::setcheckInProgress(const int &value)
{
    if (m_checkInProgress != value)
    {
        m_checkInProgress = value;
        emit checkInProgressChanged();
    }
}

void MyApplication::abortCheckLink()
{
    emit abortCheckNetworkLink();
}

void MyApplication::closeCheckLink()
{
    if (m_checkInProgress != -1)
    {
        qCritical() << "unable to close check links, it is still in progress.";
    }
    else
    {
        setNetworkLinkModel(0);
    }
}


void MyApplication::setNetworkLinkModel(ListModel *model)
{
    if (m_checkNetworkLinkModel != model)
    {
        if (m_checkNetworkLinkModel)
            m_checkNetworkLinkModel->deleteLater();

        m_checkNetworkLinkModel = model;
        emit checkNetworkLinkModelChanged();
    }
}

void MyApplication::updateFilenameMedia(const int &id, const QString &pathname)
{
    QHash<QString,QVariant> data;
    data["filename"] = pathname;
    emit updateMediaFromId(id, data);
}
