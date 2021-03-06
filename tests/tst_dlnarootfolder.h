#ifndef TST_DLNAROOTFOLDER_H
#define TST_DLNAROOTFOLDER_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "dlna/dlnarootfolder.h"

class tst_dlnarootfolder : public DlnaCheckFunctions
{
    Q_OBJECT

public:
    explicit tst_dlnarootfolder(QObject *parent = Q_NULLPTR);

private Q_SLOTS:
    void init();
    void cleanup();

    void testCase_DlnaRootFolder();

private:
    Protocol *m_dlnaProfiles = Q_NULLPTR;
};

#endif // TST_DLNAROOTFOLDER_H
