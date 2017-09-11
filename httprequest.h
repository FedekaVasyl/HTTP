#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QObject>

struct HttpRequest {
    QString method;
    QString uri;
    QString protocolVersion;
    QMap<QString, QString> headers;
    QByteArray body;
    friend bool operator == (const HttpRequest &left, const HttpRequest &right);
};

Q_DECLARE_METATYPE(HttpRequest)

class HttpRequestParser : public QObject
{
    Q_OBJECT

public:
    enum method {OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT};
    Q_ENUM(method)

    HttpRequestParser(QObject *parent = 0);
    ~HttpRequestParser();

    QByteArray formAsHtmlPage(const HttpRequest &request);
    HttpRequest getHttpRequest(const QByteArray &data);
    int requestMessageHeaderSize(const HttpRequest &request);

    friend class TestHttpRequest;

private:

    void parseRequestLine(const QByteArray &requestLine, HttpRequest &request);
    bool parseHeaders(const QByteArray &headerLine, HttpRequest &request);
    bool isCorrectMethod(const QByteArray &method);
};

#endif // HTTPREQUEST_H
