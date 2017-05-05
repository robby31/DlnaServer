#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QObject>
#include <QSettings>
#include <QThread>
#include <QThreadPool>

#include "application.h"
#include "applicationcontroller.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"
#include "updatemediavolumeinfo.h"
#include "debugitem.h"
#include "checknetworklink.h"
#include "createdatabasethread.h"
#include "checknetworklinkitem.h"
#include "upnpcontrolpoint.h"
#include "cached/cachedrootfolderreaddirectory.h"
#include "servicecontentdirectory.h"
#include "serviceconnectionmanager.h"

class MyApplication : public Application
{
    Q_OBJECT

    Q_PROPERTY(QStringList sharedFolderModel READ sharedFolderModel WRITE setsharedFolderModel NOTIFY sharedFolderModelChanged)
    Q_PROPERTY(RequestListModel *requestsModel READ requestsModel WRITE setRequestsModel NOTIFY requestsModelChanged)
    Q_PROPERTY(MediaRendererModel *renderersModel READ renderersModel WRITE setRenderersModel NOTIFY renderersModelChanged)
    Q_PROPERTY(ListModel *debugModel READ debugModel NOTIFY debugModelChanged)

    Q_PROPERTY(ListModel *checkNetworkLinkModel READ checkNetworkLinkModel WRITE setNetworkLinkModel NOTIFY checkNetworkLinkModelChanged)
    Q_PROPERTY(int checkInProgress READ checkInProgress WRITE setcheckInProgress NOTIFY checkInProgressChanged)

public:
    explicit MyApplication(int &argc, char **argv);
    virtual ~MyApplication() Q_DECL_OVERRIDE;

    Q_INVOKABLE void addSharedFolder(const QUrl &folder) { if (folder.isLocalFile()) emit addFolder(folder.toLocalFile()); }
    Q_INVOKABLE void removeFolder(const int &index);
    Q_INVOKABLE void refreshFolder(const int &index);
    Q_INVOKABLE void addNetworkLink(const QString &url)  { emit addLink(url); }
    Q_INVOKABLE void startCheckNetworkLink();
    Q_INVOKABLE void reloadLibrary() { emit reloadLibrarySignal(); }
    Q_INVOKABLE void removeMedia(const int &id);
    Q_INVOKABLE void updateFilenameMedia(const int &id, const QString &pathname);

    void setNetworkLinkModel(ListModel *model);
    void setcheckInProgress(const int &value);
    Q_INVOKABLE void abortCheckLink();
    Q_INVOKABLE void closeCheckLink();

    // Server port
    static const int SERVERPORT;

private:
    QStringList sharedFolderModel()      const { return m_sharedFolderModel; }
    RequestListModel *requestsModel()    const { return m_requestsModel;     }
    MediaRendererModel *renderersModel() const { return m_renderersModel;    }
    ListModel *debugModel()              const { return m_debugModel; }

    ListModel *checkNetworkLinkModel()   const { return m_checkNetworkLinkModel; }
    int checkInProgress()                const { return m_checkInProgress; }

    bool loadSettings();
    bool saveSettings();

signals:
    void sharedFolderModelChanged();
    void requestsModelChanged();
    void renderersModelChanged();
    void debugModelChanged();
    void checkNetworkLinkModelChanged();
    void checkInProgressChanged();

    void addFolder(QString folder);
    void addLink(QString url);
    void reloadLibrarySignal();

    void abortCheckNetworkLink();

    void updateMediaFromId(const int &id, const QHash<QString, QVariant> &data);


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
    void serverError(const QString &message);

    void checkNetworkLinkMessage(QString name, QString message);
    void checkNetworkLinkProgress(const int &value);

    void advertiseSlot();

    void newRootDevice(UpnpRootDevice *device);
    void newRequest(HttpRequest *request);
    void requestCompleted(HttpRequest *request);

    void contentDirectoryDestroyed(QObject *obj);
    void connectionManagerDestroyed(QObject *obj);

    void servingMediaFinished(QString host, QString filename, int status);

private:
    QSettings settings;
    QStringList m_sharedFolderModel;

    ApplicationController m_controller;

    QNetworkAccessManager netManager;

    UpnpTimer m_timerDiscover;
    UpnpControlPoint m_upnp;

    // list of requests received by server
    RequestListModel *m_requestsModel;

    // list of renderes connected to the server
    MediaRendererModel *m_renderersModel;

    ListModel *m_debugModel;
    ListModel *m_checkNetworkLinkModel;
    int m_checkInProgress;

    ServiceConnectionManager *m_connectionManager;
    ServiceContentDirectory *m_contentDirectory;
};

#endif // MYAPPLICATION_H
