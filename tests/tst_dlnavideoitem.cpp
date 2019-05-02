#include "tst_dlnavideoitem.h"

tst_dlnavideoitem::tst_dlnavideoitem(QObject *parent) :
    QObject(parent),
    m_dlnaProfiles("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml")
{
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
    m_sinkProtocols << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
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
    movie.setSinkProtocol(m_sinkProtocols);

    QVERIFY(movie.toTranscode()==true);
    QVERIFY(movie.format() == H264_AAC);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    QVERIFY2(movie.bitrate()==6000000, QString("%1").arg(movie.bitrate()).toUtf8().constData());
    QVERIFY2(movie.getLengthInMilliSeconds()==8405880, QString("%1").arg(movie.getLengthInMilliSeconds()).toUtf8().constData());
    QVERIFY2(movie.size()==6966373050, QString("%1").arg(movie.size()).toUtf8().constData());
    QVERIFY2(movie.framerate() == "25.000", movie.framerate().toUtf8());

    QVERIFY2(movie.audioLanguages() == QStringList() << "und", movie.audioLanguages().join(',').toUtf8());
    QVERIFY2(movie.subtitleLanguages().isEmpty() == true, movie.audioLanguages().join(',').toUtf8());

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QCOMPARE(movie.getdlnaOrgPN(), "AVC_TS_MP_HD_AAC_MULT5");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY2(movie.getProtocolInfo() == "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", movie.getProtocolInfo().toUtf8());

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 10, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -20.7, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.3, QString("%1").arg(result["max_volume"]).toUtf8());

    Device *device = movie.getStream();
    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    qint64 duration = transcodeTimer.elapsed();
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    if (timeToOpenTranscoding > 700)
        qWarning() << "time to open transcoding > 700 : " << timeToOpenTranscoding;
    if (duration > 3000000)
        qWarning() << "Duration > 3000000 : " << duration;
    QVERIFY(transcodeProcess->exitCode() == 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QVERIFY(movie.size() > transcodedSize);
    QVERIFY2(transcodedSize == 2878358584, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_AVI_Starwars_MPEG2_AC3() {

    DlnaVideoFile movie("/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    movie.setTranscodeFormat(MPEG2_AC3);

    movie.setDlnaProfiles(m_dlnaProfiles);
    movie.setSinkProtocol(m_sinkProtocols);

    QVERIFY(movie.toTranscode()==true);
    QVERIFY(movie.format() == MPEG2_AC3);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Fiction/Starwars/Star.Wars.Episode.III.FRENCH.Bdrip.Xvid.AC3-FQT.mp4");
    QVERIFY2(movie.bitrate()==4558800, QString("%1").arg(movie.bitrate()).toUtf8().constData());
    QVERIFY2(movie.getLengthInMilliSeconds()==8405880, QString("%1").arg(movie.getLengthInMilliSeconds()).toUtf8().constData());
    QVERIFY2(movie.size()==5195811401, QString("%1").arg(movie.size()).toUtf8().constData());
    QVERIFY2(movie.framerate() == "25.000", movie.framerate().toUtf8());

    QVERIFY2(movie.audioLanguages() == QStringList() << "und", movie.audioLanguages().join(',').toUtf8());
    QVERIFY2(movie.subtitleLanguages().isEmpty() == true, movie.audioLanguages().join(',').toUtf8());

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QCOMPARE(movie.getdlnaOrgPN(), "MPEG_TS_HD_NA");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY2(movie.getProtocolInfo() == "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", movie.getProtocolInfo().toUtf8());

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 10, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -20.7, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.3, QString("%1").arg(result["max_volume"]).toUtf8());

    Device *device = movie.getStream();
    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    qint64 duration = transcodeTimer.elapsed();

    QVERIFY(transcodeProcess->bytesAvailable() == 0);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QVERIFY(movie.size() > transcodedSize);
    QVERIFY2(transcodedSize == 5159838224, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

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
    movie.setSinkProtocol(m_sinkProtocols);

    QVERIFY(movie.toTranscode()==true);
    QVERIFY(movie.format() == MPEG2_AC3);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/Action/Looper.2012.DVDRip.XviD-PTpOWeR.mkv");
    QVERIFY2(movie.bitrate()==4558800, QString("%1").arg(movie.bitrate()).toUtf8().constData());
    QVERIFY2(movie.getLengthInMilliSeconds()==6825480, QString("%1").arg(movie.getLengthInMilliSeconds()).toUtf8().constData());
    QVERIFY2(movie.size()==4218940409, QString("%1").arg(movie.size()).toUtf8().constData());
    QVERIFY(movie.framerate() == "25.000");

    QVERIFY2(movie.audioLanguages() == QStringList() << "eng", movie.audioLanguages().join(',').toUtf8());
    QVERIFY2(movie.subtitleLanguages() == QStringList() << "fre", movie.subtitleLanguages().join(',').toUtf8());

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_TS_HD_NA");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY2(movie.getProtocolInfo() == "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", movie.getProtocolInfo().toUtf8());

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 19, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -33.9, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.9, QString("%1").arg(result["max_volume"]).toUtf8());

    Device *device = movie.getStream();
    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
//    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    QVERIFY(transcodeProcess->open() == true);
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    qint64 duration = transcodeTimer.elapsed();

    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    QVERIFY(transcodeProcess->exitCode() == 0);
    qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
    QVERIFY(movie.size() > transcodedSize);
    QVERIFY2(transcodedSize == 4196970468, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

    if (timeToOpenTranscoding > 7000)
        qWarning() << "time to open transcoding > 7000 : " << timeToOpenTranscoding;
    if (duration > 400000)
        qWarning() << "Duration > 400000 : " << duration;
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
}

void tst_dlnavideoitem::testCase_DlnaVideoItem_MKV_MPEG2_AC3() {

    DlnaVideoFile movie("/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");
    movie.setHostUrl(QUrl("http://host:600"));
    movie.setTranscodeFormat(MPEG2_AC3);
    QVERIFY(movie.getSystemName() == "/Users/doudou/Movies/Films/District.9.2009.720p.BrRip.YIFY.mkv");

    movie.setDlnaProfiles(m_dlnaProfiles);
    movie.setSinkProtocol(m_sinkProtocols);

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
    check_dlna_video(xml_res,
                     "", "-1",
                     "District.9.2009.720p.BrRip.YIFY", "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "01:52:16", "1280x688", 2, 48000,
                     569850, 4163526283, "http://host:600/get//District.9.2009.720p.BrRip.YIFY.mkv");
    xml_res.clear();

    QVERIFY(movie.getdlnaOrgOpFlags() == "10");
    QVERIFY(movie.getdlnaOrgPN() == "MPEG_TS_HD_NA");
    QVERIFY(movie.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(movie.getProtocolInfo() == "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(movie.getAlbumArt().isNull() == true);
    QVERIFY(movie.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = movie.volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 15, QString("%1").arg(result.keys().size()).toUtf8());
//    QVERIFY2(result["mean_volume"] == -28.2, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == -0.1, QString("%1").arg(result["max_volume"]).toUtf8());

    QVERIFY(movie.toTranscode() == true);
    QVERIFY(movie.format() == MPEG2_AC3);
    QVERIFY(movie.mimeType() == "video/vnd.dlna.mpeg-tts");
    QVERIFY(movie.size() == 4163526283);
    QVERIFY(movie.bitrate() == 4558800);
    QVERIFY(movie.getLengthInSeconds() == 6736);
    QVERIFY2(movie.getLengthInMilliSeconds() == 6735830, QString("%1").arg(movie.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(movie.samplerate() == 48000);
    QVERIFY(movie.channelCount() == 2);
    QVERIFY(movie.resolution() == "1280x688");
    QVERIFY2(movie.audioLanguages() == QStringList() << "", movie.audioLanguages().join(',').toUtf8());
    QVERIFY2(movie.subtitleLanguages() == QStringList() << "eng", movie.subtitleLanguages().join(',').toUtf8());
    QVERIFY2(movie.framerate() == "23.976", movie.framerate().toUtf8());

    // test partial transcoding (10 seconds)
    Device *device = movie.getStream();
    QVERIFY(device != Q_NULLPTR);
    device->setTimeSeek(-1, 10);

    {
        QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
        QVERIFY(transcodeProcess != Q_NULLPTR);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeTimer.start();
        timeToOpenTranscoding = 0;
        QVERIFY(transcodeProcess->open() == true);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        transcodeProcess->requestData(transcodeProcess->bytesAvailable());
        qint64 duration = transcodeTimer.elapsed();

        QVERIFY(transcodeProcess->bytesAvailable() == 0);
        QVERIFY(transcodeProcess->exitCode() == 0);
        qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
        QVERIFY(movie.size() > transcodedSize);
        QVERIFY2(transcodedSize == 6106804, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

        if (timeToOpenTranscoding > 10000)
            qWarning() << "Time to open transcoding > 10000 : " << timeToOpenTranscoding;
        if (duration > 10000)
            qWarning() << "Duration > 10000 : " << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    }

    // test full transcoding
    device = movie.getStream();
    QVERIFY(device != Q_NULLPTR);

    {
        QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
        QVERIFY(transcodeProcess != Q_NULLPTR);

        transcodedSize = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
        connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeTimer.start();
        timeToOpenTranscoding = 0;
        QVERIFY(transcodeProcess->open()==true);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        transcodeProcess->requestData(transcodeProcess->bytesAvailable());
        qint64 duration = transcodeTimer.elapsed();

        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QVERIFY(transcodeProcess->exitCode() == 0);
        qInfo() << "DELTA" << movie.size()-transcodedSize << qAbs(double(movie.size()-transcodedSize))/movie.size();
        QVERIFY(movie.size() > transcodedSize);
        QVERIFY2(transcodedSize == 4157460764, QString("transcoded size = %1").arg(transcodedSize).toUtf8());

        if (timeToOpenTranscoding > 2000)
            qWarning() << "time to open transcoding > 2000 : " << timeToOpenTranscoding;
        if (duration > 700000)
            qWarning() << "Duration > 700000 : " << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
    }
}
