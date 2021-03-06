#ifndef TST_DLNANETWORKVIDEO_H
#define TST_DLNANETWORKVIDEO_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "mysqldatabase.h"
#include "dlna/dlnanetworkvideo.h"
#include "dlna/cached/dlnacachedrootfolder.h"

class tst_dlnanetworkvideo : public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnanetworkvideo(QObject *parent = Q_NULLPTR);

private Q_SLOTS:
    void init();
    void cleanup();
    void cleanupTestCase();

    void testCase_DlnaNetworkVideo_data();
    void testCase_DlnaNetworkVideo();

    void testCase_StreamingVideo_data();
    void testCase_StreamingVideo();

    void testCase_DlnaCachedNetworkVideo();

    void testCase_DlnaNetworkVideo_checkLink_data();
    void testCase_DlnaNetworkVideo_checkLink();

private:
    QString timeToString(const qint64 &msec);

private:
    QSqlDatabase db;
    Protocol *m_dlnaProfiles = Q_NULLPTR;

    int media_timeout = 0;
    int media_not_available = 0;
    int media_ko = 0;
    int media_ok = 0;
};

#endif // TST_DLNANETWORKVIDEO_H
