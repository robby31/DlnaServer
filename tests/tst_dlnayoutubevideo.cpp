#include "tst_dlnayoutubevideo.h"

tst_dlnayoutubevideo::tst_dlnayoutubevideo(QObject *parent) :
    QObject(parent),
    transcodedSize(0),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE"))
{
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
    db.open();
}

void tst_dlnayoutubevideo::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
}

void tst_dlnayoutubevideo::LogMessage(const QString &message)
{
    qInfo() << message;
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("360p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY2(video.getSystemName() == "https://www.youtube.com/watch?v=JrlfFTS9kGU", video.getSystemName().toUtf8().constData());
    QVERIFY2(video.getName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]", video.getName().toUtf8().constData());
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");
    QVERIFY2(video.metaDataDuration() == 193678, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x360", QString("%1").arg(video.resolution()).toUtf8());
    QVERIFY2(video.framerate() == "25.000", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 15184489, QString("%1").arg(video.sourceSize()).toUtf8());

//    QHash<QString, double> result = video->volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
//    QVERIFY2(result["n_samples"] == 17082368, QString("%1").arg(result["n_samples"]).toUtf8());
//    QVERIFY2(result["mean_volume"] == -9.8, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
//    QVERIFY2(result["histogram_0db"] == 110089, QString("%1").arg(result["histogram_0db"]).toUtf8());

    qInfo() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video.isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_HD()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("720p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=JrlfFTS9kGU");
    QVERIFY2(video.getName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]", video.getName().toUtf8().constData());
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");
    QVERIFY2(video.metaDataDuration() == 193520, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "1280x720", QString("%1").arg(video.resolution()).toUtf8());
    QVERIFY2(video.framerate() == "25.000", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 27503818, QString("%1").arg(video.sourceSize()).toUtf8());


    qInfo() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video.isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_HD_NotFound()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("720p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=RQlXgAR0F4Y"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=RQlXgAR0F4Y");
    QVERIFY2(video.getName() == "Lilly Wood & The Prick en concert privé Le Mouv'", video.getName().toUtf8().constData());
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick en concert privé Le Mouv'");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick en concert privé Le Mouv'");
    QVERIFY2(video.metaDataDuration() == 3671190, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x356", QString("%1").arg(video.resolution()).toUtf8());
    QVERIFY2(video.framerate() == "25.000", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 279698593, QString("%1").arg(video.sourceSize()).toUtf8());


    qInfo() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video.isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_MPEG2()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("360p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=l9kqU_7-CgI"));

    bool res = video.waitUrl(15000);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL";
    video.setSinkProtocol(sinkProtocol);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=l9kqU_7-CgI");
    QVERIFY2(video.getName() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]", video.getName().toUtf8());
    QVERIFY(video.getDisplayName() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]");

    QVERIFY(video.metaDataTitle() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]");
    QVERIFY2(video.metaDataDuration() == 258483, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x360", QString("%1").arg(video.resolution()).toUtf8());
    QVERIFY2(video.framerate() == "29.970", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 10239458, QString("%1").arg(video.sourceSize()).toUtf8());

    QVERIFY(video.toTranscode() == true);
    QVERIFY2(video.getLengthInMilliSeconds()==258483, QString("%1").arg(video.getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video.size()==159772554, QString("%1").arg(video.size()).toUtf8());

    qInfo() << "test done in" << timer.elapsed() << "ms.";

    // test transcoding
    Device *device = video.getStream();
    QVERIFY(device != Q_NULLPTR);

    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qInfo() << "DELTA" << video.size()-transcodedSize << qAbs(double(video.size()-transcodedSize))/video.size();
    QVERIFY(video.size() > transcodedSize);
    QVERIFY2(transcodedSize == 159732696, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

    QVERIFY(video.getdlnaOrgOpFlags() == "10");
    QVERIFY(video.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY2(video.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1", video.getProtocolInfo().toUtf8());

    QVERIFY(video.getAlbumArt().isNull() == true);
    QVERIFY(video.getByteAlbumArt().isNull() == true);

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_MPEG4()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("360p");
    video.setTranscodeFormat(H264_AAC);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=l9kqU_7-CgI"));

    bool res = video.waitUrl(15000);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:video/mp4:DLNA.ORG_PN=MPEG_PS_PAL";
    video.setSinkProtocol(sinkProtocol);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=l9kqU_7-CgI");
    QVERIFY2(video.getName() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]", video.getName().toUtf8());
    QVERIFY(video.getDisplayName() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]");

    QVERIFY(video.metaDataTitle() == "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]");
    QVERIFY2(video.metaDataDuration() == 258483, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY(video.resolution() == "640x360");
    QVERIFY2(video.framerate() == "29.970", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 2500000, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mp4", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 10239458, QString("%1").arg(video.sourceSize()).toUtf8());

    QVERIFY(video.toTranscode() == true);
    QVERIFY2(video.getLengthInMilliSeconds()==258483, QString("%1").arg(video.getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video.size()==89257410, QString("%1").arg(video.size()).toUtf8());

//    QHash<QString, double> result = video->volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
//    QVERIFY2(result["n_samples"] == 22908928, QString("%1").arg(result["n_samples"]).toUtf8());
//    QVERIFY2(result["mean_volume"] == -12.9, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
//    QVERIFY2(result["histogram_0db"] == 15923, QString("%1").arg(result["histogram_0db"]).toUtf8());
//    QVERIFY2(result["histogram_1db"] == 43250, QString("%1").arg(result["histogram_1db"]).toUtf8());

    qInfo() << "test done in" << timer.elapsed() << "ms.";

    // test transcoding
    Device *device = video.getStream();
    QVERIFY(device != Q_NULLPTR);

    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qInfo() << "DELTA" << video.size()-transcodedSize << qAbs(double(video.size()-transcodedSize))/video.size();
    QVERIFY(video.size() > transcodedSize);
    QVERIFY2(transcodedSize == 88751604, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

    QVERIFY(video.getdlnaOrgOpFlags() == "10");
    QVERIFY(video.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY2(video.getProtocolInfo() == "http-get:*:video/mp4:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1", video.getProtocolInfo().toUtf8());

    QVERIFY(video.getAlbumArt().isNull() == true);
    QVERIFY(video.getByteAlbumArt().isNull() == true);

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo3()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("360p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("http://www.youtube.com/watch?v=cXxwIZwYnok"));
    bool res = video.waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=cXxwIZwYnok");
    QVERIFY(video.getName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY2(video.metaDataDuration() == 220216, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY(video.resolution() == "640x360");
    QVERIFY2(video.framerate() == "25.000", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 15729501, QString("%1").arg(video.sourceSize()).toUtf8());

    qInfo() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo4()
{
    QElapsedTimer timer;
    timer.start();

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);
    video.setPlaybackQuality("360p");
    video.setTranscodeFormat(MPEG2_AC3);
    video.setUrl(QUrl("https://www.youtube.com/watch?v=cmSYV5A9iuU"));
    bool res = video.waitUrl(30000);

    qint64 duration = timer.elapsed();
    qInfo() << "created in" << duration << "ms.";
    QVERIFY(video.getSystemName() == "https://www.youtube.com/watch?v=cmSYV5A9iuU");
    QVERIFY2(video.getName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)", video.getName().toUtf8());
    QVERIFY(video.getDisplayName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");

    QVERIFY(video.metaDataTitle() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");
    QVERIFY2(video.metaDataDuration() == 22023221, QString("%1").arg(video.metaDataDuration()).toUtf8());
    QVERIFY2(video.resolution() == "640x360", video.resolution().toUtf8());
    QVERIFY2(video.framerate() == "24.000", QString("%1").arg(video.framerate()).toUtf8());
    QVERIFY2(video.bitrate() == 4558800, QString("%1").arg(video.bitrate()).toUtf8());
    QVERIFY2(video.samplerate() == 44100, QString("%1").arg(video.samplerate()).toUtf8());
    QVERIFY2(video.channelCount() == 2, QString("%1").arg(video.channelCount()).toUtf8());

    QVERIFY2(video.metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video.metaDataFormat()).toUtf8());
    QVERIFY2(video.mimeType() == "video/mpeg", QString("%1").arg(video.mimeType()).toUtf8());
    QVERIFY2(video.sourceSize() == 493113064, QString("%1").arg(video.sourceSize()).toUtf8());

    qInfo() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY2(duration < 15000, QString("Duration: %1").arg(duration).toUtf8());
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo_checkLink_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("is_reachable");

    DlnaCachedRootFolder rootFolder(this);

    QSqlQuery query = rootFolder.getAllNetworkLinks();
    while (query.next())
    {
        QUrl url = query.value("filename").toString();
        bool is_reachable = query.value("is_reachable").toBool();
        QString title = query.value("title").toString();
        QTest::newRow(title.toUtf8()) << url << is_reachable;
    }
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo_checkLink()
{
    QFETCH(QUrl, url);

    DlnaYouTubeVideo video;
    video.setNetworkAccessManager(&manager);

    bool res = false;

    QBENCHMARK
    {
        video.setUrl(url);
        res = video.waitUrl(60000);
    }

    if (!res || !video.isValid())
    {
        if (res && !video.unavailableMessage().isEmpty())
        {
            qInfo() << "VIDEO NOT AVAILABLE" << url << video.unavailableMessage();
            ++media_not_available;
        }
        else if (res && !video.isValid())
        {
            qInfo() << "VIDEO INVALID" << url << video.metaDataTitle() << video.metaDataDuration() << video.getLengthInMilliSeconds() << video.metaDataBitrate() << video.resolution();
            ++media_ko;
        }
        else
        {
            qInfo() << "ERROR TIMEOUT" << url;
            ++media_timeout;
        }
    }
    else
    {
        qDebug() << "OK" << url << res << video.isValid() << video.metaDataTitle() << video.metaDataDuration() << video.getLengthInMilliSeconds() << video.metaDataBitrate();
        ++media_ok;
    }
}

void tst_dlnayoutubevideo::cleanupTestCase()
{
    qInfo() << media_ok << "MEDIAS OK.";
    qInfo() << media_ko << "MEDIAS INVALID.";
    qInfo() << media_not_available << "MEDIAS NOT AVAILABLE.";
    qInfo() << media_timeout << "MEDIAS TIMEOUT.";

    DecryptYoutubeSignature::print_cache();
}
