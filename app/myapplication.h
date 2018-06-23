#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QObject>
#include <QSettings>

#include "application.h"
#include "applicationcontroller.h"
#include "applicationworker.h"
#include "mediarenderermodel.h"
#include "debugitem.h"
#include "checknetworklinkitem.h"
#include "upnpcontrolpoint.h"
#include "mediaserver.h"
#include "mediaimageprovider.h"
#include "albumimageprovider.h"
#include "formatimageprovider.h"

class MyApplication : public Application
{
    Q_OBJECT

    Q_PROPERTY(QStringList sharedFolderModel READ sharedFolderModel WRITE setsharedFolderModel NOTIFY sharedFolderModelChanged)
    Q_PROPERTY(ListModel *requestsModel READ requestsModel WRITE setRequestsModel NOTIFY requestsModelChanged)
    Q_PROPERTY(MediaRendererModel *renderersModel READ renderersModel WRITE setRenderersModel NOTIFY renderersModelChanged)
    Q_PROPERTY(ListModel *debugModel READ debugModel NOTIFY debugModelChanged)

    Q_PROPERTY(ListModel *checkNetworkLinkModel READ checkNetworkLinkModel WRITE setNetworkLinkModel NOTIFY checkNetworkLinkModelChanged)

    Q_PROPERTY(QUrl ffmpegFolder READ ffmpegFolder WRITE setFfmpegFolder NOTIFY ffmpegFolderChanged)
    Q_PROPERTY(QString ffmpegVersion READ ffmpegVersion WRITE setffmpegVersion NOTIFY ffmpegVersionChanged)

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

    QUrl ffmpegFolder() const;
    void setFfmpegFolder(const QUrl &folder);

    QString ffmpegVersion() const;
    void setffmpegVersion(const QString &version);

    void setNetworkLinkModel(ListModel *model);
    Q_INVOKABLE void closeCheckLink();

    // Server port
    static const int SERVERPORT;

private:
    QStringList sharedFolderModel()      const { return m_sharedFolderModel; }
    ListModel *requestsModel()           const { return m_requestsModel;     }
    MediaRendererModel *renderersModel() const { return m_renderersModel;    }
    ListModel *debugModel()              const { return m_debugModel; }

    ListModel *checkNetworkLinkModel()   const { return m_checkNetworkLinkModel; }

    bool loadSettings();
    bool saveSettings();

signals:
    void sharedFolderModelChanged();
    void requestsModelChanged();
    void renderersModelChanged();
    void debugModelChanged();
    void checkNetworkLinkModelChanged();
    void ffmpegFolderChanged();
    void ffmpegVersionChanged();

    void scanFolder(QString path);
    void addFolder(QString folder);
    void addLink(QString url);
    void reloadLibrarySignal();

    void updateMediaFromId(const int &id, const QHash<QString, QVariant> &data);


private slots:
    void initializeDatabase();

    void setsharedFolderModel(const QStringList &model) { m_sharedFolderModel = model; emit sharedFolderModelChanged(); }
    void setRequestsModel(ListModel *model);
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

    void advertiseSlot();

    void newRootDevice(UpnpRootDevice *device);
    void newRequest(HttpRequest *request);

    void servingMediaFinished(QString host, QString filename, int status);

private:
    QSettings settings;
    QStringList m_sharedFolderModel;

    ApplicationController m_controller;
    ApplicationWorker *m_worker;

    QNetworkAccessManager netManager;

    UpnpTimer m_timerDiscover;
    UpnpControlPoint m_upnp;
    UpnpRootDevice *m_localrootdevice;

    // list of requests received by server
    ListModel *m_requestsModel;

    // list of renderes connected to the server
    MediaRendererModel *m_renderersModel;

    ListModel *m_debugModel;
    ListModel *m_checkNetworkLinkModel;

    QUrl m_ffmpegFolder;
    QString m_ffmpegVersion;
};

#endif // MYAPPLICATION_H
