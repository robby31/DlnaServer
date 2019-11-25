#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include <QObject>
#include <QDir>
#include "Worker/worker.h"
#include "dlna/cached/dlnacachedrootfolder.h"
#include "mysqldatabase.h"
#include "mynetwork.h"

#include "dlna/dlnanetworkplaylist.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QObject *parent = Q_NULLPTR);

    void initialize();

private:
    void export_media_playlist();
    void _exportMediaTo(DlnaNetworkVideo *media, const QString &filename);

signals:
    // signals used by checkNetworkLink
    void addMessage(QString name, QString message);
    void refresh(const QString &url);

    void initializeSignal();

public slots:
    void export_playlist(const QUrl &url);
    void export_media(const QUrl &url);

    void streamToOpen();
    void logMessage(const QString &message);

private slots:
    void initialisation();
    void scanFolder(const QString &path);
    void checkNetworkLink();
    void scanVolumeInfo();

    void playlistDestroyed(QObject *object);
    void mediaDestroyed(QObject *object);

private:
    DlnaNetworkPlaylist *playlist = Q_NULLPTR;
    DlnaNetworkVideo *media = Q_NULLPTR;
};

#endif // APPLICATIONWORKER_H
