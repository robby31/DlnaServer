#ifndef TST_DLNAMUSICTRACK_H
#define TST_DLNAMUSICTRACK_H

#include <QObject>
#include <QtTest>

#include "dlna_check_functions.h"
#include "dlna/dlnamusictrackfile.h"

class tst_dlnamusictrack : public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnamusictrack(QObject *parent = Q_NULLPTR);

private Q_SLOTS:
    void init();
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
    Protocol *m_dlnaProfiles = Q_NULLPTR;
};

#endif // TST_DLNAMUSICTRACK_H
