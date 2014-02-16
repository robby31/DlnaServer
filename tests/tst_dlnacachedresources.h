#ifndef TST_DLNACACHEDRESOURCES_H
#define TST_DLNACACHEDRESOURCES_H

#include <QObject>
#include <QtTest>

#include "cached/dlnacachedrootfolder.h"
#include "cached/dlnacachedfolder.h"

class tst_dlnacachedresources: public QObject
{
    Q_OBJECT

public:
    explicit tst_dlnacachedresources(QObject *parent = 0);

private slots:
    void receivedTranscodedData();

private Q_SLOTS:
    void testCase_DlnaCachedRootFolder();
    void testCase_DlnaCachedMusicTrack();
    void testCase_DlnaCachedVideo();

private:
    QProcess* transcodeProcess;
    long transcodedSize;
};

#endif // TST_DLNACACHEDRESOURCES_H
