#include "tst_dlnacachedresources.h"

tst_dlnacachedresources::tst_dlnacachedresources(QObject *parent) :
    QObject(parent),
    transcodeProcess(0)
{
}

void tst_dlnacachedresources::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        QByteArray bytes = transcodeProcess->readAllStandardOutput();
        transcodedSize += bytes.size();
    }
}

void tst_dlnacachedresources::testCase_DlnaCachedRootFolder()
{
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 100, this);
    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getParent() == 0);
    QVERIFY(rootFolder.getParentId() == "-1");
    QVERIFY(rootFolder.getResourceId() == "0");
    QVERIFY(rootFolder.isFolder() == true);
    QVERIFY(rootFolder.isDiscovered() == false);
    QVERIFY(rootFolder.getUpdateId() == 1);

    int res;
    res = rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Video");
    QVERIFY(res == false);

    res = rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");
    QVERIFY(res == true);

    res = rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/VIDEO");
    QVERIFY(res == true);

    res = rootFolder.addFolder("/Users/doudou/Movies/Films/Comédie");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().isEmpty() == false);
}

void tst_dlnacachedresources::testCase_DlnaCachedMusicTrack() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    int res = rootFolder.addFolder("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().size() == 3);

    DlnaResource* root = rootFolder.getChildren().at(2);
    QVERIFY(root->getSystemName() == "root");

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(root->getChildren().at(0));
    QVERIFY(folder->getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO");

    DlnaCachedMusicTrack* track = static_cast<DlnaCachedMusicTrack*>(folder->getChildren().at(0));
    track->setTranscodeFormat(MP3);
    QVERIFY(track->getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

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

    QDomDocument xml_res;
    xml_res.appendChild(track->getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$3$1$1");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0$3$1");
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
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get/0$3$1$1/01+Monde+virtuel.m4a");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7559480");

    xml_res.clear();

    QVERIFY(track->mimeType() == "audio/mpeg");
    QVERIFY2(track->metaDataFormat() == "aac", track->metaDataFormat().toUtf8());
    QVERIFY(track->size() == 7559480);
    QVERIFY(track->bitrate() == 320000);
    QVERIFY(track->getLengthInSeconds() == 189);
    QVERIFY(track->getLengthInMilliSeconds() == 188987);
    QVERIFY(track->samplerate() == 44100);
    QVERIFY(track->channelCount() == 2);

    QVERIFY(track->getdlnaOrgOpFlags() == "01");
    QVERIFY(track->getdlnaOrgPN() == "MP3");
    QVERIFY(track->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track->getAlbumArt().isNull() == false);
    QVERIFY(track->getByteAlbumArt().isNull() == false);
    QVERIFY(track->getAlbumArt().size().width() == 100);
    QVERIFY(track->getAlbumArt().size().height() == 100);
    QVERIFY2(track->getByteAlbumArt().size() == 4067, QString("size=%1").arg(track->getByteAlbumArt().size()).toUtf8());

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track->size());
    QVERIFY(track->getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track->getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedSize == 7558825);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    QVERIFY(folder->getChildren().size() > 2);
    track = static_cast<DlnaCachedMusicTrack*>(folder->getChildren().at(2));
    QVERIFY(track->getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");
    QVERIFY(track->bitrate() == 150005);

    QVERIFY(rootFolder.getChildren().size() > 0);
    DlnaResource* music_folder = rootFolder.getChildren().at(0);
    QVERIFY(music_folder->getSystemName() == "Music");

    QVERIFY(music_folder->getChildren().size() > 0);
    DlnaResource* artist_folder = music_folder->getChildren().at(0);
    QVERIFY(artist_folder->getSystemName() == "Artist");

    QVERIFY(artist_folder->getChildren().size() > 0);
    int artist_index = 0;
    DlnaResource* artist_M = artist_folder->getChildren().at(artist_index);
    while (artist_index < artist_folder->getChildrenSize() and artist_M->getSystemName() != "-M-") {
        artist_M = artist_folder->getChildren().at(artist_index);
        artist_index++;
    }
    QVERIFY2(artist_M->getSystemName() == "-M-", artist_M->getSystemName().toUtf8());

    QVERIFY(artist_M->getChildren().size() > 0);
    track = static_cast<DlnaCachedMusicTrack*>(artist_M->getChildren().at(0));
    track->setTranscodeFormat(MP3);
    QVERIFY(track->getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

    xml_res.clear();
    xml_res.appendChild(track->getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    node = xml_res.elementsByTagName("item").at(0);
//    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$1$1$4$1");
//    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0$1$1$4");
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
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
//    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get/0$1$1$4$1/01+Monde+virtuel.m4a");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7559480");

    xml_res.clear();

    QVERIFY(track->mimeType() == "audio/mpeg");
    QVERIFY2(track->metaDataFormat() == "aac", track->metaDataFormat().toUtf8());
    QVERIFY(track->size() == 7559480);
    QVERIFY(track->bitrate() == 320000);
    QVERIFY(track->getLengthInSeconds() == 189);
    QVERIFY(track->getLengthInMilliSeconds() == 188987);
    QVERIFY(track->samplerate() == 44100);
    QVERIFY(track->channelCount() == 2);

    QVERIFY(track->getdlnaOrgOpFlags() == "01");
    QVERIFY(track->getdlnaOrgPN() == "MP3");
    QVERIFY(track->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track->getAlbumArt().isNull() == false);
    QVERIFY(track->getByteAlbumArt().isNull() == false);
    QVERIFY(track->getAlbumArt().size().width() == 100);
    QVERIFY(track->getAlbumArt().size().height() == 100);
    QVERIFY2(track->getByteAlbumArt().size() == 4068, QString("size=%1").arg(track->getByteAlbumArt().size()).toUtf8());

    range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track->size());
    QVERIFY(track->getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track->getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedSize == 7558825);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

void tst_dlnacachedresources::testCase_DlnaCachedVideo() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    int res = rootFolder.addFolder("/Users/doudou/Movies");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().size() == 3);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(1));
    QVERIFY(folder->getChildren().size() >= 881);

    DlnaCachedVideo* movie = static_cast<DlnaCachedVideo*>(folder->getChildren().at(85));
    QVERIFY(movie->getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");

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
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$2$86");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0$2");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "District.9.2009.720p.BrRip.YIFY");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get/0$2$86/District.9.2009.720p.BrRip.YIFY.mkv");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 6);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    //QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "733723671");   // original size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:52:16");
    //QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "108929");   // original bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "1280x688");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie->getdlnaOrgOpFlags() == "10");
    QVERIFY(movie->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");

    QVERIFY(movie->getAlbumArt().isNull() == true);
    QVERIFY(movie->getByteAlbumArt().isNull() == true);

    QVERIFY(movie->toTranscode() == true);
    QVERIFY(movie->mimeType() == "video/mpeg");
//    QVERIFY(movie.size() == 4587100230);
//    QVERIFY(movie.bitrate() == 5448000);
    QVERIFY(movie->size() == -1);
    QVERIFY(movie->bitrate() == -1);
    QVERIFY(movie->getLengthInSeconds() == 6736);
    QVERIFY(movie->getLengthInMilliSeconds() == 6735830);
    QVERIFY(movie->samplerate() == 48000);
    QVERIFY(movie->channelCount() == 2);
    QVERIFY(movie->resolution() == "1280x688");
    QVERIFY(movie->audioLanguages() == QStringList() << "");
    QVERIFY2(movie->subtitleLanguages() == QStringList() << "eng", movie->subtitleLanguages().join(',').toUtf8());
    QVERIFY(movie->framerate() == "23.976");

    // test partial transcoding (10 seconds)
    QVERIFY(movie->getStream() == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 733723671);
    transcodeProcess = movie->getTranscodeProcess(0, 0, 10);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize <= 3000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
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

    int res = rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().size() == 3);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allArtists = static_cast<DlnaCachedFolder*>(folder->getChildren().at(0));
    QVERIFY(allArtists->isDiscovered() == false);

    int duration = parseFolder(allArtists->getResourceId(), allArtists);
    QVERIFY(allArtists->isDiscovered() == true);
    QVERIFY(allArtists->getSystemName() == "Artist");
    QVERIFY2(allArtists->getChildren().size() >= 1000, QString("%1").arg(allArtists->getChildren().size()).toUtf8());
    qWarning() << "PERFO" << duration << allArtists->getSystemName() << allArtists->getChildrenSize() << "children";
    QVERIFY2(duration < 200, QString("Parse all artists in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByArtist() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allArtists = static_cast<DlnaCachedFolder*>(folder->getChildren().at(0));
    QVERIFY(allArtists->getSystemName() == "Artist");
    QVERIFY(allArtists->getChildren().size() >= 1000);

    int max = 0;
    foreach(DlnaResource* artist, allArtists->getChildren()) {
        int elapsed;
        elapsed = parseFolder(artist->getResourceId(), allArtists);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << artist->getResourceId() << artist->getSystemName() << artist->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 310, QString("Parse all tracks by artist in %1 ms").arg(max).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllAlbums() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allAlbums = static_cast<DlnaCachedFolder*>(folder->getChildren().at(1));
    QVERIFY(allAlbums->isDiscovered() == false);

    int duration = parseFolder(allAlbums->getResourceId(), allAlbums);
    QVERIFY(allAlbums->isDiscovered() == true);
    QVERIFY(allAlbums->getSystemName() == "Album");
    QVERIFY(allAlbums->getChildren().size() >= 1000);
    qWarning() << "PERFO" << duration << allAlbums->getSystemName() << allAlbums->getChildrenSize() << "children";
    QVERIFY2(duration < 200, QString("Parse all albums in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByAlbum() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allAlbums = static_cast<DlnaCachedFolder*>(folder->getChildren().at(1));
    QVERIFY(allAlbums->getSystemName() == "Album");
    QVERIFY(allAlbums->getChildren().size() >= 1000);

    int max = 0;
    foreach(DlnaResource* album, allAlbums->getChildren()) {
        int elapsed;
        elapsed = parseFolder(album->getResourceId(), allAlbums);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << album->getResourceId() << album->getSystemName() << album->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 310, QString("Parse all tracks by album in %1 ms").arg(max).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllGenres() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allGenres = static_cast<DlnaCachedFolder*>(folder->getChildren().at(2));
    QVERIFY(allGenres->isDiscovered() == false);

    int duration = parseFolder(allGenres->getResourceId(), allGenres);
    QVERIFY(allGenres->isDiscovered() == true);
    QVERIFY(allGenres->getSystemName() == "Genre");
    QVERIFY(allGenres->getChildren().size() >= 100);
    qWarning() << "PERFO" << duration << allGenres->getSystemName() << allGenres->getChildrenSize() << "children";
    QVERIFY2(duration < 100, QString("Parse all genres in %1 ms").arg(duration).toUtf8());
}

void tst_dlnacachedresources::testCase_PerformanceAllTracksByGenre() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, "host", 600, this);

    DlnaCachedFolder* folder = static_cast<DlnaCachedFolder*>(rootFolder.getChildren().at(0));
    QVERIFY(folder->getSystemName() == "Music");

    DlnaCachedFolder* allGenres = static_cast<DlnaCachedFolder*>(folder->getChildren().at(2));
    QVERIFY(allGenres->getSystemName() == "Genre");
    QVERIFY(allGenres->getChildren().size() >= 100);

    int duration = parseFolder(allGenres->getResourceId(), allGenres);
    qWarning() << "PERFO" << duration << allGenres->getSystemName() << allGenres->getChildrenSize() << "children";
    QVERIFY2(duration < 200, QString("Parse all genres in %1 ms").arg(duration).toUtf8());

    int max = 0;
    foreach(DlnaResource* genre, allGenres->getChildren()) {
        int elapsed;
        elapsed = parseFolder(genre->getResourceId(), allGenres);
        if (elapsed > max) {
            max = elapsed;
            qWarning() << "PERFO" << elapsed << genre->getResourceId() << genre->getSystemName() << genre->getChildrenSize() << "children";
        }
    }
    QVERIFY2(max < 310, QString("Parse all tracks by genre in %1 ms").arg(max).toUtf8());
}
