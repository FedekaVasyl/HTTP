#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QMetaType>

struct HttpRequest {
    QString method;
    QString uri;
    QString protocol_version;
    QMap<QString, QString> headers;
    QByteArray body;
    friend bool operator == (const HttpRequest &left, const HttpRequest &right);
};

Q_DECLARE_METATYPE(HttpRequest)

class HttpRequestParser {
private:

    void parseRequestLine(const QByteArray &request_line, HttpRequest &request);
    bool parseHeaders(const QByteArray &header_line, HttpRequest &request);
public:
    HttpRequestParser();
    ~HttpRequestParser();

    QByteArray formAsHtmlPage(const HttpRequest &request);
    HttpRequest getHttpRequest(const QByteArray &data);

    friend class TestHttpRequest;
};

#endif // HTTPREQUEST_H
