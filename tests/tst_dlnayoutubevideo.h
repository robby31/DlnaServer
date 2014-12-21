#ifndef TST_DLNAYOUTUBEVIDEO_H
#define TST_DLNAYOUTUBEVIDEO_H

#include <QObject>
#include <QtTest>

#include "dlnayoutubevideo.h"

class tst_dlnayoutubevideo : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnayoutubevideo(QObject *parent = 0);

signals:

public slots:
    void receivedTranscodedData();

private Q_SLOTS:
    void testCase_DlnaYouTubeVideo();
    void testCase_DlnaYouTubeVideo2();
    void testCase_DlnaYouTubeVideo3();

private:
    TranscodeProcess* transcodeProcess;
    long transcodedSize;
};

#endif // TST_DLNAYOUTUBEVIDEO_H
