#ifndef TST_DLNAROOTFOLDER_H
#define TST_DLNAROOTFOLDER_H

#include <QObject>
#include <QtTest>
#include "dlna_check_functions.h"

#include "dlna/dlnarootfolder.h"

class tst_dlnarootfolder : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnarootfolder(QObject *parent = 0);

signals:

public slots:

private Q_SLOTS:
    void testCase_DlnaRootFolder();

private:
    Protocol m_dlnaProfiles;
};

#endif // TST_DLNAROOTFOLDER_H
