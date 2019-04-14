#include "tst_dlnanetworkvideo.h"

tst_dlnanetworkvideo::tst_dlnanetworkvideo(QObject *parent):
    QObject(parent),
    db(CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE")),
    m_dlnaProfiles("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml")
{
    FfmpegTranscoding::setDirPath("/opt/local/bin");

    db.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    db.setConnectOptions("Pooling=True;Max Pool Size=100;");
    db.open();
}

void tst_dlnanetworkvideo::receivedTranscodedData(const QByteArray &data)
{
    transcodedSize += data.size();
}

void tst_dlnanetworkvideo::LogMessage(const QString &message)
{
    qInfo() << message;
}

void tst_dlnanetworkvideo::cleanupTestCase()
{
    qInfo() << media_ok << "MEDIAS OK.";
    qInfo() << media_ko << "MEDIAS INVALID.";
    qInfo() << media_not_available << "MEDIAS NOT AVAILABLE.";
    qInfo() << media_timeout << "MEDIAS TIMEOUT.";

    DecryptYoutubeSignature::print_cache();
}

QString tst_dlnanetworkvideo::timeToString(const qint64 &msec)
{
    QTime duration(0,0,0);
    return duration.addMSecs(static_cast<int>(msec)).toString("hh:mm:ss,zzz");
}

void tst_dlnanetworkvideo::testCase_DlnaNetworkVideo_data()
{
    QTest::addColumn<bool>("is_valid");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<int>("max_height");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("media_duration");
    QTest::addColumn<QString>("resolution");
    QTest::addColumn<QString>("frame_rate");
    QTest::addColumn<int>("bit_rate");
    QTest::addColumn<int>("sample_rate");
    QTest::addColumn<int>("channels");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("audio_format");
    QTest::addColumn<QString>("video_format");
    QTest::addColumn<QString>("mime_type");
    QTest::addColumn<qint64>("source_size");

    QTest::newRow("ArteTv Jazz") << true
                                 << QUrl("https://www.arte.tv/fr/videos/086296-006-A/julian-roman-wasserfuhr-feat-joerg-brinkmann-au-wdr-3-jazzfest/") << -1
                                 << "Julian & Roman Wasserfuhr feat. J\u00F6rg Brinkmann au WDR 3 Jazzfest"
                                 << 3705000 << "1280x720" << "25.000" << 4558800 << 48000 << 2
                                 << "mov,mp4,m4a,3gp,3g2,mj2" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(1150792878);

    QTest::newRow("RMCDecouverte Vintage") << true
                                           << QUrl("https://rmcdecouverte.bfmtv.com/vintage-mecanic/program_5280/") << -1
                                           << "PORSCHE 356 CABRIOLET 1958"
                                           << 3722112 << "1280x720" << "25.000" << 4558800 << 44100 << 2
                                           << "hls,applehttp" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

    QTest::newRow("Youtube_Comptines") << true
                                       << QUrl("http://www.youtube.com/watch?v=SLbxwYTymCQ") << -1
                                       << "Comptines et chansons pour enfants - Titounis"
                                       << 1667521 << "1280x720" << "29.970" << 4558800 << 48000 << 2
                                       << "matroska,webm" << "opus" << "av01.0.05M.08" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(29818677);

    QTest::newRow("Youtube_Lilly") << true
                                   << QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU") << -1
                                   << "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]"
                                   << 193641 << "1920x1080" << "25.000" << 4558800 << 48000 << 2
                                   << "mov,mp4,m4a,3gp,3g2,mj2" << "opus" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(49042411);

    QTest::newRow("Youtube_Lilly_MaxHeight_720") << true
                                                 << QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU") << 720
                                                 << "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]"
                                                 << 193641 << "1280x720" << "25.000" << 4558800 << 48000 << 2
                                                 << "mov,mp4,m4a,3gp,3g2,mj2" << "opus" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(30880770);

    QTest::newRow("Youtube_Lilly_MaxHeight_100") << true
                                                 << QUrl("https://www.youtube.com/watch?v=JrlfFTS9kGU") << 100
                                                 << "Lilly Wood & The Prick - Prayer in C (Robin Schulz remix) [Clip officiel]"
                                                 << 193641 << "256x144" << "25.000" << 4558800 << 48000 << 2
                                                 << "matroska,webm" << "opus" << "vp9" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(5603535);

    QTest::newRow("Youtube_Lilly2") << true
                                    << QUrl("http://www.youtube.com/watch?v=cXxwIZwYnok") << -1
                                    << "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]"
                                    << 220160 << "1920x1080" << "25.000" << 4558800 << 48000 << 2
                                    << "mov,mp4,m4a,3gp,3g2,mj2" << "opus" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(67988426);

    QTest::newRow("Youtube_Lilly3") << true
                                    << QUrl("https://www.youtube.com/watch?v=RQlXgAR0F4Y") << -1
                                    << "Lilly Wood & The Prick en concert privé Le Mouv'"
                                    << 3671000 << "640x356" << "25.000" << 4558800 << 44100 << 2
                                    << "mov,mp4,m4a,3gp,3g2,mj2" << "vorbis" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(221908288);

    QTest::newRow("Youtube_not_available") << false
                                           << QUrl("https://www.youtube.com/watch?v=ji74LmoyqAg") << -1
                                           << ""
                                           << 0 << "" << "" << 4558800 << -1 << -1
                                           << "" << "" << "" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

    QTest::newRow("Youtube_Muse") << true
                                  << QUrl("https://www.youtube.com/watch?v=l9kqU_7-CgI") << -1
                                  << "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]"
                                  << 258391 << "1920x1080" << "29.970" << 4558800 << 48000 << 2
                                  << "matroska,webm" << "opus" << "vp9" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(29661109);

    QTest::newRow("Youtube_Jabig") << true
                                   << QUrl("https://www.youtube.com/watch?v=cmSYV5A9iuU") << -1
                                   << "6 Hour Jazz Music Mix by JaBig (Best of Classic Long Smooth Piano Soft Instrumental Study Playlist)"
                                   << 22023167 << "1280x720" << "24.000" << 4558800 << 48000 << 2
                                   << "matroska,webm" << "opus" << "vp9" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(602836473);

    QTest::newRow("France2 (SITE)") << true
                                    << QUrl("https://www.france.tv/france-2/direct.html") << -1
                                    << "France 2 en direct"
                                    << 0 << "1024x576" << "25.000" << 4558800 << 32000 << 2
                                    << "hls,applehttp" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

    QTest::newRow("France2 (URL)") << true
                                   << QUrl("francetv:SIM_France2") << -1
                                   << "France 2 en direct"
                                   << 0 << "1024x576" << "25.000" << 4558800 << 32000 << 2
                                   << "hls,applehttp" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

    QTest::newRow("France2 (URL) MaxHeight 396") << true
                                                 << QUrl("francetv:SIM_France2") << 396
                                                 << "France 2 en direct"
                                                 << 0 << "704x396" << "25.000" << 4558800 << 32000 << 2
                                                 << "hls,applehttp" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

    QTest::newRow("France2 (URL) MaxHeight 50") << true
                                                 << QUrl("francetv:SIM_France2") << 50
                                                 << "France 2 en direct"
                                                 << 0 << "256x144" << "25.000" << 4558800 << 32000 << 2
                                                 << "hls,applehttp" << "aac" << "h264" << "video/vnd.dlna.mpeg-tts" << static_cast<qint64>(0);

}

void tst_dlnanetworkvideo::testCase_DlnaNetworkVideo()
{
    QFETCH(bool, is_valid);
    QFETCH(QUrl, url);
    QFETCH(int, max_height);
    QFETCH(QString, name);
    QFETCH(int, media_duration);
    QFETCH(QString, resolution);
    QFETCH(QString, frame_rate);
    QFETCH(int, bit_rate);
    QFETCH(int, sample_rate);
    QFETCH(int, channels);
    QFETCH(QString, format);
    QFETCH(QString, audio_format);
    QFETCH(QString, video_format);
    QFETCH(QString, mime_type);
    QFETCH(qint64, source_size);

    QList<bool> analyse_stream;
    analyse_stream << true << false;
    foreach (const bool &flag, analyse_stream)
    {
        QElapsedTimer timer;
        timer.start();

        DlnaNetworkVideo video;
        video.setAnalyzeStream(flag);
        video.setTranscodeFormat(MPEG2_AC3);
        video.setUrl(url);
        if (max_height > 0)
            video.setMaxVideoHeight(max_height);
        bool res = video.waitUrl(15000);

        qint64 duration = timer.elapsed();
        QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
        qInfo() << &video << "created in" << timeToString(duration).toUtf8().constData();
        QCOMPARE(video.getSystemName(), url.url());
        QCOMPARE(video.getName(), name);
        QCOMPARE(video.getDisplayName(), name);

        QCOMPARE(video.metaDataTitle(), name);
        QCOMPARE(video.metaDataDuration()/1000, media_duration/1000);
        QCOMPARE(video.resolution(), resolution);
        QCOMPARE(video.framerate(), frame_rate);
        QCOMPARE(video.bitrate(), bit_rate);
        QCOMPARE(video.samplerate(), sample_rate);
        QCOMPARE(video.channelCount(), channels);

        QCOMPARE(video.metaDataFormat(), format);
        QCOMPARE(video.sourceAudioFormat(), audio_format);
        QCOMPARE(video.sourceVideoFormat(), video_format);
        QCOMPARE(video.mimeType(), mime_type);
        QCOMPARE(video.sourceSize(), source_size);

        //    QHash<QString, double> result = video->volumeInfo(-1);
        //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
        //    QVERIFY2(result["n_samples"] == 17082368, QString("%1").arg(result["n_samples"]).toUtf8());
        //    QVERIFY2(result["mean_volume"] == -9.8, QString("%1").arg(result["mean_volume"]).toUtf8());
        //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
        //    QVERIFY2(result["histogram_0db"] == 110089, QString("%1").arg(result["histogram_0db"]).toUtf8());

        QCOMPARE(video.isValid(), is_valid);
        QVERIFY(res);
    }
}

void tst_dlnanetworkvideo::testCase_DlnaCachedNetworkVideo()
{
    DlnaCachedRootFolder rootFolder(this);

    DlnaCachedGroupedFolderMetaData *folder = Q_NULLPTR;
    for (int index=0;index<rootFolder.getChildrenSize();++index)
    {
        folder = qobject_cast<DlnaCachedGroupedFolderMetaData*>(rootFolder.getChild(index));
        if (folder && folder->getDisplayName() == "YOUTUBE")
            break;
    }

    QVERIFY(folder != Q_NULLPTR);
    if (folder)
    {
        QCOMPARE(folder->getDisplayName(), "YOUTUBE");
        QCOMPARE(folder->getChildrenSize(), 3);

        auto artists = qobject_cast<DlnaCachedFolderMetaData*>(folder->getChild(0));
        QCOMPARE(artists->getDisplayName(), "Artist");
        QCOMPARE(artists->getChildrenSize(), 109);

        DlnaCachedFolder *artist = Q_NULLPTR;
        for (int index=0;index<artists->getChildrenSize();++index)
        {
            artist = qobject_cast<DlnaCachedFolder*>(artists->getChild(index));
            if (artist->getDisplayName() == "Cats On Trees")
                break;
        }

        QVERIFY(artist != Q_NULLPTR);

        if (artist)
        {
            QList<DlnaResource*> results = artists->getDLNAResources(artist->getResourceId(), false, 0, 1, "*");
            if (results.size() == 1)
                artist = qobject_cast<DlnaCachedFolder*>(results.at(0));
            QCOMPARE(artist->getDisplayName(), "Cats On Trees");
            QVERIFY2(artist->getChildrenSize() > 0, QString("%1").arg(artist->getChildrenSize()).toUtf8().constData());

            DlnaCachedNetworkVideo* movie = Q_NULLPTR;
            for (int index=0;index<artist->getChildrenSize();++index)
            {
                movie = qobject_cast<DlnaCachedNetworkVideo*>(artist->getChild(index));
                qInfo() << movie->getSystemName() << movie->getDisplayName();
                if (movie && movie->getSystemName() == "http://www.youtube.com/watch?v=X3hJYDGSXt4")
                    break;
            }

            QVERIFY(movie != Q_NULLPTR);
            if (movie)
            {
                QCOMPARE(movie->getSystemName(), QString("http://www.youtube.com/watch?v=X3hJYDGSXt4"));
                movie->setTranscodeFormat(MPEG2_AC3);

                movie->setHostUrl(QUrl("http://host:600"));

                QStringList sinkProtocol;
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
                movie->setDlnaProfiles(m_dlnaProfiles);
                movie->setSinkProtocol(sinkProtocol);

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
                check_dlna_video(xml_res,
                                 "0$7$1$22$1", "0$7$1$22",
                                 "Cats on trees \"Sirens call\" [Clip Officiel]",
                                 "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                                 "00:03:17", "1280x720", 2, 48000,
                                 569850, 121645286,
                                 "http://host:600/get/0$7$1$22$1/Media%2814955%29");
                xml_res.clear();

                QCOMPARE(movie->getdlnaOrgOpFlags(), "10");
                QCOMPARE(movie->getdlnaOrgPN(), "MPEG_TS_HD_NA");
                QCOMPARE(movie->getDlnaContentFeatures(), "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
                QCOMPARE(movie->getProtocolInfo(), "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

                QVERIFY(!movie->getAlbumArt().isNull());
                QVERIFY(!movie->getByteAlbumArt().isNull());

                QHash<QString, double> result = movie->volumeInfo(-1);
                QVERIFY2(result.size() == 2, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
                QVERIFY2(QVariant::fromValue(result["mean_volume"]).toString() == "-14.1", QString("%1").arg(result["mean_volume"]).toUtf8());
                QVERIFY2(QVariant::fromValue(result["max_volume"]).toString() == "-0.6", QString("%1").arg(result["max_volume"]).toUtf8());

                QVERIFY(movie->toTranscode());
                QCOMPARE(movie->metaDataFormat(), "mov,mp4,m4a,3gp,3g2,mj2");
                QCOMPARE(movie->mimeType(), "video/vnd.dlna.mpeg-tts");
                QCOMPARE(movie->size(), 121645286);
                QCOMPARE(movie->bitrate(), 4558800);
                QCOMPARE(movie->getLengthInSeconds(), 197);
                QCOMPARE(movie->getLengthInMilliSeconds(), 196800);
                QCOMPARE(movie->samplerate(), 48000);
                QCOMPARE(movie->channelCount(), 2);
                QCOMPARE(movie->resolution(), "1280x720");
                QVERIFY2(movie->audioLanguages() == QStringList() << "", movie->audioLanguages().join(',').toUtf8());
                QVERIFY2(movie->subtitleLanguages() == QStringList() << "", movie->subtitleLanguages().join(',').toUtf8());
                QCOMPARE(movie->framerate(), "25.000");
            }
        }
    }
}

void tst_dlnanetworkvideo::testCase_StreamingVideo_data()
{
    QTest::addColumn<TranscodeFormatAvailable>("transcode_format");
    QTest::addColumn<bool>("is_valid");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("media_duration");
    QTest::addColumn<QString>("resolution");
    QTest::addColumn<QString>("frame_rate");
    QTest::addColumn<int>("bit_rate");
    QTest::addColumn<int>("sample_rate");
    QTest::addColumn<int>("channels");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("mime_type");
    QTest::addColumn<QString>("dlna_org_pn");
    QTest::addColumn<QString>("dlna_content_features");
    QTest::addColumn<QString>("protocolInfo");
    QTest::addColumn<int>("source_size");
    QTest::addColumn<int>("video_size");
    QTest::addColumn<int>("transcoded_size");

    QTest::newRow("Youtube_Muse_MPEG2_AC3") << MPEG2_AC3
                                            << true
                                            << QUrl("https://www.youtube.com/watch?v=l9kqU_7-CgI")
                                            << "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]"
                                            << 258391
                                            << "1280x720" << "29.970" << 4558800 << 48000 << 2
                                            << "matroska,webm" << "video/vnd.dlna.mpeg-tts"
                                            << "MPEG_TS_HD_NA"
                                            << "DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000"
                                            << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000"
                                            << 17690325 << 159715687 << 159511796;

    QTest::newRow("Youtube_Muse_H264_AAC") << H264_AAC
                                           << true
                                           << QUrl("https://www.youtube.com/watch?v=l9kqU_7-CgI")
                                           << "Muse - Exogenesis: Symphony, Part 1 (Overture) [HD]"
                                           << 258391
                                           << "1280x720" << "29.970" << 4000000 << 48000 << 2
                                           << "matroska,webm" << "video/vnd.dlna.mpeg-tts"
                                           << "AVC_TS_MP_HD_AAC_MULT5"
                                           << "DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000"
                                           << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000"
                                           << 17690325 << 142761027 << 88727164;

}

void tst_dlnanetworkvideo::testCase_StreamingVideo()
{
    QFETCH(TranscodeFormatAvailable, transcode_format);
    QFETCH(bool, is_valid);
    QFETCH(QUrl, url);
    QFETCH(QString, name);
    QFETCH(int, media_duration);
    QFETCH(QString, resolution);
    QFETCH(QString, frame_rate);
    QFETCH(int, bit_rate);
    QFETCH(int, sample_rate);
    QFETCH(int, channels);
    QFETCH(QString, format);
    QFETCH(QString, mime_type);
    QFETCH(int, source_size);
    QFETCH(int, video_size);
    QFETCH(int, transcoded_size);
    QFETCH(QString, protocolInfo);
    QFETCH(QString, dlna_org_pn);
    QFETCH(QString, dlna_content_features);

    QElapsedTimer timer;
    timer.start();

    DlnaNetworkVideo video;
    video.setTranscodeFormat(transcode_format);

    video.setUrl(url);
    video.setMaxVideoHeight(720);
    bool res = video.waitUrl(15000);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
    sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
    sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
    video.setDlnaProfiles(m_dlnaProfiles);
    video.setSinkProtocol(sinkProtocol);

    qint64 duration = timer.elapsed();
    QVERIFY2(duration < 10000, QString("Duration: %1").arg(duration).toUtf8());
    qInfo() << &video << "created in" << timeToString(duration).toUtf8().constData();
    QCOMPARE(video.getSystemName(), url.url());
    QCOMPARE(video.getName(), name);
    QCOMPARE(video.getDisplayName(), name);

    QCOMPARE(video.metaDataTitle(), name);
    QCOMPARE(video.metaDataDuration(), media_duration);
    QCOMPARE(video.resolution(), resolution);
    QCOMPARE(video.framerate(), frame_rate);
    QCOMPARE(video.bitrate(), bit_rate);
    QCOMPARE(video.samplerate(), sample_rate);
    QCOMPARE(video.channelCount(), channels);

    QCOMPARE(video.metaDataFormat(), format);
    QCOMPARE(video.mimeType(), mime_type);
    QCOMPARE(video.sourceSize(), source_size);

    QVERIFY(video.toTranscode());
    QCOMPARE(video.getLengthInMilliSeconds(), media_duration);
    QCOMPARE(video.size(), video_size);

//    QHash<QString, double> result = video->volumeInfo(-1);
//    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
//    QVERIFY2(result["n_samples"] == 17082368, QString("%1").arg(result["n_samples"]).toUtf8());
//    QVERIFY2(result["mean_volume"] == -9.8, QString("%1").arg(result["mean_volume"]).toUtf8());
//    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
//    QVERIFY2(result["histogram_0db"] == 110089, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QCOMPARE(video.isValid(), is_valid);

    // test transcoding
    qInfo() << "transcode video";
    Device *device = video.getStream();
    QVERIFY(device != Q_NULLPTR);

    QScopedPointer<TranscodeProcess> transcodeProcess(qobject_cast<TranscodeProcess*>(device));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    timer.start();
    transcodedSize = 0;
    connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
    connect(transcodeProcess.data(), &TranscodeProcess::sendDataToClientSignal, this, &tst_dlnanetworkvideo::receivedTranscodedData);
    connect(transcodeProcess.data(), &TranscodeProcess::LogMessage, this, &tst_dlnanetworkvideo::LogMessage);
    QVERIFY(transcodeProcess->open());
    emit startTranscoding();
    transcodeProcess->waitForFinished(-1);

    double speed_ratio = static_cast<double>(video.getLengthInMilliSeconds()) / static_cast<double>(timer.elapsed());
    qInfo() << "transcoding done in" << timeToString(timer.elapsed()).toUtf8().constData() << "speed :" << QString::number(speed_ratio, 'f', 2).toUtf8().constData();

    QCOMPARE(transcodeProcess->exitCode(),  0);
    transcodeProcess->requestData(transcodeProcess->bytesAvailable());
    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    qInfo() << "DELTA" << video.size()-transcodedSize << qAbs(double(video.size()-transcodedSize))/video.size();
    QVERIFY(video.size() > transcodedSize);
    QCOMPARE(transcodedSize, transcoded_size);

    QCOMPARE(video.getdlnaOrgOpFlags(), "10");
    QCOMPARE(video.getdlnaOrgPN(), dlna_org_pn);
    QCOMPARE(video.getDlnaContentFeatures(), dlna_content_features);
    QCOMPARE(video.getProtocolInfo(), protocolInfo);

    QVERIFY(video.getAlbumArt().isNull());
    QVERIFY(video.getByteAlbumArt().isNull());

    QVERIFY(res == true);
}

void tst_dlnanetworkvideo::testCase_DlnaNetworkVideo_checkLink_data()
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

void tst_dlnanetworkvideo::testCase_DlnaNetworkVideo_checkLink()
{
    QFETCH(QUrl, url);

    DlnaNetworkVideo video;
    video.setAnalyzeStream(false);

    bool res = false;

    QBENCHMARK
    {
        video.setUrl(url);
        res = video.waitUrl(60000);
    }

    if (!res || !video.isValid())
    {
        if (res && !video.error().isEmpty())
        {
            qInfo() << "VIDEO NOT AVAILABLE" << url << video.error();
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
