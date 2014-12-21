#include "tst_dlnayoutubevideo.h"

tst_dlnayoutubevideo::tst_dlnayoutubevideo(QObject *parent) :
    QObject(parent)
{
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
    DlnaYouTubeVideo video(&log, QUrl("http://www.youtube.com/watch?v=fiore9Z5iUg"), "host", 600);
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=fiore9Z5iUg");
    QVERIFY2(video.getName() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)", video.getName().toUtf8().constData());
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)");
    QVERIFY(video.metaDataDuration() == 193540);
    QVERIFY(video.resolution() == "1280x720");
    QVERIFY(video.bitrate() == 5718800);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo2()
{
    Logger log;
    DlnaYouTubeVideo video(&log, QUrl("http://www.youtube.com/watch?v=04QzovLe2JM"), "host", 600);
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=04QzovLe2JM");
    QVERIFY(video.getName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video.getDisplayName() == "Muse - Exogenesis- Symphony Part 1 (Overture)");

    QVERIFY(video.metaDataTitle() == "Muse - Exogenesis- Symphony Part 1 (Overture)");
    QVERIFY(video.metaDataDuration() == 258460);
    QVERIFY(video.resolution() == "638x360");
    QVERIFY(video.bitrate() == 5718800);
    QVERIFY(video.toTranscode() == true);
    QVERIFY2(video.getLengthInMilliSeconds()==258460, QString("%1").arg(video.getLengthInMilliSeconds()).toUtf8());
    QVERIFY2(video.size()==184760131, QString("%1").arg(video.size()).toUtf8());
    QVERIFY2(video.framerate() == "25.000", video.framerate().toUtf8());

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
    QVERIFY2(transcodedSize == 184610548, QString("transcoded size = %1").arg(transcodedSize).toUtf8());
    delete transcodeProcess;
    transcodeProcess = 0;

    QVERIFY(video.getdlnaOrgOpFlags() == "10");
    QVERIFY(video.getdlnaOrgPN() == "MPEG_PS_PAL");
    QVERIFY(video.getDlnaContentFeatures() == "DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY2(video.getProtocolInfo() == "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL;DLNA.ORG_OP=10", video.getProtocolInfo().toUtf8());

    QVERIFY(video.getAlbumArt().isNull() == true);
    QVERIFY(video.getByteAlbumArt().isNull() == true);
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo3()
{
    Logger log;
    DlnaYouTubeVideo video(&log, QUrl("http://www.youtube.com/watch?v=cXxwIZwYnok"), "host", 600);
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=cXxwIZwYnok");
    QVERIFY(video.getName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick - Let's Not Pretend [Clip Officiel]");
    QVERIFY(video.metaDataDuration() == 220170);
    QVERIFY(video.resolution() == "1280x720");
    QVERIFY(video.bitrate() == 5718800);
}
