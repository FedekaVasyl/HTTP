#include <QTest>
#include <QByteArray>
#include <QTcpSocket>
#include <QIODevice>
#include "testmysocket.h"
#include "mysocket.h"

void TestMySocket::testWriteToSocket()
{
    QFETCH(QTcpSocket*, qTcpSocket);
    QFETCH(HttpRequest, request);
    QFETCH(bool, returnExspected);
    MySocket *mySocket = new MySocket(NULL,qTcpSocket);
    mySocket->setRequest(request);
    qTcpSocket->open(QIODevice::ReadWrite);
    QCOMPARE(mySocket->writeToSocket(), returnExspected);
    delete qTcpSocket;
    delete mySocket;
}

void TestMySocket::testWriteToSocket_data()
{
    QTest::addColumn<QTcpSocket*>("qTcpSocket");
    QTest::addColumn<HttpRequest>("request");
    QTest::addColumn<bool>("returnExspected");

    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocol_version = "HTTP/1.1";
    verificationRequest.headers.insert("Content-Length", "12");

    QTest::newRow("1st test") << new QTcpSocket() << verificationRequest << true;
    QTest::newRow("2nd test") << new QTcpSocket() << verificationNullRequest << false;
}
