#include "tst_dlnamusictrack.h"

tst_dlnamusictrack::tst_dlnamusictrack(QObject *parent) :
    DlnaCheckFunctions(parent)
{

}

void tst_dlnamusictrack::init()
{
    m_dlnaProfiles = new Protocol("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml");
}

void tst_dlnamusictrack::cleanup()
{
    delete m_dlnaProfiles;

    DebugInfo::display_alive_objects();

    QCOMPARE(DebugInfo::count_alive_objects(), 0);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");
    track.setHostUrl(QUrl("http://host:600"));

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X;DLNA.ORG_FLAGS=8D100000000000000000000000000000";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QCOMPARE(track.getSystemName(), "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");

    check_dlna_audio(&track,
                     "", "-1",
                     "On/Off", "Human After All", "Daft Punk", "Daft Punk", "House", 7, "2013-01-02",
                     "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000",
                     "00:00:19", 2, 44100,
                     18751, 376593, "http://host:600/get//07%20On_Off.mp3");

    QCOMPARE(track.mimeType(), "audio/mpeg");
    QCOMPARE(track.format(), MP3);
    QCOMPARE(track.size(), 376593);
    QCOMPARE(track.bitrate(), 150005);
    QCOMPARE(track.getLengthInSeconds(), 19);
    QCOMPARE(track.getLengthInMilliSeconds(), 19382);
    QCOMPARE(track.samplerate(), 44100);
    QCOMPARE(track.channelCount(), 2);
    QVERIFY(!track.toTranscode());

    QCOMPARE(track.getdlnaOrgOpFlags(), "01");
    QCOMPARE(track.getdlnaOrgPN(), "MP3");
    QCOMPARE(track.getDlnaContentFeatures(), "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
    QCOMPARE(track.getProtocolInfo(), "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);

    QCOMPARE(track.metaDataPerformerSort(), "Daft Punk");
    QCOMPARE(track.metaDataAlbumArtist(), "Daft Punk");
    QCOMPARE(track.metaDataYear(), 2005);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 1705200, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -21.3, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == -8, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_7db"] == 1, QString("%1").arg(result["histogram_7db"]).toUtf8());
    //    QVERIFY2(result["histogram_8db"] == 3518, QString("%1").arg(result["histogram_8db"]).toUtf8());

    check_streaming(&track, -1, -1, "RANGE: BYTES=0-", 376593, 376593, 376593, 376593, 5000);

    check_streaming(&track, -1, -1, "RANGE: BYTES=0-5000", 5001, 5001, 5001, 5001, 5000);

    check_streaming(&track, -1, -1, "RANGE: BYTES=1000-5000", 4001, 4001, 4001, 4001, 5000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_with_image()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");
    track.setHostUrl(QUrl("http://host:600"));

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");

    check_dlna_audio(&track,
                     "", "-1",
                     "Funk Ad", "Homework", "Daft Punk", "Daft Punk", "House", 16, "2013-01-02",
                     "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000",
                     "00:00:51", 2, 44100,
                     16000, 845029, "http://host:600/get//16%20Funk%20Ad.mp3");

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 845029);
    QVERIFY(track.bitrate() == 128000);
    QVERIFY2(track.getLengthInSeconds() == 51, QString("%1").arg(track.getLengthInSeconds()).toUtf8());
    QVERIFY2(track.getLengthInMilliSeconds() == 51085, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(!track.toTranscode());

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

    QVERIFY(!track.getAlbumArt().isNull());
    QCOMPARE(track.getAlbumArt().size().width(), 300);
    QCOMPARE(track.getAlbumArt().size().height(), 300);
    int size = track.getByteAlbumArt().size();
    QVERIFY2(size == 16554, QString("%1").arg(size).toUtf8());

    //        QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 4515840, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -15.7, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 18999, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QScopedPointer<Device>stream(qobject_cast<Device*>(track.getStream()));
    QVERIFY(stream != Q_NULLPTR);
    if (stream)
    {
        QVERIFY(stream->open());
        QVERIFY(stream->isOpen());
        QCOMPARE(stream->size(), 845029);
        QCOMPARE(track.size(), stream->size());
        QVERIFY(!stream->atEnd());
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_unicode()
{
    DlnaMusicTrackFile track("/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3");
    track.setHostUrl(QUrl("http://host:600"));

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3");

    check_dlna_audio(&track,
                     "", "-1",
                     "Phébus", "Mister Mystère", "-M-", "-M-", "Rock  Français", 2, "2013-01-02",
                     "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000",
                     "00:02:39", 2, 44100,
                     23657, 3841064, "http://host:600/get//1-02%20Phe%CC%81bus.mp3");

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 3841064);
    QVERIFY2(track.bitrate() == 189255, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 159);
    QVERIFY2(track.getLengthInMilliSeconds() == 159373, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == false);

    QVERIFY(track.getdlnaOrgOpFlags() == "01");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QCOMPARE(track.getAlbumArt().size().width(), 300);
    QCOMPARE(track.getAlbumArt().size().height(), 300);
    QCOMPARE(track.getByteAlbumArt().size(), 5357);

    //        QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 14052024, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.7, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 25653, QString("%1").arg(result["histogram_0db"]).toUtf8());

    check_streaming(&track, -1, -1, "RANGE: BYTES=0-", 3841064, 3841064, 3841064, 3841064, 5000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_Trancoding_LPCM()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(LPCM_S16BE);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/L16:DLNA.ORG_PN=LPCM";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");

    check_dlna_audio(&track,
                     "", "-1",
                     "On/Off", "Human After All", "Daft Punk", "Daft Punk", "House", 7, "2013-01-02",
                     "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:00:19", 2, 44100,
                     176400, 3419018, "http://host:600/get//07%20On_Off.mp3");

    QCOMPARE(track.mimeType(), "audio/L16;rate=44100;channels=2");
    QVERIFY(track.format() == LPCM_S16BE);
    QVERIFY(track.size() == 3419018);
    QVERIFY2(track.bitrate() == 1411200, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 19);
    QVERIFY2(track.getLengthInMilliSeconds() == 19382, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == true);

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 1705200, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -21.3, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == -8, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_7db"] == 1, QString("%1").arg(result["histogram_7db"]).toUtf8());
    //    QVERIFY2(result["histogram_8db"] == 3518, QString("%1").arg(result["histogram_8db"]).toUtf8());

    check_streaming(&track, -1, -1, "", -1, 0, 3419018, 3410400, 6000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_AAC_Transcoding_MP3()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(MP3);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

    check_dlna_audio(&track,
                     "", "-1",
                     "Monde virtuel", "Je dis aime", "-M-", "-M-", "Pop", 1, "2013-01-02",
                     "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:03:09", 2, 44100,
                     40000, 7561480, "http://host:600/get//01%20Monde%20virtuel.m4a");

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 7561480);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 189);
    QVERIFY2(track.getLengthInMilliSeconds() == 188987, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == true);

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getByteAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    int size = track.getByteAlbumArt().size();
    QVERIFY2(size == 22040, QString("%1").arg(size).toUtf8());

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 16664448, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.5, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 28983, QString("%1").arg(result["histogram_0db"]).toUtf8());

    check_streaming(&track, -1, -1, "", -1, 1089, 7561480, 7559880, 7000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_MP3()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(MP3);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QCOMPARE(track.getSystemName(), "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    check_dlna_audio(&track,
                     "", "-1",
                     "test", "", "", "", "", 0, "2013-08-27",
                     "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:09:05", 2, 48000,
                     40000, 21787040, "http://host:600/get//test.wav");

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 21787040);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    check_streaming(&track, -1, -1, "", -1, 1005, 21787040, 21787245, 22000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_LPCM()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));

    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    track.setTranscodeFormat(LPCM_S16BE);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    check_dlna_audio(&track,
                     "", "-1",
                     "test", "", "", "", "", 0, "2013-08-27",
                     "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:09:05", 2, 48000,
                     192000, 104569237, "http://host:600/get//test.wav");

    QVERIFY2(track.mimeType() == "audio/L16;rate=48000;channels=2", track.mimeType().toUtf8());
    QVERIFY(track.format() == LPCM_S16BE);
    QVERIFY(track.size() == 104569237);
    QVERIFY(track.bitrate() == 1536000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    check_streaming(&track, -1, -1, "", -1, 0, 104569237, 104568320, 700);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_AAC()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(AAC);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.toTranscode()==true);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    check_dlna_audio(&track,
                     "", "-1",
                     "test", "", "", "", "", 0, "2013-08-27",
                     "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:09:05", 2, 48000,
                     40000, 22662977, "http://host:600/get//test.wav");

    QVERIFY(track.mimeType() == "audio/mp4");
    QVERIFY(track.format() == AAC);
    QVERIFY(track.size() == 22662977);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QCOMPARE(track.getdlnaOrgPN(), "AAC_ISO");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    check_streaming(&track, -1, -1, "", -1, 40, 22662977, 21941819, 20000);
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_ALAC()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(ALAC);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    track.setDlnaProfiles(m_dlnaProfiles);

    QVERIFY(track.toTranscode()==true);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    check_dlna_audio(&track,
                     "", "-1",
                     "test", "", "", "", "", 0, "2013-08-27",
                     "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                     "00:09:05", 2, 48000,
                     192000, 116321656, "http://host:600/get//test.wav");

    QVERIFY(track.mimeType() == "audio/mp4");
    QVERIFY(track.format() == ALAC);
    QVERIFY(track.size() == 116321656);
    QVERIFY2(track.bitrate() == 1536000, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QCOMPARE(track.getdlnaOrgPN(), "AAC_ISO");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    check_streaming(&track, -1, -1, "", -1, 40, 116321656, 62792391, 20000);
}
