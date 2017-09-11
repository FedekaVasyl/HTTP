#include "httprequest.h"
#include <QByteArrayList>
#include <QMapIterator>
#include <QDebug>
#include <QMetaEnum>

HttpRequestParser::HttpRequestParser(QObject *parent) : QObject(parent)
{
}

HttpRequestParser::~HttpRequestParser()
{
}

QByteArray HttpRequestParser::formAsHtmlPage(const HttpRequest &request)
{
    QByteArray htmlPage;
    if (request.method.isNull()
            || request.uri.isNull()
            || request.protocolVersion.isNull()) {
        qDebug() << "This is not HTTP request!";
        return htmlPage;
    }
    QMapIterator<QString, QString> mapIterator(request.headers);
    const QString kNewLine = "<br>";
    const QString kSep = ": ";
    const QString kMethod = "Method: ";
    const QString kUri = "URI: ";
    const QString kProtocolVersion = "Protocol-Version: ";
    const QString kHeaders = "<h3>HEADERS:</h3>";
    const QString kBody = "<h3>BODY:</h3>";
    htmlPage.clear();
    htmlPage += kMethod + request.method + kNewLine;
    htmlPage += kUri + request.uri + kNewLine;
    htmlPage += kProtocolVersion + request.protocolVersion + kNewLine;
    htmlPage += kHeaders;
    while (mapIterator.hasNext()) {
        mapIterator.next();
        htmlPage += mapIterator.key() + kSep
                + mapIterator.value() + kNewLine;
    }
    htmlPage += kBody + kNewLine;
    htmlPage += request.body;
    return htmlPage;
}

void HttpRequestParser::parseRequestLine(const QByteArray &requestLine,
                                         HttpRequest &request)
{
    QByteArrayList list = requestLine.split(' ');
    if (list.count() == 3) {
        if (!isCorrectMethod(list[0]))
            return;
        request.method = list[0];
        request.uri = list[1];
        request.protocolVersion = list[2];
    }
}

bool HttpRequestParser::parseHeaders(const QByteArray &headerLine,
                                     HttpRequest &request)
{
    int count = headerLine.count();
    if (!headerLine.contains(':') || count <= 3)
        return false;
    QString headerName;
    QString headerValue;
    const bool kRun = true;
    int i = 1;
    while (kRun) {
        if (i == count || (i + 1) == count)
            return false;
        if(headerLine[i] == ':' && headerLine[i + 1] == ' ')
            break;
        headerName[i - 1] = headerLine[i];
        ++i;
    }
    headerValue = headerLine.mid(i + 2);
    request.headers.insert(headerName, headerValue);
    return true;
}

bool HttpRequestParser::isCorrectMethod(const QByteArray &method)
{
    if (method.isNull())
        return false;
    QMetaEnum metaEnumMethod = QMetaEnum::fromType<HttpRequestParser::method>();
    if (!metaEnumMethod.isValid())
        return false;
    int i = 0;
    int count = metaEnumMethod.keyCount();

    while (i != count || count == 0) {
        if (QByteArray(metaEnumMethod.valueToKey(i)) == method)
            return true;
        i++;
    }
    return false;
}

HttpRequest HttpRequestParser::getHttpRequest(const QByteArray &data)
{
    HttpRequest request;
    if (data.isNull())
        return request;
    QByteArrayList requestList = data.split('\r');
    int i = 1;
    if (requestList.count() > 1) {
        parseRequestLine(requestList[0], request);
        while (parseHeaders(requestList[i], request))
            ++i;
    }
    return request;
}

bool operator == (const HttpRequest &left, const HttpRequest &right)
{
    if (left.body != right.body
            || left.method != right.method
            || left.protocolVersion != right.protocolVersion
            || left.uri != right.uri
            || left.headers.count() != right.headers.count())
        return false;
    QMap<QString, QString>::const_iterator leftIterator = left.headers.begin();
    while (leftIterator != left.headers.end()) {
        if (!right.headers.contains(leftIterator.key())
                || right.headers.value(leftIterator.key()) != leftIterator.value())
            return false;
        ++leftIterator;
    }
    return true;
}

int HttpRequestParser::requestMessageHeaderSize(const HttpRequest &request)
{
    const int kCRLF = 2;    //CR - Carriage return (\r), LF - Linefeed (\n)
    const int kSP = 1;      //SP - Space (' ')
    const int kCL = 1;      //CL - Colon (':');
    int size = 0;
    if (request.method.isNull() || request.uri.isNull() || request.protocolVersion.isNull())
        return size;
    size = request.method.size() + kSP
            + request.uri.size() + kSP
            + request.protocolVersion.size() + kCRLF;
    QMapIterator<QString, QString> mapIterator(request.headers);
    while (mapIterator.hasNext()) {
        mapIterator.next();
        size += mapIterator.key().size() + kCL + kSP
                + mapIterator.value().size() + kCRLF;
    }
    size += kCRLF;
    return size;
}
