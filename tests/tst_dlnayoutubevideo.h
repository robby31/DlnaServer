#ifndef TST_DLNAYOUTUBEVIDEO_H
#define TST_DLNAYOUTUBEVIDEO_H

#include <QObject>
#include <QtTest>

#include "mysqldatabase.h"
#include "dlnayoutubevideo.h"
#include "cached/dlnacachednetworkvideo.h"
#include "cached/dlnacachedrootfolder.h"

class tst_dlnayoutubevideo : public QObject
{
    Q_OBJECT

public:
    explicit tst_dlnayoutubevideo(QObject *parent = 0);

signals:
    void bytesSent(const qint64 &size, const qint64 &towrite);
    void startTranscoding();

public slots:
    void receivedTranscodedData(const QByteArray &data);

private Q_SLOTS:
    void testCase_DlnaYouTubeVideo();
    void testCase_DlnaYouTubeVideo_HD();
    void testCase_DlnaYouTubeVideo_HD_NotFound();
    void testCase_DlnaYouTubeVideo_MPEG2();
    void testCase_DlnaYouTubeVideo_MPEG4();
    void testCase_DlnaYouTubeVideo3();
    void testCase_DlnaYouTubeVideo4();


    void testCase_DlnaCachedNetworkVideo();
    void testCase_DlnaCachedNetworkVideo_checkLink();

private:
    TranscodeProcess *transcodeProcess;
    long transcodedSize;
    QThread *backend;
    QSqlDatabase db;
    QNetworkAccessManager *manager;
};

#endif // TST_DLNAYOUTUBEVIDEO_H
