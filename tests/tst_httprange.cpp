#include "tst_httprange.h"

tst_httprange::tst_httprange(QObject *parent) :
    QObject(parent)
{
}

void tst_httprange::testCase_httprange() {
    HttpRange* range = 0;

    // range null
    range = new HttpRange();
    QVERIFY(range->isNull() == true);
    QVERIFY(range->getUnit() == "");
    QVERIFY(range->getLowRange() == 0);
    QVERIFY(range->getHighRange() == 0);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(500);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);
    delete range;

    // from 10 to 500
    range = new HttpRange("RANGE: BYTES=10-500");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 10);
    QVERIFY(range->getHighRange() == 500);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(1000);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 500);
    QVERIFY(range->getLength() == 491);

    range->setSize(0);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(3);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 2);
    QVERIFY(range->getLength() == -1);

    range->setSize(9);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 8);
    QVERIFY(range->getLength() == -1);

    range->setSize(10);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 9);
    QVERIFY(range->getLength() == -1);

    range->setSize(11);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 10);
    QVERIFY(range->getLength() == 1);

    range->setSize(400);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 399);
    QVERIFY(range->getLength() == 390);

    range->setSize(499);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 498);
    QVERIFY(range->getLength() == 489);

    range->setSize(500);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 499);
    QVERIFY(range->getLength() == 490);

    range->setSize(501);
    QVERIFY(range->getStartByte() == 10);
    QVERIFY(range->getEndByte() == 500);
    QVERIFY(range->getLength() == 491);
    delete range;

    // from 20 to the end
    range = new HttpRange("RANGE: BYTES=20-");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 20);
    QVERIFY(range->getHighRange() == -1);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(1000);
    QVERIFY(range->getStartByte() == 20);
    QVERIFY(range->getEndByte() == 999);
    QVERIFY(range->getLength() == 980);

    range->setSize(3);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 2);
    QVERIFY(range->getLength() == -1);

    range->setSize(10);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 9);
    QVERIFY(range->getLength() == -1);

    range->setSize(19);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 18);
    QVERIFY(range->getLength() == -1);

    range->setSize(20);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 19);
    QVERIFY(range->getLength() == -1);

    range->setSize(21);
    QVERIFY(range->getStartByte() == 20);
    QVERIFY(range->getEndByte() == 20);
    QVERIFY(range->getLength() == 1);
    delete range;

    // the final 900 bytes
    range = new HttpRange("RANGE: BYTES=-900");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == -1);
    QVERIFY(range->getHighRange() == 900);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(1000);
    QVERIFY(range->getStartByte() == 100);
    QVERIFY(range->getEndByte() == 999);
    QVERIFY(range->getLength() == 900);

    range->setSize(3);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 2);
    QVERIFY(range->getLength() == 3);

    range->setSize(10);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 9);
    QVERIFY(range->getLength() == 10);

    range->setSize(100);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 99);
    QVERIFY(range->getLength() == 100);
    delete range;

    // the first byte
    range = new HttpRange("RANGE: BYTES=0-0");
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 0);
    QVERIFY(range->getHighRange() == 0);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(1000);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 0);
    QVERIFY(range->getLength() == 1);

    range->setSize(3);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 0);
    QVERIFY(range->getLength() == 1);

    range->setSize(10);
    QVERIFY(range->getStartByte() == 0);
    QVERIFY(range->getEndByte() == 0);
    QVERIFY(range->getLength() == 1);
    delete range;

    // from 110 to 5000
    range = new HttpRange("RANGE: BYTES=110-5000", this);
    QVERIFY(range->isNull() == false);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == 110);
    QVERIFY(range->getHighRange() == 5000);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(1000);
    QVERIFY(range->getStartByte() == 110);
    QVERIFY(range->getEndByte() == 999);
    QVERIFY(range->getLength() == 890);

    range->setSize(3);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 2);
    QVERIFY(range->getLength() == -1);

    range->setSize(10);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == 9);
    QVERIFY(range->getLength() == -1);
    delete range;

    // invalid range
    range = new HttpRange("RANGE: BYTES=500-10");
    QVERIFY(range->isNull() == true);
    QVERIFY(range->getUnit() == "BYTES");
    QVERIFY(range->getLowRange() == -1);
    QVERIFY(range->getHighRange() == -1);
    QVERIFY(range->getSize() == -1);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);

    range->setSize(500);
    QVERIFY(range->getStartByte() == -1);
    QVERIFY(range->getEndByte() == -1);
    QVERIFY(range->getLength() == -1);
    delete range;
}
