#ifndef TST_DLNAVIDEOITEM_H
#define TST_DLNAVIDEOITEM_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "dlna/dlnavideofile.h"

class tst_dlnavideoitem : public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnavideoitem(QObject *parent = Q_NULLPTR);

signals:
    void startTranscoding();

public slots:
    void dataAvailable();
    void receivedTranscodedData(const QByteArray &data);
    void transcodingOpened();
    void LogMessage(const QString &message);

private slots:
    void init();
    void cleanup();

    void testCase_DlnaVideoItem_MKV_MPEG2_AC3();
    void testCase_DlnaVideoItem_MKV_Looper_MPEG2_AC3();
    void testCase_DlnaVideoItem_AVI_Starwars_MPEG4_AAC();
    void testCase_DlnaVideoItem_AVI_Starwars_MPEG2_AC3();

private:
    long transcodedSize = 0;
    QElapsedTimer transcodeTimer;
    qint64 timeToOpenTranscoding = 0;
    Protocol *m_dlnaProfiles = Q_NULLPTR;
    QStringList m_sinkProtocols;
};

#endif // TST_DLNAVIDEOITEM_H
