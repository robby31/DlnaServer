#ifndef TST_DLNAMUSICTRACK_H
#define TST_DLNAMUSICTRACK_H

#include <QObject>
#include <QtTest>

#include "dlnamusictrackfile.h"
#include "request.h"
#include <QElapsedTimer>

class tst_dlnamusictrack : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnamusictrack(QObject *parent = 0);

signals:

private slots:
    void receivedTranscodedData();
    void transcodingOpened();

private Q_SLOTS:
    void testCase_DlnaMusicTrack_MP3();
    void testCase_DlnaMusicTrack_MP3_with_image();
    void testCase_DlnaMusicTrack_MP3_unicode();

    void testCase_DlnaMusicTrack_AAC_Transcoding_MP3();

    void testCase_DlnaMusicTrack_WAV_Transcoding_MP3();
    void testCase_DlnaMusicTrack_WAV_Transcoding_LPCM();
    void testCase_DlnaMusicTrack_WAV_Transcoding_AAC();

private:
    TranscodeProcess* transcodeProcess;
    qint64 transcodedBytes;
    QElapsedTimer transcodeTimer;
    qint64 timeToOpenTranscoding;
};

#endif // TST_DLNAMUSICTRACK_H
