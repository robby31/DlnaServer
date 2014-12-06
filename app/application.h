#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>
#include <QApplication>
#include <QtQuick/QQuickView>
#include <QSettings>
#include <QThread>

#include "httpserver.h"
#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"

class Application : public QApplication
{
    Q_OBJECT

    Q_PROPERTY(QStringList sharedFolderModel READ sharedFolderModel WRITE setsharedFolderModel NOTIFY sharedFolderModelChanged)
    Q_PROPERTY(RequestListModel *requestsModel READ requestsModel WRITE setRequestsModel NOTIFY requestsModelChanged)
    Q_PROPERTY(MediaRendererModel *renderersModel READ renderersModel WRITE setRenderersModel NOTIFY renderersModelChanged)

public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();

    // load the qml file describing the HMI
    int load(const QUrl &url);

    Q_INVOKABLE void addSharedFolder(const QUrl &folder) { if (folder.isLocalFile()) emit addFolder(folder.toLocalFile()); }
    Q_INVOKABLE void removeFolder(const int &index);
    Q_INVOKABLE void addNetworkLink(const QString &url)  { emit addLink(url); }


private:
    QStringList sharedFolderModel()      const { return m_sharedFolderModel; }
    RequestListModel *requestsModel()    const { return m_requestsModel;     }
    MediaRendererModel *renderersModel() const { return m_renderersModel;    }

    bool loadSettings();
    bool saveSettings();

    bool reloadLibrary();


signals:
    void sharedFolderModelChanged();
    void requestsModelChanged();
    void renderersModelChanged();

    void addFolder(QString folder);
    void addLink(QString url);
    void checkNetworkLink();


private slots:
    void setsharedFolderModel(const QStringList &model) { m_sharedFolderModel = model; emit sharedFolderModelChanged(); }
    void setRequestsModel(RequestListModel *model);
    void setRenderersModel(MediaRendererModel *model);

    void folderAdded(const QString &folder);
    void folderNotAdded(const QString &folder);

    void linkAdded(const QString &url);
    void linkNotAdded(const QString &url);

    // quit the application
    void quit();

    void serverStarted();

private:
    QSettings settings;
    QStringList m_sharedFolderModel;

    QQmlApplicationEngine engine;

    Logger log;

    QThread worker;

    HttpServer *server;

    // list of requests received by server
    RequestListModel *m_requestsModel;

    // list of renderes connected to the server
    MediaRendererModel *m_renderersModel;
};

#endif // APPLICATION_H
