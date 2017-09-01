#include "httprequest.h"

HttpRequest::HttpRequest(QByteArray request)
{
    QByteArrayList request_list = request.split('\r');
    int i = 1;
    if (request_list.count() == 0) {
        this->body_ = "";
        this->method_ = "";
        this->uri_ = "";
        this->protocol_version_ = "";
    } else {
        parseRequestLine(request_list[0]);
        parseBody(request_list[request_list.count() - 1]);
        while (parseHeaders(request_list[i])) {
            i++;
        }
    }
}

HttpRequest::~HttpRequest()
{
}

QByteArray HttpRequest::formAsHtmlPage()
{
    QByteArray html_page;
    QMapIterator<QString, QString> map_iterator(headers_);
    const QString kNewLine = "<br>";
    const QString kSep = ": ";
    const QString kMethod = "Method: ";
    const QString kUri = "URI: ";
    const QString kProtocolVersion = "Protocol-Version: ";
    const QString kHeaders = "<h3>HEADERS:</h3>";
    const QString kBody = "<h3>BODY:</h3>";
    html_page.clear();
    html_page += kMethod + method_ + kNewLine;
    html_page += kUri + uri_ + kNewLine;
    html_page += kProtocolVersion + protocol_version_ + kNewLine;
    html_page += kHeaders;
    while (map_iterator.hasNext()) {
        map_iterator.next();
        html_page += map_iterator.key() + kSep +
                     map_iterator.value() + kNewLine;
    }
    html_page += kBody + kNewLine;
    html_page += body_;
    return html_page;
}

void HttpRequest::parseRequestLine(QByteArray request_line)
{
    QByteArrayList list = request_line.split(' ');
    if (list.count() != 3) {
        this->method_ = "";
        this->uri_ = "";
        this->protocol_version_ = "";
    } else {
        this->method_ = list[0];
        this->uri_ = list[1];
        this->protocol_version_ = list[2];
    }
}

bool HttpRequest::parseHeaders(QByteArray header_line)
{
    if (header_line.count() == 1) {
        return false;
    }
    QString header_name;
    QString header_value;
    int i = 1;
    while (header_line[i] != ':' && header_line[i + 1] != ' ') {
        header_name[i - 1] = header_line[i];
        i++;
    }
    i++;
    header_value = header_line.mid(i+1);
    headers_.insert(header_name, header_value);
    return true;
}

QString HttpRequest::findHeaderValue(QString key)
{
    QMap<QString, QString>::iterator iter = headers_.find(key);
    if (iter == headers_.end()) {
        return NULL;
    }
    QString value = iter.value();
    return value;
}

void HttpRequest::parseBody(QByteArray body_line)
{
    if (body_line.count() == 1) {
        this->body_ = "";
    } else {
        body_ = body_line.mid(0);
    }
}
