#include <QTest>
#include <QByteArray>
#include <QTcpSocket>
#include <QIODevice>
#include <QThread>
#include "testmysocket.h"
#include "mysocket.h"

void TestMySocket::testWriteToSocket()
{
    QFETCH(QTcpSocket*, qTcpSocket);
    QFETCH(HttpRequest, request);
    QFETCH(bool, returnExpected);
    MySocket *mySocket = new MySocket(NULL, qTcpSocket);
    mySocket->setRequest(request);
    qTcpSocket->open(QIODevice::ReadWrite);
    QCOMPARE(mySocket->writeToSocket(), returnExpected);
    mySocket->getSocket()->close();
    delete qTcpSocket;
    delete mySocket;
}

void TestMySocket::testWriteToSocket_data()
{
    QTest::addColumn<QTcpSocket*>("qTcpSocket");
    QTest::addColumn<HttpRequest>("request");
    QTest::addColumn<bool>("returnExpected");
    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = "HTTP/1.1";
    verificationRequest.headers.insert("Content-Length", "12");
    QTest::newRow("1st test") << new QTcpSocket() << verificationRequest << true;
    QTest::newRow("2nd test") << new QTcpSocket() << verificationNullRequest << false;
}

void TestMySocket::testReadyRead()
{
    QFETCH(QTcpSocket*, qTcpSocket);
    QFETCH(int, waitTime);
    QFETCH(QByteArray, writeRequest);
    QFETCH(HttpRequest, requestExpected);
    MySocket *mySocket = new MySocket(NULL, qTcpSocket);
    const int kWait = 1000;
    if (waitTime > 0) {
        mySocket->getSocket()->write(writeRequest.left(5));
        mySocket->getSocket()->waitForBytesWritten(kWait);
        this->thread()->sleep(waitTime);
        mySocket->getSocket()->write(writeRequest.right(writeRequest.size() - 5));
        mySocket->getSocket()->waitForBytesWritten(kWait);
    } else {
        mySocket->getSocket()->write(writeRequest);
        mySocket->getSocket()->waitForBytesWritten(kWait);
    }
    QCOMPARE(mySocket->getRequest(), requestExpected);
    delete qTcpSocket;
    delete mySocket;
}

void TestMySocket::testReadyRead_data()
{
    QTest::addColumn<QTcpSocket*>("qTcpSocket");
    QTest::addColumn<int>("waitTime");
    QTest::addColumn<QByteArray>("writeRequest");
    QTest::addColumn<HttpRequest>("requestExpected");
    HttpRequest verificationNullRequest;
    HttpRequest verificationRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = "HTTP/1.1";
    verificationRequest.headers.insert("Content-Length", "12");
    verificationRequest.body = "AABBCCDDEEFF";
    QTcpSocket *socketFirst = new QTcpSocket();
    QTcpSocket *socketSecond = new QTcpSocket();
    QTcpSocket *socketThird = new QTcpSocket();
    QTcpSocket *socketFourth = new QTcpSocket();
    setSocketOption(socketFirst, QString("127.0.0.24"), 7819);
    setSocketOption(socketSecond, QString("127.0.0.25"), 7820);
    setSocketOption(socketThird, QString("127.0.0.26"), 7821);
    setSocketOption(socketFourth, QString("127.0.0.27"), 7822);
    QTest::newRow("1st test") << socketFirst << 0
                              << QByteArray("GET / HTTP/1.1\r\nContent-Length: 12\r\n\r\nAABBCCDDEEFF")
                              << verificationRequest;
    QTest::newRow("2nd test") << socketSecond << 0
                              << QByteArray()
                              << verificationNullRequest;
    QTest::newRow("3rd test") << socketThird << 5
                              << QByteArray("GET / HTTP/1.1\r\nContent-Length: 12\r\n\r\nAABBCCDDEEFF")
                              << verificationRequest;
    QTest::newRow("4th test") << socketFourth << 15
                              << QByteArray()
                              << verificationNullRequest;

}

void TestMySocket::setSocketOption(QTcpSocket *socket, const QString &address, const int &port)
{
    socket->open(QIODevice::ReadWrite);
    QHostAddress addr(address);
    if (!socket->bind(addr, port, QAbstractSocket::ShareAddress)) {
        qDebug() << "bind() failed!";
        qDebug() << socket->error();
    }
    socket->connectToHost(addr, port, QIODevice::ReadWrite);
    if (!socket->waitForConnected(10000)) {
        qDebug() << "waitForConnection() failed!";
        qDebug() << socket->error();
    }
    if (!socket->isOpen()) {
        qDebug() << "open() failed!";
        socket->error();
    }
}
