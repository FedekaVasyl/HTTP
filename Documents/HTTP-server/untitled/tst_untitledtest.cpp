#include <QString>
#include <QtTest>
#include <QCoreApplication>

class UntitledTest : public QObject
{
    Q_OBJECT

public:
    UntitledTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();
};

UntitledTest::UntitledTest()
{
}

void UntitledTest::initTestCase()
{
}

void UntitledTest::cleanupTestCase()
{
}

void UntitledTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void UntitledTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(UntitledTest)

#include "tst_untitledtest.moc"
