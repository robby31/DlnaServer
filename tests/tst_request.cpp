#include "tst_request.h"
#include <QtXml>
#include <QTcpSocket>

TestRequest::TestRequest():
    transcodeProcess(0)
{
}

void TestRequest::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        QByteArray bytes = transcodeProcess->readAllStandardOutput();
        transcodedBytes.append(bytes);
    }
}

void TestRequest::testCase_httprange() {
    HttpRange* range = 0;

    // range null
    range = new HttpRange();
    QVERIFY(range->isNull() == true);
    QVERIFY(range->getUnit() == "");
    QVERIFY(range->getLowRange() == 0);
    QVERIFY(range->getHighRange() == 0);
    QVERIFY(range->getStartByte(500) == -1);
    QVERIFY(range->getEndByte(500) == -1);
    QVERIFY(range->getLength(500) == -1);
    delete range;

    // from 10 to 500
    range = new HttpRange("RANGE: BYTES=10-500");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 10);
    QVERIFY(range->getHighRange() == 500);
    QVERIFY(range->getStartByte(1000) == 10);
    QVERIFY(range->getStartByte(3) == -1);
    QVERIFY(range->getEndByte(1000) == 500);
    QVERIFY(range->getEndByte(10) == 9);
    QVERIFY(range->getLength(1000) == 491);
    QVERIFY(range->getLength(400) == 390);
    QVERIFY(range->getLength(9) == -1);
    QVERIFY(range->getLength(0) == -1);
    delete range;

    // from 20 to the end
    range = new HttpRange("RANGE: BYTES=20-");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 20);
    QVERIFY(range->getHighRange() == -1);
    QVERIFY(range->getStartByte(1000) == 20);
    QVERIFY(range->getStartByte(3) == -1);
    QVERIFY(range->getEndByte(1000) == 999);
    QVERIFY(range->getEndByte(10) == 9);
    QVERIFY(range->getLength(1000) == 980);
    delete range;

    // the final 900 bytes
    range = new HttpRange("RANGE: BYTES=-900");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == -1);
    QVERIFY(range->getHighRange() == 900);
    QVERIFY(range->getStartByte(1000) == 100);
    QVERIFY(range->getStartByte(3) == 0);
    QVERIFY(range->getEndByte(1000) == 999);
    QVERIFY(range->getEndByte(10) == 9);
    QVERIFY(range->getLength(1000) == 900);
    QVERIFY(range->getLength(100) == 100);
    delete range;

    // the first byte
    range = new HttpRange("RANGE: BYTES=0-0");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 0);
    QVERIFY(range->getHighRange() == 0);
    QVERIFY(range->getStartByte(1000) == 0);
    QVERIFY(range->getStartByte(3) == 0);
    QVERIFY(range->getEndByte(1000) == 0);
    QVERIFY(range->getEndByte(10) == 0);
    QVERIFY(range->getLength(1000) == 1);
    delete range;

    // from 110 to 5000
    range = new HttpRange("RANGE: BYTES=110-5000");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 110);
    QVERIFY(range->getHighRange() == 5000);
    QVERIFY(range->getStartByte(1000) == 110);
    QVERIFY(range->getStartByte(3) == -1);
    QVERIFY(range->getEndByte(1000) == 999);
    QVERIFY(range->getEndByte(10) == 9);
    delete range;

    // invalid range
    range = new HttpRange("RANGE: BYTES=500-10");
    QVERIFY(range->isNull() == true);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == -1);
    QVERIFY(range->getHighRange() == -1);
    QVERIFY(range->getStartByte(500) == -1);
    QVERIFY(range->getEndByte(500) == -1);
    delete range;
}

void TestRequest::testCase_DlnaRootFolder()
{
    Logger log;
    DlnaRootFolder rootFolder(&log, "host", 100);
    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getParent() == 0);
    QVERIFY(rootFolder.getParentId() == "-1");
    QVERIFY(rootFolder.getResourceId() == "0");
    QVERIFY(rootFolder.isFolder() == true);
    QVERIFY(rootFolder.isDiscovered() == true);
    QVERIFY(rootFolder.getUpdateId() == 1);
    QVERIFY(rootFolder.getChildren().isEmpty() == true);
    QVERIFY(rootFolder.mimeType() == 0);
    QVERIFY(rootFolder.getdlnaOrgOpFlags() == "01");
    QVERIFY(rootFolder.getdlnaOrgPN() == "");

    QStringList properties;
    properties << "dc:title";
    properties << "@childCount";

    QDomDocument xml_res;
    xml_res.appendChild(rootFolder.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    QDomNode node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "root");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    DlnaFolder music(&log, "/Users/doudou/Music/iTunes/iTunes Media/Music", "host", 500);
    QVERIFY(music.getName() == "Music");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "608");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Music");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    rootFolder.addChild(&music);
    QVERIFY(music.getId() == "1");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$1");
    QVERIFY(music.isFolder() == true);
    QVERIFY(rootFolder.getChildren().isEmpty() == false);
    QVERIFY(rootFolder.getChildren().size() == 1);
    QVERIFY(music.mimeType() == 0);
    QVERIFY(music.getdlnaOrgOpFlags() == "01");
    QVERIFY(music.getdlnaOrgPN() == "");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$1");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "608");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Music");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    rootFolder.addChild(&music);
    QVERIFY(music.getId() == "2");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");;
    QVERIFY(music.getParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$2");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.mimeType() == 0);

    DlnaResource* found;
    found = rootFolder.search("0", "");
    QVERIFY(found == &rootFolder);

    found = rootFolder.search("2", "");
    QVERIFY(found == 0);

    found = rootFolder.search("0$2", "");
    QVERIFY(found == &music);

    found = rootFolder.search("0$0", "");
    QVERIFY(found == 0);

    QList<DlnaResource*> list_found;
    list_found = rootFolder.getDLNAResources("0", false, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 1);
    QVERIFY(list_found.at(0)->getResourceId() == "0");

    list_found = rootFolder.getDLNAResources("0", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 2);
    QVERIFY(list_found.at(0)->getResourceId() == "0$2");
    QVERIFY(list_found.at(1)->getResourceId() == "0$2");

    list_found = music.getDLNAResources("0$2", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    list_found = music.getDLNAResources("0$2$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    list_found = music.getDLNAResources("0$2$1$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a");
    QVERIFY(list_found.at(0)->getdlnaOrgOpFlags() == "01");
    QVERIFY(list_found.at(0)->getdlnaOrgPN() == "MP3");
    QVERIFY(list_found.at(0)->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(list_found.at(0)->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    list_found.at(0)->setdlnaOrgPN(QString());
    QVERIFY(list_found.at(0)->getdlnaOrgOpFlags() == "01");
    QVERIFY(list_found.at(0)->getdlnaOrgPN() == "");
    QVERIFY(list_found.at(0)->getDlnaContentFeatures() == "DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(list_found.at(0)->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_OP=01");

    int res;
    res = rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Video");
    QVERIFY(res == false);

    res = rootFolder.addFolder("/Users/doudou/Movies");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().isEmpty() == false);
    QVERIFY(rootFolder.getChildren().size() == 3);

    found = rootFolder.search("0$3", "");
    QVERIFY(found != 0);
    QVERIFY2(found->getSystemName() == "/Users/doudou/Movies", found->getSystemName().toUtf8());

}

void TestRequest::testCase_DlnaMusicTrack_AAC() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspace/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a", "host", 600);
    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspace/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

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


    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getByteAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    QVERIFY(track.getByteAlbumArt().size() == 25089);

}

void TestRequest::testCase_DlnaMusicTrack_MP3() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspace/DLNA_server/tests/AUDIO/07 On_Off.mp3", "host", 600);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspace/DLNA_server/tests/AUDIO/07 On_Off.mp3");

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

    HttpRange* range;
    range = new HttpRange("RANGE: BYTES=0-");
    QVERIFY(track.getStream(range).size() == 376593);
    QVERIFY(track.getStream(range).size() == 376593);
    delete range;

    range = new HttpRange("RANGE: BYTES=93-");
    QVERIFY(track.getStream(range).size() == 376500);
    delete range;

    range = new HttpRange("RANGE: BYTES=3190950-");
    QVERIFY(track.getStream(range).size() == 0);
    delete range;

    range = new HttpRange("RANGE: BYTES=14-1000");
    QVERIFY(track.getStream(range).size() == 987);
    delete range;

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);
}

void TestRequest::testCase_DlnaMusicTrack_MP3_with_image() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspace/DLNA_server/tests/AUDIO/16 Funk Ad.mp3", "host", 600);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspace/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");

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

    HttpRange* range;
    range = new HttpRange("RANGE: BYTES=0-");
    QVERIFY(track.getStream(range).size() == 845029);
    QVERIFY(track.getStream(range).size() == 845029);
    delete range;

    range = new HttpRange("RANGE: BYTES=29-");
    QVERIFY(track.getStream(range).size() == 845000);
    delete range;

    range = new HttpRange("RANGE: BYTES=845050-");
    QVERIFY(track.getStream(range).size() == 0);
    delete range;

    range = new HttpRange("RANGE: BYTES=14-1000");
    QVERIFY(track.getStream(range).size() == 987);
    delete range;

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 306);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    QVERIFY(track.getByteAlbumArt().size() == 22568);
}

void TestRequest::testCase_DlnaMusicTrack_WAV() {

    Logger log;
    DlnaMusicTrack track(&log, "/Users/doudou/workspace/DLNA_server/tests/AUDIO/test.wav", "host", 600);
    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspace/DLNA_server/tests/AUDIO/test.wav");

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

    // Test getStream and getTranscodeProcess
    HttpRange* range = 0;
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 10002274);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=21886280-");
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    track.setTranscodeFormat(LPCM);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspace/DLNA_server/tests/AUDIO/test.wav");

    xml_res.clear();
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    node = xml_res.elementsByTagName("item").at(0);
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

    // Test getStream and getTranscodeProcess
    range = 0;
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
    transcodeProcess = track.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedBytes.clear();
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY(transcodedBytes.size() == 92782592);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    range = new HttpRange("RANGE: BYTES=105568500-");
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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
    QVERIFY(track.getStream(range).isNull() == true);  // returns null because it shall be transcoded
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

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01");

}

void TestRequest::testCase_DlnaFolder()
{
    Logger log;
    DlnaFolder music(&log, "/Users/doudou/Music/iTunes/iTunes Media/Music", "host", 300);
    QVERIFY(music.getId() == "");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDisplayName() == "Music");
    QVERIFY(music.getParent() == 0);
    QVERIFY(music.getParentId() == "-1");
    QVERIFY(music.getResourceId() == "");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.isDiscovered() == false);
    QVERIFY(music.getUpdateId() == 1);
    QVERIFY2(music.getChildren().isEmpty() == false, QString("%1").arg(music.getChildren().isEmpty()).toUtf8());
    QVERIFY(music.getChildren().size() == 608);

    music.setId("0$1");
    QVERIFY(music.getId() == "0$1");

    QList<DlnaResource*> list_found;
    list_found = music.getDLNAResources("0$1", false, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 1);

    list_found = music.getDLNAResources("0$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY2(list_found.size() == 10, QString("%1").arg(list_found.size()).toUtf8());
    QVERIFY(list_found.at(0)->getResourceId() == "0$1$1");
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-");

    list_found = music.getDLNAResources("0$1$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY2(list_found.size() == 2, QString("%1").arg(list_found.size()).toUtf8());
    QVERIFY(list_found.at(0)->getResourceId() == "0$1$1$1");
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime");

    list_found = music.getDLNAResources(QString(), true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == true);

    list_found = music.getDLNAResources("", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == true);

    list_found = music.getDLNAResources("10000", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == true);

}

void TestRequest::testCase_request()
{
    Logger log;
    DlnaRootFolder rootFolder(&log, "host", 400);
    QTcpSocket client;
    Request request(&log, &client, "uuid", "server name", "host IP", 56, &rootFolder);

    QVERIFY(request.getHighRange() == -1);
    QVERIFY(request.getLowRange() == -1);
    QVERIFY2(request.getHost() == "host IP", "Failure with function getHost()");
    QVERIFY(request.getArgument() == "");
    QVERIFY(request.getMethod() == "");
    QVERIFY(request.getpeerAddress() == "");
    QVERIFY(request.getStatus() == "init");
    QVERIFY(request.getTextHeader() == "");
    QVERIFY(request.getTextContent() == "");
    QVERIFY(request.isHttp10() == false);
    QVERIFY(request.getUserAgent() == "");
    QVERIFY(request.getReceivedContentLength() == -1);
    QVERIFY(request.getTransferMode() == "");
    QVERIFY(request.getContentFeatures() == "");

    QVERIFY(request.getSoapaction() == "");
    QVERIFY(request.appendHeader("CALLBACK: field_callback") == true);
    QVERIFY(request.getSoapaction() == "field_callback");

    QVERIFY(request.appendHeader("SOAPACTION: field_soapaction") == true);
    QVERIFY(request.getSoapaction() == "field_soapaction");

    QVERIFY(request.appendHeader("") == false);
    QVERIFY(request.appendHeader("METHOD") == false);

    QVERIFY(request.appendHeader("GET /get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3 HTTP/1.0") == true);
    QVERIFY(request.getMethod() == "GET");
    QVERIFY(request.getArgument() == "get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3");
    QVERIFY(request.isHttp10() == true);

    QVERIFY(request.appendHeader("RANGE: BYTES=10-500") == true);
    QVERIFY(request.getLowRange() == 10);
    QVERIFY(request.getHighRange() == 500);

    QVERIFY(request.appendHeader("User-Agent: DLNADOC/1.50") == true);
    QVERIFY(request.getUserAgent() == "DLNADOC/1.50");

    QVERIFY(request.appendHeader("CONTENT-LENGTH: 632") == true);
    QVERIFY(request.getReceivedContentLength() == 632);

    QVERIFY(request.appendHeader("Content-Length: 2122") == true);
    QVERIFY(request.getReceivedContentLength() == 2122);

    QVERIFY(request.appendHeader("transferMode.dlna.org: Interactive ") == true);
    QVERIFY(request.getTransferMode() == "Interactive");

    QVERIFY(request.appendHeader("getcontentFeatures.dlna.org:1") == true);
    QVERIFY(request.getContentFeatures() == "1");

    QVERIFY(request.appendHeader("Host: 192.168.1.103:5002") == true);

    QVERIFY(request.appendHeader("ACCEPT-LANGUAGE: en-us") == true);

    QVERIFY(request.appendHeader("Accept-Encoding: gzip, deflate") == true);

    QVERIFY(request.appendHeader("Accept: */*") == true);

    QVERIFY(request.appendHeader("Connection: keep-alive") == true);

    QVERIFY(request.appendHeader("Content-Type: text/xml; charset=\"utf-8\"") == true);

    QVERIFY(request.appendHeader("Timeout: Second-1800") == true);

    QVERIFY(request.appendHeader("Nt: upnp:event") == true);

    QVERIFY(request.appendHeader("Icy-MetaData: 1") == true);

}

void TestRequest::testCase_server() {
    Logger log;
    RequestListModel requestlistmodel;
    HttpServer server(&log, &requestlistmodel);

    /*QVERIFY(server.getHost().toString() == "192.168.1.103");
    QVERIFY(server.getPort() == 5002);
    QVERIFY(server.getURL() == "http://192.168.1.103:5002");

    QTcpSocket request;
    request.connectToHost(server.getHost(), server.getPort());
    qWarning() << "connected:" << request.waitForConnected();

    request.write("GET");
    qWarning() << "ready read" << request.waitForReadyRead();*/

}


QTEST_APPLESS_MAIN(TestRequest)

