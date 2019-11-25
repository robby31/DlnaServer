#include "tst_dlnavideoitem.h"

tst_dlnavideoitem::tst_dlnavideoitem(QObject *parent) :
    DlnaCheckFunctions(parent)
{
    FfmpegTranscoding::setDirPath("/usr/local/bin");

    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
}

void tst_dlnavideoitem::init()
{
    m_dlnaProfiles = new Protocol("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml");
    m_dlnaProfiles->setProtocols(m_sinkProtocols);
}


void tst_dlnavideoitem::cleanup()
{
    delete m_dlnaProfiles;

    DebugInfo::display_alive_objects();

    QCOMPARE(DebugInfo::count_alive_objects(), 0);
}

void tst_dlnavideoitem::dataAvailable()
{
    auto device = qobject_cast<TranscodeDevice*>(sender());
    if (device)
        device->requestData(1000000);
    else
        qCritical() << "invalid device" << sender();
}

void tst_dlnavideoitem::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
}

void tst_dlnavideoitem::transcodingOpened()
{
    if (transcodeTimer.isValid())
    {
        timeToOpenTranscoding = transcodeTimer.elapsed();
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms.";
    }
}

void tst_dlnavideoitem::LogMessage(const QString &message)
{
    qInfo() << message;
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI_Starwars_MPEG4_AAC() {

    DlnaVideoFile movie("/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    movie.setTranscodeFormat(H264_AAC);

    movie.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(movie.toTranscode());
    QCOMPARE(movie.format(), H264_AAC);
    QCOMPARE(movie.getSystemName(), "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    QCOMPARE(movie.bitrate(), 6000000);
    QCOMPARE(movie.getLengthInMilliSeconds(), 8405880);
    QCOMPARE(movie.size(), 6966373050);
    QCOMPARE(movie.framerate(), "25.000");

    QCOMPARE(movie.audioLanguages(), QStringList() << "und");
    QVERIFY(movie.subtitleLanguages().isEmpty());

    QCOMPARE(movie.getdlnaOrgOpFlags(), "10");
    QCOMPARE(movie.getdlnaOrgPN(), "AVC_TS_MP_HD_AAC_MULT5");
    QCOMPARE(movie.getDlnaContentFeatures(), "DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(movie.getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(movie.getAlbumArt().isNull());
    QVERIFY(movie.getByteAlbumArt().isNull());

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 10, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -20.7, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.3, QString("%1").arg(result["max_volume"]).toUtf8());

    QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
    QVERIFY(transcodeProcess != Q_NULLPTR);
    transcodeProcess->setVariableBitrate(false);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open());
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    qint64 duration = transcodeTimer.elapsed();
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    if (timeToOpenTranscoding > 700)
        qWarning() << "time to open transcoding > 700 : " << timeToOpenTranscoding;
    if (duration > 3000000)
        qWarning() << "Duration > 3000000 : " << duration;
    QCOMPARE(transcodeProcess->exitCode(), 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QCOMPARE(transcodedSize, 6872799848);
    QVERIFY(movie.size() > transcodedSize);
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI_Starwars_MPEG2_AC3() {

    DlnaVideoFile movie("/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    movie.setTranscodeFormat(MPEG2_AC3);

    movie.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(movie.toTranscode());
    QCOMPARE(movie.format(), MPEG2_AC3);
    QCOMPARE(movie.getSystemName(), "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    QCOMPARE(movie.bitrate(), 4558800);
    QCOMPARE(movie.getLengthInMilliSeconds(), 8405880);
    QCOMPARE(movie.size(), 5195811401);
    QCOMPARE(movie.framerate(), "25.000");

    QCOMPARE(movie.audioLanguages(), QStringList() << "und");
    QVERIFY(movie.subtitleLanguages().isEmpty());

    QCOMPARE(movie.getdlnaOrgOpFlags(), "10");
    QCOMPARE(movie.getdlnaOrgPN(), "MPEG_TS_HD_NA");
    QCOMPARE(movie.getDlnaContentFeatures(), "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(movie.getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(movie.getAlbumArt().isNull());
    QVERIFY(movie.getByteAlbumArt().isNull());

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 10, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -20.7, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.3, QString("%1").arg(result["max_volume"]).toUtf8());

    QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open());
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    qint64 duration = transcodeTimer.elapsed();

    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    QCOMPARE(transcodeProcess->exitCode(), 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QVERIFY(movie.size() > transcodedSize);
    QCOMPARE(transcodedSize, 5159838224);

    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    if (timeToOpenTranscoding > 500)
        qWarning() << "time to open transcoding > 500 : " << timeToOpenTranscoding;
    if (duration > 450000)
        qWarning() << "Duration > 450000 : " << duration;
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV_Looper_MPEG2_AC3() {

    DlnaVideoFile movie("/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv");
    movie.setTranscodeFormat(MPEG2_AC3);

    movie.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(movie.toTranscode());
    QCOMPARE(movie.format(), MPEG2_AC3);
    QCOMPARE(movie.getSystemName(), "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv");
    QCOMPARE(movie.bitrate(), 4558800);
    QCOMPARE(movie.getLengthInMilliSeconds(), 6825480);
    QCOMPARE(movie.size(), 4218940409);
    QCOMPARE(movie.framerate(), "25.000");

    QCOMPARE(movie.audioLanguages(), QStringList() << "eng");
    QCOMPARE(movie.subtitleLanguages(), QStringList() << "fre");

    QCOMPARE(movie.getdlnaOrgOpFlags(), "10");
    QCOMPARE(movie.getdlnaOrgPN(), "MPEG_TS_HD_NA");
    QCOMPARE(movie.getDlnaContentFeatures(), "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(movie.getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(movie.getAlbumArt().isNull());
    QVERIFY(movie.getByteAlbumArt().isNull());

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 19, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -33.9, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.9, QString("%1").arg(result["max_volume"]).toUtf8());

    QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
    QVERIFY(transcodeProcess != Q_NULLPTR);
    transcodeProcess->setVariableBitrate(false);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
//    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open());
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    qint64 duration = transcodeTimer.elapsed();

    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    QCOMPARE(transcodeProcess->exitCode(), 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QVERIFY(movie.size() > transcodedSize);
    QCOMPARE(transcodedSize, 4198362044);

    if (timeToOpenTranscoding > 7000)
        qWarning() << "time to open transcoding > 7000 : " << timeToOpenTranscoding;
    if (duration > 400000)
        qWarning() << "Duration > 400000 : " << duration;
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV_MPEG2_AC3()
{
    DlnaVideoFile movie("/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");
    movie.setHostUrl(QUrl("http://host:600"));
    movie.setTranscodeFormat(MPEG2_AC3);
    QCOMPARE(movie.getSystemName(), "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");

    movie.setDlnaProfiles(m_dlnaProfiles);

    check_dlna_video(&movie,
                     "", "-1",
                     "District.9.2009.720p.BrRip.YIFY", "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "01:52:16", "1280x688", 2, 48000,
                     569850, 4163526283, "http://host:600/get//District.9.2009.720p.BrRip.YIFY.mkv");

    QCOMPARE(movie.getdlnaOrgOpFlags(), "10");
    QCOMPARE(movie.getdlnaOrgPN(), "MPEG_TS_HD_NA");
    QCOMPARE(movie.getDlnaContentFeatures(), "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(movie.getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(movie.getAlbumArt().isNull());
    QVERIFY(movie.getByteAlbumArt().isNull());

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 15, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -28.2, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.1, QString("%1").arg(result["max_volume"]).toUtf8());

    QVERIFY(movie.toTranscode());
    QCOMPARE(movie.format(), MPEG2_AC3);
    QCOMPARE(movie.mimeType(), "video/vnd.dlna.mpeg-tts");
    QCOMPARE(movie.size(), 4163526283);
    QCOMPARE(movie.bitrate(), 4558800);
    QCOMPARE(movie.getLengthInSeconds(), 6736);
    QCOMPARE(movie.getLengthInMilliSeconds(), 6735830);
    QCOMPARE(movie.samplerate(), 48000);
    QCOMPARE(movie.channelCount(), 2);
    QCOMPARE(movie.resolution(), "1280x688");
    QCOMPARE(movie.audioLanguages(), QStringList() << "");
    QCOMPARE(movie.subtitleLanguages(), QStringList() << "eng");
    QCOMPARE(movie.framerate(), "23.976");

    {
        // test partial transcoding (10 seconds) in constant bitrate
        QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
        QVERIFY(transcodeProcess != Q_NULLPTR);
        transcodeProcess->setTimeSeek(-1, 10);
        transcodeProcess->setVariableBitrate(false);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeTimer.start();
        timeToOpenTranscoding = 0;
        QVERIFY(transcodeProcess->open());
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        transcodeProcess->requestData(transcodeProcess->bytesAvailable());
        qint64 duration = transcodeTimer.elapsed();

        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QCOMPARE(transcodeProcess->exitCode(), 0);
        qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
        QCOMPARE(transcodedSize, 6115828);
        QVERIFY(movie.size() > transcodedSize);

        if (timeToOpenTranscoding > 10000)
            qWarning() << "Time to open transcoding > 10000 : " << timeToOpenTranscoding;
        if (duration > 10000)
            qWarning() << "Duration > 10000 : " << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    }

    {
        // test partial transcoding (10 seconds) in variable bitrate
        QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
        QVERIFY(transcodeProcess != Q_NULLPTR);
        transcodeProcess->setTimeSeek(-1, 10);
        transcodeProcess->setVariableBitrate(true);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeTimer.start();
        timeToOpenTranscoding = 0;
        QVERIFY(transcodeProcess->open());
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        transcodeProcess->requestData(transcodeProcess->bytesAvailable());
        qint64 duration = transcodeTimer.elapsed();

        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QCOMPARE(transcodeProcess->exitCode(), 0);
        qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
        QCOMPARE(transcodedSize, 1827548);
        QVERIFY(movie.size() > transcodedSize);

        if (timeToOpenTranscoding > 10000)
            qWarning() << "Time to open transcoding > 10000 : " << timeToOpenTranscoding;
        if (duration > 10000)
            qWarning() << "Duration > 10000 : " << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    }

    {
        // test full transcoding in constant bitrate
        QScopedPointer<FfmpegTranscoding> transcodeProcess(qobject_cast<FfmpegTranscoding*>(movie.getStream()));
        QVERIFY(transcodeProcess != Q_NULLPTR);
        transcodeProcess->setVariableBitrate(false);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeTimer.start();
        timeToOpenTranscoding = 0;
        QVERIFY(transcodeProcess->open());
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        transcodeProcess->requestData(transcodeProcess->bytesAvailable());
        qint64 duration = transcodeTimer.elapsed();

        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QCOMPARE(transcodeProcess->exitCode(), 0);
        qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
        QCOMPARE(transcodedSize, 4165712084);
        QVERIFY(movie.size() > transcodedSize);

        if (timeToOpenTranscoding > 2000)
            qWarning() << "time to open transcoding > 2000 : " << timeToOpenTranscoding;
        if (duration > 700000)
            qWarning() << "Duration > 700000 : " << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    }
}
