#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QMapIterator>
#include <QByteArrayList>
#include <QByteArrayListIterator>
struct HttpRequest {
    QString method_;
    QString uri_;
    QString protocol_version_;
    //QString content_type_;
    //QString content_length_;
    QMap<QString, QString> headers_;
    QByteArray body_;

    HttpRequest(QByteArray request);
    ~HttpRequest();

    QByteArray formAsHtmlPage();
    void parseRequestLine(QByteArray request_line);
    bool parseHeaders(QByteArray header_line);
    void parseBody(QByteArray body_line);
    QString findHeaderValue(QString key);
};

#endif // HTTPREQUEST_H
