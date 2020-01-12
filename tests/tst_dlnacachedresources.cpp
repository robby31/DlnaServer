#include "tst_dlnacachedresources.h"

tst_dlnacachedresources::tst_dlnacachedresources(QObject *parent) :
    DlnaCheckFunctions(parent),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE"))
{
    FfmpegTranscoding::setDirPath("/usr/local/bin");

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
    db.open();
}

void tst_dlnacachedresources::init()
{
    m_dlnaProfiles = new Protocol("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml");
}

void tst_dlnacachedresources::cleanup()
{
    delete m_dlnaProfiles;

    DebugInfo::display_alive_objects();

    QCOMPARE(DebugInfo::count_alive_objects(), 0);
}

void tst_dlnacachedresources::testCase_Library_NbMedias()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbMedias = 0;
    if (query.exec("SELECT id from media")) {
        if (query.last())
            nbMedias = query.at() + 1;
    }
    QCOMPARE(nbMedias, 17571);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAudios()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbAudios = 0;
    if (query.exec("SELECT media.id from media LEFT OUTER JOIN type ON media.type=type.id WHERE type.name='audio'")) {
        if (query.last())
            nbAudios = query.at() + 1;
    }
    QCOMPARE(nbAudios, 15262);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbVideos()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbVideos = 0;
    if (query.exec("SELECT media.id from media LEFT OUTER JOIN type ON media.type=type.id WHERE type.name='video'")) {
        if (query.last())
            nbVideos = query.at() + 1;
    }
    QCOMPARE(nbVideos, 2309);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbums()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbAlbums = 0;
    if (query.exec("SELECT DISTINCT album from media WHERE album is not null")) {
        if (query.last())
            nbAlbums = query.at() + 1;
    }
    QCOMPARE(nbAlbums, 1373);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbumPictures()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbAlbumPictures = 0;
    if (query.exec("SELECT DISTINCT id from picture")) {
        if (query.last())
            nbAlbumPictures = query.at() + 1;
    }
    QCOMPARE(nbAlbumPictures, 873);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbTracksWithAlbum()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbTracksWithAlbum = 0;
    if (query.exec("SELECT album from media where album is not null")) {
        if (query.last())
            nbTracksWithAlbum = query.at() + 1;
    }
    QCOMPARE(nbTracksWithAlbum, 14844);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbTracksWithPicture()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    int nbTracksWithPicture = 0;
    if (query.exec("SELECT picture from media where picture is not null")) {
        if (query.last())
            nbTracksWithPicture = query.at() + 1;
    }
    QCOMPARE(nbTracksWithPicture, 9606);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbumsWithSeveralPicture()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    /* search album with several pictures */
    int nb = 0;
    query.exec("SELECT TBL_ALBUM.name, count(DISTINCT picture), TBL_ARTIST.name, media.album from media "
               "LEFT OUTER JOIN album AS TBL_ALBUM ON media.album=TBL_ALBUM.id "
               "LEFT OUTER JOIN artist AS TBL_ARTIST ON media.artist=TBL_ARTIST.id "
               "GROUP BY TBL_ALBUM.name "
               "ORDER BY TBL_ALBUM.name");
    while (query.next())
        if (query.value(1).toInt()>1) {
            nb++;
            qWarning() << "ALBUM" << query.value(0).toString() << query.value(1).toInt() << "pictures" << query.value(2).toString() << "idAlbum" << query.value(3).toInt();
        }
    QVERIFY2(nb==6, QString("%1 albums with several pictures").arg(nb).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbPictureNotUsed()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    /* search picture not used */
    int nb = 0;
    query.exec("SELECT picture.id, count(media.filename), picture.name from picture LEFT OUTER JOIN media ON picture.id=media.picture GROUP BY picture.id");
    while (query.next())
        if (query.value(1).toInt()==0) {
            nb++;
            qWarning() << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toByteArray().size();
        }
    QCOMPARE(nb, 0);
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbPictureWithNoAlbum()
{
    QVERIFY(db.open());
    QSqlQuery query(db);

    /* search picture with no album */
    int nb = 0;
    query.exec("SELECT filename, picture from media where album is null and picture is not null");
    while (query.next())
        qWarning() << query.value(0).toString() << query.value(1).toInt();
    if (query.last())
        nb = query.at() + 1;
    QVERIFY2(nb==0, QString("%1 pictures with no album").arg(nb).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_DlnaCachedRootFolder()
{
    DlnaCachedRootFolder rootFolder(this);
    connect(&rootFolder, SIGNAL(error_addFolder(QString)), this, SLOT(error_addFolder(QString)));

    QCOMPARE(rootFolder.getId(), "0");
    QCOMPARE(rootFolder.getName(), "root");
    QCOMPARE(rootFolder.getSystemName(), "root");
    QCOMPARE(rootFolder.getDisplayName(), "root");
    QCOMPARE(rootFolder.getDlnaParent(), Q_NULLPTR);
    QCOMPARE(rootFolder.getDlnaParentId(), "-1");
    QCOMPARE(rootFolder.getResourceId(), "0");
    QCOMPARE(rootFolder.isFolder(), true);
    QCOMPARE(rootFolder.getUpdateId(), 1);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Video");
    QThreadPool::globalInstance()->waitForDone();
    QCOMPARE(folderKO, "/Users/doudou/Music/iTunes/iTunes Media/Video");

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Movies/Films/Comédie");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());
    QCOMPARE(rootFolder.getChildrenSize(), 9);
    QVERIFY(rootFolder.getChild(0) != Q_NULLPTR);
}

void tst_dlnacachedresources::testCase_DlnaCachedMusicTrack() {
    DlnaCachedRootFolder rootFolder(this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());
    QCOMPARE(rootFolder.getChildrenSize(), 9);

    auto root = qobject_cast<DlnaRootFolder*>(rootFolder.getChild(8));
    QVERIFY(root != Q_NULLPTR);
    if (root)
    {
        QCOMPARE(root->getSystemName(), "root");
        QCOMPARE(root->getChildrenSize(), 1);
    }

    QVERIFY(rootFolder.getChildrenSize() > 0);
    DlnaResource* music_folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        music_folder = rootFolder.getChild(index);
        if (music_folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(music_folder != Q_NULLPTR);
    if (music_folder)
    {
        QCOMPARE(music_folder->getSystemName(), "Music");

        QVERIFY(music_folder->getChildrenSize() > 0);
        DlnaResource* artist_folder = music_folder->getChild(0);
        QCOMPARE(artist_folder->getSystemName(), "Artist");

        QVERIFY(artist_folder->getChildrenSize() > 0);
        int artist_index = 0;
        DlnaResource* artist_M = artist_folder->getChild(artist_index);
        while (artist_index < artist_folder->getChildrenSize() && artist_M->getSystemName() != "-M-") {
            artist_M = artist_folder->getChild(artist_index);
            artist_index++;
        }
        QList<DlnaResource*> results = artist_folder->getDLNAResources(artist_M->getResourceId(), false, 0, 1, "*");
        if (results.size() == 1)
            artist_M = results.at(0);
        QCOMPARE(artist_M->getSystemName(), "-M-");

        QVERIFY(artist_M->getChildrenSize() > 0);
        auto track = qobject_cast<DlnaCachedMusicTrack*>(artist_M->getChild(0));
        track->setTranscodeFormat(MP3);
        track->setHostUrl(QUrl("http://host:600"));

        QStringList sinkProtocol;
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X";
        m_dlnaProfiles->setProtocols(sinkProtocol);
        track->setDlnaProfiles(m_dlnaProfiles);

        QCOMPARE(track->getSystemName(), "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a");

        check_dlna_audio(track,
                         "0$6$1$4$1", "0$6$1$4",
                         "Monde virtuel", "Je dis aime", "-M-", "-M-", "Pop", 1, "2013-01-02",
                         1);

        check_dlna_audio_res(track, 0,
                             "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                             "00:03:09", 2, 44100,
                             40000, 7561480, "http://host:600/get/0$6$1$4$1/Media%281%29");

        QCOMPARE(track->mimeType(), "audio/mpeg");
        QCOMPARE(track->metaDataFormat(), "aac");
        QCOMPARE(track->size(), 7561480);
        QCOMPARE(track->bitrate(), 320000);
        QCOMPARE(track->getLengthInSeconds(), 189);
        QCOMPARE(track->getLengthInMilliSeconds(), 188987);
        QCOMPARE(track->samplerate(), 44100);
        QCOMPARE(track->channelCount(), 2);

        QCOMPARE(track->getdlnaOrgOpFlags(), "10");
        QCOMPARE(track->getdlnaOrgPN(), "MP3");
        QCOMPARE(track->getDlnaContentFeatures(), "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
        QCOMPARE(track->getProtocolInfo(), "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

        QCOMPARE(track->metaDataPerformerSort(), "M");
        QCOMPARE(track->metaDataAlbumArtist(), "-M-");
        QCOMPARE(track->metaDataYear(), 1999);

        QVERIFY(!track->getAlbumArt().isNull());
        QVERIFY(!track->getByteAlbumArt().isNull());
        QCOMPARE(track->getAlbumArt().size().width(), 300);
        QCOMPARE(track->getAlbumArt().size().height(), 300);
        QCOMPARE(track->getByteAlbumArt().size(), 22458);

        QHash<QString, double> result = track->volumeInfo();
        QCOMPARE(result.size(), 2);
        QCOMPARE(QVariant::fromValue(result["mean_volume"]).toString(), "-12.5");
        QCOMPARE(QVariant::fromValue(result["max_volume"]).toString(), "0");

        check_streaming(track, -1, -1, "", -1, 1089, track->size(), 7559880, 7000);
    }
}

void tst_dlnacachedresources::testCase_DlnaCachedVideo() {
    DlnaCachedRootFolder rootFolder(this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Movies");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());

    DlnaCachedFolder* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedFolder*>(rootFolder.getChild(index));
        if (folder && folder->getDisplayName() == "video")
            break;
    }

    if (folder)
    {
        QList<DlnaResource*> results = rootFolder.getDLNAResources(folder->getResourceId(), false, 0, 1, "*");
        if (results.size() == 1)
            folder = qobject_cast<DlnaCachedFolder*>(results.at(0));
    }

    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getDisplayName(), "video");
        QVERIFY2(folder->getChildrenSize() >= 881, QString("%1").arg(folder->getChildrenSize()).toUtf8().constData());

        DlnaCachedVideo* movie = Q_NULLPTR;
        for (int index=0;index<folder->getChildrenSize();++index)
        {
            movie = qobject_cast<DlnaCachedVideo*>(folder->getChild(index));
            if (movie && movie->getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv")
                break;
        }

        QVERIFY(movie != Q_NULLPTR);
        if (movie)
        {
            QCOMPARE(movie->getSystemName(), "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");
            movie->setTranscodeFormat(MPEG2_AC3);
            movie->setHostUrl(QUrl("http://host:600"));

            QStringList sinkProtocol;
            sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
            sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
            sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
            m_dlnaProfiles->setProtocols(sinkProtocol);
            movie->setDlnaProfiles(m_dlnaProfiles);

            check_dlna_video(movie,
                             "0$8$126", "0$8",
                             "District.9.2009.720p.BrRip.YIFY",
                             2);

            check_dlna_video_res(movie, 0,
                                 "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                                 "01:52:16", "1280x688", 2, 48000,
                                 569850, 3973129325, "");

            check_dlna_audio_res(movie, 1,
                                 "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1",
                                 "01:52:16", 2, 48000,
                                 40000, 52800000,
                                 "http://host:600/get/$1/S05 E05 Ninjago Le sabre du sanctuaire?format=MP3");

            QCOMPARE(movie->getdlnaOrgOpFlags(), "10");
            QCOMPARE(movie->getdlnaOrgPN(), "MPEG_TS_HD_NA");
            QCOMPARE(movie->getDlnaContentFeatures(), "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
            QCOMPARE(movie->getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

            QVERIFY(movie->getAlbumArt().isNull());
            QVERIFY(movie->getByteAlbumArt().isNull());

            QHash<QString, double> result = movie->volumeInfo();
            QCOMPARE(result.size(), 2);
            QCOMPARE(QVariant::fromValue(result["mean_volume"]).toString(), "-26.1");
            QCOMPARE(QVariant::fromValue(result["max_volume"]).toString(), "0");

            QVERIFY(movie->toTranscode());
            QCOMPARE(movie->mimeType(), "video/vnd.dlna.mpeg-tts");
            //    QVERIFY(movie->size() == 3973129325);
            QCOMPARE(movie->bitrate(), 4558800);
            //    QVERIFY(movie->getLengthInSeconds() == 6736);
            //    QVERIFY(movie->getLengthInMilliSeconds() == 6735830);
            QCOMPARE(movie->samplerate(), 48000);
            QCOMPARE(movie->channelCount(), 2);
            QCOMPARE(movie->resolution(), "1280x688");
            QCOMPARE(movie->audioLanguages(), QStringList() << "");
            QCOMPARE(movie->subtitleLanguages(), QStringList() << "eng");
            QCOMPARE(movie->framerate(), "23.976");

            /* test partial transcoding (10 seconds) */
            check_streaming(movie, -1, 10, "", 0, 0, 6181162, 1827548, 100);
        }
    }
}

qint64 tst_dlnacachedresources::parseFolder(const QString& resourceId, DlnaResource *resource) {
    QElapsedTimer timer;
    qint64 elapsed = 0;

    QList<DlnaResource*> l_child;
    for (int i = 0; i<3; i++)
    {
        QObject scope;
        timer.restart();
        l_child = resource->getDLNAResources(resourceId, true, 0, 608, "", &scope);
        foreach(DlnaResource* child, l_child)
        {
            auto item = qobject_cast<DlnaItem*>(child);

            if (item)
            {
                item->setHostUrl(QUrl("http://host:600"));

                QStringList sinkProtocol;
                sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
                sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X";
                m_dlnaProfiles->setProtocols(sinkProtocol);
                item->setDlnaProfiles(m_dlnaProfiles);

                child->getStringContentDirectory(QStringList("*"));
                Device * stream = item->getStream();
                delete stream;   /* remove stream which is destroyed with deletelater */
            }
        }

        qint64 tmp = timer.elapsed();
        if (tmp > elapsed) {
            elapsed = tmp;
        }
    }
    return elapsed;
}

void tst_dlnacachedresources::testCase_PerformanceAllArtists() {
    DlnaCachedRootFolder rootFolder(this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }

    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
        QVERIFY(allArtists != Q_NULLPTR);

        qint64 duration = parseFolder(allArtists->getResourceId(), allArtists);
        QCOMPARE(allArtists->getSystemName(), "Artist");
        QVERIFY2(allArtists->getChildrenSize() >= 1000, QString("%1").arg(allArtists->getChildrenSize()).toUtf8());
        qInfo() << "PERFO" << duration << allArtists->getSystemName() << allArtists->getChildrenSize() << "children";
        if (duration > 200)
            qWarning() << "Parse all artists in" << duration << "ms, > 200 ms";
    }
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByArtist() {
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
        QVERIFY(allArtists != Q_NULLPTR);
        if (allArtists)
        {
            QCOMPARE(allArtists->getSystemName(), "Artist");
            QVERIFY(allArtists->getChildrenSize() >= 1000);

            qint64 max = 0;
            for(int idxArtist=0;idxArtist<allArtists->getChildrenSize();idxArtist++) {
                DlnaResource *artist = allArtists->getChild(idxArtist);
                qint64 elapsed;
                elapsed = parseFolder(artist->getResourceId(), allArtists);
                if (elapsed > max) {
                    max = elapsed;
                    qInfo() << "PERFO" << elapsed << artist->getResourceId() << artist->getSystemName() << artist->getChildrenSize() << "children";
                }
            }
            if (max > 700)
                qWarning() << "Parse all tracks by artist in" << max << "ms, > 700 ms";
        }
    }
}

void tst_dlnacachedresources::testCase_PerformanceAllAlbums()
{
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != Q_NULLPTR);

    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
        QVERIFY(allAlbums != Q_NULLPTR);

        qint64 duration = parseFolder(allAlbums->getResourceId(), allAlbums);
        QCOMPARE(allAlbums->getSystemName(), "Album");
        QVERIFY(allAlbums->getChildrenSize() >= 1000);
        qInfo() << "PERFO" << duration << allAlbums->getSystemName() << allAlbums->getChildrenSize() << "children";
        if (duration > 300)
            qWarning() << "Parse all albums in" << duration << "ms, > 300 ms.";
    }
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByAlbum()
{
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }

    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
        QVERIFY(allAlbums != Q_NULLPTR);
        if (allAlbums)
        {
            QCOMPARE(allAlbums->getSystemName(), "Album");
            QVERIFY(allAlbums->getChildrenSize() >= 1000);

            qint64 max = 0;
            for(int idxAlbum=0;idxAlbum<allAlbums->getChildrenSize();idxAlbum++) {
                DlnaResource *album = allAlbums->getChild(idxAlbum);
                qint64 elapsed;
                elapsed = parseFolder(album->getResourceId(), allAlbums);
                if (elapsed > max) {
                    max = elapsed;
                    qInfo() << "PERFO" << elapsed << album->getResourceId() << album->getSystemName() << album->getChildrenSize() << "children";
                }
            }
            if (max > 500)
                qWarning() << "Parse all tracks by album in" << max << "ms, > 500 ms";
        }
    }
}

void tst_dlnacachedresources::testCase_PerformanceAllGenres()
{
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }

    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
        QVERIFY(allGenres != Q_NULLPTR);

        qint64 duration = parseFolder(allGenres->getResourceId(), allGenres);
        QCOMPARE(allGenres->getSystemName(), "Genre");
        QVERIFY(allGenres->getChildrenSize() >= 100);
        qInfo() << "PERFO" << duration << allGenres->getSystemName() << allGenres->getChildrenSize() << "children";
        if (duration > 200)
            qWarning() << "Parse all genres in" << duration << "ms, > 200 ms";
    }
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByGenre() {
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData* folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getSystemName(), "Music");

        auto allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
        QVERIFY(allGenres != Q_NULLPTR);
        if (allGenres)
        {
            QCOMPARE(allGenres->getSystemName(), "Genre");
            QVERIFY(allGenres->getChildrenSize() >= 100);

            qint64 max = 0;
            for(int idxGenre=0;idxGenre<allGenres->getChildrenSize();idxGenre++) {
                DlnaResource *genre = allGenres->getChild(idxGenre);
                qint64 elapsed;
                elapsed = parseFolder(genre->getResourceId(), allGenres);
                if (elapsed > max) {
                    max = elapsed;
                    qInfo() << "PERFO" << elapsed << genre->getResourceId() << genre->getSystemName() << genre->getChildrenSize() << "children";
                }
            }
            if (max > 2000)
                qWarning() << "Parse all tracks by genre in" << max << "ms, > 2000 ms";
        }
    }
}
