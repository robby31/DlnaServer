#include "myapplication.h"

const int MyApplication::SERVERPORT = 5002;

MyApplication::MyApplication(int &argc, char **argv):
    Application(argc, argv),
    settings("HOME", "QMS"),
    m_worker(Q_NULLPTR),
    m_upnp(5050),
    m_localrootdevice(Q_NULLPTR),
    m_requestsModel(Q_NULLPTR),
    m_renderersModel(Q_NULLPTR),
    m_debugModel(Q_NULLPTR),
    m_checkNetworkLinkModel(Q_NULLPTR)
{
    addImageProvider("media", new MediaImageProvider);
    addImageProvider("album", new AlbumImageProvider);
    addImageProvider("format", new FormatImageProvider);

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(quit()));

    setFfmpegFolder(QUrl::fromLocalFile(settings.value("ffmpegFolder").toString()));

    addController("homePageController", &m_controller);

    thread()->setObjectName("QML APPLICATION THREAD");

    m_worker = new ApplicationWorker();
    addWorker(&m_controller, m_worker);
    m_worker->initialize();

    connect(this, SIGNAL(scanFolder(QString)), m_worker, SLOT(scanFolder(QString)));

    connect(&m_controller, SIGNAL(checkNetworkLinkSignal()), m_worker, SLOT(checkNetworkLink()));
    connect(m_worker, SIGNAL(addMessage(QString,QString)), this, SLOT(checkNetworkLinkMessage(QString,QString)));

    connect(&m_controller, SIGNAL(scanVolumeInfoSignal()), m_worker, SLOT(scanVolumeInfo()));

    setRenderersModel(new MediaRendererModel(this));

    setRequestsModel(new ListModel(new HttpRequest, this));

    m_debugModel = new ListModel(new DebugItem, this);

    DebugItem *item;
    item = new DebugItem("DlnaResource", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("  DlnaStorageFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaCachedFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaCachedFolderMetaData", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaCachedGroupedFolderMetaData", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaCachedPlaylists", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaNetworkPlaylist", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("    DlnaRootFolder", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaMusicTrackFile", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaVideoFile", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaYoutubeVideo", m_debugModel);
    m_debugModel->appendRow(item);

    item = new DebugItem("DlnaNetworkVideo", m_debugModel);
    m_debugModel->appendRow(item);    

    item = new DebugItem("Device", m_debugModel);
    m_debugModel->appendRow(item);

    connect(&m_upnp, SIGNAL(newRootDevice(UpnpRootDevice*)), this, SLOT(newRootDevice(UpnpRootDevice*)));

    m_upnp.startDiscover();

    qRegisterMetaType<qintptr>("qintptr");

    connect(this, SIGNAL(databaseOpened(QString)), this, SLOT(initializeDatabase()));

    setdatabaseDiverName("QSQLITE");
    setdatabaseConnectionName("MEDIA_DATABASE");

    QString path = settings.value("databasePathName").toString();
    if (!path.isEmpty())
        setdatabaseName(QUrl::fromLocalFile(path).toLocalFile());
}

void MyApplication::initializeDatabase()
{
    QSqlDatabase db = database();

    if (db.isOpen())
    {
        settings.setValue("databasePathName", db.databaseName());
    }

    MediaServer *device = new MediaServer(m_upnp.macAddress(), m_upnp.host().toString(), SERVERPORT, m_renderersModel, this);
    connect(device, SIGNAL(serverStarted()), this, SLOT(serverStarted()));
    connect(device, SIGNAL(serverError(QString)), this, SLOT(serverError(QString)));
    connect(device, SIGNAL(newRequest(HttpRequest*)), this, SLOT(newRequest(HttpRequest*)));
    connect(m_worker, SIGNAL(refresh(QString)), device, SIGNAL(refresh(QString)));

    connect(device, SIGNAL(scanFolder(QString)), m_worker, SLOT(scanFolder(QString)));
    connect(this, SIGNAL(addFolder(QString)), device, SIGNAL(addFolder(QString)));
    connect(device, SIGNAL(folderAdded(QString)), this, SLOT(folderAdded(QString)));
    connect(device, SIGNAL(folderNotAdded(QString)), this, SLOT(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), device, SIGNAL(addLink(QString)));
    connect(device, SIGNAL(linkAdded(QString)), this, SLOT(linkAdded(QString)));
    connect(device, SIGNAL(linkNotAdded(QString)), this, SLOT(linkNotAdded(QString)));

    connect(this, SIGNAL(reloadLibrarySignal()), device, SIGNAL(reloadLibrarySignal()));
    connect(this, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)), device, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)));

    connect(device, SIGNAL(servingFinishedSignal(QString,QString,int)), this, SLOT(servingMediaFinished(QString,QString,int)));

    m_upnp.addLocalRootDevice(device);
    m_localrootdevice = device;
    m_localrootdevice->startServer();
}

void MyApplication::serverStarted()
{
    auto device = qobject_cast<UpnpRootDevice*>(sender());
    qDebug() << "SERVER STARTED" << device->url() << device->host() << device->port();

    m_controller.popMessage("Server started");

    // load the settings
    loadSettings();

    // update volume informations
//    m_controller.scanVolumeInfo();
}

void MyApplication::serverError(const QString &message)
{
    m_controller.popMessage(message, UiServices::POP_ERROR);
}

void MyApplication::setRenderersModel(MediaRendererModel *model)
{
    if (m_renderersModel)
        m_renderersModel->deleteLater();

    m_renderersModel = model;

    emit renderersModelChanged();
}

void MyApplication::setRequestsModel(ListModel *model)
{
    if (m_requestsModel)
        m_requestsModel->deleteLater();

    m_requestsModel = model;

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
        emit scanFolder(path);
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
        setNetworkLinkModel(new ListModel(new CheckNetworkLinkItem, this));
        m_controller.checkNetworkLink();
    }
}

void MyApplication::checkNetworkLinkMessage(const QString &name, const QString &message)
{
    if (m_checkNetworkLinkModel)
    {
        auto item = new CheckNetworkLinkItem(name, message, m_checkNetworkLinkModel);
        m_checkNetworkLinkModel->appendRow(item);
    }
    else
    {
        qCritical() << "ERROR in checkNetworkLinkMessage, model is not initialized.";
    }
}

void MyApplication::closeCheckLink()
{
    setNetworkLinkModel(Q_NULLPTR);
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

void MyApplication::newRootDevice(UpnpRootDevice *device)
{
    if (device->deviceType().startsWith("urn:schemas-upnp-org:device:MediaRenderer"))
        m_renderersModel->addMediaRenderer(device);
}

void MyApplication::newRequest(HttpRequest *request)
{
    qDebug() << "new request" << request;

    connect(request, &HttpRequest::itemChanged, this, &MyApplication::requestDataChanged);
    request->setParent(m_requestsModel);
    m_requestsModel->insertRow(0, request);
}

void MyApplication::servingMediaFinished(const QString &host, const QString &filename, const int &status)
{
    Q_UNUSED(filename)
    Q_UNUSED(status)
    m_renderersModel->serving(host, QString());
}

QUrl MyApplication::ffmpegFolder() const
{
    return m_ffmpegFolder;
}

void MyApplication::setFfmpegFolder(const QUrl &folder)
{
    if (folder.isLocalFile() && folder.isValid())
    {
        FfmpegTranscoding::setDirPath(folder.toLocalFile());

        setffmpegVersion(QFfmpegMedia::getVersion());

        if (!m_ffmpegVersion.isEmpty())
        {
            m_ffmpegFolder = folder.toLocalFile();
            settings.setValue("ffmpegFolder", m_ffmpegFolder);
        }
        else
        {
            m_ffmpegFolder.clear();
        }

        emit ffmpegFolderChanged();
    }
    else
    {
        qCritical() << "unable to set ffmpeg folder, invalid folder" << folder;
    }
}

QString MyApplication::ffmpegVersion() const
{
    return m_ffmpegVersion;
}

void MyApplication::setffmpegVersion(const QString &version)
{
    m_ffmpegVersion = version;
    emit ffmpegVersionChanged();
}

void MyApplication::requestDataChanged(const QVector<int> &roles)
{
    auto request = qobject_cast<HttpRequest*>(sender());

    if (autoRemoveRequest() && request && roles.contains(HttpRequest::statusRole))
    {
        QString status = request->data(HttpRequest::statusRole).toString();
        if (status == "OK")
        {
            QModelIndex index = m_requestsModel->indexFromItem(request);
            if (index.isValid())
            {
                m_requestsModel->removeRow(index.row());
            }
        }
    }
}

void MyApplication::clearRequests()
{
    int index = 0;
    while (index<m_requestsModel->rowCount())
    {
        auto request = qobject_cast<HttpRequest*>(m_requestsModel->at(index));
        if (request)
        {
            if (request->isClosed())
            {
                QModelIndex index = m_requestsModel->indexFromItem(request);
                if (index.isValid())
                {
                    m_requestsModel->removeRow(index.row());
                    continue;
                }
            }
        }

        ++index;
    }
}

bool MyApplication::autoRemoveRequest() const
{
    return m_auto_remove_request;
}

void MyApplication::setAutoRemoveRequest(const bool &flag)
{
    m_auto_remove_request = flag;
    emit autoRemoveRequestChanged();
}

void MyApplication::reload_playlists()
{
    MediaLibrary library;

    // check and reload network playlists
    QSqlQuery query = library.getAllPlaylists();
    if (query.exec())
    {
        while (query.next())
        {
            QSqlRecord elt = query.record();
            qWarning() << "RELOAD playlist" << elt.value("name").toString() << elt.value("url").toString();
            emit addLink(elt.value("url").toString());
        }
    }
}

void MyApplication::reload_network_links()
{
    MediaLibrary library;

    // check and reload network playlists
    QSqlQuery query = library.getAllNetworkLinks();
    if (query.exec())
    {
        while (query.next())
        {
            QSqlRecord elt = query.record();

            if (elt.value("is_reachable").toInt() != 1)
                continue;

            emit addLink(elt.value("filename").toString());
        }
    }
}
