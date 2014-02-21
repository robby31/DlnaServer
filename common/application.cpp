#include "application.h"

Application::Application(QQmlApplicationEngine *engine, QObject *parent):
    QObject(parent),
    engine(engine),
    topLevel(engine->rootObjects().value(0))
{
    settings = new QSettings("HOME", "QMS", this);

    server = new HttpServer(&log, &requestsModel, &renderersModel, this);
    upnp = new UPNPHelper(&log, server, this);

    log.setLevel(INF);

    // load the settings
    loadSettings();
}

int Application::load(QUrl url) {
    engine->rootContext()->setContextProperty("_app",  this);
    engine->rootContext()->setContextProperty("sharedFolderModel", QVariant::fromValue(sharedFolderModel));
    engine->rootContext()->setContextProperty("requestsModel", &requestsModel);
    engine->rootContext()->setContextProperty("renderersModel", &renderersModel);

    engine->load(url);

    if (!topLevel)
    {
        topLevel = engine->rootObjects().value(0);

        QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
        if ( !window ) {
            log.ERROR("Your root item has to be a Window.");
            return -1;
        }
        window->show();
        connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(quit()));
        connect(server, SIGNAL(progressUpdate(int)), window, SIGNAL(progressUpdate(int)));
    }

    return 0;
}

void Application::addSharedFolder(QUrl folder) {
    if (folder.isLocalFile() and server->addFolder(folder.toLocalFile())) {
        sharedFolderModel.append(folder.toLocalFile());

        // notifiy the change
        engine->rootContext()->setContextProperty("sharedFolderModel", QVariant::fromValue(sharedFolderModel));
    }
}

void Application::removeFolder(int index) {
    if (index >=0 and index < sharedFolderModel.size()) {
        sharedFolderModel.removeAt(index);

        // notifiy the change
        engine->rootContext()->setContextProperty("sharedFolderModel", QVariant::fromValue(sharedFolderModel));
    }
}

void Application::quit() {
    log.TRACE("Quit Application.");

    // save the settings
    saveSettings();
}

bool Application::loadSettings() {
    // read the settings
    int size = settings->beginReadArray("sharedFolder");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        QString folder = settings->value("folder").toString();
        if (server->addFolder(folder)) {
            sharedFolderModel.append(folder);
        }
    }
    settings->endArray();

    return true;
}

bool Application::saveSettings() {
    // save the settings
    settings->beginWriteArray("sharedFolder");
    int i = 0;
    foreach(QString folder, sharedFolderModel) {
        settings->setArrayIndex(i++);
        settings->setValue("folder", folder);
    }
    settings->endArray();

    return true;
}

