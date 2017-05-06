#include "myapplication.h"

const int MyApplication::SERVERPORT = 5002;

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    settings("HOME", "QMS"),
    m_sharedFolderModel(),
    m_controller(),
    netManager(),
    m_timerDiscover(3, 600000),
    m_upnp(5050),
    m_requestsModel(Q_NULLPTR),
    m_renderersModel(Q_NULLPTR),
    m_debugModel(Q_NULLPTR),
    m_checkNetworkLinkModel(Q_NULLPTR),
    m_checkInProgress(-1),
    m_connectionManager(Q_NULLPTR),
    m_contentDirectory(Q_NULLPTR)
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(quit()));

    // create database in backend Thread
//    CreateDatabaseThread *dbBackend = new CreateDatabaseThread();
//    dbBackend->moveToThread(backendThread());
//    connect(backendThread(), SIGNAL(finished()), dbBackend, SLOT(deleteLater()));
//    dbBackend->run();

    QFfmpegProcess::setDirPath("/opt/local/bin");
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    addController("homePageController", &m_controller);

    thread()->setObjectName("QML APPLICATION THREAD");

    setRenderersModel(new MediaRendererModel(this));

    setRequestsModel(new RequestListModel(this));

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

    item = new DebugItem("Device", m_debugModel);
    m_debugModel->appendRow(item);

    m_upnp.setNetworkManager(&netManager);
    connect(&m_upnp, SIGNAL(newRootDevice(UpnpRootDevice*)), this, SLOT(newRootDevice(UpnpRootDevice*)));

    UpnpRootDevice *device = m_upnp.addLocalRootDevice(SERVERPORT, m_upnp.generateUuid(), "/description/fetch");
    connect(device, SIGNAL(serverStarted()), this, SLOT(serverStarted()));
    connect(device, SIGNAL(serverError(QString)), this, SLOT(serverError(QString)));
    connect(device, SIGNAL(newRequest(HttpRequest*)), this, SLOT(newRequest(HttpRequest*)));
    connect(device, SIGNAL(requestCompleted(HttpRequest*)), this, SLOT(requestCompleted(HttpRequest*)));
    device->startServer();

    connect(&m_timerDiscover, SIGNAL(timeout()), this, SLOT(advertiseSlot()));
    m_timerDiscover.start(2000);

    qRegisterMetaType<qintptr>("qintptr");
}

MyApplication::~MyApplication()
{
    QThreadPool::globalInstance()->waitForDone();
}

void MyApplication::serverStarted()
{
    UpnpRootDevice *device = qobject_cast<UpnpRootDevice*>(sender());
    qDebug() << "SERVER STARTED" << device->url() << device->host() << device->port();

    m_controller.popMessage("Server started");

    if (!m_connectionManager)
    {
        m_connectionManager = new ServiceConnectionManager(this);
        connect(m_connectionManager, SIGNAL(destroyed(QObject*)), this, SLOT(connectionManagerDestroyed(QObject*)));
    }
    else
    {
        qCritical() << "service connection manager already initialised.";
    }

    if (!m_contentDirectory)
    {
        m_contentDirectory = new ServiceContentDirectory(device->host().toString(), device->port(), this);
        connect(m_contentDirectory, SIGNAL(destroyed(QObject*)), this, SLOT(contentDirectoryDestroyed(QObject*)));

        connect(m_renderersModel, SIGNAL(mediaRendererDestroyed(QString)), m_contentDirectory, SLOT(mediaRendererDestroyed(QString)));

        connect(this, SIGNAL(addFolder(QString)), m_contentDirectory, SLOT(_addFolder(QString)));
        connect(m_contentDirectory, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
        connect(m_contentDirectory, SIGNAL(error_addFolder(QString)), this, SLOT(folderNotAdded(QString)));

        connect(this, SIGNAL(addLink(QString)), m_contentDirectory, SIGNAL(addNetworkLinkSignal(QString)));
        connect(m_contentDirectory, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
        connect(m_contentDirectory, SIGNAL(error_addNetworkLink(QString)), this, SLOT(linkNotAdded(QString)));

        connect(this, SIGNAL(reloadLibrarySignal()), m_contentDirectory, SLOT(reloadLibrary()));
        connect(this, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)), m_contentDirectory, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)));

        connect(m_contentDirectory, SIGNAL(servingRendererSignal(QString,QString)), m_renderersModel, SLOT(serving(QString,QString)));
        connect(m_contentDirectory, SIGNAL(servingFinishedSignal(QString,QString,int)), this, SLOT(servingMediaFinished(QString,QString,int)));

        m_contentDirectory->setNetworkAccessManager(&netManager);
    }
    else
    {
        qCritical() << "service content directory already initialised.";
    }

    // load the settings
    loadSettings();

    // update volume informations
//    UpdateMediaVolumeInfo *volumeInfoWorker = new UpdateMediaVolumeInfo(&netManager);
//    QThreadPool::globalInstance()->start(volumeInfoWorker);
}

void MyApplication::serverError(const QString &message)
{
    m_controller.popMessage(message, UiServices::POP_ERROR);
}

void MyApplication::contentDirectoryDestroyed(QObject *obj)
{
    Q_UNUSED(obj)
    m_contentDirectory = 0;
}

void MyApplication::connectionManagerDestroyed(QObject *obj)
{
    Q_UNUSED(obj)
    m_connectionManager = 0;
}

void MyApplication::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    emit renderersModelChanged();
}

void MyApplication::setRequestsModel(RequestListModel *model)
{
    if (m_requestsModel)
        m_requestsModel->deleteLater();

    m_requestsModel = model;

//    connect(&server, SIGNAL(deleteRequest(Request*)), m_requestsModel, SLOT(requestDestroyed(Request*)));

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
    qDebug() << "Quit Application.";

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
        CheckNetworkLink *checknetworklinkWorker = new CheckNetworkLink(&netManager);
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
            qCritical() << QString("unable to remove param values for media(%1) : %2.").arg(id).arg(query.lastError().text());
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
                    qCritical() << QString("unable to remove media(%1) : %2.").arg(id).arg(query.lastError().text());
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

void MyApplication::advertiseSlot()
{
    qDebug() << "advertise UPNP root device";
    m_upnp.sendDiscover(UpnpRootDevice::UPNP_ROOTDEVICE);
}

void MyApplication::newRootDevice(UpnpRootDevice *device)
{
    if (device->deviceType() == "urn:schemas-upnp-org:device:MediaRenderer:1")
        m_renderersModel->addMediaRenderer(device);
}

void MyApplication::newRequest(HttpRequest *request)
{
    qDebug() << "new request" << request;

    request->setParent(m_requestsModel);
    m_requestsModel->insertRow(0, request);
}

void MyApplication::requestCompleted(HttpRequest *request)
{
    qDebug() << "request completed, ready for reply" << request;

    UpnpRootDevice *device = qobject_cast<UpnpRootDevice *>(sender());

    if (device)
    {
        request->setServerName(m_upnp.serverName());

        if (request->operation() == QNetworkAccessManager::GetOperation && request->url().toString() == "/description/fetch")
        {
            QFile inputStream(":/PMS.xml");
            if (inputStream.open(QFile::ReadOnly | QFile::Text)) {
                // read file PMS.xml
                QString data = inputStream.readAll();
                inputStream.close();

                // replace parameter by its value
                data.replace(QString("[uuid]"), QString("uuid:%1").arg(device->uuid()));
                data.replace(QString("[host]"), m_upnp.host().toString());
                data.replace(QString("[port]"), QString("%1").arg(SERVERPORT));

                request->replyData(data.toUtf8());
            }
            else {
                qCritical() << "Unable to read PMS.xml for description/fetch answer.";
            }
        }
        else if (request->operation() == QNetworkAccessManager::GetOperation && request->url().toString() == "/UPnP_AV_ConnectionManager_1.0.xml")
        {
            request->replyFile(":/UPnP_AV_ConnectionManager_1.0.xml");
        }
        else if (request->operation() == QNetworkAccessManager::GetOperation && request->url().toString() == "/UPnP_AV_ContentDirectory_1.0.xml")
        {
            request->replyFile(":/UPnP_AV_ContentDirectory_1.0.xml");
        }
        else if (request->operation() == QNetworkAccessManager::GetOperation && request->url().toString().startsWith("/images/"))
        {
            request->replyFile(":" + request->url().toString());
        }
        else if (request->url().toString().startsWith("/get/"))
        {
            m_contentDirectory->reply(request);
        }
        else if (request->url().toString() == "/upnp/control/content_directory")
        {
            m_contentDirectory->reply(request);
        }
        else if (request->url().toString() == "/upnp/event/content_directory")
        {
            m_contentDirectory->reply(request);
        }
        else if (request->url().toString() == "/upnp/control/connection_manager")
        {
            m_connectionManager->reply(request);
        }
        else if (request->url().toString() == "/upnp/event/connection_manager")
        {
            m_connectionManager->reply(request);
        }
        else
        {
            qCritical() << "unknwon request to reply" << request->operationString() << request->url().toString();
        }
    }
    else
    {
        qCritical() << "invalid device" << device << sender();
    }
}

void MyApplication::servingMediaFinished(QString host, QString filename, int status)
{
    Q_UNUSED(filename)
    Q_UNUSED(status)
    m_renderersModel->serving(host, QString());
}
