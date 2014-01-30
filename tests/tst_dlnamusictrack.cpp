#include "tst_dlnamusictrack.h"

tst_dlnamusictrack::tst_dlnamusictrack(QObject *parent) :
    QObject(parent),
    transcodeProcess(0)
{
}

void tst_dlnamusictrack::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        QByteArray bytes = transcodeProcess->readAllStandardOutput();
        transcodedBytes.append(bytes);
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3", "host", 600);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "On/Off");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Human After All");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "House");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "7");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//07+On_Off.mp3");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "376593");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:19");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "18729");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 376593);
    QVERIFY(track.bitrate() == 149832);
    QVERIFY(track.getLengthInSeconds() == 19);
    QVERIFY(track.getLengthInMilliSeconds() == 19382);
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);

    QIODevice* stream = track.getStream();
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 376593);
    delete stream;

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QProcess* transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess == 0);
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_with_image() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3", "host", 600);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Funk Ad");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Homework");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "House");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "16");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//16+Funk+Ad.mp3");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "845029");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:51");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "16000");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 845029);
    QVERIFY(track.bitrate() == 128000);
    QVERIFY(track.getLengthInSeconds() == 51);
    QVERIFY(track.getLengthInMilliSeconds() == 51056);
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 306);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    QVERIFY(track.getByteAlbumArt().size() == 22568);

    QIODevice* stream = track.getStream();
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 845029);
    delete stream;

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QProcess* transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess == 0);
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_unicode() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3", "host", 600);
    QVERIFY(track.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Phébus");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Mister Mystère");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "Rock  Français");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//1-02+Phébus.mp3");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "3841064");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:02:39");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "23654");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 3841064);
    QVERIFY(track.bitrate() == 189234);
    QVERIFY(track.getLengthInSeconds() == 159);
    QVERIFY(track.getLengthInMilliSeconds() == 159373);
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 500);
    QVERIFY(track.getAlbumArt().size().height() == 500);
    QVERIFY(track.getByteAlbumArt().size() == 14448);

    QIODevice* stream = track.getStream();
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 3841064);
    delete stream;

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QProcess* transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess == 0);
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_AAC_Transcoding_MP3() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a", "host", 600);
    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
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
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//01+Monde+virtuel.m4a");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7559480");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 7559480);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 189);
    QVERIFY(track.getLengthInMilliSeconds() == 188987);
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getByteAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    QVERIFY(track.getByteAlbumArt().size() == 25089);

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 7558825);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_MP3() {
    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav", "host", 600);
    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "21785040");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 21785040);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY(track.getLengthInMilliSeconds() == 544626);
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    // Test getStream and getTranscodeProcess
    HttpRange* range = 0;
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 21786274);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 21786274);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-0");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 994);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=11786000-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 10026274);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=21886280-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 1);
    QVERIFY(transcodedBytes.size() == 0);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=14-1000");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 3874);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_LPCM() {
    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav", "host", 600);
    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    track.setTranscodeFormat(LPCM);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

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
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/L16:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "104568192");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "192000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/L16");
    QVERIFY(track.size() == 104568192);
    QVERIFY(track.bitrate() == 1536000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY(track.getLengthInMilliSeconds() == 544626);
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01");

    // Test getStream and getTranscodeProcess
    HttpRange* range = 0;
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 104568320);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 104568320);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-0");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 4096);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=11786000-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 92856320);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=105568500-");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 1);
    QVERIFY(transcodedBytes.size() == 0);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=14-1000");
    range->setSize(track.size());
    QVERIFY(track.getStream() == 0);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 4096);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}
