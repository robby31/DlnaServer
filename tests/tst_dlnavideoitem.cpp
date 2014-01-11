#include "tst_dlnavideoitem.h"

tst_dlnavideoitem::tst_dlnavideoitem(QObject *parent) :
    QObject(parent)
{
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI() {

    Logger log;
    DlnaVideoItem movie(&log, "/Users/doudou/Movies/Films/PLAYTIME (restored version 2002)_[Jacques_Tati]-1967.avi", "host", 600);
    movie.setTranscodeFormat(MP3);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/PLAYTIME (restored version 2002)_[Jacques_Tati]-1967.avi");

    QStringList properties;
    properties << "upnp:genre";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@resolution";
    properties << "res@nrAudioChannels";
    properties << "res@sampleFrequency";

    QDomDocument xml_res;
    xml_res.appendChild(movie.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "PLAYTIME (restored version 2002)_[Jacques_Tati]-1967");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//PLAYTIME+(restored+version+2002)_[Jacques_Tati]-1967.avi");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 8);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/avi:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "1816807032");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:59:11");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "254056");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "720x384");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie.toTranscode() == false);
    QVERIFY(movie.mimeType() == "video/avi");
    QVERIFY(movie.size() == 1816807032);
    QVERIFY(movie.bitrate() == 2032450);
    QVERIFY(movie.getLengthInSeconds() == 7151);
    QVERIFY(movie.getLengthInMilliSeconds() == 7151200);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "720x384");

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    QIODevice* stream = movie.getStream();
    QVERIFY(stream != 0);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 1816807032);
    QProcess* transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess == 0);
    delete range;
    range = 0;
    delete stream;
    stream = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "01");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/avi:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV() {

    Logger log;
    DlnaVideoItem movie(&log, "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv", "host", 600);
    movie.setTranscodeFormat(MP3);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");

    QStringList properties;
    properties << "upnp:genre";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@resolution";
    properties << "res@nrAudioChannels";
    properties << "res@sampleFrequency";

    QDomDocument xml_res;
    xml_res.appendChild(movie.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "District 9 - YIFY");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//District.9.2009.720p.BrRip.YIFY.mkv");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 8);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/x-matroska:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "733723671");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:52:16");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "108929");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "1280x688");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie.toTranscode() == false);
    QVERIFY(movie.mimeType() == "video/x-matroska");
    QVERIFY(movie.size() == 733723671);
    QVERIFY(movie.bitrate() == 871428);
    QVERIFY(movie.getLengthInSeconds() == 6736);
    QVERIFY(movie.getLengthInMilliSeconds() == 6735830);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "1280x688");

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    QIODevice* stream = movie.getStream();
    QVERIFY(stream != 0);
    QVERIFY(stream->isOpen() == true);
    QVERIFY(stream->size() == 733723671);
    QProcess* transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess == 0);
    delete range;
    range = 0;
    delete stream;
    stream = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "01");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/x-matroska:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

