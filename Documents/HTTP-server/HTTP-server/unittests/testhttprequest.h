#ifndef TESTHTTPREQUEST_H
#define TESTHTTPREQUEST_H

#include <QObject>

class TestHttpRequest : public QObject
{
    Q_OBJECT

private slots:
    void testParseRequestLine();
    void testParseRequestLine_data();

    void testParseHeaders();
    void testParseHeaders_data();

    void testGetHttpRequest();
    void testGetHttpRequest_data();

    void testFormAsHtmlPage();
    void testFormAsHtmlPage_data();
};

#endif // TESTHTTPREQUEST_H
