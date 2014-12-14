#include "tst_dlnamusictrack.h"

tst_dlnamusictrack::tst_dlnamusictrack(QObject *parent) :
    QObject(parent),
    transcodeProcess(0)
{
}

void tst_dlnamusictrack::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        while (transcodeProcess->isOpen() && transcodeProcess->bytesAvailable()>0)
            transcodedBytes.append(transcodeProcess->read(1024*1024));
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3() {

    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3", "host", 600);
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
    QVERIFY2(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//07%20On_Off.mp3", xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "376593");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:19", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "18751", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 376593);
    QVERIFY2(track.bitrate() == 150005, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 19);
    QVERIFY2(track.getLengthInMilliSeconds() == 19382, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == false);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);

    Device* stream = track.getStream();
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 376593);
    QVERIFY(stream->atEnd() == false);

    QByteArray data;
    int readSize = 0;
    while (!stream->atEnd())
    {
        data = stream->read(1024);
        readSize += data.size();
    }
    QVERIFY2(readSize == 376593, QString("%1").arg(readSize).toUtf8().constData());
    QVERIFY(stream->pos() == 376593);
    delete stream;
    stream = 0;

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    stream = track.getStream(range);
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 376593);
    QVERIFY(stream->atEnd() == false);

    readSize = 0;
    while (!stream->atEnd())
    {
        data = stream->read(1024);
        readSize += data.size();
    }
    QVERIFY2(readSize == 376593, QString("%1").arg(readSize).toUtf8().constData());
    QVERIFY(stream->pos() == 376593);
    delete stream;
    stream = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-5000");
    range->setSize(track.size());
    stream = track.getStream(range);
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY2(stream->size() == 5001, QString("%1").arg(stream->size()).toUtf8().constData());
    QVERIFY(stream->atEnd() == false);

    readSize = 0;
    while (!stream->atEnd())
    {
        data = stream->read(1024);
        readSize += data.size();
    }
    QVERIFY2(readSize == 5001, QString("%1").arg(readSize).toUtf8().constData());
    QVERIFY(stream->pos() == 5001);
    delete stream;
    stream = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=1000-5000");
    range->setSize(track.size());
    stream = track.getStream(range);
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY2(stream->size() == 4001, QString("%1").arg(stream->size()).toUtf8().constData());
    QVERIFY(stream->atEnd() == false);

    readSize = 0;
    while (!stream->atEnd())
    {
        data = stream->read(1024);
        readSize += data.size();
    }
    QVERIFY2(readSize == 4001, QString("%1").arg(readSize).toUtf8().constData());
    QVERIFY(stream->pos() == 5001);
    delete stream;
    stream = 0;
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_with_image() {

    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3", "host", 600);
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
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//16%20Funk%20Ad.mp3");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "845029");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:53", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "16000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 845029);
    QVERIFY(track.bitrate() == 128000);
    QVERIFY2(track.getLengthInSeconds() == 53, QString("%1").arg(track.getLengthInSeconds()).toUtf8());
    QVERIFY2(track.getLengthInMilliSeconds() == 52814, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == false);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    int size = track.getByteAlbumArt().size();
    QVERIFY2(size == 16814, QString("%1").arg(size).toUtf8());

    Device* stream = track.getStream();
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 845029);
    QVERIFY(stream->atEnd() == false);
    delete stream;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_unicode() {

    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3", "host", 600);
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
    QVERIFY2(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//1-02%20Phe%CC%81bus.mp3", xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "3841064");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:02:39");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "23657", QString("%1").arg(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue()).toUtf8());
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 3841064);
    QVERIFY2(track.bitrate() == 189255, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 159);
    QVERIFY2(track.getLengthInMilliSeconds() == 159373, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == false);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    QVERIFY2(track.getByteAlbumArt().size() == 5360, QString("%1").arg(track.getByteAlbumArt().size()).toUtf8());

    Device* stream = track.getStream();
    QVERIFY(stream != 0);
    QVERIFY(stream->open() == true);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 3841064);
    QVERIFY(stream->atEnd() == false);
    delete stream;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_AAC_Transcoding_MP3() {

    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a", "host", 600);
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
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//01%20Monde%20virtuel.m4a");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7559480", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 7559480);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 189);
    QVERIFY2(track.getLengthInMilliSeconds() == 188987, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == true);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getByteAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    int size = track.getByteAlbumArt().size();
    QVERIFY2(size == 22464, QString("%1").arg(size).toUtf8());

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    QVERIFY(transcodeProcess == 0);
    transcodedBytes.clear();
    Device *device = track.getStream(range);
    QVERIFY(device != 0);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    QVERIFY(transcodeProcess != 0);
    QVERIFY(transcodeProcess->atEnd() == false);
    QVERIFY(transcodeProcess->open() == true);
    QVERIFY(transcodeProcess->waitForFinished(-1) == true);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 7560412, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd() == true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_MP3() {
    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav", "host", 600);
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
    QVERIFY2(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test", xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY2(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0", xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue().toUtf8());
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
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "21785040", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.size() == 21785040);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    // Test getStream and getTranscodeProcess
    HttpRange* range = 0;
    Device *device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    QVERIFY(transcodeProcess->atEnd()==false);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 21786284, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 21786284);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-0");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 44, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=11786000-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 10026284, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=21886280-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->open() == true);
    QVERIFY(transcodeProcess->atEnd()==true);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 1);
    QVERIFY2(transcodedBytes.size() == 0, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=14-1000");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 3884, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_LPCM() {
    Logger log;
    DlnaMusicTrackFile track(&log, "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav", "host", 600);
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
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0");
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
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "104568192", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "192000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/L16");
    QVERIFY(track.size() == 104568192);
    QVERIFY(track.bitrate() == 1536000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01");

    // Test getStream and getTranscodeProcess
    HttpRange* range = 0;
    Device *device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 104568320);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 104568320);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=0-0");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedBytes.size() == 4096, QString("%1").arg(transcodedBytes.size()).toUtf8().constData());
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=11786000-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 92856320);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=105568500-");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==true);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 1);
    QVERIFY(transcodedBytes.size() == 0);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=14-1000");
    range->setSize(track.size());
    device = track.getStream(range);
    QVERIFY(device != 0);
    QVERIFY(device->open() == true);
    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    QVERIFY(transcodeProcess->atEnd()==false);
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 4096);
    QVERIFY(transcodeProcess->atEnd()==true);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}
