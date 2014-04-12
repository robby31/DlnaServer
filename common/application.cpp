#include "application.h"

Application::Application(QQmlApplicationEngine *engine, QObject *parent):
    QObject(parent),
    settings(0),
    sharedFolderModel(),
    engine(engine),
    topLevel(engine != 0 ? engine->rootObjects().value(0) : 0),
    log(this),
    server(0),
    upnp(0),
    requestsModel(this),
    renderersModel(this)
{
    settings = new QSettings("HOME", "QMS", this);

    server = new HttpServer(&log, &requestsModel, &renderersModel, this);
    upnp = new UPNPHelper(&log, server, this);

    log.setLevel(INF);

    // load the settings
    loadSettings();
}

int Application::load(QUrl url) {
    if (engine != 0) {
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
    if (folder.isLocalFile() and server != 0 and server->addFolder(folder.toLocalFile())) {
        sharedFolderModel.append(folder.toLocalFile());

        if (engine) {
            // notifiy the change
            engine->rootContext()->setContextProperty("sharedFolderModel", QVariant::fromValue(sharedFolderModel));
        }
    }
}

void Application::removeFolder(int index) {
    if (index >=0 and index < sharedFolderModel.size()) {
        sharedFolderModel.removeAt(index);

        if (engine) {
            // notifiy the change
            engine->rootContext()->setContextProperty("sharedFolderModel", QVariant::fromValue(sharedFolderModel));
        }
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
                sharedFolderModel.append(folder);
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
        foreach(QString folder, sharedFolderModel) {
            settings->setArrayIndex(i++);
            settings->setValue("folder", folder);
        }
        settings->endArray();

        return true;
    }

    return false;
}

