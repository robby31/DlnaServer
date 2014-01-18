#ifndef TST_DLNAVIDEOITEM_H
#define TST_DLNAVIDEOITEM_H

#include <QObject>
#include <QtTest>

#include "dlnavideoitem.h"

class tst_dlnavideoitem : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnavideoitem(QObject *parent = 0);

signals:

public slots:
    void receivedTranscodedData();

private Q_SLOTS:
    void testCase_DlnaVideoItem_AVI_Starwars();
    void testCase_DlnaVideoItem_MKV_Looper();
    void testCase_DlnaVideoItem_AVI();
    void testCase_DlnaVideoItem_MKV();

private:
    QProcess* transcodeProcess;
    long transcodedSize;
};

#endif // TST_DLNAVIDEOITEM_H
