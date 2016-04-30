#include <iostream>
#include <QCoreApplication>

#include "tst_httprange.h"
#include "tst_dlnarootfolder.h"
#include "tst_dlnafolder.h"
#include "tst_dlnamusictrack.h"
#include "tst_dlnavideoitem.h"
#include "tst_dlnayoutubevideo.h"
#include "tst_request.h"
#include "tst_dlnacachedresources.h"

void executeTest(QObject* test, QStringList* summary, int argc, char *argv[])
{
    if (QTest::qExec(test, argc, argv) == 0)
        summary->append(QString(test->metaObject()->className()) + ": OK");
    else
        summary->append(QString(test->metaObject()->className()) + ": KO");
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QStringList testsSummary;

    tst_httprange test1;
    executeTest(&test1, &testsSummary, argc, argv);

    tst_dlnarootfolder test2;
    executeTest(&test2, &testsSummary, argc, argv);

    tst_dlnafolder test3;
    executeTest(&test3, &testsSummary, argc, argv);

    tst_dlnacachedresources test7;
    executeTest(&test7, &testsSummary, argc, argv);

    TestRequest test6;
    executeTest(&test6, &testsSummary, argc, argv);

    tst_dlnayoutubevideo test5_1;
    executeTest(&test5_1, &testsSummary, argc, argv);

    tst_dlnamusictrack test4;
    executeTest(&test4, &testsSummary, argc, argv);

    tst_dlnavideoitem test5;
    executeTest(&test5, &testsSummary, argc, argv);

    // print the results summary
    std::cout << std::endl;
    std::cout << "Summary:" << std::endl;
    foreach(QString result, testsSummary)
        std::cout << result.toStdString() << std::endl;
}
