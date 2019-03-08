#include "tst_dlnacachedresources.h"

tst_dlnacachedresources::tst_dlnacachedresources(QObject *parent) :
    QObject(parent),
    transcodedSize(0),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE")),
    folderKO()
{
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
    db.open();
}

void tst_dlnacachedresources::cleanup()
{
    QVERIFY2(DlnaResource::objectCounter == 0, QString("memory leak detected, %1 DlnaResource objects.").arg(DlnaResource::objectCounter).toUtf8());

    QCOMPARE(QFfmpegMedia::objectCounter, 0);
    QCOMPARE(QFfmpegStream::objectCounter, 0);
    QCOMPARE(QFfmpegFrame::objectCounter, 0);
    QCOMPARE(QFfmpegCodec::objectCounter, 0);
    QCOMPARE(QFfmpegBuffer::objectCounter, 0);
}

void tst_dlnacachedresources::dataAvailable()
{
    auto device = qobject_cast<TranscodeDevice*>(sender());
    if (device)
        device->requestData(1000000);
    else
        qCritical() << "invalid device" << sender();
}

void tst_dlnacachedresources::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
}

void tst_dlnacachedresources::LogMessage(const QString &message)
{
    qInfo() << message;
}

void tst_dlnacachedresources::testCase_Library_NbMedias()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbMedias = 0;
    if (query.exec("SELECT id from media")) {
        if (query.last())
            nbMedias = query.at() + 1;
    }
    QVERIFY2(nbMedias == 17249, QString("%1").arg(nbMedias).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAudios()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbAudios = 0;
    if (query.exec("SELECT media.id from media LEFT OUTER JOIN type ON media.type=type.id WHERE type.name='audio'")) {
        if (query.last())
            nbAudios = query.at() + 1;
    }
    QVERIFY2(nbAudios == 15241, QString("%1").arg(nbAudios).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbVideos()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbVideos = 0;
    if (query.exec("SELECT media.id from media LEFT OUTER JOIN type ON media.type=type.id WHERE type.name='video'")) {
        if (query.last())
            nbVideos = query.at() + 1;
    }
    QVERIFY2(nbVideos == 2008, QString("%1").arg(nbVideos).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbums()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbAlbums = 0;
    if (query.exec("SELECT DISTINCT album from media WHERE album is not null")) {
        if (query.last())
            nbAlbums = query.at() + 1;
    }
    QVERIFY2(nbAlbums == 1365, QString("%1").arg(nbAlbums).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbumPictures()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbAlbumPictures = 0;
    if (query.exec("SELECT DISTINCT id from picture")) {
        if (query.last())
            nbAlbumPictures = query.at() + 1;
    }
    QVERIFY2(nbAlbumPictures == 870, QString("%1").arg(nbAlbumPictures).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbTracksWithAlbum()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbTracksWithAlbum = 0;
    if (query.exec("SELECT album from media where album is not null")) {
        if (query.last())
            nbTracksWithAlbum = query.at() + 1;
    }
    QVERIFY2(nbTracksWithAlbum == 14622, QString("%1").arg(nbTracksWithAlbum).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbTracksWithPicture()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    int nbTracksWithPicture = 0;
    if (query.exec("SELECT picture from media where picture is not null")) {
        if (query.last())
            nbTracksWithPicture = query.at() + 1;
    }
    QVERIFY2(nbTracksWithPicture == 9587, QString("%1").arg(nbTracksWithPicture).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbumsWithSeveralPicture()
{
    QVERIFY(db.open() == true);
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
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    /* search picture not used */
    int nb = 0;
    query.exec("SELECT picture.id, count(media.filename), picture.name from picture LEFT OUTER JOIN media ON picture.id=media.picture GROUP BY picture.id");
    while (query.next())
        if (query.value(1).toInt()==0) {
            nb++;
            qWarning() << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toByteArray().size();
        }
    QVERIFY2(nb==0, QString("%1").arg(nb).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbPictureWithNoAlbum()
{
    QVERIFY(db.open() == true);
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

    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getDlnaParent() == Q_NULLPTR);
    QVERIFY(rootFolder.getDlnaParentId() == "-1");
    QVERIFY(rootFolder.getResourceId() == "0");
    QVERIFY(rootFolder.isFolder() == true);
    QVERIFY(rootFolder.getUpdateId() == 1);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Video");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO == "/Users/doudou/Music/iTunes/iTunes Media/Video");

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
        QVERIFY(root->getSystemName() == "root");
        QVERIFY(root->getChildrenSize() == 1);
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
        QVERIFY2(music_folder->getSystemName() == "Music", music_folder->getSystemName().toUtf8().constData());

        QVERIFY(music_folder->getChildrenSize()> 0);
        DlnaResource* artist_folder = music_folder->getChild(0);
        QVERIFY(artist_folder->getSystemName() == "Artist");

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
        QVERIFY2(artist_M->getSystemName() == "-M-", artist_M->getSystemName().toUtf8());

        QVERIFY(artist_M->getChildrenSize()> 0);
        auto track = qobject_cast<DlnaCachedMusicTrack*>(artist_M->getChild(0));
        track->setTranscodeFormat(MP3);
        track->setHostUrl(QUrl("http://host:600"));

        QStringList sinkProtocol;
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
        track->setSinkProtocol(sinkProtocol);

        QVERIFY2(track->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a", track->getSystemName().toUtf8().constData());

        QStringList properties;
        properties << "dc:title";
        properties << "upnp:album";
        properties << "upnp:artist";
        properties << "dc:contributor";
        properties << "upnp:genre";
        properties << "upnp:originalTrackNumber";
        properties << "dc:date";
        properties << "res@size";
        properties << "res@duration";
        properties << "res@bitrate";
        properties << "res@sampleFrequency";
        properties << "res@nrAudioChannels";

        QDomDocument xml_res;
        xml_res.appendChild(track->getXmlContentDirectory(&xml_res, properties));
        check_dlna_audio(xml_res,
                         "", "",
                         "Monde virtuel", "Je dis aime", "-M-", "-M-", "Pop", 1, "2013-01-02",
                         "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1",
                         "00:03:09", 2, 44100,
                         40000, 7561480, "");
        xml_res.clear();

        QVERIFY(track->mimeType() == "audio/mpeg");
        QVERIFY2(track->metaDataFormat() == "aac", track->metaDataFormat().toUtf8());
        QVERIFY(track->size() == 7561480);
        QVERIFY(track->bitrate() == 320000);
        QVERIFY(track->getLengthInSeconds() == 189);
        QVERIFY(track->getLengthInMilliSeconds() == 188987);
        QVERIFY(track->samplerate() == 44100);
        QVERIFY(track->channelCount() == 2);

        QVERIFY(track->getdlnaOrgOpFlags() == "10");
        QVERIFY(track->getdlnaOrgPN() == "MP3");
        QVERIFY(track->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
        QVERIFY(track->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1");

        QVERIFY2(track->metaDataPerformerSort() == "M", track->metaDataPerformerSort().toUtf8());
        QVERIFY2(track->metaDataAlbumArtist() == "-M-", track->metaDataAlbumArtist().toUtf8());
        QVERIFY2(track->metaDataYear() == 1999, QString("%1").arg(track->metaDataYear()).toUtf8());

        QVERIFY(track->getAlbumArt().isNull() == false);
        QVERIFY(track->getByteAlbumArt().isNull() == false);
        QVERIFY(track->getAlbumArt().size().width() == 300);
        QVERIFY(track->getAlbumArt().size().height() == 300);
        QVERIFY2(track->getByteAlbumArt().size() == 22458, QString("size=%1").arg(track->getByteAlbumArt().size()).toUtf8());

        QHash<QString, double> result = track->volumeInfo();
        QVERIFY2(result.size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
        QVERIFY2(QVariant::fromValue(result["mean_volume"]).toString() == "-12.5", QString("%1").arg(result["mean_volume"]).toUtf8());
        QVERIFY2(QVariant::fromValue(result["max_volume"]).toString() == "0", QString("%1").arg(result["max_volume"]).toUtf8());

        Device *device = track->getStream();
        QVERIFY(device != Q_NULLPTR);

        QScopedPointer<QFfmpegTranscoding> transcodeProcess(qobject_cast<QFfmpegTranscoding*>(device));
        QVERIFY(transcodeProcess != Q_NULLPTR);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        QVERIFY(transcodeProcess->open()==true);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        QVERIFY(transcodeProcess->exitCode() == 0);
        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QCOMPARE(transcodedSize, 7559880);
        QVERIFY(track->size() > transcodedSize);

        /* delete stream because deleted by deletelater */
        device = track->getStream();
        delete device;
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
        QVERIFY2(folder->getDisplayName() == "video", folder->getDisplayName().toUtf8().constData());
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
            QVERIFY(movie->getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");
            movie->setTranscodeFormat(MPEG2_AC3);
            movie->setHostUrl(QUrl("http://host:600"));

            QStringList sinkProtocol;
            sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
            movie->setSinkProtocol(sinkProtocol);

            {
                QStringList properties;
                properties << "upnp:genre";
                properties << "res@size";
                properties << "res@duration";
                properties << "res@bitrate";
                properties << "res@resolution";
                properties << "res@nrAudioChannels";
                properties << "res@sampleFrequency";

                QDomDocument xml_res;
                xml_res.appendChild(movie->getXmlContentDirectory(&xml_res, properties));
                check_dlna_video(xml_res,
                                 "0$8$126", "0$8",
                                 "District.9.2009.720p.BrRip.YIFY", "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1",
                                 "01:52:16", "1280x688", 2, 48000,
                                 569850, 3973129325, "");
            }

            QVERIFY(movie->getdlnaOrgOpFlags() == "10");
            QVERIFY(movie->getdlnaOrgPN() == "MPEG_PS_PAL");
            QVERIFY(movie->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
            QVERIFY(movie->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");

            QVERIFY(movie->getAlbumArt().isNull() == true);
            QVERIFY(movie->getByteAlbumArt().isNull() == true);

            QHash<QString, double> result = movie->volumeInfo();
            QVERIFY2(result.size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
            QVERIFY2(QVariant::fromValue(result["mean_volume"]).toString() == "-26.1", QString("%1").arg(result["mean_volume"]).toUtf8());
            QVERIFY2(QVariant::fromValue(result["max_volume"]).toString() == "0", QString("%1").arg(result["max_volume"]).toUtf8());

            QVERIFY(movie->toTranscode() == true);
            QVERIFY(movie->mimeType() == "video/mpeg");
            //    QVERIFY(movie->size() == 3973129325);
            QVERIFY(movie->bitrate() == 4558800);
            //    QVERIFY(movie->getLengthInSeconds() == 6736);
            //    QVERIFY(movie->getLengthInMilliSeconds() == 6735830);
            QVERIFY(movie->samplerate() == 48000);
            QVERIFY(movie->channelCount() == 2);
            QVERIFY(movie->resolution() == "1280x688");
            QVERIFY2(movie->audioLanguages() == QStringList() << "", movie->audioLanguages().join(',').toUtf8());
            QVERIFY2(movie->subtitleLanguages() == QStringList() << "eng", movie->subtitleLanguages().join(',').toUtf8());
            QVERIFY2(movie->framerate() == "23.976", movie->framerate().toUtf8());

            /* test partial transcoding (10 seconds) */
            Device *device = movie->getStream();
            QVERIFY(device != Q_NULLPTR);
            device->setTimeSeek(-1, 10);

            {
                QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
                QVERIFY(transcodeProcess != Q_NULLPTR);

                transcodedSize = 0;
                connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
                connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
                connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
                connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
                QVERIFY(transcodeProcess->open()==true);
                emit startTranscoding();
                transcodeProcess->waitForFinished(-1);
                QVERIFY(transcodeProcess->exitCode() == 0);
                transcodeProcess->requestData(transcodeProcess->bytesAvailable());
                QVERIFY(transcodeProcess->bytesAvailable() == 0);
                QVERIFY2(transcodedSize == 6106804, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
                QVERIFY(movie->size() > transcodedSize);
            }
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
                item->setSinkProtocol(sinkProtocol);

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
        QVERIFY2(folder->getSystemName() == "Music", folder->getSystemName().toUtf8().constData());

        auto allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
        QVERIFY(allArtists != Q_NULLPTR);

        qint64 duration = parseFolder(allArtists->getResourceId(), allArtists);
        QVERIFY(allArtists->getSystemName() == "Artist");
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
        QVERIFY(folder->getSystemName() == "Music");

        auto allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
        QVERIFY(allArtists != Q_NULLPTR);
        if (allArtists)
        {
            QVERIFY(allArtists->getSystemName() == "Artist");
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
        QVERIFY(folder->getSystemName() == "Music");

        auto allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
        QVERIFY(allAlbums != Q_NULLPTR);

        qint64 duration = parseFolder(allAlbums->getResourceId(), allAlbums);
        QVERIFY(allAlbums->getSystemName() == "Album");
        QVERIFY(allAlbums->getChildrenSize()>= 1000);
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
        QVERIFY(folder->getSystemName() == "Music");

        auto allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
        QVERIFY(allAlbums != Q_NULLPTR);
        if (allAlbums)
        {
            QVERIFY(allAlbums->getSystemName() == "Album");
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
        QVERIFY(folder->getSystemName() == "Music");

        auto allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
        QVERIFY(allGenres != Q_NULLPTR);

        qint64 duration = parseFolder(allGenres->getResourceId(), allGenres);
        QVERIFY(allGenres->getSystemName() == "Genre");
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
        QVERIFY(folder->getSystemName() == "Music");

        auto allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
        QVERIFY(allGenres != Q_NULLPTR);
        if (allGenres)
        {
            QVERIFY(allGenres->getSystemName() == "Genre");
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
