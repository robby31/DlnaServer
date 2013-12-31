#ifndef TST_REQUEST_H
#define TST_REQUEST_H

#include <QString>
#include <QtTest>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "logger.h"
#include "httpserver.h"
#include "httprange.h"
#include "request.h"
#include "dlnarootfolder.h"
#include "dlnafolder.h"
#include "dlnamusictrack.h"

class TestRequest : public QObject
{
    Q_OBJECT

public:
    TestRequest();

private slots:
    void receivedTranscodedData();

private Q_SLOTS:
    void testCase_server();
    void testCase_request();
    void testCase_httprange();
    void testCase_DlnaRootFolder();
    void testCase_DlnaFolder();
    void testCase_DlnaMusicTrack_AAC();
    void testCase_DlnaMusicTrack_MP3();
    void testCase_DlnaMusicTrack_MP3_with_image();
    void testCase_DlnaMusicTrack_WAV();

private:
    QProcess* transcodeProcess;
    QByteArray transcodedBytes;
};

#endif // TST_REQUEST_H
