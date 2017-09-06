#include "httprequest.h"
#include <QByteArrayList>
#include <QMapIterator>
#include <QDebug>

HttpRequestParser::HttpRequestParser()
{
}

HttpRequestParser::~HttpRequestParser()
{
}

QByteArray HttpRequestParser::formAsHtmlPage(const HttpRequest &request)
{
    QByteArray html_page;
    if (request.method.isNull()
            || request.uri.isNull()
            || request.protocol_version.isNull()) {
        qDebug() << "This is not HTTP request!";
        return html_page;
    }
    QMapIterator<QString, QString> map_iterator(request.headers);
    const QString kNewLine = "<br>";
    const QString kSep = ": ";
    const QString kMethod = "Method: ";
    const QString kUri = "URI: ";
    const QString kProtocolVersion = "Protocol-Version: ";
    const QString kHeaders = "<h3>HEADERS:</h3>";
    const QString kBody = "<h3>BODY:</h3>";
    html_page.clear();
    html_page += kMethod + request.method + kNewLine;
    html_page += kUri + request.uri + kNewLine;
    html_page += kProtocolVersion + request.protocol_version + kNewLine;
    html_page += kHeaders;
    while (map_iterator.hasNext()) {
        map_iterator.next();
        html_page += map_iterator.key() + kSep +
                     map_iterator.value() + kNewLine;
    }
    html_page += kBody + kNewLine;
    html_page += request.body;
    return html_page;
}

void HttpRequestParser::parseRequestLine(const QByteArray &request_line,
                                         HttpRequest &request)
{
    QByteArrayList list = request_line.split(' ');
    if (list.count() == 3) {
        request.method = list[0];
        request.uri = list[1];
        request.protocol_version = list[2];
    }
}

bool HttpRequestParser::parseHeaders(const QByteArray &header_line,
                                     HttpRequest &request)
{
    int count = header_line.count();
    if (!header_line.contains(':') || count <= 3)
        return false;
    QString header_name;
    QString header_value;
    const bool kRun = true;
    int i = 1;
    while (kRun) {
        if (i == count || (i + 1) == count)
            return false;
        if(header_line[i] == ':' && header_line[i + 1] == ' ')
            break;
        header_name[i - 1] = header_line[i];
        ++i;
    }
    header_value = header_line.mid(i + 2);
    request.headers.insert(header_name, header_value);
    return true;
}

HttpRequest HttpRequestParser::getHttpRequest(const QByteArray &data)
{
    HttpRequest request;
    if (data.isNull())
        return request;
    QByteArrayList request_list = data.split('\r');
    int i = 1;
    if (request_list.count() > 1) {
        parseRequestLine(request_list[0], request);
        while (parseHeaders(request_list[i], request))
            ++i;
    }
    return request;
}

bool operator == (const HttpRequest &left, const HttpRequest &right)
{
    if (left.body != right.body
            || left.method != right.method
            || left.protocol_version != right.protocol_version
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
