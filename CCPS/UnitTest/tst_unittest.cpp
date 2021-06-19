#include <QtTest/QtTest>

#include "E:/qt/YPGame/CCPS/functions.h"


class UnitTest : public QObject
{
    Q_OBJECT

private slots:
    void test_case1();
};

void UnitTest::test_case1()
{
    QString result = parser("testCaseLogin",  "testCasePassword");
    QVERIFY(result.toStdString() == "auth&testCaseLogin&testCasePassword");
}

QTEST_MAIN(UnitTest)
#include "tst_unittest.moc"
