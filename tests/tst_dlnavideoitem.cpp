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

    check_streaming(&movie, -1, -1, "", 0, 0, movie.size(), 6872799848, 7000, false);
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

    check_streaming(&movie, -1, -1, "", 0, 0, movie.size(), 5159838224, 450000);
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

    check_streaming(&movie, -1, -1, "", 0, 0, movie.size(), 4198362044, 400000, false);
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
                     "District.9.2009.720p.BrRip.YIFY",
                     1);

    check_dlna_video_res(&movie, 0,
                         "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                         "01:52:16", "1280x688", 2, 48000,
                         569850, 4163526283, "http://host:600/get/content?id=&amp;format=");

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

    // test partial transcoding (10 seconds) in constant bitrate
    check_streaming(&movie, -1, 10, "", 0, 0, 6181162, 6115828, 10000, false);

    // test partial transcoding (10 seconds) in variable bitrate
    check_streaming(&movie, -1, 10, "", 0, 0, 6181162, 1827548, 10000, true);

    // test full transcoding in constant bitrate
    check_streaming(&movie, -1, -1, "", 0, 0, movie.size(), 4165712084, 10000, false);
}
