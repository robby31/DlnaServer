#ifndef TST_DLNAFOLDER_H
#define TST_DLNAFOLDER_H

#include <QObject>
#include <QtTest>

#include "dlnafolder.h"

class tst_dlnafolder : public QObject
{
    Q_OBJECT
public:
    explicit tst_dlnafolder(QObject *parent = 0);

signals:

public slots:

private Q_SLOTS:
    void testCase_DlnaFolder();

};

#endif // TST_DLNAFOLDER_H
