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
    verificationRequest.protocol_version = "HTTP/1.1";
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
    QFETCH(HttpRequest, httpRequestExspected);
    QCOMPARE(parser.getHttpRequest(data), httpRequestExspected);
}

void TestHttpRequest::testGetHttpRequest_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<HttpRequest>("httpRequestExspected");
    HttpRequest verificationRequest;
    HttpRequest verificationNullRequest;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocol_version = "HTTP/1.1";
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
    QFETCH(QByteArray, returnExspected);
    QCOMPARE(parser.formAsHtmlPage(request), returnExspected);
}

void TestHttpRequest::testFormAsHtmlPage_data()
{
    QTest::addColumn<HttpRequest>("request");
    QTest::addColumn<QByteArray>("returnExspected");
    HttpRequest verificationRequest;
    const QByteArray verificationHtmlForm = "Method: GET<br>URI: /<br>Protocol-Version: HTTP/1.1<br><h3>HEADERS:</h3><h3>BODY:</h3><br>";
    QByteArray verificationNullHtmlForm;
    verificationRequest.method = "GET";
    verificationRequest.uri = "/";
    verificationRequest.protocol_version = "HTTP/1.1";
    QTest::newRow("1st test") << verificationRequest << verificationHtmlForm;
    verificationRequest.method = QString();
    QTest::newRow("2nd test") << verificationRequest << verificationNullHtmlForm;
    verificationRequest.method = "GET";
    verificationRequest.uri = QString();
    QTest::newRow("3rd test") << verificationRequest << verificationNullHtmlForm;
    verificationRequest.uri = "/";
    verificationRequest.protocol_version = QString();
    QTest::newRow("4th test") << verificationRequest << verificationNullHtmlForm;

}
