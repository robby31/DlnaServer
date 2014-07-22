#ifndef DLNACACHEDROOTFOLDER_H
#define DLNACACHEDROOTFOLDER_H

#include "dlnarootfolder.h"

#include <qmimedatabase.h>
#include "medialibrary.h"

#include "dlnamusictrackfile.h"
#include "dlnavideofile.h"
#include "dlnacachedfolder.h"
#include "dlnacachedmusicfolder.h"
#include "dlnarootfolder.h"
#include "dlnayoutubemusic.h"
#include "dlnayoutubevideo.h"

class DlnaCachedRootFolder : public DlnaRootFolder
{

public:
    explicit DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent = 0);

    // returns true if the folder is added to Root.
    virtual bool addFolder(QString path);

    bool addNetworkLink(const QString url) { return addResource(QUrl(url)); }

    DlnaRootFolder* getRootFolder() { return &rootFolder; }

    void addResource(QFileInfo fileinfo);
    bool addResource(QUrl url);

    QSqlDatabase *getDatabase() const { return library.getDatabase(); }
    bool updateLibrary(const QString &filename, const QHash<QString, QVariant> &data);
    bool incrementCounterPlayed(const QString &filename);
    void refreshLastAddedMedia() { lastAddedChild->needRefresh(); }

    bool resetLibrary();

private:
    void readDirectory(QDir folder);

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
