#include "tst_dlnacachedresources.h"

tst_dlnacachedresources::tst_dlnacachedresources(QObject *parent) :
    QObject(parent),
    transcodeProcess(0),
    transcodedSize(0),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE")),
    folderKO()
{
    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
}

void tst_dlnacachedresources::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
    emit bytesSent(transcodedSize, 1);
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
    QVERIFY2(nbMedias == 15370, QString("%1").arg(nbMedias).toUtf8().constData());
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
    QVERIFY2(nbAudios == 13739, QString("%1").arg(nbAudios).toUtf8().constData());
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
    QVERIFY2(nbVideos == 1631, QString("%1").arg(nbVideos).toUtf8().constData());
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
    QVERIFY2(nbAlbums == 1279, QString("%1").arg(nbAlbums).toUtf8().constData());
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
    QVERIFY2(nbAlbumPictures == 798, QString("%1").arg(nbAlbumPictures).toUtf8().constData());
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
    QVERIFY2(nbTracksWithAlbum == 13484, QString("%1").arg(nbTracksWithAlbum).toUtf8().constData());
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
    QVERIFY2(nbTracksWithPicture == 9450, QString("%1").arg(nbTracksWithPicture).toUtf8().constData());
    db.close();
}

void tst_dlnacachedresources::testCase_Library_NbAlbumsWithSeveralPicture()
{
    QVERIFY(db.open() == true);
    QSqlQuery query(db);

    // search album with several pictures
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

    // search picture not used
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

    // search picture with no album
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
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 100, this);
    connect(&rootFolder, SIGNAL(error_addFolder(QString)), this, SLOT(error_addFolder(QString)));

    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getDlnaParent() == 0);
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
    QVERIFY(rootFolder.getChildrenSize() == 8);
    QVERIFY(rootFolder.getChild(0) != 0);
}

void tst_dlnacachedresources::testCase_DlnaCachedMusicTrack() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());
    QVERIFY(rootFolder.getChildrenSize() == 8);

    DlnaRootFolder* root = qobject_cast<DlnaRootFolder*>(rootFolder.getChild(7));
    QVERIFY(root != 0);
    QVERIFY(root->getSystemName() == "root");
    QVERIFY(root->getChildrenSize() == 1);

    QVERIFY(rootFolder.getChildrenSize() > 0);
    DlnaResource* music_folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        music_folder = rootFolder.getChild(index);
        if (music_folder->getSystemName() == "Music")
            break;
    }
    QVERIFY2(music_folder->getSystemName() == "Music", music_folder->getSystemName().toUtf8().constData());

    QVERIFY(music_folder->getChildrenSize()> 0);
    DlnaResource* artist_folder = music_folder->getChild(0);
    QVERIFY(artist_folder->getSystemName() == "Artist");

    QVERIFY(artist_folder->getChildrenSize() > 0);
    int artist_index = 0;
    DlnaResource* artist_M = artist_folder->getChild(artist_index);
    while (artist_index < artist_folder->getChildrenSize() and artist_M->getSystemName() != "-M-") {
        artist_M = artist_folder->getChild(artist_index);
        artist_index++;
    }
    QVERIFY2(artist_M->getSystemName() == "-M-", artist_M->getSystemName().toUtf8());

    QVERIFY(artist_M->getChildrenSize()> 0);
    DlnaCachedMusicTrack *track = qobject_cast<DlnaCachedMusicTrack*>(artist_M->getChild(0));
    track->setTranscodeFormat(MP3);
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
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(!node.attributes().namedItem("id").nodeValue().isEmpty());
    QVERIFY(!node.attributes().namedItem("parentID").nodeValue().isEmpty());
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Monde virtuel");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Je dis aime");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "Pop");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "1");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY2(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02", xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY2(!xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().isEmpty(), xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().size() == 7, QString("%1").arg(xml_res.elementsByTagName("res").at(0).attributes().size()).toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7561158");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "44100");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");

    xml_res.clear();

    QVERIFY(track->mimeType() == "audio/mpeg");
    QVERIFY2(track->metaDataFormat() == "aac", track->metaDataFormat().toUtf8());
    QVERIFY(track->size() == 7561158);
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
    QVERIFY2(track->getByteAlbumArt().size() == 22471, QString("size=%1").arg(track->getByteAlbumArt().size()).toUtf8());

    QHash<QString, double> result = track->volumeInfo();
    QVERIFY2(result.keys().size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    QVERIFY2(result["mean_volume"] == -12.5, QString("%1").arg(result["mean_volume"]).toUtf8());
    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());

    HttpRange *range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track->size());
    Device *device = track->getStream(range);
    QVERIFY(device != 0);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), transcodeProcess, SLOT(bytesSent(qint64,qint64)));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open()==true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    QVERIFY2(transcodedSize == 7560469, QString("%1").arg(transcodedSize).toUtf8().constData());
    QVERIFY(track->size() > transcodedSize);
    delete transcodeProcess;
    transcodeProcess = 0;

    delete range;
    range = 0;
}

void tst_dlnacachedresources::testCase_DlnaCachedVideo() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Movies");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());

    DlnaCachedFolder* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedFolder*>(rootFolder.getChild(index));
        if (folder && folder->getDisplayName() == "video")
            break;
    }

    QVERIFY(folder != 0);
    QVERIFY2(folder->getDisplayName() == "video", folder->getDisplayName().toUtf8().constData());
    QVERIFY2(folder->getChildrenSize() >= 881, QString("%1").arg(folder->getChildrenSize()).toUtf8().constData());

    DlnaCachedVideo* movie = 0;
    for (int index=0;index<folder->getChildrenSize();++index)
    {
        movie = qobject_cast<DlnaCachedVideo*>(folder->getChild(index));
        if (movie && movie->getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv")
            break;
    }

    QVERIFY(movie != 0);
    QVERIFY(movie->getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");
    movie->setTranscodeFormat(MPEG2_AC3);

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
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(!node.attributes().namedItem("id").nodeValue().isEmpty());
    QVERIFY(!node.attributes().namedItem("parentID").nodeValue().isEmpty());
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "District.9.2009.720p.BrRip.YIFY");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY2(!xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().isEmpty(), xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 8);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
//    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:52:16", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "1280x688");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "569850", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8().constData());
//    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "3973129325", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8().constData());
    xml_res.clear();

    QVERIFY(movie->getdlnaOrgOpFlags() == "10");
    QVERIFY(movie->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY(movie->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");

    QVERIFY(movie->getAlbumArt().isNull() == true);
    QVERIFY(movie->getByteAlbumArt().isNull() == true);

    QHash<QString, double> result = movie->volumeInfo();
    QVERIFY2(result.keys().size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    QVERIFY2(result["mean_volume"] == -26.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());

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

    // test partial transcoding (10 seconds)
    Device *device = movie->getStream(0, 0, 10);
    QVERIFY(device != 0);

    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), transcodeProcess, SLOT(bytesSent(qint64,qint64)));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open()==true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    QVERIFY2(transcodedSize == 6106804, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    QVERIFY(movie->size() > transcodedSize);
    delete transcodeProcess;
    transcodeProcess = 0;
}

int tst_dlnacachedresources::parseFolder(QString resourceId, DlnaResource *resource) {
    QElapsedTimer timer;
    int elapsed = 0;

    QList<DlnaResource*> l_child;
    for (int i = 0; i<3; i++) {

        timer.restart();
        l_child = resource->getDLNAResources(resourceId, true, 0, 608, "");
        foreach(DlnaResource* child, l_child) {
            child->getStringContentDirectory(QStringList("*"));
        }
        int tmp = timer.elapsed();
        if (tmp > elapsed) {
            elapsed = tmp;
        }
    }
    return elapsed;
}

void tst_dlnacachedresources::testCase_PerformanceAllArtists() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    folderKO.clear();
    rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(folderKO.isEmpty());

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }

    QVERIFY(folder != 0);
    QVERIFY2(folder->getSystemName() == "Music", folder->getSystemName().toUtf8().constData());

    DlnaCachedFolderMetaData* allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
    QVERIFY(allArtists != 0);

    int duration = parseFolder(allArtists->getResourceId(), allArtists);
    QVERIFY(allArtists->getSystemName() == "Artist");
    QVERIFY2(allArtists->getChildrenSize() >= 1000, QString("%1").arg(allArtists->getChildrenSize()).toUtf8());
    qWarning() << "PERFO" << duration << allArtists->getSystemName() << allArtists->getChildrenSize() << "children";
    QVERIFY2(duration < 200, QString("Parse all artists in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByArtist() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != 0);
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolderMetaData* allArtists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
    QVERIFY(allArtists != 0);
    QVERIFY(allArtists->getSystemName() == "Artist");
    QVERIFY(allArtists->getChildrenSize() >= 1000);

    int max = 0;
    for(int idxArtist=0;idxArtist<allArtists->getChildrenSize();idxArtist++) {
        DlnaResource *artist = allArtists->getChild(idxArtist);
        int elapsed;
        elapsed = parseFolder(artist->getResourceId(), allArtists);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << artist->getResourceId() << artist->getSystemName() << artist->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 700, QString("Parse all tracks by artist in %1 ms").arg(max).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllAlbums() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != 0);
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolderMetaData* allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
    QVERIFY(allAlbums != 0);

    int duration = parseFolder(allAlbums->getResourceId(), allAlbums);
    QVERIFY(allAlbums->getSystemName() == "Album");
    QVERIFY(allAlbums->getChildrenSize()>= 1000);
    qWarning() << "PERFO" << duration << allAlbums->getSystemName() << allAlbums->getChildrenSize() << "children";
    QVERIFY2(duration < 300, QString("Parse all albums in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByAlbum() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != 0);
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolderMetaData* allAlbums = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(1));
    QVERIFY(allAlbums != 0);
    QVERIFY(allAlbums->getSystemName() == "Album");
    QVERIFY(allAlbums->getChildrenSize() >= 1000);

    int max = 0;
    for(int idxAlbum=0;idxAlbum<allAlbums->getChildrenSize();idxAlbum++) {
        DlnaResource *album = allAlbums->getChild(idxAlbum);
        int elapsed;
        elapsed = parseFolder(album->getResourceId(), allAlbums);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << album->getResourceId() << album->getSystemName() << album->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 500, QString("Parse all tracks by album in %1 ms").arg(max).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllGenres() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != 0);
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolderMetaData* allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
    QVERIFY(allGenres != 0);

    int duration = parseFolder(allGenres->getResourceId(), allGenres);
    QVERIFY(allGenres->getSystemName() == "Genre");
    QVERIFY(allGenres->getChildrenSize() >= 100);
    qWarning() << "PERFO" << duration << allGenres->getSystemName() << allGenres->getChildrenSize() << "children";
    QVERIFY2(duration < 200, QString("Parse all genres in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByGenre() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedGroupedFolderMetaData* folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getSystemName() == "Music")
            break;
    }
    QVERIFY(folder != 0);
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolderMetaData* allGenres = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(2));
    QVERIFY(allGenres != 0);
    QVERIFY(allGenres->getSystemName() == "Genre");
    QVERIFY(allGenres->getChildrenSize() >= 100);

    int max = 0;
    for(int idxGenre=0;idxGenre<allGenres->getChildrenSize();idxGenre++) {
        DlnaResource *genre = allGenres->getChild(idxGenre);
        int elapsed;
        elapsed = parseFolder(genre->getResourceId(), allGenres);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << genre->getResourceId() << genre->getSystemName() << genre->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 2000, QString("Parse all tracks by genre in %1 ms").arg(max).toUtf8());
}
