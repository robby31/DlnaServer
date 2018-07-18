#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include "upnprootdevice.h"
#include "Services/serviceconnectionmanager.h"
#include "mediaservercontent.h"

class MediaServer : public UpnpRootDevice
{
    Q_OBJECT

public:
    explicit MediaServer(QNetworkAccessManager *nam, QString macAddress, QString host, int port, MediaRendererModel *renderersModel, QObject *parent = 0);

    virtual void replyGetIcon(HttpRequest *request);

private:
    void initDescription();
    void initConnectionManager();
    void initContentDirectory();

signals:
    void refresh(const QString &url);

    void scanFolder(QString path);
    void addFolder(QString folder);
    void folderAdded(const QString &folder);
    void folderNotAdded(const QString &folder);

    void addLink(QString url);
    void linkAdded(const QString &url);
    void linkNotAdded(const QString &url);

    void reloadLibrarySignal();
    void updateMediaFromId(const int &id, const QHash<QString, QVariant> &data);

    void servingFinishedSignal(QString host, QString filename, int status);

private:
    MediaRendererModel *m_renderersModel = Q_NULLPTR;
};

#endif // MEDIASERVER_H