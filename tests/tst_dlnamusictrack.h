#ifndef TST_DLNAMUSICTRACK_H
#define TST_DLNAMUSICTRACK_H

#include <QObject>
#include <QtTest>

#include "dlna/dlnamusictrackfile.h"
#include <QElapsedTimer>
#include "Http/httprange.h"

class tst_dlnamusictrack : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnamusictrack(QObject *parent = Q_NULLPTR);

signals:
    void startTranscoding();
    void requestData(const qint64 &maxlen);

public slots:
    void receivedTranscodedData(const QByteArray &data);
    void dataAvailable();
    void transcodingOpened();
    void LogMessage(const QString &message);

private Q_SLOTS:
    void cleanup();

    void testCase_DlnaMusicTrack_MP3();
    void testCase_DlnaMusicTrack_MP3_with_image();
    void testCase_DlnaMusicTrack_MP3_unicode();
    void testCase_DlnaMusicTrack_MP3_Trancoding_LPCM();

    void testCase_DlnaMusicTrack_AAC_Transcoding_MP3();

    void testCase_DlnaMusicTrack_WAV_Transcoding_MP3();
    void testCase_DlnaMusicTrack_WAV_Transcoding_LPCM();
    void testCase_DlnaMusicTrack_WAV_Transcoding_AAC();
    void testCase_DlnaMusicTrack_WAV_Transcoding_ALAC();

private:
    QThread *m_streamingThread = Q_NULLPTR;
    qint64 transcodedBytes;
    QElapsedTimer transcodeTimer;
    qint64 timeToOpenTranscoding;
    Protocol m_dlnaProfiles;
};

#endif // TST_DLNAMUSICTRACK_H
