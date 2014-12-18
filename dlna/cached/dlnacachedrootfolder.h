#ifndef DLNACACHEDROOTFOLDER_H
#define DLNACACHEDROOTFOLDER_H

#include "dlnarootfolder.h"

#include <qmimedatabase.h>
#include "medialibrary.h"

#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include "dlnacachedfolder.h"
#include "dlnacachedmusicfolder.h"
#include "dlnacachedgroupedfoldermetadata.h"
#include "dlnarootfolder.h"
#include "dlnayoutubemusic.h"
#include "dlnayoutubevideo.h"

class DlnaCachedRootFolder : public DlnaRootFolder
{
    Q_OBJECT

public:
    explicit DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent = 0);

    void resetLibrary() { emit resetLibrarySignal(); }

private:
    void readDirectory(QDir folder);

    void addResource(QFileInfo fileinfo);
    bool addResource(QUrl url);

    QSqlQuery getAllNetworkLinks() { return library.getAllNetworkLinks(); }
    bool networkLinkIsValid(const QString &url);

signals:
    void linkAdded(QString url);
    void error_addNetworkLink(QString url);

    void resetLibrarySignal();

private slots:
    bool resetLibrarySlot();

    void updateLibrary(const QString &filename, const QHash<QString, QVariant> &data);
    void incrementCounterPlayed(const QString &filename);

    // returns true if the folder is added to Root.
    virtual bool addFolderSlot(QString path);

    bool addNetworkLink(const QString &url);
    void checkNetworkLink();

private:
    MediaLibrary library;
    QMimeDatabase mimeDb;
    DlnaRootFolder rootFolder;
    DlnaCachedFolder *recentlyPlayedChild;
    DlnaCachedFolder *resumeChild;
    DlnaCachedFolder *favoritesChild;
    DlnaCachedFolder *lastAddedChild;
};

#endif // DLNACACHEDROOTFOLDER_H
