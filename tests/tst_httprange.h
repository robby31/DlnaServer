#ifndef TST_HTTPRANGE_H
#define TST_HTTPRANGE_H

#include <QObject>
#include <QtTest>

#include "httprange.h"

class tst_httprange : public QObject
{
    Q_OBJECT
public:
    explicit tst_httprange(QObject *parent = 0);

signals:

public slots:

private Q_SLOTS:
    void testCase_httprange();

};

#endif // TST_HTTPRANGE_H
