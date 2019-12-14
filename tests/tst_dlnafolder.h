#ifndef TST_DLNAFOLDER_H
#define TST_DLNAFOLDER_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "analyzer.h"
#include "dlna/dlnafolder.h"
#include "dlna/dlnanetworkplaylist.h"

class tst_dlnafolder : public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnafolder(QObject *parent = Q_NULLPTR);

private Q_SLOTS:
    void init();
    void cleanup();

    void testCase_DlnaFolderPlaylist();

    void testCase_DlnaFolder();
    void testCase_PerformanceAllArtists();
    void testCase_PerformanceAllAlbums();
    void testCase_PerformanceAllTracks();

private:
    qint64 parseFolder(const QString &resourceId, DlnaResource *resource);

    Protocol *m_dlnaProfiles = Q_NULLPTR;
};

#endif // TST_DLNAFOLDER_H
