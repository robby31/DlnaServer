#include "application.h"

Application::Application(QQmlApplicationEngine *engine, QObject *parent):
    QObject(parent),
    settings(0),
    m_sharedFolderModel(),
    engine(engine),
    topLevel(engine != 0 ? engine->rootObjects().value(0) : 0),
    log(this),
    server(0),
    upnp(0),
    m_requestsModel(0),
    m_renderersModel(0)
{
    log.setLevel(INF);

    setRequestsModel(new RequestListModel(this));
    setRenderersModel(new MediaRendererModel(this));

    settings = new QSettings("HOME", "QMS", this);

    server = new HttpServer(&log, m_requestsModel, m_renderersModel, this);
    upnp = new UPNPHelper(&log, server, this);

    // load the settings
    loadSettings();

//    if (!reloadLibrary())
//        loadSettings();
}

int Application::load(QUrl url) {
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

void Application::addSharedFolder(QUrl folder) {
    if (folder.isLocalFile() && server != 0 && server->addFolder(folder.toLocalFile())) {
        m_sharedFolderModel.append(folder.toLocalFile());

        emit sharedFolderModelChanged();
    }
}

void Application::removeFolder(int index) {
    if (index >=0 && index < m_sharedFolderModel.size()) {
        m_sharedFolderModel.removeAt(index);

        emit sharedFolderModelChanged();
    }
}

void Application::quit() {
    log.Trace("Quit Application.");

    // save the settings
    saveSettings();
}

bool Application::loadSettings() {
    if (settings and server) {
        // read the settings
        int size = settings->beginReadArray("sharedFolder");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            QString folder = settings->value("folder").toString();
            if (server->addFolder(folder)) {
                m_sharedFolderModel.append(folder);
                emit sharedFolderModelChanged();
            }
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
    if (server->resetLibrary()) {
        loadSettings();
        return true;
    } else {
        log.Error("Unable to reload library");
        return false;
    }
}
