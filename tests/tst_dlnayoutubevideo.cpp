#include "tst_dlnayoutubevideo.h"

tst_dlnayoutubevideo::tst_dlnayoutubevideo(QObject *parent) :
    QObject(parent),
    transcodeProcess(0),
    transcodedSize(0),
    backend(0),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    manager(0)
{
    backend = new QThread();
    backend->setObjectName("TEST BACKEND");
    connect(this, SIGNAL(destroyed()), backend, SLOT(deleteLater()));
    backend->start();

    manager = new QNetworkAccessManager();
    connect(this, SIGNAL(destroyed()), manager, SLOT(deleteLater()));
    manager->moveToThread(backend);

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
}

void tst_dlnayoutubevideo::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        while (transcodeProcess->isOpen() && transcodeProcess->bytesAvailable()>0)
            transcodedSize += transcodeProcess->read(1024*1024).size();
    }
}


void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo()
{
    Logger log;
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video(&log, "host", 600);
    video.moveToThread(backend);
    video.setNetworkAccessManager(manager);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=JrlfFTS9kGU");
    QVERIFY2(video.getName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]", video.getName().toUtf8().constData());
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");
    QVERIFY2(video.metaDataDuration() == 193491, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x360", QString("%1").arg(video.resolution()).toUtf8());
    QVERIFY2(video.framerate() == "25/1", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4718800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 16060504, QString("%1").arg(video.sourceSize()).toUtf8());


    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video.isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo2()
{
    Logger log;
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video(&log, "host", 600);
    video.moveToThread(backend);
    video.setNetworkAccessManager(manager);
    video.setUrl(QUrl("http://www.youtube.com/watch?v=04QzovLe2JM"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=04QzovLe2JM");
    QVERIFY(video.getName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video.getDisplayName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");

    QVERIFY(video.metaDataTitle() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video.metaDataDuration() == 258459);
    QVERIFY(video.resolution() == "638x360");
    QVERIFY2(video.framerate() == "25/1", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4718800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 14172486, QString("%1").arg(video.sourceSize()).toUtf8());

    QVERIFY(video.toTranscode() == true);
    QVERIFY2(video.getLengthInMilliSeconds()==258459, QString("%1").arg(video.getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video.size()==152452041, QString("%1").arg(video.size()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    // test transcoding
    Device *device = video.getStream(0, 0, -1);
    QVERIFY(device != 0);

    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    QVERIFY(transcodeProcess->open() == true);
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qWarning() << "DELTA" << video.size()-transcodedSize << qAbs(double(video.size()-transcodedSize))/video.size();
    QVERIFY2(transcodedSize == 146022796, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(video.getdlnaOrgOpFlags() == "10");
    QVERIFY(video.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY2(video.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10", video.getProtocolInfo().toUtf8());

    QVERIFY(video.getAlbumArt().isNull() == true);
    QVERIFY(video.getByteAlbumArt().isNull() == true);

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo3()
{
    Logger log;
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video(&log, "host", 600);
    video.moveToThread(backend);
    video.setNetworkAccessManager(manager);
    video.setUrl(QUrl("http://www.youtube.com/watch?v=cXxwIZwYnok"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=cXxwIZwYnok");
    QVERIFY(video.getName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video.metaDataDuration() == 220170);
    QVERIFY(video.resolution() == "640x360");
    QVERIFY2(video.framerate() == "25/1", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4718800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 15729931, QString("%1").arg(video.sourceSize()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo4()
{
    Logger log;
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video(&log, "host", 600);
    video.moveToThread(backend);
    video.setNetworkAccessManager(manager);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=aGHT40qkysw"));
    bool res = video.waitUrl(30000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 15000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=aGHT40qkysw");
    QVERIFY2(video.getName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)", video.getName().toUtf8());
    QVERIFY(video.getDisplayName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");

    QVERIFY(video.metaDataTitle() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");
    QVERIFY2(video.metaDataDuration() == 22022871, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x360", video.resolution().toUtf8());
    QVERIFY2(video.framerate() == "30000/1001", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4718800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 489728772, QString("%1").arg(video.sourceSize()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo() {
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, &db, "host", 600, this);
    rootFolder.setNetworkAccessManager(manager);

    DlnaCachedGroupedFolderMetaData *folder = 0;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getDisplayName() == "YOUTUBE")
            break;
    }

    QVERIFY(folder != 0);
    QVERIFY2(folder->getDisplayName() == "YOUTUBE", folder->getDisplayName().toUtf8().constData());
    QVERIFY2(folder->getChildrenSize() == 3, QString("%1").arg(folder->getChildrenSize()).toUtf8().constData());

    DlnaCachedFolderMetaData *artists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
    QVERIFY2(artists->getDisplayName() == "Artist", artists->getDisplayName().toUtf8().constData());
    QVERIFY2(artists->getChildrenSize() == 6, QString("%1").arg(artists->getChildrenSize()).toUtf8().constData());

    DlnaCachedFolder *no_artist = qobject_cast<DlnaCachedFolder*>(artists->getChild(0));
    QVERIFY2(no_artist->getDisplayName() == "No artist", no_artist->getDisplayName().toUtf8().constData());
    QVERIFY2(no_artist->getChildrenSize() == 80, QString("%1").arg(no_artist->getChildrenSize()).toUtf8().constData());

    DlnaCachedNetworkVideo* movie = 0;
    for (int index=0;index<no_artist->getChildrenSize();++index)
    {
        movie = qobject_cast<DlnaCachedNetworkVideo*>(no_artist->getChild(index));
        qWarning() << movie->getSystemName() << movie->getDisplayName();
        if (movie && movie->getSystemName() == "http://www.youtube.com/watch?v=X3hJYDGSXt4")
            break;
    }

    QVERIFY(movie != 0);
    QVERIFY(movie->getSystemName() == "http://www.youtube.com/watch?v=X3hJYDGSXt4");

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
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Cats on trees \"Sirens call\" [Clip Officiel]");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.videoItem");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY2(!xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().isEmpty(), xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 8);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:17", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "640x360", xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2", xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "44100", xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "589850", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8().constData());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "116088968", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8().constData());
    xml_res.clear();

    QVERIFY(movie->getdlnaOrgOpFlags() == "10");
    QVERIFY(movie->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(movie->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10");

    QVERIFY(movie->getAlbumArt().isNull() == true);
    QVERIFY(movie->getByteAlbumArt().isNull() == true);

    QVERIFY(movie->toTranscode() == true);
    QVERIFY2(movie->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(movie->metaDataFormat()).toUtf8());
    QVERIFY(movie->mimeType() == "video/mpeg");
    QVERIFY(movie->size() == 116088968);
    QVERIFY(movie->bitrate() == 4718800);
    QVERIFY(movie->getLengthInSeconds() == 197);
    QVERIFY2(movie->getLengthInMilliSeconds() == 196811, QString("%1").arg(movie->getLengthInMilliSeconds()).toUtf8());
    QVERIFY(movie->samplerate() == 44100);
    QVERIFY(movie->channelCount() == 2);
    QVERIFY(movie->resolution() == "640x360");
    QVERIFY2(movie->audioLanguages() == QStringList() << "", movie->audioLanguages().join(',').toUtf8());
    QVERIFY2(movie->subtitleLanguages() == QStringList() << "", movie->subtitleLanguages().join(',').toUtf8());
    QVERIFY2(movie->framerate() == "25/1", movie->framerate().toUtf8());

    QElapsedTimer timer;
    timer.start();
    Device *device = movie->getStream(0);
    qWarning() << "stream created in" << timer.elapsed() << "ms.";
    QVERIFY(device != 0);

    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(receivedTranscodedData()));
    QVERIFY(transcodeProcess->open() == true);
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qWarning() << "DELTA" << movie->size()-transcodedSize << qAbs(double(movie->size()-transcodedSize))/movie->size();
    QVERIFY2(transcodedSize == 109758912, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo_checkLink()
{
    Logger log;
    DlnaCachedRootFolder rootFolder(&log, &db, "host", 600, this);

    int min_time = -1;
    int max_time = -1;
    int sum_time = 0;

    int nbOk = 0;
    int nbErrors = 0;
    QSqlQuery query = rootFolder.getAllNetworkLinks();
    DlnaYouTubeVideo *video;
    while (query.next())
    {
        QUrl url = query.value("filename").toString();
        bool is_reachable = query.value("is_reachable").toBool();

        QElapsedTimer timer;
        timer.start();

        video = new DlnaYouTubeVideo(&log, "host", 600);
        video->moveToThread(backend);
        video->setNetworkAccessManager(manager);
        video->setUrl(url);
        bool res = video->waitUrl(60000);

        int duration = timer.elapsed();
        sum_time += duration;
        if (min_time == -1 or min_time > duration)
            min_time = duration;
        if (max_time == -1 or max_time < duration)
            max_time = duration;

        if (!res or !video->isValid())
        {
            if (is_reachable)
                ++nbErrors;

            if (res && !video->unavailableMessage().isEmpty())
                qWarning() << "VIDEO NOT AVAILABLE" << url << res << video->isValid() << video->metaDataTitle() << video->metaDataDuration() << video->getLengthInMilliSeconds() << video->metaDataBitrate();
            else
                qWarning() << "ERROR TIMEOUT" << url;
        }
        else
        {
            ++nbOk;
            qWarning() << "OK" << url << res << video->isValid() << video->metaDataTitle() << video->metaDataDuration() << video->getLengthInMilliSeconds() << video->metaDataBitrate();
        }

        qWarning() << "check done in" << duration << "ms.";

        video->deleteLater();
    }

    qWarning() << "avg_time:" << sum_time/(nbErrors+nbOk) << "min_time:" << min_time << "max_time:" << max_time;
    qWarning() << "OK:" << nbOk << "KO:" << nbErrors;
    QVERIFY2(nbErrors == 0, QString("%1").arg(nbErrors).toUtf8());
}
