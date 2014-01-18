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
    DlnaVideoItem movie(&log, "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.EpisodeIII.La.Revanche.Des.S.avi", "host", 600);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.EpisodeIII.La.Revanche.Des.S.avi");

    QVERIFY(movie.framerate() == "23.976");

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qWarning() << "transcoded size" << transcodedSize;
    QVERIFY(transcodedSize <= movie.size());
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "01");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/avi:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV_Looper() {

    Logger log;
    DlnaVideoItem movie(&log, "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv", "host", 600);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv");

    QVERIFY(movie.framerate() == "25.000");

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qWarning() << "transcoded size" << transcodedSize;
    QVERIFY(transcodedSize <= movie.size());
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    QVERIFY(movie.getdlnaOrgOpFlags() == "01");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/x-matroska:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI() {

    Logger log;
    DlnaVideoItem movie(&log, "/Users/doudou/Movies/Films/PLAYTIME (restored version 2002)_[Jacques_Tati]-1967.avi", "host", 600);
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
//    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "1816807032");  // original size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "4869967200");    // transcoded size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:59:11");
//    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "254056");   // original bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "681000");    // transcoded bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "720x384");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie.toTranscode() == true);
    QVERIFY(movie.mimeType() == "video/avi");
    QVERIFY(movie.size() == 4869967200);
    QVERIFY(movie.bitrate() == 5448000);
    QVERIFY(movie.getLengthInSeconds() == 7151);
    QVERIFY(movie.getLengthInMilliSeconds() == 7151200);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "720x384");
    QVERIFY(movie.audioLanguages() == QStringList() << "");
    QVERIFY(movie.subtitleLanguages() == QStringList());
    QVERIFY(movie.framerate() == "25.000");

    HttpRange* range = 0;
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    QIODevice* stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 1816807032);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qWarning() << "transcoded size" << transcodedSize;
    QVERIFY(transcodedSize <= movie.size());
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

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
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 8);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/x-matroska:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    //QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "733723671");   // original size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "4587100230");     // transcoded size
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "01:52:16");
    //QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "108929");   // original bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "681000");     // transcoded bitrate
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "1280x688");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    xml_res.clear();

    QVERIFY(movie.getdlnaOrgOpFlags() == "01");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/x-matroska:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=01");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

    QVERIFY(movie.toTranscode() == true);
    QVERIFY(movie.mimeType() == "video/x-matroska");
    QVERIFY(movie.size() == 4587100230);
    QVERIFY(movie.bitrate() == 5448000);
    QVERIFY(movie.getLengthInSeconds() == 6736);
    QVERIFY(movie.getLengthInMilliSeconds() == 6735830);
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "1280x688");
    QVERIFY(movie.audioLanguages() == QStringList() << "");
    QVERIFY(movie.subtitleLanguages() == QStringList() << "English");
    QVERIFY(movie.framerate() == "23.976");

    HttpRange* range = 0;
    QIODevice* stream;

    // test partial transcoding (10 seconds)
    range = new HttpRange("RANGE: BYTES=0-6810000");
    range->setSize(movie.size());
    stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 733723671);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qWarning() << "transcoded size" << transcodedSize;
    QVERIFY(transcodedSize <= 3000000);
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;

    // test full transcoding
    range = new HttpRange("RANGE: BYTES=0-");
    range->setSize(movie.size());
    stream = movie.getStream();
    QVERIFY(stream == 0);
//    QVERIFY(stream != 0);
//    QVERIFY(stream->isOpen() == true);
//    QVERIFY(stream->size() == 733723671);
    delete stream;
    stream = 0;
    transcodeProcess = movie.getTranscodeProcess(range);
    QVERIFY(transcodeProcess != 0);
    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
    transcodeProcess->start();
    transcodeProcess->waitForFinished(-1);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qWarning() << "transcoded size" << transcodedSize << transcodeProcess->readAllStandardOutput().size();
    QVERIFY(transcodedSize <= movie.size());
    delete transcodeProcess;
    transcodeProcess = 0;
    delete range;
    range = 0;
}

