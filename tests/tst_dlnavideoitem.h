#ifndef TST_DLNAVIDEOITEM_H
#define TST_DLNAVIDEOITEM_H

#include <QObject>
#include <QtTest>

#include "dlnavideofile.h"

class tst_dlnavideoitem : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnavideoitem(QObject *parent = 0);

signals:

public slots:
    void receivedTranscodedData();
    void transcodingOpened();
    void LogMessage(const QString &message);

private Q_SLOTS:
    void testCase_DlnaVideoItem_AVI_Starwars();
    void testCase_DlnaVideoItem_MKV_Looper();
    void testCase_DlnaVideoItem_AVI();
    void testCase_DlnaVideoItem_MKV();

private:
    TranscodeProcess* transcodeProcess;
    long transcodedSize;
    QElapsedTimer transcodeTimer;
    qint64 timeToOpenTranscoding;
};

#endif // TST_DLNAVIDEOITEM_H
