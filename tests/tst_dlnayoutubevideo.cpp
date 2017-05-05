#include "tst_dlnayoutubevideo.h"

tst_dlnayoutubevideo::tst_dlnayoutubevideo(QObject *parent) :
    QObject(parent),
    transcodeProcess(0),
    transcodedSize(0),
    backend(0),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE")),
    manager(0)
{
    QFfmpegProcess::setDirPath("/opt/local/bin");
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    backend = new QThread();
    backend->setObjectName("TEST BACKEND");
    connect(this, SIGNAL(destroyed()), backend, SLOT(deleteLater()));
    backend->start();

    manager = new QNetworkAccessManager();
    connect(this, SIGNAL(destroyed()), manager, SLOT(deleteLater()));
    manager->moveToThread(backend);
    connect(backend, SIGNAL(finished()), manager, SLOT(deleteLater()));

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
}

void tst_dlnayoutubevideo::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
    emit bytesSent(transcodedSize, 1);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("hq");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "https://www.youtube.com/watch?v=JrlfFTS9kGU");
    QVERIFY2(video->getName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]", video->getName().toUtf8().constData());
    QVERIFY(video->getDisplayName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");

    QVERIFY(video->metaDataTitle() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");
    QVERIFY2(video->metaDataDuration() == 193676, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY2(video->resolution() == "640x360", QString("%1").arg(video->resolution()).toUtf8());
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 15960867, QString("%1").arg(video->sourceSize()).toUtf8());

    QHash<QString, double> result = video->volumeInfo(-1);
    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    QVERIFY2(result["n_samples"] == 17082368, QString("%1").arg(result["n_samples"]).toUtf8());
    QVERIFY2(result["mean_volume"] == -9.8, QString("%1").arg(result["mean_volume"]).toUtf8());
    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    QVERIFY2(result["histogram_0db"] == 110089, QString("%1").arg(result["histogram_0db"]).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video->isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_HD()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("720p");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "https://www.youtube.com/watch?v=JrlfFTS9kGU");
    QVERIFY2(video->getName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]", video->getName().toUtf8().constData());
    QVERIFY(video->getDisplayName() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");

    QVERIFY(video->metaDataTitle() == "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]");
    QVERIFY2(video->metaDataDuration() == 193538, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY2(video->resolution() == "1280x720", QString("%1").arg(video->resolution()).toUtf8());
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 39643031, QString("%1").arg(video->sourceSize()).toUtf8());


    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video->isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_HD_NotFound()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("720p");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("https://www.youtube.com/watch?v=RQlXgAR0F4Y"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "https://www.youtube.com/watch?v=RQlXgAR0F4Y");
    QVERIFY2(video->getName() == "Lilly Wood & The Prick en concert privé Le Mouv'", video->getName().toUtf8().constData());
    QVERIFY(video->getDisplayName() == "Lilly Wood & The Prick en concert privé Le Mouv'");

    QVERIFY(video->metaDataTitle() == "Lilly Wood & The Prick en concert privé Le Mouv'");
    QVERIFY2(video->metaDataDuration() == 3671190, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY2(video->resolution() == "640x356", QString("%1").arg(video->resolution()).toUtf8());
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 279698593, QString("%1").arg(video->sourceSize()).toUtf8());


    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(video->isValid() == true);
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_MPEG2()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("hq");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("http://www.youtube.com/watch?v=04QzovLe2JM"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "http://www.youtube.com/watch?v=04QzovLe2JM");
    QVERIFY(video->getName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video->getDisplayName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");

    QVERIFY(video->metaDataTitle() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY2(video->metaDataDuration() == 259738, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY(video->resolution() == "638x360");
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 14186953, QString("%1").arg(video->sourceSize()).toUtf8());

    QVERIFY(video->toTranscode() == true);
    QVERIFY2(video->getLengthInMilliSeconds()==259738, QString("%1").arg(video->getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video->size()==160548290, QString("%1").arg(video->size()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    // test transcoding
    Device *device = video->getStream(0, 0, -1);
    QVERIFY(device != 0);

    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), transcodeProcess, SLOT(bytesSent(qint64,qint64)));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qWarning() << "DELTA" << video->size()-transcodedSize << qAbs(double(video->size()-transcodedSize))/video->size();
    QVERIFY(video->size() > transcodedSize);
    QVERIFY2(transcodedSize == 159786088, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(video->getdlnaOrgOpFlags() == "10");
    QVERIFY(video->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY2(video->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1", video->getProtocolInfo().toUtf8());

    QVERIFY(video->getAlbumArt().isNull() == true);
    QVERIFY(video->getByteAlbumArt().isNull() == true);

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo_MPEG4()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("hq");
    video->setTranscodeFormat(H264_AAC);
    video->setUrl(QUrl("http://www.youtube.com/watch?v=04QzovLe2JM"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "http://www.youtube.com/watch?v=04QzovLe2JM");
    QVERIFY(video->getName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video->getDisplayName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");

    QVERIFY(video->metaDataTitle() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY2(video->metaDataDuration() == 259738, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY(video->resolution() == "638x360");
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 2500000, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mp4", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 14186953, QString("%1").arg(video->sourceSize()).toUtf8());

    QVERIFY(video->toTranscode() == true);
    QVERIFY2(video->getLengthInMilliSeconds()==259738, QString("%1").arg(video->getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video->size()==89690778, QString("%1").arg(video->size()).toUtf8());

    QHash<QString, double> result = video->volumeInfo(-1);
    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    QVERIFY2(result["n_samples"] == 22908928, QString("%1").arg(result["n_samples"]).toUtf8());
    QVERIFY2(result["mean_volume"] == -12.9, QString("%1").arg(result["mean_volume"]).toUtf8());
    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    QVERIFY2(result["histogram_0db"] == 15923, QString("%1").arg(result["histogram_0db"]).toUtf8());
    QVERIFY2(result["histogram_1db"] == 43250, QString("%1").arg(result["histogram_1db"]).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    // test transcoding
    Device *device = video->getStream(0, 0, -1);
    QVERIFY(device != 0);

    transcodeProcess = qobject_cast<TranscodeProcess*>(device);
    QVERIFY(transcodeProcess != 0);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), transcodeProcess, SLOT(bytesSent(qint64,qint64)));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qWarning() << "DELTA" << video->size()-transcodedSize << qAbs(double(video->size()-transcodedSize))/video->size();
    QVERIFY(video->size() > transcodedSize);
    QVERIFY2(transcodedSize == 87871764, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(video->getdlnaOrgOpFlags() == "10");
    QVERIFY(video->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY2(video->getProtocolInfo() == "http-get:*:video/mp4:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1", video->getProtocolInfo().toUtf8());

    QVERIFY(video->getAlbumArt().isNull() == true);
    QVERIFY(video->getByteAlbumArt().isNull() == true);

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo3()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("hq");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("http://www.youtube.com/watch?v=cXxwIZwYnok"));
    bool res = video->waitUrl(15000);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "http://www.youtube.com/watch?v=cXxwIZwYnok");
    QVERIFY(video->getName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video->getDisplayName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");

    QVERIFY(video->metaDataTitle() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY2(video->metaDataDuration() == 220216, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY(video->resolution() == "640x360");
    QVERIFY2(video->framerate() == "25.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 15729501, QString("%1").arg(video->sourceSize()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo4()
{
    QElapsedTimer timer;
    timer.start();

    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
    video->moveToThread(backend);
    connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
    video->setNetworkAccessManager(manager);
    video->setPlaybackQuality("hq");
    video->setTranscodeFormat(MPEG2_AC3);
    video->setUrl(QUrl("https://www.youtube.com/watch?v=cmSYV5A9iuU"));
    bool res = video->waitUrl(30000);

    qint64 duration = timer.elapsed();
    qWarning() << "created in" << duration << "ms.";
    QVERIFY(video->getSystemName() == "https://www.youtube.com/watch?v=cmSYV5A9iuU");
    QVERIFY2(video->getName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)", video->getName().toUtf8());
    QVERIFY(video->getDisplayName() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");

    QVERIFY(video->metaDataTitle() == "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)");
    QVERIFY2(video->metaDataDuration() == 22023220, QString("%1").arg(video->metaDataDuration()).toUtf8());
    QVERIFY2(video->resolution() == "640x360", video->resolution().toUtf8());
    QVERIFY2(video->framerate() == "24.000", QString("%1").arg(video->framerate()).toUtf8());
    QVERIFY2(video->bitrate() == 4558800, QString("%1").arg(video->bitrate()).toUtf8());
    QVERIFY2(video->samplerate() == 44100, QString("%1").arg(video->samplerate()).toUtf8());
    QVERIFY2(video->channelCount() == 2, QString("%1").arg(video->channelCount()).toUtf8());

    QVERIFY2(video->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(video->metaDataFormat()).toUtf8());
    QVERIFY2(video->mimeType() == "video/mpeg", QString("%1").arg(video->mimeType()).toUtf8());
    QVERIFY2(video->sourceSize() == 470266214, QString("%1").arg(video->sourceSize()).toUtf8());

    qWarning() << "test done in" << timer.elapsed() << "ms.";

    QVERIFY2(duration < 15000, QString("Duration: %1").arg(duration).toUtf8());
    QVERIFY(res == true);
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo()
{
    DlnaCachedRootFolder rootFolder("host", 600, this);
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
    QVERIFY2(no_artist->getDisplayName() == "No Artist", no_artist->getDisplayName().toUtf8().constData());
    QVERIFY2(no_artist->getChildrenSize() > 200, QString("%1").arg(no_artist->getChildrenSize()).toUtf8().constData());

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
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Cats on trees \"Sirens call\" [Clip Officiel]");
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
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:17", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue() == "640x360", xml_res.elementsByTagName("res").at(0).attributes().namedItem("resolution").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2", xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "44100", xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "569850", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8().constData());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "121652086", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8().constData());
    xml_res.clear();

    QVERIFY(movie->getdlnaOrgOpFlags() == "10");
    QVERIFY(movie->getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(movie->getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY(movie->getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=1");

    QVERIFY(movie->getAlbumArt().isNull() == true);
    QVERIFY(movie->getByteAlbumArt().isNull() == true);

    QHash<QString, double> result = movie->volumeInfo(-1);
    QVERIFY2(result.keys().size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    QVERIFY2(result["mean_volume"] == -14.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    QVERIFY2(result["max_volume"] == -0.6, QString("%1").arg(result["max_volume"]).toUtf8());

    QVERIFY(movie->toTranscode() == true);
    QVERIFY2(movie->metaDataFormat() == "mov,mp4,m4a,3gp,3g2,mj2", QString("%1").arg(movie->metaDataFormat()).toUtf8());
    QVERIFY(movie->mimeType() == "video/mpeg");
    QVERIFY2(movie->size() == 121652086, QString("%1").arg(movie->size()).toUtf8());
    QVERIFY2(movie->bitrate() == 4558800, QString("%1").arg(movie->bitrate()).toUtf8());
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
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), transcodeProcess, SLOT(bytesSent(qint64,qint64)));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qWarning() << "DELTA" << movie->size()-transcodedSize << qAbs(double(movie->size()-transcodedSize))/movie->size();
    QVERIFY(movie->size() > transcodedSize);
    QVERIFY2(transcodedSize == 121638068, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;
}

void tst_dlnayoutubevideo::testCase_DlnaCachedNetworkVideo_checkLink()
{
    DlnaCachedRootFolder rootFolder("host", 600, this);

    int min_time = -1;
    int max_time = -1;
    int sum_time = 0;

    int nbOk = 0;
    int nbErrors = 0;
    QSqlQuery query = rootFolder.getAllNetworkLinks();

    while (query.next())
    {
        QUrl url = query.value("filename").toString();
        bool is_reachable = query.value("is_reachable").toBool();

        QElapsedTimer timer;
        timer.start();

        QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo("host", 600));
        video->moveToThread(backend);
        connect(backend, SIGNAL(finished()), video.data(), SLOT(deleteLater()));
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
                qCritical() << "ERROR TIMEOUT" << url;
        }
        else
        {
            ++nbOk;
            qDebug() << "OK" << url << res << video->isValid() << video->metaDataTitle() << video->metaDataDuration() << video->getLengthInMilliSeconds() << video->metaDataBitrate();
        }

        qDebug() << "check done in" << duration << "ms.";
    }

    if (nbErrors+nbOk!=0)
        qWarning() << "avg_time:" << sum_time/(nbErrors+nbOk) << "min_time:" << min_time << "max_time:" << max_time;
    qWarning() << "OK:" << nbOk << "KO:" << nbErrors;
    QVERIFY2(nbErrors == 0, QString("%1").arg(nbErrors).toUtf8());
}
