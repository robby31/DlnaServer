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

signals:
    // signals used by checkNetworkLink
    void addMessage(QString name, QString message);
    void refresh(const QString &url);

    void initializeSignal();

public slots:
    void export_playlist(const QUrl &url);
    void export_media(const QUrl &url);

    void streamToOpen();
    void streamFromPlaylistCompleted();
    void streamFromMediaCompleted();
    void logMessage(const QString &message);

private slots:
    void initialisation();
    void scanFolder(const QString &path);
    void checkNetworkLink();
    void scanVolumeInfo();

private:
    DlnaNetworkPlaylist *playlist = Q_NULLPTR;
    int index_media = 0;
};

#endif // APPLICATIONWORKER_H
