#include "tst_dlnavideoitem.h"

tst_dlnavideoitem::tst_dlnavideoitem(QObject *parent) :
    QObject(parent),
    transcodeProcess(0)
{
}

void tst_dlnavideoitem::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        QByteArray bytes = transcodeProcess->readAllStandardOutput();
        transcodedSize += bytes.size();
    }
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI_Starwars() {

    Logger log;
    DlnaVideoFile movie(&log, "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.EpisodeIII.La.Revanche.Des.S.avi", "host", 600);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.EpisodeIII.La.Revanche.Des.S.avi");

    QVERIFY(movie.framerate() == "23.976");

    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(0, 0, -1);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize >= 2000000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY2(movie.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10", movie.getProtocolInfo().toUtf8());

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV_Looper() {

    Logger log;
    DlnaVideoFile movie(&log, "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv", "host", 600);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv");

    QVERIFY(movie.framerate() == "25.000");

    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(0, 0, -1);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize >= 1400000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY2(movie.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10", movie.getProtocolInfo().toUtf8());

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI() {

    Logger log;
    DlnaVideoFile movie(&log, "/Users/doudou/Movies/Films/PLAYTIME (restored version 2002)_[Jacques_Tati]-1967.avi", "host", 600);
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
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 6);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
//    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "1816807032");  // original size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:59:11");
//    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "254056");   // original bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "720x384");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie.toTranscode() == true);
    QVERIFY(movie.mimeType() == "video/mpeg");
//    QVERIFY(movie.size() == 4869967200);
//    QVERIFY(movie.bitrate() == 5448000);
    QVERIFY(movie.size() == -1);
    QVERIFY(movie.bitrate() == -1);
    QVERIFY(movie.getLengthInSeconds() == 7151);
    QVERIFY(movie.getLengthInMilliSeconds() == 7151200);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "720x384");
    QVERIFY(movie.audioLanguages() == QStringList() << "");
    QVERIFY(movie.subtitleLanguages() == QStringList());
    QVERIFY(movie.framerate() == "25.000");

    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(0, 0, -1);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize >= 3000000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV() {

    Logger log;
    DlnaVideoFile movie(&log, "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv", "host", 600);
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
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "District.9.2009.720p.BrRip.YIFY");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//District.9.2009.720p.BrRip.YIFY.mkv");
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

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

    QVERIFY(movie.toTranscode() == true);
    QVERIFY(movie.mimeType() == "video/mpeg");
//    QVERIFY(movie.size() == 4587100230);
//    QVERIFY(movie.bitrate() == 5448000);
    QVERIFY(movie.size() == -1);
    QVERIFY(movie.bitrate() == -1);
    QVERIFY(movie.getLengthInSeconds() == 6736);
    QVERIFY(movie.getLengthInMilliSeconds() == 6735830);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "1280x688");
    QVERIFY(movie.audioLanguages() == QStringList() << "");
    QVERIFY(movie.subtitleLanguages() == QStringList() << "English");
    QVERIFY(movie.framerate() == "23.976");

    // test partial transcoding (10 seconds)
    QVERIFY(movie.getStream() == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 733723671);
    transcodeProcess = movie.getTranscodeProcess(0, 0, 10);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize <= 3000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    // test full transcoding
    QVERIFY(movie.getStream() == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 733723671);
    transcodeProcess = movie.getTranscodeProcess(0, 0, -1);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    QVERIFY2(transcodedSize >= 4500000000, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;
}

