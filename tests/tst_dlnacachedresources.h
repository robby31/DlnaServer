#ifndef TST_DLNACACHEDRESOURCES_H
#define TST_DLNACACHEDRESOURCES_H

#include <QObject>
#include <QtTest>

#include "mysqldatabase.h"
#include "cached/dlnacachedrootfolder.h"
#include "cached/dlnacachedfolder.h"

class tst_dlnacachedresources: public QObject
{
    Q_OBJECT

public:
    explicit tst_dlnacachedresources(QObject *parent = 0);

signals:
    void bytesSent(const qint64 &size, const qint64 &towrite);
    void startTranscoding();

private slots:
    void receivedTranscodedData(const QByteArray &data);
    void error_addFolder(const QString &folder) { folderKO = folder; }

private Q_SLOTS:
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
    TranscodeProcess* transcodeProcess;
    long transcodedSize;
    QSqlDatabase db;
    QString folderKO;

    int parseFolder(QString resourceId, DlnaResource* resource);
};

#endif // TST_DLNACACHEDRESOURCES_H
