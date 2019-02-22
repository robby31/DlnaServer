#include <iostream>
#include <QApplication>

#include "tst_dlnarootfolder.h"
#include "tst_dlnafolder.h"
#include "tst_dlnamusictrack.h"
#include "tst_dlnavideoitem.h"
#include "tst_dlnayoutubevideo.h"
#include "tst_dlnacachedresources.h"
#include "tst_dlnanetworkvideo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);

    {
        tst_dlnarootfolder test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnafolder test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnacachedresources test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnayoutubevideo test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnanetworkvideo test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnamusictrack test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_dlnavideoitem test;
        QTest::qExec(&test, argc, argv);
    }
}
