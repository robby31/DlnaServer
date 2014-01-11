#ifndef TST_REQUEST_H
#define TST_REQUEST_H

#include <QString>
#include <QtTest>

#include "logger.h"
#include "httpserver.h"
#include "httprange.h"
#include "request.h"
#include "dlnarootfolder.h"
#include "dlnafolder.h"
#include "dlnamusictrack.h"
#include "dlnavideoitem.h"

class TestRequest : public QObject
{
    Q_OBJECT

public:
    TestRequest();

private Q_SLOTS:
    void testCase_request();

//    void testCase_DlnaMusicTrack_MP3_streaming();

};

#endif // TST_REQUEST_H
