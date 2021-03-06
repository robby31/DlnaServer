#ifndef TST_DLNACACHEDRESOURCES_H
#define TST_DLNACACHEDRESOURCES_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "mysqldatabase.h"
#include "dlna/cached/dlnacachedrootfolder.h"
#include "dlna/cached/dlnacachedfolder.h"
#include "Http/httprange.h"

class tst_dlnacachedresources: public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnacachedresources(QObject *parent = Q_NULLPTR);


public slots:
    void error_addFolder(const QString &folder) { folderKO = folder; }

private:
    qint64 parseFolder(const QString &resourceId, DlnaResource *resource);

private Q_SLOTS:
    void init();
    void cleanup();

    void testCase_DlnaCachedRootFolder();
    void testCase_DlnaCachedMusicTrack();
    void testCase_DlnaCachedVideo();

    void testCase_PerformanceAllArtists();
    void testCase_PerformanceAllAlbums();
    void testCase_PerformanceAllGenres();
    void testCase_PerformanceAllTracksByArtist();
    void testCase_PerformanceAllTracksByAlbum();
    void testCase_PerformanceAllTracksByGenre();

    void testCase_Library_NbMedias();
    void testCase_Library_NbAudios();
    void testCase_Library_NbVideos();
    void testCase_Library_NbAlbums();
    void testCase_Library_NbTracksWithAlbum();
    void testCase_Library_NbAlbumPictures();
    void testCase_Library_NbTracksWithPicture();
    void testCase_Library_NbAlbumsWithSeveralPicture();
    void testCase_Library_NbPictureNotUsed();
    void testCase_Library_NbPictureWithNoAlbum();

private:
    QSqlDatabase db;
    QString folderKO;

    Protocol *m_dlnaProfiles = Q_NULLPTR;
};

#endif // TST_DLNACACHEDRESOURCES_H
