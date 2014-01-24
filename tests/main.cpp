#include "tst_httprange.h"
#include "tst_dlnarootfolder.h"
#include "tst_dlnafolder.h"
#include "tst_dlnamusictrack.h"
#include "tst_dlnavideoitem.h"
#include "tst_request.h"

int main(int argc, char *argv[])
{
    tst_httprange test1;
    QTest::qExec(&test1, argc, argv);

    tst_dlnarootfolder test2;
    QTest::qExec(&test2, argc, argv);

    tst_dlnafolder test3;
    QTest::qExec(&test3, argc, argv);

    tst_dlnamusictrack test4;
    QTest::qExec(&test4, argc, argv);

    tst_dlnavideoitem test5;
    QTest::qExec(&test5, argc, argv);

    TestRequest test6;
    QTest::qExec(&test6, argc, argv);
}
