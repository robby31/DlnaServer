#include "tst_dlnayoutubevideo.h"

tst_dlnayoutubevideo::tst_dlnayoutubevideo(QObject *parent) :
    QObject(parent)
{
}

void tst_dlnayoutubevideo::testCase_DlnaYouTubeVideo()
{
    Logger log;
    DlnaYouTubeVideo video(&log, QUrl("http://www.youtube.com/watch?v=fiore9Z5iUg"), "host", 600);
    QVERIFY(video.getSystemName() == "http://www.youtube.com/watch?v=fiore9Z5iUg");
    QVERIFY(video.getName() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)");
    QVERIFY(video.getDisplayName() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)");

    QVERIFY(video.metaDataTitle() == "Lilly Wood & The Prick and Robin Schulz - Prayer In C (Robin Schulz Remix) (Official)");
    QVERIFY(video.metaDataDuration() == 193540);
    QVERIFY(video.resolution() == "1280x720");
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
}
