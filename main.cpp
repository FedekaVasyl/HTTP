#include <QCoreApplication>
#include <QTest>
#include <server.h>
#include <unittests/testhttprequest.h>
#include <unittests/testmysocket.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    TestHttpRequest testHttpRequest;
    TestMySocket testMySocket;
    QTest::qExec(&testHttpRequest, argc, argv);
    QTest::qExec(&testMySocket, argc, argv);
    return a.exec();
}
