#ifndef TST_DLNAFOLDER_H
#define TST_DLNAFOLDER_H

#include <QObject>
#include <QtTest>

#include "analyzer.h"
#include "dlna/dlnafolder.h"

class tst_dlnafolder : public QObject
{
    Q_OBJECT

public:
    explicit tst_dlnafolder(QObject *parent = 0);

signals:

public slots:

private Q_SLOTS:
    void cleanup();

    void testCase_DlnaFolder();
    void testCase_PerformanceAllArtists();
    void testCase_PerformanceAllAlbums();
    void testCase_PerformanceAllTracks();

private:
    int parseFolder(const QString& resourceId, DlnaResource* resource);

};

#endif // TST_DLNAFOLDER_H
