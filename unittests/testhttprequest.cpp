#include <QTest>
#include <QDebug>
#include <QByteArray>
#include "testhttprequest.h"
#include "httprequest.h"

void TestHttpRequest::testParseRequestLine()
{
    HttpRequestParser parser;
    HttpRequest request;
    QFETCH(QByteArray, requestLine);
    QFETCH(HttpRequest, requestExpected);
    parser.parseRequestLine(requestLine, request);
    QCOMPARE(request, requestExpected);
}

void TestHttpRequest::testParseRequestLine_data()
{
    QTest::addColumn<QByteArray>("requestLine");
    QTest::addColumn<HttpRequest>("requestExpected");
    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = "HTTP/1.1";
    QTest::newRow("1st test") << QByteArray("GET / HTTP/1.1") << verificationRequest;
    QTest::newRow("2nd test") << QByteArray("GET") << verificationNullRequest;
    QTest::newRow("3rd test") << QByteArray("GET /") << verificationNullRequest;
    QTest::newRow("4th test") << QByteArray() << verificationNullRequest;
    QTest::newRow("5th test") << QByteArray("Get /** Http rec") << verificationNullRequest;
}

void TestHttpRequest::testParseHeaders()
{
    HttpRequestParser parser;
    HttpRequest request;
    QFETCH(QByteArray, headerLine);
    QFETCH(HttpRequest, headerExpected);
    parser.parseHeaders(headerLine, request);
    QCOMPARE(request, headerExpected);
}

void TestHttpRequest::testParseHeaders_data()
{
    QTest::addColumn<QByteArray>("headerLine");
    QTest::addColumn<HttpRequest>("headerExpected");
    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.headers.insert("Content-Length", "12");
    QTest::newRow("1st test") << QByteArray("\nContent-Length: 12") << verificationRequest;
    QTest::newRow("2nd test") << QByteArray("Content length 12") << verificationNullRequest;
    QTest::newRow("3rd test") << QByteArray("120") << verificationNullRequest;
    QTest::newRow("4th test") << QByteArray() << verificationNullRequest;
    QTest::newRow("5th test") << QByteArray(" : ") << verificationNullRequest;
    QTest::newRow("6th test") << QByteArray(" :") << verificationNullRequest;
}

void TestHttpRequest::testGetHttpRequest()
{
    HttpRequestParser parser;
    QFETCH(QByteArray, data);
    QFETCH(HttpRequest, httpRequestExpected);
    QCOMPARE(parser.getHttpRequest(data), httpRequestExpected);
}

void TestHttpRequest::testGetHttpRequest_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<HttpRequest>("httpRequestExpected");
    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = "HTTP/1.1";
    verificationRequest.headers.insert("Content-Length", "12");
    QTest::newRow("1st test") << QByteArray("GET / HTTP/1.1\r\nContent-Length: 12\r\n\r\n")
                              << verificationRequest;
    QTest::newRow("2nd test") << QByteArray() << verificationNullRequest;
    QTest::newRow("3rd test") << QByteArray("Get") << verificationNullRequest;
    QTest::newRow("4th test") << QByteArray("GET / ") << verificationNullRequest;
    QTest::newRow("5th test") << QByteArray("GET : Http") << verificationNullRequest;
    QTest::newRow("6th test") << QByteArray("\r\n") << verificationNullRequest;
    QTest::newRow("7th test") << QByteArray("\r") << verificationNullRequest;
}

void TestHttpRequest::testFormAsHtmlPage()
{
    HttpRequestParser parser;
    QFETCH(HttpRequest, request);
    QFETCH(QByteArray, returnExpected);
    QCOMPARE(parser.formAsHtmlPage(request), returnExpected);
}

void TestHttpRequest::testFormAsHtmlPage_data()
{
    QTest::addColumn<HttpRequest>("request");
    QTest::addColumn<QByteArray>("returnExpected");
    HttpRequest verificationRequest;
    const QByteArray verificationHtmlForm = "Method: GET<br>URI: /<br>Protocol-Version: HTTP/1.1<br><h3>HEADERS:</h3><h3>BODY:</h3><br>";
    QByteArray verificationNullHtmlForm;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = "HTTP/1.1";
    QTest::newRow("1st test") << verificationRequest << verificationHtmlForm;
    verificationRequest.method = QString();
    QTest::newRow("2nd test") << verificationRequest << verificationNullHtmlForm;
    verificationRequest.method = "GET";
    verificationRequest.uri = QString();
    QTest::newRow("3rd test") << verificationRequest << verificationNullHtmlForm;
    verificationRequest.uri = "/";
    verificationRequest.protocolVersion = QString();
    QTest::newRow("4th test") << verificationRequest << verificationNullHtmlForm;

}

void TestHttpRequest::testIsCorectMethod()
{
    HttpRequestParser parser;
    QFETCH(QByteArray, method);
    QFETCH(bool, returnExpected);
    QCOMPARE(parser.isCorrectMethod(method), returnExpected);
}

void TestHttpRequest::testIsCorectMethod_data()
{
    QTest::addColumn<QByteArray>("method");
    QTest::addColumn<bool>("returnExpected");
    QTest::newRow("1st test") << QByteArray("POST") << true;
    QTest::newRow("2nd test") << QByteArray("post") << false;
    QTest::newRow("3rd test") << QByteArray("DeLeTe") << false;
    QTest::newRow("4th test") << QByteArray("GEt") << false;
    QTest::newRow("5th test") << QByteArray("method") << false;
    QTest::newRow("6th test") << QByteArray("CONNECT") << true;
    QTest::newRow("7th test") << QByteArray("OPTIONS") << true;
    QTest::newRow("8th test") << QByteArray() << false;
}

void TestHttpRequest::testRequestMessageHeaderSize()
{
    HttpRequestParser parser;
    QFETCH(HttpRequest, request);
    QFETCH(int, returnExpected);
    QCOMPARE(parser.requestMessageHeaderSize(request), returnExpected);
}

void TestHttpRequest::testRequestMessageHeaderSize_data()
{
    QTest::addColumn<HttpRequest>("request");
    QTest::addColumn<int>("returnExpected");
    HttpRequest verificationRequest;
    QTest::newRow("1st test") << verificationRequest << 0;
    verificationRequest.method = "POST";
    QTest::newRow("2nd test") << verificationRequest << 0;
    verificationRequest.uri = "/";
    QTest::newRow("3nd test") << verificationRequest << 0;
    verificationRequest.protocolVersion = "HTTP/1.1";
    QTest::newRow("4th test") << verificationRequest << QString("POST / HTTP/1.1\r\n\r\n").count();
    verificationRequest.headers.insert("Content-Length", "10");
    verificationRequest.body = "AABBCCDDEE";
    QString request = "POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\n";
    QTest::newRow("5th test") << verificationRequest << request.count();
    verificationRequest.headers.insert("Host", "http://utechcorp.com/");
    request += "Host: http://utechcorp.com/\r\n";
    QTest::newRow("6th test") << verificationRequest << request.count();
}
